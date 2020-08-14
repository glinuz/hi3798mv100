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


#ifdef RT65xx

#include	"rt_config.h"

#ifdef RTMP_USB_SUPPORT
BOOLEAN RT65xxUsbAsicRadioOff(RTMP_ADAPTER *pAd, UCHAR Stage)
{

	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __func__));

	RTMP_SET_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING);

#ifdef RTMP_MAC_USB
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);
	usb_rx_cmd_msgs_receive(pAd);
	RTUSBBulkReceive(pAd);
#endif /* RTMP_MAC_USB */

	andes_suspend_CR_setting(pAd);

	if (IS_USB_INF(pAd)) {
		UINT32 ret;

		RTMP_SEM_EVENT_WAIT(&pAd->hw_atomic, ret);
		if (ret) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return FALSE;
		}
	}

	RTMP_SET_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	if ((Stage == MLME_RADIO_OFF) || (Stage == SUSPEND_RADIO_OFF))
		PWR_SAVING_OP(pAd, RADIO_OFF, 1, 0, 0, 0, 0);

	MCU_CTRL_EXIT(pAd);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);

	/* Stop bulkin pipe */
	/* if((pAd->PendingRx > 0) && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) */
	if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST))) {
		RTUSBCancelPendingBulkInIRP(pAd);
		/* pAd->PendingRx = 0; */
	}

	if (IS_USB_INF(pAd))
		RTMP_SEM_EVENT_UP(&pAd->hw_atomic);

	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __func__));
	return TRUE;
}

BOOLEAN RT65xxUsbAsicRadioOn(RTMP_ADAPTER *pAd, UCHAR Stage)
{
	UINT32 MACValue = 0;
	UINT32 Value;

	RTMP_CLEAR_PSFLAG(pAd, fRTMP_PS_MCU_SLEEP);
#ifdef CONFIG_PM
#ifdef USB_SUPPORT_SELECTIVE_SUSPEND
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	DBGPRINT(RT_DEBUG_TRACE, ("--> %s\n", __func__));

	if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == 1) {
		DBGPRINT(RT_DEBUG_TRACE, ("%s: autopm_resume success\n", __func__));
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
	} else {
		if ((RTMP_Usb_AutoPM_Get_Interface(pObj->pUsb_Dev, pObj->intf)) == (-1)) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: autopm_resume fail ------\n", __func__));
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);
			return FALSE;
		}

		DBGPRINT(RT_DEBUG_TRACE, ("%s: autopm_resume do nothing\n", __func__));
	}

#endif /* USB_SUPPORT_SELECTIVE_SUSPEND */
#endif /* CONFIG_PM */

	if (pAd->WlanFunCtrl.field.WLAN_EN == 0)
		rlt_wlan_chip_onoff(pAd, TRUE, FALSE);

	/* make some traffic to invoke EvtDeviceD0Entry callback function */
	RTUSBReadMACRegister(pAd, 0x1000, &MACValue);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("A MAC query to invoke EvtDeviceD0Entry, MACValue = 0x%x\n", MACValue));

	/* enable RX of MAC block */
	AsicSetRxFilter(pAd);

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0c);

	/* 4. Clear idle flag */
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_SUSPEND);

	MCU_CTRL_INIT(pAd);

	if (IS_USB_INF(pAd)) {
		UINT32 ret;

		RTMP_SEM_EVENT_WAIT(&pAd->hw_atomic, ret);
		if (ret) {
			DBGPRINT(RT_DEBUG_ERROR, ("reg_atomic get failed(ret=%d)\n", ret));
			return FALSE;
		}
	}

	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0c);

	if (IS_USB_INF(pAd))
		RTMP_SEM_EVENT_UP(&pAd->hw_atomic);

	andes_resume_CR_setting(pAd, RADIO_OFF_TYPE);
	DBGPRINT(RT_DEBUG_ERROR, ("andes_resume_CR_setting\n"));

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	if (Stage == MLME_RADIO_ON)
		PWR_SAVING_OP(pAd, RADIO_ON, 1, 0, 0, 0, 0);

	/* MAC Tx/Rx shall be enable */
	RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, 0x0c);
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
	if (Value != 0x0c)
		DBGPRINT(RT_DEBUG_ERROR, ("%s: ERROR!! MAC Tx/Rx not enable\n", __func__));

#ifdef MT76x2
	if (IS_MT76x2(pAd)) {
		/*
		 *	As suggested by BBP/RF (Ben.kao),
		 *	we need to toggle the ADC clk as following to avoid from ADC no input
		 *	problem.
		 *	CRID: WCNCR00035501.
		 */
		RTMP_IO_WRITE32(pAd, RLT_RF_SETTING_0, 0x0);
		RTMP_IO_WRITE32(pAd, RLT_RF_BYPASS_0, 0x06000000);
		RtmpOsMsDelay(5); /* avoid toggle not been excuted due to hw timing */
		RTMP_IO_WRITE32(pAd, RLT_RF_BYPASS_0, 0x0);
	}
#endif /* MT76x2 */

	RTMP_CLEAR_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING);
	DBGPRINT(RT_DEBUG_TRACE, ("<== %s\n", __func__));
	return TRUE;
}
#endif /* RTMP_USB_SUPPORT */

VOID RT65xxDisableTxRx(RTMP_ADAPTER *pAd, UCHAR Level)
{
	UINT32 MacReg = 0;
	UINT32 MTxCycle;
	BOOLEAN bResetWLAN = FALSE;
	BOOLEAN bFree = TRUE;
	UINT8 CheckFreeTimes = 0;

	if (!IS_RT65XX(pAd))
		return;

	DBGPRINT(RT_DEBUG_TRACE, ("----> %s\n", __func__));

	if (Level == RTMP_HALT)
		RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE);

	DBGPRINT(RT_DEBUG_TRACE, ("%s Tx success = %ld\n",
				  __func__,
				  (ULONG) pAd->WlanCounters.TransmittedFragmentCount.u.LowPart));
	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s Tx success = %ld\n", __func__,
		  (ULONG) pAd->WlanCounters.ReceivedFragmentCount.QuadPart));

	StopDmaTx(pAd, Level);

	/*
	   Check page count in TxQ,
	 */
	for (MTxCycle = 0; MTxCycle < 2000; MTxCycle++) {
		BOOLEAN bFree = TRUE;

		RTMP_IO_READ32(pAd, 0x438, &MacReg);
		if (MacReg != 0)
			bFree = FALSE;
		RTMP_IO_READ32(pAd, 0xa30, &MacReg);
		if (MacReg & 0x000000FF)
			bFree = FALSE;
		RTMP_IO_READ32(pAd, 0xa34, &MacReg);
		if (MacReg & 0xFF00FF00)
			bFree = FALSE;
		if (bFree)
			break;
		if (MacReg == 0xFFFFFFFF) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return;
		}
	}

	if (MTxCycle >= 2000) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check TxQ page count max\n"));
		RTMP_IO_READ32(pAd, 0x0a30, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x0a30 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0a34, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x0a34 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x438, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x438 = 0x%08x\n", MacReg));
		bResetWLAN = TRUE;
	}

	/*
	   Check MAC Tx idle
	 */
	for (MTxCycle = 0; MTxCycle < 2000; MTxCycle++) {
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacReg);
		if (MacReg & 0x1)
			RtmpusecDelay(50);
		else
			break;

		if (MacReg == 0xFFFFFFFF) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return;
		}
	}

	if (MTxCycle >= 2000) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check MAC Tx idle max(0x%08x)\n", MacReg));
		bResetWLAN = TRUE;
	}

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) == FALSE) {
		if (Level == RTMP_HALT) {
			/*
			   Disable MAC TX/RX
			 */
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacReg);
			MacReg &= ~(0x0000000c);
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacReg);
		} else {
			/*
			   Disable MAC RX
			 */
			RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &MacReg);
			MacReg &= ~(0x00000008);
			RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, MacReg);
		}
	}

	/*
	   Check page count in RxQ,
	 */
	for (MTxCycle = 0; MTxCycle < 2000; MTxCycle++) {
		bFree = TRUE;
		RTMP_IO_READ32(pAd, 0x430, &MacReg);

		if (MacReg & (0x00FF0000))
			bFree = FALSE;

		RTMP_IO_READ32(pAd, 0xa30, &MacReg);

		if (MacReg != 0)
			bFree = FALSE;

		RTMP_IO_READ32(pAd, 0xa34, &MacReg);

		if (MacReg != 0)
			bFree = FALSE;

		if (bFree && (CheckFreeTimes > 20) && (!is_inband_cmd_processing(pAd)))
			break;

		if (bFree)
			CheckFreeTimes++;

		if (MacReg == 0xFFFFFFFF) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return;
		}
#ifdef RTMP_MAC_USB
		RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);
		usb_rx_cmd_msgs_receive(pAd);
		RTUSBBulkReceive(pAd);
#endif /* endif */
	}

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE);

	if (MTxCycle >= 2000) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check RxQ page count max\n"));

		RTMP_IO_READ32(pAd, 0x0a30, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x0a30 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0a34, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x0a34 = 0x%08x\n", MacReg));

		RTMP_IO_READ32(pAd, 0x0430, &MacReg);
		DBGPRINT(RT_DEBUG_ERROR, ("0x0430 = 0x%08x\n", MacReg));
		bResetWLAN = TRUE;
	}

	/*
	   Check MAC Rx idle
	 */
	for (MTxCycle = 0; MTxCycle < 2000; MTxCycle++) {
		RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &MacReg);
		if (MacReg & 0x2)
			RtmpusecDelay(50);
		else
			break;
		if (MacReg == 0xFFFFFFFF) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return;
		}
	}

	if (MTxCycle >= 2000) {
		DBGPRINT(RT_DEBUG_ERROR, ("Check MAC Rx idle max(0x%08x)\n", MacReg));
		bResetWLAN = TRUE;
	}

	StopDmaRx(pAd, Level);

	if ((Level == RTMP_HALT) && (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST) == FALSE)) {
		if (!pAd->chipCap.ram_code_protect)
			NICEraseFirmware(pAd);

		/*
		 * Disable RF/MAC and do not do reset WLAN under below cases
		 * 1. Combo card
		 * 2. suspend including wow application
		 * 3. radion off command
		 */
		if ((pAd->chipCap.IsComboChip) || RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_SUSPEND)
		    || RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_CMD_RADIO_OFF))
			bResetWLAN = 0;

		rlt_wlan_chip_onoff(pAd, FALSE, bResetWLAN);
	}

	DBGPRINT(RT_DEBUG_TRACE, ("<---- %s\n", __func__));
}

#ifdef RTMP_PCI_SUPPORT
void MT76xx_PciMlmeRadioOn(RTMP_ADAPTER *pAd)
{
#ifdef CONFIG_AP_SUPPORT
	INT32 IdBss, MaxNumBss = pAd->ApCfg.BssidNum;
#endif /* CONFIG_AP_SUPPORT */

#if 0				/* System is not stable if driver would do PM-L1 */
	POS_COOKIE pObj;
	USHORT reg16 = 0, offset = 0;

	pObj = (POS_COOKIE) pAd->OS_Cookie;

#if 0
	/*
	   IMPORTANT @20121018: This patch is for E2 workaround, E3 may not need this workaround.
	   PM-L1 exit
	   Write configuration space "0x44" to "0x8", it will set d-state to D0.
	   Write "0x0001_F044" to "0x0000_0000"  (Please DON'T read it!)
	   PCIe link will stay at L0 state.

	 */
#endif /* endif */
	offset = 0x44;
	reg16 = 0x8;
	PCI_REG_WIRTE_WORD(pObj->pci_dev, offset, reg16);
	RTMP_IO_WRITE32(pAd, 0x1F044, 0x0);
#endif /* if 0 */

#if 0
	if (pObj->parent_pci_dev && (pAd->HostLnkCtrlOffset != 0)) {
		PCI_REG_WIRTE_WORD(pObj->parent_pci_dev, pAd->HostLnkCtrlOffset,
				   pAd->HostLnkCtrlConfiguration);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Restore PCI host : offset 0x%x = 0x%x\n", pAd->HostLnkCtrlOffset,
			  pAd->HostLnkCtrlConfiguration));

		PCI_REG_WIRTE_WORD(pObj->pci_dev, pAd->RLnkCtrlOffset, pAd->RLnkCtrlConfiguration);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("Restore Ralink : offset 0x%x = 0x%x\n", pAd->RLnkCtrlOffset,
			  pAd->RLnkCtrlConfiguration));
	}
#endif /* endif */
	MCU_CTRL_INIT(pAd);

	/* Clear Radio off flag */
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	RTMPRingCleanUp(pAd, QID_AC_BK);
	RTMPRingCleanUp(pAd, QID_AC_BE);
	RTMPRingCleanUp(pAd, QID_AC_VI);
	RTMPRingCleanUp(pAd, QID_AC_VO);
	RTMPRingCleanUp(pAd, QID_HCCA);
	RTMPRingCleanUp(pAd, QID_MGMT);
	RTMPRingCleanUp(pAd, QID_RX);
#ifdef CONFIG_ANDES_SUPPORT
	RTMPRingCleanUp(pAd, QID_CTRL);
#endif /* CONFIG_ANDES_SUPPORT */

	RTMP_ASIC_INTERRUPT_ENABLE(pAd);

	PWR_SAVING_OP(pAd, RADIO_ON, 0, 0, 0, 0, 0);

	andes_load_cr(pAd, RF_BBP_CR, 0, 0);

	RtmpusecDelay(50);

#ifdef DOT11_VHT_AC
	if (pAd->CommonCfg.BBPCurrentBW == BW_80)
		pAd->hw_cfg.cent_ch = pAd->CommonCfg.vht_cent_ch;
	else
#endif /* DOT11_VHT_AC */
		pAd->hw_cfg.cent_ch = pAd->CommonCfg.CentralChannel;

	AsicSwitchChannel(pAd, pAd->hw_cfg.cent_ch, FALSE);
	AsicLockChannel(pAd, pAd->hw_cfg.cent_ch);

#ifdef MT76x2
	if (IS_MT76x2(pAd)) {
		/*
		 * add 504, 50c value per ben kao suggestion for rx receivce packet.
		 */
		RTMP_IO_WRITE32(pAd, 0x50c, 0x0);
		RTMP_IO_WRITE32(pAd, 0x504, 0x06000000);
		RtmpOsMsDelay(5);
		RTMP_IO_WRITE32(pAd, 0x504, 0x0);
	}
#endif /* endif */

#ifdef MT76x0
	if (IS_MT76x0(pAd))
		MT76x0_Calibration(pAd, pAd->hw_cfg.cent_ch, FALSE, TRUE, TRUE);
#endif /* endif */

	/* Enable Tx/Rx */
	RTMPEnableRxTx(pAd);

	if (pAd->chipCap.ed_cca_enable == TRUE) {
		UINT32 mac_val = 0;

		RTMP_IO_READ32(pAd, TXOP_CTRL_CFG, &mac_val);
		mac_val |= (1 << 20);
		RTMP_IO_WRITE32(pAd, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(pAd, TXOP_HLDR_ET, &mac_val);
		mac_val |= 2;
		RTMP_IO_WRITE32(pAd, TXOP_HLDR_ET, mac_val);
	}

	/* Restore RTS retry count */
	RTMP_IO_WRITE32(pAd, 0x1344, pAd->rts_tx_retry_num);

	/* Clear Radio off flag */
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);
	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF);

#ifdef LED_CONTROL_SUPPORT
#ifdef CONFIG_AP_SUPPORT
	/* The LEN_RADIO_ON indicates "Radio on but link down",
	   so AP shall set LED LINK_UP status */
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		RTMPSetLED(pAd, LED_LINK_UP);
	}
#endif /* CONFIG_AP_SUPPORT */
#ifdef CONFIG_STA_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_STA(pAd) {
		/* Set LED */
		RTMPSetLED(pAd, LED_RADIO_ON);
	}
#endif /* CONFIG_STA_SUPPORT */
#endif /* LED_CONTROL_SUPPORT */

	RTMP_OS_NETDEV_START_QUEUE(pAd->net_dev);
#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		if (MaxNumBss > MAX_MBSSID_NUM(pAd))
			MaxNumBss = MAX_MBSSID_NUM(pAd);

		/* first IdBss must not be 0 (BSS0), must be 1 (BSS1) */
		for (IdBss = FIRST_MBSSID; IdBss < MAX_MBSSID_NUM(pAd); IdBss++) {
			if (pAd->ApCfg.MBSSID[IdBss].MSSIDDev)
				RTMP_OS_NETDEV_START_QUEUE(pAd->ApCfg.MBSSID[IdBss].MSSIDDev);
		}
#ifdef A_BAND_SUPPORT
		if ((pAd->CommonCfg.Channel > 14)
			&& (pAd->CommonCfg.bIEEE80211H == 1)
			&& RadarChannelCheck(pAd, pAd->CommonCfg.Channel)) {
#ifdef DFS_SUPPORT
			NewRadarDetectionStart(pAd);
#endif /* DFS_SUPPORT */
		}
#endif /* A_BAND_SUPPORT */
	}
#endif /* CONFIG_AP_SUPPORT */

	RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);
}

void MT76xx_PciMlmeRadioOFF(RTMP_ADAPTER *pAd)
{

	UINT32 pwr_level = 5, mac_val = 0, bbp_val = 0, loop = 0;
	POS_COOKIE pObj;
#ifdef CONFIG_AP_SUPPORT
	INT32 IdBss, MaxNumBss = pAd->ApCfg.BssidNum;
#endif /* CONFIG_AP_SUPPORT */

	pObj = (POS_COOKIE) pAd->OS_Cookie;

	RTMP_OS_NETDEV_STOP_QUEUE(pAd->net_dev);

	RTMP_IO_READ32(pAd, TXOP_CTRL_CFG, &mac_val);
	if ((mac_val & 0x100000) == 0x100000) {
		DBGPRINT(RT_DEBUG_OFF, ("%s:: ED CCA has been enabled\n", __func__));
		pAd->chipCap.ed_cca_enable = TRUE;
		mac_val &= ~(1 << 20);
		RTMP_IO_WRITE32(pAd, TXOP_CTRL_CFG, mac_val);

		RTMP_IO_READ32(pAd, TXOP_HLDR_ET, &mac_val);
		mac_val &= ~2;
		RTMP_IO_WRITE32(pAd, TXOP_HLDR_ET, mac_val);
	}

	RTMP_IO_READ32(pAd, 0x1004, &mac_val);
	mac_val &= ~0xC;
	RTMP_IO_WRITE32(pAd, 0x1004, mac_val);

	/* Backup the original RTS retry count and then set to 0 */
	RTMP_IO_READ32(pAd, 0x1344, &pAd->rts_tx_retry_num);
	RTMP_IO_WRITE32(pAd, 0x1344, 0x00092B00);

	do {
		RTMP_IO_READ32(pAd, 0x1200, &mac_val);
		mac_val &= 0x1;
		RTMP_BBP_IO_READ32(pAd, 0x2130, &bbp_val);
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s wait MAC 0x1200 0x%x BBP 0x2130 0x%x to 0\n",
			 __func__, mac_val, bbp_val));
		RtmpusecDelay(1);
		++loop;
	} while (((mac_val != 0) || (bbp_val != 0)) && (loop < 300));

	if (loop >= 300)
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s wait MAC 0x1200 0x%x BBP 0x2130 0x%x > 300\n",
			  __func__, mac_val, bbp_val));

#ifdef CONFIG_AP_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		if (MaxNumBss > MAX_MBSSID_NUM(pAd))
			MaxNumBss = MAX_MBSSID_NUM(pAd);

		/* first IdBss must not be 0 (BSS0), must be 1 (BSS1) */
		for (IdBss = FIRST_MBSSID; IdBss < MAX_MBSSID_NUM(pAd); IdBss++) {
			if (pAd->ApCfg.MBSSID[IdBss].MSSIDDev)
				RTMP_OS_NETDEV_STOP_QUEUE(pAd->ApCfg.MBSSID[IdBss].MSSIDDev);
		}
	}
#endif /* CONFIG_AP_SUPPORT */

	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_DISABLE_DEQUEUEPACKET);

	DBGPRINT(RT_DEBUG_TRACE, ("\npwr_level = %d\n", pwr_level));

#ifdef AP_SCAN_SUPPORT
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd) {
		BOOLEAN Cancelled;

		RTMPCancelTimer(&pAd->MlmeAux.APScanTimer, &Cancelled);
	}
#endif /* AP_SCAN_SUPPORT */

#ifdef LED_CONTROL_SUPPORT
	RTMPSetLED(pAd, LED_RADIO_OFF);
#endif /* LED_CONTROL_SUPPORT */
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

	PWR_SAVING_OP(pAd, RADIO_OFF, pwr_level, 0, 0, 0, 0);

	RT65xxDisableTxRx(pAd, GUIRADIO_OFF);

	MCU_CTRL_EXIT(pAd);

	/*
	   Wait for Andes firmware receiving this in-band command packet
	 */
	RtmpusecDelay(50);

	/* Set Radio off flag */
	RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF);

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_INTERRUPT_ACTIVE)) {
		RTMP_ASIC_INTERRUPT_DISABLE(pAd);
	}
}
#endif /* RTMP_PCI_SUPPORT */

VOID dump_bw_info(RTMP_ADAPTER *pAd)
{
	UINT32 core_r1, agc_r0, be_r0, band_cfg;
	static UCHAR *bw_str[] = { "20", "10", "40", "80" };
	UCHAR bw, prim_ch_idx, decode_cap;
	static UCHAR *decode_str[] = { "0", "20", "40", "20/40",
		"80", "20/80", "40/80", "20/40/80"
	};
	UCHAR tx_prim;

	RTMP_BBP_IO_READ32(pAd, CORE_R1, &core_r1);
	RTMP_BBP_IO_READ32(pAd, AGC1_R0, &agc_r0);
	RTMP_BBP_IO_READ32(pAd, TXBE_R0, &be_r0);
	RTMP_IO_READ32(pAd, TX_BAND_CFG, &band_cfg);

	/*  Tx/RX : control channel setting */
	DBGPRINT(RT_DEBUG_OFF,
		 ("\n%s():RegisterSetting: TX_BAND_CFG=0x%x, CORE_R1=0x%x, AGC1_R0=0x%x, TXBE_R0=0x%x\n",
		  __func__, band_cfg, core_r1, agc_r0, be_r0));
	bw = ((core_r1 & 0x18) >> 3) & 0xff;
	DBGPRINT(RT_DEBUG_OFF, ("[CORE_R1]\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\tTx/Rx BandwidthCtrl(CORE_R1[4:3])=%d(%s MHz)\n",
				bw, bw_str[bw]));

	DBGPRINT(RT_DEBUG_OFF, ("[AGC_R0]\n"));
	prim_ch_idx = ((agc_r0 & 0x300) >> 8) & 0xff;
	DBGPRINT(RT_DEBUG_OFF, ("\tPrimary Channel Idx(AGC_R0[9:8])=%d\n", prim_ch_idx));
	decode_cap = ((agc_r0 & 0x7000) >> 12);
	DBGPRINT(RT_DEBUG_OFF, ("\tDecodeBWCap(AGC_R0[14:12])=%d(%s MHz Data)\n",
				decode_cap, decode_str[decode_cap]));

	DBGPRINT(RT_DEBUG_OFF, ("[TXBE_R0 - PPM]\n"));
	tx_prim = (be_r0 & 0x3);
	DBGPRINT(RT_DEBUG_OFF, ("\tTxPrimary(TXBE_R0[1:0])=%d\n", tx_prim));
}

#endif /* RT65xx */
