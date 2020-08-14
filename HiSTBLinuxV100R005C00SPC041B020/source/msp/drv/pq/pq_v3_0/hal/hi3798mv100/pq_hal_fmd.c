/****************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_fmd.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/2/18
  Description   :

******************************************************************************/

#include "pq_hal_fmd.h"

HI_S32 PQ_HAL_GetIFmdStatInfo(PQ_IFMD_CALC_S* pstIfmdCalcInfo, IFMD_TotalHardPara* pIfmdTotalHardPara)
{
    HI_S32 i = 0;
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;

    PQ_CHECK_NULL_PTR(pIfmdTotalHardPara);

    pstVpssVirReg = PQ_HAL_GetVpssReg(pstIfmdCalcInfo->u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_1 = pstVpssVirReg->VPSS_PDHISTBIN[0].bits.hsit_bin;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_2 = pstVpssVirReg->VPSS_PDHISTBIN[1].bits.hsit_bin;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_3 = pstVpssVirReg->VPSS_PDHISTBIN[2].bits.hsit_bin;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_4 = pstVpssVirReg->VPSS_PDHISTBIN[3].bits.hsit_bin;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmITDiff = pstVpssVirReg->VPSS_PDFRMITDIFF.bits.frm_it_diff;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_BWD     = pstVpssVirReg->VPSS_PDPCCBWD0.bits.pcc_bwd_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FWD     = pstVpssVirReg->VPSS_PDPCCFWD0.bits.pcc_fwd_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FFWD    = pstVpssVirReg->VPSS_PDPCCFFWD0.bits.pcc_ffwd_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_CRSS    = pstVpssVirReg->VPSS_PDPCCCRSS0.bits.pcc_crss_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FWD_TKR = pstVpssVirReg->VPSS_PDPCCFWDTKR0.bits.pcc_fwd_tkr_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_BWD_TKR = pstVpssVirReg->VPSS_PDPCCBWDTKR0.bits.pcc_bwd_tkr_0;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_BWD     = pstVpssVirReg->VPSS_PDPCCBWD1.bits.pcc_bwd_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FWD     = pstVpssVirReg->VPSS_PDPCCFWD1.bits.pcc_fwd_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FFWD    = pstVpssVirReg->VPSS_PDPCCFFWD1.bits.pcc_ffwd_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_CRSS    = pstVpssVirReg->VPSS_PDPCCCRSS1.bits.pcc_crss_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FWD_TKR = pstVpssVirReg->VPSS_PDPCCFWDTKR1.bits.pcc_fwd_tkr_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_BWD_TKR = pstVpssVirReg->VPSS_PDPCCBWDTKR1.bits.pcc_bwd_tkr_1;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.match_UM      = pstVpssVirReg->VPSS_PDUMMATCH0.bits.match_um;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.nonmatch_UM   = pstVpssVirReg->VPSS_PDUMNOMATCH0.bits.nomatch_um;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.match_UM2     = pstVpssVirReg->VPSS_PDUMMATCH1.bits.match_um;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.nonmatch_UM2  = pstVpssVirReg->VPSS_PDUMNOMATCH1.bits.nomatch_um;

    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt14  = pstVpssVirReg->VPSS_PDLASICNT140.bits.lasi_14_cnt_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt32  = pstVpssVirReg->VPSS_PDLASICNT320.bits.lasi_32_cnt_0;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt34  = pstVpssVirReg->VPSS_PDLASICNT340.bits.lasi_34_cnt_0;

    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt14 = pstVpssVirReg->VPSS_PDLASICNT141.bits.lasi_14_cnt_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt32 = pstVpssVirReg->VPSS_PDLASICNT321.bits.lasi_32_cnt_1;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt34 = pstVpssVirReg->VPSS_PDLASICNT341.bits.lasi_34_cnt_1;

    pIfmdTotalHardPara->stFmdRtlStatPara.SceneChangeInfo.iCHD = pstVpssVirReg->VPSS_PDICHD.bits.ichd;

    for (i = 0; i < 16; i++)
    {
        pIfmdTotalHardPara->stFmdRtlStatPara.StillBlkInfo.BlkSad[i] = pstVpssVirReg->VPSS_PDSTLBLKSAD[i].bits.stlblk_sad;
    }

    pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnNum = pstVpssVirReg->VPSS_DIEMTNNUM0.bits.glbm_num0;
    pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnSum = pstVpssVirReg->VPSS_DIEMTNSUM0.bits.glbm_sum0;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum0  = pstVpssVirReg->VPSS_DIEMTNNUM1.bits.glbm_num1;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum0  = pstVpssVirReg->VPSS_DIEMTNSUM1.bits.glbm_sum1;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum1  = pstVpssVirReg->VPSS_DIEMTNNUM2.bits.glbm_num2;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum1  = pstVpssVirReg->VPSS_DIEMTNSUM2.bits.glbm_sum2;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum2  = pstVpssVirReg->VPSS_DIEMTNNUM3.bits.glbm_num3;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum2  = pstVpssVirReg->VPSS_DIEMTNSUM3.bits.glbm_sum3;
    pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnNum = pstVpssVirReg->VPSS_DIEMTNNUM4.bits.glbm_num4;
    pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnSum = pstVpssVirReg->VPSS_DIEMTNSUM4.bits.glbm_sum4;

    memcpy((HI_U32*) & (pIfmdTotalHardPara->stMadRtlStatPara.u32MotionHist[0]), (HI_U32*) & (pstVpssVirReg->VPSS_DIEGLBMTNBIN[0]), sizeof(HI_U32) * 32);

    pqprint(PQ_PRN_FMD_READ, "[%s]: MtnNum:(%d, %d, %d, %d, %d);MtnSum:(%d, %d, %d, %d, %d);Hist:(%d, %d)\n",
            __FUNCTION__,
            pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnNum,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum0,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum1,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum2,
            pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnNum,
            pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnSum,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum0,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum1,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum2,
            pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnSum,
            pIfmdTotalHardPara->stMadRtlStatPara.u32MotionHist[0],
            pIfmdTotalHardPara->stMadRtlStatPara.u32MotionHist[1]);

    return HI_SUCCESS;
}

/*--------------------------------------------------------------------------------*
Function        : HI_VOID SetIFmdInfo(HI_U32 u32HandleNo,IFMD_TotalHardPara *pIfmdTotalHardPara)

Description     : 每场回写给OSD的寄存器值
Calls           :
Data Accessed   :
Data Updated    :
Input           : u32HandleNo
                : pIfmdTotalHardPara
Output          : NA
Return          : NA
*--------------------------------------------------------------------------------*/

HI_S32 PQ_HAL_SetIFmdInfo(HI_U32 u32HandleNo, REG_PLAYBACK_CONFIG_S* NewCfg)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_DIECTRL.bits.die_out_sel_c = NewCfg->die_out_sel;
    pstVpssVirReg->VPSS_DIECTRL.bits.die_out_sel_l = NewCfg->die_out_sel;
    pstVpssVirReg->VPSS_PDCTRL.bits.dir_mch_c      = NewCfg->dir_mch;
    pstVpssVirReg->VPSS_PDCTRL.bits.dir_mch_l      = NewCfg->dir_mch;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetIFmdEn(HI_U32 u32HandleNo, HI_U32 u32IFmdEn)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetEdgeSmoothEn(HI_U32 u32HandleNo, HI_BOOL bEdgeSmoothEn)
{
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_PDCTRL.bits.edge_smooth_en = bEdgeSmoothEn;

    return HI_SUCCESS;
}


