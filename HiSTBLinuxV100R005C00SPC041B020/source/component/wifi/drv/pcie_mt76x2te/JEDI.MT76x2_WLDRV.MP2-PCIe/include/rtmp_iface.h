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


#ifndef __RTMP_IFACE_H__
#define __RTMP_IFACE_H__



#ifdef RTMP_USB_SUPPORT
#include "iface/rtmp_usb.h"
#endif /* RTMP_USB_SUPPORT */

typedef struct _INF_PCI_CONFIG_ {
	unsigned long CSRBaseAddress;	/* PCI MMIO Base Address, all access will use */
	unsigned int irq_num;
} INF_PCI_CONFIG;

typedef struct _INF_USB_CONFIG_ {
	unsigned char BulkInEpAddr;	/* bulk-in endpoint address */
	unsigned char BulkOutEpAddr[6];	/* bulk-out endpoint address */
} INF_USB_CONFIG;

typedef struct _INF_RBUS_CONFIG_ {
	unsigned long csr_addr;
	unsigned int irq;
} INF_RBUS_CONFIG;

typedef union _RTMP_INF_CONFIG_ {
	struct _INF_PCI_CONFIG_ pciConfig;
	struct _INF_USB_CONFIG_ usbConfig;
	struct _INF_RBUS_CONFIG_ rbusConfig;
} RTMP_INF_CONFIG;

#endif /* __RTMP_IFACE_H__ */
