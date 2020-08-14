/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_svr_fltr_queue.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/03/02
  Description   :
  History       :
  1.Date        : 2009/03/02
    Author      : qushen
    Modification: Created file

******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "linux_list.h"
#include "hi_debug.h"
#include "so_queue.h"

typedef struct hiSO_QUEUE_S
{
    HI_S32 num;    /* number of item in msg queue*/
    HI_U32 u32MaxNodeNum;
    HI_S32 s32BufferSize; /* size of buffer in msg queue */
    HI_U32 u32MaxBufferSize;
    struct list_head list;
    pthread_mutex_t lock;
    HI_HANDLE bufHandle;
} SO_QUEUE_S;

typedef struct hiSO_NODE_S
{
    struct list_head list;
    SO_INFO_S stInfo;
    HI_U8  addr[0];
} SO_NODE_S;

#define SO_ADD_EXT_BYTE_NUM   (4)
#define SO_NODE_SIZE          sizeof(SO_NODE_S)

#define SO_QUEUE_LOCK()  \
    do{\
        int ret = pthread_mutex_lock(&pSoQueue->lock);\
        if(ret != 0){\
            HI_PRINT("SO call pthread_mutex_lock(QUEUE) failure,ret = 0x%x\n",ret);\
        }\
    }while(0)

#define SO_QUEUE_UNLOCK()  \
    do{\
        int ret = pthread_mutex_unlock(&pSoQueue->lock);\
        if(ret != 0){\
            HI_PRINT("SO call pthread_mutex_unlock(QUEUE) failure,ret = 0x%x\n",ret);\
        }\
    }while(0)

static HI_S32 _SO_GetBufLen(const SO_INFO_S* pstInfo)
{
    HI_S32 s32Len = 0;

    if (HI_UNF_SUBTITLE_BITMAP == pstInfo->eType)
    {
        s32Len = (HI_S32)pstInfo->unSubtitleParam.stGfx.u32Len;
    }
    else if (HI_UNF_SUBTITLE_TEXT == pstInfo->eType)
    {
        s32Len = (HI_S32)pstInfo->unSubtitleParam.stText.u32Len;
    }
    else if (HI_UNF_SUBTITLE_ASS == pstInfo->eType)
    {
        s32Len = (HI_S32)pstInfo->unSubtitleParam.stAss.u32FrameLen;
    }
    else
    {
        s32Len = 0;
    }

    return s32Len;
}

static HI_S32 _SO_InitNode(const SO_INFO_S* pstInfo, SO_NODE_S* pstNode)
{
    if (HI_UNF_SUBTITLE_BITMAP == pstInfo->eType)
    {
        pstNode->stInfo.eType = HI_UNF_SUBTITLE_BITMAP;
        SO_MEMCPY(pstNode->addr, pstInfo->unSubtitleParam.stGfx.pu8PixData, pstInfo->unSubtitleParam.stGfx.u32Len);
        pstNode->addr[pstInfo->unSubtitleParam.stGfx.u32Len] = '\0';
        SO_MEMCPY(&pstNode->stInfo.unSubtitleParam.stGfx, &pstInfo->unSubtitleParam.stGfx, sizeof(HI_UNF_SO_GFX_S));
    }
    else if (HI_UNF_SUBTITLE_TEXT == pstInfo->eType)
    {
        pstNode->stInfo.eType = HI_UNF_SUBTITLE_TEXT;
        SO_MEMCPY(pstNode->addr, pstInfo->unSubtitleParam.stText.pu8Data, pstInfo->unSubtitleParam.stText.u32Len);
        pstNode->addr[pstInfo->unSubtitleParam.stText.u32Len] = '\0';
        SO_MEMCPY(&pstNode->stInfo.unSubtitleParam.stText, &pstInfo->unSubtitleParam.stText, sizeof(HI_UNF_SO_TEXT_S));
    }
    else if (HI_UNF_SUBTITLE_ASS == pstInfo->eType)
    {
        pstNode->stInfo.eType = HI_UNF_SUBTITLE_ASS;
        SO_MEMCPY(pstNode->addr, pstInfo->unSubtitleParam.stAss.pu8EventData, pstInfo->unSubtitleParam.stAss.u32FrameLen);
        pstNode->addr[pstInfo->unSubtitleParam.stAss.u32FrameLen] = '\0';
        SO_MEMCPY(&pstNode->stInfo.unSubtitleParam.stAss, &pstInfo->unSubtitleParam.stAss, sizeof(HI_UNF_SO_ASS_S));
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 SO_QueueInit( HI_U32 bufsize, HI_U32 maxNodeNum, SO_QUEUE_HANDLE* handle )
{
    HI_S32 ret = HI_SUCCESS;
    SO_QUEUE_S* pSoQueue = NULL;

    SO_RETURN(bufsize < SO_NORMAL_BUFF_SIZE, HI_FAILURE, "### Queue size is less than 1024!\n");

    pSoQueue = (SO_QUEUE_S*)SO_MALLOC(sizeof(SO_QUEUE_S));
    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "### No mem!\n");

    INIT_LIST_HEAD(&pSoQueue->list);
    pthread_mutex_init(&pSoQueue->lock, NULL);
    pSoQueue->num = 0;
    pSoQueue->s32BufferSize = 0;
    pSoQueue->u32MaxBufferSize = bufsize;
    pSoQueue->u32MaxNodeNum    = maxNodeNum;

    *handle = pSoQueue;
    return ret;
}

HI_S32 SO_QueueDeinit( SO_QUEUE_HANDLE handle )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;

    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "");

    (HI_VOID)SO_QueueRemove(handle);

    pthread_mutex_destroy(&pSoQueue->lock);
    SO_FREE(pSoQueue);

    return HI_SUCCESS;
}

/*lint -e429*/
HI_S32 SO_QueuePut( SO_QUEUE_HANDLE handle, const SO_INFO_S* pstInfo )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    SO_NODE_S* pNode = NULL;
    HI_S32 s32Len = 0;

    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "");
    SO_RETURN(NULL == pstInfo, HI_FAILURE, "");

    s32Len = _SO_GetBufLen(pstInfo);
    SO_RETURN(0 > s32Len, HI_FAILURE, "");

    SO_QUEUE_LOCK();
    SO_CALL_RETURN(pSoQueue->u32MaxBufferSize <= (HI_U32)pSoQueue->s32BufferSize && (HI_U32)pSoQueue->num >= pSoQueue->u32MaxNodeNum,
                   SO_QUEUE_UNLOCK(), HI_FAILURE);

    pNode = (SO_NODE_S*)SO_MALLOC(SO_NODE_SIZE + (HI_U32)s32Len + SO_ADD_EXT_BYTE_NUM);
    SO_CALL_RETURN(NULL == pNode, SO_QUEUE_UNLOCK(), HI_FAILURE);

    SO_MEMSET(&pNode->stInfo, 0, sizeof(pNode->stInfo));
    SO_MEMSET(pNode->addr, 0, s32Len);

    /* copy node info, len(4bytes)+data(len bytes) */

    (HI_VOID)_SO_InitNode(pstInfo, pNode);

    list_add_tail(&pNode->list, &pSoQueue->list);
    pSoQueue->num++;
    pSoQueue->s32BufferSize += s32Len;

    SO_QUEUE_UNLOCK();

    return HI_SUCCESS;
}
/*lint +e429*/

HI_S32 SO_QueueGet( SO_QUEUE_HANDLE handle, SO_INFO_S* pstInfo )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    SO_NODE_S*  pNode = NULL;
    HI_S32 s32Len = 0;

    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "");
    SO_RETURN(NULL == pstInfo, HI_FAILURE, "");

    SO_QUEUE_LOCK();

    if ( !list_empty(&pSoQueue->list) )
    {
        pNode = list_entry(pSoQueue->list.next, SO_NODE_S, list);
        list_del(&pNode->list);
        pSoQueue->num--;

        SO_MEMCPY(pstInfo, &pNode->stInfo, sizeof(SO_INFO_S));

        s32Len = _SO_GetBufLen(pstInfo);
        pSoQueue->s32BufferSize -= s32Len;

        if (HI_UNF_SUBTITLE_BITMAP == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stGfx.pu8PixData = pNode->addr;
        }
        else if (HI_UNF_SUBTITLE_TEXT == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stText.pu8Data = pNode->addr;
        }
        else if (HI_UNF_SUBTITLE_ASS == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stAss.pu8EventData = pNode->addr;
        }

        SO_QUEUE_UNLOCK();

        return HI_SUCCESS;
    }

    SO_QUEUE_UNLOCK();

    return HI_FAILURE;
}

HI_S32 SO_QueueGetNodeInfoNotDel( SO_QUEUE_HANDLE handle, SO_INFO_S* pstInfo )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    SO_NODE_S*  pNode = NULL;

    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "");
    SO_RETURN(NULL == pstInfo, HI_FAILURE, "");

    SO_QUEUE_LOCK();

    if ( !list_empty(&pSoQueue->list) )
    {
        pNode = list_entry(pSoQueue->list.next, SO_NODE_S, list);

        SO_MEMCPY(pstInfo, &pNode->stInfo, sizeof(SO_INFO_S));

        if (HI_UNF_SUBTITLE_BITMAP == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stGfx.pu8PixData = pNode->addr;
        }
        else if (HI_UNF_SUBTITLE_TEXT == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stText.pu8Data = pNode->addr;
        }
        else if (HI_UNF_SUBTITLE_ASS == pNode->stInfo.eType)
        {
            pstInfo->unSubtitleParam.stAss.pu8EventData = pNode->addr;
        }

        SO_QUEUE_UNLOCK();

        return HI_SUCCESS;
    }

    SO_QUEUE_UNLOCK();

    return HI_FAILURE;
}

HI_S32 SO_QueueFree( SO_QUEUE_HANDLE handle, SO_INFO_S* pstInfo )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    HI_U8* pu8Data = NULL;

    SO_RETURN(NULL == pSoQueue || NULL == pstInfo, HI_FAILURE, "");

    if (HI_UNF_SUBTITLE_BITMAP == pstInfo->eType)
    {
        pu8Data = pstInfo->unSubtitleParam.stGfx.pu8PixData;
    }
    else if (HI_UNF_SUBTITLE_TEXT == pstInfo->eType)
    {
        pu8Data = pstInfo->unSubtitleParam.stText.pu8Data;
    }
    else if (HI_UNF_SUBTITLE_ASS == pstInfo->eType)
    {
        pu8Data = pstInfo->unSubtitleParam.stAss.pu8EventData;
    }

    SO_RETURN(NULL == pu8Data, HI_FAILURE, "");

    SO_QUEUE_LOCK();
    pu8Data = pu8Data - SO_NODE_SIZE;
    SO_FREE(pu8Data);
    SO_QUEUE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 SO_QueueRemove( SO_QUEUE_HANDLE handle )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    SO_NODE_S*  pNode = NULL;
    SO_NODE_S*  n = NULL;

    SO_RETURN(pSoQueue == NULL, HI_FAILURE, "");

    SO_QUEUE_LOCK();

    list_for_each_entry_safe(pNode, n, &pSoQueue->list, list) /*lint !e10 !e26 !e40 !e48 !e64 !e530 !e666 !e1013 !e1055*/
    {
        list_del(&pNode->list);
        SO_FREE(pNode);
    }

    pSoQueue->num = 0;
    pSoQueue->s32BufferSize = 0;

    SO_QUEUE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 SO_QueueReset( SO_QUEUE_HANDLE handle )
{
    return SO_QueueRemove(handle);
}

HI_S32 SO_QueueReset_ByPts( SO_QUEUE_HANDLE handle, HI_S64 s64Pts )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    SO_NODE_S*  pNode = NULL;
    SO_NODE_S*  n = NULL;

    SO_RETURN(pSoQueue == NULL, HI_FAILURE, "");

    SO_QUEUE_LOCK();

    list_for_each_entry_safe(pNode, n, &pSoQueue->list, list) /*lint !e10 !e26 !e40 !e48 !e64 !e530 !e666 !e1013 !e1055*/
    {
        if ((HI_UNF_SUBTITLE_BITMAP == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stGfx.s64Pts < s64Pts)
            || (HI_UNF_SUBTITLE_TEXT == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stText.s64Pts < s64Pts)
            || (HI_UNF_SUBTITLE_ASS == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stAss.s64Pts < s64Pts))
        {
            pSoQueue->num --;

            if (HI_UNF_SUBTITLE_BITMAP == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stGfx.s64Pts < s64Pts)
            {
                pSoQueue->s32BufferSize -= pNode->stInfo.unSubtitleParam.stGfx.u32Len;
            }
            else if (HI_UNF_SUBTITLE_TEXT == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stText.s64Pts < s64Pts)
            {
                pSoQueue->s32BufferSize -= pNode->stInfo.unSubtitleParam.stText.u32Len;
            }
            else if (HI_UNF_SUBTITLE_ASS == pNode->stInfo.eType && pNode->stInfo.unSubtitleParam.stAss.s64Pts < s64Pts)
            {
                pSoQueue->s32BufferSize -= pNode->stInfo.unSubtitleParam.stAss.u32FrameLen;
            }

            list_del(&pNode->list);
            SO_FREE(pNode);
        }
    }

    SO_QUEUE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 SO_QueueNum( SO_QUEUE_HANDLE handle )
{
    SO_QUEUE_S* pSoQueue = (SO_QUEUE_S*)handle;
    HI_U32 num = 0;

    SO_RETURN(NULL == pSoQueue, HI_FAILURE, "");

    SO_QUEUE_LOCK();
    num = pSoQueue->num;
    SO_QUEUE_UNLOCK();

    return num;
}

