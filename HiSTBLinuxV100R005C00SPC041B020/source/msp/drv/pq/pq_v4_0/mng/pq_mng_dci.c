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

#define DCI_SCD_SADRshft 0
#define DCI_SCD_SADThrsh 400
#define DCI_BINNUM 32

static HI_BOOL  sg_bDciEn           = HI_TRUE;
static HI_BOOL  sg_bDciBSEn         = HI_TRUE;
static HI_BOOL  sg_bDciDemoEn       = HI_FALSE;
static HI_BOOL  sg_bCtrlSceneChange = HI_TRUE;


/* using in UNF */
static HI_U32      sg_u32DCILevel = 50;
static PQ_PARAM_S *sg_pstPqParam  = HI_NULL;
static DCI_DEMO_MODE_E sg_enMode  = DCI_DEMO_ENABLE_R;

HI_S32 PQ_MNG_SetDCIWgtLut(HI_PQ_DCI_WGT_S *pstDciCoef)
{
    PQ_CHECK_NULL_PTR(sg_pstPqParam);
    PQ_CHECK_NULL_PTR(pstDciCoef);

    memcpy(&(sg_pstPqParam->stPQCoef.stDciCoef.stPqDciLut), pstDciCoef, sizeof(HI_PQ_DCI_WGT_S));
    return PQ_HAL_SetDCIWgtLut((DCI_WGT_S *)pstDciCoef);
}

HI_S32 PQ_MNG_GetDCIWgtLut(HI_PQ_DCI_WGT_S *pstDciCoef)
{
    PQ_CHECK_NULL_PTR(pstDciCoef);
    return PQ_HAL_GetDCIWgtLut((DCI_WGT_S *)pstDciCoef);
}

HI_S32 PQ_MNG_GetDCIHistgram(HI_PQ_DCI_HISTGRAM_S *pstDciHist)
{
    PQ_CHECK_NULL_PTR(pstDciHist);
    return PQ_HAL_GetDCIHistgram((DCI_HISTGRAM_S *)pstDciHist);
}

HI_S32 PQ_MNG_SetDciBSCoef(HI_PQ_DCI_BS_LUT_S *pstDciCoef)
{
    DCI_BS_LUT_S stBsLut;
    PQ_CHECK_NULL_PTR(sg_pstPqParam);
    PQ_CHECK_NULL_PTR(pstDciCoef);

    memcpy(&(sg_pstPqParam->stPQCoef.stDciCoef.stPqDciBSLut), pstDciCoef, sizeof(HI_PQ_DCI_BS_LUT_S));
    memcpy(stBsLut.s16BSDelta, pstDciCoef->s16BSDelta, sizeof(HI_S16) * 320);
    return PQ_HAL_SetDciBSCoef(&stBsLut);
}

HI_S32 PQ_MNG_GetDciBSCoef(HI_PQ_DCI_BS_LUT_S *pstDciCoef)
{
    DCI_BS_LUT_S stBsLut;
    PQ_CHECK_NULL_PTR(pstDciCoef);

    PQ_HAL_GetDciBSCoef(&stBsLut);
    memcpy(pstDciCoef->s16BSDelta, stBsLut.s16BSDelta, sizeof(HI_S16) * 320);

    return HI_SUCCESS;
}


/* 从PQBin中获取DCI增益 */
static HI_S32 PQ_MNG_FindDciGain(HI_VOID)
{
    HI_U32 i, u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8SourceMode;
    PQ_PHY_REG_S *sg_pstPhyReg   = HI_NULL;
    HI_U32 u32StartPos = 0;
    HI_U32 u32EndPos   = 0;

    PQ_CHECK_NULL_PTR(sg_pstPqParam);
    sg_pstPhyReg = sg_pstPqParam->stPQPhyReg;

    PQ_TABLE_FindBinPos(HI_PQ_MODULE_DCI, &u32StartPos, &u32EndPos);

    for (i = u32StartPos; i <= u32EndPos; i++)
    {
        u32Addr       = sg_pstPhyReg[i].u32RegAddr;
        u32Value      = sg_pstPhyReg[i].u32Value;
        u8Lsb         = sg_pstPhyReg[i].u8Lsb;
        u8Msb         = sg_pstPhyReg[i].u8Msb;
        u8SourceMode  = sg_pstPhyReg[i].u8SourceMode;

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
static HI_S32 PQ_MNG_DCILUTInit(HI_BOOL bDefault)
{
    HI_S32       s32Ret;
    DCI_LUT_S    stDciLut;
    PQ_DCI_LUT_S   *pstDciLut = HI_NULL;

    PQ_CHECK_NULL_PTR(sg_pstPqParam);

    pstDciLut = &(sg_pstPqParam->stPQCoef.stDciCoef.stPqDciLut);

    memset(&stDciLut, 0, sizeof(DCI_LUT_S));
    memcpy(stDciLut.s16HistCoef0, HistCoef0, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16HistCoef1, HistCoef1, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16HistCoef2, HistCoef2, sizeof(HI_S16) * 32);
    memcpy(stDciLut.s16YDivCoef, YdivCoef, sizeof(HI_S16) * 64);
    memcpy(stDciLut.s32DciCfgReg, DciCfgReg, sizeof(HI_S32) * 28);

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

    s32Ret = PQ_HAL_SetDciLut(&stDciLut);

    PQ_HAL_SetDCIlevel(PQ_HAL_LAYER_VID0, sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_0], \
                       sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_1], \
                       sg_u32DCILevelGain[SOURCE_MODE_HD][DCI_GLOBAL_GAIN_2]);
    return s32Ret;
}

/* 初始化DCI的BS LUT */
static HI_S32 PQ_MNG_DciBSLutInit(HI_BOOL bDefault)
{
    HI_S32       s32Ret;
    DCI_BS_LUT_S    stDciBsLut;
    PQ_DCI_BSLUT_S *pstDciBSLut = HI_NULL;

    PQ_CHECK_NULL_PTR(sg_pstPqParam);

    pstDciBSLut = &(sg_pstPqParam->stPQCoef.stDciCoef.stPqDciBSLut);

    memset(&stDciBsLut, 0, sizeof(DCI_BS_LUT_S));
    memcpy(stDciBsLut.s16VertOffset, VertOffset, sizeof(HI_S16) * 160);

    if (HI_TRUE == bDefault)
    {
        memcpy(stDciBsLut.s16BSDelta, BSDelta, sizeof(HI_S16) * 320);
        memcpy(pstDciBSLut->s16BSDelta, BSDelta, sizeof(HI_S16) * 320);
    }
    else
    {
        memcpy(stDciBsLut.s16BSDelta, pstDciBSLut->s16BSDelta, sizeof(HI_S16) * 320);
    }

    s32Ret = PQ_HAL_SetDciBSLut(&stDciBsLut);

    return s32Ret;
}

/* 初始化DCI模块 */
HI_S32 PQ_MNG_InitDCI(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;
    s32Ret = PQ_HAL_DCIInit();

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_HAL_DCIInit error!\n");
        return HI_FAILURE;
    }

    PQ_CHECK_NULL_PTR(pstPqParam);
    sg_pstPqParam = pstPqParam;

    s32Ret = PQ_MNG_DCILUTInit(bDefault);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_DCILUTInit error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_DciBSLutInit(bDefault);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_DciBSLutInit error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_TABLE_InitPhyList(u32ID, HI_PQ_MODULE_DCI, SOURCE_MODE_HD, OUTPUT_MODE_HD);

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
HI_S32 PQ_MNG_GetDCIEnable(HI_U32 *pu32OnOff)
{
    *pu32OnOff = sg_bDciEn;
    return HI_SUCCESS;
}

/* DCI模块开关 */
HI_S32 PQ_MNG_EnableDCI(HI_BOOL bOnOff)
{
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if (g_stPqStatus.b3dType == HI_TRUE)
    {
        bOnOff = HI_FALSE;
    }

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DCI, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    PQ_HAL_EnableDCI(PQ_HAL_LAYER_VID0, bOnOff);
    sg_bDciEn = bOnOff;
    return HI_SUCCESS;
}

/* DCI 黑扩展模块开关 */
HI_S32 PQ_MNG_EnableDciBS(HI_BOOL bOnOff)
{
    PQ_HAL_EnableDciBS(PQ_HAL_LAYER_VID0, bOnOff);
    sg_bDciBSEn = bOnOff;
    return HI_SUCCESS;
}


/* 设置DCI强度增益等级 */
HI_S32 PQ_MNG_SetDCILevelGain(HI_U32 u32DCILevelGain)
{
    HI_S32 s32GainOffset0 = 0;
    HI_S32 s32GainOffset1 = 0;
    HI_S32 s32GainOffset2 = 0;
    DCI_LEVEL_S stDCILevelTmp;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if (enSourceMode >= SOURCE_MODE_ALL)
    {
        enSourceMode = SOURCE_MODE_NO;
    }

    sg_u32DCILevel = u32DCILevelGain;

    /* The Second number 50 and number 35,65,55 etc has no practical physical meaning */
    switch (enSourceMode)
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

    stDCILevelTmp.u8Gain0 = ALG_CLIP3(0, 63, (sg_u32DCILevelGain[enSourceMode][DCI_GLOBAL_GAIN_0] + s32GainOffset0));
    stDCILevelTmp.u8Gain1 = ALG_CLIP3(0, 63, (sg_u32DCILevelGain[enSourceMode][DCI_GLOBAL_GAIN_1] + s32GainOffset1));
    stDCILevelTmp.u8Gain2 = ALG_CLIP3(0, 63, (sg_u32DCILevelGain[enSourceMode][DCI_GLOBAL_GAIN_2] + s32GainOffset2));

    PQ_HAL_SetDCIlevel(PQ_HAL_LAYER_VID0, stDCILevelTmp.u8Gain0, stDCILevelTmp.u8Gain1, stDCILevelTmp.u8Gain2);

    if (enSourceMode >= SOURCE_MODE_UHD)
    {
        PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID0, HI_TRUE);

        if (u32DCILevelGain == 0)
        {
            /*off*/
            PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 0);
        }
        else if (u32DCILevelGain < 25)
        {
            /*low*/
            PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 4);
        }
        else if (u32DCILevelGain > 75)
        {
            /*high*/
            PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 12);
        }
        else
        {
            /*middle*/
            PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 8);
        }

        return HI_SUCCESS;
    }


    if (u32DCILevelGain == 0)
    {
        /*off*/
        PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID0, HI_TRUE);
        PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 0);
    }
    else if (u32DCILevelGain < 25)
    {
        /*low*/
        PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID0, HI_FALSE);
        PQ_HAL_SetDciClipLimit(PQ_HAL_LAYER_VID0, 5, 5);
        PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 6);
    }
    else if (u32DCILevelGain > 75)
    {
        /*high*/
        PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID0, HI_FALSE);
        PQ_HAL_SetDciClipLimit(PQ_HAL_LAYER_VID0, 15, 15);
        PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 18);
    }
    else
    {
        /*middle*/
        PQ_HAL_SetDciManAlgBlendEn(PQ_HAL_LAYER_VID0, HI_FALSE);
        PQ_HAL_SetDciClipLimit(PQ_HAL_LAYER_VID0, 10, 10);
        PQ_HAL_SetDciAlphaBlend(PQ_HAL_LAYER_VID0, 12);
    }

    return HI_SUCCESS;
}

/* 设置DCI强度增益等级 */
HI_S32 PQ_MNG_GetDCILevelGain(HI_U32 *pu32DCILevelGain)
{
    PQ_CHECK_NULL_PTR(pu32DCILevelGain);
    *pu32DCILevelGain = sg_u32DCILevel;
    return HI_SUCCESS;
}

/* DCI开关卖场模式 */
HI_S32 PQ_MNG_EnableDCIDemo(HI_BOOL bOnOff)
{
    PQ_HAL_EnableDCIDemo(PQ_HAL_LAYER_VID0, bOnOff);
    sg_bDciDemoEn = bOnOff;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDCIDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDciDemoEn;
    return HI_SUCCESS;
}

/* 更新DCI直方图统计窗口 */
HI_S32 PQ_MNG_UpdateDCIWin(HI_PQ_DCI_WIN_S *pstWin)
{
    PQ_HAL_SetDCIWindow(PQ_HAL_LAYER_VID0, pstWin->u16HStar, pstWin->u16HEnd, pstWin->u16VStar, pstWin->u16VEnd);
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDCIDemoMode(PQ_DEMO_MODE_E enMode)
{
    PQ_HAL_LAYER_VID_E u32ChId = PQ_HAL_LAYER_VID0;
    DCI_DEMO_MODE_E enDciMode = DCI_DEMO_ENABLE_R;

    sg_enMode = enMode;

    if (enMode == PQ_DEMO_ENABLE_L)
    {
        enDciMode = DCI_DEMO_ENABLE_L;
    }
    else if (enMode == PQ_DEMO_ENABLE_R)
    {
        enDciMode = DCI_DEMO_ENABLE_R;
    }

    PQ_HAL_SetDCIDemoMode(u32ChId, enDciMode);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDCIDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);

    if ( sg_enMode == DCI_DEMO_ENABLE_R)
    {
        *penMode = PQ_DEMO_ENABLE_L;
    }
    else if ( sg_enMode == DCI_DEMO_ENABLE_L)
    {
        *penMode = PQ_DEMO_ENABLE_R;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDciScd(HI_BOOL bSceneChange)
{
    sg_bCtrlSceneChange = bSceneChange;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDciScd(HI_BOOL *pbSceneChange)
{
    PQ_CHECK_NULL_PTR(pbSceneChange);
    *pbSceneChange = sg_bCtrlSceneChange;
    return HI_SUCCESS;
}


HI_S32 PQ_MNG_CalDCIScd(HI_VOID)
{
    static HI_U32 u32CurFrmNum = 0;
    static HI_S32 s32HistSAD[7] = {0};
    static HI_S32 s32Histogram1[32] = {0};
    static HI_S32 s32Histogram2[32] = {0};
    static HI_S32 s32Count = 0;
    static HI_BOOL bSceneChange = HI_TRUE;
    DCI_HISTGRAM_S stDciHist;

    if (sg_bCtrlSceneChange == HI_TRUE)
    {
        HI_S32 i = 0, j = 0, k = 0;

        for (i = 5; i >= 0; i--)
        {
            s32HistSAD[i + 1] = s32HistSAD[i];
        }

        s32HistSAD[0] = 0;

        PQ_HAL_GetDCIHistgram(&stDciHist);

        for (j = 0; j < DCI_BINNUM; j++)
        {
            s32Histogram2[j] = s32Histogram1[j];
            s32Histogram1[j] = stDciHist.s32HistGram[j];
        }

        if (u32CurFrmNum > 0)
        {
            for (k = 0; k < DCI_BINNUM; k++)
            {
                s32HistSAD[0] += abs ((s32Histogram1[k] - s32Histogram2[k]) >> DCI_SCD_SADRshft);
            }
        }

        if ((HI_TRUE == bSceneChange) && (s32Count > 0))
        {
            bSceneChange = HI_TRUE;
            s32Count = 0;
        }
        else
        {
            if (u32CurFrmNum >= 8)
            {
                //if 有个顺序因素，先判断第一个  如果不合格后面不用判断
                if ((s32HistSAD[0] > DCI_SCD_SADThrsh) && (s32HistSAD[0] > ((s32HistSAD[1] + s32HistSAD[2] + s32HistSAD[3] + s32HistSAD[4] + s32HistSAD[5] + s32HistSAD[6]) / 6)))
                {
                    bSceneChange = HI_TRUE;
                    s32Count++;
                }
                else
                {
                    bSceneChange = HI_FALSE;
                }
            }
            else
            {
                bSceneChange = HI_TRUE;
            }
        }

        u32CurFrmNum++;
    }
    else
    {
        bSceneChange = HI_FALSE;
        u32CurFrmNum = 0;
    }

    PQ_HAL_UpdataDCIScd(PQ_HAL_LAYER_VID0, bSceneChange);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_UpdatDCICoef(HI_PQ_VDP_LAYER_VID_E enVId)
{
    PQ_HAL_UpdateDCICoef((PQ_HAL_LAYER_VID_E)enVId);
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDciDemoModeCoor(HI_U32 u32XPos)
{
    return PQ_HAL_SetDCIDemoPos(PQ_VDP_LAYER_VID0, u32XPos);
}

static stPQAlgFuncs stDCIFuncs =
{
    .Init               = PQ_MNG_InitDCI,
    .DeInit             = PQ_MNG_DeinitDCI,
    .SetEnable          = PQ_MNG_EnableDCI,
    .GetEnable          = PQ_MNG_GetDCIEnable,
    .SetDemo            = PQ_MNG_EnableDCIDemo,
    .GetDemo            = PQ_MNG_GetDCIDemo,
    .SetDemoMode        = PQ_MNG_SetDCIDemoMode,
    .GetDemoMode        = PQ_MNG_GetDCIDemoMode,
    .SetDemoModeCoor    = PQ_MNG_SetDciDemoModeCoor,
    .SetStrength        = PQ_MNG_SetDCILevelGain,
    .GetStrength        = PQ_MNG_GetDCILevelGain,
    .EnableDciBS        = PQ_MNG_EnableDciBS,
    .UpdateDCIWin       = PQ_MNG_UpdateDCIWin,
    .CalDCIScd          = PQ_MNG_CalDCIScd,
    .UpdatDCICoef       = PQ_MNG_UpdatDCICoef,
    .SetResume          = PQ_MNG_DCILUTInit,
    .SetDciScd          = PQ_MNG_SetDciScd,
    .GetDciScd          = PQ_MNG_GetDciScd,
};

HI_S32 PQ_MNG_RegisterDCI(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DCI, enType, PQ_BIN_ADAPT_SINGLE, "dci", HI_NULL, &stDCIFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDCI(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DCI);

    return s32Ret;
}


