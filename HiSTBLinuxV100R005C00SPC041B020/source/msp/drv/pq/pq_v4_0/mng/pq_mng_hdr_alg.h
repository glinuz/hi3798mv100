/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name    : pq_mng_hdr_alg.h
  Version       : Initial Draft
  Author        : p00203646
  Created      : 2016/03/19
  Description  :

******************************************************************************/


#ifndef __PQ_MNG_HDR_ALG_H__
#define __PQ_MNG_HDR_ALG_H__

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PQ_HDR_ALG_TM_LUT_SIZE 512


/* PQ Source Mode */
typedef enum hiPQ_HDR_ALG_CFG_TYPE_E
{
    PQ_HDR_ALG_CFG_TYPE_TM = 0,

    PQ_HDR_ALG_CFG_TYPE_BUTT
} PQ_HDR_ALG_CFG_TYPE_E;


HI_S32 PQ_MNG_DescrambleHdrCfg(PQ_HDR_ALG_CFG_TYPE_E enType, HI_VOID *pDate);
HI_S32 PQ_MNG_ScrambleHdrCfg(PQ_HDR_ALG_CFG_TYPE_E enType);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


