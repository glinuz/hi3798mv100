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

#ifdef RTMP_MAC_USB
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
#endif

NDIS_STATUS RTEnqueueInternalCmd(IN PRTMP_ADAPTER pAd,
				 IN NDIS_OID Oid, IN PVOID pInformationBuffer, IN UINT32 InformationBufferLength)
{
	NDIS_STATUS status;
	PCmdQElmt cmdqelmt = NULL;
	
#ifdef RTMP_MAC_USB
	BOOLEAN isGoing = TRUE;
	/*check CMD in SYS Suspend*/
	isGoing = CMDFilterInWOW(pAd, Oid);
	if (isGoing == FALSE)
		return NDIS_STATUS_SUCCESS;
#endif

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
