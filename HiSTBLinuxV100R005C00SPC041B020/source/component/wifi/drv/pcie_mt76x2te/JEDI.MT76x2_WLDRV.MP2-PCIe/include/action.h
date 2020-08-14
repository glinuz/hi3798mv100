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


#ifndef	__ACTION_H__
#define	__ACTION_H__

struct _RTMP_ADAPTER;

VOID MlmeQOSAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID MlmeDLSAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID MlmeInvalidAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerRMAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerQOSAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerAddBAReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerAddBARspAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerDelBAAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerBAAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PeerHTAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

#ifdef DOT11_VHT_AC
VOID PeerVHTAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);
#endif /* DOT11_VHT_AC */

VOID PeerPublicAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

#ifdef CONFIG_STA_SUPPORT
VOID StaPublicAction(IN PRTMP_ADAPTER pAd, IN BSS_2040_COEXIST_IE * pBss2040CoexIE);
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
VOID ApPublicAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);
#endif /* CONFIG_AP_SUPPORT */

#ifdef QOS_DLS_SUPPORT
VOID PeerDLSAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);
#endif /* QOS_DLS_SUPPORT */

#endif /* __ACTION_H__ */
