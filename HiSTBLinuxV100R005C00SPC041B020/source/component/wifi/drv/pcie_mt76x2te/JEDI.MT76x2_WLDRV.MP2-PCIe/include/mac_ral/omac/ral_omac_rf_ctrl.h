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


#ifndef __RAL_OMAC_RF_CTRL_H__
#define __RAL_OMAC_RF_CTRL_H__

/* ================================================================================= */
/* Register format  for RFCTRL                                                       */
/* ================================================================================= */

#define OSC_CTRL		0x5a4
#define PCIE_PHY_TX_ATTENUATION_CTRL		0x05C8
#define INTERNAL_1		0x05C8

#ifdef RT_BIG_ENDIAN
typedef union _INTERNAL_1_STRUCT {
	struct {
		UINT32 Reserve1:10;
		UINT32 CSO_RX_IPV6_CHKSUM_EN:1;
		UINT32 CSO_TX_IPV6_CHKSUM_EN:1;
		UINT32 CSO_HW_PARSE_TCP:1;
		UINT32 CSO_HW_PARSE_IP:1;
		UINT32 CSO_RX_CHKSUM_EN:1;
		UINT32 CSO_TX_CHKSUM_EN:1;
		UINT32 CSO_TIMEOUT_VALUE:4;
		UINT32 PCIE_PHY_TX_ATTEN_EN:1;
		UINT32 PCIE_PHY_TX_ATTEN_VALUE:3;
		UINT32 Reserve2:7;
		UINT32 RF_ISOLATION_ENABLE:1;
	} field;

	UINT32 word;
} INTERNAL_1_STRUCT;
#else
typedef union _INTERNAL_1_STRUCT {
	struct {
		UINT32 RF_ISOLATION_ENABLE:1;
		UINT32 Reserve2:7;
		UINT32 PCIE_PHY_TX_ATTEN_VALUE:3;
		UINT32 PCIE_PHY_TX_ATTEN_EN:1;
		UINT32 CSO_TIMEOUT_VALUE:4;
		UINT32 CSO_TX_CHKSUM_EN:1;
		UINT32 CSO_RX_CHKSUM_EN:1;
		UINT32 CSO_HW_PARSE_IP:1;
		UINT32 CSO_HW_PARSE_TCP:1;
		UINT32 CSO_TX_IPV6_CHKSUM_EN:1;
		UINT32 CSO_RX_IPV6_CHKSUM_EN:1;
		UINT32 Reserve1:10;
	} field;

	UINT32 word;
} INTERNAL_1_STRUCT;
#endif /* endif */

#define RF_DBG1					0x050C
#define RF_CONTROL0				0x0518
#define RTMP_RF_BYPASS0				0x051C
#define RF_CONTROL1				0x0520
#define RF_BYPASS1				0x0524
#define RF_CONTROL2				0x0528
#define RF_BYPASS2				0x052C
#define RF_CONTROL3				0x0530
#define RF_BYPASS3				0x0534

#define LDO_CFG0				0x05d4
#define GPIO_SWITCH				0x05dc

#define DEBUG_INDEX				0x05e8

#endif /* __RAL_OMAC_RF_CTRL_H__ */
