#if !defined(__DWC_PCD_H__)
#define __DWC_PCD_H__

#include <include/asm/errno.h>
#include <include/asm/io.h>
#include <common.h>

#include "usb.h"
#include "dev_list.h"
#include "dev_regs.h"

#define DWC_E_INVALID		EINVAL
#define DWC_E_NO_MEMORY		ENOMEM
#define DWC_E_NO_DEVICE		ENODEV
#define DWC_E_NOT_SUPPORTED	EOPNOTSUPP
#define DWC_E_TIMEOUT		ETIMEDOUT
#define DWC_E_BUSY		EBUSY
#define DWC_E_AGAIN		EAGAIN
#define DWC_E_RESTART		ERESTART
#define DWC_E_ABORT		ECONNABORTED
#define DWC_E_SHUTDOWN		ESHUTDOWN
#define DWC_E_NO_DATA		ENODATA
#define DWC_E_DISCONNECT	ECONNRESET
#define DWC_E_UNKNOWN		EINVAL
#define DWC_E_NO_STREAM_RES	ENOSR
#define DWC_E_COMMUNICATION	ECOMM
#define DWC_E_OVERFLOW		EOVERFLOW
#define DWC_E_PROTOCOL		EPROTO
#define DWC_E_IN_PROGRESS	EINPROGRESS
#define DWC_E_PIPE		EPIPE
#define DWC_E_IO		EIO
#define DWC_E_NO_SPACE		ENOSPC

#define _USB_COMMON_DEFINES_H

#define GET_STATUS           0
#define CLEAR_FEATURE        1
#define SET_FEATURE          3
#define SET_ADDRESS          5
#define GET_DESCRIPTOR       6
#define SET_DESCRIPTOR       7
#define GET_CONFIGURATION    8
#define SET_CONFIGURATION    9
#define GET_INTERFACE        10
#define SET_INTERFACE        11
#define SYNCH_FRAME          12

#define TYPE_DEVICE          1
#define TYPE_CONFIGURATION   2
#define TYPE_STRING          3
#define TYPE_INTERFACE       4
#define TYPE_ENDPOINT        5

#define DEVICE_READ          0x80
#define DEVICE_WRITE         0x00
#define INTERFACE_READ       0x81
#define INTERFACE_WRITE      0x01
#define ENDPOINT_READ        0x82
#define ENDPOINT_WRITE       0x02

/** Maxpacket size for EP0 */
#define MAX_EP0_SIZE	64
/** Maxpacket size for any EP */
#define MAX_PACKET_SIZE 512

//** Maximum number of Endpoints/HostChannels */
#define MAX_EPS_CHANNELS 16

#define EP0 0
#define EPIN 1
#define EPOUT 2

#ifdef CONFIG_USB_DEVICE_DBG
#define DWC_PRINTF(fmt, args...) printf( fmt , ## args)
#else
#define DWC_PRINTF(fmt, args...) 
#endif

#define REG_GLB_ADDR REG_BASE_USB2_DEVICE
#define REG_GOTGCTL (REG_GLB_ADDR+0X000)
#define REG_GOTGINT (REG_GLB_ADDR+0X004)
#define REG_GAHBCFG (REG_GLB_ADDR+0X008)
#define REG_GUSBCFG (REG_GLB_ADDR+0X00C)
#define REG_GRSTCTL (REG_GLB_ADDR+0X010)
#define REG_GINTSTS (REG_GLB_ADDR+0X014)
#define REG_GINTMSK (REG_GLB_ADDR+0X018)
#define REG_GRXSTSR (REG_GLB_ADDR+0X01C)
#define REG_GRXSTSP (REG_GLB_ADDR+0X020)
#define REG_GRXFSIZ (REG_GLB_ADDR+0X024)
#define REG_GNPTXFSIZ (REG_GLB_ADDR+0X028)
#define REG_GNPTXSTS (REG_GLB_ADDR+0X02C)
#define REG_GI2CCTL (REG_GLB_ADDR+0X030)
#define REG_GPVNDCTL (REG_GLB_ADDR+0X034)
#define REG_GGPIO (REG_GLB_ADDR+0X038)
#define REG_GUID (REG_GLB_ADDR+0X03C)
#define REG_GSNPSID (REG_GLB_ADDR+0X040)
#define REG_GHWCFG1 (REG_GLB_ADDR+0X044)
#define REG_GHWCFG2 (REG_GLB_ADDR+0X048)
#define REG_GHWCFG3 (REG_GLB_ADDR+0X04C)
#define REG_GHWCFG4 (REG_GLB_ADDR+0X050)
#define REG_GLPMCFG (REG_GLB_ADDR+0X054)
#define REG_GPWRDN (REG_GLB_ADDR+0X058)
#define REG_GDFIFOCFG (REG_GLB_ADDR+0X05C)
#define REG_GADPCTL (REG_GLB_ADDR+0X060)
#define REG_HPTXFSIZ (REG_GLB_ADDR+0X100)
#define REG_DPTXFSIZN (REG_GLB_ADDR+0X104)
#define REG_DIEPTXFN (REG_GLB_ADDR+0X104)

#define REG_DEV_ADDR (REG_GLB_ADDR+0X800)
#define REG_DCFG (REG_DEV_ADDR+0X000)
#define REG_DCTL (REG_DEV_ADDR+0X004)
#define REG_DSTS (REG_DEV_ADDR+0X008)
#define REG_DIEMSK (REG_DEV_ADDR+0X010)
#define REG_DOEMSK (REG_DEV_ADDR+0X014)
#define REG_DAINT (REG_DEV_ADDR+0X018)
#define REG_DAINTMSK (REG_DEV_ADDR+0X01C)
#define REG_DTKNQR1 (REG_DEV_ADDR+0X020)
#define REG_DTKNQR2 (REG_DEV_ADDR+0X024)
#define REG_DTKNQR3 (REG_DEV_ADDR+0X030)
#define REG_DTKNQR4 (REG_DEV_ADDR+0X034)
#define REG_DVBUSDIS (REG_DEV_ADDR+0X028)
#define REG_DVBUSPULSE (REG_DEV_ADDR+0X02C)
#define REG_DTHRCTL (REG_DEV_ADDR+0X030)
#define REG_DIEPEMPMAK (REG_DEV_ADDR+0X034)
#define REG_DEACHINT (REG_DEV_ADDR+0X038)
#define REG_DEACHINTMSK (REG_DEV_ADDR+0X03C)

#define REG_INEP0_ADDR (REG_GLB_ADDR+0x900)
#define REG_DIEPCTL0 (REG_INEP0_ADDR+0x000)
#define REG_DIEPINT0 (REG_INEP0_ADDR+0x008)
#define REG_DIEPTSIZ0 (REG_INEP0_ADDR+0x010)
#define REG_DTXFSTS0 (REG_INEP0_ADDR+0x018)
#define REG_DIEPDMA0 (REG_INEP0_ADDR+0x014)

#define REG_INEP1_ADDR (REG_GLB_ADDR+0x920)
#define REG_DIEPCTL1 (REG_INEP1_ADDR+0x000)
#define REG_DIEPINT1 (REG_INEP1_ADDR+0x008)
#define REG_DIEPTSIZ1 (REG_INEP1_ADDR+0x010)
#define REG_DTXFSTS1 (REG_INEP1_ADDR+0x018)
#define REG_DIEPDMA1 (REG_INEP1_ADDR+0x014)

#define REG_OUTEP0_ADDR (REG_GLB_ADDR+0xb00)
#define REG_DOEPCTL0 (REG_OUTEP0_ADDR+0x000)
#define REG_DOEPINT0 (REG_OUTEP0_ADDR+0x008)
#define REG_DOEPTSIZ0 (REG_OUTEP0_ADDR+0x010)
#define REG_DOEPDMA0 (REG_OUTEP0_ADDR+0x014)

#define REG_OUTEP2_ADDR (REG_GLB_ADDR+0xb40)
#define REG_DOEPCTL2 (REG_OUTEP2_ADDR+0x000)
#define REG_DOEPINT2 (REG_OUTEP2_ADDR+0x008)
#define REG_DOEPTSIZ2 (REG_OUTEP2_ADDR+0x010)
#define REG_DOEPDMA2 (REG_OUTEP2_ADDR+0x014)

#define REG_DATAFIFO0 (REG_GLB_ADDR+0x1000)
#define REG_DATAFIFO1 (REG_GLB_ADDR+0x2000)
#define REG_PCGCCTL (REG_GLB_ADDR+0xE00)

typedef dma_addr_t dwc_dma_t;

/**
 * The <code>dwc_ep</code> structure represents the state of a single
 * endpoint when acting in device mode. It contains the data items
 * needed for an endpoint to be activated and transfer packets.
 */
typedef struct dwc_ep {
	/** EP number used for register address lookup */
	uint8_t num;
	/** EP direction 0 = OUT */
	unsigned is_in:1;
	/** EP active. */
	unsigned active:1;

	/**
	 * Periodic Tx FIFO # for IN EPs For INTR EP set to 0 to use non-periodic 
	 * Tx FIFO. If dedicated Tx FIFOs are enabled Tx FIFO # FOR IN EPs*/
	unsigned tx_fifo_num:4;
	/** EP type: 0 - Control, 1 - ISOC,	 2 - BULK,	3 - INTR */
	unsigned type:2;
#define DWC_OTG_EP_TYPE_CONTROL	   0
#define DWC_OTG_EP_TYPE_ISOC	   1
#define DWC_OTG_EP_TYPE_BULK	   2
#define DWC_OTG_EP_TYPE_INTR	   3

	/** DATA start PID for INTR and BULK EP */
	unsigned data_pid_start:1;
	/** Frame (even/odd) for ISOC EP */
	unsigned even_odd_frame:1;
	/** Max Packet bytes */
	unsigned maxpacket:11;

	/** Max Transfer size */
	uint32_t maxxfer;

	/** @name Transfer state */
	/** @{ */
	dwc_dma_t dma_addr;

	uint8_t *start_xfer_buff;
	/** pointer to the transfer buffer */
	uint8_t *xfer_buff;
	/** Number of bytes to transfer */
	unsigned xfer_len:19;
	/** Number of bytes transferred. */
	unsigned xfer_count:19;
	/** Sent ZLP */
	unsigned sent_zlp:1;
	/** Total len for control transfer */
	unsigned total_len:19;

	/** stall clear flag */
	unsigned stall_clear_flag:1;

	/** SETUP pkt cnt rollover flag for EP0 out*/
	unsigned stp_rollover;

	/** bInterval */
	uint32_t bInterval;
	/** Next frame num to setup next ISOC transfer */
	uint32_t frame_num;
	/** Indicates SOF number overrun in DSTS */
	uint8_t frm_overrun;


/** @} */
} dwc_ep_t;


/**
 * The <code>dwc_otg_core_if</code> structure contains information needed to manage
 * the DWC_otg controller acting in either host or device mode. It
 * represents the programming view of the controller as a whole.
 */
typedef struct dwc_otg_core_if {

	/** Value from SNPSID register */
	uint32_t snpsid;

	/*
	 * Set to 1 if the core PHY interface bits in USBCFG have been
	 * initialized.
	 */
	uint8_t phy_init_done;



	/** Push/pop addresses for endpoints or host channels.*/
	uint32_t *data_fifo[MAX_EPS_CHANNELS];
#define DWC_OTG_DATA_FIFO_OFFSET 0x1000
#define DWC_OTG_DATA_FIFO_SIZE 0x1000

	/** Total RAM for FIFOs (Bytes) */
	uint16_t total_fifo_size;
	/** Size of Rx FIFO (Bytes) */
	uint16_t rx_fifo_size;
	/** Size of Non-periodic Tx FIFO (Bytes) */
	uint16_t nperio_tx_fifo_size;


	/** 1 if DMA descriptor is enabled, 0 otherwise. */
	uint8_t dma_enable;
	/** 1 if PTI Enhancement mode is enabled, 0 otherwise. */

	/** 1 if MPI Enhancement mode is enabled, 0 otherwise. */

	/** 1 if dedicated Tx FIFOs are enabled, 0 otherwise. */
	uint8_t en_multiple_tx_fifo;

	/** Set to 1 if multiple packets of a high-bandwidth transfer is in
	 * process of being queued */
	uint8_t queuing_high_bandwidth;

	/** Hardware Configuration -- stored here for convenience.*/
	hwcfg2_data_t hwcfg2;
	hwcfg3_data_t hwcfg3;
	hwcfg4_data_t hwcfg4;
	fifosize_data_t hptxfsiz;

	dcfg_data_t dcfg;

	/**
	 * Set to 1 if the HCD needs to be restarted on a session request
	 * interrupt. This is required if no connector ID status change has
	 * occurred since the HCD was last disconnected.
	 */
	uint8_t restart_hcd_on_session_req;

	/** Device mode Periodic Tx FIFO Mask */
	uint32_t p_tx_msk;
	/** Device mode Periodic Tx FIFO Mask */
	uint32_t tx_msk;

	/** Device mode extended hibernation flag */
	int xhib;

	/** OTG revision supported */

	/** OTG status flag used for HNP polling */
	uint8_t otg_sts;


	/** Start predict NextEP based on Learning Queue if equal 1,
	 * also used as counter of disabled NP IN EP's */
	uint8_t start_predict;

	/** NextEp sequence, including EP0: nextep_seq[] = EP if non-periodic and 
	 * active, 0xff otherwise */
	uint8_t nextep_seq[MAX_EPS_CHANNELS];

	/** Index of fisrt EP in nextep_seq array which should be re-enabled **/
	uint8_t first_in_nextep_seq;

	/** Frame number while entering to ISR - needed for ISOCs **/
	uint32_t frame_num; 

} dwc_otg_core_if_t;


/**
 * Get the pointer to the core_if from the pcd pointer.
 */
#define GET_CORE_IF( _pcd ) (_pcd->core_if)

/**
 * States of EP0.
 */
typedef enum ep0_state {
	EP0_DISCONNECT,		/* no host */
	EP0_IDLE,
	EP0_IN_DATA_PHASE,
	EP0_OUT_DATA_PHASE,
	EP0_IN_STATUS_PHASE,
	EP0_OUT_STATUS_PHASE,
	EP0_STALL,
} ep0state_e;


/** DWC_otg request structure.
 * This structure is a list of requests.
 */
typedef struct dwc_otg_pcd_request {
	void *priv;
	void *buf;
	dwc_dma_t dma;
	uint32_t length;
	uint32_t actual;
	unsigned sent_zlp:1;
	 DWC_CIRCLEQ_ENTRY(dwc_otg_pcd_request) queue_entry;
} dwc_otg_pcd_request_t;

DWC_CIRCLEQ_HEAD(req_list, dwc_otg_pcd_request);


/** Fordward declaration.*/
struct dwc_otg_pcd;

typedef struct dwc_otg_pcd dwc_otg_pcd_t;
/**	  PCD EP structure.
 * This structure describes an EP, there is an array of EPs in the PCD
 * structure.
 */
typedef struct dwc_otg_pcd_ep {
	/** USB EP Descriptor */
	const usb_endpoint_descriptor_t *desc;

	/** queue of dwc_otg_pcd_requests. */
	struct req_list queue;
	unsigned stopped:1;
	unsigned disabling:1;
	unsigned dma:1;
	unsigned queue_sof:1;


	/** DWC_otg ep data. */
	dwc_ep_t dwc_ep;

	/** Pointer to PCD */
	struct dwc_otg_pcd *pcd;

	void *priv;
} dwc_otg_pcd_ep_t;



/**
 * This structure is a wrapper that encapsulates the driver components used to
 * manage a single DWC_otg controller.
 */
typedef struct dwc_otg_device {
	/** Pointer to the core interface structure. */
	dwc_otg_core_if_t *core_if;

	/** Pointer to the PCD structure. */
	struct dwc_otg_pcd *pcd;
} dwc_otg_device_t;


/** DWC_otg PCD Structure.
 * This structure encapsulates the data for the dwc_otg PCD.
 */
struct dwc_otg_pcd {
	/** The DWC otg device pointer */
	struct dwc_otg_device *otg_dev;
	/** Core Interface */
	dwc_otg_core_if_t *core_if;
	/** State of EP0 */
	ep0state_e ep0state;
	/** EP0 Request is pending */
	unsigned ep0_pending:1;
	/** Indicates when SET CONFIGURATION Request is in process */
	unsigned request_config:1;
	/** The state of the Remote Wakeup Enable. */
	unsigned remote_wakeup_enable:1;
	/** The state of the B-Device HNP Enable. */
	unsigned b_hnp_enable:1;
	/** The state of A-Device HNP Support. */
	unsigned a_hnp_support:1;
	/** The state of the A-Device Alt HNP support. */
	unsigned a_alt_hnp_support:1;
	/** Count of pending Requests */
	unsigned request_pending;

	/** SETUP packet for EP0
	 * This structure is allocated as a DMA buffer on PCD initialization
	 * with enough space for up to 3 setup packets.
	 */
	union {
		usb_device_request_t req;
		uint32_t d32[2];
	} *setup_pkt;

	dwc_dma_t setup_pkt_dma_handle;
	/* Additional buffer and flag for CTRL_WR premature case */
	uint8_t *backup_buf;
	unsigned data_terminated;

	/** 2-byte dma buffer used to return status from GET_STATUS */
	uint16_t *status_buf;
	dwc_dma_t status_buf_dma_handle;

	/** EP0 */
	dwc_otg_pcd_ep_t ep0;

	/** Array of IN EPs. */
	dwc_otg_pcd_ep_t in_ep[MAX_EPS_CHANNELS - 1];
	/** Array of OUT EPs. */
	dwc_otg_pcd_ep_t out_ep[MAX_EPS_CHANNELS - 1];
	/** number of valid EPs in the above array. */

	/** The test mode to enter when the tasklet is executed. */
	unsigned test_mode;
	/** The cfi_api structure that implements most of the CFI API
	 * and OTG specific core configuration functionality
	 */

};


struct usb_ctrlrequest {
	unsigned char bRequestType;
	unsigned char bRequest;
	unsigned short  wValue;
	unsigned short  wIndex;
	unsigned short  wLength;
} __attribute__ ((packed));

struct usb_request {
	void			*buf;
	uint32_t		length;


	unsigned		no_interrupt:1;
	unsigned		zero:1;
	unsigned		short_not_ok:1;

	void			(*complete)(struct usb_request *req);
	void			*context;
	int			status;
	uint32_t		actual;
};

extern int ep_queue(struct usb_request *usb_req , int epnum);
extern int dwc_pcd_ep_enable(int dir);

extern void udc_disconnect(void);
extern void udc_start(void);

extern int usb_setup(const struct usb_ctrlrequest *ctrl);
extern int usb_init_ep0(void);
extern int usb_init_ep12(void);

extern void rx_cmd(void);
extern void tx_status(const char *status);

#endif

