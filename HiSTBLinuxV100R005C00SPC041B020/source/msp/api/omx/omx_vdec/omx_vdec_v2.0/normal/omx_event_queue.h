/*=================================================

Open MAX   Component: Video Decoder
This module contains the class definition for openMAX decoder component.
File:    omx_event_queue.h
Author:  yangyichang 00226912
Date:    16, 03, 2013.

=================================================*/

#ifndef __OMX_EVENT_QUEUE_H__
#define __OMX_EVENT_QUEUE_H__

#include "OMX_Types.h"

typedef struct omx_event {

    OMX_U32 param1;
    OMX_U32 param2;
    OMX_U32 id;

}omx_event;


#define OMX_CORE_CONTROL_CMDQ_SIZE   100

typedef struct omx_event_queue {

    omx_event m_q[OMX_CORE_CONTROL_CMDQ_SIZE];
    OMX_U32 m_read;
    OMX_U32 m_write;
    OMX_U32 m_size;

}omx_event_queue;


static inline OMX_U32 get_q_msg_type(omx_event_queue *queue)
{
    return queue->m_q[queue->m_read].id;
}

static inline OMX_U32 get_q_size(omx_event_queue *queue)
{
    return queue->m_size;
}

void init_event_queue(omx_event_queue *queue);
OMX_S32 push_entry(omx_event_queue *queue, OMX_U32 param1,OMX_U32 param2, OMX_U32 id);
OMX_S32 pop_entry(omx_event_queue *queue, OMX_U32 *param1, OMX_U32 *param2, OMX_U32 *id);

#endif //end of __OMX_EVENT_QUEUE_H__