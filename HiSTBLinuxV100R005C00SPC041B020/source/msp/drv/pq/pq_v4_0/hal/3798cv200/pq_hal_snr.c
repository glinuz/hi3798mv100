/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_snr.c
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2015/10/19
 Description   :
******************************************************************************/
#include <linux/string.h>

#include "hi_type.h"
#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_hal_snr.h"
#include "pq_hal_comm.h"

static HI_S32 sg_s32MidStrength = 16;

static HI_U8 CbCrLUT[16][16] =
{
    //0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 0
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 1
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 2
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 3
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 4
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 5
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 24, 16, 16, 16, 16, 16, 16}, // 6
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 31, 16, 16, 16, 16, 16, 16}, // 7
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 24, 16, 16, 16, 16, 16, 16}, // 8
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 9
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 10
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 11
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 12
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 13
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // 14
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}  // 15
};

static HI_U8 MotionEdgeLUT[16][32] =
{
    //    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
    //{31, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  8,  7,  7,  6,  6,  6,  6}, // 0
    {31, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 14, 13, 12, 11, 10, 10,  9,  8,  7,  6,  5,  4,  3}, // 0
    {30, 29, 27, 24, 22, 19, 16, 13, 11,  8,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 1
    {26, 24, 22, 20, 17, 14, 11,  9,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 2
    {22, 21, 19, 17, 14, 12,  9,  7,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 3
    {19, 18, 16, 13, 11,  9,  7,  5,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 4
    {16, 14, 11,  9,  8,  6,  4,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 5
    {19, 18, 16, 12,  9,  7,  6,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 6
    {23, 21, 19, 16, 12,  9,  8,  6,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 7
    {26, 24, 22, 19, 16, 12, 10,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 8
    {28, 27, 26, 23, 19, 16, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 9
    {30, 30, 28, 26, 23, 20, 17, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 10
    {30, 30, 30, 29, 25, 22, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 11
    {31, 31, 30, 30, 27, 24, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 12
    {31, 31, 31, 30, 29, 27, 23, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 13
    {31, 31, 31, 31, 30, 29, 28, 26, 24, 23, 22, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 14
    {31, 31, 31, 31, 30, 30, 29, 29, 28, 28, 27, 26, 25, 24, 22, 20, 18, 15, 12,  9,  7,  5,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0}  // 15
};

#if 0
static HI_U8    MotionEdgeLUT4UHD[16][32] =
{
    //    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
    // {31, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  9,  8,  8,  7,  7,  6,  6,  6,  6}, // 0, 06/30
    // {31, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 11, 11, 11, 10, 10, 10,  9,  9,  9,  8,  8,  8}, // 0, 07/01
    // {30, 29, 27, 25, 23, 22, 20, 19, 17, 16, 13, 11,  10,  9,  8,  7,  7,  6,  6,  5,  5,  5,  5,  4,  4,  4,  3,  3,  3,  3,  3,  3}, // 0, 0701, details are seen, sunset blocky, stronger
    {30, 29, 27, 24, 22, 19, 16, 13, 11,  8,  6,  3,  3,  3,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1}, // 0, 0703
    // {31, 31, 30, 29, 28, 26, 24, 22, 22, 20, 18, 16, 14, 14, 14, 14, 14, 13, 13, 11, 9, 7,  5,  4,  3,  3,  2,  2,  1,  1,  1,  1}, // 0, based on 06/30
    // {31, 31, 30, 29, 28, 26, 24, 22, 22, 20, 18, 16, 14, 13, 12, 10, 10,  9,  8,  7,  6, 5,  4,  3,  3,  2,  2,  1,  1,  0,  0,  0}, // 0, based on 06/30
    //{31, 31, 30, 29, 28, 26, 24, 22, 20, 19, 17, 15, 14, 12, 11, 10,  9,  8,  7,  6,  5, 4,  3,  2,  2,  1,  1,  0,  0,  0,  0,  0}, // 0, based on 06/30 -> 07/03
    {30, 29, 27, 24, 22, 19, 16, 13, 11,  8,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 1
    {26, 24, 22, 20, 17, 14, 11,  9,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 2
    {22, 21, 19, 17, 14, 12,  9,  7,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 3
    {19, 18, 16, 13, 11,  9,  7,  5,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 4
    {16, 14, 11,  9,  8,  6,  4,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 5
    {19, 18, 16, 12,  9,  7,  6,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 6
    {23, 21, 19, 16, 12,  9,  8,  6,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 7
    {26, 24, 22, 19, 16, 12, 10,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 8
    {28, 27, 26, 23, 19, 16, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 9
    {30, 30, 28, 26, 23, 20, 17, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 10
    {30, 30, 30, 29, 25, 22, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 11
    {31, 31, 30, 30, 27, 24, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 12
    {31, 31, 31, 30, 29, 27, 23, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 13
    {31, 31, 31, 31, 30, 29, 28, 26, 24, 23, 22, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 14
    {31, 31, 31, 31, 30, 30, 29, 29, 28, 28, 27, 26, 25, 24, 22, 20, 18, 15, 12,  9,  7,  5,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0}  // 15
};

static HI_U8 MotionEdgeLUT4SD[16][32] =
{
    //    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
    {31, 27, 23, 20, 18, 16, 14, 13, 12, 10,  8,  7,  5,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 0
    {30, 25, 21, 19, 16, 14, 13, 12, 10,  8,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 1
    {26, 24, 22, 20, 17, 14, 11,  9,  6,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 2
    {22, 21, 19, 17, 14, 12,  9,  7,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 3
    {19, 18, 16, 13, 11,  9,  7,  5,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 4
    {16, 14, 11,  9,  8,  6,  4,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 5
    {19, 18, 16, 12,  9,  7,  6,  4,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 6
    {23, 21, 19, 16, 12,  9,  8,  6,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 7
    {26, 24, 22, 19, 16, 12, 10,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 8
    {28, 27, 26, 23, 19, 16, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 9
    {30, 30, 28, 26, 23, 20, 17, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 10
    {30, 30, 30, 29, 25, 22, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 11
    {31, 31, 30, 30, 27, 24, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 12
    {31, 31, 31, 30, 29, 27, 23, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 13
    {31, 31, 31, 31, 30, 29, 28, 26, 24, 23, 22, 21, 19, 17, 15, 14, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // 14
    {31, 31, 31, 31, 30, 30, 29, 29, 28, 28, 27, 26, 25, 24, 22, 20, 18, 15, 12,  9,  7,  5,  3,  2,  1,  0,  0,  0,  0,  0,  0,  0}  // 15
};
#endif

/* Enable or Disable Snr Module */
HI_S32 PQ_HAL_EnableSNR(HI_U32 u32HandleNo, HI_BOOL bSNROnOff)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_ENABLE.bits.lumasnren   = bSNROnOff;
        pstVirReg->VPSS_SNR_ENABLE.bits.chromasnren = bSNROnOff;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.lumasnren   = bSNROnOff;
        g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.chromasnren = bSNROnOff;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableSNRDemo(HI_U32 u32HandleNo, HI_BOOL bSNRDemoEn)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {

        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_ENABLE.bits.demomodeen = bSNRDemoEn;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.demomodeen   = bSNRDemoEn;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSNRDemoMode(HI_U32 u32HandleNo, SNR_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_ENABLE.bits.demomodelr = !enMode;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.demomodelr = !enMode;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_DEMO.bits.demomodecoor = u32XPos;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_DEMO.bits.demomodecoor   = u32XPos;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrMidStr(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);

        sg_s32MidStrength = pstVirReg->VPSS_SNR_ENABLE.bits.snrstrength;
    }
    else
    {
        sg_s32MidStrength = g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.snrstrength;
    }

    return HI_SUCCESS;
}


/* Snr Strengh[0,100] */
HI_S32 PQ_HAL_SetSnrStr(HI_U32 u32Data, HI_U32 u32SnrStr)
{
    HI_U32 u32Str = 0;
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if (enSourceMode == SOURCE_MODE_SD)
    {
        u32Str = u32SnrStr >> 1;
    }
    else if (enSourceMode == SOURCE_MODE_HD)
    {
        u32Str = u32SnrStr >> 2;
    }
    else
    {
        u32Str = u32SnrStr;
    }

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_ENABLE.bits.snrstrength = u32Str * 31 / 100;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_ENABLE.bits.snrstrength = u32Str * 31 / 100;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetNoNoiseGain(HI_U32 u32Data, HI_U32 u32NoNoiseGain)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    HI_U32 u32NoNoiseGainTmp   = 32;
    /* 0~9: Close; 10~40: Low; 40~70: Mid; 70~100: High
       Low:32; Mid:32; High:30 */
    if (u32NoNoiseGain < 10) /* Close */
    {
        u32NoNoiseGainTmp = 32; /* ? */
    }
    else if ((u32NoNoiseGain >= 10) && (u32NoNoiseGain < 40))  /* Low */
    {
        u32NoNoiseGainTmp = 32;
    }
    else if ((u32NoNoiseGain >= 40) && (u32NoNoiseGain < 70))  /* Mid */
    {
        u32NoNoiseGainTmp = 32;
    }
    else if ((u32NoNoiseGain >= 70) && (u32NoNoiseGain < 100)) /* High */
    {
        u32NoNoiseGainTmp = 30;
    }

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_NOISE_CORE.bits.nonoisegain      = u32NoNoiseGainTmp;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_NOISE_CORE.bits.nonoisegain = u32NoNoiseGainTmp;
    }

    return HI_SUCCESS;
}

HI_U32 PQ_HAL_SetNoiseCore(HI_U32 u32Data, HI_U32 u32NoiseCore)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    HI_S32 s32NoiseCoreTmp     = 2;
    /* 0~9: Close; 10~40: Low; 40~70: Mid; 70~100: High
       Low:-4; Mid:2; High:4 */
    if (u32NoiseCore < 10) /* Close */
    {
        s32NoiseCoreTmp = -4; /* ? */
    }
    else if ((u32NoiseCore >= 10) && (u32NoiseCore < 40))  /* Low */
    {
        s32NoiseCoreTmp = -4;
    }
    else if ((u32NoiseCore >= 40) && (u32NoiseCore < 70))  /* Mid */
    {
        s32NoiseCoreTmp = 2;
    }
    else if ((u32NoiseCore >= 70) && (u32NoiseCore < 100)) /* High */
    {
        s32NoiseCoreTmp = 4;
    }

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32Data);
        PQ_CHECK_NULL_PTR(pstVirReg);
        pstVirReg->VPSS_SNR_NOISE_CORE.bits.noisecore      = s32NoiseCoreTmp;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_NOISE_CORE.bits.noisecore = s32NoiseCoreTmp;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSNRCbCrLut (HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    U_V0_COLOR_BASED_LUT_01a V0_COLOR_BASED_LUT_01a;
    U_COLOR_BASED_LUT_01a    COLOR_BASED_LUT_01a;
    HI_U32 i, j;
    HI_U32 *pu32Lut = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);

        pu32Lut = (HI_U32 *) & (pstVirReg->COLOR_BASED_LUT_01a.u32);

        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < 16;)
            {
                COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__0_ = CbCrLUT[i][j + 0];
                COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__1_ = CbCrLUT[i][j + 1];
                COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__2_ = CbCrLUT[i][j + 2];
                COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__3_ = CbCrLUT[i][j + 3];

                *(pu32Lut++) = COLOR_BASED_LUT_01a.u32;
                j += 4;
            }
        }
        return HI_SUCCESS;
    }

    pu32Lut = (HI_U32 *) & (g_pstIPSelVdpAlg->V0_COLOR_BASED_LUT_01a.u32);

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 16;)
        {
            V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__0_ = CbCrLUT[i][j + 0];
            V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__1_ = CbCrLUT[i][j + 1];
            V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__2_ = CbCrLUT[i][j + 2];
            V0_COLOR_BASED_LUT_01a.bits.snrcbcrlut_0__3_ = CbCrLUT[i][j + 3];

            *(pu32Lut++) = V0_COLOR_BASED_LUT_01a.u32;

            j += 4;
        }
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSNRMotionEdgeLut (HI_U32 u32HandleNo, HI_U32 u32Width)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    U_V0_MOTION_EDGE_LUT_01a V0_MOTION_EDGE_LUT_01a;
    U_MOTION_EDGE_LUT_01a    MOTION_EDGE_LUT_01a;
    HI_U32 i, j;
    HI_U8 *pu8MotionEdgeData = &MotionEdgeLUT[0][0];
    HI_U32 *pu32Lut = HI_NULL;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {

        pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVirReg);

        pu32Lut = (HI_U32 *) & (pstVirReg->MOTION_EDGE_LUT_01a.u32);

        for (i = 0; i < 16; i++)
        {
            for (j = 0; j < 32;)
            {
                MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__0_ = *(pu8MotionEdgeData + 32 * i + j + 0);
                MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__1_ = *(pu8MotionEdgeData + 32 * i + j + 1);
                MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__2_ = *(pu8MotionEdgeData + 32 * i + j + 2);
                MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__3_ = *(pu8MotionEdgeData + 32 * i + j + 3);

                *(pu32Lut++) = MOTION_EDGE_LUT_01a.u32;

                j += 4;
            }
        }

        return HI_SUCCESS;
    }

    pu32Lut = (HI_U32 *) & (g_pstIPSelVdpAlg->V0_MOTION_EDGE_LUT_01a.u32);

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 32;)
        {
            V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__0_ = *(pu8MotionEdgeData + 32 * i + j + 0);
            V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__1_ = *(pu8MotionEdgeData + 32 * i + j + 1);
            V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__2_ = *(pu8MotionEdgeData + 32 * i + j + 2);
            V0_MOTION_EDGE_LUT_01a.bits.snrmotionedgelut_0__3_ = *(pu8MotionEdgeData + 32 * i + j + 3);

            *(pu32Lut++) = V0_MOTION_EDGE_LUT_01a.u32;

            j += 4;
        }
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrMeanRatio(HI_U32 u32HandleNo, PQ_HAL_SNR_MEAN_RATIO_S *pstMeanRatio)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    PQ_CHECK_NULL_PTR(pstMeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox0 = pstMeanRatio->s32MappingX[0];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox1 = pstMeanRatio->s32MappingX[1];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox2 = pstMeanRatio->s32MappingX[2];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox3 = pstMeanRatio->s32MappingX[3];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox4 = pstMeanRatio->s32MappingX[4];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox5 = pstMeanRatio->s32MappingX[5];

        pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy0 = pstMeanRatio->s32MappingY[0];
        pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy1 = pstMeanRatio->s32MappingY[1];
        pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy2 = pstMeanRatio->s32MappingY[2];
        pstVpssReg->VPSS_SNR_EDGE_RATIOY1.bits.meanedgeratioy3 = pstMeanRatio->s32MappingY[3];
        pstVpssReg->VPSS_SNR_EDGE_RATIOY1.bits.meanedgeratioy4 = pstMeanRatio->s32MappingY[4];

        pstVpssReg->VPSS_SNR_EDGE_RATIOK0.bits.meanedgeratiok0 = pstMeanRatio->s32MappingK[0];
        pstVpssReg->VPSS_SNR_EDGE_RATIOK0.bits.meanedgeratiok1 = pstMeanRatio->s32MappingK[1];
        pstVpssReg->VPSS_SNR_EDGE_RATIOK1.bits.meanedgeratiok2 = pstMeanRatio->s32MappingK[2];
        pstVpssReg->VPSS_SNR_EDGE_RATIOK1.bits.meanedgeratiok3 = pstMeanRatio->s32MappingK[3];

        pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomax  = pstMeanRatio->s32MappingMax;
        pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomin  = pstMeanRatio->s32MappingMin;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox0 = pstMeanRatio->s32MappingX[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox1 = pstMeanRatio->s32MappingX[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox2 = pstMeanRatio->s32MappingX[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox3 = pstMeanRatio->s32MappingX[3];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox4 = pstMeanRatio->s32MappingX[4];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox5 = pstMeanRatio->s32MappingX[5];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy0 = pstMeanRatio->s32MappingY[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy1 = pstMeanRatio->s32MappingY[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy2 = pstMeanRatio->s32MappingY[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy3 = pstMeanRatio->s32MappingY[3];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy4 = pstMeanRatio->s32MappingY[4];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok0 = pstMeanRatio->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok1 = pstMeanRatio->s32MappingK[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok2 = pstMeanRatio->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok3 = pstMeanRatio->s32MappingK[3];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIO.bits.meanedgeratiomax  = pstMeanRatio->s32MappingMax;
        g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIO.bits.meanedgeratiomin  = pstMeanRatio->s32MappingMin;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetSnrMeanRatio(HI_U32 u32HandleNo, PQ_HAL_SNR_MEAN_RATIO_S *pstMeanRatio)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    PQ_CHECK_NULL_PTR(pstMeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstMeanRatio->s32MappingX[0] = pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox0;
        pstMeanRatio->s32MappingX[1] = pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox1;
        pstMeanRatio->s32MappingX[2] = pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox2;
        pstMeanRatio->s32MappingX[3] = pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox3;
        pstMeanRatio->s32MappingX[4] = pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox4;
        pstMeanRatio->s32MappingX[5] = pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox5;

        pstMeanRatio->s32MappingY[0] = pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy0;
        pstMeanRatio->s32MappingY[1] = pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy1;
        pstMeanRatio->s32MappingY[2] = pstVpssReg->VPSS_SNR_EDGE_RATIOY0.bits.meanedgeratioy2;
        pstMeanRatio->s32MappingY[3] = pstVpssReg->VPSS_SNR_EDGE_RATIOY1.bits.meanedgeratioy3;
        pstMeanRatio->s32MappingY[4] = pstVpssReg->VPSS_SNR_EDGE_RATIOY1.bits.meanedgeratioy4;

        pstMeanRatio->s32MappingK[0] = pstVpssReg->VPSS_SNR_EDGE_RATIOK0.bits.meanedgeratiok0;
        pstMeanRatio->s32MappingK[1] = pstVpssReg->VPSS_SNR_EDGE_RATIOK0.bits.meanedgeratiok1;
        pstMeanRatio->s32MappingK[2] = pstVpssReg->VPSS_SNR_EDGE_RATIOK1.bits.meanedgeratiok2;
        pstMeanRatio->s32MappingK[3] = pstVpssReg->VPSS_SNR_EDGE_RATIOK1.bits.meanedgeratiok3;

        pstMeanRatio->s32MappingMax  = pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomax;
        pstMeanRatio->s32MappingMin  = pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomin;
    }
    else
    {
        pstMeanRatio->s32MappingX[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox0;
        pstMeanRatio->s32MappingX[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox1;
        pstMeanRatio->s32MappingX[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX0.bits.meanedgeratiox2;
        pstMeanRatio->s32MappingX[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox3;
        pstMeanRatio->s32MappingX[4] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox4;
        pstMeanRatio->s32MappingX[5] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOX1.bits.meanedgeratiox5;

        pstMeanRatio->s32MappingY[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy0;
        pstMeanRatio->s32MappingY[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy1;
        pstMeanRatio->s32MappingY[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY0.bits.meanedgeratioy2;
        pstMeanRatio->s32MappingY[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy3;
        pstMeanRatio->s32MappingY[4] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOY1.bits.meanedgeratioy4;

        pstMeanRatio->s32MappingK[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok0;
        pstMeanRatio->s32MappingK[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK0.bits.meanedgeratiok1;
        pstMeanRatio->s32MappingK[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok2;
        pstMeanRatio->s32MappingK[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIOK1.bits.meanedgeratiok3;

        pstMeanRatio->s32MappingMax  = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIO.bits.meanedgeratiomax;
        pstMeanRatio->s32MappingMin  = g_pstIPSelVdpAlg->V0_SNR_EDGE_RATIO.bits.meanedgeratiomin;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrEdgeStr(HI_U32 u32HandleNo, PQ_HAL_SNR_EDGE_STR_S *pstEdgeStr)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    PQ_CHECK_NULL_PTR(pstEdgeStr);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx0 = pstEdgeStr->s32MappingX[0];
        pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx1 = pstEdgeStr->s32MappingX[1];
        pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx2 = pstEdgeStr->s32MappingX[2];
        pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx3 = pstEdgeStr->s32MappingX[3];
        pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx4 = pstEdgeStr->s32MappingX[4];
        pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx5 = pstEdgeStr->s32MappingX[5];
        pstVpssReg->VPSS_SNR_EDGE_STRX2.bits.edgestrmappingx6 = pstEdgeStr->s32MappingX[6];
        pstVpssReg->VPSS_SNR_EDGE_STRX2.bits.edgestrmappingx7 = pstEdgeStr->s32MappingX[7];

        pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy0 = pstEdgeStr->s32MappingY[0];
        pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy1 = pstEdgeStr->s32MappingY[1];
        pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy2 = pstEdgeStr->s32MappingY[2];
        pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy3 = pstEdgeStr->s32MappingY[3];
        pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy4 = pstEdgeStr->s32MappingY[4];
        pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy5 = pstEdgeStr->s32MappingY[5];
        pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy6 = pstEdgeStr->s32MappingY[6];

        pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk0 = pstEdgeStr->s32MappingK[0];
        pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk1 = pstEdgeStr->s32MappingK[1];
        pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk2 = pstEdgeStr->s32MappingK[2];
        pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk3 = pstEdgeStr->s32MappingK[3];
        pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk4 = pstEdgeStr->s32MappingK[4];
        pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk5 = pstEdgeStr->s32MappingK[5];

        pstVpssReg->VPSS_SNR_EDGE_STR.bits.edgestrmappingmax  = pstEdgeStr->s32MappingMax;
        pstVpssReg->VPSS_SNR_EDGE_STR.bits.edgestrmappingmin  = pstEdgeStr->s32MappingMin;
    }
    else
    {
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx0 = pstEdgeStr->s32MappingX[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx1 = pstEdgeStr->s32MappingX[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx2 = pstEdgeStr->s32MappingX[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx3 = pstEdgeStr->s32MappingX[3];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx4 = pstEdgeStr->s32MappingX[4];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx5 = pstEdgeStr->s32MappingX[5];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX2.bits.edgestrmappingx6 = pstEdgeStr->s32MappingX[6];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX2.bits.edgestrmappingx7 = pstEdgeStr->s32MappingX[7];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy0 = pstEdgeStr->s32MappingY[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy1 = pstEdgeStr->s32MappingY[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy2 = pstEdgeStr->s32MappingY[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy3 = pstEdgeStr->s32MappingY[3];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy4 = pstEdgeStr->s32MappingY[4];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy5 = pstEdgeStr->s32MappingY[5];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy6 = pstEdgeStr->s32MappingY[6];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk0 = pstEdgeStr->s32MappingK[0];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk1 = pstEdgeStr->s32MappingK[1];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk2 = pstEdgeStr->s32MappingK[2];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk3 = pstEdgeStr->s32MappingK[3];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk4 = pstEdgeStr->s32MappingK[4];
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk5 = pstEdgeStr->s32MappingK[5];

        g_pstIPSelVdpAlg->V0_SNR_EDGE_STR.bits.edgestrmappingmax  = pstEdgeStr->s32MappingMax;
        g_pstIPSelVdpAlg->V0_SNR_EDGE_STR.bits.edgestrmappingmin  = pstEdgeStr->s32MappingMin;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetSnrEdgeStr(HI_U32 u32HandleNo, PQ_HAL_SNR_EDGE_STR_S *pstEdgeStr)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    PQ_CHECK_NULL_PTR(pstEdgeStr);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_SNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstEdgeStr->s32MappingX[0] = pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx0;
        pstEdgeStr->s32MappingX[1] = pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx1;
        pstEdgeStr->s32MappingX[2] = pstVpssReg->VPSS_SNR_EDGE_STRX0.bits.edgestrmappingx2;
        pstEdgeStr->s32MappingX[3] = pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx3;
        pstEdgeStr->s32MappingX[4] = pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx4;
        pstEdgeStr->s32MappingX[5] = pstVpssReg->VPSS_SNR_EDGE_STRX1.bits.edgestrmappingx5;
        pstEdgeStr->s32MappingX[6] = pstVpssReg->VPSS_SNR_EDGE_STRX2.bits.edgestrmappingx6;
        pstEdgeStr->s32MappingX[7] = pstVpssReg->VPSS_SNR_EDGE_STRX2.bits.edgestrmappingx7;

        pstEdgeStr->s32MappingY[0] = pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy0;
        pstEdgeStr->s32MappingY[1] = pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy1;
        pstEdgeStr->s32MappingY[2] = pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy2;
        pstEdgeStr->s32MappingY[3] = pstVpssReg->VPSS_SNR_EDGE_STRY0.bits.edgestrmappingy3;
        pstEdgeStr->s32MappingY[4] = pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy4;
        pstEdgeStr->s32MappingY[5] = pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy5;
        pstEdgeStr->s32MappingY[6] = pstVpssReg->VPSS_SNR_EDGE_STRY1.bits.edgestrmappingy6;

        pstEdgeStr->s32MappingK[0] = pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk0;
        pstEdgeStr->s32MappingK[1] = pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk1;
        pstEdgeStr->s32MappingK[2] = pstVpssReg->VPSS_SNR_EDGE_STRK0.bits.edgestrmappingk2;
        pstEdgeStr->s32MappingK[3] = pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk3;
        pstEdgeStr->s32MappingK[4] = pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk4;
        pstEdgeStr->s32MappingK[5] = pstVpssReg->VPSS_SNR_EDGE_STRK1.bits.edgestrmappingk5;

        pstEdgeStr->s32MappingMax  = pstVpssReg->VPSS_SNR_EDGE_STR.bits.edgestrmappingmax;
        pstEdgeStr->s32MappingMin  = pstVpssReg->VPSS_SNR_EDGE_STR.bits.edgestrmappingmin;
    }
    else
    {
        pstEdgeStr->s32MappingX[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx0;
        pstEdgeStr->s32MappingX[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx1;
        pstEdgeStr->s32MappingX[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX0.bits.edgestrmappingx2;
        pstEdgeStr->s32MappingX[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx3;
        pstEdgeStr->s32MappingX[4] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx4;
        pstEdgeStr->s32MappingX[5] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX1.bits.edgestrmappingx5;
        pstEdgeStr->s32MappingX[6] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX2.bits.edgestrmappingx6;
        pstEdgeStr->s32MappingX[7] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRX2.bits.edgestrmappingx7;

        pstEdgeStr->s32MappingY[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy0;
        pstEdgeStr->s32MappingY[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy1;
        pstEdgeStr->s32MappingY[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy2;
        pstEdgeStr->s32MappingY[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY0.bits.edgestrmappingy3;
        pstEdgeStr->s32MappingY[4] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy4;
        pstEdgeStr->s32MappingY[5] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy5;
        pstEdgeStr->s32MappingY[6] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRY1.bits.edgestrmappingy6;

        pstEdgeStr->s32MappingK[0] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk0;
        pstEdgeStr->s32MappingK[1] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk1;
        pstEdgeStr->s32MappingK[2] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK0.bits.edgestrmappingk2;
        pstEdgeStr->s32MappingK[3] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk3;
        pstEdgeStr->s32MappingK[4] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk4;
        pstEdgeStr->s32MappingK[5] = g_pstIPSelVdpAlg->V0_SNR_EDGE_STRK1.bits.edgestrmappingk5;

        pstEdgeStr->s32MappingMax  = g_pstIPSelVdpAlg->V0_SNR_EDGE_STR.bits.edgestrmappingmax;
        pstEdgeStr->s32MappingMin  = g_pstIPSelVdpAlg->V0_SNR_EDGE_STR.bits.edgestrmappingmin;
    }

    return HI_SUCCESS;
}

#if 0
HI_S32 PQ_HAL_SetSnrMeanEdgeMapping(HI_U32 u32HandleNo, TNR_MAPPING_S *pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;

    U_VPSS_SNR_EDGE_RATIOX0 VPSS_SNR_EDGE_RATIOX0;
    U_VPSS_SNR_EDGE_RATIOX1 VPSS_SNR_EDGE_RATIOX1;
    U_VPSS_SNR_EDGE_RATIOY0 VPSS_SNR_EDGE_RATIOY0;
    U_VPSS_SNR_EDGE_RATIOY1 VPSS_SNR_EDGE_RATIOY1;
    U_VPSS_SNR_EDGE_RATIOK0 VPSS_SNR_EDGE_RATIOK0;
    U_VPSS_SNR_EDGE_RATIOK1 VPSS_SNR_EDGE_RATIOK1;
    U_VPSS_SNR_EDGE_RATIO   VPSS_SNR_EDGE_RATIO;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomin = pstTNR_FnlMotionCMapping->s32MappingMin;
        pstVpssReg->VPSS_SNR_EDGE_RATIO.bits.meanedgeratiomax = pstTNR_FnlMotionCMapping->s32MappingMax;

        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox0 = pstTNR_FnlMotionCMapping->s32MappingR[0];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox1 = pstTNR_FnlMotionCMapping->s32MappingR[1];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX0.bits.meanedgeratiox2 = pstTNR_FnlMotionCMapping->s32MappingR[2];

        pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox3 = pstTNR_FnlMotionCMapping->s32MappingR[3];
        pstVpssReg->VPSS_SNR_EDGE_RATIOX1.bits.meanedgeratiox4 = pstTNR_FnlMotionCMapping->s32MappingR[4];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = pstTNR_FnlMotionCMapping->s32MappingT[0];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = pstTNR_FnlMotionCMapping->s32MappingT[1];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = pstTNR_FnlMotionCMapping->s32MappingT[2];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = pstTNR_FnlMotionCMapping->s32MappingT[3];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = pstTNR_FnlMotionCMapping->s32MappingT[4];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = pstTNR_FnlMotionCMapping->s32MappingT[5];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = pstTNR_FnlMotionCMapping->s32MappingK[0];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = pstTNR_FnlMotionCMapping->s32MappingK[1];

        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = pstTNR_FnlMotionCMapping->s32MappingK[2];
        pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = pstTNR_FnlMotionCMapping->s32MappingK[3];
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();

        V0_TNR_TFM_C_MAPPING_CTRL.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_CTRL.u32));
        V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min = pstTNR_FnlMotionCMapping->s32MappingMin;
        V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max = pstTNR_FnlMotionCMapping->s32MappingMax;
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_CTRL.u32)), V0_TNR_TFM_C_MAPPING_CTRL.u32);

        V0_TNR_TFM_C_MAPPING_R0.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R0.u32));
        V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0 = pstTNR_FnlMotionCMapping->s32MappingR[0];
        V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1 = pstTNR_FnlMotionCMapping->s32MappingR[1];
        V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2 = pstTNR_FnlMotionCMapping->s32MappingR[2];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R0.u32)), V0_TNR_TFM_C_MAPPING_R0.u32);

        V0_TNR_TFM_C_MAPPING_R1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R1.u32));
        V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3 = pstTNR_FnlMotionCMapping->s32MappingR[3];
        V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4 = pstTNR_FnlMotionCMapping->s32MappingR[4];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R1.u32)), V0_TNR_TFM_C_MAPPING_R1.u32);

        V0_TNR_TFM_C_MAPPING_T1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T1.u32));
        V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1 = pstTNR_FnlMotionCMapping->s32MappingT[0];
        V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2 = pstTNR_FnlMotionCMapping->s32MappingT[1];
        V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3 = pstTNR_FnlMotionCMapping->s32MappingT[2];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T1.u32)), V0_TNR_TFM_C_MAPPING_T1.u32);

        V0_TNR_TFM_C_MAPPING_T2.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T2.u32));
        V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4 = pstTNR_FnlMotionCMapping->s32MappingT[3];
        V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5 = pstTNR_FnlMotionCMapping->s32MappingT[4];
        V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6 = pstTNR_FnlMotionCMapping->s32MappingT[5];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T2.u32)), V0_TNR_TFM_C_MAPPING_T2.u32);

        V0_TNR_TFM_C_MAPPING_K1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K1.u32));
        V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1 = pstTNR_FnlMotionCMapping->s32MappingK[0];
        V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2 = pstTNR_FnlMotionCMapping->s32MappingK[1];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K1.u32)), V0_TNR_TFM_C_MAPPING_K1.u32);

        V0_TNR_TFM_C_MAPPING_K2.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K2.u32));
        V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3 = pstTNR_FnlMotionCMapping->s32MappingK[2];
        V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4 = pstTNR_FnlMotionCMapping->s32MappingK[3];
        PQ_HAL_RegWrite((&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K2.u32)), V0_TNR_TFM_C_MAPPING_K2.u32);
    }
    return HI_SUCCESS;
}

/* 获取TNR的Final C Motion Mapping参数 */
HI_S32 PQ_HAL_GetTnrFnlMotionCMapping(HI_U32 u32HandleNo, TNR_MAPPING_S *pstTNR_FnlMotionCMapping)
{
    S_CAS_REGS_TYPE *pstVpssReg = HI_NULL;
    S_VDP_REGS_TYPE *pstVDPReg = HI_NULL;
    U_V0_TNR_TFM_C_MAPPING_CTRL V0_TNR_TFM_C_MAPPING_CTRL;
    U_V0_TNR_TFM_C_MAPPING_R0  V0_TNR_TFM_C_MAPPING_R0;
    U_V0_TNR_TFM_C_MAPPING_R1  V0_TNR_TFM_C_MAPPING_R1;
    U_V0_TNR_TFM_C_MAPPING_T1  V0_TNR_TFM_C_MAPPING_T1;
    U_V0_TNR_TFM_C_MAPPING_T2  V0_TNR_TFM_C_MAPPING_T2;
    U_V0_TNR_TFM_C_MAPPING_K1  V0_TNR_TFM_C_MAPPING_K1;
    U_V0_TNR_TFM_C_MAPPING_K2  V0_TNR_TFM_C_MAPPING_K2;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
        PQ_CHECK_NULL_PTR(pstVpssReg);

        pstTNR_FnlMotionCMapping->s32MappingMin = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min;
        pstTNR_FnlMotionCMapping->s32MappingMax = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max;

        pstTNR_FnlMotionCMapping->s32MappingR[0]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0;
        pstTNR_FnlMotionCMapping->s32MappingR[1]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1;
        pstTNR_FnlMotionCMapping->s32MappingR[2]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2;

        pstTNR_FnlMotionCMapping->s32MappingR[3]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3;
        pstTNR_FnlMotionCMapping->s32MappingR[4]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4;

        pstTNR_FnlMotionCMapping->s32MappingT[0]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1;
        pstTNR_FnlMotionCMapping->s32MappingT[1]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2;
        pstTNR_FnlMotionCMapping->s32MappingT[2]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3;

        pstTNR_FnlMotionCMapping->s32MappingT[3]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4;
        pstTNR_FnlMotionCMapping->s32MappingT[4]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5;
        pstTNR_FnlMotionCMapping->s32MappingT[5]  = pstVpssReg->VPSS_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6;

        pstTNR_FnlMotionCMapping->s32MappingK[0]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1);
        pstTNR_FnlMotionCMapping->s32MappingK[1]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2);

        pstTNR_FnlMotionCMapping->s32MappingK[2]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3);
        pstTNR_FnlMotionCMapping->s32MappingK[3]  = S10TOS32(pstVpssReg->VPSS_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4);
    }
    else
    {
        pstVDPReg = PQ_HAL_GetVdpReg();

        V0_TNR_TFM_C_MAPPING_CTRL.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_CTRL.u32));
        pstTNR_FnlMotionCMapping->s32MappingMin  = V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_min;
        pstTNR_FnlMotionCMapping->s32MappingMax  = V0_TNR_TFM_C_MAPPING_CTRL.bits.tfm_c_mapping_max;

        V0_TNR_TFM_C_MAPPING_R0.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R0.u32));
        pstTNR_FnlMotionCMapping->s32MappingR[0]    = V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r0;
        pstTNR_FnlMotionCMapping->s32MappingR[1]    = V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r1;
        pstTNR_FnlMotionCMapping->s32MappingR[2]    = V0_TNR_TFM_C_MAPPING_R0.bits.tfm_c_mapping_r2;

        V0_TNR_TFM_C_MAPPING_R1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_R1.u32));
        pstTNR_FnlMotionCMapping->s32MappingR[3]    = V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r3;
        pstTNR_FnlMotionCMapping->s32MappingR[4]    = V0_TNR_TFM_C_MAPPING_R1.bits.tfm_c_mapping_r4;

        V0_TNR_TFM_C_MAPPING_T1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T1.u32));
        pstTNR_FnlMotionCMapping->s32MappingT[0]    = V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t1;
        pstTNR_FnlMotionCMapping->s32MappingT[1]    = V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t2;
        pstTNR_FnlMotionCMapping->s32MappingT[2]    = V0_TNR_TFM_C_MAPPING_T1.bits.tfm_c_mapping_t3;

        V0_TNR_TFM_C_MAPPING_T2.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_T2.u32));
        pstTNR_FnlMotionCMapping->s32MappingT[3]    = V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t4;
        pstTNR_FnlMotionCMapping->s32MappingT[4]    = V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t5;
        pstTNR_FnlMotionCMapping->s32MappingT[5]    = V0_TNR_TFM_C_MAPPING_T2.bits.tfm_c_mapping_t6;

        V0_TNR_TFM_C_MAPPING_K1.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K1.u32));
        pstTNR_FnlMotionCMapping->s32MappingK[0]    = S10TOS32(V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k1);
        pstTNR_FnlMotionCMapping->s32MappingK[1]    = S10TOS32(V0_TNR_TFM_C_MAPPING_K1.bits.tfm_c_mapping_k2);

        V0_TNR_TFM_C_MAPPING_K2.u32 = PQ_HAL_RegRead(&(pstVDPReg->V0_TNR_TFM_C_MAPPING_K2.u32));
        pstTNR_FnlMotionCMapping->s32MappingK[2]    = S10TOS32(V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k3);
        pstTNR_FnlMotionCMapping->s32MappingK[3]    = S10TOS32(V0_TNR_TFM_C_MAPPING_K2.bits.tfm_c_mapping_k4);
    }

    return HI_SUCCESS;
}
#endif

