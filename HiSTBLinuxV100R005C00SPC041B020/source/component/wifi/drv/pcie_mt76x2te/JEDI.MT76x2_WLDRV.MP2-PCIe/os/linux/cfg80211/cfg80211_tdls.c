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

	All related CFG80211 TDLS function body.

	History:

***************************************************************************/

#define RTMP_MODULE_OS

#ifdef RT_CFG80211_SUPPORT

#include "rt_config.h"

#ifdef CFG_TDLS_SUPPORT

#define	TDLS_AKM_SUITE_PSK			7
UCHAR	CipherSuiteTDLSWpa2PskAes[] = {
		0x30,					// RSN IE
		0x14,					// Length
		0x01, 0x00,				// Version
		0x00, 0x0F, 0xAC, 0x07,	// no group cipher
		0x01, 0x00,				// number of pairwise
		0x00, 0x0f, 0xAC, 0x04,	// unicast, AES
		0x01, 0x00,				// number of authentication method
		0x00, 0x0f, 0xAC, TDLS_AKM_SUITE_PSK,	// TDLS authentication
		0x00, 0x02,				// RSN capability, peer key enabled
		};
UCHAR	CipherSuiteTDLSLen = sizeof(CipherSuiteTDLSWpa2PskAes)/ sizeof(UCHAR);

typedef struct
{
	UCHAR	regclass;		// regulatory class
	UCHAR	spacing;		// 0: 20Mhz, 1: 40Mhz
	UCHAR	channelset[16];	// max 15 channels, use 0 as terminator
} CFG_REG_CLASS;

CFG_REG_CLASS cfg_reg_class[] =
{
	{  1,  0, {36, 40, 44, 48, 0}},
	{  2,  0, {52, 56, 60, 64, 0}},
	{  3,  0, {149, 153, 157, 161, 0}},
	{  4,  0, {100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 0}},
	{  5,  0, {165, 0}},
	{ 12, 0, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0}},
	{ 22, 1, {36, 44, 0}},
	{ 23, 1, {52, 60, 0}},
	{ 24, 1, {100, 108, 116, 124, 132, 0}},
	{ 25, 1, {149, 157, 0}},
	{ 26, 1, {149, 157, 0}},
	{ 27, 1, {40, 48, 0}},
	{ 28, 1, {56, 64, 0}},
	{ 29, 1, {104, 112, 120, 128, 136, 0}},
	{ 30, 1, {153, 161, 0}},
	{ 31, 1, {153, 161, 0}},
	{ 32, 1, {1, 2, 3, 4, 5, 6, 7, 0}},
	{ 33, 1, {5, 6, 7, 8, 9, 10, 11, 0}},
	{ 0,   0, {0}}			// end
};

UCHAR cfg_tdls_GetRegulatoryClass(
	IN PRTMP_ADAPTER pAd,
	IN UCHAR	ChannelWidth,
	IN UCHAR	TargetChannel)
{
	int i = 0;
	int j = 0;
	UCHAR regclass = 0;

	do
	{
		if (cfg_reg_class[i].spacing == ChannelWidth)
		{
			do
			{
				if (cfg_reg_class[i].channelset[j] == TargetChannel)
				{
					regclass = cfg_reg_class[i].regclass;
					break;
				}
				j++;
			} while (cfg_reg_class[i].channelset[j] != 0);
		}
		i++;
	} while (cfg_reg_class[i].regclass != 0);

	//ASSERT(regclass);

	return regclass;
}

VOID cfg_tdls_adjust_channel(
	IN	PRTMP_ADAPTER				pAd,
	IN  UINT8						rf_bw,
	IN  UINT8						ext_ch,
	IN 	UCHAR						rf_channel
)
{
	DBGPRINT(RT_DEBUG_OFF, ("%s()!!!!\n", __func__));
	pAd->CommonCfg.vht_bw = VHT_BW_80;
	pAd->CommonCfg.vht_cent_ch = vht_cent_ch_freq(pAd, pAd->CommonCfg.Channel);
	if ((pAd->CommonCfg.CentralChannel > pAd->CommonCfg.Channel))
	{
		ext_ch = EXTCHA_ABOVE;
	}
	else if ((pAd->CommonCfg.CentralChannel < pAd->CommonCfg.Channel))
	{
		ext_ch = EXTCHA_BELOW;
	}
	else
	{
		ext_ch = EXTCHA_ABOVE;  //default above
	}

	rf_channel = pAd->CommonCfg.vht_cent_ch;
	bbp_set_bw(pAd, rf_bw);
	bbp_set_ctrlch(pAd, ext_ch);
	rtmp_mac_set_ctrlch(pAd, ext_ch);

	AsicSetChannel(pAd, rf_channel, rf_bw, ext_ch, FALSE);

	DBGPRINT(RT_DEBUG_OFF,
				 ("%s():BW_%s, RF-Ch=%d, CtrlCh=%d, HT-CentralCh=%d\n",
				__func__, (rf_bw == BW_80 ? "80" : (rf_bw == BW_40 ? "40" : "20")),
				pAd->LatchRfRegs.Channel,
				pAd->CommonCfg.Channel,
				pAd->CommonCfg.CentralChannel));
#ifdef DOT11_VHT_AC
	DBGPRINT(RT_DEBUG_OFF, ("VHT-CentralCh=%d\n", pAd->CommonCfg.vht_cent_ch));
#endif /* DOT11_VHT_AC */
}

VOID InitPeerEntryRateCapability(
	IN	PRTMP_ADAPTER pAd,
	IN	MAC_TABLE_ENTRY *pEntry,
	IN USHORT *pCapabilityInfo,
	IN UCHAR SupportRateLens,
	IN UCHAR *pSupportRates,
#ifdef DOT11_VHT_AC
	IN VHT_CAP_IE 				*pVhtCap,
	IN UCHAR					VhtCapLen,
	IN VHT_OP_IE				*pVhtOp,
	IN UCHAR					VhtOpLen,
#endif /*DOT11_VHT_AC*/
	IN UCHAR HtCapabilityLen,
	IN HT_CAPABILITY_IE *pHtCapability
	)
{
	UCHAR MaxSupportedRate = RATE_11;
	UCHAR MaxSupportedRateIn500Kbps = 0;
	UCHAR idx;

    for (idx = 0; idx < SupportRateLens; idx++)
    {
        if (MaxSupportedRateIn500Kbps < (pSupportRates[idx] & 0x7f))
            MaxSupportedRateIn500Kbps = pSupportRates[idx] & 0x7f;
    }

	switch (MaxSupportedRateIn500Kbps)
	{
		case 108: MaxSupportedRate = RATE_54;   break;
		case 96:  MaxSupportedRate = RATE_48;   break;
		case 72:  MaxSupportedRate = RATE_36;   break;
		case 48:  MaxSupportedRate = RATE_24;   break;
		case 36:  MaxSupportedRate = RATE_18;   break;
		case 24:  MaxSupportedRate = RATE_12;   break;
		case 18:  MaxSupportedRate = RATE_9;    break;
		case 12:  MaxSupportedRate = RATE_6;    break;
		case 22:  MaxSupportedRate = RATE_11;   break;
		case 11:  MaxSupportedRate = RATE_5_5;  break;
		case 4:   MaxSupportedRate = RATE_2;    break;
		case 2:   MaxSupportedRate = RATE_1;    break;
		default:  MaxSupportedRate = RATE_11;   break;
	}

	pEntry->MaxSupportedRate = min(pAd->CommonCfg.MaxDesiredRate, MaxSupportedRate);

	if (pEntry->MaxSupportedRate < RATE_FIRST_OFDM_RATE)
	{
		pEntry->MaxHTPhyMode.field.MODE = MODE_CCK;
		pEntry->MaxHTPhyMode.field.MCS = pEntry->MaxSupportedRate;
		pEntry->MinHTPhyMode.field.MODE = MODE_CCK;
		pEntry->MinHTPhyMode.field.MCS = pEntry->MaxSupportedRate;
		pEntry->HTPhyMode.field.MODE = MODE_CCK;
		pEntry->HTPhyMode.field.MCS = pEntry->MaxSupportedRate;
	}
	else
	{
		pEntry->MaxHTPhyMode.field.MODE = MODE_OFDM;
		pEntry->MaxHTPhyMode.field.MCS = OfdmRateToRxwiMCS[pEntry->MaxSupportedRate];
		pEntry->MinHTPhyMode.field.MODE = MODE_OFDM;
		pEntry->MinHTPhyMode.field.MCS = OfdmRateToRxwiMCS[pEntry->MaxSupportedRate];
		pEntry->HTPhyMode.field.MODE = MODE_OFDM;
		pEntry->HTPhyMode.field.MCS = OfdmRateToRxwiMCS[pEntry->MaxSupportedRate];
	}

	pEntry->MaxHTPhyMode.field.BW = BW_20;
	pEntry->MinHTPhyMode.field.BW = BW_20;

	pEntry->HTCapability.MCSSet[0] = 0;
	pEntry->HTCapability.MCSSet[1] = 0;
	pEntry->HTCapability.MCSSet[2] = 0;

	/* If this Entry supports 802.11n, upgrade to HT rate. */
	if ((HtCapabilityLen != 0) && (pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED))
	{
		UCHAR	j, bitmask; /*k,bitmask; */
		CHAR    ii;

		DBGPRINT(RT_DEBUG_ERROR,
				("TDLS - Receive Peer HT Capable STA from %02x:%02x:%02x:%02x:%02x:%02x\n",
				PRINT_MAC(pEntry->Addr)));

		if ((pHtCapability->HtCapInfo.GF) &&
			(pAd->CommonCfg.DesiredHtPhy.GF) &&
			(pAd->StaActive.SupportedHtPhy.GF))
		{
			pEntry->MaxHTPhyMode.field.MODE = MODE_HTGREENFIELD;
		}
		else
		{
			pEntry->MaxHTPhyMode.field.MODE = MODE_HTMIX;
			pAd->MacTab.fAnyStationNonGF = TRUE;
			pAd->CommonCfg.AddHTInfo.AddHtInfo2.NonGfPresent = 1;
		}

		if ((pHtCapability->HtCapInfo.ChannelWidth) &&
			(pAd->CommonCfg.DesiredHtPhy.ChannelWidth))
		{
			pEntry->MaxHTPhyMode.field.BW= BW_40;
			pEntry->MaxHTPhyMode.field.ShortGI = ((pAd->CommonCfg.DesiredHtPhy.ShortGIfor40)&(pHtCapability->HtCapInfo.ShortGIfor40));
		}
		else
		{
			pEntry->MaxHTPhyMode.field.BW = BW_20;
			pEntry->MaxHTPhyMode.field.ShortGI = ((pAd->CommonCfg.DesiredHtPhy.ShortGIfor20)&(pHtCapability->HtCapInfo.ShortGIfor20));
			pAd->MacTab.fAnyStation20Only = TRUE;
		}

		/* find max fixed rate */
		for (ii = 23; ii >= 0; ii--) /* 3*3 */
		{
			j = ii/8;
			bitmask = (1<<(ii-(j*8)));
			if ( (pAd->StaCfg.wdev.DesiredHtPhyInfo.MCSSet[j]&bitmask) && (pHtCapability->MCSSet[j]&bitmask))
			{
				pEntry->MaxHTPhyMode.field.MCS = ii;
				break;
			}

			if (ii==0)
				break;
		}

		if (pAd->StaCfg.wdev.DesiredTransmitSetting.field.MCS != MCS_AUTO)
		{
			if (pAd->StaCfg.wdev.DesiredTransmitSetting.field.MCS == 32)
			{
				/* Fix MCS as HT Duplicated Mode */
				pEntry->MaxHTPhyMode.field.BW = 1;
				pEntry->MaxHTPhyMode.field.MODE = MODE_HTMIX;
				pEntry->MaxHTPhyMode.field.STBC = 0;
				pEntry->MaxHTPhyMode.field.ShortGI = 0;
				pEntry->MaxHTPhyMode.field.MCS = 32;
			}
			else if (pEntry->MaxHTPhyMode.field.MCS > pAd->StaCfg.wdev.HTPhyMode.field.MCS)
			{
				/* STA supports fixed MCS */
				pEntry->MaxHTPhyMode.field.MCS = pAd->StaCfg.wdev.HTPhyMode.field.MCS;
			}
		}

		pEntry->MaxHTPhyMode.field.STBC = (pHtCapability->HtCapInfo.RxSTBC & (pAd->CommonCfg.DesiredHtPhy.TxSTBC));
		pEntry->MpduDensity = pHtCapability->HtCapParm.MpduDensity;
		pEntry->MaxRAmpduFactor = pHtCapability->HtCapParm.MaxRAmpduFactor;
		pEntry->MmpsMode = (UCHAR)pHtCapability->HtCapInfo.MimoPs;
		pEntry->AMsduSize = (UCHAR)pHtCapability->HtCapInfo.AMsduSize;
		pEntry->HTPhyMode.word = pEntry->MaxHTPhyMode.word;

		if (pHtCapability->HtCapInfo.ShortGIfor20)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_SGI20_CAPABLE);
		if (pHtCapability->HtCapInfo.ShortGIfor40)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_SGI40_CAPABLE);
		if (pHtCapability->HtCapInfo.TxSTBC)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_TxSTBC_CAPABLE);
		if (pHtCapability->HtCapInfo.RxSTBC)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_RxSTBC_CAPABLE);
		if (pHtCapability->ExtHtCapInfo.PlusHTC)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_HTC_CAPABLE);
		if (pAd->CommonCfg.bRdg && pHtCapability->ExtHtCapInfo.RDGSupport)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_RDG_CAPABLE);
		if (pHtCapability->ExtHtCapInfo.MCSFeedback == 0x03)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_MCSFEEDBACK_CAPABLE);

		NdisMoveMemory(&pEntry->HTCapability, pHtCapability, sizeof(HT_CAPABILITY_IE));
		CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE);
	}
	else
	{
		NdisZeroMemory(&pEntry->HTCapability, sizeof(HT_CAPABILITY_IE));
		CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE);
		DBGPRINT(RT_DEBUG_OFF, ("TDLS - Receive Peer Legacy STA \n"));
	}

	pEntry->HTPhyMode.word = pEntry->MaxHTPhyMode.word;

	if ((pHtCapability->HtCapInfo.ChannelWidth) &&
		(pAd->CommonCfg.DesiredHtPhy.ChannelWidth) &&
		(pAd->StaActive.SupportedHtPhy.ChannelWidth))
	{
		pEntry->HTPhyMode.field.BW= BW_40;
	}
	else
	{
		pEntry->HTPhyMode.field.BW= BW_20;
	}


	pEntry->CurrTxRate = pEntry->MaxSupportedRate;

	if (pAd->StaCfg.wdev.bAutoTxRateSwitch == TRUE)
	{
		PUCHAR pTable;
		UCHAR TableSize = 0;

		MlmeSelectTxRateTable(pAd, pEntry, &pTable, &TableSize, &pEntry->CurrTxRateIndex);
		pEntry->bAutoTxRateSwitch = TRUE;
	}
	else
	{
		pEntry->HTPhyMode.field.MODE	= pAd->StaCfg.wdev.HTPhyMode.field.MODE;
		pEntry->HTPhyMode.field.MCS	= pAd->StaCfg.wdev.HTPhyMode.field.MCS;
		pEntry->bAutoTxRateSwitch = FALSE;

		RTMPUpdateLegacyTxSetting((UCHAR)pAd->StaCfg.wdev.DesiredTransmitSetting.field.FixedTxMode, pEntry);
	}

	pEntry->RateLen = SupportRateLens;

#ifdef DOT11_VHT_AC
	if (pVhtCap == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: %d VHT CAP is null\n", __func__, __LINE__));
		return;
	}
	if (WMODE_CAP_AC(pAd->CommonCfg.PhyMode) &&
		(pAd->CommonCfg.Channel > 14) && (VhtCapLen != 0) &&
		(pAd->CommonCfg.BBPCurrentBW == BW_80))
	{
		VHT_CAP_INFO *vht_cap_info = (VHT_CAP_INFO *)pVhtCap;
		VHT_OP_INFO *vht_op_info = (VHT_OP_INFO *)pVhtOp;

		//TDLS link up & peer support 80M , switch to AC
		if((vht_cap_info->sgi_80M==1 || (vht_op_info != NULL && vht_op_info->ch_width >= 1)))
		{
			cfg_tdls_adjust_channel(pAd,BW_80,0,0);
		}

		pEntry->MaxHTPhyMode.field.MODE = MODE_VHT;
		//if ((pEntry->MaxHTPhyMode.field.BW== BW_40) && (pAd->StaCfg.wdev.DesiredHtPhyInfo.vht_bw == VHT_BW_80))
		if(pAd->CommonCfg.BBPCurrentBW == BW_80 &&  (vht_cap_info->sgi_80M==1 || (vht_op_info != NULL && vht_op_info->ch_width >= 1)))
		{
			pEntry->MaxHTPhyMode.field.BW = BW_80;
			pEntry->HTPhyMode.field.BW= BW_80;
			pEntry->HTPhyMode.field.MCS = 7;
		}
		/* TODO: implement get_vht_max_mcs to get peer max MCS */
		if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss1 == VHT_MCS_CAP_9) {
			if ((pEntry->MaxHTPhyMode.field.BW == BW_20))
				pEntry->MaxHTPhyMode.field.MCS = 8;
			else
				pEntry->MaxHTPhyMode.field.MCS = 9;
		} else if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss1 == VHT_MCS_CAP_8) {
			pEntry->MaxHTPhyMode.field.MCS = 8;
		} else if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss1 == VHT_MCS_CAP_7) {
			pEntry->MaxHTPhyMode.field.MCS = 7;
		}

		if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss2 == VHT_MCS_CAP_9) {
			if ((pEntry->MaxHTPhyMode.field.BW == BW_20))
				pEntry->MaxHTPhyMode.field.MCS = ((1 << 4) | 8);
			else
				pEntry->MaxHTPhyMode.field.MCS = ((1 << 4) | 9);
		} else if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss2 == VHT_MCS_CAP_8) {
			pEntry->MaxHTPhyMode.field.MCS = ((1 << 4) | 8);
		} else if (pVhtCap->mcs_set.rx_mcs_map.mcs_ss2 == VHT_MCS_CAP_7) {
			pEntry->MaxHTPhyMode.field.MCS = ((1 << 4) | 7);
		}

		DBGPRINT(RT_DEBUG_OFF, ("%s(): Peer's PhyCap=>Mode:%s, BW:%s\n",
			__func__,
			get_phymode_str(pEntry->MaxHTPhyMode.field.MODE),
			get_bw_str(pEntry->MaxHTPhyMode.field.BW)));
		DBGPRINT(RT_DEBUG_OFF,
				("TDLS - Receive Peer VHT Capable STA from %02x:%02x:%02x:%02x:%02x:%02x\n",
				PRINT_MAC(pEntry->Addr)));

		if (pAd->CommonCfg.vht_ldpc && (pAd->chipCap.phy_caps & fPHY_CAP_LDPC)) {
			if (vht_cap_info->rx_ldpc)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_VHT_RX_LDPC_CAPABLE);
		}

		if (vht_cap_info->sgi_80M)
			CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_SGI80_CAPABLE);

		if (vht_cap_info->sgi_160M)
			CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_SGI160_CAPABLE);

		if (pAd->CommonCfg.vht_stbc)
		{
			if (vht_cap_info->tx_stbc)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_VHT_TXSTBC_CAPABLE);
			if (vht_cap_info->rx_stbc)
				CLIENT_STATUS_SET_FLAG(pEntry, fCLIENT_STATUS_VHT_RXSTBC_CAPABLE);
		}
		NdisCopyMemory(&pEntry->vht_cap_ie, pVhtCap, sizeof(VHT_CAP_IE));
	}

	pEntry->force_op_mode = FALSE;

#endif /* DOT11_VHT_AC */

}

BOOLEAN CFG80211DRV_StaTdlsInsertDeletepEntry(
	VOID						*pAdOrg,
	const VOID					*pData,
	UINT						Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER)pAdOrg;
	UCHAR *peer;
	PMAC_TABLE_ENTRY pMacEntry = NULL;
	PCFG_TDLS_ENTRY pEntry;

	peer = (UCHAR *)pData;
	DBGPRINT(RT_DEBUG_ERROR,
		("=====>  %s() peer: %02x:%02x:%02x:%02x:%02x:%02x, op: %d\n",
		__func__, PRINT_MAC(peer), Data));

	/* pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.IneedKey = 0; */

	if (Data == tdls_insert_entry) {
		int i = 0, tdls_entry_index = 0;

		for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++) {
			pEntry = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i];
			if (MAC_ADDR_EQUAL(peer, pEntry->MacAddr)) {
				tdls_entry_index = i;  /* found prebuild entry */
				break;
			}
		}

		if (i == MAX_NUM_OF_CFG_TDLS_ENTRY) {
			DBGPRINT(RT_DEBUG_ERROR,
				("%s() - EXCEED MAX_NUM_OF_CFG_TDLS_ENTRY %d, CAN'T create entry\n",
				__func__, MAX_NUM_OF_CFG_TDLS_ENTRY));
			return -1;
		}


		/* allocate one MAC entry */
		pMacEntry = MacTableLookup(pAd, peer);

		if (pMacEntry && IS_ENTRY_TDLS(pMacEntry))
			DBGPRINT(RT_DEBUG_ERROR, ("%s() - MacTable Entry exist !!!\n", __func__));
		else
			pMacEntry = MacTableInsertEntry(pAd,
							peer,
							&pAd->StaCfg.wdev,
							BSS0 + MIN_NET_DEVICE_FOR_TDLS,
							OPMODE_STA,
							TRUE);

		if (pMacEntry) {
			/* update tdls link info */
			pEntry->EntryValid = TRUE;
			pEntry->MacTabMatchWCID = pMacEntry->wcid;
			pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsLinkCount++;

			pMacEntry->AuthMode = pAd->StaCfg.wdev.AuthMode;
			pMacEntry->WepStatus = pAd->StaCfg.wdev.WepStatus;
			pMacEntry->PortSecured = WPA_802_1X_PORT_SECURED;
			pMacEntry->Sst = SST_ASSOC;
			pMacEntry->StaIdleTimeout = MAC_TABLE_AGEOUT_TIME;

#ifdef UAPSD_SUPPORT
			/* update UAPSD */
			UAPSD_AssocParse(pAd, pMacEntry,
				&pEntry->QosCapability, pAd->StaCfg.UapsdInfo.bAPSDCapable);
#endif /* UAPSD_SUPPORT */

			DBGPRINT(RT_DEBUG_TRACE,
				("MacTableInsertTDlsEntry - allocate entry #%d, Total= %d\n",
				pMacEntry->Aid, pAd->MacTab.Size));

			/* Set WMM capability */
			if (pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED ||
				pAd->CommonCfg.bWmmCapable) {
				CLIENT_STATUS_SET_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE);
				DBGPRINT(RT_DEBUG_TRACE, ("TDLS -  WMM Capable\n"));
			} else {
				CLIENT_STATUS_CLEAR_FLAG(pMacEntry, fCLIENT_STATUS_WMM_CAPABLE);
			}

			InitPeerEntryRateCapability(pAd, pMacEntry, &pAd->StaActive.CapabilityInfo,
				pAd->StaActive.SupRateLen, pAd->StaActive.SupRate,
#ifdef DOT11_VHT_AC
				&pEntry->tdls_vht_cap, pEntry->tdls_vht_cap_len,
				&pEntry->tdls_vht_op, pEntry->tdls_vht_op_len,
#endif /* DOT11_VHT_AC */
				pEntry->HtCapabilityLen, &pEntry->HtCapability);

			RTMPSetSupportMCS(pAd, OPMODE_STA, pMacEntry,
				pAd->StaActive.SupRate,	pAd->StaActive.SupRateLen,
				pAd->StaActive.ExtRate,	pAd->StaActive.ExtRateLen,
#ifdef DOT11_VHT_AC
				pEntry->tdls_vht_cap_len, &pEntry->tdls_vht_cap,
#endif /* DOT11_VHT_AC */
				&pEntry->HtCapability,
				pEntry->HtCapabilityLen);

			/* Install Peer Key if RSNA Enabled */
			if (pAd->StaCfg.wdev.WepStatus != Ndis802_11EncryptionDisabled) {
				/* Write to ASIC on-chip table. */
				if (pMacEntry->Aid > 1) {
					CIPHER_KEY PairwiseKey;
					NdisZeroMemory(&PairwiseKey, sizeof(CIPHER_KEY));

					PairwiseKey.CipherAlg = CIPHER_AES;

					/* Set Peer Key */
					PairwiseKey.KeyLen = LEN_TK;

					NdisMoveMemory(PairwiseKey.Key, &pEntry->TPK[16], LEN_TK);
					hex_dump("PairwiseKey=",
						(UINT8 *)PairwiseKey.Key, PairwiseKey.KeyLen);
					RTMP_ASIC_PAIRWISE_KEY_TABLE(pAd,
						pMacEntry->Aid,	&PairwiseKey);

					RTMP_SET_WCID_SEC_INFO(pAd, BSS0, 0, PairwiseKey.CipherAlg,
						pMacEntry->Aid,	PAIRWISEKEYTABLE);

					NdisMoveMemory(&pMacEntry->PairwiseKey,
						&PairwiseKey, sizeof(CIPHER_KEY));

					pMacEntry->AuthMode = Ndis802_11AuthModeWPA2PSK;
					pMacEntry->WepStatus = Ndis802_11Encryption3Enabled;
					pMacEntry->PortSecured = WPA_802_1X_PORT_SECURED;
					pMacEntry->PrivacyFilter = Ndis802_11PrivFilterAcceptAll;
				}
			}
		} else {
			DBGPRINT(RT_DEBUG_ERROR, ("%s() - MacTableInsertEntry failed\n", __func__));
			return FALSE;
		}

		cfg_tdls_prepare_null_frame(pAd, 0, 0, peer);
	} else if (Data == tdls_delete_entry) {
		int i = 0;
		pMacEntry = MacTableLookup(pAd, peer);
		if (pMacEntry && IS_ENTRY_TDLS(pMacEntry))
			MacTableDeleteEntry(pAd, pMacEntry->wcid, peer);

		/* clear tdls link info */
		for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++) {
			PCFG_TDLS_STRUCT pTdlsStruct =
				&pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info;
			PCFG_TDLS_ENTRY pTdlsEntry =
				&(pTdlsStruct->TDLSEntry[i]);
			if (pTdlsEntry->EntryValid == TRUE &&
			    MAC_ADDR_EQUAL(peer, pTdlsEntry->MacAddr)) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s() clear TDLS entry %d\n",
					  __func__, i));
				NdisZeroMemory(pTdlsEntry, sizeof(CFG_TDLS_ENTRY));
				pTdlsStruct->TdlsLinkCount--;
				pTdlsEntry->pAd = pAd;
#ifdef UAPSD_SUPPORT
				/* re-init timer for this entry */
				RTMPInitTimer(pAd, &(pTdlsEntry->Timer),
					      GET_TIMER_FUNCTION(cfg_tdls_PTITimeoutAction),
					      pTdlsEntry, FALSE);
#endif /*UAPSD_SUPPORT*/
				break;
			}
		}
	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("%s() op: %d  DO NOTHING\n", __func__, Data));
	}

	DBGPRINT(RT_DEBUG_ERROR, ("<======  %s() out\n", __func__));
	return TRUE;
}

BOOLEAN cfg_tdls_UAPSDP_AsicCanSleep(
	IN	PRTMP_ADAPTER				pAd)
{
	UINT32 i;
	BOOLEAN FlgAllSpClosed = TRUE;
	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));

	for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++)
	{
		PCFG_TDLS_ENTRY pTDLS = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i];
		if (pTDLS->EntryValid == TRUE)
		{
			UINT32 Wcid = pTDLS->MacTabMatchWCID;
			PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[Wcid];

			if (!IS_ENTRY_TDLS(pEntry))
				continue;

			/*
				Two cases we can not sleep:
				1. One of any SP is not ended.
				2. A traffic indication is sent and no response is received.
			*/
			if ((pEntry->bAPSDFlagSPStart != 0) ||
				(pTDLS->FlgIsWaitingUapsdTraRsp == TRUE))
			{
				DBGPRINT(RT_DEBUG_TRACE,
						("tdls uapsd> SP not close or Ind sent (%d %d)!\n",
						pEntry->bAPSDFlagSPStart,
						pTDLS->FlgIsWaitingUapsdTraRsp));
				hex_dump("pEntry=", pEntry->Addr, 6);
				FlgAllSpClosed = FALSE;
				break;
			}
		}
	}

	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));

	return FlgAllSpClosed;
}

VOID cfg_tdls_UAPSDP_PsmModeChange(
	IN	PRTMP_ADAPTER				pAd,
	IN	USHORT						PsmOld,
	IN	USHORT						PsmNew)
{
	MAC_TABLE_ENTRY	*pMacEntry;
	UINT32 IdTdls;
	struct wifi_dev *wdev;
	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));
	if (PsmOld == PsmNew)
		return; /* no inform needs */

	if (pAd == NULL) {
		DBGPRINT(RT_DEBUG_ERROR,
			("%s: !bug, pAd is NULL.\n", __func__));
		return;
	}

	wdev = &pAd->StaCfg.wdev;

	/* sanity check */
    /* WPA 802.1x secured port control */
    if (((wdev->AuthMode == Ndis802_11AuthModeWPA) ||
         (wdev->AuthMode == Ndis802_11AuthModeWPAPSK) ||
         (wdev->AuthMode == Ndis802_11AuthModeWPA2) ||
         (wdev->AuthMode == Ndis802_11AuthModeWPA2PSK)
#ifdef WPA_SUPPLICANT_SUPPORT
		|| (wdev->IEEE8021X == TRUE)
#endif /* endif */
#ifdef WAPI_SUPPORT
		  || (wdev->AuthMode == Ndis802_11AuthModeWAICERT)
		  || (wdev->AuthMode == Ndis802_11AuthModeWAIPSK)
#endif /* WAPI_SUPPORT */
        ) &&
       (wdev->PortSecured == WPA_802_1X_PORT_NOT_SECURED))
	{
		return; /* port not yet secure */
	}

	DBGPRINT(RT_DEBUG_TRACE, ("tdls uapsd> our PSM mode change!\n"));

	/* indicate the peer */


	for(IdTdls=0; IdTdls<MAX_NUM_OF_CFG_TDLS_ENTRY; IdTdls++)
	{
		if (pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[IdTdls].EntryValid == TRUE)
		{
			/* get MAC Entry */
			pMacEntry = &pAd->MacTab.Content[pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[IdTdls].MacTabMatchWCID];
			if (pMacEntry == NULL)
				continue; /* check next one */

			/* check if the peer is in ACTIVE mode */
			if (pMacEntry->PsMode != PWR_SAVE)
			{
				/*
					pMacEntry->RssiSample.LastRssi0 is used to check if
					we have ever received any packet from the peer.
				*/
				/* send a null frame to the peer directly */
				DBGPRINT(RT_DEBUG_TRACE, ("tdls uapsd> send a NULL frame!\n"));

				RtmpEnqueueNullFrame(pAd, pMacEntry->Addr,
									pAd->CommonCfg.TxRate, pMacEntry->Aid,
									pMacEntry->apidx, TRUE, FALSE, 0);
				continue;
			}

			/*
				Send traffic indication frame to the peer when the peer
				is in power-save mode.
			*/
			cfg_tdls_send_PeerTrafficIndication(pAd, pMacEntry->Addr);
		}
	}
	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));
	return;
}

void cfg_tdls_rcv_PeerTrafficIndication(
PRTMP_ADAPTER pAd,
u8 dialog_token,
u8 *peer
)
{
	int tdls_entry_link_index = 0;

	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));

	/* Not TDLS Capable, ignore it */
	if (!IS_TDLS_SUPPORT(pAd))
		return;

	/* Not BSS mode, ignore it */
	if (!INFRA_ON(pAd))
		return;

	/* sanity check */
	if (pAd->StaCfg.Psm != PWR_SAVE)
		return; /* we are not in power-save mode */

	tdls_entry_link_index = cfg_tdls_search_ValidLinkIndex(pAd, peer);
	if (tdls_entry_link_index == -1) {
		DBGPRINT(RT_DEBUG_ERROR,
			("%s(): TDLS link (%02X:%02X:%02X:%02X:%02X:%02X) doesn't exist.\n",
			__func__, PRINT_MAC(peer)));
		return;
	}

	cfg_tlds_build_frame(pAd, peer, dialog_token, TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE,
		0, NULL, 0, TRUE, tdls_entry_link_index, 0);

	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));
	return;
}

int cfg_tdls_rcv_PeerTrafficResponse(PRTMP_ADAPTER pAd, u8 *peer)
{
	int i = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("======>  %s()\n", __func__));

	/* Not TDLS Capable, ignore it */
	if (!IS_TDLS_SUPPORT(pAd))
		return -1;

	/* Not BSS mode, ignore it */
	if (!INFRA_ON(pAd))
		return -1;

	i = cfg_tdls_search_ValidLinkIndex(pAd, peer);
	if (i == -1) {
		DBGPRINT(RT_DEBUG_ERROR,
			("%s(): TDLS link (%02X:%02X:%02X:%02X:%02X:%02X) doesn't exist.\n",
			__func__, PRINT_MAC(peer)));
		return 1;
	}

	pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].FlgIsWaitingUapsdTraRsp = FALSE;

	/* check if we can sleep if we are sleep mode */
	RtmpAsicSleepHandle(pAd);
	DBGPRINT(RT_DEBUG_TRACE, ("<======  %s() out\n", __func__));
	return 0;
}

void cfg_tdls_send_PeerTrafficIndication(PRTMP_ADAPTER pAd, u8 *peer)
{
	int tdls_entry_link_index = 0;
	UINT8 dialog_token = 0;
	BOOLEAN TimerCancelled;
	PCFG_TDLS_ENTRY pEntry;

	DBGPRINT(RT_DEBUG_TRACE, ("======>  %s()\n", __func__));

	if (pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsDialogToken == 0)
		pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsDialogToken = 1;

	dialog_token = pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsDialogToken ++;

	/* Not TDLS Capable, ignore it */
	if (!IS_TDLS_SUPPORT(pAd))
		return;

	/* Not BSS mode, ignore it */
	if (!INFRA_ON(pAd))
		return;

	tdls_entry_link_index = cfg_tdls_search_ValidLinkIndex(pAd, peer);
	if (tdls_entry_link_index == -1) {
		DBGPRINT(RT_DEBUG_ERROR,
			("%s(): TDLS link (%02X:%02X:%02X:%02X:%02X:%02X) doesn't exist.\n",
			__func__, PRINT_MAC(peer)));
		return;
	}

	pEntry = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[tdls_entry_link_index];
	if (pEntry->FlgIsWaitingUapsdTraRsp) {
		DBGPRINT(RT_DEBUG_TRACE,
			("tdls uapsd> traffic ind of index %d was sent before!\n",
			tdls_entry_link_index));
		return; /* has sent it */
	}

	pEntry->FlgIsWaitingUapsdTraRsp = TRUE;

	cfg_tlds_build_frame(pAd, peer, dialog_token, TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION,
		0, NULL, 0, FALSE, tdls_entry_link_index, 0);

	/* 11.2.1.14.1 Peer U-APSD Behavior at the PU buffer STA
	 * When no corresponding TDLS Peer Traffic Response frame has been
	 * received within dot11TDLSResponseTimeout after sending a TDLS Peer
	 * Traffic Indication frame, the STA shall tear down the direct link.
	 * The default value is 5 seconds.
	 */

	/* set traffic indication timer */
	RTMPCancelTimer(&pEntry->Timer, &TimerCancelled);
	RTMPSetTimer(&pEntry->Timer, TDLS_TIMEOUT);
	DBGPRINT(RT_DEBUG_TRACE, ("<======  %s() out\n", __func__));
	return;
}

int cfg_tdls_search_wcid(PRTMP_ADAPTER pAd, u8 *peer)
{
	int i=0, tdls_entry_wcid;
	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));
	for(i=0;i<MAX_NUM_OF_CFG_TDLS_ENTRY;i++)
	{
		if(MAC_ADDR_EQUAL(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacAddr,peer) &&
			pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].EntryValid== TRUE)
		{
			tdls_entry_wcid = pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacTabMatchWCID;
			break;
		}
	}
	if(i == MAX_NUM_OF_CFG_TDLS_ENTRY)
		tdls_entry_wcid = -1;
	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));
	return tdls_entry_wcid;
}

int cfg_tdls_search_ValidLinkIndex(PRTMP_ADAPTER pAd, u8 *peer)
{
	int i=0, tdls_link_index;
	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));
	for(i=0;i<MAX_NUM_OF_CFG_TDLS_ENTRY;i++)
	{
		if(MAC_ADDR_EQUAL(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacAddr,peer) &&
			pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].EntryValid== TRUE)
		{
			tdls_link_index = i;
			break;
		}
	}
	if(i == MAX_NUM_OF_CFG_TDLS_ENTRY)
		tdls_link_index = -1;
	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));
	return tdls_link_index;
}

int cfg_tlds_build_frame(
	PRTMP_ADAPTER	pAd,
	const u8		*peer,
	u8 				dialog_token,
	u8 				action_code,
	u16 			status_code,
	const u8 		*extra_ies,
	size_t 			extra_ies_len,
	BOOLEAN 		send_by_tdls_link,
	u8 				tdls_entry_link_index,
	u8 				reason_code
)
{
	UCHAR	TDLS_ETHERTYPE[] = {0x89, 0x0d};
	HEADER_802_11 Hdr80211;
	UCHAR	Header802_3[14];
	PUCHAR	pOutBuffer = NULL;
	ULONG	FrameLen = 0;
	ULONG	TempLen;
	UCHAR	RemoteFrameType = PROTO_NAME_TDLS;
	NDIS_STATUS	NStatus = NDIS_STATUS_SUCCESS;
	UINT8	category = CATEGORY_TDLS;
	UCHAR 	TDLS_IE = IE_TDLS_LINK_IDENTIFIER;
	UCHAR 	TDLS_IE_LEN = TDLS_ELM_LEN_LINK_IDENTIFIER;
	MAC_TABLE_ENTRY	*pMacEntry = NULL;
	EXT_CAP_INFO_ELEMENT	extCapInfo;
	UCHAR Length = sizeof(EXT_CAP_INFO_ELEMENT);
	BSS_2040_COEXIST_ELEMENT BssCoexistInfo;

	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));
	/* Allocate buffer for transmitting message */
	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
	if (NStatus != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_ERROR,("ACT - TDLS_SetupRequestAction() allocate memory failed \n"));
		return NStatus;
	}


	if(action_code != WLAN_PUB_ACTION_TDLS_DISCOVER_RES)
	{
		MAKE_802_3_HEADER(Header802_3, peer , pAd->CurrentAddress, TDLS_ETHERTYPE);
	}
	else // Discovery response is a MGMT Action frame, others are 890d data frame
	{
		NdisZeroMemory(&Hdr80211, sizeof(HEADER_802_11));
		Hdr80211.FC.Type = FC_TYPE_MGMT;
   		Hdr80211.FC.SubType = SUBTYPE_ACTION;
		Hdr80211.FC.FrDs = 0;
		Hdr80211.FC.ToDs = 0;
		COPY_MAC_ADDR(Hdr80211.Addr1, peer);
		COPY_MAC_ADDR(Hdr80211.Addr2, pAd->CurrentAddress);
		COPY_MAC_ADDR(Hdr80211.Addr3, pAd->CommonCfg.Bssid);

		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						sizeof(HEADER_802_11),			&Hdr80211,
						END_OF_ARGS);

		FrameLen = FrameLen + TempLen;
	}


	/* enter ACTIVE mode */
	//TDLS_CHANGE_TO_ACTIVE(pAd);



	/* fill action code */
	if(action_code != WLAN_PUB_ACTION_TDLS_DISCOVER_RES)
	{
		MakeOutgoingFrame(pOutBuffer,		&TempLen,
						1,				&RemoteFrameType,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
							1,				&category,
							1,				&action_code,
							END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}
	else
	{
		category = CATEGORY_PUBLIC;
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
							1,				&category,
							1,				&action_code,
							END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}

	if(action_code == WLAN_TDLS_SETUP_RESPONSE || action_code == WLAN_TDLS_SETUP_CONFIRM)
	{
		// fill status code
		status_code = cpu2le16(status_code);
		DBGPRINT(RT_DEBUG_ERROR,("########## status code : 0x%04x \n",status_code));
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						2,				&status_code,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}

	if(action_code == WLAN_TDLS_TEARDOWN)
	{
		// fill reason code (unspecified reason 0x001a)
		UINT16 ReasonCode;
		if (reason_code == 0)
			ReasonCode = 0x001a;
		else
			ReasonCode = reason_code;

		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						2,				&ReasonCode,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}

	if(action_code != WLAN_TDLS_TEARDOWN)
	{
		/* fill Dialog Token */
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&dialog_token,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}


	if (action_code == WLAN_TDLS_SETUP_REQUEST
		|| action_code == WLAN_PUB_ACTION_TDLS_DISCOVER_RES
		|| action_code == WLAN_TDLS_SETUP_RESPONSE)
	{
		// fill capability
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
							2,				&pAd->StaActive.CapabilityInfo,
							END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

		// fill support rate
		if (action_code != WLAN_TDLS_SETUP_RESPONSE || status_code == 0)
		{
			MakeOutgoingFrame(pOutBuffer + FrameLen,					&TempLen,
								1,							&SupRateIe,
								1,						&pAd->StaActive.SupRateLen,
								pAd->StaActive.SupRateLen,	pAd->StaActive.SupRate,
								END_OF_ARGS);
			FrameLen = FrameLen + TempLen;
		}

		// fill ext rate
		if (action_code != WLAN_TDLS_SETUP_RESPONSE || status_code == 0)
		{
			if (pAd->StaActive.ExtRateLen != 0)
			{
				MakeOutgoingFrame(pOutBuffer + FrameLen,					&TempLen,
								1,							&ExtRateIe,
								1,						&pAd->StaActive.ExtRateLen,
								pAd->StaActive.ExtRateLen,	pAd->StaActive.ExtRate,
								END_OF_ARGS);

				FrameLen = FrameLen + TempLen;
			}
		}

		// fill support channels
		if (pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsChSwitchSupp)
		{
			UCHAR SupportChIe = IE_SUPP_CHANNELS;
			UCHAR ch_set[32], list_len;

			NdisZeroMemory(ch_set, sizeof(ch_set));
			list_len = 0;
			if (WMODE_CAP_2G(pAd->CommonCfg.PhyMode))
			{
				ch_set[list_len] = 1; // first channel
				ch_set[list_len+1] = 11; // channel number
				list_len += 2;
			}

			if (WMODE_CAP_5G(pAd->CommonCfg.PhyMode))
			{
				ch_set[list_len] = 36; // first channel
				ch_set[list_len+1] = 8; // channel number
				list_len += 2;

				ch_set[list_len] = 149; // first channel
				ch_set[list_len+1] = 4; // channel number
				list_len += 2;
			}

			if (list_len > 0) {
				MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen,
									1,	&SupportChIe,
									1,	&list_len,
									list_len,	&ch_set[0],
									END_OF_ARGS);
				FrameLen = FrameLen + TempLen;
			}
		}
		// fill RSN for discovery response if security is not NONE
		if (action_code == WLAN_PUB_ACTION_TDLS_DISCOVER_RES &&
			pAd->StaCfg.wdev.WepStatus != Ndis802_11EncryptionDisabled)
		{
			UCHAR			CipherTmp[64] = {0};
			UCHAR			CipherTmpLen = 0;

			// RSNIE (7.3.2.25)
			CipherTmpLen = CipherSuiteTDLSLen;
			NdisMoveMemory(CipherTmp, CipherSuiteTDLSWpa2PskAes, CipherTmpLen);

			// Insert RSN_IE to outgoing frame
			MakeOutgoingFrame(pOutBuffer + FrameLen,	&TempLen,
					CipherTmpLen,						&CipherTmp,
					END_OF_ARGS);

			FrameLen = FrameLen + TempLen;
		}
		// fill  Extended Capabilities (7.3.2.27)
		Length = sizeof(EXT_CAP_INFO_ELEMENT);
		NdisZeroMemory(&extCapInfo, Length);

		if ((pAd->CommonCfg.bBssCoexEnable == TRUE) &&
			(pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED) &&
			(pAd->CommonCfg.Channel <= 14))
				extCapInfo.BssCoexistMgmtSupport = 1;

		if (pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TdlsChSwitchSupp)
			extCapInfo.TDLSChSwitchSupport = 1;

		if (pAd->StaCfg.UapsdInfo.bAPSDCapable)
			extCapInfo.UAPSDBufSTASupport = 1;

		extCapInfo.TDLSSupport = 1;


		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
							1,				&ExtCapIe,
							1,				&Length,
							sizeof(EXT_CAP_INFO_ELEMENT), &extCapInfo,
							END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
		// fill FTIE & Timeout Interval for discovery response if security is not NONE
		if (action_code == WLAN_PUB_ACTION_TDLS_DISCOVER_RES &&
			pAd->StaCfg.wdev.WepStatus != Ndis802_11EncryptionDisabled)
		{
			FT_FTIE			FtIe;
			UCHAR			Length;
			UCHAR			SNonce[32];	// Generated in Message 2, random variable

			// FTIE (7.3.2.48)

			NdisZeroMemory(&FtIe, sizeof(FtIe));
			Length =  sizeof(FtIe);

			// generate SNonce
			GenRandom(pAd, pAd->CurrentAddress, FtIe.SNonce);
			NdisMoveMemory(SNonce, FtIe.SNonce, 32);
			do{
				UINT16 MICCtrBuf;
				UCHAR FTIE = IE_FT_FTIE;

				MICCtrBuf = cpu2le16(FtIe.MICCtr.word);
				MakeOutgoingFrame(	pOutBuffer + FrameLen,		&TempLen,
									1,				&FTIE,
									1,				&Length,
									2,				(PUCHAR)&MICCtrBuf,
									16,				(PUCHAR)FtIe.MIC,
									32,				(PUCHAR)FtIe.ANonce,
									32,				(PUCHAR)FtIe.SNonce,
									END_OF_ARGS);

				FrameLen = FrameLen + TempLen;
			}while(0);
			// Timeout Interval (7.3.2.49)
			do{
				UINT8 Length;
				UINT8 TimeOutIntervalIE;
				UINT8 TimeoutType;
				UINT32 TimeoutValueBuf;

				Length = 5;
				TimeOutIntervalIE = IE_FT_TIMEOUT_INTERVAL;
				TimeoutType = 2;
				TimeoutValueBuf = cpu2le32(3600);

				MakeOutgoingFrame(	pOutBuffer + FrameLen,		&TempLen,
									1,				&TimeOutIntervalIE,
									1,				&Length,
									1,				(PUCHAR)&TimeoutType,
									4,				(PUCHAR)&TimeoutValueBuf,
									END_OF_ARGS);

				FrameLen = FrameLen + TempLen;
			}while(0);
		}
		// fill Supported Regulatory Classes
		do
		{
			UCHAR TDLS_IE = IE_SUPP_REG_CLASS;
			UCHAR Length = 16;
			UCHAR SuppClassesList[] = {1, 2, 3, 4, 12, 22, 23, 24, 25, 27, 28, 29, 30, 32, 33};
			UCHAR regclass;

			regclass = cfg_tdls_GetRegulatoryClass(pAd, pAd->CommonCfg.RegTransmitSetting.field.BW, pAd->CommonCfg.Channel);

			MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
								1,				&TDLS_IE,
								1,				&Length,
								1,				&regclass,
								15,				SuppClassesList,
								END_OF_ARGS);

			FrameLen = FrameLen + TempLen;
		}while(0);
		// fill HT Capability
		if (WMODE_CAP_N(pAd->CommonCfg.PhyMode))
		{
			UCHAR HtLen;
			HT_CAPABILITY_IE HtCapabilityTmp;

			HtLen = sizeof(HT_CAPABILITY_IE);
#ifndef RT_BIG_ENDIAN
			NdisZeroMemory(&HtCapabilityTmp, sizeof(HT_CAPABILITY_IE));
			NdisMoveMemory(&HtCapabilityTmp, &pAd->CommonCfg.HtCapability, HtLen);
			HtCapabilityTmp.HtCapInfo.ChannelWidth = pAd->CommonCfg.RegTransmitSetting.field.BW;

			MakeOutgoingFrame(pOutBuffer + FrameLen,	&TempLen,
								1,			&HtCapIe,
								1,			&HtLen,
								HtLen,		&HtCapabilityTmp,
								END_OF_ARGS);
#else
			NdisZeroMemory(&HtCapabilityTmp, sizeof(HT_CAPABILITY_IE));
			NdisMoveMemory(&HtCapabilityTmp, &pAd->CommonCfg.HtCapability, HtLen);
			HtCapabilityTmp.HtCapInfo.ChannelWidth = pAd->CommonCfg.RegTransmitSetting.field.BW;

			*(USHORT *)(&HtCapabilityTmp.HtCapInfo) = SWAP16(*(USHORT *)(&HtCapabilityTmp.HtCapInfo));
			*(USHORT *)(&HtCapabilityTmp.ExtHtCapInfo) = SWAP16(*(USHORT *)(&HtCapabilityTmp.ExtHtCapInfo));

			MakeOutgoingFrame(pOutBuffer + FrameLen,	&TempLen,
								1,			&HtCapIe,
								1,			&HtLen,
								HtLen,		&HtCapabilityTmp,
								END_OF_ARGS);
#endif /* endif */

			FrameLen = FrameLen + TempLen;
		}

		// fill 20/40 BSS Coexistence (7.3.2.61)
		NdisZeroMemory((PUCHAR)&BssCoexistInfo, sizeof(BSS_2040_COEXIST_ELEMENT));

		BssCoexistInfo.ElementID = IE_2040_BSS_COEXIST;
		BssCoexistInfo.Len = 1;
		BssCoexistInfo.BssCoexistIe.word = pAd->CommonCfg.LastBSSCoexist2040.word;
		BssCoexistInfo.BssCoexistIe.field.InfoReq = 1;

		MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen,
			sizeof(BSS_2040_COEXIST_ELEMENT), &BssCoexistInfo,
			END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

#ifdef DOT11_VHT_AC
		// fill VHT Capability
		/* need to send both CAP and OP */
		if(WMODE_CAP_AC(pAd->CommonCfg.PhyMode))
			FrameLen += build_vht_ies(pAd, (UCHAR *)(pOutBuffer + FrameLen), SUBTYPE_BEACON);
#endif /* DOT11_VHT_AC */
	}


	/* fill link identifier */
	//TDLS_InsertLinkIdentifierIE(pAd, (pFrameBuf + *pFrameLen), pFrameLen, pAd->CurrentAddress, pTDLS->MacAddr);

	if( action_code == WLAN_TDLS_SETUP_REQUEST ||
		action_code == WLAN_TDLS_SETUP_CONFIRM ||
		action_code == WLAN_TDLS_DISCOVERY_REQUEST ||
		action_code == WLAN_TDLS_TEARDOWN	   ||
		(action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION && pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[tdls_entry_link_index].bInitiator) ||
		(action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE && pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[tdls_entry_link_index].bInitiator)
	)
	{
		//We are initiator.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				pAd->CurrentAddress,
						6,				peer,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}
	else
	{
		//We are repsonder.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				peer,
						6,				pAd->CurrentAddress,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

	}
	// fill WMM
	if(	action_code == WLAN_TDLS_SETUP_REQUEST ||
		action_code == WLAN_TDLS_SETUP_RESPONSE
	)
	{
		if (pAd->CommonCfg.bWmmCapable)
		{
			QBSS_STA_INFO_PARM QosInfo;
			UCHAR WmeParmIe[9] = {IE_VENDOR_SPECIFIC, 0x07, 0x00, 0x50, 0xf2,
									0x02, 0x00, 0x01, 0x00};

			NdisZeroMemory(&QosInfo, sizeof(QBSS_STA_INFO_PARM));

			if (pAd->StaCfg.UapsdInfo.bAPSDCapable)
			{
				QosInfo.UAPSD_AC_BE = 1;//pAd->CommonCfg.TDLS_bAPSDAC_BE;
				QosInfo.UAPSD_AC_BK = 1;//pAd->CommonCfg.TDLS_bAPSDAC_BK;
				QosInfo.UAPSD_AC_VI = 1;//pAd->CommonCfg.TDLS_bAPSDAC_VI;
				QosInfo.UAPSD_AC_VO = 1;//pAd->CommonCfg.TDLS_bAPSDAC_VO;
				QosInfo.MaxSPLength = 0;//pAd->CommonCfg.TDLS_MaxSPLength;

				DBGPRINT(RT_DEBUG_ERROR, ("tdls our uapsd> UAPSD %d %d %d %d %d!\n",
						QosInfo.UAPSD_AC_BE,
						QosInfo.UAPSD_AC_BK,
						QosInfo.UAPSD_AC_VI,
						QosInfo.UAPSD_AC_VO,
						QosInfo.MaxSPLength));
			}
			WmeParmIe[8] |= *(PUCHAR) & QosInfo;
			MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen, 9, WmeParmIe, END_OF_ARGS);

			FrameLen = FrameLen + TempLen;
		}
	}
	else if(action_code == WLAN_TDLS_SETUP_CONFIRM)
	{
		 /*UCHAR WmeParmIe[26] = {IE_VENDOR_SPECIFIC, 0x18, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0x0f, 0x00, 0x03, 0xa4, 0x00, 0x00,
				      0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00};*/
		QBSS_STA_INFO_PARM QosInfo;
		ULONG TempLen;
		USHORT	idx;

		/* When the BSS is QoS capable, then the BSS QoS parameters shall be
		 * used by the TDLS peer STAs on the AP's channel, and the values
		 * indicated inside the TDLS Setup Confirm frame apply only for the
		 * off-channel. The EDCA parameters for the off-channel should be
		 * the same as those on the AP's channel when QoS is supported by the BSS,
		 * because this may optimize the channel switching process.
		 */

		UCHAR WmeParmIe[26] = {IE_VENDOR_SPECIFIC, 24, 0x00, 0x50, 0xf2, 0x02, 0x01, 0x01, 0, 0};

		NdisZeroMemory(&QosInfo, sizeof(QBSS_STA_INFO_PARM));

		if (pAd->StaCfg.UapsdInfo.bAPSDCapable)
		{

			QosInfo.UAPSD_AC_BE = 1;//pAd->CommonCfg.TDLS_bAPSDAC_BE;
			QosInfo.UAPSD_AC_BK = 1;//pAd->CommonCfg.TDLS_bAPSDAC_BK;
			QosInfo.UAPSD_AC_VI = 1;//pAd->CommonCfg.TDLS_bAPSDAC_VI;
			QosInfo.UAPSD_AC_VO = 1;//pAd->CommonCfg.TDLS_bAPSDAC_VO;
			QosInfo.MaxSPLength = 0;//pAd->CommonCfg.TDLS_MaxSPLength;

			DBGPRINT(RT_DEBUG_WARN, ("tdls our uapsd> UAPSD %d %d %d %d %d!\n",
					QosInfo.UAPSD_AC_BE,
					QosInfo.UAPSD_AC_BK,
					QosInfo.UAPSD_AC_VI,
					QosInfo.UAPSD_AC_VO,
					QosInfo.MaxSPLength));
		}

		WmeParmIe[8] |= *(PUCHAR)&QosInfo;

		for (idx=QID_AC_BE; idx<=QID_AC_VO; idx++)
		{
			WmeParmIe[10+ (idx*4)] = (idx << 5)								+	  // b5-6 is ACI
								   ((UCHAR)pAd->CommonCfg.APEdcaParm.bACM[idx] << 4) 	+	  // b4 is ACM
								   (pAd->CommonCfg.APEdcaParm.Aifsn[idx] & 0x0f);			  // b0-3 is AIFSN
			WmeParmIe[11+ (idx*4)] = (pAd->CommonCfg.APEdcaParm.Cwmax[idx] << 4)		+	  // b5-8 is CWMAX
								   (pAd->CommonCfg.APEdcaParm.Cwmin[idx] & 0x0f);			  // b0-3 is CWMIN
			WmeParmIe[12+ (idx*4)] = (UCHAR)(pAd->CommonCfg.APEdcaParm.Txop[idx] & 0xff);	  // low byte of TXOP
			WmeParmIe[13+ (idx*4)] = (UCHAR)(pAd->CommonCfg.APEdcaParm.Txop[idx] >> 8);	  // high byte of TXOP
		}

		MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
							26,				WmeParmIe,
							END_OF_ARGS);

		FrameLen = FrameLen + TempLen;
	}

	if(action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE || (action_code == WLAN_TDLS_TEARDOWN && reason_code != 25))
	{
		pMacEntry = MacTableLookup(pAd, peer);
		if (pMacEntry == NULL)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("tdls_cmd> ERROR! No such peer in %s!\n",
					__func__));
			return -1;
		}
	}

	if(action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION)
	{
		/* fill PU buffer status */
		MAC_TABLE_ENTRY	*pEntry;
		ULONG TempLen;
		UINT8 Length;
		UINT8 IeIdPuBufferStatus;
		UINT8 PuBufferStatus;
		UINT8 FlgIsAnyPktForBK, FlgIsAnyPktForBE;
		UINT8 FlgIsAnyPktForVI, FlgIsAnyPktForVO;


		/* get pEntry */
		pEntry = MacTableLookup(pAd, peer);

		if (pEntry == NULL)
		{
			DBGPRINT(RT_DEBUG_ERROR, ("tdls_cmd> ERROR! No such peer in %s!\n",
					__func__));
			return -1;
		}

		/* init */
		Length = 1;
		IeIdPuBufferStatus = IE_TDLS_PU_BUFFER_STATUS;
		PuBufferStatus = 0;

		/* get queue status */
		UAPSD_QueueStatusGet(pAd, pEntry,
							&FlgIsAnyPktForBK, &FlgIsAnyPktForBE,
							&FlgIsAnyPktForVI, &FlgIsAnyPktForVO);
		PuBufferStatus |= (FlgIsAnyPktForBK == TRUE)? 0x01: 0x00;
		PuBufferStatus |= (FlgIsAnyPktForBE == TRUE)? 0x02: 0x00;
		PuBufferStatus |= (FlgIsAnyPktForVI == TRUE)? 0x04: 0x00;
		PuBufferStatus |= (FlgIsAnyPktForVO == TRUE)? 0x08: 0x00;

		/* init element */
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
							1,				&IeIdPuBufferStatus,
							1,				&Length,
							1,				&PuBufferStatus,
							END_OF_ARGS);

		FrameLen = FrameLen + TempLen;

	}

        /* add extra_ies */
	if (extra_ies_len > 0 && extra_ies != NULL) {
			hex_dump("extra_ies from supplicant ", (UCHAR*)extra_ies, extra_ies_len);
			MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen,
				extra_ies_len, extra_ies,
				END_OF_ARGS);
			FrameLen = FrameLen + TempLen;
	}
	else {
		DBGPRINT(RT_DEBUG_WARN, ("### extra_ies_len:%lu\n", (ULONG)extra_ies_len));
	}

	/*                                 debug output                                                     */
	switch (action_code) {
	case WLAN_TDLS_SETUP_REQUEST:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_TDLS_SETUP_REQUEST ==>\n"));
		break;
	case WLAN_TDLS_SETUP_RESPONSE:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_TDLS_SETUP_RESPONSE ==>\n"));
		break;
	case WLAN_TDLS_SETUP_CONFIRM:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_TDLS_SETUP_CONFIRM ==>\n"));
		break;
	case WLAN_TDLS_TEARDOWN:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_TDLS_TEARDOWN ==>\n"));
		break;
	case WLAN_TDLS_DISCOVERY_REQUEST:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_TDLS_DISCOVERY_REQUEST ==>\n"));
		break;
	case WLAN_PUB_ACTION_TDLS_DISCOVER_RES:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> WLAN_PUB_ACTION_TDLS_DISCOVERY_RESPONSE ==>\n"));
		break;
	case TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION ==>\n"));
		break;
	case TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE ==>\n"));
		break;
	case TDLS_ACTION_CODE_WFD_TUNNELED_PROBE_REQ:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> TDLS_ACTION_CODE_WFD_TUNNELED_PROBE_REQ ==>\n"));
		break;
	case TDLS_ACTION_CODE_WFD_TUNNELED_PROBE_RSP:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> TDLS_ACTION_CODE_WFD_TUNNELED_PROBE_RSP ==>\n"));
		break;
	default:
		CFG80211DBG(RT_DEBUG_ERROR, ("80211> UNKNOWN TDLS PACKET %d ==>\n",action_code));
		break;
	}
	/* hex_dump("TDLS send pack", pOutBuffer, FrameLen); */

	if (action_code == TDLS_ACTION_CODE_TEARDOWN)
	{
		/*  AP path */
		if (reason_code == 25)
			RTMPToWirelessSta(pAd, &pAd->MacTab.Content[BSSID_WCID],
							  Header802_3, LENGTH_802_3, pOutBuffer,
							  (UINT)FrameLen, FALSE);
		else
			RTMPToWirelessSta(pAd, pMacEntry, Header802_3, LENGTH_802_3,
							  pOutBuffer, (UINT)FrameLen, FALSE);
	}
	else if (send_by_tdls_link == TRUE)
	{
		if (action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE)
			RTMP_PS_VIRTUAL_WAKEUP_PEER(pMacEntry); /* peer can not sleep for a while */

		RTMPToWirelessSta(pAd, pMacEntry, Header802_3, LENGTH_802_3, pOutBuffer, (UINT)FrameLen, FALSE);
	}
	else if (action_code != WLAN_PUB_ACTION_TDLS_DISCOVER_RES) /* Normal frame, go AP path */
	{
		if (action_code == TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION)
			ASIC_PS_CAN_NOT_SLEEP(pAd);

		RTMPToWirelessSta(pAd, &pAd->MacTab.Content[BSSID_WCID], Header802_3,
							LENGTH_802_3, pOutBuffer, (UINT)FrameLen, FALSE);
	}
	else /* DISCOVERY RESPONSE go direct path */
	{
		INT stat=0;
		stat = MiniportMMRequest(pAd, QID_AC_VI, pOutBuffer, FrameLen);
		if(stat != 0)
			CFG80211DBG(RT_DEBUG_ERROR, ("########### MINIPORT ERR %d\n", stat));

	}
	DBGPRINT(RT_DEBUG_INFO, ("<======  %s() out\n", __func__));
	return 0;
}

INT cfg_tdls_EntryInfo_Display_Proc(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR arg)
{
	INT i;

	DBGPRINT(RT_DEBUG_OFF, ("\n%-19s\n", "MAC\n"));
	for (i=0; i<MAX_NUM_OF_CFG_TDLS_ENTRY; i++)
	{
		if ((pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].EntryValid == TRUE))
		{
			PMAC_TABLE_ENTRY pEntry = &pAd->MacTab.Content[pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacTabMatchWCID];

			DBGPRINT(RT_DEBUG_OFF, ("%02x:%02x:%02x:%02x:%02x:%02x  \n",PRINT_MAC(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacAddr)));
			/*DBGPRINT(RT_DEBUG_OFF, ("%-8d\n", pAd->StaCfg.DLSEntry[i].TimeOut)); */
			DBGPRINT(RT_DEBUG_OFF, ("\n"));
			DBGPRINT(RT_DEBUG_OFF, ("TDLS Entry %d is valid, bInitiator = %d \n"
				,i,pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].bInitiator));
			DBGPRINT(RT_DEBUG_OFF, ("\n"));
			DBGPRINT(RT_DEBUG_OFF, ("\n%-19s%-4s%-4s%-4s%-4s%-7s%-7s%-7s","MAC", "AID", "BSS", "PSM", "WMM", "RSSI0", "RSSI1", "RSSI2"));
			DBGPRINT(RT_DEBUG_OFF, ("%-8s%-10s%-6s%-6s%-6s%-6s", "MIMOPS", "PhMd", "BW", "MCS", "SGI", "STBC"));
			DBGPRINT(RT_DEBUG_OFF, ("\n%02X:%02X:%02X:%02X:%02X:%02X  ",
				pEntry->Addr[0], pEntry->Addr[1], pEntry->Addr[2],
				pEntry->Addr[3], pEntry->Addr[4], pEntry->Addr[5]));
			DBGPRINT(RT_DEBUG_OFF, ("%-4d", (int)pEntry->Aid));
			DBGPRINT(RT_DEBUG_OFF, ("%-4d", (int)pEntry->apidx));
			DBGPRINT(RT_DEBUG_OFF, ("%-4d", (int)pEntry->PsMode));
			DBGPRINT(RT_DEBUG_OFF, ("%-4d", (int)CLIENT_STATUS_TEST_FLAG(pEntry, fCLIENT_STATUS_WMM_CAPABLE)));
			DBGPRINT(RT_DEBUG_OFF, ("%-7d", pEntry->RssiSample.AvgRssi0));
			DBGPRINT(RT_DEBUG_OFF, ("%-7d", pEntry->RssiSample.AvgRssi1));
			DBGPRINT(RT_DEBUG_OFF, ("%-7d", pEntry->RssiSample.AvgRssi2));
			DBGPRINT(RT_DEBUG_OFF, ("%-8d", (int)pEntry->MmpsMode));
			DBGPRINT(RT_DEBUG_OFF, ("%-10s", get_phymode_str(pEntry->HTPhyMode.field.MODE)));
			DBGPRINT(RT_DEBUG_OFF, ("%-6s", get_bw_str(pEntry->HTPhyMode.field.BW)));
			DBGPRINT(RT_DEBUG_OFF, ("%-6d", pEntry->HTPhyMode.field.MCS));
			DBGPRINT(RT_DEBUG_OFF, ("%-6d", pEntry->HTPhyMode.field.ShortGI));
			DBGPRINT(RT_DEBUG_OFF, ("%-6d", pEntry->HTPhyMode.field.STBC));
			DBGPRINT(RT_DEBUG_OFF, ("%-10d, %d, %d%%\n", pEntry->DebugFIFOCount, pEntry->DebugTxCount,
						(pEntry->DebugTxCount) ? ((pEntry->DebugTxCount-pEntry->DebugFIFOCount)*100/pEntry->DebugTxCount) : 0));
			DBGPRINT(RT_DEBUG_OFF, ("\n"));

		}
	}

	return TRUE;
}

VOID cfg_tdls_TimerInit(PRTMP_ADAPTER pAd)
{
	int i=0;
	for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++)
	{
		PCFG_TDLS_ENTRY pCfgTdls = &(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i]);
		pCfgTdls->pAd = pAd;
		RTMPInitTimer(pAd, &(pCfgTdls->Timer), GET_TIMER_FUNCTION(cfg_tdls_PTITimeoutAction), pCfgTdls, FALSE);
		DBGPRINT(RT_DEBUG_TRACE,("InitTimer TDLS %d\n",i));
	}
}

VOID cfg_tdls_PTITimeoutAction(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3)
{
	PCFG_TDLS_ENTRY 		pTDLS = (PCFG_TDLS_ENTRY)FunctionContext;
	PRTMP_ADAPTER			pAd;

	/*
		11.2.1.14.1 Peer U-APSD Behavior at the PU buffer STA
		When no corresponding TDLS Peer Traffic Response frame has been
		received within dot11TDLSResponseTimeout after sending a TDLS Peer
		Traffic Indication frame, the STA shall tear down the direct link.
	*/
	pAd = pTDLS->pAd;
	DBGPRINT(RT_DEBUG_OFF, ("[%s] pAd=[%p]", __func__, pAd));
	if (pTDLS->FlgIsWaitingUapsdTraRsp == TRUE)
	{
		/* timeout for traffic response frame */

		/* TODO: tear down the link with the peer */
		DBGPRINT(RT_DEBUG_OFF, ("tdls uapsd> traffic rsp timeout!!!\npeerMAC(%02X:%02X:%02X:%02X:%02X:%02X), send link teardown\n",
			PRINT_MAC(pTDLS->MacAddr)));

		cfg_auto_teardown(pAd, pTDLS->MacAddr);
	}

	return;
}
VOID cfg_tdls_BaseChannelTimeoutAction(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)FunctionContext;
	PCFG_TDLS_STRUCT pCfgTdls = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info;

	DBGPRINT(RT_DEBUG_INFO, ("tdls CHSW base channel timeout!!!\n"));

	if (pCfgTdls->bChannelSwitchInitiator && pCfgTdls->bDoingPeriodChannelSwitch)
		cfg_tdls_chsw_req(pAd, pCfgTdls->CHSWPeerMacAddr, pCfgTdls->OrigTargetOffChannel, pCfgTdls->TargetOffChannelBW);

	return;
}

#ifdef CERTIFICATION_SIGMA_SUPPORT
VOID cfg_tdls_ResetTbttTimer(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3)
{
	PRTMP_ADAPTER	pAd = (PRTMP_ADAPTER)FunctionContext;

	//AsicDisableSync(pAd);
	//mdelay(1);
	AsicEnableBssSync(pAd);

	return;
}
#endif /* CERTIFICATION_SIGMA_SUPPORT */

VOID cfg_tdls_rx_parsing(PRTMP_ADAPTER pAd,RX_BLK *pRxBlk)
{
	UCHAR TDLS_LLC_SNAP_WITH_CATEGORY[] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00, 0x89, 0x0d,
		PROTO_NAME_TDLS, CATEGORY_TDLS};
	UCHAR TDLSType, dialog_token;
	UCHAR *peerMAC;
	UCHAR nullAddr[MAC_ADDR_LEN];
	CHAR *Ptr = pRxBlk->pData;
	ULONG Length = 0;
	PEID_STRUCT pEid;
	ULONG RemainLen = pRxBlk->DataSize;
	PCFG_TDLS_STRUCT pCfgTdls = &pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info;
	PCFG_TDLS_ENTRY pEntry;

	if (pRxBlk->DataSize < (LENGTH_802_1_H + LENGTH_TDLS_H) ||
		!NdisEqualMemory(TDLS_LLC_SNAP_WITH_CATEGORY, pRxBlk->pData, LENGTH_802_1_H + 2))
		return;

	NdisZeroMemory(nullAddr, MAC_ADDR_LEN);
	TDLSType = *(pRxBlk->pData + LENGTH_802_1_H + 2);

	if (TDLSType == TDLS_ACTION_CODE_SETUP_REQUEST ||
		TDLSType == TDLS_ACTION_CODE_SETUP_RESPONSE) {
		int i = 0, tdls_entry_index = 0;

		peerMAC = pRxBlk->pHeader->Addr3;

		for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++) {
			if (pCfgTdls->TDLSEntry[i].EntryValid == FALSE &&
				(MAC_ADDR_EQUAL(pCfgTdls->TDLSEntry[i].MacAddr, nullAddr) ||
				MAC_ADDR_EQUAL(pCfgTdls->TDLSEntry[i].MacAddr, peerMAC))) {
					tdls_entry_index = i;  /* found an empty entry */
					break;
			}
		}

		if (i == MAX_NUM_OF_CFG_TDLS_ENTRY) {
			DBGPRINT(RT_DEBUG_ERROR,
				("%s() - TDLS LINK NUMBER EXCEED, SUPPORT CONCURRENT %d LINKs\n",
				__func__, MAX_NUM_OF_CFG_TDLS_ENTRY));
			return;
		}

		pEntry = &pCfgTdls->TDLSEntry[tdls_entry_index];
		COPY_MAC_ADDR(pEntry->MacAddr, peerMAC);

		if (TDLSType == TDLS_ACTION_CODE_SETUP_REQUEST)	{
			/* token 1 + capa 2 */
			Ptr += LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H + 1 + 2;
			RemainLen -= LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H + 1 + 2;
		} else {
			pEntry->bInitiator = TRUE;
			DBGPRINT(RT_DEBUG_WARN,
				("%s() - TDLSEntry[%d] Initiator = %d\n",
				__func__, tdls_entry_index, pEntry->bInitiator));
			/* status 2 + token 1 + capa 2 */
			Ptr += LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H + 2 + 1 + 2;
			RemainLen -= LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H + 2 + 1 + 2;
		}

		/* Add for 2 necessary EID field check */
		pEid = (PEID_STRUCT)Ptr;

		/* get variable fields from payload and advance the pointer */
		while ((Length + 2 + pEid->Len) <= RemainLen) {
			switch (pEid->Eid) {
			case IE_QOS_CAPABILITY:
				if (pEid->Len == 1)
					pEntry->QosCapability = *(pEid->Octet);
				break;
			case IE_VENDOR_SPECIFIC:
				/* handle WME PARAMTER ELEMENT */
				if (NdisEqualMemory(pEid->Octet, WME_INFO_ELEM, 6) &&
					(pEid->Len == 7))
					pEntry->QosCapability = pEid->Octet[6];
				break;
			case IE_HT_CAP:
				if (pAd->CommonCfg.PhyMode >= PHY_11ABGN_MIXED) {
					if (pEid->Len >= SIZE_HT_CAP_IE) {
						NdisMoveMemory(&pEntry->HtCapability,
							pEid->Octet, sizeof(HT_CAPABILITY_IE));
						pEntry->HtCapabilityLen = SIZE_HT_CAP_IE;
					}
				}
				break;
#ifdef DOT11_VHT_AC
			case IE_VHT_CAP:
				if (pEid->Len == sizeof(VHT_CAP_IE)) {
					NdisMoveMemory(&pEntry->tdls_vht_cap,
						pEid->Octet, sizeof(VHT_CAP_IE));
					pEntry->tdls_vht_cap_len = sizeof(VHT_CAP_IE);
					DBGPRINT(RT_DEBUG_TRACE, ("%s: GOT VHT CAP LEN: %d\n",
						__func__, pEntry->tdls_vht_cap_len));
				} else {
					DBGPRINT(RT_DEBUG_ERROR, ("%s():wrong IE_VHT_CAP\n",
						__func__));
				}
				break;
			case IE_VHT_OP:
				if (pEid->Len == sizeof(VHT_OP_IE)) {
					NdisMoveMemory(&pEntry->tdls_vht_op,
						pEid->Octet, sizeof(VHT_OP_IE));
					pEntry->tdls_vht_op_len = sizeof(VHT_OP_IE);
					DBGPRINT(RT_DEBUG_TRACE, ("%s: GOT VHT OP LEN: %d\n",
						__func__, pEntry->tdls_vht_op_len));
				} else {
					DBGPRINT(RT_DEBUG_ERROR, ("%s():wrong IE_VHT_OP\n",
						__func__));
				}
				break;
#endif /* DOT11_VHT_AC */
			default:
				break;
			}

			Length = Length + 2 + pEid->Len;
			pEid = (PEID_STRUCT)((UCHAR *)pEid + 2 + pEid->Len);
		}
	}
#ifdef RTMP_USB_SUPPORT
	else if (TDLSType == TDLS_ACTION_CODE_CHANNEL_SWITCH_REQUEST) {
		CFG_TDLS_CHSW_PARAM TdlsCHSWParam;
		NdisZeroMemory(&TdlsCHSWParam, sizeof(CFG_TDLS_CHSW_PARAM));
		peerMAC = pRxBlk->pHeader->Addr2;
		COPY_MAC_ADDR(pCfgTdls->CHSWPeerMacAddr, peerMAC);

		if (pCfgTdls->IamInOffChannel) {
			/* Offset to Target Channel */
			Ptr += LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;
			RemainLen -= LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;
			/* Get the value of target channel from payload and advance the pointer */
			if (RemainLen < 1) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s() - Invaild packet length - (target channel)\n",
					__func__));
				return;
			}
			pCfgTdls->TargetOffChannel = *Ptr;

			if (pCfgTdls->TargetOffChannel == pCfgTdls->BaseChannel) {
				DBGPRINT(RT_DEBUG_OFF,
					("%s() - GOT SWITCH Req in off channel, STOP CH SWITCH!!\n",
					__func__));
				pCfgTdls->bDoingPeriodChannelSwitch = FALSE;
				pCfgTdls->IamInOffChannel = FALSE;
			}
			cfg_tdls_chsw_resp(pAd, pCfgTdls->CHSWPeerMacAddr,
				TDLS_CHANNEL_SWITCH_TIME, TDLS_CHANNEL_SWITCH_TIMEOUT, 0);
		} else {
			/* in base channel , parse request for CHSW time */
			pCfgTdls->bDoingChannelSwitch = TRUE;
			/* Offset to Target Channel */
			Ptr += LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;
			RemainLen -= LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;

			/* Get the value of target channel from payload and advance the pointer */
			if (RemainLen < 1) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s() - Invaild packet length - (target channel)\n",
					__func__));
				return;
			}

			pCfgTdls->TargetOffChannel = *Ptr;

			/* Offset to Regulatory Class */
			Ptr += 1;
			RemainLen -= 1;

			/* Get the value of regulatory class from payload and advance the pointer */
			if (RemainLen < 1) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s() - Invaild packet length - (regulatory class)\n",
					__func__));
				return;
			}

			/* Offset to other elements */
			Ptr += 1;
			RemainLen -= 1;

			pEid = (PEID_STRUCT)Ptr;

			/* get variable fields from payload and advance the pointer */
			while ((Length + 2 + pEid->Len) <= RemainLen) {
				switch (pEid->Eid) {
				case IE_SECONDARY_CH_OFFSET:
					if (pEid->Len == 1) {
						pCfgTdls->TargetOffChannelExt = pEid->Octet[0];
					} else {
						DBGPRINT(RT_DEBUG_ERROR,
						("%s() - wrong IE_SECONDARY_CH_OFFSET.\n",
						__func__));
					}
					break;
				case IE_TDLS_LINK_IDENTIFIER:
					if (pEid->Len != TDLS_ELM_LEN_LINK_IDENTIFIER) {
						DBGPRINT(RT_DEBUG_ERROR,
						("%s() - wrong IE_TDLS_LINK_IDENTIFIER.\n",
						__func__));
					}
					break;
				case IE_TDLS_CHANNEL_SWITCH_TIMING:
					if (pEid->Len == 4) {
						NdisMoveMemory(&pCfgTdls->ChSwitchTime,
							&pEid->Octet[0], 2);
						NdisMoveMemory(&pCfgTdls->ChSwitchTimeout,
							&pEid->Octet[2], 2);
					} else {
						DBGPRINT(RT_DEBUG_ERROR,
						("%s() - wrong IE_TDLS_CHANNEL_SWITCH_TIMING.\n",
						__func__));
					}
					break;
				default:
					/* Unknown IE, we have to pass it as variable IEs */
					DBGPRINT(RT_DEBUG_ERROR,
						("%s() - unrecognized EID = %d\n",
						__func__, pEid->Eid));
					break;
				}
				Length = Length + 2 + pEid->Len;
				pEid = (PEID_STRUCT)((UCHAR *)pEid + 2 + pEid->Len);
			}

			if (TDLS_CHANNEL_SWITCH_TIME >= pCfgTdls->ChSwitchTime)
				pCfgTdls->ChSwitchTime = TDLS_CHANNEL_SWITCH_TIME;

			if (TDLS_CHANNEL_SWITCH_TIMEOUT >= pCfgTdls->ChSwitchTimeout)
				pCfgTdls->ChSwitchTimeout = TDLS_CHANNEL_SWITCH_TIMEOUT;

			if (pAd->StaActive.ExtCapInfo.TDLSChSwitchProhibited) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s() - AP prohibit TDLS CH SWITH (%d), STOP!!\n",
					__func__,
					pAd->StaActive.ExtCapInfo.TDLSChSwitchProhibited));
				cfg_tdls_chsw_resp(pAd, pCfgTdls->CHSWPeerMacAddr, 0, 0, 37);
				pCfgTdls->bDoingChannelSwitch = FALSE;
				return;
			} else if (!pCfgTdls->TdlsChSwitchSupp) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s() - Do not support channel switching!!\n",
					  __func__));
				cfg_tdls_chsw_resp(pAd,
						   pCfgTdls->CHSWPeerMacAddr,
						   0, 0, 37);
				pCfgTdls->bDoingChannelSwitch = FALSE;
				return;
			} else {
				cfg_tdls_chsw_resp(pAd, pCfgTdls->CHSWPeerMacAddr,
					pCfgTdls->ChSwitchTime, pCfgTdls->ChSwitchTimeout, 0);
			}

			/* START CHSW */
			pCfgTdls->OffChannelStayTime =
				pAd->StaCfg.DtimPeriod * 100 - pCfgTdls->BaseChannelStayTime;

			pCfgTdls->BaseChannel = pAd->CommonCfg.CentralChannel;
			if (pCfgTdls->TargetOffChannelExt == 0x1)
				pCfgTdls->TargetOffChannel += 2;
			else if (pCfgTdls->TargetOffChannelExt == 0x3)
				pCfgTdls->TargetOffChannel -= 2;

			pCfgTdls->TargetOffChannelBW = (pCfgTdls->TargetOffChannelExt) ? 1 : 0;
			TdlsCHSWParam.cmd = 0;

			RtmpOsMsDelay(1); /* delay 1 ms to make sure rsp tx success*/
			RTEnqueueInternalCmd(pAd, CMDTHREAD_TDLS_SEND_CH_SW_SETUP,
				&TdlsCHSWParam, sizeof(CFG_TDLS_CHSW_PARAM));
		}
	} else if (TDLSType == TDLS_ACTION_CODE_CHANNEL_SWITCH_RESPONSE) {
		CFG_TDLS_CHSW_PARAM TdlsCHSWParam;
		USHORT StatusCode = 0;

		peerMAC = pRxBlk->pHeader->Addr2;
		if (pCfgTdls->IamInOffChannel) {
			DBGPRINT(RT_DEBUG_WARN, ("On receive TDLS CH resp in OffChannel\n"));
		} else if (pCfgTdls->IsentCHSW && pCfgTdls->bChannelSwitchInitiator) {
			/* to prevent receiving off channel CHSW resp in base channel */
			pCfgTdls->IsentCHSW = FALSE;

			/* Offset to Status Code */
			Ptr += LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;
			RemainLen -= LENGTH_802_1_H + LENGTH_TDLS_PAYLOAD_H;

			/* Get the value of Status Code from payload and advance the pointer */
			if (RemainLen < 2) {
				DBGPRINT(RT_DEBUG_ERROR,
					("TDLS CS Rsp --> Invaild packet length\n"));
				return;
			}
			NdisMoveMemory(&StatusCode, Ptr, 2);

			if (StatusCode != 0) {
				DBGPRINT(RT_DEBUG_WARN,
					("TDLS CS Rsp --> status code: %d\n", StatusCode));
				return;
			}

			/* Offset to other elements */
			Ptr += 2;
			RemainLen -= 2;

			pEid = (PEID_STRUCT)Ptr;

			/* get variable fields from payload and advance the pointer */
			while ((Length + 2 + pEid->Len) <= RemainLen) {
				switch (pEid->Eid) {
				case IE_TDLS_LINK_IDENTIFIER:
					if (pEid->Len != TDLS_ELM_LEN_LINK_IDENTIFIER) {
						DBGPRINT(RT_DEBUG_ERROR,
							("TDLS CS Rsp --> wrong IDENTIFIER\n"));
					}
					break;
				case IE_TDLS_CHANNEL_SWITCH_TIMING:
					if (pEid->Len == 4) {
						NdisMoveMemory(&pCfgTdls->ChSwitchTime,
							&pEid->Octet[0], 2);
						NdisMoveMemory(&pCfgTdls->ChSwitchTimeout,
							&pEid->Octet[2], 2);
					} else {
						DBGPRINT(RT_DEBUG_ERROR,
							("TDLS CS Rsp --> wrong TIMING\n"));
					}
					break;
				default:
					/* Unknown IE, we have to pass it as variable IEs */
					DBGPRINT(RT_DEBUG_ERROR,
						("TDLS CS Rsp --> unrecognized EID = %d\n",
						pEid->Eid));
					break;
				}

				Length = Length + 2 + pEid->Len;
				pEid = (PEID_STRUCT)((UCHAR *)pEid + 2 + pEid->Len);
			}

			/* START CHSW	*/
			pCfgTdls->OffChannelStayTime =
				pAd->StaCfg.DtimPeriod * 100 - pCfgTdls->BaseChannelStayTime;
			pCfgTdls->BaseChannel = pAd->CommonCfg.Channel;
			COPY_MAC_ADDR(pCfgTdls->CHSWPeerMacAddr, peerMAC);
			TdlsCHSWParam.cmd = 0;

			RTEnqueueInternalCmd(pAd, CMDTHREAD_TDLS_SEND_CH_SW_SETUP,
				&TdlsCHSWParam, sizeof(CFG_TDLS_CHSW_PARAM));
		}
	}
#endif /* RTMP_USB_SUPPORT */
	else if (TDLSType == TDLS_ACTION_CODE_PEER_TRAFFIC_INDICATION) {
		peerMAC = pRxBlk->pHeader->Addr3;
		dialog_token = *(pRxBlk->pData + LENGTH_802_1_H + 3);
		cfg_tdls_rcv_PeerTrafficIndication(pAd, dialog_token, peerMAC);
	} else if (TDLSType == TDLS_ACTION_CODE_PEER_TRAFFIC_RESPONSE) {
		peerMAC = pRxBlk->pHeader->Addr2;
		dialog_token = *(pRxBlk->pData + LENGTH_802_1_H + 3);
		cfg_tdls_rcv_PeerTrafficResponse(pAd, peerMAC);
	}
	return;
}

int cfg_tdls_chsw_req(
PRTMP_ADAPTER	pAd,
u8 *peer,
u8 target_channel,
u8 target_bw
)
{
	UCHAR	TDLS_ETHERTYPE[] = {0x89, 0x0d};
	UCHAR	Header802_3[14];
	PUCHAR	pOutBuffer = NULL;
	ULONG	FrameLen = 0;
	ULONG	TempLen;
	UCHAR	RemoteFrameType = PROTO_NAME_TDLS;
	NDIS_STATUS	NStatus = NDIS_STATUS_SUCCESS;
	UINT8	category = CATEGORY_TDLS;
	UCHAR 	TDLS_IE = IE_TDLS_LINK_IDENTIFIER;
	UCHAR 	TDLS_IE_LEN = TDLS_ELM_LEN_LINK_IDENTIFIER;
	MAC_TABLE_ENTRY	*pMacEntry = NULL;
	UCHAR regclass;

	u8 action_code = TDLS_ACTION_CODE_CHANNEL_SWITCH_REQUEST;
	UCHAR target_ext_ch_offset = EXTCHA_NONE;
	INT tdls_entry_link_index=0;

	DBGPRINT(RT_DEBUG_INFO, ("======>  %s()\n", __func__));

	/* Allocate buffer for transmitting message */
	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
	if (NStatus != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_ERROR,("ACT - TDLS_SetupRequestAction() allocate memory failed \n"));
		return NStatus;
	}


	MAKE_802_3_HEADER(Header802_3, peer , pAd->CurrentAddress, TDLS_ETHERTYPE);

	/* fill action code */
	MakeOutgoingFrame(pOutBuffer,		&TempLen,
					1,				&RemoteFrameType,
					END_OF_ARGS);
	FrameLen = FrameLen + TempLen;

	MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&category,
						1,				&action_code,
						END_OF_ARGS);
	FrameLen = FrameLen + TempLen;

	//fill target channel
	MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
						1,				&target_channel,
						END_OF_ARGS);

	FrameLen = FrameLen + TempLen;

	//fill reg class

	if(target_bw != 0)
		target_bw = 1;

	//regclass = cfg_tdls_GetRegulatoryClass(pAd, target_bw, target_channel);
	regclass =22;
	MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
						1,				&regclass,
						END_OF_ARGS);

	FrameLen = FrameLen + TempLen;
	if(target_bw != 0)
	{
		UCHAR length = 1;
		target_ext_ch_offset = pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TargetOffChannelExt;
		DBGPRINT(RT_DEBUG_INFO,
			("@@@ send CHSW request with TargetOffChannelExt : %d, Channel:%d\n",
			target_ext_ch_offset, target_channel));
		MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
					1,				&NewExtChanIe,
					1,				&length,
					1,				&target_ext_ch_offset,
					END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}
	else
	{
		;//target_ext_ch_offset = EXTCHA_NONE;
	}

	tdls_entry_link_index = cfg_tdls_search_ValidLinkIndex(pAd,peer);
	if (tdls_entry_link_index == -1) {
		MlmeFreeMemory(pAd, pOutBuffer);
		return -1;
	}

	if(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[tdls_entry_link_index].bInitiator)
	{
		//We are initiator.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				pAd->CurrentAddress,
						6,				peer,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}
	else
	{
		//We are repsonder.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				peer,
						6,				pAd->CurrentAddress,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

	}

	do
	{
		UCHAR TDLS_IE = IE_TDLS_CHANNEL_SWITCH_TIMING;
		UCHAR TDLS_IE_LEN = 4;
		UINT16	SwitchTimeBuf = cpu2le16(TDLS_CHANNEL_SWITCH_TIME);
		UINT16	SwitchTimeOutBuf = cpu2le16(TDLS_CHANNEL_SWITCH_TIMEOUT);

		MakeOutgoingFrame(pOutBuffer + FrameLen,			&TempLen,
							1,				&TDLS_IE,
							1,				&TDLS_IE_LEN,
							2,				&SwitchTimeBuf,
							2,				&SwitchTimeOutBuf,
							END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}while(0);

	pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.IsentCHSW = TRUE;

	/* get pEntry */
	pMacEntry = MacTableLookup(pAd, peer);

	if (pMacEntry == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("tdls_cmd> ERROR! No such peer in %s!\n",
				__func__));
		return -1;
	}

	RTMPToWirelessSta(pAd, pMacEntry, Header802_3,
						LENGTH_802_3, pOutBuffer, (UINT)FrameLen, FALSE);
	MlmeFreeMemory(pAd, pOutBuffer);
	return 0;
}

int cfg_tdls_chsw_resp(
PRTMP_ADAPTER	pAd,
u8 *peer,
UINT32 ch_sw_time,
UINT32 ch_sw_timeout,
u16 status_code
)
{
	UCHAR	TDLS_ETHERTYPE[] = {0x89, 0x0d};
	UCHAR	Header802_3[14];
	PUCHAR	pOutBuffer = NULL;
	ULONG	FrameLen = 0;
	ULONG	TempLen;
	UCHAR	RemoteFrameType = PROTO_NAME_TDLS;
	NDIS_STATUS	NStatus = NDIS_STATUS_SUCCESS;
	UINT8	category = CATEGORY_TDLS;
	UCHAR 	TDLS_IE = IE_TDLS_LINK_IDENTIFIER;
	UCHAR 	TDLS_IE_LEN = TDLS_ELM_LEN_LINK_IDENTIFIER;
	u8 action_code = TDLS_ACTION_CODE_CHANNEL_SWITCH_RESPONSE;
	INT tdls_entry_link_index=0;
	MAC_TABLE_ENTRY	*pMacEntry = NULL;
	UINT16 SwitchTimeBuf;
	UINT16 SwitchTimeOutBuf;

	/* Allocate buffer for transmitting message */
	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
	if (NStatus != NDIS_STATUS_SUCCESS)
	{
		DBGPRINT(RT_DEBUG_ERROR,("ACT - TDLS_SetupRequestAction() allocate memory failed \n"));
		return NStatus;
	}


	MAKE_802_3_HEADER(Header802_3, peer , pAd->CurrentAddress, TDLS_ETHERTYPE);

	/* fill action code */
	MakeOutgoingFrame(pOutBuffer,		&TempLen,
					1,				&RemoteFrameType,
					END_OF_ARGS);
	FrameLen = FrameLen + TempLen;

	MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&category,
						1,				&action_code,
						END_OF_ARGS);
	FrameLen = FrameLen + TempLen;


	/* fill status code */
	status_code = cpu2le16(status_code);
	MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen,
		2, &status_code,
		END_OF_ARGS);
	FrameLen = FrameLen + TempLen;

	/* fill link identifier */
	tdls_entry_link_index = cfg_tdls_search_ValidLinkIndex(pAd,peer);
	if (tdls_entry_link_index == -1) {
		MlmeFreeMemory(pAd, pOutBuffer);
		return -1;
	}

	if(pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[tdls_entry_link_index].bInitiator)
	{
		//We are initiator.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				pAd->CurrentAddress,
						6,				peer,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}
	else
	{
		//We are repsonder.
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
						1,				&TDLS_IE,
						1,				&TDLS_IE_LEN,
						6,				pAd->CommonCfg.Bssid,
						6,				peer,
						6,				pAd->CurrentAddress,
						END_OF_ARGS);
		FrameLen = FrameLen + TempLen;

	}

	/* Channel Switch Timing */
	TDLS_IE = IE_TDLS_CHANNEL_SWITCH_TIMING;
	TDLS_IE_LEN = 4;
	SwitchTimeBuf = cpu2le16(ch_sw_time);
	SwitchTimeOutBuf = cpu2le16(ch_sw_timeout);

	MakeOutgoingFrame(pOutBuffer + FrameLen, &TempLen,
		1, &TDLS_IE,
		1, &TDLS_IE_LEN,
		2, &SwitchTimeBuf,
		2, &SwitchTimeOutBuf,
		END_OF_ARGS);
	FrameLen = FrameLen + TempLen;

	DBGPRINT(RT_DEBUG_INFO, ("%s(): CHSW Resp Status:%d , SwitchTime:%d , SwitchTimeout:%d\n",
			__func__, status_code, ch_sw_time, ch_sw_timeout));

	/* get pEntry */
	pMacEntry = MacTableLookup(pAd, peer);

	if (pMacEntry == NULL)
	{
		DBGPRINT(RT_DEBUG_ERROR, ("tdls_cmd> ERROR! No such peer in %s!\n",
				__func__));
		return -1;
	}

	RTMPToWirelessSta(pAd, pMacEntry, Header802_3, LENGTH_802_3, pOutBuffer,
						(UINT)FrameLen, FALSE);
	MlmeFreeMemory(pAd, pOutBuffer);

	return 0;
}

void cfg_tdls_prepare_null_frame(PRTMP_ADAPTER	pAd,BOOLEAN powersave,UCHAR dir,UCHAR *peerAddr)
{
	PMAC_TABLE_ENTRY pEntry_TDLS = MacTableLookup(pAd,peerAddr);

	RtmpPrepareHwNullFrame(pAd,
						&pAd->MacTab.Content[BSSID_WCID],
						FALSE,
						FALSE,
						0,
						OPMODE_STA,
						PWR_SAVE,
						0,
						0);
	RtmpPrepareHwNullFrame(pAd,
						pEntry_TDLS,
						TRUE,
						FALSE,
						0,
						OPMODE_STA,
						PWR_ACTIVE,
						TRUE,
						1);
	DBGPRINT(RT_DEBUG_WARN, ("\n=====>  %s() !!\n", __func__));
	return;
}
static void cfg_tdls_send_CH_SW_SETUP_callback(struct cmd_msg *msg, char *rsp_payload, u16 rsp_payload_len)
{
	INT32 chsw_fw_resp = 0;
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *)msg->priv;
	if (rsp_payload_len < 4)
	{
		DBGPRINT(RT_DEBUG_OFF, ("ERROR, payload len (%d) < 8\n", rsp_payload_len));
		return;
	}
	NdisMoveMemory(&chsw_fw_resp, rsp_payload, sizeof(INT32));
	chsw_fw_resp = le2cpu32(chsw_fw_resp);
	if(chsw_fw_resp == 0)
	{
		pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.IamInOffChannel = TRUE;
	}
	else
	{
		//BOOLEAN TimerCancelled;
		//RTMPCancelTimer(&pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.BaseChannelSwitchTimer, &TimerCancelled);
		pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.IamInOffChannel = FALSE;
	}

	DBGPRINT(RT_DEBUG_WARN, ("%s : FW response  (%d)   !!!\n", __func__, chsw_fw_resp));

	return;
}

int cfg_tdls_send_CH_SW_SETUP(
RTMP_ADAPTER *ad,
UCHAR cmd,
UINT32 start_time_tsf
)
{
	PCFG_TDLS_STRUCT pCfgTdls = &ad->StaCfg.wpa_supplicant_info.CFG_Tdls_info;
	UCHAR ext_base;
	struct cmd_msg *msg;
	u32 value = 0;
	int ret;
	msg = andes_alloc_cmd_msg(ad, 24);

	if (!msg)
	{
		ret = NDIS_STATUS_RESOURCES;
		return ret;
	}

	andes_init_cmd_msg(msg, CMD_TDLS_CH_SW, TRUE, 0, TRUE, TRUE, 4, NULL, cfg_tdls_send_CH_SW_SETUP_callback);

	value &= ~SC_PARAM1_CHL_MASK;
	value |= SC_PARAM1_CHL(cmd);
	andes_append_cmd_msg(msg, (char *)&value, 4);
	if (cmd == 1)//back to base
	{
		;
	}
	else
	{
		if (ad->CommonCfg.BBPCurrentBW != BW_20)
		{
			if (ad->CommonCfg.CentralChannel > ad->CommonCfg.Channel)
				ext_base = EXTCHA_ABOVE;
			else if (ad->CommonCfg.CentralChannel < ad->CommonCfg.Channel)
				ext_base = EXTCHA_BELOW;
			else
				ext_base = EXTCHA_NONE;
		}
		else
			ext_base = EXTCHA_NONE;

		//buffer 1
		value = 0;
		value &= ~SC_PARAM1_CHL_MASK;
		value |= SC_PARAM1_CHL(pCfgTdls->BaseChannel);
		value &= ~SC_PARAM1_SCAN_MASK;
		value |= SC_PARAM1_SCAN(pCfgTdls->TargetOffChannel);
		value &= ~(0x03 << 16);
		value |= ((((ad->CommonCfg.BBPCurrentBW == BW_40)?1:0) & 0x03) << 16);
		value &= ~(0x0C << 16);
		value |= (((pCfgTdls->TargetOffChannelBW) & 0x03) << 18);
		value &= ~(0x1 << 20);
		value |= (((pCfgTdls->bChannelSwitchInitiator) & 0x1) << 20);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
		//buffer 2
		value = 0;
		value &= ~SC_PARAM2_TR_SETTING_MASK;
		value |= SC_PARAM2_TR_SETTING(pCfgTdls->OffChannelStayTime);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
		//buffer 3
		value = 0;
		value &= ~(0x03 << 24);
		value |= (((ext_base) & 0x03) << 24);
		value &= ~(0x0C << 24);
		value |= (((pCfgTdls->TargetOffChannelExt) & 0x03) << 26);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
		//buffer 4
		value = 0;
		value |= start_time_tsf;
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
		//buffer 5
		value = 0;
		value &= ~(0xffff);
		value |= ((pCfgTdls->ChSwitchTime) & 0xffff);
		value &= ~(0xffff << 16);
		value |= (((pCfgTdls->ChSwitchTimeout) & 0xffff) << 16);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}
	ret = andes_send_cmd_msg(ad, msg);

	return ret;
}

void cfg_auto_teardown(PRTMP_ADAPTER pAd, UCHAR *peerAddr)
{
	DBGPRINT(RT_DEBUG_ERROR,
			("%s(): auto teardown link with (%02X:%02X:%02X:%02X:%02X:%02X)!!\n",
			__func__, PRINT_MAC(peerAddr)));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0))
	cfg_tlds_build_frame(pAd, peerAddr, 1, TDLS_ACTION_CODE_TEARDOWN, 0, NULL, 0, FALSE, 0, 25);
	RtmpOsMsDelay(1);
	cfg80211_tdls_oper_request(pAd->net_dev, peerAddr, NL80211_TDLS_TEARDOWN, 25, GFP_KERNEL);
#else
	cfg_tlds_build_frame(pAd, peerAddr, 1, TDLS_ACTION_CODE_TEARDOWN, 0, NULL, 0, FALSE, 0, 25);
	RtmpOsMsDelay(1);
	CFG80211DRV_StaTdlsInsertDeletepEntry(pAd, peerAddr, tdls_delete_entry);
#endif /* LINUX_VERSION_CODE: 3.8 */
	return;
}

void cfg_tdls_LinkMaintenance(PRTMP_ADAPTER pAd)
{
	int i = 0;
	MAC_TABLE_ENTRY *pEntry = NULL;

	for (i = 0; i < MAX_NUM_OF_CFG_TDLS_ENTRY; i++)
	{
		if (pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].EntryValid == TRUE)
		{
			pEntry = &pAd->MacTab.Content[pAd->StaCfg.wpa_supplicant_info.CFG_Tdls_info.TDLSEntry[i].MacTabMatchWCID];
			NdisAcquireSpinLock(&pAd->MacTabLock);
			pEntry->NoDataIdleCount++;
			//pEntry->StaConnectTime++;
			NdisReleaseSpinLock(&pAd->MacTabLock);
			DBGPRINT(RT_DEBUG_TRACE, ("[%s] pEntry->NoDataIdleCount : %lu\n", __func__, pEntry->NoDataIdleCount));
			if (pEntry->NoDataIdleCount >= pEntry->StaIdleTimeout)
			{
				DBGPRINT(RT_DEBUG_OFF, ("%s(): pEntry->NoDataIdleCount > %d  Age out TDLS link (%02X:%02X:%02X:%02X:%02X:%02X)!!\n",
				__func__, pEntry->StaIdleTimeout, PRINT_MAC(pEntry->Addr)));
				RTEnqueueInternalCmd(pAd, CMDTHREAD_TDLS_AUTO_TEARDOWN, pEntry->Addr, MAC_ADDR_LEN);
				pEntry->NoDataIdleCount = 0;
			}
		}
	}
}

/*
==========================================================================
	Description: WFD

	IRQL = PASSIVE_LEVEL
==========================================================================
*/
VOID cfg_tdls_TunneledProbeRequest(
	PRTMP_ADAPTER pAd,
	const u8 *pMacAddr,
	const u8 *extra_ies,
	size_t extra_ies_len)
{
	UCHAR	TDLS_ETHERTYPE[] = {0x89, 0x0d};
	UCHAR	Header802_3[14];
	PUCHAR	pOutBuffer = NULL;
	ULONG	FrameLen = 0;
	ULONG	TempLen;
	UCHAR	RemoteFrameType = PROTO_NAME_TDLS;
	NDIS_STATUS	NStatus = NDIS_STATUS_SUCCESS;
	UCHAR	Category = 0x7F;
	UCHAR	OUI[3] = {0x50, 0x6F, 0x9A};
	UCHAR	FrameBodyType = 4; /* 4: Request. 5: Response. */

	DBGPRINT(RT_DEBUG_TRACE, ("TDLS ===> TDLS_TunneledProbeRequest\n"));

	MAKE_802_3_HEADER(Header802_3, pMacAddr, pAd->CurrentAddress, TDLS_ETHERTYPE);

	// Allocate buffer for transmitting message
	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
	if (NStatus	!= NDIS_STATUS_SUCCESS)
		return;

	MakeOutgoingFrame(pOutBuffer,		&TempLen,
						1,				&RemoteFrameType,
						1,				&Category,
						3,				&OUI,
						1,				&FrameBodyType,
						END_OF_ARGS);

	FrameLen = FrameLen + TempLen;

	/* add extra_ies */
	if(extra_ies_len != 0)
	{
		hex_dump("extra_ies from supplicant ", (UCHAR*)extra_ies, extra_ies_len);
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
					extra_ies_len,				extra_ies,
					END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}

	RTMPToWirelessSta(pAd, &pAd->MacTab.Content[BSSID_WCID], Header802_3,
						LENGTH_802_3, pOutBuffer, (UINT)FrameLen, FALSE);

	hex_dump("TDLS tunneled request send pack", pOutBuffer, FrameLen);

	MlmeFreeMemory(pAd, pOutBuffer);

	DBGPRINT(RT_DEBUG_TRACE, ("TDLS <=== TDLS_TunneledProbeRequest\n"));

	return;
}

/*
==========================================================================
	Description: WFD

	IRQL = PASSIVE_LEVEL
==========================================================================
*/
VOID cfg_tdls_TunneledProbeResponse(
	PRTMP_ADAPTER pAd,
	const u8 *pMacAddr,
	const u8  *extra_ies,
	size_t extra_ies_len)
{
	UCHAR	TDLS_ETHERTYPE[] = {0x89, 0x0d};
	UCHAR	Header802_3[14];
	PUCHAR	pOutBuffer = NULL;
	ULONG	FrameLen = 0;
	ULONG	TempLen;
	UCHAR	RemoteFrameType = PROTO_NAME_TDLS;
	NDIS_STATUS	NStatus = NDIS_STATUS_SUCCESS;
	UCHAR	Category = 0x7F;
	UCHAR	OUI[3] = {0x50, 0x6F, 0x9A};
	UCHAR	FrameBodyType = 5; /* 4: Request. 5: Response. */

	DBGPRINT(RT_DEBUG_TRACE, ("TDLS ===> TDLS_TunneledProbeResponse\n"));

	MAKE_802_3_HEADER(Header802_3, pMacAddr, pAd->CurrentAddress, TDLS_ETHERTYPE);

	// Allocate buffer for transmitting message
	NStatus = MlmeAllocateMemory(pAd, &pOutBuffer);
	if (NStatus	!= NDIS_STATUS_SUCCESS)
		return;

	MakeOutgoingFrame(pOutBuffer,		&TempLen,
						1,				&RemoteFrameType,
						1,				&Category,
						3,				&OUI,
						1,				&FrameBodyType,
						END_OF_ARGS);

	FrameLen = FrameLen + TempLen;

	/* add extra_ies */
	if(extra_ies_len != 0)
	{
		hex_dump("extra_ies from supplicant ", (UCHAR*)extra_ies, extra_ies_len);
		MakeOutgoingFrame(pOutBuffer + FrameLen,		&TempLen,
					extra_ies_len,				extra_ies,
					END_OF_ARGS);
		FrameLen = FrameLen + TempLen;
	}

	RTMPToWirelessSta(pAd, &pAd->MacTab.Content[BSSID_WCID], Header802_3,
						LENGTH_802_3, pOutBuffer, (UINT)FrameLen, FALSE);

	hex_dump("TDLS tunneled response send pack", pOutBuffer, FrameLen);

	MlmeFreeMemory(pAd, pOutBuffer);

	DBGPRINT(RT_DEBUG_TRACE, ("TDLS <=== TDLS_TunneledProbeResponse\n"));

	return;
}
#endif /*CFG_TDLS_SUPPORT*/
#endif /* RT_CFG80211_SUPPORT */
