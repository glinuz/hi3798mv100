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
#include "hi_module.h"
#include "hi_drv_mmz.h"

#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "drv_ao_op.h"
#include "audio_util.h"

#ifdef __DPT__
#include "hi_audsp_aflt.h"

#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined (CHIP_TYPE_hi3751v500)  \
    || defined(CHIP_TYPE_hi3751v620)
#ifdef HI_BOOT_MUSIC_SUPPORT
#include "hi_drv_pdm.h"
#include "hi_drv_gpio.h"
#endif
#include "hi_board.h"
#endif

#endif /* End of #ifdef __DPT__ */

#if defined(HI_SND_MUTECTL_SUPPORT)
#if    defined(CHIP_TYPE_hi3798mv100)  \
    || defined(CHIP_TYPE_hi3796mv100)  \
    || defined(CHIP_TYPE_hi3716dv100)
#include "hi_drv_sys.h"
#endif
#endif

static HI_S32 SndOpSetDelayCompensation(HI_VOID* pSndOp, HI_U32 u32RealDelayMs);
static HI_S32 SndOpGetDelayCompensation(HI_VOID* pSndOp, HI_U32* pu32RealDelayMs);

static AIAO_PORT_ID_E SndOpGetPort(HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_AOP_TYPE_E enType)
{
    AIAO_PORT_ID_E enPortId;
    AUTIL_CHIP_PLATFORM_E enChipPlatform;

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
            if (AUTIL_CHIP_TYPE_PLATFORM_S28 == AUTIL_GetChipPlatform())
            {
                enPortId = AIAO_PORT_TX1;
            }
            else
            {
                enPortId = AIAO_PORT_TX2;
            }
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC1:
            enPortId = AIAO_PORT_TX3;
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC2:
            enPortId = AIAO_PORT_TX4;
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC3:
            enPortId = AIAO_PORT_TX5;
            break;

        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            enPortId = AIAO_PORT_SPDIF_TX1;
            break;

        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            if (SND_AOP_TYPE_I2S == enType)
            {
                enChipPlatform = AUTIL_GetChipPlatform();
                if (AUTIL_CHIP_TYPE_PLATFORM_S40 == enChipPlatform)
                {
                    enPortId = AIAO_PORT_TX3;
                }
                else if (AUTIL_CHIP_TYPE_PLATFORM_S28 == enChipPlatform)
                {
                    enPortId = AIAO_PORT_TX2;
                }
                else
                {
                    enPortId = AIAO_PORT_TX6;
                }
            }
            else
            {
                enPortId = AIAO_PORT_SPDIF_TX0;
            }
            break;

        default:
            HI_ERR_AO("Outport %d is invalid!\n", (HI_U32)enOutPort);
            enPortId = AIAO_PORT_BUTT;
    }

    return enPortId;
}

HI_UNF_SND_OUTPUTPORT_E SndOpGetOutport(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enOutPort;
}

SND_OUTPUT_TYPE_E SndOpGetOutType(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enOutType;
}

HI_VOID* SNDGetOpHandleByOutPort(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_S32 u32PortNum;
    HI_VOID* pSndOp;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        pSndOp = pCard->pSndOp[u32PortNum];
        if (pSndOp)
        {
#ifdef __DPT__
            if (HI_UNF_SND_OUTPUTPORT_ARC0 == enOutPort)
            {
                // when arc, real OUTPUTPORT must be SPDIF0
                enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
            }
#endif
            if (enOutPort == SndOpGetOutport(pSndOp))
            {
                return pSndOp;
            }
        }
    }

    return HI_NULL;
}

SND_ENGINE_TYPE_E SND_GetOpGetOutType(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enEngineType[state->ActiveId];
}

HI_VOID* SND_GetOpHandlebyOutType(SND_CARD_STATE_S* pCard, SND_OUTPUT_TYPE_E enOutType)
{
    HI_S32 u32PortNum;
    HI_VOID* pSndOp;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        pSndOp = pCard->pSndOp[u32PortNum];
        if (pSndOp)
        {
            if (enOutType == SndOpGetOutType(pSndOp))
            {
                return pSndOp;
            }
        }
    }

    return HI_NULL;
}

HI_VOID SND_GetPortDelayMs(SND_CARD_STATE_S* pCard, SND_OUTPUT_TYPE_E enOutType, HI_U32* pu32DelayMs)
{
    HI_VOID* pSndOp;
    SND_OP_STATE_S* state;

    *pu32DelayMs = 0;

    pSndOp = SND_GetOpHandlebyOutType(pCard, enOutType);
    if (pSndOp)
    {
        state = (SND_OP_STATE_S*)pSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pu32DelayMs);
    }
}

HI_VOID SND_GetDelayMs(SND_CARD_STATE_S* pCard, HI_U32* pdelayms)
{
    HI_VOID* pSndOp;
    SND_OP_STATE_S* state;
    HI_U32 u32DelayCompensationMs = 0;

    pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
    if (pSndOp)
    {
        state = (SND_OP_STATE_S*)pSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        SndOpGetDelayCompensation(pSndOp, &u32DelayCompensationMs);

        if (u32DelayCompensationMs)
        {
            // TODO:
            if (*pdelayms > u32DelayCompensationMs)
            {
                *pdelayms -= u32DelayCompensationMs;
            }
            else
            {
                *pdelayms = 0;
            }
        }

        return;
    }

    pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
    if (pSndOp)
    {
        state = (SND_OP_STATE_S*)pSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        SndOpGetDelayCompensation(pSndOp, &u32DelayCompensationMs);

        if (u32DelayCompensationMs)
        {
            // TODO:
            if(*pdelayms > u32DelayCompensationMs)
            {
                *pdelayms -= u32DelayCompensationMs;
            }
            else
            {
                *pdelayms = 0;
            }
        }

        return;
    }

    pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_SPDIF);
    if (pSndOp)
    {
        state = (SND_OP_STATE_S*)pSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        SndOpGetDelayCompensation(pSndOp, &u32DelayCompensationMs);

        if (u32DelayCompensationMs)
        {
            // TODO:
            if(*pdelayms > u32DelayCompensationMs)
            {
                *pdelayms -= u32DelayCompensationMs;
            }
            else
            {
                *pdelayms = 0;
            }
        }
        return;
    }

    pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
    if (pSndOp)
    {
        state = (SND_OP_STATE_S*)pSndOp;
        HAL_AIAO_P_GetDelayMs(state->enPortID[state->ActiveId], pdelayms);
        return;
    }

    *pdelayms = 0;

    return ;
}

#ifdef __DPT__
static HI_VOID SndOpGetDefDrcAttr(SND_OP_STATE_S* state)
{
    state->bDrcEnable = HI_FALSE;
    state->stDrcAttr.u32AttackTime = 50;
    state->stDrcAttr.u32ReleaseTime = 100;
    state->stDrcAttr.s32Thrhd = -40;
    state->stDrcAttr.s32Limit = -10;
    return;
}

static HI_VOID SndOpGetDefPeqAttr(SND_OP_STATE_S* state)
{
    HI_UNF_SND_PEQ_ATTR_S  stDfPeqAttr =
    {
        5,
        {
            {2, 120, 20, 0},
            {2, 500, 20, 0},
            {2, 1500, 20, 0},
            {2, 5000, 20, 0},
            {2, 10000, 20, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    };
    memcpy(&state->stPeqAttr, &stDfPeqAttr, sizeof(HI_UNF_SND_PEQ_ATTR_S));
    return;
}
#endif

static HI_S32 SndOpCreateAop(SND_OP_STATE_S *state,
                             DRV_SND_OUTPORT_S *pstAttr,
                             SND_AOP_TYPE_E enType,
                             AO_ALSA_I2S_Param_S* pstAoI2sParam,
                             HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    HI_S32 Ret;
    AIAO_PORT_USER_CFG_S stHwPortAttr;
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
    AOE_AOP_ID_E enAOP;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    MMZ_BUFFER_S stRbfMmz;
    HI_U32 u32BufSize;
    AIAO_RBUF_ATTR_S stRbfAttr;
    HI_U32 u32AopId;
    HI_UNF_SND_OUTPUTPORT_E enOutPort;
    HI_BOOL bAlsaI2sUse = HI_FALSE;

    HI_U32 u32AlignInBytes, u32FrameSize;
    u32AlignInBytes = AIAO_BUFFER_ADDR_ALIGN * 2 / 8;    //translate bit to byte

    if (pstAoI2sParam != NULL)
    {
        bAlsaI2sUse  = pstAoI2sParam->bAlsaI2sUse;
    }

    if (enType >= SND_AOP_TYPE_CAST)
    {
        HI_ERR_AO("enType(%d) >= SND_AOP_TYPE_CAST\n", enType);
        goto SndReturn_ERR_EXIT;
    }

    enOutPort = pstAttr->enOutPort;

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
            enPort = SndOpGetPort(enOutPort, enType);
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Adac0", MMZ_OTHERS, AO_DAC_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC1:
            enPort = SndOpGetPort(enOutPort, enType);
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            stHwPortAttr.stIfAttr.eCrgSource = AIAO_TX_CRG2;
            HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Adac1", MMZ_OTHERS, AO_DAC_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC2:
            enPort = SndOpGetPort(enOutPort, enType);
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            stHwPortAttr.stIfAttr.eCrgSource = AIAO_TX_CRG2;
            HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Adac2", MMZ_OTHERS, AO_DAC_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;

        case HI_UNF_SND_OUTPUTPORT_EXT_DAC3:
            enPort = SndOpGetPort(enOutPort, enType);
            HAL_AIAO_P_GetTxI2SDfAttr(enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            stHwPortAttr.stIfAttr.eCrgSource = AIAO_TX_CRG2;
            HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Adac3", MMZ_OTHERS, AO_DAC_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;
#if defined (HI_I2S0_SUPPORT)

        case HI_UNF_SND_OUTPUTPORT_I2S0:
            HI_ASSERT(pstAttr->unAttr.stI2sAttr.stAttr.enChannel == HI_UNF_I2S_CHNUM_2 ||
                      pstAttr->unAttr.stI2sAttr.stAttr.enChannel == HI_UNF_I2S_CHNUM_1);	//support slic mono
            HI_ASSERT(pstAttr->unAttr.stI2sAttr.stAttr.enBitDepth == HI_UNF_I2S_BIT_DEPTH_16);
            HAL_AIAO_P_GetBoardTxI2SDfAttr(0, &pstAttr->unAttr.stI2sAttr.stAttr, &enPort, &stHwPortAttr);

            if (bAlsaI2sUse == HI_TRUE)
            {
                stRbfMmz.u32Size                          = pstAoI2sParam->stBuf.u32BufSize;
                u32BufSize                                = pstAoI2sParam->stBuf.u32BufSize;
                HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
                stRbfMmz.u32StartPhyAddr                  = pstAoI2sParam->stBuf.u32BufPhyAddr;
                stRbfMmz.pu8StartVirAddr                  = pstAoI2sParam->stBuf.pu8BufVirAddr;
                stHwPortAttr.pIsrFunc                     = (AIAO_IsrFunc*)pstAoI2sParam->IsrFunc;
                stHwPortAttr.pSubstream                    = pstAoI2sParam->pSubstream;
                stHwPortAttr.stIfAttr.enRate              = (AIAO_SAMPLE_RATE_E)pstAoI2sParam->enRate;
                stHwPortAttr.stBufConfig.u32PeriodBufSize = pstAoI2sParam->stBuf.u32PeriodByteSize;
                stHwPortAttr.stBufConfig.u32PeriodNumber  = pstAoI2sParam->stBuf.u32Periods;
                Ret = HI_SUCCESS;
            }
            else
            {
                if (HI_UNF_SAMPLE_RATE_8K == enSampleRate)
                {
                    //slic 8k 1ch , the sizeof aop size should > 512, so the smallest size  > 40ms
                    u32FrameSize = AUTIL_CalcFrameSize((HI_U32)stHwPortAttr.stIfAttr.enChNum, (HI_U32)stHwPortAttr.stIfAttr.enBitDepth);
                    stHwPortAttr.stBufConfig.u32PeriodBufSize = 40 * enSampleRate * u32FrameSize / 1000;    //8k 40ms
                    stHwPortAttr.stBufConfig.u32PeriodBufSize = (stHwPortAttr.stBufConfig.u32PeriodBufSize / u32AlignInBytes) * u32AlignInBytes; //Align 256bit
                    HI_INFO_AO("Slic device aop buffersize force to 40ms !\n");
                }

                u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
                HI_ASSERT(u32BufSize < AO_I2S_MMZSIZE_MAX);
                Ret = HI_DRV_MMZ_AllocAndMap("AO_I2s0", MMZ_OTHERS, AO_I2S_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            }

            break;
#endif

#if defined (HI_I2S1_SUPPORT)
        case HI_UNF_SND_OUTPUTPORT_I2S1:
            HI_ASSERT(pstAttr->unAttr.stI2sAttr.stAttr.enChannel == HI_UNF_I2S_CHNUM_2);
            HI_ASSERT(pstAttr->unAttr.stI2sAttr.stAttr.enBitDepth == HI_UNF_I2S_BIT_DEPTH_16);
            HAL_AIAO_P_GetBorardTxI2SDfAttr(1, &pstAttr->unAttr.stI2sAttr.stAttr, &enPort, &stHwPortAttr);
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;
            HI_ASSERT(u32BufSize < AO_I2S_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_I2s1", MMZ_OTHERS, AO_I2S_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;
#endif

        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            enPort = SndOpGetPort(enOutPort, enType);
            HAL_AIAO_P_GetTxSpdDfAttr(enPort, &stHwPortAttr);
#ifdef HI_SOUND_SPDIF_COMPENSATION_SUPPORT
            stHwPortAttr.stBufConfig.u32PeriodNumber = SPDIF_PERIOND_NUM;
#endif
            u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 4; //considerate from 48k to 192k  for example arc ddp
            HI_ASSERT(u32BufSize < AO_SPDIF_MMZSIZE_MAX);
            Ret = HI_DRV_MMZ_AllocAndMap("AO_Spidf", MMZ_OTHERS, AO_SPDIF_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
            break;

        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            if (SND_AOP_TYPE_I2S == enType)
            {
                enPort = SndOpGetPort(enOutPort, enType);
                HAL_AIAO_P_GetHdmiI2SDfAttr(enPort, &stHwPortAttr);
#if 1 //def HI_ALSA_HDMI_ONLY_SUPPORT

                if (bAlsaI2sUse == HI_TRUE)
                {
                    stRbfMmz.u32Size                          = pstAoI2sParam->stBuf.u32BufSize;
                    u32BufSize                                = pstAoI2sParam->stBuf.u32BufSize;
                    HI_ASSERT(u32BufSize < AO_DAC_MMZSIZE_MAX);
                    stRbfMmz.u32StartPhyAddr                  = pstAoI2sParam->stBuf.u32BufPhyAddr;
                    stRbfMmz.pu8StartVirAddr                  = pstAoI2sParam->stBuf.pu8BufVirAddr;
                    stHwPortAttr.pIsrFunc                     = (AIAO_IsrFunc*)pstAoI2sParam->IsrFunc;
                    stHwPortAttr.pSubstream                    = pstAoI2sParam->pSubstream;
                    stHwPortAttr.stIfAttr.enRate              = pstAoI2sParam->enRate;
                    stHwPortAttr.stBufConfig.u32PeriodBufSize = pstAoI2sParam->stBuf.u32PeriodByteSize;
                    stHwPortAttr.stBufConfig.u32PeriodNumber  = pstAoI2sParam->stBuf.u32Periods;
                    //stHwPortAttr.u32VolumedB                  = AUTIL_VolumeLinear2RegdB((HI_U32)g_stUserGain.s32Gain);
                    Ret = HI_SUCCESS;
                }
                else
#endif
                {

#ifdef HI_SND_HBRA_PASSTHROUGH_SUPPORT
                    u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 32;  //verify considerate from 48k 2ch 16bit to 192k 8ch 32bit for example 24bit LPCM
#else
                    u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber;  //verify considerate from 48k 2ch 16bit to 192k 8ch 32bit for example 24bit LPCM
#endif
                    HI_ASSERT(u32BufSize < AO_HDMI_MMZSIZE_MAX);
                    Ret = HI_DRV_MMZ_AllocAndMap("AO_HdmiI2s", MMZ_OTHERS, AO_HDMI_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
                }
            }
            else
            {
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
                enPort = SndOpGetPort(HI_UNF_SND_OUTPUTPORT_HDMI0, SND_AOP_TYPE_I2S);
                HAL_AIAO_P_GetRbfAttr(enPort, &stRbfAttr);
                enPort = SndOpGetPort(enOutPort, enType);
                HAL_AIAO_P_GetTxSpdDfAttr(enPort, &stHwPortAttr);
                u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 4; //verify considerate frome 48k to 192k  for example ddp
                stRbfMmz.u32StartPhyAddr = stRbfAttr.tBufPhyAddr;
                stRbfMmz.pu8StartVirAddr = (HI_U8*)HI_NULL + stRbfAttr.tBufVirAddr;
                stRbfMmz.u32Size         = stRbfAttr.u32BufSize;
                Ret = HI_SUCCESS;
#else
                enPort = SndOpGetPort(enOutPort, enType);
                HAL_AIAO_P_GetTxSpdDfAttr(enPort, &stHwPortAttr);
                u32BufSize = stHwPortAttr.stBufConfig.u32PeriodBufSize * stHwPortAttr.stBufConfig.u32PeriodNumber * 4; //verify considerate frome 48k to 192k  for example ddp
                HI_ASSERT(u32BufSize < AO_SPDIF_MMZSIZE_MAX);
                Ret = HI_DRV_MMZ_AllocAndMap("AO_HdmiSpidf", MMZ_OTHERS, AO_SPDIF_MMZSIZE_MAX, AIAO_BUFFER_ADDR_ALIGN, &stRbfMmz);
#endif
            }

            break;

        default:
            HI_ERR_AO("Outport is invalid!\n");
            goto SndReturn_ERR_EXIT;
    }

    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("call HI_DRV_MMZ_AllocAndMap failed! Ret = 0x%x\n", Ret);
        goto SndReturn_ERR_EXIT;
    }

    stHwPortAttr.bExtDmaMem = HI_TRUE;
    stHwPortAttr.stExtMem.tBufPhyAddr = stRbfMmz.u32StartPhyAddr;
    stHwPortAttr.stExtMem.tBufVirAddr = (HI_VIRT_ADDR_T)stRbfMmz.pu8StartVirAddr;
    stHwPortAttr.stExtMem.u32BufSize = u32BufSize;
    stHwPortAttr.stIfAttr.enRate = enSampleRate;

    Ret = HAL_AIAO_P_Open(enPort, &stHwPortAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("call HAL_AIAO_P_Open failed! Ret = 0x%x\n", Ret);
        goto SndMMZRelease_ERR_EXIT;
    }

    memset(&stRbfAttr, 0, sizeof(AIAO_RBUF_ATTR_S));
    Ret = HAL_AIAO_P_GetRbfAttr(enPort, &stRbfAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("call HAL_AIAO_P_GetRbfAttr failed! Ret = 0x%x\n", Ret);
        goto SndClosePort_ERR_EXIT;
    }

    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyAddr = stRbfAttr.tBufPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirAddr = stRbfAttr.tBufVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyWptr = stRbfAttr.tBufPhyWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirWptr = stRbfAttr.tBufVirWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyRptr = stRbfAttr.tBufPhyRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirRptr = stRbfAttr.tBufVirRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag = 1;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize = stRbfAttr.u32BufSize;
    stAopAttr.stRbfOutAttr.u32BufBitPerSample = stHwPortAttr.stIfAttr.enBitDepth;
    stAopAttr.stRbfOutAttr.u32BufChannels   = stHwPortAttr.stIfAttr.enChNum;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = stHwPortAttr.stIfAttr.enRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = 0;
    stAopAttr.bRbfHwPriority = HI_TRUE;
    stAopAttr.bIsCast = HI_FALSE;
    stAopAttr.stRbfOutAttr.u32BufLatencyThdMs = AOE_AOP_BUFF_LATENCYMS_DF;

    Ret = HAL_AOE_AOP_Create(&enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("call HAL_AOE_AOP_Create failed! Ret = 0x%x\n", Ret);
        goto SndClosePort_ERR_EXIT;
    }

#ifdef __DPT__
    SndOpGetDefDrcAttr(state);
    HAL_AOE_AOP_SetDrcAttr(enAOP, &state->stDrcAttr);
    SndOpGetDefPeqAttr(state);
    HAL_AOE_AOP_SetPeqAttr(enAOP, &state->stPeqAttr);
#endif

    if (HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort || SND_AOP_TYPE_SPDIF != enType) //spdif interface of hdmi don't start aop
    {
        Ret = HAL_AIAO_P_Start(enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPort);
            goto SndDestroyAOP_ERR_EXIT;
        }

        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            goto SndStopPort_ERR_EXIT;
        }
    }

    u32AopId = (HI_U32)enType;
    state->u32OpMask |= 1 << u32AopId;
    if (HI_UNF_SND_OUTPUTPORT_HDMI0 != enOutPort || SND_AOP_TYPE_SPDIF != enType) //spdif interface of hdmi is not active
    {
        state->ActiveId  = u32AopId;
    }

    state->enPortID[u32AopId] = enPort;
    state->enAOP[u32AopId] = enAOP;
    state->stRbfMmz[u32AopId]   = stRbfMmz;
    state->stPortUserAttr[u32AopId] = stHwPortAttr;
    state->enEngineType[u32AopId] = SND_ENGINE_TYPE_PCM;
    return HI_SUCCESS;

SndStopPort_ERR_EXIT:
    (HI_VOID)HAL_AIAO_P_Stop(enPort, AIAO_STOP_IMMEDIATE);
SndDestroyAOP_ERR_EXIT:
    HAL_AOE_AOP_Destroy(enAOP);
SndClosePort_ERR_EXIT:
    HAL_AIAO_P_Close(enPort);
SndMMZRelease_ERR_EXIT:
    HI_DRV_MMZ_UnmapAndRelease(&stRbfMmz);
SndReturn_ERR_EXIT:
    return HI_FAILURE;
}

SND_ENGINE_TYPE_E SND_GetOpEngineType(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enEngineType[state->ActiveId];
}

static HI_S32 SndOpCreateCast(HI_VOID* *phSndOp, HI_HANDLE* phCast, HI_UNF_SND_CAST_ATTR_S* pstUserCastAttr,
                              MMZ_BUFFER_S* pstMMz)
{
    HI_S32 Ret;
    AOE_AOP_ID_E enAOP;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    HI_U32 uBufSize, uFrameSize;
    AIAO_CAST_ATTR_S stCastAttr;
    AIAO_CAST_ID_E enCast;
    SND_OP_STATE_S* state = HI_NULL;
    HI_UNF_SND_CAST_ATTR_S stUserCastAttr;
    HI_VOID* pWptrAddr, *pRptrAddr;

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(SND_OP_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AIAO("malloc SndOpCreate failed\n");
        return HI_FAILURE;
    }

    memset(state, 0, sizeof(SND_OP_STATE_S));
    memcpy(&stUserCastAttr, pstUserCastAttr, sizeof(HI_UNF_SND_CAST_ATTR_S));
    stCastAttr.u32BufChannels = 2;
    stCastAttr.u32BufBitPerSample = 16;
    stCastAttr.u32BufSampleRate = 48000;
    stCastAttr.u32BufDataFormat = 0;

    uFrameSize = AUTIL_CalcFrameSize(stCastAttr.u32BufChannels, stCastAttr.u32BufBitPerSample);
    uBufSize = stUserCastAttr.u32PcmFrameMaxNum * stUserCastAttr.u32PcmSamplesPerFrame * uFrameSize;
    stCastAttr.u32BufLatencyThdMs = AUTIL_ByteSize2LatencyMs(uBufSize, uFrameSize, stCastAttr.u32BufSampleRate);

    HI_ASSERT((uBufSize) < AO_CAST_MMZSIZE_MAX);

    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyAddr = pstMMz->u32StartPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirAddr = (HI_VIRT_ADDR_T)pstMMz->pu8StartVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize = uBufSize;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag = 0;  /* cast use aop Wptr&Rptr avoid dsp cache problem */
    stAopAttr.stRbfOutAttr.u32BufBitPerSample = stCastAttr.u32BufBitPerSample;
    stAopAttr.stRbfOutAttr.u32BufChannels   = stCastAttr.u32BufChannels;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = stCastAttr.u32BufSampleRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = 0;
    stAopAttr.bRbfHwPriority = HI_FALSE;
    stAopAttr.bIsCast = HI_TRUE;
    stAopAttr.bAddMute = pstUserCastAttr->bAddMute;
    stAopAttr.stRbfOutAttr.u32BufLatencyThdMs = AOE_AOP_BUFF_LATENCYMS_DF;
    Ret = HAL_AOE_AOP_Create(&enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AIAO("AOP_Create failed\n");
        AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
        return HI_FAILURE;
    }

#ifdef __DPT__
    SndOpGetDefPeqAttr(state);
    HAL_AOE_AOP_SetPeqAttr(enAOP, &state->stPeqAttr);
#endif

#ifndef CASTSIMULATEOP_DEBUG
    (HI_VOID)HAL_AOE_AOP_SetAefBypass(enAOP, HI_TRUE); //cast data don't need audio effect proccess
#endif

    stCastAttr.extDmaMem.tBufPhyAddr = pstMMz->u32StartPhyAddr;
    stCastAttr.extDmaMem.tBufVirAddr = (HI_VIRT_ADDR_T)pstMMz->pu8StartVirAddr;
    stCastAttr.extDmaMem.u32BufSize = uBufSize;
    stCastAttr.bAddMute = pstUserCastAttr->bAddMute;  //TODO proc
    iHAL_AOE_AOP_GetRptrAndWptrRegAddr(enAOP, &pWptrAddr, &pRptrAddr);
    stCastAttr.extDmaMem.tWptrAddr = (HI_VIRT_ADDR_T)pWptrAddr;
    stCastAttr.extDmaMem.tRptrAddr = (HI_VIRT_ADDR_T)pRptrAddr;
    Ret = HAL_CAST_Create(&enCast, &stCastAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AIAO("Cast_Create failed\n");
        HAL_AOE_AOP_Destroy(enAOP);
        AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
        return HI_FAILURE;
    }

    state->u32OpMask |= 1 << SND_AOP_TYPE_CAST;
    state->ActiveId = 0;
    state->enAOP[0] = enAOP;

    memcpy(&state->stCastAttr, &stCastAttr, sizeof(AIAO_CAST_ATTR_S));

    state->CastId = enCast;
    state->enEngineType[0] = SND_ENGINE_TYPE_PCM;
    state->enCurnStatus = SND_OP_STATUS_STOP;
    state->enOutType = SND_OUTPUT_TYPE_CAST;

    *phCast  = (HI_HANDLE )enCast;
    *phSndOp = (HI_VOID* )state;

    return HI_SUCCESS;
}

static HI_VOID SndOpGetSubFormatAttr(SND_OP_STATE_S* state, SND_OP_ATTR_S* pstSndPortAttr)
{
    AOE_AOP_CHN_ATTR_S stAopAttr;
    AOE_AOP_OUTBUF_ATTR_S* pstAttr;
    AIAO_BufAttr_S* pstAiAOBufAttr;

    HAL_AOE_AOP_GetAttr(state->enAOP[state->ActiveId], &stAopAttr);
    pstAttr = &stAopAttr.stRbfOutAttr;
    pstAiAOBufAttr = &state->stPortUserAttr[state->ActiveId].stBufConfig;

    pstSndPortAttr->u32Channels     = pstAttr->u32BufChannels;
    pstSndPortAttr->u32SampleRate   = pstAttr->u32BufSampleRate;
    pstSndPortAttr->u32BitPerSample = pstAttr->u32BufBitPerSample;
    pstSndPortAttr->u32DataFormat   = pstAttr->u32BufDataFormat;
    pstSndPortAttr->u32LatencyThdMs = pstAttr->u32BufLatencyThdMs;
    if (SND_OUTPUT_TYPE_CAST != state->enOutType)
    {
        pstSndPortAttr->u32PeriodBufSize = pstAiAOBufAttr->u32PeriodBufSize;
        pstSndPortAttr->u32PeriodNumber  = pstAiAOBufAttr->u32PeriodNumber;
    }
    return;
}

HI_S32 SndOpStart(HI_VOID* pSndOp, HI_VOID* pstParams)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    AOE_AOP_ID_E enAOP = state->enAOP[state->ActiveId];
    HI_S32 Ret;

    if (SND_OP_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        Ret = HAL_CAST_Start(state->CastId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_CAST_Start(%d) failed\n", state->CastId);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }
    else
    {
        Ret = HAL_AIAO_P_Start(enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPort);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Start(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Start(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }

    state->enCurnStatus = SND_OP_STATUS_START;

    return HI_SUCCESS;
}

HI_S32 SndOpStop(HI_VOID* pSndOp, HI_VOID* pstParams)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    AOE_AOP_ID_E enAOP = state->enAOP[state->ActiveId];
    HI_S32 Ret;

    if (SND_OP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        Ret = HAL_CAST_Stop(state->CastId);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_CAST_Stop(%d) failed\n", state->CastId);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Stop(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Stop(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }
    else
    {
        Ret = HAL_AIAO_P_Stop(enPort, AIAO_STOP_IMMEDIATE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Stop(%d) failed\n", enPort);
            return HI_FAILURE;
        }

        Ret = HAL_AOE_AOP_Stop(enAOP);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AOE_AOP_Stop(%d) failed\n", enAOP);
            return HI_FAILURE;
        }
    }
    state->enCurnStatus = SND_OP_STATUS_STOP;
    return HI_SUCCESS;
}

static HI_VOID SndOpDestroyAop(SND_OP_STATE_S* state, SND_AOP_TYPE_E enType)
{
    HAL_AOE_AOP_Destroy(state->enAOP[enType]);
    HAL_AIAO_P_Close(state->enPortID[enType]);

#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    /*
     * in HDMI_I2S_SPDIF_MUX mode, spdif port of HDMI0 use the same mmz of i2s port
     * here we only need to free mmz at i2s port
     */
    if (HI_UNF_SND_OUTPUTPORT_HDMI0 == state->enOutPort &&
        SND_AOP_TYPE_SPDIF == enType)
    {
        return;
    }
#endif

    HI_DRV_MMZ_UnmapAndRelease(&state->stRbfMmz[enType]);
}

static HI_VOID SndOpDestroy(HI_VOID* pSndOp, HI_BOOL bSuspend)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    SND_AOP_TYPE_E type;

    if (0 == state->u32OpMask)
    {
        return;
    }

    SndOpStop(pSndOp, HI_NULL);

    if (state->enOutType == SND_OUTPUT_TYPE_CAST)
    {
        HAL_AOE_AOP_Destroy(state->enAOP[0]);
        HAL_CAST_Destroy(state->CastId);
    }
    else
    {
        for (type = SND_AOP_TYPE_I2S; type < SND_AOP_TYPE_CAST; type++)
        {
            if (state->u32OpMask & (1 << type))
            {
                SndOpDestroyAop(state, type);
            }
        }

        if (state->enOutType == SND_OUTPUT_TYPE_DAC)
        {
#if defined (HI_TIANLAI_V500) || defined (HI_TIANLAI_V600) || defined (HI_TIANLAI_V700)
            ADAC_TIANLAI_DeInit(bSuspend);
#endif
        }
    }

    memset(state, 0, sizeof(SND_OP_STATE_S));
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    return;
}

static HI_VOID SndOpInitState(SND_OP_STATE_S* state)
{
    HI_U32 idx;

    memset(state, 0, sizeof(SND_OP_STATE_S));
    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        state->enPortID[idx] = AIAO_PORT_BUTT;
        state->enAOP[idx] = AOE_AOP_BUTT;
        state->enEngineType[idx] = SND_ENGINE_TYPE_BUTT;
    }
#ifdef HI_SND_AMP_SUPPORT
    state->pstAmpFunc = HI_NULL;
#endif
    return;
}

static HI_S32 SndOpSetSPDIFCategoryCode(HI_VOID* pSndOp, HI_UNF_SND_SPDIF_CATEGORYCODE_E enCategoryCode)
{
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    enPort = state->enPortID[state->ActiveId];
    Ret = HAL_AIAO_P_SetSpdifCategoryCode(enPort, (AIAO_SPDIF_CATEGORYCODE_E)enCategoryCode);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetSpdifCategoryCode port:%d, Category Code: %d\n", (HI_U32)enPort, (HI_U32)enCategoryCode);
        return Ret;
    }
    state->enUserSPDIFCategoryCode = enCategoryCode;
    return Ret;
}

static HI_S32 SndOpSetSPDIFSCMSMode(HI_VOID* pSndOp, HI_UNF_SND_SPDIF_SCMSMODE_E enSCMSMode)
{
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    enPort = state->enPortID[state->ActiveId];
    Ret = HAL_AIAO_P_SetSpdifSCMSMode(enPort, (AIAO_SPDIF_SCMS_MODE_E)enSCMSMode);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetSpdifSCMSMode port:%d, SCMSMode: %d\n", (HI_U32)enPort, (HI_U32)enSCMSMode);
        return Ret;
    }
    state->enUserSPDIFSCMSMode = enSCMSMode;
    return Ret;
}

#if 1//def HI_ALSA_I2S_ONLY_SUPPORT
HI_S32 AlsaHwSndOpStart(HI_VOID* pSndOp, HI_VOID* pstParams)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    HI_S32 Ret;
    if (SND_OP_STATUS_START == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if ((SND_OUTPUT_TYPE_I2S == state->enOutType) || (SND_OUTPUT_TYPE_HDMI == state->enOutType))
    {
        Ret = HAL_AIAO_P_Start(enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPort);
            return HI_FAILURE;
        }
    }
    state->enCurnStatus = SND_OP_STATUS_START;
    return HI_SUCCESS;
}

HI_S32 AlsaHwSndOpStop(HI_VOID* pSndOp, HI_VOID* pstParams)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    HI_S32 Ret;
    if (SND_OP_STATUS_STOP == state->enCurnStatus)
    {
        return HI_SUCCESS;
    }

    if ((SND_OUTPUT_TYPE_I2S == state->enOutType) || (SND_OUTPUT_TYPE_HDMI == state->enOutType))
    {
        Ret = HAL_AIAO_P_Stop(enPort, AIAO_STOP_IMMEDIATE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Stop(%d) failed\n", enPort);
            return HI_FAILURE;
        }
    }
    state->enCurnStatus = SND_OP_STATUS_STOP;
    return HI_SUCCESS;
}
#endif

static HI_S32 SndOpCreate(HI_VOID* *ppSndOp, DRV_SND_OUTPORT_S* pstAttr,
                          AO_ALSA_I2S_Param_S* pstAoI2sParam, HI_BOOL bResume,
                          HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SND_OP_STATE_S* state = HI_NULL;
    HI_S32 Ret = HI_FAILURE;
    SND_AOP_TYPE_E AopType;

    state = AUTIL_AO_MALLOC(HI_ID_AO, sizeof(SND_OP_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("malloc SndOpCreate failed\n");
        return HI_FAILURE;
    }

    SndOpInitState(state);

    for (AopType = SND_AOP_TYPE_I2S; AopType < SND_AOP_TYPE_CAST; AopType++)
    {
        if (SND_AOP_TYPE_I2S == AopType && HI_UNF_SND_OUTPUTPORT_SPDIF0 == pstAttr->enOutPort)
        {
            continue;
        }

        if (SND_AOP_TYPE_SPDIF == AopType && HI_UNF_SND_OUTPUTPORT_SPDIF0 != pstAttr->enOutPort && HI_UNF_SND_OUTPUTPORT_HDMI0 != pstAttr->enOutPort)
        {
            continue;
        }

        if (HI_SUCCESS != SndOpCreateAop(state, pstAttr, AopType, pstAoI2sParam, enSampleRate))
        {
            HI_ERR_AO("call SndOpCreateAop failed!\n");
            goto SndCreatePort_ERR_EXIT;
        }
    }

    SndOpGetSubFormatAttr(state, &state->stSndPortAttr);
    switch (pstAttr->enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC1:
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC2:
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC3:
            memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_U32));
            state->enOutType = SND_OUTPUT_TYPE_DAC;

#if defined (HI_TIANLAI_V500) || defined (HI_TIANLAI_V600) || defined (HI_TIANLAI_V700)
            ADAC_TIANLAI_Init(state->stSndPortAttr.u32SampleRate, bResume);
#endif
            break;

        case HI_UNF_SND_OUTPUTPORT_I2S0:
        case HI_UNF_SND_OUTPUTPORT_I2S1:
            memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_UNF_SND_I2S_ATTR_S));
            state->enOutType = SND_OUTPUT_TYPE_I2S;
            break;

        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_U32));
            state->enUserSPDIFSCMSMode = HI_UNF_SND_SPDIF_SCMSMODE_COPYPROHIBITED;
            state->enUserSPDIFCategoryCode = HI_UNF_SND_SPDIF_CATEGORY_GENERAL;
            SndOpSetSPDIFSCMSMode((HI_VOID*)state, state->enUserSPDIFSCMSMode);
            SndOpSetSPDIFCategoryCode((HI_VOID*)state, state->enUserSPDIFCategoryCode);
            state->enOutType = SND_OUTPUT_TYPE_SPDIF;
            break;

        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            memcpy(&state->stSndPortAttr.unAttr, &pstAttr->unAttr, sizeof(HI_U32));
            state->enOutType = SND_OUTPUT_TYPE_HDMI;
            break;

        default:
            HI_ERR_AO("Err OutPort Type!\n");
            goto SndCreatePort_ERR_EXIT;
    }

    state->enOutPort = pstAttr->enOutPort;
    state->enCurnStatus = SND_OP_STATUS_START;
    state->u32UserMute = 0;
    state->u32LatencyMs = AOE_AOP_BUFF_LATENCYMS_DF;
    state->enUserTrackMode = HI_UNF_TRACK_MODE_STEREO;
    state->stUserGain.bLinearMode = HI_FALSE;
    state->stUserGain.s32Gain = 0;
    *ppSndOp = (HI_VOID*)state;

    return HI_SUCCESS;

SndCreatePort_ERR_EXIT:
    *ppSndOp = (HI_VOID*)HI_NULL;
    SndOpDestroy((HI_VOID*)state, HI_FALSE);
    AUTIL_AO_FREE(HI_ID_AO, (HI_VOID*)state);
    return Ret;
}

HI_S32 SndOpSetAttr(HI_VOID* pSndOp, SND_OP_ATTR_S* pstSndPortAttr)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;
    AIAO_PORT_ID_E enPortID;
    AOE_AOP_ID_E enAOP;
    AIAO_PORT_ATTR_S stAiaoAttr;
    AOE_AOP_CHN_ATTR_S stAopAttr;
    AIAO_RBUF_ATTR_S stRbfAttr;
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    AIAO_OP_TYPE_E enOpType = SND_OP_TYPE_I2S;
#endif
    if (SND_OP_STATUS_STOP != state->enCurnStatus)
    {
        return HI_FAILURE;
    }

    /* note: noly spdif & hdmi support set attr as pass-through switch */
    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        return HI_FAILURE;
    }

    if (SND_OUTPUT_TYPE_DAC == state->enOutType)
    {
        return HI_FAILURE;
    }

    if (SND_OUTPUT_TYPE_I2S == state->enOutType)
    {
        return HI_FAILURE;
    }

    if (pstSndPortAttr->u32PeriodBufSize < AO_SNDOP_MIN_AOP_PERIOD_SIZE)
    {
        pstSndPortAttr->u32PeriodBufSize = AO_SNDOP_MIN_AOP_PERIOD_SIZE;
    }

    /* note: hdmi tx use spdif or i2s interface at diffrerent data format */
    if (SND_OUTPUT_TYPE_HDMI == state->enOutType)
    {
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
        enOpType = SND_OP_TYPE_I2S;
#endif
        if (0 == (pstSndPortAttr->u32DataFormat  & 0xff))
        {
            state->ActiveId = SND_AOP_TYPE_I2S;  //2.0 pcm
        }
        else if (AUTIL_isIEC61937Hbr(pstSndPortAttr->u32DataFormat  & 0xff, pstSndPortAttr->u32SampleRate))
        {
            state->ActiveId = SND_AOP_TYPE_I2S;       // hbr
            if (IEC61937_DATATYPE_DOLBY_DIGITAL_PLUS == (pstSndPortAttr->u32DataFormat & 0xff))
            {
                state->ActiveId = SND_AOP_TYPE_SPDIF; // lbr or hbr(ddp)
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
                enOpType = SND_OP_TYPE_SPDIF;
#endif
            }
        }
        else if (IEC61937_DATATYPE_71_LPCM == (pstSndPortAttr->u32DataFormat & 0xff) ||
                 IEC61937_DATATYPE_20_LPCM == (pstSndPortAttr->u32DataFormat & 0xff))
        {
            state->ActiveId = SND_AOP_TYPE_I2S;   //7.1 lpcm
        }
        else
        {
            state->ActiveId = SND_AOP_TYPE_SPDIF;     // lbr or hbr(ddp)
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
            enOpType = SND_OP_TYPE_SPDIF;
#endif
        }
    }

    enPortID = state->enPortID[state->ActiveId];
    enAOP = state->enAOP[state->ActiveId];

    Ret = HAL_AIAO_P_GetAttr(enPortID, &stAiaoAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AIAO_P_GetAttr failed(0x%x)!", Ret);
        return Ret;
    }

    memset(&stAopAttr, 0, sizeof(AOE_AOP_CHN_ATTR_S));
    Ret = HAL_AOE_AOP_GetAttr(enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AIAO_P_GetAttr failed(0x%x)!", Ret);
        return Ret;
    }
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    if (SND_OUTPUT_TYPE_HDMI == state->enOutType)
    {
        Ret = HAL_AIAO_P_SetOpType(enPortID, enOpType);
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_AO("HAL_AIAO_P_SetOpType port:0x%x\n", enPortID);
            return HI_FAILURE;
        }
    }
#endif

    stAiaoAttr.stIfAttr.enBitDepth = (AIAO_BITDEPTH_E)pstSndPortAttr->u32BitPerSample;
    stAiaoAttr.stIfAttr.enChNum = (AIAO_I2S_CHNUM_E)pstSndPortAttr->u32Channels;
    stAiaoAttr.stIfAttr.enRate = (AIAO_SAMPLE_RATE_E)pstSndPortAttr->u32SampleRate;
    if (pstSndPortAttr->u32DataFormat & 0xff)
    {
        if (IEC61937_DATATYPE_DTSCD == (pstSndPortAttr->u32DataFormat & 0xff)) //dtscd type is the same as pcm
        {
            HAL_AIAO_P_SetBypass(enPortID, HI_TRUE);
            HAL_AIAO_P_SetMode(enPortID, HI_FALSE);
        }
        else
        {
            HAL_AIAO_P_SetBypass(enPortID, HI_TRUE);
            HAL_AIAO_P_SetMode(enPortID, HI_TRUE);
        }

        if (SND_OUTPUT_TYPE_HDMI == state->enOutType)
        {
            state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_HDMI_RAW;
        }
        else if (SND_OUTPUT_TYPE_SPDIF == state->enOutType)
        {
            state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_SPDIF_RAW;
        }
    }
    else
    {
        HAL_AIAO_P_SetBypass(enPortID, HI_FALSE);
        HAL_AIAO_P_SetMode(enPortID, HI_FALSE);
        state->enEngineType[state->ActiveId] = SND_ENGINE_TYPE_PCM;
    }

    //todo, optimize u32PeriodBufSize & u32PeriodNumber
    stAiaoAttr.stBufConfig.u32PeriodBufSize = pstSndPortAttr->u32PeriodBufSize;
    stAiaoAttr.stBufConfig.u32PeriodNumber = pstSndPortAttr->u32PeriodNumber;
    Ret = HAL_AIAO_P_SetAttr(enPortID, &stAiaoAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AO("HAL_AIAO_P_SetAttr port:0x%x\n", enPortID);
        return Ret;
    }

    memset(&stRbfAttr, 0, sizeof(AIAO_RBUF_ATTR_S));
    Ret = HAL_AIAO_P_GetRbfAttr(enPortID, &stRbfAttr);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyAddr = stRbfAttr.tBufPhyAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirAddr = stRbfAttr.tBufVirAddr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyWptr = stRbfAttr.tBufPhyWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirWptr = stRbfAttr.tBufVirWptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufPhyRptr = stRbfAttr.tBufPhyRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.tBufVirRptr = stRbfAttr.tBufVirRptr;
    stAopAttr.stRbfOutAttr.stRbfAttr.u32BufSize    = stRbfAttr.u32BufSize;

    stAopAttr.stRbfOutAttr.u32BufBitPerSample = pstSndPortAttr->u32BitPerSample;
    stAopAttr.stRbfOutAttr.u32BufChannels   = pstSndPortAttr->u32Channels;
    stAopAttr.stRbfOutAttr.u32BufSampleRate = pstSndPortAttr->u32SampleRate;
    stAopAttr.stRbfOutAttr.u32BufDataFormat = pstSndPortAttr->u32DataFormat;
    Ret = HAL_AOE_AOP_SetAttr(enAOP, &stAopAttr);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    state->stPortUserAttr[state->ActiveId].stIfAttr = stAiaoAttr.stIfAttr;
    state->stPortUserAttr[state->ActiveId].stBufConfig = stAiaoAttr.stBufConfig;
    memcpy(&state->stSndPortAttr, pstSndPortAttr, sizeof(SND_OP_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 SndOpGetAttr(HI_VOID* pSndOp, SND_OP_ATTR_S* pstSndPortAttr)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    memcpy(pstSndPortAttr, &state->stSndPortAttr, sizeof(SND_OP_ATTR_S));
    return HI_SUCCESS;
}

HI_S32 SndOpGetStatus(HI_VOID* pSndOp, AIAO_PORT_STAUTS_S* pstPortStatus)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];
    HI_S32 Ret;
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    AIAO_PORT_STAUTS_S stPortProcStatus;
#endif

    Ret = HAL_AIAO_P_GetStatus(enPort, pstPortStatus);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_GetStatus (port:%d) failed\n", (HI_U32)enPort);
        return Ret;
    }
#ifdef HI_SND_HDMI_I2S_SPDIF_MUX
    if (enPort == AIAO_PORT_SPDIF_TX0)
    {
        enPort = SndOpGetPort(HI_UNF_SND_OUTPUTPORT_HDMI0, SND_AOP_TYPE_I2S);

        Ret = HAL_AIAO_P_GetStatus(enPort, &stPortProcStatus);
        if (HI_SUCCESS != Ret)
        {
            HI_FATAL_AIAO("HAL_AIAO_P_GetStatus (port:%d) failed\n", (HI_U32)enPort);
            return Ret;
        }
        memcpy(&pstPortStatus->stProcStatus, &stPortProcStatus.stProcStatus, sizeof(AIAO_PROC_STAUTS_S));
    }
#endif
    return HI_SUCCESS;
}

AOE_AOP_ID_E SND_GetOpAopId(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enAOP[state->ActiveId];
}

HI_UNF_SND_OUTPUTPORT_E SND_GetOpOutputport(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    return state->enOutPort;
}

HI_S32 SndOpSetSampleRate(HI_VOID* pSndOp, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AIAO_PORT_ID_E enPort = state->enPortID[state->ActiveId];

    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast SampleRate
        return HI_SUCCESS;
    }

    Ret = HAL_AIAO_P_SetSampleRate(enPort, enSampleRate);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetSampleRate port:%d \n", enPort);
        return HI_FAILURE;
    }

    state->enSampleRate = enSampleRate;

    return HI_SUCCESS;
}

HI_S32 SndOpSetVolume(HI_VOID* pSndOp, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;
    HI_U32 u32dBReg;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast Volume
        return HI_SUCCESS;
    }

    if (HI_TRUE == pstGain->bLinearMode)
    {
        u32dBReg = AUTIL_VolumeLinear2RegdB((HI_U32)pstGain->s32Gain);
    }
    else
    {
        u32dBReg = AUTIL_VolumedB2RegdB(pstGain->s32Gain);
    }

    enPort = state->enPortID[state->ActiveId];
    Ret = HAL_AIAO_P_SetVolume(enPort, u32dBReg);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetVolume port:%d, VolunedB: %d\n", (HI_U32)enPort, pstGain->s32Gain);
        return HI_FAILURE;
    }

    state->stUserGain.bLinearMode = pstGain->bLinearMode;
    state->stUserGain.s32Gain = pstGain->s32Gain;

    return HI_SUCCESS;
}

HI_S32 SndOpSetTrackMode(HI_VOID* pSndOp, HI_UNF_TRACK_MODE_E enMode)
{
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast TrackMode
        return HI_SUCCESS;
    }

    enPort = state->enPortID[state->ActiveId];
    Ret = HAL_AIAO_P_SetTrackMode(enPort, AUTIL_TrackModeTransform(enMode));
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AIAO("HAL_AIAO_P_SetTrackMode port:%d\n", (HI_U32)enPort);
        return Ret;
    }

    state->enUserTrackMode = enMode;

    return Ret;
}

HI_S32 SndOpSetLowLatency(HI_VOID* pSndOp, HI_U32 u32LatencyMs)
{
    HI_S32 Ret = HI_SUCCESS;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AOE_AOP_ID_E enAOP;

    enAOP = state->enAOP[state->ActiveId];
    Ret = HAL_AOE_AOP_SetLowLatency(enAOP, u32LatencyMs);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_AOP_SetLowLatency(%d) failed\n", enAOP);
        return Ret;
    }

    state->u32LatencyMs = u32LatencyMs;
    return Ret;
}

HI_S32 SndOpGetLowLatency(HI_VOID* pSndOp, HI_U32* pu32LatencyMs)
{
    HI_S32 Ret = HI_SUCCESS;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AOE_AOP_ID_E enAOP;

    enAOP = state->enAOP[state->ActiveId];
    Ret = HAL_AOE_AOP_GetLowLatency(enAOP, pu32LatencyMs);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_AOP_GetLowLatency(%d) failed\n", enAOP);
    }

    return Ret;
}

#ifdef __DPT__
HI_S32 SndOpSetPreciVol(HI_VOID* pSndOp, HI_UNF_SND_PRECIGAIN_ATTR_S stPreciGain)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;
    HI_U32 u32IntdB = 0;
    HI_S32 s32DecdB = 0;

    u32IntdB = AUTIL_VolumedB2RegdB(stPreciGain.s32IntegerGain);
    s32DecdB = AUTIL_DecimalVolumedB2RegdB(stPreciGain.s32DecimalGain);

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetPreciVol(enAOP, u32IntdB, s32DecdB);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetPreciVol(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    state->stUserPreciGain = stPreciGain;

    return HI_SUCCESS;
}

HI_S32 SndOpSetBalance(HI_VOID* pSndOp, HI_S32 s32Balance)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetBalance(enAOP, s32Balance);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetBalance(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    state->s32UserBalance = s32Balance;

    return HI_SUCCESS;
}

HI_S32 SndOpSetDrcEnable(HI_VOID* pSndOp, HI_BOOL bEnable)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetDrcEnable(enAOP, bEnable);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetDrcEnable(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    state->bDrcEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 SndOpSetDrcAttr(HI_VOID* pSndOp, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetDrcAttr(enAOP, pstDrcAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetDrc(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    memcpy(&(state->stDrcAttr), pstDrcAttr, sizeof(HI_UNF_SND_DRC_ATTR_S));

    return HI_SUCCESS;
}


HI_S32 SND_SetOpDrcAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetDrcAttr(pSndOp, pstDrcAttr);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetDrcAttr(pSndOp, pstDrcAttr);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
    return Ret;
}

HI_S32 SND_GetOpDrcAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get DrcAttr of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        memcpy(pstDrcAttr, &(state->stDrcAttr), sizeof(HI_UNF_SND_DRC_ATTR_S));
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpDrcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetDrcEnable(pSndOp, bEnable);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetDrcEnable(pSndOp, bEnable);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
    return Ret;
}

HI_S32 SND_GetOpDrcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get DrcEnable of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *pbEnable = state->bDrcEnable;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SndOpSetPeqEnable(HI_VOID* pSndOp, HI_BOOL bEnable)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetPeqEnable(enAOP, bEnable);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetPeqEnable(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    state->bPeqEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 SndOpSetPeqAttr(HI_VOID* pSndOp, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetPeqAttr(enAOP, pstPeqAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetPeqAttr(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    memcpy(&(state->stPeqAttr), pstPeqAttr, sizeof(HI_UNF_SND_PEQ_ATTR_S));

    return HI_SUCCESS;
}


HI_S32 SND_SetOpPeqAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetPeqAttr(pSndOp, pstPeqAttr);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetPeqAttr(pSndOp, pstPeqAttr);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpPeqAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get PeqAttr of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        memcpy(pstPeqAttr, &(state->stPeqAttr), sizeof(HI_UNF_SND_PEQ_ATTR_S));
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpPeqEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetPeqEnable(pSndOp, bEnable);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetPeqEnable(pSndOp, bEnable);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
    return Ret;
}

HI_S32 SND_GetOpPeqEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get PeqEnable of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *pbEnable = state->bPeqEnable;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SndOpSetADOutputEnable(HI_VOID* pSndOp, HI_BOOL bEnable)
{
    HI_U32 idx;
    AOE_AOP_ID_E enAOP;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_S32 Ret = HI_FAILURE;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];
        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetADOutputEnable(enAOP, bEnable);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetADOutputEnable(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }

    state->bADOutputEnable = bEnable;

    return HI_SUCCESS;
}

HI_S32 SND_SetOpADOutputEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetADOutputEnable(pSndOp, bEnable);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetADOutputEnable(pSndOp, bEnable);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
    return Ret;
}

HI_S32 SND_GetOpADOutputEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get ADOutputEnable of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *pbEnable = state->bADOutputEnable;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}
#endif

HI_S32 SndOpSetAefBypass(HI_VOID* pSndOp, HI_BOOL bBypass)
{
    HI_S32 Ret;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AOE_AOP_ID_E enAOP;

    enAOP = state->enAOP[state->ActiveId];
    Ret = HAL_AOE_AOP_SetAefBypass(enAOP, bBypass);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("HAL_AOE_AOP_SetAefBypass(%d) failed\n", enAOP);
        return Ret;
    }

    state->bBypass = bBypass;

    return Ret;
}

HI_S32 SndOpSetMute(HI_VOID* pSndOp, HI_U32 u32Mute)
{
    HI_U32 idx;
    AIAO_PORT_ID_E enPort;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    HI_BOOL bMute = (u32Mute == 0) ? HI_FALSE : HI_TRUE;
    HI_S32 Ret = HI_FAILURE;

    if (SND_OUTPUT_TYPE_CAST == state->enOutType)
    {
        // todo, cast mute
        return HI_SUCCESS;
    }

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enPort = state->enPortID[idx];
        if (enPort < AIAO_PORT_BUTT)
        {
            Ret = HAL_AIAO_P_Mute(enPort, bMute);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_AO("HAL_AIAO_P_Mute port:%d, Mute: %d\n", (HI_U32)enPort, (HI_U32)bMute);
                return Ret;
            }
        }
    }

#ifdef HI_SND_AMP_SUPPORT
    if (state->pstAmpFunc)
    {
        if (state->pstAmpFunc->pfnAMP_SetMute)
        {
            Ret = (state->pstAmpFunc->pfnAMP_SetMute)(bMute);
            if (HI_SUCCESS != Ret)
            {
                HI_FATAL_AO("Amp mute failed\n");
                return HI_FAILURE;
            }
        }
    }
#endif

    state->u32UserMute = u32Mute;

    return Ret;
}

HI_S32 SndOpGetMute(HI_VOID* pSndOp)
{
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    return state->u32UserMute;
}

static HI_S32 SndOpGetDelayCompensation(HI_VOID* pSndOp, HI_U32* pu32RealDelayMs)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 idx;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AOE_AOP_ID_E enAOP;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];

        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_GetDelay(enAOP, pu32RealDelayMs);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_GetDelay(%d) failed\n", enAOP);
                return HI_FAILURE;
            }
        }
    }
    return HI_SUCCESS;
}

static HI_S32 SndOpSetDelayCompensation(HI_VOID* pSndOp, HI_U32 u32RealDelayMs)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 idx;
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;
    AOE_AOP_ID_E enAOP;

    for (idx = 0; idx < AO_SNDOP_MAX_AOP_NUM; idx++)
    {
        enAOP = state->enAOP[idx];

        if (enAOP < AOE_AOP_BUTT)
        {
            Ret = HAL_AOE_AOP_SetDelay(enAOP, u32RealDelayMs);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("HAL_AOE_AOP_SetDelay(%d) failed\n", enAOP);
                return Ret;
            }
        }
    }

    state->u32Delayms = u32RealDelayMs;

    return HI_SUCCESS;
}

#ifdef HI_SND_MUTECTL_SUPPORT
static HI_VOID SndOpDisableMuteCtrl(HI_U8* pu8Data)
{
    SND_CARD_STATE_S* pCard = (SND_CARD_STATE_S*)pu8Data;
    if (pCard->pstGpioFunc && pCard->pstGpioFunc->pfnGpioDirSetBit)
    {
        (pCard->pstGpioFunc->pfnGpioDirSetBit)(HI_SND_MUTECTL_GPIO, 0); //output
    }
    if (pCard->pstGpioFunc && pCard->pstGpioFunc->pfnGpioWriteBit)
    {
        (pCard->pstGpioFunc->pfnGpioWriteBit)(HI_SND_MUTECTL_GPIO, ((0 == HI_SND_MUTECTL_LEVEL) ? 1 : 0));
    }
    ADAC_FastPowerEnable(HI_FALSE);    //diable fast power up
    return;
}

static HI_VOID SndOpEnableMuteCtrl(SND_CARD_STATE_S* pCard)
{
    if (pCard->pstGpioFunc && pCard->pstGpioFunc->pfnGpioDirSetBit)
    {
        (pCard->pstGpioFunc->pfnGpioDirSetBit)(HI_SND_MUTECTL_GPIO, 0); //output
    }
    if (pCard->pstGpioFunc && pCard->pstGpioFunc->pfnGpioWriteBit)
    {
        (pCard->pstGpioFunc->pfnGpioWriteBit)(HI_SND_MUTECTL_GPIO, ((0 == HI_SND_MUTECTL_LEVEL) ? 0 : 1));
    }
    ADAC_FastPowerEnable(HI_TRUE);     //enable fast power up
    return;
}

static HI_S32 SndMuteCtrlInit(SND_CARD_STATE_S* pCard, HI_BOOL bResume)
{
    HI_S32 s32Ret = HI_SUCCESS;
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;

    s32Ret = HI_DRV_SYS_GetChipPackageType(&enPackageType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("HI_DRV_SYS_GetChipPackageType failed!\n");
        return s32Ret;
    }

    if (HI_CHIP_PACKAGE_TYPE_QFP_216 == enPackageType)
    {
        return HI_SUCCESS;
    }
#endif

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&pCard->pstGpioFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AO("HI_DRV_MODULE_GetFunction(HI_ID_GPIO) failed!\n");
        return s32Ret;
    }

    init_timer(&pCard->stMuteDisableTimer);
    pCard->stMuteDisableTimer.function = (void *)SndOpDisableMuteCtrl;
    pCard->stMuteDisableTimer.data = (HI_U8*)pCard - (HI_U8*)(HI_NULL);

    if (HI_TRUE == bResume)
    {
        pCard->stMuteDisableTimer.expires = (jiffies + msecs_to_jiffies(AO_SND_MUTE_DISABLE_TIMEMS_RESUME));
    }
    else
    {
        pCard->stMuteDisableTimer.expires = (jiffies + msecs_to_jiffies(AO_SND_MUTE_DISABLE_TIMEMS_RESUME));
    }

    add_timer(&pCard->stMuteDisableTimer);

    return HI_SUCCESS;
}

static HI_VOID SndMuteCtrlDeInit(SND_CARD_STATE_S* pCard, HI_BOOL bSuspend)
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    HI_CHIP_PACKAGE_TYPE_E enPackageType = HI_CHIP_PACKAGE_TYPE_BUTT;
    HI_S32 s32Ret = HI_SUCCESS;
    s32Ret = HI_DRV_SYS_GetChipPackageType(&enPackageType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_AO("HI_DRV_SYS_GetChipPackageType failed(0x%x)\n", s32Ret);
        return;
    }

    if (HI_CHIP_PACKAGE_TYPE_QFP_216 == enPackageType)
    {
        return;
    }
#endif

    SndOpEnableMuteCtrl(pCard);
    del_timer_sync(&pCard->stMuteDisableTimer);
}
#endif

HI_VOID SND_DestroyOp(SND_CARD_STATE_S* pCard, HI_BOOL bSuspend)
{
    HI_S32 u32PortNum;

    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        if (pCard->pSndOp[u32PortNum])
        {
#ifdef HI_SND_MUTECTL_SUPPORT
            if (HI_UNF_SND_OUTPUTPORT_DAC0 == ((SND_OP_STATE_S*)pCard->pSndOp[u32PortNum])->enOutPort)
            {
                SndMuteCtrlDeInit(pCard, bSuspend);
            }
#endif
            SndOpDestroy(pCard->pSndOp[u32PortNum], bSuspend);
            pCard->pSndOp[u32PortNum] = HI_NULL;
        }
    }
}

HI_S32 SND_CreateOp(SND_CARD_STATE_S* pCard, DRV_SND_ATTR_S* pstAttr,
                    AO_ALSA_I2S_Param_S* pstAoI2sParam, HI_BOOL bResume)
{
    HI_U32 u32PortNum;
    HI_VOID* pSndOp;

    for (u32PortNum = 0; u32PortNum < pstAttr->u32PortNum; u32PortNum++)
    {
        if (HI_SUCCESS != SndOpCreate(&pSndOp, &pstAttr->stOutport[u32PortNum], pstAoI2sParam, bResume, pstAttr->enSampleRate))
        {
            HI_ERR_AO("call SndOpCreate failed!\n");
            goto SND_CreateOp_ERR_EXIT;
        }

        pCard->pSndOp[u32PortNum] = pSndOp;

        if (HI_UNF_SND_OUTPUTPORT_DAC0 == pstAttr->stOutport[u32PortNum].enOutPort)
        {
#ifdef HI_SND_MUTECTL_SUPPORT
            HI_S32 s32Ret = HI_SUCCESS;
            s32Ret = SndMuteCtrlInit(pCard, bResume);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_AO("call SndMuteCtrlInit failed!\n");
                goto SND_CreateOp_ERR_EXIT;
            }
#endif
            pCard->bAdacEnable = HI_TRUE;
        }
    }

    if (pstAoI2sParam != NULL) //for i2s only card resume HI_ALSA_I2S_ONLY_SUPPORT
    {
        if (pstAoI2sParam->bAlsaI2sUse == HI_TRUE)
        {
            memcpy(&pCard->stUserOpenParamI2s, pstAoI2sParam, sizeof(AO_ALSA_I2S_Param_S));
        }
        else
        {
            memset(&pCard->stUserOpenParamI2s, 0, sizeof(AO_ALSA_I2S_Param_S));
        }
    }
    else
    {
        memset(&pCard->stUserOpenParamI2s, 0, sizeof(AO_ALSA_I2S_Param_S));
    }

    memcpy(&pCard->stUserOpenParam, pstAttr, sizeof(DRV_SND_ATTR_S));
    pCard->enUserSampleRate = pstAttr->enSampleRate;
    pCard->enUserHdmiMode = HI_UNF_SND_HDMI_MODE_LPCM;
    pCard->enUserSpdifMode = HI_UNF_SND_SPDIF_MODE_LPCM;

#ifdef __DPT__
    pCard->bUserArcEnable = HI_FALSE;
    pCard->enUserArcMode = HI_UNF_SND_ARC_AUDIO_MODE_LPCM;
#endif

    pCard->u32HdmiDataFormat = 0;
    pCard->u32SpdifDataFormat = 0;

    return HI_SUCCESS;

SND_CreateOp_ERR_EXIT:
    SND_DestroyOp(pCard, HI_FALSE);
    return HI_FAILURE;
}

HI_S32 SND_SetOpMute(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute)
{
    HI_VOID* pSndOp;
    HI_U32    u32Mute = 0;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;

        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                u32Mute = SndOpGetMute(pSndOp);
                u32Mute &= (~(1L << AO_SNDOP_GLOBAL_MUTE_BIT));
                u32Mute |= (HI_U32)bMute << AO_SNDOP_GLOBAL_MUTE_BIT;
                Ret |= SndOpSetMute(pSndOp, u32Mute);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            u32Mute = SndOpGetMute(pSndOp);
            u32Mute &= (~(1L << AO_SNDOP_LOCAL_MUTE_BIT));
            u32Mute |= (HI_U32)bMute << AO_SNDOP_LOCAL_MUTE_BIT;
            return SndOpSetMute(pSndOp, u32Mute);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
}

HI_S32 SND_GetOpMute(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbMute)
{
    HI_VOID* pSndOp;
    HI_U32    u32Mute;
    HI_U32    u32MuteBit;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        enOutPort = pCard->stUserOpenParam.stOutport[0].enOutPort;
        u32MuteBit = AO_SNDOP_GLOBAL_MUTE_BIT;
    }
    else
    {
        u32MuteBit = AO_SNDOP_LOCAL_MUTE_BIT;
    }

    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        u32Mute = SndOpGetMute(pSndOp);
        *pbMute = (HI_BOOL)((u32Mute >> u32MuteBit) & 1) ;
        return HI_SUCCESS;
    }
    return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
}

HI_S32 SND_SetOpHdmiMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            pCard->enUserHdmiMode = enMode;
            break;
        default:
            HI_ERR_AO("Hdmi mode don't support OutPort(%d)\n", (HI_U32)enOutPort);
            return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_GetOpHdmiMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E* penMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            *penMode = pCard->enUserHdmiMode;
            break;
        default:
            HI_ERR_AO("Get hdmi mode don't support OutPort(%d)\n", (HI_U32)enOutPort);
            return HI_FAILURE;
    }

    return Ret;
}


HI_S32 SND_SetOpSpdifMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E enMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            pCard->enUserSpdifMode = enMode;
            break;
        default:
            HI_ERR_AO("Spdif mode don't support OutPort(%d)\n", (HI_U32)enOutPort);
            return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_GetOpSpdifMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E* penMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            *penMode = pCard->enUserSpdifMode;
            break;
        default:
            HI_ERR_AO("Get spdif mode don't support OutPort(%d)\n", (HI_U32)enOutPort);
            return HI_FAILURE;
    }

    return Ret;
}

#ifdef __DPT__
HI_S32 SND_SetOpArcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        pCard->bUserArcEnable = bEnable;
        HAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_SPDIF_TX2, bEnable);

        HAL_AIAO_P_SetSPDIFPortEn(AIAO_PORT_SPDIF_TX1, !bEnable);  //Set SPDIF TX 1端口(ARC和SPDIF端口不能同时输出声音)
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Arc Enable Setting\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_GetOpArcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        *pbEnable = pCard->bUserArcEnable;
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Getting Arc Enable\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_SetOpArcMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E enMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        pCard->enUserArcMode = enMode;
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Arc Mode Setting\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_GetOpArcMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E* penMode)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        *penMode = pCard->enUserArcMode;
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Getting Arc Mode\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_SetOpArcCap(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S stCap)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        memcpy(&pCard->stUserArcCap, &stCap, sizeof(HI_UNF_SND_ARC_AUDIO_CAP_S));
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Arc Cap Setting\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}

HI_S32 SND_GetOpArcCap(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S* pstCap)
{
    HI_S32 Ret = HI_SUCCESS;
    if (!SNDGetOpHandleByOutPort(pCard, enOutPort))
    {
        HI_ERR_AO("OutPort(%d) not attatch this card\n", (HI_U32)enOutPort);
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    switch (enOutPort)
    {
    case HI_UNF_SND_OUTPUTPORT_ARC0:
        memcpy(pstCap, &pCard->stUserArcCap, sizeof(HI_UNF_SND_ARC_AUDIO_CAP_S));
        break;
    default:
        HI_ERR_AO("OutPort(%d) don't support Getting Arc Cap\n", (HI_U32)enOutPort);
        return HI_FAILURE;
    }

    return Ret;
}
#endif

HI_S32 SND_SetOpVolume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;
    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetVolume(pSndOp, pstGain);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetVolume(pSndOp, pstGain);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpVolume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get volume of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        pstGain->bLinearMode = ((SND_OP_STATE_S*)pSndOp)->stUserGain.bLinearMode;
        pstGain->s32Gain = ((SND_OP_STATE_S*)pSndOp)->stUserGain.s32Gain;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpSpdifCategoryCode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                  HI_UNF_SND_SPDIF_CATEGORYCODE_E enCategoryCode)
{
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
    {
        HI_ERR_AO("Set spdif Category code don't support OutPort(%d)", (HI_U32)enOutPort);
        return HI_ERR_AO_INVALID_PARA;
    }

    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        return SndOpSetSPDIFCategoryCode(pSndOp, enCategoryCode);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_GetOpSpdifCategoryCode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                  HI_UNF_SND_SPDIF_CATEGORYCODE_E* penCategoryCode)
{
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
    {
        HI_ERR_AO("Get spdif Category code don't support OutPort(%d)", (HI_U32)enOutPort);
        return HI_ERR_AO_INVALID_PARA;
    }

    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        *penCategoryCode = ((SND_OP_STATE_S*)pSndOp)->enUserSPDIFCategoryCode;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpSpdifSCMSMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_SCMSMODE_E enSCMSMode)
{
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
    {
        HI_ERR_AO("Set spdif SCMS mode don't support OutPort(%d)", (HI_U32)enOutPort);
        return HI_ERR_AO_INVALID_PARA;
    }

    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        return SndOpSetSPDIFSCMSMode(pSndOp, enSCMSMode);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_GetOpSpdifSCMSMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_SCMSMODE_E* penSCMSMode)
{
    HI_VOID* pSndOp;

    if (HI_UNF_SND_OUTPUTPORT_SPDIF0 != enOutPort)
    {
        HI_ERR_AO("Get spdif SCMS mode don't support OutPort(%d)", (HI_U32)enOutPort);
        return HI_ERR_AO_INVALID_PARA;
    }

    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        *penSCMSMode = ((SND_OP_STATE_S*)pSndOp)->enUserSPDIFSCMSMode;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SAMPLE_RATE_E enSampleRate)
{
#if 1
    if (!pCard)
    {
        HI_ERR_AO( "Card is not open!\n");
        return HI_FAILURE;
    }
    if (pCard->enUserSampleRate == enSampleRate)
    {
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_AO( "SetSampleRate(%d) failed, can't change samplerate at running!\n", enSampleRate);
        return HI_FAILURE;
    }
#else
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(enSound, enOutPort);
    HI_S32 Ret = HI_FAILURE;
    if (pSndOp)
    {
        return SndOpSetSampleRate(pSndOp, enSampleRate);
    }
    return Ret;
#endif
}

HI_S32 SND_GetOpSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SAMPLE_RATE_E* penSampleRate)
{
    *penSampleRate = pCard->enUserSampleRate;
    return HI_SUCCESS;
}

HI_S32 SND_SetOpTrackMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E enMode)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetTrackMode(pSndOp, enMode);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetTrackMode(pSndOp, enMode);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpTrackMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_TRACK_MODE_E* penMode)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get trackmode of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *penMode = ((SND_OP_STATE_S*)pSndOp)->enUserTrackMode;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_U32 SND_SetLowLatency(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32LatencyMs)
{
    HI_S32 u32PortNum;
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetLowLatency(pSndOp, u32LatencyMs);
            }
        }
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            Ret |= SndOpSetLowLatency(pSndOp, u32LatencyMs);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }

    }
    return Ret;

}


HI_U32 SND_GetLowLatency(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32LatencyMs)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;


    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_DAC);
        if (pSndOp)
        {
            SndOpGetLowLatency(pSndOp, pu32LatencyMs);
            return HI_SUCCESS;
        }

        pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_I2S);
        if (pSndOp)
        {
            SndOpGetLowLatency(pSndOp, pu32LatencyMs);
            return HI_SUCCESS;
        }

        pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_SPDIF);
        if (pSndOp)
        {
            SndOpGetLowLatency(pSndOp, pu32LatencyMs);
            return HI_SUCCESS;
        }

        pSndOp = SND_GetOpHandlebyOutType(pCard, SND_OUTPUT_TYPE_HDMI);
        if (pSndOp)
        {
            SndOpGetLowLatency(pSndOp, pu32LatencyMs);
            return HI_SUCCESS;
        }
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            SndOpGetLowLatency(pSndOp, pu32LatencyMs);
            return HI_SUCCESS;
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_SetPortSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32SampleRate)
{
    HI_S32 s32Ret = HI_SUCCESS;
    AIAO_PORT_ATTR_S stAiaoAttr;
    SND_OP_STATE_S* state = HI_NULL;
    HI_U32 u32PortNum;
    AIAO_PORT_ID_E enPortID;

    // enOutPort is HI_UNF_SND_OUTPUTPORT_ALL
    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        state = (SND_OP_STATE_S*)(pCard->pSndOp[u32PortNum]);
        if (HI_NULL == state)
        {
            continue;
        }

        enPortID = state->enPortID[state->ActiveId];

        s32Ret = HAL_AIAO_P_Stop(enPortID, AIAO_STOP_IMMEDIATE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Stop(%d) failed\n", enPortID);
            return s32Ret;
        }

        s32Ret = HAL_AIAO_P_GetAttr(enPortID, &stAiaoAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_GetAttr(%d) failed\n", enPortID);
            return s32Ret;
        }
        stAiaoAttr.stIfAttr.enRate = (AIAO_SAMPLE_RATE_E)u32SampleRate;

        s32Ret = HAL_AIAO_P_SetAttr(enPortID, &stAiaoAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AO("HAL_AIAO_P_SetAttr(%d) failed\n", enPortID);
            return s32Ret;
        }

        s32Ret = HAL_AIAO_P_Start(enPortID);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("HAL_AIAO_P_Start(%d) failed\n", enPortID);
            return s32Ret;
        }

        if (HI_UNF_SND_OUTPUTPORT_HDMI0 == state->enOutPort)
        {
            HDMI_AUDIO_ATTR_S stHDMIAttr;
            if (pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiGetAoAttr)
            {
                (pCard->pstHdmiFunc->pfnHdmiGetAoAttr)(HI_UNF_HDMI_ID_0, &stHDMIAttr);
            }
            stHDMIAttr.enSampleRate = (HI_UNF_SAMPLE_RATE_E)u32SampleRate;
            if (pCard->pstHdmiFunc && pCard->pstHdmiFunc->pfnHdmiAudioChange)
            {
                (pCard->pstHdmiFunc->pfnHdmiAudioChange)(HI_UNF_HDMI_ID_0, &stHDMIAttr);
            }
        }
    }

    return s32Ret;
}

HI_S32 SND_GetPortInfo(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_PORT_KERNEL_ATTR_S* pstPortKAttr)
{
    SND_OP_STATE_S* state;
    AIAO_RBUF_ATTR_S stRbfAttr = {0};
    AIAO_PORT_ID_E enPortID;
    SND_PORT_KERNEL_ATTR_S stPortKAttr[HI_UNF_SND_OUTPUTPORT_MAX];
    HI_U32 u32PortNum;

    memset(stPortKAttr, 0, sizeof(SND_PORT_KERNEL_ATTR_S) * HI_UNF_SND_OUTPUTPORT_MAX);

    // enOutPort is HI_UNF_SND_OUTPUTPORT_ALL
    for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
    {
        state = (SND_OP_STATE_S*)(pCard->pSndOp[u32PortNum]);
        if (HI_NULL == state)
        {
            stPortKAttr[u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_BUTT;
        }
        else
        {
            stPortKAttr[u32PortNum].enOutPort = state->enOutPort;

            enPortID = state->enPortID[state->ActiveId];
            HAL_AIAO_P_GetRbfAttr(enPortID, &stRbfAttr);
            stPortKAttr[u32PortNum].tPhyDma = stRbfAttr.tBufPhyAddr;
            stPortKAttr[u32PortNum].tPhyWptr = stRbfAttr.tBufPhyWptr;
            stPortKAttr[u32PortNum].tPhyRptr = stRbfAttr.tBufPhyRptr;
            stPortKAttr[u32PortNum].u32Size = stRbfAttr.u32BufSize;
        }
    }
    memcpy(pstPortKAttr, stPortKAttr, sizeof(SND_PORT_KERNEL_ATTR_S) * HI_UNF_SND_OUTPUTPORT_MAX);
    return HI_SUCCESS;
}

#ifdef __DPT__
HI_S32 SND_SetOpPreciVol(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S stPreciGain)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetPreciVol(pSndOp, stPreciGain);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetPreciVol(pSndOp, stPreciGain);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}


HI_S32 SND_GetOpPreciVol(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get precision volume of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        memcpy(pstPreciGain, &(((SND_OP_STATE_S*)pSndOp)->stUserPreciGain), sizeof(HI_UNF_SND_PRECIGAIN_ATTR_S));
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpBalance(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32 s32Balance)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetBalance(pSndOp, s32Balance);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetBalance(pSndOp, s32Balance);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }

    return Ret;
}

HI_S32 SND_GetOpBalance(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32* ps32Balance)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get balance of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *ps32Balance = ((SND_OP_STATE_S*)pSndOp)->s32UserBalance;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}
#endif

HI_S32 SND_SetOpAefBypass(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bBypass)
{
    HI_VOID* pSndOp;
    HI_S32 Ret = HI_SUCCESS;

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_S32 u32PortNum;
        for (u32PortNum = 0; u32PortNum < HI_UNF_SND_OUTPUTPORT_MAX; u32PortNum++)
        {
            pSndOp = pCard->pSndOp[u32PortNum];
            if (pSndOp)
            {
                Ret |= SndOpSetAefBypass(pSndOp, bBypass);
            }
        }
        return Ret;
    }
    else
    {
        pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
        if (pSndOp)
        {
            return SndOpSetAefBypass(pSndOp, bBypass);
        }
        else
        {
            return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
        }
    }
}

HI_S32 SND_GetOpAefBypass(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbBypass)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (HI_UNF_SND_OUTPUTPORT_ALL == enOutPort)
    {
        HI_ERR_AO("Don't support get aef bypass status of allport!\n");
        return HI_ERR_AO_INVALID_PARA;
    }

    if (pSndOp)
    {
        *pbBypass = ((SND_OP_STATE_S*)pSndOp)->bBypass;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetOpAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_OP_ATTR_S* pstSndPortAttr)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (pSndOp)
    {
        return SndOpSetAttr(pSndOp, pstSndPortAttr);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_GetOpAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_OP_ATTR_S* pstSndPortAttr)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    if (pSndOp)
    {
        return SndOpGetAttr(pSndOp, pstSndPortAttr);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

#if 0
HI_S32 SND_GetOpStatus(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, AIAO_PORT_STAUTS_S* pstPortStatus)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    HI_S32 Ret = HI_FAILURE;

    if (pSndOp)
    {
        return SndOpGetStatus(pSndOp, pstPortStatus);
    }

    return Ret;
}
#endif

HI_S32 SND_StopOp(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (pSndOp)
    {
        return SndOpStop(pSndOp, HI_NULL);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_StartOp(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (pSndOp)
    {
        return SndOpStart(pSndOp, HI_NULL);
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }
}

HI_S32 SND_SetAdacEnable(SND_CARD_STATE_S* pCard, HI_BOOL bEnable)
{
    HI_VOID* pSndOp;
    HI_UNF_SND_OUTPUTPORT_E enOutPort;

    enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
    pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);

    if (pSndOp)
    {
        SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

        if (bEnable == HI_TRUE)
        {
#ifdef HI_SND_MUTECTL_SUPPORT
            SndOpEnableMuteCtrl(pCard);
            ADAC_TIANLAI_Init(state->stSndPortAttr.u32SampleRate, HI_FALSE);
            mod_timer(&pCard->stMuteDisableTimer, jiffies + msecs_to_jiffies(AO_SND_MUTE_DISABLE_TIMEMS_RESUME));
#else
            ADAC_TIANLAI_Init(state->stSndPortAttr.u32SampleRate, HI_FALSE);
#endif
        }
        else
        {
#ifdef HI_SND_MUTECTL_SUPPORT
            SndOpEnableMuteCtrl(pCard);
#endif
            ADAC_TIANLAI_DeInit(HI_FALSE);
        }
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    return HI_SUCCESS;
}

HI_VOID SND_GetXRunCount(SND_CARD_STATE_S* pCard, HI_U32* pu32Count)
{
    AIAO_PORT_STAUTS_S stStatus;
    HI_VOID* pSndOp;
    DRV_SND_ATTR_S* pstSndAttr;
    HI_U32 u32EmptyCnt = 0;
    HI_U32 i = 0;
    HI_UNF_SND_OUTPUTPORT_E enPort = HI_UNF_SND_OUTPUTPORT_HDMI0;

    pstSndAttr = &pCard->stUserOpenParam;

    for (i = 0; i < pstSndAttr->u32PortNum; i++)
    {
        enPort = pstSndAttr->stOutport[i].enOutPort;
        pSndOp = SNDGetOpHandleByOutPort(pCard, enPort);
        if (HI_NULL == pSndOp)
        {
            return ;
        }
        SndOpGetStatus(pSndOp, &stStatus);
        u32EmptyCnt += stStatus.stProcStatus.uInfFiFoEmptyCnt;
    }
    *pu32Count = u32EmptyCnt;

}

static HI_U32 Cast_GetSysTime(HI_VOID)
{
    HI_U64   SysTime;

    SysTime = sched_clock();

    do_div(SysTime, 1000000);

    return (HI_U32)SysTime;
}

HI_S32 SND_StopCastOp(SND_CARD_STATE_S* pCard, HI_S32 s32CastID)
{
    HI_VOID* pSndOp = pCard->pCastOp[s32CastID];
    HI_S32 Ret = HI_FAILURE;

    if (pSndOp)
    {
        return SndOpStop(pSndOp, HI_NULL);
    }
    return Ret;
}

HI_S32 SND_StartCastOp(SND_CARD_STATE_S* pCard, HI_S32 s32CastID)
{
    HI_VOID* pSndOp = pCard->pCastOp[s32CastID];
    HI_S32 Ret = HI_FAILURE;

    if (pSndOp)
    {
        return SndOpStart(pSndOp, HI_NULL);
    }
    return Ret;
}

HI_S32 SND_CreateCastOp(SND_CARD_STATE_S* pCard,  HI_S32* ps32CastId, HI_UNF_SND_CAST_ATTR_S* pstAttr, MMZ_BUFFER_S* pstMMz)
{
    HI_VOID* pCastSndOp;
#ifdef __DPT__
    HI_VOID* pSimulatedSndOp;
#endif

#ifdef CASTSIMULATEOP_DEBUG
    //如果定义CASTSIMULATEOP_DEBUG，默认cast捕获I2S1数据
    pCard->bCastSimulateOp = HI_TRUE;
    pCard->enCastSimulatePort = HI_UNF_SND_OUTPUTPORT_I2S1;
#endif

    if (HI_SUCCESS != SndOpCreateCast(&pCastSndOp, ps32CastId, pstAttr, pstMMz))
    {
        HI_ERR_AIAO("SndOpCreateCast Failed\n");
        return HI_FAILURE;
    }

#ifdef __DPT__
    if (pCard->bCastSimulateOp == HI_TRUE)
    {
        pSimulatedSndOp = SNDGetOpHandleByOutPort(pCard, pCard->enCastSimulatePort);
        if (pSimulatedSndOp)
        {
            SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSimulatedSndOp;
            SndOpSetPreciVol(pCastSndOp, state->stUserPreciGain);
            SndOpSetBalance(pCastSndOp, state->s32UserBalance);
            SndOpSetAefBypass(pCastSndOp, state->bBypass);
            SndOpSetDrcEnable(pCastSndOp, state->bDrcEnable);
            SndOpSetDrcAttr(pCastSndOp, &(state->stDrcAttr));
            SndOpSetPeqEnable(pCastSndOp, state->bPeqEnable);
            SndOpSetPeqAttr(pCastSndOp, &(state->stPeqAttr));
            SndOpSetADOutputEnable(pCastSndOp, state->bADOutputEnable);
        }
        else
        {
            HI_ERR_AIAO("CASTSIMULATEOP: SNDGetOpHandleByOutPort(%d) Failed\n", pCard->enCastSimulatePort);
        }
    }
#endif

    pCard->pCastOp[*ps32CastId] = pCastSndOp;

    return HI_SUCCESS;
}

HI_S32 SND_DestoryCastOp(SND_CARD_STATE_S* pCard,  HI_U32 CastId)
{

    if (pCard->pCastOp[CastId])
    {
        SndOpDestroy(pCard->pCastOp[CastId], HI_FALSE);
        pCard->pCastOp[CastId] = HI_NULL;
    }
    else
    {
        HI_ERR_AIAO("SND_DestoryCastOp Null pointer Failed\n");
    }

    return HI_SUCCESS;
}

HI_U32 SND_ReadCastData(SND_CARD_STATE_S* pCard, HI_S32 s32CastId, AO_Cast_Data_Param_S* pstCastData)
{
    SND_OP_STATE_S* state;
    HI_U32 u32ReadBytes = 0;
    HI_U32 u32NeedBytes = 0;

    state = (SND_OP_STATE_S*)pCard->pCastOp[s32CastId];
    if (state)
    {
        if (SND_OUTPUT_TYPE_CAST != state->enOutType)
        {
            return HI_FAILURE;
        }

        u32NeedBytes = pstCastData->u32FrameBytes;
        //HI_ERR_AO( "u32NeedBytes  0x%x  !\n",u32NeedBytes);

        u32ReadBytes = HAL_CAST_ReadData(state->CastId, &pstCastData->u32DataOffset, u32NeedBytes);
#if 0
        if (u32ReadBytes)
        {
            HI_ERR_AO( "u32NeedBytes  0x%x  , u32ReadBytes 0x%x !\n", u32NeedBytes, u32ReadBytes);
        }
#endif

#if 1   //add  pts to cast frame
        pstCastData->stAOFrame.u32PtsMs = Cast_GetSysTime();
#endif

        pstCastData->stAOFrame.u32PcmSamplesPerFrame = u32ReadBytes  / pstCastData->u32SampleBytes;
        //HI_ERR_AO( "pstCastData->u32DataOffset=0x%x ,stAOFrame.u32PcmSamplesPerFrame   0x%x\n",pstCastData->u32DataOffset ,(int)(pstCastData->stAOFrame.u32PcmSamplesPerFrame));
    }

    return u32ReadBytes;
}

HI_U32 SND_ReleaseCastData(SND_CARD_STATE_S* pCard, HI_S32 s32CastId, AO_Cast_Data_Param_S* pstCastData)
{
    SND_OP_STATE_S* state;
    HI_U32 u32RleaseBytes = 0;

    //*state = ( SND_OP_STATE_S *)SNDGetOpHandleByOutPort(pCard, HI_UNF_SND_OUTPUTPORT_CAST);
    state = (SND_OP_STATE_S*)pCard->pCastOp[s32CastId];
    if (state)
    {
        if (SND_OUTPUT_TYPE_CAST != state->enOutType)
        {
            return HI_FAILURE;
        }

        u32RleaseBytes = pstCastData->u32FrameBytes;
        u32RleaseBytes = HAL_CAST_ReleaseData(state->CastId, u32RleaseBytes);
        //HI_ERR_AO( "u32RleaseBytes  0x%x  !\n",u32RleaseBytes);

        return u32RleaseBytes;
    }

    return HI_FAILURE;
}

HI_S32 SND_SetDelayCompensation(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32DelayMs)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (state)
    {
        Ret = SndOpSetLowLatency(pSndOp, u32DelayMs + AOE_AOP_BUFF_LATENCYMS_DF);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AO("SndOpSetLowLatency failed\n");
            return Ret;
        }

        Ret = SndOpSetDelayCompensation(pSndOp, u32DelayMs);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AO("SndOpSetDelayCompensation failed\n");
            return Ret;
        }
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    HI_INFO_AO("Set Audio DelayMs %d Successful\n", u32DelayMs);
    return Ret;
}

HI_S32 SND_GetDelayCompensation(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32DelayMs)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enOutPort);
    SND_OP_STATE_S* state = (SND_OP_STATE_S*)pSndOp;

    if (state)
    {
        Ret = SndOpGetDelayCompensation(pSndOp, pu32DelayMs);
        if (Ret != HI_SUCCESS)
        {
            HI_ERR_AO("SndOpGetDelayCompensation failed\n");
        }
    }
    else
    {
        return HI_ERR_AO_OUTPORT_NOT_ATTATCH;
    }

    return Ret;
}

#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
HI_S32 SndOpGetSetting(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enPort, SND_OUTPORT_ATTR_S* pstPortAttr)
{
    SND_OP_STATE_S* state;

    state = (SND_OP_STATE_S*)SNDGetOpHandleByOutPort(pCard, enPort);
    if (!state)
    {
        return HI_FAILURE;
    }
    pstPortAttr->enCurnStatus = state->enCurnStatus;
    pstPortAttr->u32UserMute = state->u32UserMute;
    pstPortAttr->enUserTrackMode = state->enUserTrackMode;
    pstPortAttr->u32DelayMs = state->u32Delayms;
    pstPortAttr->u32LatencyMs = state->u32LatencyMs;
    pstPortAttr->bBypass = state->bBypass;

    memcpy(&pstPortAttr->stUserGain, &state->stUserGain, sizeof(HI_UNF_SND_GAIN_ATTR_S));

#ifdef __DPT__
    pstPortAttr->s32UserBalance = state->s32UserBalance;
    memcpy(&pstPortAttr->stUserPreciGain, &state->stUserPreciGain, sizeof(HI_UNF_SND_PRECIGAIN_ATTR_S));
    memcpy(&pstPortAttr->stDrcAttr, &state->stDrcAttr, sizeof(HI_UNF_SND_DRC_ATTR_S));
    pstPortAttr->bDrcEnable = state->bDrcEnable;
#endif

    return HI_SUCCESS;
}

HI_S32 SND_GetOpSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32Port;

    pstSndSettings->enUserHdmiMode = pCard->enUserHdmiMode;
    pstSndSettings->enUserSpdifMode = pCard->enUserSpdifMode;
    pstSndSettings->bAdacEnable = pCard->bAdacEnable;
#ifdef __DPT__
    pstSndSettings->bUserArcEnable = pCard->bUserArcEnable;
    pstSndSettings->enUserArcMode = pCard->enUserArcMode;
    memcpy(&pstSndSettings->stUserArcCap, &pCard->stUserArcCap, sizeof(HI_UNF_SND_ARC_AUDIO_CAP_S));
#endif
    memcpy(&pstSndSettings->stUserOpenParam, &pCard->stUserOpenParam, sizeof(DRV_SND_ATTR_S));
    if (&pCard->stUserOpenParamI2s != HI_NULL ) //for i2s only card resume HI_ALSA_I2S_ONLY_SUPPORT
    {
        memcpy(&pstSndSettings->stUserOpenParamI2s, &pCard->stUserOpenParamI2s, sizeof(AO_ALSA_I2S_Param_S));
    }
    for (u32Port = 0; u32Port < pCard->stUserOpenParam.u32PortNum; u32Port++)
    {
        SndOpGetSetting(pCard, pCard->stUserOpenParam.stOutport[u32Port].enOutPort, &pstSndSettings->stPortAttr[u32Port]);
    }

    return HI_SUCCESS;
}

HI_S32 SNDOpRestoreSetting(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enPort, SND_OUTPORT_ATTR_S* pstPortAttr)
{
    HI_VOID* pSndOp = SNDGetOpHandleByOutPort(pCard, enPort);
    if (!pSndOp)
    {
        return HI_FAILURE;
    }
    if (SND_OP_STATUS_START == pstPortAttr->enCurnStatus)
    {
        SndOpStart(pSndOp, HI_NULL);
    }
    else if (SND_OP_STATUS_STOP == pstPortAttr->enCurnStatus)
    {
        SndOpStop(pSndOp, HI_NULL);
    }

    SndOpSetLowLatency(pSndOp, pstPortAttr->u32LatencyMs);
    SndOpSetMute(pSndOp, pstPortAttr->u32UserMute);
    SndOpSetTrackMode(pSndOp, pstPortAttr->enUserTrackMode);
    SndOpSetVolume(pSndOp, &pstPortAttr->stUserGain);

    SndOpSetAefBypass(pSndOp, pstPortAttr->bBypass);
    SndOpSetDelayCompensation(pSndOp, pstPortAttr->u32DelayMs);

#ifdef __DPT__
    SndOpSetPreciVol(pSndOp, pstPortAttr->stUserPreciGain);
    SndOpSetBalance(pSndOp, pstPortAttr->s32UserBalance);
    SndOpSetDrcEnable(pSndOp, pstPortAttr->bDrcEnable);
    SndOpSetDrcAttr(pSndOp, &(pstPortAttr->stDrcAttr));
#endif

    return HI_SUCCESS;
}

HI_S32 SND_RestoreOpSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings)
{
    HI_U32 u32Port;

    pCard->enUserHdmiMode = pstSndSettings->enUserHdmiMode;
    pCard->enUserSpdifMode = pstSndSettings->enUserSpdifMode;
    pCard->bAdacEnable = pstSndSettings->bAdacEnable;
#ifdef __DPT__
    pCard->bUserArcEnable = pstSndSettings->bUserArcEnable;
    pCard->enUserArcMode = pstSndSettings->enUserArcMode;
    memcpy(&pCard->stUserArcCap, &pstSndSettings->stUserArcCap, sizeof(HI_UNF_SND_ARC_AUDIO_CAP_S));
#endif
    if (&pstSndSettings->stUserOpenParam != HI_NULL ) //for i2s only card resume HI_ALSA_I2S_ONLY_SUPPORT
    {
        memcpy(&pCard->stUserOpenParamI2s, &pstSndSettings->stUserOpenParamI2s, sizeof(AO_ALSA_I2S_Param_S));
    }
    memcpy(&pCard->stUserOpenParam, &pstSndSettings->stUserOpenParam, sizeof(DRV_SND_ATTR_S));

    for (u32Port = 0; u32Port < pstSndSettings->stUserOpenParam.u32PortNum; u32Port++)
    {
        SNDOpRestoreSetting(pCard, pstSndSettings->stUserOpenParam.stOutport[u32Port].enOutPort, &pstSndSettings->stPortAttr[u32Port]);
    }

    if (HI_FALSE == pCard->bAdacEnable)
    {
        SND_SetAdacEnable(pCard, pCard->bAdacEnable);
    }
    return HI_SUCCESS;
}
#endif

#if (1 == HI_PROC_SUPPORT)
static inline const HI_CHAR* AOPort2Name(HI_UNF_SND_OUTPUTPORT_E enPort)
{
    switch (enPort)
    {
        case HI_UNF_SND_OUTPUTPORT_DAC0:
            return "DAC0";
        case HI_UNF_SND_OUTPUTPORT_I2S0:
            return "I2S0";
        case HI_UNF_SND_OUTPUTPORT_I2S1:
            return "I2S1";
        case HI_UNF_SND_OUTPUTPORT_SPDIF0:
            return "SPDIF0";
        case HI_UNF_SND_OUTPUTPORT_HDMI0:
            return "HDMI0";
        case HI_UNF_SND_OUTPUTPORT_ARC0:
            return "ARC0";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC1:
            return "DAC1";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC2:
            return "DAC2";
        case HI_UNF_SND_OUTPUTPORT_EXT_DAC3:
            return "DAC3";
        default:
            return "UnknownPort";
    }
}

HI_S32 SND_ReadOpProc(struct seq_file* p, SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enPort)
{
    AIAO_PORT_STAUTS_S stStatus;
    HI_VOID* pSndOp;
    SND_OP_STATE_S* state;
    //SND_OP_ATTR_S stOpAttr;

    pSndOp = SNDGetOpHandleByOutPort(pCard, enPort);
    if (HI_NULL == pSndOp)
    {
        return HI_FAILURE;
    }

    memset(&stStatus, 0, sizeof(AIAO_PORT_STAUTS_S));
    if (HI_SUCCESS != SndOpGetStatus(pSndOp, &stStatus))
    {
        return HI_FAILURE;
    }

    state = (SND_OP_STATE_S*)pSndOp;
#if 0
    if (HI_SUCCESS != SND_GetOpAttr(enSnd, enPort, &stOpAttr))
    {
        return HI_FAILURE;
    }
#endif
    PROC_PRINT(p,
               /* "%s: Status(%s), Mute(%s), Vol(%d%s), TrackMode(%s), PreciVol(%s%d.%ddB), Balance(%d), AefBypass(%s)\n", */
               "%s: Status(%s), Mute(%s), Vol(%d%s), TrackMode(%s), AefBypass(%s), DelayComps(%dms)\n",
               AUTIL_Port2Name(enPort),
               (HI_CHAR*)((AIAO_PORT_STATUS_START == stStatus.enStatus) ? "start" : ((AIAO_PORT_STATUS_STOP == stStatus.enStatus) ? "stop" : "stopping")),
               (0 == state->u32UserMute) ? "off" : "on",
               state->stUserGain.s32Gain,
               (HI_TRUE == state->stUserGain.bLinearMode) ? "" : "dB",
               AUTIL_TrackMode2Name(state->enUserTrackMode),
#ifdef __DPT__
               (0 == state->stUserPreciGain.s32IntegerGain && state->stUserPreciGain.s32DecimalGain < 0) ? "-" : "",
               state->stUserPreciGain.s32IntegerGain,
               (state->stUserPreciGain.s32DecimalGain < 0) ? (-state->stUserPreciGain.s32DecimalGain) : state->stUserPreciGain.s32DecimalGain,
               state->s32UserBalance,
#endif
               (HI_TRUE == state->bBypass) ? "on" : "off",
               state->u32Delayms);
#ifdef __DPT__
    if (HI_TRUE == state->bDrcEnable)
    {
        PROC_PRINT(p,
                   "      DRC(AttackTime %dms, ReleaseTime %dms, Thrhd %ddB, Limit %ddB)\n",
                   state->stDrcAttr.u32AttackTime,
                   state->stDrcAttr.u32ReleaseTime,
                   state->stDrcAttr.s32Thrhd,
                   state->stDrcAttr.s32Limit);
    }

    if (HI_TRUE == state->bPeqEnable)
    {
        HI_U32 i = 0;
        PROC_PRINT( p, "      PEQ Attr:");
        for (i = 0; i < state->stPeqAttr.u32BandNum; i++)
        {
            PROC_PRINT( p,
                        "Band%d(%s,%dHz,%d.%.3ddB,%d.%.1dQ) ",
                        i,
                        AUTIL_PEQType2Name(state->stPeqAttr.stEqParam[i].enFltType),
                        state->stPeqAttr.stEqParam[i].u32freq,
                        state->stPeqAttr.stEqParam[i].s32Gain / 1000,
                        state->stPeqAttr.stEqParam[i].s32Gain % 1000,
                        state->stPeqAttr.stEqParam[i].u32Q / 10,
                        state->stPeqAttr.stEqParam[i].u32Q % 10);

            if (0 == ((i + 1) % 3) && (state->stPeqAttr.u32BandNum != (i + 1))) //a line 3 Band
            {
                PROC_PRINT( p, "\n      ");
            }
        }
        PROC_PRINT( p, "\n");
    }
#endif
    if (HI_UNF_SND_OUTPUTPORT_SPDIF0 == enPort)
    {
        PROC_PRINT(p,
                   "      CategoryCode(%s), ScmsMode(%s)\n",
                   AUTIL_CategoryCode2Name(state->enUserSPDIFCategoryCode),
                   AUTIL_ScmsMode2Name(state->enUserSPDIFSCMSMode));
    }

    PROC_PRINT(p,
               "      SampleRate(%.6d), Channel(%.2d), BitWidth(%2d), *Engine(%s), *AOP(0x%x), *PortID(0x%x), ADOutput(%s)\n",
               stStatus.stUserConfig.stIfAttr.enRate,
               stStatus.stUserConfig.stIfAttr.enChNum,
               stStatus.stUserConfig.stIfAttr.enBitDepth,
               AUTIL_Engine2Name(state->enEngineType[state->ActiveId]),
               (HI_U32)state->enAOP[state->ActiveId],
               (HI_U32)state->enPortID[state->ActiveId],
               (HI_TRUE == state->bADOutputEnable) ? "on" : "off");

    PROC_PRINT(p,
               "      DmaCnt(%.6u), BufEmptyCnt(%.6u), FiFoEmptyCnt(%.6u)\n\n",
               stStatus.stProcStatus.uDMACnt,
               stStatus.stProcStatus.uBufEmptyCnt,
               stStatus.stProcStatus.uInfFiFoEmptyCnt);

    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_S32 SND_WriteOpProc_AefBypass(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR* pcBuf)
{
    HI_CHAR *pcOnCmd = "on";
    HI_CHAR *pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_BOOL  bSetAefBypass;
    HI_BOOL  bGetAefBypass;
    HI_S32   Ret;

    if (pcBuf == strstr(pcBuf, pcOnCmd) || pcBuf == strstr(pcBuf, pcOneCmd))
    {
        bSetAefBypass = HI_TRUE;
    }
    else if ((pcBuf == strstr(pcBuf, pcOffCmd)) || (pcBuf == strstr(pcBuf, pcZeroCmd)))
    {
        bSetAefBypass = HI_FALSE;
    }
    else
    {
        return HI_FAILURE;
    }

    Ret = SND_GetOpAefBypass(pCard, enOutPort, &bGetAefBypass);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_PROC_EchoHelper("SND_GetOpAefBypass failed\n");
        return Ret;
    }

    Ret = SND_SetOpAefBypass(pCard, enOutPort, bSetAefBypass);
    if (HI_SUCCESS != Ret)
    {
        HI_DRV_PROC_EchoHelper("SND_SetOpAefBypass failed\n");
    }
    else
    {
        if (bSetAefBypass)
        {
            HI_DRV_PROC_EchoHelper("Set %s aef bypass ON success:%d(%s) -> 1(ON)\n", AOPort2Name(enOutPort), (HI_U32)bGetAefBypass, (bGetAefBypass ? "ON" : "OFF"));
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s aef bypass OFF success:%d(%s) -> 0(OFF)\n", AOPort2Name(enOutPort), (HI_U32)bGetAefBypass, (bGetAefBypass ? "ON" : "OFF"));
        }
    }

    return Ret;
}

static HI_S32 SND_WriteOpProc_DRC(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR* pcBuf)
{
    HI_CHAR *pcOnCmd = "on";
    HI_CHAR *pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_CHAR *pcAttackTime = "attacktime";
    HI_CHAR *pcReleaseTime = "releasetime";
    HI_CHAR *pcThrhd = "thrhd";
    HI_CHAR *pcLimit = "limit";
    HI_BOOL  bGetDrcEn;
    HI_S32   Ret;

    if ((pcBuf == strstr(pcBuf, pcOnCmd)) || (pcBuf == strstr(pcBuf, pcOneCmd)))
    {
        Ret = SND_GetOpDrcEnable(pCard, enOutPort, &bGetDrcEn);
        if (HI_SUCCESS != Ret)
        {
            HI_DRV_PROC_EchoHelper("SND_GetOpDrcEnable failed!\n");
            return Ret;
        }
        else
        {
            Ret = SND_SetOpDrcEnable(pCard, enOutPort, HI_TRUE);
            if (HI_SUCCESS != Ret)
            {
                HI_DRV_PROC_EchoHelper("SND_GetOpDrcEnable failed!\n");
                return Ret;
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc ON success: %s -> 1(ON)\n", AOPort2Name(enOutPort), (HI_U32)bGetDrcEn, (bGetDrcEn ? "1(ON)" : "0(OFF)"));
                return Ret;
            }
        }
    }
    else if ((pcBuf == strstr(pcBuf, pcOffCmd)) || (pcBuf == strstr(pcBuf, pcZeroCmd)))
    {
        Ret = SND_GetOpDrcEnable(pCard, enOutPort, &bGetDrcEn);
        if (HI_SUCCESS != Ret)
        {
            HI_DRV_PROC_EchoHelper("SND_GetOpDrcEnable failed!\n");
            return Ret;
        }
        else
        {
            Ret = SND_SetOpDrcEnable(pCard, enOutPort, HI_FALSE);
            if (HI_SUCCESS != Ret)
            {
                HI_DRV_PROC_EchoHelper("SND_GetOpDrcEnable failed!\n");
                return Ret;
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc OFF success: %s -> 0(OFF)\n", AOPort2Name(enOutPort), (HI_U32)bGetDrcEn, (bGetDrcEn ? "1(ON)" : "0(OFF)"));
                return Ret;
            }
        }

    }
    else
    {
        HI_UNF_SND_DRC_ATTR_S stSetDrcAttr;
        HI_UNF_SND_DRC_ATTR_S stGetDrcAttr;
        HI_UNF_SND_DRC_ATTR_S *pstDrcAttr = &stSetDrcAttr;
        SND_GetOpDrcAttr(pCard, enOutPort, &stSetDrcAttr);
        SND_GetOpDrcAttr(pCard, enOutPort, &stGetDrcAttr);

        if((pcBuf == strstr(pcBuf,pcAttackTime)))
        {
            pcBuf += strlen(pcAttackTime);
            AO_STRING_SKIP_BLANK(pcBuf);
            stSetDrcAttr.u32AttackTime = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if ((stSetDrcAttr.u32AttackTime < 20) || (stSetDrcAttr.u32AttackTime > 8000))
            {
                HI_DRV_PROC_EchoHelper("Invalid attacktime value! Attacktime value range: 20ms~8000ms\n");
                return HI_FAILURE;
            }

            CHECK_AO_DRCATTR(pstDrcAttr);
            Ret = SND_SetOpDrcAttr(pCard, enOutPort, &stSetDrcAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("SND_SetOpDrcAttr failed!\n");
                return Ret;
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc attacktime success: %dms -> %dms\n", AOPort2Name(enOutPort), stGetDrcAttr.u32AttackTime, stSetDrcAttr.u32AttackTime);
                return Ret;
            }

        }
        else if ((pcBuf == strstr(pcBuf, pcReleaseTime)))
        {
            pcBuf += strlen(pcReleaseTime);
            AO_STRING_SKIP_BLANK(pcBuf);
            stSetDrcAttr.u32ReleaseTime = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if ((stSetDrcAttr.u32ReleaseTime < 20) || (stSetDrcAttr.u32ReleaseTime > 2000))
            {
                HI_DRV_PROC_EchoHelper("Invalid releasetime value! Releasetime value range: 20ms~2000ms\n");
                return HI_FAILURE;
            }

            CHECK_AO_DRCATTR(pstDrcAttr);
            Ret = SND_SetOpDrcAttr(pCard, enOutPort, &stSetDrcAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("SND_SetOpDrcAttr failed!\n");
                return Ret;
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc releasetime success: %dms -> %dms\n", AOPort2Name(enOutPort), stGetDrcAttr.u32ReleaseTime, stSetDrcAttr.u32ReleaseTime);
                return Ret;
            }
        }
        else if (pcBuf == strstr(pcBuf, pcThrhd))
        {
            pcBuf += strlen(pcThrhd);
            AO_STRING_SKIP_BLANK(pcBuf);
            stSetDrcAttr.s32Thrhd = simple_strtol(pcBuf, &pcBuf, 10);
            if ((stSetDrcAttr.s32Thrhd < -80) || (stSetDrcAttr.s32Thrhd > -1))
            {
                HI_DRV_PROC_EchoHelper("Invalid threshold value! Threshold value range: -80dB~-1dB\n");
                return HI_FAILURE;
            }

            CHECK_AO_DRCATTR(pstDrcAttr);
            Ret = SND_SetOpDrcAttr(pCard, enOutPort, &stSetDrcAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("SND_SetOpDrcAttr failed!\n");
                return Ret;
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc threshold value success: %ddB -> %ddB\n", AOPort2Name(enOutPort), stGetDrcAttr.s32Thrhd, stSetDrcAttr.s32Thrhd);
                return Ret;
            }
        }
        else if (pcBuf == strstr(pcBuf, pcLimit))
        {
            pcBuf += strlen(pcLimit);
            AO_STRING_SKIP_BLANK(pcBuf);
            stSetDrcAttr.s32Limit = simple_strtol(pcBuf, &pcBuf, 10);
            if ((stSetDrcAttr.s32Limit < -80) || (stSetDrcAttr.s32Limit > -1))
            {
                HI_DRV_PROC_EchoHelper("Invalid limit value! Limit value range: -80dB~-1dB\n");
                return HI_FAILURE;
            }

            CHECK_AO_DRCATTR(pstDrcAttr);
            Ret = SND_SetOpDrcAttr(pCard, enOutPort, &stSetDrcAttr);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AO("SND_SetOpDrcAttr failed!\n");
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set %s drc limit value success: %d -> %d\n", AOPort2Name(enOutPort), stGetDrcAttr.s32Limit, stSetDrcAttr.s32Limit);
            }

            return Ret;

        }
        else
        {
            return HI_FAILURE;
        }
    }
}


static HI_S32 SND_WriteOpProc_PEQ(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR* pcBuf)
{
    HI_CHAR* pcOnCmd = "on";
    HI_CHAR* pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_CHAR* pcBand = "band";
    HI_CHAR* pcFreq = "freq";
    HI_CHAR* pcGain = "gain";
    HI_CHAR* pcType = "type";
    HI_CHAR* pcQ = "Q";
    HI_U32   u32BandId;
    HI_U32   u32Val1, u32Val2;
    HI_S32   s32Ret;
    HI_UNF_PEQ_FILTER_TYPE_E enType;
    HI_BOOL  bNagetive = HI_FALSE;
    HI_BOOL  bPeqEnable = HI_FALSE;
    HI_UNF_SND_PEQ_ATTR_S stPeqAttr = {0};

    if ((pcBuf == strstr(pcBuf,pcOnCmd)) || (pcBuf == strstr(pcBuf,pcOneCmd)))
    {
        s32Ret = SND_GetOpPeqEnable(pCard, enOutPort, &bPeqEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get PEQ enable failed!\n");
        }

        s32Ret = SND_SetOpPeqEnable(pCard, enOutPort, HI_TRUE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set PEQ enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s PEQ success: %s -> 1(ON)\n", AOPort2Name(enOutPort), bPeqEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else if ((pcBuf == strstr(pcBuf,pcOffCmd)) || (pcBuf == strstr(pcBuf,pcZeroCmd)))
    {
        s32Ret = SND_GetOpPeqEnable(pCard, enOutPort, &bPeqEnable);
        if (HI_SUCCESS != s32Ret)
        {
            HI_DRV_PROC_EchoHelper("Get PEQ enable failed!\n");
        }

        s32Ret = SND_SetOpPeqEnable(pCard, enOutPort, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AO("Set PEQ enable failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s PEQ success: %s -> 0(OFF)\n", AOPort2Name(enOutPort), bPeqEnable ? "1(ON)" : "0(OFF)");
        }

        return s32Ret;
    }
    else if (pcBuf == strstr(pcBuf, pcBand))
    {
        SND_GetOpPeqAttr(pCard, enOutPort, &stPeqAttr);

        pcBuf += strlen(pcBand);
        u32BandId = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
        if (u32BandId >= stPeqAttr.u32BandNum)
        {
            return HI_FAILURE;
        }
        AO_STRING_SKIP_BLANK(pcBuf);

        if (pcBuf == strstr(pcBuf, pcFreq))
        {
            pcBuf += strlen(pcFreq);
            AO_STRING_SKIP_BLANK(pcBuf);
            u32Val1 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            stPeqAttr.stEqParam[u32BandId].u32freq = u32Val1;
            return SND_SetOpPeqAttr(pCard, enOutPort, &stPeqAttr);
        }
        else if (pcBuf == strstr(pcBuf, pcGain))
        {
            pcBuf += strlen(pcGain);
            AO_STRING_SKIP_BLANK(pcBuf);
            if (pcBuf[0] == '-')
            {
                bNagetive = HI_TRUE;
                pcBuf++;
            }
            u32Val1 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if (pcBuf[0] == '.')
            {
                pcBuf++;
                u32Val2 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            }
            else
            {
                u32Val2 = 0;
            }
            stPeqAttr.stEqParam[u32BandId].s32Gain = (HI_TRUE == bNagetive) ? (-(u32Val1 * 1000 + u32Val2)) : (u32Val1 * 1000 + u32Val2);
            return SND_SetOpPeqAttr(pCard, enOutPort, &stPeqAttr);
        }
        else if (pcBuf == strstr(pcBuf, pcType))
        {
            pcBuf += strlen(pcType);
            AO_STRING_SKIP_BLANK(pcBuf);
            enType = AUTIL_PEQTypeName2Type(pcBuf);
            if (HI_UNF_PEQ_FILTER_TYPE_BUTT == enType)
            {
                return HI_FAILURE;
            }
            stPeqAttr.stEqParam[u32BandId].enFltType = enType;
            return SND_SetOpPeqAttr(pCard, enOutPort, &stPeqAttr);
        }
        else if (pcBuf == strstr(pcBuf, pcQ))
        {
            pcBuf += strlen(pcQ);
            AO_STRING_SKIP_BLANK(pcBuf);
            u32Val1 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if (pcBuf[0] == '.')
            {
                pcBuf++;
                u32Val2 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            }
            else
            {
                u32Val2 = 0;
            }
            stPeqAttr.stEqParam[u32BandId].u32Q = u32Val1 * 10 + u32Val2;
            return SND_SetOpPeqAttr(pCard, enOutPort, &stPeqAttr);
        }
        else
        {
            return HI_FAILURE; //TODO
        }
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 SND_WriteOpProc_Mute(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR* pcBuf)
{
    HI_CHAR *pcOnCmd = "on";
    HI_CHAR *pcOffCmd = "off";
    HI_CHAR *pcOneCmd = "1";
    HI_CHAR *pcZeroCmd = "0";
    HI_BOOL  bSetPortMute;
    HI_BOOL  bGetPortMute;
    HI_S32   Ret;

    if ((pcBuf == strstr(pcBuf,pcOnCmd)) || (pcBuf == strstr(pcBuf,pcOneCmd)))
    {
        bSetPortMute = HI_TRUE;
    }
    else if ((pcBuf == strstr(pcBuf,pcOffCmd)) || (pcBuf == strstr(pcBuf,pcZeroCmd)))
    {
        bSetPortMute = HI_FALSE;
    }
    else
    {
        return HI_FAILURE;
    }

    Ret = SND_GetOpMute(pCard, enOutPort, &bGetPortMute);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_GetOpMute failed!\n");
        return Ret;
    }

    Ret = SND_SetOpMute(pCard, enOutPort, bSetPortMute);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_SetOpMute failed!\n");
    }
    else
    {
        if (bSetPortMute)
        {
            HI_DRV_PROC_EchoHelper("Set %s mute ON success:%d(%s) -> 1(ON)\n", AOPort2Name(enOutPort), (HI_U32)bGetPortMute, (bGetPortMute ? "ON" : "OFF"));
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s mute OFF success:%d(%s) -> 0(OFF)\n",AOPort2Name(enOutPort),(HI_U32)bGetPortMute, (bGetPortMute ? "ON" : "OFF"));
        }
    }

    return Ret;

}

static HI_S32 SND_WriteOpProc_Balance(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR *pcBuf)
{
    HI_S32 s32SetBalance = 0;
    HI_S32 s32GetBalance = 0;
    HI_S32 Ret;

    s32SetBalance = simple_strtol(pcBuf, &pcBuf, 10);

    CHECK_AO_BALANCE(s32SetBalance);

    Ret = SND_GetOpBalance(pCard, enOutPort, &s32GetBalance);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_GetOpMute failed!\n");
        return Ret;
    }

    Ret = SND_SetOpBalance(pCard, enOutPort, s32SetBalance);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_SetOpBalance failed!\n");
    }
    else
    {
        Ret = SND_GetOpBalance(pCard, enOutPort, &s32SetBalance);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_GetOpMute failed!\n");
            return Ret;
        }
        HI_DRV_PROC_EchoHelper("Set %s balance success:%d -> %d\n", AOPort2Name(enOutPort), s32GetBalance, s32SetBalance);
    }
    return Ret;
}

static HI_S32 SND_WriteOpProc_Volume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR *pcBuf)
{
    HI_CHAR *pcdBUnit = "dB";
    HI_S32  s32Vol = 0;
    HI_S32  Ret;

    if (strstr(pcBuf, pcdBUnit))
    {
        HI_UNF_SND_GAIN_ATTR_S stSetGain;
        HI_UNF_SND_GAIN_ATTR_S stGetGain;
        s32Vol = simple_strtol(pcBuf, &pcBuf, 10);
        stSetGain.bLinearMode = HI_FALSE;
        stSetGain.s32Gain = s32Vol;

        CHECK_AO_ABSLUTEVOLUME(stSetGain.s32Gain);

        Ret = SND_GetOpVolume(pCard, enOutPort, &stGetGain);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_GetOpVolume failed!\n");
            return Ret;
        }

        Ret = SND_SetOpVolume(pCard, enOutPort, stSetGain);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_SetOpVolume failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s volume success:%d -> %d\n", AOPort2Name(enOutPort), stGetGain.s32Gain, stSetGain.s32Gain);
        }

        return Ret;
    }
    else
    {
        HI_DRV_PROC_EchoHelper("Invalid value! Please add unit, example: 0dB\n");
        return HI_FAILURE;
    }
}

static HI_S32 SND_WriteOpProc_Delay(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR *pcBuf)
{
    HI_U32 u32SetDelay = 0;
    HI_U32 u32GetDelay = 0;
    HI_S32 Ret;

    u32SetDelay = simple_strtoul(pcBuf, &pcBuf, 10);

    //CHECK_AO_DELAYCOMPS(s32SetDelay);

    Ret = SND_GetDelayCompensation(pCard, enOutPort, &u32GetDelay);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_GetDelayCompensation failed!\n");
        return Ret;
    }

    Ret = SND_SetDelayCompensation(pCard, enOutPort, u32SetDelay);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_AO("SND_SetDelayCompensation failed!\n");
    }
    else
    {
        Ret = SND_GetDelayCompensation(pCard, enOutPort, &u32SetDelay);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_GetDelayCompensation failed!\n");
            return Ret;
        }
        HI_DRV_PROC_EchoHelper("Set %s delay compensation success:%d -> %d\n", AOPort2Name(enOutPort), u32GetDelay, u32SetDelay);
    }
    return Ret;
}


static HI_S32 SND_WriteOpProc_PreciVolume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR *pcBuf)
{
    //HI_CHAR *pcdBUnit = "dB";
    HI_BOOL  bNagetive = HI_FALSE;
    HI_U32   u32Val1, u32Val2 = 0;
    HI_S32   Ret;
#if 0
    if (strstr(pcBuf, pcdBUnit))
    {
#endif
        HI_UNF_SND_PRECIGAIN_ATTR_S stSetPreciGain;
        HI_UNF_SND_PRECIGAIN_ATTR_S stGetPreciGain;

        if(pcBuf[0] == '-')
        {
            bNagetive = HI_TRUE;
            pcBuf++;
        }
        u32Val1 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
        if(pcBuf[0] == '.')
        {
            pcBuf++;
            u32Val2 = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);
            if (u32Val2%125)
            {
                HI_DRV_PROC_EchoHelper("Invalid value! Step by 0.125dB. \n");
                return HI_FAILURE;
            }
        }
        else
        {
            u32Val2 = 0;
        }
#if 0
        if (HI_TRUE == bNagetive)
        {
            if (((stSetPreciGain.s32IntegerGain)*1000 + (stSetPreciGain.s32DecimalGain)) > 81000)
            {
                HI_DRV_PROC_EchoHelper("Invalid value! Track prescale value range:-81dB~18dB\n");
                return HI_FAILURE;
            }
        }
        else
        {
            if (((stSetPreciGain.s32IntegerGain)*1000 + (stSetPreciGain.s32DecimalGain)) > 18000)
            {
                HI_DRV_PROC_EchoHelper("Invalid value! Track prescale value range:-81dB~18dB\n");
                return HI_FAILURE;
            }

        }
#endif
        stSetPreciGain.s32IntegerGain = (HI_TRUE == bNagetive) ? (-u32Val1) : u32Val1;
        stSetPreciGain.s32DecimalGain = (HI_TRUE == bNagetive) ? (-u32Val2) : u32Val2;

        CHECK_AO_ABSLUTEPRECIVOLUME(stSetPreciGain.s32IntegerGain, stSetPreciGain.s32DecimalGain);

        Ret = SND_GetOpPreciVol(pCard, enOutPort, &stGetPreciGain);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_GetOpPreciVol failed!\n");
            return Ret;
        }
        else
        {
            if (stGetPreciGain.s32DecimalGain < 0)
                stGetPreciGain.s32DecimalGain = -stGetPreciGain.s32DecimalGain;
        }

        Ret = SND_SetOpPreciVol(pCard, enOutPort, stSetPreciGain);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AO("SND_SetOpPreciVol failed!\n");
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set %s prescale success:%d.%d -> %d.%d\n", AOPort2Name(enOutPort), stGetPreciGain.s32IntegerGain, stGetPreciGain.s32DecimalGain,
                stSetPreciGain.s32IntegerGain, u32Val2);
        }

        return Ret;
#if 0
    }
    else
    {
        HI_DRV_PROC_EchoHelper("Invalid value! Please add unit, example: 0dB\n");
        return HI_FAILURE;
    }
#endif
}


HI_S32 SND_WriteOpProc(SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf)
{
    HI_U32 u32CmdId;
    HI_UNF_SND_OUTPUTPORT_E enOutPort;

    SND_OP_WRITE_PROC_S stOpWritePoc[] =
    {
        {"aefbypass", SND_WriteOpProc_AefBypass},
#ifdef __DPT__
        {"drc", SND_WriteOpProc_DRC},
#endif
        {"peq", SND_WriteOpProc_PEQ},
        {"mute", SND_WriteOpProc_Mute},
        {"balance", SND_WriteOpProc_Balance},
        {"volume", SND_WriteOpProc_Volume},
        {"precivolume", SND_WriteOpProc_PreciVolume},
        {"delaycomps", SND_WriteOpProc_Delay}
    };

    enOutPort = AUTIL_PortName2Port(pcBuf);
    AO_STRING_SKIP_NON_BLANK(pcBuf);
    AO_STRING_SKIP_BLANK(pcBuf);

    for (u32CmdId = 0; u32CmdId < sizeof(stOpWritePoc) / sizeof(stOpWritePoc[0]); u32CmdId++)
    {
        if (pcBuf == strstr(pcBuf, stOpWritePoc[u32CmdId].pCmd))
        {
            pcBuf += strlen(stOpWritePoc[u32CmdId].pCmd);
            AO_STRING_SKIP_BLANK(pcBuf);
            return stOpWritePoc[u32CmdId].func(pCard, enOutPort, pcBuf);
        }
    }

    return HI_FAILURE;
}
#endif
#endif

