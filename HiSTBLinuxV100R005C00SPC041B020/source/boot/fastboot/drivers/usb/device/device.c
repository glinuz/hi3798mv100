#include <malloc.h>
#include "device.h"

struct dwc_otg_pcd * pcd = NULL;
dwc_otg_core_if_t *core_if = NULL;
int usb_out_open = 0;
extern unsigned char usb_config_value;

void udc_disconnect(void)
{
	//TODO:XXX
}

void DWC_MODIFY_REG32(uint32_t reg, uint32_t clear_mask,
				uint32_t set_mask)
{
	writel((readl(reg) & ~clear_mask) | set_mask, reg);
}

static int uplayer_setup(dwc_otg_pcd_t * pcd, uint8_t * bytes)
{
	int retval = -DWC_E_NOT_SUPPORTED;
	retval = usb_setup((struct usb_ctrlrequest*)bytes);
	return retval;
}

static int uplayer_complete(dwc_otg_pcd_t * pcd, void *ep_handle,void *req_handle,
	int32_t status, uint32_t actual)
{
	struct usb_request *req = (struct usb_request *)req_handle;

	if (req && req->complete) {
		switch (status) {
		case -DWC_E_SHUTDOWN:
			req->status = -ESHUTDOWN;
			break;
		case -DWC_E_RESTART:
			req->status = -ECONNRESET;
			break;
		case -DWC_E_INVALID:
			req->status = -EINVAL;
			break;
		case -DWC_E_TIMEOUT:
			req->status = -ETIMEDOUT;
			break;
		default:
			req->status = status;

		}
		req->actual = actual;
		req->complete(req);
	}

	return 0;
}


/**
* Do core a soft reset of the core.  Be careful with this because it
* resets all the internal state machines of the core.
*/
void dwc_core_reset(void)
{
	volatile grstctl_t greset = {.d32 = 0 };
	int count = 0;

	/* Wait for AHB master IDLE state. */
	do {
		udelay(10);
		greset.d32 = readl(REG_GRSTCTL);
		if (++count > 100000) {
			DWC_PRINTF("%s() HANG! AHB Idle GRSTCTL=%0x\n", __func__,
				 greset.d32);
			return;
		}
	} while (greset.b.ahbidle == 0);

	/* Core Soft Reset */
	count = 0;
	greset.b.csftrst = 1;
	writel( greset.d32,REG_GRSTCTL);
	do {
		greset.d32 = readl(REG_GRSTCTL);
		if (++count > 10000) {
			DWC_PRINTF("%s() HANG! Soft Reset GRSTCTL=%0x\n",
				 __func__, greset.d32);
			break;
		}
		udelay(1);
	}
	while (greset.b.csftrst == 1);

	/* Wait for 3 PHY Clocks */
	udelay(100);
}


/**
* Flush a Tx FIFO.
* @param num Tx FIFO to flush.
*/
void dwc_flush_tx_fifo(const int num)
{
	int count = 0;
	volatile grstctl_t greset = {.d32 = 0 };

	greset.b.txfflsh = 1;
	greset.b.txfnum = num;
	writel( greset.d32,REG_GRSTCTL);

	do {
		greset.d32 = readl(REG_GRSTCTL);
		if (++count > 10000) {
			DWC_PRINTF("%s() HANG! GRSTCTL=%0x GNPTXSTS=0x%08x\n",
				 __func__, greset.d32,readl(REG_GNPTXSTS));
			break;
		}
		udelay(1);
	} while (greset.b.txfflsh == 1);

	/* Wait for 3 PHY Clocks */
	udelay(1);
}

/**
* Flush Rx FIFO.
*
*/
void dwc_flush_rx_fifo(void)
{
	int count = 0;
	volatile grstctl_t greset = {.d32 = 0 };

	greset.b.rxfflsh = 1;
	writel(greset.d32,REG_GRSTCTL);

	do {
		greset.d32 = readl(REG_GRSTCTL);
		if (++count > 10000) {
			DWC_PRINTF("%s() HANG! GRSTCTL=%0x\n", __func__,
				 greset.d32);
			break;
		}
		udelay(1);
	} while (greset.b.rxfflsh == 1);

	/* Wait for 3 PHY Clocks */
	udelay(1);
}


/**
* This function writes a packet into the Tx FIFO associated with the
* EP. For non-periodic EPs the non-periodic Tx FIFO is written.
*
* @param ep The EP to write packet for.
* @param dma Indicates if DMA is being used.
*
* The buffer is padded to DWORD on a per packet basis in
* slave/dma mode if the MPS is not DWORD aligned. The last
* packet, if short, is also padded to a multiple of DWORD.
*
* ep->xfer_buff always starts DWORD aligned in memory and is a
* multiple of DWORD in length
*
* ep->xfer_len can be any number of bytes
*
* ep->xfer_count is a multiple of ep->maxpacket until the last
*	packet
*
* FIFO access is DWORD
*/
void dwc_ep_write_packet(dwc_ep_t * ep,int dma)
{
	uint32_t i;
	uint32_t byte_count;
	uint32_t dword_count;
	uint32_t *fifo;
	uint32_t *data_buff = (uint32_t *) ep->xfer_buff;

	if (ep->xfer_count >= ep->xfer_len) {
		DWC_PRINTF("%s() No data for EP%d!!!\n", __func__, ep->num);
		return;
	}

	/* Find the byte length of the packet either short packet or MPS */
	if ((ep->xfer_len - ep->xfer_count) < ep->maxpacket) {
		byte_count = ep->xfer_len - ep->xfer_count;
	} else {
		byte_count = ep->maxpacket;
	}

	/* Find the DWORD length, padded by extra bytes as neccessary if MPS
	* is not a multiple of DWORD */
	dword_count = (byte_count + 3) / 4;

	if(ep->num == 0)
	fifo = (uint32_t *)REG_DATAFIFO0;
	else
	fifo = (uint32_t *)REG_DATAFIFO1;

	if (!dma) {
		for (i = 0; i < dword_count; i++, data_buff++)
		{
			writel(*data_buff,fifo);
		}
	}

	ep->xfer_count += byte_count;
	ep->xfer_buff += byte_count;
	ep->dma_addr += byte_count;
}



/**
* This function reads a packet from the Rx FIFO into the destination
* buffer. To read SETUP data use dwc_otg_read_setup_packet.
*
* @param dest	  Destination buffer for the packet.
* @param bytes  Number of bytes to copy to the destination.
*/
void dwc_read_packet(uint8_t * dest, uint16_t bytes)
{
	int i;
	int word_count = (bytes + 3) / 4;

	volatile uint32_t *fifo = (uint32_t *)REG_DATAFIFO0;
	uint32_t *data_buff = (uint32_t *) dest;

	/**
	* @todo Account for the case where _dest is not dword aligned. This
	* requires reading data from the FIFO into a uint32_t temp buffer,
	* then moving it into the data buffer.
	*/

	for (i = 0; i < word_count; i++, data_buff++) {
		*data_buff = readl(fifo);
	}

	return;
}

/**
* This function reads a setup packet from the Rx FIFO into the destination
* buffer. This function is called from the Rx Status Queue Level (RxStsQLvl)
* Interrupt routine when a SETUP packet has been received in Slave mode.
*
* @param dest Destination buffer for packet data.
*/
void dwc_read_setup_packet(uint32_t * dest)
{
	/* Get the 8 bytes of a setup transaction data */
	/* Pop 2 DWORDS off the receive data FIFO into memory */
	dest[0] = readl(REG_DATAFIFO0);
	dest[1] = readl(REG_DATAFIFO0);
}


static int dwc_get_device_speed(void)
{
	dsts_data_t dsts;
	int speed = 0;
	dsts.d32 = readl(REG_DSTS);

	switch (dsts.b.enumspd) {
	case DWC_DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
		speed = USB_SPEED_HIGH;
		DWC_PRINTF("high speed \n");
		break;
	case DWC_DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
	case DWC_DSTS_ENUMSPD_FS_PHY_48MHZ:
		speed = USB_SPEED_FULL;
		DWC_PRINTF("full speed \n");
		break;
	case DWC_DSTS_ENUMSPD_LS_PHY_6MHZ:
		speed = USB_SPEED_LOW;
		DWC_PRINTF("low speed \n");
		break;
	}

	return speed;
}

/**
* This function enables EP0 OUT to receive SETUP packets and configures EP0
* IN for transmitting packets. It is normally called when the
* "Enumeration Done" interrupt occurs.
*
* @param ep The EP0 data.
*/
void dwc_ep0_activate(dwc_ep_t * ep)
{
	dsts_data_t dsts;
	depctl_data_t diepctl;
	depctl_data_t doepctl;
	dctl_data_t dctl = {.d32 = 0 };

	ep->stp_rollover = 0;
	/* Read the Device Status and Endpoint 0 Control registers */
	dsts.d32 = readl(REG_DSTS);
	diepctl.d32 = readl(REG_DIEPCTL0);
	doepctl.d32 = readl(REG_DOEPCTL0);

	/* Set the MPS of the IN EP based on the enumeration speed */
	switch (dsts.b.enumspd) {
	case DWC_DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
	case DWC_DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
	case DWC_DSTS_ENUMSPD_FS_PHY_48MHZ:
		diepctl.b.mps = DWC_DEP0CTL_MPS_64;
		break;
	case DWC_DSTS_ENUMSPD_LS_PHY_6MHZ:
		diepctl.b.mps = DWC_DEP0CTL_MPS_8;
		break;
	}

	writel(diepctl.d32,REG_DIEPCTL0);

	/* Enable OUT EP for receive */
	if (core_if->snpsid <= CONFIG_USBDEV_REV_2_94a) {
		doepctl.b.epena = 1;
		writel(doepctl.d32,REG_DOEPCTL0);
	}
	
	dctl.b.cgnpinnak = 1;
	DWC_MODIFY_REG32(REG_DCTL, dctl.d32, dctl.d32);
}

/**
* This function activates an EP.  The Device EP control register for
* the EP is configured as defined in the ep structure. Note: This
* function is not used for EP0.
*
* @param core_if Programming view of DWC_otg controller.
* @param ep The EP to activate.
*/
void dwc_ep_activate(dwc_otg_core_if_t * core_if, dwc_ep_t * ep)
{
	depctl_data_t depctl;
	volatile uint32_t *addr;
	daint_data_t daintmsk = {.d32 = 0 };
	dcfg_data_t dcfg;
	uint8_t i;

	DWC_PRINTF("%s() EP%d-%s\n", __func__, ep->num,(ep->is_in ? "IN" : "OUT"));

	/* Read DEPCTLn register */
	if (ep->is_in == 1) {
		addr = (uint32_t *)REG_DIEPCTL1;
		daintmsk.ep.in = 1 << ep->num;
	} else {
		addr = (uint32_t *)REG_DOEPCTL2;
		daintmsk.ep.out = 1 << ep->num;
	}

	/* If the EP is already active don't change the EP Control
	* register. */
	depctl.d32 = readl(addr);
	if (!depctl.b.usbactep) {
		depctl.b.mps = ep->maxpacket;
		depctl.b.eptype = ep->type;
		depctl.b.txfnum = ep->tx_fifo_num;
		depctl.b.setd0pid = 1;
		depctl.b.usbactep = 1;

		/* Update nextep_seq array and EPMSCNT in DCFG */
		if (!(depctl.b.eptype & 1) && (ep->is_in == 1)) {	// NP IN EP
			for (i = 0; i <= 1; i++) {
				if (core_if->nextep_seq[i] == core_if->first_in_nextep_seq)
					break;
			}
			core_if->nextep_seq[i] = ep->num;
			core_if->nextep_seq[ep->num] = core_if->first_in_nextep_seq;
			depctl.b.nextep = core_if->nextep_seq[ep->num];
			dcfg.d32 = readl(REG_DCFG);
			dcfg.b.epmscnt++;
			writel(dcfg.d32,REG_DCFG);

			DWC_PRINTF("%s first_in_nextep_seq= %2d; nextep_seq[]:\n",
				    __func__, core_if->first_in_nextep_seq);
			for (i = 0; i <= 1; i++) {
				DWC_PRINTF("%2d\n",core_if->nextep_seq[i]);
			}
		}
		writel(depctl.d32,addr);
		DWC_PRINTF( "DEPCTL=%08x\n", readl(addr));
	}

	DWC_MODIFY_REG32(REG_DAINTMSK, 0, daintmsk.d32);
	DWC_PRINTF( "DAINTMSK=%0x\n",readl(REG_DAINTMSK));
	ep->stall_clear_flag = 0;

	return;
}


/**
* This function assigns periodic Tx FIFO to an periodic EP
* in shared Tx FIFO mode
*/
static uint32_t dwc_assign_tx_fifo(dwc_otg_core_if_t * core_if)
{
	uint32_t TxMsk = 1;
	int i;

	for (i = 0; i < core_if->hwcfg4.b.num_in_eps; ++i) {
		if ((TxMsk & core_if->tx_msk) == 0) {
			core_if->tx_msk |= TxMsk;
			return i + 1;
		}
		TxMsk <<= 1;
	}
	return 0;
}



/**
* This function is being called from gadget
* to enable PCD endpoint.
*/
int dwc_pcd_ep_enable(int dir)
{
	dwc_otg_pcd_ep_t *ep = NULL;
	fifosize_data_t dptxfsiz = {.d32 = 0 };
	gdfifocfg_data_t gdfifocfg = {.d32 = 0 };
	gdfifocfg_data_t gdfifocfgbase = {.d32 = 0 };
	int retval = 0;

	if (dir == UE_DIR_IN) {
		ep = &pcd->in_ep[0];
	} else {
		ep = &pcd->out_ep[0];
	}


	/*
	* Activate the EP
	*/
	ep->stopped = 0;
	ep->dwc_ep.is_in = (dir == UE_DIR_IN);
	ep->dwc_ep.maxpacket = 512;
	ep->dwc_ep.type = UE_BULK & UE_XFERTYPE;

	if (ep->dwc_ep.is_in) {

		if (!GET_CORE_IF(pcd)->en_multiple_tx_fifo) {
			ep->dwc_ep.tx_fifo_num = 0;

		} else {
			/*
			* if Dedicated FIFOs mode is on then assign a Tx FIFO.
			*/
			ep->dwc_ep.tx_fifo_num =dwc_assign_tx_fifo(GET_CORE_IF(pcd));
		}

		/* Calculating EP info controller base address */
		if ((ep->dwc_ep.tx_fifo_num)&& GET_CORE_IF(pcd)->en_multiple_tx_fifo
			&&(GET_CORE_IF(pcd)->snpsid <= CONFIG_USBDEV_REV_2_94a) ){
			gdfifocfg.d32 = readl(REG_GDFIFOCFG);
			gdfifocfgbase.d32 = gdfifocfg.d32 >> 16;
			dptxfsiz.d32 =(readl(REG_DPTXFSIZN) >> 16);
			gdfifocfg.b.epinfobase = gdfifocfgbase.d32 + dptxfsiz.d32;
			writel(gdfifocfg.d32,REG_GDFIFOCFG);
		}
	}

	/* Set initial data PID. */
	if (ep->dwc_ep.type == UE_BULK) {
		ep->dwc_ep.data_pid_start = 0;
	}

	dwc_ep_activate(GET_CORE_IF(pcd), &ep->dwc_ep);

	return retval;
}

/*
* This function completes a request.  It call's the request call back.
*/
void dwc_request_done(dwc_otg_pcd_ep_t * ep, dwc_otg_pcd_request_t * req,
			  int32_t status)
{
	unsigned stopped = ep->stopped;

	DWC_PRINTF("%s(%p)\n", __func__, ep);
	DWC_CIRCLEQ_REMOVE_INIT(&ep->queue, req, queue_entry);

	/* don't modify queue heads during completion callback */
	ep->stopped = 1;
	/* spin_unlock/spin_lock now done in fops->complete() */
	uplayer_complete(ep->pcd, ep->priv, req->priv, status,
		req->actual);

	if (ep->pcd->request_pending > 0) {
		--ep->pcd->request_pending;
	}

	ep->stopped = stopped;
	free(req);
}

/**
* This function terminates all the requsts in the EP request queue.
*/
void dwc_request_nuke(dwc_otg_pcd_ep_t * ep)
{
	dwc_otg_pcd_request_t *req;

	ep->stopped = 1;

	/* called with irqs blocked?? */
	while (!DWC_CIRCLEQ_EMPTY(&ep->queue))
	{
		req = DWC_CIRCLEQ_FIRST(&ep->queue);
		dwc_request_done(ep, req, -DWC_E_SHUTDOWN);
	}
}


/**
* This function does the setup for a data transfer for an EP and
* starts the transfer. For an IN transfer, the packets will be
* loaded into the appropriate Tx FIFO in the ISR. For OUT transfers,
* the packets are unloaded from the Rx FIFO in the ISR.  the ISR.
*
* @param core_if Programming view of DWC_otg controller.
* @param ep The EP to start the transfer on.
*/
void dwc_otg_ep_start_transfer(dwc_otg_core_if_t * core_if, dwc_ep_t * ep)
{
	depctl_data_t depctl;
	deptsiz_data_t deptsiz;
	gintmsk_data_t intr_mask = {.d32 = 0 };

	/* IN endpoint */
	if (ep->is_in == 1)
	{
		gnptxsts_data_t gtxstatus;
		gtxstatus.d32 = readl(REG_GNPTXSTS);

		if (core_if->en_multiple_tx_fifo == 0
		    && gtxstatus.b.nptxqspcavail == 0 && !core_if->dma_enable) {
			DWC_PRINTF("TX Queue Full (0x%0x)\n", gtxstatus.d32);
			return;
		}
		depctl.d32 = readl(REG_DIEPCTL1);
		deptsiz.d32 = readl(REG_DIEPTSIZ1);

		if (ep->maxpacket > ep->maxxfer / MAX_PKT_CNT)
			ep->xfer_len += (ep->maxxfer < (ep->total_len - ep->xfer_len)) ?
			ep->maxxfer : (ep->total_len - ep->xfer_len);
		else
			ep->xfer_len += (MAX_PKT_CNT * ep->maxpacket < (ep->total_len - ep->xfer_len)) ?
			 MAX_PKT_CNT * ep->maxpacket : (ep->total_len - ep->xfer_len);

		/* Zero Length Packet? */
		if ((ep->xfer_len - ep->xfer_count) == 0) {
			deptsiz.b.xfersize = 0;
			deptsiz.b.pktcnt = 1;
		} else {
			/* Program the transfer size and packet count
			*as follows: xfersize = N * maxpacket +
			*short_packet pktcnt = N + (short_packet
			*exist ? 1 : 0)
			*/
			deptsiz.b.xfersize = ep->xfer_len - ep->xfer_count;
			deptsiz.b.pktcnt =(ep->xfer_len - ep->xfer_count - 1 +
			ep->maxpacket) / ep->maxpacket;

			/* CodeCC: deptsiz.b.pktcnt > 1023" is always false regardless of the values of its operands. */
#if 0
			if (deptsiz.b.pktcnt > MAX_PKT_CNT) {
				deptsiz.b.pktcnt = MAX_PKT_CNT;
				deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
			}
#endif
		}

		/* Write the DMA register */
		if (core_if->dma_enable) {
			deptsiz.b.mc = 1;
			writel(deptsiz.d32,REG_DIEPTSIZ1);
			writel((uint32_t) ep->dma_addr,REG_DIEPDMA1);
		} else {
			writel(deptsiz.d32,REG_DIEPTSIZ1);
			/**
			* Enable the Non-Periodic Tx FIFO empty interrupt,
			* or the Tx FIFO epmty interrupt in dedicated Tx FIFO mode,
			* the data will be written into the fifo by the ISR.
			* Enable the Tx FIFO Empty Interrupt for this EP */
			if (core_if->en_multiple_tx_fifo == 0) {
					intr_mask.b.nptxfempty = 1;
					DWC_MODIFY_REG32
					    (REG_GINTMSK,
					     intr_mask.d32, intr_mask.d32);
			} else {
			if (ep->xfer_len > 0) {
				uint32_t fifoemptymsk = 0;
				fifoemptymsk = 1 << ep->num;
				DWC_MODIFY_REG32(REG_DTKNQR4,
				0, fifoemptymsk);
				}
			}
		}
		if (!core_if->en_multiple_tx_fifo && core_if->dma_enable)
			depctl.b.nextep = core_if->nextep_seq[1];
		/* EP enable, IN data in FIFO */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DIEPCTL1);
	}
	else
	{
		/* OUT endpoint */

		depctl.d32 = readl(REG_DOEPCTL2);
		deptsiz.d32 = readl(REG_DOEPTSIZ2);

		if (ep->maxpacket > ep->maxxfer / MAX_PKT_CNT)
			ep->xfer_len += (ep->maxxfer < (ep->total_len - ep->xfer_len)) ?
				ep->maxxfer : (ep->total_len - ep->xfer_len);
		else
			ep->xfer_len += (MAX_PKT_CNT * ep->maxpacket < (ep->total_len
			- ep->xfer_len)) ? MAX_PKT_CNT * ep->maxpacket : (ep->total_len - ep->xfer_len);

		/* Program the transfer size and packet count as follows:
		*
		*pktcnt = N
		*xfersize = N * maxpacket
		*/
		if ((ep->xfer_len - ep->xfer_count) == 0) {
			/* Zero Length Packet */
			deptsiz.b.xfersize = ep->maxpacket;
			deptsiz.b.pktcnt = 1;
		} else {
			deptsiz.b.pktcnt =(ep->xfer_len - ep->xfer_count +
			(ep->maxpacket - 1)) / ep->maxpacket;
			ep->xfer_len =deptsiz.b.pktcnt * ep->maxpacket + ep->xfer_count;
			deptsiz.b.xfersize = ep->xfer_len - ep->xfer_count;
		}

		if (core_if->dma_enable) {
			writel(deptsiz.d32,REG_DOEPTSIZ2);
			writel((uint32_t) ep->dma_addr,REG_DOEPDMA2);
		} else {
			writel(deptsiz.d32,REG_DOEPTSIZ2);
		}

		/* EP enable */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;

		DWC_PRINTF("ep%d xfersize=%d pktcnt=%d\n",
			ep->num, deptsiz.b.xfersize, deptsiz.b.pktcnt);

		writel(depctl.d32,REG_DOEPCTL2);
	}
}

/**
* This function setup a zero length transfer in Buffer DMA and
* Slave modes for usb requests with zero field set
*
* @param core_if Programming view of DWC_otg controller.
* @param ep The EP to start the transfer on.
*
*/
void dwc_otg_ep_start_zl_transfer(dwc_otg_core_if_t * core_if, dwc_ep_t * ep)
{

	depctl_data_t depctl;
	deptsiz_data_t deptsiz;
	gintmsk_data_t intr_mask = {.d32 = 0 };

	DWC_PRINTF( "%s()\n", __func__);

	/* IN endpoint */
	if (ep->is_in == 1) {

		depctl.d32 = readl(REG_DIEPCTL1);
		deptsiz.d32 = readl(REG_DIEPTSIZ1);

		deptsiz.b.xfersize = 0;
		deptsiz.b.pktcnt = 1;

		/* Write the DMA register */
		if (core_if->dma_enable) {
			deptsiz.b.mc = 1;
			writel(deptsiz.d32,REG_DIEPTSIZ1);
			writel((uint32_t) ep->dma_addr,REG_DIEPDMA1);
		} else {
			writel(deptsiz.d32,REG_DIEPTSIZ1);
			/**
			* Enable the Non-Periodic Tx FIFO empty interrupt,
			* or the Tx FIFO epmty interrupt in dedicated Tx FIFO mode,
			* the data will be written into the fifo by the ISR.
			* Enable the Tx FIFO Empty Interrupt for this EP */
			if (core_if->en_multiple_tx_fifo == 0) {
				intr_mask.b.nptxfempty = 1;
				DWC_MODIFY_REG32(REG_GINTMSK,
						 intr_mask.d32, intr_mask.d32);
			} else {
			if (ep->xfer_len > 0) {
				uint32_t fifoemptymsk = 0;
				fifoemptymsk = 1 << ep->num;
				DWC_MODIFY_REG32(REG_DTKNQR4,
					0, fifoemptymsk);
				}
			}
		}

		if (!core_if->en_multiple_tx_fifo && core_if->dma_enable)
			depctl.b.nextep = core_if->nextep_seq[1];
		/* EP enable, IN data in FIFO */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DIEPCTL1);
	} else {
		/* OUT endpoint */
		depctl.d32 = readl(REG_DOEPCTL2);
		deptsiz.d32 = readl(REG_DOEPTSIZ2);

		/* Zero Length Packet */
		deptsiz.b.xfersize = ep->maxpacket;
		deptsiz.b.pktcnt = 1;

		if (core_if->dma_enable) {
			writel(deptsiz.d32,REG_DOEPTSIZ2);
			writel((uint32_t) ep->dma_addr,REG_DOEPDMA2);
		} else {
			writel(deptsiz.d32,REG_DOEPTSIZ2);
		}

		/* EP enable */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DOEPCTL2);
	}
}

/**
* This function does the setup for a data transfer for EP0 and starts
* the transfer.  For an IN transfer, the packets will be loaded into
* the appropriate Tx FIFO in the ISR. For OUT transfers, the packets are
* unloaded from the Rx FIFO in the ISR.
*
* @param core_if Programming view of DWC_otg controller.
* @param ep The EP0 data.
*/
void dwc_otg_ep0_start_transfer(dwc_otg_core_if_t * core_if, dwc_ep_t * ep)
{
	depctl_data_t depctl;
	deptsiz0_data_t deptsiz;
	gnptxsts_data_t gtxstatus;
	gintmsk_data_t intr_mask = {.d32 = 0 };

	ep->total_len = ep->xfer_len;

	/* IN endpoint */
	if (ep->is_in == 1){

		if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a) {
			depctl.d32 = readl(REG_DIEPCTL0);
			if (depctl.b.epena)
				return;
		}

		gtxstatus.d32 =readl(REG_GNPTXSTS);

		/* If dedicated FIFO every time flush fifo before enable ep*/
		if (core_if->en_multiple_tx_fifo &&core_if->snpsid >= CONFIG_USBDEV_REV_3_00a)
		dwc_flush_tx_fifo(ep->tx_fifo_num);

		if (core_if->en_multiple_tx_fifo == 0&& gtxstatus.b.nptxqspcavail == 0
		    && !core_if->dma_enable) {

			deptsiz.d32 = readl(REG_DIEPTSIZ0);
			DWC_PRINTF("DIEPCTL0=%0x\n",readl(REG_DIEPCTL0));
			DWC_PRINTF("DIEPTSIZ0=%0x (sz=%d, pcnt=%d)\n",
				    deptsiz.d32,
				    deptsiz.b.xfersize, deptsiz.b.pktcnt);
			DWC_PRINTF("TX Queue or FIFO Full (0x%0x)\n",
				   gtxstatus.d32);

			return;
		}
		depctl.d32 = readl(REG_DIEPCTL0);
		deptsiz.d32 = readl(REG_DIEPTSIZ0);

		/* Zero Length Packet? */
		if (ep->xfer_len == 0)
		{
			deptsiz.b.xfersize = 0;
			deptsiz.b.pktcnt = 1;
		}
		else
		{
			/* Program the transfer size and packet count
			*      as follows: xfersize = N * maxpacket +
			*      short_packet pktcnt = N + (short_packet
			*      exist ? 1 : 0)
			*/
			if (ep->xfer_len > ep->maxpacket)
			{
				ep->xfer_len = ep->maxpacket;
				deptsiz.b.xfersize = ep->maxpacket;
			}
			else
			{
				deptsiz.b.xfersize = ep->xfer_len;
			}
			deptsiz.b.pktcnt = 1;
		}

		/* Write the DMA register */
		if (core_if->dma_enable) {
			writel(deptsiz.d32,REG_DIEPTSIZ0);
			writel((uint32_t) ep->dma_addr,REG_DIEPDMA0);
		} else {
			writel(deptsiz.d32,REG_DIEPTSIZ0);
		}

		if (!core_if->en_multiple_tx_fifo && core_if->dma_enable)
			depctl.b.nextep = core_if->nextep_seq[0];
		/* EP enable, IN data in FIFO */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DIEPCTL0);

		/**
		* Enable the Non-Periodic Tx FIFO empty interrupt, the
		* data will be written into the fifo by the ISR.
		*/
		if (!core_if->dma_enable) {
			if (core_if->en_multiple_tx_fifo == 0) {
				intr_mask.b.nptxfempty = 1;
				DWC_MODIFY_REG32(REG_GINTMSK,
						 intr_mask.d32, intr_mask.d32);
			} else {
				/* Enable the Tx FIFO Empty Interrupt for this EP */
				if (ep->xfer_len > 0){
				uint32_t fifoemptymsk = 0;
				fifoemptymsk |= 1 << ep->num;
				DWC_MODIFY_REG32(REG_DTKNQR4,0, fifoemptymsk);
				}
			}
		}
	}else{
		/* OUT endpoint */

		depctl.d32 = readl(REG_DOEPCTL0);
		deptsiz.d32 = readl(REG_DOEPTSIZ0);

		/* Program the transfer size and packet count as follows:
		*xfersize = N * (maxpacket + 4 - (maxpacket % 4))
		*pktcnt = N
		* Zero Length Packet */
		deptsiz.b.xfersize = ep->maxpacket;
		deptsiz.b.pktcnt = 1;
		if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a)
		deptsiz.b.supcnt = 3;

		if (core_if->dma_enable) {
			writel(deptsiz.d32,REG_DOEPTSIZ0);
			writel((uint32_t) ep->dma_addr,REG_DOEPDMA0);

		} else {
			writel(deptsiz.d32,REG_DOEPTSIZ0);
		}

		/* EP enable */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel( depctl.d32,REG_DOEPCTL0);
	}
}

/**
* This function continues control IN transfers started by
* dwc_otg_ep0_start_transfer, when the transfer does not fit in a
* single packet.  NOTE: The DIEPCTL0/DOEPCTL0 registers only have one
* bit for the packet count.
*
* @param core_if Programming view of DWC_otg controller.
* @param ep The EP0 data.
*/
void dwc_otg_ep0_continue_transfer(dwc_otg_core_if_t * core_if, dwc_ep_t * ep)
{
	depctl_data_t depctl;
	deptsiz0_data_t deptsiz;
	gintmsk_data_t intr_mask = {.d32 = 0 };

	if (ep->is_in == 1)
	{

		/** @todo Should there be check for room in the Tx
		* Status Queue.  If not remove the code above this comment. */

		depctl.d32 = readl(REG_DIEPCTL0);
		deptsiz.d32 = readl(REG_DIEPTSIZ0);

		/* Program the transfer size and packet count
		*      as follows: xfersize = N * maxpacket +
		*      short_packet pktcnt = N + (short_packet
		*      exist ? 1 : 0)
		*/
		deptsiz.b.xfersize =(ep->total_len - ep->xfer_count) >
			ep->maxpacket ? ep->maxpacket : (ep->total_len -ep->xfer_count);
		deptsiz.b.pktcnt = 1;
		if (core_if->dma_enable == 0) {
			ep->xfer_len += deptsiz.b.xfersize;
		} else {
			ep->xfer_len = deptsiz.b.xfersize;
		}
		writel(deptsiz.d32,REG_DIEPTSIZ0);

		/* Write the DMA register */
		if (core_if->dma_enable == 1) {
			writel((uint32_t) ep->dma_addr,REG_DIEPDMA0);
		}

		if (!core_if->en_multiple_tx_fifo && core_if->dma_enable)
			depctl.b.nextep = core_if->nextep_seq[0];
		/* EP enable, IN data in FIFO */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DIEPCTL0);

		/**
		* Enable the Non-Periodic Tx FIFO empty interrupt, the
		* data will be written into the fifo by the ISR.
		*/
		if (!core_if->dma_enable) {
			if (core_if->en_multiple_tx_fifo == 0) {
				/* First clear it from GINTSTS */
				intr_mask.b.nptxfempty = 1;
				DWC_MODIFY_REG32(REG_GINTMSK,
						 intr_mask.d32, intr_mask.d32);

			} else {
			/* Enable the Tx FIFO Empty Interrupt for this EP */
			if (ep->xfer_len > 0) {
				uint32_t fifoemptymsk = 0;
				fifoemptymsk |= 1 << ep->num;
				DWC_MODIFY_REG32(REG_DTKNQR4,
					0, fifoemptymsk);
				}
			}
		}
	} else {

		depctl.d32 = readl(REG_DOEPCTL0);
		deptsiz.d32 = readl(REG_DOEPTSIZ0);
		/* Program the transfer size and packet count
		*      as follows: xfersize = N * maxpacket +
		*      short_packet pktcnt = N + (short_packet
		*      exist ? 1 : 0)
		*/
		deptsiz.b.xfersize = ep->maxpacket;
		deptsiz.b.pktcnt = 1;

		writel(deptsiz.d32,REG_DOEPTSIZ0);

		/* Write the DMA register */
		if (core_if->dma_enable == 1) {
			writel((uint32_t) ep->dma_addr,REG_DOEPDMA0);
		}
		/* EP enable, IN data in FIFO */
		depctl.b.cnak = 1;
		depctl.b.epena = 1;
		writel(depctl.d32,REG_DOEPCTL0);
	}
}


/**
* This function checks the EP request queue, if the queue is not
* empty the next request is started.
*/
void start_next_request(dwc_otg_pcd_ep_t * ep)
{
	dwc_otg_pcd_request_t *req = 0;
	uint32_t max_transfer =65535;

	if (!DWC_CIRCLEQ_EMPTY(&ep->queue)) {
		req = DWC_CIRCLEQ_FIRST(&ep->queue);

		ep->dwc_ep.dma_addr = req->dma;
		ep->dwc_ep.start_xfer_buff = req->buf;
		ep->dwc_ep.xfer_buff = req->buf;
		ep->dwc_ep.sent_zlp = 0;
		ep->dwc_ep.total_len = req->length;
		ep->dwc_ep.xfer_len = 0;
		ep->dwc_ep.xfer_count = 0;

		ep->dwc_ep.maxxfer = max_transfer;

		if (ep->dwc_ep.maxxfer < ep->dwc_ep.total_len) {
			ep->dwc_ep.maxxfer -=
			(ep->dwc_ep.maxxfer % ep->dwc_ep.maxpacket);
		}

		if (req->sent_zlp) {
			if ((ep->dwc_ep.total_len %ep->dwc_ep.maxpacket == 0)
				&& (ep->dwc_ep.total_len != 0)) {
				ep->dwc_ep.sent_zlp = 1;
			}

		}
		dwc_otg_ep_start_transfer(GET_CORE_IF(ep->pcd), &ep->dwc_ep);
	}
}

static int dwc_pcd_ep_queue(dwc_otg_pcd_t * pcd, dwc_otg_pcd_ep_t *ep,
			 uint8_t * buf,  uint32_t buflen,int zero, void *req_handle)
{
	dwc_otg_pcd_request_t *req;
	uint32_t max_transfer;

	req = malloc(sizeof(*req));
	if (!req) {
		return -DWC_E_NO_MEMORY;
	}

	DWC_CIRCLEQ_INIT_ENTRY(req, queue_entry);

	req->buf = buf;
	req->dma = (dwc_dma_t)buf;
	req->length = buflen;
	req->sent_zlp = zero;
	req->priv = req_handle;

	/* Start the transfer */
	if (DWC_CIRCLEQ_EMPTY(&ep->queue) && !ep->stopped) {
		/* EP0 Transfer? */
		if (ep->dwc_ep.num == 0) {
			switch (pcd->ep0state) {
			case EP0_IN_DATA_PHASE:
				DWC_PRINTF(
					    "%s ep0: EP0_IN_DATA_PHASE\n",
					    __func__);
				break;

			case EP0_OUT_DATA_PHASE:
				DWC_PRINTF(
					    "%s ep0: EP0_OUT_DATA_PHASE\n",
					    __func__);
				if (pcd->request_config) {
					/* Complete STATUS PHASE */
					ep->dwc_ep.is_in = 1;
					pcd->ep0state = EP0_IN_STATUS_PHASE;
				}
				break;

			case EP0_IN_STATUS_PHASE:
				DWC_PRINTF(
					    "%s ep0: EP0_IN_STATUS_PHASE\n",
					    __func__);
				break;

			default:
				DWC_PRINTF( "ep0: odd state %d\n",
					    pcd->ep0state);
				free(req);
				return -DWC_E_SHUTDOWN;
			}

			ep->dwc_ep.dma_addr =(dwc_dma_t)buf;
			ep->dwc_ep.start_xfer_buff = buf;
			ep->dwc_ep.xfer_buff = buf;
			ep->dwc_ep.xfer_len = buflen;
			ep->dwc_ep.xfer_count = 0;
			ep->dwc_ep.sent_zlp = 0;
			ep->dwc_ep.total_len = ep->dwc_ep.xfer_len;

			if (zero) {
				if ((ep->dwc_ep.xfer_len %
				ep->dwc_ep.maxpacket == 0)
				&& (ep->dwc_ep.xfer_len != 0)) {
					ep->dwc_ep.sent_zlp = 1;
				}
			}

			dwc_otg_ep0_start_transfer(GET_CORE_IF(pcd),&ep->dwc_ep);
		}		// non-ep0 endpoints
		else {
			max_transfer = 65535;

			/* Setup and start the Transfer */
			ep->dwc_ep.dma_addr = (dwc_dma_t)buf;
			ep->dwc_ep.start_xfer_buff = buf;
			ep->dwc_ep.xfer_buff = buf;
			ep->dwc_ep.xfer_len = 0;
			ep->dwc_ep.xfer_count = 0;
			ep->dwc_ep.sent_zlp = 0;
			ep->dwc_ep.total_len = buflen;
			ep->dwc_ep.maxxfer = max_transfer;

			if (ep->dwc_ep.maxxfer < ep->dwc_ep.total_len) {
				ep->dwc_ep.maxxfer -=(ep->dwc_ep.maxxfer %
						ep->dwc_ep.maxpacket);
			}

			if (zero) {
				if ((ep->dwc_ep.total_len %ep->dwc_ep.maxpacket == 0)
					&& (ep->dwc_ep.total_len != 0)) {
					ep->dwc_ep.sent_zlp = 1;
				}
			}
			dwc_otg_ep_start_transfer(GET_CORE_IF(pcd),&ep->dwc_ep);
		}
	}

	if (req != 0) {
		++pcd->request_pending;
		DWC_CIRCLEQ_INSERT_TAIL(&ep->queue, req, queue_entry);
		if (ep->dwc_ep.is_in && ep->stopped&& !(GET_CORE_IF(pcd)->dma_enable)) {
			/** @todo NGS Create a function for this. */
			diepmsk_data_t diepmsk = {.d32 = 0 };
			diepmsk.b.intktxfemp = 1;
			DWC_MODIFY_REG32(REG_DIEMSK, 0, diepmsk.d32);
		}
	}

	return 0;
}



/**
* This function is used to submit an I/O Request to an EP.
*
*	- When the request completes the request's completion callback
*	  is called to return the request to the driver.
*	- An EP, except control EPs, may have multiple requests
*	  pending.
*	- Once submitted the request cannot be examined or modified.
*	- Each request is turned into one or more packets.
*	- A BULK EP can queue any amount of data; the transfer is
*	  packetized.
*	- Zero length Packets are specified with the request 'zero'
*	  flag.
*/
int ep_queue(struct usb_request *usb_req , int epnum)
{
	int retval = 0;
	dwc_otg_pcd_ep_t *ep =NULL;
	if (!usb_req || !usb_req->complete || !usb_req->buf) {
		DWC_PRINTF("bad params\n");
		return -EINVAL;
	}

	if(epnum ==EP0){
		ep = &pcd->ep0;
	}
	else if(epnum == EPIN){
		ep = &pcd->in_ep[0];
	}
	else if(epnum == EPOUT){
		ep = &pcd->out_ep[0];
	}
	else{
		DWC_PRINTF("bad epnum\n");
		return -EINVAL;
	}

	usb_req->status = -EINPROGRESS;
	usb_req->actual = 0;

	retval = dwc_pcd_ep_queue(pcd, ep,usb_req->buf,
		usb_req->length, usb_req->zero, usb_req);
	if (retval) {
		return -EINVAL;
	}

	return 0;
}

/**
* This function is called when dedicated Tx FIFO Empty interrupt occurs.
* The active request is checked for the next packet to be loaded into
* apropriate Tx FIFO.
*/
static int32_t write_empty_tx_fifo(dwc_otg_pcd_t * pcd, uint32_t epnum)
{
	dtxfsts_data_t txstatus = {.d32 = 0 };
	dwc_otg_pcd_ep_t *ep = 0;
	uint32_t len = 0;
	int dwords;

	if (epnum == 0) {
		ep= &pcd->ep0;
	} else if (epnum == 1) {
		ep=&pcd->in_ep[0];
	} else {
		DWC_PRINTF(  "no this epnum\n");
		return -1;
	}

	len = ep->dwc_ep.xfer_len - ep->dwc_ep.xfer_count;

	if (len > ep->dwc_ep.maxpacket) {
		len = ep->dwc_ep.maxpacket;
	}

	dwords = (len + 3) / 4;

	/* While there is space in the queue and space in the FIFO and
	 * More data to tranfer, Write packets to the Tx FIFO */
	txstatus.d32 = readl(REG_DTXFSTS0+epnum*0x20);

	while (txstatus.b.txfspcavail > dwords &&ep->dwc_ep.xfer_count < ep->dwc_ep.xfer_len &&
		ep->dwc_ep.xfer_len != 0) {
		/* Write the FIFO */
		dwc_ep_write_packet(&ep->dwc_ep, 0);

		len = ep->dwc_ep.xfer_len - ep->dwc_ep.xfer_count;
		if (len > ep->dwc_ep.maxpacket) {
			len = ep->dwc_ep.maxpacket;
		}

		dwords = (len + 3) / 4;
		txstatus.d32 =readl(REG_DTXFSTS0+epnum*0x20);
	}

	return 1;
}



/**
* This function configures EPO to receive SETUP packets.
*
* @todo NGS: Update the comments from the HW FS.
*
*	-# Program the following fields in the endpoint specific registers
*	for Control OUT EP 0, in order to receive a setup packet
*	- DOEPTSIZ0.Packet Count = 3 (To receive up to 3 back to back
*	  setup packets)
*	- DOEPTSIZE0.Transfer Size = 24 Bytes (To receive up to 3 back
*	  to back setup packets)
*		- In DMA mode, DOEPDMA0 Register with a memory address to
*		  store any setup packets received
*
* @param core_if Programming view of DWC_otg controller.
* @param pcd	  Programming view of the PCD.
*/
static inline void ep0_out_start(dwc_otg_core_if_t * core_if,
				dwc_otg_pcd_t * pcd)
{
	deptsiz0_data_t doeptsize0 = {.d32 = 0 };
	depctl_data_t doepctl = {.d32 = 0 };

	if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a) {
		doepctl.d32 = readl(REG_DOEPCTL0);
		if (doepctl.b.epena) {
			return;
		}
	}

	doeptsize0.b.supcnt = 3;
	doeptsize0.b.pktcnt = 1;
	doeptsize0.b.xfersize = 8 * 3;

	if (core_if->dma_enable) {

		/** put here as for Hermes mode deptisz register should not be written */
		writel(doeptsize0.d32,REG_DOEPTSIZ0);
		/** @todo dma needs to handle multiple setup packets (up to 3) */
		writel(pcd->setup_pkt_dma_handle,REG_DOEPDMA0);
	} else {
		/** put here as for Hermes mode deptisz register should not be written */
		writel(doeptsize0.d32,REG_DOEPTSIZ0);
	}

	/** DOEPCTL0 Register write cnak will be set after setup interrupt */
	doepctl.d32 = 0;
	doepctl.b.epena = 1;
	if (core_if->snpsid <= CONFIG_USBDEV_REV_2_94a) {
		doepctl.b.cnak = 1;
		writel(doepctl.d32,REG_DOEPCTL0);
	} else {
		DWC_MODIFY_REG32(REG_DOEPCTL0, 0, doepctl.d32);
	}
}



/**
* This function starts the Zero-Length Packet for the IN status phase of a 2 stage control transfer.
*/
static inline void do_setup_in_status_phase(dwc_otg_pcd_t * pcd)
{
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;
	if (pcd->ep0state == EP0_STALL) {
		return;
	}

	pcd->ep0state = EP0_IN_STATUS_PHASE;
	ep0->dwc_ep.xfer_len = 0;
	ep0->dwc_ep.xfer_count = 0;
	ep0->dwc_ep.is_in = 1;
	ep0->dwc_ep.dma_addr = pcd->setup_pkt_dma_handle;
	dwc_otg_ep0_start_transfer(GET_CORE_IF(pcd), &ep0->dwc_ep);
}

/**
* This function starts the Zero-Length Packet for the OUT status phase
* of a 2 stage control transfer.
*/
static inline void do_setup_out_status_phase(dwc_otg_pcd_t * pcd)
{
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;
	if (pcd->ep0state == EP0_STALL) {
		DWC_PRINTF(  "EP0 STALLED\n");
		return;
	}
	pcd->ep0state = EP0_OUT_STATUS_PHASE;

	DWC_PRINTF(  "EP0 OUT ZLP\n");
	ep0->dwc_ep.xfer_len = 0;
	ep0->dwc_ep.xfer_count = 0;
	ep0->dwc_ep.is_in = 0;
	ep0->dwc_ep.dma_addr = pcd->setup_pkt_dma_handle;
	dwc_otg_ep0_start_transfer(GET_CORE_IF(pcd), &ep0->dwc_ep);

	/* Prepare for more SETUP Packets */
	if (GET_CORE_IF(pcd)->dma_enable == 0) {
		ep0_out_start(GET_CORE_IF(pcd), pcd);
	}
}


/**
 * This function process the GET_STATUS Setup Commands.
 */
static inline void do_get_status(dwc_otg_pcd_t * pcd)
{
	usb_device_request_t ctrl = pcd->setup_pkt->req;
	dwc_otg_pcd_ep_t *ep;
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;
	uint16_t *status = pcd->status_buf;
	uint32_t ep_num;

	switch (UT_GET_RECIPIENT(ctrl.bmRequestType)) {
	case UT_DEVICE:
		*status = 0x1;	/* Self powered */
		*status |= pcd->remote_wakeup_enable << 1;
		break;

	case UT_INTERFACE:
		*status = 0;
		break;

	case UT_ENDPOINT:
		ep_num = UE_GET_ADDR(UGETW(ctrl.wIndex));

		if (ep_num == 0) {
			ep = &pcd->ep0;
		} else if (UE_GET_DIR(UGETW(ctrl.wIndex)) == UE_DIR_IN) {
			ep = &pcd->in_ep[0];
		} else {
			ep = &pcd->out_ep[0];
		}
		if (ep == 0 || UGETW(ctrl.wLength) > 2) {
			return;
		}
		/** @todo check for EP stall */
		*status = ep->stopped;
		break;
	}
	pcd->ep0_pending = 1;
	ep0->dwc_ep.start_xfer_buff = (uint8_t *) status;
	ep0->dwc_ep.xfer_buff = (uint8_t *) status;
	ep0->dwc_ep.dma_addr = pcd->status_buf_dma_handle;
	ep0->dwc_ep.xfer_len = 2;
	ep0->dwc_ep.xfer_count = 0;
	ep0->dwc_ep.total_len = ep0->dwc_ep.xfer_len;
	dwc_otg_ep0_start_transfer(GET_CORE_IF(pcd), &ep0->dwc_ep);
}



static inline void pcd_setup(dwc_otg_pcd_t * pcd)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
	usb_device_request_t ctrl = pcd->setup_pkt->req;
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;

	deptsiz0_data_t doeptsize0 = {.d32 = 0 };


	DWC_PRINTF("SETUP %02x.%02x v%04x i%04x l%04x\n",
		    ctrl.bmRequestType, ctrl.bRequest,
		    UGETW(ctrl.wValue), UGETW(ctrl.wIndex),
		    UGETW(ctrl.wLength));
	doeptsize0.d32 = readl(REG_DOEPTSIZ0);

	/** In BDMA more then 1 setup packet is not supported till 3.00a */
	if (core_if->dma_enable  && (doeptsize0.b.supcnt < 2)
	    && (core_if->snpsid < CONFIG_USBDEV_REV_2_94a)) {
	     printf("\n\nCANNOT handle > 1 setup packet in DMA mode\n\n");
	}

	if ((core_if->snpsid >= CONFIG_USBDEV_REV_3_00a)
	    && ( core_if->dma_enable == 1)) {
		ctrl = (pcd->setup_pkt +(3 - doeptsize0.b.supcnt - 1 +
			ep0->dwc_ep.stp_rollover))->req;
	}

	/* Clean up the request queue */
	dwc_request_nuke(ep0);
	ep0->stopped = 0;

	if (ctrl.bmRequestType & UE_DIR_IN)
	{
		ep0->dwc_ep.is_in = 1;
		pcd->ep0state = EP0_IN_DATA_PHASE;
	}
	else
	{
		ep0->dwc_ep.is_in = 0;
		pcd->ep0state = EP0_OUT_DATA_PHASE;
	}

	if (UGETW(ctrl.wLength) == 0)
	{
		ep0->dwc_ep.is_in = 1;
		pcd->ep0state = EP0_IN_STATUS_PHASE;
	}

	switch (ctrl.bRequest) {
	case UR_GET_STATUS:
		do_get_status(pcd);
		break;

	case UR_SET_ADDRESS:
		{
		dcfg_data_t dcfg;
		dcfg.d32= readl(REG_DCFG);
		dcfg.b.devaddr = UGETW(ctrl.wValue);
		writel(dcfg.d32, REG_DCFG);
		do_setup_in_status_phase(pcd);
		}
		break;

	case UR_SET_INTERFACE:
	case UR_SET_CONFIG:
	case UR_SYNCH_FRAME:
	default:
		uplayer_setup(pcd, (uint8_t *)&ctrl);
		break;
	}
}



/**
* This function completes the ep0 control transfer.
*/
static int32_t ep0_complete_request(dwc_otg_pcd_ep_t * ep)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(ep->pcd);

	deptsiz0_data_t deptsiz;
	dwc_otg_pcd_request_t *req;
	int is_last = 0;
	dwc_otg_pcd_t *pcd = ep->pcd;

	if (pcd->ep0_pending && DWC_CIRCLEQ_EMPTY(&ep->queue)) {
		if (ep->dwc_ep.is_in) {
			do_setup_out_status_phase(pcd);
		} else {
			do_setup_in_status_phase(pcd);
		}
		pcd->ep0_pending = 0;
		return 1;
	}

	if (DWC_CIRCLEQ_EMPTY(&ep->queue)) {
		return 0;
	}
	req = DWC_CIRCLEQ_FIRST(&ep->queue);

	if (pcd->ep0state == EP0_OUT_STATUS_PHASE
		|| pcd->ep0state == EP0_IN_STATUS_PHASE) {
		is_last = 1;
	} else if (ep->dwc_ep.is_in) {
		deptsiz.d32 = readl(REG_DIEPTSIZ0);

		if (deptsiz.b.xfersize == 0) {
			req->actual = ep->dwc_ep.xfer_count;
			/* Is a Zero Len Packet needed? */
			if (req->sent_zlp) {
				req->sent_zlp = 0;
			}
			do_setup_out_status_phase(pcd);
		}
	} else {
		/* ep0-OUT */
		req->actual = ep->dwc_ep.xfer_count;

		/* Is a Zero Len Packet needed? */
		if (req->sent_zlp) {
			req->sent_zlp = 0;
		}
		/* For older cores do setup in status phase in Slave/BDMA modes,
		 * starting from 3.00 do that only in slave, and for DMA modes
		 * just re-enable ep 0 OUT here*/

		if (core_if->dma_enable == 0
		    || (core_if->snpsid <= CONFIG_USBDEV_REV_2_94a)){
			do_setup_in_status_phase(pcd);
		} else if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a){
			ep0_out_start(core_if, pcd);
		}
	}

	/* Complete the request */
	if (is_last) {
		dwc_request_done(ep, req, 0);
		ep->dwc_ep.start_xfer_buff = 0;
		ep->dwc_ep.xfer_buff = 0;
		ep->dwc_ep.xfer_len = 0;
		return 1;
	}
	return 0;
}


/**
* This function completes the request for the EP. If there are
* additional requests for the EP in the queue they will be started.
*/
static void complete_ep(dwc_otg_pcd_ep_t * ep)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(ep->pcd);
	deptsiz_data_t deptsiz;
	dwc_otg_pcd_request_t *req = 0;
	uint32_t byte_count = 0;
	int is_last = 0;

	/* Get any pending requests */
	if (!DWC_CIRCLEQ_EMPTY(&ep->queue)) {
		req = DWC_CIRCLEQ_FIRST(&ep->queue);
		if (!req) {
		DWC_PRINTF( "complete_ep 0x%p, req = NULL!\n", ep);
			return;
		}
	} else {
		DWC_PRINTF( "complete_ep 0x%p, ep->queue empty!\n", ep);
		return;
	}

	if (ep->dwc_ep.is_in) {
		deptsiz.d32 = readl(REG_DIEPTSIZ1);
		if (core_if->dma_enable) {
			if (deptsiz.b.xfersize == 0&& deptsiz.b.pktcnt == 0) {
				byte_count =ep->dwc_ep.xfer_len -
					ep->dwc_ep.xfer_count;
				ep->dwc_ep.xfer_buff += byte_count;
				ep->dwc_ep.dma_addr += byte_count;
				ep->dwc_ep.xfer_count += byte_count;

				if (ep->dwc_ep.xfer_len <
					ep->dwc_ep.total_len) {
					dwc_otg_ep_start_transfer
					(core_if, &ep->dwc_ep);
				} else if (ep->dwc_ep.sent_zlp) {
					ep->dwc_ep.sent_zlp = 0;
					dwc_otg_ep_start_zl_transfer
					(core_if, &ep->dwc_ep);
				} else {
						is_last = 1;
				}
			}
		} else {

			if (deptsiz.b.xfersize == 0 && deptsiz.b.pktcnt == 0) {
				if (ep->dwc_ep.xfer_len < ep->dwc_ep.total_len) {
					dwc_otg_ep_start_transfer(core_if,&ep->dwc_ep);
				} else if (ep->dwc_ep.sent_zlp) {
					ep->dwc_ep.sent_zlp = 0;
					dwc_otg_ep_start_zl_transfer(core_if,&ep->dwc_ep);
				} else {
					is_last = 1;
				}
			} else {
				DWC_PRINTF( "Incomplete transfer (%d-%s [siz=%d pkt=%d])\n",
				ep->dwc_ep.num,(ep->dwc_ep.is_in ? "IN" : "OUT"),
				deptsiz.b.xfersize, deptsiz.b.pktcnt);
			}
		}
	} else {
		if (core_if->dma_enable) {

			deptsiz.d32 = 0;
			deptsiz.d32 =readl(REG_DOEPTSIZ2);

			byte_count = (ep->dwc_ep.xfer_len -
					ep->dwc_ep.xfer_count -
					deptsiz.b.xfersize);
			ep->dwc_ep.xfer_buff += byte_count;
			ep->dwc_ep.dma_addr += byte_count;
			ep->dwc_ep.xfer_count += byte_count;

			/*Check if the whole transfer was completed,
			*if no, setup transfer for next portion of data
			*/
			if (ep->dwc_ep.xfer_len < ep->dwc_ep.total_len) {
				dwc_otg_ep_start_transfer(core_if,&ep->dwc_ep);
			} else if (ep->dwc_ep.sent_zlp) {
				ep->dwc_ep.sent_zlp = 0;
				dwc_otg_ep_start_zl_transfer(core_if,&ep->dwc_ep);
			} else {
				is_last = 1;
			}

		} else {
			/*Check if the whole transfer was completed,
			*if no, setup transfer for next portion of data
			*/
			if (ep->dwc_ep.xfer_len < ep->dwc_ep.total_len) {
				dwc_otg_ep_start_transfer(core_if, &ep->dwc_ep);
			} else if (ep->dwc_ep.sent_zlp) {
				/*
				* This fragment of code should initiate 0
				* length transfer in case if it is queued
				* a transfer with size divisible to EPs max
				* packet size and with usb_request zero field
				* is set, which means that after data is transfered,
				* it is also should be transfered
				* a 0 length packet at the end. For Slave and
				* Buffer DMA modes in this case SW has
				* to initiate 2 transfers one with transfer size,
				* and the second with 0 size. For Descriptor
				* DMA mode SW is able to initiate a transfer,
				* which will handle all the packets including
				* the last  0 length.
				*/
				ep->dwc_ep.sent_zlp = 0;
				dwc_otg_ep_start_zl_transfer(core_if,&ep->dwc_ep);
			} else {
				is_last = 1;
			}
		}
	}

	/* Complete the request */
	if (is_last) {
		req->actual = ep->dwc_ep.xfer_count;
		dwc_request_done(ep, req, 0);
		ep->dwc_ep.start_xfer_buff = 0;
		ep->dwc_ep.xfer_buff = 0;
		ep->dwc_ep.xfer_len = 0;

		/* If there is a request in the queue start it. */
		start_next_request(ep);
	}
}



/**
* This function handles EP0 Control transfers.
*
* The state of the control tranfers are tracked in
* <code>ep0state</code>.
*/
static void handle_ep0(dwc_otg_pcd_t * pcd)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;
	deptsiz0_data_t deptsiz;
	uint32_t byte_count;

	switch (pcd->ep0state) {
	case EP0_DISCONNECT:
		break;

	case EP0_IDLE:
		pcd->request_config = 0;
		pcd_setup(pcd);
		break;

	case EP0_IN_DATA_PHASE:

		if (core_if->dma_enable != 0) {
			/*
			* For EP0 we can only program 1 packet at a time so we
			* need to do the make calculations after each complete.
			* Call write_packet to make the calculations, as in
			* slave mode, and use those values to determine if we
			* can complete.
			*/
			deptsiz.d32 =readl(REG_DIEPTSIZ0);
			byte_count =ep0->dwc_ep.xfer_len - deptsiz.b.xfersize;
			ep0->dwc_ep.xfer_count += byte_count;
			ep0->dwc_ep.xfer_buff += byte_count;
			ep0->dwc_ep.dma_addr += byte_count;
		}

		if (ep0->dwc_ep.xfer_count < ep0->dwc_ep.total_len) {
			dwc_otg_ep0_continue_transfer(GET_CORE_IF(pcd),&ep0->dwc_ep);
			DWC_PRINTF(  "CONTINUE TRANSFER\n");
		} else if (ep0->dwc_ep.sent_zlp) {
			dwc_otg_ep0_continue_transfer(GET_CORE_IF(pcd),&ep0->dwc_ep);
			ep0->dwc_ep.sent_zlp = 0;
			DWC_PRINTF(  "CONTINUE TRANSFER\n");
		} else {
			ep0_complete_request(ep0);
			DWC_PRINTF(  "COMPLETE TRANSFER\n");
		}
		break;
	case EP0_OUT_DATA_PHASE:
		if (core_if->dma_enable != 0) {

			deptsiz.d32 = readl(REG_DOEPTSIZ0);
			byte_count =  ep0->dwc_ep.maxpacket - deptsiz.b.xfersize;

			ep0->dwc_ep.xfer_count += byte_count;
			ep0->dwc_ep.xfer_buff += byte_count;
			ep0->dwc_ep.dma_addr += byte_count;
		}
		if (ep0->dwc_ep.xfer_count < ep0->dwc_ep.total_len) {
			dwc_otg_ep0_continue_transfer(GET_CORE_IF(pcd),&ep0->dwc_ep);
			DWC_PRINTF( "CONTINUE TRANSFER\n");
		} else if (ep0->dwc_ep.sent_zlp) {
			dwc_otg_ep0_continue_transfer(GET_CORE_IF(pcd),&ep0->dwc_ep);
			ep0->dwc_ep.sent_zlp = 0;
			DWC_PRINTF( "CONTINUE TRANSFER\n");
		} else {
			ep0_complete_request(ep0);
			DWC_PRINTF(  "COMPLETE TRANSFER\n");
		}
		break;

	case EP0_IN_STATUS_PHASE:
	case EP0_OUT_STATUS_PHASE:
		DWC_PRINTF( "CASE: EP0_STATUS\n");
		ep0_complete_request(ep0);
		pcd->ep0state = EP0_IDLE;
		ep0->stopped = 1;
		ep0->dwc_ep.is_in = 0;	/* OUT for next SETUP */

		/* Prepare for more SETUP Packets */
		if (core_if->dma_enable) {
			ep0_out_start(core_if, pcd);
		}
		break;

	case EP0_STALL:
		DWC_PRINTF( "EP0 STALLed, should not get here pcd_setup()\n");
		break;
	}
}


/**
* This interrupt occurs when a USB Reset is detected. When the USB
* Reset Interrupt occurs the device state is set to DEFAULT and the
* EP0 state is set to IDLE.
*	-#	Set the NAK bit for all OUT endpoints (DOEPCTLn.SNAK = 1)
*	-#	Unmask the following interrupt bits
*		- DAINTMSK.INEP0 = 1 (Control 0 IN endpoint)
*	- DAINTMSK.OUTEP0 = 1 (Control 0 OUT endpoint)
*	- DOEPMSK.SETUP = 1
*	- DOEPMSK.XferCompl = 1
*	- DIEPMSK.XferCompl = 1
*	- DIEPMSK.TimeOut = 1
*	-# Program the following fields in the endpoint specific registers
*	for Control OUT EP 0, in order to receive a setup packet
*	- DOEPTSIZ0.Packet Count = 3 (To receive up to 3 back to back
*	  setup packets)
*	- DOEPTSIZE0.Transfer Size = 24 Bytes (To receive up to 3 back
*	  to back setup packets)
*		- In DMA mode, DOEPDMA0 Register with a memory address to
*		  store any setup packets received
* At this point, all the required initialization, except for enabling
* the control 0 OUT endpoint is done, for receiving SETUP packets.
*/
int32_t dwc_handle_usb_reset_intr(dwc_otg_pcd_t * pcd)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
	depctl_data_t doepctl = {.d32 = 0 };
	depctl_data_t diepctl = {.d32 = 0 };
	daint_data_t daintmsk = {.d32 = 0 };
	doepmsk_data_t doepmsk = {.d32 = 0 };
	diepmsk_data_t diepmsk = {.d32 = 0 };
	dcfg_data_t dcfg = {.d32 = 0 };
	dctl_data_t dctl = {.d32 = 0 };
	grstctl_t resetctl = {.d32 = 0 };
	gintsts_data_t gintsts;

	DWC_PRINTF("USB RESET\n");

	/* Clear the Remote Wakeup Signalling */
	dctl.d32=readl(REG_DCTL);
	dctl.b.rmtwkupsig = 0;
	writel(dctl.d32, REG_DCTL);

	/* Set NAK for all OUT EPs */
	doepctl.b.snak = 1;

	writel(doepctl.d32,REG_DOEPCTL0);
	writel(doepctl.d32,REG_DOEPCTL2);


	/* Flush the  Tx FIFO */
	dwc_flush_tx_fifo( 0x10);

	resetctl.b.intknqflsh = 1;
	writel(resetctl.d32,REG_GRSTCTL);


	if (!core_if->en_multiple_tx_fifo && core_if->dma_enable) {
		core_if->start_predict = 0;
		core_if->nextep_seq[1] = 0xff;	// 0xff - EP not active
		core_if->nextep_seq[0] = 0;	
		core_if->first_in_nextep_seq = 0;
		diepctl.d32 = readl(REG_DIEPCTL0);
		diepctl.b.nextep = 0;
		writel(diepctl.d32,REG_DIEPCTL0);

		/* Update IN Endpoint Mismatch Count by active IN NP EP count + 1 */
		dcfg.d32 = readl(REG_DCFG);
		dcfg.b.epmscnt = 2;
		writel(dcfg.d32,REG_DCFG);
	}
	daintmsk.b.inep0 = 1;
	daintmsk.b.outep0 = 1;
	writel(daintmsk.d32,REG_DAINTMSK);
	doepmsk.b.setup = 1;
	doepmsk.b.xfercompl = 1;
	if ((core_if->dma_enable)&&(core_if->snpsid >= CONFIG_USBDEV_REV_3_00a)) {
		doepmsk.b.stsphsercvd = 1;
	}
	writel(doepmsk.d32,REG_DOEMSK);
	diepmsk.b.xfercompl = 1;
	if (!core_if->en_multiple_tx_fifo && core_if->dma_enable)
		diepmsk.b.intknepmis = 0; 
	writel(diepmsk.d32,REG_DIEMSK);

	/* Reset Device Address */
	dcfg.d32 = readl(REG_DCFG);
	dcfg.b.devaddr = 0;
	writel(dcfg.d32,REG_DCFG);

	/* setup EP0 to receive SETUP packets */
	if (core_if->snpsid <= CONFIG_USBDEV_REV_2_94a)
	ep0_out_start(core_if, pcd);
	
	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.usbreset = 1;
	writel(gintsts.d32,REG_GINTSTS);

	return 1;
}
/**
* Read the device status register and set the device speed in the
* data structure.
* Set up EP0 to receive SETUP packets by calling dwc_ep0_activate.
*/
int32_t dwc_handle_enum_done_intr(dwc_otg_pcd_t * pcd)
{
	dwc_otg_pcd_ep_t *ep0 = &pcd->ep0;
	gintsts_data_t gintsts;
	gusbcfg_data_t gusbcfg;
	uint8_t utmi16b, utmi8b;
	int speed;
	DWC_PRINTF("SPEED ENUM\n");

	utmi16b = 6;	//vahrama old value was 6;
	utmi8b = 9;

	dwc_ep0_activate( &ep0->dwc_ep);
	if (GET_CORE_IF(pcd)->snpsid >= CONFIG_USBDEV_REV_3_00a) {
		ep0_out_start(GET_CORE_IF(pcd), pcd);
	}
	pcd->ep0state = EP0_IDLE;
	ep0->stopped = 0;

	/* Set USB turnaround time based on device speed and PHY interface. */
	gusbcfg.d32 = readl(REG_GUSBCFG);
	speed = dwc_get_device_speed();

	if (speed == USB_SPEED_HIGH) {
		/* UTMI+ interface */
			gusbcfg.b.usbtrdtim = utmi8b;
	}
	else {
		/* Full or low speed */
		gusbcfg.b.usbtrdtim = 9;
	}

	writel(gusbcfg.d32,REG_GUSBCFG);

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.enumdone = 1;
	writel(gintsts.d32,REG_GINTSTS);
	return 1;
}


/**
* This function handles the Rx Status Queue Level Interrupt, which
* indicates that there is a least one packet in the Rx FIFO.  The
* packets are moved from the FIFO to memory, where they will be
* processed when the Endpoint Interrupt Register indicates Transfer
* Complete or SETUP Phase Done.
* Repeat the following until the Rx Status Queue is empty:
*	 -# Read the Receive Status Pop Register (GRXSTSP) to get Packet
*		info
*	 -# If Receive FIFO is empty then skip to step Clear the interrupt
*		and exit
*	 -# If SETUP Packet call dwc_otg_read_setup_packet to copy the
*		SETUP data to the buffer
*	 -# If OUT Data Packet call dwc_otg_read_packet to copy the data
*		to the destination buffer
*/
int32_t dwc_handle_rx_status_q_level_intr(dwc_otg_pcd_t * pcd)
{
	gintmsk_data_t gintmask = {.d32 = 0 };
	device_grxsts_data_t status;
	dwc_otg_pcd_ep_t *ep =NULL;
	gintsts_data_t gintsts;

	DWC_PRINTF("receive  packet\n");

	/* Disable the Rx Status Queue Level interrupt */
	gintmask.b.rxstsqlvl = 1;
	DWC_MODIFY_REG32(REG_GINTMSK, gintmask.d32, 0);

	/* Get the Status from the top of the FIFO */
	status.d32 = readl(REG_GRXSTSP);

	/* Get pointer to EP structure */
	if (status.b.epnum == 0) {
		ep = &pcd->ep0;
	} else if(status.b.epnum == 2) {
		ep =  &pcd->out_ep[0];
	} else {
		DWC_PRINTF("no this epnum\n");
		return -1;
	}

	switch (status.b.pktsts) {
	case DWC_STS_DATA_UPDT:
		DWC_PRINTF("receive data packet\n");
		if (status.b.bcnt && ep->dwc_ep.xfer_buff) {
			dwc_read_packet(ep->dwc_ep.xfer_buff,status.b.bcnt);
			ep->dwc_ep.xfer_count += status.b.bcnt;
			ep->dwc_ep.xfer_buff += status.b.bcnt;
		}
		break;
	case DWC_DSTS_SETUP_UPDT:
		DWC_PRINTF("receive setup packet\n");
		dwc_read_setup_packet(pcd->setup_pkt->d32);
		DWC_PRINTF(
			    "SETUP PKT: %02x.%02x v%04x i%04x l%04x\n",
			    pcd->setup_pkt->req.bmRequestType,
			    pcd->setup_pkt->req.bRequest,
			    UGETW(pcd->setup_pkt->req.wValue),
			    UGETW(pcd->setup_pkt->req.wIndex),
			    UGETW(pcd->setup_pkt->req.wLength));
		ep->dwc_ep.xfer_count += status.b.bcnt;
		break;
	case DWC_DSTS_GOUT_NAK:
	case DWC_STS_XFER_COMP:
	case DWC_DSTS_SETUP_COMP:
	default:
		break;
	}

	/* Enable the Rx Status Queue Level interrupt */
	DWC_MODIFY_REG32(REG_GINTMSK, 0, gintmask.d32);

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.rxstsqlvl = 1;
	writel(gintsts.d32,REG_GINTSTS);

	return 1;
}

/**
* This interrupt indicates that an IN EP has a pending Interrupt.
* The sequence for handling the IN EP interrupt is shown below:
* -#	Read the Device All Endpoint Interrupt register
* -#	Repeat the following for each IN EP interrupt bit set (from
*		LSB to MSB).
* -#	Read the Device Endpoint Interrupt (DIEPINTn) register
* -#	If "Transfer Complete" call the request complete function
* -#	If "Endpoint Disabled" complete the EP disable procedure.
* -#	If "AHB Error Interrupt" log error
* -#	If "Time-out Handshake" log error
* -#	If "IN Token Received when TxFIFO Empty" write packet to Tx
*		FIFO.
* -#	If "IN Token EP Mismatch" (disable, this is handled by EP
*		Mismatch Interrupt)
*/
static int32_t dwc_handle_in_ep_intr(dwc_otg_pcd_t * pcd)
{
#define CLEAR_IN_EP_INTR(__epnum,__intr) \
do { \
		diepint_data_t diepint = {.d32=0}; \
		diepint.b.__intr = 1; \
		writel(diepint.d32,REG_DIEPINT0+0x20*__epnum); \
} while (0)

	diepint_data_t diepint = {.d32 = 0 };
	depctl_data_t depctl = {.d32 = 0 };
	gintmsk_data_t intr_mask = {.d32 = 0 };
	uint32_t ep_intr;
	uint32_t epnum = 0;
	dwc_otg_pcd_ep_t *ep = NULL;
	dwc_ep_t *dwc_ep;

	DWC_PRINTF("in_ep_intr\n");
	/* Read in the device interrupt bits */
	ep_intr = (readl(REG_DAINT) &readl(REG_DAINTMSK )& 0xffff);

	/* Service the Device IN interrupts for each endpoint */
	while (ep_intr) {
		if (ep_intr & 0x1) {
			uint32_t empty_msk, msk;
			/* Get EP pointer */
			if (epnum == 0) {
				ep= &pcd->ep0;
			}
			else if (epnum == 1)
			{
				ep=&pcd->in_ep[0];
			} else {
				DWC_PRINTF("no this epnum\n");
				epnum++;
				ep_intr >>= 1;
				continue;
			}
			dwc_ep = &ep->dwc_ep;

			depctl.d32 =readl(REG_DIEPCTL0+0x20*epnum);
			empty_msk =readl(REG_DTKNQR4);
			msk = readl(REG_DIEMSK);
			msk |= ((empty_msk >> dwc_ep->num) & 0x1) << 7;
			diepint.d32 = readl(REG_DIEPINT0+0x20*dwc_ep->num) & msk;

			/** IN EP Tx FIFO Empty Intr */
			if (diepint.b.emptyintr) {
				write_empty_tx_fifo(pcd, epnum);
				CLEAR_IN_EP_INTR(epnum, emptyintr);
			}

			/* Transfer complete */
			if (diepint.b.xfercompl)
			{
				/* Disable the NP Tx FIFO Empty
				 * Interrrupt */
				if (core_if->en_multiple_tx_fifo == 0) {
					intr_mask.b.nptxfempty = 1;
					DWC_MODIFY_REG32(REG_GINTMSK,intr_mask.d32, 0);
				} else {
				/* Disable the Tx FIFO Empty Interrupt for this EP */
				uint32_t fifoemptymsk = 0x1 << dwc_ep->num;
				DWC_MODIFY_REG32(REG_DTKNQR4,fifoemptymsk, 0);
				}
				/* Clear the bit in DIEPINTn for this interrupt */
				CLEAR_IN_EP_INTR(epnum, xfercompl);

				/* Complete the transfer */
				if (epnum == 0) {
					handle_ep0(pcd);
				}
				else
				{
					complete_ep(ep);
					if(diepint.b.nak)
					CLEAR_IN_EP_INTR( epnum, nak);
				}
			}
		}
		epnum++;
		ep_intr >>= 1;
	}
	return 1;
#undef CLEAR_IN_EP_INTR
}

/**
* This interrupt indicates that an OUT EP has a pending Interrupt.
* The sequence for handling the OUT EP interrupt is shown below:
* -#	Read the Device All Endpoint Interrupt register
* -#	Repeat the following for each OUT EP interrupt bit set (from
*		LSB to MSB).
* -#	Read the Device Endpoint Interrupt (DOEPINTn) register
* -#	If "Transfer Complete" call the request complete function
* -#	If "Endpoint Disabled" complete the EP disable procedure.
* -#	If "AHB Error Interrupt" log error
* -#	If "Setup Phase Done" process Setup Packet (See Standard USB
*		Command Processing)
*/
static int32_t dwc_handle_out_ep_intr(dwc_otg_pcd_t * pcd)
{
#define CLEAR_OUT_EP_INTR(__epnum,__intr) \
do { \
		doepint_data_t doepint = {.d32=0}; \
		doepint.b.__intr = 1; \
		writel(doepint.d32,REG_DOEPINT0+__epnum*0x20); \
} while (0)

	dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
	uint32_t ep_intr;
	doepint_data_t doepint = {.d32 = 0 };
	uint32_t epnum = 0;
	dwc_otg_pcd_ep_t *ep = NULL;
	dwc_ep_t *dwc_ep;

	DWC_PRINTF("out_ep_intr\n");
	/* Read in the device interrupt bits */
	ep_intr =  ((readl(REG_DAINT) &	readl(REG_DAINTMSK) & 0xffff0000) >> 16);

	while (ep_intr) {
		if (ep_intr & 0x1) {
			/* Get EP pointer */
			if (epnum == 0) {
				ep = &pcd->ep0;
			} else if(epnum == 2) {
				ep =  &pcd->out_ep[0];
			} else {
				DWC_PRINTF("no this epnum\n");
				epnum++;
				ep_intr >>= 1;
				continue;
			}
			dwc_ep = &ep->dwc_ep;

			doepint.d32 = (readl(REG_DOEPINT0+dwc_ep->num*0x20)&readl(REG_DOEMSK));

			/* Moved this interrupt upper due to core deffect of asserting
			 * OUT EP 0 xfercompl along with stsphsrcvd in BDMA */
			if (doepint.b.stsphsercvd) {
				deptsiz0_data_t deptsiz;
				CLEAR_OUT_EP_INTR(epnum, stsphsercvd);
				deptsiz.d32 =readl(REG_DOEPTSIZ0+dwc_ep->num*0x20);
				if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a
				&&core_if->dma_enable&& doepint.b.xfercompl
				&& deptsiz.b.xfersize == 24) {
					CLEAR_OUT_EP_INTR( epnum,xfercompl);
					doepint.b.xfercompl = 0;
					ep0_out_start(core_if, pcd);
				}
				if ((core_if->dma_enable)&& (core_if->snpsid >=
				     CONFIG_USBDEV_REV_3_00a))  {
					do_setup_in_status_phase(pcd);
				}
			}

			/* Transfer complete */
			if (doepint.b.xfercompl) {
				if (epnum == 0) {
					/* Clear the bit in DOEPINTn for this interrupt */
				    CLEAR_OUT_EP_INTR( epnum, xfercompl);
				    if (core_if->snpsid >= CONFIG_USBDEV_REV_3_00a) {
					if (core_if->dma_enable == 0) {
						doepint_data_t doepint;
						doepint.d32 = readl(REG_DOEPINT0);
						if (pcd->ep0state == EP0_IDLE && doepint.b.sr) {
							CLEAR_OUT_EP_INTR( epnum, sr);
							goto exit_xfercompl;
						}
					}

					if ( core_if->dma_enable) {
						doepint_data_t doepint_temp = {.d32 = 0};
						deptsiz0_data_t doeptsize0 = {.d32 = 0 };
						doepint_temp.d32 = readl(REG_DOEPINT0);
						doeptsize0.d32 = readl(REG_DOEPTSIZ0);
						if (pcd->ep0state == EP0_IDLE) {
							if (doepint_temp.b.sr) {
								CLEAR_OUT_EP_INTR(epnum, sr);
							}
							doepint.d32 = readl(REG_DOEPINT0);
							if (doeptsize0.b.supcnt == 3) {
								ep->dwc_ep.stp_rollover = 1;
							}
							if (doepint.b.setup) {
retry:
								/* Already started data stage, clear setup */
								CLEAR_OUT_EP_INTR( epnum, setup);
								doepint.b.setup = 0;
								handle_ep0(pcd);
								ep->dwc_ep.stp_rollover = 0;
								/* Prepare for more setup packets */
								if (pcd->ep0state == EP0_IN_STATUS_PHASE ||
									pcd->ep0state == EP0_IN_DATA_PHASE) {
									ep0_out_start(core_if, pcd);
								}
								goto exit_xfercompl;
							} else {
								doepint.d32 = readl(REG_DOEPINT0);
								if(doepint.b.setup)
									goto retry;
									ep0_out_start(core_if, pcd);
								}
							} else {
								dwc_otg_pcd_request_t *req;
								diepint_data_t diepint0 = {.d32 = 0};
								doepint_data_t doepint_temp = {.d32 = 0};
								depctl_data_t diepctl0;
								diepint0.d32 = readl(REG_DIEPINT0);
								diepctl0.d32 = readl(REG_DIEPCTL0);

								if (pcd->ep0state == EP0_IN_DATA_PHASE
									|| pcd->ep0state == EP0_IN_STATUS_PHASE) {
									if (diepint0.b.xfercompl) {
										writel(diepint0.d32,REG_DIEPINT0 );
									}
									if (diepctl0.b.epena) {
										diepint_data_t diepint = {.d32 = 0};
										diepctl0.b.snak = 1;
										writel(diepctl0.d32,REG_DIEPCTL0);
										do {
											udelay(10);
											diepint.d32 = readl(REG_DIEPINT0);
										} while (!diepint.b.inepnakeff);
										diepint.b.inepnakeff = 1;
										writel(diepint.d32,REG_DIEPINT0);
										diepctl0.d32 = 0;
										diepctl0.b.epdis = 1;
										writel(diepctl0.d32,REG_DIEPCTL0);
										do {
											udelay(10);
											diepint.d32 = readl(REG_DIEPINT0);
										} while (!diepint.b.epdisabled);
										diepint.b.epdisabled = 1;
										writel(diepint.d32,REG_DIEPINT0);
									}
								}
								doepint_temp.d32 = readl(REG_DOEPINT0);
								if (doepint_temp.b.sr) {
									CLEAR_OUT_EP_INTR(epnum, sr);
									if (DWC_CIRCLEQ_EMPTY(&ep->queue)) {

									} else {

										req = DWC_CIRCLEQ_FIRST(&ep->queue);
										if (ep->dwc_ep.xfer_count != ep->dwc_ep.total_len &&
											pcd->ep0state == EP0_OUT_DATA_PHASE) {
											/* Read arrived setup packet from req->buf */
											memcpy(&pcd->setup_pkt->req,
												req->buf + ep->dwc_ep.xfer_count, 8);
										}
										req->actual = ep->dwc_ep.xfer_count;
										dwc_request_done(ep, req, -ECONNRESET);
										ep->dwc_ep.start_xfer_buff = 0;
										ep->dwc_ep.xfer_buff = 0;
										ep->dwc_ep.xfer_len = 0;
									}
									pcd->ep0state = EP0_IDLE;
									if (doepint.b.setup) {

										/* Data stage started, clear setup */
										CLEAR_OUT_EP_INTR(epnum, setup);
										doepint.b.setup = 0;
										handle_ep0(pcd);
										/* Prepare for setup packets if ep0in was enabled*/
										if (pcd->ep0state == EP0_IN_STATUS_PHASE) {
											ep0_out_start(core_if, pcd);
										}
										goto exit_xfercompl;
									} else {
										/* Prepare for more setup packets */
										ep0_out_start(core_if, pcd);
									}
								}
							}
						}
						if (core_if->dma_enable == 0 || pcd->ep0state != EP0_IDLE)
							handle_ep0(pcd);
exit_xfercompl:					DWC_PRINTF("exit_xfercompl\n");
					} else {

							handle_ep0(pcd);
					}
				} else {
					CLEAR_OUT_EP_INTR(epnum,xfercompl);
					complete_ep(ep);
				}
			}

			/* Setup Phase Done (contorl EPs) */
			if (doepint.b.setup) {
				CLEAR_OUT_EP_INTR(epnum, setup);
				handle_ep0(pcd);
			}
		}
		epnum++;
		ep_intr >>= 1;
	}

	return 1;

#undef CLEAR_OUT_EP_INTR
}



/**
 * This function examines the Device IN Token Learning Queue to
 * determine the EP number of the last IN token received.  This
 * implementation is for the Mass Storage device where there are only
 * 2 IN EPs (Control-IN and BULK-IN).
 *
 * The EP numbers for the first six IN Tokens are in DTKNQR1 and there
 * are 8 EP Numbers in each of the other possible DTKNQ Registers.
 *
 * @param core_if Programming view of DWC_otg controller.
 *
 */
static inline int get_ep_of_last_in_token(dwc_otg_core_if_t * core_if)
{
	uint32_t TOKEN_Q_DEPTH = core_if->hwcfg2.b.dev_token_q_depth;
	/* Number of Token Queue Registers */
	int DTKNQ_REG_CNT = (TOKEN_Q_DEPTH + 7) / 8;
	dtknq1_data_t dtknqr1;
	uint32_t in_tkn_epnums[4] = {0};
	int ndx = 0;
	int i = 0;
	volatile uint32_t *addr = (volatile uint32_t *)REG_DTKNQR1;
	int epnum = 0;

	/* Read the DTKNQ Registers */
	for (i = 0; i < DTKNQ_REG_CNT; i++) {
		in_tkn_epnums[i] = readl(addr);
		DWC_PRINTF( "DTKNQR%d=0x%08x\n", i + 1,
			    in_tkn_epnums[i]);
		if (addr == (volatile uint32_t *)REG_DVBUSDIS) {
			addr = (volatile uint32_t *)REG_DTKNQR3;
		} else {
			++addr;
		}

	}

	/* Copy the DTKNQR1 data to the bit field. */
	dtknqr1.d32 = in_tkn_epnums[0];
	/* Get the EP numbers */
	in_tkn_epnums[0] = dtknqr1.b.epnums0_5;
	ndx = dtknqr1.b.intknwptr - 1;

	if (ndx == -1) {
		/** @todo Find a simpler way to calculate the max
		 * queue position.*/
		int cnt = TOKEN_Q_DEPTH;
		if (TOKEN_Q_DEPTH <= 6) {
			cnt = TOKEN_Q_DEPTH - 1;
		} else if (TOKEN_Q_DEPTH <= 14) {
			cnt = TOKEN_Q_DEPTH - 7;
		} else if (TOKEN_Q_DEPTH <= 22) {
			cnt = TOKEN_Q_DEPTH - 15;
		} else {
			cnt = TOKEN_Q_DEPTH - 23;
		}
		epnum = (in_tkn_epnums[DTKNQ_REG_CNT - 1] >> (cnt * 4)) & 0xF;
	} else {
		if (ndx <= 5) {
			epnum = (in_tkn_epnums[0] >> (ndx * 4)) & 0xF;
		} else if (ndx <= 13) {
			ndx -= 6;
			epnum = (in_tkn_epnums[1] >> (ndx * 4)) & 0xF;
		} else if (ndx <= 21) {
			ndx -= 14;
			epnum = (in_tkn_epnums[2] >> (ndx * 4)) & 0xF;
		} else if (ndx <= 29) {
			ndx -= 22;
			epnum = (in_tkn_epnums[3] >> (ndx * 4)) & 0xF;
		}
	}
	return epnum;
}


/**
 * This interrupt occurs when the non-periodic Tx FIFO is half-empty.
 * The active request is checked for the next packet to be loaded into
 * the non-periodic Tx FIFO.
 */
int32_t dwc_pcd_handle_np_tx_fifo_empty_intr(dwc_otg_pcd_t * pcd)
{
	dwc_otg_core_if_t *core_if = GET_CORE_IF(pcd);
	dwc_otg_dev_in_ep_regs_t *ep_regs;
	gnptxsts_data_t txstatus = {.d32 = 0 };
	gintsts_data_t gintsts;

	int epnum = 0;
	dwc_otg_pcd_ep_t *ep = 0;
	uint32_t len = 0;
	int dwords;

	/* Get the epnum from the IN Token Learning Queue. */
	epnum = get_ep_of_last_in_token(core_if);

	if (epnum == 0) {
		ep= &pcd->ep0;
	} else {
		ep= &pcd->in_ep[0];
	}

	DWC_PRINTF("NP TxFifo Empty: %d \n", epnum);


	len = ep->dwc_ep.xfer_len - ep->dwc_ep.xfer_count;
	if (len > ep->dwc_ep.maxpacket) {
		len = ep->dwc_ep.maxpacket;
	}
	dwords = (len + 3) / 4;

	/* While there is space in the queue and space in the FIFO and
	 * More data to tranfer, Write packets to the Tx FIFO */
	txstatus.d32 = readl(REG_GNPTXSTS);
	DWC_PRINTF("b4 GNPTXSTS=0x%08x\n", txstatus.d32);

	while (txstatus.b.nptxqspcavail > 0 &&
	       txstatus.b.nptxfspcavail > dwords &&
	       ep->dwc_ep.xfer_count < ep->dwc_ep.xfer_len) {
		/* Write the FIFO */
		dwc_ep_write_packet(&ep->dwc_ep, 0);
		len = ep->dwc_ep.xfer_len - ep->dwc_ep.xfer_count;

		if (len > ep->dwc_ep.maxpacket) {
			len = ep->dwc_ep.maxpacket;
		}

		dwords = (len + 3) / 4;
		txstatus.d32 = readl(REG_GNPTXSTS);
		DWC_PRINTF("GNPTXSTS=0x%08x\n", txstatus.d32);
	}

	DWC_PRINTF("GNPTXSTS=0x%08x\n",readl(REG_GNPTXSTS));

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.nptxfempty = 1;
	writel(gintsts.d32,REG_GINTSTS);

	return 1;
}


/**
* PCD interrupt handler.
*
* The PCD handles the device interrupts.  Many conditions can cause a
* device interrupt. When an interrupt occurs, the device interrupt
* service routine determines the cause of the interrupt and
* dispatches handling to the appropriate function. These interrupt
* handling functions are described below.
*
* All interrupt registers are processed from LSB to MSB.
*
*/
void dwc_intr_poll(void)
{
	gintsts_data_t gintr_status;

	gintr_status.d32 =(readl(REG_GINTSTS) &readl(REG_GINTMSK));

	if (gintr_status.b.usbreset) {
		dwc_handle_usb_reset_intr(pcd);
	}
	if (gintr_status.b.enumdone) {
		dwc_handle_enum_done_intr(pcd);
	}

	if (gintr_status.b.inepint) {
		dwc_handle_in_ep_intr(pcd);
	}
	if (gintr_status.b.outepintr) {
		dwc_handle_out_ep_intr(pcd);
	}
	if (gintr_status.b.rxstsqlvl) {
		dwc_handle_rx_status_q_level_intr(pcd);
	}

	if (gintr_status.b.nptxfempty) {
		dwc_pcd_handle_np_tx_fifo_empty_intr(pcd);
	}
}


static void dwc_pcd_init_ep(dwc_otg_pcd_t * pcd, dwc_otg_pcd_ep_t * pcd_ep,
		uint32_t is_in, uint32_t ep_num)
{
	/* Init EP structure */
	pcd_ep->desc = 0;
	pcd_ep->pcd = pcd;
	pcd_ep->stopped = 1;
	pcd_ep->queue_sof = 0;

	/* Init DWC ep structure */
	pcd_ep->dwc_ep.is_in = is_in;
	pcd_ep->dwc_ep.num = ep_num;
	pcd_ep->dwc_ep.active = 0;
	pcd_ep->dwc_ep.tx_fifo_num = 0;
	/* Control until ep is actvated */
	pcd_ep->dwc_ep.type = DWC_OTG_EP_TYPE_CONTROL;
	pcd_ep->dwc_ep.maxpacket = MAX_PACKET_SIZE;
	pcd_ep->dwc_ep.dma_addr = 0;
	pcd_ep->dwc_ep.start_xfer_buff = 0;
	pcd_ep->dwc_ep.xfer_buff = 0;
	pcd_ep->dwc_ep.xfer_len = 0;
	pcd_ep->dwc_ep.xfer_count = 0;
	pcd_ep->dwc_ep.sent_zlp = 0;
	pcd_ep->dwc_ep.total_len = 0;
	DWC_CIRCLEQ_INIT(&pcd_ep->queue);
}
dwc_otg_pcd_t *dwc_pcd_init(dwc_otg_core_if_t *core_if)
{

	if(pcd != NULL)
	{
		free(pcd->setup_pkt);
		free(pcd->status_buf);
		free(pcd);
		pcd = NULL;
	}

	pcd = malloc(sizeof(dwc_otg_pcd_t));
	if (pcd == NULL) {
		return NULL;
	}

	pcd->core_if = core_if;

	/*
	* Initialize the buffer for SETUP packets
	*/
	pcd->setup_pkt = malloc(sizeof(*pcd->setup_pkt) * 5);
	pcd->setup_pkt_dma_handle=(dwc_dma_t)pcd->setup_pkt;
	if (pcd->setup_pkt == NULL) {
		free(pcd);
		return NULL;
	}

	pcd->status_buf = malloc(sizeof(uint16_t));
	pcd->status_buf_dma_handle = (dwc_dma_t)pcd->status_buf;
	if (pcd->status_buf == NULL) {
		free(pcd->setup_pkt);
		free(pcd);
		return NULL;
	}


	dwc_pcd_init_ep(pcd, &pcd->ep0, 0, 0);
	dwc_pcd_init_ep(pcd, &pcd->in_ep[0], 1 /* IN */ , 1);
	dwc_pcd_init_ep(pcd, &pcd->out_ep[0], 0 /* OUT */ , 2);

	pcd->ep0state = EP0_DISCONNECT;
	pcd->ep0.dwc_ep.maxpacket = MAX_EP0_SIZE;
	pcd->ep0.dwc_ep.type = DWC_OTG_EP_TYPE_CONTROL;

	return pcd;
}

int dwc_dev_init(void)
{
	core_if = malloc(sizeof(dwc_otg_core_if_t));

	if (!core_if) {
		printf("Not enough malloc space available!\n");
		return -1;
	}

	/*
	* Store the contents of the hardware configuration registers here for
	* easy access later.
	*/
	core_if->hwcfg2.d32 =readl(REG_GHWCFG2);
	core_if->hwcfg3.d32 =readl(REG_GHWCFG3);
	core_if->hwcfg4.d32 =readl(REG_GHWCFG4);
	core_if->snpsid = readl(REG_GSNPSID);
	DWC_PRINTF("Core Release: %x.%x%x%x\n",
		   (core_if->snpsid >> 12 & 0xF),
		   (core_if->snpsid >> 8 & 0xF),
		   (core_if->snpsid >> 4 & 0xF), (core_if->snpsid & 0xF));
	core_if->en_multiple_tx_fifo = core_if->hwcfg4.b.ded_fifo_en;
	core_if->dma_enable = 0;
	
	pcd = dwc_pcd_init(core_if);

	usb_init_ep0();
	usb_config_value = 1;
	usb_init_ep12();
	rx_cmd();
	usb_out_open = 1;
	printf("start download process.\n");

	return 0;
}


