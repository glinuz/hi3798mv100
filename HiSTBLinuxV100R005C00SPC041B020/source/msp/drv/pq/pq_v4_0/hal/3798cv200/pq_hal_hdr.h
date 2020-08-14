/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name    : pq_hal_hdr.h
  Version       : Initial Draft
  Author        : p00203646
  Created      : 2016/06/15
  Description  :

******************************************************************************/

#ifndef _PQ_HAL_HDR_H_
#define _PQ_HAL_HDR_H_

#include "hi_type.h"
#include "pq_mng_hdr_ext.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PQ_HAL_HDR_TM_LUT_SIZE 512

typedef enum hiPQ_HAL_HDR_TM_E
{
    PQ_HAL_HDR_TM_TMLUTI = 0,
    PQ_HAL_HDR_TM_TMLUTS,
    PQ_HAL_HDR_TM_SMLUTI,
    PQ_HAL_HDR_TM_SMLUTS,
    PQ_HAL_HDR_TM_BUTT
} PQ_HAL_HDR_TM_E;

typedef struct hiPQ_HDR_ALG_TM
{
    HI_S16 as16HDR10toSDRTMLut[PQ_HAL_HDR_TM_BUTT][PQ_HAL_HDR_TM_LUT_SIZE];
    HI_S16 as16HLGtoSDRTMLut[PQ_HAL_HDR_TM_BUTT][PQ_HAL_HDR_TM_LUT_SIZE];
    HI_S16 as16HLGtoHDR10TMLut[PQ_HAL_HDR_TM_BUTT][PQ_HAL_HDR_TM_LUT_SIZE];
    HI_S16 as16ReservedTMLut[PQ_HAL_HDR_TM_BUTT][PQ_HAL_HDR_TM_LUT_SIZE];
} PQ_HDR_ALG_TM;

typedef enum hiPQ_HAL_HDR_MODE_E
{
    PQ_HAL_HDR_MODE_HDR10_TO_SDR = 0,
    PQ_HAL_HDR_MODE_HDR10_TO_HDR10,
    PQ_HAL_HDR_MODE_HLG_TO_SDR,
    PQ_HAL_HDR_MODE_HLG_TO_HDR10,
    PQ_HAL_HDR_MODE_BUTT,
} PQ_HAL_HDR_MODE_E;


HI_S32 PQ_HAL_GetHDRCfg(HI_PQ_WIN_HDR_INFO* pstWinHdrInfo, HI_PQ_HDR_CFG* pstPqHdrCfg);

HI_S32 PQ_HAL_SetHDRSaturation(HI_U32 u32Saturation);

HI_S32 PQ_HAL_InitHDR(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault);

HI_S32 PQ_HAL_SetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut);

HI_S32 PQ_HAL_GetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut);

HI_S32 PQ_HAL_SetHDRDefaultCfg(HI_BOOL bDefault);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
