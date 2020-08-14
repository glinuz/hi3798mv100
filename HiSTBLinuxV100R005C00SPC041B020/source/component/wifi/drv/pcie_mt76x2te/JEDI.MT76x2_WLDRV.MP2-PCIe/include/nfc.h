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

#ifndef __WSC_NFC_H__
#define __WSC_NFC_H__

VOID	NfcParseRspCommand(
	IN  PRTMP_ADAPTER	pAd,
	IN  PUCHAR pData,
	IN  USHORT DataLen);

VOID	NfcCommand(
	IN	PRTMP_ADAPTER	pAd, 
	IN	UCHAR Action,
	IN  UCHAR Type,
	IN  SHORT DataLen,
	IN  PUCHAR pData);

VOID	NfcGenRandomPasswd(
	IN  PRTMP_ADAPTER	pAd,
	IN  PWSC_CTRL       pWscCtrl);

INT 	NfcBuildWscProfileTLV(
	IN	PRTMP_ADAPTER pAd,
	IN  PWSC_CTRL pWscCtrl,
	OUT	UCHAR *pbuf,
	OUT USHORT *pBufLen);

INT 	NfcBuildOOBDevPasswdTLV(
	IN	PRTMP_ADAPTER pAd,
	IN  PWSC_CTRL pWscCtrl,
	IN	UCHAR	HandoverType,
	OUT	UCHAR *pbuf,
	OUT USHORT *pBufLen);

INT		Set_NfcStatus_Proc(
	IN  RTMP_ADAPTER		*pAd,
	IN  PSTRING			arg);

INT 	Set_NfcPasswdToken_Proc(
	IN  RTMP_ADAPTER		*pAd,
	IN  PSTRING			arg);

INT 	Set_NfcConfigurationToken_Proc(
	IN  RTMP_ADAPTER		*pAd,
	IN  PSTRING			arg);

INT 	Get_NfcStatus_Proc(
	IN	PRTMP_ADAPTER	pAd, 
	IN	PSTRING			arg);

INT		Set_DoWpsByNFC_Proc(
	IN  PRTMP_ADAPTER 	pAd, 
	IN  PSTRING 			arg);

INT Set_NfcRegenPK_Proc(
	IN PRTMP_ADAPTER 	pAd, 
	IN PSTRING 			arg);


#endif /* __WSC_NFC_H__ */
