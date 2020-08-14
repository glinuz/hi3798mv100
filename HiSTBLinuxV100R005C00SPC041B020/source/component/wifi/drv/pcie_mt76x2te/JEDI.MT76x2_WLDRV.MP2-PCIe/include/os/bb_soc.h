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


#ifndef	__BBU_SOC_H__
#define	__BBU_SOC_H__

#define POLLING_MODE_DETECT_INTV  300
extern void os_TCIfQuery(unsigned short query_id, void *result, void *result2);


typedef struct _BBUPollingMode {
	RALINK_TIMER_STRUCT			PollingModeDetect;
	BOOLEAN						PollingModeDetectRunning;
	struct work_struct			PollingDataBH;
	UINT32						pAd_va;
	NDIS_SPIN_LOCK				PollingModeLock;
} BBUPollingMode, *PBBUPollingMode;

extern void tc3162_enable_irq(unsigned int irq);
extern void tc3162_disable_irq(unsigned int irq);
extern VOID PeriodicPollingModeDetect(
	IN PVOID SystemSpecific1,
	IN PVOID FunctionContext,
	IN PVOID SystemSpecific2,
	IN PVOID SystemSpecific3);
DECLARE_TIMER_FUNCTION(PeriodicPollingModeDetect);

extern VOID PollingModeIsr(struct work_struct *work);
/*
    ========================================================================

    Routine Description:
       Trendchip DMT Trainning status detect

    Arguments:
	data		Point to RTMP_ADAPTER

    Return Value:
	NONE

    Note:

    ========================================================================
*/
extern void pcieReset(void);
extern int pcieRegInitConfig(void);
extern void BBU_PCIE_Init(void);
extern VOID BBUPollingModeClose(IN RTMP_ADAPTER * pAd);
extern VOID BBUPollingModeInit(IN RTMP_ADAPTER * pAd);
extern VOID BBUPollingModeStart(IN RTMP_ADAPTER * pAd);
extern VOID BBUPrepareMAC(IN RTMP_ADAPTER * pAd, PUCHAR macaddr);
#endif /* endif */

