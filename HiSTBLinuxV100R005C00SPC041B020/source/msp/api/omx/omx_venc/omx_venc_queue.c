/*========================================================================
Open MAX   Component: Video Decoder
This module contains the class definition for openMAX decoder component.
file:	omx_event_queue.h
author: liucan l00180788
date:	26, DEC, 2011.
==========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "omx_dbg.h"
#include "OMX_Types.h"
#include "omx_venc_queue.h"

void init_event_queue(omx_event_queue *queue)
{
	memset(queue->m_q, 0,
		sizeof(omx_event)*OMX_CORE_CONTROL_CMDQ_SIZE);
	queue->m_read = 0;
	queue->m_write= 0;
	queue->m_size = 0;
}


OMX_S32 push_entry(omx_event_queue *queue, OMX_U32 param1, OMX_U32 param2, OMX_U32 id)           //压队列
{

	if (queue->m_size >= OMX_CORE_CONTROL_CMDQ_SIZE) {
		DEBUG_PRINT_ERROR("\nERROR: %s()::Command Queue Full", __func__);
		return -1;
	}

	queue->m_q[queue->m_write].id       = id;
	queue->m_q[queue->m_write].param1   = param1;           //存放
	queue->m_q[queue->m_write].param2   = param2;
	queue->m_write++;
	queue->m_size++;
	if(queue->m_write >= OMX_CORE_CONTROL_CMDQ_SIZE)
		queue->m_write = 0;

	return 0;
}

OMX_S32 pop_entry(omx_event_queue *queue, OMX_U32 *param1, OMX_U32 *param2, OMX_U32 *id)            //出队列
{
	if (queue->m_size == 0) {
		DEBUG_PRINT_ERROR("\nERROR: %s()::Command Queue empty", __func__);
		return -1;
	}

	*id = queue->m_q[queue->m_read].id;
	*param1 = queue->m_q[queue->m_read].param1;
	*param2 = queue->m_q[queue->m_read].param2;

	// Move the read pointer ahead
	++queue->m_read;
	--queue->m_size;
	if(queue->m_read >= OMX_CORE_CONTROL_CMDQ_SIZE)
		queue->m_read = 0;

	return 0;
}
