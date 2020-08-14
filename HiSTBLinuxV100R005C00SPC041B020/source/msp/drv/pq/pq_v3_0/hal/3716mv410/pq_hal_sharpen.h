/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_sharpen.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/11/10
  Description   :

******************************************************************************/

#ifndef __PQ_HAL_SHARPEN_H__
#define __PQ_HAL_SHARPEN_H__

#include "hi_type.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* MV410 Sharpen in vdp */
#define SHARP_EDGE_GAIN_ADDR      0xf8cc441c
#define SHARP_EDGE_GAIN_LSB       10
#define SHARP_EDGE_GAIN_MSB       19

#define SHARP_PEAK_GAIN_ADDR      0xf8cc441c
#define SHARP_PEAK_GAIN_LSB       0
#define SHARP_PEAK_GAIN_MSB       9


typedef struct hiPQ_SHARP_GAIN_S
{
    HI_U32 u32PeakGain;
    HI_U32 u32EdgeGain;
} PQ_SHARP_GAIN_S;

typedef enum hi_SHARP_DEMO_MODE_E
{
    SHARP_DEMO_ENABLE_R = 0,
    SHARP_DEMO_ENABLE_L    ,

} SHARP_DEMO_MODE_E;


HI_S32  PQ_HAL_EnableSharp(HI_BOOL bOnOff);
HI_S32  PQ_HAL_SetSharpStrReg(HI_S32 s32ShpStr, PQ_SHARP_GAIN_S* pstSharpGain);
HI_S32  PQ_HAL_EnableSharpDemo(HI_BOOL bSharpDemoEn);
HI_S32  PQ_HAL_SetSharpDemoMode(HI_U32 u32Data, HI_U32 u32SharpDemoMode);
HI_VOID PQ_HAL_SetSharpDemoPos(HI_U32 u32Data, HI_U32 u32Pos);
HI_VOID PQ_HAL_GetSharpDemoPos(HI_U32 u32Data, HI_U32* u32Pos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

