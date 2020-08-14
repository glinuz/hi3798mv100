/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_lcacm.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_LCACM_H__
#define __PQ_MNG_LCACM_H__


#include "drv_pq_table.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 PQ_MNG_RegisterLowCostACM(PQ_REG_TYPE_E enType);
HI_S32 PQ_MNG_UnRegisterLowCostACM(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


