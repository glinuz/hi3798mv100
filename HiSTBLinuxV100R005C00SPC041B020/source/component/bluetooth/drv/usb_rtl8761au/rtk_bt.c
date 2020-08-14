/*
 *
 *  Realtek Bluetooth USB driver
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/usb.h>
#include <linux/dcache.h>
#include <net/sock.h>
#include <asm/unaligned.h>

#include "rtk_bt.h"

#define VERSION "3.1"

#ifdef BTCOEX
#include "rtk_coex.h"
extern rtk_usb_coexistence_info usb_coex_info;
#endif

static uint8_t gEVersion = 0xFF;

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 7, 1)
static bool reset = 0;
#endif

static struct usb_driver btusb_driver;
static struct usb_device_id btusb_table[] = { {
	.match_flags = USB_DEVICE_ID_MATCH_VENDOR |
		USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor = 0x0bda,
	.bInterfaceClass = 0xe0,
	.bInterfaceSubClass = 0x01,
	.bInterfaceProtocol = 0x01 }, {

	.match_flags = USB_DEVICE_ID_MATCH_VENDOR |
		USB_DEVICE_ID_MATCH_INT_INFO,
	.idVendor = 0x13d3,
	.bInterfaceClass = 0xe0,
	.bInterfaceSubClass = 0x01,
	.bInterfaceProtocol = 0x01}, {
	}
};

static void rtk_free(struct btusb_data *data)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 1)
	kfree(data);
#endif
	return;
}

static struct btusb_data *rtk_alloc(struct usb_interface *intf)
{
	struct btusb_data *data;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 7, 1)
	data = kzalloc(sizeof(*data), GFP_KERNEL);
#else
	data = devm_kzalloc(&intf->dev, sizeof(*data), GFP_KERNEL);
#endif
	return data;
}

MODULE_DEVICE_TABLE(usb, btusb_table);

static int inc_tx(struct btusb_data *data)
{
	unsigned long flags;
	int rv;

	spin_lock_irqsave(&data->txlock, flags);
	rv = test_bit(BTUSB_SUSPENDING, &data->flags);
	if (!rv)
		data->tx_in_flight++;
	spin_unlock_irqrestore(&data->txlock, flags);

	return rv;
}

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
static inline void btusb_free_frags(struct btusb_data *data)
{
	unsigned long flags;

	spin_lock_irqsave(&data->rxlock, flags);

	kfree_skb(data->evt_skb);
	data->evt_skb = NULL;

	kfree_skb(data->acl_skb);
	data->acl_skb = NULL;

	kfree_skb(data->sco_skb);
	data->sco_skb = NULL;

	spin_unlock_irqrestore(&data->rxlock, flags);
}

static int btusb_recv_intr(struct btusb_data *data, void *buffer, int count)
{
	struct sk_buff *skb;
	int err = 0;

	spin_lock(&data->rxlock);
	skb = data->evt_skb;

	while (count) {
		int len;

		if (!skb) {
			skb = bt_skb_alloc(HCI_MAX_EVENT_SIZE, GFP_ATOMIC);
			if (!skb) {
				err = -ENOMEM;
				break;
			}

			bt_cb(skb)->pkt_type = HCI_EVENT_PKT;
			bt_cb(skb)->expect = HCI_EVENT_HDR_SIZE;
		}

		len = min_t(uint, bt_cb(skb)->expect, count);
		memcpy(skb_put(skb, len), buffer, len);

		count -= len;
		buffer += len;
		bt_cb(skb)->expect -= len;

		if (skb->len == HCI_EVENT_HDR_SIZE) {
			/* Complete event header */
			bt_cb(skb)->expect = hci_event_hdr(skb)->plen;

			if (skb_tailroom(skb) < bt_cb(skb)->expect) {
				kfree_skb(skb);
				skb = NULL;

				err = -EILSEQ;
				break;
			}
		}

		if (bt_cb(skb)->expect == 0) {
			/* Complete frame */
			hci_recv_frame(data->hdev, skb);
			skb = NULL;
		}
	}

	data->evt_skb = skb;
	spin_unlock(&data->rxlock);

	return err;
}

static int btusb_recv_bulk(struct btusb_data *data, void *buffer, int count)
{
	struct sk_buff *skb;
	int err = 0;

	spin_lock(&data->rxlock);
	skb = data->acl_skb;

	while (count) {
		int len;

		if (!skb) {
			skb = bt_skb_alloc(HCI_MAX_FRAME_SIZE, GFP_ATOMIC);
			if (!skb) {
				err = -ENOMEM;
				break;
			}

			bt_cb(skb)->pkt_type = HCI_ACLDATA_PKT;
			bt_cb(skb)->expect = HCI_ACL_HDR_SIZE;
		}

		len = min_t(uint, bt_cb(skb)->expect, count);
		memcpy(skb_put(skb, len), buffer, len);

		count -= len;
		buffer += len;
		bt_cb(skb)->expect -= len;

		if (skb->len == HCI_ACL_HDR_SIZE) {
			__le16 dlen = hci_acl_hdr(skb)->dlen;

			/* Complete ACL header */
			bt_cb(skb)->expect = __le16_to_cpu(dlen);

			if (skb_tailroom(skb) < bt_cb(skb)->expect) {
				kfree_skb(skb);
				skb = NULL;

				err = -EILSEQ;
				break;
			}
		}

		if (bt_cb(skb)->expect == 0) {
			/* Complete frame */
			hci_recv_frame(data->hdev, skb);
			skb = NULL;
		}
	}

	data->acl_skb = skb;
	spin_unlock(&data->rxlock);

	return err;
}

static int btusb_recv_isoc(struct btusb_data *data, void *buffer, int count)
{
	struct sk_buff *skb;
	int err = 0;

	spin_lock(&data->rxlock);
	skb = data->sco_skb;

	while (count) {
		int len;

		if (!skb) {
			skb = bt_skb_alloc(HCI_MAX_SCO_SIZE, GFP_ATOMIC);
			if (!skb) {
				err = -ENOMEM;
				break;
			}

			bt_cb(skb)->pkt_type = HCI_SCODATA_PKT;
			bt_cb(skb)->expect = HCI_SCO_HDR_SIZE;
		}

		len = min_t(uint, bt_cb(skb)->expect, count);
		memcpy(skb_put(skb, len), buffer, len);

		count -= len;
		buffer += len;
		bt_cb(skb)->expect -= len;

		if (skb->len == HCI_SCO_HDR_SIZE) {
			/* Complete SCO header */
			bt_cb(skb)->expect = hci_sco_hdr(skb)->dlen;

			if (skb_tailroom(skb) < bt_cb(skb)->expect) {
				kfree_skb(skb);
				skb = NULL;

				err = -EILSEQ;
				break;
			}
		}

		if (bt_cb(skb)->expect == 0) {
			/* Complete frame */
			hci_recv_frame(data->hdev, skb);
			skb = NULL;
		}
	}

	data->sco_skb = skb;
	spin_unlock(&data->rxlock);

	return err;
}
#endif

static void btusb_intr_complete(struct urb *urb)
{
	struct hci_dev *hdev = urb->context;
	struct btusb_data *data = GET_DRV_DATA(hdev);
	int err;

	//RTKBT_DBG("%s: urb %p status %d count %d ", __func__,
	//urb, urb->status, urb->actual_length);

	if (!test_bit(HCI_RUNNING, &hdev->flags))
		return;

	if (urb->status == 0) {
		hdev->stat.byte_rx += urb->actual_length;

#ifdef BTCOEX
		rtk_usb_parse_event(urb);
#endif
#if HCI_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
		if (hci_recv_fragment(hdev, HCI_EVENT_PKT,
				      urb->transfer_buffer,
				      urb->actual_length) < 0) {
			RTKBT_ERR("%s: Corrupted event packet", __func__);
			hdev->stat.err_rx++;
		}
#else
		if (btusb_recv_intr(data, urb->transfer_buffer,
				    urb->actual_length) < 0) {
			RTKBT_ERR("%s corrupted event packet", hdev->name);
			hdev->stat.err_rx++;
		}
#endif
	}
	/* Avoid suspend failed when usb_kill_urb */
	else if (urb->status == -ENOENT) {
		return;
	}

	if (!test_bit(BTUSB_INTR_RUNNING, &data->flags))
		return;

	usb_mark_last_busy(data->udev);
	usb_anchor_urb(urb, &data->intr_anchor);

	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
		/* -EPERM: urb is being killed;
		 * -ENODEV: device got disconnected */
		if (err != -EPERM && err != -ENODEV)
			RTKBT_ERR("%s: Failed to re-submit urb %p, err %d",
				  __func__, urb, err);
		usb_unanchor_urb(urb);
	}
}

static int btusb_submit_intr_urb(struct hci_dev *hdev, gfp_t mem_flags)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	struct urb *urb;
	unsigned char *buf;
	unsigned int pipe;
	int err, size;

	//RTKBT_DBG("%s", hdev->name);

	if (!data->intr_ep)
		return -ENODEV;

	urb = usb_alloc_urb(0, mem_flags);
	if (!urb)
		return -ENOMEM;

	size = le16_to_cpu(data->intr_ep->wMaxPacketSize);

	buf = kmalloc(size, mem_flags);
	if (!buf) {
		usb_free_urb(urb);
		return -ENOMEM;
	}

	pipe = usb_rcvintpipe(data->udev, data->intr_ep->bEndpointAddress);

	usb_fill_int_urb(urb, data->udev, pipe, buf, size,
			 btusb_intr_complete, hdev, data->intr_ep->bInterval);

	urb->transfer_flags |= URB_FREE_BUFFER;

	usb_anchor_urb(urb, &data->intr_anchor);

	err = usb_submit_urb(urb, mem_flags);
	if (err < 0) {
		RTKBT_ERR
		    ("btusb_submit_intr_urb %s urb %p submission failed (%d)",
		     hdev->name, urb, -err);
		usb_unanchor_urb(urb);
	}

	usb_free_urb(urb);

	return err;
}

static void btusb_bulk_complete(struct urb *urb)
{
	struct hci_dev *hdev = urb->context;
	struct btusb_data *data = GET_DRV_DATA(hdev);
	int err;

	//RTKBT_DBG("%s: urb %p status %d count %d",
	//__func__, urb, urb->status, urb->actual_length);

	if (!test_bit(HCI_RUNNING, &hdev->flags))
		return;

#ifdef BTCOEX
	if (urb->status == 0)
		rtk_usb_parse_l2cap_data_rx(urb);
#endif

	if (urb->status == 0) {
		hdev->stat.byte_rx += urb->actual_length;

#if HCI_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
		if (hci_recv_fragment(hdev, HCI_ACLDATA_PKT,
				      urb->transfer_buffer,
				      urb->actual_length) < 0) {
			RTKBT_ERR("%s: Corrupted ACL packet", __func__);
			hdev->stat.err_rx++;
		}
#else
		if (data->recv_bulk(data, urb->transfer_buffer,
				    urb->actual_length) < 0) {
			RTKBT_ERR("%s corrupted ACL packet", hdev->name);
			hdev->stat.err_rx++;
		}
#endif
	}
	/* Avoid suspend failed when usb_kill_urb */
	else if (urb->status == -ENOENT) {
		return;
	}

	if (!test_bit(BTUSB_BULK_RUNNING, &data->flags))
		return;

	usb_anchor_urb(urb, &data->bulk_anchor);
	usb_mark_last_busy(data->udev);

	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
		/* -EPERM: urb is being killed;
		 * -ENODEV: device got disconnected */
		if (err != -EPERM && err != -ENODEV)
			RTKBT_ERR
			    ("btusb_bulk_complete %s urb %p failed to resubmit (%d)",
			     hdev->name, urb, -err);
		usb_unanchor_urb(urb);
	}
}

static int btusb_submit_bulk_urb(struct hci_dev *hdev, gfp_t mem_flags)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	struct urb *urb;
	unsigned char *buf;
	unsigned int pipe;
	int err, size = HCI_MAX_FRAME_SIZE;

	//RTKBT_DBG("%s: hdev name %s", __func__, hdev->name);

	if (!data->bulk_rx_ep)
		return -ENODEV;

	urb = usb_alloc_urb(0, mem_flags);
	if (!urb)
		return -ENOMEM;

	buf = kmalloc(size, mem_flags);
	if (!buf) {
		usb_free_urb(urb);
		return -ENOMEM;
	}

	pipe = usb_rcvbulkpipe(data->udev, data->bulk_rx_ep->bEndpointAddress);

	usb_fill_bulk_urb(urb, data->udev, pipe,
			  buf, size, btusb_bulk_complete, hdev);

	urb->transfer_flags |= URB_FREE_BUFFER;

	usb_mark_last_busy(data->udev);
	usb_anchor_urb(urb, &data->bulk_anchor);

	err = usb_submit_urb(urb, mem_flags);
	if (err < 0) {
		RTKBT_ERR("%s: Failed to submit urb %p, err %d", __func__, urb,
			  err);
		usb_unanchor_urb(urb);
	}

	usb_free_urb(urb);

	return err;
}

static void btusb_isoc_complete(struct urb *urb)
{
	struct hci_dev *hdev = urb->context;
	struct btusb_data *data = GET_DRV_DATA(hdev);
	int i, err;

	/*
	   RTKBT_DBG("%s urb %p status %d count %d", hdev->name,
	   urb, urb->status, urb->actual_length);
	 */
	if (!test_bit(HCI_RUNNING, &hdev->flags))
		return;

	if (urb->status == 0) {
		for (i = 0; i < urb->number_of_packets; i++) {
			unsigned int offset = urb->iso_frame_desc[i].offset;
			unsigned int length =
			    urb->iso_frame_desc[i].actual_length;

			if (urb->iso_frame_desc[i].status)
				continue;

			hdev->stat.byte_rx += length;

#if HCI_VERSION_CODE < KERNEL_VERSION(3, 18, 0)
			if (hci_recv_fragment(hdev, HCI_SCODATA_PKT,
					      urb->transfer_buffer + offset,
					      length) < 0) {
				RTKBT_ERR("%s: Corrupted SCO packet", __func__);
				hdev->stat.err_rx++;
			}
#else
			if (btusb_recv_isoc(data, urb->transfer_buffer + offset,
					    length) < 0) {
				RTKBT_ERR("%s corrupted SCO packet",
					  hdev->name);
				hdev->stat.err_rx++;
			}
#endif
		}
	}
	/* Avoid suspend failed when usb_kill_urb */
	else if (urb->status == -ENOENT) {
		return;
	}

	if (!test_bit(BTUSB_ISOC_RUNNING, &data->flags))
		return;

	usb_anchor_urb(urb, &data->isoc_anchor);
	i = 0;
retry:
	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
		/* -EPERM: urb is being killed;
		 * -ENODEV: device got disconnected */
		if (err != -EPERM && err != -ENODEV)
			RTKBT_ERR
			    ("%s: Failed to re-sumbit urb %p, retry %d, err %d",
			     __func__, urb, i, err);
		if (i < 10) {
			i++;
			mdelay(1);
			goto retry;
		}

		usb_unanchor_urb(urb);
	}
}

static inline void __fill_isoc_descriptor(struct urb *urb, int len, int mtu)
{
	int i, offset = 0;

	//RTKBT_DBG("len %d mtu %d", len, mtu);

	for (i = 0; i < BTUSB_MAX_ISOC_FRAMES && len >= mtu;
	     i++, offset += mtu, len -= mtu) {
		urb->iso_frame_desc[i].offset = offset;
		urb->iso_frame_desc[i].length = mtu;
	}

	if (len && i < BTUSB_MAX_ISOC_FRAMES) {
		urb->iso_frame_desc[i].offset = offset;
		urb->iso_frame_desc[i].length = len;
		i++;
	}

	urb->number_of_packets = i;
}

static int btusb_submit_isoc_urb(struct hci_dev *hdev, gfp_t mem_flags)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	struct urb *urb;
	unsigned char *buf;
	unsigned int pipe;
	int err, size;

	//RTKBT_DBG("%s", hdev->name);

	if (!data->isoc_rx_ep)
		return -ENODEV;

	urb = usb_alloc_urb(BTUSB_MAX_ISOC_FRAMES, mem_flags);
	if (!urb)
		return -ENOMEM;

	size = le16_to_cpu(data->isoc_rx_ep->wMaxPacketSize) *
	    BTUSB_MAX_ISOC_FRAMES;

	buf = kmalloc(size, mem_flags);
	if (!buf) {
		usb_free_urb(urb);
		return -ENOMEM;
	}

	pipe = usb_rcvisocpipe(data->udev, data->isoc_rx_ep->bEndpointAddress);

	urb->dev = data->udev;
	urb->pipe = pipe;
	urb->context = hdev;
	urb->complete = btusb_isoc_complete;
	urb->interval = data->isoc_rx_ep->bInterval;

	urb->transfer_flags = URB_FREE_BUFFER | URB_ISO_ASAP;
	urb->transfer_buffer = buf;
	urb->transfer_buffer_length = size;

	__fill_isoc_descriptor(urb, size,
			       le16_to_cpu(data->isoc_rx_ep->wMaxPacketSize));

	usb_anchor_urb(urb, &data->isoc_anchor);

	err = usb_submit_urb(urb, mem_flags);
	if (err < 0) {
		RTKBT_ERR("%s %s urb %p submission failed (%d)",
			  __func__, hdev->name, urb, err);
		usb_unanchor_urb(urb);
	}

	usb_free_urb(urb);

	return err;
}

static void btusb_tx_complete(struct urb *urb)
{
	struct sk_buff *skb = urb->context;
	struct hci_dev *hdev = (struct hci_dev *)skb->dev;
	struct btusb_data *data = GET_DRV_DATA(hdev);

//      RTKBT_DBG("btusb_tx_complete %s urb %p status %d count %d", hdev->name,
//                                      urb, urb->status, urb->actual_length);

	if (!test_bit(HCI_RUNNING, &hdev->flags))
		goto done;

	if (!urb->status)
		hdev->stat.byte_tx += urb->transfer_buffer_length;
	else
		hdev->stat.err_tx++;

done:
	spin_lock(&data->txlock);
	data->tx_in_flight--;
	spin_unlock(&data->txlock);

	kfree(urb->setup_packet);

	kfree_skb(skb);
}

static void btusb_isoc_tx_complete(struct urb *urb)
{
	struct sk_buff *skb = urb->context;
	struct hci_dev *hdev = (struct hci_dev *)skb->dev;

	//RTKBT_DBG("%s: urb %p status %d count %d",
	//__func__, urb, urb->status, urb->actual_length);

	if (skb && hdev) {
		if (!test_bit(HCI_RUNNING, &hdev->flags))
			goto done;

		if (!urb->status)
			hdev->stat.byte_tx += urb->transfer_buffer_length;
		else
			hdev->stat.err_tx++;
	} else
		RTKBT_ERR("%s: skb 0x%p hdev 0x%p", __func__, skb, hdev);

done:
	kfree(urb->setup_packet);

	kfree_skb(skb);
}

static int btusb_open(struct hci_dev *hdev)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	int err;

	err = usb_autopm_get_interface(data->intf);
	if (err < 0)
		return err;

	data->intf->needs_remote_wakeup = 1;
	RTKBT_DBG("%s start pm_usage_cnt(0x%x)", __FUNCTION__,
		  atomic_read(&(data->intf->pm_usage_cnt)));

	/*******************************/
	if (0 == atomic_read(&hdev->promisc)) {
		RTKBT_ERR("btusb_open hdev->promisc ==0");
		err = -1;
		//goto failed;
	}
	err = download_patch(data->intf);
	if (err < 0)
		goto failed;
	/*******************************/

	if (test_and_set_bit(HCI_RUNNING, &hdev->flags))
		goto done;

	if (test_and_set_bit(BTUSB_INTR_RUNNING, &data->flags))
		goto done;

	err = btusb_submit_intr_urb(hdev, GFP_KERNEL);
	if (err < 0)
		goto failed;

	err = btusb_submit_bulk_urb(hdev, GFP_KERNEL);
	if (err < 0) {
		mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
		usb_kill_anchored_urbs(&data->intr_anchor);
		goto failed;
	}

	set_bit(BTUSB_BULK_RUNNING, &data->flags);
	btusb_submit_bulk_urb(hdev, GFP_KERNEL);

done:
	usb_autopm_put_interface(data->intf);

#ifdef BTCOEX
	rtk_usb_coex_open(hdev);
#endif
	RTKBT_DBG("%s end  pm_usage_cnt(0x%x)", __FUNCTION__,
		  atomic_read(&(data->intf->pm_usage_cnt)));

	return 0;

failed:
	clear_bit(BTUSB_INTR_RUNNING, &data->flags);
	clear_bit(HCI_RUNNING, &hdev->flags);
	usb_autopm_put_interface(data->intf);
	RTKBT_ERR("%s failed  pm_usage_cnt(0x%x)", __FUNCTION__,
		  atomic_read(&(data->intf->pm_usage_cnt)));
	return err;
}

static void btusb_stop_traffic(struct btusb_data *data)
{
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_kill_anchored_urbs(&data->intr_anchor);
	usb_kill_anchored_urbs(&data->bulk_anchor);
	usb_kill_anchored_urbs(&data->isoc_anchor);
}

static int btusb_close(struct hci_dev *hdev)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	int err;

#if HCI_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
	int i;
#endif

	/* When in kernel 4.4.0 and greater, the HCI_RUNNING bit is
	 * cleared in hci_dev_do_close(). */
#if HCI_VERSION_CODE < KERNEL_VERSION(4, 4, 0)
	if (!test_and_clear_bit(HCI_RUNNING, &hdev->flags))
		return 0;
#else
	if (test_bit(HCI_RUNNING, &hdev->flags)) {
		RTKBT_ERR("HCI_RUNNING is not cleared before.");
		return -1;
	}
#endif

	RTKBT_DBG("btusb_close");
#if HCI_VERSION_CODE < KERNEL_VERSION(4, 1, 0)
	/*******************************/
	for (i = 0; i < NUM_REASSEMBLY; i++) {
		if (hdev->reassembly[i]) {
			kfree_skb(hdev->reassembly[i]);
			hdev->reassembly[i] = NULL;
			RTKBT_DBG("%s free ressembly i=%d", __FUNCTION__, i);
		}
	}
	/*******************************/
#endif
	cancel_work_sync(&data->work);
	cancel_work_sync(&data->waker);

	clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
	clear_bit(BTUSB_BULK_RUNNING, &data->flags);
	clear_bit(BTUSB_INTR_RUNNING, &data->flags);

	btusb_stop_traffic(data);
#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
	btusb_free_frags(data);
#endif

	err = usb_autopm_get_interface(data->intf);
	if (err < 0)
		goto failed;

	data->intf->needs_remote_wakeup = 0;
	usb_autopm_put_interface(data->intf);

#ifdef BTCOEX
	rtk_usb_coex_close();
#endif

failed:
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_scuttle_anchored_urbs(&data->deferred);
	return 0;
}

static int btusb_flush(struct hci_dev *hdev)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);

	RTKBT_DBG("%s add delay ", __FUNCTION__);
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_kill_anchored_urbs(&data->tx_anchor);
#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
	btusb_free_frags(data);
#endif

	return 0;
}

const char pkt_ind[][8] = {
	[HCI_COMMAND_PKT] = "cmd",
	[HCI_ACLDATA_PKT] = "acl",
	[HCI_SCODATA_PKT] = "sco",
};

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 13, 0)
int btusb_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
#else
int btusb_send_frame(struct sk_buff *skb)
{
	struct hci_dev *hdev = (struct hci_dev *)skb->dev;
#endif

	struct btusb_data *data = GET_DRV_DATA(hdev);
	struct usb_ctrlrequest *dr;
	struct urb *urb;
	unsigned int pipe;
	int err;

	//RTKBT_DBG("%s", hdev->name);

	if (!test_bit(HCI_RUNNING, &hdev->flags)) {
		/* If the parameter is wrong, the hdev isn't the correct
		 * one. Then no HCI commands can be sent.
		 * This issue is related to the wrong HCI_VERSION_CODE set */
		RTKBT_ERR("HCI is not running");
		return -EBUSY;
	}

	/* Before kernel/hci version 3.13.0, the skb->dev is set before
	 * entering btusb_send_frame(). So there is no need to set it here.
	 *
	 * The skb->dev will be used in the callbacks when urb transfer
	 * completes. See btusb_tx_complete() and btusb_isoc_tx_complete() */
#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 13, 0)
	skb->dev = (void *)hdev;
#endif

	switch (bt_cb(skb)->pkt_type) {
	case HCI_COMMAND_PKT:
		print_command(skb);

#ifdef BTCOEX
		rtk_usb_parse_cmd(skb);
#endif
		urb = usb_alloc_urb(0, GFP_ATOMIC);
		if (!urb)
			return -ENOMEM;

		dr = kmalloc(sizeof(*dr), GFP_ATOMIC);
		if (!dr) {
			usb_free_urb(urb);
			return -ENOMEM;
		}

		dr->bRequestType = data->cmdreq_type;
		dr->bRequest = 0;
		dr->wIndex = 0;
		dr->wValue = 0;
		dr->wLength = __cpu_to_le16(skb->len);

		pipe = usb_sndctrlpipe(data->udev, 0x00);

		usb_fill_control_urb(urb, data->udev, pipe, (void *)dr,
				     skb->data, skb->len, btusb_tx_complete,
				     skb);

		hdev->stat.cmd_tx++;
		break;

	case HCI_ACLDATA_PKT:
		print_acl(skb, 1);
#ifdef BTCOEX
		rtk_usb_parse_l2cap_data_tx(skb);
#endif
		if (!data->bulk_tx_ep)
			return -ENODEV;

		urb = usb_alloc_urb(0, GFP_ATOMIC);
		if (!urb)
			return -ENOMEM;

		pipe = usb_sndbulkpipe(data->udev,
				       data->bulk_tx_ep->bEndpointAddress);

		usb_fill_bulk_urb(urb, data->udev, pipe,
				  skb->data, skb->len, btusb_tx_complete, skb);

		hdev->stat.acl_tx++;
		break;

	case HCI_SCODATA_PKT:
		if (!data->isoc_tx_ep || SCO_NUM < 1)
			return -ENODEV;

		urb = usb_alloc_urb(BTUSB_MAX_ISOC_FRAMES, GFP_ATOMIC);
		if (!urb)
			return -ENOMEM;

		pipe = usb_sndisocpipe(data->udev,
				       data->isoc_tx_ep->bEndpointAddress);

		usb_fill_int_urb(urb, data->udev, pipe,
				 skb->data, skb->len, btusb_isoc_tx_complete,
				 skb, data->isoc_tx_ep->bInterval);

		urb->transfer_flags = URB_ISO_ASAP;

		__fill_isoc_descriptor(urb, skb->len,
				       le16_to_cpu(data->isoc_tx_ep->
						   wMaxPacketSize));

		hdev->stat.sco_tx++;
		goto skip_waking;

	default:
		return -EILSEQ;
	}

	err = inc_tx(data);
	if (err) {
		usb_anchor_urb(urb, &data->deferred);
		schedule_work(&data->waker);
		err = 0;
		goto done;
	}

skip_waking:
	usb_anchor_urb(urb, &data->tx_anchor);
	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err < 0) {
		RTKBT_ERR("%s %s urb %p submission for %s failed, err %d",
			  __func__, hdev->name, urb,
			  pkt_ind[bt_cb(skb)->pkt_type], err);
		kfree(urb->setup_packet);
		usb_unanchor_urb(urb);
	} else {
		usb_mark_last_busy(data->udev);
	}
	usb_free_urb(urb);

done:
	return err;
}

#if HCI_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
static void btusb_destruct(struct hci_dev *hdev)
{
	RTKBT_DBG("btusb_destruct %s", hdev->name);
	hci_free_dev(hdev);
}
#endif

static void btusb_notify(struct hci_dev *hdev, unsigned int evt)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);

	RTKBT_DBG("%s evt %d", hdev->name, evt);
	RTKBT_DBG("btusb_notify : %s evt %d", hdev->name, evt);

	if (SCO_NUM != data->sco_num) {
		data->sco_num = SCO_NUM;
		schedule_work(&data->work);
	}
}

static inline int __set_isoc_interface(struct hci_dev *hdev, int altsetting)
{
	struct btusb_data *data = GET_DRV_DATA(hdev);
	struct usb_interface *intf = data->isoc;
	struct usb_endpoint_descriptor *ep_desc;
	int i, err;

	if (!data->isoc)
		return -ENODEV;

	RTKBT_INFO("set isoc interface: alt %d", altsetting);

	err = usb_set_interface(data->udev, 1, altsetting);
	if (err < 0) {
		RTKBT_ERR("%s setting interface failed (%d)", hdev->name, -err);
		return err;
	}

	data->isoc_altsetting = altsetting;

	data->isoc_tx_ep = NULL;
	data->isoc_rx_ep = NULL;

	for (i = 0; i < intf->cur_altsetting->desc.bNumEndpoints; i++) {
		ep_desc = &intf->cur_altsetting->endpoint[i].desc;

		if (!data->isoc_tx_ep && usb_endpoint_is_isoc_out(ep_desc)) {
			data->isoc_tx_ep = ep_desc;
			continue;
		}

		if (!data->isoc_rx_ep && usb_endpoint_is_isoc_in(ep_desc)) {
			data->isoc_rx_ep = ep_desc;
			continue;
		}
	}

	if (!data->isoc_tx_ep || !data->isoc_rx_ep) {
		RTKBT_ERR("%s invalid SCO descriptors", hdev->name);
		return -ENODEV;
	}

	return 0;
}

static void btusb_work(struct work_struct *work)
{
	struct btusb_data *data = container_of(work, struct btusb_data, work);
	struct hci_dev *hdev = data->hdev;
	int err;
	int new_alts;
	if (data->sco_num > 0) {
		if (!test_bit(BTUSB_DID_ISO_RESUME, &data->flags)) {
			err =
			    usb_autopm_get_interface(data->isoc ? data->
						     isoc : data->intf);
			if (err < 0) {
				clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
				mdelay(URB_CANCELING_DELAY_MS);
				usb_kill_anchored_urbs(&data->isoc_anchor);
				return;
			}

			set_bit(BTUSB_DID_ISO_RESUME, &data->flags);
		}
#if HCI_VERSION_CODE > KERNEL_VERSION(3, 7, 1)
		if (hdev->voice_setting & 0x0020) {
			static const int alts[3] = { 2, 4, 5 };
			new_alts = alts[data->sco_num - 1];
		} else {
			new_alts = data->sco_num;
		}
		if (data->isoc_altsetting != new_alts) {
#else
		if (data->isoc_altsetting != 2) {
			new_alts = 2;
#endif

			clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
			mdelay(URB_CANCELING_DELAY_MS);
			usb_kill_anchored_urbs(&data->isoc_anchor);

			if (__set_isoc_interface(hdev, new_alts) < 0)
				return;
		}

		if (!test_and_set_bit(BTUSB_ISOC_RUNNING, &data->flags)) {
			RTKBT_INFO("submit SCO RX urb.");
			if (btusb_submit_isoc_urb(hdev, GFP_KERNEL) < 0)
				clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
			else
				btusb_submit_isoc_urb(hdev, GFP_KERNEL);
		}
	} else {
		clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
		mdelay(URB_CANCELING_DELAY_MS);
		usb_kill_anchored_urbs(&data->isoc_anchor);

		__set_isoc_interface(hdev, 0);
		if (test_and_clear_bit(BTUSB_DID_ISO_RESUME, &data->flags))
			usb_autopm_put_interface(data->isoc ? data->
						 isoc : data->intf);
	}
}

static void btusb_waker(struct work_struct *work)
{
	struct btusb_data *data = container_of(work, struct btusb_data, waker);
	int err;

	err = usb_autopm_get_interface(data->intf);
	RTKBT_DBG("%s start  pm_usage_cnt(0x%x)", __FUNCTION__,
		  atomic_read(&(data->intf->pm_usage_cnt)));
	if (err < 0)
		return;

	usb_autopm_put_interface(data->intf);
	RTKBT_DBG("%s end  pm_usage_cnt(0x%x)", __FUNCTION__,
		  atomic_read(&(data->intf->pm_usage_cnt)));
}

static int btusb_probe(struct usb_interface *intf,
		       const struct usb_device_id *id)
{
	struct usb_endpoint_descriptor *ep_desc;
	struct btusb_data *data;
	struct hci_dev *hdev;
	int i, err, flag1, flag2;
	struct usb_device *udev;
	udev = interface_to_usbdev(intf);

	RTKBT_DBG("btusb_probe intf->cur_altsetting->desc.bInterfaceNumber=%d",
		  intf->cur_altsetting->desc.bInterfaceNumber);

	/* interface numbers are hardcoded in the spec */
	if (intf->cur_altsetting->desc.bInterfaceNumber != 0)
		return -ENODEV;

	/*******************************/
	flag1 = device_can_wakeup(&udev->dev);
	flag2 = device_may_wakeup(&udev->dev);
	RTKBT_DBG("btusb_probe can_wakeup %x, may wakeup %x", flag1, flag2);
#if BTUSB_WAKEUP_HOST
	device_wakeup_enable(&udev->dev);
#endif
	//device_wakeup_enable(&udev->dev);
	/*device_wakeup_disable(&udev->dev);
	   flag1=device_can_wakeup(&udev->dev);
	   flag2=device_may_wakeup(&udev->dev);
	   RTKBT_DBG("btusb_probe can_wakeup=%x  flag2=%x",flag1,flag2);
	 */
	err = patch_add(intf);
	if (err < 0)
		return -1;
	/*******************************/

	data = rtk_alloc(intf);
	if (!data)
		return -ENOMEM;

	for (i = 0; i < intf->cur_altsetting->desc.bNumEndpoints; i++) {
		ep_desc = &intf->cur_altsetting->endpoint[i].desc;

		if (!data->intr_ep && usb_endpoint_is_int_in(ep_desc)) {
			data->intr_ep = ep_desc;
			continue;
		}

		if (!data->bulk_tx_ep && usb_endpoint_is_bulk_out(ep_desc)) {
			data->bulk_tx_ep = ep_desc;
			continue;
		}

		if (!data->bulk_rx_ep && usb_endpoint_is_bulk_in(ep_desc)) {
			data->bulk_rx_ep = ep_desc;
			continue;
		}
	}

	if (!data->intr_ep || !data->bulk_tx_ep || !data->bulk_rx_ep) {
		rtk_free(data);
		return -ENODEV;
	}

	data->cmdreq_type = USB_TYPE_CLASS;

	data->udev = interface_to_usbdev(intf);
	data->intf = intf;

	spin_lock_init(&data->lock);

	INIT_WORK(&data->work, btusb_work);
	INIT_WORK(&data->waker, btusb_waker);
	spin_lock_init(&data->txlock);

	init_usb_anchor(&data->tx_anchor);
	init_usb_anchor(&data->intr_anchor);
	init_usb_anchor(&data->bulk_anchor);
	init_usb_anchor(&data->isoc_anchor);
	init_usb_anchor(&data->deferred);

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
	spin_lock_init(&data->rxlock);
	data->recv_bulk = btusb_recv_bulk;
#endif

	hdev = hci_alloc_dev();
	if (!hdev) {
		rtk_free(data);
		return -ENOMEM;
	}

	HDEV_BUS = HCI_USB;

	data->hdev = hdev;

	SET_HCIDEV_DEV(hdev, &intf->dev);

	hdev->open = btusb_open;
	hdev->close = btusb_close;
	hdev->flush = btusb_flush;
	hdev->send = btusb_send_frame;
	hdev->notify = btusb_notify;

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
	hci_set_drvdata(hdev, data);
#else
	hdev->driver_data = data;
	hdev->destruct = btusb_destruct;
	hdev->owner = THIS_MODULE;
#endif

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 7, 1)
	if (!reset)
		set_bit(HCI_QUIRK_RESET_ON_CLOSE, &hdev->quirks);
	RTKBT_DBG("set_bit(HCI_QUIRK_RESET_ON_CLOSE, &hdev->quirks);");
#endif

	/* Interface numbers are hardcoded in the specification */
	data->isoc = usb_ifnum_to_if(data->udev, 1);

	if (data->isoc) {
		err = usb_driver_claim_interface(&btusb_driver,
						 data->isoc, data);
		if (err < 0) {
			hci_free_dev(hdev);
			rtk_free(data);
			return err;
		}
	}

	/* Should reset the gEVersion to 0xff, otherwise the stored gEVersion
	 * would cause rtk_get_eversion() returning previous gEVersion if
	 * change to different ECO chip.
	 * This would cause downloading wrong patch, and the controller can't
	 * work. */
	gEVersion = 0xff;

	err = hci_register_dev(hdev);
	if (err < 0) {
		hci_free_dev(hdev);
		rtk_free(data);
		return err;
	}

	usb_set_intfdata(intf, data);

#ifdef BTCOEX
	rtk_usb_coex_probe(hdev);
#endif

	return 0;
}

static void btusb_disconnect(struct usb_interface *intf)
{
	struct btusb_data *data = usb_get_intfdata(intf);
	struct hci_dev *hdev;
	struct usb_device *udev;
	udev = interface_to_usbdev(intf);

	if (intf->cur_altsetting->desc.bInterfaceNumber != 0)
		return;

	if (!data)
		return;

	RTKBT_DBG("btusb_disconnect");
	/*******************************/
	patch_remove(intf);
	/*******************************/

	hdev = data->hdev;

#if HCI_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
	__hci_dev_hold(hdev);
#endif

	usb_set_intfdata(data->intf, NULL);

	if (data->isoc)
		usb_set_intfdata(data->isoc, NULL);

	hci_unregister_dev(hdev);

	if (intf == data->isoc)
		usb_driver_release_interface(&btusb_driver, data->intf);
	else if (data->isoc)
		usb_driver_release_interface(&btusb_driver, data->isoc);

#if HCI_VERSION_CODE < KERNEL_VERSION(3, 4, 0)
	__hci_dev_put(hdev);
#endif

#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
	btusb_free_frags(data);
#endif

	hci_free_dev(hdev);
	rtk_free(data);
}

#ifdef CONFIG_PM
static int btusb_suspend(struct usb_interface *intf, pm_message_t message)
{
	struct btusb_data *data = usb_get_intfdata(intf);

	if (intf->cur_altsetting->desc.bInterfaceNumber != 0)
		return 0;

	/*******************************/
	RTKBT_DBG("btusb_suspend message.event=0x%x,data->suspend_count=%d",
		  message.event, data->suspend_count);
	if (!test_bit(HCI_RUNNING, &data->hdev->flags)) {
		RTKBT_DBG("btusb_suspend-----bt is off");
		set_btoff(data->intf);
	}
	/*******************************/

	if (data->suspend_count++)
		return 0;

	spin_lock_irq(&data->txlock);
	if (!((message.event & PM_EVENT_AUTO) && data->tx_in_flight)) {
		set_bit(BTUSB_SUSPENDING, &data->flags);
		spin_unlock_irq(&data->txlock);
	} else {
		spin_unlock_irq(&data->txlock);
		data->suspend_count--;
		return -EBUSY;
	}

	cancel_work_sync(&data->work);

	btusb_stop_traffic(data);
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_kill_anchored_urbs(&data->tx_anchor);

	return 0;
}

static void play_deferred(struct btusb_data *data)
{
	struct urb *urb;
	int err;

	while ((urb = usb_get_from_anchor(&data->deferred))) {
	    /************************************/
		usb_anchor_urb(urb, &data->tx_anchor);
		err = usb_submit_urb(urb, GFP_ATOMIC);
		if (err < 0) {
			RTKBT_ERR("play_deferred urb %p submission failed",
				  urb);
			kfree(urb->setup_packet);
			usb_unanchor_urb(urb);
		} else {
			usb_mark_last_busy(data->udev);
		}
		usb_free_urb(urb);
		/************************************/
		data->tx_in_flight++;
	}
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_scuttle_anchored_urbs(&data->deferred);
}

static int btusb_resume(struct usb_interface *intf)
{
	struct btusb_data *data = usb_get_intfdata(intf);
	struct hci_dev *hdev = data->hdev;
	int err = 0;

	if (intf->cur_altsetting->desc.bInterfaceNumber != 0)
		return 0;

	/*******************************/
	RTKBT_DBG("btusb_resume data->suspend_count=%d", data->suspend_count);

	if (!test_bit(HCI_RUNNING, &hdev->flags)) {
		RTKBT_DBG("btusb_resume-----bt is off,download patch");
		download_patch(intf);
	} else
		RTKBT_DBG("btusb_resume,----bt is on");
	/*******************************/
	if (--data->suspend_count)
		return 0;

	if (test_bit(BTUSB_INTR_RUNNING, &data->flags)) {
		err = btusb_submit_intr_urb(hdev, GFP_NOIO);
		if (err < 0) {
			clear_bit(BTUSB_INTR_RUNNING, &data->flags);
			goto failed;
		}
	}

	if (test_bit(BTUSB_BULK_RUNNING, &data->flags)) {
		err = btusb_submit_bulk_urb(hdev, GFP_NOIO);
		if (err < 0) {
			clear_bit(BTUSB_BULK_RUNNING, &data->flags);
			goto failed;
		}

		btusb_submit_bulk_urb(hdev, GFP_NOIO);
	}

	if (test_bit(BTUSB_ISOC_RUNNING, &data->flags)) {
		if (btusb_submit_isoc_urb(hdev, GFP_NOIO) < 0)
			clear_bit(BTUSB_ISOC_RUNNING, &data->flags);
		else
			btusb_submit_isoc_urb(hdev, GFP_NOIO);
	}

	spin_lock_irq(&data->txlock);
	play_deferred(data);
	clear_bit(BTUSB_SUSPENDING, &data->flags);
	spin_unlock_irq(&data->txlock);
	schedule_work(&data->work);

	return 0;

failed:
	mdelay(URB_CANCELING_DELAY_MS);	// Added by Realtek
	usb_scuttle_anchored_urbs(&data->deferred);
//done:
	spin_lock_irq(&data->txlock);
	clear_bit(BTUSB_SUSPENDING, &data->flags);
	spin_unlock_irq(&data->txlock);

	return err;
}
#endif

static struct usb_driver btusb_driver = {
	.name = "rtk_btusb",
	.probe = btusb_probe,
	.disconnect = btusb_disconnect,
#ifdef CONFIG_PM
	.suspend = btusb_suspend,
	.resume = btusb_resume,
#endif
#if  CONFIG_RESET_RESUME
	.reset_resume = btusb_resume,
#endif
	.id_table = btusb_table,
	.supports_autosuspend = 1,
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 7, 1)
	.disable_hub_initiated_lpm = 1,
#endif
};

static int __init btusb_init(void)
{
	RTKBT_DBG("Realtek Bluetooth USB driver ver %s", VERSION);
#ifdef BTCOEX
	rtk_usb_coex_init();
#endif
	return usb_register(&btusb_driver);
}

static void __exit btusb_exit(void)
{
	RTKBT_DBG(KERN_INFO "rtk_btusb: btusb_exit");
	usb_deregister(&btusb_driver);

#ifdef BTCOEX
	rtk_usb_coex_exit();
#endif
}

module_init(btusb_init);
module_exit(btusb_exit);

MODULE_AUTHOR("");
MODULE_DESCRIPTION("Realtek Bluetooth USB driver ver " VERSION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");

/*******************************
**    Reasil patch code
********************************/
#define CMD_CMP_EVT		    0x0e
#define PKT_LEN			    300
#define MSG_TO			    1000	//us
#define PATCH_SEG_MAX	    252
#define PATCH_LENGTH_MAX    24576	//24*1024
#define DATA_END		    0x80
#define DOWNLOAD_OPCODE	    0xfc20
#define BTOFF_OPCODE	    0xfc28
#define TRUE			    1
#define FALSE			    0
#define CMD_HDR_LEN		    sizeof(struct hci_command_hdr)
#define EVT_HDR_LEN		    sizeof(struct hci_event_hdr)
#define CMD_CMP_LEN		    sizeof(struct hci_ev_cmd_complete)

//signature: Realtech
const uint8_t RTK_EPATCH_SIGNATURE[8] =
    { 0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x63, 0x68 };
//Extension Section IGNATURE:0x77FD0451
const uint8_t Extension_Section_SIGNATURE[4] = { 0x51, 0x04, 0xFD, 0x77 };

uint16_t project_id[] = {
	ROM_LMP_8723a,
	ROM_LMP_8723b,
	ROM_LMP_8821a,
	ROM_LMP_8761a,
	ROM_LMP_NONE
};

enum rtk_endpoit {
	CTRL_EP = 0,
	INTR_EP = 1,
	BULK_EP = 2,
	ISOC_EP = 3
};

typedef struct {
	uint16_t prod_id;
	uint16_t lmp_sub;
	char *mp_patch_name;
	char *patch_name;
	char *config_name;
	uint8_t *fw_cache;
	int fw_len;
} patch_info;

typedef struct {
	struct list_head list_node;
	struct usb_interface *intf;
	struct usb_device *udev;
	struct notifier_block pm_notifier;
	patch_info *patch_entry;
} dev_data;

typedef struct {
	dev_data *dev_entry;
	int pipe_in, pipe_out;
	uint8_t *send_pkt;
	uint8_t *rcv_pkt;
	struct hci_command_hdr *cmd_hdr;
	struct hci_event_hdr *evt_hdr;
	struct hci_ev_cmd_complete *cmd_cmp;
	uint8_t *req_para, *rsp_para;
	uint8_t *fw_data;
	int pkt_len, fw_len;
} xchange_data;

typedef struct {
	uint8_t index;
	uint8_t data[PATCH_SEG_MAX];
} __attribute__ ((packed)) download_cp;

typedef struct {
	uint8_t status;
	uint8_t index;
} __attribute__ ((packed)) download_rp;

#define RTK_VENDOR_CONFIG_MAGIC 0x8723ab55
struct rtk_bt_vendor_config_entry {
	uint16_t offset;
	uint8_t entry_len;
	uint8_t entry_data[0];
} __attribute__ ((packed));

struct rtk_bt_vendor_config {
	uint32_t signature;
	uint16_t data_len;
	struct rtk_bt_vendor_config_entry entry[0];
} __attribute__ ((packed));

static dev_data *dev_data_find(struct usb_interface *intf);
static patch_info *get_patch_entry(struct usb_device *udev);
static int rtkbt_pm_notify(struct notifier_block *notifier, ulong pm_event,
			   void *unused);
static int load_firmware(dev_data * dev_entry, uint8_t ** buff);
static void init_xdata(xchange_data * xdata, dev_data * dev_entry);
static int check_fw_version(xchange_data * xdata);
static int get_firmware(xchange_data * xdata);
static int download_data(xchange_data * xdata);
static int send_hci_cmd(xchange_data * xdata);
static int rcv_hci_evt(xchange_data * xdata);
static uint8_t rtk_get_eversion(dev_data * dev_entry);

static patch_info fw_patch_table[] = {
/* { pid, lmp_sub, mp_fw_name, fw_name, config_name, fw_cache, fw_len } */
	{0x1724, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* RTL8723A */
	{0x8723, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AE */
	{0xA723, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AE for LI */
	{0x0723, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AE */
	{0x3394, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AE for Azurewave */

	{0x0724, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AU */
	{0x8725, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AU */
	{0x872A, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AU */
	{0x872B, 0x1200, "mp_rtl8723a_fw", "rtl8723a_fw", "rtl8723a_config", NULL, 0},	/* 8723AU */

	{0xb720, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723bu_config", NULL, 0},	/* RTL8723BU */
	{0xb72A, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723bu_config", NULL, 0},	/* RTL8723BU */
	{0xb728, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for LC */
	{0xb723, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */
	{0xb72B, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */
	{0xb001, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for HP */
	{0xb002, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */
	{0xb003, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */
	{0xb004, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */
	{0xb005, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE */

	{0x3410, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for Azurewave */
	{0x3416, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for Azurewave */
	{0x3459, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for Azurewave */
	{0xE085, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for Foxconn */
	{0xE08B, 0x8723, "mp_rtl8723b_fw", "rtl8723b_fw", "rtl8723b_config", NULL, 0},	/* RTL8723BE for Foxconn */

	{0xA761, 0x8761, "mp_rtl8761a_fw", "rtl8761au_fw", "rtl8761a_config", NULL, 0},	/* RTL8761AU only */
	{0x818B, 0x8761, "mp_rtl8761a_fw", "rtl8761aw_fw", "rtl8761aw_config", NULL, 0},	/* RTL8761AW + 8192EU */
	{0x818C, 0x8761, "mp_rtl8761a_fw", "rtl8761aw_fw", "rtl8761aw_config", NULL, 0},	/* RTL8761AW + 8192EU */
	{0x8760, 0x8761, "mp_rtl8761a_fw", "rtl8761au_fw", "rtl8761a_config", NULL, 0},	/* RTL8761AU + 8192EE */
	{0xB761, 0x8761, "mp_rtl8761a_fw", "rtl8761au_fw", "rtl8761a_config", NULL, 0},	/* RTL8761AU + 8192EE */
	{0x8761, 0x8761, "mp_rtl8761a_fw", "rtl8761au_fw", "rtl8761a_config", NULL, 0},	/* RTL8761AU + 8192EE for LI */
	{0x8A60, 0x8761, "mp_rtl8761a_fw", "rtl8761au_fw", "rtl8761a_config", NULL, 0},	/* RTL8761AU + 8812AE */

	{0x8821, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */
	{0x0821, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */
	{0x0823, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AU */
	{0x3414, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */
	{0x3458, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */
	{0x3461, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */
	{0x3462, 0x8821, "mp_rtl8821a_fw", "rtl8821a_fw", "rtl8821a_config", NULL, 0},	/* RTL8821AE */

/* NOTE: must append patch entries above the null entry */
	{0, 0, NULL, NULL, NULL, NULL, 0}
};

static LIST_HEAD(dev_data_list);

int patch_add(struct usb_interface *intf)
{
	dev_data *dev_entry;
	struct usb_device *udev;

	RTKBT_DBG("patch_add");
	dev_entry = dev_data_find(intf);
	if (NULL != dev_entry) {
		return -1;
	}

	udev = interface_to_usbdev(intf);
#if BTUSB_RPM
	RTKBT_DBG("auto suspend is enabled");
	usb_enable_autosuspend(udev);
	pm_runtime_set_autosuspend_delay(&(udev->dev), 2000);
#else
	RTKBT_DBG("auto suspend is disabled");
	usb_disable_autosuspend(udev);
#endif

	dev_entry = kzalloc(sizeof(dev_data), GFP_KERNEL);
	dev_entry->intf = intf;
	dev_entry->udev = udev;
	dev_entry->pm_notifier.notifier_call = rtkbt_pm_notify;
	dev_entry->patch_entry = get_patch_entry(udev);
	if (NULL == dev_entry->patch_entry) {
		kfree(dev_entry);
		return -1;
	}
	list_add(&dev_entry->list_node, &dev_data_list);
	register_pm_notifier(&dev_entry->pm_notifier);

	return 0;
}

void patch_remove(struct usb_interface *intf)
{
	dev_data *dev_entry;
	struct usb_device *udev;

	udev = interface_to_usbdev(intf);
#if BTUSB_RPM
	usb_disable_autosuspend(udev);
#endif

	dev_entry = dev_data_find(intf);
	if (NULL == dev_entry) {
		return;
	}

	RTKBT_DBG("patch_remove");
	list_del(&dev_entry->list_node);
	unregister_pm_notifier(&dev_entry->pm_notifier);
	kfree(dev_entry);
}

static int send_reset_command(xchange_data *xdata)
{
	int ret_val;

	RTKBT_DBG("HCI reset.");

	xdata->cmd_hdr->opcode = cpu_to_le16(HCI_OP_RESET);
	xdata->cmd_hdr->plen = 0;
	xdata->pkt_len = CMD_HDR_LEN;

	ret_val = send_hci_cmd(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("failed to send hci cmd.");
		return ret_val;
	}

	ret_val = rcv_hci_evt(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("failed to recv hci event.");
		return ret_val;
	}

	return 0;
}

int download_patch(struct usb_interface *intf)
{
	dev_data *dev_entry;
	xchange_data *xdata = NULL;
	uint8_t *fw_buf;
	int ret_val;

	RTKBT_DBG("download_patch start");
	dev_entry = dev_data_find(intf);
	if (NULL == dev_entry) {
		ret_val = -1;
		RTKBT_ERR("NULL == dev_entry");
		goto patch_end;
	}

	xdata = kzalloc(sizeof(xchange_data), GFP_KERNEL);
	if (NULL == xdata) {
		ret_val = -1;
		RTKBT_DBG("NULL == xdata");
		goto patch_end;
	}

	init_xdata(xdata, dev_entry);

	ret_val = check_fw_version(xdata);
	if (ret_val != 0) {
		if (gEVersion == 0xFF) {
			RTKBT_DBG("global_version is not set, get it!");
			gEVersion = rtk_get_eversion(dev_entry);
		}
		goto patch_end;
	}

	ret_val = get_firmware(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("get_firmware failed!");
		goto patch_end;
	}
	fw_buf = xdata->fw_data;

	if (xdata->fw_len > PATCH_LENGTH_MAX) {
		RTKBT_ERR("fw&config total length larger than allowed!");
		goto patch_fail;
	}

	ret_val = download_data(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("download_data failed!");
		goto patch_fail;
	}

	ret_val = check_fw_version(xdata);
	if (ret_val <= 0) {
		ret_val = -1;
		goto patch_fail;
	}

	ret_val = 0;
patch_fail:
	kfree(fw_buf);
patch_end:
	if (xdata != NULL) {
		if (xdata->send_pkt)
			kfree(xdata->send_pkt);
		if (xdata->rcv_pkt)
			kfree(xdata->rcv_pkt);
		kfree(xdata);
	}
	RTKBT_DBG("Rtk patch end %d", ret_val);
	return ret_val;
}

int set_btoff(struct usb_interface *intf)
{
	dev_data *dev_entry;
	xchange_data *xdata = NULL;
	int ret_val;

	RTKBT_DBG("set_btoff");
	dev_entry = dev_data_find(intf);
	if (NULL == dev_entry) {
		return -1;
	}

	xdata = kzalloc(sizeof(xchange_data), GFP_KERNEL);
	if (NULL == xdata) {
		ret_val = -1;
		RTKBT_DBG("NULL == xdata");
		return ret_val;
	}

	init_xdata(xdata, dev_entry);

	xdata->cmd_hdr->opcode = cpu_to_le16(BTOFF_OPCODE);
	xdata->cmd_hdr->plen = 1;
	xdata->pkt_len = CMD_HDR_LEN + 1;
	xdata->send_pkt[CMD_HDR_LEN] = 1;

	ret_val = send_hci_cmd(xdata);
	if (ret_val < 0) {
		goto tagEnd;
	}

	ret_val = rcv_hci_evt(xdata);
	if (ret_val < 0) {
		goto tagEnd;
	}

tagEnd:
	if (xdata != NULL) {
		if (xdata->send_pkt)
			kfree(xdata->send_pkt);
		if (xdata->rcv_pkt)
			kfree(xdata->rcv_pkt);
		kfree(xdata);
	}

	RTKBT_DBG("set_btoff done");

	return ret_val;
}

dev_data *dev_data_find(struct usb_interface * intf)
{
	dev_data *dev_entry;

	list_for_each_entry(dev_entry, &dev_data_list, list_node) {
		if (dev_entry->intf == intf) {
			return dev_entry;
		}
	}

	return NULL;
}

patch_info *get_patch_entry(struct usb_device * udev)
{
	patch_info *patch_entry;
	uint16_t pid;

	patch_entry = fw_patch_table;
	pid = le16_to_cpu(udev->descriptor.idProduct);
	RTKBT_DBG("pid = 0x%x", pid);
	while (pid != patch_entry->prod_id) {
		if (0 == patch_entry->prod_id) {
			RTKBT_DBG
			    ("get_patch_entry =NULL, can not find device pid in patch_table");
			return NULL;	//break;
		}
		patch_entry++;
	}

	return patch_entry;
}

int rtkbt_pm_notify(struct notifier_block *notifier,
		    ulong pm_event, void *unused)
{
	dev_data *dev_entry;
	patch_info *patch_entry;
	struct usb_device *udev;

	dev_entry = container_of(notifier, dev_data, pm_notifier);
	patch_entry = dev_entry->patch_entry;
	udev = dev_entry->udev;
	RTKBT_DBG("rtkbt_pm_notify pm_event =%ld", pm_event);
	switch (pm_event) {
	case PM_SUSPEND_PREPARE:
	case PM_HIBERNATION_PREPARE:
		patch_entry->fw_len =
		    load_firmware(dev_entry, &patch_entry->fw_cache);
		if (patch_entry->fw_len <= 0) {
			RTKBT_DBG("rtkbt_pm_notify return NOTIFY_BAD");
			return NOTIFY_BAD;
		}

		if (!device_may_wakeup(&udev->dev)) {
#if (CONFIG_RESET_RESUME)
			RTKBT_DBG
			    ("remote wakeup not support, reset_resume support ");
#else
			dev_entry->intf->needs_binding = 1;
			RTKBT_DBG
			    ("remote wakeup not support, set intf->needs_binding = 1");
#endif
		}
		break;

	case PM_POST_SUSPEND:
	case PM_POST_HIBERNATION:
	case PM_POST_RESTORE:
		if (patch_entry->fw_len > 0) {
			kfree(patch_entry->fw_cache);
			patch_entry->fw_cache = NULL;
			patch_entry->fw_len = 0;
		}
#if BTUSB_RPM
		usb_disable_autosuspend(udev);
		usb_enable_autosuspend(udev);
		pm_runtime_set_autosuspend_delay(&(udev->dev), 2000);
#endif
		break;

	default:
		break;
	}

	return NOTIFY_DONE;
}

int rtk_parse_config_file(unsigned char *config_buf, int *filelen,
			  char bt_addr[6])
{
	struct rtk_bt_vendor_config *config =
	    (struct rtk_bt_vendor_config *)config_buf;
	uint16_t config_len = 0, temp = 0;
	struct rtk_bt_vendor_config_entry *entry = NULL;
	unsigned int i = 0;
	//int j = 0;

	if (config == NULL)
		return 0;

	config_len = le16_to_cpu(config->data_len);
	entry = config->entry;

	if (le32_to_cpu(config->signature) != RTK_VENDOR_CONFIG_MAGIC) {
		RTKBT_ERR
		    ("config signature magic number(%x) is not set to RTK_VENDOR_CONFIG_MAGIC",
		     config->signature);
		return 0;
	}

	if (config_len != *filelen - sizeof(struct rtk_bt_vendor_config)) {
		RTKBT_ERR("config len(%d) is not right(%zd)", config_len,
			  *filelen - sizeof(struct rtk_bt_vendor_config));
		return 0;
	}

	for (i = 0; i < config_len;) {
		switch (le16_to_cpu(entry->offset)) {
#if 0
		case 0x3c:
			for (j = 0; j < entry->entry_len; j++)
				entry->entry_data[j] =
				    bt_addr[entry->entry_len - 1 - j];
			RTKBT_DBG("config has bdaddr");
			break;
#endif
		default:
			RTKBT_DBG("config offset(%x),length(%x)", entry->offset,
				  entry->entry_len);
			break;
		}
		temp =
		    entry->entry_len +
		    sizeof(struct rtk_bt_vendor_config_entry);
		i += temp;
		entry =
		    (struct rtk_bt_vendor_config_entry *)((uint8_t *) entry +
							  temp);
	}

	return 1;
}

uint8_t rtk_get_fw_project_id(uint8_t * p_buf)
{
	uint8_t opcode;
	uint8_t len;
	uint8_t data = 0;

	do {
		opcode = *p_buf;
		len = *(p_buf - 1);
		if (opcode == 0x00) {
			if (len == 1) {
				data = *(p_buf - 2);
				RTKBT_DBG
				    ("rtk_get_fw_project_id: opcode %d, len %d, data %d",
				     opcode, len, data);
				break;
			} else {
				RTKBT_ERR
				    ("rtk_get_fw_project_id: invalid len %d",
				     len);
			}
		}
		p_buf -= len + 2;
	} while (*p_buf != 0xFF);

	return data;
}

static void rtk_get_patch_entry(uint8_t * epatch_buf,
				struct rtk_epatch_entry *entry)
{
	uint16_t i;
	struct rtk_epatch *epatch_info = (struct rtk_epatch *)epatch_buf;

	epatch_info->number_of_total_patch =
	    le16_to_cpu(epatch_info->number_of_total_patch);
	RTKBT_DBG("fw_version = 0x%x", le32_to_cpu(epatch_info->fw_version));
	RTKBT_DBG("number_of_total_patch = %d",
		  epatch_info->number_of_total_patch);

	/* get right epatch entry */
	for (i = 0; i < epatch_info->number_of_total_patch; i++) {
		if (get_unaligned_le16(epatch_buf + 14 + 2 * i) ==
		    gEVersion + 1) {
			entry->chipID = gEVersion + 1;
			entry->patch_length =
			    get_unaligned_le16(epatch_buf + 14 +
					       2 *
					       epatch_info->
					       number_of_total_patch + 2 * i);
			entry->start_offset =
			    get_unaligned_le32(epatch_buf + 14 +
					       4 *
					       epatch_info->
					       number_of_total_patch + 4 * i);
			break;
		}
	}
	RTKBT_DBG("chipID = %d", entry->chipID);
	RTKBT_DBG("patch_length = 0x%x", entry->patch_length);
	RTKBT_DBG("start_offset = 0x%x", entry->start_offset);
}

int load_firmware(dev_data * dev_entry, uint8_t ** buff)
{
	const struct firmware *fw;
	struct usb_device *udev;
	patch_info *patch_entry;
	char *fw_name;
	int fw_len = 0, ret_val = 0, config_len = 0, buf_len = -1;
	uint8_t *buf = *buff, *config_file_buf = NULL, *epatch_buf = NULL;
	uint8_t proj_id = 0;
	uint8_t need_download_fw = 1;
	uint16_t lmp_version;
	struct rtk_epatch_entry current_entry = { 0 };
	uint8_t vnd_local_bd_addr[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };

	RTKBT_DBG("load_firmware start");
	udev = dev_entry->udev;
	patch_entry = dev_entry->patch_entry;
	lmp_version = patch_entry->lmp_sub;
	RTKBT_DBG("lmp_version = 0x%04x", lmp_version);

	fw_name = patch_entry->config_name;
	RTKBT_DBG("config name is  %s", fw_name);

	ret_val = request_firmware(&fw, fw_name, &udev->dev);
	if (ret_val < 0)
		config_len = 0;
	else {
		config_file_buf = kzalloc(fw->size, GFP_KERNEL);
		if (NULL == config_file_buf)
			goto alloc_fail;
		memcpy(config_file_buf, fw->data, fw->size);
		config_len = fw->size;
		rtk_parse_config_file(config_file_buf, &config_len,
				      vnd_local_bd_addr);

	}

	release_firmware(fw);
	fw_name = patch_entry->patch_name;
	RTKBT_ERR("fw name is  %s", fw_name);

	ret_val = request_firmware(&fw, fw_name, &udev->dev);
	if (ret_val < 0) {
		fw_len = 0;
		kfree(config_file_buf);
		config_file_buf = NULL;
		goto fw_fail;
	}
	epatch_buf = kzalloc(fw->size, GFP_KERNEL);
	if (NULL == epatch_buf)
		goto alloc_fail;

	memcpy(epatch_buf, fw->data, fw->size);
	buf_len = fw->size + config_len;

	if (lmp_version == ROM_LMP_8723a) {
		RTKBT_DBG("This is 8723a, use old patch style!");

		if (memcmp(epatch_buf, RTK_EPATCH_SIGNATURE, 8) == 0) {
			RTKBT_ERR("8723a Check signature error!");
			need_download_fw = 0;
		} else {
			if (!(buf = kzalloc(buf_len, GFP_KERNEL))) {
				RTKBT_ERR("Can't alloc memory for fw&config");
				buf_len = -1;
			} else {
				RTKBT_DBG("8723a, fw copy direct");
				memcpy(buf, epatch_buf, fw->size);
				if (config_len) {
					memcpy(&buf[buf_len - config_len],
					       config_file_buf, config_len);
				}
			}
		}
	} else {
		RTKBT_ERR("This is not 8723a, use new patch style!");

		/* Get version from ROM */
		gEVersion = rtk_get_eversion(dev_entry);
		RTKBT_DBG("gEVersion=%d", gEVersion);
		if (gEVersion == 0xFE) {
			need_download_fw = 0;
			fw_len = 0;
			goto alloc_fail;
		}

		/* check Signature and Extension Section Field */
		if ((memcmp(epatch_buf, RTK_EPATCH_SIGNATURE, 8) != 0) ||
		    memcmp(epatch_buf + buf_len - config_len - 4,
			   Extension_Section_SIGNATURE, 4) != 0) {
			RTKBT_ERR("Check SIGNATURE error! do not download fw");
			need_download_fw = 0;
		} else {
			proj_id =
			    rtk_get_fw_project_id(epatch_buf + buf_len -
						  config_len - 5);

			if (lmp_version != project_id[proj_id]) {
				RTKBT_ERR
				    ("lmp_version is %x, project_id is %x, does not match!!!",
				     lmp_version, project_id[proj_id]);
				need_download_fw = 0;
			} else {
				RTKBT_DBG
				    ("lmp_version is %x, project_id is %x, match!",
				     lmp_version, project_id[proj_id]);
				rtk_get_patch_entry(epatch_buf, &current_entry);

				if (current_entry.patch_length == 0)
					goto fw_fail;

				buf_len =
				    current_entry.patch_length + config_len;
				RTKBT_DBG("buf_len = 0x%x", buf_len);

				if (!(buf = kzalloc(buf_len, GFP_KERNEL))) {
					RTKBT_ERR
					    ("Can't alloc memory for multi fw&config");
					buf_len = -1;
				} else {
					memcpy(buf,
					       epatch_buf +
					       current_entry.start_offset,
					       current_entry.patch_length);
					memcpy(buf + current_entry.patch_length - 4, epatch_buf + 8, 4);	/*fw version */
					if (config_len) {
						memcpy(&buf
						       [buf_len - config_len],
						       config_file_buf,
						       config_len);
					}
				}
			}
		}
	}

	RTKBT_DBG("fw:%s exists, config file:%s exists",
		  (buf_len > 0) ? "" : "not", (config_len > 0) ? "" : "not");
	if (buf && (buf_len > 0) && (need_download_fw)) {
		fw_len = buf_len;
		*buff = buf;
	}

	RTKBT_DBG("load_firmware done");

alloc_fail:
	release_firmware(fw);

	if (epatch_buf)
		kfree(epatch_buf);

	if (config_file_buf)
		kfree(config_file_buf);
fw_fail:
	return fw_len;
}

void init_xdata(xchange_data * xdata, dev_data * dev_entry)
{
	memset(xdata, 0, sizeof(xchange_data));
	xdata->dev_entry = dev_entry;
	xdata->pipe_in = usb_rcvintpipe(dev_entry->udev, INTR_EP);
	xdata->pipe_out = usb_sndctrlpipe(dev_entry->udev, CTRL_EP);
	xdata->send_pkt = kzalloc(PKT_LEN, GFP_KERNEL);
	xdata->rcv_pkt = kzalloc(PKT_LEN, GFP_KERNEL);
	xdata->cmd_hdr = (struct hci_command_hdr *)(xdata->send_pkt);
	xdata->evt_hdr = (struct hci_event_hdr *)(xdata->rcv_pkt);
	xdata->cmd_cmp =
	    (struct hci_ev_cmd_complete *)(xdata->rcv_pkt + EVT_HDR_LEN);
	xdata->req_para = xdata->send_pkt + CMD_HDR_LEN;
	xdata->rsp_para = xdata->rcv_pkt + EVT_HDR_LEN + CMD_CMP_LEN;
}

int check_fw_version(xchange_data * xdata)
{
	struct hci_rp_read_local_version *read_ver_rsp;
	patch_info *patch_entry;
	int ret_val;
	int retry = 0;

get_ver:
	xdata->cmd_hdr->opcode = cpu_to_le16(HCI_OP_READ_LOCAL_VERSION);
	xdata->cmd_hdr->plen = 0;
	xdata->pkt_len = CMD_HDR_LEN;

	ret_val = send_hci_cmd(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("%s: Failed to send HCI command.", __func__);
		goto version_end;
	}

	ret_val = rcv_hci_evt(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("%s: Failed to receive HCI event.", __func__);
		goto version_end;
	}

	patch_entry = xdata->dev_entry->patch_entry;
	read_ver_rsp = (struct hci_rp_read_local_version *)(xdata->rsp_para);
	read_ver_rsp->lmp_subver = le16_to_cpu(read_ver_rsp->lmp_subver);
	read_ver_rsp->hci_rev = le16_to_cpu(read_ver_rsp->hci_rev);
	read_ver_rsp->manufacturer = le16_to_cpu(read_ver_rsp->manufacturer);

	RTKBT_DBG("read_ver_rsp->lmp_subver = 0x%x", read_ver_rsp->lmp_subver);
	RTKBT_DBG("patch_entry->lmp_sub = 0x%x", patch_entry->lmp_sub);
	if (patch_entry->lmp_sub != read_ver_rsp->lmp_subver) {
		return 1;
	}

	ret_val = 0;
version_end:
	if (ret_val) {
		send_reset_command(xdata);
		retry++;
		if (retry < 2)
			goto get_ver;
	}

	return ret_val;
}

uint8_t rtk_get_eversion(dev_data * dev_entry)
{
	struct rtk_eversion_evt *eversion;
	patch_info *patch_entry;
	int ret_val = 0;
	xchange_data *xdata = NULL;

	RTKBT_DBG("gEVersion=%d", gEVersion);
	if (gEVersion != 0xFF && gEVersion != 0xFE) {
		RTKBT_DBG("gEVersion != 0xFF, return it directly!");
		return gEVersion;
	}

	xdata = kzalloc(sizeof(xchange_data), GFP_KERNEL);
	if (NULL == xdata) {
		ret_val = 0xFE;
		RTKBT_DBG("NULL == xdata");
		return ret_val;
	}

	init_xdata(xdata, dev_entry);

	xdata->cmd_hdr->opcode = cpu_to_le16(HCI_VENDOR_READ_RTK_ROM_VERISION);
	xdata->cmd_hdr->plen = 0;
	xdata->pkt_len = CMD_HDR_LEN;

	ret_val = send_hci_cmd(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("Failed to send read RTK rom version cmd.");
		ret_val = 0xFE;
		goto version_end;
	}

	ret_val = rcv_hci_evt(xdata);
	if (ret_val < 0) {
		RTKBT_ERR("Failed to receive HCI event from rom version.");
		ret_val = 0xFE;
		goto version_end;
	}

	patch_entry = xdata->dev_entry->patch_entry;
	eversion = (struct rtk_eversion_evt *)(xdata->rsp_para);
	RTKBT_DBG("eversion->status = 0x%x, eversion->version = 0x%x",
		  eversion->status, eversion->version);
	if (eversion->status) {
		ret_val = 0;
		//global_eversion = 0;
	} else {
		ret_val = eversion->version;
		//global_eversion = eversion->version;
	}

version_end:
	if (xdata != NULL) {
		if (xdata->send_pkt)
			kfree(xdata->send_pkt);
		if (xdata->rcv_pkt)
			kfree(xdata->rcv_pkt);
		kfree(xdata);
	}
	return ret_val;
}

int get_firmware(xchange_data * xdata)
{
	dev_data *dev_entry;
	patch_info *patch_entry;
	RTKBT_DBG("get_firmware start");

	dev_entry = xdata->dev_entry;
	patch_entry = dev_entry->patch_entry;
	if (patch_entry->fw_len > 0) {
		xdata->fw_data = kzalloc(patch_entry->fw_len, GFP_KERNEL);
		if (NULL == xdata->fw_data)
			return -ENOMEM;
		memcpy(xdata->fw_data, patch_entry->fw_cache,
		       patch_entry->fw_len);
		xdata->fw_len = patch_entry->fw_len;
	} else {
		xdata->fw_len = load_firmware(dev_entry, &xdata->fw_data);
		if (xdata->fw_len <= 0)
			return -1;
	}
	RTKBT_DBG("get_firmware done");
	return 0;
}

int download_data(xchange_data * xdata)
{
	download_cp *cmd_para;
	download_rp *evt_para;
	uint8_t *pcur;
	int pkt_len, frag_num, frag_len;
	int i, ret_val;

	RTKBT_DBG("download_data start");

	cmd_para = (download_cp *) xdata->req_para;
	evt_para = (download_rp *) xdata->rsp_para;
	pcur = xdata->fw_data;
	pkt_len = CMD_HDR_LEN + sizeof(download_cp);
	frag_num = xdata->fw_len / PATCH_SEG_MAX + 1;
	frag_len = PATCH_SEG_MAX;

	for (i = 0; i < frag_num; i++) {
		cmd_para->index = i;
		if (i == (frag_num - 1)) {
			cmd_para->index |= DATA_END;
			frag_len = xdata->fw_len % PATCH_SEG_MAX;
			pkt_len -= (PATCH_SEG_MAX - frag_len);
		}
		xdata->cmd_hdr->opcode = cpu_to_le16(DOWNLOAD_OPCODE);
		xdata->cmd_hdr->plen = sizeof(uint8_t) + frag_len;
		xdata->pkt_len = pkt_len;
		memcpy(cmd_para->data, pcur, frag_len);

		ret_val = send_hci_cmd(xdata);
		if (ret_val < 0) {
			return ret_val;
		}

		ret_val = rcv_hci_evt(xdata);
		if (ret_val < 0) {
			return ret_val;
		}

		if (0 != evt_para->status) {
			return -1;
		}

		pcur += PATCH_SEG_MAX;
	}

	RTKBT_DBG("download_data done");
	return xdata->fw_len;
}

int send_hci_cmd(xchange_data * xdata)
{
	int ret_val;

	ret_val = usb_control_msg(xdata->dev_entry->udev, xdata->pipe_out,
				  0, USB_TYPE_CLASS, 0, 0,
				  (void *)(xdata->send_pkt),
				  xdata->pkt_len, MSG_TO);

	return ret_val;
}

int rcv_hci_evt(xchange_data * xdata)
{
	int ret_len = 0, ret_val = 0;
	int i;			// Added by Realtek

	while (1) {
		// **************************** Modifed by Realtek (begin)
		for (i = 0; i < 5; i++)	// Try to send USB interrupt message 5 times.
		{
			ret_val =
			    usb_interrupt_msg(xdata->dev_entry->udev,
					      xdata->pipe_in,
					      (void *)(xdata->rcv_pkt), PKT_LEN,
					      &ret_len, MSG_TO);
			if (ret_val >= 0)
				break;
		}
		// **************************** Modifed by Realtek (end)

		if (ret_val < 0) {
			return ret_val;
		}

		if (CMD_CMP_EVT == xdata->evt_hdr->evt) {
			if (xdata->cmd_hdr->opcode == xdata->cmd_cmp->opcode)
				return ret_len;
		}
	}
}

void print_acl(struct sk_buff *skb, int dataOut)
{
#if PRINT_ACL_DATA
	uint wlength = skb->len;
	uint icount = 0;
	u16 *handle = (u16 *) (skb->data);
	u16 dataLen = *(handle + 1);
	u8 *acl_data = (u8 *) (skb->data);
//if (0==dataOut)
	printk("%d handle:%04x,len:%d,", dataOut, *handle, dataLen);
//else
//      printk("In handle:%04x,len:%d,",*handle,dataLen);
/*	for(icount=4;(icount<wlength)&&(icount<32);icount++)
		{
			printk("%02x ",*(acl_data+icount) );
		}
	printk("\n");
*/
#endif
}

void print_command(struct sk_buff *skb)
{
#if PRINT_CMD_EVENT
	uint wlength = skb->len;
	uint icount = 0;
	u16 *opcode = (u16 *) (skb->data);
	u8 *cmd_data = (u8 *) (skb->data);
	u8 paramLen = *(cmd_data + 2);

	switch (*opcode) {
	case HCI_OP_INQUIRY:
		printk("HCI_OP_INQUIRY");
		break;
	case HCI_OP_INQUIRY_CANCEL:
		printk("HCI_OP_INQUIRY_CANCEL");
		break;
	case HCI_OP_EXIT_PERIODIC_INQ:
		printk("HCI_OP_EXIT_PERIODIC_INQ");
		break;
	case HCI_OP_CREATE_CONN:
		printk("HCI_OP_CREATE_CONN");
		break;
	case HCI_OP_DISCONNECT:
		printk("HCI_OP_DISCONNECT");
		break;
	case HCI_OP_CREATE_CONN_CANCEL:
		printk("HCI_OP_CREATE_CONN_CANCEL");
		break;
	case HCI_OP_ACCEPT_CONN_REQ:
		printk("HCI_OP_ACCEPT_CONN_REQ");
		break;
	case HCI_OP_REJECT_CONN_REQ:
		printk("HCI_OP_REJECT_CONN_REQ");
		break;
	case HCI_OP_AUTH_REQUESTED:
		printk("HCI_OP_AUTH_REQUESTED");
		break;
	case HCI_OP_SET_CONN_ENCRYPT:
		printk("HCI_OP_SET_CONN_ENCRYPT");
		break;
	case HCI_OP_REMOTE_NAME_REQ:
		printk("HCI_OP_REMOTE_NAME_REQ");
		break;
	case HCI_OP_READ_REMOTE_FEATURES:
		printk("HCI_OP_READ_REMOTE_FEATURES");
		break;
	case HCI_OP_SNIFF_MODE:
		printk("HCI_OP_SNIFF_MODE");
		break;
	case HCI_OP_EXIT_SNIFF_MODE:
		printk("HCI_OP_EXIT_SNIFF_MODE");
		break;
	case HCI_OP_SWITCH_ROLE:
		printk("HCI_OP_SWITCH_ROLE");
		break;
	case HCI_OP_SNIFF_SUBRATE:
		printk("HCI_OP_SNIFF_SUBRATE");
		break;
	case HCI_OP_RESET:
		printk("HCI_OP_RESET");
		break;
	default:
		printk("CMD");
		break;
	}
	printk(":%04x,len:%d,", *opcode, paramLen);
	for (icount = 3; (icount < wlength) && (icount < 24); icount++) {
		printk("%02x ", *(cmd_data + icount));
	}
	printk("\n");

#endif
}

void print_event(struct sk_buff *skb)
{
#if PRINT_CMD_EVENT
	uint wlength = skb->len;
	uint icount = 0;
	u8 *opcode = (u8 *) (skb->data);
	u8 paramLen = *(opcode + 1);

	switch (*opcode) {
	case HCI_EV_INQUIRY_COMPLETE:
		printk("HCI_EV_INQUIRY_COMPLETE");
		break;
	case HCI_EV_INQUIRY_RESULT:
		printk("HCI_EV_INQUIRY_RESULT");
		break;
	case HCI_EV_CONN_COMPLETE:
		printk("HCI_EV_CONN_COMPLETE");
		break;
	case HCI_EV_CONN_REQUEST:
		printk("HCI_EV_CONN_REQUEST");
		break;
	case HCI_EV_DISCONN_COMPLETE:
		printk("HCI_EV_DISCONN_COMPLETE");
		break;
	case HCI_EV_AUTH_COMPLETE:
		printk("HCI_EV_AUTH_COMPLETE");
		break;
	case HCI_EV_REMOTE_NAME:
		printk("HCI_EV_REMOTE_NAME");
		break;
	case HCI_EV_ENCRYPT_CHANGE:
		printk("HCI_EV_ENCRYPT_CHANGE");
		break;
	case HCI_EV_CHANGE_LINK_KEY_COMPLETE:
		printk("HCI_EV_CHANGE_LINK_KEY_COMPLETE");
		break;
	case HCI_EV_REMOTE_FEATURES:
		printk("HCI_EV_REMOTE_FEATURES");
		break;
	case HCI_EV_REMOTE_VERSION:
		printk("HCI_EV_REMOTE_VERSION");
		break;
	case HCI_EV_QOS_SETUP_COMPLETE:
		printk("HCI_EV_QOS_SETUP_COMPLETE");
		break;
	case HCI_EV_CMD_COMPLETE:
		printk("HCI_EV_CMD_COMPLETE");
		break;
	case HCI_EV_CMD_STATUS:
		printk("HCI_EV_CMD_STATUS");
		break;
	case HCI_EV_ROLE_CHANGE:
		printk("HCI_EV_ROLE_CHANGE");
		break;
	case HCI_EV_NUM_COMP_PKTS:
		printk("HCI_EV_NUM_COMP_PKTS");
		break;
	case HCI_EV_MODE_CHANGE:
		printk("HCI_EV_MODE_CHANGE");
		break;
	case HCI_EV_PIN_CODE_REQ:
		printk("HCI_EV_PIN_CODE_REQ");
		break;
	case HCI_EV_LINK_KEY_REQ:
		printk("HCI_EV_LINK_KEY_REQ");
		break;
	case HCI_EV_LINK_KEY_NOTIFY:
		printk("HCI_EV_LINK_KEY_NOTIFY");
		break;
	case HCI_EV_CLOCK_OFFSET:
		printk("HCI_EV_CLOCK_OFFSET");
		break;
	case HCI_EV_PKT_TYPE_CHANGE:
		printk("HCI_EV_PKT_TYPE_CHANGE");
		break;
	case HCI_EV_PSCAN_REP_MODE:
		printk("HCI_EV_PSCAN_REP_MODE");
		break;
	case HCI_EV_INQUIRY_RESULT_WITH_RSSI:
		printk("HCI_EV_INQUIRY_RESULT_WITH_RSSI");
		break;
	case HCI_EV_REMOTE_EXT_FEATURES:
		printk("HCI_EV_REMOTE_EXT_FEATURES");
		break;
	case HCI_EV_SYNC_CONN_COMPLETE:
		printk("HCI_EV_SYNC_CONN_COMPLETE");
		break;
	case HCI_EV_SYNC_CONN_CHANGED:
		printk("HCI_EV_SYNC_CONN_CHANGED");
		break;
	case HCI_EV_SNIFF_SUBRATE:
		printk("HCI_EV_SNIFF_SUBRATE");
		break;
	case HCI_EV_EXTENDED_INQUIRY_RESULT:
		printk("HCI_EV_EXTENDED_INQUIRY_RESULT");
		break;
	case HCI_EV_IO_CAPA_REQUEST:
		printk("HCI_EV_IO_CAPA_REQUEST");
		break;
	case HCI_EV_SIMPLE_PAIR_COMPLETE:
		printk("HCI_EV_SIMPLE_PAIR_COMPLETE");
		break;
	case HCI_EV_REMOTE_HOST_FEATURES:
		printk("HCI_EV_REMOTE_HOST_FEATURES");
		break;
	default:
		printk("event");
		break;
	}
	printk(":%02x,len:%d,", *opcode, paramLen);
	for (icount = 2; (icount < wlength) && (icount < 24); icount++) {
		printk("%02x ", *(opcode + icount));
	}
	printk("\n");

#endif
}
