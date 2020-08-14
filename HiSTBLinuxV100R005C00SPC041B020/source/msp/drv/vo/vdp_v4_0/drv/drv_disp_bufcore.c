/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_bufcore.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_bufcore.h"
#include "drv_disp_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

//#define DISP_BUF_DEBUG 1
//#define DISP_BUFFER_DEBUG_PRINT 1

static HI_U32 s_u32BufIDCount = 1;

HI_S32 DispBuf_Create(DISP_BUF_S *pstBuffer, HI_U32 number)
{
    DISP_BUF_NODE_S *pstNodeBaseAddr = HI_NULL;
    DISP_BUF_NODE_S *pstNewNode = HI_NULL;
    HI_U32 u;

    if (!number || (number > DISP_BUF_NODE_MAX_NUMBER))
    {
        DISP_ERROR("DispBuf_Create buffer number is invalid %d\n", number);
        return HI_FAILURE;
    }

    DISP_MEMSET(pstBuffer, 0, sizeof(DISP_BUF_S));

    pstBuffer->u32BufID = (s_u32BufIDCount++) & 0xffffUL;
    pstBuffer->u32Number = number;

    pstNodeBaseAddr = (DISP_BUF_NODE_S *)DISP_MALLOC(sizeof(DISP_BUF_NODE_S)*pstBuffer->u32Number);
    if (!pstNodeBaseAddr)
    {
       DISP_ERROR("DispBuf_Create malloc node memory!\n");
       goto __ERR_RELEASE__;
    }

    pstNewNode = pstNodeBaseAddr;
    for(u=0; u<pstBuffer->u32Number; u++)
    {
        pstBuffer->pstBufArray[u] = pstNewNode++;
    }

    DispBuf_Reset(pstBuffer);
    return HI_SUCCESS;

__ERR_RELEASE__:


    return HI_FAILURE;
}

HI_S32 DispBuf_Destoy(DISP_BUF_S *pstBuffer)
{

    if (pstBuffer)
    {
        DISP_FREE(pstBuffer->pstBufArray[0]); //pstBufArray[0] has the base addr

    }
    return HI_SUCCESS;
}

HI_S32 DispBuf_Reset(DISP_BUF_S *pstBuffer)
{
    DISP_BUF_NODE_S *pstNewNode;
    HI_U32 u;

    if (pstBuffer)
    {
        // reset node
        for(u = 0; u<pstBuffer->u32Number; u++)
        {
            pstNewNode = pstBuffer->pstBufArray[u];

            DISP_MEMSET(pstNewNode, 0, sizeof(DISP_BUF_NODE_S));
            pstNewNode->u32ID = DISP_MAKE_ID(pstBuffer->u32BufID, u);
            pstNewNode->u32State = DISP_BUFN_STATE_EMPTY;
            pstNewNode->u32EmptyCount = 0;
            pstNewNode->u32FullCount  = 0;
        }

        // reset empyt array, set all node in empty array
        for(u = 0; u<pstBuffer->u32Number; u++)
        {
            pstBuffer->u32EmptyArray[u] = pstBuffer->pstBufArray[u]->u32ID;
        }
        pstBuffer->u32EmptyReadPos = 0;
        pstBuffer->u32EmptyWritePos = 0;

        // reset full array
        for(u = 0; u<pstBuffer->u32Number; u++)
        {
            pstBuffer->u32FullArray[u] = DISP_BUF_INVALID_ID;
        }
        pstBuffer->u32FullReaddPos = 0;
        pstBuffer->u32FullWritePos = 0;

        // reset used array,
        for(u = 0; u<pstBuffer->u32Number; u++)
        {
            pstBuffer->u32UsedArray[u] = DISP_BUF_INVALID_ID;
        }
        pstBuffer->u32UsedReaddPos = 0;
        pstBuffer->u32UsedWritePos = 0;
    }

    return HI_SUCCESS;
}


// create a new node
HI_S32 DispBuf_GetNodeContent(DISP_BUF_S *pstBuffer, HI_S32 s32Index, DISP_BUF_NODE_S **ppstNode)
{
    if (pstBuffer && ppstNode)
    {
        if (s32Index < pstBuffer->u32Number)
        {
            *ppstNode = pstBuffer->pstBufArray[s32Index];
            return HI_SUCCESS;
        }
    }

    return HI_FAILURE;
}

// initial node data and put in empty array
HI_S32 DispBuf_PutInitNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{


    return HI_SUCCESS;
}

#if 0
#define DISPBUF_ARRAY_EMPTY 0
#define DISPBUF_ARRAY_FULL  1
#define DISPBUF_ARRAY_USED  2
DISP_BUF_NODE_S *DispBufGetNode(DISP_BUF_S *pstBuffer, HI_U32 array, HI_U32 ID)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 nodeID;

    if (DISPBUF_ARRAY_EMPTY == array)
    {
        if (pstBuffer->u32EmptyArray[pstBuffer->u32EmptyHead] != DISP_BUF_INVALID_ID)
        {
            nodeID = DISP_GET_NODE_ID(pstBuffer->u32EmptyArray[pstBuffer->u32EmptyHead]);
            DISP_ASSERT(nodeID < pstBuffer->u32Number);

            pstTmp = pstBuffer->pstBufArray[nodeID];
            DISP_ASSERT(pstTmp->u32State == DISP_BUFN_STATE_EMPTY);

            *ppstNode = pstTmp;

            return HI_SUCCESS;
        }
    }


    return HI_SUCCESS;
}
#endif

// get first empty node and not take it away from empty array
HI_S32 DispBuf_GetEmptyNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S **ppstNode)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 nodeID;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(ppstNode);

    if (pstBuffer->u32EmptyArray[pstBuffer->u32EmptyReadPos] != DISP_BUF_INVALID_ID)
    {
        nodeID = DISP_GET_NODE_ID(pstBuffer->u32EmptyArray[pstBuffer->u32EmptyReadPos]);
        DISP_ASSERT(nodeID < pstBuffer->u32Number);
        pstTmp = pstBuffer->pstBufArray[nodeID];

        //DISP_ASSERT(pstTmp->u32State == DISP_BUFN_STATE_EMPTY);
        *ppstNode = pstTmp;
#ifdef DISP_BUFFER_DEBUG_PRINT
        printk("========= get empty %d, node=0x%x\n", pstBuffer->u32EmptyReadPos,pstTmp->u32ID);
#endif
        return HI_SUCCESS;
    }

    *ppstNode = HI_NULL;
    return HI_FAILURE;
}

// take node away from empty array and set Writing State
HI_S32 DispBuf_DelEmptyNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 nodeID;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(pstNode);
#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= del empty %d\n", pstBuffer->u32EmptyReadPos);
#endif
    DISP_ASSERT(pstBuffer->u32EmptyArray[pstBuffer->u32EmptyReadPos] != DISP_BUF_INVALID_ID);
    DISP_ASSERT(pstNode->u32State == DISP_BUFN_STATE_EMPTY);


    //START : this code is for DEBUG
    nodeID = DISP_GET_NODE_ID(pstBuffer->u32EmptyArray[pstBuffer->u32EmptyReadPos]);
    pstTmp = pstBuffer->pstBufArray[nodeID];
    DISP_ASSERT(pstTmp == pstNode);
    // END : this code is for DEBUG
    pstBuffer->u32EmptyArray[pstBuffer->u32EmptyReadPos] = DISP_BUF_INVALID_ID;
    smp_wmb();
    pstBuffer->u32EmptyReadPos = (pstBuffer->u32EmptyReadPos + 1)%pstBuffer->u32Number;
    pstNode->u32State = DISP_BUFN_STATE_WRITING;

    return HI_SUCCESS;
}

// put node into empry array and set empty state
HI_S32 DispBuf_AddEmptyNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    HI_U32 u32EmptyWritePos = 0;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(pstNode);

#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= add empty %d, node=0x%x\n", pstBuffer->u32EmptyWritePos, pstNode->u32ID);
#endif
    DISP_ASSERT(pstBuffer->u32EmptyArray[pstBuffer->u32EmptyWritePos] == DISP_BUF_INVALID_ID);
    DISP_ASSERT(pstNode->u32State == DISP_BUFN_STATE_READING);

#ifdef DISP_BUF_DEBUG
    if (pstNode->u32State != DISP_BUFN_STATE_READING)
    {
        DispBuf_PrintFullState(pstBuffer);
    }
#endif

    u32EmptyWritePos  = pstBuffer->u32EmptyWritePos;

    pstNode->u32State = DISP_BUFN_STATE_EMPTY;
    pstBuffer->u32EmptyWritePos = (pstBuffer->u32EmptyWritePos + 1)%pstBuffer->u32Number;
    smp_wmb();
    pstBuffer->u32EmptyArray[u32EmptyWritePos] = pstNode->u32ID;
    pstNode->u32EmptyCount++;

    return HI_SUCCESS;
}


// get first full node and not take it away from full array
HI_S32 DispBuf_GetFullNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S **ppstNode)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 nodeID;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(ppstNode);

    if (pstBuffer->u32FullArray[pstBuffer->u32FullReaddPos] != DISP_BUF_INVALID_ID)
    {
        nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[pstBuffer->u32FullReaddPos]);
        DISP_ASSERT(nodeID < pstBuffer->u32Number);

        pstTmp = pstBuffer->pstBufArray[nodeID];
        DISP_ASSERT(pstTmp->u32State == DISP_BUFN_STATE_FULL);

#ifdef DISP_BUF_DEBUG
        if (pstTmp->u32State != DISP_BUFN_STATE_FULL)
        {
            DispBuf_PrintFullState(pstBuffer);
        }
#endif

        //printk("--get,ptr:%d,nodeid:%d!\n", pstBuffer->u32FullReaddPos,pstTmp->u32ID);

        *ppstNode = pstTmp;
#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= get full %d, node=0x%x\n", pstBuffer->u32FullReaddPos, pstTmp->u32ID);
#endif
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

// get second full node and not take it away from full array
HI_S32 DispBuf_GetNextFullNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S **ppstNode)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 RP1, nodeID;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(ppstNode);

    RP1 = (pstBuffer->u32FullReaddPos + 1)% pstBuffer->u32Number;

    if (pstBuffer->u32FullArray[RP1] != DISP_BUF_INVALID_ID)
    {
        nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[RP1]);
        DISP_ASSERT(nodeID < pstBuffer->u32Number);

        pstTmp = pstBuffer->pstBufArray[nodeID];
        DISP_ASSERT(pstTmp->u32State == DISP_BUFN_STATE_FULL);

#ifdef DISP_BUF_DEBUG
        if (pstTmp->u32State != DISP_BUFN_STATE_FULL)
        {
            DispBuf_PrintFullState(pstBuffer);
        }
#endif

        *ppstNode = pstTmp;
#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= get nxt full %d, node=0x%x\n", RP1, pstTmp->u32ID);
#endif
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_BOOL DispBuf_FullListHasSameNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    HI_U32 u32Cnt = 0;
    HI_BOOL bRet = HI_FALSE;
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 nodeID;
    HI_DRV_VIDEO_FRAME_S *pstTmpFrm;
    HI_DRV_VIDEO_FRAME_S *pstNodeFrm;
    DBC_CHECK_NULL_RETURN(pstBuffer);

    pstNodeFrm = (HI_DRV_VIDEO_FRAME_S *)pstNode->u32Data;

    for(u32Cnt = 0; u32Cnt < DISP_BUF_NODE_MAX_NUMBER; u32Cnt++)
    {
        if (pstBuffer->u32FullArray[u32Cnt] != DISP_BUF_INVALID_ID)
        {
            nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[u32Cnt]);
            pstTmp = pstBuffer->pstBufArray[nodeID];
            pstTmpFrm = (HI_DRV_VIDEO_FRAME_S *)pstTmp->u32Data;

            if (pstTmp->u32State == DISP_BUFN_STATE_FULL
                && pstTmpFrm->u32FrameIndex == pstNodeFrm->u32FrameIndex
                && pstTmpFrm->stBufAddr[0].u32PhyAddr_Y == pstNodeFrm->stBufAddr[0].u32PhyAddr_Y)
            {
                bRet = HI_TRUE;
                break;
            }
        }
    }

    return bRet;
}

HI_S32 DispBuf_GetFullNodeByIndex(DISP_BUF_S *pstBuffer, HI_U32 u32Index, DISP_BUF_NODE_S **ppstNode)
{
    DISP_BUF_NODE_S *pstTmp;
    HI_S32 RP1, nodeID;

    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(ppstNode);

    if (u32Index >= pstBuffer->u32Number)
    {
        return HI_FAILURE;
    }

    RP1 = (pstBuffer->u32FullReaddPos + u32Index) % pstBuffer->u32Number;
    if (pstBuffer->u32FullArray[RP1] != DISP_BUF_INVALID_ID)
    {
        nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[RP1]);
        DISP_ASSERT(nodeID < pstBuffer->u32Number);

        pstTmp = pstBuffer->pstBufArray[nodeID];
        DISP_ASSERT(pstTmp->u32State == DISP_BUFN_STATE_FULL);

#ifdef DISP_BUF_DEBUG
        if (pstTmp->u32State != DISP_BUFN_STATE_FULL)
        {
            DispBuf_PrintFullState(pstBuffer);
        }
#endif

        *ppstNode = pstTmp;
#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= get nxt full %d, node=0x%x\n", RP1, pstTmp->u32ID);
#endif
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

// take node away from full array and set reading state
HI_S32 DispBuf_DelFullNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    DBC_CHECK_NULL_RETURN(pstBuffer);

    if (!pstNode)
        return HI_FAILURE;

    DBC_CHECK_NULL_RETURN(pstNode);

#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= del full %d, node=0x%x\n", pstBuffer->u32FullReaddPos, pstNode->u32ID);
#endif
    DISP_ASSERT(pstBuffer->u32FullArray[pstBuffer->u32FullReaddPos] != DISP_BUF_INVALID_ID);
    DISP_ASSERT(pstNode->u32State == DISP_BUFN_STATE_FULL);

#ifdef DISP_BUF_DEBUG
    if (pstNode->u32State != DISP_BUFN_STATE_FULL)
    {
        DispBuf_PrintFullState(pstBuffer);
    }
#endif

#ifdef DISP_BUF_DEBUG
    {
        DISP_BUF_NODE_S *pstTmp;
        HI_S32 nodeID;

        // START : this code is for DEBUG
        nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[pstBuffer->u32FullReaddPos]);
        pstTmp = pstBuffer->pstBufArray[nodeID];
        DISP_ASSERT(pstTmp == pstNode);
        // END : this code is for DEBUG
    }
#endif

    pstBuffer->u32FullArray[pstBuffer->u32FullReaddPos] = DISP_BUF_INVALID_ID;
    smp_wmb();
    pstBuffer->u32FullReaddPos = (pstBuffer->u32FullReaddPos + 1)%pstBuffer->u32Number;
    pstNode->u32State = DISP_BUFN_STATE_READING;

    return HI_SUCCESS;
}

// put node into full array and set full state
HI_S32 DispBuf_AddFullNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    HI_U32 u32FullWritePos = 0;
    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(pstNode);
#ifdef DISP_BUFFER_DEBUG_PRINT
printk("========= add full %d, node=0x%x\n", pstBuffer->u32FullWritePos, pstNode->u32ID);
#endif
    DISP_ASSERT(pstBuffer->u32FullArray[pstBuffer->u32FullWritePos] == DISP_BUF_INVALID_ID);
    DISP_ASSERT(pstNode->u32State == DISP_BUFN_STATE_WRITING);

    u32FullWritePos  = pstBuffer->u32FullWritePos;

    pstNode->u32State = DISP_BUFN_STATE_FULL;
    pstBuffer->u32FullWritePos = (pstBuffer->u32FullWritePos + 1)%pstBuffer->u32Number;
    smp_wmb();
    pstBuffer->u32FullArray[u32FullWritePos] = pstNode->u32ID;

    pstNode->u32FullCount++;

    return HI_SUCCESS;
}

// test whether the node is last one in full array
HI_S32 DispBuf_IsLastFullNode(DISP_BUF_S *pstBuffer, DISP_BUF_NODE_S *pstNode)
{
    return HI_SUCCESS;
}

HI_S32 DispBuf_GetFullNodeNumber(DISP_BUF_S *pstBuffer, HI_U32 *pu32Num)
{
    HI_U32 num;
    //HI_U32 r, w;
    HI_U32 u32Start,u32End;
    DBC_CHECK_NULL_RETURN(pstBuffer);
    DBC_CHECK_NULL_RETURN(pu32Num);

    #if 0
    r = pstBuffer->u32FullReaddPos;
    w = pstBuffer->u32FullWritePos;
    if (w >= r)
    {
        num = w - r;
    }
    else
    {
        num = w + pstBuffer->u32Number- r;
    }

    if (num <= pstBuffer->u32Number)
    {
        *pu32Num = num;
    }
    else
    {
        DISP_ERROR("DispBuf_GetFullNodeNumber invalid!\n");
        *pu32Num = 0;
    }
    #else

    u32Start = pstBuffer->u32FullReaddPos;
    u32End = pstBuffer->u32FullWritePos;
    num = 0;
    while (u32Start != u32End)
    {
        DISP_BUF_NODE_S *pstTmp;
        HI_S32 nodeID;

        if (pstBuffer->u32FullArray[u32Start] != DISP_BUF_INVALID_ID)
        {
            nodeID = DISP_GET_NODE_ID(pstBuffer->u32FullArray[u32Start]);
            DISP_ASSERT(nodeID < pstBuffer->u32Number);
            pstTmp = pstBuffer->pstBufArray[nodeID];
            num += pstTmp->u32PlayCnt;
        }

        u32Start = (u32Start + 1) % DISP_BUF_NODE_MAX_NUMBER;
    }

    *pu32Num = num;
    #endif
    return HI_SUCCESS;
}

#define DISP_BUF_PRINT printk
HI_S32 DispBuf_PrintFullState(DISP_BUF_S *pstBuffer)
{
    HI_S32 i;

    DISP_BUF_PRINT("BufId=%d, num=%d\n", pstBuffer->u32BufID, pstBuffer->u32Number);

    DISP_BUF_PRINT("EP=%d, EW=%d, FP=%d, FW=%d\n",
                      pstBuffer->u32EmptyReadPos,
                      pstBuffer->u32EmptyWritePos,
                      pstBuffer->u32FullReaddPos,
                      pstBuffer->u32FullWritePos);

    DISP_BUF_PRINT("====u32EmptyArray state===== \n");
    for(i=0;i<pstBuffer->u32Number;i++)
    {
        DISP_BUF_PRINT("[0x%x] ", pstBuffer->u32EmptyArray[i]);
    }

    DISP_BUF_PRINT("====u32FullArray state===== \n");
    for(i=0;i<pstBuffer->u32Number;i++)
    {
        DISP_BUF_PRINT("[0x%x] ", pstBuffer->u32FullArray[i]);
    }

    DISP_BUF_PRINT("====pstBufArray state===== \n");
    for(i=0;i<pstBuffer->u32Number;i++)
    {
        DISP_BUF_PRINT("[0x%x] ", pstBuffer->pstBufArray[i]->u32State);
    }

    return HI_SUCCESS;
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




