/******************************************************************************
 Copyright (C), 2001-2014, Hisilicon Tech. Co. Ltd.
******************************************************************************
File Name     : pq_mng_hsharpen.c
Version       : Initial Draft
Author        : d00241380
Created       : 2014/12/24
Last Modified :
Description   : PQ MNG层HSHARPEN模块函数实现
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

/* Hsharpen */
static HI_BOOL     sg_bHSharpen_EnFlag;
static HI_BOOL     g_bHShpInitFlag  = HI_FALSE;
static HI_S32      sg_s32HSharpStr  = 128;
static PQ_PARAM_S* g_pstPqParam     = HI_NULL;
static SHARP_DEMO_MODE_E sg_enMode  = SHARP_DEMO_ENABLE_R;


static HI_S32 PQ_MNG_FindSharpenGainRatio(HI_U32* pu32Value, HI_U32 u32RegAddr, HI_U8 u8GainLsb, HI_U8 u8GainMsb, HI_U8 u8InMode, HI_U8 u8OutMode)
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
        if (u8SourceMode != u8InMode)
        {
            continue;
        }
        if (u8OutputMode != u8OutMode)
        {
            continue;
        }

        *pu32Value = u32Value;
        pqprint(PQ_PRN_HSHARPEN, "GetSharpenGainRatio:%d\n", *pu32Value);

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 PQ_MNG_GetHSharpGain(HI_U32* pu32HspPosGainH0, HI_U32* pu32HspNegGainH0, HI_U32* pu32HspPosGainH1,
                                   HI_U32* pu32HspNegGainH1, HI_U8 u8InMode, HI_U8 u8OutMode)
{
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pu32HspPosGainH0);
    PQ_CHECK_NULL_PTR(pu32HspNegGainH0);
    PQ_CHECK_NULL_PTR(pu32HspPosGainH1);
    PQ_CHECK_NULL_PTR(pu32HspNegGainH1);

    s32Ret  = PQ_MNG_FindSharpenGainRatio(pu32HspPosGainH0, HSHARP_H0_GAIN_POS_ADDR, HSHARP_H0_GAIN_POS_LSB, HSHARP_H0_GAIN_POS_MSB, u8InMode, u8OutMode);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(pu32HspNegGainH0, HSHARP_H0_GAIN_NEG_ADDR, HSHARP_H0_GAIN_NEG_LSB, HSHARP_H0_GAIN_NEG_MSB, u8InMode, u8OutMode);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(pu32HspPosGainH1, HSHARP_H1_GAIN_NEG_ADDR, HSHARP_H1_GAIN_NEG_LSB, HSHARP_H1_GAIN_NEG_MSB, u8InMode, u8OutMode);
    s32Ret |= PQ_MNG_FindSharpenGainRatio(pu32HspNegGainH1, HSHARP_H1_GAIN_NEG_ADDR, HSHARP_H1_GAIN_NEG_LSB, HSHARP_H1_GAIN_NEG_MSB, u8InMode, u8OutMode);

    return s32Ret;
}

/***************************************** HSHARPEN START*************************************************/
/* Init HSharp parameter */
static HI_VOID PQ_MNG_HSharpParaInit(HI_VOID)
{
    sg_bHSharpen_EnFlag = HI_FALSE;
    return;
}

/* Init HSharp */
static HI_S32 PQ_MNG_HSharpInit(HI_U32 u32Data)
{
    HI_S32 s32Ret;

    if (HI_TRUE == g_bHShpInitFlag)
    {
        return HI_SUCCESS;
    }

    PQ_MNG_HSharpParaInit();

    /* Hsharpen and Sharpen Use Same ID--PQ_BIN_MODULE_SHARPEN 0x4 */
    s32Ret = PQ_MNG_InitPhyList(u32Data, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_SHARPEN);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("HSharpen InitPhyList error\n");
        return HI_FAILURE;
    }

    g_bHShpInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Deinit HSharp */
static HI_S32 PQ_MNG_HSharpDeinit(HI_VOID)
{
    if (HI_FALSE == g_bHShpInitFlag)
    {
        return HI_SUCCESS;
    }
    g_bHShpInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

/* Get Hsharpen Strength */
static HI_S32 PQ_MNG_GetHSharpStr(HI_S32* ps32HShpStr)
{
    *ps32HShpStr = sg_s32HSharpStr;

    return HI_SUCCESS;
}

/* Enable Sharp */
static HI_S32 PQ_MNG_HSharpEn(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableHSharp(u32HandleNo, bOnOff);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in Hsharpen enable!\n");
                return HI_FAILURE;
            }
        }
    }
    sg_bHSharpen_EnFlag = bOnOff;

    return HI_SUCCESS;
}

/* Get Sharpen Enable Flag */
static HI_S32 PQ_MNG_GetHSharpEn(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bHSharpen_EnFlag;

    return HI_SUCCESS;
}

/* Enable Sharpen DEMO */
static HI_S32 PQ_MNG_HSharpDemoEn(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo;
    HI_U32 u32HshpDemoMode = SHARP_DEMO_ENABLE_R;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            PQ_HAL_SetHSharpDemoMode(u32HandleNo, u32HshpDemoMode);

            s32Ret = PQ_HAL_EnableHSharpDemo(u32HandleNo, bOnOff);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PQ("error in Hsharpen demo enable!\n");
                return HI_FAILURE;
            }
        }
    }
    return HI_SUCCESS;
}

/****************************************** HSHARPEN END*************************************************/

/****************************************** COMMON START*************************************************/
/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_DeinitSharpen(HI_VOID)
{
    PQ_MNG_HSharpDeinit();

    return HI_SUCCESS;
}

/* Deinit Sharp Moudle (Include Sharpen and HSharpen) */
HI_S32 PQ_MNG_InitSharpen(PQ_PARAM_S* pstPqParam)
{
    HI_S32 s32Ret;
    HI_U32 u32Data = 0;

    g_pstPqParam = pstPqParam;

    s32Ret = PQ_MNG_HSharpInit(u32Data);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in init Hsharpen!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Set Sharpen Strength, Range: 0~255;
   说明: UI菜单设置直接调用，根据当前的源和输出信息将设置的s32ShpStr(0-255)值混合Bin文件的Gain值设置Sharpen强度
   */
HI_S32 PQ_MNG_SetSharpenStr(HI_S32 s32ShpStr, HI_U32 u32SourceMode, HI_U32 u32OutputMode)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    HI_U32 u32BinHspPosGainH0 = 0;
    HI_U32 u32BinHspNegGainH0 = 0;
    HI_U32 u32BinHspPosGainH1 = 0;
    HI_U32 u32BinHspNegGainH1 = 0;

    PQ_HSHARP_GAIN_S stHSharpGain;

    if ((s32ShpStr < 0) || (s32ShpStr > 255))
    {
        HI_ERR_PQ("the value of setting Hsharpen strength is out of range!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_GetHSharpGain(&u32BinHspPosGainH0, &u32BinHspNegGainH0, &u32BinHspPosGainH1, &u32BinHspNegGainH1, u32SourceMode, u32OutputMode);
    if (HI_FAILURE == s32Ret)
    {
        u32BinHspPosGainH0 = 64;
        u32BinHspNegGainH0 = 64;
        u32BinHspPosGainH1 = 64;
        u32BinHspNegGainH1 = 64;
        HI_DEBUG_PQ("Get HSharpGain failure\n");
    }

    stHSharpGain.s32H0GainPos = (HI_S32)u32BinHspPosGainH0;
    stHSharpGain.s32H0GainNeg = (HI_S32)u32BinHspNegGainH0;
    stHSharpGain.s32H1GainPos = (HI_S32)u32BinHspPosGainH1;
    stHSharpGain.s32H1GainNeg = (HI_S32)u32BinHspNegGainH1;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            PQ_HAL_SetHSharpStrReg(u32HandleNo, s32ShpStr, &stHSharpGain);
        }
    }
    sg_s32HSharpStr = s32ShpStr;

    return HI_SUCCESS;
}

/* Get Sharpen Strength, Range: 0~255 */
HI_S32 PQ_MNG_GetSharpenStr(HI_S32* ps32ShpStr)
{
    PQ_MNG_GetHSharpStr(ps32ShpStr);

    return HI_SUCCESS;
}

/* Enable Sharpen Moudle(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpen(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret =  PQ_MNG_HSharpEn(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable Hsharpen!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Get Sharpen Enable(Sharpen and HSharpen) */
HI_S32 PQ_MNG_GetSharpenEnable(HI_BOOL* pbOnOff)
{
    PQ_MNG_GetHSharpEn(pbOnOff);

    return HI_SUCCESS;
}

/* Enable Sharpen Demo(Sharpen and HSharpen) */
HI_S32 PQ_MNG_EnableSharpenDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret;

    s32Ret = PQ_MNG_HSharpDemoEn(bOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable Hsharpen demo!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetSharpDemoMode(SHARP_DEMO_MODE_E enMode)
{
    HI_U32 u32VId = 0;
    PQ_HAL_SetHSharpDemoMode(u32VId, enMode);

    sg_enMode = enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetSharpDemoMode(SHARP_DEMO_MODE_E* enMode)
{
    *enMode = sg_enMode ;

    return HI_SUCCESS;
}
/******************************************* COMMON END*************************************************/


