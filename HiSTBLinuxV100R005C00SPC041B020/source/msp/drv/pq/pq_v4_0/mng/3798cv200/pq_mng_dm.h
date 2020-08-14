/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dm.h
  Version       : Initial Draft
  Author        :
  Created       : 2015-10-09
  Description   : De-Mosquito

******************************************************************************/


#ifndef __PQ_MNG_DM_H__
#define __PQ_MNG_DM_H__

#include "pq_hal_db.h"
#include "pq_hal_dm.h"
#include "pq_hal_comm.h"
#include "drv_pq_table.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct
{
    HI_U8  u8NumWindow; /* DM Calc time domain window */

} DM_CFG_INFO_S;


HI_S32 PQ_MNG_RegisterDM(PQ_REG_TYPE_E enType);
HI_S32 PQ_MNG_UnRegisterDM(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



