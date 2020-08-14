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


static HI_VOID PQ_MNG_DeshootParaInit(HI_VOID)
{
    return;
}

HI_S32 PQ_MNG_InitDeshoot(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;
    
    PQ_MNG_DeshootParaInit();
    /* Include Deshooot with ZME mixed And single DS */
    s32Ret = PQ_MNG_InitPhyList(u32ID, REG_TYPE_VDP, SOURCE_MODE_HD, OUTPUT_MODE_HD, PQ_BIN_MODULE_DS);
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


