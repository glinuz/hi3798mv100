#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "cc_debug.h"
#include "ccdisp_api.h"
#include "aribcc.h"
#include "aribcc_common.h"
#include "aribcc_decode.h"
#include "aribcc_osd.h"
#include "aribcc_disp.h"
#include "aribcc_obj.h"


typedef struct tagARIBCC_LOCAL_PARAM_S
{
    HI_BOOL bEnable;
    HI_BOOL bStart;
    pthread_t u32DispThread;
    pthread_t u32CycleThread;
} ARIBCC_LOCAL_PARAM_S;


static HI_VOID * _AribCC_CycleTask(HI_VOID *param)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)param;

    while(pstARIBCC->bEnable)
    {
        if (pstARIBCC->bStart)
        {
            ARIBCC_Com_TimerCheck(ARIB_CAP_CAPTION);
            ARIBCC_DEC_Cycle();
        }

        usleep(100 * 1000);
    }

    return HI_NULL;
}

static HI_VOID * _AribCC_DispTask(HI_VOID *param)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)param;

    while(pstARIBCC->bEnable)
    {
        if (pstARIBCC->bStart)
        {
            (HI_VOID)ARIBCC_DISP_Cycle(ARIB_CAP_CAPTION);
        }

        usleep(10 * 1000);
    }

    return HI_NULL;
}

static HI_S32 _AribCC_StartThread(ARIBCC_LOCAL_PARAM_S *pstARIBCC)
{
    pstARIBCC->bEnable = HI_TRUE;

    if(0 == pstARIBCC->u32DispThread)
    {
        if(pthread_create(&pstARIBCC->u32DispThread, NULL, (void* (*)(void *))_AribCC_DispTask, (void*)pstARIBCC) != 0)
        {
            HI_ERR_CC("failed to create arib check time thread\n");
            return HI_FAILURE;
        }
    }

    if(0 == pstARIBCC->u32CycleThread)
    {
        if(pthread_create(&pstARIBCC->u32CycleThread, NULL, (void* (*)(void *))_AribCC_CycleTask, (void*)pstARIBCC) != 0)
        {
            HI_ERR_CC("failed to create arib display thread\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 _AribCC_StopThread(ARIBCC_LOCAL_PARAM_S *pstARIBCC)
{
    pstARIBCC->bEnable = HI_FALSE;

    if (pstARIBCC->u32DispThread)
    {
        (HI_VOID)pthread_join(pstARIBCC->u32DispThread, HI_NULL);
        pstARIBCC->u32DispThread = 0;
    }

    if (pstARIBCC->u32CycleThread)
    {
        (HI_VOID)pthread_join(pstARIBCC->u32CycleThread, HI_NULL);
        pstARIBCC->u32CycleThread = 0;
    }

    return HI_SUCCESS;
}


HI_S32 AribCC_Init(HI_VOID)
{
	ARIBCC_OSD_Init();
	return ARIBCC_Com_Init();
}

HI_S32 AribCC_DeInit(HI_VOID)
{
    ARIBCC_OSD_DeInit();
    return ARIBCC_Com_DeInit();
}

HI_VOID* AribCC_Create(HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfig)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = HI_NULL;

    pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)malloc(sizeof(ARIBCC_LOCAL_PARAM_S));
    if(HI_NULL == pstARIBCC)
    {
        HI_ERR_CC("Malloc error\n");
        return HI_NULL;
    }
    memset(pstARIBCC,0,sizeof(ARIBCC_LOCAL_PARAM_S));

    (HI_VOID)ARIBCC_OBJ_Create(pstAribConfig);
    (HI_VOID)ARIBCC_DEC_Create(ARIB_CAP_CAPTION);
    (HI_VOID)ARIBCC_DISP_Create(ARIB_CAP_CAPTION);

    return (HI_VOID*)pstARIBCC;
}

HI_S32 AribCC_Destroy(HI_VOID* hAribCC)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)hAribCC;

    if (pstARIBCC)
    {
        (HI_VOID)ARIBCC_DISP_Destroy(ARIB_CAP_CAPTION);
        (HI_VOID)_AribCC_StopThread(pstARIBCC);
        (HI_VOID)ARIBCC_DEC_Destroy(ARIB_CAP_CAPTION);
        (HI_VOID)free(pstARIBCC);
        pstARIBCC = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 AribCC_Start(HI_VOID* hAribCC)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)hAribCC;

    if (HI_NULL == pstARIBCC)
    {
        HI_ERR_CC("param is error\n");
        return HI_FAILURE;
    }

    pstARIBCC->bStart = HI_TRUE;

    (HI_VOID)ARIBCC_DEC_Start(ARIB_CAP_CAPTION);
    (HI_VOID)ARIBCC_DISP_Start(ARIB_CAP_CAPTION);
    (HI_VOID)_AribCC_StartThread(pstARIBCC);

    return HI_SUCCESS;
}

HI_S32 AribCC_Stop(HI_VOID* hAribCC)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)hAribCC;

    if (HI_NULL == pstARIBCC)
    {
        HI_ERR_CC("param is error\n");
        return HI_FAILURE;
    }

    pstARIBCC->bStart = HI_FALSE;

    (HI_VOID)ARIBCC_DEC_Stop(ARIB_CAP_CAPTION);
    (HI_VOID)ARIBCC_DISP_Stop(ARIB_CAP_CAPTION);

    return HI_SUCCESS;
}

HI_S32 AribCC_Reset(HI_VOID* hAribCC)
{
    (HI_VOID)ARIBCC_DEC_Reset(ARIB_CAP_CAPTION);
    (HI_VOID)ARIBCC_DISP_Reset(ARIB_CAP_CAPTION);
    return HI_SUCCESS;
}

HI_S32 AribCC_Config(HI_VOID* hAribCC, HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribConfigParam)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if(HI_NULL == pstAribConfigParam)
    {
        HI_ERR_CC("Null config pointer!\n");
        return HI_FAILURE;
    }

    return s32Ret;

}

HI_S32 AribCC_GetConfig(HI_VOID* hAribCC, HI_UNF_CC_ARIB_CONFIGPARAM_S *pstAribCCConfig)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();

    if(HI_NULL == pstAribCCConfig)
    {
        HI_ERR_CC("Null pointer!\n");
        return HI_FAILURE;
    }

    pstAribCCConfig->u32BufferSize = pstObject->u32PesBufSize;

    return s32Ret;
}

HI_S32 AribCC_GetCaptionInfo(HI_VOID* hAribCC, HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)hAribCC;

    if ((HI_NULL == pstARIBCC)||(HI_NULL == pstCCAribInfo))
    {
        HI_ERR_CC("Null pointer!\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == pstARIBCC->bStart)
    {
         (HI_VOID)ARIBCC_DEC_GetCaptionInfo(pstCCAribInfo);
    }

    return HI_SUCCESS;
}

HI_S32 AribCC_InjectPESData(HI_VOID* hAribCC, HI_U8 *pu8PesData, HI_U32 u32DataLen)
{
    ARIBCC_LOCAL_PARAM_S *pstARIBCC = (ARIBCC_LOCAL_PARAM_S *)hAribCC;

    if (HI_NULL == pstARIBCC)
    {
        HI_ERR_CC("param is error\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == pstARIBCC->bStart && pu8PesData && u32DataLen)
    {
        (HI_VOID)ARIBCC_DEC_DecodePesData(pu8PesData, u32DataLen);
    }

    return HI_SUCCESS;
}
