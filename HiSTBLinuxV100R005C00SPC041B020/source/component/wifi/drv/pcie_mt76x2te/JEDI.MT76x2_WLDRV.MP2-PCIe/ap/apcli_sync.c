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


#ifdef APCLI_SUPPORT

#include "rt_config.h"

static VOID ApCliProbeTimeout(IN PVOID SystemSpecific1,
			      IN PVOID FunctionContext,
			      IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);

static VOID ApCliMlmeProbeReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

static VOID ApCliPeerProbeRspAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

static VOID ApCliProbeTimeoutAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

static VOID ApCliInvalidStateWhenJoin(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

static VOID ApCliEnqueueProbeRequest(IN PRTMP_ADAPTER pAd,
				     IN UCHAR SsidLen, OUT PCHAR Ssid, IN USHORT ifIndex);
static VOID ApCliMlmeScanAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

DECLARE_TIMER_FUNCTION(ApCliProbeTimeout);
BUILD_TIMER_FUNCTION(ApCliProbeTimeout);

/*
    ==========================================================================
    Description:
	The sync state machine,
    Parameters:
	Sm - pointer to the state machine
    Note:
	the state machine looks like the following
    ==========================================================================
 */
VOID ApCliSyncStateMachineInit(IN PRTMP_ADAPTER pAd,
			       IN STATE_MACHINE * Sm, OUT STATE_MACHINE_FUNC Trans[])
{
	UCHAR i;
#ifdef APCLI_CONNECTION_TRIAL
	PAPCLI_STRUCT pApCliEntry;
#endif /*APCLI_CONNECTION_TRIAL */

	StateMachineInit(Sm, (STATE_MACHINE_FUNC *) Trans,
			 APCLI_MAX_SYNC_STATE, APCLI_MAX_SYNC_MSG,
			 (STATE_MACHINE_FUNC) Drop, APCLI_SYNC_IDLE, APCLI_SYNC_MACHINE_BASE);

	/* column 1 */
	StateMachineSetAction(Sm, APCLI_SYNC_IDLE, APCLI_MT2_MLME_PROBE_REQ,
			      (STATE_MACHINE_FUNC) ApCliMlmeProbeReqAction);
	StateMachineSetAction(Sm, APCLI_SYNC_IDLE, APCLI_MT2_PEER_PROBE_RSP,
				(STATE_MACHINE_FUNC)ApCliMlmeScanAction);
	/*column 2 */
	StateMachineSetAction(Sm, APCLI_JOIN_WAIT_PROBE_RSP, APCLI_MT2_MLME_PROBE_REQ,
			      (STATE_MACHINE_FUNC) ApCliInvalidStateWhenJoin);
	StateMachineSetAction(Sm, APCLI_JOIN_WAIT_PROBE_RSP, APCLI_MT2_PEER_PROBE_RSP,
			      (STATE_MACHINE_FUNC) ApCliPeerProbeRspAtJoinAction);
	StateMachineSetAction(Sm, APCLI_JOIN_WAIT_PROBE_RSP, APCLI_MT2_PROBE_TIMEOUT,
			      (STATE_MACHINE_FUNC) ApCliProbeTimeoutAtJoinAction);
	StateMachineSetAction(Sm, APCLI_JOIN_WAIT_PROBE_RSP, APCLI_MT2_PEER_BEACON,
			      (STATE_MACHINE_FUNC) ApCliPeerProbeRspAtJoinAction);


	for (i = 0; i < MAX_APCLI_NUM; i++) {
		/* timer init */
#ifdef APCLI_CONNECTION_TRIAL
		pApCliEntry = &pAd->ApCfg.ApCliTab[i];
#endif /*APCLI_CONNECTION_TRIAL */

#ifdef APCLI_CONNECTION_TRIAL
		RTMPInitTimer(pAd, &pAd->ApCfg.ApCliTab[i].MlmeAux.ProbeTimer,
			      GET_TIMER_FUNCTION(ApCliProbeTimeout), (PVOID) pApCliEntry, FALSE);
#else
		RTMPInitTimer(pAd, &pAd->ApCfg.ApCliTab[i].MlmeAux.ProbeTimer,
			      GET_TIMER_FUNCTION(ApCliProbeTimeout), pAd, FALSE);
#endif /* APCLI_CONNECTION_TRIAL */
		pAd->ApCfg.ApCliTab[i].SyncCurrState = APCLI_SYNC_IDLE;
	}
}

/*
    ==========================================================================
    Description:
	Becaon timeout handler, executed in timer thread
    ==========================================================================
 */
static VOID ApCliProbeTimeout(IN PVOID SystemSpecific1,
			      IN PVOID FunctionContext,
			      IN PVOID SystemSpecific2, IN PVOID SystemSpecific3)
{
#ifdef APCLI_CONNECTION_TRIAL
	PAPCLI_STRUCT pApCliEntry = (APCLI_STRUCT *) FunctionContext;
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pApCliEntry->pAd;
#else
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) FunctionContext;
#endif /*APCLI_CONNECTION_TRIAL */

	DBGPRINT(RT_DEBUG_TRACE, ("ApCli_SYNC - ProbeReqTimeout\n"));
#ifndef APCLI_CONNECTION_TRIAL
	MlmeEnqueue(pAd, APCLI_SYNC_STATE_MACHINE, APCLI_MT2_PROBE_TIMEOUT, 0, NULL, 0);
#else
	MlmeEnqueue(pAd, APCLI_SYNC_STATE_MACHINE, APCLI_MT2_PROBE_TIMEOUT, 0, NULL,
		    pApCliEntry->ifIndex);
#endif /* APCLI_CONNECTION_TRIAL */
	RTMP_MLME_HANDLER(pAd);
}

static VOID ApCliMlmeScanAction(
		IN PRTMP_ADAPTER pAd,
		IN MLME_QUEUE_ELEM * Elem)
{

	UCHAR *VarIE = NULL;
	USHORT LenVIE;
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	BCN_IE_LIST *ie_list = NULL;
	PFRAME_802_11 pFrame;

	DBGPRINT(RT_DEBUG_ERROR, ("ApCli SYNC - ApCliMlmeScanAction\n"));

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **)&ie_list, sizeof(BCN_IE_LIST));
	if (ie_list == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate ie_list fail!!!\n", __func__));
		return;
	}
	NdisZeroMemory(ie_list, sizeof(BCN_IE_LIST));

	/* Init Variable IE structure */
	os_alloc_mem(NULL, (UCHAR **)&VarIE, MAX_VIE_LEN);
	if (VarIE == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate VarIE fail!!!\n", __func__));
		if (ie_list != NULL)
			os_free_mem(NULL, ie_list);
		return;
	}
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;

	ie_list->Channel = Elem->Channel;
	pFrame = (PFRAME_802_11) Elem->Msg;

	if (PeerBeaconAndProbeRspSanity(pAd,
								Elem->Msg,
								Elem->MsgLen,
								Elem->Channel,
								ie_list,
								&LenVIE,
								pVIE)) {
		ULONG Idx = 0;
		CHAR Rssi = 0;

		Idx = BssTableSearch(&pAd->ScanTab, &ie_list->Bssid[0], ie_list->Channel);
		if (Idx != BSS_NOT_FOUND)
			Rssi = pAd->ScanTab.BssEntry[Idx].Rssi;

		Rssi = RTMPMaxRssi(pAd, ConvertToRssi(pAd, Elem->Rssi0, RSSI_0),
							ConvertToRssi(pAd, Elem->Rssi1, RSSI_1),
							ConvertToRssi(pAd, Elem->Rssi2, RSSI_2));

#ifdef DOT11_N_SUPPORT
		if ((ie_list->HtCapabilityLen > 0) || (ie_list->PreNHtCapabilityLen > 0))
			ie_list->HtCapabilityLen = SIZE_HT_CAP_IE;
#endif /* DOT11_N_SUPPORT */

		Idx = BssTableSetEntry(pAd, &pAd->ScanTab, ie_list, Rssi, NULL, LenVIE, pVIE);


		if (Idx != BSS_NOT_FOUND) {
			BSS_ENTRY *pBssEntry = &pAd->ScanTab.BssEntry[Idx];

			NdisCopyMemory(pBssEntry->PTSF, &Elem->Msg[24], 4);
			NdisCopyMemory(&pBssEntry->TTSF[0], &Elem->TimeStamp.u.LowPart, 4);
			NdisCopyMemory(&pBssEntry->TTSF[4], &Elem->TimeStamp.u.LowPart, 4);

			pBssEntry->MinSNR = Elem->Signal % 10;
			if (pBssEntry->MinSNR == 0)
				pBssEntry->MinSNR = -5;

			NdisCopyMemory(pBssEntry->MacAddr, &ie_list->Addr2[0], MAC_ADDR_LEN);

			if (LenVIE != 0) {
				pBssEntry->VarIeFromProbeRspLen = 0;
				if (pBssEntry->pVarIeFromProbRsp) {
					pBssEntry->VarIeFromProbeRspLen = LenVIE;
					RTMPZeroMemory(pBssEntry->pVarIeFromProbRsp, MAX_VIE_LEN);
					NdisCopyMemory(pBssEntry->pVarIeFromProbRsp, pVIE, LenVIE);
				}
			}
		}
#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
		if (ie_list->Channel != 0)
			Elem->Channel = ie_list->Channel;

		RT_CFG80211_SCANNING_INFORM(pAd, Idx, Elem->Channel, (UCHAR *)pFrame,
									Elem->MsgLen, Rssi);
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */
	}

	if (ie_list != NULL)
		os_free_mem(NULL, ie_list);
	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);

}

/*
    ==========================================================================
    Description:
	MLME PROBE req state machine procedure
    ==========================================================================
 */
static VOID ApCliMlmeProbeReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	BOOLEAN Cancelled;
	APCLI_MLME_JOIN_REQ_STRUCT *Info = (APCLI_MLME_JOIN_REQ_STRUCT *) (Elem->Msg);
	USHORT ifIndex = (USHORT) (Elem->Priv);
	PULONG pCurrState = &pAd->ApCfg.ApCliTab[ifIndex].SyncCurrState;
	APCLI_STRUCT *pApCliEntry = NULL;
	ULONG Timeout;
	ULONG bss_idx = BSS_NOT_FOUND;

	DBGPRINT(RT_DEBUG_TRACE, ("ApCli SYNC - ApCliMlmeProbeReqAction(Ssid %s)\n", Info->Ssid));

	if (ifIndex >= MAX_APCLI_NUM)
		return;

	pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];

	/* reset all the timers */
	RTMPCancelTimer(&(pApCliEntry->MlmeAux.ProbeTimer), &Cancelled);

	pApCliEntry->MlmeAux.Rssi = -9999;

	bss_idx = BssSsidTableSearchBySSID(&pAd->ScanTab, (PCHAR) Info->Ssid, Info->SsidLen);
	if (bss_idx == BSS_NOT_FOUND) {
#ifdef APCLI_CONNECTION_TRIAL
		if (pApCliEntry->TrialCh == 0)
			pApCliEntry->MlmeAux.Channel = pAd->CommonCfg.Channel;
		else
			pApCliEntry->MlmeAux.Channel = pApCliEntry->TrialCh;
#else
		pApCliEntry->MlmeAux.Channel = pAd->CommonCfg.Channel;
#endif /* APCLI_CONNECTION_TRIAL */
	} else {
#ifdef APCLI_CONNECTION_TRIAL
		if (pApCliEntry->TrialCh == 0)
			pApCliEntry->MlmeAux.Channel = pAd->CommonCfg.Channel;
		else
			pApCliEntry->MlmeAux.Channel = pApCliEntry->TrialCh;
#else
		DBGPRINT(RT_DEBUG_TRACE, ("%s, Found %s in scanTable , goto channel %d\n",
					  __func__, pAd->ScanTab.BssEntry[bss_idx].Ssid,
					  pAd->ScanTab.BssEntry[bss_idx].Channel));

		pApCliEntry->MlmeAux.Channel = pAd->ScanTab.BssEntry[bss_idx].Channel;
#endif /* APCLI_CONNECTION_TRIAL */
	}

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	pApCliEntry->MlmeAux.SupRateLen = pAd->cfg80211_ctrl.P2pSupRateLen;
	NdisMoveMemory(pApCliEntry->MlmeAux.SupRate, pAd->cfg80211_ctrl.P2pSupRate,
		       pAd->cfg80211_ctrl.P2pSupRateLen);

	pApCliEntry->MlmeAux.ExtRateLen = pAd->cfg80211_ctrl.P2pExtRateLen;
	NdisMoveMemory(pApCliEntry->MlmeAux.ExtRate, pAd->cfg80211_ctrl.P2pExtRate,
		       pAd->cfg80211_ctrl.P2pExtRateLen);
#else
	pApCliEntry->MlmeAux.SupRateLen = pAd->CommonCfg.SupRateLen;
	NdisMoveMemory(pApCliEntry->MlmeAux.SupRate, pAd->CommonCfg.SupRate,
		       pAd->CommonCfg.SupRateLen);

	/* Prepare the default value for extended rate */
	pApCliEntry->MlmeAux.ExtRateLen = pAd->CommonCfg.ExtRateLen;
	NdisMoveMemory(pApCliEntry->MlmeAux.ExtRate, pAd->CommonCfg.ExtRate,
		       pAd->CommonCfg.ExtRateLen);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	Timeout = ((pApCliEntry->ProbeReqCnt <= 2) ?
		APCLI_PROBE_LITTLE_TIMEOUT : APCLI_PROBE_TIMEOUT);
	RTMPSetTimer(&(pApCliEntry->MlmeAux.ProbeTimer), Timeout);


	NdisZeroMemory(pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Bssid, MAC_ADDR_LEN);
	NdisZeroMemory(pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Ssid, MAX_LEN_OF_SSID);
	NdisCopyMemory(pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Bssid,
		       pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid, MAC_ADDR_LEN);
	NdisCopyMemory(pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.Ssid,
		       pAd->ApCfg.ApCliTab[ifIndex].CfgSsid,
		       pAd->ApCfg.ApCliTab[ifIndex].CfgSsidLen);
	pAd->ApCfg.ApCliTab[ifIndex].MlmeAux.SsidLen = pAd->ApCfg.ApCliTab[ifIndex].CfgSsidLen;


	ApCliEnqueueProbeRequest(pAd, Info->SsidLen, (PCHAR) Info->Ssid, ifIndex);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("ApCli SYNC - Start Probe the SSID %s on channel =%d\n",
		  pApCliEntry->MlmeAux.Ssid, pApCliEntry->MlmeAux.Channel));

	*pCurrState = APCLI_JOIN_WAIT_PROBE_RSP;
}

/*
    ==========================================================================
    Description:
	When waiting joining the (I)BSS, beacon received from external
    ==========================================================================
 */
static VOID ApCliPeerProbeRspAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	USHORT LenVIE;
	UCHAR *VarIE = NULL;
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	APCLI_CTRL_MSG_STRUCT ApCliCtrlMsg;
	PAPCLI_STRUCT pApCliEntry = NULL;
	struct wifi_dev *wdev;
	UCHAR CentralChannel = 0;
	USHORT ifIndex = (USHORT) (Elem->Priv);
	ULONG *pCurrState;
	BCN_IE_LIST *ie_list = NULL;

	if (ifIndex >= MAX_APCLI_NUM)
		return;

	/* Init Variable IE structure */
	os_alloc_mem(NULL, (UCHAR **) &VarIE, MAX_VIE_LEN);
	if (VarIE == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __func__));
		goto LabelErr;
	}
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;

	os_alloc_mem(NULL, (UCHAR **) &ie_list, sizeof(BCN_IE_LIST));
	if (ie_list == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate ie_list fail!!!\n", __func__));
		goto LabelErr;
	}
	NdisZeroMemory(ie_list, sizeof(BCN_IE_LIST));

	pCurrState = &pAd->ApCfg.ApCliTab[ifIndex].SyncCurrState;
	if (PeerBeaconAndProbeRspSanity(pAd,
					Elem->Msg,
					Elem->MsgLen, Elem->Channel, ie_list, &LenVIE, pVIE)) {
		/*
		   BEACON from desired BSS/IBSS found. We should be able to decide most
		   BSS parameters here.
		   Q. But what happen if this JOIN doesn't conclude a successful ASSOCIATEION?
		   Do we need to receover back all parameters belonging to previous BSS?
		   A. Should be not. There's no back-door recover to previous AP. It still need
		   a new JOIN-AUTH-ASSOC sequence.
		 */
		INT ssidEqualFlag = FALSE;
		INT ssidEmptyFlag = FALSE;
		INT bssidEqualFlag = FALSE;
		INT bssidEmptyFlag = FALSE;
		INT matchFlag = FALSE;

		ULONG Bssidx;
		CHAR RealRssi = -127;

		RealRssi = (LONG) (RTMPMaxRssi(pAd, ConvertToRssi(pAd, Elem->Rssi0, RSSI_0),
					       ConvertToRssi(pAd, Elem->Rssi1, RSSI_1),
					       ConvertToRssi(pAd, Elem->Rssi2, RSSI_2)));

		/* Update ScanTab */
		Bssidx = BssTableSearch(&pAd->ScanTab, ie_list->Bssid, ie_list->Channel);
		if (Bssidx == BSS_NOT_FOUND) {
			/* discover new AP of this network, create BSS entry */
			Bssidx =
			    BssTableSetEntry(pAd, &pAd->ScanTab, ie_list, -127, NULL, LenVIE, pVIE);

			/* return if BSS table full */
			if ((Bssidx == BSS_NOT_FOUND) || (Bssidx >= MAX_LEN_OF_BSS_TABLE)) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("ERROR: Driver ScanTable Full In Apcli ProbeRsp Join\n"));
				goto LabelErr;
			}

			NdisMoveMemory(pAd->ScanTab.BssEntry[Bssidx].PTSF, &Elem->Msg[24], 4);
			NdisMoveMemory(&pAd->ScanTab.BssEntry[Bssidx].TTSF[0],
				       &Elem->TimeStamp.u.LowPart, 4);
			NdisMoveMemory(&pAd->ScanTab.BssEntry[Bssidx].TTSF[4],
				       &Elem->TimeStamp.u.LowPart, 4);
			pAd->ScanTab.BssEntry[Bssidx].MinSNR = Elem->Signal % 10;
			if (pAd->ScanTab.BssEntry[Bssidx].MinSNR == 0)
				pAd->ScanTab.BssEntry[Bssidx].MinSNR = -5;

			NdisMoveMemory(pAd->ScanTab.BssEntry[Bssidx].MacAddr, ie_list->Addr2,
				       MAC_ADDR_LEN);
		}
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Info: Update the SSID %s in Kernel Table\n", ie_list->Ssid));
		if (ie_list->Channel != 0)
			Elem->Channel = ie_list->Channel;

		RT_CFG80211_SCANNING_INFORM(pAd, Bssidx, Elem->Channel /*ie_list->Channel */ ,
					    (UCHAR *) Elem->Msg, Elem->MsgLen, RealRssi);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

		pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];
		wdev = &pApCliEntry->wdev;

		/* Check the Probe-Rsp's Bssid. */
		if (!MAC_ADDR_EQUAL(pApCliEntry->CfgApCliBssid, ZERO_MAC_ADDR))
			bssidEqualFlag = MAC_ADDR_EQUAL(pApCliEntry->CfgApCliBssid, ie_list->Bssid);
		else
			bssidEmptyFlag = TRUE;

		/* Check the Probe-Rsp's Ssid. */
		if (pApCliEntry->CfgSsidLen != 0)
			ssidEqualFlag =
			    SSID_EQUAL(pApCliEntry->CfgSsid, pApCliEntry->CfgSsidLen, ie_list->Ssid,
				       ie_list->SsidLen);
		else
			ssidEmptyFlag = TRUE;

		/* bssid and ssid, Both match. */
		if (bssidEqualFlag && ssidEqualFlag)
			matchFlag = TRUE;

		/* ssid match but bssid doesn't be indicate. */
		else if (ssidEqualFlag && bssidEmptyFlag)
			matchFlag = TRUE;

		/* user doesn't indicate any bssid or ssid. AP-Clinet will auto pick a AP to join by most
		strong siganl strength. */
		else if (bssidEmptyFlag && ssidEmptyFlag)
			matchFlag = TRUE;

		DBGPRINT(RT_DEBUG_TRACE,
			 ("SYNC - bssidEqualFlag=%d, ssidEqualFlag=%d, matchFlag=%d\n",
			  bssidEqualFlag, ssidEqualFlag, matchFlag));
		if (matchFlag) {
			/* Validate RSN IE if necessary, then copy store this information */
			if ((LenVIE > 0)
#ifdef WSC_AP_SUPPORT
			    && ((pApCliEntry->WscControl.WscConfMode == WSC_DISABLE) ||
				(pApCliEntry->WscControl.bWscTrigger == FALSE))
#endif /* WSC_AP_SUPPORT */
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
			    /* When using CFG80211 and trigger WPS, do not check security. */
			    && !(pApCliEntry->wpa_supplicant_info.
				 WpaSupplicantUP & WPA_SUPPLICANT_ENABLE_WPS)
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
			    ) {
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
				if (1)
#else
			if (ApCliValidateRSNIE(pAd, (PEID_STRUCT) pVIE, LenVIE, ifIndex))
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
				{
					pApCliEntry->MlmeAux.VarIELen = LenVIE;
					NdisMoveMemory(pApCliEntry->MlmeAux.VarIEs, pVIE,
						       pApCliEntry->MlmeAux.VarIELen);
				} else {
					/* ignore this response */
					pApCliEntry->MlmeAux.VarIELen = 0;
					DBGPRINT(RT_DEBUG_ERROR,
	("ERROR: The RSN IE of this received Probe-resp is dis-match !!!!!!!!!!\n"));
					goto LabelErr;
				}
			} else {
				if (pApCliEntry->wdev.AuthMode >= Ndis802_11AuthModeWPA
#ifdef WSC_AP_SUPPORT
				    && ((pApCliEntry->WscControl.WscConfMode == WSC_DISABLE) ||
					(pApCliEntry->WscControl.bWscTrigger == FALSE))
#endif /* WSC_AP_SUPPORT */
				    ) {
					/* ignore this response */
					DBGPRINT(RT_DEBUG_ERROR,
						 ("ERROR: The received Probe-resp has empty RSN IE !!!!!!!!!!\n"));
					goto LabelErr;
				}

				pApCliEntry->MlmeAux.VarIELen = 0;
			}

			DBGPRINT(RT_DEBUG_OFF,
				 ("SYNC - receive desired PROBE_RSP at JoinWaitProbeRsp... Channel = %d\n",
				  ie_list->Channel));

			/* if the Bssid doesn't be indicated then you need to decide which AP to connect
			by most strong Rssi signal strength. */
			if (bssidEqualFlag == FALSE) {
				/* caculate real rssi value. */
				CHAR Rssi0 = ConvertToRssi(pAd, Elem->Rssi0, RSSI_0);
				CHAR Rssi1 = ConvertToRssi(pAd, Elem->Rssi1, RSSI_1);
				CHAR Rssi2 = ConvertToRssi(pAd, Elem->Rssi2, RSSI_2);
				LONG RealRssi = (LONG) (RTMPMaxRssi(pAd, Rssi0, Rssi1, Rssi2));

				DBGPRINT(RT_DEBUG_TRACE,
					 ("SYNC - previous Rssi = %ld current Rssi=%ld\n",
					  pApCliEntry->MlmeAux.Rssi, (LONG) RealRssi));
				if (pApCliEntry->MlmeAux.Rssi > (LONG) RealRssi)
					goto LabelErr;
				else
					pApCliEntry->MlmeAux.Rssi = RealRssi;
			} else {
				BOOLEAN Cancelled;

				RTMPCancelTimer(&pApCliEntry->MlmeAux.ProbeTimer, &Cancelled);
			}

			NdisMoveMemory(pApCliEntry->MlmeAux.Ssid, ie_list->Ssid, ie_list->SsidLen);
			pApCliEntry->MlmeAux.SsidLen = ie_list->SsidLen;

			NdisMoveMemory(pApCliEntry->MlmeAux.Bssid, ie_list->Bssid, MAC_ADDR_LEN);
			pApCliEntry->MlmeAux.CapabilityInfo =
			    ie_list->CapabilityInfo & SUPPORTED_CAPABILITY_INFO;
			pApCliEntry->MlmeAux.BssType = ie_list->BssType;
			pApCliEntry->MlmeAux.BeaconPeriod = ie_list->BeaconPeriod;
			pApCliEntry->MlmeAux.Channel = ie_list->Channel;
			pApCliEntry->MlmeAux.AtimWin = ie_list->AtimWin;
			pApCliEntry->MlmeAux.CfpPeriod = ie_list->CfParm.CfpPeriod;
			pApCliEntry->MlmeAux.CfpMaxDuration = ie_list->CfParm.CfpMaxDuration;
			pApCliEntry->MlmeAux.APRalinkIe = ie_list->RalinkIe;

			/* Copy AP's supported rate to MlmeAux for creating assoication request */
			/* Also filter out not supported rate */
			pApCliEntry->MlmeAux.SupRateLen = ie_list->SupRateLen;
			NdisMoveMemory(pApCliEntry->MlmeAux.SupRate, ie_list->SupRate,
				       ie_list->SupRateLen);
			RTMPCheckRates(pAd, pApCliEntry->MlmeAux.SupRate,
				       &pApCliEntry->MlmeAux.SupRateLen);
			pApCliEntry->MlmeAux.ExtRateLen = ie_list->ExtRateLen;
			NdisMoveMemory(pApCliEntry->MlmeAux.ExtRate, ie_list->ExtRate,
				       ie_list->ExtRateLen);
			RTMPCheckRates(pAd, pApCliEntry->MlmeAux.ExtRate,
				       &pApCliEntry->MlmeAux.ExtRateLen);
#ifdef APCLI_CERT_SUPPORT
			/*  Get the ext capability info element */
			if (pAd->bApCliCertTest == TRUE && pAd->CommonCfg.bBssCoexEnable == TRUE) {
				NdisMoveMemory(&pApCliEntry->MlmeAux.ExtCapInfo,
					       &ie_list->ExtCapInfo, sizeof(ie_list->ExtCapInfo));

				/* zero debug 210121122 */
				DBGPRINT(RT_DEBUG_TRACE,
					("\x1b[31m ApCliMlmeAux.ExtCapInfo=%d \x1b[m\n",
					pApCliEntry->MlmeAux.ExtCapInfo.BssCoexistMgmtSupport));
				pAd->CommonCfg.ExtCapIE.BssCoexistMgmtSupport = 1;
			}
#endif /* APCLI_CERT_SUPPORT */
			NdisZeroMemory(pApCliEntry->RxMcsSet, sizeof(pApCliEntry->RxMcsSet));
			/* filter out un-supported ht rates */
			DBGPRINT(RT_DEBUG_TRACE,
				 ("=====================> %d, %d, %d, %d\n",
				  pApCliEntry->MlmeAux.BeaconPeriod, ie_list->HtCapabilityLen,
				  pApCliEntry->wdev.DesiredHtPhyInfo.bHtEnable,
				  WMODE_CAP_N(pAd->CommonCfg.PhyMode)));

#ifdef DOT11_VHT_AC
			pAd->StaActive.SupportedPhyInfo.bVhtEnable = FALSE;
			pAd->StaActive.SupportedPhyInfo.vht_bw = VHT_BW_2040;
#endif /* DOT11_VHT_AC */

			if ((ie_list->HtCapabilityLen > 0)
			    && (pApCliEntry->wdev.DesiredHtPhyInfo.bHtEnable)
			    && WMODE_CAP_N(pAd->CommonCfg.PhyMode) &&
			    /* For Dissallow TKIP rule on STA */
			    !(pAd->CommonCfg.HT_DisallowTKIP
			      && IS_INVALID_HT_SECURITY(wdev->WepStatus))) {

				UINT8 P2PGroup_BW;
				UCHAR BwFallBack = 0;

				RTMPMoveMemory(&pApCliEntry->MlmeAux.AddHtInfo, &ie_list->AddHtInfo,
						SIZE_ADD_HT_INFO_IE);

				RTMPZeroMemory(&pApCliEntry->MlmeAux.HtCapability, SIZE_HT_CAP_IE);
				pApCliEntry->MlmeAux.NewExtChannelOffset =
				    ie_list->NewExtChannelOffset;
				pApCliEntry->MlmeAux.HtCapabilityLen = ie_list->HtCapabilityLen;
				ApCliCheckHt(pAd, ifIndex, &ie_list->HtCapability,
					     &ie_list->AddHtInfo);

				if (ie_list->AddHtInfoLen > 0) {
					CentralChannel = ie_list->AddHtInfo.ControlChan;
					/* Check again the Bandwidth capability of this AP. */
					CentralChannel =
					    get_cent_ch_by_htinfo(pAd, &ie_list->AddHtInfo,
								  &ie_list->HtCapability);
					DBGPRINT(RT_DEBUG_TRACE,
						 ("APCLI PeerBeaconAtJoinAction HT===>CentralCh = %d, ControlCh = %d\n",
						  CentralChannel, ie_list->AddHtInfo.ControlChan));
				}

#ifdef DOT11_VHT_AC
				DBGPRINT(RT_DEBUG_TRACE,
					("vht_op_len: %d, vht_cap_len: %d, mode: %d, ch: %d\n",
					ie_list->vht_op_len,
					ie_list->vht_cap_len,
					WMODE_CAP_AC(pAd->CommonCfg.PhyMode),
					pApCliEntry->MlmeAux.Channel));

				if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode)
				&& (pApCliEntry->MlmeAux.Channel > 14)
				&& (ie_list->vht_cap_len) && (ie_list->vht_op_len)) {
					VHT_OP_INFO *vht_op = &ie_list->vht_op_ie.vht_op_info;

					RTMPZeroMemory(&pAd->MlmeAux.vht_cap, sizeof(VHT_CAP_IE));
					NdisMoveMemory(&pAd->MlmeAux.vht_cap,
						&ie_list->vht_cap_ie, ie_list->vht_cap_len);
					pAd->MlmeAux.vht_cap_len = ie_list->vht_cap_len;

					RTMPZeroMemory(&pAd->MlmeAux.vht_op, sizeof(VHT_OP_IE));
					NdisMoveMemory(&pAd->MlmeAux.vht_op,
						&ie_list->vht_op_ie, ie_list->vht_op_len);
					pAd->MlmeAux.vht_op_len = ie_list->vht_op_len;

					RTMPZeroMemory(&pApCliEntry->MlmeAux.vht_cap,
						sizeof(VHT_CAP_IE));
					NdisMoveMemory(&pApCliEntry->MlmeAux.vht_cap,
						&ie_list->vht_cap_ie, ie_list->vht_cap_len);
					pApCliEntry->MlmeAux.vht_cap_len = ie_list->vht_cap_len;

					RTMPZeroMemory(&pApCliEntry->MlmeAux.vht_op,
						sizeof(VHT_OP_IE));
					NdisMoveMemory(&pApCliEntry->MlmeAux.vht_op,
						&ie_list->vht_op_ie, ie_list->vht_op_len);
					pApCliEntry->MlmeAux.vht_op_len = ie_list->vht_op_len;

					pAd->StaActive.SupportedPhyInfo.bVhtEnable = TRUE;

					if (vht_op->ch_width == 0) {
						DBGPRINT(RT_DEBUG_TRACE, ("VHT_BW_2040\n"));
						pAd->StaActive.SupportedPhyInfo.vht_bw =
							VHT_BW_2040;
					} else if (vht_op->ch_width == 1) {
						DBGPRINT(RT_DEBUG_TRACE, ("BW_80\n"));
						CentralChannel = vht_op->center_freq_1;
						pAd->StaActive.SupportedPhyInfo.vht_bw = VHT_BW_80;
						DBGPRINT(RT_DEBUG_TRACE, ("CentralChannel: %d\n",
							CentralChannel));
					}

					DBGPRINT(RT_DEBUG_TRACE, ("%s(): CentralChannel=>%d\n",
							__func__, CentralChannel));
				}
#endif /* DOT11_VHT_AC */

				pApCliEntry->MlmeAux.CentralChannel = CentralChannel;
				/* P2PInitChannelRelatedValue(pAd); */
				if (pApCliEntry->MlmeAux.HtCapability.HtCapInfo.ChannelWidth ==
				    BW_40)
					P2PGroup_BW = BW_40;
				else
					P2PGroup_BW = BW_20;

				AdjustChannelRelatedValue(pAd,
							  &BwFallBack,
							  ifIndex,
							  P2PGroup_BW,
							  pApCliEntry->MlmeAux.Channel,
							  pApCliEntry->MlmeAux.CentralChannel);

				if (BwFallBack == 1) {
					DBGPRINT(RT_DEBUG_TRACE,
	("Infra STA connection to 40MHz AP, but Infra extra and P2P Group extra is different!!!\n"));
					pApCliEntry->MlmeAux.HtCapability.HtCapInfo.ChannelWidth =
					    BW_20;
					pApCliEntry->MlmeAux.CentralChannel =
					    pApCliEntry->MlmeAux.Channel;
					pApCliEntry->MlmeAux.bBwFallBack = TRUE;
				} else {
					pApCliEntry->MlmeAux.bBwFallBack = FALSE;
				}

				pApCliEntry->MlmeAux.ConCurrentCentralChannel =
				    pAd->CommonCfg.CentralChannel;
			} else {
				RTMPZeroMemory(&pApCliEntry->MlmeAux.HtCapability, SIZE_HT_CAP_IE);
				RTMPZeroMemory(&pApCliEntry->MlmeAux.AddHtInfo,
					       SIZE_ADD_HT_INFO_IE);
				pApCliEntry->MlmeAux.HtCapabilityLen = 0;
			}
			ApCliUpdateMlmeRate(pAd, ifIndex);

			/* copy QOS related information */
			if (WMODE_CAP_N(pAd->CommonCfg.PhyMode)) {
				NdisMoveMemory(&pApCliEntry->MlmeAux.APEdcaParm, &ie_list->EdcaParm,
					       sizeof(EDCA_PARM));
				NdisMoveMemory(&pApCliEntry->MlmeAux.APQbssLoad, &ie_list->QbssLoad,
					       sizeof(QBSS_LOAD_PARM));
				NdisMoveMemory(&pApCliEntry->MlmeAux.APQosCapability,
					       &ie_list->QosCapability,
					       sizeof(QOS_CAPABILITY_PARM));
			} else {
				NdisZeroMemory(&pApCliEntry->MlmeAux.APEdcaParm, sizeof(EDCA_PARM));
				NdisZeroMemory(&pApCliEntry->MlmeAux.APQbssLoad,
					       sizeof(QBSS_LOAD_PARM));
				NdisZeroMemory(&pApCliEntry->MlmeAux.APQosCapability,
					       sizeof(QOS_CAPABILITY_PARM));
			}

			DBGPRINT(RT_DEBUG_TRACE,
				 ("APCLI SYNC - after JOIN, SupRateLen=%d, ExtRateLen=%d\n",
				  pApCliEntry->MlmeAux.SupRateLen,
				  pApCliEntry->MlmeAux.ExtRateLen));

			if (ie_list->AironetCellPowerLimit != 0xFF) {
				/* We need to change our TxPower for CCX 2.0 AP Control of Client Transmit Power */
				ChangeToCellPowerLimit(pAd, ie_list->AironetCellPowerLimit);
			} else	/* Used the default TX Power Percentage. */
				pAd->CommonCfg.TxPowerPercentage = pAd->CommonCfg.TxPowerDefault;

#ifdef WSC_AP_SUPPORT
			if ((pApCliEntry->WscControl.WscConfMode != WSC_DISABLE) &&
			    (pApCliEntry->WscControl.bWscTrigger == TRUE)) {
				ADD_HTINFO RootApHtInfo, ApHtInfo;

				ApHtInfo = pAd->CommonCfg.AddHTInfo.AddHtInfo;
				RootApHtInfo = ie_list->AddHtInfo.AddHtInfo;
				if ((pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth == BW_40) &&
				    (RootApHtInfo.RecomWidth) &&
				    (RootApHtInfo.ExtChanOffset != ApHtInfo.ExtChanOffset)) {
					if (RootApHtInfo.ExtChanOffset == EXTCHA_ABOVE)
						Set_HtExtcha_Proc(pAd, "1");
					else
						Set_HtExtcha_Proc(pAd, "0");

					goto LabelErr;
				}
			}
#endif /* WSC_AP_SUPPORT */
			if (bssidEqualFlag == TRUE) {
				*pCurrState = APCLI_SYNC_IDLE;

				ApCliCtrlMsg.Status = MLME_SUCCESS;
#ifdef MAC_REPEATER_SUPPORT
				ApCliCtrlMsg.BssIdx = ifIndex;
				ApCliCtrlMsg.CliIdx = 0xFF;
#endif /* MAC_REPEATER_SUPPORT */

				MlmeEnqueue(pAd, APCLI_CTRL_STATE_MACHINE, APCLI_CTRL_PROBE_RSP,
					    sizeof(APCLI_CTRL_MSG_STRUCT), &ApCliCtrlMsg, ifIndex);
			}
		}
	}

LabelErr:
	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);
	if (ie_list != NULL)
		os_free_mem(NULL, ie_list);
}

static VOID ApCliProbeTimeoutAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	USHORT ifIndex = (USHORT) (Elem->Priv);
	PULONG pCurrState = &pAd->ApCfg.ApCliTab[ifIndex].SyncCurrState;
	APCLI_STRUCT *pApCliEntry = NULL;

	DBGPRINT(RT_DEBUG_OFF, ("APCLI_SYNC - ProbeTimeoutAtJoinAction\n"));

	if (ifIndex >= MAX_APCLI_NUM)
		return;

	pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];
	*pCurrState = SYNC_IDLE;

#ifdef APCLI_CONNECTION_TRIAL
	if (ifIndex == 1)
		*pCurrState = APCLI_CTRL_DISCONNECTED;
#endif /* APCLI_CONNECTION_TRIAL */

	DBGPRINT(RT_DEBUG_TRACE, ("APCLI_SYNC - MlmeAux.Bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
				  PRINT_MAC(pApCliEntry->MlmeAux.Bssid)));

	MlmeEnqueue(pAd, APCLI_CTRL_STATE_MACHINE, APCLI_CTRL_JOIN_REQ_TIMEOUT, 0, NULL,
			    ifIndex);
}

/*
    ==========================================================================
    Description:
    ==========================================================================
 */
static VOID ApCliInvalidStateWhenJoin(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	APCLI_CTRL_MSG_STRUCT ApCliCtrlMsg;
	USHORT ifIndex = (USHORT) (Elem->Priv);
	PULONG pCurrState = &pAd->ApCfg.ApCliTab[ifIndex].SyncCurrState;

	*pCurrState = APCLI_SYNC_IDLE;
	ApCliCtrlMsg.Status = MLME_STATE_MACHINE_REJECT;
#ifdef MAC_REPEATER_SUPPORT
	ApCliCtrlMsg.BssIdx = ifIndex;
	ApCliCtrlMsg.CliIdx = 0xFF;
#endif /* MAC_REPEATER_SUPPORT */

	MlmeEnqueue(pAd, APCLI_CTRL_STATE_MACHINE, APCLI_CTRL_PROBE_RSP,
		    sizeof(APCLI_CTRL_MSG_STRUCT), &ApCliCtrlMsg, ifIndex);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("APCLI_AYNC - ApCliInvalidStateWhenJoin(state=%ld). Reset SYNC machine\n",
		  *pCurrState));
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
static VOID ApCliEnqueueProbeRequest(IN PRTMP_ADAPTER pAd,
				     IN UCHAR SsidLen, OUT PCHAR Ssid, IN USHORT ifIndex)
{
	NDIS_STATUS NState;
	PUCHAR pOutBuffer;
	ULONG FrameLen = 0;
	HEADER_802_11 Hdr80211;
	UCHAR SsidIe = IE_SSID;
	UCHAR SupRateIe = IE_SUPP_RATES;
	UCHAR ssidLen;
	CHAR ssid[MAX_LEN_OF_SSID];
	APCLI_STRUCT *pApCliEntry = NULL;

	DBGPRINT(RT_DEBUG_TRACE, ("force out a ProbeRequest ...\n"));

	if (ifIndex >= MAX_APCLI_NUM)
		return;

	pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];

	NState = MlmeAllocateMemory(pAd, &pOutBuffer);	/* Get an unused nonpaged memory */
	if (NState != NDIS_STATUS_SUCCESS) {
		DBGPRINT(RT_DEBUG_TRACE, ("EnqueueProbeRequest() allocate memory fail\n"));
		return;
	}

	if (MAC_ADDR_EQUAL(pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid, ZERO_MAC_ADDR))
		ApCliMgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
				      BROADCAST_ADDR, BROADCAST_ADDR, ifIndex);
	else {
#ifdef CERTIFICATION_SIGMA_SUPPORT
		if (pAd->cfg80211_ctrl.bSigmaEnabledFlag) {
			/* Fix WFD Certification 6.1.21B, broadcom will not reply probe response if
			we send the probe request by unicast */
			DBGPRINT(RT_DEBUG_TRACE,
				 ("[apcli_sync] force out a ProbeRequest by broadcast but not unicast...\n"));
			ApCliMgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
					      BROADCAST_ADDR, BROADCAST_ADDR, ifIndex);
		} else
#endif /* CERTIFICATION_SIGMA_SUPPORT */
			ApCliMgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
					      pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid,
					      pAd->ApCfg.ApCliTab[ifIndex].CfgApCliBssid,
					      ifIndex);

	}

	ssidLen = SsidLen;
	NdisZeroMemory(ssid, MAX_LEN_OF_SSID);
	NdisMoveMemory(ssid, Ssid, ssidLen);

	/* this ProbeRequest explicitly specify SSID to reduce unwanted ProbeResponse */
	MakeOutgoingFrame(pOutBuffer, &FrameLen,
			  sizeof(HEADER_802_11), &Hdr80211,
			  1, &SsidIe,
			  1, &ssidLen,
			  ssidLen, ssid,
			  1, &SupRateIe,
			  1, &pApCliEntry->MlmeAux.SupRateLen,
			  pApCliEntry->MlmeAux.SupRateLen, pApCliEntry->MlmeAux.SupRate,
			  END_OF_ARGS);

	/* Add the extended rate IE */
	if (pApCliEntry->MlmeAux.ExtRateLen != 0) {
		ULONG tmp;

		MakeOutgoingFrame(pOutBuffer + FrameLen, &tmp,
				  1, &ExtRateIe,
				  1, &pApCliEntry->MlmeAux.ExtRateLen,
				  pApCliEntry->MlmeAux.ExtRateLen,
				  pApCliEntry->MlmeAux.ExtRate, END_OF_ARGS);
		FrameLen += tmp;
	}

	/* Add HT Cap IE if needed */
	if (WMODE_CAP_N(pAd->CommonCfg.PhyMode)) {
		ULONG Tmp;
		UCHAR HtLen;
		UCHAR BROADCOM[4] = { 0x0, 0x90, 0x4c, 0x33 };
#ifdef RT_BIG_ENDIAN
		HT_CAPABILITY_IE HtCapabilityTmp;
#endif /* endif */
		if (pAd->bBroadComHT == TRUE) {
			HtLen = pAd->MlmeAux.HtCapabilityLen + 4;
#ifdef RT_BIG_ENDIAN
			NdisMoveMemory(&HtCapabilityTmp, &pAd->MlmeAux.HtCapability,
				       SIZE_HT_CAP_IE);
			*(USHORT *) (&HtCapabilityTmp.HtCapInfo) =
			    SWAP16(*(USHORT *) (&HtCapabilityTmp.HtCapInfo));
#ifdef UNALIGNMENT_SUPPORT
			{
				EXT_HT_CAP_INFO extHtCapInfo;

				NdisMoveMemory((PUCHAR) (&extHtCapInfo),
					       (PUCHAR) (&HtCapabilityTmp.ExtHtCapInfo),
					       sizeof(EXT_HT_CAP_INFO));
				*(USHORT *) (&extHtCapInfo) =
				    cpu2le16(*(USHORT *) (&extHtCapInfo));
				NdisMoveMemory((PUCHAR) (&HtCapabilityTmp.ExtHtCapInfo),
					       (PUCHAR) (&extHtCapInfo),
					       sizeof(EXT_HT_CAP_INFO));
			}
#else
			*(USHORT *) (&HtCapabilityTmp.ExtHtCapInfo) =
			    cpu2le16(*(USHORT *) (&HtCapabilityTmp.ExtHtCapInfo));
#endif /* UNALIGNMENT_SUPPORT */

			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
					  1, &WpaIe,
					  1, &HtLen,
					  4, &BROADCOM[0],
					  pAd->MlmeAux.HtCapabilityLen, &HtCapabilityTmp,
					  END_OF_ARGS);
#else
			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
					  1, &WpaIe,
					  1, &HtLen,
					  4, &BROADCOM[0],
					  pAd->MlmeAux.HtCapabilityLen,
					  &pAd->MlmeAux.HtCapability, END_OF_ARGS);
#endif /* RT_BIG_ENDIAN */
		} else {
			HtLen = sizeof(HT_CAPABILITY_IE);
#ifdef RT_BIG_ENDIAN
			NdisMoveMemory(&HtCapabilityTmp, &pAd->CommonCfg.HtCapability,
				       SIZE_HT_CAP_IE);
			*(USHORT *) (&HtCapabilityTmp.HtCapInfo) =
			    SWAP16(*(USHORT *) (&HtCapabilityTmp.HtCapInfo));
#ifdef UNALIGNMENT_SUPPORT
			{
				EXT_HT_CAP_INFO extHtCapInfo;

				NdisMoveMemory((PUCHAR) (&extHtCapInfo),
					       (PUCHAR) (&HtCapabilityTmp.ExtHtCapInfo),
					       sizeof(EXT_HT_CAP_INFO));
				*(USHORT *) (&extHtCapInfo) =
				    cpu2le16(*(USHORT *) (&extHtCapInfo));
				NdisMoveMemory((PUCHAR) (&HtCapabilityTmp.ExtHtCapInfo),
					       (PUCHAR) (&extHtCapInfo),
					       sizeof(EXT_HT_CAP_INFO));
			}
#else
			*(USHORT *) (&HtCapabilityTmp.ExtHtCapInfo) =
			    cpu2le16(*(USHORT *) (&HtCapabilityTmp.ExtHtCapInfo));
#endif /* UNALIGNMENT_SUPPORT */

			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
					  1, &HtCapIe,
					  1, &HtLen, HtLen, &HtCapabilityTmp, END_OF_ARGS);
#else
			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
					  1, &HtCapIe,
					  1, &HtLen,
					  HtLen, &pAd->CommonCfg.HtCapability, END_OF_ARGS);
#endif /* RT_BIG_ENDIAN */
		}
		FrameLen += Tmp;

		if ((pAd->MlmeAux.Channel <= 14) && (pAd->CommonCfg.bBssCoexEnable == TRUE)) {
			ULONG Tmp;

			HtLen = 1;
			MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
					  1, &ExtHtCapIe,
					  1, &HtLen,
					  1, &pAd->CommonCfg.BSSCoexist2040.word,
					  END_OF_ARGS);

			FrameLen += Tmp;
		}
	}
#ifdef DOT11_VHT_AC
	if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode) && (pAd->CommonCfg.Channel > 14)) {
		build_vht_cap_ie(pAd, (UCHAR *) &pApCliEntry->MlmeAux.vht_cap);
		pApCliEntry->MlmeAux.vht_cap_len = sizeof(VHT_CAP_IE);
		FrameLen +=
		    build_vht_ies(pAd, (UCHAR *) (pOutBuffer + FrameLen),
				  SUBTYPE_PROBE_REQ);
	}
#endif /* DOT11_VHT_AC */

#ifdef WSC_AP_SUPPORT
/* Append WSC information in probe request if WSC state is running */
	if ((pAd->ApCfg.ApCliTab[ifIndex].WscControl.WscConfMode != WSC_DISABLE) &&
	    (pAd->ApCfg.ApCliTab[ifIndex].WscControl.bWscTrigger)) {
		bHasWscIe = TRUE;
	}
#ifdef WSC_V2_SUPPORT
	else if (pAd->ApCfg.ApCliTab[ifIndex].WscControl.WscV2Info.bEnableWpsV2)
		bHasWscIe = TRUE;
#endif /* WSC_V2_SUPPORT */
	if (bHasWscIe) {
		UCHAR /* WscBuf[256], */ WscIeLen = 0;
		UCHAR *WscBuf = NULL;
		ULONG WscTmpLen = 0;
		/* allocate memory */
		os_alloc_mem(NULL, (UCHAR **) &WscBuf, 512);
		if (WscBuf != NULL) {
			NdisZeroMemory(WscBuf, 512);
			WscBuildProbeReqIE(pAd, STA_MODE, WscBuf, &WscIeLen);
			MakeOutgoingFrame(pOutBuffer + FrameLen, &WscTmpLen,
					  WscIeLen, WscBuf, END_OF_ARGS);
			FrameLen += WscTmpLen;
			os_free_mem(NULL, WscBuf);
		} else
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Allocate memory fail!!!\n", __func__));
	}
#endif /*WSC_AP_SUPPORT */

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	if ((pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP != WPA_SUPPLICANT_DISABLE) &&
	    (pAd->cfg80211_ctrl.ExtraIeLen != 0)) {
		ULONG ExtraIeTmpLen = 0;

		MakeOutgoingFrame(pOutBuffer + FrameLen, &ExtraIeTmpLen,
				  pAd->cfg80211_ctrl.ExtraIeLen,
				  pAd->cfg80211_ctrl.pExtraIe, END_OF_ARGS);

		FrameLen += ExtraIeTmpLen;
	}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	MiniportMMRequest(pAd, QID_AC_BE, pOutBuffer, FrameLen);
	MlmeFreeMemory(pAd, pOutBuffer);
}

#endif /* APCLI_SUPPORT */
