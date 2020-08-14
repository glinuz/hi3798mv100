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


#ifndef __SPECTRUM_H__
#define __SPECTRUM_H__

#include "rtmp_type.h"
#include "spectrum_def.h"

UINT8 GetRegulatoryMaxTxPwr(IN PRTMP_ADAPTER pAd, IN UINT8 channel);

CHAR RTMP_GetTxPwr(IN PRTMP_ADAPTER pAd, IN HTTRANSMIT_SETTING HTTxMode);

/*
	==========================================================================
	Description:
		Prepare Measurement request action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
VOID MakeMeasurementReqFrame(IN PRTMP_ADAPTER pAd,
			     OUT PUCHAR pOutBuffer,
			     OUT PULONG pFrameLen,
			     IN UINT8 TotalLen,
			     IN UINT8 Category,
			     IN UINT8 Action,
			     IN UINT8 MeasureToken,
			     IN UINT8 MeasureReqMode,
			     IN UINT8 MeasureReqType, IN UINT16 NumOfRepetitions);

/*
	==========================================================================
	Description:
		Prepare Measurement report action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
VOID EnqueueMeasurementRep(IN PRTMP_ADAPTER pAd,
			   IN PUCHAR pDA,
			   IN UINT8 DialogToken,
			   IN UINT8 MeasureToken,
			   IN UINT8 MeasureReqMode,
			   IN UINT8 MeasureReqType, IN UINT8 ReportInfoLen, IN PUINT8 pReportInfo);

/*
	==========================================================================
	Description:
		Prepare TPC Request action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
VOID EnqueueTPCReq(IN PRTMP_ADAPTER pAd, IN PUCHAR pDA, IN UCHAR DialogToken);

/*
	==========================================================================
	Description:
		Prepare TPC Report action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.

	Return	: None.
	==========================================================================
 */
VOID EnqueueTPCRep(IN PRTMP_ADAPTER pAd,
		   IN PUCHAR pDA, IN UINT8 DialogToken, IN UINT8 TxPwr, IN UINT8 LinkMargin);

#ifdef WDS_SUPPORT
/*
	==========================================================================
	Description:
		Prepare Channel Switch Announcement action frame and enqueue it into
		management queue waiting for transmition.

	Parametrs:
		1. the destination mac address of the frame.
		2. Channel switch announcement mode.
		2. a New selected channel.

	Return	: None.
	==========================================================================
 */
VOID EnqueueChSwAnn(IN PRTMP_ADAPTER pAd, IN PUCHAR pDA, IN UINT8 ChSwMode, IN UINT8 NewCh);
#endif /* WDS_SUPPORT */

/*
	==========================================================================
	Description:
		Spectrun action frames Handler such as channel switch annoucement,
		measurement report, measurement request actions frames.

	Parametrs:
		Elme - MLME message containing the received frame

	Return	: None.
	==========================================================================
 */
VOID PeerSpectrumAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

/*
	==========================================================================
	Description:

	Parametrs:

	Return	: None.
	==========================================================================
 */
INT Set_MeasureReq_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_TpcReq_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_PwrConstraint(IN PRTMP_ADAPTER pAd, IN PSTRING arg);


NDIS_STATUS MeasureReqTabInit(IN PRTMP_ADAPTER pAd);

VOID MeasureReqTabExit(IN PRTMP_ADAPTER pAd);

PMEASURE_REQ_ENTRY MeasureReqLookUp(IN PRTMP_ADAPTER pAd, IN UINT8 DialogToken);

PMEASURE_REQ_ENTRY MeasureReqInsert(IN PRTMP_ADAPTER pAd, IN UINT8 DialogToken);

VOID MeasureReqDelete(IN PRTMP_ADAPTER pAd, IN UINT8 DialogToken);

VOID InsertChannelRepIE(IN PRTMP_ADAPTER pAd,
			OUT PUCHAR pFrameBuf,
			OUT PULONG pFrameLen, IN PSTRING pCountry, IN UINT8 RegulatoryClass);

VOID InsertTpcReportIE(IN PRTMP_ADAPTER pAd,
		       OUT PUCHAR pFrameBuf,
		       OUT PULONG pFrameLen, IN UINT8 TxPwr, IN UINT8 LinkMargin);

VOID InsertDialogToken(IN PRTMP_ADAPTER pAd,
		       OUT PUCHAR pFrameBuf, OUT PULONG pFrameLen, IN UINT8 DialogToken);

NDIS_STATUS TpcReqTabInit(IN PRTMP_ADAPTER pAd);

VOID TpcReqTabExit(IN PRTMP_ADAPTER pAd);

VOID NotifyChSwAnnToPeerAPs(IN PRTMP_ADAPTER pAd,
			    IN PUCHAR pRA, IN PUCHAR pTA, IN UINT8 ChSwMode, IN UINT8 Channel);

VOID RguClass_BuildBcnChList(IN PRTMP_ADAPTER pAd, OUT PUCHAR pBuf, OUT PULONG pBufLen);
#endif /* __SPECTRUM_H__ */
