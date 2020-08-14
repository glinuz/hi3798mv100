/******************************************************************************
 Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mng_pq_dci.c
Version       : Initial Draft
Author        :
Created       : 2013/10/20
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
//#include <linux/slab.h>      /* kmalloc() */
#include <linux/string.h>      /* memcpy() */
#include <linux/fcntl.h>

#include "hi_math.h"
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "hi_drv_file.h"

#include "pq_hal_comm.h"
#include "pq_hal_dci.h"
#include "pq_mng_dci.h"
#include "pq_mng_dci_coef.h"


static HI_BOOL     bDciEn         = HI_TRUE;
/* using in UNF */
static HI_U32      sg_u32DCILevel = 50;
static PQ_PARAM_S* sg_pstPqParam  = HI_NULL;
static DCI_DEMO_MODE_E sg_enMode  = DCI_DEMO_ENABLE_R;


/* 初始化DCI参数 */
static HI_S32 PQ_MNG_DCIParaInit(HI_VOID)
{
    return HI_SUCCESS;
}

/* 从PQBin中获取DCI增益 */
static HI_S32 PQ_MNG_FindDciGain(HI_VOID)
{
    HI_U32 i, u32Addr, u32Value;
    HI_U32 u32ListNum = PHY_REG_MAX;
    HI_U8  u8Lsb, u8Msb, u8SourceMode;
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

        if (u32Addr != DCI_GAIN_ADDR)
        {
            continue;
        }

        if (u8SourceMode > SOURCE_MODE_UHD)
        {
            continue;
        }

        if ((DCI_GAIN0_LSB == u8Lsb) && (DCI_GAIN0_MSB == u8Msb))
        {
            sg_u32DCILevelGain[u8SourceMode][DCI_GLOBAL_GAIN_0] = u32Value;
            pqprint(PQ_PRN_DCI, "Get Dci Gain0:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }
        else if ((DCI_GAIN1_LSB == u8Lsb) && (DCI_GAIN1_MSB == u8Msb))
        {
            sg_u32DCILevelGain[u8SourceMode][DCI_GLOBAL_GAIN_1] = u32Value;
            pqprint(PQ_PRN_DCI, "Get Dci Gain1:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }
        else if ((DCI_GAIN2_LSB == u8Lsb) && (DCI_GAIN2_MSB == u8Msb))
        {
            sg_u32DCILevelGain[u8SourceMode][DCI_GLOBAL_GAIN_2] = u32Value;
            pqprint(PQ_PRN_DCI, "Get Dci Gain2:%d, SourceMode:%d\n", u32Value, u8SourceMode);
        }

    }

    return HI_SUCCESS;

}


/* 初始化DCI的LUT */
HI_S32 PQ_MNG_DCILUTInit(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32       s32Ret;
    DCI_LUT_S    stDciLut;
    PQ_DCI_LUT_S* pstDciLut = HI_NULL;

    PQ_CHECK_NULL_PTR(pstPqParam);
    sg_pstPqParam = pstPqParam;
    pstDciLut = &(pstPqParam->stPQCoef.stDciCoef.stPqDciLut);

    memset(&stDciLut, 0, sizeof(DCI_LUT_S));
    memcpy(stDciLut.s16HistCoef0, HistCoef0, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16HistCoef1, HistCoef1, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16HistCoef2, HistCoef2, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16YDivCoef, YdivCoef, sizeof(HI_S16) * 64);

    if (HI_TRUE == bDefault)
    {
        memcpy(stDciLut.s16WgtCoef0, WgtCoef0, sizeof(HI_S16) * 33);
        memcpy(stDciLut.s16WgtCoef1, WgtCoef1, sizeof(HI_S16) * 33);
        memcpy(stDciLut.s16WgtCoef2, WgtCoef2, sizeof(HI_S16) * 33);

        memcpy(pstDciLut->s16WgtCoef0, WgtCoef0, sizeof(HI_S16) * 33);
        memcpy(pstDciLut->s16WgtCoef1, WgtCoef1, sizeof(HI_S16) * 33);
        memcpy(pstDciLut->s16WgtCoef2, WgtCoef2, sizeof(HI_S16) * 33);

        pstDciLut->u16Gain0 = sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_0];
        pstDciLut->u16Gain1 = sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_1];
        pstDciLut->u16Gain2 = sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_2];
    }
    else
    {
        memcpy(stDciLut.s16WgtCoef0, pstDciLut->s16WgtCoef0, sizeof(HI_S16) * 33);
        memcpy(stDciLut.s16WgtCoef1, pstDciLut->s16WgtCoef1, sizeof(HI_S16) * 33);
        memcpy(stDciLut.s16WgtCoef2, pstDciLut->s16WgtCoef2, sizeof(HI_S16) * 33);
    }

    s32Ret = PQ_HAL_SetDCILut(&stDciLut);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("PQ_HAL_SetDCILut error!\n");
        return HI_FAILURE;
    }

    PQ_HAL_SetDCIlevel(VDP_LAYER_VP0, sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_0], \
                       sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_1], \
                       sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_2]);
    return HI_SUCCESS;
}

/* 初始化DCI模块 */
HI_S32 PQ_MNG_InitDCI(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;
    s32Ret = PQ_HAL_DCIInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_HAL_DCIInit error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_DCIParaInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_DCIParaInit error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_DCILUTInit(pstPqParam, bDefault);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_DCILUTInit error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_InitPhyList(u32ID, REG_TYPE_VDP, SOURCE_MODE_HD, OUTPUT_MODE_HD, PQ_BIN_MODULE_DCI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DCI InitPhyList error\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_FindDciGain();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DCI FindDciGain error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* 去初始化DCI模块 */
HI_S32 PQ_MNG_DeinitDCI(HI_VOID)
{
    PQ_HAL_DCIDeinit();
    return HI_SUCCESS;
}

/* 获取DCI模块开关状态 */
HI_S32 PQ_MNG_GetDCIEnable(HI_U32* pu32OnOff)
{
    *pu32OnOff = bDciEn;
    return HI_SUCCESS;
}

/* DCI模块开关 */
HI_S32 PQ_MNG_EnableDCI(HI_BOOL bOnOff)
{
    PQ_HAL_EnableDCI(VDP_LAYER_VP0, bOnOff);
    bDciEn = bOnOff;
    return HI_SUCCESS;
}


/* 设置DCI强度增益等级 */
HI_S32 PQ_MNG_SetDCILevelGain(HI_U32 u32DCILevelGain, HI_U32 u32SourceMode)
{
    HI_S32 s32GainOffset0 = 0;
    HI_S32 s32GainOffset1 = 0;
    HI_S32 s32GainOffset2 = 0;
    DCI_LEVEL_S stDCILevelTmp;

    if (u32SourceMode >= SOURCE_MODE_ALL)
    {
        u32SourceMode = SOURCE_MODE_NO;
    }

    sg_u32DCILevel = u32DCILevelGain;

    /* The Second number 50 and number 35,65,55 etc has no practical physical meaning */
    switch (u32SourceMode)
    {
        case SOURCE_MODE_NO:
            if (u32DCILevelGain > 50) /* 42;42;42 */
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
            }
            else
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
            }
            break;
        case SOURCE_MODE_SD:
            if (u32DCILevelGain > 50) /* 32;32;24 */
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 55 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 55 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
            }
            else
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 50 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 50 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
            }
            break;
        case SOURCE_MODE_HD:
            if (u32DCILevelGain > 50) /* 42;42;42 */
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
            }
            else
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
            }
            break;
        case SOURCE_MODE_UHD:
            if (u32DCILevelGain > 50) /* 48;48;42 */
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 25 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 25 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
            }
            else
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 75 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 75 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
            }
            break;

        default:
            if (u32DCILevelGain > 50)
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 35 + 50) / 100 );
            }
            else
            {
                s32GainOffset0 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset1 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
                s32GainOffset2 = (HI_S32)((((HI_S32)(u32DCILevelGain - 50)) * 65 + 50) / 100 );
            }
            break;
    }

    stDCILevelTmp.u8Gain0 = CLIP3((sg_u32DCILevelGain[u32SourceMode][DCI_GLOBAL_GAIN_0] + s32GainOffset0), 0, 63);
    stDCILevelTmp.u8Gain1 = CLIP3((sg_u32DCILevelGain[u32SourceMode][DCI_GLOBAL_GAIN_1] + s32GainOffset1), 0, 63);
    stDCILevelTmp.u8Gain2 = CLIP3((sg_u32DCILevelGain[u32SourceMode][DCI_GLOBAL_GAIN_2] + s32GainOffset2), 0, 63);

    PQ_HAL_SetDCIlevel(VDP_LAYER_VP0, stDCILevelTmp.u8Gain0, stDCILevelTmp.u8Gain1, stDCILevelTmp.u8Gain2);

    return HI_SUCCESS;
}

/* 设置DCI强度增益等级 */
HI_S32 PQ_MNG_GetDCILevelGain(HI_U32* pu32DCILevelGain)
{
    PQ_CHECK_NULL_PTR(pu32DCILevelGain);

    *pu32DCILevelGain = sg_u32DCILevel;

    return HI_SUCCESS;
}

/* DCI开关卖场模式 */
HI_S32 PQ_MNG_EnableDCIDemo(HI_BOOL bOnOff)
{
    PQ_HAL_EnableDCIDemo(VDP_LAYER_VP0, bOnOff);
    return HI_SUCCESS;
}

/* 更新DCI直方图统计窗口 */
HI_S32 PQ_MNG_UpdateDCIWin(DCI_WIN_S* pstWin)
{
    PQ_HAL_SetDCIWindow(VDP_LAYER_VP0, pstWin->u16HStar, pstWin->u16HEnd, pstWin->u16VStar, pstWin->u16VEnd);
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDCIDemoMode(DCI_DEMO_MODE_E enMode)
{
    PQ_HAL_LAYER_VP_E u32ChId = VDP_LAYER_VP0;

    PQ_HAL_SetDCIDemoMode(u32ChId, enMode);

    sg_enMode = enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDCIDemoMode(DCI_DEMO_MODE_E* enMode)
{
    *enMode = sg_enMode;
    return HI_SUCCESS;
}


