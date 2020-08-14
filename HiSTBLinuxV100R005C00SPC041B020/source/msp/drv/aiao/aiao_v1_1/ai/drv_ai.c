/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: ai_intf_k.c
 * Description: ai interface of module.
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
#include "hi_drv_file.h"
#include "hi_drv_ai.h"

#include "hi_module.h"
#include "hi_drv_mmz.h"
#include "hi_drv_sys.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "hi_error_mpi.h"

#include "hal_aiao.h"
#include "audio_util.h"

#include "hi_drv_ai.h"
#include "drv_ai_private.h"
#include "drv_ai_ioctl.h"
#include "hal_tianlai_adac_v510.h"

#include "drv_ao_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HI_AI_DRV_SUSPEND_SUPPORT

DEFINE_SEMAPHORE(g_AIMutex);

#if (1 == HI_PROC_SUPPORT)
static HI_S32 AI_RegProc(HI_U32 u32Ai);
static HI_VOID AI_UnRegProc(HI_U32 u32Ai);
#endif

extern HI_S32 AO_Track_GetDelayMs(HI_U32 u32TrackID, HI_U32* pu32DelayMs);
#ifdef __DPT__
extern HI_S32 AO_Track_SetNRAttr(HI_U32 u32TrackID, HI_UNF_AI_NR_ATTR_S* pstNRParam);
#endif

static AI_GLOBAL_PARAM_S g_stAIDrv =
{
#if (1 == HI_PROC_SUPPORT)
    .pstProcParam         = HI_NULL,
#endif
    .stExtFunc            =
    {
        .pfnAI_DrvResume  = AI_DRV_Resume,
        .pfnAI_DrvSuspend = AI_DRV_Suspend,
    },

    .atmOpenCnt           = ATOMIC_INIT(0),
};

#ifdef HI_ALSA_AI_SUPPORT
#include <sound/pcm.h>
static AI_ALSA_Param_S g_stAlsaAttr;
#endif

static HI_BOOL AICheckPortValid(HI_UNF_AI_E enAiPort)
{
#if    defined(CHIP_TYPE_hi3798mv100)   \
    || defined(CHIP_TYPE_hi3796mv100)   \
    || defined(CHIP_TYPE_hi3716dv100)   \
    || defined(CHIP_TYPE_hi3798cv200_a) \
    || defined(CHIP_TYPE_hi3798cv200_b) \
    || defined(CHIP_TYPE_hi3798cv200)   \
    || defined(CHIP_TYPE_hi3716mv420)   \
    || defined(CHIP_TYPE_hi3798mv200)   \
    || defined(CHIP_TYPE_hi3798mv200_a) \
    || defined(CHIP_TYPE_hi3796mv200)
    if (HI_UNF_AI_I2S0 != enAiPort)
    {
        HI_ERR_AI("just support I2S0 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v100)  || defined(CHIP_TYPE_hi3751v100b)
    if ((HI_UNF_AI_I2S0 != enAiPort) && (HI_UNF_AI_I2S1 != enAiPort) && (HI_UNF_AI_SIF0 != enAiPort)
        && (HI_UNF_AI_ADC0 != enAiPort) && (HI_UNF_AI_ADC1 != enAiPort) && (HI_UNF_AI_ADC2 != enAiPort)
        && (HI_UNF_AI_ADC3 != enAiPort) && (HI_UNF_AI_ADC4 != enAiPort) && (HI_UNF_AI_HDMI3 != enAiPort)
        && (HI_UNF_AI_HDMI0 != enAiPort) && (HI_UNF_AI_HDMI1 != enAiPort) && (HI_UNF_AI_HDMI2 != enAiPort))
    {
        HI_ERR_AI("just support I2S0, I2S1 , SIF, HDMI, ADC0/ADC1/ADC2/ADC3/ADC4 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v600) || defined(CHIP_TYPE_hi3751lv500)|| defined(CHIP_TYPE_hi3751v620)
    if ((HI_UNF_AI_I2S0 != enAiPort) && (HI_UNF_AI_I2S1 != enAiPort) && (HI_UNF_AI_SIF0 != enAiPort)
        && (HI_UNF_AI_ADC0 != enAiPort) && (HI_UNF_AI_ADC1 != enAiPort) && (HI_UNF_AI_ADC2 != enAiPort)
        && (HI_UNF_AI_ADC3 != enAiPort) && (HI_UNF_AI_HDMI0 != enAiPort)
        && (HI_UNF_AI_HDMI1 != enAiPort) && (HI_UNF_AI_HDMI2 != enAiPort))
    {
        HI_ERR_AI("just support I2S0, I2S1 , SIF, HDMI, ADC0/ADC1/ADC2/ADC4 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3751v500)
    if ((HI_UNF_AI_I2S1 != enAiPort) && (HI_UNF_AI_SIF0 != enAiPort)
        && (HI_UNF_AI_ADC0 != enAiPort) && (HI_UNF_AI_ADC1 != enAiPort) && (HI_UNF_AI_ADC2 != enAiPort)
        && (HI_UNF_AI_ADC3 != enAiPort) && (HI_UNF_AI_HDMI0 != enAiPort)
        && (HI_UNF_AI_HDMI1 != enAiPort) && (HI_UNF_AI_HDMI2 != enAiPort))
    {
        HI_ERR_AI("just support I2S0, I2S1 , SIF, HDMI, ADC0/ADC1/ADC2/ADC4 Port!\n");
        return HI_FALSE;
    }
#elif defined(CHIP_TYPE_hi3716mv410)
#error "CHIP does not support AI!"
#else
#error "YOU MUST DEFINE CHIP_TYPE!"
#endif

    return HI_TRUE;
}

static HI_BOOL AICheckPortUsed(HI_UNF_AI_E enAiPort)
{
    HI_U32 i;

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_stAIDrv.apAiState[i])
        {
            continue;
        }

        if (g_stAIDrv.apAiState[i]->enAiPort == enAiPort)
        {
            HI_ERR_AI("This port has been occupied!\n");
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

static HI_VOID AIInitChnState(AI_CHN_STATE_S* state)
{
    HI_UNF_AI_ATTR_S* pstAiAttr = &state->stSndPortAttr;
    switch (state->enAiPort)
    {
        case HI_UNF_AI_I2S0:
        case HI_UNF_AI_I2S1:
            state->u32Channels = pstAiAttr->unAttr.stI2sAttr.stAttr.enChannel;
            state->u32BitPerSample = pstAiAttr->unAttr.stI2sAttr.stAttr.enBitDepth;
            return;

        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            state->u32Channels = pstAiAttr->unAttr.stHDMIAttr.enChannel;
            state->u32BitPerSample = pstAiAttr->unAttr.stHDMIAttr.enBitDepth;
            if ((HI_UNF_AI_HDMI_FORMAT_LBR == pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataFormat)
                || (HI_UNF_AI_HDMI_FORMAT_HBR == pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataFormat))
            {
                pstAiAttr->u32PcmSamplesPerFrame = AI_SAMPLE_PERFRAME_DF * 20;
            }
            return;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
        case HI_UNF_AI_SIF0:
            state->u32Channels = HI_UNF_I2S_CHNUM_2;
            state->u32BitPerSample = HI_UNF_I2S_BIT_DEPTH_16;
            return;
        default:
            state->u32Channels = HI_UNF_I2S_CHNUM_2;
            state->u32BitPerSample = HI_UNF_I2S_BIT_DEPTH_16;
            return;
    }
}

static HI_S32 AIGetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr)
{
    if (HI_FALSE == AICheckPortValid(enAiPort))
    {
        return HI_FAILURE;
    }

    pstAiAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstAiAttr->u32PcmFrameMaxNum = AI_BUFF_FRAME_NUM_DF;
    pstAiAttr->u32PcmSamplesPerFrame = AI_SAMPLE_PERFRAME_DF;

    if (HI_UNF_AI_I2S0 == enAiPort ||
        HI_UNF_AI_I2S1 == enAiPort)
    {
        pstAiAttr->unAttr.stI2sAttr.stAttr.bMaster = HI_TRUE;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enI2sMode = HI_UNF_I2S_STD_MODE;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enMclkSel = HI_UNF_I2S_MCLK_256_FS;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enBclkSel = HI_UNF_I2S_BCLK_4_DIV;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_2;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
        pstAiAttr->unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = HI_TRUE;
        pstAiAttr->unAttr.stI2sAttr.stAttr.enPcmDelayCycle = HI_UNF_I2S_PCM_0_DELAY;
    }
    else if (HI_UNF_AI_ADC0 == enAiPort ||
             HI_UNF_AI_ADC1 == enAiPort ||
             HI_UNF_AI_ADC2 == enAiPort ||
             HI_UNF_AI_ADC3 == enAiPort ||
             HI_UNF_AI_ADC4 == enAiPort)
    {
        pstAiAttr->unAttr.stAdcAttr.bByPass = HI_FALSE;
    }
    else if (HI_UNF_AI_HDMI0 == enAiPort ||
             HI_UNF_AI_HDMI1 == enAiPort ||
             HI_UNF_AI_HDMI2 == enAiPort ||
             HI_UNF_AI_HDMI3 == enAiPort)
    {
        pstAiAttr->unAttr.stHDMIAttr.enSampleRate = HI_UNF_SAMPLE_RATE_48K;
        pstAiAttr->unAttr.stHDMIAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
        pstAiAttr->unAttr.stHDMIAttr.enChannel = HI_UNF_I2S_CHNUM_2;
        pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataFormat = HI_UNF_AI_HDMI_FORMAT_LPCM;
    }

    return HI_SUCCESS;
}

#ifdef HI_ALSA_AI_SUPPORT
HI_S32 AI_GetIsrFunc(AIAO_IsrFunc** pFunc)
{
    AIAO_PORT_USER_CFG_S stAttr;

    HAL_AIAO_P_GetTxI2SDfAttr(AIAO_PORT_TX0, &stAttr);

    *pFunc = stAttr.pIsrFunc;

    return HI_SUCCESS;
}

HI_S32 AI_GetAiaoPortId(HI_HANDLE hAI, AIAO_PORT_ID_E* enPort)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    *enPort = state->enPort;

    return HI_SUCCESS;
}

static HI_S32 AIAlsaSetParam(HI_HANDLE hAI, HI_VOID* pAlsaPara)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    state->bAlsa = HI_TRUE;
    state->pAlsaPara = pAlsaPara;

    return HI_SUCCESS;
}
#endif

static HI_S32 AICheckI2sAttr(HI_UNF_AI_I2S_ATTR_S* pstI2sAttr)
{
    CHECK_AI_MCLKDIV(pstI2sAttr->stAttr.enMclkSel);
    CHECK_AI_BCLKDIV(pstI2sAttr->stAttr.enBclkSel);
    CHECK_AI_CHN(pstI2sAttr->stAttr.enChannel);
    CHECK_AI_BITDEPTH(pstI2sAttr->stAttr.enBitDepth);
    CHECK_AI_PCMDELAY(pstI2sAttr->stAttr.enPcmDelayCycle);

    if (HI_UNF_I2S_MODE_BUTT <= pstI2sAttr->stAttr.enI2sMode)
    {
        HI_ERR_AI("dont support I2sMode(%d)\n", pstI2sAttr->stAttr.enI2sMode);
        return HI_ERR_AI_INVALID_PARA;
    }

    if (HI_UNF_I2S_MCLK_BUTT <= pstI2sAttr->stAttr.enMclkSel)
    {
        HI_ERR_AI("dont support I2S MclkSel(%d)\n", pstI2sAttr->stAttr.enBclkSel);
        return HI_ERR_AI_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_S32 AICheckHdmiAttr(HI_UNF_AI_HDMI_ATTR_S* pstHdmiAttr)
{
#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)

    if ((g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_I2S1_MSK)) ||
        (g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_ADAC_MSK)))
    {
        HI_ERR_AO("SIF, ADC and HDMIRX can not coexist!\n");
        return HI_FAILURE;
    }
#endif

    CHECK_AI_CHN(pstHdmiAttr->enChannel);
    CHECK_AI_BITDEPTH(pstHdmiAttr->enBitDepth);
    CHECK_AI_HdmiDataFormat(pstHdmiAttr->enHdmiAudioDataFormat);

    return HI_SUCCESS;
}

static HI_S32 AICheckAdcAttr(HI_UNF_AI_ATTR_S* pstAiAttr)
{
#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)

    if ((g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_I2S1_MSK)) ||
        (g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_HDMI_MSK)))
    {
        HI_ERR_AO("SIF, ADC and HDMIRX can not coexist!\n");
        return HI_FAILURE;
    }
#endif

    if (HI_UNF_SAMPLE_RATE_48K != pstAiAttr->enSampleRate)
    {
        HI_ERR_AI("ADC port only support 48k samplerate!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AICheckSifAttr(HI_UNF_AI_E enAiPort)
{
#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)

    if ((g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_ADAC_MSK)) ||
        (g_stAIDrv.u32AiUseFlag & ((HI_U32)1 << AI_HDMI_MSK)))
    {
        HI_ERR_AO("SIF, ADC and HDMIRX can not coexist!\n");
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

static HI_S32 AICheckAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CHECK_AI_SAMPLERATE(pstAiAttr->enSampleRate);

    if ((pstAiAttr->u32PcmFrameMaxNum <= 0) ||
        (pstAiAttr->u32PcmSamplesPerFrame <= 0))
    {
        HI_ERR_AI("PcmFrameMaxNum(%d) is invalid!\n", pstAiAttr->u32PcmFrameMaxNum);
        return HI_ERR_AI_INVALID_PARA;
    }

    switch (enAiPort)
    {
        case HI_UNF_AI_I2S0:
        case HI_UNF_AI_I2S1:
            s32Ret = AICheckI2sAttr(&pstAiAttr->unAttr.stI2sAttr);
            break;

        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            s32Ret = AICheckHdmiAttr(&pstAiAttr->unAttr.stHDMIAttr);
            break;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
            s32Ret = AICheckAdcAttr(pstAiAttr);
            break;

        case HI_UNF_AI_SIF0:
            s32Ret = AICheckSifAttr(enAiPort);
            break;

        default:
            break;
    }

    return s32Ret;
}

static HI_VOID AISetPortIfAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr, AIAO_IfAttr_S* pstIfAttr)
{
    pstIfAttr->enRate = (AIAO_SAMPLE_RATE_E)(pstAiAttr->enSampleRate);

    if (HI_UNF_AI_I2S0 == enAiPort ||
        HI_UNF_AI_I2S1 == enAiPort)
    {
        HI_UNF_I2S_ATTR_S* pstI2SAttr = &pstAiAttr->unAttr.stI2sAttr.stAttr;
        if (HI_TRUE == pstI2SAttr->bMaster)
        {
            pstIfAttr->enCrgMode = AIAO_CRG_MODE_DUPLICATE;
            if (HI_UNF_AI_I2S0 == enAiPort)
            {
                pstIfAttr->eCrgSource = AIAO_TX_CRG0;
            }

            if (HI_UNF_AI_I2S1 == enAiPort)
            {
#ifdef HI_FPGA
                pstIfAttr->eCrgSource = AIAO_TX_CRG2;
#else
                pstIfAttr->eCrgSource = AIAO_TX_CRG1;
#endif
            }

            pstIfAttr->u32BCLK_DIV = pstI2SAttr->enBclkSel;
            pstIfAttr->u32FCLK_DIV = AUTIL_BclkFclkDiv(pstI2SAttr->enMclkSel, pstI2SAttr->enBclkSel);
        }
        else
        {
            pstIfAttr->enCrgMode = AIAO_CRG_MODE_SLAVE;
        }
        pstIfAttr->enI2SMode = AUTIL_I2S_MODE_UNF2AIAO(pstI2SAttr->enI2sMode);
        pstIfAttr->enChNum = AUTIL_CHNUM_UNF2AIAO(pstI2SAttr->enChannel);
        pstIfAttr->enBitDepth = AUTIL_BITDEPTH_UNF2AIAO(pstI2SAttr->enBitDepth);
        pstIfAttr->u32PcmDelayCycles = pstI2SAttr->enPcmDelayCycle;
        if (HI_TRUE == pstI2SAttr->bPcmSampleRiseEdge)
        {
            pstIfAttr->enRiseEdge = AIAO_MODE_EDGE_RISE;
        }
        else
        {
            pstIfAttr->enRiseEdge = AIAO_MODE_EDGE_FALL;
        }
    }
    else if (HI_UNF_AI_HDMI0 == enAiPort ||
             HI_UNF_AI_HDMI1 == enAiPort ||
             HI_UNF_AI_HDMI2 == enAiPort ||
             HI_UNF_AI_HDMI3 == enAiPort)
    {
        pstIfAttr->enChNum = AUTIL_CHNUM_UNF2AIAO(pstAiAttr->unAttr.stHDMIAttr.enChannel);
        pstIfAttr->enBitDepth = AUTIL_BITDEPTH_UNF2AIAO(pstAiAttr->unAttr.stHDMIAttr.enBitDepth);
    }
    else if (HI_UNF_AI_ADC0 == enAiPort ||
             HI_UNF_AI_ADC1 == enAiPort ||
             HI_UNF_AI_ADC2 == enAiPort ||
             HI_UNF_AI_ADC3 == enAiPort ||
             HI_UNF_AI_ADC4 == enAiPort)
    {

    }
}

static void Port2TianlaiSel(HI_UNF_AI_E enAiPort, HI_BOOL bByPass)
{
    S5_TIANLAI_LINEIN_SEL_E enSel = S5_TIANLAI_LINEIN_SEL_BUTT;
    switch (enAiPort)
    {
        case HI_UNF_AI_ADC0:
            enSel = S5_TIANLAI_LINEIN_SEL_MIC;
            break;
        case HI_UNF_AI_ADC1:
            enSel = S5_TIANLAI_LINEIN_SEL_L1;
            break;
        case HI_UNF_AI_ADC2:
            enSel = S5_TIANLAI_LINEIN_SEL_L2;
            break;
        case HI_UNF_AI_ADC3:
            enSel = S5_TIANLAI_LINEIN_SEL_L3;
            break;
        case HI_UNF_AI_ADC4:
            enSel = S5_TIANLAI_LINEIN_SEL_L4;
            break;
        default:
            break;
    }

#ifdef HI_TIANLAI_V510
    HAL_TIANLAI_V510_SetLineInSuorce(enSel, bByPass, HI_TRUE);
#endif
}

static HI_VOID AIFreeBuf(AI_CHN_STATE_S* state)
{
    HI_DRV_MMZ_UnmapAndRelease(&state->stRbfMmz);
    HI_DRV_MMZ_UnmapAndRelease(&state->stFrameMmz);
}

static HI_S32 AIAllocBuf(AI_CHN_STATE_S* state)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR szName[16];
    HI_U32 u32AiFrameSize = 0;
    HI_U32 u32BufSize = 0;
    HI_UNF_AI_ATTR_S* pstAiAttr = &state->stSndPortAttr;
    HI_HANDLE hAI = ((state->hAI) & AI_CHNID_MASK);

    u32AiFrameSize = pstAiAttr->u32PcmSamplesPerFrame * AUTIL_CalcFrameSize(state->u32Channels, state->u32BitPerSample);

    /* frame buffer for AI acquire frame */
    snprintf(szName, sizeof(szName), "AI_ChnBuf%d", hAI);
    s32Ret = HI_DRV_MMZ_AllocAndMap(szName, MMZ_OTHERS, u32AiFrameSize, AIAO_BUFFER_ADDR_ALIGN, &state->stFrameMmz);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AI("HI_MMZ AI_BUF failed, AllocSize(%d)\n", u32AiFrameSize);
        return s32Ret;
    }
    state->stAiBuf.tPhyBaseAddr = state->stFrameMmz.u32StartPhyAddr;
    state->stAiBuf.u32Size = state->stFrameMmz.u32Size;
    state->stAiBuf.tKernelVirAddr = (HI_VIRT_ADDR_T)(state->stFrameMmz.pu8StartVirAddr - (HI_U8*)HI_NULL);
    state->stAiBuf.tUserVirAddr = 0;
    state->stAiBuf.u32Read = 0;
    state->stAiBuf.u32Write = 0;

    /* AI port mmz */
    u32BufSize = u32AiFrameSize * pstAiAttr->u32PcmFrameMaxNum;
    snprintf(szName, sizeof(szName), "AI_I2sBuf%d", hAI);
    s32Ret = HI_DRV_MMZ_AllocAndMap(szName, MMZ_OTHERS, u32BufSize, AIAO_BUFFER_ADDR_ALIGN, &state->stRbfMmz);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AI("HI_MMZ AI_PORT_BUF failed, AllocSize(%d)\n", u32BufSize);

        HI_DRV_MMZ_UnmapAndRelease(&state->stFrameMmz);
    }

    return s32Ret;
}

static HI_VOID AICreateSetFlag(AI_CHN_STATE_S* state)
{
    switch (state->enAiPort)
    {
        case HI_UNF_AI_I2S0:
            g_stAIDrv.u32AiUseFlag |= (HI_U32)1 << AI_I2S0_MSK;
            break;

        case HI_UNF_AI_I2S1:
            g_stAIDrv.u32AiUseFlag |= (HI_U32)1 << AI_I2S1_MSK;
            break;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
            g_stAIDrv.u32AiUseFlag |= (HI_U32)1 << AI_ADAC_MSK;
            break;

        case HI_UNF_AI_SIF0:
            g_stAIDrv.u32AiUseFlag |= (HI_U32)1 << AI_I2S1_MSK;
            break;

        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            g_stAIDrv.u32AiUseFlag |= (HI_U32)1 << AI_HDMI_MSK;
            break;

        default:
            HI_ERR_AI("Aiport is invalid!\n");
            break;
    }
}

static HI_VOID AIDestorySetFlag(AI_CHN_STATE_S* state)
{
    switch (state->enAiPort)
    {
        case HI_UNF_AI_I2S0:
            g_stAIDrv.u32AiUseFlag &= (HI_U32)(~(1 << AI_I2S0_MSK));
            break;

        case HI_UNF_AI_I2S1:
            g_stAIDrv.u32AiUseFlag &= (HI_U32)(~(1 << AI_I2S1_MSK));
            break;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
            g_stAIDrv.u32AiUseFlag &= (HI_U32)(~(1 << AI_ADAC_MSK));
#ifdef HI_TIANLAI_V510
            HAL_TIANLAI_V510_SetLineInSuorce(S5_TIANLAI_LINEIN_SEL_MIC, HI_FALSE, HI_FALSE);
#endif
            break;

        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            g_stAIDrv.u32AiUseFlag &= (HI_U32)(~(1 << AI_HDMI_MSK));
            break;

        case HI_UNF_AI_SIF0:
            g_stAIDrv.u32AiUseFlag &= (HI_U32)(~(1 << AI_I2S1_MSK));
            break;

        default:
            HI_ERR_AI("Aiport is invalid!\n");
            break;
    }
}

static HI_S32 AIAllocHandle(HI_HANDLE* phHandle,
                            struct file* file,
                            HI_UNF_AI_E enAiPort,
                            HI_UNF_AI_ATTR_S* pstAiAttr)
{
    HI_U32 i;
    HI_HANDLE hAI;
    HI_S32 s32Ret;
    AI_CHN_STATE_S* state;

    if (HI_FALSE == AICheckPortValid(enAiPort))
    {
        return HI_ERR_AI_INVALID_PARA;
    }

    if (HI_FALSE == AICheckPortUsed(enAiPort))
    {
        return HI_ERR_AI_INVALID_PARA;
    }

    s32Ret = AICheckAttr(enAiPort, pstAiAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AICheckAttr failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_stAIDrv.apAiState[i])
        {
            break;
        }
    }

    if (i >= AI_MAX_TOTAL_NUM)
    {
        HI_ERR_AI("Too many Ai channel!\n");
        return HI_ERR_AI_NOTSUPPORT;
    }

    state = (AI_CHN_STATE_S*)HI_KMALLOC(HI_ID_AI, sizeof(AI_CHN_STATE_S), GFP_KERNEL);
    if (HI_NULL == state)
    {
        HI_ERR_AI("call HI_KMALLOC failed!\n");
        return HI_ERR_AI_NULL_PTR;
    }

    memset(state, 0, sizeof(AI_CHN_STATE_S));

    state->enAiPort = enAiPort;
    memcpy(&state->stSndPortAttr, pstAiAttr, sizeof(HI_UNF_AI_ATTR_S));

    AIInitChnState(state);

    state->pstFile = file;
    g_stAIDrv.apAiState[i] = state;

    hAI = ((HI_ID_AI << 16) | i);

    state->hAI = hAI;

    s32Ret = AIAllocBuf(state);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIAllocBuf failed(0x%x)!\n", s32Ret);
        HI_KFREE(HI_ID_AI, state);
        return s32Ret;
    }

    *phHandle = hAI;

    return HI_SUCCESS;
}

static HI_VOID AIFreeHandle(HI_HANDLE hAI)
{
    AI_CHN_STATE_S* state;

    hAI &= AI_CHNID_MASK;
    if (AI_MAX_TOTAL_NUM <= hAI)
    {
        return;
    }

    state = g_stAIDrv.apAiState[hAI];
    if (HI_NULL != state)
    {
        AIFreeBuf(state);
        HI_KFREE(HI_ID_AI, state);
    }

    g_stAIDrv.apAiState[hAI] = HI_NULL;
}

static HI_S32 AICreateChn(AI_CHN_STATE_S* state)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BoardI2sNum = 0;
    AIAO_PORT_USER_CFG_S stHwPortAttr;
    HI_UNF_AI_ATTR_S* pstAttr = &state->stSndPortAttr;

    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;

    switch (state->enAiPort)
    {
        case HI_UNF_AI_I2S0:
            u32BoardI2sNum = 0;
            HAL_AIAO_P_GetBoardRxI2SDfAttr(u32BoardI2sNum, &enPort, &stHwPortAttr);
            break;

        case HI_UNF_AI_I2S1:
            u32BoardI2sNum = 1;
            HAL_AIAO_P_GetBoardRxI2SDfAttr(u32BoardI2sNum, &enPort, &stHwPortAttr);
            break;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
            Port2TianlaiSel(state->enAiPort, pstAttr->unAttr.stAdcAttr.bByPass);
#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)
            enPort = AIAO_PORT_RX3;
#elif    defined (CHIP_TYPE_hi3751v100) \
      || defined (CHIP_TYPE_hi3751v100b)
            enPort = AIAO_PORT_RX2;
#endif
            HAL_AIAO_P_GetRxAdcDfAttr(enPort, &stHwPortAttr);
            break;
        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
#if    defined (CHIP_TYPE_hi3751v100)
            || defined (CHIP_TYPE_hi3751v100b)
        case HI_UNF_AI_HDMI3:
#endif
            enPort = AIAO_PORT_RX3;
            HAL_AIAO_P_GetRxHdmiDfAttr(enPort, &stHwPortAttr);

#if    defined (CHIP_TYPE_hi3751v600)  \
    || defined (CHIP_TYPE_hi3751lv500) \
    || defined(CHIP_TYPE_hi3751v500)   \
    || defined(CHIP_TYPE_hi3751v620)
            if (HI_UNF_AI_HDMI2 == state->enAiPort)
            {
                stHwPortAttr.stIfAttr.enSource = AIAO_RX4;
                stHwPortAttr.stIfAttr.eCrgSource = AIAO_RX_CRG4;
            }
#endif
            break;
        case HI_UNF_AI_SIF0:
#if    defined (CHIP_TYPE_hi3751v600)   \
    || defined (CHIP_TYPE_hi3751lv500)  \
    || defined(CHIP_TYPE_hi3751v500)    \
    || defined(CHIP_TYPE_hi3751v620)
            enPort = AIAO_PORT_RX3;
#elif    defined (CHIP_TYPE_hi3751v100)  \
      || defined (CHIP_TYPE_hi3751v100b)
            enPort = AIAO_PORT_RX1;
#endif
            HAL_AIAO_P_GetRxSifDfAttr(enPort, &stHwPortAttr);
            break;
        default:
            HI_ERR_AI("AiPort is invalid!\n");
            return HI_ERR_AI_INVALID_PARA;
    }

    stHwPortAttr.stBufConfig.u32PeriodNumber = state->stRbfMmz.u32Size / stHwPortAttr.stBufConfig.u32PeriodBufSize;

    if (HI_TRUE == state->bAlsa)
    {
        AI_ALSA_Param_S* pstAlsaAttr = (AI_ALSA_Param_S*)state->pAlsaPara;
        if (HI_NULL == pstAlsaAttr)
        {
            HI_ERR_AI("pAlsaPara is null!\n");
            return HI_ERR_AI_INVALID_PARA;
        }

        stHwPortAttr.bExtDmaMem = HI_TRUE;
        stHwPortAttr.stExtMem.tBufPhyAddr = pstAlsaAttr->stBuf.tBufPhyAddr;
        stHwPortAttr.stExtMem.tBufVirAddr = pstAlsaAttr->stBuf.tBufVirAddr;
        stHwPortAttr.stExtMem.u32BufSize  = pstAlsaAttr->stBuf.u32BufSize;
        stHwPortAttr.pIsrFunc             = pstAlsaAttr->IsrFunc;
        stHwPortAttr.pSubstream           = pstAlsaAttr->pSubstream;
        stHwPortAttr.stBufConfig.u32PeriodBufSize = pstAlsaAttr->stBuf.u32PeriodByteSize;
        stHwPortAttr.stBufConfig.u32PeriodNumber  = pstAlsaAttr->stBuf.u32Periods;

        state->stRbfMmz.u32Size         = pstAlsaAttr->stBuf.u32BufSize;
        state->stRbfMmz.u32StartPhyAddr = pstAlsaAttr->stBuf.tBufPhyAddr;
        state->stRbfMmz.pu8StartVirAddr = (HI_U8*)HI_NULL + pstAlsaAttr->stBuf.tBufVirAddr;
    }
    else
    {
        stHwPortAttr.bExtDmaMem = HI_TRUE;
        stHwPortAttr.stExtMem.tBufPhyAddr = state->stRbfMmz.u32StartPhyAddr;
        stHwPortAttr.stExtMem.tBufVirAddr = (state->stRbfMmz.pu8StartVirAddr - (HI_U8*)HI_NULL);
        stHwPortAttr.stExtMem.u32BufSize = state->stRbfMmz.u32Size;
    }

    AISetPortIfAttr(state->enAiPort, pstAttr, &stHwPortAttr.stIfAttr);
    s32Ret = HAL_AIAO_P_Open(enPort, &stHwPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("HAL_AIAO_P_Open failed\n");
        return HI_FAILURE;
    }

#ifdef __DPT__
    if (HI_UNF_AI_ADC0 == state->enAiPort || HI_UNF_AI_ADC1 == state->enAiPort || HI_UNF_AI_ADC2 == state->enAiPort
        || HI_UNF_AI_ADC3 == state->enAiPort || HI_UNF_AI_ADC4 == state->enAiPort || HI_UNF_AI_SIF0 == state->enAiPort)
    {
        state->stNR.enNRType = HI_UNF_AI_NR_NORMAL;   //when input is adc, set NR mode normal
    }
    else
    {
        state->stNR.enNRType = HI_UNF_AI_NR_OFF;
    }

    state->stNR.unNRAttr.stMuteAttr.s32Threshold = -80;
#endif
    state->enCurnStatus = AI_CHN_STATUS_STOP;
    state->enPort = enPort;

    return HI_SUCCESS;
}

static HI_VOID AIDestoryChn(AI_CHN_STATE_S* state)
{
    HAL_AIAO_P_Close(state->enPort);
}

static HI_S32 AICreate(HI_HANDLE hAI)
{
    HI_S32 s32Ret = HI_SUCCESS;

    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    s32Ret = AICreateChn(state);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AICreateChn failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    state->bAttach = HI_FALSE;

#if (1 == HI_PROC_SUPPORT)
    state->enSaveState = AI_CMD_CTRL_STOP;
    state->bSaveThreadRun = HI_FALSE;
#endif

#if defined (HI_ALSA_AI_SUPPORT)
    if (HI_TRUE == state->bAlsa)
    {
        memcpy(&g_stAlsaAttr, (AI_ALSA_Param_S*)(state->pAlsaPara), sizeof(AI_ALSA_Param_S));
    }
#endif

    AICreateSetFlag(state);

#if (1 == HI_PROC_SUPPORT)
    s32Ret = AI_RegProc(hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AI_RegProc failed(0x%x)!\n", s32Ret);
        AIDestoryChn(state);
        AIDestorySetFlag(state);
    }
#endif

    return s32Ret;
}

static HI_S32 AIDestory(HI_HANDLE hAI)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

#if (1 == HI_PROC_SUPPORT)
    if (state->pSaveBuf)
    {
        HI_KFREE(HI_ID_AI, state->pSaveBuf);
        state->pSaveBuf = HI_NULL;
    }

    AI_UnRegProc(hAI);
#endif

    AIDestoryChn(state);
    AIDestorySetFlag(state);

    return HI_SUCCESS;
}

static HI_S32 AISetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pstAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    AIAO_PORT_ATTR_S stPortAttr;

    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    s32Ret = AICheckAttr(state->enAiPort, pstAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("Invalid ai attr!\n");
        return s32Ret;
    }

    if (AI_CHN_STATUS_STOP != state->enCurnStatus)
    {
        HI_ERR_AI("current state is not stop, can not set attr!\n");
        return HI_FAILURE;
    }

    s32Ret = HAL_AIAO_P_GetAttr(state->enPort, &stPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("HAL_AIAO_P_GetAttr failed(0x%x)!", s32Ret);
        return s32Ret;
    }

    AISetPortIfAttr(state->enAiPort, pstAttr, &stPortAttr.stIfAttr);

    s32Ret = HAL_AIAO_P_SetAttr(state->enPort, &stPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("HAL_AIAO_P_SetAttr failed(0x%x)!", s32Ret);
        return s32Ret;
    }
    memcpy(&state->stSndPortAttr, pstAttr, sizeof(HI_UNF_AI_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 AIGetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pAiAttr)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    memcpy(pAiAttr, &state->stSndPortAttr, sizeof(HI_UNF_AI_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 AI_GetPortAttr(HI_HANDLE hAI, AIAO_PORT_ATTR_S* pstPortAttr)
{
    HI_S32 s32Ret;
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    s32Ret = HAL_AIAO_P_GetAttr(state->enPort, pstPortAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("HAL_AIAO_P_GetAttr failed(0x%x)!", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AICheckDelayComps(AI_CHN_STATE_S* state, HI_U32 u32CompensationMs)
{
    HI_U32 u32BufDelayMs = 0;

    if (state->stSndPortAttr.enSampleRate)
    {
        u32BufDelayMs = state->stSndPortAttr.u32PcmSamplesPerFrame * state->stSndPortAttr.u32PcmFrameMaxNum
                        * 1000 / state->stSndPortAttr.enSampleRate;
    }

    if (u32CompensationMs > u32BufDelayMs)
    {
        HI_ERR_AI("u32CompensationMs(%d) exceed u32BufDelayMs(%d)!\n", u32CompensationMs, u32BufDelayMs);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AISetDelayComps(HI_HANDLE hAI, HI_UNF_AI_DELAY_S* pstDelayComps)
{
    HI_S32 s32Ret;
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if (AI_CHN_STATUS_STOP != state->enCurnStatus)
    {
        HI_ERR_AI("current state is not stop,can not set delay compensation!\n");
        return HI_FAILURE;
    }

    s32Ret = AICheckDelayComps(state, pstDelayComps->u32DelayMs);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    memcpy(&state->stDelayComps, pstDelayComps, sizeof(HI_UNF_AI_DELAY_S));

    return s32Ret;
}

static HI_S32 AIGetDelayComps(HI_HANDLE hAI, HI_UNF_AI_DELAY_S* pstDelayComps)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    memcpy(pstDelayComps, &state->stDelayComps, sizeof(HI_UNF_AI_DELAY_S));

    return HI_SUCCESS;
}

static HI_U32 AICalcAcquireSize(HI_HANDLE hAI)
{
    HI_U32 u32NeedBytes;
    HI_U32 u32FrameSize;
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    if (HI_NULL == state)
    {
        return 0;
    }

    u32FrameSize = AUTIL_CalcFrameSize(state->u32Channels, state->u32BitPerSample);
    u32NeedBytes = u32FrameSize * state->stSndPortAttr.u32PcmSamplesPerFrame;

    return u32NeedBytes;
}

#define HI_ERR_AI_NOT_ENOUGH_DATA           (HI_S32)(0x801B0009)

static HI_S32 AIAcquireFrame(HI_HANDLE hAI, HI_U32 u32NeedBytes)
{
    HI_U32 u32ReadBytes  = 0;
    HI_U32 u32FrameSize  = 0;
    HI_U32 u32AvailBytes = 0;
    HI_U32 u32WaitMs     = 0;
    HI_U32 u32TimeoutMs  = 1; //default wait time
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    state->stAiProcAttr.pstAiProcInfo->u32AqcTryCnt++;

    if (AI_CHN_STATUS_STOP == state->enCurnStatus)
    {
        HI_ERR_AI("current state is stop, can not get frame!\n");
        return HI_FAILURE;
    }

    if (state->stAiBuf.u32Size < u32NeedBytes)
    {
        HI_ERR_AI("u32NeedBytes(%d) is too big than u32Size(%d)!\n", u32NeedBytes, state->stAiBuf.u32Size);
        return HI_FAILURE;
    }

    u32FrameSize = AUTIL_CalcFrameSize(state->u32Channels, state->u32BitPerSample);
    if (state->stSndPortAttr.enSampleRate && u32FrameSize)
    {
        u32TimeoutMs = (u32NeedBytes * 1000) / (state->stSndPortAttr.enSampleRate * u32FrameSize);
    }

    while (1)
    {
        u32AvailBytes = HAL_AIAO_P_QueryBufData(state->enPort);
        if (u32AvailBytes >= u32NeedBytes)
        {
            break;
        }

        if (u32WaitMs++ > u32TimeoutMs)
        {
            HI_ERR_AI("Query BufData time out! u32NeedBytes(%d), u32AvailBytes(%d)\n", u32NeedBytes, u32AvailBytes);
            return HI_ERR_AI_NOT_ENOUGH_DATA;
        }

        msleep(1);
    }

    u32ReadBytes = HAL_AIAO_P_ReadData(state->enPort, (HI_U8*)HI_NULL + state->stAiBuf.tKernelVirAddr, u32NeedBytes);
    if (u32ReadBytes != u32NeedBytes)
    {
        HI_ERR_AI("HAL_AIAO_P_ReadData failed! u32ReadBytes(%d)!=u32NeedBytes(%d)\n", u32ReadBytes, u32NeedBytes);
        return HI_ERR_AI_NOT_ENOUGH_DATA;
    }

    state->stAiProcAttr.pstAiProcInfo->u32AqcCnt++;

    return HI_SUCCESS;
}

static HI_S32 AIReleaseFrame(HI_HANDLE hAI)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    state->stAiProcAttr.pstAiProcInfo->u32RelTryCnt++;
    state->stAiProcAttr.pstAiProcInfo->u32RelCnt++;
    return HI_SUCCESS;
}

#ifdef __DPT__
static HI_S32 AIGetStreamType(HI_HANDLE hAI, HI_UNF_AI_HDMI_DATA_TYPE_E* penStreamType)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    return HI_SUCCESS;
}

static HI_S32 AISetNRAttr(HI_HANDLE hAI, HI_UNF_AI_NR_ATTR_S* pstNRParam)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if (pstNRParam->unNRAttr.stMuteAttr.s32Threshold < -100 ||
        pstNRParam->unNRAttr.stMuteAttr.s32Threshold > -60)
    {
        HI_ERR_AI("AI chn NR level should between -60dB ~ -100dB!\n");
        return HI_ERR_AI_INVALID_PARA;
    }

    memcpy(&state->stNR, pstNRParam, sizeof(HI_UNF_AI_NR_ATTR_S));

    return AO_Track_SetNRAttr(state->hTrack, pstNRParam);
}

HI_S32 AI_GetNRAttr(HI_HANDLE hAI, HI_UNF_AI_NR_ATTR_S* pstNRParam)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    memcpy(pstNRParam, &state->stNR, sizeof(HI_UNF_AI_NR_ATTR_S));
    return HI_SUCCESS;
}
#endif

static HI_S32 AIGetAiBufInfo(HI_HANDLE hAI, AI_BUF_ATTR_S* pstAiBuf)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    memcpy(pstAiBuf, &state->stAiBuf, sizeof(AI_BUF_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 AISetAiBufInfo(HI_HANDLE hAI, AI_BUF_ATTR_S* pstAiBuf)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    state->stAiBuf.tUserVirAddr = pstAiBuf->tUserVirAddr;

    return HI_SUCCESS;
}

static HI_S32 AIProcDeInit(HI_HANDLE hAI)
{
    AI_PROC_ATTR_S* pstAiProcAttr;
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    pstAiProcAttr = &state->stAiProcAttr;
    if (HI_FALSE == pstAiProcAttr->bUsed)
    {
        return HI_SUCCESS;
    }

    HI_DRV_MMZ_UnmapAndRelease(&pstAiProcAttr->AIProcMmz);

    memset(pstAiProcAttr, 0, sizeof(AI_PROC_ATTR_S));

    return HI_SUCCESS;
}

static HI_S32 AIProcInit(HI_HANDLE hAI, HI_PHYS_ADDR_T* pPhyAddr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    AI_PROC_ATTR_S* pstAiProcAttr;
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    pstAiProcAttr = &state->stAiProcAttr;
    if (HI_TRUE == pstAiProcAttr->bUsed)
    {
        HI_ERR_AI("proc is already registered!");
        return HI_SUCCESS;
    }

    hAI &= AI_CHNID_MASK;

    snprintf(pstAiProcAttr->szProcMmzName, sizeof(pstAiProcAttr->szProcMmzName), "%s%02d", "AI_Proc", hAI);

    s32Ret = HI_DRV_MMZ_AllocAndMap(pstAiProcAttr->szProcMmzName, MMZ_OTHERS, sizeof(AI_PROC_INFO_S), 0, &pstAiProcAttr->AIProcMmz);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstAiProcAttr->pstAiProcInfo = (AI_PROC_INFO_S*)pstAiProcAttr->AIProcMmz.pu8StartVirAddr;
    memset(pstAiProcAttr->pstAiProcInfo, 0, sizeof(AI_PROC_INFO_S));

    *pPhyAddr = pstAiProcAttr->AIProcMmz.u32StartPhyAddr;

    pstAiProcAttr->bUsed = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable, HI_BOOL bTrackResume)
{
    HI_S32 Ret;
    HI_U32 u32AiDelayMs = 0;
    HI_U32 u32AoDelayMs = 0;
    HI_U32 u32BytesSize = 0;
    HI_U32 u32FrameSize = 0;
    AIAO_PORT_ATTR_S stPortAttr;

    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    memset(&stPortAttr, 0, sizeof(AIAO_PORT_ATTR_S));

    if (bEnable)
    {
        Ret = HAL_AIAO_P_GetAttr(state->enPort, &stPortAttr);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AI("HAL_AIAO_P_GetAttr(%d) failed\n", state->enPort);
            return Ret;
        }
        u32FrameSize = AUTIL_CalcFrameSize(stPortAttr.stIfAttr.enChNum, stPortAttr.stIfAttr.enBitDepth);

        if (bTrackResume)
        {
            u32BytesSize = AUTIL_LatencyMs2ByteSize(20,  u32FrameSize, stPortAttr.stIfAttr.enRate);
            HAL_AIAO_P_WriteData(state->enPort, HI_NULL, u32BytesSize);
        }
        HAL_AIAO_P_GetDelayMs(state->enPort, &u32AiDelayMs);
        if (HI_TRUE == state->bAttach)
        {
            Ret = AO_Track_GetDelayMs(state->hTrack, &u32AoDelayMs);
            if (HI_SUCCESS != Ret)
            {
                HI_ERR_AI("AO_Track_GetDelayMs(%d) failed\n", state->hTrack);
                return Ret;
            }
        }

        if (u32AiDelayMs + u32AoDelayMs < state->stDelayComps.u32DelayMs)
        {
            u32BytesSize = AUTIL_LatencyMs2ByteSize(state->stDelayComps.u32DelayMs - u32AiDelayMs - u32AoDelayMs,
                                                    u32FrameSize, stPortAttr.stIfAttr.enRate);
            HAL_AIAO_P_WriteData(state->enPort, HI_NULL, u32BytesSize);
        }

        Ret = HAL_AIAO_P_Start(state->enPort);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AI("HAL_AIAO_P_Start(%d) failed\n", state->enPort);
        }
        else
        {
            state->enCurnStatus  = AI_CHN_STATUS_START;
        }
    }
    else
    {
#if (1 == HI_PROC_SUPPORT)
        if (HI_TRUE == state->bSaveThreadRun)
        {
            state->bSaveThreadRun = HI_FALSE;
            msleep(2);   //wait util save thread exit
        }
#endif

        Ret = HAL_AIAO_P_Stop(state->enPort, AIAO_STOP_IMMEDIATE);
        if (HI_SUCCESS != Ret)
        {
            HI_ERR_AI("HAL_AIAO_P_Stop(%d) failed\n", state->enPort);
        }
        else
        {
            state->enCurnStatus  = AI_CHN_STATUS_STOP;
        }
    }

    return Ret;
}

HI_S32 AI_GetEnable(HI_HANDLE hAI, HI_BOOL* pbEnable)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if (AI_CHN_STATUS_START == state->enCurnStatus)
    {
        *pbEnable = HI_TRUE;
    }
    else if (AI_CHN_STATUS_STOP == state->enCurnStatus)
    {
        *pbEnable = HI_FALSE;
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 AI_GetDelayMs(HI_HANDLE hAI, HI_U32* pu32DelayMs)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    *pu32DelayMs = state->stDelayComps.u32DelayMs;

    return HI_SUCCESS;
}

HI_S32 AI_GetPortBuf(HI_HANDLE hAI, AIAO_RBUF_ATTR_S* pstAiaoBuf)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    return HAL_AIAO_P_GetRbfAttr(state->enPort, pstAiaoBuf);
}

HI_S32 AI_AttachTrack(HI_HANDLE hAI, HI_HANDLE hTrack)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if ((HI_TRUE == state->bAttach) && (state->hTrack != hTrack))
    {
        HI_ERR_AI("AI is attached, can not be attached again!\n");
        return HI_FAILURE;
    }

    state->bAttach = HI_TRUE;
    state->hTrack = hTrack;

    return HI_SUCCESS;
}

HI_S32 AI_DetachTrack(HI_HANDLE hAI, HI_HANDLE hTrack)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if ((HI_TRUE != state->bAttach) || (state->hTrack != hTrack))
    {
        HI_ERR_AI("track(0x%x) is not attach this AI channel, can not detach!\n", hTrack);
        return HI_FAILURE;
    }

    state->bAttach = HI_FALSE;
    state->hTrack = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}

HI_BOOL AI_CheckIsHdmiPort(HI_HANDLE hAI)
{
    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    if (HI_NULL == state)
    {
        HI_ERR_AI("AI chn(%d) not open!\n", hAI);
        return HI_FALSE;
    }

    if ((HI_UNF_AI_HDMI0 == state->enAiPort) ||
        (HI_UNF_AI_HDMI1 == state->enAiPort) ||
        (HI_UNF_AI_HDMI2 == state->enAiPort) ||
        (HI_UNF_AI_HDMI3 == state->enAiPort))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_S32 AI_OpenDev(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        g_stAIDrv.apAiState[i] = HI_NULL;
    }

    HAL_AIAO_Init();

    return HI_SUCCESS;
}

static HI_S32 AI_CloseDev(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        g_stAIDrv.apAiState[i] = HI_NULL;
    }

    HAL_AIAO_DeInit();

    return HI_SUCCESS;
}

static HI_S32 AI_ProcessCmd(struct inode* inode, struct file* file, HI_U32 cmd, HI_VOID* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    switch (cmd)
    {
        case CMD_AI_GETDEFAULTATTR:
        {
            AI_GetDfAttr_Param_S_PTR pstParam = (AI_GetDfAttr_Param_S_PTR)arg;
            s32Ret = AIGetDefaultAttr(pstParam->enAiPort, &pstParam->stAttr);
            break;
        }

        case CMD_AI_CREATE:
        {
            HI_HANDLE hAI = HI_INVALID_HANDLE;
            AI_Create_Param_S_PTR pstParam = (AI_Create_Param_S_PTR)arg;

            s32Ret = AIAllocHandle(&hAI, file, pstParam->enAiPort, &pstParam->stAttr);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AI("AIAllocHandle failed(0x%x)!\n", s32Ret);
                break;
            }

            s32Ret = AICreate(hAI);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AI("AICreate failed(0x%x)!\n", s32Ret);
                AIFreeHandle(hAI);
                break;
            }
            pstParam->hAI = hAI;

            break;
        }
        case CMD_AI_DESTROY:
        {
            HI_HANDLE hAI = *(HI_HANDLE*)arg;
            CHECK_AI_ID(hAI);
            s32Ret = AIDestory(hAI);
            if (HI_SUCCESS != s32Ret)
            {
                break;
            }

            AIFreeHandle(hAI);
            break;
        }

        case CMD_AI_SETENABLE:
        {
            AI_Enable_Param_S_PTR pstParam = (AI_Enable_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AI_SetEnable(pstParam->hAI, pstParam->bAiEnable, HI_FALSE);
            break;
        }

        case CMD_AI_GETENABLE:
        {
            AI_Enable_Param_S_PTR pstParam = (AI_Enable_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AI_GetEnable(pstParam->hAI, &pstParam->bAiEnable);
            break;
        }

        case CMD_AI_ACQUIREFRAME:
        {
            AI_Frame_Param_S_PTR pstParam = (AI_Frame_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIAcquireFrame(pstParam->hAI, pstParam->u32NeedBytes);
            break;
        }

        case CMD_AI_RELEASEFRAME:
        {
            HI_HANDLE hAI = *(HI_HANDLE*)arg;
            CHECK_AI_ID(hAI);
            s32Ret = AIReleaseFrame(hAI);
            break;
        }

        case CMD_AI_SETATTR:
        {
            AI_Attr_Param_S_PTR pstParam = (AI_Attr_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AISetAttr(pstParam->hAI, &pstParam->stAttr);
            break;
        }

        case CMD_AI_GETATTR:
        {
            AI_Attr_Param_S_PTR pstParam = (AI_Attr_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIGetAttr(pstParam->hAI, &pstParam->stAttr);
            break;
        }

        case CMD_AI_GETBUFINFO:
        {
            AI_Buf_Param_S_PTR pstParam = (AI_Buf_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIGetAiBufInfo(pstParam->hAI, &pstParam->stAiBuf);
            break;
        }

        case CMD_AI_SETBUFINFO:
        {
            AI_Buf_Param_S_PTR pstParam = (AI_Buf_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AISetAiBufInfo(pstParam->hAI, &pstParam->stAiBuf);
            break;
        }

        case CMD_AI_SETDELAYCOMPS:
        {
            AI_DelayComps_Param_S_PTR pstParam = (AI_DelayComps_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AISetDelayComps(pstParam->hAI, &pstParam->stDelayComps);
            break;
        }

        case CMD_AI_GETDELAYCOMPS:
        {
            AI_DelayComps_Param_S_PTR pstParam = (AI_DelayComps_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIGetDelayComps(pstParam->hAI, &pstParam->stDelayComps);
            break;
        }

#ifdef __DPT__
        case CMD_AI_GETSTREAMTYPE:
        {
            AI_StreamType_Param_S_PTR pstParam = (AI_StreamType_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIGetStreamType(pstParam->hAI, &pstParam->enStreamType);
            break;
        }

        case CMD_AI_SETNRATTRS:
        {
            AI_NR_Param_S_PTR pstParam = (AI_NR_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AISetNRAttr(pstParam->hAI, &pstParam->stNR);
            break;
        }

        case CMD_AI_GETNRATTRS:
        {
            AI_NR_Param_S_PTR pstParam = (AI_NR_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AI_GetNRAttr(pstParam->hAI, &pstParam->stNR);
            break;
        }
#endif

        case CMD_AI_PROCINIT:
        {
            AI_ProcInit_Param_S_PTR pstParam = (AI_ProcInit_Param_S_PTR)arg;
            CHECK_AI_ID(pstParam->hAI);
            s32Ret = AIProcInit(pstParam->hAI, &pstParam->tProcPhyAddr);
            break;
        }

        case CMD_AI_PROCDEINIT:
        {
            HI_HANDLE hAI = *(HI_HANDLE*)arg;
            CHECK_AI_ID(hAI);
            s32Ret = AIProcDeInit(hAI);
            break;
        }

        default:
        {
            s32Ret = HI_ERR_AI_INVALID_PARA;
            HI_WARN_AI("unknown cmd: 0x%x\n", cmd);
            break;
        }

    }

    return s32Ret;
}

long AI_DRV_Ioctl(struct file* file, HI_U32 cmd, unsigned long arg)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AIMutex);

    s32Ret = HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, AI_ProcessCmd);

    up(&g_AIMutex);
    return (long)s32Ret;
}

HI_S32 AI_DRV_Open(struct inode* inode, struct file* filp)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = down_interruptible(&g_AIMutex);

    if (1 == atomic_inc_return(&g_stAIDrv.atmOpenCnt))
    {
        s32Ret = AI_OpenDev();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AI("call AI_OpenDev failed(0x%x)!", s32Ret);
        }
    }

    up(&g_AIMutex);
    return s32Ret;
}

HI_S32 AI_DRV_Release(struct inode* inode, struct file* filp)
{
    HI_S32 s32Ret, i;
    HI_HANDLE hAI;

    s32Ret = down_interruptible(&g_AIMutex);

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_stAIDrv.apAiState[i])
        {
            continue;
        }

        if (filp != g_stAIDrv.apAiState[i]->pstFile)
        {
            continue;
        }

        hAI = (HI_HANDLE)i;
        s32Ret = AIProcDeInit(hAI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIProcDeInit failed(hAI = %d, s32Ret = 0x%x)!\n", hAI, s32Ret);
        }

        s32Ret = AIDestory(hAI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIDestory failed(hAI = %d, s32Ret = 0x%x)!\n", hAI, s32Ret);
        }

        AIFreeHandle(hAI);
    }

    if (atomic_dec_and_test(&g_stAIDrv.atmOpenCnt))
    {
        s32Ret = AI_CloseDev();
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AI("call AI_CloseDev failed(0x%x)!", s32Ret);
        }
    }

    up(&g_AIMutex);

    return s32Ret;
}

#if (1 == HI_PROC_SUPPORT)
static HI_S32 AISaveThread(HI_VOID* args)
{
    HI_HANDLE hAI = (HI_U8*)args - (HI_U8*)HI_NULL;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ReadBytes = 0;
    HI_U32 u32NeedBytes = 0;
    HI_U32 u32DataBytes = 0, u32WritePos = 0;

    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[hAI & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(hAI, state);

    if (AI_CHN_STATUS_STOP == state->enCurnStatus)
    {
        HI_ERR_AI("current state is stop,can not save frame!\n");
        return HI_FAILURE;
    }

    u32NeedBytes = AICalcAcquireSize(hAI);

    while (HI_TRUE == state->bSaveThreadRun)
    {
        u32DataBytes = HAL_AIAO_P_QueryBufData_ProvideRptr(state->enPort, &state->u32SaveReadPos);
        if (u32DataBytes < u32NeedBytes)
        {
            msleep(1);
            continue;
        }

        if (state->pSaveBuf && state->fileHandle)
        {
            u32ReadBytes = HAL_AIAO_P_ReadData_NotUpRptr(state->enPort, (HI_U8*)state->pSaveBuf, u32NeedBytes, &state->u32SaveReadPos, &u32WritePos);
            if (u32ReadBytes != u32NeedBytes)
            {
                HI_ERR_AI("HAL_AIAO_P_ReadData_NotUpRptr Error!\n");
                return HI_FAILURE;
            }
            s32Ret = HI_DRV_FILE_Write(state->fileHandle, (HI_S8*)state->pSaveBuf, u32NeedBytes);
            if (s32Ret < 0)
            {
                HI_ERR_AI("HI_DRV_FILE_Write return err(0x%x)!\n", s32Ret);
            }
        }
        else
        {
            msleep(1);
        }
    }

    return HI_SUCCESS;
}

HI_S32 AI_WriteProc(HI_U32 u32Ai, AI_CMD_CTRL_E enCmd)
{
    static struct task_struct* g_pstAiSaveThread = HI_NULL;
    HI_CHAR szPath[AI_PATH_NAME_MAXLEN + AI_FILE_NAME_MAXLEN] = {0};
    struct tm now;

    AI_CHN_STATE_S* state = g_stAIDrv.apAiState[u32Ai & AI_CHNID_MASK];
    CHECK_AI_CHN_STATE(u32Ai, state);

    if (AI_CMD_CTRL_START == enCmd && AI_CMD_CTRL_STOP == state->enSaveState)
    {
        if (HI_SUCCESS != HI_DRV_FILE_GetStorePath(szPath, AI_PATH_NAME_MAXLEN))
        {
            HI_ERR_AI("get store path failed\n");
            return HI_FAILURE;
        }
        time_to_tm(get_seconds(), 0, &now);

        snprintf(szPath, sizeof(szPath), "%s/ai%d_%02u_%02u_%02u.pcm", szPath, u32Ai, now.tm_hour, now.tm_min, now.tm_sec);
        state->fileHandle = HI_DRV_FILE_Open(szPath, 1);
        if (!state->fileHandle)
        {
            HI_ERR_AI("open %s error\n", szPath);
            return HI_FAILURE;
        }

        state->pSaveBuf = HI_KMALLOC(HI_ID_AI, state->stAiBuf.u32Size, GFP_KERNEL);
        if (!state->pSaveBuf)
        {
            HI_DRV_FILE_Close(state->fileHandle);
            HI_ERR_AI("malloc save buffer failed\n");
            return HI_FAILURE;
        }

        state->bSaveThreadRun = HI_TRUE;
        g_pstAiSaveThread = kthread_create(AISaveThread, (HI_VOID*)((HI_U8*)HI_NULL + u32Ai), "AiSaveDataThread");
        if (HI_NULL == g_pstAiSaveThread)
        {
            HI_ERR_AI("creat ai save thread failed\n");
            return HI_FAILURE;
        }
        wake_up_process(g_pstAiSaveThread);

        HI_DRV_PROC_EchoHelper("Started saving ai data to %s\n", szPath);

        state->u32SaveCnt++;
    }
    else if ( AI_CMD_CTRL_START == enCmd && AI_CMD_CTRL_START == state->enSaveState)
    {
        HI_DRV_PROC_EchoHelper("Started saving ai data already!\n");
    }

    if (AI_CMD_CTRL_STOP == enCmd && AI_CMD_CTRL_START == state->enSaveState)
    {
        state->bSaveThreadRun = HI_FALSE;

        if (state->fileHandle)
        {
            HI_DRV_FILE_Close(state->fileHandle);
            state->fileHandle = HI_NULL;
        }
        if (state->pSaveBuf)
        {
            HI_KFREE(HI_ID_AI, state->pSaveBuf);
            state->pSaveBuf = HI_NULL;
        }

        HI_DRV_PROC_EchoHelper("Finished saving ai data.\n");
    }
    else if ( AI_CMD_CTRL_STOP == enCmd && AI_CMD_CTRL_STOP == state->enSaveState)
    {
        HI_DRV_PROC_EchoHelper("No ai data saved, please start saving.\n");
        AI_PROC_SHOW_HELP(u32Ai);
    }

    state->enSaveState = enCmd;

    return HI_SUCCESS;
}

static HI_VOID AIShowSpecAttr(struct seq_file* p, AI_CHN_STATE_S* state)
{
    HI_UNF_AI_I2S_ATTR_S stI2sAttr;
    HI_UNF_AI_HDMI_ATTR_S stHDMIAttr;

    switch (state->enAiPort)
    {
        case HI_UNF_AI_I2S0:
        case HI_UNF_AI_I2S1:
            stI2sAttr = state->stSndPortAttr.unAttr.stI2sAttr;
            PROC_PRINT(p,
                       "Channel                              :%d\n"
                       "BitWidth                             :%d\n"
                       "ClkMode                              :%s\n"
                       "I2sMode                              :%s\n"
                       "Mclk/Fs                              :%d\n"
                       "Mclk/Bclk                            :%d\n"
                       "SampleEdge                           :%s\n"
                       "DelayCycle                           :%d\n",
                       stI2sAttr.stAttr.enChannel,
                       stI2sAttr.stAttr.enBitDepth,
                       (HI_TRUE == stI2sAttr.stAttr.bMaster) ? "Master" : "Slave",
                       (HI_UNF_I2S_STD_MODE == stI2sAttr.stAttr.enI2sMode) ? "Standard" : "Pcm",
                       AUTIL_MclkFclkDiv(stI2sAttr.stAttr.enMclkSel),
                       stI2sAttr.stAttr.enBclkSel,
                       (HI_TRUE == stI2sAttr.stAttr.bPcmSampleRiseEdge) ? "Positive" : "Negative",
                       stI2sAttr.stAttr.enPcmDelayCycle);
            break;
        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            stHDMIAttr = state->stSndPortAttr.unAttr.stHDMIAttr;
            PROC_PRINT(p,
                       "Channel                              :%d\n"
                       "BitWidth                             :%d\n"
                       "Format                               :%s\n",
                       stHDMIAttr.enChannel,
                       stHDMIAttr.enBitDepth,
                       (HI_UNF_AI_HDMI_FORMAT_LPCM == stHDMIAttr.enHdmiAudioDataFormat) ? "Pcm" : ((HI_UNF_AI_HDMI_FORMAT_LBR == stHDMIAttr.enHdmiAudioDataFormat) ? "LBR" : "HBR"));
#ifdef __DPT__
            if (HI_UNF_AI_HDMI_FORMAT_LPCM != stHDMIAttr.enHdmiAudioDataFormat)
            {
                PROC_PRINT(p,
                           "DataType                             :%s\n",
                           AUTIL_DataType2Name(state->stAiProcAttr.pstAiProcInfo->enDataType));
            }
#endif
            break;
        default:
            break;
    }
}

static HI_S32 AI_ShowChnProc(struct seq_file* p, HI_U32 u32Chn)
{
    HI_S32 Ret;
    HI_U32 u32BufSizeUsed, u32BufPerCentUsed;
    AIAO_PORT_ID_E enPort;
    AIAO_PORT_STAUTS_S pstPortStatus;
    AI_CHN_STATE_S* state = HI_NULL;

    state = g_stAIDrv.apAiState[u32Chn];

    enPort = state->enPort;

    memset(&pstPortStatus, 0, sizeof(AIAO_PORT_STAUTS_S));
    Ret = HAL_AIAO_P_GetStatus(enPort, &pstPortStatus);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    if (*(pstPortStatus.stCircBuf.pu32Write) >= *(pstPortStatus.stCircBuf.pu32Read))
    {
        u32BufSizeUsed = *(pstPortStatus.stCircBuf.pu32Write) - *(pstPortStatus.stCircBuf.pu32Read);
    }
    else
    {
        u32BufSizeUsed = pstPortStatus.stCircBuf.u32Lenght - (*(pstPortStatus.stCircBuf.pu32Read) - * (pstPortStatus.stCircBuf.pu32Write));
    }

    u32BufPerCentUsed = u32BufSizeUsed * 100 / pstPortStatus.stCircBuf.u32Lenght;

    PROC_PRINT( p,
                "\n--------------------- AI%d[%s] Status ---------------------\n",
                u32Chn,
                AUTIL_AiPort2Name(state->enAiPort));
    PROC_PRINT(p,
               "Status                               :%s\n",
               (HI_CHAR*)((AIAO_PORT_STATUS_START == pstPortStatus.enStatus) ? "start" : ((AIAO_PORT_STATUS_STOP == pstPortStatus.enStatus) ? "stop" : "stopping")));
    PROC_PRINT(p,
               "SampleRate                           :%d\n",
               state->stSndPortAttr.enSampleRate);
    PROC_PRINT(p,
               "PcmFrameMaxNum                       :%d\n",
               state->stSndPortAttr.u32PcmFrameMaxNum);
    PROC_PRINT(p,
               "PcmSamplesPerFrame                   :%d\n",
               state->stSndPortAttr.u32PcmSamplesPerFrame);

    AIShowSpecAttr(p, state);

    PROC_PRINT(p,
               "DelayCompensation                    :%dms\n",
               state->stDelayComps.u32DelayMs);
    PROC_PRINT(p,
               "*AiPort                              :0x%.2x\n",
               enPort);
    PROC_PRINT(p,
               "*Alsa                                :%s\n",
               (state->bAlsa == HI_TRUE) ? "Yes" : "No");
#ifdef __DPT__
    PROC_PRINT(p,
               "*NR MODE                             :%s\n",
               (state->stNR.enNRType == HI_UNF_AI_NR_NORMAL) ? "Normal" : ((state->stNR.enNRType == HI_UNF_AI_NR_MUTE) ? "Mute" : "Off"));
    PROC_PRINT(p,
               "*NR level                            :%d\n",
               ((state->stNR.enNRType == HI_UNF_AI_NR_MUTE) ? (state->stNR.unNRAttr.stMuteAttr.s32Threshold) : 0xFF) );
#endif
    PROC_PRINT(p,
               "DmaCnt                               :%d\n",
               pstPortStatus.stProcStatus.uDMACnt);
    PROC_PRINT(p,
               "BufFullCnt                           :%d\n",
               pstPortStatus.stProcStatus.uBufFullCnt);
    PROC_PRINT(p,
               "FiFoFullCnt                          :%d\n",
               pstPortStatus.stProcStatus.uInfFiFoFullCnt);
    PROC_PRINT(p,
               "FrameBuf(Total/Use/Percent)(Bytes)   :%d/%d/%d%%\n",
               pstPortStatus.stBuf.u32BUFF_SIZE, u32BufSizeUsed, u32BufPerCentUsed);
    PROC_PRINT(p,
               "AcquireFrame(Try/OK)                 :%d/%d\n",
               state->stAiProcAttr.pstAiProcInfo->u32AqcTryCnt, state->stAiProcAttr.pstAiProcInfo->u32AqcCnt);
    PROC_PRINT(p,
               "ReleaseFrame(Try/OK)                 :%d/%d\n",
               state->stAiProcAttr.pstAiProcInfo->u32RelTryCnt, state->stAiProcAttr.pstAiProcInfo->u32RelCnt);
    PROC_PRINT( p,
                "-----------------------------------------------------------\n");

    return HI_SUCCESS;
}

HI_S32 AI_DRV_ReadProc(struct seq_file* filp, HI_VOID* args)
{
    HI_S32 i;

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (g_stAIDrv.apAiState[i])
        {
            AI_ShowChnProc(filp, i);
        }
    }

    return HI_SUCCESS;
}

HI_S32 AI_DRV_WriteProc(struct file* file, const char __user* buf, size_t count, loff_t* ppos)
{
    HI_S32 s32Ret;
    HI_U32 u32Ai;

    AI_CMD_PROC_E enProcCmd;
    AI_CMD_CTRL_E enCtrlCmd;

    HI_CHAR szBuf[256] = {0};
    HI_CHAR* pcBuf = szBuf;
    HI_CHAR* pcStartCmd = "start";
    HI_CHAR* pcStopCmd = "stop";
    HI_CHAR* pcSaveAICmd = "save";
    HI_CHAR* pcNRSwitch = "NR";
    HI_CHAR* pcNRLevel = "setNRlevel";
#ifdef __DPT__
    HI_CHAR* pcNormalCmd = "normal";
    HI_CHAR* pcMuteCmd = "mute";
    HI_CHAR* pcOffCmd = "off";
#endif
    HI_CHAR* pcSetAIDelayCmd = "delay";

    HI_CHAR* pcHelpCmd = "help";
    AI_CHN_STATE_S* state = HI_NULL;

    struct seq_file* p = file->private_data;
    DRV_PROC_ITEM_S* pstProcItem = p->private;

    s32Ret = down_interruptible(&g_AIMutex);
    if (copy_from_user(szBuf, buf, count < sizeof(szBuf) - 1 ? count : sizeof(szBuf) - 1))
    {
        HI_ERR_AI("copy from user failed\n");
        up(&g_AIMutex);
        return HI_FAILURE;
    }

    // sizeof("ai") is 2
    u32Ai = (pstProcItem->entry_name[2] - '0');
    if (u32Ai >= AI_MAX_TOTAL_NUM)
    {
        HI_ERR_AI("Invalid Ai ID:%d.\n", u32Ai);
        goto SAVE_CMD_FAULT;
    }

    state = g_stAIDrv.apAiState[u32Ai];
    if (HI_NULL == state)
    {
        HI_ERR_AI("this AI chn is not open!\n");
        goto SAVE_CMD_FAULT;
    }

    AI_STRING_SKIP_BLANK(pcBuf);
    if (strstr(pcBuf, pcSaveAICmd))
    {
        enProcCmd = AI_CMD_PROC_SAVE_AI;
        pcBuf += strlen(pcSaveAICmd);
    }
    else if (strstr(pcBuf, pcNRLevel))
    {
        enProcCmd = AI_CMD_PROC_SET_NRLEVEL;
        pcBuf += strlen(pcNRLevel);
    }
    else if (strstr(pcBuf, pcNRSwitch))
    {
        enProcCmd = AI_CMD_PROC_NR_SWITCH;
        pcBuf += strlen(pcNRSwitch);
    }
    else if (strstr(pcBuf, pcSetAIDelayCmd))
    {
        enProcCmd = AI_CMD_PROC_SET_DELAY;
        pcBuf += strlen(pcSetAIDelayCmd);
    }
    else if (strstr(pcBuf, pcHelpCmd))
    {
        AI_PROC_SHOW_HELP(u32Ai);
        up(&g_AIMutex);
        return count;
    }
    else
    {
        goto SAVE_CMD_FAULT;
    }

    AI_STRING_SKIP_BLANK(pcBuf);

    if (AI_CMD_PROC_SAVE_AI == enProcCmd)
    {
        if (strstr(pcBuf, pcStartCmd))
        {
            enCtrlCmd = AI_CMD_CTRL_START;
        }
        else if (strstr(pcBuf, pcStopCmd))
        {
            enCtrlCmd = AI_CMD_CTRL_STOP;
        }
        else
        {
            goto SAVE_CMD_FAULT;
        }
        s32Ret = AI_WriteProc(u32Ai, enCtrlCmd);
        if (s32Ret != HI_SUCCESS)
        {
            goto SAVE_CMD_FAULT;
        }
    }

#ifdef __DPT__
    if (AI_CMD_PROC_NR_SWITCH == enProcCmd)
    {
        if (strstr(pcBuf, pcNormalCmd))
        {
            state->stNR.enNRType = HI_UNF_AI_NR_NORMAL;
            (HI_VOID)AO_Track_SetNRAttr(state->hTrack, &(state->stNR));
        }
        else if (strstr(pcBuf, pcMuteCmd))
        {
            state->stNR.enNRType = HI_UNF_AI_NR_MUTE;
            (HI_VOID)AO_Track_SetNRAttr(state->hTrack, &(state->stNR));
        }
        else if (strstr(pcBuf, pcOffCmd))
        {
            state->stNR.enNRType = HI_UNF_AI_NR_OFF;
            (HI_VOID)AO_Track_SetNRAttr(state->hTrack, &(state->stNR));
        }
        else
        {
            goto SAVE_CMD_FAULT;
        }
    }

    if (AI_CMD_PROC_SET_NRLEVEL == enProcCmd)
    {
        HI_S32  s32NRlevel = 0;

        if (pcBuf[0] != '-' || pcBuf[1] < '0' || pcBuf[1] > '9') //do not have param
        {
            return HI_FAILURE;
        }
        pcBuf += 1;
        s32NRlevel = (HI_U32)(-simple_strtoul(pcBuf, &pcBuf, 10));
        state->stNR.unNRAttr.stMuteAttr.s32Threshold = s32NRlevel;
        s32Ret = AISetNRAttr((HI_HANDLE)u32Ai, &(state->stNR));
        HI_DRV_PROC_EchoHelper("Set NR level %s!\n",
                               (HI_SUCCESS == s32Ret) ? "success" : "faiure");
    }
#endif

    if (AI_CMD_PROC_SET_DELAY == enProcCmd)
    {
        HI_U32  u32AIDelay = 0;
        HI_U32  u32BufDelayMs = 0;
        HI_UNF_AI_DELAY_S   stGetDelayComps;
        HI_UNF_AI_DELAY_S   stSetDelayComps;
        AI_CHN_STATUS_E enStatusTmp;

        if (pcBuf[0] < '0' || pcBuf[0] > '9')//do not have param
        {
            return HI_FAILURE;
        }

        u32AIDelay = (HI_U32)simple_strtoul(pcBuf, &pcBuf, 10);

        stSetDelayComps.bDelayMsAutoHold = HI_FALSE;
        stSetDelayComps.u32DelayMs       = u32AIDelay;
        stGetDelayComps.bDelayMsAutoHold = HI_NULL;
        stGetDelayComps.u32DelayMs       = 0;

        enStatusTmp = state->enCurnStatus;

        if (AI_CHN_STATUS_START == enStatusTmp)
        {
            AI_SetEnable(u32Ai, HI_FALSE, HI_FALSE);
        }

        if (state->stSndPortAttr.enSampleRate)
        {
            u32BufDelayMs = state->stSndPortAttr.u32PcmSamplesPerFrame * state->stSndPortAttr.u32PcmFrameMaxNum
                            * 1000 / state->stSndPortAttr.enSampleRate;
        }

        if (stSetDelayComps.u32DelayMs <= u32BufDelayMs)
        {
            s32Ret = AIGetDelayComps(u32Ai, &stGetDelayComps);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AI("AI_GetDelayComps Failed!\n");
            }

            s32Ret = AISetDelayComps(u32Ai, &stSetDelayComps);
            if (HI_SUCCESS == s32Ret)
            {
                HI_DRV_PROC_EchoHelper("Set ai%d delay success:%dms -> %dms\n", u32Ai,
                                       stGetDelayComps.u32DelayMs, stSetDelayComps.u32DelayMs);
            }
            else
            {
                HI_DRV_PROC_EchoHelper("Set ai%d delay faiure! \n", u32Ai);
                AI_PROC_SHOW_HELP(u32Ai);
            }
        }
        else
        {
            HI_DRV_PROC_EchoHelper("Set ai%d delay faiure! Ai delay range: [0~%d]ms\n", u32Ai, u32BufDelayMs);
            AI_PROC_SHOW_HELP(u32Ai);
        }

        if (AI_CHN_STATUS_START == enStatusTmp)
        {
            AI_SetEnable(u32Ai, HI_TRUE, HI_FALSE);
        }
    }

    up(&g_AIMutex);
    return count;

SAVE_CMD_FAULT:

    HI_ERR_AI("Command  NOT  found!\n");
    AI_PROC_SHOW_HELP(u32Ai);
    up(&g_AIMutex);

    return HI_FAILURE;
}

static HI_S32 AI_RegProc(HI_HANDLE hAI)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S* pProcItem;

    /* Check parameters */
    if (HI_NULL == g_stAIDrv.pstProcParam)
    {
        return HI_FAILURE;
    }

    hAI &= AI_CHNID_MASK;

    /* Create proc */
    snprintf(aszBuf, sizeof(aszBuf), "ai%d", hAI);
    pProcItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);
    if (!pProcItem)
    {
        HI_FATAL_AO("Create ai proc entry fail!\n");
        return HI_FAILURE;
    }

    /* Set functions */
    pProcItem->read  = g_stAIDrv.pstProcParam->pfnReadProc;
    pProcItem->write = g_stAIDrv.pstProcParam->pfnWriteProc;

    HI_INFO_AI("Create AI proc entry OK!\n");
    return HI_SUCCESS;
}

static HI_VOID AI_UnRegProc(HI_HANDLE hAI)
{
    HI_CHAR aszBuf[16];
    hAI &= AI_CHNID_MASK;
    snprintf(aszBuf, sizeof(aszBuf), "ai%d", hAI);

    HI_DRV_PROC_RemoveModule(aszBuf);
}

HI_S32 AI_DRV_RegisterProc(AI_REGISTER_PARAM_S* pstParam)
{
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }
    g_stAIDrv.pstProcParam = pstParam;

    return HI_SUCCESS;
}

HI_VOID AI_DRV_UnregisterProc(HI_VOID)
{
    g_stAIDrv.pstProcParam = HI_NULL;
}
#endif

HI_S32 AI_DRV_Suspend(PM_BASEDEV_S* dev,
                      pm_message_t msg)
{
#if defined (HI_AI_DRV_SUSPEND_SUPPORT)
    HI_U32 i;
    HI_S32 s32Ret;
    AI_CHN_STATE_S* pAistate = HI_NULL;

    s32Ret = down_interruptible(&g_AIMutex);
    if (0 == atomic_read(&g_stAIDrv.atmOpenCnt))
    {
        up(&g_AIMutex);
        return HI_SUCCESS; // TODO: return value
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_stAIDrv.apAiState[i])
        {
            continue;
        }

        pAistate = g_stAIDrv.apAiState[i];
        s32Ret = AIDestory(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AI("AI Destory fail\n");
            up(&g_AIMutex);
            return HI_FAILURE;
        }
    }

    s32Ret = HAL_AIAO_Suspend();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AI("AIAO Suspend fail\n");
        up(&g_AIMutex);
        return HI_FAILURE;
    }

    HI_PRINT("AI suspend OK\n");
#endif
    up(&g_AIMutex);
    return HI_SUCCESS;
}

HI_S32 AI_DRV_Resume(PM_BASEDEV_S* dev)
{
#if defined (HI_AI_DRV_SUSPEND_SUPPORT)
    HI_S32 s32Ret;
    HI_U32 i;
    HI_UNF_AI_E enAiPort;
    HI_UNF_AI_ATTR_S stAiAttr;
    HI_BOOL bAlsa;
    HI_VOID* pAlsaPara;
    AI_CHN_STATE_S* state = HI_NULL;
    AI_CHN_STATUS_E  enAiStatus;

    s32Ret = down_interruptible(&g_AIMutex);

    if (0 == atomic_read(&g_stAIDrv.atmOpenCnt))
    {
        up(&g_AIMutex);
        return HI_SUCCESS; // TODO: return value
    }

    s32Ret = HAL_AIAO_Resume();
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AI("call HAL_AIAO_Resume failed(0x%x)!\n", s32Ret);
        up(&g_AIMutex);
        return s32Ret;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_stAIDrv.apAiState[i])
        {
            continue;
        }

        state = g_stAIDrv.apAiState[i];
        enAiPort = state->enAiPort;
        stAiAttr = state->stSndPortAttr;
        bAlsa = state->bAlsa;
        pAlsaPara = state->pAlsaPara;
        enAiStatus = state->enCurnStatus;
#ifdef HI_ALSA_AI_SUPPORT
        if (HI_TRUE == bAlsa)
        {
            pAlsaPara = (void*)&g_stAlsaAttr;
        }
#endif
        s32Ret = AICreate(i);
        if (HI_SUCCESS != s32Ret)
        {
            HI_FATAL_AI("AICreate failed(0x%x)\n", s32Ret);
            up(&g_AIMutex);
            return HI_FAILURE;
        }

        if ((AI_CHN_STATUS_START == enAiStatus) && (!(state->bAttach)))
        {
            s32Ret = AI_SetEnable(i, HI_TRUE, HI_FALSE);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_AI("Set AI Enable failed\n");
                up(&g_AIMutex);
                return HI_FAILURE;
            }
        }
    }

    up(&g_AIMutex);
    HI_PRINT("AI resume OK\n");
#endif
    return HI_SUCCESS;
}

HI_S32 AI_DRV_Init(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AIMutex);
    s32Ret = HI_DRV_MODULE_Register(HI_ID_AI, AI_NAME, (HI_VOID*)&g_stAIDrv.stExtFunc);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FATAL_AI("HI_DRV_MODULE_Register HI_ID_AI failed(0x%x)!\n", s32Ret);
    }

    up(&g_AIMutex);
    return s32Ret;
}

HI_VOID AI_DRV_Exit(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = down_interruptible(&g_AIMutex);

    HI_DRV_MODULE_UnRegister(HI_ID_AI);

    up(&g_AIMutex);
    return;
}

HI_S32 HI_DRV_AI_Init(HI_VOID)
{
    return AI_DRV_Init();
}

HI_VOID HI_DRV_AI_DeInit(HI_VOID)
{
    AI_DRV_Exit();
}

HI_S32 HI_DRV_AI_Open(struct file* file)
{
    return AI_DRV_Open(HI_NULL, file);
}

HI_S32 HI_DRV_AI_Release(struct file* file)
{
    return AI_DRV_Release(HI_NULL, file);
}

HI_S32 HI_DRV_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAttr)
{
    return AIGetDefaultAttr(enAiPort, pstAttr);
}

HI_S32 HI_DRV_AI_Create(AI_DRV_Create_Param_S* pstAiPara, struct file* file)
{
    HI_S32 s32Ret;
    HI_HANDLE hAI = HI_INVALID_HANDLE;

    s32Ret = AIAllocHandle(&hAI, file, pstAiPara->enAiPort, &pstAiPara->stAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIAllocHandle failed(0x%x)!", s32Ret);
        return s32Ret;
    }

#ifdef HI_ALSA_AI_SUPPORT
    if (HI_TRUE == pstAiPara->bAlsa)
    {
        AIAlsaSetParam(hAI, pstAiPara->pAlsaPara);
    }
#endif

    s32Ret = AICreate(hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AICreate failed(0x%x)!", s32Ret);
        AIFreeHandle(hAI);
    }

    pstAiPara->hAI = hAI;

    return s32Ret;
}

HI_S32 HI_DRV_AI_Destroy(HI_HANDLE hAI)
{
    HI_S32 s32Ret;

    s32Ret = AIDestory(hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIDestory failed(0x%x)!", s32Ret);
        return s32Ret;
    }

    AIFreeHandle(hAI);

    return s32Ret;
}

HI_S32 HI_DRV_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable)
{
    return AI_SetEnable(hAI, bEnable, HI_FALSE);
}

HI_S32 HI_DRV_AI_GetEnable(HI_HANDLE hAI, HI_BOOL* pbEnable)
{
    return AI_GetEnable(hAI, pbEnable);
}

HI_S32 HI_DRV_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pstAttr)
{
    return AIGetAttr(hAI, pstAttr);
}

HI_S32 HI_DRV_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pstAttr)
{
    return AISetAttr(hAI, pstAttr);
}

HI_S32 HI_DRV_AI_AcquireFrame(HI_HANDLE hAI, AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;
    AI_BUF_ATTR_S stAiBuf;

    s32Ret = AIGetAiBufInfo(hAI, &stAiBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIGetAiBufInfo failed(0x%x)!", s32Ret);
        return s32Ret;
    }

    s32Ret = AIAcquireFrame(hAI, AICalcAcquireSize(hAI));
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIAcquireFrame failed(0x%x)!", s32Ret);
        return s32Ret;
    }

    // TODO: build ao frame
    pstFrame->tPcmBuffer = stAiBuf.tKernelVirAddr;

    return s32Ret;
}

HI_S32 HI_DRV_AI_ReleaseFrame(HI_HANDLE hAI)
{
    return AIReleaseFrame(hAI);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
