/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */


#define RTMP_MODULE_OS

#include "mt76x2_version.h"
#include "rt_config.h"
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"

#ifdef WCX_SUPPORT
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#ifdef WCX_FT_SUPPORT
#include <mach/mt_boot.h>
#endif /* WCX_FT_SUPPORT */
#endif /* WCX_SUPPORT */

#ifdef WOW_INPUTDEV_SUPPORT
#include <linux/input.h>
#endif /* WOW_INPUTDEV_SUPPORT */

#ifdef DEBUGFS_SUPPORT
#include "debugfs.h"
#endif

extern USB_DEVICE_ID rtusb_dev_id[];
extern INT const rtusb_usb_id_len;

static BOOLEAN USBDevConfigInit(struct usb_device *dev, struct usb_interface *intf, VOID *pAd);

#ifndef PF_NOFREEZE
#define PF_NOFREEZE  0
#endif /* endif */

#ifdef MODULE_INIT_DELAY_SLEEP
#define WIFI_INIT_SLEEP_TIME_MS 4000
USHORT init_sleep_ms = WIFI_INIT_SLEEP_TIME_MS;
module_param(init_sleep_ms, ushort, S_IRUGO);
#endif /* MODULE_INIT_DELAY_SLEEP */

#ifdef PROFILE_PATH_DYNAMIC
static char *profilePath = STA_PROFILE_PATH;
module_param(profilePath, charp, S_IRUGO);
#endif /* PROFILE_PATH_DYNAMIC */

#ifdef RESUME_WITH_USB_RESET_SUPPORT
static VOID *gpAd = (VOID *) NULL;
#endif /* endif */


static VOID rtusb_vendor_specific_check(struct usb_device *dev, VOID *pAd)
{

	RT_CMD_USB_MORE_FLAG_CONFIG Config = { dev->descriptor.idVendor,
		dev->descriptor.idProduct
	};
	RTMP_DRIVER_USB_MORE_FLAG_SET(pAd, &Config);
}

#ifdef RESUME_WITH_USB_RESET_SUPPORT
int rtusb_fast_probe(struct os_cookie *handle, VOID **ppAd, struct usb_interface *intf)
{
	VOID *pAd = *ppAd;
	struct os_cookie *pCookie = NULL;

	struct net_device *net_dev = NULL;
	struct usb_device *usb_dev = NULL;
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR WOWRun;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

	pCookie = RTMPCheckOsCookie(handle, &pAd);
	if (pCookie == NULL)
		return NDIS_STATUS_FAILURE;

	usb_dev =  pCookie->pUsb_Dev;
	if (USBDevConfigInit(usb_dev, intf, pAd) == FALSE) {
		RTMPFreeAdapter(pAd);
		return NDIS_STATUS_FAILURE;
	}

	RTMP_DRIVER_USB_INIT(pAd, usb_dev, 0);

	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);
	SET_NETDEV_DEV(net_dev, &(usb_dev->dev));
	set_wiphy_dev(net_dev->ieee80211_ptr->wiphy, &(usb_dev->dev));

/* RESUME */

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	INT pm_usage_cnt;
	UCHAR Flag;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	pm_usage_cnt = atomic_read(&intf->pm_usage_cnt);
#else
	pm_usage_cnt = intf->pm_usage_cnt;
#endif /* endif */

	if (pm_usage_cnt <= 0)
		usb_autopm_get_interface(intf);

	DBGPRINT(RT_DEBUG_ERROR, ("%s():=>autosuspend\n", __func__));

	RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);

	RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_ON(pAd);

	DBGPRINT(RT_DEBUG_ERROR, ("%s(): <=autosuspend\n", __func__));

	return NDIS_STATUS_SUCCESS;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	RTMP_DRIVER_ADAPTER_RT28XX_WOW_RUNSTATUS(pAd, &WOWRun);
	if (WOWRun)
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_DISABLE(pAd);
	else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{

		DBGPRINT(RT_DEBUG_ERROR, ("%s :radio_on\n", __func__));
		RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);
		RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_ON(pAd);
	}

	RTMP_DRIVER_USB_RESUME(pAd);
	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);
	netif_device_attach(net_dev);
	netif_start_queue(net_dev);
	netif_carrier_on(net_dev);
	netif_wake_queue(net_dev);

	DBGPRINT(RT_DEBUG_TRACE, ("<=%s()\n", __func__));
	return NDIS_STATUS_SUCCESS;
}
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

#ifdef WOW_INPUTDEV_SUPPORT
#define WOWLAN_NAME "WOWLAN"
#ifdef WCX_WOW_SUPPORT
static struct input_dev *g_wlan_input_dev;
#endif /* WCX_WOW_SUPPORT */
static int WowRegisterInputDevice(RTMP_ADAPTER *pAd)
{
	int ret = 0;
	struct input_dev *input;

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));
	pAd->input_key = NULL;
	input = input_allocate_device();
	if (!input) {
		ret = -ENOMEM;
		goto err1;
	}

	/* Indicate that we generate key events */
	__set_bit(EV_KEY, input->evbit);

	/* Indicate that we generate *any* key event */
	__set_bit(KEY_POWER, input->keybit);
	input->name = WOWLAN_NAME;
	ret = input_register_device(input);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("register fail(%d)\n", ret));
		goto err2;
	}
	pAd->input_key = input;
#ifdef WCX_WOW_SUPPORT
	g_wlan_input_dev = input;
#endif /* WCX_WOW_SUPPORT */
	DBGPRINT(RT_DEBUG_TRACE, ("%s, register input device success!\n", __func__));

	return ret;

err2:
	input_free_device(input);
err1:
	return ret;
}

static int WowUnRegisterInputDevice(RTMP_ADAPTER *pAd)
{
	if (pAd->input_key)
		input_unregister_device(pAd->input_key);
	pAd->input_key = NULL;

	return 0;
}

static int WowInputReportKey(RTMP_ADAPTER *pAd)
{
	DBGPRINT(RT_DEBUG_TRACE, ("%s, send KEY_POWER to input device!\n", __func__));

	input_report_key(pAd->input_key, KEY_POWER, 1);
	input_sync(pAd->input_key);

	input_report_key(pAd->input_key, KEY_POWER, 0);
	input_sync(pAd->input_key);

	return 0;
}
#endif /* WOW_INPUTDEV_SUPPORT */

#ifdef WCX_WOW_SUPPORT
static irqreturn_t mt76xx_wow_isr(int irq, void *dev)
{
	struct _RTMP_ADAPTER *pAd = (struct _RTMP_ADAPTER *)dev;

	DBGPRINT(RT_DEBUG_ERROR, ("%s()\n", __func__));
	WowInputReportKey(pAd);
	disable_irq_nosync(pAd->wow_irq);
	return IRQ_HANDLED;
}

static int mt76xxRegisterWowIrq(RTMP_ADAPTER *pAd)
{
	struct device_node *eint_node = NULL;

	eint_node = of_find_compatible_node(NULL, NULL, "mediatek, mt76xx_wifi_ctrl");
	if (eint_node) {
		DBGPRINT(RT_DEBUG_TRACE, ("Get mt76xx_ctrl compatible node\n"));
		pAd->wow_irq = irq_of_parse_and_map(eint_node, 0);
		if (pAd->wow_irq) {
			int interrupts[2];

			of_property_read_u32_array(eint_node, "interrupts",
						   interrupts, ARRAY_SIZE(interrupts));
			pAd->wow_irqlevel = interrupts[1];
			if (request_irq(pAd->wow_irq, mt76xx_wow_isr,
					pAd->wow_irqlevel, INF_MAIN_DEV_NAME, pAd))
				DBGPRINT(RT_DEBUG_ERROR, ("MT76xx WOWIRQ LINE NOT AVAILABLE!!\n"));
			else
				disable_irq_nosync(pAd->wow_irq);

		} else
			DBGPRINT(RT_DEBUG_ERROR, ("can't find mt76xx_ctrl irq\n"));

	} else {
		pAd->wow_irq = 0;
		DBGPRINT(RT_DEBUG_ERROR, ("can't find mt76xx_ctrl compatible node\n"));
	}
	return 0;
}

static int mt76xxUnRegisterWowIrq(RTMP_ADAPTER *pAd)
{
	if (pAd->wow_irq)
		free_irq(pAd->wow_irq, pAd);
	return 0;
}
#endif /* WCX_WOW_SUPPORT */

static int rt2870_probe(struct usb_interface *intf,
			struct usb_device *usb_dev, const USB_DEVICE_ID *dev_id, VOID **ppAd)
{
	struct net_device *net_dev = NULL;
#ifdef RESUME_WITH_USB_RESET_SUPPORT
	VOID *pAd = (VOID *) gpAd;
#else
	VOID *pAd = (VOID *) NULL;
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

	INT status, rv;
	struct os_cookie *handle;
	RTMP_OS_NETDEV_OP_HOOK netDevHook;
	ULONG OpMode;
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
/*	INT pm_usage_cnt; */
	INT res = 1;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	DBGPRINT(RT_DEBUG_TRACE, ("===>rt2870_probe()!\n"));

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	res = usb_autopm_get_interface(intf);
	if (res) {
		DBGPRINT(RT_DEBUG_ERROR, ("rt2870_probe autopm_resume fail ------\n"));
		return -EIO;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	atomic_set(&intf->pm_usage_cnt, 1);
	printk(KERN_DEBUG " rt2870_probe ====> pm_usage_cnt %d\n", atomic_read(&intf->pm_usage_cnt));

#else
	intf->pm_usage_cnt = 1;
	printk(KERN_DEBUG " rt2870_probe ====> pm_usage_cnt %d\n", intf->pm_usage_cnt);
#endif /* endif */

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	handle = RtmpOsVmalloc(sizeof(struct os_cookie));
	if (!handle) {
		printk(KERN_ERR "rt2870_probe(): Allocate memory for os handle failed!\n");
		return -ENOMEM;
	}
	memset(handle, 0, sizeof(struct os_cookie));

	handle->pUsb_Dev = usb_dev;

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	handle->intf = intf;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	/* set/get operators to/from DRIVER module */

#ifdef RESUME_WITH_USB_RESET_SUPPORT
	if (rtusb_fast_probe(handle, &pAd, intf) == NDIS_STATUS_SUCCESS) {
		*ppAd = pAd;
		goto fast_probe_done;
	}
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

	rv = RTMPAllocAdapterBlock(handle, &pAd);
	if (rv != NDIS_STATUS_SUCCESS) {
		RtmpOsVfree(handle);
		goto err_out;
	}

	if (USBDevConfigInit(usb_dev, intf, pAd) == FALSE)
		goto err_out_free_radev;

	RTMP_DRIVER_USB_INIT(pAd, usb_dev, dev_id->driver_info);

	net_dev = RtmpPhyNetDevInit(pAd, &netDevHook);
	if (net_dev == NULL)
		goto err_out_free_radev;

	/* Here are the net_device structure with usb specific parameters. */
#ifdef NATIVE_WPA_SUPPLICANT_SUPPORT
	/* for supporting Network Manager.
	 * Set the sysfs physical device reference for the network logical device if set prior to registration will
	 * cause a symlink during initialization.
	 */
	SET_NETDEV_DEV(net_dev, &(usb_dev->dev));
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
/*    pAd->StaCfg.OriDevType = net_dev->type; */
	RTMP_DRIVER_STA_DEV_TYPE_SET(pAd, net_dev->type);
#ifdef PROFILE_PATH_DYNAMIC
	RTMP_DRIVER_STA_PROFILEPATH_SET(pAd, (ULONG) profilePath);
#endif /* PROFILE_PATH_DYNAMIC */
#endif /* CONFIG_STA_SUPPORT */

/*All done, it's time to register the net device to linux kernel. */
	/* Register this device */
#ifdef RT_CFG80211_SUPPORT
	{
/*	pAd->pCfgDev = &(usb_dev->dev); */
/*	pAd->CFG80211_Register = CFG80211_Register; */
/*	RTMP_DRIVER_CFG80211_INIT(pAd, usb_dev); */

		/*
		   In 2.6.32, cfg80211 register must be before register_netdevice();
		   We can not put the register in rt28xx_open();
		   Or you will suffer NULL pointer in list_add of
		   cfg80211_netdev_notifier_call().
		 */
		CFG80211_Register(pAd, &(usb_dev->dev), net_dev);
	}
#endif /* RT_CFG80211_SUPPORT */

#ifdef PRE_ASSIGN_MAC_ADDR
	/* Read MAC Address from eeprom, and copy it to netdev hook address */
	RTMP_DRIVER_MAC_ADDR_GET(pAd, &netDevHook.devAddr[0]);
	DBGPRINT(RT_DEBUG_TRACE, ("%s(): MAC Addr [%02x:%02x:%02x:%02x:%02x:%02x] from EEPROM\n",
							__func__,
							(UINT)netDevHook.devAddr[0],
							(UINT)netDevHook.devAddr[1],
							(UINT)netDevHook.devAddr[2],
							(UINT)netDevHook.devAddr[3],
							(UINT)netDevHook.devAddr[4],
							(UINT)netDevHook.devAddr[5]));
#endif /* PRE_ASSIGN_MAC_ADDR */

	RTMP_DRIVER_OP_MODE_GET(pAd, &OpMode);
	status = RtmpOSNetDevAttach(OpMode, net_dev, &netDevHook);
	if (status != 0)
		goto err_out_free_netdev;

#ifdef DEBUGFS_SUPPORT
	mt_dev_debugfs_init(pAd);
#endif /* DEBUGFS_SUPPORT */

/*#ifdef KTHREAD_SUPPORT */

	*ppAd = pAd;
#ifdef RESUME_WITH_USB_RESET_SUPPORT
	gpAd = pAd;
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

#ifdef INF_PPA_SUPPORT
	RTMP_DRIVER_INF_PPA_INIT(pAd);
#endif /* INF_PPA_SUPPORT */

#ifdef EXT_BUILD_CHANNEL_LIST
	RTMP_DRIVER_SET_PRECONFIG_VALUE(pAd);
#endif /* EXT_BUILD_CHANNEL_LIST */
#ifdef WCX_FT_SUPPORT
	{
		BOOTMODE boot_mode;

		boot_mode = get_boot_mode();
		if (boot_mode == FACTORY_BOOT) {
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s, open in Factory boot, ifc_up itself\n", __func__));
			rt28xx_open(net_dev);
		}
	}
#endif /* WCX_FT_SUPPORT */
#ifdef WCX_WOW_SUPPORT
	mt76xxRegisterWowIrq(pAd);
#endif /* WCX_WOW_SUPPORT */
#ifdef WOW_INPUTDEV_SUPPORT
	WowRegisterInputDevice(pAd);
#endif /* WOW_INPUTDEV_SUPPORT */
#ifdef CFG80211_STA_AP_CONCURRENT
	RTMP_CFG80211_MultiApIf_Init(pAd);
#endif /* CFG80211_STA_AP_CONCURRENT */
	DBGPRINT(RT_DEBUG_TRACE, ("<===rt2870_probe()!\n"));

	return 0;

	/* --------------------------- ERROR HANDLE --------------------------- */
err_out_free_netdev:
	RtmpOSNetDevFree(net_dev);
	net_dev = NULL;

err_out_free_radev:
	RTMPFreeAdapter(pAd);

err_out:
	*ppAd = NULL;

	return -1;

#ifdef RESUME_WITH_USB_RESET_SUPPORT
fast_probe_done:
	printk(KERN_DEBUG "fast probe done\n");
	return 0;
#endif /* RESUME_WITH_USB_RESET_SUPPORT */
}

/*
========================================================================
Routine Description:
    Release allocated resources.

Arguments:
    *dev				Point to the PCI or USB device
	pAd					driver control block pointer

Return Value:
    None

Note:
========================================================================
*/
static void rt2870_disconnect(struct usb_device *dev, VOID *pAd)
{
	struct net_device *net_dev;

	DBGPRINT(RT_DEBUG_ERROR, ("rtusb_disconnect: unregister usbnet usb-%s-%s\n",
				  dev->bus->bus_name, dev->devpath));
	if (!pAd) {
		usb_put_dev(dev);

		printk(KERN_ERR "rtusb_disconnect: pAd == NULL!\n");
		return;
	}
/*	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST); */
	RTMP_DRIVER_NIC_NOT_EXIST_SET(pAd);

	/* for debug, wait to show some messages to /proc system */
	udelay(1);

	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);

	RtmpPhyNetDevExit(pAd, net_dev);

#ifdef RT_CFG80211_SUPPORT
	RTMP_DRIVER_80211_UNREGISTER(pAd, net_dev);
#endif /* RT_CFG80211_SUPPORT */

	/* free the root net_device */
/* RtmpOSNetDevFree(net_dev); */
#ifdef WCX_WOW_SUPPORT
	mt76xxUnRegisterWowIrq(pAd);
#endif /* WCX_WOW_SUPPORT */
#ifdef WOW_INPUTDEV_SUPPORT
	DBGPRINT(RT_DEBUG_TRACE, ("WowRegisterInputDevice...\n"));
	WowUnRegisterInputDevice(pAd);
#endif /* WOW_INPUTDEV_SUPPORT */

#ifdef DEBUGFS_SUPPORT
	mt_dev_debugfs_remove(pAd);
#endif /* DEBUGFS_SUPPORT */

	RtmpRaDevCtrlExit(pAd);

	/* free the root net_device */
	RtmpOSNetDevFree(net_dev);
	net_dev = NULL;

	/* release a use of the usb device structure */
	usb_put_dev(dev);
	udelay(1);



#ifdef RESUME_WITH_USB_RESET_SUPPORT
	gpAd = NULL;
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

	DBGPRINT(RT_DEBUG_ERROR, (" RTUSB disconnect successfully\n"));
}

/**************************************************************************/
/**************************************************************************/
/*tested for kernel 2.6series */
/**************************************************************************/
/**************************************************************************/

#ifdef CONFIG_PM
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 10)
#define pm_message_t u32
#endif /* endif */
static int rtusb_suspend(struct usb_interface *intf, pm_message_t state)
{
	struct net_device *net_dev;
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR WOWEnable;
#endif /* endif */
	VOID *pAd = usb_get_intfdata(intf);

	DBGPRINT(RT_DEBUG_TRACE, ("===> rtusb_suspend()\n"));
	if (!RTMP_TEST_FLAG((PRTMP_ADAPTER) pAd, fRTMP_ADAPTER_START_UP))
		goto out;

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	UCHAR Flag;

	DBGPRINT(RT_DEBUG_ERROR, ("%s():=>autosuspend\n", __func__));

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (WOWEnable == TRUE)
		;
	else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
/*	if(!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF)) */
		RTMP_DRIVER_ADAPTER_END_DISSASSOCIATE(pAd);
		RTMP_DRIVER_ADAPTER_IDLE_RADIO_OFF_TEST(pAd, &Flag);
		if (!Flag) {
			/*RT28xxUsbAsicRadioOff(pAd); */
			RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_OFF(pAd);
		}
	}
	/*RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND); */
	RTMP_DRIVER_ADAPTER_SUSPEND_SET(pAd);
	return 0;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	RTMP_DRIVER_ADAPTER_RT28XX_WOW_READY(pAd, &WOWEnable);
	if (WOWEnable) {
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_ENABLE(pAd);
#ifdef WCX_WOW_SUPPORT
		irq_set_irq_wake(((struct _RTMP_ADAPTER *)pAd)->wow_irq, TRUE);
		enable_irq(((struct _RTMP_ADAPTER *)pAd)->wow_irq);
#endif /* WCX_WOW_SUPPORT */
	} else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
		DBGPRINT(RT_DEBUG_ERROR, ("%s :radio_off\n", __func__));
		RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_OFF(pAd);
	}

	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);
	if (!net_dev) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: ERROR! net_dev=NULL!\n", __func__));
	} else {
		netif_device_detach(net_dev);
		RTMP_DRIVER_USB_SUSPEND(pAd, netif_running(net_dev));
		}

out:
	DBGPRINT(RT_DEBUG_TRACE, ("<=%s()\n", __func__));
	return 0;
}

#ifdef RESUME_WITH_USB_RESET_SUPPORT
static int rtusb_resume(struct usb_interface *intf)
{
	/* host usb driver is not available yet when usb_device's resume function is callback */
	DBGPRINT(RT_DEBUG_OFF, ("<= Dummy %s()\n", __func__));
	return 0;
}
#else
static int rtusb_resume(struct usb_interface *intf)
{
	struct net_device *net_dev;
	VOID *pAd = usb_get_intfdata(intf);
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR WOWRun;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	INT pm_usage_cnt;
	UCHAR Flag;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

	DBGPRINT(RT_DEBUG_TRACE, ("%s()=>\n", __func__));
	if (!RTMP_TEST_FLAG((PRTMP_ADAPTER) pAd, fRTMP_ADAPTER_START_UP))
		goto out;

#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	pm_usage_cnt = atomic_read(&intf->pm_usage_cnt);
#else
	pm_usage_cnt = intf->pm_usage_cnt;
#endif /* endif */

	if (pm_usage_cnt <= 0)
		usb_autopm_get_interface(intf);

	DBGPRINT(RT_DEBUG_ERROR, ("%s():=>autosuspend\n", __func__));

	/*RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND); */
	RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	if (WOWEnable == FALSE)
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
		/*RT28xxUsbAsicRadioOn(pAd); */
		RTMP_DRIVER_ADAPTER_RT28XX_USB_ASICRADIO_ON(pAd);

	DBGPRINT(RT_DEBUG_ERROR, ("%s(): <=autosuspend\n", __func__));

	return 0;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	RTMP_DRIVER_ADAPTER_RT28XX_WOW_RUNSTATUS(pAd, &WOWRun);

	if (WOWRun == TRUE) {
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_DISABLE(pAd);

#ifdef WCX_WOW_SUPPORT
		disable_irq_nosync(((struct _RTMP_ADAPTER *)pAd)->wow_irq);
#endif /* WCX_WOW_SUPPORT */
	} else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
		RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);
		DBGPRINT(RT_DEBUG_ERROR, ("%s :radio_on\n", __func__));
		RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_ON(pAd);
	}

	RTMP_DRIVER_USB_RESUME(pAd);
	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);
	netif_device_attach(net_dev);
	netif_start_queue(net_dev);
	netif_carrier_on(net_dev);
	netif_wake_queue(net_dev);

out:
	DBGPRINT(RT_DEBUG_TRACE, ("<=%s()\n", __func__));
	return 0;
}
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

#ifdef NEW_FAST_CONNECT_SCHEME_SUPPORT
static int rtusb_reset_resume(struct usb_interface *intf)
{
	struct net_device *net_dev;
	VOID *pAd = usb_get_intfdata(intf);
#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	UCHAR WOWRun;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */

	DBGPRINT(RT_DEBUG_TRACE, ("%s()=>\n", __func__));

	if (!RTMP_TEST_FLAG((PRTMP_ADAPTER) pAd, fRTMP_ADAPTER_START_UP))
		goto out;

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	RTMP_DRIVER_ADAPTER_RT28XX_WOW_RUNSTATUS(pAd, &WOWRun);

	if (WOWRun == TRUE) {
		RTMP_DRIVER_ADAPTER_RT28XX_WOW_DISABLE(pAd);
#ifdef WCX_WOW_SUPPORT
		disable_irq_nosync(((struct _RTMP_ADAPTER *)pAd)->wow_irq);
#endif /* WCX_WOW_SUPPORT */
	} else
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
	{
		RTMP_DRIVER_ADAPTER_SUSPEND_CLEAR(pAd);
		DBGPRINT(RT_DEBUG_ERROR, ("%s :radio_on\n", __func__));
		RTMP_DRIVER_ADAPTER_RT28XX_CMD_RADIO_ON(pAd);
	}

	RTMP_DRIVER_USB_RESUME(pAd);
	RTMP_DRIVER_NET_DEV_GET(pAd, &net_dev);
	netif_device_attach(net_dev);
	netif_start_queue(net_dev);
	netif_carrier_on(net_dev);
	netif_wake_queue(net_dev);

out:
	DBGPRINT(RT_DEBUG_TRACE, ("<=%s()\n", __func__));
	return 0;
}

#endif

#endif /* CONFIG_PM */

static BOOLEAN USBDevConfigInit(struct usb_device *dev, struct usb_interface *intf, VOID *pAd)
{
	struct usb_host_interface *iface_desc;
	ULONG BulkOutIdx;
	ULONG BulkInIdx;
	UINT32 i;
	RT_CMD_USB_DEV_CONFIG Config, *pConfig = &Config;

	/* get the active interface descriptor */
	iface_desc = intf->cur_altsetting;

	/* get # of enpoints  */
	pConfig->NumberOfPipes = iface_desc->desc.bNumEndpoints;
	DBGPRINT(RT_DEBUG_TRACE, ("NumEndpoints=%d\n", iface_desc->desc.bNumEndpoints));

	/* Configure Pipes */
	BulkOutIdx = 0;
	BulkInIdx = 0;

	for (i = 0; i < pConfig->NumberOfPipes; i++) {
		if ((iface_desc->endpoint[i].desc.bmAttributes == USB_ENDPOINT_XFER_BULK) &&
		    ((iface_desc->endpoint[i].desc.bEndpointAddress & USB_ENDPOINT_DIR_MASK) ==
		     USB_DIR_IN)) {
			if (BulkInIdx < 2) {
				pConfig->BulkInEpAddr[BulkInIdx++] =
				    iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 11)
				pConfig->BulkInMaxPacketSize =
				    le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
				pConfig->BulkInMaxPacketSize =
				    iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif /* LINUX_VERSION_CODE */

				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("BULK IN MaxPacketSize = %d\n",
					      pConfig->BulkInMaxPacketSize));
				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("EP address = 0x%2x\n",
					      iface_desc->endpoint[i].desc.bEndpointAddress));
			} else {
				DBGPRINT(RT_DEBUG_ERROR, ("Bulk IN endpoint nums large than 2\n"));
			}
		} else if ((iface_desc->endpoint[i].desc.bmAttributes == USB_ENDPOINT_XFER_INT) &&
		    ((iface_desc->endpoint[i].desc.bEndpointAddress & USB_ENDPOINT_DIR_MASK) ==
		     USB_DIR_IN)) {
				pConfig->IntrInEpAddr =
				    iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 11)
				pConfig->BulkInMaxPacketSize =
				    le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
				pConfig->BulkInMaxPacketSize =
				    iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif /* LINUX_VERSION_CODE */

				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("[Interrupt Transfer Type] IN MaxPacketSize = %d\n",
					      pConfig->BulkInMaxPacketSize));
				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("[Interrupt Transfer Type] EP address = 0x%2x\n",
					      iface_desc->endpoint[i].desc.bEndpointAddress));
		} else if ((iface_desc->endpoint[i].desc.bmAttributes == USB_ENDPOINT_XFER_BULK) &&
			   ((iface_desc->endpoint[i].desc.
			     bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT)) {
			if (BulkOutIdx < 6) {
				/* there are 6 bulk out EP. EP6 highest priority. */
				/* EP1-4 is EDCA.  EP5 is HCCA. */
				pConfig->BulkOutEpAddr[BulkOutIdx++] =
				    iface_desc->endpoint[i].desc.bEndpointAddress;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 11)
				pConfig->BulkOutMaxPacketSize =
				    le2cpu16(iface_desc->endpoint[i].desc.wMaxPacketSize);
#else
				pConfig->BulkOutMaxPacketSize =
				    iface_desc->endpoint[i].desc.wMaxPacketSize;
#endif /* endif */

				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("BULK OUT MaxPacketSize = %d\n",
					      pConfig->BulkOutMaxPacketSize));
				DBGPRINT_RAW(RT_DEBUG_TRACE,
					     ("EP address = 0x%2x\n",
					      iface_desc->endpoint[i].desc.bEndpointAddress));
			} else {
				DBGPRINT(RT_DEBUG_ERROR, ("Bulk Out endpoint nums large than 6\n"));
			}
		}
	}

	if (!(pConfig->BulkInEpAddr[0] && pConfig->BulkOutEpAddr[0])) {
		DBGPRINT_RAW(RT_DEBUG_ERROR, ("%s: Could not find %s %s endpoints\n",
			__func__, pConfig->BulkInEpAddr[0] ? "" : "bulk-in",
			pConfig->BulkOutEpAddr[0] ? "" : "bulk-out"));
		return FALSE;
	}

	pConfig->pConfig = &dev->config->desc;
	usb_set_intfdata(intf, pAd);
	RTMP_DRIVER_USB_CONFIG_INIT(pAd, pConfig);
	rtusb_vendor_specific_check(dev, pAd);

	return TRUE;

}

static int rtusb_probe(struct usb_interface *intf, const USB_DEVICE_ID *id)
{
	VOID *pAd;
	struct usb_device *dev;
	int rv;

	dev = interface_to_usbdev(intf);
	dev = usb_get_dev(dev);

	rv = rt2870_probe(intf, dev, id, &pAd);
	if (rv != 0)
		usb_put_dev(dev);
	return rv;
}

static void rtusb_disconnect(struct usb_interface *intf)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	VOID *pAd;

	pAd = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);

	rt2870_disconnect(dev, pAd);

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	printk(KERN_DEBUG "rtusb_disconnect usb_autopm_put_interface\n");

	usb_autopm_put_interface(intf);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	printk(KERN_DEBUG "%s() => pm_usage_cnt %d\n", __func__, atomic_read(&intf->pm_usage_cnt));
#else
	printk(KERN_DEBUG "%s() => pm_usage_cnt %d\n", __func__, intf->pm_usage_cnt);
#endif /* endif */
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

}

struct usb_driver rtusb_driver = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15)
	.owner = THIS_MODULE,
#endif /* endif */
	.name = RTMP_DRV_NAME,
	.probe = rtusb_probe,
	.disconnect = rtusb_disconnect,
	.id_table = rtusb_dev_id,

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	.supports_autosuspend = 1,
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
	.suspend = rtusb_suspend,
	.resume = rtusb_resume,
#ifdef NEW_FAST_CONNECT_SCHEME_SUPPORT
	.reset_resume = rtusb_reset_resume,
#endif
#endif /* CONFIG_PM */
};

static INT __init rtusb_init(void)
{
	INT res = 0;

	printk(KERN_DEBUG "%s : %s --->\n", __func__, RTMP_DRV_NAME);

#ifdef DEBUGFS_SUPPORT
	mt_debugfs_init();
#endif
	res = usb_register(&rtusb_driver);
#ifdef MODULE_INIT_DELAY_SLEEP
	DBGPRINT(RT_DEBUG_OFF, ("%s : sleeping for %d ms\n", __func__, init_sleep_ms));
	if (init_sleep_ms)
		msleep(init_sleep_ms);
#endif /* MODULE_INIT_DELAY_SLEEP */

	printk(KERN_DEBUG "Exiting %s\n", __func__);
	return res;
}

static VOID __exit rtusb_exit(void)
{
	usb_deregister(&rtusb_driver);
#ifdef DEBUGFS_SUPPORT
	mt_debugfs_remove();
#endif
	printk(KERN_DEBUG "<--- rtusb exit\n");
}

#ifndef MULTI_INF_SUPPORT

module_init(rtusb_init);
module_exit(rtusb_exit);

/* Following information will be show when you run 'modinfo' */
/* *** If you have a solution for the bug in current version of driver, please mail to me. */
/* Otherwise post to forum in ralinktech's web site(www.ralinktech.com) and let all users help you. *** */
MODULE_AUTHOR("Ralink");
MODULE_DESCRIPTION("Ralink Wireless Lan Linux Driver");
MODULE_LICENSE("Dual BSD/GPL");

#ifdef MODULE_VERSION
MODULE_VERSION(DRIVER_VERSION);
#endif /* endif */

#endif /* MULTI_INF_SUPPORT */
