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


#ifndef __RAL_NMAC_PBF_H__
#define __RAL_NMAC_PBF_H__

/* ================================================================ */
/* Register format  for PBF                                                                                     */
/* ================================================================ */

/* Most are for debug. Driver doesn't touch PBF register. */
#ifdef RT_BIG_ENDIAN
typedef union _RLT_PBF_SYS_CTRL_STRUC {
	struct {
		UINT32 rsv5:7;	/* Reserved */
		UINT32 CSR_TEST_EN:1;
		UINT32 MAC_CLKSEL:2;	/* MAC clock selection */
		UINT32 PWRSV_EN:2;
		UINT32 SHRM_SEL:1;	/* Shared memory access selection */
		UINT32 PBF_MSEL:2;	/* Packet buffer memory access selection */
		UINT32 rsv4:5;
		UINT32 PBF_CLK_EN:1;	/* PBF clock enable */
		UINT32 MAC_CLK_EN:1;	/* MAC clock enable */
		UINT32 rsv3:6;
		UINT32 PBF_RESET:1;	/* PBF hardware reset */
		UINT32 MAC_RESET:1;	/* MAC hardware reset */
		UINT32 rsv:2;
	} field;
	UINT32 word;
} RLT_PBF_SYS_CTRL_STRUC;
#else
typedef union _RLT_PBF_SYS_CTRL_STRUC {
	struct {
		UINT32 rsv5:7;	/* Reserved */
		UINT32 CSR_TEST_EN:1;
		UINT32 MAC_CLKSEL:2;	/* MAC clock selection */
		UINT32 PWRSV_EN:2;
		UINT32 SHRM_SEL:1;	/* Shared memory access selection */
		UINT32 PBF_MSEL:2;	/* Packet buffer memory access selection */
		UINT32 rsv4:5;
		UINT32 PBF_CLK_EN:1;	/* PBF clock enable */
		UINT32 MAC_CLK_EN:1;	/* MAC clock enable */
		UINT32 rsv3:6;
		UINT32 PBF_RESET:1;	/* PBF hardware reset */
		UINT32 MAC_RESET:1;	/* MAC hardware reset */
		UINT32 rsv:2;
	} field;
	UINT32 word;
} RLT_PBF_SYS_CTRL_STRUC;
#endif /* endif */

#define RLT_PBF_CFG			0x0404
#define RX_DROP_MODE		(1 << 8)
#define TX_MAX_PCNT			0x0408
#define RX_MAX_PCNT			0x040c
#define RXQ_STA				0x0430
#define TXQ_STA				0x0434

#define RLT_BCN_OFFSET0		0x041c
#define RLT_BCN_OFFSET1		0x0420
#define RLT_BCN_OFFSET2		0x0424
#define RLT_BCN_OFFSET3		0x0428

#define FCE_CTRL			0x0800
#define FCE_PARAM		0x0804
#define CHECKSUM_OFFLOAD	0x0808

#endif /* __RAL_NMAC_PBF_H__ */
