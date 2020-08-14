/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_acm.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/09/11
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_ACM_H__
#define __PQ_HAL_ACM_H__

#include "hi_type.h"
#include "hi_drv_mmz.h"
#include "pq_hal_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Gain Address */
#define ACM_GAIN_ADDR      0xf8cc41a4
#define ACM_GAIN_LUMA_LSB  20
#define ACM_GAIN_LUMA_MSB  29
#define ACM_GAIN_HUE_LSB   10
#define ACM_GAIN_HUE_MSB   19
#define ACM_GAIN_SAT_LSB   0
#define ACM_GAIN_SAT_MSB   9

/* ACM Convert */
#define LUT0_PIXEL_NUM 525
#define LUT1_PIXEL_NUM 420
#define LUT2_PIXEL_NUM 450
#define LUT3_PIXEL_NUM 360
#define LUT4_PIXEL_NUM 490
#define LUT5_PIXEL_NUM 392
#define LUT6_PIXEL_NUM 420
#define LUT7_PIXEL_NUM 336


/* ACM table结构 */
typedef struct hiCOLOR_LUT_S
{
    HI_S16 as16Color[9][13][29];  /* ACM 查找表 */
} COLOR_LUT_S;

typedef struct hiCOLOR_ACM_LUT_S
{
    HI_S16 as16Luma[9][13][29];   /* Luma查找表 */
    HI_S16 as16Hue[9][13][29];    /* Hue查找表 */
    HI_S16 as16Sat[9][13][29];    /* Saturation查找表 */
} COLOR_ACM_LUT_S;

/* 六基色映射表 */
typedef struct hiHUE_RANG_S
{
    HI_U32  u32HueStart;
    HI_U32  u32HueEnd;
} HUE_RANG_S;

/* 六基色类型 */
typedef enum hiSIX_BASE_COLOR_E
{
    SIX_BASE_COLOR_R       , /* 红 */
    SIX_BASE_COLOR_G       , /* 绿 */
    SIX_BASE_COLOR_B1      , /* 蓝 */
    SIX_BASE_COLOR_B2      , /* 蓝 */
    SIX_BASE_COLOR_CYAN    , /* 青 */
    SIX_BASE_COLOR_MAGENTA , /* 紫 */
    SIX_BASE_COLOR_YELLOW  , /* 黄 */
    SIX_BASE_COLOR_ALL     ,

    SIX_BASE_COLOR_BUTT
} SIX_BASE_COLOR_E;

/* ACM demo mode */
typedef enum hiACM_DEMO_MODE_E
{
    ACM_DEMO_ENABLE_R = 0 ,
    ACM_DEMO_ENABLE_L     ,

} ACM_DEMO_MODE_E;

/*------------- ACM Convert Start ---------*/
typedef struct acm_split_Table
{
    HI_S32 LUT0[LUT0_PIXEL_NUM];
    HI_S32 LUT1[LUT1_PIXEL_NUM];
    HI_S32 LUT2[LUT2_PIXEL_NUM];
    HI_S32 LUT3[LUT3_PIXEL_NUM];
    HI_S32 LUT4[LUT4_PIXEL_NUM];
    HI_S32 LUT5[LUT5_PIXEL_NUM];
    HI_S32 LUT6[LUT6_PIXEL_NUM];
    HI_S32 LUT7[LUT7_PIXEL_NUM];
} ACM_SPLIT_TABLE;

typedef struct llllong
{
    HI_U32 u32Data[4];
} LLLONG_S;

typedef struct acm_convert_Table
{
    LLLONG_S LUT0_CONF[132];
    LLLONG_S LUT1_CONF[105];
    LLLONG_S LUT2_CONF[113];
    LLLONG_S LUT3_CONF[90];
    LLLONG_S LUT4_CONF[123];
    LLLONG_S LUT5_CONF[98];
    LLLONG_S LUT6_CONF[105];
    LLLONG_S LUT7_CONF[84];
} ACM_CONVERT_TABLE;
/*------------- ACM Convert End ---------*/


/* DeInit ACM */
HI_S32  PQ_HAL_DeInitACM(HI_VOID);
/* Init ACM */
HI_S32  PQ_HAL_InitACM(HI_VOID);
/* Set Color Para */
HI_S32  PQ_HAL_SetACMParam(COLOR_ACM_LUT_S* pstColorTable);
/* Enable Acm Demo */
HI_VOID PQ_HAL_EnableACMDemo(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff);
HI_VOID PQ_HAL_SetACMDemoMode(PQ_HAL_LAYER_VP_E u32ChId, ACM_DEMO_MODE_E enMode);
/* Set ACM Enable */
HI_VOID PQ_HAL_EnableACM(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff);
/* Get ACM Enable Flag */
HI_S32  PQ_HAL_GetACMEnableFlag(HI_BOOL* bOnOff);
HI_VOID PQ_HAL_SetACMCbcrThd(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data);
HI_VOID PQ_HAL_SetACMStretch(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data);
HI_VOID PQ_HAL_SetACMClipRange(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Data);
HI_VOID PQ_HAL_SetACMGain(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Luma, HI_U32 u32Hue, HI_U32 u32Sat);
HI_VOID PQ_HAL_GetACMGain(PQ_HAL_LAYER_VP_E u32ChId, HI_U32* pu32Luma, HI_U32* pu32Hue, HI_U32* pu32Sat);
HI_S32  PQ_HAL_SetACMLumaTbl(COLOR_LUT_S* pstColorData);
HI_S32  PQ_HAL_SetACMHueTbl(COLOR_LUT_S* pstColorData);
HI_S32  PQ_HAL_SetACMSatTbl(COLOR_LUT_S* pstColorData);
HI_S32  PQ_HAL_GetACMLumaTbl(COLOR_LUT_S* pstColorData);
HI_S32  PQ_HAL_GetACMHueTbl(COLOR_LUT_S* pstColorData);
HI_S32  PQ_HAL_GetACMSatTbl(COLOR_LUT_S* pstColorData);
HI_VOID PQ_HAL_UpdatACMCoef(PQ_HAL_LAYER_VP_E u32ChId);
HI_VOID PQ_HAL_SetACMDemoPos(PQ_HAL_LAYER_VP_E u32ChId, HI_U32 u32Pos);
HI_VOID PQ_HAL_GetACMDemoPos(PQ_HAL_LAYER_VP_E u32ChId, HI_U32* u32Pos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
