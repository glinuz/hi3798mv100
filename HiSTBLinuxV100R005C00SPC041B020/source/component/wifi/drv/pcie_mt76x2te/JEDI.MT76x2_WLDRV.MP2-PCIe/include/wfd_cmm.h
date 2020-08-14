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


/*
	Module Name:
	wfd.h

	Revision History:
	Who			When			What
	--------		----------		----------------------------------------------

*/

#ifndef	__WFD_CMM_H__
#define	__WFD_CMM_H__

#ifdef WFD_SUPPORT

#include "rtmp_type.h"

#define WFD_OUI									0x506F9A

/* Subelement ID Definitions */
#define SUBID_WFD_DEVICE_INFO					0
#define SUBID_WFD_ASSOCIATED_BSSID			1
#define SUBID_WFD_AUDIO_FORMATS				2
#define SUBID_WFD_VIDEO_FORMATS				3
#define SUBID_WFD_3D_VIDEO_FORMATS			4
#define SUBID_WFD_CONTENT_PROTECTION		5
#define SUBID_WFD_COUPLED_SINK_INFO			6
#define SUBID_WFD_EXTENDED_CAP				7
#define SUBID_WFD_LOCAL_IP_ADDR				8
#define SUBID_WFD_SESSION_INFO				9
#define SUBID_WFD_ALTERNATE_MAC_ADDR		10
#define SUBID_WFD_END		11

/* Subelement ID Definitions */
#define SUBID_WFD_DEVICE_INFO_LEN				6
#define SUBID_WFD_ASSOCIATED_BSSID_LEN			6
#define SUBID_WFD_AUDIO_FORMATS_LEN				15
#define SUBID_WFD_VIDEO_FORMATS_LEN				21
#define SUBID_WFD_3D_VIDEO_FORMATS_LEN			13
#define SUBID_WFD_CONTENT_PROTECTION_LEN		1
#define SUBID_WFD_COUPLED_SINK_INFO_LEN			1
#define SUBID_WFD_EXTENDED_CAP_LEN				2
#define SUBID_WFD_LOCAL_IP_ADDR_LEN				5
#define SUBID_WFD_ALTERNATE_MAC_ADDR_LEN		6
#define WFD_SOURCE								0x0
#define WFD_PRIMARY_SINK						0x1
#define WFD_SECONDARY_SINK					0x2
#define WFD_SOURCE_PRIMARY_SINK				0x3
#define WFD_DEVICE_TYPE_END					0x4

#define WFD_COUPLED_NOT_SUPPORT				0x0
#define WFD_COUPLED_SUPPORT					0x1

#define WFD_SESSION_NOT_AVAILABLE			0x0
#define WFD_SESSION_AVAILABLE					0x1

#define WFD_WSD_NOT_SUPPORT					0x0
#define WFD_WSD_SUPPORT						0x1

#define WFD_PC_P2P								0x0
#define WFD_PC_TDLS							0x1

#define WFD_CP_NOT_SUPPORT					0x0
#define WFD_CP_HDCP20							0x1

#define WFD_TIME_SYNC_NOT_SUPPORT			0x0
#define WFD_TIME_SYNC_SUPPORT				0x1	/* Time Synchronization using 802.1AS */

#define WFD_TDLS_WEAK_SECURITY				0x0
#define WFD_TDLS_STRONG_SECURITY			0x1

/* Coupled Sink Status Bitmap */
#define WFD_AVAILABLE_FOR_COUPLING			0x0
#define WFD_COUPLED_SUCCESS					0x1
#define WFD_TEARDOWN_COUPLING				0x2

/* WFD RTSP Default Port */
#define WFD_RTSP_DEFAULT_PORT				7236

/* Version of Local IP Address Subelement */
#define WFD_LOCAL_IP_ADDR_VERSION_IPV4		1

/* Default max throughput */
#define WFD_MAX_THROUGHPUT_DEFAULT			150

/* Default content protection bit */
#define WFD_CONTENT_PROTECT_DEFAULT			WFD_CP_NOT_SUPPORT

typedef struct GNU_PACKED _WFD_DEVICE_INFO {
#ifndef RT_BIG_ENDIAN
	USHORT DeviceType:2;
	USHORT SourceCoupled:1;
	USHORT SinkCoupled:1;
	USHORT SessionAvail:2;
	USHORT WSD:1;
	USHORT PC:1;		/* Preferred Connectivity */
	USHORT CP:1;
	USHORT TimeSync:1;
	USHORT Rsvd:6;
#else
	USHORT Rsvd:6;
	USHORT TimeSync:1;	/* 802.1AS Support */
	USHORT CP:1;		/* Content Protection */
	USHORT PC:1;		/* Preferred Connectivity */
	USHORT WSD:1;		/* WFD Service Discovery */
	USHORT SessionAvail:2;
	USHORT SinkCoupled:1;
	USHORT SourceCoupled:1;
	USHORT DeviceType:2;
#endif				/* endif */
} WFD_DEVICE_INFO, *PWFD_DEVICE_INFO;

typedef struct GNU_PACKED _WFD_COUPLED_SINK_INFO {
#ifndef RT_BIG_ENDIAN
	UCHAR CoupledStat:2;
	UCHAR Rsvd:6;
#else
	UCHAR Rsvd:6;
	UCHAR CoupledStat:2;
#endif				/* endif */
} WFD_COUPLED_SINK_INFO, *PWFD_COUPLED_SINK_INFO;

typedef struct GNU_PACKED _WFD_SESSION_INFO {
#ifndef RT_BIG_ENDIAN
	UCHAR Length;
	UCHAR DeviceAddr[MAC_ADDR_LEN];
	UCHAR Bssid[MAC_ADDR_LEN];
	WFD_DEVICE_INFO WfdDevInfo;
	USHORT MaxThroughput;
	WFD_COUPLED_SINK_INFO CoupledSinkInfo;
	UCHAR CoupledPeerAddr[MAC_ADDR_LEN];
#else
	UCHAR CoupledPeerAddr[MAC_ADDR_LEN];
	WFD_COUPLED_SINK_INFO CoupledSinkInfo;
	USHORT MaxThroughput;
	WFD_DEVICE_INFO WfdDevInfo;
	UCHAR Bssid[MAC_ADDR_LEN];
	UCHAR DeviceAddr[MAC_ADDR_LEN];
	UCHAR Length;
#endif				/* RT_BIG_ENDIAN */
} WFD_SESSION_INFO, *PWFD_SESSION_INFO;

typedef struct _WFD_SERV_DISC_QUERY_INFO {
	BOOLEAN bWfd_device_info_ie;
	UCHAR wfd_device_info_ie[SUBID_WFD_DEVICE_INFO_LEN];
	BOOLEAN bWfd_associate_bssid_ie;
	UCHAR wfd_associate_bssid_ie[SUBID_WFD_ASSOCIATED_BSSID_LEN];
	BOOLEAN bWfd_audio_format_ie;
	UCHAR wfd_audio_format_ie[SUBID_WFD_AUDIO_FORMATS_LEN];
	BOOLEAN bWfd_video_format_ie;
	UCHAR wfd_video_format_ie[SUBID_WFD_VIDEO_FORMATS_LEN];
	BOOLEAN bWfd_3d_video_format_ie;
	UCHAR wfd_3d_video_format_ie[SUBID_WFD_3D_VIDEO_FORMATS_LEN];
	BOOLEAN bWfd_content_proctection;
	UCHAR wfd_content_proctection[SUBID_WFD_CONTENT_PROTECTION_LEN];
	BOOLEAN bWfd_couple_sink_info_ie;
	UCHAR wfd_couple_sink_info_ie[SUBID_WFD_COUPLED_SINK_INFO_LEN];
	BOOLEAN bWfd_extent_capability_ie;
	UCHAR wfd_extent_capability_ie[SUBID_WFD_EXTENDED_CAP_LEN];
	BOOLEAN bWfd_local_ip_ie;
	UCHAR wfd_local_ip_ie[SUBID_WFD_LOCAL_IP_ADDR_LEN];
	BOOLEAN bWfd_session_info_ie;
	UCHAR wfd_session_info_ie[120];
	BOOLEAN bWfd_alternate_mac_addr_ie;
	UCHAR wfd_alternate_mac_addr_ie[SUBID_WFD_ALTERNATE_MAC_ADDR_LEN];
} WFD_SERV_DISC_QUERY_INFO, *PWFD_SERV_DISC_QUERY_INFO;

/* Store for WFD Entry Configuration */
typedef struct _WFD_ENTRY_INFO {
	UCHAR bWfdClient;
	UCHAR wfd_devive_type;
	UCHAR source_coupled;
	UCHAR sink_coupled;
	UCHAR session_avail;
	UCHAR wfd_service_discovery;
	UCHAR wfd_PC;
	UCHAR wfd_CP;
	UCHAR wfd_time_sync;
	UCHAR sink_audio_unsupport;
	UCHAR source_audio_only;
	UCHAR tdls_persistent_group;
	USHORT rtsp_port;
	USHORT max_throughput;
	UCHAR assoc_addr[MAC_ADDR_LEN];
	WFD_COUPLED_SINK_INFO coupled_sink_status;
	UCHAR coupled_peer_addr[MAC_ADDR_LEN];
	/* Service Discovery */
	WFD_SERV_DISC_QUERY_INFO wfd_serv_disc_query_info;
} WFD_ENTRY_INFO, *PWFD_ENTRY_INFO;

/* Store for WFD Configuration */
typedef struct _RT_WFD_CONFIG {
	BOOLEAN bWfdEnable;
#ifdef RT_CFG80211_SUPPORT
	BOOLEAN bSuppInsertWfdIe;	/* Insert WFD IE to management frames from wpa_supplicant */
	BOOLEAN bSuppGoOn;	/* wpa_supplicant P2P GO is on */
#endif				/* RT_CFG80211_SUPPORT */
	UCHAR DeviceType;
	UCHAR SourceCoupled;
	UCHAR SinkCoupled;
	UCHAR SessionAvail;
	UCHAR WSD;
	UCHAR PC;
	UCHAR CP;		/* WFD Content Protection capability */
	UCHAR TimeSync;
	USHORT RtspPort;	/* Deafult WFD_RTSP_DEFAULT_PORT */
	USHORT MaxThroughput;	/* Maximum average throughput capability */
	UCHAR Bssid[MAC_ADDR_LEN];
	UCHAR IPv4Addr[4];
	UCHAR TdlsSecurity;
	UCHAR PeerSessionAvail;
	UCHAR PeerPC;
	WFD_COUPLED_SINK_INFO CoupledSinkStatus;
	/* Service Discovery */
	UINT32 WfdSerDiscCapable;
	WFD_SERV_DISC_QUERY_INFO wfd_serv_disc_query_info;
} RT_WFD_CONFIG, *PRT_WFD_CONFIG;

#endif /* WFD_SUPPORT */
#endif /* __WFD_H__ */
