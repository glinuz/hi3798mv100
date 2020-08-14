/******************************************************************************
*
* Copyright (C) 2014 -2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
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
#include "drv_pq_table.h"



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



HI_S32 PQ_MNG_GetACMLumaTbl(HI_PQ_COLOR_LUT_S* pstColorData);
HI_S32 PQ_MNG_GetACMHueTbl(HI_PQ_COLOR_LUT_S* pstColorData);
HI_S32 PQ_MNG_GetACMSatTbl(HI_PQ_COLOR_LUT_S* pstColorData);
HI_S32 PQ_MNG_SetAcmLuma(HI_PQ_ACM_LUT_S* pstAttr);
HI_S32 PQ_MNG_SetAcmHue(HI_PQ_ACM_LUT_S* pstAttr);
HI_S32 PQ_MNG_SetAcmSat(HI_PQ_ACM_LUT_S* pstAttr);

HI_S32 PQ_MNG_RegisterACM(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterACM(HI_VOID);






#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
