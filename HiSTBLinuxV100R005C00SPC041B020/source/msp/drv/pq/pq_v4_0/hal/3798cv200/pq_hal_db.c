/******************************************************************************

Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     : pq_hal_db.c
Version       : Initial Draft
Author        :
Created       : 2015/10/09
Description   : De-blocking

******************************************************************************/
#include <linux/string.h>   /* memcpy() */
#include "pq_hal_db.h"
#include "pq_hal_comm.h"
#include "drv_pq_comm.h"
//#include "hi_math.h"


#if 0
HI_U32 u32VpssVirReg = 0;

static HI_S32 DB_SetParamToReg(VPSS_REG_S* pstVpssReg, DB_PARAM_S* pstDBValue)
{
    PQ_CHECK_NULL_PTR(pstVpssReg);
    PQ_CHECK_NULL_PTR(pstDBValue);

    PQ_CHECK_NULL_PTR(pstDBValue->pu8Lum_hor_delta_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Lum_ver_delta_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Chr_hor_delta_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Lum_h_str_fade_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Lum_v_str_fade_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Chr_h_str_fade_lut);
    PQ_CHECK_NULL_PTR(pstDBValue->pu8Dir_str_gain_lut);
    //VPSS_CTRL db_en control by vpss
    pstVpssReg->VPSS_CTRL.bits.dbm_en   = pstDBValue->bDBM_En;
    pstVpssReg->VPSS_DB_CTRL.bits.db_en = pstDBValue->bDBM_En;

    //VPSS_DB_CTRL
    //pstVpssReg->VPSS_DB_CTRL.bits.db_lum_en            = pstDBValue->u16Luma_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_lum_ver_en          = pstDBValue->u16Luma_ver_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_lum_hor_en          = pstDBValue->u16Luma_hor_en;
    /*
    pstVpssReg->VPSS_DB_CTRL.bits.db_cb_en               = pstDBValue->u16Db_cb_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_cr_en               = pstDBValue->u16Db_cr_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_chr_ver_en          = pstDBValue->u16Db_chr_ver_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_chr_hor_en          = pstDBValue->u16Db_chr_hor_en;
    pstVpssReg->VPSS_DB_CTRL.bits.db_chr_hor_mid_grey_en = pstDBValue->u16Db_chr_hor_mid_grey_en;
    */
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_en            = pstDBValue->u16DbmDemoEn;
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_mode          = pstDBValue->u16DbmDemoMode;
    pstVpssReg->VPSS_CTRL.bits.blk_det_en                = pstDBValue->u16Det_size_en;
    pstVpssReg->VPSS_DB_CTRL1.bits.db_det_en             = pstDBValue->u16Det_size_en;
    pstVpssReg->VPSS_DB_CTRL.bits.det_hy_en              = pstDBValue->u16Det_hy_en;
    pstVpssReg->VPSS_DB_CTRL.bits.det_hc_en              = pstDBValue->u16Det_hc_en;
    pstVpssReg->VPSS_DB_CTRL.bits.det_vy_en              = pstDBValue->u16Det_vy_en;
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_out_sel_gain       = pstDBValue->u16DbmOutSelGain;
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_out_mode           = pstDBValue->u16DbmOutMode;

    //VPSS_DBD_THDEDGE
    pstVpssReg->VPSS_DBD_THDEDGE.bits.dbd_hy_thd_edge = pstDBValue->u16Hy_thd_edge;
    pstVpssReg->VPSS_DBD_THDEDGE.bits.dbd_hc_thd_edge = pstDBValue->u16Hc_thd_edge;
    pstVpssReg->VPSS_DBD_THDEDGE.bits.dbd_vy_thd_edge = pstDBValue->u16Vy_thd_edge;

    //VPSS_DBD_THDTXT
    pstVpssReg->VPSS_DBD_THDTXT.bits.dbd_hy_thd_txt = pstDBValue->u16Hy_thd_txt;
    pstVpssReg->VPSS_DBD_THDTXT.bits.dbd_hc_thd_txt = pstDBValue->u16Hc_thd_txt;
    pstVpssReg->VPSS_DBD_THDTXT.bits.dbd_vy_thd_txt = pstDBValue->u16Vy_thd_txt;

    //VPSS_DBD_LUTWGT0
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt0  = pstDBValue->u16Det_lut_wgt0;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt1  = pstDBValue->u16Det_lut_wgt1;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt2  = pstDBValue->u16Det_lut_wgt2;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt3  = pstDBValue->u16Det_lut_wgt3;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt4  = pstDBValue->u16Det_lut_wgt4;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt5  = pstDBValue->u16Det_lut_wgt5;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt6  = pstDBValue->u16Det_lut_wgt6;
    pstVpssReg->VPSS_DBD_LUTWGT0.bits.dbd_det_lut_wgt7  = pstDBValue->u16Det_lut_wgt7;
    //VPSS_DBD_LUTWGT1
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt8  = pstDBValue->u16Det_lut_wgt8;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt9  = pstDBValue->u16Det_lut_wgt9;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt10 = pstDBValue->u16Det_lut_wgt10;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt11 = pstDBValue->u16Det_lut_wgt11;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt12 = pstDBValue->u16Det_lut_wgt12;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt13 = pstDBValue->u16Det_lut_wgt13;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt14 = pstDBValue->u16Det_lut_wgt14;
    pstVpssReg->VPSS_DBD_LUTWGT1.bits.dbd_det_lut_wgt15 = pstDBValue->u16Det_lut_wgt15;

    //VPSS_DBD_RTNBDTXT
    pstVpssReg->VPSS_DBD_RTNBDTXT.bits.dbd_hy_rtn_bd_txt = pstDBValue->u16Hy_rtn_bd_txt;
    pstVpssReg->VPSS_DBD_RTNBDTXT.bits.dbd_hc_rtn_bd_txt = pstDBValue->u16Hc_rtn_bd_txt;
    pstVpssReg->VPSS_DBD_RTNBDTXT.bits.dbd_vy_rtn_bd_txt = pstDBValue->u16Vy_rtn_bd_txt;

    //VPSS_DBD_TSTBLKNUM
    pstVpssReg->VPSS_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num = pstDBValue->u16Hy_tst_blk_num;
    pstVpssReg->VPSS_DBD_TSTBLKNUM.bits.dbd_hc_tst_blk_num = pstDBValue->u16Hc_tst_blk_num;
    pstVpssReg->VPSS_DBD_TSTBLKNUM.bits.dbd_vy_tst_blk_num = pstDBValue->u16Vy_tst_blk_num;

    //VPSS_DBD_HYRDX8BINLUT0
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut0 = pstDBValue->u16Hy_rdx8bin_lut0;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut1 = pstDBValue->u16Hy_rdx8bin_lut1;
    //VPSS_DBD_HYRDX8BINLUT1
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut2 = pstDBValue->u16Hy_rdx8bin_lut2;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut3 = pstDBValue->u16Hy_rdx8bin_lut3;
    //VPSS_DBD_HYRDX8BINLUT2
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut4 = pstDBValue->u16Hy_rdx8bin_lut4;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut5 = pstDBValue->u16Hy_rdx8bin_lut5;
    //VPSS_DBD_HYRDX8BINLUT3
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT3.bits.dbd_hy_rdx8bin_lut6 = pstDBValue->u16Hy_rdx8bin_lut6;

    //VPSS_DBD_HCRDX8BINLUT0
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT0.bits.dbd_hc_rdx8bin_lut0 = pstDBValue->u16Hc_rdx8bin_lut0;
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT0.bits.dbd_hc_rdx8bin_lut1 = pstDBValue->u16Hc_rdx8bin_lut1;
    //VPSS_DBD_HCRDX8BINLUT1
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT1.bits.dbd_hc_rdx8bin_lut2 = pstDBValue->u16Hc_rdx8bin_lut2;
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT1.bits.dbd_hc_rdx8bin_lut3 = pstDBValue->u16Hc_rdx8bin_lut3;
    //VPSS_DBD_HCRDX8BINLUT2
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT2.bits.dbd_hc_rdx8bin_lut4 = pstDBValue->u16Hc_rdx8bin_lut4;
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT2.bits.dbd_hc_rdx8bin_lut5 = pstDBValue->u16Hc_rdx8bin_lut5;
    //VPSS_DBD_HCRDX8BINLUT3
    pstVpssReg->VPSS_DBD_HCRDX8BINLUT3.bits.dbd_hc_rdx8bin_lut6 = pstDBValue->u16Hc_rdx8bin_lut6;

    //VPSS_DBD_VYRDX8BINLUT0
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT0.bits.dbd_vy_rdx8bin_lut0 = pstDBValue->u16Vy_rdx8bin_lut0;
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT0.bits.dbd_vy_rdx8bin_lut1 = pstDBValue->u16Vy_rdx8bin_lut1;
    //VPSS_DBD_VYRDX8BINLUT1
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT1.bits.dbd_vy_rdx8bin_lut2 = pstDBValue->u16Vy_rdx8bin_lut2;
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT1.bits.dbd_vy_rdx8bin_lut3 = pstDBValue->u16Vy_rdx8bin_lut3;
    //VPSS_DBD_VYRDX8BINLUT2
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT2.bits.dbd_vy_rdx8bin_lut4 = pstDBValue->u16Vy_rdx8bin_lut4;
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT2.bits.dbd_vy_rdx8bin_lut5 = pstDBValue->u16Vy_rdx8bin_lut5;
    //VPSS_DBD_VYRDX8BINLUT3
    pstVpssReg->VPSS_DBD_VYRDX8BINLUT3.bits.dbd_vy_rdx8bin_lut6 = pstDBValue->u16Vy_rdx8bin_lut6;

    //VPSS_DBD_BLKSIZE
    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_min_blk_size    = pstDBValue->u16Min_blk_size;
    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_hy_max_blk_size = pstDBValue->u16Hy_max_blk_size;
    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_hc_max_blk_size = pstDBValue->u16Hc_max_blk_size;
    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_vy_max_blk_size = pstDBValue->u16Vy_max_blk_size;

    {
        int i = 0;
        //VPSS_DB_BORD_FLAG
        for (i = 0; i < 128; ++i)
        {
            pstVpssReg->VPSS_DB_BORD_FLAG[i] = pstDBValue->u32Vpss_db_bord_flag[i];
        }
    }
    //VPSS_DB_DIR
    pstVpssReg->VPSS_DB_THR0.bits.db_grad_sub_ratio = pstDBValue->u16Grad_sub_ratio;
    pstVpssReg->VPSS_DB_THR0.bits.db_ctrst_thresh   = pstDBValue->u16Ctrst_thresh;

    /*
        //VPSS_DB_BLK
        pstVpssReg->VPSS_DB_BLK.bits.db_lum_h_blk_size = pstDBValue->u16Lum_h_blk_size;
        pstVpssReg->VPSS_DB_BLK.bits.db_lum_v_blk_size = pstDBValue->u16Lum_v_blk_size;
        pstVpssReg->VPSS_DB_BLK.bits.db_chr_h_blk_size = pstDBValue->u16Chr_h_blk_size;
        pstVpssReg->VPSS_DB_BLK.bits.db_chr_v_blk_size = pstDBValue->u16Chr_v_blk_size;

        //VPSS_DB_RATIO
        pstVpssReg->VPSS_DB_RATIO.bits.lum_hor_scale_ratio   = pstDBValue->u16Lum_hor_scale_ratio;
        pstVpssReg->VPSS_DB_RATIO.bits.lum_ver_scale_ratio   = pstDBValue->u16Lum_ver_scale_ratio;
        pstVpssReg->VPSS_DB_RATIO.bits.chr_hor_scale_ratio   = pstDBValue->u16Chr_hor_scale_ratio;
        pstVpssReg->VPSS_DB_RATIO.bits.chr_ver_scale_ratio   = pstDBValue->u16Chr_ver_scale_ratio;
        pstVpssReg->VPSS_DB_RATIO.bits.lum_hor_filter_sel    = pstDBValue->u16Lum_hor_filter_sel;
        pstVpssReg->VPSS_DB_RATIO.bits.chr_hor_filter_sel    = pstDBValue->u16Chr_hor_filter_sel;
        pstVpssReg->VPSS_DB_RATIO.bits.global_db_strenth_lum = pstDBValue->u16Glabal_db_strenth_lum;
        pstVpssReg->VPSS_DB_RATIO.bits.global_db_strenth_chr = pstDBValue->u16Glabal_db_strenth_chr;
        pstVpssReg->VPSS_DB_RATIO.bits.lum_hor_txt_win_size  = pstDBValue->u16Lum_hor_txt_win_size;
    */
    /*
        //VPSS_DB_LHHF
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_core  = pstDBValue->u16Lum_hor_hf_diff_core;
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_gain1 = pstDBValue->u16Lum_hor_hf_diff_gain1;
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_gain2 = pstDBValue->u16Lum_hor_hf_diff_gain2;
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_core   = pstDBValue->u16Lum_hor_hf_var_core;
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_gain1  = pstDBValue->u16Lum_hor_hf_var_gain1;
        pstVpssReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_gain2  = pstDBValue->u16Lum_hor_hf_var_gain2;
    */

    /*
        //VPSS_DB_LVHF
        pstVpssReg->VPSS_DB_LVHF.bits.lum_ver_adj_gain      = pstDBValue->u16Lum_ver_adj_gain;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_ver_bord_adj_gain = pstDBValue->u16Lum_ver_bord_adj_gain;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_gain1  = pstDBValue->u16Lum_ver_hf_var_gain1;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_gain2  = pstDBValue->u16Lum_ver_hf_var_gain2;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_hor_adj_gain      = pstDBValue->u16Lum_hor_adj_gain;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_hor_bord_adj_gain = pstDBValue->u16Lum_hor_bord_adj_gain;
        pstVpssReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_core   = pstDBValue->u16Lum_ver_hf_var_core;
    */

    /*
        //VPSS_DB_CTRS
        pstVpssReg->VPSS_DB_CTRS.bits.ctrst_adj_core      = pstDBValue->u16Ctrst_adj_core;
        pstVpssReg->VPSS_DB_CTRS.bits.dir_smooth_mode     = pstDBValue->u16Dir_smooth_mode;
        pstVpssReg->VPSS_DB_CTRS.bits.ctrst_adj_gain1     = pstDBValue->u16Ctrst_adj_gain1;
        pstVpssReg->VPSS_DB_CTRS.bits.ctrst_adj_gain2     = pstDBValue->u16Ctrst_adj_gain2;
        pstVpssReg->VPSS_DB_CTRS.bits.max_lum_hor_db_dist = pstDBValue->u16Max_lum_hor_db_dist;
        pstVpssReg->VPSS_DB_CTRS.bits.max_chr_hor_db_dist = pstDBValue->u16Max_chr_hor_db_dist;
    */

    /*
        //VPSS_DB_LUT0
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p0  = pstDBValue->pu8Lum_hor_delta_lut[0];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p1  = pstDBValue->pu8Lum_hor_delta_lut[1];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p2  = pstDBValue->pu8Lum_hor_delta_lut[2];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p3  = pstDBValue->pu8Lum_hor_delta_lut[3];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p4  = pstDBValue->pu8Lum_hor_delta_lut[4];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p5  = pstDBValue->pu8Lum_hor_delta_lut[5];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p6  = pstDBValue->pu8Lum_hor_delta_lut[6];
        pstVpssReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p7  = pstDBValue->pu8Lum_hor_delta_lut[7];
        //VPSS_DB_LUT1
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p8  = pstDBValue->pu8Lum_hor_delta_lut[8];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p9  = pstDBValue->pu8Lum_hor_delta_lut[9];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p10 = pstDBValue->pu8Lum_hor_delta_lut[10];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p11 = pstDBValue->pu8Lum_hor_delta_lut[11];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p12 = pstDBValue->pu8Lum_hor_delta_lut[12];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p13 = pstDBValue->pu8Lum_hor_delta_lut[13];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p14 = pstDBValue->pu8Lum_hor_delta_lut[14];
        pstVpssReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p15 = pstDBValue->pu8Lum_hor_delta_lut[15];
    */
    /*
        //VPSS_DB_LUT2
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p0  = pstDBValue->pu8Lum_ver_delta_lut[0];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p1  = pstDBValue->pu8Lum_ver_delta_lut[1];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p2  = pstDBValue->pu8Lum_ver_delta_lut[2];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p3  = pstDBValue->pu8Lum_ver_delta_lut[3];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p4  = pstDBValue->pu8Lum_ver_delta_lut[4];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p5  = pstDBValue->pu8Lum_ver_delta_lut[5];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p6  = pstDBValue->pu8Lum_ver_delta_lut[6];
        pstVpssReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p7  = pstDBValue->pu8Lum_ver_delta_lut[7];
        //VPSS_DB_LUT3
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p8  = pstDBValue->pu8Lum_ver_delta_lut[8];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p9  = pstDBValue->pu8Lum_ver_delta_lut[9];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p10 = pstDBValue->pu8Lum_ver_delta_lut[10];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p11 = pstDBValue->pu8Lum_ver_delta_lut[11];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p12 = pstDBValue->pu8Lum_ver_delta_lut[12];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p13 = pstDBValue->pu8Lum_ver_delta_lut[13];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p14 = pstDBValue->pu8Lum_ver_delta_lut[14];
        pstVpssReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p15 = pstDBValue->pu8Lum_ver_delta_lut[15];
        //VPSS_DB_LUT4
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p0  = pstDBValue->pu8Chr_hor_delta_lut[0];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p1  = pstDBValue->pu8Chr_hor_delta_lut[1];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p2  = pstDBValue->pu8Chr_hor_delta_lut[2];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p3  = pstDBValue->pu8Chr_hor_delta_lut[3];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p4  = pstDBValue->pu8Chr_hor_delta_lut[4];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p5  = pstDBValue->pu8Chr_hor_delta_lut[5];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p6  = pstDBValue->pu8Chr_hor_delta_lut[6];
        pstVpssReg->VPSS_DB_LUT4.bits.chr_hor_delta_lut_p7  = pstDBValue->pu8Chr_hor_delta_lut[7];
        //VPSS_DB_LUT5
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p8  = pstDBValue->pu8Chr_hor_delta_lut[8];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p9  = pstDBValue->pu8Chr_hor_delta_lut[9];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p10 = pstDBValue->pu8Chr_hor_delta_lut[10];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p11 = pstDBValue->pu8Chr_hor_delta_lut[11];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p12 = pstDBValue->pu8Chr_hor_delta_lut[12];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p13 = pstDBValue->pu8Chr_hor_delta_lut[13];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p14 = pstDBValue->pu8Chr_hor_delta_lut[14];
        pstVpssReg->VPSS_DB_LUT5.bits.chr_hor_delta_lut_p15 = pstDBValue->pu8Chr_hor_delta_lut[15];
    */
    /*
        //VPSS_DB_LUT6
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p0  = pstDBValue->pu8Lum_h_str_fade_lut[0];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p1  = pstDBValue->pu8Lum_h_str_fade_lut[1];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p2  = pstDBValue->pu8Lum_h_str_fade_lut[2];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p3  = pstDBValue->pu8Lum_h_str_fade_lut[3];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p4  = pstDBValue->pu8Lum_h_str_fade_lut[4];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p5  = pstDBValue->pu8Lum_h_str_fade_lut[5];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p6  = pstDBValue->pu8Lum_h_str_fade_lut[6];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p7  = pstDBValue->pu8Lum_h_str_fade_lut[7];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p8  = pstDBValue->pu8Lum_h_str_fade_lut[8];
        pstVpssReg->VPSS_DB_LUT6.bits.lum_h_str_fade_lut_p9  = pstDBValue->pu8Lum_h_str_fade_lut[9];
        //VPSS_DB_LUT7
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p0  = pstDBValue->pu8Lum_v_str_fade_lut[0];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p1  = pstDBValue->pu8Lum_v_str_fade_lut[1];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p2  = pstDBValue->pu8Lum_v_str_fade_lut[2];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p3  = pstDBValue->pu8Lum_v_str_fade_lut[3];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p4  = pstDBValue->pu8Lum_v_str_fade_lut[4];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p5  = pstDBValue->pu8Lum_v_str_fade_lut[5];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p6  = pstDBValue->pu8Lum_v_str_fade_lut[6];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p7  = pstDBValue->pu8Lum_v_str_fade_lut[7];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p8  = pstDBValue->pu8Lum_v_str_fade_lut[8];
        pstVpssReg->VPSS_DB_LUT7.bits.lum_v_str_fade_lut_p9  = pstDBValue->pu8Lum_v_str_fade_lut[9];
        //VPSS_DB_LUT8
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p0  = pstDBValue->pu8Chr_h_str_fade_lut[0];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p1  = pstDBValue->pu8Chr_h_str_fade_lut[1];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p2  = pstDBValue->pu8Chr_h_str_fade_lut[2];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p3  = pstDBValue->pu8Chr_h_str_fade_lut[3];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p4  = pstDBValue->pu8Chr_h_str_fade_lut[4];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p5  = pstDBValue->pu8Chr_h_str_fade_lut[5];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p6  = pstDBValue->pu8Chr_h_str_fade_lut[6];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p7  = pstDBValue->pu8Chr_h_str_fade_lut[7];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p8  = pstDBValue->pu8Chr_h_str_fade_lut[8];
        pstVpssReg->VPSS_DB_LUT8.bits.chr_h_str_fade_lut_p9  = pstDBValue->pu8Chr_h_str_fade_lut[9];
        //VPSS_DB_LUT9
        pstVpssReg->VPSS_DB_LUT9.bits.lum_h_str_fade_lut_p10 = pstDBValue->pu8Lum_h_str_fade_lut[10];
        pstVpssReg->VPSS_DB_LUT9.bits.lum_h_str_fade_lut_p11 = pstDBValue->pu8Lum_h_str_fade_lut[11];
        pstVpssReg->VPSS_DB_LUT9.bits.lum_v_str_fade_lut_p10 = pstDBValue->pu8Lum_v_str_fade_lut[10];
        pstVpssReg->VPSS_DB_LUT9.bits.lum_v_str_fade_lut_p11 = pstDBValue->pu8Lum_v_str_fade_lut[11];
        pstVpssReg->VPSS_DB_LUT9.bits.chr_h_str_fade_lut_p10 = pstDBValue->pu8Chr_h_str_fade_lut[10];
        pstVpssReg->VPSS_DB_LUT9.bits.chr_h_str_fade_lut_p11 = pstDBValue->pu8Chr_h_str_fade_lut[11];
    */
    /*
        //VPSS_DB_LUT10
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p0 = pstDBValue->pu8Dir_str_gain_lut[0];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p1 = pstDBValue->pu8Dir_str_gain_lut[1];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p2 = pstDBValue->pu8Dir_str_gain_lut[2];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p3 = pstDBValue->pu8Dir_str_gain_lut[3];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p4 = pstDBValue->pu8Dir_str_gain_lut[4];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p5 = pstDBValue->pu8Dir_str_gain_lut[5];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p6 = pstDBValue->pu8Dir_str_gain_lut[6];
        pstVpssReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p7 = pstDBValue->pu8Dir_str_gain_lut[7];

        //VPSS_DB_LUT11
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p0 = pstDBValue->pu8Dir_str_lut[0];
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p1 = pstDBValue->pu8Dir_str_lut[1];
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p2 = pstDBValue->pu8Dir_str_lut[2];
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p3 = pstDBValue->pu8Dir_str_lut[3];
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p4 = pstDBValue->pu8Dir_str_lut[4];
        pstVpssReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p5 = pstDBValue->pu8Dir_str_lut[5];

        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p6 = pstDBValue->pu8Dir_str_lut[6];
        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p7 = pstDBValue->pu8Dir_str_lut[7];
        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p8 = pstDBValue->pu8Dir_str_lut[8];
        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p9 = pstDBValue->pu8Dir_str_lut[9];
        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p10 = pstDBValue->pu8Dir_str_lut[10];
        pstVpssReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p11 = pstDBValue->pu8Dir_str_lut[11];

        pstVpssReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p12 = pstDBValue->pu8Dir_str_lut[12];
        pstVpssReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p13 = pstDBValue->pu8Dir_str_lut[13];
        pstVpssReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p14 = pstDBValue->pu8Dir_str_lut[14];
        pstVpssReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p15 = pstDBValue->pu8Dir_str_lut[15];
    */
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDBParamRegister(HI_U32 u32HandleNo, DB_PARAM_S* pstDBPara)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;
    HI_S32 s32Ret;

    PQ_CHECK_NULL_PTR(pstDBPara);
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    s32Ret = DB_SetParamToReg(pstVpssVirReg, pstDBPara);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ SetDBDefaultValue failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

/* 设置DB软算法api计算出来的参数值 */
HI_S32 PQ_HAL_SetDBApiReg(HI_U32 u32HandleNo, DB_API_REG_S* pstApiPara)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;
    DB_REG_S* pstDbRegs;
    VDH_INFO_S* pstTransVDHInf;
    HI_S32 i = 0;

    PQ_CHECK_NULL_PTR(pstApiPara);
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstDbRegs = &(pstApiPara->stDbRegs);
    pstTransVDHInf = &(pstApiPara->stTransVDHInf);
#if 0
    pstApiPara->db_lum_en = 1;
    pstApiPara->db_lum_ver_en = 0;
    pstApiPara->db_lum_hor_en = 0;
    pstApiPara->db_cb_en = 0;
    pstApiPara->db_cr_en = 0;
    pstApiPara->db_chr_ver_en = 0;
    pstApiPara->db_chr_hor_en = 0;
    pstApiPara->db_chr_hor_mid_grey_en = 0;
    pstApiPara->det_size_en = 0;
    pstApiPara->det_hy_en = 0;
    pstApiPara->det_hc_en = 0;
    pstApiPara->det_vy_en = 0;
#endif

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        //VPSS_DB_CTRL
        pstVpssVirReg->VPSS_DB_CTRL.bits.db_lum_ver_en          = pstDbRegs->u8DBLumVerTrue;//modify by z214841 2015.7.21
        pstVpssVirReg->VPSS_DB_CTRL.bits.db_lum_hor_en          = pstDbRegs->u8DBLumHorTrue;//modify by z214841 2015.7.21
        //pstVpssVirReg->VPSS_DB_CTRL.bits.db_chr_ver_en          = pstApiPara->db_chr_ver_en;
        //pstVpssVirReg->VPSS_DB_CTRL.bits.db_chr_hor_en          = pstApiPara->db_chr_hor_en;
        //pstVpssVirReg->VPSS_DB_CTRL.bits.db_chr_hor_mid_grey_en = pstApiPara->db_chr_hor_mid_grey_en;

        /* VPSS不控制det 使能 */
        pstVpssVirReg->VPSS_DB_CTRL1.bits.db_det_en             = pstApiPara->det_size_en;
        pstVpssVirReg->VPSS_CTRL.bits.blk_det_en                = pstApiPara->det_size_en;

        pstVpssVirReg->VPSS_DB_CTRL.bits.det_hy_en              = pstApiPara->det_hy_en;
        pstVpssVirReg->VPSS_DB_CTRL.bits.det_hc_en              = pstApiPara->det_hc_en;
        pstVpssVirReg->VPSS_DB_CTRL.bits.det_vy_en              = pstApiPara->det_vy_en;

        //VPSS_DBD_TSTBLKNUM
        pstVpssVirReg->VPSS_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num = pstApiPara->hy_tst_blk_num;
        pstVpssVirReg->VPSS_DBD_TSTBLKNUM.bits.dbd_hc_tst_blk_num = pstApiPara->hc_tst_blk_num;
        pstVpssVirReg->VPSS_DBD_TSTBLKNUM.bits.dbd_vy_tst_blk_num = pstApiPara->vy_tst_blk_num;

        //VPSS_DBD_BLKSIZE
        pstVpssVirReg->VPSS_DBD_BLKSIZE.bits.dbd_min_blk_size    = pstApiPara->min_blk_size;
        pstVpssVirReg->VPSS_DBD_BLKSIZE.bits.dbd_hy_max_blk_size = pstApiPara->hy_max_blk_size;
        pstVpssVirReg->VPSS_DBD_BLKSIZE.bits.dbd_hc_max_blk_size = pstApiPara->hc_max_blk_size;
        pstVpssVirReg->VPSS_DBD_BLKSIZE.bits.dbd_vy_max_blk_size = pstApiPara->vy_max_blk_size;

        //VPSS_DB_BORD_FLAG
        for (i = 0; i < 128; ++i)
        {
            pstVpssVirReg->VPSS_DB_BORD_FLAG[i] = pstApiPara->vpss_db_bord_flag[i];
        }

        //VPSS_DB_DIR
        //pstVpssVirReg->VPSS_DB_DIR.bits.grad_sub_ratio = pstApiPara->grad_sub_ratio;
        pstVpssVirReg->VPSS_DB_THR0.bits.db_grad_sub_ratio = pstDbRegs->u8DBGradSubRatio;//modify by z214841 2015.7.21

        /*
        //VPSS_DB_BLK
        pstVpssVirReg->VPSS_DB_BLK.bits.lum_h_blk_size = pstApiPara->lum_h_blk_size;
        pstVpssVirReg->VPSS_DB_BLK.bits.lum_v_blk_size = pstApiPara->lum_v_blk_size;
        pstVpssVirReg->VPSS_DB_BLK.bits.chr_h_blk_size = pstApiPara->chr_h_blk_size;
        pstVpssVirReg->VPSS_DB_BLK.bits.chr_v_blk_size = pstApiPara->chr_v_blk_size;

        //VPSS_DB_RATIO
        pstVpssVirReg->VPSS_DB_RATIO.bits.lum_hor_scale_ratio  = pstApiPara->lum_hor_scale_ratio;
        pstVpssVirReg->VPSS_DB_RATIO.bits.lum_ver_scale_ratio  = pstApiPara->lum_ver_scale_ratio;
        pstVpssVirReg->VPSS_DB_RATIO.bits.chr_hor_scale_ratio  = pstApiPara->chr_hor_scale_ratio;
        pstVpssVirReg->VPSS_DB_RATIO.bits.lum_hor_filter_sel   = pstApiPara->lum_hor_filter_sel;
        pstVpssVirReg->VPSS_DB_RATIO.bits.chr_hor_filter_sel   = pstApiPara->chr_hor_filter_sel;
        pstVpssVirReg->VPSS_DB_RATIO.bits.lum_hor_txt_win_size = pstApiPara->lum_hor_txt_win_size;
        //VPSS_DB_LHHF
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_core  = pstApiPara->lum_hor_hf_diff_core;
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_gain1 = pstApiPara->lum_hor_hf_diff_gain1;
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_diff_gain2 = pstApiPara->lum_hor_hf_diff_gain2;
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_core   = pstApiPara->lum_hor_hf_var_core;
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_gain1  = pstApiPara->lum_hor_hf_var_gain1;
        pstVpssVirReg->VPSS_DB_LHHF.bits.lum_hor_hf_var_gain2  = pstApiPara->lum_hor_hf_var_gain2;

        //VPSS_DB_LVHF
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_core   = pstApiPara->lum_ver_hf_var_core;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_hor_bord_adj_gain = pstApiPara->lum_hor_bord_adj_gain;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_ver_bord_adj_gain = pstApiPara->lum_ver_bord_adj_gain;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_gain1  = pstApiPara->lum_ver_hf_var_gain1;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_ver_hf_var_gain2  = pstApiPara->lum_ver_hf_var_gain2;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_hor_adj_gain      = pstApiPara->lum_hor_adj_gain;
        pstVpssVirReg->VPSS_DB_LVHF.bits.lum_ver_adj_gain      = pstApiPara->lum_ver_adj_gain;

        //VPSS_DB_CTRS
        pstVpssVirReg->VPSS_DB_CTRS.bits.max_lum_hor_db_dist = pstApiPara->max_lum_hor_db_dist;
        pstVpssVirReg->VPSS_DB_CTRS.bits.max_chr_hor_db_dist = pstApiPara->max_chr_hor_db_dist;
        */

        //add by z214841 2015.7.21
        pstVpssVirReg->VPSS_DB_CTRL.bits.dbm_protocol               = pstDbRegs->DBprotocol;

        pstVpssVirReg->VPSS_DB_THR0.bits.db_ctrst_thresh            = pstDbRegs->u8DBCtrstThresh;
        pstVpssVirReg->VPSS_DB_THR0.bits.db_lum_h_border_str_th     = pstDbRegs->DBLumHBorderStrTh;//0x3F;
        pstVpssVirReg->VPSS_DB_THR0.bits.db_lum_v_border_str_th     = pstDbRegs->DBLumVBorderStrTh;//0x3F;
        pstVpssVirReg->VPSS_DB_THR0.bits.db_dir_smooth_mode         = pstDbRegs->u16DBDirSmoothMode;

        pstVpssVirReg->VPSS_DB_THR1.bits.db_detect_ratio_h          = pstDbRegs->DBDetectRatioH;
        pstVpssVirReg->VPSS_DB_THR1.bits.db_detect_ratio_v          = pstDbRegs->DBDetectRatioV;
        pstVpssVirReg->VPSS_DB_THR1.bits.db_text_str_gain           = pstDbRegs->DBTextStrGain;

        pstVpssVirReg->VPSS_DB_THR2.bits.db_global_db_strength_lum  = pstDbRegs->u8GlobalDbStrengthLum;

        pstVpssVirReg->VPSS_DB_THR3.bits.db_lum_hor_var_core        = pstDbRegs->u16LumHorVarCore;
        pstVpssVirReg->VPSS_DB_THR3.bits.db_lum_hor_gain1           = pstDbRegs->u16LumHorGain1;
        pstVpssVirReg->VPSS_DB_THR3.bits.db_lum_hor_gain2           = pstDbRegs->u16LumHorGain2;

        pstVpssVirReg->VPSS_DB_INFO.bits.db_blk_width               = pstDbRegs->DBvdhinfostridew - 1;
        pstVpssVirReg->VPSS_DB_INFO.bits.db_blk_height              = pstDbRegs->DBvdhinfostrideh - 1;

        pstVpssVirReg->VPSS_DB_OST0.bits.db_stt_offset_x            = pstDbRegs->TLXOffset;
        pstVpssVirReg->VPSS_DB_OST0.bits.db_stt_offset_y            = pstDbRegs->TLYOffset;
        pstVpssVirReg->VPSS_DB_OST1.bits.db_end_offset_x            = pstDbRegs->BRXOffset;
        pstVpssVirReg->VPSS_DB_OST1.bits.db_end_offset_y            = pstDbRegs->BRYOffset;

        pstVpssVirReg->VPSS_DB_CTRL1.bits.db_blk_pos_mode           = pstDbRegs->bBlkPosMode;
        pstVpssVirReg->VPSS_DB_CTRL1.bits.db_vdh_trans_info_en      = pstDbRegs->bBlkPosMode;

        pstVpssVirReg->VPSS_DB_CTRL1.bits.db_streth_en              = pstTransVDHInf->db_strethEn;
        pstVpssVirReg->VPSS_DB_CTRL1.bits.db_blk_mode               = !(pstTransVDHInf->db_strethEn);
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.db_lum_ver_en          = pstDbRegs->u8DBLumVerTrue;//modify by z214841 2015.7.21
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.db_lum_hor_en          = pstDbRegs->u8DBLumHorTrue;//modify by z214841 2015.7.21
        g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_det_en             = pstApiPara->det_size_en;

        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_hy_en              = pstApiPara->det_hy_en;
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_hc_en              = pstApiPara->det_hc_en;
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_vy_en              = pstApiPara->det_vy_en;

        g_pstIPSelVdpAlg->V0_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num = pstApiPara->hy_tst_blk_num;
        g_pstIPSelVdpAlg->V0_DBD_TSTBLKNUM.bits.dbd_hc_tst_blk_num = pstApiPara->hc_tst_blk_num;
        g_pstIPSelVdpAlg->V0_DBD_TSTBLKNUM.bits.dbd_vy_tst_blk_num = pstApiPara->vy_tst_blk_num;

        g_pstIPSelVdpAlg->V0_DBD_BLKSIZE.bits.dbd_min_blk_size    = pstApiPara->min_blk_size;
        g_pstIPSelVdpAlg->V0_DBD_BLKSIZE.bits.dbd_hy_max_blk_size = pstApiPara->hy_max_blk_size;
        g_pstIPSelVdpAlg->V0_DBD_BLKSIZE.bits.dbd_hc_max_blk_size = pstApiPara->hc_max_blk_size;
        g_pstIPSelVdpAlg->V0_DBD_BLKSIZE.bits.dbd_vy_max_blk_size = pstApiPara->vy_max_blk_size;

        for (i = 0; i < 128; ++i)
        {
            g_pstIPSelVdpAlg->V0_DB_BORD_FLAG[i].u32 = pstApiPara->vpss_db_bord_flag[i];
        }

        g_pstIPSelVdpAlg->V0_DB_THR0.bits.db_grad_sub_ratio = pstDbRegs->u8DBGradSubRatio;

        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.dbm_protocol               = pstDbRegs->DBprotocol;

        g_pstIPSelVdpAlg->V0_DB_THR0.bits.db_ctrst_thresh            = pstDbRegs->u8DBCtrstThresh;
        g_pstIPSelVdpAlg->V0_DB_THR0.bits.db_lum_h_border_str_th     = pstDbRegs->DBLumHBorderStrTh;//0x3F;
        g_pstIPSelVdpAlg->V0_DB_THR0.bits.db_lum_v_border_str_th     = pstDbRegs->DBLumVBorderStrTh;//0x3F;
        g_pstIPSelVdpAlg->V0_DB_THR0.bits.db_dir_smooth_mode         = pstDbRegs->u16DBDirSmoothMode;

        g_pstIPSelVdpAlg->V0_DB_THR1.bits.db_detect_ratio_h          = pstDbRegs->DBDetectRatioH;
        g_pstIPSelVdpAlg->V0_DB_THR1.bits.db_detect_ratio_v          = pstDbRegs->DBDetectRatioV;
        g_pstIPSelVdpAlg->V0_DB_THR1.bits.db_text_str_gain           = pstDbRegs->DBTextStrGain;

        g_pstIPSelVdpAlg->V0_DB_THR2.bits.db_global_db_strength_lum  = pstDbRegs->u8GlobalDbStrengthLum;

        g_pstIPSelVdpAlg->V0_DB_THR3.bits.db_lum_hor_var_core        = pstDbRegs->u16LumHorVarCore;
        g_pstIPSelVdpAlg->V0_DB_THR3.bits.db_lum_hor_gain1           = pstDbRegs->u16LumHorGain1;
        g_pstIPSelVdpAlg->V0_DB_THR3.bits.db_lum_hor_gain2           = pstDbRegs->u16LumHorGain2;

        g_pstIPSelVdpAlg->V0_DB_INFO.bits.db_blk_width               = pstDbRegs->DBvdhinfostridew - 1;
        g_pstIPSelVdpAlg->V0_DB_INFO.bits.db_blk_height              = pstDbRegs->DBvdhinfostrideh - 1;

        g_pstIPSelVdpAlg->V0_DB_OST0.bits.db_stt_offset_x            = pstDbRegs->TLXOffset;
        g_pstIPSelVdpAlg->V0_DB_OST0.bits.db_stt_offset_y            = pstDbRegs->TLYOffset;
        g_pstIPSelVdpAlg->V0_DB_OST1.bits.db_end_offset_x            = pstDbRegs->BRXOffset;
        g_pstIPSelVdpAlg->V0_DB_OST1.bits.db_end_offset_y            = pstDbRegs->BRYOffset;

        g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_blk_pos_mode           = pstDbRegs->bBlkPosMode;
        g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_vdh_trans_info_en      = pstDbRegs->bBlkPosMode;

        g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_streth_en              = pstTransVDHInf->db_strethEn;
        g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_blk_mode               = !(pstTransVDHInf->db_strethEn);
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut1(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT34.bits.db_lum_h_str_fade_lut1_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT34.bits.db_lum_h_str_fade_lut1_p1 = u16SrcLut[1];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT34.bits.db_lum_h_str_fade_lut1_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT34.bits.db_lum_h_str_fade_lut1_p1 = u16SrcLut[1];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut2(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT35.bits.db_lum_h_str_fade_lut2_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT35.bits.db_lum_h_str_fade_lut2_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT35.bits.db_lum_h_str_fade_lut2_p2 = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT35.bits.db_lum_h_str_fade_lut2_p3 = u16SrcLut[3];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p2 = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT35.bits.db_lum_h_str_fade_lut2_p3 = u16SrcLut[3];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut3(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p2 = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p3 = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p4 = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT36.bits.db_lum_h_str_fade_lut3_p5 = u16SrcLut[5];

        pstVpssVirReg->VPSS_DB_LUT37.bits.db_lum_h_str_fade_lut3_p6 = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT37.bits.db_lum_h_str_fade_lut3_p7 = u16SrcLut[7];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p2 = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p3 = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p4 = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT36.bits.db_lum_h_str_fade_lut3_p5 = u16SrcLut[5];

        g_pstIPSelVdpAlg->V0_DB_LUT37.bits.db_lum_h_str_fade_lut3_p6 = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT37.bits.db_lum_h_str_fade_lut3_p7 = u16SrcLut[7];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Lum_Hor_Fade_Lut4(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p3  = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p4  = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT38.bits.db_lum_h_str_fade_lut4_p5  = u16SrcLut[5];

        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p6  = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p7  = u16SrcLut[7];
        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p8  = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p9  = u16SrcLut[9];
        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p10 = u16SrcLut[10];
        pstVpssVirReg->VPSS_DB_LUT39.bits.db_lum_h_str_fade_lut4_p11 = u16SrcLut[11];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT38.bits.db_lum_h_str_fade_lut4_p5  = u16SrcLut[5];

        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p7  = u16SrcLut[7];
        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p8  = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p9  = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p10 = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DB_LUT39.bits.db_lum_h_str_fade_lut4_p11 = u16SrcLut[11];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Lum_V_Str_Fade_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[2])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT40.bits.db_lum_v_str_fade_lut_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT40.bits.db_lum_v_str_fade_lut_p1 = u16SrcLut[1];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT40.bits.db_lum_v_str_fade_lut_p1 = u16SrcLut[1];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Sobel_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[26])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p3  = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p4  = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT44.bits.db_sobel_str_lut_p5  = u16SrcLut[5];

        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p6  = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p7  = u16SrcLut[7];
        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p8  = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p9  = u16SrcLut[9];
        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p10 = u16SrcLut[10];
        pstVpssVirReg->VPSS_DB_LUT45.bits.db_sobel_str_lut_p11 = u16SrcLut[11];

        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p12 = u16SrcLut[12];
        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p13 = u16SrcLut[13];
        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p14 = u16SrcLut[14];
        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p15 = u16SrcLut[15];
        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p16 = u16SrcLut[16];
        pstVpssVirReg->VPSS_DB_LUT46.bits.db_sobel_str_lut_p17 = u16SrcLut[17];

        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p18 = u16SrcLut[18];
        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p19 = u16SrcLut[19];
        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p20 = u16SrcLut[20];
        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p21 = u16SrcLut[21];
        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p22 = u16SrcLut[22];
        pstVpssVirReg->VPSS_DB_LUT47.bits.db_sobel_str_lut_p23 = u16SrcLut[23];

        pstVpssVirReg->VPSS_DB_LUT48.bits.db_sobel_str_lut_p24 = u16SrcLut[24];
        pstVpssVirReg->VPSS_DB_LUT48.bits.db_sobel_str_lut_p25 = u16SrcLut[25];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT44.bits.db_sobel_str_lut_p5  = u16SrcLut[5];

        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p7  = u16SrcLut[7];
        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p8  = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p9  = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p10 = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DB_LUT45.bits.db_sobel_str_lut_p11 = u16SrcLut[11];

        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p12 = u16SrcLut[12];
        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p13 = u16SrcLut[13];
        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p14 = u16SrcLut[14];
        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p15 = u16SrcLut[15];
        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p16 = u16SrcLut[16];
        g_pstIPSelVdpAlg->V0_DB_LUT46.bits.db_sobel_str_lut_p17 = u16SrcLut[17];

        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p18 = u16SrcLut[18];
        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p19 = u16SrcLut[19];
        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p20 = u16SrcLut[20];
        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p21 = u16SrcLut[21];
        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p22 = u16SrcLut[22];
        g_pstIPSelVdpAlg->V0_DB_LUT47.bits.db_sobel_str_lut_p23 = u16SrcLut[23];

        g_pstIPSelVdpAlg->V0_DB_LUT48.bits.db_sobel_str_lut_p24 = u16SrcLut[24];
        g_pstIPSelVdpAlg->V0_DB_LUT48.bits.db_sobel_str_lut_p25 = u16SrcLut[25];
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetReg_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p3  = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p4  = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT49.bits.db_dir_str_lut_p5  = u16SrcLut[5];

        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p6  = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p7  = u16SrcLut[7];
        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p8  = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p9  = u16SrcLut[9];
        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p10 = u16SrcLut[10];
        pstVpssVirReg->VPSS_DB_LUT50.bits.db_dir_str_lut_p11 = u16SrcLut[11];

        pstVpssVirReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p12 = u16SrcLut[12];
        pstVpssVirReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p13 = u16SrcLut[13];
        pstVpssVirReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p14 = u16SrcLut[14];
        pstVpssVirReg->VPSS_DB_LUT51.bits.db_dir_str_lut_p15 = u16SrcLut[15];

    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT49.bits.db_dir_str_lut_p5  = u16SrcLut[5];

        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p7  = u16SrcLut[7];
        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p8  = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p9  = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p10 = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DB_LUT50.bits.db_dir_str_lut_p11 = u16SrcLut[11];

        g_pstIPSelVdpAlg->V0_DB_LUT51.bits.db_dir_str_lut_p12 = u16SrcLut[12];
        g_pstIPSelVdpAlg->V0_DB_LUT51.bits.db_dir_str_lut_p13 = u16SrcLut[13];
        g_pstIPSelVdpAlg->V0_DB_LUT51.bits.db_dir_str_lut_p14 = u16SrcLut[14];
        g_pstIPSelVdpAlg->V0_DB_LUT51.bits.db_dir_str_lut_p15 = u16SrcLut[15];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[10])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p2 = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p3 = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p4 = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT52.bits.db_dir_str_gain_lut_p5 = u16SrcLut[5];

        pstVpssVirReg->VPSS_DB_LUT53.bits.db_dir_str_gain_lut_p6 = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT53.bits.db_dir_str_gain_lut_p7 = u16SrcLut[7];
        pstVpssVirReg->VPSS_DB_LUT53.bits.db_dir_str_gain_lut_p8 = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT53.bits.db_dir_str_gain_lut_p9 = u16SrcLut[9];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p2 = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p3 = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p4 = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT52.bits.db_dir_str_gain_lut_p5 = u16SrcLut[5];

        g_pstIPSelVdpAlg->V0_DB_LUT53.bits.db_dir_str_gain_lut_p6 = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT53.bits.db_dir_str_gain_lut_p7 = u16SrcLut[7];
        g_pstIPSelVdpAlg->V0_DB_LUT53.bits.db_dir_str_gain_lut_p8 = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT53.bits.db_dir_str_gain_lut_p9 = u16SrcLut[9];
    }

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetReg_Diff_Map_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT54.bits.db_diff_map_lut_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT54.bits.db_diff_map_lut_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT54.bits.db_diff_map_lut_p2 = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT54.bits.db_diff_map_lut_p3 = u16SrcLut[3];

        pstVpssVirReg->VPSS_DB_LUT55.bits.db_diff_map_lut_p4 = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT55.bits.db_diff_map_lut_p5 = u16SrcLut[5];
        pstVpssVirReg->VPSS_DB_LUT55.bits.db_diff_map_lut_p6 = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT55.bits.db_diff_map_lut_p7 = u16SrcLut[7];

    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT54.bits.db_diff_map_lut_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT54.bits.db_diff_map_lut_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT54.bits.db_diff_map_lut_p2 = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT54.bits.db_diff_map_lut_p3 = u16SrcLut[3];

        g_pstIPSelVdpAlg->V0_DB_LUT55.bits.db_diff_map_lut_p4 = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT55.bits.db_diff_map_lut_p5 = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DB_LUT55.bits.db_diff_map_lut_p6 = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT55.bits.db_diff_map_lut_p7 = u16SrcLut[7];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Diff_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[7])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT59.bits.db_diff_str_lut_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT59.bits.db_diff_str_lut_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT59.bits.db_diff_str_lut_p2 = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT59.bits.db_diff_str_lut_p3 = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT59.bits.db_diff_str_lut_p4 = u16SrcLut[4];

        pstVpssVirReg->VPSS_DB_LUT60.bits.db_diff_str_lut_p5 = u16SrcLut[5];
        pstVpssVirReg->VPSS_DB_LUT60.bits.db_diff_str_lut_p6 = u16SrcLut[6];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT59.bits.db_diff_str_lut_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT59.bits.db_diff_str_lut_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT59.bits.db_diff_str_lut_p2 = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT59.bits.db_diff_str_lut_p3 = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT59.bits.db_diff_str_lut_p4 = u16SrcLut[4];

        g_pstIPSelVdpAlg->V0_DB_LUT60.bits.db_diff_str_lut_p5 = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DB_LUT60.bits.db_diff_str_lut_p6 = u16SrcLut[6];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Diff_Weight_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[6])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT56.bits.db_diff_weight_lut_p0 = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT56.bits.db_diff_weight_lut_p1 = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT56.bits.db_diff_weight_lut_p2 = u16SrcLut[2];

        pstVpssVirReg->VPSS_DB_LUT57.bits.db_diff_weight_lut_p3 = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT57.bits.db_diff_weight_lut_p4 = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT57.bits.db_diff_weight_lut_p5 = u16SrcLut[5];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT56.bits.db_diff_weight_lut_p0 = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT56.bits.db_diff_weight_lut_p1 = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT56.bits.db_diff_weight_lut_p2 = u16SrcLut[2];

        g_pstIPSelVdpAlg->V0_DB_LUT57.bits.db_diff_weight_lut_p3 = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT57.bits.db_diff_weight_lut_p4 = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT57.bits.db_diff_weight_lut_p5 = u16SrcLut[5];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Hor_Delta_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p3  = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p4  = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p5  = u16SrcLut[5];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p6  = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT61.bits.db_lum_hor_delta_lut_p7  = u16SrcLut[7];

        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p8  = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p9  = u16SrcLut[9];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p10 = u16SrcLut[10];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p11 = u16SrcLut[11];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p12 = u16SrcLut[12];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p13 = u16SrcLut[13];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p14 = u16SrcLut[14];
        pstVpssVirReg->VPSS_DB_LUT62.bits.db_lum_hor_delta_lut_p15 = u16SrcLut[15];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p5  = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT61.bits.db_lum_hor_delta_lut_p7  = u16SrcLut[7];

        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p8  = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p9  = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p10 = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p11 = u16SrcLut[11];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p12 = u16SrcLut[12];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p13 = u16SrcLut[13];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p14 = u16SrcLut[14];
        g_pstIPSelVdpAlg->V0_DB_LUT62.bits.db_lum_hor_delta_lut_p15 = u16SrcLut[15];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Ver_Delta_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p3  = u16SrcLut[3];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p4  = u16SrcLut[4];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p5  = u16SrcLut[5];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p6  = u16SrcLut[6];
        pstVpssVirReg->VPSS_DB_LUT63.bits.db_lum_ver_delta_lut_p7  = u16SrcLut[7];

        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p8  = u16SrcLut[8];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p9  = u16SrcLut[9];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p10 = u16SrcLut[10];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p11 = u16SrcLut[11];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p12 = u16SrcLut[12];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p13 = u16SrcLut[13];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p14 = u16SrcLut[14];
        pstVpssVirReg->VPSS_DB_LUT64.bits.db_lum_ver_delta_lut_p15 = u16SrcLut[15];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p5  = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DB_LUT63.bits.db_lum_ver_delta_lut_p7  = u16SrcLut[7];

        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p8  = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p9  = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p10 = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p11 = u16SrcLut[11];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p12 = u16SrcLut[12];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p13 = u16SrcLut[13];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p14 = u16SrcLut[14];
        g_pstIPSelVdpAlg->V0_DB_LUT64.bits.db_lum_ver_delta_lut_p15 = u16SrcLut[15];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetDBDetEnReg(HI_U32 u32Data, HI_S32* pDet_size_en,
                            HI_S32* pDet_hy_en, HI_S32* pDet_hc_en, HI_S32* pDet_vy_en)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        *pDet_size_en = pstVpssReg->VPSS_DB_CTRL1.bits.db_det_en;
        *pDet_hy_en   = pstVpssReg->VPSS_DB_CTRL.bits.det_hy_en;
        *pDet_hc_en   = pstVpssReg->VPSS_DB_CTRL.bits.det_hc_en;
        *pDet_vy_en   = pstVpssReg->VPSS_DB_CTRL.bits.det_vy_en;
    }
    else
    {
        *pDet_size_en = g_pstIPSelVdpAlg->V0_DB_CTRL1.bits.db_det_en;
        *pDet_hy_en   = g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_hy_en;
        *pDet_hc_en   = g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_hc_en;
        *pDet_vy_en   = g_pstIPSelVdpAlg->V0_DB_CTRL.bits.det_vy_en;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetDbmEnReg(HI_U32 u32HandleNo, HI_BOOL* pbDbEn)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;
    PQ_CHECK_NULL_PTR(pbDbEn);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        *pbDbEn = pstVpssVirReg->VPSS_CTRL.bits.dbm_en;
    }
    else
    {
        *pbDbEn = g_pstIPSelVdpAlg->V0_PRERD.bits.dbm_en;
    }

    return HI_SUCCESS;
}

/* Set DB Det Size */
HI_S32 PQ_HAL_SetDBDetSizeEn(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_CTRL.bits.blk_det_en = bOnOff;
    }
    else
    {
        //g_pstIPSelVdpAlg->V0.bits.blk_det_en = bOnOff;
    }

    return HI_SUCCESS;
}

/* Set DB Enable */
HI_S32 PQ_HAL_EnableDB(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_DB_CTRL.bits.db_en = bOnOff;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.db_en = bOnOff;
    }

    return HI_SUCCESS;
}

/* Set DB Demo */
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);
        pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_en = bOnOff;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.dbm_demo_en = bOnOff;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32Data, DBM_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);
        pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_mode = enMode;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.dbm_demo_mode = enMode;
    }

    return HI_SUCCESS;
}

/* Set DB Pos */
HI_S32 PQ_HAL_DBDemoPos(HI_U32 u32HandleNo, HI_U32 u32DBDemoPos)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);
        pstVpssReg->VPSS_DB_DEMO.bits.db_demo_pos_x = u32DBDemoPos;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_DEMO.bits.db_demo_pos_x = u32DBDemoPos;
    }

    return HI_SUCCESS;
}


