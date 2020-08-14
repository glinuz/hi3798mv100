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

	All CFG80211 Function Prototype.

***************************************************************************/

#ifndef __CFG80211EXTR_H__
#define __CFG80211EXTR_H__

#ifdef RT_CFG80211_SUPPORT

#define CFG80211CB				    (pAd->pCfg80211_CB)
#define RT_CFG80211_DEBUG	/* debug use */
#ifdef RT_CFG80211_DEBUG
#define CFG80211DBG(__Flg, __pMsg)		DBGPRINT(__Flg, __pMsg)
#else
#define CFG80211DBG(__Flg, __pMsg)
#endif /* RT_CFG80211_DEBUG */

/* CFG_TODO */
#include "wfa_p2p.h"

#define RT_CFG80211_BEACON_TIM_UPDATE(__pAd, __pNetDev)                                \
	CFG80211_UpdateBeacon((VOID *)__pAd, __pNetDev, NULL, 0, NULL, 0, FALSE)

#define RT_CFG80211_CRDA_REG_HINT(__pAd, __pCountryIe, __CountryIeLen)		\
	CFG80211_RegHint((VOID *)__pAd, __pCountryIe, __CountryIeLen)

#define RT_CFG80211_CRDA_REG_HINT11D(__pAd, __pCountryIe, __CountryIeLen)	\
	CFG80211_RegHint11D((VOID *)__pAd, __pCountryIe, __CountryIeLen)

#define RT_CFG80211_CRDA_REG_RULE_APPLY(__pAd)								\
	CFG80211_RegRuleApply((VOID *)__pAd, NULL, __pAd->cfg80211_ctrl.Cfg80211_Alpha2)

#define RT_CFG80211_CONN_RESULT_INFORM(__pAd, __pBSSID, __pReqIe,			\
			__ReqIeLen,	__pRspIe, __RspIeLen, __FlgIsSuccess)				\
	CFG80211_ConnectResultInform((VOID *)__pAd, __pBSSID,					\
			__pReqIe, __ReqIeLen, __pRspIe, __RspIeLen, __FlgIsSuccess)

#define RT_CFG80211_SCANNING_INFORM(__pAd, __BssIdx, __ChanId, __pFrame,	\
			__FrameLen, __RSSI)									\
	CFG80211_Scaning((VOID *)__pAd, __BssIdx, __ChanId, __pFrame,			\
						__FrameLen, __RSSI)

#define RT_CFG80211_SCAN_END(__pAd, __FlgIsAborted) CFG80211_ScanEnd((VOID *)__pAd, __FlgIsAborted)
#ifdef CONFIG_STA_SUPPORT
#define RT_CFG80211_LOST_AP_INFORM(__pAd)   CFG80211_LostApInform((VOID *)__pAd)
#endif /*CONFIG_STA_SUPPORT */
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
#define RT_CFG80211_LOST_GO_INFORM(__pAd, _Reason) CFG80211_LostP2pGoInform((VOID *)__pAd, _Reason)
#endif /*RT_CFG80211_P2P_CONCURRENT_DEVICE */
#define RT_CFG80211_REINIT(__pAd)    CFG80211_SupBandReInit((VOID *)__pAd)

#define RT_CFG80211_RFKILL_STATUS_UPDATE(_pAd, _active)					\
	CFG80211_RFKillStatusUpdate(_pAd, _active)

#define RT_CFG80211_P2P_CLI_CONN_RESULT_INFORM(__pAd, __pBSSID, __pReqIe,               \
			__ReqIeLen,	__pRspIe, __RspIeLen, __FlgIsSuccess)		\
	CFG80211_P2pClientConnectResultInform(__pAd, __pBSSID,			        \
			__pReqIe, __ReqIeLen, __pRspIe, __RspIeLen, __FlgIsSuccess)

#define RT_CFG80211_P2P_CLI_SEND_NULL_FRAME(_pAd, _PwrMgmt)				\
	CFG80211_P2pClientSendNullFrame(_pAd, _PwrMgmt)

#define RT_CFG80211_GET_IF_TYPE(_pNetDev) _pNetDev->ieee80211_ptr->iftype
#ifdef SINGLE_SKU
#define CFG80211_BANDINFO_FILL(__pAd, __pBandInfo)                                   \
{                                                                                    \
	(__pBandInfo)->RFICType = __pAd->phy_ctrl.rf_band_cap;	                     \
	(__pBandInfo)->MpduDensity = __pAd->CommonCfg.BACapability.field.MpduDensity;\
	(__pBandInfo)->TxStream = __pAd->CommonCfg.TxStream;			     \
	(__pBandInfo)->RxStream = __pAd->CommonCfg.RxStream;			     \
	(__pBandInfo)->MaxTxPwr = __pAd->CommonCfg.DefineMaxTxPwr;		     \
	if (WMODE_EQUAL(__pAd->CommonCfg.PhyMode, WMODE_B))			     \
		(__pBandInfo)->FlgIsBMode = TRUE;				     \
	else							                     \
		(__pBandInfo)->FlgIsBMode = FALSE;			             \
	if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode))		\
		(__pBandInfo)->FlgVhtSupport = TRUE;		\
	else											\
		(__pBandInfo)->FlgVhtSupport = FALSE;		\
	(__pBandInfo)->MaxBssTable = MAX_LEN_OF_BSS_TABLE;	                     \
	(__pBandInfo)->RtsThreshold = pAd->CommonCfg.RtsThreshold;                   \
	(__pBandInfo)->FragmentThreshold = pAd->CommonCfg.FragmentThreshold;	     \
	(__pBandInfo)->RetryMaxCnt = 0;	                                             \
	RTMP_IO_READ32(__pAd, TX_RTY_CFG, &((__pBandInfo)->RetryMaxCnt));            \
}
#else
#define CFG80211_BANDINFO_FILL(__pAd, __pBandInfo)                                   \
{                                                                                    \
	(__pBandInfo)->RFICType = __pAd->phy_ctrl.rf_band_cap;                       \
	(__pBandInfo)->MpduDensity = __pAd->CommonCfg.BACapability.field.MpduDensity;\
	(__pBandInfo)->TxStream = __pAd->CommonCfg.TxStream;                         \
	(__pBandInfo)->RxStream = __pAd->CommonCfg.RxStream;                         \
	(__pBandInfo)->MaxTxPwr = 0;                                                 \
	if (WMODE_EQUAL(__pAd->CommonCfg.PhyMode, WMODE_B))                          \
		(__pBandInfo)->FlgIsBMode = TRUE;                                    \
	else                                                                         \
		(__pBandInfo)->FlgIsBMode = FALSE;                                   \
	if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode))		\
		(__pBandInfo)->FlgVhtSupport = TRUE;		\
	else											\
		(__pBandInfo)->FlgVhtSupport = FALSE;		\
	(__pBandInfo)->MaxBssTable = MAX_LEN_OF_BSS_TABLE;                           \
	(__pBandInfo)->RtsThreshold = pAd->CommonCfg.RtsThreshold;                   \
	(__pBandInfo)->FragmentThreshold = pAd->CommonCfg.FragmentThreshold;	     \
	(__pBandInfo)->RetryMaxCnt = 0;	                                             \
	RTMP_IO_READ32(__pAd, TX_RTY_CFG, &((__pBandInfo)->RetryMaxCnt));	     \
}
#endif /* SINGLE_SKU */

/* Scan Releated */
#ifdef CONFIG_STA_SUPPORT
BOOLEAN CFG80211DRV_OpsScanRunning(VOID *pAdOrg);
#endif /*CONFIG_STA_SUPPORT */
BOOLEAN CFG80211DRV_OpsScanSetSpecifyChannel(VOID *pAdOrg, VOID *pData, UINT8 dataLen);

BOOLEAN CFG80211DRV_OpsScanCheckStatus(VOID *pAdOrg, UINT8 IfType);

#ifdef CONFIG_MULTI_CHANNEL
VOID CFG80211DRV_Set_NOA(VOID *pAdOrg, VOID *pData);
#endif /* CONFIG_MULTI_CHANNEL */

BOOLEAN CFG80211DRV_OpsScanExtraIesSet(VOID *pAdOrg);

VOID CFG80211DRV_OpsScanInLinkDownAction(VOID *pAdOrg);

INT CFG80211DRV_OpsScanGetNextChannel(VOID *pAdOrg, BOOLEAN pending);

VOID CFG80211_ScanStatusLockInit(VOID *pAdCB, UINT init);

VOID CFG80211_Scaning(VOID *pAdCB, UINT32 BssIdx, UINT32 ChanId, UCHAR *pFrame, UINT32 FrameLen,
		      INT32 RSSI);

VOID CFG80211_ScanEnd(VOID *pAdCB, BOOLEAN FlgIsAborted);

/* Connect Releated */
BOOLEAN CFG80211DRV_OpsLeave(VOID *pAdOrg, UINT8 IfType);
BOOLEAN CFG80211DRV_Connect(VOID *pAdOrg, VOID *pData);
VOID CFG80211_P2pClientConnectResultInform(IN VOID *pAdCB,
					   IN UCHAR *pBSSID,
					   IN UCHAR *pReqIe,
					   IN UINT32 ReqIeLen,
					   IN UCHAR *pRspIe,
					   IN UINT32 RspIeLen, IN UCHAR FlgIsSuccess);
VOID CFG80211_ConnectResultInform(VOID *pAdCB, UCHAR *pBSSID, UCHAR *pReqIe, UINT32 ReqIeLen,
				  UCHAR *pRspIe, UINT32 RspIeLen, UCHAR FlgIsSuccess);
VOID CFG80211DRV_PmkidConfig(VOID *pAdOrg, VOID *pData);

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
VOID CFG80211_LostP2pGoInform(VOID *pAdCB, UINT16 Reason);
#endif /*RT_CFG80211_P2P_CONCURRENT_DEVICE */
VOID CFG80211_LostApInform(VOID *pAdCB);

INT CFG80211_StaPortSecured(RTMP_ADAPTER *pAd, const UCHAR *mac, UINT flag);

/* AP Related*/
INT CFG80211_ApStaDel(RTMP_ADAPTER *pAd, const UCHAR *mac);

VOID CFG80211_UpdateBeacon(VOID *pAdOrg,
			   PNET_DEV pNetDev,
			   UCHAR *beacon_head_buf,
			   UINT32 beacon_head_len,
			   UCHAR *beacon_tail_buf, UINT32 beacon_tail_len, BOOLEAN isAllUpdate);

INT CFG80211_ApStaDelSendEvent(PRTMP_ADAPTER pAd, const PUCHAR mac_addr);

/* Information Releated */
BOOLEAN CFG80211DRV_StaGet(RTMP_ADAPTER *pAd, const u8 *mac, CMD_IOCTL_80211_STA_INFO *sta);

VOID CFG80211DRV_SurveyGet(VOID *pAdOrg, VOID *pData);

INT CFG80211_reSetToDefault(VOID *pAdCB);

/* Key Releated */
BOOLEAN CFG80211DRV_StaKeyAdd(VOID *pAdOrg, VOID *pData);

BOOLEAN CFG80211DRV_ApKeyAdd(VOID *pAdOrg, VOID *pData);

BOOLEAN CFG80211DRV_ApKeyDel(VOID *pAdOrg, VOID *pData);

VOID CFG80211DRV_RtsThresholdAdd(VOID *pAdOrg, UINT threshold);

VOID CFG80211DRV_FragThresholdAdd(VOID *pAdOrg, UINT threshold);

INT CFG80211_setApDefaultKey(VOID *pAdCB, INT *pData, UINT Data);

INT CFG80211_setPowerMgmt(VOID *pAdCB, UINT Enable);

#ifdef CONFIG_STA_SUPPORT
INT CFG80211_setStaDefaultKey(VOID *pAdCB, UINT Data);

#ifdef DOT11W_PMF_SUPPORT
INT CFG80211_setStaMgmtDefaultKey(VOID *pAdCB, UINT Data);
#endif /* DOT11W_PMF_SUPPORT */

#endif /*CONFIG_STA_SUPPORT */
/* General Releated */
BOOLEAN CFG80211DRV_OpsSetChannel(RTMP_ADAPTER *pAd, VOID *pData);

BOOLEAN CFG80211DRV_OpsChgVirtualInf(RTMP_ADAPTER *pAd, VOID *pData);

VOID CFG80211DRV_OpsChangeBssParm(VOID *pAdOrg, VOID *pData);

VOID CFG80211_UnRegister(VOID *pAdOrg, VOID *pNetDev);

VOID CFG80211_BeaconCountryRegionParse(IN VOID *pAdCB,
				       IN NDIS_802_11_VARIABLE_IEs * pVIE, IN UINT16 LenVIE);

INT CFG80211DRV_IoctlHandle(VOID *pAdSrc,
			    RTMP_IOCTL_INPUT_STRUCT *wrq,
			    INT cmd, USHORT subcmd, VOID *pData, ULONG Data);

UCHAR CFG80211_getCenCh(RTMP_ADAPTER *pAd, UCHAR prim_ch);

/* CRDA Releatd */
VOID CFG80211DRV_RegNotify(VOID *pAdOrg, VOID *pData);

VOID CFG80211_RegHint(VOID *pAdCB, UCHAR *pCountryIe, ULONG CountryIeLen);

VOID CFG80211_RegHint11D(VOID *pAdCB, UCHAR *pCountryIe, ULONG CountryIeLen);

VOID CFG80211_RegRuleApply(VOID *pAdCB, VOID *pWiphy, UCHAR *pAlpha2);
#ifdef JUNGLE_SUPPORT
NDIS_STATUS CFG80211_SyncCore_ChList(IN VOID *pAdCB, IN VOID *pWiphy);
#endif
BOOLEAN CFG80211_SupBandReInit(VOID *pAdCB);

#ifdef RFKILL_HW_SUPPORT
VOID CFG80211_RFKillStatusUpdate(PVOID pAd, BOOLEAN active);
#endif /* RFKILL_HW_SUPPORT */

/* P2P Related */
VOID CFG80211DRV_SetP2pCliAssocIe(RTMP_ADAPTER *pAdOrg, const u8 *pData, size_t ie_len);
VOID CFG80211DRV_P2pClientKeyAdd(VOID *pAdOrg, VOID *pData);

BOOLEAN CFG80211DRV_P2pClientConnect(VOID *pAdOrg, VOID *pData);

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
BOOLEAN CFG80211_checkScanResInKernelCache(IN VOID *pAdCB,
					   IN UCHAR *pBSSID, IN UCHAR *pSsid, IN INT ssidLen);

BOOLEAN CFG80211_checkScanTable(IN VOID *pAdCB);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
VOID CFG80211_P2pClientSendNullFrame(VOID *pAdCB, INT PwrMgmt);

VOID CFG80211RemainOnChannelTimeout(PVOID SystemSpecific1,
				    PVOID FunctionContext,
				    PVOID SystemSpecific2, PVOID SystemSpecific3);

BOOLEAN CFG80211DRV_OpsRemainOnChannel(VOID *pAdOrg, VOID *pData, UINT32 duration);

BOOLEAN CFG80211DRV_OpsCancelRemainOnChannel(VOID *pAdOrg, UINT32 cookie);

VOID CFG80211DRV_OpsMgmtFrameProbeRegister(VOID *pAdOrg, VOID *pData, BOOLEAN isReg);

VOID CFG80211DRV_OpsMgmtFrameActionRegister(VOID *pAdOrg, VOID *pData, BOOLEAN isReg);

ACTION_FRAME_TYPE CFG80211_CheckActionFrameType(IN RTMP_ADAPTER *pAd,
						IN PUCHAR preStr,
						IN PUCHAR pData, IN UINT32 length);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
BOOLEAN CFG80211_SyncPacketWmmIe(RTMP_ADAPTER *pAd, UINT mbss_idx, VOID *pData, ULONG dataLen);
#endif /* endif */
BOOLEAN CFG80211_HandleP2pMgmtFrame(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR OpMode);
INT CFG80211_SendMgmtFrame(RTMP_ADAPTER *pAd, VOID *pMgmtFrame);
INT CFG80211_SetTxNdev(RTMP_ADAPTER *pAd, PNET_DEV pNetdev);

#ifdef RT_CFG80211_P2P_SUPPORT
VOID CFG80211_P2PMakeFakeNoATlv(PRTMP_ADAPTER pAd, ULONG StartTime, PUCHAR pOutBuffer);
BOOLEAN CFG80211_P2pAdjustSwNoATimer(PRTMP_ADAPTER pAd, ULONG CurrentTimeStamp,
				     ULONG NextTimePoint);
VOID CFG80211_P2pGPTimeOutHandle(PRTMP_ADAPTER pAd);
VOID CFG80211_PeerP2pBeacon(IN PRTMP_ADAPTER pAd,
			    IN PUCHAR pAddr2,
			    IN MLME_QUEUE_ELEM * Elem, IN LARGE_INTEGER TimeStamp);

VOID CFG80211_P2pGOStartNoA(PRTMP_ADAPTER pAd);

VOID CFG80211_P2pStopNoA(IN PRTMP_ADAPTER pAd, IN PMAC_TABLE_ENTRY pMacClient);

VOID CFG80211_P2pStartOpPS(PRTMP_ADAPTER pAd);
VOID CFG80211_P2pStopOpPS(PRTMP_ADAPTER pAd);

BOOLEAN CFG80211_P2pResetNoATimer(IN PRTMP_ADAPTER pAd, IN ULONG DiffTimeInus);

BOOLEAN CFG80211_P2pHandleNoAAttri(IN PRTMP_ADAPTER pAd,
				   IN PMAC_TABLE_ENTRY pMacClient, IN PUCHAR pData);

VOID CFG80211_P2pParseNoASubElmt(PRTMP_ADAPTER pAd, VOID *Msg, ULONG MsgLen,
				 UCHAR wcidindex, UINT32 Sequence);

BOOLEAN CFG8211_PeerP2pBeaconSanity(PRTMP_ADAPTER pAd, VOID *Msg, ULONG MsgLen,
				    PUCHAR pAddr2, CHAR Ssid[], UCHAR *pSsidLen,
				    ULONG *Peerip, ULONG *P2PSubelementLen,
				    PUCHAR pP2pSubelement);

#endif /* RT_CFG80211_P2P_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
VOID CFG80211_ParseBeaconIE(RTMP_ADAPTER *pAd, MULTISSID_STRUCT * pMbss, struct wifi_dev *wdev,
			    const UCHAR *wpa_ie, const UCHAR *rsn_ie);
#endif /* endif */

/* -------------------------------- */
VOID CFG80211_Convert802_3Packet(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR *pHeader802_3);
VOID CFG80211_Announce802_3Packet(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR FromWhichBSSID);
VOID CFG80211_SendMgmtFrameDone(RTMP_ADAPTER *pAd, USHORT Sequence, BOOLEAN Ack);
/*tx status*/
#define MAX_MGMTID 7		/*0~7, 8 sets of MgmtID */
#define TXS_TIMEOUT  70		/*timeout for txs cmd,  for worst case:WCNCR00033034*/
#define DRIVER_TXS_TIMEOUT  120   /*timeout for driver wait for fw txs event*/
#define TXS_P2P_MgmtID  0
#define TXS_P2P_WCID  2

BOOLEAN CFG80211_IsNeedTxStatus(
			VOID *pAdOrg,
			UCHAR *pFrame,
			IN UINT32 FrameLength,
			BOOLEAN NeedTxStatus);

VOID CFG80211_TxStatusEvent(RTMP_ADAPTER *pAd, UCHAR *EventContent, UINT32 Length);

int CFG80211_TxStatusCmd(RTMP_ADAPTER *pAd, UINT8 Timeout, UINT8 Seq, UINT8 WCID, UINT8 MgmtID,
			 UINT32 PktLen);
/*~tx status*/
#define APCLI_CONN_TIMEOUT  2000		/*timeout for apcli connect */

VOID CFG80211_SwitchTxChannel(RTMP_ADAPTER *pAd, ULONG Data);

BOOLEAN CFG80211DRV_OpsBeaconSet(VOID *pAdOrg, VOID *pData);

BOOLEAN CFG80211DRV_OpsBeaconAdd(VOID *pAdOrg, VOID *pData);

VOID CFG80211DRV_DisableApInterface(PRTMP_ADAPTER pAd,
				    struct net_device *pNetDev,
				    u8 oldIfType);

INT RTMP_CFG80211_GET_MBSSID_INDEX(VOID *pAdSrc, PNET_DEV pNetDev);

VOID RTMP_CFG80211_DISABLE_BEACON(VOID *pAdSrc, PNET_DEV pNetDev,
				  UINT8 oldIfType);

#if defined(CONFIG_AP_SUPPORT) || defined(RT_CFG80211_AP_SUPPORT)
VOID RTMP_CFG80211_ENABLE_BEACON(VOID *pAdSrc, PNET_DEV pNetDev);
#endif /* defined(CONFIG_AP_SUPPORT) || defined(RT_CFG80211_AP_SUPPORT) */

#ifdef CFG_TDLS_SUPPORT
BOOLEAN CFG80211DRV_StaTdlsInsertDeletepEntry(VOID *pAdOrg, const VOID *pData, UINT Data);
BOOLEAN CFG80211DRV_StaTdlsSetKeyCopyFlag(VOID *pAdOrg);
BOOLEAN CFG80211_HandleTdlsDiscoverRespFrame(RTMP_ADAPTER *pAd, RX_BLK *pRxBlk, UCHAR OpMode);

VOID cfg_tdls_send_PeerTrafficIndication(PRTMP_ADAPTER pAd, u8 *peer);
VOID cfg_tdls_rcv_PeerTrafficIndication(PRTMP_ADAPTER pAd, u8 dialog_token, u8 *peer);
INT cfg_tdls_rcv_PeerTrafficResponse(PRTMP_ADAPTER pAd, u8 *peer);
INT cfg_tdls_search_wcid(PRTMP_ADAPTER pAd, u8 *peer);
INT cfg_tdls_search_ValidLinkIndex(PRTMP_ADAPTER pAd, u8 *peer);
INT cfg_tlds_build_frame(PRTMP_ADAPTER pAd, const u8 *peer, u8 dialog_token, u8 action_code,
			 u16 status_code, const u8 *extra_ies, size_t extra_ies_len,
			 BOOLEAN send_by_tdls_link, u8 tdls_entry_wcid, u8 reason_code);
VOID cfg_tdls_UAPSDP_PsmModeChange(PRTMP_ADAPTER pAd, USHORT PsmOld, USHORT PsmNew);
BOOLEAN cfg_tdls_UAPSDP_AsicCanSleep(PRTMP_ADAPTER pAd);
INT cfg_tdls_EntryInfo_Display_Proc(PRTMP_ADAPTER pAd, PUCHAR arg);
VOID cfg_tdls_TimerInit(PRTMP_ADAPTER pAd);
VOID cfg_tdls_PTITimeoutAction(IN PVOID SystemSpecific1, IN PVOID FunctionContext,
			       IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);
#ifdef CERTIFICATION_SIGMA_SUPPORT
VOID cfg_tdls_ResetTbttTimer(IN PVOID SystemSpecific1, IN PVOID FunctionContext,
			     IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);
#endif /* CERTIFICATION_SIGMA_SUPPORT */
VOID cfg_tdls_BaseChannelTimeoutAction(IN PVOID SystemSpecific1, IN PVOID FunctionContext,
				       IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);
VOID cfg_tdls_rx_parsing(PRTMP_ADAPTER pAd, RX_BLK *pRxBlk);
INT cfg_tdls_chsw_req(PRTMP_ADAPTER pAd, u8 *peer, u8 target_channel, u8 target_bw);
INT cfg_tdls_chsw_resp(PRTMP_ADAPTER pAd, u8 *peer, UINT32 ch_sw_time, UINT32 ch_sw_timeout,
		       u16 reason_code);
VOID cfg_tdls_prepare_null_frame(PRTMP_ADAPTER pAd, BOOLEAN powersave, UCHAR dir, UCHAR *peerAddr);
VOID cfg_tdls_TunneledProbeRequest(PRTMP_ADAPTER pAd, const u8 *pMacAddr, const u8 *extra_ies,
				   size_t extra_ies_len);
VOID cfg_tdls_TunneledProbeResponse(PRTMP_ADAPTER pAd, const u8 *pMacAddr, const u8 *extra_ies,
				    size_t extra_ies_len);
VOID cfg_auto_teardown(PRTMP_ADAPTER pAd, UCHAR *peerAddr);
VOID cfg_tdls_LinkMaintenance(PRTMP_ADAPTER pAd);

int cfg_tdls_send_CH_SW_SETUP(RTMP_ADAPTER *ad, UCHAR cmd, UINT32 start_time_tsf);
#endif /*CFG_TDLS_SUPPORT */
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
BOOLEAN CFG80211DRV_OpsVifAdd(VOID *pAdOrg, VOID *pData);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
#endif /* RT_CFG80211_SUPPORT */

#endif /* __CFG80211EXTR_H__ */
