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


#ifndef __CHLIST_H__
#define __CHLIST_H__

#include "rtmp_type.h"
#include "rtmp_def.h"

typedef struct _CH_DESC {
	UCHAR FirstChannel;
	UCHAR NumOfCh;
	UCHAR ChannelProp;
	UCHAR ChannelWidth;
} CH_DESC, *PCH_DESC;

typedef struct _COUNTRY_REGION_CH_DESC {
	UCHAR RegionIndex;
	PCH_DESC pChDesc;
} COUNTRY_REGION_CH_DESC, *PCOUNTRY_REGION_CH_DESC;

#ifdef EXT_BUILD_CHANNEL_LIST
#define ODOR			0
#define IDOR			1
#define BOTH			2

typedef struct _CH_DESP {
	UCHAR FirstChannel;
	UCHAR NumOfCh;
	CHAR MaxTxPwr;		/* dBm */
	UCHAR Geography;	/* 0:out door, 1:in door, 2:both */
	BOOLEAN DfsReq;		/* Dfs require, 0: No, 1: yes. */
} CH_DESP, *PCH_DESP;

typedef struct _CH_REGION {
	UCHAR CountReg[3];
	UCHAR DfsType;		/* 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56 */
	CH_DESP *pChDesp;
	BOOLEAN edcca_on;
} CH_REGION, *PCH_REGION;

extern CH_REGION ChRegion[];
#endif /* EXT_BUILD_CHANNEL_LIST */

#ifdef ED_MONITOR
typedef struct _COUNTRY_PROP {
	UCHAR CountReg[3];
	UCHAR DfsType;		/* 0: CE, 1: FCC, 2: JAP, 3:JAP_W53, JAP_W56 */
	BOOLEAN edcca_on;
} COUNTRY_PROP, *PCOUNTRY_PROP;
#endif /* ED_MONITOR */

#ifdef SINGLE_SKU_V2
/* TODO: shiang-usw, integrate these data structures to a single one! */
#define	SINGLE_SKU_TABLE_LENGTH	  (SINGLE_SKU_TABLE_CCK_LENGTH+   \
	SINGLE_SKU_TABLE_OFDM_LENGTH+(SINGLE_SKU_TABLE_HT_LENGTH*2))

#define SINGLE_SKU_TABLE_CCK_LENGTH	4
#define SINGLE_SKU_TABLE_OFDM_LENGTH	8
#define SINGLE_SKU_TABLE_HT_LENGTH	16
#define SINGLE_SKU_TABLE_VHT_LENGTH	10	/* VHT80 MCS 0 ~ 9 */
#ifdef CUSTOMIZE_SINGLE_SKU_V2
#define SINGLE_SKU_TABLE_VHT20_LENGTH	1	/* VHT MCS 8 */
#define SINGLE_SKU_TABLE_VHT40_LENGTH	2	/* VHT MCS 8, 9 */
#endif /* CUSTOMIZE_SINGLE_SKU_V2 */

/* TODO: shiang-usw, need to re-organize these for MT7610/MT7601/MT7620!! */
typedef struct _CH_POWER_ {
	DL_LIST List;
	UCHAR StartChannel;
	UCHAR num;
	UCHAR *Channel;
	UCHAR PwrCCK[SINGLE_SKU_TABLE_CCK_LENGTH];
	UCHAR PwrOFDM[SINGLE_SKU_TABLE_OFDM_LENGTH];
	UCHAR PwrHT20[SINGLE_SKU_TABLE_HT_LENGTH];
	UCHAR PwrHT40[SINGLE_SKU_TABLE_HT_LENGTH];
	UCHAR PwrVHT80[SINGLE_SKU_TABLE_VHT_LENGTH];
#ifdef CUSTOMIZE_SINGLE_SKU_V2
	UCHAR PwrVHT20[SINGLE_SKU_TABLE_VHT20_LENGTH];	/* VHT20 MCS 8 */
	UCHAR PwrVHT40[SINGLE_SKU_TABLE_VHT40_LENGTH];	/* VHT40 MCS 8,9 */
	UCHAR PwrVHT40All[SINGLE_SKU_TABLE_VHT_LENGTH];	/* VHT40 MCS 0~9 */
#endif				/* CUSTOMIZE_SINGLE_SKU_V2 */

} CH_POWER;
#ifdef JUNGLE_SUPPORT
#define REG_NAME_LEN 2
struct _PRELOAD_SKU_LIST {
	DL_LIST List;
	UCHAR Country[REG_NAME_LEN];
	DL_LIST sku_pwr_list;
	BOOLEAN in_use;
};
#endif
#endif /* SINGLE_SKU_V2 */

typedef struct _CH_FREQ_MAP_ {
	UINT16 channel;
	UINT16 freqKHz;
} CH_FREQ_MAP;

extern CH_FREQ_MAP CH_HZ_ID_MAP[];
extern int CH_HZ_ID_MAP_NUM;

#define     MAP_CHANNEL_ID_TO_KHZ(_ch, _khz)                 \
			RTMP_MapChannelID2KHZ(_ch, (UINT32 *)&(_khz))
#define     MAP_KHZ_TO_CHANNEL_ID(_khz, _ch)                 \
			RTMP_MapKHZ2ChannelID(_khz, (INT *)&(_ch))

/* Check if it is Japan W53(ch52,56,60,64) channel. */
#define JapanChannelCheck(_ch)  ((_ch == 52) || (_ch == 56) || (_ch == 60) || (_ch == 64))

#ifdef ED_MONITOR
BOOLEAN GetEDCCASupport(IN PRTMP_ADAPTER pAd);
#endif /* ED_MONITOR */

#ifdef EXT_BUILD_CHANNEL_LIST
VOID BuildChannelListEx(IN PRTMP_ADAPTER pAd);

VOID BuildBeaconChList(IN PRTMP_ADAPTER pAd, OUT PUCHAR pBuf, OUT PULONG pBufLen);
#endif /* EXT_BUILD_CHANNEL_LIST */

VOID N_ChannelCheck(RTMP_ADAPTER *pAd);
UCHAR N_SetCenCh(RTMP_ADAPTER *pAd, UCHAR channel);
BOOLEAN N_ChannelGroupCheck(RTMP_ADAPTER *pAd, UCHAR channel);
#ifdef DOT11_VHT_AC
INT get_vht_neighbor_index(UCHAR channel);
BOOLEAN AC_ChannelGroupCheck(RTMP_ADAPTER *pAd, UCHAR channel);
#endif /* DOT11_VHT_AC */

UINT8 GetCuntryMaxTxPwr(IN PRTMP_ADAPTER pAd, IN UINT8 channel);

VOID RTMP_MapChannelID2KHZ(IN UCHAR Ch, OUT UINT32 *pFreq);

VOID RTMP_MapKHZ2ChannelID(IN ULONG Freq, OUT INT *pCh);

UCHAR GetChannel_5GHZ(IN PCH_DESC pChDesc, IN UCHAR index);

UCHAR GetChannel_2GHZ(IN PCH_DESC pChDesc, IN UCHAR index);

UCHAR GetChannelFlag(IN PCH_DESC pChDesc, IN UCHAR index);

UINT16 TotalChNum(IN PCH_DESC pChDesc);

INT32 TotalRuleNum(IN PCH_DESC pChDesc);

#endif /* __CHLIST_H__ */
