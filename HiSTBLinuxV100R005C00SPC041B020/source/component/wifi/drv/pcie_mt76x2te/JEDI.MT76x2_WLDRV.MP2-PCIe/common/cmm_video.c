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


#include "rt_config.h"


#ifdef VIDEO_TURBINE_SUPPORT



BOOLEAN UpdateFromGlobal = FALSE;

void VideoTurbineUpdate(
	IN PRTMP_ADAPTER pAd)
{
	if (UpdateFromGlobal == TRUE)
	{
		pAd->VideoTurbine.Enable = GLOBAL_AP_VIDEO_CONFIG.Enable;
		pAd->VideoTurbine.ClassifierEnable = GLOBAL_AP_VIDEO_CONFIG.ClassifierEnable;
		pAd->VideoTurbine.HighTxMode = GLOBAL_AP_VIDEO_CONFIG.HighTxMode;
		pAd->VideoTurbine.TxPwr = GLOBAL_AP_VIDEO_CONFIG.TxPwr;
		pAd->VideoTurbine.VideoMCSEnable = GLOBAL_AP_VIDEO_CONFIG.VideoMCSEnable;
		pAd->VideoTurbine.VideoMCS = GLOBAL_AP_VIDEO_CONFIG.VideoMCS;
		pAd->VideoTurbine.TxBASize = GLOBAL_AP_VIDEO_CONFIG.TxBASize;
		pAd->VideoTurbine.TxLifeTimeMode = GLOBAL_AP_VIDEO_CONFIG.TxLifeTimeMode;
		pAd->VideoTurbine.TxLifeTime = GLOBAL_AP_VIDEO_CONFIG.TxLifeTime;
		pAd->VideoTurbine.TxRetryLimit = GLOBAL_AP_VIDEO_CONFIG.TxRetryLimit;
	}
}


VOID TxSwQDepthAdjust(IN RTMP_ADAPTER *pAd, IN UINT32 qLen)
{
	ULONG IrqFlags;
	INT qIdx;
	QUEUE_HEADER *pTxQ, *pEntry;
	PNDIS_PACKET pPacket;

	RTMP_IRQ_LOCK(&pAd->irq_lock, IrqFlags);
	pAd->TxSwQMaxLen = qLen;
	for (qIdx = 0; qIdx < NUM_OF_TX_RING; qIdx++)
	{
		pTxQ = &pAd->TxSwQueue[qIdx];
		while(pTxQ->Number >= pAd->TxSwQMaxLen)
		{
			pEntry = RemoveHeadQueue(pTxQ);
			if (pEntry)
			{
				pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
				RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
			}
			else
				break;
		}
	}
	RTMP_IRQ_UNLOCK(&pAd->irq_lock, IrqFlags);

	DBGPRINT(RT_DEBUG_OFF, ("%s():Set TxSwQMaxLen as %d\n",
			__FUNCTION__, pAd->TxSwQMaxLen));
}


VOID VideoTurbineDynamicTune(
	IN PRTMP_ADAPTER pAd)
{
	if (pAd->VideoTurbine.Enable == TRUE)
	{
			UINT32 MacReg = 0;

		{
			/* Tx retry limit = 2F,1F */
			RTMP_IO_READ32(pAd, TX_RTY_CFG, &MacReg);
			MacReg &= 0xFFFF0000;
			MacReg |= GetAsicVideoRetry(pAd);
			RTMP_IO_WRITE32(pAd, TX_RTY_CFG, MacReg);
		}

		pAd->VideoTurbine.TxBASize = GetAsicVideoTxBA(pAd);

		Set_RateAdaptInterval(pAd, "100:50");
		TxSwQDepthAdjust(pAd, 1024);

	}
	else
	{
			UINT32 MacReg = 0;


		/* Default Tx retry limit = 1F,0F */
		RTMP_IO_READ32(pAd, TX_RTY_CFG, &MacReg);
		MacReg &= 0xFFFF0000;
			MacReg |= GetAsicDefaultRetry(pAd);
		RTMP_IO_WRITE32(pAd, TX_RTY_CFG, MacReg);

		pAd->VideoTurbine.TxBASize = GetAsicDefaultTxBA(pAd);

		/* reset to default rate adaptation simping interval */
		if ((pAd->ra_interval != DEF_RA_TIME_INTRVAL) ||
			(pAd->ra_fast_interval != DEF_QUICK_RA_TIME_INTERVAL))
			Set_RateAdaptInterval(pAd, "500:100");

		TxSwQDepthAdjust(pAd, MAX_PACKETS_IN_QUEUE);
	}
}

UINT32 GetAsicDefaultRetry(
	IN PRTMP_ADAPTER pAd)
{
	UINT32 RetryLimit;

	RetryLimit = 0x1F0F;

	return RetryLimit;
}

UCHAR GetAsicDefaultTxBA(
	IN PRTMP_ADAPTER pAd)
{
        return pAd->CommonCfg.TxBASize;
}

UINT32 GetAsicVideoRetry(
	IN PRTMP_ADAPTER pAd)
{
	return pAd->VideoTurbine.TxRetryLimit;
}

UCHAR GetAsicVideoTxBA(
	IN PRTMP_ADAPTER pAd)
{
	return pAd->VideoTurbine.TxBASize;
}

VOID VideoConfigInit(
	IN PRTMP_ADAPTER pAd)
{
	pAd->VideoTurbine.Enable = FALSE;
	pAd->VideoTurbine.TxRetryLimit = 0x2F1F;
	pAd->VideoTurbine.TxBASize = pAd->CommonCfg.TxBASize;
}

#endif /* VIDEO_TURBINE_SUPPORT */


