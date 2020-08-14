/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_ifmd.h
  Version       : Initial Draft
  Author        : pengjunwei
  Created       : 2016-08-01
  Description   : 

******************************************************************************/
#ifndef __PQ_MNG_IFMD_H__
#define __PQ_MNG_IFMD_H__

#include "hi_type.h"

#include "pq_hal_comm.h"
#include "pq_hal_fmd.h"
#include "drv_pq_table.h"
#include "pq_mng_ifmd_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



typedef enum hiPQ_DIE_OUT_MODE_E
{
    DIE_OUT_MODE_AUTO = 0,
    DIE_OUT_MODE_FIR,
    DIE_OUT_MODE_COPY,
    DIE_OUT_MODE_BUTT
} PQ_DIE_OUT_MODE_E;

typedef enum hiPQ_FOD_MODE_E
{
    PQ_FOD_ENABLE_AUTO  = 1,
    PQ_FOD_TOP_FIRST    = 2,
    PQ_FOD_BOTTOM_FIRST = 3,
    PQ_FOD_MODE_BUTT
} PQ_FOD_MODE_E;

typedef enum HiPQ_IFMD_TUN_MODE_E
{
    IFMD_TUN_NORMAL  = 0,
    IFMD_TUN_DEBUG      ,

    IFMD_TUN_BUTT
} PQ_IFMD_TUN_MODE_E;

HI_S32 PQ_MNG_IFMD_SofewareParaInitByHandleNo(HI_U32 u32HandleNo);
HI_S32 PQ_MNG_InitIfmd(PQ_PARAM_S* pstPqParam);
HI_S32 PQ_MNG_DeInitIfmd(HI_VOID);
HI_S32 PQ_MNG_IfmdDect(PQ_IFMD_CALC_S* pstIfmdCalc, REG_PLAYBACK_CONFIG_S* pstIfmdResult);
HI_S32 PQ_MNG_GetIfmdDectInfo(REG_PLAYBACK_CONFIG_S* pstIfmdInfo);
HI_S32 PQ_MNG_SetDieOutMode(PQ_DIE_OUT_MODE_E enMode);
HI_S32 PQ_MNG_SetFodMode(PQ_FOD_MODE_E enMode);
HI_S32 PQ_MNG_GetDeiMdLum(HI_VOID);
HI_S32 PQ_MNG_SetIfmdTunMode(PQ_IFMD_TUN_MODE_E enMode);
HI_S32 PQ_MNG_GetIfmdTunMode(PQ_IFMD_TUN_MODE_E* penMode);



#ifdef __cplusplus
#if __cplusplus
}
#endif

#endif

#endif

