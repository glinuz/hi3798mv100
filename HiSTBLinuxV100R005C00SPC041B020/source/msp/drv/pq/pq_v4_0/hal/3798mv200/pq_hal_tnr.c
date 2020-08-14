/******************************************************************************
*
* Copyright (C) 2015-2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

 File Name    : pq_hal_tnr.c
 Version        : Initial Draft
 Author         : p00203646
 Created       : 2016/05/25
 Description   :
******************************************************************************/

#include "hi_type.h"
#include "pq_hal_tnr.h"
#include "pq_hal_comm.h"

static HI_S32 sg_s32TMTYMotionGain = 15;
static HI_S32 sg_s32TMTCMotionGain = 15;

const static HI_U8 sg_YMotionStrLut[128]          =
{
    0,  0,  0,  0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  4,  5,  5,
    6,  6,  7,  7,  8,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
    27, 28, 28, 29, 29, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

const static HI_U8 sg_CMotionStrLut[128]          =
{
    0,  0,  0,  0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  4,  5,  5,
    6,  6,  7,  7,  8,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 12,
    13, 13, 14, 14, 15, 15, 16, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27,
    27, 28, 28, 29, 29, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

const static HI_U8 sg_YBlendingAlphaLut[32] = { 7,  7,  7,  8,  9, 10, 12, 15, 18, 21, 23, 25, 27, 28, 29, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31};
const static HI_U8 sg_CBlendingAlphaLut[32] = { 7,  7,  7,  8,  9, 10, 12, 15, 18, 21, 23, 25, 27, 28, 29, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31};


HI_S32 PQ_HAL_EnableTNR(HI_U32 u32HandleNo, HI_BOOL bTNROnOff)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_CONTRL.bits.nren  = bTNROnOff;
    pstVpssReg->VPSS_TNR_CONTRL.bits.cnren = bTNROnOff;
    pstVpssReg->VPSS_TNR_CONTRL.bits.ynren = bTNROnOff;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_EnableTNRDemo(HI_U32 u32HandleNo, HI_BOOL bTNRDemoEn)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_CONTRL.bits.marketmodeen = bTNRDemoEn;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTNRDemoMode(HI_U32 u32HandleNo, TNR_DEMO_MODE_E enMode)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_CONTRL.bits.marketmode = (HI_U32)enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrMidStr(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    sg_s32TMTYMotionGain = pstVpssReg->VPSS_TNR_MAMC_MAP.bits.ymotiongain;
    sg_s32TMTCMotionGain = pstVpssReg->VPSS_TNR_MAMC_MAP.bits.cmotiongain;

    return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetTnrStr(HI_U32 u32HandleNo, HI_U32 u32TnrStr)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;
    HI_U32 u32YMotionGain = ((100 - u32TnrStr) * sg_s32TMTYMotionGain + 25) / 50;
    HI_U32 u32CMotionGain = ((100 - u32TnrStr) * sg_s32TMTCMotionGain + 25) / 50;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    if (u32YMotionGain > 31)
    {
        u32YMotionGain = 31;
    }

    if (u32CMotionGain > 31)
    {
        u32CMotionGain = 31;
    }

    pstVpssReg->VPSS_TNR_MAMC_MAP.bits.ymotiongain = u32YMotionGain;
    pstVpssReg->VPSS_TNR_MAMC_MAP.bits.cmotiongain = u32CMotionGain;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTnrDemoModeCoor(HI_U32 u32HandleNo, HI_U32 u32XPos)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_CONTRL.bits.marketcoor = u32XPos;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetTNRBlendingAlphaLut(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut0    = sg_CBlendingAlphaLut[0];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut1    = sg_CBlendingAlphaLut[1];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut2    = sg_CBlendingAlphaLut[2];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT0.bits.cblendingalphalut3    = sg_CBlendingAlphaLut[3];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut4    = sg_CBlendingAlphaLut[4];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut5    = sg_CBlendingAlphaLut[5];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut6    = sg_CBlendingAlphaLut[6];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT1.bits.cblendingalphalut7    = sg_CBlendingAlphaLut[7];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut8    = sg_CBlendingAlphaLut[8];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut9    = sg_CBlendingAlphaLut[9];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut10   = sg_CBlendingAlphaLut[10];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT2.bits.cblendingalphalut11   = sg_CBlendingAlphaLut[11];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut12   = sg_CBlendingAlphaLut[12];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut13   = sg_CBlendingAlphaLut[13];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut14   = sg_CBlendingAlphaLut[14];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT3.bits.cblendingalphalut15   = sg_CBlendingAlphaLut[15];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut16   = sg_CBlendingAlphaLut[16];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut17   = sg_CBlendingAlphaLut[17];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut18   = sg_CBlendingAlphaLut[18];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT4.bits.cblendingalphalut19   = sg_CBlendingAlphaLut[19];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut20   = sg_CBlendingAlphaLut[20];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut21   = sg_CBlendingAlphaLut[21];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut22   = sg_CBlendingAlphaLut[22];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT5.bits.cblendingalphalut23   = sg_CBlendingAlphaLut[23];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut24   = sg_CBlendingAlphaLut[24];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut25   = sg_CBlendingAlphaLut[25];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut26   = sg_CBlendingAlphaLut[26];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT6.bits.cblendingalphalut27   = sg_CBlendingAlphaLut[27];

    pstVpssReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut28   = sg_CBlendingAlphaLut[28];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut29   = sg_CBlendingAlphaLut[29];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut30   = sg_CBlendingAlphaLut[30];
    pstVpssReg->VPSS_TNR_CBLENDING_LUT7.bits.cblendingalphalut31   = sg_CBlendingAlphaLut[31];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut0    = sg_YBlendingAlphaLut[0];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut1    = sg_YBlendingAlphaLut[1];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut2    = sg_YBlendingAlphaLut[2];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT0.bits.yblendingalphalut3    = sg_YBlendingAlphaLut[3];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut4    = sg_YBlendingAlphaLut[4];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut5    = sg_YBlendingAlphaLut[5];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut6    = sg_YBlendingAlphaLut[6];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT1.bits.yblendingalphalut7    = sg_YBlendingAlphaLut[7];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut8    = sg_YBlendingAlphaLut[8];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut9    = sg_YBlendingAlphaLut[9];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut10   = sg_YBlendingAlphaLut[10];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT2.bits.yblendingalphalut11   = sg_YBlendingAlphaLut[11];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut12   = sg_YBlendingAlphaLut[12];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut13   = sg_YBlendingAlphaLut[13];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut14   = sg_YBlendingAlphaLut[14];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT3.bits.yblendingalphalut15   = sg_YBlendingAlphaLut[15];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut16   = sg_YBlendingAlphaLut[16];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut17   = sg_YBlendingAlphaLut[17];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut18   = sg_YBlendingAlphaLut[18];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT4.bits.yblendingalphalut19   = sg_YBlendingAlphaLut[19];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut20   = sg_YBlendingAlphaLut[20];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut21   = sg_YBlendingAlphaLut[21];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut22   = sg_YBlendingAlphaLut[22];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT5.bits.yblendingalphalut23   = sg_YBlendingAlphaLut[23];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut24   = sg_YBlendingAlphaLut[24];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut25   = sg_YBlendingAlphaLut[25];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut26   = sg_YBlendingAlphaLut[26];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT6.bits.yblendingalphalut27   = sg_YBlendingAlphaLut[27];

    pstVpssReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut28   = sg_YBlendingAlphaLut[28];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut29   = sg_YBlendingAlphaLut[29];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut30   = sg_YBlendingAlphaLut[30];
    pstVpssReg->VPSS_TNR_YBLENDING_LUT7.bits.yblendingalphalut31   = sg_YBlendingAlphaLut[31];

	return HI_SUCCESS;
}


HI_S32 PQ_HAL_SetTNRMotionStrLut(HI_U32 u32HandleNo)
{
    S_CAS_REGS_TYPE* pstVpssReg = HI_NULL;

    pstVpssReg = PQ_HAL_GetVpssReg(u32HandleNo);
    PQ_CHECK_NULL_PTR(pstVpssReg);

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut0     = sg_YMotionStrLut[0];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut1     = sg_YMotionStrLut[1];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut2     = sg_YMotionStrLut[2];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT0.bits.ymotionstrlut3     = sg_YMotionStrLut[3];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut4     = sg_YMotionStrLut[4];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut5     = sg_YMotionStrLut[5];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut6     = sg_YMotionStrLut[6];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT1.bits.ymotionstrlut7     = sg_YMotionStrLut[7];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut8     = sg_YMotionStrLut[8];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut9     = sg_YMotionStrLut[9];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut10 	 = sg_YMotionStrLut[10];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT2.bits.ymotionstrlut11 	 = sg_YMotionStrLut[11];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut12 	 = sg_YMotionStrLut[12];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut13 	 = sg_YMotionStrLut[13];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut14 	 = sg_YMotionStrLut[14];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT3.bits.ymotionstrlut15 	 = sg_YMotionStrLut[15];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut16 	 = sg_YMotionStrLut[16];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut17 	 = sg_YMotionStrLut[17];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut18 	 = sg_YMotionStrLut[18];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT4.bits.ymotionstrlut19 	 = sg_YMotionStrLut[19];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut20 	 = sg_YMotionStrLut[20];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut21 	 = sg_YMotionStrLut[21];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut22 	 = sg_YMotionStrLut[22];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT5.bits.ymotionstrlut23 	 = sg_YMotionStrLut[23];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut24 	 = sg_YMotionStrLut[24];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut25 	 = sg_YMotionStrLut[25];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut26 	 = sg_YMotionStrLut[26];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT6.bits.ymotionstrlut27 	 = sg_YMotionStrLut[27];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut28 	 = sg_YMotionStrLut[28];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut29 	 = sg_YMotionStrLut[29];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut30 	 = sg_YMotionStrLut[30];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT7.bits.ymotionstrlut31 	 = sg_YMotionStrLut[31];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut32 	 = sg_YMotionStrLut[32];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut33 	 = sg_YMotionStrLut[33];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut34 	 = sg_YMotionStrLut[34];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT8.bits.ymotionstrlut35 	 = sg_YMotionStrLut[35];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut36 	 = sg_YMotionStrLut[36];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut37 	 = sg_YMotionStrLut[37];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut38 	 = sg_YMotionStrLut[38];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT9.bits.ymotionstrlut39 	 = sg_YMotionStrLut[39];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut40	 = sg_YMotionStrLut[40];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut41	 = sg_YMotionStrLut[41];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut42	 = sg_YMotionStrLut[42];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT10.bits.ymotionstrlut43	 = sg_YMotionStrLut[43];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut44	 = sg_YMotionStrLut[44];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut45	 = sg_YMotionStrLut[45];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut46	 = sg_YMotionStrLut[46];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT11.bits.ymotionstrlut47	 = sg_YMotionStrLut[47];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut48	 = sg_YMotionStrLut[48];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut49	 = sg_YMotionStrLut[49];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut50	 = sg_YMotionStrLut[50];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT12.bits.ymotionstrlut51	 = sg_YMotionStrLut[51];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut52	 = sg_YMotionStrLut[52];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut53	 = sg_YMotionStrLut[53];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut54	 = sg_YMotionStrLut[54];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT13.bits.ymotionstrlut55	 = sg_YMotionStrLut[55];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut56	 = sg_YMotionStrLut[56];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut57	 = sg_YMotionStrLut[57];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut58	 = sg_YMotionStrLut[58];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT14.bits.ymotionstrlut59	 = sg_YMotionStrLut[59];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut60	 = sg_YMotionStrLut[60];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut61	 = sg_YMotionStrLut[61];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut62	 = sg_YMotionStrLut[62];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT15.bits.ymotionstrlut63	 = sg_YMotionStrLut[63];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut64	 = sg_YMotionStrLut[64];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut65	 = sg_YMotionStrLut[65];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut66	 = sg_YMotionStrLut[66];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT16.bits.ymotionstrlut67	 = sg_YMotionStrLut[67];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut68	 = sg_YMotionStrLut[68];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut69	 = sg_YMotionStrLut[69];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut70	 = sg_YMotionStrLut[70];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT17.bits.ymotionstrlut71	 = sg_YMotionStrLut[71];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut72	 = sg_YMotionStrLut[72];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut73	 = sg_YMotionStrLut[73];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut74	 = sg_YMotionStrLut[74];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT18.bits.ymotionstrlut75	 = sg_YMotionStrLut[75];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut76	 = sg_YMotionStrLut[76];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut77	 = sg_YMotionStrLut[77];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut78	 = sg_YMotionStrLut[78];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT19.bits.ymotionstrlut79	 = sg_YMotionStrLut[79];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut80	 = sg_YMotionStrLut[80];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut81	 = sg_YMotionStrLut[81];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut82	 = sg_YMotionStrLut[82];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT20.bits.ymotionstrlut83	 = sg_YMotionStrLut[83];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut84	 = sg_YMotionStrLut[84];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut85	 = sg_YMotionStrLut[85];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut86	 = sg_YMotionStrLut[86];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT21.bits.ymotionstrlut87	 = sg_YMotionStrLut[87];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut88	 = sg_YMotionStrLut[88];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut89	 = sg_YMotionStrLut[89];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut90	 = sg_YMotionStrLut[90];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT22.bits.ymotionstrlut91	 = sg_YMotionStrLut[91];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut92	 = sg_YMotionStrLut[92];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut93	 = sg_YMotionStrLut[93];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut94	 = sg_YMotionStrLut[94];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT23.bits.ymotionstrlut95	 = sg_YMotionStrLut[95];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut96	 = sg_YMotionStrLut[96];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut97	 = sg_YMotionStrLut[97];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut98	 = sg_YMotionStrLut[98];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT24.bits.ymotionstrlut99	 = sg_YMotionStrLut[99];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut100	 = sg_YMotionStrLut[100];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut101	 = sg_YMotionStrLut[101];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut102	 = sg_YMotionStrLut[102];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT25.bits.ymotionstrlut103	 = sg_YMotionStrLut[103];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut104	 = sg_YMotionStrLut[104];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut105	 = sg_YMotionStrLut[105];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut106	 = sg_YMotionStrLut[106];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT26.bits.ymotionstrlut107	 = sg_YMotionStrLut[107];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut108	 = sg_YMotionStrLut[108];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut109	 = sg_YMotionStrLut[109];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut110	 = sg_YMotionStrLut[110];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT27.bits.ymotionstrlut111	 = sg_YMotionStrLut[111];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut112	 = sg_YMotionStrLut[112];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut113	 = sg_YMotionStrLut[113];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut114	 = sg_YMotionStrLut[114];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT28.bits.ymotionstrlut115	 = sg_YMotionStrLut[115];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut116	 = sg_YMotionStrLut[116];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut117	 = sg_YMotionStrLut[117];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut118	 = sg_YMotionStrLut[118];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT29.bits.ymotionstrlut119	 = sg_YMotionStrLut[119];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut120	 = sg_YMotionStrLut[120];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut121	 = sg_YMotionStrLut[121];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut122	 = sg_YMotionStrLut[122];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT30.bits.ymotionstrlut123	 = sg_YMotionStrLut[123];

    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut124	 = sg_YMotionStrLut[124];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut125	 = sg_YMotionStrLut[125];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut126	 = sg_YMotionStrLut[126];
    pstVpssReg->VPSS_TNR_YMOTIONSTR_LUT31.bits.ymotionstrlut127	 = sg_YMotionStrLut[127];


    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut0 	 = sg_CMotionStrLut[0];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut1     = sg_CMotionStrLut[1];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut2     = sg_CMotionStrLut[2];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT0.bits.cmotionstrlut3     = sg_CMotionStrLut[3];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut4     = sg_CMotionStrLut[4];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut5     = sg_CMotionStrLut[5];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut6     = sg_CMotionStrLut[6];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT1.bits.cmotionstrlut7     = sg_CMotionStrLut[7];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut8     = sg_CMotionStrLut[8];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut9     = sg_CMotionStrLut[9];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut10 	 = sg_CMotionStrLut[10];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT2.bits.cmotionstrlut11 	 = sg_CMotionStrLut[11];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut12 	 = sg_CMotionStrLut[12];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut13 	 = sg_CMotionStrLut[13];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut14 	 = sg_CMotionStrLut[14];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT3.bits.cmotionstrlut15 	 = sg_CMotionStrLut[15];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut16 	 = sg_CMotionStrLut[16];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut17 	 = sg_CMotionStrLut[17];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut18 	 = sg_CMotionStrLut[18];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT4.bits.cmotionstrlut19 	 = sg_CMotionStrLut[19];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut20 	 = sg_CMotionStrLut[20];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut21 	 = sg_CMotionStrLut[21];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut22 	 = sg_CMotionStrLut[22];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT5.bits.cmotionstrlut23 	 = sg_CMotionStrLut[23];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut24 	 = sg_CMotionStrLut[24];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut25 	 = sg_CMotionStrLut[25];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut26 	 = sg_CMotionStrLut[26];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT6.bits.cmotionstrlut27 	 = sg_CMotionStrLut[27];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut28 	 = sg_CMotionStrLut[28];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut29 	 = sg_CMotionStrLut[29];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut30 	 = sg_CMotionStrLut[30];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT7.bits.cmotionstrlut31 	 = sg_CMotionStrLut[31];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut32 	 = sg_CMotionStrLut[32];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut33 	 = sg_CMotionStrLut[33];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut34 	 = sg_CMotionStrLut[34];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT8.bits.cmotionstrlut35 	 = sg_CMotionStrLut[35];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut36 	 = sg_CMotionStrLut[36];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut37 	 = sg_CMotionStrLut[37];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut38 	 = sg_CMotionStrLut[38];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT9.bits.cmotionstrlut39 	 = sg_CMotionStrLut[39];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut40	 = sg_CMotionStrLut[40];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut41	 = sg_CMotionStrLut[41];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut42	 = sg_CMotionStrLut[42];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT10.bits.cmotionstrlut43	 = sg_CMotionStrLut[43];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut44	 = sg_CMotionStrLut[44];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut45	 = sg_CMotionStrLut[45];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut46	 = sg_CMotionStrLut[46];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT11.bits.cmotionstrlut47	 = sg_CMotionStrLut[47];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut48	 = sg_CMotionStrLut[48];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut49	 = sg_CMotionStrLut[49];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut50	 = sg_CMotionStrLut[50];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT12.bits.cmotionstrlut51	 = sg_CMotionStrLut[51];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut52	 = sg_CMotionStrLut[52];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut53	 = sg_CMotionStrLut[53];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut54	 = sg_CMotionStrLut[54];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT13.bits.cmotionstrlut55	 = sg_CMotionStrLut[55];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut56	 = sg_CMotionStrLut[56];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut57	 = sg_CMotionStrLut[57];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut58	 = sg_CMotionStrLut[58];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT14.bits.cmotionstrlut59	 = sg_CMotionStrLut[59];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut60	 = sg_CMotionStrLut[60];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut61	 = sg_CMotionStrLut[61];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut62	 = sg_CMotionStrLut[62];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT15.bits.cmotionstrlut63	 = sg_CMotionStrLut[63];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut64	 = sg_CMotionStrLut[64];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut65	 = sg_CMotionStrLut[65];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut66	 = sg_CMotionStrLut[66];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT16.bits.cmotionstrlut67	 = sg_CMotionStrLut[67];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut68	 = sg_CMotionStrLut[68];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut69	 = sg_CMotionStrLut[69];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut70	 = sg_CMotionStrLut[70];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT17.bits.cmotionstrlut71	 = sg_CMotionStrLut[71];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut72	 = sg_CMotionStrLut[72];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut73	 = sg_CMotionStrLut[73];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut74	 = sg_CMotionStrLut[74];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT18.bits.cmotionstrlut75	 = sg_CMotionStrLut[75];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut76	 = sg_CMotionStrLut[76];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut77	 = sg_CMotionStrLut[77];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut78	 = sg_CMotionStrLut[78];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT19.bits.cmotionstrlut79	 = sg_CMotionStrLut[79];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut80	 = sg_CMotionStrLut[80];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut81	 = sg_CMotionStrLut[81];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut82	 = sg_CMotionStrLut[82];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT20.bits.cmotionstrlut83	 = sg_CMotionStrLut[83];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut84	 = sg_CMotionStrLut[84];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut85	 = sg_CMotionStrLut[85];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut86	 = sg_CMotionStrLut[86];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT21.bits.cmotionstrlut87	 = sg_CMotionStrLut[87];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut88	 = sg_CMotionStrLut[88];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut89	 = sg_CMotionStrLut[89];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut90	 = sg_CMotionStrLut[90];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT22.bits.cmotionstrlut91	 = sg_CMotionStrLut[91];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut92	 = sg_CMotionStrLut[92];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut93	 = sg_CMotionStrLut[93];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut94	 = sg_CMotionStrLut[94];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT23.bits.cmotionstrlut95	 = sg_CMotionStrLut[95];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut96	 = sg_CMotionStrLut[96];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut97	 = sg_CMotionStrLut[97];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut98	 = sg_CMotionStrLut[98];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT24.bits.cmotionstrlut99	 = sg_CMotionStrLut[99];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut100	 = sg_CMotionStrLut[100];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut101	 = sg_CMotionStrLut[101];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut102	 = sg_CMotionStrLut[102];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT25.bits.cmotionstrlut103	 = sg_CMotionStrLut[103];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut104	 = sg_CMotionStrLut[104];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut105	 = sg_CMotionStrLut[105];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut106	 = sg_CMotionStrLut[106];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT26.bits.cmotionstrlut107	 = sg_CMotionStrLut[107];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut108	 = sg_CMotionStrLut[108];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut109	 = sg_CMotionStrLut[109];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut110	 = sg_CMotionStrLut[110];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT27.bits.cmotionstrlut111	 = sg_CMotionStrLut[111];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut112	 = sg_CMotionStrLut[112];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut113	 = sg_CMotionStrLut[113];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut114	 = sg_CMotionStrLut[114];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT28.bits.cmotionstrlut115	 = sg_CMotionStrLut[115];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut116	 = sg_CMotionStrLut[116];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut117	 = sg_CMotionStrLut[117];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut118	 = sg_CMotionStrLut[118];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT29.bits.cmotionstrlut119	 = sg_CMotionStrLut[119];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut120	 = sg_CMotionStrLut[120];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut121	 = sg_CMotionStrLut[121];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut122	 = sg_CMotionStrLut[122];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT30.bits.cmotionstrlut123	 = sg_CMotionStrLut[123];

    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut124	 = sg_CMotionStrLut[124];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut125	 = sg_CMotionStrLut[125];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut126	 = sg_CMotionStrLut[126];
    pstVpssReg->VPSS_TNR_CMOTIONSTR_LUT31.bits.cmotionstrlut127	 = sg_CMotionStrLut[127];

	return HI_SUCCESS;
}


