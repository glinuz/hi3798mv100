/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dbdm_alg.h
  Version       : Initial Draft
  Author        :
  Created       : 2014-10-16
  Description   : De-blocking and De-Ringning

******************************************************************************/

#ifndef __PQ_MNG_DBDM_ALG_H__
#define __PQ_MNG_DBDM_ALG_H__

#include "hi_math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define MAXLINESIZE 4096

#define DBM_MIN(x, y) (((x) > (y)) ? (y) : (x))
#define DBM_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define DBM_CLIP3(low,high,x) (DBM_MAX(DBM_MIN((high),(x)),(low)))

#define DBM_Blend(F,B,An,Ad)   (F*An + B*(Ad-An) + Ad/2)/Ad




typedef struct DetblkInfo_s
{
    HI_S8  Detsize;
    HI_U32 CounterBorder;
} DetblkInfo_s;

typedef struct  DetStaticUsing_S
{
    /* detect result After time domain calc info */
    HI_S8  UsingSizeHy;
    HI_S8  UsingSizeHchr;
    HI_S8  UsingSizeVy;
    HI_U32 UsingHyCounterBorder;
    HI_U32 UsingVyCounterBorder;

    HI_U32 UsingHchrCounterBorder;
    HI_U8  UsingBorderIndex[MAXLINESIZE];
    HI_U8* ptrUsingHyBorderIndex;
    HI_U8* ptrUsingVyBorderIndex;
    HI_U8* ptrUsingHchrBorderIndex;

    HI_U8  NumberWindow; /* DB Calc time domain window */
    HI_U8  SaturatWindow;

    HI_U8  HyMINBlkNum;
    HI_U8  HchrMINBlkNum;
    HI_U8  VyMINBlkNum;

    HI_U32 frms_NumsHy;
    HI_U32 frms_NumsHchr;
    HI_U32 frms_NumsVy;
    HI_U8  keyfrm_num;
    HI_U8  HyBordCountThreshold;
    HI_U8  HchrBordCountThreshold;
    HI_U8  VyBordCountThreshold;

    DetblkInfo_s Hy[16];
    DetblkInfo_s Vy[16];
    DetblkInfo_s Hchr[16];

    HI_S8  BorderCnt[MAXLINESIZE];
    HI_S8* ptrHyCnt;
    HI_S8* ptrHchrCnt;
    HI_S8* ptrVyCnt;
    HI_U8  frmsCntTempHy;
    HI_U8  frmsCntTempHchr;
    HI_U8  frmsCntTempVy;
    HI_U8  frmsWindowTemp;
    HI_U8  frmsSatWindowTemp;

    HI_S32 FillBorderNumHy;
    HI_S32 FillBorderNumVy;
    HI_S32 FillBorderNumHchr;
} DetStaticUsing_S;

typedef struct
{
    /* basic info, might be modified */
    HI_S32 s32LumWidth;
    HI_S32 s32LumHeight;
    HI_S32 s32ChrWidth;
    HI_S32 s32ChrHeight;
    HI_S32 s32LumHBlkNum; /* [0,480] */
    HI_S32 s32LumVBlkNum; /* [0,180] */
    HI_S32 s32ChrHBlkNum; /* [0,480] */

    HI_S32 det_size_en;
    HI_S32 det_hy_en;
    HI_S32 det_hc_en;
    HI_S32 det_vy_en;

    /* from wbc stt infor */
    HI_S32 hy_counter;
    HI_S32 hy_size;
    HI_S32 vy_counter;
    HI_S32 vy_size;
    HI_S32 hc_counter;
    HI_S32 hc_size;
    HI_S32 db_border[128];
    HI_S32 db_hy_counter[1920];
} DB_DET_INFO_S;

typedef struct DetResultInfo_S
{
    //HI_U32* ptrHyStrRow;
    HI_U32 ptrHyStrRow[MAXLINESIZE];
    HI_U32 HyCounterBorder;            /* [0,1023] */
    HI_U32 HchrCounterBorder;          /* [0,1023] */
    HI_U32 VyCounterBorder;            /* [0,1023] */
    HI_S8  detsize_hy;                 /* [-1,63]  */
    HI_S8  detsize_hchr;               /* [-1,63]  */
    HI_S8  detsize_vy;                 /* [-1,63]  */
    HI_U8  BorderIndex[MAXLINESIZE];
    HI_U8* ptrHyBorderIndex;           /* [0,1]    */
    HI_U8* ptrHchrBorderIndex;         /* [0,1]    */
    HI_U8* ptrVyBorderIndex;           /* [0,1]    */
} DetResultInfo_S;

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

typedef struct
{
    HI_U32 u32LumWidth;
    HI_U32 u32ChrWidth;
    HI_U32 u32LumHeight;
    HI_S32 s32RateBuf[32];
    HI_S32 s32RateSmthWin;
    HI_S32 s32LumHBlkDetNum;
    HI_S32 s32LumHBlkSize;       /* non-interface register [6,64] */
    HI_S32* pNewRate;
} DM_INF_S;

typedef struct
{
    HI_U8 u8Dm_edge_thr;
    HI_U8 u8Dm_asymtrc_gain;
    HI_U8 u8Dm_edge_gain1;
    HI_U8 u8Dm_edge_gain2;
    HI_U8 u8Dm_sw_thr_gain;
    HI_U8 u8Dm_global_str;
    HI_U8 u8Dm_str_idx;
	HI_U8 u8Dm_str_idxT;
	HI_U8 u8Dm_LimintT;
	HI_U8 *pu8DmStrLut;
} DM_API_REG_S;


HI_S32 PQ_MNG_ALG_GetDbDetInfo(DB_DET_INFO_S* p_InfOut, DetStaticUsing_S* p_DetUsing_info, DB_DET_INFO_S* p_InfIn);
HI_S32 PQ_MNG_ALG_DBCfgDetector(DB_API_REG_S* pInput,
                                DB_DET_INFO_S* p_Inf,
                                const DetStaticUsing_S* pDetUsing_info,
                                HI_U32 u32HyTstBlkNum,
                                HI_U32 u32HcTstBlkNum);
HI_S32 PQ_MNG_ALG_UpdateDMCfg(DM_INF_S* pInf, DM_API_REG_S* pstDmApiReg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

