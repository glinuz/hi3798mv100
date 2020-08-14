/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dbdr.h
  Version       : Initial Draft
  Author        :
  Created       : 2014/10/16
  Description   : De-blocking and De-Mosquito
  
******************************************************************************/

#ifndef __PQ_HAL_DBDR_H__
#define __PQ_HAL_DBDR_H__

#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_dbdm_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* DB BLK NUM */
#define DB_HY_TST_BLK_NUM_ADDR    0xf8cb2564
#define DB_HY_TST_BLK_NUM_LSB     0
#define DB_HY_TST_BLK_NUM_MSB     8

#define DB_HC_TST_BLK_NUM_ADDR    0xf8cb2564
#define DB_HC_TST_BLK_NUM_LSB     10
#define DB_HC_TST_BLK_NUM_MSB     18

#define DB_VY_TST_BLK_NUM_ADDR    0xf8cb2564
#define DB_VY_TST_BLK_NUM_LSB     20
#define DB_VY_TST_BLK_NUM_MSB     28


/* DB强度值,对应VPSS_DB_WEIGHT寄存器 */
typedef struct
{
    HI_U8    u8HWeight;     /* 6bits [0,63] */
    HI_U8    u8VWeight;     /* 6bits [0,63] */
} DB_WEIGHT_S;


/* De-Blocking运算需要VPSS传递进来的信息 */
typedef struct hiDB_CALC_INFO_S
{
    HI_U32 u32HandleNo;
    HI_U32 u32Width;
    HI_U32 u32Height;
    S_VPSSWB_REGS_TYPE* pstVPSSWbcReg;

} DB_CALC_INFO_S;

/* dbm demo mode */
/* mv410 Enable Left = 0, HiFone Enable Right = 1*/
typedef enum
{
    DBM_DEMO_ENABLE_L = 0,
    DBM_DEMO_ENABLE_R
} DBM_DEMO_MODE_E;


/* 设置DB软算法api计算出来的参数值 */
HI_S32 PQ_HAL_SetDBApiReg(HI_U32 u32HandleNo, DB_API_REG_S* pstApiPara);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Delta_Lut(HI_U32 u32HandleNo, HI_U32 u8SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut(HI_U32 u32HandleNo, HI_U32 u8SrcLut[12]);
HI_S32 PQ_HAL_SetReg_Chr_Hor_Fade_Lut(HI_U32 u32HandleNo, HI_U32 u8SrcLut[12]);
HI_S32 PQ_HAL_SetReg_Lum_Ver_Fade_Lut(HI_U32 u32HandleNo, HI_U32 p_SrcLut[12]);
HI_S32 PQ_HAL_SetReg_Lum_Ver_Delta_Lut(HI_U32 u32HandleNo, HI_U32 p_SrcLut[16]);
HI_S32 PQ_HAL_GetDBDetEnReg(HI_U32 u32HandleNo, HI_S32* pDet_size_en, HI_S32* pDet_hy_en, HI_S32* pDet_hc_en, HI_S32* pDet_vy_en);
HI_S32 PQ_HAL_GetDBEnReg(HI_U32 u32HandleNo, HI_BOOL* pbOnOff);
HI_S32 PQ_HAL_SetDBDetSizeEn(HI_U32 u32HandleNo, HI_BOOL bOnOff);
/* Set DB Demo Enable */
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32HandleNo, DBM_DEMO_MODE_E enMode);
HI_S32 PQ_HAL_EnableDM(HI_U32 u32HandleNo, HI_BOOL bOnOff);
/* Set DM Demo Enable */
HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg);
/*  Get DR Enable Reg */
HI_S32 PQ_HAL_GetDREnReg(HI_U32 u32HandleNo, HI_BOOL* pbOnOff);
/*  Set DR Demo Mode Enable */
HI_S32 PQ_HAL_EnableDRDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetVpssDemoPos(HI_U32 u32HandleNo, HI_U32 u32DemoPos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

