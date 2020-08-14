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
	cmm_cmd.c

	Abstract:
	All command related API.

	Revision History:
	Who			When	    What
	--------	----------  ----------------------------------------------
	Name		Date	    Modification logs
	Paul Lin    06-25-2004  created
*/

#include "rt_config.h"

/*
	========================================================================

	Routine Description:

	Arguments:

	Return Value:

	IRQL =

	Note:

	========================================================================
*/
VOID RTInitializeCmdQ(IN PCmdQ cmdq)
{
	cmdq->head = NULL;
	cmdq->tail = NULL;
	cmdq->size = 0;
	cmdq->CmdQState = RTMP_TASK_STAT_INITED;
}

/*
	========================================================================

	Routine Description:

	Arguments:

	Return Value:

	IRQL =

	Note:

	========================================================================
*/
VOID RTThreadDequeueCmd(IN PCmdQ cmdq, OUT PCmdQElmt * pcmdqelmt)
{
	*pcmdqelmt = cmdq->head;

	if (*pcmdqelmt != NULL) {
		cmdq->head = cmdq->head->next;
		cmdq->size--;
		if (cmdq->size == 0)
			cmdq->tail = NULL;
	}
}

/*
	========================================================================

	Routine Description:

	Arguments:

	Return Value:

	IRQL =

	Note:

	========================================================================
*/
BOOLEAN CMDFilterInWOW(IN PRTMP_ADAPTER pAd, IN NDIS_OID Oid)
{
	BOOLEAN ret = TRUE;

	if (RTMP_TEST_SUSPEND_FLAG(pAd, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING))
		switch (Oid) {
		case CMDTHREAD_RESET_BULK_OUT:
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_BULKOUT_RESET);
			ret = FALSE;
			DBGPRINT(RT_DEBUG_TRACE,
				("DO nothing for CMDTHREAD_RESET_BULK_OUT in SUSPEND process\n"));
			break;
		case CMDTHREAD_RESET_BULK_IN:
			RTMP_CLEAR_FLAG(pAd, fRTMP_ADAPTER_BULKIN_RESET);
			DBGPRINT(RT_DEBUG_OFF,
				("DO nothing for CMDTHREAD_RESET_BULK_IN in SUSPEND process\n"));
			ret = FALSE;
			break;
		default:
			DBGPRINT(RT_DEBUG_TRACE,
				("DO with it for CMD[%u] in SUSPEND process\n", Oid));
			break;
		}
		return ret;
}

NDIS_STATUS RTEnqueueInternalCmd(IN PRTMP_ADAPTER pAd,
				 IN NDIS_OID Oid, IN PVOID pInformationBuffer, IN UINT32 InformationBufferLength)
{
	NDIS_STATUS status;
	PCmdQElmt cmdqelmt = NULL;
	BOOLEAN isGoing = TRUE;
	/*check CMD in SYS Suspend*/
	isGoing = CMDFilterInWOW(pAd, Oid);
	if (isGoing == FALSE)
		return NDIS_STATUS_SUCCESS;

	status = os_alloc_mem(pAd, (PUCHAR *) &cmdqelmt, sizeof(CmdQElmt));
	if ((status != NDIS_STATUS_SUCCESS) || (cmdqelmt == NULL))
		return NDIS_STATUS_RESOURCES;
	NdisZeroMemory(cmdqelmt, sizeof(CmdQElmt));

	if (InformationBufferLength > 0) {
		status = os_alloc_mem(pAd, (PUCHAR *) &cmdqelmt->buffer, InformationBufferLength);
		if ((status != NDIS_STATUS_SUCCESS) || (cmdqelmt->buffer == NULL)) {
			os_free_mem(pAd, cmdqelmt);
			return NDIS_STATUS_RESOURCES;
		}

		NdisMoveMemory(cmdqelmt->buffer, pInformationBuffer, InformationBufferLength);
		cmdqelmt->bufferlength = InformationBufferLength;
	} else {
		cmdqelmt->buffer = NULL;
		cmdqelmt->bufferlength = 0;
	}

	cmdqelmt->command = Oid;
	cmdqelmt->CmdFromNdis = FALSE;

	if (cmdqelmt != NULL) {
		NdisAcquireSpinLock(&pAd->CmdQLock);
		if (pAd->CmdQ.CmdQState & RTMP_TASK_CAN_DO_INSERT) {
			EnqueueCmd((&pAd->CmdQ), cmdqelmt);
			status = NDIS_STATUS_SUCCESS;
		} else {
			status = NDIS_STATUS_FAILURE;
		}
		NdisReleaseSpinLock(&pAd->CmdQLock);

		if (status == NDIS_STATUS_FAILURE) {
			if (cmdqelmt->buffer)
				os_free_mem(pAd, cmdqelmt->buffer);
			os_free_mem(pAd, cmdqelmt);
		} else
			RTCMDUp(&pAd->cmdQTask);
	}
	return NDIS_STATUS_SUCCESS;
}
