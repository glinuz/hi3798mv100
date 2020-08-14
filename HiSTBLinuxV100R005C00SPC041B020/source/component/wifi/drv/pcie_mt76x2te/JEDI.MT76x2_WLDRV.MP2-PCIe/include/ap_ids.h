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


VOID RTMPIdsPeriodicExec(IN PVOID SystemSpecific1,
			 IN PVOID FunctionContext,
			 IN PVOID SystemSpecific2, IN PVOID SystemSpecific3);

BOOLEAN RTMPSpoofedMgmtDetection(IN RTMP_ADAPTER * pAd,
				 IN HEADER_802_11 * pHeader, IN RX_BLK * rxblk);

VOID RTMPConflictSsidDetection(IN PRTMP_ADAPTER pAd,
			       IN PUCHAR pSsid,
			       IN UCHAR SsidLen, IN CHAR Rssi0, IN CHAR Rssi1, IN CHAR Rssi2);

BOOLEAN RTMPReplayAttackDetection(IN RTMP_ADAPTER * pAd, IN UCHAR * pAddr2, IN RX_BLK * rxblk);

VOID RTMPUpdateStaMgmtCounter(IN PRTMP_ADAPTER pAd, IN USHORT type);

VOID RTMPClearAllIdsCounter(IN PRTMP_ADAPTER pAd);

VOID RTMPIdsStart(IN PRTMP_ADAPTER pAd);

VOID RTMPIdsStop(IN PRTMP_ADAPTER pAd);

VOID rtmp_read_ids_from_file(IN PRTMP_ADAPTER pAd, char *tmpbuf, char *buffer);
