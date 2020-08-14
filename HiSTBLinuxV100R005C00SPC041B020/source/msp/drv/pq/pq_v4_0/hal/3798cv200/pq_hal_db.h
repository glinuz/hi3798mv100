/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_hal_db.h
  Version       : Initial Draft
  Author        :
  Created       : 2015/10/09
  Description   : De-blocking

******************************************************************************/

#ifndef __PQ_HAL_DB_H__
#define __PQ_HAL_DB_H__


#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_mng_db_alg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/* dbm demo mode */
typedef enum
{
    DBM_DEMO_ENABLE_L = 0, /* Enable Left = 0 */
    DBM_DEMO_ENABLE_R,
    DBM_DEMO_BUTT
} DBM_DEMO_MODE_E;


//定义DB模块中寄存器所使用到的参数值
typedef struct
{
    //VPSS_CTRL
    HI_BOOL bDBM_En;
    HI_U16  u16DbmDemoEn;
    HI_U16  u16DbmDemoMode;
    HI_U16  u16DbmOutSelGain;
    HI_U16  u16DbmOutMode;
    //VPSS_DB_CTRL
    HI_U16 u16Luma_en;
    HI_U16 u16Luma_ver_en;
    HI_U16 u16Luma_hor_en;
    HI_U16 u16Db_cb_en;
    HI_U16 u16Db_cr_en;
    HI_U16 u16Db_chr_ver_en;    // 此版本固定为0
    HI_U16 u16Db_chr_hor_en;
    HI_U16 u16Db_chr_hor_mid_grey_en;
    HI_U16 u16Det_size_en;
    HI_U16 u16Det_hy_en;
    HI_U16 u16Det_hc_en;       // 根据api调节
    HI_U16 u16Det_vy_en;       // 根据api调节

    //VPSS_DBD_THDEDGE
    HI_U16 u16Hy_thd_edge;
    HI_U16 u16Hc_thd_edge;
    HI_U16 u16Vy_thd_edge;

    //VPSS_DBD_THDTXT
    HI_U16 u16Hy_thd_txt;
    HI_U16 u16Hc_thd_txt;
    HI_U16 u16Vy_thd_txt;

    //VPSS_DBD_LUTWGT0
    HI_U16 u16Det_lut_wgt0;
    HI_U16 u16Det_lut_wgt1;
    HI_U16 u16Det_lut_wgt2;
    HI_U16 u16Det_lut_wgt3;
    HI_U16 u16Det_lut_wgt4;
    HI_U16 u16Det_lut_wgt5;
    HI_U16 u16Det_lut_wgt6;
    HI_U16 u16Det_lut_wgt7;
    //VPSS_DBD_LUTWGT1
    HI_U16 u16Det_lut_wgt8;
    HI_U16 u16Det_lut_wgt9;
    HI_U16 u16Det_lut_wgt10;
    HI_U16 u16Det_lut_wgt11;
    HI_U16 u16Det_lut_wgt12;
    HI_U16 u16Det_lut_wgt13;
    HI_U16 u16Det_lut_wgt14;
    HI_U16 u16Det_lut_wgt15;

    //VPSS_DBD_RTNBDTXT
    HI_U16 u16Hy_rtn_bd_txt;
    HI_U16 u16Hc_rtn_bd_txt;
    HI_U16 u16Vy_rtn_bd_txt;

    //VPSS_DBD_TSTBLKNUM
    HI_U16 u16Hy_tst_blk_num;
    HI_U16 u16Hc_tst_blk_num;
    HI_U16 u16Vy_tst_blk_num;

    //VPSS_DBD_HYRDX8BINLUT0
    HI_U16 u16Hy_rdx8bin_lut0;
    HI_U16 u16Hy_rdx8bin_lut1;
    //VPSS_DBD_HYRDX8BINLUT1
    HI_U16 u16Hy_rdx8bin_lut2;
    HI_U16 u16Hy_rdx8bin_lut3;
    //VPSS_DBD_HYRDX8BINLUT2
    HI_U16 u16Hy_rdx8bin_lut4;
    HI_U16 u16Hy_rdx8bin_lut5;
    //VPSS_DBD_HYRDX8BINLUT3
    HI_U16 u16Hy_rdx8bin_lut6;

    //VPSS_DBD_HCRDX8BINLUT0
    HI_U16 u16Hc_rdx8bin_lut0;
    HI_U16 u16Hc_rdx8bin_lut1;
    //VPSS_DBD_HCRDX8BINLUT1
    HI_U16 u16Hc_rdx8bin_lut2;
    HI_U16 u16Hc_rdx8bin_lut3;
    //VPSS_DBD_HCRDX8BINLUT2
    HI_U16 u16Hc_rdx8bin_lut4;
    HI_U16 u16Hc_rdx8bin_lut5;
    //VPSS_DBD_HCRDX8BINLUT3
    HI_U16 u16Hc_rdx8bin_lut6;

    //VPSS_DBD_VYRDX8BINLUT0
    HI_U16 u16Vy_rdx8bin_lut0;
    HI_U16 u16Vy_rdx8bin_lut1;
    //VPSS_DBD_VYRDX8BINLUT1
    HI_U16 u16Vy_rdx8bin_lut2;
    HI_U16 u16Vy_rdx8bin_lut3;
    //VPSS_DBD_VYRDX8BINLUT2
    HI_U16 u16Vy_rdx8bin_lut4;
    HI_U16 u16Vy_rdx8bin_lut5;
    //VPSS_DBD_VYRDX8BINLUT3
    HI_U16 u16Vy_rdx8bin_lut6;

    //VPSS_DBD_BLKSIZE
    HI_U16 u16Min_blk_size;
    HI_U16 u16Hy_max_blk_size;  // 需根据api调节
    HI_U16 u16Hc_max_blk_size;  // 需根据api调节
    HI_U16 u16Vy_max_blk_size;  // 需根据api调节

    //VPSS_DB_BORD_FLAG
    HI_U32 u32Vpss_db_bord_flag[128];

    //VPSS_DB_DIR
    HI_U16 u16Grad_sub_ratio;
    HI_U16 u16Ctrst_thresh;

    //VPSS_DB_BLK
    HI_U16 u16Lum_h_blk_size;   // l_h_bs;
    HI_U16 u16Lum_v_blk_size;   // l_v_bs;
    HI_U16 u16Chr_h_blk_size;   // c_h_bs;
    HI_U16 u16Chr_v_blk_size;

    //VPSS_DB_RATIO
    HI_U16 u16Lum_hor_scale_ratio;
    HI_U16 u16Lum_ver_scale_ratio;
    HI_U16 u16Chr_hor_scale_ratio;
    HI_U16 u16Chr_ver_scale_ratio;
    HI_U16 u16Lum_hor_filter_sel;
    HI_U16 u16Chr_hor_filter_sel;
    HI_U16 u16Glabal_db_strenth_lum;
    HI_U16 u16Glabal_db_strenth_chr;
    HI_U16 u16Lum_hor_txt_win_size;

    //VPSS_DB_LHHF
    HI_U16 u16Lum_hor_hf_diff_core;
    HI_U16 u16Lum_hor_hf_diff_gain1;
    HI_U16 u16Lum_hor_hf_diff_gain2;
    HI_U16 u16Lum_hor_hf_var_core;
    HI_U16 u16Lum_hor_bord_adj_gain;
    HI_U16 u16Lum_hor_hf_var_gain1;
    HI_U16 u16Lum_hor_hf_var_gain2;

    //VPSS_DB_LVHF
    HI_U16 u16Lum_ver_hf_var_core;
    HI_U16 u16Lum_ver_bord_adj_gain;
    HI_U16 u16Lum_ver_hf_var_gain1;
    HI_U16 u16Lum_ver_hf_var_gain2;
    HI_U16 u16Lum_hor_adj_gain;
    HI_U16 u16Lum_ver_adj_gain;

    //VPSS_DB_CTRS
    HI_U16 u16Ctrst_adj_core;
    HI_U16 u16Dir_smooth_mode;
    HI_U16 u16Ctrst_adj_gain1;
    HI_U16 u16Ctrst_adj_gain2;
    HI_U16 u16Max_lum_hor_db_dist;
    HI_U16 u16Max_chr_hor_db_dist;

    //VPSS_DB_LUT0--VPSS_DB_LUT1
    HI_U8 const* pu8Lum_hor_delta_lut;

    //VPSS_DB_LUT2--VPSS_DB_LUT3
    HI_U8 const* pu8Lum_ver_delta_lut;

    //VPSS_DB_LUT4--VPSS_DB_LUT5
    HI_U8 const* pu8Chr_hor_delta_lut;

    //VPSS_DB_LUT6,VPSS_DB_LUT9
    HI_U8 const* pu8Lum_h_str_fade_lut;

    //VPSS_DB_LUT7,VPSS_DB_LUT9
    HI_U8 const* pu8Lum_v_str_fade_lut;

    //VPSS_DB_LUT8,VPSS_DB_LUT9
    HI_U8 const* pu8Chr_h_str_fade_lut;

    //VPSS_DB_LUT10
    HI_U8 const* pu8Dir_str_gain_lut; //p0-p7

    //VPSS_DB_LUT11,VPSS_DB_LUT12
    HI_U8 const* pu8Dir_str_lut;     //p0-p15

} DB_PARAM_S;

//DB强度值,对应VPSS_DB_WEIGHT寄存器
typedef struct
{
    HI_U8    u8HWeight;  // 6bits [0,63]
    HI_U8    u8VWeight;  // 6bits [0,63]
} DB_WEIGHT_S;


HI_S32 PQ_HAL_SetDBApiReg(HI_U32 u32HandleNo, DB_API_REG_S* pstApiPara);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut1(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut2(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut3(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut4(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Lum_V_Str_Fade_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[2]);
HI_S32 PQ_HAL_SetReg_Sobel_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[26]);
HI_S32 PQ_HAL_SetReg_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[10]);
HI_S32 PQ_HAL_SetReg_Diff_Map_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8]);
HI_S32 PQ_HAL_SetReg_Diff_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[7]);
HI_S32 PQ_HAL_SetReg_Diff_Weight_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[6]);
HI_S32 PQ_HAL_SetReg_Hor_Delta_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);
HI_S32 PQ_HAL_SetReg_Ver_Delta_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16]);

HI_S32 PQ_HAL_GetDBDetEnReg(HI_U32 u32Data, HI_S32* pDet_size_en,
                            HI_S32* pDet_hy_en, HI_S32* pDet_hc_en, HI_S32* pDet_vy_en);
HI_S32 PQ_HAL_GetDbmEnReg(HI_U32 u32HandleNo, HI_BOOL* pbDbEn);
HI_S32 PQ_HAL_SetDBDetSizeEn(HI_U32 u32HandleNo, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDB(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32Data, HI_BOOL bOnOff);
HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32Data, DBM_DEMO_MODE_E enMode);
HI_S32 PQ_HAL_DBDemoPos(HI_U32 u32HandleNo, HI_U32 u32DBDemoPos);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif



