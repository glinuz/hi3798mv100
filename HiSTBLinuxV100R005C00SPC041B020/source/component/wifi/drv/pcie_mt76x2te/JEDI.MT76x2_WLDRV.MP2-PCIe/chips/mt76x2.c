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

#ifndef FW_LOAD_FROM_BIN
#include "mcu/mt7662_rom_patch.h"
#include "mcu/mt7662_firmware.h"

#include "mcu/mt7662t_rom_patch.h"
#include "mcu/mt7662t_firmware.h"
#endif

#include "chip/mt76x2_def_prof.h"

#ifdef RTMP_EFUSE_SUPPORT
#include "eeprom/mt76x2_e2p.h"
#endif /* RTMP_EFUSE_SUPPORT */

#define MT7662_EFUSE_CTRL	0x0024
static RTMP_REG_PAIR mt76x2_mac_cr_table[] = {
	{PBF_SYS_CTRL, 0x80c00},
	{RLT_PBF_CFG, 0x1efebcff},
	{FCE_PSE_CTRL, 0x1},
	{MAC_SYS_CTRL, 0x0C},
	{MAX_LEN_CFG, 0x003E3FFF},
	{AMPDU_MAX_LEN_20M1S_MCS0_7, 0xAAA99887},
	{AMPDU_MAX_LEN_20M1S_MCS8_9, 0xAA},
	{XIFS_TIME_CFG, 0x33A40D0A},
	{BKOFF_SLOT_CFG, 0x209},
	{TBTT_SYNC_CFG, 0x422010},
	{PWR_PIN_CFG, 0x0},
	{0x1238, 0x001700C8},
	{TX_SW_CFG0, 0x00101001},
	{TX_SW_CFG1, 0x00010000},
	{TX_SW_CFG2, 0x0},
	{TXOP_CTRL_CFG, 0x0000583F},
	{TX_RTS_CFG, 0x00092B20},
	{TX_TIMEOUT_CFG, 0x000A2290},
	{TX_RTY_CFG, 0x47D01F0F},
	{EXP_ACK_TIME, 0x002C00DC},
	{TX_PROT_CFG6, 0xE3F42004},
	{TX_PROT_CFG7, 0xE3F42084},
	{TX_PROT_CFG8, 0xE3F42104},
	{PIFS_TX_CFG, 0x00060FFF},
	{RX_FILTR_CFG, 0x00015F97},
	{LEGACY_BASIC_RATE, 0x0000017F},
	{HT_BASIC_RATE, 0x00004003},
	{HT_CTRL_CFG, 0x000001FF},
	{0x150C, 0x00000002},	/* Enable TX length > 4095 bytes */
	{TXOP_HLDR_ET, 0x00000000},
	{0xa44, 0x0},
#ifdef HDR_TRANS_SUPPORT
	{HEADER_TRANS_CTRL_REG, 0x2},	/* 0x1: TX, 0x2: RX */
	{TSO_CTRL, 0x7050},
	{0x148, 0x15000001},
#else
	{HEADER_TRANS_CTRL_REG, 0x0},
	{TSO_CTRL, 0x0},
#endif /* HDR_TRANS_SUPPORT */
	{AUX_CLK_CFG, 0x0},
	{DACCLK_EN_DLY_CFG, 0x0},	/* MAC dynamic control TX 960MHZ */
	{TX_ALC_CFG_4, 0x00000000},
	{TX_ALC_VGA3, 0x0},
	{TX_PWR_CFG_0, 0x3A3A3A3A},
	{TX_PWR_CFG_1, 0x3A3A3A3A},
	{TX_PWR_CFG_2, 0x3A3A3A3A},
	{TX_PWR_CFG_3, 0x3A3A3A3A},
	{TX_PWR_CFG_4, 0x3A3A3A3A},
	{TX_PWR_CFG_7, 0x3A3A3A3A},
	{TX_PWR_CFG_8, 0x3A},
	{TX_PWR_CFG_9, 0x3A},
	{MT7662_EFUSE_CTRL, 0xD000},
	{PER_PORT_PAUSE_ENABLE_CONTROL1, 0xA},
	{0x824, 0x60401c18},	/* change the FCE threshold from 0x60402c28 to 0x60401c18 */
#ifdef TXBF_SUPPORT
	{TX_TXBF_CFG_0, 0x4002FC21},	/* Force MCS2 for sounding response */
	{TX_TXBF_CFG_1, 0xFE23727F},
	/* The explicit TxBF feedback is applied only when the value of (local TSF timer) -
	 * (TSF timestamp of the feedback frame) is greater then or equal to 0xFFFFFFFF */
	{TX_TXBF_CFG_2, 0xFFFFFFFF},
#endif /* endif */
	{0x210, 0x94ff0000},
	{0x1478, 0x00000004},
	{0x1384, 0x00001818},
	{0x1358, 0xEDCBA980},
	{PROT_AUTO_TX_CFG, 0x00830083},	/* default use 2 stream to Tx HW auto gened packets */
};
static UCHAR mt76x2_mac_cr_nums = (sizeof(mt76x2_mac_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_g_band_cr_table[] = {
	{BB_PA_MODE_CFG0, 0x010055FF},
	{BB_PA_MODE_CFG1, 0x00550055},
	{RF_PA_MODE_CFG0, 0x010055FF},
	{RF_PA_MODE_CFG1, 0x00550055},
	{TX_ALC_CFG_2, 0x35160A00},
	{TX_ALC_CFG_3, 0x35160A06},
	{TX_ALC_CFG_4, 0x00000606},
};
UCHAR mt76x2_mac_g_band_cr_nums = (sizeof(mt76x2_mac_g_band_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_g_band_internal_pa_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x0F3C3C3C},
	{TX1_RF_GAIN_CORR, 0x0F3C3C3C},
	{TX0_BB_GAIN_ATTEN, 0x00000606},
	{PAMODE_PWR_ADJ0, 0xF4000200},
	{PAMODE_PWR_ADJ1, 0xFA000200},
};
UCHAR mt76x2_mac_g_band_internal_pa_cr_nums =
	(sizeof(mt76x2_mac_g_band_internal_pa_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_g_band_external_pa_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x3C3C023C},
	{TX1_RF_GAIN_CORR, 0x3C3C023C},
	{TX0_BB_GAIN_ATTEN, 0x00001818},
	{PAMODE_PWR_ADJ0, 0x0000EC00},
	{PAMODE_PWR_ADJ1, 0x0000EC00},
};
UCHAR mt76x2_mac_g_band_external_pa_cr_nums =
	(sizeof(mt76x2_mac_g_band_external_pa_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_g_band_external_pa_low_temp_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x3C3C023C},
	{TX1_RF_GAIN_CORR, 0x3C3C023C},
	{TX0_BB_GAIN_ATTEN, 0x00001F1F},
};
UCHAR mt76x2_mac_g_band_external_pa_low_temp_cr_nums =
	(sizeof(mt76x2_mac_g_band_external_pa_low_temp_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_a_band_cr_table[] = {
	{BB_PA_MODE_CFG0, 0x0000FFFF},
	{BB_PA_MODE_CFG1, 0x00FF00FF},
	{RF_PA_MODE_CFG0, 0x0000FFFF},
	{RF_PA_MODE_CFG1, 0x00FF00FF},
	{TX_ALC_CFG_2, 0x1B0F0400},
	{TX_ALC_CFG_3, 0x1B0F0476},
	{TX_ALC_CFG_4, 0x00000000},
};
UCHAR mt76x2_mac_a_band_cr_nums = (sizeof(mt76x2_mac_a_band_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_a_band_internal_pa_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x383C023C},
	{TX1_RF_GAIN_CORR, 0x24282E28},
	{TX0_BB_GAIN_ATTEN, 0x00000000},
	{PAMODE_PWR_ADJ0, 0x00000000},
	{PAMODE_PWR_ADJ1, 0x00000000},
};
UCHAR mt76x2_mac_a_band_internal_pa_cr_nums =
	(sizeof(mt76x2_mac_a_band_internal_pa_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_a_band_external_pa_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x3C3C023C},
	{TX1_RF_GAIN_CORR, 0x3C3C023C},
	{TX0_BB_GAIN_ATTEN, 0x00001818},
	{PAMODE_PWR_ADJ0, 0x04000000},
	{PAMODE_PWR_ADJ1, 0x04000000},
};
UCHAR mt76x2_mac_a_band_external_pa_cr_nums =
	(sizeof(mt76x2_mac_a_band_external_pa_cr_table) / sizeof(RTMP_REG_PAIR));

RTMP_REG_PAIR mt76x2_mac_a_band_external_pa_low_temp_cr_table[] = {
	{TX0_RF_GAIN_CORR, 0x3C3C023C},
	{TX1_RF_GAIN_CORR, 0x3C3C023C},
	{TX0_BB_GAIN_ATTEN, 0x00001F1F},
};
UCHAR mt76x2_mac_a_band_external_pa_low_temp_cr_nums =
	(sizeof(mt76x2_mac_a_band_external_pa_low_temp_cr_table) / sizeof(RTMP_REG_PAIR));

struct RF_INDEX_OFFSET mt76x2_rf_index_offset[] = {
	{0, 0x0000, 0x033c},
	{1, 0x0000, 0x033c},
};

#ifdef CONFIG_CALIBRATION_COLLECTION
void record_calibration_info(RTMP_ADAPTER *pAd, u32 cal_id)
{
	switch (cal_id) {
	case RXDCOC_CALIBRATION_7662:
		/* RF0 214 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_214] = 0x214;
		mt_rf_read(pAd, 0, 0x214,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_214]);
		/* RF0 218 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_218] = 0x218;
		mt_rf_read(pAd, 0, 0x218,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_218]);
		/* RF0 21C */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_21C] = 0x21C;
		mt_rf_read(pAd, 0, 0x21C,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_21C]);
		/* RF0 220 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_220] = 0x220;
		mt_rf_read(pAd, 0, 0x220,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_220]);
		/* RF0 224 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_224] = 0x224;
		mt_rf_read(pAd, 0, 0x224,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_224]);
		/* RF0 228 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_228] = 0x228;
		mt_rf_read(pAd, 0, 0x228,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_228]);
		/* RF1 214 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_214] = 0x214;
		mt_rf_read(pAd, 1, 0x214,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_214]);
		/* RF1 218 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_218] = 0x218;
		mt_rf_read(pAd, 1, 0x218,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_218]);
		/* RF1 21C */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_21C] = 0x21C;
		mt_rf_read(pAd, 1, 0x21C,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_21C]);
		/* RF1 220 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_220] = 0x220;
		mt_rf_read(pAd, 1, 0x220,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_220]);
		/* RF1 224 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_224] = 0x224;
		mt_rf_read(pAd, 1, 0x224,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_224]);
		/* RF1 228 */
		pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_228] = 0x228;
		mt_rf_read(pAd, 1, 0x228,
			   &pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_228]);
		break;
	case RC_CALIBRATION_7662:
		/* RF0 24C */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_24C] = 0x24C;
		mt_rf_read(pAd, 0, 0x24C,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_24C]);
		/* RF0 250 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_250] = 0x250;
		mt_rf_read(pAd, 0, 0x250,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_250]);
		/* RF0 264 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_264] = 0x264;
		mt_rf_read(pAd, 0, 0x264,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_264]);
		/* RF0 278 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_278] = 0x278;
		mt_rf_read(pAd, 0, 0x278,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_278]);
		/* RF1 24C */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_24C] = 0x24C;
		mt_rf_read(pAd, 1, 0x24C,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_24C]);
		/* RF1 250 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_250] = 0x250;
		mt_rf_read(pAd, 1, 0x250,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_250]);
		/* RF1 264 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_264] = 0x264;
		mt_rf_read(pAd, 1, 0x264,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_264]);
		/* RF1 278 */
		pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_278] = 0x278;
		mt_rf_read(pAd, 1, 0x278,
			   &pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_278]);
		break;
	case LC_CALIBRATION_7662:
		/* BBP 2774 */
		pAd->calibration_info[LC_CALIBRATION_7662].addr[BBP_2774] = 0x2774;
		RTMP_BBP_IO_READ32(pAd, 0x2774,
				   &pAd->calibration_info[LC_CALIBRATION_7662].value[BBP_2774]);
		break;
	case TX_LOFT_CALIBRATION_7662:
		/* BBP 2780 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2780] = 0x2780;
		RTMP_BBP_IO_READ32(pAd, 0x2780,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_2780]);
		/* BBP 2784 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2784] = 0x2784;
		RTMP_BBP_IO_READ32(pAd, 0x2784,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_2784]);
		/* BBP 2788 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2788] = 0x2788;
		RTMP_BBP_IO_READ32(pAd, 0x2788,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_2788]);
		/* BBP 278C */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_278C] = 0x278C;
		RTMP_BBP_IO_READ32(pAd, 0x278C,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_278C]);
		/* BBP 2790 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2790] = 0x2790;
		RTMP_BBP_IO_READ32(pAd, 0x2790,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_2790]);
		/* BBP 2794 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2794] = 0x2794;
		RTMP_BBP_IO_READ32(pAd, 0x2794,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_2794]);
		/* BBP 27A8 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27A8] = 0x27A8;
		RTMP_BBP_IO_READ32(pAd, 0x27A8,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27A8]);
		/* BBP 27AC */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27AC] = 0x27AC;
		RTMP_BBP_IO_READ32(pAd, 0x27AC,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27AC]);
		/* BBP 27B0 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B0] = 0x27B0;
		RTMP_BBP_IO_READ32(pAd, 0x27B0,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27B0]);
		/* BBP 27B4 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B4] = 0x27B4;
		RTMP_BBP_IO_READ32(pAd, 0x27B4,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27B4]);
		/* BBP 27B8 */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B8] = 0x27B8;
		RTMP_BBP_IO_READ32(pAd, 0x27B8,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27B8]);
		/* BBP 27BC */
		pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27BC] = 0x27BC;
		RTMP_BBP_IO_READ32(pAd, 0x27BC,
				   &pAd->calibration_info[TX_LOFT_CALIBRATION_7662].
				   value[BBP_27BC]);
		break;
	case TXIQ_CALIBRATION_7662:
		/* BBP 2798 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_2798] = 0x2798;
		RTMP_BBP_IO_READ32(pAd, 0x2798,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_2798]);
		/* BBP 279C */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_279C] = 0x279C;
		RTMP_BBP_IO_READ32(pAd, 0x279C,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_279C]);
		/* BBP 27A0 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27A0] = 0x27A0;
		RTMP_BBP_IO_READ32(pAd, 0x27A0,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A0]);
		/* BBP 27A4 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27A4] = 0x27A4;
		RTMP_BBP_IO_READ32(pAd, 0x27A4,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A4]);
		/* BBP 27C0 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C0] = 0x27C0;
		RTMP_BBP_IO_READ32(pAd, 0x27C0,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C0]);
		/* BBP 27C4 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C4] = 0x27C4;
		RTMP_BBP_IO_READ32(pAd, 0x27C4,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C4]);
		/* BBP 27C8 */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C8] = 0x27C8;
		RTMP_BBP_IO_READ32(pAd, 0x27C8,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C8]);
		/* BBP 27CC */
		pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27CC] = 0x27CC;
		RTMP_BBP_IO_READ32(pAd, 0x27CC,
				   &pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27CC]);
		break;
	case TSSI_CALIBRATION_7662:
		/* BBP 208C */
		pAd->calibration_info[TSSI_CALIBRATION_7662].addr[BBP_208C] = 0x208C;
		RTMP_BBP_IO_READ32(pAd, 0x208C,
				   &pAd->calibration_info[TSSI_CALIBRATION_7662].value[BBP_208C]);
		break;
	case DPD_CALIBRATION_7662:
		/* BBP 2720 */
		pAd->calibration_info[DPD_CALIBRATION_7662].addr[BBP_2720] = 0x2720;
		RTMP_BBP_IO_READ32(pAd, 0x2720,
				   &pAd->calibration_info[DPD_CALIBRATION_7662].value[BBP_2720]);
		break;
	case RXIQC_FI_CALIBRATION_7662:
		/* BBP 2C60 */
		pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C60] = 0x2C60;
		RTMP_BBP_IO_READ32(pAd, 0x2C60,
				   &pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].
				   value[BBP_2C60]);
		/* BBP 2C64 */
		pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C64] = 0x2C64;
		RTMP_BBP_IO_READ32(pAd, 0x2C64,
				   &pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].
				   value[BBP_2C64]);
		/* BBP 2C70 */
		pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C70] = 0x2C70;
		RTMP_BBP_IO_READ32(pAd, 0x2C70,
				   &pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].
				   value[BBP_2C70]);
		/* BBP 2C74 */
		pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C74] = 0x2C74;
		RTMP_BBP_IO_READ32(pAd, 0x2C74,
				   &pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].
				   value[BBP_2C74]);
		break;
	case RXIQC_FD_CALIBRATION_7662:
		/* BBP 2818 */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2818] = 0x2818;
		RTMP_BBP_IO_READ32(pAd, 0x2818,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_2818]);
		/* BBP 281C */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_281C] = 0x281C;
		RTMP_BBP_IO_READ32(pAd, 0x281C,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_281C]);
		/* BBP 2820 */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2820] = 0x2820;
		RTMP_BBP_IO_READ32(pAd, 0x2820,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_2820]);
		/* BBP 2824 */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2824] = 0x2824;
		RTMP_BBP_IO_READ32(pAd, 0x2824,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_2824]);
		/* BBP 2828 */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2828] = 0x2828;
		RTMP_BBP_IO_READ32(pAd, 0x2828,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_2828]);
		/* BBP 282C */
		pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_282C] = 0x282C;
		RTMP_BBP_IO_READ32(pAd, 0x282C,
				   &pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].
				   value[BBP_282C]);
		break;
	default:
		break;
	}
}

static void dump_calibration_info_for_robust(RTMP_ADAPTER *pAd, UINT32 cal_id)
{
	switch (cal_id) {
	case RXDCOC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_214],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_218],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_21C],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_220],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_224],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_228],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_214],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_218],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_21C],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_220],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_224],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_228]));
		break;
	case RC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_24C],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_250],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_264],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_278],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_24C],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_250],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_264],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_278]));
		break;
	case LC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x\n", pAd->calibration_info[LC_CALIBRATION_7662].value[BBP_2774]));

		break;
	case TX_LOFT_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2780],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2784],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2788],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_278C],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2790],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2794],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27A8],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27AC],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B0],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B4],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B8],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27BC]));
		break;
	case TXIQ_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_2798],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_279C],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A0],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A4],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C0],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C4],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C8],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27CC]));
		break;
	case TSSI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x\n",
			  pAd->calibration_info[TSSI_CALIBRATION_7662].value[BBP_208C]));

		break;
	case DPD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x\n", pAd->calibration_info[DPD_CALIBRATION_7662].value[BBP_2720]));
		break;
	case RXIQC_FI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C60],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C64],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C70],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C74]));
		break;
	case RXIQC_FD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF,
			 ("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2818],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_281C],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2820],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2824],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2828],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_282C]));
		break;
	default:
		DBGPRINT(RT_DEBUG_OFF, ("unknbow cal id = %d\n", cal_id));
		break;
	}

}

static void do_calibrtion_info_for_robust(RTMP_ADAPTER *ad, UINT32 cal_id, UINT8 channel)
{
	UINT32 i;

	switch (cal_id) {
	case RXDCOC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nRXDCOC_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF,
			 ("RF0_214 RF0_218 RF0_21C RF0_220 RF0_224 RF0_228 RF1_214 RF1_218 RF1_21C RF1_220 RF1_224 RF1_228\n"));
		for (i = 0; i < 100; i++) {
			CHIP_CALIBRATION(ad, RXDCOC_CALIBRATION_7662, channel);
			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}

		break;
	case RC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nRC_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF,
			 ("RF0_24C RF0_250 RF0_264 RF0_278 RF1_24C RF1_250 RF1_264 RF1_278\n"));
		for (i = 0; i < 100; i++) {
			CHIP_CALIBRATION(ad, RC_CALIBRATION_7662, 0x00);
			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case LC_CALIBRATION_7662:
		if (channel > 14) {
			DBGPRINT(RT_DEBUG_OFF, ("\nLC_Calibration\n"));
			DBGPRINT(RT_DEBUG_OFF, ("BBP2774\n"));
			for (i = 0; i < 100; i++) {
				CHIP_CALIBRATION(ad, LC_CALIBRATION_7662, 0x00);
				record_calibration_info(ad, cal_id);
				dump_calibration_info_for_robust(ad, cal_id);
			}
		}
		break;
	case TX_LOFT_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nTXLOFT_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF,
			 ("BBP2780 BBP2784 BBP2788 BBP278C BBP2790 BBP2794 BBP27A8 BBP27AC BBP27B0 BBP27B4 BBP27B8 BBP27BC\n"));

		for (i = 0; i < 100; i++) {
			if (channel > 14)
				CHIP_CALIBRATION(ad, TX_LOFT_CALIBRATION_7662, 0x1);
			else
				CHIP_CALIBRATION(ad, TX_LOFT_CALIBRATION_7662, 0x0);

			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case TXIQ_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nTXIQ_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF,
			 ("BBP2798 BBP279C BBP27A0 BBP27A4 BBP27C0 BBP27C4 BBP27C8 BBP27CC\n"));
		for (i = 0; i < 100; i++) {
			if (channel > 14)
				CHIP_CALIBRATION(ad, TXIQ_CALIBRATION_7662, 0x1);
			else
				CHIP_CALIBRATION(ad, TXIQ_CALIBRATION_7662, 0x0);

			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case TSSI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nTSSI_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF, ("BBP208C\n"));
		for (i = 0; i < 100; i++) {
			mt76x2_tssi_calibration(ad, channel);
			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case DPD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nDPD_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF, ("BBP2720\n"));
		for (i = 0; i < 100; i++) {
			CHIP_CALIBRATION(ad, DPD_CALIBRATION_7662, channel);
			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case RXIQC_FI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nRXIQC_FI_Calibration\n"));
		DBGPRINT(RT_DEBUG_OFF, ("BBP2C60 BBP2C64 BBP2C70 BBP2C74\n"));
		for (i = 0; i < 100; i++) {
			if (channel > 14)
				CHIP_CALIBRATION(ad, RXIQC_FI_CALIBRATION_7662, 0x1);
			else
				CHIP_CALIBRATION(ad, RXIQC_FI_CALIBRATION_7662, 0x0);

			record_calibration_info(ad, cal_id);
			dump_calibration_info_for_robust(ad, cal_id);
		}
		break;
	case RXIQC_FD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_OFF, ("\nnot supoort RXIQC_FD_CALIBRATION_7662 yet\n"));
		break;
	default:
		DBGPRINT(RT_DEBUG_OFF, ("\nunknbow cal id = %d\n", cal_id));
		break;
	}
}

void dump_calibration_info(RTMP_ADAPTER *pAd, u32 cal_id)
{
	switch (cal_id) {
	case RXDCOC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nRXDCOC_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_214],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_214]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_218],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_218]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_21C],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_21C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_220],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_220]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_224],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_224]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF0_228],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF0_228]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_214],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_214]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_218],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_218]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_21C],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_21C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_220],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_220]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_224],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_224]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].addr[RF1_228],
			  pAd->calibration_info[RXDCOC_CALIBRATION_7662].value[RF1_228]));
		break;
	case RC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nRC_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_24C],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_24C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_250],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_250]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_264],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_264]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF0[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF0_278],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF0_278]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_24C],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_24C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_250],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_250]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_264],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_264]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("RF1[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RC_CALIBRATION_7662].addr[RF1_278],
			  pAd->calibration_info[RC_CALIBRATION_7662].value[RF1_278]));
		break;
	case LC_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nLC_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[LC_CALIBRATION_7662].addr[BBP_2774],
			  pAd->calibration_info[LC_CALIBRATION_7662].value[BBP_2774]));
		break;
	case TX_LOFT_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nTX_LOFT_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2780],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2780]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2784],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2784]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2788],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2788]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_278C],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_278C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2790],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2790]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_2794],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_2794]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27A8],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27A8]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27AC],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27AC]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B0],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B0]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B4],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B4]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27B8],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27B8]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].addr[BBP_27BC],
			  pAd->calibration_info[TX_LOFT_CALIBRATION_7662].value[BBP_27BC]));
		break;
	case TXIQ_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nTXIQ_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_2798],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_2798]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_279C],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_279C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27A0],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A0]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27A4],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27A4]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C0],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C0]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C4],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C4]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27C8],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27C8]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].addr[BBP_27CC],
			  pAd->calibration_info[TXIQ_CALIBRATION_7662].value[BBP_27CC]));
		break;
	case TSSI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nTSSI_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[TSSI_CALIBRATION_7662].addr[BBP_208C],
			  pAd->calibration_info[TSSI_CALIBRATION_7662].value[BBP_208C]));
		break;
	case DPD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nDPD_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[DPD_CALIBRATION_7662].addr[BBP_2720],
			  pAd->calibration_info[DPD_CALIBRATION_7662].value[BBP_2720]));
		break;
	case RXIQC_FI_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nRXIQC_FI_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C60],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C60]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C64],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C64]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C70],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C70]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].addr[BBP_2C74],
			  pAd->calibration_info[RXIQC_FI_CALIBRATION_7662].value[BBP_2C74]));
		break;
	case RXIQC_FD_CALIBRATION_7662:
		DBGPRINT(RT_DEBUG_ERROR, ("\nRXIQC_FD_CALIBRATION_7662\n"));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2818],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2818]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_281C],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_281C]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2820],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2820]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2824],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2824]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_2828],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_2828]));
		DBGPRINT(RT_DEBUG_ERROR,
			 ("BBP[0x%04x]=0x%08x\n",
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].addr[BBP_282C],
			  pAd->calibration_info[RXIQC_FD_CALIBRATION_7662].value[BBP_282C]));
		break;
	default:
		break;
	}
}
#endif /* endif */

static VOID mt76x2_bbp_adjust(RTMP_ADAPTER *pAd)
{
	static const char * const ext_str[] = { "extNone", "extAbove", "", "extBelow" };
	UCHAR rf_bw, ext_ch;
#ifdef CONFIG_MULTI_CHANNEL
#if defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT)
	PMULTISSID_STRUCT pMbss = &pAd->ApCfg.MBSSID[MAIN_MBSSID];
	struct wifi_dev *wdev = &pMbss->wdev;
#endif /* defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT) */
#endif /* CONFIG_MULTI_CHANNEL */

	if (get_ht_cent_ch(pAd, &rf_bw, &ext_ch) == FALSE) {
		rf_bw = BW_20;
		ext_ch = EXTCHA_NONE;
		pAd->CommonCfg.CentralChannel = pAd->CommonCfg.Channel;
	}
#ifdef DOT11_VHT_AC
	if (WMODE_CAP(pAd->CommonCfg.PhyMode, WMODE_AC) &&
	    (pAd->CommonCfg.Channel > 14) &&
	    (rf_bw == BW_40) &&
	    (pAd->CommonCfg.vht_bw == VHT_BW_80)) {
		rf_bw = BW_80;
		if (pAd->CommonCfg.vht_cent_ch != pAd->CommonCfg.CentralChannel)
			pAd->CommonCfg.vht_cent_ch = vht_cent_ch_freq(pAd, pAd->CommonCfg.Channel);
	}

	DBGPRINT(RT_DEBUG_OFF,
		 ("%s():rf_bw=%d, ext_ch=%d, PrimCh=%d, HT-CentCh=%d, VHT-CentCh=%d\n",
		  __func__, rf_bw, ext_ch, pAd->CommonCfg.Channel,
		  pAd->CommonCfg.CentralChannel, pAd->CommonCfg.vht_cent_ch));
#endif /* DOT11_VHT_AC */

#ifdef CONFIG_MULTI_CHANNEL
#if defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT)
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd) && (wdev->bw == 1))
		bbp_set_bw(pAd, wdev->bw);
	else
#endif /* defined(RT_CFG80211_SUPPORT) && defined(CONFIG_AP_SUPPORT) */
#endif /* CONFIG_MULTI_CHANNEL */
		bbp_set_bw(pAd, rf_bw);

	/* TX/Rx : control channel setting */
	rtmp_mac_set_ctrlch(pAd, ext_ch);
	bbp_set_ctrlch(pAd, ext_ch);

	DBGPRINT(RT_DEBUG_TRACE, ("%s() : %s, ChannelWidth=%d, Channel=%d, ExtChanOffset=%d(%d)\n",
				  __func__, ext_str[ext_ch],
				  pAd->CommonCfg.HtCapability.HtCapInfo.ChannelWidth,
				  pAd->CommonCfg.Channel,
				  pAd->CommonCfg.RegTransmitSetting.field.EXTCHA,
				  pAd->CommonCfg.AddHTInfo.AddHtInfo.ExtChanOffset));
}

UCHAR get_chl_grp(u8 channel)
{
	UCHAR chl_grp = A_BAND_GRP0_CHL;

	if (channel >= 184 && channel <= 196)
		chl_grp = A_BAND_GRP0_CHL;
	else if (channel >= 36 && channel <= 48)
		chl_grp = A_BAND_GRP1_CHL;
	else if (channel >= 52 && channel <= 64)
		chl_grp = A_BAND_GRP2_CHL;
	else if (channel >= 98 && channel <= 114)
		chl_grp = A_BAND_GRP3_CHL;
	else if (channel >= 116 && channel <= 144)
		chl_grp = A_BAND_GRP4_CHL;
	else if (channel >= 149 && channel <= 165)
		chl_grp = A_BAND_GRP5_CHL;
	else
		DBGPRINT(RT_DEBUG_INFO, ("%s:illegal channel (%d)\n", __func__, channel));

	return chl_grp;
}

UCHAR get_low_mid_hi_index(u8 channel)
{
	UCHAR index = G_BAND_LOW;

	if (channel <= 14) {
		if (channel >= 1 && channel <= 5)
			index = G_BAND_LOW;
		else if (channel >= 6 && channel <= 10)
			index = G_BAND_MID;
		else if (channel >= 11 && channel <= 14)
			index = G_BAND_HI;
		else
			DBGPRINT(RT_DEBUG_INFO, ("%s:illegal channel(%d)\n", __func__, channel));
	} else {
		if (channel >= 184 && channel <= 188)
			index = A_BAND_LOW;
		else if (channel >= 192 && channel <= 196)
			index = A_BAND_HI;
		else if (channel >= 36 && channel <= 42)
			index = A_BAND_LOW;
		else if (channel >= 44 && channel <= 48)
			index = A_BAND_HI;
		else if (channel >= 52 && channel <= 56)
			index = A_BAND_LOW;
		else if (channel >= 58 && channel <= 64)
			index = A_BAND_HI;
		else if (channel >= 98 && channel <= 104)
			index = A_BAND_LOW;
		else if (channel >= 106 && channel <= 114)
			index = A_BAND_HI;
		else if (channel >= 116 && channel <= 128)
			index = A_BAND_LOW;
		else if (channel >= 130 && channel <= 144)
			index = A_BAND_HI;
		else if (channel >= 149 && channel <= 156)
			index = A_BAND_LOW;
		else if (channel >= 157 && channel <= 165)
			index = A_BAND_HI;
		else
			DBGPRINT(RT_DEBUG_INFO, ("%s:illegal channel(%d)\n", __func__, channel));
	}

	return index;
}

void mt76x2_adjust_per_rate_pwr_delta(RTMP_ADAPTER *ad, u8 channel, char delta_pwr)
{
	u32 value;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	unsigned int band;

	if (channel > 14)
		band = _A_BAND;
	else
		band = _G_BAND;

	RTMP_IO_READ32(ad, TX_PWR_CFG_0, &value);
	value &= ~TX_PWR_CCK_1_2_MASK;

	value |= TX_PWR_CCK_1_2(cap->tx_pwr_cck_1_2 + delta_pwr);

	value &= ~TX_PWR_CCK_5_11_MASK;
	value |= TX_PWR_CCK_5_11(cap->tx_pwr_cck_5_11 + delta_pwr);

	value &= ~TX_PWR_OFDM_6_9_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_OFDM_6_9(cap->tx_pwr_g_band_ofdm_6_9 + delta_pwr);
	else
		value |= TX_PWR_OFDM_6_9(cap->tx_pwr_a_band_ofdm_6_9 + delta_pwr);

	value &= ~TX_PWR_OFDM_12_18_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_OFDM_12_18(cap->tx_pwr_g_band_ofdm_12_18 + delta_pwr);
	else
		value |= TX_PWR_OFDM_12_18(cap->tx_pwr_a_band_ofdm_12_18 + delta_pwr);
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_0, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_1, &value);
	value &= ~TX_PWR_OFDM_24_36_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_OFDM_24_36(cap->tx_pwr_g_band_ofdm_24_36 + delta_pwr);
	else
		value |= TX_PWR_OFDM_24_36(cap->tx_pwr_a_band_ofdm_24_36 + delta_pwr);

	value &= ~TX_PWR_OFDM_48_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_OFDM_48(cap->tx_pwr_g_band_ofdm_48_54 + delta_pwr);
	else
		value |= TX_PWR_OFDM_48(cap->tx_pwr_a_band_ofdm_48_54 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_1SS_MCS_0_1_MASK;
	value |= TX_PWR_HT_VHT_1SS_MCS_0_1(cap->tx_pwr_ht_mcs_0_1 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_1SS_MCS_2_3_MASK;
	value |= TX_PWR_HT_VHT_1SS_MCS_2_3(cap->tx_pwr_ht_mcs_2_3 + delta_pwr);
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_1, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_2, &value);
	value &= ~TX_PWR_HT_VHT_1SS_MCS_4_5_MASK;
	value |= TX_PWR_HT_VHT_1SS_MCS_4_5(cap->tx_pwr_ht_mcs_4_5 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_1SS_MCS_6_MASK;
	value |= TX_PWR_HT_VHT_1SS_MCS_6(cap->tx_pwr_ht_mcs_6_7 + delta_pwr);

	value &= ~TX_PWR_HT_MCS_8_9_VHT_2SS_0_1_MASK;
	value |= TX_PWR_HT_MCS_8_9_VHT_2SS_0_1(cap->tx_pwr_ht_mcs_8_9 + delta_pwr);

	value &= ~TX_PWR_HT_MCS_10_11_VHT_2SS_MCS_2_3_MASK;
	value |= TX_PWR_HT_MCS_10_11_VHT_2SS_MCS_2_3(cap->tx_pwr_ht_mcs_10_11 + delta_pwr);
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_2, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_3, &value);
	value &= ~TX_PWR_HT_MCS_12_13_VHT_2SS_MCS_4_5_MASK;
	value |= TX_PWR_HT_MCS_12_13_VHT_2SS_MCS_4_5(cap->tx_pwr_ht_mcs_12_13 + delta_pwr);

	value &= ~TX_PWR_HT_MCS_14_VHT_2SS_MCS_6_MASK;
	value |= TX_PWR_HT_MCS_14_VHT_2SS_MCS_6(cap->tx_pwr_ht_mcs_14_15 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_STBC_MCS_0_1_MASK;
	value |= TX_PWR_HT_VHT_STBC_MCS_0_1(cap->tx_pwr_ht_mcs_0_1 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_STBC_MCS_2_3_MASK;
	value |= TX_PWR_HT_VHT_STBC_MCS_2_3(cap->tx_pwr_ht_mcs_2_3 + delta_pwr);
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_3, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_4, &value);
	value &= ~TX_PWR_HT_VHT_STBC_MCS_4_5_MASK;
	value |= TX_PWR_HT_VHT_STBC_MCS_4_5(cap->tx_pwr_ht_mcs_4_5 + delta_pwr);

	value &= ~TX_PWR_HT_VHT_STBC_MCS_6_MASK;
	value |= TX_PWR_HT_VHT_STBC_MCS_6(cap->tx_pwr_ht_mcs_6_7 + delta_pwr);
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_4, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_7, &value);
	value &= ~TX_PWR_OFDM_54_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_OFDM_54(cap->tx_pwr_g_band_ofdm_48_54 + delta_pwr);
	else
		value |= TX_PWR_OFDM_54(cap->tx_pwr_a_band_ofdm_48_54 + delta_pwr);

	value &= ~TX_PWR_VHT_2SS_MCS_8_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_2SS_MCS_8(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_2SS_MCS_8(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	value &= ~TX_PWR_HT_MCS_7_VHT_1SS_MCS_7_MASK;
	value |= TX_PWR_HT_MCS_7_VHT_1SS_MCS_7(cap->tx_pwr_ht_mcs_6_7 + delta_pwr);

	value &= ~TX_PWR_VHT_2SS_MCS_9_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_2SS_MCS_9(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_2SS_MCS_9(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_7, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_8, &value);
	value &= ~TX_PWR_HT_MCS_15_VHT_2SS_MCS7_MASK;
	value |= TX_PWR_HT_MCS_15_VHT_2SS_MCS7(cap->tx_pwr_ht_mcs_14_15 + delta_pwr);

	value &= ~TX_PWR_VHT_1SS_MCS_8_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_1SS_MCS_8(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_1SS_MCS_8(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	value &= ~TX_PWR_VHT_1SS_MCS_9_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_1SS_MCS_9(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_1SS_MCS_9(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_8, value);

	RTMP_IO_READ32(ad, TX_PWR_CFG_9, &value);
	value &= ~TX_PWR_HT_VHT_STBC_MCS_7_MASK;
	value |= TX_PWR_HT_VHT_STBC_MCS_7(cap->tx_pwr_ht_mcs_6_7 + delta_pwr);

	value &= ~TX_PWR_VHT_STBC_MCS_8_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_STBC_MCS_8(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_STBC_MCS_8(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	value &= ~TX_PWR_VHT_STBC_MCS_9_MASK;
	if (band == _G_BAND)
		value |= TX_PWR_VHT_STBC_MCS_9(cap->tx_pwr_2g_vht_mcs_8_9 + delta_pwr);
	else
		value |= TX_PWR_VHT_STBC_MCS_9(cap->tx_pwr_5g_vht_mcs_8_9 + delta_pwr);

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_9, value);
}

void mt76x2_tx_pwr_gain(RTMP_ADAPTER *ad, u8 channel, u8 bw)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	CHAR tx_0_pwr, tx_1_pwr;
	UINT32 value;

	/* set 54Mbps target power */
	if (channel <= 14) {
		tx_0_pwr = cap->tx_0_target_pwr_g_band;
		tx_0_pwr += cap->tx_0_chl_pwr_delta_g_band[get_low_mid_hi_index(channel)];
		if (bw == BW_40)
			tx_0_pwr += cap->delta_tx_pwr_bw40_g_band;

		tx_1_pwr = cap->tx_1_target_pwr_g_band;
		tx_1_pwr += cap->tx_1_chl_pwr_delta_g_band[get_low_mid_hi_index(channel)];
		if (bw == BW_40)
			tx_1_pwr += cap->delta_tx_pwr_bw40_g_band;

	} else {
		tx_0_pwr = cap->tx_0_target_pwr_a_band[get_chl_grp(channel)];
		tx_0_pwr +=
		    cap->tx_0_chl_pwr_delta_a_band[get_chl_grp(channel)][get_low_mid_hi_index
									 (channel)];

		if (bw == BW_40)
			tx_0_pwr += cap->delta_tx_pwr_bw40_a_band;
		else if (bw == BW_80)
			tx_0_pwr += cap->delta_tx_pwr_bw80;

		tx_1_pwr = cap->tx_1_target_pwr_a_band[get_chl_grp(channel)];
		tx_1_pwr +=
		    cap->tx_1_chl_pwr_delta_a_band[get_chl_grp(channel)][get_low_mid_hi_index
									 (channel)];

		if (bw == BW_40)
			tx_1_pwr += cap->delta_tx_pwr_bw40_a_band;
		else if (bw == BW_80)
			tx_1_pwr += cap->delta_tx_pwr_bw80;
	}

	/* range 0~23.5 db */
	if (tx_0_pwr >= 0x2f)
		tx_0_pwr = 0x2f;

	if (tx_0_pwr < 0)
		tx_0_pwr = 0;

	if (tx_1_pwr >= 0x2f)
		tx_1_pwr = 0x2f;

	if (tx_1_pwr < 0)
		tx_1_pwr = 0;

	/* TX0 channel initial transmission gain setting */
	RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
	value = value & (~TX_ALC_CFG_0_CH_INT_0_MASK);
	value |= TX_ALC_CFG_0_CH_INT_0(tx_0_pwr);
	/* value |= TX_ALC_CFG_0_CH_INT_0(0x7); */
	DBGPRINT(RT_DEBUG_INFO, ("tx_0_pwr = %d\n", tx_0_pwr));
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_0, value);

	/* TX1 channel initial transmission gain setting */
	RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
	value = value & (~TX_ALC_CFG_0_CH_INT_1_MASK);
	value |= TX_ALC_CFG_0_CH_INT_1(tx_1_pwr);
	/* value |= TX_ALC_CFG_0_CH_INT_1(0x7); */
	DBGPRINT(RT_DEBUG_INFO, ("tx_1_pwr = %d\n", tx_1_pwr));
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_0, value);
}

#define TEST_BIT(value, BIT) (((value)&(BIT)) == (BIT))

#ifdef RTMP_USB_SUPPORT
/* Polling flow: Recommended by DE team:
   1. Check UDMA Tx EP4~EP9 is empty  (0x2240 ~ 0x2290, check BIT17==1)
   2. Wait UDMA Tx state idle (0x9100, check 0x7f00000==0)
   3. Check FCE Tx empty (0x0A30, check 0xFF==0)
   4. check FCE Tx2 empty (0x0A34, check 0xFF00==0)
 */
INT32 mt76x2_polling_txq_empty(RTMP_ADAPTER *ad)
{
#define WAIT_RETRY_COUNT      (5)
#define WAIT_TIME             (4)

	UINT32 Value = 0;
	UINT32 count = 0;
	UINT8 u1EP4, u1EP5, u1EP6, u1EP7, u1EP8, u1EP9;
	UINT32 u4UdmaTxSM, u4FceTx1, u4FceTx2;

	/* ------------------------------------------------------------
	   1.Check UDMA Tx EP4~EP9 is empty  (0x2240 ~ 0x2290, check BIT17==1)
	   --------------------------------------------------------------- */
	read_reg(ad, 0x40, 0x2240, &Value);
	u1EP4 = TEST_BIT(Value, BIT17);
	read_reg(ad, 0x40, 0x2250, &Value);
	u1EP5 = TEST_BIT(Value, BIT17);
	read_reg(ad, 0x40, 0x2260, &Value);
	u1EP6 = TEST_BIT(Value, BIT17);
	read_reg(ad, 0x40, 0x2270, &Value);
	u1EP7 = TEST_BIT(Value, BIT17);
	read_reg(ad, 0x40, 0x2280, &Value);
	u1EP8 = TEST_BIT(Value, BIT17);
	read_reg(ad, 0x40, 0x2290, &Value);
	u1EP9 = TEST_BIT(Value, BIT17);

	/* ------------------------------------------------------------
	   2.Wait UDMA Tx state idle (0x9100, check 0x7f00000==0)
	   --------------------------------------------------------------- */
	read_reg(ad, 0x40, 0x9100, &Value);
	u4UdmaTxSM = (Value & 0x07F00000);

	/* ------------------------------------------------------------
	   3.Check FCE Tx1 empty (0x0A30, check 0xFF==0)
	   --------------------------------------------------------------- */
	RTUSBReadMACRegister(ad, 0x0A30, &Value);
	u4FceTx1 = (Value & 0x000000FF);

	/* ------------------------------------------------------------
	   4.Check FCE Tx2 empty (0x0A34, check 0xFF00==0)
	   --------------------------------------------------------------- */
	RTUSBReadMACRegister(ad, 0x0A34, &Value);
	u4FceTx2 = (Value & 0x0000FF00);

	count = 0;
	while (count < WAIT_RETRY_COUNT) {
		if (u1EP4 && u1EP5 && u1EP6 && u1EP7 && u1EP8 && u1EP9 &&
			(u4UdmaTxSM == 0) && (u4FceTx1 == 0) && (u4FceTx2 == 0)) {
			DBGPRINT(RT_DEBUG_INFO, ("%s: %d ms for waiting Tx empty\n",
				__func__, count*WAIT_TIME));
			break;
		}

		if (!u1EP4) {
			read_reg(ad, 0x40, 0x2240, &Value);
			u1EP4 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP4=%d\n",
				__func__, count*WAIT_TIME, u1EP4));
		}
		if (!u1EP5) {
			read_reg(ad, 0x40, 0x2250, &Value);
			u1EP5 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP5=%d\n",
				__func__, count*WAIT_TIME, u1EP5));
		}
		if (!u1EP6) {
			read_reg(ad, 0x40, 0x2260, &Value);
			u1EP6 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP6=%d\n",
				__func__, count*WAIT_TIME, u1EP6));
		}
		if (!u1EP7) {
			read_reg(ad, 0x40, 0x2270, &Value);
			u1EP7 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP7=%d\n",
				__func__, count*WAIT_TIME, u1EP7));
		}
		if (!u1EP8) {
			read_reg(ad, 0x40, 0x2280, &Value);
			u1EP8 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP8=%d\n",
				__func__, count*WAIT_TIME, u1EP8));
		}
		if (!u1EP9) {
			read_reg(ad, 0x40, 0x2290, &Value);
			u1EP9 = TEST_BIT(Value, BIT17);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting EP9=%d\n",
				__func__, count*WAIT_TIME, u1EP9));
		}
		if (u4UdmaTxSM != 0) {
			read_reg(ad, 0x40, 0x9100, &Value);
			u4UdmaTxSM = (Value & 0x07F00000);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting UdmaTxSM=0x%x\n",
				__func__, count*WAIT_TIME, u4UdmaTxSM));
		}
		if (u4FceTx1 != 0) {
			RTUSBReadMACRegister(ad, 0x0A30, &Value);
			u4FceTx1 = (Value & 0x000000FF);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting u4FceTx1=0x%x\n",
				__func__, count*WAIT_TIME, u4FceTx1));
		}
		if (u4FceTx2 != 0) {
			RTUSBReadMACRegister(ad, 0x0A34, &Value);
			u4FceTx2 = (Value & 0x0000FF00);
			DBGPRINT(RT_DEBUG_TRACE, ("%s: %d ms for waiting u4FceTx2=0x%x\n",
				__func__, count*WAIT_TIME, u4FceTx2));
		}

		count++;
		msleep(WAIT_TIME);
	}

	if (count == WAIT_RETRY_COUNT) {
		if (!u1EP4 || !u1EP5 || !u1EP6 || !u1EP7 || !u1EP8 || !u1EP9 ||
			(u4UdmaTxSM != 0) || (u4FceTx1 != 0) || (u4FceTx2 != 0)) {
			DBGPRINT(RT_DEBUG_TRACE,
				("%s: TX Not Empty!!! (seems not a problem though)\n", __func__));
			DBGPRINT(RT_DEBUG_TRACE,
				("u1EP4=%d, u1EP5=%d, u1EP6=%d, u1EP7=%d, u1EP8=%d, u1EP9=%d\n",
				u1EP4, u1EP5, u1EP6, u1EP7, u1EP8, u1EP9));
			DBGPRINT(RT_DEBUG_TRACE,
				("u4UdmaTxSM=0x%x, u4FceTx1=0x%x, u4FceTx2=0x%x\n",
				u4UdmaTxSM, u4FceTx1, u4FceTx2));

			return STATUS_UNSUCCESSFUL;
		}
	}

	return STATUS_SUCCESS;
}

void mt76x2_dump_tx_buffer_cr(RTMP_ADAPTER *ad)
{
	UINT32 Value = 0;
	UINT8 u1EP4, u1EP5, u1EP6, u1EP7, u1EP8, u1EP9;

	DBGPRINT(RT_DEBUG_OFF, ("dump tx buffer cr:--------begain-------------\n"));

	/* ------------------------------------------------------------
	   1.Check UDMA Tx EP4~EP9 is empty  (0x2240 ~ 0x2290, check BIT17==1, empty(1))
	   --------------------------------------------------------------- */
	/* Check if UDMA EP4 Tx is Empty  */
	read_reg(ad, 0x40, 0x2240, &Value);
	u1EP4 = TEST_BIT(Value, BIT17);

	/* Check if UDMA EP5 Tx is Empty  */
	read_reg(ad, 0x40, 0x2250, &Value);
	u1EP5 = TEST_BIT(Value, BIT17);

	/* Check if UDMA EP6 Tx is Empty  */
	read_reg(ad, 0x40, 0x2260, &Value);
	u1EP6 = TEST_BIT(Value, BIT17);

	/* Check if UDMA EP7 Tx is Empty  */
	read_reg(ad, 0x40, 0x2270, &Value);
	u1EP7 = TEST_BIT(Value, BIT17);

	/* Check if UDMA EP8 Tx is Empty  */
	read_reg(ad, 0x40, 0x2280, &Value);
	u1EP8 = TEST_BIT(Value, BIT17);

	/* Check if UDMA EP9 Tx is Empty */
	read_reg(ad, 0x40, 0x2290, &Value);
	u1EP9 = TEST_BIT(Value, BIT17);

	DBGPRINT(RT_DEBUG_OFF,
		("UDMA Tx EP4~EP9:u1EP4=%d, u1EP5=%d, u1EP6=%d, u1EP7=%d, u1EP8=%d, u1EP9=%d\n",
		u1EP4, u1EP5, u1EP6, u1EP7, u1EP8, u1EP9));

	/* ------------------------------------------------------------
	   2.Check if UDMA Tx state idle (0x9100, check 0x7f00000==0)
	   --------------------------------------------------------------- */
	/* Check if UDMA Tx State Machine idle      */
	read_reg(ad, 0x40, 0x9100, &Value);
	DBGPRINT(RT_DEBUG_OFF,
		("UDMA Tx State Machine(0x7f00000==0): Value=0x%x, (Value&0x7F00000)=0x%x\n",
		Value, (Value&0x7F00000)));

	/* ------------------------------------------------------------
	   3.Check FCE Tx1 empty (0x0A30, check 0xFF==0)
	   --------------------------------------------------------------- */
	/* Check if FCE Tx1 Empty */
	RTUSBReadMACRegister(ad, 0x0A30, &Value);
	DBGPRINT(RT_DEBUG_OFF,
		("FCE Tx1(0xFF==0): Value=0x%x, (Value & 0xFF)=0x%x\n",
		Value, (Value & 0xFF)));

	/* ------------------------------------------------------------
	   4.Check FCE Tx2 empty (0x0A34, check 0xFF00==0)
	   --------------------------------------------------------------- */
	/* Check FCE Tx2 Empty */
	RTUSBReadMACRegister(ad, 0x0A34, &Value);
	DBGPRINT(RT_DEBUG_OFF,
		("FCE Tx2(0xFF00==0): Value=0x%x, (Value & 0xFF00)=0x%x\n",
		Value, (Value & 0xFF00)));

	DBGPRINT(RT_DEBUG_OFF, ("dump tx buffer cr:--------end-------------\n"));
}
#endif

#define EXT_CH_NONE  0x00
#define EXT_CH_ABOVE 0X01
#define EXT_CH_BELOW 0x03

static void mt76x2_switch_channel(RTMP_ADAPTER *ad, u8 channel, BOOLEAN scan)
{
#ifdef ED_MONITOR
	RTMP_CHIP_CAP *cap = &ad->chipCap;
#endif /* ED_MONITOR */
	unsigned int latch_band, band, bw, tx_rx_setting;
	UINT32 ret, value, value1, restore_value, loop = 0;
	UINT16 e2p_value;
	UCHAR bbp_ch_idx;
	BOOLEAN band_change = FALSE;
	UINT32 RegValue = 0;
	UINT32 eLNA_gain_from_e2p = 0;
#ifdef ED_MONITOR
	UINT32 ed_th_backup = 0;
#endif /* ED_MONITOR */

#if defined(RT_CFG80211_SUPPORT) && defined(RT_CFG80211_P2P_SUPPORT)
	BCN_TIME_CFG_STRUC csr;
	INT32 BeaconEnabled = 0;
#endif /* endif */

#ifdef ED_MONITOR
	/* Backup ED threshold */
	if (ad->ed_chk && (cap->fw_non_support & fNONSUPPORT_EDCCA_UPDATE))
		RTMP_BBP_IO_READ32(ad, AGC1_R2, &ed_th_backup);
#endif /* ED_MONITOR */

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(ad)) {
		UINT32 ret;

		RTMP_SEM_EVENT_WAIT(&ad->hw_atomic, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return;
		}
	}
#endif /* RTMP_USB_SUPPORT */

#if defined(RT_CFG80211_SUPPORT) && defined(RT_CFG80211_P2P_SUPPORT)
	if (RTMP_CFG80211_VIF_P2P_GO_ON(ad)) {
		RTMP_IO_READ32(ad, BCN_TIME_CFG, &csr.word);
		if (csr.field.bBeaconGen) {
			/* disable Beacon before channel switching */
			BeaconEnabled = 1;
			csr.field.bBeaconGen = 0;
			RTMP_IO_WRITE32(ad, BCN_TIME_CFG, csr.word);
		}
	}
#endif /* defined(RT_CFG80211_SUPPORT) && defined(RT_CFG80211_P2P_SUPPORT) */

	RTMP_IO_READ32(ad, EXT_CCA_CFG, &RegValue);
	RegValue &= ~0xFFF;
	if (ad->CommonCfg.BBPCurrentBW == BW_80) {
		bw = 2;
		bbp_ch_idx = vht_prim_ch_idx(channel, ad->CommonCfg.Channel);
		if (bbp_ch_idx == 0)
			RegValue |= 0x1e4;
		else if (bbp_ch_idx == 1)
			RegValue |= 0x2e1;
		else if (bbp_ch_idx == 2)
			RegValue |= 0x41e;
		else if (bbp_ch_idx == 3)
			RegValue |= 0x81b;
	} else if (ad->CommonCfg.BBPCurrentBW == BW_40) {
		bw = 1;
		if (ad->CommonCfg.CentralChannel > ad->CommonCfg.Channel) {
			bbp_ch_idx = EXT_CH_ABOVE;
			RegValue |= 0x1e4;
		} else {
			bbp_ch_idx = EXT_CH_BELOW;
			RegValue |= 0x2e1;
		}
	} else {
		bw = 0;
		bbp_ch_idx = EXT_CH_NONE;
		RegValue |= 0x1e4;
	}
	RTMP_IO_WRITE32(ad, EXT_CCA_CFG, RegValue);

	/* determine channel flags */
	if (channel > 14)
		band = _A_BAND;
	else
		band = _G_BAND;

	if (!ad->MCUCtrl.power_on)
		band_change = TRUE;
	else {
		if (ad->LatchRfRegs.Channel > 14)
			latch_band = _A_BAND;
		else
			latch_band = _G_BAND;

		if (band != latch_band)
			band_change = TRUE;
		else
			band_change = FALSE;
	}

	/* Change MAC OFDM SIFS according to BW */
	RTMP_IO_READ32(ad, XIFS_TIME_CFG, &RegValue);
	RegValue &= ~XIFS_TIME_OFDM_SIFS_MASK;
	if ((WMODE_CAP_AC(ad->CommonCfg.PhyMode) && (ad->CommonCfg.vht_ldpc == TRUE)) ||
	    (WMODE_CAP_N(ad->CommonCfg.PhyMode) && (ad->CommonCfg.ht_ldpc == TRUE))) {
		if (bw == 0)
			RegValue |= XIFS_TIME_OFDM_SIFS(0x0D);
		else		/* 40/80Mhz */
			RegValue |= XIFS_TIME_OFDM_SIFS(0x0E);
	} else
		RegValue |= XIFS_TIME_OFDM_SIFS(0x0C);
	RTMP_IO_WRITE32(ad, XIFS_TIME_CFG, RegValue);

	if ((ad->CommonCfg.TxStream == 1) && (ad->CommonCfg.RxStream == 1))
		tx_rx_setting = 0x101;
	else if ((ad->CommonCfg.TxStream == 2) && (ad->CommonCfg.RxStream == 1))
		tx_rx_setting = 0x201;
	else if ((ad->CommonCfg.TxStream == 1) && (ad->CommonCfg.RxStream == 2))
		tx_rx_setting = 0x102;
	else if ((ad->CommonCfg.TxStream == 2) && (ad->CommonCfg.RxStream == 2))
		tx_rx_setting = 0x202;
	else
		tx_rx_setting = 0x202;

	/* GeK: copied from mt76x2_ate_switch_channel(). [todo] merge as
	 * a common function.
	 */
	if (band_change) {
		RTMP_REG_PAIR *band_tb;
		RTMP_REG_PAIR *pa_tb;
		UCHAR band_num;
		UCHAR pa_num;
		/* mac setting per band */
		if (band == _G_BAND) {
			band_tb = mt76x2_mac_g_band_cr_table;
			band_num = mt76x2_mac_g_band_cr_nums;

			if (ad->chipCap.PAType & INT_PA_2G) {
				pa_tb = mt76x2_mac_g_band_internal_pa_cr_table;
				pa_num = mt76x2_mac_g_band_internal_pa_cr_nums;
			} else {
				pa_tb = mt76x2_mac_g_band_external_pa_cr_table;
				pa_num = mt76x2_mac_g_band_external_pa_cr_nums;
			}
		} else {
			band_tb = mt76x2_mac_a_band_cr_table;
			band_num = mt76x2_mac_a_band_cr_nums;

			if (ad->chipCap.PAType & INT_PA_5G) {
				pa_tb = mt76x2_mac_a_band_internal_pa_cr_table;
				pa_num = mt76x2_mac_a_band_internal_pa_cr_nums;
			} else {
				pa_tb = mt76x2_mac_a_band_external_pa_cr_table;
				pa_num = mt76x2_mac_a_band_external_pa_cr_nums;
			}
		}

		if (IS_PCI_INF(ad)) {
			UINT32 i;

			for (i = 0; i < band_num; i++)
				RTMP_IO_WRITE32(ad, band_tb[i].Register,
						band_tb[i].Value);

			for (i = 0; i < pa_num; i++)
				RTMP_IO_WRITE32(ad, pa_tb[i].Register,
						pa_tb[i].Value);
		} else if (IS_USB_INF(ad)) {
			RANDOM_WRITE(ad, band_tb, band_num);
			RANDOM_WRITE(ad, pa_tb, pa_num);
		}
	}


	if (IS_MT7602(ad)) {
		/* default use 2 stream to Tx HW auto gened packets */
		RTMP_IO_WRITE32(ad, 0x1648, 0x00830083);
	}

	/* Fine tune tx power ramp on time based on BBP Tx delay */
	if (isExternalPAMode(ad, channel)) {
		if (bw == 0)
			RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x00101101);
		else
			RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x000B0C01);

		RTMP_IO_WRITE32(ad, TX_SW_CFG1, 0x00011414);
	} else {
		if (bw == 0)
			RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x00101001);
		else
			RTMP_IO_WRITE32(ad, TX_SW_CFG0, 0x000B0B01);

		RTMP_IO_WRITE32(ad, TX_SW_CFG1, 0x00021414);
	}

	/* tx pwr gain setting */
	mt76x2_tx_pwr_gain(ad, channel, bw);

	/* per-rate power delta */
	mt76x2_adjust_per_rate_pwr_delta(ad, channel, 0);

	ret = andes_switch_channel(ad, channel, scan, bw, tx_rx_setting, bbp_ch_idx);

	if (ret == NDIS_STATUS_SUCCESS && !IS_MT76x2T(ad)) {
		if ((WMODE_CAP_AC(ad->CommonCfg.PhyMode) && (ad->CommonCfg.vht_ldpc == TRUE)) ||
	    (WMODE_CAP_N(ad->CommonCfg.PhyMode) && (ad->CommonCfg.ht_ldpc == TRUE))) {
			if (bw == 0)
				/*Reduce LDPC iteration count*/
				RTMP_IO_WRITE32(ad, 0x29ac, 0x24);
		}
	}

#ifdef NEW_WOW_SUPPORT
	ad->WOW_Cfg.CentralChannel = channel;
	ad->WOW_Cfg.ExtChSetting = bbp_ch_idx;
#endif /* NEW_WOW_SUPPORT */

	eLNA_gain_from_e2p = (ad->ALNAGain2 & 0xFF) << 24;
	eLNA_gain_from_e2p |= (ad->ALNAGain1 & 0xFF) << 16;
	eLNA_gain_from_e2p |= (ad->ALNAGain0 & 0xFF) << 8;
	eLNA_gain_from_e2p |= ad->BLNAGain & 0xFF;
	andes_init_gain(ad, channel, TRUE, eLNA_gain_from_e2p);

#ifdef ED_MONITOR
	/* Restore ED threshold */
	if (ad->ed_chk && (cap->fw_non_support & fNONSUPPORT_EDCCA_UPDATE))
		RTMP_BBP_IO_WRITE32(ad, AGC1_R2, ed_th_backup);
#endif /* ED_MONITOR */

	RTMP_BBP_IO_READ32(ad, AGC1_R8, &value);
	DBGPRINT(RT_DEBUG_INFO, ("%s::BBP 0x2320=0x%08x\n", __func__, value));
	RTMP_BBP_IO_READ32(ad, AGC1_R9, &value);
	DBGPRINT(RT_DEBUG_INFO, ("%s::BBP 0x2324=0x%08x\n", __func__, value));
	RTMP_BBP_IO_READ32(ad, AGC1_R4, &value);
	DBGPRINT(RT_DEBUG_INFO, ("%s::BBP 0x2310=0x%08x\n", __func__, value));
	RTMP_BBP_IO_READ32(ad, AGC1_R5, &value);
	DBGPRINT(RT_DEBUG_INFO, ("%s::BBP 0x2314=0x%08x\n", __func__, value));

	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		/* LDPC RX */
		RTMP_BBP_IO_READ32(ad, 0x2934, &value);
		value |= (1 << 10);
		RTMP_BBP_IO_WRITE32(ad, 0x2934, value);
	}

	/* Disable ED_CCA Block Tx Assertion */
	if (IS_USB_INF(ad)) {
		UINT32 mac_val = 0;

		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		if ((mac_val & 0x100000) == 0x100000) {
			DBGPRINT(RT_DEBUG_INFO, ("%s:: ED CCA has been enabled\n", __func__));
			ad->chipCap.ed_cca_enable = TRUE;
			mac_val &= ~(1 << 20);
			RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

			RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
			mac_val &= ~2;
			RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
		} else
			ad->chipCap.ed_cca_enable = FALSE;
	}

	/* Backup the original RTS retry count and then set to 0 */
	RTMP_IO_READ32(ad, TX_RTS_CFG, &ad->rts_tx_retry_num);

	/* backup mac 1004 value */
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &restore_value);

#ifdef RTMP_USB_SUPPORT
	mt76x2_polling_txq_empty(ad);
#else
	OS_WAIT(10);
#endif

	/* disable mac tx/rx */
	value = restore_value;
	value &= ~0xC;
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, value);

	/* set RTS retry count = 0 */
	RTMP_IO_WRITE32(ad, TX_RTS_CFG, 0x00092B00);

	/* wait mac 0x1200, bbp 0x2130 idle */
	do {
		RTMP_IO_READ32(ad, MAC_STATUS_CFG, &value);
		value &= 0x1;
		RTMP_BBP_IO_READ32(ad, 0x2130, &value1);
		DBGPRINT(RT_DEBUG_INFO,
			 ("%s:: Wait until MAC 0x1200 bit0 and BBP 0x2130 become 0\n", __func__));
		/*RtmpusecDelay(1);*/
		RtmpusecDelay(100); /* might need 100*200us */
		loop++;
	} while (((value != 0) || (value1 != 0)) && (loop < 300));

	if (loop >= 300) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s:: Wait until MAC 0x1200 bit0 and BBP 0x2130 become 0 > 300 times\n",
			  __func__));

		if ((value == 0) && (value1 != 0)) {
			DBGPRINT(RT_DEBUG_OFF,
				 ("%s:: doing IBI and core soft progress\n", __func__));

			RTMP_BBP_IO_READ32(ad, CORE_R4, &value1);
			value1 |= 0x2;
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, value1);

			RTMP_BBP_IO_READ32(ad, CORE_R4, &value1);
			value1 &= ~0x2;
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, value1);

			RTMP_BBP_IO_READ32(ad, CORE_R4, &value1);
			value1 |= 0x1;
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, value1);

			RTMP_BBP_IO_READ32(ad, CORE_R4, &value1);
			value1 &= ~0x1;
			RTMP_BBP_IO_WRITE32(ad, CORE_R4, value1);
		}
	} else if (loop > 100)
		DBGPRINT(RT_DEBUG_TRACE, ("%s: loop %d\n", __func__, loop));

	if (!ad->MCUCtrl.power_on) {
		RT28xx_EEPROM_READ16(ad, BT_RCAL_RESULT, e2p_value);

		if ((e2p_value & 0xff) != 0xff) {
			DBGPRINT(RT_DEBUG_OFF, ("r-cal result = %d\n", e2p_value & 0xff));
			CHIP_CALIBRATION(ad, R_CALIBRATION_7662, 0x00);
		}
	}

	/* RXDCOC calibration */
	CHIP_CALIBRATION(ad, RXDCOC_CALIBRATION_7662, channel);

	if (!ad->MCUCtrl.power_on) {
		/* RX LPF calibration */
		CHIP_CALIBRATION(ad, RC_CALIBRATION_7662, 0x00);
	}

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(ad)) {
		UINT32 ret;

		RTMP_SEM_EVENT_WAIT(&ad->tssi_lock, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("tssi_lock get failed(ret=%d)\n", ret));
			return;
		}
	}
#endif /* RTMP_MAC_USB */

#ifdef RTMP_PCI_SUPPORT
	if (IS_PCI_INF(ad))
		NdisAcquireSpinLock(&ad->tssi_lock);
#endif /* RTMP_PCI_SUPPORT */

	/* TSSI Clibration */
	if (!IS_DOT11_H_RADAR_STATE(ad, RD_SILENCE_MODE)
	    && !ad->chipCap.temp_tx_alc_enable
#ifdef ED_MONITOR
	    && !ad->ed_tx_stopped
#endif /* ED_MOINTOR */
	   )
		mt76x2_tssi_calibration(ad, channel);

	/* enable TX/RX */
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, 0xC | restore_value);

#ifdef RTMP_USB_SUPPORT
	if (ad->chipCap.ed_cca_enable) {
		UINT32 mac_val = 0;

		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val |= (1 << 20);
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
		mac_val |= 2;
		RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
	}
#endif /* RTMP_USB_SUPPORT */

	/* Restore RTS retry count */
	RTMP_IO_WRITE32(ad, TX_RTS_CFG, ad->rts_tx_retry_num);

	if (!ad->MCUCtrl.power_on && ad->chipCap.tssi_enable && !ad->chipCap.temp_tx_alc_enable) {
		RTMP_IO_READ32(ad, TX_ALC_CFG_1, &value);
		value = value & (~TX_ALC_CFG_1_TX0_TEMP_COMP_MASK);
		value |= TX_ALC_CFG_1_TX0_TEMP_COMP(0x38);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_1, value);
		DBGPRINT(RT_DEBUG_OFF, ("TX0 power compensation = 0x%x\n", value & 0x3f));
		RTMP_IO_READ32(ad, TX_ALC_CFG_2, &value);
		value = value & (~TX_ALC_CFG_2_TX1_TEMP_COMP_MASK);
		value |= TX_ALC_CFG_2_TX1_TEMP_COMP(0x38);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_2, value);
		DBGPRINT(RT_DEBUG_OFF, ("TX1 power compensation = 0x%x\n", value & 0x3f));
	}
#ifdef RTMP_PCI_SUPPORT
	if (IS_PCI_INF(ad))
		NdisReleaseSpinLock(&ad->tssi_lock);
#endif /* RTMP_PCI_SUPPORT */

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(ad))
		RTMP_SEM_EVENT_UP(&ad->tssi_lock);
#endif /* RTMP_MAC_USB */

	/* Channel latch */
	ad->LatchRfRegs.Channel = channel;

	if (!ad->MCUCtrl.power_on)
		ad->MCUCtrl.power_on = TRUE;

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(ad))
		RTMP_SEM_EVENT_UP(&ad->hw_atomic);
#endif /* RTMP_MAC_USB */

	/* mt76x2_set_ed_cca(ad, TRUE); */

#ifdef SINGLE_SKU_V2
	mt76x2_single_sku(ad, channel);
#endif /* SINGLE_SKU_V2 */

	percentage_delta_pwr(ad);

#ifdef DYNAMIC_VGA_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	RTMP_BBP_IO_READ32(ad, AGC1_R35, &ad->chipCap.agc1_r35_backup);
	RTMP_BBP_IO_READ32(ad, AGC1_R39, &ad->chipCap.agc1_r39_backup);
	RTMP_BBP_IO_READ32(ad, AGC1_R41, &ad->chipCap.agc1_r41_backup);
#endif /* CONFIG_AP_SUPPORT */

	RTMP_BBP_IO_WRITE32(ad, AGC1_R61, 0xFF64A4E2);	/* microwave's function initial gain */
	RTMP_BBP_IO_WRITE32(ad, AGC1_R7, 0x08081010);	/* microwave's ED CCA threshold */
	RTMP_BBP_IO_WRITE32(ad, AGC1_R11, 0x00000404);	/* microwave's ED CCA threshold */
	/* initial ED CCA threshold */
	if (ad->CommonCfg.DynamicVGA_Disabled) {
		RTMP_BBP_IO_WRITE32(ad, AGC1_R2, 0x00001414);
		/* Note: In TGac certification, the dynamic VGA will be turned off,
		and requires to turn on EDCCA by default. */
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, 0x04101B3F);
	}
#endif /* DYNAMIC_VGA_SUPPORT */

#if defined(RT_CFG80211_SUPPORT) && defined(RT_CFG80211_P2P_SUPPORT)
	if (RTMP_CFG80211_VIF_P2P_GO_ON(ad)) {
		/* should restore Beacon if it was disabled before switching */
		if (BeaconEnabled) {
			RTMP_IO_READ32(ad, BCN_TIME_CFG, &csr.word);
			csr.field.bBeaconGen = 1;
			RTMP_IO_WRITE32(ad, BCN_TIME_CFG, csr.word);
		}
	}
#endif /* defined(RT_CFG80211_SUPPORT) && defined(RT_CFG80211_P2P_SUPPORT) */

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s(): Switch to Ch#%d(%dT%dR), BBP_BW=%d, bbp_ch_idx=%d)\n",
		  __func__,
		  channel,
		  ad->CommonCfg.TxStream,
		  ad->CommonCfg.RxStream, ad->CommonCfg.BBPCurrentBW, bbp_ch_idx));
}

void mt76x2_external_pa_rf_dac_control(RTMP_ADAPTER *ad, u8 channel)
{
	if (isExternalPAMode(ad, channel)) {
		if (!strncmp((PSTRING) ad->CommonCfg.CountryCode, "US", 2)
		    && (channel >= 36 && channel <= 48)) {
			mt_rf_write(ad, 0, 0x058, 0x226C6000);
			mt_rf_write(ad, 1, 0x058, 0x226C6000);
			DBGPRINT(RT_DEBUG_OFF,
				 ("%s::change MT7612E's RF DAC control range since in US band 1 frequency range\n",
				  __func__));
		}
	}
}

void mt76x2_tssi_calibration(RTMP_ADAPTER *ad, u8 channel)
{
	/* TSSI Clibration */
	if (ad->chipCap.tssi_enable) {
		ad->chipCap.tssi_stage = TSSI_CAL_STAGE;
		if (channel > 14) {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0101);
			else if (ad->chipCap.PAType == EXT_PA_5G_ONLY)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0101);
			else
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0001);
		} else {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0100);
			else if ((ad->chipCap.PAType == EXT_PA_5G_ONLY) ||
				 (ad->chipCap.PAType == INT_PA_2G_5G))
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0000);
			else if (ad->chipCap.PAType == EXT_PA_2G_ONLY)
				CHIP_CALIBRATION(ad, TSSI_CALIBRATION_7662, 0x0100);
			else
				DBGPRINT(RT_DEBUG_ERROR,
					 ("illegal PA Type(%d)\n", ad->chipCap.PAType));
		}
		ad->chipCap.tssi_stage = TSSI_TRIGGER_STAGE;
	}
	DBGPRINT(RT_DEBUG_INFO, ("%s --> tssi_stage = %d , PAType = %d\n",
				  __func__, ad->chipCap.tssi_stage, ad->chipCap.PAType));
}

void mt76x2_tssi_compensation(RTMP_ADAPTER *ad, u8 channel)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	ANDES_CALIBRATION_PARAM param;
	UINT32 pa_mode = 0, tssi_slope_offset = 0, value = 0;

#ifdef RTMP_USB_SUPPORT
	if (IS_USB_INF(ad)) {
		UINT32 ret;

		RTMP_SEM_EVENT_WAIT(&ad->tssi_lock, ret);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("tssi_lock get failed(ret=%d)\n", ret));
			return;
		}
	}
#endif /* endif */


	DBGPRINT(RT_DEBUG_INFO, ("%s --> tssi_stage = %d, channel = %d\n",
				  __func__, ad->chipCap.tssi_stage, channel));
	if (ad->chipCap.tssi_stage <= TSSI_CAL_STAGE)
		goto done;

	if (cap->tssi_stage == TSSI_TRIGGER_STAGE) {
		DBGPRINT(RT_DEBUG_INFO, ("%s:TSS_TRIGGER(channel = %d)\n", __func__, channel));
		param.mt76x2_tssi_comp_param.pa_mode = (1 << 8);
		param.mt76x2_tssi_comp_param.tssi_slope_offset = 0;

		/* TSSI Trigger */
		if (ad->chipOps.Calibration != NULL)
			ad->chipOps.Calibration(ad, TSSI_COMPENSATION_7662, &param);
		else
			goto done;

		cap->tssi_stage = TSSI_COMP_STAGE;

		goto done;
	}

	/* Check 0x2088[4] = 0 */
	RTMP_BBP_IO_READ32(ad, CORE_R34, &value);

	if ((value & (1 << 4)) == 0) {

		DBGPRINT(RT_DEBUG_INFO, ("%s:TSSI_COMP(channel = %d)\n", __func__, channel));

		if (channel > 14) {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				pa_mode = 1;
			else if (ad->chipCap.PAType == EXT_PA_5G_ONLY)
				pa_mode = 1;
			else
				pa_mode = 0;
		} else {
			if (ad->chipCap.PAType == EXT_PA_2G_5G)
				pa_mode = 1;
			else if ((ad->chipCap.PAType == EXT_PA_5G_ONLY) ||
				 (ad->chipCap.PAType == INT_PA_2G_5G))
				pa_mode = 0;
			else if (ad->chipCap.PAType == EXT_PA_2G_ONLY)
				pa_mode = 1;
		}

		if (channel <= 14) {
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET0_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_g_band);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET1_MASK;
			tssi_slope_offset |= TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_g_band);
		} else {
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE0_MASK;
			tssi_slope_offset |=
			    TSSI_PARAM2_SLOPE0(cap->tssi_0_slope_a_band[get_chl_grp(channel)]);
			tssi_slope_offset &= ~TSSI_PARAM2_SLOPE1_MASK;
			tssi_slope_offset |=
			    TSSI_PARAM2_SLOPE1(cap->tssi_1_slope_a_band[get_chl_grp(channel)]);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET0_MASK;
			tssi_slope_offset |=
			    TSSI_PARAM2_OFFSET0(cap->tssi_0_offset_a_band[get_chl_grp(channel)]);
			tssi_slope_offset &= ~TSSI_PARAM2_OFFSET1_MASK;
			tssi_slope_offset |=
			    TSSI_PARAM2_OFFSET1(cap->tssi_1_offset_a_band[get_chl_grp(channel)]);
		}

		param.mt76x2_tssi_comp_param.pa_mode = (pa_mode | ((0x1) << 9));
		param.mt76x2_tssi_comp_param.tssi_slope_offset = tssi_slope_offset;

		/* TSSI Compensation */
		if (ad->chipOps.Calibration != NULL)
			ad->chipOps.Calibration(ad, TSSI_COMPENSATION_7662, &param);

		cap->tssi_stage = TSSI_TRIGGER_STAGE;

		if (!ad->MCUCtrl.dpd_on
#ifdef ED_MONITOR
		    && (ad->ed_tx_stopped == FALSE)
#endif /* ED_MONITOR */
		    ) {
#ifdef RTMP_PCI_SUPPORT
			RtmpOsMsDelay(10);
#endif /* endif */
			/* DPD Calibration */
			if ((ad->chipCap.PAType == INT_PA_2G_5G)
			    || ((ad->chipCap.PAType == INT_PA_5G) && (channel > 14))
			    || ((ad->chipCap.PAType == INT_PA_2G) && (channel <= 14))
			    )
				CHIP_CALIBRATION(ad, DPD_CALIBRATION_7662, channel);
			ad->MCUCtrl.dpd_on = TRUE;
		}
	}

done:
	;

#ifdef RTMP_MAC_USB
	if (IS_USB_INF(ad))
		RTMP_SEM_EVENT_UP(&ad->tssi_lock);
#endif /* endif */
}

void mt76x2_calibration(RTMP_ADAPTER *ad, u8 channel)
{
	UINT32 value, value1, restore_value, loop = 0;

	if (IS_DOT11_H_RADAR_STATE(ad, RD_SILENCE_MODE)) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s():RDMode  is in Silent State, do not calibration.\n", __func__));
		return;
	}
#ifdef RTMP_USB_SUPPORT
	{
		UINT32 mac_val = 0;

		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		if ((mac_val & 0x100000) == 0x100000) {
			ad->chipCap.ed_cca_enable = TRUE;
			mac_val &= ~(1 << 20);
			RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

			RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
			mac_val &= ~2;
			RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
		}
	}
#endif /* RTMP_USB_SUPPORT */

	DBGPRINT(RT_DEBUG_TRACE, ("%s(channel = %d)\n", __func__, channel));

	/* backup mac 1004 value */
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &restore_value);

	/* Backup the original RTS retry count and then set to 0 */
	RTMP_IO_READ32(ad, TX_RTS_CFG, &ad->rts_tx_retry_num);

	/* disable mac tx/rx */
	value = restore_value;
	value &= ~0xC;
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, value);

	/* set RTS retry count = 0 */
	RTMP_IO_WRITE32(ad, TX_RTS_CFG, 0x00092B00);

	/* wait mac 0x1200, bbp 0x2130 idle */
	do {
		RTMP_IO_READ32(ad, MAC_STATUS_CFG, &value);
		value &= 0x1;
		RTMP_BBP_IO_READ32(ad, 0x2130, &value1);
		DBGPRINT(RT_DEBUG_INFO,
			 ("%s:: Wait until MAC 0x1200 bit0 and BBP 0x2130 become 0\n", __func__));
		RtmpusecDelay(1);
		loop++;
	} while (((value != 0) || (value1 != 0)) && (loop < 300));

	if (loop >= 300) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s:: Wait until MAC 0x1200 bit0 and BBP 0x2130 become 0 > 300 times\n",
			  __func__));
	}

	/* LC Calibration */
	if (channel > 14)
		CHIP_CALIBRATION(ad, LC_CALIBRATION_7662, 0x00);

	/* TX LOFT */
	if (channel > 14)
		CHIP_CALIBRATION(ad, TX_LOFT_CALIBRATION_7662, 0x1);
	else
		CHIP_CALIBRATION(ad, TX_LOFT_CALIBRATION_7662, 0x0);

	/* TXIQ Clibration */
	if (channel > 14)
		CHIP_CALIBRATION(ad, TXIQ_CALIBRATION_7662, 0x1);
	else
		CHIP_CALIBRATION(ad, TXIQ_CALIBRATION_7662, 0x0);


	/* RXIQC-FI */
	if (channel > 14)
		CHIP_CALIBRATION(ad, RXIQC_FI_CALIBRATION_7662, 0x1);
	else
		CHIP_CALIBRATION(ad, RXIQC_FI_CALIBRATION_7662, 0x0);


	/* TEMP SENSOR */
	CHIP_CALIBRATION(ad, TEMP_SENSOR_CALIBRATION_7662, 0x00);

	/* Trigger TX-Shaping */
	CHIP_CALIBRATION(ad, TX_SHAPING_CALIBRATION_7662, 0x00);


	/* enable TX/RX */
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, restore_value);

#ifdef RTMP_USB_SUPPORT
	if (ad->chipCap.ed_cca_enable) {
		UINT32 mac_val = 0;

		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val |= (1 << 20);
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
		mac_val |= 2;
		RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
	}
#endif /* RTMP_USB_SUPPORT */

	/* Restore RTS retry count */
	RTMP_IO_WRITE32(ad, TX_RTS_CFG, ad->rts_tx_retry_num);

#ifdef CONFIG_CALIBRATION_COLLECTION
	{
		UCHAR i = 0;

		RtmpOsMsDelay(200);

		for (i = R_CALIBRATION_7662; i <= RXIQC_FD_CALIBRATION_7662; i++) {
			switch (i) {
			case RXDCOC_CALIBRATION_7662:
				record_calibration_info(ad, RXDCOC_CALIBRATION_7662);
				break;
			case RC_CALIBRATION_7662:
				record_calibration_info(ad, RC_CALIBRATION_7662);
				break;
			case LC_CALIBRATION_7662:
				record_calibration_info(ad, LC_CALIBRATION_7662);
				break;
			case TX_LOFT_CALIBRATION_7662:
				record_calibration_info(ad, TX_LOFT_CALIBRATION_7662);
				break;
			case TXIQ_CALIBRATION_7662:
				record_calibration_info(ad, TXIQ_CALIBRATION_7662);
				break;
			case TSSI_CALIBRATION_7662:
				record_calibration_info(ad, TSSI_CALIBRATION_7662);
				break;
			case DPD_CALIBRATION_7662:
				record_calibration_info(ad, DPD_CALIBRATION_7662);
				break;
			case RXIQC_FI_CALIBRATION_7662:
				record_calibration_info(ad, RXIQC_FI_CALIBRATION_7662);
				break;
			case RXIQC_FD_CALIBRATION_7662:
				record_calibration_info(ad, RXIQC_FD_CALIBRATION_7662);
				break;
			default:
				break;
			}
		}
	}
#endif /* CONFIG_CALIBRATION_COLLECTION */
}

static void mt76x2_cal_test(RTMP_ADAPTER *ad, UINT32 type)
{
	UCHAR cent_ch;

#ifdef DOT11_VHT_AC
	if (ad->CommonCfg.BBPCurrentBW == BW_80)
		cent_ch = ad->CommonCfg.vht_cent_ch;
	else
#endif /* DOT11_VHT_AC */
		cent_ch = ad->CommonCfg.CentralChannel;

#ifdef CONFIG_CALIBRATION_COLLECTION
	if (type == CAL_ROBUST_TEST) {
		do_calibrtion_info_for_robust(ad, RXDCOC_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, RC_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, LC_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, TX_LOFT_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, TXIQ_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, TSSI_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, DPD_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, RXIQC_FI_CALIBRATION_7662, cent_ch);
		do_calibrtion_info_for_robust(ad, RXIQC_FD_CALIBRATION_7662, cent_ch);
	}
#endif /* CONFIG_CALIBRATION_COLLECTION */
}

/*
 * Initialize FCE
 */
static VOID init_fce(RTMP_ADAPTER *ad)
{
	L2_STUFFING_STRUC reg;

	reg.word = 0;

#ifdef RTMP_PCI_SUPPORT

#if 1				/* change FCE clock to middile to prevent PCI-e credit overflow */
	/* test the TP on MT7620/ MT7621 platform, the TP is not degrade */
	RTMP_IO_WRITE32(ad, 0x708, 0x1401);
#endif /* endif */
	RTMP_IO_READ32(ad, FCE_L2_STUFF, &reg.word);
	reg.field.FS_WR_MPDU_LEN_EN = 0;
	RTMP_IO_WRITE32(ad, FCE_L2_STUFF, reg.word);
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
	RTUSBReadMACRegister(ad, FCE_L2_STUFF, &reg.word);
	reg.field.FS_WR_MPDU_LEN_EN = 0;
	RTUSBWriteMACRegister(ad, FCE_L2_STUFF, reg.word, FALSE);
#endif /* endif */
}

static void mt76x2_init_mac_cr(RTMP_ADAPTER *ad)
{
	u32 value = 0;
	u16 e2p_value;
	char xtal_freq_offset = 0;

	/*
	   Enable PBF and MAC clock
	   SYS_CTRL[11:10] = 0x3
	 */
#ifdef RTMP_PCI_SUPPORT
	if (IS_PCI_INF(ad)) {
		u32 i;
		for (i = 0; i < mt76x2_mac_cr_nums; i++) {
			RTMP_IO_WRITE32(ad, mt76x2_mac_cr_table[i].Register,
					mt76x2_mac_cr_table[i].Value);
		}
	}
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
	if (IS_USB_INF(ad))
		RANDOM_WRITE(ad, mt76x2_mac_cr_table, mt76x2_mac_cr_nums);
#endif /* endif */

#ifdef HDR_TRANS_TX_SUPPORT
	/*
	 * Enable Header Translation TX
	 */
	RTMP_IO_READ32(ad, HEADER_TRANS_CTRL_REG, &value);
	value |= 0x1;		/* 0x1: TX, 0x2: RX */
	RTMP_IO_WRITE32(ad, HEADER_TRANS_CTRL_REG, value);
#endif /* HDR_TRANS_TX_SUPPORT */

	/*
	 * Release BBP and MAC reset
	 * MAC_SYS_CTRL[1:0] = 0x0
	 */
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &value);
	value &= ~(0x3);
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, value);

	if (IS_MT7612(ad)) {
		/*
		 * Disable COEX_EN
		 */
		RTMP_IO_READ32(ad, COEXCFG0, &value);
		value &= 0xFFFFFFFE;
		RTMP_IO_WRITE32(ad, COEXCFG0, value);
	}

	/*
	   Set 0x141C[15:12]=0xF
	 */
	RTMP_IO_READ32(ad, EXT_CCA_CFG, &value);
	value |= (0x0000F000);
	RTMP_IO_WRITE32(ad, EXT_CCA_CFG, value);

	/*
	 * Set 0x13C0[31] = 0x0
	 */
	RTMP_IO_READ32(ad, TX_ALC_CFG_4, &value);
	value &= ~WL_LOWGAIN_CH_EN;
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_4, value);

	/*
	 * Check crystal trim2 first
	 */
	RT28xx_EEPROM_READ16(ad, G_BAND_BANDEDGE_PWR_BACK_OFF, e2p_value);

	if (((e2p_value & 0xff) == 0x00) || ((e2p_value & 0xff) == 0xff))
		xtal_freq_offset = 0;
	else if ((e2p_value & 0x80) == 0x80)
		xtal_freq_offset = 0 - (e2p_value & 0x7f);
	else if ((e2p_value & 0x80) == 0x00)
		xtal_freq_offset = (e2p_value & 0x7f);

	if ((((e2p_value >> 8) & 0xff) == 0x00) || (((e2p_value >> 8) & 0xff) == 0xff)) {
		/*
		 * Compesate crystal trim1
		 */
		RT28xx_EEPROM_READ16(ad, XTAL_TRIM1, e2p_value);

		/* crystal trim default value set to 0x14 */
		if (((e2p_value & 0xff) == 0x00) || ((e2p_value & 0xff) == 0xff))
			e2p_value = 0x14;

		/* Set crystal trim1 */
		read_reg(ad, 0x40, XO_CTRL5, &value);
		value &= 0xffff80ff;
		value |=
		    ((((e2p_value & XTAL_TRIM1_MASK) + xtal_freq_offset) & XTAL_TRIM1_MASK) << 8);
		write_reg(ad, 0x40, XO_CTRL5, value);

		/* Enable */
		read_reg(ad, 0x40, XO_CTRL6, &value);
		value &= 0xffff80ff;
		value |= (0x7f << 8);
		write_reg(ad, 0x40, XO_CTRL6, value);
	} else {
		/* Set crystal trim2 */
		read_reg(ad, 0x40, XO_CTRL5, &value);
		value &= 0xffff80ff;
		value |=
		    (((e2p_value & XTAL_TRIM2_MASK) + (xtal_freq_offset << 8)) & XTAL_TRIM2_MASK);
		write_reg(ad, 0x40, XO_CTRL5, value);

		/* Enable */
		read_reg(ad, 0x40, XO_CTRL6, &value);
		value &= 0xffff80ff;
		value |= (0x7f << 8);
		write_reg(ad, 0x40, XO_CTRL6, value);
	}

	/*
	 * add 504, 50c value per ben kao suggestion for rx receivce packet, need to revise this bit
	 * only mt7662u do not this setting
	 */
	if (IS_MT76x2E(ad) || IS_MT76x2U(ad)) {
		RTMP_IO_WRITE32(ad, 0x504, 0x06000000);
		RTMP_IO_WRITE32(ad, 0x50c, 0x08800000);
		RtmpOsMsDelay(5);
		RTMP_IO_WRITE32(ad, 0x504, 0x0);
	}

	/* Decrease MAC OFDM SIFS from 16 to 13us */
	RTMP_IO_READ32(ad, XIFS_TIME_CFG, &value);
	value = value & (~XIFS_TIME_OFDM_SIFS_MASK);
	value |= XIFS_TIME_OFDM_SIFS(0x0D);
	RTMP_IO_WRITE32(ad, XIFS_TIME_CFG, value);

	RTMP_IO_READ32(ad, BKOFF_SLOT_CFG, &value);
	value &= ~(BKOFF_SLOT_CFG_CC_DELAY_TIME_MASK);
	value |= BKOFF_SLOT_CFG_CC_DELAY_TIME(0x01);
	RTMP_IO_WRITE32(ad, BKOFF_SLOT_CFG, value);

	init_fce(ad);

#ifdef MCS_LUT_SUPPORT
	RTMP_IO_READ32(ad, TX_FBK_LIMIT, &value);
	if (RTMP_TEST_MORE_FLAG(ad, fASIC_CAP_MCS_LUT))
		value |= 0x40000;
	else
		value &= (~0x40000);
	RTMP_IO_WRITE32(ad, TX_FBK_LIMIT, value);
#endif /* MCS_LUT_SUPPORT */

	/*
	   For co-clock image
	   00 : one crystal , disable co-clock out
	   01 : One crystal, enable co-clock out
	   10 : Two crystal (Default)
	 */
	RTMP_IO_READ32(ad, 0x11C, &value);
	if (ad->NicConfig3.field.XtalOption == 0x0)
		value = 0x5C1FEE80;
	else if (ad->NicConfig3.field.XtalOption == 0x1)
		value = 0x5C1FEED0;
	RTMP_IO_WRITE32(ad, 0x11C, value);

#ifdef RTMP_PCI_SUPPORT
	if (IS_PCI_INF(ad)) {
		/* ASPM: set rg_ssusb_g1_cdr_bir_ltr = 0x9 */
		read_reg(ad, 0x40, 0x5A10, &value);
		value &= 0xFFE0FFFF;
		value |= (0x9 << 16);
		write_reg(ad, 0x40, 0x5A10, value);

		/* ASPM: set rg_ssusb_g1_cdr_bic_ltr = 0xF */
		read_reg(ad, 0x40, 0x5A0C, &value);
		value &= 0x0FFFFFFF;
		value |= (0xF << 28);
		write_reg(ad, 0x40, 0x5A0C, value);

		/* ASPM: set RG_SSUSB_CDR_BR_PE1D = 0x3 */
		read_reg(ad, 0x40, 0x5C58, &value);
		value &= 0xFFFFFE3F;
		value |= (0x3 << 6);
		write_reg(ad, 0x40, 0x5C58, value);
	}
#endif /* RTMP_PCI_SUPPORT */
}

static void mt76x2_init_rf_cr(RTMP_ADAPTER *ad)
{
	andes_load_cr(ad, RF_BBP_CR, 0, 0);
}

#ifdef DYNAMIC_VGA_SUPPORT
void mt76x2_get_external_lna_gain(RTMP_ADAPTER *ad)
{
	USHORT e2p_val = 0;
	UINT8 lna_type = 0;

	/* b'00: 2.4G+5G external LNA, b'01: 5G external LNA, b'10: 2.4G external LNA, b'11: Internal LNA */
	RT28xx_EEPROM_READ16(ad, 0x36, e2p_val);
	lna_type = e2p_val & 0xC;
	if (lna_type == 0xC)
		ad->chipCap.LNA_type = 0x0;
	else if (lna_type == 0x8)
		ad->chipCap.LNA_type = 0x1;
	else if (lna_type == 0x4)
		ad->chipCap.LNA_type = 0x10;
	else if (lna_type == 0x0)
		ad->chipCap.LNA_type = 0x11;

	RT28xx_EEPROM_READ16(ad, 0x44, e2p_val);
	ad->BLNAGain = (e2p_val & 0xFF);	/* store external LNA gain for 2.4G on EEPROM 0x44h */
	/* store external LNA gain for 5G ch#36 ~ ch#64 on EEPROM 0x45h */
	ad->ALNAGain0 = (e2p_val & 0xFF00) >> 8;

	RT28xx_EEPROM_READ16(ad, 0x48, e2p_val);
	/* store external LNA gain for 5G ch#100 ~ ch#128 on EEPROM 0x49h */
	ad->ALNAGain1 = (e2p_val & 0xFF00) >> 8;

	RT28xx_EEPROM_READ16(ad, 0x4C, e2p_val);
	/* store external LNA gain for 5G ch#132 ~ ch#165 on EEPROM 0x4Dh */
	ad->ALNAGain2 = (e2p_val & 0xFF00) >> 8;

	DBGPRINT(RT_DEBUG_ERROR,
		 ("%s::LNA type=0x%x, BLNAGain=0x%x, ALNAGain0=0x%x, ALNAGain1=0x%x, ALNAGain2=0x%x\n",
		  __func__, ad->chipCap.LNA_type, ad->BLNAGain, ad->ALNAGain0, ad->ALNAGain1,
		  ad->ALNAGain2));
}

void mt76x2_get_agc_gain(RTMP_ADAPTER *ad, BOOLEAN init_phase)
{
	UCHAR val;
	USHORT val16;
	UINT32 bbp_val;

	RTMP_BBP_IO_READ32(ad, AGC1_R8, &bbp_val);
	val = ((bbp_val & (0x00007f00)) >> 8) & 0x7f;
	ad->CommonCfg.lna_vga_ctl.agc_vga_init_0 = val;
	ad->CommonCfg.lna_vga_ctl.agc1_r8_backup = bbp_val;
	if (init_phase == TRUE) {
		ad->CommonCfg.lna_vga_ctl.agc_vga_ori_0 = ad->CommonCfg.lna_vga_ctl.agc_vga_init_0;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("original vga value(chain0) = %x\n",
			  ad->CommonCfg.lna_vga_ctl.agc_vga_ori_0));
	}
	val16 = ((bbp_val & (0xffff0000)) >> 16) & (0xffff);
	ad->CommonCfg.lna_vga_ctl.agc_0_vga_set1_2 = val16;
	DBGPRINT(RT_DEBUG_TRACE,
		 ("initial vga value(chain0) = %x\n", ad->CommonCfg.lna_vga_ctl.agc_vga_init_0));

	if (ad->CommonCfg.RxStream >= 2) {
		RTMP_BBP_IO_READ32(ad, AGC1_R9, &bbp_val);
		val = ((bbp_val & (0x00007f00)) >> 8) & 0x7f;
		ad->CommonCfg.lna_vga_ctl.agc_vga_init_1 = val;
		ad->CommonCfg.lna_vga_ctl.agc1_r9_backup = bbp_val;
		if (init_phase == TRUE) {
			ad->CommonCfg.lna_vga_ctl.agc_vga_ori_1 =
			    ad->CommonCfg.lna_vga_ctl.agc_vga_init_1;
			DBGPRINT(RT_DEBUG_TRACE,
				 ("original vga value(chain1) = %x\n",
				  ad->CommonCfg.lna_vga_ctl.agc_vga_ori_1));
		}
		val16 = ((bbp_val & (0xffff0000)) >> 16) & (0xffff);
		ad->CommonCfg.lna_vga_ctl.agc_1_vga_set1_2 = val16;

		DBGPRINT(RT_DEBUG_TRACE,
			 ("initial vga value(chain1) = %x\n",
			  ad->CommonCfg.lna_vga_ctl.agc_vga_init_1));
	}

	ad->CommonCfg.lna_vga_ctl.bDyncVgaEnable = TRUE;
}

int mt76x2_reinit_agc_gain(RTMP_ADAPTER *ad, u8 channel)
{
	UINT32 value0, value1;
	CHAR agc_vga0, agc_vga1;
	UINT8 chl_grp;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	RTMP_BBP_IO_READ32(ad, AGC1_R8, &value0);
	agc_vga0 = ((value0 & (0x00007f00)) >> 8) & 0x7f;

	RTMP_BBP_IO_READ32(ad, AGC1_R9, &value1);
	agc_vga1 = ((value1 & (0x00007f00)) >> 8) & 0x7f;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s:original agc_vga0 = 0x%x, agc_vga1 = 0x%x\n", __func__, agc_vga0, agc_vga1));

	if (channel > 14) {
		chl_grp = get_chl_grp(channel);
		switch (chl_grp) {
		case A_BAND_GRP0_CHL:
			agc_vga0 += cap->rf0_5g_grp0_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp0_rx_high_gain;
			break;
		case A_BAND_GRP1_CHL:
			agc_vga0 += cap->rf0_5g_grp1_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp1_rx_high_gain;
			break;
		case A_BAND_GRP2_CHL:
			agc_vga0 += cap->rf0_5g_grp2_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp2_rx_high_gain;
			break;
		case A_BAND_GRP3_CHL:
			agc_vga0 += cap->rf0_5g_grp3_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp3_rx_high_gain;
			break;
		case A_BAND_GRP4_CHL:
			agc_vga0 += cap->rf0_5g_grp4_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp4_rx_high_gain;
			break;
		case A_BAND_GRP5_CHL:
			agc_vga0 += cap->rf0_5g_grp5_rx_high_gain;
			agc_vga1 += cap->rf1_5g_grp5_rx_high_gain;
			break;
		default:
			DBGPRINT(RT_DEBUG_OFF, ("illegal channel group(%d)\n", chl_grp));
			break;
		}
	} else {
		agc_vga0 += cap->rf0_2g_rx_high_gain;
		agc_vga1 += cap->rf1_2g_rx_high_gain;
	}

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s:updated agc_vga0 = 0x%x, agc_vga1 = 0x%x\n", __func__, agc_vga0, agc_vga1));

	value0 &= 0xffff80ff;
	value0 |= ((0x7f & agc_vga0) << 8);

	value1 &= 0xffff80ff;
	value1 |= ((0x7f & agc_vga1) << 8);

	RTMP_BBP_IO_WRITE32(ad, AGC1_R8, value0);
	RTMP_BBP_IO_WRITE32(ad, AGC1_R9, value1);

	return 0;
}

int mt76x2_reinit_hi_lna_gain(RTMP_ADAPTER *ad, u8 channel)
{
	UINT32 value0, value1;
	CHAR hi_lna0, hi_lna1;
	UINT8 chl_grp;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	RTMP_BBP_IO_READ32(ad, AGC1_R4, &value0);
	hi_lna0 = ((value0 & (0x003f0000)) >> 16) & 0x3f;

	RTMP_BBP_IO_READ32(ad, AGC1_R5, &value1);
	hi_lna1 = ((value1 & (0x003f0000)) >> 16) & 0x3f;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s:original hi_lna0 = 0x%x, hi_lna1 = 0x%x\n", __func__, hi_lna0, hi_lna1));

	if (channel > 14) {
		chl_grp = get_chl_grp(channel);
		switch (chl_grp) {
		case A_BAND_GRP0_CHL:
			hi_lna0 -= (cap->rf0_5g_grp0_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp0_rx_high_gain / 2);
			break;
		case A_BAND_GRP1_CHL:
			hi_lna0 -= (cap->rf0_5g_grp1_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp1_rx_high_gain / 2);
			break;
		case A_BAND_GRP2_CHL:
			hi_lna0 -= (cap->rf0_5g_grp2_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp2_rx_high_gain / 2);
			break;
		case A_BAND_GRP3_CHL:
			hi_lna0 -= (cap->rf0_5g_grp3_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp3_rx_high_gain / 2);
			break;
		case A_BAND_GRP4_CHL:
			hi_lna0 -= (cap->rf0_5g_grp4_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp4_rx_high_gain / 2);
			break;
		case A_BAND_GRP5_CHL:
			hi_lna0 -= (cap->rf0_5g_grp5_rx_high_gain / 2);
			hi_lna1 -= (cap->rf1_5g_grp5_rx_high_gain / 2);
			break;
		default:
			DBGPRINT(RT_DEBUG_OFF, ("illegal channel group(%d)\n", chl_grp));
			break;
		}
	} else {
		hi_lna0 -= (cap->rf0_2g_rx_high_gain / 2);
		hi_lna1 -= (cap->rf1_2g_rx_high_gain / 2);
	}

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s:updated hi_lna0 = 0x%x, hi_lna1 = 0x%x\n", __func__, hi_lna0, hi_lna1));

	value0 &= 0xffc0ffff;
	value0 |= ((0x3f & hi_lna0) << 16);

	value1 &= 0xffc0ffff;
	value1 |= ((0x3f & hi_lna1) << 16);

	RTMP_BBP_IO_WRITE32(ad, AGC1_R4, value0);
	RTMP_BBP_IO_WRITE32(ad, AGC1_R5, value1);

	return 0;
}

int mt76x2_get_rx_high_gain(RTMP_ADAPTER *ad)
{
	UINT16 value;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	RT28xx_EEPROM_READ16(ad, RF_2G_RX_HIGH_GAIN, value);
	if ((value & 0xff00) == 0x0000 || ((value & 0xff00) == 0xff00)) {
		cap->rf0_2g_rx_high_gain = 0;
		cap->rf1_2g_rx_high_gain = 0;
	} else {
		if (value & RF0_2G_RX_HIGH_GAIN_SIGN)
			cap->rf0_2g_rx_high_gain = ((value & RF0_2G_RX_HIGH_GAIN_MASK) >> 8);
		else
			cap->rf0_2g_rx_high_gain = -((value & RF0_2G_RX_HIGH_GAIN_MASK) >> 8);

		if (value & RF1_2G_RX_HIGH_GAIN_SIGN)
			cap->rf1_2g_rx_high_gain = ((value & RF1_2G_RX_HIGH_GAIN_MASK) >> 12);
		else
			cap->rf1_2g_rx_high_gain = -((value & RF1_2G_RX_HIGH_GAIN_MASK) >> 12);
	}

	RT28xx_EEPROM_READ16(ad, RF_5G_GRP0_1_RX_HIGH_GAIN, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->rf0_5g_grp0_rx_high_gain = 0;
		cap->rf1_5g_grp0_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP0_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp0_rx_high_gain = (value & RF0_5G_GRP0_RX_HIGH_GAIN_MASK);
		else
			cap->rf0_5g_grp0_rx_high_gain = -(value & RF0_5G_GRP0_RX_HIGH_GAIN_MASK);

		if (value & RF1_5G_GRP0_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp0_rx_high_gain =
			    ((value & RF1_5G_GRP0_RX_HIGH_GAIN_MASK) >> 4);
		else
			cap->rf1_5g_grp0_rx_high_gain =
			    -((value & RF1_5G_GRP0_RX_HIGH_GAIN_MASK) >> 4);
	}

	if ((value & 0xff00) == 0x0000 || ((value & 0xff00) == 0xff00)) {
		cap->rf0_5g_grp1_rx_high_gain = 0;
		cap->rf1_5g_grp1_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP1_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp1_rx_high_gain =
			    ((value & RF0_5G_GRP1_RX_HIGH_GAIN_MASK) >> 8);
		else
			cap->rf0_5g_grp1_rx_high_gain =
			    -((value & RF0_5G_GRP1_RX_HIGH_GAIN_MASK) >> 8);

		if (value & RF1_5G_GRP1_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp1_rx_high_gain =
			    ((value & RF1_5G_GRP1_RX_HIGH_GAIN_MASK) >> 12);
		else
			cap->rf1_5g_grp1_rx_high_gain =
			    -((value & RF1_5G_GRP1_RX_HIGH_GAIN_MASK) >> 12);

	}

	RT28xx_EEPROM_READ16(ad, RF_5G_GRP2_3_RX_HIGH_GAIN, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->rf0_5g_grp2_rx_high_gain = 0;
		cap->rf1_5g_grp2_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP2_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp2_rx_high_gain = (value & RF0_5G_GRP2_RX_HIGH_GAIN_MASK);
		else
			cap->rf0_5g_grp2_rx_high_gain = -(value & RF0_5G_GRP2_RX_HIGH_GAIN_MASK);

		if (value & RF1_5G_GRP2_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp2_rx_high_gain =
			    ((value & RF1_5G_GRP2_RX_HIGH_GAIN_MASK) >> 4);
		else
			cap->rf1_5g_grp2_rx_high_gain =
			    -((value & RF1_5G_GRP2_RX_HIGH_GAIN_MASK) >> 4);
	}

	if ((value & 0xff00) == 0x0000 || ((value & 0xff00) == 0xff00)) {
		cap->rf0_5g_grp3_rx_high_gain = 0;
		cap->rf1_5g_grp3_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP3_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp3_rx_high_gain =
			    ((value & RF0_5G_GRP3_RX_HIGH_GAIN_MASK) >> 8);
		else
			cap->rf0_5g_grp3_rx_high_gain =
			    -((value & RF0_5G_GRP3_RX_HIGH_GAIN_MASK) >> 8);

		if (value & RF1_5G_GRP3_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp3_rx_high_gain =
			    ((value & RF1_5G_GRP3_RX_HIGH_GAIN_MASK) >> 12);
		else
			cap->rf1_5g_grp3_rx_high_gain =
			    -((value & RF1_5G_GRP3_RX_HIGH_GAIN_MASK) >> 12);
	}

	RT28xx_EEPROM_READ16(ad, RF_5G_GRP4_5_RX_HIGH_GAIN, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->rf0_5g_grp4_rx_high_gain = 0;
		cap->rf1_5g_grp4_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP4_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp4_rx_high_gain = (value & RF0_5G_GRP4_RX_HIGH_GAIN_MASK);
		else
			cap->rf0_5g_grp4_rx_high_gain = -(value & RF0_5G_GRP4_RX_HIGH_GAIN_MASK);

		if (value & RF1_5G_GRP4_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp4_rx_high_gain =
			    ((value & RF1_5G_GRP4_RX_HIGH_GAIN_MASK) >> 4);
		else
			cap->rf1_5g_grp4_rx_high_gain =
			    -((value & RF1_5G_GRP4_RX_HIGH_GAIN_MASK) >> 4);
	}

	if ((value & 0xff00) == 0x0000 || ((value & 0xff00) == 0xff00)) {
		cap->rf0_5g_grp5_rx_high_gain = 0;
		cap->rf1_5g_grp5_rx_high_gain = 0;
	} else {
		if (value & RF0_5G_GRP5_RX_HIGH_GAIN_SIGN)
			cap->rf0_5g_grp5_rx_high_gain =
			    ((value & RF0_5G_GRP5_RX_HIGH_GAIN_MASK) >> 8);
		else
			cap->rf0_5g_grp5_rx_high_gain =
			    -((value & RF0_5G_GRP5_RX_HIGH_GAIN_MASK) >> 8);

		if (value & RF1_5G_GRP5_RX_HIGH_GAIN_SIGN)
			cap->rf1_5g_grp5_rx_high_gain =
			    ((value & RF1_5G_GRP5_RX_HIGH_GAIN_MASK) >> 12);
		else
			cap->rf1_5g_grp5_rx_high_gain =
			    -((value & RF1_5G_GRP5_RX_HIGH_GAIN_MASK) >> 12);
	}
	return 0;
}
#endif /* DYNAMIC_VGA_SUPPORT */

static int mt76x2_get_tx_pwr_info(RTMP_ADAPTER *ad)
{
	u16 value;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	RT28xx_EEPROM_READ16(ad, G_BAND_20_40_BW_PWR_DELTA, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->delta_tx_pwr_bw40_g_band = 0;
	} else {
		if (value & G_BAND_20_40_BW_PWR_DELTA_EN) {
			if (value & G_BAND_20_40_BW_PWR_DELTA_SIGN)
				cap->delta_tx_pwr_bw40_g_band =
				    (value & G_BAND_20_40_BW_PWR_DELTA_MASK);
			else
				cap->delta_tx_pwr_bw40_g_band =
				    -(value & G_BAND_20_40_BW_PWR_DELTA_MASK);
		} else {
			cap->delta_tx_pwr_bw40_g_band = 0;
		}
	}

	if (((value & 0xff00) == 0x0000) || ((value & 0xff00) == 0xff00)) {
		cap->delta_tx_pwr_bw40_a_band = 0;
	} else {
		if (value & A_BAND_20_40_BW_PWR_DELTA_EN) {
			if (value & A_BAND_20_40_BW_PWR_DELTA_SIGN)
				cap->delta_tx_pwr_bw40_a_band =
				    ((value & A_BAND_20_40_BW_PWR_DELTA_MASK) >> 8);
			else
				cap->delta_tx_pwr_bw40_a_band =
				    -((value & A_BAND_20_40_BW_PWR_DELTA_MASK) >> 8);
		} else {
			cap->delta_tx_pwr_bw40_a_band = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, A_BAND_20_80_BW_PWR_DELTA, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->delta_tx_pwr_bw80 = 0;
	} else {
		if (value & A_BAND_20_80_BW_PWR_DELTA_EN) {
			if (value & A_BAND_20_80_BW_PWR_DELTA_SIGN)
				cap->delta_tx_pwr_bw80 = (value & A_BAND_20_80_BW_PWR_DELTA_MASK);
			else
				cap->delta_tx_pwr_bw80 = -(value & A_BAND_20_80_BW_PWR_DELTA_MASK);
		} else {
			cap->delta_tx_pwr_bw80 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX0_G_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_slope_g_band = TSSI_0_SLOPE_G_BAND_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_g_band = (value & TX0_G_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_offset_g_band = TSSI_0_OFFSET_G_BAND_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_g_band = ((value & TX0_G_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, TX0_G_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_0_target_pwr_g_band = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_g_band = (value & TX0_G_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_g_band[G_BAND_LOW] = 0;
	} else {
		if (value & TX0_G_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & TX0_G_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_LOW] =
				    ((value & TX0_G_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_LOW] =
				    -((value & TX0_G_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_g_band[G_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX0_G_BAND_CHL_PWR_DELTA_MID, value);

	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_g_band[G_BAND_MID] = 0;
	} else {
		if (value & TX0_G_BAND_CHL_PWR_DELTA_MID_EN) {
			if (value & TX0_G_BAND_CHL_PWR_DELTA_MID_SIGN)
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_MID] =
				    (value & TX0_G_BAND_CHL_PWR_DELTA_MID_MASK);
			else
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_MID] =
				    -(value & TX0_G_BAND_CHL_PWR_DELTA_MID_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_g_band[G_BAND_MID] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_g_band[G_BAND_HI] = 0;
	} else {
		if (value & TX0_G_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & TX0_G_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_HI] =
				    ((value & TX0_G_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_g_band[G_BAND_HI] =
				    -((value & TX0_G_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_g_band[G_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX1_G_BAND_TSSI_SLOPE, value);

	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_slope_g_band = TSSI_1_SLOPE_G_BAND_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_g_band = (value & TX1_G_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_offset_g_band = TSSI_1_OFFSET_G_BAND_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_g_band = ((value & TX1_G_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, TX1_G_BAND_TARGET_PWR, value);

	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_1_target_pwr_g_band = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_g_band = (value & TX1_G_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_g_band[G_BAND_LOW] = 0;
	} else {
		if (value & TX1_G_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & TX1_G_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_LOW] =
				    ((value & TX1_G_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_LOW] =
				    -((value & TX1_G_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_g_band[G_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX1_G_BAND_CHL_PWR_DELTA_MID, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_g_band[G_BAND_MID] = 0;
	} else {
		if (value & TX1_G_BAND_CHL_PWR_DELTA_MID_EN) {
			if (value & TX1_G_BAND_CHL_PWR_DELTA_MID_SIGN)
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_MID] =
				    (value & TX1_G_BAND_CHL_PWR_DELTA_MID_MASK);
			else
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_MID] =
				    -(value & TX1_G_BAND_CHL_PWR_DELTA_MID_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_g_band[G_BAND_MID] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_g_band[G_BAND_HI] = 0;
	} else {
		if (value & TX1_G_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & TX1_G_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_HI] =
				    ((value & TX1_G_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_g_band[G_BAND_HI] =
				    -((value & TX1_G_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_g_band[G_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP0_TX0_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_slope_a_band[A_BAND_GRP0_CHL] = TSSI_0_SLOPE_A_BAND_GRP0_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP0_CHL] =
		    (value & GRP0_TX0_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_offset_a_band[A_BAND_GRP0_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP0_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP0_CHL] =
		    ((value & GRP0_TX0_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP0_TX0_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP0_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP0_CHL] =
		    (value & GRP0_TX0_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] =
				    ((value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] =
				    -((value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] =
				    (value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] =
				    -(value & GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_slope_a_band[A_BAND_GRP1_CHL] = TSSI_0_SLOPE_A_BAND_GRP1_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP1_CHL] =
		    ((value & GRP1_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP1_TX0_A_BAND_TSSI_OFFSET, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_offset_a_band[A_BAND_GRP1_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP1_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP1_CHL] =
		    (value & GRP1_TX0_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP1_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP1_CHL] =
		    ((value & GRP1_TX0_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP1_TX0_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN) {
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] =
				    (value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			} else {
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] =
				    -(value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			}
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] =
				    ((value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] =
				    -((value & GRP1_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP0_TX0_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_slope_a_band[A_BAND_GRP2_CHL] = TSSI_0_SLOPE_A_BAND_GRP2_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP2_CHL] =
		    (value & GRP2_TX0_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_offset_a_band[A_BAND_GRP2_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP2_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP2_CHL] =
		    ((value & GRP2_TX0_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP2_TX0_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP2_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP2_CHL] =
		    (value & GRP2_TX0_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] =
				    ((value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] =
				    -((value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] =
				    (value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] =
				    -(value & GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_slope_a_band[A_BAND_GRP3_CHL] = TSSI_0_SLOPE_A_BAND_GRP3_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP3_CHL] =
		    ((value & GRP3_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP3_TX0_A_BAND_TSSI_OFFSET, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_offset_a_band[A_BAND_GRP3_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP3_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP3_CHL] =
		    (value & GRP3_TX0_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP3_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP3_CHL] =
		    ((value & GRP3_TX0_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] =
				    (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] =
				    -(value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] =
				    ((value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] =
				    -((value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP4_TX0_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_slope_a_band[A_BAND_GRP4_CHL] = TSSI_0_SLOPE_A_BAND_GRP4_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP4_CHL] =
		    (value & GRP4_TX0_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_offset_a_band[A_BAND_GRP4_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP4_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP4_CHL] =
		    ((value & GRP4_TX0_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP4_TX0_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP4_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP4_CHL] =
		    (value & GRP4_TX0_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] =
				    ((value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] =
				    -((value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] =
				    (value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] =
				    -(value & GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_0_slope_a_band[A_BAND_GRP5_CHL] = TSSI_0_SLOPE_A_BAND_GRP5_DEFAULT_VALUE;
	else
		cap->tssi_0_slope_a_band[A_BAND_GRP5_CHL] =
		    ((value & GRP5_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP5_TX0_A_BAND_TSSI_OFFSET, value);

	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_0_offset_a_band[A_BAND_GRP5_CHL] =
		    TSSI_0_OFFSET_A_BAND_GRP5_DEFAULT_VALUE;
	else
		cap->tssi_0_offset_a_band[A_BAND_GRP5_CHL] =
		    (value & GRP5_TX0_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_0_target_pwr_a_band[A_BAND_GRP5_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_0_target_pwr_a_band[A_BAND_GRP5_CHL] =
		    ((value & GRP5_TX0_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP5_TX0_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] =
				    (value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] =
				    -(value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_LOW_MASK);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] =
				    ((value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] =
				    -((value & GRP5_TX0_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = 0;
		}
	}

	/* 5G TX1 chl pwr */
	RT28xx_EEPROM_READ16(ad, GRP0_TX1_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_slope_a_band[A_BAND_GRP0_CHL] = TSSI_1_SLOPE_A_BAND_GRP0_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP0_CHL] =
		    (value & GRP0_TX1_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_offset_a_band[A_BAND_GRP0_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP0_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP0_CHL] =
		    ((value & GRP0_TX1_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP0_TX1_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP0_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP0_CHL] =
		    (value & GRP0_TX1_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] =
				    ((value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] =
				    -((value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] =
				    (value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] =
				    -(value & GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_slope_a_band[A_BAND_GRP1_CHL] = TSSI_1_SLOPE_A_BAND_GRP1_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP1_CHL] =
		    ((value & GRP1_TX1_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP1_TX1_A_BAND_TSSI_OFFSET, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_offset_a_band[A_BAND_GRP1_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP1_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP1_CHL] =
		    (value & GRP1_TX1_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP1_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP1_CHL] =
		    ((value & GRP1_TX1_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP1_TX1_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN) {
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] =
				    (value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			} else {
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] =
				    -(value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			}
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] =
				    ((value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] =
				    -((value & GRP1_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP0_TX1_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_slope_a_band[A_BAND_GRP2_CHL] = TSSI_1_SLOPE_A_BAND_GRP2_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP2_CHL] =
		    (value & GRP2_TX1_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_offset_a_band[A_BAND_GRP2_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP2_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP2_CHL] =
		    ((value & GRP2_TX1_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP2_TX1_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP2_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP2_CHL] =
		    (value & GRP2_TX1_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] =
				    ((value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] =
				    -((value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] =
				    (value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] =
				    -(value & GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_slope_a_band[A_BAND_GRP3_CHL] = TSSI_1_SLOPE_A_BAND_GRP3_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP3_CHL] =
		    ((value & GRP3_TX1_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP3_TX1_A_BAND_TSSI_OFFSET, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_offset_a_band[A_BAND_GRP3_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP3_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP3_CHL] =
		    (value & GRP3_TX1_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP3_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP3_CHL] =
		    ((value & GRP3_TX1_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP3_TX1_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] =
				    (value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] =
				    -(value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP3_TX0_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] =
				    ((value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] =
				    -((value & GRP3_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP4_TX1_A_BAND_TSSI_SLOPE, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_slope_a_band[A_BAND_GRP4_CHL] = TSSI_1_SLOPE_A_BAND_GRP4_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP4_CHL] =
		    (value & GRP4_TX1_A_BAND_TSSI_SLOPE_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_offset_a_band[A_BAND_GRP4_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP4_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP4_CHL] =
		    ((value & GRP4_TX1_A_BAND_TSSI_OFFSET_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP4_TX1_A_BAND_TARGET_PWR, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP4_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP4_CHL] =
		    (value & GRP4_TX1_A_BAND_TARGET_PWR_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] =
				    ((value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] =
				    -((value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] =
				    (value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] =
				    -(value & GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tssi_1_slope_a_band[A_BAND_GRP5_CHL] = TSSI_1_SLOPE_A_BAND_GRP5_DEFAULT_VALUE;
	else
		cap->tssi_1_slope_a_band[A_BAND_GRP5_CHL] =
		    ((value & GRP5_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP5_TX1_A_BAND_TSSI_OFFSET, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff))
		cap->tssi_1_offset_a_band[A_BAND_GRP5_CHL] =
		    TSSI_1_OFFSET_A_BAND_GRP5_DEFAULT_VALUE;
	else
		cap->tssi_1_offset_a_band[A_BAND_GRP5_CHL] =
		    (value & GRP5_TX1_A_BAND_TSSI_OFFSET_MASK);

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00))
		cap->tx_1_target_pwr_a_band[A_BAND_GRP5_CHL] = TX_TARGET_PWR_DEFAULT_VALUE;
	else
		cap->tx_1_target_pwr_a_band[A_BAND_GRP5_CHL] =
		    ((value & GRP5_TX1_A_BAND_TARGET_PWR_MASK) >> 8);

	RT28xx_EEPROM_READ16(ad, GRP5_TX1_A_BAND_CHL_PWR_DELTA_LOW, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = 0;
	} else {
		if (value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_LOW_EN) {
			if (value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_LOW_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] =
				    (value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] =
				    -(value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_LOW_MASK);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = 0;
	} else {
		if (value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_HI_EN) {
			if (value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_HI_SIGN)
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] =
				    ((value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
			else
				cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] =
				    -((value & GRP5_TX1_A_BAND_CHL_PWR_DELTA_HI_MASK) >> 8);
		} else {
			cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = 0;
		}
	}

	/* check tssi if enable */
	RT28xx_EEPROM_READ16(ad, NIC_CONFIGURE_1, value);
	if (value & INTERNAL_TX_ALC_EN)
		cap->tssi_enable = TRUE;
	else
		cap->tssi_enable = FALSE;

	/* check PA type combination */
	RT28xx_EEPROM_READ16(ad, EEPROM_NIC1_OFFSET, value);
	cap->PAType = GET_PA_TYPE(value);
	return 0;
}

static u8 mt76x2_txpwr_chlist[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	36, 38, 40, 44, 46, 48, 52, 54, 56, 60, 62, 64,
	100, 102, 104, 108, 110, 112, 116, 118, 120, 124, 126, 128, 132, 134, 136, 140,
	149, 151, 153, 157, 159, 161, 165, 167, 169, 171, 173,
	42, 58, 106, 122, 155,
};

int mt76x2_read_chl_pwr(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	u32 i, choffset;

	mt76x2_get_tx_pwr_info(ad);

	/* Read Tx power value for all channels */
	/* Value from 1 - 0x7f. Default value is 24. */
	/* Power value : 2.4G 0x00 (0) ~ 0x1F (31) */
	/*             : 5.5G 0xF9 (-7) ~ 0x0F (15) */

	DBGPRINT(RT_DEBUG_TRACE, ("%s()--->\n", __func__));

	for (i = 0; i < sizeof(mt76x2_txpwr_chlist); i++) {
		ad->TxPower[i].Channel = mt76x2_txpwr_chlist[i];
		ad->TxPower[i].Power = TX_TARGET_PWR_DEFAULT_VALUE;
		ad->TxPower[i].Power2 = TX_TARGET_PWR_DEFAULT_VALUE;
	}

	/* 0. 11b/g, ch1 - ch 14, 2SS */
	for (i = 0; i < 14; i++) {
		ad->TxPower[i].Power = cap->tx_0_target_pwr_g_band;
		ad->TxPower[i].Power2 = cap->tx_1_target_pwr_g_band;
	}

	/* 1. U-NII lower/middle band: 36, 38, 40; 44, 46, 48; 52, 54, 56; 60, 62, 64
	 * (including central frequency in BW 40MHz) */
	choffset = 14;
	ASSERT((ad->TxPower[choffset].Channel == 36));

	for (i = 0; i < 39; i++) {
		ad->TxPower[i + choffset].Power =
		    cap->tx_0_target_pwr_a_band[get_chl_grp(ad->TxPower[i + choffset].Channel)];
		ad->TxPower[i + choffset].Power2 =
		    cap->tx_1_target_pwr_a_band[get_chl_grp(ad->TxPower[i + choffset].Channel)];
	}

	choffset = 14 + 12 + 16 + 11;

#ifdef DOT11_VHT_AC
	ASSERT((ad->TxPower[choffset].Channel == 42));

	/* TODO: shiang-6590, fix me for the TxPower setting code here! */
	/* For VHT80MHz, we need assign tx power for central channel 42, 58, 106, 122, and 155 */
	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: Update Tx power control of the central channel (42, 58, 106, 122 and 155) for VHT BW80\n",
		  __func__));

	NdisMoveMemory(&ad->TxPower[53], &ad->TxPower[16], sizeof(CHANNEL_TX_POWER));	/* channel 42 = channel 40 */
	NdisMoveMemory(&ad->TxPower[54], &ad->TxPower[22], sizeof(CHANNEL_TX_POWER));	/* channel 58 = channel 56 */
	NdisMoveMemory(&ad->TxPower[55], &ad->TxPower[28], sizeof(CHANNEL_TX_POWER));	/* channel 106 = channel 104 */
	NdisMoveMemory(&ad->TxPower[56], &ad->TxPower[34], sizeof(CHANNEL_TX_POWER));	/* channel 122 = channel 120 */
	NdisMoveMemory(&ad->TxPower[57], &ad->TxPower[44], sizeof(CHANNEL_TX_POWER));	/* channel 155 = channel 153 */

	ad->TxPower[choffset].Channel = 42;
	ad->TxPower[choffset + 1].Channel = 58;
	ad->TxPower[choffset + 2].Channel = 106;
	ad->TxPower[choffset + 3].Channel = 122;
	ad->TxPower[choffset + 4].Channel = 155;

	choffset = (MAX_NUM_OF_CHANNELS - 1);
#endif /* DOT11_VHT_AC */

	/* 4. Print and Debug */
	for (i = 0; i < choffset; i++) {
		DBGPRINT(RT_DEBUG_INFO,
			 ("E2PROM: TxPower[%03d], Channel=%d, Power[Tx0:%d, Tx1:%d]\n", i,
			  ad->TxPower[i].Channel, ad->TxPower[i].Power, ad->TxPower[i].Power2));
	}

	return TRUE;
}


void mt76x2_get_tx_pwr_per_rate(RTMP_ADAPTER *ad)
{
	u16 value;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	RT28xx_EEPROM_READ16(ad, TX_PWR_CCK_1_2M, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_cck_1_2 = 0;
	} else {
		if (value & TX_PWR_CCK_1_2M_EN) {
			if (value & TX_PWR_CCK_1_2M_SIGN)
				cap->tx_pwr_cck_1_2 = (value & TX_PWR_CCK_1_2M_MASK);
			else
				cap->tx_pwr_cck_1_2 = -(value & TX_PWR_CCK_1_2M_MASK);
		} else {
			cap->tx_pwr_cck_1_2 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_cck_5_11 = 0;
	} else {
		if (value & TX_PWR_CCK_5_11M_EN) {
			if (value & TX_PWR_CCK_5_11M_SIGN)
				cap->tx_pwr_cck_5_11 = ((value & TX_PWR_CCK_5_11M_MASK) >> 8);
			else
				cap->tx_pwr_cck_5_11 = -((value & TX_PWR_CCK_5_11M_MASK) >> 8);
		} else {
			cap->tx_pwr_cck_5_11 = 0;
		}

	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_G_BAND_OFDM_6_9M, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_g_band_ofdm_6_9 = 0;
	} else {
		if (value & TX_PWR_G_BAND_OFDM_6_9M_EN) {
			if (value & TX_PWR_G_BAND_OFDM_6_9M_SIGN)
				cap->tx_pwr_g_band_ofdm_6_9 =
				    (value & TX_PWR_G_BAND_OFDM_6_9M_MASK);
			else
				cap->tx_pwr_g_band_ofdm_6_9 =
				    -(value & TX_PWR_G_BAND_OFDM_6_9M_MASK);
		} else {
			cap->tx_pwr_g_band_ofdm_6_9 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_g_band_ofdm_12_18 = 0;
	} else {
		if (value & TX_PWR_G_BAND_OFDM_12_18M_EN) {
			if (value & TX_PWR_G_BAND_OFDM_12_18M_SIGN)
				cap->tx_pwr_g_band_ofdm_12_18 =
				    ((value & TX_PWR_G_BAND_OFDM_12_18M_MASK) >> 8);
			else
				cap->tx_pwr_g_band_ofdm_12_18 =
				    -((value & TX_PWR_G_BAND_OFDM_12_18M_MASK) >> 8);
		} else {
			cap->tx_pwr_g_band_ofdm_12_18 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_G_BAND_OFDM_24_36M, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_g_band_ofdm_24_36 = 0;
	} else {
		if (value & TX_PWR_G_BAND_OFDM_24_36M_EN) {
			if (value & TX_PWR_G_BAND_OFDM_24_36M_SIGN)
				cap->tx_pwr_g_band_ofdm_24_36 =
				    (value & TX_PWR_G_BAND_OFDM_24_36M_MASK);
			else
				cap->tx_pwr_g_band_ofdm_24_36 =
				    -(value & TX_PWR_G_BAND_OFDM_24_36M_MASK);
		} else {
			cap->tx_pwr_g_band_ofdm_24_36 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_g_band_ofdm_48_54 = 0;
	} else {
		if (value & TX_PWR_G_BAND_OFDM_48_54M_EN) {
			if (value & TX_PWR_G_BAND_OFDM_48_54M_SIGN)
				cap->tx_pwr_g_band_ofdm_48_54 =
				    ((value & TX_PWR_G_BAND_OFDM_48_54M_MASK) >> 8);
			else
				cap->tx_pwr_g_band_ofdm_48_54 =
				    -((value & TX_PWR_G_BAND_OFDM_48_54M_MASK) >> 8);
		} else {
			cap->tx_pwr_g_band_ofdm_48_54 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_HT_MCS_0_1, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_ht_mcs_0_1 = (value & TX_PWR_HT_MCS_0_1_MASK);
	} else {
		if (value & TX_PWR_HT_MCS_0_1_EN) {
			if (value & TX_PWR_HT_MCS_0_1_SIGN)
				cap->tx_pwr_ht_mcs_0_1 = (value & TX_PWR_HT_MCS_0_1_MASK);
			else
				cap->tx_pwr_ht_mcs_0_1 = -(value & TX_PWR_HT_MCS_0_1_MASK);
		} else {
			cap->tx_pwr_ht_mcs_0_1 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_ht_mcs_2_3 = ((value & TX_PWR_HT_MCS_2_3_MASK) >> 8);
	} else {
		if (value & TX_PWR_HT_MCS_2_3_EN) {
			if (value & TX_PWR_HT_MCS_2_3_SIGN)
				cap->tx_pwr_ht_mcs_2_3 = ((value & TX_PWR_HT_MCS_2_3_MASK) >> 8);
			else
				cap->tx_pwr_ht_mcs_2_3 = -((value & TX_PWR_HT_MCS_2_3_MASK) >> 8);
		} else {
			cap->tx_pwr_ht_mcs_2_3 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_HT_MCS_4_5, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_ht_mcs_4_5 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_4_5_EN) {
			if (value & TX_PWR_HT_MCS_4_5_SIGN)
				cap->tx_pwr_ht_mcs_4_5 = (value & TX_PWR_HT_MCS_4_5_MASK);
			else
				cap->tx_pwr_ht_mcs_4_5 = -(value & TX_PWR_HT_MCS_4_5_MASK);
		} else {
			cap->tx_pwr_ht_mcs_4_5 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_ht_mcs_6_7 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_6_7_EN) {
			if (value & TX_PWR_HT_MCS_6_7_SIGN)
				cap->tx_pwr_ht_mcs_6_7 = ((value & TX_PWR_HT_MCS_6_7_MASK) >> 8);
			else
				cap->tx_pwr_ht_mcs_6_7 = -((value & TX_PWR_HT_MCS_6_7_MASK) >> 8);
		} else {
			cap->tx_pwr_ht_mcs_6_7 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_HT_MCS_8_9, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_ht_mcs_8_9 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_8_9_EN) {
			if (value & TX_PWR_HT_MCS_8_9_SIGN)
				cap->tx_pwr_ht_mcs_8_9 = (value & TX_PWR_HT_MCS_8_9_MASK);
			else
				cap->tx_pwr_ht_mcs_8_9 = -(value & TX_PWR_HT_MCS_8_9_MASK);
		} else {
			cap->tx_pwr_ht_mcs_8_9 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_ht_mcs_10_11 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_10_11_EN) {
			if (value & TX_PWR_HT_MCS_10_11_SIGN)
				cap->tx_pwr_ht_mcs_10_11 =
				    ((value & TX_PWR_HT_MCS_10_11_MASK) >> 8);
			else
				cap->tx_pwr_ht_mcs_10_11 =
				    -((value & TX_PWR_HT_MCS_10_11_MASK) >> 8);
		} else {
			cap->tx_pwr_ht_mcs_10_11 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_HT_MCS_12_13, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_ht_mcs_12_13 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_12_13_EN) {
			if (value & TX_PWR_HT_MCS_12_13_SIGN)
				cap->tx_pwr_ht_mcs_12_13 = (value & TX_PWR_HT_MCS_12_13_MASK);
			else
				cap->tx_pwr_ht_mcs_12_13 = -(value & TX_PWR_HT_MCS_12_13_MASK);
		} else {
			cap->tx_pwr_ht_mcs_12_13 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_ht_mcs_14_15 = 0;
	} else {
		if (value & TX_PWR_HT_MCS_14_15_EN) {
			if (value & TX_PWR_HT_MCS_14_15_SIGN)
				cap->tx_pwr_ht_mcs_14_15 =
				    ((value & TX_PWR_HT_MCS_14_15_MASK) >> 8);
			else
				cap->tx_pwr_ht_mcs_14_15 =
				    -((value & TX_PWR_HT_MCS_14_15_MASK) >> 8);
		} else {
			cap->tx_pwr_ht_mcs_14_15 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_A_BAND_OFDM_6_9M, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_a_band_ofdm_6_9 = 0;
	} else {
		if (value & TX_PWR_A_BAND_OFDM_6_9M_EN) {
			if (value & TX_PWR_A_BAND_OFDM_6_9M_SIGN)
				cap->tx_pwr_a_band_ofdm_6_9 =
				    (value & TX_PWR_A_BAND_OFDM_6_9M_MASK);
			else
				cap->tx_pwr_a_band_ofdm_6_9 =
				    -(value & TX_PWR_A_BAND_OFDM_6_9M_MASK);
		} else {
			cap->tx_pwr_a_band_ofdm_6_9 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_a_band_ofdm_12_18 = 0;
	} else {
		if (value & TX_PWR_A_BAND_OFDM_12_18M_EN) {
			if (value & TX_PWR_A_BAND_OFDM_12_18M_SIGN)
				cap->tx_pwr_a_band_ofdm_12_18 =
				    ((value & TX_PWR_A_BAND_OFDM_12_18M_MASK) >> 8);
			else
				cap->tx_pwr_a_band_ofdm_12_18 =
				    -((value & TX_PWR_A_BAND_OFDM_12_18M_MASK) >> 8);
		} else {
			cap->tx_pwr_a_band_ofdm_12_18 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_A_BAND_OFDM_24_36M, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_a_band_ofdm_24_36 = 0;
	} else {
		if (value & TX_PWR_A_BAND_OFDM_24_36M_EN) {
			if (value & TX_PWR_A_BAND_OFDM_24_36M_SIGN)
				cap->tx_pwr_a_band_ofdm_24_36 =
				    (value & TX_PWR_A_BAND_OFDM_24_36M_MASK);
			else
				cap->tx_pwr_a_band_ofdm_24_36 =
				    -(value & TX_PWR_A_BAND_OFDM_24_36M_MASK);
		} else {
			cap->tx_pwr_a_band_ofdm_24_36 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_a_band_ofdm_48_54 = 0;
	} else {
		if (value & TX_PWR_A_BAND_OFDM_48_54M_EN) {
			if (value & TX_PWR_A_BAND_OFDM_48_54M_SIGN)
				cap->tx_pwr_a_band_ofdm_48_54 =
				    ((value & TX_PWR_A_BAND_OFDM_48_54M_MASK) >> 8);
			else
				cap->tx_pwr_a_band_ofdm_48_54 =
				    -((value & TX_PWR_A_BAND_OFDM_48_54M_MASK) >> 8);
		} else {
			cap->tx_pwr_a_band_ofdm_48_54 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_VHT_MCS_0_1, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_vht_mcs_0_1 = 0;
	} else {
		if (value & TX_PWR_VHT_MCS_0_1_EN) {
			if (value & TX_PWR_VHT_MCS_0_1_SIGN)
				cap->tx_pwr_vht_mcs_0_1 = (value & TX_PWR_VHT_MCS_0_1_MASK);
			else
				cap->tx_pwr_vht_mcs_0_1 = -(value & TX_PWR_VHT_MCS_0_1_MASK);
		} else {
			cap->tx_pwr_vht_mcs_0_1 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_vht_mcs_2_3 = 0;
	} else {
		if (value & TX_PWR_VHT_MCS_2_3_EN) {
			if (value & TX_PWR_VHT_MCS_2_3_SIGN)
				cap->tx_pwr_vht_mcs_2_3 = ((value & TX_PWR_VHT_MCS_2_3_MASK) >> 8);
			else
				cap->tx_pwr_vht_mcs_2_3 = -((value & TX_PWR_VHT_MCS_2_3_MASK) >> 8);
		} else {
			cap->tx_pwr_vht_mcs_2_3 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_VHT_MCS_4_5, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_vht_mcs_4_5 = 0;
	} else {
		if (value & TX_PWR_VHT_MCS_4_5_EN) {
			if (value & TX_PWR_VHT_MCS_4_5_SIGN)
				cap->tx_pwr_vht_mcs_4_5 = (value & TX_PWR_VHT_MCS_4_5_MASK);
			else
				cap->tx_pwr_vht_mcs_4_5 = -(value & TX_PWR_VHT_MCS_4_5_MASK);
		} else {
			cap->tx_pwr_vht_mcs_4_5 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_vht_mcs_6_7 = 0;
	} else {
		if (value & TX_PWR_VHT_MCS_6_7_EN) {
			if (value & TX_PWR_VHT_MCS_6_7_SIGN)
				cap->tx_pwr_vht_mcs_6_7 = ((value & TX_PWR_VHT_MCS_6_7_MASK) >> 8);
			else
				cap->tx_pwr_vht_mcs_6_7 = -((value & TX_PWR_VHT_MCS_6_7_MASK) >> 8);
		} else {
			cap->tx_pwr_vht_mcs_6_7 = 0;
		}
	}

	RT28xx_EEPROM_READ16(ad, TX_PWR_5G_VHT_MCS_8_9, value);
	if (((value & 0xff) == 0x00) || ((value & 0xff) == 0xff)) {
		cap->tx_pwr_5g_vht_mcs_8_9 = 0;
	} else {
		if (value & TX_PWR_5G_VHT_MCS_8_9_EN) {
			if (value & TX_PWR_5G_VHT_MCS_8_9_SIGN)
				cap->tx_pwr_5g_vht_mcs_8_9 = (value & TX_PWR_5G_VHT_MCS_8_9_MASK);
			else
				cap->tx_pwr_5g_vht_mcs_8_9 = -(value & TX_PWR_5G_VHT_MCS_8_9_MASK);
		} else {
			cap->tx_pwr_5g_vht_mcs_8_9 = 0;
		}
	}

	if (((value & 0xff00) == 0x00) || ((value & 0xff00) == 0xff00)) {
		cap->tx_pwr_2g_vht_mcs_8_9 = 0;
	} else {
		if (value & TX_PWR_2G_VHT_MCS_8_9_EN) {
			if (value & TX_PWR_2G_VHT_MCS_8_9_SIGN)
				cap->tx_pwr_2g_vht_mcs_8_9 =
				    ((value & TX_PWR_2G_VHT_MCS_8_9_MASK) >> 8);
			else
				cap->tx_pwr_2g_vht_mcs_8_9 =
				    -((value & TX_PWR_2G_VHT_MCS_8_9_MASK) >> 8);
		} else {
			cap->tx_pwr_2g_vht_mcs_8_9 = 0;
		}
	}
}

static void mt76x2_show_pwr_info(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	UINT32 value;

	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("channel info related to power\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));

	if (ad->LatchRfRegs.Channel < 14) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("central channel = %d, low_mid_hi = %d\n", ad->LatchRfRegs.Channel,
			  get_low_mid_hi_index(ad->LatchRfRegs.Channel)));

	} else {
		DBGPRINT(RT_DEBUG_OFF, ("central channel = %d, group = %d, low_mid_hi = %d\n",
					ad->LatchRfRegs.Channel,
					get_chl_grp(ad->LatchRfRegs.Channel),
					get_low_mid_hi_index(ad->LatchRfRegs.Channel)));
	}

	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("channel power(unit: 0.5dbm)\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("tx_0_target_pwr_g_band = 0x%x\n", cap->tx_0_target_pwr_g_band));
	DBGPRINT(RT_DEBUG_OFF, ("tx_1_target_pwr_g_band = 0x%x\n", cap->tx_1_target_pwr_g_band));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_target_pwr_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tx_0_target_pwr_a_band[A_BAND_GRP5_CHL]));

	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_target_pwr_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tx_1_target_pwr_a_band[A_BAND_GRP5_CHL]));

	/* channel power delta */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("channel power deltai(unit: 0.5db)\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_g_band[G_BAND_LOW] = 0x%x\n",
		  cap->tx_0_chl_pwr_delta_g_band[G_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_g_band[G_BAND_MID] = 0x%x\n",
		  cap->tx_0_chl_pwr_delta_g_band[G_BAND_MID]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_g_band[G_BAND_HI] = 0x%x\n",
		  cap->tx_0_chl_pwr_delta_g_band[G_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_g_band[G_BAND_LOW] = 0x%x\n",
		  cap->tx_1_chl_pwr_delta_g_band[G_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_g_band[G_BAND_MID] = 0x%x\n",
		  cap->tx_1_chl_pwr_delta_g_band[G_BAND_MID]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_g_band[G_BAND_HI] = 0x%x\n",
		  cap->tx_1_chl_pwr_delta_g_band[G_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = %d\n",
		  cap->tx_0_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI]));

	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP0_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP1_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP2_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP3_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP4_CHL][A_BAND_HI]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW] = %d\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_LOW]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI] = %d\n\n",
		  cap->tx_1_chl_pwr_delta_a_band[A_BAND_GRP5_CHL][A_BAND_HI]));

	/* bw power delta */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("bw power delta(unit: 0.5db)\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("delta_tx_pwr_bw40_g_band = %d\n", cap->delta_tx_pwr_bw40_g_band));
	DBGPRINT(RT_DEBUG_OFF, ("delta_tx_pwr_bw40_a_band = %d\n", cap->delta_tx_pwr_bw40_a_band));
	DBGPRINT(RT_DEBUG_OFF, ("delta_tx_pwr_bw80 = %d\n", cap->delta_tx_pwr_bw80));

	/* 54Mbps target power in MAC 0x13b0[5:0] and 0x13b0[13:8] */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF,
		 ("54Mbps target power(unit: 0.5dbm) MAC 0x13b0[5:0] and 0x13b0[13:8]\n");
		 DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
		 RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
		 value = value & TX_ALC_CFG_0_CH_INT_0_MASK;
		 DBGPRINT(RT_DEBUG_OFF, ("tx_0_pwr(0x13b0[5:0])= 0x%x\n", value));
		 RTMP_IO_READ32(ad, TX_ALC_CFG_0, &value);
		 value = (value & TX_ALC_CFG_0_CH_INT_1_MASK) >> 8);
	DBGPRINT(RT_DEBUG_OFF, ("tx_1_pwr(0x13b0[13:8]) = 0x%x\n", value));

	/* per-rate power delta */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("per-rate power delta\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_cck_1_2 = %d\n", cap->tx_pwr_cck_1_2));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_cck_5_11 = %d\n", cap->tx_pwr_cck_5_11));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_g_band_ofdm_6_9 = %d\n", cap->tx_pwr_g_band_ofdm_6_9));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_g_band_ofdm_12_18 = %d\n", cap->tx_pwr_g_band_ofdm_12_18));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_g_band_ofdm_24_36 = %d\n", cap->tx_pwr_g_band_ofdm_24_36));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_g_band_ofdm_48_54 = %d\n", cap->tx_pwr_g_band_ofdm_48_54));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_0_1 = %d\n", cap->tx_pwr_ht_mcs_0_1));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_2_3 = %d\n", cap->tx_pwr_ht_mcs_2_3));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_4_5 = %d\n", cap->tx_pwr_ht_mcs_4_5));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_6_7 = %d\n", cap->tx_pwr_ht_mcs_6_7));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_8_9 = %d\n", cap->tx_pwr_ht_mcs_8_9));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_10_11 = %d\n", cap->tx_pwr_ht_mcs_10_11));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_12_13 = %d\n", cap->tx_pwr_ht_mcs_12_13));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_ht_mcs_14_15 = %d\n", cap->tx_pwr_ht_mcs_14_15));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_a_band_ofdm_6_9 = %d\n", cap->tx_pwr_a_band_ofdm_6_9));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_a_band_ofdm_12_18 = %d\n", cap->tx_pwr_a_band_ofdm_12_18));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_a_band_ofdm_24_36 = %d\n", cap->tx_pwr_a_band_ofdm_24_36));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_a_band_ofdm_48_54 = %d\n", cap->tx_pwr_a_band_ofdm_48_54));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_vht_mcs_0_1 = %d\n", cap->tx_pwr_vht_mcs_0_1));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_vht_mcs_2_3 = %d\n", cap->tx_pwr_vht_mcs_2_3));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_vht_mcs_4_5 = %d\n", cap->tx_pwr_vht_mcs_4_5));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_vht_mcs_6_7 = %d\n", cap->tx_pwr_vht_mcs_6_7));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_5g_vht_mcs_8_9 = %d\n", cap->tx_pwr_5g_vht_mcs_8_9));
	DBGPRINT(RT_DEBUG_OFF, ("tx_pwr_2g_vht_mcs_8_9 = %d\n", cap->tx_pwr_2g_vht_mcs_8_9));

	/* per rate delta in mac 0x1314 ~ 0x13DC */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("per-rate power delta in MAC 0x1314 ~ 0x13DC\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	RTMP_IO_READ32(ad, TX_PWR_CFG_0, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_0 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_1, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_1 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_2, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_2 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_3, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_3 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_4, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_4 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_7, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_7 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_8, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_8 = 0x%x\n", value));
	RTMP_IO_READ32(ad, TX_PWR_CFG_9, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX_PWR_CFG_9 = 0x%x\n", value));

	/* TSSI info */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("TSSI info\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("PA type = %d\n", cap->PAType));
	DBGPRINT(RT_DEBUG_OFF, ("TSSI enable = %d\n", cap->tssi_enable));
	DBGPRINT(RT_DEBUG_OFF, ("tssi_0_slope_g_band = 0x%x\n", cap->tssi_0_slope_g_band));
	DBGPRINT(RT_DEBUG_OFF, ("tssi_1_slope_g_band = 0x%x\n", cap->tssi_1_slope_g_band));
	DBGPRINT(RT_DEBUG_OFF, ("tssi_0_offset_g_band = 0x%x\n", cap->tssi_0_offset_g_band));
	DBGPRINT(RT_DEBUG_OFF, ("tssi_1_offset_g_band = 0x%x\n", cap->tssi_1_offset_g_band));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_slope_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tssi_0_slope_a_band[A_BAND_GRP5_CHL]));

	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_0_offset_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tssi_0_offset_a_band[A_BAND_GRP5_CHL]));

	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_slope_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tssi_1_slope_a_band[A_BAND_GRP5_CHL]));

	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP0_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP0_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP1_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP1_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP2_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP2_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP3_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP3_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP4_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP4_CHL]));
	DBGPRINT(RT_DEBUG_OFF,
		 ("cap->tssi_1_offset_a_band[A_BAND_GRP5_CHL] = 0x%x\n",
		  cap->tssi_1_offset_a_band[A_BAND_GRP5_CHL]));

	/* TSSI compensation value */
	DBGPRINT(RT_DEBUG_OFF, ("\n===================================\n"));
	DBGPRINT(RT_DEBUG_OFF, ("TSSI compensation value(unit: 0.5db) in mac 0x13B4 and 0x13A8\n"));
	DBGPRINT(RT_DEBUG_OFF, ("===================================\n"));
	RTMP_IO_READ32(ad, TX_ALC_CFG_1, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX0 power compensation = 0x%x\n", value & 0x3f));
	RTMP_IO_READ32(ad, TX_ALC_CFG_2, &value);
	DBGPRINT(RT_DEBUG_OFF, ("TX1 power compensation = 0x%x\n", value & 0x3f));
}

static void mt76x2_antenna_default_reset(struct _RTMP_ADAPTER *pAd, EEPROM_ANTENNA_STRUC *pAntenna)
{
	pAntenna->word = 0;
	pAntenna->field.RfIcType = 0;	/* RFIC_7662 overflow */
	pAntenna->field.TxPath = 2;
	pAntenna->field.RxPath = 2;
}

void percentage_delta_pwr(RTMP_ADAPTER *ad)
{
	CHAR mac_drop_pwr = 0, tx_alc_ch_init_0 = 0, tx_alc_ch_init_1 = 0;
	UINT32 mac_val = 0;

	/*
	   Calculate delta power based on the percentage specified from UI.
	   EEPROM setting is calibrated for maximum Tx power (i.e. 100%).
	   Here lower Tx power according to the percentage specified from UI.
	 */

	if (ad->CommonCfg.TxPowerPercentage > 90) {
		/* 91 ~ 100% & auto, treat as 100% in terms of mW */
		;
	} else if (ad->CommonCfg.TxPowerPercentage > 60) {
		/* 61 ~ 90%, treat as 75% in terms of mW */
		mac_drop_pwr -= 1;
	} else if (ad->CommonCfg.TxPowerPercentage > 30) {
		/* 31 ~ 60%, treat as 50% in terms of mW */
		mac_drop_pwr -= 3;
	} else if (ad->CommonCfg.TxPowerPercentage > 15) {
		/* 16 ~ 30%, treat as 25% in terms of mW */
		mac_drop_pwr -= 6;
	} else if (ad->CommonCfg.TxPowerPercentage > 9) {
		/* 10 ~ 15%, treat as 12.5% in terms of mW */
		mac_drop_pwr -= 9;
	} else {
		/* 0 ~ 9 %, treat as MIN(~3%) in terms of mW */
		mac_drop_pwr -= 12;
	}

	RTMP_IO_READ32(ad, TX_ALC_CFG_0, &mac_val);
	tx_alc_ch_init_0 = (mac_val & 0x3F) + mac_drop_pwr * 2;
	if (tx_alc_ch_init_0 <= 0)
		tx_alc_ch_init_0 = 0;
	tx_alc_ch_init_1 = ((mac_val & 0x3F00) >> 8) + mac_drop_pwr * 2;
	if (tx_alc_ch_init_1 <= 0)
		tx_alc_ch_init_1 = 0;
	DBGPRINT(RT_DEBUG_INFO,
		 ("%s::<Before> TX_ALC_CFG_0=0x%0x, tx_alc_ch_init_0=0x%0x, tx_alc_ch_init_1=0x%0x\n",
		  __func__, mac_val, tx_alc_ch_init_0, tx_alc_ch_init_1));

	mac_val = mac_val & (~TX_ALC_CFG_0_CH_INT_0_MASK);
	mac_val |= TX_ALC_CFG_0_CH_INT_0(tx_alc_ch_init_0);
	mac_val = mac_val & (~TX_ALC_CFG_0_CH_INT_1_MASK);
	mac_val |= TX_ALC_CFG_0_CH_INT_1(tx_alc_ch_init_1);
	RTMP_IO_WRITE32(ad, TX_ALC_CFG_0, mac_val);

	DBGPRINT(RT_DEBUG_INFO, ("%s::<After> TX_ALC_CFG_0=0x%0x\n", __func__, mac_val));
}

void mt76x2_get_current_temp(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	INT32 temp_val = 0;
	UINT32 Value = 0;

	RTMP_IO_READ32(ad, WLAN_FUN_CTRL, &Value);
	if ((Value & WLAN_FUN_CTRL_THERM_CKEN)
			!= WLAN_FUN_CTRL_THERM_CKEN) {
		DBGPRINT(RT_DEBUG_WARN, (
				"%s: WLAN_FUN_CTRL = 0x%x thermal clock is not enabled, don't read temp\n",
				__func__, Value));
		return;
	}
	read_reg(ad, 0x40, 0xD000, &temp_val);
	temp_val &= 0x7F;

	if (pChipCap->temp_25_ref == 0)
		pChipCap->current_temp = 25;
	else if (temp_val > pChipCap->temp_25_ref)
		pChipCap->current_temp = ((temp_val - pChipCap->temp_25_ref) * 18 / 10) + 25;	/* 1.789 */
	else if (temp_val < pChipCap->temp_25_ref)
		pChipCap->current_temp = 25 - ((pChipCap->temp_25_ref - temp_val) * 18 / 10);
	else
		pChipCap->current_temp = 25;

	DBGPRINT(RT_DEBUG_INFO, ("%s::read_temp=%d (0x%x), current_temp=%d (0x%x)\n",
				 __func__, temp_val, temp_val, pChipCap->current_temp,
				 pChipCap->current_temp));
}

void mt76x2_read_temp_info_from_eeprom(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	BOOLEAN is_temp_tx_alc = FALSE;
	USHORT e2p_value = 0;

	RT28xx_EEPROM_READ16(ad, 0x36, e2p_value);
	if ((e2p_value & 0x2) == 0x2)
		is_temp_tx_alc = TRUE;
	else
		is_temp_tx_alc = FALSE;

	RT28xx_EEPROM_READ16(ad, 0x54, e2p_value);
	pChipCap->temp_25_ref = (e2p_value & 0x7F00) >> 8;
	if (((e2p_value & 0x8000) == 0x8000) && is_temp_tx_alc)
		pChipCap->temp_tx_alc_enable = TRUE;
	else
		pChipCap->temp_tx_alc_enable = FALSE;

	DBGPRINT(RT_DEBUG_ERROR, ("%s:: is_temp_tx_alc=%d, temp_tx_alc_enable=%d\n",
				  __func__, is_temp_tx_alc, pChipCap->temp_tx_alc_enable));
}

#ifdef RTMP_TEMPERATURE_TX_ALC
void mt76x2_read_tx_alc_info_from_eeprom(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	BOOLEAN is_ePA_mode = FALSE;
	USHORT e2p_value = 0, ePA_type = 3;

	RT28xx_EEPROM_READ16(ad, 0x34, e2p_value);
	/*
	   00: 2.4G+5G external PA
	   01: 5G external PA
	   10: 2.4G external PA
	   11: disable
	 */
	if ((e2p_value & 0x300) != 0x300) {
		is_ePA_mode = TRUE;
		ePA_type = ((e2p_value & 0x300) >> 8);
	} else {
		is_ePA_mode = FALSE;
		ePA_type = 3;
	}

	if ((ePA_type == 0) || (ePA_type == 1)) {
		/* get 5G ePA NT to high and low temperature slope, the values are all positive numbers */
		RT28xx_EEPROM_READ16(ad, 0xF2, e2p_value);
		pChipCap->high_temp_slope_a_band = e2p_value & 0xFF;
		pChipCap->low_temp_slope_a_band = (e2p_value & 0xFF00) >> 8;
		if ((e2p_value == 0xFFFF) || (e2p_value == 0))
			pChipCap->temp_tx_alc_enable = FALSE;

		/* get 5G temperature compensation upper and lower limit, the values are all positive numbers */
		RT28xx_EEPROM_READ16(ad, 0x54, e2p_value);
		pChipCap->tc_lower_bound_a_band = 0 - (e2p_value & 0xF);
		pChipCap->tc_upper_bound_a_band = (e2p_value & 0xF0) >> 4;
		if (((e2p_value & 0xFF) == 0xFF) || ((e2p_value & 0xFF) == 0))
			pChipCap->temp_tx_alc_enable = FALSE;
	}

	if ((ePA_type == 0) || (ePA_type == 2)) {
		/* get 2G ePA NT to high and low temperature slope, the values are all positive numbers */
		RT28xx_EEPROM_READ16(ad, 0xF4, e2p_value);
		pChipCap->high_temp_slope_g_band = e2p_value & 0xFF;
		pChipCap->low_temp_slope_g_band = (e2p_value & 0xFF00) >> 8;
		if ((e2p_value == 0xFFFF) || (e2p_value == 0))
			pChipCap->temp_tx_alc_enable = FALSE;

		/* get 2G temperature compensation upper and lower limit, the values are all positive numbers */
		RT28xx_EEPROM_READ16(ad, 0x52, e2p_value);
		pChipCap->tc_lower_bound_g_band = 0 - ((e2p_value & 0x0F00) >> 8);
		pChipCap->tc_upper_bound_g_band = (e2p_value & 0xF000) >> 12;
		if (((e2p_value & 0xFF00) == 0xFF00) || ((e2p_value & 0xFF00) == 0))
			pChipCap->temp_tx_alc_enable = FALSE;
	}

	DBGPRINT(RT_DEBUG_ERROR, ("%s:: is_ePA_mode=%d, ePA_type=%d\n",
				  __func__, is_ePA_mode, ePA_type));
	DBGPRINT(RT_DEBUG_ERROR, ("%s:: [5G band] high_temp_slope=%d, low_temp_slope=%d\n",
				  __func__, pChipCap->high_temp_slope_a_band,
				  pChipCap->low_temp_slope_a_band));
	DBGPRINT(RT_DEBUG_ERROR,
		 ("%s:: [2G band] high_temp_slope=%d, low_temp_slope=%d\n", __func__,
		  pChipCap->high_temp_slope_g_band, pChipCap->low_temp_slope_g_band));
	DBGPRINT(RT_DEBUG_ERROR,
		 ("%s:: [5G band] tc_lower_bound=%d, tc_upper_bound=%d\n", __func__,
		  pChipCap->tc_lower_bound_a_band, pChipCap->tc_upper_bound_a_band));
	DBGPRINT(RT_DEBUG_ERROR,
		 ("%s:: [2G band] tc_lower_bound=%d, tc_upper_bound=%d\n", __func__,
		  pChipCap->tc_lower_bound_g_band, pChipCap->tc_upper_bound_g_band));
}

void mt76x2_temp_tx_alc(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	INT32 temp_diff = 0, dB_diff = 0, tx0_temp_comp = 0, tx1_temp_comp = 0;
	UCHAR tc_init_val = 0;

#ifdef SINGLE_SKU_V2
	tc_init_val = ad->tc_init_val;

	if (ad->sku_init_done == FALSE)
		return;
#endif /* SINGLE_SKU_V2 */

	if (pChipCap->temp_tx_alc_enable) {
		temp_diff = pChipCap->current_temp - 25;

		if (ad->CommonCfg.Channel > 14) {
			if (temp_diff > 0)
				dB_diff = (temp_diff / pChipCap->high_temp_slope_a_band);
			else if (temp_diff < 0)
				dB_diff = 0 - ((0 - temp_diff) / pChipCap->low_temp_slope_a_band);
			else
				dB_diff = 0;

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s::[5G] temp_diff=%d (0x%x), dB_diff=%d (0x%x)\n", __func__,
				  temp_diff, temp_diff, dB_diff, dB_diff));

			/* temperature compensation boundary check and limit */
			dB_diff =
			    (dB_diff >
			     pChipCap->tc_upper_bound_a_band) ? pChipCap->
			    tc_upper_bound_a_band : dB_diff;
			dB_diff =
			    (dB_diff <
			     pChipCap->tc_lower_bound_a_band) ? pChipCap->
			    tc_lower_bound_a_band : dB_diff;
		} else {
			if (temp_diff > 0)
				dB_diff = (temp_diff / pChipCap->high_temp_slope_g_band);
			else if (temp_diff < 0)
				dB_diff = 0 - ((0 - temp_diff) / pChipCap->low_temp_slope_g_band);
			else
				dB_diff = 0;

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s::[2G] temp_diff=%d (0x%x), dB_diff=%d (0x%x)\n", __func__,
				  temp_diff, temp_diff, dB_diff, dB_diff));

			/* temperature compensation boundary check and limit */
			dB_diff =
			    (dB_diff >
			     pChipCap->tc_upper_bound_g_band) ? pChipCap->
			    tc_upper_bound_g_band : dB_diff;
			dB_diff =
			    (dB_diff <
			     pChipCap->tc_lower_bound_g_band) ? pChipCap->
			    tc_lower_bound_g_band : dB_diff;
		}

		DBGPRINT(RT_DEBUG_TRACE, ("%s::temp_diff=%d (0x%x), dB_diff=%d (0x%x)\n",
					  __func__, temp_diff, temp_diff, dB_diff, dB_diff));

		RTMP_IO_READ32(ad, TX_ALC_CFG_1, &tx0_temp_comp);
		tx0_temp_comp &= ~TX_ALC_CFG_1_TX0_TEMP_COMP_MASK;
		tx0_temp_comp |= ((tc_init_val + dB_diff * 2) & TX_ALC_CFG_1_TX0_TEMP_COMP_MASK);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_1, tx0_temp_comp);
		DBGPRINT(RT_DEBUG_TRACE, ("%s::Tx0 power compensation = 0x%x\n",
					  __func__, tx0_temp_comp & 0x3f));

		RTMP_IO_READ32(ad, TX_ALC_CFG_2, &tx1_temp_comp);
		tx1_temp_comp &= ~TX_ALC_CFG_2_TX1_TEMP_COMP_MASK;
		tx1_temp_comp |= ((tc_init_val + dB_diff * 2) & TX_ALC_CFG_2_TX1_TEMP_COMP_MASK);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_2, tx1_temp_comp);
		DBGPRINT(RT_DEBUG_TRACE, ("%s::Tx1 power compensation = 0x%x\n",
					  __func__, tx1_temp_comp & 0x3f));
	}
}
#endif /* RTMP_TEMPERATURE_TX_ALC */

#ifdef SINGLE_SKU_V2
void mt76x2_single_sku(RTMP_ADAPTER *ad, u8 channel)
{
	CHAR sku_base_pwr = 0, ch_pwr_adj = 0;
	CHAR delta_power = 0;
	UINT32 reg_val = 0;

	if (channel > 14) {
		if ((ad->chipCap.temp_tx_alc_enable == FALSE) && (ad->chipCap.tssi_enable == TRUE)) {
			if ((channel >= 36) && (channel <= 48))
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP1_CHL];
			else if ((channel >= 52) && (channel <= 64))
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP2_CHL];
			else if ((channel >= 98) && (channel <= 114))
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP3_CHL];
			else if ((channel >= 116) && (channel <= 144))
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP4_CHL];
			else if ((channel >= 149) && (channel <= 165))
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP5_CHL];
			else
				ad->DefaultTargetPwr =
				    ad->chipCap.tssi_54m_target_pwr_a_band[A_BAND_GRP0_CHL];
		} else
			ad->DefaultTargetPwr = ad->chipCap.tssi_off_54m_target_pwr_a_band;
#ifdef DOT11_VHT_AC
		if (ad->CommonCfg.BBPCurrentBW == BW_80)
			delta_power = ad->chipCap.delta_tx_pwr_bw80;
		else
#endif /* DOT11_VHT_AC */
			delta_power = ad->chipCap.delta_tx_pwr_bw40_a_band;
	} else {
		if ((ad->chipCap.temp_tx_alc_enable == FALSE) && (ad->chipCap.tssi_enable == TRUE))
			ad->DefaultTargetPwr = ad->chipCap.tssi_54m_target_pwr_g_band;
		else
			ad->DefaultTargetPwr = ad->chipCap.tssi_off_54m_target_pwr_g_band;
		delta_power = ad->chipCap.delta_tx_pwr_bw40_g_band;
	}

	if ((ad->DefaultTargetPwr == 0x00) || (ad->DefaultTargetPwr == 0xFF)) {
		ad->DefaultTargetPwr = 0x20;
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s fail to get the target power, use default %d\n",
			  __func__, ad->DefaultTargetPwr));
	} else {
		DBGPRINT(RT_DEBUG_ERROR, ("%s DefaultTargetPwr %d ch %d\n",
			 __func__, ad->DefaultTargetPwr, channel));
	}

	/*
	   EEPROM 0x50 - Power delta for 2G 20/40 BW
	   EEPROM 0x51 - Power delta for 5G 20/40 BW
	   EEPROM 0x52 - Power delta for 5G 20/80 BW
	   Bit<7>: Enable/disable power delta of this BW
	   Bit<6>: 0: decrease power, 1: increase power
	   Bit<5:0>: Each step represents 0.5dB, range from 0 to 4

	   Increase or decrease 0x13b0<5:0> when bandwidth is changed
	 */
	if (ad->CommonCfg.BBPCurrentBW != BW_20)
		ad->DefaultTargetPwr = (CHAR)ad->DefaultTargetPwr + delta_power;

	DBGPRINT(RT_DEBUG_ERROR, ("%s::DefaultTargetPwr = 0x%x, delta_power = 0x%x\n",
				  __func__, ad->DefaultTargetPwr, delta_power));

	if ((ad->chipCap.temp_tx_alc_enable == TRUE) && (ad->chipCap.tssi_enable == FALSE)) {
		sku_base_pwr = mt76x2_get_sku_channel_base_pwr(ad, channel);

		if (ad->DefaultTargetPwr > sku_base_pwr)
			ch_pwr_adj = sku_base_pwr - ad->DefaultTargetPwr;
		else
			ch_pwr_adj = 0;

		if (ch_pwr_adj > 31)
			ch_pwr_adj = 31;
		if (ch_pwr_adj < -32)
			ch_pwr_adj = -32;

		RTMP_IO_READ32(ad, TX_ALC_CFG_1, &reg_val);
		reg_val = (reg_val & ~0x3F) | (ch_pwr_adj & 0x3F);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_1, reg_val);

		RTMP_IO_READ32(ad, TX_ALC_CFG_2, &reg_val);
		reg_val = (reg_val & ~0x3F) | (ch_pwr_adj & 0x3F);
		RTMP_IO_WRITE32(ad, TX_ALC_CFG_2, reg_val);

		ad->tc_init_val = ch_pwr_adj & 0x3F;

		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s::sku_base_pwr = 0x%x, DefaultTargetPwr = 0x%x, ch_pwr_adj = 0x%x(%d), 0x13B4: 0x%x\n",
			  __func__, sku_base_pwr, ad->DefaultTargetPwr,
			  ch_pwr_adj, ch_pwr_adj, reg_val));
	}

	mt76x2_update_sku_pwr(ad, channel);

	ad->sku_init_done = TRUE;
}

void mt76x2_read_single_sku_info_from_eeprom(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	USHORT ee_val = 0;

	/* TSSI off 2.4G Tx power SKU */
	RT28xx_EEPROM_READ16(ad, 0xF7, ee_val);
	pChipCap->tssi_off_54m_target_pwr_g_band = ee_val & 0xFF;

	/* TSSI off 5G Tx power SKU */
	RT28xx_EEPROM_READ16(ad, 0xF8, ee_val);
	pChipCap->tssi_off_54m_target_pwr_a_band = ee_val & 0xFF;

	/*  TSSI on 2.4G Tx power */
	RT28xx_EEPROM_READ16(ad, 0x58, ee_val);
	pChipCap->tssi_54m_target_pwr_g_band = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group0) */
	RT28xx_EEPROM_READ16(ad, 0x64, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP0_CHL] = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group1) */
	RT28xx_EEPROM_READ16(ad, 0x69, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP1_CHL] = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group2) */
	RT28xx_EEPROM_READ16(ad, 0x6E, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP2_CHL] = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group3) */
	RT28xx_EEPROM_READ16(ad, 0x73, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP3_CHL] = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group4) */
	RT28xx_EEPROM_READ16(ad, 0x78, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP4_CHL] = ee_val & 0xFF;

	/*  TSSI on 5G Tx power (Group5) */
	RT28xx_EEPROM_READ16(ad, 0x7D, ee_val);
	pChipCap->tssi_54m_target_pwr_a_band[A_BAND_GRP5_CHL] = ee_val & 0xFF;
}

void mt76x2_make_up_rate_pwr_table(RTMP_ADAPTER *ad)
{
	UINT32 reg_val = 0;

	RTMP_IO_READ32(ad, TX_PWR_CFG_0, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_0, reg_val));
	ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* CCK 1M */
	if (ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.CCK[1].mcs_pwr = ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr;	/* CCK 2M */

	ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* CCK 5.5M */
	if (ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.CCK[3].mcs_pwr = ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr;	/* CCK 11M */

	ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* OFDM 6M */
	if (ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.OFDM[1].mcs_pwr = ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr;	/* OFDM 9M */

	ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* OFDM 12M */
	if (ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.OFDM[3].mcs_pwr = ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr;	/* OFDM 18M */

	RTMP_IO_READ32(ad, TX_PWR_CFG_1, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_1, reg_val));
	ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* OFDM 24M */
	if (ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.OFDM[5].mcs_pwr = ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr;	/* OFDM 36M */

	ad->chipCap.rate_pwr_table.OFDM[6].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* OFDM 48M */
	if (ad->chipCap.rate_pwr_table.OFDM[6].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.OFDM[6].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.HT[0].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* HT/VHT1SS MCS0 */
	if (ad->chipCap.rate_pwr_table.HT[0].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[0].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT1SS[0].mcs_pwr = ad->chipCap.rate_pwr_table.HT[0].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[1].mcs_pwr = ad->chipCap.rate_pwr_table.HT[0].mcs_pwr;	/* HT/VHT1SS MCS1 */
	ad->chipCap.rate_pwr_table.VHT1SS[1].mcs_pwr = ad->chipCap.rate_pwr_table.VHT1SS[0].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[2].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* HT/VHT1SS MCS2 */
	if (ad->chipCap.rate_pwr_table.HT[2].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[2].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT1SS[2].mcs_pwr = ad->chipCap.rate_pwr_table.HT[2].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[3].mcs_pwr = ad->chipCap.rate_pwr_table.HT[2].mcs_pwr;	/* HT/VHT1SS MCS3 */
	ad->chipCap.rate_pwr_table.VHT1SS[3].mcs_pwr = ad->chipCap.rate_pwr_table.VHT1SS[2].mcs_pwr;

	RTMP_IO_READ32(ad, TX_PWR_CFG_2, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_2, reg_val));
	ad->chipCap.rate_pwr_table.HT[4].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* HT/VHT1SS MCS4 */
	if (ad->chipCap.rate_pwr_table.HT[4].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[4].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT1SS[4].mcs_pwr = ad->chipCap.rate_pwr_table.HT[4].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[5].mcs_pwr = ad->chipCap.rate_pwr_table.HT[4].mcs_pwr;	/* HT/VHT1SS MCS5 */
	ad->chipCap.rate_pwr_table.VHT1SS[5].mcs_pwr = ad->chipCap.rate_pwr_table.VHT1SS[4].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[6].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* HT/VHT1SS MCS6 */
	if (ad->chipCap.rate_pwr_table.HT[6].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[6].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT1SS[6].mcs_pwr = ad->chipCap.rate_pwr_table.HT[6].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[8].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* HT MCS8 / VHT2SS MCS0 */
	if (ad->chipCap.rate_pwr_table.HT[8].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[8].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT2SS[0].mcs_pwr = ad->chipCap.rate_pwr_table.HT[8].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[9].mcs_pwr =
		ad->chipCap.rate_pwr_table.HT[9].mcs_pwr;	/* HT MCS9 / VHT2SS MCS1 */
	ad->chipCap.rate_pwr_table.VHT2SS[1].mcs_pwr = ad->chipCap.rate_pwr_table.VHT2SS[1].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[10].mcs_pwr =
		(CHAR) ((reg_val & 0x3F000000) >> 24);	/* HT MCS10 / VHT2SS MCS2 */
	if (ad->chipCap.rate_pwr_table.HT[10].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[10].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT2SS[2].mcs_pwr = ad->chipCap.rate_pwr_table.HT[10].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[11].mcs_pwr =
		ad->chipCap.rate_pwr_table.HT[10].mcs_pwr;	/* HT MCS11 / VHT2SS MCS3 */
	ad->chipCap.rate_pwr_table.VHT2SS[3].mcs_pwr = ad->chipCap.rate_pwr_table.VHT2SS[2].mcs_pwr;

	RTMP_IO_READ32(ad, TX_PWR_CFG_3, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_3, reg_val));
	ad->chipCap.rate_pwr_table.HT[12].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* HT MCS12 / VHT2SS MCS4 */
	if (ad->chipCap.rate_pwr_table.HT[12].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[12].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT2SS[4].mcs_pwr = ad->chipCap.rate_pwr_table.HT[12].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[13].mcs_pwr =
		ad->chipCap.rate_pwr_table.HT[12].mcs_pwr;	/* HT MCS13 / VHT2SS MCS5 */
	ad->chipCap.rate_pwr_table.VHT2SS[5].mcs_pwr = ad->chipCap.rate_pwr_table.VHT2SS[4].mcs_pwr;

	ad->chipCap.rate_pwr_table.HT[14].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* HT MCS14 / VHT2SS MCS6 */
	if (ad->chipCap.rate_pwr_table.HT[14].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[14].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT2SS[6].mcs_pwr = ad->chipCap.rate_pwr_table.HT[14].mcs_pwr;

	ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* HT/VHT STBC MCS0 */
	if (ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.STBC[1].mcs_pwr =
		ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr;	/* HT/VHT STBC MCS1 */

	ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* HT/VHT STBC MCS2 */
	if (ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.STBC[3].mcs_pwr =
		ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr;	/* HT/VHT STBC MCS3 */

	RTMP_IO_READ32(ad, TX_PWR_CFG_4, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_4, reg_val));
	ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* HT/VHT STBC MCS4 */
	if (ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.STBC[5].mcs_pwr =
		ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr;	/* HT/VHT STBC MCS5 */

	ad->chipCap.rate_pwr_table.STBC[6].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* HT/VHT STBC MCS6 */
	if (ad->chipCap.rate_pwr_table.STBC[6].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[6].mcs_pwr -= 64;

	RTMP_IO_READ32(ad, TX_PWR_CFG_7, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_7, reg_val));
	ad->chipCap.rate_pwr_table.OFDM[7].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* OFDM 54M */
	if (ad->chipCap.rate_pwr_table.OFDM[7].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.OFDM[7].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.VHT2SS[8].mcs_pwr = (CHAR) ((reg_val & 0x3F00) >> 8);	/* VHT2SS MCS8 */
	if (ad->chipCap.rate_pwr_table.VHT2SS[8].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.VHT2SS[8].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.HT[7].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* HT/VHT1SS MCS7 */
	if (ad->chipCap.rate_pwr_table.HT[7].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[7].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT1SS[7].mcs_pwr = ad->chipCap.rate_pwr_table.HT[7].mcs_pwr;

	ad->chipCap.rate_pwr_table.VHT2SS[9].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* VHT2SS MCS9 */
	if (ad->chipCap.rate_pwr_table.VHT2SS[9].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.VHT2SS[9].mcs_pwr -= 64;

	RTMP_IO_READ32(ad, TX_PWR_CFG_8, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_8, reg_val));
	ad->chipCap.rate_pwr_table.HT[15].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* HT MCS15 / VHT2SS MCS7 */
	if (ad->chipCap.rate_pwr_table.HT[15].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.HT[15].mcs_pwr -= 64;
	ad->chipCap.rate_pwr_table.VHT2SS[7].mcs_pwr = ad->chipCap.rate_pwr_table.HT[15].mcs_pwr;

	ad->chipCap.rate_pwr_table.VHT1SS[8].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* VHT1SS MCS8 */
	if (ad->chipCap.rate_pwr_table.VHT1SS[8].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.VHT1SS[8].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.VHT1SS[9].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* VHT1SS MCS9 */
	if (ad->chipCap.rate_pwr_table.VHT1SS[9].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.VHT1SS[9].mcs_pwr -= 64;

	RTMP_IO_READ32(ad, TX_PWR_CFG_9, &reg_val);
	DBGPRINT(RT_DEBUG_TRACE, ("0x%x: 0x%x\n", TX_PWR_CFG_9, reg_val));
	ad->chipCap.rate_pwr_table.STBC[7].mcs_pwr = (CHAR) (reg_val & 0x3F);	/* HT/VHT STBC MCS7 */
	if (ad->chipCap.rate_pwr_table.STBC[7].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[7].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.STBC[8].mcs_pwr = (CHAR) ((reg_val & 0x3F0000) >> 16);	/* VHT STBC MCS8 */
	if (ad->chipCap.rate_pwr_table.STBC[8].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[8].mcs_pwr -= 64;

	ad->chipCap.rate_pwr_table.STBC[9].mcs_pwr = (CHAR) ((reg_val & 0x3F000000) >> 24);	/* VHT STBC MCS9 */
	if (ad->chipCap.rate_pwr_table.STBC[9].mcs_pwr & 0x20)
		ad->chipCap.rate_pwr_table.STBC[9].mcs_pwr -= 64;

	DBGPRINT(RT_DEBUG_TRACE, ("\n"));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.CCK[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.CCK[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.CCK[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.CCK[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.CCK[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.CCK[3].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[3].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[4].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[5].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[5].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[6].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[6].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.OFDM[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.OFDM[7].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[3].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[4].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[4].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[5].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[5].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[6].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[6].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[7].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[8].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[8].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[9].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[9].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[10].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[10].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[11].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[11].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[12].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[12].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[13].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[13].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[14].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[14].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.HT[15].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.HT[15].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[3].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[4].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[4].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[5].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[5].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[6].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[6].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[7].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[8].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[8].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT1SS[9].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT1SS[9].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[3].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[4].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[4].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[5].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[5].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[6].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[6].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[7].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[8].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[8].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.VHT2SS[9].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.VHT2SS[9].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[0].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[1].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[1].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[2].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[3].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[3].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[4].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[5].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[5].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[6].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[6].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[7].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[8].mcs_pwr));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.STBC[7].MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.STBC[9].mcs_pwr));

	DBGPRINT(RT_DEBUG_TRACE,
		 ("rate_pwr_table.MCS32.MCS_Power = %d\n",
		  ad->chipCap.rate_pwr_table.MCS32.mcs_pwr));
}

UCHAR mt76x2_get_sku_channel_base_pwr_proc(RTMP_ADAPTER *ad, u8 channel, CH_POWER *ch)
{
	UINT8 i = 0;
	UCHAR base_pwr = ad->DefaultTargetPwr;

	if (channel <= 14) {
		for (i = 0; i < SINGLE_SKU_TABLE_CCK_LENGTH; i++) {
			if (base_pwr > ch->PwrCCK[i])
				base_pwr = ch->PwrCCK[i];
		}
	}

	for (i = 0; i < SINGLE_SKU_TABLE_OFDM_LENGTH; i++) {
		if (base_pwr > ch->PwrOFDM[i])
			base_pwr = ch->PwrOFDM[i];
	}

	if (ad->CommonCfg.BBPCurrentBW == BW_20) {
		for (i = 0; i < SINGLE_SKU_TABLE_HT_LENGTH; i++) {
			if (ch->PwrHT20[i] == 0)
				break;

			if (base_pwr > ch->PwrHT20[i])
				base_pwr = ch->PwrHT20[i];
		}
	}

	if (ad->CommonCfg.BBPCurrentBW == BW_40) {
		for (i = 0; i < SINGLE_SKU_TABLE_HT_LENGTH; i++) {
			if (ch->PwrHT40[i] == 0)
				break;

			if (base_pwr > ch->PwrHT40[i])
				base_pwr = ch->PwrHT40[i];
		}
	}

	if (ad->CommonCfg.BBPCurrentBW == BW_80) {
		for (i = 0; i < SINGLE_SKU_TABLE_VHT_LENGTH; i++) {
			if (ch->PwrVHT80[i] == 0)
				break;

			if (base_pwr > ch->PwrVHT80[i])
				base_pwr = ch->PwrVHT80[i];
		}
	}
	return base_pwr;
}

UCHAR mt76x2_get_sku_channel_base_pwr(RTMP_ADAPTER *ad, u8 channel)
{
	CH_POWER *ch, *ch_temp;
	UCHAR start_ch = 0, base_pwr = ad->DefaultTargetPwr;
	UINT8 j = 0;

	DlListForEachSafe(ch, ch_temp, &ad->SingleSkuPwrList, CH_POWER, List) {
		start_ch = ch->StartChannel;

		if (channel >= start_ch) {
			for (j = 0; j < ch->num; j++) {
				if (channel == ch->Channel[j]) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("%s ==> channel = %d, ch->channel = %d\n",
						  __func__, channel, ch->Channel[j]));

					base_pwr = mt76x2_get_sku_channel_base_pwr_proc(
						ad, channel, ch);
					break;
				}
			}
		}
	}

	return base_pwr;
}

void mt76x2_update_per_rate_pwr(RTMP_ADAPTER *ad)
{
	UINT32 data = 0;
	UCHAR t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	/*
	   Bit 29:24 -> OFDM 12M/18M
	   Bit 21:16 -> OFDM 6M/9M
	   Bit 13:8 -> CCK 5.5M/11M
	   Bit 5:0 -> CCK 1M/2M
	 */
	t1 = ad->chipCap.rate_pwr_table.CCK[0].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.CCK[2].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.OFDM[0].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.OFDM[2].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);
	data = (t4 << 24) | (t3 << 16) | (t2 << 8) | t1;

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_0, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_0, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_0, data));

	/*
	   Bit 29:24 -> HT/VHT1SS MCS 2/3
	   Bit 21:16 -> HT/VHT1SS MCS 0/1
	   Bit 13:8 -> OFDM 48M
	   Bit 5:0 -> OFDM 24M/36M
	 */
	t1 = ad->chipCap.rate_pwr_table.OFDM[4].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.OFDM[6].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.HT[0].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.HT[2].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);
	data = (t4 << 24) | (t3 << 16) | (t2 << 8) | t1;

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_1, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_1, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_1, data));

	/*
	   Bit 29:24 -> HT MCS 10/11 / VHT2SS MCS 2/3
	   Bit 21:16 -> HT MCS 8/9 / VHT2SS MCS 0/1
	   Bit 13:8 -> HT/VHT1SS MCS 6
	   Bit 5:0 -> HT/VHT1SS MCS 4/5
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_2, &data);
	t1 = ad->chipCap.rate_pwr_table.HT[4].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.HT[6].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.HT[8].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.HT[10].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);
	data = (t4 << 24) | (t3 << 16) | (t2 << 8) | t1;

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_2, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_2, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_2, data));

	/*
	   Bit 29:24 -> HT/VHT STBC MCS 2/3
	   Bit 21:16 -> HT/VHT STBC MCS 0/1
	   Bit 13:8 -> HT MCS 14 / VHT2SS MCS 6
	   Bit 5:0 -> HT MCS 12/13 / VHT2SS MCS 4/5
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_3, &data);
	t1 = ad->chipCap.rate_pwr_table.HT[12].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.HT[14].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.STBC[0].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.STBC[2].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);
	data = (t4 << 24) | (t3 << 16) | (t2 << 8) | t1;

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_3, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_3, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_3, data));

	/*
	   Bit 13:8 -> HT/VHT STBC MCS 6
	   Bit 5:0 -> HT/VHT STBC MCS 4/5
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_4, &data);
	t1 = ad->chipCap.rate_pwr_table.STBC[4].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.STBC[6].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	data &= 0xFFFF0000;
	data = (t2 << 8) | t1;
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_4, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_4, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_4, data));

	/*
	   Bit 29:24 -> VHT2SS MCS 9
	   Bit 21:16 -> HT/VHT1SS MCS 7
	   Bit 13:8 -> VHT2SS MCS 8
	   Bit 5:0 -> OFDM 54M
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_7, &data);
	t1 = ad->chipCap.rate_pwr_table.OFDM[7].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t2 = ad->chipCap.rate_pwr_table.VHT2SS[8].mcs_pwr;
	t2 = (t2 & 0x80) ? ((t2 & 0x1f) | 0x20) : (t2 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.HT[7].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.VHT2SS[9].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);
	data = (t4 << 24) | (t3 << 16) | (t2 << 8) | t1;

	RTMP_IO_WRITE32(ad, TX_PWR_CFG_7, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_7, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_7, data));

	/*
	   Bit 29:24 -> VHT1SS MCS 9
	   Bit 21:16 -> VHT1SS MCS 8
	   Bit 5:0 -> HT MCS 15 / VHT2SS MCS 7
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_8, &data);
	t1 = ad->chipCap.rate_pwr_table.HT[15].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.VHT1SS[8].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.VHT1SS[9].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);

	data &= 0xFF00;
	data = (t4 << 24) | (t3 << 16) | t1;
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_8, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_8, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_8, data));

	/*
	   Bit 29:24 -> VHT STBC MCS 9
	   Bit 21:16 -> VHT STBC MCS 8
	   Bit 5:0 -> HT/VHT STBC MCS 7
	 */
	RTMP_IO_READ32(ad, TX_PWR_CFG_9, &data);
	t1 = ad->chipCap.rate_pwr_table.STBC[7].mcs_pwr;
	t1 = (t1 & 0x80) ? ((t1 & 0x1f) | 0x20) : (t1 & 0x3f);

	t3 = ad->chipCap.rate_pwr_table.STBC[8].mcs_pwr;
	t3 = (t3 & 0x80) ? ((t3 & 0x1f) | 0x20) : (t3 & 0x3f);

	t4 = ad->chipCap.rate_pwr_table.STBC[9].mcs_pwr;
	t4 = (t4 & 0x80) ? ((t4 & 0x1f) | 0x20) : (t4 & 0x3f);

	data &= 0xFF00;
	data = (t4 << 24) | (t3 << 16) | t1;
	RTMP_IO_WRITE32(ad, TX_PWR_CFG_9, data);
	RTMP_IO_READ32(ad, TX_PWR_CFG_9, &data);
	DBGPRINT(RT_DEBUG_TRACE, ("%s - 0x%x: 0x%08X\n", __func__, TX_PWR_CFG_9, data));
}

VOID mt76x2_update_sku_pwr_proc(RTMP_ADAPTER *ad, CHAR ch_delta_pwr, CH_POWER *ch)
{
	INT32 i = 0;
	INT32 pwr_delta = 0;
	INT32 rate_pwr = 0;
	const CHAR DefaultTargetPwr = ad->DefaultTargetPwr;

	for (i = 0; i < SINGLE_SKU_TABLE_CCK_LENGTH; i++) {
		pwr_delta = 0;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> CCK[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
			  __func__, i, ad->chipCap.rate_pwr_table.CCK[i].mcs_pwr, DefaultTargetPwr,
			  ch_delta_pwr));

		rate_pwr = ad->chipCap.rate_pwr_table.CCK[i].mcs_pwr + DefaultTargetPwr
			+ ch_delta_pwr;
		pwr_delta = rate_pwr - ch->PwrCCK[i];
		if (pwr_delta > 0)	/* power exceed */
			ad->chipCap.rate_pwr_table.CCK[i].mcs_pwr -= pwr_delta;

		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> rate_pwr (%d) - ch->PwrCCK[%d] (%d) = rate_delta (%d)\n",
			  __func__, rate_pwr, i, ch->PwrCCK[i], pwr_delta));

		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> new CCK[%d].MCS_Power = %d\n\n",
			  __func__, i, ad->chipCap.rate_pwr_table.CCK[i].mcs_pwr));
	}

	for (i = 0; i < SINGLE_SKU_TABLE_OFDM_LENGTH; i++) {
		pwr_delta = 0;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> OFDM[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
			  __func__, i, ad->chipCap.rate_pwr_table.OFDM[i].mcs_pwr, DefaultTargetPwr,
			  ch_delta_pwr));

		rate_pwr = ad->chipCap.rate_pwr_table.OFDM[i].mcs_pwr + DefaultTargetPwr + ch_delta_pwr;
		pwr_delta = rate_pwr - ch->PwrOFDM[i];
		if (pwr_delta > 0)	/* power exceed */
			ad->chipCap.rate_pwr_table.OFDM[i].mcs_pwr -= pwr_delta;

		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> rate_pwr (%d) - ch->PwrOFDM[%d] (%d) = rate_delta (%d)\n",
			  __func__, rate_pwr, i, ch->PwrOFDM[i], pwr_delta));

		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s ==> new OFDM[%d].MCS_Power = %d\n\n",
			  __func__, i, ad->chipCap.rate_pwr_table.OFDM[i].mcs_pwr));
	}

	if (ad->CommonCfg.BBPCurrentBW != BW_80) {
		for (i = 0; i < SINGLE_SKU_TABLE_HT_LENGTH; i++) {
			pwr_delta = 0;
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> HT[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.HT[i].mcs_pwr, DefaultTargetPwr,
				  ch_delta_pwr));

			rate_pwr = ad->chipCap.rate_pwr_table.HT[i].mcs_pwr + DefaultTargetPwr + ch_delta_pwr;

			if (ad->CommonCfg.BBPCurrentBW == BW_20) {
				pwr_delta = rate_pwr - ch->PwrHT20[i];

				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s ==> rate_pwr (%d) - ch->PwrHT20[%d] (%d) = rate_delta (%d)\n",
					  __func__, rate_pwr, i, ch->PwrHT20[i], pwr_delta));

			} else if (ad->CommonCfg.BBPCurrentBW == BW_40) {
				pwr_delta = rate_pwr - ch->PwrHT40[i];

				DBGPRINT(RT_DEBUG_TRACE,
					 ("%s ==> rate_pwr (%d) - ch->PwrHT40[%d] (%d) = rate_delta (%d)\n",
					  __func__, rate_pwr, i, ch->PwrHT40[i], pwr_delta));
			}

			if (pwr_delta > 0)
				ad->chipCap.rate_pwr_table.HT[i].mcs_pwr -= pwr_delta;

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> new HT[%d].MCS_Power = %d\n\n",
				  __func__, i, ad->chipCap.rate_pwr_table.HT[i].mcs_pwr));
		}

		for (i = 8; i < SINGLE_SKU_TABLE_VHT_LENGTH; i++) {
			pwr_delta = 0;
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> VHT1SS[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr, DefaultTargetPwr,
				  ch_delta_pwr));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> VHT2SS[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr, DefaultTargetPwr,
				  ch_delta_pwr));

			rate_pwr = ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr + DefaultTargetPwr + ch_delta_pwr;
			pwr_delta = rate_pwr - ch->PwrVHT80[i];

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> rate_pwr (%d) - ch->PwrVHT80[%d] (%d) = rate_delta (%d)\n",
				  __func__, rate_pwr, i, ch->PwrVHT80[i], pwr_delta));

			if (pwr_delta > 0) {
				ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr -= pwr_delta;
				ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr -= pwr_delta;
			}

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> new VHT1SS[%d].MCS_Power = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> new VHT2SS[%d].MCS_Power = %d\n\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr));
		}
	} else if (ad->CommonCfg.BBPCurrentBW == BW_80) {
		for (i = 0; i < SINGLE_SKU_TABLE_VHT_LENGTH; i++) {
			pwr_delta = 0;
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> VHT1SS[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr, DefaultTargetPwr,
				  ch_delta_pwr));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> VHT2SS[%d].MCS_Power = %d, DefaultTargetPwr = %d, ch_delta_pwr = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr, DefaultTargetPwr,
				  ch_delta_pwr));

			rate_pwr =
				ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr + DefaultTargetPwr + ch_delta_pwr;
			pwr_delta = rate_pwr - ch->PwrVHT80[i];

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> rate_pwr (%d) - ch->PwrVHT80[%d] (%d) = rate_delta (%d)\n",
				  __func__, rate_pwr, i, ch->PwrVHT80[i], pwr_delta));

			if (pwr_delta > 0) {
				ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr -= pwr_delta;
				ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr -= pwr_delta;
				if (i < 8) {
					ad->chipCap.rate_pwr_table.HT[i].mcs_pwr =
						ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr;
					ad->chipCap.rate_pwr_table.HT[i + 8].mcs_pwr =
						ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr;
				}
			}

			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> new VHT1SS[%d].MCS_Power = %d\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT1SS[i].mcs_pwr));
			DBGPRINT(RT_DEBUG_TRACE,
				 ("%s ==> new VHT2SS[%d].MCS_Power = %d\n\n",
				  __func__, i, ad->chipCap.rate_pwr_table.VHT2SS[i].mcs_pwr));
		}
	}
}


UCHAR mt76x2_update_sku_pwr(RTMP_ADAPTER *ad, u8 channel)
{
	CH_POWER *ch, *ch_temp;
	INT32 j = 0;
	UINT32 reg_val = 0;
	CHAR ch_delta_pwr = 0;
	BOOLEAN bFound = FALSE;
	UCHAR start_ch = 0;

	if ((ad->chipCap.temp_tx_alc_enable == TRUE) && (ad->chipCap.tssi_enable == FALSE)) {
		/*
		   Get power temperature compensation
		   Format: 6-bit, signed value
		   Unit: 0.5dB, range: -10 ~ 10dB
		 */
		RTMP_IO_READ32(ad, TX_ALC_CFG_1, &reg_val);
		ch_delta_pwr = (UCHAR) (reg_val & 0x3F);
		if (ch_delta_pwr & 0x20)
			ch_delta_pwr -= 64;
		DBGPRINT(RT_DEBUG_TRACE, ("%s ==> 0x%08X = 0x%08X, ch_delta_pwr = %d\n",
					  __func__, TX_ALC_CFG_1, reg_val, ch_delta_pwr));
	} else
		ch_delta_pwr = 0;

		mt76x2_get_tx_pwr_per_rate(ad);

	/*
	   Get per rate power delta
	 */
	mt76x2_make_up_rate_pwr_table(ad);

	DlListForEachSafe(ch, ch_temp, &ad->SingleSkuPwrList, CH_POWER, List) {
		start_ch = ch->StartChannel;

		if (channel >= start_ch) {
			for (j = 0; j < ch->num; j++) {
				if (channel == ch->Channel[j]) {
					DBGPRINT(RT_DEBUG_TRACE,
						 ("%s ==> channel = %d, ch->channel = %d\n",
						  __func__, channel, ch->Channel[j]));

					mt76x2_update_sku_pwr_proc(ad, ch_delta_pwr, ch);
					bFound = TRUE;
					break;
				}
			}
		}
	}

	if (bFound) {
		mt76x2_update_per_rate_pwr(ad);
		return TRUE;
	} else
		return FALSE;
}
#endif /* SINGLE_SKU_V2 */

#ifdef CONFIG_STA_SUPPORT
static VOID mt76x2_init_dev_nick_name(RTMP_ADAPTER *ad)
{
#ifdef RTMP_MAC_PCI
	if (IS_MT7662E(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7662e_sta");
	else if (IS_MT7632E(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7632e_sta");
	else if (IS_MT7612E(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7612e_sta");
#endif /* endif */

#ifdef RTMP_MAC_USB
	if (IS_MT7662U(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7662u_sta");
	else if (IS_MT7632U(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7632u_sta");
	else if (IS_MT7612U(ad))
		snprintf((PSTRING) ad->nickname, sizeof(ad->nickname), "mt7612u_sta");
#endif /* endif */
}
#endif /* CONFIG_STA_SUPPORT */

#ifdef CAL_FREE_IC_SUPPORT
static BOOLEAN mt76x2_is_cal_free_ic(RTMP_ADAPTER *ad)
{
	UINT16 NicConfig, FrequencyOffset;
	UINT16 PowerDelta, TssiSlope, TxPower;
	UINT EfuseFreeBlock = 0;

	eFuseGetFreeBlockCount(ad, &EfuseFreeBlock);

	if (EfuseFreeBlock < ad->chipCap.EFUSE_RESERVED_SIZE)
		return FALSE;

	eFuseReadRegisters(ad, XTAL_TRIM1, 2, &FrequencyOffset);
	eFuseReadRegisters(ad, NIC_CONFIGURE_0, 2, &NicConfig);

	if (!((NicConfig == 0x0) && (FrequencyOffset != 0xFFFF)))
		return FALSE;

	eFuseReadRegisters(ad, G_BAND_20_40_BW_PWR_DELTA, 2, &PowerDelta);
	eFuseReadRegisters(ad, TX0_G_BAND_TSSI_SLOPE, 2, &TssiSlope);

	if (!((PowerDelta == 0x0) && (TssiSlope != 0xFFFF)))
		return FALSE;

	eFuseReadRegisters(ad, GRP3_TX0_A_BAND_CHL_PWR_DELTA_LOW, 2, &TxPower);
	eFuseReadRegisters(ad, GRP4_TX0_A_BAND_TSSI_SLOPE, 2, &TssiSlope);

	if (!((TxPower == 0x0) && (TssiSlope != 0xFFFF)))
		return FALSE;

	return TRUE;

}

static VOID mt76x2_cal_free_data_get(RTMP_ADAPTER *ad)
{
	UINT16 value;

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));

	/* 0x3A */
	eFuseReadRegisters(ad, XTAL_TRIM1, 2, &value);
	ad->EEPROMImage[XTAL_TRIM1] = value & 0xFF;

	/* 0x55 0x56 0x57 0x5C 0x5D */
	eFuseReadRegisters(ad, A_BAND_EXT_PA_SETTING, 2, &value);
	ad->EEPROMImage[A_BAND_EXT_PA_SETTING + 1] = (value >> 8) & 0xFF;
	eFuseReadRegisters(ad, TX0_G_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[TX0_G_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, TX1_G_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[TX1_G_BAND_TSSI_SLOPE]) = value;

	/* 0x62 0x63 0x67 0x68 0x6C 0x6D */
	eFuseReadRegisters(ad, GRP0_TX0_A_BAND_TSSI_SLOPE, 2, &value);
	if (value != 0)
		*(UINT16 *) (&ad->EEPROMImage[GRP0_TX0_A_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP0_TX0_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP1_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8;
	eFuseReadRegisters(ad, GRP1_TX0_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP1_TX0_A_BAND_TSSI_OFFSET] = value & GRP1_TX0_A_BAND_TSSI_OFFSET_MASK;
	eFuseReadRegisters(ad, GRP2_TX0_A_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[GRP2_TX0_A_BAND_TSSI_SLOPE]) = value;

	/* 0x71 0x72 0x76 0x77 0x7B 0x7C */
	eFuseReadRegisters(ad, GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP2_TX0_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP3_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8;
	eFuseReadRegisters(ad, GRP3_TX0_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP3_TX0_A_BAND_TSSI_OFFSET] = value & GRP3_TX0_A_BAND_TSSI_OFFSET_MASK;
	eFuseReadRegisters(ad, GRP4_TX0_A_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[GRP4_TX0_A_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP4_TX0_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP5_TX0_A_BAND_TSSI_SLOPE_MASK) >> 8;
	eFuseReadRegisters(ad, GRP5_TX0_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP5_TX0_A_BAND_TSSI_OFFSET] = value & GRP5_TX0_A_BAND_TSSI_OFFSET_MASK;

	/* 0x80 0x81 0x85 0x86 0x8A 0x8B 0x8F */
	eFuseReadRegisters(ad, GRP0_TX1_A_BAND_TSSI_SLOPE, 2, &value);
	if (value != 0)
		*(UINT16 *) (&ad->EEPROMImage[GRP0_TX1_A_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP0_TX1_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP1_TX1_A_BAND_TSSI_SLOPE_MASK) >> 8;
	eFuseReadRegisters(ad, GRP1_TX1_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP1_TX1_A_BAND_TSSI_OFFSET] = value & GRP1_TX1_A_BAND_TSSI_OFFSET_MASK;
	eFuseReadRegisters(ad, GRP2_TX1_A_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[GRP2_TX1_A_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP2_TX1_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP3_TX1_A_BAND_TSSI_SLOPE_MASK) >> 8;

	/* 0x90 0x94 0x95 0x99 0x9A */
	eFuseReadRegisters(ad, GRP3_TX1_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP3_TX1_A_BAND_TSSI_OFFSET] = value & GRP3_TX1_A_BAND_TSSI_OFFSET_MASK;
	eFuseReadRegisters(ad, GRP4_TX1_A_BAND_TSSI_SLOPE, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[GRP4_TX1_A_BAND_TSSI_SLOPE]) = value;
	eFuseReadRegisters(ad, GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI, 2, &value);
	ad->EEPROMImage[GRP4_TX1_A_BAND_CHL_PWR_DELTA_HI + 1] =
	    (value & GRP5_TX1_A_BAND_TSSI_SLOPE_MASK) >> 8;
	eFuseReadRegisters(ad, GRP5_TX1_A_BAND_TSSI_OFFSET, 2, &value);
	ad->EEPROMImage[GRP5_TX1_A_BAND_TSSI_OFFSET] = value & GRP5_TX1_A_BAND_TSSI_OFFSET_MASK;

	/* 0xF6 */
	eFuseReadRegisters(ad, CP_FT_VERSION, 2, &value);
	ad->EEPROMImage[CP_FT_VERSION] = value & CP_FT_VERSION_MASK;

	/* 0xF9 0xFA 0xFB 0xFC 0xFD 0xFE 0xFF */
	eFuseReadRegisters(ad, RF_2G_RX_HIGH_GAIN, 2, &value);
	ad->EEPROMImage[RF_2G_RX_HIGH_GAIN + 1] = (value >> 8) & 0xFF;
	eFuseReadRegisters(ad, RF_5G_GRP0_1_RX_HIGH_GAIN, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[RF_5G_GRP0_1_RX_HIGH_GAIN]) = value;
	eFuseReadRegisters(ad, RF_5G_GRP2_3_RX_HIGH_GAIN, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[RF_5G_GRP2_3_RX_HIGH_GAIN]) = value;
	eFuseReadRegisters(ad, RF_5G_GRP4_5_RX_HIGH_GAIN, 2, &value);
	*(UINT16 *) (&ad->EEPROMImage[RF_5G_GRP4_5_RX_HIGH_GAIN]) = value;

	/* BT: 0x138 0x13D 0x13E 0x13F */
	eFuseReadRegisters(ad, BT_RCAL_RESULT, 2, &value);
	if (value != 0xFFFF)
		ad->EEPROMImage[BT_RCAL_RESULT] = value & 0xFF;
	eFuseReadRegisters(ad, BT_VCDL_CALIBRATION, 2, &value);
	if (value != 0xFFFF)
		ad->EEPROMImage[BT_VCDL_CALIBRATION + 1] = (value >> 8) & 0xFF;
	eFuseReadRegisters(ad, BT_PMUCFG, 2, &value);
	if (value != 0xFFFF)
		*(UINT16 *) (&ad->EEPROMImage[BT_PMUCFG]) = value;

}
#endif /* CAL_FREE_IC_SUPPORT */

#ifdef ED_MONITOR
void mt7612_set_ed_cca(RTMP_ADAPTER *ad, BOOLEAN enable)
{
	UINT32 mac_val = 0;
	UINT32 bbp_val;
	UCHAR ED_TH = 0x0e;

	if (enable) {
		RTMP_IO_READ32(ad, CH_TIME_CFG, &mac_val);
		mac_val |= 0x40;
		RTMP_IO_WRITE32(ad, CH_TIME_CFG, mac_val);

		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val |= (1 << 20);
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(ad, CH_TIME_CFG, &mac_val);
		/* enable channel status check */
		mac_val |= 0x05;
		RTMP_IO_WRITE32(ad, CH_TIME_CFG, mac_val);

		RTMP_BBP_IO_READ32(ad, AGC1_R2, &bbp_val);
		bbp_val = (bbp_val & 0xFFFF0000) | (ED_TH << 8) | ED_TH;
		RTMP_BBP_IO_WRITE32(ad, AGC1_R2, bbp_val);
	} else {
		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val &= ~(1 << 20);
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s::0x%x: 0x%08X\n", __func__, TXOP_CTRL_CFG, mac_val));
}
#endif /* ED_MONITOR */

VOID mt76x2_antenna_sel_ctl(IN RTMP_ADAPTER *ad)
{
/* Antenna selection control in 76x2 is controlled by fw/rom_patch */
}

#ifdef DYNAMIC_VGA_SUPPORT
#ifdef HOST_DYNAMIC_VGA_SUPPORT
static void periodic_monitor_false_cca_adjust_init_vga(RTMP_ADAPTER *pAd, UCHAR *initvga_0,
						       UCHAR *initvga_1)
{
#ifdef MT76x2
	UCHAR lowbound_0 =
	    (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 >=
	     0x10) ? (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 -
		      0x10) : (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0);
	UCHAR lowbound_1 =
	    (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 >=
	     0x10) ? (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 -
		      0x10) : (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1);
	UCHAR upbound_0 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0;
	UCHAR upbound_1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1;
	UINT32 fcca_1s_count = pAd->RalinkCounters.OneSecFalseCCACnt;

	if (pAd->CommonCfg.Channel <= 48 || pAd->CommonCfg.Channel >= 149) {
		/* DBGPRINT(RT_DEBUG_TRACE, ("%s: OneSecFalseCCACnt=%d, init_0=0x%02x(U:0x%02x,L:0x%02x), */
		/* init_1=0x%02x(U:0x%02x,L:0x%02x)\n", */
		/* __FUNCTION__,fcca_1s_count, *initvga_0,upbound_0,lowbound_0,*initvga_1,upbound_1,lowbound_1)); */
		if (fcca_1s_count > 600) {
			(*initvga_0) =
			    (((*initvga_0) - (0x2)) <=
			     lowbound_0) ? (lowbound_0) : ((*initvga_0) - (0x2));
			(*initvga_1) =
			    (((*initvga_1) - (0x2)) <=
			     lowbound_1) ? (lowbound_1) : ((*initvga_1) - (0x2));
		} else if (fcca_1s_count < 10) {
			(*initvga_0) =
			    (((*initvga_0) + (0x2)) >=
			     upbound_0) ? (upbound_0) : ((*initvga_0) + (0x2));
			(*initvga_1) =
			    (((*initvga_1) + (0x2)) >=
			     upbound_1) ? (upbound_1) : ((*initvga_1) + (0x2));
		} else {
			/* No change */
		}
	} else {
		/* No change */
	}
#endif /* MT76x2 */
}

#ifdef CONFIG_STA_SUPPORT
static void periodic_monitor_rssi_adjust_vga_sta_mode(RTMP_ADAPTER *pAd)
{
	periodic_monitor_rssi_adjust_vga(pAd);
}
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
static void periodic_monitor_rssi_adjust_vga_ap_mode(RTMP_ADAPTER *pAd)
{
	int apidx = 0, sta_count = 0;
	static int there_was_client;	/* remember previous time has client or not */

	/* check if there is any clients */
	for (apidx = 0; apidx < pAd->ApCfg.BssidNum; apidx++)
		sta_count += pAd->ApCfg.MBSSID[apidx].StaCount;

	if (sta_count > 0) {
		if (there_was_client == 0) {
			/* if previously there was no client, we may need to get init vga such that
			   dynamic vga algorithm can run properly.
			 */
#ifdef MT76x2
			if (IS_MT76x2(pAd)) {
				mt76x2_reinit_agc_gain(pAd, pAd->hw_cfg.cent_ch);
				mt76x2_reinit_hi_lna_gain(pAd, pAd->hw_cfg.cent_ch);
				mt76x2_get_agc_gain(pAd, FALSE);
			}
#endif /* MT76x2 */
			there_was_client = 1;
		}
		periodic_monitor_rssi_adjust_vga(pAd);
	} else {
		/* currently no clients, but if previous time there was client,
		   it means the vga was changed, so we need to rollback its value
		 */
		if (there_was_client) {
			/* recover vga to initial values if no clients */
#ifdef MT76x2
			mt76x2_get_agc_gain(pAd, TRUE);
#endif /* endif */
			there_was_client = 0;
		}
	}

}

#endif /* CONFIG_AP_SUPPORT */

static void host_periodic_monitor_rssi_adjust_vga(RTMP_ADAPTER *pAd)
{
#ifdef ATE_DYNAMIC_VGA_SUPPORT
	if (!pAd->CommonCfg.DynamicVGA_Disabled && ATE_ON(pAd))
		periodic_monitor_rssi_adjust_vga(pAd);
	else
#endif /* ATE_DYNAMIC_VGA_SUPPORT*/
	{
#ifdef CONFIG_AP_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
		/* Only when P2P GO is on && Infra is not connected, we start the dynamic VGA for GO */
		if (!pAd->CommonCfg.DynamicVGA_Disabled && RTMP_CFG80211_VIF_P2P_GO_ON(pAd)
		    && !INFRA_ON(pAd))
#else
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
#endif /* endif */
		{
			/* periodic_monitor_false_cca_adjust_vga(pAd); */
			/* DBGPRINT(RT_DEBUG_TRACE, ("ApCfgRSSI=%d\n", RTMPAvgRssi(pAd, &pAd->ApCfg.RssiSample))); */
			periodic_monitor_rssi_adjust_vga_ap_mode(pAd);
		}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
		if (!pAd->CommonCfg.DynamicVGA_Disabled && INFRA_ON(pAd)) {
			periodic_monitor_rssi_adjust_vga_sta_mode(pAd);

			/* only g band need to check if disable channel smoothing */
		}
#endif /* CONFIG_STA_SUPPORT */
	}
}

void periodic_monitor_rssi_adjust_vga(RTMP_ADAPTER *pAd)
{
	UINT32 bbp_val1, bbp_val2;
	UCHAR val1, val2;
	INT32 is_g_band = (pAd->CommonCfg.Channel <= 14) ? (TRUE) : (FALSE);
	INT32 is_elna_g_band = (pAd->NicConfig2.field.ExternalLNAForG);
	INT32 is_elna_a_band = (pAd->NicConfig2.field.ExternalLNAForA);
	static INT32 previous_rssi_in_75_range;

	RTMP_BBP_IO_READ32(pAd, AGC1_R8, &bbp_val1);
	RTMP_BBP_IO_READ32(pAd, AGC1_R9, &bbp_val2);
	if (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 == 0
	    || pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 == 0){
		DBGPRINT(RT_DEBUG_ERROR,
			 ("Initial VGA gain is ZERO! (init_0=0x%x, init_1=0x%x)\n",
			  pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0,
			  pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1));
		return;
	}
	/*
	   DBGPRINT(RT_DEBUG_TRACE, ("is_g_band=%d, elna_g=%d, elna_a=%d, vga_init_0 = %x,
	   vga_init_1 = %x\n",  is_g_band,is_elna_g_band,is_elna_a_band,
	   pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0, pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1));
	 */

	/* dynamic vga algorithm based on CSD document - "MT7662_SW_based_solution_20131128.pptx" */
	if (pAd->chipCap.avg_rssi_all > -45) {
		/* init vga - 0x30 */
		val1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 - 0x30;
		val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x30;
		bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		/* agc vga */
		bbp_val1 &= ~(0xffff << 16);
		bbp_val2 &= ~(0xffff << 16);
		if (is_g_band) {	/* 2G band */
			if (is_elna_g_band) {	/* eLNA */
				bbp_val1 |= ((0x1a0e) << 16);
				bbp_val2 |= ((0x1a0e) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x181e) << 16);
				bbp_val2 |= ((0x181e) << 16);
			}
		} else {	/* 5G band */

			if (is_elna_a_band) {	/* eLNA */
				bbp_val1 |= ((0x1e20) << 16);
				bbp_val2 |= ((0x1e20) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1820) << 16);
				bbp_val2 |= ((0x1820) << 16);
			}
		}
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		previous_rssi_in_75_range = 0;

	} else if (pAd->chipCap.avg_rssi_all > -60) {
		/* init vga - 0x20 */
		val1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 - 0x20;
		val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x20;
		bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		/* agc vga */
		bbp_val1 &= ~(0xffff << 16);
		bbp_val2 &= ~(0xffff << 16);
		if (is_g_band) {	/* 2G band */
			if (is_elna_g_band) {	/* eLNA */
				bbp_val1 |= ((0x161c) << 16);
				bbp_val2 |= ((0x161c) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1830) << 16);
				bbp_val2 |= ((0x1830) << 16);
			}
		} else {	/* 5G band */

			if (is_elna_a_band) {	/* eLNA */
				bbp_val1 |= ((0x202a) << 16);
				bbp_val2 |= ((0x202a) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1830) << 16);
				bbp_val2 |= ((0x1830) << 16);
			}
		}
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		previous_rssi_in_75_range = 0;
		/* Old algorithm */
		/*
		   bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		   bbp_val1 &= ~(0xffff << 16);
		   bbp_val1 |= ((0x1836) << 16);
		   RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		   val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x20;
		   bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		   bbp_val2 &= ~(0xffff << 16);
		   bbp_val2 |= ((0x1836) << 16);
		   RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		 */
	} else if ((pAd->chipCap.avg_rssi_all >= -75) && (pAd->chipCap.avg_rssi_all <= -60)) {
		/* init vga - 0x10 */
		val1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 - 0x10;
		val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x10;
		bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		/* agc vga */
		bbp_val1 &= ~(0xffff << 16);
		bbp_val2 &= ~(0xffff << 16);
		if (is_g_band) {	/* 2G band */
			if (is_elna_g_band) {	/* eLNA */
				bbp_val1 |= ((0x1822) << 16);
				bbp_val2 |= ((0x1822) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1836) << 16);
				bbp_val2 |= ((0x1836) << 16);
			}
		} else {	/* 5G band */

			if (is_elna_a_band) {	/* eLNA */
				bbp_val1 |= ((0x1424) << 16);
				bbp_val2 |= ((0x1424) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1836) << 16);
				bbp_val2 |= ((0x1836) << 16);
			}
		}
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		previous_rssi_in_75_range = 0;
		/* Old algorithm
		   val1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 - 0x10;
		   bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		   bbp_val1 &= ~(0xffff << 16);
		   bbp_val1 |= ((pAd->CommonCfg.lna_vga_ctl.agc_0_vga_set1_2 & 0xffff) << 16);
		   RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		   val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x10;
		   bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		   bbp_val2 &= ~(0xffff << 16);
		   bbp_val2 |= ((pAd->CommonCfg.lna_vga_ctl.agc_1_vga_set1_2 & 0xffff) << 16);
		   RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		 */
	} else {
		/* RSSI < -75 */
		/* adjust init vga */
		if (previous_rssi_in_75_range) {
			val1 = ((bbp_val1 & (0x00007f00)) >> 8) & 0x7f;
			val2 = ((bbp_val2 & (0x00007f00)) >> 8) & 0x7f;
		} else {
			val1 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0;
			val2 = pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1;
		}
		periodic_monitor_false_cca_adjust_init_vga(pAd, &val1, &val2);
		/* agc vga */
		bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
		bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
		/* agc vga */
		bbp_val1 &= ~(0xffff << 16);
		bbp_val2 &= ~(0xffff << 16);
		if (is_g_band) {	/* 2G band */
			if (is_elna_g_band) {	/* eLNA */
				bbp_val1 |= ((0x1830) << 16);
				bbp_val2 |= ((0x1830) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1e42) << 16);
				bbp_val2 |= ((0x1e42) << 16);
			}
		} else {	/* 5G band */

			if (is_elna_a_band) {	/* eLNA */
				bbp_val1 |= ((0x2232) << 16);
				bbp_val2 |= ((0x2232) << 16);
			} else {	/* iLNA */
				bbp_val1 |= ((0x1e42) << 16);
				bbp_val2 |= ((0x1e42) << 16);
			}
		}
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
		RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
		previous_rssi_in_75_range = 1;
		/* Old algorithm
		   periodic_monitor_false_cca_adjust_vga(pAd);
		   bbp_val1 &= ~(0xffff << 16);
		   bbp_val1 |= ((pAd->CommonCfg.lna_vga_ctl.agc_0_vga_set1_2 & 0xffff) << 16);
		   bbp_val2 &= ~(0xffff << 16);
		   bbp_val2 |= ((pAd->CommonCfg.lna_vga_ctl.agc_1_vga_set1_2 & 0xffff) << 16);
		 */
	}
	/* DBGPRINT(RT_DEBUG_TRACE, ("Rssi=%d,new_bbpval1=0x%08x, new_bbpval2=0x%08x\n", Rssi, bbp_val1, bbp_val2)); */
}
#else
/* !defined(HOST_DYNAMIC_VGA_SUPPORT) */
#ifdef CONFIG_STA_SUPPORT
void periodic_monitor_rssi_adjust_vga(RTMP_ADAPTER *pAd)
{
	if ((pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable) &&
	    (pAd->chipCap.dynamic_vga_support) && INFRA_ON(pAd)) {
		UINT32 bbp_val1 = 0, bbp_val2 = 0;

		andes_dynamic_vga(pAd, pAd->CommonCfg.Channel, FALSE, FALSE,
				  pAd->chipCap.avg_rssi_all, pAd->RalinkCounters.OneSecFalseCCACnt);

		RTMP_BBP_IO_READ32(pAd, AGC1_R8, &bbp_val1);
		RTMP_BBP_IO_READ32(pAd, AGC1_R9, &bbp_val2);

		DBGPRINT(RT_DEBUG_INFO, ("%s::0x2320=0x%08x, 0x2324=0x%08x\n",
					 __func__, bbp_val1, bbp_val2));
	}
}
#endif /* CONFIG_STA_SUPPORT */
#endif /* HOST_DYNAMIC_VGA_SUPPORT */

void update_rssi_for_channel_model(RTMP_ADAPTER *pAd)
{
	INT32 rx0_rssi = 0, rx1_rssi = 0;

#ifdef ATE_DYNAMIC_VGA_SUPPORT
	if (ATE_ON(pAd)) {
		rx0_rssi = (CHAR) (pAd->ate.LastRssi0);
		rx1_rssi = (CHAR) (pAd->ate.LastRssi1);
	} else
#endif /* ATE_DYNAMIC_VGA_SUPPORT*/
	{
#ifdef CONFIG_STA_SUPPORT
		if (INFRA_ON(pAd)) {
			rx0_rssi = (CHAR) (pAd->StaCfg.RssiSample.LastRssi0);
			rx1_rssi = (CHAR) (pAd->StaCfg.RssiSample.LastRssi1);
		}
#endif /* CONFIG_STA_SUPPORT */
#ifdef CONFIG_AP_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
		/* Only when P2P GO is on && Infra is not connected, we start the dynamic VGA for GO */
		if (!pAd->CommonCfg.DynamicVGA_Disabled && RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
#else
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
#endif /* endif */
		{
			rx0_rssi = (CHAR) (pAd->ApCfg.RssiSample.LastRssi0);
			rx1_rssi = (CHAR) (pAd->ApCfg.RssiSample.LastRssi1);
		}
#endif /* CONFIG_AP_SUPPORT */
	}

	DBGPRINT(RT_DEBUG_INFO, ("%s:: rx0_rssi(%d), rx1_rssi(%d)\n",
				 __func__, rx0_rssi, rx1_rssi));

	/*
	   RSSI_DUT(n) = RSSI_DUT(n-1)*15/16 + RSSI_R2320_100ms_sample*1/16
	 */
	pAd->chipCap.avg_rssi_0 = ((pAd->chipCap.avg_rssi_0) * 15) / 16 + (rx0_rssi << 8) / 16;
	pAd->chipCap.avg_rssi_1 = ((pAd->chipCap.avg_rssi_1) * 15) / 16 + (rx1_rssi << 8) / 16;
	pAd->chipCap.avg_rssi_all = (pAd->chipCap.avg_rssi_0 + pAd->chipCap.avg_rssi_1) / 512;

	DBGPRINT(RT_DEBUG_INFO, ("%s:: update rssi all(%d)\n",
				 __func__, pAd->chipCap.avg_rssi_all));
}

void dynamic_ed_cca_threshold_adjust(RTMP_ADAPTER *pAd)
{
	UINT32 reg_val = 0;
	CHAR high_gain = 0, mid_gain = 0, low_gain = 0, ulow_gain = 0, lna_gain_mode = 0;
	UCHAR lna_gain = 0, vga_gain = 0, y = 0, z = 0;

	RTMP_BBP_IO_READ32(pAd, AGC1_R4, &reg_val);
	high_gain = ((reg_val & (0x3F0000)) >> 16) & 0x3F;
	mid_gain = ((reg_val & (0x3F00)) >> 8) & 0x3F;
	low_gain = reg_val & 0x3F;

	RTMP_BBP_IO_READ32(pAd, AGC1_R6, &reg_val);
	ulow_gain = reg_val & 0x3F;

	RTMP_BBP_IO_READ32(pAd, AGC1_R8, &reg_val);
	lna_gain_mode = ((reg_val & 0xC0) >> 6) & 0x3;
	vga_gain = ((reg_val & 0x7E00) >> 9) & 0x3F;

	if (lna_gain_mode == 0)
		lna_gain = ulow_gain;
	else if (lna_gain_mode == 1)
		lna_gain = low_gain;
	else if (lna_gain_mode == 2)
		lna_gain = mid_gain;
	else if (lna_gain_mode == 3)
		lna_gain = high_gain;

	if ((vga_gain + lna_gain) > 64)
		y = ((vga_gain + lna_gain) - 64) / 3;
	else
		y = 0;

	if (y > 1)
		z = min((1 << (y - 2)), 14);
	else
		z = 1;

	RTMP_BBP_IO_READ32(pAd, AGC1_R2, &reg_val);
	reg_val = (reg_val & 0xFFFF0000) | (z << 8) | z;
	RTMP_BBP_IO_WRITE32(pAd, AGC1_R2, reg_val);

	DBGPRINT(RT_DEBUG_INFO,
		 ("%s:: lna_gain(%d), vga_gain(%d), lna_gain_mode(%d), y=%d, z=%d, 0x2308=0x%08x\n",
		  __func__, lna_gain, vga_gain, lna_gain_mode, y, z, reg_val));
}

void dynamic_cck_mrc(RTMP_ADAPTER *pAd)
{
	UINT32 bbp_val = 0;

	/* CCK MRC PER bump at larger power ~-30dBm */
	if (pAd->CommonCfg.RxStream >= 2) {
		if (pAd->chipCap.avg_rssi_all > -70) {
			RTMP_BBP_IO_READ32(pAd, AGC1_R30, &bbp_val);
			bbp_val &= 0xfffffffb;	/* disable CCK MRC */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R30, bbp_val);
		} else if (pAd->chipCap.avg_rssi_all < -80) {
			RTMP_BBP_IO_READ32(pAd, AGC1_R30, &bbp_val);
			bbp_val = (bbp_val & 0xfffffffb) | (1 << 2);	/* enable CCK MRC */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R30, bbp_val);
		}
	}
}

#define shift_left16(x)			((x) << 16)
#define shift_left8(x)			((x) << 8)

BOOLEAN dynamic_channel_model_adjust(RTMP_ADAPTER *pAd)
{
	UCHAR mode = 0, default_init_vga = 0, eLNA_init_vga = 0, iLNA_init_vga = 0;
	UINT32 value = 0;
	BOOLEAN no_dynamic_vga = FALSE;

	/* dynamic_chE_mode:
	   bit7         0:average RSSI <= threshold     1:average RSSI > threshold
	   bit4:6       0:BW_20         1:BW_40         2:BW_80         3~7:Reserved
	   bit1:3       Reserved
	   bit0         0: eLNA         1: iLNA
	 */

	if (((pAd->chipCap.avg_rssi_all > -62) && (pAd->CommonCfg.BBPCurrentBW == BW_80))
	    || ((pAd->chipCap.avg_rssi_all > -65) && (pAd->CommonCfg.BBPCurrentBW == BW_40))
	    || ((pAd->chipCap.avg_rssi_all > -68) && (pAd->CommonCfg.BBPCurrentBW == BW_20))) {
		RTMP_BBP_IO_WRITE32(pAd, RXO_R18, 0xF000A990);
		if (pAd->CommonCfg.BBPCurrentBW == BW_80) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0xA0;	/* BW80::eLNA lower VGA/PD */
			else
				mode = 0xA1;	/* BW80::iLNA lower VGA/PD */

			RTMP_BBP_IO_READ32(pAd, AGC1_R26, &value);
			value = (value & ~0xF) | 0x3;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R26, value);
		} else if (pAd->CommonCfg.BBPCurrentBW == BW_40) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0x90;	/* BW40::eLNA lower VGA/PD */
			else
				mode = 0x91;	/* BW40::iLNA lower VGA/PD */
		} else if (pAd->CommonCfg.BBPCurrentBW == BW_20) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0x80;	/* BW20::eLNA lower VGA/PD */
			else
				mode = 0x81;	/* BW20::iLNA lower VGA/PD */
		}
	} else {
		RTMP_BBP_IO_WRITE32(pAd, RXO_R18, 0xF000A991);
		if (pAd->CommonCfg.BBPCurrentBW == BW_80) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0x20;	/* BW80::eLNA default */
			else
				mode = 0x21;	/* BW80::iLNA default */

			RTMP_BBP_IO_READ32(pAd, AGC1_R26, &value);
			value = (value & ~0xF) | 0x5;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R26, value);
		} else if (pAd->CommonCfg.BBPCurrentBW == BW_40) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0x10;	/* BW40::eLNA default */
			else
				mode = 0x11;	/* BW40::iLNA default */
		} else if (pAd->CommonCfg.BBPCurrentBW == BW_20) {
			if (is_external_lna_mode(pAd, pAd->CommonCfg.Channel))
				mode = 0x00;	/* BW20::eLNA default */
			else
				mode = 0x01;	/* BW20::iLNA default */
		}
	}

	DBGPRINT(RT_DEBUG_INFO, ("%s:: dynamic ChE mode(0x%x)\n", __func__, mode));

	if (((pAd->chipCap.avg_rssi_all <= -76) && (pAd->CommonCfg.BBPCurrentBW == BW_80))
	    || ((pAd->chipCap.avg_rssi_all <= -79) && (pAd->CommonCfg.BBPCurrentBW == BW_40))
	    || ((pAd->chipCap.avg_rssi_all <= -82) && (pAd->CommonCfg.BBPCurrentBW == BW_20)))
		no_dynamic_vga = TRUE;

	if (((mode & 0xFF) != pAd->chipCap.dynamic_chE_mode) || no_dynamic_vga) {
		pAd->chipCap.dynamic_chE_trigger = TRUE;
		default_init_vga = pAd->CommonCfg.lna_vga_ctl.agc_vga_ori_0;
		eLNA_init_vga = pAd->CommonCfg.lna_vga_ctl.agc_vga_ori_0 - 10;
		iLNA_init_vga = pAd->CommonCfg.lna_vga_ctl.agc_vga_ori_0 - 14;

		/* the following has to be done by firmware,thus it is a temporary way to support this */
		if (pAd->CommonCfg.BBPCurrentBW == BW_80)
			RTMP_BBP_IO_WRITE32(pAd, RXO_R14, 0x00560411);
		else
			RTMP_BBP_IO_WRITE32(pAd, RXO_R14, 0x00560423);

		switch (mode & 0xFF) {
		case 0xA0:	/* BW80::eLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0xA0;

			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(eLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0xA1:	/* BW80::iLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0xA1;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(iLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x90:	/* BW40::eLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0x90;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(eLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x91:	/* BW40::iLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0x91;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(iLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x80:	/* BW20::eLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0x80;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1836) | shift_left8(eLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x81:	/* BW20::iLNA lower VGA/PD */
			pAd->chipCap.dynamic_chE_mode = 0x81;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x08080808);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x08080808);	/* BBP 0x2394 */
			value = shift_left16(0x1836) | shift_left8(iLNA_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x20:	/* BW80::eLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x20;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161c);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x21:	/* BW80::iLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x21;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161c);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x10:	/* BW40::eLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x10;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161c);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x11:	/* BW40::iLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x11;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161c);	/* BBP 0x2394 */
			value = shift_left16(0x1E42) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x00:	/* BW20::eLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x00;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161C);	/* BBP 0x2394 */
			value = shift_left16(0x1836) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;

		case 0x01:	/* BW20::iLNA default */
			pAd->chipCap.dynamic_chE_mode = 0x01;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R35, 0x11111116);	/* BBP 0x238C */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R37, 0x1010161C);	/* BBP 0x2394 */
			value = shift_left16(0x1836) | shift_left8(default_init_vga) | 0xF8;
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, value);	/* BBP 0x2320 */
			RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, value);	/* BBP 0x2324 */
			break;
		}

		DBGPRINT(RT_DEBUG_INFO,
			 ("%s:: updated dynamic_chE_mode(0x%x), dynamic_chE_trigger(%d)\n",
			  __func__, pAd->chipCap.dynamic_chE_mode,
			  pAd->chipCap.dynamic_chE_trigger));
	} else
		pAd->chipCap.dynamic_chE_trigger = FALSE;

	return no_dynamic_vga;
}

#if !defined(HOST_DYNAMIC_VGA_SUPPORT)
void periodic_monitor_false_cca_adjust_vga(RTMP_ADAPTER *pAd)
{
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		if ((pAd->CommonCfg.lna_vga_ctl.bDyncVgaEnable) &&
		    (pAd->MacTab.Size > 0) &&
		    (pAd->chipCap.dynamic_vga_support) &&
		    OPSTATUS_TEST_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED)) {
			UCHAR val1, val2;
			UINT32 bbp_val1, bbp_val2;

			if (dynamic_channel_model_adjust(pAd) == TRUE) {
				DBGPRINT(RT_DEBUG_INFO,
					 ("%s:: no need to do dynamic vga\n", __func__));
				return;
			}

			if (pAd->chipCap.dynamic_chE_trigger == TRUE) {
				mt76x2_get_agc_gain(pAd, FALSE);	/* real time update init values */
				bbp_val1 = pAd->CommonCfg.lna_vga_ctl.agc1_r8_backup;
				val1 =
				    ((((bbp_val1 & (0x00007f00)) >> 8) & 0x7f) -
				     pAd->chipCap.compensate_level);
				bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
				RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);

				bbp_val2 = pAd->CommonCfg.lna_vga_ctl.agc1_r9_backup;
				val2 =
				    ((((bbp_val2 & (0x00007f00)) >> 8) & 0x7f) -
				     pAd->chipCap.compensate_level);
				bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
				RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
			} else {
				RTMP_BBP_IO_READ32(pAd, AGC1_R8, &bbp_val1);
				val1 = ((bbp_val1 & (0x00007f00)) >> 8) & 0x7f;
				RTMP_BBP_IO_READ32(pAd, AGC1_R9, &bbp_val2);
				val2 = ((bbp_val2 & (0x00007f00)) >> 8) & 0x7f;
			}

			DBGPRINT(RT_DEBUG_INFO,
				 ("vga_init_0 = %x, vga_init_1 = %x\n",
				  pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0,
				  pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1));
			DBGPRINT(RT_DEBUG_INFO,
				 ("ori AGC1_R8 = %x, ori AGC1_R9 = %x\n",
				  pAd->CommonCfg.lna_vga_ctl.agc1_r8_backup,
				  pAd->CommonCfg.lna_vga_ctl.agc1_r9_backup));
			DBGPRINT(RT_DEBUG_INFO,
				 ("one second False CCA=%d, fixed agc_vga_0:0%x, fixed agc_vga_1:0%x\n",
				  pAd->RalinkCounters.OneSecFalseCCACnt, val1, val2));
			DBGPRINT(RT_DEBUG_INFO,
				 ("compensate level = %x\n", pAd->chipCap.compensate_level));

			if (pAd->RalinkCounters.OneSecFalseCCACnt >
			    pAd->CommonCfg.lna_vga_ctl.nFalseCCATh) {
				if (val1 > (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0 - 0x10)) {
					val1 -= 0x02;
					pAd->chipCap.compensate_level += 0x02;
					if (pAd->chipCap.compensate_level >= 0x10)
						pAd->chipCap.compensate_level = 0x10;
					bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
					RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
#ifdef DFS_SUPPORT
					pAd->CommonCfg.RadarDetect.bAdjustDfsAgc = TRUE;
#endif /* endif */
				}

				if (pAd->CommonCfg.RxStream >= 2) {
					if (val2 >
					    (pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1 - 0x10)) {
						val2 -= 0x02;
						bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
						RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
					}
				}
			} else if (pAd->RalinkCounters.OneSecFalseCCACnt <
				   pAd->CommonCfg.lna_vga_ctl.nLowFalseCCATh) {
				if (val1 < pAd->CommonCfg.lna_vga_ctl.agc_vga_init_0) {
					val1 += 0x02;
					pAd->chipCap.compensate_level -= 0x02;
					if (pAd->chipCap.compensate_level < 0)
						pAd->chipCap.compensate_level = 0;
					bbp_val1 = (bbp_val1 & 0xffff80ff) | (val1 << 8);
					RTMP_BBP_IO_WRITE32(pAd, AGC1_R8, bbp_val1);
#ifdef DFS_SUPPORT
					pAd->CommonCfg.RadarDetect.bAdjustDfsAgc = TRUE;
#endif /* endif */
				}

				if (pAd->CommonCfg.RxStream >= 2) {
					if (val2 < pAd->CommonCfg.lna_vga_ctl.agc_vga_init_1) {
						val2 += 0x02;
						bbp_val2 = (bbp_val2 & 0xffff80ff) | (val2 << 8);
						RTMP_BBP_IO_WRITE32(pAd, AGC1_R9, bbp_val2);
					}
				}
			}
		}
	}
}
#endif /* !defined(HOST_DYNAMIC_VGA_SUPPORT) */
#endif /* DYNAMIC_VGA_SUPPORT */

static const RTMP_CHIP_CAP MT76x2_ChipCap = {
	.max_nss = 2,
#ifdef DOT11_VHT_AC
	.max_vht_mcs = VHT_MCS_CAP_9,
	.ac_off_mode = 0,
#endif /* DOT11_VHT_AC */
	.TXWISize = 20,
	.RXWISize = 28,
#ifdef RTMP_MAC_PCI
	.WPDMABurstSIZE = 3,
#endif /* RTMP_MAC_PCI */
	.SnrFormula = SNR_FORMULA3,
	.FlgIsHwWapiSup = TRUE,
	.VcoPeriod = 10,
	.FlgIsVcoReCalMode = VCO_CAL_DISABLE,
	.FlgIsHwAntennaDiversitySup = FALSE,
	.Flg7662ChipCap = TRUE,
#ifdef STREAM_MODE_SUPPORT
	.FlgHwStreamMode = FALSE,
#endif /* STREAM_MODE_SUPPORT */
#ifdef TXBF_SUPPORT
	.FlgHwTxBfCap = TRUE,
	.FlgITxBfBinWrite = FALSE,
#endif /* TXBF_SUPPORT */
#ifdef FIFO_EXT_SUPPORT
	.FlgHwFifoExtCap = TRUE,
#endif /* FIFO_EXT_SUPPORT */
	.asic_caps = (fASIC_CAP_PMF_ENC | fASIC_CAP_MCS_LUT),
	.phy_caps = (fPHY_CAP_24G | fPHY_CAP_5G | fPHY_CAP_HT | fPHY_CAP_VHT | fPHY_CAP_LDPC),
	.MaxNumOfRfId = MAX_RF_ID,
	.pRFRegTable = NULL,
	.MaxNumOfBbpId = 200,
	.pBBPRegTable = NULL,
	.bbpRegTbSize = 0,
#ifdef DFS_SUPPORT
	.DfsEngineNum = 4,
#endif /* DFS_SUPPORT */
#ifdef CARRIER_DETECTION_SUPPORT
	.carrier_func = TONE_RADAR_V3,
#endif /* CARRIER_DETECTION_SUPPORT */
#ifdef NEW_MBSSID_MODE
#ifdef ENHANCE_NEW_MBSSID_MODE
	.MBSSIDMode = MBSSID_MODE4,
#else
	.MBSSIDMode = MBSSID_MODE1,
#endif /* ENHANCE_NEW_MBSSID_MODE */
#else
	.MBSSIDMode = MBSSID_MODE0,
#endif /* NEW_MBSSID_MODE */
#ifdef RTMP_EFUSE_SUPPORT
	.EFUSE_USAGE_MAP_START = 0x1e0,
	.EFUSE_USAGE_MAP_END = 0x1fc,
	.EFUSE_USAGE_MAP_SIZE = 29,
	.EFUSE_RESERVED_SIZE = 21,	/* Cal-Free is 22 free block */
#endif /* RTMP_EFUSE_SUPPORT */
	.EEPROM_DEFAULT_BIN = MT76x2_E2PImage,
	.EEPROM_DEFAULT_BIN_SIZE = sizeof(MT76x2_E2PImage),
#ifdef CONFIG_ANDES_SUPPORT
	.WlanMemmapOffset = 0x410000,
	.InbandPacketMaxLen = 192,
	.CmdRspRxRing = RX_RING1,
	.IsComboChip = TRUE,
	.need_load_fw = TRUE,
	.need_load_rom_patch = TRUE,
	.ram_code_protect = FALSE,
	.rom_code_protect = TRUE,
	.load_iv = FALSE,
	.ilm_offset = 0x80000,
	.dlm_offset = 0x110000,
	.rom_patch_offset = 0x90000,
#endif /* CONFIG_ANDES_SUPPORT */
	.MCUType = ANDES,
	.cmd_header_len = 4,
#ifdef RTMP_PCI_SUPPORT
	.cmd_padding_len = 0,
#endif /* endif */
#ifdef RTMP_USB_SUPPORT
	.cmd_padding_len = 4,
	.CommandBulkOutAddr = 0x8,
	.WMM0ACBulkOutAddr[0] = 0x4,
	.WMM0ACBulkOutAddr[1] = 0x5,
	.WMM0ACBulkOutAddr[2] = 0x6,
	.WMM0ACBulkOutAddr[3] = 0x7,
	.WMM1ACBulkOutAddr = 0x9,
	.DataBulkInAddr = 0x84,
	.CommandRspBulkInAddr = 0x85,
#endif /* endif */
#ifdef FW_LOAD_FROM_BIN
	.load_code_method = BIN_FILE_METHOD,

	.rom_patch_header_image = NULL,
	.rom_patch_len = 0,
	.rom_patch_bin_file_name = "mt7662_patch_e3_hdr.bin",

	.fw_header_image = NULL,
	.fw_len = 0,
	.fw_bin_file_name = "mt7662_firmware_e3_tvbox.bin",
#else
	.load_code_method = HEADER_METHOD,

	.fw_header_image = MT7662_FirmwareImage,
	.fw_len = sizeof(MT7662_FirmwareImage),
	.rom_patch_header_image = mt7662_rom_patch,
	.rom_patch_len = sizeof(mt7662_rom_patch),
#endif /* FW_LOAD_FROM_BIN */

	/* Non-support EDCCA update on tvbox firmware */
	.fw_non_support = fNONSUPPORT_EDCCA_UPDATE,
	.MACRegisterVer = "",
	.BBPRegisterVer = "MT7662E1_BBP_CR_20130116.xls",
	.RFRegisterVer = "",
#ifdef CONFIG_WIFI_TEST
	.MemMapStart = 0x0000,
	.MemMapEnd = 0xffff,
	.MacMemMapOffset = 0x1000,
	.MacStart = 0x0000,
	.MacEnd = 0x0600,
	.BBPMemMapOffset = 0x2000,
	.BBPStart = 0x0000,
	.BBPEnd = 0x0f00,
	.RFIndexNum = sizeof(mt76x2_rf_index_offset) / sizeof(struct RF_INDEX_OFFSET),
	.RFIndexOffset = mt76x2_rf_index_offset,
	.E2PStart = 0x0000,
	.E2PEnd = 0x00fe,
#endif /* endif */
	.hif_type = HIF_RLT,
	.rf_type = RF_MT,
#ifdef DYNAMIC_VGA_SUPPORT
	.dynamic_vga_support = TRUE,
	.compensate_level = 0,
	.dynamic_chE_mode = 0xFF,
	.dynamic_chE_trigger = FALSE,
	.avg_rssi_all = -90,
	.avg_rssi_0 = -90,
	.avg_rssi_1 = -90,
#ifdef CONFIG_AP_SUPPORT
	.dynamic_lna_trigger_timer = 1,
	.microwave_enable = TRUE,
#endif /* CONFIG_AP_SUPPORT */
#endif /* DYNAMIC_VGA_SUPPORT */
	.chl_smth_enable = TRUE,
	.ed_cca_enable = FALSE,
#ifdef DOT11W_PMF_SUPPORT
	.FlgPMFEncrtptMode = PMF_ENCRYPT_MODE_1,
#endif /* DOT11W_PMF_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
#ifdef RTMP_FREQ_CALIBRATION_SUPPORT
	/* Frequence Calibration */
	.FreqCalibrationSupport = TRUE,
	/* BBP CR for Rx OFDM/CCK frequency offset report is unnecessary */
	.FreqCalInitMode = FREQ_CAL_INIT_UNKNOW,
	.FreqCalMode = FREQ_CAL_MODE2,
	.RxWIFrqOffset = RXWI_FRQ_OFFSET_FIELD0,
#endif /* RTMP_FREQ_CALIBRATION_SUPPORT */
#endif /* CONFIG_STA_SUPPORT */
#ifdef THERMAL_PROTECT_SUPPORT
	.ThermalProtectSup = TRUE,
#endif /* THERMAL_PROTECT_SUPPORT */
	.default_profile = MT76x2_DEFAULT_PROFILE,
};

static const RTMP_CHIP_OP MT76x2_ChipOp = {
	.ChipBBPAdjust = mt76x2_bbp_adjust,
	.ChipSwitchChannel = mt76x2_switch_channel,
	.AsicMacInit = mt76x2_init_mac_cr,
	.AsicRfInit = mt76x2_init_rf_cr,
	.AsicAntennaDefaultReset = mt76x2_antenna_default_reset,

	/* ITxBf phase calibration */
#ifdef TXBF_SUPPORT
	.fITxBfDividerCalibration = ITxBFDividerCalibrationStartUp,
	.fITxBfLNAPhaseCompensate = mt76x2_ITxBFLoadLNAComp,
	.fITxBfCal = mt76x2_Set_ITxBfCal_Proc,
	.fITxBfLNACalibration = ITxBFLNACalibrationStartUp,
#endif /* endif */

	.ChipAGCInit = NULL,
#ifdef CONFIG_STA_SUPPORT
	.ChipAGCAdjust = NULL,
#endif /* endif */
	.AsicRfTurnOn = NULL,
	.AsicHaltAction = NULL,
	.AsicRfTurnOff = NULL,
	.AsicReverseRfFromSleepMode = NULL,
	.AsicResetBbpAgent = NULL,
	.InitDesiredTSSITable = NULL,
	.ATETssiCalibration = NULL,
	.ATETssiCalibrationExtend = NULL,
	.AsicTxAlcGetAutoAgcOffset = NULL,
	.ATEReadExternalTSSI = NULL,
	.TSSIRatio = NULL,
#ifdef CONFIG_STA_SUPPORT
	.NetDevNickNameInit = mt76x2_init_dev_nick_name,
#endif /* endif */
#ifdef CARRIER_DETECTION_SUPPORT
	.ToneRadarProgram = ToneRadarProgram_v3,
#endif /* endif */
	.RxSensitivityTuning = NULL,
	.DisableTxRx = RT65xxDisableTxRx,
#ifdef RTMP_USB_SUPPORT
	.AsicRadioOn = RT65xxUsbAsicRadioOn,
	.AsicRadioOff = RT65xxUsbAsicRadioOff,
	.usb_cfg_read = usb_cfg_read_v3,
	.usb_cfg_write = usb_cfg_write_v3,
#endif /* endif */
#ifdef RTMP_PCI_SUPPORT
	.AsicRadioOn = RT28xxPciAsicRadioOn,
	.AsicRadioOff = RT28xxPciAsicRadioOff,
#endif /* endif */
#ifdef CAL_FREE_IC_SUPPORT
	.is_cal_free_ic = mt76x2_is_cal_free_ic,
	.cal_free_data_get = mt76x2_cal_free_data_get,
#endif /* CAL_FREE_IC_SUPPORT */
	.show_pwr_info = mt76x2_show_pwr_info,
	.cal_test = mt76x2_cal_test,
#ifdef RTMP_TEMPERATURE_TX_ALC
	.TemperCompensation = mt76x2_temp_tx_alc,
#endif /* RTMP_TEMPERATURE_TX_ALC */
#ifdef ED_MONITOR
	.ChipSetEDCCA = mt7612_set_ed_cca,
#else
	.ChipSetEDCCA = NULL,
#endif /* ED_MONITOR */
#ifdef DYNAMIC_VGA_SUPPORT
#ifdef HOST_DYNAMIC_VGA_SUPPORT
	.AsicDynamicVgaGainControl = host_periodic_monitor_rssi_adjust_vga,
#else
/* !defined(HOST_DYNAMIC_VGA_SUPPORT) */
#ifdef CONFIG_AP_SUPPORT
	.AsicDynamicVgaGainControl = periodic_monitor_false_cca_adjust_vga,
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
	.AsicDynamicVgaGainControl = periodic_monitor_rssi_adjust_vga,
#endif /* CONFIG_STA_SUPPORT */
#endif /* HOST_DYNAMIC_VGA_SUPPORT */
#endif /* DYNAMIC_VGA_SUPPORT */
};

VOID mt76x2_init(RTMP_ADAPTER *pAd)
{
	RTMP_CHIP_CAP *pChipCap = &pAd->chipCap;

	memcpy(&pAd->chipCap, &MT76x2_ChipCap, sizeof(RTMP_CHIP_CAP));
	memcpy(&pAd->chipOps, &MT76x2_ChipOp, sizeof(RTMP_CHIP_OP));

	rlt_phy_probe(pAd);

	if (IS_MT7612(pAd))
		pChipCap->IsComboChip = FALSE;

	if (IS_MT7632(pAd))
		pChipCap->phy_caps = (fPHY_CAP_24G | fPHY_CAP_5G | fPHY_CAP_HT | fPHY_CAP_LDPC);

	if (IS_MT76x2T(pAd)) {
		DBGPRINT(RT_DEBUG_OFF, ("MT76x2T chip!\n"));
		pChipCap->rom_patch_offset = 0xBC000;
		pChipCap->dlm_offset = 0x115800;

#ifdef FW_LOAD_FROM_BIN
		pChipCap->load_code_method = BIN_FILE_METHOD,

		pChipCap->rom_patch_bin_file_name = "mt7662t_patch_e1_hdr.bin";
		pChipCap->rom_patch_header_image = NULL;
		pChipCap->rom_patch_len = 0,

		pChipCap->fw_bin_file_name = "mt7662t_firmware_e1.bin";
		pChipCap->fw_header_image = NULL;
		pChipCap->fw_len = 0;
#else
		pChipCap->load_code_method = HEADER_METHOD,

		pChipCap->fw_header_image = MT7662T_FirmwareImage;
		pChipCap->fw_len = sizeof(MT7662T_FirmwareImage);

		pChipCap->rom_patch_header_image = mt7662t_rom_patch;
		pChipCap->rom_patch_len = sizeof(mt7662t_rom_patch);
#endif /* FW_LOAD_FROM_BIN */
	}

#ifdef DOT11_VHT_AC
	{
		UINT32 mac_val = 0;

		RTMP_IO_READ32(pAd, 0x38, &mac_val);

		if ((mac_val & 0x80000) == 0x80000)
			pChipCap->ac_off_mode = 1;

	}
#endif /* DOT11_VHT_AC */

	    pChipCap->asic_caps |= (fASIC_CAP_MCS_LUT);

	RTMP_DRS_ALG_INIT(pAd, RATE_ALG_GRP);

	rlt_bcn_buf_init(pAd);

#ifdef HDR_TRANS_SUPPORT
	UINT8 cnt = HT_RX_WCID_SIZE / HT_RX_WCID_OFFSET;
	UINT32 RegVal;

	/* enable TX/RX Header Translation */
	RTMP_IO_WRITE32(pAd, HT_RX_WCID_EN_BASE, 0xFF);	/* all RX WCID enable */

	/* black list - skip EAP-888e/DLS-890d */
	RTMP_IO_WRITE32(pAd, HT_RX_BL_BASE, 0x888e890d);
#endif /* HDR_TRANS_SUPPORT */

	pChipCap->tssi_stage = TSSI_INIT_STAGE;
}

static void patch_BBPL_on(RTMP_ADAPTER *pAd)
{
	UINT32 value = 0;
    
	read_reg(pAd, 0x40, 0x130, &value);
    value |= ((1<<16) | (1<<0));
    write_reg(pAd, 0x40, 0x130, value);
    
    RtmpusecDelay(1); 
    
    read_reg(pAd, 0x40, 0x64, &value);
    if ((value >> 29) & 0x1)
    {
        read_reg(pAd, 0x40, 0x1c, &value);
    	value &= 0xFFFFFF00;
    	write_reg(pAd, 0x40, 0x1c, value);
    	
        read_reg(pAd, 0x40, 0x1c, &value);
    	value |= 0x30;
    	write_reg(pAd, 0x40, 0x1c, value);
    }
    else
    {
        read_reg(pAd, 0x40, 0x1c, &value);
    	value &= 0xFFFFFF00;
    	write_reg(pAd, 0x40, 0x1c, value);
    	
        read_reg(pAd, 0x40, 0x1c, &value);
    	value |= 0x30;
    	write_reg(pAd, 0x40, 0x1c, value);
    }
    value = 0x0000484F;
    write_reg(pAd, 0x40, 0x14, value);
    	
    RtmpusecDelay(1); 
    
    read_reg(pAd, 0x40, 0x130, &value);
    value |= (1<<17);
    write_reg(pAd, 0x40, 0x130, value);
    
    RtmpusecDelay(125); 
    
    read_reg(pAd, 0x40, 0x130, &value);
    value  &= ~(1<<16);
    write_reg(pAd, 0x40, 0x130, value);
    
    RtmpusecDelay(50); 
    
    read_reg(pAd, 0x40, 0x14C, &value);
    value  |= ((1<<20) | (1<<19));
    write_reg(pAd, 0x40, 0x14C, value);
}

static VOID WF_CTRL(RTMP_ADAPTER *pAd, UINT8 wfID, UINT8 isON)
{
	UINT32 value = 0;

	if (wfID == 0) {
		if (isON == 1) {	/* WIFI ON mode */
			/* Enable WF0 BG */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 0);
			write_reg(pAd, 0x40, 0x130, value);

			RtmpusecDelay(10);

			/* Enable RFDIG LDO/AFE/ABB/ADDA */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= ((1 << 1) | (1 << 3) | (1 << 4) | (1 << 5));
			write_reg(pAd, 0x40, 0x130, value);

			RtmpusecDelay(10);

			/* Switch WF0 RFDIG power to internal LDO */
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~(1 << 2);
			write_reg(pAd, 0x40, 0x130, value);

			patch_BBPL_on(pAd);

			RTMP_IO_READ32(pAd, 0x530, &value);
			value |= 0xF;
			RTMP_IO_WRITE32(pAd, 0x530, value);
		} else {
			/* Disable WF0 BG */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 2);	/* Switch WF0 RFDIG power to CLDO */
			write_reg(pAd, 0x40, 0x130, value);
			RtmpusecDelay(5);
			RTMP_IO_READ32(pAd, 0x530, &value);
			value &= ~(0xF);
			RTMP_IO_WRITE32(pAd, 0x530, value);
			/* BBPL off */
			read_reg(pAd, 0x40, 0x14c, &value);
			value &= ~((1 << 19) | (1 << 20));
			write_reg(pAd, 0x40, 0x14c, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~((1 << 16) | (1 << 17));
			write_reg(pAd, 0x40, 0x130, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 0);
			write_reg(pAd, 0x40, 0x130, value);
			/* BBPL off done */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 0);
			write_reg(pAd, 0x40, 0x130, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~((1 << 1) | (1 << 3) | (1 << 4) | (1 << 5));
			write_reg(pAd, 0x40, 0x130, value);
		}
	} else {
		if (isON == 1) {	/* WIFI ON mode */
			/* Enable WF1 BG */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 8);
			write_reg(pAd, 0x40, 0x130, value);

			RtmpusecDelay(10);

			/* Enable RFDIG LDO/AFE/ABB/ADDA */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= ((1 << 9) | (1 << 11) | (1 << 12) | (1 << 13));
			write_reg(pAd, 0x40, 0x130, value);

			RtmpusecDelay(10);
			/* Switch WF1 RFDIG power to internal LDO */
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~(1 << 10);
			write_reg(pAd, 0x40, 0x130, value);

			patch_BBPL_on(pAd);

			RTMP_IO_READ32(pAd, 0x530, &value);
			value |= 0xF;
			RTMP_IO_WRITE32(pAd, 0x530, value);
		} else {
			/* Disable WF1 BG */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 10);	/* Switch WF0 RFDIG power to CLDO */
			write_reg(pAd, 0x40, 0x130, value);
			RtmpusecDelay(5);
			RTMP_IO_READ32(pAd, 0x530, &value);
			value &= ~(0xF);
			RTMP_IO_WRITE32(pAd, 0x530, value);
			/* BBPL off */
			read_reg(pAd, 0x40, 0x14c, &value);
			value &= ~((1 << 19) | (1 << 20));
			write_reg(pAd, 0x40, 0x14c, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~((1 << 16) | (1 << 17));
			write_reg(pAd, 0x40, 0x130, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 0);
			write_reg(pAd, 0x40, 0x130, value);
			/* BBPL off done */
			read_reg(pAd, 0x40, 0x130, &value);
			value |= (1 << 8);
			write_reg(pAd, 0x40, 0x130, value);
			read_reg(pAd, 0x40, 0x130, &value);
			value &= ~((1 << 9) | (1 << 11) | (1 << 12) | (1 << 13));
			write_reg(pAd, 0x40, 0x130, value);
		}
	}
}

static void WL_POWER_ON(RTMP_ADAPTER *pAd)
{
	UINT32 cnt = 0;
	UINT32 regval = 0;
	UINT32 value = 0;

	read_reg(pAd, 0x40, 0x148, &value);
	value |= 0x1;
	write_reg(pAd, 0x40, 0x148, value);	/* turn on WL MTCMOS */
	do {
		read_reg(pAd, 0x40, 0x148, &regval);
		if ((((regval >> 28) & 0x1) == 0x1) && (((regval >> 12) & 0x3) == 0x3))
			break;
		RtmpusecDelay(10);
		cnt++;
	} while (cnt < 100);

	read_reg(pAd, 0x40, 0x148, &value);
	value &= ~(0x7F << 16);
	write_reg(pAd, 0x40, 0x148, value);

	RtmpusecDelay(10);
	read_reg(pAd, 0x40, 0x148, &value);
	value &= ~(0xF << 24);
	write_reg(pAd, 0x40, 0x148, value);
	RtmpusecDelay(10);

	read_reg(pAd, 0x40, 0x148, &value);
	value |= (0xF << 24);
	write_reg(pAd, 0x40, 0x148, value);

	read_reg(pAd, 0x40, 0x148, &value);
	value &= ~(0xFFF);
	write_reg(pAd, 0x40, 0x148, value);

	/* Set 1'b0 to turn on AD/DA power down */
	read_reg(pAd, 0x40, 0x1204, &value);
	value &= ~(0x1 << 3);
	write_reg(pAd, 0x40, 0x1204, value);

	/* WLAN function enable */
	read_reg(pAd, 0x40, 0x80, &value);
	value |= (0x1 << 0);
	write_reg(pAd, 0x40, 0x80, value);

	/* release "BBP software reset */
	read_reg(pAd, 0x40, 0x64, &value);
	value &= ~(0x1 << 18);
	write_reg(pAd, 0x40, 0x64, value);
}

static void WL_POWER_OFF(RTMP_ADAPTER *pAd)
{
	UINT32 cnt = 0;
	UINT32 regval = 0;

	read_reg(pAd, 0x40, 0x148, &regval);
	regval &= ~(1 << 21);
	regval = regval | (1 << 22) | (1 << 20) | (0xF << 16);
	regval = regval & ~(0xFFF);
	regval |= 0x2;
	write_reg(pAd, 0x40, 0x148, regval);

	do {
		read_reg(pAd, 0x40, 0x148, &regval);
		if ((((regval >> 28) & 0xF) == 0x0) && (((regval >> 12) & 0x3) == 0x0))
			break;
		RtmpusecDelay(10);
		cnt++;
	} while (cnt < 100);

	read_reg(pAd, 0x40, 0x148, &regval);
	regval &= ~(0xFFF);
	write_reg(pAd, 0x40, 0x148, regval);

	read_reg(pAd, 0x40, 0x80, &regval);
	regval &= ~(0x1 << 0);	/* WLAN function disable */
	write_reg(pAd, 0x40, 0x80, regval);

	read_reg(pAd, 0x40, 0x64, &regval);
	regval |= (0x1 << 18);	/* "BBP software reset" */
	write_reg(pAd, 0x40, 0x64, regval);
}

#ifdef RTMP_USB_SUPPORT
void mt76x2_rompatch_pwrOn(RTMP_ADAPTER *pAd)
{
#define ROMPATCH_FEATURE_BIT					0x1
#define ROMPATCH_FEATURE_BIT_BBPLON				0

#define ROMPATCH_WOW_OFF_STATUS					0x2
#define ROMPATCH_WOW_OFF_STATUS_BIT_READY		0

	NTSTATUS ret;
	INT16 wait_countdown = 5;
	UINT32 rompatch_value;

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));

	/* Check if FW is ready or not */
	RTUSBReadMACRegister(pAd, COM_REG0, &rompatch_value);
	if (OS_TEST_BIT(0, (unsigned long *)&rompatch_value)) {
		/* FW is alive. We need to stop WoW first. */

		/* disable WoW status (Rompatch would check if WoW is on or off) */
		ret = RTUSB_VendorRequest(pAd,
				USBD_TRANSFER_DIRECTION_OUT,
				DEVICE_VENDOR_REQUEST_OUT,
				0x91,
				VENDOR_CMD_FEATURE_SET_WIFI_WOW_STOP,
				0,
				0,
				0);

		/* Waiting for previous command is ready */
		while (wait_countdown--
				&& ret == NDIS_STATUS_SUCCESS) {
			ret = andes_usb_read_feature(pAd, ROMPATCH_WOW_OFF_STATUS, &rompatch_value);
			DBGPRINT(RT_DEBUG_OFF, ("%s: ROMPATCH_WOW_OFF_STATUS ret = %d. %d\n"
						, __func__, rompatch_value, ret));
			if (OS_TEST_BIT(ROMPATCH_WOW_OFF_STATUS_BIT_READY
					, (unsigned long *)&rompatch_value))
				break;
			RtmpOsMsDelay(1);
		}
		if (wait_countdown <= 0)
			DBGPRINT(RT_DEBUG_OFF,
					("\n\nWarning: Need WoW off feature in rompatch/FW\n\n"));
	}

	/* check if rompatch support BBP_ON function */
	ret = andes_usb_read_feature(pAd, ROMPATCH_FEATURE_BIT, &rompatch_value);
	DBGPRINT(RT_DEBUG_OFF, ("%s: ROMPATCH_FEATURE_BIT ret = %d. wait_countdown=%d\n"
				, __func__, rompatch_value, wait_countdown));

	/* config BBP */
	if (OS_TEST_BIT(ROMPATCH_FEATURE_BIT_BBPLON, (unsigned long *)&rompatch_value)
		&& ret == NDIS_STATUS_SUCCESS) {
		ret = RTUSB_VendorRequest(pAd,
				USBD_TRANSFER_DIRECTION_OUT,
				DEVICE_VENDOR_REQUEST_OUT,
				0x91,
				VENDOR_CMD_FEATURE_SET_WIFI_BBPLON,
				0,
				0,
				0);
	} else {
		DBGPRINT(RT_DEBUG_OFF,
				("\n\nWarning: Please check your rompatch. Run default pwron\n\n"));
		mt76x2_pwrOn(pAd);
	}
}
#endif

void mt76x2_pwrOn(RTMP_ADAPTER *pAd)
{
	/* Radio On */
	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));
	WL_POWER_ON(pAd);
	WF_CTRL(pAd, 0, 1);
	WF_CTRL(pAd, 1, 1);
}

void mt76x2_pwrOff(RTMP_ADAPTER *pAd)
{
	/* Radio Off */
	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));
	WF_CTRL(pAd, 0, 0);
	WF_CTRL(pAd, 1, 0);
	WL_POWER_OFF(pAd);
}

int mt76x2_set_ed_cca(RTMP_ADAPTER *ad, u8 enable)
{
	UINT32 mac_val;
	UINT32 bbp_val;

	if (enable) {
		RTMP_IO_READ32(ad, CH_TIME_CFG, &mac_val);
		mac_val |= 0x05;	/* enable channel status check */
		RTMP_IO_WRITE32(ad, CH_TIME_CFG, mac_val);

		/* BBP: latched ED_CCA and high/low threshold */
		RTMP_BBP_IO_READ32(ad, AGC1_R2, &bbp_val);
		/* bbp_val &= 0xFFFF; */
		bbp_val = 0x80000808;
		RTMP_BBP_IO_WRITE32(ad, AGC1_R2, bbp_val);

		/* MAC: enable ED_CCA/ED_2nd_CCA */
		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val |= ((1 << 20) | (1 << 7));
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
		mac_val |= 2;
		RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
	} else {
		/* MAC: disable ED_CCA/ED_2nd_CCA */
		RTMP_IO_READ32(ad, TXOP_CTRL_CFG, &mac_val);
		mac_val &= (~((1 << 20) | (1 << 7)));
		RTMP_IO_WRITE32(ad, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(ad, TXOP_HLDR_ET, &mac_val);
		mac_val &= ~2;
		RTMP_IO_WRITE32(ad, TXOP_HLDR_ET, mac_val);
	}

	/* Clear previous status */
	RTMP_IO_READ32(ad, CH_IDLE_STA, &mac_val);
	RTMP_IO_READ32(ad, CH_BUSY_STA, &mac_val);
	RTMP_IO_READ32(ad, CH_BUSY_STA_SEC, &mac_val);
	RTMP_IO_READ32(ad, 0x1140, &mac_val);

	return TRUE;
}
