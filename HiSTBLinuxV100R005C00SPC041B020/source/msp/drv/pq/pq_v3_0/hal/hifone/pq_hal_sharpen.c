/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_sharpen.c
  Version       : Initial Draft
  Author        : 
  Created       : 2013/11/10
  Description   :
******************************************************************************/
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "pq_hal_comm.h"
#include "pq_hal_sharpen.h"


/* Set hsharpen Strength reg, Global variable, Range:0~255 */
HI_S32 PQ_HAL_SetHSharpStrReg(HI_U32 u32HandleNo, HI_S32 s32ShpStr, PQ_HSHARP_GAIN_S *pstHSharpGain)
{
    HI_U32 u32GainPosH0;
    HI_U32 u32GainNegH0;
    HI_U32 u32GainPosH1; 
    HI_U32 u32GainNegH1;
    HI_U32 u32SharpIntensity = 128;
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    u32GainPosH0 = (HI_U32)(128 * 78 * s32ShpStr / 10000);
    u32GainNegH0 = (HI_U32)(128 * 78 * s32ShpStr / 10000);
    u32GainPosH1 = (HI_U32)(128 * 78 * s32ShpStr / 10000);
    u32GainNegH1 = (HI_U32)(128 * 78 * s32ShpStr / 10000);

    pstVpssVirReg->VPSS_HSPCFG3.bits.hsp_hf0_gainpos = (HI_S32)(((pstHSharpGain->s32H0GainPos) *u32GainPosH0 + 64) / u32SharpIntensity);
    pstVpssVirReg->VPSS_HSPCFG3.bits.hsp_hf0_gainneg = (HI_S32)(((pstHSharpGain->s32H0GainNeg) *u32GainNegH0 + 64) / u32SharpIntensity);
    pstVpssVirReg->VPSS_HSPCFG7.bits.hsp_hf1_gainpos = (HI_S32)(((pstHSharpGain->s32H1GainPos) *u32GainPosH1 + 64) / u32SharpIntensity);
    pstVpssVirReg->VPSS_HSPCFG7.bits.hsp_hf1_gainneg = (HI_S32)(((pstHSharpGain->s32H1GainNeg) *u32GainNegH1 + 64) / u32SharpIntensity);
    
    return HI_SUCCESS;
}

/* Enable HSharpen */
HI_S32 PQ_HAL_EnableHSharp(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL.bits.hsp_en = bOnOff;

    return HI_SUCCESS;
}

/* Get HSharpen Enable Flag */
HI_S32 PQ_HAL_GetEnableHSharp(HI_U32 u32HandleNo, HI_BOOL *bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *bOnOff = pstVpssVirReg->VPSS_CTRL.bits.hsp_en;

    return HI_SUCCESS;
}

/* Enable HSharpen Demo */
HI_S32 PQ_HAL_EnableHSharpDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_HSPCFG10.bits.hsp_mk_en = bOnOff;

    return HI_SUCCESS;
}

/* Choose HSharpen Demo Mode; 0£ºEnable Left,Right Orignal; 1£ºEnable Right,Left Orignal */
HI_S32 PQ_HAL_SetHSharpDemoMode(HI_U32 u32HandleNo, HI_U32 u32HshpDemoMode)
{
    S_CAS_REGS_TYPE *pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_HSPCFG10.bits.hsp_mk_mode  = u32HshpDemoMode;

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_SetSharpDemoPos(HI_U32 u32Data, HI_U32 u32Pos)
{
    return;
}

HI_VOID PQ_HAL_GetSharpDemoPos(HI_U32 u32Data, HI_U32* u32Pos)
{
    return;
}


