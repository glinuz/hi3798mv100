/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_ao_op_func.c
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
#include "hi_drv_sys.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "audio_util.h"
#include "drv_ao_op.h"
#include "hal_aoe_func.h"
#include "hal_aiao_func.h"
#include "hal_aiao_common.h"
#include "hal_aoe_common.h"
#include "hal_aoe.h"
#include "hal_cast.h"

#include "drv_ao_track.h"

#include "drv_ao_cast.h"

/******************************Cast process FUNC*************************************/
HI_VOID*  CastGetEngineHandlebyType(SND_CARD_STATE_S* pCard, SND_ENGINE_TYPE_E enType)
{
    HI_VOID* pSndEngine;

    pSndEngine = pCard->pSndEngine[enType];
    if (pSndEngine)
    {
        return pSndEngine;
    }

    return HI_NULL;
}

HI_S32 CastGetIDbyHandle(SND_CARD_STATE_S* pCard, HI_HANDLE  handle)
{
    HI_U32 ID;
    SND_CAST_STATE_S* state;

    state = (SND_CAST_STATE_S*)(pCard->pCast[handle]);
    if (!state)
    {
        HI_ERR_AO("SndProcCastRoute  pCard->hCast[%d] NULL\n", handle);
        return HI_FAILURE;
    }

    ID = state->CastId;
    return ID;
}




static HI_S32 CastCreate(SND_CARD_STATE_S* pCard, HI_S32* ps32CastId, HI_UNF_SND_CAST_ATTR_S* pstCastAttr,
                         MMZ_BUFFER_S* pstMMz)
{
    HI_S32 Ret;

    Ret = SND_CreateCastOp(pCard, ps32CastId, pstCastAttr, pstMMz);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CastDestory(SND_CARD_STATE_S* pCard, HI_U32 u32CastID)
{
    HI_S32 Ret;

    Ret = SND_DestoryCastOp(pCard,  u32CastID);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CastEnable(SND_CARD_STATE_S* pCard, SND_CAST_STATE_S* state, HI_S32 s32CastID, HI_BOOL bEnable)
{
    HI_VOID* pSndOp;
    AOE_AOP_ID_E Aop;
    HI_VOID* hEngine;
    SND_ENGINE_STATE_S* pEnginestate;

    pSndOp = pCard->pCastOp[s32CastID];
    if (pSndOp == HI_NULL)
    {
        HI_ERR_AO("SndProcCastRoute  hSndOp=%p\n", pSndOp);
        return HI_FAILURE;
    }
    Aop = SND_GetOpAopId(pSndOp);

    SND_StopCastOp(pCard, s32CastID);
    HI_INFO_AO("SND_StopOp Over\n");

    hEngine = CastGetEngineHandlebyType(pCard, SND_ENGINE_TYPE_PCM);
    if (!hEngine)
    {
        state->enCurnStatus = SND_CAST_STATUS_STOP;
        HI_INFO_AO("CastGetEngineHandlebyType found no Engine(%s cast).\n", (HI_TRUE == bEnable) ? "Enable" : "Disable");
        return HI_SUCCESS;
    }
    pEnginestate = (SND_ENGINE_STATE_S*)hEngine;

    if (bEnable)
    {
        HAL_AOE_ENGINE_AttachAop(pEnginestate->enEngine, Aop);
        SND_StartCastOp(pCard, s32CastID);
        state->enCurnStatus = SND_CAST_STATUS_START;
    }
    else
    {
        SND_StopCastOp(pCard, s32CastID);
        HAL_AOE_ENGINE_DetachAop(pEnginestate->enEngine, Aop);
        state->enCurnStatus = SND_CAST_STATUS_STOP;
    }

    return HI_SUCCESS;
}





/******************************AO Cast FUNC*************************************/

static HI_VOID  CastSetMute(HI_VOID* pSndOp, HI_BOOL bMute)
{
    AOE_AOP_ID_E Aop;

    if (pSndOp == HI_NULL)
    {
        HI_ERR_AO("CastSetVolume  hSndOp=%p\n", pSndOp);
        return ;
    }
    Aop = SND_GetOpAopId(pSndOp);

    HAL_AOE_AOP_SetMute(Aop, bMute);
}

#ifdef __DPT__
HI_S32 CAST_SetAllMute(SND_CARD_STATE_S* pCard, HI_BOOL bMute)
{
    HI_U32 i;
    SND_CAST_STATE_S* pCast;

    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        if (pCard->pCast[i])
        {
            pCast = (SND_CAST_STATE_S*)pCard->pCast[i];

            if (HI_FALSE == pCast->bMute && HI_FALSE == bMute)
            {
                HI_INFO_AO("Set cast unmute Id %d\n", i);
                CastSetMute(pCard->pCastOp[i], HI_FALSE);
            }
            else
            {
                HI_INFO_AO("Set cast mute Id %d\n", i);
                CastSetMute(pCard->pCastOp[i], HI_TRUE);
            }
        }
    }

    return HI_SUCCESS;
}
#endif

HI_S32 CAST_SetMute(SND_CARD_STATE_S* pCard, HI_U32 u32CastID, HI_BOOL bMute)
{
    SND_CAST_STATE_S* pCast;

    pCast = (SND_CAST_STATE_S*)pCard->pCast[u32CastID];
    if (pCast == HI_NULL)
        return HI_SUCCESS;

    if (
#ifdef __DPT__
        HI_FALSE == pCard->bAllCastMute &&
#endif
        HI_FALSE == bMute)
    {
        HI_INFO_AO("Cast Set unMute Id %d\n", u32CastID);
        CastSetMute(pCard->pCastOp[u32CastID], HI_FALSE);
    }
    else
    {
        HI_INFO_AO("Cast Set Mute Id %d\n", u32CastID);
        CastSetMute(pCard->pCastOp[u32CastID], HI_TRUE);
    }

    pCast->bMute = bMute;

    return HI_SUCCESS;
}


HI_S32 CAST_GetMute(SND_CARD_STATE_S* pCard, HI_U32 u32CastID, HI_BOOL* pbMute)
{
    SND_CAST_STATE_S* pCast;

    if (HI_NULL == pbMute)
    {
        return HI_FAILURE;
    }

    pCast = (SND_CAST_STATE_S*)pCard->pCast[u32CastID];


    *pbMute = pCast->bMute;

    return HI_SUCCESS;
}

static HI_VOID  CastSetVolume(HI_VOID* pSndOp, HI_U32 u32VolumeLdB, HI_U32 u32VolumeRdB)
{
    AOE_AOP_ID_E Aop;

    if (pSndOp == HI_NULL)
    {
        HI_ERR_AO("CastSetVolume  hSndOp=%p\n", pSndOp);
        return ;
    }
    Aop = SND_GetOpAopId(pSndOp);

    HAL_AOE_AOP_SetLRVolume(Aop, u32VolumeLdB, u32VolumeRdB);

}

HI_S32 CAST_SetAbsGain(SND_CARD_STATE_S* pCard, HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S* pstCastAbsGain)
{
    SND_CAST_STATE_S* pCast;
    HI_U32 u32dBLReg;
    HI_U32 u32dBRReg;

    if (HI_NULL == pstCastAbsGain)
    {
        return HI_FAILURE;
    }
    if (HI_TRUE == pstCastAbsGain->bLinearMode)
    {
        CHECK_AO_LINEARVOLUME(pstCastAbsGain->s32GainL);
        CHECK_AO_LINEARVOLUME(pstCastAbsGain->s32GainR);

        u32dBLReg = AUTIL_VolumeLinear2RegdB((HI_U32)pstCastAbsGain->s32GainL);
        u32dBRReg = AUTIL_VolumeLinear2RegdB((HI_U32)pstCastAbsGain->s32GainR);
    }
    else
    {
        CHECK_AO_ABSLUTEVOLUMEEXT(pstCastAbsGain->s32GainL);
        CHECK_AO_ABSLUTEVOLUMEEXT(pstCastAbsGain->s32GainR);

        u32dBLReg = AUTIL_VolumedB2RegdB(pstCastAbsGain->s32GainL);
        u32dBRReg = AUTIL_VolumedB2RegdB(pstCastAbsGain->s32GainR);
    }

    pCast = (SND_CAST_STATE_S*)pCard->pCast[u32CastID];

    CastSetVolume(pCard->pCastOp[u32CastID], u32dBLReg, u32dBRReg);

    pCast->stCastAbsGain.bLinearMode = pstCastAbsGain->bLinearMode;
    pCast->stCastAbsGain.s32GainL = pstCastAbsGain->s32GainL;
    pCast->stCastAbsGain.s32GainR = pstCastAbsGain->s32GainR;
    return HI_SUCCESS;
}

HI_S32 CAST_GetAbsGain(SND_CARD_STATE_S* pCard, HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S* pstCastAbsGain)
{
    SND_CAST_STATE_S* pCast;

    if (HI_NULL == pstCastAbsGain)
    {
        return HI_FAILURE;
    }
    pCast = (SND_CAST_STATE_S*)pCard->pCast[u32CastID];

    pstCastAbsGain->bLinearMode = pCast->stCastAbsGain.bLinearMode;
    pstCastAbsGain->s32GainL = pCast->stCastAbsGain.s32GainL;
    pstCastAbsGain->s32GainR = pCast->stCastAbsGain.s32GainR;
    return HI_SUCCESS;
}

static HI_VOID  CastSetAefBypass(HI_VOID* pSndOp, HI_BOOL bBypass)
{
    AOE_AOP_ID_E Aop;

    if (pSndOp == HI_NULL)
    {
        HI_ERR_AO("CastSetAefBypass  pSndOp=%p\n", pSndOp);
        return ;
    }
    Aop = SND_GetOpAopId(pSndOp);

    HAL_AOE_AOP_SetAefBypass(Aop, bBypass);
}

HI_S32 Cast_SetAefBypass(SND_CARD_STATE_S* pCard, HI_U32 u32CastID, HI_BOOL bBypass)
{
    SND_CAST_STATE_S* pCast;

    pCast = (SND_CAST_STATE_S*)pCard->pCast[u32CastID];
    CastSetAefBypass(pCard->pCastOp[u32CastID], bBypass);

    return HI_SUCCESS;
}

#if (1 == HI_PROC_SUPPORT)
HI_VOID CAST_ReadProc(struct seq_file* p, SND_CARD_STATE_S* pCard)
{
    SND_CAST_STATE_S* pCast;
    HI_U32 i;
    HI_VOID* pSndOp;
    AOE_AOP_ID_E Aop;

    for (i = 0; i < AO_MAX_CAST_NUM; i++)
    {
        if (pCard->uSndCastInitFlag & ((HI_U32)1L << i))
        {
            pCast = (SND_CAST_STATE_S*)pCard->pCast[i];
            pSndOp = pCard->pCastOp[pCast->CastId];
            Aop = SND_GetOpAopId(pSndOp);
            PROC_PRINT(p,
                       "Cast(%d): *Aop(0x%x), Status(%s), UserEnable(%s), Weight(%.3d/%.3d%s), Mute(%s)",
                       pCast->CastId,
                       (HI_U32)Aop,
                       (HI_CHAR*)((SND_CAST_STATUS_START == pCast->enCurnStatus) ? "start" : ((SND_CAST_STATUS_STOP == pCast->enCurnStatus) ? "stop" : "pause")),
                       (HI_CHAR*)((pCast->bUserEnableSetting == HI_TRUE) ? "On" : "Off"),
                       pCast->stCastAbsGain.s32GainL,
                       pCast->stCastAbsGain.s32GainR,
                       (HI_TRUE == pCast->stCastAbsGain.bLinearMode) ? "" : "dB",
                       (HI_FALSE == pCast->bMute) ? "off" : "on"
                      );
            if (pCard->bCastSimulateOp == HI_TRUE)
            {
                PROC_PRINT(p,
                           ", *CastSimulatedOp(%s)\n",
                           AUTIL_Port2Name(pCard->enCastSimulatePort)
                          );
            }
            else
            {
                PROC_PRINT(p, "\n");
            }
            PROC_PRINT(p,
                       "         SampleRate(%.6d), Channel(%.2d), BitWidth(%2d)\n",
                       pCast->u32SampleRate,
                       pCast->u32Channels,
                       pCast->s32BitPerSample
                      );
            PROC_PRINT(p,
                       "         MaxFrameNum(%.2d), SamplePerFrame(%.5d), AcquireFrame(%s)\n",
                       pCast->stUserCastAttr.u32PcmFrameMaxNum,  //verify
                       pCast->stUserCastAttr.u32PcmSamplesPerFrame,  //verify
                       (HI_CHAR*)((pCast->bAcquireCastFrameFlag == HI_TRUE) ? "On" : "Off")
                      );
        }
    }
}
#endif

HI_S32 CAST_GetDefAttr(HI_UNF_SND_CAST_ATTR_S* pstDefAttr)
{
    pstDefAttr->u32PcmFrameMaxNum = AO_CAST_DEFATTR_FRAMEMAXNUM;
    pstDefAttr->u32PcmSamplesPerFrame = AO_CAST_DEFATTR_SAMPLESPERFRAME;
    pstDefAttr->bAddMute = HI_TRUE;
    /*
    pstDefAttr->s32BitPerSample = AO_CAST_DEFATTR_BITSPERSAMPLE;
    pstDefAttr->u32Channels = AO_CAST_DEFATTR_CHANNEL;
    pstDefAttr->u32SampleRate = AO_CAST_DEFATTR_SAMPLERATE;
    pstDefAttr->u32DataFormat = 0;
    */
    return HI_SUCCESS;
}

HI_S32 CAST_CreateNew(SND_CARD_STATE_S* pCard, HI_UNF_SND_CAST_ATTR_S* pstCastAttr, MMZ_BUFFER_S* pstMMz, HI_U32 hCast)
{
    SND_CAST_STATE_S* state = HI_NULL;
    HI_U32 CastId;
    HI_S32 Ret = HI_FAILURE;

    if (!pCard)
    {
        return HI_FAILURE;
    }

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(SND_CAST_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc CAST_Create failed\n");
        goto CastCreate_ERR_EXIT;
    }

    memset(state, 0, sizeof(SND_CAST_STATE_S));

    memcpy(&state->stUserCastAttr, pstCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));

    if (HI_SUCCESS != CastCreate(pCard, &CastId, &state->stUserCastAttr, pstMMz))
    {
        goto CastCreate_ERR_EXIT;
    }

    state->tPhyAddr = pstMMz->u32StartPhyAddr;
    state->tKernelVirtAddr = (HI_VIRT_ADDR_T)pstMMz->pu8StartVirAddr;
    state->hCast  = hCast;
    state->CastId = CastId;

    state->u32Channels = 2;
    state->u32SampleRate = 48000;
    state->s32BitPerSample = 16;

    state->u32SampleBytes = AUTIL_CalcFrameSize(state->u32Channels , (HI_U32)state->s32BitPerSample);
    state->u32FrameBytes = pstCastAttr->u32PcmSamplesPerFrame *  state->u32SampleBytes;
    state->u32FrameSamples = pstCastAttr->u32PcmSamplesPerFrame;
    state->bAddMute = pstCastAttr->bAddMute;

    state->bUserEnableSetting = HI_FALSE;
    state->enCurnStatus = SND_CAST_STATUS_STOP;
    state->bAcquireCastFrameFlag = HI_FALSE;
    state->stCastAbsGain.bLinearMode = HI_TRUE;
    state->stCastAbsGain.s32GainL = AO_MAX_LINEARVOLUME;
    state->stCastAbsGain.s32GainR = AO_MAX_LINEARVOLUME;
    state->bMute = HI_FALSE;

    //pstCastAttr->u32PhyAddr = state->stUserCastAttr.u32PhyAddr;
    //pstCastAttr->u32Channels = state->stUserCastAttr.u32Channels;
    //pstCastAttr->u32SampleRate = state->stUserCastAttr.u32SampleRate;
    //pstCastAttr->s32BitPerSample = state->stUserCastAttr.s32BitPerSample;

#if 0
    HI_ERR_AO("state->hCast = 0x%x\n", state->hCast);
    HI_ERR_AO("state->CastId = 0x%x\n", state->CastId);
    HI_ERR_AO("state->stUserCastAttr.tPhyAddr = 0x%x\n", state->stUserCastAttr.tPhyAddr);
    HI_ERR_AO("state->u32SampleBytes = 0x%x\n", state->u32SampleBytes);
    HI_ERR_AO("state->u32FrameBytes = 0x%x\n", state->u32FrameBytes);
#endif

    pCard->pCast[hCast] = (HI_VOID*)state;
    pCard->uSndCastInitFlag |= ((HI_U32)1L << hCast);

    return HI_SUCCESS;

CastCreate_ERR_EXIT:
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}

HI_S32 CAST_DestroyCast(SND_CARD_STATE_S* pCard, HI_U32 hCast)
{
    SND_ENGINE_STATE_S* pEnginestate;
    SND_CAST_STATE_S* state;
    HI_S32 s32CastID;
    HI_VOID* pSndOp;
    HI_VOID* pEngine;
    AOE_AOP_ID_E Aop;

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        return HI_FAILURE;
    }

    s32CastID = state->CastId;

    if (SND_CAST_STATUS_STOP != state->enCurnStatus)
    {
        pSndOp = pCard->pCastOp[s32CastID];
        if (pSndOp == HI_NULL)
        {
            HI_ERR_AO("SndProcCastRoute  hSndOp=%p\n", pSndOp);
            return HI_FAILURE;
        }

        Aop = SND_GetOpAopId(pSndOp);
        pEngine = CastGetEngineHandlebyType(pCard, SND_ENGINE_TYPE_PCM);
        if (!pEngine)
        {
            HI_ERR_AO("No Engine Found !\n");
            return HI_FAILURE;
        }

        pEnginestate = (SND_ENGINE_STATE_S*)pEngine;

        SND_StopCastOp(pCard, s32CastID);
        HAL_AOE_ENGINE_DetachAop(pEnginestate->enEngine, Aop);
        state->enCurnStatus = SND_CAST_STATUS_STOP;
    }


    CastDestory(pCard, s32CastID);

    pCard->uSndCastInitFlag &= ~((HI_U32)1L << state->hCast);
    pCard->pCast[state->hCast] = HI_NULL;

    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    return HI_SUCCESS;
}


HI_S32 CAST_SetInfo(SND_CARD_STATE_S* pCard, HI_U32 hCast, HI_VIRT_ADDR_T tUserVirtAddr)
{
    SND_CAST_STATE_S* state;

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        return HI_FAILURE;
    }

    state->tUserVirtAddr = tUserVirtAddr;

    //HI_ERR_AO("tUserVirtAddr = 0x%p\n", tUserVirtAddr);

    return HI_SUCCESS;
}

HI_S32 CAST_GetInfo(SND_CARD_STATE_S* pCard, HI_U32 hCast, AO_Cast_Info_Param_S* pstInfo)
{
    SND_CAST_STATE_S* state;

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        return HI_FAILURE;
    }

    pstInfo->tUserVirtAddr = state->tUserVirtAddr;
    pstInfo->tKernelVirtAddr = state->tKernelVirtAddr;
    pstInfo->tPhyAddr = state->tPhyAddr;
    pstInfo->u32FrameBytes = state->u32FrameBytes;
    pstInfo->u32FrameSamples = state->u32FrameSamples;

    pstInfo->u32Channels = state->u32Channels;
    pstInfo->s32BitPerSample = state->s32BitPerSample;
    //HI_ERR_AO("tUserVirtAddr = 0x%p\n", tUserVirtAddr);

    return HI_SUCCESS;
}

HI_VOID CAST_GetSettings(SND_CARD_STATE_S* pCard, HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings)
{
    SND_CAST_STATE_S* state;

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        return;
    }
    pstCastSettings->bMute = state->bMute;
    memcpy(&pstCastSettings->stCastAbsGain, &state->stCastAbsGain, sizeof(HI_UNF_SND_ABSGAIN_ATTR_S));
    pstCastSettings->tUserVirtAddr = state->tUserVirtAddr;
    pstCastSettings->bUserEnableSetting = state->bUserEnableSetting;
    return;
}

HI_VOID CAST_RestoreSettings(SND_CARD_STATE_S* pCard, HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings)
{
    SND_CAST_STATE_S* state;

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        HI_ERR_AO("Cast(%d) don't attach card!\n", hCast);
        return;
    }

    state->tUserVirtAddr = pstCastSettings->tUserVirtAddr;

    /* fource discard ReleaseCastFrame after resume */
    state->bAcquireCastFrameFlag = HI_FALSE;
    if (pstCastSettings->bUserEnableSetting != state->bUserEnableSetting)
    {
        CAST_SetEnable(pCard, hCast, pstCastSettings->bUserEnableSetting);
        CAST_SetAbsGain(pCard, hCast, &pstCastSettings->stCastAbsGain);
        CAST_SetMute(pCard, hCast, pstCastSettings->bMute);
    }

    return;
}

HI_S32 CAST_SetEnable(SND_CARD_STATE_S* pCard, HI_U32 hCast, HI_BOOL bEnable)
{
    SND_CAST_STATE_S* state;
    HI_S32 s32CastID;

    //TOCHECK CURRENT state
    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        HI_ERR_AO("SND_CAST_STATE_S pointer  NULL\n");
        return HI_FAILURE;
    }

    s32CastID = CastGetIDbyHandle(pCard, hCast);
    if (HI_FAILURE == s32CastID)
    {
        HI_ERR_AO("CastGetIDbyHandle  Failed\n");
        return HI_FAILURE;
    }

#if 1
    state->bUserEnableSetting = bEnable;
    return CastEnable(pCard, state, s32CastID, bEnable);

#else

    //hSndOp = SND_GetOpHandlebyOutType(pCard, HI_UNF_SND_OUTPUTTYPE_CAST);
    hSndOp = pCard->hCastOp[s32CastID];
    if (hSndOp == HI_NULL)
    {
        HI_ERR_AO("SndProcCastRoute  hSndOp=%p\n", hSndOp);
        return HI_FAILURE;
    }
    Aop = SND_GetOpAopId(hSndOp);

    //HI_ERR_AO("hSndOp=%p Aop=0x%x\n", hSndOp, Aop);

#if 0
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32BitPerSample=%d\n", stOpAttr.u32BitPerSample);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32Channels=%d\n", stOpAttr.u32Channels);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32DataFormat=%d\n", stOpAttr.u32DataFormat);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32LatencyThdMs=%d\n", stOpAttr.u32LatencyThdMs);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32PeriodBufSize=%d\n", stOpAttr.u32PeriodBufSize);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32PeriodNumber=%d\n", stOpAttr.u32PeriodNumber);
    HI_ERR_AO("SndProcCastRoute  SND_GetOpAttr u32SampleRate=%d\n", stOpAttr.u32SampleRate);
#endif

    SND_StopCastOp(pCard, s32CastID);
    HI_INFO_AIAO("SND_StopOp Over\n");

    pCaststate->bUserEnableSetting = bEnable;

    hEngine = CastGetEngineHandlebyType(pCard, SND_ENGINE_TYPE_PCM);
    if (!hEngine)
    {
        if (bEnable)
        {
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_AO("Disable Cast But No Engine Found !\n");
            return HI_FAILURE;
        }
    }
    pEnginestate = (SND_ENGINE_STATE_S*)hEngine;

    if (bEnable)
    {
        HAL_AOE_ENGINE_AttachAop(pEnginestate->enEngine, Aop);
        SND_StartCastOp(pCard, s32CastID);
        pCaststate->enCurnStatus = SND_CAST_STATUS_START;
    }
    else
    {
        SND_StopCastOp(pCard, s32CastID);
        HAL_AOE_ENGINE_DetachAop(pEnginestate->enEngine, Aop);
        pCaststate->enCurnStatus = SND_CAST_STATUS_STOP;
    }

    return HI_SUCCESS;
#endif
}


HI_S32 CAST_GetEnable(SND_CARD_STATE_S* pCard, HI_U32 hCast, HI_BOOL* pbEnable)
{
    SND_CAST_STATE_S* state;

    //TOCHECK CURRENT state
    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (HI_NULL == state)
    {
        HI_ERR_AO("SND_CAST_STATE_S pointer  NULL\n");
        return HI_FAILURE;
    }

    *pbEnable = state->bUserEnableSetting;
    return HI_SUCCESS;
}

HI_S32 CAST_ReadData(SND_CARD_STATE_S* pCard, HI_U32 hCast,
                     AO_Cast_Data_Param_S* pstCastData)
{
    SND_CAST_STATE_S* state = HI_NULL;
    HI_S32 Ret;
    HI_S32 s32CastID;

    //TOCHECK CURRENT state

    s32CastID = CastGetIDbyHandle(pCard, hCast);
    if (HI_FAILURE == s32CastID)
    {
        HI_ERR_AO("CastGetIDbyHandle  Failed\n");
        return HI_FAILURE;
    }

    if (HI_NULL == pCard->pCastOp[s32CastID])
    {
        HI_ERR_AO("  hSndOp=%p\n", pCard->pCastOp[s32CastID]);
        return HI_FAILURE;
    }

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (!state)
    {
        HI_ERR_AO("SND_CAST_STATE_S  state =%p\n", state);
        return HI_FAILURE;
    }

#if 1
    pstCastData->stAOFrame.u32PcmSamplesPerFrame = 0;       //clear ao frame sample size

    if (HI_FALSE == state->bUserEnableSetting)
    {
        //HI_ERR_AO("Cast is not Enable!\n");
        return HI_FAILURE;
    }
    else            //user enable cast , but cast is not activity
    {
        if (SND_CAST_STATUS_STOP == state->enCurnStatus)    //to active cast
        {
            Ret = CastEnable(pCard, state, s32CastID, state->bUserEnableSetting);
            if (HI_FAILURE == Ret)
            {
                HI_ERR_AO("Enable Cast Failed when read data !\n");
                return HI_FAILURE;
            }
        }
        if (SND_CAST_STATUS_STOP == state->enCurnStatus)
        {
            return HI_SUCCESS;
        }

    }
#endif

    pstCastData->u32FrameBytes = state->u32FrameBytes;
    pstCastData->u32SampleBytes = state->u32SampleBytes;

#if 0
    HI_ERR_AO("state hCast=0x%x\n", state->hCast);
    HI_ERR_AO("state CastId=0x%x\n", state->CastId);
    HI_ERR_AO("stUserCastAttr.s32BitPerSample=0x%x\n", state->stUserCastAttr.s32BitPerSample);
    HI_ERR_AO("stUserCastAttr.u32Channels=0x%x\n", state->stUserCastAttr.u32Channels);
    HI_ERR_AO("stUserCastAttr.u32SampleRate=0x%x\n", state->stUserCastAttr.u32SampleRate);
    //HI_ERR_AO("stUserCastAttr.u32DataFormat=0x%x\n", state->stUserCastAttr.u32DataFormat);
    //HI_ERR_AO("stUserCastAttr.u32LatencyThdMs=0x%x\n", state->stUserCastAttr.u32LatencyThdMs);

    HI_ERR_AO("stUserCastAttr.u32PcmFrameMaxNum=0x%x\n", state->stUserCastAttr.u32PcmFrameMaxNum);
    HI_ERR_AO("stUserCastAttr.u32PcmSamplesPerFrame=0x%x\n", state->stUserCastAttr.u32PcmSamplesPerFrame);

    HI_ERR_AO("pstCastData->u32FrameBytes=0x%x\n", pstCastData->u32FrameBytes);
    HI_ERR_AO("pstCastData->u32SampleBytes=0x%x\n", pstCastData->u32SampleBytes);
#endif

    pstCastData->stAOFrame.s32BitPerSample = state->s32BitPerSample;
    pstCastData->stAOFrame.u32Channels = state->u32Channels;
    pstCastData->stAOFrame.u32SampleRate = state->u32SampleRate;
    pstCastData->stAOFrame.u32PcmSamplesPerFrame = state->stUserCastAttr.u32PcmSamplesPerFrame;
    pstCastData->stAOFrame.bInterleaved = HI_TRUE;

    Ret = SND_ReadCastData(pCard,  s32CastID, pstCastData);
    if (HI_FAILURE == Ret)
    {
        return HI_FAILURE;
    }

    state->bAcquireCastFrameFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 CAST_ReleaseData(SND_CARD_STATE_S* pCard, HI_U32 hCast,
                        AO_Cast_Data_Param_S* pstCastData)
{
    SND_CAST_STATE_S* state = HI_NULL;
    //HI_HANDLE hSndOp;
    HI_S32 Ret;
    HI_S32 s32CastID;

    //TOCHECK CURRENT state     //TODO   Start state

    s32CastID = CastGetIDbyHandle(pCard, hCast);
    if (HI_FAILURE == s32CastID)
    {
        HI_ERR_AO("CastGetIDbyHandle  Failed\n");
        return HI_FAILURE;
    }

    if (HI_NULL == pCard->pCastOp[s32CastID])
    {
        HI_ERR_AO("  hSndOp=%p\n", pCard->pCastOp[s32CastID]);
        return HI_FAILURE;
    }

    state = (SND_CAST_STATE_S*)pCard->pCast[hCast];
    if (!state)
    {
        HI_ERR_AO("SND_CAST_STATE_S  state =%p\n", state);
        return HI_FAILURE;
    }

    /* discard ReleaseCastFrame before call AcquireCastFrame */
    if (HI_FALSE == state->bAcquireCastFrameFlag)
    {
        return HI_SUCCESS;
    }

    pstCastData->u32FrameBytes  = state->u32FrameBytes;
    pstCastData->u32SampleBytes = state->u32SampleBytes;

#if 0
    HI_ERR_AO("state hCast=0x%x\n", state->hCast);
    HI_ERR_AO("state CastId=0x%x\n", state->CastId);
    HI_ERR_AO("pstCastData->u32FrameBytes=0x%x\n", pstCastData->u32FrameBytes);
    HI_ERR_AO("pstCastData->u32SampleBytes=0x%x\n", pstCastData->u32SampleBytes);
#endif


    Ret = SND_ReleaseCastData(pCard,  s32CastID, pstCastData);
    if (HI_FAILURE == Ret)
    {
        return HI_FAILURE;
    }

    state->bAcquireCastFrameFlag = HI_FALSE;

    return HI_SUCCESS;

}

