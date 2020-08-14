/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_mcnr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#include "pq_hal_mcnr.h"
#include "hi_math.h"

HI_S32 PQ_HAL_UpdateTnrSoftReg(HI_U32 u32HandleNo, MCNR_TIMING_PARAM* pstMcApiParam)
{
    S_VPSS_REGS_TYPE* pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_TNR_MAND_CTRL.bits.noisedetecten    =  pstMcApiParam->Reg_ND_NoiseDetectEn     ;
    pstVpssVirReg->VPSS_TNR_MAMD_CTRL.bits.mdprelpfen       =  pstMcApiParam->Reg_MD_MDPreLPFEn        ;
    pstVpssVirReg->VPSS_TNR_MAME_CTRL.bits.motionestimateen =  pstMcApiParam->Reg_ME_MotionEstimationEn;
    pstVpssVirReg->VPSS_TNR_MAMC_MCADJ.bits.ymcadjen        =  pstMcApiParam->Reg_MC_YMCAdjEn          ;
    pstVpssVirReg->VPSS_TNR_MAMC_MCADJ.bits.cmcadjen        =  pstMcApiParam->Reg_MC_CMCAdjEn          ;
    pstVpssVirReg->VPSS_TNR_MAMC_SALTUS.bits.saltusdecten   =  pstMcApiParam->Reg_MC_SaltusDetectEn    ;

    pstVpssVirReg->VPSS_TNR_SCENE_CHANGE.bits.scenechangeinfo  =  pstMcApiParam->Reg_MC_Scenechange;
    pstVpssVirReg->VPSS_TNR_MAMC_SALTUS.bits.saltuslevel       = pstMcApiParam->Reg_MC_SaltusLevel;

    //   pstVpssVirReg->VPSS_TNR_MAMC_MAP.bits.cmotioncore          = pstMcApiParam->Reg_MC_CMotionCore;
    //   pstVpssVirReg->VPSS_TNR_MAMC_MAP.bits.cmotiongain          = pstMcApiParam->Reg_MC_CMotionGain;
    //   pstVpssVirReg->VPSS_TNR_MAMC_MAP.bits.ymotioncore          = pstMcApiParam->Reg_MC_YMotionCore;
    //   pstVpssVirReg->VPSS_TNR_MAMC_MAP.bits.ymotiongain          = pstMcApiParam->Reg_MC_YMotionGain; 
	
    pstVpssVirReg->VPSS_TNR_MAMD_CTRL.bits.alpha1              = pstMcApiParam->Reg_MD_Alpha1;
    pstVpssVirReg->VPSS_TNR_MAMD_CTRL.bits.alpha2              = pstMcApiParam->Reg_MD_Alpha2;
    pstVpssVirReg->VPSS_TNR_MAME_CTRL.bits.gm_adj              = pstMcApiParam->Reg_ME_GM_Adj;

    pstVpssVirReg->VPSS_TNR_MAMD_GAIN.bits.ymdgain                    = pstMcApiParam->Reg_MD_YMDGain               ;
    pstVpssVirReg->VPSS_TNR_MAMD_GAIN.bits.ymdcore                    = pstMcApiParam->Reg_MD_YMDCore               ;
    pstVpssVirReg->VPSS_TNR_MAMD_GAIN.bits.cmdgain                    = pstMcApiParam->Reg_MD_CMDGain               ;
    pstVpssVirReg->VPSS_TNR_MAMD_GAIN.bits.cmdcore                    = pstMcApiParam->Reg_MD_CMDCore               ;

    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_0mv_min              = pstMcApiParam->Reg_ME_Adj_0MV_Min           ;
    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_xmv_min              = pstMcApiParam->Reg_ME_Adj_XMV_Min           ;
    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_0mv_max              = pstMcApiParam->Reg_ME_Adj_0MV_Max           ;
    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_xmv_max              = pstMcApiParam->Reg_ME_Adj_XMV_Max           ;
    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_mv_max               = pstMcApiParam->Reg_ME_Adj_MV_Max            ;
    pstVpssVirReg->VPSS_TNR_MAME_MV_THD.bits.adj_mv_min               = pstMcApiParam->Reg_ME_Adj_MV_Min            ;

    pstVpssVirReg->VPSS_TNR_MAME_GAIN.bits.mag_pnl_gain_0mv           = pstMcApiParam->Reg_ME_Mag_Pnl_Gain_0MV      ;
    pstVpssVirReg->VPSS_TNR_MAME_GAIN.bits.mag_pnl_core_0mv           = pstMcApiParam->Reg_ME_Mag_Pnl_Core_0MV      ;
    pstVpssVirReg->VPSS_TNR_MAME_GAIN.bits.mag_pnl_gain_xmv           = pstMcApiParam->Reg_ME_Mag_Pnl_Gain_XMV      ;
    pstVpssVirReg->VPSS_TNR_MAME_GAIN.bits.mag_pnl_core_xmv           = pstMcApiParam->Reg_ME_Mag_Pnl_Core_XMV      ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_pnl_gain_0mv       = pstMcApiParam->Reg_ME_Std_Pnl_Gain_0MV      ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_pnl_core_0mv       = pstMcApiParam->Reg_ME_Std_Pnl_Core_0MV      ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_pnl_gain_xmv       = pstMcApiParam->Reg_ME_Std_Pnl_Gain_XMV      ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_pnl_core_xmv       = pstMcApiParam->Reg_ME_Std_Pnl_Core_XMV      ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_core_0mv           = pstMcApiParam->Reg_ME_Std_Core_0MV          ;
    pstVpssVirReg->VPSS_TNR_MAME_STD_CORE.bits.std_core_xmv           = pstMcApiParam->Reg_ME_Std_Core_XMV          ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut0    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[0]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut1    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[1]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut2    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[2]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut3    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[3]   ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut4    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[4]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut5    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[5]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut6    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[6]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut7    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[7]   ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut8    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[8]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut9    = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[9]   ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut10   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[10]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut11   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[11]  ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut12   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[12]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut13   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[13]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut14   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[14]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut15   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[15]  ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut16   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[16]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut17   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[17]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut18   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[18]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut19   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[19]  ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut20   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[20]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut21   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[21]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut22   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[22]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut23   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[23]  ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut24   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[24]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut25   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[25]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut26   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[26]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut27   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[27]  ;

    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut28   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[28]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut29   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[29]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut30   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[30]  ;
    pstVpssVirReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut31   = pstMcApiParam->Reg_TF_CBlendingAlphaLUT[31]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut0    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[0]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut1    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[1]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut2    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[2]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut3    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[3]   ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut4    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[4]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut5    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[5]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut6    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[6]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut7    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[7]   ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut8    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[8]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut9    = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[9]   ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut10   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[10]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut11   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[11]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut12   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[12]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut13   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[13]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut14   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[14]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut15   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[15]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut16   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[16]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut17   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[17]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut18   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[18]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut19   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[19]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut20   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[20]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut21   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[21]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut22   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[22]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut23   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[23]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut24   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[24]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut25   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[25]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut26   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[26]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut27   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[27]  ;

    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut28   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[28]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut29   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[29]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut30   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[30]  ;
    pstVpssVirReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut31   = pstMcApiParam->Reg_TF_YBlendingAlphaLUT[31]  ;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_UpdateSnrSoftReg(HI_U32 u32HandleNo, SNR_OUTPUT_REG* pstSnrParam)
{
    S_VPSS_REGS_TYPE* pstVpssVirReg = HI_NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);

    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_SNR_MT.bits.motionalpha = pstSnrParam->MotionAlpha;
    pstVpssVirReg->VPSS_TNR_SCENE_CHANGE.bits.scenechangeinfo = pstSnrParam->SceneChange;

    return HI_SUCCESS;
}



