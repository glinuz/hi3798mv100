/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dbdm.h
  Version       : Initial Draft
  Author        :
  Created       : 2014-10-16
  Description   : De-blocking and De-Ringning

******************************************************************************/

#ifndef __PQ_MNG_DBDM_H__
#define __PQ_MNG_DBDM_H__

#include "pq_hal_dbdr.h"
#include "pq_hal_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define MAXLINESIZE 4096

#define DBM_MIN(x, y) (((x) > (y)) ? (y) : (x))
#define DBM_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define DBM_CLIP3(low,high,x)  (DBM_MAX(DBM_MIN((high),(x)),(low)))

#define DBM_Blend(F,B,An,Ad)   (F*An + B*(Ad-An) + Ad/2)/Ad


typedef enum HiDB_TUN_MODE_E
{
    DB_TUN_NORMAL  = 0,
    DB_TUN_DEBUG      ,

    DB_TUN_BUTT
} DB_TUN_MODE_E;


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


typedef struct
{
    HI_U8  u8NumWindow; /* DM Calc time domain window */

} DM_CFG_INFO_S;


/* Init DBDM */
HI_S32 PQ_MNG_InitDNR(PQ_PARAM_S* pstPqParam);
/* Deinit DBDM */
HI_S32 PQ_MNG_DeinitDNR(HI_VOID);
HI_S32 PQ_MNG_EnableDB(HI_BOOL bOnOff);
/* Get DB Enable state */
HI_S32 PQ_MNG_GetDBEnableFlag(HI_BOOL* pbOnOff);
/* DB DEMO Enable */
HI_S32 PQ_MNG_EnableDBDemo(HI_BOOL bOnOff);
/* Set DM Enable */
HI_S32 PQ_MNG_EnableDM(HI_BOOL bOnOff);
/* Get DM Enable state */
HI_S32 PQ_MNG_GetDMEnableFlag(HI_BOOL* pbOnOff);
/* Enable DM DEMO */
HI_S32 PQ_MNG_EnableDMDemo(HI_BOOL bOnOff);
/* Set DR Enable */
HI_S32 PQ_MNG_EnableDR(HI_BOOL bOnOff);
/* Get DR Enable state */
HI_S32 PQ_MNG_GetDREnableFlag(HI_BOOL* pbOnOff);
/* Enable DR DEMO */
HI_S32 PQ_MNG_EnableDRDemo(HI_BOOL bOnOff);

HI_S32 PQ_MNG_SetDbmDemoMode(DBM_DEMO_MODE_E enMode);
HI_S32 PQ_MNG_GetDbmDemoMode(DBM_DEMO_MODE_E* penMode);

HI_S32 PQ_MNG_DBCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate);
HI_S32 PQ_MNG_DBCalcConfigCfg(DB_CALC_INFO_S* pstDbCalcInfo);
HI_S32 PQ_MNG_DMCalcConfigCfg(DB_CALC_INFO_S* pstDmCalcInfo);

HI_S32 PQ_MNG_SetDbTunMode(DB_TUN_MODE_E enDBTunMode);
HI_S32 PQ_MNG_GetDbTunMode(DB_TUN_MODE_E* enDBTunMode);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

