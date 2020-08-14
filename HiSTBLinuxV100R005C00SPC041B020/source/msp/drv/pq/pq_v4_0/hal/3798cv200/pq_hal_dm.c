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


#if 0
static HI_S32 DM_SetParamToReg(VPSS_REG_S* pstVpssReg, DM_PARAM_S* pstDRValue)
{
    PQ_CHECK_NULL_PTR(pstVpssReg);
    PQ_CHECK_NULL_PTR(pstDRValue);
    PQ_CHECK_NULL_PTR(pstDRValue->pu8DmSwWhtLut);
    PQ_CHECK_NULL_PTR(pstDRValue->pu8DmDirStrGainLut);
    PQ_CHECK_NULL_PTR(pstDRValue->pu8DmDirStrLut);

    //VPSS_DB_CTRL---DM_CTRL
    pstVpssReg->VPSS_DB_CTRL.bits.dm_en = pstDRValue->bDM_En;

    //VPSS_DM_DIR
    pstVpssReg->VPSS_DM_DIR.bits.dm_grad_sub_ratio = pstDRValue->u8Dm_grad_sub_ratio;
    pstVpssReg->VPSS_DM_DIR.bits.dm_ctrst_thresh   = pstDRValue->u8Dm_ctrst_thresh;
    //pstVpssReg->VPSS_DM_DIR.bits.dm_str_idx_t      = pstDRValue->u8Dm_str_idx_t;
    //pstVpssReg->VPSS_DM_DIR.bits.dm_limit_t        = pstDRValue->u8Dm_limit_t;

    //VPSS_DM_EDGE
    pstVpssReg->VPSS_DM_EDGE.bits.dm_edge_thr     = pstDRValue->u8Dm_edge_thr;
    pstVpssReg->VPSS_DM_EDGE.bits.dm_asymtrc_gain = pstDRValue->u8Dm_asymtrc_gain;
    pstVpssReg->VPSS_DM_EDGE.bits.dm_edge_gain1   = pstDRValue->u8Dm_edge_gain1;
    pstVpssReg->VPSS_DM_EDGE.bits.dm_edge_gain2   = pstDRValue->u8Dm_edge_gain2;
    pstVpssReg->VPSS_DM_EDGE.bits.dm_sw_thr_gain  = pstDRValue->u8Dm_sw_thr_gain;
    pstVpssReg->VPSS_DM_EDGE.bits.dm_global_str   = pstDRValue->u8Dm_global_str;
    //pstVpssReg->VPSS_DM_EDGE.bits.dm_str_idx      = pstDRValue->u8Dm_str_idx;

    //VPSS_DM_LUT0
    pstVpssReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p0 = pstDRValue->pu8DmSwWhtLut[0];
    pstVpssReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p1 = pstDRValue->pu8DmSwWhtLut[1];
    pstVpssReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p2 = pstDRValue->pu8DmSwWhtLut[2];
    pstVpssReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p3 = pstDRValue->pu8DmSwWhtLut[3];

    //VPSS_DM_LUT1
    pstVpssReg->VPSS_DM_LUT1.bits.dm_sw_wht_lut_p4 = pstDRValue->pu8DmSwWhtLut[4];

    //VPSS_DM_LUT2
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p0 = pstDRValue->pu8DmDirStrGainLut[0];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p1 = pstDRValue->pu8DmDirStrGainLut[1];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p2 = pstDRValue->pu8DmDirStrGainLut[2];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p3 = pstDRValue->pu8DmDirStrGainLut[3];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p4 = pstDRValue->pu8DmDirStrGainLut[4];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p5 = pstDRValue->pu8DmDirStrGainLut[5];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p6 = pstDRValue->pu8DmDirStrGainLut[6];
    pstVpssReg->VPSS_DM_LUT2.bits.dm_dir_str_gain_lut_p7 = pstDRValue->pu8DmDirStrGainLut[7];

    //VPSS_DM_LUT3
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p0 = pstDRValue->pu8DmDirStrLut[0];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p1 = pstDRValue->pu8DmDirStrLut[1];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p2 = pstDRValue->pu8DmDirStrLut[2];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p3 = pstDRValue->pu8DmDirStrLut[3];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p4 = pstDRValue->pu8DmDirStrLut[4];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p5 = pstDRValue->pu8DmDirStrLut[5];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p6 = pstDRValue->pu8DmDirStrLut[6];
    pstVpssReg->VPSS_DM_LUT3.bits.dm_dir_str_lut_p7 = pstDRValue->pu8DmDirStrLut[7];
    //VPSS_DM_LUT4
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p8 = pstDRValue->pu8DmDirStrLut[8];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p9 = pstDRValue->pu8DmDirStrLut[9];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p10 = pstDRValue->pu8DmDirStrLut[10];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p11 = pstDRValue->pu8DmDirStrLut[11];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p12 = pstDRValue->pu8DmDirStrLut[12];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p13 = pstDRValue->pu8DmDirStrLut[13];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p14 = pstDRValue->pu8DmDirStrLut[14];
    pstVpssReg->VPSS_DM_LUT4.bits.dm_dir_str_lut_p15 = pstDRValue->pu8DmDirStrLut[15];

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetDMParamRegister(HI_U32 u32HandleNo, DM_PARAM_S* pstDRPara)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;
    HI_S32 s32Ret;

    PQ_CHECK_NULL_PTR(pstDRPara);
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    s32Ret = DM_SetParamToReg(pstVpssVirReg, pstDRPara);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ SetDRDefaultValue failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Gain_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[8])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
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
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p3  = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p4  = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p5  = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p6  = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DM_LUT2.bits.dm_dir_str_gain_lut_p7  = u16SrcLut[7];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Dm_Sw_Wht_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[5])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p0  = u16SrcLut[0];
        pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p1  = u16SrcLut[1];
        pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p2  = u16SrcLut[2];
        pstVpssVirReg->VPSS_DM_LUT0.bits.dm_sw_wht_lut_p3  = u16SrcLut[3];

        pstVpssVirReg->VPSS_DM_LUT1.bits.dm_sw_wht_lut_p4  = u16SrcLut[4];
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DM_LUT0.bits.dm_sw_wht_lut_p0  = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DM_LUT0.bits.dm_sw_wht_lut_p1  = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DM_LUT0.bits.dm_sw_wht_lut_p2  = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DM_LUT0.bits.dm_sw_wht_lut_p3  = u16SrcLut[3];

        g_pstIPSelVdpAlg->V0_DM_LUT1.bits.dm_sw_wht_lut_p4  = u16SrcLut[4];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetReg_Dm_Dir_Str_Lut(HI_U32 u32HandleNo, const HI_U16 u16SrcLut[16])
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
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

    }
    else
    {
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p0   = u16SrcLut[0];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p1   = u16SrcLut[1];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p2   = u16SrcLut[2];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p3   = u16SrcLut[3];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p4   = u16SrcLut[4];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p5   = u16SrcLut[5];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p6   = u16SrcLut[6];
        g_pstIPSelVdpAlg->V0_DM_LUT3.bits.dm_dir_str_lut_p7   = u16SrcLut[7];

        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p8   = u16SrcLut[8];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p9   = u16SrcLut[9];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p10  = u16SrcLut[10];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p11  = u16SrcLut[11];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p12  = u16SrcLut[12];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p13  = u16SrcLut[13];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p14  = u16SrcLut[14];
        g_pstIPSelVdpAlg->V0_DM_LUT4.bits.dm_dir_str_lut_p15  = u16SrcLut[15];
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDM(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVpssReg);
        pstVpssReg->VPSS_DB_CTRL.bits.dm_en = bOnOff;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DB_CTRL.bits.dm_en = bOnOff;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableDMDemo(HI_U32 u32Data, HI_BOOL bOnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
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

HI_S32 PQ_HAL_SetDMApiReg(HI_U32 u32HandleNo, DM_API_REG_S* pstDmApiReg)
{
    VPSS_REG_S* pstVpssVirReg = HI_NULL;

    PQ_CHECK_NULL_PTR(pstDmApiReg);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
        pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssVirReg);

        //VPSS_DM_EDGE
        /* can not refresh dm enable every frame */
        //pstVpssVirReg->VPSS_DB_CTRL.bits.dm_en                    = pstDmApiReg->bDmEn;
        pstVpssVirReg->VPSS_DM_LSW.bits.dm_sdbw_mode                = pstDmApiReg->SDBWmode;
        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_str_idx_flag            = pstDmApiReg->StrIdxFlag;

        pstVpssVirReg->VPSS_DM_DIR.bits.dm_opp_ang_ctrst_t          = pstDmApiReg->OppAngCtrstT;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_mndir_opp_ctrst_t       = pstDmApiReg->MNDirOppCtrstT;
        pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_csw_trsnt_lt           = pstDmApiReg->cSWTrsntLT;
        pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_csw_trsnt_lt_10        = pstDmApiReg->cSWTrsntLT10bit;
        pstVpssVirReg->VPSS_DM_LSW.bits.dm_lsw_ratio                = pstDmApiReg->LSWRatio;
        pstVpssVirReg->VPSS_DM_THR0.bits.dm_limit_lsw_ratio         = pstDmApiReg->LimitLSWRatio;

        pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_mmf_lr                 = pstDmApiReg->MMFLR;
        pstVpssVirReg->VPSS_DM_MMFLR.bits.dm_mmf_lr_10              = pstDmApiReg->MMFLR10bit;
        pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_mmf_sr                 = pstDmApiReg->MMFSR;
        pstVpssVirReg->VPSS_DM_MMFSR.bits.dm_mmf_sr_10              = pstDmApiReg->MMFSR10bit;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_mmf_limit_en            = pstDmApiReg->MMFlimitEn;

        pstVpssVirReg->VPSS_DM_THR1.bits.dm_min_lim_val_trans       = pstDmApiReg->MinLimValTrans;
        pstVpssVirReg->VPSS_DM_THR2.bits.dm_min_lim_val_trans_10    = pstDmApiReg->MinLimValTrans10bit;
        pstVpssVirReg->VPSS_DM_THR1.bits.dm_min_lim_trans_wid       = pstDmApiReg->MinLimTransWid;
        pstVpssVirReg->VPSS_DM_THR2.bits.dm_min_lim_trans_wid_10    = pstDmApiReg->MinLimTransWid10bit;
        pstVpssVirReg->VPSS_DM_THR1.bits.dm_lim_trans_wid           = pstDmApiReg->LimTransWid;
        pstVpssVirReg->VPSS_DM_THR2.bits.dm_lim_trans_wid_10        = pstDmApiReg->LimTransWid10bit;

        pstVpssVirReg->VPSS_DM_LUT1.bits.dm_limit_t                 = pstDmApiReg->LimitT;
        pstVpssVirReg->VPSS_DM_LUT1.bits.dm_limit_t_10              = pstDmApiReg->LimitT10bit;
        pstVpssVirReg->VPSS_DM_THR0.bits.dm_lim_res_blend_str1      = pstDmApiReg->LimResBlendStr1;
        pstVpssVirReg->VPSS_DM_THR0.bits.dm_lim_res_blend_str2      = pstDmApiReg->LimResBlendStr2;
        pstVpssVirReg->VPSS_DM_THR0.bits.dm_dir_blend_str           = pstDmApiReg->DirBlendStr;

        pstVpssVirReg->VPSS_DM_DIRO.bits.dm_dir_opp_ctrst_t         = pstDmApiReg->DirOppCtrstT;
        pstVpssVirReg->VPSS_DM_DIRO.bits.dm_dir_opp_ctrst_t_10      = pstDmApiReg->DirOppCtrstT10bit;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_dir_ctrst_t             = pstDmApiReg->DirCtrstT;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_dir_ctrst_t_10          = pstDmApiReg->DirCtrstT10bit;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_sim_dir_pt_count_t      = pstDmApiReg->SimDirPtCountT;

        pstVpssVirReg->VPSS_DM_DIRLR.bits.dm_dir_mmf_lr             = pstDmApiReg->dirMMFLR;
        pstVpssVirReg->VPSS_DM_DIRLR.bits.dm_dir_mmf_lr_10          = pstDmApiReg->dirMMFLR10bit;
        pstVpssVirReg->VPSS_DM_DIRSR.bits.dm_dir_mmf_sr             = pstDmApiReg->dirMMFSR;
        pstVpssVirReg->VPSS_DM_DIRSR.bits.dm_dir_mmf_sr_10          = pstDmApiReg->dirMMFSR10bit;
        pstVpssVirReg->VPSS_DM_DIRC.bits.dm_dir_mmf_en              = pstDmApiReg->DirMMFEn;

        pstVpssVirReg->VPSS_DM_LSW.bits.dm_lw_ctrst_t               = pstDmApiReg->LWCtrstT;
        pstVpssVirReg->VPSS_DM_LSW.bits.dm_lw_ctrst_t_10            = pstDmApiReg->LWCtrstT10bit;
        pstVpssVirReg->VPSS_DM_LSW.bits.dm_csw_trsnt_st             = pstDmApiReg->cSWTrsntST;
        pstVpssVirReg->VPSS_DM_DIRO.bits.dm_csw_trsnt_st_10         = pstDmApiReg->cSWTrsntST10bit;

        pstVpssVirReg->VPSS_DM_DIR.bits.dm_grad_sub_ratio           = pstDmApiReg->u32GradSubRatio;
        pstVpssVirReg->VPSS_DM_DIR.bits.dm_ctrst_thresh             = pstDmApiReg->u32CtrstThresh;
        pstVpssVirReg->VPSS_DM_DIR.bits.dm_opp_ang_ctrst_div        = pstDmApiReg->u32OppAngCtrstDiv;
        pstVpssVirReg->VPSS_DM_DIRO.bits.dm_dir_stren_shift_fac     = pstDmApiReg->DirStrenShiftFac;

        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_edge_thr                = pstDmApiReg->u32EdgeThr;    //u32Dm_edge_thr;
        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_edge_gain1              = pstDmApiReg->u32EdgeGain1;  //u8Dm_edge_gain1;
        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_edge_gain2              = pstDmApiReg->u32EdgeGain2;  //u8Dm_edge_gain2;
        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_asymtrc_gain            = pstDmApiReg->u32AsymtrcGain;//u8Dm_asymtrc_gain;

        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_sw_thr_gain             = pstDmApiReg->u32SWThrGain;  //u8Dm_sw_thr_gain;
        pstVpssVirReg->VPSS_DM_EDGE.bits.dm_global_str              = pstDmApiReg->u32DmGlobalStr;//u8Dm_global_str;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_DM_LSW.bits.dm_sdbw_mode                = pstDmApiReg->SDBWmode;
        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_str_idx_flag            = pstDmApiReg->StrIdxFlag;

        g_pstIPSelVdpAlg->V0_DM_DIR.bits.dm_opp_ang_ctrst_t          = pstDmApiReg->OppAngCtrstT;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_mndir_opp_ctrst_t       = pstDmApiReg->MNDirOppCtrstT;
        g_pstIPSelVdpAlg->V0_DM_MMFLR.bits.dm_csw_trsnt_lt           = pstDmApiReg->cSWTrsntLT;
        g_pstIPSelVdpAlg->V0_DM_MMFSR.bits.dm_csw_trsnt_lt_10        = pstDmApiReg->cSWTrsntLT10bit;
        g_pstIPSelVdpAlg->V0_DM_LSW.bits.dm_lsw_ratio                = pstDmApiReg->LSWRatio;
        g_pstIPSelVdpAlg->V0_DM_THR0.bits.dm_limit_lsw_ratio         = pstDmApiReg->LimitLSWRatio;

        g_pstIPSelVdpAlg->V0_DM_MMFLR.bits.dm_mmf_lr                 = pstDmApiReg->MMFLR;
        g_pstIPSelVdpAlg->V0_DM_MMFLR.bits.dm_mmf_lr_10              = pstDmApiReg->MMFLR10bit;
        g_pstIPSelVdpAlg->V0_DM_MMFSR.bits.dm_mmf_sr                 = pstDmApiReg->MMFSR;
        g_pstIPSelVdpAlg->V0_DM_MMFSR.bits.dm_mmf_sr_10              = pstDmApiReg->MMFSR10bit;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_mmf_limit_en            = pstDmApiReg->MMFlimitEn;

        g_pstIPSelVdpAlg->V0_DM_THR1.bits.dm_min_lim_val_trans       = pstDmApiReg->MinLimValTrans;
        g_pstIPSelVdpAlg->V0_DM_THR2.bits.dm_min_lim_val_trans_10    = pstDmApiReg->MinLimValTrans10bit;
        g_pstIPSelVdpAlg->V0_DM_THR1.bits.dm_min_lim_trans_wid       = pstDmApiReg->MinLimTransWid;
        g_pstIPSelVdpAlg->V0_DM_THR2.bits.dm_min_lim_trans_wid_10    = pstDmApiReg->MinLimTransWid10bit;
        g_pstIPSelVdpAlg->V0_DM_THR1.bits.dm_lim_trans_wid           = pstDmApiReg->LimTransWid;
        g_pstIPSelVdpAlg->V0_DM_THR2.bits.dm_lim_trans_wid_10        = pstDmApiReg->LimTransWid10bit;

        g_pstIPSelVdpAlg->V0_DM_LUT1.bits.dm_limit_t                 = pstDmApiReg->LimitT;
        g_pstIPSelVdpAlg->V0_DM_LUT1.bits.dm_limit_t_10              = pstDmApiReg->LimitT10bit;
        g_pstIPSelVdpAlg->V0_DM_THR0.bits.dm_lim_res_blend_str1      = pstDmApiReg->LimResBlendStr1;
        g_pstIPSelVdpAlg->V0_DM_THR0.bits.dm_lim_res_blend_str2      = pstDmApiReg->LimResBlendStr2;
        g_pstIPSelVdpAlg->V0_DM_THR0.bits.dm_dir_blend_str           = pstDmApiReg->DirBlendStr;

        g_pstIPSelVdpAlg->V0_DM_DIRO.bits.dm_dir_opp_ctrst_t         = pstDmApiReg->DirOppCtrstT;
        g_pstIPSelVdpAlg->V0_DM_DIRO.bits.dm_dir_opp_ctrst_t_10      = pstDmApiReg->DirOppCtrstT10bit;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_dir_ctrst_t             = pstDmApiReg->DirCtrstT;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_dir_ctrst_t_10          = pstDmApiReg->DirCtrstT10bit;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_sim_dir_pt_count_t      = pstDmApiReg->SimDirPtCountT;

        g_pstIPSelVdpAlg->V0_DM_DIRLR.bits.dm_dir_mmf_lr             = pstDmApiReg->dirMMFLR;
        g_pstIPSelVdpAlg->V0_DM_DIRLR.bits.dm_dir_mmf_lr_10          = pstDmApiReg->dirMMFLR10bit;
        g_pstIPSelVdpAlg->V0_DM_DIRSR.bits.dm_dir_mmf_sr             = pstDmApiReg->dirMMFSR;
        g_pstIPSelVdpAlg->V0_DM_DIRSR.bits.dm_dir_mmf_sr_10          = pstDmApiReg->dirMMFSR10bit;
        g_pstIPSelVdpAlg->V0_DM_DIRC.bits.dm_dir_mmf_en              = pstDmApiReg->DirMMFEn;

        g_pstIPSelVdpAlg->V0_DM_LSW.bits.dm_lw_ctrst_t               = pstDmApiReg->LWCtrstT;
        g_pstIPSelVdpAlg->V0_DM_LSW.bits.dm_lw_ctrst_t_10            = pstDmApiReg->LWCtrstT10bit;
        g_pstIPSelVdpAlg->V0_DM_LSW.bits.dm_csw_trsnt_st             = pstDmApiReg->cSWTrsntST;
        g_pstIPSelVdpAlg->V0_DM_DIRO.bits.dm_csw_trsnt_st_10         = pstDmApiReg->cSWTrsntST10bit;

        g_pstIPSelVdpAlg->V0_DM_DIR.bits.dm_grad_sub_ratio           = pstDmApiReg->u32GradSubRatio;
        g_pstIPSelVdpAlg->V0_DM_DIR.bits.dm_ctrst_thresh             = pstDmApiReg->u32CtrstThresh;
        g_pstIPSelVdpAlg->V0_DM_DIR.bits.dm_opp_ang_ctrst_div        = pstDmApiReg->u32OppAngCtrstDiv;
        g_pstIPSelVdpAlg->V0_DM_DIRO.bits.dm_dir_stren_shift_fac     = pstDmApiReg->DirStrenShiftFac;

        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_edge_thr                = pstDmApiReg->u32EdgeThr;    //u32Dm_edge_thr;
        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_edge_gain1              = pstDmApiReg->u32EdgeGain1;  //u8Dm_edge_gain1;
        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_edge_gain2              = pstDmApiReg->u32EdgeGain2;  //u8Dm_edge_gain2;
        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_asymtrc_gain            = pstDmApiReg->u32AsymtrcGain;//u8Dm_asymtrc_gain;

        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_sw_thr_gain             = pstDmApiReg->u32SWThrGain;  //u8Dm_sw_thr_gain;
        g_pstIPSelVdpAlg->V0_DM_EDGE.bits.dm_global_str              = pstDmApiReg->u32DmGlobalStr;//u8Dm_global_str;
    }

    return HI_SUCCESS;
}

