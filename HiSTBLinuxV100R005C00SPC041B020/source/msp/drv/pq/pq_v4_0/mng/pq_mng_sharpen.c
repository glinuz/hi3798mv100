/******************************************************************************
 Copyright (C), 2014-2016, Hisilicon Tech. Co. Ltd.
******************************************************************************
File Name     : pq_mng_sharpen.c
Version       : Initial Draft
Author        : d00241380
Created       : 2014/06/19
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <linux/slab.h>      /* kmalloc() */
#include <linux/string.h>    /* memcpy() */
#include <linux/fcntl.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "drv_pq_define.h"

#include "pq_hal_comm.h"
#include "pq_hal_sharpen.h"
#include "pq_hal_comm.h"
#include "drv_pq_table.h"

#define S10TOS16(x) ((x>511)?(x-1024):(x))

static HI_BOOL sg_bSharpen_EnFlag;
static HI_BOOL sg_bSharpenDemoEn    = HI_FALSE;
static HI_BOOL sg_bShpInitFlag      = HI_FALSE;
static HI_U32  sg_u32SharpStr       = 50; /* 0~100 */
static PQ_PARAM_S *sg_pstPqParam    = HI_NULL;
static PQ_DEMO_MODE_E sg_enMode     = PQ_DEMO_ENABLE_R;


static HI_U32 sg_u32Gain1[SOURCE_MODE_ALL][OUTPUT_MODE_ALL] =
{
    {256, 256, 256, 256, 160, 160}, /* SOURCE_MODE_NO  */
    {256, 256, 256, 256, 160, 160}, /* SOURCE_MODE_SD  */
    {256, 256, 256, 256, 160, 160}, /* SOURCE_MODE_HD  */
    {80,  80,   80,  80, 80, 80}, /* SOURCE_MODE_UHD */
};

static HI_U32 sg_u32Gain2[SOURCE_MODE_ALL][OUTPUT_MODE_ALL] =
{
    {128, 128, 128, 128, 160, 160}, /* SOURCE_MODE_NO  */
    {128, 128, 128, 128, 160, 160}, /* SOURCE_MODE_SD  */
    {128, 128, 128, 128, 160, 160}, /* SOURCE_MODE_HD  */
    { 40,  40,  40,  40, 40,  40}, /* SOURCE_MODE_UHD */
};

static HI_S32 PQ_MNG_FindSharpenGainRatio(HI_U32 ppu32Gain[][OUTPUT_MODE_ALL], HI_U32 u32RegAddr, HI_U8 u8GainLsb, HI_U8 u8GainMsb)
{
    HI_U32 u32Addr, u32Value;
    PQ_PHY_REG_S *sg_pstPhyReg   = HI_NULL;
    HI_U32 i;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    HI_U32 u32StartPos = 0;
    HI_U32 u32EndPos   = 0;

    PQ_CHECK_NULL_PTR(sg_pstPqParam);
    sg_pstPhyReg = sg_pstPqParam->stPQPhyReg;

    PQ_TABLE_FindBinPos(HI_PQ_MODULE_SHARPNESS, &u32StartPos, &u32EndPos);

    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;
        u8OutputMode  = sg_pstPhyReg[i].u8OutputMode;

        if (u32Addr != u32RegAddr)
        {
            continue;
        }
        if (u8Lsb != u8GainLsb)
        {
            continue;
        }
        if (u8Msb != u8GainMsb)
        {
            continue;
        }

        if ((SOURCE_MODE_ALL <= u8SourceMode) || (OUTPUT_MODE_ALL <= u8OutputMode))
        {
            continue;
        }

        ppu32Gain[u8SourceMode][u8OutputMode] = u32Value;

        pqprint(PQ_PRN_SHARPEN, "GetSharpenGainRatio:%d, SourceMode:%d, OutputMode:%d\n", u32Value, u8SourceMode, u8OutputMode);
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_FindSharpGain(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret  = PQ_MNG_FindSharpenGainRatio(sg_u32Gain1, SHARP_PEAK_GAIN_ADDR, SHARP_PEAK_GAIN_LSB, SHARP_PEAK_GAIN_MSB);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(sg_u32Gain2, SHARP_EDGE_GAIN_ADDR, SHARP_EDGE_GAIN_LSB, SHARP_EDGE_GAIN_MSB);
    return s32Ret;
}

/* Deinit Sharp Moudle */
HI_S32 PQ_MNG_DeinitSharpen(HI_VOID)
{
    if (HI_FALSE == sg_bShpInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bShpInitFlag = HI_FALSE;
    return HI_SUCCESS;
}

/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_InitSharpen(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32Data = 0;

    sg_pstPqParam = pstPqParam;

    if (HI_TRUE == sg_bShpInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bSharpen_EnFlag = HI_FALSE;

    s32Ret = PQ_TABLE_InitPhyList(u32Data, HI_PQ_MODULE_SHARPNESS, SOURCE_MODE_HD, OUTPUT_MODE_NO);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Sharpen InitPhyList error\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_FindSharpGain();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("FindSharpGain error\n");
        return HI_FAILURE;
    }

    sg_bShpInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Set Sharpen Strength, Range: 0~100;
   说明: UI菜单设置直接调用，根据当前的源和输出信息将设置的s32ShpStr(0-100)值混合Bin文件的Gain值设置Sharpen强度
   */
HI_S32 PQ_MNG_SetSharpenStr(HI_U32 u32ShpStr)
{
    HI_U32 u32ShpStrTmp;
    HI_U32 u32PeakGain, u32EdgeGain;
    PQ_SHARP_GAIN_S stSharpGain;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();
    PQ_OUTPUT_MODE_E enOutMode = PQ_COMM_GetOutputMode();

    if (100 < u32ShpStr)
    {
        HI_ERR_PQ("the value of setting sharpen strength [%d] is out of range!\n", u32ShpStr);
        return HI_FAILURE;
    }
    sg_u32SharpStr = u32ShpStr;
    u32ShpStrTmp = LEVEL2NUM(u32ShpStr);

    if (enSourceMode >= SOURCE_MODE_ALL)
    {
        HI_ERR_PQ("source mode [%d] out of range!\n", enSourceMode);
        return HI_FAILURE;
    }

    if (enOutMode >= OUTPUT_MODE_ALL)
    {
        HI_ERR_PQ("output mode [%d] out of range!\n", enOutMode);
        return HI_FAILURE;
    }
    stSharpGain.u32PeakGain = sg_u32Gain1[enSourceMode][enOutMode];
    stSharpGain.u32EdgeGain = sg_u32Gain2[enSourceMode][enOutMode];

    u32PeakGain = (stSharpGain.u32PeakGain * u32ShpStrTmp + 64) / 128;
    u32EdgeGain = (stSharpGain.u32EdgeGain * u32ShpStrTmp + 64) / 128;

    PQ_HAL_SetSharpStrReg(PQ_VDP_LAYER_VID0, u32PeakGain, u32EdgeGain);

    return HI_SUCCESS;
}

/* Get Sharpen Strength, Range: 0~100 */
HI_S32 PQ_MNG_GetSharpenStr(HI_U32 *pu32ShpStr)
{
    *pu32ShpStr = sg_u32SharpStr;

    return HI_SUCCESS;
}

/* Enable Sharpen Moudle(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpen(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    sg_bSharpen_EnFlag = bOnOff;

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_SHARPNESS, enSourceMode)))
    {
        sg_bSharpen_EnFlag = HI_FALSE;
    }

    s32Ret = PQ_HAL_EnableSharp(sg_bSharpen_EnFlag);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in sharpen enable!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Get Sharpen Enable(Sharpen and HSharpen) */
HI_S32 PQ_MNG_GetSharpenEnable(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);

    *pbOnOff = sg_bSharpen_EnFlag;

    return HI_SUCCESS;
}

/* Enable Sharpen Demo(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpenDemo(HI_BOOL bOnOff)
{
    sg_bSharpenDemoEn = bOnOff;

    return PQ_HAL_EnableSharpDemo(PQ_VDP_LAYER_VID0, bOnOff);
}

HI_S32 PQ_MNG_GetSharpenDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bSharpenDemoEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSharpDemoMode(PQ_DEMO_MODE_E enMode)
{
    SHARP_DEMO_MODE_E enSharpMode = SHARP_DEMO_ENABLE_L;

    sg_enMode = enMode;

    if (enMode == PQ_DEMO_ENABLE_L)
    {
        enSharpMode = SHARP_DEMO_ENABLE_L;
    }
    else if (enMode == PQ_DEMO_ENABLE_R)
    {
        enSharpMode = SHARP_DEMO_ENABLE_R;
    }

    PQ_HAL_SetSharpDemoMode(PQ_VDP_LAYER_VID0, enSharpMode);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetSharpDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSharpDemoModeCoor(HI_U32 u32XPos)
{
    return PQ_HAL_SetSharpDemoPos(PQ_VDP_LAYER_VID0, u32XPos);
}

static stPQAlgFuncs stSharpFuncs =
{
    .Init            = PQ_MNG_InitSharpen,
    .DeInit          = PQ_MNG_DeinitSharpen,
    .SetEnable       = PQ_MNG_EnableSharpen,
    .GetEnable       = PQ_MNG_GetSharpenEnable,
    .SetDemo         = PQ_MNG_EnableSharpenDemo,
    .GetDemo         = PQ_MNG_GetSharpenDemo,
    .SetDemoMode     = PQ_MNG_SetSharpDemoMode,
    .GetDemoMode     = PQ_MNG_GetSharpDemoMode,
    .SetDemoModeCoor = PQ_MNG_SetSharpDemoModeCoor,
    .SetStrength     = PQ_MNG_SetSharpenStr,
    .GetStrength     = PQ_MNG_GetSharpenStr,
};

HI_S32 PQ_MNG_RegisterSharp(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_SHARPNESS, enType, PQ_BIN_ADAPT_MULTIPLE, "sharp", HI_NULL, &stSharpFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterSharp(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_SHARPNESS);

    return s32Ret;
}

