/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_csc.h
  Version       : Initial Draft
  Author        : l00212594
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_CSC_H__
#define __PQ_MNG_CSC_H__


#include "pq_hal_csc.h"
#include "drv_pq_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 窗口类型 */
typedef enum hiCSC_WINDOW_E
{
    CSC_WINDOW_MAIN = 0,
    CSC_WINDOW_SUB,

    CSC_WINDOW_BUTT
} CSC_WINDOW_E;

/* 亮度/对比度/色调/饱和度设定 */
typedef struct hiPICTURE_SETTING_S
{
    HI_U16 u16Brightness;
    HI_U16 u16Contrast;
    HI_U16 u16Hue;
    HI_U16 u16Saturation;
} PICTURE_SETTING_S;

/* 色温设定 */
typedef struct hiCOLOR_TEMPERATURE_S
{
    HI_S16 s16RedGain;
    HI_S16 s16GreenGain;
    HI_S16 s16BlueGain;
    HI_S16 s16RedOffset;
    HI_S16 s16GreenOffset;
    HI_S16 s16BlueOffset;
} COLOR_TEMPERATURE_S;

/* 色彩空间设定 */
typedef struct hiCOLOR_SPACE_S
{
    COLOR_SPACE_TYPE_E u16InputColorSpace;    /* 输入色彩空间 */
    COLOR_SPACE_TYPE_E u16OutputColorSpace;   /* 输出色彩空间 */
    HI_BOOL            bFullRange;            /* 0:limit,1:full range */
} COLOR_SPACE_S;

typedef struct hiCSC_COLOR_TEMP
{
    HI_U32 u32Red;
    HI_U32 u32Green;
    HI_U32 u32Blue;
} CSC_COLOR_TEMP;


HI_S32 PQ_MNG_RegisterCSC(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterCSC(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
