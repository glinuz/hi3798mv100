/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_acm.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/4/22
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_ACM_H__
#define __PQ_MNG_ACM_H__

#include "hi_type.h"
#include "drv_pq_define.h"
#include "pq_hal_acm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* 肤色增益类型 */
typedef enum hiFLESHTONE_LEVEL_E
{
    FLE_GAIN_OFF = 0 ,
    FLE_GAIN_LOW     ,
    FLE_GAIN_MID     ,
    FLE_GAIN_HIGH    ,

    FLE_GAIN_BUTT
} FLESHTONE_LEVEL_E;

/* Color Gain Level */
typedef enum hiCOLOR_GAIN_LEVEL_E
{
    COLOR_GAIN_OFF = 0,
    COLOR_GAIN_LOW    ,
    COLOR_GAIN_MID    ,
    COLOR_GAIN_HIGH   ,

    COLOR_GAIN_BUTT
} COLOR_GAIN_LEVEL_E;

/* PQ Source Mode */
typedef enum hiPQ_ACM_EHANCE_GAIN_E
{
    ACM_EH_GAIN_LUMA = 0,
    ACM_EH_GAIN_HUE     ,
    ACM_EH_GAIN_SAT     ,

    ACM_EH_GAIN_BUTT
} PQ_ACM_EHANCE_GAIN_E;

/* 六基色偏移值 */
typedef struct hiSIX_BASE_COLOR_OFFSET_S
{
    HI_U32  u32RedOffset;
    HI_U32  u32GreenOffset;
    HI_U32  u32BlueOffset;
    HI_U32  u32CyanOffset;
    HI_U32  u32MagentaOffset;
    HI_U32  u32YellowOffset;

} SIX_BASE_COLOR_OFFSET_S;

/* 颜色增强类型 */
typedef enum hiCOLOR_SPEC_MODE_E
{
    COLOR_MODE_RECOMMEND = 0  , /* 推荐的颜色增强模式   */
    COLOR_MODE_BLUE           , /* 固定的蓝色增强模式   */
    COLOR_MODE_GREEN          , /* 固定的绿色增强模式   */
    COLOR_MODE_BG             , /* 固定的蓝绿色增强模式 */
    COLOR_MODE_ORIGINAL       , /* 原始颜色模式 */

    COLOR_MODE_BUTT
} COLOR_SPEC_MODE_E;

/*ACM 增强类型*/
typedef enum hiACM_ENHANCE_E
{
    ACM_ENHANCE_FLESHTONE = 0,    /* 肤色增强 */
    ACM_ENHANCE_SIX_BASE,         /* 六基色增强,自定义颜色的增强 */
    ACM_ENHANCE_SPEC_COLOR_MODE,  /* 固定模式的颜色增强模式 */
    ACM_ENHANCE_BUTT
} ACM_ENHANCE_E;


/* ACM GAIN 消息结构 */
typedef struct hiCOLOR_GAIN_S
{
    HI_U32 u32GainMode;   /* 0:SD;1:HD;2:UHD */
    HI_U32 u32Gainluma;   /* 表示对Hue的增益, 范围0-1023 */
    HI_U32 u32Gainhue;    /* 表示对Hue的增益, 范围0-1023 */
    HI_U32 u32Gainsat;    /* 表示对Luma的增益 */

} COLOR_GAIN_S;


/* 去初始化Color */
HI_S32 PQ_MNG_DeInitColor(HI_VOID);
/* 初始化Color模块 */
HI_S32 PQ_MNG_InitColor(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault);
/* 获取Color增益 */
HI_S32 PQ_MNG_GetColorGainLevel(HI_U32* pu32ColorLevel);
/* Color增强增益控制 */
HI_S32 PQ_MNG_SetColorGainLevel(HI_U32 enGainLevel);
/* 获取肤色增益 */
HI_S32 PQ_MNG_GetFleshToneLevel(HI_U32* pu32FleshToneLevel);
/* 肤色增益控制 */
HI_S32 PQ_MNG_SetFleshToneLevel(FLESHTONE_LEVEL_E enGainLevel, HI_U32 u32SourceMode);
/* 六基色控制设置 */
HI_S32 PQ_MNG_SetSixBaseColorLevel(SIX_BASE_COLOR_OFFSET_S* pstSixBaseColorOffset, HI_U32 u32SourceMode);
/* 获取六基色设置值 */
HI_S32 PQ_MNG_GetSixBaseColorLevel(SIX_BASE_COLOR_OFFSET_S* pstSixBaseColorOffset);
/* 开关卖场模式 */
HI_S32 PQ_MNG_EnableACMDemo(HI_BOOL bOnOff);
HI_S32 PQ_MNG_SetACMDemoMode(ACM_DEMO_MODE_E enMode);
HI_S32 PQ_MNG_GetACMDemoMode(ACM_DEMO_MODE_E* enMode);
/* 打开ACM模块 */
HI_S32 PQ_MNG_EnableColorEnhance(HI_BOOL bOnOff);
/* 获取颜色增强模式 */
HI_S32 PQ_MNG_GetColorEnhanceMode(HI_U32* pu32ColorEnhanceMode);
/* 颜色增强模式设置: enColorSpecMode: 0-RECOMMEND;1-BLUE;2-GREEN;3-BG */
HI_S32 PQ_MNG_SetColorEnhanceMode(COLOR_SPEC_MODE_E  enColorSpecMode, HI_U32 u32SourceMode);

HI_S32 PQ_MNG_SetACMGain(HI_U32 u32SourceMode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
