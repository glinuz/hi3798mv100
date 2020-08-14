/******************************************************************************
*
* Copyright (C) 2015 - 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_tnr.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/05/25
  Description   :

******************************************************************************/

#ifndef __PQ_MNG_TNR_H__
#define __PQ_MNG_TNR_H__

#include "drv_pq_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 PQ_MNG_RegisterTNR(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterTNR(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif



