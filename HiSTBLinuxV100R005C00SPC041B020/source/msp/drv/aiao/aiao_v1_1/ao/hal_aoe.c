/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hal_aiao.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/
#include <linux/string.h>

#include "hi_type.h"

#include "hal_aoe.h"
#include "hal_aoe_func.h"

#include "circ_buf.h"
#include "audio_util.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct
{
    HI_VOID* pAip[AOE_AIP_BUTT];
    HI_VOID* pAop[AOE_AOP_BUTT];
    HI_VOID* pMix[AOE_ENGINE_BUTT];
} AOE_GLOBAL_SOURCE_S;

typedef struct
{
    HI_U32 uTotalByteWrite;
    HI_U32 uTryWriteCnt;
} AOE_AIP_PROC_STATUS_S;

typedef struct
{
    AOE_AIP_CHN_ATTR_S stUserAttr;

    /* internal state */
    AOE_AIP_ID_E aip;
    HI_U32                u32BufFrameSize;
    HI_U32                u32FiFoFrameSize;
    HI_S32                s32AdjSpeed;
    HI_U32                u32LVolumedB;
    HI_U32                u32RVolumedB;
    HI_BOOL               bMute;
    HI_U32                u32ChannelMode;
    AOE_AIP_STATUS_E      enCurnStatus;
    CIRC_BUF_S            stCB;
    AOE_AIP_PROC_STATUS_S stProc;
    HI_BOOL  bFifoBypass;
#ifdef __DPT__
    HI_UNF_AI_NR_ATTR_S        stNR;
#endif
} AOE_AIP_CHN_STATE_S;

/* private state */
static AOE_GLOBAL_SOURCE_S g_AoeRm;

#define CHECK_AIP_OPEN(AIP) \
    do { \
        if (HI_NULL == g_AoeRm.pAip[AIP]) \
        { \
            HI_ERR_AO("aip (%d) is not create.\n", AIP); \
            return HI_FAILURE; \
        } \
    } while (0)

static HI_VOID AOEAIPFlushState(AOE_AIP_CHN_STATE_S* state)
{
    state->s32AdjSpeed = 0;
    memset(&state->stProc, 0, sizeof(AOE_AIP_PROC_STATUS_S));
    iHAL_AOE_AIP_SetSpeed(state->aip, state->s32AdjSpeed);
}

/* global function */
HI_S32 HAL_AOE_Init(HI_BOOL bSwAoeFlag)
{
    AOE_AIP_ID_E aip;
    AOE_AOP_ID_E aop;
    AOE_ENGINE_ID_E engine;

    /* init rm */
    for (aip = AOE_AIP0; aip < AOE_AIP_BUTT; aip++)
    {
        g_AoeRm.pAip[aip] = HI_NULL;
    }

    for (aop = AOE_AOP0; aop < AOE_AOP_BUTT; aop++)
    {
        g_AoeRm.pAop[aop] = HI_NULL;
    }

    for (engine = AOE_ENGINE0; engine < AOE_ENGINE_BUTT; engine++)
    {
        g_AoeRm.pMix[engine] = HI_NULL;
    }

    return iHAL_AOE_Init(bSwAoeFlag);
}

HI_VOID HAL_AOE_DeInit(HI_VOID)
{
    AOE_AIP_ID_E aip;
    AOE_AOP_ID_E aop;
    AOE_ENGINE_ID_E engine;

    /* init rm */
    for (aip = AOE_AIP0; aip < AOE_AIP_BUTT; aip++)
    {
        if (g_AoeRm.pAip[aip])
        {
            iHAL_AOE_AIP_Destroy(aip);
        }
        g_AoeRm.pAip[aip] = HI_NULL;
    }

    for (aop = AOE_AOP0; aop < AOE_AOP_BUTT; aop++)
    {
        if (g_AoeRm.pAop[aop])
        {
            iHAL_AOE_AOP_Destroy(aop);
        }
        g_AoeRm.pAop[aop] = HI_NULL;
    }

    for (engine = AOE_ENGINE0; engine < AOE_ENGINE_BUTT; engine++)
    {
        if (g_AoeRm.pMix[engine])
        {
            iHAL_AOE_ENGINE_Destroy(engine);
        }
        g_AoeRm.pMix[engine] = HI_NULL;
    }

    iHAL_AOE_DeInit();
}

static AOE_AIP_ID_E  AOEGetFreeAIP(HI_VOID)
{
    AOE_AIP_ID_E enFreeAip;

    for (enFreeAip = AOE_AIP0; enFreeAip < AOE_AIP_BUTT; enFreeAip++)
    {
        if (!g_AoeRm.pAip[enFreeAip])
        {
            return enFreeAip;
        }
    }
    return AOE_AIP_BUTT;
}

static AOE_AOP_ID_E  AOEGetFreeAOP(HI_VOID)
{
    AOE_AOP_ID_E enFreeAop;

    for (enFreeAop = AOE_AOP0; enFreeAop < AOE_AOP_BUTT; enFreeAop++)
    {
        if (!g_AoeRm.pAop[enFreeAop])
        {
            return enFreeAop;
        }
    }
    return AOE_AOP_BUTT;
}

static HI_U32 UTIL_CalcFrameSize(HI_U32 uCh, HI_U32 uBitDepth)
{
    HI_U32 uFrameSize = 0;

    switch (uBitDepth)
    {
        case 16:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U16);
            break;
        case 24:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U32);
            break;
        default:
            break;
    }

    return uFrameSize;
}

HI_S32  HAL_AOE_AIP_Create(AOE_AIP_ID_E* penAIP, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    AOE_AIP_ID_E enAIP;
    HI_S32 Ret = HI_FAILURE;

    enAIP = AOEGetFreeAIP();
    if (AOE_AIP_BUTT == enAIP)
    {
        HI_ERR_AO("Get free Aip failed!\n");
        return HI_FAILURE;
    }

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(AOE_AIP_CHN_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc AOE_AIP_CHN_STATE_S failed\n");
        goto AIP_Create_ERR_EXIT;
    }

    memset(state, 0, sizeof(AOE_AIP_CHN_STATE_S));
    g_AoeRm.pAip[enAIP] = (HI_VOID*)state;
    state->bFifoBypass = HI_FALSE;

    if (HI_SUCCESS != (Ret = HAL_AOE_AIP_SetAttr(enAIP, pstAttr)))
    {
        HI_ERR_AO("HAL_AOE_AIP_SetAttr failed!\n");
        goto AIP_Create_ERR_EXIT;
    }

    state->u32LVolumedB = AOE_AIP_VOL_0dB;
    state->u32RVolumedB = AOE_AIP_VOL_0dB;
    iHAL_AOE_AIP_SetLRVolume(enAIP, AOE_AIP_VOL_0dB, AOE_AIP_VOL_0dB);
    iHAL_AOE_AIP_SetPrescale(enAIP, AOE_AIP_VOL_0dB, 0);
    state->bMute = HI_FALSE;
    iHAL_AOE_AIP_SetMute(enAIP, HI_FALSE);
    state->u32ChannelMode = AIP_CHANNEL_MODE_STEREO;
    iHAL_AOE_AIP_SetChannelMode(enAIP, AIP_CHANNEL_MODE_STEREO);
    state->enCurnStatus = AOE_AIP_STATUS_STOP;
    state->aip = enAIP;

    *penAIP = enAIP;
    return HI_SUCCESS;

AIP_Create_ERR_EXIT:
    *penAIP = AOE_AIP_BUTT;
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pAip[enAIP] = HI_NULL;
    return Ret;
}

HI_VOID HAL_AOE_AIP_Destroy(AOE_AIP_ID_E enAIP)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        return;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_STOP != state->enCurnStatus)
    {
        HAL_AOE_AIP_Stop(enAIP);
    }

    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pAip[enAIP] = HI_NULL;
}


HI_S32 HAL_AOE_AIP_SetAttr(AOE_AIP_ID_E enAIP, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    HI_U32* pu32WptrAddr, *pu32RptrAddr;
    AOE_AIP_INBUF_ATTR_S* pInAttr;
    AOE_AIP_OUTFIFO_ATTR_S* pOuAttr;

    CHECK_AIP_OPEN(enAIP);

    if (HI_NULL == pstAttr)
    {
        HI_FATAL_AO("pstAttr is null\n");
        return HI_FAILURE;
    }
    pInAttr  = &pstAttr->stBufInAttr;
    pOuAttr  = &pstAttr->stFifoOutAttr;
    if (!pInAttr->stRbfAttr.tBufPhyAddr || !pInAttr->stRbfAttr.tBufVirAddr)
    {
        HI_FATAL_AO("BufPhyAddr(0x%x) BufVirAddr(0x%x) invalid\n", pInAttr->stRbfAttr.tBufPhyAddr, pInAttr->stRbfAttr.tBufVirAddr);
        return HI_FAILURE;
    }
    if (!pInAttr->stRbfAttr.u32BufSize)
    {
        HI_FATAL_AO("BufSize(0x%x) invalid\n", pInAttr->stRbfAttr.u32BufSize);
        return HI_FAILURE;
    }

    if (pOuAttr->u32FiFoLatencyThdMs < AOE_AIP_FIFO_LATENCYMS_MIN)
    {
        HI_FATAL_AO("FiFoLatencyThdMs(%d) is less than min(%d)\n", pOuAttr->u32FiFoLatencyThdMs, AOE_AIP_FIFO_LATENCYMS_MIN);
        return HI_FAILURE;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    if (AOE_AIP_STATUS_STOP != state->enCurnStatus)
    {
        return HI_FAILURE;
    }

    pstAttr->bFifoBypass = state->bFifoBypass;
    Ret = iHAL_AOE_AIP_SetAttr(enAIP, pstAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    if (!pstAttr->stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        iHAL_AOE_AIP_GetRptrAndWptrRegAddr(enAIP, &pu32WptrAddr, &pu32RptrAddr);
        CIRC_BUF_Init(&state->stCB,
                      pu32WptrAddr,
                      pu32RptrAddr,
                      (HI_U32*)pstAttr->stBufInAttr.stRbfAttr.tBufVirAddr,
                      pstAttr->stBufInAttr.stRbfAttr.u32BufSize);
    }

    state->u32BufFrameSize  = UTIL_CalcFrameSize(pstAttr->stBufInAttr.u32BufChannels,
                              pstAttr->stBufInAttr.u32BufBitPerSample);
    state->u32FiFoFrameSize = UTIL_CalcFrameSize(pstAttr->stFifoOutAttr.u32FifoChannels,
                              pstAttr->stFifoOutAttr.u32FifoBitPerSample);

    state->s32AdjSpeed = 0;
    iHAL_AOE_AIP_SetSpeed(enAIP, 0);

    memset(&state->stProc, 0, sizeof(AOE_AIP_PROC_STATUS_S));
    memcpy(&state->stUserAttr, pstAttr, sizeof(AOE_AIP_CHN_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_GetAttr(AOE_AIP_ID_E enAIP, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    memcpy(pstAttr, &state->stUserAttr, sizeof(AOE_AIP_CHN_ATTR_S));
    return HI_SUCCESS;
}

HI_U32 HAL_AOE_AIP_GetRegAddr(AOE_AIP_ID_E enAIP)
{
    return iHAL_AOE_AIP_GetRegAddr(enAIP);
}

HI_S32 HAL_AOE_AIP_Start(AOE_AIP_ID_E enAIP)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AIP_SetCmd(enAIP, AOE_AIP_CMD_START);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_AIP_STATUS_START;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_Stop(AOE_AIP_ID_E enAIP)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AIP_SetCmd(enAIP, AOE_AIP_CMD_STOP);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    AOEAIPFlushState(state);
    state->enCurnStatus = AOE_AIP_STATUS_STOP;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_Group_Stop(HI_U32 u32StopMask)
{
    HI_U32 u32AipId = 0;
    HI_U32 u32CmdDone = 0;
    AOE_AIP_CHN_STATE_S* state[AOE_MAX_AIP_NUM] = {HI_NULL};

    u32CmdDone = iHAL_AOE_AIP_Group_SetCmd(AOE_AIP_CMD_STOP, u32StopMask);

    for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
    {
        if ((1 << u32AipId) & u32StopMask)
        {
            if ((1 << u32AipId) & u32CmdDone)
            {
                state[u32AipId] = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[u32AipId];
                AOEAIPFlushState(state[u32AipId]);
                state[u32AipId]->enCurnStatus = AOE_AIP_STATUS_STOP;
            }
            else
            {
                HI_ERR_AO("Err: AIP(%d) track stop failed!\n", u32AipId);
            }
        }
    }

    if (u32CmdDone == u32StopMask)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 HAL_AOE_AIP_Pause(AOE_AIP_ID_E enAIP)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_PAUSE == state->enCurnStatus ||
        AOE_AIP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AIP_SetCmd(enAIP, AOE_AIP_CMD_PAUSE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_AIP_STATUS_PAUSE;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_Flush(AOE_AIP_ID_E enAIP)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_START != state->enCurnStatus &&
        AOE_AIP_STATUS_PAUSE != state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AIP_SetCmd(enAIP, AOE_AIP_CMD_FLUSH);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_SetLRVolume(AOE_AIP_ID_E enAIP, HI_U32 u32VolumeLdB, HI_U32 u32VolumeRdB)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    iHAL_AOE_AIP_SetLRVolume(enAIP, u32VolumeLdB, u32VolumeRdB);

    state->u32LVolumedB = u32VolumeLdB;
    state->u32RVolumedB = u32VolumeRdB;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_SetPrescale(AOE_AIP_ID_E enAIP, HI_U32 u32IntdB, HI_S32 s32DecdB)
{
    CHECK_AIP_OPEN(enAIP);

    iHAL_AOE_AIP_SetPrescale(enAIP, u32IntdB, s32DecdB);

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_SetMute(AOE_AIP_ID_E enAIP, HI_BOOL bMute)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    iHAL_AOE_AIP_SetMute(enAIP, bMute);

    state->bMute = bMute;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_SetChannelMode(AOE_AIP_ID_E enAIP, HI_U32 u32ChannelMode)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    iHAL_AOE_AIP_SetChannelMode(enAIP, u32ChannelMode);

    state->u32ChannelMode = u32ChannelMode;

    return HI_SUCCESS;
}

#ifdef __DPT__
HI_S32 HAL_AOE_AIP_SetNRAttr(AOE_AIP_ID_E enAIP, HI_UNF_AI_NR_ATTR_S* pstNRParam)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    iHAL_AOE_AIP_SetNRAttr(enAIP, pstNRParam);
    memcpy(&state->stNR, pstNRParam, sizeof(HI_UNF_AI_NR_ATTR_S));

    return HI_SUCCESS;
}
#endif

HI_S32 HAL_AOE_AIP_SetSpeed(AOE_AIP_ID_E enAIP, HI_S32 s32AdjSpeed)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    iHAL_AOE_AIP_SetSpeed(enAIP, s32AdjSpeed);
    state->s32AdjSpeed = s32AdjSpeed;
    return HI_SUCCESS;
}

HI_U32 HAL_AOE_AIP_QueryBufData(AOE_AIP_ID_E enAIP)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (AOE_AIP_STATUS_STOP == state->enCurnStatus)
    {
        return 0;
    }

    if (state->stUserAttr.stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        HI_WARN_AO("dont support AIP_QueryBufData whent u32BufWptrRptrFlag(1)\n");
        return 0;
    }

    return CIRC_BUF_QueryBusy(&state->stCB);
}

HI_U32 HAL_AOE_AIP_QueryBufFree(AOE_AIP_ID_E enAIP)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    if (state->stUserAttr.stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        HI_WARN_AO("dont support AIP_QueryBufFree whent u32BufWptrRptrFlag(1)\n");
        return 0;
    }

    return CIRC_BUF_QueryFree(&state->stCB);
}

HI_U32 HAL_AOE_AIP_WriteBufData(AOE_AIP_ID_E enAIP, HI_U8* pu32Src, HI_U32 u32SrcBytes)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    HI_U32 Bytes;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    state->stProc.uTryWriteCnt++;
    if (state->stUserAttr.stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        HI_WARN_AO("dont support AIP_QueryBufFree whent u32BufWptrRptrFlag(1)\n");
        return 0;
    }

    Bytes = CIRC_BUF_Write(&state->stCB, pu32Src, u32SrcBytes);
    state->stProc.uTotalByteWrite += Bytes;
    return Bytes;
}

//for ALSA
HI_U32 HAL_AOE_AIP_UpdateWritePos(AOE_AIP_ID_E enAIP, HI_U32* pu32WptrLen)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    return CIRC_BUF_ALSA_UpdateWptr(&state->stCB, *pu32WptrLen);
}

HI_U32 HAL_AOE_AIP_ChangeReadPos(AOE_AIP_ID_E enAIP, HI_U32 u32RptrLen)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    return CIRC_BUF_ChangeRptr(&state->stCB, u32RptrLen);
}

//for ALSA
HI_U32 HAL_AOE_AIP_UpdateReadPos(AOE_AIP_ID_E enAIP, HI_U32* pu32RptrLen)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    return CIRC_BUF_ALSA_UpdateRptr(&state->stCB, *pu32RptrLen);
}

//for ALSA
HI_U32 HAL_AOE_AIP_GetReadPos(AOE_AIP_ID_E enAIP, HI_U32* pu32ReadPos)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    *pu32ReadPos = CIRC_BUF_QueryReadPos(&state->stCB);

    return 0;
}

//for ALSA
HI_U32 HAL_AOE_AIP_FlushBuf(AOE_AIP_ID_E enAIP)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        HI_WARN_AO("aip (%d) is not create.\n", enAIP);
        return 0;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    CIRC_BUF_Flush(&state->stCB);

    return 0;
}

HI_VOID HAL_AOE_AIP_GetBufDelayMs(AOE_AIP_ID_E enAIP, HI_U32* pDelayms)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    HI_U32 FreeBytes = 0;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        *pDelayms = 0;
        return;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (state->stUserAttr.stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        HI_WARN_AO("dont support AIP_GetBufDelayMs whent u32BufWptrRptrFlag(1)\n");
        *pDelayms = 0;
        return;
    }

    FreeBytes = CIRC_BUF_QueryBusy(&state->stCB);
    *pDelayms = CALC_LATENCY_MS(state->stUserAttr.stBufInAttr.u32BufSampleRate, state->u32BufFrameSize, FreeBytes);
}

HI_VOID HAL_AOE_AIP_GetFiFoDelayMs(AOE_AIP_ID_E enAIP, HI_U32* pDelayms)
{
    AOE_AIP_CHN_STATE_S* state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    if (HI_NULL == state)
    {
        *pDelayms = 0;
        return;
    }

    if (AOE_AIP_STATUS_STOP == state->enCurnStatus)
    {
        *pDelayms = 0;
        return;
    }

    *pDelayms = iHAL_AOE_AIP_GetFiFoDelayMs(enAIP);
}

HI_VOID HAL_AOE_AIP_GetStatus(AOE_AIP_ID_E enAIP, AOE_AIP_STATUS_E* peStatus)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        *peStatus = AOE_AIP_STATUS_STOP;
        return;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    *peStatus = state->enCurnStatus;
}

HI_S32 HAL_AOE_AIP_SetFiFoBypass(AOE_AIP_ID_E enAIP, HI_BOOL bEnable)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret;

    CHECK_AIP_OPEN(enAIP);
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    Ret = iHAL_AOE_AIP_SetFifoBypass(enAIP, bEnable);
    if (HI_SUCCESS == Ret)
    {
        state->bFifoBypass = bEnable;
    }
    return Ret;
}

HI_S32 HAL_AOE_AIP_GetFiFoBypass(AOE_AIP_ID_E enAIP, HI_BOOL* pbEnable)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        return HI_FAILURE;
    }

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    *pbEnable =  state->bFifoBypass;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_GetMixPriority(AOE_AIP_ID_E enAIP, HI_BOOL* bPriority)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        return HI_FAILURE;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    *bPriority =  state->stUserAttr.stBufInAttr.bMixPriority;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AIP_SetMixPriority(AOE_AIP_ID_E enAIP, HI_BOOL bPriority)
{
    HI_S32 Ret;
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    if (HI_NULL == g_AoeRm.pAip[enAIP])
    {
        return HI_FAILURE;
    }
    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];
    Ret = iHAL_AOE_AIP_SetMixPriority(enAIP, bPriority);
    if (HI_SUCCESS == Ret)
    {
        state->stUserAttr.stBufInAttr.bMixPriority = bPriority;
    }
    else
    {
        HI_ERR_AO("HAL_AOE_AIP_SetMixPriority Failed !\n");
    }
    return Ret;
}

HI_S32 HAL_AOE_AIP_AttachAop(AOE_AIP_ID_E enAIP, AOE_AOP_ID_E enAOP, HI_U32 u32SkipMs)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    CHECK_AIP_OPEN(enAIP);

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    return iHAL_AOE_AIP_AttachAop(enAIP, enAOP, u32SkipMs);
}

HI_S32 HAL_AOE_AIP_DetachAop(AOE_AIP_ID_E enAIP, AOE_AOP_ID_E enAOP)
{
    AOE_AIP_CHN_STATE_S* state = HI_NULL;
    CHECK_AIP_OPEN(enAIP);

    state = (AOE_AIP_CHN_STATE_S*)g_AoeRm.pAip[enAIP];

    return iHAL_AOE_AIP_DetachAop(enAIP, enAOP);
}

/*aop func*/
typedef struct
{
    AOE_AOP_CHN_ATTR_S stUserAttr;
    HI_U32                u32LVolumedB;
    HI_U32                u32RVolumedB;
    HI_BOOL               bMute;
#ifdef __DPT__
    HI_BOOL               bDrcEnable;
    HI_UNF_SND_DRC_ATTR_S stDrcAttr;

    HI_BOOL               bPeqEnable;
    HI_UNF_SND_PEQ_ATTR_S stPeqAttr;

    HI_BOOL               bADOutputEnable;
#endif
    /* internal state */
    AOE_AOP_ID_E enAop;
    AOE_AOP_STATUS_E      enCurnStatus;
    HI_BOOL               bBypass;
    HI_U32                u32DelayMs;
} AOE_AOP_CHN_STATE_S;

#define CHECK_AOP_OPEN(AOP) \
    do {\
        if (HI_NULL == g_AoeRm.pAop[AOP])\
        {\
            HI_ERR_AO("aop (%d) is not create.\n", AOP); \
            return HI_FAILURE; \
        } \
    } while (0)

HI_S32 HAL_AOE_AOP_SetDelay(AOE_AOP_ID_E enAOP, HI_U32 u32RealDelayMs)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    iHAL_AOE_AOP_SetDelay(enAOP, u32RealDelayMs);

    state->u32DelayMs = u32RealDelayMs;

    HI_INFO_AO("Set Aop(0x%x) DelayMs %d\n", enAOP, u32RealDelayMs);
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_GetDelay(AOE_AOP_ID_E enAOP, HI_U32* pu32RealDelayMs)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    iHAL_AOE_AOP_GetDelay(enAOP, pu32RealDelayMs);

    HI_INFO_AO("Get Aop(0x%x) DelayMs %d\n", enAOP, *pu32RealDelayMs);
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetMute(AOE_AOP_ID_E enAOP, HI_BOOL bMute)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    iHAL_AOE_AOP_SetMute(enAOP, bMute);

    state->bMute = bMute;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetLRVolume(AOE_AOP_ID_E enAOP, HI_U32 u32VolumeLdB, HI_U32 u32VolumeRdB)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    iHAL_AOE_AOP_SetLRVolume(enAOP, u32VolumeLdB, u32VolumeRdB);

    state->u32LVolumedB = u32VolumeLdB;
    state->u32RVolumedB = u32VolumeRdB;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetPreciVol(AOE_AOP_ID_E enAOP, HI_U32 u32IntdB, HI_S32 s32DecdB)
{
    CHECK_AOP_OPEN(enAOP);

    iHAL_AOE_AOP_SetPreciVol(enAOP, u32IntdB, s32DecdB);

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetBalance(AOE_AOP_ID_E enAOP, HI_S32 s32Balance)
{
    CHECK_AOP_OPEN(enAOP);

    iHAL_AOE_AOP_SetBalance(enAOP, s32Balance);

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetAttr(AOE_AOP_ID_E enAOP, AOE_AOP_CHN_ATTR_S* pstAttr)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    AOE_AOP_OUTBUF_ATTR_S* pOuAttr;

    CHECK_AOP_OPEN(enAOP);

    if (HI_NULL == pstAttr)
    {
        HI_FATAL_AO("pstAttr is null\n");
        return HI_FAILURE;
    }
    state = (AOE_AOP_CHN_STATE_S* )g_AoeRm.pAop[enAOP];
    pOuAttr  = &pstAttr->stRbfOutAttr;
    if (!pOuAttr->stRbfAttr.tBufPhyAddr || !pOuAttr->stRbfAttr.tBufVirAddr)
    {
        HI_FATAL_AO("BufPhyAddr(0x%x) BufVirAddr(0x%x) invalid\n", pOuAttr->stRbfAttr.tBufPhyAddr, pOuAttr->stRbfAttr.tBufVirAddr);
        return HI_FAILURE;
    }
    if (!pOuAttr->stRbfAttr.u32BufSize)
    {
        HI_FATAL_AO("BufSize(0x%x) invalid\n", pOuAttr->stRbfAttr.u32BufSize);
        return HI_FAILURE;
    }

    if (pOuAttr->u32BufLatencyThdMs < AOE_AOP_BUFF_LATENCYMS_MIN)
    {
        HI_FATAL_AO("FiFoLatencyThdMs(%d) is less than min(%d)\n", pOuAttr->u32BufLatencyThdMs, AOE_AOP_BUFF_LATENCYMS_MIN);
        return HI_FAILURE;
    }

    Ret = iHAL_AOE_AOP_SetAttr(enAOP, pstAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("call iHAL_AOE_AOP_SetAttr failed!\n");
        return Ret;
    }

    iHAL_AOE_AOP_SetDelay(enAOP, 0);

    memcpy(&state->stUserAttr, pstAttr, sizeof(AOE_AOP_CHN_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_Create(AOE_AOP_ID_E* penAOP, AOE_AOP_CHN_ATTR_S* pstAttr)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    AOE_AOP_ID_E enAOP;
    HI_S32 Ret = HI_FAILURE;

    enAOP = AOEGetFreeAOP();
    if (AOE_AOP_BUTT == enAOP)
    {
        HI_ERR_AO("call AOEGetFreeAOP failed!\n");
        return HI_FAILURE;
    }

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(AOE_AOP_CHN_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc AOE_AOP_CHN_ATTR_S failed\n");
        goto AOP_Create_ERR_EXIT;
    }

    memset(state, 0, sizeof(AOE_AOP_CHN_STATE_S));
    g_AoeRm.pAop[enAOP] = (HI_VOID*)state;

    if (HI_SUCCESS != (Ret = HAL_AOE_AOP_SetAttr(enAOP, pstAttr)))
    {
        HI_ERR_AO("call HAL_AOE_AOP_SetAttr failed! Ret = %d\n", Ret);
        goto AOP_Create_ERR_EXIT;
    }

    state->bBypass = 0;
    state->enCurnStatus = AOE_AOP_STATUS_STOP;
    state->enAop = enAOP;

    state->u32LVolumedB = AOE_AIP_VOL_0dB;
    state->u32RVolumedB = AOE_AIP_VOL_0dB;
    iHAL_AOE_AOP_SetLRVolume(enAOP, AOE_AIP_VOL_0dB, AOE_AIP_VOL_0dB);
    state->bMute = HI_FALSE;
    iHAL_AOE_AOP_SetMute(enAOP, HI_FALSE);

    iHAL_AOE_AOP_SetDelay(enAOP, 0);

    *penAOP = enAOP;
    return HI_SUCCESS;

AOP_Create_ERR_EXIT:
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pAop[enAOP] = HI_NULL;
    *penAOP = AOE_AOP_BUTT;

    return Ret;
}

HI_VOID HAL_AOE_AOP_Destroy(AOE_AOP_ID_E enAOP)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pAop[enAOP])
    {
        return;
    }
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pAop[enAOP] = HI_NULL;
}

HI_S32 HAL_AOE_AOP_GetAttr(AOE_AOP_ID_E enAOP, AOE_AOP_CHN_ATTR_S* pstAttr)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);

    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    memcpy(pstAttr, &state->stUserAttr, sizeof(AOE_AOP_CHN_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_Start(AOE_AOP_ID_E enAOP)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    if (AOE_AOP_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AOP_SetCmd(enAOP, AOE_AOP_CMD_START);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_AOP_STATUS_START;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_Stop(AOE_AOP_ID_E enAOP)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    if (AOE_AOP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_AOP_SetCmd(enAOP, AOE_AOP_CMD_STOP);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_AOP_STATUS_STOP;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetLowLatency(AOE_AOP_ID_E enAOP, HI_U32 u32Latency)
{
    CHECK_AOP_OPEN(enAOP);
    return iHAL_AOE_AOP_SetLatency(enAOP, u32Latency);
}

HI_S32 HAL_AOE_AOP_GetLowLatency(AOE_AOP_ID_E enAOP, HI_U32* pu32Latency)
{
    CHECK_AOP_OPEN(enAOP);
    return iHAL_AOE_AOP_GetLatency(enAOP, pu32Latency);
}

HI_S32 HAL_AOE_AOP_SetAefBypass(AOE_AOP_ID_E enAOP, HI_BOOL bBypass)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    Ret = iHAL_AOE_AOP_SetAefBypass(enAOP, bBypass);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->bBypass = bBypass;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_GetStatus(AOE_AOP_ID_E enAOP, HI_VOID* pstStatus)
{
    return HI_SUCCESS;
}

/* ENGINE function */

typedef struct
{
    AOE_ENGINE_CHN_ATTR_S stUserAttr;

    /* internal state */
    AOE_ENGINE_ID_E enMix;
    AOE_ENGINE_STATUS_E enCurnStatus;
} AOE_ENGINE_CHN_STATE_S;

AOE_ENGINE_ID_E AOEGetFreeEngine(HI_VOID)
{
    AOE_ENGINE_ID_E enFreeEngine;

    for (enFreeEngine = AOE_ENGINE0; enFreeEngine < AOE_ENGINE_BUTT; enFreeEngine++)
    {
        if (!g_AoeRm.pMix[enFreeEngine])
        {
            return enFreeEngine;
        }
    }
    return AOE_ENGINE_BUTT;
}

#define CHECK_ENGINE_OPEN(ENGINE) \
    do {\
        if (HI_NULL == g_AoeRm.pMix[ENGINE])\
        {\
            HI_ERR_AO("engine (%d) is not create.\n", ENGINE); \
            return HI_FAILURE; \
        } \
    } while (0)


HI_S32 HAL_AOE_ENGINE_Create(AOE_ENGINE_ID_E* penENGINE, AOE_ENGINE_CHN_ATTR_S* pstAttr)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    AOE_ENGINE_ID_E enEngine;
    HI_S32 Ret = HI_FAILURE;

    enEngine = AOEGetFreeEngine();
    if (AOE_ENGINE_BUTT == enEngine)
    {
        HI_ERR_AO("Get free engine failed!\n");
        return HI_FAILURE;
    }

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(AOE_ENGINE_CHN_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc AOE_ENGINE_CHN_STATE_S failed\n");
        goto Engine_Create_ERR_EXIT;
    }

    memset(state, 0, sizeof(AOE_ENGINE_CHN_STATE_S));
    g_AoeRm.pMix[enEngine] = (HI_VOID*)state;
    if (HI_SUCCESS != (Ret = HAL_AOE_ENGINE_SetAttr(enEngine, pstAttr)))
    {
        goto Engine_Create_ERR_EXIT;
    }

    state->enCurnStatus = AOE_ENGINE_STATUS_STOP;
    state->enMix = enEngine;

    *penENGINE = enEngine;
    return HI_SUCCESS;

Engine_Create_ERR_EXIT:
    *penENGINE = AOE_ENGINE_BUTT;
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pMix[enEngine] = HI_NULL;

    return Ret;
}

HI_VOID HAL_AOE_ENGINE_Destroy(AOE_ENGINE_ID_E enENGINE)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;

    if (HI_NULL == g_AoeRm.pMix[enENGINE])
    {
        return;
    }
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    g_AoeRm.pMix[enENGINE] = HI_NULL;
}

HI_S32 HAL_AOE_ENGINE_SetAttr(AOE_ENGINE_ID_E enENGINE, AOE_ENGINE_CHN_ATTR_S* pstAttr)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);
    if (HI_NULL == pstAttr)
    {
        HI_FATAL_AO("pstAttr is null\n");
        return HI_FAILURE;
    }

    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];
    Ret = iHAL_AOE_ENGINE_SetAttr(enENGINE, pstAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    memcpy(&state->stUserAttr, pstAttr, sizeof(AOE_ENGINE_CHN_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_GetAttr(AOE_ENGINE_ID_E enENGINE, AOE_ENGINE_CHN_ATTR_S* pstAttr)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;

    CHECK_ENGINE_OPEN(enENGINE);

    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    memcpy(pstAttr, &state->stUserAttr, sizeof(AOE_ENGINE_CHN_ATTR_S));
    return HI_SUCCESS;
}

#ifdef __DPT__
HI_S32 HAL_AOE_ENGINE_SetAvcAttr(AOE_ENGINE_ID_E enENGINE, HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    CHECK_ENGINE_OPEN(enENGINE);
    if (HI_NULL == pstAvcAttr)
    {
        HI_FATAL_AO("pstAvcAttr is null\n");
        return HI_FAILURE;
    }
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];
    Ret = iHAL_AOE_ENGINE_SetAvcAttr(enENGINE, pstAvcAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    Ret = iHAL_AOE_AOP_SetAvcChangeAttr(enENGINE, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetAvcEnable(AOE_ENGINE_ID_E enENGINE, HI_BOOL bEnable)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    CHECK_ENGINE_OPEN(enENGINE);
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];
    Ret = iHAL_AOE_ENGINE_SetAvcEnable(enENGINE, bEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    Ret = iHAL_AOE_AOP_SetAvcChangeAttr(enENGINE, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetGeqAttr(AOE_ENGINE_ID_E enENGINE, HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);
    if (HI_NULL == pstGeqAttr)
    {
        HI_FATAL_AO("pstGeqAttr is null\n");
        return HI_FAILURE;
    }

    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];
    Ret = iHAL_AOE_ENGINE_SetGeqAttr(enENGINE, pstGeqAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetGeqChangeAttr(enENGINE, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetGeqEnable(AOE_ENGINE_ID_E enENGINE, HI_BOOL bEnable)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);

    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    Ret = iHAL_AOE_ENGINE_SetGeqEnable(enENGINE, bEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetGeqChangeAttr(enENGINE, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetGeqGain(AOE_ENGINE_ID_E enENGINE, HI_U32 u32Band, HI_S32 s32Gain)
{
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);

    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    Ret = iHAL_AOE_ENGINE_SetGeqGain(enENGINE, u32Band, s32Gain);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetGeqChangeAttr(enENGINE, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_GetRegAddr(AOE_ENGINE_ID_E enENGINE, HI_PHYS_ADDR_T* ptPhyEngineAddr)
{
    CHECK_ENGINE_OPEN(enENGINE);
    if (HI_NULL == ptPhyEngineAddr)
    {
        HI_FATAL_AO("ptPhyEngineAddr is null\n");
        return HI_FAILURE;
    }

    *ptPhyEngineAddr = (HI_PHYS_ADDR_T)((AOE_COM_REG_BASE + AOE_ENGINE_REG_OFFSET) + AOE_ENGINE_REG_BANDSIZE * (HI_U32)enENGINE);

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetAefInbuf(AOE_ENGINE_ID_E enENGINE, AOE_ENGINE_AEFBUF_ATTR_S* pstAefBuf)
{
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);
    Ret = iHAL_AOE_ENGINE_SetAefInbuf(enENGINE, pstAefBuf);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_SetAefOutbuf(AOE_ENGINE_ID_E enENGINE, AOE_ENGINE_AEFBUF_ATTR_S* pstAefBuf)
{
    HI_S32 Ret = HI_FAILURE;

    CHECK_ENGINE_OPEN(enENGINE);
    Ret = iHAL_AOE_ENGINE_SetAefOutbuf(enENGINE, pstAefBuf);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetDrcEnable(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    Ret = iHAL_AOE_AOP_SetDrcEnable(enAOP, bEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetDrcChangeAttr(enAOP, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->bDrcEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetDrcAttr(AOE_AOP_ID_E enAOP, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_AOP_OPEN(enAOP);
    if (HI_NULL == pstDrcAttr)
    {
        HI_FATAL_AO("pstDrcAttr is null\n");
        return HI_FAILURE;
    }

    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];
    Ret = iHAL_AOE_AOP_SetDrcAttr(enAOP, pstDrcAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetDrcChangeAttr(enAOP, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
    memcpy(&state->stDrcAttr, pstDrcAttr, sizeof(HI_UNF_SND_DRC_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetPeqAttr(AOE_AOP_ID_E enAOP, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_AOP_OPEN(enAOP);
    if (HI_NULL == pstPeqAttr)
    {
        HI_FATAL_AO("pstPeqAttr is null\n");
        return HI_FAILURE;
    }

    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];
    Ret = iHAL_AOE_AOP_SetPeqAttr(enAOP, pstPeqAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetPeqChangeAttr(enAOP, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    memcpy(&state->stPeqAttr, pstPeqAttr, sizeof(HI_UNF_SND_PEQ_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetPeqEnable(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    AOE_AOP_CHN_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;

    CHECK_AOP_OPEN(enAOP);

    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    Ret = iHAL_AOE_AOP_SetPeqEnable(enAOP, bEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = iHAL_AOE_AOP_SetPeqChangeAttr(enAOP, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->bPeqEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_AOP_SetADOutputEnable(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    HI_S32 Ret;
    AOE_AOP_CHN_STATE_S* state = HI_NULL;

    CHECK_AOP_OPEN(enAOP);
    state = (AOE_AOP_CHN_STATE_S*)g_AoeRm.pAop[enAOP];

    Ret = iHAL_AOE_AOP_SetADOutputEnable(enAOP, bEnable);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->bADOutputEnable = bEnable;

    return HI_SUCCESS;
}
#endif

HI_S32 HAL_AOE_ENGINE_Start(AOE_ENGINE_ID_E enENGINE)
{
    HI_S32 Ret;
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;

    CHECK_ENGINE_OPEN(enENGINE);
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    if (AOE_ENGINE_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_ENGINE_SetCmd(enENGINE, AOE_ENGINE_CMD_START);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_ENGINE_STATUS_START;

    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_Stop(AOE_ENGINE_ID_E enENGINE)
{
    HI_S32 Ret;
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;

    CHECK_ENGINE_OPEN(enENGINE);
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    if (AOE_ENGINE_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    Ret = iHAL_AOE_ENGINE_SetCmd(enENGINE, AOE_ENGINE_CMD_STOP);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    state->enCurnStatus = AOE_ENGINE_STATUS_STOP;
    return HI_SUCCESS;
}

HI_S32 HAL_AOE_ENGINE_AttachAip(AOE_ENGINE_ID_E enENGINE, AOE_AIP_ID_E enAIP)
{
    CHECK_ENGINE_OPEN(enENGINE);
    CHECK_AIP_OPEN(enAIP);

    return iHAL_AOE_ENGINE_AttachAip(enENGINE, enAIP);
}

HI_S32 HAL_AOE_ENGINE_DetachAip(AOE_ENGINE_ID_E enENGINE, AOE_AIP_ID_E enAIP)
{
    CHECK_ENGINE_OPEN(enENGINE);
    CHECK_AIP_OPEN(enAIP);

    return iHAL_AOE_ENGINE_DetachAip(enENGINE, enAIP);
}

HI_S32 HAL_AOE_ENGINE_AttachAop(AOE_ENGINE_ID_E enENGINE, AOE_AOP_ID_E enAOP)
{
    CHECK_ENGINE_OPEN(enENGINE);
    CHECK_AOP_OPEN(enAOP);

    return iHAL_AOE_ENGINE_AttachAop(enENGINE, enAOP);
}

HI_S32 HAL_AOE_ENGINE_DetachAop(AOE_ENGINE_ID_E enENGINE, AOE_AOP_ID_E enAOP)
{
    CHECK_ENGINE_OPEN(enENGINE);
    CHECK_AOP_OPEN(enAOP);

    return iHAL_AOE_ENGINE_DetachAop(enENGINE, enAOP);
}

HI_S32 HAL_AOE_ENGINE_GetStatus(AOE_ENGINE_ID_E enENGINE, HI_VOID* pstStatus)
{
    return HI_SUCCESS;
}

#ifdef __DPT__
HI_S32 HAL_AOE_ENGINE_AttachAef(AOE_ENGINE_ID_E enENGINE, HI_U32 u32AefId)
{
    CHECK_ENGINE_OPEN(enENGINE);

    return iHAL_AOE_ENGINE_AttachAef(enENGINE, u32AefId);
}

HI_S32 HAL_AOE_ENGINE_DetachAef(AOE_ENGINE_ID_E enENGINE, HI_U32 u32AefId)
{
    CHECK_ENGINE_OPEN(enENGINE);

    return iHAL_AOE_ENGINE_DetachAef(enENGINE, u32AefId);
}

HI_S32 HAL_AOE_ENGINE_SetAefAttr(AOE_ENGINE_ID_E enENGINE, HI_BOOL bMcProcess, HI_U32 u32FrameDelay)
{
    CHECK_ENGINE_OPEN(enENGINE);

    return iHAL_AOE_ENGINE_SetAefAttr(enENGINE, bMcProcess, u32FrameDelay);
}

HI_S32 HAL_AOE_ENGINE_GetAefDelayMs(AOE_ENGINE_ID_E enENGINE, HI_U32* u32AefDelayMs)
{
    HI_U32 u32DelayFrame = 0;
    AOE_ENGINE_CHN_STATE_S* state = HI_NULL;

    CHECK_ENGINE_OPEN(enENGINE);
    state = (AOE_ENGINE_CHN_STATE_S*)g_AoeRm.pMix[enENGINE];

    iHAL_AOE_ENGINE_GetAefDelayFrame(enENGINE, &u32DelayFrame);

    //Frame Latency -> Ms Latency
    *u32AefDelayMs = u32DelayFrame * AOE_ENGINE_PROC_SAMPLES * 1000 / state->stUserAttr.u32SampleRate;

    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
