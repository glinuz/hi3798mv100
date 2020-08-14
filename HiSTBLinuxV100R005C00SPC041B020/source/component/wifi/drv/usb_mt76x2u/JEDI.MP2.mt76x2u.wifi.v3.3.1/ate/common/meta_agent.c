/*
 ***************************************************************************
 * MediaTek Inc.
 *
 * All rights reserved. source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek, Inc. is obtained.
 ***************************************************************************

	Module Name:
	meta_agent.c
*/
#include "rt_config.h"

typedef enum _WIFI_SUB_TEST_STATE {
	WIFI_SUB_TEST_STANDBY,
	WIFI_SUB_TEST_RESET,
	WIFI_SUB_TEST_TX,
	WIFI_SUB_TEST_RX,
	WIFI_SUB_TEST_OUTPUT_PWR,
	WIFI_SUB_TEST_LOCAL_FREQ,
	WIFI_SUB_TEST_CARR_SUPPRESSION,
	WIFI_SUB_TEST_CONTI_WAVE,
	NUM_STATE
} WIFI_SUB_TEST_STATE;
#define _DUMP_HEX_ 1
static WIFI_SUB_TEST_STATE g_SUB_TEST_STATE = WIFI_SUB_TEST_STANDBY;
static UINT32 g_MT_META_ATCMD_TBL_VERSION = MT_META_WIFI_TEST_TABLE_VER;
static INT32 resp_to_meta(INT32 ioctl_cmd, META_CMD_HDR *rsp, RTMP_IOCTL_INPUT_STRUCT *wrq,
			  INT32 len, INT32 status)
{
	if (ioctl_cmd == MTPRIV_IOCTL_META_SET) {
		rsp->len_in = len;
	} else if (ioctl_cmd == MTPRIV_IOCTL_META_QUERY) {
		rsp->len_out = len;
	}

	wrq->u.data.length = sizeof(rsp->oid) + sizeof(rsp->len_in) +
			     sizeof(rsp->len_out) + len;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("WRQ->u.data.length = %u, oid: %zu, rsp: %zu, rsp->len: %d\n",
		  wrq->u.data.length, sizeof(rsp->oid), sizeof(rsp->len_in),
		  len));

	if (copy_to_user(wrq->u.data.pointer, rsp, wrq->u.data.length)) {

		DBGPRINT(RT_DEBUG_ERROR, ("copy_to_user() fail in %s\n", __func__));
		return (-EFAULT);
	}

#ifdef _DUMP_HEX_
	{
		UCHAR *pSrcBufVA = rsp;
		UINT32 SrcBufLen = wrq->u.data.length;
		unsigned char *pt;
		INT32 x;

		pt = pSrcBufVA;
		DBGPRINT(RT_DEBUG_TRACE, ("%s: %p, len = %d\n",
			 "META_IOCTL Ret Content", pSrcBufVA, SrcBufLen));
		for (x = 0; x < SrcBufLen; x++) {
			if (x % 16 == 0)
				printk("0x%04x : ", x);
			printk("%02x ", ((unsigned char)pt[x]));
			if (x % 16 == 15)
				printk("\n");
		}
		printk("\n");
	}
#endif /* endif */
	return 0;
}

static INT32 wifi_sub_test_stop(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = 0;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);

	switch (g_SUB_TEST_STATE) {
	case WIFI_SUB_TEST_STANDBY:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, WIFI_SUB_TEST_STANDBY\n", __func__));
		ret = Set_ATE_Proc(pAd, "ATESTART");
		break;
	case WIFI_SUB_TEST_RESET:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, WIFI_SUB_TEST_RESET\n", __func__));
		break;
	case WIFI_SUB_TEST_TX:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, WIFI_SUB_TEST_TX STOP\n", __func__));
		pATEInfo->Mode &= ATE_TXSTOP;
		pATEInfo->bQATxStart = FALSE;
		ret = Set_ATE_Proc(pAd, "ATESTART");
		/* ret = Set_ATE_Proc(pAd, "TXSTOP"); */
		pATEInfo->TxStatus = 0;
		break;
	case WIFI_SUB_TEST_RX:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, WIFI_SUB_TEST_RX STOP\n", __func__));
		pATEInfo->Mode &= ATE_RXSTOP;
		pATEInfo->bQARxStart = FALSE;
		ret = Set_ATE_Proc(pAd, "ATESTART");
		/* ret = Set_ATE_Proc(pAd, "RXSTOP"); */
		break;
	case WIFI_SUB_TEST_OUTPUT_PWR:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, Conti. Tx stop\n", __func__));
		ret = Set_ATE_Proc(pAd, "ATESTART");
		break;
	case WIFI_SUB_TEST_LOCAL_FREQ:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, Tx Tone DC stop\n", __func__));
		ret = Set_ATE_Proc(pAd, "ATESTART");
		break;
	case WIFI_SUB_TEST_CARR_SUPPRESSION:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, Tx Carrier Suppression stop\n", __func__));
		ret = Set_ATE_Proc(pAd, "ATESTART");
		break;
	case WIFI_SUB_TEST_CONTI_WAVE:
		DBGPRINT(RT_DEBUG_TRACE, ("%s, Conti. Tx stop\n", __func__));
		ret = Set_ATE_Proc(pAd, "ATESTART");
		break;
	default:
		break;
	}
	g_SUB_TEST_STATE = WIFI_SUB_TEST_STANDBY;
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_start_tx(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	/* PARAM_MTK_WIFI_TEST* param = (PARAM_MTK_WIFI_TEST *)cmd_hdr->data; */
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	ret = Set_ATE_Proc(pAd, "TXFRAME");
	g_SUB_TEST_STATE = WIFI_SUB_TEST_TX;
	if (pATEInfo->bQATxStart == TRUE)
		pATEInfo->TxStatus = 1;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_start_rx(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	/* PARAM_MTK_WIFI_TEST* param = (PARAM_MTK_WIFI_TEST *)cmd_hdr->data; */
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	g_SUB_TEST_STATE = WIFI_SUB_TEST_RX;
	pATEInfo->bQARxStart = TRUE;
	ret = Set_ATE_Proc(pAd, "RXFRAME");
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_reset(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	/* PARAM_MTK_WIFI_TEST* param = (PARAM_MTK_WIFI_TEST *)cmd_hdr->data; */
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	if (ioctl_cmd == MTPRIV_IOCTL_META_SET) {
		/* 1. Table Version */
		g_MT_META_ATCMD_TBL_VERSION = 0x01000002;
		/* 3. Rate */
#ifdef RLT_MAC
		if (pAd->chipCap.hif_type == HIF_RLT) {
			pATEInfo->TxWI.TXWI_N.PHYMODE = MODE_CCK;
			pATEInfo->TxWI.TXWI_N.BW = BW_20;
			pATEInfo->TxWI.TXWI_N.MCS = 0;
			pATEInfo->TxWI.TXWI_N.ShortGI = 0;
		}
#endif /* RLT_MAC */
#ifdef RTMP_MAC
		if (pAd->chipCap.hif_type == HIF_RTMP) {
			pATEInfo->TxWI.TXWI_O.PHYMODE = MODE_CCK;
			pATEInfo->TxWI.TXWI_O.BW = BW_20;
			pATEInfo->TxWI.TXWI_O.MCS = 0;
			pATEInfo->TxWI.TXWI_O.ShortGI = 0;
		}
#endif /* RTMP_MAC */
		/* 4. Preamble, follow PhyMode, and always using long preamble currently, in mac/mt_mac.c line. 754 */
		/* 6. Packet Length */
		pATEInfo->TxLength = 1024;
		/* 7. Packet Count */
		pATEInfo->TxCount = 1000;
		/* 18. CH FREQ */
		Set_ATE_TX_FREQ_OFFSET_Proc(pAd, "0");
		/* 23. TODO: Slow Clk Mode (?) */
		/* 29. TODO: TSSI */
		/* 31. Tx Power mode - always dBm */
		/* 65. TODO: CW Mode (?) */
		/* 71. CH BW */
		/* 72. Data BW */
		/* 73. Primary setting */
		Set_ATE_CHANNEL_Proc(pAd, "1");
		ATEAsicSwitchChannel(pAd);
		/* 74. J mode Setting (tone) */

		g_SUB_TEST_STATE = WIFI_SUB_TEST_STANDBY;
	} else {
		if (g_SUB_TEST_STATE != WIFI_SUB_TEST_STANDBY)
			ret = NDIS_STATUS_FAILURE;
		resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	}
	return ret;
}

static INT32 wifi_sub_test_output_pwr(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	/* PATE_INFO pATEInfo = &(pAd->ate); */
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);
	ret = Set_ATE_Proc(pAd, "TXCONT");
	g_SUB_TEST_STATE = WIFI_SUB_TEST_OUTPUT_PWR;

	DBGPRINT(RT_DEBUG_TRACE, ("%s, val:0x%x\n", __func__, val));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_local_freq(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_FAILURE;
	ret = Set_ATE_Proc(pAd, "TXCARR");
	g_SUB_TEST_STATE = WIFI_SUB_TEST_LOCAL_FREQ;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_rf_suppression(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					  RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_FAILURE;
	ret = Set_ATE_Proc(pAd, "TXCARS");
	g_SUB_TEST_STATE = WIFI_SUB_TEST_CARR_SUPPRESSION;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_trx_iq_cali(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				       RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	if (pATEInfo->Channel > 14) {
		CHIP_CALIBRATION(pAd, TXIQ_CALIBRATION_7662, 0x1);
		CHIP_CALIBRATION(pAd, RXIQC_FI_CALIBRATION_7662, 0x1);
	} else {
		CHIP_CALIBRATION(pAd, TXIQ_CALIBRATION_7662, 0x0);
		CHIP_CALIBRATION(pAd, RXIQC_FI_CALIBRATION_7662, 0x0);
	}

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_tssi_cali(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				     RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	if (pATEInfo->Channel > 14)
		CHIP_CALIBRATION(pAd, TSSI_CALIBRATION_7662, 0x1);
	else
		CHIP_CALIBRATION(pAd, TSSI_CALIBRATION_7662, 0x0);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_dpd_cali(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_conti_waveform(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					  RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);
	ret = Set_ATE_Proc(pAd, "TXCONT");
	g_SUB_TEST_STATE = WIFI_SUB_TEST_CONTI_WAVE;

	DBGPRINT(RT_DEBUG_TRACE, ("%s, val:0x%x\n", __func__, val));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_sub_test_start_icap(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static META_CMD_HANDLER WIFI_TEST_CMD_SUBTEST[] = {
	wifi_sub_test_stop,
	wifi_sub_test_start_tx,
	wifi_sub_test_start_rx,
	wifi_sub_test_reset,
	wifi_sub_test_output_pwr,
	wifi_sub_test_local_freq,
	wifi_sub_test_rf_suppression,
	wifi_sub_test_trx_iq_cali,
	wifi_sub_test_tssi_cali,
	wifi_sub_test_dpd_cali,
	wifi_sub_test_conti_waveform,
	wifi_sub_test_start_icap,
};

static INT32 wifi_test_version(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 status = NDIS_STATUS_SUCCESS;

	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: version:%04x !\n", __func__, g_MT_META_ATCMD_TBL_VERSION));
	memcpy(&param->data, &g_MT_META_ATCMD_TBL_VERSION, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(PARAM_MTK_WIFI_TEST), status);
	return status;
}

static INT32 wifi_test_cmd(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			   META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 op = param->data;
	if (op > (sizeof(WIFI_TEST_CMD_SUBTEST) / sizeof(META_CMD_HANDLER))) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s:op_idx: %04x, size of handle tbl: %zu\n", __func__, op,
			  sizeof(WIFI_TEST_CMD_SUBTEST) / sizeof(META_CMD_HANDLER)));
		return NDIS_STATUS_INVALID_DATA;
	}

	ret = (*WIFI_TEST_CMD_SUBTEST[op]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
	return ret;
}

static INT32 wifi_test_pwr_cfg(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, sizeof(val));
	pATEInfo->TxPower0 = val;
	snprintf((char *)str, sizeof(str), "%d", val);
	DBGPRINT(RT_DEBUG_OFF, ("%s, val:0x%x, str:%s\n", __func__, val, str));
	ret = Set_ATE_TX_POWER0_Proc(pAd, str);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_rate_cfg(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, sizeof(val));

	if ((val >> 31) & 0x00000001) {
		/* b31(msb) set(1) - 11n MCS:0~15, 32 */
		val &= 0x7FFFFFF;
		if (((val > 15) && (val < 32))
		    || (val > 32))
			return -1;
		if (val == 32)
			val = 17;	/* for corresponding to idx of OFDMRateTable */
		/*      case MODE_HTMIX / case MODE_HTGREENFIELD */

#ifdef DOT11_VHT_AC
	if (pAd->ate.vht_nss == 2)
		val += 16;
#endif				/* DOT11_VHT_AC */
	} else {
		/* b31(msb) set(0) - CCK:0~3, OFDM:4~11 */
		val &= 0x7FFFFFF;
		if ((val < 0) || (val > 11)) {
			return -1;
		} else if (val >= 5) {
			val -= 4;
		}
	}
	snprintf((char *)str, sizeof(str), "%d", val);
	Set_ATE_TX_MCS_Proc(pAd, str);
	DBGPRINT(RT_DEBUG_OFF,
		 ("%s, val:0x%x, str:%s\n", __func__, val, str));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_preamble_cfg(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));

	switch (val) {
	case 0:		/* Normal: don't know what is for normal, anyway choose OFDM */
		val = MODE_OFDM;
		break;
	case 1:		/* Have to check if its short preamble by default in CCK */
		val = MODE_CCK;
		break;
	case 2:
		val = MODE_HTMIX;
		break;
	case 3:
		val = MODE_HTGREENFIELD;
		break;
	case 4:
		val = MODE_VHT;
		break;
	default:
		break;
	}
	snprintf((char *)str, sizeof(str), "%d", val);
	Set_ATE_TX_MODE_Proc(pAd, str);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_antenna_cfg(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));

/* TODO need to check UI and doc*/
	switch (val) {
	case 0:
	case 1:
		val = val + 1;
		break;
	case 2:
		val = 0;
		break;
	default:
		break;
	}

	snprintf((char *)str, sizeof(str), "%d", val);

/* Setting nss number base on antenna setting */
#ifdef MT76x2
	if (val == 0)
		pATEInfo->vht_nss = 2;	/* max nss number */
	else
		pATEInfo->vht_nss = 1;
#endif
	ret = Set_ATE_TX_Antenna_Proc(pAd, str);
	ret = Set_ATE_RX_Antenna_Proc(pAd, str);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_pkt_len_cfg(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);

	if ((val < 24) || (val > (MAX_FRAME_SIZE - 34 /* == 2312 */))) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s: Out of range (%d), it should be in range of 24~%d.\n", __func__,
			  val, (MAX_FRAME_SIZE - 34 /* == 2312 */)));
		return -1;
	}
	pATEInfo->TxLength = val;
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	DBGPRINT(RT_DEBUG_OFF, ("%s: TxLength = %x\n", __func__, pATEInfo->TxLength));
	return ret;
}

static INT32 wifi_test_set_pkt_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));
	snprintf((char *)str, sizeof(str), "%d", val);

	Set_ATE_TX_COUNT_Proc(pAd, str);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

/*
 *	Since different chip has different wifi Tx off time duration, meta tool do not set it
 */
static INT32 wifi_test_set_pkt_interval(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	/* PATE_INFO pATEInfo = &(pAd->ate); */
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 val = 0;
	/* UINT32 slot_t = 0; */
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));
	snprintf((char *)str, sizeof(str), "%d", val);
	Set_ATE_IPG_Proc(pAd, str);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_tmp_comp(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));

	switch (val) {
	case 0:
		/* disable TSSI compensation */
		Set_ATE_AUTO_ALC_Proc(pAd, "0");
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: disable TSSI compensation.\n", __func__));
		break;
	case 1:
#ifdef RTMP_INTERNAL_TX_ALC
		/* enable TSSI compensation */
		Set_ATE_AUTO_ALC_Proc(pAd, "1");
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: enable TSSI compensation.\n", __func__));
#else
		DBGPRINT_ERR(("%s: not support TSSI compensation.\n",
			      __func__));
#endif /* RTMP_INTERNAL_TX_ALC */
		break;
	default:
		DBGPRINT_ERR(("%s: Unknown payload from QA = %u\n",
			      __func__, val));
		break;
	}

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_txop_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	return ret;
}

static INT32 wifi_test_set_ack(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	return ret;
}

static INT32 wifi_test_set_pkt_content(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				       RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	return ret;
}

static INT32 wifi_test_set_retry_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				     RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	return ret;
}

static INT32 wifi_test_set_qos_q(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);
	if (val > 4)
		return NDIS_STATUS_INVALID_DATA;

	/* ATECtrl->q_idx = (UCHAR)val; */
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_bw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	STRING str[LEN_OF_ARG];
	INT32 val = 0;
	UCHAR rf_bw, ext_ch;
	NdisZeroMemory(str, LEN_OF_ARG);

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s, val: 0x%x\n", __func__, val));
	if (val > 3) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s, Invalid Parameter(%x)\n", __func__, val));
		ret = -1;
		goto _meta_set_bw_err;
	}

	switch (val) {
	case 0:
		rf_bw = BW_20;
		ext_ch = EXTCHA_NONE;
		break;
	case 1:
	case 2:		/* Upper 20MHz of a 40MHz Ch, ex. Ch6 upper, then control ch will be Ch4 */
		rf_bw = BW_40;
		ext_ch = EXTCHA_ABOVE;
		break;
	case 3:		/* Lower 20MHz of a 40MHz Ch, ex. Ch6 lower, then control ch will be Ch8 */
		rf_bw = BW_40;
		ext_ch = EXTCHA_BELOW;
		break;
	default:
		rf_bw = BW_20;
		ext_ch = EXTCHA_NONE;
		break;
	}

	snprintf((char *)str, sizeof(str), "%d", rf_bw);

	/*  Tx/RX : control channel setting */
	bbp_set_ctrlch(pAd, ext_ch);
	rtmp_mac_set_ctrlch(pAd, ext_ch);

	Set_ATE_TX_BW_Proc(pAd, str);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Channel = %d, BW = %s\n", __func__, pATEInfo->Channel, str));

_meta_set_bw_err:
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);

	return ret;
}

/*
 *	param:
 *		0: normal GI
 *		1: short GI
 */
static INT32 wifi_test_set_gi(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	memcpy((PUCHAR) & val, &param->data, 4);

	DBGPRINT(RT_DEBUG_OFF, ("%s, val:0x%x\n", __func__, val));
	if (val > 1 || val < 0)
		return -1;

	NdisZeroMemory(str, LEN_OF_ARG);
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);
	snprintf((char *)str, sizeof(str), "%d", val);
	ret = Set_ATE_TX_GI_Proc(pAd, str);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_stbc(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);
	snprintf((char *)str, sizeof(str), "%d", val);
	ret = Set_ATE_TX_STBC_Proc(pAd, str);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_ch_freq(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	INT32 ch = 1;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);
	/* TODO: Need to check the value passed from host */
	RTMP_MapKHZ2ChannelID(val / 1000, &ch);
	pATEInfo->Channel = ch;
	/* Set Control channel according to BW */
	snprintf((char *)str, sizeof(str), "%d", pATEInfo->Channel);
	Set_ATE_CHANNEL_Proc(pAd, str);
	ATEAsicSwitchChannel(pAd);

	DBGPRINT(RT_DEBUG_OFF,
		 ("%s, val:%d, ch: 0x%x, str:%s\n", __func__, val, pATEInfo->Channel, str));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_rifs(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_tr_switch(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_rf_sx_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_pll_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_slow_clk_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_adc_clk_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_measure_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_vlt_comp(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_dpd_tx_gain(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				       RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	/* PARAM_MTK_WIFI_TEST* param = (PARAM_MTK_WIFI_TEST *)cmd_hdr->data; */
	INT32 ret = NDIS_STATUS_SUCCESS;
	/* INT32 val = 0; */

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_dpd_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	/* PARAM_MTK_WIFI_TEST* param = (PARAM_MTK_WIFI_TEST *)cmd_hdr->data; */
	INT32 ret = NDIS_STATUS_SUCCESS;
	/* INT32 val = 0; */

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_tssi_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	INT32 wf_sel = 0;

	/* TODO: Plan to use upper 16 bits for antenna selection, lower 16 bits for on/off */
	memcpy((PUCHAR) & val, &param->data, 4);
	val = val & 0x0000ffff;

	memcpy((PUCHAR) & wf_sel, &param->data, 4);
	wf_sel = ((wf_sel & 0xffff0000) >> 16) & 0x0000ffff;
	if (val > 0) {
		pATEInfo->bAutoTxAlc = TRUE;
	} else {
		pATEInfo->bAutoTxAlc = FALSE;

#ifdef RTMP_MAC_USB
		if (IS_USB_INF(pAd)) {
			RTMP_SEM_EVENT_WAIT(&pAd->tssi_lock, ret);
			if (ret != 0) {
				DBGPRINT(RT_DEBUG_ERROR, ("tssi_lock get failed(ret=%d)\n", ret));
				return STATUS_UNSUCCESSFUL;
			}
		}
#endif /* RTMP_MAC_USB */

		RTMP_IO_READ32(pAd, TX_ALC_CFG_1, &val);
		val = val & (~TX_ALC_CFG_1_TX0_TEMP_COMP_MASK);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_1, val);
		RTMP_IO_READ32(pAd, TX_ALC_CFG_2, &val);
		val = val & (~TX_ALC_CFG_2_TX1_TEMP_COMP_MASK);
		RTMP_IO_WRITE32(pAd, TX_ALC_CFG_2, val);

#ifdef RTMP_MAC_USB
		if (IS_USB_INF(pAd)) {
			RTMP_SEM_EVENT_UP(&pAd->tssi_lock);
		}
#endif /* endif */

	}
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_tx_gain_code(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_tx_pwr_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static META_CMD_HANDLER WIFI_TEST_CMD_SET1[] = {
	wifi_test_version,
	wifi_test_cmd,
	wifi_test_pwr_cfg,
	wifi_test_rate_cfg,
	wifi_test_preamble_cfg,
	wifi_test_antenna_cfg,
	wifi_test_pkt_len_cfg,
	wifi_test_set_pkt_cnt,
	wifi_test_set_pkt_interval,
	wifi_test_tmp_comp,
	wifi_test_txop_en,
	wifi_test_set_ack,
	wifi_test_set_pkt_content,
	wifi_test_set_retry_cnt,
	wifi_test_set_qos_q,
	wifi_test_set_bw,
	wifi_test_set_gi,
	wifi_test_set_stbc,
	wifi_test_set_ch_freq,
	wifi_test_set_rifs,
	wifi_test_tr_switch,
	wifi_test_rf_sx_en,
	wifi_test_pll_en,
	wifi_test_slow_clk_en,
	wifi_test_adc_clk_mode,
	wifi_test_measure_mode,
	wifi_test_vlt_comp,
	wifi_test_get_dpd_tx_gain,
	wifi_test_dpd_en,
	wifi_test_tssi_en,
	wifi_test_get_tx_gain_code,
	wifi_test_tx_pwr_mode,
};

static INT32 wifi_test_get_tx_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				  META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;

	memcpy(&param->data, &pATEInfo->TxDoneCount, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_tx_ok_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				     RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_rx_ok_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) (&cmd_hdr->data);
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 value = 0;

	/* Get latest FCSErr. sync iwpriv & HQA same FCSErr result */
	NICUpdateRawCounters(pAd);

	value = pAd->WlanCounters.ReceivedFragmentCount.QuadPart;
	memcpy(&param->data, &value, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_rx_err_cnt(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				  META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) (&cmd_hdr->data);
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 mcs_err = 0;

	/* Get latest FCSErr. sync iwpriv & HQA same FCSErr result */
	NICUpdateRawCounters(pAd);

	mcs_err = pAd->WlanCounters.FCSErrorCount.u.LowPart;
	memcpy(&param->data, &mcs_err, 4);

	DBGPRINT(RT_DEBUG_OFF, ("%s, mcs_err: %u\n", __func__, mcs_err));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_result_len(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_trx_iq_cal(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_tssi_cal(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_dpd_cal(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_rxv_dump(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_rx_stat(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_rpi_ipi(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_tmp_sensor(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_vlt_sensor(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				      RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_read_efuse(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				  META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_rx_rssi(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 value = 0;

	value |= (0x000000ff & pATEInfo->LastRssi0);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->MaxRssi0);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->MinRssi0);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->AvgRssi0);

	memcpy(&param->data, &value, 4);
	DBGPRINT(RT_DEBUG_OFF,
		 ("%s, value:0x%x, last:0x%x,max:0x%x, min:0x%x, avg:0x%x\n",
		  __func__, value, pATEInfo->LastRssi0, pATEInfo->MaxRssi0,
		  pATEInfo->MinRssi0, pATEInfo->AvgRssi0));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_fw_info(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0x0001;
	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));
	memcpy(&param->data, &val, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_dri_info(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0x7662;
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy(&param->data, &val, 4);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_pwr_detector(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_get_phy_rssi(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	/* UINT32 IBRssi0, IBRssi1, WBRssi0, WBRssi1; */
	UINT32 value;
	memcpy(&param->data, &value, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_get_rx_rssi1(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 value = 0;

	value |= (0x000000ff & pATEInfo->LastRssi1);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->MaxRssi1);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->MinRssi1);
	value = value << 8;
	value |= (0x000000ff & pATEInfo->AvgRssi1);

	memcpy(&param->data, &value, 4);
	DBGPRINT(RT_DEBUG_OFF,
		 ("%s, value:0x%x, last:0x%x,max:0x%x, min:0x%x, avg:0x%x\n",
		  __func__, value, pATEInfo->LastRssi1, pATEInfo->MaxRssi1,
		  pATEInfo->MinRssi1, pATEInfo->AvgRssi1));
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*param), ret);
	return ret;
}

static INT32 wifi_test_pwr_cfg_tx1(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	UINT32 value = 0;
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, sizeof(val));
	DBGPRINT(RT_DEBUG_OFF, ("%s, val:0x%x\n", __func__, val));
	pATEInfo->TxPower1 = val;
	snprintf((char *)str, sizeof(str), "%d", val);
	DBGPRINT(RT_DEBUG_OFF, ("%s, val:0x%x, str:%s\n", __func__, val, str));
	ret = Set_ATE_TX_POWER1_Proc(pAd, str);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return value;
}

static INT32 wifi_test_tx_path_sel(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	PATE_INFO pATEInfo = &(pAd->ate);

	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));

	switch (val) {
	case 0:
	case 1:
		val = val + 1;
		break;
	case 2:
		val = 0;
		break;
	default:
		break;
	}

	snprintf((char *)str, sizeof(str), "%d", val);

/* Setting nss number base on antenna setting */
#ifdef MT76x2
	if (val == 0)
		pATEInfo->vht_nss = 2;	/* max nss number */
	else
		pATEInfo->vht_nss = 1;
#endif
	snprintf((char *)str, sizeof(str), "%d", val);
	ret = Set_ATE_TX_Antenna_Proc(pAd, str);
	return ret;
}

static INT32 wifi_test_rx_path_sel(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	PATE_INFO pATEInfo = &(pAd->ate);
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s: Val: %x\n", __func__, val));

	switch (val) {
	case 0:
	case 1:
		val = val + 1;
		break;
	case 2:
		val = 0;
		break;
	default:
		break;
	}

	snprintf((char *)str, sizeof(str), "%d", val);

/* Setting nss number base on antenna setting */
#ifdef MT76x2
	if (val == 0)
		pATEInfo->vht_nss = 2;	/* max nss number */
	else
		pATEInfo->vht_nss = 1;
#endif
	snprintf((char *)str, sizeof(str), "%d", val);
	ret = Set_ATE_RX_Antenna_Proc(pAd, str);

	return ret;
}

static META_CMD_HANDLER WIFI_TEST_CMD_SET2[] = {
	wifi_test_get_tx_cnt,	/* 32 */
	wifi_test_get_tx_ok_cnt,
	wifi_test_rx_ok_cnt,
	wifi_test_rx_err_cnt,
	wifi_test_get_result_len,	/* For next 6 command */
	wifi_test_get_trx_iq_cal,
	wifi_test_get_tssi_cal,
	wifi_test_get_dpd_cal,
	wifi_test_rxv_dump,
	wifi_test_get_rx_stat,
	wifi_test_get_rpi_ipi,
	wifi_test_get_tmp_sensor,
	wifi_test_get_vlt_sensor,
	wifi_test_read_efuse,
	wifi_test_get_rx_rssi,
	wifi_test_get_fw_info,
	wifi_test_get_dri_info,
	wifi_test_get_pwr_detector,
	wifi_test_get_phy_rssi,	/* 50 */
	wifi_test_pwr_cfg_tx1,
	wifi_test_tx_path_sel,
	wifi_test_rx_path_sel,
	wifi_test_get_rx_rssi1,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

static INT32 wifi_test_set_dpd(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			       META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_cw_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s, val: %d, Ant: 0x%x\n", __func__, val, pATEInfo->TxAntennaSel));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_jp_ch_flt_en(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				    RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_write_efuse(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	UINT32 offset = 0;
	FUNC_IDX_GET_SUBFIELD(param->idx, offset);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_ra(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_ta(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_rx_match_rule(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					 RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;
	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_ch_bw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;
	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);

	snprintf((char *)str, sizeof(str), "%d", val);
	ret = Set_ATE_TX_BW_Proc(pAd, str);
	DBGPRINT(RT_DEBUG_OFF, ("%s, bw:%s\n", __func__, str));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_data_bw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_OFF, ("%s, val:0x%x\n", __func__, val));
	switch (val) {
	case 0:
		/* ATECtrl->PerPktBW = BW_20; */
		break;
	case 1:
		/* ATECtrl->PerPktBW = BW_40; */
		break;
	case 2:
		/* ATECtrl->PerPktBW = BW_80; */
		break;
	case 3:
	default:
		/* Not support yet */
		DBGPRINT(RT_DEBUG_OFF, ("%s, Invalid Paramter(%x)\n", __func__, val));
		ret = -1;
		break;
	}

	DBGPRINT(RT_DEBUG_OFF, ("%s: Channel = %d\n", __func__, pATEInfo->Channel));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_primary(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				   RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	STRING str[LEN_OF_ARG];

	NdisZeroMemory(str, LEN_OF_ARG);
	memcpy((PUCHAR) & val, &param->data, 4);

	snprintf((char *)str, sizeof(str), "%d", val);
	DBGPRINT(RT_DEBUG_OFF, ("%s, channel:%s\n", __func__, str));
	Set_ATE_CHANNEL_Proc(pAd, str);
	ATEAsicSwitchChannel(pAd);
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_encode_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				       RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 ret = NDIS_STATUS_SUCCESS;
	INT32 val = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	memcpy((PUCHAR) & val, &param->data, 4);

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_jmode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_FAILURE;
	PARAM_MTK_WIFI_TEST *param = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	PATE_INFO pATEInfo = &(pAd->ate);
	INT32 val = 0;
	memcpy((PUCHAR) & val, &param->data, 4);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s, val: %d, Ant: 0x%x\n", __func__, val, pATEInfo->TxAntennaSel));
	/* TxAntennaSel, 0: All 1:TX0 2:TX1 */
	/* val, 0: disable, 1.5M single tone, 2.10M single tone */
	switch (val) {
	case 1:
		ret = Set_ATE_Proc(pAd, "TXCARS");
		break;
	case 2:
		/* Not support 10M */
		ret = Set_ATE_Proc(pAd, "TXCARS");
		break;
	default:
		/* Stop TXCARS */
		ret = Set_ATE_Proc(pAd, "ATESTART");
		goto meta_jmode_done;
		break;
	}

meta_jmode_done:
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_inter_cap_cont(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					  RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_inter_cap_trig(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					  RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_inter_cap_size(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
					  RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static INT32 wifi_test_set_inter_cap_trig_offset(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
						 RTMP_IOCTL_INPUT_STRUCT *WRQ,
						 META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static META_CMD_HANDLER WIFI_TEST_CMD_SET3[] = {
	wifi_test_set_dpd,	/* 64 */
	wifi_test_set_cw_mode,
	wifi_test_jp_ch_flt_en,
	wifi_test_write_efuse,
	wifi_test_set_ra,
	wifi_test_set_ta,
	wifi_test_set_rx_match_rule,
	wifi_test_set_ch_bw,
	wifi_test_set_data_bw,
	wifi_test_set_primary,
	wifi_test_set_encode_mode,
	wifi_test_set_jmode,
	wifi_test_set_inter_cap_cont,
	wifi_test_set_inter_cap_trig,
	wifi_test_set_inter_cap_size,
	wifi_test_set_inter_cap_trig_offset,	/* 84 */
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,			/* 95 */
};

static INT32 wifi_test_set_mod_patch(INT32 ioctl_cmd, PRTMP_ADAPTER pAd,
				     RTMP_IOCTL_INPUT_STRUCT *WRQ, META_CMD_HDR *cmd_hdr)
{
	INT32 ret = NDIS_STATUS_SUCCESS;

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, ret);
	return ret;
}

static META_CMD_HANDLER WIFI_TEST_CMD_SET5[] = {
	wifi_test_set_mod_patch,	/* 128 */
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

/*
 *	Command Set for OID_CUSTOM_MTK_WIFI_TEST
 */
static META_CMD_HANDLER *WIFI_TEST_CMD_SETS[] = {
	WIFI_TEST_CMD_SET1,	/* 0~31 */
	WIFI_TEST_CMD_SET2,	/* 32~63 */
	WIFI_TEST_CMD_SET3,	/* 64~95 */
	NULL,			/* 96~127 */
	WIFI_TEST_CMD_SET5,	/* 128~149 */
};

/*
 *	Buffer Length: 4
 *	Return value: Chip Number, ex:0x00006620
 */
static INT32 oid_if_version(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			    META_CMD_HDR *cmd_hdr)
{
	META_CMD_HDR *cmd = cmd_hdr;
	INT32 status = NDIS_STATUS_SUCCESS;
	UINT32 value = 0x00006620;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: oid = 0x%04x, len:0x%x!\n", __func__, cmd_hdr->oid, cmd_hdr->len_in));
	memcpy(&cmd->data, &value, 4);
	status = resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 4, status);

	return status;
}

/*
 *	Both SET/QUERY, using struct _PARAM_CUSTOM_MCR_RW for payload
 */
static INT32 oid_mcr_rw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			META_CMD_HDR *cmd_hdr)
{
	PARAM_CUSTOM_MCR_RW *cmd = (PARAM_CUSTOM_MCR_RW *) cmd_hdr->data;
	INT32 status = NDIS_STATUS_SUCCESS;
	UINT32 value = 0;
	DBGPRINT(RT_DEBUG_TRACE, ("%s: MCR Access Offset = 0x%x\n", __func__, cmd->offset));

	/* Check if access F/W Domain MCR (due to WiFiSYS is placed from 0x6000-0000 */
	if (ioctl_cmd == MTPRIV_IOCTL_META_SET) {
		if (cmd->offset & 0xFFFF0000) {
			RTMP_IO_WRITE32(pAd, cmd->offset, cmd->data);
			RTMP_IO_READ32(pAd, cmd->offset, &value);
		} else {
#ifdef RTMP_SDIO_SUPPORT
			RTMP_SDIO_WRITE32(pAd, cmd->offset, cmd->data);
#endif /* endif */
		}
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: MCR Set Offset 0x%x, write in data: 0x%x, read after write: 0x%x\n",
			  __func__, cmd->offset, cmd->data, value));
	} else if (ioctl_cmd == MTPRIV_IOCTL_META_QUERY) {
#ifdef RTMP_SDIO_SUPPORT
			RTMP_SDIO_READ32(pAd, cmd->offset, &value);
#else
			RTMP_IO_READ32(pAd, cmd->offset, &value);
#endif /* endif */
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: MCR Query Offset = 0x%02x, Val: 0x%x\n",
			  __func__, cmd->offset, value));
	} else {
		status = NDIS_STATUS_INVALID_DATA;
		goto _mcr_rw_err;
	}
	memcpy(&cmd->data, &value, 4);
_mcr_rw_err:
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*cmd), status);
	return status;
}

/*
 *	Both SET/QUERY, using struct _PARAM_CUSTOM_EEPROM_RW for payload
 */
static INT32 oid_eeprom_rw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			   META_CMD_HDR *cmd_hdr)
{
	PARAM_CUSTOM_EEPROM_RW *cmd = (PARAM_CUSTOM_EEPROM_RW *) cmd_hdr->data;
	INT32 status = NDIS_STATUS_SUCCESS;
	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: EEPROM Write Data = 0x%02x !\n", __func__, cmd->data));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*cmd), status);
	return status;
}

/*
 *	QUERY, using struct _PARAM_CUSTOM_EFUSE_RW for payload
 */
static INT32 oid_efuse_rw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			  META_CMD_HDR *cmd_hdr)
{
	PARAM_CUSTOM_EFUSE_RW *cmd = (PARAM_CUSTOM_EFUSE_RW *) cmd_hdr->data;
	INT32 status = NDIS_STATUS_SUCCESS;
	DBGPRINT(RT_DEBUG_TRACE, ("\n%s: EFUSE Write Data = 0x%02x !\n", __func__, cmd->data));

	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*cmd), status);
	return status;
}

/*
 *	Like ATEStart (?)
 */
static INT32 oid_test_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			   META_CMD_HDR *cmd_hdr)
{
	INT32 status = NDIS_STATUS_SUCCESS;
	PATE_INFO pATEInfo = &(pAd->ate);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: oid = 0x%04x, len:0x%x\n", __func__, cmd_hdr->oid, cmd_hdr->len_in));
	pATEInfo->bQAEnabled = TRUE;
	/* Prepare feedback as soon as we can to avoid QA timeout. */
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, status);
	/* Initialization */
	g_SUB_TEST_STATE = WIFI_SUB_TEST_STANDBY;
	Set_ATE_CHANNEL_Proc(pAd, "1");
	ATEAsicSwitchChannel(pAd);
	status = Set_ATE_Proc(pAd, "ATESTART");
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, status);

	return status;
}

/*
 *	Like ATEStop (?)
 */
static INT32 oid_abort_test_mode(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
				 META_CMD_HDR *cmd_hdr)
{
	INT32 status = NDIS_STATUS_SUCCESS;
	PATE_INFO pATEInfo = &(pAd->ate);

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: oid = 0x%04x, len:0x%x!\n", __func__, cmd_hdr->oid, cmd_hdr->len_in));
	/* Prepare feedback as soon as we can to avoid QA timeout. */
	resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 0, status);
	pATEInfo->bQAEnabled = FALSE;
	status = Set_ATE_Proc(pAd, "ATESTOP");

	return status;
}

/*
 *	Bunch of further commands
 */
static INT32 oid_mt_wifi_test(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	INT32 status = NDIS_STATUS_SUCCESS;
	PARAM_MTK_WIFI_TEST *cmd = (PARAM_MTK_WIFI_TEST *) cmd_hdr->data;
	INT32 idx = 0;
	INT32 set_idx = 0;
	INT32 handle_idx = 0;

	FUNC_IDX_GET_IDX(cmd->idx, idx);
	set_idx = ((idx & ~MT_META_WIFI_TEST_CMD_MASK) >> 5) & MT_META_WIFI_TEST_SET_MASK;
	handle_idx = idx & MT_META_WIFI_TEST_CMD_MASK;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: cmd->idx:0x%x ,set_idx:0x%04x, handle_idx:0x%04x\n", __func__, cmd->idx,
		  set_idx, handle_idx));

	if (!WIFI_TEST_CMD_SETS[set_idx])
		goto err_handle;
	if (!(*WIFI_TEST_CMD_SETS[set_idx][handle_idx]))
		goto err_handle;
	(*WIFI_TEST_CMD_SETS[set_idx][handle_idx]) (ioctl_cmd, pAd, WRQ, cmd_hdr);

	return status;
err_handle:
	return NDIS_STATUS_INVALID_DATA;
}

static INT32 oid_cfg_src_type(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			      META_CMD_HDR *cmd_hdr)
{
	META_CMD_HDR *cmd = cmd_hdr;
	INT32 status = NDIS_STATUS_SUCCESS;
	UINT32 value = 1;	/* 1 For NVRAM */

	DBGPRINT(RT_DEBUG_TRACE, ("%s: len:0x%x!\n", __func__, cmd->len_in));
	memcpy(&cmd->data, &value, 4);
	status = resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 4, status);

	return status;
}

static INT32 oid_eeprom_type(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			     META_CMD_HDR *cmd_hdr)
{
	META_CMD_HDR *cmd = cmd_hdr;
	INT32 status = NDIS_STATUS_SUCCESS;
	UINT32 value = 0;	/* 0 For EEPROM Not Present */

	DBGPRINT(RT_DEBUG_TRACE, ("%s: len:0x%x!\n", __func__, cmd_hdr->len_in));
	memcpy(&cmd->data, &value, 4);
	status = resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, 4, status);

	return status;
}

static INT32 oid_nvram_rw(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
			  META_CMD_HDR *cmd_hdr)
{
	PARAM_CUSTOM_NVRAM_RW *cmd = (PARAM_CUSTOM_NVRAM_RW *) cmd_hdr->data;
	INT32 status = NDIS_STATUS_SUCCESS;
	UINT16 value = 0;
	UINT8 idx = cmd->idx << 1;
	if (ioctl_cmd == MTPRIV_IOCTL_META_SET) {
		memcpy((PUCHAR) & value, &cmd->data, 4);
		pAd->EEPROMImage[idx] = (0xff00 & value) >> 8;
		pAd->EEPROMImage[idx + 1] = (0x00ff & value);
		rtmp_ee_write_to_bin(pAd);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s-SET, method:0x%x ,offset:0x%x, valu:0x%x, EEPROMImage:0x%x\n",
			  __func__, cmd->method, idx, value, pAd->EEPROMImage[idx]));
	} else if (ioctl_cmd == MTPRIV_IOCTL_META_QUERY) {
		if (cmd->method == PARAM_EEPROM_READ_METHOD_READ) {
			value |= pAd->EEPROMImage[idx];
			value <<= 8;
			value |= pAd->EEPROMImage[idx + 1];
		} else if (cmd->method == PARAM_EEPROM_READ_METHOD_GETSIZE) {
			value = EEPROM_SIZE;
		}
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s-QUREY, method:0x%x ,offset:0x%x, valu:0x%x\n", __func__,
			  cmd->method, idx, value));
	} else {
	}
	memcpy(&cmd->data, &value, 4);
	status = resp_to_meta(ioctl_cmd, cmd_hdr, WRQ, sizeof(*cmd), status);

	return status;
}

/*
 *	Handler for Each OID
 */
static META_CMD_HANDLER OID_HANDLE[] = {
	oid_if_version,
	oid_mcr_rw,
	oid_eeprom_rw,
	oid_efuse_rw,
	oid_test_mode,
	oid_abort_test_mode,
	oid_mt_wifi_test,
	oid_cfg_src_type,
	oid_eeprom_type,
	oid_nvram_rw,
};

INT32 do_meta_cmd(INT32 ioctl_cmd, PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *WRQ,
		  PSTRING *wrq_name)
{
	INT32 Status = NDIS_STATUS_SUCCESS;
	META_CMD_HDR *cmd_hdr;
	UINT32 oid = 0;
	INT32 ret = 0;

	os_alloc_mem_suspend(pAd, (UCHAR **) &cmd_hdr, sizeof(*cmd_hdr));
	if (!cmd_hdr) {
		Status = -ENOMEM;
		DBGPRINT(RT_DEBUG_TRACE, ("%s, cmd_hdr NULL\n", __func__));
		goto ERROR0;
	}

	if (!WRQ) {
		Status = -EINVAL;
		DBGPRINT(RT_DEBUG_TRACE, ("%s. wrq is null\n", __func__));
		goto ERROR1;
	}

	if (WRQ->u.data.length != sizeof(*cmd_hdr)) {
		Status = -EINVAL;
		goto ERROR1;
	}

	NdisZeroMemory(cmd_hdr, sizeof(*cmd_hdr));
	if (copy_from_user(cmd_hdr, WRQ->u.data.pointer, WRQ->u.data.length)) {
		Status = -EFAULT;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s, copy_from_user error\n", __func__));
		goto ERROR1;
	}

#ifdef _DUMP_HEX_
	{
		UCHAR *pSrcBufVA = cmd_hdr;
		UINT32 SrcBufLen = sizeof(*cmd_hdr);
		unsigned char *pt;
		INT32 x;

		pt = pSrcBufVA;
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: %p, len = %d\n", "META_IOCTL Content",
			  pSrcBufVA, SrcBufLen));
		for (x = 0; x < SrcBufLen; x++) {
			if (x % 16 == 0)
				DBGPRINT(RT_DEBUG_TRACE, ("0x%04x : ", x));
			DBGPRINT(RT_DEBUG_TRACE, ("%02x ", ((unsigned char)pt[x]));
			if (x % 16 == 15)
				DBGPRINT(RT_DEBUG_TRACE, ("\n"));
		}
		DBGPRINT(RT_DEBUG_TRACE, ("\n"));
	}
#endif /* endif */
	oid = cmd_hdr->oid;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s() ==>,OID:0x%x, len:0x%x\n", __func__, oid, SrcBufLen));

	switch (oid) {
	case OID_CUSTOM_IF_VER:
		ret = (*OID_HANDLE[0]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_MCR_RW:
		ret = (*OID_HANDLE[1]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_EEPROM_RW:
		ret = (*OID_HANDLE[2]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_EFUSE_RW:
		ret = (*OID_HANDLE[3]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_TEST_MODE:
		ret = (*OID_HANDLE[4]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_ABORT_TEST_MODE:
		ret = (*OID_HANDLE[5]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_MTK_WIFI_TEST:
		ret = (*OID_HANDLE[6]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_CFG_SRC_TYPE:
		ret = (*OID_HANDLE[7]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_EEPROM_TYPE:
		ret = (*OID_HANDLE[8]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	case OID_CUSTOM_MTK_NVRAM_RW:
		ret = (*OID_HANDLE[9]) (ioctl_cmd, pAd, WRQ, cmd_hdr);
		break;
	default:
		Status = NDIS_STATUS_FAILURE;
		DBGPRINT(RT_DEBUG_ERROR, ("Unknown oid number of AT command = %x\n", oid));
		goto ERROR1;
		break;
	}

ERROR1:
	os_free_mem(NULL, cmd_hdr);
ERROR0:
	return Status;
}

#ifdef RTMP_SDIO_SUPPORT
INT Set_Sdio_Bist_Write(IN RTMP_ADAPTER * pAd, IN RTMP_STRING * arg)
{
	UCHAR *pBuf;
	INT32 u4Ret = 0;
	ULONG u4bist = 0;
	ULONG rwlimit = (UINT32) simple_strtol(arg, 0, 10);
	os_alloc_mem(NULL, (UCHAR **) &pBuf, 512 * 4000);

	DBGPRINT(RT_DEBUG_WARN, ("%s(): PRBS pattern seed=73 (Read/Write 256)\n", __func__));
	u4Ret = MTSDIOMultiRead(pAd, WTMDR, pBuf, 256);
	for (u4bist = 0; u4bist < rwlimit; u4bist++) {
		printk("---> %lu round\n", u4bist);
		u4Ret = MTSDIOMultiWrite(pAd, WTMDR, pBuf, 256);
	}

	return TRUE;
}

INT Set_Sdio_Bist_Read(IN RTMP_ADAPTER * pAd, IN RTMP_STRING * arg)
{
	UCHAR *pBuf;
	INT32 u4Ret = 0;
	ULONG u4bist = 0;
	INT32 bistlen = 1536000;
	ULONG prbslimit = (ULONG) simple_strtol(arg, 0, 10);
	os_alloc_mem(NULL, (UCHAR **) &pBuf, 512 * 4000);

	DBGPRINT(RT_DEBUG_WARN, ("%s(): PRBS pattern seed=37 (Read)\n", __func__));
	for (u4bist = 0; u4bist < prbslimit; u4bist++) {
		u4Ret = MTSDIOMultiRead(pAd, WTMDR, pBuf, bistlen);
		if (u4Ret != 0) {
			DBGPRINT(RT_DEBUG_WARN,
				 ("%s(): error MTSDIOMultiRead u4Ret=%x\n", __func__, u4Ret));
		}
	}

	return TRUE;
}
#endif /* endif */
