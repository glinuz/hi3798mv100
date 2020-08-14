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


#define ADHOC_ENTRY_BEACON_LOST_TIME	(2*OS_HZ)	/* 2 sec */

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
VOID SyncStateMachineInit(IN PRTMP_ADAPTER pAd,
			  IN STATE_MACHINE * Sm, OUT STATE_MACHINE_FUNC Trans[])
{
	StateMachineInit(Sm, Trans, MAX_SYNC_STATE, MAX_SYNC_MSG, (STATE_MACHINE_FUNC) Drop,
			 SYNC_IDLE, SYNC_MACHINE_BASE);

	/* column 1 */
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_SCAN_REQ,
			      (STATE_MACHINE_FUNC) MlmeScanReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_FORCE_SCAN_REQ,
			      (STATE_MACHINE_FUNC) MlmeForceScanReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_JOIN_REQ,
			      (STATE_MACHINE_FUNC) MlmeJoinReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_FORCE_JOIN_REQ,
			      (STATE_MACHINE_FUNC) MlmeForceJoinReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_MLME_START_REQ,
			      (STATE_MACHINE_FUNC) MlmeStartReqAction);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_PEER_BEACON, (STATE_MACHINE_FUNC) PeerBeacon);
	StateMachineSetAction(Sm, SYNC_IDLE, MT2_PEER_PROBE_REQ,
			      (STATE_MACHINE_FUNC) PeerProbeReqAction);

	/* column 2 */
	StateMachineSetAction(Sm, JOIN_WAIT_BEACON, MT2_MLME_JOIN_REQ,
			      (STATE_MACHINE_FUNC) MlmeJoinReqAction);
	StateMachineSetAction(Sm, JOIN_WAIT_BEACON, MT2_MLME_START_REQ,
			      (STATE_MACHINE_FUNC) InvalidStateWhenStart);
	StateMachineSetAction(Sm, JOIN_WAIT_BEACON, MT2_PEER_BEACON,
			      (STATE_MACHINE_FUNC) PeerBeaconAtJoinAction);
	StateMachineSetAction(Sm, JOIN_WAIT_BEACON, MT2_BEACON_TIMEOUT,
			      (STATE_MACHINE_FUNC) BeaconTimeoutAtJoinAction);
	StateMachineSetAction(Sm, JOIN_WAIT_BEACON, MT2_PEER_PROBE_RSP,
			      (STATE_MACHINE_FUNC) PeerBeaconAtScanAction);

	/* column 3 */
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_MLME_JOIN_REQ,
			      (STATE_MACHINE_FUNC) MlmeJoinReqAction);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_MLME_START_REQ,
			      (STATE_MACHINE_FUNC) InvalidStateWhenStart);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_PEER_BEACON,
			      (STATE_MACHINE_FUNC) PeerBeaconAtScanAction);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_PEER_PROBE_RSP,
			      (STATE_MACHINE_FUNC) PeerBeaconAtScanAction);
	StateMachineSetAction(Sm, SCAN_LISTEN, MT2_SCAN_TIMEOUT,
			      (STATE_MACHINE_FUNC) ScanTimeoutAction);
	/* StateMachineSetAction(Sm, SCAN_LISTEN, MT2_MLME_SCAN_CNCL, (STATE_MACHINE_FUNC)ScanCnclAction); */

	/* resume scanning for fast-roaming */
	StateMachineSetAction(Sm, SCAN_PENDING, MT2_MLME_SCAN_REQ,
			      (STATE_MACHINE_FUNC) MlmeScanReqAction);
	StateMachineSetAction(Sm, SCAN_PENDING, MT2_PEER_BEACON, (STATE_MACHINE_FUNC) PeerBeacon);

	/* timer init */
	RTMPInitTimer(pAd, &pAd->MlmeAux.BeaconTimer, GET_TIMER_FUNCTION(BeaconTimeout), pAd,
		      FALSE);
	RTMPInitTimer(pAd, &pAd->MlmeAux.ScanTimer, GET_TIMER_FUNCTION(ScanTimeout), pAd, FALSE);
}

/*
	==========================================================================
	Description:
		Beacon timeout handler, executed in timer thread

	IRQL = DISPATCH_LEVEL

	==========================================================================
 */
VOID BeaconTimeout(IN PVOID SystemSpecific1,
		   IN PVOID FunctionContext, IN PVOID SystemSpecific2, IN PVOID SystemSpecific3)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) FunctionContext;

	DBGPRINT(RT_DEBUG_TRACE, ("SYNC - BeaconTimeout\n"));

	/*
	   Do nothing if the driver is starting halt state.
	   This might happen when timer already been fired before cancel timer with mlmehalt
	 */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		return;

	if ((pAd->CommonCfg.BBPCurrentBW == BW_40)
	    ) {
		bbp_set_bw(pAd, BW_40);

		AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
		AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("SYNC - End of SCAN, restore to 40MHz channel %d, Total BSS[%02d]\n",
			  pAd->CommonCfg.CentralChannel, pAd->ScanTab.BssNr));
	}

	MlmeEnqueue(pAd, SYNC_STATE_MACHINE, MT2_BEACON_TIMEOUT, 0, NULL, 0);
	RTMP_MLME_HANDLER(pAd);
}

/*
	==========================================================================
	Description:
		Scan timeout handler, executed in timer thread

	IRQL = DISPATCH_LEVEL

	==========================================================================
 */
VOID ScanTimeout(IN PVOID SystemSpecific1,
		 IN PVOID FunctionContext, IN PVOID SystemSpecific2, IN PVOID SystemSpecific3)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) FunctionContext;


	/*
	   Do nothing if the driver is starting halt state.
	   This might happen when timer already been fired before cancel timer with mlmehalt
	 */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS))
		return;

	if (MlmeEnqueue(pAd, SYNC_STATE_MACHINE, MT2_SCAN_TIMEOUT, 0, NULL, 0)) {
		RTMP_MLME_HANDLER(pAd);
	} else {
		/* To prevent SyncMachine.CurrState is SCAN_LISTEN forever. */
		pAd->MlmeAux.Channel = 0;
		ScanNextChannel(pAd, OPMODE_STA);
		RTMPSendWirelessEvent(pAd, IW_SCAN_ENQUEUE_FAIL_EVENT_FLAG, NULL, BSS0, 0);
	}
}

VOID MlmeForceJoinReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	BOOLEAN TimerCancelled;
	HEADER_802_11 Hdr80211;
	NDIS_STATUS NStatus;
	ULONG FrameLen = 0;
	PUCHAR pOutBuffer = NULL;
	PUCHAR pSupRate = NULL;
	UCHAR SupRateLen;
	PUCHAR pExtRate = NULL;
	UCHAR ExtRateLen;
	UCHAR ASupRate[] = { 0x8C, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6C };
	UCHAR ASupRateLen = sizeof(ASupRate) / sizeof(UCHAR);
	MLME_JOIN_REQ_STRUCT *pInfo = (MLME_JOIN_REQ_STRUCT *) (Elem->Msg);

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	DBGPRINT(RT_DEBUG_TRACE, ("SYNC - MlmeForeJoinReqAction(BSS #%ld)\n", pInfo->BssIdx));

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND

	if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == 1) {
		DBGPRINT(RT_DEBUG_TRACE, ("MlmeJoinReqAction: autopm_resume success\n"));
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
	} else if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == (-1)) {
		DBGPRINT(RT_DEBUG_ERROR, ("MlmeJoinReqAction autopm_resume fail ------\n"));
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		return;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("MlmeJoinReqAction: autopm_resume do nothing\n"));

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

#ifdef PCIE_PS_SUPPORT
	if ((OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) &&
	    (IDLE_ON(pAd)) &&
	    (pAd->StaCfg.bRadio == TRUE) && (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))) {
		RT28xxPciAsicRadioOn(pAd, GUI_IDLE_POWER_SAVE);
	}
#endif /* PCIE_PS_SUPPORT */

	/* reset all the timers */
	RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &TimerCancelled);
	RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);

	{
		RTMPZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->MlmeAux.Ssid, pAd->StaCfg.ConnectinfoSsid,
			       pAd->StaCfg.ConnectinfoSsidLen);
		pAd->MlmeAux.SsidLen = pAd->StaCfg.ConnectinfoSsidLen;
	}

	pAd->MlmeAux.BssType = pAd->StaCfg.ConnectinfoBssType;
	pAd->MlmeAux.Channel = pAd->StaCfg.ConnectinfoChannel;

	/* Let BBP register at 20MHz to do scan */
	AsicSetChannel(pAd, pAd->MlmeAux.Channel, BW_20, EXTCHA_NONE, FALSE);
	DBGPRINT(RT_DEBUG_TRACE, ("SYNC - BBP R4 to 20MHz.l\n"));

	RTMPSetTimer(&pAd->MlmeAux.BeaconTimer, JOIN_TIMEOUT);

	do {
		/*
		   send probe request
		 */
		NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
		if (NStatus == NDIS_STATUS_SUCCESS) {
			if (pAd->MlmeAux.Channel <= 14) {
				pSupRate = pAd->CommonCfg.SupRate;
				SupRateLen = pAd->CommonCfg.SupRateLen;
				pExtRate = pAd->CommonCfg.ExtRate;
				ExtRateLen = pAd->CommonCfg.ExtRateLen;
			} else {
				/*
				   Overwrite Support Rate, CCK rate are not allowed
				 */
				pSupRate = ASupRate;
				SupRateLen = ASupRateLen;
				ExtRateLen = 0;
			}

			if ((pAd->MlmeAux.BssType == BSS_INFRA)
			    && (!MAC_ADDR_EQUAL(ZERO_MAC_ADDR, pAd->StaCfg.ConnectinfoBssid))) {
				COPY_MAC_ADDR(pAd->MlmeAux.Bssid, pAd->StaCfg.ConnectinfoBssid);
				MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
						 pAd->MlmeAux.Bssid, pAd->CurrentAddress,
						 pAd->MlmeAux.Bssid);
			} else
				MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
						 BROADCAST_ADDR, pAd->CurrentAddress,
						 BROADCAST_ADDR);

			MakeOutgoingFrame(pOutBuffer, &FrameLen,
					  sizeof(HEADER_802_11), &Hdr80211,
					  1, &SsidIe,
					  1, &pAd->MlmeAux.SsidLen,
					  pAd->MlmeAux.SsidLen, pAd->MlmeAux.Ssid,
					  1, &SupRateIe,
					  1, &SupRateLen, SupRateLen, pSupRate, END_OF_ARGS);

			if (ExtRateLen) {
				ULONG Tmp;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
						  1, &ExtRateIe,
						  1, &ExtRateLen,
						  ExtRateLen, pExtRate, END_OF_ARGS);
				FrameLen += Tmp;
			}
#ifdef WPA_SUPPLICANT_SUPPORT
			if ((pAd->OpMode == OPMODE_STA) &&
			    (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP !=
			     WPA_SUPPLICANT_DISABLE)
			    && (pAd->StaCfg.wpa_supplicant_info.WpsProbeReqIeLen != 0)) {
				ULONG WpsTmpLen = 0;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &WpsTmpLen,
						  pAd->StaCfg.wpa_supplicant_info.WpsProbeReqIeLen,
						  pAd->StaCfg.wpa_supplicant_info.pWpsProbeReqIe,
						  END_OF_ARGS);

				FrameLen += WpsTmpLen;
			}
#ifdef RT_CFG80211_SUPPORT
			if ((pAd->OpMode == OPMODE_STA) &&
			    (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP !=
			     WPA_SUPPLICANT_DISABLE) && (pAd->cfg80211_ctrl.ExtraIeLen != 0)) {
				ULONG ExtraIeTmpLen = 0;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &ExtraIeTmpLen,
						  pAd->cfg80211_ctrl.ExtraIeLen,
						  pAd->cfg80211_ctrl.pExtraIe, END_OF_ARGS);

				FrameLen += ExtraIeTmpLen;
			}
#endif /* RT_CFG80211_SUPPORT */

#endif /* WPA_SUPPLICANT_SUPPORT */
#ifdef WFD_SUPPORT
#ifdef RT_CFG80211_SUPPORT
			if (pAd->StaCfg.WfdCfg.bSuppInsertWfdIe) {
				ULONG WfdIeLen, WfdIeBitmap;
				PUCHAR ptr;

				ptr = pOutBuffer + FrameLen;
				WfdIeBitmap =
				    (0x1 << SUBID_WFD_DEVICE_INFO) | (0x1 <<
								      SUBID_WFD_ASSOCIATED_BSSID) |
				    (0x1 << SUBID_WFD_COUPLED_SINK_INFO);
				WfdMakeWfdIE(pAd, WfdIeBitmap, ptr, &WfdIeLen);
				FrameLen += WfdIeLen;
			}
#endif /* RT_CFG80211_SUPPORT */
#endif /* WFD_SUPPORT */

			MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
			MlmeFreeMemory(pAd, pOutBuffer);
		}
	} while (FALSE);

	DBGPRINT(0,
		 ("FORCE JOIN SYNC - Switch to ch %d, Wait BEACON from %02x:%02x:%02x:%02x:%02x:%02x\n",
		  pAd->StaCfg.ConnectinfoChannel, PRINT_MAC(pAd->StaCfg.ConnectinfoBssid)));

	pAd->Mlme.SyncMachine.CurrState = JOIN_WAIT_BEACON;
}

VOID MlmeForceScanReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	UCHAR Ssid[MAX_LEN_OF_SSID], SsidLen, ScanType, BssType;
	BOOLEAN TimerCancelled;
	ULONG Now;
	USHORT Status;

#ifdef RTMP_MAC_USB
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
		ASIC_RADIO_ON(pAd, MLME_RADIO_ON);
#endif /* RTMP_MAC_USB */
	/*
	   Check the total scan tries for one single OID command
	   If this is the CCX 2.0 Case, skip that!
	 */
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP)) {
		DBGPRINT(RT_DEBUG_TRACE, ("SYNC - MlmeForceScanReqAction before Startup\n"));
		return;
	}

	/* first check the parameter sanity */
	if (MlmeScanReqSanity(pAd,
			      Elem->Msg,
			      Elem->MsgLen, &BssType, (PCHAR) Ssid, &SsidLen, &ScanType)) {
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
		if (pAd->ApCfg.ApCliTab[MAIN_MBSSID].Valid && RTMP_CFG80211_VIF_P2P_CLI_ON(pAd)) {
			DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_NULL: PWR_SAVE IN ForceScanStart\n"));
			RT_CFG80211_P2P_CLI_SEND_NULL_FRAME(pAd, PWR_SAVE);
		}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

		/*
		   To prevent data lost.
		   Send an NULL data with turned PSM bit on to current associated AP before SCAN progress.
		   And should send an NULL data with turned PSM bit off to AP, when scan progress done
		 */
		if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED) && (INFRA_ON(pAd))) {
			RTMPSendNullFrame(pAd,
					  pAd->CommonCfg.TxRate,
					  (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_WMM_INUSED) ? TRUE :
					   FALSE), PWR_SAVE);

			DBGPRINT(RT_DEBUG_TRACE,
				 ("MlmeForceScanReqAction -- Send PSM Data frame for off channel RM, SCAN_IN_PROGRESS=%d!\n",
				  RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)));
			OS_WAIT(20);
		}

		/*
		   Check for channel load and noise hist request
		   Suspend MSDU only at scan request, not the last two mentioned
		   Suspend MSDU transmission here
		   YF: Move this line after SendNullFrame due to ScanInProg Flag
		 */
		RTMPSuspendMsduTransmission(pAd);

		RTMPSendWirelessEvent(pAd, IW_SCANNING_EVENT_FLAG, NULL, BSS0, 0);

		NdisGetSystemUpTime(&Now);
		pAd->StaCfg.LastScanTime = Now;
		/* reset all the timers */
		RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);
		RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &TimerCancelled);

		/* record desired BSS parameters */
		pAd->MlmeAux.BssType = BssType;
		pAd->MlmeAux.ScanType = ScanType;
		pAd->MlmeAux.SsidLen = SsidLen;
		NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->MlmeAux.Ssid, Ssid, SsidLen);

		/*
		   Scanning was pending (for fast scanning)
		 */
		if ((pAd->StaCfg.bImprovedScan)
		    && (pAd->Mlme.SyncMachine.CurrState == SCAN_PENDING)) {
			pAd->MlmeAux.Channel = pAd->StaCfg.LastScanChannel;
		} else {
			if (pAd->StaCfg.bFastConnect && (pAd->CommonCfg.Channel != 0)
			    && !pAd->StaCfg.bNotFirstScan) {
				pAd->MlmeAux.Channel = pAd->CommonCfg.Channel;
			} else
				/* start from the first channel */
				pAd->MlmeAux.Channel = FirstChannel(pAd);
		}

		/* Let BBP register at 20MHz to do scan */
		bbp_set_bw(pAd, BW_20);
		DBGPRINT(RT_DEBUG_TRACE, ("SYNC - BBP R4 to 20MHz.l\n"));
		/* Before scan, reset trigger event table. */
		TriEventInit(pAd);

		ScanNextChannel(pAd, OPMODE_STA);
		if (pAd->StaCfg.ConnectinfoChannel != 0)
			pAd->MlmeAux.Channel = 0;
		pAd->Mlme.CntlMachine.CurrState = CNTL_WAIT_SCAN_FOR_CONNECT;
	} else {
		DBGPRINT_ERR(("SYNC - MlmeForceScanReqAction() sanity check fail\n"));
		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
		Status = MLME_INVALID_FORMAT;
		MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_SCAN_CONF, 2, &Status, 0);
	}
}

/*
	==========================================================================
	Description:
		MLME SCAN req state machine procedure
	==========================================================================
 */
VOID MlmeScanReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	UCHAR Ssid[MAX_LEN_OF_SSID], SsidLen, ScanType, BssType;
	BOOLEAN TimerCancelled;
	ULONG Now;
	USHORT Status;
	CFG80211_CB *pCfg80211_CB;

	pCfg80211_CB = pAd->pCfg80211_CB;

#ifdef RTMP_MAC_USB
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
		ASIC_RADIO_ON(pAd, MLME_RADIO_ON);
#endif /* RTMP_MAC_USB */
	/*
	   Check the total scan tries for one single OID command
	   If this is the CCX 2.0 Case, skip that!
	 */
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP)) {
		DBGPRINT(RT_DEBUG_ERROR, ("SYNC - MlmeScanReqAction before Startup\n"));
		CFG80211OS_ScanEnd(pCfg80211_CB, TRUE);
		return;
	}
#ifdef PCIE_PS_SUPPORT
	if ((OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) &&
	    (IDLE_ON(pAd)) &&
	    (pAd->StaCfg.bRadio == TRUE) && (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))) {
		if (pAd->StaCfg.PSControl.field.EnableNewPS == FALSE) {
			AsicSendCmdToMcuAndWait(pAd, 0x31, PowerWakeCID, 0x00, 0x02, FALSE);
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);
			DBGPRINT(RT_DEBUG_TRACE, ("PSM - Issue Wake up command\n"));
		} else {
			RT28xxPciAsicRadioOn(pAd, GUI_IDLE_POWER_SAVE);
		}
	}
#endif /* PCIE_PS_SUPPORT */

	/* first check the parameter sanity */
	if (MlmeScanReqSanity(pAd,
			      Elem->Msg,
			      Elem->MsgLen, &BssType, (PCHAR) Ssid, &SsidLen, &ScanType)) {
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
		if (pAd->ApCfg.ApCliTab[MAIN_MBSSID].Valid && RTMP_CFG80211_VIF_P2P_CLI_ON(pAd)) {
			DBGPRINT(RT_DEBUG_TRACE, ("CFG80211_NULL: PWR_SAVE IN ScanStart\n"));
			RT_CFG80211_P2P_CLI_SEND_NULL_FRAME(pAd, PWR_SAVE);
		}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef CONFIG_MULTI_CHANNEL
#ifdef RT_CFG80211_SUPPORT
		if ((RTMP_CFG80211_VIF_P2P_GO_ON(pAd))) {
			DBGPRINT(RT_DEBUG_TRACE, ("delay 380 for noa !!!!\n"));
			OS_WAIT(380);
		}
#endif /* RT_CFG80211_SUPPORT */
#endif /* CONFIG_MULTI_CHANNEL */

		/* Stop TX de-queue before sending out NULL frame with power-save bit. */
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS);

#ifdef RTMP_USB_SUPPORT
		/* Check the TX queue is empty */
		if (mt76x2_polling_txq_empty(pAd) != STATUS_SUCCESS) {
			DBGPRINT(RT_DEBUG_ERROR,
					("[warning]%s(%d):Tx buffer have data.\n",
					__func__, __LINE__));
		}
#else
		OS_WAIT(10);
#endif

		/*
		   To prevent data lost.
		   Send an NULL data with turned PSM bit on to current associated AP before SCAN progress.
		   And should send an NULL data with turned PSM bit off to AP, when scan progress done
		 */
		if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED) && (INFRA_ON(pAd))) {
			RTMPSendNullFrame(pAd,
					  pAd->CommonCfg.TxRate,
					  (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_WMM_INUSED) ? TRUE :
					   FALSE), PWR_SAVE);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("MlmeScanReqAction -- Send PSM Data frame for off channel RM, SCAN_IN_PROGRESS=%d!\n",
				  RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)));
			OS_WAIT(20);
		}

		/*
		   Check for channel load and noise hist request
		   Suspend MSDU only at scan request, not the last two mentioned
		   Suspend MSDU transmission here
		 */
		RTMPSuspendMsduTransmission(pAd);

		RTMPSendWirelessEvent(pAd, IW_SCANNING_EVENT_FLAG, NULL, BSS0, 0);

		NdisGetSystemUpTime(&Now);
		pAd->StaCfg.LastScanTime = Now;
		/* reset all the timers */
		RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);
		RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &TimerCancelled);

		/* record desired BSS parameters */
		pAd->MlmeAux.BssType = BssType;
		pAd->MlmeAux.ScanType = ScanType;
		pAd->MlmeAux.SsidLen = SsidLen;
		NdisZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->MlmeAux.Ssid, Ssid, SsidLen);

		/*
		   Scanning was pending (for fast scanning)
		 */
		if ((pAd->StaCfg.bImprovedScan)
		    && (pAd->Mlme.SyncMachine.CurrState == SCAN_PENDING)) {
			pAd->MlmeAux.Channel = pAd->StaCfg.LastScanChannel;
		} else {
			if (pAd->StaCfg.bFastConnect && (pAd->CommonCfg.Channel != 0)
			    && !pAd->StaCfg.bNotFirstScan) {
				pAd->MlmeAux.Channel = pAd->CommonCfg.Channel;
			} else {
				{
					/* start from the first channel */
					pAd->MlmeAux.Channel = FirstChannel(pAd);
				}
			}
		}

		/* Let BBP register at 20MHz to do scan */
		bbp_set_bw(pAd, BW_20);
		DBGPRINT(RT_DEBUG_TRACE, ("SYNC - BBP R4 to 20MHz.l\n"));

		/* Before scan, reset trigger event table. */
		TriEventInit(pAd);

#ifdef MT76XX_BTCOEX_SUPPORT
		MLMEHook(pAd, WLAN_SCANREQEST, WLAN_NO_BSSID);
#endif /*MT76XX_BTCOEX_SUPPORT */
#if defined(RT_CFG80211_SUPPORT) && defined(NEW_WOW_SUPPORT)
		/* If FlgCfg80211Scanning was set to FALSE, it means the scan migth be canceled.
		 * So we should ignore this scan by setting MlmeAux.Channel = 0. It will make
		 * ScanNextChannel going into scan_ch_restore, then terminate the scan.
		 * E.g., During in the suspend process, the new scan request will be canceled.
		 * But if in specific timing, it may go into this function and trigger channel
		 * switch unexpectedly. So we need to take care here.
		 */
		if (!(pAd->cfg80211_ctrl.FlgCfg80211Scanning) &&
		    RTMP_TEST_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING)) {
			if (ScanType == SCAN_2040_BSS_COEXIST) {
				pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: Ignore BSS_2040_Coex scan during suspend\n",
					  __func__));
				return;
			}
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s: FlgCfg80211Scanning==FALSE, set scan channel to 0\n",
				  __func__));
			pAd->MlmeAux.Channel = 0;
		}
#endif /* RT_CFG80211_SUPPORT && NEW_WOW_SUPPORT */

		ScanNextChannel(pAd, OPMODE_STA);
	} else {
		DBGPRINT_ERR(("SYNC - MlmeScanReqAction() sanity check fail\n"));
		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
		Status = MLME_INVALID_FORMAT;
		MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_SCAN_CONF, 2, &Status, 0);
	}
}

/*
	==========================================================================
	Description:
		MLME JOIN req state machine procedure
	==========================================================================
 */
VOID MlmeJoinReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	BSS_ENTRY *pBss;
	BOOLEAN TimerCancelled;
	HEADER_802_11 Hdr80211;
	NDIS_STATUS NStatus;
	ULONG FrameLen = 0;
	PUCHAR pOutBuffer = NULL;
	PUCHAR pSupRate = NULL;
	UCHAR SupRateLen;
	PUCHAR pExtRate = NULL;
	UCHAR ExtRateLen;
	UCHAR ASupRate[] = { 0x8C, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6C };
	UCHAR ASupRateLen = sizeof(ASupRate) / sizeof(UCHAR);
	MLME_JOIN_REQ_STRUCT *pInfo = (MLME_JOIN_REQ_STRUCT *) (Elem->Msg);
#ifdef WSC_STA_SUPPORT
	BOOLEAN bHasWscIe = FALSE;
#endif /* WSC_STA_SUPPORT */

#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */
	BOOLEAN bChangeInitBW = FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("SYNC - MlmeJoinReqAction(BSS #%ld)\n", pInfo->BssIdx));
#ifdef MT76XX_BTCOEX_SUPPORT
	MLMEHook(pAd, WLAN_CONNECTION_START, QueryHashID(pAd, pAd->MlmeAux.Bssid, FALSE));
#endif /*MT76XX_BTCOEX_SUPPORT */
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND

	if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == 1) {
		DBGPRINT(RT_DEBUG_TRACE, ("MlmeJoinReqAction: autopm_resume success\n"));
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
	} else if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == (-1)) {
		DBGPRINT(RT_DEBUG_ERROR, ("MlmeJoinReqAction autopm_resume fail ------\n"));
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		return;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("MlmeJoinReqAction: autopm_resume do nothing\n"));

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

#ifdef PCIE_PS_SUPPORT
	if ((OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) &&
	    (IDLE_ON(pAd)) &&
	    (pAd->StaCfg.bRadio == TRUE) && (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))) {
		RT28xxPciAsicRadioOn(pAd, GUI_IDLE_POWER_SAVE);
	}
#endif /* PCIE_PS_SUPPORT */

	/* reset all the timers */
	RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &TimerCancelled);
	RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);

	pBss = &pAd->MlmeAux.SsidBssTab.BssEntry[pInfo->BssIdx];

	/* record the desired SSID & BSSID we're waiting for */
	COPY_MAC_ADDR(pAd->MlmeAux.Bssid, pBss->Bssid);

	/* If AP's SSID is not hidden, it is OK for updating ssid to MlmeAux again. */
	if (pBss->Hidden == 0) {
		RTMPZeroMemory(pAd->MlmeAux.Ssid, MAX_LEN_OF_SSID);
		NdisMoveMemory(pAd->MlmeAux.Ssid, pBss->Ssid, pBss->SsidLen);
		pAd->MlmeAux.SsidLen = pBss->SsidLen;
	}

	pAd->MlmeAux.BssType = pBss->BssType;
	pAd->MlmeAux.Channel = pBss->Channel;
	pAd->MlmeAux.CentralChannel = pBss->CentralChannel;

#ifdef CONFIG_MULTI_CHANNEL
	pAd->MCC_InfraConnect_Protect = TRUE;
	pAd->MCC_InfraConnect_Count = 0;
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef EXT_BUILD_CHANNEL_LIST
	/* Country IE of the AP will be evaluated and will be used. */
	if ((pAd->StaCfg.IEEE80211dClientMode != Rt802_11_D_None) && (pBss->bHasCountryIE == TRUE)) {
		NdisMoveMemory(&pAd->CommonCfg.CountryCode[0], &pBss->CountryString[0], 2);
		if (pBss->CountryString[2] == 'I')
			pAd->CommonCfg.Geography = IDOR;
		else if (pBss->CountryString[2] == 'O')
			pAd->CommonCfg.Geography = ODOR;
		else
			pAd->CommonCfg.Geography = BOTH;
		BuildChannelListEx(pAd);
	}
#endif /* EXT_BUILD_CHANNEL_LIST */

	{
		bChangeInitBW = TRUE;
	}

#ifdef CFG80211_CHANNEL_SWITCH_NOTIFICATION
	/* If there is an softAP started on another channel already
	*  , the softAP has to restart on the STA channel.Otherwise
	*  , there would be some RF concurrent issues.
	*/

	if (CFG_AP_ON(pAd)
		&& (pAd->MlmeAux.Channel != pAd->ApCfg.MBSSID[SECOND_MBSSID].wdev.channel)) {
		int index;

		/* If we did notify the AP, De-Auth all connected clients */
		for (index = 0; index < MAX_LEN_OF_MAC_TABLE; index++) {

			MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[index];

			if (IS_ENTRY_CLIENT(pEntry)
				&& !MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, pEntry->Addr)) {
				DBGPRINT(RT_DEBUG_OFF
				, ("CNTL - Send De-auth frame to %02X:%02X:%02X:%02X:%02X:%02X\n"
				, PRINT_MAC(pEntry->Addr)));
				MlmeDeAuthAction(pAd, pEntry, REASON_DISASSOC_INACTIVE, FALSE);
			}
		}

		CFG80211OS_ChannelSwitchNotify((VOID *)pAd, pAd->MlmeAux.Channel);
	}
#endif /* CFG80211_CHANNEL_SWITCH_NOTIFICATION */

	if (bChangeInitBW == TRUE) {
		/* Let BBP register at 20MHz to do scan */
		bbp_set_bw(pAd, BW_20);
		DBGPRINT(RT_DEBUG_TRACE, ("%s(): Set BBP BW=20MHz\n", __func__));

		/* switch channel and waiting for beacon timer */
		AsicSwitchChannel(pAd, pAd->MlmeAux.Channel, FALSE);
		AsicLockChannel(pAd, pAd->MlmeAux.Channel);
	}
#ifdef WSC_STA_SUPPORT
#ifdef WSC_LED_SUPPORT
	/* LED indication. */
	if (pAd->MlmeAux.BssType == BSS_INFRA) {
		LEDConnectionStart(pAd);
		LEDConnectionCompletion(pAd, TRUE);
	}
#endif /* WSC_LED_SUPPORT */
#endif /* WSC_STA_SUPPORT */

	RTMPSetTimer(&pAd->MlmeAux.BeaconTimer, JOIN_TIMEOUT);

	do {
		if (((pAd->CommonCfg.bIEEE80211H == 1) &&
		     (pAd->MlmeAux.Channel > 14) && RadarChannelCheck(pAd, pAd->MlmeAux.Channel))
#ifdef CARRIER_DETECTION_SUPPORT	/* Roger sync Carrier */
		    || (pAd->CommonCfg.CarrierDetect.Enable == TRUE)
#endif /* CARRIER_DETECTION_SUPPORT */
		    ) {
			/* We can't send any Probe request frame to meet 802.11h. */
			if (pBss->Hidden == 0)
				break;
		}

		/*
		   send probe request
		 */
		NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
		if (NStatus == NDIS_STATUS_SUCCESS) {
			if (pAd->MlmeAux.Channel <= 14) {
				pSupRate = pAd->CommonCfg.SupRate;
				SupRateLen = pAd->CommonCfg.SupRateLen;
				pExtRate = pAd->CommonCfg.ExtRate;
				ExtRateLen = pAd->CommonCfg.ExtRateLen;
			} else {
				/* Overwrite Support Rate, CCK rate are not allowed */
				pSupRate = ASupRate;
				SupRateLen = ASupRateLen;
				ExtRateLen = 0;
			}

			if (pAd->MlmeAux.BssType == BSS_INFRA)
				MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
						 pAd->MlmeAux.Bssid, pAd->CurrentAddress,
						 pAd->MlmeAux.Bssid);
			else
				MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0,
						 BROADCAST_ADDR, pAd->CurrentAddress,
						 BROADCAST_ADDR);

			MakeOutgoingFrame(pOutBuffer, &FrameLen,
					  sizeof(HEADER_802_11), &Hdr80211,
					  1, &SsidIe,
					  1, &pAd->MlmeAux.SsidLen,
					  pAd->MlmeAux.SsidLen, pAd->MlmeAux.Ssid,
					  1, &SupRateIe,
					  1, &SupRateLen, SupRateLen, pSupRate, END_OF_ARGS);

			if (ExtRateLen) {
				ULONG Tmp;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &Tmp,
						  1, &ExtRateIe,
						  1, &ExtRateLen,
						  ExtRateLen, pExtRate, END_OF_ARGS);
				FrameLen += Tmp;
			}
#ifdef WSC_STA_SUPPORT
			/* Append WSC information in probe request if WSC state is running */
			if ((pAd->StaCfg.WscControl.WscEnProbeReqIE) &&
			    (pAd->StaCfg.WscControl.WscConfMode != WSC_DISABLE) &&
			    (pAd->StaCfg.WscControl.bWscTrigger
			    ))
				bHasWscIe = TRUE;
#ifdef WSC_V2_SUPPORT
			else if ((pAd->StaCfg.WscControl.WscEnProbeReqIE) &&
				 (pAd->StaCfg.WscControl.WscV2Info.bEnableWpsV2))
				bHasWscIe = TRUE;
#endif /* WSC_V2_SUPPORT */

			if (bHasWscIe) {
				UCHAR WscIeLen = 0;
				UCHAR *WscBuf = NULL;
				ULONG WscTmpLen = 0;

				/* allocate memory */
				os_alloc_mem(NULL, (UCHAR **) &WscBuf, 256);
				if (WscBuf != NULL) {
					NdisZeroMemory(WscBuf, 256);
					WscBuildProbeReqIE(pAd, STA_MODE, WscBuf, &WscIeLen);

					MakeOutgoingFrame(pOutBuffer + FrameLen, &WscTmpLen,
							  WscIeLen, WscBuf, END_OF_ARGS);

					FrameLen += WscTmpLen;
					os_free_mem(NULL, WscBuf);
				} else
					DBGPRINT(RT_DEBUG_ERROR,
						 ("%s: Allocate memory fail!!!\n", __func__));
			}
#endif /* WSC_STA_SUPPORT */


#ifdef WPA_SUPPLICANT_SUPPORT
			if ((pAd->OpMode == OPMODE_STA) &&
			    (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP !=
			     WPA_SUPPLICANT_DISABLE)
			    && (pAd->StaCfg.wpa_supplicant_info.WpsProbeReqIeLen != 0)) {
				ULONG WpsTmpLen = 0;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &WpsTmpLen,
						  pAd->StaCfg.wpa_supplicant_info.WpsProbeReqIeLen,
						  pAd->StaCfg.wpa_supplicant_info.pWpsProbeReqIe,
						  END_OF_ARGS);

				FrameLen += WpsTmpLen;
			}
#endif /* WPA_SUPPLICANT_SUPPORT */

#ifdef RT_CFG80211_SUPPORT
			if ((pAd->OpMode == OPMODE_STA) &&
			    (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP !=
			     WPA_SUPPLICANT_DISABLE) && (pAd->cfg80211_ctrl.ExtraIeLen != 0)) {
				ULONG ExtraIeTmpLen = 0;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &ExtraIeTmpLen,
						  pAd->cfg80211_ctrl.ExtraIeLen,
						  pAd->cfg80211_ctrl.pExtraIe, END_OF_ARGS);

				FrameLen += ExtraIeTmpLen;
			}
#endif /* RT_CFG80211_SUPPORT */

#ifdef WFD_SUPPORT
#ifdef RT_CFG80211_SUPPORT
			if (pAd->StaCfg.WfdCfg.bSuppInsertWfdIe) {
				ULONG WfdIeLen, WfdIeBitmap;
				PUCHAR ptr;

				ptr = pOutBuffer + FrameLen;
				WfdIeBitmap =
				    (0x1 << SUBID_WFD_DEVICE_INFO) | (0x1 <<
								      SUBID_WFD_ASSOCIATED_BSSID) |
				    (0x1 << SUBID_WFD_COUPLED_SINK_INFO);
				WfdMakeWfdIE(pAd, WfdIeBitmap, ptr, &WfdIeLen);
				FrameLen += WfdIeLen;
			}
#endif /* RT_CFG80211_SUPPORT */
#endif /* WFD_SUPPORT */

			MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
			MlmeFreeMemory(pAd, pOutBuffer);
		}
	} while (FALSE);

	DBGPRINT(RT_DEBUG_OFF,
		 ("SYNC - Switch to ch %d, Wait BEACON from %02x:%02x:%02x:%02x:%02x:%02x\n",
		  pBss->Channel, pBss->Bssid[0], pBss->Bssid[1], pBss->Bssid[2], pBss->Bssid[3],
		  pBss->Bssid[4], pBss->Bssid[5]));

	pAd->Mlme.SyncMachine.CurrState = JOIN_WAIT_BEACON;
}

/*
	==========================================================================
	Description:
		MLME START Request state machine procedure, starting an IBSS
	==========================================================================
 */
VOID MlmeStartReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	UCHAR Ssid[MAX_LEN_OF_SSID], SsidLen;
	BOOLEAN TimerCancelled;
	UCHAR *VarIE = NULL;	/* New for WPA security suites */
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	LARGE_INTEGER TimeStamp;
	BOOLEAN Privacy;
	USHORT Status;

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **) &VarIE, MAX_VIE_LEN);
	if (VarIE == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __func__));
		return;
	}

	/* Init Variable IE structure */
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;
	TimeStamp.u.LowPart = 0;
	TimeStamp.u.HighPart = 0;

	if ((MlmeStartReqSanity(pAd, Elem->Msg, Elem->MsgLen, (PCHAR) Ssid, &SsidLen)) &&
	    (CHAN_PropertyCheck(pAd, pAd->MlmeAux.Channel, CHANNEL_NO_IBSS) == FALSE)) {
		struct wifi_dev *wdev = &pAd->StaCfg.wdev;

		/* reset all the timers */
		RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &TimerCancelled);
		RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);

		/* Start a new IBSS. All IBSS parameters are decided now */
		DBGPRINT(RT_DEBUG_TRACE,
			 ("MlmeStartReqAction - Start a new IBSS. All IBSS parameters are decided now....\n"));
		pAd->MlmeAux.BssType = BSS_ADHOC;
		NdisMoveMemory(pAd->MlmeAux.Ssid, Ssid, SsidLen);
		pAd->MlmeAux.SsidLen = SsidLen;

#ifdef IWSC_SUPPORT
		if (pAd->StaCfg.IWscInfo.bDoNotChangeBSSID)
			pAd->StaCfg.IWscInfo.bDoNotChangeBSSID = FALSE;
		else
#endif /* IWSC_SUPPORT */
		{
			/* generate a radom number as BSSID */
			MacAddrRandomBssid(pAd, pAd->MlmeAux.Bssid);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("MlmeStartReqAction - generate a radom number as BSSID\n"));
		}

		Privacy = (wdev->WepStatus == Ndis802_11WEPEnabled) ||
		    (wdev->WepStatus == Ndis802_11TKIPEnable) ||
		    (wdev->WepStatus == Ndis802_11AESEnable);
		pAd->MlmeAux.CapabilityInfo =
		    CAP_GENERATE(0, 1, Privacy,
				 (pAd->CommonCfg.TxPreamble == Rt802_11PreambleShort), 1, 0);
		pAd->MlmeAux.BeaconPeriod = pAd->CommonCfg.BeaconPeriod;
		pAd->MlmeAux.AtimWin = pAd->StaCfg.AtimWin;
		pAd->MlmeAux.Channel = pAd->CommonCfg.Channel;

		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
		pAd->MlmeAux.CentralChannel = pAd->CommonCfg.CentralChannel;

		pAd->MlmeAux.SupRateLen = pAd->CommonCfg.SupRateLen;
		NdisMoveMemory(pAd->MlmeAux.SupRate, pAd->CommonCfg.SupRate,
			       MAX_LEN_OF_SUPPORTED_RATES);
		RTMPCheckRates(pAd, pAd->MlmeAux.SupRate, &pAd->MlmeAux.SupRateLen);
		pAd->MlmeAux.ExtRateLen = pAd->CommonCfg.ExtRateLen;
		NdisMoveMemory(pAd->MlmeAux.ExtRate, pAd->CommonCfg.ExtRate,
			       MAX_LEN_OF_SUPPORTED_RATES);
		RTMPCheckRates(pAd, pAd->MlmeAux.ExtRate, &pAd->MlmeAux.ExtRateLen);
		if (WMODE_CAP_N(pAd->CommonCfg.PhyMode) && (pAd->StaCfg.bAdhocN == TRUE)) {
			RTMPUpdateHTIE(&pAd->CommonCfg.DesiredHtPhy,
				       &wdev->DesiredHtPhyInfo.MCSSet[0],
				       &pAd->MlmeAux.HtCapability, &pAd->MlmeAux.AddHtInfo);
			pAd->MlmeAux.HtCapabilityLen = sizeof(HT_CAPABILITY_IE);
			/* Not turn pAd->StaActive.SupportedHtPhy.bHtEnable = TRUE here. */
			DBGPRINT(RT_DEBUG_TRACE,
				 ("SYNC -pAd->StaActive.SupportedHtPhy.bHtEnable = TRUE\n"));
#ifdef DOT11_VHT_AC
			if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode) && (pAd->MlmeAux.Channel > 14)) {
				build_vht_cap_ie(pAd, (UCHAR *) &pAd->MlmeAux.vht_cap);
				pAd->MlmeAux.vht_cap_len = sizeof(VHT_CAP_IE);
			}
#endif /* DOT11_VHT_AC */
		} else {
			pAd->MlmeAux.HtCapabilityLen = 0;
			pAd->StaActive.SupportedPhyInfo.bHtEnable = FALSE;
			NdisZeroMemory(&pAd->StaActive.SupportedPhyInfo.MCSSet[0], 16);
		}
		/* temporarily not support QOS in IBSS */
		NdisZeroMemory(&pAd->MlmeAux.APEdcaParm, sizeof(EDCA_PARM));
		NdisZeroMemory(&pAd->MlmeAux.APQbssLoad, sizeof(QBSS_LOAD_PARM));
		NdisZeroMemory(&pAd->MlmeAux.APQosCapability, sizeof(QOS_CAPABILITY_PARM));

		AsicSwitchChannel(pAd, pAd->MlmeAux.Channel, FALSE);
		AsicLockChannel(pAd, pAd->MlmeAux.Channel);

		DBGPRINT(RT_DEBUG_TRACE,
			 ("SYNC - MlmeStartReqAction(ch= %d,sup rates= %d, ext rates=%d)\n",
			  pAd->MlmeAux.Channel, pAd->MlmeAux.SupRateLen, pAd->MlmeAux.ExtRateLen));

		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
		Status = MLME_SUCCESS;
		MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_START_CONF, 2, &Status, 0);
	} else {
		DBGPRINT_ERR(("SYNC - MlmeStartReqAction() sanity check fail.\n"));
		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
		Status = MLME_INVALID_FORMAT;
		MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_START_CONF, 2, &Status, 0);
	}

	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);
}

/*
	==========================================================================
	Description:
		peer sends beacon back when scanning
	==========================================================================
 */
VOID PeerBeaconAtScanAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	PFRAME_802_11 pFrame;
	USHORT LenVIE;
	UCHAR *VarIE = NULL;
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	BCN_IE_LIST *ie_list = NULL;
	MESH_SCAN_INFO_STRUCT MeshScanInfo;

	NdisZeroMemory(&MeshScanInfo, sizeof(MESH_SCAN_INFO_STRUCT));

	os_alloc_mem(NULL, (UCHAR **) &ie_list, sizeof(BCN_IE_LIST));
	if (!ie_list) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Alloc ie_list failed!\n", __func__));
		return;
	}
	NdisZeroMemory((UCHAR *) ie_list, sizeof(BCN_IE_LIST));

	/* Init Variable IE structure */
	os_alloc_mem(NULL, (UCHAR **) &VarIE, MAX_VIE_LEN);
	if (VarIE == NULL)
		goto LabelErr;
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;

	pFrame = (PFRAME_802_11) Elem->Msg;

	if (PeerBeaconAndProbeRspSanity(pAd,
					&Elem->Msg[0], Elem->MsgLen,
					Elem->Channel, ie_list, &LenVIE, pVIE)) {
		ULONG Idx = 0;
		CHAR Rssi = 0;

		/*
		*filter beacon or rsp signal from non-adjacent channels,
		*which are much lower than real signal and these signals
		*received by hardware are invalid.
		*we don't need to process them in later function.
		*/
		if (ie_list->Channel < 14) {
			if (pAd->LatchRfRegs.Channel - ie_list->Channel > 1 ||
				ie_list->Channel - pAd->LatchRfRegs.Channel > 1)
				goto LabelOK;
		} else {
			if (pAd->LatchRfRegs.Channel - ie_list->Channel > 4 ||
				ie_list->Channel - pAd->LatchRfRegs.Channel > 4)
				goto LabelOK;
		}

		Idx = BssTableSearch(&pAd->ScanTab, &ie_list->Bssid[0], ie_list->Channel);

#if defined(RT_CFG80211_SUPPORT) && defined(JUNGLE_SUPPORT)
		CFG80211_UpdateChFlagsByBeacon(pAd, ie_list->Channel);
#endif
		Rssi = RTMPMaxRssi(pAd, ConvertToRssi(pAd, Elem->Rssi0, RSSI_0),
				   ConvertToRssi(pAd, Elem->Rssi1, RSSI_1),
				   ConvertToRssi(pAd, Elem->Rssi2, RSSI_2));


		if ((ie_list->HtCapabilityLen > 0) || (ie_list->PreNHtCapabilityLen > 0))
			ie_list->HtCapabilityLen = SIZE_HT_CAP_IE;

		Idx =
		    BssTableSetEntry(pAd, &pAd->ScanTab, ie_list, Rssi, &MeshScanInfo, LenVIE,
				     pVIE);
		/* TODO: Check for things need to do when enable "DOT11V_WNM_SUPPORT" */
		/* Check if this scan channel is the effeced channel */
		if (INFRA_ON(pAd)
		    && (pAd->CommonCfg.bBssCoexEnable == TRUE)
		    && ((ie_list->Channel > 0) && (ie_list->Channel <= 14))) {
			int chListIdx;

			/* find the channel list idx by the channel number */
			for (chListIdx = 0; chListIdx < pAd->ChannelListNum; chListIdx++) {
				if (ie_list->Channel == pAd->ChannelList[chListIdx].Channel)
					break;
			}

			if (chListIdx < pAd->ChannelListNum) {
				/*
				   If this channel is effected channel for the
				   20/40 coex operation. Check the related IEs.
				 */
				if (pAd->ChannelList[chListIdx].bEffectedChannel == TRUE) {
					UCHAR RegClass;
					OVERLAP_BSS_SCAN_IE BssScan;

					/* Read Beacon's Reg Class IE if any. */
					PeerBeaconAndProbeRspSanity2(pAd, Elem->Msg, Elem->MsgLen,
								     &BssScan, ie_list, &RegClass);
					TriEventTableSetEntry(pAd, &pAd->CommonCfg.TriggerEventTab,
							      &ie_list->Bssid[0],
							      &ie_list->HtCapability,
							      ie_list->HtCapabilityLen, RegClass,
							      ie_list->Channel);
				}
			}
		}
		if (Idx != BSS_NOT_FOUND && Idx < MAX_LEN_OF_BSS_TABLE) {
			BSS_ENTRY *pBssEntry = &pAd->ScanTab.BssEntry[Idx];

			NdisMoveMemory(pBssEntry->PTSF, &Elem->Msg[24], 4);
			NdisMoveMemory(&pBssEntry->TTSF[0], &Elem->TimeStamp.u.LowPart, 4);
			NdisMoveMemory(&pBssEntry->TTSF[4], &Elem->TimeStamp.u.LowPart, 4);

			pBssEntry->MinSNR = Elem->Signal % 10;
			if (pBssEntry->MinSNR == 0)
				pBssEntry->MinSNR = -5;

			NdisMoveMemory(pBssEntry->MacAddr, &ie_list->Addr2[0], MAC_ADDR_LEN);

			if ((pFrame->Hdr.FC.SubType == SUBTYPE_PROBE_RSP) && (LenVIE != 0)) {
				pBssEntry->VarIeFromProbeRspLen = 0;
				if (pBssEntry->pVarIeFromProbRsp) {
					pBssEntry->VarIeFromProbeRspLen = LenVIE;
					RTMPZeroMemory(pBssEntry->pVarIeFromProbRsp, MAX_VIE_LEN);
					RTMPMoveMemory(pBssEntry->pVarIeFromProbRsp, pVIE, LenVIE);
				}
			}
		}
#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
		if (ie_list->Channel != 0)
			Elem->Channel = ie_list->Channel;

		RT_CFG80211_SCANNING_INFORM(pAd, Idx, Elem->Channel, (UCHAR *) pFrame,
					    Elem->MsgLen, Rssi);
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */
	}
	/* sanity check fail, ignored */
	goto LabelOK;

LabelErr:
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __func__));

LabelOK:
	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);
	if (ie_list != NULL)
		os_free_mem(NULL, ie_list);
}

/*
	==========================================================================
	Description:
		When waiting joining the (I)BSS, beacon received from external
	==========================================================================
 */
VOID PeerBeaconAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem)
{
	BOOLEAN TimerCancelled;
	USHORT LenVIE;
	USHORT Status;
	UCHAR *VarIE = NULL;
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	ULONG Idx = 0;
	CHAR Rssi = 0;

	UCHAR CentralChannel;
	BOOLEAN bAllowNrate = FALSE;
	BCN_IE_LIST *ie_list = NULL;

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **) &ie_list, sizeof(BCN_IE_LIST));
	if (ie_list == NULL)
		goto LabelErr;
	NdisZeroMemory(ie_list, sizeof(BCN_IE_LIST));

	os_alloc_mem(NULL, (UCHAR **) &VarIE, MAX_VIE_LEN);
	if (VarIE == NULL)
		goto LabelErr;
	/* Init Variable IE structure */
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;

	if (PeerBeaconAndProbeRspSanity(pAd,
					Elem->Msg,
					Elem->MsgLen, Elem->Channel, ie_list, &LenVIE, pVIE)) {
		/* Disqualify 11b only adhoc when we are in 11g only adhoc mode */
		if ((ie_list->BssType == BSS_ADHOC) &&
		    WMODE_EQUAL(pAd->CommonCfg.PhyMode, WMODE_G) &&
		    ((ie_list->SupRateLen + ie_list->ExtRateLen) < 12))
			goto LabelOK;
#if defined(RT_CFG80211_SUPPORT) && defined(JUNGLE_SUPPORT)
		CFG80211_UpdateChFlagsByBeacon(pAd, ie_list->Channel);
#endif

		/*
		   BEACON from desired BSS/IBSS found. We should be able to decide most
		   BSS parameters here.
		   Q. But what happen if this JOIN doesn't conclude a successful ASSOCIATEION?
		   Do we need to receover back all parameters belonging to previous BSS?
		   A. Should be not. There's no back-door recover to previous AP. It still need
		   a new JOIN-AUTH-ASSOC sequence.
		 */
		if (MAC_ADDR_EQUAL(pAd->MlmeAux.Bssid, &ie_list->Bssid[0])) {
			struct wifi_dev *wdev = &pAd->StaCfg.wdev;

			DBGPRINT(RT_DEBUG_OFF, ("%s():receive desired BEACON,Channel=%d\n",
						  __func__, ie_list->Channel));
			RTMPCancelTimer(&pAd->MlmeAux.BeaconTimer, &TimerCancelled);

			/* Update RSSI to prevent No signal display when cards first initialized */
			pAd->StaCfg.RssiSample.LastRssi0 = ConvertToRssi(pAd, Elem->Rssi0, RSSI_0);
			pAd->StaCfg.RssiSample.LastRssi1 = ConvertToRssi(pAd, Elem->Rssi1, RSSI_1);
			pAd->StaCfg.RssiSample.LastRssi2 = ConvertToRssi(pAd, Elem->Rssi2, RSSI_2);
			pAd->StaCfg.RssiSample.AvgRssi0 = pAd->StaCfg.RssiSample.LastRssi0;
			pAd->StaCfg.RssiSample.AvgRssi0X8 = pAd->StaCfg.RssiSample.AvgRssi0 << 3;
			pAd->StaCfg.RssiSample.AvgRssi1 = pAd->StaCfg.RssiSample.LastRssi1;
			pAd->StaCfg.RssiSample.AvgRssi1X8 = pAd->StaCfg.RssiSample.AvgRssi1 << 3;
			pAd->StaCfg.RssiSample.AvgRssi2 = pAd->StaCfg.RssiSample.LastRssi2;
			pAd->StaCfg.RssiSample.AvgRssi2X8 = pAd->StaCfg.RssiSample.AvgRssi2 << 3;

			/* Still update RSSI for BSS including found BSS */
			Rssi = RTMPMaxRssi(pAd, ConvertToRssi(pAd, Elem->Rssi0, RSSI_0),
					   ConvertToRssi(pAd, Elem->Rssi1, RSSI_1),
					   ConvertToRssi(pAd, Elem->Rssi2, RSSI_2));

			/*
			   We need to check if SSID only set to any, then we can record the current SSID.
			   Otherwise will cause hidden SSID association failed.
			 */
			if (pAd->MlmeAux.SsidLen == 0) {
				NdisMoveMemory(pAd->MlmeAux.Ssid, ie_list->Ssid, ie_list->SsidLen);
				pAd->MlmeAux.SsidLen = ie_list->SsidLen;
			} else {
				Idx = BssSsidTableSearch(&pAd->ScanTab, ie_list->Bssid,
							 pAd->MlmeAux.Ssid, pAd->MlmeAux.SsidLen,
							 ie_list->Channel);

				if (Idx == BSS_NOT_FOUND) {
					Idx =
					    BssTableSetEntry(pAd, &pAd->ScanTab, ie_list, Rssi,
							     NULL, LenVIE, pVIE);
					if (Idx != BSS_NOT_FOUND && Idx < MAX_LEN_OF_BSS_TABLE) {
						NdisMoveMemory(pAd->ScanTab.BssEntry[Idx].PTSF,
							       &Elem->Msg[24], 4);
						NdisMoveMemory(&pAd->ScanTab.BssEntry[Idx].TTSF[0],
							       &Elem->TimeStamp.u.LowPart, 4);
						NdisMoveMemory(&pAd->ScanTab.BssEntry[Idx].TTSF[4],
							       &Elem->TimeStamp.u.LowPart, 4);
						ie_list->CapabilityInfo =
						    pAd->ScanTab.BssEntry[Idx].CapabilityInfo;

						pAd->ScanTab.BssEntry[Idx].MinSNR =
						    Elem->Signal % 10;
						if (pAd->ScanTab.BssEntry[Idx].MinSNR == 0)
							pAd->ScanTab.BssEntry[Idx].MinSNR = -5;

						NdisMoveMemory(pAd->ScanTab.BssEntry[Idx].MacAddr,
							       ie_list->Addr2, MAC_ADDR_LEN);
					}
				} else {
#ifdef WPA_SUPPLICANT_SUPPORT
					if (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP &
					    WPA_SUPPLICANT_ENABLE_WPS)
						;
					else
#endif /* WPA_SUPPLICANT_SUPPORT */
#ifdef WSC_STA_SUPPORT
						if (pAd->StaCfg.WscControl.WscState !=
						     WSC_STATE_OFF)
							;
					else
#endif /* WSC_STA_SUPPORT */
					{

						/*
						   Check if AP privacy is different Staion, if yes,
						   start a new scan and ignore the frame
						   (often happen during AP change privacy at short time)
						 */
						if ((((wdev->WepStatus !=
						       Ndis802_11WEPDisabled) << 4) ^
						     ie_list->CapabilityInfo) & 0x0010) {
#if defined(NATIVE_WPA_SUPPLICANT_SUPPORT)
							/* When using -Dwext and trigger WPS, do not check security. */
							if (ie_list->selReg == 0)
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */
								{
								MLME_SCAN_REQ_STRUCT ScanReq;

								DBGPRINT(RT_DEBUG_TRACE,
									 ("%s:AP privacy %d is differenct from STA privacy%d\n",
									  __func__,
									  (ie_list->CapabilityInfo &
									   0x0010)
									  >> 4,
									  wdev->WepStatus !=
									  Ndis802_11WEPDisabled));
								ScanParmFill(pAd, &ScanReq,
									     (PSTRING) pAd->MlmeAux.
									     Ssid,
									     pAd->MlmeAux.SsidLen,
									     BSS_ANY, SCAN_ACTIVE);
								MlmeEnqueue(pAd, SYNC_STATE_MACHINE,
									    MT2_MLME_SCAN_REQ,
									    sizeof
									    (MLME_SCAN_REQ_STRUCT),
									    &ScanReq, 0);
								pAd->Mlme.CntlMachine.CurrState =
								    CNTL_WAIT_OID_LIST_SCAN;
								NdisGetSystemUpTime(&pAd->StaCfg.
										    LastScanTime);
								pAd->Mlme.SyncMachine.CurrState =
								    SYNC_IDLE;
								RTMP_MLME_HANDLER(pAd);
								goto LabelOK;
							}
						}
					}

					/* Multiple SSID case, used correct CapabilityInfo */
					if (Idx < MAX_LEN_OF_BSS_TABLE)
						ie_list->CapabilityInfo =
						    pAd->ScanTab.BssEntry[Idx].CapabilityInfo;
					else {
						DBGPRINT(RT_DEBUG_ERROR,
							 ("%s: Idx >= MAX_LEN_OF_BSS_TABLE.\n",
							  __func__));
						return;
					}
				}
			}
			pAd->MlmeAux.CapabilityInfo =
			    ie_list->CapabilityInfo & SUPPORTED_CAPABILITY_INFO;
			pAd->MlmeAux.BssType = ie_list->BssType;
			pAd->MlmeAux.BeaconPeriod = ie_list->BeaconPeriod;

			/*
			   Some AP may carrys wrong beacon interval (ex. 0) in Beacon IE.
			   We need to check here for preventing divided by 0 error.
			 */
			if (pAd->MlmeAux.BeaconPeriod == 0)
				pAd->MlmeAux.BeaconPeriod = 100;

			pAd->MlmeAux.Channel = ie_list->Channel;
			pAd->MlmeAux.AtimWin = ie_list->AtimWin;
			pAd->MlmeAux.CfpPeriod = ie_list->CfParm.CfpPeriod;
			pAd->MlmeAux.CfpMaxDuration = ie_list->CfParm.CfpMaxDuration;
			pAd->MlmeAux.APRalinkIe = ie_list->RalinkIe;

			/*
			   Copy AP's supported rate to MlmeAux for creating assoication request
			   Also filter out not supported rate
			 */
			pAd->MlmeAux.SupRateLen = ie_list->SupRateLen;
			NdisMoveMemory(pAd->MlmeAux.SupRate, ie_list->SupRate, ie_list->SupRateLen);
			RTMPCheckRates(pAd, pAd->MlmeAux.SupRate, &pAd->MlmeAux.SupRateLen);
			pAd->MlmeAux.ExtRateLen = ie_list->ExtRateLen;
			NdisMoveMemory(pAd->MlmeAux.ExtRate, ie_list->ExtRate, ie_list->ExtRateLen);
			RTMPCheckRates(pAd, pAd->MlmeAux.ExtRate, &pAd->MlmeAux.ExtRateLen);

			NdisZeroMemory(pAd->StaActive.SupportedPhyInfo.MCSSet, 16);

			pAd->MlmeAux.ExtCapInfoLen = ie_list->ExtCapInfoLen;
			DBGPRINT(RT_DEBUG_TRACE, ("%s: MlmeAux.ExtCapInfoLen=%d\n",
				 __func__, pAd->MlmeAux.ExtCapInfoLen));

			/*  Get the ext capability info element */
			NdisMoveMemory(&pAd->MlmeAux.ExtCapInfo, &ie_list->ExtCapInfo,
				       sizeof(ie_list->ExtCapInfo));

#ifdef DOT11_VHT_AC
			pAd->StaActive.SupportedPhyInfo.bVhtEnable = FALSE;
			pAd->StaActive.SupportedPhyInfo.vht_bw = VHT_BW_2040;
#endif /* DOT11_VHT_AC */

			DBGPRINT(RT_DEBUG_TRACE,
				 ("MlmeAux.ExtCapInfo=%d\n",
				  pAd->MlmeAux.ExtCapInfo.BssCoexistMgmtSupport));
			if (pAd->CommonCfg.bBssCoexEnable == TRUE)
				pAd->CommonCfg.ExtCapIE.BssCoexistMgmtSupport = 1;

			if (((wdev->WepStatus != Ndis802_11WEPEnabled)
			     && (wdev->WepStatus != Ndis802_11TKIPEnable))
			    || (pAd->CommonCfg.HT_DisallowTKIP == FALSE)) {
				if ((pAd->StaCfg.BssType == BSS_INFRA) ||
				    ((pAd->StaCfg.BssType == BSS_ADHOC)
				     && (pAd->StaCfg.bAdhocN == TRUE)))
					bAllowNrate = TRUE;
			}

			pAd->MlmeAux.NewExtChannelOffset = ie_list->NewExtChannelOffset;
			pAd->MlmeAux.HtCapabilityLen = ie_list->HtCapabilityLen;

			CentralChannel = ie_list->Channel;

			RTMPZeroMemory(&pAd->MlmeAux.HtCapability, SIZE_HT_CAP_IE);
			/* filter out un-supported ht rates */
			if (((ie_list->HtCapabilityLen > 0) || (ie_list->PreNHtCapabilityLen > 0))
			    && (wdev->DesiredHtPhyInfo.bHtEnable)
			    && (WMODE_CAP_N(pAd->CommonCfg.PhyMode) && bAllowNrate)) {

				if (pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20
				    && pAd->CommonCfg.default_bw != BW_20) {
					pAd->CommonCfg.RegTransmitSetting.field.BW =
						pAd->CommonCfg.default_bw;
					SetCommonHT(pAd);
				}

				RTMPMoveMemory(&pAd->MlmeAux.AddHtInfo, &ie_list->AddHtInfo,
					       SIZE_ADD_HT_INFO_IE);

				/* StaActive.SupportedHtPhy.MCSSet stores Peer AP's 11n Rx capability */
				NdisMoveMemory(pAd->StaActive.SupportedPhyInfo.MCSSet,
					       ie_list->HtCapability.MCSSet, 16);
				pAd->MlmeAux.NewExtChannelOffset = ie_list->NewExtChannelOffset;
				pAd->MlmeAux.HtCapabilityLen = SIZE_HT_CAP_IE;
				pAd->StaActive.SupportedPhyInfo.bHtEnable = TRUE;
				if (ie_list->PreNHtCapabilityLen > 0)
					pAd->StaActive.SupportedPhyInfo.bPreNHt = TRUE;
				RTMPCheckHt(pAd, BSSID_WCID, &ie_list->HtCapability,
					    &ie_list->AddHtInfo);
				/* Copy AP Parameter to StaActive.  This is also in LinkUp. */
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s():(MpduDensity=%d, MaxRAmpduFactor=%d, BW=%d)\n",
					  __func__, pAd->StaActive.SupportedHtPhy.MpduDensity,
					  pAd->StaActive.SupportedHtPhy.MaxRAmpduFactor,
					  ie_list->HtCapability.HtCapInfo.ChannelWidth));

				if (ie_list->AddHtInfoLen > 0) {
					/* Check again the Bandwidth capability of this AP. */
					CentralChannel = get_cent_ch_by_htinfo(pAd,
									       &ie_list->AddHtInfo,
									       &ie_list->
									       HtCapability);

					DBGPRINT(RT_DEBUG_OFF,
						 ("%s():AP HT-CtrlChannel=%d, CentralChannel=>%d\n",
						  __func__, ie_list->AddHtInfo.ControlChan,
						  CentralChannel));
				}
#ifdef DOT11_VHT_AC
				if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode) &&
				    (pAd->MlmeAux.Channel > 14) &&
				    (ie_list->vht_cap_len) && (ie_list->vht_op_len)) {
					VHT_OP_INFO *vht_op = &ie_list->vht_op_ie.vht_op_info;

					RTMPZeroMemory(&pAd->MlmeAux.vht_cap, sizeof(VHT_CAP_IE));
					NdisMoveMemory(&pAd->MlmeAux.vht_cap, &ie_list->vht_cap_ie,
						       ie_list->vht_cap_len);
					pAd->MlmeAux.vht_cap_len = ie_list->vht_cap_len;

					RTMPZeroMemory(&pAd->MlmeAux.vht_op, sizeof(VHT_OP_IE));
					NdisMoveMemory(&pAd->MlmeAux.vht_op, &ie_list->vht_op_ie,
						       ie_list->vht_op_len);
					pAd->MlmeAux.vht_op_len = ie_list->vht_op_len;

					pAd->StaActive.SupportedPhyInfo.bVhtEnable = TRUE;

					if (vht_op->ch_width == 0) {
						pAd->StaActive.SupportedPhyInfo.vht_bw =
						    VHT_BW_2040;
					} else if (vht_op->ch_width == 1) {
#ifdef CUSTOMIZED_BW_SETTING
						if (RTMP_GetCustomizedChannelBw
						    (pAd, pAd->MlmeAux.Channel) != BW_80) {
							pAd->StaActive.SupportedPhyInfo.vht_bw =
							    VHT_BW_2040;
						} else
#endif /* CUSTOMIZED_BW_SETTING */
						{
							CentralChannel = vht_op->center_freq_1;
							pAd->StaActive.SupportedPhyInfo.vht_bw =
							    VHT_BW_80;
						}
					}

					DBGPRINT(RT_DEBUG_OFF, ("%s(): CentralChannel=>%d\n",
								__func__, CentralChannel));
				}
#endif /* DOT11_VHT_AC */
			} else {
				/* To prevent error, let legacy AP must have same CentralChannel and Channel. */
				if ((ie_list->HtCapabilityLen == 0)
				    && (ie_list->PreNHtCapabilityLen == 0))
					pAd->MlmeAux.CentralChannel = pAd->MlmeAux.Channel;

				pAd->StaActive.SupportedPhyInfo.bHtEnable = FALSE;
#ifdef DOT11_VHT_AC
				pAd->StaActive.SupportedPhyInfo.bVhtEnable = FALSE;
				pAd->StaActive.SupportedPhyInfo.vht_bw = VHT_BW_2040;
#endif /* DOT11_VHT_AC */
				pAd->MlmeAux.NewExtChannelOffset = 0xff;
				RTMPZeroMemory(&pAd->MlmeAux.HtCapability, SIZE_HT_CAP_IE);
				pAd->MlmeAux.HtCapabilityLen = 0;
				RTMPZeroMemory(&pAd->MlmeAux.AddHtInfo, SIZE_ADD_HT_INFO_IE);
			}
			/* Fix if AP is HT40 and we are HT20 only, we shall stay at HT20 mode */
			/*some AP like Dlink DIR-855/655
			* Secondary Channel Offset != 0 && STA Channel Width = 0 case */
			if (pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20
				|| pAd->MlmeAux.HtCapability.HtCapInfo.ChannelWidth == BW_20)
					CentralChannel = pAd->MlmeAux.Channel;

			pAd->hw_cfg.cent_ch = CentralChannel;

			pAd->MlmeAux.CentralChannel = pAd->hw_cfg.cent_ch;

			DBGPRINT(RT_DEBUG_OFF,
				 ("%s(): Set CentralChannel=%d\n", __func__,
				  pAd->MlmeAux.CentralChannel));

			RTMPUpdateMlmeRate(pAd);

			/* copy QOS related information */
			if ((pAd->CommonCfg.bWmmCapable)
			    || WMODE_CAP_N(pAd->CommonCfg.PhyMode)
			    ) {
				NdisMoveMemory(&pAd->MlmeAux.APEdcaParm, &ie_list->EdcaParm,
					       sizeof(EDCA_PARM));
				NdisMoveMemory(&pAd->MlmeAux.APQbssLoad, &ie_list->QbssLoad,
					       sizeof(QBSS_LOAD_PARM));
				NdisMoveMemory(&pAd->MlmeAux.APQosCapability,
					       &ie_list->QosCapability,
					       sizeof(QOS_CAPABILITY_PARM));
			} else {
				NdisZeroMemory(&pAd->MlmeAux.APEdcaParm, sizeof(EDCA_PARM));
				NdisZeroMemory(&pAd->MlmeAux.APQbssLoad, sizeof(QBSS_LOAD_PARM));
				NdisZeroMemory(&pAd->MlmeAux.APQosCapability,
					       sizeof(QOS_CAPABILITY_PARM));
			}

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s(): - after JOIN, SupRateLen=%d, ExtRateLen=%d\n",
				  __func__, pAd->MlmeAux.SupRateLen, pAd->MlmeAux.ExtRateLen));

			if (ie_list->AironetCellPowerLimit != 0xFF) {
				/* We need to change our TxPower for CCX 2.0 AP Control of Client Transmit Power */
				ChangeToCellPowerLimit(pAd, ie_list->AironetCellPowerLimit);
			} else	/* Used the default TX Power Percentage. */
				pAd->CommonCfg.TxPowerPercentage = pAd->CommonCfg.TxPowerDefault;

			if (pAd->StaCfg.BssType == BSS_INFRA) {
				UINT8 InfraAP_BW;
				UCHAR BwFallBack = 0;

				if (pAd->MlmeAux.HtCapability.HtCapInfo.ChannelWidth == BW_40)
					InfraAP_BW = BW_40;
				else
					InfraAP_BW = BW_20;

				AdjustChannelRelatedValue(pAd,
							  &BwFallBack,
							  BSS0,
							  InfraAP_BW,
							  pAd->MlmeAux.Channel,
							  pAd->MlmeAux.CentralChannel);
			}

			pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
			Status = MLME_SUCCESS;
			MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_JOIN_CONF, 2, &Status, 0);

#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
			if (RTMPEqualMemory(ie_list->Ssid, "DIRECT-", 7))
				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s P2P_SCANNING: %s [%lu]\n", __func__,
					  ie_list->Ssid, Idx));

			if (ie_list->Channel != 0)
				Elem->Channel = ie_list->Channel;

			RT_CFG80211_SCANNING_INFORM(pAd, Idx, Elem->Channel, Elem->Msg,
						    Elem->MsgLen, Rssi);
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */
		}
		/* not to me BEACON, ignored */
	}
	/* sanity check fail, ignore this frame */

	goto LabelOK;

LabelErr:
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __func__));

LabelOK:
	if (ie_list != NULL)
		os_free_mem(NULL, ie_list);
	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);

}

/*
	==========================================================================
	Description:
		receive BEACON from peer

	IRQL = DISPATCH_LEVEL

	==========================================================================
 */
VOID PeerBeacon(RTMP_ADAPTER *pAd, MLME_QUEUE_ELEM *Elem)
{
	UCHAR index = 0;
	/*sync @125579*/
	USHORT LenVIE;
	UCHAR *VarIE = NULL;	/* Total VIE length = MAX_VIE_LEN - -5 */
	NDIS_802_11_VARIABLE_IEs *pVIE = NULL;
	struct wifi_dev *wdev = &pAd->StaCfg.wdev;
	BCN_IE_LIST *bcn_ie_list = NULL;
#ifdef CFG_TDLS_SUPPORT
	PCFG_TDLS_STRUCT pCFG_TDLS;
#endif /* CFG_TDLS_SUPPORT */

#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		return;
#endif /* RALINK_ATE */

	if (!(INFRA_ON(pAd) || ADHOC_ON(pAd)
#if defined(RT_CFG80211_P2P_SUPPORT) && defined(SUPPORT_ACS_ALL_CHANNEL_RANK)
	      || (pAd->ApCfg.bAutoChannelAtBootup && pAd->ApCfg.bAutoChannelScaned == 0)
#endif /* SUPPORT_ACS_ALL_CHANNEL_RANK */
	    ))
		return;

	/* allocate memory */
	os_alloc_mem(NULL, (UCHAR **) &bcn_ie_list, sizeof(BCN_IE_LIST));
	if (bcn_ie_list == NULL)
		goto LabelErr;
	NdisZeroMemory(bcn_ie_list, sizeof(BCN_IE_LIST));

	os_alloc_mem(NULL, (UCHAR **) &VarIE, MAX_VIE_LEN);
	if (VarIE == NULL)
		goto LabelErr;
	/* Init Variable IE structure */
	pVIE = (PNDIS_802_11_VARIABLE_IEs) VarIE;
	pVIE->Length = 0;

	if (PeerBeaconAndProbeRspSanity(pAd,
					Elem->Msg,
					Elem->MsgLen, Elem->Channel, bcn_ie_list, &LenVIE, pVIE)) {
		BOOLEAN is_my_bssid, is_my_ssid;
		ULONG Bssidx, Now;
		BSS_ENTRY *pBss;
		CHAR RealRssi = RTMPMaxRssi(pAd, ConvertToRssi(pAd, Elem->Rssi0, RSSI_0),
					    ConvertToRssi(pAd, Elem->Rssi1, RSSI_1),
					    ConvertToRssi(pAd, Elem->Rssi2, RSSI_2));

		is_my_bssid =
		    MAC_ADDR_EQUAL(bcn_ie_list->Bssid, pAd->CommonCfg.Bssid) ? TRUE : FALSE;
		is_my_ssid =
		    SSID_EQUAL(bcn_ie_list->Ssid, bcn_ie_list->SsidLen, pAd->CommonCfg.Ssid,
			       pAd->CommonCfg.SsidLen) ? TRUE : FALSE;
#if defined(RT_CFG80211_SUPPORT) && defined(JUNGLE_SUPPORT)
		CFG80211_UpdateChFlagsByBeacon(pAd, bcn_ie_list->Channel);
#endif
#if defined(RT_CFG80211_P2P_SUPPORT) && defined(SUPPORT_ACS_ALL_CHANNEL_RANK)
		if (bcn_ie_list->AddHtInfoLen != 0)
			bcn_ie_list->Channel = bcn_ie_list->AddHtInfo.ControlChan;
		/*
		   DBGPRINT(RT_DEBUG_TRACE, ("(ACS)%s: AutoChannel_Channel=%d, bcn_ie_list->Channel=%d, "
		   "SSID=[%s], bssid=[%02x %02x %02x %02x %02x %02x], RSSI=%d\n",
		   __FUNCTION__, __LINE__ , pAd->ApCfg.AutoChannel_Channel,
		   bcn_ie_list->Channel, bcn_ie_list->Ssid, bcn_ie_list->Bssid[0],
		   bcn_ie_list->Bssid[1], bcn_ie_list->Bssid[2], bcn_ie_list->Bssid[3],
		   bcn_ie_list->Bssid[4], bcn_ie_list->Bssid[5], RealRssi));
		 */
		if (bcn_ie_list->Channel == pAd->ApCfg.AutoChannel_Channel) {
			if (AutoChBssSearchWithSSID
			    (pAd, bcn_ie_list->Bssid, (PUCHAR) bcn_ie_list->Ssid,
			     bcn_ie_list->SsidLen, bcn_ie_list->Channel) == BSS_NOT_FOUND)
				pAd->pChannelInfo->ApCnt[pAd->ApCfg.current_channel_index]++;
			AutoChBssInsertEntry(pAd, bcn_ie_list->Bssid, bcn_ie_list->Ssid,
					     bcn_ie_list->SsidLen, bcn_ie_list->Channel,
					     bcn_ie_list->NewExtChannelOffset, RealRssi);
		}
#endif /* SUPPORT_ACS_ALL_CHANNEL_RANK */


		/* ignore BEACON not for my SSID */
		if ((!is_my_ssid) && (!is_my_bssid)) {
#ifdef ED_MONITOR
			if (pAd->ed_chk && INFRA_ON(pAd)) {
				;	/* also update my scan table, even this not for me */
			} else
#endif /* ED_MONITOR */

				goto LabelOK;
		}

		/* It means STA waits disassoc completely from this AP, ignores this beacon. */
		if (pAd->Mlme.CntlMachine.CurrState == CNTL_WAIT_DISASSOC)
			goto LabelOK;

		/* Copy Control channel for this BSSID. */
		if (bcn_ie_list->AddHtInfoLen != 0)
			bcn_ie_list->Channel = bcn_ie_list->AddHtInfo.ControlChan;

		if ((bcn_ie_list->HtCapabilityLen > 0) || (bcn_ie_list->PreNHtCapabilityLen > 0))
			bcn_ie_list->HtCapabilityLen = SIZE_HT_CAP_IE;

		/* Housekeeping "SsidBssTab" table for later-on ROAMing usage. */
		Bssidx =
		    BssTableSearchWithSSID(&pAd->MlmeAux.SsidBssTab, bcn_ie_list->Bssid,
					   bcn_ie_list->Ssid, bcn_ie_list->SsidLen,
					   bcn_ie_list->Channel);
		if (Bssidx == BSS_NOT_FOUND) {
			/* discover new AP of this network, create BSS entry */
			Bssidx =
			    BssTableSetEntry(pAd, &pAd->MlmeAux.SsidBssTab, bcn_ie_list, RealRssi,
					     NULL, LenVIE, pVIE);
			if (Bssidx == BSS_NOT_FOUND)
				;
			else {
				BSS_ENTRY *pBssEntry = &pAd->MlmeAux.SsidBssTab.BssEntry[Bssidx];

				NdisMoveMemory(&pBssEntry->PTSF[0], &Elem->Msg[24], 4);
				NdisMoveMemory(&pBssEntry->TTSF[0], &Elem->TimeStamp.u.LowPart, 4);
				NdisMoveMemory(&pBssEntry->TTSF[4], &Elem->TimeStamp.u.LowPart, 4);
				pBssEntry->Rssi = RealRssi;

				NdisMoveMemory(pBssEntry->MacAddr, bcn_ie_list->Addr2,
					       MAC_ADDR_LEN);


#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
/* if (RTMPEqualMemory(ie_list->Ssid, "DIRECT-", 7)) */
				DBGPRINT(RT_DEBUG_INFO,
					 ("%s PASSIVE SCANNING: %s [%lu]\n", __func__,
					  bcn_ie_list->Ssid, Bssidx));

				if (bcn_ie_list->Channel != 0)
					Elem->Channel = bcn_ie_list->Channel;

				RT_CFG80211_SCANNING_INFORM(pAd, Bssidx, Elem->Channel, Elem->Msg,
							    Elem->MsgLen, RealRssi);
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */

			}
		}

		/* Update ScanTab */
		Bssidx = BssTableSearch(&pAd->ScanTab, bcn_ie_list->Bssid, bcn_ie_list->Channel);
		if (Bssidx == BSS_NOT_FOUND) {
			/* discover new AP of this network, create BSS entry */
			Bssidx =
			    BssTableSetEntry(pAd, &pAd->ScanTab, bcn_ie_list, RealRssi, NULL,
					     LenVIE, pVIE);
			if (Bssidx == BSS_NOT_FOUND)	/* return if BSS table full */
				goto LabelOK;

			NdisMoveMemory(pAd->ScanTab.BssEntry[Bssidx].PTSF, &Elem->Msg[24], 4);
			NdisMoveMemory(&pAd->ScanTab.BssEntry[Bssidx].TTSF[0],
				       &Elem->TimeStamp.u.LowPart, 4);
			NdisMoveMemory(&pAd->ScanTab.BssEntry[Bssidx].TTSF[4],
				       &Elem->TimeStamp.u.LowPart, 4);
			pAd->ScanTab.BssEntry[Bssidx].MinSNR = Elem->Signal % 10;
			if (pAd->ScanTab.BssEntry[Bssidx].MinSNR == 0)
				pAd->ScanTab.BssEntry[Bssidx].MinSNR = -5;

			NdisMoveMemory(pAd->ScanTab.BssEntry[Bssidx].MacAddr, bcn_ie_list->Addr2,
				       MAC_ADDR_LEN);


		}

		/*
		   if the ssid matched & bssid unmatched, we should select the bssid with large value.
		   This might happened when two STA start at the same time
		 */
		if ((!is_my_bssid) && ADHOC_ON(pAd)) {
			INT i;
#ifdef IWSC_SUPPORT
			if ((pAd->StaCfg.WscControl.WscConfMode != WSC_DISABLE) &&
			    (pAd->StaCfg.WscControl.bWscTrigger == TRUE)) {
				;
			} else
#endif /* IWSC_SUPPORT */
				/* Add the safeguard against the mismatch of adhoc wep status */
			if ((wdev->WepStatus != pAd->ScanTab.BssEntry[Bssidx].WepStatus) ||
				    (wdev->AuthMode != pAd->ScanTab.BssEntry[Bssidx].AuthMode)) {
				goto LabelOK;
			}
			/* collapse into the ADHOC network which has bigger BSSID value. */
			for (i = 0; i < 6; i++) {
				if (bcn_ie_list->Bssid[i] > pAd->CommonCfg.Bssid[i]) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("SYNC - merge to the IBSS with bigger BSSID=%02x:%02x:%02x:%02x:%02x:%02x\n",
						  PRINT_MAC(bcn_ie_list->Bssid)));
					AsicDisableSync(pAd);
					COPY_MAC_ADDR(pAd->CommonCfg.Bssid, bcn_ie_list->Bssid);
					AsicSetBssid(pAd, pAd->CommonCfg.Bssid);
					MakeIbssBeacon(pAd);	/* re-build BEACON frame */
					AsicEnableIbssSync(pAd);	/* copy BEACON frame to on-chip memory */
					is_my_bssid = TRUE;
					break;
				} else if (bcn_ie_list->Bssid[i] < pAd->CommonCfg.Bssid[i])
					break;
			}
		}

		NdisGetSystemUpTime(&Now);
		pBss = &pAd->ScanTab.BssEntry[Bssidx];
		if (RealRssi < 0) {
			if (!pBss->bNot1stForEvyScan) {
				pBss->MaxRssi = RealRssi;
				pBss->bNot1stForEvyScan = TRUE;
			} else {
				if (RealRssi > pBss->MaxRssi)
					pBss->MaxRssi = RealRssi;
			}
		}
		pBss->Rssi = pBss->MaxRssi;	/* lastest RSSI */
		pBss->LastBeaconRxTime = Now;	/* last RX timestamp */
		/*
		   BEACON from my BSSID - either IBSS or INFRA network
		 */
		if (is_my_bssid) {
			RXWI_STRUC RxWI;
			UINT8 RXWISize = pAd->chipCap.RXWISize;

			OVERLAP_BSS_SCAN_IE BssScan;
			UCHAR RegClass;
			BOOLEAN brc;

			/* Read Beacon's Reg Class IE if any. */
			brc =
			    PeerBeaconAndProbeRspSanity2(pAd, Elem->Msg, Elem->MsgLen, &BssScan,
							 bcn_ie_list, &RegClass);
			if (brc == TRUE) {
				UpdateBssScanParm(pAd, BssScan);
				pAd->StaCfg.RegClass = RegClass;
			}

			pAd->StaCfg.DtimCount = bcn_ie_list->DtimCount;
			pAd->StaCfg.DtimPeriod = bcn_ie_list->DtimPeriod;
			pAd->StaCfg.LastBeaconRxTime = Now;

			NdisZeroMemory(&RxWI, RXWISize);
#ifdef RLT_MAC
			if (pAd->chipCap.hif_type == HIF_RLT) {
				{
					RxWI.RXWI_N.rssi[0] = Elem->Rssi0;
					RxWI.RXWI_N.rssi[1] = Elem->Rssi1;
					RxWI.RXWI_N.rssi[2] = Elem->Rssi2;
					RxWI.RXWI_N.phy_mode = 0;	/* Prevent SNR calculate error. */
				}
			}
#endif /* RLT_MAC */
#ifdef RTMP_MAC
			if (pAd->chipCap.hif_type == HIF_RTMP) {
				RxWI.RXWI_O.RSSI0 = Elem->Rssi0;
				RxWI.RXWI_O.RSSI1 = Elem->Rssi1;
				RxWI.RXWI_O.RSSI2 = Elem->Rssi2;
				RxWI.RXWI_O.phy_mode = 0;	/* Prevent SNR calculate error. */
			}
#endif /* RTMP_MAC */
			if (INFRA_ON(pAd)) {
				MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[BSSID_WCID];

				if (pEntry) {
					Update_Rssi_Sample(pAd, &pEntry->RssiSample, &RxWI);
#ifdef DOT11_VHT_AC
					if ((bcn_ie_list->operating_mode.rx_nss_type == 0)
						&& (bcn_ie_list->vht_op_mode_len)) {
						pEntry->force_op_mode = TRUE;
						NdisMoveMemory(&pEntry->operating_mode,
							&bcn_ie_list->operating_mode,
							sizeof(OPERATING_MODE));
					}
#endif /* DOT11_VHT_AC */

#ifdef RT_CFG80211_P2P_SINGLE_DEVICE
					if (CFG_P2PCLI_ON(pAd)) {
						CFG80211_PeerP2pBeacon(pAd, bcn_ie_list->Addr2,
								       Elem,
								       bcn_ie_list->TimeStamp);
					}
#endif /* RT_CFG80211_P2P_SINGLE_DEVICE */
				}
			}

			Update_Rssi_Sample(pAd, &pAd->StaCfg.RssiSample, &RxWI);

			if ((pAd->CommonCfg.bIEEE80211H == 1) &&
			    (bcn_ie_list->NewChannel != 0) &&
			    (bcn_ie_list->Channel != bcn_ie_list->NewChannel)) {
				/*
				   Switching to channel 1 can prevent from rescanning
				   the current channel immediately (by auto reconnection).
				   In addition, clear the MLME queue and the scan table to
				   discard the RX packets and previous scanning results.
				*/
				AsicSwitchChannel(pAd, 1, FALSE);
				AsicLockChannel(pAd, 1);
				BssTableInit(&pAd->ScanTab);
				LinkDown(pAd, FALSE);

				/* channel sanity check */
				for (index = 0; index < pAd->ChannelListNum; index++) {
					if (pAd->ChannelList[index].Channel ==
					    bcn_ie_list->NewChannel) {
						pAd->ScanTab.BssEntry[Bssidx].Channel =
						    bcn_ie_list->NewChannel;
						pAd->CommonCfg.Channel = bcn_ie_list->NewChannel;
#ifdef RT_CFG80211_SUPPORT
						/* CFG_TODO */
#else
						AsicSwitchChannel(pAd, pAd->CommonCfg.Channel,
								  FALSE);
						AsicLockChannel(pAd, pAd->CommonCfg.Channel);
#endif /* RT_CFG80211_SUPPORT */
						DBGPRINT(RT_DEBUG_TRACE,
							 ("PeerBeacon - STA receive channel switch announcement IE (New Channel =%d)\n",
							  bcn_ie_list->NewChannel));
						break;
					}
				}

				if (index >= pAd->ChannelListNum)
					DBGPRINT_ERR(
						("PeerBeacon(can not find New Channel=%d in ChannelList[%d]\n",
						pAd->CommonCfg.Channel, pAd->ChannelListNum));
			}
#ifdef WPA_SUPPLICANT_SUPPORT
			if (pAd->StaCfg.wpa_supplicant_info.WpaSupplicantUP
				& WPA_SUPPLICANT_ENABLE_WPS)
				;
			else
#endif /* WPA_SUPPLICANT_SUPPORT */
#ifdef WSC_STA_SUPPORT
			if (pAd->StaCfg.WscControl.WscState == WSC_STATE_OFF)
#endif /* WSC_STA_SUPPORT */
				{
				if ((((wdev->WepStatus !=
				       Ndis802_11WEPDisabled) << 4) ^ bcn_ie_list->CapabilityInfo) &
				    0x0010) {

#if defined(NATIVE_WPA_SUPPLICANT_SUPPORT)
					/* When using -Dwext and trigger WPS, do not check security. */
					if (bcn_ie_list->selReg == 0)
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */
						{
						/*
						   To prevent STA connect to OPEN/WEP AP when STA is OPEN/NONE or
						   STA connect to OPEN/NONE AP when STA is OPEN/WEP AP.
						 */
						DBGPRINT(RT_DEBUG_OFF,
							 ("%s:AP privacy:%x is differenct from STA privacy:%x\n",
							  __func__,
							  (bcn_ie_list->CapabilityInfo & 0x0010) >>
							  4,
							  wdev->WepStatus !=
							  Ndis802_11WEPDisabled));
						if (INFRA_ON(pAd)) {
							LinkDown(pAd, FALSE);
							BssTableInit(&pAd->ScanTab);
						}
						goto LabelOK;
					}
				}
			}
#ifdef LINUX
#ifdef RT_CFG80211_SUPPORT
/*			CFG80211_BeaconCountryRegionParse(pAd, pVIE, LenVIE); */
#endif /* RT_CFG80211_SUPPORT */
#endif /* LINUX */

			if (bcn_ie_list->AironetCellPowerLimit != 0xFF) {
				/*
				   We get the Cisco (ccx) "TxPower Limit" required
				   Changed to appropriate TxPower Limit for Ciso Compatible Extensions
				 */
				ChangeToCellPowerLimit(pAd, bcn_ie_list->AironetCellPowerLimit);
			} else {
				/*
				   AironetCellPowerLimit equal to 0xFF means the Cisco (ccx) "TxPower Limit" not exist.
				   Used the default TX Power Percentage, that set from UI.
				 */
				pAd->CommonCfg.TxPowerPercentage = pAd->CommonCfg.TxPowerDefault;
			}

			if (ADHOC_ON(pAd) && (CAP_IS_IBSS_ON(bcn_ie_list->CapabilityInfo))) {
				UCHAR MaxSupportedRateIn500Kbps = 0;
				UCHAR idx;
				MAC_TABLE_ENTRY *pEntry;
#ifdef IWSC_SUPPORT
				PWSC_CTRL pWpsCtrl = &pAd->StaCfg.WscControl;
#endif /* IWSC_SUPPORT */

				MaxSupportedRateIn500Kbps =
				    dot11_max_sup_rate(bcn_ie_list->SupRateLen,
						       &bcn_ie_list->SupRate[0],
						       bcn_ie_list->ExtRateLen,
						       &bcn_ie_list->ExtRate[0]);

				/* look up the existing table */
				pEntry = MacTableLookup(pAd, bcn_ie_list->Addr2);

				/*
				   Ad-hoc mode is using MAC address as BA session.
				   So we need to continuously find newly joined adhoc station
				   by receiving beacon.
				   To prevent always check this, we use wcid == RESERVED_WCID
				   to recognize it as newly joined adhoc station.
				 */
				if ((ADHOC_ON(pAd)
				     && ((!pEntry) || (pEntry && IS_ENTRY_NONE(pEntry))))
				     || (pEntry &&
				     RTMP_TIME_AFTER(Now,
				     pEntry->LastBeaconRxTime + ADHOC_ENTRY_BEACON_LOST_TIME))
				     ){
					/* Another adhoc joining, add to our MAC table. */
					if (pEntry == NULL)
						pEntry =
						    MacTableInsertEntry(pAd, bcn_ie_list->Addr2,
									wdev, BSS0, OPMODE_STA,
									FALSE);

					if (pEntry == NULL)
						goto LabelOK;

#ifdef IWSC_SUPPORT
					hex_dump("Another adhoc joining - Addr2",
						 bcn_ie_list->Addr2, 6);
					hex_dump("Another adhoc joining - WscPeerMAC",
						 pAd->StaCfg.WscControl.WscPeerMAC, 6);
					if ((NdisEqualMemory
					     (bcn_ie_list->Addr2, pAd->StaCfg.WscControl.WscPeerMAC,
					      MAC_ADDR_LEN))
					    && (pAd->StaCfg.IWscInfo.bSendEapolStart == FALSE)
					    && (pWpsCtrl->bWscTrigger == TRUE)) {
						pAd->StaCfg.IWscInfo.bSendEapolStart = TRUE;
					}
#endif /* IWSC_SUPPORT */

#ifdef DOT11_VHT_AC
					{
						BOOLEAN result;
						IE_LISTS *ielist;

						os_alloc_mem(NULL, (UCHAR **) &ielist,
							     sizeof(IE_LISTS));
						if (!ielist)
							goto LabelOK;
						NdisZeroMemory((UCHAR *) ielist, sizeof(IE_LISTS));

						if (bcn_ie_list->vht_cap_len
						    && bcn_ie_list->vht_op_len) {
							NdisMoveMemory(&ielist->vht_cap,
								       &bcn_ie_list->vht_cap_ie,
								       sizeof(VHT_CAP_IE));
							NdisMoveMemory(&ielist->vht_op,
								       &bcn_ie_list->vht_op_ie,
								       sizeof(VHT_OP_IE));
							ielist->vht_cap_len =
							    bcn_ie_list->vht_cap_len;
							ielist->vht_op_len =
							    bcn_ie_list->vht_op_len;
						}
						result = StaUpdateMacTableEntry(pAd,
										pEntry,
										MaxSupportedRateIn500Kbps,
										&bcn_ie_list->
										HtCapability,
										bcn_ie_list->
										HtCapabilityLen,
										&bcn_ie_list->
										AddHtInfo,
										bcn_ie_list->
										AddHtInfoLen,
										ielist,
										bcn_ie_list->
										CapabilityInfo);

						os_free_mem(NULL, ielist);
						if (result == FALSE) {
							DBGPRINT(RT_DEBUG_TRACE,
								 ("ADHOC - Add Entry failed.\n"));
							goto LabelOK;
						}
#ifdef IWSC_SUPPORT
						else
							pEntry->bUpdateInfoFromPeerBeacon = TRUE;
#endif /* IWSC_SUPPORT */
					}
#else
					if (StaUpdateMacTableEntry(pAd,
								   pEntry,
								   MaxSupportedRateIn500Kbps,
								   &bcn_ie_list->HtCapability,
								   bcn_ie_list->HtCapabilityLen,
								   &bcn_ie_list->AddHtInfo,
								   bcn_ie_list->AddHtInfoLen,
								   NULL,
								   bcn_ie_list->CapabilityInfo) ==
					    FALSE) {
						DBGPRINT(RT_DEBUG_TRACE,
							 ("ADHOC - Add Entry failed.\n"));
						goto LabelOK;
					}
#ifdef IWSC_SUPPORT
					else
						pEntry->bUpdateInfoFromPeerBeacon = TRUE;
#endif /* IWSC_SUPPORT */
#endif /* DOT11_VHT_AC */

					if (ADHOC_ON(pAd) && pEntry) {
						RTMPSetSupportMCS(pAd,
								  OPMODE_STA,
								  pEntry,
								  bcn_ie_list->SupRate,
								  bcn_ie_list->SupRateLen,
								  bcn_ie_list->ExtRate,
								  bcn_ie_list->ExtRateLen,
#ifdef DOT11_VHT_AC
								  bcn_ie_list->vht_cap_len,
								  &bcn_ie_list->vht_cap_ie,
#endif /* DOT11_VHT_AC */
								  &bcn_ie_list->HtCapability,
								  bcn_ie_list->HtCapabilityLen);
					}

					pEntry->LastBeaconRxTime = 0;

#ifdef ADHOC_WPA2PSK_SUPPORT
					/* Adhoc support WPA2PSK by Eddy */
					if ((wdev->AuthMode == Ndis802_11AuthModeWPA2PSK)
					    && (pEntry->WPA_Authenticator.WpaState < AS_INITPSK)
#ifdef IWSC_SUPPORT
					    && ((pAd->StaCfg.WscControl.WscConfMode == WSC_DISABLE)
						|| (pAd->StaCfg.WscControl.bWscTrigger == FALSE)
						||
						(NdisEqualMemory
						 (pEntry->Addr, pAd->StaCfg.WscControl.WscPeerMAC,
						  MAC_ADDR_LEN) == FALSE))
#ifdef IWSC_TEST_SUPPORT
					    && (pAd->StaCfg.IWscInfo.bBlockConnection == FALSE)
#endif /* IWSC_TEST_SUPPORT */
#endif /* IWSC_SUPPORT // */
					    ) {
						INT len, i;
						PEID_STRUCT pEid;
						NDIS_802_11_VARIABLE_IEs *pVIE2 = NULL;
						BOOLEAN bHigherMAC = FALSE;

						pVIE2 = pVIE;
						len = LenVIE;
						while (len > 0) {
							pEid = (PEID_STRUCT) pVIE;
							if ((pEid->Eid == IE_RSN)
							    &&
							    (NdisEqualMemory
							     (pEid->Octet + 2, RSN_OUI, 3))) {
								NdisMoveMemory(pEntry->RSN_IE, pVIE,
									       (pEid->Len + 2));
								pEntry->RSNIE_Len = (pEid->Len + 2);
							}
							pVIE2 += (pEid->Len + 2);
							len -= (pEid->Len + 2);
						}
						pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
						NdisZeroMemory(&pEntry->WPA_Supplicant.
							       ReplayCounter, LEN_KEY_DESC_REPLAY);
						NdisZeroMemory(&pEntry->WPA_Authenticator.
							       ReplayCounter, LEN_KEY_DESC_REPLAY);
						pEntry->WPA_Authenticator.WpaState = AS_INITPSK;
						pEntry->WPA_Supplicant.WpaState = AS_INITPSK;
						pEntry->EnqueueEapolStartTimerRunning =
						    EAPOL_START_PSK;

						for (i = 0; i < 6; i++) {
							if (bcn_ie_list->Addr2[i] >
							    pAd->CurrentAddress[i]) {
								bHigherMAC = TRUE;
								break;
							} else if (bcn_ie_list->Addr2[i] <
								   pAd->CurrentAddress[i])
								break;
						}
						hex_dump("PeerBeacon:: Addr2", bcn_ie_list->Addr2,
							 MAC_ADDR_LEN);
						hex_dump("PeerBeacon:: CurrentAddress",
							 pAd->CurrentAddress, MAC_ADDR_LEN);
						pEntry->bPeerHigherMAC = bHigherMAC;
						if (pEntry->bPeerHigherMAC == FALSE) {
							/*
							   My MAC address is higher than peer's MAC address.
							 */
							DBGPRINT(RT_DEBUG_TRACE,
								 ("ADHOC - EnqueueStartForPSKTimer.\n"));
							RTMPSetTimer
							    (&pEntry->EnqueueStartForPSKTimer,
							     ENQUEUE_EAPOL_START_TIMER);
						}
					} else {
						pEntry->PortSecured = WPA_802_1X_PORT_SECURED;
					}
#endif /* ADHOC_WPA2PSK_SUPPORT */

					if (pEntry && (Elem->Wcid == RESERVED_WCID)) {
						idx = wdev->DefaultKeyId;
						RTMP_SET_WCID_SEC_INFO(pAd, BSS0, idx,
								       pAd->SharedKey[BSS0]
								       [idx].CipherAlg,
								       pEntry->wcid,
								       SHAREDKEYTABLE);
					}
				}

				if (pEntry && IS_ENTRY_CLIENT(pEntry)) {
					pEntry->LastBeaconRxTime = Now;
#ifdef IWSC_SUPPORT
					if (pEntry->bUpdateInfoFromPeerBeacon == FALSE) {
						if (StaUpdateMacTableEntry(pAd,
									   pEntry,
									   MaxSupportedRateIn500Kbps,
									   &bcn_ie_list->
									   HtCapability,
									   bcn_ie_list->
									   HtCapabilityLen,
									   &bcn_ie_list->AddHtInfo,
									   bcn_ie_list->
									   AddHtInfoLen, NULL,
									   bcn_ie_list->
									   CapabilityInfo)
						    == FALSE) {
							DBGPRINT(RT_DEBUG_TRACE,
								 ("ADHOC 2 - Add Entry failed.\n"));
							return;
						}

						if (ADHOC_ON(pAd) && pEntry) {
							RTMPSetSupportMCS(pAd,
									  OPMODE_STA,
									  pEntry,
									  bcn_ie_list->SupRate,
									  bcn_ie_list->SupRateLen,
									  bcn_ie_list->ExtRate,
									  bcn_ie_list->ExtRateLen,
#ifdef DOT11_VHT_AC
									  bcn_ie_list->vht_cap_len,
									  &bcn_ie_list->vht_cap_ie,
#endif /* DOT11_VHT_AC */
									  &bcn_ie_list->
									  HtCapability,
									  bcn_ie_list->
									  HtCapabilityLen);
						}

						pEntry->bUpdateInfoFromPeerBeacon = TRUE;
					}
#endif /* IWSC_SUPPORT */
				}

				/* At least another peer in this IBSS, declare MediaState as CONNECTED */
				if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)) {
					OPSTATUS_SET_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
					RTMP_IndicateMediaState(pAd, NdisMediaStateConnected);
#ifdef MT76XX_BTCOEX_SUPPORT
					MLMEHook(pAd, WLAN_CONNECTION_COMPLETION,
						 QueryHashID(pAd, pAd->CommonCfg.Bssid, FALSE));
#endif /*MT76XX_BTCOEX_SUPPORT */
					pAd->ExtraInfo = GENERAL_LINK_UP;
					DBGPRINT(RT_DEBUG_TRACE,
						 ("ADHOC  fOP_STATUS_MEDIA_STATE_CONNECTED.\n"));
				}
#ifdef IWSC_SUPPORT
				if (pAd->StaCfg.IWscInfo.bSendEapolStart &&
				    (pAd->Mlme.IWscMachine.CurrState != IWSC_WAIT_PIN) &&
				    (pAd->StaCfg.WscControl.WscConfMode == WSC_ENROLLEE)) {
					pAd->StaCfg.IWscInfo.bSendEapolStart = FALSE;
					pWpsCtrl->WscState = WSC_STATE_LINK_UP;
					pWpsCtrl->WscStatus = STATUS_WSC_LINK_UP;
					NdisMoveMemory(pWpsCtrl->EntryAddr, pWpsCtrl->WscPeerMAC,
						       MAC_ADDR_LEN);
					WscSendEapolStart(pAd, pWpsCtrl->WscPeerMAC, STA_MODE);
				}
#endif /* IWSC_SUPPORT */
			}

			if (INFRA_ON(pAd)) {
				BOOLEAN bUseShortSlot, bUseBGProtection;

				/*
				   decide to use/change to -
				   1. long slot (20 us) or short slot (9 us) time
				   2. turn on/off RTS/CTS and/or CTS-to-self protection
				   3. short preamble
				 */


				/*
				   bUseShortSlot = pAd->CommonCfg.bUseShortSlotTime
					&& CAP_IS_SHORT_SLOT(CapabilityInfo);
				*/
				bUseShortSlot = CAP_IS_SHORT_SLOT(bcn_ie_list->CapabilityInfo);
				if (bUseShortSlot !=
				    OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_SHORT_SLOT_INUSED))
					AsicSetSlotTime(pAd, bUseShortSlot);

				bUseBGProtection = (pAd->CommonCfg.UseBGProtection == 1) ||	/* always use */
				    ((pAd->CommonCfg.UseBGProtection == 0)
				     && ERP_IS_USE_PROTECTION(bcn_ie_list->Erp));

				/*
				   always no BG protection in A-band.
				   falsely happened when switching A/G band to a dual-band AP
				*/
				if (pAd->CommonCfg.Channel > 14)
					bUseBGProtection = FALSE;

				if (bUseBGProtection !=
				    OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_BG_PROTECTION_INUSED)) {
					if (bUseBGProtection) {
						OPSTATUS_SET_FLAG(pAd,
								  fOP_STATUS_BG_PROTECTION_INUSED);
						AsicUpdateProtect(pAd,
								  pAd->MlmeAux.AddHtInfo.AddHtInfo2.
								  OperaionMode,
								  (OFDMSETPROTECT | CCKSETPROTECT |
								   ALLN_SETPROTECT), FALSE,
								  (pAd->MlmeAux.AddHtInfo.
								   AddHtInfo2.NonGfPresent == 1));
					} else {
						OPSTATUS_CLEAR_FLAG(pAd,
								    fOP_STATUS_BG_PROTECTION_INUSED);
						AsicUpdateProtect(pAd,
								  pAd->MlmeAux.AddHtInfo.AddHtInfo2.
								  OperaionMode,
								  (OFDMSETPROTECT | CCKSETPROTECT |
								   ALLN_SETPROTECT), TRUE,
								  (pAd->MlmeAux.AddHtInfo.
								   AddHtInfo2.NonGfPresent == 1));
					}

					DBGPRINT(RT_DEBUG_TRACE,
						 ("SYNC - AP changed B/G protection to %d\n",
						  bUseBGProtection));
				}

				/* check Ht protection mode. and adhere to the Non-GF device indication by AP. */
				if ((bcn_ie_list->AddHtInfoLen != 0) &&
				    ((bcn_ie_list->AddHtInfo.AddHtInfo2.OperaionMode !=
				      pAd->MlmeAux.AddHtInfo.AddHtInfo2.OperaionMode)
				     || (bcn_ie_list->AddHtInfo.AddHtInfo2.NonGfPresent !=
					 pAd->MlmeAux.AddHtInfo.AddHtInfo2.NonGfPresent))) {
					pAd->MlmeAux.AddHtInfo.AddHtInfo2.NonGfPresent =
					    bcn_ie_list->AddHtInfo.AddHtInfo2.NonGfPresent;
					pAd->MlmeAux.AddHtInfo.AddHtInfo2.OperaionMode =
					    bcn_ie_list->AddHtInfo.AddHtInfo2.OperaionMode;
					if (pAd->MlmeAux.AddHtInfo.AddHtInfo2.NonGfPresent == 1) {
						AsicUpdateProtect(pAd,
								  pAd->MlmeAux.AddHtInfo.AddHtInfo2.
								  OperaionMode, ALLN_SETPROTECT,
								  FALSE, TRUE);
					} else
						AsicUpdateProtect(pAd,
								  pAd->MlmeAux.AddHtInfo.AddHtInfo2.
								  OperaionMode, ALLN_SETPROTECT,
								  FALSE, FALSE);

					DBGPRINT(RT_DEBUG_TRACE,
						 ("SYNC - AP changed N OperaionMode to %d\n",
						  pAd->MlmeAux.AddHtInfo.AddHtInfo2.OperaionMode));
				}

				if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_SHORT_PREAMBLE_INUSED) &&
				    ERP_IS_USE_BARKER_PREAMBLE(bcn_ie_list->Erp)) {
					MlmeSetTxPreamble(pAd, Rt802_11PreambleLong);
					DBGPRINT(RT_DEBUG_TRACE,
						 ("SYNC - AP forced to use LONG preamble\n"));
				}

				if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_WMM_INUSED) &&
				    (bcn_ie_list->EdcaParm.bValid == TRUE) &&
				    (bcn_ie_list->EdcaParm.EdcaUpdateCount !=
				     pAd->CommonCfg.APEdcaParm.EdcaUpdateCount)) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("SYNC - AP change EDCA parameters(from %d to %d)\n",
						  pAd->CommonCfg.APEdcaParm.EdcaUpdateCount,
						  bcn_ie_list->EdcaParm.EdcaUpdateCount));
					AsicSetEdcaParm(pAd, &bcn_ie_list->EdcaParm);
				}

				/* copy QOS related information */
				NdisMoveMemory(&pAd->CommonCfg.APQbssLoad, &bcn_ie_list->QbssLoad,
					       sizeof(QBSS_LOAD_PARM));
				NdisMoveMemory(&pAd->CommonCfg.APQosCapability,
					       &bcn_ie_list->QosCapability,
					       sizeof(QOS_CAPABILITY_PARM));
				/*
				   2009: PF#1: 20/40 Coexistence in 2.4 GHz Band
				   When AP changes "STA Channel Width" and "Secondary
				   Channel Offset" fields of HT Operation Element
				   in the Beacon to 0
				 */
				if ((bcn_ie_list->AddHtInfoLen != 0) && INFRA_ON(pAd)
				    && pAd->CommonCfg.Channel <= 14) {
					BOOLEAN bChangeBW = FALSE;

					/*
					   1) HT Information
					   2) Secondary Channel Offset Element

					   40 -> 20 case
					 */
					if (pAd->CommonCfg.BBPCurrentBW == BW_40) {
						if (((bcn_ie_list->AddHtInfo.AddHtInfo.
						      ExtChanOffset == EXTCHA_NONE)
						     && (bcn_ie_list->AddHtInfo.AddHtInfo.
							 RecomWidth == 0))
						    || (bcn_ie_list->NewExtChannelOffset == 0x0)
						    ) {
							pAd->StaActive.SupportedHtPhy.ChannelWidth =
							    BW_20;
							pAd->MacTab.Content[BSSID_WCID].HTPhyMode.
							    field.BW = 0;

							{
								bChangeBW = TRUE;
								pAd->CommonCfg.CentralChannel =
								    pAd->CommonCfg.Channel;
								DBGPRINT(RT_DEBUG_OFF,
									 ("FallBack from 40MHz to 20MHz(CtrlCh=%d, CentralCh=%d)\n",
									  pAd->CommonCfg.Channel,
									  pAd->CommonCfg.
									  CentralChannel));
								CntlChannelWidth(pAd,
										 pAd->CommonCfg.
										 Channel,
										 pAd->CommonCfg.
										 CentralChannel,
										 BW_20, 0);
								Enable_Tx2Q(pAd);
							}
						}
					}

					/*
					   20 -> 40 case
					   1.) Supported Channel Width Set Field of the HT
					       Capabilities element of both STAs is set to
					       a non-zero
					   2.) Secondary Channel Offset field is SCA or SCB
					   3.) 40MHzRegulatoryClass is TRUE (not implement it)
					 */
					else if (((pAd->CommonCfg.BBPCurrentBW == BW_20)
						  || (bcn_ie_list->NewExtChannelOffset != 0x0))
						 && (pAd->CommonCfg.DesiredHtPhy.ChannelWidth !=
						     BW_20)
					    ) {
						if ((bcn_ie_list->AddHtInfo.AddHtInfo.
						     ExtChanOffset != EXTCHA_NONE)
						    && (bcn_ie_list->AddHtInfo.AddHtInfo.
							RecomWidth == 1)
						    && (bcn_ie_list->HtCapabilityLen > 0)
						    && (bcn_ie_list->HtCapability.HtCapInfo.
							ChannelWidth == 1)
						    ) {
							{
								pAd->CommonCfg.CentralChannel =
								    get_cent_ch_by_htinfo(pAd,
											  &bcn_ie_list->
											  AddHtInfo,
											  &bcn_ie_list->
											  HtCapability);
								if (pAd->CommonCfg.CentralChannel !=
								    bcn_ie_list->AddHtInfo.
								    ControlChan)
									bChangeBW = TRUE;

								if (bChangeBW) {
									pAd->CommonCfg.Channel =
									    bcn_ie_list->AddHtInfo.
									    ControlChan;
									pAd->StaActive.
									    SupportedHtPhy.
									    ChannelWidth = BW_40;
									DBGPRINT(RT_DEBUG_OFF,
										 ("FallBack from 20MHz to 40MHz(CtrlCh=%d, CentralCh=%d)\n",
										  pAd->CommonCfg.
										  Channel,
										  pAd->CommonCfg.
										  CentralChannel));
									/* Fix if AP is HT40 and we are HT20 only, we shall stay at HT20 mode */
									if (!(pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20
										/*some AP like Dlink DIR-855/655 Secondary Channel Offset != 0 && STA Channel Width = 0 case */
										|| pAd->MlmeAux.HtCapability.HtCapInfo.ChannelWidth == BW_20))
										CntlChannelWidth(pAd,
											 pAd->
											 CommonCfg.
											 Channel,
											 pAd->
											 CommonCfg.
											 CentralChannel,
											 BW_40,
											 bcn_ie_list->
											 AddHtInfo.
											 AddHtInfo.
											 ExtChanOffset);
									pAd->MacTab.
									    Content[BSSID_WCID].
									    HTPhyMode.field.BW = 1;
									Enable_Tx2Q(pAd);

								}
							}
						}
					}

					if (bChangeBW) {
						pAd->CommonCfg.BSSCoexist2040.word = 0;
						TriEventInit(pAd);
						BuildEffectedChannelList(pAd);
					}
				}
			}

			/* only INFRASTRUCTURE mode support power-saving feature */
			if ((INFRA_ON(pAd) && (RtmpPktPmBitCheck(pAd) == TRUE))
			    || (pAd->CommonCfg.bAPSDForcePowerSave)
#ifdef RT_CFG80211_P2P_SUPPORT
			    || (INFRA_ON(pAd) && pAd->cfg80211_ctrl.bP2pCliPmEnable)
#endif /* RT_CFG80211_SUPPORT */
			    ) {
				UCHAR FreeNumber;
				/*
				   1. AP has backlogged unicast-to-me frame, stay AWAKE, send PSPOLL
				   2. AP has backlogged broadcast/multicast frame and we want those frames, stay AWAKE
				   3. we have outgoing frames in TxRing or MgmtRing, better stay AWAKE
				   4. Psm change to PWR_SAVE, but AP not been informed yet, we better stay AWAKE
				   5. otherwise, put PHY back to sleep to save battery.
				 */
				if (bcn_ie_list->MessageToMe) {
#ifdef PCIE_PS_SUPPORT
					if (OPSTATUS_TEST_FLAG
					    (pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) {
						/* Restore to correct BBP R3 value */
						if (pAd->Antenna.field.RxPath > 1)
							RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3,
										     pAd->StaCfg.
										     BBPR3);
						/* Turn clk to 80Mhz. */
					}
#endif /* PCIE_PS_SUPPORT */
#ifdef UAPSD_SUPPORT
					if (pAd->StaCfg.UapsdInfo.bAPSDCapable &&
					    pAd->CommonCfg.APEdcaParm.bAPSDCapable &&
					    pAd->CommonCfg.bAPSDAC_BE &&
					    pAd->CommonCfg.bAPSDAC_BK &&
					    pAd->CommonCfg.bAPSDAC_VI &&
					    pAd->CommonCfg.bAPSDAC_VO) {
						pAd->CommonCfg.bNeedSendTriggerFrame = TRUE;
#if defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT)
						RTMPSendNullFrame(pAd,
								  pAd->CommonCfg.TxRate,
								  TRUE,
								  pAd->
								  CommonCfg.bAPSDForcePowerSave ?
								  PWR_SAVE : pAd->StaCfg.Psm);
#endif /* defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT) */
					} else
#endif /* UAPSD_SUPPORT */
					{
						if (pAd->StaCfg.WindowsBatteryPowerMode ==
						    Ndis802_11PowerModeFast_PSP) {
							/* wake up and send a NULL frame with PM = 0 to the AP */
							RTMP_SET_PSM_BIT(pAd, PWR_ACTIVE);
							RTMPSendNullFrame(pAd,
									  pAd->CommonCfg.TxRate,
									  (OPSTATUS_TEST_FLAG
									   (pAd,
									    fOP_STATUS_WMM_INUSED) ?
									   TRUE : FALSE),
									  PWR_ACTIVE);
						} else {
							/* use PS-Poll to get any buffered packet */
							RTMP_PS_POLL_ENQUEUE(pAd);
						}
					}
				} else if (bcn_ie_list->BcastFlag && (bcn_ie_list->DtimCount == 0)
					   && OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_RECEIVE_DTIM)) {
#ifdef PCIE_PS_SUPPORT
					if (OPSTATUS_TEST_FLAG
					    (pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) {
						if (pAd->Antenna.field.RxPath > 1)
							RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3,
										     pAd->StaCfg.
										     BBPR3);
					}
#endif /* PCIE_PS_SUPPORT */
				} else if ((pAd->TxSwQueue[QID_AC_BK].Number != 0) ||
					   (pAd->TxSwQueue[QID_AC_BE].Number != 0) ||
					   (pAd->TxSwQueue[QID_AC_VI].Number != 0) ||
					   (pAd->TxSwQueue[QID_AC_VO].Number != 0) ||
					   (RTMPFreeTXDRequest
					    (pAd, QID_AC_BK, TX_RING_SIZE - 1,
					     &FreeNumber) != NDIS_STATUS_SUCCESS)
					   ||
					   (RTMPFreeTXDRequest
					    (pAd, QID_AC_BE, TX_RING_SIZE - 1,
					     &FreeNumber) != NDIS_STATUS_SUCCESS)
					   ||
					   (RTMPFreeTXDRequest
					    (pAd, QID_AC_VI, TX_RING_SIZE - 1,
					     &FreeNumber) != NDIS_STATUS_SUCCESS)
					   ||
					   (RTMPFreeTXDRequest
					    (pAd, QID_AC_VO, TX_RING_SIZE - 1,
					     &FreeNumber) != NDIS_STATUS_SUCCESS)
					   ||
					   (RTMPFreeTXDRequest
					    (pAd, QID_MGMT, MGMT_RING_SIZE - 1,
					     &FreeNumber) != NDIS_STATUS_SUCCESS)) {
					/*
					   TODO: consider scheduled HCCA. might not be proper to
					   use traditional DTIM-based power-saving scheme
					*/
					/* can we cheat here (i.e. just check MGMT & AC_BE) for better performance? */
#ifdef PCIE_PS_SUPPORT
					if (OPSTATUS_TEST_FLAG
						(pAd, fOP_STATUS_ADVANCE_POWER_SAVE_PCIE_DEVICE)) {
						if (pAd->Antenna.field.RxPath > 1)
							RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R3,
								pAd->StaCfg.BBPR3);
					}
#endif /* PCIE_PS_SUPPORT */
				}
			}
#ifdef CFG_TDLS_SUPPORT
			pCFG_TDLS = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info;
#ifdef CERTIFICATION_SIGMA_SUPPORT
			/*ULONG TimeTillTbtt = 0;

			   RTMP_IO_READ32(pAd, TBTT_TIMER, &TimeTillTbtt);
			   TimeTillTbtt = TimeTillTbtt&0x1ffff;
			   DBGPRINT(RT_DEBUG_TRACE, ("Got Beacon===> TBTT_TIMER(%ld)\n", TimeTillTbtt<<6)); */

			/* This is a workaround to adjust TBTT timer */
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
			if (pCFG_TDLS->BeaconSynced == FALSE && !CFG_P2PGO_ON(pAd)
				&& !CFG_AP_ON(pAd))
#else
			if (pCFG_TDLS->BeaconSynced == FALSE)
#endif
				{
				pCFG_TDLS->BeaconCount++;
				if (INFRA_ON(pAd) && pCFG_TDLS->BeaconCount >= 25) {
					RTMPSetTimer(&pCFG_TDLS->BeaconSyncTimer,
						pAd->CommonCfg.BeaconPeriod - 8);
					/* This value may adjust by different platform */
					pCFG_TDLS->BeaconSynced = TRUE;
				}
			}
#endif /* CERTIFICATION_SIGMA_SUPPORT */
			if (INFRA_ON(pAd)) {
				if (pCFG_TDLS->IamInOffChannel == TRUE) {
					DBGPRINT(RT_DEBUG_OFF,
						 ("Recieve Orig AP Beacon but FW didn't inform Back to Base, force back to base !!!\n"));
					pCFG_TDLS->IamInOffChannel = FALSE;
				}

				if (bcn_ie_list->MessageToMe ||
				    (bcn_ie_list->BcastFlag && (bcn_ie_list->DtimCount == 0))) {
					/* condition to stay in base channel */
					DBGPRINT(RT_DEBUG_INFO, ("Got Beacon===> MessageToMe!!\n"));
				} else if (pCFG_TDLS->bDoingPeriodChannelSwitch
					 && pCFG_TDLS->bChannelSwitchInitiator) {
					RTMPSetTimer(&pCFG_TDLS->BaseChannelSwitchTimer,
						     pCFG_TDLS->BaseChannelStayTime);
				}
			}
#endif /* CFG_TDLS_SUPPORT */
		}
		/* not my BSSID, ignore it */
	}

	/* sanity check fail, ignore this frame */
	goto LabelOK;

LabelErr:
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Allocate memory fail!!!\n", __func__));

LabelOK:
	if (VarIE != NULL)
		os_free_mem(NULL, VarIE);
	if (bcn_ie_list != NULL)
		os_free_mem(NULL, bcn_ie_list);

}

/*
	==========================================================================
	Description:
		Receive PROBE REQ from remote peer when operating in IBSS mode
	==========================================================================
 */
VOID PeerProbeReqAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{
	PEER_PROBE_REQ_PARAM ProbeReqParam;
	UCHAR HtLen, AddHtLen, NewExtLen;
	HEADER_802_11 ProbeRspHdr;
	NDIS_STATUS NStatus;
	PUCHAR pOutBuffer = NULL;
	ULONG FrameLen = 0;
	LARGE_INTEGER FakeTimestamp;
	UCHAR DsLen = 1, IbssLen = 2;
	UCHAR LocalErpIe[3] = { IE_ERP, 1, 0 };
	BOOLEAN Privacy;
	USHORT CapabilityInfo;


	if (!ADHOC_ON(pAd))
		return;

	if (PeerProbeReqSanity(pAd, Elem->Msg, Elem->MsgLen, &ProbeReqParam)) {
		struct wifi_dev *wdev = &pAd->StaCfg.wdev;

		if ((ProbeReqParam.SsidLen == 0) ||
		    SSID_EQUAL(ProbeReqParam.Ssid, ProbeReqParam.SsidLen, pAd->CommonCfg.Ssid,
			       pAd->CommonCfg.SsidLen)) {
			/* allocate and send out ProbeRsp frame */
			NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);	/* Get an unused nonpaged memory */
			if (NStatus != NDIS_STATUS_SUCCESS)
				return;

			MgtMacHeaderInit(pAd, &ProbeRspHdr, SUBTYPE_PROBE_RSP,
					 0, ProbeReqParam.Addr2,
					 pAd->CurrentAddress, pAd->CommonCfg.Bssid);

			Privacy = (wdev->WepStatus == Ndis802_11WEPEnabled) ||
			    (wdev->WepStatus == Ndis802_11TKIPEnable) ||
			    (wdev->WepStatus == Ndis802_11AESEnable);
			CapabilityInfo =
			    CAP_GENERATE(0, 1, Privacy,
					 (pAd->CommonCfg.TxPreamble == Rt802_11PreambleShort), 0,
					 0);

			MakeOutgoingFrame(pOutBuffer, &FrameLen,
					  sizeof(HEADER_802_11), &ProbeRspHdr,
					  TIMESTAMP_LEN, &FakeTimestamp,
					  2, &pAd->CommonCfg.BeaconPeriod,
					  2, &CapabilityInfo,
					  1, &SsidIe,
					  1, &pAd->CommonCfg.SsidLen,
					  pAd->CommonCfg.SsidLen, pAd->CommonCfg.Ssid,
					  1, &SupRateIe,
					  1, &pAd->StaActive.SupRateLen,
					  pAd->StaActive.SupRateLen, pAd->StaActive.SupRate,
					  1, &DsIe,
					  1, &DsLen,
					  1, &pAd->CommonCfg.Channel,
					  1, &IbssIe,
					  1, &IbssLen, 2, &pAd->StaActive.AtimWin, END_OF_ARGS);

			if (pAd->StaActive.ExtRateLen) {
				ULONG tmp;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &tmp,
						  3, LocalErpIe,
						  1, &ExtRateIe,
						  1, &pAd->StaActive.ExtRateLen,
						  pAd->StaActive.ExtRateLen,
						  &pAd->StaActive.ExtRate, END_OF_ARGS);
				FrameLen += tmp;
			}

			/* Modify by Eddy, support WPA2PSK in Adhoc mode */
			if ((wdev->AuthMode == Ndis802_11AuthModeWPANone)
#ifdef ADHOC_WPA2PSK_SUPPORT
			    || (wdev->AuthMode == Ndis802_11AuthModeWPA2PSK)
#endif /* ADHOC_WPA2PSK_SUPPORT */
			    ) {
				ULONG tmp;
				UCHAR RSNIe = IE_WPA;

#ifdef ADHOC_WPA2PSK_SUPPORT
				RTMPMakeRSNIE(pAd, wdev->AuthMode, wdev->WepStatus, BSS0);
				if (wdev->AuthMode == Ndis802_11AuthModeWPA2PSK)
					RSNIe = IE_RSN;
#endif /* ADHOC_WPA2PSK_SUPPORT */

				MakeOutgoingFrame(pOutBuffer + FrameLen, &tmp,
						  1, &RSNIe,
						  1, &pAd->StaCfg.RSNIE_Len,
						  pAd->StaCfg.RSNIE_Len, pAd->StaCfg.RSN_IE,
						  END_OF_ARGS);
				FrameLen += tmp;
			}

			if (WMODE_CAP_N(pAd->CommonCfg.PhyMode)) {
				ULONG TmpLen;
				USHORT epigram_ie_len;
				UCHAR BROADCOM[4] = { 0x0, 0x90, 0x4c, 0x33 };

				HtLen = sizeof(pAd->CommonCfg.HtCapability);
				AddHtLen = sizeof(pAd->CommonCfg.AddHTInfo);
				NewExtLen = 1;
				/*
				   New extension channel offset IE is included in Beacon,
				   Probe Rsp or channel Switch Announcement Frame
				*/
				if (pAd->bBroadComHT == TRUE) {
					epigram_ie_len = pAd->MlmeAux.HtCapabilityLen + 4;
					MakeOutgoingFrame(pOutBuffer + FrameLen, &TmpLen,
							  1, &WpaIe,
							  1, &epigram_ie_len,
							  4, &BROADCOM[0],
							  pAd->MlmeAux.HtCapabilityLen,
							  &pAd->MlmeAux.HtCapability, END_OF_ARGS);
				} else {
					MakeOutgoingFrame(pOutBuffer + FrameLen, &TmpLen,
							  1, &HtCapIe,
							  1, &HtLen,
							  sizeof(HT_CAPABILITY_IE),
							  &pAd->CommonCfg.HtCapability, 1,
							  &AddHtInfoIe, 1, &AddHtLen,
							  sizeof(ADD_HT_INFO_IE),
							  &pAd->CommonCfg.AddHTInfo, END_OF_ARGS);
				}
				FrameLen += TmpLen;
			}
#ifdef WSC_STA_SUPPORT
			/* add Simple Config Information Element */
			if (pAd->StaCfg.WpsIEProbeResp.ValueLen != 0) {
				ULONG WscTmpLen = 0;

				MakeOutgoingFrame(pOutBuffer + FrameLen, &WscTmpLen,
						  pAd->StaCfg.WpsIEProbeResp.ValueLen,
						  pAd->StaCfg.WpsIEProbeResp.Value, END_OF_ARGS);
				FrameLen += WscTmpLen;
			}
#endif /* WSC_STA_SUPPORT */

			MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
			MlmeFreeMemory(pAd, pOutBuffer);
		}
	}
}

VOID BeaconTimeoutAtJoinAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{
	USHORT Status;

	DBGPRINT(RT_DEBUG_TRACE, ("SYNC - BeaconTimeoutAtJoinAction\n"));
	pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
	Status = MLME_REJ_TIMEOUT;
	MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_JOIN_CONF, 2, &Status, 0);
}

/*
	==========================================================================
	Description:
		Scan timeout procedure. basically add channel index by 1 and rescan
	==========================================================================
 */
VOID ScanTimeoutAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{

#ifdef RTMP_MAC_USB
	/*
	   To prevent data lost.
	   Send an NULL data with turned PSM bit on to current associated AP when SCAN in the channel where
	   associated AP located.
	 */
	if ((pAd->CommonCfg.Channel == pAd->MlmeAux.Channel) &&
	    (pAd->MlmeAux.ScanType == SCAN_ACTIVE) &&
	    (INFRA_ON(pAd)) && OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)
	    ) {
		RTMPSendNullFrame(pAd,
				  pAd->CommonCfg.TxRate,
				  (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_WMM_INUSED) ? TRUE : FALSE),
				  PWR_SAVE);
		/* yiwei add */
		OS_WAIT(20);
	}
#endif /* RTMP_MAC_USB */

	if (pAd->StaCfg.bFastConnect && !pAd->StaCfg.bNotFirstScan) {

		pAd->MlmeAux.Channel = 0;
		pAd->StaCfg.bNotFirstScan = TRUE;
	} else {
			pAd->MlmeAux.Channel = NextChannel(pAd, pAd->MlmeAux.Channel);
	}

	/* Only one channel scanned for CISCO beacon request */
	if ((pAd->MlmeAux.ScanType == SCAN_CISCO_ACTIVE) ||
	    (pAd->MlmeAux.ScanType == SCAN_CISCO_PASSIVE) ||
	    (pAd->MlmeAux.ScanType == SCAN_CISCO_NOISE) ||
	    (pAd->MlmeAux.ScanType == SCAN_CISCO_CHANNEL_LOAD))
		pAd->MlmeAux.Channel = 0;

	/* this routine will stop if pAd->MlmeAux.Channel == 0 */
	ScanNextChannel(pAd, OPMODE_STA);
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID InvalidStateWhenScan(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{
	USHORT Status;

	if (Elem->MsgType != MT2_MLME_SCAN_REQ)
		DBGPRINT(RT_DEBUG_TRACE,
			 ("AYNC - InvalidStateWhenScan(state=%ld). Reset SYNC machine\n",
			  pAd->Mlme.SyncMachine.CurrState));
	else
		DBGPRINT(RT_DEBUG_TRACE,
			 ("AYNC - Already in scanning, do nothing here.(state=%ld).\n",
			  pAd->Mlme.SyncMachine.CurrState));

	if (Elem->MsgType != MT2_MLME_SCAN_REQ) {
		pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
		Status = MLME_STATE_MACHINE_REJECT;
		MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_SCAN_CONF, 2, &Status, 0);
	}
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID InvalidStateWhenJoin(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{
	USHORT Status;

	DBGPRINT(RT_DEBUG_TRACE, ("InvalidStateWhenJoin(state=%ld, msg=%ld). Reset SYNC machine\n",
				  pAd->Mlme.SyncMachine.CurrState, Elem->MsgType));
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS))
		RTMPResumeMsduTransmission(pAd);

	pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
	Status = MLME_STATE_MACHINE_REJECT;
	MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_JOIN_CONF, 2, &Status, 0);
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID InvalidStateWhenStart(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM *Elem)
{
	USHORT Status;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("InvalidStateWhenStart(state=%ld). Reset SYNC machine\n",
		  pAd->Mlme.SyncMachine.CurrState));
	pAd->Mlme.SyncMachine.CurrState = SYNC_IDLE;
	Status = MLME_STATE_MACHINE_REJECT;
	MlmeEnqueue(pAd, MLME_CNTL_STATE_MACHINE, MT2_START_CONF, 2, &Status, 0);
}

/*
	==========================================================================
	Description:

	IRQL = DISPATCH_LEVEL

	==========================================================================
 */
VOID EnqueuePsPoll(RTMP_ADAPTER *pAd)
{
#ifdef RALINK_ATE
	if (ATE_ON(pAd))
		return;
#endif /* RALINK_ATE */


	if (pAd->StaCfg.WindowsPowerMode == Ndis802_11PowerModeLegacy_PSP)
		pAd->PsPollFrame.FC.PwrMgmt = PWR_SAVE;
	MiniportMMRequest(pAd, 0, (PUCHAR) &pAd->PsPollFrame, sizeof(PSPOLL_FRAME));
#ifdef RTMP_MAC_USB
	/* Keep Waking up */
	if (pAd->CountDowntoPsm == 0)
		pAd->CountDowntoPsm = 2;	/* 100 ms; stay awake 200ms at most, average will be 1xx ms */
#endif /* RTMP_MAC_USB */
}

/*
	==========================================================================
	Description:
	==========================================================================
 */
VOID EnqueueProbeRequest(RTMP_ADAPTER *pAd)
{
	NDIS_STATUS NState;
	PUCHAR pOutBuffer;
	ULONG FrameLen = 0;
	HEADER_802_11 Hdr80211;

	DBGPRINT(RT_DEBUG_TRACE, ("force out a ProbeRequest ...\n"));

	NState = MlmeAllocateMemory(pAd, &pOutBuffer);	/* Get an unused nonpaged memory */
	if (NState == NDIS_STATUS_SUCCESS) {
		MgtMacHeaderInit(pAd, &Hdr80211, SUBTYPE_PROBE_REQ, 0, BROADCAST_ADDR,
				 pAd->CurrentAddress, BROADCAST_ADDR);

		/* this ProbeRequest explicitly specify SSID to reduce unwanted ProbeResponse */
		MakeOutgoingFrame(pOutBuffer, &FrameLen,
				  sizeof(HEADER_802_11), &Hdr80211,
				  1, &SsidIe,
				  1, &pAd->CommonCfg.SsidLen,
				  pAd->CommonCfg.SsidLen, pAd->CommonCfg.Ssid,
				  1, &SupRateIe,
				  1, &pAd->StaActive.SupRateLen,
				  pAd->StaActive.SupRateLen, pAd->StaActive.SupRate, END_OF_ARGS);
		MiniportMMRequest(pAd, 0, pOutBuffer, FrameLen);
		MlmeFreeMemory(pAd, pOutBuffer);
	}

}


/*
	========================================================================

	Routine Description:
		Control Primary&Central Channel, ChannelWidth and Second Channel Offset

	Arguments:
		pAd						Pointer to our adapter
		PrimaryChannel			Primary Channel
		CentralChannel			Central Channel
		ChannelWidth				BW_20 or BW_40
		SecondaryChannelOffset	EXTCHA_NONE, EXTCHA_ABOVE and EXTCHA_BELOW

	Return Value:
		None

	Note:

	========================================================================
*/
VOID CntlChannelWidth(IN PRTMP_ADAPTER pAd,
		      IN UCHAR prim_ch, IN UCHAR cent_ch, IN UCHAR ch_bw, IN UCHAR sec_ch_offset)
{
	UCHAR rf_channel = 0;
	UINT8 rf_bw, ext_ch;

	DBGPRINT(RT_DEBUG_TRACE, ("%s: PrimaryChannel[%d]\n", __func__, prim_ch));
	DBGPRINT(RT_DEBUG_TRACE, ("%s: CentralChannel[%d]\n", __func__, cent_ch));
	DBGPRINT(RT_DEBUG_TRACE, ("%s: ChannelWidth[%d]\n", __func__, ch_bw));
	DBGPRINT(RT_DEBUG_TRACE, ("%s: SecondaryChannelOffset[%d]\n", __func__, sec_ch_offset));

	/*Change to AP channel */
	if (ch_bw == BW_40) {
		if (sec_ch_offset == EXTCHA_ABOVE) {
			rf_bw = BW_40;
			ext_ch = EXTCHA_ABOVE;
			rf_channel = cent_ch;
		} else if (sec_ch_offset == EXTCHA_BELOW) {
			rf_bw = BW_40;
			ext_ch = EXTCHA_BELOW;
			rf_channel = cent_ch;
		}
	} else {
		rf_bw = BW_20;
		ext_ch = EXTCHA_NONE;
		rf_channel = prim_ch;
	}

	if (rf_channel != 0) {
		bbp_set_bw(pAd, rf_bw);

		/* Tx/ RX : control channel setting */
		bbp_set_ctrlch(pAd, ext_ch);
		rtmp_mac_set_ctrlch(pAd, ext_ch);

		AsicSwitchChannel(pAd, rf_channel, FALSE);
		AsicLockChannel(pAd, rf_channel);

		DBGPRINT(RT_DEBUG_TRACE,
			 ("!!!40MHz Lower !!! Control Channel at Below. Central = %d\n",
			  pAd->CommonCfg.CentralChannel));

		bbp_get_agc(pAd, &pAd->BbpTuning.R66CurrentValue, RX_CHAIN_0);
	}
}

/*
    ==========================================================================
    Description:
	MLME Cancel the SCAN req state machine procedure
    ==========================================================================
 */
/*
VOID ScanCnclAction(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem)
{
	BOOLEAN Cancelled;

	RTMPCancelTimer(&pAd->MlmeAux.ScanTimer, &Cancelled);
	pAd->MlmeAux.Channel = 0;
	ScanNextChannel(pAd, OPMODE_STA);

	return;
}
*/
