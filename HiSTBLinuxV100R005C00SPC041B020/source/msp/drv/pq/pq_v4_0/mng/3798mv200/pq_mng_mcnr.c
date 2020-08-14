/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_mcnr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2016/07/18
  Description   :

******************************************************************************/
#include "hi_math.h"
#include "drv_pq_comm.h"
#include "pq_hal_mcnr.h"
#include "pq_mng_mcnr.h"


#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define NR_CLIP3(low,high,x)  (MAX(MIN((high),(x)),(low)))

#define NR_CLIP(x) (((x) < 0) ? 0 : (x))
#define NR_CLIP255(x)  (MAX( MIN((x), 255), 0))
#define NR_CLIP1023(x) (MAX( MIN((x), 1023), 0))

#define MCNR_Y_MAX_WIDTH  (720)
#define MCNR_Y_MIN_WIDTH  (128)

static MCNR_TIMING_INFO g_McnrTimingInfo;
static HI_BOOL sg_bMCNREn = HI_TRUE;

/*--------------------------------------------------------------------------------*
Function        :   HI_S32 pq_mng_MCNR_InitCfg(MCNR_OUTPUT_REG *pReg, MCNR_INPUT_INFO *pInf)
Description     :   初始化MCNR参数
Calls           :
Data Accessed   :
Data Updated    :
Input           :       pReg.CNcomment:输出逻辑寄存器值 CNend;
                pInf.CNcomment:输出中间变量 CNend;
Output          :   NA
Return          :       HI_SUCCESS     success.  CNcomment:成功 CNend
Others          :       NA
*--------------------------------------------------------------------------------*/
static HI_S32 pq_mng_MCNR_InitCfg(MCNR_INPUT_INFO *pInf)
{
    pInf->Cfg_InputPort = 0;

    /* info */
    pInf->Cfg_ND_FlatInfoXMin   = 0;    // ND_FlatInfoXMin
    pInf->Cfg_ND_FlatInfoXMax   = 56;   // ND_FlatInfoXMax
    pInf->Cfg_ND_FlatInfoYMin   = 0;    // ND_FlatInfoYMin
    pInf->Cfg_ND_FlatInfoYMax   = 64;   // ND_FlatInfoYMax

    pInf->Cfg_MD_YMDGain        = 15;
    pInf->Cfg_MD_YMDCore        = 0;
    pInf->Cfg_MD_CMDGain        = 15;
    pInf->Cfg_MD_CMDCore        = 0;

    pInf->Cfg_ME_Adj_MV_Max          = 24;
    pInf->Cfg_ME_Adj_MV_Min          = 8;
    pInf->Cfg_ME_MotionEstimationEn  = 1;    // ME_MotionEstimateEn

    pInf->Cfg_MC_YMotionGain         = 31;   // MC_YMotionGain
    pInf->Cfg_MC_YMotionCore         = 0;    // MC_YMotionCore
    pInf->Cfg_MC_CMotionGain         = 31;   // MC_CMotionGain
    pInf->Cfg_MC_CMotionCore         = 0;    // MC_CMotionCore
    pInf->Cfg_MC_YMotionStrXMin      = 0;    // MC_YMotionStrXMin
    pInf->Cfg_MC_YMotionStrXMax      = 64;   // MC_YMotionStrXMax
    pInf->Cfg_MC_YMotionStrYMin      = 0;    // MC_YMotionStrYMin
    pInf->Cfg_MC_YMotionStrYMax      = 31;   // MC_YMotionStrYMax
    pInf->Cfg_MC_CMotionStrXMin      = 0;    // MC_YMotionStrXMin
    pInf->Cfg_MC_CMotionStrXMax      = 64;   // MC_YMotionStrXMax
    pInf->Cfg_MC_CMotionStrYMin      = 0;    // MC_YMotionStrYMin
    pInf->Cfg_MC_CMotionStrYMax      = 31;   // MC_YMotionStrYMax
    pInf->Cfg_MC_AutoMotionMappingEn = 0;    // MC_AutoMotionMappingEn

    /* inf */
    pInf->Cfg_TF_AutoMotionAlphaEn = 1; // TNR_AutoMotionAlphaEn
    pInf->Cfg_TF_YBlendingXMin     = 0; // TNR_YBlendingXMin
    pInf->Cfg_TF_YBlendingXMax     = 5; // TNR_YBlendingXMax
    pInf->Cfg_TF_YBlendingYMin     = 12;// TNR_YBlendingYMin
    pInf->Cfg_TF_YBlendingYMax     = 31;// TNR_YBlendingYMax
    pInf->Cfg_TF_YBlendingMin      = 4; // TNR_YBlendingMin

    pInf->Cfg_TF_CBlendingXMin     = 0; // TNR_CBlendingXMin
    pInf->Cfg_TF_CBlendingXMax     = 5; // TNR_CBlendingXMax
    pInf->Cfg_TF_CBlendingYMin     = 12;// TNR_CBlendingYMin
    pInf->Cfg_TF_CBlendingYMax     = 31;// TNR_CBlendingYMax
    pInf->Cfg_TF_CBlendingMin      = 4; // TNR_CBlendingMin

    pInf->Cfg_TF_DTBlendingXMin    = 12;// TNR_DTBlendingXMin
    pInf->Cfg_TF_DTBlendingXMax    = 30;// TNR_DTBlendingXMax
    pInf->Cfg_TF_DTBlendingYMin    = 2; // TNR_DTBlendingYMin
    pInf->Cfg_TF_DTBlendingYMax    = 31;// TNR_DTBlendingYMax

    return HI_SUCCESS;
}

/*--------------------------------------------------------------------------------*
Function        :   HI_S32 pq_mng_MCNR_GetMCTFBlendingAlpha(HI_S32 Index, HI_S32 X_Min, HI_S32 X_Max, HI_S32 Y_Min, HI_S32 Y_Max)
Description     :   获取Alhpa-blend 的结果
Calls           :
Data Accessed   :
Data Updated    :
Input           : Index.CNcomment:索引 CNend;
                  X_Min.CNcomment:最小X CNend;
                  X_Max.CNcomment:最大X CNend;
                  Y_Min.CNcomment:最小Y CNend;
                  Y_Max.CNcomment:最大Y CNend;
Output          :   NA
Return          :   result     .  CNcomment:结果 CNend
Others          :   NA
*--------------------------------------------------------------------------------*/
static HI_S32 pq_mng_MCNR_GetMCTFBlendingAlpha(HI_S32 Index,
        HI_S32 X_Min,
        HI_S32 X_Max,
        HI_S32 Y_Min,
        HI_S32 Y_Max)
{
    HI_S32 x;
    HI_S32 ThreeSigma;
    HI_S32 result;
    HI_S8 LUT32[32] = {100, 100, 98, 96, 93, 89, 84, 79, 74, 68, 63, 57, 51, 45, 40, 35, 30, 26, 22, 18, 15, 13, 10, 8, 7, 5, 4, 3, 3, 2, 1, 1};
    HI_S8 B;

    ThreeSigma = ((X_Max > X_Min) ? (X_Max - X_Min) : 31);

    if (Index < X_Min)
    {
        x = 0;
    }
    else if (Index > X_Max)
    {
        x = 31;
    }
    else
    {
        x = NR_CLIP3(0, 31, 31 * (Index - X_Min) / ThreeSigma);
    }

    B = LUT32[x];

    result = ((Y_Max - Y_Min) * (100 - B) + 50) / 100 + Y_Min;

    return  NR_CLIP3(0, 31, result);

}

HI_VOID pq_mng_CopyU32ByBitWithSignal(HI_U32 u32Src, HI_U32 u32SrcStartBit, HI_S32 *pu32Dst, HI_U32 u32DstStartBit, HI_U32 u32Num)
{
    HI_U32 u32SrcTmp = 0;

    if (((u32SrcStartBit + u32Num) > 32) || (u32DstStartBit + u32Num) > 32)
    {
        return;
    }

    /* 保存src需要拷贝的u32Num个bits，并移动到dst指定的位置; 将高位清0 */
    u32SrcTmp = u32Src << (32 - u32SrcStartBit - u32Num);

    /* 将指定的u32Num位移到低bit位上 */
    u32SrcTmp = u32SrcTmp >> (32 - u32Num);

    if (((u32SrcTmp >> ( u32Num - 1)) & 0x1) == 1)
    {
        //*pu32Dst = ((u32SrcTmp&((1<<u32Num) -1)) - (1<<u32Num));
        *pu32Dst = 0 - ((u32SrcTmp ^ ((1 << u32Num) - 1)) + 1);
    }
    else
    {
        *pu32Dst = u32SrcTmp & ((1 << u32Num) - 1);
    }

    return;
}

HI_VOID pq_mng_CopyU32ByBitWithoutSignal(HI_U32 u32Src, HI_U32 u32SrcStartBit, HI_U32 *pu32Dst, HI_U32 u32DstStartBit, HI_U32 u32Num)
{
    HI_U32 u32SrcTmp = 0;

    if (((u32SrcStartBit + u32Num) > 32) || (u32DstStartBit + u32Num) > 32)
    {
        return;
    }

    /* 保存src需要拷贝的u32Num个bits，并移动到dst指定的位置; 将高位清0 */
    u32SrcTmp = u32Src << (32 - u32SrcStartBit - u32Num);

    /* 将指定的u32Num位移到低bit位上 */
    u32SrcTmp = u32SrcTmp >> (32 - u32Num);
    *pu32Dst  = u32SrcTmp & ((1 << u32Num) - 1);

    return;
}
static HI_S32 pq_mng_MCNR_SetTimingInfo(HI_S32 GlobalMotion, HI_S32 saltusVal)
{
    HI_S32 i;
    HI_S32 ZeroGlobalMotion = 0;
    HI_S32 ImageStillLevel;
    HI_S32 SaltusLevel = NR_CLIP3(0, 310000, saltusVal * 10 + 5);

    if (SaltusLevel >= 100000) /* 10 */
    {
        g_McnrTimingInfo.Inf_ME_SaltusFlag = 1;
    }

    g_McnrTimingInfo.Inf_ME_SaltusFlag = g_McnrTimingInfo.Inf_ME_SaltusFlag - 1;

    if (g_McnrTimingInfo.Inf_ME_SaltusFlag < -2)
    {
        g_McnrTimingInfo.Inf_ME_SaltusFlag = -2;
    }

    if (SaltusLevel >= g_McnrTimingInfo.Inf_ME_preSaltusLevel[2] || SaltusLevel >= g_McnrTimingInfo.Inf_ME_preSaltusLevel[1])
    {
        SaltusLevel = (g_McnrTimingInfo.Inf_ME_preSaltusLevel[1] + g_McnrTimingInfo.Inf_ME_preSaltusLevel[2] + 6 * SaltusLevel) / 8;
    }
    else if ((g_McnrTimingInfo.Inf_ME_SaltusFlag < 0) && (g_McnrTimingInfo.Inf_ME_SaltusFlag > -4))
    {
        SaltusLevel = (g_McnrTimingInfo.Inf_ME_preSaltusLevel[2] + SaltusLevel) / 2;
    }
    else
    {
        SaltusLevel = (g_McnrTimingInfo.Inf_ME_preSaltusLevel[0] + g_McnrTimingInfo.Inf_ME_preSaltusLevel[1] + 2 * g_McnrTimingInfo.Inf_ME_preSaltusLevel[2] + 4 * SaltusLevel) / 8;
    }

    if (GlobalMotion >= g_McnrTimingInfo.Inf_ME_preGlobalMotion[3] || GlobalMotion >= g_McnrTimingInfo.Inf_ME_preGlobalMotion[2])
    {
        GlobalMotion = (g_McnrTimingInfo.Inf_ME_preGlobalMotion[2] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[3] + 6 * GlobalMotion) / 8;
    }
    else
    {
        GlobalMotion = (g_McnrTimingInfo.Inf_ME_preGlobalMotion[0] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[1] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[2] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[3] + GlobalMotion) / 5;
    }

    for (i = 0; i < 2; i++)
    {
        g_McnrTimingInfo.Inf_ME_preSaltusLevel[i]  = g_McnrTimingInfo.Inf_ME_preSaltusLevel[i + 1];
    }

    g_McnrTimingInfo.Inf_ME_preSaltusLevel[i] = SaltusLevel;

    for (i = 0; i < 3; i++)
    {
        g_McnrTimingInfo.Inf_ME_preGlobalMotion[i] = g_McnrTimingInfo.Inf_ME_preGlobalMotion[i + 1];
        ZeroGlobalMotion += g_McnrTimingInfo.Inf_ME_preGlobalMotion[i];
    }

    g_McnrTimingInfo.Inf_ME_preGlobalMotion[i] = GlobalMotion;
    ZeroGlobalMotion += g_McnrTimingInfo.Inf_ME_preGlobalMotion[i];

    ImageStillLevel = NR_CLIP3(0, 310000, ZeroGlobalMotion * 10 + 5);

    if (ImageStillLevel >= g_McnrTimingInfo.Inf_ME_preImageStillLevel[1]
        || ImageStillLevel >= g_McnrTimingInfo.Inf_ME_preImageStillLevel[2])
    {
        ImageStillLevel = (g_McnrTimingInfo.Inf_ME_preImageStillLevel[1]
                           + g_McnrTimingInfo.Inf_ME_preImageStillLevel[2]
                           + 6 * ImageStillLevel) / 8;
    }
    else
    {
        ImageStillLevel = (g_McnrTimingInfo.Inf_ME_preImageStillLevel[0]
                           + g_McnrTimingInfo.Inf_ME_preImageStillLevel[1]
                           + 2 * g_McnrTimingInfo.Inf_ME_preImageStillLevel[2]
                           + 4 * ImageStillLevel) / 8;
    }

    for (i = 0; i < 2; i++)
    {
        g_McnrTimingInfo.Inf_ME_preImageStillLevel[i]  = g_McnrTimingInfo.Inf_ME_preImageStillLevel[i + 1];
    }

    g_McnrTimingInfo.Inf_ME_preImageStillLevel[i] = ImageStillLevel;
    g_McnrTimingInfo.Inf_ME_SaltusLevel = (HI_S32)NR_CLIP3(0, 31, (SaltusLevel + 5000) / 10000);
    g_McnrTimingInfo.Inf_ME_ImageStillLevel = (HI_S32)NR_CLIP3(0, 31, (ZeroGlobalMotion * 10 + 5000) / 10000); // ZeroGlobalMotion;
    g_McnrTimingInfo.Inf_ME_GlobalMotion = (HI_S32)((GlobalMotion + 5000) / 10000);

    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_AdjGlobalMotion(void)
{
    if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 3) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel >= 32))
    {
        g_McnrTimingInfo.Inf_ME_GlobalMotion += 2;
    }

    return HI_SUCCESS;
}


static HI_S32 pq_mng_MCNR_EstimationGlobalMotionMCME(MCNR_INPUT_INFO *pInfIn, HI_VOID *pRGMV, HI_U32 u32Stride)
{
    HI_S32 i;
    HI_S32 wRGNum, hRGNum;
    HI_S32 MV_x = 0;
    HI_S32 MV_y = 0;
    HI_S32 MV_mag = 0;
    HI_S32 tmpMV;
    HI_S32 tmpMag;
    HI_S32 ME_HistMV[5];
    HI_S32 ME_HistMag[5];
    HI_S32 MVNum;
    HI_S32 nCol, nRow;
    HI_S32 GlobalMotion;
    HI_S32 saltusVal;
    HI_S32 meanPreGM;
    HI_S32 noiseLevel;
    MCMVSTRUCT RGMV;

    PQ_CHECK_NULL_PTR(pRGMV);

    noiseLevel = g_McnrTimingInfo.Inf_ND_NoiseLevel;
    RGMV.x   = 0;
    RGMV.mvy = 0;
    RGMV.mag = 0;

    hRGNum = (pInfIn->YHeight + 10) / 16;

    if (pInfIn->YWidth > 1920)
    {
        wRGNum = (pInfIn->YWidth / 2 + 33 ) / 64;
    }
    else
    {
        wRGNum = (pInfIn->YWidth + 33) / 64;
    }

    for (i = 0; i < 5; i++)
    {
        ME_HistMV[i] = 0;
        ME_HistMag[i] = 0;
    }

    for (nRow = 0; nRow < hRGNum; nRow++)
    {
        for (nCol = 0; nCol < wRGNum; nCol++)
        {
            /*解析RGMV的参数*/
            /* 98mv200:
            |63 61|60 57|56      47|46    39|38      29|28      19|18       9|8       0|
            ----------------------------------------------------------------------------
            |3'b0 | 3~0 |    9~0   |   7~0  |    9~0   |    9~0   |    9~0   |    8~0  |
            ----------------------------------------------------------------------------
            | Rs  |  ls |   vsad   |   mvy  | vstiilsad|    mag   |    sad   |     x   |
            */
            pq_mng_CopyU32ByBitWithSignal(*(HI_U32 *)((HI_U32 *)pRGMV    + (nRow * u32Stride / 4 + nCol * 2)), 0,  &(RGMV.x),   0, 9); //x[8:0]
            pq_mng_CopyU32ByBitWithSignal(*(HI_U32 *)((HI_U32 *)pRGMV    + (nRow * u32Stride / 4 + nCol * 2)), 19, &(RGMV.mag), 0, 10); //mag[9:0]
            pq_mng_CopyU32ByBitWithSignal(*(HI_U32 *)((HI_U32 *)pRGMV    + (nRow * u32Stride / 4 + nCol * 2) + 1), 7,  &(RGMV.mvy),  0, 8); //mvy[7:0]

            MV_x  = RGMV.x;
            MV_y  = RGMV.mvy;
            MV_mag = RGMV.mag;

            tmpMV = ABS(MV_x) + ABS(MV_y);

            if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 1) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel <= 16))
            {
                tmpMV = NR_CLIP(tmpMV - 2);
            }

            if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 1) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel <= 8))
            {
                tmpMV = NR_CLIP(tmpMV - 2);
            }

            if (0 >= tmpMV)
            {
                ME_HistMV[0]++;
            }
            else if (1 >= tmpMV)
            {
                ME_HistMV[1]++;
            }
            else if (2 >= tmpMV)
            {
                ME_HistMV[2]++;
            }
            else if (4 >= tmpMV)
            {
                ME_HistMV[3]++;
            }
            else
            {
                ME_HistMV[4]++;
            }

            tmpMag = NR_CLIP(MV_mag - 16);

            if (0 >= tmpMag)
            {
                ME_HistMag[0]++;
            }
            else if (8 >= tmpMag)
            {
                ME_HistMag[1]++;
            }
            else if (16 >= tmpMag)
            {
                ME_HistMag[2]++;
            }
            else if (48 >= tmpMag)
            {
                ME_HistMag[3]++;
            }
            else
            {
                ME_HistMag[4]++;
            }


        }
    }

    MVNum = hRGNum * wRGNum;

    if (pInfIn->YWidth < 1920)
    {
        GlobalMotion = NR_CLIP3(0, 320000, 640000 * (ME_HistMV[1] + ME_HistMV[2] + 2 * ME_HistMV[3] + 2 * ME_HistMV[4]) / 7 / MVNum);
    }
    else
    {
        GlobalMotion = NR_CLIP3(0, 320000, 640000 * (ME_HistMV[1] + 2 * ME_HistMV[2] + 2 * ME_HistMV[3] + 2 * ME_HistMV[4]) / 7 / MVNum);
    }

    g_McnrTimingInfo.Inf_ME_NonRgmeLevel = NR_CLIP3(0, 32, 6 * (8 * ME_HistMag[0] + 4 * ME_HistMag[1] + 2 * ME_HistMag[2] + ME_HistMag[3] + ME_HistMag[4]) / MVNum);
    meanPreGM = (g_McnrTimingInfo.Inf_ME_preGlobalMotion[1] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[2] + g_McnrTimingInfo.Inf_ME_preGlobalMotion[3]) / 3;

    if (meanPreGM < 10000) /* meanPreGM < 1 */
    {
        meanPreGM = 10000; /* 1.0 */
    }

    if (GlobalMotion <= 30000) /* GlobalMotion <= 3 */
    {
        saltusVal = 0;
    }
    else
    {
        if (GlobalMotion - meanPreGM < 20000) /* GlobalMotion - meanPreGM < 2 */
        {
            saltusVal = 0;
        }
        else
        {
            saltusVal = NR_CLIP(10000 * GlobalMotion / meanPreGM - 10000);
        }
    }

    pq_mng_MCNR_SetTimingInfo(GlobalMotion, saltusVal);
    pq_mng_MCNR_AdjGlobalMotion();

    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_GetMCNDFlatInfo(HI_S32 Index, HI_S32 X_Min, HI_S32 X_Max, HI_S32 Y_Min, HI_S32 Y_Max)
{
    HI_S32 x;
    HI_S32 ThreeSigma;
    HI_S32 result;
    HI_S16 B;
    HI_S16 LUT256[256] =
    {
        10000, 9999, 9997, 9994, 9989, 9983, 9975, 9966, 9956, 9944, 9931, 9917, 9901, 9884, 9865, 9845,
        9824, 9802, 9778, 9753, 9727, 9699, 9671, 9641, 9609, 9577, 9543, 9508, 9472, 9435, 9396, 9357,
        9316, 9274, 9231, 9187, 9142, 9096, 9049, 9001, 8952, 8902, 8851, 8799, 8746, 8692, 8638, 8582,
        8526, 8469, 8411, 8353, 8293, 8233, 8173, 8111, 8049, 7986, 7923, 7859, 7795, 7730, 7664, 7598,
        7532, 7465, 7397, 7330, 7261, 7193, 7124, 7055, 6985, 6916, 6846, 6775, 6705, 6634, 6564, 6493,
        6422, 6351, 6279, 6208, 6137, 6065, 5994, 5923, 5851, 5780, 5709, 5638, 5567, 5496, 5425, 5355,
        5285, 5215, 5145, 5075, 5006, 4936, 4868, 4799, 4731, 4663, 4595, 4528, 4461, 4395, 4328, 4263,
        4197, 4133, 4068, 4004, 3941, 3878, 3815, 3753, 3692, 3630, 3570, 3510, 3450, 3391, 3333, 3275,
        3218, 3161, 3105, 3049, 2994, 2940, 2886, 2833, 2780, 2728, 2677, 2626, 2576, 2526, 2477, 2429,
        2381, 2334, 2287, 2242, 2196, 2152, 2107, 2064, 2021, 1979, 1937, 1896, 1856, 1816, 1777, 1739,
        1701, 1663, 1626, 1590, 1555, 1520, 1485, 1451, 1418, 1385, 1353, 1322, 1291, 1260, 1230, 1201,
        1172, 1144, 1116, 1089, 1062, 1036, 1010,  985,  960,  936,  912,  889,  866,  844,  822,  801,
        780,   759,  739,  720,  700,  682,  663,  645,  628,  611,  594,  577,  561,  546,  530,  515,
        501,   487,  473,  459,  446,  433,  420,  408,  396,  384,  373,  362,  351,  340,  330,  320,
        310,   301,  292,  283,  274,  265,  257,  249,  241,  234,  226,  219,  212,  205,  198,  192,
        186,   180,  174,  168,  162,  157,  152,  147,  142,  137,  132,  128,  123,  119,  115,  111
    };

    ThreeSigma = ((X_Max > X_Min) ? (X_Max - X_Min) : 255);

    if (Index < X_Min)
    {
        x = 0; /* 0.0 */
    }
    else if (Index > X_Max)
    {
        x = 255; /* 1.0 */
    }
    else
    {
        x = NR_CLIP255(255 * (Index - X_Min) / ThreeSigma);
    }

    B = LUT256[x];
    result = ((Y_Max - Y_Min) * (10000 - B) + 5000) / 10000 + Y_Min;

    return  NR_CLIP3(Y_Min, Y_Max, result);
}

static HI_S32 pq_mng_MCNR_AdjNoiseLevel(void)
{
    if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 1) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel <= 16) && (g_McnrTimingInfo.Inf_ND_NoiseLevel <= 4))
    {
        g_McnrTimingInfo.Inf_ND_NoiseLevel += 2 * NR_CLIP(2 - g_McnrTimingInfo.Inf_ME_GlobalMotion);
    }

    if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 1) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel <= 8) && (g_McnrTimingInfo.Inf_ND_NoiseLevel <= 4))
    {
        g_McnrTimingInfo.Inf_ND_NoiseLevel += NR_CLIP(2 - g_McnrTimingInfo.Inf_ME_GlobalMotion);
    }
    if ((g_McnrTimingInfo.Inf_ME_GlobalMotion <= 4) && (g_McnrTimingInfo.Inf_ME_NonRgmeLevel <= 25) && (g_McnrTimingInfo.Inf_ND_NoiseLevel <= 5))
    {
        g_McnrTimingInfo.Inf_ND_NoiseLevel = NR_CLIP3(5, 6, g_McnrTimingInfo.Inf_ND_NoiseLevel + 1);
    }

    return HI_SUCCESS;
}


static HI_S32 pq_mng_MCNR_UpdateMCNDInformation(MCNR_INPUT_REG *pReg, MCNR_INPUT_INFO *pInfIn)
{
    HI_S32 rawNoise;
    HI_S32 noiseInfo;
    HI_S32 flatRatio;
    HI_S32 reliability;
    HI_S32 noiseLevel;
    HI_U32 tmp1;
    HI_U32 tmp2;
    HI_S32 globalMotion = g_McnrTimingInfo.Inf_ME_GlobalMotion;
    HI_S32 noiseRatio;

    if (pInfIn->YWidth >= 3840)
    {
        noiseRatio = 10000;
    }
    else if (pInfIn->YWidth >= 1280)
    {
        noiseRatio = 12000;
    }
    else
    {
        noiseRatio = 10000;
    }

    tmp1 = ((pReg->Reg_ND_SumNoiseInfoMax + 3 * pReg->Reg_ND_SumNoiseInfoMed + 4 * pReg->Reg_ND_SumNoiseInfoMin));
    tmp2 = (pReg->Reg_ND_NoisePointCntMed + 3 * pReg->Reg_ND_NoisePointCntMed + 4 * pReg->Reg_ND_NoisePointCntMin);

    if (tmp2 > 0)
    {
        tmp1 = noiseRatio * (tmp1 / tmp2);
    }
    else
    {
        tmp1 = 0;
    }

    rawNoise    = NR_CLIP3(0, 1270000, tmp1);
    flatRatio   = (255 * (pReg->Reg_ND_NoisePointCntMax +  3 * pReg->Reg_ND_NoisePointCntMed + 4 * pReg->Reg_ND_NoisePointCntMin + 4)) / 8 / pInfIn->YWidth / pInfIn->YHeight;
    reliability = pq_mng_MCNR_GetMCNDFlatInfo(flatRatio, pInfIn->Cfg_ND_FlatInfoXMin, pInfIn->Cfg_ND_FlatInfoXMax, pInfIn->Cfg_ND_FlatInfoYMin, pInfIn->Cfg_ND_FlatInfoYMax);
    reliability = NR_CLIP3(0, 50, reliability);
    noiseInfo   = rawNoise * reliability / 50;
    noiseLevel  = NR_CLIP3(0, 320000, noiseInfo);

    if (pInfIn->Cfg_InputPort == 1)
    {
        globalMotion = (HI_S32)NR_CLIP3(0, 16, NR_CLIP(globalMotion - NR_CLIP(noiseLevel / 10000 - 12)));
        noiseLevel = NR_CLIP3(noiseLevel / 4, 320000, noiseLevel * NR_CLIP(8 - NR_CLIP(globalMotion - 6)) / 8);
    }
    else
    {
        if (pInfIn->YWidth < 1920)
        {
            globalMotion = (HI_S32)NR_CLIP3(0, 16, NR_CLIP(globalMotion - NR_CLIP(2 * noiseLevel / 10000 - 16)));
            noiseLevel = NR_CLIP3(noiseLevel / 4, 320000, noiseLevel * NR_CLIP(6 - NR_CLIP(globalMotion - 5)) / 6);
        }
        else
        {
            globalMotion = (HI_S32)NR_CLIP3(0, 16, NR_CLIP(globalMotion - NR_CLIP(2 * noiseLevel / 10000 - 16)));
            noiseLevel = NR_CLIP3(noiseLevel / 4, 320000, noiseLevel * NR_CLIP(6 - NR_CLIP(globalMotion - 4)) / 6);
        }
    }

    noiseLevel = (g_McnrTimingInfo.Inf_ND_preNoiseLevel[0] + 2 * g_McnrTimingInfo.Inf_ND_preNoiseLevel[1] + 2 * g_McnrTimingInfo.Inf_ND_preNoiseLevel[2] + 3 * noiseLevel) / 8;

    g_McnrTimingInfo.Inf_ND_preNoiseLevel[0] = g_McnrTimingInfo.Inf_ND_preNoiseLevel[1];
    g_McnrTimingInfo.Inf_ND_preNoiseLevel[1] = g_McnrTimingInfo.Inf_ND_preNoiseLevel[2];
    g_McnrTimingInfo.Inf_ND_preNoiseLevel[2] = noiseLevel;
    g_McnrTimingInfo.Inf_ND_rawNoiseLevel       = (HI_S32)NR_CLIP255((noiseInfo + 5000) / 10000);
    g_McnrTimingInfo.Inf_ND_TextureDenseLevel   = (HI_S32)NR_CLIP3(0, 16, 16 - flatRatio);
    g_McnrTimingInfo.Inf_ND_NoiseLevel          = (HI_S32)((noiseLevel + 5000) / 10000);

    pq_mng_MCNR_AdjNoiseLevel();

    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_UpdateSNRRegisters(HI_U32 u32SceneChange, SNR_OUTPUT_REG *pRegOut)
{
    HI_S32 SNR_Motion_Ratio;

    SNR_Motion_Ratio     = NR_CLIP3(0, 63, NR_CLIP(g_McnrTimingInfo.Inf_ND_rawNoiseLevel - 2) + NR_CLIP(g_McnrTimingInfo.Inf_ND_NoiseLevel - 1) + NR_CLIP3(0, 6, g_McnrTimingInfo.Inf_ME_GlobalMotion - 1));
    pRegOut->MotionAlpha = NR_CLIP3(0, 63, 4 + 2 * SNR_Motion_Ratio);
    pRegOut->SceneChange = u32SceneChange;
    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_UpdateTimingParam(MCNR_INPUT_INFO *pInf, MCNR_TIMING_PARAM *tParam)
{
    HI_S32 Index;
    HI_S32 X_YMin, X_CMin, X_DTMin;
    HI_S32 X_YMax, X_CMax, X_DTMax;
    HI_S32 Y_YMin, Y_CMin, Y_DTMin;
    HI_S32 Y_YMax, Y_CMax, Y_DTMax;
    HI_S32 YAlpha;
    HI_S32 CAlpha;
    HI_S32 Alpha0;
    HI_S32 TextureDenseLevel;
    HI_S32 YBlendingMin;
    HI_S32 CBlendingMin;
    HI_S32 alphaCnt;
    HI_S8 NoiseLevel = 0;
    HI_S8 miniAlpha[32] = {24, 15, 10,  8,  6,  5,  4,  4,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};

    HI_S8 Noise_Noise_LUT_UHD[32] = { 0,  1,  2,  3,  4,  5,  5,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8};
    HI_S8 Noise_Noise_LUT_FHD[32] = { 0,  1,  2,  3,  4,  5,  6,  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8};
    HI_S8 Noise_Noise_LUT_TVD[32] = { 0,  1,  2,  3,  4,  5,  6,  7,  6,  5,  4,  3,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    HI_S8 Noise_Noise_LUT_SD[32]  = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  9,  8,  7,  6,  5,  4,  3,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};
    HI_S8 Noise_Noise_LUT_HD[32]  = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  8,  7,  6,  5,  4,  3,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};

    HI_S8 Noise_XMin_LUT[32] = { 0,  0,  1,  1,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6};
    HI_S8 Noise_XMax_LUT[32] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};


    /*** Motion Detection ***/
    if (pInf->YWidth <= MCNR_Y_MAX_WIDTH)
    {
        tParam->Reg_MD_Alpha1   = NR_CLIP3(0, 4, g_McnrTimingInfo.Inf_ND_NoiseLevel - 4);
        tParam->Reg_MD_Alpha2   = NR_CLIP3(0, 4, g_McnrTimingInfo.Inf_ND_NoiseLevel - 7);

        tParam->Reg_MD_YMDGain  = NR_CLIP3(0, 31, pInf->Cfg_MD_YMDGain);
        tParam->Reg_MD_YMDCore  = (HI_S32)NR_CLIP3(0, 31, pInf->Cfg_MD_YMDCore + 2 * NR_CLIP3(0, 8, g_McnrTimingInfo.Inf_ND_NoiseLevel - 8));
        tParam->Reg_MD_CMDGain  = NR_CLIP3(0, 31, pInf->Cfg_MD_CMDGain);
        tParam->Reg_MD_CMDCore  = (HI_S32)NR_CLIP3(0, 31, pInf->Cfg_MD_CMDCore + 2 * NR_CLIP3(0, 8, g_McnrTimingInfo.Inf_ND_NoiseLevel - 8));
    }
    else
    {
        tParam->Reg_MD_Alpha1   = NR_CLIP3(0, 4, 0);
        tParam->Reg_MD_Alpha2   = NR_CLIP3(0, 4, 0);

        tParam->Reg_MD_YMDGain  = NR_CLIP3(0, 31, pInf->Cfg_MD_YMDGain);
        tParam->Reg_MD_YMDCore  = (HI_S32)NR_CLIP3(0, 31, pInf->Cfg_MD_YMDCore);
        tParam->Reg_MD_CMDGain  = NR_CLIP3(0, 31, pInf->Cfg_MD_CMDGain);
        tParam->Reg_MD_CMDCore  = (HI_S32)NR_CLIP3(0, 31, pInf->Cfg_MD_CMDCore);
    }


    /***  Motion Detection  ***/
    /*** Motion Vector Estimation ***/
    if (pInf->Cfg_ME_MotionEstimationEn)
    {

        tParam->Reg_ME_Mag_Pnl_Gain_0MV = NR_CLIP3(0, 255, 127 - g_McnrTimingInfo.Inf_ME_GlobalMotion);
        tParam->Reg_ME_Mag_Pnl_Core_0MV = NR_CLIP1023(127 - 8 * g_McnrTimingInfo.Inf_ME_GlobalMotion);

        tParam->Reg_ME_Mag_Pnl_Gain_XMV = NR_CLIP3(0, 255, 127 + 10 * g_McnrTimingInfo.Inf_ME_GlobalMotion);

        if (g_McnrTimingInfo.Inf_ME_GlobalMotion <= 1)
        {
            tParam->Reg_ME_Mag_Pnl_Core_XMV = 560;
        }
        else
        {
            tParam->Reg_ME_Mag_Pnl_Core_XMV = NR_CLIP1023(127 - 8 * g_McnrTimingInfo.Inf_ME_GlobalMotion);
        }

        tParam->Reg_ME_Std_Pnl_Gain = NR_CLIP3(0, 31, 32);
        tParam->Reg_ME_Std_Pnl_Core = g_McnrTimingInfo.Inf_ND_NoiseLevel;

        tParam->Reg_ME_Adj_0MV_Min = NR_CLIP3(0, 16, 4);
        tParam->Reg_ME_Adj_0MV_Max = NR_CLIP3(0, 16, 16);
        tParam->Reg_ME_Adj_XMV_Min = NR_CLIP3(0, 16, 4);
        tParam->Reg_ME_Adj_XMV_Max = NR_CLIP3(0, 16, 16);

        tParam->Reg_ME_Std_Core_0MV             = 28;
        tParam->Reg_ME_Std_Core_XMV             = 3;

        tParam->Reg_ME_Std_Pnl_Gain_0MV         = 16;
        tParam->Reg_ME_Std_Pnl_Core_0MV         = 10;
        tParam->Reg_ME_Std_Pnl_Gain_XMV         = 10;
        tParam->Reg_ME_Std_Pnl_Core_XMV         = 16;

        tParam->Reg_ME_Adj_MV_Max = NR_CLIP3(16, 31, pInf->Cfg_ME_Adj_MV_Max - 2 * NR_CLIP(g_McnrTimingInfo.Inf_ND_NoiseLevel - 7));
        tParam->Reg_ME_Adj_MV_Min = NR_CLIP3( 0, 16, pInf->Cfg_ME_Adj_MV_Min +  NR_CLIP(g_McnrTimingInfo.Inf_ND_NoiseLevel - 9));

        tParam->Reg_ME_GM_Adj = NR_CLIP3(0, 4, g_McnrTimingInfo.Inf_ME_GlobalMotion);
    }

    /*** Motion Vector Estimation ***/
    /*** Motion Calculation ***/
    tParam->Reg_MC_NoiseLevel           = g_McnrTimingInfo.Inf_ND_NoiseLevel;
    tParam->Reg_MC_Scenechange          = pInf->SceneChange;
    tParam->Reg_MC_GlobalMotion         = g_McnrTimingInfo.Inf_ME_GlobalMotion;
    tParam->Reg_MC_ImageStillLevel      = g_McnrTimingInfo.Inf_ME_ImageStillLevel;
    tParam->Reg_MC_SaltusLevel          = NR_CLIP3(0, 15, g_McnrTimingInfo.Inf_ME_SaltusLevel - 1);

    tParam->Reg_MC_YMotionGain          = NR_CLIP3(0, 31, pInf->Cfg_MC_YMotionGain);
    tParam->Reg_MC_YMotionCore          = NR_CLIP3(0, 31, pInf->Cfg_MC_YMotionCore);
    tParam->Reg_MC_CMotionGain          = NR_CLIP3(0, 31, pInf->Cfg_MC_CMotionGain);
    tParam->Reg_MC_CMotionCore          = NR_CLIP3(0, 31, pInf->Cfg_MC_CMotionCore);

    /*** Motion Calculation ***/
    /*** Temporal Filtering ***/
    if (pInf->Cfg_TF_AutoMotionAlphaEn)
    {
        NoiseLevel = NR_CLIP3(0, 31, g_McnrTimingInfo.Inf_ND_NoiseLevel * (32 - NR_CLIP(g_McnrTimingInfo.Inf_ME_SaltusLevel - 3)) / 32);

        if (pInf->YWidth > 1920)
        {
            NoiseLevel = Noise_Noise_LUT_UHD[NoiseLevel];
        }
        else if (pInf->YWidth == 1920)
        {
            NoiseLevel = Noise_Noise_LUT_FHD[NoiseLevel];
        }
        else if (pInf->YWidth > MCNR_Y_MAX_WIDTH)
        {
            NoiseLevel = Noise_Noise_LUT_HD[NoiseLevel];
        }
        else if (pInf->Cfg_InputPort == 1)  /* TVD input port */
        {
            NoiseLevel = Noise_Noise_LUT_TVD[NoiseLevel];
        }
        else
        {
            NoiseLevel = Noise_Noise_LUT_SD[NoiseLevel];
        }

        if (pInf->CurFNum < 32)
        {
            alphaCnt = pInf->CurFNum;
            YBlendingMin = MAX(pInf->Cfg_TF_YBlendingMin, miniAlpha[alphaCnt]);
            CBlendingMin = MAX(pInf->Cfg_TF_CBlendingMin, miniAlpha[alphaCnt]);
        }
        else
        {
            YBlendingMin = MAX(pInf->Cfg_TF_YBlendingMin, 0);
            CBlendingMin = MAX(pInf->Cfg_TF_CBlendingMin, 0);
        }

        X_YMin = NR_CLIP3(0, 31, pInf->Cfg_TF_YBlendingXMin + Noise_XMin_LUT[NoiseLevel]);
        X_YMax = NR_CLIP3(0, 31, pInf->Cfg_TF_YBlendingXMax + Noise_XMax_LUT[NoiseLevel]);
        Y_YMin = NR_CLIP3(YBlendingMin, pInf->Cfg_TF_YBlendingYMin, pInf->Cfg_TF_YBlendingYMin - NoiseLevel);
        Y_YMax = pInf->Cfg_TF_YBlendingYMax;

        X_CMin = NR_CLIP3(0, 31, pInf->Cfg_TF_CBlendingXMin + Noise_XMin_LUT[NoiseLevel]);
        X_CMax = NR_CLIP3(0, 31, pInf->Cfg_TF_CBlendingXMax + Noise_XMax_LUT[NoiseLevel]);
        Y_CMin = NR_CLIP3(CBlendingMin, pInf->Cfg_TF_CBlendingYMin, pInf->Cfg_TF_CBlendingYMin - NoiseLevel);
        Y_CMax = pInf->Cfg_TF_CBlendingYMax;


        X_DTMin = NR_CLIP3(0, 31, pInf->Cfg_TF_DTBlendingXMin);
        X_DTMax = NR_CLIP3(0, 31, pInf->Cfg_TF_DTBlendingXMax);
        Y_DTMin = NR_CLIP3(0, 31, pInf->Cfg_TF_DTBlendingYMin);
        Y_DTMax = NR_CLIP3(0, 31, pInf->Cfg_TF_DTBlendingYMax);

        TextureDenseLevel = g_McnrTimingInfo.Inf_ND_TextureDenseLevel;

        for (Index = 0; Index < 32; Index++)
        {

            YAlpha = pq_mng_MCNR_GetMCTFBlendingAlpha(Index, X_YMin,  X_YMax,  Y_YMin,  Y_YMax);
            CAlpha = pq_mng_MCNR_GetMCTFBlendingAlpha(Index, X_CMin,  X_CMax,  Y_CMin,  Y_CMax);
            Alpha0 = pq_mng_MCNR_GetMCTFBlendingAlpha(Index, X_DTMin, X_DTMax, Y_DTMin, Y_DTMax);

            tParam->Reg_TF_YBlendingAlphaLUT[Index] = NR_CLIP3(0, 31, (YAlpha * (16 - TextureDenseLevel) + Alpha0 * TextureDenseLevel + 8) / 16);
            tParam->Reg_TF_CBlendingAlphaLUT[Index] = NR_CLIP3(0, 31, (CAlpha * (16 - TextureDenseLevel) + Alpha0 * TextureDenseLevel + 8) / 16);
        }
    }

    /*** Temporal Filtering ***/
    return HI_SUCCESS;
}

/* 获取统计信息 */
static HI_S32 pq_mng_MCNR_GetSttParam(S_STT_REGS_TYPE *pVpssSWBReg, MCNR_INPUT_REG *pinputReg)
{
    pinputReg->Reg_ND_NoisePointCntMax = pVpssSWBReg->TNR_NOISEPOINTCNT_MAX.bits.noisepointcntmax;
    pinputReg->Reg_ND_NoisePointCntMed = pVpssSWBReg->TNR_NOISEPOINTCNT_MED.bits.noisepointcntmed;
    pinputReg->Reg_ND_NoisePointCntMin = pVpssSWBReg->TNR_NOISEPOINTCNT_MIN.bits.noisepointcntmin;

    pinputReg->Reg_ND_SumNoiseInfoMax = pVpssSWBReg->TNR_SUMNOISEINFO_MAX.bits.sumnoiseinfomax;
    pinputReg->Reg_ND_SumNoiseInfoMed = pVpssSWBReg->TNR_SUMNOISEINFO_MED.bits.sumnoiseinfomed;
    pinputReg->Reg_ND_SumNoiseInfoMin = pVpssSWBReg->TNR_SUMNOISEINFO_MIN.bits.sumnoiseinfomin;

    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_UpdateTimingInfo(MCNR_INPUT_REG *pRegIn, MCNR_INPUT_INFO *pInfIn, HI_VOID *pRGMV, HI_U32 u32stride)
{
    /*** Motion Vector Estimation ***/
    if (pInfIn->Cfg_ME_MotionEstimationEn)
    {
        pq_mng_MCNR_EstimationGlobalMotionMCME(pInfIn, pRGMV, u32stride);
    }
    else
    {
        g_McnrTimingInfo.Inf_ME_SaltusLevel     = 0;
        g_McnrTimingInfo.Inf_ME_ImageStillLevel = 31;
        g_McnrTimingInfo.Inf_ME_GlobalMotion    = 0;
    }

    /*** Noise Level Estimation ***/
    if (pInfIn->Cfg_ND_NoiseDetectEn)
    {
        pq_mng_MCNR_UpdateMCNDInformation(pRegIn, pInfIn);
    }
    else
    {
        g_McnrTimingInfo.Inf_ND_NoiseLevel = 2;
    }

    return HI_SUCCESS;
}

static HI_S32 pq_mng_MCNR_UpdateSoftWareCfg(HI_U32 u32HandleNo,
        HI_VOID *pRGMV,
        HI_U32 u32Stride,
        S_STT_REGS_TYPE *pVpssSWBReg,
        HI_U32 u32Width,
        HI_U32 u32Height,
        HI_U32 u32SceneChange)
{
    MCNR_INPUT_INFO     stInputInf;
    MCNR_INPUT_REG      stInputReg;
    SNR_OUTPUT_REG      stOutputSnrReg;
    MCNR_TIMING_PARAM   stParam;

    stInputInf.YWidth      = u32Width;
    stInputInf.YHeight     = u32Height;
    stInputInf.SceneChange = u32SceneChange;

    pq_mng_MCNR_InitCfg(&stInputInf);

    if (u32SceneChange >= 224)
    {
        g_McnrTimingInfo.Inf_FrameNum_after_Scenechange = 0;
    }
    else
    {
        g_McnrTimingInfo.Inf_FrameNum_after_Scenechange++;
    }

    if (g_McnrTimingInfo.Inf_FrameNum_after_Scenechange >= 32)
    {
        g_McnrTimingInfo.Inf_FrameNum_after_Scenechange = 32;
    }

    stInputInf.CurFNum = g_McnrTimingInfo.Inf_FrameNum_after_Scenechange;

    if (u32Width > MCNR_Y_MAX_WIDTH)
    {
        stParam.Reg_ND_NoiseDetectEn         = HI_FALSE;
        stParam.Reg_MD_MDPreLPFEn            = HI_FALSE;
        stParam.Reg_ME_MotionEstimationEn    = HI_FALSE;
        stParam.Reg_MC_YMCAdjEn              = HI_FALSE;
        stParam.Reg_MC_CMCAdjEn              = HI_FALSE;
        stParam.Reg_MC_SaltusDetectEn        = HI_FALSE;

        stInputInf.Cfg_TF_AutoMotionAlphaEn  = HI_FALSE;
        stInputInf.Cfg_ND_NoiseDetectEn      = HI_FALSE;
        stInputInf.Cfg_ME_MotionEstimationEn = HI_FALSE;
    }
    else
    {
        stParam.Reg_ND_NoiseDetectEn         = HI_TRUE;
        stParam.Reg_MD_MDPreLPFEn            = HI_TRUE;
        stParam.Reg_ME_MotionEstimationEn    = HI_TRUE;
        stParam.Reg_MC_YMCAdjEn              = HI_TRUE;
        stParam.Reg_MC_CMCAdjEn              = HI_TRUE;
        stParam.Reg_MC_SaltusDetectEn        = HI_TRUE;

        stInputInf.Cfg_TF_AutoMotionAlphaEn  = HI_TRUE;
        stInputInf.Cfg_ND_NoiseDetectEn      = HI_TRUE;
        stInputInf.Cfg_ME_MotionEstimationEn = HI_TRUE;
    }

    pq_mng_MCNR_GetSttParam(pVpssSWBReg, &stInputReg);
    pq_mng_MCNR_UpdateTimingInfo(&stInputReg, &stInputInf, pRGMV, u32Stride);
    pq_mng_MCNR_UpdateTimingParam(&stInputInf, &stParam);
    pq_mng_MCNR_UpdateSNRRegisters(u32SceneChange, &stOutputSnrReg);

    PQ_HAL_UpdateTnrSoftReg(u32HandleNo, &stParam);
    PQ_HAL_UpdateSnrSoftReg(u32HandleNo, &stOutputSnrReg);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_UpdateMCNRCfg(HI_PQ_WBC_INFO_S *pstVpssWbcInfo)
{
    HI_S32 s32Ret         = HI_FAILURE;
    HI_BOOL bTNROnOff     = HI_FALSE;

    HI_U32 u32HandleNo    = pstVpssWbcInfo->u32HandleNo;
    HI_U32 u32Stride      = pstVpssWbcInfo->stride;
    HI_U32 u32Width       = pstVpssWbcInfo->u32Width;
    HI_U32 u32Height      = pstVpssWbcInfo->u32Height;
    HI_U32 u32SceneChange = 0;
    HI_VOID *pRGMV        = pstVpssWbcInfo->pRGMV;
    S_STT_REGS_TYPE *pVpssSWBReg =  pstVpssWbcInfo->pstVPSSWbcReg;

    if (HI_FALSE == pstVpssWbcInfo->bMcnrEnableFlag)
    {
        return HI_SUCCESS;
    }

    if ((u32Width > MCNR_Y_MAX_WIDTH) || (u32Width < MCNR_Y_MIN_WIDTH))
    {
        return HI_SUCCESS;
    }

    if (GET_ALG(HI_PQ_MODULE_TNR) && GET_ALG_FUN(HI_PQ_MODULE_TNR)->GetEnable)
    {
        GET_ALG_FUN(HI_PQ_MODULE_TNR)->GetEnable(&bTNROnOff);
    }

    if (HI_TRUE != bTNROnOff)
    {
        return HI_FAILURE;
    }

    if (HI_TRUE != sg_bMCNREn)
    {
        return HI_FAILURE;
    }

    s32Ret = pq_mng_MCNR_UpdateSoftWareCfg(u32HandleNo, pRGMV, u32Stride, pVpssSWBReg, u32Width, u32Height, u32SceneChange);

    return s32Ret;
}

HI_S32 PQ_MNG_EnableMCNR(HI_BOOL bOnOff)
{
    sg_bMCNREn = bOnOff;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetMCNREnable(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bMCNREn;
    return HI_SUCCESS;
}

