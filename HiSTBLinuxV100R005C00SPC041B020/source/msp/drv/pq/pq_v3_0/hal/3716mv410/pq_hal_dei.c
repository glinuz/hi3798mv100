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
    PQ_CHECK_NULL_PTR(pstMotionInput);

    pstMotionHist->MOTION_HIST[0]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN0;
    pstMotionHist->MOTION_HIST[1]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN2;
    pstMotionHist->MOTION_HIST[2]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN2;
    pstMotionHist->MOTION_HIST[3]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN3;
    pstMotionHist->MOTION_HIST[4]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN4;
    pstMotionHist->MOTION_HIST[5]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN5;
    pstMotionHist->MOTION_HIST[6]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN6;
    pstMotionHist->MOTION_HIST[7]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN7;
    pstMotionHist->MOTION_HIST[8]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN8;
    pstMotionHist->MOTION_HIST[9]  = pstMotionInput->pstMotionReg->IGLB_MTN_BIN9;
    pstMotionHist->MOTION_HIST[10] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN10;
    pstMotionHist->MOTION_HIST[11] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN11;
    pstMotionHist->MOTION_HIST[12] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN12;
    pstMotionHist->MOTION_HIST[13] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN13;
    pstMotionHist->MOTION_HIST[14] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN14;
    pstMotionHist->MOTION_HIST[15] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN15;
    pstMotionHist->MOTION_HIST[16] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN16;
    pstMotionHist->MOTION_HIST[17] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN17;
    pstMotionHist->MOTION_HIST[18] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN18;
    pstMotionHist->MOTION_HIST[19] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN19;
    pstMotionHist->MOTION_HIST[20] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN20;
    pstMotionHist->MOTION_HIST[21] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN21;
    pstMotionHist->MOTION_HIST[22] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN22;
    pstMotionHist->MOTION_HIST[23] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN23;
    pstMotionHist->MOTION_HIST[24] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN24;
    pstMotionHist->MOTION_HIST[25] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN25;
    pstMotionHist->MOTION_HIST[26] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN26;
    pstMotionHist->MOTION_HIST[27] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN27;
    pstMotionHist->MOTION_HIST[28] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN28;
    pstMotionHist->MOTION_HIST[29] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN29;
    pstMotionHist->MOTION_HIST[30] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN30;
    pstMotionHist->MOTION_HIST[31] = pstMotionInput->pstMotionReg->IGLB_MTN_BIN31;

    pstMotionHist->SMALL_MOTION_NUM = pstMotionInput->pstMotionReg->IGLB_MTN_NUM0;
    pstMotionHist->SMALL_MOTION_SUM = pstMotionInput->pstMotionReg->IGLB_MTN_SUM0;
    pstMotionHist->MIDDL_MOTION_NUM = pstMotionInput->pstMotionReg->IGLB_MTN_NUM1
                                      + pstMotionInput->pstMotionReg->IGLB_MTN_NUM2
                                      + pstMotionInput->pstMotionReg->IGLB_MTN_NUM3;
    pstMotionHist->MIDDL_MOTION_SUM = pstMotionInput->pstMotionReg->IGLB_MTN_SUM1
                                      + pstMotionInput->pstMotionReg->IGLB_MTN_SUM2
                                      + pstMotionInput->pstMotionReg->IGLB_MTN_SUM3;
    pstMotionHist->LARGE_MOTION_NUM = pstMotionInput->pstMotionReg->IGLB_MTN_NUM4;
    pstMotionHist->LARGE_MOTION_SUM = pstMotionInput->pstMotionReg->IGLB_MTN_SUM4;

    return HI_SUCCESS;

}

/* 获取隔行运动信息interlace globa motion开关状态 */
/* It does not exist in 98m_a and 98m; In 98C and HiFoneB2 it need this reg */
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo)
{
    HI_BOOL bIGlbEn;
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    bIGlbEn = pstVpssVirReg->VPSS_CTRL.bits.iglb_en;

    return bIGlbEn;
}

/* It does not exist in 98m_a and 98m; In 98C and HiFoneB2 it need this reg */
HI_S32 PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bIGlbEn)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL.bits.iglb_en = bIGlbEn;

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
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.die_demo_en = bDemoEn;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.die_demo_mode = (HI_U32)enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetVpssMktCoordinate(HI_U32 u32HandleNo, HI_U32 u32DbgPos)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL2.bits.market_coordinate = u32DbgPos;

    return HI_SUCCESS;
}


