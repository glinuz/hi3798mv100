/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hal_vi.c
* Description:
*
***********************************************************************************/

#include <asm/io.h>
#include "hal_vi.h"
#include "hi_drv_vi.h"
#include "drv_vi_utils.h"

VI_REG_S *pViReg = NULL;

HI_U32 ViRegRead(HI_U32 a)
{
    HI_U32 *reg = NULL;

    reg = (HI_U32 *)(a + (HI_U32)pViReg);
    return (*(reg));
}

HI_VOID ViRegWrite(HI_U32 a, HI_U32 b)
{
    HI_U32 *reg = NULL;

    reg = (HI_U32 *)(a + (HI_U32)pViReg);
    (*(reg)) = b;
}

/**
 *    Work Mode
 *
 */

HI_VOID VI_DRV_SetWorkMode(HI_BOOL bLowDelay, HI_BOOL bInnerClk)
{
    volatile U_WK_MODE WK_MODE;
    volatile HI_U32 u32RegAddr = 0;

    u32RegAddr  = (HI_U32)&(pViReg->WK_MODE.u32) - (HI_U32)pViReg;
    WK_MODE.u32 = ViRegRead(u32RegAddr);
    WK_MODE.bits.power_mode  = bLowDelay;
    WK_MODE.bits.pt0_ppc_sel = bInnerClk;

    ViRegWrite(u32RegAddr, WK_MODE.u32);

    return;
}

/**
 *    Master
 *
 */
HI_VOID VI_DRV_SetMasterOtd(HI_U32 u32MasterId, HI_U32 u32Otd)
{
    volatile U_AXI_CFG AXI_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_MASTER_NUM <= u32MasterId)
    {
        HI_FATAL_VI("Error Master Id %d!\n", u32MasterId);
        return;
    }

    u32RegAddr  = ((HI_U32)&(pViReg->AXI_CFG.u32) - (HI_U32)pViReg);
    AXI_CFG.u32 = ViRegRead(u32RegAddr);
    AXI_CFG.bits.outstanding = u32Otd;

    ViRegWrite(u32RegAddr, AXI_CFG.u32);
    return;
}

/**
 *    System
 *
 */
HI_U32 VI_DRV_GetVicapInt(HI_VOID)
{
    volatile HI_U32 u32RegAddr = 0;

    u32RegAddr = (HI_U32)&(pViReg->VICAP_INT.u32) - (HI_U32)pViReg;

    return ViRegRead(u32RegAddr);
}


HI_VOID VI_DRV_SetVicapIntMask(HI_U32 int_pt0_en, HI_U32 int_ch0_en)
{
    volatile U_VICAP_INT_MASK VICAP_INT_MASK;
    volatile HI_U32 u32RegAddr = 0;

    u32RegAddr = (HI_U32)&(pViReg->VICAP_INT_MASK.u32) - (HI_U32)pViReg;
    VICAP_INT_MASK.u32 = ViRegRead(u32RegAddr);
    VICAP_INT_MASK.bits.int_pt0_en = int_pt0_en;
    VICAP_INT_MASK.bits.int_ch0_en = int_ch0_en;
    ViRegWrite(u32RegAddr, VICAP_INT_MASK.u32);

    return;
}


HI_VOID VI_DRV_SetApbTimeout(HI_BOOL bEn, HI_U32 timeout)
{
    volatile U_APB_TIMEOUT APB_TIMEOUT;
    volatile HI_U32 u32RegAddr = 0;

    u32RegAddr = (HI_U32)&(pViReg->APB_TIMEOUT.u32) - (HI_U32)pViReg;
    APB_TIMEOUT.u32 = ViRegRead(u32RegAddr);
    APB_TIMEOUT.bits.enable  = bEn;
    APB_TIMEOUT.bits.timeout = timeout;
    ViRegWrite(u32RegAddr, APB_TIMEOUT.u32);

    return;
}

/**
 *    Port
 *
 */
HI_VOID VI_DRV_SetPtEn(HI_U32 PtId, HI_BOOL bEn)
{
    volatile U_PT_INTF_MOD PT_INTF_MOD;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_MOD.u32) - (HI_U32)pViReg);
    PT_INTF_MOD.u32 = ViRegRead(u32RegAddr);
    PT_INTF_MOD.bits.enable = bEn;
    ViRegWrite(u32RegAddr, PT_INTF_MOD.u32);

    return;
}

HI_U32 VI_DRV_GetPtStatus(HI_U32 PtId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return 0;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_STATUS.u32) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_U32 VI_DRV_GetPtBT656Status(HI_U32 PtId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return 0;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_BT656_STATUS.u32) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_VOID VI_DRV_SetPtSize(HI_U32 PtId, HI_U32 Width, HI_U32 Height)
{
    volatile U_PT_SIZE PT_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr  = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_SIZE.u32) - (HI_U32)pViReg);
    PT_SIZE.u32 = ViRegRead(u32RegAddr);
    PT_SIZE.bits.width  = Width;
    PT_SIZE.bits.height = Height;
    ViRegWrite(u32RegAddr, PT_SIZE.u32);

    return;
}

HI_VOID VI_DRV_SetPtIntMask(HI_U32 PtId, HI_U32 fstart_en, HI_U32 width_err_en, HI_U32 height_err_en)
{
    volatile U_PT_INT_MASK PT_INT_MASK;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INT_MASK.u32) - (HI_U32)pViReg);
    PT_INT_MASK.u32 = ViRegRead(u32RegAddr);
    PT_INT_MASK.bits.fstart_en = fstart_en;
    PT_INT_MASK.bits.width_err_en  = width_err_en;
    PT_INT_MASK.bits.height_err_en = height_err_en;
    ViRegWrite(u32RegAddr, PT_INT_MASK.u32);

    return;
}

HI_VOID VI_DRV_ClrPtIntStatus(HI_U32 PtId, HI_U32 u32ClrInt)
{
    volatile U_PT_INT PT_INT;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INT.u32) - (HI_U32)pViReg);
    PT_INT.u32 = ViRegRead(u32RegAddr);
    PT_INT.u32 = PT_INT.u32 & u32ClrInt;
    ViRegWrite(u32RegAddr, PT_INT.u32);

    return;
}

HI_U32 VI_DRV_GetPtIntStatus(HI_U32 PtId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return 0;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INT.u32) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_VOID VI_DRV_SetPtTimingMode(HI_U32 PtId, HI_U32 u32Mode)
{
    volatile U_PT_INTF_MOD PT_INTF_MOD;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_MOD.u32) - (HI_U32)pViReg);
    PT_INTF_MOD.u32 = ViRegRead(u32RegAddr);
    PT_INTF_MOD.bits.mode = u32Mode;
    ViRegWrite(u32RegAddr, PT_INTF_MOD.u32);

    return;
}

HI_VOID VI_DRV_SetPtMask(HI_U32 PtId, VI_DRV_PORT_MSK * pstMsk)
{
    volatile U_PT_OFFSET0 PT_OFFSET0;
    volatile U_PT_OFFSET1 PT_OFFSET1;
    volatile U_PT_OFFSET2 PT_OFFSET2;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_OFFSET0) - (HI_U32)pViReg);
    PT_OFFSET0.u32 = ViRegRead(u32RegAddr);
    PT_OFFSET0.bits.mask = pstMsk->u32RMsk;
    PT_OFFSET0.bits.rev = pstMsk->u32RRev;
    PT_OFFSET0.bits.offset = pstMsk->u32ROff;
    ViRegWrite(u32RegAddr, PT_OFFSET0.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_OFFSET1) - (HI_U32)pViReg);
    PT_OFFSET1.u32 = ViRegRead(u32RegAddr);
    PT_OFFSET1.bits.mask = pstMsk->u32GMsk;
    PT_OFFSET1.bits.rev = pstMsk->u32GRev;
    PT_OFFSET1.bits.offset = pstMsk->u32GOff;
    ViRegWrite(u32RegAddr, PT_OFFSET1.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_OFFSET2) - (HI_U32)pViReg);
    PT_OFFSET2.u32 = ViRegRead(u32RegAddr);
    PT_OFFSET2.bits.mask = pstMsk->u32BMsk;
    PT_OFFSET2.bits.rev = pstMsk->u32BRev;
    PT_OFFSET2.bits.offset = pstMsk->u32BOff;
    ViRegWrite(u32RegAddr, PT_OFFSET2.u32);

    return;
}

HI_VOID VI_DRV_SetPtGenTiming(HI_U32 PtId, HI_BOOL bEn, HI_BOOL bVsyncMode, HI_BOOL bHsyncMode)
{
    volatile U_PT_GEN_TIMING_CFG PT_GEN_TIMING_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_GEN_TIMING_CFG.u32) - (HI_U32)pViReg);
    PT_GEN_TIMING_CFG.u32 = ViRegRead(u32RegAddr);
    PT_GEN_TIMING_CFG.bits.enable = bEn;
    PT_GEN_TIMING_CFG.bits.vsync_mode = bVsyncMode;
    PT_GEN_TIMING_CFG.bits.hsync_mode = bHsyncMode;
    ViRegWrite(u32RegAddr, PT_GEN_TIMING_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetPtGenData(HI_U32 PtId, VI_DRV_PORT_GEN_DATA * pstgendat)
{
    volatile U_PT_GEN_DATA_CFG PT_GEN_DATA_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_GEN_DATA_CFG.u32) - (HI_U32)pViReg);
    PT_GEN_DATA_CFG.u32 = ViRegRead(u32RegAddr);
    PT_GEN_DATA_CFG.bits.enable = pstgendat->enable;
    PT_GEN_DATA_CFG.bits.data2_move  = pstgendat->data_move[2];
    PT_GEN_DATA_CFG.bits.data1_move  = pstgendat->data_move[1];
    PT_GEN_DATA_CFG.bits.data0_move  = pstgendat->data_move[0];
    PT_GEN_DATA_CFG.bits.vsync_reset = pstgendat->vsync_reset;
    PT_GEN_DATA_CFG.bits.hsync_reset = pstgendat->hsync_reset;
    PT_GEN_DATA_CFG.bits.vsync_move = pstgendat->vsync_move;
    PT_GEN_DATA_CFG.bits.hsync_move = pstgendat->hsync_move;
    PT_GEN_DATA_CFG.bits.de_move = pstgendat->de_move;
    ViRegWrite(u32RegAddr, PT_GEN_DATA_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetPtGenDataCoef(HI_U32 PtId, VI_DRV_PORT_GEN_DATA_COEF * pstgendatcoef)
{
    volatile U_PT_GEN_DATA_COEF PT_GEN_DATA_COEF;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_GEN_DATA_COEF.u32) - (HI_U32)pViReg);
    PT_GEN_DATA_COEF.u32 = ViRegRead(u32RegAddr);
    PT_GEN_DATA_COEF.bits.inc_frame  = pstgendatcoef->inc_frame;
    PT_GEN_DATA_COEF.bits.step_frame = pstgendatcoef->step_frame;
    PT_GEN_DATA_COEF.bits.inc_space  = pstgendatcoef->inc_space;
    PT_GEN_DATA_COEF.bits.step_space = pstgendatcoef->step_space;
    ViRegWrite(u32RegAddr, PT_GEN_DATA_COEF.u32);

    return;
}

HI_VOID VI_DRV_SetPtGenDataInit(HI_U32 PtId, HI_U32 data[3])
{
    volatile U_PT_GEN_DATA_INIT PT_GEN_DATA_INIT;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_GEN_DATA_INIT.u32) - (HI_U32)pViReg);
    PT_GEN_DATA_INIT.u32 = ViRegRead(u32RegAddr);
    PT_GEN_DATA_INIT.bits.data2 = data[2];
    PT_GEN_DATA_INIT.bits.data1 = data[1];
    PT_GEN_DATA_INIT.bits.data0 = data[0];
    ViRegWrite(u32RegAddr, PT_GEN_DATA_INIT.u32);

    return;
}

HI_VOID VI_DRV_SetPtYUV444(HI_U32 PtId, HI_BOOL bEn)
{
    volatile U_PT_YUV444_CFG PT_YUV444_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_YUV444_CFG.u32) - (HI_U32)pViReg);
    PT_YUV444_CFG.u32 = ViRegRead(u32RegAddr);
    PT_YUV444_CFG.bits.enable = bEn;
    ViRegWrite(u32RegAddr, PT_YUV444_CFG.u32);
}

HI_VOID VI_DRV_SetPtFstartDly(HI_U32 PtId, HI_U32 fstartdly)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_FSTART_DLY) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, fstartdly);
}

HI_VOID VI_DRV_SetPtIntf(HI_U32 PtId, VI_DRV_TIMING_BLANK * pstIntfCfg)
{
    volatile U_PT_INTF_HFB PT_INTF_HFB;
    volatile U_PT_INTF_HBB PT_INTF_HBB;
    volatile U_PT_INTF_VFB PT_INTF_VFB;
    volatile U_PT_INTF_VACT PT_INTF_VACT;
    volatile U_PT_INTF_VBB PT_INTF_VBB;
    volatile U_PT_INTF_VBFB PT_INTF_VBFB;
    volatile U_PT_INTF_VBACT PT_INTF_VBACT;
    volatile U_PT_INTF_VBBB PT_INTF_VBBB;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_HFB.u32) - (HI_U32)pViReg);
    PT_INTF_HFB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_HFB.bits.hfb = pstIntfCfg->u32HsyncHfb;
    ViRegWrite(u32RegAddr, PT_INTF_HFB.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_HACT) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, pstIntfCfg->u32HsyncAct);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_HBB.u32) - (HI_U32)pViReg);
    PT_INTF_HBB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_HBB.bits.hbb = pstIntfCfg->u32HsyncHbb;
    ViRegWrite(u32RegAddr, PT_INTF_HBB.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VFB.u32) - (HI_U32)pViReg);
    PT_INTF_VFB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VFB.bits.vfb = pstIntfCfg->u32VsyncOfb;
    ViRegWrite(u32RegAddr, PT_INTF_VFB.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VACT.u32) - (HI_U32)pViReg);
    PT_INTF_VACT.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VACT.bits.vact = pstIntfCfg->u32VsyncOAct;
    ViRegWrite(u32RegAddr, PT_INTF_VACT.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VBB.u32) - (HI_U32)pViReg);
    PT_INTF_VBB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VBB.bits.vbb = pstIntfCfg->u32VsyncObb;
    ViRegWrite(u32RegAddr, PT_INTF_VBB.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VBFB.u32) - (HI_U32)pViReg);
    PT_INTF_VBFB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VBFB.bits.vbfb = pstIntfCfg->u32VsyncEfb;
    ViRegWrite(u32RegAddr, PT_INTF_VBFB.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VBACT.u32) - (HI_U32)pViReg);
    PT_INTF_VBACT.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VBACT.bits.vbact = pstIntfCfg->u32VsyncEAct;
    ViRegWrite(u32RegAddr, PT_INTF_VBACT.u32);

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_INTF_VBBB.u32) - (HI_U32)pViReg);
    PT_INTF_VBBB.u32 = ViRegRead(u32RegAddr);
    PT_INTF_VBBB.bits.vbbb = pstIntfCfg->u32VsyncEbb;
    ViRegWrite(u32RegAddr, PT_INTF_VBBB.u32);

    return;
}

HI_VOID VI_DRV_SetPtTimingBT656(HI_U32 PtId, VI_DRV_BT656_CFG * pst656Cfg)
{
    volatile U_PT_BT656_CFG PT_BT656_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_BT656_CFG.u32) - (HI_U32)pViReg);
    PT_BT656_CFG.u32 = ViRegRead(u32RegAddr);
    PT_BT656_CFG.bits.enable = pst656Cfg->bEn;
    PT_BT656_CFG.bits.field_inv = pst656Cfg->bFieldInv;
    PT_BT656_CFG.bits.vsync_inv = pst656Cfg->bVsyncInv;
    PT_BT656_CFG.bits.hsync_inv = pst656Cfg->bHsyncInv;
    PT_BT656_CFG.bits.mode = pst656Cfg->u32Mode;
    ViRegWrite(u32RegAddr, PT_BT656_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetPtUnifyTiming(HI_U32 PtId, VI_DRV_PORT_UNIFY_TIMING * pstunitiming)
{
    volatile U_PT_UNIFY_TIMING_CFG PT_UNIFY_TIMING_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_UNIFY_TIMING_CFG.u32) - (HI_U32)pViReg);
    PT_UNIFY_TIMING_CFG.u32 = ViRegRead(u32RegAddr);
    PT_UNIFY_TIMING_CFG.bits.field_inv  = pstunitiming->field_inv;
    PT_UNIFY_TIMING_CFG.bits.field_sel  = pstunitiming->field_sel;
    PT_UNIFY_TIMING_CFG.bits.vsync_mode = pstunitiming->vsync_mode;
    PT_UNIFY_TIMING_CFG.bits.vsync_inv  = pstunitiming->vsync_inv;
    PT_UNIFY_TIMING_CFG.bits.vsync_sel  = pstunitiming->vsync_sel;
    PT_UNIFY_TIMING_CFG.bits.hsync_mode = pstunitiming->hsync_mode;
    PT_UNIFY_TIMING_CFG.bits.hsync_and = pstunitiming->hsync_and;
    PT_UNIFY_TIMING_CFG.bits.hsync_inv = pstunitiming->hsync_inv;
    PT_UNIFY_TIMING_CFG.bits.hsync_sel = pstunitiming->hsync_sel;
    PT_UNIFY_TIMING_CFG.bits.de_inv = pstunitiming->de_inv;
    PT_UNIFY_TIMING_CFG.bits.de_sel = pstunitiming->de_sel;

    ViRegWrite(u32RegAddr, PT_UNIFY_TIMING_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetPtUnifyData(HI_U32 PtId, VI_DRV_PORT_UNIFY_DATA * pstunidate)
{
    volatile U_PT_UNIFY_DATA_CFG PT_UNIFY_DATA_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_PT_NUM <= PtId)
    {
        HI_FATAL_VI("Error Port Id %d!\n", PtId);
        return;
    }

    u32RegAddr = VI_PT_DIFF * PtId + ((HI_U32)&(pViReg->PT_UNIFY_DATA_CFG.u32) - (HI_U32)pViReg);
    PT_UNIFY_DATA_CFG.u32 = ViRegRead(u32RegAddr);
    PT_UNIFY_DATA_CFG.bits.enable   = pstunidate->enable;
    PT_UNIFY_DATA_CFG.bits.uv_seq   = pstunidate->uv_seq;
    PT_UNIFY_DATA_CFG.bits.yc_seq   = pstunidate->yc_seq;
    PT_UNIFY_DATA_CFG.bits.comp_num = pstunidate->comp_num;
    ViRegWrite(u32RegAddr, PT_UNIFY_DATA_CFG.u32);

    return;
}

/**
 *    Channel
 *
 */
HI_VOID VI_DRV_SetChEn(HI_U32 ChId, HI_BOOL bEn)
{
    volatile U_CH_CTRL CH_CTRL;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr  = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CTRL.u32) - (HI_U32)pViReg);
    CH_CTRL.u32 = ViRegRead(u32RegAddr);
    CH_CTRL.bits.enable = bEn;
    ViRegWrite(u32RegAddr, CH_CTRL.u32);

    return;
}

HI_VOID VI_DRV_SetChRegNewer(HI_U32 ChId)
{
    volatile U_CH_REG_NEWER CH_REG_NEWER;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_REG_NEWER.u32) - (HI_U32)pViReg);
    CH_REG_NEWER.bits.reg_newer = 1;
    ViRegWrite(u32RegAddr, CH_REG_NEWER.u32);

    return;
}

HI_VOID VI_DRV_SetChIntMask(HI_U32 ChId, HI_U32 u32IntMask)
{
    volatile U_CH_INT_MASK CH_INT_MASK;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_INT_MASK.u32) - (HI_U32)pViReg);
    CH_INT_MASK.u32 = u32IntMask;
    ViRegWrite(u32RegAddr, CH_INT_MASK.u32);

    return;
}

HI_VOID VI_DRV_ClrChIntStatus(HI_U32 ChId, HI_U32 u32ClrInt)
{
    volatile U_CH_INT CH_INT;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_INT.u32) - (HI_U32)pViReg);
    CH_INT.u32 = ViRegRead(u32RegAddr);
    CH_INT.u32 = CH_INT.u32 & u32ClrInt;
    ViRegWrite(u32RegAddr, CH_INT.u32);

    return;
}

HI_U32 VI_DRV_GetChIntStatus(HI_U32 ChId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return 0;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_INT.u32) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_VOID VI_DRV_SetChAdapter(HI_U32 ChId, HI_BOOL bVsyncMode, HI_BOOL bHsyncMode)
{
    volatile U_CH_ADAPTER_CFG CH_ADAPTER_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_ADAPTER_CFG.u32) - (HI_U32)pViReg);
    CH_ADAPTER_CFG.u32 = ViRegRead(u32RegAddr);
    CH_ADAPTER_CFG.bits.vsync_mode = bVsyncMode;
    CH_ADAPTER_CFG.bits.hsync_mode = bHsyncMode;
    ViRegWrite(u32RegAddr, CH_ADAPTER_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChCropEn(HI_U32 ChId, HI_BOOL n0_en, HI_BOOL n1_en)
{
    volatile U_CH_CROP_CFG CH_CROP_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP_CFG.u32) - (HI_U32)pViReg);
    CH_CROP_CFG.u32 = ViRegRead(u32RegAddr);
    CH_CROP_CFG.bits.n0_en = n0_en;
    CH_CROP_CFG.bits.n1_en = n1_en;
    ViRegWrite(u32RegAddr, CH_CROP_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChCrop(HI_U32 ChId, HI_BOOL cropzoom, RECT_S *pstCapRect)
{
    volatile U_CH_CROP0_START CH_CROP0_START;
    volatile U_CH_CROP0_SIZE CH_CROP0_SIZE;
    volatile U_CH_CROP1_START CH_CROP1_START;
    volatile U_CH_CROP1_SIZE CH_CROP1_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    if (cropzoom == HI_FALSE)
    {
        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP0_START.u32) - (HI_U32)pViReg);
        CH_CROP0_START.u32 = ViRegRead(u32RegAddr);
        CH_CROP0_START.bits.x_start = pstCapRect->s32X;
        CH_CROP0_START.bits.y_start = pstCapRect->s32Y;
        ViRegWrite(u32RegAddr, CH_CROP0_START.u32);

        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP0_SIZE.u32) - (HI_U32)pViReg);
        CH_CROP0_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_CROP0_SIZE.bits.width  = pstCapRect->u32Width - 1;
        CH_CROP0_SIZE.bits.height = pstCapRect->u32Height - 1;
        ViRegWrite(u32RegAddr, CH_CROP0_SIZE.u32);
    }
    else
    {
        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP1_START.u32) - (HI_U32)pViReg);
        CH_CROP1_START.u32 = ViRegRead(u32RegAddr);
        CH_CROP1_START.bits.x_start = pstCapRect->s32X;
        CH_CROP1_START.bits.y_start = pstCapRect->s32Y;
        ViRegWrite(u32RegAddr, CH_CROP1_START.u32);

        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP1_SIZE.u32) - (HI_U32)pViReg);
        CH_CROP1_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_CROP1_SIZE.bits.width  = pstCapRect->u32Width - 1;
        CH_CROP1_SIZE.bits.height = pstCapRect->u32Height - 1;
        ViRegWrite(u32RegAddr, CH_CROP1_SIZE.u32);
    }

    return;
}

HI_VOID VI_DRV_SetChCropWin(HI_U32 ChId, HI_U32 width, HI_U32 height)
{
    volatile U_CH_CROP_WIN CH_CROP_WIN;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CROP_WIN.u32) - (HI_U32)pViReg);
    CH_CROP_WIN.u32 = ViRegRead(u32RegAddr);
    CH_CROP_WIN.bits.width  = width - 1;
    CH_CROP_WIN.bits.height = height - 1;
    ViRegWrite(u32RegAddr, CH_CROP_WIN.u32);
}

HI_VOID VI_DRV_SetChSkip(HI_U32 ChId, HI_U32 u32YSkip, HI_U32 u32CSkip)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_SKIP_Y_CFG) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32YSkip);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_SKIP_C_CFG) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32CSkip);

    return;
}

HI_VOID VI_DRV_SetChSkipYWin(HI_U32 ChId, HI_U32 width, HI_U32 height)
{
    volatile U_CH_SKIP_Y_WIN CH_SKIP_Y_WIN;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_SKIP_Y_WIN) - (HI_U32)pViReg);
    CH_SKIP_Y_WIN.u32 = ViRegRead(u32RegAddr);
    CH_SKIP_Y_WIN.bits.width  = width - 1;
    CH_SKIP_Y_WIN.bits.height = height - 1;
    ViRegWrite(u32RegAddr, CH_SKIP_Y_WIN.u32);

    return;
}

HI_VOID VI_DRV_SetChSkipCWin(HI_U32 ChId, HI_U32 width, HI_U32 height)
{
    volatile U_CH_SKIP_C_WIN CH_SKIP_C_WIN;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_SKIP_C_WIN) - (HI_U32)pViReg);
    CH_SKIP_C_WIN.u32 = ViRegRead(u32RegAddr);
    CH_SKIP_C_WIN.bits.width  = width - 1;
    CH_SKIP_C_WIN.bits.height = height - 1;
    ViRegWrite(u32RegAddr, CH_SKIP_C_WIN.u32);

    return;
}

HI_VOID VI_DRV_SetChCoefUpda(HI_U32 ChId, HI_U32 lhcoef_update, HI_U32 chcoef_update)
{
    volatile U_CH_COEF_UPDATE CH_COEF_UPDATE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_COEF_UPDATE) - (HI_U32)pViReg);
    CH_COEF_UPDATE.u32 = ViRegRead(u32RegAddr);
    CH_COEF_UPDATE.bits.lhcoef_update = lhcoef_update;
    CH_COEF_UPDATE.bits.chcoef_update = chcoef_update;
    ViRegWrite(u32RegAddr, CH_COEF_UPDATE.u32);

    return;
}

HI_VOID VI_DRV_SetChCoefRsel(HI_U32 ChId, HI_U32 lhcoef_read_sel, HI_U32 chcoef_read_sel)
{
    volatile U_CH_COEF_RSEL CH_COEF_RSEL;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_COEF_RSEL) - (HI_U32)pViReg);
    CH_COEF_RSEL.u32 = ViRegRead(u32RegAddr);
    CH_COEF_RSEL.bits.lhcoef_read_sel = lhcoef_read_sel;
    CH_COEF_RSEL.bits.chcoef_read_sel = chcoef_read_sel;
    ViRegWrite(u32RegAddr, CH_COEF_RSEL.u32);

    return;
}

HI_VOID VI_DRV_SetChHblankWidth(HI_U32 ChId, HI_U32 hblank_width)
{
    volatile U_CH_HBLANK_WIDTH CH_HBLANK_WIDTH;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_HBLANK_WIDTH) - (HI_U32)pViReg);
    CH_HBLANK_WIDTH.u32 = ViRegRead(u32RegAddr);
    CH_HBLANK_WIDTH.bits.hblank_width = hblank_width;
    ViRegWrite(u32RegAddr, CH_HBLANK_WIDTH.u32);

    return;
}

HI_VOID VI_DRV_SetChLhFirSph(HI_U32 ChId, VI_DRV_CHN_LHFIR_SPH * pstlhfir)
{
    volatile U_CH_LHFIR_SPH CH_LHFIR_SPH;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LHFIR_SPH) - (HI_U32)pViReg);
    CH_LHFIR_SPH.u32 = ViRegRead(u32RegAddr);
    CH_LHFIR_SPH.bits.hlratio  = pstlhfir->hlratio;
    CH_LHFIR_SPH.bits.hlfir_en = pstlhfir->hlfir_en;
    CH_LHFIR_SPH.bits.hlmid_en = pstlhfir->hlmid_en;
    CH_LHFIR_SPH.bits.hlmsc_en = pstlhfir->hlmsc_en;
    ViRegWrite(u32RegAddr, CH_LHFIR_SPH.u32);

    return;
}

HI_VOID VI_DRV_SetChChFirSph(HI_U32 ChId, VI_DRV_CHN_CHFIR_SPH * pstchfir)
{
    volatile U_CH_CHFIR_SPH CH_CHFIR_SPH;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CHFIR_SPH) - (HI_U32)pViReg);
    CH_CHFIR_SPH.u32 = ViRegRead(u32RegAddr);
    CH_CHFIR_SPH.bits.hchratio  = pstchfir->hchratio;
    CH_CHFIR_SPH.bits.hchfir_en = pstchfir->hchfir_en;
    CH_CHFIR_SPH.bits.hchmid_en = pstchfir->hchmid_en;
    CH_CHFIR_SPH.bits.hchmsc_en = pstchfir->hchmsc_en;
    ViRegWrite(u32RegAddr, CH_CHFIR_SPH.u32);

    return;
}

HI_VOID VI_DRV_SetChLhFirOffset(HI_U32 ChId, HI_S32 hluma_offset)
{
    volatile U_CH_LHFIR_OFFSET CH_LHFIR_OFFSET;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LHFIR_OFFSET.u32) - (HI_U32)pViReg);
    CH_LHFIR_OFFSET.u32 = ViRegRead(u32RegAddr);

    /* complement */
    CH_LHFIR_OFFSET.bits.hluma_offset = hluma_offset;
    ViRegWrite(u32RegAddr, CH_LHFIR_OFFSET.u32);

    return;
}

HI_VOID VI_DRV_SetChChFirOffset(HI_U32 ChId, HI_S32 hchroma_offset)
{
    volatile U_CH_CHFIR_OFFSET CH_CHFIR_OFFSET;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CHFIR_OFFSET.u32) - (HI_U32)pViReg);
    CH_CHFIR_OFFSET.u32 = ViRegRead(u32RegAddr);

    if (0 > hchroma_offset)
    {
        hchroma_offset *= -1;
        CH_CHFIR_OFFSET.bits.hchroma_offset = (((~hchroma_offset) + 1) & 0x1FFFF);
    }
    else
    {
        /* complement */
        CH_CHFIR_OFFSET.bits.hchroma_offset = hchroma_offset;
    }

    ViRegWrite(u32RegAddr, CH_CHFIR_OFFSET.u32);
}

HI_VOID VI_DRV_SetChLFirInSize(HI_U32 ChId, HI_U32 width)
{
    volatile U_CH_LFIR_IN_SIZE CH_LFIR_IN_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LFIR_IN_SIZE) - (HI_U32)pViReg);
    CH_LFIR_IN_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_LFIR_IN_SIZE.bits.width = width - 1;
    ViRegWrite(u32RegAddr, CH_LFIR_IN_SIZE.u32);

    return;
}

HI_VOID VI_DRV_SetChCFirInSize(HI_U32 ChId, HI_U32 width)
{
    volatile U_CH_CFIR_IN_SIZE CH_CFIR_IN_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CFIR_IN_SIZE) - (HI_U32)pViReg);
    CH_CFIR_IN_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_CFIR_IN_SIZE.bits.width = width - 1;
    ViRegWrite(u32RegAddr, CH_CFIR_IN_SIZE.u32);

    return;
}

HI_VOID VI_DRV_SetChLFirOutSize(HI_U32 ChId, HI_U32 width)
{
    volatile U_CH_LFIR_OUT_SIZE CH_LFIR_OUT_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LFIR_OUT_SIZE) - (HI_U32)pViReg);
    CH_LFIR_OUT_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_LFIR_OUT_SIZE.bits.width = width - 1;
    ViRegWrite(u32RegAddr, CH_LFIR_OUT_SIZE.u32);

    return;
}

HI_VOID VI_DRV_SetChCFirOutSize(HI_U32 ChId, HI_U32 width)
{
    volatile U_CH_CFIR_OUT_SIZE CH_CFIR_OUT_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CFIR_OUT_SIZE) - (HI_U32)pViReg);
    CH_CFIR_OUT_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_CFIR_OUT_SIZE.bits.width = width - 1;
    ViRegWrite(u32RegAddr, CH_CFIR_OUT_SIZE.u32);

    return;
}

HI_U32 inline VI_DRV_Conver_FirCoef(HI_S32 s32Value)
{
    HI_U32 temp = 0;

    if (s32Value >= 0)
    {
        return s32Value;
    }
    /* valid bit 0~9, bit9 is sign bit */
    else
    {
        temp = (-1) * s32Value;
        return (((~temp) + 1) & 0x3FF);
    }
}

HI_VOID VI_DRV_SetChLhFirCoef(HI_U32 ChId, HI_U32 phase_offset, const HI_S32 hlCoef[8])
{
    volatile U_CH_LHFIR_COEF0 CH_LHFIR_COEF0;
    volatile U_CH_LHFIR_COEF1 CH_LHFIR_COEF1;
    volatile U_CH_LHFIR_COEF2 CH_LHFIR_COEF2;
    volatile HI_U32 u32RegAddr = 0;
    HI_U32 coef_tmp[8] = {0};
    HI_U32 i;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    for (i = 0; i < 8; i++)
    {
        coef_tmp[i] = VI_DRV_Conver_FirCoef(hlCoef[i]);
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LHFIR_COEF0.u32) - (HI_U32)pViReg) + phase_offset * 0x10;
    CH_LHFIR_COEF0.u32 = ViRegRead(u32RegAddr);
    CH_LHFIR_COEF0.bits.coef0 = coef_tmp[0];
    CH_LHFIR_COEF0.bits.coef1 = coef_tmp[1];
    CH_LHFIR_COEF0.bits.coef2 = coef_tmp[2];
    ViRegWrite(u32RegAddr, CH_LHFIR_COEF0.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LHFIR_COEF1.u32) - (HI_U32)pViReg) + phase_offset * 0x10;
    CH_LHFIR_COEF1.u32 = ViRegRead(u32RegAddr);
    CH_LHFIR_COEF1.bits.coef3 = coef_tmp[3];
    CH_LHFIR_COEF1.bits.coef4 = coef_tmp[4];
    CH_LHFIR_COEF1.bits.coef5 = coef_tmp[5];
    ViRegWrite(u32RegAddr, CH_LHFIR_COEF1.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_LHFIR_COEF2.u32) - (HI_U32)pViReg) + phase_offset * 0x10;
    CH_LHFIR_COEF2.u32 = ViRegRead(u32RegAddr);
    CH_LHFIR_COEF2.bits.coef6 = coef_tmp[6];
    CH_LHFIR_COEF2.bits.coef7 = coef_tmp[7];
    ViRegWrite(u32RegAddr, CH_LHFIR_COEF2.u32);

    return;
}

HI_VOID VI_DRV_SetChChFirCoef(HI_U32 ChId, HI_U32 phase_offset, const HI_S32 hchCoef[4])
{
    volatile U_CH_CHFIR_COEF0 CH_CHFIR_COEF0;
    volatile U_CH_CHFIR_COEF1 CH_CHFIR_COEF1;
    volatile HI_U32 u32RegAddr = 0;
    HI_U32 coef_tmp[4] = {0};
    HI_U32 i;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    for (i = 0; i < 4; i++)
    {
        coef_tmp[i] = VI_DRV_Conver_FirCoef(hchCoef[i]);
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CHFIR_COEF0.u32) - (HI_U32)pViReg) + phase_offset * 0x10;
    CH_CHFIR_COEF0.u32 = ViRegRead(u32RegAddr);
    CH_CHFIR_COEF0.bits.coef0 = coef_tmp[0];
    CH_CHFIR_COEF0.bits.coef1 = coef_tmp[1];
    CH_CHFIR_COEF0.bits.coef2 = coef_tmp[2];
    ViRegWrite(u32RegAddr, CH_CHFIR_COEF0.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CHFIR_COEF1.u32) - (HI_U32)pViReg) + phase_offset * 0x10;
    CH_CHFIR_COEF1.u32 = ViRegRead(u32RegAddr);
    CH_CHFIR_COEF1.bits.coef3 = coef_tmp[3];
    ViRegWrite(u32RegAddr, CH_CHFIR_COEF1.u32);

    return;
}


HI_VOID VI_DRV_SetChVcdsEn(HI_U32 ChId, HI_BOOL bEn)
{
    volatile U_CH_VCDS_CFG CH_VCDS_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_VCDS_CFG.u32) - (HI_U32)pViReg);
    CH_VCDS_CFG.u32 = ViRegRead(u32RegAddr);
    CH_VCDS_CFG.bits.cds_en = bEn;
    ViRegWrite(u32RegAddr, CH_VCDS_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChVcdsCoef(HI_U32 ChId, HI_U32 Coef[2])
{
    volatile U_CH_VCDS_COEF CH_VCDS_COEF;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_VCDS_COEF.u32) - (HI_U32)pViReg);
    CH_VCDS_COEF.u32 = ViRegRead(u32RegAddr);
    CH_VCDS_COEF.bits.coef0 = Coef[0];
    CH_VCDS_COEF.bits.coef1 = Coef[1];
    ViRegWrite(u32RegAddr, CH_VCDS_COEF.u32);

    return;
}

HI_VOID VI_DRV_SetChDither(HI_U32 ChId, HI_U32 u32Mode)
{
    volatile U_CH_DITHER_CFG CH_DITHER_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DITHER_CFG.u32) - (HI_U32)pViReg);
    CH_DITHER_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DITHER_CFG.bits.dither_md = u32Mode;
    ViRegWrite(u32RegAddr, CH_DITHER_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChDitherCoef(HI_U32 ChId, HI_U32 Coef[8])
{
    volatile U_CH_DITHER_COEF0 CH_DITHER_COEF0;
    volatile U_CH_DITHER_COEF1 CH_DITHER_COEF1;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DITHER_COEF0.u32) - (HI_U32)pViReg);
    CH_DITHER_COEF0.u32 = ViRegRead(u32RegAddr);
    CH_DITHER_COEF0.bits.dither_coef0 = Coef[0];
    CH_DITHER_COEF0.bits.dither_coef1 = Coef[1];
    CH_DITHER_COEF0.bits.dither_coef2 = Coef[2];
    CH_DITHER_COEF0.bits.dither_coef3 = Coef[3];
    ViRegWrite(u32RegAddr, CH_DITHER_COEF0.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DITHER_COEF1.u32) - (HI_U32)pViReg);
    CH_DITHER_COEF1.u32 = ViRegRead(u32RegAddr);
    CH_DITHER_COEF1.bits.dither_coef4 = Coef[4];
    CH_DITHER_COEF1.bits.dither_coef5 = Coef[5];
    CH_DITHER_COEF1.bits.dither_coef6 = Coef[6];
    CH_DITHER_COEF1.bits.dither_coef7 = Coef[7];
    ViRegWrite(u32RegAddr, CH_DITHER_COEF1.u32);

    return;
}

HI_VOID VI_DRV_SetChDitherSize(HI_U32 ChId, SIZE_S * pstYSrcSize, SIZE_S * pstCSrcSize)
{
    volatile U_CH_DITHER_Y_SIZE CH_DITHER_Y_SIZE;
    volatile U_CH_DITHER_C_SIZE CH_DITHER_C_SIZE;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DITHER_Y_SIZE.u32) - (HI_U32)pViReg);
    CH_DITHER_Y_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_DITHER_Y_SIZE.bits.width = pstYSrcSize->u32Width - 1;
    ViRegWrite(u32RegAddr, CH_DITHER_Y_SIZE.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DITHER_C_SIZE.u32) - (HI_U32)pViReg);
    CH_DITHER_C_SIZE.u32 = ViRegRead(u32RegAddr);
    CH_DITHER_C_SIZE.bits.width = pstCSrcSize->u32Width - 1;
    ViRegWrite(u32RegAddr, CH_DITHER_C_SIZE.u32);

    return;
}

HI_VOID VI_DRV_SetChMirror(HI_U32 ChId, HI_BOOL bEn)
{
    volatile U_CH_PACK_Y_CFG CH_PACK_Y_CFG;
    volatile U_CH_PACK_C_CFG CH_PACK_C_CFG;
    volatile U_CH_DES_Y_CFG CH_DES_Y_CFG;
    volatile U_CH_DES_C_CFG CH_DES_C_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_Y_CFG.u32) - (HI_U32)pViReg);
    CH_PACK_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_PACK_Y_CFG.bits.mode = (bEn ? 0x3 : 0x0);
    ViRegWrite(u32RegAddr, CH_PACK_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_C_CFG.u32) - (HI_U32)pViReg);
    CH_PACK_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_PACK_C_CFG.bits.mode = (bEn ? 0x3 : 0x0);
    ViRegWrite(u32RegAddr, CH_PACK_C_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_CFG.u32) - (HI_U32)pViReg);
    CH_DES_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_CFG.bits.mirror = bEn;
    ViRegWrite(u32RegAddr, CH_DES_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_CFG.u32) - (HI_U32)pViReg);
    CH_DES_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_C_CFG.bits.mirror = bEn;
    ViRegWrite(u32RegAddr, CH_DES_C_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChFlip(HI_U32 ChId, HI_BOOL bEn)
{
    volatile U_CH_DES_Y_CFG CH_DES_Y_CFG;
    volatile U_CH_DES_C_CFG CH_DES_C_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_CFG.u32) - (HI_U32)pViReg);
    CH_DES_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_CFG.bits.flip = bEn;
    ViRegWrite(u32RegAddr, CH_DES_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_CFG.u32) - (HI_U32)pViReg);
    CH_DES_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_C_CFG.bits.flip = bEn;
    ViRegWrite(u32RegAddr, CH_DES_C_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChDes(HI_U32 ChId, VI_DRV_CHN_STORE_INFO * pstStoreCfg)
{
    volatile U_CH_PACK_Y_CFG CH_PACK_Y_CFG;
    volatile U_CH_PACK_C_CFG CH_PACK_C_CFG;
    volatile HI_U32 CH_PACK_Y_WIDTH;
    volatile HI_U32 CH_PACK_C_WIDTH;

    volatile U_CH_DES_Y_WIDTH CH_DES_Y_WIDTH;
    volatile U_CH_DES_Y_HEIGHT CH_DES_Y_HEIGHT;
    volatile U_CH_DES_C_WIDTH CH_DES_C_WIDTH;
    volatile U_CH_DES_C_HEIGHT CH_DES_C_HEIGHT;
    volatile U_CH_DES_Y_STRIDE CH_DES_Y_STRIDE;
    volatile U_CH_DES_C_STRIDE CH_DES_C_STRIDE;

    volatile HI_U32 u32RegAddr = 0;
    volatile HI_U32 u32Stride = pstStoreCfg->u32Stride;
    volatile HI_U32 u32Height = pstStoreCfg->u32Height;

    if (HI_FALSE == pstStoreCfg->bProgressive)
    {
        u32Stride = u32Stride * 2;
        u32Height = u32Height / 2;
    }

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_Y_CFG) - (HI_U32)pViReg);
    CH_PACK_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_PACK_Y_CFG.bits.bitw = pstStoreCfg->u32BitWidth;
    ViRegWrite(u32RegAddr, CH_PACK_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_C_CFG) - (HI_U32)pViReg);
    CH_PACK_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_PACK_C_CFG.bits.bitw = pstStoreCfg->u32BitWidth * 2;
    ViRegWrite(u32RegAddr, CH_PACK_C_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_Y_WIDTH) - (HI_U32)pViReg);
    CH_PACK_Y_WIDTH = ViRegRead(u32RegAddr);
    CH_PACK_Y_WIDTH = pstStoreCfg->u32Width * pstStoreCfg->u32BitWidth - 1;
    ViRegWrite(u32RegAddr, CH_PACK_Y_WIDTH);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_PACK_C_WIDTH) - (HI_U32)pViReg);
    CH_PACK_C_WIDTH = ViRegRead(u32RegAddr);
    if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
    {
        CH_PACK_C_WIDTH = pstStoreCfg->u32Width * 2 * pstStoreCfg->u32BitWidth - 1;
    }
    else
    {
        CH_PACK_C_WIDTH = pstStoreCfg->u32Width * pstStoreCfg->u32BitWidth - 1;
    }

    ViRegWrite(u32RegAddr, CH_PACK_C_WIDTH);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_HEIGHT) - (HI_U32)pViReg);
    CH_DES_Y_HEIGHT.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_HEIGHT.bits.height = u32Height - 1;
    ViRegWrite(u32RegAddr, CH_DES_Y_HEIGHT.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_WIDTH) - (HI_U32)pViReg);
    CH_DES_Y_WIDTH.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_WIDTH.bits.width = (pstStoreCfg->u32Width * pstStoreCfg->u32BitWidth + 127) / 128 - 1;
    ViRegWrite(u32RegAddr, CH_DES_Y_WIDTH.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_HEIGHT) - (HI_U32)pViReg);
    CH_DES_C_HEIGHT.u32 = ViRegRead(u32RegAddr);
    if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420)
    {
        CH_DES_C_HEIGHT.bits.height = u32Height / 2 - 1;
    }
    else
    {
        CH_DES_C_HEIGHT.bits.height = u32Height - 1;
    }

    ViRegWrite(u32RegAddr, CH_DES_C_HEIGHT.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_WIDTH) - (HI_U32)pViReg);
    CH_DES_C_WIDTH.u32 = ViRegRead(u32RegAddr);
    if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
    {
        CH_DES_C_WIDTH.bits.width = (pstStoreCfg->u32Width * 2 * pstStoreCfg->u32BitWidth + 127) / 128 - 1;
    }
    else
    {
        CH_DES_C_WIDTH.bits.width = (pstStoreCfg->u32Width * pstStoreCfg->u32BitWidth + 127) / 128 - 1;
    }

    ViRegWrite(u32RegAddr, CH_DES_C_WIDTH.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_STRIDE) - (HI_U32)pViReg);
    CH_DES_Y_STRIDE.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_STRIDE.bits.stride = u32Stride;
    ViRegWrite(u32RegAddr, CH_DES_Y_STRIDE.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_STRIDE) - (HI_U32)pViReg);
    CH_DES_C_STRIDE.u32 = ViRegRead(u32RegAddr);
    if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
    {
        CH_DES_C_STRIDE.bits.stride = u32Stride * 2;
    }
    else
    {
        CH_DES_C_STRIDE.bits.stride = u32Stride;
    }

    ViRegWrite(u32RegAddr, CH_DES_C_STRIDE.u32);

    return;
}

HI_VOID VI_DRV_SetChDesBufLen(HI_U32 ChId, HI_U32 u32YBufLen, HI_U32 u32CBufLen)
{
    volatile U_CH_DES_Y_CFG CH_DES_Y_CFG;
    volatile U_CH_DES_C_CFG CH_DES_C_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_CFG.u32) - (HI_U32)pViReg);
    CH_DES_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_CFG.bits.buf_len = u32YBufLen;
    ViRegWrite(u32RegAddr, CH_DES_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_CFG.u32) - (HI_U32)pViReg);
    CH_DES_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_C_CFG.bits.buf_len = u32CBufLen;
    ViRegWrite(u32RegAddr, CH_DES_C_CFG.u32);

    return;
}

HI_VOID VI_DRV_SetChDesDis(HI_U32 ChId, HI_U32 u32YDis, HI_U32 u32CDis)
{
    volatile U_CH_DES_Y_CFG CH_DES_Y_CFG;
    volatile U_CH_DES_C_CFG CH_DES_C_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_CFG.u32) - (HI_U32)pViReg);
    CH_DES_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_Y_CFG.bits.dis = u32YDis;
    ViRegWrite(u32RegAddr, CH_DES_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_CFG.u32) - (HI_U32)pViReg);
    CH_DES_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_DES_C_CFG.bits.dis = u32CDis;
    ViRegWrite(u32RegAddr, CH_DES_C_CFG.u32);

    return;
}


HI_VOID VI_DRV_SetChDesAddr(HI_U32 ChId, HI_U32 u32YAddr, HI_U32 u32CAddr)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_FADDR0) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32YAddr);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_FADDR0) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32CAddr);

    return;
}

HI_U32 VI_DRV_GetChDesYAddr(HI_U32 ChId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return 0;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_FADDR0) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_U32 VI_DRV_GetChDesCAddr(HI_U32 ChId)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return 0;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_FADDR0) - (HI_U32)pViReg);

    return ViRegRead(u32RegAddr);
}

HI_VOID VI_DRV_SetChDesAddr1(HI_U32 ChId, HI_U32 u32YAddr, HI_U32 u32CAddr)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_FADDR1) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32YAddr);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_FADDR1) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32CAddr);

    return;
}

HI_VOID VI_DRV_SetChDesYFaddr(HI_U32 ChId, HI_U32 u32YFaddr0, HI_U32 u32YFaddr1)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_FADDR0) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32YFaddr0);
    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_Y_FADDR1) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32YFaddr1);

    return;
}

HI_VOID VI_DRV_SetChDesCFaddr(HI_U32 ChId, HI_U32 u32CFaddr0, HI_U32 u32CFaddr1)
{
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_FADDR0) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32CFaddr0);
    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_DES_C_FADDR1) - (HI_U32)pViReg);
    ViRegWrite(u32RegAddr, u32CFaddr1);

    return;
}

HI_VOID VI_DRV_SetCh3dSplit(HI_U32 ChId, HI_BOOL bEn, HI_BOOL bMode)
{
    volatile U_CH_3D_SPLIT_CFG CH_3D_SPLIT_CFG;
    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_CFG.u32) - (HI_U32)pViReg);
    CH_3D_SPLIT_CFG.u32 = ViRegRead(u32RegAddr);
    CH_3D_SPLIT_CFG.bits.enable = bEn;
    CH_3D_SPLIT_CFG.bits.mode = bMode;
    ViRegWrite(u32RegAddr, CH_3D_SPLIT_CFG.u32);

    return;
}

HI_VOID VI_DRV_Set3dSize(HI_U32 ChId, VI_DRV_CHN_STORE_INFO * pstStoreCfg)
{
    volatile U_CH_3D_SPLIT_Y_SIZE CH_3D_SPLIT_Y_SIZE;
    volatile U_CH_3D_SPLIT_C_SIZE CH_3D_SPLIT_C_SIZE;

    volatile HI_U32 u32RegAddr = 0;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422)
    {
        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_Y_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_Y_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_Y_SIZE.bits.width  = pstStoreCfg->u32Width;
        CH_3D_SPLIT_Y_SIZE.bits.height = pstStoreCfg->u32Height;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_Y_SIZE.u32);

        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_C_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_C_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_C_SIZE.bits.width  = pstStoreCfg->u32Width / 2;
        CH_3D_SPLIT_C_SIZE.bits.height = pstStoreCfg->u32Height;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_C_SIZE.u32);
    }
    else if (pstStoreCfg->enPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420)
    {
        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_Y_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_Y_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_Y_SIZE.bits.width  = pstStoreCfg->u32Width;
        CH_3D_SPLIT_Y_SIZE.bits.height = pstStoreCfg->u32Height;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_Y_SIZE.u32);

        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_C_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_C_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_C_SIZE.bits.width  = pstStoreCfg->u32Width / 2;
        CH_3D_SPLIT_C_SIZE.bits.height = pstStoreCfg->u32Height / 2;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_C_SIZE.u32);
    }
    else
    {
        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_Y_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_Y_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_Y_SIZE.bits.width  = pstStoreCfg->u32Width;
        CH_3D_SPLIT_Y_SIZE.bits.height = pstStoreCfg->u32Height;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_Y_SIZE.u32);

        u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_3D_SPLIT_C_SIZE) - (HI_U32)pViReg);
        CH_3D_SPLIT_C_SIZE.u32 = ViRegRead(u32RegAddr);
        CH_3D_SPLIT_C_SIZE.bits.width  = pstStoreCfg->u32Width;
        CH_3D_SPLIT_C_SIZE.bits.height = pstStoreCfg->u32Height;
        ViRegWrite(u32RegAddr, CH_3D_SPLIT_C_SIZE.u32);
    }

    return;
}

HI_VOID VI_DRV_SetChClip(HI_U32 ChId, HI_U32 u32YMax, HI_U32 u32YMin, HI_U32 u32CMax, HI_U32 u32CMin)
{
    volatile U_CH_CLIP_Y_CFG CH_CLIP_Y_CFG;
    volatile U_CH_CLIP_C_CFG CH_CLIP_C_CFG;
    volatile HI_U32 u32RegAddr = 0;
    HI_U32 bitWidth = 8;

    if (VI_CH_NUM <= ChId)
    {
        HI_FATAL_VI("Error Channel Id %d!\n", ChId);
        return;
    }

    if (u32YMax < u32YMin)
    {
        HI_FATAL_VI("Ch %d Y clip min larger than max!\n", ChId);
        return;
    }

    if (u32CMax < u32CMin)
    {
        HI_FATAL_VI("Ch %d C clip min larger than max!\n", ChId);
        return;
    }

    if (u32YMax > 65535)
    {
        u32YMax = 65535;
    }

    if (u32CMax > 65535)
    {
        u32CMax = 65535;
    }

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CLIP_Y_CFG.u32) - (HI_U32)pViReg);
    CH_CLIP_Y_CFG.u32 = ViRegRead(u32RegAddr);
    CH_CLIP_Y_CFG.bits.max = u32YMax* VI_UtilsMathPow(2, 16 - bitWidth); // msb
    CH_CLIP_Y_CFG.bits.min = u32YMin* VI_UtilsMathPow(2, 16 - bitWidth);
    ViRegWrite(u32RegAddr, CH_CLIP_Y_CFG.u32);

    u32RegAddr = VI_CH_DIFF * ChId + ((HI_U32)&(pViReg->CH_CLIP_C_CFG.u32) - (HI_U32)pViReg);
    CH_CLIP_C_CFG.u32 = ViRegRead(u32RegAddr);
    CH_CLIP_C_CFG.bits.max = u32CMax* VI_UtilsMathPow(2, 16 - bitWidth);
    CH_CLIP_C_CFG.bits.min = u32CMin* VI_UtilsMathPow(2, 16 - bitWidth);
    ViRegWrite(u32RegAddr, CH_CLIP_C_CFG.u32);

    return;
}


HI_BOOL VI_DRV_IsInCapTop(HI_U32 PtId)
{
    volatile HI_U32 u32RegAddr = 0;

    u32RegAddr = VI_DRV_GetPtStatus(PtId);

    if ((0x8 & u32RegAddr) == 0x8)
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}

/**
 *    Adapter Layer
 *
 */

HI_VOID VI_DRV_SetTiming_BT656(HI_U32 PtId)
{
    VI_DRV_BT656_CFG st656Cfg;
    VI_DRV_PORT_MSK stMsk;
    VI_DRV_PORT_UNIFY_TIMING unitiming;
    VI_DRV_PORT_UNIFY_DATA unidat;

#if defined(CFG_HI_VI_BT656_HIGH8)
    stMsk.u32ROff = 8;
#else
    stMsk.u32ROff = 0;
#endif
    stMsk.u32RMsk = 0xff00;
    stMsk.u32RRev = 0;
    stMsk.u32GMsk = 0x0000;
    stMsk.u32GOff = 0;
    stMsk.u32GRev = 0;
    stMsk.u32BMsk = 0x0000;
    stMsk.u32BOff = 0;
    stMsk.u32BRev = 0;
    VI_DRV_SetPtMask(PtId, &stMsk);

    unitiming.field_inv  = 0;
    unitiming.field_sel  = 0;
    unitiming.vsync_mode = 1;
    unitiming.vsync_inv  = 0;
    unitiming.vsync_sel  = 0;
    unitiming.hsync_mode = 1;
    unitiming.hsync_and = 1;
    unitiming.hsync_inv = 0;
    unitiming.hsync_sel = 0;
    unitiming.de_inv = 0;
    unitiming.de_sel = 1;
    VI_DRV_SetPtUnifyTiming(PtId, &unitiming);

    unidat.enable   = 1;
    unidat.uv_seq   = 0;
    unidat.yc_seq   = 0;
    unidat.comp_num = 0;
    VI_DRV_SetPtUnifyData(PtId, &unidat);

    VI_DRV_SetPtTimingMode(PtId, 1);

    st656Cfg.bFieldInv = 1;
    st656Cfg.bHsyncInv = 1;
    st656Cfg.bVsyncInv = 1;
    st656Cfg.u32Mode = 3;
    st656Cfg.bEn= 1;
    VI_DRV_SetPtTimingBT656(PtId, &st656Cfg);
    VI_DRV_SetPtYUV444(PtId, HI_TRUE);

    return;
}

HI_VOID VI_DRV_SetTiming_BT1120(HI_U32 PtId)
{
    VI_DRV_BT656_CFG st656Cfg;
    VI_DRV_PORT_MSK stMsk;
    VI_DRV_PORT_UNIFY_TIMING unitiming;
    VI_DRV_PORT_UNIFY_DATA unidat;

    stMsk.u32RMsk = 0xff00;
    stMsk.u32ROff = 0;
    stMsk.u32RRev = 0;
    stMsk.u32GMsk = 0xff00;
    stMsk.u32GOff = 0x8;
    stMsk.u32GRev = 0;
    stMsk.u32BMsk = 0x0000;
    stMsk.u32BOff = 0;
    stMsk.u32BRev = 0;
    VI_DRV_SetPtMask(PtId, &stMsk);

    unitiming.field_inv  = 0;
    unitiming.field_sel  = 0;
    unitiming.vsync_mode = 1;
    unitiming.vsync_inv  = 0;
    unitiming.vsync_sel  = 0;
    unitiming.hsync_mode = 1;
    unitiming.hsync_and = 1;
    unitiming.hsync_inv = 0;
    unitiming.hsync_sel = 0;
    unitiming.de_inv = 0;
    unitiming.de_sel = 1;
    VI_DRV_SetPtUnifyTiming(PtId, &unitiming);

    unidat.enable   = 1;
    unidat.uv_seq   = 0;
    unidat.yc_seq   = 0;
    unidat.comp_num = 1;
    VI_DRV_SetPtUnifyData(PtId, &unidat);
    VI_DRV_SetPtTimingMode(PtId, 1);

    st656Cfg.bFieldInv = 1;
    st656Cfg.bHsyncInv = 1;
    st656Cfg.bVsyncInv = 1;
    st656Cfg.u32Mode = 3;
    st656Cfg.bEn= 1;
    VI_DRV_SetPtTimingBT656(PtId, &st656Cfg);
    VI_DRV_SetPtYUV444(PtId, HI_TRUE);

    return;
}

HI_VOID VI_DRV_SetTiming_DigitalCam(HI_U32 PtId)
{
   VI_DRV_PORT_MSK stMsk;
   VI_DRV_PORT_UNIFY_TIMING stUnifyTiming;
   VI_DRV_PORT_UNIFY_DATA stUnifyData;

   stMsk.u32RMsk = 0xfff0;
   stMsk.u32ROff = 0;
   stMsk.u32RRev = 0;
   stMsk.u32GMsk = 0;
   stMsk.u32GOff = 0;
   stMsk.u32GRev = 0;
   stMsk.u32BMsk = 0;
   stMsk.u32BOff = 0;
   stMsk.u32BRev = 0;
   VI_DRV_SetPtMask(PtId, &stMsk);

   stUnifyTiming.field_inv  = 0;
   stUnifyTiming.field_sel  = 0;
   stUnifyTiming.vsync_mode = 1;
   stUnifyTiming.vsync_inv  = 1;
   stUnifyTiming.vsync_sel  = 0;
   stUnifyTiming.hsync_mode = 1;
   stUnifyTiming.hsync_and  = 1;
   stUnifyTiming.hsync_inv  = 0;
   stUnifyTiming.hsync_sel  = 0;
   stUnifyTiming.de_inv     = 0;
   stUnifyTiming.de_sel     = 1;
   VI_DRV_SetPtUnifyTiming(PtId, &stUnifyTiming);//0xc2801

   stUnifyData.enable = 1;
   stUnifyData.uv_seq   = 0;
   stUnifyData.yc_seq   = 1;
   stUnifyData.comp_num = 0;
   VI_DRV_SetPtUnifyData(PtId, &stUnifyData);//0x80000004

   VI_DRV_SetPtTimingMode(PtId, 0);
   VI_DRV_SetPtYUV444(PtId, HI_TRUE);

   return;

}

HI_VOID VI_DRV_SetTiming_HDMI_RX(HI_U32 PtId)
{
    VI_DRV_BT656_CFG st656Cfg;
    VI_DRV_PORT_MSK stMsk;
    VI_DRV_PORT_UNIFY_TIMING unitiming;
    VI_DRV_PORT_UNIFY_DATA unidat;

    stMsk.u32RMsk = 0xffc0;
    stMsk.u32ROff = 0xc;
    stMsk.u32RRev = 0;
    stMsk.u32GMsk = 0xffc0;
    stMsk.u32GOff = 0x18;
    stMsk.u32GRev = 0;
    stMsk.u32BMsk = 0xffc0;
    stMsk.u32BOff = 0x0;
    stMsk.u32BRev = 0;
    VI_DRV_SetPtMask(PtId, &stMsk);


    unitiming.field_inv  = 0;
    unitiming.field_sel  = 0;
    unitiming.vsync_mode = 1;
    unitiming.vsync_inv  = 0;
    unitiming.vsync_sel  = 0;
    unitiming.hsync_mode = 1;
    unitiming.hsync_and = 0;
    unitiming.hsync_inv = 0;
    unitiming.hsync_sel = 0;
    unitiming.de_inv = 0;
    unitiming.de_sel = 0;
    VI_DRV_SetPtUnifyTiming(PtId, &unitiming);

    unidat.enable   = 1;
    unidat.uv_seq   = 0;
    unidat.yc_seq   = 0;
    unidat.comp_num = 2;
    VI_DRV_SetPtUnifyData(PtId, &unidat);
    VI_DRV_SetPtTimingMode(PtId, 0);
    st656Cfg.bEn= 1;
    st656Cfg.bFieldInv = 0;
    st656Cfg.bHsyncInv = 1;
    st656Cfg.bVsyncInv = 1;
    st656Cfg.u32Mode = 3;
    VI_DRV_SetPtTimingBT656(PtId, &st656Cfg);
    VI_DRV_SetPtYUV444(PtId, HI_TRUE);

    return;
}

HI_VOID VI_DRV_SetVcapInit(HI_U32 ChId, VI_DRV_CHN_STORE_INFO *pstStoreCfg)
{
    VI_DRV_CHN_LHFIR_SPH stlhfir;
    VI_DRV_CHN_CHFIR_SPH stchfir;

    VI_DRV_SetMasterOtd(0, 4);
    VI_DRV_SetChDes(ChId, pstStoreCfg);
    VI_DRV_SetChEn(0, HI_FALSE);
    VI_DRV_SetChCropEn(ChId, HI_FALSE, HI_FALSE);
    VI_DRV_SetChMirror(ChId, HI_FALSE);
    VI_DRV_SetChFlip(ChId, HI_FALSE);
    VI_DRV_SetChDither(ChId, 7);
    memset(&stlhfir, 0, sizeof(VI_DRV_CHN_LHFIR_SPH));
    VI_DRV_SetChLhFirSph(ChId, &stlhfir);
    memset(&stchfir, 0, sizeof(VI_DRV_CHN_CHFIR_SPH));
    VI_DRV_SetChChFirSph(ChId, &stchfir);
    VI_DRV_SetChSkip(ChId, 0xffffffff, 0xffffffff);
    VI_DRV_ClrPtIntStatus(0, 0xffffffff);
    VI_DRV_ClrChIntStatus(0, 0xffffffff);
    VI_DRV_SetPtIntMask(0, 0, 0, 0);
    VI_DRV_SetChIntMask(ChId, 0);
    VI_DRV_SetWorkMode(HI_FALSE, HI_FALSE);
    //VI_DRV_SetVicapIntMask(0, 1);
    return;
}

HI_VOID VI_DRV_SetChZme(HI_U32 ChId, VI_DRV_CHN_ZME_INFO * pstZmeInfo)
{
    HI_U32 i;
    VI_DRV_CHN_LHFIR_SPH stlhfir;
    VI_DRV_CHN_CHFIR_SPH stchfir;
    HI_U32 u32Phase = 32;
    HI_U32 u32CoeffGrpNum = u32Phase / 2 + 1;

    int vi_coefficient4_cubic[18][4] =
    {
        {  0, 511,   0,   0}, {-19, 511,  21,  -1}, {-37, 509,  42,  -2}, {-51, 504,  64,  -5},
        {-64, 499,  86,  -9}, {-74, 492, 108, -14}, {-82, 484, 129, -19}, {-89, 474, 152, -25},
        {-94, 463, 174, -31}, {-97, 451, 196, -38}, {-98, 438, 217, -45}, {-98, 424, 238, -52},
        {-98, 409, 260, -59}, {-95, 392, 280, -65}, {-92, 376, 300, -72}, {-88, 358, 320, -78},
        {-83, 339, 339, -83}, {  0,   0,   0,   0}
    };

    int vi_coefficient8_cubic[18][8] =
    {
        { 0,  0,   0, 511,   0,   0,  0,  0}, {-1,  3, -12, 511,  14,  -4,  1,  0},
        {-2,  6, -23, 509,  28,  -8,  2,  0}, {-2,  9, -33, 503,  44, -12,  3,  0},
        {-3, 11, -41, 496,  61, -16,  4,  0}, {-3, 13, -48, 488,  79, -21,  5, -1},
        {-3, 14, -54, 477,  98, -25,  7, -2}, {-4, 16, -59, 465, 118, -30,  8, -2},
        {-4, 17, -63, 451, 138, -35,  9, -1}, {-4, 18, -66, 437, 158, -39, 10, -2},
        {-4, 18, -68, 421, 180, -44, 11, -2}, {-4, 18, -69, 404, 201, -48, 13, -3},
        {-4, 18, -70, 386, 222, -52, 14, -2}, {-4, 18, -70, 368, 244, -56, 15, -3},
        {-4, 18, -69, 348, 265, -59, 16, -3}, {-4, 18, -67, 329, 286, -63, 16, -3},
        {-3, 17, -65, 307, 307, -65, 17, -3}, { 0,  0,   0,   0,   0,   0,  0,  0}
    };

    stlhfir.hlmsc_en = 1;
    stlhfir.hlmid_en = 1;
    stlhfir.hlfir_en = 0;
    stlhfir.hlratio   = 4096 * pstZmeInfo->stSrcSize.u32Width / pstZmeInfo->stDstSize.u32Width;
    stchfir.hchmsc_en = 1;
    stchfir.hchmid_en = 1;
    stchfir.hchfir_en = 0;
    stchfir.hchratio  = 4096 * pstZmeInfo->stSrcSize.u32Width / pstZmeInfo->stDstSize.u32Width;

    if (pstZmeInfo->stSrcSize.u32Width == pstZmeInfo->stDstSize.u32Width)
    {
        stlhfir.hlmsc_en = 0;
    }

    if ((pstZmeInfo->enSrcPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
        && (pstZmeInfo->enDstPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422))
    {
        stchfir.hchratio = 4096 * pstZmeInfo->stSrcSize.u32Width / pstZmeInfo->stDstSize.u32Width * 2;
    }

    VI_DRV_SetChLhFirSph(ChId, &stlhfir);
    VI_DRV_SetChChFirSph(ChId, &stchfir);
    VI_DRV_SetChLhFirOffset(ChId, 0 * 4096);
    VI_DRV_SetChChFirOffset(ChId, 0 * 4096);

    if ((pstZmeInfo->enSrcPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
        && (pstZmeInfo->enDstPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444))
    {
        VI_DRV_SetChLFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChCFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChLFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width);
        VI_DRV_SetChCFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width);
    }
    else if ((pstZmeInfo->enSrcPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422)
             && (pstZmeInfo->enDstPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422))
    {
        VI_DRV_SetChLFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChCFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width / 2);
        VI_DRV_SetChLFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width);
        VI_DRV_SetChCFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width / 2);
    }
    else if ((pstZmeInfo->enSrcPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420)
             && (pstZmeInfo->enDstPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_420))
    {
        VI_DRV_SetChLFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChCFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width / 2);
        VI_DRV_SetChLFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width);
        VI_DRV_SetChCFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width / 2);
    }
    else if ((pstZmeInfo->enSrcPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_444)
             && (pstZmeInfo->enDstPixFormat == HI_UNF_FORMAT_YUV_SEMIPLANAR_422))
    {
        VI_DRV_SetChLFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChCFirInSize(ChId, pstZmeInfo->stSrcSize.u32Width);
        VI_DRV_SetChLFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width);
        VI_DRV_SetChCFirOutSize(ChId, pstZmeInfo->stDstSize.u32Width / 2);
    }

    for (i = 0; i < u32CoeffGrpNum; i++)
    {
        VI_DRV_SetChLhFirCoef(ChId, i, vi_coefficient8_cubic[i]);
        VI_DRV_SetChChFirCoef(ChId, i, vi_coefficient4_cubic[i]);
    }

    VI_DRV_SetChCoefUpda(ChId, 1, 1);

    return;
}

HI_VOID VI_DRV_EnableColourBar(HI_VOID)
{
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0180)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0184)) = 1280;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0188)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca018c)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0190)) = 720;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0194)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0198)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca019c)) = 720;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca01a0)) = 400;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0134)) = 0x80000006;
    *(volatile HI_U32  *)(IO_ADDRESS(0xf8ca0144)) = 0x800000ed;

    return;
}
