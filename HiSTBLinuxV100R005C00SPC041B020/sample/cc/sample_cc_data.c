#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_unf_demux.h"
#include "sample_cc_data.h"

#define HI_FATAL_CC(fmt...)      HI_FATAL_PRINT(HI_ID_CC, fmt)
#define HI_ERR_CC(fmt...)        HI_ERR_PRINT(HI_ID_CC, fmt)
#define HI_WARN_CC(fmt...)       HI_WARN_PRINT(HI_ID_CC, fmt)
#define HI_INFO_CC(fmt...)       HI_INFO_PRINT(HI_ID_CC, fmt)


#define CC_STREAM_KEYWORD (0xBD)
#define MAX_CHANNEL_BUF_SIZE (64 * 1024)

typedef struct tagCC_DATA_S
{
    HI_BOOL bEnable;
    CC_DATA_INSTALL_PARAM_S stInstallParam;
    HI_HANDLE hChannelID;
    HI_HANDLE hFilterID;
}CC_DATA_S;

static CC_DATA_S       s_stCCDataRecv;
static pthread_t       s_stCCDataThreadID;
static pthread_mutex_t s_stCCDataMutex;
static HI_BOOL         s_bCCDataThreadReady = HI_FALSE;
static HI_BOOL         s_bInit = HI_FALSE;

static void CC_Data_Thread(HI_VOID *args)
{
    HI_UNF_DMX_DATA_S stSection[5] = { {0} };
    HI_U32 u32SectionNum = 0;
    CC_DATA_S *pstCCData = NULL;

    while (HI_TRUE == s_bCCDataThreadReady)
    {
        /* Get section data */
		pthread_mutex_lock(&s_stCCDataMutex);

		pstCCData = &s_stCCDataRecv;
		if (HI_FALSE == pstCCData->bEnable)
		{
			pthread_mutex_unlock(&s_stCCDataMutex);
			continue;
		}

		if (HI_SUCCESS != HI_UNF_DMX_AcquireBuf(pstCCData->hChannelID, 1, &u32SectionNum, stSection, 0))
		{
			pthread_mutex_unlock(&s_stCCDataMutex);
			continue;
		}

		if(u32SectionNum == 0)
		{
			pthread_mutex_unlock(&s_stCCDataMutex);
			continue;
		}

		if (pstCCData->stInstallParam.pfnCallback)
		{
			(HI_VOID)pstCCData->stInstallParam.pfnCallback(pstCCData->stInstallParam.u32UserData, stSection[0].pu8Data, stSection[0].u32Size);
		}

		(HI_VOID)HI_UNF_DMX_ReleaseBuf(pstCCData->hChannelID, u32SectionNum, stSection);
		pthread_mutex_unlock(&s_stCCDataMutex);

        usleep(50*1000);

	}
	HI_INFO_CC("Data receive thread exit!!!\n");
}

static HI_VOID CC_Data_Filter_UnInstall(HI_VOID)
{
    CC_DATA_S *pstCCData = &s_stCCDataRecv;

    (HI_VOID)HI_UNF_DMX_CloseChannel(pstCCData->hChannelID);
    (HI_VOID)HI_UNF_DMX_DetachFilter(pstCCData->hFilterID, pstCCData->hChannelID);
    (HI_VOID)HI_UNF_DMX_DestroyFilter(pstCCData->hFilterID);
    (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

    pthread_mutex_lock(&s_stCCDataMutex);
    pstCCData->hChannelID = 0;
    pstCCData->hFilterID  = 0;
    pstCCData->bEnable    = HI_FALSE;
    pthread_mutex_unlock(&s_stCCDataMutex);
}

HI_S32 CC_Data_Init(HI_VOID)
{
    if (HI_FALSE == s_bInit)
    {
        memset(&s_stCCDataRecv, 0, sizeof(s_stCCDataRecv));

        (HI_VOID)pthread_mutex_init(&s_stCCDataMutex, NULL);
		
        s_bCCDataThreadReady = HI_TRUE;
        pthread_create(&s_stCCDataThreadID, NULL, (void * (*)(void *))CC_Data_Thread, (void*)HI_NULL);
        s_bInit = HI_TRUE;
    }
	
    return HI_SUCCESS;
}

HI_S32 CC_Data_DeInit(HI_VOID)
{

    if (HI_TRUE == s_bInit)
    {
		(HI_VOID)CC_Data_Uninstall();

		memset(&s_stCCDataRecv, 0, sizeof(s_stCCDataRecv));


		s_bCCDataThreadReady = HI_FALSE;
		pthread_join(s_stCCDataThreadID, NULL);
		pthread_mutex_destroy(&s_stCCDataMutex);
		s_bInit = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 CC_Data_Install(CC_DATA_INSTALL_PARAM_S *pstInstallParam)
{
    HI_UNF_DMX_CHAN_ATTR_S tChAttr = {0};
    HI_UNF_DMX_FILTER_ATTR_S tFilterAttr = {0};
    HI_S32 s32Ret = 0;
    CC_DATA_S *pstCCData = NULL;

    if (HI_FALSE == s_bInit)
    {
        HI_ERR_CC("not init...\n");

        return HI_FAILURE;
    }

    if (NULL == pstInstallParam)
    {
        HI_ERR_CC("parameter is invalid...\n");

        return HI_FAILURE;
    }

	pstCCData = &s_stCCDataRecv;

    if (HI_TRUE == pstCCData->bEnable)
    {
	    
		CC_Data_Filter_UnInstall();
    }
	

    s32Ret = HI_UNF_DMX_GetChannelDefaultAttr(&tChAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CC("failed to HI_FILTER_Destroy !\n");
        return HI_FAILURE;
    }

    tChAttr.u32BufSize = MAX_CHANNEL_BUF_SIZE;
    tChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
    tChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
    tChAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

    s32Ret = HI_UNF_DMX_CreateChannel(pstInstallParam->u32DmxID, &tChAttr, &pstCCData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to HI_UNF_DMX_CreateChannel !\n");
        return HI_FAILURE;
    }

    /* set channel PID for recving data */
    s32Ret = HI_UNF_DMX_SetChannelPID(pstCCData->hChannelID, pstInstallParam->u16CCPID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

        HI_ERR_CC("failed to HI_UNF_DMX_SetChannelPID !\n");
        return HI_FAILURE;
    }

    memset(tFilterAttr.au8Match, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Mask, 0, DMX_FILTER_MAX_DEPTH);
    memset(tFilterAttr.au8Negate, 0, DMX_FILTER_MAX_DEPTH);
    tFilterAttr.u32FilterDepth = 1;
    tFilterAttr.au8Match[0] = CC_STREAM_KEYWORD; /* Subtitle stream id is 0xbd */
    tFilterAttr.au8Mask[0] = 0xff;

    s32Ret = HI_UNF_DMX_CreateFilter(pstInstallParam->u32DmxID, &tFilterAttr, &pstCCData->hFilterID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

        HI_ERR_CC("failed to HI_UNF_DMX_CreateFilter !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_SetFilterAttr(pstCCData->hFilterID, &tFilterAttr);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstCCData->hFilterID);

        HI_ERR_CC("failed to HI_UNF_DMX_SetFilterAttr !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_AttachFilter(pstCCData->hFilterID, pstCCData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstCCData->hFilterID);

        HI_ERR_CC("failed to HI_UNF_DMX_AttachFilter !\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(pstCCData->hChannelID);
    if (s32Ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DetachFilter(pstCCData->hFilterID, pstCCData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyChannel(pstCCData->hChannelID);

        (HI_VOID)HI_UNF_DMX_DestroyFilter(pstCCData->hFilterID);

        HI_ERR_CC("failed to HI_UNF_DMX_OpenChannel !\n");
        return HI_FAILURE;
    }

    pthread_mutex_lock(&s_stCCDataMutex);
    pstCCData->stInstallParam = *pstInstallParam;
    pstCCData->bEnable = HI_TRUE;
    pthread_mutex_unlock(&s_stCCDataMutex);

    return HI_SUCCESS;
}

HI_S32 CC_Data_Uninstall()
{
    if (HI_FALSE == s_bInit)
    {
        HI_ERR_CC("not init...\n");

        return HI_FAILURE;
    }

	CC_Data_Filter_UnInstall();
	
    return HI_SUCCESS;
}

