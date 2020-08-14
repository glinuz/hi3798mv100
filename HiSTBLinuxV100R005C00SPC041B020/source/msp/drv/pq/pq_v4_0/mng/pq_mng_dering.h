/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dering.h
  Version       : Initial Draft
  Author        : l00216738
  Created       : 2015/09/01
  Description   :

******************************************************************************/


#ifndef __PQ_MNG_DERING_H__
#define __PQ_MNG_DERING_H__

#include "hi_type.h"
#include "drv_pq_comm.h"
#include "pq_hal_dering.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 PQ_MNG_RegisterDR(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterDR(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


