/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_sr.c
  Version       : Initial Draft
  Author        :
  Created       : 2014/1/16
  Description   :

******************************************************************************/
//#include <linux/slab.h>      /* kmalloc() */
#include <linux/string.h>      /* memcpy() */
#include <linux/fcntl.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"

#include "pq_hal_comm.h"
#include "pq_hal_sr.h"
#include "drv_pq_table.h"
#include "pq_mng_sr.h"


static HI_BOOL   sg_bSREnableFlag = HI_FALSE;
static HI_BOOL   sg_bSRInitFlag   = HI_FALSE;
static SR_DEMO_E sg_enSRMode      = SR_ENABLE_A;


/* 初始化SR参数 */
HI_S32 PQ_MNG_SRParaInit(HI_VOID)
{
    return HI_SUCCESS;
}

/* 初始化SR模块 */
HI_S32 PQ_MNG_InitSR(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32ID = 0;

    s32Ret = PQ_MNG_SRParaInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_SRParaInit error\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_InitPhyList(u32ID, REG_TYPE_VDP, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_SR);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("SR InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bSRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* 去初始化SR模块 */
HI_S32 PQ_MNG_DeinitSR(HI_VOID)
{
    if (HI_FALSE == sg_bSRInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bSRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

/* 获取SR模块DEMO模式 */
HI_S32 PQ_MNG_GetSRDemo(HI_U32* pu32Type)
{
    *pu32Type = sg_enSRMode;

    return HI_SUCCESS;
}

/* 设置SR模块DEMO显示模式 */
HI_S32 PQ_MNG_SetSRDemo(SR_DEMO_E enDemoCtrl)
{
    switch (enDemoCtrl)
    {
        case SR_DISABLE:
        {
            PQ_HAL_SR_EnZme(VDP_LAYER_VID0, SR_ONLY_ZME_EN);
            break;
        }
        case SR_ENABLE_R:
        {
            PQ_HAL_SR_EnZme(VDP_LAYER_VID0, SR_AND_ZME_EN_R);
            break;
        }
        case SR_ENABLE_L:
        {
            PQ_HAL_SR_EnZme(VDP_LAYER_VID0, SR_AND_ZME_EN_L);
            break;
        }
        case SR_ENABLE_A:
        {
            PQ_HAL_SR_EnZme(VDP_LAYER_VID0, SR_AND_ZME_EN);
            break;
        }
        default:
        {
            HI_ERR_PQ("SR demo type error\n");
            return HI_FAILURE;
        }
    }

    sg_enSRMode = enDemoCtrl;

    return HI_SUCCESS;
}

/* 开关SR模块 */
HI_S32 PQ_MNG_EnableSR(HI_BOOL bOnOff)
{
    HI_S32    s32Ret;
    HI_BOOL   bSrOnOff;
    SR_DEMO_E enDemo;

    /* VDP CTRL SR; Virtual Control In PQ */
    PQ_HAL_GetSREnable(VDP_LAYER_VID0, &bSrOnOff);/* Get From RegList */

    if (HI_TRUE == bSrOnOff)
    {
        if (HI_TRUE == bOnOff)
        {
            enDemo = SR_ENABLE_A;
        }
        else
        {
            enDemo = SR_DISABLE;
        }

        s32Ret = PQ_MNG_SetSRDemo(enDemo);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("SR Enable error\n");
            return HI_FAILURE;
        }
        sg_bSREnableFlag = bOnOff;
    }
    else
    {
        sg_bSREnableFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetSREnableFlag(HI_BOOL* bOnOff)
{
    HI_BOOL bSrOnOff;

    PQ_HAL_GetSREnable(VDP_LAYER_VID0, &bSrOnOff);/* Get From RegList */

    if ( HI_TRUE == bSrOnOff )
    {
        *bOnOff = sg_bSREnableFlag;
    }
    else
    {
        *bOnOff = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableSRDemo(HI_BOOL bOnOff)
{
    HI_BOOL bSrOnOff;
    SR_DEMO_E enDemo;

    PQ_HAL_GetSREnable(VDP_LAYER_VID0, &bSrOnOff);/* Get From RegList */

    if ( HI_TRUE == bSrOnOff )
    {
        /* Set Demo To Left Close, Right Open */
        enDemo = (bOnOff == HI_TRUE) ? SR_ENABLE_R : SR_ENABLE_A;
    }
    else
    {
        enDemo = SR_DISABLE;
    }

    return PQ_MNG_SetSRDemo(enDemo);
}

/* SR模块中的Sharpen的模块增强和细节增强 */
HI_S32 PQ_MNG_UpdateSRSpMode(HI_U32 u32Width, HI_U32 u32Hight)
{
    SR_SHARP_MODE_E eHsh_nodir = SR_SHARP_MODE_DETAIL;

    if ((u32Width >= 1920) && (u32Hight >= 1080))
    {
        eHsh_nodir = SR_SHARP_MODE_TRAD;
    }

    PQ_HAL_SetSRSpMode(VDP_LAYER_VID0, eHsh_nodir);

    return HI_SUCCESS;
}


