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

#ifndef __PMF_H
#define __PMF_H
#ifdef DOT11W_PMF_SUPPORT

VOID PMF_PeerAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PMF_MlmeSAQueryReq(IN PRTMP_ADAPTER pAd, IN MAC_TABLE_ENTRY * pEntry);

VOID PMF_PeerSAQueryReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PMF_PeerSAQueryRspAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID PMF_DerivePTK(IN PRTMP_ADAPTER pAd,
		   IN UCHAR * PMK,
		   IN UCHAR * ANonce,
		   IN UCHAR * AA,
		   IN UCHAR * SNonce, IN UCHAR * SA, OUT UCHAR * output, IN UINT len);

VOID PMF_DeriveIGTK(IN PRTMP_ADAPTER pAd, OUT UCHAR * output);

VOID PMF_InsertIGTKKDE(IN PRTMP_ADAPTER pAd,
		       IN INT apidx, IN PUCHAR pFrameBuf, OUT PULONG pFrameLen);

BOOLEAN PMF_ExtractIGTKKDE(IN PRTMP_ADAPTER pAd, IN PUCHAR pBuf, IN INT buf_len);

VOID PMF_MakeRsnIeGMgmtCipher(IN PRTMP_ADAPTER pAd,
			      IN UCHAR ElementID,
			      IN UCHAR apidx, OUT PUCHAR pRsnIe, OUT UCHAR * rsn_len);

NTSTATUS PMF_RsnCapableValidation(IN PRTMP_ADAPTER pAd,
				  IN PUINT8 pRsnie,
				  IN UINT rsnie_len,
				  IN BOOLEAN self_MFPC,
				  IN BOOLEAN self_MFPR, IN PMAC_TABLE_ENTRY pEntry);

BOOLEAN PMF_PerformTxFrameAction(IN PRTMP_ADAPTER pAd, OUT PNDIS_PACKET pPacket);

BOOLEAN PMF_PerformRxFrameAction(IN PRTMP_ADAPTER pAd, IN RX_BLK * pRxBlk);

void rtmp_read_pmf_parameters_from_file(IN PRTMP_ADAPTER pAd,
				IN PSTRING tmpbuf, IN const char *pBuffer);

INT Set_PMFMFPC_Proc(RTMP_ADAPTER *pAd, PSTRING arg);

INT Set_PMFMFPR_Proc(RTMP_ADAPTER *pAd, PSTRING arg);

INT Set_PMFSHA256_Proc(RTMP_ADAPTER *pAd, PSTRING arg);

#endif /* DOT11W_PMF_SUPPORT */

#endif /* __PMF_H */
