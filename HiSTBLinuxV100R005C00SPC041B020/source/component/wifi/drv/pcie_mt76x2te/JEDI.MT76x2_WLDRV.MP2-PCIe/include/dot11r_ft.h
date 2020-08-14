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


#ifndef __DOT11R_FT_H
#define __DOT11R_FT_H

#include "rtmp_type.h"


#if defined(DOT11R_FT_SUPPORT) || defined(DOT11Z_TDLS_SUPPORT)
#define FT_MIC_LEN					16
#define FT_NONCE_LEN				32
#endif /* endif */


#if defined(DOT11R_FT_SUPPORT) || defined(DOT11Z_TDLS_SUPPORT)
/* Information element ID defined in 802.11rD9.0 specification. */
#define IE_FT_MDIE				54
#define IE_FT_FTIE				55
#define IE_FT_TIMEOUT_INTERVAL	56
#define IE_FT_RIC_DATA			57
#define IE_FT_RIC_DESCRIPTOR	75

/* RIC Type */
#define FT_RIC_TYPE_BA			1

/* AKM SUITE */
#define FT_AKM_SUITE_1X		3
#define FT_AKM_SUITE_PSK	4
#endif /* endif */


#if defined(DOT11R_FT_SUPPORT) || defined(DOT11Z_TDLS_SUPPORT)
typedef union GNU_PACKED _FT_MIC_CTR_FIELD {
	/*
	   IECnt: contains the number of IEs
	   that are included int eht MIC calculation.
	 */
	struct GNU_PACKED {
#ifdef RT_BIG_ENDIAN
		UINT16 IECnt:8;
		 UINT16:8;
#else
		 UINT16:8;
		UINT16 IECnt:8;
#endif				/* endif */
	} field;
	UINT16 word;
} FT_MIC_CTR_FIELD, *PFT_MIC_CTR_FIELD;

/*
** FTIE: Fast Transition IE.
*/
typedef struct GNU_PACKED _FT_FTIE {
	FT_MIC_CTR_FIELD MICCtr;	/* 2 Octects. */
	UINT8 MIC[FT_MIC_LEN];	/* 16 Octects. */
	UINT8 ANonce[FT_NONCE_LEN];	/* 32 Octects. */
	UINT8 SNonce[FT_NONCE_LEN];	/* 32 Octects. */
	UINT8 Option[0];	/* 1:R1KHID, 2:GTK, 3:ROKHId, else:Res */
} FT_FTIE, *PFT_FTIE;
#endif /* endif */


#if defined(DOT11R_FT_SUPPORT) || defined(DOT11Z_TDLS_SUPPORT)
/*
** Timeout Interval IE.
*/
typedef enum _FT_TIMEOUT_INTERVAL_TYPE {
	REASSOC_DEADLINE_INTERVAL = 1,	/* TUs */
	KEY_LIFETIME_INTERVAL,	/* seconds. */
#ifdef DOT11W_PMF_SUPPORT
	ASSOC_COMEBACK_TIME,	/* TUs */
#endif /* DOT11W_PMF_SUPPORT */
	RESERVED_INTERVAL
} FT_TIMEOUT_INTERVAL_TYPE, *PFT_TIMEOUT_INTERVAL_TYPE;

typedef struct GNU_PACKED _FT_TIMEOUT_INTERVAL_IE {
	UINT8 TimeoutIntervalType;
	UINT32 TimeoutIntervalValue;
} FT_TIMEOUT_INTERVAL_IE, *PFT_TIMEOUT_INTERVAL_IE;
#endif /* endif */


#endif /* __DOT11R_FT_H */
