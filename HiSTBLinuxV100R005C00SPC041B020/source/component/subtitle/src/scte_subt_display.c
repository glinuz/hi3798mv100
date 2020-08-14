#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hi_type.h"
#include "subtitle_debug.h"
#include "scte_subt_parse.h"
#include "scte_subt_display.h"
//#include "subtitle_parse.h"

typedef struct tagSCTE_SUBT_DISPALY_INFO_S
{
    SCTE_SUBT_DISPALY_PARAM_S*    pstDisplayParam;
    SCTE_SUBT_DISPLAY_CALLBACK_FN pfnCallback;
    HI_U32                        u32UserData;
} SCTE_SUBT_DISPALY_INFO_S;

HI_S32 SCTE_SUBT_Display_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    return s32Ret;
}

HI_S32 SCTE_SUBT_Display_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    return s32Ret;
}

HI_S32 SCTE_SUBT_Display_Create(SCTE_SUBT_DISPLAY_CALLBACK_FN pfnCallback, HI_U32 u32UserData, HI_VOID** pphDisplay)
{
    HI_S32 s32Ret = HI_SUCCESS;
    SCTE_SUBT_DISPALY_INFO_S* pstDisplayInfo = HI_NULL;

    pstDisplayInfo = (SCTE_SUBT_DISPALY_INFO_S*)malloc(sizeof(SCTE_SUBT_DISPALY_INFO_S));

    if (HI_NULL == pstDisplayInfo)
    {
        HI_ERR_SUBT("in SCTE_SUBT_Display_Create ,malloc error!!!\n");
        return HI_FAILURE;
    }

    memset(pstDisplayInfo, 0, sizeof(SCTE_SUBT_DISPALY_INFO_S));

    pstDisplayInfo->pstDisplayParam = (SCTE_SUBT_DISPALY_PARAM_S*)malloc(sizeof(SCTE_SUBT_DISPALY_PARAM_S));

    if (HI_NULL == pstDisplayInfo->pstDisplayParam)
    {
        free(pstDisplayInfo);
        pstDisplayInfo = HI_NULL;
        HI_ERR_SUBT("in SCTE_SUBT_Display_Create,Malloc failed!!!...1\n");
        return HI_FAILURE;
    }

    memset(pstDisplayInfo->pstDisplayParam, 0, sizeof(SCTE_SUBT_DISPALY_PARAM_S));

    pstDisplayInfo->pfnCallback = pfnCallback;
    pstDisplayInfo->u32UserData = u32UserData;

    *pphDisplay = pstDisplayInfo;
    return s32Ret;
}

HI_S32 SCTE_SUBT_Display_Destroy(HI_VOID* hDisplay)
{
    SCTE_SUBT_DISPALY_INFO_S* pstDisplayInfo = (SCTE_SUBT_DISPALY_INFO_S*)hDisplay;

    if (HI_NULL == pstDisplayInfo)
    {
        HI_ERR_SUBT("in SCTE_SUBT_Display_Destroy ,param invalid!!!\n");
        return HI_FAILURE;
    }

    if (HI_NULL != pstDisplayInfo->pstDisplayParam)
    {
        free((void*)pstDisplayInfo->pstDisplayParam);
        pstDisplayInfo->pstDisplayParam = HI_NULL;
    }

    free((void*)pstDisplayInfo);
    pstDisplayInfo = HI_NULL;
    
    return HI_SUCCESS;
}

HI_S32 SCTE_SUBT_Display_DisplaySubt(HI_VOID* hDisplay, SCTE_SUBT_OUTPUT_S* pstOutData)
{
    HI_S32 s32Ret = HI_SUCCESS;

    SCTE_SUBT_DISPALY_INFO_S* pstDisplayInfo = (SCTE_SUBT_DISPALY_INFO_S*)hDisplay;

    if (HI_NULL == pstDisplayInfo)
    {
        HI_ERR_SUBT("param is HI_NULL...\n");
        return HI_FAILURE;
    }

    if (SCTE_SUBT_BACKGROUD_FRAMED == pstOutData->enBackgroundStyle)
    {
        pstDisplayInfo->pstDisplayParam->u32x = pstOutData->stFramed.u32TopXPos;
        pstDisplayInfo->pstDisplayParam->u32y = pstOutData->stFramed.u32TopYPos;
        pstDisplayInfo->pstDisplayParam->u32w = (pstOutData->stFramed.u32BottomXPos - pstOutData->stFramed.u32TopXPos) + 1;
        pstDisplayInfo->pstDisplayParam->u32h = (pstOutData->stFramed.u32BottomYPos - pstOutData->stFramed.u32TopYPos) + 1;
    }
    else if (SCTE_SUBT_BACKGROUD_TRANSPARENT == pstOutData->enBackgroundStyle)
    {
        pstDisplayInfo->pstDisplayParam->u32x = pstOutData->u32TopXPos;
        pstDisplayInfo->pstDisplayParam->u32y = pstOutData->u32TopYPos;
        pstDisplayInfo->pstDisplayParam->u32w = (pstOutData->u32BottomXPos - pstOutData->u32TopXPos) + 1;
        pstDisplayInfo->pstDisplayParam->u32h = (pstOutData->u32BottomYPos - pstOutData->u32TopYPos) + 1;
    }

    switch (pstOutData->enDispStandard)
    {
        case STANDARD_720_480_30:
            pstDisplayInfo->pstDisplayParam->u32DisplayWidth = 720;
            pstDisplayInfo->pstDisplayParam->u32DisplayHeight = 480;
            break;

        case STANDARD_720_576_25:
            pstDisplayInfo->pstDisplayParam->u32DisplayWidth = 720;
            pstDisplayInfo->pstDisplayParam->u32DisplayHeight = 576;
            break;

        case STANDARD_1280_720_60:
            pstDisplayInfo->pstDisplayParam->u32DisplayWidth = 1280;
            pstDisplayInfo->pstDisplayParam->u32DisplayHeight = 720;
            break;

        case STANDARD_1920_1080_60:
            pstDisplayInfo->pstDisplayParam->u32DisplayWidth = 1920;
            pstDisplayInfo->pstDisplayParam->u32DisplayHeight = 1080;
            break;

        default:
            pstDisplayInfo->pstDisplayParam->u32DisplayWidth = 720;
            pstDisplayInfo->pstDisplayParam->u32DisplayHeight = 576;
            break;
    }

    pstDisplayInfo->pstDisplayParam->u32PTS = pstOutData->u32PTS;
    pstDisplayInfo->pstDisplayParam->u32Duration = pstOutData->u32Duration;
    pstDisplayInfo->pstDisplayParam->u32DataLen = pstOutData->u32BitmapDataLen;

    pstDisplayInfo->pstDisplayParam->pu8SubtData = pstOutData->pu8SCTESubtData;
    pstDisplayInfo->pstDisplayParam->u32BitWidth = pstOutData->u32BitWidth;
    pstDisplayInfo->pstDisplayParam->enDISP = pstOutData->enDISP;

    if (pstDisplayInfo->pfnCallback)
    {
        s32Ret = pstDisplayInfo->pfnCallback(pstDisplayInfo->u32UserData, (HI_VOID*)pstDisplayInfo->pstDisplayParam);

        if (HI_SUCCESS != s32Ret)
        {
            HI_WARN_SUBT("in SCTE_SUBT_Display_DisplaySubt,function callback failed!!!\n");
        }
    }

    return s32Ret;
}

