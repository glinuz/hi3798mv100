/*
 ***************************************************************************
 * MediaTek Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2015 MediaTek, Inc.
 *
 * All rights reserved. MediaTek source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek Technology, Inc. is obtained.
 ***************************************************************************
 */

#include "rt_config.h"

#ifdef ED_MONITOR
static INT edcca_tx_stop_start(RTMP_ADAPTER *pAd, BOOLEAN stop)
{
	UINT32 macCfg = 0, macStatus = 0;
	UINT32 MTxCycle;
	ULONG stTime, mt_time;
	UINT32 TxPinCfg = 0;

	/* Disable MAC Tx and wait MAC Tx/Rx status in idle state or direcyl enable tx */
	NdisGetSystemUpTime(&stTime);

	if (stop == TRUE) {
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &macCfg);
		macCfg &= (~0x04);
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, macCfg);

		RTMP_IO_READ32(pAd, TX_PIN_CFG, &TxPinCfg); /* close PA */
		TxPinCfg &= 0xFFFFFFF0;
		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
	} else {
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &macCfg);
		macCfg |= 0x04;
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, macCfg);

		RTMP_IO_READ32(pAd, TX_PIN_CFG, &TxPinCfg);
		TxPinCfg |= 0x0000000F;
		RTMP_IO_WRITE32(pAd, TX_PIN_CFG, TxPinCfg);
	}

	if (stop == TRUE) {
		for (MTxCycle = 0; MTxCycle < 10000; MTxCycle++) {
			RTMP_IO_READ32(pAd, MAC_STATUS_CFG, &macStatus);
			if (macStatus & 0x1)
				RtmpusecDelay(50);
			else
				break;
		}
		NdisGetSystemUpTime(&mt_time);
		mt_time -= stTime;
		if (MTxCycle == 10000)
			DBGPRINT(RT_DEBUG_OFF, ("%s(cnt=%d,time=0x%lx):stop MTx,macStatus=0x%x!\n",
				 __func__, MTxCycle, mt_time, macStatus));
	}

	DBGPRINT(RT_DEBUG_OFF, ("%s():%s tx\n", __func__,
		 ((stop == TRUE) ? "stop" : "start")));

	return TRUE;
}

INT ed_status_read(RTMP_ADAPTER *pAd)
{
	UINT32 period_us = pAd->ed_chk_period * 1000;
	ULONG irqflag;
	INT percent;
	RX_STA_CNT1_STRUC RxStaCnt1;
	UINT32 ch_idle_stat = 0, ch_busy_stat = 0, ed_2nd_stat = 0, ed_stat = 0;

	if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF) ||
	    RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RADIO_OFF) ||
	    RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS))
		return TRUE;

	RTMP_IO_READ32(pAd, CH_IDLE_STA, &ch_idle_stat);
	RTMP_IO_READ32(pAd, 0x1140, &ed_stat);
	RTMP_IO_READ32(pAd, RX_STA_CNT1, &RxStaCnt1.word);

	RTMP_IRQ_LOCK(&pAd->irq_lock, irqflag);

	pAd->ch_idle_stat[pAd->ed_stat_lidx] = ch_idle_stat;
	pAd->ch_busy_stat[pAd->ed_stat_lidx] = ch_busy_stat;
	pAd->ed_2nd_stat[pAd->ed_stat_lidx] = ed_2nd_stat;
	pAd->ed_stat[pAd->ed_stat_lidx] = ed_stat;

	pAd->false_cca_stat[pAd->ed_stat_lidx] += RxStaCnt1.field.FalseCca;

	NdisGetSystemUpTime(&pAd->chk_time[pAd->ed_stat_lidx]);

	if ((pAd->ed_threshold > 0) && (period_us > 0) && (pAd->ed_block_tx_threshold > 0)) {
		percent = (pAd->ed_stat[pAd->ed_stat_lidx] * 100) / period_us;
		if (percent > 100)
			percent = 100;

		/* sync with Shiang's ppt's Algorithm. (20131217) */
		if (percent > pAd->ed_threshold) {
			pAd->ed_trigger_cnt++;
			pAd->ed_silent_cnt = 0;
		} else {
			pAd->ed_trigger_cnt = 0;
			pAd->ed_silent_cnt++;
		}
	}
	pAd->ed_trigger_stat[pAd->ed_stat_lidx] = pAd->ed_trigger_cnt;
	pAd->ed_silent_stat[pAd->ed_stat_lidx] = pAd->ed_silent_cnt;

	INC_RING_INDEX(pAd->ed_stat_lidx, ED_STAT_CNT);
	pAd->false_cca_stat[pAd->ed_stat_lidx] = 0;
	if (pAd->ed_stat_sidx == pAd->ed_stat_lidx)
		INC_RING_INDEX(pAd->ed_stat_sidx, ED_STAT_CNT);

	RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqflag);

	if (pAd->ed_trigger_cnt > pAd->ed_block_tx_threshold) {
		if (pAd->ed_tx_stopped == FALSE) {
			edcca_tx_stop_start(pAd, TRUE);
			pAd->ed_tx_stopped = TRUE;
		}
	}

	if (pAd->ed_silent_cnt > pAd->ed_block_tx_threshold) {
		if (pAd->ed_tx_stopped == TRUE) {
			edcca_tx_stop_start(pAd, FALSE);
			pAd->ed_tx_stopped = FALSE;
		}
	}

	return TRUE;
}

/* this function will be called in multi entry */
INT ed_monitor_exit(RTMP_ADAPTER *pAd)
{
	ULONG irqflag;
	BOOLEAN old_ed_tx_stopped, old_ed_chk;

	RTMP_IRQ_LOCK(&pAd->irq_lock, irqflag);
	DBGPRINT(RT_DEBUG_OFF, ("@@@ %s : ===>\n", __func__));

	NdisZeroMemory(&pAd->ed_stat[0], sizeof(pAd->ed_stat));
	NdisZeroMemory(&pAd->ch_idle_stat[0], sizeof(pAd->ch_idle_stat));
	NdisZeroMemory(&pAd->ch_busy_stat[0], sizeof(pAd->ch_busy_stat));
	NdisZeroMemory(&pAd->chk_time[0], sizeof(pAd->chk_time));
	NdisZeroMemory(&pAd->ed_trigger_stat[0], sizeof(pAd->ed_trigger_stat));
	NdisZeroMemory(&pAd->ed_silent_stat[0], sizeof(pAd->ed_silent_stat));
	NdisZeroMemory(&pAd->false_cca_stat[0], sizeof(pAd->false_cca_stat));

	pAd->ed_stat_lidx = pAd->ed_stat_sidx = 0;
	pAd->ed_trigger_cnt = 0;
	pAd->ed_silent_cnt = 0;
	/* ignore fisrt time's incorrect false cca */
	pAd->ed_false_cca_cnt = 0;

	old_ed_tx_stopped = pAd->ed_tx_stopped;
	old_ed_chk = pAd->ed_chk;

	pAd->ed_tx_stopped = FALSE;
	/* also clear top level flags */
	pAd->ed_chk = FALSE;

	pAd->CommonCfg.bRdg = TRUE;

	DBGPRINT(RT_DEBUG_OFF, ("@@@ %s : <===\n", __func__));
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqflag);

	if (old_ed_tx_stopped)
		edcca_tx_stop_start(pAd, FALSE);

	if (old_ed_chk)
		RTMP_CHIP_ASIC_SET_EDCCA(pAd, FALSE);

	return TRUE;
}

/* open & muanl cmd will call */
VOID ed_monitor_init(RTMP_ADAPTER *pAd)
{
	ULONG irqflag;
	TX_LINK_CFG_STRUCT TxLinkCfg;

	RTMP_IRQ_LOCK(&pAd->irq_lock, irqflag);
	DBGPRINT(RT_DEBUG_OFF, ("@@@ %s : ===>\n", __func__));
	NdisZeroMemory(&pAd->ed_stat[0], sizeof(pAd->ed_stat));
	NdisZeroMemory(&pAd->ch_idle_stat[0], sizeof(pAd->ch_idle_stat));
	NdisZeroMemory(&pAd->ch_busy_stat[0], sizeof(pAd->ch_busy_stat));
	NdisZeroMemory(&pAd->chk_time[0], sizeof(pAd->chk_time));
	NdisZeroMemory(&pAd->ed_trigger_stat[0], sizeof(pAd->ed_trigger_stat));
	NdisZeroMemory(&pAd->ed_silent_stat[0], sizeof(pAd->ed_silent_stat));
	NdisZeroMemory(&pAd->false_cca_stat[0], sizeof(pAd->false_cca_stat));

	pAd->ed_stat_lidx = pAd->ed_stat_sidx = 0;
	pAd->ed_trigger_cnt = 0;
	pAd->ed_silent_cnt = 0;

	/* ignore fisrt time's incorrect false cca */
	pAd->ed_false_cca_cnt = 0;

	pAd->ed_tx_stopped = FALSE;
	/* also set  top level flags */
	pAd->ed_chk = TRUE;

	RTMP_IO_READ32(pAd, TX_LINK_CFG, &TxLinkCfg.word);
	TxLinkCfg.field.TxRDGEn = 0;
	TxLinkCfg.field.TxCFAckEn = 0;
	RTMP_IO_WRITE32(pAd, TX_LINK_CFG, TxLinkCfg.word);

	pAd->CommonCfg.bRdg = FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("@@@ %s : <===\n", __func__));
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqflag);

	RTMP_CHIP_ASIC_SET_EDCCA(pAd, TRUE);
}

INT set_ed_block_tx_thresh(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long block_tx_threshold;
	int ret;

	ret = os_strtol(arg, 10, &block_tx_threshold);
	if (ret < 0)
		return FALSE;

	pAd->ed_block_tx_threshold = block_tx_threshold;
	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_block_tx_threshold=%d\n",
				__func__, pAd->ed_block_tx_threshold));

	return TRUE;
}

INT set_ed_threshold(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long percent;
	int ret;

	ret = os_strtol(arg, 10, &percent);
	if (ret < 0)
		return FALSE;

	if (percent > 100)
		pAd->ed_threshold = (percent % 100);
	else
		pAd->ed_threshold = percent;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_threshold=%d\n", __func__, pAd->ed_threshold));

	return TRUE;
}

INT set_ed_false_cca_threshold(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long false_cca_threshold;
	int ret;

	ret = os_strtol(arg, 10, &false_cca_threshold);
	if (ret < 0)
		return FALSE;

	pAd->false_cca_threshold = false_cca_threshold > 0 ? false_cca_threshold : 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): false_cca_threshold=%d\n",
				__func__, pAd->false_cca_threshold));

	return TRUE;
}

/* let run-time turn on/off */
INT set_ed_chk_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_chk;
	int ret;

	ret = os_strtol(arg, 10, &ed_chk);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_chk=%ld\n", __func__, ed_chk));

	if (ed_chk != 0)
		ed_monitor_init(pAd);
	else
		ed_monitor_exit(pAd);

	return TRUE;
}

#ifdef CONFIG_AP_SUPPORT
INT set_ed_sta_count_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_sta_th;
	int ret;

	ret = os_strtol(arg, 10, &ed_sta_th);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_sta_th=%ld\n", __func__, ed_sta_th));

	pAd->ed_sta_threshold = ed_sta_th;

	return TRUE;
}

INT set_ed_ap_count_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_ap_th;
	int ret;

	ret = os_strtol(arg, 10, &ed_ap_th);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_ap_th=%ld\n", __func__, ed_ap_th));

	pAd->ed_ap_threshold = ed_ap_th;

	return TRUE;
}
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
INT set_ed_ap_scanned_count_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_ap_scanned;
	int ret;

	ret = os_strtol(arg, 10, &ed_ap_scanned);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_ap_scanned=%ld\n", __func__, ed_ap_scanned));

	pAd->ed_ap_scanned = ed_ap_scanned;

	return TRUE;
}

INT set_ed_current_ch_ap_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_current_ch_aps;
	int ret;

	ret = os_strtol(arg, 10, &ed_current_ch_aps);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_current_ch_aps=%ld\n", __func__, ed_current_ch_aps));

	pAd->ed_current_ch_aps = ed_current_ch_aps;

	return TRUE;
}

INT set_ed_current_rssi_threhold_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	long ed_rssi_threshold;
	int ret;

	ret = os_strtol(arg, 10, &ed_rssi_threshold);
	if (ret < 0)
		return FALSE;

	DBGPRINT(RT_DEBUG_OFF, ("%s(): ed_rssi_threshold=%ld\n", __func__, ed_rssi_threshold));

	pAd->ed_rssi_threshold = ed_rssi_threshold;

	return TRUE;
}
#endif /* CONFIG_STA_SUPPORT */

INT show_ed_stat_proc(RTMP_ADAPTER *pAd, PSTRING arg)
{
	unsigned long irqflags;
	UINT32 ed_stat[ED_STAT_CNT], ed_2nd_stat[ED_STAT_CNT], false_cca_stat[ED_STAT_CNT];
	UINT32 silent_stat[ED_STAT_CNT], trigger_stat[ED_STAT_CNT];
	UINT32 busy_stat[ED_STAT_CNT], idle_stat[ED_STAT_CNT];
	ULONG chk_time[ED_STAT_CNT];
	INT period_us;
	UCHAR start, end, idx;

	RTMP_IRQ_LOCK(&pAd->irq_lock, irqflags);
	start = pAd->ed_stat_sidx;
	end = pAd->ed_stat_lidx;
	NdisMoveMemory(&ed_stat[0], &pAd->ed_stat[0], sizeof(ed_stat));
	NdisMoveMemory(&ed_2nd_stat[0], &pAd->ed_2nd_stat[0], sizeof(ed_2nd_stat));
	NdisMoveMemory(&busy_stat[0], &pAd->ch_busy_stat[0], sizeof(busy_stat));
	NdisMoveMemory(&idle_stat[0], &pAd->ch_idle_stat[0], sizeof(idle_stat));
	NdisMoveMemory(&chk_time[0], &pAd->chk_time[0], sizeof(chk_time));
	NdisMoveMemory(&trigger_stat[0], &pAd->ed_trigger_stat[0], sizeof(trigger_stat));
	NdisMoveMemory(&silent_stat[0], &pAd->ed_silent_stat[0], sizeof(silent_stat));
	NdisMoveMemory(&false_cca_stat[0], &pAd->false_cca_stat[0], sizeof(false_cca_stat));
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, irqflags);

#ifdef CONFIG_AP_SUPPORT
	DBGPRINT(RT_DEBUG_OFF,
		 ("Dump ChannelBusy Counts, ed_chk=%u, ed_sta_threshold=%u, ed_ap_threshold=%u, ",
		  pAd->ed_chk, pAd->ed_sta_threshold, pAd->ed_ap_threshold));
	DBGPRINT(RT_DEBUG_OFF,
		 ("false_cca_threshold=%u, ChkPeriod=%dms,ED_Threshold=%d%%,HitCntForBlockTx=%d\n",
		  pAd->false_cca_threshold, pAd->ed_chk_period, pAd->ed_threshold,
		  pAd->ed_block_tx_threshold));
#endif /* endif */

#ifdef CONFIG_STA_SUPPORT
	DBGPRINT(RT_DEBUG_OFF, ("Dump ChannelBusy Counts, "));
	DBGPRINT(RT_DEBUG_OFF,
		 ("ed_chk=%u, ed_ap_scanned=%u, ed_current_ch_aps=%u, ed_rssi_threshold=%d, ",
		  pAd->ed_chk, pAd->ed_ap_scanned, pAd->ed_current_ch_aps, pAd->ed_rssi_threshold));
	DBGPRINT(RT_DEBUG_OFF,
		 ("false_cca_threshold=%u, ChkPeriod=%dms,ED_Threshold=%d%%,HitCntForBlockTx=%d\n",
		  pAd->false_cca_threshold, pAd->ed_chk_period, pAd->ed_threshold,
		  pAd->ed_block_tx_threshold));
#endif /* endif */

	period_us = pAd->ed_chk_period * 1000;
	DBGPRINT(RT_DEBUG_OFF, ("TimeSlot:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("%ld  ", chk_time[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Dump ED_STAT\n"));
	DBGPRINT(RT_DEBUG_OFF, ("RawCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("%d  ", ed_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Percent:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", (ed_stat[idx] * 100) / period_us));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("FalseCCA:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", false_cca_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("TriggerCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", trigger_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);

	DBGPRINT(RT_DEBUG_OFF, ("SilentCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", silent_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n==========================\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Dump ED_2nd_STAT\n"));
	DBGPRINT(RT_DEBUG_OFF, ("RawCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("%d  ", ed_2nd_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Percent:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", (ed_2nd_stat[idx] * 100) / period_us));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\n==========================\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Dump CH_IDLE_STAT\n"));
	DBGPRINT(RT_DEBUG_OFF, ("RawCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("%d  ", idle_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Percent:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", (idle_stat[idx] * 100) / period_us));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\n==========================\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Dump CH_BUSY_STAT\n"));
	DBGPRINT(RT_DEBUG_OFF, ("RawCnt:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("%d  ", busy_stat[idx]));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("Percent:"));
	idx = start;
	do {
		DBGPRINT(RT_DEBUG_OFF, ("\t%d", (busy_stat[idx] * 100) / period_us));
		INC_RING_INDEX(idx, ED_STAT_CNT);
	} while (idx != end);
	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	DBGPRINT(RT_DEBUG_OFF, ("\n==========================\n"));

	return TRUE;
}
#endif /* ED_MONITOR */
