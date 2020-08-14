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

#ifndef __HOTSPOT_H__
#define __HOTSPOT_H__

#ifndef CONFIG_DOT11V_WNM
#error "For HOTSPOT2.0 feature, you must define the compile flag -DCONFIG_DOT11V_WNM"
#endif /* endif */

#ifndef CONFIG_DOT11U_INTERWORKING
#error "For HOTSPOT2.0 feature, you must define the compile flag -DCONFIG_DOT11U_INTERWORKING"
#endif /* endif */

#include "rtmp.h"

#define HSCTRL_MACHINE_BASE 0

enum HSCTRL_STATE {
	WAIT_HSCTRL_OFF,
	WAIT_HSCTRL_ON,
	HSCTRL_IDLE,
	MAX_HSCTRL_STATE,
};

enum HSCTRL_EVENT {
	HSCTRL_OFF,
	HSCTRL_ON,
	MAX_HSCTRL_MSG,
};

typedef struct GNU_PACKED _HSCTRL_EVENT_DATA {
	UCHAR ControlIndex;
	UCHAR EventTrigger;
	UCHAR EventType;
} HSCTRL_EVENT_DATA, *PHSCTRL_EVENT_DATA;

typedef struct _HOTSPOT_CTRL {
	UINT32 HSIndicationIELen;
	UINT32 P2PIELen;
	UINT32 InterWorkingIELen;
	UINT32 AdvertisementProtoIELen;
	UINT32 QosMapSetIELen;
	UINT32 RoamingConsortiumIELen;
	PUCHAR InterWorkingIE;
	PUCHAR AdvertisementProtoIE;
	PUCHAR QosMapSetIE;
	PUCHAR RoamingConsortiumIE;
	PUCHAR HSIndicationIE;
	PUCHAR P2PIE;
	BOOLEAN HSDaemonReady;
	BOOLEAN HotSpotEnable;
	enum HSCTRL_STATE HSCtrlState;
	BOOLEAN IsHessid;
	UCHAR Hessid[MAC_ADDR_LEN];
	UINT8 AccessNetWorkType;
	BOOLEAN DGAFDisable;
	UINT8 L2Filter;
	BOOLEAN ICMPv4Deny;
	UINT32 MMPDUSize;
} HOTSPOT_CTRL, *PHOTSPOT_CTRL;

enum {
	L2FilterDisable,
	L2FilterBuiltIn,
	L2FilterExternal,
};

enum {
	PARAM_DGAF_DISABLED,
	PARAM_PROXY_ARP,
	PARAM_L2_FILTER,
	PARAM_ICMPV4_DENY,
	PARAM_MMPDU_SIZE,
	PARAM_EXTERNAL_ANQP_SERVER_TEST,
};

BOOLEAN L2FilterInspection(IN PRTMP_ADAPTER pAd, IN PHOTSPOT_CTRL pHSCtrl, IN PUCHAR pData);

VOID HSCtrlStateMachineInit(IN PRTMP_ADAPTER pAd,
			    IN STATE_MACHINE * S, OUT STATE_MACHINE_FUNC Trans[]);

INT Set_STAHotSpot_OnOff(IN PRTMP_ADAPTER pAd, IN UINT8 OnOff);

enum HSCTRL_STATE HSCtrlCurrentState(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

BOOLEAN HotSpotEnable(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem, IN INT Type);

VOID HSCtrlExit(IN PRTMP_ADAPTER pAd);

VOID HSCtrlHalt(IN PRTMP_ADAPTER pAd);

INT Set_HotSpot_OnOff(IN PRTMP_ADAPTER pAd,
		      IN UINT8 OnOff, IN UINT8 EventTrigger, IN UINT8 EventType);

struct _PEER_PROBE_REQ_PARAM;

BOOLEAN ProbeReqforHSAP(IN PRTMP_ADAPTER pAd,
			IN UCHAR APIndex, IN struct _PEER_PROBE_REQ_PARAM *ProbeReqParam);

VOID Clear_Hotspot_All_IE(IN PRTMP_ADAPTER PAd);

#define isBcastEtherAddr(addr)  ((addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) == 0xff)

#define GAS_STATE_MESSAGES    0
#define ACTION_STATE_MESSAGES 1

void HotspotOnOffEvent(PNET_DEV net_dev, int onoff);
void HotspotAPReload(PNET_DEV net_dev);

INT Set_HotSpot_Param(IN PRTMP_ADAPTER pAd, UINT32 Param, UINT32 Value);

enum {
	HS_ON_OFF_BASE,
	HS_AP_RELOAD,
};

#ifdef CONFIG_AP_SUPPORT
BOOLEAN HSIPv4Check(IN PRTMP_ADAPTER pAd,
		    PUCHAR pWcid,
		    PNDIS_PACKET pPacket, PUCHAR pSrcBUf, UINT16 srcPort, UINT16 dscPort);
#endif /* endif */
#endif /* __HOTSPOT_H__ */
