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

	All related CFG80211 function body.

	History:

***************************************************************************/
#define RTMP_MODULE_OS

#ifdef RT_CFG80211_SUPPORT

#include "rt_config.h"

extern struct notifier_block cfg80211_netdev_notifier;

extern INT RtmpIoctl_rt_ioctl_siwauth(IN RTMP_ADAPTER * pAd, IN VOID * pData, IN ULONG Data);

INT CFG80211DRV_IoctlHandle(IN VOID * pAdSrc,
			    IN RTMP_IOCTL_INPUT_STRUCT * wrq,
			    IN INT cmd, IN USHORT subcmd, IN VOID * pData, IN ULONG Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;

	switch (cmd) {
	case CMD_RTPRIV_IOCTL_80211_START:
	case CMD_RTPRIV_IOCTL_80211_END:
		/* nothing to do */
		break;

	case CMD_RTPRIV_IOCTL_80211_CB_GET:
		*(VOID **) pData = (VOID *) (pAd->pCfg80211_CB);
		break;

	case CMD_RTPRIV_IOCTL_80211_CB_SET:
		pAd->pCfg80211_CB = pData;
		break;

	case CMD_RTPRIV_IOCTL_80211_CHAN_SET:
		if (CFG80211DRV_OpsSetChannel(pAd, pData) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_80211_VIF_CHG:
		if (CFG80211DRV_OpsChgVirtualInf(pAd, pData) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_80211_SCAN:
		if (CFG80211DRV_OpsScanCheckStatus(pAd, Data) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_80211_SCAN_STATUS_LOCK_INIT:
		CFG80211_ScanStatusLockInit(pAd, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_STA_LEAVE:
		CFG80211DRV_OpsLeave(pAd, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_STA_KEY_ADD:
		CFG80211DRV_StaKeyAdd(pAd, pData);
		break;

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	case CMD_RTPRIV_IOCTL_80211_P2P_CLIENT_KEY_ADD:
		CFG80211DRV_P2pClientKeyAdd(pAd, pData);
		break;
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	case CMD_RTPRIV_IOCTL_80211_POWER_MGMT_SET:
		CFG80211_setPowerMgmt(pAd, Data);
		break;

#ifdef CONFIG_STA_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_STA_KEY_DEFAULT_SET:
		CFG80211_setStaDefaultKey(pAd, Data);
		break;

#ifdef DOT11W_PMF_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_STA_MGMT_KEY_DEFAULT_SET:
		CFG80211_setStaMgmtDefaultKey(pAd, Data);
		break;
#endif /* DOT11W_PMF_SUPPORT */

#endif /*CONFIG_STA_SUPPORT */
	case CMD_RTPRIV_IOCTL_80211_CONNECT_TO:
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
		if (Data == RT_CMD_80211_IFTYPE_P2P_CLIENT)
			CFG80211DRV_P2pClientConnect(pAd, pData);
		else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
			CFG80211DRV_Connect(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_REG_NOTIFY_TO:
		CFG80211DRV_RegNotify(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_UNREGISTER:
		CFG80211_UnRegister(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_BANDINFO_GET:
		{
			CFG80211_BAND *pBandInfo = (CFG80211_BAND *) pData;

			CFG80211_BANDINFO_FILL(pAd, pBandInfo);
		}
		break;

	case CMD_RTPRIV_IOCTL_80211_SURVEY_GET:
		CFG80211DRV_SurveyGet(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_EXTRA_IES_SET:
		CFG80211DRV_OpsScanExtraIesSet(pAd);
		break;

#ifdef RT_CFG80211_P2P_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_REMAIN_ON_CHAN_SET:
		CFG80211DRV_OpsRemainOnChannel(pAd, pData, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_CANCEL_REMAIN_ON_CHAN_SET:
		CFG80211DRV_OpsCancelRemainOnChannel(pAd, Data);
		break;
#endif /* RT_CFG80211_P2P_SUPPORT */

		/* CFG_TODO */
	case CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_REG:
		CFG80211DRV_OpsMgmtFrameProbeRegister(pAd, pData, Data);
		break;

		/* CFG_TODO */
	case CMD_RTPRIV_IOCTL_80211_ACTION_FRAME_REG:
		CFG80211DRV_OpsMgmtFrameActionRegister(pAd, pData, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_CHANNEL_LOCK:
		CFG80211_SwitchTxChannel(pAd, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_CHANNEL_RESTORE:
		break;

	case CMD_RTPRIV_IOCTL_80211_MGMT_FRAME_SEND:
		CFG80211_SendMgmtFrame(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_TX_NETDEV_SET:
		CFG80211_SetTxNdev(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_CHANNEL_LIST_SET:
		return CFG80211DRV_OpsScanSetSpecifyChannel(pAd, pData, Data);

#ifdef CONFIG_AP_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_BEACON_SET:
		CFG80211DRV_OpsBeaconSet(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_BEACON_ADD:
		CFG80211DRV_OpsBeaconAdd(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_BEACON_DEL:
		{
			INT i;
			PNET_DEV pNetDev = (PNET_DEV) pData;
			INT mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd,
								       pNetDev);
			PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[mbssidIdx];

			for (i = 0; i < WLAN_MAX_NUM_OF_TIM; i++)
				pMbss->TimBitmaps[i] = 0;
			if (pMbss->beacon_tail_buf != NULL) {
				os_free_mem(NULL, pMbss->beacon_tail_buf);
				pMbss->beacon_tail_buf = NULL;
			}
			pMbss->beacon_tail_len = 0;
			pMbss->wdev.allow_data_tx = FALSE;
		}
		break;

	case CMD_RTPRIV_IOCTL_80211_AP_KEY_ADD:
		CFG80211DRV_ApKeyAdd(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_RTS_THRESHOLD_ADD:
		CFG80211DRV_RtsThresholdAdd(pAd, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_FRAG_THRESHOLD_ADD:
		CFG80211DRV_FragThresholdAdd(pAd, Data);
		break;

	case CMD_RTPRIV_IOCTL_80211_AP_KEY_DEL:
		CFG80211DRV_ApKeyDel(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_AP_KEY_DEFAULT_SET:
		CFG80211_setApDefaultKey(pAd, pData, Data);
		break;

#endif /* CONFIG_AP_SUPPORT */

	case CMD_RTPRIV_IOCTL_80211_CHANGE_BSS_PARM:
		CFG80211DRV_OpsChangeBssParm(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_80211_AP_PROBE_RSP_EXTRA_IE:
		break;

	case CMD_RTPRIV_IOCTL_80211_BITRATE_SET:
		break;

	case CMD_RTPRIV_IOCTL_80211_RESET:
		CFG80211_reSetToDefault(pAd);
		break;

	case CMD_RTPRIV_IOCTL_80211_NETDEV_EVENT:
		{
			/*
			   CFG_TODO: For Scan_req per netdevice
			   struct wireless_dev *pWdev = pAd->pCfg80211_CB->pCfg80211_Wdev;
			   if (RTMPEqualMemory(pNetDev->dev_addr, pNewNetDev->dev_addr, MAC_ADDR_LEN))
			 */
			if (pAd->cfg80211_ctrl.FlgCfg80211Scanning == TRUE) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("CFG_SCAN: close the scan cmd in device close phase\n"));
				CFG80211OS_ScanEnd(pAd->pCfg80211_CB, TRUE);
				pAd->cfg80211_ctrl.FlgCfg80211Scanning = FALSE;
			}
		}
		break;

#ifdef CONFIG_MULTI_CHANNEL
	case CMD_RTPRIV_IOCTL_MCC_DHCP_PROTECT_STATUS:
		DBGPRINT(RT_DEBUG_TRACE, ("pAd->MCC_DHCP_Protect  %d\n", pAd->MCC_DHCP_Protect));
		*(UCHAR *) pData = (UCHAR) pAd->MCC_DHCP_Protect;
		break;

	case CMD_RTPRIV_IOCTL_80211_SET_NOA:
		CFG80211DRV_Set_NOA(pAd, pData);
		break;

#endif /* CONFIG_MULTI_CHANNEL */

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	case CMD_RTPRIV_IOCTL_80211_VIF_ADD:
		if (CFG80211DRV_OpsVifAdd(pAd, pData) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_80211_VIF_DEL:
		RTMP_CFG80211_VirtualIF_Remove(pAd, pData, Data);
		break;
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

#ifdef RT_CFG80211_ANDROID_PRIV_LIB_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_ANDROID_PRIV_CMD:
		/* rt_android_private_command_entry(pAd, ); */
		break;
#endif /* RT_CFG80211_ANDROID_PRIV_LIB_SUPPORT */

#ifdef RT_P2P_SPECIFIC_WIRELESS_EVENT
	case CMD_RTPRIV_IOCTL_80211_SEND_WIRELESS_EVENT:
		CFG80211_SendWirelessEvent(pAd, pData);
		break;
#endif /* RT_P2P_SPECIFIC_WIRELESS_EVENT */

#ifdef RFKILL_HW_SUPPORT
	case CMD_RTPRIV_IOCTL_80211_RFKILL:
		{
			UINT32 data = 0;
			BOOLEAN active;

			/* Read GPIO pin2 as Hardware controlled radio state */
			RTMP_IO_READ32(pAd, GPIO_CTRL_CFG, &data);
			active = !!(data & 0x04);

			if (!active) {
				RTMPSetLED(pAd, LED_RADIO_OFF);
				*(UINT8 *) pData = 0;
			} else
				*(UINT8 *) pData = 1;
		}
		break;
#endif /* RFKILL_HW_SUPPORT */

	default:
		return NDIS_STATUS_FAILURE;
	}

	return NDIS_STATUS_SUCCESS;
}

VOID CFG80211DRV_OpsMgmtFrameProbeRegister(VOID *pAdOrg, VOID *pData, BOOLEAN isReg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	PNET_DEV pNewNetDev = (PNET_DEV) pData;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	/* Search the CFG80211 VIF List First */
	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (RTMPEqualMemory
		    (pDevEntry->net_dev->dev_addr, pNewNetDev->dev_addr, MAC_ADDR_LEN))
			break;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	/* Check The Registration is for VIF Device */
	if ((pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) && (pDevEntry != NULL)) {
		if (isReg) {
			if (pDevEntry->Cfg80211ProbeReqCount < 255)
				pDevEntry->Cfg80211ProbeReqCount++;
		} else {
			if (pDevEntry->Cfg80211ProbeReqCount > 0)
				pDevEntry->Cfg80211ProbeReqCount--;
		}
		if (pDevEntry->Cfg80211ProbeReqCount > 0)
			pDevEntry->Cfg80211RegisterProbeReqFrame = TRUE;
		else
			pDevEntry->Cfg80211RegisterProbeReqFrame = FALSE;

#ifndef RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE
		return;		/* workaround? check formal solution */
#endif /* RT_CFG80211_P2P_STATIC_CONCURRENT_DEVICE */
	}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	/* IF Not Exist on VIF List, the device must be MAIN_DEV */
	if (isReg) {
		if (pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount < 255)
			pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount++;
	} else {
		if (pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount > 0)
			pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount--;
	}

	if (pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount > 0)
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame = TRUE;
	else {
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame = FALSE;
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount = 0;
	}

	DBGPRINT(RT_DEBUG_INFO, ("[%d] pAd->Cfg80211RegisterProbeReqFrame=%d[%d]\n",
				 isReg,
				 pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame,
				 pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount));
}

VOID CFG80211DRV_OpsMgmtFrameActionRegister(VOID *pAdOrg, VOID *pData, BOOLEAN isReg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	PNET_DEV pNewNetDev = (PNET_DEV) pData;
	PLIST_HEADER pCacheList = &pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList;
	PCFG80211_VIF_DEV pDevEntry = NULL;
	PLIST_ENTRY pListEntry = NULL;

	/* Search the CFG80211 VIF List First */
	pListEntry = pCacheList->pHead;
	pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	while (pDevEntry != NULL) {
		if (RTMPEqualMemory
		    (pDevEntry->net_dev->dev_addr, pNewNetDev->dev_addr, MAC_ADDR_LEN))
			break;

		pListEntry = pListEntry->pNext;
		pDevEntry = (PCFG80211_VIF_DEV) pListEntry;
	}

	/* Check The Registration is for VIF Device */
	if ((pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) && (pDevEntry != NULL)) {
		if (isReg)
			pDevEntry->Cfg80211ActionCount++;
		else
			pDevEntry->Cfg80211ActionCount--;

		if (pDevEntry->Cfg80211ActionCount > 0)
			pDevEntry->Cfg80211RegisterActionFrame = TRUE;
		else
			pDevEntry->Cfg80211RegisterActionFrame = FALSE;

		DBGPRINT(RT_DEBUG_INFO,
			 ("[%d] TYPE pDevEntry->Cfg80211RegisterActionFrame=%d[%d]\n", isReg,
			  pDevEntry->Cfg80211RegisterActionFrame, pDevEntry->Cfg80211ActionCount));

		return;
	}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	/* IF Not Exist on VIF List, the device must be MAIN_DEV */
	if (isReg)
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount++;
	else
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount--;

	if (pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount > 0)
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterActionFrame = TRUE;
	else {
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterActionFrame = FALSE;
		pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount = 0;
	}

	DBGPRINT(RT_DEBUG_INFO, ("[%d] TYPE pAd->Cfg80211RegisterActionFrame=%d[%d]\n",
				 isReg, pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterActionFrame,
				 pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount));
}

VOID CFG80211DRV_OpsChangeBssParm(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_BSS_PARM *pBssInfo;
	BOOLEAN TxPreamble;

	CFG80211DBG(RT_DEBUG_TRACE, ("%s\n", __func__));

	pBssInfo = (CMD_RTPRIV_IOCTL_80211_BSS_PARM *) pData;

	/* Short Preamble */
	if (pBssInfo->use_short_preamble != -1) {
		CFG80211DBG(RT_DEBUG_TRACE,
			    ("%s: ShortPreamble %d\n", __func__, pBssInfo->use_short_preamble));
		pAd->CommonCfg.TxPreamble =
		    (pBssInfo->use_short_preamble ==
		     0 ? Rt802_11PreambleLong : Rt802_11PreambleShort);
		TxPreamble = (pAd->CommonCfg.TxPreamble == Rt802_11PreambleLong ? 0 : 1);
		MlmeSetTxPreamble(pAd, (USHORT) pAd->CommonCfg.TxPreamble);
	}

	/* CTS Protection */
	if (pBssInfo->use_cts_prot != -1) {
		CFG80211DBG(RT_DEBUG_TRACE,
			    ("%s: CTS Protection %d\n", __func__, pBssInfo->use_cts_prot));
	}

	/* Short Slot */
	if (pBssInfo->use_short_slot_time != -1) {
		CFG80211DBG(RT_DEBUG_TRACE,
			    ("%s: Short Slot %d\n", __func__, pBssInfo->use_short_slot_time));
	}
}

BOOLEAN CFG80211DRV_OpsSetChannel(RTMP_ADAPTER *pAd, VOID *pData)
{
	CMD_RTPRIV_IOCTL_80211_CHAN *pChan;
	UINT8 ChanId, IfType, ChannelType;
	BOOLEAN FlgIsChanged;
	UCHAR NewBW = BW_20;

#ifdef CONFIG_MULTI_CHANNEL
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	struct wifi_dev *wdev = &pMbss->wdev;
#endif /* CONFIG_MULTI_CHANNEL */

/*
 *  enum nl80211_channel_type {
 *	NL80211_CHAN_NO_HT,
 *	NL80211_CHAN_HT20,
 *	NL80211_CHAN_HT40MINUS,
 *	NL80211_CHAN_HT40PLUS
 *  };
 */
	/* init */
	pChan = (CMD_RTPRIV_IOCTL_80211_CHAN *) pData;
	ChanId = pChan->ChanId;
	IfType = pChan->IfType;
	ChannelType = pChan->ChanType;

	/* Preserve BW from supplicant */
	if (ChannelType == RT_CMD_80211_CHANTYPE_HT20 ||
	    ChannelType == RT_CMD_80211_CHANTYPE_NOHT)
		pAd->CommonCfg.preserve_bw = BW_20;
	else if (ChannelType == RT_CMD_80211_CHANTYPE_HT40MINUS ||
		 ChannelType == RT_CMD_80211_CHANTYPE_HT40PLUS)
		pAd->CommonCfg.preserve_bw = BW_40;
	else {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s: ChanType is unexpected, set BW_20 by default\n", __func__));
		pAd->CommonCfg.preserve_bw = BW_20;
	}

	if (IfType != RT_CMD_80211_IFTYPE_MONITOR) {
		/* get channel BW */
		FlgIsChanged = TRUE;

		/* Reset ChannelType based on Infra connection setting */
		if (INFRA_ON(pAd) && (IfType == RT_CMD_80211_IFTYPE_P2P_GO)) {
			if ((pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40)
			    && (ChannelType <= RT_CMD_80211_CHANTYPE_HT20)) {
				ChannelType =
				    pAd->CommonCfg.CentralChannel >
				    ChanId ? RT_CMD_80211_CHANTYPE_HT40PLUS :
				    RT_CMD_80211_CHANTYPE_HT40MINUS;
				DBGPRINT(RT_DEBUG_ERROR, ("Infra@HT40, Force P2P GO@HT40\n"));
			}

			if ((pAd->CommonCfg.BBPCurrentBW == BW_80)
			    && (ChannelType <= RT_CMD_80211_CHANTYPE_HT40PLUS)) {
				ChannelType = RT_CMD_80211_CHANTYPE_VHT80;
				DBGPRINT(RT_DEBUG_OFF, ("Infra@VHT80, Force P2P GO@VHT80\n"));
			}

			if ((pAd->CommonCfg.RegTransmitSetting.field.BW == BW_20)
			    && (ChannelType >= RT_CMD_80211_CHANTYPE_HT40MINUS)) {
				ChannelType = RT_CMD_80211_CHANTYPE_HT20;
				DBGPRINT(RT_DEBUG_ERROR, ("Infra@HT20, Force P2P GO@HT20\n"));
			}

			FlgIsChanged = FALSE;
		}
#ifdef CUSTOMIZED_BW_SETTING
		if (RTMP_GetCustomizedChannelBw(pAd, ChanId) == BW_20) {
			DBGPRINT(RT_DEBUG_ERROR, ("[%s] Force 20BW in 2.4G band\n", __func__));
			if (ChannelType >= RT_CMD_80211_CHANTYPE_HT40MINUS)
				ChannelType = RT_CMD_80211_CHANTYPE_HT20;
		}
#endif /* CUSTOMIZED_BW_SETTING */

		/* set to new channel BW */
		if (ChannelType == RT_CMD_80211_CHANTYPE_HT20) {
#ifdef CONFIG_MULTI_CHANNEL
			if (IfType == RT_CMD_80211_IFTYPE_AP ||
			    IfType == RT_CMD_80211_IFTYPE_P2P_GO) {
				wdev->bw = 0;
				wdev->CentralChannel = ChanId;
				wdev->channel = ChanId;
			}
#endif /* CONFIG_MULTI_CHANNEL */

			NewBW = BW_20;
			pAd->CommonCfg.RegTransmitSetting.field.BW = BW_20;
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_NONE;
			pAd->CommonCfg.HT_Disable = 0;
		} else if (ChannelType == RT_CMD_80211_CHANTYPE_HT40MINUS) {
			NewBW = BW_40;
			pAd->CommonCfg.RegTransmitSetting.field.BW = BW_40;
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_BELOW;
			pAd->CommonCfg.HT_Disable = 0;
#ifdef CONFIG_MULTI_CHANNEL
			if (IfType == RT_CMD_80211_IFTYPE_AP ||
			    IfType == RT_CMD_80211_IFTYPE_P2P_GO) {

				wdev->bw = pAd->CommonCfg.RegTransmitSetting.field.BW;
				pAd->CommonCfg.Channel = ChanId;

				if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_BELOW)
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel - 2;
				else if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA ==
					 EXTCHA_ABOVE)
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel + 2;
				else
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;

				wdev->CentralChannel = pAd->CommonCfg.CentralChannel;
				wdev->channel = pAd->CommonCfg.Channel;

			}
#endif /* CONFIG_MULTI_CHANNEL */

		} else if (ChannelType == RT_CMD_80211_CHANTYPE_HT40PLUS) {
			/* not support NL80211_CHAN_HT40MINUS or NL80211_CHAN_HT40PLUS */
			/* i.e. primary channel = 36, secondary channel must be 40 */
			NewBW = BW_40;
			pAd->CommonCfg.RegTransmitSetting.field.BW = BW_40;
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_ABOVE;
			pAd->CommonCfg.HT_Disable = 0;
#ifdef CONFIG_MULTI_CHANNEL
			if (IfType == RT_CMD_80211_IFTYPE_AP ||
			    IfType == RT_CMD_80211_IFTYPE_P2P_GO) {

				wdev->bw = pAd->CommonCfg.RegTransmitSetting.field.BW;
				pAd->CommonCfg.Channel = ChanId;

				if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_BELOW)
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel - 2;
				else if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA ==
					 EXTCHA_ABOVE)
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel + 2;
				else
					pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;

				wdev->CentralChannel = pAd->CommonCfg.CentralChannel;
				wdev->channel = pAd->CommonCfg.Channel;

			}
#endif /* CONFIG_MULTI_CHANNEL */

		}
#ifdef DOT11_VHT_AC
		else if (ChannelType == RT_CMD_80211_CHANTYPE_VHT80) {
			NewBW = BW_80;
			if (pChan->CenterChanId > pChan->ChanId)
				pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_ABOVE;
			else
				pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_BELOW;

			pAd->CommonCfg.HT_Disable = 0;
			pAd->CommonCfg.vht_bw = VHT_BW_80;
			pAd->StaActive.SupportedPhyInfo.bVhtEnable = TRUE;

#ifdef CONFIG_MULTI_CHANNEL
			if (IfType == RT_CMD_80211_IFTYPE_AP ||
			    IfType == RT_CMD_80211_IFTYPE_P2P_GO) {
				wdev->bw = BW_80;
				wdev->CentralChannel = pAd->CommonCfg.CentralChannel;
				wdev->channel = pAd->CommonCfg.Channel;
			}
#endif /* CONFIG_MULTI_CHANNEL */
		}
#endif /* DOT11_VHT_AC	 */
		else if (ChannelType == RT_CMD_80211_CHANTYPE_NOHT) {
#ifdef CONFIG_MULTI_CHANNEL

			if (IfType == RT_CMD_80211_IFTYPE_AP ||
			    IfType == RT_CMD_80211_IFTYPE_P2P_GO) {
				wdev->bw = 0;
				wdev->CentralChannel = ChanId;
				wdev->channel = ChanId;
			}
#endif /* CONFIG_MULTI_CHANNEL */
			NewBW = BW_20;
			pAd->CommonCfg.RegTransmitSetting.field.BW = BW_20;
			pAd->CommonCfg.RegTransmitSetting.field.EXTCHA = EXTCHA_NONE;
			pAd->CommonCfg.HT_Disable = 1;
		}

		CFG80211DBG(RT_DEBUG_TRACE,
			    ("80211> HT Disable = %d\n", pAd->CommonCfg.HT_Disable));
	} else {
		/* for monitor mode */
		FlgIsChanged = TRUE;
		pAd->CommonCfg.HT_Disable = 0;
		NewBW = BW_40;
		pAd->CommonCfg.RegTransmitSetting.field.BW = BW_40;
	}

	if (FlgIsChanged == TRUE) {
		SetCommonHT(pAd);

		/* switch to the channel with Common Channel */
		pAd->CommonCfg.Channel = ChanId;
		pAd->MlmeAux.Channel = ChanId;


		if (pChan->CenterChanId != 0) {
			pAd->CommonCfg.CentralChannel = pChan->CenterChanId;
		} else {
		if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_BELOW)
			pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel - 2;
		else if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel + 2;
		else
			pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
		}
		pAd->hw_cfg.cent_ch = pAd->CommonCfg.CentralChannel;
		bbp_set_bw(pAd, NewBW);
		AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
		AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
	}

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> New CH = %d, New BW = %d with Ext[%d]\n",
				     pAd->CommonCfg.CentralChannel,
				     NewBW,
				     pAd->CommonCfg.RegTransmitSetting.field.EXTCHA));

	if (IfType == RT_CMD_80211_IFTYPE_AP || IfType == RT_CMD_80211_IFTYPE_P2P_GO) {
#ifdef CONFIG_MULTI_CHANNEL
		pAd->MCC_GOConnect_Protect = TRUE;

#endif /* CONFIG_MULTI_CHANNEL */

#ifdef CONFIG_AP_SUPPORT
    /* If this wifi_dev is working for AP, save channel info to object */
		pAd->ApCfg.MBSSID[SECOND_MBSSID].wdev.channel = ChanId;
		DBGPRINT(RT_DEBUG_ERROR, ("%s - Save channel info (ch%d) in AP Mode\n",
				  __func__, ChanId));

		CFG80211DBG(RT_DEBUG_ERROR, ("80211> Set the channel in AP Mode\n"));
#endif /* CONFIG_AP_SUPPORT */

		return TRUE;
	}
#ifdef CONFIG_STA_SUPPORT
	if ((IfType == RT_CMD_80211_IFTYPE_STATION) && (FlgIsChanged == TRUE)) {
		/*
		   1. Station mode;
		   2. New BW settings is 20MHz but current BW is not 20MHz;
		   3. New BW settings is 40MHz but current BW is 20MHz;

		   Re-connect to the AP due to BW 20/40 or HT/non-HT change.
		 */
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> Set the channel in STA Mode\n"));
	}

	if (IfType == RT_CMD_80211_IFTYPE_ADHOC) {
		/* update IBSS beacon */
		MlmeUpdateTxRates(pAd, FALSE, 0);
		MakeIbssBeacon(pAd);
		AsicEnableIbssSync(pAd);

		Set_SSID_Proc(pAd, (PSTRING) pAd->CommonCfg.Ssid);
	}

	if (IfType == RT_CMD_80211_IFTYPE_MONITOR) {
		/* reset monitor mode in the new channel */
		Set_NetworkType_Proc(pAd, "Monitor");
		RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, pChan->MonFilterFlag);
	}
#endif /*CONFIG_STA_SUPPORT */
	return TRUE;
}

BOOLEAN CFG80211DRV_OpsLeave(VOID *pAdOrg, UINT8 IfType)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;

	MLME_DEAUTH_REQ_STRUCT DeAuthReq;
	MLME_QUEUE_ELEM *pMsgElem = NULL;

	pAd->StaCfg.bAutoReconnect = FALSE;
	pAd->cfg80211_ctrl.FlgCfg80211Connecting = FALSE;

	pAd->MlmeAux.AutoReconnectSsidLen = 32;
	NdisZeroMemory(pAd->MlmeAux.AutoReconnectSsid, pAd->MlmeAux.AutoReconnectSsidLen);

	os_alloc_mem(pAd, (UCHAR **) &pMsgElem, sizeof(MLME_QUEUE_ELEM));
	if (NULL == pMsgElem) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s():Error: alloc memory failed\n", __func__));
		return FALSE;
	}
	NdisZeroMemory(pMsgElem, sizeof(MLME_QUEUE_ELEM));

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	if (IfType == RT_CMD_80211_IFTYPE_P2P_CLIENT)
		COPY_MAC_ADDR(DeAuthReq.Addr, pAd->ApCfg.ApCliTab[MAIN_MBSSID].MlmeAux.Bssid);
	else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
		COPY_MAC_ADDR(DeAuthReq.Addr, pAd->CommonCfg.Bssid);
	DBGPRINT(RT_DEBUG_OFF, ("%s - Bssid=%02x:%02x:%02x:%02x:%02x:%02x\n",
				  __func__, PRINT_MAC(DeAuthReq.Addr)));
	DeAuthReq.Reason = REASON_DEAUTH_STA_LEAVING;
	pMsgElem->MsgLen = sizeof(MLME_DEAUTH_REQ_STRUCT);
	pMsgElem->Priv = MAIN_MBSSID;
	NdisMoveMemory(pMsgElem->Msg, &DeAuthReq, sizeof(MLME_DEAUTH_REQ_STRUCT));
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	if (IfType == RT_CMD_80211_IFTYPE_P2P_CLIENT)	{
		ApCliMlmeDeauthReqAction(pAd, pMsgElem);
		ApCliLinkDown(pAd, MAIN_MBSSID /*ifIndex */);
	} else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
	{
		MlmeDeauthReqAction(pAd, pMsgElem);
		LinkDown(pAd, FALSE);
#ifdef RT_CFG80211_SUPPORT
		pAd->SME = SME_IDLE;
#endif
	}

	os_free_mem(NULL, pMsgElem);
	pMsgElem = NULL;

#endif /* CONFIG_STA_SUPPORT */
	return TRUE;
}

BOOLEAN CFG80211DRV_StaGet(RTMP_ADAPTER *pAd, const u8 *mac, CMD_IOCTL_80211_STA_INFO *sta)
{
#ifdef CONFIG_AP_SUPPORT
	{
		MAC_TABLE_ENTRY *pEntry;
		UINT32 DataRate = 0;
		UINT32 RSSI;

		pEntry = MacTableLookup(pAd, mac);
		if (pEntry == NULL)
			return FALSE;

		/* fill tx rate */
		/* getRate(pEntry->HTPhyMode, &DataRate); */
		RtmpDrvRateGet(pAd, pEntry->HTPhyMode.field.MODE, pEntry->HTPhyMode.field.ShortGI,
			       pEntry->HTPhyMode.field.BW, pEntry->HTPhyMode.field.MCS,
			       newRateGetAntenna(pEntry->MaxHTPhyMode.field.MCS), &DataRate);
		DataRate /= 500000;
		DataRate /= 2;

		if ((pEntry->HTPhyMode.field.MODE == MODE_HTMIX) ||
		    (pEntry->HTPhyMode.field.MODE == MODE_HTGREENFIELD)) {
			if (pEntry->HTPhyMode.field.BW)
				sta->TxRateFlags |= RT_CMD_80211_TXRATE_BW_40;

			if (pEntry->HTPhyMode.field.ShortGI)
				sta->TxRateFlags |= RT_CMD_80211_TXRATE_SHORT_GI;

			sta->TxRateMCS = pEntry->HTPhyMode.field.MCS;
		} else {
			sta->TxRateFlags = RT_CMD_80211_TXRATE_LEGACY;
			sta->TxRateMCS = DataRate * 10;	/* unit: 100kbps */
		}

		/* fill signal */
		RSSI = RTMPAvgRssi(pAd, &pEntry->RssiSample);
		sta->Signal = RSSI;

		/* fill tx count */
		sta->TxPacketCnt = pEntry->OneSecTxNoRetryOkCount +
		    pEntry->OneSecTxRetryOkCount + pEntry->OneSecTxFailCount;

		/* fill inactive time */
		sta->InactiveTime = pEntry->NoDataIdleCount * 1000;	/* unit: ms */
		sta->InactiveTime *= MLME_TASK_EXEC_MULTIPLE;
		sta->InactiveTime /= 20;
	}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
	{
		HTTRANSMIT_SETTING PhyInfo;
		UINT32 DataRate = 0;
		UINT32 RSSI;

		/* fill tx rate */
		if ((!WMODE_CAP_N(pAd->CommonCfg.PhyMode)) ||
		    (pAd->MacTab.Content[BSSID_WCID].HTPhyMode.field.MODE <= MODE_OFDM)) {
			PhyInfo.word = pAd->StaCfg.wdev.HTPhyMode.word;
		} else
			PhyInfo.word = pAd->MacTab.Content[BSSID_WCID].HTPhyMode.word;

		DrvRateGet(PhyInfo.word, &DataRate);
		DataRate /= 500000;
		DataRate /= 2;

		if ((PhyInfo.field.MODE == MODE_HTMIX) || (PhyInfo.field.MODE == MODE_HTGREENFIELD)) {
			if (PhyInfo.field.BW)
				sta->TxRateFlags |= RT_CMD_80211_TXRATE_BW_40;

			if (PhyInfo.field.ShortGI)
				sta->TxRateFlags |= RT_CMD_80211_TXRATE_SHORT_GI;

			sta->TxRateMCS = PhyInfo.field.MCS;
		} else {
			/* cfg80211's rato_info structure and rate_info_flags can't support 11ac,
			we use legacy way to describe the actually vht rate as well*/
			sta->TxRateFlags = RT_CMD_80211_TXRATE_LEGACY;
			sta->TxRateMCS = DataRate * 10;	/* unit: 100kbps */
		}

		/* fill tx/rx packet count */
		sta->tx_packets = pAd->WlanCounters.TransmittedFragmentCount.u.LowPart;
		sta->tx_retries = pAd->WlanCounters.RetryCount.u.LowPart;
		sta->tx_failed = pAd->WlanCounters.FailedCount.u.LowPart;
		sta->rx_packets = pAd->WlanCounters.ReceivedFragmentCount.QuadPart;

		/* fill signal */
		RSSI = RTMPAvgRssi(pAd, &pAd->StaCfg.RssiSample);
		sta->Signal = RSSI;
	}
#endif /* CONFIG_STA_SUPPORT */

	return TRUE;
}

BOOLEAN CFG80211DRV_StaKeyAdd(VOID *pAdOrg, VOID *pData)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_KEY *pKeyInfo;
	RT_CMD_STA_IOCTL_SECURITY IoctlSec;

#ifdef CONFIG_MULTI_CHANNEL
	struct wifi_dev *wdev = &pAd->StaCfg.wdev;
	MAC_TABLE_ENTRY *pMacEntry = NULL;
	PAPCLI_STRUCT pApCliEntry = pApCliEntry = &pAd->ApCfg.ApCliTab[MAIN_MBSSID];
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	struct wifi_dev *p2p_wdev = NULL;
#endif /* CONFIG_MULTI_CHANNEL */
#ifdef CFG_TDLS_SUPPORT
	int i;
	PCFG_TDLS_STRUCT pCfgTdls;
	UCHAR nullAddr[MAC_ADDR_LEN];
#endif /* CFG_TDLS_SUPPORT */

	pKeyInfo = (CMD_RTPRIV_IOCTL_80211_KEY *) pData;

#ifdef CFG_TDLS_SUPPORT
	DBGPRINT(RT_DEBUG_WARN,
		 ("[%s] key.MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", __func__,
		  PRINT_MAC(pKeyInfo->MAC)));
	NdisZeroMemory(nullAddr, MAC_ADDR_LEN);
	pCfgTdls = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info;

	for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++) {
		if (!MAC_ADDR_EQUAL(pCfgTdls->TDLSEntry[i].MacAddr, nullAddr)
		    && MAC_ADDR_EQUAL(pKeyInfo->MAC, pCfgTdls->TDLSEntry[i].MacAddr)) {
			NdisMoveMemory(&(pCfgTdls->TDLSEntry[i].TPK[16]), pKeyInfo->KeyBuf, LEN_TK);
			DBGPRINT(RT_DEBUG_TRACE, ("[%s] ===> TDLS_COPY_KEY\n", __func__));
			return TRUE;
		}
	}
#endif /* CFG_TDLS_SUPPORT */
#ifdef DOT11W_PMF_SUPPORT
	if (pKeyInfo->KeyType == RT_CMD_80211_KEY_AES_CMAC) {
		PPMF_CFG pPmfCfg = &pAd->StaCfg.PmfCfg;

		hex_dump("PMF IGTK pKeyInfo->KeyBuf=", (UINT8 *) pKeyInfo->KeyBuf,
			 pKeyInfo->KeyLen);
		DBGPRINT(RT_DEBUG_ERROR, ("PMF IGTK pKeyInfo->KeyId=%d\n", pKeyInfo->KeyId));

		/* only 4 or 5, no other case! */
		if (pKeyInfo->KeyId == 4 || pKeyInfo->KeyId == 5) {
			/* no PN is passed, PN is useless in PMF_CalculateBIPMIC() */
			NdisZeroMemory(&pPmfCfg->IPN[pKeyInfo->KeyId - 4][0], LEN_WPA_TSC);
			NdisMoveMemory(&pPmfCfg->IGTK[pKeyInfo->KeyId - 4][0], pKeyInfo->KeyBuf,
				       pKeyInfo->KeyLen);
		} else {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("ERROR !! pKeyInfo->KeyId=%d\n", pKeyInfo->KeyId));

		}

	} else
#endif /* DOT11W_PMF_SUPPORT */
	if (pKeyInfo->KeyType == RT_CMD_80211_KEY_WEP40
		    || pKeyInfo->KeyType == RT_CMD_80211_KEY_WEP104) {
		DBGPRINT(RT_DEBUG_TRACE, ("RT_CMD_80211_KEY_WEP\n"));
	} else {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Set_WPAPSK_Proc ==> %d, %d, %zu...\n", pKeyInfo->KeyId,
			  pKeyInfo->KeyType, strlen(pKeyInfo->KeyBuf)));

		IoctlSec.KeyIdx = pKeyInfo->KeyId;
		IoctlSec.pData = pKeyInfo->KeyBuf;
		IoctlSec.length = pKeyInfo->KeyLen;
		IoctlSec.Alg = 0;

		/* YF@20120327: Due to WepStatus will be set in the cfg connect function. */
		if (pAd->StaCfg.wdev.WepStatus == Ndis802_11Encryption2Enabled)
			IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_TKIP;
		else if (pAd->StaCfg.wdev.WepStatus == Ndis802_11Encryption3Enabled)
			IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_CCMP;
		IoctlSec.flags = RT_CMD_STA_IOCTL_SECURITY_ENABLED;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)
		if (pKeyInfo->bPairwise == FALSE)
#else
		if (pKeyInfo->KeyId > 0)
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37) */
			{
			if (pAd->StaCfg.GroupCipher == Ndis802_11Encryption2Enabled)
				IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_TKIP;
			else if (pAd->StaCfg.GroupCipher == Ndis802_11Encryption3Enabled)
				IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_CCMP;

			DBGPRINT(RT_DEBUG_TRACE, ("Install GTK: %d\n", IoctlSec.Alg));
			IoctlSec.ext_flags = RT_CMD_STA_IOCTL_SECURTIY_EXT_GROUP_KEY;
		} else {
			if (pAd->StaCfg.PairCipher == Ndis802_11Encryption2Enabled)
				IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_TKIP;
			else if (pAd->StaCfg.PairCipher == Ndis802_11Encryption3Enabled)
				IoctlSec.Alg = RT_CMD_STA_IOCTL_SECURITY_ALG_CCMP;

			DBGPRINT(RT_DEBUG_TRACE, ("Install PTK: %d\n", IoctlSec.Alg));
			IoctlSec.ext_flags = RT_CMD_STA_IOCTL_SECURTIY_EXT_SET_TX_KEY;
		}

		/*Set_GroupKey_Proc(pAd, &IoctlSec) */
		RTMP_STA_IoctlHandle(pAd, NULL, CMD_RTPRIV_IOCTL_STA_SIOCSIWENCODEEXT, 0,
				     &IoctlSec, 0, INT_MAIN);

#ifdef CONFIG_MULTI_CHANNEL
		/* only ptk to avoid group key rekey cases */
		if (IoctlSec.ext_flags == RT_CMD_STA_IOCTL_SECURTIY_EXT_SET_TX_KEY) {
			if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {

				p2p_wdev = &pMbss->wdev;

				DBGPRINT(RT_DEBUG_TRACE,
					 ("p2p_wdev->channel %d pAd->CommonCfg.Channel %d\n",
					  p2p_wdev->channel, pAd->CommonCfg.Channel));

				if ((wdev->bw != p2p_wdev->bw)
				    && ((wdev->channel == p2p_wdev->channel))) {
					DBGPRINT(RT_DEBUG_TRACE, ("start bw !=  && P2P GO SCC\n"));
					pAd->Mlme.bStartScc = TRUE;
				} else if ((((wdev->bw == p2p_wdev->bw)
					     && (wdev->channel != p2p_wdev->channel))
					    || !((wdev->bw == p2p_wdev->bw)
						 && ((wdev->channel == p2p_wdev->channel))))) {

					LONG timeDiff;
					INT starttime = pAd->Mlme.channel_1st_staytime;

					NdisGetSystemUpTime(&pAd->Mlme.BeaconNow32);
					timeDiff =
					    (pAd->Mlme.BeaconNow32 -
					     pAd->StaCfg.LastBeaconRxTime) %
					    (pAd->CommonCfg.BeaconPeriod);
					DBGPRINT(RT_DEBUG_TRACE,
						 ("Mlme.Now32 %ld, LastBeaconRxTime %ld\n",
						  pAd->Mlme.BeaconNow32,
						  pAd->StaCfg.LastBeaconRxTime));
					DBGPRINT(RT_DEBUG_TRACE,
						 ("####    timeDiff %ld\n", timeDiff));
					AsicDisableSync(pAd);
					if (starttime > timeDiff) {
						OS_WAIT((starttime - timeDiff));
					} else {
						OS_WAIT((starttime +
							 (pAd->CommonCfg.BeaconPeriod - timeDiff)));
					}
					AsicEnableApBssSync(pAd);

					Start_MCC(pAd);
					pAd->MCC_DHCP_Protect = TRUE;
				}

			} else if (RTMP_CFG80211_VIF_P2P_CLI_ON(pAd)) {	/* check GC is key done , then trigger MCC */
				pMacEntry = &pAd->MacTab.Content[pApCliEntry->MacTabWCID];
				p2p_wdev = &(pApCliEntry->wdev);

				DBGPRINT(RT_DEBUG_TRACE,
					 ("p2p_wdev->channel %d pAd->CommonCfg.Channel %d\n",
					  p2p_wdev->channel, pAd->CommonCfg.Channel));

				if (pMacEntry) {
					/* P2P GC will have security */
					if (IS_ENTRY_APCLI(pMacEntry) &&
						(pMacEntry->PairwiseKey.KeyLen == LEN_TK)) {

						if ((wdev->bw != p2p_wdev->bw)
						    && ((wdev->channel == p2p_wdev->channel))) {
							DBGPRINT(RT_DEBUG_TRACE,
								 ("start bw !=  && P2P GC SCC\n"));
							pAd->Mlme.bStartScc = TRUE;
						} else if ((((wdev->bw == p2p_wdev->bw)
							     && (wdev->channel !=
								 p2p_wdev->channel))
							    || !((wdev->bw == p2p_wdev->bw)
								 &&
								 ((wdev->channel ==
								   p2p_wdev->channel))))) {
							Start_MCC(pAd);
							pAd->MCC_DHCP_Protect = TRUE;
						}

					}
				}
			}
		}
#endif /* CONFIG_MUTIL_CHANNEL */
	}
#endif /* CONFIG_STA_SUPPORT */

	return TRUE;
}

BOOLEAN CFG80211DRV_Connect(VOID *pAdOrg, VOID *pData)
{
#ifdef CONFIG_STA_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_CONNECT *pConnInfo;
	UCHAR SSID[NDIS_802_11_LENGTH_SSID + 1];	/* Add One for SSID_Len == 32 */
	UINT32 SSIDLen;
	RT_CMD_STA_IOCTL_SECURITY_ADV IoctlWpa;

#ifdef CONFIG_MULTI_CHANNEL
	ULONG Value;
	ULONG TimeTillTbtt;
	ULONG temp;
#endif /* CONFIG_MULTI_CHANNEL */

	if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_INFRA_ON) &&
	    OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG80211: Connected, disconnect first !\n"));
	} else {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG80211: No Connection\n"));
	}

#ifdef RT_CFG80211_SUPPORT
	/*Update SME status*/
	pAd->SME = SME_CONNECTING;
#endif

#ifdef CONFIG_MULTI_CHANNEL
/* (when go on , we need to protect  infra connect for 1.2s.) */
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {
		/* update noa */
		RTMP_IO_READ32(pAd, TBTT_TIMER, &TimeTillTbtt);
		TimeTillTbtt = TimeTillTbtt & 0x1ffff;
		DBGPRINT(RT_DEBUG_TRACE, ("   .!!!!TimeTillTbtt =  %ld\n", TimeTillTbtt));

		RTMP_IO_READ32(pAd, TSF_TIMER_DW0, &Value);
		DBGPRINT(RT_DEBUG_TRACE, ("   .!!!!Current Tsf LSB = = %ld\n", Value));
		temp = TimeTillTbtt * 64 + Value;
		DBGPRINT(RT_DEBUG_TRACE, ("   .!!!!Tsf LSB + TimeTillTbtt= %ld\n", temp));
		/* Wait five beacon 0x7d00 for 5 beacon interval.  0x6400 is set to 25%*beacon interval */
		pAd->GONoASchedule.StartTime = Value + TimeTillTbtt * 64 + 409600 + 3200;
/* pAd->GONoASchedule.StartTime = Value + TimeTillTbtt*64 + 512000 + 3200; */

		DBGPRINT(RT_DEBUG_TRACE,
			 (" pAd->GONoASchedule.StartTime = %ld\n", pAd->GONoASchedule.StartTime));
		pAd->GONoASchedule.Count = 20;	/*wait 4 beacon + (interval * 4) */
/* pAd->GONoASchedule.Duration= 409600; */
/* pAd->GONoASchedule.Duration= (409600 + 3200); */
		pAd->GONoASchedule.Duration = 1228800;

/* pAd->GONoASchedule.Interval=  614400; */
/* pAd->GONoASchedule.Interval=  (819200 + 3200); */
		pAd->GONoASchedule.Interval = 1638400;
/* DBGPRINT(RT_DEBUG_TRACE,("iversomcc delay 400!!!!\n")); */
		OS_WAIT(400);

/* pAd->MCC_Connect_Protect = TRUE; */
/* pAd->MCC_Connect_Count = 0; */

	}
/*  */
#endif /* CONFIG_MULTI_CHANNEL */

	pConnInfo = (CMD_RTPRIV_IOCTL_80211_CONNECT *) pData;

	/* change to infrastructure mode if we are in ADHOC mode */
	Set_NetworkType_Proc(pAd, "Infra");

	SSIDLen = pConnInfo->SsidLen;
	if (SSIDLen > NDIS_802_11_LENGTH_SSID)
		SSIDLen = NDIS_802_11_LENGTH_SSID;
	else if (SSIDLen == 0)
		return false;

	memcpy(SSID, pConnInfo->pSsid, SSIDLen);
	SSID[SSIDLen] = '\0';

	/* set authentication mode */
	if (pConnInfo->WpaVer == 2) {
		if (pConnInfo->FlgIs8021x == TRUE) {
			DBGPRINT(RT_DEBUG_TRACE, ("WPA2\n"));
			Set_AuthMode_Proc(pAd, "WPA2");
		} else {
			DBGPRINT(RT_DEBUG_TRACE, ("WPA2PSK\n"));
			Set_AuthMode_Proc(pAd, "WPA2PSK");
		}
	} else if (pConnInfo->WpaVer == 1) {
		if (pConnInfo->FlgIs8021x == TRUE) {
			DBGPRINT(RT_DEBUG_TRACE, ("WPA\n"));
			Set_AuthMode_Proc(pAd, "WPA");
		} else {
			DBGPRINT(RT_DEBUG_TRACE, ("WPAPSK\n"));
			Set_AuthMode_Proc(pAd, "WPAPSK");
		}
	} else if (pConnInfo->AuthType == Ndis802_11AuthModeAutoSwitch)
		Set_AuthMode_Proc(pAd, "WEPAUTO");
	else if (pConnInfo->AuthType == Ndis802_11AuthModeShared)
		Set_AuthMode_Proc(pAd, "SHARED");
	else
		Set_AuthMode_Proc(pAd, "OPEN");

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> AuthMode = %d\n", pAd->StaCfg.wdev.AuthMode));

	/* set encryption mode */
	if (pConnInfo->PairwiseEncrypType & RT_CMD_80211_CONN_ENCRYPT_CCMP) {
		DBGPRINT(RT_DEBUG_TRACE, ("AES\n"));
		Set_EncrypType_Proc(pAd, "AES");
	} else if (pConnInfo->PairwiseEncrypType & RT_CMD_80211_CONN_ENCRYPT_TKIP) {
		DBGPRINT(RT_DEBUG_TRACE, ("TKIP\n"));
		Set_EncrypType_Proc(pAd, "TKIP");
	} else if (pConnInfo->PairwiseEncrypType & RT_CMD_80211_CONN_ENCRYPT_WEP) {
		DBGPRINT(RT_DEBUG_TRACE, ("WEP\n"));
		Set_EncrypType_Proc(pAd, "WEP");
	} else {
		DBGPRINT(RT_DEBUG_TRACE, ("NONE\n"));
		Set_EncrypType_Proc(pAd, "NONE");
	}

	/* Groupwise Key Information Setting */
	IoctlWpa.flags = RT_CMD_STA_IOCTL_WPA_GROUP;
	if (pConnInfo->GroupwiseEncrypType & RT_CMD_80211_CONN_ENCRYPT_CCMP) {
		DBGPRINT(RT_DEBUG_TRACE, ("GTK AES\n"));
		IoctlWpa.value = RT_CMD_STA_IOCTL_WPA_GROUP_CCMP;
		RtmpIoctl_rt_ioctl_siwauth(pAd, &IoctlWpa, 0);
	} else if (pConnInfo->GroupwiseEncrypType & RT_CMD_80211_CONN_ENCRYPT_TKIP) {
		DBGPRINT(RT_DEBUG_TRACE, ("GTK TKIP\n"));
		IoctlWpa.value = RT_CMD_STA_IOCTL_WPA_GROUP_TKIP;
		RtmpIoctl_rt_ioctl_siwauth(pAd, &IoctlWpa, 0);
	}

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> EncrypType = %d\n", pAd->StaCfg.wdev.WepStatus));

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> Key = %s\n", pConnInfo->pKey));

	/* set channel: STATION will auto-scan */

	/* set WEP key */
	if (pConnInfo->pKey &&
	    ((pConnInfo->GroupwiseEncrypType | pConnInfo->PairwiseEncrypType) &
	     RT_CMD_80211_CONN_ENCRYPT_WEP)) {
		UCHAR KeyBuf[50];

		/* reset AuthMode and EncrypType */
		Set_EncrypType_Proc(pAd, "WEP");

		/* reset key */
#ifdef RT_CFG80211_DEBUG
		hex_dump("KeyBuf=", (UINT8 *) pConnInfo->pKey, pConnInfo->KeyLen);
#endif /* RT_CFG80211_DEBUG */

		pAd->StaCfg.wdev.DefaultKeyId = pConnInfo->KeyIdx;	/* base 0 */
		if (pConnInfo->KeyLen >= sizeof(KeyBuf))
			return FALSE;

		memcpy(KeyBuf, pConnInfo->pKey, pConnInfo->KeyLen);
		KeyBuf[pConnInfo->KeyLen] = 0x00;

		CFG80211DBG(RT_DEBUG_ERROR,
			    ("80211> pAd->StaCfg.DefaultKeyId = %d\n",
			     pAd->StaCfg.wdev.DefaultKeyId));

		Set_Wep_Key_Proc(pAd, (PSTRING) KeyBuf, (INT) pConnInfo->KeyLen,
				 (INT) pConnInfo->KeyIdx);

	}

	/* End of if */
	/* TODO: We need to provide a command to set BSSID to associate a AP */
	pAd->cfg80211_ctrl.FlgCfg80211Connecting = TRUE;
	/* Record the requested BSSID */
	if (pConnInfo->pBssid != NULL)
		NdisMoveMemory(pAd->cfg80211_ctrl.Cfg80211_ConnReqBssid, pConnInfo->pBssid,
			       MAC_ADDR_LEN);
	else
		NdisZeroMemory(pAd->cfg80211_ctrl.Cfg80211_ConnReqBssid, MAC_ADDR_LEN);

#ifdef DOT11W_PMF_SUPPORT
	if (pConnInfo->mfp) {
		/* Set_PMFSHA256_Proc(pAd,"1"); */
		Set_PMFMFPC_Proc(pAd, "1");
	}
#endif /* DOT11W_PMF_SUPPORT */

	Set_SSID_Proc(pAd, (PSTRING) SSID);
	CFG80211DBG(RT_DEBUG_TRACE, ("80211> Connecting SSID = %s\n", SSID));
#endif /* CONFIG_STA_SUPPORT */

	return TRUE;
}

VOID CFG80211DRV_RegNotify(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_REG_NOTIFY *pRegInfo;

	pRegInfo = (CMD_RTPRIV_IOCTL_80211_REG_NOTIFY *) pData;

	/* keep Alpha2 and we can re-call the function when interface is up */
	pAd->cfg80211_ctrl.Cfg80211_Alpha2[0] = pRegInfo->Alpha2[0];
	pAd->cfg80211_ctrl.Cfg80211_Alpha2[1] = pRegInfo->Alpha2[1];

	/* apply the new regulatory rule */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP)) {
		/* interface is up */
		CFG80211_RegRuleApply(pAd, pRegInfo->pWiphy, (UCHAR *) pRegInfo->Alpha2);
	} else {
		CFG80211DBG(RT_DEBUG_ERROR, ("crda> interface is down!\n"));
	}
}

VOID CFG80211DRV_SurveyGet(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_SURVEY *pSurveyInfo;

	pSurveyInfo = (CMD_RTPRIV_IOCTL_80211_SURVEY *) pData;

	pSurveyInfo->pCfg80211 = pAd->pCfg80211_CB;

#ifdef AP_QLOAD_SUPPORT
	pSurveyInfo->ChannelTimeBusy = pAd->phy_ctrl.QloadLatestChannelBusyTimePri;
	pSurveyInfo->ChannelTimeExtBusy = pAd->phy_ctrl.QloadLatestChannelBusyTimeSec;
#endif /* AP_QLOAD_SUPPORT */
}

VOID CFG80211_UnRegister(IN VOID *pAdOrg, IN VOID *pNetDev)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

	/* sanity check */
	if (pAd->pCfg80211_CB == NULL)
		return;

	CFG80211OS_UnRegister(pAd->pCfg80211_CB, pNetDev);
	RTMP_DRIVER_80211_SCAN_STATUS_LOCK_INIT(pAd, FALSE);
	unregister_netdevice_notifier(&cfg80211_netdev_notifier);

	/* Reset CFG80211 Global Setting Here */
	DBGPRINT(RT_DEBUG_TRACE,
		 ("==========> TYPE Reset CFG80211 Global Setting Here <==========\n"));
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterActionFrame =
	    FALSE, pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount = 0;

	pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame = FALSE;
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount = 0;

	pAd->pCfg80211_CB = NULL;
	pAd->CommonCfg.HT_Disable = 0;

	/* It should be free when ScanEnd,
	   But Hit close the device in Scanning */
	if (pCfg80211_ctrl->pCfg80211ChanList != NULL) {
		os_free_mem(NULL, pCfg80211_ctrl->pCfg80211ChanList);
		pCfg80211_ctrl->pCfg80211ChanList = NULL;
	}

	pCfg80211_ctrl->Cfg80211ChanListLen = 0;
	pCfg80211_ctrl->Cfg80211CurChanIndex = 0;

	if (pCfg80211_ctrl->pExtraIe) {
		os_free_mem(NULL, pCfg80211_ctrl->pExtraIe);
		pCfg80211_ctrl->pExtraIe = NULL;
	}
	pCfg80211_ctrl->ExtraIeLen = 0;

	if (pCfg80211_ctrl->pAssocRspIe) {
		os_free_mem(NULL, pCfg80211_ctrl->pAssocRspIe);
		pCfg80211_ctrl->pAssocRspIe = NULL;
	}
	pCfg80211_ctrl->assocRspIeLen = 0;

/*
CFG_TODO
     if (pAd->pTxStatusBuf != NULL)
     {
	 os_free_mem(NULL, pAd->pTxStatusBuf);
	 pAd->pTxStatusBuf = NULL;
     }
	 pAd->TxStatusBufLen = 0;
*/
#ifdef CONFIG_AP_SUPPORT
#endif /* CONFIG_AP_SUPPORT */

}

/*
========================================================================
Routine Description:
	Parse and handle country region in beacon from associated AP.

Arguments:
	pAdCB			- WLAN control block pointer
	pVIE			- Beacon elements
	LenVIE			- Total length of Beacon elements

Return Value:
	NONE

Note:
========================================================================
*/
VOID CFG80211_BeaconCountryRegionParse(IN VOID *pAdCB,
				       IN NDIS_802_11_VARIABLE_IEs * pVIE, IN UINT16 LenVIE)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	UCHAR *pElement = (UCHAR *) pVIE;
	UINT32 LenEmt;

	while (LenVIE > 0) {
		pVIE = (NDIS_802_11_VARIABLE_IEs *) pElement;

		if (pVIE->ElementID == IE_COUNTRY) {
			/* send command to do regulation hint only when associated */
			RT_CFG80211_CRDA_REG_HINT11D(pAd, pVIE->data, pVIE->Length);
			/* RTEnqueueInternalCmd(pAd, CMDTHREAD_REG_HINT_11D, */
			/* pVIE->data, pVIE->Length); */
			break;
		}

		LenEmt = pVIE->Length + 2;

		if (LenVIE <= LenEmt)
			break;	/* length is not enough */

		pElement += LenEmt;
		LenVIE -= LenEmt;
	}
}				/* End of CFG80211_BeaconCountryRegionParse */

/*
========================================================================
Routine Description:
	Re-Initialize wireless channel/PHY in 2.4GHZ and 5GHZ.

Arguments:
	pAdCB			- WLAN control block pointer

Return Value:
	NONE

Note:
	CFG80211_SupBandInit() is called in xx_probe().
========================================================================
*/
#ifdef CONFIG_STA_SUPPORT
VOID CFG80211_LostApInform(IN VOID *pAdCB)
{

	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 11, 0)
	CFG80211_CB *p80211CB = pAd->pCfg80211_CB;
#endif /* endif */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	DBGPRINT(RT_DEBUG_TRACE, ("80211> CFG80211_LostApInform ==> %d\n",
					  pAd->cfg80211_ctrl.FlgCfg80211Connecting));
#else
	DBGPRINT(RT_DEBUG_TRACE, ("80211> CFG80211_LostApInform ==> %d\n",
				  p80211CB->pCfg80211_Wdev->sme_state));
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)) */

	pAd->StaCfg.bAutoReconnect = FALSE;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	/* sme_state is removed. need maintain internal state in driver */
	/* [tmp] FlgCfg80211Connecting = TRUE imply CONNECTING? */
	if (pAd->cfg80211_ctrl.FlgCfg80211Connecting)
#else
	if (p80211CB->pCfg80211_Wdev->sme_state == CFG80211_SME_CONNECTING)
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3,12,0)) */
		{
		CFG80211DBG(RT_DEBUG_TRACE,
	("%s: Sending connect result (fail) to kernel (bssid=%02x:%02x:%02x:%02x:%02x:%02x)\n",
					     __func__, pAd->CommonCfg.Bssid[0],
					     pAd->CommonCfg.Bssid[1], pAd->CommonCfg.Bssid[2],
					     pAd->CommonCfg.Bssid[3], pAd->CommonCfg.Bssid[4],
					     pAd->CommonCfg.Bssid[5]));

		cfg80211_connect_result(pAd->net_dev, pAd->CommonCfg.Bssid, NULL, 0, NULL, 0,
					WLAN_STATUS_UNSPECIFIED_FAILURE, GFP_KERNEL);
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 11, 0)
	else
#else
	else if (p80211CB->pCfg80211_Wdev->sme_state == CFG80211_SME_CONNECTED)
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3,12,0)) */
		{
#if defined(CONFIG_DFS_SLAVE) && defined(RT_CFG80211_P2P_SUPPORT) && defined(CONFIG_P2PGO_DFS_SLAVE)
		if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd) &&
		    pAd->StaCfg.bChannelSwitchCountingDown &&
		    pAd->CommonCfg.ChannelSwitchCount == 0) {
			cfg80211_disconnected(pAd->net_dev, 0,
					      &(pAd->CommonCfg.NewChannel), 1, GFP_KERNEL);
			pAd->StaCfg.bChannelSwitchCountingDown = FALSE;
		} else
#endif /* endif */
			cfg80211_disconnected(pAd->net_dev, 0, NULL, 0,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 2, 0))
						FALSE,/* locally_generated */
#endif
						GFP_KERNEL);
	}
}
#endif /*CONFIG_STA_SUPPORT */

/*
========================================================================
Routine Description:
	Hint to the wireless core a regulatory domain from driver.

Arguments:
	pAd				- WLAN control block pointer
	pCountryIe		- pointer to the country IE
	CountryIeLen	- length of the country IE

Return Value:
	NONE

Note:
	Must call the function in kernel thread.
========================================================================
*/
VOID CFG80211_RegHint(IN VOID *pAdCB, IN UCHAR *pCountryIe, IN ULONG CountryIeLen)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;

	CFG80211OS_RegHint(CFG80211CB, pCountryIe, CountryIeLen);
}

/*
========================================================================
Routine Description:
	Hint to the wireless core a regulatory domain from country element.

Arguments:
	pAdCB			- WLAN control block pointer
	pCountryIe		- pointer to the country IE
	CountryIeLen	- length of the country IE

Return Value:
	NONE

Note:
	Must call the function in kernel thread.
========================================================================
*/
VOID CFG80211_RegHint11D(IN VOID *pAdCB, IN UCHAR *pCountryIe, IN ULONG CountryIeLen)
{
	/* no regulatory_hint_11d() in 2.6.32 */
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;

	CFG80211OS_RegHint11D(CFG80211CB, pCountryIe, CountryIeLen);
}

#ifdef JUNGLE_SUPPORT
NDIS_STATUS CFG80211_StoreChList(PRTMP_ADAPTER pAd, struct ieee80211_supported_band *band2g,
				struct ieee80211_supported_band *band5g)
{
	UINT32 band_id;
	UINT32 num_ch, ch_id, RecId, DfsType;

	CFG80211DBG(RT_DEBUG_TRACE, ("crda> set by core sync channel list, %s\n", __func__));
	DfsType = CE;
	RecId = 0;

	NdisZeroMemory(pAd->ChannelList, MAX_NUM_OF_CHANNELS * sizeof(CHANNEL_TX_POWER));
	for (band_id = 0; band_id < 2; band_id++) {
		struct ieee80211_supported_band *band_tmp = NULL;
		struct ieee80211_channel *channels = NULL;

		if (band_id == 0) {
			CFG80211DBG(RT_DEBUG_TRACE, ("crda> reset chan/power for 2.4GHz\n"));
			band_tmp = band2g;
		} else {
			CFG80211DBG(RT_DEBUG_TRACE, ("crda> reset chan/power for 5GHz\n"));
			band_tmp = band5g;
		}

		if (!band_tmp)
			goto fail4;
		num_ch = band_tmp->n_channels;
		channels = band_tmp->channels;

		if (!channels)
			goto fail5;

		for (ch_id = 0; ch_id < num_ch; ch_id++) {
			struct ieee80211_channel ch = channels[ch_id];
			UINT32 chidx = 0;

			chidx = ieee80211_frequency_to_channel(ch.center_freq);
			if (ch.flags & IEEE80211_CHAN_DISABLED) {
				CFG80211DBG(RT_DEBUG_TRACE,
					("Chan %u (frq %d):\t not allowed, flags:%x\n"
					, chidx, ch.center_freq, ch.flags));
				continue;
			}
			CFG80211DBG(RT_DEBUG_TRACE, ("channel %d,\t", chidx));
			CFG80211DBG(RT_DEBUG_TRACE, ("flag(%x): ", ch.flags));
			if (ch.flags & CHANNEL_PASSIVE_SCAN)
				CFG80211DBG(RT_DEBUG_TRACE, ("%s ", "PASSIVE_SCAN"));
			if (ch.flags & CHANNEL_RADAR)
				CFG80211DBG(RT_DEBUG_TRACE, ("%s ", "RADAR"));
			if (!(ch.flags & IEEE80211_CHAN_NO_HT40PLUS)
				|| !(ch.flags & IEEE80211_CHAN_NO_HT40MINUS))
				CFG80211DBG(RT_DEBUG_TRACE, ("%s ", "CHANNEL_40M_CAP"));
			if (!(ch.flags & IEEE80211_CHAN_NO_80MHZ))
				CFG80211DBG(RT_DEBUG_TRACE, ("%s\n", "CHANNEL_80M_CAP"));
			/* keep channel number */
			pAd->ChannelList[RecId].Channel = chidx;
			/* keep maximum tranmission power */
			pAd->ChannelList[RecId].MaxTxPwr = ch.max_power;
			/* keep DFS flag */
			pAd->ChannelList[RecId].DfsReq = (ch.flags & CHANNEL_RADAR)?TRUE:FALSE;
			/* keep DFS type */
			pAd->ChannelList[RecId].RegulatoryDomain = DfsType;
			pAd->ChannelList[RecId].regFlags = ch.flags;
			pAd->ChannelList[RecId].Flags = (ch.flags & 0x3f);
			if (!(ch.flags & IEEE80211_CHAN_NO_HT40MINUS)
				|| !(ch.flags & IEEE80211_CHAN_NO_HT40PLUS))
				pAd->ChannelList[RecId].Flags |= CHANNEL_40M_CAP;
			if (!(ch.flags & IEEE80211_CHAN_NO_80MHZ))
				pAd->ChannelList[RecId].Flags |= CHANNEL_80M_CAP;
			/* re-set DFS info. */
			pAd->CommonCfg.RDDurRegion = DfsType;

			CFG80211DBG(RT_DEBUG_TRACE,
				("Chan %03d:\tpower %d dBm, DFS %d, DFS Type %d\n"
				, chidx, ch.max_power, pAd->ChannelList[RecId].DfsReq, DfsType));
			/* change to record next channel info. */
			RecId++;
		}
	}
	pAd->ChannelListNum = RecId;

	return NDIS_STATUS_SUCCESS;
fail4:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail4: Parsinig band info fail\n"
		, __func__));
fail5:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail5: channelist NULL\n"
		, __func__));
	return NDIS_STATUS_FAILURE;
}

NDIS_STATUS CFG80211_UpdateChList(PRTMP_ADAPTER pAd, UINT8 supFlags
	, struct ieee80211_supported_band *band2g, struct ieee80211_supported_band *band5g)
{
	UINT32 band_id;
	UINT32 num_ch, ch_id;
	INT sup_band = 0;
	INT start_band = (supFlags & RFIC_24GHZ)?0:1;
	CFG80211_CB *cfg80211_cb = NULL;
	struct _CH_FLAGS_BEACON *flags_updated;
	ULONG IrqFlags;

	if (supFlags & RFIC_24GHZ)
		sup_band++;

	if (supFlags & RFIC_5GHZ)
		sup_band++;

	RTMP_DRIVER_80211_CB_GET(pAd, &cfg80211_cb);
	flags_updated = cfg80211_cb->ch_flags_by_beacon;
	RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
	/* 1. Calcute the Channel Number */
	CFG80211DBG(RT_DEBUG_TRACE, ("crda> update wiphy channel list, %s\n", __func__));
	for (band_id = start_band; band_id < sup_band; band_id++) {
		struct ieee80211_supported_band *band_tmp = NULL;
		struct ieee80211_channel *channels = NULL;

		if (band_id == 0) {
			CFG80211DBG(RT_DEBUG_TRACE,
				("%s, crda> reset chan/power for 2.4GHz\n", __func__));
			band_tmp = band2g;
		} else {
			CFG80211DBG(RT_DEBUG_TRACE,
				("%s crda> reset chan/power for 5GHz\n", __func__));
			band_tmp = band5g;
		}

		if (!band_tmp)
			goto fail4;
		num_ch = band_tmp->n_channels;
		channels = band_tmp->channels;

		if (!channels)
			goto fail5;

		for (ch_id = 0; ch_id < num_ch; ch_id++) {
			struct ieee80211_channel *ch = &channels[ch_id];
			UINT32 chidx = 0;
			INT drv_idx = 0;
			INT idx = 0;

			chidx = ieee80211_frequency_to_channel(ch->center_freq);

			for (drv_idx = 0; drv_idx < pAd->ChannelListNum; drv_idx++) {
				if (pAd->ChannelList[drv_idx].Channel == chidx)
					break;
			}

			for (idx = 0; idx < Num_Cfg80211_Chan; idx++) {
				if (flags_updated[idx].ch == chidx)
					break;
			}

			if (drv_idx == pAd->ChannelListNum) {
				ch->flags = 0x3b;
				CFG80211DBG(RT_DEBUG_TRACE,
					("Chan %u (frq %d):\t not allowed, flags:%x\n"
					, chidx, ch->center_freq, ch->flags));
			} else {
				ch->max_power = pAd->ChannelList[drv_idx].MaxTxPwr;
				ch->flags = pAd->ChannelList[drv_idx].regFlags;
				CFG80211DBG(RT_DEBUG_TRACE,
					("Chan %u (frq %d):\t allowed, flags:%x\n"
					, chidx, ch->center_freq, ch->flags));
			}

			if (idx != Num_Cfg80211_Chan) {
				if (!(ch->flags & flags_updated[idx].flags
					& IEEE80211_CHAN_PASSIVE_SCAN)
					&& (ch->flags & IEEE80211_CHAN_PASSIVE_SCAN)) {
					ch->flags &= ~IEEE80211_CHAN_PASSIVE_SCAN;
					pAd->ChannelList[drv_idx].Flags &=
						~IEEE80211_CHAN_PASSIVE_SCAN;
					CFG80211DBG(RT_DEBUG_TRACE,
						("CHAN_PASSIVE_SCAN update flags %x ch:%u\n"
						, ch->flags, flags_updated[idx].ch));
				}
				if (!(ch->flags & flags_updated[idx].flags & IEEE80211_CHAN_RADAR)
					&& (ch->flags & IEEE80211_CHAN_RADAR)) {
					ch->flags &= ~IEEE80211_CHAN_RADAR;
					pAd->ChannelList[drv_idx].Flags &= ~IEEE80211_CHAN_RADAR;
					pAd->ChannelList[drv_idx].DfsReq = FALSE;
					CFG80211DBG(RT_DEBUG_TRACE,
						("IEEE80211_CHAN_RADAR update beacon flags %x\n"
						, ch->flags));
				}
			} else
				CFG80211DBG(RT_DEBUG_TRACE, ("%s, flags %x not updated ch %u\n"
					, __func__, ch->flags, chidx));
		}
	}
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);
	return NDIS_STATUS_SUCCESS;

fail4:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail4: Parsinig band info fail\n"
		, __func__));
fail5:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail5: channelist NULL\n"
		, __func__));
	return NDIS_STATUS_FAILURE;
}

NDIS_STATUS CFG80211_SyncCore_ChList(IN VOID *pAdCB, IN VOID *wiphy)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	struct wiphy *pWiphy = (struct wiphy *)wiphy;
	struct ieee80211_supported_band *band2g = NULL;
	struct ieee80211_supported_band *band5g = NULL;
	CFG80211_BAND band_info;

	CFG80211DBG(RT_DEBUG_TRACE, ("crda> set by core sync channel list\n"));
	if (!wiphy) {
		CFG80211DBG(RT_DEBUG_TRACE, ("%s, wiphy null try get from driver.\n"
			, __func__));
		if (!pAd->net_dev)
			goto fail1;
		pWiphy = pAd->net_dev->ieee80211_ptr->wiphy;
	}

	if (pWiphy == NULL)
		goto fail1;

	/* Init */
	RTMP_DRIVER_80211_BANDINFO_GET(pAd, &band_info);

	if (band_info.RFICType == 0)
		band_info.RFICType = RFIC_24GHZ | RFIC_5GHZ;

	/* 1. Calcute the Channel Number */
	if (band_info.RFICType & RFIC_5GHZ)
		band5g = pWiphy->bands[IEEE80211_BAND_5GHZ];

	band2g = pWiphy->bands[IEEE80211_BAND_2GHZ];

	if (!band2g)
		goto fail3;

	if (pAd->flagsUpdated)
		CFG80211_UpdateChList(pAd, band_info.RFICType, band2g, band5g);
	else if (wiphy)
		CFG80211_StoreChList(pAd, band2g, band5g);
	else if (pAd->ChannelListNum)
		CFG80211_UpdateChList(pAd, band_info.RFICType, band2g, band5g);
	else
		RT_CFG80211_CRDA_REG_RULE_APPLY(pAd);
	return NDIS_STATUS_SUCCESS;
fail1:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail1: wiphy NULL\n", __func__));
fail3:
	CFG80211DBG(RT_DEBUG_ERROR, ("%s-fail3: 2.4g channel list NULL\n"
		, __func__));
	return NDIS_STATUS_FAILURE;
}
#endif
/*
========================================================================
Routine Description:
	Apply new regulatory rule.

Arguments:
	pAdCB			- WLAN control block pointer
	pWiphy			- Wireless hardware description
	pAlpha2			- Regulation domain (2B)

Return Value:
	NONE

Note:
	Can only be called when interface is up.

	For general mac80211 device, it will be set to new power by Ops->config()
	In rt2x00/, the settings is done in rt2x00lib_config().
========================================================================
*/
VOID CFG80211_RegRuleApply(IN VOID *pAdCB, IN VOID *pWiphy, IN UCHAR *pAlpha2)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	VOID *pBand24G, *pBand5G;
	UINT32 IdBand, IdChan, IdPwr;
	UINT32 ChanNum, ChanId, Power, RecId, DfsType;
	BOOLEAN FlgIsRadar;
	ULONG IrqFlags;
	UINT32 chFlags = 0;
#ifdef DFS_SUPPORT
	RADAR_DETECT_STRUCT *pRadarDetect;
#endif /* DFS_SUPPORT */
	UCHAR ChannelListFlags[MAX_NUM_OF_CHANNELS][2];
	INT32 Idx = 0;

	CFG80211DBG(RT_DEBUG_TRACE, ("crda> CFG80211_RegRuleApply ==>\n"));

	/* init */
	pBand24G = NULL;
	pBand5G = NULL;

	if (pAd == NULL)
		return;

	RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);

	/* Fix channel flags been cleared problem.
	   Since the channel flags have been built already, we need to backup and restore those flags
	 */
#ifdef JUNGLE_SUPPORT
	if (pAd->flagsUpdated)
#endif
		for (IdChan = 0; IdChan < MAX_NUM_OF_CHANNELS; IdChan++) {
			ChannelListFlags[IdChan][0] = pAd->ChannelList[IdChan].Channel;
			ChannelListFlags[IdChan][1] = pAd->ChannelList[IdChan].Flags;
		}
	/* zero first */
	NdisZeroMemory(pAd->ChannelList, MAX_NUM_OF_CHANNELS * sizeof(CHANNEL_TX_POWER));

	/* 2.4GHZ & 5GHz */
	RecId = 0;
#ifdef DFS_SUPPORT
	pRadarDetect = &pAd->CommonCfg.RadarDetect;
#endif /* DFS_SUPPORT */

	/* find the DfsType */
	DfsType = CE;

	pBand24G = NULL;
	pBand5G = NULL;

	if (CFG80211OS_BandInfoGet(CFG80211CB, pWiphy, &pBand24G, &pBand5G) == FALSE)
		return;

#ifdef AUTO_CH_SELECT_ENHANCE
#ifdef EXT_BUILD_CHANNEL_LIST
	if ((pAlpha2[0] != '0') && (pAlpha2[1] != '0')) {
		UINT32 IdReg;

		if (pBand5G != NULL) {
			for (IdReg = 0;; IdReg++) {
				if (ChRegion[IdReg].CountReg[0] == 0x00)
					break;

				if ((pAlpha2[0] == ChRegion[IdReg].CountReg[0]) &&
				    (pAlpha2[1] == ChRegion[IdReg].CountReg[1])) {
					if (pAd->CommonCfg.DfsType != MAX_RD_REGION)
						DfsType = pAd->CommonCfg.DfsType;
					else
						DfsType = ChRegion[IdReg].DfsType;

					CFG80211DBG(RT_DEBUG_TRACE,
						    ("crda> find region %c%c, DFS Type %d\n",
						     pAlpha2[0], pAlpha2[1], DfsType));
					break;
				}
			}
		}
	}
#endif /* EXT_BUILD_CHANNEL_LIST */
#endif /* AUTO_CH_SELECT_ENHANCE */

	CFG80211DBG(RT_DEBUG_ERROR, ("crda> find region %d-%d, DFS Type %d\n",
				     pAlpha2[0], pAlpha2[1], DfsType));
#if defined(SINGLE_SKU_V2) && defined(JUNGLE_SUPPORT)
	RTMPLoadSKUProfile(pAd, pAlpha2);
#endif

	for (IdBand = 0; IdBand < 2; IdBand++) {
		if (((IdBand == 0) && (pBand24G == NULL)) || ((IdBand == 1) && (pBand5G == NULL)))
			continue;

		if (IdBand == 0)
			CFG80211DBG(RT_DEBUG_TRACE, ("crda> reset chan/power for 2.4GHz\n"));
		else
			CFG80211DBG(RT_DEBUG_TRACE, ("crda> reset chan/power for 5GHz\n"));

		ChanNum = CFG80211OS_ChanNumGet(CFG80211CB, pWiphy, IdBand);

		for (IdChan = 0; IdChan < ChanNum; IdChan++) {
			if (CFG80211OS_ChanInfoGet(CFG80211CB, pWiphy, IdBand, IdChan,
						&ChanId, &Power, &FlgIsRadar, &chFlags) == FALSE) {
				/* the channel is not allowed in the regulatory domain */
				/* get next channel information */
				continue;
			}

			if (!WMODE_CAP_2G(pAd->CommonCfg.PhyMode)) {
				/* 5G-only mode */
				if (ChanId <= CFG80211_NUM_OF_CHAN_2GHZ)
					continue;
			}

			if (!WMODE_CAP_5G(pAd->CommonCfg.PhyMode)) {
				/* 2.4G-only mode */
				if (ChanId > CFG80211_NUM_OF_CHAN_2GHZ)
					continue;
			}

			for (IdPwr = 0; IdPwr < MAX_NUM_OF_CHANNELS; IdPwr++) {
				if (ChanId == pAd->TxPower[IdPwr].Channel) {
					/* init the channel info. */
					NdisMoveMemory(&pAd->ChannelList[RecId],
						       &pAd->TxPower[IdPwr],
						       sizeof(CHANNEL_TX_POWER));

					/* keep channel number */
					pAd->ChannelList[RecId].Channel = ChanId;

					/* keep maximum tranmission power */
					pAd->ChannelList[RecId].MaxTxPwr = Power;

					/* keep DFS flag */
					if (FlgIsRadar == TRUE)
						pAd->ChannelList[RecId].DfsReq = TRUE;
					else
						pAd->ChannelList[RecId].DfsReq = FALSE;

					/* keep DFS type */
					pAd->ChannelList[RecId].RegulatoryDomain = DfsType;

					/* Fix channel flags been cleared problem.
	Since the channel flags have been built already, we need to backup and restore those flags
					 */
					for (Idx = 0; Idx < MAX_NUM_OF_CHANNELS; Idx++) {
						if (ChannelListFlags[Idx][0] ==
						    pAd->ChannelList[RecId].Channel) {
							pAd->ChannelList[RecId].Flags =
							    ChannelListFlags[Idx][1];
							break;
						}
					}
#ifdef JUNGLE_SUPPORT /* Flags definition is different from kernel in 3.10 */
					pAd->ChannelList[RecId].regFlags = chFlags;
					pAd->ChannelList[RecId].Flags = (chFlags & 0x3f);
					if (!(chFlags & IEEE80211_CHAN_NO_HT40MINUS)
						|| !(chFlags & IEEE80211_CHAN_NO_HT40PLUS))
						pAd->ChannelList[RecId].Flags |= CHANNEL_40M_CAP;
					if (!(chFlags & IEEE80211_CHAN_NO_80MHZ))
						pAd->ChannelList[RecId].Flags |= CHANNEL_80M_CAP;
					if (pAd->flagsUpdated && (Idx != MAX_NUM_OF_CHANNELS)
						&& !(ChannelListFlags[Idx][1] &&
						(IEEE80211_CHAN_PASSIVE_SCAN|IEEE80211_CHAN_RADAR)))
						pAd->ChannelList[RecId].Flags &=
							~(IEEE80211_CHAN_PASSIVE_SCAN
								|IEEE80211_CHAN_RADAR);
#endif
					/* re-set DFS info. */
					pAd->CommonCfg.RDDurRegion = DfsType;

					CFG80211DBG(RT_DEBUG_TRACE,
				("Chan %03d:\tpower %d dBm, DFS %d, DFS Type %d\n",
						     ChanId, Power,
						     ((FlgIsRadar == TRUE) ? 1 : 0), DfsType));

					/* change to record next channel info. */
					RecId++;
					break;
				}
			}
		}
	}

	pAd->ChannelListNum = RecId;
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);

#ifdef JUNGLE_SUPPORT
	if (pAd->flagsUpdated)
		CFG80211_SyncCore_ChList(pAd, NULL);
#if defined(SINGLE_SKU_V2)
	AsicSwitchChannel(pAd, pAd->CommonCfg.Channel, FALSE);
	CFG80211DBG(RT_DEBUG_TRACE, ("%s commoncfg channel %u, hw_cfg.cent_ch, %u\n", __func__
		, pAd->CommonCfg.Channel, pAd->hw_cfg.cent_ch));
#endif
#endif
	CFG80211DBG(RT_DEBUG_TRACE, ("crda> Number of channels = %d\n", RecId));
}				/* End of CFG80211_RegRuleApply */

/*
========================================================================
Routine Description:
	Inform CFG80211 about association status.

Arguments:
	pAdCB			- WLAN control block pointer
	pBSSID			- the BSSID of the AP
	pReqIe			- the element list in the association request frame
	ReqIeLen		- the request element length
	pRspIe			- the element list in the association response frame
	RspIeLen		- the response element length
	FlgIsSuccess	- 1: success; otherwise: fail

Return Value:
	None

Note:
========================================================================
*/
VOID CFG80211_ConnectResultInform(IN VOID *pAdCB,
				  IN UCHAR *pBSSID,
				  IN UCHAR *pReqIe,
				  IN UINT32 ReqIeLen,
				  IN UCHAR *pRspIe, IN UINT32 RspIeLen, IN UCHAR FlgIsSuccess)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> %s ==>\n", __func__));

	CFG80211OS_ConnectResultInform(CFG80211CB,
				       pBSSID, pReqIe, ReqIeLen, pRspIe, RspIeLen, FlgIsSuccess);

	if (pAd->cfg80211_ctrl.FlgCfg80211Scanning == TRUE ||
	    RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
		CFG80211DBG(RT_DEBUG_ERROR, ("[%s] Clear ongoing scan\n", __func__));
		CFG80211DRV_OpsScanInLinkDownAction(pAd);
	}

	pAd->cfg80211_ctrl.FlgCfg80211Connecting = FALSE;
}				/* End of CFG80211_ConnectResultInform */

/*
========================================================================
Routine Description:
	Re-Initialize wireless channel/PHY in 2.4GHZ and 5GHZ.

Arguments:
	pAdCB			- WLAN control block pointer

Return Value:
	TRUE			- re-init successfully
	FALSE			- re-init fail

Note:
	CFG80211_SupBandInit() is called in xx_probe().
	But we do not have complete chip information in xx_probe() so we
	need to re-init bands in xx_open().
========================================================================
*/
BOOLEAN CFG80211_SupBandReInit(IN VOID *pAdCB)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	CFG80211_BAND BandInfo;

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> re-init bands...\n"));

	/* re-init bands */
#ifdef JUNGLE_SUPPORT
	pAd->applyUpperLayerReg = FALSE;
#endif
	NdisZeroMemory(&BandInfo, sizeof(BandInfo));
	CFG80211_BANDINFO_FILL(pAd, &BandInfo);

	return CFG80211OS_SupBandReInit(CFG80211CB, &BandInfo);
}				/* End of CFG80211_SupBandReInit */

#ifdef CONFIG_STA_SUPPORT
/* CMD_RTPRIV_IOCTL_80211_KEY_DEFAULT_SET: */
INT CFG80211_setStaDefaultKey(IN VOID *pAdCB, IN UINT Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;

	DBGPRINT(RT_DEBUG_TRACE, ("Set Sta Default Key: %d\n", Data));
	pAd->StaCfg.wdev.DefaultKeyId = Data;	/* base 0 */
	return 0;
}

#ifdef DOT11W_PMF_SUPPORT
INT CFG80211_setStaMgmtDefaultKey(IN VOID *pAdCB, IN UINT Data)
{
	/* PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdCB; */

	DBGPRINT(RT_DEBUG_TRACE, ("Set Sta MgmtDefault Key: %d\n", Data));
	return 0;
}
#endif /* DOT11W_PMF_SUPPORT */

#endif /*CONFIG_STA_SUPPORT */
INT CFG80211_reSetToDefault(IN VOID *pAdCB)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	PCFG80211_CTRL pCfg80211_ctrl = &pAd->cfg80211_ctrl;

	DBGPRINT(RT_DEBUG_TRACE, (" %s\n", __func__));
#ifdef CONFIG_STA_SUPPORT
	/* Driver Internal Parm */
	pAd->StaCfg.bAutoConnectByBssid = FALSE;
#endif /*CONFIG_STA_SUPPORT */
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterProbeReqFrame = FALSE;
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211RegisterActionFrame = FALSE;
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211ProbeReqCount = 0;
	pCfg80211_ctrl->cfg80211MainDev.Cfg80211ActionCount = 0;

	pCfg80211_ctrl->Cfg80211RocTimerInit = FALSE;
	pCfg80211_ctrl->Cfg80211RocTimerRunning = FALSE;
	pCfg80211_ctrl->FlgCfg80211Scanning = FALSE;

	if (pAd->cfg80211_ctrl.pTxStatusBuf != NULL) {
		os_free_mem(NULL, pAd->cfg80211_ctrl.pTxStatusBuf);
		pAd->cfg80211_ctrl.pTxStatusBuf = NULL;
	}

	return TRUE;
}

/*
initList(&pAd->Cfg80211VifDevSet.vifDevList);
initList(&pAd->cfg80211_ctrl.cfg80211TxPacketList);
*/
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
BOOLEAN CFG80211_checkScanResInKernelCache(IN VOID *pAdCB,
					   IN UCHAR *pBSSID, IN UCHAR *pSsid, IN INT ssidLen)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	CFG80211_CB *pCfg80211_CB = (CFG80211_CB *) pAd->pCfg80211_CB;
	struct wiphy *pWiphy = pCfg80211_CB->pCfg80211_Wdev->wiphy;
	struct cfg80211_bss *bss;

	if (pBSSID) {
		DBGPRINT(RT_DEBUG_OFF, ("%s find BSS [0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X]\n"
		, __func__, PRINT_MAC(pBSSID)));
	}
	if (pSsid)
		DBGPRINT(RT_DEBUG_OFF, ("%s find BSS %s\n", __func__, pSsid));
	bss = cfg80211_get_bss(pWiphy, NULL, pBSSID,
			       pSsid, ssidLen, WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
	if (bss) {
		/*add to bss table*/
		/*
		ULONG Idx = 0;
		BCN_IE_LIST *ie_list = NULL;
		*/
		const UCHAR *ssid_eid;

		DBGPRINT(RT_DEBUG_OFF,
			("%s found BSS [0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X] CH freq %d\n"
			, __func__, PRINT_MAC(bss->bssid), bss->channel->center_freq));
		ssid_eid = ieee80211_bss_get_ie(bss, WLAN_EID_SSID);
		if (!ssid_eid)
			DBGPRINT(RT_DEBUG_OFF, ("%s no ssid!\n", __func__));
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0)
		if (bss->ies)
			DBGPRINT(RT_DEBUG_OFF, ("%s found ies\n", __func__));
#endif /* >= 3.8.0 */
		if (bss->beacon_ies)
			DBGPRINT(RT_DEBUG_OFF, ("%s found beacon_ies\n", __func__));
		if (bss->proberesp_ies)
			DBGPRINT(RT_DEBUG_OFF, ("%s found proberesp_ies\n", __func__));
		/*
		Idx =
		    BssTableSetEntry(pAd, &pAd->ScanTab, ie_list, Rssi, NULL, LenVIE,
				     pVIE);
		*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
		cfg80211_put_bss(pWiphy, bss);
#else
		cfg80211_put_bss(bss);
#endif /* endif */
		return TRUE;
	}

	return FALSE;
}

BOOLEAN CFG80211_checkScanTable(IN VOID *pAdCB)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	CFG80211_CB *pCfg80211_CB = (CFG80211_CB *) pAd->pCfg80211_CB;
	struct wiphy *pWiphy = pCfg80211_CB->pCfg80211_Wdev->wiphy;
	ULONG bss_idx = BSS_NOT_FOUND;
	struct cfg80211_bss *bss;
	struct ieee80211_channel *chan;
	UINT32 CenFreq;
	UINT64 timestamp;
	struct timeval tv;
	UCHAR *ie, ieLen = 0;
	BOOLEAN isOk = FALSE;
	BSS_ENTRY *pBssEntry;

	USHORT ifIndex = 0;
	PAPCLI_STRUCT pApCliEntry = NULL;

	pApCliEntry = &pAd->ApCfg.ApCliTab[ifIndex];

	if (MAC_ADDR_EQUAL(pApCliEntry->MlmeAux.Bssid, ZERO_MAC_ADDR)) {
		CFG80211DBG(RT_DEBUG_ERROR, ("pAd->ApCliMlmeAux.Bssid ==> ZERO_MAC_ADDR\n"));
		/* ToDo: pAd->ApCfg.ApCliTab[0].CfgApCliBssid */
		return FALSE;
	}

	/* Fake TSF */
	do_gettimeofday(&tv);
	timestamp = ((UINT64) tv.tv_sec * 1000000) + tv.tv_usec;

	bss = cfg80211_get_bss(pWiphy, NULL, pApCliEntry->MlmeAux.Bssid,
			       pApCliEntry->MlmeAux.Ssid, pApCliEntry->MlmeAux.SsidLen,
			       WLAN_CAPABILITY_ESS, WLAN_CAPABILITY_ESS);
	if (bss) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Found %s in Kernel_ScanTable with CH[%d]\n", pApCliEntry->MlmeAux.Ssid,
			  bss->channel->center_freq));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
		cfg80211_put_bss(pWiphy, bss);
#else
		bss->tsf = timestamp;
		cfg80211_put_bss(bss);
#endif /* endif */
		return TRUE;
	}
	DBGPRINT(RT_DEBUG_ERROR,
		("Can't Found %s in Kernel_ScanTable & Try Fake it\n",
			pApCliEntry->MlmeAux.Ssid));

	bss_idx =
	    BssSsidTableSearchBySSID(&pAd->ScanTab, pApCliEntry->MlmeAux.Ssid,
				     pApCliEntry->MlmeAux.SsidLen);

	if (bss_idx != BSS_NOT_FOUND) {
		/* Since the cfg80211 kernel scanTable not exist this Entry,
		 * Build an Entry for this connect inform event.
		 */

		pBssEntry = &pAd->ScanTab.BssEntry[bss_idx];

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 39))
		if (pAd->ScanTab.BssEntry[bss_idx].Channel > 14)
			CenFreq =
			    ieee80211_channel_to_frequency(pBssEntry->Channel, IEEE80211_BAND_5GHZ);
		else
			CenFreq =
			    ieee80211_channel_to_frequency(pBssEntry->Channel, IEEE80211_BAND_2GHZ);
#else
		CenFreq = ieee80211_channel_to_frequency(pBssEntry->Channel);
#endif /* endif */
		chan = ieee80211_get_channel(pWiphy, CenFreq);

		ieLen = 2 + pApCliEntry->MlmeAux.SsidLen + pBssEntry->VarIeFromProbeRspLen;

		os_alloc_mem(NULL, (UCHAR **) &ie, ieLen);
		if (!ie) {
			CFG80211DBG(RT_DEBUG_ERROR,
				    ("Memory Allocate Fail in CFG80211_checkScanTable\n"));
			return FALSE;
		}

		ie[0] = WLAN_EID_SSID;
		ie[1] = pApCliEntry->MlmeAux.SsidLen;
		NdisCopyMemory(ie + 2, pApCliEntry->MlmeAux.Ssid, pApCliEntry->MlmeAux.SsidLen);
		NdisCopyMemory(ie + 2 + pApCliEntry->MlmeAux.SsidLen, pBssEntry->pVarIeFromProbRsp,
			       pBssEntry->VarIeFromProbeRspLen);

		bss = cfg80211_inform_bss(pWiphy, chan,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 18, 0))
					  CFG80211_BSS_FTYPE_UNKNOWN,
#endif
					  pApCliEntry->MlmeAux.Bssid, timestamp,
					  WLAN_CAPABILITY_ESS, pApCliEntry->MlmeAux.BeaconPeriod,
					  ie, ieLen,
#ifdef CFG80211_SCAN_SIGNAL_AVG
					  (pBssEntry->MaxRssi * 100),
#else
					  (pBssEntry->Rssi * 100),
#endif /* endif */
					  GFP_KERNEL);
		if (bss) {
			printk
			    ("Fake New %s(%02x:%02x:%02x:%02x:%02x:%02x) in Kernel_ScanTable with CH[%d][%d] BI:%d len:%d\n",
			     pApCliEntry->MlmeAux.Ssid, PRINT_MAC(pApCliEntry->MlmeAux.Bssid),
			     bss->channel->center_freq, pBssEntry->Channel,
			     pApCliEntry->MlmeAux.BeaconPeriod, pBssEntry->VarIeFromProbeRspLen);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0))
			cfg80211_put_bss(pWiphy, bss);
#else
			cfg80211_put_bss(bss);
#endif /* endif */
			isOk = TRUE;
		}

		if (ie != NULL)
			os_free_mem(NULL, ie);

		if (isOk)
			return TRUE;
	} else
	printk(KERN_DEBUG "%s Not In Driver Scan Table\n", pApCliEntry->MlmeAux.Ssid);

	return FALSE;
}
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

/* CFG_TODO */
UCHAR CFG80211_getCenCh(RTMP_ADAPTER *pAd, UCHAR prim_ch)
{
	UCHAR ret_channel;

	if (pAd->CommonCfg.RegTransmitSetting.field.BW == BW_40) {
		if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
			ret_channel = prim_ch + 2;
		else {
			if (prim_ch == 14)
				ret_channel = prim_ch - 1;
			else
				ret_channel = prim_ch - 2;
		}
	} else
		ret_channel = prim_ch;

	return ret_channel;
}

#endif /* RT_CFG80211_SUPPORT */
