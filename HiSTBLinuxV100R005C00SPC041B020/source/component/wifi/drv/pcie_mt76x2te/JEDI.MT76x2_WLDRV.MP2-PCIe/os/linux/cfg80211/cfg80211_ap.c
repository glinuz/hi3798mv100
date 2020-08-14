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

#ifdef CONFIG_AP_SUPPORT
static INT CFG80211DRV_UpdateTimIE(PRTMP_ADAPTER pAd, UINT mbss_idx, PUCHAR pBeaconFrame,
				   UINT32 tim_ie_pos)
{
	UCHAR ID_1B, TimFirst, TimLast, *pTim, *ptr, New_Tim_Len;
	UINT i;

	ptr = pBeaconFrame + tim_ie_pos;	/* TIM LOCATION */
	*ptr = IE_TIM;
	*(ptr + 2) = pAd->ApCfg.DtimCount[mbss_idx];
	*(ptr + 3) = pAd->ApCfg.DtimPeriod[mbss_idx];

	TimFirst = 0;		/* record first TIM byte != 0x00 */
	TimLast = 0;		/* record last  TIM byte != 0x00 */

	pTim = pAd->ApCfg.MBSSID[mbss_idx].TimBitmaps;

	for (ID_1B = 0; ID_1B < WLAN_MAX_NUM_OF_TIM; ID_1B++) {
		/* get the TIM indicating PS packets for 8 stations */
		UCHAR tim_1B = pTim[ID_1B];

		if (ID_1B == 0)
			tim_1B &= 0xfe;	/* skip bit0 bc/mc */

		if (tim_1B == 0)
			continue;	/* find next 1B */

		if (TimFirst == 0)
			TimFirst = ID_1B;

		TimLast = ID_1B;
	}

	/* fill TIM content to beacon buffer */
	if (TimFirst & 0x01)
		TimFirst--;	/* find the even offset byte */

	*(ptr + 1) = 3 + (TimLast - TimFirst + 1);	/* TIM IE length */
	*(ptr + 4) = TimFirst;

	for (i = TimFirst; i <= TimLast; i++)
		*(ptr + 5 + i - TimFirst) = pTim[i];

	/* bit0 means backlogged mcast/bcast */
	if (pAd->ApCfg.DtimCount[mbss_idx] == 0)
		*(ptr + 4) |=
		    (pAd->ApCfg.MBSSID[mbss_idx].TimBitmaps[WLAN_CT_TIM_BCMC_OFFSET] & 0x01);

	/* adjust BEACON length according to the new TIM */
	New_Tim_Len = (2 + *(ptr + 1));

	return New_Tim_Len;
}

static INT CFG80211DRV_UpdateApSettingFromBeacon(PRTMP_ADAPTER pAd, UINT mbss_idx,
						 CMD_RTPRIV_IOCTL_80211_BEACON *pBeacon)
{
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[mbss_idx];
	struct wifi_dev *wdev = &pMbss->wdev;

	const UCHAR *wpa_ie = NULL, *rsn_ie = NULL;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0))
	UINT WFA_OUI = 0x0050F2;
	UCHAR WMM_OUI_TYPE = 0x2;
	const UCHAR *wmm_ie = NULL;
#endif /* endif */

	const UCHAR *supp_rates_ie = NULL;
	const UCHAR *ext_supp_rates_ie = NULL, *ht_cap = NULL, *ht_info = NULL;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0))
	const UCHAR CFG_HT_OP_EID = WLAN_EID_HT_OPERATION;
#else
	const UCHAR CFG_HT_OP_EID = WLAN_EID_HT_INFORMATION;
#endif /* LINUX_VERSION_CODE: 3.5.0 */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 8, 0))
	const UCHAR CFG_WPA_EID = WLAN_EID_VENDOR_SPECIFIC;
#else
	const UCHAR CFG_WPA_EID = WLAN_EID_WPA;
#endif /* LINUX_VERSION_CODE: 3.8.0 */

	supp_rates_ie =
	    cfg80211_find_ie(WLAN_EID_SUPP_RATES, pBeacon->beacon_head + 36,
			     pBeacon->beacon_head_len - 36);
	/* if it doesn't find WPA_IE in tail first 30 bytes. treat it as is not found */
	wpa_ie = cfg80211_find_ie(CFG_WPA_EID, pBeacon->beacon_tail, pBeacon->beacon_tail_len);
	rsn_ie = cfg80211_find_ie(WLAN_EID_RSN, pBeacon->beacon_tail, pBeacon->beacon_tail_len);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0))
	wmm_ie =
	    cfg80211_find_vendor_ie(WFA_OUI, WMM_OUI_TYPE, pBeacon->beacon_tail,
				    pBeacon->beacon_tail_len);
#endif /* endif */
	ext_supp_rates_ie =
	    cfg80211_find_ie(WLAN_EID_EXT_SUPP_RATES, pBeacon->beacon_tail,
			     pBeacon->beacon_tail_len);
	ht_cap =
	    cfg80211_find_ie(WLAN_EID_HT_CAPABILITY, pBeacon->beacon_tail,
			     pBeacon->beacon_tail_len);
	ht_info = cfg80211_find_ie(CFG_HT_OP_EID, pBeacon->beacon_tail, pBeacon->beacon_tail_len);



	/* WMM EDCA Paramter */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 1, 0))
	CFG80211_SyncPacketWmmIe(pAd, mbss_idx, pBeacon->beacon_tail, pBeacon->beacon_tail_len);
#endif /* endif */
	/* Security */
	CFG80211_ParseBeaconIE(pAd, pMbss, wdev, wpa_ie, rsn_ie);

	pMbss->CapabilityInfo =
	    CAP_GENERATE(1, 0, (wdev->WepStatus != Ndis802_11EncryptionDisabled),
			 (pAd->CommonCfg.TxPreamble == Rt802_11PreambleLong ? 0 : 1),
			 pAd->CommonCfg.bUseShortSlotTime, /*SpectrumMgmt */ FALSE);

	/* Disable Driver-Internal Rekey */
	pMbss->WPAREKEY.ReKeyInterval = 0;
	pMbss->WPAREKEY.ReKeyMethod = DISABLE_REKEY;

	if (pBeacon->interval != 0) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG_TIM New BI %d\n", pBeacon->interval));
		pAd->CommonCfg.BeaconPeriod = pBeacon->interval;
	}

	if (pBeacon->dtim_period != 0) {
		DBGPRINT(RT_DEBUG_TRACE, ("CFG_TIM New DP %d\n", pBeacon->dtim_period));
#ifdef RT_CFG80211_P2P_SUPPORT
		pAd->ApCfg.DtimPeriod[mbss_idx] = 1;
#else
		pAd->ApCfg.DtimPeriod[mbss_idx] = pBeacon->dtim_period;
#endif
	}

	return 1;
}

static VOID CFG80211DRV_UpdateApSsidFromBeacon(PRTMP_ADAPTER pAd, UINT mbss_idx,
						 CMD_RTPRIV_IOCTL_80211_BEACON *pBeacon)
{
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[mbss_idx];

	if (pBeacon->ssid_len <= IEEE80211_MAX_SSID_LEN) {
		pMbss->SsidLen = pBeacon->ssid_len;
		NdisCopyMemory(pMbss->Ssid, pBeacon->ssid, pMbss->SsidLen);
	} else
		DBGPRINT(RT_DEBUG_ERROR, ("CFG: Invalid SSID len %d\n", (INT)pBeacon->ssid_len));

	DBGPRINT(RT_DEBUG_TRACE, ("CFG : SSID: %s, %d\n", pMbss->Ssid, pMbss->SsidLen));

	pMbss->hidden_ssid = pBeacon->hidden_ssid;
}

VOID CFG80211DRV_DisableApInterface(PRTMP_ADAPTER pAd,
				    struct net_device *pNetDev,
				    u8 oldIfType)
{
	/*CFG_TODO: IT Should be set fRTMP_ADAPTER_HALT_IN_PROGRESS */
	int mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAd, pNetDev);
	MULTISSID_STRUCT *pMbss = &pAd->ApCfg.MBSSID[mbssidIdx];
	struct wifi_dev *wdev = &pMbss->wdev;

	pAd->ApCfg.MBSSID[mbssidIdx].bBcnSntReq = FALSE;
	wdev->Hostapd = Hostapd_Diable;

	/* For AP - STA switch */
	if ((pAd->CommonCfg.BBPCurrentBW != BW_20) && (!INFRA_ON(pAd))) {
		CFG80211DBG(RT_DEBUG_TRACE, ("80211> %s, switch to BW_20\n", __func__));
		bbp_set_bw(pAd, BW_20);
	}

	/* Disable pre-TBTT interrupt */
	AsicSetPreTbtt(pAd, FALSE);

	if (!INFRA_ON(pAd)) {
		/* Disable piggyback */
		RTMPSetPiggyBack(pAd, FALSE);
		AsicUpdateProtect(pAd, 0, (ALLN_SETPROTECT | CCKSETPROTECT | OFDMSETPROTECT), TRUE,
				  FALSE);
	}

	RTMP_CFG80211_DISABLE_BEACON(pAd, pNetDev, oldIfType);

	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) &&
	    !CFG_P2PGO_ON(pAd)
#ifdef RT_CFG80211_AP_SUPPORT
	    && !CFG_AP_ON(pAd)
#endif /* RT_CFG80211_AP_SUPPORT */
	   ) {
		AsicDisableSync(pAd);
	}
#ifdef RTMP_MAC_USB
	/* For RT2870, we need to clear the beacon sync buffer. */
	if (!CFG_P2PGO_ON(pAd)
#ifdef RT_CFG80211_AP_SUPPORT
	    && !CFG_AP_ON(pAd)
#endif /* RT_CFG80211_AP_SUPPORT */
	   )
		RTUSBBssBeaconExit(pAd);
#endif /* RTMP_MAC_USB */

	OPSTATUS_CLEAR_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED);
	RTMP_IndicateMediaState(pAd, NdisMediaStateDisconnected);
#ifdef CONFIG_STA_SUPPORT
	/* re-assoc to STA's wdev */
	RTMP_OS_NETDEV_SET_WDEV(pAd->net_dev, &pAd->StaCfg.wdev);
#endif /*CONFIG_STA_SUPPORT */
}

#ifdef CONFIG_MULTI_CHANNEL
INT rtstrstr2(PSTRING s1, const PSTRING s2, INT s1_len, INT s2_len)
{
	INT offset = 0;

	while (s1_len >= s2_len) {
		s1_len--;
		if (!memcmp(s1, s2, s2_len))
			return offset;
		s1++;
		offset++;
	}
	return 0;
}
#endif /* CONFIG_MULTI_CHANNEL */

VOID CFG80211_UpdateBeacon(VOID *pAdOrg,
			   PNET_DEV pNetDev,
			   UCHAR *beacon_head_buf,
			   UINT32 beacon_head_len,
			   UCHAR *beacon_tail_buf, UINT32 beacon_tail_len, BOOLEAN isAllUpdate)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	HTTRANSMIT_SETTING BeaconTransmit;	/* MGMT frame PHY rate setting when operatin at Ht rate. */
	INT mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdOrg, pNetDev);
	PUCHAR pBeaconFrame = (PUCHAR) pAd->ApCfg.MBSSID[mbssidIdx].BeaconBuf;
	TXWI_STRUC *pTxWI = &pAd->BeaconTxWI;
	UCHAR New_Tim_Len;
	UINT32 beacon_len;
	PMULTISSID_STRUCT pMbss = &(pAd->ApCfg.MBSSID[mbssidIdx]);

#ifdef CONFIG_MULTI_CHANNEL
	INT bufferoffset = 0;
	USHORT bufferoffset2 = 0;
	STRING temp_buf[512] = { 0 };
	STRING P2POUIBYTE[4] = { 0x50, 0x6f, 0x9a, 0x9 };
	INT P2P_IE = 4;
#endif /* CONFIG_MULTI_CHANNEL */

	/* Invoke From CFG80211 OPS For setting Beacon buffer */
	if (isAllUpdate) {
		/* 1. Update the Before TIM IE */
		NdisCopyMemory(pBeaconFrame, beacon_head_buf, beacon_head_len);

		/* 2. Update the TIM IE */
		pAd->ApCfg.MBSSID[mbssidIdx].TimIELocationInBeacon = beacon_head_len;

		/* 3. Store the Tail Part For appending later */
		if (pMbss->beacon_tail_buf != NULL)
			os_free_mem(NULL, pMbss->beacon_tail_buf);

		os_alloc_mem(NULL, (UCHAR **) &pMbss->beacon_tail_buf, beacon_tail_len);
		if (pMbss->beacon_tail_buf != NULL) {
			NdisCopyMemory(pMbss->beacon_tail_buf, beacon_tail_buf,
				       beacon_tail_len);
			pMbss->beacon_tail_len = beacon_tail_len;
		} else {
			pMbss->beacon_tail_len = 0;
			DBGPRINT(RT_DEBUG_ERROR, ("CFG80211 Beacon: MEM ALLOC ERROR\n"));
		}

		RTMP_CFG80211_ENABLE_BEACON(pAd, pNetDev);

		return;
	}
	/* Invoke From Beacon Timer */
	if (pAd->ApCfg.DtimCount[mbssidIdx] == 0)
		pAd->ApCfg.DtimCount[mbssidIdx] =
			pAd->ApCfg.DtimPeriod[mbssidIdx] - 1;
	else
		pAd->ApCfg.DtimCount[mbssidIdx] -= 1;

	/* 4. Update the TIM IE */
	New_Tim_Len = CFG80211DRV_UpdateTimIE(pAd, mbssidIdx, pBeaconFrame,
					      pMbss->TimIELocationInBeacon);

	beacon_len = pMbss->TimIELocationInBeacon + New_Tim_Len;

	/* first time enable */
	if (pNetDev->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP) {
		if (VENDOR_IE_ON(pAd) && pAd->vendorIELocation == 0 &&
		    pAd->vendorIELen != 0) {

			if (pMbss->beacon_tail_buf != NULL) {

				UCHAR *beacon_tail_buf_tmp = NULL;
				INT beacon_tail_len_tmp = 0;

				os_alloc_mem(NULL,
					     (UCHAR **)&beacon_tail_buf_tmp,
					     pMbss->beacon_tail_len);

				if (beacon_tail_buf_tmp != NULL)
					NdisCopyMemory(beacon_tail_buf_tmp,
						       pMbss->beacon_tail_buf,
						       pMbss->beacon_tail_len);
				else
					DBGPRINT(RT_DEBUG_ERROR,
						 ("%s: MEM ALLOC for beacon_tail_buf_tmp ERROR\n",
						  __func__));

				beacon_tail_len_tmp = pMbss->beacon_tail_len;
				pAd->vendorIELocation = pMbss->beacon_tail_len;

				if (pMbss->beacon_tail_buf != NULL)
					os_free_mem(NULL,
						    pMbss->beacon_tail_buf);

				os_alloc_mem(NULL,
					     (UCHAR **)&pMbss->beacon_tail_buf,
					     beacon_tail_len_tmp +
					     pAd->vendorIELen);

				if (pMbss->beacon_tail_buf != NULL) {
					if (beacon_tail_buf_tmp != NULL)
						NdisCopyMemory(pMbss->beacon_tail_buf,
							       beacon_tail_buf_tmp,
							       beacon_tail_len_tmp);
					else
						DBGPRINT(RT_DEBUG_ERROR,
							 ("%s: beacon_tail_buf_tmp is NULL\n",
							 __func__));

					pMbss->beacon_tail_len =
						beacon_tail_len_tmp;

					NdisCopyMemory(pMbss->beacon_tail_buf +
						       pMbss->beacon_tail_len,
						       pAd->pVendorIE,
						       pAd->vendorIELen);
					pMbss->beacon_tail_len +=
						pAd->vendorIELen;
					DBGPRINT(RT_DEBUG_TRACE,
						 ("Update Vendor IE ok\n"));

				}

				if (beacon_tail_buf_tmp != NULL)
					os_free_mem(NULL, beacon_tail_buf_tmp);
			}
		}
		/* disable after enabled */
		else if (!VENDOR_IE_ON(pAd) && pAd->vendorIELocation != 0) {
			if (pMbss->beacon_tail_buf != NULL) {
				UCHAR *beacon_tail_buf_tmp = NULL;
				UINT32 vendor_ie_end_location =
					pAd->vendorIELocation + pAd->vendorIELen;

				os_alloc_mem(NULL,
					     (UCHAR **)&beacon_tail_buf_tmp,
					     pMbss->beacon_tail_len -
					     pAd->vendorIELen);

				if (beacon_tail_buf_tmp != NULL) {
					NdisCopyMemory(beacon_tail_buf_tmp,
						       pMbss->beacon_tail_buf,
						       pAd->vendorIELocation);
				} else {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("%s: MEM ALLOC for beacon_tail_buf_tmp ERROR\n",
						 __func__));
					return;
				}

				if (pMbss->beacon_tail_len >
				    vendor_ie_end_location)
					NdisCopyMemory(beacon_tail_buf_tmp +
						       pAd->vendorIELocation,
						       pMbss->beacon_tail_buf +
						       vendor_ie_end_location,
						       pMbss->beacon_tail_len -
						       vendor_ie_end_location);

				if (pMbss->beacon_tail_buf != NULL)
					os_free_mem(NULL,
						    pMbss->beacon_tail_buf);

				pMbss->beacon_tail_len -= pAd->vendorIELen;
				os_alloc_mem(NULL,
					     (UCHAR **)&pMbss->beacon_tail_buf,
					     pMbss->beacon_tail_len);

				if (beacon_tail_buf_tmp != NULL)
					NdisCopyMemory(pMbss->beacon_tail_buf,
						       beacon_tail_buf_tmp,
						       pMbss->beacon_tail_len);
				else
					DBGPRINT(RT_DEBUG_ERROR,
						 ("%s: beacon_tail_buf_tmp is NULL\n",
						 __func__));

				if (beacon_tail_buf_tmp != NULL)
					os_free_mem(NULL,
						    beacon_tail_buf_tmp);

				DBGPRINT(RT_DEBUG_TRACE, ("Remove Vendor IE ok\n"));
			}
			pAd->vendorIELocation = 0;
		}
	}

#if defined(RT_CFG80211_P2P_SUPPORT) && defined(CONFIG_DFS_SLAVE) && defined(CONFIG_P2PGO_DFS_SLAVE)

	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {
		if (pAd->StaCfg.bChannelSwitchCountingDown) {
			STRING CSAIE[5] = { 37, 3, 0, 0, 0 };

			CSAIE[2] = pAd->CommonCfg.ChannelSwitchMode;
			CSAIE[3] = pAd->CommonCfg.NewChannel;
			CSAIE[4] = pAd->CommonCfg.ChannelSwitchCount;

			NdisCopyMemory(pAd->ApCfg.MBSSID[mbssidIdx].BeaconBuf +
				       beacon_len, CSAIE, 5);

			beacon_len += 5;
		}

		if (pAd->StaCfg.bExtChannelSwitchCountingDown) {
			STRING ECSAIE[6] = { 60, 4, 0, 0, 0, 0 };

			ECSAIE[2] = pAd->CommonCfg.ExtChannelSwitchMode;
			ECSAIE[3] = pAd->CommonCfg.ExtNewOperatingClass;
			ECSAIE[4] = pAd->CommonCfg.ExtNewChannel;
			ECSAIE[5] = pAd->CommonCfg.ExtChannelSwitchCount;

			NdisCopyMemory(pAd->ApCfg.MBSSID[mbssidIdx].BeaconBuf +
				       beacon_len, ECSAIE, 6);

			beacon_len += 6;
		}
	}
#endif /* endif */

	/* 5. Update the AFTER TIM IE */
	if (pMbss->beacon_tail_buf != NULL) {

#ifdef CONFIG_MULTI_CHANNEL
		/* 3 mode:
		* 1. infra scan  7 channel  ( Duration(30+3) *7   interval (+120)  *   count  1 ),
		* 2. p2p find  3 channel (Duration (65 ) *3 interval (+130)) * count 2 > 120 sec
		* 3. mcc tw channel switch (Duration )(Infra time ) interva (+ GO time )
		*  count 3 mcc enabel always;
		*/

		if (pAd->GONoASchedule.Count > 0) {
			if (pAd->GONoASchedule.Count != 200)
				pAd->GONoASchedule.Count--;
			NdisMoveMemory(temp_buf, pMbss->beacon_tail_buf,
				       pMbss->beacon_tail_len);
			bufferoffset =
			    rtstrstr2(temp_buf, P2POUIBYTE, pMbss->beacon_tail_len,
				      P2P_IE);
			while (bufferoffset2 <=
			       (pMbss->beacon_tail_len - bufferoffset - 4 - bufferoffset2 -
				3)) {
				if ((pMbss->beacon_tail_buf)[bufferoffset + 4 +
								      bufferoffset2] == 12)
					break;

				bufferoffset2 = pMbss->beacon_tail_buf[bufferoffset + 4 + 1 +
					bufferoffset2] + bufferoffset2;
				bufferoffset2 = bufferoffset2 + 3;
			}

			NdisCopyMemory(&pMbss->beacon_tail_buf
				       [bufferoffset + 4 + bufferoffset2 + 5],
				       &pAd->GONoASchedule.Count, 1);
			NdisCopyMemory(&pMbss->beacon_tail_buf
				       [bufferoffset + 4 + bufferoffset2 + 6],
				       &pAd->GONoASchedule.Duration, 4);
			NdisCopyMemory(&pMbss->beacon_tail_buf
				       [bufferoffset + 4 + bufferoffset2 + 10],
				       &pAd->GONoASchedule.Interval, 4);
			NdisCopyMemory(&pMbss->beacon_tail_buf
				       [bufferoffset + 4 + bufferoffset2 + 14],
				       &pAd->GONoASchedule.StartTime, 4);

		}
#endif /* CONFIG_MULTI_CHANNEL */

		NdisCopyMemory(pMbss->BeaconBuf + beacon_len,
			       pMbss->beacon_tail_buf, pMbss->beacon_tail_len);

		beacon_len += pMbss->beacon_tail_len;
	} else {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("BEACON ====> CFG80211_UpdateBeacon OOPS %s\n",
			  pNetDev->name));
		return;
	}

	BeaconTransmit.word = 0;
#ifdef RT_CFG80211_P2P_SUPPORT

	/* Should be Find the P2P IE Then Set Basic Rate to 6M */
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd)) {
		BeaconTransmit.field.MODE = MODE_OFDM;	/* Use 6Mbps */
		BeaconTransmit.field.MCS = MCS_RATE_6;
		/* This value comes from experiment and simulation*/
		AsicSetTxTSFCompensation(pAd, 48);
	} else
#endif /* endif */
	if (pAd->LatchRfRegs.Channel > 14) {
		BeaconTransmit.field.MODE = MODE_OFDM;	/* Use 6Mbps */
		BeaconTransmit.field.MCS = MCS_RATE_6;
		/* This value comes from experiment and simulation */
		AsicSetTxTSFCompensation(pAd, 48);
	} else {
		BeaconTransmit.field.MODE = MODE_CCK;
		BeaconTransmit.field.MCS = MCS_LONGP_RATE_1;
		/* beacon header 24bytes - 2byte (=22bytes)delay), sent by 1Mbps */
	    AsicSetTxTSFCompensation(pAd, 176);
	}

	/* YF */
	RTMPWriteTxWI(pAd, (TXWI_STRUC *) pTxWI, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, 0,
		      BSS0Mcast_WCID, beacon_len, PID_MGMT, 0, 0, IFS_HTTXOP, &BeaconTransmit);

	updateAllBeacon(pAd, mbssidIdx, beacon_len);
}

BOOLEAN CFG80211DRV_OpsBeaconSet(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_BEACON *pBeacon;
	PNET_DEV pNetDev = NULL;
	INT mbssidIdx = 0;

	pBeacon = (CMD_RTPRIV_IOCTL_80211_BEACON *) pData;
	pNetDev = pBeacon->pNetDev;
	mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdOrg, pNetDev);

	CFG80211DRV_UpdateApSettingFromBeacon(pAd, mbssidIdx, pBeacon);
	CFG80211_UpdateBeacon(pAd, pNetDev, pBeacon->beacon_head,
			      pBeacon->beacon_head_len,
			      pBeacon->beacon_tail, pBeacon->beacon_tail_len, TRUE);
	return TRUE;
}

BOOLEAN CFG80211DRV_OpsBeaconAdd(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_BEACON *pBeacon;
	UINT32 rx_filter_flag;
	INT i, mbssidIdx = 0;
	PMULTISSID_STRUCT pMbss = NULL;
	struct wifi_dev *wdev = NULL;
	PNET_DEV pNetDev = NULL;
#ifdef RTMP_MAC_USB
	UCHAR num_idx;
#endif /* RTMP_MAC_USB */

	CFG80211DBG(RT_DEBUG_TRACE, ("80211> %s ==>\n", __func__));
#ifdef RTMP_MAC_USB
#ifdef CONFIG_AP_SUPPORT
	do {
		BOOLEAN Cancelled;

		RTMPCancelTimer(&pAd->CommonCfg.BeaconUpdateTimer, &Cancelled);
	} while (0);
#endif /*CONFIG_AP_SUPPORT */
#endif /* RTMP_MAC_USB */

	pBeacon = (CMD_RTPRIV_IOCTL_80211_BEACON *) pData;
	pNetDev = pBeacon->pNetDev;
	mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdOrg, pNetDev);
	pMbss = &pAd->ApCfg.MBSSID[mbssidIdx];
	wdev = &pMbss->wdev;

#ifdef UAPSD_SUPPORT

	pAd->ApCfg.MBSSID[mbssidIdx].UapsdInfo.bAPSDCapable = TRUE;
	wdev->UapsdInfo.bAPSDCapable = TRUE;

	pMbss->CapabilityInfo |= 0x0800;
#endif /* UAPSD_SUPPORT */

	CFG80211DRV_UpdateApSsidFromBeacon(pAd, mbssidIdx, pBeacon);
	CFG80211DRV_UpdateApSettingFromBeacon(pAd, mbssidIdx, pBeacon);

	rx_filter_flag = APNORMAL;
	RTMP_IO_WRITE32(pAd, RX_FILTR_CFG, rx_filter_flag);	/* enable RX of DMA block */

	pAd->ApCfg.BssidNum = pAd->ApCfg.BssidNum > mbssidIdx ?
			      pAd->ApCfg.BssidNum : (mbssidIdx + 1);
	pAd->MacTab.MsduLifeTime = 20;	/* default 5 seconds */
	/* CFG_TODO */
	pAd->ApCfg.MBSSID[mbssidIdx].BcnBufIdx = 0;
	for (i = 0; i < WLAN_MAX_NUM_OF_TIM; i++)
		pAd->ApCfg.MBSSID[mbssidIdx].TimBitmaps[i] = 0;

	pMbss->bBcnSntReq = TRUE;

	/* For GO Timeout */
#ifdef CONFIG_MULTI_CHANNEL
	pAd->ApCfg.StaIdleTimeout = 300;
	pMbss->StationKeepAliveTime = 60;
#else
#ifdef CERTIFICATION_SIGMA_SUPPORT
	if (SIGMA_ON(pAd)) {/*not send null frame for p2p 6.1.11, avoid sniffer check fail*/
		pMbss->StationKeepAliveTime = 0;
		pAd->ApCfg.StaIdleTimeout = 300;
	} else
#endif
	{
		pAd->ApCfg.StaIdleTimeout = MAC_TABLE_MIN_AGEOUT_TIME;
		pMbss->StationKeepAliveTime = 10;
	}
#endif /* CONFIG_MULTI_CHANNEL */

	AsicDisableSync(pAd);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: SavePyhMode=0x%x, PhyMode=0x%x\n", __func__, pAd->CommonCfg.SavedPhyMode,
		  pAd->CommonCfg.PhyMode));
	/*
	   if (pAd->CommonCfg.Channel > 14)
	   pAd->CommonCfg.PhyMode = (WMODE_A | WMODE_AN);
	   else
	   pAd->CommonCfg.PhyMode = (WMODE_B | WMODE_G |WMODE_GN);
	 */

	/* cfg_todo */
	wdev->bWmmCapable = TRUE;

	wdev->wdev_type = WDEV_TYPE_AP;
	wdev->tx_pkt_allowed = ApAllowToSendPacket;
	wdev->allow_data_tx = TRUE;
	wdev->func_dev = (void *)&pAd->ApCfg.MBSSID[mbssidIdx];
	wdev->sys_handle = (void *)pAd;

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	if ((pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) &&
	    (pNetDev->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_P2P_GO)) {
		pMbss->MSSIDDev = pNetDev;
		wdev->if_dev = pNetDev;
		COPY_MAC_ADDR(wdev->bssid, pNetDev->dev_addr);
		COPY_MAC_ADDR(wdev->if_addr, pNetDev->dev_addr);

		RTMP_OS_NETDEV_SET_WDEV(pNetDev, wdev);
	} else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
#ifdef RT_CFG80211_AP_SUPPORT
	if ((pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) &&
	    (pNetDev->ieee80211_ptr->iftype == RT_CMD_80211_IFTYPE_AP)) {
		pMbss->MSSIDDev = pNetDev;
		wdev->if_dev = pNetDev;
		COPY_MAC_ADDR(wdev->bssid, pNetDev->dev_addr);
		COPY_MAC_ADDR(wdev->if_addr, pNetDev->dev_addr);

		RTMP_OS_NETDEV_SET_WDEV(pNetDev, wdev);
	} else
#endif /* RT_CFG80211_AP_SUPPORT */
	{
		pMbss->MSSIDDev = pAd->net_dev;
		wdev->if_dev = pAd->net_dev;
		COPY_MAC_ADDR(wdev->bssid, pAd->CurrentAddress);
		COPY_MAC_ADDR(wdev->if_addr, pAd->CurrentAddress);

		/* assoc to MBSSID's wdev */
		RTMP_OS_NETDEV_SET_WDEV(pAd->net_dev, wdev);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("New AP BSSID %02x:%02x:%02x:%02x:%02x:%02x (%d)\n",
				  PRINT_MAC(wdev->bssid), pAd->CommonCfg.PhyMode));

	RTMPSetPhyMode(pAd, pAd->CommonCfg.PhyMode);

	if (WMODE_CAP_N(pAd->CommonCfg.PhyMode) && (pAd->Antenna.field.TxPath == 2))
		bbp_set_txdac(pAd, 2);
	else
		bbp_set_txdac(pAd, 0);

	/* Receiver Antenna selection */
	bbp_set_rxpath(pAd, pAd->Antenna.field.RxPath);

	if (!OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)) {
		if (WMODE_CAP_N(pAd->CommonCfg.PhyMode) || wdev->bWmmCapable) {
			/* EDCA parameters used for AP's own transmission */
			if (pAd->CommonCfg.APEdcaParm.bValid == FALSE)
				set_default_ap_edca_param(pAd);

			/* EDCA parameters to be annouced in outgoing BEACON, used by WMM STA */
			if (pAd->ApCfg.BssEdcaParm.bValid == FALSE)
				set_default_sta_edca_param(pAd);

			AsicSetEdcaParm(pAd, &pAd->CommonCfg.APEdcaParm);
		} else
			AsicSetEdcaParm(pAd, NULL);
	}

/*change here for support P2P GO 40 BW*/
#ifndef CONFIG_MULTI_CHANNEL
#endif /* CONFIG_MULTI_CHANNEL */

	AsicSetRDG(pAd, pAd->CommonCfg.bRdg);

	AsicSetRalinkBurstMode(pAd, pAd->CommonCfg.bRalinkBurstMode);

	AsicSetBssid(pAd, pAd->CurrentAddress);
	mgmt_tb_set_mcast_entry(pAd);

	DBGPRINT(RT_DEBUG_TRACE, ("%s():Reset WCID Table\n", __func__));
	{
		/* WCNCR12061 */
		if (INFRA_ON(pAd)) {
			/* skip 0 and 1 for wlan0 concurrent mode */
			for (i = 2; i < MAX_LEN_OF_MAC_TABLE; i++)
				AsicDelWcidTab(pAd, i);
		} else {
			;/*AsicDelWcidTab(pAd, WCID_ALL);*/
		}
	}

	pAd->MacTab.Content[0].Addr[0] = 0x01;
	pAd->MacTab.Content[0].HTPhyMode.field.MODE = MODE_OFDM;
	pAd->MacTab.Content[0].HTPhyMode.field.MCS = 3;
#ifdef CONFIG_MULTI_CHANNEL
	SetCommonHT(pAd);

/*In MCC  & p2p GO not support VHT now, */
/*change here for support P2P GO 40 BW*/

/* pAd->CommonCfg.vht_bw = 0; */

	if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_BELOW)
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel - 2;
	else if (pAd->CommonCfg.RegTransmitSetting.field.EXTCHA == EXTCHA_ABOVE)
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel + 2;
	else
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;

	AsicSwitchChannel(pAd, pAd->CommonCfg.CentralChannel, FALSE);
	AsicLockChannel(pAd, pAd->CommonCfg.CentralChannel);
	bbp_set_bw(pAd, wdev->bw);
#endif /* CONFIG_MULTI_CHANNEL */

	/*Currently P2P GO not support VHT*/
	/*SetCommonHT changes the vht_cent_ch */
	/*So AsicBBPAdjust always extends to BW80 When HT40*/
	/*pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;*/

	/*AsicBBPAdjust(pAd);*/
	/* MlmeSetTxPreamble(pAd, (USHORT)pAd->CommonCfg.TxPreamble); */

#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	/* P2P_GO */
	MlmeUpdateTxRates(pAd, FALSE, mbssidIdx + MIN_NET_DEVICE_FOR_CFG80211_VIF_P2P_GO);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */

	/*AP */
#ifdef RT_CFG80211_P2P_SUPPORT
	if (!RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
#endif /* endif */
		MlmeUpdateTxRates(pAd, FALSE, MIN_NET_DEVICE_FOR_MBSSID);

	if (WMODE_CAP_N(pAd->CommonCfg.PhyMode))
		MlmeUpdateHtTxRates(pAd, MIN_NET_DEVICE_FOR_MBSSID);

	/* Disable Protection first. */
	if (!INFRA_ON(pAd))
		AsicUpdateProtect(pAd, 0, (ALLN_SETPROTECT | CCKSETPROTECT | OFDMSETPROTECT), TRUE,
				  FALSE);

	APUpdateCapabilityAndErpIe(pAd);
	APUpdateOperationMode(pAd);
	CFG80211_UpdateBeacon(pAd, pNetDev, pBeacon->beacon_head,
			      pBeacon->beacon_head_len, pBeacon->beacon_tail,
			      pBeacon->beacon_tail_len, TRUE);

#ifdef RTMP_MAC_USB
	RTUSBBssBeaconInit(pAd);
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_USB
#ifdef CONFIG_STA_SUPPORT
	RTMPInitTimer(pAd, &pAd->CommonCfg.BeaconUpdateTimer, GET_TIMER_FUNCTION(BeaconUpdateExec),
		      pAd, TRUE);
#endif /*CONFIG_STA_SUPPORT */
	RTUSBBssBeaconStart(pAd);
#endif /* RTMP_MAC_USB */

	/* Enable BSS Sync */
#ifdef CONFIG_MULTI_CHANNEL
	if (INFRA_ON(pAd)) {
		LONG timeDiff;
		INT starttime = pAd->Mlme.channel_1st_staytime;

		NdisGetSystemUpTime(&pAd->Mlme.BeaconNow32);

		/* BPtoJiffies = (((pAd->CommonCfg.BeaconPeriod * 1024 / 1000) * OS_HZ) / 1000); */
		timeDiff =
		    (pAd->Mlme.BeaconNow32 -
		     pAd->StaCfg.LastBeaconRxTime) % (pAd->CommonCfg.BeaconPeriod);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("#####pAd->Mlme.Now32 %ld pAd->StaCfg.LastBeaconRxTime %ld\n",
			  pAd->Mlme.BeaconNow32, pAd->StaCfg.LastBeaconRxTime));
		DBGPRINT(RT_DEBUG_TRACE, ("####    timeDiff %ld\n", timeDiff));
		if (starttime > timeDiff)
			timeDiff = starttime - timeDiff;
		else
			timeDiff = starttime + (pAd->CommonCfg.BeaconPeriod - timeDiff);

		OS_WAIT(timeDiff);
	}
#endif /* CONFIG_MULTI_CHANNEL */

	AsicEnableApBssSync(pAd);
	/* pAd->P2pCfg.bSentProbeRSP = TRUE; */

	AsicSetPreTbtt(pAd, TRUE);

#ifdef RTMP_MAC_USB
	/*
	 * Support multiple BulkIn IRP,
	 * the value on pAd->CommonCfg.NumOfBulkInIRP may be large than 1.
	 */
	for (num_idx = 0; num_idx < pAd->CommonCfg.NumOfBulkInIRP; num_idx++)
		RTUSBBulkReceive(pAd);
#endif /* RTMP_MAC_USB */

	OPSTATUS_SET_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED);
	RTMP_IndicateMediaState(pAd, NdisMediaStateConnected);
#ifdef RT_CFG80211_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
	if (!RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
#endif /*RT_CFG80211_P2P_SUPPORT */
		wdev->Hostapd = Hostapd_CFG;
#endif /*RT_CFG80211_SUPPORT */
	return TRUE;
}

BOOLEAN CFG80211DRV_ApKeyDel(VOID *pAdOrg, VOID *pData)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_KEY *pKeyInfo;
	MAC_TABLE_ENTRY *pEntry;
	INT mbssidIdx = 0;

	pKeyInfo = (CMD_RTPRIV_IOCTL_80211_KEY *) pData;
	mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdOrg, pKeyInfo->pNetDev);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)
	if (pKeyInfo->bPairwise == FALSE)
#else
	if (pKeyInfo->KeyId > 0)
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37) */
		{
		DBGPRINT(RT_DEBUG_TRACE, ("CFG: AsicRemoveSharedKeyEntry %d\n", pKeyInfo->KeyId));
		AsicRemoveSharedKeyEntry(pAd, mbssidIdx, pKeyInfo->KeyId);
	} else {
		pEntry = MacTableLookup(pAd, pKeyInfo->MAC);

		if (pEntry && (pEntry->Aid != 0)) {
			NdisZeroMemory(&pEntry->PairwiseKey, sizeof(CIPHER_KEY));
			AsicRemovePairwiseKeyEntry(pAd, (UCHAR) pEntry->Aid);
		}
	}

	return TRUE;
}

VOID CFG80211DRV_RtsThresholdAdd(VOID *pAdOrg, UINT threshold)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;

	if ((threshold > 0) && (threshold <= MAX_RTS_THRESHOLD))
		pAd->CommonCfg.RtsThreshold = (USHORT) threshold;
#ifdef CONFIG_STA_SUPPORT
	else if (threshold == 0)
		pAd->CommonCfg.RtsThreshold = MAX_RTS_THRESHOLD;
#endif /* CONFIG_STA_SUPPORT */
}

VOID CFG80211DRV_FragThresholdAdd(VOID *pAdOrg, UINT threshold)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;

	if (threshold > MAX_FRAG_THRESHOLD || threshold < MIN_FRAG_THRESHOLD) {
		/*Illegal FragThresh so we set it to default */
		pAd->CommonCfg.FragmentThreshold = MAX_FRAG_THRESHOLD;
	} else if (threshold % 2 == 1) {
		/*
		   The length of each fragment shall always be an even number of octets,
		   except for the last fragment of an MSDU or MMPDU, which may be either
		   an even or an odd number of octets.
		 */
		pAd->CommonCfg.FragmentThreshold = (USHORT) (threshold - 1);
	} else {
		pAd->CommonCfg.FragmentThreshold = (USHORT) threshold;
	}

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		if (pAd->CommonCfg.FragmentThreshold == MAX_FRAG_THRESHOLD)
			pAd->CommonCfg.bUseZeroToDisableFragment = TRUE;
		else
			pAd->CommonCfg.bUseZeroToDisableFragment = FALSE;
	}
#endif /* CONFIG_STA_SUPPORT */
}

BOOLEAN CFG80211DRV_ApKeyAdd(VOID *pAdOrg, VOID *pData)
{
#ifdef CONFIG_AP_SUPPORT
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdOrg;
	CMD_RTPRIV_IOCTL_80211_KEY *pKeyInfo;
	MAC_TABLE_ENTRY *pEntry = NULL;
	PMULTISSID_STRUCT pMbss = NULL;
	struct wifi_dev *pWdev = NULL;
	UINT8 Wcid;
	INT mbssidIdx = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("%s =====>\n", __func__));
	pKeyInfo = (CMD_RTPRIV_IOCTL_80211_KEY *) pData;
	mbssidIdx = RTMP_CFG80211_GET_MBSSID_INDEX(pAdOrg, pKeyInfo->pNetDev);
	pMbss = &pAd->ApCfg.MBSSID[mbssidIdx];
	pWdev = &pMbss->wdev;

	if (pKeyInfo->KeyType == RT_CMD_80211_KEY_WEP40
	    || pKeyInfo->KeyType == RT_CMD_80211_KEY_WEP104) {
		pWdev->WepStatus = Ndis802_11WEPEnabled;
		{
			CIPHER_KEY *pSharedKey;
			POS_COOKIE pObj;

			pObj = (POS_COOKIE) pAd->OS_Cookie;

			pSharedKey = &pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId];
			NdisMoveMemory(pSharedKey->Key, pKeyInfo->KeyBuf, pKeyInfo->KeyLen);

			if (pKeyInfo->KeyType == RT_CMD_80211_KEY_WEP40)
				pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].CipherAlg
					= CIPHER_WEP64;
			else
				pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].CipherAlg
					= CIPHER_WEP128;

			AsicAddSharedKeyEntry(pAd, mbssidIdx, pKeyInfo->KeyId, pSharedKey);
		}
	} else if (pKeyInfo->KeyType == RT_CMD_80211_KEY_WPA) {
		if (pKeyInfo->cipher == Ndis802_11AESEnable) {
			/* AES */
			/* pWdev->WepStatus = Ndis802_11Encryption3Enabled; */
			/* pWdev->GroupKeyWepStatus = Ndis802_11Encryption3Enabled; */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
			if (pKeyInfo->bPairwise == FALSE)
#else
			if (pKeyInfo->KeyId > 0)
#endif /* LINUX_VERSION_CODE 2.6.37 */
				{
				if (pWdev->GroupKeyWepStatus == Ndis802_11Encryption3Enabled) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("CFG: Set AES Security Set. (GROUP) %d\n",
						  pKeyInfo->KeyLen));
					pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].KeyLen =
					    LEN_TK;
					NdisMoveMemory(pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].
						       Key, pKeyInfo->KeyBuf, pKeyInfo->KeyLen);

					pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].CipherAlg =
					    CIPHER_AES;

					AsicAddSharedKeyEntry(pAd, mbssidIdx, pKeyInfo->KeyId,
							      &pAd->SharedKey[mbssidIdx]
							      [pKeyInfo->KeyId]);

					GET_GroupKey_WCID(pAd, Wcid, mbssidIdx);
					RTMPSetWcidSecurityInfo(pAd, mbssidIdx,
								(UINT8) (pKeyInfo->KeyId),
								pAd->SharedKey[mbssidIdx]
								[pKeyInfo->KeyId].CipherAlg, Wcid,
								SHAREDKEYTABLE);

				}
			} else {
				pEntry = MacTableLookup(pAd, pKeyInfo->MAC);

				if (pEntry) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("CFG: Set AES Security Set. (PAIRWISE) %d\n",
						  pKeyInfo->KeyLen));
					pEntry->PairwiseKey.KeyLen = LEN_TK;
					NdisCopyMemory(&pEntry->PTK[OFFSET_OF_PTK_TK],
						       pKeyInfo->KeyBuf, OFFSET_OF_PTK_TK);
					NdisMoveMemory(pEntry->PairwiseKey.Key,
						       &pEntry->PTK[OFFSET_OF_PTK_TK],
						       pKeyInfo->KeyLen);
					pEntry->PairwiseKey.CipherAlg = CIPHER_AES;

					AsicAddPairwiseKeyEntry(pAd, (UCHAR) pEntry->Aid,
								&pEntry->PairwiseKey);
					RTMPSetWcidSecurityInfo(pAd, pEntry->apidx,
								(UINT8) (pKeyInfo->KeyId & 0x0fff),
								pEntry->PairwiseKey.CipherAlg,
								pEntry->Aid, PAIRWISEKEYTABLE);

#ifdef RT_CFG80211_AP_SUPPORT
					if (mbssidIdx == SECOND_MBSSID &&
						pAd->cfg80211_ctrl.ap_mac_is_changed) {
						AsicAddPairwiseKeyEntry(pAd,
							(UCHAR) pEntry->wcid + 1,
							&pEntry->PairwiseKey);
						RTMPSetWcidSecurityInfo(pAd,
							pEntry->apidx,
							(UINT8) (pKeyInfo->KeyId & 0x0fff),
							pEntry->PairwiseKey.CipherAlg,
							pEntry->wcid + 1, PAIRWISEKEYTABLE);
					}
#endif /* RT_CFG80211_AP_SUPPORT */

#ifdef CONFIG_MULTI_CHANNEL
					DBGPRINT(RT_DEBUG_TRACE,
						 ("%s: InfraCh=%d, pWdev->channel=%d\n",
						  __func__, pAd->MlmeAux.InfraChannel,
						  pWdev->channel));
					if (INFRA_ON(pAd)
					    && (((pAd->StaCfg.wdev.bw == pWdev->bw)
						 && (pAd->StaCfg.wdev.channel != pWdev->channel))
						|| !((pAd->StaCfg.wdev.bw == pWdev->bw)
						     &&
						     ((pAd->StaCfg.wdev.channel ==
						       pWdev->channel))))) {
						/*wait 1 s  DHCP  for P2P CLI */
						OS_WAIT(1000);
						DBGPRINT(RT_DEBUG_TRACE,
							 ("OS WAIT 1000 FOR DHCP\n"));
						pAd->MCC_GOConnect_Protect = FALSE;
						pAd->MCC_GOConnect_Count = 0;
						Start_MCC(pAd);
						DBGPRINT(RT_DEBUG_ERROR, ("infra => GO test\n"));
					} else if ((pAd->StaCfg.wdev.bw != pWdev->bw)
						   &&
						   ((pAd->StaCfg.wdev.channel == pWdev->channel))) {
						DBGPRINT(RT_DEBUG_TRACE, ("start bw !=  && SCC\n"));
						pAd->Mlme.bStartScc = TRUE;
					}
					/*after p2p cli connect , neet to change to default configure */
					/* DBGPRINT(RT_DEBUG_TRACE, ("iversontest pWdev->bw %d\n",pWdev->bw)); */
					if (pWdev->bw == 0) {
						pAd->CommonCfg.RegTransmitSetting.field.EXTCHA =
						    EXTCHA_BELOW;
						pAd->CommonCfg.RegTransmitSetting.field.BW = BW_40;
						pAd->CommonCfg.HT_Disable = 0;
						SetCommonHT(pAd);
					}
#endif /* CONFIG_MULTI_CHANNEL */
				} else {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("CFG: Set AES Security Set. (PAIRWISE) But pEntry NULL\n"));
				}

			}
		} else if (pKeyInfo->cipher == Ndis802_11TKIPEnable) {
			/* TKIP */
			/* pWdev->WepStatus = Ndis802_11Encryption2Enabled; */
			/* pWdev->GroupKeyWepStatus = Ndis802_11Encryption2Enabled; */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
			if (pKeyInfo->bPairwise == FALSE)
#else
			if (pKeyInfo->KeyId > 0)
#endif /* LINUX_VERSION_CODE 2.6.37 */
				{
				if (pWdev->GroupKeyWepStatus == Ndis802_11Encryption2Enabled) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("CFG: Set TKIP Security Set. (GROUP) %d\n",
						  pKeyInfo->KeyLen));
					pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].KeyLen =
					    LEN_TK;
					NdisMoveMemory(pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].
						       Key, pKeyInfo->KeyBuf, pKeyInfo->KeyLen);

					pAd->SharedKey[mbssidIdx][pKeyInfo->KeyId].CipherAlg =
					    CIPHER_TKIP;

					AsicAddSharedKeyEntry(pAd, mbssidIdx, pKeyInfo->KeyId,
							      &pAd->SharedKey[mbssidIdx]
							      [pKeyInfo->KeyId]);

					GET_GroupKey_WCID(pAd, Wcid, mbssidIdx);
					RTMPSetWcidSecurityInfo(pAd, mbssidIdx,
								(UINT8) (pKeyInfo->KeyId),
								pAd->SharedKey[mbssidIdx]
								[pKeyInfo->KeyId].CipherAlg, Wcid,
								SHAREDKEYTABLE);

				}
			} else {
				pEntry = MacTableLookup(pAd, pKeyInfo->MAC);

				if (pEntry) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("CFG: Set TKIP Security Set. (PAIRWISE) %d\n",
						  pKeyInfo->KeyLen));
					pEntry->PairwiseKey.KeyLen = LEN_TK;
					NdisCopyMemory(&pEntry->PTK[OFFSET_OF_PTK_TK],
						       pKeyInfo->KeyBuf, OFFSET_OF_PTK_TK);
					NdisMoveMemory(pEntry->PairwiseKey.Key,
						       &pEntry->PTK[OFFSET_OF_PTK_TK],
						       pKeyInfo->KeyLen);
					pEntry->PairwiseKey.CipherAlg = CIPHER_TKIP;

					AsicAddPairwiseKeyEntry(pAd, (UCHAR) pEntry->Aid,
								&pEntry->PairwiseKey);
					RTMPSetWcidSecurityInfo(pAd, pEntry->apidx,
								(UINT8) (pKeyInfo->KeyId & 0x0fff),
								pEntry->PairwiseKey.CipherAlg,
								pEntry->Aid, PAIRWISEKEYTABLE);
				} else {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("CFG: Set TKIP Security Set. (PAIRWISE) But pEntry NULL\n"));
				}

			}
		}

	}
#endif /* CONFIG_AP_SUPPORT */
	return TRUE;

}

INT CFG80211_setApDefaultKey(IN VOID *pAdCB, IN INT *pData, IN UINT Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdCB;
	INT mbssidIdx = *pData;

	DBGPRINT(RT_DEBUG_TRACE, ("Set Ap Default Key: %d\n", Data));
	pAd->ApCfg.MBSSID[mbssidIdx].wdev.DefaultKeyId = Data;
	return 0;
}

INT CFG80211_ApStaDelSendEvent(PRTMP_ADAPTER pAd, const PUCHAR mac_addr)
{
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	PNET_DEV pNetDev = NULL;

	if (pAd->cfg80211_ctrl.Cfg80211VifDevSet.vifDevList.size > 0) {
		pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd,
				RT_CMD_80211_IFTYPE_P2P_GO);
		if (!pNetDev)
			pNetDev = RTMP_CFG80211_FindVifEntry_ByType(pAd,
				RT_CMD_80211_IFTYPE_AP);
		if (pNetDev) {
			DBGPRINT(RT_DEBUG_TRACE,
				 ("GO NOITFY THE CLIENT Disconnected\n"));
			CFG80211OS_DelSta(pNetDev, mac_addr);
		}
	} else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
	{
		DBGPRINT(RT_DEBUG_TRACE,
			 ("SINGLE_DEVICE CFG : GO NOITFY THE CLIENT Disconnected\n"));
		CFG80211OS_DelSta(pAd->net_dev, mac_addr);
	}

	return 0;
}

#endif /* CONFIG_AP_SUPPORT */

INT CFG80211_StaPortSecured(RTMP_ADAPTER *pAd, const UCHAR *mac, UINT flag)
{
	MAC_TABLE_ENTRY *pEntry = MacTableLookup(pAd, mac);

	if (!pEntry) {
		DBGPRINT(RT_DEBUG_ERROR, ("Can't find pEntry in %s\n", __func__));
	} else {
		if (flag) {
			pEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
			pEntry->WpaState = AS_PTKINITDONE;
			pEntry->PortSecured = WPA_802_1X_PORT_SECURED;
			DBGPRINT(RT_DEBUG_TRACE, ("AID:%d, PortSecured\n", pEntry->Aid));
		} else {
			pEntry->PrivacyFilter = Ndis802_11PrivFilter8021xWEP;
			pEntry->PortSecured = WPA_802_1X_PORT_NOT_SECURED;
			DBGPRINT(RT_DEBUG_TRACE, ("AID:%d, PortNotSecured\n", pEntry->Aid));
		}
	}

	return 0;
}

INT CFG80211_ApStaDel(RTMP_ADAPTER *pAd, const UCHAR *mac)
{
	UCHAR pMacF[MAC_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	if (mac == NULL || MAC_ADDR_EQUAL(mac, pMacF)) {
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
		/* From WCID=2 */
		if (INFRA_ON(pAd))
			P2PMacTableReset(pAd);
		else
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
			MacTableReset(pAd);
	} else {
		MAC_TABLE_ENTRY *pEntry = MacTableLookup(pAd, mac);

		if (pEntry)
			MlmeDeAuthAction(pAd, pEntry, REASON_NO_LONGER_VALID, FALSE);
		else
			DBGPRINT(RT_DEBUG_ERROR, ("Can't find pEntry in ApStaDel\n"));
	}

	return 0;
}

#endif /* RT_CFG80211_SUPPORT */
