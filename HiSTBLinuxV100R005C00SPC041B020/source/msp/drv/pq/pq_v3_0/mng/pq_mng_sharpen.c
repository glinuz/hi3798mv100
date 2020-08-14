/******************************************************************************
 Copyright (C), 2001-2014, Hisilicon Tech. Co. Ltd.
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
#include "drv_pq_table.h"


#define S10TOS16(x) ((x>511)?(x-1024):(x))


static HI_BOOL sg_bSharpen_EnFlag;
static HI_BOOL g_bShpInitFlag      = HI_FALSE;
static HI_S32  sg_s32SharpStr      = 128;
static PQ_PARAM_S* g_pstPqParam    = HI_NULL;
static SHARP_DEMO_MODE_E sg_enMode = SHARP_DEMO_ENABLE_R;


static HI_U32 sg_u32Gain1[SOURCE_MODE_ALL][OUTPUT_MODE_ALL] =
{
    {256, 256, 256}, /* SOURCE_MODE_NO  */
    {256, 256, 256}, /* SOURCE_MODE_SD  */
    {256, 256, 256}, /* SOURCE_MODE_HD  */
    {256, 256, 256}, /* SOURCE_MODE_UHD */
};

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
static HI_U32 sg_u32Gain2[SOURCE_MODE_ALL][OUTPUT_MODE_ALL] =
{
    {304, 304, 304}, /* SOURCE_MODE_NO  */
    {304, 304, 304}, /* SOURCE_MODE_SD  */
    {304, 304, 304}, /* SOURCE_MODE_HD  */
    {304, 304, 304}, /* SOURCE_MODE_UHD */
};
#endif

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
static HI_U32 sg_u32Gain2[SOURCE_MODE_ALL][OUTPUT_MODE_ALL] =
{
    {128, 128, 128}, /* SOURCE_MODE_NO  */
    {128, 128, 128}, /* SOURCE_MODE_SD  */
    {128, 128, 128}, /* SOURCE_MODE_HD  */
    {128, 128, 128}, /* SOURCE_MODE_UHD */
};
#endif



static HI_S32 PQ_MNG_FindSharpenGainRatio(HI_U32 ppu32Gain[][4], HI_U32 u32RegAddr, HI_U8 u8GainLsb, HI_U8 u8GainMsb)
{
    HI_U32 u32Addr, u32Value;
    HI_U32 u32ListNum = PHY_REG_MAX;
    HI_U32 i;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    PQ_PHY_REG_S* pstPqMultitReg = HI_NULL;

    PQ_MNG_GetPqMultitReg(&pstPqMultitReg);
    PQ_CHECK_NULL_PTR(pstPqMultitReg);

    u32ListNum = PQ_MNG_GetMultiListNum();

    for (i = 0; i < u32ListNum; i++)
    {
        u32Addr       = (pstPqMultitReg + i)->u32RegAddr;
        u32Value      = (pstPqMultitReg + i)->u32Value;
        u8Lsb         = (pstPqMultitReg + i)->u8Lsb;
        u8Msb         = (pstPqMultitReg + i)->u8Msb;
        u8SourceMode  = (pstPqMultitReg + i)->u8SourceMode;
        u8OutputMode  = (pstPqMultitReg + i)->u8OutputMode;

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

        if ((SOURCE_MODE_UHD < u8SourceMode) || (OUTPUT_MODE_UHD < u8OutputMode))
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

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    s32Ret  = PQ_MNG_FindSharpenGainRatio(sg_u32Gain1, SHARP_LGAIN_RATIO_ADDR, SHARP_LGAIN_RATIO_LSB, SHARP_LGAIN_RATIO_MSB);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(sg_u32Gain2, SHARP_CGAIN_RATIO_ADDR, SHARP_CGAIN_RATIO_LSB, SHARP_CGAIN_RATIO_MSB);
#endif
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    s32Ret  = PQ_MNG_FindSharpenGainRatio(sg_u32Gain1, SHARP_PEAK_GAIN_ADDR, SHARP_PEAK_GAIN_LSB, SHARP_PEAK_GAIN_MSB);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(sg_u32Gain2, SHARP_EDGE_GAIN_ADDR, SHARP_EDGE_GAIN_LSB, SHARP_EDGE_GAIN_MSB);
#endif
    return s32Ret;
}

/****************************************** SHARPEN START*************************************************/
/* Init Sharp parameter */
static HI_VOID PQ_MNG_SharpParaInit(HI_VOID)
{
    sg_bSharpen_EnFlag = HI_FALSE;
    return;
}

/* Init Sharp */
static HI_S32 PQ_MNG_SharpInit(HI_U32 u32Data)
{
    HI_S32 s32Ret;

    if (HI_TRUE == g_bShpInitFlag)
    {
        return HI_SUCCESS;
    }
    PQ_MNG_SharpParaInit();

    /* Hsharpen and Sharpen Use Same ID--PQ_BIN_MODULE_SHARPEN 0x4 */
    
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    s32Ret = PQ_MNG_InitPhyList(u32Data, REG_TYPE_VPSS, SOURCE_MODE_HD, OUTPUT_MODE_HD, PQ_BIN_MODULE_SHARPEN);
#endif
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    s32Ret = PQ_MNG_InitPhyList(u32Data, REG_TYPE_VDP, SOURCE_MODE_HD, OUTPUT_MODE_HD, PQ_BIN_MODULE_SHARPEN);
#endif

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

    g_bShpInitFlag = HI_TRUE;
    return HI_SUCCESS;
}

/* Deinit Sharp */
static HI_S32 PQ_MNG_SharpDeinit(HI_VOID)
{
    if (HI_FALSE == g_bShpInitFlag)
    {
        return HI_SUCCESS;
    }
    g_bShpInitFlag = HI_FALSE;
    return HI_SUCCESS;
}

/* Get Sharpen Strength */
static HI_S32 PQ_MNG_GetSharpStr(HI_S32* ps32ShpStr)
{
    *ps32ShpStr = sg_s32SharpStr;

    return HI_SUCCESS;
}

/* Enable Sharp Include Mv410 Vdp Sharpen And 98M Vpss Sharpen; difference in Hal layer */
static HI_S32 PQ_MNG_SharpEn(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret = PQ_HAL_EnableSharp(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in sharpen enable!\n");
        return HI_FAILURE;
    }
    sg_bSharpen_EnFlag = bOnOff;

    return HI_SUCCESS;
}

/* Get Sharpen Enable Flag */
static HI_S32 PQ_MNG_GetSharpEn(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bSharpen_EnFlag;

    return HI_SUCCESS;
}

/* Enable Sharpen DEMO */
static HI_S32 PQ_MNG_SharpDemoEn(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret = PQ_HAL_EnableSharpDemo(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in sharpen demo enable!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
/****************************************** SHARPEN END*************************************************/

/****************************************** COMMON START*************************************************/
/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_DeinitSharpen(HI_VOID)
{
    PQ_MNG_SharpDeinit();

    return HI_SUCCESS;
}

/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_InitSharpen(PQ_PARAM_S* pstPqParam)
{
    HI_S32 s32Ret;
    HI_U32 u32Data = 0;

    g_pstPqParam = pstPqParam;

    s32Ret = PQ_MNG_SharpInit(u32Data);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in init sharpen!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Set Sharpen Strength, Range: 0~255;
   说明: UI菜单设置直接调用，根据当前的源和输出信息将设置的s32ShpStr(0-255)值混合Bin文件的Gain值设置Sharpen强度
   */
HI_S32 PQ_MNG_SetSharpenStr(HI_S32 s32ShpStr, HI_U32 u32SourceMode, HI_U32 u32OutputMode)
{
    PQ_SHARP_GAIN_S stSharpGain;

    if ((s32ShpStr < 0) || (s32ShpStr > 255))
    {
        HI_ERR_PQ("the value of setting sharpen strength is out of range!\n");
        return HI_FAILURE;
    }

    if ((SOURCE_MODE_UHD < u32SourceMode) || (OUTPUT_MODE_UHD < u32OutputMode))
    {
        HI_ERR_PQ("source mode or output mode out of range!\n");
        return HI_FAILURE;
    }

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    /* SD At 98M_A, There is only HD in 98M */
    stSharpGain.u32HD_Lgain_ratio = sg_u32Gain1[u32SourceMode][u32OutputMode];
    stSharpGain.u32HD_Cgain_ratio = sg_u32Gain2[u32SourceMode][u32OutputMode];
#endif
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    stSharpGain.u32PeakGain = sg_u32Gain1[u32SourceMode][u32OutputMode];
    stSharpGain.u32EdgeGain = sg_u32Gain2[u32SourceMode][u32OutputMode];
#endif

    PQ_HAL_SetSharpStrReg(s32ShpStr, &stSharpGain);

    sg_s32SharpStr = s32ShpStr;

    return HI_SUCCESS;
}

/* Get Sharpen Strength, Range: 0~255 */
HI_S32 PQ_MNG_GetSharpenStr(HI_S32* ps32ShpStr)
{
    PQ_MNG_GetSharpStr(ps32ShpStr);

    return HI_SUCCESS;
}

/* Enable Sharpen Moudle(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpen(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret = PQ_MNG_SharpEn(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable sharpen!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Get Sharpen Enable(Sharpen and HSharpen) */
HI_S32 PQ_MNG_GetSharpenEnable(HI_BOOL* pbOnOff)
{
    PQ_MNG_GetSharpEn(pbOnOff);

    return HI_SUCCESS;
}

/* Enable Sharpen Demo(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpenDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret = PQ_MNG_SharpDemoEn(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable sharpen demo!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSharpDemoMode(SHARP_DEMO_MODE_E enMode)
{
    HI_U32 u32VId = 0;
    PQ_HAL_SetSharpDemoMode(u32VId, enMode);

    sg_enMode = enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetSharpDemoMode(SHARP_DEMO_MODE_E* enMode)
{
    *enMode = sg_enMode ;

    return HI_SUCCESS;
}
/******************************************* COMMON END*************************************************/


