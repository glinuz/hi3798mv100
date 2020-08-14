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
#ifndef	__ED_MONITOR_COMM_H__
#define __ED_MONITOR_COMM_H__

#ifdef ED_MONITOR

#include "rtmp.h"

extern INT ed_status_read(RTMP_ADAPTER *pAd);
extern VOID ed_monitor_init(RTMP_ADAPTER *pAd);
extern INT ed_monitor_exit(RTMP_ADAPTER *pAd);

/* let run-time turn on/off */
extern INT set_ed_chk_proc(RTMP_ADAPTER *pAd, PSTRING arg);

#ifdef CONFIG_AP_SUPPORT
extern INT set_ed_sta_count_proc(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT set_ed_ap_count_proc(RTMP_ADAPTER *pAd, PSTRING arg);
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
extern INT set_ed_ap_scanned_count_proc(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT set_ed_current_ch_ap_proc(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT set_ed_current_rssi_threhold_proc(RTMP_ADAPTER *pAd, PSTRING arg);
#endif /* CONFIG_STA_SUPPORT */

extern INT set_ed_block_tx_thresh(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT set_ed_false_cca_threshold(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT set_ed_threshold(RTMP_ADAPTER *pAd, PSTRING arg);
extern INT show_ed_stat_proc(RTMP_ADAPTER *pAd, PSTRING arg);
#endif /* ED_MONITOR */

#endif /* __ED_MONITOR_COMM_H__ */
