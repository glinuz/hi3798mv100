/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************
  File Name     :    pq_hal_mcnr.h
  Version       :     Initial Draft
  Author        :     w00273863
  Created       :   2015/11/25
  Description   :

******************************************************************************/
#ifndef __PQ_HAL_MCNR_H__
#define __PQ_HAL_MCNR_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/* 需根据视频timing信息帧级更新的寄存器参数*/
typedef struct hiMCNR_TIMING_PARAM
{
    /* Parameters for Logic Circut */
    HI_S32 Reg_ND_NoiseDetectEn;

    HI_S32 Reg_MD_MDPreLPFEn;

    HI_S32 Reg_ME_MotionEstimationEn;

    HI_S32 Reg_MC_YMCAdjEn;
    HI_S32 Reg_MC_CMCAdjEn;
    HI_S32 Reg_MC_SaltusDetectEn;

    HI_S32 Reg_MD_Alpha1;
    HI_S32 Reg_MD_Alpha2;

    HI_S32 Reg_MD_YMDGain;
    HI_S32 Reg_MD_YMDCore;

    HI_S32 Reg_MD_CMDGain;
    HI_S32 Reg_MD_CMDCore;

    HI_S32 Reg_ME_Adj_MV_Min; // [5 bits] {0~31}
    HI_S32 Reg_ME_Adj_MV_Max; // [5 bits] {0~31}

    HI_S32 Reg_ME_Mag_Pnl_Gain_0MV;
    HI_S32 Reg_ME_Mag_Pnl_Core_0MV;

    HI_S32 Reg_ME_Mag_Pnl_Gain_XMV;
    HI_S32 Reg_ME_Mag_Pnl_Core_XMV;

    HI_S32 Reg_ME_Std_Pnl_Gain;
    HI_S32 Reg_ME_Std_Pnl_Core;

    HI_S32 Reg_ME_Adj_0MV_Min;
    HI_S32 Reg_ME_Adj_0MV_Max;
    HI_S32 Reg_ME_Adj_XMV_Min;
    HI_S32 Reg_ME_Adj_XMV_Max;

    HI_S32 Reg_ME_Std_Core_0MV;
    HI_S32 Reg_ME_Std_Core_XMV;

    HI_S32 Reg_ME_Std_Pnl_Gain_0MV;
    HI_S32 Reg_ME_Std_Pnl_Core_0MV;
    HI_S32 Reg_ME_Std_Pnl_Gain_XMV;
    HI_S32 Reg_ME_Std_Pnl_Core_XMV;


    HI_S32 Reg_ME_GM_Adj;

    HI_S32 Reg_MC_NoiseLevel;
    HI_S32 Reg_MC_Scenechange;
    HI_S32 Reg_MC_SaltusLevel;
    HI_S32 Reg_MC_GlobalMotion;
    HI_S32 Reg_MC_ImageStillLevel;

    HI_S32 Reg_MC_YMotionGain;
    HI_S32 Reg_MC_YMotionCore;

    HI_S32 Reg_MC_CMotionGain;
    HI_S32 Reg_MC_CMotionCore;

    HI_S8 Reg_TF_YBlendingAlphaLUT[32];
    HI_S8 Reg_TF_CBlendingAlphaLUT[32];
} MCNR_TIMING_PARAM;


typedef struct hiSNR_OUTPUT_REG
{
    HI_S32  MotionAlpha;
    HI_S32  SceneChange;
} SNR_OUTPUT_REG;


HI_S32 PQ_HAL_UpdateTnrSoftReg(HI_U32 u32HandleNo, MCNR_TIMING_PARAM* pstMcApiParam);

HI_S32 PQ_HAL_UpdateSnrSoftReg(HI_U32 u32HandleNo, SNR_OUTPUT_REG*   pstSnrParam);





#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


