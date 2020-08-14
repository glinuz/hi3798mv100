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
  Author        : pengjunwei
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
    HI_S32  BitDepth;
    HI_S32  YWidth;
    HI_S32  YHeight;
    HI_S32  YStride;
    HI_U16* pYInBuf;
    HI_U16* pYOutBuf;
    HI_U16* pUInBuf;
    HI_U16* pUOutBuf;
    HI_U16* pVInBuf;
    HI_U16* pVOutBuf;

    HI_S32  DmEn;  //u1,[0,1];   default value:1

    HI_U8 u8GradSubRatio;     //u5,[0,31]; default value:24
    HI_U8 u8CtrstThresh;      //u2,[0,3];  default value:2
    HI_U8 u8DirStrGainLut[8]; //u4,[0,8];  default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
    HI_U8 u8DirStrLut[16];    //u4,[0,8];  default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
    HI_U8 u8SwWhtLut[5]; //u7,[0,127]; default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
    HI_U8 u8DmGlobalStr; //u4,[0,15];  default value:8

    HI_U16 LimitT;      //u8,[0,255];   default value:18
    HI_U16 LimitT10bit; //u10,[0,1023]; default value:72

    HI_U8 u8OppAngCtrstDiv; //u2,[1,3];   default value:2
    HI_U16 OppAngCtrstT;    //u8,[0,255]; default value:20
    HI_U16 MNDirOppCtrstT; //u8,[0,255]; default value:18
    HI_U8 DirBlendStr;     //u4,[0,8];   default value:7; pTxt->DirStr = (HI_U8)DM_Blend(pTxt->DirStr,Gain,pReg->DirBlendStr,8);

    HI_U16 LWCtrstT;        //u8,[0,255];   default value:30
    HI_U16 LWCtrstT10bit;   //u10,[0,1023]; default value:120
    HI_U16 cSWTrsntLT;      //u8,[0,255];   default value:40
    HI_U16 cSWTrsntLT10bit; //u10,[0,1023]; default value:160
    HI_U16 cSWTrsntST;      //u8,[0,255];   default value:0
    HI_U16 cSWTrsntST10bit; //u10,[0,1023]; default value:0

    /* for MMF limit */
    HI_S16 MMFLR;       //s9, [-256,255];    default value:50;  MMFLR > MMFSR
    HI_S16 MMFLR10bit;  //s11,[-1024,1023];  default value:200; MMFLR10bit > MMFSR10bit
    HI_S16 MMFSR;       //s9, [-256,255];    default value:0
    HI_S16 MMFSR10bit;  //s11,[-1024,1023];  default value:0

    HI_U8 MMFlimitEn; //u1,[0,1];  default value:1;  1: enable the MMF limit condition; 0: disable
    HI_U8 MMFSet;     //u1,[0,1];  default value:0;  0: MMF[0,3], 1: MMF[1,2]

    HI_U8 LSWRatio;      //u3,[2,6]; default value:4
    HI_U8 LimitLSWRatio; //u3,[2,6]; default value:4

    /* for MMF filtering blending */
    HI_U8 LimResBlendStr1;    //u4,[0,8];  default value:7;  Res = (HI_U16)DM_Blend(ResLimitT, ResMMF, pReg->LimResBlendStr1, 8);
    HI_U8 LimResBlendStr2;    //u4,[0,8];  default value:7;  Res = (HI_U16)DM_Blend(ResLimitT, Pix, pReg->LimResBlendStr2, 8);

    /* for test */
    HI_U8 InitValStep;     //u2,[0,3];   default value:0
    HI_U16 TransBand[31];  //u6,[0,63];  default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
} DM_API_REG_S;

typedef struct
{
    HI_U16 TransBandL[31];
    HI_U16 TransBandM[31];

} DM_MMFLimTransBand;


/* DM Limits & Flag Lut Option I: */
typedef struct
{
    HI_U16 mndet_LimitLut[4];
    HI_U8 mndet_FlagLut[5];
} DM_LimitFlagLut;

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
    HI_U16* MMFLimTransBand;

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
    HI_S32* pNewRate;
    HI_U32 ImgGlobalAdjustEn;
    HI_S32 YWidth;

    HI_U8  DefaultDMParaFlag; //u,[0,3]; default value:1
    HI_U8  DMmndetEn;         //u,[0,1]; default value:1

    DM_LimitFlagLut LimitFlagLutSD;
    DM_Para DMParaSD[4];
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

typedef struct
{
    HI_U8 MPEG2_RatIdxLut[16];
    HI_U8 H264_RatIdxLut[16];
    HI_U8 H265_RatIdxLut[16];
    HI_U8 VP9_RatIdxLut[16];
    HI_U8 mndet_RatIdxLut[16];
    HI_U8 MV410_RatIdxLut[16];
} DM_RatIdxLut;

typedef struct
{
    HI_U16 OppAngCtrstT;
    HI_U16 MNDirOppCtrstT;
    HI_U16 LimitT;
    HI_U16 LimitT10bit;
    HI_U8  DirBlendStr;
} DM_StrIdxParam;

/* DM QP Limits & Flag Lut of different encoders; Option I: */
typedef struct
{
    HI_U16 MPEG2_QPLimit[4];
    HI_U8  MPEG2_QPFlagLut[5];
    HI_U16 H264_QPLimit[4];
    HI_U8  H264_QPFlagLut[5];
    HI_U16 H265_QPLimit[4];
    HI_U8  H265_QPFlagLut[5];
    HI_U16 VP9_QPLimit[4];
    HI_U8  VP9_QPFlagLut[5];
    HI_U16 mndet_QPLimit[4];
    HI_U8  mndet_QPFlagLut[5];
} DM_QPLimitFlagLut;

typedef struct
{
    HI_U8 GradSubRatio;
    HI_U8 u8CtrstThresh;
    HI_U8 u8EdgeThr;
    HI_U8 u8EdgeGain1;
    HI_U8 u8EdgeGain2;
    HI_U8 u8SWThrGain;
} DM_DynmcPara;

HI_S32 PQ_MNG_ALG_UpdateDMCfg(DM_INF_S* pInf, DM_API_REG_S* pstDmApiReg, DB_CALC_INFO_S* pstDmCalcInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



