/******************************************************************************

  Copyright (C), 2012-2016, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_db.c
  Version       : Initial Draft
  Author        : wangting chenxiaopeng
  Created       :
  Description   : De-blocking and De-Ringning

******************************************************************************/
#include <linux/string.h>

#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_mng_db.h"
#include "pq_mng_db_alg.h"

#define DB_VER_MAX_RESOLUTION  576
#define DB_HOR_MAX_RESOLUTION  720
#define DB_BORDER_COUNTER      2048

HI_U8 DbStr_Delta_LutUltraStrong[17] = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 2, 0, 0};
HI_U8 DbStr_Delta_LutStrong[17]      = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0};
HI_U8 DbStr_Delta_LutMiddle[17]      = {0, 8, 8, 8, 8, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
HI_U8 DbStr_Delta_LutWeak[17]        = {0, 2, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
HI_U8 DbStr_Delta_LutLumVer[17]      = {6, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
HI_U8 DbStr_Delta_LutChroma[17]      = {8, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

HI_U8 g_DbDirStrLut[16]    = {8, 8, 8, 8, 8, 8, 8, 6, 1, 1, 0, 0, 0, 0, 0, 0};
HI_U8 g_DbDirStrGainLut[8] = {8, 5, 3, 1, 0, 0, 0, 0};

HI_U8 DbStrFadLut1[12]     = {4, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0};
HI_U8 DbStrFadLut1_4[12]   = {4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
HI_U8 DbStrFadLut2[12]     = {4, 4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0};
HI_U8 DbStrFadLut3[12]     = {4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0};
HI_U8 DbStrFadLut4[4]      = {2, 1, 0, 0}; //only for org vertical

HI_U8  RatIdxLut[16] = {0, 1, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8};
HI_U16 ptrPixValAdjGain[9]   = { 4,  5,  7,  8,  9, 10, 11, 12, 12};
HI_U16 ptrHFVarCore[9]       = { 0,  3,  5,  8, 11, 13, 16, 20, 24}; /* texture Th fish still strong, other patent OK */
HI_U16 ptrHFVarGain1[9]      = { 4,  3,  3,  2,  1,  1,  0,  0,  0}; /* [0,8] */
HI_U16 ptrHFVarGain2[9]      = { 8,  7,  7,  6,  5,  5,  4,  4,  4}; /* [0,8] */
HI_U16 ptrBordAdjGain[9]     = { 7,  7,  7,  7,  7,  7,  7,  7,  7}; /* no change */
HI_U8 ptrGradSubRatio[9]     = {24, 24, 24, 24, 24, 24, 24, 16,  8};
/* special case for ver */
HI_U16 ptrSWPixValAdjGain[9] = { 4,  5,  6,  7,  8,   9, 10,  11, 12};
HI_U16 ptrSWHFVarCore[9]     = { 0,  3,  6,  9, 12,  15, 18,  21, 24};
HI_U16 ptrSWHFVarGain1[9]    = { 4,  4,  3,  3,  2,   2,  1,   1,  0};
HI_U16 ptrSWHFVarGain2[9]    = { 8,  8,  7,  7,  6,   6,  5,   5,  4};

#define DB_SIZE_CLIP(x) ((x>0x3F)?(-1):(x))

HI_U32  g_u32DbStr = 8;

DB_DET_INFO_S     g_astDbDetInf[VPSS_HANDLE_NUM];
DB_API_REG_S      g_astDBValue[VPSS_HANDLE_NUM];
DB_DET_INFO_S     g_astDbDetInf_Out[VPSS_HANDLE_NUM];
DetStaticUsing_S  g_astDetUsingInf[VPSS_HANDLE_NUM];

static HI_BOOL        sg_bDBInitFlag   = HI_FALSE;
static HI_BOOL        sg_bDB_EnFlag;
static HI_BOOL        sg_bDB_Demo_EnFlag;
static HI_U32         sg_u32DBStrength = 50;
static PQ_DEMO_MODE_E sg_enMode        = PQ_DEMO_ENABLE_R;
static HI_U32         sg_u32PreGMotion[VPSS_HANDLE_NUM];

//const static HI_U8 DB_DELTA_LUM_VER[16]          = {6, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0}; /* LUMA_VER p0-p15 */
const static HI_U8 DB_DELTA_LUM_VER[16]            = {0, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* LUMA_VER p0-p15 */
const static HI_U8 DB_DELTA_CHR_HOR[16]            = {8, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* CHR_HOR  p0-p15 */
const static HI_U8 DB_DELTA_LUM_H_STRONG[16]       = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0}; /* LUMA_HOR p0-p15 */
const static HI_U8 DB_DELTA_LUM_H_ULTRA_STRONG[16] = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 2, 0}; /* LUMA_HOR p0-p15 */

/* 遗留问题: Fade强度调小，会不会影响水平方向的滤波强度 */
//const static HI_U8 DB_STR_FADE_LUM_VER[12] = {4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* LUMA_VER p0-p11 */
const static HI_U8 DB_STR_FADE_LUM_VER[12]   = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* LUMA_VER p0-p11 */
const static HI_U8 DB_STR_FADE_LUT_1[12]     = {4, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0};
const static HI_U8 DB_STR_FADE_LUT_2[12]     = {4, 4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0};
const static HI_U8 DB_STR_FADE_LUT_3[12]     = {4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0};

const static HI_U8 DB_DIR_GAIN_LUT[8]    = {8, 5, 3, 1, 0, 0, 0, 0}; /* dir_str_gain_lut p0-p7 */
const static HI_U8 DB_DIR_LUT[16]        = {8, 8, 8, 8, 8, 8, 8, 6, 1, 1, 0, 0, 0, 0, 0, 0}; /* dir_str_lut p0-p15 */
const static HI_U8 RAT_INDEX_LUT[16]     = {0, 1, 2, 3, 4, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8};
const static DB_DynmcPara STRONG_PARAM   = {12, 0, 0, 0, 24, 0, 4, 7, 8};
const static DB_DynmcPara MID_PARAM      = {12, 0, 0, 0, 16, 0, 4, 7, 24};
const static DB_DynmcPara WEAK_PARAM     = {4, 0, 8, 8, 0, 4, 8, 7, 24};


/* 获取DB模块的初始参数值 */
static HI_S32 DB_Init_Value(DB_PARAM_S *pstDBValue)
{
    HI_U8 lutweight[16] = {0, 0, 1, 2, 5, 7, 10, 12, 12, 13, 13, 14, 14, 15, 15, 15};

    PQ_CHECK_NULL_PTR(pstDBValue);

    pstDBValue->u16DbmOutMode = 4;

    pstDBValue->DetSizeEn = HI_TRUE;
    pstDBValue->hyEn      = HI_TRUE;
    pstDBValue->vyEn      = HI_FALSE;
    pstDBValue->hchrEn    = HI_FALSE;
    pstDBValue->vchrEn    = HI_FALSE;

    pstDBValue->HyRationBorderText   = 20; //[8,32]/8
    pstDBValue->VyRationBorderText   = 16; //[8,32]/8
    pstDBValue->HchrRationBorderText = 16; //[8,32]/8
    pstDBValue->VchrRationBorderText = 16; //[8,32]/8

    pstDBValue->Hythreshold_edge   = 32;
    pstDBValue->Vythreshold_edge   = 32;
    pstDBValue->Hchrthreshold_edge = 32;
    pstDBValue->Vchrthreshold_edge = 32;

    pstDBValue->Hythreshold_texture   = 16;
    pstDBValue->Hchrthreshold_texture = 16;
    pstDBValue->Vythreshold_texture   = 16;
    pstDBValue->Vchrthreshold_texture = 16;

    memcpy(pstDBValue->lutweight, lutweight, 16 * sizeof(HI_U8));

    pstDBValue->HyRadix8BinLut0 = 16;
    pstDBValue->HyRadix8BinLut1 = 48;
    pstDBValue->HyRadix8BinLut2 = 80;
    pstDBValue->HyRadix8BinLut3 = 112;
    pstDBValue->HyRadix8BinLut4 = 144;
    pstDBValue->HyRadix8BinLut5 = 176;
    pstDBValue->HyRadix8BinLut6 = 208;

    pstDBValue->VyRadix8BinLut0 = 16;
    pstDBValue->VyRadix8BinLut1 = 48;
    pstDBValue->VyRadix8BinLut2 = 80;
    pstDBValue->VyRadix8BinLut3 = 112;
    pstDBValue->VyRadix8BinLut4 = 144;
    pstDBValue->VyRadix8BinLut5 = 176;
    pstDBValue->VyRadix8BinLut6 = 208;

    pstDBValue->HchrRadix8BinLut0 = 16;
    pstDBValue->HchrRadix8BinLut1 = 48;
    pstDBValue->HchrRadix8BinLut2 = 80;
    pstDBValue->HchrRadix8BinLut3 = 112;
    pstDBValue->HchrRadix8BinLut4 = 144;
    pstDBValue->HchrRadix8BinLut5 = 176;
    pstDBValue->HchrRadix8BinLut6 = 208;

    pstDBValue->VchrRadix8BinLut0 = 16;
    pstDBValue->VchrRadix8BinLut1 = 48;
    pstDBValue->VchrRadix8BinLut2 = 80;
    pstDBValue->VchrRadix8BinLut3 = 112;
    pstDBValue->VchrRadix8BinLut4 = 144;
    pstDBValue->VchrRadix8BinLut5 = 176;
    pstDBValue->VchrRadix8BinLut6 = 208;
    pstDBValue->MinBlkSize        = 4;
    pstDBValue->Thr_flat          = 16;

    pstDBValue->u8GlobalDbStrengthLum = 8;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDBBlkNumberCfg(DB_PARAM_S *pstDBValue)
{
    PQ_CHECK_NULL_PTR(pstDBValue);

    pstDBValue->u16DbmOutMode = 4;

    pstDBValue->DetSizeEn = HI_TRUE;
    pstDBValue->hyEn      = HI_TRUE;
    pstDBValue->HyRationBorderText   = 20;

    pstDBValue->Hythreshold_edge   = 32;
    pstDBValue->Hythreshold_texture   = 16;

    pstDBValue->HyRadix8BinLut0 = 16;
    pstDBValue->HyRadix8BinLut1 = 48;
    pstDBValue->HyRadix8BinLut2 = 80;
    pstDBValue->HyRadix8BinLut3 = 112;
    pstDBValue->HyRadix8BinLut4 = 144;
    pstDBValue->HyRadix8BinLut5 = 176;
    pstDBValue->HyRadix8BinLut6 = 208;

    pstDBValue->MinBlkSize        = 4;
    pstDBValue->Thr_flat          = 16;

    pstDBValue->u8GlobalDbStrengthLum = 8;

    pstDBValue->HyTestBlkNumber   = pstDBValue->YWidth / 8;
    pstDBValue->VyTestBlkNumber   = pstDBValue->YHeight / 8;

    if (pstDBValue->YWidth <= 720)
    {
        pstDBValue->HyMaxBlkSize = 16;
    }
    else if (pstDBValue->YWidth <= 1920)
    {
        pstDBValue->HyMaxBlkSize = 42;
    }
    else
    {
        pstDBValue->HyMaxBlkSize = 64;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_InitDB(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_U32     u32HandleNo = 0;
    HI_S32     s32Ret;
    DB_PARAM_S stParamValueDB;

    if (HI_TRUE == sg_bDBInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = DB_Init_Value(&stParamValueDB);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DB parameter init error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_DB, SOURCE_MODE_NO, OUTPUT_MODE_NO);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("db InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDBInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeinitDB(HI_VOID)
{
    if (HI_FALSE == sg_bDBInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDBInitFlag   = HI_FALSE;

    memset(sg_u32PreGMotion, 0, sizeof(sg_u32PreGMotion));

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDB(HI_BOOL bOnOff)
{
    HI_U32  u32HandleNo = 0;
    HI_U32  u32Vid      = PQ_VDP_LAYER_VID0;
    HI_S32  s32Ret      = HI_SUCCESS;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DB, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    sg_bDB_EnFlag = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDB(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDB(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDBEnableFlag(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDB_EnFlag;

    return HI_SUCCESS;
}

/* u32Strength[0,100] */
HI_S32 PQ_MNG_SetDBStrength(HI_U32 u32Strength)
{
    sg_u32DBStrength = u32Strength;
    g_u32DbStr       = u32Strength * 15 / 100; /* 0~15; Low:4; Mid:8; High:12 */

    return HI_SUCCESS;
}

/* u32Strength[0,100] */
HI_S32 PQ_MNG_GetDBStrength(HI_U32 *pu32Strength)
{
    *pu32Strength = sg_u32DBStrength; /* 0~100 */

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDBDemo(HI_BOOL bOnOff)
{
    HI_U32 u32HandleNo = 0, u32Vid = PQ_VDP_LAYER_VID0;
    HI_S32 s32Ret = HI_SUCCESS;

    sg_bDB_Demo_EnFlag = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; ++u32HandleNo)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDBDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDBDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDBDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDB_Demo_EnFlag;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDbmDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;

    sg_enMode = enMode;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetDbmDemoMode(u32HandleNo, (DBM_DEMO_MODE_E)enMode);
        }
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDbmDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;
    return HI_SUCCESS;
}


HI_S32 PQ_MNG_SetDbmDemoModeCoor(HI_U32 u32XPos)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DB))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_DBDemoPos(u32HandleNo, u32XPos);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_DBDemoPos(u32Vid, u32XPos);
    }

    return s32Ret;
}


/* software alg  begin*/
static HI_S32 PQ_MNG_InitDbApi(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width)
{
    DB_PARAM_S stParamValueDB;

    DetStaticUsing_S *pApiDetInf = &(g_astDetUsingInf[u32HandleNo]);
    stParamValueDB.YHeight = u32Height;
    stParamValueDB.YWidth  = u32Width;

    pApiDetInf->NumberWindow          = 8;
    pApiDetInf->HyMINBlkNum           = 10;
    pApiDetInf->UsingSizeHy           = 0;
    pApiDetInf->UsingHyCounterBorder  = 0;

    pApiDetInf->frms_NumsHy           = 0;
    pApiDetInf->keyfrm_num            = 6;
    pApiDetInf->HyBordCountThreshold  = 15;
    pApiDetInf->ptrHyCnt              = pApiDetInf->BorderCnt;
    pApiDetInf->frmsCntTempHy         = 0;
    pApiDetInf->frmsWindowTemp        = 4;
    pApiDetInf->ptrUsingHyBorderIndex = pApiDetInf->UsingBorderIndex;

    memset(pApiDetInf->UsingBorderIndex, 0, sizeof(HI_U8) * MAXLINESIZE);
    memset(pApiDetInf->BorderCnt,        0, sizeof(HI_S8) * MAXLINESIZE);

    PQ_MNG_GetDBBlkNumberCfg(&stParamValueDB);
    PQ_HAL_SetDBBlkNumberCfg(u32HandleNo, &stParamValueDB);
    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_DB_API_Init(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{
    if (u32HandleNo >= VPSS_HANDLE_NUM)
    {
        HI_ERR_PQ("u32HandleNo :%d\n", u32HandleNo);
        return HI_FAILURE;
    }

    g_astDbDetInf[u32HandleNo].s32LumWidth  = u32Width;
    g_astDbDetInf[u32HandleNo].s32LumHeight = u32Height;
    g_astDbDetInf[u32HandleNo].s32ChrWidth  = u32Width / 2;
    g_astDbDetInf[u32HandleNo].s32ChrHeight = u32Height / 2;
    g_astDbDetInf[u32HandleNo].DbFrmCnt          = 0;
    g_astDbDetInf[u32HandleNo].DbDetBlkNumWindow = 3;

    PQ_HAL_GetDBDetEnReg(u32HandleNo, &(g_astDbDetInf[u32HandleNo].det_size_en), &(g_astDbDetInf[u32HandleNo].det_hy_en),
                         &(g_astDbDetInf[u32HandleNo].det_hc_en), &(g_astDbDetInf[u32HandleNo].det_vy_en));

    if (u32Height >= DB_VER_MAX_RESOLUTION && u32Width >= DB_HOR_MAX_RESOLUTION)
    {
        g_astDbDetInf[u32HandleNo].det_size_en = 0;
    }

    return PQ_MNG_InitDbApi(u32HandleNo, u32Height, u32Width);
}

/* DB API Init; refresh when timing change */
HI_S32 PQ_MNG_DBCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{
    PQ_MNG_DB_API_Init(u32HandleNo, u32Height, u32Width, u32FRate);

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_GetDbSttInfo(DB_DET_INFO_S *pDBDetInput, HI_PQ_DB_CALC_INFO_S *pstDbCalcInfo)
{
    int i = 0;

    PQ_CHECK_NULL_PTR(pstDbCalcInfo);
    PQ_CHECK_NULL_PTR(pstDbCalcInfo->pstVPSSWbcReg);

    pDBDetInput->hy_counter = pstDbCalcInfo->pstVPSSWbcReg->DB_HY_SIZE.bits.hy_counter;
    pDBDetInput->hy_size    = DB_SIZE_CLIP( pstDbCalcInfo->pstVPSSWbcReg->DB_HY_SIZE.bits.hy_size );
    PQ_HAL_GetDBDetEnReg(pstDbCalcInfo->u32HandleNo, &(pDBDetInput->det_size_en), &(pDBDetInput->det_hy_en),
                         &(pDBDetInput->det_hc_en), &(pDBDetInput->det_vy_en));

    for (i = 0; i < 128; i++)
    {
        pDBDetInput->db_border[i] = pstDbCalcInfo->pstVPSSWbcReg->DB_BORDER[i].u32;
    }

    for (i = 0; i < DB_BORDER_COUNTER; i++)
    {
        pDBDetInput->db_hy_counter[i] = pstDbCalcInfo->pstVPSSWbcReg->DB_HY_COUNTER[i].u32;
    }

    return HI_SUCCESS;
}

HI_S32 DB_CountBorderNum(HI_U8 *pSrcPos, HI_U16 MaxPos)
{
    HI_U16 i, BordCnt;

    for (i = 1, BordCnt = 0; i < MaxPos; i++)
    {
        if (pSrcPos[i] == 1)
        {
            BordCnt++;
        }
    }

    return BordCnt;
}

static HI_U16 SmoothDetNumHy(DB_DET_INFO_S *p_Inf, HI_U16 DetRatioLumH)
{
    HI_U16 SmoothDetNum = DetRatioLumH;
    HI_U16 curCnt = 0, tempCnt = 0;
    HI_U16 tempAvgDetRatioLumH = 0, tempDiffDetRatioLumH = 0;
    HI_U16 tempDiffDetRatioLumHLut[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2};

    curCnt = p_Inf->DbFrmCnt;

    if (curCnt < p_Inf->DbDetBlkNumWindow)
    {
        p_Inf->DBTempDetBlkRatioH[curCnt] = DetRatioLumH;
        p_Inf->DbFrmCnt++;
    }
    else
    {
        for (tempCnt = 0; tempCnt < p_Inf->DbDetBlkNumWindow; tempCnt++)
        {
            tempAvgDetRatioLumH += p_Inf->DBTempDetBlkRatioH[tempCnt];
        }
        tempAvgDetRatioLumH  = tempAvgDetRatioLumH / (p_Inf->DbDetBlkNumWindow);
        tempDiffDetRatioLumH = ABS(DetRatioLumH - tempAvgDetRatioLumH);
        tempDiffDetRatioLumH = tempDiffDetRatioLumHLut[tempDiffDetRatioLumH];
        if (DetRatioLumH > tempAvgDetRatioLumH)
        {
            SmoothDetNum = DBM_CLIP3(0, DetRatioLumH, tempAvgDetRatioLumH + tempDiffDetRatioLumH);
        }
        else
        {
            SmoothDetNum = DBM_CLIP3(DetRatioLumH, tempAvgDetRatioLumH, tempAvgDetRatioLumH - tempDiffDetRatioLumH);
        }

        for (tempCnt = 0; tempCnt < p_Inf->DbDetBlkNumWindow - 1; tempCnt++)
        {
            p_Inf->DBTempDetBlkRatioH[tempCnt] = p_Inf->DBTempDetBlkRatioH[tempCnt + 1];
        }

        p_Inf->DBTempDetBlkRatioH[p_Inf->DbDetBlkNumWindow - 1] = SmoothDetNum;
    }

    return SmoothDetNum;
}

static HI_VOID CalcStrIdx(DB_API_REG_S *pInput, DB_DET_INFO_S *p_Inf, HI_U16 *pStrIdxLumH, HI_U16 *pStrIdxLumV, HI_U16 *pStrIdxChr)
{
    HI_U16 DetRatioLumH  = 0, DetRatioLumV = 0, DetRatioChr = 0;
    DB_REG_S *pReg = &(pInput->stDbRegs);

    if (p_Inf->hy_size >= 4 && pReg->u8DbEn == 1)
    {
        pInput->lum_h_blk_size = p_Inf->hy_size;
        pReg->s32LumHBlkNum    = p_Inf->s32LumHBlkNum; //DB_CountBorderNum(pInput->vpss_db_bord_flag, pReg->YWidth) + 1;
        DetRatioLumH           = 16 * p_Inf->hy_counter / pReg->s32LumHBlkNum;
        DetRatioLumH           = DBM_CLIP3(0, 15, DetRatioLumH);
        DetRatioLumH = SmoothDetNumHy(p_Inf, DetRatioLumH);
        *pStrIdxLumH = RatIdxLut[DetRatioLumH];
    }
    else
    {
        pReg->s32LumHBlkNum = 0;
        *pStrIdxLumH = 0;
    }

    if (p_Inf->vy_size >= 4 && pReg->u8DbEn == 1)
    {
        pReg->s32LumVBlkNum = DB_CountBorderNum(pReg->pLumVerBordFlag, pReg->u32DBYHeight) + 1;
        DetRatioLumV        = 16 * p_Inf->vy_counter / pReg->s32LumVBlkNum;
        DetRatioLumV        = DBM_CLIP3(0, 15, DetRatioLumV);
        /* refer to lum hor */
        *pStrIdxLumV = DBM_CLIP3(0, 8, (RatIdxLut[DetRatioLumV] * (*pStrIdxLumH)) / 8);
    }
    else
    {
        pReg->s32LumVBlkNum = 0;
        *pStrIdxLumV = 0;
    }

    if (p_Inf->hc_size >= 4 && pReg->u8DbEn == 1)
    {
        pReg->s32ChrHBlkNum = DB_CountBorderNum(pReg->pChrHorBordFlag, pReg->CWidth) + 1;
        DetRatioChr         = 16 * p_Inf->hc_counter / pReg->s32ChrHBlkNum;
        DetRatioChr         = DBM_CLIP3(0, 15, DetRatioChr);
        *pStrIdxChr = DBM_CLIP3(0, 8, (RatIdxLut[DetRatioChr] * (*pStrIdxLumH)) / 8);
    }
    else
    {
        pReg->s32ChrHBlkNum = 0;
        *pStrIdxChr = 0;
    }
}

static HI_VOID SetHyPara_WinSize_FadeLut_DbDist(DB_REG_S *pReg, HI_S32 BlkSizeHy)
{
    pReg->u8DBLumHorTrue     = pReg->u8DBLumHorEn && HI_TRUE;
    pReg->u8LumHorScaleRatio = (BlkSizeHy + 7) / 8; /* ceil the fraction part to up side integer */
    pReg->u8LumHorFilterSel  = pReg->u8LumHorScaleRatio;

    switch (pReg->u8LumHorScaleRatio)
    {
        case 0:
            pReg->u8LumHorTxtWinSize = 3;
            pReg->pLumHStrFadeLut    = DbStrFadLut1;
            pReg->u8MaxLumHorDbDist  = 2;
            break;
        case 1:
            pReg->u8LumHorTxtWinSize = 4;
            pReg->pLumHStrFadeLut    = DbStrFadLut1;
            pReg->u8MaxLumHorDbDist  = 4;
            if (BlkSizeHy == 4)
            {
                pReg->pLumHStrFadeLut = DbStrFadLut1_4;
            }
            break;
        case 2:
            pReg->u8LumHorTxtWinSize = 8;
            pReg->pLumHStrFadeLut    = DbStrFadLut2;
            pReg->u8MaxLumHorDbDist  = 8;
            break;
        case 3:
        default:
            pReg->u8LumHorTxtWinSize = 12;
            pReg->pLumHStrFadeLut    = DbStrFadLut3;
            pReg->u8MaxLumHorDbDist  = 12;
            break;
    }

    pReg->u8MaxLumHorDbDist = DBM_MIN(BlkSizeHy / 2, pReg->u8MaxLumHorDbDist);
}

static HI_VOID SetHyPara_DeltaLut(DB_REG_S *pReg)
{
    if (pReg->u8LumHorScaleRatio <= 1)
    {
        /* origin */
        pReg->pLumHorDeltaLut = DbStr_Delta_LutStrong;
    }
    else
    {
        pReg->pLumHorDeltaLut = DbStr_Delta_LutUltraStrong;
    }
}

static HI_VOID SetHyPara(DB_REG_S *pReg, HI_S32 BlkSizeHy, HI_U16 StrIdxLumH)
{
    SetHyPara_WinSize_FadeLut_DbDist(pReg, BlkSizeHy);
    SetHyPara_DeltaLut(pReg);
    /* dynamic adjustment just search the lut, change it with the stridx.
           since we can't see the strength diff, maybe we can set the same para for different strength
           */
    pReg->u16LumHorAdjGain     = ptrPixValAdjGain[StrIdxLumH];
    pReg->u16LumHorHFVarCore   = ptrHFVarCore[StrIdxLumH];
    pReg->u16LumHorHFVarGain1  = ptrHFVarGain1[StrIdxLumH];
    pReg->u16LumHorHFVarGain2  = ptrHFVarGain2[StrIdxLumH];
    pReg->u16LumHorBordAdjGain = ptrBordAdjGain[StrIdxLumH];
    pReg->u8GradSubRatio       = ptrGradSubRatio[StrIdxLumH];
    pReg->u16LumHorHFDiffCore  = 0;
    pReg->u16LumHorHFDiffGain1 = 0;
    pReg->u16LumHorHFDiffGain2 = 0;

    return;
}

static HI_VOID SetHchrPara_FadeLut_DbDist(DB_REG_S *pReg, HI_S32 BlkSizeHchr)
{
    switch (pReg->u8ChrHorScaleRatio)
    {
        case 0:
            pReg->pChrHStrFadeLut   = DbStrFadLut1;
            pReg->u8MaxChrHorDbDist = 2;
            break;
        case 1:
            pReg->pChrHStrFadeLut   = DbStrFadLut1;
            pReg->u8MaxChrHorDbDist = 4;
            if (BlkSizeHchr == 4)
            {
                pReg->pChrHStrFadeLut   = DbStrFadLut1_4;
                pReg->u8MaxChrHorDbDist = 2;
            }
            break;
        case 2:
            pReg->pChrHStrFadeLut   = DbStrFadLut2;
            pReg->u8MaxChrHorDbDist = 8;
            break;
        case 3:
        default:
            pReg->pChrHStrFadeLut   = DbStrFadLut3;
            pReg->u8MaxChrHorDbDist = 12;
            break;
    }

    return;
}

static HI_VOID SetHchrPara(DB_REG_S *pReg, HI_S32 BlkSizeHchr)
{
    pReg->u8DBChrHorTrue     = pReg->u8DBChrHorEn && HI_TRUE;
    pReg->u8ChrHorScaleRatio = (BlkSizeHchr + 7) / 8; /* ceil the fraction part to up side integer */
    pReg->u8ChrHorFilterSel  = pReg->u8ChrHorScaleRatio;
    pReg->u16ChrHorAdjGain   = 12;
    pReg->u8MaxChrHorDbDist  = (pReg->u8ChrHorScaleRatio == 0) ? 2 : DBM_MIN(BlkSizeHchr / 2, 4 * pReg->u8ChrHorScaleRatio - 1);
    pReg->pChrHorDeltaLut    = DbStr_Delta_LutChroma;

    SetHchrPara_FadeLut_DbDist(pReg, BlkSizeHchr);

    return;
}

static HI_VOID SetVyPara(DB_REG_S *pReg, HI_S32 BlkSizeVy, HI_U16 StrIdxLumV)
{
    pReg->u8DBLumVerTrue       = pReg->u8DBLumVerEn && HI_TRUE;
    pReg->u8LumVerScaleRatio   = (BlkSizeVy + 7) / 8; /* ceil the fraction part to up side integer */
    pReg->u8LumVerTxtWinSize   = 3;
    pReg->pLumVStrFadeLut      = DbStrFadLut4;
    //pReg->pLumVerDeltaLut    = DbStr_Delta_LutLumVer_msl;//0 8 8 8 4 2
    pReg->pLumVerDeltaLut      = DbStr_Delta_LutLumVer;
    pReg->u16LumVerAdjGain     = ptrSWPixValAdjGain[StrIdxLumV];
    pReg->u16LumVerHFVarCore   = ptrSWHFVarCore[StrIdxLumV];
    pReg->u16LumVerHFVarGain1  = ptrSWHFVarGain1[StrIdxLumV];
    pReg->u16LumVerHFVarGain2  = ptrSWHFVarGain2[StrIdxLumV];
    pReg->u16LumVerBordAdjGain = 4;

    return;
}

static HI_S32 PQ_MNG_DBCfgFilter(DB_API_REG_S *pInput, DB_DET_INFO_S *p_Inf, DB_CALC_INFO_S *pstDbCalcInfo)
{
    HI_U16 StrIdxLumH  = 0;
    HI_U16 StrIdxLumV  = 0;
    HI_U16 StrIdxChr   = 0;

    DB_REG_S *pReg = &(pInput->stDbRegs);

    /* set default value */
    pReg->u8GradSubRatio = 24;
    pReg->u8CtrstThresh  = 2;
    memcpy(pReg->u8DirStrGainLut, g_DbDirStrGainLut, sizeof(g_DbDirStrGainLut));
    memcpy(pReg->u8DirStrLut,     g_DbDirStrLut,     sizeof(g_DbDirStrLut));
    pReg->pLumHStrFadeLut = DbStrFadLut1;
    pReg->pChrHStrFadeLut = DbStrFadLut1;
    pReg->pLumVStrFadeLut = DbStrFadLut1;
    pReg->pChrVStrFadeLut = DbStrFadLut1;
    pReg->pLumHorDeltaLut = HI_NULL;
    /* calc det ratio to get strength estimation */
    /* luma hor */
    pReg->u8DbEn       = 1;
    pReg->u8DBLumHorEn = 1;

    CalcStrIdx(pInput, p_Inf, &StrIdxLumH, &StrIdxLumV, &StrIdxChr);
    /* hy */
    if ((p_Inf->hy_size >= 4) && (pReg->u8DBLumHorEn))
    {
        SetHyPara(pReg, p_Inf->hy_size, StrIdxLumH);
    }
    else
    {
        pReg->u8DBLumHorTrue = HI_FALSE;
    }
    /* hchr */
    if ((p_Inf->hc_size >= 4) && (pReg->u8DBChrHorEn))
    {
        SetHchrPara(pReg, p_Inf->hc_size);
    }
    else
    {
        pReg->u8DBChrHorTrue = HI_FALSE;
    }
    /* vy */
    if ((p_Inf->vy_size >= 4) && (pReg->u8DBLumVerEn))
    {
        SetVyPara(pReg, p_Inf->vy_size, StrIdxLumV);
    }
    else
    {
        pReg->u8DBLumVerTrue = HI_FALSE;
    }
    /* vchr */
    pReg->u8DBChrVerTrue = HI_FALSE;

    /* to be added for adaptive on global strength */
    pReg->u16CtrstAdjCore  = 0;
    pReg->u16CtrstAdjGain1 = 0;
    pReg->u16CtrstAdjGain2 = 8;
    pReg->u16DirSmoothMode = 2;
    /* process for 10 bit precision;
       since S5V200 don't has enough bits for these parameters, so close this shift ops
       but in S5V200L we can do more in this case, so we can rebuild it
       */
    /* total switch */
    pReg->u8DBLumTrue = (pReg->u8DBLumHorTrue || pReg->u8DBLumVerTrue);
    pReg->u8DBCbTrue  = (pReg->u8DBChrHorTrue || pReg->u8DBChrVerTrue);
    pReg->u8DBCrTrue  = (pReg->u8DBChrHorTrue || pReg->u8DBChrVerTrue);

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_DBCfg(DB_API_REG_S *pInput, DB_DET_INFO_S *p_Inf, DetStaticUsing_S *pDetUsing_info, HI_PQ_DB_CALC_INFO_S *pstDbCalcInfo)
{
    PQ_MNG_ALG_DBCfgDetector(pInput, p_Inf, pDetUsing_info);

    PQ_MNG_DBCfgFilter(pInput, p_Inf, (DB_CALC_INFO_S *)pstDbCalcInfo);

    return HI_SUCCESS;
}


/* refresh every frame */
HI_S32 PQ_MNG_DBCalcConfigCfg(HI_PQ_DB_CALC_INFO_S *pstDbCalcInfo)
{
    PQ_CHECK_NULL_PTR(pstDbCalcInfo);
    /* read reg value from VPSS WBC stt, tranfer to g_astDbDetInf */
    PQ_MNG_GetDbSttInfo(&g_astDbDetInf[pstDbCalcInfo->u32HandleNo], pstDbCalcInfo);

    /* Calc counter and size, Update result */
    PQ_MNG_ALG_GetDbDetInfo(&g_astDbDetInf_Out[pstDbCalcInfo->u32HandleNo], &g_astDetUsingInf[pstDbCalcInfo->u32HandleNo], &g_astDbDetInf[pstDbCalcInfo->u32HandleNo]);

    /* Calc to detection and filter */
    PQ_MNG_DBCfg(&g_astDBValue[pstDbCalcInfo->u32HandleNo], &g_astDbDetInf_Out[pstDbCalcInfo->u32HandleNo], &g_astDetUsingInf[pstDbCalcInfo->u32HandleNo], pstDbCalcInfo);

    /* DB detect and filter para Info Set to Reg After Calc */
    PQ_HAL_SetDBApiReg(pstDbCalcInfo->u32HandleNo, &g_astDBValue[pstDbCalcInfo->u32HandleNo]);

    return HI_SUCCESS;
}
/* software alg  end*/

static stPQAlgFuncs stDbFuncs =
{
    .Init               = PQ_MNG_InitDB,
    .DeInit             = PQ_MNG_DeinitDB,
    .SetEnable          = PQ_MNG_EnableDB,
    .GetEnable          = PQ_MNG_GetDBEnableFlag,
    .SetStrength        = PQ_MNG_SetDBStrength,
    .GetStrength        = PQ_MNG_GetDBStrength,
    .SetDemo            = PQ_MNG_EnableDBDemo,
    .GetDemo            = PQ_MNG_GetDBDemo,
    .SetDemoMode        = PQ_MNG_SetDbmDemoMode,
    .GetDemoMode        = PQ_MNG_GetDbmDemoMode,
    .SetDemoModeCoor    = PQ_MNG_SetDbmDemoModeCoor,
    .DBCfgRefresh       = PQ_MNG_DBCfgRefresh,
    .DBCalcConfigCfg    = PQ_MNG_DBCalcConfigCfg,

};

HI_S32 PQ_MNG_RegisterDB(PQ_REG_TYPE_E enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DB, enType, PQ_BIN_ADAPT_SINGLE, "db", HI_NULL, &stDbFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDB(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DB);

    return s32Ret;
}


