/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name    : pq_mng_hdr.c
  Version       : Initial Draft
  Author        : p00203646
  Created      : 2016/03/19
  Description  :

******************************************************************************/
#include <linux/string.h>

#include "pq_mng_hdr_alg.h"
#include "pq_mng_hdr.h"
#include "pq_hal_comm.h"
#include "pq_hal_hdr.h"


static HI_BOOL sg_bHdrInitFlag      = HI_FALSE;
static HI_U32 sg_u32HdrSaturation   = 50;

HI_S32 PQ_MNG_InitHDR(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_TRUE == sg_bHdrInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bHdrInitFlag = HI_TRUE;

    PQ_CHECK_NULL_PTR(pstPqParam);

    s32Ret = PQ_HAL_InitHDR(pstPqParam, bDefault);

    return s32Ret;
}

HI_S32 PQ_MNG_DeInitHDR(HI_VOID)
{
    if (HI_FALSE == sg_bHdrInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bHdrInitFlag   = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetHDRCfg(HI_PQ_WIN_HDR_INFO* pstWinHdrInfo, HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    PQ_CHECK_NULL_PTR(pstWinHdrInfo);
    PQ_CHECK_NULL_PTR(pstPqHdrCfg);

    return PQ_HAL_GetHDRCfg(pstWinHdrInfo, pstPqHdrCfg);
}

HI_S32 PQ_MNG_SetHDRDefaultCfg(HI_BOOL bDefault)
{
    return PQ_HAL_SetHDRDefaultCfg(bDefault);
}

HI_S32 PQ_MNG_GetHDRSaturation(HI_U32* pu32Saturation)
{
    PQ_CHECK_NULL_PTR(pu32Saturation);
    *pu32Saturation = sg_u32HdrSaturation;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetHDRSaturation(HI_U32 u32Saturation)
{
    static HI_U32 u32CurHdrSatu = 0;

    sg_u32HdrSaturation = u32Saturation;

    if (HI_FALSE == sg_bHdrInitFlag)
    {
        return HI_FAILURE;
    }

    if (u32CurHdrSatu == u32Saturation)
    {
        return HI_SUCCESS;
    }

    u32CurHdrSatu = u32Saturation;

    return PQ_HAL_SetHDRSaturation(u32Saturation);
}


HI_S32 PQ_MNG_SetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut)
{
    PQ_CHECK_NULL_PTR(pstTMSetLut);

    return PQ_HAL_SetHDRTmLut(pstTMSetLut);
}

HI_S32 PQ_MNG_GetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut)
{
    PQ_CHECK_NULL_PTR(pstTMSetLut);

    return PQ_HAL_GetHDRTmLut(pstTMSetLut);
}

#define TRANSFER_CHARACTERISSTICS_HEVC_ST2084  16
#define TRANSFER_CHARACTERISSTICS_AVS2_ST2084  12

static HI_VOID pq_mng_UnifyHDRHevcType(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
                                       HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    HI_BOOL bFullRange      = pstHDRInfoIn->stColorDescriptionInfo.u8FullRangeFlag;
    HI_U8 u8ColourPrimaries = pstHDRInfoIn->stColorDescriptionInfo.u8ColourPrimaries;
    HI_U8 u8MatrixCoeffs    = pstHDRInfoIn->stColorDescriptionInfo.u8MatrixCoeffs;

    if (HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstHDRInfoIn->enSrcFrameType)
    {
        if (TRANSFER_CHARACTERISSTICS_HEVC_ST2084 == pstHDRInfoIn->u8TransferCharacteristics)
        {
            pstHDRInfoOut->enEotfType = HI_PQ_HDR_EOTF_2084;
        }
        else
        {
            pstHDRInfoOut->enEotfType = HI_PQ_HDR_EOTF_1886;
        }
    }

    switch (u8ColourPrimaries)
    {
        case 1:
            pstHDRInfoOut->enColorSpace =
                (HI_TRUE == bFullRange) ? HI_PQ_HDR_COLOR_PRIMARIES_BT709_F : HI_PQ_HDR_COLOR_PRIMARIES_BT709_L;
            break;
        case 9:
            pstHDRInfoOut->enColorSpace =
                (HI_TRUE == bFullRange) ? HI_PQ_HDR_COLOR_PRIMARIES_BT2020_F : HI_PQ_HDR_COLOR_PRIMARIES_BT2020_L;
            break;
        default:
            pstHDRInfoOut->enColorSpace = HI_PQ_HDR_COLOR_PRIMARIES_UNKNOW;
            break;
    }

    switch (u8MatrixCoeffs)
    {
        case 1:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT709;
            break;
        case 9:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT2020_NCL;
            break;
        case 10:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT2020_CL;
            break;
        default:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_UNKNOW;
            break;
    }

    return;
}

static HI_VOID pq_mng_UnifyHDRAvs2Type(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
                                       HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    HI_BOOL bFullRange      = pstHDRInfoIn->stColorDescriptionInfo.u8FullRangeFlag;
    HI_U8 u8ColourPrimaries = pstHDRInfoIn->stColorDescriptionInfo.u8ColourPrimaries;
    HI_U8 u8MatrixCoeffs    = pstHDRInfoIn->stColorDescriptionInfo.u8MatrixCoeffs;

    if (HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstHDRInfoIn->enSrcFrameType)
    {
        if (TRANSFER_CHARACTERISSTICS_AVS2_ST2084 == pstHDRInfoIn->u8TransferCharacteristics)
        {
            pstHDRInfoOut->enEotfType = HI_PQ_HDR_EOTF_2084;
        }
        else
        {
            pstHDRInfoOut->enEotfType = HI_PQ_HDR_EOTF_1886;
        }
    }

    switch (u8ColourPrimaries)
    {
        case 1:
            pstHDRInfoOut->enColorSpace =
                (HI_TRUE == bFullRange) ? HI_PQ_HDR_COLOR_PRIMARIES_BT709_F : HI_PQ_HDR_COLOR_PRIMARIES_BT709_L;
            break;
        case 9:
            pstHDRInfoOut->enColorSpace =
                (HI_TRUE == bFullRange) ? HI_PQ_HDR_COLOR_PRIMARIES_BT2020_F : HI_PQ_HDR_COLOR_PRIMARIES_BT2020_L;
            break;
        default:
            pstHDRInfoOut->enColorSpace = HI_PQ_HDR_COLOR_PRIMARIES_UNKNOW;
            break;
    }

    switch (u8MatrixCoeffs)
    {
        case 1:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT709;
            break;
        case 8:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT2020_NCL;
            break;
        case 9:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_BT2020_CL;
            break;
        default:
            pstHDRInfoOut->enMatrixType = HI_PQ_HDR_MATRIX_COEFF_UNKNOW;
            break;
    }

    return;

}

static HI_VOID pq_mng_UnifyHDRType(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
                                   HI_DRV_VIDEO_STD_E  enVideoStd,
                                   HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    if (HI_DRV_STD_HEVC == enVideoStd)
    {
        pq_mng_UnifyHDRHevcType(pstHDRInfoIn, pstHDRInfoOut);
    }
    else if (HI_DRV_STD_AVS2 == enVideoStd)
    {
        pq_mng_UnifyHDRAvs2Type(pstHDRInfoIn, pstHDRInfoOut);
    }

    return;
}

static HI_VOID pq_mng_UnifyHDRValueHDR10Hevc(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
        HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    HI_U32 i;

    /* Mastering Info */
    pstHDRInfoOut->bMasteringAvailable = pstHDRInfoIn->bMasteringAvailable;
    pstHDRInfoOut->stMasteringInfo.u16WhitePoint_x = pstHDRInfoIn->stMasteringInfo.u16WhitePoint_x;
    pstHDRInfoOut->stMasteringInfo.u16WhitePoint_y = pstHDRInfoIn->stMasteringInfo.u16WhitePoint_y;
    pstHDRInfoOut->stMasteringInfo.u32MaxDisplayMasteringLuminance
        = pstHDRInfoIn->stMasteringInfo.u32MaxDisplayMasteringLuminance;
    pstHDRInfoOut->stMasteringInfo.u32MinDisplayMasteringLuminance
        = pstHDRInfoIn->stMasteringInfo.u32MinDisplayMasteringLuminance;
    for (i = 0; i < 3; i++)
    {
        pstHDRInfoOut->stMasteringInfo.u16DisplayPrimaries_x[i]
            = pstHDRInfoIn->stMasteringInfo.u16DisplayPrimaries_x[i];
        pstHDRInfoOut->stMasteringInfo.u16DisplayPrimaries_y[i]
            = pstHDRInfoIn->stMasteringInfo.u16DisplayPrimaries_y[i];
    }

    /* Content Info */
    pstHDRInfoOut->bContentAvailable  = pstHDRInfoIn->bContentAvailable;
    pstHDRInfoOut->stContentInfo.u32MaxContentLightLevel
        = pstHDRInfoIn->stContentInfo.u32MaxContentLightLevel;
    pstHDRInfoOut->stContentInfo.u32MaxPicAverageLightLevel
        = pstHDRInfoIn->stContentInfo.u32MaxPicAverageLightLevel;

    return;
}

static HI_VOID pq_mng_UnifyHDRValue(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
                                    HI_DRV_VIDEO_STD_E  enVideoStd,
                                    HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    if ((HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstHDRInfoIn->enSrcFrameType)
        && (HI_DRV_STD_HEVC == enVideoStd))
    {
        pq_mng_UnifyHDRValueHDR10Hevc(pstHDRInfoIn, pstHDRInfoOut);
    }

    return;
}

HI_S32 PQ_MNG_GetHDRUnifiedMetadata(HI_PQ_HDR_UNIFIED_METADATA_IN_S* pstHDRInfoIn,
                                    HI_DRV_VIDEO_STD_E  enVideoStd,
                                    HI_PQ_HDR_UNIFIED_METADATA_OUT_S* pstHDRInfoOut)
{
    PQ_CHECK_NULL_PTR(pstHDRInfoIn);
    PQ_CHECK_NULL_PTR(pstHDRInfoOut);

    pstHDRInfoOut->enSrcFrameType = pstHDRInfoIn->enSrcFrameType;

    pq_mng_UnifyHDRType(pstHDRInfoIn, enVideoStd, pstHDRInfoOut);
    pq_mng_UnifyHDRValue(pstHDRInfoIn, enVideoStd, pstHDRInfoOut);

    return HI_SUCCESS;
}

static stPQAlgFuncs stHDRFuncs =
{
    .Init                  = PQ_MNG_InitHDR,
    .DeInit                = PQ_MNG_DeInitHDR,
    .GetHdrCfg             = PQ_MNG_GetHDRCfg,
    .SetHdrDefaultCfg      = PQ_MNG_SetHDRDefaultCfg,
    .GetHdrSaturation      = PQ_MNG_GetHDRSaturation,
    .SetHdrSaturation      = PQ_MNG_SetHDRSaturation,
    .GetHdrUnifiedMetadata = PQ_MNG_GetHDRUnifiedMetadata,
};

HI_S32 PQ_MNG_RegisterHDR(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_HDR, enType, PQ_BIN_ADAPT_SINGLE, "hdr", HI_NULL, &stHDRFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterHDR(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_HDR);

    return s32Ret;
}

