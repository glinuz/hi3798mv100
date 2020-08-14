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


#ifndef __DOT11V_WNM_H__
#define __DOT11V_WNM_H__

enum WNM_ACTION_FIELD {
	EVENT_REQUEST,
	EVENT_REPORT,
	DIAGNOSTIC_REQ,
	DIAGNOSTIC_REPORT,
	LOCATION_CONF_REQ,
	LOCATION_CONF_RSP,
	BSS_TRANSITION_QUERY,
	BSS_TRANSITION_REQ,
	BSS_TRANSITION_RSP,
	FMS_REQ,
	FMS_RSP,
	COLLOCATED_INTERFERENCE_REQ,
	COLLOCATED_INTERFERENCE_REPORT,
	TFS_REQ,
	TFS_RSP,
	TFS_NOTIFY,
	WNM_SLEEP_MODE_REQ,
	WNM_SLEEP_MODE_RSP,
	TIM_BROADCAST_REQ,
	TIM_BROADCAST_RSP,
	QOS_TRAFFIC_CAPABILITY_UPDATE,
	CHANNEL_USAGE_REQ,
	CHANNEL_USAGE_RSP,
	DMS_REQ,
	DMS_RSP,
	TIMING_MEASUREMENT_REQ,
	WNM_NOTIFICATION_REQ,
	WNM_NOTIFICATION_RSP,
};

typedef struct GNU_PACKED _WNM_FRAME {
	HEADER_802_11 Hdr;
	UCHAR Category;
	union {
		struct {
			UCHAR Action;
			UCHAR DialogToken;
			UCHAR Variable[0];
		} GNU_PACKED BTM_QUERY;

		struct {
			UCHAR Action;
			UCHAR DialogToken;
			UCHAR Variable[0];
		} GNU_PACKED BTM_REQ;

		struct {
			UCHAR Action;
			UCHAR DialogToken;
			UCHAR Variable[0];
		} GNU_PACKED BTM_RSP;
	} u;
} WNM_FRAME, *PWNM_FRAME;

void PeerWNMAction(IN PRTMP_ADAPTER pAd, IN MLME_QUEUE_ELEM * Elem);

#endif /* __DOT11V_WNM_H__ */
