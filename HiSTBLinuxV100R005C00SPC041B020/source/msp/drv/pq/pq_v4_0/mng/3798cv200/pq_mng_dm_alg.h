/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dm_alg.h
  Version       : Initial Draft
  Author        : zhangjing wangting
  Created       : 2016-03-02
  Description   : soft alg

******************************************************************************/

#ifndef __PQ_MNG_DM_ALG_H__
#define __PQ_MNG_DM_ALG_H__

#include "pq_mng_db_alg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    /* VPSS_DM_EDGE */
    HI_BOOL bDmEn;
    HI_U32 SDBWmode;
    HI_U32 StrIdxFlag;

    HI_U32 OppAngCtrstT;
    HI_U32 MNDirOppCtrstT;
    HI_U32 cSWTrsntLT;
    HI_U32 cSWTrsntLT10bit;
    HI_U32 LSWRatio;
    HI_U32 LimitLSWRatio;

    HI_S32 MMFLR;       /* signed */
    HI_S32 MMFLR10bit;  /* signed */
    HI_S32 MMFSR;       /* signed */
    HI_S32 MMFSR10bit;  /* signed */
    HI_U32 MMFlimitEn;

    HI_U32 MinLimValTrans;
    HI_U32 MinLimValTrans10bit;
    HI_U32 MinLimTransWid;
    HI_U32 MinLimTransWid10bit;
    HI_U32 LimTransWid;
    HI_U32 LimTransWid10bit;

    HI_U32 LimitT;
    HI_U32 LimitT10bit;
    HI_U32 LimResBlendStr1;
    HI_U32 LimResBlendStr2;
    HI_U32 DirBlendStr;

    HI_U32 DirOppCtrstT;
    HI_U32 DirOppCtrstT10bit;
    HI_U32 DirCtrstT;
    HI_U32 DirCtrstT10bit;
    HI_U32 SimDirPtCountT;

    HI_S32 dirMMFLR;       /* signed */
    HI_S32 dirMMFLR10bit;  /* signed */
    HI_S32 dirMMFSR;       /* signed */
    HI_S32 dirMMFSR10bit;  /* signed */
    HI_U32 DirMMFEn;

    HI_U32 LWCtrstT;
    HI_U32 LWCtrstT10bit;
    HI_U32 cSWTrsntST;
    HI_U32 cSWTrsntST10bit;

    HI_U32 u32GradSubRatio;
    HI_U32 u32CtrstThresh;
    HI_U32 u32OppAngCtrstDiv;
    HI_U32 DirStrenShiftFac;

    HI_U32 u32EdgeThr;
    HI_U32 u32EdgeGain1;
    HI_U32 u32EdgeGain2;
    HI_U32 u32AsymtrcGain;

    HI_U32 u32SWThrGain;
    HI_U32 u32DmGlobalStr;

    HI_U32 u32Dm_edge_thr;
    HI_U32 u32Dm_asymtrc_gain;
    HI_U32 u32Dm_edge_gain1;
    HI_U32 u32Dm_edge_gain2;
    HI_U32 u32Dm_sw_thr_gain;
    HI_U32 u32Dm_global_str;
    HI_U32 u32Dm_str_idx;

} DM_API_REG_S;

typedef struct
{
    HI_U16 TransBandL[31];
    HI_U16 TransBandM[31];

} DM_MMFLimTransBand;


typedef struct
{
    HI_U16 OppAngCtrstT;
    HI_U16 MNDirOppCtrstT;
    HI_U16 cSWTrsntLT;
    HI_U16 cSWTrsntLT10bit;
    HI_U8  LSWRatio;      //3,4,5,6
    HI_U8  LimitLSWRatio; //3,4,5,6

    HI_S16 MMFLR;       //MMFLR > MMFSR
    HI_S16 MMFLR10bit;  //MMFLR10bit > MMFSR10bit
    HI_S16 MMFSR;
    HI_S16 MMFSR10bit;

    /* for test */
    HI_U8 InitValStep; //0,1,2,3
    HI_U8 MMFSet;      //0:MMF[0,3],1:MMF[1,2]
    HI_U8 MMFlimitEn;  //1: enable the MMF limit condition; 0: disable

    HI_U16 LimitT;
    HI_U16 LimitT10bit;
    HI_U8  LimResBlendStr1;  //[0,8]  Res = (HI_U16)DM_Blend(ResLimitT, ResMMF, pReg->LimResBlendStr1, 8);
    HI_U8  LimResBlendStr2;  //[0,8]  Res = (HI_U16)DM_Blend(ResLimitT, Pix, pReg->LimResBlendStr2, 8);
    HI_U8  DirBlendStr;      //[0,8]  pTxt->DirStr = (HI_U8)DM_Blend(pTxt->DirStr,Gain,pReg->DirBlendStr,8);

    HI_U16 LWCtrstT;
    HI_U16 LWCtrstT10bit;
    HI_U16 cSWTrsntST;
    HI_U16 cSWTrsntST10bit;

    /* test */
    HI_U16 *MMFLimTransBand;

} DM_Para;


typedef struct
{
    /* software only */
    HI_U32 u32LumWidth;
    HI_U32 u32ChrWidth;
    HI_U32 u32LumHeight;
    HI_S32 s32RateBuf[32];
    HI_S32 s32RateSmthWin;
    HI_S32 s32LumHBlkDetNum;
    HI_S32 s32LumHBlkSize;   /* non-interface register [6,64] */
    HI_S32 *pNewRate;
} DM_INF_S;


/* Adjust DM params according to QP */
typedef struct
{
    HI_U16 OppAngCtrstT;
    HI_U16 MNDirOppCtrstT;
    HI_U16 cSWTrsntLT;
    HI_U16 cSWTrsntLT10bit;
    HI_U8  LSWRatio;       /* LSWRatio = 2,3,4 */
    HI_U8  LimitLSWRatio;  /* 3,4,5 */

    HI_S16 MMFLR;          /* MMFLR > MMFSR */
    HI_S16 MMFLR10bit;     /* MMFLR10bit > MMFSR10bit */
    HI_S16 MMFSR;
    HI_S16 MMFSR10bit;
    HI_U8  MMFlimitEn;     /* 1: enable the MMF limit condition; 0: disable */

    HI_U16 MinLimValTrans;
    HI_U16 MinLimValTrans10bit;
    HI_U16 MinLimTransWid;
    HI_U16 MinLimTransWid10bit;
    HI_U16 LimTransWid;     /* >=4 */
    HI_U16 LimTransWid10bit;

    HI_U16 LimitT;
    HI_U16 LimitT10bit;
    HI_U8  LimResBlendStr1; /* [0,8]  Res = (HI_U16)DM_Blend(ResLimitT, ResMMF, pReg->LimResBlendStr1, 8); */
    HI_U8  LimResBlendStr2; /* [0,8]  Res = (HI_U16)DM_Blend(ResLimitT, Pix,    pReg->LimResBlendStr2, 8); */
    HI_U8  DirBlendStr;     /* [0,8]  pTxt->DirStr = (HI_U8)DM_Blend(pTxt->DirStr,Gain,pReg->DirBlendStr,8); */

    HI_U16 DirOppCtrstT;
    HI_U16 DirOppCtrstT10bit;
    HI_U16 DirCtrstT;
    HI_U16 DirCtrstT10bit;
    HI_U8  SimDirPtCountT;

    HI_S16 dirMMFLR;       /* dirMMFLR > dirMMFSR */
    HI_S16 dirMMFLR10bit;  /* dirMMFLR10bit > dirMMFSR10bit */
    HI_S16 dirMMFSR;
    HI_S16 dirMMFSR10bit;
    HI_U8  DirMMFEn;       /* 1: enable the dir MMF condition; 0: disable */

    HI_U16 LWCtrstT;
    HI_U16 LWCtrstT10bit;
    HI_U16 cSWTrsntST;
    HI_U16 cSWTrsntST10bit;

} DM_QPPara;

/* DM QP Limits & Flag Lut of different encoders; Option I: */
typedef struct
{
    HI_U16 mndet_QPLimit[4];
    HI_U8  mndet_QPFlagLut[5];
} DM_QPLimitFlagLut;


HI_S32 PQ_MNG_ALG_UpdateDMCfg(DM_INF_S *pInf, DM_API_REG_S *pstDmApiReg, DB_CALC_INFO_S *pstDmCalcInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



