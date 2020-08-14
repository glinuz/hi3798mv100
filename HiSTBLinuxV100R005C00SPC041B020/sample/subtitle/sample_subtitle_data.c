#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_unf_demux.h"
#include "hi_unf_so.h"
#include "hi_unf_subt.h"

#include "hi_filter.h"
#include "sample_subtitle_data.h"

#define HI_FATAL_SUBT(fmt...)      HI_FATAL_PRINT(HI_ID_SUBT, fmt)
#define HI_ERR_SUBT(fmt...)        HI_ERR_PRINT(HI_ID_SUBT, fmt)
#define HI_WARN_SUBT(fmt...)       HI_WARN_PRINT(HI_ID_SUBT, fmt)
#define HI_INFO_SUBT(fmt...)       HI_INFO_PRINT(HI_ID_SUBT, fmt)

#define SUBT_STREAM_KEYWORD (0xBD)
#define SCTE_STREAM_KEYWORD (0xC6)
#define MAX_CHANNEL_BUF_SIZE (64 * 1024)
#define DATA_RECV_MAX_NUM (32)


#define SAMPLE_SUBT_HANDLE_BASE  ((HI_ID_SUBT << 16) & 0xFFFF0000)


static HI_U16 GetIndexByHandle(HI_HANDLE hSubtData)
{
    HI_U16 u16HandleIndex = hSubtData & 0x0000FFFF;

    return u16HandleIndex;
}

static HI_HANDLE GetHandleByIndex(HI_U16 u16HandleIndex)
{
    HI_HANDLE hSubtData = SAMPLE_SUBT_HANDLE_BASE | u16HandleIndex;

    return hSubtData;
}

#ifdef SUBTITLE_USED_HI_FILTER
typedef struct tagSUBT_DATA_S
{
    HI_BOOL bEnable;
    SUBT_DATA_INSTALL_PARAM_S stInstallParam;
    HI_S32  s32FilterID;
}SUBT_DATA_S;

static SUBT_DATA_S s_astSubtDataRecv[DATA_RECV_MAX_NUM];


static SUBT_DATA_S* FindSubtData(HI_S32 s32Filterid)
{
    HI_U8 i = 0;
    SUBT_DATA_S *pstSubtData = NULL;

    for (i = 0; i < DATA_RECV_MAX_NUM; i++)
    {
        pstSubtData = &s_astSubtDataRecv[i];

        if ((HI_TRUE == pstSubtData->bEnable) && (s32Filterid == pstSubtData->s32FilterID))
        {
            return pstSubtData;
        }
    }

    return (SUBT_DATA_S*)NULL;
}

static HI_S32 FilterDataCallBack(HI_S32 s32Filterid, HI_FILTER_CALLBACK_TYPE_E enCallbackType,
                   HI_UNF_DMX_DATA_TYPE_E eDataType, HI_U8 *pu8Buffer, HI_U32 u32BufferLength)
{
    if (HI_FILTER_CALLBACK_TYPE_TIMEOUT != enCallbackType
        && pu8Buffer
        && u32BufferLength)
    {
        SUBT_DATA_S *pstSubtData = FindSubtData(s32Filterid);

        if (pstSubtData)
        {
            (HI_VOID)pstSubtData->stInstallParam.pfnCallback(pstSubtData->stInstallParam.u32UserData, pu8Buffer, u32BufferLength);
        }

    }
    else if(HI_FILTER_CALLBACK_TYPE_TIMEOUT == enCallbackType)
    {

    }

    return 0;
}

HI_S32 SUBT_Data_Init()
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_FILTER_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Init !\n");

        return HI_FAILURE;
    }

    memset(s_astSubtDataRecv, 0, sizeof(s_astSubtDataRecv));

    return HI_SUCCESS;
}

HI_S32 SUBT_Data_DeInit()
{
    HI_U8 i = 0;

    for (i = 0; i < DATA_RECV_MAX_NUM; i++)
    {
        (HI_VOID)SUBT_Data_Uninstall(GetHandleByIndex(i));
    }

    memset(s_astSubtDataRecv, 0, sizeof(s_astSubtDataRecv));

    return HI_SUCCESS;
}

HI_S32 SUBT_Data_Install(SUBT_DATA_INSTALL_PARAM_S *pstInstallParam, HI_HANDLE *hData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_FILTER_ATTR_S stFilterAttr;
    HI_S32 s32FilterID;
    SUBT_DATA_S *pstSubtData = NULL;
    HI_U8 i = 0;

    if (NULL == pstInstallParam || NULL == hData)
    {
        HI_ERR_SUBT("parameter is invalid...\n");

        return HI_FAILURE;
    }

    for (i = 0; i < DATA_RECV_MAX_NUM; i++)
    {
        pstSubtData = &s_astSubtDataRecv[i];

        if (HI_FALSE == pstSubtData->bEnable)
        {
            break;
        }
    }
    if (i >= DATA_RECV_MAX_NUM)
    {
        HI_ERR_SUBT("install too much, max is %d !\n", DATA_RECV_MAX_NUM);

        return HI_FAILURE;
    }

    stFilterAttr.u32DMXID = pstInstallParam->u32DmxID;
    stFilterAttr.funCallback = FilterDataCallBack;
    stFilterAttr.u32DirTransFlag = 1;
    stFilterAttr.u32PID = pstInstallParam->u16SubtPID;
    stFilterAttr.u32TimeOutMs = 2000;
    memset(stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
    memset(stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
    stFilterAttr.u32FilterDepth = 1;

    switch(pstInstallParam->enDataType)
    {
        case HI_UNF_SUBT_SCTE:
            stFilterAttr.u32FilterType = 0;//section
            stFilterAttr.u32CrcFlag = 2;
            stFilterAttr.u8Match[0] = SCTE_STREAM_KEYWORD;
            break;
        case HI_UNF_SUBT_DVB:
        default:
            stFilterAttr.u32FilterType = 1;//PES
            stFilterAttr.u32CrcFlag = 0;
            stFilterAttr.u8Match[0] = SUBT_STREAM_KEYWORD;
            break;
    }

    s32Ret = HI_FILTER_Creat(&stFilterAttr,&s32FilterID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Creat !\n");

        return HI_FAILURE;
    }
    s32Ret = HI_FILTER_Start(s32FilterID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Start !\n");

        return HI_FAILURE;
    }

    pstSubtData->stInstallParam = *pstInstallParam;
    pstSubtData->s32FilterID = s32FilterID;
    pstSubtData->bEnable = HI_TRUE;

    *hData = GetHandleByIndex(i);

    return HI_SUCCESS;
}

HI_S32 SUBT_Data_Uninstall(HI_HANDLE hData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SUBT_DATA_S *pstSubtData = NULL;
    HI_S32 s32FilterID = pstSubtData->s32FilterID;


    HI_U16 u16Index = GetIndexByHandle(hData);

    if (u16Index >= DATA_RECV_MAX_NUM)
    {
        HI_ERR_SUBT("install too much, max is %d !\n", DATA_RECV_MAX_NUM);

        return HI_FAILURE;
    }

    pstSubtData = &s_astSubtDataRecv[u16Index];


    s32Ret = HI_FILTER_Stop(s32FilterID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Stop !\n");

        return HI_FAILURE;
    }

    s32Ret = HI_FILTER_Destroy(s32FilterID);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Destroy !\n");

        return HI_FAILURE;
    }

    pstSubtData->s32FilterID = 0;
    pstSubtData->bEnable = HI_FALSE;

    return HI_SUCCESS;
}

#else

typedef struct tagSUBT_DATA_S
{
    HI_BOOL bEnable;
    SUBT_DATA_INSTALL_PARAM_S stInstallParam;
    HI_HANDLE hChannelID;
    HI_HANDLE hFilterID;
}SUBT_DATA_S;

static SUBT_DATA_S     s_astSubtDataRecv[DATA_RECV_MAX_NUM];
static pthread_t       s_stSubtDataThreadID;
static pthread_mutex_t s_stSubtDataMutex;
static HI_BOOL         s_bSubtDataThreadReady = HI_FALSE;
static HI_BOOL         s_bInit = HI_FALSE;

static void SUBT_Data_Thread(HI_VOID *args)
{
    HI_UNF_DMX_DATA_S stSection[5] = { {0} };
    HI_U32 u32SectionNum = 0;
    HI_U8 i = 0;
    SUBT_DATA_S *pstSubtData = NULL;

    while (HI_TRUE == s_bSubtDataThreadReady)
    {
        /* Get section data */
        for (i = 0; i < DATA_RECV_MAX_NUM; i++)
        {
            pthread_mutex_lock(&s_stSubtDataMutex);

            pstSubtData = &s_astSubtDataRecv[i];
            if (HI_FALSE == pstSubtData->bEnable)
            {
                pthread_mutex_unlock(&s_stSubtDataMutex);
                continue;
            }

            if (HI_SUCCESS != HI_UNF_DMX_AcquireBuf(pstSubtData->hChannelID, 1, &u32SectionNum, stSection, 0))
            {
                pthread_mutex_unlock(&s_stSubtDataMutex);
                continue;
            }

            if(u32SectionNum == 0)
            {
                pthread_mutex_unlock(&s_stSubtDataMutex);
                continue;
            }

            if (pstSubtData->stInstallParam.pfnCallback)
            {
                (HI_VOID)pstSubtData->stInstallParam.pfnCallback(pstSubtData->stInstallParam.u32UserData, stSection[0].pu8Data, stSection[0].u32Size);
            }

            (HI_VOID)HI_UNF_DMX_ReleaseBuf(pstSubtData->hChannelID, u32SectionNum, stSection);
            pthread_mutex_unlock(&s_stSubtDataMutex);
        }

        usleep(50*1000);
    }

    HI_INFO_SUBT("Data receive thread exit!!!\n");
}

HI_S32 SUBT_Data_Init(HI_VOID)
{
    if (HI_FALSE == s_bInit)
    {
        memset(s_astSubtDataRecv, 0, sizeof(s_astSubtDataRecv));

        (HI_VOID)pthread_mutex_init(&s_stSubtDataMutex, NULL);
        s_bSubtDataThreadReady = HI_TRUE;
        pthread_create(&s_stSubtDataThreadID, NULL, (void * (*)(void *))SUBT_Data_Thread, (void*)HI_NULL);
        s_bInit = HI_TRUE;
    }
    return HI_SUCCESS;
}

HI_S32 SUBT_Data_DeInit(HI_VOID)
{
    HI_U8 i = 0;

    if (HI_TRUE == s_bInit)
    {
        for (i = 0; i < DATA_RECV_MAX_NUM; i++)
        {
            (HI_VOID)SUBT_Data_Uninstall(GetHandleByIndex(i));
        }
        memset(s_astSubtDataRecv, 0, sizeof(s_astSubtDataRecv));

        s_bSubtDataThreadReady = HI_FALSE;
        pthread_join(s_stSubtDataThreadID, NULL);
        pthread_mutex_destroy(&s_stSubtDataMutex);
        s_bInit = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 SUBT_Data_Install(SUBT_DATA_INSTALL_PARAM_S *pstInstallParam, HI_HANDLE *hData)
{
    HI_UNF_DMX_CHAN_ATTR_S tChAttr = {0};
    HI_UNF_DMX_FILTER_ATTR_S tFilterAttr = {0};
    HI_S32 s32Ret = 0;
    HI_U8  i = 0;
    SUBT_DATA_S *pstSubtData = NULL;

    if (HI_FALSE == s_bInit)
    {
        HI_ERR_SUBT("not init...\n");

        return HI_FAILURE;
    }

    if (NULL == pstInstallParam || NULL == hData)
    {
        HI_ERR_SUBT("parameter is invalid...\n");

        return HI_FAILURE;
    }

    for (i = 0; i < DATA_RECV_MAX_NUM; i++)
    {
        pstSubtData = &s_astSubtDataRecv[i];

        if (HI_FALSE == pstSubtData->bEnable)
        {
            break;
        }
    }

    if (i >= DATA_RECV_MAX_NUM)
    {
        HI_ERR_SUBT("install too much, max is %d !\n", DATA_RECV_MAX_NUM);

        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&tChAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_SUBT("failed to HI_FILTER_Destroy !\n");
        return HI_FAILURE;
    }
     switch(pstInstallParam->enDataType)
    {
        case HI_UNF_SUBT_SCTE:
            tChAttr.u32BufSize = MAX_CHANNEL_BUF_SIZE;
            tChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
            tChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
            tChAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
            break;
        case HI_UNF_SUBT_DVB:
        default:
            tChAttr.u32BufSize = MAX_CHANNEL_BUF_SIZE;
            tChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
            tChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
            tChAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
            break;
    }

    s32Ret = HI_UNF_DMX_CreateChannel(pstInstallParam->u32DmxID, &tChAttr, &pstSubtData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_SUBT("failed to HI_UNF_DMX_CreateChannel !\n");
        return HI_FAILURE;
    }

    /* set channel PID for recving data */
    s32Ret = HI_UNF_DMX_SetChannelPID(pstSubtData->hChannelID, pstInstallParam->u16SubtPID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

        HI_ERR_SUBT("failed to HI_UNF_DMX_SetChannelPID !\n");
        return HI_FAILURE;
    }

    memset(tFilterAttr.au8Match, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Mask, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Negate, 0, DMX_FILTER_MAX_DEPTH);
    tFilterAttr.u32FilterDepth = 1;
    tFilterAttr.au8Match[0] = SUBT_STREAM_KEYWORD; /* Subtitle stream id is 0xbd */
    tFilterAttr.au8Mask[0] = 0xff;

    s32Ret = HI_UNF_DMX_CreateFilter(pstInstallParam->u32DmxID, &tFilterAttr, &pstSubtData->hFilterID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

        HI_ERR_SUBT("failed to HI_UNF_DMX_CreateFilter !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_SetFilterAttr(pstSubtData->hFilterID, &tFilterAttr);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstSubtData->hFilterID);

        HI_ERR_SUBT("failed to HI_UNF_DMX_SetFilterAttr !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_AttachFilter(pstSubtData->hFilterID, pstSubtData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstSubtData->hFilterID);

        HI_ERR_SUBT("failed to HI_UNF_DMX_AttachFilter !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(pstSubtData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DetachFilter(pstSubtData->hFilterID, pstSubtData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstSubtData->hFilterID);

        HI_ERR_SUBT("failed to HI_UNF_DMX_OpenChannel !\n");
        return HI_FAILURE;
    }

    pthread_mutex_lock(&s_stSubtDataMutex);
    pstSubtData->stInstallParam = *pstInstallParam;
    pstSubtData->bEnable = HI_TRUE;
    pthread_mutex_unlock(&s_stSubtDataMutex);

    *hData = GetHandleByIndex(i);

    return HI_SUCCESS;
}

HI_S32 SUBT_Data_Uninstall(HI_HANDLE hData)
{
    SUBT_DATA_S *pstSubtData = NULL;

    HI_U16 u16Index = GetIndexByHandle(hData);

    if (u16Index >= DATA_RECV_MAX_NUM)
    {
        HI_ERR_SUBT("install too much, max is %d !\n", DATA_RECV_MAX_NUM);

        return HI_FAILURE;
    }


    if (HI_FALSE == s_bInit)
    {
        HI_ERR_SUBT("not init...\n");

        return HI_FAILURE;
    }

    pstSubtData = &s_astSubtDataRecv[u16Index];

    if ( HI_NULL == pstSubtData )
    {
        return HI_FAILURE;
    }

    if (HI_FALSE == pstSubtData->bEnable)
    {
        return HI_SUCCESS;
    }

    (HI_VOID)HI_UNF_DMX_CloseChannel(pstSubtData->hChannelID);
    (HI_VOID)HI_UNF_DMX_DetachFilter(pstSubtData->hFilterID, pstSubtData->hChannelID);
    (HI_VOID)HI_UNF_DMX_DestroyFilter(pstSubtData->hFilterID);
    (HI_VOID)HI_UNF_DMX_DestroyChannel(pstSubtData->hChannelID);

    pthread_mutex_lock(&s_stSubtDataMutex);
    pstSubtData->hChannelID = 0;
    pstSubtData->hFilterID  = 0;
    pstSubtData->bEnable    = HI_FALSE;
    pthread_mutex_unlock(&s_stSubtDataMutex);

    return HI_SUCCESS;
}
#endif

