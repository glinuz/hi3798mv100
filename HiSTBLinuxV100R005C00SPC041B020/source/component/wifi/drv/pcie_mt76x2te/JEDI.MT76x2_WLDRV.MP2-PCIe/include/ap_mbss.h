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
	For MBSS, the phy mode is different;
	So MBSS_PHY_MODE_RESET() can help us to adjust the correct mode &
	maximum MCS for the BSS.
*/
#define MBSS_PHY_MODE_RESET(__BssId, __HtPhyMode)				\
	{                                                             \
		UCHAR __PhyMode = pAd->ApCfg.MBSSID[__BssId].wdev.PhyMode;	\
		if ((__PhyMode == WMODE_B) &&							\
			(__HtPhyMode.field.MODE != MODE_CCK)) {             \
			__HtPhyMode.field.MODE = MODE_CCK;					\
			__HtPhyMode.field.MCS = 3;							\
		}                                                      \
		else if ((!WMODE_CAP_N(__PhyMode)) &&						\
				(__PhyMode != WMODE_B) &&						\
				(__HtPhyMode.field.MODE != MODE_OFDM)) {    \
			__HtPhyMode.field.MODE = MODE_OFDM;					\
			__HtPhyMode.field.MCS = 7;							\
		}	                                                    \
		else if ((__PhyMode != WMODE_B) &&                                              \
				(__PhyMode != WMODE_G) &&                                               \
				(__PhyMode != WMODE_A) &&                                               \
				(!WMODE_CAP_AC(__PhyMode)) &&   \
				(WMODE_CAP_N(__PhyMode))) {    \
			__HtPhyMode.field.MODE = MODE_HTMIX;                                    \
		}				\
		else if (WMODE_CAP_AC(__PhyMode)) {       \
			__HtPhyMode.field.MODE = MODE_VHT;                                      \
			__HtPhyMode.field.MCS = 9;                                                      \
	       }       \
	}

/* Public function list */
INT Show_MbssInfo_Display_Proc(RTMP_ADAPTER *pAd, PSTRING arg);
VOID MBSS_Init(RTMP_ADAPTER *pAd, RTMP_OS_NETDEV_OP_HOOK *pNetDevOps);

VOID MBSS_Remove(RTMP_ADAPTER *pAd);

INT MBSS_Open(PNET_DEV pDev);

INT MBSS_Close(PNET_DEV pDev);

INT32 RT28xx_MBSS_IdxGet(IN PRTMP_ADAPTER pAd, IN PNET_DEV pDev);
