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


/****************************************************************************

	Abstract:
	Management Frame Protection Required is defined in IEEE802.11w

	Define all structures, data types that rtmp.h needed in this file. Don't
	put any sturctures and functions definition which refer to RTMP_ADAPTER
	Here.

***************************************************************************/

#ifdef DOT11W_PMF_SUPPORT

#ifndef __PMF_CMM_H__
#define __PMF_CMM_H__

#include "rtmp_type.h"
#include "dot11w_pmf.h"

#define	NORMAL_FRAME                    0
#define	ERROR_FRAME                     1
#define	NOT_ROBUST_GROUP_FRAME		2
#define	NOT_ROBUST_UNICAST_FRAME	3
#define	UNICAST_ROBUST_FRAME		4
#define	GROUP_ROBUST_FRAME		5

typedef enum _PMF_STATUS_NUM {
	PMF_STATUS_SUCCESS,
	PMF_POLICY_VIOLATION,
	PMF_UNICAST_ENCRYPT_FAILURE,
	PMF_ENCAP_BIP_FAILURE,
	PMF_UNICAST_DECRYPT_FAILURE,
	PMF_EXTRACT_BIP_FAILURE,
	PMF_STATUS_RESV
} PMF_STATUS_NUM;

/*
 * Management Frame Protection Required is defined in IEEE802.11w
 */
typedef struct GNU_PACKED _PMF_CFG {
	/*
	 * A STA sets this bit to 1 to advertise that protection of Robust
	 * Management Frames is enabled.
	 */
	BOOLEAN MFPC;		/* This is actual active */
	BOOLEAN Desired_MFPC;	/* This is user desired */

	/*
	 * A STA sets this bit to 1 to advertise that protection of
	 * Robust Management Frames is mandatory.
	 * If a STA sets this bit to 1, then that STA only allows RSNAs
	 * from STAs which provide Management Frame Protection.
	 */
	BOOLEAN MFPR;		/* This is actual active */
	BOOLEAN Desired_MFPR;	/* This is user desired */

	BOOLEAN PMFSHA256;	/* This is actual active */
	BOOLEAN Desired_PMFSHA256;	/* This is user desired */

	UINT8 IGTK_KeyIdx;	/* It shall be 4 or 5 */
	UCHAR IGTK[2][LEN_AES_GTK];
	UCHAR IPN[2][LEN_WPA_TSC];
} PMF_CFG, *PPMF_CFG;

#endif /* __PMF_CMM_H__ */
#endif /* DOT11W_PMF_SUPPORT */
