/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_acm.c
  Version       : Initial Draft
  Author        :
  Created       : 2013/10/15
  Description   :

******************************************************************************/
#include <linux/string.h>      /* memcpy() */
#include <linux/fcntl.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "hi_drv_file.h"
#include "hi_drv_pq.h"


#include "pq_hal_comm.h"
#include "pq_hal_acm.h"
#include "pq_mng_acm.h"
#include "pq_mng_acm_coef.h"


#define ACM_GAIN_OFFSET_LOW_SD    16
#define ACM_GAIN_OFFSET_HIGH_SD   16
#define ACM_GAIN_OFFSET_LOW_HD    20
#define ACM_GAIN_OFFSET_HIGH_HD   20
#define ACM_GAIN_OFFSET_LOW_UHD   30
#define ACM_GAIN_OFFSET_HIGH_UHD  30


static HI_BOOL sg_bColorEn      = HI_TRUE;
static HI_BOOL sg_bColorDemoEn  = HI_FALSE;
static PQ_DEMO_MODE_E sg_enMode = PQ_DEMO_ENABLE_R;

static HI_U32  sg_u32ColorLevel = 50;
static HI_BOOL sg_bColorInitial = HI_FALSE;

static PQ_PARAM_S    *sg_pstPqParam = HI_NULL;
static PQ_ACM_COEF_S *sg_pstAcmCoef = HI_NULL;

static FLESHTONE_LEVEL_E sg_enFleshLevel        = FLE_GAIN_MID;
static COLOR_SPEC_MODE_E sg_enColorSpecModeType = COLOR_MODE_RECOMMEND;
static ACM_ENHANCE_E     sg_enAcmEnhance        = ACM_ENHANCE_SPEC_COLOR_MODE;

static SIX_BASE_COLOR_OFFSET_S sg_stSixbaseColorOffset = {50, 50, 50, 50, 50, 50};
static HUE_RANG_S stSixColorRange[SIX_BASE_COLOR_BUTT] =
{
    { 3, 11}, /* R         */
    {21, 23}, /* G         */
    { 0,  1}, /* B1        */
    {26, 28}, /* B2        */
    {24, 26}, /* CYAN      */
    { 2,  3}, /* MAGENTA   */
    {14, 20}, /* YELLOW    */
    { 0, 28}, /* COLOR_ALL */
};

HI_S32 PQ_MNG_DeInitColor(HI_VOID)
{
    if (sg_bColorInitial == HI_FALSE)
    {
        return HI_SUCCESS;
    }
    PQ_HAL_DeInitACM();

    sg_pstAcmCoef    = HI_NULL;
    sg_bColorInitial = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_FindAcmGain(HI_VOID)
{
    HI_U32 i, u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8SourceMode;
    PQ_PHY_REG_S *sg_pstPhyReg   = HI_NULL;
    HI_U32 u32StartPos = 0;
    HI_U32 u32EndPos   = 0;

    PQ_CHECK_NULL_PTR(sg_pstPqParam);
    sg_pstPhyReg = sg_pstPqParam->stPQPhyReg;

    PQ_TABLE_FindBinPos(HI_PQ_MODULE_COLOR, &u32StartPos, &u32EndPos);

    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;

        if (u32Addr != ACM_GAIN_ADDR)
        {
            continue;
        }

        if (u8SourceMode > SOURCE_MODE_UHD)
        {
            continue;
        }

        if ((ACM_GAIN_LUMA_LSB == u8Lsb) && (ACM_GAIN_LUMA_MSB == u8Msb))
        {
            sg_u32ColorGain[u8SourceMode][FLE_GAIN_MID][ACM_EH_GAIN_LUMA] = u32Value;
            pqprint(PQ_PRN_ACM, "Get ACM GAIN LUMA:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }
        else if ((ACM_GAIN_HUE_LSB == u8Lsb) && (ACM_GAIN_HUE_MSB == u8Msb))
        {
            sg_u32ColorGain[u8SourceMode][FLE_GAIN_MID][ACM_EH_GAIN_HUE] = u32Value;
            pqprint(PQ_PRN_ACM, "Get ACM GAIN HUE:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }
        else if ((ACM_GAIN_SAT_LSB == u8Lsb) && (ACM_GAIN_SAT_MSB == u8Msb))
        {
            sg_u32ColorGain[u8SourceMode][FLE_GAIN_MID][ACM_EH_GAIN_SAT] = u32Value;
            pqprint(PQ_PRN_ACM, "Get ACM GAIN SAT:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }
    }

    return HI_SUCCESS;
}

/* Init Color */
static HI_S32 PQ_MNG_ACMLUTInit(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    PQ_ACM_LUT_S *pstAcmModeRecmd = HI_NULL;

    PQ_CHECK_NULL_PTR(pstPqParam);
    sg_pstPqParam = pstPqParam;
    sg_pstAcmCoef = &(pstPqParam->stPQCoef.stAcmCoef);

    if (HI_TRUE == bDefault)
    {
        /* set default ACM Lut */
        memcpy(sg_pstAcmCoef->stModeLut.stModeRecmd.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeRecmd.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeRecmd.as16S, s16SatLut, sizeof(s16SatLut));

        memcpy(sg_pstAcmCoef->stModeLut.stModeBlue.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeBlue.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeBlue.as16S, s16SatLut, sizeof(s16SatLut));

        memcpy(sg_pstAcmCoef->stModeLut.stModeGreen.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeGreen.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeGreen.as16S, s16SatLut, sizeof(s16SatLut));

        memcpy(sg_pstAcmCoef->stModeLut.stModeBG.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeBG.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeBG.as16S, s16SatLut, sizeof(s16SatLut));

        memcpy(sg_pstAcmCoef->stModeLut.stModeFleshtone.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeFleshtone.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stModeFleshtone.as16S, s16SatLut, sizeof(s16SatLut));

        memcpy(sg_pstAcmCoef->stModeLut.stMode6BColor.as16Y, s16LumaLut, sizeof(s16LumaLut));
        memcpy(sg_pstAcmCoef->stModeLut.stMode6BColor.as16H, s16HueLut, sizeof(s16HueLut));
        memcpy(sg_pstAcmCoef->stModeLut.stMode6BColor.as16S, s16SatLut, sizeof(s16SatLut));
    }

    pstAcmModeRecmd = (void *)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_ACM_LUT_S), GFP_KERNEL);
    if (HI_NULL == pstAcmModeRecmd)
    {
        HI_ERR_PQ("%s:pstAcmModeRecmd is Null poniter!\n", __FUNCTION__);
        return HI_FAILURE;
    }

    memcpy(pstAcmModeRecmd, &(sg_pstAcmCoef->stModeLut.stModeRecmd), sizeof(PQ_ACM_LUT_S));
    s32Ret = PQ_HAL_SetACMParam((COLOR_ACM_LUT_S *)pstAcmModeRecmd);
    if (HI_FAILURE == s32Ret)
    {
        HI_KFREE(HI_ID_PQ, pstAcmModeRecmd);
        HI_ERR_PQ("PQ_HAL_SetACMParam failure\n");
        return HI_FAILURE;
    }

    HI_KFREE(HI_ID_PQ, pstAcmModeRecmd);

    return HI_SUCCESS;
}

/* Init Color */
HI_S32 PQ_MNG_InitColor(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;

    if (sg_bColorInitial == HI_TRUE)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_HAL_InitACM();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("ACM HAL Init error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_ACMLUTInit(pstPqParam, bDefault);
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("ACM LUT Init failure\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_TABLE_InitPhyList(u32ID, HI_PQ_MODULE_COLOR, SOURCE_MODE_HD, OUTPUT_MODE_HD);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("ACM InitPhyList error\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_FindAcmGain();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("ACM FindAcmGain error\n");
        return HI_FAILURE;
    }

    sg_bColorInitial = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetColorGainLevel(HI_U32 *pu32ColorLevel)
{
    PQ_CHECK_NULL_PTR(pu32ColorLevel);

    *pu32ColorLevel = sg_u32ColorLevel;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetColorGainLevel(HI_U32 u32GainLevel)
{
    HI_U32 u32Gainluma = 0;
    HI_U32 u32Gainhue  = 0;
    HI_U32 u32GainSat  = 0;

    if (u32GainLevel > 100)
    {
        HI_ERR_PQ("%d:ColorGainLevel is over range!\n", u32GainLevel);
        return HI_FAILURE;
    }

    PQ_HAL_SetACMStretch(VDP_LAYER_VP0, 1);
    PQ_HAL_SetACMClipRange(VDP_LAYER_VP0, 1);
    PQ_HAL_SetACMCbcrThd(VDP_LAYER_VP0, 0);

    PQ_HAL_GetACMGain(VDP_LAYER_VP0, &u32Gainluma, &u32Gainhue, &u32GainSat);
    PQ_HAL_SetACMGain(VDP_LAYER_VP0, u32Gainluma, u32Gainhue, LEVEL2NUM(u32GainLevel));
    //PQ_HAL_EnableACM(VDP_LAYER_VP0, HI_TRUE);

    sg_u32ColorLevel = u32GainLevel;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetFleshToneLevel(HI_U32 *pu32FleshToneLevel)
{
    PQ_CHECK_NULL_PTR(pu32FleshToneLevel);

    *pu32FleshToneLevel = sg_enFleshLevel;

    return HI_SUCCESS;
}

static HI_VOID PQ_MNG_SetColorGainOffset(HI_U32 u32InMode)
{
    switch (u32InMode)
    {
        case SOURCE_MODE_NO:
            sg_u32ColorGain[SOURCE_MODE_NO][FLE_GAIN_LOW][ACM_EH_GAIN_LUMA] = sg_u32ColorGain[SOURCE_MODE_NO][FLE_GAIN_MID][ACM_EH_GAIN_LUMA] - ACM_GAIN_OFFSET_LOW_HD;
            sg_u32ColorGain[SOURCE_MODE_NO][FLE_GAIN_HIGH][ACM_EH_GAIN_SAT] = sg_u32ColorGain[SOURCE_MODE_NO][FLE_GAIN_MID][ACM_EH_GAIN_SAT]  + ACM_GAIN_OFFSET_HIGH_HD;
            break;
        case SOURCE_MODE_SD:
            sg_u32ColorGain[SOURCE_MODE_SD][FLE_GAIN_LOW][ACM_EH_GAIN_LUMA] = sg_u32ColorGain[SOURCE_MODE_SD][FLE_GAIN_MID][ACM_EH_GAIN_LUMA] - ACM_GAIN_OFFSET_LOW_SD;
            sg_u32ColorGain[SOURCE_MODE_SD][FLE_GAIN_HIGH][ACM_EH_GAIN_SAT] = sg_u32ColorGain[SOURCE_MODE_SD][FLE_GAIN_MID][ACM_EH_GAIN_SAT]  + ACM_GAIN_OFFSET_HIGH_SD;
            break;
        case SOURCE_MODE_HD:
            sg_u32ColorGain[SOURCE_MODE_HD][FLE_GAIN_LOW][ACM_EH_GAIN_LUMA] = sg_u32ColorGain[SOURCE_MODE_HD][FLE_GAIN_MID][ACM_EH_GAIN_LUMA] - ACM_GAIN_OFFSET_LOW_HD;
            sg_u32ColorGain[SOURCE_MODE_HD][FLE_GAIN_HIGH][ACM_EH_GAIN_SAT] = sg_u32ColorGain[SOURCE_MODE_HD][FLE_GAIN_MID][ACM_EH_GAIN_SAT]  + ACM_GAIN_OFFSET_HIGH_HD;
            break;
        case SOURCE_MODE_UHD:
            sg_u32ColorGain[SOURCE_MODE_UHD][FLE_GAIN_LOW][ACM_EH_GAIN_LUMA] = sg_u32ColorGain[SOURCE_MODE_UHD][FLE_GAIN_MID][ACM_EH_GAIN_LUMA] - ACM_GAIN_OFFSET_LOW_UHD;
            sg_u32ColorGain[SOURCE_MODE_UHD][FLE_GAIN_HIGH][ACM_EH_GAIN_SAT] = sg_u32ColorGain[SOURCE_MODE_UHD][FLE_GAIN_MID][ACM_EH_GAIN_SAT]  + ACM_GAIN_OFFSET_HIGH_UHD;
            break;
        default:
            break;
    }

    return;
}

HI_S32 PQ_MNG_SetACMGain(HI_VOID)
{
    HI_U32 u32GainLuma = 0;
    HI_U32 u32GainHue  = 0;
    HI_U32 u32GainSat  = 0;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if (ACM_ENHANCE_FLESHTONE == sg_enAcmEnhance)
    {
        PQ_MNG_SetColorGainOffset(enSourceMode);

        u32GainLuma = sg_u32ColorGain[enSourceMode][sg_enFleshLevel][ACM_EH_GAIN_LUMA];
        u32GainHue  = sg_u32ColorGain[enSourceMode][sg_enFleshLevel][ACM_EH_GAIN_HUE];
        u32GainSat  = sg_u32ColorGain[enSourceMode][sg_enFleshLevel][ACM_EH_GAIN_SAT];
    }
    else
    {
        u32GainLuma = sg_u32ColorGain[enSourceMode][COLOR_GAIN_MID][ACM_EH_GAIN_LUMA];
        u32GainHue  = sg_u32ColorGain[enSourceMode][COLOR_GAIN_MID][ACM_EH_GAIN_HUE];
        u32GainSat  = sg_u32ColorGain[enSourceMode][COLOR_GAIN_MID][ACM_EH_GAIN_SAT];
    }

    PQ_HAL_SetACMGain(VDP_LAYER_VP0, u32GainLuma, u32GainHue, u32GainSat);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetFleshToneLevel(HI_PQ_FLESHTONE_E enGainLevel)
{
    HI_S32 s32Ret;
    FLESHTONE_LEVEL_E GainLevel = (FLESHTONE_LEVEL_E)enGainLevel;
    PQ_ACM_LUT_S *pstColorParam = HI_NULL;

    if (GainLevel >= FLE_GAIN_BUTT)
    {
        HI_ERR_PQ("[%d]Flesh tone level range error!\n", GainLevel);
        return HI_FAILURE;
    }

    PQ_CHECK_NULL_PTR(sg_pstAcmCoef);
    pstColorParam = (void *)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_ACM_LUT_S), GFP_ATOMIC);
    if (HI_NULL == pstColorParam)
    {
        HI_ERR_PQ("%s:pstColorParam is Null poniter!\n", __FUNCTION__);
        return HI_FAILURE;
    }
    memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeFleshtone), sizeof(PQ_ACM_LUT_S));

    sg_enFleshLevel = GainLevel;
    sg_enAcmEnhance = ACM_ENHANCE_FLESHTONE;

    PQ_MNG_SetACMGain();

    s32Ret = PQ_HAL_SetACMParam((COLOR_ACM_LUT_S *)pstColorParam);
    if (HI_FAILURE == s32Ret)
    {
        HI_KFREE(HI_ID_PQ, pstColorParam);
        HI_ERR_PQ("PQ_MNG_SetFleshToneLevel failure\n");
        return HI_FAILURE;
    }

    HI_KFREE(HI_ID_PQ, pstColorParam);

    return HI_SUCCESS;
}

/* Connect to Saturation curve, When it is 50, Saturation curve equal to 0
   When it is 100,the curve is positive,0 it is negative
   */
HI_S32 PQ_MNG_SetColorSatGain(COLOR_ACM_LUT_S  *pstColorParam, HI_S32 s32HueStart, HI_S32 s32HueEnd, HI_S32 s32Sat)
{
    HI_S32 i, j, k;

    if (HI_NULL == pstColorParam)
    {
        HI_ERR_PQ("pstColorParam is NULL!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < ACM_Y_NUM; i++)
    {
        for (j = 0; j < ACM_S_NUM; j++)
        {
            for (k = 0; k < ACM_H_NUM; k++)
            {
                //HI_INFO_PQ("Sat_old[%d][%d][%d]=%d\t", i, j, k, pstColorParam->as16Sat[i][j][k]);
                if ((k >= s32HueStart) && (k <= s32HueEnd))
                {
                    pstColorParam->as16Sat[i][j][k] = (pstColorParam->as16Sat[i][j][k]) * (s32Sat - 50) / 50;
                }
                //HI_INFO_PQ("Sat_new[%d][%d][%d]=%d\n", i, j, k, pstColorParam->as16Sat[i][j][k]);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetSixBaseColorLevel(HI_PQ_SIX_BASE_S *pstSixBaseColorOffset)
{
    PQ_CHECK_NULL_PTR(pstSixBaseColorOffset);

    pstSixBaseColorOffset->u32Red     = sg_stSixbaseColorOffset.u32RedOffset;
    pstSixBaseColorOffset->u32Green   = sg_stSixbaseColorOffset.u32GreenOffset;
    pstSixBaseColorOffset->u32Blue    = sg_stSixbaseColorOffset.u32BlueOffset;
    pstSixBaseColorOffset->u32Cyan    = sg_stSixbaseColorOffset.u32CyanOffset;
    pstSixBaseColorOffset->u32Magenta = sg_stSixbaseColorOffset.u32MagentaOffset;
    pstSixBaseColorOffset->u32Yellow  = sg_stSixbaseColorOffset.u32YellowOffset;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSixBaseColorLevel(HI_PQ_SIX_BASE_S *pstSixBaseColorOffset)
{
    HI_S32 s32Ret;
    PQ_ACM_LUT_S  *pstColorParam = NULL;

    PQ_CHECK_NULL_PTR(sg_pstAcmCoef);
    PQ_CHECK_NULL_PTR(pstSixBaseColorOffset);

    if ((pstSixBaseColorOffset->u32Red > 100)
        || (pstSixBaseColorOffset->u32Green > 100)
        || (pstSixBaseColorOffset->u32Blue > 100)
        || (pstSixBaseColorOffset->u32Cyan > 100)
        || (pstSixBaseColorOffset->u32Magenta > 100)
        || (pstSixBaseColorOffset->u32Yellow > 100))
    {
        HI_ERR_PQ("out of range\n");
        return HI_FAILURE;
    }

    sg_stSixbaseColorOffset.u32RedOffset     = pstSixBaseColorOffset->u32Red;
    sg_stSixbaseColorOffset.u32GreenOffset   = pstSixBaseColorOffset->u32Green;
    sg_stSixbaseColorOffset.u32BlueOffset    = pstSixBaseColorOffset->u32Blue;
    sg_stSixbaseColorOffset.u32CyanOffset    = pstSixBaseColorOffset->u32Cyan;
    sg_stSixbaseColorOffset.u32MagentaOffset = pstSixBaseColorOffset->u32Magenta;
    sg_stSixbaseColorOffset.u32YellowOffset  = pstSixBaseColorOffset->u32Yellow;

    pstColorParam = (void *)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_ACM_LUT_S), GFP_ATOMIC);
    if (pstColorParam == NULL)
    {
        HI_ERR_PQ("pstColorParam is Null poniter!\n");
        return HI_FAILURE;
    }

    memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stMode6BColor), sizeof(PQ_ACM_LUT_S));

    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_R].u32HueStart, stSixColorRange[SIX_BASE_COLOR_R].u32HueEnd, sg_stSixbaseColorOffset.u32RedOffset);
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_G].u32HueStart, stSixColorRange[SIX_BASE_COLOR_G].u32HueEnd, sg_stSixbaseColorOffset.u32GreenOffset);
    /* if 0 and 28 is different, it may result in contor in blue */
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_B1].u32HueStart, stSixColorRange[SIX_BASE_COLOR_B1].u32HueEnd, sg_stSixbaseColorOffset.u32BlueOffset);
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_B2].u32HueStart, stSixColorRange[SIX_BASE_COLOR_B2].u32HueEnd, sg_stSixbaseColorOffset.u32BlueOffset);
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_CYAN].u32HueStart, stSixColorRange[SIX_BASE_COLOR_CYAN].u32HueEnd, sg_stSixbaseColorOffset.u32CyanOffset);
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_MAGENTA].u32HueStart, stSixColorRange[SIX_BASE_COLOR_MAGENTA].u32HueEnd, sg_stSixbaseColorOffset.u32MagentaOffset);
    PQ_MNG_SetColorSatGain((COLOR_ACM_LUT_S *)pstColorParam, stSixColorRange[SIX_BASE_COLOR_YELLOW].u32HueStart, stSixColorRange[SIX_BASE_COLOR_YELLOW].u32HueEnd, sg_stSixbaseColorOffset.u32YellowOffset);

    sg_enAcmEnhance = ACM_ENHANCE_SIX_BASE;

    PQ_MNG_SetACMGain();

    s32Ret = PQ_HAL_SetACMParam((COLOR_ACM_LUT_S *)pstColorParam);
    if (HI_FAILURE == s32Ret )
    {
        HI_KFREE(HI_ID_PQ, pstColorParam);
        HI_ERR_PQ("PQ_HAL_SetACMParam failure\n");
        return HI_FAILURE;
    }

    HI_KFREE(HI_ID_PQ, pstColorParam);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetColorEnhanceMode(HI_U32 *pu32ColorEnhanceMode)
{
    PQ_CHECK_NULL_PTR(pu32ColorEnhanceMode);

    *pu32ColorEnhanceMode = sg_enColorSpecModeType;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetColorEnhanceMode(HI_PQ_COLOR_SPEC_MODE_E enColorSpecMode)
{
    HI_S32 s32Ret;
    PQ_ACM_LUT_S  *pstColorParam = NULL;
    COLOR_SPEC_MODE_E ColorSpecMode = (COLOR_SPEC_MODE_E)enColorSpecMode;


    if (COLOR_MODE_BUTT <= ColorSpecMode)
    {
        HI_ERR_PQ("[%d]Color Enhance Mode Set error!\n", ColorSpecMode);
        return HI_FAILURE;
    }

    PQ_CHECK_NULL_PTR(sg_pstAcmCoef);

    pstColorParam = (void *)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_ACM_LUT_S), GFP_ATOMIC);
    PQ_CHECK_NULL_PTR(pstColorParam);

    switch (ColorSpecMode)
    {
        case COLOR_MODE_RECOMMEND:
            memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeRecmd), sizeof(PQ_ACM_LUT_S));
            break;
        case COLOR_MODE_BLUE:
            memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeBlue), sizeof(PQ_ACM_LUT_S));
            break;
        case COLOR_MODE_GREEN:
            memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeGreen), sizeof(PQ_ACM_LUT_S));
            break;
        case COLOR_MODE_BG:
            memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeBG), sizeof(PQ_ACM_LUT_S));
            break;
        case COLOR_MODE_ORIGINAL:
            memset(pstColorParam, 0, sizeof(PQ_ACM_LUT_S));
            break;
        default:
            memcpy(pstColorParam, &(sg_pstAcmCoef->stModeLut.stModeRecmd), sizeof(PQ_ACM_LUT_S));
            break;
    }

    sg_enAcmEnhance   = ACM_ENHANCE_SPEC_COLOR_MODE;

    s32Ret = PQ_HAL_SetACMParam((COLOR_ACM_LUT_S *)pstColorParam);
    if (s32Ret == HI_FAILURE)
    {
        HI_KFREE(HI_ID_PQ, pstColorParam);
        HI_ERR_PQ("PQ_MNG_SetColorEnhanceMode failure\n");
        return HI_FAILURE;
    }

    HI_KFREE(HI_ID_PQ, pstColorParam);
    sg_enColorSpecModeType = ColorSpecMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetACMLumaTbl(HI_PQ_COLOR_LUT_S *pstColorData)
{
    PQ_CHECK_NULL_PTR(pstColorData);
    return PQ_HAL_GetACMLumaTbl((COLOR_LUT_S *) pstColorData);
}

HI_S32 PQ_MNG_GetACMHueTbl(HI_PQ_COLOR_LUT_S *pstColorData)
{
    PQ_CHECK_NULL_PTR(pstColorData);
    return PQ_HAL_GetACMHueTbl((COLOR_LUT_S *) pstColorData);
}

HI_S32 PQ_MNG_GetACMSatTbl(HI_PQ_COLOR_LUT_S *pstColorData)
{
    PQ_CHECK_NULL_PTR(pstColorData);
    return PQ_HAL_GetACMSatTbl((COLOR_LUT_S *) pstColorData);
}

HI_S32 PQ_MNG_SetAcmLuma(HI_PQ_ACM_LUT_S *pstAttr)
{
    HI_S32 s32Ret;
    HI_U32 u32RegCmd;
    PQ_ACM_MODE_S *pstModeLut;

    PQ_CHECK_NULL_PTR(pstAttr);
    PQ_CHECK_NULL_PTR(sg_pstPqParam);

    u32RegCmd = pstAttr->u32LutType;
    pstModeLut = &(sg_pstPqParam->stPQCoef.stAcmCoef.stModeLut);

    switch (u32RegCmd)
    {
        case PQ_CMD_VIRTUAL_ACM_RECMD_LUT:
            memcpy(pstModeLut->stModeRecmd.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BLUE_LUT:
            memcpy(pstModeLut->stModeBlue.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_GREEN_LUT:
            memcpy(pstModeLut->stModeGreen.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BG_LUT:
            memcpy(pstModeLut->stModeBG.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_FLESH_LUT:
            memcpy(pstModeLut->stModeFleshtone.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_6BCOLOR_LUT:
            memcpy(pstModeLut->stMode6BColor.as16Y, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        default:
            HI_ERR_PQ("%s:u32RegCmd[0x%x] is error!\n", __FUNCTION__, u32RegCmd);
            break;
    }

    s32Ret = PQ_HAL_SetACMLumaTbl((COLOR_LUT_S *)pstAttr->as16Lut);

    return s32Ret;
}

HI_S32 PQ_MNG_SetAcmHue(HI_PQ_ACM_LUT_S *pstAttr)
{
    HI_S32 s32Ret;
    HI_U32 u32RegCmd;
    PQ_ACM_MODE_S *pstModeLut;

    PQ_CHECK_NULL_PTR(pstAttr);
    PQ_CHECK_NULL_PTR(sg_pstPqParam);

    u32RegCmd = pstAttr->u32LutType;
    pstModeLut = &(sg_pstPqParam->stPQCoef.stAcmCoef.stModeLut);

    switch (u32RegCmd)
    {
        case PQ_CMD_VIRTUAL_ACM_RECMD_LUT:
            memcpy(pstModeLut->stModeRecmd.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BLUE_LUT:
            memcpy(pstModeLut->stModeBlue.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_GREEN_LUT:
            memcpy(pstModeLut->stModeGreen.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BG_LUT:
            memcpy(pstModeLut->stModeBG.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_FLESH_LUT:
            memcpy(pstModeLut->stModeFleshtone.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_6BCOLOR_LUT:
            memcpy(pstModeLut->stMode6BColor.as16H, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        default:
            HI_ERR_PQ("%s:u32RegCmd[0x%x] is error!\n", __FUNCTION__, u32RegCmd);
            break;
    }

    s32Ret = PQ_HAL_SetACMHueTbl((COLOR_LUT_S *)pstAttr->as16Lut);

    return s32Ret;
}


HI_S32 PQ_MNG_SetAcmSat(HI_PQ_ACM_LUT_S *pstAttr)
{
    HI_S32 s32Ret;
    HI_U32 u32RegCmd;
    PQ_ACM_MODE_S *pstModeLut;

    PQ_CHECK_NULL_PTR(pstAttr);
    PQ_CHECK_NULL_PTR(sg_pstPqParam);

    u32RegCmd = pstAttr->u32LutType;
    pstModeLut = &(sg_pstPqParam->stPQCoef.stAcmCoef.stModeLut);

    switch (u32RegCmd)
    {
        case PQ_CMD_VIRTUAL_ACM_RECMD_LUT:
            memcpy(pstModeLut->stModeRecmd.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BLUE_LUT:
            memcpy(pstModeLut->stModeBlue.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_GREEN_LUT:
            memcpy(pstModeLut->stModeGreen.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_BG_LUT:
            memcpy(pstModeLut->stModeBG.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_FLESH_LUT:
            memcpy(pstModeLut->stModeFleshtone.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        case PQ_CMD_VIRTUAL_ACM_6BCOLOR_LUT:
            memcpy(pstModeLut->stMode6BColor.as16S, pstAttr->as16Lut, sizeof(COLOR_LUT_S));
            break;
        default:
            HI_ERR_PQ("%s:u32RegCmd[0x%x] is error!\n", __FUNCTION__, u32RegCmd);
            break;
    }

    s32Ret = PQ_HAL_SetACMSatTbl((COLOR_LUT_S *)pstAttr->as16Lut);

    return s32Ret;
}

HI_S32 PQ_MNG_EnableACMDemo(HI_BOOL bOnOff)
{
    PQ_HAL_EnableACMDemo(VDP_LAYER_VP0, bOnOff);
    sg_bColorDemoEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetACMDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bColorDemoEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableColorEnhance(HI_BOOL bOnOff)
{
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_COLOR, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    PQ_HAL_EnableACM(VDP_LAYER_VP0, bOnOff);
    sg_bColorEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetColorEnable(HI_U32 *pu32OnOff)
{
    PQ_CHECK_NULL_PTR(pu32OnOff);
    *pu32OnOff = sg_bColorEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetACMDemoMode(PQ_DEMO_MODE_E enMode)
{
    PQ_HAL_LAYER_VP_E u32ChId = VDP_LAYER_VP0;
    ACM_DEMO_MODE_E enAcmMode = ACM_DEMO_ENABLE_L;

    sg_enMode = enMode;

    if (enMode == PQ_DEMO_ENABLE_L)
    {
        enAcmMode = ACM_DEMO_ENABLE_L;
    }
    else if (enMode == PQ_DEMO_ENABLE_R)
    {
        enAcmMode = ACM_DEMO_ENABLE_R;
    }

    PQ_HAL_SetACMDemoMode(u32ChId, enAcmMode);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetACMDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetACMDemoModeCoor(HI_U32 u32XPos)
{
    return PQ_HAL_SetACMDemoPos(PQ_VDP_LAYER_VID0, u32XPos);
}

HI_S32 PQ_MNG_UpdatACMCoef(PQ_LAYER_VP_E enLayer)
{
    PQ_HAL_UpdatACMCoef((PQ_HAL_LAYER_VP_E)enLayer);
    return HI_SUCCESS;
}

static stPQAlgFuncs stACMFuncs =
{
    .Init                 = PQ_MNG_InitColor,
    .DeInit               = PQ_MNG_DeInitColor,
    .SetEnable            = PQ_MNG_EnableColorEnhance,
    .GetEnable            = PQ_MNG_GetColorEnable,
    .SetDemo              = PQ_MNG_EnableACMDemo,
    .GetDemo              = PQ_MNG_GetACMDemo,
    .SetDemoMode          = PQ_MNG_SetACMDemoMode,
    .GetDemoMode          = PQ_MNG_GetACMDemoMode,
    .SetDemoModeCoor      = PQ_MNG_SetACMDemoModeCoor,
    .SetStrength          = PQ_MNG_SetColorGainLevel,
    .GetStrength          = PQ_MNG_GetColorGainLevel,
    .SetFleshToneLevel    = PQ_MNG_SetFleshToneLevel,
    .GetFleshToneLevel    = PQ_MNG_GetFleshToneLevel,
    .SetACMGain           = PQ_MNG_SetACMGain,
    .SetSixBaseColorLevel = PQ_MNG_SetSixBaseColorLevel,
    .GetSixBaseColorLevel = PQ_MNG_GetSixBaseColorLevel,
    .SetColorEnhanceMode  = PQ_MNG_SetColorEnhanceMode,
    .GetColorEnhanceMode  = PQ_MNG_GetColorEnhanceMode,
    .UpdatACMCoef         = PQ_MNG_UpdatACMCoef,
};

HI_S32 PQ_MNG_RegisterACM(PQ_REG_TYPE_E enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_COLOR, enType, PQ_BIN_ADAPT_SINGLE, "acm", HI_NULL, &stACMFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterACM(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_COLOR);

    return s32Ret;
}

