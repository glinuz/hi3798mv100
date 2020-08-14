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

	All related CFG80211 Scan function body.

	History:

***************************************************************************/

#define RTMP_MODULE_OS

#ifdef RT_CFG80211_SUPPORT

#include "rt_config.h"

#ifdef CONFIG_STA_SUPPORT
VOID CFG80211DRV_OpsScanInLinkDownAction(VOID *pAdOrg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	BOOLEAN Cancelled;

	DBGPRINT(RT_DEBUG_TRACE, ("---> CFG80211_MLME Disconnect in Scaning, ORI ==> %lu\n",
				  pAd->Mlme.CntlMachine.CurrState));

	RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &Cancelled);
	pAd->MlmeAux.Channel = 0;

	pAd->cfg80211_ctrl.FlgCfg80211Scanning = FALSE;
	CFG80211OS_ScanEnd(pAd->pCfg80211_CB, TRUE);

	ScanNextChannel(pAd, OPMODE_STA);
	DBGPRINT(RT_DEBUG_TRACE, ("<--- CFG80211_MLME Disconnect in Scan END, ORI ==> %lu\n",
				  pAd->Mlme.CntlMachine.CurrState));
}

BOOLEAN CFG80211DRV_OpsScanRunning(VOID *pAdOrg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;

	return pAd->cfg80211_ctrl.FlgCfg80211Scanning;
}
#endif /*CONFIG_STA_SUPPORT */

/* Refine on 2013/04/30 for two functin into one */
INT CFG80211DRV_OpsScanGetNextChannel(VOID *pAdOrg, BOOLEAN pending)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	if (cfg80211_ctrl->pCfg80211ChanList != NULL) {
		if (cfg80211_ctrl->Cfg80211CurChanIndex < cfg80211_ctrl->Cfg80211ChanListLen) {
			if (pending) {
				return cfg80211_ctrl->pCfg80211ChanList[cfg80211_ctrl->
								Cfg80211CurChanIndex];
			} else {
				return cfg80211_ctrl->pCfg80211ChanList[cfg80211_ctrl->
								Cfg80211CurChanIndex++];
			}
		} else {
			os_free_mem(NULL, cfg80211_ctrl->pCfg80211ChanList);
			cfg80211_ctrl->pCfg80211ChanList = NULL;
			cfg80211_ctrl->Cfg80211ChanListLen = 0;
			cfg80211_ctrl->Cfg80211CurChanIndex = 0;

			return 0;
		}
	}

	return 0;
}

BOOLEAN CFG80211DRV_OpsScanSetSpecifyChannel(VOID *pAdOrg, VOID *pData, UINT8 dataLen)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;
	UINT32 *pChanList = (UINT32 *) pData;

	if (pChanList != NULL) {
		if (cfg80211_ctrl->pCfg80211ChanList != NULL)
			os_free_mem(NULL, cfg80211_ctrl->pCfg80211ChanList);

		os_alloc_mem(NULL, (UCHAR **) &cfg80211_ctrl->pCfg80211ChanList,
			     sizeof(UINT32) * dataLen);
		if (cfg80211_ctrl->pCfg80211ChanList != NULL) {
			NdisCopyMemory(cfg80211_ctrl->pCfg80211ChanList, pChanList,
				       sizeof(UINT32) * dataLen);
			cfg80211_ctrl->Cfg80211ChanListLen = dataLen;
			cfg80211_ctrl->Cfg80211CurChanIndex = 0;	/* Start from index 0 */
			return NDIS_STATUS_SUCCESS;
		} else {
			return NDIS_STATUS_FAILURE;
		}
	}

	return NDIS_STATUS_FAILURE;
}

BOOLEAN CFG80211DRV_OpsScanCheckStatus(VOID *pAdOrg, UINT8 IfType)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;

	/* CFG_TODO */
	if (CFG80211DRV_OpsScanRunning(pAd)) {
		CFG80211DBG(RT_DEBUG_ERROR, ("SCAN_FAIL: CFG80211 Internal SCAN Flag On\n"));
		return FALSE;
	}

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
		CFG80211DBG(RT_DEBUG_ERROR, ("SCAN_FAIL: BSS_SCAN_IN_PROGRESS\n"));
		return FALSE;
	}

	/* To avoid the scan cmd come-in during driver init */
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP)) {
		DBGPRINT(RT_DEBUG_TRACE, ("SCAN_FAIL: Scan cmd before Startup finish\n"));
		return FALSE;
	}

	if ((OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)) &&
	    ((pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA) ||
	     (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPAPSK) ||
	     (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2) ||
	     (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2PSK)) &&
	    (pAd->StaCfg.wdev.PortSecured == WPA_802_1X_PORT_NOT_SECURED)) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("SCAN_FAIL: Link UP, Port Not Secured! ignore this set::OID_802_11_BSSID_LIST_SCAN\n"));
		return FALSE;
	}

	if (pAd->Mlme.CntlMachine.CurrState != CNTL_IDLE) {
		DBGPRINT(RT_DEBUG_ERROR, ("SCAN_FAIL: MLME busying\n"));
		DBGPRINT(RT_DEBUG_ERROR, ("Mlme.CntlMachine.CurrState=%lu\n",
			pAd->Mlme.CntlMachine.CurrState));

		if (((CFG80211_CB *)(pAd->pCfg80211_CB))->ScanIsAborted)
			pAd->StaCfg.ScanAbortedNum++;
		else
			pAd->StaCfg.ScanAbortedNum = 1;
		if (pAd->StaCfg.ScanAbortedNum > 5) {
			RTMP_MLME_RESET_STATE_MACHINE(pAd);
			pAd->StaCfg.ScanAbortedNum = 0;
		}
		return FALSE;
	}
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	if (RTMP_CFG80211_VIF_P2P_CLI_ON(pAd) &&
	    (pAd->cfg80211_ctrl.FlgCfg80211Connecting == TRUE) &&
	    (IfType == RT_CMD_80211_IFTYPE_STATION)) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("SCAN_FAIL: P2P_CLIENT In Connecting & Canncel Scan with Infra Side\n"));
		return FALSE;
	}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_SUPPORT
	if (pAd->cfg80211_ctrl.FlgCfg8021Disable2040Scan == TRUE &&
	    (IfType == RT_CMD_80211_IFTYPE_AP)) {
		DBGPRINT(RT_DEBUG_ERROR, ("Disable 20/40 scan!!\n"));
		return FALSE;
	}
#endif /* RT_CFG80211_SUPPORT */

	/* do scan */
	pAd->cfg80211_ctrl.FlgCfg80211Scanning = TRUE;
#endif /*CONFIG_STA_SUPPORT */
	return TRUE;
}

#ifdef CONFIG_MULTI_CHANNEL
void CFG80211DRV_Set_NOA(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	/* iversonmcc */
	ULONG Value;
	ULONG TimeTillTbtt;
	ULONG temp;

	if (pAd->Mlme.bStartMcc == TRUE) {
		/* scan , send null fram to infra , and switch to GO */
		Stop_MCC(pAd, 2);
	}

/*
pNdev->name = wlan0
 896.635300] 80211> CFG80211_OpsScan ==> wlan0(2)
50ms  start scan
 896.683053] mt76x2_switch_channel(): Switch to Ch#1(2T2R), BBP_BW=0, bbp_ch_idx=0)
 897.102052] mt76x2_switch_channel(): Switch to Ch#1(2T2R), BBP_BW=0, bbp_ch_idx=0)
one channel 60 ms ~  7 channel
420ms

 898.328054] mt76x2_switch_channel(): Switch to Ch#9(2T2R), BBP_BW=0, bbp_ch_idx=0)
interval = 420+120 ms
Count =4;

pNdev->name = p2p0
50ms  start scan

 887.081731] 80211> CFG80211_OpsScan ==> p2p0(2)
50ms  start scan

 887.130050] mt76x2_switch_channel(): Switch to Ch#1(2T2R), BBP_BW=0, bbp_ch_idx=0)
 887.190051] mt76x2_switch_channel(): Switch to Ch#6(2T2R), BBP_BW=0, bbp_ch_idx=0)
 887.251555] mt76x2_switch_channel(): Switch to Ch#11(2T2R), BBP_BW=0, bbp_ch_idx=0)
 887.285554] mt76x2_switch_channel(): Switch to Ch#1(2T2R), BBP_BW=0, bbp_ch_idx=0)
 887.319045] mt76x2_switch_channel(): Switch to Ch#1(2T2R), BBP_BW=0, bbp_ch_idx=0)
180ms
count =1
*/

	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {

		if (rtstrcasecmp(pData, "p2p0") == TRUE) {
			DBGPRINT(RT_DEBUG_TRACE, ("this is p2p scan!!!!!!!!!!!!\n"));
			RTMP_IO_READ32(pAd, TSF_TIMER_DW1, &pAd->GONoASchedule.TsfHighByte);
/* DBGPRINT(RT_DEBUG_TRACE,("P2pGOStartNoA parameter.!!!!HighByte = %lx\n", pAd->GONoASchedule.TsfHighByte)); */
			RTMP_IO_READ32(pAd, TBTT_TIMER, &TimeTillTbtt);
			TimeTillTbtt = TimeTillTbtt & 0x1ffff;
/* DBGPRINT(RT_DEBUG_TRACE,("   .!!!!TimeTillTbtt =  %ld\n", TimeTillTbtt)); */

			RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &Value);
/* DBGPRINT(RT_DEBUG_TRACE,("   .!!!!Current Tsf LSB = = %ld\n",  Value)); */
			temp = TimeTillTbtt * 64 + Value;
			DBGPRINT(RT_DEBUG_TRACE, ("   .!!!!Tsf LSB + TimeTillTbtt= %ld\n", temp));
			/* Wait five beacon 0x7d00 for 5 beacon interval.  0x6400 is set to 25%*beacon interval */
/* pAd->GONoASchedule.StartTime = Value + TimeTillTbtt*64 + 204800 + 3200; */
			pAd->GONoASchedule.StartTime = Value + TimeTillTbtt * 64 + 409600 + 3200;

			DBGPRINT(RT_DEBUG_TRACE,
				 (" pAd->GONoASchedule.StartTime = %ld\n",
				  pAd->GONoASchedule.StartTime));
			pAd->GONoASchedule.Count = 9;
			pAd->GONoASchedule.Duration = 180000;
			pAd->GONoASchedule.Interval = 384800;

		}

		if (rtstrcasecmp(pData, "wlan0") == TRUE) {
			DBGPRINT(RT_DEBUG_TRACE, ("this is wlan0 scan!!!!!!!!!!!!\n"));
/* RTMP_IO_READ32(pAd, TSF_TIMER_DW1, &pAd->GONoASchedule.TsfHighByte); */
/* DBGPRINT(RT_DEBUG_ERROR,("P2pGOStartNoA parameter.!!!!HighByte = %lx\n", pAd->GONoASchedule.TsfHighByte)); */
			RTMP_IO_READ32(pAd, TBTT_TIMER, &TimeTillTbtt);
			TimeTillTbtt = TimeTillTbtt & 0x1ffff;
/* DBGPRINT(RT_DEBUG_TRACE,("   .!!!!TimeTillTbtt =  %ld\n", TimeTillTbtt)); */

			RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &Value);
/* DBGPRINT(RT_DEBUG_TRACE,("   .!!!!Current Tsf LSB = = %ld\n",  Value)); */
			temp = TimeTillTbtt * 64 + Value;
/* DBGPRINT(RT_DEBUG_TRACE,("   .!!!!Tsf LSB + TimeTillTbtt= %ld\n", temp)); */
			/* Wait five beacon 0x7d00 for 5 beacon interval.  0x6400 is set to 25%*beacon interval */
/* pAd->GONoASchedule.StartTime = Value + TimeTillTbtt*64 + 204800 + 3200; */
/* pAd->GONoASchedule.StartTime = Value + TimeTillTbtt*64 + 409600 + 3200; */

			pAd->GONoASchedule.StartTime = Value + TimeTillTbtt * 64 + 512000 + 3200;

			DBGPRINT(RT_DEBUG_TRACE,
				 (" pAd->GONoASchedule.StartTime = %ld\n",
				  pAd->GONoASchedule.StartTime));
			pAd->GONoASchedule.Count = (4 + (7 * 4) + 2);	/*wait 4 beacon + (interval * 4) */
/* pAd->GONoASchedule.Duration= 409600; */
/* pAd->GONoASchedule.Duration= (409600 + 3200); */
			pAd->GONoASchedule.Duration = 307200;

/* pAd->GONoASchedule.Interval=  614400; */
/* pAd->GONoASchedule.Interval=  (819200 + 3200); */
			pAd->GONoASchedule.Interval = 716800;

		}
	}
	return;
}
#endif /* CONFIG_MULTI_CHANNEL */

BOOLEAN CFG80211DRV_OpsScanExtraIesSet(VOID *pAdOrg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CFG80211_CB *pCfg80211_CB = pAd->pCfg80211_CB;
	UINT ie_len = 0;
	PCFG80211_CTRL cfg80211_ctrl = &pAd->cfg80211_ctrl;

	if (pCfg80211_CB->pCfg80211_ScanReq)
		ie_len = pCfg80211_CB->pCfg80211_ScanReq->ie_len;

	CFG80211DBG(RT_DEBUG_INFO, ("80211> CFG80211DRV_OpsExtraIesSet ==> %d\n", ie_len));
#ifdef CONFIG_STA_SUPPORT
	CFG80211DBG(RT_DEBUG_INFO, ("80211> is_wpa_supplicant_up ==> %d\n",
				    pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP));
#endif /*CONFIG_STA_SUPPORT */
	if (ie_len == 0)
		return FALSE;

	/* Reset the ExtraIe and Len */
	if (cfg80211_ctrl->pExtraIe) {
		os_free_mem(NULL, cfg80211_ctrl->pExtraIe);
		cfg80211_ctrl->pExtraIe = NULL;
	}
	cfg80211_ctrl->ExtraIeLen = 0;

	os_alloc_mem(pAd, (UCHAR **) &(cfg80211_ctrl->pExtraIe), ie_len);
	if (cfg80211_ctrl->pExtraIe) {
		NdisCopyMemory(cfg80211_ctrl->pExtraIe, pCfg80211_CB->pCfg80211_ScanReq->ie,
			       ie_len);
		cfg80211_ctrl->ExtraIeLen = ie_len;

		/* hex_dump("CFG8021_SCAN_EXTRAIE", cfg80211_ctrl->pExtraIe,
			 cfg80211_ctrl->ExtraIeLen); */
	} else {
		CFG80211DBG(RT_DEBUG_ERROR,
			    ("80211> CFG80211DRV_OpsExtraIesSet ==> allocate fail.\n"));
		return FALSE;
	}

	return TRUE;
}

#ifdef CFG80211_SCAN_SIGNAL_AVG
static void CFG80211_CalBssAvgRssi(IN BSS_ENTRY * pBssEntry)
{
	UINT RssiTemp;

	if (0 == pBssEntry->LastRssi) {
		pBssEntry->LastRssi = pBssEntry->MaxRssi;
		return;
	}
	if (!pBssEntry->bContinous && pBssEntry->MaxRssi < pBssEntry->LastRssi - 20) {
		RssiTemp = pBssEntry->LastRssi;
		RssiTemp = RssiTemp * 7  + pBssEntry->MaxRssi;
		pBssEntry->MaxRssi = RssiTemp / 8;
		pBssEntry->LastRssi = pBssEntry->MaxRssi;
		pBssEntry->bContinous = TRUE;
		return;
	}
	if (pBssEntry->bContinous)
		pBssEntry->bContinous = FALSE;
	pBssEntry->LastRssi = pBssEntry->MaxRssi;
}

static void CFG80211_UpdateBssTableRssi(IN VOID *pAdCB)
{

	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	CFG80211_CB *pCfg80211_CB = (CFG80211_CB *) pAd->pCfg80211_CB;
	struct wiphy *pWiphy = pCfg80211_CB->pCfg80211_Wdev->wiphy;
	struct ieee80211_channel *chan;
	struct cfg80211_bss *bss;
	BSS_ENTRY *pBssEntry;
	UINT index;
	UINT32 CenFreq;

	for (index = 0; index < pAd->ScanTab.BssNr; index++) {
		pBssEntry = &pAd->ScanTab.BssEntry[index];

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
		if (pAd->ScanTab.BssEntry[index].Channel > 14)
			CenFreq =
			    ieee80211_channel_to_frequency(pAd->ScanTab.BssEntry[index].Channel,
							   IEEE80211_BAND_5GHZ);
		else
			CenFreq =
			    ieee80211_channel_to_frequency(pAd->ScanTab.BssEntry[index].Channel,
							   IEEE80211_BAND_2GHZ);
#else
		CenFreq = ieee80211_channel_to_frequency(pAd->ScanTab.BssEntry[index].Channel);
#endif /* endif */

		chan = ieee80211_get_channel(pWiphy, CenFreq);
		bss =
		    cfg80211_get_bss(pWiphy, chan, pBssEntry->Bssid, pBssEntry->Ssid,
				     pBssEntry->SsidLen, WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
		if (bss == NULL) {
			/* ScanTable Entry not exist in kernel buffer */
		} else {
			/* HIT */
			CFG80211_CalBssAvgRssi(pBssEntry);
			bss->signal = pBssEntry->MaxRssi * 100;	/* UNIT: MdBm */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
			cfg80211_put_bss(pWiphy, bss);
#else
			cfg80211_put_bss(bss);
#endif /* endif */
		}
		pBssEntry->bNot1stForEvyScan = FALSE;
	}
}
#endif /* CFG80211_SCAN_SIGNAL_AVG */

/*
========================================================================
Routine Description:
	Inform us that a scan is got.

Arguments:
	pAdCB				- WLAN control block pointer

Return Value:
	NONE

Note:
	Call RT_CFG80211_SCANNING_INFORM, not CFG80211_Scaning
========================================================================
*/
VOID CFG80211_Scaning(IN VOID *pAdCB,
		      IN UINT32 BssIdx,
		      IN UINT32 ChanId, IN UCHAR *pFrame, IN UINT32 FrameLen, IN INT32 RSSI)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	VOID *pCfg80211_CB = pAd->pCfg80211_CB;
	BOOLEAN FlgIsNMode;
	UINT8 BW;

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)) {
		DBGPRINT(RT_DEBUG_TRACE, ("80211> Network is down!\n"));
		return;
	}

	/*
	   In connect function, we also need to report BSS information to cfg80211;
	   Not only scan function.
	 */
	if ((!CFG80211DRV_OpsScanRunning(pAd)) &&
	    (pAd->cfg80211_ctrl.FlgCfg80211Connecting == FALSE)) {
		return;		/* no scan is running from wpa_supplicant */
	}

	/* init */
	/* Note: Can not use local variable to do pChan */
	if (WMODE_CAP_N(pAd->CommonCfg.PhyMode))
		FlgIsNMode = TRUE;
	else
		FlgIsNMode = FALSE;

	if (pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20)
		BW = 0;
	else
		BW = 1;

	CFG80211OS_Scaning(pCfg80211_CB, ChanId, pFrame, FrameLen, RSSI, FlgIsNMode, BW);
#endif /* CONFIG_STA_SUPPORT */
}

/*
========================================================================
Routine Description:
	Inform us that scan ends.

Arguments:
	pAdCB			- WLAN control block pointer
	FlgIsAborted	- 1: scan is aborted

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_ScanEnd(IN VOID *pAdCB, IN BOOLEAN FlgIsAborted)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;

#ifdef CONFIG_MULTI_CHANNEL
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	PAPCLI_STRUCT pApCliEntry = pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];
	struct wifi_dev *wdev = &pMbss->wdev;

	if (RTMP_CFG80211_VIF_P2P_CLI_ON(pAd))
		wdev = &(pApCliEntry->wdev);
#endif /* CONFIG_MULTI_CHANNEL */

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)) {
		DBGPRINT(RT_DEBUG_ERROR, ("80211> Network is down!\n"));
		return;
	}

	if (!CFG80211DRV_OpsScanRunning(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR, ("80211> No scan is running!\n"));
		return;		/* no scan is running */
	}

	if (FlgIsAborted == TRUE)
		FlgIsAborted = 1;
	else {
		FlgIsAborted = 0;
#ifdef CFG80211_SCAN_SIGNAL_AVG
		CFG80211_UpdateBssTableRssi(pAd);
#endif /* CFG80211_SCAN_SIGNAL_AVG */
	}

	CFG80211OS_ScanEnd(CFG80211CB, FlgIsAborted);

#ifdef CONFIG_MULTI_CHANNEL
	if ((pAd->StaCfg.wdev.bw == wdev->bw) && (pAd->StaCfg.wdev.channel == wdev->channel)) {
		DBGPRINT(RT_DEBUG_TRACE, ("Scc case , not star mcc when scan end\n"));
	} else if (wdev->channel == 0) {
		DBGPRINT(RT_DEBUG_TRACE, ("CLI still connect  , not star mcc when scan end\n"));
	} else if (INFRA_ON(pAd)
		   && (RTMP_CFG80211_VIF_P2P_GO_ON(pAd) || RTMP_CFG80211_VIF_P2P_CLI_ON(pAd))
		   && (((pAd->StaCfg.wdev.bw == wdev->bw)
			&& (pAd->StaCfg.wdev.channel != wdev->channel))
		       || !((pAd->StaCfg.wdev.bw == wdev->bw)
			    && ((pAd->StaCfg.wdev.channel == wdev->channel))))
		   && (pAd->MCC_GOConnect_Protect == FALSE)
	    ) {
		Start_MCC(pAd);
	}
#endif /* CONFIG_MULTI_CHANNEL */
	pAd->cfg80211_ctrl.FlgCfg80211Scanning = FALSE;
#endif /* CONFIG_STA_SUPPORT */
}

VOID CFG80211_ScanStatusLockInit(IN VOID *pAdCB, IN UINT init)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	CFG80211_CB *pCfg80211_CB = (CFG80211_CB *) pAd->pCfg80211_CB;

	if (init) {
		NdisAllocateSpinLock(pAd, &pCfg80211_CB->scan_notify_lock);
	} else
		NdisFreeSpinLock(&pCfg80211_CB->scan_notify_lock);
}

#endif /* RT_CFG80211_SUPPORT */
