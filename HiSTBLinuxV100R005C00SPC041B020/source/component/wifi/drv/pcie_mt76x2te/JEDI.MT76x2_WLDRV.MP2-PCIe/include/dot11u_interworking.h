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


#ifndef	__DOT11U_HOTSPOT_H__
#define	__DOT11U_HOTSPOT_H__

#include "rtmp_type.h"

/* unit ms */
#define dot11GASResponseTimeout	5120
#define dotGASResponseBufferingTime 5120


enum GAS_ACTION_FIELD {
	ACTION_GAS_INIT_REQ = 10,
	ACTION_GAS_INIT_RSP = 11,
	ACTION_GAS_CB_REQ = 12,
	ACTION_GAS_CB_RSP = 13,
};

typedef struct GNU_PACKED _GAS_FRAME
{
	HEADER_802_11 Hdr;
	UCHAR Category;
	union {
		struct {
			UCHAR Action;
			UCHAR DialogToken;
			/* 
 			 * Following are advertisement protocol element, 
 			 * query request length, and query request
 			 */
			UCHAR Variable[0];
		} GNU_PACKED GAS_INIT_REQ;
		struct {
			UCHAR Action;
			UCHAR DialogToken;
			UINT16 StatusCode;
			UINT16 GASComebackDelay;
			/*
 			 *  Following are advertisement protocol element,
 			 *  query response length, and query response(optional)
 			 */
			UCHAR Variable[0];
		} GNU_PACKED GAS_INIT_RSP;
		struct {
			UCHAR Action;
			UCHAR DialogToken;
		} GNU_PACKED GAS_CB_REQ;
		struct {
			UCHAR Action;
			UCHAR DialogToken;
			UINT16 StatusCode;
			UCHAR GASRspFragID;
			UINT16 GASComebackDelay;
			/*
 			 * Following are advertisment protocol element,
 			 * query response length, and query response(optional)
 			 */
			UCHAR Variable[0]; 
		} GNU_PACKED GAS_CB_RSP;
	}u;
}GAS_FRAME, *PGAS_FRAME;


/* Status Code */
enum DOT11U_STATUS_CODE {
	ADVERTISEMENT_PROTOCOL_NOT_SUPPORTED = 59,	
	UNSPECIFIED_FAILURE = 60,
	RESPONSE_NOT_RECEIVED_FROM_SERVER = 61,
	TIMEOUT = 62,
	QUERY_RESPONSE_TOO_LARGE = 63,
	REQUEST_REFUSED_HOME_NETWORK_NOT_SUPPORT = 64,
	SERVER_UNREACHABLE = 65,
	REQUEST_REFUSED_PERMISSIONS_RECEIVED_FROM_SSPN = 67,
	REQUEST_REFUSED_AP_NOT_SUPPORT_UNAUTH_ACCESS = 68,
	TRANSMISSION_FAILURE = 79,	
};

#endif /* __DOT11U_HOTSPOT_H__ */

