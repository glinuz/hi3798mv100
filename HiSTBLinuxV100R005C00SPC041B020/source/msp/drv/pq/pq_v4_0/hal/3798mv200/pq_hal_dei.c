/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dei.c
  Version       : Initial Draft
  Author        : y00220178
  Created       : 2013/11/20
  Description   :

******************************************************************************/
#include "hi_type.h"
#include "pq_hal_dei.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"


/* 根据globalmotion 计算出的运动信息调整gain值 */
HI_S32 PQ_HAL_SetDeiAdjustGain(HI_U32 u32HandleNo, HI_U32 u32AdjustGain)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain = u32AdjustGain;
    return HI_SUCCESS;
}

/* 获取隔行运动信息interlace globa motion开关状态 */
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo)
{
    HI_BOOL bIGlbEn;
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    bIGlbEn = pstVpssVirReg->VPSS_CTRL.bits.igbm_en;

    return bIGlbEn;
}

HI_S32 PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    /* Logic limit: When dei close, can not open igbm_en; So PQ do not control */
    pstVpssVirReg->VPSS_CTRL.bits.igbm_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSoftReg(HI_U32 u32HandleNo, HI_Mc_DeiParam_S *pstMcParm, HI_DEI_Variable_S *pstDeiVar)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);
#if 0
    //pstVpssVirReg->VPSS_MCDI_MC2.bits.k_hfcore_gmd_mc = pstMcParm->K_HFCORE_GMD_MC;
    pstVpssVirReg->VPSS_DIECTRL.bits.mc_only = pstMcParm->MC_ONLY;
    pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = pstMcParm->MA_ONLY;
#endif
    //pstVpssVirReg->VPSS_DIECTRL.bits.mc_only = 1;
    //pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = 1;

    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcw   = pstMcParm->nK_MCW_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_y_mcbld = pstMcParm->nK_MCW_C;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcw   = pstMcParm->nK_MCBLD_Y;
    pstVpssVirReg->VPSS_DIEFUSION0.bits.k_c_mcbld = pstMcParm->nK_MCBLD_C;
    pstVpssVirReg->VPSS_MCDI_MC2.bits.k_c_vertw   = pstMcParm->K_C_VERTW;

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain = pstDeiVar->stMDINFO.ADJUST_GAIN;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_coring     = pstDeiVar->stINTERPINFO.MC_EDGE_CORING;
    pstVpssVirReg->VPSS_DIEEDGEFORMC.bits.edge_scale      = pstDeiVar->stINTERPINFO.MC_EDGE_SCALE;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_1 = pstDeiVar->stMDINFO.rec_mode_fld_motion_step_1 ;
    pstVpssVirReg->VPSS_DIEHISMODE.bits.rec_mode_fld_motion_step_0 = pstDeiVar->stMDINFO.rec_mode_fld_motion_step_0 ;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_UpdateBlkRect(HI_U32 u32HandleNo, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_U32 blkmvendc, blkmvstartc;
    VPSS_REG_S *pstVpssVirReg = NULL;

    blkmvstartc = 0;

    if (u32Width > 960)
    {
        blkmvendc = u32Width / 2 - 1;
    }
    else
    {
        blkmvendc = u32Width - 1;
    }
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    //pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvendc   = blkmvendc;
    //pstVpssVirReg->VPSS_MCDI_BLKH.bits.blkmvstartc = blkmvstartc;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetMotionCompensateEn(HI_U32 u32HandleNo, HI_BOOL bMcEnable)
{
    VPSS_REG_S *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.ma_only = 1 - bMcEnable;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDeiDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMaDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.dei_en;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMcDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.mcdi_en;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    if (DEI_DEMO_ENABLE_L == enMode) /* Enable_L: L-McEnable; R-McDisable */
    {
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_l = MC_DIOUTPUT;
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_r = MC_DIDISABLE;
    }
    else /* Enable_R: R-McEnable; L-McDisable */
    {
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_l = MC_DIDISABLE;
        pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_mode_r = MC_DIOUTPUT;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeiDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_MCDI_DEMO.bits.demo_border = u32XPos;

    return HI_SUCCESS;
}




