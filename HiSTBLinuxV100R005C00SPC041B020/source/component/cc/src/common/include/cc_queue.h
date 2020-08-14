/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/
#ifndef __CC_QUEUE_H__
#define __CC_QUEUE_H__

#include <pthread.h>
#include <stdlib.h>

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif


#define CRITICAL_SECTION pthread_mutex_t

typedef struct tag_TRI_QUEUE
{
    HI_U32 Head;  /**<queue head*/
    HI_U32 Tail;  /**<queue tail*/
    HI_U32 Size;  /**<queue size*/
    HI_U32 Element_size;/**<element size*/
    HI_VOID *Element;/**<the element*/
    pthread_cond_t cond_insert,cond_remove;/**< pthread condition for blocking insert and remove*/
    CRITICAL_SECTION mCriticalSection; /**<mutual exclusive critical section*/
} TRI_QUEUE;

#define TIMEOUT_RET_REASON_SUCCESS 0
#define TIMEOUT_RET_REASON_TIMEOUT 1
#define TIMEOUT_RET_REASON_NO_TIME_FUNCTION 2
#define TIMEOUT_RET_REASON_QUEUE_EMPTY 3
#define TIMEOUT_RET_REASON_QUEUE_FULL 4
#define TIMEOUT_RET_REASON_WAIT_FAIL 5


void CCQueue_Init(TRI_QUEUE *queue, HI_U32 queue_size,HI_U32 element_size);

void CCQueue_Destroy(TRI_QUEUE *queue);

HI_BOOL CCQueue_Insert(TRI_QUEUE *queue, void *element);

HI_BOOL CCQueue_InsertTimeout(TRI_QUEUE *queue, void *element,HI_S32 ms,HI_U32 *reason);

HI_BOOL CCQueue_Remove(TRI_QUEUE *queue, void *element);

HI_BOOL CCQueue_RemoveTimeout(TRI_QUEUE *queue, void *element,HI_S32 ms,HI_U32 *reason);

HI_BOOL CCQueue_Get(TRI_QUEUE *queue,void *element);

HI_BOOL CCQueue_IsFull(TRI_QUEUE *queue);

HI_BOOL CCQueue_IsEmpty(TRI_QUEUE *queue);

HI_U32 CCQueue_Availability(TRI_QUEUE *queue);

HI_U32 CCQueue_GetHeadPos(TRI_QUEUE *queue);

HI_U32 CCQueue_TailPos(TRI_QUEUE *queue);

void CCQueue_Flush(TRI_QUEUE *queue);

HI_U32 CCQueue_GetItemNum(TRI_QUEUE *queue);

#ifdef __cplusplus
}
#endif

#endif


