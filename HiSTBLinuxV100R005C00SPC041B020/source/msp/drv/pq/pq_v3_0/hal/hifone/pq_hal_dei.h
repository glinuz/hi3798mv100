/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_dei.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/06/16
  Description   :

******************************************************************************/


#ifndef __PQ_HAL_DEI_H__
#define __PQ_HAL_DEI_H__

#include "hi_type.h"
#include "hi_reg_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    /* input of logic */
    HI_U32 SMALL_MOTION_THD;            /* input of logic, 10bit unsigend */
    HI_U32 LARGE_MOTION_THD;            /* input of logic, 10bit unsigend */
    /* output of logic and input of software */
    HI_U32 MOTION_HIST[32];             /* output of logic, 32bit unsigned */
    HI_U32 SMALL_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 SMALL_MOTION_SUM;            /* output of logic, 32bit unsigned */
    HI_U32 LARGE_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 LARGE_MOTION_SUM;            /* output of logic, 32bit unsigned */
    HI_U32 MIDDL_MOTION_NUM;            /* output of logic, 32bit unsigned */
    HI_U32 MIDDL_MOTION_SUM;            /* output of logic, 32bit unsigned */
    /* input of software */
    HI_U16 GLOBAL_MOTION_EN;
    HI_U16 GLOBAL_MOTION_THD[6];        /* 8-bit unsigned */
    HI_U16 GLOBAL_MOTION_RATIO[5];      /* 8-bit unsigned, (2.6) */
    HI_S16 GLOBAL_MOTION_SLOPE[4];      /* 6-bit signed,   (3.3) */
    HI_U16 MAX_GLOBAL_MOTION_RATIO;     /* 8-bit unsigned, (2.6) */
    HI_U16 MIN_GLOBAL_MOTION_RATIO;     /* 8-bit unsigned, (2.6) */
} MotionHist_S;

typedef struct
{
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32HandleNo;
    S_VPSSWB_REGS_TYPE* pstMotionReg;
} MOTION_INPUT_S;

/* dei demo mode */
typedef enum
{
    DEI_DEMO_ENABLE_R = 0,
    DEI_DEMO_ENABLE_L
} DEI_DEMO_MODE_E;

HI_S32  PQ_HAL_SetDeiAdjustGain(HI_U32 u32HandleNo, HI_U32 u32AdjustGain);
HI_S32  PQ_HAL_GetMotionHistReg(MOTION_INPUT_S* pstMotionInput, MotionHist_S* pstMotionHist);
HI_BOOL PQ_HAL_GetInterlaceMotionEn(HI_U32 u32HandleNo);
HI_S32  PQ_HAL_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bIGlbEn);
HI_S32  PQ_HAL_GetDeiEn(HI_U32 u32HandleNo, HI_BOOL* pbOnOff);
HI_S32  PQ_HAL_EnableDeiDemo(HI_U32 u32HandleNo, HI_BOOL bDemoEn);
HI_S32  PQ_HAL_SetDeiDemoMode(HI_U32 u32HandleNo, DEI_DEMO_MODE_E enMode);
HI_S32  PQ_HAL_SetVpssMktCoordinate(HI_U32 u32HandleNo, HI_U32 u32DbgPos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


