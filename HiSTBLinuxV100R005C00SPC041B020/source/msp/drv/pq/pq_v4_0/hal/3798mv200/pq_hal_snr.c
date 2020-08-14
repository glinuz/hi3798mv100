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

#include "hi_type.h"
#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_hal_snr.h"
#include "pq_hal_comm.h"

#define SNR_MIN(x, y) (((x) > (y)) ? (y) : (x))
#define SNR_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SNR_CLIP3(low, high, x)  (SNR_MAX( SNR_MIN((x), SNR_MAX(high, low)), SNR_MIN(high, low)));

static HI_U32  sg_u32YSnrStr = 24;
static HI_U32  sg_u32CSnrStr = 24;

static HI_U8 SNRMotionEdgeLut[16][16] =
{
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6},
    { 0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8},
    { 0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9},
    { 0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10},
    { 0,  0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12},
    { 0,  0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13},
    { 0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14},
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15},
    { 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 15},
    { 2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 15, 15},
    { 3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 15, 15, 15},
    { 4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 15, 15, 15, 15},
    { 5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 15, 15, 15, 15, 15},
    { 6,  7,  8,  9, 10,  11, 12, 13, 14, 15, 15, 15, 15, 15, 15, 15}
};

HI_S32 PQ_HAL_EnableSNR(HI_U32 u32HandleNo, HI_BOOL bSNROnOff)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVirReg);
    pstVirReg->VPSS_SNR_ENABLE.bits.ynr2den = bSNROnOff;
    pstVirReg->VPSS_SNR_ENABLE.bits.cnr2den = bSNROnOff;

    return HI_SUCCESS;
}

static HI_S32 PQ_HAL_InitSNRRatioMap(PQ_HAL_SNR_PARA_S *psSNRPara)
{
    psSNRPara->edgemeank1 = 0;
    psSNRPara->edgemeank2 = 24;
    psSNRPara->edgemeank3 = 32;
    psSNRPara->edgemeank4 = 24;
    psSNRPara->edgemeank5 = 20;
    psSNRPara->edgemeank6 = 12;
    psSNRPara->edgemeank7 = 8;
    psSNRPara->edgemeank8 = 8;

    psSNRPara->edgemeanth1 = 64 / 16;
    psSNRPara->edgemeanth2 = 90 / 16;
    psSNRPara->edgemeanth3 = 128 / 16;
    psSNRPara->edgemeanth4 = 164 / 16;
    psSNRPara->edgemeanth5 = 196 / 16;
    psSNRPara->edgemeanth6 = 300 / 16;
    psSNRPara->edgemeanth7 = 512 / 16;
    psSNRPara->edgemeanth8 = 768 / 16;
    psSNRPara->edgestrk1 = 16;
    psSNRPara->edgestrk2 = 32;
    psSNRPara->edgestrk3 = 32;
    psSNRPara->edgestrth1 = 8;
    psSNRPara->edgestrth2 = 16;
    psSNRPara->edgestrth3 = 24;
    psSNRPara->edgeoristrength = 0;
    psSNRPara->edgeoriratio = 15;
    psSNRPara->edge1 = SNR_CLIP3(0, 15, psSNRPara->edgeoristrength + ((psSNRPara->edgestrk1 * psSNRPara->edgestrth1 + 16) >> 5));
    psSNRPara->edge2 = SNR_CLIP3(0, 15, ((psSNRPara->edgestrk2 * (psSNRPara->edgestrth2 - psSNRPara->edgestrth1) + 16) >> 5) + psSNRPara->edge1 );
    psSNRPara->edge3 = SNR_CLIP3(0, 15, ((psSNRPara->edgestrk3 * (psSNRPara->edgestrth3 - psSNRPara->edgestrth2) + 16) >> 5) + psSNRPara->edge2 );
    psSNRPara->ratio1 =  SNR_CLIP3(0, 31, psSNRPara->edgeoriratio * 2 - ((psSNRPara->edgemeank1 * psSNRPara->edgemeanth1 + 8) >> 4));
    psSNRPara->ratio2 = SNR_CLIP3(0, 31, psSNRPara->ratio1 - ((psSNRPara->edgemeank2 * (psSNRPara->edgemeanth2 - psSNRPara->edgemeanth1) + 8) >> 4));
    psSNRPara->ratio3 = SNR_CLIP3(0, 31, psSNRPara->ratio2 - ((psSNRPara->edgemeank3 * (psSNRPara->edgemeanth3 - psSNRPara->edgemeanth2) + 8) >> 4));
    psSNRPara->ratio4 = SNR_CLIP3(0, 31, psSNRPara->ratio3 - ((psSNRPara->edgemeank4 * (psSNRPara->edgemeanth4 - psSNRPara->edgemeanth3) + 8) >> 4));
    psSNRPara->ratio5 = SNR_CLIP3(0, 31, psSNRPara->ratio4 - ((psSNRPara->edgemeank5 * (psSNRPara->edgemeanth5 - psSNRPara->edgemeanth4) + 8) >> 4));
    psSNRPara->ratio6 = SNR_CLIP3(0, 31, psSNRPara->ratio5 - ((psSNRPara->edgemeank6 * (psSNRPara->edgemeanth6 - psSNRPara->edgemeanth5) + 8) >> 4));
    psSNRPara->ratio7 = SNR_CLIP3(0, 31, psSNRPara->ratio6 - ((psSNRPara->edgemeank7 * (psSNRPara->edgemeanth7 - psSNRPara->edgemeanth6) + 8) >> 4));
    psSNRPara->ratio8 = SNR_CLIP3(0, 31, psSNRPara->ratio7 - ((psSNRPara->edgemeank8 * (psSNRPara->edgemeanth8 - psSNRPara->edgemeanth7) + 8 ) >> 4));

    return HI_SUCCESS;
}
HI_S32 PQ_HAL_SetSNRRatioMap(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;
    PQ_HAL_SNR_PARA_S stSNRPara;

    PQ_HAL_InitSNRRatioMap(&stSNRPara);

    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVirReg);

    pstVirReg->VPSS_SNR_EDGE_MEANK1.bits.edgemeank1 = stSNRPara.edgemeank1;
    pstVirReg->VPSS_SNR_EDGE_MEANK1.bits.edgemeank2 = stSNRPara.edgemeank2;
    pstVirReg->VPSS_SNR_EDGE_MEANK1.bits.edgemeank3 = stSNRPara.edgemeank3;
    pstVirReg->VPSS_SNR_EDGE_MEANK1.bits.edgemeank4 = stSNRPara.edgemeank4;
    pstVirReg->VPSS_SNR_EDGE_MEANK2.bits.edgemeank5 = stSNRPara.edgemeank5;
    pstVirReg->VPSS_SNR_EDGE_MEANK2.bits.edgemeank6 = stSNRPara.edgemeank6;
    pstVirReg->VPSS_SNR_EDGE_MEANK2.bits.edgemeank7 = stSNRPara.edgemeank7;
    pstVirReg->VPSS_SNR_EDGE_MEANK2.bits.edgemeank8 = stSNRPara.edgemeank8;

    pstVirReg->VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth1 = stSNRPara.edgemeanth1;
    pstVirReg->VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth2 = stSNRPara.edgemeanth2;
    pstVirReg->VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth3 = stSNRPara.edgemeanth3;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth4 = stSNRPara.edgemeanth4;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth5 = stSNRPara.edgemeanth5;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth6 = stSNRPara.edgemeanth6;
    pstVirReg->VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth7 = stSNRPara.edgemeanth7;
    pstVirReg->VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth8 = stSNRPara.edgemeanth8;

    pstVirReg->VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth2 = stSNRPara.edgemeanth2;
    pstVirReg->VPSS_SNR_EDGE_MEANTH1.bits.edgemeanth3 = stSNRPara.edgemeanth3;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth4 = stSNRPara.edgemeanth4;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth5 = stSNRPara.edgemeanth5;
    pstVirReg->VPSS_SNR_EDGE_MEANTH2.bits.edgemeanth6 = stSNRPara.edgemeanth6;
    pstVirReg->VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth7 = stSNRPara.edgemeanth7;
    pstVirReg->VPSS_SNR_EDGE_MEANTH3.bits.edgemeanth8 = stSNRPara.edgemeanth8;

    pstVirReg->VPSS_SNR_EDGE_RATIORANGE.bits.edgeoriratio = stSNRPara.edgeoriratio;
    pstVirReg->VPSS_SNR_EDGE_RATIO_1.bits.ratio1 = stSNRPara.ratio1;
    pstVirReg->VPSS_SNR_EDGE_RATIO_1.bits.ratio2 = stSNRPara.ratio2;
    pstVirReg->VPSS_SNR_EDGE_RATIO_1.bits.ratio3 = stSNRPara.ratio3;
    pstVirReg->VPSS_SNR_EDGE_RATIO_2.bits.ratio4 = stSNRPara.ratio4;
    pstVirReg->VPSS_SNR_EDGE_RATIO_2.bits.ratio5 = stSNRPara.ratio5;
    pstVirReg->VPSS_SNR_EDGE_RATIO_2.bits.ratio6 = stSNRPara.ratio6;
    pstVirReg->VPSS_SNR_EDGE_RATIO_3.bits.ratio7 = stSNRPara.ratio7;
    pstVirReg->VPSS_SNR_EDGE_RATIO_3.bits.ratio8 = stSNRPara.ratio8;
    pstVirReg->VPSS_SNR_EDGE_STRK.bits.edgestrk1 = stSNRPara.edgestrk1;
    pstVirReg->VPSS_SNR_EDGE_STRK.bits.edgestrk2 = stSNRPara.edgestrk2;
    pstVirReg->VPSS_SNR_EDGE_STRK.bits.edgestrk3 = stSNRPara.edgestrk3;
    pstVirReg->VPSS_SNR_EDGE_STRRANGE.bits.edgeoristrength = stSNRPara.edgeoristrength;
    pstVirReg->VPSS_SNR_EDGE_STR.bits.edge1 = stSNRPara.edge1;
    pstVirReg->VPSS_SNR_EDGE_STR.bits.edge2 = stSNRPara.edge2;
    pstVirReg->VPSS_SNR_EDGE_STR.bits.edge3 = stSNRPara.edge3;
    pstVirReg->VPSS_SNR_EDGE_STRTH.bits.edgestrth1 = stSNRPara.edgestrth1;
    pstVirReg->VPSS_SNR_EDGE_STRTH.bits.edgestrth2 = stSNRPara.edgestrth2;
    pstVirReg->VPSS_SNR_EDGE_STRTH.bits.edgestrth3 = stSNRPara.edgestrth3;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_EnableSNRDemo(HI_U32 u32HandleNo, HI_BOOL bSNRDemoEn)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSNRDemoMode(HI_U32 u32HandleNo, SNR_DEMO_MODE_E enMode)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrMidStr(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVirReg);

    sg_u32YSnrStr = pstVirReg->VPSS_SNR_STR.bits.ysnrstr;
    sg_u32CSnrStr = pstVirReg->VPSS_SNR_STR.bits.csnrstr;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSnrStr(HI_U32 u32HandleNo, HI_U32 u32SnrStr)
{
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVirReg);

    pstVirReg->VPSS_SNR_STR.bits.ysnrstr = (u32SnrStr * sg_u32YSnrStr * 2) / 100;
    pstVirReg->VPSS_SNR_STR.bits.csnrstr = (u32SnrStr * sg_u32YSnrStr * 2) / 100;
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetSNRCbCrLut (HI_U32 u32HandleNo)
{
    return HI_SUCCESS;
}

static HI_VOID pq_mng_ConvertSNRMotionEdgeLut(HI_U8 *pMotionEdgeLut, HI_U32 *pConvertTable)
{
    HI_S32 i = 0;
    HI_S32 j = 0;
    HI_S32 k = 0;

    for (i = 0; i < 16; i++)
    {
        for (j = 0; j < 8; j++)
        {
            pConvertTable[k] = pConvertTable[k] << 4;
            pConvertTable[k] &= ~0x0000000F;
            pConvertTable[k] |= pMotionEdgeLut[(7 - j)  + i * 16] & 0xF;
        }

        k++;

        for (j = 8; j < 16; j++)
        {
            pConvertTable[k] = pConvertTable[k] << 4;
            pConvertTable[k] &= ~0x0000000F;
            pConvertTable[k] |= pMotionEdgeLut[(23 - j)  + i * 16] & 0xF;
        }

        k++;
    }

    return ;
}

HI_S32 PQ_HAL_SetSNRMotionEdgeLut (HI_U32 u32HandleNo, HI_U32 u32Width)
{
    HI_U32 au32MotionEdgeLUT[32] = {0};
    S_CAS_REGS_TYPE *pstVirReg = HI_NULL;

    pstVirReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVirReg);

    pq_mng_ConvertSNRMotionEdgeLut((HI_U8 *)SNRMotionEdgeLut, au32MotionEdgeLUT);


    pstVirReg->MOTION_EDGE_LUT_01.u32 = au32MotionEdgeLUT[0];
    pstVirReg->MOTION_EDGE_LUT_02.u32 = au32MotionEdgeLUT[1];
    pstVirReg->MOTION_EDGE_LUT_03.u32 = au32MotionEdgeLUT[2];
    pstVirReg->MOTION_EDGE_LUT_04.u32 = au32MotionEdgeLUT[3];
    pstVirReg->MOTION_EDGE_LUT_05.u32 = au32MotionEdgeLUT[4];
    pstVirReg->MOTION_EDGE_LUT_06.u32 = au32MotionEdgeLUT[5];
    pstVirReg->MOTION_EDGE_LUT_07.u32 = au32MotionEdgeLUT[6];
    pstVirReg->MOTION_EDGE_LUT_08.u32 = au32MotionEdgeLUT[7];
    pstVirReg->MOTION_EDGE_LUT_09.u32 = au32MotionEdgeLUT[8];
    pstVirReg->MOTION_EDGE_LUT_10.u32 = au32MotionEdgeLUT[9];
    pstVirReg->MOTION_EDGE_LUT_11.u32 = au32MotionEdgeLUT[10];
    pstVirReg->MOTION_EDGE_LUT_12.u32 = au32MotionEdgeLUT[11];
    pstVirReg->MOTION_EDGE_LUT_13.u32 = au32MotionEdgeLUT[12];
    pstVirReg->MOTION_EDGE_LUT_14.u32 = au32MotionEdgeLUT[13];
    pstVirReg->MOTION_EDGE_LUT_15.u32 = au32MotionEdgeLUT[14];
    pstVirReg->MOTION_EDGE_LUT_16.u32 = au32MotionEdgeLUT[15];
    pstVirReg->MOTION_EDGE_LUT_17.u32 = au32MotionEdgeLUT[16];
    pstVirReg->MOTION_EDGE_LUT_18.u32 = au32MotionEdgeLUT[17];
    pstVirReg->MOTION_EDGE_LUT_19.u32 = au32MotionEdgeLUT[18];
    pstVirReg->MOTION_EDGE_LUT_20.u32 = au32MotionEdgeLUT[19];
    pstVirReg->MOTION_EDGE_LUT_21.u32 = au32MotionEdgeLUT[20];
    pstVirReg->MOTION_EDGE_LUT_22.u32 = au32MotionEdgeLUT[21];
    pstVirReg->MOTION_EDGE_LUT_23.u32 = au32MotionEdgeLUT[22];
    pstVirReg->MOTION_EDGE_LUT_24.u32 = au32MotionEdgeLUT[23];
    pstVirReg->MOTION_EDGE_LUT_25.u32 = au32MotionEdgeLUT[24];
    pstVirReg->MOTION_EDGE_LUT_26.u32 = au32MotionEdgeLUT[25];
    pstVirReg->MOTION_EDGE_LUT_27.u32 = au32MotionEdgeLUT[26];
    pstVirReg->MOTION_EDGE_LUT_28.u32 = au32MotionEdgeLUT[27];
    pstVirReg->MOTION_EDGE_LUT_29.u32 = au32MotionEdgeLUT[28];
    pstVirReg->MOTION_EDGE_LUT_30.u32 = au32MotionEdgeLUT[29];
    pstVirReg->MOTION_EDGE_LUT_31.u32 = au32MotionEdgeLUT[30];
    pstVirReg->MOTION_EDGE_LUT_32.u32 = au32MotionEdgeLUT[31];

    return HI_SUCCESS;
}

