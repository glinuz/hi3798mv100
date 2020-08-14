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

#include	"rt_config.h"


#ifdef CONFIG_STA_SUPPORT
#ifdef PROFILE_STORE
NDIS_STATUS WriteDatThread(RTMP_ADAPTER *pAd);
#endif /* PROFILE_STORE */
#endif /* CONFIG_STA_SUPPORT */

int rt28xx_init(VOID *pAdSrc, PSTRING pDefaultMac, PSTRING pHostName)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pAdSrc;
	NDIS_STATUS Status;
#ifdef RTMP_MAC_USB
	UINT index = 0;
#endif /* RTMP_MAC_USB */

	if (!pAd)
		return FALSE;

#ifdef RTMP_MAC_USB
	RTMPSwReset(pAd);
#endif

	if (rtmp_asic_top_init(pAd) != TRUE)
		goto err1;

	DBGPRINT(RT_DEBUG_TRACE, ("MAC[Ver:Rev=0x%08x : 0x%08x]\n", pAd->MACVersion, pAd->ChipID));

	RT28XXDMADisable(pAd);

	if (mcu_sys_init(pAd) != TRUE)
		goto err1;
#if defined(RT_CFG80211_SUPPORT) && defined(JUNGLE_SUPPORT)
	pAd->applyUpperLayerReg = FALSE;
#endif
#ifdef RTMP_MAC_PCI
	/* TODO: shiang-usw, need to check this for RTMP_MAC */
	/* Disable interrupts here which is as soon as possible */
	/* This statement should never be true. We might consider to remove it later */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE)) {
		RTMP_ASIC_INTERRUPT_DISABLE(pAd);
	}
#endif /* RTMP_MAC_PCI */

	/* reset Adapter flags */
	RTMP_CLEAR_FLAGS(pAd);

	if (MAX_LEN_OF_MAC_TABLE > MAX_AVAILABLE_CLIENT_WCID(pAd)) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("MAX_LEN_OF_MAC_TABLE can not be larger than MAX_AVAILABLE_CLIENT_WCID!!!!\n"));
		goto err1;
	}
#ifdef CONFIG_AP_SUPPORT
	/* Init BssTab & ChannelInfo tabbles for auto channel select. */
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		AutoChBssTableInit(pAd);
		ChannelInfoInit(pAd);
	}
#endif /* CONFIG_AP_SUPPORT */

	/* Allocate BA Reordering memory */
	if (ba_reordering_resource_init(pAd, MAX_REORDERING_MPDU_NUM) != TRUE)
		goto err1;

	Status = RTMPInitTxRxRingMemory(pAd);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("RTMPAllocTxRxMemory failed, Status[=0x%08x]\n", Status));
		goto err2;
	}
#ifdef RTMP_MAC_USB
#ifdef MULTI_URB
	pAd->CommonCfg.bMultipleIRP = TRUE;
#else
	pAd->CommonCfg.bMultipleIRP = FALSE;
#endif /* endif */
	if (pAd->CommonCfg.bMultipleIRP)
		pAd->CommonCfg.NumOfBulkInIRP = RX_RING_SIZE;
	else
		pAd->CommonCfg.NumOfBulkInIRP = 1;
#endif /* RTMP_MAC_USB */

#ifdef WLAN_SKB_RECYCLE
	skb_queue_head_init(&pAd->rx0_recycle);
#endif /* WLAN_SKB_RECYCLE */

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);

	/* initialize MLME */

	Status = RtmpMgmtTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err3;

	Status = MlmeInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("MlmeInit failed, Status[=0x%08x]\n", Status));
		goto err4;
	}

	/* Initialize pAd->StaCfg, pAd->ApCfg, pAd->CommonCfg to manufacture default */
	UserCfgInit(pAd);


	Status = RtmpNetTaskInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS)
		goto err5;

	CfgInitHook(pAd);

#ifdef BLOCK_NET_IF
	initblockQueueTab(pAd);
#endif /* BLOCK_NET_IF */

	Status = MeasureReqTabInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("MeasureReqTabInit failed, Status[=0x%08x]\n", Status));
		goto err6;
	}
	Status = TpcReqTabInit(pAd);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("TpcReqTabInit failed, Status[=0x%08x]\n", Status));
		goto err6;
	}

	/*
	   WiFi system operation mode setting base on following partitions:
	   1. Parameters from config file
	   2. Hardware cap from EEPROM
	   3. Chip capabilities in code
	 */
	pAd->RfIcType = RFIC_UNKNOWN;
	Status = RTMPReadParametersHook(pAd);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("RTMPReadParametersHook failed, Status[=0x%08x]\n", Status));
		goto err6;
	}
	DBGPRINT(RT_DEBUG_OFF, ("1. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

	/* We should read EEPROM for all cases */
	NICReadEEPROMParameters(pAd, (PSTRING) pDefaultMac);
#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */
	DBGPRINT(RT_DEBUG_OFF, ("2. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

	/* After operation mode is finialized, init the AP or STA mode */
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		APInitialize(pAd);

	}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		STAInitialize(pAd);

#ifdef CREDENTIAL_STORE
		RecoverConnectInfo(pAd);
#endif /* CREDENTIAL_STORE */
	}
#endif /* CONFIG_STA_SUPPORT */

	/*Init Ba Capability parameters. */
	pAd->CommonCfg.DesiredHtPhy.MpduDensity =
	    (UCHAR) pAd->CommonCfg.BACapability.field.MpduDensity;
	pAd->CommonCfg.DesiredHtPhy.AmsduEnable =
	    (USHORT) pAd->CommonCfg.BACapability.field.AmsduEnable;
	pAd->CommonCfg.DesiredHtPhy.AmsduSize =
	    (USHORT) pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.DesiredHtPhy.MimoPs = (USHORT) pAd->CommonCfg.BACapability.field.MMPSmode;
	/* UPdata to HT IE */
	pAd->CommonCfg.HtCapability.HtCapInfo.MimoPs =
	    (USHORT) pAd->CommonCfg.BACapability.field.MMPSmode;
	pAd->CommonCfg.HtCapability.HtCapInfo.AMsduSize =
	    (USHORT) pAd->CommonCfg.BACapability.field.AmsduSize;
	pAd->CommonCfg.HtCapability.HtCapParm.MpduDensity =
	    (UCHAR) pAd->CommonCfg.BACapability.field.MpduDensity;

	/* after reading Registry, we now know if in AP mode or STA mode */
	DBGPRINT(RT_DEBUG_OFF, ("3. Phy Mode = %d\n", pAd->CommonCfg.PhyMode));

#ifdef RALINK_ATE
	if (ATEInit(pAd) != NDIS_STATUS_SUCCESS) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s(): ATE initialization failed !\n", __func__));
		goto err6;
	}
#endif /* RALINK_ATE */

	/*
	   All settle down, now it's time to init asic related parameters
	 */
	/* Init the hardware, we need to init asic before read registry, otherwise mac register will be reset */
	Status = NICInitializeAdapter(pAd, TRUE);
	if (Status != NDIS_STATUS_SUCCESS) {
		DBGPRINT_ERR(("NICInitializeAdapter failed, Status[=0x%08x]\n", Status));
		if (Status != NDIS_STATUS_SUCCESS)
			goto err6;
	}
#ifdef CONFIG_FPGA_MODE
#ifdef CAPTURE_MODE
	cap_mode_init(pAd);
#endif /* CAPTURE_MODE */
#endif /* CONFIG_FPGA_MODE */

	NICInitAsicFromEEPROM(pAd);

#ifdef LED_CONTROL_SUPPORT
	/* Send LED Setting to MCU */
	RTMPInitLEDMode(pAd);
#endif /* LED_CONTROL_SUPPORT */

	/*
	   Do necessary calibration after ASIC initialized
	   this's chip variant and may different for different chips
	 */

#ifdef RTMP_FREQ_CALIBRATION_SUPPORT
#ifdef CONFIG_STA_SUPPORT
	/* Initialize the frequency calibration */
	if (pAd->chipCap.FreqCalibrationSupport)
		FrequencyCalibration(pAd);
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_FREQ_CALIBRATION_SUPPORT */

#ifdef RTMP_INTERNAL_TX_ALC
	/* Initialize the desired TSSI table */
	RTMP_CHIP_ASIC_TSSI_TABLE_INIT(pAd);
#endif /* RTMP_INTERNAL_TX_ALC */


#ifdef RT8592
	/* TODO: shiang-6590, actually, this operation shall be move to bbp_init */
	if (IS_RT8592(pAd))
		bw_filter_cal(pAd);
#endif /* RT8592 */

#ifdef RTMP_TEMPERATURE_COMPENSATION
	/* Temperature compensation, initialize the lookup table */
	DBGPRINT(RT_DEBUG_OFF, ("bAutoTxAgcG = %d\n", pAd->bAutoTxAgcG));

	if (pAd->chipCap.bTempCompTxALC && pAd->bAutoTxAgcG)
		InitLookupTable(pAd);
#endif /* RTMP_TEMPERATURE_COMPENSATION */

#ifdef RTMP_FREQ_CALIBRATION_SUPPORT
#ifdef CONFIG_STA_SUPPORT
	if (pAd->chipCap.FreqCalibrationSupport)
		InitFrequencyCalibration(pAd);
#endif /* CONFIG_STA_SUPPORT */
#endif /* RTMP_FREQ_CALIBRATION_SUPPORT */

	/* Set PHY to appropriate mode and will update the ChannelListNum in this function */
	RTMPSetPhyMode(pAd, pAd->CommonCfg.PhyMode);
	if (pAd->ChannelListNum == 0) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("Wrong configuration. No valid channel found. Check \"ContryCode\" and \"ChannelGeography\" setting.\n"));
		goto err6;
	}

	DBGPRINT(RT_DEBUG_OFF,
		 ("MCS Set = %02x %02x %02x %02x %02x\n", pAd->CommonCfg.HtCapability.MCSSet[0],
		  pAd->CommonCfg.HtCapability.MCSSet[1], pAd->CommonCfg.HtCapability.MCSSet[2],
		  pAd->CommonCfg.HtCapability.MCSSet[3], pAd->CommonCfg.HtCapability.MCSSet[4]));

#ifdef WIN_NDIS
	/* Patch cardbus controller if EEPROM said so. */
	if (pAd->bTest1 == FALSE)
		RTMPPatchCardBus(pAd);
#endif /* WIN_NDIS */

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
#ifdef AP_QLOAD_SUPPORT
		QBSS_LoadInit(pAd);
#endif /* AP_QLOAD_SUPPORT */
	}
#endif /* CONFIG_AP_SUPPORT */

	/* APInitialize(pAd); */

#ifdef IKANOS_VX_1X0
	VR_IKANOS_FP_Init(pAd->ApCfg.BssidNum, pAd->PermanentAddress);
#endif /* IKANOS_VX_1X0 */

#ifdef RTMP_MAC_USB
	if (pAd->chipCap.MCUType == M8051) {
		AsicSendCommandToMcu(pAd, 0x31, 0xff, 0x00, 0x02, FALSE);
		RtmpusecDelay(10000);
	}
#endif /* RTMP_MAC_USB */

#ifdef RALINK_ATE
#endif /* RALINK_ATE */

#ifdef RTMP_INTERNAL_TX_ALC
#endif /* RTMP_INTERNAL_TX_ALC */

	/*
	   Some modules init must be called before APStartUp().
	   Or APStartUp() will make up beacon content and call
	   other modules API to get some information to fill.
	 */



	if (pAd && (Status != NDIS_STATUS_SUCCESS)) {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE))
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
	} else if (pAd) {
		/* Microsoft HCT require driver send a disconnect event after driver initialization. */
		OPSTATUS_CLEAR_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED);
		OPSTATUS_CLEAR_FLAG(pAd, fOP_AP_STATUS_MEDIA_STATE_CONNECTED);

		DBGPRINT(RT_DEBUG_TRACE, ("NDIS_STATUS_MEDIA_DISCONNECT Event B!\n"));

#ifdef CONFIG_AP_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
			if (pAd->ApCfg.bAutoChannelAtBootup || (pAd->CommonCfg.Channel == 0)) {
				/* Enable Interrupt first due to we need to scan channel to receive beacons. */
#ifdef RTMP_MAC_PCI
				RTMP_IRQ_ENABLE(pAd);
#endif /* RTMP_MAC_PCI */
#ifdef RTMP_MAC_USB
				RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS);
				RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);

				/* Support multiple BulkIn IRP, */
				/* the value on pAd->CommonCfg.NumOfBulkInIRP may be large than 1. */
#ifdef MULTI_URB
				for (index = 0; index < SUBMIT_URB_NUM; index++) {
					DoBulkIn(pAd);
					DBGPRINT(RT_DEBUG_TRACE, ("DoBulkIn!\n"));
				}
#else
				for (index = 0; index < pAd->CommonCfg.NumOfBulkInIRP; index++) {
					RTUSBBulkReceive(pAd);
					DBGPRINT(RT_DEBUG_TRACE, ("RTUSBBulkReceive!\n"));
				}
#endif /* MULTI_URB */
#endif /* RTMP_MAC_USB */

				/* Now Enable RxTx */
				RTMPEnableRxTx(pAd);
#ifdef MT762x
				/* TODO: shiang-usw, check why MT76x2 don't need to set this flag here! */
				if (!IS_MT762x(pAd))
#endif /* MT762x */
					RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);

				/* Let BBP register at 20MHz to do scan */
				bbp_set_bw(pAd, BW_20);

				/* Now we can receive the beacon and do the listen beacon */
				/* use default BW to select channel */
				pAd->CommonCfg.Channel =
				    AP_AUTO_CH_SEL(pAd, pAd->ApCfg.AutoChannelAlg);
				pAd->ApCfg.bAutoChannelAtBootup = FALSE;
			}

			/* If WMODE_CAP_N(phymode) and BW=40 check extension channel, after select channel  */
			N_ChannelCheck(pAd);

			/*
			   We only do this Overlapping BSS Scan when system up, for the
			   other situation of channel changing, we depends on station's
			   report to adjust ourself.
			 */
			if (pAd->CommonCfg.bForty_Mhz_Intolerant == TRUE) {
				DBGPRINT(RT_DEBUG_TRACE,
					 ("Disable 20/40 BSSCoex Channel Scan(BssCoex=%d, 40MHzIntolerant=%d)\n",
					  pAd->CommonCfg.bBssCoexEnable,
					  pAd->CommonCfg.bForty_Mhz_Intolerant));
			} else if (pAd->CommonCfg.bBssCoexEnable == TRUE) {
				DBGPRINT(RT_DEBUG_TRACE,
					 ("Enable 20/40 BSSCoex Channel Scan(BssCoex=%d)\n",
					  pAd->CommonCfg.bBssCoexEnable));
				APOverlappingBSSScan(pAd);
			}

			RTMP_11N_D3_TimerInit(pAd);
			/* RTMPInitTimer(pAd, &pAd->CommonCfg.Bss2040CoexistTimer,
			*  GET_TIMER_FUNCTION(Bss2040CoexistTimeOut), pAd, FALSE);
			*/


			APStartUp(pAd);
			DBGPRINT(RT_DEBUG_OFF, ("Main bssid = %02x:%02x:%02x:%02x:%02x:%02x\n",
						PRINT_MAC(pAd->ApCfg.MBSSID[BSS0].wdev.bssid)));

#ifdef DYNAMIC_VGA_SUPPORT
#ifdef MT76x2
			if (IS_MT76x2(pAd)) {
				mt76x2_reinit_agc_gain(pAd, pAd->hw_cfg.cent_ch);
				mt76x2_reinit_hi_lna_gain(pAd, pAd->hw_cfg.cent_ch);
				mt76x2_get_agc_gain(pAd, TRUE);
			}
#endif /* MT76x2 */
#endif /* DYNAMIC_VGA_SUPPORT */
		}
#endif /* CONFIG_AP_SUPPORT */


#ifdef RTMP_MAC_USB
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);

		/* Support multiple BulkIn IRP, */
		/* the value on pAd->CommonCfg.NumOfBulkInIRP may be large than 1. */
#ifdef MULTI_URB
		for (index = 0; index < SUBMIT_URB_NUM; index++) {
			DoBulkIn(pAd);
			DBGPRINT(RT_DEBUG_TRACE, ("DoBulkIn!\n"));
		}
#else
		for (index = 0; index < pAd->CommonCfg.NumOfBulkInIRP; index++) {
			RTUSBBulkReceive(pAd);
			DBGPRINT(RT_DEBUG_TRACE, ("RTUSBBulkReceive!\n"));
		}
#endif /* MULTI_URB */
#endif /* RTMP_MAC_USB */
	}

	/* Set up the Mac address */
#ifdef CONFIG_AP_SUPPORT
	RtmpOSNetDevAddrSet(pAd->OpMode, pAd->net_dev, &pAd->CurrentAddress[0], NULL);
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
	NdisMoveMemory(&pAd->StaCfg.wdev.if_addr[0], &pAd->CurrentAddress[0], MAC_ADDR_LEN);
	RtmpOSNetDevAddrSet(pAd->OpMode, pAd->net_dev, &pAd->CurrentAddress[0],
			    (PUCHAR) (pAd->StaCfg.dev_name));
	NdisMoveMemory(&pAd->StaCfg.wdev.if_addr[0], &pAd->CurrentAddress[0], MAC_ADDR_LEN);
#endif /* CONFIG_STA_SUPPORT */

#ifdef UAPSD_SUPPORT
	UAPSD_Init(pAd);
#endif /* UAPSD_SUPPORT */

	/* assign function pointers */
#ifdef MAT_SUPPORT
	/* init function pointers, used in OS_ABL */
	RTMP_MATOpsInit(pAd);
#endif /* MAT_SUPPORT */


#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
#ifdef MAT_SUPPORT
		MATEngineInit(pAd);
#endif /* MAT_SUPPORT */

	}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
#if defined(MESH_SUPPORT) && defined(MESH_WIRELESS_BRIDGE_SUPPORT)
		MATEngineInit(pAd);
#endif /* endif */
#ifdef DOT11Z_TDLS_SUPPORT
		TDLS_Table_Init(pAd);
#endif /* DOT11Z_TDLS_SUPPORT */

#ifdef WPA_SUPPLICANT_SUPPORT
#ifndef NATIVE_WPA_SUPPLICANT_SUPPORT
		/* send wireless event to wpa_supplicant for infroming interface up. */
		RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CUSTOM, RT_INTERFACE_UP, NULL,
					NULL, 0);
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */
#endif /* WPA_SUPPLICANT_SUPPORT */

	}
#endif /* CONFIG_STA_SUPPORT */

	/* auto-fall back settings */
#ifdef DOT11N_SS3_SUPPORT
	if (pAd->CommonCfg.TxStream >= 3) {
		RTMP_IO_WRITE32(pAd, TX_FBK_CFG_3S_0, 0x12111008);
		RTMP_IO_WRITE32(pAd, TX_FBK_CFG_3S_1, 0x16151413);
	}
#endif /* DOT11N_SS3_SUPPORT */

#ifdef STREAM_MODE_SUPPORT
	RtmpStreamModeInit(pAd);
#endif /* STREAM_MODE_SUPPORT */


#ifdef TXBF_SUPPORT
#ifndef MT76x2
	if (pAd->CommonCfg.ITxBfTimeout) {
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R179, 0x02);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R180, 0);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R182, pAd->CommonCfg.ITxBfTimeout & 0xFF);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R180, 1);
		RTMP_BBP_IO_WRITE8_BY_REG_ID(pAd, BBP_R182,
					     (pAd->CommonCfg.ITxBfTimeout >> 8) & 0xFF);
	}
#endif /* endif */

	if (pAd->CommonCfg.ETxBfTimeout)
		RTMP_IO_WRITE32(pAd, TX_TXBF_CFG_3, pAd->CommonCfg.ETxBfTimeout);
#endif /* TXBF_SUPPORT */


#ifdef MT76x2
	if (IS_MT76x2(pAd)) {
		/*
		* As suggested by BBP/RF (Ben.kao),
		* we need to toggle the ADC clk as following to avoid from ADC no input
		* problem.
		* CRID: WCNCR00052428.
		*/
		RTMP_IO_WRITE32(pAd, RLT_RF_SETTING_0, 0x0);
		RTMP_IO_WRITE32(pAd, RLT_RF_BYPASS_0, 0x06000000);
		RtmpOsMsDelay(5);/* avoid toggle not been excuted due to hw timing */
		RTMP_IO_WRITE32(pAd, RLT_RF_BYPASS_0, 0x0);
		}
#endif /* MT76x2 */

	DBGPRINT_S(Status, ("<==== rt28xx_init, Status=%x\n", Status));

	return TRUE;

err6:

#ifdef IGMP_SNOOP_SUPPORT
	MultiCastFilterTableReset(&pAd->pMulticastFilterTable);
#endif /* IGMP_SNOOP_SUPPORT */

	MeasureReqTabExit(pAd);
	TpcReqTabExit(pAd);
err5:
	RtmpNetTaskExit(pAd);
	UserCfgExit(pAd);
err4:
	MlmeHalt(pAd);
	RTMP_AllTimerListRelease(pAd);
err3:
	RtmpMgmtTaskExit(pAd);
#ifdef RTMP_TIMER_TASK_SUPPORT
	NdisFreeSpinLock(&pAd->TimerQLock);
#endif /* RTMP_TIMER_TASK_SUPPORT */
err2:
	RTMPResetTxRxRingMemory(pAd);

err1:

	MCU_CTRL_EXIT(pAd);

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		/* Free BssTab & ChannelInfo tabbles. */
		AutoChBssTableDestroy(pAd);
		ChannelInfoDestroy(pAd);
	}
#endif /* CONFIG_AP_SUPPORT */


	if (pAd->mpdu_blk_pool.mem)
		os_free_mem(pAd, pAd->mpdu_blk_pool.mem);	/* free BA pool */

#ifdef INF_AMAZON_SE
err0:
#endif /* INF_AMAZON_SE */
#ifdef ST
err0:
#endif /* ST */

	DBGPRINT(RT_DEBUG_ERROR, ("!!! rt28xx init fail !!!\n"));
	return FALSE;
}

VOID RTMPDrvOpen(VOID *pAdSrc)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pAdSrc;

	RTMP_CLEAR_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);
	RTMP_CLEAR_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING);

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

#ifdef RTMP_MAC
	/* TODO: shiang-usw, check this for RMTP_MAC */
	if (pAd->chipCap.hif_type == HIF_RTMP) {
#ifdef RTMP_MAC_PCI
		/* Enable Interrupt */
		RTMP_IRQ_ENABLE(pAd);
#endif /* RTMP_MAC_PCI */

		/* Now Enable RxTx */
		RTMPEnableRxTx(pAd);
	}
#endif /* RTMP_MAC */

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);

#ifdef MT76XX_BTCOEX_SUPPORT
	/* Init BT Coexistence */
	if (IS_MT76XXBTCOMBO(pAd))
		InitBTCoexistence(pAd);
#endif /*MT76XX_BTCOEX_SUPPORT */



#ifdef CONFIG_STA_SUPPORT
#ifdef PCIE_PS_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd)
	    RTMPInitPCIeLinkCtrlValue(pAd);
#endif /* PCIE_PS_SUPPORT */

#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef BG_FT_SUPPORT
	BG_FTPH_Init();
#endif /* BG_FT_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


/* +++Add by shiang for debug */
	DBGPRINT(RT_DEBUG_OFF, ("%s(1):Check if PDMA is idle!\n", __func__));
	AsicWaitPDMAIdle(pAd, 5, 10);
/* ---Add by shiang for debug */

#ifdef CONFIG_STA_SUPPORT
	/*
	   To reduce connection time,
	   do auto reconnect here instead of waiting STAMlmePeriodicExec to do auto reconnect.
	 */
	if (pAd->OpMode == OPMODE_STA)
		MlmeAutoReconnectLastSSID(pAd);
#endif /* CONFIG_STA_SUPPORT */
/* +++Add by shiang for debug */
	DBGPRINT(RT_DEBUG_OFF, ("%s(2):Check if PDMA is idle!\n", __func__));
	AsicWaitPDMAIdle(pAd, 5, 10);
/* ---Add by shiang for debug */

#ifdef CONFIG_STA_SUPPORT
#ifdef DOT11W_PMF_SUPPORT
	if (pAd->OpMode == OPMODE_STA) {
		pAd->StaCfg.PmfCfg.MFPC = FALSE;
		pAd->StaCfg.PmfCfg.MFPR = FALSE;
		pAd->StaCfg.PmfCfg.PMFSHA256 = FALSE;
		if ((pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2
		     || pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2PSK)
		    && (pAd->StaCfg.wdev.WepStatus == Ndis802_11AESEnable)) {
			pAd->StaCfg.PmfCfg.PMFSHA256 = pAd->StaCfg.PmfCfg.Desired_PMFSHA256;
			if (pAd->StaCfg.PmfCfg.Desired_MFPC) {
				pAd->StaCfg.PmfCfg.MFPC = TRUE;
				pAd->StaCfg.PmfCfg.MFPR = pAd->StaCfg.PmfCfg.Desired_MFPR;

				if (pAd->StaCfg.PmfCfg.MFPR)
					pAd->StaCfg.PmfCfg.PMFSHA256 = TRUE;
			}
		} else if (pAd->StaCfg.PmfCfg.Desired_MFPC) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("[PMF]%s:: Security is not WPA2/WPA2PSK AES\n", __func__));
		}

		DBGPRINT(RT_DEBUG_ERROR, ("[PMF]%s:: MFPC=%d, MFPR=%d, SHA256=%d\n",
					  __func__, pAd->StaCfg.PmfCfg.MFPC,
					  pAd->StaCfg.PmfCfg.MFPR, pAd->StaCfg.PmfCfg.PMFSHA256));
	}
#endif /* DOT11W_PMF_SUPPORT */
#endif /* CONFIG_STA_SUPPORT */

#ifdef WSC_INCLUDED
#ifdef CONFIG_AP_SUPPORT
	if ((pAd->OpMode == OPMODE_AP)
	    ) {
		INT index;

		for (index = 0; index < pAd->ApCfg.BssidNum; index++) {
#ifdef HOSTAPD_SUPPORT
			if (pAd->ApCfg.MBSSID[index].Hostapd == Hostapd_EXT) {
				DBGPRINT(RT_DEBUG_TRACE, ("WPS is control by hostapd now.\n"));
			} else
#endif /*HOSTAPD_SUPPORT */
			{
				PWSC_CTRL pWscControl;
				UCHAR zeros16[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0 };

				pWscControl = &pAd->ApCfg.MBSSID[index].WscControl;
				DBGPRINT(RT_DEBUG_TRACE, ("Generate UUID for apidx(%d)\n", index));
				if (NdisEqualMemory
				    (&pWscControl->Wsc_Uuid_E[0], zeros16, UUID_LEN_HEX))
					WscGenerateUUID(pAd, &pWscControl->Wsc_Uuid_E[0],
							&pWscControl->Wsc_Uuid_Str[0], index,
							FALSE);
				WscInit(pAd, FALSE, index);
			}
		}

#ifdef APCLI_SUPPORT
		for (index = 0; index < MAX_APCLI_NUM; index++) {
			PWSC_CTRL pWpsCtrl = &pAd->ApCfg.ApCliTab[index].WscControl;

			pWpsCtrl->pAd = pAd;
			NdisZeroMemory(pWpsCtrl->EntryAddr, MAC_ADDR_LEN);
#ifdef WSC_V2_SUPPORT
			pWpsCtrl->WscConfigMethods = 0x238C;
#else
			pWpsCtrl->WscConfigMethods = 0x018C;
#endif /*WSC_V2_SUPPORT */
			RTMP_AP_IoctlHandle(pAd, NULL, CMD_RTPRIV_IOCTL_WSC_INIT, 0,
					    (VOID *) &pAd->ApCfg.ApCliTab[index], index);
		}
#endif /* APCLI_SUPPORT */
	}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		PWSC_CTRL pWscControl = &pAd->StaCfg.WscControl;

		WscGenerateUUID(pAd, &pWscControl->Wsc_Uuid_E[0], &pWscControl->Wsc_Uuid_Str[0], 0,
				FALSE);
		WscInit(pAd, FALSE, BSS0);
#ifdef WSC_V2_SUPPORT
		WscInitRegistrarPair(pAd, &pAd->StaCfg.WscControl, BSS0);
#endif /* WSC_V2_SUPPORT */
#if ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT)
		if (pAd->WOW_Cfg.bEnable == TRUE)
			ASIC_WOW_DISABLE(pAd);
#endif /*((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) ||
defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT) */

	}
#endif /* CONFIG_STA_SUPPORT */

	/* WSC hardware push button function 0811 */
	WSC_HDR_BTN_Init(pAd);
#endif /* WSC_INCLUDED */
#ifdef MT76XX_BTCOEX_SUPPORT
	/* SendAndesWLANStatus(pAd,WLAN_Device_ON,0); */
	if (IS_MT76XXBTCOMBO(pAd))
		MLMEHook(pAd, WLAN_Device_ON, 0);
#endif /*MT76XX_BTCOEX_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef MULTI_CLIENT_SUPPORT
	pAd->CommonCfg.txRetryCfg = 0;

	{
		UINT32 TxRtyCfg;

		RTMP_IO_READ32(pAd, TX_RTY_CFG, &TxRtyCfg);
		pAd->CommonCfg.txRetryCfg = TxRtyCfg;
	}
#endif /* MULTI_CLIENT_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */

#if defined(RT_CFG80211_P2P_SUPPORT) && defined(SUPPORT_ACS_ALL_CHANNEL_RANK)
	if (pAd->ApCfg.bAutoChannelAtBootup && pAd->ApCfg.bAutoChannelScaned == 0) {
#ifdef RTMP_MAC_PCI
		/* Enable Interrupt first due to we need to scan channel to receive beacons. */
		RTMP_IRQ_ENABLE(pAd);
#endif /* RTMP_MAC_PCI */
#ifdef RTMP_MAC_USB
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS);
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);

		/* Support multiple BulkIn IRP, */
		/* the value on pAd->CommonCfg.NumOfBulkInIRP may be large than 1. */
		{
			UCHAR index;
#ifdef MULTI_URB
			for (index = 0; index < SUBMIT_URB_NUM; index++) {
				DoBulkIn(pAd);
				DBGPRINT(RT_DEBUG_TRACE, ("DoBulkIn!\n"));
			}
#else /* !MULTI_URB */
			for (index = 0; index < pAd->CommonCfg.NumOfBulkInIRP; index++) {
				RTUSBBulkReceive(pAd);
				DBGPRINT(RT_DEBUG_TRACE, ("RTUSBBulkReceive!\n"));
			}
#endif /* MULTI_URB */
		}
#endif /* RTMP_MAC_USB */
		/* Now Enable RxTx */
		RTMPEnableRxTx(pAd);
		/* RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP); */

#ifdef MT762x
		/* TODO: shiang-usw, check why MT76x2 don't need to set this flag here! */
		if (!IS_MT762x(pAd))
#endif /* MT762x */
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_START_UP);
		/* Let BBP register at 20MHz to do scan */
		bbp_set_bw(pAd, BW_20);
		DBGPRINT(RT_DEBUG_ERROR, ("SYNC - BBP R4 to 20MHz.l\n"));
		AP_AUTO_CH_SEL(pAd, pAd->ApCfg.AutoChannelAlg);
		pAd->ApCfg.bAutoChannelScaned = 1;
	}
#endif /* SUPPORT_ACS_ALL_CHANNEL_RANK */

#ifdef CONFIG_MULTI_CHANNEL
	{
		UINT32 tmp;
		/* iwpriv ra0 mac 730 = 13 */
		RTMP_IO_WRITE32(pAd, 0x730, 13);
		RTMP_IO_READ32(pAd, 0x730, &tmp);
		DBGPRINT(RT_DEBUG_ERROR, ("$$$$$ mac 730=%u\n", tmp));
	}
#endif /* CONFIG_MULTI_CHANNEL */

#ifdef ED_MONITOR
	{
		BOOLEAN bEdcca = FALSE;

		bEdcca = GetEDCCASupport(pAd);

		if (bEdcca)
			ed_monitor_init(pAd);
	}
#endif /* ED_MONITOR */

}

VOID RTMPDrvClose(VOID *pAdSrc, VOID *net_dev)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pAdSrc;
	UINT32 i = 0;
	UINT32 MacReg1, MacReg2;

#ifdef LED_CONTROL_SUPPORT
	RTMPExitLEDMode(pAd);
#endif /* LED_CONTROL_SUPPORT */

#ifdef RT_CFG80211_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	if (pAd->cfg80211_ctrl.isCfgInApMode == RT_CMD_80211_IFTYPE_AP
	    && RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_START_UP)) {
		CFG80211DRV_DisableApInterface(pAd, net_dev,
					       RT_CMD_80211_IFTYPE_AP);
		pAd->cfg80211_ctrl.isCfgInApMode = RT_CMD_80211_IFTYPE_STATION;
	}
#endif /* CONFIG_AP_SUPPORT */
#endif /*RT_CFG80211_SUPPORT */

#ifdef BB_SOC
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF)) {
		DBGPRINT(RT_DEBUG_TRACE, ("Radio_ON first....\n"));
		MlmeRadioOn(pAd);
	}
#endif /* BB_SOC */

#ifdef CONFIG_STA_SUPPORT
#ifdef CREDENTIAL_STORE
	if (pAd->IndicateMediaState == NdisMediaStateConnected)
		StoreConnectInfo(pAd);
	else {
		RTMP_SEM_LOCK(&pAd->StaCtIf.Lock);
		pAd->StaCtIf.Changeable = FALSE;
		RTMP_SEM_UNLOCK(&pAd->StaCtIf.Lock);
	}
#endif /* CREDENTIAL_STORE */
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
#ifdef BG_FT_SUPPORT
	BG_FTPH_Remove();
#endif /* BG_FT_SUPPORT */
#endif /* CONFIG_AP_SUPPORT */


#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
#ifdef PCIE_PS_SUPPORT
		RTMPPCIeLinkCtrlValueRestore(pAd, RESTORE_CLOSE);
#endif /* PCIE_PS_SUPPORT */

		/* If dirver doesn't wake up firmware here, */
		/* NICLoadFirmware will hang forever when interface is up again. */
		if (OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_DOZE)) {
			AsicForceWakeup(pAd, TRUE);
		}
#ifdef RTMP_MAC_USB
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_REMOVE_IN_PROGRESS);
#endif /* RTMP_MAC_USB */

#ifdef RTMP_MAC_PCI
		pAd->bPCIclkOff = FALSE;
#endif /* RTMP_MAC_PCI */
	}
#endif /* CONFIG_STA_SUPPORT */

#if ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT)
	if (pAd->WOW_Cfg.bEnable == FALSE)
#endif /*((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) ||
		defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT) */
		{
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
		}
#ifdef EXT_BUILD_CHANNEL_LIST
	if (pAd->CommonCfg.pChDesp != NULL)
		os_free_mem(NULL, pAd->CommonCfg.pChDesp);
	pAd->CommonCfg.pChDesp = NULL;
	pAd->CommonCfg.DfsType = MAX_RD_REGION;
	pAd->CommonCfg.bCountryFlag = 0;
#endif /* EXT_BUILD_CHANNEL_LIST */
	pAd->CommonCfg.bCountryFlag = FALSE;



#ifdef WDS_SUPPORT
	WdsDown(pAd);
#endif /* WDS_SUPPORT */

	for (i = 0; i < NUM_OF_TX_RING; i++) {
		while (pAd->DeQueueRunning[i] == TRUE) {
			DBGPRINT(RT_DEBUG_TRACE, ("Waiting for TxQueue[%d] done..........\n", i));
			RtmpusecDelay(1000);
		}
	}

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		BOOLEAN Cancelled = FALSE;
#ifdef RTMP_MAC_USB
		RTMPCancelTimer(&pAd->CommonCfg.BeaconUpdateTimer, &Cancelled);
#endif /* RTMP_MAC_USB */

		if (pAd->CommonCfg.Bss2040CoexistFlag & BSS_2040_COEXIST_TIMER_FIRED) {
			RTMPCancelTimer(&pAd->CommonCfg.Bss2040CoexistTimer, &Cancelled);
			pAd->CommonCfg.Bss2040CoexistFlag = 0;
		}

		/* PeriodicTimer already been canceled by MlmeHalt() API. */
		/*RTMPCancelTimer(&pAd->PeriodicTimer,  &Cancelled); */
	}
#endif /* CONFIG_AP_SUPPORT */

	/* Stop Mlme state machine */
	MlmeHalt(pAd);

	/* Close net tasklets */
	RtmpNetTaskExit(pAd);

#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
#if ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT)
		if (pAd->WOW_Cfg.bEnable == FALSE)
#endif /* ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB))) && defined(WOW_IFDOWN_SUPPORT) */
			MacTableReset(pAd);
#ifdef MAT_SUPPORT
		MATEngineExit(pAd);
#endif /* MAT_SUPPORT */
#if ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT)
		if (pAd->WOW_Cfg.bEnable == TRUE) {
			RTMP_IO_READ32(pAd, 0x80, &MacReg1);
			RTMP_IO_READ32(pAd, BCN_TIME_CFG, &MacReg2);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("asic wow enable, 0x80 (0x%x), BCN_TIME_CFG (0x%x)\n", MacReg1,
				  MacReg2));
			ASIC_WOW_ENABLE(pAd);
			MCU_CTRL_EXIT(pAd);
		} else
#endif /*((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) ||
defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT) */
			MlmeRadioOff(pAd);
	}
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
#ifdef MAT_SUPPORT
		MATEngineExit(pAd);
#endif /* MAT_SUPPORT */

		/* Shutdown Access Point function, release all related resources */
		APShutdown(pAd);

/*#ifdef AUTO_CH_SELECT_ENHANCE*/
		/* Free BssTab & ChannelInfo tabbles. */
/*		AutoChBssTableDestroy(pAd); */
/*		ChannelInfoDestroy(pAd); */
/*#endif  AUTO_CH_SELECT_ENHANCE */
	}
#endif /* CONFIG_AP_SUPPORT */

	MeasureReqTabExit(pAd);
	TpcReqTabExit(pAd);



	/* Close kernel threads */
	RtmpMgmtTaskExit(pAd);

#if defined(RT_CFG80211_P2P_SUPPORT) && defined(SUPPORT_ACS_ALL_CHANNEL_RANK)
	AutoChBssTableDestroy(pAd);
	ChannelInfoDestroy(pAd);
#endif /* endif */

#ifdef RTMP_MAC_PCI
	{
#if defined(MT76x0) || defined(MT76x2)
		if (IS_MT76x0(pAd) || IS_MT76x2(pAd)) {
			DISABLE_TX_RX(pAd, RTMP_HALT);
		} else
#endif /* MT76x0 */
		{
			if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE)) {
				DISABLE_TX_RX(pAd, RTMP_HALT);
				RTMP_ASIC_INTERRUPT_DISABLE(pAd);
			}
		}

		/* Receive packets to clear DMA index after disable interrupt. */
		/*RTMPHandleRxDoneInterrupt(pAd); */
		/* put to radio off to save power when driver unload.  After radiooff, can't write /read register.  So need to finish all */
		/* register access before Radio off. */

#ifdef RTMP_PCI_SUPPORT
		if (pAd->infType == RTMP_DEV_INF_PCI || pAd->infType == RTMP_DEV_INF_PCIE) {
			BOOLEAN brc = TRUE;

#if defined(MT76x0) || defined(MT76x2)
			/* TODO: shiang, how about RadioOff for 65xx?? */
			if (!(IS_MT76x0(pAd) || IS_MT76x2(pAd)))
#endif /* defined(MT76x0) || defined(MT76x2) */
				brc = RT28xxPciAsicRadioOff(pAd, RTMP_HALT, 0);

/*In  solution 3 of 3090F, the bPCIclkOff will be set to TRUE after calling RT28xxPciAsicRadioOff*/
#ifdef PCIE_PS_SUPPORT
			pAd->bPCIclkOff = FALSE;
#endif /* PCIE_PS_SUPPORT */

			if (brc == FALSE) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s call RT28xxPciAsicRadioOff fail !!\n", __func__));
			}
		}
#endif /* RTMP_PCI_SUPPORT */
	}
#endif /* RTMP_MAC_PCI */
	/* Free IRQ */
	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE)) {
#ifdef RTMP_MAC_PCI
		/* Deregister interrupt function */
		RTMP_OS_IRQ_RELEASE(pAd, net_dev);
#endif /* RTMP_MAC_PCI */
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_IN_USE);
	}
#ifdef SINGLE_SKU_V2
	{
		CH_POWER *ch, *ch_temp;

		DlListForEachSafe(ch, ch_temp, &pAd->SingleSkuPwrList, CH_POWER, List) {
			DlListDel(&ch->List);
			os_free_mem(NULL, ch->Channel);
			os_free_mem(NULL, ch);
		}
	}
#endif /* SINGLE_SKU_V2 */

	/* Free Ring or USB buffers */
	RTMPResetTxRxRingMemory(pAd);

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);

#ifdef WLAN_SKB_RECYCLE
	skb_queue_purge(&pAd->rx0_recycle);
#endif /* WLAN_SKB_RECYCLE */

	/* Free BA reorder resource */
	ba_reordering_resource_release(pAd);

	UserCfgExit(pAd);	/* must after ba_reordering_resource_release */

#ifdef MT76XX_BTCOEX_SUPPORT
	NdisFreeSpinLock(&pAd->CoexMode.UpdateLock);
#endif /* endif */

#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_START_UP);

/*+++Modify by woody to solve the bulk fail+++*/
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
#ifdef DOT11Z_TDLS_SUPPORT
		TDLS_Table_Destory(pAd);
#ifdef TDLS_AUTOLINK_SUPPORT
		TDLS_ClearEntryList(&pAd->StaCfg.TdlsInfo.TdlsDiscovPeerList);
		NdisFreeSpinLock(&pAd->StaCfg.TdlsInfo.TdlsDiscovPeerListSemLock);
		TDLS_ClearEntryList(&pAd->StaCfg.TdlsInfo.TdlsBlackList);
		NdisFreeSpinLock(&pAd->StaCfg.TdlsInfo.TdlsBlackListSemLock);
#endif /* TDLS_AUTOLINK_SUPPORT */
#endif /* DOT11Z_TDLS_SUPPORT */
	}
#endif /* CONFIG_STA_SUPPORT */

	/* clear MAC table */
	/* TODO: do not clear spin lock, such as fLastChangeAccordingMfbLock */
	NdisZeroMemory(&pAd->MacTab, sizeof(MAC_TABLE));

	RTMP_IO_READ32(pAd, 0x80, &MacReg1);
	RTMP_IO_READ32(pAd, 0x1114, &MacReg2);
	DBGPRINT(RT_DEBUG_TRACE, ("close 0x80 (0x%x), BCN_TIME_CFG (0x%x)\n", MacReg1, MacReg2));

	/* release all timers */
	RtmpusecDelay(2000);
	RTMP_AllTimerListRelease(pAd);

#ifdef ED_MONITOR
	if (pAd->ed_chk) {
		DBGPRINT(RT_DEBUG_ERROR, ("@@@ %s: go to ed_monitor_exit()!!\n", __func__));
		ed_monitor_exit(pAd);
	}
#endif /* ED_MONITOR */

#ifdef RTMP_TIMER_TASK_SUPPORT
	NdisFreeSpinLock(&pAd->TimerQLock);
#endif /* RTMP_TIMER_TASK_SUPPORT */

#ifdef CONFIG_FPGA_MODE
#ifdef CAPTURE_MODE
	cap_mode_deinit(pAd);
#endif /* CAPTURE_MODE */
#endif /* CONFIG_FPGA_MODE */

}

VOID RTMPInfClose(VOID *pAdSrc)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pAdSrc;

#ifdef CONFIG_AP_SUPPORT
	pAd->ApCfg.MBSSID[MAIN_MBSSID].bBcnSntReq = FALSE;

	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		/* kick out all STAs behind the bss. */
		MbssKickOutStas(pAd, MAIN_MBSSID, REASON_DISASSOC_INACTIVE);
	}

	/* APMakeAllBssBeacon(pAd); */
	/* APUpdateAllBeaconFrame(pAd); */
#endif /* CONFIG_AP_SUPPORT */


#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
#ifdef PROFILE_STORE
		WriteDatThread(pAd);
		RtmpusecDelay(1000);
#endif /* PROFILE_STORE */
#ifdef QOS_DLS_SUPPORT
		/* send DLS-TEAR_DOWN message, */
		if (pAd->CommonCfg.bDLSCapable) {
			UCHAR i;

			/* tear down local dls table entry */
			for (i = 0; i < MAX_NUM_OF_INIT_DLS_ENTRY; i++) {
				if (pAd->StaCfg.DLSEntry[i].Valid
				    && (pAd->StaCfg.DLSEntry[i].Status == DLS_FINISH)) {
					RTMPSendDLSTearDownFrame(pAd,
								 pAd->StaCfg.DLSEntry[i].MacAddr);
					pAd->StaCfg.DLSEntry[i].Status = DLS_NONE;
					pAd->StaCfg.DLSEntry[i].Valid = FALSE;
				}
			}

			/* tear down peer dls table entry */
			for (i = MAX_NUM_OF_INIT_DLS_ENTRY; i < MAX_NUM_OF_DLS_ENTRY; i++) {
				if (pAd->StaCfg.DLSEntry[i].Valid
				    && (pAd->StaCfg.DLSEntry[i].Status == DLS_FINISH)) {
					RTMPSendDLSTearDownFrame(pAd,
								 pAd->StaCfg.DLSEntry[i].MacAddr);
					pAd->StaCfg.DLSEntry[i].Status = DLS_NONE;
					pAd->StaCfg.DLSEntry[i].Valid = FALSE;
				}
			}
			RTMP_MLME_HANDLER(pAd);
		}
#endif /* QOS_DLS_SUPPORT */

		if (INFRA_ON(pAd) &&
#if ((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) || defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT)
		    /* In WOW state, can't issue disassociation reqeust */
		    pAd->WOW_Cfg.bEnable == FALSE &&
#endif /*((defined(WOW_SUPPORT) && defined(RTMP_MAC_USB)) ||
defined(NEW_WOW_SUPPORT)) && defined(WOW_IFDOWN_SUPPORT) */
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) {
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

			RtmpusecDelay(1000);
		}
#ifdef WPA_SUPPLICANT_SUPPORT
#ifndef NATIVE_WPA_SUPPLICANT_SUPPORT
		/* send wireless event to wpa_supplicant for infroming interface down. */
		RtmpOSWrielessEventSend(pAd->net_dev, RT_WLAN_EVENT_CUSTOM, RT_INTERFACE_DOWN, NULL,
					NULL, 0);
#endif /* NATIVE_WPA_SUPPLICANT_SUPPORT */

		if (pAd->StaCfg.wpa_supplicant_info.pWpsProbeReqIe) {
			os_free_mem(NULL, pAd->StaCfg.wpa_supplicant_info.pWpsProbeReqIe);
			pAd->StaCfg.wpa_supplicant_info.pWpsProbeReqIe = NULL;
			pAd->StaCfg.wpa_supplicant_info.WpsProbeReqIeLen = 0;
		}

		if (pAd->StaCfg.wpa_supplicant_info.pWpaAssocIe) {
			os_free_mem(NULL, pAd->StaCfg.wpa_supplicant_info.pWpaAssocIe);
			pAd->StaCfg.wpa_supplicant_info.pWpaAssocIe = NULL;
			pAd->StaCfg.wpa_supplicant_info.WpaAssocIeLen = 0;
		}
#endif /* WPA_SUPPLICANT_SUPPORT */

	}
#endif /* CONFIG_STA_SUPPORT */

#ifdef RT_CFG80211_P2P_SUPPORT
#ifdef RT_CFG80211_P2P_CONCURRENT_DEVICE
	RTMP_CFG80211_DummyP2pIf_Finalize(pAdSrc);
#endif /* RT_CFG80211_P2P_CONCURRENT_DEVICE */
#endif /* RT_CFG80211_P2P_SUPPORT */
}


PNET_DEV RtmpPhyNetDevMainCreate(VOID *pAdSrc)
{
	RTMP_ADAPTER *pAd = (RTMP_ADAPTER *) pAdSrc;
	PNET_DEV pDevNew;
	UINT32 MC_RowID = 0, IoctlIF = 0;
	char *dev_name;

#ifdef MULTIPLE_CARD_SUPPORT
	MC_RowID = pAd->MC_RowID;
#endif /* MULTIPLE_CARD_SUPPORT */
#ifdef HOSTAPD_SUPPORT
	IoctlIF = pAd->IoctlIF;
#endif /* HOSTAPD_SUPPORT */

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) {
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpPhyNetDevMainCreate fail NO NIC!\n"));
		return NULL;
	}

	dev_name = get_dev_name_prefix(pAd, INT_MAIN);
	if (dev_name == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("RtmpPhyNetDevMainCreate fail dev_name == NULL!\n"));
		return NULL;
	}

	pDevNew = RtmpOSNetDevCreate((INT32) MC_RowID, (UINT32 *) &IoctlIF,
				     INT_MAIN, 0, sizeof(struct mt_dev_priv), dev_name);

#ifdef HOSTAPD_SUPPORT
	pAd->IoctlIF = IoctlIF;
#endif /* HOSTAPD_SUPPORT */

	return pDevNew;
}

#ifdef CONFIG_STA_SUPPORT
#ifdef PROFILE_STORE
static void WriteConfToDatFile(RTMP_ADAPTER *pAd)
{
	char *cfgData = 0, *offset = 0;
	PSTRING fileName = NULL, pTempStr = NULL;
	RTMP_OS_FD file_r, file_w;
	RTMP_OS_FS_INFO osFSInfo;
	LONG rv, fileLen = 0;

	DBGPRINT(RT_DEBUG_TRACE, ("-----> WriteConfToDatFile\n"));

		fileName = STA_PROFILE_PATH;

	RtmpOSFSInfoChange(&osFSInfo, TRUE);

	file_r = RtmpOSFileOpen(fileName, O_RDONLY, 0);
	if (IS_FILE_OPEN_ERR(file_r)) {
		DBGPRINT(RT_DEBUG_TRACE, ("-->1) %s: Error opening file %s\n", __func__, fileName));
		return;
	}
	char tempStr[64] = { 0 };

	while ((rv = RtmpOSFileRead(file_r, tempStr, 64)) > 0)
		fileLen += rv;

	os_alloc_mem(NULL, (UCHAR **) &cfgData, fileLen);
	if (cfgData == NULL) {
		RtmpOSFileClose(file_r);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("CfgData mem alloc fail. (fileLen = %ld)\n", fileLen));
		goto out;
	}
	NdisZeroMemory(cfgData, fileLen);
	RtmpOSFileSeek(file_r, 0);
	rv = RtmpOSFileRead(file_r, (PSTRING) cfgData, fileLen);
	RtmpOSFileClose(file_r);
	if (rv != fileLen) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("CfgData mem alloc fail, fileLen = %ld\n", fileLen));
		goto ReadErr;
	}

	file_w = RtmpOSFileOpen(fileName, O_WRONLY | O_TRUNC, 0);
	if (IS_FILE_OPEN_ERR(file_w)) {
		goto WriteFileOpenErr;
	} else {
		offset = (PCHAR) rtstrstr((PSTRING) cfgData, "Default\n");
		offset += strlen("Default\n");
		RtmpOSFileWrite(file_w, (PSTRING) cfgData, (int)(offset - cfgData));
		os_alloc_mem(NULL, (UCHAR **) &pTempStr, 512);
		if (!pTempStr) {
			DBGPRINT(RT_DEBUG_TRACE, ("pTempStr mem alloc fail. (512)\n"));
			RtmpOSFileClose(file_w);
			goto WriteErr;
		}

		for (;;) {
			int i = 0;
			PSTRING ptr;

			NdisZeroMemory(pTempStr, 512);
			ptr = (PSTRING) offset;
			while (*ptr && *ptr != '\n')
				pTempStr[i++] = *ptr++;

			pTempStr[i] = 0x00;
			if ((size_t) (offset - cfgData) < fileLen) {
				offset += strlen(pTempStr) + 1;
				if (strncmp(pTempStr, "SSID=", strlen("SSID=")) == 0) {
					NdisZeroMemory(pTempStr, 512);
					NdisMoveMemory(pTempStr, "SSID=", strlen("SSID="));
					NdisMoveMemory(pTempStr + 5, pAd->CommonCfg.Ssid,
						       pAd->CommonCfg.SsidLen);
				} else if (strncmp(pTempStr, "AuthMode=", strlen("AuthMode=")) == 0) {
					NdisZeroMemory(pTempStr, 512);
					if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeOpen)
						sprintf(pTempStr, "AuthMode=OPEN");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeShared)
						sprintf(pTempStr, "AuthMode=SHARED");
					else if (pAd->StaCfg.AuthMode ==
						 Ndis802_11AuthModeAutoSwitch)
						sprintf(pTempStr, "AuthMode=WEPAUTO");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPAPSK)
						sprintf(pTempStr, "AuthMode=WPAPSK");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA2PSK)
						sprintf(pTempStr, "AuthMode=WPA2PSK");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA)
						sprintf(pTempStr, "AuthMode=WPA");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPA2)
						sprintf(pTempStr, "AuthMode=WPA2");
					else if (pAd->StaCfg.AuthMode == Ndis802_11AuthModeWPANone)
						sprintf(pTempStr, "AuthMode=WPANONE");
				} else if (strncmp(pTempStr, "EncrypType=", strlen("EncrypType="))
					   == 0) {
					NdisZeroMemory(pTempStr, 512);
					if (pAd->StaCfg.WepStatus == Ndis802_11WEPDisabled)
						sprintf(pTempStr, "EncrypType=NONE");
					else if (pAd->StaCfg.WepStatus == Ndis802_11WEPEnabled)
						sprintf(pTempStr, "EncrypType=WEP");
					else if (pAd->StaCfg.WepStatus == Ndis802_11TKIPEnable)
						sprintf(pTempStr, "EncrypType=TKIP");
					else if (pAd->StaCfg.WepStatus == Ndis802_11AESEnable)
						sprintf(pTempStr, "EncrypType=AES");
				}
				RtmpOSFileWrite(file_w, pTempStr, strlen(pTempStr));
				RtmpOSFileWrite(file_w, "\n", 1);
			} else {
				break;
			}
		}
		RtmpOSFileClose(file_w);
	}

WriteErr:
	if (pTempStr)
		os_free_mem(NULL, pTempStr);
ReadErr:
WriteFileOpenErr:
	if (cfgData)
		os_free_mem(NULL, cfgData);
out:
	RtmpOSFSInfoChange(&osFSInfo, FALSE);

	DBGPRINT(RT_DEBUG_TRACE, ("<----- WriteConfToDatFile\n"));
}

INT write_dat_file_thread(IN ULONG Context)
{
	RTMP_OS_TASK *pTask;
	RTMP_ADAPTER *pAd;
	/* int   Status = 0; */

	pTask = (RTMP_OS_TASK *) Context;

	if (pTask == NULL) {
		DBGPRINT(RT_DEBUG_TRACE, ("%s: pTask is NULL\n", __func__));
		return 0;
	}

	pAd = (PRTMP_ADAPTER) RTMP_OS_TASK_DATA_GET(pTask);

	if (pAd == NULL) {
		DBGPRINT(RT_DEBUG_TRACE, ("%s: pAd is NULL\n", __func__));
		return 0;
	}

	RtmpOSTaskCustomize(pTask);

	/* Update ssid, auth mode and encr type to DAT file */
	WriteConfToDatFile(pAd);

	RtmpOSTaskNotifyToExit(pTask);

	return 0;
}

NDIS_STATUS WriteDatThread(IN RTMP_ADAPTER *pAd)
{
	NDIS_STATUS status = NDIS_STATUS_FAILURE;
	RTMP_OS_TASK *pTask;

	if (pAd->bWriteDat == FALSE)
		return 0;

	DBGPRINT(RT_DEBUG_TRACE, ("-->WriteDatThreadInit()\n"));

	pTask = &pAd->WriteDatTask;

	RTMP_OS_TASK_INIT(pTask, "RtmpWriteDatTask", pAd);
	status = RtmpOSTaskAttach(pTask, write_dat_file_thread, (ULONG) & pAd->WriteDatTask);
	DBGPRINT(RT_DEBUG_TRACE, ("<--WriteDatThreadInit(), status=%d!\n", status));

	return status;
}
#endif /* PROFILE_STORE */
#endif /* CONFIG_STA_SUPPORT */
