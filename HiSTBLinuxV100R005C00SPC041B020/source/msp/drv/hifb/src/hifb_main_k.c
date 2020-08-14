/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hifb_main_k.c
Version             : Initial Draft
Author              :
Created             : 2015/06/15
Description         : 给开机logo处理使用
Function List       :
History             :
Date                       Author                   Modification
2015/06/15                y00181162                Created file
******************************************************************************/


/*********************************add include here******************************/
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>

#include <linux/slab.h>
#include <linux/mm.h>

#include <linux/fb.h>
#include <asm/uaccess.h>

#include <asm/types.h>
#include <asm/stat.h>
#include <asm/fcntl.h>

#include <linux/interrupt.h>
#include "hi_module.h"
#include "hi_drv_module.h"

#include "drv_hifb_ext.h"
#include "drv_pdm_ext.h"
#include "drv_hifb_osr.h"
#include "optm_define.h"

/**
 **寄存器相关配置
 **/
#include "drv_hifb_config.h"


/***************************** Macro Definition ******************************/

#define HIFB_NAME "HI_FB"


#ifndef HI_ADVCA_FUNCTION_RELEASE
    #define HIFB_PRINT_INFO  HI_PRINT
#else
    #define HIFB_PRINT_INFO(x...)
#endif


#define HIFB_DISPCHN_HD         1
#define HIFB_DISPCHN_SD         0


/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

static volatile HI_U32 *g_u32HDLogoCtrlReg = HI_NULL;
static volatile HI_U32 *g_u32SDLogoCtrlReg = HI_NULL;
static volatile HI_U32 *g_u32SDCtrlReg     = HI_NULL;
static volatile HI_U32 *g_u32WBCCtrlReg    = HI_NULL;
static HI_BOOL gs_bLogoOff                 = HI_FALSE;

extern HIFB_DRV_OPS_S g_stDrvAdpCallBackFunction;

/******************************* API declaration *****************************/


/***************************************************************************************
* func          : HifbSetLogoLayerEnable
* description   : CNcomment: logo图层使能，在这里是关logo使用 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID HifbSetLogoLayerEnable(HI_BOOL bEnable)
{
    PDM_EXPORT_FUNC_S *ps_PdmExportFuncs = HI_NULL;

    if (gs_bLogoOff)
    {
        return;
    }

    *g_u32HDLogoCtrlReg &= ~(0x1 << 31);
    *g_u32SDLogoCtrlReg &= ~(0x1 << 31);
    *g_u32SDCtrlReg     &= ~(0x1 << 31);
    *g_u32WBCCtrlReg    &= ~(0x1 << 31);

    *(g_u32HDLogoCtrlReg + 1) = 0x1;
    *(g_u32SDLogoCtrlReg + 1) = 0x1;
    *(g_u32SDCtrlReg + 1)     = 0x1;

    gs_bLogoOff = HI_TRUE;

    msleep(40);

    if (HI_SUCCESS != HI_DRV_MODULE_GetFunction(HI_ID_PDM, (HI_VOID**)&ps_PdmExportFuncs))
    {
        return;
    }

    if (HI_NULL != ps_PdmExportFuncs)
    {
        #if !defined(CONFIG_PM_HIBERNATE) && !defined(CONFIG_HISI_SNAPSHOT_BOOT)
            ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(DISPLAY_BUFFER_HD);
            ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(DISPLAY_BUFFER_SD);
            ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(OPTM_GFX_WBC2_BUFFER);
            ps_PdmExportFuncs->pfnPDM_ReleaseReserveMem(HIFB_ZME_COEF_BUFFER);
        #endif
    }

    return;
}

/***************************************************************************************
* func          : HifbUpdatePqData
* description   : CNcomment: 更新抗闪类型 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 HifbUpdatePqData(HI_U32 u32UpdateType, PQ_PARAM_S* pstPqParam)
{/** 98M/hifone should no set defliker,pq set by self**/
    #ifdef HI_PQ_V1_0
        switch (u32UpdateType){
            case PQ_CMD_VIRTUAL_GFX_DEFLICKER:
                if(pstPqParam->stPQCoef.stGfxCoef.u32HdCtrlEn){
                    g_stDrvAdpCallBackFunction.HIFB_DRV_SetGpDeflicker(HIFB_DISPCHN_HD, HI_TRUE);
                }else{
                    g_stDrvAdpCallBackFunction.HIFB_DRV_SetGpDeflicker(HIFB_DISPCHN_HD, HI_FALSE);
                }

                if(pstPqParam->stPQCoef.stGfxCoef.u32SdCtrlEn){
                    g_stDrvAdpCallBackFunction.HIFB_DRV_SetGpDeflicker(HIFB_DISPCHN_SD, HI_TRUE);
                }else{
                    g_stDrvAdpCallBackFunction.HIFB_DRV_SetGpDeflicker(HIFB_DISPCHN_SD, HI_FALSE);
                }
                break;
            default:
                break;
         }
    #endif
    return HI_SUCCESS;
}


static HI_VOID DRV_HIFB_SetLogoEnable(HI_BOOL bEnable)
{
     HifbSetLogoLayerEnable(bEnable);
     g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerMaskFlag(HIFB_LAYER_HD_0, HI_FALSE);
     g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerMaskFlag(HIFB_LAYER_SD_0, HI_FALSE);
     return;
}

/***************************************************************************************
* func          : s_HifbExportFuncs
* description   : CNcomment: 向外注册函数给其它模块调用 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HIFB_EXPORT_FUNC_S s_HifbExportFuncs =
{
    .pfnHifbSetLogoLayerEnable = DRV_HIFB_SetLogoEnable,
    .pfnHifbUpdatePqData       = HifbUpdatePqData,
};



HI_S32 DRV_HIFB_ModInit_K(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    /**
     **高清通道开机logo使用的图形层
     **/
    g_u32HDLogoCtrlReg  = (HI_U32*)ioremap_nocache(CONFIG_HIFB_HD_LOGO_REG_BASEADDR,    8);
    /**
     **标清清通道开机logo使用的图形层(双显)
     **/
    g_u32SDLogoCtrlReg  = (HI_U32*)ioremap_nocache(CONFIG_HIFB_SD_LOGO_REG_BASEADDR,    8);
    /**
     **同源回写使用的标清层SD0
     **/
    g_u32SDCtrlReg      = (HI_U32*)ioremap_nocache(CONFIG_HIFB_WBC_SLAYER_REG_BASEADDR, 8);
    /**
     **WBC_GP0
     **/
    g_u32WBCCtrlReg     = (HI_U32*)ioremap_nocache(CONFIG_HIFB_WBC_GP0_REG_BASEADDR,    8);

    s32Ret = HI_GFX_MODULE_Register(HIGFX_FB_ID, HIFB_NAME, &s_HifbExportFuncs);
    if (HI_SUCCESS != s32Ret){
        HIFB_PRINT_INFO("HI_DRV_MODULE_Register failed\n");
        DRV_HIFB_ModExit_K();
        return s32Ret;
    }
    return HI_SUCCESS;
}


HI_VOID DRV_HIFB_ModExit_K(HI_VOID)
{
    HI_GFX_MODULE_UnRegister(HIGFX_FB_ID);
    iounmap(g_u32HDLogoCtrlReg);
    iounmap(g_u32SDLogoCtrlReg);
    iounmap(g_u32SDCtrlReg);
    iounmap(g_u32WBCCtrlReg);
}
