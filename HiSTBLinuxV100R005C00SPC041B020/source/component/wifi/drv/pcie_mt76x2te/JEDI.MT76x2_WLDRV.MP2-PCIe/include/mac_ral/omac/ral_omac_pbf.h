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


#ifndef __RAL_OMAC_PBF_H__
#define __RAL_OMAC_PBF_H__

#include "rtmp_type.h"

/* ============================================================== */
/* Register format  for PBF                                                                                 */
/* ============================================================== */

#ifdef RT_BIG_ENDIAN
typedef union _RTMP_PBF_SYS_CTRL_STRUC {
	struct {
		UINT32 Reserved5:12;	/* Reserved */
		UINT32 SHR_MSEL:1;	/* Shared memory access selection */
		UINT32 PBF_MSEL:2;	/* Packet buffer memory access selection */
		UINT32 HST_PM_SEL:1;	/* The write selection of the host program RAM */
		UINT32 Reserved4:1;	/* Reserved */
		UINT32 CAP_MODE:1;	/* Packet buffer capture mode */
		UINT32 Reserved3:1;	/* Reserved */
		UINT32 CLK_SEL:1;	/* MAC/PBF clock source selection */
		UINT32 PBF_CLK_EN:1;	/* PBF clock enable */
		UINT32 MAC_CLK_EN:1;	/* MAC clock enable */
		UINT32 DMA_CLK_EN:1;	/* DMA clock enable */
		UINT32 Reserved2:1;	/* Reserved */
		UINT32 MCU_READY:1;	/* MCU ready */
		UINT32 Reserved1:2;	/* Reserved */
		UINT32 ASY_RESET:1;	/* ASYNC interface reset */
		UINT32 PBF_RESET:1;	/* PBF hardware reset */
		UINT32 MAC_RESET:1;	/* MAC hardware reset */
		UINT32 DMA_RESET:1;	/* DMA hardware reset */
		UINT32 MCU_RESET:1;	/* MCU hardware reset */
	} field;
	UINT32 word;
} RTMP_PBF_SYS_CTRL_STRUC;
#else
typedef union _RTMP_PBF_SYS_CTRL_STRUC {
	struct {
		UINT32 MCU_RESET:1;
		UINT32 DMA_RESET:1;
		UINT32 MAC_RESET:1;
		UINT32 PBF_RESET:1;
		UINT32 ASY_RESET:1;
		UINT32 Reserved1:2;
		UINT32 MCU_READY:1;
		UINT32 Reserved2:1;
		UINT32 DMA_CLK_EN:1;
		UINT32 MAC_CLK_EN:1;
		UINT32 PBF_CLK_EN:1;
		UINT32 CLK_SEL:1;
		UINT32 Reserved3:1;
		UINT32 CAP_MODE:1;
		UINT32 Reserved4:1;
		UINT32 HST_PM_SEL:1;
		UINT32 PBF_MSEL:2;
		UINT32 SHR_MSEL:1;
		UINT32 Reserved5:12;
	} field;
	UINT32 word;
} RTMP_PBF_SYS_CTRL_STRUC;
#endif /* endif */

#define PBF_CFG			0x0408
#define PBF_MAX_PCNT	0x040C
#define PBF_CAP_CTRL	0x0440

#define BCN_OFFSET0		0x042C
#define BCN_OFFSET1		0x0430
#define TXRXQ_STA		0x0434

#endif /* __RAL_OMAC_PBF_H__ */
