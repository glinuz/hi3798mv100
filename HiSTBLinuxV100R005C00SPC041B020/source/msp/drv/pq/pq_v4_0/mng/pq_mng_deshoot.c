/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_deshoot.c
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2015/05/11
  Description   :

******************************************************************************/

#include "pq_hal_deshoot.h"
#include "drv_pq_table.h"
#include "pq_hal_comm.h"

static PQ_DEMO_MODE_E sg_enMode  = PQ_DEMO_ENABLE_R;
static HI_BOOL sg_bDeshootDemoEn = HI_FALSE;
static HI_BOOL sg_bDeshootEnFlag = HI_TRUE;

HI_S32 PQ_MNG_InitDeshoot(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32VID = 0;

    /* single DS */
    s32Ret = PQ_TABLE_InitPhyList(u32VID, HI_PQ_MODULE_DS, SOURCE_MODE_HD, OUTPUT_MODE_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Deshoot InitPhyList error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitDeshoot(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDeshootDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_bDeshootDemoEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDeshootDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDeshootDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetDeshootDemo(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDeshootDemoEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDeshootDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;
    HI_U32 u32HandleNo;

    sg_enMode = enMode;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetDeshootDemoMode(u32HandleNo, (DS_DEMO_MODE_E)enMode);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetDeshootDemoMode(u32Vid, (DS_DEMO_MODE_E)enMode);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDeshootDemoMode(PQ_DEMO_MODE_E* penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDeshootThr(HI_U32 u32ID, HI_U32 u32Width)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        s32Ret = PQ_HAL_SetDeshootThr(u32ID, u32Width);
    }
    else
    {
        s32Ret = PQ_HAL_SetDeshootThr(u32Vid, u32Width);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_EnableDeshoot(HI_BOOL bOnOff)
{
    HI_U32  u32HandleNo = 0, u32Vid = PQ_VDP_LAYER_VID0;
    HI_S32  s32Ret = HI_SUCCESS;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    sg_bDeshootEnFlag = bOnOff;

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DS, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DS))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDeshoot(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDeshoot(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDeshootEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDeshootEnFlag;
    return HI_SUCCESS;
}

static stPQAlgFuncs stDSFuncs =
{
    .Init               = PQ_MNG_InitDeshoot,
    .DeInit             = PQ_MNG_DeInitDeshoot,
    .SetEnable          = PQ_MNG_EnableDeshoot,
    .GetEnable          = PQ_MNG_GetDeshootEnableFlag,
    .SetDemo            = PQ_MNG_EnableDeshootDemo,
    .GetDemo            = PQ_MNG_GetDeshootDemo,
    .SetDemoMode        = PQ_MNG_SetDeshootDemoMode,
    .GetDemoMode        = PQ_MNG_GetDeshootDemoMode,
    .SetDeShootThr      = PQ_MNG_SetDeshootThr,
};

HI_S32 PQ_MNG_RegisterDS(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DS, enType, PQ_BIN_ADAPT_SINGLE, "ds", HI_NULL, &stDSFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDS(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DS);

    return s32Ret;
}


