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


#ifndef __CHIP_ID_H__
#define __CHIP_ID_H__

#define NIC_PCI_VENDOR_ID		0x1814
#define MTK_PCI_VENDOR_ID		0x14C3

#define NIC2860_PCI_DEVICE_ID	0x0601
#define NIC2860_PCIe_DEVICE_ID	0x0681
#define NIC2760_PCI_DEVICE_ID	0x0701	/* 1T/2R Cardbus ??? */
#define NIC2790_PCIe_DEVICE_ID  0x0781	/* 1T/2R miniCard */

#define VEN_AWT_PCIe_DEVICE_ID	0x1059
#define DLINK_PCI_VENDOR_ID    0x1186
#define VEN_AWT_PCI_VENDOR_ID	0x1A3B

#define EDIMAX_PCI_VENDOR_ID	0x1432

#define NIC3090_PCIe_DEVICE_ID  0x3090	/* 1T/1R miniCard */
#define NIC3091_PCIe_DEVICE_ID  0x3091	/* 1T/2R miniCard */
#define NIC3092_PCIe_DEVICE_ID  0x3092	/* 2T/2R miniCard */
#define NIC3390_PCIe_DEVICE_ID  0x3390	/* 1T/1R miniCard */

#define NIC3062_PCI_DEVICE_ID	0x3062	/* 2T/2R miniCard */
#define NIC3562_PCI_DEVICE_ID	0x3562	/* 2T/2R miniCard */
#define NIC3060_PCI_DEVICE_ID	0x3060	/* 1T/1R miniCard */

#define NIC3592_PCIe_DEVICE_ID	0x3592	/* 2T/2R miniCard */

#define NIC3593_PCI_OR_PCIe_DEVICE_ID	0x3593

#define NIC5390_PCIe_DEVICE_ID	0x5390
#define NIC539F_PCIe_DEVICE_ID	0x539F
#define NIC5392_PCIe_DEVICE_ID	0x5392
#define NIC5360_PCI_DEVICE_ID	0x5360
#define NIC5362_PCI_DEVICE_ID	0x5362

#define NIC5592_PCIe_DEVICE_ID  0x5592

#define NIC3290_PCIe_DEVICE_ID 0x3290

#define NIC6590_PCIe_DEVICE_ID 0x6590
#define NIC7610_PCIe_DEVICE_ID 0x7610
#define NIC7630_PCIe_DEVICE_ID 0x7630
#define NIC7650_PCIe_DEVICE_ID 0x7650
#define NIC7602_PCIe_DEVICE_ID 0x7602
#define NIC7612_PCIe_DEVICE_ID 0x7612
#define NIC7632_PCIe_DEVICE_ID 0x7632
#define NIC7662_PCIe_DEVICE_ID 0x7662

#ifdef CONFIG_FPGA_MODE
#define NIC6390_PCIe_DEVICE_ID 0x6390
#endif /* CONFIG_FPGA_MODE */

#define NIC7601_PCIe_DEVICE_ID 0x7601

#define NIC8592_PCIe_DEVICE_ID	0x8592
#endif /* __CHIP_ID_H__ */
