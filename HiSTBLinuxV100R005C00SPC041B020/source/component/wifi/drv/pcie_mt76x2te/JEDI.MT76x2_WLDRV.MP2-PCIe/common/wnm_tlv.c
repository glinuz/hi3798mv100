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

#include "rt_config.h"


VOID InsertDMSReqElement(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN WNM_DMS_REQUEST_ELEMENT DMSReqElement)
{
	ULONG TempLen;
	
	MakeOutgoingFrame(	pFrameBuf,		&TempLen,
						3,				&DMSReqElement,
						END_OF_ARGS);

/*	*pFrameLen = *pFrameLen + TempLen; */

	return;	
}



VOID WNM_InsertDMS(
	IN PRTMP_ADAPTER pAd,
	OUT PUCHAR pFrameBuf,
	OUT PULONG pFrameLen,
	IN UINT8 Len,
	IN UCHAR DMSID,
	IN WNM_TCLAS wmn_tclas,
	IN ULONG IpAddr)
{
	ULONG TempLen;
	UINT8 DMSIDId = DMSID;
	WNM_TCLAS tclas;
	UCHAR 	TCLASElementID = WNM_CATEGORY;
	UCHAR 	TCLASLength = WNM_TCLAS_TYPE_IP_V4_LEN;

	tclas.UserPriority = 0;
	tclas.ClassifierType = 1;
	tclas.ClassifierMask = 255;
	tclas.Clasifier.IPv4.Version = 0;
	tclas.Clasifier.IPv4.IpSource = 0;
	tclas.Clasifier.IPv4.PortSource = 0;
	tclas.Clasifier.IPv4.PortDest = 0;
	tclas.Clasifier.IPv4.DSCP = 0;
	tclas.Clasifier.IPv4.Protocol = 4;

	tclas.Clasifier.IPv4.IpDest = IpAddr;
	
	MakeOutgoingFrame(	pFrameBuf,						&TempLen,
						1,								&DMSIDId,
						1,								&Len,
						1,								&TCLASElementID,
						1,								&TCLASLength,
						WNM_TCLAS_TYPE_IP_V4_LEN,				&tclas,
						END_OF_ARGS);

	*pFrameLen = *pFrameLen + TempLen;

	return;
}


