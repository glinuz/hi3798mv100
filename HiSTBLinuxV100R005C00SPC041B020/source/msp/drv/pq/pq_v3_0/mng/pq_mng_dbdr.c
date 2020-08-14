/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_dbdr.c
  Version       : Initial Draft
  Author        :
  Created       : 2014-12-23
  Description   : De-blocking and De-Ringning

******************************************************************************/
#include "pq_mng_dbdr.h"
#include "drv_pq_table.h"


static PQ_PARAM_S*    g_pstPqParam   = HI_NULL;
static DB_TUN_MODE_E  sg_enDBTunMode = DB_TUN_NORMAL;


static HI_BOOL sg_bDB_EnFlag;
static HI_BOOL sg_bDR_EnFlag;
static HI_BOOL sg_bDNRInitFlag = HI_FALSE;
static HI_BOOL sg_bDR_Demo_EnFlag;


HI_S32 PQ_MNG_SetDbTunMode(DB_TUN_MODE_E enDBTunMode)
{
    sg_enDBTunMode = enDBTunMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDbTunMode(DB_TUN_MODE_E* enDBTunMode)
{
    *enDBTunMode = sg_enDBTunMode;

    return HI_SUCCESS;
}

/* SD DR init Para */
static HI_VOID PQ_MNG_DRParaInit(HI_VOID)
{
    return;
}

/* Init SD DR */
HI_S32 PQ_MNG_InitDNR(PQ_PARAM_S* pstPqParam)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    g_pstPqParam = pstPqParam;

    PQ_MNG_DRParaInit();

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_DNR);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("SD DR InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Deinit SD DR */
HI_S32 PQ_MNG_DeinitDNR(HI_VOID)
{
    if (HI_FALSE == sg_bDNRInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

/* Set DR Enable *//* Notice: DR Enable or not relate to resolution; */
HI_S32 PQ_MNG_EnableDR(HI_BOOL bOnOff)
{
    HI_S32  s32Ret;
    HI_BOOL bDrEn;
    HI_U32  u32HandleNo = 0;

    s32Ret = PQ_HAL_GetDREnReg(u32HandleNo, &bDrEn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Get DR_en Failure!\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == bDrEn)
    {
        sg_bDR_EnFlag = HI_TRUE;
    }
    else
    {
        sg_bDR_EnFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

/* Get DR Enable state */
HI_S32 PQ_MNG_GetDREnableFlag(HI_BOOL* pbOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo = 0;

    s32Ret = PQ_HAL_GetDREnReg(u32HandleNo, pbOnOff);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in get DR enable!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Enable DR DEMO */
HI_S32 PQ_MNG_EnableDRDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableDRDemo(u32HandleNo, bOnOff);
        }
    }
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable DR Demo!\n");
        return HI_FAILURE;
    }
    sg_bDR_Demo_EnFlag = bOnOff;

    return HI_SUCCESS;
}

/* Enable DB *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDB(HI_BOOL bOnOff)
{
    sg_bDB_EnFlag = bOnOff;

    return HI_SUCCESS;
}

/* Get DB Enable state *//* no practical meaning*/
HI_S32 PQ_MNG_GetDBEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);

    *pbOnOff = sg_bDB_EnFlag;

    return HI_SUCCESS;
}

/* DB DEMO Enable *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDBDemo(HI_BOOL bOnOff)
{
    return HI_SUCCESS;
}

/* Set DM Enable *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDM(HI_BOOL bOnOff)
{
    return HI_SUCCESS;
}

/* Get DM Enable state *//* no practical meaning*/
HI_S32 PQ_MNG_GetDMEnableFlag(HI_BOOL* pbOnOff)
{
    return HI_SUCCESS;
}

/* Enable DM DEMO *//* no practical meaning*/
HI_S32 PQ_MNG_EnableDMDemo(HI_BOOL bOnOff)
{
    return HI_SUCCESS;
}

/* no practical meaning*/
HI_S32 PQ_MNG_SetDbmDemoMode(DBM_DEMO_MODE_E enMode)
{
    return HI_SUCCESS;
}

/* no practical meaning*/
HI_S32 PQ_MNG_DBCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{
    return HI_SUCCESS;
}

/* no practical meaning*/
HI_S32 PQ_MNG_DBCalcConfigCfg(DB_CALC_INFO_S* pstDbCalcInfo)
{
    return HI_SUCCESS;
}

/* no practical meaning*/
HI_S32 PQ_MNG_DMCalcConfigCfg(DB_CALC_INFO_S* pstDmCalcInfo)
{
    return HI_SUCCESS;
}

#if 0
/* get DR Demo Enable state */
HI_S32 PQ_MNG_GetDRDemoEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);

    *pbOnOff = sg_bDR_Demo_EnFlag;
    return HI_SUCCESS;
}
#endif


