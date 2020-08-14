/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved.	Ralink's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of Ralink Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

	Module Name:
	rtusb_bulk.c

	Abstract:

	Revision History:
	Who			When		What
	--------	----------	----------------------------------------------
	Name		Date		Modification logs

*/

#include "rt_config.h"

/*
========================================================================
Routine Description:
    Create kernel threads & tasklets.

Arguments:
    *net_dev			Pointer to wireless net device interface

Return Value:
	NDIS_STATUS_SUCCESS
	NDIS_STATUS_FAILURE

Note:
========================================================================
*/
NDIS_STATUS RtmpMgmtTaskInit(IN RTMP_ADAPTER * pAd)
{
	RTMP_OS_TASK *pTask;
	NDIS_STATUS status;

	/*
	   Creat TimerQ Thread, We need init timerQ related structure before create the timer thread.
	 */
	RtmpTimerQInit(pAd);

	pTask = &pAd->timerTask;
	RTMP_OS_TASK_INIT(pTask, "RtmpTimerTask", pAd);
	status = RtmpOSTaskAttach(pTask, RtmpTimerQThread, (ULONG) pTask);
	if (status == NDIS_STATUS_FAILURE) {
		printk(KERN_WARNING "%s: unable to start RtmpTimerQThread\n",
		       RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev));
		return NDIS_STATUS_FAILURE;
	}

	/* Creat Command Thread */
	pTask = &pAd->cmdQTask;
	RTMP_OS_TASK_INIT(pTask, "RtmpCmdQTask", pAd);
	status = RtmpOSTaskAttach(pTask, RTUSBCmdThread, (ULONG) pTask);
	if (status == NDIS_STATUS_FAILURE) {
		printk(KERN_WARNING "%s: unable to start RTUSBCmdThread\n",
		       RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev));
		return NDIS_STATUS_FAILURE;
	}
#ifdef WSC_INCLUDED
	/* start the crediential write task first. */
	WscThreadInit(pAd);
#endif /* WSC_INCLUDED */

#ifdef RXPKT_THREAD
	pTask = &pAd->rxPktTask;
	RTMP_OS_TASK_INIT(pTask, "RtmpRxPktTask", pAd);
	status = RtmpOSTaskAttach(pTask, RTRxPktThread, (ULONG) pTask);
	if (status == NDIS_STATUS_FAILURE) {
		pr_warn("%s: unable to start RTRxPktThread\n",
		       RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev));
		return NDIS_STATUS_FAILURE;
	}
	DBGPRINT(RT_DEBUG_ERROR, ("%s:RtmpRxPktTask Create\n", __func__));
#endif /* RXPKT_THREAD */
	return NDIS_STATUS_SUCCESS;
}

/*
========================================================================
Routine Description:
    Close kernel threads.

Arguments:
	*pAd				the raxx interface data pointer

Return Value:
    NONE

Note:
========================================================================
*/
VOID RtmpMgmtTaskExit(IN RTMP_ADAPTER * pAd)
{
	INT ret;
	RTMP_OS_TASK *pTask;
#ifdef RXPKT_THREAD
	unsigned long IrqFlags;
#endif /* RXPKT_THREAD */

	/* Sleep 50 milliseconds so pending io might finish normally */
	RtmpusecDelay(50000);

	/* We want to wait until all pending receives and sends to the */
	/* device object. We cancel any */
	/* irps. Wait until sends and receives have stopped. */
	RTUSBCancelPendingIRPs(pAd);

	/* We need clear timerQ related structure before exits of the timer thread. */
	RtmpTimerQExit(pAd);

	/* Terminate cmdQ thread */
	pTask = &pAd->cmdQTask;
	RTMP_OS_TASK_LEGALITY(pTask) {
		NdisAcquireSpinLock(&pAd->CmdQLock);
		pAd->CmdQ.CmdQState = RTMP_TASK_STAT_STOPED;
		NdisReleaseSpinLock(&pAd->CmdQLock);

		/*RTUSBCMDUp(&pAd->cmdQTask); */
		ret = RtmpOSTaskKill(pTask);
		if (ret == NDIS_STATUS_FAILURE) {
			/*DBGPRINT(RT_DEBUG_ERROR, ("%s: kill task(%s) failed!\n", */
			/*RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev), pTask->taskName)); */
			DBGPRINT(RT_DEBUG_ERROR, ("kill command task failed!\n"));
		}
		pAd->CmdQ.CmdQState = RTMP_TASK_STAT_UNKNOWN;
	}

	/* Terminate timer thread */
	pTask = &pAd->timerTask;
	ret = RtmpOSTaskKill(pTask);
	if (ret == NDIS_STATUS_FAILURE) {
		/*DBGPRINT(RT_DEBUG_ERROR, ("%s: kill task(%s) failed!\n", */
		/*RTMP_OS_NETDEV_GET_DEVNAME(pAd->net_dev), pTask->taskName)); */
		DBGPRINT(RT_DEBUG_ERROR, ("kill timer task failed!\n"));
	}
#ifdef WSC_INCLUDED
	WscThreadExit(pAd);
#endif /* WSC_INCLUDED */
#ifdef RXPKT_THREAD
	/* Terminate rxPkt thread */
	pTask = &pAd->rxPktTask;
	RTMP_OS_TASK_LEGALITY(pTask) {
		RTMP_IRQ_LOCK(&pAd->rxPktQLock, IrqFlags);
		RtmpCleanupRxPktQueue(pAd, &pAd->rxPktQ);
		RTMP_IRQ_UNLOCK(&pAd->rxPktQLock, IrqFlags);

		ret = RtmpOSTaskKill(pTask);
		if (ret == NDIS_STATUS_FAILURE)
			DBGPRINT(RT_DEBUG_ERROR, ("kill command task failed!\n"));
	}
#endif /* RXPKT_THREAD */

}

static void rtusb_dataout_complete(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	purbb_t pUrb;
	POS_COOKIE pObj;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId;
	NTSTATUS Status;
	unsigned long IrqFlags;
#ifdef USB_BULK_BUF_ALIGMENT
	unsigned long IrqFlags2 = 0;
	INT idx = 0;
#endif /* USB_BULK_BUF_ALIGMENT */


	pUrb = (purbb_t) data;
	/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	Status = RTMP_USB_URB_STATUS_GET(pUrb);
	pAd = pHTTXContext->pAd;
	pObj = (POS_COOKIE) pAd->OS_Cookie;
	/*	Status = pUrb->status; */

	/* Store BulkOut PipeId */
	BulkOutPipeId = pHTTXContext->BulkOutPipeId;
	pAd->BulkOutDataOneSecCount++;

	/*DBGPRINT(RT_DEBUG_LOUD,
	("Done-B(%d):I=0x%lx, CWPos=%ld, NBPos=%ld, ENBPos=%ld, bCopy=%d!\n",
	BulkOutPipeId, in_interrupt(), pHTTXContext->CurWritePosition, */
	/*pHTTXContext->NextBulkOutPosition, pHTTXContext->ENextBulkOutPosition,
	pHTTXContext->bCopySavePad)); */

	RTMP_IRQ_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
	pAd->BulkOutPending[BulkOutPipeId] = FALSE;
	pHTTXContext->IRPPending = FALSE;
	pAd->watchDogTxPendingCnt[BulkOutPipeId] = 0;

	if (Status == USB_ST_NOERROR) {
		pAd->BulkOutComplete++;

		RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

		pAd->Counters8023.GoodTransmits++;
		/*RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags); */
		FREE_HTTX_RING(pAd, BulkOutPipeId, pHTTXContext);
		/*RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags); */

#ifdef UAPSD_SUPPORT
#if defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT)
#ifdef USB_BULK_BUF_ALIGMENT
		UAPSD_UnTagFrame(pAd, BulkOutPipeId, pHTTXContext->CurtBulkIdx,
				 pHTTXContext->NextBulkIdx);
#else
		UAPSD_UnTagFrame(pAd, BulkOutPipeId, pHTTXContext->NextBulkOutPosition,
				 pHTTXContext->ENextBulkOutPosition);
#endif /* USB_BULK_BUF_ALIGMENT */

#else
#ifdef CONFIG_AP_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
		if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
#else
		IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
#endif /* RT_CFG80211_P2P_SUPPORT */
		{
#ifdef USB_BULK_BUF_ALIGMENT
			UAPSD_UnTagFrame(pAd, BulkOutPipeId, pHTTXContext->CurtBulkIdx,
					 pHTTXContext->NextBulkIdx);
#else
			UAPSD_UnTagFrame(pAd, BulkOutPipeId, pHTTXContext->NextBulkOutPosition,
					 pHTTXContext->ENextBulkOutPosition);
#endif /* USB_BULK_BUF_ALIGMENT */

		}
#endif /* CONFIG_AP_SUPPORT */
#endif /* defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT) */
#endif /* UAPSD_SUPPORT */

#ifdef USB_BULK_BUF_ALIGMENT
		RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags2);
		CUR_WRITE_IDX_INC(pHTTXContext->CurtBulkIdx, BUF_ALIGMENT_RINGSIZE);
		RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags2);
#endif /* USB_BULK_BUF_ALIGMENT */

	} else {		/* STATUS_OTHER */

		PUCHAR pBuf;

		pAd->BulkOutCompleteOther++;

#ifdef USB_BULK_BUF_ALIGMENT
		idx = pHTTXContext->CurtBulkIdx;
		pBuf =
		    &pHTTXContext->TransferBuffer[idx]->field.
		    WirelessPacket[pHTTXContext->NextBulkOutPosition];
#else
		pBuf =
		    &pHTTXContext->TransferBuffer->field.
		    WirelessPacket[pHTTXContext->NextBulkOutPosition];
#endif /* USB_BULK_BUF_ALIGMENT */

		if (!RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
					  fRTMP_ADAPTER_HALT_IN_PROGRESS |
					  fRTMP_ADAPTER_NIC_NOT_EXIST |
					  fRTMP_ADAPTER_BULKOUT_RESET))) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);
			pAd->bulkResetPipeid = BulkOutPipeId;
			pAd->bulkResetReq[BulkOutPipeId] = pAd->BulkOutReq;
		}
		RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

		DBGPRINT_RAW(RT_DEBUG_ERROR,
			     ("BulkOutDataPacket failed: ReasonCode=%d!\n", Status));
		DBGPRINT_RAW(RT_DEBUG_ERROR,
			     ("\t>>BulkOut Req=0x%lx, Complete=0x%lx, Other=0x%lx\n",
			      pAd->BulkOutReq, pAd->BulkOutComplete, pAd->BulkOutCompleteOther));
		DBGPRINT_RAW(RT_DEBUG_ERROR,
			     ("\t>>BulkOut Header:%x %x %x %x %x %x %x %x\n", pBuf[0], pBuf[1],
			      pBuf[2], pBuf[3], pBuf[4], pBuf[5], pBuf[6], pBuf[7]));
		/*DBGPRINT_RAW(RT_DEBUG_ERROR, (">>BulkOutCompleteCancel=0x%x,
		BulkOutCompleteOther=0x%x\n", pAd->BulkOutCompleteCancel,
		pAd->BulkOutCompleteOther)); */

	}

	/* */
	/* bInUse = TRUE, means some process are filling TX data, after that must turn on bWaitingBulkOut */
	/* bWaitingBulkOut = TRUE, means the TX data are waiting for bulk out. */
	/* */
	/*RTMP_IRQ_LOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags); */
	if (((pHTTXContext->ENextBulkOutPosition != pHTTXContext->CurWritePosition) &&
	     (pHTTXContext->ENextBulkOutPosition != (pHTTXContext->CurWritePosition + 8)) &&
	     !RTUSB_TEST_BULK_FLAG(pAd, (fRTUSB_BULK_OUT_DATA_FRAG << BulkOutPipeId)))
#ifdef USB_BULK_BUF_ALIGMENT
	    || (pHTTXContext->NextBulkIdx != pHTTXContext->CurWriteIdx)
#endif /* USB_BULK_BUF_ALIGMENT */
	    ) {
		/* Indicate There is data avaliable */
		RTUSB_SET_BULK_FLAG(pAd, (fRTUSB_BULK_OUT_DATA_NORMAL << BulkOutPipeId));
	}
	/*RTMP_IRQ_UNLOCK(&pAd->TxContextQueueLock[BulkOutPipeId], IrqFlags); */

	/* Always call Bulk routine, even reset bulk. */
	/* The protection of rest bulk should be in BulkOut routine */
	RTUSBKickBulkOut(pAd);
}

static void rtusb_null_frame_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PTX_CONTEXT pNullContext;
	purbb_t pUrb;
	NTSTATUS Status;
	unsigned long irqFlag;


	pUrb = (purbb_t) data;
/*	pNullContext	= (PTX_CONTEXT)pUrb->context; */
	pNullContext = (PTX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	Status = RTMP_USB_URB_STATUS_GET(pUrb);
	pAd = pNullContext->pAd;
/*	Status			= pUrb->status; */

	/* Reset Null frame context flags */
	RTMP_IRQ_LOCK(&pAd->BulkOutLock[MGMTPIPEIDX], irqFlag);
	pNullContext->IRPPending = FALSE;
	pNullContext->InUse = FALSE;
	pAd->BulkOutPending[MGMTPIPEIDX] = FALSE;
	pAd->watchDogTxPendingCnt[MGMTPIPEIDX] = 0;

	if (Status == USB_ST_NOERROR) {
		RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[MGMTPIPEIDX], irqFlag);

		RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);
	} else {		/* STATUS_OTHER */

		if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET))) {
			DBGPRINT_RAW(RT_DEBUG_ERROR,
				     ("Bulk Out Null Frame Failed, ReasonCode=%d!\n", Status));
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);
			pAd->bulkResetPipeid = (MGMTPIPEIDX | BULKOUT_MGMT_RESET_FLAG);
			RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[MGMTPIPEIDX], irqFlag);
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[MGMTPIPEIDX], irqFlag);
		}
	}

	/* Always call Bulk routine, even reset bulk. */
	/* The protectioon of rest bulk should be in BulkOut routine */
	RTUSBKickBulkOut(pAd);
}

static void rtusb_pspoll_frame_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PTX_CONTEXT pPsPollContext;
	purbb_t pUrb;
	NTSTATUS Status;


	pUrb = (purbb_t) data;
/*	pPsPollContext	= (PTX_CONTEXT)pUrb->context; */
	pPsPollContext = (PTX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	Status = RTMP_USB_URB_STATUS_GET(pUrb);
	pAd = pPsPollContext->pAd;
/*	Status			= pUrb->status; */

	/* Reset PsPoll context flags */
	pPsPollContext->IRPPending = FALSE;
	pPsPollContext->InUse = FALSE;
	pAd->watchDogTxPendingCnt[0] = 0;

	if (Status == USB_ST_NOERROR) {
		RTMPDeQueuePacket(pAd, FALSE, NUM_OF_TX_RING, MAX_TX_PROCESS);
	} else {		/* STATUS_OTHER */

		if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET))) {
			DBGPRINT_RAW(RT_DEBUG_ERROR, ("Bulk Out PSPoll Failed\n"));
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);
			pAd->bulkResetPipeid = (MGMTPIPEIDX | BULKOUT_MGMT_RESET_FLAG);
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		}
	}

	RTMP_SEM_LOCK(&pAd->BulkOutLock[0]);
	pAd->BulkOutPending[0] = FALSE;
	RTMP_SEM_UNLOCK(&pAd->BulkOutLock[0]);

	/* Always call Bulk routine, even reset bulk. */
	/* The protectioon of rest bulk should be in BulkOut routine */
	RTUSBKickBulkOut(pAd);

}

#ifdef MULTI_URB
#define fRTMP_ADAPTER_NEED_STOP_RX		\
		(fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_HALT_IN_PROGRESS |	\
		 fRTMP_ADAPTER_RADIO_OFF | fRTMP_ADAPTER_RESET_IN_PROGRESS | \
		 fRTMP_ADAPTER_REMOVE_IN_PROGRESS | fRTMP_ADAPTER_BULKIN_RESET)

#define fRTMP_ADAPTER_NEED_STOP_HANDLE_RX	\
		(fRTMP_ADAPTER_NIC_NOT_EXIST | fRTMP_ADAPTER_HALT_IN_PROGRESS |	\
		 fRTMP_ADAPTER_RADIO_OFF | fRTMP_ADAPTER_RESET_IN_PROGRESS | \
		 fRTMP_ADAPTER_REMOVE_IN_PROGRESS)
#endif /* endif */
/*
========================================================================
Routine Description:
    Handle received packets.

Arguments:
	data				- URB information pointer

Return Value:
    None

Note:
========================================================================
*/
static void rx_done_tasklet(unsigned long data)
{
	purbb_t pUrb;
	PRX_CONTEXT pRxContext;
	PRTMP_ADAPTER pAd;
	NTSTATUS Status;
#ifndef MULTI_URB
	unsigned long IrqFlags;
#endif /* endif */

	pUrb = (purbb_t) data;
	pRxContext = (PRX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	Status = RTMP_USB_URB_STATUS_GET(pUrb);
	pAd = pRxContext->pAd;

#ifdef MULTI_URB
	if (!((RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NEED_STOP_RX)
	       && (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_POLL_IDLE))))) {
		INT32 idx;
#ifdef CONFIG_STA_SUPPORT
		if (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_IDLE_RADIO_OFF))
#endif /* CONFIG_STA_SUPPORT */
			for (idx = pAd->PendingRx; idx < SUBMIT_URB_NUM; idx++)
				DoBulkIn(pAd);
	}

	if (Status != USB_ST_NOERROR && (Status != -EINPROGRESS)) {
		if ((!RTMP_TEST_FLAG(pAd,
						(fRTMP_ADAPTER_RESET_IN_PROGRESS |
						fRTMP_ADAPTER_BULKIN_RESET |
						fRTMP_ADAPTER_HALT_IN_PROGRESS |
						fRTMP_ADAPTER_RADIO_OFF |
						fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
			DBGPRINT_RAW(RT_DEBUG_ERROR,
				("%s -> Status=%d, BIIdx=%d, BIRIdx=%d, actual_length=%u\n",
				__func__, Status,
				pAd->NextRxBulkInIndex,
				pAd->NextRxBulkInReadIndex,
				RTMP_USB_URB_LEN_GET(pUrb)));

			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKIN_RESET);
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_IN, NULL, 0);
		}
	}
#endif /* endif */

#ifndef MULTI_URB
	RTMP_IRQ_LOCK(&pAd->BulkInLock, IrqFlags);
	pRxContext->InUse = FALSE;
	pRxContext->IRPPending = FALSE;
	pRxContext->BulkInOffset += RTMP_USB_URB_LEN_GET(pUrb);	/*pUrb->actual_length; */
	/*NdisInterlockedDecrement(&pAd->PendingRx); */

	if (pAd->PendingRx > 0)
		pAd->PendingRx--;

	if (Status == USB_ST_NOERROR) {
		pAd->BulkInComplete++;
		pAd->NextRxBulkInPosition = 0;
		if (pRxContext->BulkInOffset) {	/* As jan's comment, it may bulk-in success but size is zero. */
			pRxContext->Readable = TRUE;
			INC_RING_INDEX(pAd->NextRxBulkInIndex, RX_RING_SIZE);
		}
		RTMP_IRQ_UNLOCK(&pAd->BulkInLock, IrqFlags);
	} else {		/* STATUS_OTHER */

		pAd->BulkInCompleteFail++;
		/* Still read this packet although it may comtain wrong bytes. */
		pRxContext->Readable = FALSE;
		RTMP_IRQ_UNLOCK(&pAd->BulkInLock, IrqFlags);

		/* Parsing all packets. because after reset, the index will reset to all zero. */
		if ((!RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
					   fRTMP_ADAPTER_BULKIN_RESET |
					   fRTMP_ADAPTER_HALT_IN_PROGRESS |
					   fRTMP_ADAPTER_RADIO_OFF |
					   fRTMP_ADAPTER_NIC_NOT_EXIST)))&&
(!RTMP_TEST_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING))) {

			DBGPRINT_RAW(RT_DEBUG_WARN,
				     ("Bulk In Failed. Status=%d, BIIdx=0x%x, BIRIdx=0x%x, actual_length= 0x%x\n",
				      Status, pAd->NextRxBulkInIndex, pAd->NextRxBulkInReadIndex,
				      RTMP_USB_URB_LEN_GET(pRxContext->pUrb)));
			/*->actual_length)); */

			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKIN_RESET);
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_IN, NULL, 0);
		}
	}

	ASSERT((pRxContext->InUse == pRxContext->IRPPending));
#endif /* endif */
#ifdef RALINK_ATE
	if (ATE_ON(pAd)) {
		/* If the driver is in ATE mode and Rx frame is set into here. */
		if (pAd->ContinBulkIn == TRUE)
			RTUSBBulkReceive(pAd);
	} else
#endif /* RALINK_ATE */
		RTUSBBulkReceive(pAd);


	return;

}


static void rtusb_mgmt_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PTX_CONTEXT pMLMEContext;
	int index;
	PNDIS_PACKET pPacket;
	purbb_t pUrb;
	NTSTATUS Status;
	unsigned long IrqFlags;


	pUrb = (purbb_t) data;
/*	pMLMEContext	= (PTX_CONTEXT)pUrb->context; */
	pMLMEContext = (PTX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	Status = RTMP_USB_URB_STATUS_GET(pUrb);
	pAd = pMLMEContext->pAd;
/*	Status			= pUrb->status; */
	index = pMLMEContext->SelfIdx;

	ASSERT((pAd->MgmtRing.TxDmaIdx == index));

	RTMP_IRQ_LOCK(&pAd->BulkOutLock[MGMTPIPEIDX], IrqFlags);

#ifdef UAPSD_SUPPORT
	/* Qos Null frame with EOSP shall have valid Wcid value. reference RtmpUSBMgmtKickOut() API. */
	/* otherwise will be value of MCAST_WCID. */
#if defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT)
	if ((pMLMEContext->Wcid != MCAST_WCID) && (pMLMEContext->Wcid < MAX_LEN_OF_MAC_TABLE)) {
		MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[pMLMEContext->Wcid];

		UAPSD_SP_Close(pAd, pEntry);
		pMLMEContext->Wcid = MCAST_WCID;
	}
#else
#ifdef CONFIG_AP_SUPPORT
#ifdef RT_CFG80211_P2P_SUPPORT
	if (RTMP_CFG80211_VIF_P2P_GO_ON(pAd))
#else
	IF_DEV_CONFIG_OPMODE_ON_AP(pAd)
#endif /* RT_CFG80211_P2P_SUPPORT */
	{
		/* Qos Null frame with EOSP shall have valid Wcid value. reference RtmpUSBMgmtKickOut() API. */
		/* otherwise will be value of MCAST_WCID. */
		if ((pMLMEContext->Wcid != MCAST_WCID)
		    && (pMLMEContext->Wcid < MAX_LEN_OF_MAC_TABLE)) {
			MAC_TABLE_ENTRY *pEntry = &pAd->MacTab.Content[pMLMEContext->Wcid];

			UAPSD_SP_Close(pAd, pEntry);
			pMLMEContext->Wcid = MCAST_WCID;
		}
	}
#endif /* CONFIG_AP_SUPPORT */
#endif /* defined(DOT11Z_TDLS_SUPPORT) || defined(CFG_TDLS_SUPPORT) */
#endif /* UAPSD_SUPPORT */

	if (Status != USB_ST_NOERROR) {
		/*Bulk-Out fail status handle */
		if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET))) {
			DBGPRINT_RAW(RT_DEBUG_ERROR,
				     ("Bulk Out MLME Failed, Status=%d!\n", Status));
			/* TODO: How to handle about the MLMEBulkOut failed issue. Need to resend the mgmt pkt? */
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);
			pAd->bulkResetPipeid = (MGMTPIPEIDX | BULKOUT_MGMT_RESET_FLAG);
		}
	}

	pAd->BulkOutPending[MGMTPIPEIDX] = FALSE;
	RTMP_IRQ_UNLOCK(&pAd->BulkOutLock[MGMTPIPEIDX], IrqFlags);

	RTMP_IRQ_LOCK(&pAd->MLMEBulkOutLock, IrqFlags);
	/* Reset MLME context flags */
	pMLMEContext->IRPPending = FALSE;
	pMLMEContext->InUse = FALSE;
	pMLMEContext->bWaitingBulkOut = FALSE;
	pMLMEContext->BulkOutSize = 0;

	pPacket = pAd->MgmtRing.Cell[index].pNdisPacket;
	pAd->MgmtRing.Cell[index].pNdisPacket = NULL;

	/* Increase MgmtRing Index */
	INC_RING_INDEX(pAd->MgmtRing.TxDmaIdx, MGMT_RING_SIZE);
	pAd->MgmtRing.TxSwFreeIdx++;
	RTMP_IRQ_UNLOCK(&pAd->MLMEBulkOutLock, IrqFlags);

	/* No-matter success or fail, we free the mgmt packet. */
	if (pPacket)
		RTMPFreeNdisPacket(pAd, pPacket);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)
			&& ((pAd->bulkResetPipeid & BULKOUT_MGMT_RESET_FLAG)
			== BULKOUT_MGMT_RESET_FLAG)) {
			/* For Mgmt Bulk-Out failed, ignore it now. */
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {

			/* Always call Bulk routine, even reset bulk. */
			/* The protectioon of rest bulk should be in BulkOut routine */
			if (pAd->MgmtRing.TxSwFreeIdx <
			    MGMT_RING_SIZE /* pMLMEContext->bWaitingBulkOut == TRUE */) {
				RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_MLME);
			}
			RTUSBKickBulkOut(pAd);
		}
	}


#ifdef CONFIG_STA_SUPPORT
#endif /* CONFIG_STA_SUPPORT */
}

static void rtusb_hcca_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId = 4;
	purbb_t pUrb;

	DBGPRINT_RAW(RT_DEBUG_INFO, ("--->hcca_dma_done_tasklet\n"));

	pUrb = (purbb_t) data;
/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	pAd = pHTTXContext->pAd;

	rtusb_dataout_complete((unsigned long)pUrb);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) {
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			pHTTXContext = &pAd->TxContext[BulkOutPipeId];
			if ((pAd->TxSwQueue[BulkOutPipeId].Number > 0) &&
			    (pAd->DeQueueRunning[BulkOutPipeId] == FALSE) &&
			    (pHTTXContext->bCurWriting == FALSE)) {
				RTMPDeQueuePacket(pAd, FALSE, BulkOutPipeId, MAX_TX_PROCESS);
			}
#ifdef CONFIG_MULTI_CHANNEL	/* yiwei shift 4 */
			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL << QID_HCCA);
#else /* CONFIG_MULTI_CHANNEL */
			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL);
#endif /* !CONFIG_MULTI_CHANNEL */
			RTUSBKickBulkOut(pAd);
		}
	}

	DBGPRINT_RAW(RT_DEBUG_INFO, ("<---hcca_dma_done_tasklet\n"));
}

static void rtusb_ac3_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId = 3;
	purbb_t pUrb;


	pUrb = (purbb_t) data;
/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	pAd = pHTTXContext->pAd;

	rtusb_dataout_complete((unsigned long)pUrb);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) {
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			pHTTXContext = &pAd->TxContext[BulkOutPipeId];
			if ((pAd->TxSwQueue[BulkOutPipeId].Number > 0) &&
			    (pAd->DeQueueRunning[BulkOutPipeId] == FALSE) &&
			    (pHTTXContext->bCurWriting == FALSE)) {
				RTMPDeQueuePacket(pAd, FALSE, BulkOutPipeId, MAX_TX_PROCESS);
			}

			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL << 3);
			RTUSBKickBulkOut(pAd);
		}
	}
}

static void rtusb_ac2_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId = 2;
	purbb_t pUrb;


	pUrb = (purbb_t) data;
/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	pAd = pHTTXContext->pAd;

	rtusb_dataout_complete((unsigned long)pUrb);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) {
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			pHTTXContext = &pAd->TxContext[BulkOutPipeId];
			if ((pAd->TxSwQueue[BulkOutPipeId].Number > 0) &&
			    (pAd->DeQueueRunning[BulkOutPipeId] == FALSE) &&
			    (pHTTXContext->bCurWriting == FALSE)) {
				RTMPDeQueuePacket(pAd, FALSE, BulkOutPipeId, MAX_TX_PROCESS);
			}

			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL << 2);
			RTUSBKickBulkOut(pAd);
		}
	}
}

static void rtusb_ac1_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId = 1;
	purbb_t pUrb;


	pUrb = (purbb_t) data;
/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	pAd = pHTTXContext->pAd;

	rtusb_dataout_complete((unsigned long)pUrb);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) {
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			pHTTXContext = &pAd->TxContext[BulkOutPipeId];
			if ((pAd->TxSwQueue[BulkOutPipeId].Number > 0) &&
			    (pAd->DeQueueRunning[BulkOutPipeId] == FALSE) &&
			    (pHTTXContext->bCurWriting == FALSE)) {
				RTMPDeQueuePacket(pAd, FALSE, BulkOutPipeId, MAX_TX_PROCESS);
			}

			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL << 1);
			RTUSBKickBulkOut(pAd);
		}
	}
	return;

}

static void rtusb_ac0_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PHT_TX_CONTEXT pHTTXContext;
	UCHAR BulkOutPipeId = 0;
	purbb_t pUrb;


	pUrb = (purbb_t) data;
/*	pHTTXContext	= (PHT_TX_CONTEXT)pUrb->context; */
	pHTTXContext = (PHT_TX_CONTEXT) RTMP_USB_URB_DATA_GET(pUrb);
	pAd = pHTTXContext->pAd;

	rtusb_dataout_complete((unsigned long)pUrb);

	if ((RTMP_TEST_FLAG(pAd, (fRTMP_ADAPTER_RESET_IN_PROGRESS |
				  fRTMP_ADAPTER_HALT_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST)))) {
		/* do nothing and return directly. */
	} else {
		if (RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET)) {
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);
		} else {
			pHTTXContext = &pAd->TxContext[BulkOutPipeId];
			if ((pAd->TxSwQueue[BulkOutPipeId].Number > 0) &&
			    (pAd->DeQueueRunning[BulkOutPipeId] == FALSE) &&
			    (pHTTXContext->bCurWriting == FALSE)) {
				RTMPDeQueuePacket(pAd, FALSE, BulkOutPipeId, MAX_TX_PROCESS);
			}

			RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_NORMAL);
			RTUSBKickBulkOut(pAd);
		}
	}


	return;

}

#ifdef RALINK_ATE
static void rtusb_ate_ac0_dma_done_tasklet(unsigned long data)
{
	PRTMP_ADAPTER pAd;
	PTX_CONTEXT pNullContext;
	UCHAR BulkOutPipeId;
	NTSTATUS Status;
	ULONG IrqFlags;
	ULONG OldValue;
	purbb_t pURB;
	PATE_INFO pATEInfo;
	ULONG stTimeChk;

	pURB = (purbb_t) data;
	/*pNullContext = (PTX_CONTEXT)pURB->rtusb_urb_context; */
	pNullContext = (PTX_CONTEXT) RTMP_USB_URB_DATA_GET(pURB);
	pAd = pNullContext->pAd;
	pATEInfo = &(pAd->ate);

	/* Reset Null frame context flags */
	pNullContext->IRPPending = FALSE;
	pNullContext->InUse = FALSE;
	Status = RTMP_USB_URB_STATUS_GET(pURB);	/*pURB->rtusb_urb_status; */

	/* Store BulkOut PipeId. */
	BulkOutPipeId = pNullContext->BulkOutPipeId;
	pAd->BulkOutDataOneSecCount++;

	if (Status == USB_ST_NOERROR) {
		if ((ATE_ON(pAd)) && (pAd->ate.QID == BulkOutPipeId)) {
			RtmpusecDelay(10);
			pAd->ate.TxDoneCount++;
			pAd->RalinkCounters.KickTxCount++;
			ASSERT(pAd->ate.QID == 0);
#ifdef RALINK_QA
			if ((ATE_ON(pAd)) && (pAd->ate.bQATxStart == TRUE))
				pAd->ate.TxAc0++;
#endif /* RALINK_QA */
		}
		pAd->BulkOutComplete++;

		pAd->Counters8023.GoodTransmits++;

		RTMPDeQueuePacket(pAd, TRUE, BulkOutPipeId, MAX_TX_PROCESS);

	} else {
		pAd->BulkOutCompleteOther++;

		DBGPRINT(RT_DEBUG_ERROR,
			 ("BulkOutDataPacket Failed STATUS_OTHER = 0x%x .\n", Status));
		DBGPRINT(RT_DEBUG_ERROR,
			 (">>BulkOutReq=0x%lx, BulkOutComplete=0x%lx\n", pAd->BulkOutReq,
			  pAd->BulkOutComplete));

		if ((!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_RESET_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_NIC_NOT_EXIST)) &&
		    (!RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET))) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);

			/* In 28xx, RT_OID_USB_RESET_BULK_OUT ==> CMDTHREAD_RESET_BULK_OUT */
			RTEnqueueInternalCmd(pAd, CMDTHREAD_RESET_BULK_OUT, NULL, 0);

			/* check */
			BULK_OUT_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
			pAd->BulkOutPending[BulkOutPipeId] = FALSE;
			pAd->bulkResetPipeid = BulkOutPipeId;
			BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

			return;
		}
	}


	if (atomic_read(&pAd->BulkOutRemained) > 0)
		atomic_dec(&pAd->BulkOutRemained);

	/* 1st - Transmit Success */
	OldValue = pAd->WlanCounters.TransmittedFragmentCount.u.LowPart;
	pAd->WlanCounters.TransmittedFragmentCount.u.LowPart++;

	if (pAd->WlanCounters.TransmittedFragmentCount.u.LowPart < OldValue)
		pAd->WlanCounters.TransmittedFragmentCount.u.HighPart++;

	if (((pAd->ContinBulkOut == TRUE) || (atomic_read(&pAd->BulkOutRemained) > 0))
	    && (pAd->ate.Mode & ATE_TXFRAME)) {

#ifdef TXBF_SUPPORT
		/* ATE special mode: Every 500ms, send one sounding packet.
		After the packet is sent out, revert the packet back to normal packet */

		/* get system time */
		NdisGetSystemUpTime(&stTimeChk);

		if (pAd->ceBfCertificationFlg == TRUE) {
			if (pAd->timeout_flg == FALSE) {

				BULK_OUT_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

				pAd->timeout_flg = TRUE;
				pATEInfo->TxLength = pAd->txLengthBackup;
				pATEInfo->txSoundingMode = 0;

				ATESetUpFrame(pAd, 0);

				BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
			}
			if ((((stTimeChk - pAd->sounding_periodic_count)*1000/OS_HZ) >= 500)) {
				/* 500ms period */
				pAd->timeout_flg = FALSE;
				/* get and save system time */
				NdisGetSystemUpTime(&pAd->sounding_periodic_count);

				pATEInfo->TxLength = 258;
				pATEInfo->txSoundingMode = pAd->soundingMode;

				BULK_OUT_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
				pAd->soundingPacketDone = 1;

				if (pATEInfo->TxWI.TXWI_N.PHYMODE == MODE_VHT &&
				    pATEInfo->bTxBF == TRUE &&
				    pATEInfo->txSoundingMode != 0)
					ATESetUpNDPAFrame(pAd, 0);
				else
					ATESetUpFrame(pAd, 0);
				BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
			}
		}
#endif

		RTUSB_SET_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_ATE);
	} else {
		RTUSB_CLEAR_BULK_FLAG(pAd, fRTUSB_BULK_OUT_DATA_ATE);
#ifdef RALINK_QA
		pAd->ate.TxStatus = 0;
#endif /* RALINK_QA */
	}

	BULK_OUT_LOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);
	pAd->BulkOutPending[BulkOutPipeId] = FALSE;
	BULK_OUT_UNLOCK(&pAd->BulkOutLock[BulkOutPipeId], IrqFlags);

	/* Always call Bulk routine, even reset bulk. */
	/* The protection of rest bulk should be in BulkOut routine. */
	RTUSBKickBulkOut(pAd);
}
#endif /* RALINK_ATE */

NDIS_STATUS RtmpNetTaskInit(IN RTMP_ADAPTER * pAd)
{
	POS_COOKIE pObj = (POS_COOKIE) pAd->OS_Cookie;

	/* Create receive tasklet */
	RTMP_OS_TASKLET_INIT(pAd, &pObj->rx_done_task, rx_done_tasklet, (ULONG) pAd);
	/* RTMP_OS_TASKLET_INIT(pAd, &pObj->cmd_rsp_event_task, cmd_rsp_event_tasklet, (ULONG)pAd); */
	RTMP_OS_TASKLET_INIT(pAd, &pObj->mgmt_dma_done_task, rtusb_mgmt_dma_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->ac0_dma_done_task, rtusb_ac0_dma_done_tasklet,
			     (unsigned long)pAd);
#ifdef RALINK_ATE
	RTMP_OS_TASKLET_INIT(pAd, &pObj->ate_ac0_dma_done_task, rtusb_ate_ac0_dma_done_tasklet,
			     (unsigned long)pAd);
#endif /* RALINK_ATE */
	RTMP_OS_TASKLET_INIT(pAd, &pObj->ac1_dma_done_task, rtusb_ac1_dma_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->ac2_dma_done_task, rtusb_ac2_dma_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->ac3_dma_done_task, rtusb_ac3_dma_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->hcca_dma_done_task, rtusb_hcca_dma_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->tbtt_task, tbtt_tasklet, (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->null_frame_complete_task, rtusb_null_frame_done_tasklet,
			     (unsigned long)pAd);
	RTMP_OS_TASKLET_INIT(pAd, &pObj->pspoll_frame_complete_task,
			     rtusb_pspoll_frame_done_tasklet, (unsigned long)pAd);

	return NDIS_STATUS_SUCCESS;
}

void RtmpNetTaskExit(IN RTMP_ADAPTER * pAd)
{
	POS_COOKIE pObj;

	pObj = (POS_COOKIE) pAd->OS_Cookie;

	RTMP_OS_TASKLET_KILL(&pObj->rx_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->cmd_rsp_event_task);
	RTMP_OS_TASKLET_KILL(&pObj->mgmt_dma_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->ac0_dma_done_task);
#ifdef RALINK_ATE
	RTMP_OS_TASKLET_KILL(&pObj->ate_ac0_dma_done_task);
#endif /* endif */
	RTMP_OS_TASKLET_KILL(&pObj->ac1_dma_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->ac2_dma_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->ac3_dma_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->hcca_dma_done_task);
	RTMP_OS_TASKLET_KILL(&pObj->tbtt_task);
	RTMP_OS_TASKLET_KILL(&pObj->null_frame_complete_task);
	RTMP_OS_TASKLET_KILL(&pObj->pspoll_frame_complete_task);
}

/*
========================================================================
Routine Description:
    USB command kernel thread.

Arguments:
	*Context			the pAd, driver control block pointer

Return Value:
    0					close the thread

Note:
========================================================================
*/
INT RTUSBCmdThread(IN ULONG Context)
{
	RTMP_ADAPTER *pAd;
	RTMP_OS_TASK *pTask;
	int status = 0;

	pTask = (RTMP_OS_TASK *) Context;
	pAd = (PRTMP_ADAPTER) RTMP_OS_TASK_DATA_GET(pTask);

	if (pAd == NULL)
		return 0;

	RtmpOSTaskCustomize(pTask);

	NdisAcquireSpinLock(&pAd->CmdQLock);
	pAd->CmdQ.CmdQState = RTMP_TASK_STAT_RUNNING;
	NdisReleaseSpinLock(&pAd->CmdQLock);

	while (pAd->CmdQ.CmdQState == RTMP_TASK_STAT_RUNNING) {
		if (RtmpOSTaskWait(pAd, pTask, &status) == FALSE) {
			RTMP_SET_FLAG(pAd, fRTMP_ADAPTER_HALT_IN_PROGRESS);
			break;
		}

		if (pAd->CmdQ.CmdQState == RTMP_TASK_STAT_STOPED)
			break;

		if (!pAd->PM_FlgSuspend)
			CMDHandler(pAd);
	}

	if (!pAd->PM_FlgSuspend) {	/* Clear the CmdQElements. */
		CmdQElmt *pCmdQElmt = NULL;

		NdisAcquireSpinLock(&pAd->CmdQLock);
		pAd->CmdQ.CmdQState = RTMP_TASK_STAT_STOPED;
		while (pAd->CmdQ.size) {
			RTThreadDequeueCmd(&pAd->CmdQ, &pCmdQElmt);
			if (pCmdQElmt) {
				if (pCmdQElmt->CmdFromNdis == TRUE) {
					if (pCmdQElmt->buffer != NULL)
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR) pCmdQElmt);
				} else {
					if ((pCmdQElmt->buffer != NULL)
					    && (pCmdQElmt->bufferlength != 0))
						os_free_mem(pAd, pCmdQElmt->buffer);
					os_free_mem(pAd, (PUCHAR) pCmdQElmt);
				}
			}
		}

		NdisReleaseSpinLock(&pAd->CmdQLock);
	}
	/* notify the exit routine that we're actually exiting now
	 *
	 * complete()/wait_for_completion() is similar to up()/down(),
	 * except that complete() is safe in the case where the structure
	 * is getting deleted in a parallel mode of execution (i.e. just
	 * after the down() -- that's necessary for the thread-shutdown
	 * case.
	 *
	 * complete_and_exit() goes even further than this -- it is safe in
	 * the case that the thread of the caller is going away (not just
	 * the structure) -- this is necessary for the module-remove case.
	 * This is important in preemption kernels, which transfer the flow
	 * of execution immediately upon a complete().
	 */
	DBGPRINT(RT_DEBUG_TRACE, ("<---RTUSBCmdThread\n"));

	RtmpOSTaskNotifyToExit(pTask);
	return 0;

}

void InitUSBDevice(RT_CMD_USB_INIT *config, VOID *ad_src)
{
#ifdef RESUME_WITH_USB_RESET_SUPPORT
	u32 loop, mac_value;
#endif
	PRTMP_ADAPTER ad = (PRTMP_ADAPTER) ad_src;

	ad->infType = RTMP_DEV_INF_USB;

#ifdef RESUME_WITH_USB_RESET_SUPPORT
	if (ad->UsbVendorReqBuf != NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("[%s] UsbVendorReqBuf no empty\n", __func__));
		RTMP_SET_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);

		/* Check MCU if ready */
		loop = 0;
		do {
			RTUSBReadMACRegister(ad, COM_REG0, &mac_value);
			if ((mac_value & 0x01) == 0x01)
				return;

			DBGPRINT(RT_DEBUG_ERROR, ("%s: mac 730 not ready\n", __func__));
			RtmpOsMsDelay(10);
			loop++;
		} while (loop <= 100);
	}
#endif /* RESUME_WITH_USB_RESET_SUPPORT */

	RTMP_SEM_EVENT_INIT(&(ad->UsbVendorReq_semaphore), &ad->RscSemMemList);
	RTMP_SEM_EVENT_INIT(&(ad->reg_atomic), &ad->RscSemMemList);
	RTMP_SEM_EVENT_INIT(&(ad->hw_atomic), &ad->RscSemMemList);
	if (ad->UsbVendorReqBuf == NULL)
		os_alloc_mem(ad, (PUCHAR *) &ad->UsbVendorReqBuf, MAX_PARAM_BUFFER_SIZE - 1);
	else {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("UsbVendorReqBufis not null 0x%p\n", ad->UsbVendorReqBuf));
	}

	if (ad->UsbVendorReqBuf == NULL) {
		DBGPRINT(RT_DEBUG_ERROR, ("Allocate vendor request temp buffer failed!\n"));
		return;
	}
#ifdef RLT_MAC
	if (config->driver_info == RLT_MAC_BASE) {
		UINT32 value;

		RTMP_IO_READ32(ad, 0x00, &value);

		ad->ChipID = value;

		if (ad->ChipID == 0xffffffff) {
			DBGPRINT(RT_DEBUG_ERROR, ("Read chip ID failed!\n"));
			RTMP_SET_FLAG(ad, fRTMP_ADAPTER_NIC_NOT_EXIST);
			return;
		}
		DBGPRINT(RT_DEBUG_OFF, ("WiFi Chip ID = 0x%x\n", ad->ChipID));
#ifdef RT65xx
		if (IS_RT65XX(ad))
			rlt_wlan_chip_onoff(ad, TRUE, TRUE);
#endif /* endif */
#ifdef MT76x2
		if (IS_MT76x2(ad))
			mt76x2_pwrOn(ad);
#endif /* endif */
	}
#endif /* endif */

	RtmpRaDevCtrlInit(ad, ad->infType);
}

#ifdef RXPKT_THREAD
static NTSTATUS SendPktToOSHdlr(RTMP_ADAPTER *pAd)
{
	PQUEUE_ENTRY pQEntry;
	NDIS_PACKET *pPacket;
	unsigned long IrqFlags;
	UINT loopcnt = CFG_RX_OS_LOOP_CNT;

	while (loopcnt--) {
		while (pAd->rxPktQ.Number > 0) {
			RTMP_IRQ_LOCK(&pAd->rxPktQLock, IrqFlags);
			pQEntry = RemoveHeadQueue(&pAd->rxPktQ);
			pPacket = QUEUE_ENTRY_TO_PACKET(pQEntry);
			RTMP_IRQ_UNLOCK(&pAd->rxPktQLock, IrqFlags);

			if (pPacket != NULL)
				netif_rx_ni(RTPKT_TO_OSPKT(pPacket));
		}
	}
	return NDIS_STATUS_SUCCESS;
}

INT RTRxPktThread(IN ULONG Context)
{
	RTMP_ADAPTER *pAd;
	RTMP_OS_TASK *pTask;
	int status = 0;

	pTask = (RTMP_OS_TASK *) Context;
	pAd = (PRTMP_ADAPTER) RTMP_OS_TASK_DATA_GET(pTask);

	if (pAd == NULL)
		goto LabelExit;
	set_user_nice(current, -10);
	RtmpOSTaskCustomize(pTask);

	while (!RTMP_OS_TASK_IS_KILLED(pTask)) {
		if (RtmpOSTaskWait(pAd, pTask, &status) == TRUE)
			SendPktToOSHdlr(pAd);
		else
			break;

	}

	/* Clear RX Pkt Q */

LabelExit:
	DBGPRINT(RT_DEBUG_TRACE, ("<---%s\n", __func__));
	RtmpOSTaskNotifyToExit(pTask);
	return 0;

}

VOID RtmpOsRxPktUp(RTMP_OS_TASK *prxPktTask)
{
	OS_TASK *pTask = RTMP_OS_TASK_GET(prxPktTask);
#ifdef KTHREAD_SUPPORT
	pTask->kthread_running = TRUE;
	wake_up(&pTask->kthread_q);
#else
	CHECK_PID_LEGALITY(pTask->taskPID) {
		RTMP_SEM_EVENT_UP(&(pTask->taskSema));
	}
#endif /* KTHREAD_SUPPORT */
}

VOID RtmpCleanupRxPktQueue(IN RTMP_ADAPTER *pAd, IN PQUEUE_HEADER pQueue)
{
	PQUEUE_ENTRY pEntry;
	PNDIS_PACKET pPacket;

	DBGPRINT(RT_DEBUG_INFO, ("%s(): (0x%08lx)...\n", __func__, (ULONG) pQueue));

	while (pQueue->Head) {
		DBGPRINT(RT_DEBUG_INFO, ("%s():%d...\n", __func__, pQueue->Number));

		pEntry = RemoveHeadQueue(pQueue);
		/*pPacket = CONTAINING_RECORD(pEntry, NDIS_PACKET, MiniportReservedEx); */
		pPacket = QUEUE_ENTRY_TO_PACKET(pEntry);
		RELEASE_NDIS_PACKET(pAd, pPacket, NDIS_STATUS_FAILURE);
	}
}
#endif /* RXPKT_THREAD */

