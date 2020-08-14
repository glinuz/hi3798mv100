/*
 * Most of this source has been derived from the Linux USB
 */

/*
 * How it works:
 *
 * Since this is a bootloader, the devices will not be automatic
 * (re)configured on hotplug, but after a restart of the USB the
 * device should work.
 *
 * For each transfer (except "Interrupt") we wait for completion.
 */
#include <compile.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <flash.h>
#include <string.h>
#include <malloc.h>
#include <delay.h>

#include "part.h"
#include "usb.h"

#ifdef CONFIG_ARCH_HI3716MV310
#  include "hiusb-hi3716mv310.c"
#endif

static struct usb_device usb_dev[USB_MAX_DEVICE];
static int dev_index;
static ccb usb_ccb;
static u32 CBWTag;
int usb_max_devs; /* number of highest available usb device */
static struct us_data usb_stor[USB_MAX_STOR_DEV];
block_dev_desc_t usb_dev_desc[USB_MAX_STOR_DEV];

int rootdev;
struct ehci_hccr *ghccr;
volatile struct ehci_hcor *ghcor;
static uint16_t portreset;
#define roundup(x, y)		((((x) + ((y) - 1)) / (y)) * (y))
static char __qh_list[roundup(sizeof(struct QH), 32)] __attribute__((aligned(32)));				
static struct QH*qh_list = (struct QH *)__qh_list ;	

/* root hub descriptor */
struct descriptor  root_hub_descriptor = {
	{0x8, 0x29, 2, 0, 10 },
	{0x12, 1, 0x0200, 9, 0, 1, 64, 0, 0, 0x0100, 1, 2, 0, 1},
	{0x9, 2, 0x19, 1, 1, 0, 0x40, 0},
	{0x9,4,0,0,1,9,0,0,0	},
	{0x7,5,0x81,3,	8,255},
};

/***********************the bottom layer: ehci **************************/
static inline int min3(int a, int b, int c)
{
	if (b < a)
		a = b;
	if (c < a)
		a = c;
	return a;
}

/* timeout wait */
static int handshake(uint32_t *ptr, uint32_t mask, uint32_t done, int usec)
{
	uint32_t result;
	do {
		result = ehci_readl(ptr);
		udelay(5);
		if (result == ~(uint32_t)0)
			return -1;
		result &= mask;
		if (result == done)
			return 0;
		usec--;
	} while (usec > 0);
	return -1;
}

static int ehci_reset(void)
{
	uint32_t cmd;
	int ret = 0;
	
	cmd = ehci_readl(&ghcor->or_usbcmd);
	cmd &= ~CMD_RUN;
	cmd |= CMD_RESET;
	ehci_writel(&ghcor->or_usbcmd, cmd);
	
	ret = handshake((uint32_t *)&ghcor->or_usbcmd, CMD_RESET, 0, 250 * 1000);
	if (ret < 0) {
		printf("EHCI fail to reset\n");
	}

	return ret;
}

/* fill up data to qtd */
static int ehci_td_buffer(struct qTD *td, void *buf, size_t sz)
{
#define ALIGNE	0x1000
	uint32_t delta, next;
	uint32_t addr = (uint32_t)buf;
	int idx = 0;

	td->qt_buffer[0] = addr;
	td->qt_buffer_hi[0] = 0;
	next = (addr + ALIGNE) & ~(ALIGNE - 1);
	delta = next - addr;
	if (sz <= delta)
		return 0;
	sz -= delta;
	addr = next;

	for (idx = 1;idx < 5; idx++) {
		td->qt_buffer[idx] = addr;
		td->qt_buffer_hi[idx] = 0;
		if (sz <= ALIGNE)
			break;
		sz -= ALIGNE;
		addr += ALIGNE;
	}

	if (idx == 5) {
		printf("out of buffer pointers (%u bytes left)\n", sz);
		return -1;
	}

	return 0;
}

static void setup_qh(struct usb_device *dev, unsigned long pipe, struct QH* qh)
{
	int maxpacket;

	memset(qh, 0, sizeof(struct QH));
	/*
	 * Setup QH (3.6 in ehci-r10.pdf)
	 */
	qh->qh_link = (uint32_t)qh_list | QH_LINK_TYPE_QH;
	
	if (pipe & USB_DIR_IN)
		maxpacket = dev->epmaxpacketin[((pipe>>15) & 0xf)];
	else
		maxpacket = dev->epmaxpacketout[((pipe>>15) & 0xf)];
	
	qh->qh_endpt1 = (8 << 28) |(maxpacket << 16) |
	    (0 << 15) |(1 << 14) |(usb_pipespeed(pipe) << 12) |
	    (usb_pipeendpoint(pipe) << 8) | (0 << 7) | (usb_pipedevice(pipe) << 0);

	if ((usb_pipespeed(pipe) != USB_SPEED_HIGH) 
	    && (usb_pipeendpoint(pipe) == 0))
		qh->qh_endpt1 |= 1 << 27;

	qh->qh_endpt2 = (1 << 30) | (dev->portnr << 23) |
			(dev->parent->devnum << 16) | (0 << 8) | (0 << 0);
	qh->qh_overlay.qt_next = QT_NEXT_TERMINATE;
}

/* handle device request function */
static int
ehci_submit_async(struct usb_device *dev, unsigned long pipe, void *buffer,
		   int length, struct devrequest *req)
{
	static char __qh[roundup(sizeof(struct QH), 32)] __attribute__((aligned(32)));
	static struct QH*qh = (struct QH *)__qh;

	static char __qtd[roundup(3*sizeof(struct qTD), 32)] __attribute__((aligned(32)));
	static struct qTD*qtd = (struct qTD *)__qtd;

	int qtd_counter = 0;

	volatile struct qTD *vtd;
	uint32_t *tdp;
	uint32_t token, usbsts;
	uint32_t toggle;
	uint32_t cmd;
	int timeout;
	int ret = 0;

	memset(qtd, 0, 3 * sizeof(*qtd));

	toggle = usb_gettoggle(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));
	setup_qh(dev, pipe, qh);

	tdp = &qh->qh_overlay.qt_next;

	if (!req)
		goto setup_request_qTD1;

	/*
	 * Setup request qTD (3.5 in ehci-r10.pdf)
	 */
	qtd[qtd_counter].qt_next = QT_NEXT_TERMINATE;
	qtd[qtd_counter].qt_altnext = QT_NEXT_TERMINATE;
	token = (0 << 31) | (sizeof(*req) << 16) | (0 << 15) 
		| (2 << 8) | (0 << 12) | (3 << 10) | (0x80 << 0);
	qtd[qtd_counter].qt_token = token;
	if (ehci_td_buffer(&qtd[qtd_counter], req, sizeof(*req)) != 0) {
		printf("SETUP td construct failed.\n");
		goto fail;
	}
	/* Update previous qTD! */
	*tdp = (uint32_t)&qtd[qtd_counter];
	tdp = &qtd[qtd_counter++].qt_next;
	toggle = 1;

	if (length <= 0)
		goto setup_request_qTD2;

setup_request_qTD1:
	/*
	 * Setup request qTD (3.5 in ehci-r10.pdf)
	 */
	qtd[qtd_counter].qt_next = QT_NEXT_TERMINATE;
	qtd[qtd_counter].qt_altnext = QT_NEXT_TERMINATE;
	token = (toggle << 31) | (0 << 12) | (3 << 10) | (0x80 << 0);

	if (length > 0)
		token |= length << 16;

	if (req == NULL)
		token |= 1 << 15;
	if (usb_pipein(pipe))
		token |= 1 << 8;

	qtd[qtd_counter].qt_token = token;
	if (ehci_td_buffer(&qtd[qtd_counter], buffer, length) != 0) {
		printf("DATA td construct failed\n");
		goto fail;
	}
	/* Update previous qTD! */
	*tdp = (uint32_t)&qtd[qtd_counter];
	tdp = &qtd[qtd_counter++].qt_next;

	if (!req)
		goto setup_request_qTD_done;

setup_request_qTD2:
	/*
	 * Setup request qTD (3.5 in ehci-r10.pdf)
	 */
	qtd[qtd_counter].qt_next = QT_NEXT_TERMINATE;
	qtd[qtd_counter].qt_altnext = QT_NEXT_TERMINATE;
	token = (toggle << 31) | (0 << 16) | (1 << 15) 
		| (0 << 12) | (3 << 10) | (0x80 << 0);
	if (!usb_pipein(pipe))
		token |= 1 << 8;
	
	qtd[qtd_counter].qt_token = token;
	/* Update previous qTD! */
	*tdp = (uint32_t)&qtd[qtd_counter];
	tdp = &qtd[qtd_counter++].qt_next;

setup_request_qTD_done:
	qh_list->qh_link = (uint32_t)qh | QH_LINK_TYPE_QH;

	/* Set async. queue head pointer. */
	ehci_writel(&ghcor->or_asynclistaddr, (uint32_t)qh_list);

	usbsts = ehci_readl(&ghcor->or_usbsts);
	ehci_writel(&ghcor->or_usbsts, (usbsts & 0x3f));

	/* Enable async. schedule. */
	cmd = ehci_readl(&ghcor->or_usbcmd);
	cmd |= CMD_ASE;
	ehci_writel(&ghcor->or_usbcmd, cmd);
	ret = handshake((uint32_t *)&ghcor->or_usbsts, STD_ASS, STD_ASS,100 * 1000);
	if (ret < 0) {
		printf("EHCI failed: set STD_ASS timeout\n");
		goto fail;
	}
	
	/* Wait for TDs to be processed. */
	vtd = &qtd[qtd_counter - 1];
	timeout = USB_TIMEOUT_MS(pipe);
	do {

		token = vtd->qt_token;
		if (!(token & 0x80))
			break;
		mdelay(1);
	} while (timeout--);

	/* Check that the TD processing happened */
	if (token & 0x80) {
		printf("EHCI timed out on TD - token=%#x\n", token);
	}

	/* Disable async schedule. */
	cmd = ehci_readl(&ghcor->or_usbcmd);
	cmd &= ~CMD_ASE;
	ehci_writel(&ghcor->or_usbcmd, cmd);
	ret = handshake((uint32_t *)&ghcor->or_usbsts, STD_ASS, 0,
			100 * 1000);
	if (ret < 0) {
		printf("EHCI failed: reset STD_ASS timeout\n");
		goto fail;
	}

	/* handle err/ok result  */
	token = qh->qh_overlay.qt_token;
	if (!(token & 0x80)) {
		uint32_t tmp = token & 0xfc;
		if (tmp == 0) {
			toggle = token >> 31;
			usb_settoggle(dev, usb_pipeendpoint(pipe),
				       usb_pipeout(pipe), toggle);
			dev->status = 0;
		} else if (tmp == 0x40) {
			dev->status = USB_ST_STALLED;
		} else if (tmp == 0xa0 || tmp == 0x20) {
			dev->status = USB_ST_BUF_ERR;
		} else if (tmp == 0x50 || tmp == 0x10) {
			dev->status = USB_ST_BABBLE_DET;
		} else {
			dev->status = USB_ST_CRC_ERR;
			if ((token & 0x40) == 0x40)
				dev->status |= USB_ST_STALLED;
		}
		dev->act_len = length - ((token >> 16) & 0x7fff);
	} else {
		dev->act_len = 0;
	}

	if (dev->status != USB_ST_NOT_PROC)
		return 0;

fail:
	return -1;
}


/* handle root hub request  function */
int
ehci_submit_roothub(struct usb_device *dev, unsigned long pipe, void *buffer,
		 int length, struct devrequest *req)
{
	uint32_t tmpval = 0;
	u16 typeReq;
	void *srcptr = NULL;
	int len, srclen;
	uint32_t reg;
	uint32_t *status_reg;

	if ((req->index - 1) >= CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS) {
		printf("The request port(%d) is out of range\n",req->index - 1);
		return -1;
	}
	status_reg = (uint32_t *)&ghcor->or_portsc[req->index - 1];
	srclen = 0;
	typeReq = req->request | req->requesttype << 8;

	switch (typeReq) {
	case DeviceRequest | USB_REQ_GET_DESCRIPTOR:
		switch (req->value >> 8) {
		case USB_DT_DEVICE:
			srcptr = &root_hub_descriptor.device;
			srclen = 0x12;
			break;
		case USB_DT_CONFIG:
			srcptr = &root_hub_descriptor.config;
			srclen = 0x19;
			break;
		default:
			goto unknown;
		}
		break;
	case USB_REQ_GET_DESCRIPTOR | ((USB_DIR_IN | USB_RT_HUB) << 8):
		switch (req->value >> 8) {
		case USB_DT_HUB:
			reg = ehci_readl(&ghccr->cr_hcsparams);
			root_hub_descriptor.hub.bNbrPorts = HCS_N_PORTS(reg);
			root_hub_descriptor.hub.wHubCharacteristics 
				|= HCS_INDICATOR(reg) ? 0x80 : 0;
			root_hub_descriptor.hub.wHubCharacteristics 
				|= HCS_PPC(reg) ? 0x01 : 0;
			srcptr = &root_hub_descriptor.hub;
			srclen = 0x8;
			break;
		default:
			goto unknown;
		}
		break;
	case USB_REQ_SET_ADDRESS | (USB_RECIP_DEVICE << 8):
		rootdev = req->value;
		break;
	case DeviceOutRequest | USB_REQ_SET_CONFIGURATION:
		/* Nothing to do */
		break;
	case USB_REQ_GET_STATUS | ((USB_DIR_IN | USB_RT_HUB) << 8):
		tmpval = 0x0001; /* USB_STATUS_SELFPOWERED */
		srcptr = &tmpval;
		srclen = 2;
		break;
	case USB_REQ_GET_STATUS | ((USB_RT_PORT | USB_DIR_IN) << 8):
		tmpval = 0;
		tmpval |= (USB_PORT_STAT_HIGH_SPEED >> 8) << 8;

		reg = ehci_readl(status_reg);

		tmpval |= (reg & EHCI_PS_CS) ? USB_PORT_STAT_CONNECTION : 0;
		tmpval |= (reg & EHCI_PS_PE) ? USB_PORT_STAT_ENABLE : 0;
		tmpval |= (reg & EHCI_PS_SUSP) ? USB_PORT_STAT_SUSPEND : 0;
		tmpval |= (reg & EHCI_PS_OCA) ? USB_PORT_STAT_OVERCURRENT : 0;
		tmpval |= (reg & EHCI_PS_PR) ? USB_PORT_STAT_RESET : 0;
		tmpval |= (reg & EHCI_PS_PP) ? ((USB_PORT_STAT_POWER >> 8) << 8) : 0;
		tmpval |= (reg & EHCI_PS_CSC) ? (USB_PORT_STAT_C_CONNECTION << 16) : 0;
		tmpval |= (reg & EHCI_PS_PEC) ? (USB_PORT_STAT_C_ENABLE << 16) : 0;
		tmpval |= (reg & EHCI_PS_OCC) ? (USB_PORT_STAT_C_OVERCURRENT << 16) : 0;
		tmpval |= (portreset & (1 << req->index)) ? (USB_PORT_STAT_C_RESET << 16) : 0;

		srcptr = &tmpval;
		srclen = 4;
		break;
	case USB_REQ_SET_FEATURE | ((USB_DIR_OUT | USB_RT_PORT) << 8):
		tmpval = (uint32_t)req->value;
		reg = ehci_readl(status_reg);
		reg &= ~EHCI_PS_CLEAR;

		if (tmpval == USB_PORT_FEAT_ENABLE) {
			reg |= EHCI_PS_PE;
			ehci_writel(status_reg, reg);
		} else if ((tmpval == USB_PORT_FEAT_POWER) 
			   && HCS_PPC(ehci_readl(&ghccr->cr_hcsparams))) {
			reg |= EHCI_PS_PP;
			ehci_writel(status_reg, reg);
		} else if (tmpval == USB_PORT_FEAT_RESET) {
			if (((reg & (EHCI_PS_PE | EHCI_PS_CS)) == EHCI_PS_CS) &&
			    EHCI_PS_IS_LOWSPEED(reg)) {
				/* Low speed device, give up ownership. */
				reg |= EHCI_PS_PO;
				ehci_writel(status_reg, reg);
			} else {
				int ret;

				reg |= EHCI_PS_PR;
				reg &= ~EHCI_PS_PE;
				ehci_writel(status_reg, reg);
				/*
				 * caller must wait, then call GetPortStatus
				 * usb 2.0 specification say 50 ms resets on
				 * root
				 */
				mdelay(50);
				reg &= ~EHCI_PS_PR;
				ehci_writel(status_reg, reg);
				/*
				 * A host controller must terminate the reset
				 * and stabilize the state of the port within
				 * 2 milliseconds
				 */
				ret = handshake(status_reg, EHCI_PS_PR, 0,
						2 * 1000);
				if (!ret)
					portreset |= 1 << req->index;
				else
					printf("port(%d) reset error\n",
					req->index - 1);
			}
		} else
			goto unknown;
		/* unblock posted writes */
		(void) ehci_readl(&ghcor->or_usbcmd);
		break;
	case USB_REQ_CLEAR_FEATURE | ((USB_DIR_OUT | USB_RT_PORT) << 8):
		reg = ehci_readl(status_reg);
		tmpval = (uint32_t)req->value;
		if (tmpval == USB_PORT_FEAT_ENABLE)
			reg &= ~EHCI_PS_PE;
		else if (tmpval == USB_PORT_FEAT_C_ENABLE)
			reg = (reg & ~EHCI_PS_CLEAR) | EHCI_PS_PE;
		else if ((tmpval == USB_PORT_FEAT_POWER) 
			 && HCS_PPC(ehci_readl(&ghccr->cr_hcsparams)))
			reg = reg & ~(EHCI_PS_CLEAR | EHCI_PS_PP);
		else if (tmpval == USB_PORT_FEAT_C_CONNECTION)
			reg = (reg & ~EHCI_PS_CLEAR) | EHCI_PS_CSC;
		else if (tmpval == USB_PORT_FEAT_OVER_CURRENT)
			reg = (reg & ~EHCI_PS_CLEAR) | EHCI_PS_OCC;
		else if (tmpval == USB_PORT_FEAT_C_RESET)
			portreset &= ~(1 << req->index);
		else 
			goto unknown;

		ehci_writel(status_reg, reg);
		/* unblock posted write */
		(void) ehci_readl(&ghcor->or_usbcmd);
		break;
	default:
		goto unknown;
	}

	mdelay(1);
	len = min3(srclen, req->length, length);
	if (srcptr != NULL && len > 0)
		memcpy(buffer, srcptr, len);

	dev->act_len = len;
	dev->status = 0;
	return 0;

unknown:
	dev->act_len = 0;
	dev->status = USB_ST_STALLED;
	return -1;
}

int usb_lowlevel_init(void)
{
	uint32_t cmd;

	/* step 1: init clock/reset */
	usb_cpu_init();

	/* step 2: init base address */
	ghccr = (struct ehci_hccr *)REG_BASE_EHCI;
	ghcor = (struct ehci_hcor *)((uint32_t) ghccr + HC_LENGTH(ehci_readl(&ghccr->cr_capbase)));

	/* step 2:EHCI spec section 4.1 */
	if (ehci_reset() != 0)
		return -1;


	/* Start the host controller. */
	cmd = ehci_readl(&ghcor->or_usbcmd);
	/*
	 * Philips, Intel, and maybe others need CMD_RUN before the
	 * root hub will detect new devices (why?); NEC doesn't
	 */
	cmd &= ~(CMD_LRESET|CMD_IAAD|CMD_PSE|CMD_ASE|CMD_RESET);
	cmd |= CMD_RUN;
	ehci_writel(&ghcor->or_usbcmd, cmd);

	/* take control over the ports */
	cmd = ehci_readl(&ghcor->or_configflag);
	cmd |= FLAG_CF;
	ehci_writel(&ghcor->or_configflag, cmd);
	/* unblock posted write */
	cmd = ehci_readl(&ghcor->or_usbcmd);
	mdelay(5);

	rootdev = 0;

	return 0;
}

/*********************the middle layer: usb statck and hub************************/

int usb_control_msg(struct usb_device *dev, unsigned int pipe,
			unsigned char request, unsigned char requesttype,
			unsigned short value, unsigned short index,
			void *data, unsigned short size, int timeout)
{
	struct devrequest setup_packet = {
		.requesttype = requesttype,
		.request = request,
		.value = value,
		.index = index,
		.length = size,
	};

	dev->status = USB_ST_NOT_PROC; /*not yet processed */

	if (usb_pipedevice(pipe) == rootdev) {
		if (!rootdev)
			dev->speed = USB_SPEED_HIGH;
		if (ehci_submit_roothub(dev, pipe, data, size, &setup_packet) < 0)
			goto fail;
	} else if (ehci_submit_async(dev, pipe, data, size, &setup_packet) < 0)
		goto fail;

	do {
		if (!((volatile unsigned long)dev->status & USB_ST_NOT_PROC))
			break;
		mdelay(1);
		
	} while (--timeout);

	if (dev->status)
		goto fail;

	return dev->act_len;

fail:
	return -1;
}

/*-------------------------------------------------------------------
 * submits bulk message, and waits for completion. returns 0 if Ok or
 * -1 if Error.
 * synchronous behavior
 */
int usb_bulk_msg(struct usb_device *dev, unsigned int pipe,
			void *data, int len, int *actual_length, int timeout)
{
	dev->status = USB_ST_NOT_PROC; /*not yet processed */

	if((len < 0) || (ehci_submit_async(dev, pipe, data, len, NULL) < 0))
		goto fail;

	do {
		if (!((volatile unsigned long)dev->status & USB_ST_NOT_PROC))
			break;
		mdelay(1);
	} while (--timeout);

	*actual_length = dev->act_len;

	if (dev->status != 0)
		goto fail;

	return 0;
fail:
	return -1;
}

/*
 * set the max packed value of all endpoints in the given configuration
 */
static int usb_set_maxpacket(struct usb_device *dev)
{
	int if_idx, ep_idx;
	int b;
	struct usb_endpoint_descriptor *ep;

	for (if_idx = 0;(if_idx < dev->config.desc.bNumInterfaces)&&(if_idx <USB_MAXINTERFACES); if_idx++)
		for (ep_idx = 0; (ep_idx < dev->config.if_desc[if_idx].desc.bNumEndpoints)
			&&(ep_idx <USB_MAXENDPOINTS); ep_idx++){
			ep = &dev->config.if_desc[if_idx].ep_desc[ep_idx];
			b = ep->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;

			if ((ep->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
						USB_ENDPOINT_XFER_CONTROL) {
				/* Control => bidirectional */
				dev->epmaxpacketout[b] = ep->wMaxPacketSize;
				dev->epmaxpacketin[b] = ep->wMaxPacketSize;
			} else {
				if ((ep->bEndpointAddress & 0x80) == 0) {
					/* OUT Endpoint */
					if (ep->wMaxPacketSize > dev->epmaxpacketout[b]) {
						dev->epmaxpacketout[b] = ep->wMaxPacketSize;
					}
				} else {
					/* IN Endpoint */
					if (ep->wMaxPacketSize > dev->epmaxpacketin[b]) {
					dev->epmaxpacketin[b] = ep->wMaxPacketSize;
					}
				} 
			}
		}

	return 0;
}

/*******************************************************************************
 * Parse the config, located in buffer, and fills the dev->config structure.
 */
static int usb_parse_config(struct usb_device *dev,
			unsigned char *buffer, int cfgno)
{
	struct usb_descriptor_header *head;
	int index, ifno, epno, curr_if_num;

	head = (struct usb_descriptor_header *) &buffer[0];
	if (head->bDescriptorType != USB_DT_CONFIG) {
		return -1;
	}

	ifno = epno = curr_if_num = -1;

	memcpy(&dev->config, buffer, buffer[0]);
	dev->config.no_of_if = 0;
	dev->configno = cfgno;

	index = dev->config.desc.bLength;
	/* Ok the first entry must be a configuration entry,
	 * now process the others */
	for (; index + 1 < dev->config.desc.wTotalLength; index += head->bLength) {
		head = (struct usb_descriptor_header *)&buffer[index];
		if (head->bDescriptorType == USB_DT_INTERFACE) {
			if (((struct usb_interface_descriptor *) \
			     &buffer[index])->bInterfaceNumber == curr_if_num) {
				/* found alternate setting for the interface */
				dev->config.if_desc[ifno].num_altsetting++;
			} else {
				/* this is a new interface, copy new desc */
				ifno = dev->config.no_of_if;
				dev->config.no_of_if++;
				memcpy(&dev->config.if_desc[ifno],
					&buffer[index], buffer[index]);
				dev->config.if_desc[ifno].no_of_ep = 0;
				dev->config.if_desc[ifno].num_altsetting = 1;
				curr_if_num =
				     dev->config.if_desc[ifno].desc.bInterfaceNumber;
			}
		} else if (head->bDescriptorType == USB_DT_ENDPOINT) {
			epno = dev->config.if_desc[ifno].no_of_ep;
			/* found an endpoint */
			dev->config.if_desc[ifno].no_of_ep++;
			memcpy(&dev->config.if_desc[ifno].ep_desc[epno],
				&buffer[index], buffer[index]);
		} else if (head->bLength == 0)
				return 1;

	}

	return 1;
}

/**********************************************************************
 * get_descriptor type
 */
static int usb_get_descriptor(struct usb_device *dev, unsigned char type,
			unsigned char index, void *buf, int size)
{
	return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
			USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
			(type << 8) + index, 0,
			buf, size, USB_CNTL_TIMEOUT);
}

/**********************************************************************
 * gets configuration cfgno and store it in the buffer
 */
int usb_get_configuration_no(struct usb_device *dev,
			     unsigned char *buffer, int cfgno)
{
	int err;
	struct usb_configuration_descriptor *config;

	config = (struct usb_configuration_descriptor *)&buffer[0];
	err = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, buffer, 9);
	if (err < 0) {
		printf("unable to get descriptor, error %lX\n",
			dev->status);
		return -1;
	} else if (err < 9) {
		printf("get descriptor failed, error %lX\n",
			dev->status);
		return -1;
	}

	if ( config->wTotalLength > USB_BUFSIZ) {
		return -1;
	}

	return usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, buffer, config->wTotalLength);
}


/********************************************************************
 * set configuration number to configuration
 */
static int usb_set_configuration(struct usb_device *dev, int configuration)
{
	int res;
	/* set setup command */
	res = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				USB_REQ_SET_CONFIGURATION, 0,
				configuration, 0,
				NULL, 0, USB_CNTL_TIMEOUT);
	if (res == 0) {
		dev->toggle[0] = 0;
		dev->toggle[1] = 0;
		return 0;
	} else
		return -1;
}

/****************************************************************************
 * HUB "Driver"
 * Probes device for being a hub and configurate it
 */

static int usb_clear_port_feature(struct usb_device *dev, int port, int feature)
{
	return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				USB_REQ_CLEAR_FEATURE, USB_RT_PORT, feature,
				port, NULL, 0, USB_CNTL_TIMEOUT);
}

static int usb_get_port_status(struct usb_device *dev, int port, void *data)
{
	return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
			USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_PORT, 0, port,
			data, sizeof(struct usb_hub_status), USB_CNTL_TIMEOUT);
}


static void usb_hub_power_on(struct usb_device * usbdev, unsigned int bPwrOn2PwrGood)
{
	int i;
	struct usb_device *dev;
	unsigned pgood_delay = bPwrOn2PwrGood * 2;

	dev = usbdev;
	/* Enable power to the ports */
	for (i = 0; i < dev->maxchild; i++) {
		usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
				USB_REQ_SET_FEATURE, USB_RT_PORT, USB_PORT_FEAT_POWER,
				i + 1, NULL, 0, USB_CNTL_TIMEOUT);
	}

	/* Wait at least 100 msec for power to become stable */
	mdelay(max(pgood_delay, (unsigned)100));
}

int hub_port_reset(struct usb_device *dev, int port,
			unsigned short *portstat)
{
	int tries;
	struct usb_port_status portsts;

	for (tries = 0; tries < MAX_TRIES; tries++) {

		usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
			USB_REQ_SET_FEATURE, USB_RT_PORT, USB_PORT_FEAT_RESET,
			port + 1, NULL, 0, USB_CNTL_TIMEOUT);
		
		mdelay(200);

		if (usb_get_port_status(dev, port + 1, &portsts) < 0) {
			return -1;
		}

		if ((portsts.wPortChange & USB_PORT_STAT_C_CONNECTION) ||
		    !(portsts.wPortStatus & USB_PORT_STAT_CONNECTION))
			return -1;

		if (portsts.wPortStatus & USB_PORT_STAT_ENABLE)
			break;

		mdelay(200);
	}

	if (tries == MAX_TRIES) {
		return -1;
	}

	usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_C_RESET);
	if (portstat)
		*portstat = portsts.wPortStatus;
	return 0;
}


void usb_hub_port_connect_change(struct usb_device *dev, int port)
{
	struct usb_device *usb;
	struct usb_port_status portsts;

	/* Check status */
	if (usb_get_port_status(dev, port + 1, &portsts) < 0) {
		return;
	}

	/* Clear the connection change status */
	usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_C_CONNECTION);

	/* Disconnect any existing devices under this port */
	if (((!(portsts.wPortStatus & USB_PORT_STAT_CONNECTION)) &&
	     (!(portsts.wPortStatus & USB_PORT_STAT_ENABLE))) || (dev->children[port])) {
		/* Return now if nothing is connected */
		if (!(portsts.wPortStatus & USB_PORT_STAT_CONNECTION))
			return;
	}
	mdelay(200);

	/* Reset the port */
	if (hub_port_reset(dev, port, &portsts.wPortStatus) < 0) {
		printf("cannot reset port %i!?\n", port + 1);
		return;
	}

	mdelay(200);

	/* Allocate a new device struct for it */
	usb = usb_alloc_new_device();

	if (portsts.wPortStatus & USB_PORT_STAT_HIGH_SPEED)
		usb->speed = USB_SPEED_HIGH;
	else if (portsts.wPortStatus & USB_PORT_STAT_LOW_SPEED)
		usb->speed = USB_SPEED_LOW;
	else
		usb->speed = USB_SPEED_FULL;
	dev->children[port] = usb;
	usb->parent = dev;
	usb->portnr = port + 1;
	/* Run it through the hoops (find a driver, etc) */
	if (usb_new_device(usb)) {
		usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_ENABLE);
	}
}


static int usb_hub_configure(struct usb_device *dev)
{
	int i;
	unsigned char buffer[USB_BUFSIZ];
	
	struct usb_hub_descriptor *descriptor;

	/* Get the the hub descriptor */
	if (usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
		USB_REQ_GET_DESCRIPTOR, USB_DIR_IN | USB_RT_HUB,
		USB_DT_HUB << 8, 0, buffer, 4, USB_CNTL_TIMEOUT) < 0) {
		return -1;
	}

	descriptor = (struct usb_hub_descriptor *)buffer;

	/* silence compiler warning if USB_BUFSIZ is > 256 [= sizeof(char)] */
	i = descriptor->bLength;
	if (i > USB_BUFSIZ) {
		return -1;
	}

	if (usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
		USB_REQ_GET_DESCRIPTOR, USB_DIR_IN | USB_RT_HUB,
	USB_DT_HUB << 8, 0, buffer, descriptor->bLength, USB_CNTL_TIMEOUT)  < 0) {
		return -1;
	}

	dev->maxchild = descriptor->bNbrPorts;

	if (sizeof(struct usb_hub_status) > USB_BUFSIZ) {
		return -1;
	}

	if (usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
		USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_HUB, 0, 0,
		buffer, sizeof(struct usb_hub_status), USB_CNTL_TIMEOUT)< 0) {
		
		return -1;
	}

	usb_hub_power_on(dev,descriptor->bPwrOn2PwrGood);
	/* incase for some USB3.0, scan no storage device for the 1st time. */
	mdelay(2000);

	for (i = 0; i < dev->maxchild; i++) {
		struct usb_port_status portsts;

		if (usb_get_port_status(dev, i + 1, &portsts) < 0) {
			continue;
		}

		if (portsts.wPortChange & USB_PORT_STAT_C_CONNECTION) {
			usb_hub_port_connect_change(dev, i);
		}
		if (portsts.wPortChange & USB_PORT_STAT_C_ENABLE) {
			usb_clear_port_feature(dev, i + 1,
						USB_PORT_FEAT_C_ENABLE);

			/* EM interference sometimes causes bad shielded USB
			 * devices to be shutdown by the hub, this hack enables
			 * them again. Works at least with mouse driver */
			if (!(portsts.wPortStatus & USB_PORT_STAT_ENABLE) &&
			     (portsts.wPortStatus & USB_PORT_STAT_CONNECTION) &&
			     ((dev->children[i]))) {
					usb_hub_port_connect_change(dev, i);
			}
		}
		if (portsts.wPortStatus & USB_PORT_STAT_SUSPEND) {
			usb_clear_port_feature(dev, i + 1,
						USB_PORT_FEAT_SUSPEND);
		}

		if (portsts.wPortChange & USB_PORT_STAT_C_OVERCURRENT) {
			usb_clear_port_feature(dev, i + 1,
						USB_PORT_FEAT_C_OVER_CURRENT);
			usb_hub_power_on(dev,descriptor->bPwrOn2PwrGood);
		}

		if (portsts.wPortChange & USB_PORT_STAT_C_RESET) {
			usb_clear_port_feature(dev, i + 1,
						USB_PORT_FEAT_C_RESET);
		}
	} /* end for i all ports */

	return 0;
}


/****************************usb storage *******************************/
static unsigned int usb_get_max_lun(struct us_data *us)
{
	int len;
	unsigned char result;
	len = usb_control_msg(us->pusb_dev,
			      usb_rcvctrlpipe(us->pusb_dev, 0),
			      US_BBB_GET_MAX_LUN,
			      USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_DIR_IN,
			      0, us->ifnum,
			      &result, sizeof(char),
			      USB_CNTL_TIMEOUT * 5);

	return (len > 0) ? result : 0;
}


/***********************************************************************
 * Data transfer routines
 ***********************************************************************/
static int usb_stor_BBB_reset(struct us_data *us)
{
	int result;
	unsigned int pipe;
	int endp;
	int done = 0;

	/*
	 * Reset recovery (5.3.4 in Universal Serial Bus Mass Storage Class)
	 *
	 * For Reset Recovery the host shall issue in the following order:
	 * a) a Bulk-Only Mass Storage Reset
	 * b) a Clear Feature HALT to the Bulk-In endpoint
	 * c) a Clear Feature HALT to the Bulk-Out endpoint
	 *
	 * This is done in 3 steps.
	 *
	 * If the reset doesn't succeed, the device should be port reset.
	 *
	 * This comment stolen from FreeBSD's /sys/dev/usb/umass.c.
	 */
	result = usb_control_msg(us->pusb_dev, usb_sndctrlpipe(us->pusb_dev, 0),
				 US_BBB_RESET,
				 USB_TYPE_CLASS | USB_RECIP_INTERFACE,
				 0, us->ifnum, 0, 0, USB_CNTL_TIMEOUT * 5);

	if ((result < 0) && (us->pusb_dev->status & USB_ST_STALLED)) {
		return -1;
	}

	/* long wait for reset */
	mdelay(150);

	pipe = usb_rcvbulkpipe(us->pusb_dev, us->ep_in);

usb_clear_halt:
	if (done)
		goto success;

	endp = usb_pipeendpoint(pipe)|(usb_pipein(pipe)<<7);

	usb_control_msg(us->pusb_dev, usb_sndctrlpipe(us->pusb_dev, 0),
				 USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0,
				 endp, NULL, 0, USB_CNTL_TIMEOUT * 3);
	/*
	 * NOTE: we do not get status and verify reset was successful
	 * as some devices are reported to lock up upon this check..
	 */
	usb_endpoint_running(us->pusb_dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

	/* toggle is reset on clear */
	usb_settoggle(us->pusb_dev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);

	/* long wait for reset */
	mdelay(150);

	/* long wait for reset */
	pipe = usb_sndbulkpipe(us->pusb_dev, us->ep_out);
	mdelay(150);

	done = 1;

	goto usb_clear_halt;

success:
	return 0;
}

/*
 * Set up the command for a BBB device. Note that the actual SCSI
 * command is copied into cbw.CBWCDB.
 */
int usb_stor_BBB_comdat(ccb *srb, struct us_data *us)
{
	int result;
	int actlen;
	int dir_in;
	unsigned int pipe;
	umass_bbb_cbw_t cbw;
	unsigned char us_direction[32] = {
	0x28, 0x81, 0x14, 0x14, 0x20, 0x01, 0x90, 0x77,
	0x0C, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
#define US_DIRECTION(x) ((us_direction[x>>3] >> (x & 7)) & 1)

	dir_in = US_DIRECTION(srb->cmd[0]);

	/* sanity checks */
	if (!(srb->cmdlen <= CBWCDBLENGTH)) {
		return -1;
	}

	/* always OUT to the ep */
	pipe = usb_sndbulkpipe(us->pusb_dev, us->ep_out);

	cbw.dCBWSignature = CBWSIGNATURE;
	cbw.dCBWTag = CBWTag++;
	cbw.dCBWDataTransferLength = srb->datalen;
	cbw.bCBWFlags = (dir_in ? CBWFLAGS_IN : CBWFLAGS_OUT);
	cbw.bCBWLUN = srb->lun;
	cbw.bCDBLength = srb->cmdlen;
	/* copy the command data into the CBW command data buffer */
	/* DST SRC LEN!!! */
	memcpy(cbw.CBWCDB, srb->cmd, srb->cmdlen);
	result = usb_bulk_msg(us->pusb_dev, pipe, &cbw, UMASS_BBB_CBW_SIZE,
			      &actlen, USB_CNTL_TIMEOUT * 5);
	return result;
}



/* clear a stall on an endpoint - special for BBB devices */
int usb_stor_BBB_clear_endpt_stall(struct us_data *us, u8 endpt)
{
	int result;

	/* ENDPOINT_HALT = 0, so set value to 0 */
	result = usb_control_msg(us->pusb_dev, usb_sndctrlpipe(us->pusb_dev, 0),
				USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT,
				0, endpt, 0, 0, USB_CNTL_TIMEOUT * 5);
	return result;
}

int usb_stor_BBB_transport(ccb *srb, struct us_data *us)
{
	int result, retry;
	int dir_in;
	int actlen, data_actlen;
	unsigned int pipe, pipein, pipeout;
	umass_bbb_csw_t csw;
	unsigned char us_direction[32] = {
	0x28, 0x81, 0x14, 0x14, 0x20, 0x01, 0x90, 0x77,
	0x0C, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
#define US_DIRECTION(x) ((us_direction[x>>3] >> (x & 7)) & 1)
	dir_in = US_DIRECTION(srb->cmd[0]);

	/* COMMAND phase */
	result = usb_stor_BBB_comdat(srb, us);
	if (result < 0) {
		usb_stor_BBB_reset(us);
		return USB_STOR_TRANSPORT_FAILED;
	}
	mdelay(5);
	pipein = usb_rcvbulkpipe(us->pusb_dev, us->ep_in);
	pipeout = usb_sndbulkpipe(us->pusb_dev, us->ep_out);
	/* DATA phase + error handling */
	data_actlen = 0;
	/* no data, go immediately to the STATUS phase */
	if (srb->datalen == 0)
		goto st;

	if (dir_in)
		pipe = pipein;
	else
		pipe = pipeout;
	result = usb_bulk_msg(us->pusb_dev, pipe, srb->pdata, srb->datalen,
			      &data_actlen, USB_CNTL_TIMEOUT * 5);
	/* special handling of STALL in DATA phase */
	if ((result < 0) && (us->pusb_dev->status & USB_ST_STALLED)) {
		/* clear the STALL on the endpoint */
		result = usb_stor_BBB_clear_endpt_stall(us,
					dir_in ? us->ep_in : us->ep_out);
		if (result >= 0)
			/* continue on to STATUS phase */
			goto st;
	}
	if (result < 0) {
		usb_stor_BBB_reset(us);
		return USB_STOR_TRANSPORT_FAILED;
	}
	/* STATUS phase + error handling */
st:
	retry = 0;
again:
	result = usb_bulk_msg(us->pusb_dev, pipein, &csw, UMASS_BBB_CSW_SIZE,
				&actlen, USB_CNTL_TIMEOUT*5);

	/* special handling of STALL in STATUS phase */
	if ((result < 0) && (retry < 1) &&
	    (us->pusb_dev->status & USB_ST_STALLED)) {

		/* clear the STALL on the endpoint */
		result = usb_stor_BBB_clear_endpt_stall(us, us->ep_in);
		if (result >= 0 && (retry++ < 1))
			/* do a retry */
			goto again;
	}
	if (result < 0) {
		usb_stor_BBB_reset(us);
		return USB_STOR_TRANSPORT_FAILED;
	}
	/* misuse pipe to get the residue */
	pipe = csw.dCSWDataResidue;
	if (pipe == 0 && srb->datalen != 0 && srb->datalen - data_actlen != 0)
		pipe = srb->datalen - data_actlen;
	if ((CSWSIGNATURE != csw.dCSWSignature) ||
	((CBWTag - 1) != csw.dCSWTag) ||
	 (csw.bCSWStatus >= CSWSTATUS_PHASE) ){
		usb_stor_BBB_reset(us);
		return USB_STOR_TRANSPORT_FAILED;
	}else if ((data_actlen > srb->datalen) ||
		(csw.bCSWStatus == CSWSTATUS_FAILED) ){
		return USB_STOR_TRANSPORT_FAILED;
	}

	return result;
}


static int usb_inquiry(ccb *srb, struct us_data *ss)
{
	int retry, i;
	retry = 5;
	do {
		memset(&srb->cmd[0], 0, 12);
		srb->cmd[0] = SCSI_INQUIRY;
		srb->cmd[1] = srb->lun << 5;
		srb->cmd[4] = 36;
		srb->datalen = 36;
		srb->cmdlen = 12;
		i = ss->transport(srb, ss);
		if (i == 0)
			break;
	} while (--retry);

	if (!retry) {
		printf("error in inquiry\n");
		return -1;
	}
	return 0;
}

static int usb_request_sense(ccb *srb, struct us_data *ss)
{
	char *ptr;

	ptr = (char *)srb->pdata;
	memset(&srb->cmd[0], 0, 12);
	srb->cmd[0] = SCSI_REQ_SENSE;
	srb->cmd[1] = srb->lun << 5;
	srb->cmd[4] = 18;
	srb->datalen = 18;
	srb->pdata = &srb->sense_buf[0];
	srb->cmdlen = 12;
	ss->transport(srb, ss);
	srb->pdata = (unsigned char *)ptr;
	return 0;
}

static int usb_test_unit_ready(ccb *srb, struct us_data *ss)
{
	int retries = 10;

	do {
		memset(&srb->cmd[0], 0, 12);
		srb->cmd[0] = SCSI_TST_U_RDY;
		srb->cmd[1] = srb->lun << 5;
		srb->datalen = 0;
		srb->cmdlen = 12;
		if (ss->transport(srb, ss) == USB_STOR_TRANSPORT_GOOD)
			return 0;
		usb_request_sense(srb, ss);
		mdelay(100);
	} while (retries--);

	return -1;
}

static int usb_read_capacity(ccb *srb, struct us_data *ss)
{
	int retry;
	/* XXX retries */
	retry = 3;
	do {
		memset(&srb->cmd[0], 0, 12);
		srb->cmd[0] = SCSI_RD_CAPAC;
		srb->cmd[1] = srb->lun << 5;
		srb->datalen = 8;
		srb->cmdlen = 12;
		if (ss->transport(srb, ss) == USB_STOR_TRANSPORT_GOOD)
			return 0;
	} while (retry--);

	return -1;
}

static int usb_read_10(ccb *srb, struct us_data *ss, unsigned long start,
		       unsigned short blocks)
{
	memset(&srb->cmd[0], 0, 12);
	srb->cmd[0] = SCSI_READ10;
	srb->cmd[1] = srb->lun << 5;
	srb->cmd[2] = ((unsigned char) (start >> 24)) & 0xff;
	srb->cmd[3] = ((unsigned char) (start >> 16)) & 0xff;
	srb->cmd[4] = ((unsigned char) (start >> 8)) & 0xff;
	srb->cmd[5] = ((unsigned char) (start)) & 0xff;
	srb->cmd[7] = ((unsigned char) (blocks >> 8)) & 0xff;
	srb->cmd[8] = (unsigned char) blocks & 0xff;
	srb->cmdlen = 12;
	return ss->transport(srb, ss);
}


unsigned long usb_stor_read(int device, unsigned long blknr,
			    unsigned long blkcnt, void *buffer)
{
	unsigned long start, blks, buf_addr, max_xfer_blk;
	unsigned short smallblks;
	struct usb_device *dev;
	struct us_data *ss;
	int retry, i;
	ccb *srb = &usb_ccb;

	if (blkcnt == 0)
		return 0;

	device &= 0xff;
	/* Setup  device */
	dev = NULL;
	for (i = 0; i < USB_MAX_DEVICE; i++) {
		dev = usb_get_dev_index(i);
		if (dev == NULL)
			return 0;
		if (dev->devnum == usb_dev_desc[device].target)
			break;
	}
	ss = (struct us_data *)dev->privptr;

	srb->lun = usb_dev_desc[device].lun;
	buf_addr = (unsigned long)buffer;
	start = blknr;
	blks = blkcnt;
	if (usb_test_unit_ready(srb, ss)) {
		printf("Device NOT ready\n   Request Sense returned %02X %02X"
		       " %02X\n", srb->sense_buf[2], srb->sense_buf[12],
		       srb->sense_buf[13]);
		return 0;
	}


	do {
		/* XXX need some comment here */
		retry = 2;
		srb->pdata = (unsigned char *)buf_addr;
		max_xfer_blk = USB_MAX_XFER_BLK(buf_addr,
						usb_dev_desc[device].blksz);
		if (blks > max_xfer_blk)
			smallblks = (unsigned short) max_xfer_blk;
		else
			smallblks = (unsigned short) blks;
retry_it:
		srb->datalen = usb_dev_desc[device].blksz * smallblks;
		srb->pdata = (unsigned char *)buf_addr;
		if (usb_read_10(srb, ss, start, smallblks)) {
			usb_request_sense(srb, ss);
			if (retry--)
				goto retry_it;
			blkcnt -= blks;
			break;
		}
		start += smallblks;
		blks -= smallblks;
		buf_addr += srb->datalen;
	} while (blks != 0);

	return blkcnt;
}


/* Probe to see if a new device is actually a Storage device */
int usb_storage_probe(struct usb_device *dev, unsigned int ifnum,
		      struct us_data *ss)
{
	struct usb_interface *iface;
	int i;


	/* let's examine the device now */
	iface = &dev->config.if_desc[ifnum];

	if (dev->descriptor.bDeviceClass != 0 ||
			iface->desc.bInterfaceClass != USB_CLASS_MASS_STORAGE ||
			iface->desc.bInterfaceSubClass != US_SC_SCSI ||
			iface->desc.bInterfaceProtocol != US_PR_BULK) {
		return 0;
	}

	memset(ss, 0, sizeof(struct us_data));

	/* At this point, we know we've got a live one */
	/* Initialize the us_data structure with some useful info */
	ss->ifnum = ifnum;
	ss->pusb_dev = dev;
	ss->transport = usb_stor_BBB_transport;
	ss->transport_reset = usb_stor_BBB_reset;
	/*
	 * We are expecting a minimum of 2 endpoints - in and out (bulk).
	 * We will ignore any others.
	 */
	for (i = 0; (i < iface->desc.bNumEndpoints)&&(i <USB_MAXENDPOINTS); i++) {
		/* is it an BULK endpoint? */
		if ((iface->ep_desc[i].bmAttributes &
		     USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK) {
			if (iface->ep_desc[i].bEndpointAddress & USB_DIR_IN)
				ss->ep_in = iface->ep_desc[i].bEndpointAddress &
					USB_ENDPOINT_NUMBER_MASK;
			else
				ss->ep_out = iface->ep_desc[i].bEndpointAddress &
					USB_ENDPOINT_NUMBER_MASK;
		}

	}

	/* Do some basic sanity checks, and bail if we find a problem */
	if ( !ss->ep_in || !ss->ep_out) {
		printf("Do some basic sanity checks\n");
		return 0;
	}

	dev->privptr = (void *)ss;

	return 1;
}

int usb_stor_get_info(struct usb_device *dev, struct us_data *ss,
		      block_dev_desc_t *dev_desc)
{
	unsigned char perq, modi;

	unsigned long cap[2];
	unsigned char usb_stor_buf[36];
	unsigned long *capacity, *blksz;
	ccb *pccb = &usb_ccb;

	pccb->pdata = usb_stor_buf;

	dev_desc->target = dev->devnum;
	pccb->lun = dev_desc->lun;

	if (usb_inquiry(pccb, ss))
		return -1;

	perq = usb_stor_buf[0];
	modi = usb_stor_buf[1];

	if ((perq & 0x1f) == 0x1f) {
		/* skip unknown devices */
		return 0;
	}
	if ((modi&0x80) == 0x80) {
		/* drive is removable */
		dev_desc->removable = 1;
	}

	if (usb_test_unit_ready(pccb, ss)) {
		printf("Device NOT ready\n"
		       "   Request Sense returned %02X %02X %02X\n",
		       pccb->sense_buf[2], pccb->sense_buf[12],
		       pccb->sense_buf[13]);
		if (dev_desc->removable == 1) {
			return 1;
		}
		return 0;
	}

	pccb->pdata = (unsigned char *)&cap[0];
	memset(pccb->pdata, 0, 8);
	if (usb_read_capacity(pccb, ss) != 0) {
		printf("READ_CAP ERROR\n");
		cap[0] = 2880;
		cap[1] = 0x200;
	}
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
	 
	cap[0] = uswap_32(cap[0]);
	cap[1] = uswap_32(cap[1]);
	
	/* this assumes bigendian! */
	cap[0] += 1;
	capacity = &cap[0];
	blksz = &cap[1];

	dev_desc->lba = *capacity;
	dev_desc->blksz = *blksz;

	return 1;
}

static int reset_port(struct usb_device *dev)
{
	int i;
	int port = -1;
	struct usb_device *parent = dev->parent;

	for (i = 0;i < parent->maxchild; i++) {
		if (parent->children[i] == dev) {
			port = i;
			break;
		}
	}
	if (port < 0) {
		printf("usb_new_device:cannot locate device's port.\n");
		return -1;
	}

	/* reset the port for the second time */
	if (hub_port_reset(dev->parent, port, NULL) < 0) {
		printf("\n     Couldn't reset port %i\n", port);
		return -1;
	}

	return 0;
}

/*
 * By the time we get here, the device has gotten a new device ID
 * and is in the default state. We need to identify the thing and
 * get the ball rolling..
 *
 * Returns 0 for success, != 0 for error.
 */
int usb_new_device(struct usb_device *dev)
{
	int addr, err;
	struct usb_interface *iface;

	unsigned char  tmpbuf[USB_BUFSIZ];
	/* We still haven't set the Address yet */
	addr = dev->devnum;
	dev->devnum = 0;

	/* This is a Windows scheme of initialization sequence, with double
	 * reset of the device (Linux uses the same sequence)
	 * Some equipment is said to work only with such init sequence; this
	 * patch is based on the work by Alan Stern:
	 * http://sourceforge.net/mailarchive/forum.php?
	 * thread_id=5729457&forum_id=5398
	 */
	struct usb_device_descriptor *desc;

	/* send 64-byte GET-DEVICE-DESCRIPTOR request.  Since the descriptor is
	 * only 18 bytes long, this will terminate with a short packet.  But if
	 * the maxpacket size is 8 or 16 the device may be waiting to transmit
	 * some more, or keeps on retransmitting the 8 byte header. */

	desc = (struct usb_device_descriptor *)tmpbuf;
	dev->descriptor.bMaxPacketSize0 = 64;	    /* Start off at 64 bytes  */
	/* Default to 64 byte max packet size */
	dev->maxpacketsize = PACKET_SIZE_64;
	dev->epmaxpacketin[0] = 64;
	dev->epmaxpacketout[0] = 64;


	/* in case for some USB3.0 init failed for the first time. */
	mdelay(1000);

	err = usb_get_descriptor(dev, USB_DT_DEVICE, 0, desc, 64);
	if (err < 0) {
		printf("usb_new_device: usb_get_descriptor() failed\n");
		return 1;
	}

	dev->descriptor.bMaxPacketSize0 = desc->bMaxPacketSize0;

	/* find the port number we're at */
	if (dev->parent && reset_port(dev))
		return 1;

	dev->epmaxpacketin[0] = dev->epmaxpacketout[0] = dev->descriptor.bMaxPacketSize0;

	if (dev->descriptor.bMaxPacketSize0 == 8) {
		dev->maxpacketsize  = PACKET_SIZE_8;
	} else if (dev->descriptor.bMaxPacketSize0 == 16) {
		dev->maxpacketsize = PACKET_SIZE_16;
	} else if (dev->descriptor.bMaxPacketSize0 == 32) {
		dev->maxpacketsize = PACKET_SIZE_32;
	} else if (dev->descriptor.bMaxPacketSize0 == 64) {
		dev->maxpacketsize = PACKET_SIZE_64;
	}

	dev->devnum = addr;

	if (usb_control_msg(dev, usb_snddefctrl(dev),
				USB_REQ_SET_ADDRESS, 0,
				(dev->devnum), 0,
				NULL, 0, USB_CNTL_TIMEOUT) < 0) /* set address */
	{
		printf("\n      USB device not accepting new address " \
			"(error=%lX)\n", dev->status);
		return 1;
	}

	mdelay(10);	/* Let the SET_ADDRESS settle */

	err = usb_get_descriptor(dev, USB_DT_DEVICE, 0,
				 tmpbuf, sizeof(dev->descriptor));
	if (err < 0) {
		printf("unable to get device descriptor (error=%d)\n", err);
		return 1;
	} else if (err < sizeof(dev->descriptor)) {
		printf("USB device descriptor short read " \
				"(expected %i, got %i)\n", 
				sizeof(dev->descriptor), err);
		return 1;
	}

	memcpy(&dev->descriptor, tmpbuf, sizeof(dev->descriptor));

	/* only support for one config for now */
	usb_get_configuration_no(dev, tmpbuf, 0);
	usb_parse_config(dev, tmpbuf, 0);
	usb_set_maxpacket(dev);
	/* we set the default configuration here */
	if (usb_set_configuration(dev, dev->config.desc.bConfigurationValue)) {
		return -1;
	}

	/* now prode if the device is a hub */
	iface = &dev->config.if_desc[0];

	if ((iface->desc.bInterfaceClass == USB_CLASS_HUB)
		&&(iface->desc.bNumEndpoints == 1)
		&&((iface->desc.bInterfaceSubClass == 0) ||(iface->desc.bInterfaceSubClass == 1))
		&&(iface->ep_desc[0].bEndpointAddress & USB_DIR_IN)
		&&((iface->ep_desc[0].bmAttributes & 3) == 3)){ 
		usb_hub_configure(dev);
	} else {

		if (usb_storage_probe(dev, 0, &usb_stor[usb_max_devs])) {

		int lun, max_lun, start = usb_max_devs;

		max_lun = usb_get_max_lun(&usb_stor[usb_max_devs]);

		for (lun = 0; lun <= max_lun && usb_max_devs < USB_MAX_STOR_DEV;
			     lun++) {
			usb_dev_desc[usb_max_devs].lun = lun;
			if (usb_stor_get_info(dev, &usb_stor[start], &usb_dev_desc[usb_max_devs]) == 1) {
				usb_max_devs++;
			}
			}

			if (usb_max_devs == USB_MAX_STOR_DEV) {
				printf("max USB Storage Device reached: %d stopping\n",
				usb_max_devs);
			}
	 	}
	}
	return 0;
}

/* returns a pointer to the device with the index [index].
 * if the device is not assigned (dev->devnum==-1) returns NULL
 */
struct usb_device *usb_get_dev_index(int index)
{
	if (usb_dev[index].devnum == -1)
		return NULL;
	else
		return &usb_dev[index];
}


/* returns a pointer of a new device structure or NULL, if
 * no device struct is available
 */
struct usb_device *usb_alloc_new_device(void)
{
	int i;

	if (dev_index == USB_MAX_DEVICE) {
		return NULL;
	}

	printf("New Device %d\n", dev_index);

	/* default Address is 0, real addresses start with 1 */
	usb_dev[dev_index].devnum = dev_index + 1;
	usb_dev[dev_index].maxchild = 0;
	usb_dev[dev_index].parent = NULL;

	for (i = 0; i < USB_MAXCHILDREN; i++)
		usb_dev[dev_index].children[i] = NULL;

	return &usb_dev[++dev_index - 1];
}

/***************************************************************************
 * Init USB Device
 */

int usbhost_init(void)
{
	int result;
	int i;
	struct usb_device * dev;

	dev_index = 0;
	usb_max_devs = 0;
	/* init low_level USB */
	printf("USB:   ");

	/* Set head of reclaim list */
	memset(qh_list, 0, sizeof(struct QH));
	qh_list->qh_link = (uint32_t)qh_list | QH_LINK_TYPE_QH;
	qh_list->qh_endpt1 = (1 << 15) | (USB_SPEED_HIGH << 12);
	qh_list->qh_curtd = QT_NEXT_TERMINATE;
	qh_list->qh_overlay.qt_next = QT_NEXT_TERMINATE;
	qh_list->qh_overlay.qt_altnext = QT_NEXT_TERMINATE;
	qh_list->qh_overlay.qt_token = 0x40;

	result = usb_lowlevel_init();

	if (result == 0) {
		printf("scanning bus for devices... ");

		for (i = 0; i < USB_MAX_STOR_DEV; i++) {
			memset(&usb_dev_desc[i], 0, sizeof(block_dev_desc_t));
			usb_dev_desc[i].dev = i;
			usb_dev_desc[i].target = 0xff;
			usb_dev_desc[i].block_read = usb_stor_read;
		}
		

		/* first make all devices unknown */
		for (i = 0; i < USB_MAX_DEVICE; i++) {
			memset(&usb_dev[i], 0, sizeof(struct usb_device));
			usb_dev[i].devnum = -1;
		}
		
		/* device 0 is always present (root hub) */
		dev = usb_alloc_new_device();

		if (usb_new_device(dev))
			printf("No USB Device found\n");
		else
			printf("%d USB Device(s) found\n", dev_index);

		return 0;
	} else {
		printf("Error, couldn't init Lowlevel part\n");
		return -1;
	}
}

