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

#ifndef __GAS_H__
#define __GAS_H__

#include "link_list.h"

#define GAS_MACHINE_BASE 0

/* gas states */
enum GAS_STATE {
	WAIT_GAS_REQ,
	WAIT_GAS_RSP,
	WAIT_PEER_GAS_REQ,
	WAIT_PEER_GAS_RSP,
	WAIT_GAS_CB_REQ,
	WAIT_GAS_CB_RSP,
	GAS_UNKNOWN,
	MAX_GAS_STATE,
};

/* gas events */
enum GAS_EVENT {
	GAS_REQ,
	GAS_RSP,
	GAS_RSP_MORE,
	PEER_GAS_REQ,			
	PEER_GAS_RSP,
	PEER_GAS_RSP_MORE,
	GAS_CB_REQ,
	GAS_CB_REQ_MORE,
	GAS_CB_RSP,
	GAS_CB_RSP_MORE,
	MAX_GAS_MSG,
};

#define GAS_FUNC_SIZE (MAX_GAS_STATE * MAX_GAS_MSG)

typedef struct _GAS_QUERY_RSP_FRAGMENT
{
	DL_LIST List;
	UCHAR GASRspFragID;
	UINT16 FragQueryRspLen;
	UCHAR *FragQueryRsp;
}GAS_QUERY_RSP_FRAGMENT, *PGAS_QUERY_RSP_FRAGMENT;

typedef struct _GAS_PEER_ENTRY {
	DL_LIST List;
	enum GAS_STATE CurrentState;
	UCHAR ControlIndex;
	UCHAR PeerMACAddr[MAC_ADDR_LEN];
	UCHAR DialogToken;
	UCHAR AdvertisementProID;
	void *Priv;
#ifdef CONFIG_AP_SUPPORT
	RALINK_TIMER_STRUCT PostReplyTimer;
	BOOLEAN PostReplyTimerRunning;
	RALINK_TIMER_STRUCT GASRspBufferingTimer;
	BOOLEAN GASRspBufferingTimerRunning;
#endif /* CONFIG_AP_SUPPORT */

#ifdef CONFIG_STA_SUPPORT
	RALINK_TIMER_STRUCT GASResponseTimer;
	BOOLEAN GASResponseTimerRunning;
	RALINK_TIMER_STRUCT GASCBDelayTimer;
	BOOLEAN GASCBDelayTimerRunning;
#endif /* CONFIG_STA_SUPPORT */
	UCHAR GASRspFragNum;
	UCHAR CurrentGASFragNum;
	UINT32 AllocResource;
	UINT32 FreeResource;
	UCHAR QueryNum;
	DL_LIST GASQueryRspFragList;
}GAS_PEER_ENTRY, *PGAS_PEER_ENTRY;

typedef struct _GAS_CTRL {
	DL_LIST GASPeerList;
	NDIS_SPIN_LOCK GASPeerListLock;
	UINT8 ExternalANQPServerTest;
} GAS_CTRL, *PGAS_CTRL; 

/* 
 * gas events data 
 * GASComebackDelay : unit(TU)
 */
typedef struct GNU_PACKED _GAS_EVENT_DATA {
	UCHAR ControlIndex; 
	UCHAR PeerMACAddr[MAC_ADDR_LEN];
	UINT16 EventType;
	union{
#ifdef CONFIG_STA_SUPPORT
		struct {
			UCHAR DialogToken;	
			UCHAR AdvertisementProID;
			UINT16 QueryReqLen;
			UCHAR QueryReq[0];
		} GNU_PACKED GAS_REQ_DATA;
		struct {
			UINT16 StatusCode;
			UCHAR AdvertisementProID;
			UINT16 QueryRspLen;
			UCHAR QueryRsp[0];
		} GNU_PACKED PEER_GAS_RSP_DATA;
		struct {
			UCHAR DialogToken;
		} GNU_PACKED PEER_GAS_RSP_MORE_DATA;
		struct {
			UINT16 StatusCode;
			UCHAR AdvertisementProID;
			UINT16 QueryRspLen;
			UCHAR QueryRsp[0];
		} GNU_PACKED GAS_CB_RSP_DATA;
		struct {
			UCHAR DialogToken;
		} GNU_PACKED GAS_CB_RSP_MORE_DATA;
#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
		struct {
			UCHAR DialogToken;
			UINT16 StatusCode;
			UINT16 GASComebackDelay;
			UCHAR AdvertisementProID;
			UINT16 QueryRspLen;
			UCHAR QueryRsp[0];
		} GNU_PACKED GAS_RSP_DATA;
		struct {
			UCHAR DialogToken;
			UINT16 StatusCode;
			UINT16 GASComebackDelay;
			UCHAR AdvertisementProID;
		} GNU_PACKED GAS_RSP_MORE_DATA;
		struct {
			UCHAR DialogToken;
			UCHAR AdvertisementProID;
			UINT16 QueryReqLen;	
			UCHAR QueryReq[0];
		} GNU_PACKED PEER_GAS_REQ_DATA;
		struct {
			UCHAR DialogToken;
			UCHAR AdvertisementProID;
			UINT16 StatusCode;
		} GNU_PACKED GAS_CB_REQ_DATA;
		struct {
			UCHAR DialogToken;
			UCHAR AdvertisementProID;
			UINT16 StatusCode;
		} GNU_PACKED GAS_CB_REQ_MORE_DATA;
#endif /* CONFIG_AP_SUPPORT */
	}u;
}GAS_EVENT_DATA, *PGAS_EVENT_DATA;

VOID GASStateMachineInit(
	IN	PRTMP_ADAPTER		pAd, 
	IN	STATE_MACHINE		*S, 
	OUT	STATE_MACHINE_FUNC	Trans[]);

enum GAS_STATE GASPeerCurrentState(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem);

VOID GASSetPeerCurrentState(
	IN PRTMP_ADAPTER pAd,
	IN MLME_QUEUE_ELEM *Elem,
	IN enum GAS_STATE State);

VOID GASCtrlExit(IN PRTMP_ADAPTER pAd);

#ifdef CONFIG_STA_SUPPORT
VOID ReceiveGASInitRsp(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

VOID ReceiveGASCBRsp(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

void SendAnqpRspEvent(void *net_dev, const char *peer_mac_addr,
				u16 status, const char *anqp_rsp, u16 anqp_rsp_len);

#endif /* CONFIG_STA_SUPPORT */

#ifdef CONFIG_AP_SUPPORT
DECLARE_TIMER_FUNCTION(PostReplyTimeout); 
DECLARE_TIMER_FUNCTION(GASRspBufferingTimeout);

void SendAnqpReqEvent(PNET_DEV net_dev, const char *peer_mac_addr,
				const char *anqp_req, u16 anqp_req_len);

VOID ReceiveGASInitReq(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);

VOID ReceiveGASCBReq(
	IN PRTMP_ADAPTER pAd, 
	IN MLME_QUEUE_ELEM *Elem);
#endif /* CONFIG_AP_SUPPORT */

#endif /* __GAS_H__ */
