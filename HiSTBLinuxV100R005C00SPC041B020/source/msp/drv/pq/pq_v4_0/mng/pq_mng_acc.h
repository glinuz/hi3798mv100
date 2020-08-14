/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_acc.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_ACC_H__
#define __PQ_MNG_ACC_H__


#include "drv_pq_table.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


HI_S32 PQ_MNG_RegisterACC(PQ_REG_TYPE_E enType);
HI_S32 PQ_MNG_UnRegisterACC(HI_VOID);
HI_S32 PQ_MNG_SetACCLut(PQ_ACC_LUT_S *pstACCLut);
HI_S32 PQ_MNG_GetACCLut(PQ_ACC_LUT_S *pstACCLut);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


