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


#ifndef __MT65XX_H__
#define __MT65XX_H__


#ifdef MT76x2
#include "mt76x2.h"
#endif /* MT76x2 */

struct _RTMP_ADAPTER;

/* b'00: 2.4G+5G external PA, b'01: 5G external PA, b'10: 2.4G external PA, b'11: Internal PA */
#define EXT_PA_2G_5G		0x0
#define EXT_PA_5G_ONLY		0x1
#define EXT_PA_2G_ONLY		0x2

#define INT_PA_2G_5G		0x3
#define INT_PA_5G			0x2
#define INT_PA_2G			0x1

#define MAX_CHECK_COUNT 200

#ifdef RTMP_USB_SUPPORT
BOOLEAN RT65xxUsbAsicRadioOn(struct _RTMP_ADAPTER *pAd, UCHAR Stage);
BOOLEAN RT65xxUsbAsicRadioOff(struct _RTMP_ADAPTER *pAd, UCHAR Stage);
#endif /* endif */

/*
	EEPROM format
*/

#ifdef RT_BIG_ENDIAN
typedef union _EEPROM_NIC_CINFIG0_STRUC {
	struct {
#ifdef MT76x2
		USHORT Rsv:5;
		USHORT PACurrent:1;
#else
		USHORT Rsv:6;
#endif				/* endif */
		USHORT PAType:2;	/* 00: 2.4G+5G external PA, 01: 5G external PA,
					10: 2.4G external PA, 11: Internal PA */
		USHORT TxPath:4;	/* 1: 1T, 2: 2T, 3: 3T */
		USHORT RxPath:4;	/* 1: 1R, 2: 2R, 3: 3R */
	} field;
	USHORT word;
} EEPROM_NIC_CONFIG0_STRUC, *PEEPROM_NIC_CONFIG0_STRUC;
#else
typedef union _EEPROM_NIC_CINFIG0_STRUC {
	struct {
		USHORT RxPath:4;	/* 1: 1R, 2: 2R, 3: 3R */
		USHORT TxPath:4;	/* 1: 1T, 2: 2T, 3: 3T */
		USHORT PAType:2;	/* 00: 2.4G+5G external PA, 01: 5G external PA,
					10: 2.4G external PA, 11: Internal PA */
#ifdef MT76x2
		USHORT PACurrent:1;
		USHORT Rsv:5;
#else
		USHORT Rsv:6;
#endif				/* MT76x2 */
	} field;
	USHORT word;
} EEPROM_NIC_CONFIG0_STRUC, *PEEPROM_NIC_CONFIG0_STRUC;
#endif /* RT_BIG_ENDIAN */

VOID RT65xxDisableTxRx(struct _RTMP_ADAPTER *pAd, UCHAR Level);
void MT76xx_PciMlmeRadioOFF(struct _RTMP_ADAPTER *pAd);
void MT76xx_PciMlmeRadioOn(struct _RTMP_ADAPTER *pAd);
VOID dump_bw_info(struct _RTMP_ADAPTER *pAd);
VOID dump_pwr_info(struct _RTMP_ADAPTER *pAd);

#endif /* __MT65XX_H__ */
