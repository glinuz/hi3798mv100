/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_mcnr.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#ifndef __PQ_MNG_MCNR_H__
#define __PQ_MNG_MCNR_H__

#include "hi_type.h"
#include "drv_pq_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiMCNR_INPUT_INFO
{
    HI_S32 CurFNum; /* number of current field/frame after scene changed */
    HI_S32 YWidth;
    HI_S32 YHeight;

    HI_S32 Cfg_InputPort;
    HI_S32 SceneChange;

    /* CFG */
    HI_S32 Cfg_ND_NoiseDetectEn;
    HI_S32 CFG_ND_ZeroNoiseCntEn;

    HI_S32 Cfg_ND_FlatInfoXMin;
    HI_S32 Cfg_ND_FlatInfoXMax;
    HI_S32 Cfg_ND_FlatInfoYMin;
    HI_S32 Cfg_ND_FlatInfoYMax;

    HI_S32 Cfg_MD_YMDGain;
    HI_S32 Cfg_MD_YMDCore;

    HI_S32 Cfg_MD_CMDGain;
    HI_S32 Cfg_MD_CMDCore;

    HI_S32 Cfg_ME_Adj_MV_Max;
    HI_S32 Cfg_ME_Adj_MV_Min;

    HI_S32 Cfg_ME_MotionEstimationEn;

    HI_S32 wRGNum;
    HI_S32 hRGNum;

    HI_S32* pRGMV_mag;

    HI_S32 Cfg_MC_AutoMotionMappingEn;

    HI_S32 Cfg_MC_YMotionStrXMin;
    HI_S32 Cfg_MC_YMotionStrXMax;
    HI_S32 Cfg_MC_YMotionStrYMin;
    HI_S32 Cfg_MC_YMotionStrYMax;

    HI_S32 Cfg_MC_CMotionStrXMin;
    HI_S32 Cfg_MC_CMotionStrXMax;
    HI_S32 Cfg_MC_CMotionStrYMin;
    HI_S32 Cfg_MC_CMotionStrYMax;

    HI_S32 Cfg_MC_YMotionGain;
    HI_S32 Cfg_MC_YMotionCore;

    HI_S32 Cfg_MC_CMotionGain;
    HI_S32 Cfg_MC_CMotionCore;

    HI_S32 Cfg_TF_AutoMotionAlphaEn;

    HI_S32 Cfg_TF_YBlendingXMin ;
    HI_S32 Cfg_TF_YBlendingXMax ;
    HI_S32 Cfg_TF_YBlendingYMin ;
    HI_S32 Cfg_TF_YBlendingYMax ;
    HI_S32 Cfg_TF_YBlendingMin  ;

    HI_S32 Cfg_TF_CBlendingXMin ;
    HI_S32 Cfg_TF_CBlendingXMax ;
    HI_S32 Cfg_TF_CBlendingYMin ;
    HI_S32 Cfg_TF_CBlendingYMax ;
    HI_S32 Cfg_TF_CBlendingMin  ;

    HI_S32 Cfg_TF_DTBlendingXMin;
    HI_S32 Cfg_TF_DTBlendingXMax;
    HI_S32 Cfg_TF_DTBlendingYMin;
    HI_S32 Cfg_TF_DTBlendingYMax;
} MCNR_INPUT_INFO;

typedef struct hiMCNR_INPUT_REG
{
    HI_S32 Reg_ND_NoisePointCntMax;
    HI_S32 Reg_ND_NoisePointCntMed;
    HI_S32 Reg_ND_NoisePointCntMin;

    HI_S32 Reg_ND_SumNoiseInfoMax;
    HI_S32 Reg_ND_SumNoiseInfoMed;
    HI_S32 Reg_ND_SumNoiseInfoMin;

} MCNR_INPUT_REG;

typedef struct hiMCNR_TIMING_INFO
{
    HI_S32 Inf_FrameNum_after_Scenechange;
    /* Parameters for SDK */
    HI_S32 Inf_ND_rawNoiseLevel;
    HI_S32 Inf_ND_TextureDenseLevel;
    HI_S32 Inf_ND_NoiseLevel;
    HI_S32 Inf_ND_preNoiseLevel[3];//double

    HI_S32 Inf_ME_SaltusFlag;
    HI_S32 Inf_ME_preGlobalMotion[4];   //double
    HI_S32 Inf_ME_preImageStillLevel[3];//double
    HI_S32 Inf_ME_preSaltusLevel[3];    //double

    HI_S32 Inf_ME_SaltusLevel;
    HI_S32 Inf_ME_ImageStillLevel;
    HI_S32 Inf_ME_GlobalMotion;
	HI_S32 Inf_ME_NonRgmeLevel;
} MCNR_TIMING_INFO;

/* RGMV  DDR 信息结构 */
typedef struct hiMCMVSTRUCT
{
    /* RGMV信息,64 bit */
    HI_S32 x;         //[8:0]
    HI_U32 sad;       //[9:0]
    HI_U32 mag;       //[9:0]
    HI_S32 vstillsad; //[9:0]
    HI_S32 mvy;       //[7:0]
    HI_U32 vsad;      //[9:0]
    HI_U32 ls;        //[3:0]
} MCMVSTRUCT;

typedef struct hiSNR_INPUT_INFO
{
    HI_S32  rawNoiseLevel;
    HI_S32  NoiseLevel;
    HI_S32  GlobalMotionVal;
    HI_S32  SceneChange;
} SNR_INPUT_INFO;

HI_S32 PQ_MNG_UpdateMCNRCfg(HI_PQ_WBC_INFO_S* pstVpssWbcInfo);

HI_S32 PQ_MNG_EnableMCNR(HI_BOOL bOnOff);

HI_S32 PQ_MNG_GetMCNREnable(HI_BOOL* pbOnOff);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*End of #ifndef __PQ_MNG_MCNR_H__ */



