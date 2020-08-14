/****************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_ifmd.c
  Version       : Initial Draft
  Author        : z00128410
  Created       : 2014/7/21
  Description   : 移植3716CV200的IFMD软算法

******************************************************************************/
#include "pq_hal_comm.h"
#include "pq_hal_fmd.h"
#include "pq_mng_ifmd.h"
#include "drv_pq_table.h"



#define FIELD_UNKNOWN      (2)


extern HI_U32 g_u32IPDetect;
extern HI_U32 g_u32FrmRateThr_L;
extern HI_U32 g_u32FrmRateThr_H;

static HI_S32  sg_s32DeiMdLum   = 0;
static ALG_FMD_SOFTINFO_S    sgIfmdTotalSoftParaN[VPSS_HANDLE_NUM];
static REG_PLAYBACK_CONFIG_S sg_FmdDectInfo;

static PQ_DIE_OUT_MODE_E     sg_enDieOutMode  = DIE_OUT_MODE_AUTO;
static PQ_FOD_MODE_E         sg_enFodMode     = PQ_FOD_ENABLE_AUTO;
static PQ_IFMD_TUN_MODE_E    sg_enIfmdTunMode = IFMD_TUN_NORMAL;

static HI_BOOL sg_bIFmdInitFlag = HI_FALSE;

HI_S32 PQ_MNG_IFMD_SofewareParaInitByHandleNo(HI_U32 u32HandleNo)
{
    ALG_FMD_SOFTINFO_S* pstFmdSoftInfo = &(sgIfmdTotalSoftParaN[u32HandleNo]);
    REF_FLD_S*          pstRefFld      = &(pstFmdSoftInfo->stRefFld);
    BTMODE_S*           pstBtMode      = &(pstFmdSoftInfo->stBtMode);
    ALG_FMD_CTX_S*      pstFmdCtx      = &(pstFmdSoftInfo->stFmdCtx);

    pstFmdSoftInfo->u32InitFlag = 0;
    memset(pstFmdCtx, 0, sizeof(ALG_FMD_CTX_S));
    pstFmdCtx->pSadRd      = pstFmdCtx->pSadWt = pstFmdCtx->SadBuf;
    pstFmdCtx->SadCnt      = 0;
    pstFmdCtx->Last32Phase = -1;

    pstFmdCtx->SADDiffAcc  = 0;
    pstFmdCtx->FieldOrder  = FIELD_UNKNOWN;
    pstFmdCtx->init_symbol = 1;
    pstFmdCtx->stRepeatHist.lst_repeat = 0;
    pstFmdCtx->stRepeatHist.cur_repeat = 0;
    pstFmdCtx->stRepeatHist.nxt_repeat = 0;
    pstFmdCtx->stDropHist.lst_drop = 0;
    pstFmdCtx->stDropHist.cur_drop = 0;
    pstFmdCtx->stDropHist.nxt_drop = 0;

    pstRefFld->g_lstreffld = 0xff;
    pstRefFld->g_curreffld = 0xff;
    pstRefFld->g_nxtreffld = 0xff;

    pstBtMode->g_lstbtmode = 0xff;
    pstBtMode->g_curbtmode = 0xff;
    pstBtMode->g_nxtbtmode = 0xff;

    pstFmdSoftInfo->StillBlkCtrl.STILLBLK_THR = 0;

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_FmdDeInit(ALG_FMD_SOFTINFO_S* pstFmdSoftInfo)
{
    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_IFMD_SofewareParaInit(HI_VOID)
{
    HI_U32 i = 0;

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        PQ_MNG_IFMD_SofewareParaInitByHandleNo(i);
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_GetIfmdSoftTable(HI_BOOL bInit)
{
    if ((bInit == HI_FALSE) && (IFMD_TUN_NORMAL == sg_enIfmdTunMode))
    {
        return HI_FAILURE;
    }
	
    g_u32IPDetect     = PQ_MNG_GetSoftTable(PQ_SOFT_IP_DETECT,    SOURCE_MODE_NO, OUTPUT_MODE_NO, IP_DECTECT_INTERLACE_PRIMARY);
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    g_u32FrmRateThr_L = PQ_MNG_GetSoftTable(PQ_SOFT_FRMRATETHR_L, SOURCE_MODE_NO, OUTPUT_MODE_NO, PLAYBACK_FRMRATETHR_L);
    g_u32FrmRateThr_H = PQ_MNG_GetSoftTable(PQ_SOFT_FRMRATETHR_H, SOURCE_MODE_NO, OUTPUT_MODE_NO, PLAYBACK_FRMRATETHR_H);
#endif

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_InitIfmd(PQ_PARAM_S* pstPqParam)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    if (HI_TRUE == sg_bIFmdInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_MNG_IFMD_SofewareParaInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("ifmd init failed\n");
        return HI_FAILURE;
    }

    PQ_HAL_SetIFmdEn(u32HandleNo, HI_TRUE);

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_FMD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("ifmd InitPhyList error\n");
        return HI_FAILURE;
    }

    PQ_MNG_GetIfmdSoftTable(HI_TRUE);

    sg_bIFmdInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitIfmd(HI_VOID)
{
    HI_U32 i = 0;
    ALG_FMD_SOFTINFO_S* pstFmdSoftInfo = HI_NULL;

    if (HI_FALSE == sg_bIFmdInitFlag)
    {
        return HI_SUCCESS;
    }

    for (i = 0; i < VPSS_HANDLE_NUM; i++)
    {
        pstFmdSoftInfo = &(sgIfmdTotalSoftParaN[i]);
        PQ_MNG_FmdDeInit(pstFmdSoftInfo);
    }
    sg_bIFmdInitFlag = HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 PQ_MNG_SetIFmdEn(HI_U32 u32HandleNo, HI_U32 u32IFmdEn)
{
    return PQ_HAL_SetIFmdEn(u32HandleNo, u32IFmdEn);
}

static HI_S32 PQ_MNG_GetIFmdStatInfo(PQ_IFMD_CALC_S* pstIfmdCalcInfo, IFMD_TotalHardPara* pIfmdTotalHardPara)
{
    return PQ_HAL_GetIFmdStatInfo(pstIfmdCalcInfo, pIfmdTotalHardPara);
}

HI_S32 PQ_MNG_SetIFmdInfo(HI_U32 u32HandleNo, REG_PLAYBACK_CONFIG_S* pstCfg)
{
    REG_PLAYBACK_CONFIG_S stNewCfg;

    memcpy(&stNewCfg, pstCfg, sizeof(REG_PLAYBACK_CONFIG_S));

    if (DIE_OUT_MODE_FIR == sg_enDieOutMode)
    {
        stNewCfg.die_out_sel = 0;
    }
    else if (DIE_OUT_MODE_COPY == sg_enDieOutMode)
    {
        stNewCfg.die_out_sel = 1;
    }

    return PQ_HAL_SetIFmdInfo( u32HandleNo, &stNewCfg);
}

HI_S32 PQ_MNG_SetEdgeSmoothEn(HI_U32 u32HandleNo, HI_BOOL bEdgeSmoothEn)
{
    return PQ_HAL_SetEdgeSmoothEn(u32HandleNo, bEdgeSmoothEn);
}


/*****************************************************************************
 函 数 名  : PQ_MNG_IFMD_GetPlayBackConfig
 功能描述  : API的总入口，用于根据检测和回放延时来获取新的回放配置参数
 返 回 值  :
 调用函数  :
 被调函数  :

*****************************************************************************/
HI_S32 PQ_MNG_IFMD_GetPlayBackConfig(IFMD_TotalHardPara* pIfmdTotalHardPara, PQ_IFMD_CALC_S* pstIfmdCalc, REG_PLAYBACK_CONFIG_S* pstIfmdResult)
{
    ALG_FMD_SOFTINFO_S*   pstFmdSoftInfo = &(sgIfmdTotalSoftParaN[pstIfmdCalc->u32HandleNo]);
    ALG_FMD_RTL_OUTPARA_S stFmdRtlOutPara;
    ALG_DEI_DRV_PARA_S    stDeiDrvPara;

    memset(&stFmdRtlOutPara, 0, sizeof(stFmdRtlOutPara));
    memset(&stDeiDrvPara, 0, sizeof(stDeiDrvPara));

    memcpy(&(stDeiDrvPara.stFmdRtlStatPara), &(pIfmdTotalHardPara->stFmdRtlStatPara), sizeof(ALG_FMD_RTL_STATPARA_S));
    memcpy(&(stDeiDrvPara.stMadRtlStatPara), &(pIfmdTotalHardPara->stMadRtlStatPara), sizeof(ALG_MAD_RTL_STATPARA_S));
    memcpy(&(stDeiDrvPara.stVdecInfo), &(pstIfmdCalc->stVdecTranInfo), sizeof(ALG_VDEC_INFO_S));
    stDeiDrvPara.stVdecInfo.RealFrmRate = pstIfmdCalc->u32FrameRate / 2;

    stDeiDrvPara.s32DeiMdLum  = ALG_DEI_MODE_4FLD;
    stDeiDrvPara.s32DeiMdChr  = ALG_DEI_MODE_4FLD;

    stDeiDrvPara.s32FrmWidth  = pstIfmdCalc->u32WidthY;
    stDeiDrvPara.s32FrmHeight = pstIfmdCalc->u32HeightY;
    stDeiDrvPara.BtMode       = pstIfmdCalc->s32FieldOrder;
    stDeiDrvPara.RefFld       = pstIfmdCalc->s32FieldMode;
    stDeiDrvPara.bPreInfo     = pstIfmdCalc->bPreInfo;
    stDeiDrvPara.bOfflineMode = HI_TRUE;
    stDeiDrvPara.FodEnable    = (HI_S32)sg_enFodMode;
    stDeiDrvPara.Pld22Enable  = HI_TRUE;
    stDeiDrvPara.Pld32Enable  = HI_TRUE;
    stDeiDrvPara.s32Drop      = 0;
    stDeiDrvPara.s32Repeat    = 0;

    /* 利用从硬件得到的数据，进行软件层的判断 */
    PQ_MNG_ALG_FmdSet(pstFmdSoftInfo, &stDeiDrvPara, &stFmdRtlOutPara);

    pstIfmdResult->die_out_sel     = stFmdRtlOutPara.DieOutSelLum;
    pstIfmdResult->dir_mch         = stFmdRtlOutPara.DirMch;
    pstIfmdResult->s32FieldOrder   = stFmdRtlOutPara.s32FieldOrder;
    pstIfmdResult->u32EdgeSmoothEn = stFmdRtlOutPara.EdgeSmoothEn;

    sg_s32DeiMdLum = stDeiDrvPara.s32DeiMdLum;

    if (DIE_OUT_MODE_FIR == sg_enDieOutMode)
    {
        pstIfmdResult->die_out_sel = 0;
    }
    else if (DIE_OUT_MODE_COPY == sg_enDieOutMode)
    {
        pstIfmdResult->die_out_sel = 1;
    }

    sg_FmdDectInfo.die_out_sel     = pstIfmdResult->die_out_sel;
    sg_FmdDectInfo.dir_mch         = pstIfmdResult->dir_mch;
    sg_FmdDectInfo.s32FieldOrder   = pstIfmdResult->s32FieldOrder;
    sg_FmdDectInfo.u32EdgeSmoothEn = pstIfmdResult->u32EdgeSmoothEn;

    return HI_SUCCESS;
}
/* 隔行FMD模式检测
   param[in]  pstIfmdCalc   算法所需参数(统计信息地址，场序，顶底场标识等)
   param[out] pstIfmdResult IFMD计算结果
   */
HI_S32 PQ_MNG_IfmdDect(PQ_IFMD_CALC_S* pstIfmdCalc, REG_PLAYBACK_CONFIG_S* pstIfmdResult)
{
    static IFMD_TotalHardPara IfmdTotalHardPara;

    PQ_CHECK_NULL_PTR(pstIfmdResult);
    PQ_CHECK_NULL_PTR(pstIfmdCalc);

    /*get software table*/
    PQ_MNG_GetIfmdSoftTable(HI_FALSE);
    /* From VPSS Get IfmdTotalHardPara */
    PQ_MNG_GetIFmdStatInfo(pstIfmdCalc, &IfmdTotalHardPara);
    /* Clac to pstIfmdResult */
    PQ_MNG_IFMD_GetPlayBackConfig(&IfmdTotalHardPara, pstIfmdCalc, pstIfmdResult);
    /* Set pstIfmdResult to CFG Reg */
    PQ_MNG_SetIFmdInfo(pstIfmdCalc->u32HandleNo, pstIfmdResult);
    PQ_MNG_SetEdgeSmoothEn(pstIfmdCalc->u32HandleNo, pstIfmdResult->u32EdgeSmoothEn);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetIfmdDectInfo(REG_PLAYBACK_CONFIG_S* pstIfmdInfo)
{
    pstIfmdInfo->die_out_sel     = sg_FmdDectInfo.die_out_sel;
    pstIfmdInfo->dir_mch         = sg_FmdDectInfo.dir_mch;
    pstIfmdInfo->s32FieldOrder   = sg_FmdDectInfo.s32FieldOrder;
    pstIfmdInfo->u32EdgeSmoothEn = sg_FmdDectInfo.u32EdgeSmoothEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDieOutMode(PQ_DIE_OUT_MODE_E enMode)
{
    sg_enDieOutMode = enMode;
    if (DIE_OUT_MODE_FIR == sg_enDieOutMode)
    {
        sg_FmdDectInfo.die_out_sel = 0;
    }
    else if (DIE_OUT_MODE_COPY == sg_enDieOutMode)
    {
        sg_FmdDectInfo.die_out_sel = 1;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetFodMode(PQ_FOD_MODE_E enMode)
{
    sg_enFodMode = enMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDeiMdLum(HI_VOID)
{
    return sg_s32DeiMdLum;
}

HI_S32 PQ_MNG_SetIfmdTunMode(PQ_IFMD_TUN_MODE_E enMode)
{
    sg_enIfmdTunMode = enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetIfmdTunMode(PQ_IFMD_TUN_MODE_E* penMode)
{
    *penMode = sg_enIfmdTunMode;

    return HI_SUCCESS;
}



