/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_db.h
  Version       : Initial Draft
  Author        :
  Created       : 2015-10-09
  Description   : De-blocking

******************************************************************************/


#ifndef __PQ_MNG_DB_H__
#define __PQ_MNG_DB_H__


#include "pq_hal_db.h"
#include "pq_hal_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct
{
    HI_U16 PixValAdjGain;
    HI_U16 HFDiffCore;
    HI_U16 HFDiffGain1;
    HI_U16 HFDiffGain2;
    HI_U16 HFVarCore;
    HI_U16 HFVarGain1;
    HI_U16 HFVarGain2;
    HI_U16 BordAdjGain;
    HI_U16 GradSubRatio;
} DB_DynmcPara;


HI_S32 PQ_MNG_RegisterDB(PQ_REG_TYPE_E enType);
HI_S32 PQ_MNG_UnRegisterDB(HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



