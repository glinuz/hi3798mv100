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


/*
	Module Name:
	wfd.h

	Revision History:
	Who			When			What
	--------		----------		----------------------------------------------

*/

#ifndef	__WFD_H__
#define	__WFD_H__

#ifdef WFD_SUPPORT

#include "rtmp_type.h"

INT Set_WfdEnable_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

#ifdef RT_CFG80211_SUPPORT
INT Set_WfdInsertIe_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);
#endif /* RT_CFG80211_SUPPORT */

INT Set_WfdDeviceType_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdCouple_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdSessionAvailable_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdCP_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdRtspPort_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdMaxThroughput_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_WfdLocalIp_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

INT Set_PeerRtspPort_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg);

VOID WfdMakeWfdIE(IN PRTMP_ADAPTER pAd,
		  IN ULONG WfdIeBitmap, OUT PUCHAR pOutBuf, OUT PULONG pIeLen);

ULONG InsertWfdSubelmtTlv(IN PRTMP_ADAPTER pAd,
			  IN UCHAR SubId,
			  IN PUCHAR pInBuffer, IN PUCHAR pOutBuffer, IN UINT Action);

VOID WfdParseSubElmt(IN PRTMP_ADAPTER pAd,
		     IN PWFD_ENTRY_INFO pWfdEntryInfo, IN VOID *Msg, IN ULONG MsgLen);

VOID WfdCfgInit(IN PRTMP_ADAPTER pAd);

#endif /* WFD_SUPPORT */
#endif /* __WFD_H__ */
