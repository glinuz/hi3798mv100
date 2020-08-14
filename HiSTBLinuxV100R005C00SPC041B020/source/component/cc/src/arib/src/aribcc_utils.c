/*
ZWY@20160715 for:DTS2016071104163
原有的消息收发msgsnd/msgrcv在AndroidN上被禁掉，在64位Linux上也不可用，
所以，增加此文件，参考TTX增加消息收发机制
*/

#include <stdlib.h>
#include <string.h>
#include "aribcc_utils.h"
//#include "teletext_debug.h"
#include "hi_debug.h"
#include "cc_debug.h"

#define ARIBCC_QUEUE_LOCK()  \
   do{\
        int ret = pthread_mutex_lock(&pstQueue->Queue_lock);\
        if(ret != 0){\
            HI_ERR_CC("ARIBCC call pthread_mutex_lock(QUEUE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define ARIBCC_QUEUE_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&pstQueue->Queue_lock);\
        if(ret != 0){\
            HI_ERR_CC("ARIBCC call pthread_mutex_unlock(QUEUE) failure ret = 0x%x\n",ret);\
        }\
    }while(0)

#define MIN_GAP (1536) /* 1.5KBytes*/

static HI_S32 AIRBCC_IsEmptyQueue(ARIBCC_SEGMENT_QUEUE_S* pstQueue)
{
    if (pstQueue->pstSegmentFront == pstQueue->pstSegmentRear)
    {
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 AIRBCC_IsFullQueue(ARIBCC_SEGMENT_QUEUE_S* pstQueue)
{
    HI_U32 u32Step = sizeof(ARIBCC_SEGMENT_S) + pstQueue->pstSegmentRear->u16DataLength;

    HI_U32 u32RemainSpace = 0;

    if (pstQueue->pstSegmentRear >= pstQueue->pstSegmentFront)
    {
        u32RemainSpace = ((size_t)pstQueue->pu8BaseAddr + pstQueue->u32MaxLen) - (size_t)pstQueue->pstSegmentRear;

        if ((size_t)pstQueue->pstSegmentFront > (size_t)pstQueue->pu8BaseAddr)
        {
            u32RemainSpace += ((size_t)pstQueue->pstSegmentFront - MIN_GAP - (size_t)pstQueue->pu8BaseAddr);
        }
        else
        {
            u32RemainSpace -= MIN_GAP;
        }
    }
    else
    {
        u32RemainSpace = (size_t)pstQueue->pstSegmentFront - MIN_GAP - (size_t)pstQueue->pstSegmentRear;
    }

    if (u32Step > u32RemainSpace)
    {
        /* Queue  full*/
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 ARIBCC_PesQueue_Init(ARIBCC_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen)
{
    if ((pstQueue == NULL) || (pszBufAddr == NULL) || (u32BufLen == 0))
    {
        return HI_FAILURE;
    }

    memset((void*)pszBufAddr, 0, u32BufLen);
    ARIBCC_QUEUE_LOCK();
    pstQueue->pu8BaseAddr = pszBufAddr;
    pstQueue->u32MaxLen = u32BufLen;

    pstQueue->pstSegmentFront = (ARIBCC_SEGMENT_S*)pszBufAddr;
    pstQueue->pstSegmentFront->pu8SegmentData = NULL;
    pstQueue->pstSegmentFront->pstNext = NULL;

    pstQueue->pstSegmentRear = (ARIBCC_SEGMENT_S*)pszBufAddr;
    pstQueue->pstSegmentRear->pu8SegmentData = NULL;
    pstQueue->pstSegmentRear->pstNext = NULL;
    ARIBCC_QUEUE_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 ARIBCC_PesQueue_En(ARIBCC_SEGMENT_QUEUE_S* pstQueue, ARIBCC_SEGMENT_S* pSegment)
{
    HI_U32 u32DataTypeSize = sizeof(ARIBCC_SEGMENT_S);
    HI_U32 u32ItemSize = 0;
    ARIBCC_SEGMENT_S* pstNextSegAddr = NULL;

    if ((pstQueue == NULL) || (pSegment == NULL))
    {
        return HI_FAILURE;
    }

    ARIBCC_QUEUE_LOCK();

    pstQueue->pstSegmentRear->u16DataLength = pSegment->u16DataLength;
    if (AIRBCC_IsFullQueue(pstQueue) == HI_SUCCESS)
    {
        ARIBCC_QUEUE_UNLOCK();

        return HI_FAILURE;
    }

    u32ItemSize = u32DataTypeSize + pSegment->u16DataLength;

    if (((size_t)pstQueue->pstSegmentRear + u32ItemSize) > ((size_t)pstQueue->pu8BaseAddr + pstQueue->u32MaxLen))
    {
        pstQueue->pstSegmentRear->pu8SegmentData = (HI_U8*)(pstQueue->pu8BaseAddr + u32DataTypeSize);
    }
    else
    {
        pstQueue->pstSegmentRear->pu8SegmentData = (HI_U8*)(pstQueue->pstSegmentRear + u32DataTypeSize);
    }

    pstQueue->pstSegmentRear->u16DataLength = pSegment->u16DataLength;

    memcpy(pstQueue->pstSegmentRear->pu8SegmentData, pSegment->pu8SegmentData, pSegment->u16DataLength);

    pstNextSegAddr = (ARIBCC_SEGMENT_S*)((size_t)pstQueue->pstSegmentRear + u32ItemSize);
    if (((size_t)pstNextSegAddr + u32DataTypeSize) > ((size_t)pstQueue->pu8BaseAddr + pstQueue->u32MaxLen))
    {
        pstNextSegAddr = (ARIBCC_SEGMENT_S*)pstQueue->pu8BaseAddr;
    }

    pstNextSegAddr->pu8SegmentData = NULL;
    pstNextSegAddr->pstNext = NULL;
    pstQueue->pstSegmentRear->pstNext = pstNextSegAddr;
    pstQueue->pstSegmentRear = pstNextSegAddr;
    pstQueue->pstSegmentRear->u16DataLength = 0;
    if (AIRBCC_IsFullQueue(pstQueue) == HI_SUCCESS)
    {
        ARIBCC_QUEUE_UNLOCK();
        return HI_FAILURE;
    }

    ARIBCC_QUEUE_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 ARIBCC_PesQueue_De(ARIBCC_SEGMENT_QUEUE_S* pstQueue, ARIBCC_SEGMENT_S* pSegment)
{
    ARIBCC_QUEUE_LOCK();

    if (AIRBCC_IsEmptyQueue(pstQueue) == HI_SUCCESS)
    {
        ARIBCC_QUEUE_UNLOCK();

        return HI_FAILURE;
    }

    pSegment->u16DataLength = pstQueue->pstSegmentFront->u16DataLength;

    pSegment->pu8SegmentData = pstQueue->pstSegmentFront->pu8SegmentData;
    pSegment->pstNext = NULL;
    pstQueue->pstSegmentFront = pstQueue->pstSegmentFront->pstNext;

    ARIBCC_QUEUE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 ARIBCC_PesQueue_Reset(ARIBCC_SEGMENT_QUEUE_S* pstQueue, const HI_U8* pszBufAddr, HI_U32 u32BufLen)
{
    if ((pstQueue == NULL) || (pszBufAddr == NULL) || (u32BufLen == 0))
    {
        return HI_FAILURE;
    }

    ARIBCC_QUEUE_LOCK();
    memset((void*)pszBufAddr, 0, u32BufLen);

    pstQueue->pu8BaseAddr = pszBufAddr;
    pstQueue->u32MaxLen = u32BufLen;

    pstQueue->pstSegmentFront = (ARIBCC_SEGMENT_S*)pszBufAddr;
    pstQueue->pstSegmentFront->pu8SegmentData = NULL;
    pstQueue->pstSegmentFront->pstNext = NULL;

    pstQueue->pstSegmentRear = (ARIBCC_SEGMENT_S*)pszBufAddr;
    pstQueue->pstSegmentRear->pu8SegmentData = NULL;
    pstQueue->pstSegmentRear->pstNext = NULL;

    ARIBCC_QUEUE_UNLOCK();
    return HI_SUCCESS;
}
