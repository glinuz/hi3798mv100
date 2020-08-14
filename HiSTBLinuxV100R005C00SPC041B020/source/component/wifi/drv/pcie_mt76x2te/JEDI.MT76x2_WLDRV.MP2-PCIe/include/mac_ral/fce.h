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


#ifndef __FCE_H__
#define __FCE_H__

#include "rt_config.h"

#define FCE_PSE_CTRL	0x0800
#define FCE_PARAMETERS	0x0804
#define FCE_CSO			0x0808
#define FCE_L2_STUFF	0x080c
#define TX_CPU_PORT_FROM_FCE_BASE_PTR		0x09A0
#define TX_CPU_PORT_FROM_FCE_MAX_COUNT		0x09A4
#define FCE_PDMA_GLOBAL_CONF				0x09C4
#define TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX 0x09A8
#define FCE_SKIP_FS							0x0A6C
#define PER_PORT_PAUSE_ENABLE_CONTROL1		0x0A38

#ifdef RT_BIG_ENDIAN
typedef union _L2_STUFFING_STRUC {
	struct {
		UINT32 RSV:6;
		UINT32 OTHER_PORT:2;
		UINT32 TS_LENGTH_EN:8;
		UINT32 TS_CMD_QSEL_EN:8;
		UINT32 RSV2:2;
		UINT32 MVINF_BYTE_SWP:1;
		UINT32 FS_WR_MPDU_LEN_EN:1;
		UINT32 TX_L2_DE_STUFFING_EN:1;
		UINT32 RX_L2_STUFFING_EN:1;
		UINT32 QoS_L2_EN:1;
		UINT32 HT_L2_EN:1;
	} field;

	UINT32 word;
} L2_STUFFING_STRUC, *PL2_STUFFING_STRUC;
#else
typedef union _L2_STUFFING_STRUC {
	struct {
		UINT32 HT_L2_EN:1;
		UINT32 QoS_L2_EN:1;
		UINT32 RX_L2_STUFFING_EN:1;
		UINT32 TX_L2_DE_STUFFING_EN:1;
		UINT32 FS_WR_MPDU_LEN_EN:1;
		UINT32 MVINF_BYTE_SWP:1;
		UINT32 RSV2:2;
		UINT32 TS_CMD_QSEL_EN:8;
		UINT32 TS_LENGTH_EN:8;
		UINT32 OTHER_PORT:2;
		UINT32 RSV:6;
	} field;

	UINT32 word;
} L2_STUFFING_STRUC, *PL2_STUFFING_STRUC;
#endif /* endif */

#define NORMAL_PKT				0x0
#define CMD_PKT					0x1

#define FCE_WLAN_PORT			0x0
#define FCE_CPU_RX_PORT			0x1
#define FCE_CPU_TX_PORT			0x2
#define FCE_HOST_PORT			0x3
#define FCE_VIRTUAL_CPU_RX_PORT	0x4
#define FCE_VIRTUAL_CPU_TX_PORT	0x5
#define FCE_DISCARD				0x6
#endif /*__FCE_H__ */
