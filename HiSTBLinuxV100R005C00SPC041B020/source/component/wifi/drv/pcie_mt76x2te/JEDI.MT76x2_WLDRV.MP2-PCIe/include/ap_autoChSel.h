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


#include "ap_autoChSel_cmm.h"

#ifndef __AUTOCHSELECT_H__
#define __AUTOCHSELECT_H__

#define AP_AUTO_CH_SEL(__P, __O)	APAutoSelectChannel((__P), (__O))

ULONG AutoChBssSearchWithSSID(IN PRTMP_ADAPTER pAd,
			      IN PUCHAR Bssid, IN PUCHAR pSsid, IN UCHAR SsidLen, IN UCHAR Channel);

VOID APAutoChannelInit(IN PRTMP_ADAPTER pAd);

VOID UpdateChannelInfo(IN PRTMP_ADAPTER pAd, IN int ch, IN ChannelSel_Alg Alg);

#ifdef SUPPORT_ACS_BY_SCAN
VOID AutoChBssTableUpdateByScanTab(IN PRTMP_ADAPTER pAd);
#endif /* endif */

ULONG AutoChBssInsertEntry(IN PRTMP_ADAPTER pAd,
			   IN PUCHAR pBssid,
			   IN CHAR Ssid[],
			   IN UCHAR SsidLen,
			   IN UCHAR ChannelNo, IN UCHAR ExtChOffset, IN CHAR Rssi);

VOID AutoChBssTableInit(IN PRTMP_ADAPTER pAd);

VOID ChannelInfoInit(IN PRTMP_ADAPTER pAd);

VOID AutoChBssTableDestroy(IN PRTMP_ADAPTER pAd);

VOID ChannelInfoDestroy(IN PRTMP_ADAPTER pAd);

VOID CheckPhyModeIsABand(IN PRTMP_ADAPTER pAd);

UCHAR SelectBestChannel(IN PRTMP_ADAPTER pAd, IN ChannelSel_Alg Alg);

UCHAR APAutoSelectChannel(IN PRTMP_ADAPTER pAd, IN ChannelSel_Alg Alg);

INT32 ACS_PerformAlgorithm(RTMP_ADAPTER *pAd, ChannelSel_Alg Alg);

#endif /* __AUTOCHSELECT_H__ */
