/******************************************************************************

Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     : pq_hal_dm.c
Version       : Initial Draft
Author        :
Created       : 2015/10/09
Description   : De-Mosquito

******************************************************************************/
#include <linux/string.h>   /* memcpy() */
#include "pq_hal_dm.h"
#include "pq_hal_comm.h"
#include "drv_pq_comm.h"

HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p0  = u16SrcLut[0];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p1  = u16SrcLut[1];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p2  = u16SrcLut[2];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p3  = u16SrcLut[3];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p4  = u16SrcLut[4];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p5  = u16SrcLut[5];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p6  = u16SrcLut[6];
    pstVpssVirReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p7  = u16SrcLut[7];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Dm_Sw_Wht_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[5])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p0  = u16SrcLut[0];
    pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p1  = u16SrcLut[1];
    pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p2  = u16SrcLut[2];
    pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p3  = u16SrcLut[3];

    pstVpssVirReg->VPSS_DM_LUT1.bits.dm_sw_wht_lut_p4  = u16SrcLut[4];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p0   = u16SrcLut[0];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p1   = u16SrcLut[1];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p2   = u16SrcLut[2];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p3   = u16SrcLut[3];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p4   = u16SrcLut[4];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p5   = u16SrcLut[5];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p6   = u16SrcLut[6];
    pstVpssVirReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p7   = u16SrcLut[7];

    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p8   = u16SrcLut[8];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p9   = u16SrcLut[9];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p10  = u16SrcLut[10];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p11  = u16SrcLut[11];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p12  = u16SrcLut[12];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p13  = u16SrcLut[13];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p14  = u16SrcLut[14];
    pstVpssVirReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p15  = u16SrcLut[15];


    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDM(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);
    pstVpssReg->VPSS_DB_CTRL.bits.dm_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
    PQ_CHECK_NULL_PTR(pstVpssReg);
    pstVpssReg->VPSS_DB_CTRL.bits.dbm_demo_en = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    PQ_CHECK_NULL_PTR(pstDmApiReg);
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DM_DIR.bits.dm_opp_ang_ctrst_t             =     pstDmApiReg->OppAngCtrstT              ;
    pstVpssVirReg->VPSS_DM_EDGE.bits.dm_init_val_step              =     pstDmApiReg->InitValStep               ;
    pstVpssVirReg->VPSS_DM_EDGE.bits.dm_mmf_set                    =     pstDmApiReg->MMFSet                    ;

    pstVpssVirReg->VPSS_DM_DIRC.bits.dm_mndir_opp_ctrst_t          =     pstDmApiReg->MNDirOppCtrstT            ;
    pstVpssVirReg->VPSS_DM_DIRC.bits.dm_mmf_limit_en               =     pstDmApiReg->MMFlimitEn                ;

    pstVpssVirReg->VPSS_DM_LSW.bits.dm_lw_ctrst_t                  =     pstDmApiReg->LWCtrstT                  ;
    pstVpssVirReg->VPSS_DM_LSW.bits.dm_lw_ctrst_t_10               =     pstDmApiReg->LWCtrstT10bit             ;

    pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_csw_trsnt_lt              =     pstDmApiReg->cSWTrsntLT                ;
    pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_mmf_lr                    =     pstDmApiReg->MMFLR                     ;
    pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_mmf_lr_10                 =     pstDmApiReg->MMFLR10bit                ;

    pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_csw_trsnt_lt_10           =     pstDmApiReg->cSWTrsntLT10bit           ;
    pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_mmf_sr                    =     pstDmApiReg->MMFSR                     ;
    pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_mmf_sr_10                 =     pstDmApiReg->MMFSR10bit                ;

    pstVpssVirReg->VPSS_DM_LSW.bits.dm_csw_trsnt_st                =     pstDmApiReg->cSWTrsntST                ;
    pstVpssVirReg->VPSS_DM_LSW.bits.dm_lsw_ratio                   =     pstDmApiReg->LSWRatio                  ;

    pstVpssVirReg->VPSS_DM_DIRO.bits.dm_csw_trsnt_st_10            =     pstDmApiReg->cSWTrsntST10bit           ;

    pstVpssVirReg->VPSS_DM_THR0.bits.dm_dir_blend_str              =     pstDmApiReg->DirBlendStr               ;
    pstVpssVirReg->VPSS_DM_THR0.bits.dm_limit_lsw_ratio            =     pstDmApiReg->LimitLSWRatio             ;
    pstVpssVirReg->VPSS_DM_THR0.bits.dm_lim_res_blend_str1         =     pstDmApiReg->LimResBlendStr1           ;
    pstVpssVirReg->VPSS_DM_THR0.bits.dm_lim_res_blend_str2         =     pstDmApiReg->LimResBlendStr2           ;

    pstVpssVirReg->VPSS_DM_LUT1.bits.dm_limit_t                    =     pstDmApiReg->LimitT                    ;
    pstVpssVirReg->VPSS_DM_LUT1.bits.dm_limit_t_10                 =     pstDmApiReg->LimitT10bit               ;

    pstVpssVirReg->VPSS_DM_LUT5.bits.dm_trans_band_lut_p0          =     pstDmApiReg->TransBand[0]              ;
    pstVpssVirReg->VPSS_DM_LUT5.bits.dm_trans_band_lut_p1          =     pstDmApiReg->TransBand[1]              ;
    pstVpssVirReg->VPSS_DM_LUT5.bits.dm_trans_band_lut_p2          =     pstDmApiReg->TransBand[2]              ;
    pstVpssVirReg->VPSS_DM_LUT5.bits.dm_trans_band_lut_p3          =     pstDmApiReg->TransBand[3]              ;
    pstVpssVirReg->VPSS_DM_LUT5.bits.dm_trans_band_lut_p4          =     pstDmApiReg->TransBand[4]              ;

    pstVpssVirReg->VPSS_DM_LUT6.bits.dm_trans_band_lut_p5          =     pstDmApiReg->TransBand[5]              ;
    pstVpssVirReg->VPSS_DM_LUT6.bits.dm_trans_band_lut_p6          =     pstDmApiReg->TransBand[6]              ;
    pstVpssVirReg->VPSS_DM_LUT6.bits.dm_trans_band_lut_p7          =     pstDmApiReg->TransBand[7]              ;
    pstVpssVirReg->VPSS_DM_LUT6.bits.dm_trans_band_lut_p8          =     pstDmApiReg->TransBand[8]              ;
    pstVpssVirReg->VPSS_DM_LUT6.bits.dm_trans_band_lut_p9          =     pstDmApiReg->TransBand[9]              ;

    pstVpssVirReg->VPSS_DM_LUT7.bits.dm_trans_band_lut_p10         =     pstDmApiReg->TransBand[10]             ;
    pstVpssVirReg->VPSS_DM_LUT7.bits.dm_trans_band_lut_p11         =     pstDmApiReg->TransBand[11]             ;
    pstVpssVirReg->VPSS_DM_LUT7.bits.dm_trans_band_lut_p12         =     pstDmApiReg->TransBand[12]             ;
    pstVpssVirReg->VPSS_DM_LUT7.bits.dm_trans_band_lut_p13         =     pstDmApiReg->TransBand[13]             ;
    pstVpssVirReg->VPSS_DM_LUT7.bits.dm_trans_band_lut_p14         =     pstDmApiReg->TransBand[14]             ;

    pstVpssVirReg->VPSS_DM_LUT8.bits.dm_trans_band_lut_p15         =     pstDmApiReg->TransBand[15]             ;
    pstVpssVirReg->VPSS_DM_LUT8.bits.dm_trans_band_lut_p16         =     pstDmApiReg->TransBand[16]             ;
    pstVpssVirReg->VPSS_DM_LUT8.bits.dm_trans_band_lut_p17         =     pstDmApiReg->TransBand[17]             ;
    pstVpssVirReg->VPSS_DM_LUT8.bits.dm_trans_band_lut_p18         =     pstDmApiReg->TransBand[18]             ;
    pstVpssVirReg->VPSS_DM_LUT8.bits.dm_trans_band_lut_p19         =     pstDmApiReg->TransBand[19]             ;

    pstVpssVirReg->VPSS_DM_LUT9.bits.dm_trans_band_lut_p20         =     pstDmApiReg->TransBand[20]             ;
    pstVpssVirReg->VPSS_DM_LUT9.bits.dm_trans_band_lut_p21         =     pstDmApiReg->TransBand[21]             ;
    pstVpssVirReg->VPSS_DM_LUT9.bits.dm_trans_band_lut_p22         =     pstDmApiReg->TransBand[22]             ;
    pstVpssVirReg->VPSS_DM_LUT9.bits.dm_trans_band_lut_p23         =     pstDmApiReg->TransBand[23]             ;
    pstVpssVirReg->VPSS_DM_LUT9.bits.dm_trans_band_lut_p24         =     pstDmApiReg->TransBand[24]             ;

    pstVpssVirReg->VPSS_DM_LUT10.bits.dm_trans_band_lut_p25        =     pstDmApiReg->TransBand[25]             ;
    pstVpssVirReg->VPSS_DM_LUT10.bits.dm_trans_band_lut_p26        =     pstDmApiReg->TransBand[26]             ;
    pstVpssVirReg->VPSS_DM_LUT10.bits.dm_trans_band_lut_p27        =     pstDmApiReg->TransBand[27]             ;
    pstVpssVirReg->VPSS_DM_LUT10.bits.dm_trans_band_lut_p28        =     pstDmApiReg->TransBand[28]             ;
    pstVpssVirReg->VPSS_DM_LUT10.bits.dm_trans_band_lut_p29        =     pstDmApiReg->TransBand[29]             ;
    pstVpssVirReg->VPSS_DM_LUT11.bits.dm_trans_band_lut_p30        =     pstDmApiReg->TransBand[30]             ;

    return HI_SUCCESS;
}

