/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_ao_aef.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 ********************************************************************************/
#include <asm/setup.h>
#include <linux/interrupt.h>

#include "hi_type.h"
#include "hi_drv_struct.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"

#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#include "hi_drv_proc.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "audio_util.h"
#include "drv_ao_aef.h"

static HI_S32 AEFAttachSnd(SND_CARD_STATE_S* pCard, HI_U32 u32AefId)
{
    HI_VOID* pSndEngine;
    SND_ENGINE_STATE_S* pstEngineState;

    pSndEngine = pCard->pSndEngine[SND_ENGINE_TYPE_PCM];
    if (pSndEngine)
    {
        pstEngineState = (SND_ENGINE_STATE_S*)pSndEngine;
        if (HI_SUCCESS != HAL_AOE_ENGINE_AttachAef(pstEngineState->enEngine, u32AefId))
        {
            HI_ERR_AO("HAL_AOE_ENGINE_AttachAef failed!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_VOID AEF_DestroyDebugAddr(SND_CARD_STATE_S* pCard)
{
    if(pCard->stAefSetting.u32StartPhyAddr)
    {
        HI_DRV_MMZ_UnmapAndRelease(&(pCard->stAefSetting));
    }
}

static HI_S32 AEFDetachSnd(SND_CARD_STATE_S *pCard, HI_U32 u32AefId)
{
    HI_VOID* pSndEngine;
    SND_ENGINE_STATE_S* pstEngineState;

    pSndEngine = pCard->pSndEngine[SND_ENGINE_TYPE_PCM];
    if (!pSndEngine)
    {
        HI_ERR_AO("no have pcm engine!\n");
        return HI_FAILURE;
    }

    pstEngineState = (SND_ENGINE_STATE_S*)pSndEngine;

    if (HI_SUCCESS != HAL_AOE_ENGINE_DetachAef(pstEngineState->enEngine, u32AefId))
    {
        HI_ERR_AO("HAL_AOE_ENGINE_DetachAef failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AEFSetAttr(SND_CARD_STATE_S* pCard, HI_U32 u32AttAef, HI_U32 u32AefMcSupported, HI_U32 u32AefFrameDelay)
{
    HI_U32  u32AefId;
    HI_BOOL bMcProcess = HI_FALSE;   //The flag of whether to do mc process
    SND_ENGINE_STATE_S* pstEngineState = HI_NULL;

    for (u32AefId = 0; u32AefId < AOE_AEF_MAX_CHAN_NUM * 2; u32AefId++)
    {
        if (u32AttAef & (1 << u32AefId))
        {
            bMcProcess = (u32AefMcSupported & (1 << u32AefId)) ? HI_TRUE : HI_FALSE;
            break;
        }
    }

    pstEngineState = (SND_ENGINE_STATE_S *)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];
    if (pstEngineState)
    {
        if (HI_SUCCESS != HAL_AOE_ENGINE_SetAefAttr(pstEngineState->enEngine, bMcProcess, u32AefFrameDelay))
        {
            HI_ERR_AO("HAL_AOE_ENGINE_SetAefAttr failed!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 AEF_AttachSnd(SND_CARD_STATE_S *pCard, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr, HI_U32* pu32AefProcAddr)
{
    HI_U32  u32AttAef;
    HI_S32  Ret;
    HI_CHAR szProcMmzName[32];
    HI_U32  u32AefMcSupported, u32AefFrameDelay;
    SND_AEF_PROC_ATTR_S* pstAefProc = HI_NULL;

    u32AttAef  = pCard->u32AttAef;
    u32AttAef |= ((HI_U32)1L << u32AefId);

    //Update the supported mc of aef(each aef one bit)
    u32AefMcSupported = pCard->u32AefMcSupported;
    if (HI_TRUE == pstAefAttr->bMcSupported)
    {
        u32AefMcSupported |= ((HI_U32)1L << u32AefId);
    }
    else
    {
        u32AefMcSupported &= (~((HI_U32)1L << u32AefId));
    }

    //Update the frame delay of aef
    u32AefFrameDelay  = pCard->u32AefFrameDelay;
    u32AefFrameDelay += pstAefAttr->u32FrameDelay;
    if (AOE_FIFODELAY_FRAME_NUM < u32AefFrameDelay)
    {
        HI_ERR_AO(" All of Aef frame delay is more than (%d)\n", AOE_FIFODELAY_FRAME_NUM);
        u32AefFrameDelay = AOE_FIFODELAY_FRAME_NUM;
    }

    if(HI_SUCCESS != AEFSetAttr(pCard, u32AttAef, u32AefMcSupported, u32AefFrameDelay))
    {
        HI_ERR_AO("AEFSetAttr failed!\n");
        goto err0;
    }

    Ret = AEFAttachSnd(pCard, u32AefId);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("AEFAttachSnd failed\n");
        goto err0;
    }

    pstAefProc = AUTIL_AO_MALLOC(HI_ID_AFLT, sizeof(SND_AEF_PROC_ATTR_S), GFP_KERNEL);
    if (HI_NULL == pstAefProc)
    {
        HI_ERR_AO("malloc SND_AEF_PROC_ATTR_S failed\n");
        goto err1;
    }

    snprintf(szProcMmzName, sizeof(szProcMmzName), "AO_AefProcItem");
    Ret = HI_DRV_MMZ_AllocAndMap(szProcMmzName, MMZ_OTHERS, sizeof(AO_AEF_PROC_ITEM_S), AIAO_BUFFER_ADDR_ALIGN, &pstAefProc->stProcMMz);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HI_DRV_MMZ_AllocAndMap %s failed\n", szProcMmzName);
        goto err2;
    }

    pstAefProc->pstProcItem   = (AO_AEF_PROC_ITEM_S*)(pstAefProc->stProcMMz.pu8StartVirAddr);
    pCard->pAefProc[u32AefId] = (HI_VOID*)pstAefProc;
    pCard->u32AefMcSupported  = u32AefMcSupported;
    pCard->u32AefFrameDelay   = u32AefFrameDelay;
    pCard->u32AttAef = u32AttAef;
    *pu32AefProcAddr = pstAefProc->stProcMMz.u32StartPhyAddr;

    return HI_SUCCESS;

err2:
    AUTIL_AO_FREE(HI_ID_AFLT, pstAefProc);

err1:
    (HI_VOID)AEFDetachSnd(pCard, u32AefId);

err0:
    return HI_FAILURE;
}

HI_S32 AEF_DetachSnd(SND_CARD_STATE_S* pCard, HI_U32 u32AefId, AO_AEF_ATTR_S* pstAefAttr)
{
    HI_U32  u32AttAef;
    HI_U32  u32AefMcSupported, u32AefFrameDelay;
    SND_AEF_PROC_ATTR_S* pstAefProc = HI_NULL;

    pstAefProc = (SND_AEF_PROC_ATTR_S*)pCard->pAefProc[u32AefId];
    u32AttAef  = pCard->u32AttAef;
    u32AttAef &= ~((HI_U32)1L << u32AefId);

    if (pstAefProc)
    {
        HI_DRV_MMZ_UnmapAndRelease(&pstAefProc->stProcMMz);
        AUTIL_AO_FREE(HI_ID_AFLT, pstAefProc);
        pCard->pAefProc[u32AefId] = (HI_VOID*)HI_NULL;
    }

    if (HI_SUCCESS != AEFDetachSnd(pCard, u32AefId))
    {
        HI_ERR_AO("AEFDetachSnd failed\n");
        return HI_FAILURE;
    }

    //Update the supported mc of aef(each aef one bit)
    u32AefMcSupported  = pCard->u32AefMcSupported;
    u32AefMcSupported &= (~((HI_U32)1L << u32AefId));

    //Update the frame delay of aef
    u32AefFrameDelay  = pCard->u32AefFrameDelay;
    u32AefFrameDelay -= pstAefAttr->u32FrameDelay;

    if(HI_SUCCESS != AEFSetAttr(pCard, u32AttAef, u32AefMcSupported, u32AefFrameDelay))
    {
        HI_ERR_AO("AEFSetAttr failed!\n");
        return HI_FAILURE;
    }

    pCard->u32AefMcSupported = u32AefMcSupported;
    pCard->u32AefFrameDelay  = u32AefFrameDelay;
    pCard->u32AttAef = u32AttAef;

    return HI_SUCCESS;
}

HI_S32 AEF_GetSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32AefId;

    pstSndSettings->u32AttAef = pCard->u32AttAef;
    pstSndSettings->u32AefMcSupported = pCard->u32AefMcSupported;
    pstSndSettings->u32AefFrameDelay  = pCard->u32AefFrameDelay;

    for(u32AefId = 0; u32AefId < AOE_AEF_MAX_CHAN_NUM * 2; u32AefId++)
    {
        pstSndSettings->pAefProc[u32AefId]  = pCard->pAefProc[u32AefId];
    }

    return HI_SUCCESS;
}

HI_S32 AEF_RestoreSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32AefId;

    for (u32AefId = 0; u32AefId < AOE_AEF_MAX_CHAN_NUM * 2; u32AefId++)
    {
        if (pstSndSettings->u32AttAef & ((HI_U32)1L << u32AefId))
        {
            if (HI_SUCCESS != AEFAttachSnd(pCard, u32AefId))
            {
                return HI_FAILURE;
            }
        }
        pCard->pAefProc[u32AefId] = pstSndSettings->pAefProc[u32AefId];
    }

    if(HI_SUCCESS != AEFSetAttr(pCard, pstSndSettings->u32AttAef,
                     pstSndSettings->u32AefMcSupported, pstSndSettings->u32AefFrameDelay))
    {
        HI_ERR_AO("AEFSetAttr failed!\n");
        return HI_FAILURE;
    }

    pCard->u32AefMcSupported = pstSndSettings->u32AefMcSupported;
    pCard->u32AefFrameDelay  = pstSndSettings->u32AefFrameDelay;
    pCard->u32AttAef = pstSndSettings->u32AttAef;

    return HI_SUCCESS;
}

#if (1 == HI_PROC_SUPPORT)
HI_S32 AEF_ReadProc( struct seq_file* p, SND_CARD_STATE_S* pCard )
{
    HI_U32 i;
    HI_VOID* pAefProc;
    AO_AEF_PROC_ITEM_S*  pstProcItem;

    for (i = 0; i < AOE_AEF_MAX_CHAN_NUM * 2; i++)
    {
        if (pCard->u32AttAef & ((HI_U32)1L << i))
        {
            pAefProc = pCard->pAefProc[i];
            if (HI_NULL == pAefProc)
            {
                return HI_FAILURE;
            }

            pstProcItem = ((SND_AEF_PROC_ATTR_S*)pAefProc)->pstProcItem;
            PROC_PRINT(p,
                       "Aef(%d): Type(%s), AuthDescription(%s), Status(%s)\n",
                       pstProcItem->u32AefId,
                       pstProcItem->szName,
                       pstProcItem->szDescription,
                       (HI_CHAR*)((HI_TRUE == pstProcItem->bEnable) ? "start" : "stop"));
        }
    }

    PROC_PRINT(p, "\n");

    return HI_SUCCESS;
}

HI_S32 AEF_WriteProc(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_U32 u32AefId = AOE_AEF_MAX_CHAN_NUM * 2;
    HI_CHAR* pcAttachCmd = "attach";
    HI_CHAR* pcDetachCmd = "detach";
    HI_CHAR* pcOneCmd = "1";
    HI_CHAR* pcZeroCmd = "0";
    HI_U32 u32Ret;

    if (pcBuf[0] < '0' || pcBuf[0] > '9')//do not have param
    {
        return HI_FAILURE;
    }

    u32AefId = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
    if (u32AefId >= AOE_AEF_MAX_CHAN_NUM * 2)
    {
        return HI_FAILURE;
    }
    AO_STRING_SKIP_NON_BLANK(pcBuf);
    AO_STRING_SKIP_BLANK(pcBuf);
    if (strstr(pcBuf, pcAttachCmd) || strstr(pcBuf, pcOneCmd))
    {
        u32Ret = AEFAttachSnd(pCard, u32AefId);
        if (HI_SUCCESS == u32Ret)
        {
            HI_DRV_PROC_EchoHelper("Set aef%d ATTACH success.\n", u32AefId);
        }

        return u32Ret;
    }
    else if (strstr(pcBuf, pcDetachCmd) || strstr(pcBuf, pcZeroCmd))
    {
        u32Ret = AEFDetachSnd(pCard, u32AefId);
        if (HI_SUCCESS == u32Ret)
        {
            HI_DRV_PROC_EchoHelper("Set aef%d DETACH success.\n", u32AefId);
        }

        return u32Ret;
    }
    else
    {
        return HI_FAILURE;
    }
}
#endif

HI_S32 AEF_GetBufAttr(SND_CARD_STATE_S* pCard, AO_AEF_BUF_ATTR_S* pstAefBuf)
{
#ifdef HI_SND_USER_AEF_SUPPORT
    HI_S32  Ret;
    AOE_ENGINE_CHN_ATTR_S stEngAttr;
    HI_CHAR szSettingMmzName[32];
    SND_ENGINE_STATE_S* pstEnginestate = (SND_ENGINE_STATE_S*)pCard->pSndEngine[SND_ENGINE_TYPE_PCM];

    if (!pstEnginestate)
    {
        pstAefBuf->tPhyEngineAddr = 0;
        HI_INFO_AO("pcm engine do not exist!\n");
        return HI_SUCCESS;
    }

    pstAefBuf->tPhyInBufAddr    = pCard->stAefInBufMmz.u32StartPhyAddr;
    pstAefBuf->tPhyOutBufAddr   = pCard->stAefOutBufMmz.u32StartPhyAddr;
    pstAefBuf->u32InBufSize     = pCard->stAefInBufMmz.u32Size;
    pstAefBuf->u32OutBufSize    = pCard->stAefOutBufMmz.u32Size;

    (HI_VOID)HAL_AOE_ENGINE_GetAttr(pstEnginestate->enEngine, &stEngAttr);

    pstAefBuf->u32InBufFrameSize        = stEngAttr.stAefInBufAttr.u32PeriodSize;
    pstAefBuf->u32OutBufFrameSize       = stEngAttr.stAefOutBufAttr.u32PeriodSize;
    pstAefBuf->u32InBufFrameNum         = stEngAttr.stAefInBufAttr.u32PeriodNum;
    pstAefBuf->u32OutBufFrameNum        = stEngAttr.stAefOutBufAttr.u32PeriodNum;

    (HI_VOID)HAL_AOE_ENGINE_GetRegAddr(pstEnginestate->enEngine, &pstAefBuf->tPhyEngineAddr);

    if (!pCard->stAefSetting.u32StartPhyAddr)// =0²ÅÉêÇë
    {
        snprintf(szSettingMmzName, sizeof(szSettingMmzName), "AO_AefSettingItem");
        Ret = HI_DRV_MMZ_AllocAndMap(szSettingMmzName, MMZ_OTHERS, pstAefBuf->stAefDebugAddr.u32SettingSize, AIAO_BUFFER_ADDR_ALIGN, &pCard->stAefSetting);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HI_DRV_MMZ_AllocAndMap %s failed\n", szSettingMmzName);
            goto err0;
        }
        memset(pCard->stAefSetting.pu8StartVirAddr, 0, pstAefBuf->stAefDebugAddr.u32SettingSize);
    }

    /*·µ»Ø¸ømpi_aef*/
    pstAefBuf->stAefDebugAddr.tPhySettingAddr = pCard->stAefSetting.u32StartPhyAddr;
    pstAefBuf->stAefDebugAddr.u32SettingSize = pCard->stAefSetting.u32Size;

    return HI_SUCCESS;
err0:
    return HI_FAILURE;
#else

    return HI_SUCCESS;
#endif
}

HI_S32 AEF_GetDebugAddr(SND_CARD_STATE_S* pCard, AEF_DEBUG_ATTR_S *pstDebugAttr)
{
#ifdef HI_SND_USER_AEF_SUPPORT
    pstDebugAttr->tPhySettingAddr = pCard->stAefSetting.u32StartPhyAddr;
    pstDebugAttr->u32SettingSize = pCard->stAefSetting.u32Size;
#endif

    return HI_SUCCESS;
}



