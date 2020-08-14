/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************
  File Name     : pq_hal_dei.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/11/20
  Description   :

******************************************************************************/

#include "pq_hal_comm.h"
#include "pq_hal_dei.h"


/* 根据globalmotion 计算出的运动信息调整gain值 */
HI_S32 PQ_HAL_SetDeiAdjustGain(HI_U32 u32HandleNo, HI_U32 u32AdjustGain)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain     = u32AdjustGain;
    pstVpssVirReg->VPSS_DIEMTNADJ.bits.motion_adjust_gain_chr = u32AdjustGain;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetMotionHistReg(MOTION_INPUT_S* pstMotionInput, MotionHist_S* pstMotionHist)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(pstMotionInput->u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    memcpy((HI_U32*) & (pstMotionHist->MOTION_HIST[0]), (HI_U32*) & (pstVpssVirReg->VPSS_DIEGLBMTNBIN[0]), sizeof(HI_U32) * 32);

    pstMotionHist->SMALL_MOTION_NUM = pstVpssVirReg->VPSS_DIEMTNNUM0.bits.glbm_num0;
    pstMotionHist->SMALL_MOTION_SUM = pstVpssVirReg->VPSS_DIEMTNSUM0.bits.glbm_sum0;
    pstMotionHist->MIDDL_MOTION_NUM = pstVpssVirReg->VPSS_DIEMTNNUM1.bits.glbm_num1 + pstVpssVirReg->VPSS_DIEMTNNUM2.bits.glbm_num2 + pstVpssVirReg->VPSS_DIEMTNNUM3.bits.glbm_num3;
    pstMotionHist->MIDDL_MOTION_SUM = pstVpssVirReg->VPSS_DIEMTNSUM1.bits.glbm_sum1 + pstVpssVirReg->VPSS_DIEMTNSUM2.bits.glbm_sum2 + pstVpssVirReg->VPSS_DIEMTNSUM3.bits.glbm_sum3;
    pstMotionHist->LARGE_MOTION_NUM = pstVpssVirReg->VPSS_DIEMTNNUM4.bits.glbm_num4;
    pstMotionHist->LARGE_MOTION_SUM = pstVpssVirReg->VPSS_DIEMTNSUM4.bits.glbm_sum4;

    pqprint(PQ_PRN_DEI, "[%s]:MtnNum:(%d, %d, %d);MtnSum:(%d, %d, %d);Hist:(%d, %d)\n",
            __FUNCTION__,
            pstMotionHist->SMALL_MOTION_NUM,
            pstMotionHist->MIDDL_MOTION_NUM,
            pstMotionHist->LARGE_MOTION_NUM,
            pstMotionHist->SMALL_MOTION_SUM,
            pstMotionHist->MIDDL_MOTION_SUM,
            pstMotionHist->LARGE_MOTION_SUM,
            pstMotionHist->MOTION_HIST[0],
            pstMotionHist->MOTION_HIST[1]);

    return HI_SUCCESS;
}

/* 获取隔行运动信息interlace globa motion开关状态 */
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo)
{
    return HI_TRUE;
}

HI_S32 PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bIGlbEn)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetDeiEn(HI_U32 u32HandleNo, HI_BOOL *pbOnOff)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    *pbOnOff = pstVpssVirReg->VPSS_CTRL.bits.dei_en;
    
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDeiDemo(HI_U32 u32HandleNo, HI_BOOL bDemoEn)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetVpssMktCoordinate(HI_U32 u32HandleNo, HI_U32 u32DbgPos)
{
    return HI_SUCCESS;
}


