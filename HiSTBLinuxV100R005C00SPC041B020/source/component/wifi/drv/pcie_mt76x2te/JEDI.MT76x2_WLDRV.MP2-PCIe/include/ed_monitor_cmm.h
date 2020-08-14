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
