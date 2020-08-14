/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dei.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/06/16
  Description   :

******************************************************************************/


#ifndef __PQ_MNG_DEI_H__
#define __PQ_MNG_DEI_H__

#include "hi_type.h"
#include "drv_pq_comm.h"
#include "pq_hal_dei.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* input of sofeware */
typedef struct
{
    HI_U16 au16GlobalMotionThd[6];
    HI_U16 au16GlobalMotionRatio[5];
    HI_S16 as16GlobalMotionSlope[4];
    HI_U16 u16MaxGlobalMotionRatio;
    HI_U16 u16MinGlobalMotionRatio;
    HI_U32 u32SmallMotionThd;
    HI_U32 u32LargeMotionThd;
} MOTION_CALC_S; /* MotionCalc_S */


typedef struct
{
    HI_U32  u32Width;
    HI_U32  u32Height;
    HI_U32  u32HandleNo;

    HI_U32  u32Scd;
    HI_U32  stride;
    HI_VOID* pRGMV;

    S_STT_REGS_TYPE* pstMotionReg;
} MOTION_INPUT_S;


HI_S32 PQ_MNG_RegisterDEI(PQ_REG_TYPE_E enType);

HI_S32 PQ_MNG_UnRegisterDEI(HI_VOID);

HI_S32 PQ_MNG_GetGlobMotiGain(HI_U32* pu32Gain);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

