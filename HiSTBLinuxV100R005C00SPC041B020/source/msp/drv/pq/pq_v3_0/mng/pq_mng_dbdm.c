/******************************************************************************
*
* Copyright (C) 2012-2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************
  File Name     : pq_mng_dbdm.c
  Version       : Initial Draft
  Author        :
  Created       :
  Description   : De-blocking and De-Ringning

******************************************************************************/
#include "hi_math.h"
#include "drv_pq_table.h"
#include "pq_mng_dbdm.h"
#include "pq_mng_dbdm_alg.h"


static PQ_PARAM_S*     g_pstPqParam = HI_NULL;
static DBM_DEMO_MODE_E sg_enMode    = DBM_DEMO_ENABLE_R;
/********************************** De-blocking Function Start **********************************/

static HI_BOOL sg_bDB_EnFlag      = HI_TRUE;
static HI_BOOL sg_bUNF_DB_EnFlag  = HI_FALSE;
static HI_BOOL sg_bDB_Demo_EnFlag = HI_FALSE;
static HI_BOOL sg_bDBInitFlag     = HI_FALSE;
static HI_S32  sg_s32DBStrength   = 128;
static HI_U32  sg_u32PreGMotion[VPSS_HANDLE_NUM];/* 多实例的前一帧Global Motion */

static DB_TUN_MODE_E    sg_enDBTunMode = DB_TUN_NORMAL;
static DB_API_REG_S     sg_stDBValue;
static DB_DET_INFO_S    s_DbDetInf[VPSS_HANDLE_NUM];
static DB_DET_INFO_S    s_DbDetInf_Out[VPSS_HANDLE_NUM];
static DetStaticUsing_S s_DetUsing_info[VPSS_HANDLE_NUM];

static HI_U32 DB_DELTA_LUM_VER[16]            = {0, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* LUMA_VER p0-p15 */
static HI_U32 DB_DELTA_CHR_HOR[16]            = {8, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* CHR_HOR  p0-p15 */

static HI_U32 DB_DELTA_LUM_H_STRONG[16]       = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0}; /* LUMA_HOR p0-p15 */
static HI_U32 DB_DELTA_LUM_H_ULTRA_STRONG[16] = {4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 2, 0}; /* LUMA_HOR p0-p15 */

static HI_U32 DB_STR_FADE_LUT_1[12]    = {4, 4, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0};
static HI_U32 DB_STR_FADE_LUT_2[12]    = {4, 4, 4, 4, 3, 3, 3, 0, 0, 0, 0, 0};
static HI_U32 DB_STR_FADE_LUT_3[12]    = {4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 0};

static HI_U32 RAT_INDEX_LUT[16]  = {0, 1, 2, 3, 4, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8};
static DB_DynmcPara STRONG_PARAM = {12, 0, 0, 0, 24, 0, 4, 7,  8};
static DB_DynmcPara MID_PARAM    = {12, 0, 0, 0, 16, 0, 4, 7, 24};
static DB_DynmcPara WEAK_PARAM   = { 4, 0, 8, 8,  0, 4, 8, 7, 24};

HI_S32 PQ_MNG_DM_API_Init(HI_VOID);

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

/* DB Init parameter */
static HI_S32 DB_Init_Value(HI_VOID)
{
    /* set flag */
    sg_bDB_EnFlag      = HI_TRUE;
    sg_bUNF_DB_EnFlag  = HI_FALSE;
    sg_bDB_Demo_EnFlag = HI_FALSE;
    sg_enDBTunMode     = DB_TUN_NORMAL;

    return HI_SUCCESS;
}

/* Init DB, Vpss Handle ID = 0, Other Handle Init At Top Layer */
static HI_S32 PQ_MNG_InitDB(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    if (HI_TRUE == sg_bDBInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = DB_Init_Value();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DB parameter init error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_SD, OUTPUT_MODE_NO, PQ_BIN_MODULE_DB);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DB InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDBInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Deinit DB */
static HI_S32 PQ_MNG_DeinitDB(HI_VOID)
{
    if (HI_FALSE == sg_bDBInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDBInitFlag   = HI_FALSE;
    sg_s32DBStrength = 128;
    memset(sg_u32PreGMotion, 0, sizeof(sg_u32PreGMotion));

    return HI_SUCCESS;
}

/* DB DEMO Enable */
HI_S32 PQ_MNG_EnableDBDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; ++u32HandleNo)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableDBDemo(u32HandleNo, bOnOff);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable DB Demo!\n");
        return HI_FAILURE;
    }
    sg_bDB_Demo_EnFlag = bOnOff;

    return HI_SUCCESS;
}

/* Get DB Enable state */
HI_S32 PQ_MNG_GetDBEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDB_EnFlag;
    return HI_SUCCESS;
}

/* Notice: MV410 take effect; and it do effect in HiFone.

   DB Enable Control By Vpss;
   We don not control it by demoPos, demoMode and demoEn, because it will couple with  demo mode Unf interface;
   We only control it by control db filter enable.
   */
HI_S32 PQ_MNG_EnableDB(HI_BOOL bOnOff)
{
    HI_S32  s32Ret;
    HI_BOOL bDBEn;
    HI_U32  u32HandleNo = 0;

    s32Ret = PQ_HAL_GetDBEnReg(u32HandleNo, &bDBEn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Get DB_en Failure!\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == bDBEn)
    {
        sg_bDB_EnFlag     = bOnOff;
        sg_bUNF_DB_EnFlag = HI_TRUE; /* When 'Hi_TRUE == dbm_en' in reg; soft DB enable is useful */
    }
    else
    {
        sg_bDB_EnFlag     = HI_FALSE;
        sg_bUNF_DB_EnFlag = HI_FALSE; /* if 'Hi_FALSE == dbm_en' in reg; soft DB enable is useless;So We don not control it */
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_FindDBRegValue(HI_U32* pu32Value, HI_U32 u32RegAddr, HI_U8 u8RegLsb, HI_U8 u8RegMsb, HI_U8 u8InMode)
{
    HI_U32 u32Addr, u32Value;
    HI_U8  u8Lsb, u8Msb, u8SourceMode, u8OutputMode;
    HI_U32 i;

    PQ_CHECK_NULL_PTR(g_pstPqParam);

    for (i = 0; i < PHY_REG_MAX; i++)
    {
        u32Addr       = g_pstPqParam->stPQPhyReg[i].u32RegAddr;
        u32Value      = g_pstPqParam->stPQPhyReg[i].u32Value;
        u8Lsb         = g_pstPqParam->stPQPhyReg[i].u8Lsb;
        u8Msb         = g_pstPqParam->stPQPhyReg[i].u8Msb;
        u8SourceMode  = g_pstPqParam->stPQPhyReg[i].u8SourceMode;
        u8OutputMode  = g_pstPqParam->stPQPhyReg[i].u8OutputMode;

        if (u32Addr != u32RegAddr)
        {
            continue;
        }
        if (u8Lsb != u8RegLsb)
        {
            continue;
        }
        if (u8Msb != u8RegMsb)
        {
            continue;
        }

        *pu32Value = u32Value;

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 PQ_MNG_GetDBTstNum(HI_U32* pu32HyTstBlkNum, HI_U32* pu32HcTstBlkNum, HI_U32* pu32VyTstBlkNum)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret  = PQ_MNG_FindDBRegValue(pu32HyTstBlkNum, DB_HY_TST_BLK_NUM_ADDR, DB_HY_TST_BLK_NUM_LSB, DB_HY_TST_BLK_NUM_MSB, SOURCE_MODE_NO);
    s32Ret |= PQ_MNG_FindDBRegValue(pu32HcTstBlkNum, DB_HC_TST_BLK_NUM_ADDR, DB_HC_TST_BLK_NUM_LSB, DB_HC_TST_BLK_NUM_MSB, SOURCE_MODE_NO);
    s32Ret |= PQ_MNG_FindDBRegValue(pu32VyTstBlkNum, DB_VY_TST_BLK_NUM_ADDR, DB_VY_TST_BLK_NUM_LSB, DB_VY_TST_BLK_NUM_MSB, SOURCE_MODE_NO);

    return s32Ret;
}

static HI_S32 PQ_MNG_DB_API_Init(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{
    HI_U8 u8NumberWindow           = 6;
    HI_U8 u8HyMINBlkNum            = 10;
    HI_U8 u8HchrMINBlkNum          = 10;
    HI_U8 u8VyMINBlkNum            = 10;
    HI_U8 u8keyfrm_num             = 4;
    HI_U8 u8HyBordCountThreshold   = 15;
    HI_U8 u8HchrBordCountThreshold = 15;
    HI_U8 u8VyBordCountThreshold   = 15;
    HI_U8 u8frmsWindowTemp         = 4;
    HI_U8 u8frmsSatWindowTemp      = 8;

    s_DbDetInf[u32HandleNo].s32LumWidth  = u32Width;
    s_DbDetInf[u32HandleNo].s32LumHeight = u32Height;
    s_DbDetInf[u32HandleNo].s32ChrWidth  = u32Width / 2;
    s_DbDetInf[u32HandleNo].s32ChrHeight = u32Height / 2;

    PQ_HAL_GetDBDetEnReg(u32HandleNo, &(s_DbDetInf[u32HandleNo].det_size_en), &(s_DbDetInf[u32HandleNo].det_hy_en),
                         &(s_DbDetInf[u32HandleNo].det_hc_en), &(s_DbDetInf[u32HandleNo].det_vy_en));

    if (u32Height >= 2160 && u32Width >= 3840 && u32FRate >= 60)
    {
        s_DbDetInf[u32HandleNo].det_size_en = HI_FALSE;
    }

    u8NumberWindow           = PQ_MNG_GetSoftTable(PQ_SOFT_DB_NUM_WIN,        SOURCE_MODE_NO, OUTPUT_MODE_NO, 6);
    u8HyMINBlkNum            = PQ_MNG_GetSoftTable(PQ_SOFT_DB_HYMINBLKNUM,    SOURCE_MODE_NO, OUTPUT_MODE_NO, 10);
    u8HchrMINBlkNum          = PQ_MNG_GetSoftTable(PQ_SOFT_DB_HCMINBLKNUM,    SOURCE_MODE_NO, OUTPUT_MODE_NO, 10);
    u8VyMINBlkNum            = PQ_MNG_GetSoftTable(PQ_SOFT_DB_VYMINBLKNUM,    SOURCE_MODE_NO, OUTPUT_MODE_NO, 10);
    u8keyfrm_num             = PQ_MNG_GetSoftTable(PQ_SOFT_DB_KEYFRM_NUM,     SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
    u8HyBordCountThreshold   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_HYBORDCOUNTHRD, SOURCE_MODE_NO, OUTPUT_MODE_NO, 15);
    u8HchrBordCountThreshold = PQ_MNG_GetSoftTable(PQ_SOFT_DB_HCBORDCOUNTHRD, SOURCE_MODE_NO, OUTPUT_MODE_NO, 15);
    u8VyBordCountThreshold   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_VYBORDCOUNTHRD, SOURCE_MODE_NO, OUTPUT_MODE_NO, 15);
    u8frmsWindowTemp         = PQ_MNG_GetSoftTable(PQ_SOFT_DB_FRMWINTMP,      SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
    u8frmsSatWindowTemp      = PQ_MNG_GetSoftTable(PQ_SOFT_DB_FRMSATWINTMP,   SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);

    s_DetUsing_info[u32HandleNo].NumberWindow           = u8NumberWindow;
    s_DetUsing_info[u32HandleNo].SaturatWindow          = 12;
    s_DetUsing_info[u32HandleNo].HyMINBlkNum            = u8HyMINBlkNum;
    s_DetUsing_info[u32HandleNo].HchrMINBlkNum          = u8HchrMINBlkNum;
    s_DetUsing_info[u32HandleNo].VyMINBlkNum            = u8VyMINBlkNum;
    s_DetUsing_info[u32HandleNo].UsingSizeHy            = -1;
    s_DetUsing_info[u32HandleNo].UsingSizeHchr          = -1;
    s_DetUsing_info[u32HandleNo].UsingSizeVy            = -1;
    s_DetUsing_info[u32HandleNo].UsingHyCounterBorder   = 0;
    s_DetUsing_info[u32HandleNo].UsingVyCounterBorder   = 0;
    s_DetUsing_info[u32HandleNo].UsingHchrCounterBorder = 0;

    memset(s_DetUsing_info[u32HandleNo].UsingBorderIndex, 0, sizeof( HI_U8 )* MAXLINESIZE);
    s_DetUsing_info[u32HandleNo].ptrUsingHyBorderIndex   = s_DetUsing_info[u32HandleNo].UsingBorderIndex;
    s_DetUsing_info[u32HandleNo].ptrUsingHchrBorderIndex = s_DetUsing_info[u32HandleNo].ptrUsingHyBorderIndex   + 1920;
    s_DetUsing_info[u32HandleNo].ptrUsingVyBorderIndex   = s_DetUsing_info[u32HandleNo].ptrUsingHchrBorderIndex + 960;
    s_DetUsing_info[u32HandleNo].frms_NumsHy             = 0;
    s_DetUsing_info[u32HandleNo].frms_NumsHchr           = 0;
    s_DetUsing_info[u32HandleNo].frms_NumsVy             = 0;
    s_DetUsing_info[u32HandleNo].keyfrm_num              = u8keyfrm_num;
    s_DetUsing_info[u32HandleNo].HyBordCountThreshold    = u8HyBordCountThreshold;
    s_DetUsing_info[u32HandleNo].HchrBordCountThreshold  = u8HchrBordCountThreshold;
    s_DetUsing_info[u32HandleNo].VyBordCountThreshold    = u8VyBordCountThreshold;

    memset(s_DetUsing_info[u32HandleNo].BorderCnt, 0, sizeof(HI_S8)*MAXLINESIZE);
    s_DetUsing_info[u32HandleNo].ptrHyCnt          = s_DetUsing_info[u32HandleNo].BorderCnt;
    s_DetUsing_info[u32HandleNo].ptrHchrCnt        = s_DetUsing_info[u32HandleNo].ptrHyCnt   + 1920;
    s_DetUsing_info[u32HandleNo].ptrVyCnt          = s_DetUsing_info[u32HandleNo].ptrHchrCnt + 960;
    s_DetUsing_info[u32HandleNo].frmsCntTempHy     = 0;
    s_DetUsing_info[u32HandleNo].frmsCntTempHchr   = 0;
    s_DetUsing_info[u32HandleNo].frmsCntTempVy     = 0;
    s_DetUsing_info[u32HandleNo].frmsWindowTemp    = u8frmsWindowTemp;
    s_DetUsing_info[u32HandleNo].frmsSatWindowTemp = u8frmsSatWindowTemp;

    pqprint(PQ_PRN_DB_READ, "NumWin:%d,BlkNum(Hy;Hc;Vy)--(%d,%d,%d),KeyFrmNum:%d,BordThd(Hy;Hc;Vy)--(%d,%d,%d),FrmWin:%d,FrmSatWin:%d\n",
            s_DetUsing_info[u32HandleNo].NumberWindow,
            s_DetUsing_info[u32HandleNo].HyMINBlkNum,
            s_DetUsing_info[u32HandleNo].HchrMINBlkNum,
            s_DetUsing_info[u32HandleNo].VyMINBlkNum,
            s_DetUsing_info[u32HandleNo].keyfrm_num,
            s_DetUsing_info[u32HandleNo].HyBordCountThreshold,
            s_DetUsing_info[u32HandleNo].HchrBordCountThreshold,
            s_DetUsing_info[u32HandleNo].VyBordCountThreshold,
            s_DetUsing_info[u32HandleNo].frmsWindowTemp,
            s_DetUsing_info[u32HandleNo].frmsSatWindowTemp);

    return HI_SUCCESS;
}

#define DB_SIZE_CLIP(x) ((x>0x3F)?(-1):(x))
static HI_VOID GetDbSttInfo(DB_DET_INFO_S* pDBDetInput, DB_CALC_INFO_S* pstDbCalcInfo)
{
    int i = 0;

    pDBDetInput->hy_counter = pstDbCalcInfo->pstVPSSWbcReg->DB_HY_SIZE.bits.hy_counter;
    pDBDetInput->hy_size    = DB_SIZE_CLIP( pstDbCalcInfo->pstVPSSWbcReg->DB_HY_SIZE.bits.hy_size );
    pDBDetInput->vy_counter = pstDbCalcInfo->pstVPSSWbcReg->DB_VY_SIZE.bits.vy_counter;
    pDBDetInput->vy_size    = DB_SIZE_CLIP( pstDbCalcInfo->pstVPSSWbcReg->DB_VY_SIZE.bits.vy_size );
    pDBDetInput->hc_counter = pstDbCalcInfo->pstVPSSWbcReg->DB_HC_SIZE.bits.hc_counter;
    pDBDetInput->hc_size    = DB_SIZE_CLIP( pstDbCalcInfo->pstVPSSWbcReg->DB_HC_SIZE.bits.hc_size );

    PQ_HAL_GetDBDetEnReg(pstDbCalcInfo->u32HandleNo, &(pDBDetInput->det_size_en), &(pDBDetInput->det_hy_en),
                         &(pDBDetInput->det_hc_en), &(pDBDetInput->det_vy_en));

    for (i = 0; i < 128; i++)
    {
        pDBDetInput->db_border[i] = pstDbCalcInfo->pstVPSSWbcReg->DB_BORDER[i];
    }

    for (i = 0; i < 1920; i++)
    {
        pDBDetInput->db_hy_counter[i] = pstDbCalcInfo->pstVPSSWbcReg->DB_HY_COUNTER[i];
    }

    pqprint(PQ_PRN_DB_READ, "[%s]: hy;vy;hc(coter,size)--(%d,%d);(%d,%d);(%d,%d)\n",
            __FUNCTION__,
            pDBDetInput->hy_counter, pDBDetInput->hy_size,
            pDBDetInput->vy_counter, pDBDetInput->vy_size,
            pDBDetInput->hc_counter, pDBDetInput->hc_size);

    return;
}


static HI_S32 DB_Blend(HI_S32 F, HI_S32 B, HI_S32 An, HI_S32 Ad)
{
    HI_S32 result;

    result = (F * An + B * (Ad - An) + Ad / 2) / Ad;
    return result;
}

/* db_lum_hor_en */ /* DB滤波水平亮度使能 */
static HI_S32 DB_UpdateLumHorPara(DB_API_REG_S* pInput, const DB_DET_INFO_S* p_Inf, HI_U32 u32HandleNo)
{
    HI_S32 DetRatio = 0, StrIdx = 0;

    DB_DynmcPara PARAM_STR;
    DB_DynmcPara PARAM_MID;
    DB_DynmcPara PARAM_WEK;

    HI_U32 DB_DELTA_LUM_STRONG_H[16]     = {0};
    HI_U32 DB_DELTA_LUM_ULT_STRONG_H[16] = {0};

    HI_U32 DB_FADE_STR_LUT_1[12]         = {0};
    HI_U32 DB_FADE_STR_LUT_2[12]         = {0};
    HI_U32 DB_FADE_STR_LUT_3[12]         = {0};

    /* add for DBM debug Mode */
    if ( DB_TUN_DEBUG == sg_enDBTunMode )
    {
        HI_U32 u32LumStrNum_H_1,  u32LumStrNum_H_2,  u32LumUltStrNum_H_1, u32LumUltStrNum_H_2;
        HI_U32 u32FadeLutNum_1_1, u32FadeLutNum_1_2, u32FadeLutNum_2_1,   u32FadeLutNum_2_2, u32FadeLutNum_3_1, u32FadeLutNum_3_2;

        HI_U32 DB_DELTA_LUM_STRONG_H_1[8]  = {4, 8, 8, 8, 8, 8, 8, 8};
        HI_U32 DB_DELTA_LUM_STRONG_H_2[8]  = {8, 8, 8, 6, 4, 2, 0, 0};
        HI_U32 DB_DELTA_LUM_ULT_STR_H_1[8] = {4, 8, 8, 8, 8, 8, 8, 8};
        HI_U32 DB_DELTA_LUM_ULT_STR_H_2[8] = {8, 8, 8, 8, 8, 6, 2, 0};

        HI_U32 DB_FADE_STR_LUT_1_1[10]     = {4, 4, 3, 3, 0, 0, 0, 0, 0, 0};
        HI_U32 DB_FADE_STR_LUT_1_2[2]      = {0, 0};
        HI_U32 DB_FADE_STR_LUT_2_1[10]     = {4, 4, 4, 4, 3, 3, 3, 0, 0, 0};
        HI_U32 DB_FADE_STR_LUT_2_2[2]      = {0, 0};
        HI_U32 DB_FADE_STR_LUT_3_1[10]     = {4, 4, 4, 4, 4, 4, 4, 3, 3, 3};
        HI_U32 DB_FADE_STR_LUT_3_2[2]      = {3, 0};

        u32LumStrNum_H_1    = sizeof(DB_DELTA_LUM_STRONG_H_1) / sizeof(HI_U32);
        u32LumStrNum_H_2    = sizeof(DB_DELTA_LUM_STRONG_H_2) / sizeof(HI_U32);
        u32LumUltStrNum_H_1 = sizeof(DB_DELTA_LUM_ULT_STR_H_1) / sizeof(HI_U32);
        u32LumUltStrNum_H_2 = sizeof(DB_DELTA_LUM_ULT_STR_H_2) / sizeof(HI_U32);

        u32FadeLutNum_1_1   = sizeof(DB_FADE_STR_LUT_1_1) / sizeof(HI_U32);
        u32FadeLutNum_1_2   = sizeof(DB_FADE_STR_LUT_1_2) / sizeof(HI_U32);
        u32FadeLutNum_2_1   = sizeof(DB_FADE_STR_LUT_2_1) / sizeof(HI_U32);
        u32FadeLutNum_2_2   = sizeof(DB_FADE_STR_LUT_2_2) / sizeof(HI_U32);
        u32FadeLutNum_3_1   = sizeof(DB_FADE_STR_LUT_3_1) / sizeof(HI_U32);
        u32FadeLutNum_3_2   = sizeof(DB_FADE_STR_LUT_3_2) / sizeof(HI_U32);

        memset(&PARAM_STR, 0, sizeof(DB_DynmcPara));
        memset(&PARAM_MID, 0, sizeof(DB_DynmcPara));
        memset(&PARAM_WEK, 0, sizeof(DB_DynmcPara));

        memset(DB_DELTA_LUM_STRONG_H,     0, 16 * sizeof(HI_U32));
        memset(DB_DELTA_LUM_ULT_STRONG_H, 0, 16 * sizeof(HI_U32));

        memset(DB_FADE_STR_LUT_1, 0, 12 * sizeof(HI_U32));
        memset(DB_FADE_STR_LUT_2, 0, 12 * sizeof(HI_U32));
        memset(DB_FADE_STR_LUT_3, 0, 12 * sizeof(HI_U32));

        PARAM_STR.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 12);
        PARAM_STR.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);
        PARAM_STR.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_STR.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_STR.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);

        PARAM_MID.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 12);
        PARAM_MID.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 16);
        PARAM_MID.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_MID.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_MID.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);

        PARAM_WEK.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_WEK.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_WEK.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_WEK.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_WEK.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_WEK.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);

        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HL_DELTA_STRONG_1,  DB_DELTA_LUM_STRONG_H_1,  u32LumStrNum_H_1,    SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_STRONG_H_1);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HL_DELTA_STRONG_2,  DB_DELTA_LUM_STRONG_H_2,  u32LumStrNum_H_2,    SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_STRONG_H_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HL_DELTA_ULT_STR_2, DB_DELTA_LUM_ULT_STR_H_2, u32LumUltStrNum_H_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_ULT_STR_H_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HL_DELTA_ULT_STR_1, DB_DELTA_LUM_ULT_STR_H_1, u32LumUltStrNum_H_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_ULT_STR_H_1);

        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_1_2, DB_FADE_STR_LUT_1_2, u32FadeLutNum_1_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_1_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_1_1, DB_FADE_STR_LUT_1_1, u32FadeLutNum_1_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_1_1);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_2_2, DB_FADE_STR_LUT_2_2, u32FadeLutNum_2_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_2_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_2_1, DB_FADE_STR_LUT_2_1, u32FadeLutNum_2_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_2_1);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_3_2, DB_FADE_STR_LUT_3_2, u32FadeLutNum_3_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_3_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_3_1, DB_FADE_STR_LUT_3_1, u32FadeLutNum_3_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_3_1);

        memcpy(&DB_DELTA_LUM_STRONG_H[0],     DB_DELTA_LUM_STRONG_H_1,  u32LumStrNum_H_1 * sizeof(HI_U32));
        memcpy(&DB_DELTA_LUM_STRONG_H[8],     DB_DELTA_LUM_STRONG_H_2,  u32LumStrNum_H_2 * sizeof(HI_U32));
        memcpy(&DB_DELTA_LUM_ULT_STRONG_H[0], DB_DELTA_LUM_ULT_STR_H_1, u32LumUltStrNum_H_1 * sizeof(HI_U32));
        memcpy(&DB_DELTA_LUM_ULT_STRONG_H[8], DB_DELTA_LUM_ULT_STR_H_2, u32LumUltStrNum_H_2 * sizeof(HI_U32));

        memcpy(&DB_FADE_STR_LUT_1[0],  DB_FADE_STR_LUT_1_1, u32FadeLutNum_1_1 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_1[10], DB_FADE_STR_LUT_1_2, u32FadeLutNum_1_2 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_2[0],  DB_FADE_STR_LUT_2_1, u32FadeLutNum_2_1 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_2[10], DB_FADE_STR_LUT_2_2, u32FadeLutNum_2_2 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_3[0],  DB_FADE_STR_LUT_3_1, u32FadeLutNum_3_1 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_3[10], DB_FADE_STR_LUT_3_2, u32FadeLutNum_3_2 * sizeof(HI_U32));

    }
    else
    {
        memcpy(&PARAM_STR, &STRONG_PARAM, sizeof(DB_DynmcPara));
        memcpy(&PARAM_MID, &MID_PARAM,    sizeof(DB_DynmcPara));
        memcpy(&PARAM_WEK, &WEAK_PARAM,   sizeof(DB_DynmcPara));

        memcpy(&DB_DELTA_LUM_STRONG_H,     DB_DELTA_LUM_H_STRONG,       16 * sizeof(HI_U32));
        memcpy(&DB_DELTA_LUM_ULT_STRONG_H, DB_DELTA_LUM_H_ULTRA_STRONG, 16 * sizeof(HI_U32));

        memcpy(&DB_FADE_STR_LUT_1, DB_STR_FADE_LUT_1, 12 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_2, DB_STR_FADE_LUT_2, 12 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_3, DB_STR_FADE_LUT_3, 12 * sizeof(HI_U32));
    }

    pInput->lum_h_blk_size = p_Inf->hy_size;

    if ((pInput->lum_h_blk_size > 4) && (p_Inf->s32LumHBlkNum > 0))
    {
        pInput->db_lum_hor_en       = HI_TRUE;
        pInput->lum_hor_scale_ratio = (pInput->lum_h_blk_size + 7) / 8;
        pInput->lum_hor_filter_sel  = pInput->lum_hor_scale_ratio;
        //pInput->lum_hor_scale_ratio = 1;
        //pInput->lum_hor_filter_sel  = 1;

        /* 根据scale_ratio 选择滤波系数载入Lookup Table */
        switch (pInput->lum_hor_scale_ratio)
        {
            case 0:
            case 1:
                pInput->lum_hor_txt_win_size = 0;//4;
                PQ_HAL_SetReg_Lum_Hor_Fade_Lut(u32HandleNo,  DB_FADE_STR_LUT_1);
                PQ_HAL_SetReg_Lum_Hor_Delta_Lut(u32HandleNo, DB_DELTA_LUM_STRONG_H);
                break;

            case 2:
                pInput->lum_hor_txt_win_size = 1;//8;
                PQ_HAL_SetReg_Lum_Hor_Fade_Lut(u32HandleNo,  DB_FADE_STR_LUT_2);
                PQ_HAL_SetReg_Lum_Hor_Delta_Lut(u32HandleNo, DB_DELTA_LUM_ULT_STRONG_H);
                break;

            case 3:
            default:
                pInput->lum_hor_txt_win_size = 2;//12;
                PQ_HAL_SetReg_Lum_Hor_Fade_Lut(u32HandleNo,  DB_FADE_STR_LUT_3);
                PQ_HAL_SetReg_Lum_Hor_Delta_Lut(u32HandleNo, DB_DELTA_LUM_ULT_STRONG_H);
                break;
        }

        pInput->max_lum_hor_db_dist = DBM_MIN(pInput->lum_h_blk_size / 2, 12); /* 最大允许12 */

        /* adaptive strength */
        DetRatio = 16 * p_Inf->hy_counter / p_Inf->s32LumHBlkNum;
        DetRatio = DBM_CLIP3(0, 15, DetRatio);
        StrIdx   = RAT_INDEX_LUT[DetRatio];

        if (StrIdx < 6)
        {
            /* weak bias */
            pInput->lum_hor_adj_gain      = DB_Blend(PARAM_MID.PixValAdjGain, PARAM_WEK.PixValAdjGain, StrIdx, 6);
            pInput->lum_hor_hf_diff_core  = DB_Blend(PARAM_MID.HFDiffCore,    PARAM_WEK.HFDiffCore,    StrIdx, 6);
            pInput->lum_hor_hf_diff_gain1 = DB_Blend(PARAM_MID.HFDiffGain1,   PARAM_WEK.HFDiffGain1,   StrIdx, 6);
            pInput->lum_hor_hf_diff_gain2 = DB_Blend(PARAM_MID.HFDiffGain2,   PARAM_WEK.HFDiffGain2,   StrIdx, 6);
            pInput->lum_hor_hf_var_core   = DB_Blend(PARAM_MID.HFVarCore,     PARAM_WEK.HFVarCore,     StrIdx, 6);
            pInput->lum_hor_hf_var_gain1  = DB_Blend(PARAM_MID.HFVarGain1,    PARAM_WEK.HFVarGain1,    StrIdx, 6);
            pInput->lum_hor_hf_var_gain2  = DB_Blend(PARAM_MID.HFVarGain2,    PARAM_WEK.HFVarGain2,    StrIdx, 6);
            pInput->lum_hor_bord_adj_gain = DB_Blend(PARAM_MID.BordAdjGain,   PARAM_WEK.BordAdjGain,   StrIdx, 6);
            pInput->grad_sub_ratio        = DB_Blend(PARAM_MID.GradSubRatio,  PARAM_WEK.GradSubRatio,  StrIdx, 6);
        }
        else
        {
            /* strong bias */
            pInput->lum_hor_adj_gain      = DB_Blend(PARAM_STR.PixValAdjGain, PARAM_MID.PixValAdjGain, StrIdx - 6, 2);
            pInput->lum_hor_hf_diff_core  = DB_Blend(PARAM_STR.HFDiffCore,    PARAM_MID.HFDiffCore,    StrIdx - 6, 2);
            pInput->lum_hor_hf_diff_gain1 = DB_Blend(PARAM_STR.HFDiffGain1,   PARAM_MID.HFDiffGain1,   StrIdx - 6, 2);
            pInput->lum_hor_hf_diff_gain2 = DB_Blend(PARAM_STR.HFDiffGain2,   PARAM_MID.HFDiffGain2,   StrIdx - 6, 2);
            pInput->lum_hor_hf_var_core   = DB_Blend(PARAM_STR.HFVarCore,     PARAM_MID.HFVarCore,     StrIdx - 6, 2);
            pInput->lum_hor_hf_var_gain1  = DB_Blend(PARAM_STR.HFVarGain1,    PARAM_MID.HFVarGain1,    StrIdx - 6, 2);
            pInput->lum_hor_hf_var_gain2  = DB_Blend(PARAM_STR.HFVarGain2,    PARAM_MID.HFVarGain2,    StrIdx - 6, 2);
            pInput->lum_hor_bord_adj_gain = DB_Blend(PARAM_STR.BordAdjGain,   PARAM_MID.BordAdjGain,   StrIdx - 6, 2);
            pInput->grad_sub_ratio        = DB_Blend(PARAM_STR.GradSubRatio,  PARAM_MID.GradSubRatio,  StrIdx - 6, 2);
        }
    }
    else
    {
        StrIdx = 0;
        pInput->db_lum_hor_en = HI_FALSE;
    }

    return StrIdx;
}

/* db_lum_ver_en */ /* DB滤波垂直亮度使能 */
static HI_S32 DB_UpdateLumVerPara(DB_API_REG_S* pInput, const DB_DET_INFO_S* p_Inf, HI_S32 S32RefStr, HI_U32 u32HandleNo)
{
    HI_S32 DetRatio = 0, StrIdx = 0;
    DB_DynmcPara PARAM_STR;
    DB_DynmcPara PARAM_MID;
    DB_DynmcPara PARAM_WEK;

    HI_U32 DB_DELTA_LUM_V[16] = {0};

    /* add for DBM debug mode */
    if ( DB_TUN_DEBUG == sg_enDBTunMode )
    {
        HI_U32 u32LumStrNum_V_1, u32LumStrNum_V_2;

        HI_U32 DB_DELTA_LUM_V_1[8] = {0, 6, 4, 2, 0, 0, 0, 0};
        HI_U32 DB_DELTA_LUM_V_2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        u32LumStrNum_V_1 = sizeof(DB_DELTA_LUM_V_1) / sizeof(HI_U32);
        u32LumStrNum_V_2 = sizeof(DB_DELTA_LUM_V_2) / sizeof(HI_U32);

        memset(&PARAM_STR, 0, sizeof(DB_DynmcPara));
        memset(&PARAM_MID, 0, sizeof(DB_DynmcPara));
        memset(&PARAM_WEK, 0, sizeof(DB_DynmcPara));

        memset(DB_DELTA_LUM_V, 0, 16 * sizeof(HI_U32));

        PARAM_STR.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 12);
        PARAM_STR.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);
        PARAM_STR.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_STR.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_STR.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_STR.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_STR_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);

        PARAM_MID.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 12);
        PARAM_MID.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 16);
        PARAM_MID.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_MID.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_MID.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_MID.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_MID_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);

        PARAM_WEK.PixValAdjGain = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_1, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_WEK.HFDiffCore    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_2, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_WEK.HFDiffGain1   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_3, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.HFDiffGain2   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_4, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.HFVarCore     = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_5, SOURCE_MODE_NO, OUTPUT_MODE_NO, 0);
        PARAM_WEK.HFVarGain1    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_6, SOURCE_MODE_NO, OUTPUT_MODE_NO, 4);
        PARAM_WEK.HFVarGain2    = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_7, SOURCE_MODE_NO, OUTPUT_MODE_NO, 8);
        PARAM_WEK.BordAdjGain   = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_8, SOURCE_MODE_NO, OUTPUT_MODE_NO, 7);
        PARAM_WEK.GradSubRatio  = PQ_MNG_GetSoftTable(PQ_SOFT_DB_DYN_PARA_WEK_9, SOURCE_MODE_NO, OUTPUT_MODE_NO, 24);

        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_LV_DELTA_2, DB_DELTA_LUM_V_2, u32LumStrNum_V_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_V_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_LV_DELTA_1, DB_DELTA_LUM_V_1, u32LumStrNum_V_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_LUM_V_1);

        memcpy(&DB_DELTA_LUM_V[0], DB_DELTA_LUM_V_1, u32LumStrNum_V_1 * sizeof(HI_U32));
        memcpy(&DB_DELTA_LUM_V[8], DB_DELTA_LUM_V_2, u32LumStrNum_V_2 * sizeof(HI_U32));

    }
    else
    {
        memcpy(&PARAM_STR, &STRONG_PARAM, sizeof(DB_DynmcPara));
        memcpy(&PARAM_MID, &MID_PARAM,    sizeof(DB_DynmcPara));
        memcpy(&PARAM_WEK, &WEAK_PARAM,   sizeof(DB_DynmcPara));

        memcpy(&DB_DELTA_LUM_V, DB_DELTA_LUM_VER, 16 * sizeof(HI_U32));
    }

    pInput->lum_v_blk_size = p_Inf->vy_size;

    if (pInput->lum_v_blk_size > 4 && p_Inf->s32LumVBlkNum > 0)
    {
        pInput->db_lum_ver_en       = HI_TRUE;
        pInput->lum_ver_scale_ratio = (pInput->lum_v_blk_size + 7) / 8;

        /* adaptive strength */
        DetRatio = 16 * p_Inf->vy_counter / p_Inf->s32LumVBlkNum;
        DetRatio = DBM_CLIP3(0, 15, DetRatio);

        /* refer to lum hor */
        StrIdx = DBM_CLIP3(0, 8, (RAT_INDEX_LUT[DetRatio] * S32RefStr) / 8);

        pInput->lum_ver_adj_gain      = DB_Blend(PARAM_STR.PixValAdjGain, PARAM_WEK.PixValAdjGain, StrIdx, 8);
        pInput->lum_ver_hf_var_core   = DB_Blend(PARAM_STR.HFVarCore,     PARAM_WEK.HFVarCore,     StrIdx, 8);
        pInput->lum_ver_hf_var_gain1  = DB_Blend(PARAM_STR.HFVarGain1,    PARAM_WEK.HFVarGain1,    StrIdx, 8);
        pInput->lum_ver_hf_var_gain2  = DB_Blend(PARAM_STR.HFVarGain2,    PARAM_WEK.HFVarGain2,    StrIdx, 8);
        pInput->lum_ver_bord_adj_gain = DB_Blend(PARAM_STR.BordAdjGain,   PARAM_WEK.BordAdjGain,   StrIdx, 8);
    }
    else
    {
        StrIdx = 0;
        pInput->db_lum_ver_en = HI_FALSE;
    }

    return StrIdx;
}

/* db_chr_hor_en */ /* DB滤波水平色度使能 */
static HI_S32 DB_UpdateChrHorPara(DB_API_REG_S* pInput, const DB_DET_INFO_S* p_Inf, HI_S32 S32RefStr, HI_U32 u32HandleNo)
{
    HI_S32 DetRatio = 0, StrIdx = 0;
    HI_U32 DB_DELTA_CHR_H[16]    = {0};

    HI_U32 DB_FADE_STR_LUT_1[12] = {0};
    HI_U32 DB_FADE_STR_LUT_2[12] = {0};
    HI_U32 DB_FADE_STR_LUT_3[12] = {0};

    /* add for DBM debug mode */
    if ( DB_TUN_DEBUG == sg_enDBTunMode )
    {
        HI_U32 u32ChrStrNum_H_1, u32ChrStrNum_H_2;
        HI_U32 u32FadeLutNum_1_1, u32FadeLutNum_1_2, u32FadeLutNum_2_1,   u32FadeLutNum_2_2, u32FadeLutNum_3_1, u32FadeLutNum_3_2;

        HI_U32 DB_DELTA_CHR_H_1[8]     = {8, 8, 8, 4, 0, 0, 0, 0};
        HI_U32 DB_DELTA_CHR_H_2[8]     = {0, 0, 0, 0, 0, 0, 0, 0};

        HI_U32 DB_FADE_STR_LUT_1_1[10] = {4, 4, 3, 3, 0, 0, 0, 0, 0, 0};
        HI_U32 DB_FADE_STR_LUT_1_2[2]  = {0, 0};
        HI_U32 DB_FADE_STR_LUT_2_1[10] = {4, 4, 4, 4, 3, 3, 3, 0, 0, 0};
        HI_U32 DB_FADE_STR_LUT_2_2[2]  = {0, 0};
        HI_U32 DB_FADE_STR_LUT_3_1[10] = {4, 4, 4, 4, 4, 4, 4, 3, 3, 3};
        HI_U32 DB_FADE_STR_LUT_3_2[2]  = {3, 0};

        u32ChrStrNum_H_1  = sizeof(DB_DELTA_CHR_H_1) / sizeof(HI_U32);
        u32ChrStrNum_H_2  = sizeof(DB_DELTA_CHR_H_2) / sizeof(HI_U32);

        u32FadeLutNum_1_1 = sizeof(DB_FADE_STR_LUT_1_1) / sizeof(HI_U32);
        u32FadeLutNum_1_2 = sizeof(DB_FADE_STR_LUT_1_2) / sizeof(HI_U32);
        u32FadeLutNum_2_1 = sizeof(DB_FADE_STR_LUT_2_1) / sizeof(HI_U32);
        u32FadeLutNum_2_2 = sizeof(DB_FADE_STR_LUT_2_2) / sizeof(HI_U32);
        u32FadeLutNum_3_1 = sizeof(DB_FADE_STR_LUT_3_1) / sizeof(HI_U32);
        u32FadeLutNum_3_2 = sizeof(DB_FADE_STR_LUT_3_2) / sizeof(HI_U32);

        memset(DB_DELTA_CHR_H, 0, 16 * sizeof(HI_U32));

        memset(DB_FADE_STR_LUT_1, 0, 12 * sizeof(HI_U32));
        memset(DB_FADE_STR_LUT_2, 0, 12 * sizeof(HI_U32));
        memset(DB_FADE_STR_LUT_3, 0, 12 * sizeof(HI_U32));

        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HC_DELTA_2, DB_DELTA_CHR_H_2, u32ChrStrNum_H_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_CHR_H_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_HC_DELTA_1, DB_DELTA_CHR_H_1, u32ChrStrNum_H_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_DELTA_CHR_H_1);

        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_1_2, DB_FADE_STR_LUT_1_2, u32FadeLutNum_1_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_1_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_1_1, DB_FADE_STR_LUT_1_1, u32FadeLutNum_1_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_1_1);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_2_2, DB_FADE_STR_LUT_2_2, u32FadeLutNum_2_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_2_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_2_1, DB_FADE_STR_LUT_2_1, u32FadeLutNum_2_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_2_1);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_3_2, DB_FADE_STR_LUT_3_2, u32FadeLutNum_3_2, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_3_2);
        PQ_MNG_GetArraySoftTable(PQ_SOFT_DB_STR_FADE_3_1, DB_FADE_STR_LUT_3_1, u32FadeLutNum_3_1, SOURCE_MODE_NO, SOURCE_MODE_NO, DB_FADE_STR_LUT_3_1);

        memcpy(&DB_DELTA_CHR_H[0], DB_DELTA_CHR_H_1, u32ChrStrNum_H_1 * sizeof(HI_U32));
        memcpy(&DB_DELTA_CHR_H[8], DB_DELTA_CHR_H_2, u32ChrStrNum_H_2 * sizeof(HI_U32));
    }
    else
    {
        memcpy(&DB_DELTA_CHR_H,    DB_DELTA_CHR_HOR,  16 * sizeof(HI_U32));

        memcpy(&DB_FADE_STR_LUT_1, DB_STR_FADE_LUT_1, 12 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_2, DB_STR_FADE_LUT_2, 12 * sizeof(HI_U32));
        memcpy(&DB_FADE_STR_LUT_3, DB_STR_FADE_LUT_3, 12 * sizeof(HI_U32));
    }

    pInput->chr_h_blk_size = p_Inf->hc_size;

    if (pInput->chr_h_blk_size > 4 && p_Inf->s32ChrHBlkNum > 0)
    {
        pInput->db_chr_hor_en       = HI_TRUE;
        pInput->chr_hor_scale_ratio = (pInput->chr_h_blk_size + 7) / 8;
        pInput->chr_hor_filter_sel  = pInput->chr_hor_scale_ratio;
        pInput->max_chr_hor_db_dist = pInput->chr_h_blk_size / 2;

        /* 根据scale_ratio 选择滤波系数载入Lookup Table */
        switch (pInput->chr_hor_scale_ratio)
        {
            case 0:
                PQ_HAL_SetReg_Chr_Hor_Fade_Lut(u32HandleNo, DB_FADE_STR_LUT_1);
                break;

            case 1:
                PQ_HAL_SetReg_Chr_Hor_Fade_Lut(u32HandleNo, DB_FADE_STR_LUT_1);
                break;

            case 2:
                PQ_HAL_SetReg_Chr_Hor_Fade_Lut(u32HandleNo, DB_FADE_STR_LUT_2);
                break;

            case 3:
            default:
                PQ_HAL_SetReg_Chr_Hor_Fade_Lut(u32HandleNo, DB_FADE_STR_LUT_3);
                break;
        }

        pInput->max_chr_hor_db_dist = DBM_MIN(pInput->chr_h_blk_size / 2, 12);

        DetRatio = 16 * p_Inf->hc_counter / p_Inf->s32ChrHBlkNum;
        DetRatio = DBM_CLIP3(0, 15, DetRatio);

        /* refer to lum hor */
        StrIdx = DBM_CLIP3(0, 8, (RAT_INDEX_LUT[DetRatio] * S32RefStr) / 8);
    }
    else
    {
        StrIdx = 0;
        pInput->db_chr_hor_en = HI_FALSE;
    }

    return StrIdx;
}

static HI_S32 DB_CfgFilter(DB_API_REG_S* pInput, DB_DET_INFO_S* p_Inf, HI_U32 u32HandleNo)
{
    //HI_U32 u32FilterStrength;
    HI_S32 StrIdxLumH = 0, StrIdxLumV = 0, StrIdxChr = 0;

    pInput->db_lum_en              = HI_TRUE;
    pInput->db_cb_en               = HI_TRUE;
    pInput->db_cr_en               = HI_TRUE;
    pInput->db_chr_hor_mid_grey_en = HI_TRUE;
    pInput->db_chr_ver_en          = HI_FALSE;
    pInput->chr_v_blk_size         = 0;
    /* config direction detect para */
    pInput->grad_sub_ratio         = 24;

    if (p_Inf->s32LumWidth > 1920 || p_Inf->s32LumHeight > 1080)
    {
        pInput->db_lum_ver_en = 0;
        pInput->db_cb_en      = 0;
        pInput->db_cr_en      = 0;
        pInput->db_chr_hor_en = 0;
    }

    /* if size changed, the paras should been updated */
    StrIdxLumH = DB_UpdateLumHorPara(pInput, p_Inf, u32HandleNo);
    StrIdxLumV = DB_UpdateLumVerPara(pInput, p_Inf, StrIdxLumH, u32HandleNo);
    StrIdxChr  = DB_UpdateChrHorPara(pInput, p_Inf, StrIdxLumH, u32HandleNo);

    return HI_SUCCESS;
}

static HI_S32 DB_Cfg(DB_API_REG_S* pInput, DB_DET_INFO_S* p_Inf, DetStaticUsing_S* pDetUsing_info, HI_U32 u32HandleNo)
{
    HI_U32 u32HyTstBlkNum = 90;
    HI_U32 u32HcTstBlkNum = 45;/* 420--45;422--90 */
    HI_U32 u32VyTstBlkNum = 60;

    PQ_MNG_GetDBTstNum(&u32HyTstBlkNum, &u32HcTstBlkNum, &u32VyTstBlkNum);
    PQ_MNG_ALG_DBCfgDetector(pInput, p_Inf, pDetUsing_info, u32HyTstBlkNum, u32HcTstBlkNum);

    DB_CfgFilter(pInput, p_Inf, u32HandleNo);

    /* When dbm_en Enable Open; DB soft Enable control is useful */
    if (HI_TRUE == sg_bUNF_DB_EnFlag)
    {
        pInput->db_lum_ver_en          = (pInput->db_lum_ver_en) & sg_bDB_EnFlag;
        pInput->db_lum_hor_en          = (pInput->db_lum_hor_en) & sg_bDB_EnFlag;
        pInput->db_chr_ver_en          = (pInput->db_chr_ver_en) & sg_bDB_EnFlag;
        pInput->db_chr_hor_en          = (pInput->db_chr_hor_en) & sg_bDB_EnFlag;
        pInput->db_chr_hor_mid_grey_en = (pInput->db_chr_hor_mid_grey_en) & sg_bDB_EnFlag;
    }
    else
    {}

    return HI_SUCCESS;
}

/* DB and DM API Init; refresh when timing change */
HI_S32 PQ_MNG_DBCfgRefresh(HI_U32 u32HandleNo, HI_U32 u32Height, HI_U32 u32Width, HI_U32 u32FRate)
{
    /* DB */
    PQ_MNG_DB_API_Init(u32HandleNo, u32Height, u32Width, u32FRate);

    /* DM */
    PQ_MNG_DM_API_Init();

    return HI_SUCCESS;
}

/* refresh every frame */
HI_S32 PQ_MNG_DBCalcConfigCfg(DB_CALC_INFO_S* pstDbCalcInfo)
{
    /* read reg value from VPSS WBC stt, tranfer to s_DbDetInf */
    GetDbSttInfo(&s_DbDetInf[pstDbCalcInfo->u32HandleNo], pstDbCalcInfo);

    /* Calc counter and size, Update result */
    PQ_MNG_ALG_GetDbDetInfo(&s_DbDetInf_Out[pstDbCalcInfo->u32HandleNo], &s_DetUsing_info[pstDbCalcInfo->u32HandleNo], &s_DbDetInf[pstDbCalcInfo->u32HandleNo]);

    /* Calc to detection and filter */
    DB_Cfg(&sg_stDBValue, &s_DbDetInf_Out[pstDbCalcInfo->u32HandleNo], &s_DetUsing_info[pstDbCalcInfo->u32HandleNo], pstDbCalcInfo->u32HandleNo);

    /* DB detect and filter para Info Set to Reg After Calc */
    PQ_HAL_SetDBApiReg(pstDbCalcInfo->u32HandleNo, &sg_stDBValue);

    return HI_SUCCESS;
}

/**********************************  De-Blocking Function End **********************************/

/********************************** De-Mosquito Function Start **********************************/
static HI_BOOL sg_bDM_EnFlag      = HI_TRUE;
static HI_BOOL sg_bDM_Demo_EnFlag = HI_FALSE;
static HI_BOOL sg_bDMInitFlag     = HI_FALSE;
static HI_S32  sg_s32DMStrength   = 128;/* DM strength */



static DM_CFG_INFO_S g_stDmCfgInfo;


/* DB Init parameter */
static HI_S32 DM_Init_Value(HI_VOID)
{
    /* set flag */
    sg_bDM_EnFlag      = HI_TRUE;
    sg_bDM_Demo_EnFlag = HI_FALSE;

    return HI_SUCCESS;
}

/* Init DR, Vpss Handle ID = 0, Other Handle Init At Top Layer */
static HI_S32 PQ_MNG_InitDM(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    if (HI_TRUE == sg_bDMInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = DM_Init_Value();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DM parameter init error!\n");
        return HI_FAILURE;
    }

    /* Set DM Strength Close It */
    //PQ_HAL_SetDMMidStr(0);

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_DM);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("DM InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDMInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Deinit DR */
static HI_S32 PQ_MNG_DeinitDM(HI_VOID)
{
    if (HI_FALSE == sg_bDMInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDMInitFlag   = HI_FALSE;
    sg_s32DMStrength = 128;

    return HI_SUCCESS;
}

/* Set DM Enable *//* Notice: MV410 Different to Hifone; but can not control dm_en in hifone */
HI_S32 PQ_MNG_EnableDM(HI_BOOL bOnOff)
{
    HI_S32  s32Ret;
    HI_BOOL bDbmEn;
    HI_U32  u32HandleNo = 0;

    s32Ret = PQ_HAL_GetDBEnReg(u32HandleNo, &bDbmEn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Get DB_en Failure!\n");
        return HI_FAILURE;
    }

    if (HI_TRUE == bDbmEn)
    {
        s32Ret = PQ_HAL_EnableDM(u32HandleNo, bOnOff);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("enable DM error!\n");
            return HI_FAILURE;
        }
        sg_bDM_EnFlag = bOnOff;
    }
    else
    {
        /* for rwzb: close dbm_en; but dm is not closed meanwhile */
        s32Ret = PQ_HAL_EnableDM(u32HandleNo, HI_FALSE);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("disable DM error!\n");
            return HI_FAILURE;
        }
        sg_bDM_EnFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

/* Get DM Enable state */
HI_S32 PQ_MNG_GetDMEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bDM_EnFlag;
    return HI_SUCCESS;
}

/* Enable DM DEMO */
HI_S32 PQ_MNG_EnableDMDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; ++u32HandleNo)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableDMDemo(u32HandleNo, bOnOff);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in enable DM Demo!\n");
        return HI_FAILURE;
    }
    sg_bDM_Demo_EnFlag = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DM_API_Init(HI_VOID)
{
    g_stDmCfgInfo.u8NumWindow = 8;

    return HI_SUCCESS;
}

static HI_S32 DBM_ReadDmPara(DetStaticUsing_S* pMnDetInf, DB_DET_INFO_S* s_DbDetInf_In, DM_INF_S* pInf)
{
    PQ_CHECK_NULL_PTR(pMnDetInf);
    PQ_CHECK_NULL_PTR(s_DbDetInf_In);
    PQ_CHECK_NULL_PTR(pInf);

    //pInf->s32RateSmthWin = pMnDetInf->NumberWindow;
    pInf->s32RateSmthWin   = g_stDmCfgInfo.u8NumWindow;

    /* Tran from DB Block size detection */
    pInf->s32LumHBlkSize   = pMnDetInf->UsingSizeHy;
    pInf->s32LumHBlkDetNum = pMnDetInf->UsingHyCounterBorder;
    pInf->u32LumWidth      = s_DbDetInf_In->s32LumWidth;
    pInf->u32LumHeight     = s_DbDetInf_In->s32LumHeight;

    return HI_SUCCESS;
}

DM_INF_S     g_stDmInf[VPSS_HANDLE_NUM];
DM_API_REG_S g_stDmApiReg;

/* Software Alg Function; Take effect in Mv410; no practical meaning In HiFone */
HI_S32 PQ_MNG_DMCalcConfigCfg(DB_CALC_INFO_S* pstDmCalcInfo)
{
    PQ_CHECK_NULL_PTR(pstDmCalcInfo);

    DBM_ReadDmPara(&s_DetUsing_info[pstDmCalcInfo->u32HandleNo], &s_DbDetInf[pstDmCalcInfo->u32HandleNo], &g_stDmInf[pstDmCalcInfo->u32HandleNo]);
    /* Calc */
    PQ_MNG_ALG_UpdateDMCfg(&g_stDmInf[pstDmCalcInfo->u32HandleNo], &g_stDmApiReg);

    PQ_HAL_SetDMApiReg(pstDmCalcInfo->u32HandleNo, &g_stDmApiReg);

    return HI_SUCCESS;
}

/********************************** De-Mosquito Function End **********************************/

/********************************** De-Ringing Function Start **********************************/
/* SD De-Ringing is same to 3798mv100; There is SD DR in 98CV200_A; no practical meaning In Mv410 */
static HI_BOOL sg_bDR_EnFlag;
static HI_BOOL sg_bDR_Demo_EnFlag;


/* DR para init */
static HI_VOID PQ_MNG_DRParaInit(HI_VOID)
{
    return;
}

/* Init DR */
static HI_S32 PQ_MNG_InitDRPara(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    PQ_MNG_DRParaInit();

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_DNR);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("SD DR InitPhyList error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Set DR Enable *//* Notice: It does not exist DR in MV410; DR Enable or not relate to resolution */
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

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; ++u32HandleNo)
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
/********************************** De-Ringing Function Start **********************************/

/********************************** Common Function Start **********************************/
/* DBDM Init Flag */
static HI_BOOL sg_bDNRInitFlag = HI_FALSE;


/* Init DBDM And DR*/
HI_S32 PQ_MNG_InitDNR(PQ_PARAM_S* pstPqParam)
{
    HI_S32 s32Ret;

    g_pstPqParam = pstPqParam;

    if (HI_TRUE == sg_bDNRInitFlag)
    {
        return HI_SUCCESS;
    }

    /* DR */
    s32Ret = PQ_MNG_InitDRPara();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Set DR register error!\n");
        return HI_FAILURE;
    }

    /* DB */
    s32Ret = PQ_MNG_InitDB();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Init DB register error!\n");
        return HI_FAILURE;
    }

    /* DM */
    s32Ret = PQ_MNG_InitDM();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Init DM register error!\n");
        return HI_FAILURE;
    }

    sg_bDNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* Deinit DBDM */
HI_S32 PQ_MNG_DeinitDNR(HI_VOID)
{
    HI_S32 s32Ret;

    if (HI_FALSE == sg_bDNRInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_MNG_DeinitDB();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Deinit DB error!\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_DeinitDM();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Deinit DM error!\n");
        return HI_FAILURE;
    }

    sg_bDNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetDbmDemoMode(DBM_DEMO_MODE_E enMode)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetDbmDemoMode(u32HandleNo, enMode);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    sg_enMode = enMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetDbmDemoMode(DBM_DEMO_MODE_E* penMode)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(penMode);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = HI_SUCCESS;
            break;
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in get dbm demo mode!\n");
        return HI_FAILURE;
    }

    *penMode = sg_enMode;

    return HI_SUCCESS;
}

/********************************** Common Function End **********************************/


