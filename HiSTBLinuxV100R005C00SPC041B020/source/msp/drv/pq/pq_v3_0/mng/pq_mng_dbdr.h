/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dbdr.h
  Version       : Initial Draft
  Author        :
  Created       : 2014-12-23
  Description   : De-blocking and De-Ringning

******************************************************************************/

#ifndef __PQ_MNG_DBDR_H__
#define __PQ_MNG_DBDR_H__


#include "pq_hal_dbdr.h"
#include "pq_hal_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef enum HiDB_TUN_MODE_E
{
    DB_TUN_NORMAL  = 0,
    DB_TUN_DEBUG      ,

    DB_TUN_BUTT
} DB_TUN_MODE_E;


/* Init SD DR */
HI_S32 PQ_MNG_InitDNR(PQ_PARAM_S* pstPqParam);
/* Deinit SD DR */
HI_S32 PQ_MNG_DeinitDNR(HI_VOID);
/* Enable DR */
HI_S32 PQ_MNG_EnableDR(HI_BOOL bOnOff);
/* get DR Enable state */
HI_S32 PQ_MNG_GetDREnableFlag(HI_BOOL* pbOnOff);
/* Enable DR Demo */
HI_S32 PQ_MNG_EnableDRDemo(HI_BOOL bOnOff);

/* Enable DB *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDB(HI_BOOL bOnOff);
/* Get DB Enable state *//* no practical meaning*/
HI_S32 PQ_MNG_GetDBEnableFlag(HI_BOOL* pbOnOff);
/* DB DEMO Enable *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDBDemo(HI_BOOL bOnOff);
/* Set DM Enable *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDM(HI_BOOL bOnOff);
/* Get DM Enable state *//* no practical meaning*/
HI_S32 PQ_MNG_GetDMEnableFlag(HI_BOOL* pbOnOff);
/* Enable DM DEMO *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDMDemo(HI_BOOL bOnOff);

/* no practical meaning*/
HI_S32 PQ_MNG_SetDbmDemoMode(DBM_DEMO_MODE_E enMode);
/* no practical meaning*/
HI_S32 PQ_MNG_DBCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate);
/* no practical meaning*/
HI_S32 PQ_MNG_DBCalcConfigCfg(DB_CALC_INFO_S* pstDbCalcInfo);
/* no practical meaning*/
HI_S32 PQ_MNG_DMCalcConfigCfg(DB_CALC_INFO_S* pstDmCalcInfo);

HI_S32 PQ_MNG_SetDbTunMode(DB_TUN_MODE_E enDBTunMode);
HI_S32 PQ_MNG_GetDbTunMode(DB_TUN_MODE_E* enDBTunMode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



