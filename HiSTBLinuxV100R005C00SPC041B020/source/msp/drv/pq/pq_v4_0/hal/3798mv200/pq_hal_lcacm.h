/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_lcacm.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#ifndef __PQ_HAL_LCACM_H__
#define __PQ_HAL_LCACM_H__

#include "hi_type.h"
#include "drv_pq_table.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct
{
    HI_U32 acm0_en;
    HI_U32 acm1_en;
    HI_U32 acm2_en;
    HI_U32 acm3_en;
    HI_U32 acm_dbg_mode;
    HI_U32 acm_test_en;
    HI_U32 bw_pro_mode;
    HI_U32 acm0_sec_blk;
    HI_U32 acm0_fir_blk;
    HI_U32 acm1_sec_blk;
    HI_U32 acm1_fir_blk;
    HI_U32 acm2_sec_blk;
    HI_U32 acm2_fir_blk;
    HI_U32 acm3_sec_blk;
    HI_U32 acm3_fir_blk;

} PQ_HAL_ACM_CFG_S;

typedef struct
{
    HI_U32 acm0_d_u_off;
    HI_U32 acm0_c_u_off;
    HI_U32 acm0_b_u_off;
    HI_U32 acm0_a_u_off;

    HI_U32 acm0_d_v_off;
    HI_U32 acm0_c_v_off;
    HI_U32 acm0_b_v_off;
    HI_U32 acm0_a_v_off;

    HI_U32 acm1_d_u_off;
    HI_U32 acm1_c_u_off;
    HI_U32 acm1_b_u_off;
    HI_U32 acm1_a_u_off;
    HI_U32 acm1_d_v_off;
    HI_U32 acm1_c_v_off;
    HI_U32 acm1_b_v_off;
    HI_U32 acm1_a_v_off;

    HI_U32 acm2_d_u_off;
    HI_U32 acm2_c_u_off;
    HI_U32 acm2_b_u_off;
    HI_U32 acm2_a_u_off;
    HI_U32 acm2_d_v_off;
    HI_U32 acm2_c_v_off;
    HI_U32 acm2_b_v_off;
    HI_U32 acm2_a_v_off;

    HI_U32 acm3_d_u_off;
    HI_U32 acm3_c_u_off;
    HI_U32 acm3_b_u_off;
    HI_U32 acm3_a_u_off;
    HI_U32 acm3_d_v_off;
    HI_U32 acm3_c_v_off;
    HI_U32 acm3_b_v_off;
    HI_U32 acm3_a_v_off;

    PQ_HAL_ACM_CFG_S stAcmCfg;

} PQ_HAL_ACM_PARA_S;

/* ACM demo mode */
typedef enum hiHAL_ACM_DEMO_MODE_E
{
    HAL_ACM_DEMO_ENABLE_R = 0,
    HAL_ACM_DEMO_ENABLE_L    ,

    HAL_ACM_DEMO_BUTT
} HAL_ACM_DEMO_MODE_E;



HI_VOID PQ_HAL_EnableACMDemo(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff);
HI_VOID PQ_HAL_SetACMDemoMode(PQ_HAL_LAYER_VP_E u32ChId, HAL_ACM_DEMO_MODE_E enACMDemoMode);
HI_VOID PQ_HAL_EnableACM(PQ_HAL_LAYER_VP_E u32ChId, HI_BOOL bOnOff);
HI_S32  PQ_HAL_GetACMEnableFlag(HI_BOOL *bOnOff);
HI_VOID PQ_HAL_SetLowCostAcmPara(HI_U32 enLayer, PQ_HAL_ACM_PARA_S *pstAcmPara);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

