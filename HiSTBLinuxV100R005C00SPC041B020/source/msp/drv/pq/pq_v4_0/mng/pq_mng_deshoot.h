/******************************************************************************

  Copyright (C), 2014-2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_deshoot.h
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2015/05/11
  Description   :

******************************************************************************/


#ifndef __PQ_MNG_DESHOOT_H__
#define __PQ_MNG_DESHOOT_H__

#include "hi_type.h"
#include "drv_pq_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_S32 PQ_MNG_RegisterDS(PQ_REG_TYPE_E  enType);

HI_S32 PQ_MNG_UnRegisterDS(HI_VOID);

//HI_S32 PQ_MNG_RegisterArtDS(PQ_REG_TYPE_E  enType);
//
//HI_S32 PQ_MNG_UnRegisterArtDS(HI_VOID);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


