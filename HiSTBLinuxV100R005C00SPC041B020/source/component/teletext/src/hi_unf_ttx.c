#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_common.h"

#include "hi_unf_ttx.h"
#include "teletext_debug.h"
#include "teletext_data.h"
#include "teletext_parse.h"
#include "teletext_recv.h"
#include "teletext_showpage.h"
#include "teletext_utils.h"
#include "teletext_mem.h"

#ifndef HI_ADVCA_FUNCTION_RELEASE
static const HI_CHAR s_szTTXVersion[] __attribute__((used)) = USER_VERSION_STRING;
#endif

#define TTX_MAX_HANDLE (1)
#define TTX_MIN_GAP (40)
#define TTX_MIN_QUEUESIZE (20 * 1024)
#define TTX_HANDLE_BASE  ((HI_ID_TTX << 16) & 0xFFFF0000)

static HI_BOOL s_bIsInitTTX = HI_FALSE;
static pthread_mutex_t s_mutex_interface = PTHREAD_MUTEX_INITIALIZER;

static HI_VOID* s_ahTTX[TTX_MAX_HANDLE] = {0};

static HI_S32 TTX_Destroy(HI_HANDLE hTTX);

#define CHECK_PARAM(param) \
    do {                                                 \
        if (HI_NULL == param)                           \
        {                                               \
            return HI_FAILURE;                          \
        }                                               \
    } while (0)

#define TTX_MUTEX_LOCK(x)  \
    do{\
        int ret = pthread_mutex_lock(x);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_lock failure,ret = 0x%x\n",ret);\
        }\
    }while(0)

#define TTX_MUTEX_UNLOCK(x)  \
    do{\
        int ret = pthread_mutex_unlock(x);\
        if(ret != 0){\
            HI_ERR_TTX("TTX call pthread_mutex_unlock failure, ret = 0x%x\n",ret);\
        }\
    }while(0)



static HI_U16 TTX_GetIndexByHandle(HI_HANDLE hTTX)
{
    HI_U16 u16HandleIndex = hTTX & 0x0000FFFF;

    return u16HandleIndex;
}


static HI_HANDLE TTX_GetHandleByIndex(HI_U16 u16HandleIndex)
{
    HI_HANDLE hTTX = TTX_HANDLE_BASE | u16HandleIndex;

    return hTTX;
}


static HI_VOID* TTX_GetParamAddr(HI_HANDLE hTTX)
{
    HI_U16 u16HandleIndex = TTX_GetIndexByHandle(hTTX);

    if (u16HandleIndex >= TTX_MAX_HANDLE)
    {
        HI_ERR_TTX("param invalid!\n");

        return HI_NULL;
    }

    if (TTX_HANDLE_BASE == (hTTX & 0xFFFF0000))
    {
        return s_ahTTX[u16HandleIndex];
    }
    else
    {
        return HI_NULL;
    }
}

static HI_S32 TTX_GetFreeIndex(HI_U16* pu16FreeIndex)
{
    HI_U16  i = 0;

    for (i = 0; i < TTX_MAX_HANDLE; i++)
    {
        if (HI_NULL == s_ahTTX[i])
        {
            break;
        }
    }

    if (i >= TTX_MAX_HANDLE)
    {
        HI_ERR_TTX("instance num > %d\n", TTX_MAX_HANDLE);

        return HI_FAILURE;
    }

    *pu16FreeIndex = i;


    return HI_SUCCESS;
}



/**  Initialize  a  page context */
static HI_S32  TTX_PageContext_Init(HI_VOID** phDataParse)
{
    HI_S32 i = 0;
    HI_S32 j = 0;
    TTX_PAGE_CONTEXT_S* pstContext = HI_NULL;

    CHECK_PARAM(phDataParse);
    pstContext = (TTX_PAGE_CONTEXT_S*)TTX_Mem_Malloc(sizeof(TTX_PAGE_CONTEXT_S));
    CHECK_PARAM(pstContext);

    if (HI_FAILURE == TTX_Parse_SetPgAddr(&(pstContext->stCurPgAddr), 0xff, 0xff, 0))
    {
        goto ERROR;
    }

    if (HI_FAILURE == TTX_Parse_SetPgAddr(&(pstContext->stWantPgAddr), 1, 0, 0))
    {
        goto ERROR;
    }

    if (HI_FAILURE == TTX_Parse_SetPgAddr(&(pstContext->stIndexPgAddr), 0xff, 0, 0))
    {
        goto ERROR;
    }

    for (j = 0; j < TTX_FLOF_NUM; j++)
    {
        if (HI_FAILURE == TTX_Parse_SetPgAddr(&(pstContext->astLink27[j]), 0xff, 0xff, 0x3f7f))
        {
            goto ERROR;
        }
    }

    pstContext->u32Flag = 0;
    pstContext->u32ValLine = 0;
    pstContext->u32ExtValLineL = 0;
    pstContext->u32ExtValLineH = 0;
    pstContext->bTtxHiden = HI_FALSE;
    pstContext->bConceal = HI_TRUE;
    pstContext->bFlash = HI_TRUE;
    pstContext->bNavigation = HI_TRUE;
    pstContext->bTtxMix = HI_FALSE;
    pstContext->u32ColumnNum = TTX_LOP_COL_NUM;
    pstContext->u32MaxIntervalMs = TTX_MAX_TIMESPACE;

    for (i = 0; i < TTX_ROW_NUM; i++)
    {
        for (j = 0; j < TTX_CMB_MAX_COL_NUM; j++)
        {
            pstContext->u32aPageText[i][j] = 0xff;
        }
    }

    TTX_ResetInputNum(&pstContext->stInputNum);
    memset(&(pstContext->stCharSet), 0, sizeof(pstContext->stCharSet));

    pstContext->bSubtitle   = HI_FALSE;
    pstContext->bUpdateTime = HI_FALSE;
    pstContext->bInvalidReq = HI_FALSE;
    pstContext->bAutoPlay = HI_TRUE;
    pstContext->bHold = HI_FALSE;
    pstContext->pFlaAreaHead = NULL;
    pstContext->bStart = HI_TRUE;
    pstContext->bShowTaskStart = HI_TRUE;
    pstContext->bProcessed = HI_TRUE;
    pstContext->bTTXSearching = HI_FALSE;
    pstContext->pfnCallBackFunction = HI_NULL;
    pstContext->pu8PgQueueMemory = HI_NULL;
    pstContext->pstPAGEQueue   = HI_NULL;
    pstContext->u32QueueSize = 0;
    pstContext->bSubpageMode = HI_FALSE;

    if (0 != pthread_mutex_init(&(pstContext->Mutexlock), NULL))
    {
        goto ERROR;
    }

    if (0 != pthread_mutex_init(&pstContext->stPAGEQueue.Queue_lock, NULL))
    {
        goto INIT_CONTEXTMUTEX_ERROR;
    }

    pstContext->stPAGEQueue.pstSegmentFront = HI_NULL;
    pstContext->stPAGEQueue.pstSegmentRear = HI_NULL;
    pstContext->stPAGEQueue.bFull = HI_FALSE;
    pstContext->pstPAGEQueue = &pstContext->stPAGEQueue;

    *phDataParse = pstContext;

    return HI_SUCCESS;
INIT_CONTEXTMUTEX_ERROR:

    if (0 != pthread_mutex_destroy(&(pstContext->Mutexlock)))
    {
        goto ERROR;
    }

ERROR:
    TTX_Mem_Free(pstContext);
    return HI_FAILURE;
}

/**     Create a page context */
static HI_S32  TTX_PageContext_Create(HI_VOID* hDataParse)
{
    TTX_PAGE_CONTEXT_S* pstContext = HI_NULL;

    CHECK_PARAM(hDataParse);

    pstContext = (TTX_PAGE_CONTEXT_S*)hDataParse;

    /** Start  UpdateTask thread */
    if (0 != pthread_create(&pstContext->PidUpdateTask, HI_NULL, (HI_VOID * (*)(HI_VOID*))TTX_Parse_UpdateTask,
                          (HI_VOID*)pstContext))
    {
        pstContext->bStart = HI_FALSE;
        TTX_Mem_Free(pstContext);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/** Initialize queue, used in HI_UNF_TTX_SwitchContent */
static HI_S32 TTX_InitSubQueue(TTX_PAGE_CONTEXT_S* pstContext, HI_BOOL bSubtitle)
{
    HI_U32 u32MallocMemSize = 0;

    CHECK_PARAM(pstContext);

    if (HI_NULL != pstContext->pu8PgQueueMemory)
    {
        if (pstContext->bSubtitle == bSubtitle)  /** the same type, just reset queue*/
        {
            return HI_SUCCESS;
        }
        else  /** the different type , first free memory */
        {
            TTX_Mem_Free(pstContext->pu8PgQueueMemory);
            pstContext->pu8PgQueueMemory = HI_NULL;
        }
    }

    if (HI_SUCCESS != TTX_Mem_GetSpareSize(&u32MallocMemSize))
    {
        return HI_FAILURE;
    }
    else
    {
        if (HI_FALSE == bSubtitle)
        {
            if (u32MallocMemSize < TTX_MIN_QUEUESIZE)
            {
                return HI_FAILURE;
            }
            else
            {
                u32MallocMemSize = TTX_MIN_QUEUESIZE + TTX_MIN_GAP;
            }
        }
    }

    u32MallocMemSize -= TTX_MIN_GAP;

    pstContext->pu8PgQueueMemory = (HI_U8*)TTX_Mem_Malloc(u32MallocMemSize);

    if (HI_NULL == pstContext->pu8PgQueueMemory)     /**  Malloc error */
    {
        HI_ERR_TTX("malloc memory error\n");

        return HI_FAILURE;
    }
    else
    {
        pstContext->u32QueueSize = u32MallocMemSize;
    }

    if (HI_SUCCESS != TTX_PesQueue_Init(&pstContext->stPAGEQueue, pstContext->pu8PgQueueMemory, u32MallocMemSize))
    {
        TTX_Mem_Free(pstContext->pu8PgQueueMemory);

        pstContext->pu8PgQueueMemory = HI_NULL;

        HI_ERR_TTX("InitQueue error\n");

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/** Deinitialize  a  page  context*/
static HI_S32 TTX_PageContext_DeInit(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    CHECK_PARAM(pstContextHead);
    TTX_MUTEX_LOCK(&(pstContextHead->Mutexlock));
    TTX_MUTEX_UNLOCK(&(pstContextHead->Mutexlock));

    if (0 != pthread_mutex_destroy(&(pstContextHead->Mutexlock)))
    {
        HI_ERR_TTX("TTX_PageContext_DeInit error\n");
    }

    if (0 != pthread_mutex_destroy(&pstContextHead->stPAGEQueue.Queue_lock))
    {
        HI_ERR_TTX("TTX_PageContext_DeInit error\n");
    }

    if (HI_NULL != pstContextHead->pu8PgQueueMemory)
    {
        TTX_Mem_Free(pstContextHead->pu8PgQueueMemory);

        pstContextHead->pu8PgQueueMemory = HI_NULL;
    }

    pstContextHead->pu8PgQueueMemory = HI_NULL;

    TTX_Mem_Free(pstContextHead);

    return HI_SUCCESS;
}

static HI_S32  TTX_PageContext_Destroy(TTX_PAGE_CONTEXT_S* pstContextHead)
{
    CHECK_PARAM(pstContextHead);

    pstContextHead->bStart = HI_FALSE;
    pstContextHead->bShowTaskStart = HI_FALSE;
    pstContextHead->bTTXSearching = HI_FALSE;

    if (0 == pstContextHead->PidUpdateTask)
    {
        return HI_SUCCESS;
    }

    if (0 != pthread_join(pstContextHead->PidUpdateTask, NULL))
    {
        HI_ERR_TTX("TTX_PageContext_Destroy error\n");
        return HI_FAILURE;
    }

    pstContextHead->PidUpdateTask = 0;
    return HI_SUCCESS;
}

static HI_S32 TTX_InjectData(HI_HANDLE hTTX, HI_U8* pu8Data, HI_U32 u32DataSize)
{
    HI_S32 s32Ret = HI_FAILURE;
    TTX_PARAM_S* pstParam = HI_NULL;

    CHECK_PARAM(hTTX);
    CHECK_PARAM(pu8Data);

    if (0 == u32DataSize)
    {
        return HI_FAILURE;
    }

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    s32Ret = TTX_Recv_ProcessData(pstParam->hDataRecv, pu8Data, u32DataSize);

    return s32Ret;
}

static HI_S32  TTX_CheckPage(HI_HANDLE hTTX, HI_UNF_TTX_CHECK_PARAM_S* pstCheckParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    CHECK_PARAM(hTTX);
    CHECK_PARAM(pstCheckParam);

    HI_UNF_TTX_PAGE_ADDR_S stPageAddr = pstCheckParam->stPageAddr;

    if (stPageAddr.u8MagazineNum > 8)
    {
        pstCheckParam->bSucceed = HI_FALSE;
        s32Ret = HI_FAILURE;
        goto RETURN;
    }

    if (stPageAddr.u8PageNum > 99)
    {
        pstCheckParam->bSucceed = HI_FALSE;
        s32Ret = HI_FAILURE;
        goto RETURN;
    }

    TTX_DEC_TO_HEX(stPageAddr.u8PageNum);

    if (HI_SUCCESS == TTX_Data_GetValidPage(&stPageAddr.u8MagazineNum, &stPageAddr.u8PageNum,
                                 &stPageAddr.u16PageSubCode, TTX_NEXT_SUBPAGE))
    {
        pstCheckParam->bSucceed = HI_TRUE;
    }
    else
    {
        pstCheckParam->bSucceed = HI_FALSE;
    }

    s32Ret = HI_SUCCESS;
    
RETURN:
    return s32Ret;
}

static HI_S32  TTX_RequestPage(HI_HANDLE hTTX, const HI_UNF_TTX_PAGE_ADDR_S stPageAddr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = NULL;
    
    TTX_Filter_S stTtxRequest;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));

    //Init ttx filter
    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    stTtxRequest.stReqPgAddr = stPageAddr;
    stTtxRequest.NotifyFunction = TTX_Parse_GetPgData;
    stTtxRequest.pvUserData = 0;

    if (HI_SUCCESS
        == TTX_Data_GetValidPage(&stTtxRequest.stReqPgAddr.u8MagazineNum, &stTtxRequest.stReqPgAddr.u8PageNum,
                                 &stTtxRequest.stReqPgAddr.u16PageSubCode, TTX_NEXT_SUBPAGE))
    {
        if (HI_SUCCESS == TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            pstCurrentPoint->stWantPgAddr = stTtxRequest.stReqPgAddr;
        }
        else
        {
            HI_INFO_TTX("this is a invalid filter!\n");
        }


        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;

        if (HI_SUCCESS != TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            HI_INFO_TTX("this is a invalid filter!\n");
        }

        HI_INFO_TTX("Have no page!\n");
    }

    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

    return s32Ret;
}

static HI_S32 TTX_GetRawData(const HI_VOID* hParam, TTX_SEGMENT_S_PTR pstSegment)
{
    TTX_PARAM_S* pstParam = (TTX_PARAM_S*)hParam;

    CHECK_PARAM(pstParam);

    if (HI_NULL == pstSegment)    /** Invalid param, just return */
    {
        HI_ERR_TTX("Invalid pstSegment\n");
        return HI_FAILURE;
    }

    if (pstParam->stRequest.pfnRequestCallback && pstParam->stRequest.pstRawData)
    {
        HI_UNF_TTX_RAWDATA_S* pstRawData = (HI_UNF_TTX_RAWDATA_S*)pstSegment->pu8SegmentData;

        if (pstSegment->u16DataLength != sizeof(HI_UNF_TTX_RAWDATA_S))
        {
            HI_ERR_TTX("Invalid pstSegment\n");
            return HI_FAILURE;
        }

        pstParam->stRequest.pstRawData->u32ValidLines = pstRawData->u32ValidLines;
        memcpy(pstParam->stRequest.pstRawData->au8Lines, pstRawData->au8Lines, HI_UNF_TTX_MAX_LINES * HI_UNF_TTX_LINE_SIZE);
        (HI_VOID)pstParam->stRequest.pfnRequestCallback(pstParam->stRequest.pstRawData);
    }

    return HI_SUCCESS;
}

static HI_S32  TTX_RequestRawData(HI_HANDLE hTTX, const HI_UNF_TTX_REQUEST_RAWDATA_S* pstRequestRawData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    TTX_Filter_S stTtxRequest;

    CHECK_PARAM(hTTX);

    if (HI_NULL == pstRequestRawData)
    {
        HI_ERR_TTX("pstRequestRawData is null!\n");
        return HI_FAILURE;
    }

    if (HI_NULL == pstRequestRawData->pfnRequestCallback || HI_NULL == pstRequestRawData->pstRawData)
    {
        HI_ERR_TTX("pfnRequestCallback is null!\n");
        return HI_FAILURE;
    }

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstParam->stRequest = *pstRequestRawData;

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));

    //Init ttx filter
    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    stTtxRequest.NotifyFunction = TTX_GetRawData;
    stTtxRequest.pvUserData = pstParam;

    if (HI_SUCCESS != TTX_Recv_SetRequest(pstParam->hDataRecv, &stTtxRequest))
    {
        HI_ERR_TTX("failed to TTX_Recv_SetRequest!\n");
    }

    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

    return s32Ret;
}

static HI_S32 TTX_Destroy(HI_HANDLE hTTX)
{
    TTX_PARAM_S* pstParam = HI_NULL;
    HI_U16 u16HandleIndex = TTX_GetIndexByHandle(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);

    CHECK_PARAM(pstParam);

    /**  Disable display */
    if (HI_SUCCESS != TTX_Show_SetOSDOutputMsg((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse, HI_FALSE))
    {
        HI_WARN_TTX("TTX_Show_SetOSDOutputMsg failure!\n");
    }

    /**  Stop  rececive data */
    if (HI_FAILURE == TTX_Recv_Destroy(pstParam->hDataRecv))
    {
        HI_ERR_TTX( "TTX_Recv_Destroy error!\n");
    }

    /** Deinit page context */
    if (HI_SUCCESS != TTX_PageContext_Destroy((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse))
    {
        HI_ERR_TTX("TTX_PageContext_Destroy error!\n");
    }

    if (HI_SUCCESS
        != TTX_Show_CallBack((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse, HI_UNF_TTX_CB_DESTROY_BUFF, (HI_VOID*)1))
    {
        HI_ERR_TTX("TTX_Show_CallBack error!\n");
    }

    if (HI_SUCCESS != TTX_PageContext_DeInit((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse))
    {
        HI_ERR_TTX("TTX_PageContext_DeInit error!\n");
    }

    TTX_Mem_Free(pstParam);

    pstParam = HI_NULL;

    if (HI_FAILURE == TTX_Data_DeInit())
    {
        HI_ERR_TTX("TTX_Data_DeInit error!\n");
    }

    if (HI_FAILURE == TTX_Mem_DeInit())
    {
        HI_ERR_TTX("TTX_Mem_DeInit error!\n");
    }

    s_ahTTX[u16HandleIndex] = HI_NULL;

    return HI_SUCCESS;
}


static HI_VOID TTX_ProcInputNum(TTX_INPUT_NUM_S* pstInputNum, HI_U8 u8InputNumConuntMax, HI_UNF_TTX_KEY_E enKey)
{
    HI_U32 i = 0;

    if (enKey > HI_UNF_TTX_KEY_9)
    {
        return;
    }

    if (pstInputNum->u8Count == u8InputNumConuntMax)
    {
        HI_ERR_TTX("shuld not occur!\n");
        TTX_ResetInputNum(pstInputNum);
    }

    pstInputNum->au8Buf[pstInputNum->u8Count] = (HI_U8)(TTX_INT_TO_CHAR(enKey));

    if (pstInputNum->u8Count == 0)
    {
        for (i = 1; i < u8InputNumConuntMax; i++)
        {
            pstInputNum->au8Buf[i] = '-';
        }
    }

    (pstInputNum->u8Count)++;
}

static HI_S32  TTX_Handle_SubpageNumKey(HI_HANDLE hTTX, HI_UNF_TTX_KEY_E enKey)
{
    HI_U8 u8MagazineNum = 0, u8PageNum = 0;
    HI_U16 u16PageSubCode = 0;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    TTX_Filter_S stTtxRequest;
    TTX_INPUT_NUM_S* pstInputSubcode = HI_NULL;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    if (enKey > HI_UNF_TTX_KEY_9)
    {
        HI_WARN_TTX("Invalid number key!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));
    pstCurrentPoint->bHold = HI_FALSE;

    if (HI_SUCCESS != TTX_Recv_Hold(pstParam->hDataRecv, pstCurrentPoint->bHold))
    {
        HI_WARN_TTX("TTX_Recv_Hold failure!\n");
    }

    pstInputSubcode = &pstCurrentPoint->stInputNum;
    TTX_ProcInputNum(&pstCurrentPoint->stInputNum, TTX_DIGIT_NUM_OF_SUBCODE, enKey);

    if (TTX_IsLastFigureInputted(pstInputSubcode, TTX_DIGIT_NUM_OF_SUBCODE))
    {
        u8MagazineNum = pstCurrentPoint->stCurPgAddr.u8MagazineNum;
        u8PageNum = pstCurrentPoint->stCurPgAddr.u8PageNum;

        u16PageSubCode = ((TTX_CHAR_TO_INT(pstInputSubcode->au8Buf[0])) * 1000);
        u16PageSubCode += ((TTX_CHAR_TO_INT(pstInputSubcode->au8Buf[1])) * 100);
        u16PageSubCode += ((TTX_CHAR_TO_INT(pstInputSubcode->au8Buf[2])) * 10);
        u16PageSubCode += (TTX_CHAR_TO_INT(pstInputSubcode->au8Buf[3]));

        HI_INFO_TTX("current u8MagazineNum = %d u8PageNum= %d u16PageSubCode = %d\n",
                    pstCurrentPoint->stCurPgAddr.u8MagazineNum,
                    pstCurrentPoint->stCurPgAddr.u8PageNum,
                    pstCurrentPoint->stCurPgAddr.u16PageSubCode);
        HI_INFO_TTX("u8PageNum = %d u16PageSubCode = %d\n", u8PageNum, u16PageSubCode);

        if (HI_SUCCESS != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stWantPgAddr), u8MagazineNum, u8PageNum, u16PageSubCode))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        if (HI_SUCCESS != TTX_Parse_SetPgAddr(&(stTtxRequest.stReqPgAddr), u8MagazineNum, u8PageNum, u16PageSubCode))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        stTtxRequest.NotifyFunction = TTX_Parse_GetPgData;
        stTtxRequest.pvUserData = 0;

        if (HI_SUCCESS == TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            pstCurrentPoint->stWantPgAddr = stTtxRequest.stReqPgAddr;
        }
        else
        {
            HI_ERR_TTX("this is a invalid filter!\n");
        }
    }

    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

    return HI_SUCCESS;
}

static HI_S32  TTX_Handle_NumKey(HI_HANDLE hTTX, HI_UNF_TTX_KEY_E enKey)
{
    HI_U8 u8MagazineNum = 0, u8PageNum = 0;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    TTX_Filter_S stTtxRequest;
    TTX_INPUT_NUM_S* pstInputPageNum = HI_NULL;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    if (enKey > HI_UNF_TTX_KEY_9)
    {
        HI_WARN_TTX("Invalid number key!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));
    pstCurrentPoint->bHold = HI_FALSE;

    if (HI_SUCCESS != TTX_Recv_Hold(pstParam->hDataRecv, pstCurrentPoint->bHold))
    {
        HI_WARN_TTX("TTX_Recv_Hold failure!\n");
    }

    pstInputPageNum = &pstCurrentPoint->stInputNum;
    TTX_ProcInputNum(pstInputPageNum, TTX_DIGIT_NUM_OF_PAGE_NUM, enKey);

    if (TTX_IsLastFigureInputted(pstInputPageNum, TTX_DIGIT_NUM_OF_PAGE_NUM))
    {
        u8MagazineNum = (HI_U8)((TTX_CHAR_TO_INT(pstInputPageNum->au8Buf[0])) % 8);
        u8PageNum = (HI_U8)((TTX_CHAR_TO_INT(pstInputPageNum->au8Buf[1])) * 10 + (TTX_CHAR_TO_INT(pstInputPageNum->au8Buf[2])));
        TTX_DEC_TO_HEX(u8PageNum);

        if (HI_SUCCESS != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stWantPgAddr), u8MagazineNum, u8PageNum, 0))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        if (HI_SUCCESS != TTX_Parse_SetPgAddr(&(stTtxRequest.stReqPgAddr), u8MagazineNum, u8PageNum, 0))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        stTtxRequest.NotifyFunction = TTX_Parse_GetPgData;
        stTtxRequest.pvUserData = 0;

        if (HI_SUCCESS == TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            pstCurrentPoint->stWantPgAddr = stTtxRequest.stReqPgAddr;
        }
        else
        {
            HI_ERR_TTX("this is a invalid filter!\n");
        }
    }

    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

    return HI_SUCCESS;
}

static HI_S32  TTX_Handle_DirectionKey(HI_HANDLE hTTX, HI_UNF_TTX_KEY_E enKey)
{
    HI_U8 u8MagazineNum = 0;
    HI_U8 u8PageNum = 0;
    HI_S32 s32Ret = HI_FAILURE;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    TTX_Filter_S stTtxRequest;
    TTX_SEARCH_DIRECTION_E enSeaDirection = TTX_NEXT_SUBPAGE;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));

    TTX_ResetInputNum(&pstCurrentPoint->stInputNum);
    pstCurrentPoint->bHold = HI_FALSE;

    if (HI_SUCCESS != TTX_Recv_Hold(pstParam->hDataRecv, pstCurrentPoint->bHold))
    {
        HI_WARN_TTX("TTX_Recv_Hold failure!\n");
    }

    switch (enKey)
    {
        case  HI_UNF_TTX_KEY_PREVIOUS_PAGE:
        {
            if (pstCurrentPoint->stWantPgAddr.u8PageNum >= 1)
            {
                u8PageNum = pstCurrentPoint->stWantPgAddr.u8PageNum;
                TTX_HEX_TO_DEC(u8PageNum);
                u8PageNum --;
                TTX_DEC_TO_HEX(u8PageNum);
                pstCurrentPoint->stWantPgAddr.u8PageNum = u8PageNum;
                pstCurrentPoint->stWantPgAddr.u16PageSubCode = 0;
            }
            else
            {
                if (pstCurrentPoint->stWantPgAddr.u8MagazineNum == 0)
                {
                    pstCurrentPoint->stWantPgAddr.u8MagazineNum = 8;
                }

                pstCurrentPoint->stWantPgAddr.u8MagazineNum--;
                pstCurrentPoint->stWantPgAddr.u8PageNum = 0x99;
                pstCurrentPoint->stWantPgAddr.u16PageSubCode = 0;
            }

            enSeaDirection = TTX_PREV_PAGE;

            break;
        }

        case  HI_UNF_TTX_KEY_NEXT_PAGE:
        {
            u8PageNum = pstCurrentPoint->stWantPgAddr.u8PageNum;
            TTX_HEX_TO_DEC(u8PageNum);

            //HI_ERR_TTX("mag=%d page=%d\n",pstCurrentPoint->stWantPgAddr.u8MagazineNum,u8PageNum);
            if (u8PageNum < 99)
            {
                u8PageNum++;
                TTX_DEC_TO_HEX(u8PageNum);
                pstCurrentPoint->stWantPgAddr.u8PageNum = u8PageNum;
                pstCurrentPoint->stWantPgAddr.u16PageSubCode = 0;
            }
            else
            {
                pstCurrentPoint->stWantPgAddr.u8MagazineNum++;
                pstCurrentPoint->stWantPgAddr.u8MagazineNum &= 0x7;
                pstCurrentPoint->stWantPgAddr.u8PageNum = 0;
                pstCurrentPoint->stWantPgAddr.u16PageSubCode = 0;
            }

            enSeaDirection = TTX_NEXT_PAGE;

            break;
        }

        case  HI_UNF_TTX_KEY_PREVIOUS_SUBPAGE:
        {
            if ((1 <= pstCurrentPoint->stCurPgAddr.u16PageSubCode))
            {
                pstCurrentPoint->stWantPgAddr.u16PageSubCode--;
            }

            enSeaDirection = TTX_PREV_SUBPAGE;

            break;
        }

        case  HI_UNF_TTX_KEY_NEXT_SUBPAGE:
        {
            pstCurrentPoint->stWantPgAddr.u16PageSubCode++;

            enSeaDirection = TTX_NEXT_SUBPAGE;

            HI_INFO_TTX("HI_UNF_TTX_KEY_NEXT_SUBPAGE u16PageSubCode = %d!\n", pstCurrentPoint->stWantPgAddr.u16PageSubCode);
            break;
        }

        case  HI_UNF_TTX_KEY_PREVIOUS_MAGAZINE:
        {
            if (pstCurrentPoint->stWantPgAddr.u8MagazineNum == 0)
            {
                pstCurrentPoint->stWantPgAddr.u8MagazineNum = 8;
            }

            u8MagazineNum = pstCurrentPoint->stWantPgAddr.u8MagazineNum - 1;

            if (HI_SUCCESS != TTX_Parse_SetPgAddr(&pstCurrentPoint->stWantPgAddr, u8MagazineNum, 0, 0))
            {
                HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
            }

            enSeaDirection = TTX_PREV_MAGZINE;
            break;
        }

        case HI_UNF_TTX_KEY_NEXT_MAGAZINE:
        {
            pstCurrentPoint->stWantPgAddr.u8MagazineNum++;

            u8MagazineNum = pstCurrentPoint->stWantPgAddr.u8MagazineNum % 8;

            if (HI_SUCCESS != TTX_Parse_SetPgAddr(&pstCurrentPoint->stWantPgAddr, u8MagazineNum, 0, 0))
            {
                HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
            }

            enSeaDirection = TTX_NEXT_MAGZINE;
            break;
        }

        default:
        {
            HI_WARN_TTX("Invalid direction key!\n");
            s32Ret = HI_FAILURE;
            goto   RETURN;
        }
    }

    stTtxRequest.stReqPgAddr = pstCurrentPoint->stWantPgAddr;

    if (HI_SUCCESS == TTX_Data_GetValidPage(&stTtxRequest.stReqPgAddr.u8MagazineNum,
                                            &stTtxRequest.stReqPgAddr.u8PageNum,
                                            &stTtxRequest.stReqPgAddr.u16PageSubCode, enSeaDirection))
    {

        stTtxRequest.NotifyFunction = TTX_Parse_GetPgData;
        stTtxRequest.pvUserData = 0;

        if (HI_SUCCESS == TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            pstCurrentPoint->stWantPgAddr = stTtxRequest.stReqPgAddr;
        }
        else
        {
            HI_INFO_TTX("this is a invalid filter,mag=%d,page=0x%x\n", stTtxRequest.stReqPgAddr.u8MagazineNum, stTtxRequest.stReqPgAddr.u8PageNum);
        }
    }
    else
    {
        pstCurrentPoint->stWantPgAddr = pstCurrentPoint->stCurPgAddr;

        HI_INFO_TTX("No  such page!\n");
    }

    s32Ret = HI_SUCCESS;

RETURN:
    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));
    return s32Ret;
}


static HI_S32 TTX_FindSubcodeIndex(HI_U8* pu8SubcodeBuf, HI_U32 u32BufLen, HI_U8 u8SubcodeToFind)
{
    HI_U32 i = 0;

    for (i = 0; i < u32BufLen; i++)
    {
        if (u8SubcodeToFind == pu8SubcodeBuf[i])
        {
            return i;
        }
    }

    return HI_FAILURE;
}
static HI_S32  TTX_Handle_ColorKey(HI_HANDLE hTTX, HI_UNF_TTX_KEY_E enKey)
{
    HI_S32 s32Ret = HI_FAILURE;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    TTX_Filter_S stTtxRequest;
    HI_U8 au8SubPage[TTX_MAX_SUBPAGENO] = {0}, u8NumOfSubPg = TTX_MAX_SUBPAGENO;
    HI_S32 s32Index = 0;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    memset(&stTtxRequest, 0x0, sizeof(TTX_Filter_S));

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));

    TTX_ResetInputNum(&pstCurrentPoint->stInputNum);
    pstCurrentPoint->bHold = HI_FALSE;

    if (HI_SUCCESS != TTX_Recv_Hold(pstParam->hDataRecv, pstCurrentPoint->bHold))
    {
        HI_WARN_TTX("TTX_Recv_Hold failure!\n");
    }

    switch (enKey)
    {
        case  HI_UNF_TTX_KEY_RED:
        {
            /* go to previous subpage */
            if (pstCurrentPoint->bSubpageMode)
            {
                u8NumOfSubPg = TTX_MAX_SUBPAGENO;
                s32Ret |= TTX_Data_GetSubpageNum(pstCurrentPoint->stCurPgAddr.u8MagazineNum,
                                                 pstCurrentPoint->stCurPgAddr.u8PageNum,
                                                 au8SubPage, &u8NumOfSubPg);

                if (u8NumOfSubPg > 1)
                {
                    s32Index = TTX_FindSubcodeIndex(au8SubPage, u8NumOfSubPg, (HI_U8)pstCurrentPoint->stWantPgAddr.u16PageSubCode);

                    if (s32Index >= 0)
                    {
                        if (s32Index == 0)
                        {
                            s32Index = u8NumOfSubPg - 1;
                        }
                        else
                        {
                            s32Index--;
                        }

                        pstCurrentPoint->stWantPgAddr.u16PageSubCode = au8SubPage[s32Index];
                    }
                }
            }
            else
            {
                pstCurrentPoint->stWantPgAddr = pstCurrentPoint->astLink27[0];
            }

            break;
        }

        case  HI_UNF_TTX_KEY_GREEN:
        {
            /* go to next subpage */
            if (pstCurrentPoint->bSubpageMode)
            {
                u8NumOfSubPg = TTX_MAX_SUBPAGENO;
                s32Ret |= TTX_Data_GetSubpageNum(pstCurrentPoint->stCurPgAddr.u8MagazineNum,
                                                 pstCurrentPoint->stCurPgAddr.u8PageNum,
                                                 au8SubPage, &u8NumOfSubPg);

                if (u8NumOfSubPg > 1)
                {
                    s32Index = TTX_FindSubcodeIndex(au8SubPage, u8NumOfSubPg, (HI_U8)pstCurrentPoint->stWantPgAddr.u16PageSubCode);

                    if (s32Index >= 0)
                    {
                        s32Index = (s32Index + 1) % u8NumOfSubPg;
                        pstCurrentPoint->stWantPgAddr.u16PageSubCode = au8SubPage[s32Index];
                    }
                }
            }
            else
            {
                pstCurrentPoint->stWantPgAddr = pstCurrentPoint->astLink27[1];
            }

            break;
        }

        case  HI_UNF_TTX_KEY_YELLOW:
        {
            pstCurrentPoint->stWantPgAddr = pstCurrentPoint->astLink27[2];
            break;
        }

        case  HI_UNF_TTX_KEY_CYAN:
        {
            pstCurrentPoint->stWantPgAddr = pstCurrentPoint->astLink27[3];
            break;
        }

        case  HI_UNF_TTX_KEY_INDEX:
        {
            if (pstCurrentPoint->astLink27[5].u8MagazineNum != 0xff)
            {
                pstCurrentPoint->stWantPgAddr = pstCurrentPoint->astLink27[5];
            }
            else
            {
                if (pstCurrentPoint->stIndexPgAddr.u8MagazineNum != 0xff)
                {
                    pstCurrentPoint->stWantPgAddr = pstCurrentPoint->stIndexPgAddr;
                }
            }

            break;
        }

        default:
        {
            HI_WARN_TTX("Invalid Color key!\n");
            s32Ret = HI_FAILURE;
            goto RETURN;
        }
    }

    stTtxRequest.stReqPgAddr = pstCurrentPoint->stWantPgAddr;

    if (HI_SUCCESS == TTX_Data_GetValidPage(&stTtxRequest.stReqPgAddr.u8MagazineNum,
                                            &stTtxRequest.stReqPgAddr.u8PageNum,
                                            &stTtxRequest.stReqPgAddr.u16PageSubCode, TTX_NEXT_SUBPAGE))
    {
        stTtxRequest.NotifyFunction = TTX_Parse_GetPgData;
        stTtxRequest.pvUserData = 0;

        if (HI_SUCCESS == TTX_Recv_SetFilter(pstParam->hDataRecv, pstParam->hDataParse, &stTtxRequest))
        {
            pstCurrentPoint->stWantPgAddr = stTtxRequest.stReqPgAddr;
        }
        else
        {
            HI_INFO_TTX("this is a invalid filter!\n");
        }
    }
    else
    {
        pstCurrentPoint->bInvalidReq  = HI_TRUE;
        pstCurrentPoint->stWantPgAddr = pstCurrentPoint->stCurPgAddr;
    }

    s32Ret = HI_SUCCESS;
RETURN:
    TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));
    return s32Ret;
}

static HI_S32 TTX_Handle_ControlKey(HI_HANDLE hTTX, HI_UNF_TTX_KEY_E enKey)
{
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    switch (enKey)
    {
        case  HI_UNF_TTX_KEY_MIX:
        {
            TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));
            pstCurrentPoint->bTtxMix = pstCurrentPoint->bTtxMix ? HI_FALSE : HI_TRUE;
            TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        case  HI_UNF_TTX_KEY_REVEAL:
        {
            TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));
            pstCurrentPoint->bConceal = pstCurrentPoint->bConceal ? HI_FALSE : HI_TRUE;
            TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        case  HI_UNF_TTX_KEY_UPDATE:
        {
            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        case  HI_UNF_TTX_KEY_HOLD:
        {
            pstCurrentPoint->bHold = pstCurrentPoint->bHold ? HI_FALSE : HI_TRUE;
            HI_INFO_TTX("HI_UNF_TTX_KEY_HOLD bHold = %d!\n", pstCurrentPoint->bHold);

            if (HI_SUCCESS != TTX_Recv_Hold(pstParam->hDataRecv, pstCurrentPoint->bHold))
            {
                HI_WARN_TTX("TTX_Recv_Hold failure!\n");
            }

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        case  HI_UNF_TTX_KEY_ZOOM:
        {
            /* TODO */
            pstCurrentPoint->u32ZoomFlag = (pstCurrentPoint->u32ZoomFlag + 1) % 3;

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        case  HI_UNF_TTX_KEY_SUBPAGE:
        {
            /* TODO */
            pstCurrentPoint->bSubpageMode = pstCurrentPoint->bSubpageMode ? HI_FALSE : HI_TRUE;

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stCurPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }

            break;
        }

        default:
        {
            HI_WARN_TTX("Invalid Control key!\n");
        }
    }

    return HI_SUCCESS;
}


static HI_VOID TTX_ExecKeyCmd(HI_HANDLE hTTX, TTX_PAGE_CONTEXT_S* pstCurrentPoint, HI_UNF_TTX_KEY_E enKey)
{
    switch (enKey)
    {
        case HI_UNF_TTX_KEY_0:
        case HI_UNF_TTX_KEY_1:
        case HI_UNF_TTX_KEY_2:
        case HI_UNF_TTX_KEY_3:
        case HI_UNF_TTX_KEY_4:
        case HI_UNF_TTX_KEY_5:
        case HI_UNF_TTX_KEY_6:
        case HI_UNF_TTX_KEY_7:
        case HI_UNF_TTX_KEY_8:
        case HI_UNF_TTX_KEY_9:
        {
            if (pstCurrentPoint->bSubpageMode)
            {
                if (HI_SUCCESS != TTX_Handle_SubpageNumKey(hTTX, enKey))
                {
                    HI_WARN_TTX("TTX_Handle_SubpageNumKey failure!\n");
                }
            }
            else
            {
                if (HI_SUCCESS != TTX_Handle_NumKey(hTTX, enKey))
                {
                    HI_WARN_TTX("TTX_Handle_NumKey failure!\n");
                }
            }

            break;
        }

        case HI_UNF_TTX_KEY_PREVIOUS_PAGE:                           /**  prve page */
        case HI_UNF_TTX_KEY_NEXT_PAGE:                   /** Next page */
        case HI_UNF_TTX_KEY_PREVIOUS_SUBPAGE:                       /** Prve subpage */
        case HI_UNF_TTX_KEY_NEXT_SUBPAGE:                    /** Next subpage */
        case HI_UNF_TTX_KEY_PREVIOUS_MAGAZINE:                       /** Prve magazine */
        case HI_UNF_TTX_KEY_NEXT_MAGAZINE:                       /** Next magazine */
        {
            if (HI_SUCCESS != TTX_Handle_DirectionKey(hTTX, enKey))
            {
                HI_WARN_TTX("TTX_Handle_DirectionKey failure!\n");
            }

            break;
        }

        case HI_UNF_TTX_KEY_RED:                    /** First link */
        case HI_UNF_TTX_KEY_GREEN:              /** Second link */
        case HI_UNF_TTX_KEY_YELLOW:              /** Third link */
        case HI_UNF_TTX_KEY_CYAN:                  /** Fourth link */
        case HI_UNF_TTX_KEY_INDEX:                  /** Index page */
        {
            if (HI_SUCCESS != TTX_Handle_ColorKey(hTTX, enKey))
            {
                HI_WARN_TTX("TTX_Handle_ColorKey failure!\n");
            }

            break;
        }

        case HI_UNF_TTX_KEY_REVEAL:                  /** Display/conceal  teletext  */
        case HI_UNF_TTX_KEY_HOLD:                     /** Start / stop autoplay */
        case HI_UNF_TTX_KEY_MIX:                        /** Enable /disable mix */
        case HI_UNF_TTX_KEY_UPDATE:                 /** Update current page */
        case HI_UNF_TTX_KEY_ZOOM:
        case HI_UNF_TTX_KEY_SUBPAGE:
        {
            if (HI_SUCCESS != TTX_Handle_ControlKey(hTTX, enKey))
            {
                HI_WARN_TTX("TTX_Handle_ControlKey failure!\n");
            }

            break;
        }

        default:
        {
            HI_INFO_TTX("Unkown Key!\n");
            break;
        }
    }
}



HI_S32 HI_UNF_TTX_Init(HI_VOID )
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (!s_bIsInitTTX)
    {
        if (0 != pthread_mutex_init(&s_mutex_interface, HI_NULL))
        {
            s32Ret = HI_FAILURE;
        }

        if (HI_SUCCESS != HI_MODULE_Register(HI_ID_TTX, "HI_TTX"))
        {
            HI_ERR_TTX("HI_MODULE_Register failure, s32Ret = 0x%x!\n", s32Ret);
        }
        else
        {
            HI_SYS_SetLogLevel(HI_ID_TTX, HI_LOG_LEVEL_ERROR);
        }

        s_bIsInitTTX = HI_TRUE;
    }

    return s32Ret;
}

HI_S32 HI_UNF_TTX_DeInit(HI_VOID)
{
    HI_U16 i = 0;

    HI_MODULE_UnRegister(HI_ID_TTX);

    for (i = 0; i < TTX_MAX_HANDLE; i++)
    {
        if (0 != s_ahTTX[i])
        {
            TTX_MUTEX_LOCK(&s_mutex_interface);

            HI_HANDLE hTtx = TTX_GetHandleByIndex(i);

            if (HI_SUCCESS != TTX_Destroy(hTtx))
            {
                HI_ERR_TTX("TTX_Destroy failure!\n");
            }

            TTX_MUTEX_UNLOCK(&s_mutex_interface);
        }
    }

    if (s_bIsInitTTX)
    {
        if (0 != pthread_mutex_destroy(&s_mutex_interface))
        {
            return HI_FAILURE;
        }

        s_bIsInitTTX = HI_FALSE;
    }

    return HI_SUCCESS;
}

/** Create a  context  and return a handle  */
HI_S32 HI_UNF_TTX_Create(HI_UNF_TTX_INIT_PARA_S* pstInitParam, HI_HANDLE* phTTX)
{
    HI_U16 u16Index = 0;
    HI_S32 s32Ret = HI_FAILURE;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint  = HI_NULL;
    HI_UNF_TTX_BUFFER_PARAM_S stBuffParam;

    if (!s_bIsInitTTX)
    {
        HI_ERR_TTX("InitParam error!\n");
        return HI_FAILURE;
    }

    CHECK_PARAM(pstInitParam);
    CHECK_PARAM(pstInitParam->pfnCB);

    memset(&stBuffParam, 0x0, sizeof(HI_UNF_TTX_BUFFER_PARAM_S));

    if (HI_SUCCESS != TTX_GetFreeIndex(&u16Index))
    {
        HI_ERR_TTX("TTX_GetFreeIndex failure...\n");

        return HI_FAILURE;
    }


    if (((HI_NULL == pstInitParam->pu8MemAddr) && (0 != pstInitParam->u32MemSize)))
    {
        return HI_FAILURE;
    }

    if (((pstInitParam->bFlash != HI_FALSE) && (pstInitParam->bFlash != HI_TRUE))
        || ((pstInitParam->bNavigation != HI_FALSE) && (pstInitParam->bNavigation != HI_TRUE)))

    {
        return HI_FAILURE;
    }

    s32Ret = TTX_Mem_Init(pstInitParam->pu8MemAddr, pstInitParam->u32MemSize);

    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_TTX("TTX_Mem_Init error!\n");
        goto MEM_INIT_ERROR;
    }

    s32Ret = TTX_Data_Init();

    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_TTX("TTX_Data_Init error!\n");
        goto DATA_INIT_ERROR;
    }

    pstParam = (TTX_PARAM_S*)TTX_Mem_Malloc(sizeof(TTX_PARAM_S));

    if (HI_NULL == pstParam)
    {
        HI_ERR_TTX("TTX_Malloc error!\n");
        goto MALLOC_ERROR;
    }

    TTX_Mem_Memset(pstParam, 0, sizeof(TTX_PARAM_S));

    s32Ret = TTX_Recv_Create(&pstParam->hDataRecv);

    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_TTX("TTX_Init error!\n");
        goto RECV_CREATE_ERROR;
    }

    /** Init page context  */
    s32Ret = TTX_PageContext_Init(&pstParam->hDataParse);

    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_TTX("TTX_PageContext_Init error!\n");
        goto PAGECONTEXT_INIT_ERROR;
    }

    /** Create page context, start thread  */
    s32Ret = TTX_PageContext_Create(pstParam->hDataParse);

    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_TTX("TTX_PageContext_Create error!\n");
        goto PAGECONTEXT_CREATE_ERROR;
    }

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;
    CHECK_PARAM(pstCurrentPoint);

    if (HI_NULL != pstInitParam->pfnCB)     /** Register  callback function  */
    {
        pstCurrentPoint->pfnCallBackFunction = pstInitParam->pfnCB;
    }

    pstCurrentPoint->bFlash = pstInitParam->bFlash;
    pstCurrentPoint->bNavigation = pstInitParam->bNavigation;
    pstCurrentPoint->hTTX = TTX_GetHandleByIndex(u16Index);


    if (pstInitParam->bNavigation)
    {
        stBuffParam.u32Row = TTX_ROW_NUM + TTX_NAVIGATION_BAR + TTX_SUBPAGE_BAR;
    }
    else
    {
        stBuffParam.u32Row = TTX_ROW_NUM + TTX_SUBPAGE_BAR;
    }

    stBuffParam.u32Column = TTX_LOP_COL_NUM;

    if (HI_SUCCESS != TTX_Show_CallBack(pstCurrentPoint, HI_UNF_TTX_CB_CREATE_BUFF, &stBuffParam))
    {
        HI_ERR_TTX("TTX_Show_CallBack error!\n");
        goto CALLBACK_ERROR;
    }

    pstCurrentPoint->u8Language[0] = 'd';
    pstCurrentPoint->u8Language[1] = 'e';
    pstCurrentPoint->u8Language[2] = 'f';
    pstCurrentPoint->u8Language[3] = '\0';

    TTX_MUTEX_LOCK(&s_mutex_interface);

    s_ahTTX[u16Index] = pstParam;
    *phTTX = pstCurrentPoint->hTTX;

    TTX_MUTEX_UNLOCK(&s_mutex_interface);
    return HI_SUCCESS;

CALLBACK_ERROR:

    if (HI_SUCCESS != TTX_PageContext_Destroy((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse))
    {
        HI_ERR_TTX("TTX_PageContext_Destroy error!\n");
    }

PAGECONTEXT_CREATE_ERROR:

    if (HI_SUCCESS != TTX_PageContext_DeInit((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse))
    {
        HI_ERR_TTX("TTX_PageContext_DeInit error!\n");
    }

PAGECONTEXT_INIT_ERROR:

    if (HI_SUCCESS != TTX_Recv_Destroy(pstParam->hDataRecv))
    {
        HI_ERR_TTX("TTX_Recv_Destroy error!\n");
    }

RECV_CREATE_ERROR:
    memset(pstParam, 0, sizeof(TTX_PARAM_S));
    TTX_Mem_Free(pstParam);
    pstParam = HI_NULL;
MALLOC_ERROR:

    if (HI_FAILURE == TTX_Data_DeInit())
    {
        HI_ERR_TTX("TTX_Data_DeInit error!\n");
    }

DATA_INIT_ERROR:

    if (HI_FAILURE == TTX_Mem_DeInit())
    {
        HI_ERR_TTX("TTX_Mem_DeInit error!\n");
    }

MEM_INIT_ERROR:
    return HI_FAILURE;
}



/** Destory  a context */
HI_S32 HI_UNF_TTX_Destroy(HI_HANDLE hTTX)
{
    HI_S32 s32Ret = HI_FAILURE;

    CHECK_PARAM(hTTX);

    TTX_MUTEX_LOCK(&s_mutex_interface);
    s32Ret = TTX_Destroy(hTTX);
    TTX_MUTEX_UNLOCK(&s_mutex_interface);
    
    return s32Ret;
}

/** Switch  teletext , teletext  subtitle and other information  */

HI_S32 HI_UNF_TTX_SwitchContent (HI_HANDLE hTTX, HI_UNF_TTX_CONTENT_PARA_S* pstContentParam)
{
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S*   pstCurrentPoint = NULL;
    HI_U32 i = 0;

    CHECK_PARAM(hTTX);
    CHECK_PARAM(pstContentParam);

    if (pstContentParam->enType >= HI_UNF_TTX_ADDINFO)
    {
        HI_WARN_TTX("Non-support!\n");
        return HI_FAILURE;
    }

    if ((pstContentParam->stInitPgAddr.u8MagazineNum > 8) || \
        ((pstContentParam->stInitPgAddr.u8PageNum > 99) && (0xff != pstContentParam->stInitPgAddr.u8PageNum)))
    {
        HI_ERR_TTX("invalid args!\n");
        return HI_FAILURE;
    }

    if (0xff != pstContentParam->stInitPgAddr.u8PageNum)
    {
        TTX_DEC_TO_HEX(pstContentParam->stInitPgAddr.u8PageNum);
    }

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (!pstCurrentPoint)
    {
        HI_ERR_TTX("The hTTX is  invalid!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < 8; i++)
    {
        g_bHasM29_0[i] = HI_FALSE;
        g_bHasM29_1[i] = HI_FALSE;
        g_bHasM29_4[i] = HI_FALSE;
    }

    pstCurrentPoint->u8Language[0] = (pstContentParam->u32ISO639LanCode >> 16 ) & 0xff;
    pstCurrentPoint->u8Language[1] = (pstContentParam->u32ISO639LanCode >> 8 ) & 0xff;
    pstCurrentPoint->u8Language[2] = pstContentParam->u32ISO639LanCode & 0xff;
    pstCurrentPoint->u8Language[3] = '\0';

    if ((pstContentParam->stInitPgAddr.u8MagazineNum == 0xff) || (pstContentParam->stInitPgAddr.u8PageNum == 0xff))
    {
        if (HI_SUCCESS
            != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stWantPgAddr), TTX_DEFAULT_INITMAGAZINE, TTX_DEFAULT_INITPAGE,
                                   TTX_DEDAULT_INITSUBPAGE ))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        if (HI_SUCCESS
            != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stIndexPgAddr), TTX_DEFAULT_INITMAGAZINE, TTX_DEFAULT_INITPAGE,
                                   TTX_DEDAULT_INITSUBPAGE ))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }
    }
    else
    {
        if (HI_SUCCESS
            != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stWantPgAddr), pstContentParam->stInitPgAddr.u8MagazineNum % 8,
                                   pstContentParam->stInitPgAddr.u8PageNum % 256, 0))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }

        if (HI_SUCCESS
            != TTX_Parse_SetPgAddr(&(pstCurrentPoint->stIndexPgAddr), pstContentParam->stInitPgAddr.u8MagazineNum % 8,
                                   pstContentParam->stInitPgAddr.u8PageNum % 256, 0))
        {
            HI_WARN_TTX("TTX_Parse_SetPgAddr failure!\n");
        }
    }

    switch (pstContentParam->enType)
    {
            /**  Teletext  */
        case HI_UNF_TTX_INITTTX:
        {
            if (HI_SUCCESS != TTX_InitSubQueue(pstCurrentPoint, HI_FALSE))
            {
                HI_ERR_TTX("TTX_InitSubQueue failure!\n");
                return HI_FAILURE;
            }

            pstCurrentPoint->bSubtitle = HI_FALSE;

            if (HI_SUCCESS != TTX_Recv_SetInitpage(pstParam->hDataRecv, pstContentParam->stInitPgAddr.u8MagazineNum,
                                                   pstContentParam->stInitPgAddr.u8PageNum, HI_FALSE))
            {
                HI_WARN_TTX("TTX_Recv_SetInitpage failure!\n");
            }

            if ((pstCurrentPoint->stWantPgAddr.u8MagazineNum > 8) || \
                ((pstCurrentPoint->stWantPgAddr.u8PageNum >> 4) > 9) || \
                ((pstCurrentPoint->stWantPgAddr.u8PageNum & 0xf) > 9))
            {
                HI_ERR_TTX("invalid page num!\n");
                return HI_FAILURE;
            }

            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stWantPgAddr))
            {
                TTX_ShowPromptMsg(pstCurrentPoint);
            }
        }
        break;

        /** Teletext subtitle */
        case HI_UNF_TTX_TTXSUBT:
        {
            pstCurrentPoint->bSubtitle = HI_TRUE;

            if (HI_SUCCESS != TTX_Recv_SetInitpage(pstParam->hDataRecv, pstContentParam->stInitPgAddr.u8MagazineNum,
                                                   pstContentParam->stInitPgAddr.u8PageNum, HI_TRUE))
            {
                HI_WARN_TTX("TTX_Recv_SetInitpage failure!\n");
            }


            if (HI_SUCCESS != TTX_InitSubQueue(pstCurrentPoint, HI_TRUE))
            {
                HI_ERR_TTX("TTX_InitSubQueue failure!\n");
                return HI_FAILURE;
            }


            if (HI_SUCCESS != TTX_RequestPage(hTTX, pstCurrentPoint->stWantPgAddr))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }
        }
        break;

        /** Other information */
        case HI_UNF_TTX_ADDINFO:

            break;

        case HI_UNF_TTX_PROGSCD:

            break;

        case HI_UNF_TTX_TTXSUBT_HIP:

            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TTX_Display(HI_HANDLE hTTX, HI_HANDLE hDispalyHandle)
{
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;
    TTX_MSG_ACTION_E enMsgAction = TTX_MSG_ACTION_BUTT;
    HI_UNF_TTX_PAGE_ADDR_S stPageAddr;


    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    memset(&stPageAddr, 0x0, sizeof(HI_UNF_TTX_PAGE_ADDR_S));

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;
    CHECK_PARAM(pstCurrentPoint);

    if ((hDispalyHandle & 0xffff0000) != 0x7f7f0000)
    {
        HI_INFO_TTX("Invalid display handle");
        return HI_FAILURE;
    }

    enMsgAction = (TTX_MSG_ACTION_E)(hDispalyHandle & 0xffff);

    switch (enMsgAction)
    {
        case TTX_MSG_ACTION_SHOWPAGE:
            TTX_Show_ShowPage(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_UPDATETIME:
            TTX_Parse_UpdateTime(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_FLASH:
            TTX_Parse_Flash(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_INPUTNUMBER:
            if (pstCurrentPoint->bNavigation && pstCurrentPoint->bSubpageMode)
            {
                TTX_Show_DrawNavigationSubpageMode(pstCurrentPoint, TTX_LOP_COL_NUM);
            }
            else
            {
                TTX_Parse_InputNumber(pstCurrentPoint);
            }

            break;

        case TTX_MSG_ACTION_INVALIDREQUEST:
            TTX_Parse_InvalidRequest(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_UPDATESUBPAGE:
            TTX_Parse_UpdateSubpage(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_SHOWPROMPT_SEARCH:
            TTX_Show_ShowPrompt(pstCurrentPoint);
            break;

        case TTX_MSG_ACTION_SHOWPROMPT_SEARCH_TIMEOUT:
            if (HI_SUCCESS == TTX_Data_FindFirstValidPage(&stPageAddr.u8MagazineNum, &stPageAddr.u8PageNum, &stPageAddr.u16PageSubCode))
            {
                (HI_VOID)TTX_RequestPage(hTTX, stPageAddr);
            }
            else
            {
                TTX_Show_ShowPromptTimeOut(pstCurrentPoint);
            }

            break;

        case TTX_MSG_ACTION_OSDOUTPUT_ON:
            if (HI_SUCCESS != TTX_Show_SetOSDOutput_ON(pstCurrentPoint))
            {
                HI_ERR_TTX("TTX_Show_SetOSDOutput_ON error!\n");
            }

            break;

        case TTX_MSG_ACTION_OSDOUTPUT_OFF:
            if (HI_SUCCESS != TTX_Show_SetOSDOutput_OFF(pstCurrentPoint))
            {
                HI_ERR_TTX("TTX_Show_SetOSDOutput_OFF error!\n");
            }

            break;

        case TTX_MSG_ACTION_BUTT:
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}

/** Set the output attribution */
HI_S32 HI_UNF_TTX_Output (HI_HANDLE hTTX, HI_UNF_TTX_OUTPUT_E enOutput, HI_BOOL bEnable)
{
    HI_S32 s32Ret = HI_FAILURE;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = HI_NULL;

    if ((HI_NULL == hTTX) || ((HI_TRUE != bEnable) && (HI_FALSE != bEnable)) || (enOutput >= HI_UNF_TTX_BUTT))
    {
        HI_ERR_TTX("Invalid param!\n");
        return HI_FAILURE;
    }

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    pstCurrentPoint->bInvalidReq = HI_FALSE;

    switch (enOutput)
    {
        case HI_UNF_TTX_VBI_OUTPUT:        /** VBI  output*/
        {
            s32Ret = TTX_Recv_SetOutputType(pstParam->hDataRecv, TTX_VBI, bEnable);

            if (HI_FAILURE == s32Ret)
            {
                HI_ERR_TTX("TTX_Set_Output_Type error!\n");
                return HI_FAILURE;
            }
        }
        break;

        case HI_UNF_TTX_OSD_OUTPUT:       /** OSD output */
        {
            s32Ret = TTX_Recv_SetOutputType(pstParam->hDataRecv, TTX_OSD, bEnable);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TTX("TTX_Set_Output_Type error!\n");
                return HI_FAILURE;
            }

            s32Ret = TTX_Show_SetOSDOutputMsg((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse, bEnable);

            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_TTX("TTX_Show_SetOSDOutputMsg error!\n");
                return HI_FAILURE;
            }
        }
        break;

        case HI_UNF_TTX_DUAL_OUTPUT:       /** Both OSD and VBI output */
        {
            s32Ret = TTX_Recv_SetOutputType(pstParam->hDataRecv, TTX_OSD_VBI, bEnable);

            if (HI_FAILURE == s32Ret)
            {
                HI_ERR_TTX("TTX_Set_Output_Type error!\n");
                return HI_FAILURE;
            }

            s32Ret = TTX_Show_SetOSDOutputMsg((TTX_PAGE_CONTEXT_S*)pstParam->hDataParse, bEnable);           /**shut sown OSD output*/

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TTX("TTX_Show_SetOSDOutputMsg error!\n");
                return HI_FAILURE;
            }
        }
        break;

        default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/** Exce  user's input */
HI_S32 HI_UNF_TTX_ExecCmd(HI_HANDLE hTTX, HI_UNF_TTX_CMD_E enCMD, HI_VOID* pvCMDParam)
{
    HI_U8 i = 0;
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstCurrentPoint = NULL;
    HI_UNF_TTX_KEY_E enKey = HI_UNF_TTX_KEY_BUTT;
    HI_UNF_TTX_PAGE_ADDR_S* pstOpenpage = HI_NULL;
    HI_UNF_TTX_GETPAGEADDR_S* pstGetpageaddr = HI_NULL;

    CHECK_PARAM(hTTX);

    if ((enCMD >= HI_UNF_TTX_CMD_BUTT) || (pvCMDParam == HI_NULL))
    {
        return HI_FAILURE;
    }


    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstCurrentPoint = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;

    if (HI_NULL == pstCurrentPoint)
    {
        HI_ERR_TTX("pstCurrentPoint is null!\n");
        return HI_FAILURE;
    }

    switch (enCMD)
    {
            /** remote control */
        case HI_UNF_TTX_CMD_KEY:
        {

            enKey = *(HI_UNF_TTX_KEY_E*)pvCMDParam;

            /*when is subtitle, not process remote key*/
            if ((HI_TRUE == pstCurrentPoint->bSubtitle) && (HI_UNF_TTX_KEY_MIX != enKey))
            {
                break;
            }

            TTX_ExecKeyCmd(hTTX, pstCurrentPoint, enKey);
        }

        break;

        case HI_UNF_TTX_CMD_OPENPAGE:           /** Open specify page */
        {
            pstOpenpage = (HI_UNF_TTX_PAGE_ADDR_S*)pvCMDParam;

            if ((pstOpenpage->u8MagazineNum > 8) || (pstOpenpage->u8PageNum >= 100))
            {
                return HI_FAILURE;
            }

            pstOpenpage->u8MagazineNum %= 8;

            TTX_DEC_TO_HEX(pstOpenpage->u8PageNum);

            if (HI_SUCCESS != TTX_RequestPage(hTTX, *pstOpenpage))
            {
                HI_WARN_TTX("TTX_RequestPage failure!\n");
            }
        }
        break;

        case HI_UNF_TTX_CMD_GETPAGEADDR:           /** Get page address */
        {
            pstGetpageaddr = (HI_UNF_TTX_GETPAGEADDR_S*)pvCMDParam;

            if (pstGetpageaddr->enPageType >= HI_UNF_TTX_PAGE_BUTT)
            {
                return HI_FAILURE;
            }

            TTX_MUTEX_LOCK(&(pstCurrentPoint->Mutexlock));

            switch (pstGetpageaddr->enPageType)
            {
                case HI_UNF_TTX_PAGE_CUR:                /**<Current displaying page */
                {
                    pstGetpageaddr->stPageAddr = pstCurrentPoint->stCurPgAddr;
                }
                break;

                case  HI_UNF_TTX_PAGE_INDEX:              /**<Initial Teletext page   */
                {
                    pstGetpageaddr->stPageAddr = pstCurrentPoint->stIndexPgAddr;

                    if (HI_SUCCESS != TTX_Data_GetValidPage(&pstGetpageaddr->stPageAddr.u8MagazineNum,
                                                            &pstGetpageaddr->stPageAddr.u8PageNum,
                                                            &pstGetpageaddr->stPageAddr.u16PageSubCode, TTX_INDEX_PAGE))
                    {
                        HI_WARN_TTX("TTX_Data_GetValidPage failure!\n");
                    }
                }
                break;

                case HI_UNF_TTX_PAGE_LINK1:               /** First link in packet 27 */
                case HI_UNF_TTX_PAGE_LINK2:              /** Second  link in packet 27 */
                case HI_UNF_TTX_PAGE_LINK3:              /** Third link in packet 27 */
                case HI_UNF_TTX_PAGE_LINK4:              /** Fourth link in packet 27 */
                case HI_UNF_TTX_PAGE_LINK5:              /** Fifth link in packet 27 */
                case HI_UNF_TTX_PAGE_LINK6:              /** Sixth link in packet 27 */
                {
                    i = (HI_U8)((HI_U8)(pstGetpageaddr->enPageType) - (HI_U8)(HI_UNF_TTX_PAGE_LINK1));

                    pstGetpageaddr->stPageAddr = pstCurrentPoint->astLink27[i];
                }
                break;

                default:
                    break;
            }

            TTX_MUTEX_UNLOCK(&(pstCurrentPoint->Mutexlock));
        }
        break;

        case HI_UNF_TTX_CMD_CHECKPAGE:
        {
            if (HI_SUCCESS != TTX_CheckPage(hTTX, (HI_UNF_TTX_CHECK_PARAM_S*) pvCMDParam))
            {
                HI_WARN_TTX("TTX_CheckPage failure!\n");
            }

            break;
        }

        case HI_UNF_TTX_CMD_SETREQUEST:
        {
            HI_UNF_TTX_REQUEST_RAWDATA_S* pstRawData = (HI_UNF_TTX_REQUEST_RAWDATA_S*)pvCMDParam;

            if (HI_SUCCESS != TTX_RequestRawData(hTTX, pstRawData))
            {
                HI_WARN_TTX("TTX_RequestRawData failure!\n");
            }

            break;
        }

        case HI_UNF_TTX_CMD_CLEARREQUEST:
        {
            if (HI_SUCCESS != TTX_Recv_ClearRequest(pstParam->hDataRecv))
            {
                HI_WARN_TTX("TTX_Recv_ClearRequest failure!\n");
            }

            break;
        }

        default:
            break;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_TTX_InjectData(HI_HANDLE hTTX, HI_U8* pu8Data, HI_U32 u32DataSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    TTX_MUTEX_LOCK(&s_mutex_interface);
    
    s32Ret = TTX_InjectData(hTTX, pu8Data, u32DataSize);
    
    TTX_MUTEX_UNLOCK(&s_mutex_interface);

    return s32Ret;
}

HI_S32 HI_UNF_TTX_ResetData(HI_HANDLE hTTX)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 i = 0;
    TTX_PAGE_CONTEXT_S* pstContext = HI_NULL;
    TTX_PARAM_S* pstParam = HI_NULL;

    CHECK_PARAM(hTTX);

    pstParam = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstContext = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;
    CHECK_PARAM(pstContext);

    TTX_MUTEX_LOCK(&s_mutex_interface);

    for (i = 0; i < 8; i++)
    {
        g_bHasM29_0[i] = HI_FALSE;
        g_bHasM29_1[i] = HI_FALSE;
        g_bHasM29_4[i] = HI_FALSE;
    }

    pstContext->bTTXSearching = HI_FALSE;
    TTX_MUTEX_LOCK(&(pstContext->Mutexlock));

    if (HI_FAILURE == TTX_Data_Reset())
    {
        s32Ret = HI_FAILURE;
    }

    if (HI_FAILURE
        == TTX_PesQueue_Reset(pstContext->pstPAGEQueue, pstContext->pu8PgQueueMemory, pstContext->u32QueueSize))
    {
        s32Ret = HI_FAILURE;
    }

    TTX_MUTEX_UNLOCK(&(pstContext->Mutexlock));
    TTX_MUTEX_UNLOCK(&s_mutex_interface);

    return s32Ret;
}

HI_S32 HI_UNF_TTX_SetLanguage(HI_HANDLE hTTX, HI_U8* pu8Language)
{
    TTX_PAGE_CONTEXT_S* pstContext = HI_NULL;
    TTX_PARAM_S* pstParam = HI_NULL;

    CHECK_PARAM(hTTX);
    CHECK_PARAM(pu8Language);

    pstParam  = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstContext = (TTX_PAGE_CONTEXT_S*)pstParam->hDataParse;
    CHECK_PARAM(pstContext);

    pstContext->u8Language[0] = pu8Language[0];
    pstContext->u8Language[1] = pu8Language[1];
    pstContext->u8Language[2] = pu8Language[2];
    
    return HI_SUCCESS;
}

HI_S32 HI_UNF_TTX_SetMaxInterval(HI_HANDLE hTTX, HI_U32 u32IntervalMs )
{
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_PAGE_CONTEXT_S* pstContext = HI_NULL;

    CHECK_PARAM(hTTX);

    pstParam  = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstContext = (TTX_PAGE_CONTEXT_S*)(pstParam->hDataParse);

    if (HI_NULL != pstContext)
    {
        pstContext->u32MaxIntervalMs = u32IntervalMs;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 HI_UNF_TTX_SetPacketType (HI_HANDLE hTTX, HI_UNF_TTX_PACKET_TYPE_E enPacketType)
{
    TTX_PARAM_S* pstParam = HI_NULL;
    TTX_CONTEXT_S_PTR pstThisElem = HI_NULL;
    CHECK_PARAM(hTTX);

    pstParam  = (TTX_PARAM_S*)TTX_GetParamAddr(hTTX);
    CHECK_PARAM(pstParam);

    pstThisElem = (TTX_CONTEXT_S_PTR)pstParam->hDataRecv;

    if (HI_NULL != pstThisElem)
    {
        pstThisElem->enPacketType = enPacketType;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}
