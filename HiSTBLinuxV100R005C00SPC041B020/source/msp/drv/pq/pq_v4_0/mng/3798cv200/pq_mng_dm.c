/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dm.c
  Version       : Initial Draft
  Author        :
  Created       :
  Description   : De-Mosquito

******************************************************************************/
#include <linux/string.h>

#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_mng_db_alg.h"
#include "pq_mng_db.h"
#include "pq_mng_dm.h"

#define DBM_Blend(F,B,An,Ad)  (F*An + B*(Ad-An) + Ad/2)/Ad

extern HI_U32           g_u32DmStr;
extern DetStaticUsing_S g_astDetUsingInf[VPSS_HANDLE_NUM];
extern DB_DET_INFO_S    g_astDbDetInf[VPSS_HANDLE_NUM];

static HI_BOOL sg_bDM_EnFlag;
static HI_BOOL sg_bDM_Demo_EnFlag;
static HI_BOOL sg_bDMInitFlag       = HI_FALSE;
static HI_U32  sg_u32DMStrength     = 50;

static const HI_U8 DM_SW_WHT_LUT[5]       = {4, 8, 16, 32, 64};
static const HI_U8 DM_DIR_STR_GAIN_LUT[8] = {8, 8, 8, 8, 8, 8, 6, 5};
static const HI_U8 DM_DIR_STR_LUT[16]     = {8, 8, 8, 8, 8, 8, 8, 6, 1, 1, 0, 0, 0, 0, 0, 0};
static DM_CFG_INFO_S sg_stDmCfgInfo;

DM_INF_S     g_stDmInf[VPSS_HANDLE_NUM];
DM_API_REG_S g_stDmApiReg;

HI_U16 g_DmDirStrGainLut[8] = {8, 8, 8, 8, 8, 8, 6, 5};
HI_U16 g_DmDirStrLut[16]    = {8, 8, 8, 8, 8, 8, 8, 6, 1, 1, 0, 0, 0, 0, 0, 0};
HI_U16 g_DmSwWhtLut[5]      = {4, 8, 16, 32, 64}; /* original g_DmSwWhtLut[5] setting *//* 8 bit MV410 setting */




/* 获取DR模块的初始参数值 */
static HI_S32 DM_Init_Value(DM_PARAM_S* pstDRValue)
{
    PQ_CHECK_NULL_PTR(pstDRValue);
    /* set flag */
    sg_bDM_EnFlag      = HI_TRUE;
    sg_bDM_Demo_EnFlag = HI_FALSE;

    pstDRValue->bDM_En = 1;

    //VPSS_DM_DIR
    pstDRValue->u8Dm_grad_sub_ratio = 24;
    pstDRValue->u8Dm_ctrst_thresh   = 2;
    pstDRValue->u8Dm_str_idx_t      = 4;
    pstDRValue->u8Dm_limit_t        = 8;

    //VPSS_DM_EDGE
    pstDRValue->u8Dm_str_idx       = 0; /* 临时计算值，后面会被改写 */
    pstDRValue->u8Dm_global_str    = 8;
    pstDRValue->u8Dm_edge_thr      = DBM_Blend(0, 60, pstDRValue->u8Dm_str_idx, 8);
    pstDRValue->u8Dm_asymtrc_gain  = 1 << (pstDRValue->u8Dm_str_idx / 2);
    pstDRValue->u8Dm_edge_gain1    = DBM_Blend(4, 0, pstDRValue->u8Dm_str_idx, 8) ;
    pstDRValue->u8Dm_edge_gain2    = DBM_Blend(5, 1, pstDRValue->u8Dm_str_idx, 8);
    pstDRValue->u8Dm_sw_thr_gain   = ALG_CLIP3(2, 4, (4 - pstDRValue->u8Dm_str_idx / 2));

    pstDRValue->pu8DmSwWhtLut      = DM_SW_WHT_LUT;
    pstDRValue->pu8DmDirStrGainLut = DM_DIR_STR_GAIN_LUT;
    pstDRValue->pu8DmDirStrLut     = DM_DIR_STR_LUT;


    return HI_SUCCESS;
}

/* Init DM, Vpss Handle ID = 0, Other Handle Init At Top Layer */
HI_S32 PQ_MNG_InitDM(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_U32 u32HandleNo = 0;
    DM_PARAM_S stParamValueDR;
    HI_S32 s32Ret;

    if (HI_TRUE == sg_bDMInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = DM_Init_Value(&stParamValueDR);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DM parameter init error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_DM, SOURCE_MODE_NO, OUTPUT_MODE_NO);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("dm InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDMInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeinitDM(HI_VOID)
{
    if (HI_FALSE == sg_bDMInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDMInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDM(HI_BOOL bOnOff)
{
    HI_U32  u32HandleNo = 0;
    HI_S32  s32Ret = HI_SUCCESS;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_DM, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    sg_bDM_EnFlag = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDM(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDM(PQ_VDP_LAYER_VID0, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDMEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDM_EnFlag;
    return HI_SUCCESS;
}

/* u32Strength: 0~100 */
HI_S32 PQ_MNG_SetDMStrength(HI_U32 u32Strength)
{
    sg_u32DMStrength = u32Strength;
    g_u32DmStr       = u32Strength * 15 / 100; /* 0~15; Low:4; Mid:8; High:12 */

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDMStrength(HI_U32* pu32Strength)
{
    *pu32Strength = sg_u32DMStrength;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableDMDemo(HI_BOOL bOnOff)
{
    HI_U32 u32HandleNo = 0, u32Vid = PQ_VDP_LAYER_VID0;
    HI_S32 s32Ret = HI_SUCCESS;

    sg_bDM_Demo_EnFlag = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_DM))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; ++u32HandleNo)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableDMDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableDMDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetDMDemo(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDM_Demo_EnFlag;
    return HI_SUCCESS;
}

static HI_S32 DBM_ReadDmPara(DetStaticUsing_S* pMnDetInf, DB_DET_INFO_S* s_DbDetInf_In, DM_INF_S* pInf)
{
    PQ_CHECK_NULL_PTR(pMnDetInf);
    PQ_CHECK_NULL_PTR(s_DbDetInf_In);

    //pInf->s32RateSmthWin = pMnDetInf->NumberWindow;
    pInf->s32RateSmthWin   = sg_stDmCfgInfo.u8NumWindow;
    pInf->u32LumWidth      = s_DbDetInf_In->s32LumWidth;
    pInf->u32LumHeight     = s_DbDetInf_In->s32LumHeight;

    /* compatible error: when using info from db detection */
    if (pMnDetInf->UsingSizeHy > 64 || pMnDetInf->UsingSizeHy < 4)
    {
        pInf->s32LumHBlkSize = 0;
    }
    else
    {
        pInf->s32LumHBlkSize = pMnDetInf->UsingSizeHy;
    }

    pInf->s32LumHBlkDetNum = pMnDetInf->UsingHyCounterBorder;

    return HI_SUCCESS;
}


HI_S32 PQ_MNG_DMCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{

    sg_stDmCfgInfo.u8NumWindow = 8;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DMCalcConfigCfg(HI_PQ_DB_CALC_INFO_S* pstDmCalcInfo)
{
    PQ_CHECK_NULL_PTR(pstDmCalcInfo);
    DBM_ReadDmPara(&g_astDetUsingInf[pstDmCalcInfo->u32HandleNo], &g_astDbDetInf[pstDmCalcInfo->u32HandleNo], &g_stDmInf[pstDmCalcInfo->u32HandleNo]);

    pstDmCalcInfo->u32Protocol = 4; /* 4: mndet mode */
    PQ_MNG_ALG_UpdateDMCfg(&g_stDmInf[pstDmCalcInfo->u32HandleNo], &g_stDmApiReg, (DB_CALC_INFO_S*)pstDmCalcInfo);
    PQ_HAL_SetReg_Dm_Dir_Str_Gain_Lut(pstDmCalcInfo->u32HandleNo, g_DmDirStrGainLut);
    PQ_HAL_SetReg_Dm_Dir_Str_Lut(pstDmCalcInfo->u32HandleNo, g_DmDirStrLut);
    PQ_HAL_SetReg_Dm_Sw_Wht_Lut(pstDmCalcInfo->u32HandleNo, g_DmSwWhtLut);
    PQ_HAL_SetDMApiReg(pstDmCalcInfo->u32HandleNo, &g_stDmApiReg);

    return HI_SUCCESS;
}

static stPQAlgFuncs stDmFuncs =
{
    .Init               = PQ_MNG_InitDM,
    .DeInit             = PQ_MNG_DeinitDM,
    .SetEnable          = PQ_MNG_EnableDM,
    .GetEnable          = PQ_MNG_GetDMEnableFlag,
    .SetStrength        = PQ_MNG_SetDMStrength,
    .GetStrength        = PQ_MNG_GetDMStrength,
    .SetDemo            = PQ_MNG_EnableDMDemo,
    .GetDemo            = PQ_MNG_GetDMDemo,
    .DMCfgRefresh       = PQ_MNG_DMCfgRefresh,
    .DMCalcConfigCfg    = PQ_MNG_DMCalcConfigCfg,
};

HI_S32 PQ_MNG_RegisterDM(PQ_REG_TYPE_E enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_DM, enType, PQ_BIN_ADAPT_MULTIPLE, "dm", HI_NULL, &stDmFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterDM(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_DM);

    return s32Ret;
}

