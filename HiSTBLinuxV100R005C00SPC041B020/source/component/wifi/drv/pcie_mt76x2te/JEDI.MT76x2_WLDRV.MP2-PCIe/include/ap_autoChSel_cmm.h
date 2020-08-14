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


#ifndef __AUTOCHSELECT_CMM_H__
#define __AUTOCHSELECT_CMM_H__

#define RSSI_TO_DBM_OFFSET          120	/* RSSI-115 = dBm */
#define ACS_DIRTINESS_RSSI_STRONG   (-50)	/* dBm */
#define ACS_DIRTINESS_RSSI_WEAK     (-80)	/* dBm */
#define ACS_FALSECCA_THRESHOLD      (100)

typedef struct {
	ULONG dirtyness[MAX_NUM_OF_CHANNELS + 1];
	ULONG ApCnt[MAX_NUM_OF_CHANNELS + 1];
	UINT32 FalseCCA[MAX_NUM_OF_CHANNELS + 1];
	BOOLEAN SkipList[MAX_NUM_OF_CHANNELS + 1];
#ifdef AP_QLOAD_SUPPORT
	UINT32 chanbusytime[MAX_NUM_OF_CHANNELS + 1];	/* QLOAD ALARM */
#endif				/* AP_QLOAD_SUPPORT */
	BOOLEAN IsABand;
} CHANNELINFO, *PCHANNELINFO;

typedef struct {
	UCHAR Bssid[MAC_ADDR_LEN];
	UCHAR SsidLen;
	CHAR Ssid[MAX_LEN_OF_SSID];
	UCHAR Channel;
	UCHAR ExtChOffset;
	CHAR Rssi;
} BSSENTRY, *PBSSENTRY;

typedef struct {
	UCHAR BssNr;
	BSSENTRY BssEntry[MAX_LEN_OF_BSS_TABLE];
} BSSINFO, *PBSSINFO;

typedef struct {
	UCHAR ch;
	ULONG dirtyness;
	ULONG falseCCA;
} ACS_SORT_ENTRY;

typedef enum ChannelSelAlg {
	ChannelAlgCombined,	/* Default */
	ChannelAlgApCnt,
	ChannelAlgCCA,
	ChannelAlgRandom	/*use by Dfs */
} ChannelSel_Alg;

#endif /* __AUTOCHSELECT_CMM_H__ */
