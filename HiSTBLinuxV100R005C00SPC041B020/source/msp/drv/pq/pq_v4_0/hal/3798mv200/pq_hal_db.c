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


HI_S32 PQ_HAL_SetReg_lum_h_str_fade_lut(HI_U32 u32HandleNo, const HI_U8 u8SrcLut[12])
{
    VPSS_REG_S *pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p0  = u8SrcLut[0 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p1  = u8SrcLut[1 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p2  = u8SrcLut[2 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p3  = u8SrcLut[3 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p4  = u8SrcLut[4 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p5  = u8SrcLut[5 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p6  = u8SrcLut[6 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p7  = u8SrcLut[7 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p8  = u8SrcLut[8 ] ;
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p9  = u8SrcLut[9 ] ;
    pstVpssVirReg->VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p10 = u8SrcLut[10] ;
    pstVpssVirReg->VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p11 = u8SrcLut[11] ;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetReg_dir_str_lut(HI_U32 u32HandleNo, const HI_U8 u8SrcLut[16])
{
    VPSS_REG_S *pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p0  = u8SrcLut[0] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p1  = u8SrcLut[1] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p2  = u8SrcLut[2] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p3  = u8SrcLut[3] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p4  = u8SrcLut[4] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p5  = u8SrcLut[5] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p6  = u8SrcLut[6] ;
    pstVpssVirReg->VPSS_DB_LUT5.bits.db_dir_str_lut_p7  = u8SrcLut[7] ;
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p8  = u8SrcLut[8] ;
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p9  = u8SrcLut[9] ;
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p10 = u8SrcLut[10];
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p11 = u8SrcLut[11];
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p12 = u8SrcLut[12];
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p13 = u8SrcLut[13];
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p14 = u8SrcLut[14];
    pstVpssVirReg->VPSS_DB_LUT6.bits.db_dir_str_lut_p15 = u8SrcLut[15];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_dir_str_gain_lut(HI_U32 u32HandleNo, const HI_U8 u8SrcLut[8])
{
    VPSS_REG_S *pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p0 = u8SrcLut[0];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p1 = u8SrcLut[1];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p2 = u8SrcLut[2];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p3 = u8SrcLut[3];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p4 = u8SrcLut[4];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p5 = u8SrcLut[5];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p6 = u8SrcLut[6];
    pstVpssVirReg->VPSS_DB_LUT4.bits.db_dir_str_gain_lut_p7 = u8SrcLut[7];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDBBlkNumberCfg(HI_U32 u32HandleNo, DB_PARAM_S *pstDBPara)
{
    VPSS_REG_S *pstVpssReg = HI_NULL;

    PQ_CHECK_NULL_PTR(pstDBPara);
    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_DB_CTRL.bits.dbm_out_mode              = pstDBPara->u16DbmOutMode;

    pstVpssReg->VPSS_CTRL.bits.blk_det_en                   = pstDBPara->DetSizeEn;
    pstVpssReg->VPSS_DB_CTRL.bits.det_hy_en                 = pstDBPara->hyEn;
    pstVpssReg->VPSS_DBD_THDEDGE.bits.dbd_hy_thd_edge       = pstDBPara->Hythreshold_edge;
    pstVpssReg->VPSS_DBD_THDTXT.bits.dbd_hy_thd_txt         = pstDBPara->Hythreshold_texture;
    pstVpssReg->VPSS_DB_RATIO.bits.db_global_db_strenth_lum = pstDBPara->u8GlobalDbStrengthLum;

    pstVpssReg->VPSS_DBD_RTNBDTXT.bits.dbd_hy_rtn_bd_txt    = pstDBPara->HyRationBorderText;
    pstVpssReg->VPSS_DBD_TSTBLKNUM.bits.dbd_hy_tst_blk_num  = pstDBPara->HyTestBlkNumber;

    pstVpssReg->VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut0 = pstDBPara->HyRadix8BinLut0;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT0.bits.dbd_hy_rdx8bin_lut1 = pstDBPara->HyRadix8BinLut1;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut2 = pstDBPara->HyRadix8BinLut2;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT1.bits.dbd_hy_rdx8bin_lut3 = pstDBPara->HyRadix8BinLut3;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut4 = pstDBPara->HyRadix8BinLut4;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT2.bits.dbd_hy_rdx8bin_lut5 = pstDBPara->HyRadix8BinLut5;
    pstVpssReg->VPSS_DBD_HYRDX8BINLUT3.bits.dbd_hy_rdx8bin_lut6 = pstDBPara->HyRadix8BinLut6;

    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_min_blk_size          = pstDBPara->MinBlkSize;
    pstVpssReg->VPSS_DBD_BLKSIZE.bits.dbd_hy_max_blk_size       = pstDBPara->HyMaxBlkSize;
    pstVpssReg->VPSS_DBD_FLAT.bits.dbd_thr_flat                 = pstDBPara->Thr_flat;

    return HI_SUCCESS;
}


/* 设置DB软算法api计算出来的参数值 */
HI_S32 PQ_HAL_SetDBApiReg(HI_U32 u32HandleNo, DB_API_REG_S *pstApiPara)
{
    VPSS_REG_S *pstVpssVirReg = HI_NULL;
    DB_REG_S   *pstDbRegs;

    PQ_CHECK_NULL_PTR(pstApiPara);
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstDbRegs = &(pstApiPara->stDbRegs);
    PQ_CHECK_NULL_PTR(pstDbRegs);

    PQ_CHECK_NULL_PTR(pstDbRegs->u8DirStrGainLut);
    PQ_CHECK_NULL_PTR(pstDbRegs->u8DirStrGainLut);

    PQ_HAL_SetReg_dir_str_gain_lut(u32HandleNo, pstDbRegs->u8DirStrGainLut);
    PQ_HAL_SetReg_dir_str_lut(     u32HandleNo, pstDbRegs->u8DirStrLut);

    //pstVpssVirReg->VPSS_DB_CTRL.bits.db_en                       = pstApiPara->stDbRegs.u8DbEn;
    pstVpssVirReg->VPSS_DB_CTRL.bits.db_lum_hor_en               = pstApiPara->stDbRegs.u8DBLumHorEn;
    pstVpssVirReg->VPSS_DB_DIR.bits.db_grad_sub_ratio            = pstApiPara->stDbRegs.u8GradSubRatio;
    pstVpssVirReg->VPSS_DB_DIR.bits.db_ctrst_thresh              = pstApiPara->stDbRegs.u8CtrstThresh;
    pstVpssVirReg->VPSS_DB_RATIO.bits.db_lum_hor_scale_ratio     = pstApiPara->stDbRegs.u8LumHorScaleRatio;
    pstVpssVirReg->VPSS_DB_RATIO.bits.db_lum_hor_filter_sel      = pstApiPara->stDbRegs.u8LumHorFilterSel;
    pstVpssVirReg->VPSS_DB_CTRL.bits.db_lum_hor_en               = pstApiPara->stDbRegs.u8DBLumTrue;
    pstVpssVirReg->VPSS_DB_CTRL.bits.db_lum_hor_en               = pstApiPara->stDbRegs.u8DBLumHorTrue;
    //pstVpssVirReg->VPSS_DB_RATIO.bits.db_global_db_strenth_lum = pstApiPara->stDbRegs.u8GlobalDbStrengthLum;
    pstVpssVirReg->VPSS_DB_RATIO.bits.db_lum_hor_txt_win_size    = pstApiPara->stDbRegs.u8LumHorTxtWinSize;

    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_core     = pstApiPara->stDbRegs.u16LumHorHFDiffCore;
    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_gain1    = pstApiPara->stDbRegs.u16LumHorHFDiffGain1;
    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_diff_gain2    = pstApiPara->stDbRegs.u16LumHorHFDiffGain2;

    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_var_core      = pstApiPara->stDbRegs.u16LumHorHFVarCore;
    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_var_gain1     = pstApiPara->stDbRegs.u16LumHorHFVarGain1;
    pstVpssVirReg->VPSS_DB_LHHF.bits.db_lum_hor_hf_var_gain2     = pstApiPara->stDbRegs.u16LumHorHFVarGain2;

    pstVpssVirReg->VPSS_DB_LVHF.bits.db_lum_hor_bord_adj_gain    = pstApiPara->stDbRegs.u16LumHorBordAdjGain;
    pstVpssVirReg->VPSS_DB_CTRS.bits.db_ctrst_adj_core           = pstApiPara->stDbRegs.u16CtrstAdjCore;
    pstVpssVirReg->VPSS_DB_CTRS.bits.db_ctrst_adj_gain1          = pstApiPara->stDbRegs.u16CtrstAdjGain1;
    pstVpssVirReg->VPSS_DB_CTRS.bits.db_ctrst_adj_gain2          = pstApiPara->stDbRegs.u16CtrstAdjGain2;
    pstVpssVirReg->VPSS_DB_CTRS.bits.db_dir_smooth_mode          = pstApiPara->stDbRegs.u16DirSmoothMode;
    pstVpssVirReg->VPSS_DB_LVHF.bits.db_lum_hor_adj_gain         = pstApiPara->stDbRegs.u16LumHorAdjGain;
    pstVpssVirReg->VPSS_DB_CTRS.bits.db_max_lum_hor_db_dist      = pstApiPara->stDbRegs.u8MaxLumHorDbDist;

    pstVpssVirReg->VPSS_DB_BLK.bits.db_lum_h_blk_size            = pstApiPara->lum_h_blk_size;

    if (pstApiPara->stDbRegs.pLumHorDeltaLut != HI_NULL)
    {
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p0  = pstApiPara->stDbRegs.pLumHorDeltaLut[0 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p1  = pstApiPara->stDbRegs.pLumHorDeltaLut[1 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p2  = pstApiPara->stDbRegs.pLumHorDeltaLut[2 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p3  = pstApiPara->stDbRegs.pLumHorDeltaLut[3 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p4  = pstApiPara->stDbRegs.pLumHorDeltaLut[4 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p5  = pstApiPara->stDbRegs.pLumHorDeltaLut[5 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p6  = pstApiPara->stDbRegs.pLumHorDeltaLut[6 ];
        pstVpssVirReg->VPSS_DB_LUT0.bits.db_lum_hor_delta_lut_p7  = pstApiPara->stDbRegs.pLumHorDeltaLut[7 ];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p8  = pstApiPara->stDbRegs.pLumHorDeltaLut[8 ];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p9  = pstApiPara->stDbRegs.pLumHorDeltaLut[9 ];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p10 = pstApiPara->stDbRegs.pLumHorDeltaLut[10];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p11 = pstApiPara->stDbRegs.pLumHorDeltaLut[11];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p12 = pstApiPara->stDbRegs.pLumHorDeltaLut[12];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p13 = pstApiPara->stDbRegs.pLumHorDeltaLut[13];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p14 = pstApiPara->stDbRegs.pLumHorDeltaLut[14];
        pstVpssVirReg->VPSS_DB_LUT1.bits.db_lum_hor_delta_lut_p15 = pstApiPara->stDbRegs.pLumHorDeltaLut[15];
    }

    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p0  = pstApiPara->stDbRegs.pLumHStrFadeLut[0 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p1  = pstApiPara->stDbRegs.pLumHStrFadeLut[1 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p2  = pstApiPara->stDbRegs.pLumHStrFadeLut[2 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p3  = pstApiPara->stDbRegs.pLumHStrFadeLut[3 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p4  = pstApiPara->stDbRegs.pLumHStrFadeLut[4 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p5  = pstApiPara->stDbRegs.pLumHStrFadeLut[5 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p6  = pstApiPara->stDbRegs.pLumHStrFadeLut[6 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p7  = pstApiPara->stDbRegs.pLumHStrFadeLut[7 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p8  = pstApiPara->stDbRegs.pLumHStrFadeLut[8 ];
    pstVpssVirReg->VPSS_DB_LUT2.bits.db_lum_h_str_fade_lut_p9  = pstApiPara->stDbRegs.pLumHStrFadeLut[9 ];
    pstVpssVirReg->VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p10 = pstApiPara->stDbRegs.pLumHStrFadeLut[10];
    pstVpssVirReg->VPSS_DB_LUT3.bits.db_lum_h_str_fade_lut_p11 = pstApiPara->stDbRegs.pLumHStrFadeLut[11];

    //VPSS_DB_BORD_FLAG
    pstVpssVirReg->VPSS_DB_BORD_FLAG0.bits.vpss_db_bord_flag_0   = pstApiPara->vpss_db_bord_flag[0];
    pstVpssVirReg->VPSS_DB_BORD_FLAG1.bits.vpss_db_bord_flag_1   = pstApiPara->vpss_db_bord_flag[1];
    pstVpssVirReg->VPSS_DB_BORD_FLAG2.bits.vpss_db_bord_flag_2   = pstApiPara->vpss_db_bord_flag[2];
    pstVpssVirReg->VPSS_DB_BORD_FLAG3.bits.vpss_db_bord_flag_3   = pstApiPara->vpss_db_bord_flag[3];
    pstVpssVirReg->VPSS_DB_BORD_FLAG4.bits.vpss_db_bord_flag_4   = pstApiPara->vpss_db_bord_flag[4];
    pstVpssVirReg->VPSS_DB_BORD_FLAG5.bits.vpss_db_bord_flag_5   = pstApiPara->vpss_db_bord_flag[5];
    pstVpssVirReg->VPSS_DB_BORD_FLAG6.bits.vpss_db_bord_flag_6   = pstApiPara->vpss_db_bord_flag[6];
    pstVpssVirReg->VPSS_DB_BORD_FLAG7.bits.vpss_db_bord_flag_7   = pstApiPara->vpss_db_bord_flag[7];
    pstVpssVirReg->VPSS_DB_BORD_FLAG8.bits.vpss_db_bord_flag_8   = pstApiPara->vpss_db_bord_flag[8];
    pstVpssVirReg->VPSS_DB_BORD_FLAG9.bits.vpss_db_bord_flag_9   = pstApiPara->vpss_db_bord_flag[9];
    pstVpssVirReg->VPSS_DB_BORD_FLAG10.bits.vpss_db_bord_flag_10 = pstApiPara->vpss_db_bord_flag[10];
    pstVpssVirReg->VPSS_DB_BORD_FLAG11.bits.vpss_db_bord_flag_11 = pstApiPara->vpss_db_bord_flag[11];
    pstVpssVirReg->VPSS_DB_BORD_FLAG12.bits.vpss_db_bord_flag_12 = pstApiPara->vpss_db_bord_flag[12];
    pstVpssVirReg->VPSS_DB_BORD_FLAG13.bits.vpss_db_bord_flag_13 = pstApiPara->vpss_db_bord_flag[13];
    pstVpssVirReg->VPSS_DB_BORD_FLAG14.bits.vpss_db_bord_flag_14 = pstApiPara->vpss_db_bord_flag[14];
    pstVpssVirReg->VPSS_DB_BORD_FLAG15.bits.vpss_db_bord_flag_15 = pstApiPara->vpss_db_bord_flag[15];
    pstVpssVirReg->VPSS_DB_BORD_FLAG16.bits.vpss_db_bord_flag_16 = pstApiPara->vpss_db_bord_flag[16];
    pstVpssVirReg->VPSS_DB_BORD_FLAG17.bits.vpss_db_bord_flag_17 = pstApiPara->vpss_db_bord_flag[17];
    pstVpssVirReg->VPSS_DB_BORD_FLAG18.bits.vpss_db_bord_flag_18 = pstApiPara->vpss_db_bord_flag[18];
    pstVpssVirReg->VPSS_DB_BORD_FLAG19.bits.vpss_db_bord_flag_19 = pstApiPara->vpss_db_bord_flag[19];
    pstVpssVirReg->VPSS_DB_BORD_FLAG20.bits.vpss_db_bord_flag_20 = pstApiPara->vpss_db_bord_flag[20];
    pstVpssVirReg->VPSS_DB_BORD_FLAG21.bits.vpss_db_bord_flag_21 = pstApiPara->vpss_db_bord_flag[21];
    pstVpssVirReg->VPSS_DB_BORD_FLAG22.bits.vpss_db_bord_flag_22 = pstApiPara->vpss_db_bord_flag[22];
    pstVpssVirReg->VPSS_DB_BORD_FLAG23.bits.vpss_db_bord_flag_23 = pstApiPara->vpss_db_bord_flag[23];
    pstVpssVirReg->VPSS_DB_BORD_FLAG24.bits.vpss_db_bord_flag_24 = pstApiPara->vpss_db_bord_flag[24];
    pstVpssVirReg->VPSS_DB_BORD_FLAG25.bits.vpss_db_bord_flag_25 = pstApiPara->vpss_db_bord_flag[25];
    pstVpssVirReg->VPSS_DB_BORD_FLAG26.bits.vpss_db_bord_flag_26 = pstApiPara->vpss_db_bord_flag[26];
    pstVpssVirReg->VPSS_DB_BORD_FLAG27.bits.vpss_db_bord_flag_27 = pstApiPara->vpss_db_bord_flag[27];
    pstVpssVirReg->VPSS_DB_BORD_FLAG28.bits.vpss_db_bord_flag_28 = pstApiPara->vpss_db_bord_flag[28];
    pstVpssVirReg->VPSS_DB_BORD_FLAG29.bits.vpss_db_bord_flag_29 = pstApiPara->vpss_db_bord_flag[29];
    pstVpssVirReg->VPSS_DB_BORD_FLAG30.bits.vpss_db_bord_flag_30 = pstApiPara->vpss_db_bord_flag[30];
    pstVpssVirReg->VPSS_DB_BORD_FLAG31.bits.vpss_db_bord_flag_31 = pstApiPara->vpss_db_bord_flag[31];

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_GetDBDetEnReg(HI_U32 u32Data, HI_S32 *pDet_size_en,
                            HI_S32 *pDet_hy_en, HI_S32 *pDet_hc_en, HI_S32 *pDet_vy_en)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    *pDet_size_en = pstVpssReg->VPSS_CTRL.bits.blk_det_en;
    *pDet_hy_en   = pstVpssReg->VPSS_DB_CTRL.bits.det_hy_en;
    //*pDet_hc_en = pstVpssReg->VPSS_DB_CTRL.bits.det_hc_en;
    //*pDet_vy_en = pstVpssReg->VPSS_DB_CTRL.bits.det_vy_en;


    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetDbmEnReg(HI_U32 u32HandleNo, HI_BOOL *pbDbEn)
{
    return HI_SUCCESS;
}

/* Set DB Det Size */
HI_S32 PQ_HAL_SetDBDetSizeEn(HI_U32 u32HandleNo, HI_BOOL bOnOff)
{
    VPSS_REG_S *pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL.bits.blk_det_en = bOnOff;

    return HI_SUCCESS;
}

/* Set DB Enable */
HI_S32 PQ_HAL_EnableDB(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_DB_CTRL.bits.db_en = bOnOff;

    return HI_SUCCESS;
}

/* Set DB Demo */
HI_S32 PQ_HAL_EnableDBDemo(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDbmDemoMode(HI_U32 u32Data, DBM_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_mode = enMode;

    return HI_SUCCESS;
}

/* Set DB Pos */
HI_S32 PQ_HAL_DBDemoPos(HI_U32 u32HandleNo, HI_U32 u32DBDemoPos)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);
    pstVpssReg->VPSS_DBM_DEMO.bits.dbm_demo_pos_x = u32DBDemoPos;

    return HI_SUCCESS;
}



