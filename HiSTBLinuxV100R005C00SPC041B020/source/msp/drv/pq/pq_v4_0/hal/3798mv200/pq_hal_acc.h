/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_acm.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#ifndef __PQ_HAL_ACC_H__
#define __PQ_HAL_ACC_H__

#include "hi_type.h"
#include "pq_hal_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct
{
    HI_U32 acc_en;
    HI_U32 acc_mode;
    HI_U32 acc_dbg_mode;
    HI_U32 acc_dbg_en;
    HI_U32 acc_rst;

} PQ_HAL_ACC_CFG_S;

typedef struct
{
    HI_U32 thd_med_low;
    HI_U32 thd_high;
    HI_U32 thd_low;
    HI_U32 acc_multiple;
    HI_U32 thd_med_high;
    HI_U32 cnt3_low;
    HI_U32 cnt3_med;
    HI_U32 cnt3_high;
    HI_U32 cnt8_med_low;
    HI_U32 cnt8_med_high;
    HI_U32 acc_pix_total;

    PQ_HAL_ACC_CFG_S stAccCfg;

} PQ_HAL_ACC_PARA_S;

typedef enum hiACC_DEMO_MODE_E
{
    ACC_DEMO_ENABLE_R = 0,
    ACC_DEMO_ENABLE_L    ,

    ACC_DEMO_ENABLE_BUTT
} ACC_DEMO_MODE_E;

HI_VOID PQ_HAL_SetAccAccEn(HI_U32 u32Data, HI_U32 acc_en);
HI_VOID PQ_HAL_SetAccAccMode(HI_U32 u32Data, HI_U32 acc_mode);
HI_VOID PQ_HAL_SetAccDbgMode(HI_U32 u32Data, HI_U32 acc_dbg_mode);
HI_VOID PQ_HAL_SetAccDbgEn(HI_U32 u32Data, HI_U32 acc_dbg_en);
HI_S32  PQ_HAL_SetAccTab(HI_U32 u32Data, HI_U32 *upTable);
HI_S32  PQ_HAL_SetAccPara(HI_U32 u32Data, PQ_HAL_ACC_PARA_S stAccPara);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

