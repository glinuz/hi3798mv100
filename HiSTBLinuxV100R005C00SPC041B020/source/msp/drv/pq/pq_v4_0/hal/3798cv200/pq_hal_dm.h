/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dm.h
  Version       : Initial Draft
  Author        :
  Created       : 2015/10/09
  Description   : De-Mosquito

******************************************************************************/

#ifndef __PQ_HAL_DM_H__
#define __PQ_HAL_DM_H__


#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_dm_alg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* 定义DR模块中寄存器所使用到的参数值 */
typedef struct
{
    //VPSS_DB_CTRL
    HI_BOOL bDM_En;
    HI_U16  u16DbmDemoEn;
    HI_U16  u16DbmDemoMode;
    HI_U16  u16DbmOutSelGain;
    HI_U16  u16DbmOutMode;

    //VPSS_DM_DIR
    HI_U8 u8Dm_grad_sub_ratio;
    HI_U8 u8Dm_ctrst_thresh;
    HI_U8 u8Dm_str_idx_t;
    HI_U8 u8Dm_limit_t;

    //VPSS_DM_EDGE
    HI_U8 u8Dm_edge_thr;
    HI_U8 u8Dm_asymtrc_gain;
    HI_U8 u8Dm_edge_gain1;
    HI_U8 u8Dm_edge_gain2;
    HI_U8 u8Dm_sw_thr_gain;
    HI_U8 u8Dm_global_str;
    HI_U8 u8Dm_str_idx;

    //VPSS_DM_LUT0-VPSS_DM_LUT1
    HI_U8 const* pu8DmSwWhtLut;

    //VPSS_DM_LUT2
    HI_U8 const* pu8DmDirStrGainLut;

    //VPSS_DM_LUT3--VPSS_DM_LUT4
    HI_U8 const* pu8DmDirStrLut;

} DM_PARAM_S;



#if 0
HI_S32 PQ_HAL_SetDMParamRegister(HI_U32 u32HandleNo, DM_PARAM_S* pstDRPara);
#endif
HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8]);
HI_S32 PQ_HAL_SetReg_Dm_Sw_Wht_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[5]);
HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_EnableDM(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


