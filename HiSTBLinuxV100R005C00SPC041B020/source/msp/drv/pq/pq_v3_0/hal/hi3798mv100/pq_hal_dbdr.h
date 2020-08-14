/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dbdr.h
  Version       : Initial Draft
  Author        :
  Created       : 2014/01/28
  Description   : De-blocking and De-Ringning

******************************************************************************/

#ifndef __PQ_HAL_DBDR_H__
#define __PQ_HAL_DBDR_H__

#include "hi_type.h"
#include "pq_hal_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
    DBM_DEMO_ENABLE_L = 0,
    DBM_DEMO_ENABLE_R
} DBM_DEMO_MODE_E;

/* no practical meaning*/
typedef struct hiDB_CALC_INFO_S
{
    HI_U32 u32HandleNo;
    HI_U32 u32Width;
    HI_U32 u32Height;
    S_VPSSWB_REGS_TYPE* pstVPSSWbcReg;
} DB_CALC_INFO_S;


/*  Get DR Enable Reg */
HI_S32 PQ_HAL_GetDREnReg(HI_U32 u32HandleNo, HI_BOOL* pbOnOff);
/*  Set DR Demo Enable */
HI_S32 PQ_HAL_EnableDRDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

