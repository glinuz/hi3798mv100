/******************************************************************************
*
* Copyright (C) 2015 -2016  Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_mcmatnr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/05/25
  Description   :

******************************************************************************/
#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_mng_tnr.h"
#include "pq_hal_comm.h"
#include "pq_hal_tnr.h"
#ifdef PQ_ALG_MCNR
#include "pq_mng_mcnr.h"
#endif


static HI_BOOL sg_bTNRInitFlag = HI_FALSE;
static HI_BOOL sg_bTnrEn       = HI_FALSE;
static HI_BOOL sg_bTnrDemoEn   = HI_FALSE;

static HI_U32  sg_u32TnrStr    = 0;
static PQ_DEMO_MODE_E sg_enTnrDemoMode = PQ_DEMO_ENABLE_R;


HI_S32 PQ_MNG_InitTnr(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_U32 u32HandleNo = 0;
    sg_u32TnrStr = 50;
    sg_bTnrEn = HI_TRUE;
    sg_bTnrDemoEn = HI_FALSE;

    if (HI_TRUE == sg_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

    PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_TNR, SOURCE_MODE_SD, OUTPUT_MODE_NO);
    PQ_HAL_SetTNRMotionStrLut(u32HandleNo);
    PQ_HAL_SetTNRBlendingAlphaLut(u32HandleNo);

    PQ_HAL_SetTnrMidStr(u32HandleNo);

    sg_bTNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 PQ_MNG_DeInitTnr(HI_VOID)
{
    if (HI_FALSE == sg_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bTNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableTnr(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    sg_bTnrEn = bOnOff;

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_TNR, enSourceMode)))
    {
        sg_bTnrEn = HI_FALSE;
    }

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableTNR(u32HandleNo, sg_bTnrEn);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bTnrEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableTnrDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;

    sg_bTnrDemoEn = bOnOff;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableTNRDemo(u32HandleNo, bOnOff);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrDemo(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bTnrDemoEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;

    sg_enTnrDemoMode = enMode;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTNRDemoMode(u32HandleNo, (TNR_DEMO_MODE_E)enMode);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrDemoMode(PQ_DEMO_MODE_E* penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enTnrDemoMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrStrength(HI_U32 u32TnrStr)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;

    if (u32TnrStr > 100)
    {
        HI_ERR_PQ("the value[%d] of setting NR strength is out of range!\n", u32TnrStr);
        return HI_FAILURE;
    }

    sg_u32TnrStr = u32TnrStr;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrStr(u32HandleNo, u32TnrStr);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrStrength(HI_U32* pu32TnrStr)
{
    PQ_CHECK_NULL_PTR(pu32TnrStr);
    *pu32TnrStr = sg_u32TnrStr;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrDemoModeCoor (HI_U32 u32XPos)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetTnrDemoModeCoor(u32HandleNo, u32XPos);
        }
    }

    return s32Ret;
}


static stPQAlgFuncs stTNRFuncs =
{
    .Init               = PQ_MNG_InitTnr,
    .DeInit             = PQ_MNG_DeInitTnr,
    .SetEnable          = PQ_MNG_EnableTnr,
    .GetEnable          = PQ_MNG_GetTnrEnableFlag,
    .SetDemo            = PQ_MNG_EnableTnrDemo,
    .GetDemo            = PQ_MNG_GetTnrDemo,
    .SetDemoMode        = PQ_MNG_SetTnrDemoMode,
    .GetDemoMode        = PQ_MNG_GetTnrDemoMode,
    .SetStrength        = PQ_MNG_SetTnrStrength,
    .GetStrength        = PQ_MNG_GetTnrStrength,
    .SetDemoModeCoor    = PQ_MNG_SetTnrDemoModeCoor,
#ifdef PQ_ALG_MCNR
    .UpdateMCNRCfg	    = PQ_MNG_UpdateMCNRCfg,
    .EnableMCNR         = PQ_MNG_EnableMCNR,
    .GetMCNREnable      = PQ_MNG_GetMCNREnable,
#endif
};

HI_S32 PQ_MNG_RegisterTNR(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_TNR, enType, PQ_BIN_ADAPT_MULTIPLE, "tnr", HI_NULL, &stTNRFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterTNR()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_TNR);

    return s32Ret;
}

