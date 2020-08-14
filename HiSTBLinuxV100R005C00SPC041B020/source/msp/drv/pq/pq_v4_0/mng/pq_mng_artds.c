/******************************************************************************
*
* Copyright (C) 2015-2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_artds.c
  Version       : Initial Draft
  Author        : d00241380
  Created       : 2015/05/11
  Description   :

******************************************************************************/
#include "drv_pq_table.h"
#include "pq_hal_comm.h"

static HI_VOID PQ_MNG_ArtDSParaInit(HI_VOID)
{
    return;
}

HI_S32 PQ_MNG_InitArtDS(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;

    PQ_MNG_ArtDSParaInit();
    /*Include Deshooot with ZME mixed and sharpen shoot control*/
    s32Ret = PQ_TABLE_InitPhyList(u32ID, HI_PQ_MODULE_ARTDS, SOURCE_MODE_HD, OUTPUT_MODE_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Deshoot InitPhyList error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitArtDS(HI_VOID)
{
    return HI_SUCCESS;
}

static stPQAlgFuncs stArtDSFuncs =
{
    .Init               = PQ_MNG_InitArtDS,
    .DeInit             = PQ_MNG_DeInitArtDS,
};

HI_S32 PQ_MNG_RegisterArtDS(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_ARTDS, enType, PQ_BIN_ADAPT_SINGLE, "artds", HI_NULL, &stArtDSFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterArtDS(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_ARTDS);

    return s32Ret;
}

