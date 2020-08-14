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


#ifndef __STA_H__
#define __STA_H__

BOOLEAN RTMPCheckChannel(IN PRTMP_ADAPTER pAd, IN UCHAR CentralChannel, IN UCHAR Channel);

VOID InitChannelRelatedValue(IN PRTMP_ADAPTER pAd);

VOID AdjustChannelRelatedValue(IN PRTMP_ADAPTER pAd,
			       OUT UCHAR * pBwFallBack,
			       IN USHORT ifIndex,
			       IN UINT8 BandWidth, IN UCHAR PriCh, IN UCHAR ExtraCh);

VOID RTMPReportMicError(IN PRTMP_ADAPTER pAd, IN PCIPHER_KEY pWpaKey);

INT RTMPCheckRxError(IN RTMP_ADAPTER * pAd,
		     IN HEADER_802_11 * pHeader, IN RX_BLK * pRxBlk, IN RXINFO_STRUC * pRxInfo);

VOID WpaMicFailureReportFrame(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

VOID WpaDisassocApAndBlockAssoc(IN PVOID SystemSpecific1,
				IN PVOID FunctionContext,
				IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);

VOID WpaStaPairwiseKeySetting(IN PRTMP_ADAPTER pAd);

VOID WpaStaGroupKeySetting(IN PRTMP_ADAPTER pAd);

VOID WpaSendEapolStart(IN PRTMP_ADAPTER pAdapter, IN PUCHAR pBssid);

BOOLEAN STACheckTkipMICValue(IN PRTMP_ADAPTER pAd, IN MAC_TABLE_ENTRY * pEntry, IN RX_BLK * pRxBlk);

VOID STAHandleRxDataFrame(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk);

VOID STARxDataFrameAnnounce(IN PRTMP_ADAPTER pAd,
			    IN MAC_TABLE_ENTRY * pEntry,
			    IN RX_BLK *pRxBlk, IN UCHAR FromWhichBSSID);

VOID STARxEAPOLFrameIndicate(IN PRTMP_ADAPTER pAd,
			     IN MAC_TABLE_ENTRY * pEntry,
			     IN RX_BLK *pRxBlk, IN UCHAR FromWhichBSSID);

NDIS_STATUS STAHardTransmit(IN PRTMP_ADAPTER pAd, IN TX_BLK * pTxBlk, IN UCHAR QueIdx);

INT STASendPacket(IN RTMP_ADAPTER *pAd, IN PNDIS_PACKET pPacket);

INT STAInitialize(RTMP_ADAPTER *pAd);

VOID RTMPHandleTwakeupInterrupt(IN PRTMP_ADAPTER pAd);

#endif /* __STA_H__ */
