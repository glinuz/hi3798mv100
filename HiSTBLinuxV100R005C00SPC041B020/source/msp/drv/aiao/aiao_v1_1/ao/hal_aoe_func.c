/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hal_aoe_func.c
 * Description: aiao interface of module.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2012-09-22   z40717     NULL         init.
 ********************************************************************************/
#include <asm/setup.h>
#include <asm/io.h>
#include <linux/delay.h>

#include "hi_type.h"
#include "hi_audsp_aoe.h"
#include "hal_aoe_func.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

static volatile S_AOE_REGS_TYPE*        g_pAOEReg;
static volatile S_AIP_REGS_TYPE*        g_pAipReg[AOE_AIP_BUTT];
static volatile S_MIXER_REGS_TYPE*      g_pMixerReg[AOE_ENGINE_BUTT];
static volatile S_AOP_REGS_TYPE*        g_pAopReg[AOE_AOP_BUTT];
static  HI_VOID* pRegMapAddr = HI_NULL;
static HI_BOOL g_bSwAoeFlag = HI_TRUE;  /* HI_TRUE: sw; HI_FALSE: hw */

static HI_VOID AoeIOAddressMap(HI_VOID)
{
    AOE_AIP_ID_E aip;
    AOE_AOP_ID_E aop;
    AOE_ENGINE_ID_E engine;

    pRegMapAddr = ioremap_nocache(AOE_COM_REG_BASE, AOE_REG_LENGTH);

    /* reg map */
    g_pAOEReg = (S_AOE_REGS_TYPE*)(pRegMapAddr + AOE_COM_REG_OFFSET);
    for (aip = AOE_AIP0; aip < AOE_AIP_BUTT; aip++)
    {
        g_pAipReg[aip] = (S_AIP_REGS_TYPE*)((pRegMapAddr + AOE_AIP_REG_OFFSET) + AOE_AIP_REG_BANDSIZE * aip);
    }

    for (aop = AOE_AOP0; aop < AOE_AOP_BUTT; aop++)
    {
        g_pAopReg[aop] = (S_AOP_REGS_TYPE*)((pRegMapAddr + AOE_AOP_REG_OFFSET) + AOE_AOP_REG_BANDSIZE * aop);
    }

    for (engine = AOE_ENGINE0; engine < AOE_ENGINE_BUTT; engine++)
    {
        g_pMixerReg[engine] = (S_MIXER_REGS_TYPE*)((pRegMapAddr
                              + AOE_ENGINE_REG_OFFSET) + AOE_ENGINE_REG_BANDSIZE * engine);
    }

}

static HI_VOID IOaddressUnmap(HI_VOID)
{
    AOE_AIP_ID_E aip;
    AOE_AOP_ID_E aop;
    AOE_ENGINE_ID_E engine;

    /* reg map */
    for (aip = AOE_AIP0; aip < AOE_AIP_BUTT; aip++)
    {
        g_pAipReg[aip] = HI_NULL;
    }

    for (aop = AOE_AOP0; aop < AOE_AOP_BUTT; aop++)
    {
        g_pAopReg[aop] = HI_NULL;
    }

    for (engine = AOE_ENGINE0; engine < AOE_ENGINE_BUTT; engine++)
    {
        g_pAipReg[engine] = HI_NULL;
    }

    g_pAOEReg = HI_NULL;
    if (pRegMapAddr)
    {
        iounmap(pRegMapAddr);
        pRegMapAddr = HI_NULL;
    }
}

HI_S32 iHAL_AOE_Init(HI_BOOL bSwAoeFlag)
{
    AoeIOAddressMap();
    g_bSwAoeFlag = bSwAoeFlag;
    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_DeInit(HI_VOID)
{
    IOaddressUnmap();
}

HI_VOID iHAL_AOE_GetHwCapability(HI_U32* pu32Capability)
{
}

HI_VOID iHAL_AOE_GetHwVersion(HI_U32* pu32Version)
{
}

static HI_VOID AoeRegBitDepth(HI_U32 u32BitPerSample, HI_U32* pReg)
{
    switch (u32BitPerSample)
    {
        case 16:
            *pReg = 1;
            break;
        case 24:
            *pReg = 2;
            break;
        default:
            *pReg = 0;
    }

    return;
}

static HI_VOID AoeRegChannels(HI_U32 u32Channels, HI_U32* pReg)
{
    switch (u32Channels)
    {
        case 0x01:
            *pReg = 0;
            break;
        case 0x02:
            *pReg = 1;
            break;
        case 0x08:
            *pReg = 3;
            break;
        default:
            *pReg = 0;
    }

    return;
}

static HI_VOID AoeRegSampelRate(HI_U32 u32SampelRate, HI_U32* pReg)
{
    switch (u32SampelRate)
    {
        case 8000:
            *pReg = 0;
            break;
        case 11025:
            *pReg = 1;
            break;
        case 12000:
            *pReg = 2;
            break;
        case 16000:
            *pReg = 3;
            break;
        case 22050:
            *pReg = 4;
            break;
        case 24000:
            *pReg = 5;
            break;
        case 32000:
            *pReg = 6;
            break;
        case 44100:
            *pReg = 7;
            break;
        case 48000:
            *pReg = 8;
            break;
        case 88200:
            *pReg = 9;
            break;
        case 96000:
            *pReg = 10;
            break;
        case 176400:
            *pReg = 11;
            break;
        case 192000:
            *pReg = 12;
            break;
        default:
            *pReg = 0xf; // ext fs
    }

    return;
}

static HI_VOID AoeRegMixRoute(HI_U32 enAIP, HI_U32* pReg)
{
    switch (enAIP)
    {
        case 0:
            *pReg = 1 << 0;
            break;
        case 1:
            *pReg = 1 << 1;
            break;
        case 2:
            *pReg = 1 << 2;
            break;
        case 3:
            *pReg = 1 << 3;
            break;
        case 4:
            *pReg = 1 << 4;
            break;
        case 5:
            *pReg = 1 << 5;
            break;
        case 6:
            *pReg = 1 << 6;
            break;
        case 7:
            *pReg = 1 << 7;
            break;
        case 8:
            *pReg = 1 << 8;
            break;
        case 9:
            *pReg = 1 << 9;
            break;
        case 10:
            *pReg = 1 << 10;
            break;
        case 11:
            *pReg = 1 << 11;
            break;
        case 12:
            *pReg = 1 << 12;
            break;
        case 13:
            *pReg = 1 << 13;
            break;
        case 14:
            *pReg = 1 << 14;
            break;
        case 15:
            *pReg = 1 << 15;
            break;

        default:
            *pReg = 0x0;
    }

    return;
}

HI_VOID AoeAipGetRemapAddr(HI_PHYS_ADDR_T* u32DspRemapAddr)
{
#if defined(DSP_DDR_DMAREMAP_SUPPORT)
    if ((*u32DspRemapAddr >= DSP_DDR_DMAREMAP_BEG_ADDR) && (*u32DspRemapAddr < DSP_DDR_DMAREMAP_END_ADDR))
    {
        *u32DspRemapAddr += DSP_DDR_DMAREMAP_MAP_ADDR;
    }
#endif
}

HI_U32 AoeAipGetDataFormat(HI_U32 u32DataFormat)
{
    if (0 == u32DataFormat)
    {
        return AOE_FORMAT_LPCM;
    }
    else
    {
        return (u32DataFormat & 0xff00) ? AOE_FORMAT_IEC61937_COMP : AOE_FORMAT_IEC61937;
    }
}

HI_VOID AoeAipBufSetAddr(S_AIP_REGS_TYPE* AipReg, AOE_AIP_INBUF_ATTR_S stBufInAttr)
{
    if (HI_TRUE == g_bSwAoeFlag)
    {
        ADSP_WriteAddr(stBufInAttr.stRbfAttr.tBufVirAddr, AipReg->AIP_BUF_ADDR);
        AipReg->AIP_BUF_PHYADDR = stBufInAttr.stRbfAttr.tBufPhyAddr;
    }
    else
    {
        HI_PHYS_ADDR_T u32DspRemapAddr = stBufInAttr.stRbfAttr.tBufPhyAddr;
        AoeAipGetRemapAddr(&u32DspRemapAddr);
        ADSP_WriteAddr(u32DspRemapAddr, AipReg->AIP_BUF_ADDR);
        AipReg->AIP_BUF_PHYADDR = u32DspRemapAddr;
    }

    if (stBufInAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        if (HI_TRUE == g_bSwAoeFlag)
        {
            ADSP_WriteAddr(stBufInAttr.stRbfAttr.tBufVirWptr, AipReg->AIP_BUF_WPTR);
            ADSP_WriteAddr(stBufInAttr.stRbfAttr.tBufVirRptr, AipReg->AIP_BUF_RPTR);
        }
        else
        {
            HI_PHYS_ADDR_T u32DspRemapAddr = stBufInAttr.stRbfAttr.tBufPhyWptr;
            AoeAipGetRemapAddr(&u32DspRemapAddr);
            ADSP_WriteAddr(u32DspRemapAddr, AipReg->AIP_BUF_WPTR);

            u32DspRemapAddr = stBufInAttr.stRbfAttr.tBufPhyRptr;
            AoeAipGetRemapAddr(&u32DspRemapAddr);
            ADSP_WriteAddr(u32DspRemapAddr, AipReg->AIP_BUF_RPTR);
        }
    }
    else
    {
        AipReg->AIP_BUF_WPTR.ADDR_HIGH = 0;
        AipReg->AIP_BUF_WPTR.ADDR_LOW  = 0;
        AipReg->AIP_BUF_RPTR.ADDR_HIGH = 0;
        AipReg->AIP_BUF_RPTR.ADDR_LOW  = 0;
    }

    AipReg->AIP_BUF_SIZE.bits.buff_flag = stBufInAttr.stRbfAttr.u32BufWptrRptrFlag;
    AipReg->AIP_BUF_SIZE.bits.buff_size = stBufInAttr.stRbfAttr.u32BufSize;
    AipReg->AIP_BUF_TRANS_SIZE.bits.tx_trans_size = 0;
}

HI_VOID AoeAipBufSetAttr(S_AIP_REGS_TYPE* AipReg, AOE_AIP_INBUF_ATTR_S stBufInAttr)
{
    HI_U32 Ch;
    HI_U32 Rate;
    HI_U32 u32DataFormat;
    HI_U32 BitDepth = 0;

    u32DataFormat = stBufInAttr.u32BufDataFormat;
    AipReg->AIP_BUFF_ATTR.bits.aip_format = AoeAipGetDataFormat(u32DataFormat);

    AoeRegBitDepth(stBufInAttr.u32BufBitPerSample, &BitDepth);
    AipReg->AIP_BUFF_ATTR.bits.aip_precision = BitDepth;
    AoeRegChannels(stBufInAttr.u32BufChannels, &Ch);
    AipReg->AIP_BUFF_ATTR.bits.aip_ch = Ch;
    AoeRegSampelRate(stBufInAttr.u32BufSampleRate, &Rate);
    AipReg->AIP_BUFF_ATTR.bits.aip_fs = Rate;
    AipReg->AIP_BUFF_ATTR.bits.aip_priority = ((HI_TRUE == stBufInAttr.bMixPriority) ? 1 : 0);
    if (0xf == Rate)//nonstandard samplerate
    {
        // todo, #define  AOE_AIP_EXT_FS 0xf
        AipReg->AIP_SRC_ATTR_EXT.bits.fs_ext = stBufInAttr.u32BufSampleRate;
    }

    if (HI_TRUE == stBufInAttr.bFadeEnable)
    {
        AipReg->AIP_CTRL.bits.fade_en = 1;

        AipReg->AIP_CTRL.bits.fade_in_rate = stBufInAttr.u32FadeinMs;
        AipReg->AIP_CTRL.bits.fade_out_rate = stBufInAttr.u32FadeoutMs;
    }
    else
    {
        AipReg->AIP_CTRL.bits.fade_en = 0;
    }

    AipReg->AIP_BUFF_ATTR.bits.aip_pcm_type = stBufInAttr.eAipType;
}

HI_VOID AoeAipFifoSetAttr(S_AIP_REGS_TYPE* AipReg, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    HI_U32 Ch;
    HI_U32 Rate;
    HI_U32 u32DataFormat;
    HI_U32 BitDepth = 0;

    AoeRegBitDepth(pstAttr->stFifoOutAttr.u32FifoBitPerSample, &BitDepth);
    AipReg->AIP_FIFO_ATTR.bits.fifo_precision = BitDepth;
    AoeRegChannels(pstAttr->stFifoOutAttr.u32FifoChannels, &Ch);
    AipReg->AIP_FIFO_ATTR.bits.fifo_ch = Ch;
    AoeRegSampelRate(pstAttr->stFifoOutAttr.u32FifoSampleRate, &Rate);
    AipReg->AIP_FIFO_ATTR.bits.fifo_fs = Rate;

    u32DataFormat = pstAttr->stFifoOutAttr.u32FifoDataFormat;
    AipReg->AIP_FIFO_ATTR.bits.fifo_format = AoeAipGetDataFormat(u32DataFormat);

    AipReg->AIP_FIFO_ATTR.bits.fifo_latency = pstAttr->stFifoOutAttr.u32FiFoLatencyThdMs;
    AipReg->AIP_FIFO_ATTR.bits.fifo_bypass = ((HI_TRUE == pstAttr->bFifoBypass) ? 1 : 0);
}

HI_S32 iHAL_AOE_AIP_SetAttr(AOE_AIP_ID_E enAIP, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    AOE_AIP_INBUF_ATTR_S stBufInAttr = pstAttr->stBufInAttr;
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AoeAipBufSetAttr(AipReg, stBufInAttr);
    AoeAipBufSetAddr(AipReg, stBufInAttr);
    AoeAipFifoSetAttr(AipReg, pstAttr);
    return HI_SUCCESS;
}

HI_U32 iHAL_AOE_AIP_GetRegAddr(AOE_AIP_ID_E enAIP)
{
    return ((AOE_COM_REG_BASE + AOE_AIP_REG_OFFSET) + AOE_AIP_REG_BANDSIZE * (HI_U32)enAIP);
}

HI_S32 iHAL_AOE_AIP_GetStatus(AOE_AIP_ID_E enAIP, HI_VOID* pstStatus)
{
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AIP_SetMixPriority(AOE_AIP_ID_E enAIP, HI_BOOL bMixPriority)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    HI_WARN_AO("Set aip_priority %d!\n", bMixPriority);
    AipReg->AIP_BUFF_ATTR.bits.aip_priority = bMixPriority;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AIP_AttachAop(AOE_AIP_ID_E enAIP, AOE_AOP_ID_E enAOP, HI_U32 u32SkipMs)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT2_CTRL.bits.dest_aop = enAOP;
    AipReg->AIP_EXT2_CTRL.bits.skip_ms = u32SkipMs;
    AipReg->AIP_EXT2_CTRL.bits.is_duplicate = 1;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AIP_DetachAop(AOE_AIP_ID_E enAIP, AOE_AOP_ID_E enAOP)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT2_CTRL.bits.dest_aop = AOE_AOP_BUTT;
    AipReg->AIP_EXT2_CTRL.bits.skip_ms = 0;
    AipReg->AIP_EXT2_CTRL.bits.is_duplicate = 0;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AIP_Create(AOE_AIP_ID_E enAIP, AOE_AIP_CHN_ATTR_S* pstAttr)
{
    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_AIP_Destroy(AOE_AIP_ID_E enAIP)
{
    return;
}

AOE_AIP_CMD_RET_E iHAL_AOE_AIP_AckCmd(AOE_AIP_ID_E enAIP)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    volatile HI_U32 loop_outer = 0;
    volatile HI_U32 loop_inner = 0;
    for (loop_outer = 0; loop_outer < 100; loop_outer++)
    {
        for (loop_inner = 0; loop_inner < 8; loop_inner++)
        {
            if (AipReg->AIP_CTRL.bits.cmd_done)
            {
                return (AOE_AIP_CMD_RET_E)AipReg->AIP_CTRL.bits.cmd_return_value;
            }
            udelay(10);
        }
        msleep(1);
    }
    return AOE_AIP_CMD_ERR_TIMEOUT;
}

HI_U32 iHAL_AOE_AIP_Group_AckCmd(HI_U32 u32AckCmdMask)
{
    S_AIP_REGS_TYPE* AipReg[AOE_MAX_AIP_NUM] = {HI_NULL};
    HI_U32 u32CmdDoneMask = 0;
    HI_U32 u32AipId = 0;
    volatile HI_U32 u32LoopOuter = 0;
    volatile HI_U32 u32LoopInner = 0;

    for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
    {
        if ((1 << u32AipId) & u32AckCmdMask)
        {
            AipReg[u32AipId] = (S_AIP_REGS_TYPE*)g_pAipReg[u32AipId];
        }
    }

    for (u32LoopOuter = 0; u32LoopOuter < 100; u32LoopOuter++)
    {
        for (u32LoopInner = 0; u32LoopInner < 8; u32LoopInner++)
        {
            for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
            {
                if ((1 << u32AipId) & u32AckCmdMask)
                {
                    if (AipReg[u32AipId]->AIP_CTRL.bits.cmd_done)
                    {
                        u32CmdDoneMask |= (1 << u32AipId);
                    }
                }
            }

            if (u32AckCmdMask == u32CmdDoneMask)
            {
                return u32CmdDoneMask;
            }

            udelay(10);
        }

        msleep(1);
    }

    return u32CmdDoneMask;
}

AOE_AIP_CMD_RET_E iHAL_AOE_AIP_NoBlock_AckCmd(AOE_AIP_ID_E enAIP)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    volatile HI_U32 loop = 0;

    for (loop = 0; loop < 800; loop++)
    {
        if (AipReg->AIP_CTRL.bits.cmd_done)
        {
            return (AOE_AIP_CMD_RET_E)AipReg->AIP_CTRL.bits.cmd_return_value;
        }
    }
    return AOE_AIP_CMD_DONE;
}

HI_S32 iHAL_AOE_AIP_SetCmd(AOE_AIP_ID_E enAIP, AOE_AIP_CMD_E newcmd)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    AOE_AIP_CMD_RET_E Ack;

    switch (newcmd)
    {
        case AOE_AIP_CMD_START:
        case AOE_AIP_CMD_PAUSE:
        case AOE_AIP_CMD_FLUSH:
        case AOE_AIP_CMD_STOP:
            AipReg->AIP_CTRL.bits.cmd = newcmd;
            break;
        default:

            return HI_SUCCESS;
    }

    AipReg->AIP_CTRL.bits.cmd_done = 0;

    Ack = iHAL_AOE_AIP_AckCmd(enAIP);
    if (AOE_AIP_CMD_DONE != Ack)
    {
        HI_ERR_AO("AIP(%d) SetCmd(0x%x) failed(0x%x)\n", enAIP, newcmd, Ack);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_U32 iHAL_AOE_AIP_Group_SetCmd(AOE_AIP_CMD_E newcmd, HI_U32 u32SetCmdMask)
{
    S_AIP_REGS_TYPE* AipReg[AOE_MAX_AIP_NUM] = {HI_NULL};
    HI_U32 u32AipId = 0;
    HI_U32 u32CmdDone = 0;

    for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
    {
        if ((1 << u32AipId) & u32SetCmdMask)
        {
            AipReg[u32AipId] = (S_AIP_REGS_TYPE*)g_pAipReg[u32AipId];
        }
    }

    switch (newcmd)
    {
        case AOE_AIP_CMD_START:
            for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
            {
                if ((1 << u32AipId) & u32SetCmdMask)
                {
                    AipReg[u32AipId]->AIP_CTRL.bits.cmd = AOE_AIP_CMD_START;
                }
            }
            break;

        case AOE_AIP_CMD_PAUSE:
            for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
            {
                if ((1 << u32AipId) & u32SetCmdMask)
                {
                    AipReg[u32AipId]->AIP_CTRL.bits.cmd = AOE_AIP_CMD_PAUSE;
                }
            }
            break;

        case AOE_AIP_CMD_FLUSH:
            for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
            {
                if ((1 << u32AipId) & u32SetCmdMask)
                {
                    AipReg[u32AipId]->AIP_CTRL.bits.cmd = AOE_AIP_CMD_FLUSH;
                }
            }
            break;

        case AOE_AIP_CMD_STOP:
            for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
            {
                if ((1 << u32AipId) & u32SetCmdMask)
                {
                    AipReg[u32AipId]->AIP_CTRL.bits.cmd = AOE_AIP_CMD_STOP;
                }
            }
            break;

        default:
            return u32SetCmdMask;
    }

    for (u32AipId = 0; u32AipId < AOE_MAX_AIP_NUM; u32AipId++)
    {
        if ((1 << u32AipId) & u32SetCmdMask)
        {
            AipReg[u32AipId]->AIP_CTRL.bits.cmd_done = 0;
        }
    }

    u32CmdDone = iHAL_AOE_AIP_Group_AckCmd(u32SetCmdMask);

    return u32CmdDone;
}

HI_VOID iHAL_AOE_AIP_SetVolume(AOE_AIP_ID_E enAIP, HI_U32 u32VolumedB)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_CTRL.bits.volume = u32VolumedB;
}

HI_VOID iHAL_AOE_AIP_SetLRVolume(AOE_AIP_ID_E enAIP, HI_U32 u32VolumeLdB, HI_U32 u32VolumeRdB)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT_CTRL.bits.volume_l = u32VolumeLdB;
    AipReg->AIP_EXT_CTRL.bits.volume_r = u32VolumeRdB;
}

HI_VOID iHAL_AOE_AIP_SetPrescale(AOE_AIP_ID_E enAIP, HI_U32 u32IntdB, HI_S32 s32DecdB)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT2_CTRL.bits.prescale_int = u32IntdB;
    if (s32DecdB >= 0)
    {
        AipReg->AIP_EXT2_CTRL.bits.prescale_dec_dir = 0;
        AipReg->AIP_EXT2_CTRL.bits.prescale_dec = s32DecdB;
    }
    else
    {
        AipReg->AIP_EXT2_CTRL.bits.prescale_dec_dir = 1;
        AipReg->AIP_EXT2_CTRL.bits.prescale_dec = -s32DecdB;
    }
}

HI_VOID iHAL_AOE_AIP_SetMute(AOE_AIP_ID_E enAIP, HI_BOOL bMute)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT_CTRL.bits.mute = (HI_U32)bMute;
}

HI_VOID iHAL_AOE_AIP_SetChannelMode(AOE_AIP_ID_E enAIP, HI_U32 u32ChannelMode)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    AipReg->AIP_EXT_CTRL.bits.channel_mode = u32ChannelMode;
}

#ifdef __DPT__
HI_VOID iHAL_AOE_AIP_SetNRAttr(AOE_AIP_ID_E enAIP, HI_UNF_AI_NR_ATTR_S* pstNRParam)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    AipReg->AIP_EXT_CTRL.bits.dre = pstNRParam->enNRType;
    if (pstNRParam->enNRType == HI_UNF_AI_NR_NORMAL)
    {
        AipReg->AIP_EXT_CTRL.bits.dre_thrhd = 0x7f;
    }
    else if (pstNRParam->enNRType == HI_UNF_AI_NR_MUTE)
    {
        AipReg->AIP_EXT_CTRL.bits.dre_thrhd = -pstNRParam->unNRAttr.stMuteAttr.s32Threshold;
    }
    else
    {
        AipReg->AIP_EXT_CTRL.bits.dre_thrhd = 0;
    }
    AipReg->AIP_EXT_CTRL.bits.dre_dither_decbps = 3;  //default decrease 3 bps
}
#endif

HI_S32 iHAL_AOE_AIP_SetSpeed(AOE_AIP_ID_E enAIP, HI_S32 s32AdjSpeed)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    if (s32AdjSpeed >= 0)
    {
        AipReg->AIP_CTRL.bits.dst_fs_adj_dir = 0;
        AipReg->AIP_CTRL.bits.dst_fs_adj_step = s32AdjSpeed;
    }
    else
    {
        AipReg->AIP_CTRL.bits.dst_fs_adj_dir = 1;
        AipReg->AIP_CTRL.bits.dst_fs_adj_step = -s32AdjSpeed;
    }

    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_AIP_GetRptrAndWptrRegAddr(AOE_AIP_ID_E enAIP, HI_U32** ppu32WptrReg, HI_U32** ppu32RptrReg)
{
    *ppu32WptrReg = (HI_U32*)(&(g_pAipReg[enAIP]->AIP_BUF_WPTR.ADDR_LOW));
    *ppu32RptrReg = (HI_U32*)(&(g_pAipReg[enAIP]->AIP_BUF_RPTR.ADDR_LOW));
}

HI_VOID iHAL_AOE_AIP_ReSetRptrAndWptrReg(AOE_AIP_ID_E enAIP)
{
    g_pAipReg[enAIP]->AIP_BUF_WPTR.ADDR_LOW  = 0;
    g_pAipReg[enAIP]->AIP_BUF_WPTR.ADDR_HIGH = 0;
    g_pAipReg[enAIP]->AIP_BUF_RPTR.ADDR_LOW  = 0;
    g_pAipReg[enAIP]->AIP_BUF_RPTR.ADDR_HIGH = 0;
}

HI_U32 iHAL_AOE_AIP_GetFiFoDelayMs(AOE_AIP_ID_E enAIP)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];

    return AipReg->AIP_FIFO_ATTR.bits.fifo_latency_real;
}

HI_S32 iHAL_AOE_AIP_SetFifoBypass(AOE_AIP_ID_E enAIP, HI_BOOL bEnable)
{
    S_AIP_REGS_TYPE* AipReg = (S_AIP_REGS_TYPE*)g_pAipReg[enAIP];
    AipReg->AIP_FIFO_ATTR.bits.fifo_bypass = (HI_TRUE == bEnable) ? 1 :  0;
    return HI_SUCCESS;
}

/* aop func */
HI_VOID iHAL_AOE_AOP_SetMute(AOE_AOP_ID_E enAOP, HI_BOOL bMute)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    AopReg->AOP_EXT_CTRL.bits.mute = (HI_U32)bMute;
}

HI_VOID iHAL_AOE_AOP_SetLRVolume(AOE_AOP_ID_E enAOP, HI_U32 u32VolumeLdB, HI_U32 u32VolumeRdB)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_EXT_CTRL.bits.volume_l = u32VolumeLdB;
    AopReg->AOP_EXT_CTRL.bits.volume_r = u32VolumeRdB;
}

HI_VOID iHAL_AOE_AOP_SetPreciVol(AOE_AOP_ID_E enAOP, HI_U32 u32IntdB, HI_S32 s32DecdB)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_EXT_CTRL.bits.volume_l = u32IntdB;
    AopReg->AOP_EXT_CTRL.bits.volume_r = u32IntdB;
    if (s32DecdB >= 0)
    {
        AopReg->AOP_EXT_CTRL.bits.volumedec_dir_l = 0;
        AopReg->AOP_EXT_CTRL.bits.volumedec_dir_r = 0;
        AopReg->AOP_EXT_CTRL.bits.volumedec_l = s32DecdB;
        AopReg->AOP_EXT_CTRL.bits.volumedec_r = s32DecdB;
    }
    else
    {
        AopReg->AOP_EXT_CTRL.bits.volumedec_dir_l = 1;
        AopReg->AOP_EXT_CTRL.bits.volumedec_dir_r = 1;
        AopReg->AOP_EXT_CTRL.bits.volumedec_l = -s32DecdB;
        AopReg->AOP_EXT_CTRL.bits.volumedec_r = -s32DecdB;
    }
}

HI_VOID iHAL_AOE_AOP_SetDelay(AOE_AOP_ID_E enAOP, HI_U32 u32RealDelayMS)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    AopReg->AOP_CTRL.bits.realdelayms = u32RealDelayMS;
}

HI_VOID iHAL_AOE_AOP_GetDelay(AOE_AOP_ID_E enAOP, HI_U32* pu32RealDelayMS)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    *pu32RealDelayMS = AopReg->AOP_CTRL.bits.realdelayms;
}

HI_VOID iHAL_AOE_AOP_SetBalance(AOE_AOP_ID_E enAOP, HI_S32 s32Balance)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    if (s32Balance >= 0)
    {
        AopReg->AOP_EXT_CTRL.bits.balance_dir = 0;
        AopReg->AOP_EXT_CTRL.bits.balance_val = s32Balance;
    }
    else
    {
        AopReg->AOP_EXT_CTRL.bits.balance_dir = 1;
        AopReg->AOP_EXT_CTRL.bits.balance_val = -s32Balance;
    }
}

HI_VOID iHAL_AOE_AOP_GetRptrAndWptrRegAddr(AOE_AOP_ID_E enAOP, HI_VOID** ppu32WptrReg, HI_VOID** ppu32RptrReg)
{
    *ppu32WptrReg = (HI_VOID*)(&(g_pAopReg[enAOP]->AOP_BUF_WPTR));
    *ppu32RptrReg = (HI_VOID*)(&(g_pAopReg[enAOP]->AOP_BUF_RPTR));
}

HI_S32 iHAL_AOE_AOP_SetAttr(AOE_AOP_ID_E enAOP, AOE_AOP_CHN_ATTR_S* pstAttr)
{
    HI_U32 Rate, BitDepth, Ch, u32DataFormat;
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    //set  AOP_BUFF_ATTR
    if (HI_TRUE == g_bSwAoeFlag)
    {
        ADSP_WriteAddr(pstAttr->stRbfOutAttr.stRbfAttr.tBufVirAddr, AopReg->AOP_BUF_ADDR);
    }
    else
    {
        HI_PHYS_ADDR_T u32DspRemapAddr = pstAttr->stRbfOutAttr.stRbfAttr.tBufPhyAddr;
#if defined (DSP_DDR_DMAREMAP_SUPPORT)
        if ((u32DspRemapAddr >= DSP_DDR_DMAREMAP_BEG_ADDR) && (u32DspRemapAddr < DSP_DDR_DMAREMAP_END_ADDR))
        {
            u32DspRemapAddr += DSP_DDR_DMAREMAP_MAP_ADDR;
        }
#endif

        ADSP_WriteAddr(u32DspRemapAddr, AopReg->AOP_BUF_ADDR);
    }

    if (pstAttr->stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag)
    {
        if (HI_TRUE == g_bSwAoeFlag)
        {
            ADSP_WriteAddr(pstAttr->stRbfOutAttr.stRbfAttr.tBufVirWptr, AopReg->AOP_BUF_WPTR);
            ADSP_WriteAddr(pstAttr->stRbfOutAttr.stRbfAttr.tBufVirRptr, AopReg->AOP_BUF_RPTR);
        }
        else
        {
            HI_PHYS_ADDR_T u32DspRemapAddr;
            u32DspRemapAddr = pstAttr->stRbfOutAttr.stRbfAttr.tBufPhyWptr;
#if defined (DSP_DDR_DMAREMAP_SUPPORT)
            if ((u32DspRemapAddr >= DSP_DDR_DMAREMAP_BEG_ADDR) && (u32DspRemapAddr < DSP_DDR_DMAREMAP_END_ADDR))
            {
                u32DspRemapAddr += DSP_DDR_DMAREMAP_MAP_ADDR;
            }
#endif

            ADSP_WriteAddr(u32DspRemapAddr, AopReg->AOP_BUF_WPTR);

            u32DspRemapAddr = pstAttr->stRbfOutAttr.stRbfAttr.tBufPhyRptr;
#if defined (DSP_DDR_DMAREMAP_SUPPORT)
            if ((u32DspRemapAddr >= DSP_DDR_DMAREMAP_BEG_ADDR) && (u32DspRemapAddr < DSP_DDR_DMAREMAP_END_ADDR))
            {
                u32DspRemapAddr += DSP_DDR_DMAREMAP_MAP_ADDR;

                //HI_ERR_AIAO("u32DspRemapAddr=0x%.8x,u32BufPhyRptr=0x%.8x\n",u32DspRemapAddr,pstAttr->stRbfOutAttr.stRbfAttr.u32BufPhyRptr);
            }
#endif

            ADSP_WriteAddr(u32DspRemapAddr, AopReg->AOP_BUF_RPTR);
        }
    }
    else
    {
        AopReg->AOP_BUF_WPTR.ADDR_LOW = 0;
        AopReg->AOP_BUF_WPTR.ADDR_HIGH = 0;
        AopReg->AOP_BUF_RPTR.ADDR_LOW = 0;
        AopReg->AOP_BUF_RPTR.ADDR_HIGH = 0;
    }

    AopReg->AOP_BUF_SIZE.bits.buff_size = pstAttr->stRbfOutAttr.stRbfAttr.u32BufSize;
    AopReg->AOP_BUF_SIZE.bits.buff_flag = pstAttr->stRbfOutAttr.stRbfAttr.u32BufWptrRptrFlag; /* u32BufWptrRptrFlag */

    //set fifo attr
    u32DataFormat = pstAttr->stRbfOutAttr.u32BufDataFormat;
    AopReg->AOP_BUFF_ATTR.bits.buf_format =
        (0 == u32DataFormat) ? AOE_FORMAT_LPCM : ((u32DataFormat & 0xff00) ? AOE_FORMAT_IEC61937_COMP : AOE_FORMAT_IEC61937);
    AoeRegBitDepth(pstAttr->stRbfOutAttr.u32BufBitPerSample, &BitDepth);
    AopReg->AOP_BUFF_ATTR.bits.buf_precision = BitDepth;
    AoeRegChannels(pstAttr->stRbfOutAttr.u32BufChannels, &Ch);
    AopReg->AOP_BUFF_ATTR.bits.buf_ch = Ch;
    AoeRegSampelRate(pstAttr->stRbfOutAttr.u32BufSampleRate, &Rate);
    AopReg->AOP_BUFF_ATTR.bits.buf_fs = Rate;

    AopReg->AOP_BUFF_ATTR.bits.buf_priority = ((HI_TRUE == pstAttr->bRbfHwPriority) ? 1 : 0);

    AopReg->AOP_CTRL.bits.cast_flag = ((HI_TRUE == pstAttr->bIsCast) ? 1 : 0);
    AopReg->AOP_CTRL.bits.ena_add_mute  = ((HI_TRUE == pstAttr->bAddMute) ? 1 : 0);  //only for cast aop
    AopReg->AOP_BUFF_ATTR.bits.buf_latency = pstAttr->stRbfOutAttr.u32BufLatencyThdMs;

    return HI_SUCCESS;
}

AOE_AOP_CMD_RET_E iHAL_AOE_AOP_AckCmd(AOE_AOP_ID_E enAOP)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    volatile HI_S32 loop = 0;

    for (loop = 0; loop < 100; loop++)
    {
        if (AopReg->AOP_CTRL.bits.cmd_done)
        {
            return (AOE_AOP_CMD_RET_E)AopReg->AOP_CTRL.bits.cmd_return_value;
        }
        msleep(1);
    }
    return AOE_AOP_CMD_ERR_TIMEOUT;
}

HI_S32 iHAL_AOE_AOP_SetCmd(AOE_AOP_ID_E enAOP, AOE_AOP_CMD_E newcmd)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    AOE_AOP_CMD_RET_E Ack;

    switch (newcmd)
    {
        case AOE_AOP_CMD_START:
        case AOE_AOP_CMD_STOP:
            AopReg->AOP_CTRL.bits.cmd = newcmd;
            break;

        default:
            return HI_SUCCESS;
    }

    AopReg->AOP_CTRL.bits.cmd_done = 0;
    Ack = iHAL_AOE_AOP_AckCmd(enAOP);
    if (AOE_AOP_CMD_DONE != Ack)
    {
        HI_ERR_AO("AOP(%d) SetCmd(0x%x) failed(0x%x)\n", enAOP, newcmd, Ack);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_GetStatus(AOE_AOP_ID_E enAOP, HI_VOID* pstStatus)
{
    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_AOP_Destroy(AOE_AOP_ID_E enAOP)
{
    return;
}

HI_S32 iHAL_AOE_AOP_SetAefBypass(AOE_AOP_ID_E enAOP, HI_BOOL bBypass)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_CTRL.bits.ena_aef_bypass = (HI_TRUE == bBypass) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetLatency(AOE_AOP_ID_E enAOP, HI_U32 u32Latency)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_BUFF_ATTR.bits.buf_latency = u32Latency;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_GetLatency(AOE_AOP_ID_E enAOP, HI_U32* pu32Latency)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    *pu32Latency = AopReg->AOP_BUFF_ATTR.bits.buf_latency;

    return HI_SUCCESS;
}

/* ENGINE function */
AOE_ENGINE_CMD_RET_E iHAL_AOE_ENGINE_AckCmd(AOE_ENGINE_ID_E enEngine)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    volatile HI_S32 loop = 0;

    for (loop = 0; loop < 100; loop++)
    {
        if (MixerReg->ENGINE_CTRL.bits.cmd_done)
        {
            return (AOE_ENGINE_CMD_RET_E)MixerReg->ENGINE_CTRL.bits.cmd_return_value;
        }
        msleep(1);
    }
    return AOE_ENGINE_CMD_ERR_TIMEOUT;
}

HI_S32 iHAL_AOE_ENGINE_SetCmd(AOE_ENGINE_ID_E enEngine, AOE_ENGINE_CMD_E newcmd)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    AOE_ENGINE_CMD_RET_E Ack;

    switch (newcmd)
    {
        case AOE_ENGINE_CMD_START:
        case AOE_ENGINE_CMD_STOP:
            MixerReg->ENGINE_CTRL.bits.cmd = newcmd;
            break;

        default:
            return HI_SUCCESS;
    }

    MixerReg->ENGINE_CTRL.bits.cmd_done = 0;
    Ack = iHAL_AOE_ENGINE_AckCmd(enEngine);
    if (AOE_ENGINE_CMD_DONE != Ack)
    {
        HI_ERR_AO("ENGINE(%d) SetCmd(0x%x) failed(0x%x)\n", enEngine, newcmd, Ack);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetAttr(AOE_ENGINE_ID_E enEngine, AOE_ENGINE_CHN_ATTR_S* pstAttr)
{
    HI_U32 Rate, BitDepth, Ch, u32DataFormat;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegBitDepth(pstAttr->u32BitPerSample, &BitDepth);
    MixerReg->ENGINE_ATTR.bits.precision = BitDepth;
    AoeRegChannels(pstAttr->u32Channels, &Ch);
    MixerReg->ENGINE_ATTR.bits.ch = Ch;
    AoeRegSampelRate(pstAttr->u32SampleRate, &Rate);
    MixerReg->ENGINE_ATTR.bits.fs = Rate;

    u32DataFormat = pstAttr->u32DataFormat;
    MixerReg->ENGINE_ATTR.bits.format =
        (0 == u32DataFormat) ? AOE_FORMAT_LPCM : ((u32DataFormat & 0xff00) ? AOE_FORMAT_IEC61937_COMP : AOE_FORMAT_IEC61937);

#ifdef HI_SND_USER_AEF_SUPPORT
    if (HI_TRUE == g_bSwAoeFlag)
    {
        ADSP_WriteAddr(pstAttr->stAefInBufAttr.tBufVirAddr, MixerReg->ENGINE_AEF_INBUF_ADDR);
    }
    else
    {
        ADSP_WriteAddr(pstAttr->stAefInBufAttr.tBufPhyAddr, MixerReg->ENGINE_AEF_INBUF_ADDR);
    }
    MixerReg->ENGINE_AEF_INBUF_SIZE.bits.periond_size = pstAttr->stAefInBufAttr.u32PeriodSize;
    MixerReg->ENGINE_AEF_INBUF_SIZE.bits.periond_num  = pstAttr->stAefInBufAttr.u32PeriodNum;
    MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_idx  = 0;
    MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_wrap = 0;
    MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_idx   = 0;
    MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_wrap  = 0;

    if (HI_TRUE == g_bSwAoeFlag)
    {
        ADSP_WriteAddr(pstAttr->stAefOutBufAttr.tBufVirAddr, MixerReg->ENGINE_AEF_OUTBUF_ADDR);
    }
    else
    {
        ADSP_WriteAddr(pstAttr->stAefOutBufAttr.tBufPhyAddr, MixerReg->ENGINE_AEF_OUTBUF_ADDR);
    }
    MixerReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_size = pstAttr->stAefOutBufAttr.u32PeriodSize;
    MixerReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_num  = pstAttr->stAefOutBufAttr.u32PeriodNum;
    MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_idx  = 0;
    MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap = 0;
    MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_idx   = 0;
    MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_wrap  = 0;
#endif

    return HI_SUCCESS;
}

#ifdef __DPT__
HI_S32 iHAL_AOE_ENGINE_SetAvcAttr(AOE_ENGINE_ID_E enEngine, HI_UNF_SND_AVC_ATTR_S* pstAvcAttr)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    U_ENGINE_AVC_TIME_ATTR* AvcTimeReg = (U_ENGINE_AVC_TIME_ATTR*)(&MixerReg->ENGINE_AVC_TIME_ATTR);
    U_ENGINE_AVC_LEVEL_ATTR* AvcLevelReg = (U_ENGINE_AVC_LEVEL_ATTR*)(&MixerReg->ENGINE_AVC_LEVEL_ATTR);
    AvcTimeReg->bits.attack_time = pstAvcAttr->u32AttackTime;
    AvcTimeReg->bits.release_time = pstAvcAttr->u32ReleaseTime;
    AvcLevelReg->bits.limiter_level = -(pstAvcAttr->s32LimiterLevel);
    AvcLevelReg->bits.threshold_level = -(pstAvcAttr->s32ThresholdLevel);
    AvcLevelReg->bits.gain = pstAvcAttr->s32Gain;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetAvcEnable(AOE_ENGINE_ID_E enEngine, HI_BOOL bEnable)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    MixerReg->ENGINE_CTRL.bits.ena_avc = (HI_TRUE == bEnable) ? 1 : 0;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetAvcChangeAttr(AOE_ENGINE_ID_E enEngine, HI_BOOL bEnable)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    MixerReg->ENGINE_CTRL.bits.avc_change_attr = (HI_TRUE == bEnable) ? 1 : 0;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetGeqAttr(AOE_ENGINE_ID_E enEngine, HI_UNF_SND_GEQ_ATTR_S* pstGeqAttr)
{
    HI_U32 i;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    U_EQ_BAND_ATTR* GeqReg = (U_EQ_BAND_ATTR*)(&MixerReg->ENGINE_GEQ_BAND1_ATTR);

    MixerReg->ENGINE_ATTR.bits.geq_bandnum = pstGeqAttr->u32BandNum;
    for (i = 0; i < pstGeqAttr->u32BandNum; i++)
    {
        GeqReg->bits.freq = pstGeqAttr->stEqParam[i].u32freq;

        if (pstGeqAttr->stEqParam[i].s32Gain < 0)
        {
            GeqReg->bits.gain_dir = 1;
            GeqReg->bits.gain_value = (-(pstGeqAttr->stEqParam[i].s32Gain)) / 125;
        }
        else
        {
            GeqReg->bits.gain_dir = 0;
            GeqReg->bits.gain_value = (pstGeqAttr->stEqParam[i].s32Gain) / 125;
        }
        GeqReg++;
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetGeqEnable(AOE_ENGINE_ID_E enEngine, HI_BOOL bEnable)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    MixerReg->ENGINE_CTRL.bits.ena_geq = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetGeqGain(AOE_ENGINE_ID_E enEngine, HI_U32 u32Band, HI_S32 s32Gain)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    U_EQ_BAND_ATTR* GeqReg = (U_EQ_BAND_ATTR*)(&MixerReg->ENGINE_GEQ_BAND1_ATTR);

    GeqReg += u32Band;

    if (s32Gain < 0)
    {
        GeqReg->bits.gain_dir = 1;
        GeqReg->bits.gain_value = -(s32Gain) / 125;
    }
    else
    {
        GeqReg->bits.gain_dir = 0;
        GeqReg->bits.gain_value = s32Gain / 125;
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetGeqChangeAttr(AOE_ENGINE_ID_E enEngine, HI_BOOL bEnable)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];
    MixerReg->ENGINE_CTRL.bits.geq_change_attr = (HI_TRUE == bEnable) ? 1 : 0;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetDrcEnable(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_CTRL.bits.ena_drc = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetDrcChangeAttr(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_CTRL.bits.drc_change_attr = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetDrcAttr(AOE_AOP_ID_E enAOP, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    U_AOP_DRC_TIME_ATTR* DrcTimeReg = (U_AOP_DRC_TIME_ATTR*)(&AopReg->AOP_DRC_TIME_ATTR);
    U_AOP_DRC_LEVEL_ATTR* DrcLeveleReg = (U_AOP_DRC_LEVEL_ATTR*)(&AopReg->AOP_DRC_LEVEL_ATTR);

    DrcTimeReg->bits.attack_time = pstDrcAttr->u32AttackTime;
    DrcTimeReg->bits.release_time = pstDrcAttr->u32ReleaseTime;
    DrcLeveleReg->bits.start_level = -(pstDrcAttr->s32Thrhd);
    DrcLeveleReg->bits.targert_level = -(pstDrcAttr->s32Limit);

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetPeqAttr(AOE_AOP_ID_E enAOP, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr)
{
    HI_U32 i;
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];
    U_EQ_BAND_ATTR* PeqReg = (U_EQ_BAND_ATTR*)(&AopReg->AOP_PEQ_BAND1_ATTR);

    AopReg->AOP_CTRL.bits.peq_bandnum = pstPeqAttr->u32BandNum;
    for (i = 0; i < pstPeqAttr->u32BandNum; i++)
    {
        PeqReg->bits.freq = pstPeqAttr->stEqParam[i].u32freq;
        PeqReg->bits.q_value = pstPeqAttr->stEqParam[i].u32Q;

        if (pstPeqAttr->stEqParam[i].s32Gain < 0)
        {
            PeqReg->bits.gain_dir = 1;
            PeqReg->bits.gain_value = (-(pstPeqAttr->stEqParam[i].s32Gain)) / 125;
        }
        else
        {
            PeqReg->bits.gain_dir = 0;
            PeqReg->bits.gain_value = (pstPeqAttr->stEqParam[i].s32Gain) / 125;
        }

        if (i < 8)
        {
            AopReg->AOP_PEQ_BAND_TYPE1.u32 &= (~(0xf << (4 * i)));
            AopReg->AOP_PEQ_BAND_TYPE1.u32 |= (pstPeqAttr->stEqParam[i].enFltType << (4 * i));
        }
        else
        {
            AopReg->AOP_PEQ_BAND_TYPE2.u32 &= (~(0xf << (4 * (i - 8))));
            AopReg->AOP_PEQ_BAND_TYPE2.u32 |= (pstPeqAttr->stEqParam[i].enFltType << (4 * (i - 8)));
        }

        PeqReg++;
    }

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetPeqEnable(AOE_AOP_ID_E enAop, HI_BOOL bEnable)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAop];

    AopReg->AOP_CTRL.bits.ena_peq = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetPeqChangeAttr(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_CTRL.bits.peq_change_attr = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_AOP_SetADOutputEnable(AOE_AOP_ID_E enAOP, HI_BOOL bEnable)
{
    S_AOP_REGS_TYPE* AopReg = (S_AOP_REGS_TYPE*)g_pAopReg[enAOP];

    AopReg->AOP_CTRL.bits.ena_ad = (HI_TRUE == bEnable) ? 1 : 0;

    return HI_SUCCESS;
}
#endif

HI_S32 iHAL_AOE_ENGINE_AttachAip(AOE_ENGINE_ID_E enEngine, AOE_AIP_ID_E enAIP)
{
    HI_U32 Src;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute((HI_U32)enAIP, &Src);

    MixerReg->ENGINE_MIX_SRC.bits.aip_fifo_ena |= Src;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_DetachAip(AOE_ENGINE_ID_E enEngine, AOE_AIP_ID_E enAIP)
{
    HI_U32 Src;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute((HI_U32)enAIP, &Src);
    if (MixerReg->ENGINE_MIX_SRC.bits.aip_fifo_ena & Src)
    {
        MixerReg->ENGINE_MIX_SRC.bits.aip_fifo_ena &= ~Src;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

HI_S32 iHAL_AOE_ENGINE_AttachAop(AOE_ENGINE_ID_E enEngine, AOE_AOP_ID_E enAOP)
{
    HI_U32 Dst;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute((HI_U32)enAOP, &Dst);

    MixerReg->ENGINE_ROU_DST.bits.aop_buf_ena |= Dst;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_DetachAop(AOE_ENGINE_ID_E enEngine, AOE_AOP_ID_E enAOP)
{
    HI_U32 Dst;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute((HI_U32)enAOP, &Dst);

    if (MixerReg->ENGINE_ROU_DST.bits.aop_buf_ena & Dst)
    {
        MixerReg->ENGINE_ROU_DST.bits.aop_buf_ena &= ~Dst;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}

#ifdef __DPT__
HI_S32 iHAL_AOE_ENGINE_AttachAef(AOE_ENGINE_ID_E enEngine, HI_U32 u32AefId)
{
    HI_U32 Dst;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute(u32AefId, &Dst);

    MixerReg->ENGINE_ATT_AEF.bits.aef_att_ena |= Dst;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_DetachAef(AOE_ENGINE_ID_E enEngine, HI_U32 u32AefId)
{
    HI_U32 Dst;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    AoeRegMixRoute(u32AefId, &Dst);

    if (MixerReg->ENGINE_ATT_AEF.bits.aef_att_ena & Dst)
    {
        MixerReg->ENGINE_ATT_AEF.bits.aef_att_ena &= ~Dst;
        return HI_SUCCESS;
    }
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetAefAttr(AOE_ENGINE_ID_E enEngine, HI_BOOL bMcProcess, HI_U32 u32FrameDelay)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    MixerReg->ENGINE_ATTR.bits.aef_support_mc = (HI_U32)bMcProcess;
    MixerReg->ENGINE_ATTR.bits.aef_delay = u32FrameDelay;
    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetAefInbuf(AOE_ENGINE_ID_E enEngine, AOE_ENGINE_AEFBUF_ATTR_S* pstAefBuf)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    ADSP_WriteAddr(pstAefBuf->tBufVirAddr, MixerReg->ENGINE_AEF_INBUF_ADDR);
    MixerReg->ENGINE_AEF_INBUF_SIZE.bits.periond_size = pstAefBuf->u32PeriodSize;
    MixerReg->ENGINE_AEF_INBUF_SIZE.bits.periond_num  = pstAefBuf->u32PeriodNum;
    MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_idx  = 0;
    MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_wrap = 0;
    MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_idx    = 0;
    MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_wrap   = 0;

    return HI_SUCCESS;
}

HI_S32 iHAL_AOE_ENGINE_SetAefOutbuf(AOE_ENGINE_ID_E enEngine, AOE_ENGINE_AEFBUF_ATTR_S* pstAefBuf)
{
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    ADSP_WriteAddr(pstAefBuf->tBufVirAddr, MixerReg->ENGINE_AEF_OUTBUF_ADDR);
    MixerReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_size = pstAefBuf->u32PeriodSize;
    MixerReg->ENGINE_AEF_OUTBUF_SIZE.bits.periond_num  = pstAefBuf->u32PeriodNum;
    MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_idx  = 0;
    MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap = 0;
    MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_idx    = 0;
    MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_wrap   = 0;

    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_ENGINE_GetAefDelayFrame(AOE_ENGINE_ID_E enEngine, HI_U32* u32DelayFrame)
{
    HI_U32 u32FrmLatency = 0;
    HI_U32 u32WriteWrap  = 0;
    HI_U32 u32WriteIdx   = 0;
    HI_U32 u32ReadIdx    = 0;
    HI_U32 u32ReadWrap   = 0;
    S_MIXER_REGS_TYPE* MixerReg = (S_MIXER_REGS_TYPE*)g_pMixerReg[enEngine];

    u32FrmLatency = MixerReg->ENGINE_ATTR.bits.aef_delay;   //Aef FIFO Delay

    //AEF In Period Buf Frame Latency
    u32WriteWrap = MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_wrap;
    u32WriteIdx  = MixerReg->ENGINE_AEF_INBUF_WRITEIDX.bits.periond_write_idx;
    u32ReadWrap  = MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_wrap;
    u32ReadIdx   = MixerReg->ENGINE_AEF_INBUF_READIDX.bits.periond_read_idx;
    u32FrmLatency += ((u32WriteWrap == u32ReadWrap) ? (u32WriteIdx - u32ReadIdx) : (AOE_AEF_PERIODBUF_NUM - u32ReadIdx + u32WriteIdx));

    //AEF Out Period Buf Frame Latency
    u32WriteWrap = MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_wrap;
    u32WriteIdx  = MixerReg->ENGINE_AEF_OUTBUF_WRITEIDX.bits.periond_write_idx;
    u32ReadWrap  = MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_wrap;
    u32ReadIdx   = MixerReg->ENGINE_AEF_OUTBUF_READIDX.bits.periond_read_idx;
    u32FrmLatency += ((u32WriteWrap == u32ReadWrap) ? (u32WriteIdx - u32ReadIdx) : (AOE_AEF_PERIODBUF_NUM - u32ReadIdx + u32WriteIdx));

    *u32DelayFrame = u32FrmLatency;
}
#endif

HI_S32 iHAL_AOE_ENGINE_GetStatus(AOE_ENGINE_ID_E enEngine, HI_VOID* pstStatus)
{
    return HI_SUCCESS;
}

HI_VOID iHAL_AOE_ENGINE_Destroy(AOE_ENGINE_ID_E enENGINE)
{
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
