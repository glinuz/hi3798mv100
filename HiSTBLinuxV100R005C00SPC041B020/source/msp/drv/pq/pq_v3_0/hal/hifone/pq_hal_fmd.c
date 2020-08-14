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
    HI_U32 i;

    PQ_CHECK_NULL_PTR(pIfmdTotalHardPara);
    PQ_CHECK_NULL_PTR(pstIfmdCalcInfo);

    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_1 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDHISTBIN[0];
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_2 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDHISTBIN[1];
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_3 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDHISTBIN[2];
    pIfmdTotalHardPara->stFmdRtlStatPara.frmHstBin.HISTOGRAM_BIN_4 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDHISTBIN[3];

    pIfmdTotalHardPara->stFmdRtlStatPara.frmITDiff = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDFRMITDIFF;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_BWD      = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCBWD0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FWD      = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFWD0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FFWD     = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFFWD0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_CRSS     = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCCRSS0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_FWD_TKR  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFWDTKR0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc.PCC_BWD_TKR  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCBWDTKR0;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_BWD     = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCBWD1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FWD     = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFWD1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FFWD    = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFFWD1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_CRSS    = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCCRSS1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_FWD_TKR = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCFWDTKR1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmPcc2.PCC_BWD_TKR = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDPCCBWDTKR1;

    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.match_UM      = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDUMMATCH0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.nonmatch_UM   = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDUMNOMATCH0;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.match_UM2     = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDUMMATCH1;
    pIfmdTotalHardPara->stFmdRtlStatPara.frmUm.nonmatch_UM2  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDUMNOMATCH1;

    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt14  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT140;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt32  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT320;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat.lasiCnt34  = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT340;

    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt14 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT141;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt32 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT321;
    pIfmdTotalHardPara->stFmdRtlStatPara.lasiStat2.lasiCnt34 = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDLASICNT341;

    pIfmdTotalHardPara->stFmdRtlStatPara.SceneChangeInfo.iCHD = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDICHD;

    for (i = 0; i < 16; i++)
    {
        pIfmdTotalHardPara->stFmdRtlStatPara.StillBlkInfo.BlkSad[i] = pstIfmdCalcInfo->pstIfmdHardOutReg->VPSS_PDSTLBLKSAD[i];
    }

    pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnNum = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_NUM0;
    pIfmdTotalHardPara->stMadRtlStatPara.u32SmallMtnSum = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_SUM0;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum0  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_NUM1;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum0  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_SUM1;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum1  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_NUM2;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum1  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_SUM2;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnNum2  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_NUM3;
    pIfmdTotalHardPara->stMadRtlStatPara.u32MidMtnSum2  = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_SUM3;
    pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnNum = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_NUM4;
    pIfmdTotalHardPara->stMadRtlStatPara.u32LargeMtnSum = pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_SUM4;

    memcpy((HI_U32*) & (pIfmdTotalHardPara->stMadRtlStatPara.u32MotionHist[0]), (HI_U32*) & (pstIfmdCalcInfo->pstIfmdHardOutReg->IGLB_MTN_BIN0), sizeof(HI_U32) * 32);

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
    S_CAS_REGS_TYPE* pstVpssVirReg = NULL;
    pstVpssVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssVirReg);

    pstVpssVirReg->VPSS_CTRL.bits.ifmd_en = u32IFmdEn;

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


