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

static BOOLEAN RT_isLegalCmdBeforeInfUp(IN PSTRING SetCmd);

INT ComputeChecksum(IN UINT PIN)
{
	INT digit_s;
	UINT accum = 0;

	PIN *= 10;
	accum += 3 * ((PIN / 10000000) % 10);
	accum += 1 * ((PIN / 1000000) % 10);
	accum += 3 * ((PIN / 100000) % 10);
	accum += 1 * ((PIN / 10000) % 10);
	accum += 3 * ((PIN / 1000) % 10);
	accum += 1 * ((PIN / 100) % 10);
	accum += 3 * ((PIN / 10) % 10);

	digit_s = (accum % 10);
	return ((10 - digit_s) % 10);
}				/* ComputeChecksum */

UINT GenerateWpsPinCode(IN PRTMP_ADAPTER pAd, IN BOOLEAN bFromApcli, IN UCHAR apidx)
{
	UCHAR macAddr[MAC_ADDR_LEN];
	UINT iPin;
	UINT checksum;

	NdisZeroMemory(macAddr, MAC_ADDR_LEN);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
#ifdef APCLI_SUPPORT
		if (bFromApcli)
			NdisMoveMemory(&macAddr[0], pAd->ApCfg.ApCliTab[apidx].wdev.if_addr,
				       MAC_ADDR_LEN);
		else
#endif /* APCLI_SUPPORT */
			NdisMoveMemory(&macAddr[0], pAd->ApCfg.MBSSID[apidx].wdev.if_addr,
				       MAC_ADDR_LEN);
	}
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	    NdisMoveMemory(&macAddr[0], pAd->CurrentAddress, MAC_ADDR_LEN);
#endif /* CONFIG_STA_SUPPORT */

	iPin = macAddr[3] * 256 * 256 + macAddr[4] * 256 + macAddr[5];

	iPin = iPin % 10000000;

	checksum = ComputeChecksum(iPin);
	iPin = iPin * 10 + checksum;

	return iPin;
}

static char *phy_mode_str[] = { "CCK", "OFDM", "HTMIX", "GF", "VHT" };

char *get_phymode_str(int Mode)
{
	if (Mode >= MODE_CCK && Mode <= MODE_VHT)
		return phy_mode_str[Mode];
	else
		return "N/A";
}

/*
 *  Add "BOTH" to phy_bw_str[] due to the definition of BAND_WIDTH_ in include/oid.h.
 *  Refer: Klocwork ID: 191.
 */
static UCHAR *phy_bw_str[] = { "20M", "40M", "80M", "BOTH", "10M" };

char *get_bw_str(int bandwidth)
{
	if (bandwidth >= BW_20 && bandwidth <= BW_10)
		return phy_bw_str[bandwidth];
	else
		return "N/A";
}

/*
    ==========================================================================
    Description:
	Set Country Region to pAd->CommonCfg.CountryRegion.
	This command will not work, if the field of CountryRegion in eeprom is programmed.

    Return:
	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetCountryRegion(IN PRTMP_ADAPTER pAd, IN PSTRING arg, IN INT band)
{
	LONG region;
	UCHAR *pCountryRegion;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;


	region = longtmp;

	if (band == BAND_24G) {
#ifdef DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION
		extern UCHAR CountryRegion_SetDuringIfDownFlag;
		extern UCHAR CountryRegion_SetDuringIfDown;

		if (CountryRegion_SetDuringIfDownFlag)
			region = (LONG) CountryRegion_SetDuringIfDown;
#endif /* DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION */
		pCountryRegion = &pAd->CommonCfg.CountryRegion;
	} else {
#ifdef DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION
		extern UCHAR CountryRegionABand_SetDuringIfDownFlag;
		extern UCHAR CountryRegionABand_SetDuringIfDown;

		if (CountryRegionABand_SetDuringIfDownFlag)
			region = (LONG) CountryRegionABand_SetDuringIfDown;
#endif /* DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION */
		pCountryRegion = &pAd->CommonCfg.CountryRegionForABand;
	}

	/*
	   1. If this value is set before interface up, do not reject this value.
	   2. Country can be set only when EEPROM not programmed
	 */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)
	    && (*pCountryRegion & EEPROM_IS_PROGRAMMED)) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("CfgSetCountryRegion():CountryRegion in eeprom was programmed\n"));
		return FALSE;
	}

	if ((region >= 0) &&
	    (((band == BAND_24G) && ((region <= REGION_MAXIMUM_BG_BAND) ||
					(region == REGION_31_BG_BAND) ||
					(region == REGION_32_BG_BAND) ||
					(region == REGION_33_BG_BAND))) ||
		((band == BAND_5G) && (region <= REGION_MAXIMUM_A_BAND)) ||
		region == REGION_IDX_CERTIFICATION)
		) {
		*pCountryRegion = (UCHAR) region;
	} else {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("CfgSetCountryRegion():region(%ld) out of range!\n", region));
		return FALSE;
	}

	return TRUE;

}

static UCHAR CFG_WMODE_MAP[] = {
	PHY_11BG_MIXED, (WMODE_B | WMODE_G),	/* 0 => B/G mixed */
	PHY_11B, (WMODE_B),	/* 1 => B only */
	PHY_11A, (WMODE_A),	/* 2 => A only */
	PHY_11ABG_MIXED, (WMODE_A | WMODE_B | WMODE_G),	/* 3 => A/B/G mixed */
	PHY_11G, WMODE_G,	/* 4 => G only */
	PHY_11ABGN_MIXED, (WMODE_B | WMODE_G | WMODE_GN | WMODE_A | WMODE_AN),	/* 5 => A/B/G/GN/AN mixed */
	PHY_11N_2_4G, (WMODE_GN),	/* 6 => N in 2.4G band only */
	PHY_11GN_MIXED, (WMODE_G | WMODE_GN),	/* 7 => G/GN, i.e., no CCK mode */
	PHY_11AN_MIXED, (WMODE_A | WMODE_AN),	/* 8 => A/N in 5 band */
	PHY_11BGN_MIXED, (WMODE_B | WMODE_G | WMODE_GN),	/* 9 => B/G/GN mode */
	PHY_11AGN_MIXED, (WMODE_G | WMODE_GN | WMODE_A | WMODE_AN),	/* 10 => A/AN/G/GN mode, not support B mode */
	PHY_11N_5G, (WMODE_AN),	/* 11 => only N in 5G band */
#ifdef DOT11_VHT_AC
	/* 12 => B/G/GN/A/AN/AC mixed */
	PHY_11VHT_N_ABG_MIXED, (WMODE_B | WMODE_G | WMODE_GN | WMODE_A | WMODE_AN | WMODE_AC),
	/* 13 => G/GN/A/AN/AC mixed , no B mode */
	PHY_11VHT_N_AG_MIXED, (WMODE_G | WMODE_GN | WMODE_A | WMODE_AN | WMODE_AC),
	PHY_11VHT_N_A_MIXED, (WMODE_A | WMODE_AN | WMODE_AC),	/* 14 => A/AC/AN mixed */
	PHY_11VHT_N_MIXED, (WMODE_AN | WMODE_AC),	/* 15 => AC/AN mixed, but no A mode */
#endif /* DOT11_VHT_AC */
	PHY_MODE_MAX, WMODE_INVALID	/* default phy mode if not match */
};

static PSTRING BAND_STR[] = { "Invalid", "2.4G", "5G", "2.4G/5G" };
static PSTRING WMODE_STR[] = { "", "A", "B", "G", "gN", "aN", "AC" };

UCHAR *wmode_2_str(UCHAR wmode)
{
	UCHAR *str;
	INT idx, pos, max_len;

	max_len = WMODE_COMP * 3;
	if (os_alloc_mem(NULL, &str, max_len) == NDIS_STATUS_SUCCESS) {
		NdisZeroMemory(str, max_len);
		pos = 0;
		for (idx = 0; idx < WMODE_COMP; idx++) {
			if (wmode & (1 << idx)) {
				if ((strlen(str) + strlen(WMODE_STR[idx + 1])) >= (max_len - 1))
					break;
				if (strlen(str)) {
					NdisMoveMemory(&str[pos], "/", 1);
					pos++;
				}
				NdisMoveMemory(&str[pos], WMODE_STR[idx + 1],
					       strlen(WMODE_STR[idx + 1]));
				pos += strlen(WMODE_STR[idx + 1]);
			}
			if (strlen(str) >= max_len)
				break;
		}

		return str;
	} else
		return NULL;
}

RT_802_11_PHY_MODE wmode_2_cfgmode(UCHAR wmode)
{
	INT i, mode_cnt = sizeof(CFG_WMODE_MAP) / (sizeof(UCHAR) * 2);

	for (i = 1; i < mode_cnt; i += 2) {
		if (CFG_WMODE_MAP[i] == wmode)
			return CFG_WMODE_MAP[i - 1];
	}

	DBGPRINT(RT_DEBUG_ERROR, ("%s(): Cannot get cfgmode by wmode(%x)\n", __func__, wmode));

	return 0;
}

UCHAR cfgmode_2_wmode(UCHAR cfg_mode)
{
	DBGPRINT(RT_DEBUG_OFF, ("cfg_mode=%d\n", cfg_mode));
	if (cfg_mode >= PHY_MODE_MAX)
		cfg_mode = PHY_MODE_MAX;

	return CFG_WMODE_MAP[cfg_mode * 2 + 1];
}

static BOOLEAN wmode_valid(RTMP_ADAPTER *pAd, enum WIFI_MODE wmode)
{
	if ((WMODE_CAP_5G(wmode) && (!PHY_CAP_5G(pAd->chipCap.phy_caps))) ||
	    (WMODE_CAP_2G(wmode) && (!PHY_CAP_2G(pAd->chipCap.phy_caps))) ||
	    (WMODE_CAP_N(wmode) && RTMP_TEST_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N))
	    )
		return FALSE;
	else
		return TRUE;
}

static BOOLEAN wmode_band_equal(UCHAR smode, UCHAR tmode)
{
	BOOLEAN eq = FALSE;
	UCHAR *str1, *str2;

	if ((WMODE_CAP_5G(smode) == WMODE_CAP_5G(tmode)) &&
	    (WMODE_CAP_2G(smode) == WMODE_CAP_2G(tmode)))
		eq = TRUE;

	str1 = wmode_2_str(smode);
	str2 = wmode_2_str(tmode);
	if (str1 && str2) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Old WirelessMode:%s(0x%x), New WirelessMode:%s(0x%x)!\n",
			 str1, smode, str2, tmode));
	}
	if (str1)
		os_free_mem(NULL, str1);
	if (str2)
		os_free_mem(NULL, str2);

	return eq;
}

/*
    ==========================================================================
    Description:
	Set Wireless Mode
    Return:
	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetWirelessMode(RTMP_ADAPTER *pAd, PSTRING arg)
{
	LONG cfg_mode;
	UCHAR wmode, *mode_str;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	LONG longtmp;
	INT ret;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	cfg_mode = longtmp;

	/* check if chip support 5G band when WirelessMode is 5G band */
	wmode = cfgmode_2_wmode((UCHAR) cfg_mode);
	if ((wmode == WMODE_INVALID) || (!wmode_valid(pAd, wmode))) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s(): Invalid wireless mode(%ld, wmode=0x%x), ChipCap(%s)\n",
			  __func__, cfg_mode, wmode, BAND_STR[pAd->chipCap.phy_caps & 0x3]));
		return FALSE;
	}
#ifdef MT76x2
	if (pChipCap->ac_off_mode && WMODE_CAP_AC(wmode)) {
		DBGPRINT(RT_DEBUG_ERROR, ("it doesn't support VHT AC!\n"));
		wmode &= ~(WMODE_AC);
	}
#endif /* MT76x2 */

	if (wmode_band_equal(pAd->CommonCfg.PhyMode, wmode) == TRUE)
		DBGPRINT(RT_DEBUG_OFF, ("wmode_band_equal(): Band Equal!\n"));
	else
		DBGPRINT(RT_DEBUG_OFF, ("wmode_band_equal(): Band Not Equal!\n"));

	pAd->CommonCfg.PhyMode = wmode;
	pAd->CommonCfg.cfg_wmode = wmode;

	mode_str = wmode_2_str(wmode);
	if (mode_str) {
		DBGPRINT(RT_DEBUG_TRACE, ("%s(): Set WMODE=%s(0x%x)\n", __func__, mode_str, wmode));
		os_free_mem(NULL, mode_str);
	}

	return TRUE;
}

/* maybe can be moved to GPL code, ap_mbss.c, but the code will be open */
#ifdef CONFIG_AP_SUPPORT
#ifdef MBSS_SUPPORT
static UCHAR RT_CfgMbssWirelessModeMaxGet(RTMP_ADAPTER *pAd)
{
	UCHAR wmode = 0, *mode_str;
	INT idx;
	struct wifi_dev *wdev;

	for (idx = 0; idx < pAd->ApCfg.BssidNum; idx++) {
		wdev = &pAd->ApCfg.MBSSID[idx].wdev;
		mode_str = wmode_2_str(wdev->PhyMode);
		if (mode_str) {
			DBGPRINT(RT_DEBUG_TRACE, ("%s(BSS%d): wmode=%s(0x%x)\n",
						  __func__, idx, mode_str, wdev->PhyMode));
			os_free_mem(pAd, mode_str);
		}
		wmode |= wdev->PhyMode;
	}

	mode_str = wmode_2_str(wmode);
	if (mode_str) {
		DBGPRINT(RT_DEBUG_TRACE, ("%s(): Combined WirelessMode = %s(0x%x)\n",
					  __func__, mode_str, wmode));
		os_free_mem(pAd, mode_str);
	}
	return wmode;
}

/*
    ==========================================================================
    Description:
	Set Wireless Mode for MBSS
    Return:
	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetMbssWirelessMode(RTMP_ADAPTER *pAd, PSTRING arg)
{
	INT cfg_mode;
	UCHAR wmode;
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;
	LONG longtmp;
	INT ret;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	cfg_mode = longtmp;

	wmode = cfgmode_2_wmode((UCHAR) cfg_mode);
	if ((wmode == WMODE_INVALID) || (!wmode_valid(pAd, wmode))) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s(): Invalid wireless mode(%d, wmode=0x%x), ChipCap(%s)\n",
			  __func__, cfg_mode, wmode, BAND_STR[pAd->chipCap.phy_caps & 0x3]));
		return FALSE;
	}

	if (WMODE_CAP_5G(wmode) && WMODE_CAP_2G(wmode)) {
		DBGPRINT(RT_DEBUG_ERROR, ("AP cannot support 2.4G/5G band mxied mode!\n"));
		return FALSE;
	}
#ifdef MT76x2
	if (pChipCap->ac_off_mode && WMODE_CAP_AC(wmode)) {
		DBGPRINT(RT_DEBUG_ERROR, ("it doesn't support VHT AC!\n"));
		wmode &= ~(WMODE_AC);
	}
#endif /* MT76x2 */

	if (pAd->ApCfg.BssidNum > 1) {
		/* pAd->CommonCfg.PhyMode = maximum capability of all MBSS */
		if (wmode_band_equal(pAd->CommonCfg.PhyMode, wmode) == TRUE) {
			wmode = RT_CfgMbssWirelessModeMaxGet(pAd);

			DBGPRINT(RT_DEBUG_TRACE, ("mbss> Maximum phy mode = %d!\n", wmode));
		} else {
			UINT32 IdBss;

			/* replace all phy mode with the one with different band */
			DBGPRINT(RT_DEBUG_TRACE, ("mbss> Different band with the current one!\n"));
			DBGPRINT(RT_DEBUG_TRACE, ("mbss> Reset band of all BSS to the new one!\n"));

			for (IdBss = 0; IdBss < pAd->ApCfg.BssidNum; IdBss++)
				pAd->ApCfg.MBSSID[IdBss].wdev.PhyMode = wmode;
		}
	}

	pAd->CommonCfg.PhyMode = wmode;
	pAd->CommonCfg.cfg_wmode = wmode;
	return TRUE;
}
#endif /* MBSS_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

static BOOLEAN RT_isLegalCmdBeforeInfUp(IN PSTRING SetCmd)
{
	BOOLEAN TestFlag;

	TestFlag = !strcmp(SetCmd, "Debug") ||
#ifdef CONFIG_APSTA_MIXED_SUPPORT
	    !strcmp(SetCmd, "OpMode") ||
#endif /* CONFIG_APSTA_MIXED_SUPPORT */
#ifdef EXT_BUILD_CHANNEL_LIST
	    !strcmp(SetCmd, "CountryCode") ||
	    !strcmp(SetCmd, "DfsType") ||
	    !strcmp(SetCmd, "ChannelListAdd") ||
	    !strcmp(SetCmd, "ChannelListShow") || !strcmp(SetCmd, "ChannelListDel") ||
#endif /* EXT_BUILD_CHANNEL_LIST */
#ifdef SINGLE_SKU
	    !strcmp(SetCmd, "ModuleTxpower") ||
#endif /* SINGLE_SKU */
#ifdef DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION
	    !strcmp(SetCmd, "CountryRegion") || !strcmp(SetCmd, "CountryRegionABand") ||
#endif /* DYNAMIC_PWR_TABLE_MAPPED_BY_COUNTRY_REGION */
	    FALSE;		/* default */
	return TestFlag;
}

INT RT_CfgSetShortSlot(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	LONG ShortSlot;
	INT ret;

	ret = os_strtol(arg, 10, &ShortSlot);
	if (ret < 0)
		return FALSE;

	if (ShortSlot == 1)
		pAd->CommonCfg.bUseShortSlotTime = TRUE;
	else if (ShortSlot == 0)
		pAd->CommonCfg.bUseShortSlotTime = FALSE;
	else
		return FALSE;	/*Invalid argument */

	return TRUE;
}

/*
    ==========================================================================
    Description:
	Set WEP KEY base on KeyIdx
    Return:
	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetWepKey(IN PRTMP_ADAPTER pAd,
		    IN PSTRING keyString, IN CIPHER_KEY * pSharedKey, IN INT keyIdx)
{
	INT KeyLen;
	INT i;
	/*UCHAR                 CipherAlg = CIPHER_NONE; */
	BOOLEAN bKeyIsHex = FALSE;

	/* TODO: Shall we do memset for the original key info?? */
	memset(pSharedKey, 0, sizeof(CIPHER_KEY));
	KeyLen = strlen(keyString);
	switch (KeyLen) {
	case 5:		/*wep 40 Ascii type */
	case 13:		/*wep 104 Ascii type */
		bKeyIsHex = FALSE;
		pSharedKey->KeyLen = KeyLen;
		NdisMoveMemory(pSharedKey->Key, keyString, KeyLen);
		break;

	case 10:		/*wep 40 Hex type */
	case 26:		/*wep 104 Hex type */
		for (i = 0; i < KeyLen; i++) {
			if (!isxdigit(*(keyString + i)))
				return FALSE;	/*Not Hex value; */
		}
		bKeyIsHex = TRUE;
		pSharedKey->KeyLen = KeyLen / 2;
		AtoH(keyString, pSharedKey->Key, pSharedKey->KeyLen);
		break;

	default:		/*Invalid argument */
		DBGPRINT(RT_DEBUG_TRACE,
			 ("RT_CfgSetWepKey(keyIdx=%d):Invalid argument (arg=%s)\n", keyIdx,
			  keyString));
		return FALSE;
	}

	pSharedKey->CipherAlg = ((KeyLen % 5) ? CIPHER_WEP128 : CIPHER_WEP64);
	DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetWepKey:(KeyIdx=%d,type=%s, Alg=%s)\n",
				  keyIdx, (bKeyIsHex == FALSE ? "Ascii" : "Hex"),
				  CipherName[pSharedKey->CipherAlg]));

	return TRUE;
}

/*
    ==========================================================================
    Description:
	Set WPA PSK key

    Arguments:
	pAdapter	Pointer to our adapter
	keyString	WPA pre-shared key string
	pHashStr	String used for password hash function
	hashStrLen	Lenght of the hash string
	pPMKBuf		Output buffer of WPAPSK key

    Return:
	TRUE if all parameters are OK, FALSE otherwise
    ==========================================================================
*/
INT RT_CfgSetWPAPSKKey(IN RTMP_ADAPTER *pAd,
		       IN PSTRING keyString,
		       IN INT keyStringLen,
		       IN UCHAR *pHashStr, IN INT hashStrLen, OUT PUCHAR pPMKBuf)
{
	UCHAR keyMaterial[40];

	if ((keyStringLen < 8) || (keyStringLen > 64)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("WPAPSK Key length(%d) error, required 8 ~ 64 characters!(keyStr=%s)\n",
			  keyStringLen, keyString));
		return FALSE;
	}

	NdisZeroMemory(pPMKBuf, 32);
	if (keyStringLen == 64) {
		AtoH(keyString, pPMKBuf, 32);
	} else {
		RtmpPasswordHash(keyString, pHashStr, hashStrLen, keyMaterial);
		NdisMoveMemory(pPMKBuf, keyMaterial, 32);
	}

	return TRUE;
}

INT RT_CfgSetFixedTxPhyMode(PSTRING arg)
{
	INT fix_tx_mode = FIXED_TXMODE_HT;
	ULONG value;
	INT ret;
	LONG longtmp;

	if (rtstrcasecmp(arg, "OFDM") == TRUE)
		fix_tx_mode = FIXED_TXMODE_OFDM;
	else if (rtstrcasecmp(arg, "CCK") == TRUE)
		fix_tx_mode = FIXED_TXMODE_CCK;
	else if (rtstrcasecmp(arg, "HT") == TRUE)
		fix_tx_mode = FIXED_TXMODE_HT;
	else if (rtstrcasecmp(arg, "VHT") == TRUE)
		fix_tx_mode = FIXED_TXMODE_VHT;
	else {
		ret = os_strtol(arg, 10, &longtmp);
		if (ret < 0)
			return FALSE;

		value = longtmp;
		switch (value) {
		case FIXED_TXMODE_CCK:
		case FIXED_TXMODE_OFDM:
		case FIXED_TXMODE_HT:
		case FIXED_TXMODE_VHT:
			fix_tx_mode = value;
		default:
			fix_tx_mode = FIXED_TXMODE_HT;
		}
	}

	return fix_tx_mode;

}

INT RT_CfgSetMacAddress(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	INT i, mac_len;

	/* Mac address acceptable format 01:02:03:04:05:06 length 17 */
	mac_len = strlen(arg);
	if (mac_len != 17) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s : invalid length (%d)\n", __func__, mac_len));
		return FALSE;
	}

	if (strcmp(arg, "00:00:00:00:00:00") == 0) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s : invalid mac setting\n", __func__));
		return FALSE;
	}

	for (i = 0; i < MAC_ADDR_LEN; i++) {
		AtoH(arg, &pAd->CurrentAddress[i], 1);
		arg = arg + 3;
	}

	pAd->bLocalAdminMAC = TRUE;
	return TRUE;
}

INT RT_CfgSetTxMCSProc(PSTRING arg, BOOLEAN *pAutoRate)
{
	INT ret;
	LONG longtmp;
	INT Value;
	INT TxMcs;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	Value = longtmp;

	if ((Value >= 0 && Value <= 23) || (Value == 32)) {	/* 3*3 */
		TxMcs = Value;
		*pAutoRate = FALSE;
	} else {
		TxMcs = MCS_AUTO;
		*pAutoRate = TRUE;
	}

	return TxMcs;

}

INT RT_CfgSetAutoFallBack(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	INT ret;
	LONG longtmp;
	UCHAR AutoFallBack;


	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	AutoFallBack = (UCHAR) longtmp;

	if (AutoFallBack)
		AutoFallBack = TRUE;
	else
		AutoFallBack = FALSE;

	AsicSetAutoFallBack(pAd, (AutoFallBack) ? TRUE : FALSE);
	DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetAutoFallBack::(AutoFallBack=%d)\n", AutoFallBack));
	return TRUE;
}

#ifdef WSC_INCLUDED
INT RT_CfgSetWscPinCode(IN RTMP_ADAPTER *pAd, IN PSTRING pPinCodeStr, OUT PWSC_CTRL pWscControl)
{
	UINT pinCode;
	INT ret;
	LONG longtmp;

	ret = os_strtol(pPinCodeStr, 10, &longtmp);
	if (ret < 0)
		return;

	/* When PinCode is 03571361, return value is 3571361. */
	pinCode = (UINT) longtmp;
	if (strlen(pPinCodeStr) == 4) {
		pWscControl->WscEnrolleePinCode = pinCode;
		pWscControl->WscEnrolleePinCodeLen = 4;
	} else if (ValidateChecksum(pinCode)) {
		pWscControl->WscEnrolleePinCode = pinCode;
		pWscControl->WscEnrolleePinCodeLen = 8;
	} else {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RT_CfgSetWscPinCode(): invalid Wsc PinCode (%d)\n", pinCode));
		return FALSE;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("RT_CfgSetWscPinCode():Wsc PinCode=%d\n", pinCode));

	return TRUE;

}
#endif /* WSC_INCLUDED */

/*
========================================================================
Routine Description:
	Handler for CMD_RTPRIV_IOCTL_STA_SIOCGIWNAME.

Arguments:
	pAd				- WLAN control block pointer
	*pData			- the communication data pointer
	Data			- the communication data

Return Value:
	NDIS_STATUS_SUCCESS or NDIS_STATUS_FAILURE

Note:
========================================================================
*/
static INT RtmpIoctl_rt_ioctl_giwname(IN RTMP_ADAPTER *pAd, IN VOID * pData, IN ULONG Data)
{
	UCHAR CurOpMode = OPMODE_AP;

	if (CurOpMode == OPMODE_AP)
		strcpy(pData, "RTWIFI SoftAP");

	return NDIS_STATUS_SUCCESS;
}

INT RTMP_COM_IoctlHandle(IN VOID * pAdSrc,
			 IN RTMP_IOCTL_INPUT_STRUCT * wrq,
			 IN INT cmd, IN USHORT subcmd, IN VOID * pData, IN ULONG Data)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) pAdSrc;
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;
	INT Status = NDIS_STATUS_SUCCESS, i;

	pObj = pObj;		/* avoid compile warning */

	switch (cmd) {
	case CMD_RTPRIV_IOCTL_NETDEV_GET:
		/* get main net_dev */
		{
			VOID **ppNetDev = (VOID **) pData;
			*ppNetDev = (VOID *) (pAd->net_dev);
		}
		break;

	case CMD_RTPRIV_IOCTL_NETDEV_SET:
		{
			struct wifi_dev *wdev = NULL;
			/* set main net_dev */
			pAd->net_dev = pData;

#ifdef CONFIG_AP_SUPPORT
			if (pAd->OpMode == OPMODE_AP) {
				pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev.if_dev = (void *)pData;
				pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev.func_dev =
				    (void *)&pAd->ApCfg.MBSSID[MAIN_MBSSID];
				pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev.sys_handle = (void *)pAd;
				RTMP_OS_NETDEV_SET_WDEV(pData,
							&pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev);
				wdev = &pAd->ApCfg.MBSSID[MAIN_MBSSID].wdev;
			}
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
			if (pAd->OpMode == OPMODE_STA) {
				pAd->StaCfg.wdev.if_dev = pData;
				pAd->StaCfg.wdev.func_dev = (void *)&pAd->StaCfg;
				pAd->StaCfg.wdev.sys_handle = (void *)pAd;
				RTMP_OS_NETDEV_SET_WDEV(pData, &pAd->StaCfg.wdev);
				wdev = &pAd->StaCfg.wdev;
			}
#endif /* CONFIG_STA_SUPPORT */
			if (wdev) {
				if (rtmp_wdev_idx_reg(pAd, wdev) < 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("Assign wdev idx for %s failed, free net device!\n",
						  RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev)));
					RtmpOSNetDevFree(pAd->net_dev);
					pAd->net_dev = NULL;
				}
			}
			break;
		}
	case CMD_RTPRIV_IOCTL_OPMODE_GET:
		/* get Operation Mode */
		*(ULONG *) pData = pAd->OpMode;
		break;

	case CMD_RTPRIV_IOCTL_TASK_LIST_GET:
		/* get all Tasks */
		{
			RT_CMD_WAIT_QUEUE_LIST *pList = (RT_CMD_WAIT_QUEUE_LIST *) pData;

			pList->pMlmeTask = &pAd->mlmeTask;
#ifdef RTMP_TIMER_TASK_SUPPORT
			pList->pTimerTask = &pAd->timerTask;
#endif /* RTMP_TIMER_TASK_SUPPORT */
			pList->pCmdQTask = &pAd->cmdQTask;
#ifdef WSC_INCLUDED
			pList->pWscTask = &pAd->wscTask;
#endif /* WSC_INCLUDED */
		}
		break;

#ifdef RTMP_MAC_PCI
	case CMD_RTPRIV_IOCTL_IRQ_INIT:
		/* init IRQ */
		rtmp_irq_init(pAd);
		break;
#endif /* RTMP_MAC_PCI */

	case CMD_RTPRIV_IOCTL_IRQ_RELEASE:
		/* release IRQ */
		RTMP_OS_IRQ_RELEASE(pAd, pAd->net_dev);
		break;

#ifdef RTMP_MAC_PCI
	case CMD_RTPRIV_IOCTL_MSI_ENABLE:
		/* enable MSI */
		RTMP_MSI_ENABLE(pAd);
		*(ULONG **) pData = (ULONG *) (pObj->pci_dev);
		break;
#endif /* RTMP_MAC_PCI */

	case CMD_RTPRIV_IOCTL_NIC_NOT_EXIST:
		/* set driver state to fRTMP_ADAPTER_NIC_NOT_EXIST */
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
		break;

	case CMD_RTPRIV_IOCTL_MCU_SLEEP_CLEAR:
		RTMP_CLEAR_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);
		break;

#ifdef CONFIG_STA_SUPPORT
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	case CMD_RTPRIV_IOCTL_USB_DEV_GET:
		/* get USB DEV */
		{
			VOID **ppUsb_Dev = (VOID **) pData;
			*ppUsb_Dev = (VOID *) (pObj->pUsb_Dev);
		}
		break;

	case CMD_RTPRIV_IOCTL_USB_INTF_GET:
		/* get USB INTF */
		{
			VOID **ppINTF = (VOID **) pData;
			*ppINTF = (VOID *) (pObj->intf);
		}
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_SET:
		/* set driver state to fRTMP_ADAPTER_SUSPEND */
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_CLEAR:
		/* clear driver state to fRTMP_ADAPTER_SUSPEND */
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
		RTMP_CLEAR_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SEND_DISSASSOCIATE:
		/* clear driver state to fRTMP_ADAPTER_SUSPEND */
		if (INFRA_ON(pAd) && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) {
			MLME_DISASSOC_REQ_STRUCT DisReq;
			MLME_QUEUE_ELEM *MsgElem;

			os_alloc_mem(NULL, (UCHAR **) &MsgElem, sizeof(MLME_QUEUE_ELEM));
			if (MsgElem) {
				COPY_MAC_ADDR(DisReq.Addr, pAd->CommonCfg.Bssid);
				DisReq.Reason = REASON_DEAUTH_STA_LEAVING;
				MsgElem->Machine = ASSOC_STATE_MACHINE;
				MsgElem->MsgType = MT2_MLME_DISASSOC_REQ;
				MsgElem->MsgLen = sizeof(MLME_DISASSOC_REQ_STRUCT);
				NdisMoveMemory(MsgElem->Msg, &DisReq,
					       sizeof(MLME_DISASSOC_REQ_STRUCT));
				/* Prevent to connect AP again in STAMlmePeriodicExec */
				pAd->MlmeAux.AutoReconnectSsidLen = 32;
				NdisZeroMemory(pAd->MlmeAux.AutoReconnectSsid,
					       pAd->MlmeAux.AutoReconnectSsidLen);
				pAd->Mlme.CntlMachine.CurrState = CNTL_WAIT_OID_DISASSOC;
				MlmeDisassocReqAction(pAd, MsgElem);
				os_free_mem(NULL, MsgElem);
			}
			/*                              RtmpusecDelay(1000); */
			RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CGIWAP, -1, NULL, NULL,
						0);
		}
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_TEST:
		/* test driver state to fRTMP_ADAPTER_SUSPEND */
		*(UCHAR *) pData = RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_IDLE_RADIO_OFF_TEST:
		/* test driver state to fRTMP_ADAPTER_IDLE_RADIO_OFF */
		*(UCHAR *) pData = RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_OFF:
		ASIC_RADIO_OFF(pAd, SUSPEND_RADIO_OFF);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_ON:
		ASIC_RADIO_ON(pAd, RESUME_RADIO_ON);
		break;

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */

#if (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)
	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_STATUS:
		*(UCHAR *) pData = (UCHAR) pAd->WOW_Cfg.bEnable;
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_ENABLE:
		pAd->WOW_Cfg.bWoWRunning = TRUE;
		ASIC_WOW_ENABLE(pAd);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_DISABLE:
		pAd->WOW_Cfg.bWoWRunning = FALSE;
		ASIC_WOW_DISABLE(pAd);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_READY:
		DBGPRINT(RT_DEBUG_TRACE,
		("WOW_Enable: %d, Adv_WOW_Enable: %d, Connect AP: %d\n",
		pAd->WOW_Cfg.bEnable,
		pAd->WOW_Cfg.bAdvEnable,
		INFRA_ON(pAd)));
		*(UCHAR *) pData = (UCHAR) ((pAd->WOW_Cfg.bEnable || ADV_WOW_ENTRY(pAd))
					    & INFRA_ON(pAd));
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_WOW_RUNSTATUS:
		*(UCHAR *) pData = (UCHAR) ((pAd->WOW_Cfg.bEnable || ADV_WOW_ENTRY(pAd))
					    & pAd->WOW_Cfg.bWoWRunning);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_CMD_RADIO_OFF:
		if (pAd->dp_ctrl.nUsb2DisconMode != 0)
			usb2_disconnect_cmd(pAd, pAd->dp_ctrl.nUsb2DisconMode,
			pAd->dp_ctrl.nGPIONotifyMode,
			pAd->dp_ctrl.nGPIONotifyPin);

		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
			if (pAd->Mlme.CntlMachine.CurrState != CNTL_IDLE) {
				RTMP_MLME_RESET_STATE_MACHINE(pAd);
				DBGPRINT(RT_DEBUG_TRACE,
				("!!! MLME busy, reset MLME state machine !!!\n"));
			}
		}

		ASIC_RADIO_OFF(pAd, SUSPEND_RADIO_OFF);
		pAd->ExtraInfo = SW_RADIO_OFF;
		MacTableReset(pAd);
		pAd->StaCfg.SavedPMKNum = 0;
		RTMPZeroMemory(pAd->StaCfg.SavedPMK, (PMKID_NO * sizeof(BSSID_INFO)));
#ifndef ANDROID_SUPPORT
		/* because abdroid will get scan table when interface down
		   , so we not clean scan table */
		BssTableInit(&pAd->ScanTab);
#endif /* ANDROID_SUPPORT */
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_CMD_RADIO_ON:
		ASIC_RADIO_ON(pAd, RESUME_RADIO_ON);
		/* we expect to disconnect the link, but we cannot
		   call LinkDown() directly in "CMD_RADIO_OFF". Because
		   calling LinkDown() may cause Android system failed
		   to go into suspend due to NETLINK will take the wakelock.
		   So we do LinkDown() in the resume path, just right
		   after RADIO_ON is processed.
		 */
		if (INFRA_ON(pAd)) {
			DBGPRINT(RT_DEBUG_TRACE, ("Resume from radio_off, force LinkDown\n"));
			LinkDown(pAd, FALSE);
		}
		pAd->ExtraInfo = EXTRA_INFO_CLEAR;
		BssTableInit(&pAd->ScanTab);
		RTUSBBulkReceive(pAd);
		break;
#endif /* (defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT) */
#endif /* CONFIG_PM */

	case CMD_RTPRIV_IOCTL_AP_BSSID_GET:
		if (pAd->StaCfg.wdev.PortSecured == WPA_802_1X_PORT_NOT_SECURED)
			NdisCopyMemory(pData, pAd->MlmeAux.Bssid, 6);
		else
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_SET:
		/* set driver state to fRTMP_ADAPTER_SUSPEND */
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_SUSPEND_CLEAR:
		/* clear driver state to fRTMP_ADAPTER_SUSPEND */
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_OFF:
		/* RT28xxUsbAsicRadioOff */
		/* RT28xxUsbAsicRadioOff(pAd); */
		ASIC_RADIO_OFF(pAd, SUSPEND_RADIO_OFF);
		break;

	case CMD_RTPRIV_IOCTL_ADAPTER_RT28XX_USB_ASICRADIO_ON:
		/* RT28xxUsbAsicRadioOn */
		/* RT28xxUsbAsicRadioOn(pAd); */
		ASIC_RADIO_ON(pAd, RESUME_RADIO_ON);
		break;
#endif /* CONFIG_STA_SUPPORT */

	case CMD_RTPRIV_IOCTL_SANITY_CHECK:
		/* sanity check before IOCTL */
		if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)) {
			if (pData == NULL || RT_isLegalCmdBeforeInfUp((PSTRING) pData) == FALSE)
				return NDIS_STATUS_FAILURE;
		}
		break;

	case CMD_RTPRIV_IOCTL_SIOCGIWFREQ:
		/* get channel number */
		*(ULONG *) pData = pAd->CommonCfg.Channel;
		break;
#ifdef CONFIG_SNIFFER_SUPPORT
	case CMD_RTPRIV_IOCTL_SNIFF_INIT:
		Monitor_Init(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_SNIFF_OPEN:
		if (Monitor_Open(pAd, pData) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_SNIFF_CLOSE:
		if (Monitor_Close(pAd, pData) != TRUE)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_SNIFF_REMOVE:
		Monitor_Remove(pAd);
		break;
#endif /*CONFIG_SNIFFER_SUPPORT */

	case CMD_RTPRIV_IOCTL_BEACON_UPDATE:
		/* update all beacon contents */
#ifdef CONFIG_AP_SUPPORT
		APMakeAllBssBeacon(pAd);
		APUpdateAllBeaconFrame(pAd);
#endif /* CONFIG_AP_SUPPORT */
		break;

	case CMD_RTPRIV_IOCTL_RXPATH_GET:
		/* get the number of rx path */
		*(ULONG *) pData = pAd->Antenna.field.RxPath;
		break;

	case CMD_RTPRIV_IOCTL_CHAN_LIST_NUM_GET:
		*(ULONG *) pData = pAd->ChannelListNum;
		break;

	case CMD_RTPRIV_IOCTL_CHAN_LIST_GET:
		{
			UINT32 i;
			UCHAR *pChannel = (UCHAR *) pData;

			for (i = 1; i <= pAd->ChannelListNum; i++) {
				*pChannel = pAd->ChannelList[i - 1].Channel;
				pChannel++;
			}
		}
		break;

	case CMD_RTPRIV_IOCTL_FREQ_LIST_GET:
		{
			UINT32 i;
			UINT32 *pFreq = (UINT32 *) pData;
			UINT32 m;

			for (i = 1; i <= pAd->ChannelListNum; i++) {
				m = 2412000;
				MAP_CHANNEL_ID_TO_KHZ(pAd->ChannelList[i - 1].Channel, m);
				(*pFreq) = m;
				pFreq++;
			}
		}
		break;

#ifdef EXT_BUILD_CHANNEL_LIST
	case CMD_RTPRIV_SET_PRECONFIG_VALUE:
		/* Set some preconfigured value before interface up */
		pAd->CommonCfg.DfsType = MAX_RD_REGION;
		break;
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef RTMP_USB_SUPPORT
	case CMD_RTPRIV_IOCTL_USB_MORE_FLAG_SET:
		{
			RT_CMD_USB_MORE_FLAG_CONFIG *pConfig;
			UINT32 VendorID, ProductID;

			pConfig = (RT_CMD_USB_MORE_FLAG_CONFIG *) pData;
			VendorID = pConfig->VendorID;
			ProductID = pConfig->ProductID;

			if (VendorID == 0x0DB0) {
				if ((ProductID == 0x871C) || (ProductID == 0x822C)) {
					RTMP_SET_MORE_FLAG(pAd,
							   (fRTMP_ADAPTER_DISABLE_DOT_11N |
							    fRTMP_ADAPTER_WSC_PBC_PIN0));
				}
				if ((ProductID == 0x871A) || (ProductID == 0x822A))
					RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N);
				if ((ProductID == 0x871B) || (ProductID == 0x822B))
					RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_WSC_PBC_PIN0);
			}

			if (VendorID == 0x07D1) {
				if (ProductID == 0x3C0F)
					RTMP_SET_MORE_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DOT_11N);
			}
		}
		break;

	case CMD_RTPRIV_IOCTL_USB_CONFIG_INIT:
		{
			RT_CMD_USB_DEV_CONFIG *pConfig;
			UINT32 i;

			pConfig = (RT_CMD_USB_DEV_CONFIG *) pData;

			pAd->NumberOfPipes = pConfig->NumberOfPipes;
			pAd->BulkInMaxPacketSize = pConfig->BulkInMaxPacketSize;
			pAd->BulkOutMaxPacketSize = pConfig->BulkOutMaxPacketSize;

			for (i = 0; i < 6; i++)
				pAd->BulkOutEpAddr[i] = pConfig->BulkOutEpAddr[i];

			for (i = 0; i < 2; i++)
				pAd->BulkInEpAddr[i] = pConfig->BulkInEpAddr[i];

			pAd->IntrInEpAddr = pConfig->IntrInEpAddr;

			pAd->config = pConfig->pConfig;
		}
		break;

	case CMD_RTPRIV_IOCTL_USB_SUSPEND:
		pAd->PM_FlgSuspend = 1;
		if (Data) {
			RTUSBCancelPendingBulkInIRP(pAd);
			RTUSBCancelPendingBulkOutIRP(pAd);
		}
		if (pAd->dp_ctrl.nUsb2DisconMode != 0)
			usb2_disconnect_cmd(pAd, pAd->dp_ctrl.nUsb2DisconMode,
			pAd->dp_ctrl.nGPIONotifyMode,
			pAd->dp_ctrl.nGPIONotifyPin);
		break;

	case CMD_RTPRIV_IOCTL_USB_RESUME:
		pAd->PM_FlgSuspend = 0;
		break;

	case CMD_RTPRIV_IOCTL_USB_INIT:
		InitUSBDevice(pData, pAd);
		break;

#endif /* RTMP_USB_SUPPORT */

#ifdef RTMP_PCI_SUPPORT
	case CMD_RTPRIV_IOCTL_PCI_SUSPEND:
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
		break;

	case CMD_RTPRIV_IOCTL_PCI_RESUME:
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
		break;

	case CMD_RTPRIV_IOCTL_PCI_CSR_SET:
		pAd->CSRBaseAddress = (PUCHAR) Data;
		DBGPRINT(RT_DEBUG_ERROR,
			 ("pAd->CSRBaseAddress =0x%lx, csr_addr=0x%lx!\n",
			  (ULONG) pAd->CSRBaseAddress, (ULONG) Data));
		break;

	case CMD_RTPRIV_IOCTL_PCIE_INIT:
		RTMPInitPCIeDevice(pData, pAd);
		break;
#endif /* RTMP_PCI_SUPPORT */

#ifdef RT_CFG80211_SUPPORT
	case CMD_RTPRIV_IOCTL_CFG80211_CFG_START:
		RT_CFG80211_REINIT(pAd);
#ifdef JUNGLE_SUPPORT
		CFG80211_SyncCore_ChList(pAd, NULL);
#else
		RT_CFG80211_CRDA_REG_RULE_APPLY(pAd);
#endif
		break;
#endif /* RT_CFG80211_SUPPORT */

#ifdef INF_PPA_SUPPORT
	case CMD_RTPRIV_IOCTL_INF_PPA_INIT:
		os_alloc_mem(NULL, (UCHAR **) &(pAd->pDirectpathCb), sizeof(PPA_DIRECTPATH_CB));
		break;

	case CMD_RTPRIV_IOCTL_INF_PPA_EXIT:
		if (ppa_hook_directpath_register_dev_fn && (pAd->PPAEnable == TRUE)) {
			UINT status;

			status =
			    ppa_hook_directpath_register_dev_fn(&pAd->g_if_id, pAd->net_dev, NULL,
								0);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("Unregister PPA::status=%d, if_id=%d\n", status, pAd->g_if_id));
		}
		os_free_mem(NULL, pAd->pDirectpathCb);
		break;
#endif /* INF_PPA_SUPPORT */

	case CMD_RTPRIV_IOCTL_VIRTUAL_INF_UP:
		/* interface up */
		{
			RT_CMD_INF_UP_DOWN *pInfConf = (RT_CMD_INF_UP_DOWN *) pData;

			if (VIRTUAL_IF_NUM(pAd) == 0) {
				if (pInfConf->rt28xx_open(pAd->net_dev) != 0) {
					DBGPRINT(RT_DEBUG_TRACE, ("rt28xx_open return fail!\n"));
					return NDIS_STATUS_FAILURE;
				}
			} else {
#ifdef CONFIG_AP_SUPPORT
				extern VOID APMakeAllBssBeacon(IN PRTMP_ADAPTER pAd);
				extern VOID APUpdateAllBeaconFrame(IN PRTMP_ADAPTER pAd);
				APMakeAllBssBeacon(pAd);
				APUpdateAllBeaconFrame(pAd);
#endif /* CONFIG_AP_SUPPORT */
			}
			VIRTUAL_IF_INC(pAd);
		}
		break;

	case CMD_RTPRIV_IOCTL_VIRTUAL_INF_DOWN:
		/* interface down */
		{
			RT_CMD_INF_UP_DOWN *pInfConf = (RT_CMD_INF_UP_DOWN *) pData;

			VIRTUAL_IF_DEC(pAd);
			if (VIRTUAL_IF_NUM(pAd) == 0)
				pInfConf->rt28xx_close(pAd->net_dev);
		}
		break;

	case CMD_RTPRIV_IOCTL_VIRTUAL_INF_GET:
		/* get virtual interface number */
		*(ULONG *) pData = VIRTUAL_IF_NUM(pAd);
		break;

	case CMD_RTPRIV_IOCTL_INF_TYPE_GET:
		/* get current interface type */
		*(ULONG *) pData = pAd->infType;
		break;

	case CMD_RTPRIV_IOCTL_INF_STATS_GET:
		/* get statistics */
		{
			RT_CMD_STATS *pStats = (RT_CMD_STATS *) pData;

			pStats->pStats = pAd->stats;
			if (pAd->OpMode == OPMODE_STA) {
				pStats->rx_packets =
				    pAd->WlanCounters.ReceivedFragmentCount.QuadPart;
				pStats->tx_packets =
				    pAd->WlanCounters.TransmittedFragmentCount.QuadPart;
				pStats->rx_bytes = pAd->RalinkCounters.ReceivedByteCount;
				pStats->tx_bytes = pAd->RalinkCounters.TransmittedByteCount;
				pStats->rx_errors = pAd->Counters8023.RxErrors;
				pStats->tx_errors = pAd->Counters8023.TxErrors;
				/* multicast packets received */
				pStats->multicast =
				pAd->WlanCounters.MulticastReceivedFrameCount.QuadPart;
				pStats->collisions = 0;	/* Collision packets */
				/* receiver ring buff overflow */
				pStats->rx_over_errors = pAd->Counters8023.RxNoBuffer;
				/*pAd->WlanCounters.FCSErrorCount; recved pkt with crc error */
				pStats->rx_crc_errors = 0;
				pStats->rx_frame_errors = 0;	/* recv'd frame alignment error */
				pStats->rx_fifo_errors = pAd->Counters8023.RxNoBuffer;	/* recv'r fifo overrun */
			}
#ifdef CONFIG_AP_SUPPORT
			else if (pAd->OpMode == OPMODE_AP) {
				INT index;

				for (index = 0; index < MAX_MBSSID_NUM(pAd); index++) {
					if (pAd->ApCfg.MBSSID[index].wdev.if_dev ==
					    (PNET_DEV) (pStats->pNetDev))
						break;

				}

				if (index >= MAX_MBSSID_NUM(pAd)) {
					/* reset counters */
					pStats->rx_packets = 0;
					pStats->tx_packets = 0;
					pStats->rx_bytes = 0;
					pStats->tx_bytes = 0;
					pStats->rx_errors = 0;
					pStats->tx_errors = 0;
					pStats->multicast = 0;	/* multicast packets received */
					pStats->collisions = 0;	/* Collision packets */
					pStats->rx_over_errors = 0;	/* receiver ring buff overflow */
					pStats->rx_crc_errors = 0;	/* recved pkt with crc error */
					pStats->rx_frame_errors = 0;	/* recv'd frame alignment error */
					pStats->rx_fifo_errors = 0;	/* recv'r fifo overrun */

					DBGPRINT(RT_DEBUG_ERROR,
						 ("CMD_RTPRIV_IOCTL_INF_STATS_GET: can not find mbss I/F\n"));
					return NDIS_STATUS_FAILURE;
				}

				pStats->rx_packets = pAd->ApCfg.MBSSID[index].RxCount;
				pStats->tx_packets = pAd->ApCfg.MBSSID[index].TxCount;
				pStats->rx_bytes = pAd->ApCfg.MBSSID[index].ReceivedByteCount;
				pStats->tx_bytes = pAd->ApCfg.MBSSID[index].TransmittedByteCount;
				pStats->rx_errors = pAd->ApCfg.MBSSID[index].RxErrorCount;
				pStats->tx_errors = pAd->ApCfg.MBSSID[index].TxErrorCount;
				pStats->multicast = pAd->ApCfg.MBSSID[index].mcPktsRx;	/* multicast packets received */
				pStats->collisions = 0;	/* Collision packets */
				pStats->rx_over_errors = 0;	/* receiver ring buff overflow */
				pStats->rx_crc_errors = 0;	/* recved pkt with crc error */
				pStats->rx_frame_errors = 0;	/* recv'd frame alignment error */
				pStats->rx_fifo_errors = 0;	/* recv'r fifo overrun */
			}
#endif /* endif */
		}
		break;

	case CMD_RTPRIV_IOCTL_INF_IW_STATUS_GET:
		/* get wireless statistics */
		{
			UCHAR CurOpMode = OPMODE_AP;
#ifdef CONFIG_AP_SUPPORT
			PMAC_TABLE_ENTRY pMacEntry = NULL;
#endif /* CONFIG_AP_SUPPORT */
			RT_CMD_IW_STATS *pStats = (RT_CMD_IW_STATS *) pData;

			pStats->qual = 0;
			pStats->level = 0;
			pStats->noise = 0;
			pStats->pStats = pAd->iw_stats;

#ifdef CONFIG_STA_SUPPORT
			if (pAd->OpMode == OPMODE_STA)
				CurOpMode = OPMODE_STA;
#endif /* CONFIG_STA_SUPPORT */

			/*check if the interface is down */
			if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
				return NDIS_STATUS_FAILURE;

#ifdef CONFIG_AP_SUPPORT
			if (CurOpMode == OPMODE_AP) {
#ifdef APCLI_SUPPORT
				if ((pStats->priv_flags == INT_APCLI)
				    ) {
					INT ApCliIdx =
					    ApCliIfLookUp(pAd, (PUCHAR) pStats->dev_addr);

					if ((ApCliIdx >= 0)
					    && VALID_WCID(pAd->ApCfg.ApCliTab[ApCliIdx].MacTabWCID))
						pMacEntry =
						    &pAd->MacTab.Content[pAd->ApCfg.ApCliTab
									 [ApCliIdx].MacTabWCID];
				} else
#endif /* APCLI_SUPPORT */
				{
					/*
					   only AP client support wireless stats function.
					   return NULL pointer for all other cases.
					 */
					pMacEntry = NULL;
				}
			}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
			if (CurOpMode == OPMODE_STA)
				pStats->qual = ((pAd->Mlme.ChannelQuality * 12) / 10 + 10);
#endif /* CONFIG_STA_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
			if (CurOpMode == OPMODE_AP) {
				if (pMacEntry != NULL)
					pStats->qual = ((pMacEntry->ChannelQuality * 12) / 10 + 10);
				else
					pStats->qual = ((pAd->Mlme.ChannelQuality * 12) / 10 + 10);
			}
#endif /* CONFIG_AP_SUPPORT */

			if (pStats->qual > 100)
				pStats->qual = 100;

#ifdef CONFIG_STA_SUPPORT
			if (CurOpMode == OPMODE_STA) {
				pStats->level =
				    RTMPMaxRssi(pAd, pAd->StaCfg.RssiSample.AvgRssi0,
						pAd->StaCfg.RssiSample.AvgRssi1,
						pAd->StaCfg.RssiSample.AvgRssi2);
			}
#endif /* CONFIG_STA_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
			if (CurOpMode == OPMODE_AP) {
				if (pMacEntry != NULL)
					pStats->level =
					    RTMPMaxRssi(pAd, pMacEntry->RssiSample.AvgRssi0,
							pMacEntry->RssiSample.AvgRssi1,
							pMacEntry->RssiSample.AvgRssi2);
			}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
			pStats->noise = RTMPMaxRssi(pAd, pAd->ApCfg.RssiSample.AvgRssi0,
						    pAd->ApCfg.RssiSample.AvgRssi1,
						    pAd->ApCfg.RssiSample.AvgRssi2) -
			    RTMPMinSnr(pAd, pAd->ApCfg.RssiSample.AvgSnr0,
				       pAd->ApCfg.RssiSample.AvgSnr1);
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
			pStats->noise = RTMPMaxRssi(pAd, pAd->StaCfg.RssiSample.AvgRssi0,
						    pAd->StaCfg.RssiSample.AvgRssi1,
						    pAd->StaCfg.RssiSample.AvgRssi2) -
			    RTMPMinSnr(pAd, pAd->StaCfg.RssiSample.AvgSnr0,
				       pAd->StaCfg.RssiSample.AvgSnr1);
#endif /* CONFIG_STA_SUPPORT */
		}
		break;

	case CMD_RTPRIV_IOCTL_INF_MAIN_CREATE:
		*(VOID **) pData = RtmpPhyNetDevMainCreate(pAd);
		break;

	case CMD_RTPRIV_IOCTL_INF_MAIN_ID_GET:
		*(ULONG *) pData = INT_MAIN;
		break;

	case CMD_RTPRIV_IOCTL_INF_MAIN_CHECK:
		if (Data != INT_MAIN)
			return NDIS_STATUS_FAILURE;
		break;

	case CMD_RTPRIV_IOCTL_INF_P2P_CHECK:
		if (Data != INT_P2P)
			return NDIS_STATUS_FAILURE;
		break;

#ifdef WDS_SUPPORT
	case CMD_RTPRIV_IOCTL_WDS_INIT:
		WDS_Init(pAd, pData);
		break;

	case CMD_RTPRIV_IOCTL_WDS_REMOVE:
		WDS_Remove(pAd);
		break;

	case CMD_RTPRIV_IOCTL_WDS_STATS_GET:
		if (Data == INT_WDS) {
			if (WDS_StatsGet(pAd, pData) != TRUE)
				return NDIS_STATUS_FAILURE;
		} else
			return NDIS_STATUS_FAILURE;
		break;
#endif /* WDS_SUPPORT */

#ifdef RALINK_ATE
#ifdef WCX_SUPPORT
	case CMD_RTPRIV_IOCTL_META_QUERY:
		do_meta_cmd(RTPRIV_IOCTL_META_QUERY, pAd, wrq, pData);
		break;
	case CMD_RTPRIV_IOCTL_META_SET:
		do_meta_cmd(RTPRIV_IOCTL_META_SET, pAd, wrq, pData);
		break;
#endif /* endif */
#ifdef RALINK_QA
	case CMD_RTPRIV_IOCTL_ATE:
		RtmpDoAte(pAd, wrq, pData);
		break;
#endif /* RALINK_QA */
#endif /* RALINK_ATE */

	case CMD_RTPRIV_IOCTL_MAC_ADDR_GET:
		{
			UCHAR mac_addr[MAC_ADDR_LEN];
			USHORT Addr01, Addr23, Addr45;

			RT28xx_EEPROM_READ16(pAd, 0x04, Addr01);
			RT28xx_EEPROM_READ16(pAd, 0x06, Addr23);
			RT28xx_EEPROM_READ16(pAd, 0x08, Addr45);

			mac_addr[0] = (UCHAR) (Addr01 & 0xff);
			mac_addr[1] = (UCHAR) (Addr01 >> 8);
			mac_addr[2] = (UCHAR) (Addr23 & 0xff);
			mac_addr[3] = (UCHAR) (Addr23 >> 8);
			mac_addr[4] = (UCHAR) (Addr45 & 0xff);
			mac_addr[5] = (UCHAR) (Addr45 >> 8);

			for (i = 0; i < 6; i++)
				*(UCHAR *) (pData + i) = mac_addr[i];
			break;
		}
#ifdef CONFIG_AP_SUPPORT
	case CMD_RTPRIV_IOCTL_AP_SIOCGIWRATEQ:
		/* handle for SIOCGIWRATEQ */
		{
			RT_CMD_IOCTL_RATE *pRate = (RT_CMD_IOCTL_RATE *) pData;
			HTTRANSMIT_SETTING HtPhyMode;
			UINT8 BW = 0, GI = 0;

#ifdef APCLI_SUPPORT
			if (pRate->priv_flags == INT_APCLI)
				memcpy(&HtPhyMode,
				       &pAd->ApCfg.ApCliTab[pObj->ioctl_if].wdev.HTPhyMode,
				       sizeof(HTTRANSMIT_SETTING));
			else
#endif /* APCLI_SUPPORT */
#ifdef WDS_SUPPORT
			if (pRate->priv_flags == INT_WDS)
				memcpy(&HtPhyMode,
				       &pAd->WdsTab.WdsEntry[pObj->ioctl_if].wdev.HTPhyMode,
				       sizeof(HTTRANSMIT_SETTING));
			else
#endif /* WDS_SUPPORT */
			{
				memcpy(&HtPhyMode,
				       &pAd->ApCfg.MBSSID[pObj->ioctl_if].wdev.HTPhyMode,
				       sizeof(HTTRANSMIT_SETTING));
#ifdef MBSS_SUPPORT
				/* reset phy mode for MBSS */
				MBSS_PHY_MODE_RESET(pObj->ioctl_if, HtPhyMode);
#endif /* MBSS_SUPPORT */
			}

#ifdef DOT11_VHT_AC
			if (HtPhyMode.field.BW == BW_40 && pAd->CommonCfg.vht_bw == VHT_BW_80
			    && HtPhyMode.field.MODE >= MODE_VHT) {
				BW = 2;
				GI = pAd->CommonCfg.vht_sgi_80;
			} else
#endif /* DOT11_VHT_AC */
			{
				BW = HtPhyMode.field.BW;
				GI = HtPhyMode.field.ShortGI;
			}

			RtmpDrvRateGet(pAd, HtPhyMode.field.MODE, GI,
				       BW, HtPhyMode.field.MCS,
				       pAd->Antenna.field.TxPath, (UINT32 *) &pRate->BitRate);
		}
		break;
#endif /* CONFIG_AP_SUPPORT */

	case CMD_RTPRIV_IOCTL_SIOCGIWNAME:
		RtmpIoctl_rt_ioctl_giwname(pAd, pData, 0);
		break;

	default:
		break;
	}

#ifdef RT_CFG80211_SUPPORT
	if ((CMD_RTPRIV_IOCTL_80211_START <= cmd) && (cmd <= CMD_RTPRIV_IOCTL_80211_END))
		Status = CFG80211DRV_IoctlHandle(pAd, wrq, cmd, subcmd, pData, Data);
#endif /* RT_CFG80211_SUPPORT */

	if (cmd >= CMD_RTPRIV_IOCTL_80211_COM_LATEST_ONE)
		return NDIS_STATUS_FAILURE;

	return Status;
}

/*
    ==========================================================================
    Description:
	Issue a site survey command to driver
	Arguments:
	    pAdapter                    Pointer to our adapter
	    wrq                         Pointer to the ioctl argument

    Return Value:
	None

    Note:
	Usage:
	       1.) iwpriv ra0 set site_survey
    ==========================================================================
*/
INT Set_SiteSurvey_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	NDIS_802_11_SSID Ssid;
	POS_COOKIE pObj;

	pObj = (POS_COOKIE) pAd->OS_Cookie;

	/* check if the interface is down */
	if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)) {
		DBGPRINT(RT_DEBUG_TRACE, ("INFO::Network is down!\n"));
		return -ENETDOWN;
	}
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		if (MONITOR_ON(pAd)) {
			DBGPRINT(RT_DEBUG_TRACE, ("!!! Driver is in Monitor Mode now !!!\n"));
			return -EINVAL;
		}
	}
#endif /* CONFIG_STA_SUPPORT // */

	NdisZeroMemory(&Ssid, sizeof(NDIS_802_11_SSID));

#ifdef CONFIG_AP_SUPPORT
#ifdef AP_SCAN_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		if ((strlen(arg) != 0) && (strlen(arg) <= MAX_LEN_OF_SSID)) {
			NdisMoveMemory(Ssid.Ssid, arg, strlen(arg));
			Ssid.SsidLength = strlen(arg);
		}
#ifndef APCLI_CONNECTION_TRIAL
		if (Ssid.SsidLength == 0)
			ApSiteSurvey(pAd, &Ssid, SCAN_PASSIVE, FALSE);
		else
			ApSiteSurvey(pAd, &Ssid, SCAN_ACTIVE, FALSE);
#else
		/*for shorter scan time. use active scan and send probe req. */
		DBGPRINT(RT_DEBUG_TRACE, ("!!! Fast Scan for connection trial !!!\n"));
		ApSiteSurvey(pAd, &Ssid, FAST_SCAN_ACTIVE, FALSE);
#endif /* APCLI_CONNECTION_TRIAL */

		return TRUE;
	}
#endif /* AP_SCAN_SUPPORT */
#endif /* CONFIG_AP_SUPPORT // */

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		Ssid.SsidLength = 0;
		if ((arg != NULL) && (strlen(arg) <= MAX_LEN_OF_SSID)) {
			RTMPMoveMemory(Ssid.Ssid, arg, strlen(arg));
			Ssid.SsidLength = strlen(arg);
		}

		pAd->StaCfg.bSkipAutoScanConn = TRUE;
		StaSiteSurvey(pAd, &Ssid, SCAN_ACTIVE);
	}
#endif /* CONFIG_STA_SUPPORT // */

	DBGPRINT(RT_DEBUG_TRACE, ("Set_SiteSurvey_Proc\n"));

	return TRUE;
}

INT Set_Antenna_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	ANT_DIVERSITY_TYPE UsedAnt;
	int i;
	INT ret;
	LONG longtmp;



	DBGPRINT(RT_DEBUG_OFF, ("==> Set_Antenna_Proc *******************\n"));

	for (i = 0; i < strlen(arg); i++)
		if (!isdigit(arg[i]))
			return -EINVAL;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;
	UsedAnt = longtmp;

	switch (UsedAnt) {
		/* 2: Fix in the PHY Antenna CON1 */
	case ANT_FIX_ANT0:
		AsicSetRxAnt(pAd, 0);
		DBGPRINT(RT_DEBUG_OFF, ("<== Set_Antenna_Proc(Fix in Ant CON1), (%d,%d)\n",
					pAd->RxAnt.Pair1PrimaryRxAnt,
					pAd->RxAnt.Pair1SecondaryRxAnt));
		break;
		/* 3: Fix in the PHY Antenna CON2 */
	case ANT_FIX_ANT1:
		AsicSetRxAnt(pAd, 1);
		DBGPRINT(RT_DEBUG_OFF, ("<== %s(Fix in Ant CON2), (%d,%d)\n",
					__func__, pAd->RxAnt.Pair1PrimaryRxAnt,
					pAd->RxAnt.Pair1SecondaryRxAnt));
		break;
	default:
		DBGPRINT(RT_DEBUG_ERROR, ("<== %s(N/A cmd: %d), (%d,%d)\n", __func__, UsedAnt,
					  pAd->RxAnt.Pair1PrimaryRxAnt,
					  pAd->RxAnt.Pair1SecondaryRxAnt));
		break;
	}

	return TRUE;
}

#ifdef MAC_REPEATER_SUPPORT
#ifdef MULTI_MAC_ADDR_EXT_SUPPORT
INT Set_EnMultiMacAddrExt_Proc(IN RTMP_ADAPTER *pAd, IN PSTRING arg)
{
	UCHAR Enable;
	UINT32 MacReg;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return;

	Enable = longtmp;

	RTMP_IO_READ32(pAd, MAC_ADDR_EXT_EN, &MacReg);
	if (Enable) {
		MacReg |= 0x1;
		pAd->bUseMultiMacAddrExt = TRUE;
		DBGPRINT(RT_DEBUG_TRACE, ("==>UseMultiMacAddrExt (ON)\n"));
	} else {
		MacReg &= (~0x1);
		pAd->bUseMultiMacAddrExt = FALSE;
		DBGPRINT(RT_DEBUG_TRACE, ("==>UseMultiMacAddrExt (OFF)\n"));
	}
	RTMP_IO_WRITE32(pAd, MAC_ADDR_EXT_EN, MacReg);

	DBGPRINT(RT_DEBUG_WARN, ("UseMultiMacAddrExt = %d\n", pAd->bUseMultiMacAddrExt));

	return TRUE;
}

INT Set_MultiMacAddrExt_Proc(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{
	UCHAR tempMAC[6], idx;
	PSTRING token;
	STRING sepValue[] = ":", DASH = '-';
	ULONG offset, Addr;
	INT i;
	INT ret;
	LONG longtmp;

	/*Mac address acceptable format 01:02:03:04:05:06 length 17 plus the "-"
	and tid value in decimal format. */
	if (strlen(arg) < 19)
		return FALSE;

	token = strchr(arg, DASH);
	if ((token != NULL) && (strlen(token) > 1)) {
		ret = os_strtol((token + 1), 10, &longtmp);
		if (ret < 0)
			return FALSE;

		idx = (UCHAR) longtmp;

		if (idx > 15)
			return FALSE;

		*token = '\0';
		for (i = 0, token = rstrtok(arg, &sepValue[0]); token;
		     token = rstrtok(NULL, &sepValue[0]), i++) {
			if ((strlen(token) != 2) || (!isxdigit(*token))
			    || (!isxdigit(*(token + 1))))
				return FALSE;
			AtoH(token, (&tempMAC[i]), 1);
		}

		if (i != 6)
			return FALSE;

		DBGPRINT(RT_DEBUG_OFF, ("\n%02x:%02x:%02x:%02x:%02x:%02x-%02x\n",
					tempMAC[0], tempMAC[1], tempMAC[2], tempMAC[3], tempMAC[4],
					tempMAC[5], idx));

		offset = 0x1480 + (HW_WCID_ENTRY_SIZE * idx);
		Addr = tempMAC[0] + (tempMAC[1] << 8) + (tempMAC[2] << 16) + (tempMAC[3] << 24);
		RTMP_IO_WRITE32(pAd, offset, Addr);
		Addr = tempMAC[4] + (tempMAC[5] << 8);
		RTMP_IO_WRITE32(pAd, offset + 4, Addr);

		return TRUE;
	}

	return FALSE;
}
#endif /* MULTI_MAC_ADDR_EXT_SUPPORT */
#endif /* MAC_REPEATER_SUPPORT */

INT set_tssi_enable(RTMP_ADAPTER *pAd, PSTRING arg)
{
	UINT8 tssi_enable = 0;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	tssi_enable = longtmp;

	if (tssi_enable == 1) {
		pAd->chipCap.tssi_enable = TRUE;
		DBGPRINT(RT_DEBUG_OFF, ("turn on TSSI mechanism\n"));
	} else if (tssi_enable == 0) {
		pAd->chipCap.tssi_enable = FALSE;
		DBGPRINT(RT_DEBUG_OFF, ("turn off TSS mechanism\n"));
	} else {
		DBGPRINT(RT_DEBUG_OFF, ("illegal param(%u)\n", tssi_enable));
		return FALSE;
	}
	return TRUE;
}

#ifdef CONFIG_WIFI_TEST
INT set_pbf_loopback(RTMP_ADAPTER *pAd, PSTRING arg)
{
	UINT8 enable = 0;
	UINT32 value;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	enable = longtmp;
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &value);

	if (enable == 1) {
		pAd->chipCap.pbf_loopback = TRUE;
		value |= PBF_LOOP_EN;
		DBGPRINT(RT_DEBUG_OFF, ("turn on pbf loopback\n"));
	} else if (enable == 0) {
		pAd->chipCap.pbf_loopback = FALSE;
		value &= ~PBF_LOOP_EN;
		DBGPRINT(RT_DEBUG_OFF, ("turn off pbf loopback\n"));
	} else {
		DBGPRINT(RT_DEBUG_OFF, ("illegal param\n"));
		return FALSE;
	}

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, value);

	return TRUE;
}

INT set_pbf_rx_drop(RTMP_ADAPTER *pAd, PSTRING arg)
{
	UINT8 enable = 0;
	UINT32 value = 0;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	enable = longtmp;

#ifdef RLT_MAC
	if (pAd->chipCap.hif_type == HIF_RLT)
		RTMP_IO_READ32(pAd, RLT_PBF_CFG, &value);
#endif /* RLT_MAC */

	if (enable == 1) {
		pAd->chipCap.pbf_rx_drop = TRUE;
		value |= RX_DROP_MODE;
		DBGPRINT(RT_DEBUG_OFF, ("turn on pbf loopback\n"));
	} else if (enable == 0) {
		pAd->chipCap.pbf_rx_drop = FALSE;
		value &= ~RX_DROP_MODE;
		DBGPRINT(RT_DEBUG_OFF, ("turn off pbf loopback\n"));
	} else {
		DBGPRINT(RT_DEBUG_OFF, ("illegal param\n"));
		return FALSE;
	}

#ifdef RLT_MAC
	if (pAd->chipCap.hif_type == HIF_RLT)
		RTMP_IO_WRITE32(pAd, RLT_PBF_CFG, value);
#endif /* RLT_MAC */

	return TRUE;
}
#endif /* CONFIG_WIFI_TEST */

#ifdef CONFIG_ANDES_SUPPORT
#ifdef CONFIG_FW_DEBUG
INT set_fw_debug(RTMP_ADAPTER *ad, PSTRING arg)
{
	UINT8 fw_debug_param;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	fw_debug_param = longtmp;

	andes_fun_set(ad, LOG_FW_DEBUG_MSG, fw_debug_param);

	return TRUE;
}
#endif /* CONFIG_FW_DEBUG */
#ifdef CONFIG_FW_CORE_DUMP
INT set_fw_core_dump(RTMP_ADAPTER *ad, PSTRING arg)
{
	UINT8 enable_dump_param;
	INT ret;
	LONG longtmp;

	ret = os_strtol(arg, 10, &longtmp);
	if (ret < 0)
		return FALSE;

	enable_dump_param = longtmp;

	andes_fun_set(ad, CORE_DUMP_TO_HOST, enable_dump_param);

	return TRUE;
}
#endif /* CONFIG_FW_CORE_DUMP */
#endif /* CONFIG_ANDES_SUPPORT */

/* Dump Debug Register Values */
INT Set_Register_Dump(IN PRTMP_ADAPTER pAd, IN PSTRING arg)
{

	UINT32 Value = 0;

	/* Toggle Error */
	DBGPRINT(RT_DEBUG_OFF, ("--dump_registers:--\n"));
	read_reg(pAd, 0x40, 0x2820, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2820: 0x%x\n", Value));
	/* Reset to default */
	/* write_reg(ad, 0x40, 0x2820, 0x1); */
	read_reg(pAd, 0x40, 0x023C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_023C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x024c, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_024C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0254, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0254: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1500, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1500: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1504, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1504: 0x%x\n", Value));
#ifdef RTMP_MAC_USB
	read_reg(pAd, 0x40, 0x9018, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_9018: 0x%x\n", Value));
	if (Value == 0x0)
		DBGPRINT(RT_DEBUG_OFF, ("UDMA not enabled\n"));
#endif
	read_reg(pAd, 0x40, 0x9100, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_9100: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x9110, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_9110: 0x%x\n", Value));

	read_reg(pAd, 0x40, 0x2140, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2140: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2240, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2240: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2280, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2280: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2290, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2290: 0x%x\n", Value));
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	read_reg(pAd, 0x41, 0x1004, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1004: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1200, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1200: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0A38, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0A38: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0A30, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0A30: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0A34, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0A34: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0A0C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0A0C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09C4, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09C4: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09E0, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09E0: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09E8, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09E8: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09F0, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09F0: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09F4, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09F4: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09F8, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09F8: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09FC, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09FC: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x15F4, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_15F4: 0x%x\n", Value));

	read_reg(pAd, 0x41, 0x080c, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_080c: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1700, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1700: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1704, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1704: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x1708, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_1708: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x170C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_170C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0430, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0430: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0434, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0434: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0438, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0438: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x043C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_043C: 0x%x\n", Value));
	RTMP_IO_READ32(pAd, RLT_PBF_CFG, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0404: 0x%x\n", Value));
	/* FCE */
	read_reg(pAd, 0x41, 0x0800, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0800: 0x%x\n", Value));
	if (Value == 0x0)
		DBGPRINT(RT_DEBUG_OFF, ("FCE not enabled\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	/* FW Traffic to Host or not */
	read_reg(pAd, 0x41, 0x0A0C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0A0C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x080c, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_080c: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0810, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0810: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0814, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0814: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0818, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0818: 0x%x\n", Value));

	read_reg(pAd, 0x41, 0x0988, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0988: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x098C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_098C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x0998, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_0998: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x099C, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_099C: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09a8, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09a8: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09ac, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09ac: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09b8, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09b8: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0x09bc, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_09bc: 0x%x\n", Value));
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	/* USB IP CRs */
	read_reg(pAd, 0x40, 0x2244, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2244: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2254, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2254: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2264, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2264: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2274, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2274: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2284, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2284: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x2294, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_2294: 0x%x\n", Value));
	read_reg(pAd, 0x40, 0x80, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x40_80: 0x%x\n", Value));

	read_reg(pAd, 0x41, 0xa10, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a10: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa14, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a14: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa18, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a18: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa1c, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a1c: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa20, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a20: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa24, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a24: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa28, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a28: 0x%x\n", Value));
	read_reg(pAd, 0x41, 0xa2c, &Value);
	DBGPRINT(RT_DEBUG_OFF, (" 0x41_a2c: 0x%x\n", Value));

	/* RTUSBReadMACRegister(pAd, 0x738, &Value); */
	read_reg(pAd, 0x41, 0x738, &Value);
	DBGPRINT(RT_DEBUG_ERROR, ("F/W Heart beat(0x738): 0x%x\n", Value));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	return 0;
}
