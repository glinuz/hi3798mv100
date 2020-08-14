/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_dbdr.h
  Version       : Initial Draft
  Author        :
  Created       : 2014/10/16
  Description   : De-blocking and De-Mosquito and SD De-Ringing

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

typedef struct
{
    /* VPSS_DB_CTRL */
    HI_U16 db_lum_en;
    HI_U16 db_lum_ver_en;          /* DB滤波垂直亮度使能 */
    HI_U16 db_lum_hor_en;          /* DB滤波水平亮度使能 */
    HI_U16 db_cb_en;
    HI_U16 db_cr_en;
    HI_U16 db_chr_ver_en;          /* DB滤波垂直色度使能 */
    HI_U16 db_chr_hor_en;          /* DB滤波水平色度使能 */
    HI_U16 db_chr_hor_mid_grey_en;
    //HI_U16 det_size_en;
    //HI_U16 det_hy_en;
    HI_U16 det_hc_en;
    HI_U16 det_vy_en;

    /* VPSS_DBD_TSTBLKNUM */
    HI_U16 hy_tst_blk_num;
    HI_U16 hc_tst_blk_num;
    HI_U16 vy_tst_blk_num;

    /* VPSS_DBD_BLKSIZE */
    HI_U16 min_blk_size;
    HI_U16 hy_max_blk_size;
    HI_U16 hc_max_blk_size;
    HI_U16 vy_max_blk_size;

    /* VPSS_DB_BORD_FLAG */
    HI_U32 vpss_db_bord_flag[128];

    /* VPSS_DB_DIR */
    HI_U16 grad_sub_ratio;

    /* VPSS_DB_BLK */
    HI_U16 lum_h_blk_size;
    HI_U16 lum_v_blk_size;
    HI_U16 chr_h_blk_size;
    HI_U16 chr_v_blk_size;

    /* VPSS_DB_RATIO */
    HI_U16 lum_hor_scale_ratio;
    HI_U16 lum_ver_scale_ratio;
    HI_U16 chr_hor_scale_ratio;
    HI_U16 lum_hor_filter_sel;
    HI_U16 chr_hor_filter_sel;
    HI_U16 lum_hor_txt_win_size;

    /* VPSS_DB_LHHF */
    HI_U16 lum_hor_hf_diff_core;
    HI_U16 lum_hor_hf_diff_gain1;
    HI_U16 lum_hor_hf_diff_gain2;
    HI_U16 lum_hor_hf_var_core;
    HI_U16 lum_hor_bord_adj_gain;
    HI_U16 lum_hor_hf_var_gain1;
    HI_U16 lum_hor_hf_var_gain2;

    /* VPSS_DB_LVHF */
    HI_U16 lum_ver_hf_var_core;
    HI_U16 lum_ver_bord_adj_gain;
    HI_U16 lum_ver_hf_var_gain1;
    HI_U16 lum_ver_hf_var_gain2;
    HI_U16 lum_hor_adj_gain;
    HI_U16 lum_ver_adj_gain;

    /* VPSS_DB_CTRS */
    HI_U16 max_lum_hor_db_dist;
    HI_U16 max_chr_hor_db_dist;

} DB_API_REG_S;

/* De-Blocking运算需要VPSS传递进来的信息 */
typedef struct hiDB_CALC_INFO_S
{
    HI_U32 u32HandleNo;
    HI_U32 u32Width;
    HI_U32 u32Height;
    S_VPSSWB_REGS_TYPE* pstVPSSWbcReg;

} DB_CALC_INFO_S;

typedef enum
{
    DBM_DEMO_ENABLE_L = 0,
    DBM_DEMO_ENABLE_R
} DBM_DEMO_MODE_E;

typedef struct
{
    HI_U8 u8Dm_edge_thr;
    HI_U8 u8Dm_asymtrc_gain;
    HI_U8 u8Dm_edge_gain1;
    HI_U8 u8Dm_edge_gain2;
    HI_U8 u8Dm_sw_thr_gain;
    HI_U8 u8Dm_global_str;
    HI_U8 u8Dm_str_idx;

} DM_API_REG_S;

/* software only */
typedef struct
{
    HI_U32 u32LumWidth;
    HI_U32 u32ChrWidth;
    HI_U32 u32LumHeight;
    HI_S32 s32RateBuf[32];
    HI_S32 s32RateSmthWin;
    HI_S32 s32LumHBlkDetNum;
    HI_S32 s32LumHBlkSize;
    HI_S32* pNewRate;
} DM_INF_S;

typedef struct
{
    HI_U8 GradSubRatio;
    HI_U8 u8CtrstThresh;
    HI_U8 u8EdgeThr;
    HI_U8 u8EdgeGain1;
    HI_U8 u8EdgeGain2;
    HI_U8 u8SWThrGain;
} DM_DynmcPara;


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
/*  Set DB Demo Mode Enable */
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDM(HI_U32 u32HandleNo, HI_BOOL bOnOff);
/*  Set DM Demo Mode Enable */
HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg);
/*  Get DR Enable Reg */
HI_S32 PQ_HAL_GetDREnReg(HI_U32 u32HandleNo, HI_BOOL* pbOnOff);
/*  Set DR Demo Mode Enable */
HI_S32 PQ_HAL_EnableDRDemo(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32HandleNo, DBM_DEMO_MODE_E enMode);
HI_S32 PQ_HAL_SetVpssDemoPos(HI_U32 u32HandleNo, HI_U32 u32DemoPos);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

