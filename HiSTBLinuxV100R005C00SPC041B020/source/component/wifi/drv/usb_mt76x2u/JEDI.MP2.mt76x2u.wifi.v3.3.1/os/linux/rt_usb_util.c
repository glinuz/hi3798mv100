/****************************************************************************

    Module Name:
	rt_usb_util.c

    Abstract:
	Any utility is used in UTIL module for USB function.

    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------

***************************************************************************/

#define RTMP_MODULE_OS
#define RTMP_MODULE_OS_UTIL

#include "rtmp_comm.h"
#include "rt_os_util.h"

#ifdef RTMP_MAC_USB

/*
========================================================================
Routine Description:
	Dump URB information.

Arguments:
	purb_org		- the URB

Return Value:
	None

Note:
========================================================================
*/
void dump_urb(VOID *purb_org)
{
	struct urb *purb = (struct urb *)purb_org;

	printk(KERN_INFO "urb                  :0x%08lx\n", (unsigned long)purb);
	printk(KERN_INFO "\tdev                   :0x%08lx\n", (unsigned long)purb->dev);
	printk(KERN_INFO "\t\tdev->state          :0x%d\n", purb->dev->state);
	printk(KERN_INFO "\tpipe                  :0x%08x\n", purb->pipe);
	printk(KERN_INFO "\tstatus                :%d\n", purb->status);
	printk(KERN_INFO "\ttransfer_flags        :0x%08x\n", purb->transfer_flags);
	printk(KERN_INFO "\ttransfer_buffer       :0x%08lx\n", (unsigned long)purb->transfer_buffer);
	printk(KERN_INFO "\ttransfer_buffer_length:%d\n", purb->transfer_buffer_length);
	printk(KERN_INFO "\tactual_length         :%d\n", purb->actual_length);
	printk(KERN_INFO "\tsetup_packet          :0x%08lx\n", (unsigned long)purb->setup_packet);
	printk(KERN_INFO "\tstart_frame           :%d\n", purb->start_frame);
	printk(KERN_INFO "\tnumber_of_packets     :%d\n", purb->number_of_packets);
	printk(KERN_INFO "\tinterval              :%d\n", purb->interval);
	printk(KERN_INFO "\terror_count           :%d\n", purb->error_count);
	printk(KERN_INFO "\tcontext               :0x%08lx\n", (unsigned long)purb->context);
	printk(KERN_INFO "\tcomplete              :0x%08lx\n\n", (unsigned long)purb->complete);
}

#ifdef CONFIG_STA_SUPPORT
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND

void rausb_autopm_put_interface(void *intf)
{
	usb_autopm_put_interface((struct usb_interface *)intf);
}

int rausb_autopm_get_interface(void *intf)
{
	return usb_autopm_get_interface((struct usb_interface *)intf);
}

/*
========================================================================
Routine Description:
	RTMP_Usb_AutoPM_Put_Interface

Arguments:

Return Value:

Note:
========================================================================
*/

int RTMP_Usb_AutoPM_Put_Interface(IN VOID *pUsb_Devsrc, IN VOID *intfsrc)
{

	INT pm_usage_cnt;
	struct usb_interface *intf = (struct usb_interface *)intfsrc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	pm_usage_cnt = atomic_read(&intf->pm_usage_cnt);
#else
	pm_usage_cnt = intf->pm_usage_cnt;
#endif /* endif */

	if (pm_usage_cnt == 1)
		rausb_autopm_put_interface(intf);

	return 0;
}
EXPORT_SYMBOL(RTMP_Usb_AutoPM_Put_Interface);

/*
========================================================================
Routine Description:
	RTMP_Usb_AutoPM_Get_Interface

Arguments:

Return Value: (-1)  error (resume fail )    1 success ( resume success)  2  (do  nothing)

Note:
========================================================================
*/

int RTMP_Usb_AutoPM_Get_Interface(IN VOID *pUsb_Devsrc, IN VOID *intfsrc)
{

	INT pm_usage_cnt;
	struct usb_interface *intf = (struct usb_interface *)intfsrc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
	pm_usage_cnt = (INT) atomic_read(&intf->pm_usage_cnt);
#else
	pm_usage_cnt = intf->pm_usage_cnt;
#endif /* endif */

	if (pm_usage_cnt == 0) {
		int res = 1;

		res = rausb_autopm_get_interface(intf);
		if (res) {
			DBGPRINT(RT_DEBUG_ERROR, ("AsicSwitchChannel autopm_resume fail ------\n"));
			return (-1);
		}

	}
	return 2;

}
EXPORT_SYMBOL(RTMP_Usb_AutoPM_Get_Interface);

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */
#endif /* CONFIG_STA_SUPPORT */

VOID RtmpOsUsbInitHTTxDesc(IN VOID *pUrbSrc,
			   IN VOID *pUsb_Dev,
			   IN UINT BulkOutEpAddr,
			   IN PUCHAR pSrc,
			   IN ULONG BulkOutSize,
			   IN USB_COMPLETE_HANDLER Func,
			   IN VOID *pTxContext, IN ra_dma_addr_t TransferDma)
{
	PURB pUrb = (PURB) pUrbSrc;
	dma_addr_t DmaAddr = (dma_addr_t) (TransferDma);

	ASSERT(pUrb);

	/*Initialize a tx bulk urb */
	RTUSB_FILL_HTTX_BULK_URB(pUrb,
				 pUsb_Dev,
				 BulkOutEpAddr,
				 pSrc, BulkOutSize, (usb_complete_t) Func, pTxContext, DmaAddr);
}

VOID RtmpOsUsbInitRxDesc(IN VOID *pUrbSrc,
			 IN VOID *pUsb_Dev,
			 IN UINT BulkInEpAddr,
			 IN UCHAR * pTransferBuffer,
			 IN UINT32 BufSize,
			 IN USB_COMPLETE_HANDLER Func,
			 IN VOID *pRxContext, IN ra_dma_addr_t TransferDma)
{
	PURB pUrb = (PURB) pUrbSrc;
	dma_addr_t DmaAddr = (dma_addr_t) (TransferDma);

	ASSERT(pUrb);

	/*Initialize a rx bulk urb */
	RTUSB_FILL_RX_BULK_URB(pUrb,
			       pUsb_Dev,
			       BulkInEpAddr,
			       pTransferBuffer,
			       BufSize, (usb_complete_t) Func, pRxContext, DmaAddr);
}

VOID *RtmpOsUsbContextGet(IN VOID *pUrb)
{
	return ((purbb_t) pUrb)->rtusb_urb_context;
}

NTSTATUS RtmpOsUsbStatusGet(IN VOID *pUrb)
{
	return ((purbb_t) pUrb)->rtusb_urb_status;
}

VOID RtmpOsUsbDmaMapping(IN VOID *pUrb)
{
	RTUSB_URB_DMA_MAPPING(((purbb_t) pUrb));
}

/*
========================================================================
Routine Description:
	Get the data pointer from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the data pointer

Note:
========================================================================
*/
VOID *RtmpOsUsbUrbDataGet(IN VOID *pUrb)
{
	return RTMP_USB_URB_DATA_GET(pUrb);
}

/*
========================================================================
Routine Description:
	Get the status from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the status

Note:
========================================================================
*/
NTSTATUS RtmpOsUsbUrbStatusGet(IN VOID *pUrb)
{
	return RTMP_USB_URB_STATUS_GET(pUrb);
}

/*
========================================================================
Routine Description:
	Get the data length from the URB.

Arguments:
	pUrb			- USB URB

Return Value:
	the data length

Note:
========================================================================
*/
ULONG RtmpOsUsbUrbLenGet(IN VOID *pUrb)
{
	return RTMP_USB_URB_LEN_GET(pUrb);
}

/*
========================================================================
Routine Description:
	Get USB Vendor ID.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the name

Note:
========================================================================
*/
UINT32 RtmpOsGetUsbDevVendorID(IN VOID *pUsbDev)
{
	return ((struct usb_device *)pUsbDev)->descriptor.idVendor;
}

/*
========================================================================
Routine Description:
	Get USB Product ID.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the name

Note:
========================================================================
*/
UINT32 RtmpOsGetUsbDevProductID(IN VOID *pUsbDev)
{
	return ((struct usb_device *)pUsbDev)->descriptor.idProduct;
}

/*
========================================================================
Routine Description:
	Get USB Manufacturer Name.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the Manufacturer Name

Note:
========================================================================
*/

UINT32 RtmpOsGetUsbDevManufacturerName(IN VOID *pUsbDev, IN UCHAR * pBuffer, IN UINT bufferLen)
{
	return usb_string(((struct usb_device *)pUsbDev),
			  ((struct usb_device *)pUsbDev)->descriptor.iManufacturer, pBuffer,
			  bufferLen);
}

/*
========================================================================
Routine Description:
	Get USB Product Name.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the Product Name

Note:
========================================================================
*/

UINT32 RtmpOsGetUsbDevProductName(IN VOID *pUsbDev, IN UCHAR * pBuffer, IN UINT bufferLen)
{
	return usb_string(((struct usb_device *)pUsbDev),
			  ((struct usb_device *)pUsbDev)->descriptor.iProduct, pBuffer, bufferLen);
}

/*
========================================================================
Routine Description:
	Get USB SerialNumber.

Arguments:
	pUsbDev			- the usb device

Return Value:
	the Serial Number

Note:
========================================================================
*/

UINT32 RtmpOsGetUsbDevSerialNumber(IN VOID *pUsbDev, IN UCHAR * pBuffer, IN UINT bufferLen)
{
	return usb_string(((struct usb_device *)pUsbDev),
			  ((struct usb_device *)pUsbDev)->descriptor.iSerialNumber, pBuffer,
			  bufferLen);
}
#endif /* RTMP_MAC_USB */
