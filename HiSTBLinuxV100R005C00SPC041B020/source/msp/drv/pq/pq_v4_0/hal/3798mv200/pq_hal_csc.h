/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_csc.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/09/18
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_CSC_H__
#define __PQ_HAL_CSC_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/* CSC Tuning Or No Tuning */
typedef enum hiHAL_CSC_TYPE_E
{
    HAL_CSC_TUNING_V0 = 0  ,
    HAL_CSC_TUNING_V1      ,
    HAL_CSC_TUNING_V2      ,
    HAL_CSC_TUNING_V3      ,
    HAL_CSC_TUNING_V4      ,
    HAL_CSC_TUNING_GP0     ,
    HAL_CSC_TUNING_GP1     ,
    HAL_CSC_NORMAL_SETTING ,

    HAL_CSC_TYPE_BUUT
} HAL_PQ_CSC_TYPE_E;

/* color space convert type */
typedef enum hiCSC_MODE_E
{
    HAL_CSC_YUV2RGB_601 = 0      , /* YUV_601 L  -> RGB        */
    HAL_CSC_YUV2RGB_709          , /* YUV_709 L  -> RGB        */
    HAL_CSC_RGB2YUV_601          , /* RGB        -> YUV_601 L  */
    HAL_CSC_RGB2YUV_709          , /* RGB        -> YUV_709 L  */
    HAL_CSC_YUV2YUV_709_601      , /* YUV_709 L  -> YUV_601 L  */
    HAL_CSC_YUV2YUV_601_709      , /* YUV_601 L  -> YUV_709 L  */
    HAL_CSC_YUV2YUV              , /* YUV L      -> YUV L      */

    HAL_CSC_RGB2RGB_709_2020     , /* RGB_709    -> RGB_2020   */
    HAL_CSC_RGB2YUV_2020_2020_L  , /* RGB_2020   -> YUV_2020 L */
    HAL_CSC_RGB2RGB_2020_709     , /* RGB_2020   -> RGB_709    */
    HAL_CSC_RGB2YUV_2020_709_L   , /* RGB_2020   -> YUV_601 L  */

    HAL_CSC_RGB2RGB_601_2020     , /* RGB_601    -> RGB_2020   */
    HAL_CSC_RGB2RGB_2020_601     , /* RGB_2020   -> RGB_601    */
    HAL_CSC_RGB2YUV_2020_601_L   , /* RGB_2020   -> YUV_601 L  */

    HAL_CSC_YUV2YUV_709_2020_L_L , /* YUV_709 L  -> YUV_2020 L */
    HAL_CSC_YUV2RGB_709_2020_L   , /* YUV_709 L  -> RGB_2020   */

    HAL_CSC_YUV2YUV_601_2020_L_L , /* YUV_601 L  -> YUV_2020 L */
    HAL_CSC_YUV2RGB_601_2020_L   , /* YUV_601 L  -> RGB_2020   */

    HAL_CSC_YUV2YUV_2020_2020_L_L, /* YUV_2020 L -> YUV_2020 L */
    HAL_CSC_YUV2YUV_2020_709_L_L , /* YUV_2020 L -> YUV_709 L  */
    HAL_CSC_YUV2YUV_2020_601_L_L , /* YUV_2020 L -> YUV_601 L  */
    HAL_CSC_YUV2RGB_2020_2020_L  , /* YUV_2020 L -> RGB_2020   */
    HAL_CSC_YUV2RGB_2020_709_L   , /* YUV_2020 L -> RGB_709    */

    HAL_CSC_YUV2RGB_601_FULL     , /* YUV_601 F  -> RGB        */
    HAL_CSC_YUV2RGB_709_FULL     , /* YUV_709 F  -> RGB        */
    HAL_CSC_RGB2YUV_601_FULL     , /* RGB        -> YUV_601 F  */
    HAL_CSC_RGB2YUV_709_FULL     , /* RGB        -> YUV_709 F  */
    HAL_CSC_RGB2RGB              , /* RGB        -> RGB        */

    HAL_CSC_BUTT
} CSC_MODE_E;

/* 色彩空间标准 */
typedef enum hiCOLOR_SPACE_TYPE_E
{
    OPTM_CS_eUnknown           = 0,
    OPTM_CS_eItu_R_BT_709      = 1,
    OPTM_CS_eFCC               = 4,
    OPTM_CS_eItu_R_BT_470_2_BG = 5,
    OPTM_CS_eSmpte_170M        = 6,
    OPTM_CS_eSmpte_240M        = 7,
    OPTM_CS_eXvYCC_709         = OPTM_CS_eItu_R_BT_709,
    OPTM_CS_eXvYCC_601         = 8,
    OPTM_CS_eRGB               = 9,
    /*
    OPTM_CS_eItu_BT_2020       = 10,
    OPTM_CS_eRGB_601           = 11,
    OPTM_CS_eRGB_709           = 12,
    OPTM_CS_eRGB_2020          = 13,
    */

    OPTM_CS_BUTT
} COLOR_SPACE_TYPE_E;

/* 色温通道 */
typedef enum hiCOLORTEMP_CHANNEL_E
{
    COLORTEMP_R = 0  ,
    COLORTEMP_G = 1  ,
    COLORTEMP_B = 2  ,

    COLORTEMP_ALL = 3
} COLORTEMP_CHANNEL_E;

/* CSC 参数结构 */
typedef struct hiCSC_PARA_S
{
    HI_U32   u16Brightness;
    HI_U32   u16Contrast;
    HI_U32   u16Hue;
    HI_U32   u16Saturation;
    HI_U32   as16ColorTempGain[COLORTEMP_ALL];
    HI_U32   as16ColorTempOffset[COLORTEMP_ALL];
    HI_U16   u16InputColorSpace;
    HI_U16   u16OutputColorSpace;
    HI_BOOL  bFullRange;
} CSC_PARA_S;

/*CSC 矩阵系数结构*/
typedef struct  hiCSC_COEF_S
{
    HI_S32 csc_coef00;
    HI_S32 csc_coef01;
    HI_S32 csc_coef02;

    HI_S32 csc_coef10;
    HI_S32 csc_coef11;
    HI_S32 csc_coef12;

    HI_S32 csc_coef20;
    HI_S32 csc_coef21;
    HI_S32 csc_coef22;
} CSC_COEF_S;

typedef struct  hiCSC_DCCOEF_S
{
    HI_S32 csc_in_dc0;
    HI_S32 csc_in_dc1;
    HI_S32 csc_in_dc2;

    HI_S32 csc_out_dc0;
    HI_S32 csc_out_dc1;
    HI_S32 csc_out_dc2;
} CSC_DCCOEF_S;

typedef struct  hiCSC_MODE_S
{
    HI_BOOL     bCSCEn;
    CSC_MODE_E  enCscMode;
} CSC_MODE_S;

typedef enum  hiWBC_POINT_SEL_E
{
    WBC_POINT_AFTER_CSC  = 0,   /* 00：回写点在VP CSC后，数据格式为YUV444 */
    WBC_POINT_AFTER_ZME     ,   /* 01：回写点在V0 ZME后，数据格式为YUV422 */
    WBC_POINT_BEFORE_ZME    ,   /* 10：回写点在V0 ZME前，数据格式为YUV420或YUV422 */

    WBC_POINT_BUTT
} WBC_POINT_SEL_E;


HI_U32 PQ_HAL_GetWbcPointSel(HI_VOID);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
