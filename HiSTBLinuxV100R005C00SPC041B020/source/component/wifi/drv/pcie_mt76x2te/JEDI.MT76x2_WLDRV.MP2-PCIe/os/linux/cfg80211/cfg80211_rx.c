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


#define RTMP_MODULE_OS

#ifdef RT_CFG80211_SUPPORT

#include "rt_config.h"

extern UCHAR CFG_P2POUIBYTE[];

VOID CFG80211_Convert802_3Packet(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR *pHeader802_3)
{
#ifdef CONFIG_AP_SUPPORT
	if (IS_PKT_OPMODE_AP(pRxBlk)) {
		RTMP_AP_802_11_REMOVE_LLC_AND_CONVERT_TO_802_3(pRxBlk, pHeader802_3);
	} else
#endif /* CONFIG_AP_SUPPORT */
	{
#ifdef CONFIG_STA_SUPPORT
		RTMP_802_11_REMOVE_LLC_AND_CONVERT_TO_802_3(pRxBlk, pHeader802_3);
#endif /*CONFIG_STA_SUPPORT */
	}
}

VOID CFG80211_Announce802_3Packet(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR FromWhichBSSID)
{
#ifdef CONFIG_AP_SUPPORT
	if (IS_PKT_OPMODE_AP(pRxBlk)) {
		AP_ANNOUNCE_OR_FORWARD_802_3_PACKET(pAd, pRxBlk->pRxPacket, FromWhichBSSID);
	} else
#endif /* CONFIG_AP_SUPPORT */
	{
#ifdef CONFIG_STA_SUPPORT
		ANNOUNCE_OR_FORWARD_802_3_PACKET(pAd, pRxBlk->pRxPacket, FromWhichBSSID);
#endif /*CONFIG_STA_SUPPORT */
	}

}

ACTION_FRAME_TYPE CFG80211_CheckActionFrameType(IN RTMP_ADAPTER *pAd,
						IN PUCHAR preStr, IN PUCHAR pData, IN UINT32 length)
{
	ACTION_FRAME_TYPE actionFrame = NONE_ACTION_FRAME_TYPE;
	struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)pData;

	if (ieee80211_is_mgmt(mgmt->frame_control)) {
		if (ieee80211_is_probe_resp(mgmt->frame_control)) {
			DBGPRINT(RT_DEBUG_INFO,
				 ("CFG80211_PKT: %s ProbeRsp Frame %d\n", preStr,
				  pAd->LatchRfRegs.Channel));
			if (!mgmt->u.probe_resp.timestamp) {
				struct timeval tv;
				do_gettimeofday(&tv);
				mgmt->u.probe_resp.timestamp =
				    ((UINT64) tv.tv_sec * 1000000) + tv.tv_usec;
			}
		} else if (ieee80211_is_disassoc(mgmt->frame_control)) {
			DBGPRINT(RT_DEBUG_ERROR, ("CFG80211_PKT: %s DISASSOC Frame\n", preStr));
		} else if (ieee80211_is_deauth(mgmt->frame_control)) {
			DBGPRINT(RT_DEBUG_ERROR, ("CFG80211_PKT: %s Deauth Frame\n", preStr));
		} else if (ieee80211_is_action(mgmt->frame_control)) {
			PP2P_PUBLIC_FRAME pP2PFrame = (PP2P_PUBLIC_FRAME) pData;

			if (pP2PFrame->Category == CATEGORY_PUBLIC) {
				actionFrame = PUBLIC_ACTION_FRAME_TYPE;

				switch (pP2PFrame->Action) {
				case ACTION_WIFI_DIRECT:
					DBGPRINT(RT_DEBUG_ERROR,
						("Rx %s" MACSTR "->" MACSTR "in CH: %d\n",
						p2p_action_frame_txt(pP2PFrame->Subtype),
						PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
						pAd->LatchRfRegs.Channel));
					break;
				case ACTION_GAS_INITIAL_REQ:
					DBGPRINT(RT_DEBUG_ERROR,
						("ACTION_GAS_INITIAL_REQ\n"));
					break;
				case ACTION_GAS_INITIAL_RSP:
					DBGPRINT(RT_DEBUG_ERROR,
						("ACTION_GAS_INITIAL_RSP\n"));
					break;
				case ACTION_GAS_COMEBACK_REQ:
					DBGPRINT(RT_DEBUG_ERROR,
						("ACTION_GAS_COMEBACK_REQ\n"));
					break;
				case ACTION_GAS_COMEBACK_RSP:
					DBGPRINT(RT_DEBUG_ERROR,
						("ACTION_GAS_COMEBACK_RSP\n"));
					break;
				default:
					DBGPRINT(RT_DEBUG_ERROR,
						("[%s] unknown action frame subtype (%d)\n",
						__func__, pP2PFrame->Subtype));
					break;
				}
			} else if (pP2PFrame->Category == CATEGORY_VENDOR_SPECIFIC_WFD) {
				PP2P_ACTION_FRAME pP2PActFrame = (PP2P_ACTION_FRAME) pData;

				if (RTMPEqualMemory(&pP2PActFrame->Octet[2], CFG_P2POUIBYTE, 4)) {
					actionFrame = P2P_ACTION_FRAME_TYPE;
					switch (pP2PActFrame->Subtype) {
					case P2PACT_NOA:
						DBGPRINT(RT_DEBUG_ERROR,
							 ("CFG80211_PKT: (%02X:%02X:%02X:%02X:%02X:%02X) -> (%02X:%02X:%02X:%02X:%02X:%02X) %s P2PACT_NOA %d\n",
							  PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
							  preStr, pAd->LatchRfRegs.Channel));
						break;
					case P2PACT_PERSENCE_REQ:
						DBGPRINT(RT_DEBUG_ERROR,
							 ("CFG80211_PKT: (%02X:%02X:%02X:%02X:%02X:%02X) -> (%02X:%02X:%02X:%02X:%02X:%02X) %s P2PACT_PERSENCE_REQ %d\n",
							  PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
							  preStr, pAd->LatchRfRegs.Channel));
						break;
					case P2PACT_PERSENCE_RSP:
						DBGPRINT(RT_DEBUG_ERROR,
							 ("CFG80211_PKT: (%02X:%02X:%02X:%02X:%02X:%02X) -> (%02X:%02X:%02X:%02X:%02X:%02X) %s P2PACT_PERSENCE_RSP %d\n",
							  PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
							  preStr, pAd->LatchRfRegs.Channel));
						break;
					case P2PACT_GO_DISCOVER_REQ:
						DBGPRINT(RT_DEBUG_ERROR,
							 ("CFG80211_PKT: (%02X:%02X:%02X:%02X:%02X:%02X) -> (%02X:%02X:%02X:%02X:%02X:%02X) %s P2PACT_GO_DISCOVER_REQ %d\n",
							  PRINT_MAC(mgmt->sa), PRINT_MAC(mgmt->da),
							  preStr, pAd->LatchRfRegs.Channel));
						break;
					default:
						DBGPRINT(RT_DEBUG_ERROR,
							("[%s] unknown WFD Subtype (%d)\n",
							__func__, pP2PActFrame->Subtype));
						break;
					}
				}
			} else {
				DBGPRINT(RT_DEBUG_TRACE,
						("[%s] unknown Action Subtype (%d)\n",
						__func__, pP2PFrame->Subtype));
			}
		} else {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("CFG80211_PKT: %s UNKOWN MGMT FRAME TYPE\n", preStr));
		}
	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("CFG80211_PKT: %s UNKOWN FRAME TYPE\n", preStr));
	}

	return actionFrame;
}

#ifdef CFG_TDLS_SUPPORT
BOOLEAN CFG80211_HandleTdlsDiscoverRespFrame(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR OpMode)
{
	RXWI_STRUC *pRxWI = pRxBlk->pRxWI;
	PHEADER_802_11 pHeader = pRxBlk->pHeader;
	UINT32 freq;
	/* PNDIS_PACKET pRxPacket = pRxBlk->pRxPacket; */
	PP2P_PUBLIC_FRAME pFrame = (PP2P_PUBLIC_FRAME) pHeader;
	UINT32 MPDUtotalByteCnt = 0;

#ifdef MT_MAC
	MPDUtotalByteCnt = pRxBlk->MPDUtotalByteCnt;
#else
	MPDUtotalByteCnt = pRxWI->RXWI_N.MPDUtotalByteCnt;
#endif /* MT_MAC */

	if ((pFrame->p80211Header.FC.SubType == SUBTYPE_ACTION) &&
	    (pFrame->Category == CATEGORY_PUBLIC) &&
	    (pFrame->Action == ACTION_TDLS_DISCOVERY_RSP)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s : GOT ACTION_TDLS_DISCOVERY_RSP ACTION: 0x%x\n", __func__,
			  pFrame->Action));
		MAP_CHANNEL_ID_TO_KHZ(pAd->LatchRfRegs.Channel, freq);
		freq /= 1000;
		CFG80211OS_RxMgmt(pAd->net_dev, freq, (PUCHAR)pHeader, MPDUtotalByteCnt);
		return TRUE;
	}

	/*
	   report TDLS DiscoverRespFrame to supplicant , but it does nothing ,
	   events.c line 2432
	 */

	return FALSE;

}
#endif /* CFG_TDLS_SUPPORT */
BOOLEAN CFG80211_HandleP2pMgmtFrame(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR OpMode)
{
	RXWI_STRUC *pRxWI = pRxBlk->pRxWI;
	PHEADER_802_11 pHeader = pRxBlk->pHeader;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;
	UINT32 freq;

	if ((pHeader->FC.SubType == SUBTYPE_PROBE_REQ) ||
	    ((pHeader->FC.SubType == SUBTYPE_ACTION) &&
	     CFG80211_CheckActionFrameType(pAd, "RX", (PUCHAR) pHeader,
					   pRxWI->RXWI_N.MPDUtotalByteCnt))) {
		MAP_CHANNEL_ID_TO_KHZ(RTMP_GetPrimaryCh(pAd, pAd->LatchRfRegs.Channel), freq);
		freq /= 1000;

#if defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) || defined(RT_CFG80211_AP_SUPPORT)
		/* Check the P2P_GO exist in the VIF List */
		if (pCfg80211_ctrl->Cfg80211VifDevSet.vifDevList.size > 0) {
			PNET_DEV pNetDev = NULL;
			BOOLEAN res = FALSE;
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
			pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd,
					RT_CMD_80211_IFTYPE_P2P_GO);

			if (pNetDev) {
				DBGPRINT(RT_DEBUG_TRACE,
					 ("VIF STA GO RtmpOsCFG80211RxMgmt OK!!\n"));
				DBGPRINT(RT_DEBUG_TRACE,
					 ("TYPE = %d, freq = %d, %02x:%02x:%02x:%02x:%02x:%02x, seq (%d)\n",
						pHeader->FC.SubType, freq,
						PRINT_MAC(pHeader->Addr2), pHeader->Sequence));

				if (pHeader->FC.SubType == SUBTYPE_ACTION) {
					if (CFG80211_CheckActionFrameType
					    (pAd, "CHECK", (PUCHAR) pHeader,
					     pRxWI->RXWI_N.MPDUtotalByteCnt) ==
					    P2P_ACTION_FRAME_TYPE)
						res =
						    CFG80211OS_RxMgmt(pNetDev, freq,
								      (PUCHAR) pHeader,
								      pRxWI->RXWI_N.
								      MPDUtotalByteCnt);
					else
						res =
						    CFG80211OS_RxMgmt(CFG80211_GetEventDevice(pAd),
								      freq, (PUCHAR) pHeader,
								      pRxWI->RXWI_N.
								      MPDUtotalByteCnt);

					if (res != TRUE)
						DBGPRINT(RT_DEBUG_ERROR,
							 ("ACTION CFG80211OS_RxMgmt failed!\n"));
				} else if (pHeader->FC.SubType == SUBTYPE_PROBE_REQ)
					res =
					    CFG80211OS_RxMgmt(pNetDev, freq, (PUCHAR) pHeader,
							      pRxWI->RXWI_N.MPDUtotalByteCnt);
				if (res != TRUE)
					DBGPRINT(RT_DEBUG_INFO,
						 ("CFG80211OS_RxMgmt failed! (%x:%x:%x:%x:%x:%x)\n",
						  PRINT_MAC(pHeader->Addr2)));

			}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
#ifdef RT_CFG80211_AP_SUPPORT
			pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd,
					RT_CMD_80211_IFTYPE_AP);
			if (pNetDev != NULL) {
				if (pHeader->FC.SubType == SUBTYPE_PROBE_REQ)
					res = CFG80211OS_RxMgmt(pNetDev, freq,
								(PUCHAR) pHeader,
								pRxWI->RXWI_N.MPDUtotalByteCnt);
				if (res != TRUE)
					DBGPRINT(RT_DEBUG_INFO,
						 ("CFG80211OS_RxMgmt failed! (%x:%x:%x:%x:%x:%x)\n",
						  PRINT_MAC(pHeader->Addr2)));
			}
#endif /* RT_CFG80211_AP_SUPPORT */
			if (OpMode == OPMODE_AP)
				return TRUE;
		}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

		if (((pHeader->FC.SubType == SUBTYPE_PROBE_REQ) &&
		     (pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame == TRUE)) ||
		    ((pHeader->FC.SubType ==
		      SUBTYPE_ACTION) /*&& ( pAd->Cfg80211RegisterActionFrame == TRUE) */)) {
			DBGPRINT(RT_DEBUG_INFO, ("MAIN STA RtmpOsCFG80211RxMgmt OK!!"));
			DBGPRINT(RT_DEBUG_INFO,
				 ("TYPE = %d, freq = %d, Seq %d, %02x:%02x:%02x:%02x:%02x:%02x\n",
				  pHeader->FC.SubType, freq, pHeader->Sequence, PRINT_MAC(pHeader->Addr2)));
			if (CFG80211_GetEventDevice(pAd) == NULL) {	/* Fix crash problem when hostapd boot up. */
				DBGPRINT(RT_DEBUG_ERROR, ("Not Ready for p2p0 netdevice\n"));
				return FALSE;
			}
			if (RTMP_CFG80211_HOSTAPD_ON(pAd))
				CFG80211OS_RxMgmt(pAd->net_dev, freq, (PUCHAR) pHeader,
					pRxWI->RXWI_N.MPDUtotalByteCnt);
			else
				CFG80211OS_RxMgmt(CFG80211_GetEventDevice(pAd), freq,
					(PUCHAR) pHeader, pRxWI->RXWI_N.MPDUtotalByteCnt);

			if (OpMode == OPMODE_AP)
				return TRUE;
		}
	}

	return FALSE;
}

#endif /* RT_CFG80211_SUPPORT */
