
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_buffer.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_buffer.h"
#include "hi_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_S32 BP_Create(HI_U32 u32BufNum, BUF_ALLOC_S *pstAlloc, BUF_POOL_S *pstBP)
{
    HI_U32 u, BufStride;
    HI_S32 nRet;

    if ((!pstBP) || (!pstAlloc))
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (   (u32BufNum < WIN_BUFFER_MIN_NUMBER)
        || (u32BufNum > WIN_BUFFER_MAX_NUMBER)
        )
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    DISP_MEMSET(pstBP, 0, sizeof(BUF_POOL_S));

    pstBP->enMemType = BUF_MEM_SRC_SUPPLY;



    if (  (pstAlloc->eDataFormat != HI_DRV_PIX_FMT_NV21)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_NV12)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_NV16_2X1)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_NV61_2X1)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_YUYV)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_YVYU)
        &&(pstAlloc->eDataFormat != HI_DRV_PIX_FMT_UYVY)
       )
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (  (pstAlloc->u32BufWidth < WIN_BUFFER_MIN_W)
        ||(pstAlloc->u32BufWidth > WIN_BUFFER_MAX_W)
        ||(pstAlloc->u32BufHeight < WIN_BUFFER_MIN_H)
        ||(pstAlloc->u32BufHeight > WIN_BUFFER_MAX_H)
        )
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    pstBP->stAlloc = *pstAlloc;

    if (!pstAlloc->bFbAllocMem)
    {
        if (  (pstAlloc->u32BufStride < pstAlloc->u32BufWidth)
            ||((pstAlloc->u32BufStride & 0xful) != 0)
            )
        {
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        if (  (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV21)
            ||(pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV12)
            ||(pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
            ||(pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
           )
        {
            BufStride = (pstAlloc->u32BufWidth + 15) & 0xFFFFFFF0ul;
        }
        else if (  (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_YUYV)
                 ||(pstAlloc->eDataFormat == HI_DRV_PIX_FMT_YVYU)
                 ||(pstAlloc->eDataFormat == HI_DRV_PIX_FMT_UYVY)
                )
        {
            BufStride = ( (pstAlloc->u32BufWidth * 2) + 15) & 0xFFFFFFF0ul;
        }
        else
        {
            BufStride =  (pstAlloc->u32BufWidth + 15) & 0xFFFFFFF0ul;
        }

        pstBP->stAlloc.u32BufStride = BufStride;
    }

    if (pstAlloc->bFbAllocMem)
        pstBP->enMemType = BUF_MEM_FB_SUPPLY;
    else
        pstBP->enMemType = BUF_MEM_USER_SUPPLY;


    pstBP->pstBufQueue = (BUF_S *) DISP_MALLOC(sizeof(BUF_S) * u32BufNum);
    pstBP->pstEmptyQueue = (BUF_ID_S *)DISP_MALLOC(sizeof(BUF_ID_S) * u32BufNum);
    pstBP->pstFullQueue = (BUF_ID_S *)DISP_MALLOC(sizeof(BUF_ID_S) * u32BufNum);
    pstBP->pstCfgWritingQueue = (BUF_ID_S *)DISP_MALLOC(sizeof(BUF_ID_S) * u32BufNum);

    if (!pstBP->pstBufQueue || !pstBP->pstEmptyQueue || !pstBP->pstFullQueue || !pstBP->pstCfgWritingQueue)
    {
        goto __ERR_EXIT__;
    }

    if (pstBP->enMemType == BUF_MEM_FB_SUPPLY)
    {
        HI_U32 BufSize = 0;
        HI_CHAR BufName[10] = {'V', 'D', 'P', '_','C','a','s','t','0', '\0'};

        if (   (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV21)
            || (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV12)
           )
        {
            BufSize = (pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight * 3 >> 1);
        }

        if (   (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_YUYV)
            || (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_YVYU)
            || (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_UYVY)
           )
        {
            BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight * 2;
        }

        if (   (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
            || (pstAlloc->eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
           )
        {
            BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight * 2;
        }
        BufSize += 0x1000;
        for(u=0; u<u32BufNum; u++)
        {
            BufName[8] = (HI_CHAR)('0' + u);

            nRet = DISP_OS_MMZ_Alloc((const char *)BufName, HI_NULL, BufSize, 16, &pstBP->pstBufQueue[u].stMem);
            if (nRet)
            {
                break;
            }
        }

        if (u<u32BufNum)
        {
            HI_U32 k;

            for(k=0; k<u; k++)
            {
                DISP_OS_MMZ_Release(&pstBP->pstBufQueue[k].stMem);
            }
                goto __ERR_EXIT__;
        }
    }

    pstBP->u32BufNum = u32BufNum;

    /*this is just for storing the buffer size passed by user, not allocated buffer.*/
    pstBP->u32BufSize = pstAlloc->u32BufSize;
    pstBP->u32BufStride = pstAlloc->u32BufStride;

    BP_Reset(pstBP);

    return HI_SUCCESS;

__ERR_EXIT__:

    if (pstBP->pstFullQueue)
    {
        DISP_FREE(pstBP->pstFullQueue);
    }

    if (pstBP->pstEmptyQueue)
    {
        DISP_FREE(pstBP->pstEmptyQueue);
    }

    if (pstBP->pstBufQueue)
    {
        DISP_FREE(pstBP->pstBufQueue);
    }

    if (pstBP->pstCfgWritingQueue)
    {
        DISP_FREE(pstBP->pstCfgWritingQueue);
    }

    return HI_SUCCESS;
}


HI_S32 BP_Destroy(BUF_POOL_S *pstBP)
{
    HI_U32 u;

    // if buffer in local memory, release memory
    if (pstBP->enMemType == BUF_MEM_FB_SUPPLY)
    {
        for(u=0; u<pstBP->u32BufNum; u++)
        {
            DISP_OS_MMZ_Release(&pstBP->pstBufQueue[u].stMem);
        }
    }

    if (pstBP->pstFullQueue)
    {
        DISP_FREE(pstBP->pstFullQueue);
        pstBP->pstFullQueue = HI_NULL;
    }

    if (pstBP->pstEmptyQueue)
    {
        DISP_FREE(pstBP->pstEmptyQueue);
        pstBP->pstEmptyQueue = HI_NULL;
    }

    if (pstBP->pstBufQueue)
    {
        DISP_FREE(pstBP->pstBufQueue);
        pstBP->pstBufQueue = HI_NULL;
    }

    if (pstBP->pstCfgWritingQueue)
    {
        DISP_FREE(pstBP->pstCfgWritingQueue);
        pstBP->pstCfgWritingQueue = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 BP_Reset(BUF_POOL_S *pstBP)
{
    HI_U32 u;
    HI_U32 Coffset = 0;

    // s1 reset buf queue
    if (   (pstBP->enMemType == BUF_MEM_FB_SUPPLY)
        || (pstBP->enMemType == BUF_MEM_USER_SUPPLY)
        )
    {
        Coffset = pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight;
    }

    for (u = 0; u < pstBP->u32BufNum; u++)
    {
        pstBP->pstBufQueue[u].u32Index = u;
        pstBP->pstBufQueue[u].enState  = BUF_STATE_EMPTY;

        if ( (pstBP->enMemType == BUF_MEM_FB_SUPPLY) || (pstBP->enMemType == BUF_MEM_USER_SUPPLY))
        {
            pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32Stride_Y = pstBP->stAlloc.u32BufStride;
            pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32Stride_C = pstBP->stAlloc.u32BufStride;
            pstBP->pstBufQueue[u].stFrame.u32TunnelPhyAddr = pstBP->pstBufQueue[u].stMem.u32StartPhyAddr;

            pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32PhyAddr_Y = pstBP->pstBufQueue[u].stMem.u32StartPhyAddr + 0x1000;
            pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32PhyAddr_C = pstBP->pstBufQueue[u].stMem.u32StartPhyAddr + 0x1000 + Coffset;
            pstBP->pstBufQueue[u].stFrame.u32Width = pstBP->stAlloc.u32BufWidth;
            pstBP->pstBufQueue[u].stFrame.u32Height = pstBP->stAlloc.u32BufHeight;
            pstBP->pstBufQueue[u].stFrame.enFieldMode = HI_DRV_FIELD_ALL;

#if 0
            WIN_ERROR("F: y=0x%x, c=0x%x\n",
                    pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32PhyAddr_Y,
                    pstBP->pstBufQueue[u].stFrame.stBufAddr[0].u32PhyAddr_C);
#endif
        }
    }
    
    // s2 reset empty queue
    DISP_MEMSET(pstBP->pstEmptyQueue, 0, sizeof(BUF_ID_S) * pstBP->u32BufNum);

    for(u = 0; u < pstBP->u32BufNum; u++)
    {
        pstBP->pstEmptyQueue[u].u32Index = WIN_BUFFER_INDEX_PREFIX |(u<<WIN_BUFFER_INDEX_SHIFT);
    }

    pstBP->u32EmptyRPtr = 0;
    pstBP->u32EmptyWPtr = 0;

    // s3 reset full queue
    DISP_MEMSET(pstBP->pstFullQueue , 0, sizeof(BUF_ID_S) * pstBP->u32BufNum);
    for(u=0; u<pstBP->u32BufNum; u++)
    {
        pstBP->pstFullQueue[u].u32Index = WIN_BUFFER_INDEX_INVALID;
    }

    pstBP->u32FullRPtr = 0;
    pstBP->u32FullWPtr = 0;

    DISP_MEMSET(pstBP->pstCfgWritingQueue , 0, sizeof(BUF_ID_S) * pstBP->u32BufNum);
    for(u=0; u<pstBP->u32BufNum; u++)
    {
        pstBP->pstCfgWritingQueue[u].u32Index = WIN_BUFFER_INDEX_INVALID;
    }

    pstBP->u32CfgWritingRPtr = 0;
    pstBP->u32CfgWritingWPtr = 0;
    // s4 clean statistic
    DISP_MEMSET(&pstBP->stStatistic, 0, sizeof(BUF_STAT_S));

    return HI_SUCCESS;
}

// produce and consume empty buffer
// put in node
HI_S32 BP_GetEmptyBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId)
{
    HI_U32 id, index;

    id = pstBP->pstEmptyQueue[pstBP->u32EmptyRPtr].u32Index;
    if (WIN_BUFFER_INDEX_INVALID == id)
    {
        DISP_INFO("GE queue empty!\n");
        return HI_FAILURE;
    }

    index = (id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;
    if (BUF_STATE_EMPTY == pstBP->pstBufQueue[index].enState)
    {
        *pu32BufId = id;
        return HI_SUCCESS;
    }
    else
    {
        DISP_ERROR("GE invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}


HI_S32 BP_GetDoneBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId)
{
    HI_U32 id, index;

    id = pstBP->pstEmptyQueue[pstBP->u32EmptyRPtr].u32Index;
    if (WIN_BUFFER_INDEX_INVALID == id)
    {
        DISP_INFO("GD queue empty!\n");
        return HI_FAILURE;
    }

    index = (id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;
    if (BUF_STATE_DONE == pstBP->pstBufQueue[index].enState)
    {
        *pu32BufId = id;
        return HI_SUCCESS;
    }
    else
    {
        DISP_ERROR("GD invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}



HI_S32 BP_DelEmptyBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("DE invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (BUF_STATE_EMPTY == pstBP->pstBufQueue[index].enState)
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_WRITING;
        pstBP->pstEmptyQueue[pstBP->u32EmptyRPtr].u32Index = WIN_BUFFER_INDEX_INVALID;

        pstBP->u32EmptyRPtr = (pstBP->u32EmptyRPtr + 1) % pstBP->u32BufNum;

        pstBP->stStatistic.u32EmptyDoing = index;
        pstBP->stStatistic.u32EmptyDel++;
        return HI_SUCCESS;
    }
    else
    {
        DISP_ERROR("DE invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}

HI_S32 BP_SetBufReading(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("DE invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    pstBP->pstBufQueue[index].enState = BUF_STATE_READING;
    return HI_SUCCESS;
}

HI_S32 BP_AddEmptyBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("AE invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstEmptyQueue[pstBP->u32EmptyWPtr].u32Index != WIN_BUFFER_INDEX_INVALID)
    {
        DISP_ERROR("AE queue full!\n");
        return HI_FAILURE;
    }
    else if (pstBP->pstBufQueue[index].enState != BUF_STATE_READING)
    {
        DISP_ERROR("AE invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_DONE;
        pstBP->pstEmptyQueue[pstBP->u32EmptyWPtr].u32Index = u32BufId;
        pstBP->u32EmptyWPtr = (pstBP->u32EmptyWPtr + 1) % pstBP->u32BufNum;

        pstBP->stStatistic.u32EmptyAdd++;
        return HI_SUCCESS;
    }
}


HI_S32 BP_SetBufEmpty(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("SE invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstBufQueue[index].enState != BUF_STATE_DONE)
    {
        DISP_ERROR("SE invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_EMPTY;

        return HI_SUCCESS;
    }
}

// produce and consume full buffer
HI_S32 BP_GetFullBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId)
{
    HI_U32 id, index;

    id = pstBP->pstFullQueue[pstBP->u32FullRPtr].u32Index;
    if (WIN_BUFFER_INDEX_INVALID == id)
    {
        DISP_INFO("GF queue empty!\n");
        return HI_FAILURE;
    }

    index = (id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (BUF_STATE_FULL == pstBP->pstBufQueue[index].enState)
    {
        *pu32BufId = id;
        return HI_SUCCESS;
    }
    else
    {
        DISP_ERROR("GF invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}

HI_S32 BP_DelFullBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("DF invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (BUF_STATE_FULL == pstBP->pstBufQueue[index].enState)
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_READING;
        pstBP->pstFullQueue[pstBP->u32FullRPtr].u32Index = WIN_BUFFER_INDEX_INVALID;
        pstBP->u32FullRPtr = (pstBP->u32FullRPtr + 1) % pstBP->u32BufNum;

        pstBP->stStatistic.u32FullDoing = index;
        pstBP->stStatistic.u32FullDel++;
        return HI_SUCCESS;
    }
    else
    {
        DISP_ERROR("DF invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}

HI_S32 BP_AddFullBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("AF invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstFullQueue[pstBP->u32FullWPtr].u32Index != WIN_BUFFER_INDEX_INVALID)
    {
        DISP_ERROR("AF queue full!\n");
        return HI_FAILURE;
    }
    else if (pstBP->pstBufQueue[index].enState != BUF_STATE_WRITING)
    {
        DISP_ERROR("AF invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_FULL;
        pstBP->pstFullQueue[pstBP->u32FullWPtr].u32Index = u32BufId;
        pstBP->u32FullWPtr = (pstBP->u32FullWPtr + 1) % pstBP->u32BufNum;

        pstBP->stStatistic.u32FullAdd++;
        return HI_SUCCESS;
    }
}


// get and set frame to buffer
HI_S32 BP_GetFrame(BUF_POOL_S *pstBP, HI_U32 u32BufId, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("GetF invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

#if 0
    if (pstBP->pstBufQueue[index].enState != BUF_STATE_WORKING)
    {
        DISP_ERROR("GetF invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
#endif
    {
        *pstFrame = pstBP->pstBufQueue[index].stFrame;

        return HI_SUCCESS;
    }
}

HI_S32 BP_ReAllocBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index = 0, Coffset = 0;
    HI_S32 nRet = HI_SUCCESS, BufSize = 0;
    HI_U8  BufName[40] = {'\0'};

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if ( ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("GetF invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }


     if (  (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV21)
            ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV12)
            ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
            ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
           )
    {
        pstBP->stAlloc.u32BufStride = (pstBP->stAlloc.u32BufWidth + 15) & 0xFFFFFFF0ul;
    }
    else if (  (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YUYV)
             ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YVYU)
             ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_UYVY)
            )
    {
        pstBP->stAlloc.u32BufStride = ( (pstBP->stAlloc.u32BufWidth * 2) + 15) & 0xFFFFFFF0ul;
    }
    else
    {
        DISP_ERROR("invalid pixel format: %d\n", pstBP->stAlloc.eDataFormat);
        return HI_ERR_DISP_NOT_SUPPORT_FMT;
    }

    if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV21)
            || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV12)
           )
    {
        BufSize = (pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight * 3 >> 1);
    }

    if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YUYV)
        || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YVYU)
        || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_UYVY)
       )
    {
        BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight;
    }

    if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
        || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
       )
    {
        BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight * 2;
    }

    BufSize += DISP_CAST_LOWDELAY_MEM_LENGTH;
    DISP_OS_MMZ_Release(&pstBP->pstBufQueue[index].stMem);

    HI_OSAL_Snprintf(BufName,40,"VDP_Cast%d", index);

    nRet = DISP_OS_MMZ_Alloc((const char *)BufName, HI_NULL, BufSize, 16, &pstBP->pstBufQueue[index].stMem);
    if (nRet)
        return HI_ERR_DISP_CREATE_ERR;


    if (   (pstBP->enMemType == BUF_MEM_FB_SUPPLY)
        || (pstBP->enMemType == BUF_MEM_USER_SUPPLY)
        )
    {
        Coffset = pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight;
    }

    pstBP->pstBufQueue[index].u32Index = index;
    pstBP->pstBufQueue[index].enState  = BUF_STATE_READING;

    if ( (pstBP->enMemType == BUF_MEM_FB_SUPPLY) || (pstBP->enMemType == BUF_MEM_USER_SUPPLY))
    {
        pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32Stride_Y = pstBP->stAlloc.u32BufStride;
        pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32Stride_C = pstBP->stAlloc.u32BufStride;
        pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32PhyAddr_Y = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr + 0x1000;
        pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32PhyAddr_C = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr + 0x1000 + Coffset;
        pstBP->pstBufQueue[index].stFrame.u32TunnelPhyAddr = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr;

        pstBP->pstBufQueue[index].stFrame.u32Width = pstBP->stAlloc.u32BufWidth;
        pstBP->pstBufQueue[index].stFrame.u32Height = pstBP->stAlloc.u32BufHeight;
    }

    return HI_SUCCESS;
}

HI_S32 BP_ReAllocAllBuf(BUF_POOL_S *pstBP)
{
    HI_U32 index = 0, Coffset = 0;
    HI_S32 nRet = HI_SUCCESS, BufSize = 0;
    HI_U8  BufName[40] = {'\0'};

    for (index  = 0; index < pstBP->u32BufNum; index++)
    {    
        
         if (  (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV21)
                ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV12)
                ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
                ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
               )
        {
            pstBP->stAlloc.u32BufStride = (pstBP->stAlloc.u32BufWidth + 15) & 0xFFFFFFF0ul;
        }
        else if (  (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YUYV)
                 ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YVYU)
                 ||(pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_UYVY)
                )
        {
            pstBP->stAlloc.u32BufStride = ( (pstBP->stAlloc.u32BufWidth * 2) + 15) & 0xFFFFFFF0ul;
        }
        else
        {            
            DISP_ERROR("invalid pixel format: %d\n", pstBP->stAlloc.eDataFormat);
            return HI_ERR_DISP_NOT_SUPPORT_FMT;            
        }

        if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV21)
                || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV12)
               )
        {
            BufSize = (pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight * 3 >> 1);
        }

        if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YUYV)
            || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_YVYU)
            || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_UYVY)
           )
        {
            BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight;
        }

        if (   (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV16_2X1)
            || (pstBP->stAlloc.eDataFormat == HI_DRV_PIX_FMT_NV61_2X1)
           )
        {
            BufSize = pstBP->stAlloc.u32BufStride* pstBP->stAlloc.u32BufHeight * 2;
        }

        BufSize += DISP_CAST_LOWDELAY_MEM_LENGTH;
        DISP_OS_MMZ_Release(&pstBP->pstBufQueue[index].stMem);

        HI_OSAL_Snprintf(BufName,40,"VDP_Cast%d", index);

        nRet = DISP_OS_MMZ_Alloc((const char *)BufName, HI_NULL, BufSize, 16, &pstBP->pstBufQueue[index].stMem);
        if (nRet)
            return HI_ERR_DISP_CREATE_ERR;


        if (   (pstBP->enMemType == BUF_MEM_FB_SUPPLY)
            || (pstBP->enMemType == BUF_MEM_USER_SUPPLY)
            )
        {
            Coffset = pstBP->stAlloc.u32BufStride * pstBP->stAlloc.u32BufHeight;
        }

        pstBP->pstBufQueue[index].u32Index = index;
        pstBP->pstBufQueue[index].enState  = BUF_STATE_EMPTY;

        if ( (pstBP->enMemType == BUF_MEM_FB_SUPPLY) || (pstBP->enMemType == BUF_MEM_USER_SUPPLY))
        {
            pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32Stride_Y = pstBP->stAlloc.u32BufStride;
            pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32Stride_C = pstBP->stAlloc.u32BufStride;
            pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32PhyAddr_Y = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr + 0x1000;
            pstBP->pstBufQueue[index].stFrame.stBufAddr[0].u32PhyAddr_C = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr + 0x1000 + Coffset;
            pstBP->pstBufQueue[index].stFrame.u32TunnelPhyAddr = pstBP->pstBufQueue[index].stMem.u32StartPhyAddr;

            pstBP->pstBufQueue[index].stFrame.u32Width = pstBP->stAlloc.u32BufWidth;
            pstBP->pstBufQueue[index].stFrame.u32Height = pstBP->stAlloc.u32BufHeight;
        }
    }

    return HI_SUCCESS;
}


HI_S32 BP_SetFrame(BUF_POOL_S *pstBP, HI_U32 u32BufId, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("SetF invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstBufQueue[index].enState != BUF_STATE_WRITING)
    {
        DISP_ERROR("SetF invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        pstBP->pstBufQueue[index].stFrame = *pstFrame;
        return HI_SUCCESS;
    }
}

// get and set user data to buffer
HI_S32 BP_GetUserData(BUF_POOL_S *pstBP, HI_U32 u32BufId, BUF_USERDATA_S *pstData)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("GetU invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

#if 0
    if (pstBP->pstBufQueue[index].enState != BUF_STATE_WORKING)
    {
        DISP_ERROR("GetU invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
#endif
    {
        memcpy(pstData, &pstBP->pstBufQueue[index].stUserData, sizeof(BUF_USERDATA_S));
        return HI_SUCCESS;
    }
}

HI_S32 BP_SetUserData(BUF_POOL_S *pstBP, HI_U32 u32BufId, BUF_USERDATA_S *pstData)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("SetU invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstBufQueue[index].enState != BUF_STATE_WRITING)
    {
        DISP_ERROR("SetU invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        memcpy(&pstBP->pstBufQueue[index].stUserData, pstData, sizeof(BUF_USERDATA_S));
        return HI_SUCCESS;
    }
}

HI_S32 BP_GetFullBufNum(BUF_POOL_S *pstBP, HI_U32 *pu32BufNum)
{
    HI_U32 num;
    HI_U32 r, w;

    r = pstBP->u32FullRPtr;
    w = pstBP->u32FullWPtr;
    if (w >= r)
    {
        num = w - r;
    }
    else
    {
        num = w + pstBP->u32BufNum - r;
    }

    if (num <= pstBP->u32BufNum)
    {
        *pu32BufNum = num;
    }
    else
    {
        DISP_ERROR("BP_GetFullBufNum invalid!\n");
        *pu32BufNum = 0;
    }

    return HI_SUCCESS;
}

HI_S32 BP_GetEmptyBufNum(BUF_POOL_S *pstBP, HI_U32 *pu32BufNum)
{
    HI_U32 num;

    if (pstBP->u32EmptyWPtr >= pstBP->u32EmptyRPtr)
    {
        num = pstBP->u32EmptyWPtr - pstBP->u32EmptyRPtr;
    }
    else
    {
        num = pstBP->u32EmptyWPtr+ pstBP->u32BufNum - pstBP->u32EmptyRPtr;
    }

    if (num <= pstBP->u32BufNum)
    {
        *pu32BufNum = num;
    }
    else
    {
        DISP_ERROR("BP_GetEmptyBufNum invalid!\n");
        *pu32BufNum = 0;
    }

    return HI_SUCCESS;
}

HI_S32 BP_GetBufState(BUF_POOL_S *pstBP, BUF_STT_S *pstBufState)
{
    HI_U32 BufId, u;

    if (!pstBP || !pstBufState)
    {
        DISP_ERROR("%s input null pointer!\n", __FUNCTION__);
        return HI_FAILURE;
    }

    DISP_MEMSET(pstBufState, 0, sizeof(BUF_STT_S));

    pstBufState->u32BufNum = pstBP->u32BufNum;
    pstBufState->enMemType    = (HI_U32)pstBP->enMemType;
    pstBufState->enPixFmt     = (HI_U32)pstBP->stAlloc.eDataFormat;

    pstBufState->u32EmptyRPtr = pstBP->u32EmptyRPtr;
    pstBufState->u32EmptyWPtr = pstBP->u32EmptyWPtr;
    pstBufState->u32FullRPtr  = pstBP->u32FullRPtr;
    pstBufState->u32FullWPtr  = pstBP->u32FullWPtr;
    pstBufState->u32EmptyDel  = pstBP->stStatistic.u32EmptyDel;
    pstBufState->u32EmptyAdd  = pstBP->stStatistic.u32EmptyAdd;
    pstBufState->u32EmptyDoing= pstBP->stStatistic.u32EmptyDoing;
    pstBufState->u32FullDel   = pstBP->stStatistic.u32FullDel;
    pstBufState->u32FullAdd   = pstBP->stStatistic.u32FullAdd;
    pstBufState->u32FullDoing = pstBP->stStatistic.u32FullDoing;

    for(u=0; u<pstBP->u32BufNum; u++)
    {
        pstBufState->stBufState[u]   = (HI_U32)pstBP->pstBufQueue[u].enState;
        pstBufState->stEmptyQueue[u] = pstBP->pstEmptyQueue[u].u32Index;
        pstBufState->stFullQueue[u]  = pstBP->pstFullQueue[u].u32Index;
    }


    if (!BP_GetFullBuf(pstBP, &BufId))
    {
        (HI_VOID)BP_GetFrame(pstBP, BufId, &pstBufState->stFrame);
    }

    return HI_SUCCESS;
}



HI_S32 BP_AddCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;
    if ( ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("AC invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingWPtr].u32Index != WIN_BUFFER_INDEX_INVALID)
    {
        DISP_ERROR("AC queue full!\n");
        return HI_FAILURE;
    }
    else if (pstBP->pstBufQueue[index].enState != BUF_STATE_WRITING)
    {
        DISP_ERROR("AC invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
    else
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_WRITING;
        pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingWPtr].u32Index = u32BufId;
        pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingWPtr].u32NodeStage = 0;

        pstBP->u32CfgWritingWPtr = (pstBP->u32CfgWritingWPtr + 1) % pstBP->u32BufNum;
        pstBP->stStatistic.u32CfgAdd++;
        return HI_SUCCESS;
    }
}

HI_S32 BP_DelCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId)
{
    HI_U32 index;

    index = (u32BufId >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    if (  ((u32BufId & WIN_BUFFER_INDEX_PREFIX_MASK) != WIN_BUFFER_INDEX_PREFIX)
        ||(index >= pstBP->u32BufNum)
        )
    {
        DISP_ERROR("DC invalid id=0x%x\n", u32BufId);
        return HI_FAILURE;
    }

    if (BUF_STATE_WRITING == pstBP->pstBufQueue[index].enState)
    {
        pstBP->pstBufQueue[index].enState = BUF_STATE_WRITING;
        pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingRPtr].u32Index = WIN_BUFFER_INDEX_INVALID;
        pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingRPtr].u32NodeStage = 0;

        pstBP->u32CfgWritingRPtr = (pstBP->u32CfgWritingRPtr + 1) % pstBP->u32BufNum;
        pstBP->stStatistic.u32CfgDoing = index;
        pstBP->stStatistic.u32CfgDel++;
        return HI_SUCCESS;
    }
    else
    {
        DISP_WARN("DC invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}
HI_S32 BP_GetCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId, HI_U32 u32CfgStage,HI_U32 *u32CfgStageAct)
{
    HI_U32 id, index;

    id = pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingRPtr].u32Index;
    if (WIN_BUFFER_INDEX_INVALID == id)
    {
        DISP_INFO("GC queue empty!\n");
        return HI_FAILURE;
    }

    index = (id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;

    /*cfg frame, may be configured, may be write back already
     *we just get we want.
     */
    if ((BUF_STATE_WRITING == pstBP->pstBufQueue[index].enState)
        && (u32CfgStage <= pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingRPtr].u32NodeStage))
    {
        *pu32BufId = id;
        *u32CfgStageAct = pstBP->pstCfgWritingQueue[pstBP->u32CfgWritingRPtr].u32NodeStage;
        return HI_SUCCESS;
    }
    else
    {
        DISP_WARN("GC invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
        return HI_FAILURE;
    }
}


HI_S32 BP_GetCfgWritingBuf_JustWriting(BUF_POOL_S *pstBP, HI_U32 *pu32BufId, HI_U32 u32Stage)
{
    HI_U32 u32Id = 0,index = 0, i = 0;


    if (pstBP->u32CfgWritingRPtr == pstBP->u32CfgWritingWPtr)
    {
        return HI_FAILURE;
    }

    for (i = pstBP->u32CfgWritingRPtr;  (i % pstBP->u32BufNum)  != pstBP->u32CfgWritingWPtr; i++)
    {
        u32Id = pstBP->pstCfgWritingQueue[i % pstBP->u32BufNum].u32Index;
        if (WIN_BUFFER_INDEX_INVALID == u32Id)
        {
            DISP_INFO("GC queue empty!\n");
            return HI_FAILURE;
        }

        index = (u32Id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;
        if (BUF_STATE_WRITING != pstBP->pstBufQueue[index].enState)
        {
            DISP_INFO("GC queue empty!\n");
            return HI_FAILURE;
        }


        /*equal to 1 means the frame is written back just now.*/
        if (u32Stage == pstBP->pstCfgWritingQueue[i % pstBP->u32BufNum].u32NodeStage)
        {
            *pu32BufId  =  u32Id;

            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}


HI_VOID BP_IncreaseAllCfgWritingState(BUF_POOL_S *pstBP, HI_U32 *pu32BufId)
{
    HI_U32 u32Id = 0,index, i = 0;

    if (pstBP->u32CfgWritingRPtr == pstBP->u32CfgWritingWPtr)
    {
        return ;
    }

    for (i = pstBP->u32CfgWritingRPtr;  (i % pstBP->u32BufNum)  != pstBP->u32CfgWritingWPtr; i++)
    {
        u32Id = pstBP->pstCfgWritingQueue[i % pstBP->u32BufNum].u32Index;
        if (WIN_BUFFER_INDEX_INVALID == u32Id)
        {
            DISP_INFO("GC queue empty!\n");
            return ;
        }

        index = (u32Id >> WIN_BUFFER_INDEX_SHIFT) & WIN_BUFFER_INDEX_MASK;
        if (BUF_STATE_WRITING == pstBP->pstBufQueue[index].enState)
        {
            pstBP->pstCfgWritingQueue[i % pstBP->u32BufNum].u32NodeStage ++;
        }
        else
        {
            DISP_WARN("GC invalid state buf[%d]=%d\n", index,(HI_S32)pstBP->pstBufQueue[index].enState);
            break;
        }
    }

    return;
}

HI_S32 BP_GetIDFromFrmInfo(BUF_POOL_S *pstBP, HI_DRV_VIDEO_FRAME_S *pstFrmInfo,HI_U32 *pu32BufId)
{
    HI_U32 u32DstIdx = 0xffffffff;
    HI_S32 nRet = HI_SUCCESS;
    HI_U32 u32BPLenth = pstBP->u32BufNum;
    HI_U32 i;
    BUF_S *pstBuf;
    HI_U32 u32TmpID;
    for (i = 0;i < u32BPLenth;i++)
    {
        pstBuf = &(pstBP->pstBufQueue[i]);

        if (pstBuf->stFrame.u32FrameIndex == pstFrmInfo->u32FrameIndex
            && pstBuf->stFrame.stBufAddr[0].u32PhyAddr_Y 
                == pstFrmInfo->stBufAddr[0].u32PhyAddr_Y)
        {
            u32DstIdx = i;
        }
    }

    if (u32DstIdx == 0xffffffff)
    {
        nRet = HI_FAILURE;
    }
    else
    {
        u32TmpID = pstBP->pstBufQueue[u32DstIdx].u32Index;
        *pu32BufId = WIN_BUFFER_INDEX_PREFIX |(u32TmpID<<WIN_BUFFER_INDEX_SHIFT);
        nRet = HI_SUCCESS;
    }

    return nRet;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

