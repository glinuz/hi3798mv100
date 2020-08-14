/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dering.c
  Version       : Initial Draft
  Author        : l00216738
  Created       : 2015/09/01
  Description   :

******************************************************************************/

#include "drv_pq_table.h"
#include "pq_hal_comm.h"
#include "pq_mng_dering.h"


static PQ_DEMO_MODE_E sg_enMode = PQ_DEMO_ENABLE_R;
static HI_BOOL sg_bDeringDemoEn = HI_FALSE;
static HI_BOOL sg_bDeringEnFlag = HI_TRUE;

static HI_VOID PQ_MNG_DeringParaInit(HI_VOID)
{
    return;
}

HI_S32 PQ_MNG_InitDering(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;

    PQ_MNG_DeringParaInit();

    s32Ret = PQ_TABLE_InitPhyList(u32ID, HI_PQ_MODULE_DR, SOURCE_MODE_SD, OUTPUT_MODE_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Dering InitPhyList error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitDering(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDeringDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = 0;

    sg_bDeringDemoEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDeringDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDeringDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDeringDemo(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDeringDemoEn;
    return HI_SUCCESS;
}


/**
 \brief :  set dering demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */
HI_S32 PQ_MNG_SetDeringDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_enMode = enMode;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetDeringDemoMode(u32HandleNo, (DR_DEMO_MODE_E)enMode);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetDeringDemoMode(u32Vid, (DR_DEMO_MODE_E)enMode);
    }

    return s32Ret;
}

/**
 \brief :  get dering demo mode ;

 \param[in] enMode

 \retval ::HI_SUCCESS
 */

HI_S32 PQ_MNG_GetDeringDemoMode(PQ_DEMO_MODE_E* penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDering(HI_BOOL bOnOff)
{
    HI_U32  u32HandleNo = 0, u32Vid = PQ_VDP_LAYER_VID0;
    HI_S32  s32Ret = HI_SUCCESS;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DR, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    sg_bDeringEnFlag = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDering(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDering(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDeringEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDeringEnFlag;
    return HI_SUCCESS;
}

static stPQAlgFuncs stDRFuncs =
{
    .Init               = PQ_MNG_InitDering,
    .DeInit             = PQ_MNG_DeInitDering,
    .SetEnable          = PQ_MNG_EnableDering,
    .GetEnable          = PQ_MNG_GetDeringEnableFlag,
    .SetDemo            = PQ_MNG_EnableDeringDemo,
    .GetDemo            = PQ_MNG_GetDeringDemo,
    .SetDemoMode        = PQ_MNG_SetDeringDemoMode,
    .GetDemoMode        = PQ_MNG_GetDeringDemoMode,
};

HI_S32 PQ_MNG_RegisterDR(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DR, enType, PQ_BIN_ADAPT_SINGLE, "dr", HI_NULL, &stDRFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDR(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DR);

    return s32Ret;
}

