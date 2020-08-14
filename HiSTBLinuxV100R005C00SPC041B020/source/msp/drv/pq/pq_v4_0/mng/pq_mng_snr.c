/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_snr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/10/19
  Description   :

******************************************************************************/
#include "hi_drv_mem.h"
#include "drv_pq_table.h"
#include "pq_hal_comm.h"
#include "pq_mng_snr.h"
#if defined(CHIP_TYPE_hi3798cv200)
#include "pq_hal_algchange.h"
#endif

static HI_BOOL g_bSNRInitFlag = HI_FALSE;
static HI_BOOL sg_bSnrEn      = HI_FALSE;
static HI_BOOL sg_bSnrDemoEn  = HI_FALSE;

static HI_S32 sg_u32SnrStr;
static PQ_DEMO_MODE_E sg_enSnrDemoMode = PQ_DEMO_ENABLE_R;

HI_S32 PQ_MNG_InitSnr(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    sg_u32SnrStr = 50;
    sg_bSnrEn = HI_TRUE;
    sg_bSnrDemoEn = HI_FALSE;

    if (HI_TRUE == g_bSNRInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_HAL_SetSNRCbCrLut(u32HandleNo);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("SetSNRCbCrLut error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_HAL_SetSNRMotionEdgeLut(u32HandleNo, 1920);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("SetSNRCbCrLut error!\n");
        return HI_FAILURE;
    }

    PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_SNR, SOURCE_MODE_NO, OUTPUT_MODE_NO);
#if defined(CHIP_TYPE_hi3798mv200) ||  defined(CHIP_TYPE_hi3798mv200_a)
    PQ_HAL_SetSNRRatioMap(u32HandleNo);
#endif
    PQ_HAL_SetSnrMidStr(0);

    g_bSNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitSnr(HI_VOID)
{
    if (HI_FALSE == g_bSNRInitFlag)
    {
        return HI_SUCCESS;
    }

    g_bSNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableSnr(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_SNR, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    sg_bSnrEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableSNR(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableSNR(u32Vid, bOnOff);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetSnrEnableFlag(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bSnrEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableSnrDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_bSnrDemoEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableSNRDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableSNRDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetSnrDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bSnrDemoEn;
    return HI_SUCCESS;
}


HI_S32 PQ_MNG_SetSnrDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_enSnrDemoMode = enMode;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSNRDemoMode(u32HandleNo, (SNR_DEMO_MODE_E)enMode);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSNRDemoMode(u32Vid, (SNR_DEMO_MODE_E)enMode);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetSnrDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enSnrDemoMode;
    return HI_SUCCESS;
}

/* Set Snr Strengh; s32SnrStr  Snr Strengh[0,100] */
HI_S32 PQ_MNG_SetSnrStrength(HI_U32 u32SnrStr)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (u32SnrStr > 100)
    {
        HI_ERR_PQ("the value[%d] of setting SNR strength is out of range!\n", u32SnrStr);
        return HI_FAILURE;
    }

    sg_u32SnrStr = u32SnrStr;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSnrStr(u32HandleNo, u32SnrStr);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSnrStr(u32Vid, u32SnrStr);
    }

    return s32Ret;
}


/* Get Snr Strengh ; s32SnrStr Strengh[0,100] */
HI_S32 PQ_MNG_GetSnrStrength(HI_U32 *pu32SnrStr)
{
    PQ_CHECK_NULL_PTR(pu32SnrStr);

    *pu32SnrStr = sg_u32SnrStr;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSnrDemoModeCoor (HI_U32 u32XPos)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSnrDemoModeCoor(u32HandleNo, u32XPos);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSnrDemoModeCoor(u32Vid, u32XPos);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetSNRCbCrLut (HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSNRCbCrLut(u32HandleNo);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSNRCbCrLut(u32Vid);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetSNRMotionEdgeLut (HI_U32 u32Width)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSNRMotionEdgeLut(u32HandleNo, u32Width);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSNRMotionEdgeLut(u32Vid, u32Width);
    }

    return s32Ret;
}


#if defined(CHIP_TYPE_hi3798cv200)
HI_S32 PQ_MNG_SetSnrMeanRatio(HI_PQ_SNR_MEAN_RATIO_S *pstMeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstMeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSnrMeanRatio(u32HandleNo, (PQ_HAL_SNR_MEAN_RATIO_S *)pstMeanRatio);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSnrMeanRatio(u32Vid, (PQ_HAL_SNR_MEAN_RATIO_S *)pstMeanRatio);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetSnrMeanRatio(HI_PQ_SNR_MEAN_RATIO_S *pstMeanRatio)
{
    PQ_CHECK_NULL_PTR(pstMeanRatio);

    return PQ_HAL_GetSnrMeanRatio(PQ_VDP_LAYER_VID0, (PQ_HAL_SNR_MEAN_RATIO_S *)pstMeanRatio);
}

HI_S32 PQ_MNG_SetSnrEdgeStr(HI_PQ_SNR_EDGE_STR_S *pstEdgeStr)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetSnrEdgeStr(u32HandleNo, (PQ_HAL_SNR_EDGE_STR_S *)pstEdgeStr);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetSnrEdgeStr(u32Vid, (PQ_HAL_SNR_EDGE_STR_S *)pstEdgeStr);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetSnrEdgeStr(HI_PQ_SNR_EDGE_STR_S *pstEdgeStr)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    s32Ret = PQ_HAL_GetSnrEdgeStr(u32Vid, (PQ_HAL_SNR_EDGE_STR_S *)pstEdgeStr);

    return s32Ret;
}

HI_S32  PQ_MNG_SetVdpSnrParaUpd (HI_U32 u32Data)
{
    HI_U32 u32VId = PQ_VDP_LAYER_VID0;
    return PQ_HAL_SetVdpSnrParaUpd(u32VId);
}

HI_S32 PQ_MNG_InitVdpSnr(HI_VOID)
{
    HI_U32 u32VId = PQ_VDP_LAYER_VID0;
    PQ_HAL_InitVdpSnr();
    return PQ_HAL_SetVdpSnrEnable(u32VId, HI_TRUE);
}
#endif

static stPQAlgFuncs stSNRFuncs =
{
    .Init                = PQ_MNG_InitSnr,
    .DeInit              = PQ_MNG_DeInitSnr,
    .SetEnable           = PQ_MNG_EnableSnr,
    .GetEnable           = PQ_MNG_GetSnrEnableFlag,
    .SetDemo             = PQ_MNG_EnableSnrDemo,
    .GetDemo             = PQ_MNG_GetSnrDemo,
    .SetDemoMode         = PQ_MNG_SetSnrDemoMode,
    .GetDemoMode         = PQ_MNG_GetSnrDemoMode,
    .SetStrength         = PQ_MNG_SetSnrStrength,
    .GetStrength         = PQ_MNG_GetSnrStrength,
    .SetDemoModeCoor     = PQ_MNG_SetSnrDemoModeCoor,
    .SetSNRCbCrLut       = PQ_MNG_SetSNRCbCrLut,
    .SetSNRMotionEdgeLut = PQ_MNG_SetSNRMotionEdgeLut,
#if defined(CHIP_TYPE_hi3798cv200)
    .SetVdpSnrParaUpd    = PQ_MNG_SetVdpSnrParaUpd,
    .InitVdpSnr          = PQ_MNG_InitVdpSnr,
#endif
};

HI_S32 PQ_MNG_RegisterSNR(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_SNR, enType, PQ_BIN_ADAPT_MULTIPLE, "snr", HI_NULL, &stSNRFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterSNR()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_SNR);

    return s32Ret;
}

