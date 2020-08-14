/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_dm.h
  Version       : Initial Draft
  Author        :
  Created       : 2015/10/09
  Description   : De-Mosquito

******************************************************************************/

#ifndef __PQ_HAL_DM_H__
#define __PQ_HAL_DM_H__


#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_dm_alg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* 定义DR模块中寄存器所使用到的参数值 */
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

    HI_U8 u8GradSubRatio; //u5,[0,31];   default value:24
    HI_U8 u8CtrstThresh; //u2,[0,3];     default value:2

    HI_U8 const* pu8DirStrGainLut; //u4,[0,8];  default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
    HI_U8 const* pu8DirStrLut;  //u4,[0,8];    default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
    //original codes
    HI_U8 const* pu8SwWhtLut; //u7,[0,127];  default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c

    HI_U8 u8DmGlobalStr; //u4,[0,15];     default value:8
    //original codes
    //HI_U8 StrIdxFlag;
    HI_U16 LimitT;      //u8,[0,255];   default value:18
    HI_U16 LimitT10bit; //u10,[0,1023];  default value:72

    HI_U8 u8OppAngCtrstDiv;  //u2,[1,3];  default value:2
    HI_U16 OppAngCtrstT;  //u8,[0,255];   default value:20

    //original codes
    HI_U16 MNDirOppCtrstT; //u8,[0,255];  default value:18
    HI_U8 DirBlendStr; //u4,[0,8];  default value:7;      pTxt->DirStr = (HI_U8)DM_Blend(pTxt->DirStr,Gain,pReg->DirBlendStr,8);

    HI_U16 LWCtrstT; //u8,[0,255];  default value:30
    HI_U16 LWCtrstT10bit; //u10,[0,1023];  default value:120
    HI_U16 cSWTrsntLT; //u8,[0,255];   default value:40
    HI_U16 cSWTrsntLT10bit;  //u10,[0,1023];  default value:160
    HI_U16 cSWTrsntST; //u8,[0,255]; default value:0
    HI_U16 cSWTrsntST10bit; //u10,[0,1023]; default value:0

    //original codes
    //SD big window process mode: 0: interlace mode; 1: progressive mode
    //HI_U8 SDBWmode;

    //for MMF limit
    HI_S16 MMFLR;           //s9,[-256,255];      default value:50;      MMFLR > MMFSR
    HI_S16 MMFLR10bit;      //s11,[-1024,1023];    default value:200;    MMFLR10bit > MMFSR10bit
    HI_S16 MMFSR;       //s9,[-256,255];     default value:0
    HI_S16 MMFSR10bit;  //s11,[-1024,1023];     default value:0

    HI_U8 MMFlimitEn; //u1,[0,1];     default value:1;     1: enable the MMF limit condition; 0: disable
    //test
    HI_U8 MMFSet;  //u1,[0,1];    default value:0;     0:MMF[0,3], 1:MMF[1,2]

    HI_U8 LSWRatio;  //u3,[2,6];     default value:4
    HI_U8 LimitLSWRatio; //u3,[2,6];   default value:4

    //for MMF filtering blending
    HI_U8 LimResBlendStr1;    //u4,[0,8];  default value:7;  Res = (HI_U16)DM_Blend(ResLimitT, ResMMF, pReg->LimResBlendStr1, 8);
    HI_U8 LimResBlendStr2;    //u4,[0,8];  default value:7;  Res = (HI_U16)DM_Blend(ResLimitT, Pix, pReg->LimResBlendStr2, 8);

    //for test
    HI_U8 InitValStep;     //u2,[0,3];     default value:0

    //test
    HI_U16 const* pTransBand;  //u6,[0,63];   default value @ HI_S32 DM_InitReg(DM_INF_S *pInf) function in dm_api.c
} DM_PARAM_S;


HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8]);
HI_S32 PQ_HAL_SetReg_Dm_Sw_Wht_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[5]);
HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_EnableDM(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


