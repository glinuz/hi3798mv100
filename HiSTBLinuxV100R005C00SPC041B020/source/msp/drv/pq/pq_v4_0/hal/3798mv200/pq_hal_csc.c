/******************************************************************************
*
* Copyright (C) 2014-2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_hal_csc.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/10/15
  Description   :

******************************************************************************/

#include "pq_hal_csc.h"
#include "pq_hal_comm.h"

typedef enum hiPQ_VDP_LAYER_WBC_E
{
    PQ_VDP_LAYER_WBC_HD0        = 0,
    PQ_VDP_LAYER_WBC_GP0        = 1,
    PQ_VDP_LAYER_WBC_G0         = 2,
    PQ_VDP_LAYER_WBC_VP0        = 3,
    PQ_VDP_LAYER_WBC_G4         = 3,
    PQ_VDP_LAYER_WBC_ME         = 5,
    PQ_VDP_LAYER_WBC_FI         = 6,
    PQ_VDP_LAYER_WBC_BMP        = 7,
    PQ_VDP_LAYER_HC_BMP         = 8,
    PQ_VDP_LAYER_WBC_TNR_REF    = 9,
    PQ_VDP_LAYER_WBC_TNR_MAD    = 10,
    PQ_VDP_LAYER_WBC_TNR_STT3   = 11,
    PQ_VDP_LAYER_WBC_DBM_STT1   = 12,
    PQ_VDP_LAYER_WBC_DBM_STT2   = 13,

    PQ_VDP_LAYER_WBC_BUTT

} PQ_VDP_LAYER_WBC_E;

HI_U32 PQ_HAL_GetWbcPointSel(HI_VOID)
{
    S_VDP_REGS_TYPE* pstVDPReg = HI_NULL;
    U_WBC_DHD0_CTRL WBC_DHD0_CTRL;

    HI_U32 u32Sel = WBC_POINT_AFTER_CSC; /* 00：回写点在VP内的CSC后，数据格式为YUV444 */

    pstVDPReg = PQ_HAL_GetVdpReg();

    WBC_DHD0_CTRL.u32 = PQ_HAL_RegRead((HI_U32*)((unsigned long)(&(pstVDPReg->WBC_DHD0_CTRL.u32)) + (PQ_VDP_LAYER_WBC_VP0 - PQ_VDP_LAYER_WBC_HD0) * PQ_WBC_OFFSET));
    u32Sel = WBC_DHD0_CTRL.bits.wbc_point_sel;

    return u32Sel;
}


