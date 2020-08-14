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


#ifndef __DOT11W_PMF_H
#define __DOT11W_PMF_H

#ifdef DOT11W_PMF_SUPPORT

#include "rtmp_type.h"

#define LEN_PMF_MMIE			16
#define PMF_CIPHER_SUITE_LEN	4
#define LEN_PMF_BIP_AAD_HDR		20
#define LEN_PMF_BIP_MIC			8
#define LEN_PMF_IGTK_KDE		24

/* PMF Action filed value */
#define PMF_ACTION_QA_QUERY_REQ		0
#define PMF_ACTION_QA_QUERY_RESP	1

/* Information element ID defined in 802.11w specification. */
#define IE_PMF_MMIE				76

/* The definition in IEEE 802.11w - Table 7-32 Cipher suite selectors */
extern UCHAR OUI_PMF_BIP_CIPHER[];

/* The definition in IEEE 802.11w - Table 7-34 AKM suite selectors */
extern UCHAR OUI_PMF_8021X_AKM[];
extern UCHAR OUI_PMF_PSK_AKM[];

/* The Key ID filed */
typedef union GNU_PACKED _PMF_IGTK_KEY_ID {
	struct GNU_PACKED {
#ifdef RT_BIG_ENDIAN
		UINT16:4;
		UINT16 KeyId:12;
#else
		UINT16 KeyId:12;
		 UINT16:4;
#endif				/* RT_BIG_ENDIAN */
	} field;
	UINT16 word;
} PMF_IGTK_KEY_ID, *PPMF_IGTK_KEY_ID;

/* The SubIE of Fast BSS transition information element */
typedef struct GNU_PACKED _FT_IGTK_SUB_ELEMENT {
	UINT8 KeyID[2];		/* indicates the value of the BIP key ID */
	UINT8 IPN[6];		/* indicates the receive sequence counter for the IGTK being installed */
	UINT8 KeyLen;		/* the length of IGTK in octets */
	UINT8 Key[24];		/* The length of the resulting AES-Keywrapped IGTK in the Key field */
} FT_IGTK_SUB_ELEMENT, *PFT_IGTK_SUB_ELEMENT;

/* Management MIC information element */
typedef struct GNU_PACKED _PMF_MMIE {
	UINT8 KeyID[2];		/* identifies the IGTK used to compute the MIC */
	UINT8 IPN[6];		/* indicates the receive sequence counter for the IGTK being installed */
	UINT8 MIC[LEN_PMF_BIP_MIC];	/* The length of the resulting AES-Keywrapped IGTK in the Key field */
} PMF_MMIE, *PPMF_MMIE;

/* IGTK KDE format */
typedef struct GNU_PACKED _PMF_IGTK_KDE {
	UINT8 KeyID[2];		/* identifies the IGTK used to compute the MIC */
	UINT8 IPN[6];		/* indicates the receive sequence counter for the IGTK being installed */
	UINT8 IGTK[0];		/* The length of the IGTK */
} PMF_IGTK_KDE, *PPMF_IGTK_KDE;

/* =====================
 *	PMF SA Query Action
 * ===================== */
typedef struct GNU_PACKED _PMF_SA_QUERY_ACTION {
	UCHAR Category;
	UCHAR Action;

	/*  a 16-bit non-negative counter value */
	UINT8 TransactionID[2];
} PMF_SA_QUERY_ACTION, *PPMF_SA_QUERY_ACTION;

#endif /* DOT11W_PMF_SUPPORT */

#endif /* __DOT11W_PMF_H */
