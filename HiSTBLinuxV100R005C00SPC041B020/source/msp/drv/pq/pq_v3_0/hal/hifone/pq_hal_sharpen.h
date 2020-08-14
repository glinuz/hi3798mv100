/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* HiFoneB2 HSharpen in vpss */
#define HSHARP_H0_GAIN_POS_ADDR   0xf8cb2114
#define HSHARP_H0_GAIN_POS_LSB    0
#define HSHARP_H0_GAIN_POS_MSB    10

#define HSHARP_H0_GAIN_NEG_ADDR   0xf8cb2114
#define HSHARP_H0_GAIN_NEG_LSB    16
#define HSHARP_H0_GAIN_NEG_MSB    26

#define HSHARP_H1_GAIN_POS_ADDR   0xf8cb2130
#define HSHARP_H1_GAIN_POS_LSB    0
#define HSHARP_H1_GAIN_POS_MSB    10

#define HSHARP_H1_GAIN_NEG_ADDR   0xf8cb2130
#define HSHARP_H1_GAIN_NEG_LSB    16
#define HSHARP_H1_GAIN_NEG_MSB    26


typedef struct hiPQ_HSHARP_GAIN_S
{
    HI_S32 s32H0GainPos;
    HI_S32 s32H0GainNeg;
    HI_S32 s32H1GainPos;
    HI_S32 s32H1GainNeg;
} PQ_HSHARP_GAIN_S;

/* HSharp Demo Mode */
typedef enum hiSHARP_DEMO_MODE_E
{
    SHARP_DEMO_ENABLE_L = 0,
    SHARP_DEMO_ENABLE_R    ,

    SHARP_DEMO_BUTT
} SHARP_DEMO_MODE_E;


/* Set hsharpen Strength reg, Global variable, Range:0~255 */
HI_S32  PQ_HAL_SetHSharpStrReg(HI_U32 u32HandleNo, HI_S32 s32ShpStr, PQ_HSHARP_GAIN_S* pstHSharpGain);
/* Enable HSharpen */
HI_S32  PQ_HAL_EnableHSharp(HI_U32 u32HandleNo, HI_BOOL bOnOff);
/* Get HSharpen Enable Flag */
HI_S32  PQ_HAL_GetEnableHSharp(HI_U32 u32HandleNo, HI_BOOL* bOnOff);
/* Enable HSharpen Demo */
HI_S32  PQ_HAL_EnableHSharpDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
/* Choose HSharpen Demo Mode; 0£ºEnable Left,Right Orignal; 1£ºEnable Right,Left Orignal */
HI_S32  PQ_HAL_SetHSharpDemoMode(HI_U32 u32HandleNo, HI_U32 u32HshpDemoMode);
HI_VOID PQ_HAL_SetSharpDemoPos(HI_U32 u32Data, HI_U32 u32Pos);
HI_VOID PQ_HAL_GetSharpDemoPos(HI_U32 u32Data, HI_U32* u32Pos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


