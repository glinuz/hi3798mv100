/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:G1AlbldKCUivdSvL3ezVMXi4KnLMonDmVwiaXjIqLTEv8TMsj2VBK4g39dMdkVeeiEN8z6iv
FartdfMSIIW1SnEvTWRQDLSWXY+N+RvdLM2jOYWoVDfxNHcDDWjb1+WvzhXmO+x6HXGfFody
avcJ9BU1ZlNsyJ5aX1KwiM6tFsmKYh0ZDHRrNnLK4XDyQg5rXf7hHnffePWM+3unDGLFlTOk
qg3+MVhueWPHp5YsKGLOB0xb2VP0z/9Zz7SJIbCOC7gkYmX7HvgsvCG2TxDTOA==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司

******************************************************************************
    文 件 名   : product.c
    版 本 号   : 初稿
    作    者   :
    生成日期   :
    最近修改   :
    功能描述   : 产品差异亩酝饨涌谑迪?

 修改历史   :
    1.日    期 : 2016-06-01
    作    者   : y00226912
    修改内容   :

******************************************************************************/

#include "hi_reg_common.h"
#include "product.h"
#include "public.h"
#include "vfmw_osal_ext.h"
#include "vdm_hal_v400r003_local.h"

#ifdef VFMW_VDH_V400R003_SUPPORT
#include "vdm_hal_v400r003_api.h"
#endif

#if defined(ENV_ARMLINUX_KERNEL)
#include "hi_kernel_adapt.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#endif


/*======================================================================*/
/*   全局数据                                                           */
/*======================================================================*/
SINT32 g_ChipMaxWidth  = 8192;   // chip support max width
SINT32 g_ChipMaxHeight = 8192;  // chip support max height

#ifdef VFMW_MODULE_LOWDLY_SUPPORT
UINT8  g_LowDelayCalcStat[MAX_CHAN_NUM];
SINT32 g_LowDelayCalcHandle[MAX_CHAN_NUM];
UINT32 g_LowDelaySeqIndex[MAX_CHAN_NUM];
#endif

#ifdef ENV_SOS_KERNEL
volatile HI_REG_CRG_S  *g_pstRegCrg;
#endif


/*======================================================================*/
/*   内部接口                                                           */
/*======================================================================*/

#ifdef VFMW_MODULE_LOWDLY_SUPPORT
/************************************************************************
    低延迟时间统计
 ************************************************************************/
VOID VDEC_Lowdelay_Event_Time(SINT32 ChanId, HI_LD_Event_ID_E eventId, HI_U32 u32FrameId)
{
    HI_LD_Event_S evt;
    HI_U32 TmpTime = 0;
    HI_DRV_SYS_GetTimeStampMs(&TmpTime);

    evt.evt_id = eventId;
    evt.frame = u32FrameId;
    evt.handle = g_LowDelayCalcHandle[ChanId];
    evt.time = TmpTime;

    if (1 == g_LowDelayCalcStat[ChanId])
    {
        HI_DRV_LD_Notify_Event(&evt);
    }
}
#endif

VOID VDM_SetClockSkip(UINT32 SkipValue)
{
    #if 0
	U_PERI_CRG30 u32TmpValue_30;

	u32TmpValue_30.u32 = g_pstRegCrg->PERI_CRG30.u32;
    u32TmpValue_30.bits.vdhclk_skipcfg = SkipValue;

    /* To reset config, we should set 0 then 1 */
    u32TmpValue_30.bits.vdhclk_loaden = 0x0;
    g_pstRegCrg->PERI_CRG30.u32 = u32TmpValue_30.u32;
    u32TmpValue_30.bits.vdhclk_loaden = 0x1;
    g_pstRegCrg->PERI_CRG30.u32 = u32TmpValue_30.u32;
    #endif

	return;
}

VOID VDM_OpenHardware(SINT32 VdhId)
{
    SINT32 ScdId, D32;

    if (0 == VdhId)
    {
        UINT32 u32TmpValue;
        RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
        ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_vdec_clken = 0x2;
        WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);

        RD_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, VdhId);
        ((VDHREG_RESET *)(&u32TmpValue))->vdh_all_srst_req = 0x0;
        ((VDHREG_RESET *)(&u32TmpValue))->vdh_mfde_srst_req = 0x0;
        WR_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, VdhId);

    	//VDM_SetClockSkip(25);

        vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

        {   //VDH SMMU
            RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
            ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_mmu_clken = 0x2;
            WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
            vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

            //u32VdhSmmu.bits.vdh_smmu_srst_req = 0x0;
            //g_pstRegCrg->PERI_CRG207.u32 = u32VdhSmmu.u32;
            vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

        #ifdef HI_SMMU_SUPPORT
            VDMHAL_SetSmmuPageTableAddr(0);
            VDMHAL_EnableSmmu();
        #endif
        }

    }
    else
    {
        dprint(PRN_FATAL, "VDM_OpenHardware failed, wrong VdhId %d\n", VdhId);
    }

    for (ScdId = 0; ScdId < MAX_SCD_NUM; ScdId++)
    {
        D32 = 0x481a1f7;
        WR_SCDREG(ScdId, REG_EMAR_ID, D32);
    }

    return;
}

VOID VDM_CloseHardware(SINT32 VdhId)
{
    if (0 == VdhId)
    {
        UINT32 u32TmpValue;

        VDMHAL_GlbResetX(VdhId);

        RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
        ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_vdec_clken = 0x0;
        WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);

        vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

        {   //VDH SMMU
            RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);

            if (((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_mmu_clken != 0)
            {
                //u32VdhSmmu.bits.vdh_smmu_srst_req = 0x1;
                //g_pstRegCrg->PERI_CRG207.u32 = u32VdhSmmu.u32;
                vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

                RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
                ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_mmu_clken = 0x0;
                WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, VdhId);
                vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);
            }
        }
    }
    else
    {
        dprint(PRN_FATAL, "VDM_CloseHardware failed, wrong VdhId %d\n", VdhId);
    }

    return;
}

VOID DSP_OpenHardware(SINT32 VdhId)
{
    #if 0
    if (0 == VdhId)
    {
        UINT32 u32TmpValue;
        u32TmpValue.u32 = g_pstRegCrg->PERI_CRG30.u32;
        u32TmpValue.bits.vdhdsp_cken = 0x1;

        g_pstRegCrg->PERI_CRG30.u32 = u32TmpValue.u32;

        vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);
    }
    else
    {
        dprint(PRN_FATAL, "DSP_OpenHardware failed, wrong VdhId %d\n", VdhId);
    }
    #endif

    return;
}

VOID DSP_CloseHardware(SINT32 VdhId)
{
#if 0
    if (0 == VdhId)
    {
        UINT32 u32TmpValue;
        u32TmpValue.u32 = g_pstRegCrg->PERI_CRG30.u32;
        u32TmpValue.bits.vdhdsp_cken = 0x0;

        g_pstRegCrg->PERI_CRG30.u32 = u32TmpValue.u32;

        vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay(30);

    }
    else
    {
        dprint(PRN_FATAL, "DSP_CloseHardware failed, wrong VdhId %d\n", VdhId);
    }
#endif
    return;
}

VOID SCD_OpenHardware(SINT32 ScdId)
{
    if (0 == ScdId)
    {
        UINT32 u32TmpValue;
        RD_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, ScdId);
        ((VDHREG_RESET *)(&u32TmpValue))->vdh_scd_srst_req = 0x0;
        WR_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, ScdId);
    }
    else
    {
        dprint(PRN_FATAL, "SCD_OpenHardware Wrong ScdId %d\n", ScdId);
    }

    return;
}

VOID SCD_CloseHardware(SINT32 ScdId)
{
    if (0 == ScdId)
    {
        UINT32 u32TmpValue;
        RD_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, ScdId);
        ((VDHREG_RESET *)(&u32TmpValue))->vdh_scd_srst_req = 0x1;
        WR_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, ScdId);
    }
    else
    {
        dprint(PRN_FATAL, "SCD_CloseHardware Wrong ScdId %d\n", ScdId);
    }

    return;
}

VOID BPD_OpenHardware(SINT32 BpdId)
{
    UINT32 u32TmpValue;
    RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, BpdId);
    ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_bpd_clken = 0x2;
    WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, BpdId);

    RD_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, BpdId);
    ((VDHREG_RESET *)(&u32TmpValue))->vdh_bpd_srst_req = 0x0;
    WR_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, BpdId);

    return;
}

VOID BPD_CloseHardware(SINT32 BpdId)
{
    UINT32 u32TmpValue;
    RD_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, BpdId);
    ((VDHREG_CLKSEL *)(&u32TmpValue))->vdh_bpd_clken = 0x0;
    WR_VREG(VREG_V400R003_VDHCRG_CLKSEL, u32TmpValue, BpdId);

    RD_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, BpdId);
    ((VDHREG_RESET *)(&u32TmpValue))->vdh_bpd_srst_req = 0x1;
    WR_VREG(VREG_V400R003_VDHCRG_RESET, u32TmpValue, BpdId);

    return;
}

/************************************************************************
    初始化hal层函数指针
 ************************************************************************/
VOID PDT_InitHalInterface(VDMHAL_FUN_PTR_S *pFuncPtr)
{
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(pFuncPtr, 0, sizeof(VDMHAL_FUN_PTR_S));

    pFuncPtr->pfun_VDMDRV_SetClockSkip           = VDM_SetClockSkip;
    pFuncPtr->pfun_VDMDRV_OpenHardware           = VDM_OpenHardware;
    pFuncPtr->pfun_VDMDRV_CloseHardware          = VDM_CloseHardware;
    pFuncPtr->pfun_DSPDRV_OpenHardware           = DSP_OpenHardware;
    pFuncPtr->pfun_DSPDRV_CloseHardware          = DSP_CloseHardware;
    pFuncPtr->pfun_SCDDRV_OpenHardware           = SCD_OpenHardware;
    pFuncPtr->pfun_SCDDRV_CloseHardware          = SCD_CloseHardware;
    pFuncPtr->pfun_BPDDRV_OpenHardware           = BPD_OpenHardware;
    pFuncPtr->pfun_BPDDRV_CloseHardware          = BPD_CloseHardware;

#ifdef VFMW_VDH_V400R003_SUPPORT
    pFuncPtr->pfun_VDMHAL_GetHalMemSize          = VDMHAL_V400R003_GetHalMemSize;
    pFuncPtr->pfun_VDMHAL_CalcFsSize             = VDMHAL_V400R003_CalcFsSize;
    pFuncPtr->pfun_VDMHAL_OpenHAL                = VDMHAL_V400R003_OpenHAL;
    pFuncPtr->pfun_VDMHAL_CloseHAL               = VDMHAL_V400R003_CloseHAL;
    pFuncPtr->pfun_VDMHAL_ArrangeMem             = VDMHAL_V400R003_ArrangeMem;
    pFuncPtr->pfun_VDMHAL_ResetVdm               = VDMHAL_V400R003_ResetVdm;
    pFuncPtr->pfun_VDMHAL_GetRpuSize             = VDMHAL_V400R003_GetRpuSize;
    pFuncPtr->pfun_VDMHAL_SetSmmuPageTableAddr   = VDMHAL_V400R003_SetSmmuPageTableAddr;
    pFuncPtr->pfun_VDMHAL_EnableSmmu             = VDMHAL_V400R003_EnableSmmu;
    pFuncPtr->pfun_VDMHAL_GlbReset               = VDMHAL_V400R003_GlbReset;
    pFuncPtr->pfun_VDMHAL_GlbResetX              = VDMHAL_V400R003_GlbResetX;
    pFuncPtr->pfun_VDMHAL_ClearIntState          = VDMHAL_V400R003_ClearIntState;
    pFuncPtr->pfun_VDMHAL_ClearMMUIntState       = VDMHAL_V400R003_ClearMMUIntState;
    pFuncPtr->pfun_VDMHAL_MaskInt                = VDMHAL_V400R003_MaskInt;
    pFuncPtr->pfun_VDMHAL_EnableInt              = VDMHAL_V400R003_EnableInt;
    pFuncPtr->pfun_VDMHAL_CheckReg               = VDMHAL_V400R003_CheckReg;
    pFuncPtr->pfun_VDMHAL_ReadMMUMask            = VDMHAL_V400R003_ReadMMUMask;
    pFuncPtr->pfun_VDMHAL_WriteMMUMask           = VDMHAL_V400R003_WriteMMUMask;
    pFuncPtr->pfun_VDMHAL_StartHwRepair          = VDMHAL_V400R003_StartHwRepair;
    pFuncPtr->pfun_VDMHAL_StartHwDecode          = VDMHAL_V400R003_StartHwDecode;
    pFuncPtr->pfun_VDMHAL_PrepareDec             = VDMHAL_V400R003_PrepareDec;
    pFuncPtr->pfun_VDMHAL_IsVdmReady             = VDMHAL_V400R003_IsVdmReady;
    pFuncPtr->pfun_VDMHAL_IsVdmRun               = VDMHAL_V400R003_IsVdmRun;
    pFuncPtr->pfun_VDMHAL_PrepareRepair          = VDMHAL_V400R003_PrepareRepair;
    pFuncPtr->pfun_VDMHAL_MakeDecReport          = VDMHAL_V400R003_MakeDecReport;
    pFuncPtr->pfun_VDMHAL_BackupInfo             = VDMHAL_V400R003_BackupInfo;
    pFuncPtr->pfun_VDMHAL_GetCharacter           = VDMHAL_V400R003_GetCharacter;
    pFuncPtr->pfun_VDMHAL_Write1DYuv             = VDMHAL_V400R003_WriteBigTitle1DYuv;
#endif

    return;
}


/*======================================================================*/
/*   外部接口                                                           */
/*======================================================================*/

/************************************************************************
    获取解码器能力集
 ************************************************************************/
VOID PDT_GetVdecCapability(VDEC_CAP_S *pCap)
{
    SINT32 StdCnt = 0;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet(pCap, 0, sizeof(VDEC_CAP_S));

    pCap->s32MaxChanNum = MAX_CHAN_NUM;
    pCap->s32MaxBitRate = 50;
    pCap->s32MaxFrameWidth  = 4096;
    pCap->s32MaxFrameHeight = 2304;
    pCap->s32MaxPixelPerSec = pCap->s32MaxFrameWidth * pCap->s32MaxFrameHeight * 30 * 3 / 2;

    pCap->SupportedStd[StdCnt++] = VFMW_H264;
    pCap->SupportedStd[StdCnt++] = VFMW_MPEG2;
    pCap->SupportedStd[StdCnt++] = VFMW_MPEG4;
    pCap->SupportedStd[StdCnt++] = VFMW_AVS;
#ifdef VFMW_REAL8_SUPPORT
    pCap->SupportedStd[StdCnt++] = VFMW_REAL8;
#endif
#ifdef VFMW_REAL9_SUPPORT
    pCap->SupportedStd[StdCnt++] = VFMW_REAL9;
#endif
    pCap->SupportedStd[StdCnt++] = VFMW_VC1;
    pCap->SupportedStd[StdCnt++] = VFMW_DIVX3;
#ifdef VFMW_H263_SUPPORT
    pCap->SupportedStd[StdCnt++] = VFMW_H263;
    pCap->SupportedStd[StdCnt++] = VFMW_SORENSON;
#endif
    pCap->SupportedStd[StdCnt++] = VFMW_VP6;
    pCap->SupportedStd[StdCnt++] = VFMW_VP6F;
    pCap->SupportedStd[StdCnt++] = VFMW_VP6A;
    pCap->SupportedStd[StdCnt++] = VFMW_VP8;
    pCap->SupportedStd[StdCnt++] = VFMW_MVC;
#ifdef VFMW_HEVC_SUPPORT
    pCap->SupportedStd[StdCnt++] = VFMW_HEVC;
#endif
#ifdef VFMW_VP9_SUPPORT
	pCap->SupportedStd[StdCnt++] = VFMW_VP9;
#endif
    pCap->SupportedStd[StdCnt++] = VFMW_RAW;
    pCap->SupportedStd[StdCnt++] = VFMW_USER;
    pCap->SupportedStd[StdCnt++] = VFMW_END_RESERVED;

    return;
}

/************************************************************************
    产品初始化
 ************************************************************************/
SINT32 PDT_Init(VDMHAL_FUN_PTR_S *pFuncPtr)
{
#ifdef ENV_SOS_KERNEL
    g_pstRegCrg = (HI_REG_CRG_S *)vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelRegisterMap(SYSTEM_REG_PHY_ADDR);
    if (g_pstRegCrg == NULL)
    {
        dprint(PRN_FATAL, "SOS map CRG reg failed!\n");
        return VDEC_ERR;
    }
#endif

    PDT_InitHalInterface(pFuncPtr);

    VDMHAL_GetCharacter();

    return VDEC_OK;
}


