/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : drv_hifb_adp.c
Version          : Initial Draft
Author           : y00181162
Created          : 2016/05/09
Description      :
Function List    :


History          :
Date                Author                Modification
2016/05/09          y00181162            Created file
******************************************************************************/


/***************************** add include here*********************************/
#ifndef HI_BUILD_IN_BOOT
    #include <linux/string.h>
    #include <linux/fb.h>
    #include <linux/time.h>
    #include <linux/version.h>
    #include <linux/module.h>
    #include <linux/types.h>
    #include <linux/errno.h>
    #include <linux/fcntl.h>
    #include <linux/mm.h>
    #include <linux/miscdevice.h>
    #include <linux/proc_fs.h>
    #include <linux/fs.h>
    #include <linux/slab.h>
    #include <linux/init.h>
    #include <linux/delay.h>
    #include <asm/uaccess.h>
    #include <asm/io.h>
    #include <linux/interrupt.h>
    #include <linux/ioport.h>
    #include <linux/string.h>
    #include <linux/workqueue.h>
    #include "hi_module.h"
    #include "hi_drv_module.h"
#else
    #include "hifb_debug.h"
    #include "hi_drv_pq.h"
#endif


#include "drv_disp_ext.h"
#include "drv_hifb_hal.h"
#include "drv_hifb_adp.h"
#include "optm_alg_csc.h"
#include "optm_alg_gzme.h"
#include "optm_alg_gsharp.h"
#include "hi_drv_disp.h"
#include "drv_hifb_wbc.h"
#include "drv_pq_ext.h"

/**
 **所有发生变化的所在的头文件
 **/
#include "drv_hifb_config.h"
#include "hi_gfx_sys_k.h"

/***************************** Macro Definition ********************************/

#ifndef HI_BUILD_IN_BOOT
static DISP_EXPORT_FUNC_S *ps_DispExportFuncs = HI_NULL;
#endif

#ifndef HI_BUILD_IN_BOOT
/**
 ** wait v blanking
 **/
#define OPTM_HIFB_WVM_ENABLE                1
/**
 ** call-back after registers' update
 **/
#define OPTM_HIFB_GFXRR_ENABLE              1

#define CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH     4096
#define CONFIG_GFX_DISP_4K_VIRTUAL_WIDTH           3840
#define CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT          2160
#endif


#define DispWidth_HD                     1280
#define DispHeight_HD                    720
#define DispWidth_SD                     720
#define DispHeight_SD                    576
#define OPTM_COLOR_DEFVALUE              50
#define OPTM_EXTRACTLINE_RATIO           4
#define OPTM_EXTRACTLINE_WIDTH           1080


/**
 ** G3(中间空掉G2，这里HD_2 -> G3)一直是做为鼠标层，
 ** CV200绑定到FB5中，所以CV200用的是SD1
 **/
#define OPTM_CURSOR_LAYERID                    HIFB_LAYER_SD_1
/**
 **gp support max layer number
 **/
#define OPTM_GP_SUPPORT_MAXLAYERNUM            5
#define OPTM_GP0_GFX_COUNT                     CONFIG_HIFB_GP0_SUPPORT_GFX_COUNT
#define OPTM_GP1_GFX_COUNT                     CONFIG_HIFB_GP1_SUPPORT_GFX_COUNT
#define OPTM_GFXCLUT_LENGTH                    256

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    #define OPTM_COMPRESSION_INTERVAL          2
    #define OPTM_COMPRESSION_ALIGNMENT         16
    #define OPTM_COMPRESSION_WIDTH_MAX         2048
    #define OPTM_COMPRESSION_HEAD_STRIDE_32    32
    #define OPTM_COMPRESSION_HEAD_STRIDE_16    16
    #define OPTM_COMPRESSION_MDDRC_ZONE        32
#endif


#define D_OPTM_HIFB_CheckGfxOpen(enLayerId)  \
        do{ if (enLayerId >= HIFB_LAYER_ID_BUTT){\
              HIFB_ERROR("no suppout Gfx%d!\n",enLayerId);       \
              return HI_FAILURE;}\
            if (g_stGfxDevice[enLayerId].bOpened != HI_TRUE){    \
              HIFB_ERROR("Error! Gfx%d not open!\n",enLayerId);  \
              return HI_FAILURE;}\
        }while(0)


/***************************** Structure Definition ****************************/

typedef enum tagOPTM_VDP_CONNECT_E
{
    OPTM_VDP_CONNECT_G3_DHD0 = 0,
    OPTM_VDP_CONNECT_G3_DHD1    ,
    OPTM_VDP_CONNECT_BUTT       ,
}OPTM_VDP_CONNECT_E;

typedef enum tagOPTM_WBC_BUFFER_STATUS_E{
    OPTM_WBC_BUFFER_RELEASED = 0,
    OPTM_WBC_BUFFER_ACQUIRED
}OPTM_WBC_BUFFER_STATUS_E;

/**
 ** IRQ Para
 ** 中断处理相关的参数信息
 **/
typedef struct tagOPTM_GFX_IRQ_S
{
    HI_VOID* pParam0;  /** layer id **/
    HI_U32   u32Param1;
    IntCallBack pFunc; /** irq function **/
}OPTM_GFX_IRQ_S;

/** each bit: 0---no irq,1---irq
 ** 0---HIFB_CALLBACK_TYPE_VO
 ** 1---HIFB_CALLBACK_TYPE_3DMode_CHG
 ** 2---HIFB_CALLBACK_TYPE_REGUP
 ** 3---HIFB_CALLBACK_TYPE_FRAME_START
 ** 4---HIFB_CALLBACK_TYPE_FRAME_END
 **/
typedef struct tagOPTM_GFX_CALLBACK_S
{
    HI_U32  u32CTypeFlag;
    OPTM_GFX_IRQ_S stGfxIrq[HIFB_CALLBACK_TYPE_BUTT];
}OPTM_GFX_CALLBACK_S;


/**
 ** Gp only need to register callback func to disp once
 **/
typedef struct tagOPTM_GP_IRQ_S
{
    HI_BOOL bRegistered[HI_DRV_DISP_C_TYPE_BUTT];
    OPTM_GFX_CALLBACK_S stGfxCallBack[OPTM_GP_SUPPORT_MAXLAYERNUM];
}OPTM_GP_IRQ_S;



/***************************** Global Variable declaration *********************/


static volatile HI_U32  *gs_pu32Phyaddr = HI_NULL;

/** ZME系数buffer **/
#ifndef CONFIG_GFX_PQ
static OPTM_ALG_GZME_MEM_S  gs_stGPZme;
#endif

static HI_U32 g_u32GFXInitFlag      = OPTM_DISABLE;


/* WORKMODE */
HIFB_GFX_MODE_EN g_enOptmGfxWorkMode = HIFB_GFX_MODE_NORMAL;

/**
 ** gfx0,gfx1,gfx2,gfx3,gfx4,gfx5
 **/
OPTM_GFX_LAYER_S g_stGfxDevice[OPTM_MAX_LOGIC_HIFB_LAYER];

/**
 ** graphics process device gp0 and gp1
 ** gp0: process gfx0,gfx1,gfx2,gfx3
 ** gp1: process gfx4,gfx5
 **/
OPTM_GFX_GP_S g_stGfxGPDevice[OPTM_GFX_GP_BUTT];
/**
 ** save irq info of each gfx
 **/
static OPTM_GP_IRQ_S g_stGfxGPIrq[OPTM_GFX_GP_BUTT];
extern OPTM_GFX_WBC_S  g_stGfxWbc2;

#ifndef HI_BUILD_IN_BOOT
   static PQ_EXPORT_FUNC_S*   gs_pstPqFuncs = HI_NULL;
   static HIFB_LAYER_ID_E     gs_CallBackLayerId[HIFB_LAYER_ID_BUTT];
#endif

const HIFB_CAPABILITY_S g_stGfxCap[OPTM_MAX_LOGIC_HIFB_LAYER] =
{
    /* HD0 */
    {
        .bKeyAlpha       = 1,
        .bGlobalAlpha    = 1,
        .bCmap           = 1,
        .bHasCmapReg     = 1,
        .bColFmt         = {1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,0,0,     \
                            0,0,0,0,     \
                            0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_HD0_LAYER_SUPPORT,
        .bCompression    = CONFIG_HIFB_HD0_COMPRESSION_SUPPORT,
        .bStereo         = 1,
        .u32MaxWidth     = CONFIG_HIFB_HD_LAYER_MAXWIDTH,
        .u32MaxHeight    = CONFIG_HIFB_HD_LAYER_MAXHEIGHT,
        .u32MinWidth     = 0,
        .u32MinHeight    = 0,
        .u32VDefLevel    = 0,  /* not surpport */
        .u32HDefLevel    = 0,  /* not surpport */
    },

    /* HD1 */
    {
        .bKeyAlpha       = 1,
        .bGlobalAlpha    = 1,
        .bCmap           = 1,
        .bHasCmapReg     = 1,
        .bColFmt         = {CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_24BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT, \
                            CONFIG_HIFB_HD1_LAYER_32BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_32BIT_FMT, \
                            CONFIG_HIFB_HD1_LAYER_24BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_24BIT_FMT, \
                            CONFIG_HIFB_HD1_LAYER_32BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_24BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT, \
                            CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_32BIT_FMT,CONFIG_HIFB_HD1_LAYER_24BIT_FMT,CONFIG_HIFB_HD1_LAYER_16BIT_FMT, \
                            CONFIG_HIFB_HD1_LAYER_16BIT_FMT,CONFIG_HIFB_HD1_LAYER_32BIT_FMT,CONFIG_HIFB_HD1_LAYER_CLUT1_FMT,CONFIG_HIFB_HD1_LAYER_CLUT2_FMT, \
                            CONFIG_HIFB_HD1_LAYER_CLUT4_FMT,CONFIG_HIFB_HD1_LAYER_CLUT8_FMT,0,0,  \
                            0,0,0,0,                                                              \
                            0,0},
        .bVoScale        = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_HD1_LAYER_SUPPORT,
        .bCompression    = CONFIG_HIFB_HD1_COMPRESSION_SUPPORT,
        .bStereo           = 0,
        .u32MaxWidth       = CONFIG_HIFB_HD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_HD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel     = 0,  /* not surpport */
    },
    /* HD2 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 1,
        .bHasCmapReg       = 1,
        .bColFmt         = {1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,1,1,     \
                            1,1,0,0,     \
                            0,0,0,0,     \
                            0,0,0,0,     \
                            0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_HD2_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_HD2_COMPRESSION_SUPPORT,
        .bStereo           = 0,
        .u32MaxWidth       = CONFIG_HIFB_HD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_HD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* HD3 */
    {
        .bKeyAlpha        = 1,
        .bGlobalAlpha      = 1,
        .bCmap            = 1,
        .bHasCmapReg       = 1,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale         = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_HD3_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_HD3_COMPRESSION_SUPPORT,
        .bStereo           = 0,
        .u32MaxWidth       = CONFIG_HIFB_HD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_HD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel     = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* SD0 */
    {
        .bKeyAlpha        = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 1,
        .bHasCmapReg       = 1,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,0,0,  0,1,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_SD0_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_SD0_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_SD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_SD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel     = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* SD1 */
    {
        .bKeyAlpha        = 1,
        .bGlobalAlpha     = 1,
        .bCmap             = 1,
        .bHasCmapReg       = 1,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,0,0,  0,1,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_FALSE,
        .bLayerSupported = CONFIG_HIFB_SD1_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_SD1_COMPRESSION_SUPPORT,
        .bStereo          = 1,
        .u32MaxWidth       = CONFIG_HIFB_SD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_SD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* SD2 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,1,1,  1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_FALSE,
        .bLayerSupported = CONFIG_HIFB_SD2_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_SD2_COMPRESSION_SUPPORT,
        .bStereo           = 0,
        .u32MaxWidth       = CONFIG_HIFB_SD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_SD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* SD3 */
    {
        .bKeyAlpha        = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_FALSE,
        .bLayerSupported = CONFIG_HIFB_SD3_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_SD3_COMPRESSION_SUPPORT,
        .bStereo          = 0,
        .u32MaxWidth       = CONFIG_HIFB_SD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_SD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },
#ifndef HI_BUILD_IN_BOOT
    /* AD0 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt          = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_AD0_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_AD0_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_AD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_AD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight     = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* AD1 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg        = 0,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_AD1_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_AD1_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_AD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_AD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* AD2 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt            = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_AD2_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_AD2_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_AD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_AD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* AD3 */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_AD3_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_AD3_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_AD_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_AD_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },

    /* CURSOR */
    {
        .bKeyAlpha         = 1,
        .bGlobalAlpha      = 1,
        .bCmap             = 0,
        .bHasCmapReg       = 0,
        .bColFmt           = {1,1,1,1,  1,1,1,1,  1,1,1,1,   1,1,1,1,   1,1,1,1,   1,1,0,0,  0,0,0,0,  0,0,0,0,  0,0}, /** 36 **/
        .bVoScale          = HI_TRUE,
        .bLayerSupported = CONFIG_HIFB_CURSOR_LAYER_SUPPORT,
        .bCompression      = CONFIG_HIFB_CURSOR_COMPRESSION_SUPPORT,
        .bStereo           = 1,
        .u32MaxWidth       = CONFIG_HIFB_CURSOR_LAYER_MAXWIDTH,
        .u32MaxHeight      = CONFIG_HIFB_CURSOR_LAYER_MAXHEIGHT,
        .u32MinWidth       = 0,
        .u32MinHeight      = 0,
        .u32VDefLevel      = 0,  /* not surpport */
        .u32HDefLevel      = 0,  /* not surpport */
    },
#endif
};

/***************************** API forward declarations ************************/

#ifdef OPTM_HIFB_WVM_ENABLE
HI_VOID OPTM_GfxWVBCallBack(HI_U32 enLayerId, HI_U32 u32Param1);
#endif


#ifndef HI_BUILD_IN_BOOT
HI_S32  OPTM_GFX_SetStereoDepth    (HIFB_LAYER_ID_E enLayerId, HI_S32 s32Depth);
HI_S32  OPTM_Distribute_Callback   (HI_VOID* u32Param0, HI_VOID* u32Param1);
HI_VOID DRV_HIFB_ADP_FrameEndCallBack (HI_VOID* u32Param0, HI_VOID* u32Param1);
#endif

HI_S32 OPTM_GPRecovery             (OPTM_GFX_GP_E enGPId);
HI_S32 OPTM_GfxSetDispFMTSize      (OPTM_GFX_GP_E enGpId, const HI_RECT_S *pstOutRect);
HI_S32 OPTM_GPSetCsc               (OPTM_GFX_GP_E enGfxGpId, OPTM_GFX_CSC_PARA_S *pstCscPara, HI_BOOL bIsBGRIn);

#ifndef CONFIG_GFX_PQ
OPTM_COLOR_SPACE_E OPTM_AdaptCscTypeFromDisp(HI_DRV_COLOR_SPACE_E enHiDrvCsc);
#else
HI_PQ_GFX_COLOR_SPACE_E PQ_AdaptCscTypeFromDisp(HI_DRV_COLOR_SPACE_E enHiDrvCsc);
#endif


#ifdef CFG_HIFB_COMPRESSION_SUPPORT
HI_VOID OPTM_GFX_CMP_Clean         (HIFB_LAYER_ID_E enLayerId);
HI_S32 OPTM_GFX_CMP_Open           (HIFB_LAYER_ID_E enLayerId);
HI_S32 OPTM_GFX_CMP_Close          (HIFB_LAYER_ID_E enLayerId);
HI_VOID OPTM_GFX_CMP_Process       (HI_VOID* u32Param0);
HI_VOID OPTM_GFX_CMP_ProcessNOCMP  (HIFB_LAYER_ID_E enLayerId);
#endif

HI_S32 OPTM_DispInfoUpdate         (OPTM_VDP_LAYER_GP_E enGPId);




static inline OPTM_VDP_GFX_IFMT_E OPTM_ClutPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt);
static inline OPTM_VDP_GFX_IFMT_E OPTM_RGBPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt);
static inline OPTM_VDP_GFX_IFMT_E OPTM_XRGBPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt);
static inline OPTM_VDP_GFX_IFMT_E OPTM_XYCbCrPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt);

#ifndef HI_BUILD_IN_BOOT
static inline HIFB_COLOR_FMT_E OPTM_ClutHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt);
static inline HIFB_COLOR_FMT_E OPTM_RGBHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt);
static inline HIFB_COLOR_FMT_E OPTM_XRGBHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt);
static inline HIFB_COLOR_FMT_E OPTM_XYCbCrHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt);
static inline HI_VOID HIFB_ADP_ReOpen(OPTM_GFX_GP_E enGpId);
static inline HI_VOID HIFB_ADP_ReClose(OPTM_GFX_GP_E enGpId);
static inline HI_VOID HIFB_ADP_DispSetting(OPTM_GFX_GP_E enGpId, HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo);
static inline HI_VOID HIFB_ADP_UpdataLayerInfo(HI_VOID* u32Param0, HI_VOID* u32Param1);
#endif


static inline HI_VOID HIFB_ADP_ResetLayerInRect(OPTM_GFX_GP_E enGpId);


#ifndef CONFIG_GFX_PQ
static inline HI_VOID HIFB_ADP_SetZme(OPTM_GFX_GP_E enGpId, HI_BOOL bGfxSharpen, OPTM_ALG_GZME_RTL_PARA_S *pstZmeRtlPara, OPTM_ALG_GDTI_RTL_PARA_S *pstDtiRtlPara);
#endif


static inline HI_S32 HIFB_ADP_SetLayerDataBigEndianFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt);
static inline HI_S32 HIFB_ADP_SetLayerDataLittleEndianFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt);


/***************************** API realization *********************************/


/*****************************************************************************
* func         :  OPTM_AlignUp
: description  :  align byte
               :  字节对齐
* retval       :  NA
* others:       :  NA
*****************************************************************************/
HI_U32 OPTM_AlignUp(HI_U32 x, HI_U32 a)
{
    if(!a){
        return x;
    }else{
        return ( (( x + (a-1) ) / a ) * a);
    }
}


#ifdef HI_BUILD_IN_BOOT

#define IO_ADDRESS(addr) (addr)

/***************************************************************************************
* func          : Win_ReviseOutRect
* description   : CNcomment: 改变窗口输出大小,在fastboot中实现  CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
extern HI_S32 Win_ReviseOutRect(const HI_RECT_S *tmp_virtscreen,
                                const HI_DRV_DISP_OFFSET_S *stOffsetInfo,
                                const HI_RECT_S *stFmtResolution,
                                const HI_RECT_S *stPixelFmtResolution,
                                HI_RECT_S *stToBeRevisedRect,
                                HI_RECT_S *stRevisedRect);

/***************************************************************************************
* func          : OPTM_GpInitFromDisp
* description   : CNcomment: 初始化参数，参数来源于display  CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GpInitFromDisp(OPTM_GFX_GP_E enGPId, HI_BOOL Support4KLogo)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_DISP_DISPLAY_INFO_S pstInfo;

#ifndef CONFIG_GFX_PQ
    OPTM_COLOR_SPACE_E  enGpCsc;
#else
    HI_PQ_GFX_COLOR_SPACE_E  enGpCsc;
#endif
    if(!g_stGfxGPDevice[enGPId].bOpen){
        return HI_FAILURE;
    }

    if(enGPId == OPTM_GFX_GP_0){
        enDisp = HI_DRV_DISPLAY_1;
    }else{
        enDisp = HI_DRV_DISPLAY_0;
    }

    HI_GFX_Memset(&pstInfo, 0, sizeof(HI_DISP_DISPLAY_INFO_S));

    HI_DISP_GetDisplayInfo(enDisp, &pstInfo);
    g_stGfxGPDevice[enGPId].bInterface = pstInfo.bInterlace;


    Win_ReviseOutRect(&pstInfo.stVirtaulScreen, &pstInfo.stOffsetInfo,
                      &pstInfo.stFmtResolution, &pstInfo.stPixelFmtResolution,
                      &pstInfo.stVirtaulScreen, &g_stGfxGPDevice[enGPId].stOutRect);

    /**=============================================================================**/
    /** boot logo no need resize, ensure let input rect equal to out rect **/
    if ((OPTM_GFX_GP_0 == enGPId) && (HI_TRUE == Support4KLogo))
    {
        g_stGfxGPDevice[enGPId].stOutRect.s32X      = 0;
        g_stGfxGPDevice[enGPId].stOutRect.s32Y      = 0;
        g_stGfxGPDevice[enGPId].stOutRect.s32Width  = pstInfo.stFmtResolution.s32Width;
        g_stGfxGPDevice[enGPId].stOutRect.s32Height = pstInfo.stFmtResolution.s32Height;
    }
    /**=============================================================================**/
#ifdef HI_PQ_V1_0
    g_stGfxGPDevice[enGPId].stCscPara.u32Bright     = pstInfo.u32Bright;
    g_stGfxGPDevice[enGPId].stCscPara.u32Contrast   = pstInfo.u32Contrst;
    g_stGfxGPDevice[enGPId].stCscPara.u32Hue        = pstInfo.u32Hue;
    g_stGfxGPDevice[enGPId].stCscPara.u32Kb         = pstInfo.u32Kb;
    g_stGfxGPDevice[enGPId].stCscPara.u32Kg         = pstInfo.u32Kg;
    g_stGfxGPDevice[enGPId].stCscPara.u32Kr         = pstInfo.u32Kr;
    g_stGfxGPDevice[enGPId].stCscPara.u32Saturation = pstInfo.u32Satur;
#endif

    if(pstInfo.bIsMaster && enGPId == OPTM_GFX_GP_0){
        g_enOptmGfxWorkMode = HIFB_GFX_MODE_HD_WBC;
    }

    OPTM_GPRecovery(enGPId);

    enGpCsc = OPTM_AdaptCscTypeFromDisp(pstInfo.eColorSpace);
#ifdef HI_PQ_V1_0
    if (g_stGfxGPDevice[enGPId].enOutputCsc != enGpCsc)
#endif
    {
        g_stGfxGPDevice[enGPId].enOutputCsc = enGpCsc;
        OPTM_GPSetCsc(enGPId, &g_stGfxGPDevice[enGPId].stCscPara, g_stGfxGPDevice[enGPId].bBGRState);
    }

    return HI_SUCCESS;
}
#endif


/***************************************************************************************
* func          : OPTM_GFX_GetDevCap
* description   : CNcomment: 获取图层能力级 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID  OPTM_GFX_GetDevCap(const HIFB_CAPABILITY_S **pstCap)
{
    *pstCap = &g_stGfxCap[0];
}

/***************************************************************************************
* func          : OPTM_GetGfxHalId
* description   : CNcomment: hifb ID 转换成 图层ID，配寄存器使用 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
OPTM_VDP_LAYER_GFX_E OPTM_GetGfxHalId(HIFB_LAYER_ID_E enLayerId)
{

    if(HIFB_LAYER_SD_1 == enLayerId && OPTM_GP0_GFX_COUNT < 4){
    /** GP0支持的图层数小于的时候才支持挂载，区别98C/96C/hifone的 **/
            #ifdef CONFIG_HIFB_GFX3_TO_GFX5
        /** 支持将高清层挂接到标清通道上，支持TC双显功能**/
            return OPTM_VDP_LAYER_GFX3;
        #else
            return OPTM_VDP_LAYER_GFX_BUTT;
        #endif
    }else if(enLayerId < HIFB_LAYER_SD_1){
        return (OPTM_VDP_LAYER_GFX_E)enLayerId;
    }else{
        return OPTM_VDP_LAYER_GFX_BUTT;
    }
}

/***************************************************************************************
* func          : OPTM_AdaptCscTypeFromDisp
* description   : CNcomment: 从disp中获取CSC类型 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifndef CONFIG_GFX_PQ
OPTM_COLOR_SPACE_E OPTM_AdaptCscTypeFromDisp(HI_DRV_COLOR_SPACE_E enHiDrvCsc)
{
    switch(enHiDrvCsc)
    {
        case HI_DRV_CS_BT601_YUV_LIMITED:
            return OPTM_CS_BT601_YUV_LIMITED;
        case HI_DRV_CS_BT601_YUV_FULL:
            return OPTM_CS_BT601_YUV_FULL;
        case HI_DRV_CS_BT601_RGB_FULL:
            return OPTM_CS_BT601_RGB_FULL;
        case HI_DRV_CS_BT709_YUV_LIMITED:
            return OPTM_CS_BT709_YUV_LIMITED;
        case HI_DRV_CS_BT709_YUV_FULL:
            return OPTM_CS_BT709_YUV_FULL;
        case HI_DRV_CS_BT709_RGB_FULL:
            return OPTM_CS_BT709_RGB_FULL;
        default:
            return OPTM_CS_BUTT;
    }
}
#else
HI_PQ_GFX_COLOR_SPACE_E PQ_AdaptCscTypeFromDisp(HI_DRV_COLOR_SPACE_E enHiDrvCsc)
{
    switch(enHiDrvCsc)
    {
        case HI_DRV_CS_BT601_YUV_LIMITED:
            return PQ_GFX_CS_BT601_YUV_LIMITED;
        case HI_DRV_CS_BT601_YUV_FULL:
            return PQ_GFX_CS_BT601_YUV_FULL;
        case HI_DRV_CS_BT601_RGB_FULL:
            return PQ_GFX_CS_BT601_RGB_FULL;
        case HI_DRV_CS_BT709_YUV_LIMITED:
            return PQ_GFX_CS_BT709_YUV_LIMITED;
        case HI_DRV_CS_BT709_YUV_FULL:
            return PQ_GFX_CS_BT709_YUV_FULL;
        case HI_DRV_CS_BT709_RGB_FULL:
            return PQ_GFX_CS_BT709_RGB_FULL;
        default:
            return PQ_GFX_CS_BUTT;
    }
}
#endif


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_AdaptTriDimModeFromDisp
* description   : CNcomment: 从disp中获取3D模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HIFB_STEREO_MODE_E OPTM_AdaptTriDimModeFromDisp(OPTM_VDP_DISP_MODE_E enDispStereo)
{
    switch(enDispStereo)
    {
        case VDP_DISP_MODE_2D:
            return HIFB_STEREO_MONO;
        case VDP_DISP_MODE_SBS:
            return HIFB_STEREO_SIDEBYSIDE_HALF;
        case VDP_DISP_MODE_TAB:
            return HIFB_STEREO_TOPANDBOTTOM;
        case VDP_DISP_MODE_FP:
            return HIFB_STEREO_FRMPACKING;
        default:
            return HIFB_STEREO_BUTT;
    }

    return HIFB_STEREO_BUTT;
}
#endif


/***************************************************************************************
* func          : OPTM_GPDATA_Init
* description   : CNcomment: GP data inital  CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GPDATA_Init(HI_VOID)
{
    HI_GFX_Memset(&(g_stGfxDevice[0])   , 0, sizeof(OPTM_GFX_LAYER_S) * OPTM_MAX_LOGIC_HIFB_LAYER);
    HI_GFX_Memset(&(g_stGfxGPDevice[0]) , 0, sizeof(OPTM_GFX_GP_S) * OPTM_GFX_GP_BUTT);
    HI_GFX_Memset(&(g_stGfxGPIrq[0])    , 0, sizeof(OPTM_GP_IRQ_S) * OPTM_GFX_GP_BUTT);
}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_Aapt_Module_GetFunction
* description   : CNcomment: 获取函数指针 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_Aapt_Module_GetFunction(HI_U32 u32ModuleID, HI_VOID** ppFunc)
{
    if(HI_NULL == ppFunc){
        return HI_FAILURE;
    }

    if(HI_SUCCESS != HI_DRV_MODULE_GetFunction(u32ModuleID, ppFunc)){
        return HI_FAILURE;
    }

    if (HI_NULL == *ppFunc){
        return HI_FAILURE;
    }else{
        return HI_SUCCESS;
    }
}
#endif


/***************************************************************************************
* func          : OPTM_GfxInit
* description   : CNcomment: 驱动初始化 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxInit(HI_VOID)
{

#if !defined(CONFIG_GFX_PQ) || defined(HI_BUILD_IN_BOOT)
    HI_S32 s32Ret     = HI_SUCCESS;
#endif

    if(OPTM_ENABLE == g_u32GFXInitFlag){
        /** 保证只初始化一次 **/
        return HI_SUCCESS;
    }
    /**
     **vdp register address
     **/
#ifdef HI_BUILD_IN_BOOT
    gs_pu32Phyaddr = (volatile HI_U32*)IO_ADDRESS(CONFIG_VDP_REG_BASEADDR);
#else
    gs_pu32Phyaddr = (volatile HI_U32*)HI_GFX_REG_MAP(CONFIG_VDP_REG_BASEADDR, CONFIG_VDP_SIZE);
#endif
    if(NULL == gs_pu32Phyaddr){
        HIFB_ERROR("map vdp register failed\n");
        return HI_FAILURE;
    }


    OPTM_VDP_DRIVER_Initial(gs_pu32Phyaddr);

    /**
     **压缩解压
     **/
    #ifdef CFG_HIFB_COMPRESSION_SUPPORT
        u32Phyaddr = (HI_U32)IO_ADDRESS(CONFIG_HIFB_MDDRC_REG_BASEADDR);
        OPTM_MDDRC_DRIVER_Initial(u32Phyaddr);
    #endif


    /**
     **初始化gfx gp gpirq wbc全局变量
     **/
    OPTM_GPDATA_Init();
    HIFB_WBC2_Init();

    /**
     **加载ZME系数，这个是有算法提供
     **/
#ifndef CONFIG_GFX_PQ
    s32Ret = OPTM_ALG_GZmeVdpComnInit(&gs_stGPZme);
    if (HI_SUCCESS != s32Ret){
        HIFB_ERROR("Malloc Gfxzme coeff buffer failed\n");
        return s32Ret;
    }
#endif

    /** has been initial **/
    g_u32GFXInitFlag = OPTM_ENABLE;

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GfxDeInit
* description   : CNcomment: 图形设备去初始化 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxDeInit(HI_VOID)
{
#ifndef HI_BUILD_IN_BOOT
    HI_U32 u32LayerId = HIFB_LAYER_HD_0;

    if(OPTM_DISABLE == g_u32GFXInitFlag){
        return HI_SUCCESS;
    }

    for(u32LayerId = HIFB_LAYER_HD_0; u32LayerId < HIFB_LAYER_ID_BUTT; u32LayerId++){
        if(HI_FALSE != g_stGfxDevice[u32LayerId].bOpened){
            OPTM_GfxCloseLayer(u32LayerId);
        }
        #ifdef CFG_HIFB_COMPRESSION_SUPPORT
            OPTM_GFX_CMP_Clean(u32LayerId);
        #endif
    }

    #ifndef CONFIG_GFX_PQ
        /** load Gfx zoom coefficients **/
        OPTM_ALG_GZmeVdpComnDeInit(&gs_stGPZme);
    #endif

    HIFB_WBC2_Dinit();
    HIFB_WBC2_SetTcFlag(HI_FALSE);

    g_u32GFXInitFlag   = OPTM_DISABLE;
    ps_DispExportFuncs = HI_NULL;

    if(NULL != gs_pu32Phyaddr){
        HI_GFX_REG_UNMAP((HI_VOID*)gs_pu32Phyaddr);
    }
#endif

    return HI_SUCCESS;

}


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_3DMode_Callback
* description   : CNcomment: 执行3D工作对列函数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID OPTM_3DMode_Callback(struct work_struct *data)
{
    HI_U32          i;
    HI_U32          u32CTypeFlag;
    HI_U32          u32LayerCount;
    OPTM_GFX_GP_E   enGpHalId;
    OPTM_GFX_WORK_S *pst3DModeWork = HI_NULL;

    if(HI_NULL == data){
        return;
    }

    pst3DModeWork = container_of(data, OPTM_GFX_WORK_S, work);
    enGpHalId     = (OPTM_GFX_GP_E)(pst3DModeWork->u32Data);

    if(enGpHalId >= OPTM_GFX_GP_BUTT){
        return;
    }

    u32LayerCount = (OPTM_GFX_GP_0 == enGpHalId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;

    for (i = 0; i < u32LayerCount;i++){
        u32CTypeFlag = g_stGfxGPIrq[enGpHalId].stGfxCallBack[i].u32CTypeFlag;

        if(!u32CTypeFlag){
            continue;
        }

        if (u32CTypeFlag & HIFB_CALLBACK_TYPE_3DMode_CHG){
            /*callback function*/
            if (g_stGfxGPIrq[enGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_3DMode_CHG].pFunc)
            {
                g_stGfxGPIrq[enGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_3DMode_CHG].pFunc(
                    g_stGfxGPIrq[enGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_3DMode_CHG].pParam0,
                    (HI_VOID*)&g_stGfxGPDevice[enGpHalId].enTriDimMode);
            }
        }
    }
}
#endif


/***************************************************************************************
* func          : OPTM_ALG_Init
* description   : CNcomment: 算法初始化 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_ALG_Init(OPTM_GFX_GP_E enGPId)
{
#ifndef CONFIG_GFX_PQ
    OPTM_ALG_GDTI_RTL_PARA_S stDtiRtlPara;

    HI_GFX_Memset(&stDtiRtlPara, 0, sizeof(stDtiRtlPara));

    OPTM_ALG_GDtiInit(HI_NULL, &stDtiRtlPara);

    OPTM_VDP_GP_SetTiHpCoef(enGPId, VDP_TI_MODE_CHM, (HI_S32 *)stDtiRtlPara.s32CTIHPTmp);
    OPTM_VDP_GP_SetTiHpCoef(enGPId, VDP_TI_MODE_LUM, (HI_S32 *)stDtiRtlPara.s32LTIHPTmp);

    OPTM_VDP_GP_SetTiGainRatio(enGPId, VDP_TI_MODE_CHM, (HI_S32)stDtiRtlPara.s16CTICompsatRatio);
    OPTM_VDP_GP_SetTiGainRatio(enGPId, VDP_TI_MODE_LUM, (HI_S32)stDtiRtlPara.s16LTICompsatRatio);

    OPTM_VDP_GP_SetTiCoringThd(enGPId, VDP_TI_MODE_CHM, (HI_U32)stDtiRtlPara.u16CTICoringThrsh);
    OPTM_VDP_GP_SetTiCoringThd(enGPId, VDP_TI_MODE_LUM, (HI_U32)stDtiRtlPara.u16LTICoringThrsh);

    OPTM_VDP_GP_SetTiSwingThd(enGPId, VDP_TI_MODE_CHM, (HI_U32)stDtiRtlPara.u16CTIOverSwingThrsh, (HI_U32)stDtiRtlPara.u16CTIUnderSwingThrsh);
    OPTM_VDP_GP_SetTiSwingThd(enGPId, VDP_TI_MODE_LUM, (HI_U32)stDtiRtlPara.u16LTIOverSwingThrsh, (HI_U32)stDtiRtlPara.u16LTIUnderSwingThrsh);

    OPTM_VDP_GP_SetTiMixRatio(enGPId, VDP_TI_MODE_CHM, (HI_U32)stDtiRtlPara.u8CTIMixingRatio);
    OPTM_VDP_GP_SetTiMixRatio(enGPId, VDP_TI_MODE_LUM, (HI_U32)stDtiRtlPara.u8LTIMixingRatio);

    OPTM_VDP_GP_SetTiHfThd(enGPId, VDP_TI_MODE_LUM, (HI_U32 *)stDtiRtlPara.u32LTIHFreqThrsh);
    OPTM_VDP_GP_SetTiGainCoef(enGPId, VDP_TI_MODE_LUM, (HI_U32 *)stDtiRtlPara.u32LTICompsatMuti);
#endif
    return;
}


/***************************************************************************************
* func          : OPTM_GPOpen
* description   : CNcomment: 打开GP设备并设置ZME系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GPOpen(OPTM_GFX_GP_E enGPId)
{


    HI_U32 i                = 0;
    OPTM_VDP_BKG_S  stBkg   = {0};
    HI_U32 u32InitLayerID   = 0;
    HI_U32 u32MaxLayerCount = 0;


#if !defined(HI_BUILD_IN_BOOT) && defined(HI_PQ_V1_0)
    /** CV200used, 98M and hifone not used **/
    PQ_EXPORT_FUNC_S*   pstPqFuncs = HI_NULL;
    PQ_PARAM_S*         pstPqParam = HI_NULL;
    HI_S32 s32Ret                  = HI_FAILURE;
#endif

    if (g_stGfxGPDevice[enGPId].bOpen){
        return HI_SUCCESS;
    }

#ifndef HI_BUILD_IN_BOOT
    if (HI_NULL == ps_DispExportFuncs){
        if(HI_SUCCESS != OPTM_Aapt_Module_GetFunction(HI_ID_DISP, (HI_VOID**)&ps_DispExportFuncs)){
            HIFB_ERROR("Fail to get disp export functions!\n");
            return HI_FAILURE;
        }
    }
    if(OPTM_GFX_GP_0 == enGPId){
            g_stGfxGPDevice[enGPId].queue = create_workqueue(HIFB_WORK_QUEUE);
            if (HI_NULL != g_stGfxGPDevice[enGPId].queue){
                INIT_WORK(&g_stGfxGPDevice[enGPId].stOpenSlvWork.work,   HIFB_WBC_StartWorkQueue);
                INIT_WORK(&g_stGfxGPDevice[enGPId].st3DModeChgWork.work, OPTM_3DMode_Callback);
                HIFB_INFO("create_workqueue success.\n");
            }
    }
#endif

    HI_GFX_Memset(&g_stGfxGPIrq[enGPId], 0, sizeof(OPTM_GP_IRQ_S));

#if !defined(HI_BUILD_IN_BOOT) && defined(HI_PQ_V1_0)
    /** CV200used, 98M and hifone not used **/
    g_stGfxGPDevice[enGPId].u32ZmeDeflicker = 0;
    /**
     ** Get Pq param
     **/
    if (HI_SUCCESS != HI_DRV_MODULE_GetFunction(HI_ID_PQ, (HI_VOID**)&pstPqFuncs)){
        HIFB_WARNING("Get PQ_EXPORT_FUNC_S failed\r\n");
    }else{
        if (NULL == pstPqFuncs){
            HIFB_WARNING("Get PQ_EXPORT_FUNC_S failed\r\n");
        }else{
            s32Ret = pstPqFuncs->pfnPQ_GetPqParam(&pstPqParam);
            if (HI_SUCCESS == s32Ret){
                g_stGfxGPDevice[enGPId].u32ZmeDeflicker =
                                (OPTM_GFX_GP_0 == enGPId)?
                                pstPqParam ->stPQCoef.stGfxCoef.u32HdCtrlEn : pstPqParam ->stPQCoef.stGfxCoef.u32SdCtrlEn;
            }
        }
    }
#else
   g_stGfxGPDevice[enGPId].u32ZmeDeflicker = (OPTM_GFX_GP_0 == enGPId)?0:1;
#endif

    g_stGfxGPDevice[enGPId].u32Alpha          = 0xff;
    g_stGfxGPDevice[enGPId].enReadMode        = VDP_RMODE_PROGRESSIVE;
    g_stGfxGPDevice[enGPId].stBkg             = stBkg;
#ifndef CONFIG_GFX_PQ
    g_stGfxGPDevice[enGPId].enInputCsc        = OPTM_CS_BT709_RGB_FULL;
    g_stGfxGPDevice[enGPId].enOutputCsc       = OPTM_CS_UNKNOWN;
#else
    g_stGfxGPDevice[enGPId].enInputCsc        = PQ_GFX_CS_BT709_RGB_FULL;
    g_stGfxGPDevice[enGPId].enOutputCsc       = PQ_GFX_CS_BUTT;
#endif
    g_stGfxGPDevice[enGPId].bBGRState         = HI_FALSE;
    g_stGfxGPDevice[enGPId].bGpClose          = HI_FALSE;
    g_stGfxGPDevice[enGPId].bRecoveryInNextVT = HI_TRUE;
    g_stGfxGPDevice[enGPId].bDispInitial      = HI_FALSE;

    /**
     ** 0:HIFB_LAYER_HD_0;  1:HIFB_LAYER_HD_1;  2:HIFB_LAYER_HD_2;  3:HIFB_LAYER_HD_3
     ** 0:HIFB_LAYER_SD_0;  1:HIFB_LAYER_SD_1
     **/
    if (OPTM_GFX_GP_0 == enGPId){
        g_stGfxGPDevice[enGPId].enMixg       = VDP_CBM_MIXG0;
        g_stGfxGPDevice[enGPId].enGpHalId    = OPTM_VDP_LAYER_GP0;
        g_stGfxGPDevice[enGPId].enDispCh     = OPTM_DISPCHANNEL_1;
        u32InitLayerID                       = (HI_U32)HIFB_LAYER_HD_0;
        u32MaxLayerCount                     = (HI_U32)(OPTM_GP0_GFX_COUNT + u32InitLayerID - 1);
    }else if (OPTM_GFX_GP_1 == enGPId){
        g_stGfxGPDevice[enGPId].enMixg       = VDP_CBM_MIXG1;
        g_stGfxGPDevice[enGPId].enGpHalId    = OPTM_VDP_LAYER_GP1;
        g_stGfxGPDevice[enGPId].enDispCh     = OPTM_DISPCHANNEL_0;
        u32InitLayerID                       = (HI_U32)HIFB_LAYER_SD_0;
        u32MaxLayerCount                     = (HI_U32)(OPTM_GP1_GFX_COUNT + u32InitLayerID - 1);
    }else{
        return HI_SUCCESS;
    }

    OPTM_VDP_GP_SetLayerGalpha(enGPId, g_stGfxGPDevice[enGPId].u32Alpha);
    OPTM_VDP_GP_SetReadMode   (enGPId, g_stGfxGPDevice[enGPId].enReadMode);
    OPTM_VDP_CBM_SetMixerBkg  (g_stGfxGPDevice[enGPId].enMixg, g_stGfxGPDevice[enGPId].stBkg);


    g_stGfxGPDevice[enGPId].u32Prior = 0x0;
    for(i = u32InitLayerID; i <= u32MaxLayerCount; i++){
        /** 98CV200缺了G2图层，所以要调整Z序，第三个位置为G3**/
        g_stGfxDevice[i].u32ZOrder = i;
        OPTM_VDP_CBM_SetMixerPrio(g_stGfxGPDevice[enGPId].enMixg, i, g_stGfxDevice[i].u32ZOrder);
        g_stGfxDevice[i].enGfxHalId       = OPTM_GetGfxHalId(i);
        g_stGfxGPDevice[enGPId].u32Prior |= ((i+1)<<(i*4));
    }

    OPTM_ALG_Init(enGPId);

    g_stGfxGPDevice[enGPId].bOpen = HI_TRUE;

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : OPTM_GPClose
* description   : CNcomment: 关闭GP设备 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 OPTM_GPClose(OPTM_GFX_GP_E enGPId)
{

    if (HI_FALSE == g_stGfxGPDevice[enGPId].bOpen){
        return HI_SUCCESS;
    }

    g_enOptmGfxWorkMode = HIFB_GFX_MODE_NORMAL;

#ifndef HI_BUILD_IN_BOOT
    OPTM_SetCallbackToDisp(enGPId, (IntCallBack)DRV_HIFB_ADP_UpDataCallBack,   HI_DRV_DISP_C_INTPOS_90_PERCENT,  HI_FALSE);
    OPTM_SetCallbackToDisp(enGPId, (IntCallBack)DRV_HIFB_ADP_FrameEndCallBack, HI_DRV_DISP_C_INTPOS_100_PERCENT, HI_FALSE);
    if (g_stGfxGPDevice[enGPId].queue){
        destroy_workqueue(g_stGfxGPDevice[enGPId].queue);
        g_stGfxGPDevice[enGPId].queue = HI_NULL;
    }
#endif

    g_stGfxGPDevice[enGPId].bOpen            = HI_FALSE;
    g_stGfxGPDevice[enGPId].bDispInitial     = HI_FALSE;
    g_stGfxGPDevice[enGPId].bNeedExtractLine = HI_FALSE;
    g_stGfxGPDevice[enGPId].bMaskFlag        = HI_FALSE;

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GfxSetLayerReadMode
* description   : CNcomment: 设置读取数据模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerReadMode(HIFB_LAYER_ID_E enLayerId, OPTM_VDP_DATA_RMODE_E enReadMode)
{
    OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;

    OPTM_VDP_GFX_SetReadMode(g_stGfxDevice[enLayerId].enGfxHalId, enReadMode);
    OPTM_VDP_GP_SetReadMode (g_stGfxGPDevice[enGPId].enGpHalId,   enReadMode);

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GfxInitLayer
* description   : CNcomment: 初始化图层信息 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GfxInitLayer(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_BKG_S stBkg = {0};

    init_waitqueue_head(&(g_stGfxDevice[enLayerId].vblEvent));

    g_stGfxDevice[enLayerId].stBkg        = stBkg;
    g_stGfxDevice[enLayerId].enGfxHalId   = OPTM_GetGfxHalId(enLayerId);
    g_stGfxDevice[enLayerId].CscState     = OPTM_CSC_SET_PARA_RGB;
    g_stGfxDevice[enLayerId].enBitExtend  = VDP_GFX_BITEXTEND_3RD;
    g_stGfxDevice[enLayerId].enReadMode   = VDP_RMODE_PROGRESSIVE;
    g_stGfxDevice[enLayerId].enUpDateMode = VDP_RMODE_PROGRESSIVE;
#ifdef CONFIG_HIFB_GFX3_TO_GFX5
    g_stGfxDevice[enLayerId].enGPId       = (g_stGfxDevice[enLayerId].enGfxHalId > OPTM_VDP_LAYER_GFX2) ? OPTM_GFX_GP_1 : OPTM_GFX_GP_0;
#else
    g_stGfxDevice[enLayerId].enGPId       = (g_stGfxDevice[enLayerId].enGfxHalId > OPTM_VDP_LAYER_GFX3) ? OPTM_GFX_GP_1 : OPTM_GFX_GP_0;
#endif

    OPTM_VDP_GFX_SetNoSecFlag  (g_stGfxDevice[enLayerId].enGfxHalId, HI_TRUE);
    OPTM_VDP_GFX_SetDcmpEnable (g_stGfxDevice[enLayerId].enGfxHalId, HI_FALSE);
    OPTM_VDP_GFX_SetLayerBkg   (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].stBkg);
    OPTM_VDP_GFX_SetBitExtend  (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enBitExtend);
    OPTM_VDP_GFX_SetReadMode   (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enReadMode);
    OPTM_VDP_GFX_SetUpdMode    (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enUpDateMode);

#ifdef CONFIG_HIFB_GFX3_TO_GFX5
    if (OPTM_VDP_LAYER_GFX3 == g_stGfxDevice[enLayerId].enGfxHalId){
        /** 支持双显，绑定到DHD1上 **/
        OPTM_VDP_SetLayerConnect(OPTM_VDP_CONNECT_G3_DHD1);
        OPTM_VDP_OpenGFX3(HI_TRUE);
    }
#endif
    OPTM_VDP_GFX_SetRegUp   (g_stGfxDevice[enLayerId].enGfxHalId);
}


/***************************************************************************************
* func          : OPTM_GFX_ReleaseClutBuf
* description   : CNcomment: 释放调色板内存 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_ReleaseClutBuf(HIFB_LAYER_ID_E enLayerId)
{
#ifndef HI_BUILD_IN_BOOT
    if (g_stGfxCap[enLayerId].bHasCmapReg != HI_FALSE){
       /*  release CLUT TABLE buffer */
       if (g_stGfxDevice[enLayerId].stCluptTable.pu8StartVirAddr != 0){
            OPTM_UnmapAndRelease(&(g_stGfxDevice[enLayerId].stCluptTable));
            g_stGfxDevice[enLayerId].stCluptTable.pu8StartVirAddr  = NULL;
            g_stGfxDevice[enLayerId].stCluptTable.u32StartPhyAddr  = 0;
            g_stGfxDevice[enLayerId].stCluptTable.u32StartSmmuAddr = 0;
            g_stGfxDevice[enLayerId].stCluptTable.u32Size          = 0;
            g_stGfxDevice[enLayerId].stCluptTable.bSmmu            = HI_FALSE;
       }
    }
#endif
    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GPSetCsc
* description   : CNcomment: 设置GP CSC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GPSetCsc(OPTM_GFX_GP_E enGfxGpId, OPTM_GFX_CSC_PARA_S *pstCscPara, HI_BOOL bIsBGRIn)
{
    OPTM_ALG_CSC_DRV_PARA_S stCscDrvPara;
    OPTM_ALG_CSC_RTL_PARA_S stCscRtlPara;
    OPTM_VDP_CSC_COEF_S     stVDPCscCoef;
    OPTM_VDP_CSC_DC_COEF_S  stVDPCscDcCoef;

#ifndef HI_PQ_V1_0
#ifndef HI_BUILD_IN_BOOT
    HI_PQ_PICTURE_SETTING_S stPictureSetting = {50, 50, 50, 50};
#else
    HI_U32 enChan;
    HI_U32 u32Bright  = 50;  /** 亮度   **/
    HI_U32 u32Contrst = 50;  /** 对比度 **/
    HI_U32 u32Hue     = 50;  /** 色调   **/
    HI_U32 u32Satur   = 50;  /** 饱和度 **/
#endif

#ifndef HI_BUILD_IN_BOOT
    OPTM_GFX_CSC_PARA_S stCscPara;
    if(enGfxGpId == OPTM_GFX_GP_0){
        gs_pstPqFuncs->pfnPQ_GetHDPictureSetting(&stPictureSetting);
    }else{
        gs_pstPqFuncs->pfnPQ_GetSDPictureSetting(&stPictureSetting);
    }

    stCscPara.u32Bright     = stCscDrvPara.u32Bright  = stPictureSetting.u16Brightness;
    stCscPara.u32Contrast   = stCscDrvPara.u32Contrst = stPictureSetting.u16Contrast;
    stCscPara.u32Hue        = stCscDrvPara.u32Hue     = stPictureSetting.u16Hue;
    stCscPara.u32Saturation = stCscDrvPara.u32Satur   = stPictureSetting.u16Saturation;

    stCscPara.u32Kb = stCscDrvPara.u32Kb = 50;
    stCscPara.u32Kg = stCscDrvPara.u32Kg = 50;
    stCscPara.u32Kr = stCscDrvPara.u32Kr = 50;

    if(memcmp(&g_stGfxGPDevice[enGfxGpId].stCscPara, &stCscPara, sizeof(OPTM_GFX_CSC_PARA_S)) != 0)
    {
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Bright          = stCscDrvPara.u32Bright;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Contrast        = stCscDrvPara.u32Contrst;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Hue             = stCscDrvPara.u32Hue;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Kb              = stCscDrvPara.u32Kb;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Kg              = stCscDrvPara.u32Kg;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Kr              = stCscDrvPara.u32Kr;
        g_stGfxGPDevice[enGfxGpId].stCscPara.u32Saturation      = stCscDrvPara.u32Satur;
        HIFB_WBC2_Reset(HI_TRUE);
    }

#else

    if(enGfxGpId == OPTM_GFX_GP_0){
        enChan = HI_UNF_DISPLAY1;
    }else{
        enChan = HI_UNF_DISPLAY0;
    }

    HI_PQ_GetBrightness   (enChan, &u32Bright);
    HI_PQ_GetContrast     (enChan, &u32Contrst);
    HI_PQ_GetHue          (enChan, &u32Hue);
    HI_PQ_GetSaturation   (enChan, &u32Satur);

    stCscDrvPara.u32Bright  = u32Bright;
    stCscDrvPara.u32Contrst = u32Contrst;
    stCscDrvPara.u32Hue     = u32Hue;
    stCscDrvPara.u32Satur   = u32Satur;
    stCscDrvPara.u32Kb      = 50;
    stCscDrvPara.u32Kg      = 50;
    stCscDrvPara.u32Kr      = 50;

#endif

#endif //HI_PQ_V2_0  HI_PQ_V3_0

    g_stGfxGPDevice[enGfxGpId].enInputCsc  = OPTM_CS_BT709_RGB_FULL;
    /**
     **use logo para, do not set csc para again
     **使用开机logo的参数，不需要重新配置寄存器
     **/
    OPTM_CheckGPMask_BYGPID(enGfxGpId);

    stCscDrvPara.eInputCS   = g_stGfxGPDevice[enGfxGpId].enInputCsc;
    stCscDrvPara.eOutputCS  = g_stGfxGPDevice[enGfxGpId].enOutputCsc;
    stCscDrvPara.bIsBGRIn   = bIsBGRIn;

#ifdef HI_PQ_V1_0
    stCscDrvPara.u32Bright  = pstCscPara->u32Bright;
    stCscDrvPara.u32Contrst = pstCscPara->u32Contrast;
    stCscDrvPara.u32Hue     = pstCscPara->u32Hue;
    stCscDrvPara.u32Kb      = pstCscPara->u32Kb;
    stCscDrvPara.u32Kg      = pstCscPara->u32Kg;
    stCscDrvPara.u32Kr      = pstCscPara->u32Kr;
    stCscDrvPara.u32Satur   = pstCscPara->u32Saturation;

    HIFB_INFO("<<<<<gpid %d csc in %d, out%d, BGR %d.\n", enGfxGpId, stCscDrvPara.eInputCS, stCscDrvPara.eOutputCS, stCscDrvPara.bIsBGRIn);
    HIFB_INFO("parameter %d,%d,%d,%d,%d,%d,%d\n>>>", pstCscPara->u32Bright, pstCscPara->u32Contrast, pstCscPara->u32Hue,
              pstCscPara->u32Kb, pstCscPara->u32Kg, pstCscPara->u32Kr, pstCscPara->u32Saturation);
#endif //HI_PQ_V1_0

    OPTM_ALG_CscCoefSet(&stCscDrvPara, &stCscRtlPara);

    stVDPCscCoef.csc_coef00 = stCscRtlPara.s32CscCoef_00;
    stVDPCscCoef.csc_coef01 = stCscRtlPara.s32CscCoef_01;
    stVDPCscCoef.csc_coef02 = stCscRtlPara.s32CscCoef_02;
    stVDPCscCoef.csc_coef10 = stCscRtlPara.s32CscCoef_10;
    stVDPCscCoef.csc_coef11 = stCscRtlPara.s32CscCoef_11;
    stVDPCscCoef.csc_coef12 = stCscRtlPara.s32CscCoef_12;
    stVDPCscCoef.csc_coef20 = stCscRtlPara.s32CscCoef_20;
    stVDPCscCoef.csc_coef21 = stCscRtlPara.s32CscCoef_21;
    stVDPCscCoef.csc_coef22 = stCscRtlPara.s32CscCoef_22;

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    stVDPCscCoef.rgb2yuv_coef00 = stCscRtlPara.s32Rgb2YuvCoef_00;
    stVDPCscCoef.rgb2yuv_coef01 = stCscRtlPara.s32Rgb2YuvCoef_01;
    stVDPCscCoef.rgb2yuv_coef02 = stCscRtlPara.s32Rgb2YuvCoef_02;
    stVDPCscCoef.rgb2yuv_coef10 = stCscRtlPara.s32Rgb2YuvCoef_10;
    stVDPCscCoef.rgb2yuv_coef11 = stCscRtlPara.s32Rgb2YuvCoef_11;
    stVDPCscCoef.rgb2yuv_coef12 = stCscRtlPara.s32Rgb2YuvCoef_12;
    stVDPCscCoef.rgb2yuv_coef20 = stCscRtlPara.s32Rgb2YuvCoef_20;
    stVDPCscCoef.rgb2yuv_coef21 = stCscRtlPara.s32Rgb2YuvCoef_21;
    stVDPCscCoef.rgb2yuv_coef22 = stCscRtlPara.s32Rgb2YuvCoef_22;
#endif

    stVDPCscDcCoef.csc_in_dc0 = stCscRtlPara.s32CscDcIn_0;
    stVDPCscDcCoef.csc_in_dc1 = stCscRtlPara.s32CscDcIn_1;
    stVDPCscDcCoef.csc_in_dc2 = stCscRtlPara.s32CscDcIn_2;

    stVDPCscDcCoef.csc_out_dc0 = stCscRtlPara.s32CscDcOut_0;
    stVDPCscDcCoef.csc_out_dc1 = stCscRtlPara.s32CscDcOut_1;
    stVDPCscDcCoef.csc_out_dc2 = stCscRtlPara.s32CscDcOut_2;

#ifdef CONFIG_HIFB_GP0CSC_USE_HDRMOD
    stVDPCscDcCoef.rgb2yuv_out_dc0 = stCscRtlPara.s32Rgb2YuvOut_0;
    stVDPCscDcCoef.rgb2yuv_out_dc1 = stCscRtlPara.s32Rgb2YuvOut_1;
    stVDPCscDcCoef.rgb2yuv_out_dc2 = stCscRtlPara.s32Rgb2YuvOut_2;

    OPTM_VDP_GP_SetRgb2YuvScale2p  (g_stGfxGPDevice[enGfxGpId].enGpHalId,  stCscDrvPara.u32Rgb2YuvScale2p);
    OPTM_VDP_GP_SetRgb2YuvMin      (g_stGfxGPDevice[enGfxGpId].enGpHalId,  stCscDrvPara.u32Rgb2YuvMin);
    OPTM_VDP_GP_SetRgb2YuvMax      (g_stGfxGPDevice[enGfxGpId].enGpHalId,  stCscDrvPara.u32Rgb2YuvMax);
#endif

    OPTM_VDP_GP_SetCscCoef         (g_stGfxGPDevice[enGfxGpId].enGpHalId,  stVDPCscCoef);
    OPTM_VDP_GP_SetCscDcCoef       (g_stGfxGPDevice[enGfxGpId].enGpHalId,  stVDPCscDcCoef);
    OPTM_VDP_GP_SetCscEnable       (g_stGfxGPDevice[enGfxGpId].enGpHalId,  HI_TRUE);

    return HI_SUCCESS;

}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GfxSetCallback
* description   : CNcomment: set call back function
                             设置中断响应函数,在对应的中断中调用该对应的赋值函数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetCallback(HIFB_LAYER_ID_E enLayerId, IntCallBack pCallBack, HIFB_CALLBACK_TPYE_E eCallbackType)
{

    HI_U32         u32GfxIndex = 0;
    OPTM_GFX_GP_E  enGPId      = OPTM_GFX_GP_0;

    if (eCallbackType >= HIFB_CALLBACK_TYPE_BUTT){
        HIFB_ERROR("Fail to set callback func!\n");
        return HI_FAILURE;
    }

    enGPId      = g_stGfxDevice[enLayerId].enGPId;
    u32GfxIndex = (enLayerId > HIFB_LAYER_HD_3) ? (enLayerId - HIFB_LAYER_HD_3 - 1) : enLayerId;

    if(u32GfxIndex >= OPTM_GP_SUPPORT_MAXLAYERNUM){
        HIFB_ERROR("Fail to set callback func!\n");
        return HI_FAILURE;
    }

    gs_CallBackLayerId[enLayerId] = enLayerId;
    if(HI_NULL != pCallBack){
        g_stGfxGPIrq[enGPId].stGfxCallBack[u32GfxIndex].stGfxIrq[eCallbackType].pFunc     = pCallBack;
        g_stGfxGPIrq[enGPId].stGfxCallBack[u32GfxIndex].stGfxIrq[eCallbackType].pParam0   = (HI_VOID*)&gs_CallBackLayerId[enLayerId];
        g_stGfxGPIrq[enGPId].stGfxCallBack[u32GfxIndex].u32CTypeFlag |= eCallbackType;
    }else{
        g_stGfxGPIrq[enGPId].stGfxCallBack[u32GfxIndex].u32CTypeFlag &= ~((HI_U32)eCallbackType);
        g_stGfxGPIrq[enGPId].stGfxCallBack[u32GfxIndex].stGfxIrq[eCallbackType].pFunc     = HI_NULL;
    }
    return HI_SUCCESS;

}
/***************************************************************************************
* func          : OPTM_CheckGfxCallbackReg
* description   : CNcomment: check whether call back function have register
                             判断相应的回调函数是否被注册 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_CheckGfxCallbackReg(OPTM_GFX_GP_E enGPId, HIFB_CALLBACK_TPYE_E eCallbackType)
{
    HI_U32 u32LayerCount = (enGPId == OPTM_GFX_GP_0) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    HI_U32 u32LayerId = 0;
    for(u32LayerId = 0; u32LayerId < u32LayerCount; u32LayerId++){
        if(g_stGfxGPIrq[enGPId].stGfxCallBack[u32LayerId].u32CTypeFlag & eCallbackType){
            if (g_stGfxGPIrq[enGPId].stGfxCallBack[u32LayerId].stGfxIrq[eCallbackType].pFunc){
                return HI_SUCCESS;
            }
        }
    }
    return HI_FAILURE;
}
#endif


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GetScreenRectFromDispInfo
* description   : CNcomment: 从disp中获取screen rect CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GetScreenRectFromDispInfo(const HI_RECT_S *tmp_virtscreen,      \
                                      const OPTM_GFX_OFFSET_S *stOffsetInfo,\
                                      const HI_RECT_S *stFmtResolution,     \
                                      const HI_RECT_S *stPixelFmtResolution,\
                                      HI_RECT_S *stScreenRect)
{

    OPTM_GFX_OFFSET_S tmp_offsetInfo = *stOffsetInfo;

    /**
     **stPixelFmtResolution这个仅在S40V2且PAL制下有效，其它情况下和stFmtResolution相等
     **虚拟屏幕相当于GP的输入，GP的输出由stFmtResolution - stOffsetInfo所得
     **/
    if(tmp_virtscreen->s32Width == 0 || tmp_virtscreen->s32Height == 0){
        return HI_FAILURE;
    }

    if( (stFmtResolution->s32Width * 2) == stPixelFmtResolution->s32Width){
        tmp_offsetInfo.u32Left  *= 2;
        tmp_offsetInfo.u32Right *= 2;
    }

    stScreenRect->s32X      = tmp_offsetInfo.u32Left;
    stScreenRect->s32Y      = tmp_offsetInfo.u32Top;
    stScreenRect->s32Width  = (stPixelFmtResolution->s32Width - tmp_offsetInfo.u32Left - tmp_offsetInfo.u32Right);
    stScreenRect->s32Height = (stPixelFmtResolution->s32Height - tmp_offsetInfo.u32Top - tmp_offsetInfo.u32Bottom);
    stScreenRect->s32X      = OPTM_AlignUp(stScreenRect->s32X, 2);
    stScreenRect->s32Y      = OPTM_AlignUp(stScreenRect->s32Y, 2);
    stScreenRect->s32Width  = OPTM_AlignUp(stScreenRect->s32Width, 2);
    stScreenRect->s32Height = OPTM_AlignUp(stScreenRect->s32Height, 2);

    return HI_SUCCESS;

}
#endif



#ifndef HI_BUILD_IN_BOOT
HI_VOID DRV_HIFB_ADP_FrameEndCallBack(HI_VOID* u32Param0, HI_VOID* u32Param1)
{
#ifdef CFG_HIFB_FENCE_SUPPORT
    HI_U32 u32CTypeFlag = 0;
    HI_VOID* pLayerId   = NULL;
    HIFB_LAYER_ID_E enLayerId = HIFB_LAYER_HD_0;
#endif
    OPTM_GFX_GP_E  *pEnGpId   = (OPTM_GFX_GP_E *)u32Param0;

    HIFB_CHECK_WHETHER_PONITER_IS_NULL(u32Param0);
    HIFB_CHECK_WHETHER_PONITER_IS_NULL(u32Param1);
    HIFB_CHECK_GPID_SUPPORT(*pEnGpId);

    if (HI_FALSE == g_stGfxGPDevice[*pEnGpId].bOpen)
    {
        return;
    }

    if (OPTM_GFX_GP_0 != *pEnGpId)
    {
        return;
    }

    HIFB_WBC_FrameEndProcess(*pEnGpId);

#ifdef CFG_HIFB_FENCE_SUPPORT
    for (enLayerId = HIFB_LAYER_HD_0; enLayerId <= HIFB_LAYER_HD_3; enLayerId++)
    {
        pLayerId = g_stGfxGPIrq[*pEnGpId].stGfxCallBack[enLayerId].stGfxIrq[HIFB_CALLBACK_TYPE_FRAME_END].pParam0;
        u32CTypeFlag = g_stGfxGPIrq[*pEnGpId].stGfxCallBack[enLayerId].u32CTypeFlag;
        if ((u32CTypeFlag & HIFB_CALLBACK_TYPE_FRAME_END) && (NULL != g_stGfxGPIrq[*pEnGpId].stGfxCallBack[enLayerId].stGfxIrq[HIFB_CALLBACK_TYPE_FRAME_END].pFunc))
        {
           g_stGfxGPIrq[*pEnGpId].stGfxCallBack[enLayerId].stGfxIrq[HIFB_CALLBACK_TYPE_FRAME_END].pFunc(pLayerId,HI_NULL);
        }
    }
#endif

    return;
}

/***************************************************************************************
* func          : OPTM_DispInfoProcess
* description   : CNcomment: display信息处理 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_BOOL OPTM_DispInfoProcess(OPTM_VDP_LAYER_GP_E eGpId, HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo)
{
        HI_BOOL bDispInfoChange = HI_FALSE;
        HI_RECT_S stOutRect;

#ifdef HI_PQ_V1_0
        OPTM_GFX_CSC_PARA_S stTempCscPara;
        stTempCscPara.u32Bright     = pstDispInfo->stDispInfo.u32Bright;
        stTempCscPara.u32Contrast   = pstDispInfo->stDispInfo.u32Contrst;
        stTempCscPara.u32Hue        = pstDispInfo->stDispInfo.u32Hue;
        stTempCscPara.u32Kb         = pstDispInfo->stDispInfo.u32Kb;
        stTempCscPara.u32Kg         = pstDispInfo->stDispInfo.u32Kg;
        stTempCscPara.u32Kr         = pstDispInfo->stDispInfo.u32Kr;
        stTempCscPara.u32Saturation = pstDispInfo->stDispInfo.u32Satur;
        if (memcmp(&g_stGfxGPDevice[eGpId].stCscPara, &stTempCscPara, sizeof(OPTM_GFX_CSC_PARA_S)) != 0)
        {
            HI_GFX_Memcpy(&g_stGfxGPDevice[eGpId].stCscPara, &stTempCscPara, sizeof(OPTM_GFX_CSC_PARA_S));
            bDispInfoChange = HI_TRUE;
        }

        HIFB_INFO("====u32Bright %d,u32Contrast %d,u32Hue %d,u32Kb %d,u32Kg %d,u32Kr %d,u32Saturation %d !===\n"
                ,pstDispInfo->stDispInfo.u32Bright,pstDispInfo->stDispInfo.u32Contrst,pstDispInfo->stDispInfo.u32Hue
                ,pstDispInfo->stDispInfo.u32Kb,pstDispInfo->stDispInfo.u32Kg,pstDispInfo->stDispInfo.u32Kr
                ,pstDispInfo->stDispInfo.u32Satur);

#endif

        /**
         **重新设置CSC系数
         **/
        OPTM_GPSetCsc(eGpId, &g_stGfxGPDevice[eGpId].stCscPara,g_stGfxGPDevice[eGpId].bBGRState);

        if(g_stGfxGPDevice[eGpId].stInRect.s32Width != pstDispInfo->stDispInfo.stVirtaulScreen.s32Width \
        || g_stGfxGPDevice[eGpId].stInRect.s32Height != pstDispInfo->stDispInfo.stVirtaulScreen.s32Height)
        {
            bDispInfoChange = HI_TRUE;
        }
        HI_GFX_Memcpy(&g_stGfxGPDevice[eGpId].stInRect, &pstDispInfo->stDispInfo.stVirtaulScreen, sizeof(HI_RECT_S));

        OPTM_GetScreenRectFromDispInfo(&pstDispInfo->stDispInfo.stVirtaulScreen, (OPTM_GFX_OFFSET_S *)&pstDispInfo->stDispInfo.stOffsetInfo,
                        &pstDispInfo->stDispInfo.stFmtResolution,&pstDispInfo->stDispInfo.stPixelFmtResolution,
                        &stOutRect);

        if (g_stGfxGPDevice[eGpId].stOutRect.s32Width != stOutRect.s32Width ||
            g_stGfxGPDevice[eGpId].stOutRect.s32Height != stOutRect.s32Height ||
            g_stGfxGPDevice[eGpId].stOutRect.s32X != stOutRect.s32X ||
            g_stGfxGPDevice[eGpId].stOutRect.s32Y != stOutRect.s32Y ){
            bDispInfoChange = HI_TRUE;
        }

        HI_GFX_Memcpy(&g_stGfxGPDevice[eGpId].stOutRect, &stOutRect, sizeof(HI_RECT_S));

        return bDispInfoChange;

}

/***************************************************************************************
* func          : OPTM_SET_GPMask
* description   : CNcomment: 在90%中断处理中调用,设置GP掩码 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_SET_GPMask(OPTM_GFX_GP_E enGPId, HI_BOOL bFlag)
{

    HI_U32           i             = 0;
    HI_BOOL          bEnable       = HI_FALSE;
    HI_U32           u32LayerCount = 0;
    HIFB_LAYER_ID_E  enLayerId     = HIFB_LAYER_HD_0;
    OPTM_GFX_WBC_S   *pstWbc2      = &g_stGfxWbc2;

    if(!g_stGfxGPDevice[enGPId].bOpen){
        return HI_SUCCESS;
    }
    u32LayerCount = (OPTM_GFX_GP_0 == enGPId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    enLayerId     = (OPTM_GFX_GP_0 == enGPId) ? HIFB_LAYER_HD_0    : HIFB_LAYER_SD_0;

    g_stGfxGPDevice[enGPId].bMaskFlag = bFlag;

    if(IS_MASTER_GP((OPTM_GFX_GP_E)enGPId)){
        g_stGfxGPDevice[OPTM_SLAVER_GPID].bMaskFlag = bFlag;
    }

    if(IS_SLAVER_GP(enGPId)){
           bEnable = bFlag ? HI_FALSE : g_stGfxDevice[HIFB_LAYER_SD_0].bEnable;
        OPTM_VDP_GFX_SetLayerEnable (g_stGfxDevice[HIFB_LAYER_SD_0].enGfxHalId, bEnable);
        OPTM_VDP_WBC_SetEnable      (pstWbc2->enWbcHalId, bEnable);
    }else{
        for (i = 0; i < u32LayerCount;i++){
            if(!g_stGfxDevice[enLayerId+i].bOpened){
                continue;
            }
            /**
             ** before disable layer, we need to save data.
             ** when set layer mask true, we save operations of user in global variable.
             ** until layer mask become false, these operations will be setted to hardware.
             **/
            bEnable = bFlag ? HI_FALSE : g_stGfxDevice[enLayerId+i].bEnable;
            OPTM_VDP_GFX_SetLayerEnable(g_stGfxDevice[enLayerId+i].enGfxHalId, bEnable);
            g_stGfxDevice[HIFB_LAYER_SD_0].bEnable = bEnable;
        }
        if (IS_MASTER_GP((OPTM_GFX_GP_E)enGPId)){
            g_stGfxDevice[HIFB_LAYER_SD_0].bEnable = bFlag ? HI_FALSE : g_stGfxDevice[HIFB_LAYER_SD_0].bEnable;
            g_stGfxGPDevice[OPTM_SLAVER_GPID].unUpFlag.bits.IsEnable = 1;
        }
    }

    return HI_SUCCESS;
}


#ifndef HI_BUILD_IN_BOOT
static HI_VOID HIFB_ADP_GetResumeStatus(HIFB_LAYER_ID_E enLayerId, HI_BOOL *pbResume)
{
    if (enLayerId >= HIFB_LAYER_SD_0)
    {
       return;
    }
    *pbResume = g_stGfxGPDevice[0].bResume;
    g_stGfxGPDevice[0].bResume = HI_FALSE;

    return;
}
#endif

#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT)
static HI_VOID HIFB_ADP_SetDeCmpDdrInfo(HIFB_LAYER_ID_E enLayerId, HI_U32 ARHeadDdr,HI_U32 ARDataDdr,HI_U32 GBHeadDdr,HI_U32 GBDataDdr,HI_U32 DeCmpStride)
{
    DRV_HAL_SetDeCmpDdrInfo(enLayerId,ARHeadDdr,ARDataDdr,GBHeadDdr,GBDataDdr,DeCmpStride);
    return;
}

static HI_VOID HIFB_ADP_GetDeCmpStatus(HIFB_LAYER_ID_E enLayerId, DRV_HIFB_DECMPSTATUS_S *pstDeCmpStatus)
{
    DRV_HAL_GetDeCmpStatus(enLayerId, pstDeCmpStatus);
    return;
}

static HI_VOID OPTM_GFX_DECMP_Open(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_GFX_SetDcmpEnable(enLayerId,HI_TRUE);
}

static HI_VOID OPTM_GFX_DECMP_Close(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_GFX_SetDcmpEnable(enLayerId,HI_FALSE);
}
#endif



HI_VOID DRV_HIFB_ADP_UpDataCallBack(HI_VOID* u32Param0, HI_VOID* u32Param1)
{
    OPTM_GFX_GP_E *pEnGpId = (OPTM_GFX_GP_E *)u32Param0;
    HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo = (HI_DRV_DISP_CALLBACK_INFO_S *)u32Param1;

    HIFB_CHECK_WHETHER_PONITER_IS_NULL(u32Param0);
    HIFB_CHECK_WHETHER_PONITER_IS_NULL(u32Param1);
    HIFB_CHECK_GPID_SUPPORT(*pEnGpId);

    if (HI_FALSE == g_stGfxGPDevice[*pEnGpId].bOpen)
    {
        return;
    }


    if ((g_enOptmGfxWorkMode == HIFB_GFX_MODE_NORMAL) && (HI_TRUE == pstDispInfo->stDispInfo.bIsMaster) && (OPTM_GFX_GP_0 == *pEnGpId))
    {
        g_enOptmGfxWorkMode = HIFB_GFX_MODE_HD_WBC;
        if (NULL != g_stGfxGPDevice[*pEnGpId].queue)
        {
            g_stGfxGPDevice[*pEnGpId].stOpenSlvWork.u32Data = HIFB_LAYER_SD_0;
            queue_work(g_stGfxGPDevice[*pEnGpId].queue, &g_stGfxGPDevice[*pEnGpId].stOpenSlvWork.work);
        }
    }

    if ((HI_DRV_DISP_C_PREPARE_TO_PEND == pstDispInfo->eEventType) || (HI_DRV_DISP_C_PREPARE_CLOSE == pstDispInfo->eEventType))
    {
        return HIFB_ADP_ReClose(*pEnGpId);
    }

    if ((HI_DRV_DISP_C_RESUME == pstDispInfo->eEventType) || (HI_DRV_DISP_C_OPEN == pstDispInfo->eEventType))
    {
        return HIFB_ADP_ReOpen(*pEnGpId);
    }

    if (HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE == pstDispInfo->eEventType)
    {
        return HIFB_ADP_DispSetting(*pEnGpId,pstDispInfo);
    }

    if (HI_DRV_DISP_C_VT_INT == pstDispInfo->eEventType)
    {
         return HIFB_ADP_UpdataLayerInfo(u32Param0,u32Param1);
    }

    return;
}


static inline HI_VOID HIFB_ADP_ReOpen(OPTM_GFX_GP_E enGpId)
{
     if (HI_FALSE == g_stGfxGPDevice[enGpId].bDispInitial)
     {
         return;
     }

     if (HI_TRUE == g_stGfxGPDevice[enGpId].bGpClose)
     {
         g_stGfxGPDevice[enGpId].bGpClose = HI_FALSE;
         g_stGfxGPDevice[enGpId].bRecoveryInNextVT = HI_TRUE;
     }

     HIFB_WBC2_Reset(HI_FALSE);

     OPTM_SET_GPMask(enGpId, HI_FALSE);

     HIFB_INFO("======disp%d open======\n",enGpId);

     return;
}

static inline HI_VOID HIFB_ADP_ReClose(OPTM_GFX_GP_E enGpId)
{

     if (HI_FALSE == g_stGfxGPDevice[enGpId].bDispInitial)
     {
         return;
     }
     g_stGfxGPDevice[enGpId].bGpClose = HI_TRUE;


     OPTM_SET_GPMask(enGpId, HI_TRUE);

     HIFB_INFO("======disp%d close======\n",enGpId);

     return;
}


static inline HI_VOID HIFB_ADP_DispSetting(OPTM_GFX_GP_E enGpId, HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo)
{
     HI_BOOL bDispInfoChange = HI_FALSE;

     bDispInfoChange = OPTM_DispInfoProcess(enGpId, pstDispInfo);
     if (HI_FALSE == bDispInfoChange)
     {
         return;
     }

     OPTM_DispInfoUpdate(enGpId);

     if ((enGpId == OPTM_GFX_GP_1) || IS_MASTER_GP(enGpId))
     {
         HIFB_WBC2_Reset(HI_TRUE);
         g_stGfxGPDevice[OPTM_GFX_GP_1].unUpFlag.bits.IsNeedUpInRect = 1;
         g_stGfxWbc2.s32WbcCnt = 2;
     }

     return;
}


static inline HI_VOID HIFB_ADP_UpdataLayerInfo(HI_VOID* u32Param0, HI_VOID* u32Param1)
{
    OPTM_COLOR_SPACE_E       enGpCsc = OPTM_CS_BUTT;
    OPTM_GFX_GP_E *pEnGpId = (OPTM_GFX_GP_E *)u32Param0;
    HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo = (HI_DRV_DISP_CALLBACK_INFO_S *)u32Param1;

    if ((HI_TRUE == g_stGfxGPDevice[*pEnGpId].bGpClose) && (HI_TRUE == g_stGfxGPDevice[*pEnGpId].bDispInitial))
    {
        g_stGfxGPDevice[*pEnGpId].bRecoveryInNextVT = HI_TRUE;
        g_stGfxGPDevice[*pEnGpId].bGpClose          = HI_FALSE;
        OPTM_SET_GPMask(*pEnGpId, HI_FALSE);
    }

    if (HI_TRUE == g_stGfxGPDevice[*pEnGpId].bRecoveryInNextVT)
    {
        g_stGfxGPDevice[*pEnGpId].bInterface = pstDispInfo->stDispInfo.bInterlace;

        HI_GFX_Memcpy(&g_stGfxGPDevice[*pEnGpId].stInRect, &pstDispInfo->stDispInfo.stVirtaulScreen, sizeof(HI_RECT_S));

        OPTM_GetScreenRectFromDispInfo(\
                           &pstDispInfo->stDispInfo.stVirtaulScreen,                   \
                           (OPTM_GFX_OFFSET_S *)&pstDispInfo->stDispInfo.stOffsetInfo, \
                           &pstDispInfo->stDispInfo.stFmtResolution,                   \
                           &pstDispInfo->stDispInfo.stPixelFmtResolution,              \
                           &g_stGfxGPDevice[*pEnGpId].stOutRect);

        OPTM_GPRecovery(*pEnGpId);

        g_stGfxGPDevice[*pEnGpId].bRecoveryInNextVT = HI_FALSE;
        g_stGfxGPDevice[*pEnGpId].bDispInitial      = HI_TRUE;

    }

    enGpCsc = OPTM_AdaptCscTypeFromDisp(pstDispInfo->stDispInfo.eColorSpace);

#ifdef HI_PQ_V1_0
    if (g_stGfxGPDevice[*pEnGpId].enOutputCsc != enGpCsc)
    {
        g_stGfxGPDevice[*pEnGpId].enOutputCsc = enGpCsc;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Bright     = pstDispInfo->stDispInfo.u32Bright;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Contrast   = pstDispInfo->stDispInfo.u32Contrst;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Hue        = pstDispInfo->stDispInfo.u32Hue;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Kb         = pstDispInfo->stDispInfo.u32Kb;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Kg         = pstDispInfo->stDispInfo.u32Kg;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Kr         = pstDispInfo->stDispInfo.u32Kr;
        g_stGfxGPDevice[*pEnGpId].stCscPara.u32Saturation = pstDispInfo->stDispInfo.u32Satur;
        OPTM_GPSetCsc(*pEnGpId, &g_stGfxGPDevice[*pEnGpId].stCscPara, g_stGfxGPDevice[*pEnGpId].bBGRState);
    }
#else
    g_stGfxGPDevice[*pEnGpId].enOutputCsc = enGpCsc;
    OPTM_GPSetCsc(*pEnGpId, &g_stGfxGPDevice[*pEnGpId].stCscPara,g_stGfxGPDevice[*pEnGpId].bBGRState);
#endif

    if(IS_SLAVER_GP(*pEnGpId))
    {
        HIFB_WBC_Process(*pEnGpId);
    }
    else
    {
        OPTM_Distribute_Callback(u32Param0, u32Param1);
    }

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    OPTM_GFX_CMP_Process(u32Param0);
#endif

    return;
}


/***************************************************************************************
* func          : OPTM_GfxChn2DispChn
* description   : CNcomment: 获取display通道 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_DRV_DISPLAY_E OPTM_GfxChn2DispChn(OPTM_DISPCHANNEL_E enDispCh)
{
    if(OPTM_DISPCHANNEL_0 == enDispCh){
        return HI_DRV_DISPLAY_0;
    }else if (OPTM_DISPCHANNEL_1 == enDispCh){
        return HI_DRV_DISPLAY_1;
    }else{
        return HI_DRV_DISPLAY_BUTT;
    }
}
/***************************************************************************************
* func          : OPTM_SetCallbackToDisp
* description   : CNcomment: 向Display注册中断 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_SetCallbackToDisp(OPTM_GFX_GP_E enGPId, IntCallBack pCallBack, HI_DRV_DISP_CALLBACK_TYPE_E eType, HI_BOOL bFlag)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_DRV_DISPLAY_E            enDisp;
    HI_DRV_DISP_CALLBACK_S      stCallback;

    if (bFlag == g_stGfxGPIrq[enGPId].bRegistered[eType]){
        /** if register, not register again **/
        return HI_SUCCESS;
    }

    if (eType >= HI_DRV_DISP_C_TYPE_BUTT){
        HIFB_ERROR("Fail to set callback func!\n");
        return HI_FAILURE;
    }

    if(bFlag == g_stGfxGPIrq[enGPId].bRegistered[eType]){
        return HI_SUCCESS;
    }

    if (HI_NULL == pCallBack){
        HIFB_ERROR("Unable to handle the null func point!\n");
        return HI_FAILURE;
    }

    enDisp = OPTM_GfxChn2DispChn(g_stGfxGPDevice[enGPId].enDispCh);
    stCallback.hDst            = (HI_VOID*)(&g_stGfxGPDevice[enGPId].enGpHalId);
    stCallback.pfDISP_Callback = (HI_VOID*)pCallBack;

    if(bFlag){
        s32Ret = ps_DispExportFuncs->pfnDispRegCallback(enDisp,eType,&stCallback);
    }else{
        s32Ret = ps_DispExportFuncs->pfnDispUnRegCallback(enDisp,eType,&stCallback);
    }

    if (HI_SUCCESS == s32Ret){
        g_stGfxGPIrq[enGPId].bRegistered[eType] = bFlag;
    }

    return s32Ret;

}
#endif


/***************************************************************************************
* func          : OPTM_GfxOpenLayer
* description   : CNcomment: 打开图层 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxOpenLayer(HIFB_LAYER_ID_E enLayerId)
{

#ifndef HI_BUILD_IN_BOOT
    HI_S32 s32Ret = HI_SUCCESS;
#endif

    if (HI_TRUE != g_stGfxCap[enLayerId].bLayerSupported){
        HIFB_ERROR("Gfx%d was not supported!\n",enLayerId);
        return HI_FAILURE;
    }

    if (HI_TRUE == g_stGfxDevice[enLayerId].bOpened){
        HIFB_WARNING("info:Gfx%d was opened!\n",enLayerId);
        return HI_SUCCESS;
    }

    if (  (HIFB_GFX_MODE_HD_WBC == g_enOptmGfxWorkMode) && (HIFB_LAYER_SD_0 == enLayerId)){
        HIFB_WARNING("GFX work at wbc mode, gfx%d is working!\n", enLayerId);
        return HI_FAILURE;
    }

#ifndef HI_BUILD_IN_BOOT
    if (HI_NULL == gs_pstPqFuncs){
        if (HI_SUCCESS != HI_DRV_MODULE_GetFunction(HI_ID_PQ, (HI_VOID**)&gs_pstPqFuncs)){
            HIFB_WARNING("Get PQ_EXPORT_FUNC_S failed\r\n");
        }
    }
    if (HI_NULL == gs_pstPqFuncs){
        HIFB_ERROR("pq is not available!\n");
        return HI_FAILURE;
    }
#endif

    OPTM_GfxInitLayer(enLayerId);

#ifndef HI_BUILD_IN_BOOT
    s32Ret =
#else
    (HI_VOID)
#endif
    OPTM_GPOpen(g_stGfxDevice[enLayerId].enGPId);

#ifndef HI_BUILD_IN_BOOT
    if (HI_SUCCESS != s32Ret){
        return HI_FAILURE;
    }
#endif


#ifndef HI_BUILD_IN_BOOT
    s32Ret = OPTM_SetCallbackToDisp(g_stGfxDevice[enLayerId].enGPId, (IntCallBack)DRV_HIFB_ADP_UpDataCallBack, HI_DRV_DISP_C_INTPOS_90_PERCENT, HI_TRUE);
    if (HI_SUCCESS != s32Ret){
        HIFB_ERROR("Disp was not ready, open gfx%d failure!\n", enLayerId);
        return HI_FAILURE;
    }
    s32Ret = OPTM_SetCallbackToDisp(g_stGfxDevice[enLayerId].enGPId, (IntCallBack)DRV_HIFB_ADP_FrameEndCallBack, HI_DRV_DISP_C_INTPOS_100_PERCENT, HI_TRUE);
    if (HI_SUCCESS != s32Ret){
        HIFB_ERROR("fail to register FrameEndCallBack\n");
        return HI_FAILURE;
    }
#endif

    if (g_stGfxCap[enLayerId].bHasCmapReg != HI_FALSE)
    {
        HI_CHAR name[32] = {'\0'};
        snprintf(name, sizeof(name), "HIFB_Fb%d_Clut", enLayerId);
        if (OPTM_Adapt_AllocAndMap(name, NULL, OPTM_CMAP_SIZE, 0, &g_stGfxDevice[enLayerId].stCluptTable) != HI_SUCCESS){
            HIFB_ERROR("GFX Get clut buffer failed!\n");
            return HI_FAILURE;
        }
        OPTM_VDP_GFX_SetLutAddr(g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].stCluptTable.u32StartPhyAddr);
    }

    g_stGfxDevice[enLayerId].bOpened = HI_TRUE;

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_CheckGpState
* description   : CNcomment: 判断GP状态 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 OPTM_CheckGpState(OPTM_GFX_GP_E enGPId)
{
    HI_U32 i             = 0;
    HI_U32 u32LayerIdSta = 0;
    HI_U32 u32LayerIdEnd = 0;

    if(OPTM_GFX_GP_0 == enGPId){
        u32LayerIdSta = HIFB_LAYER_HD_0;
        u32LayerIdEnd = HIFB_LAYER_HD_3;
    }else if (OPTM_GFX_GP_1 == enGPId){
        u32LayerIdSta = HIFB_LAYER_SD_0;
        u32LayerIdEnd = HIFB_LAYER_SD_1;
    }else{
        return OPTM_DISABLE;
    }

    for (i = u32LayerIdSta; i <= u32LayerIdEnd; i++){
        if (g_stGfxDevice[i].bOpened){
            return OPTM_ENABLE;
        }
    }

    return OPTM_DISABLE;
}


/***************************************************************************************
* func          : OPTM_GfxCloseLayer
* description   : CNcomment: 关闭打开的图层 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxCloseLayer(HIFB_LAYER_ID_E enLayerId)
{

    OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;

    if(HI_FALSE == g_stGfxDevice[enLayerId].bOpened){
        return HI_SUCCESS;
    }

#ifdef CONFIG_HIFB_GFX3_TO_GFX5
    if (OPTM_VDP_LAYER_GFX3 == g_stGfxDevice[enLayerId].enGfxHalId){
        OPTM_VDP_OpenGFX3(HI_FALSE);
        OPTM_VDP_SetLayerConnect(OPTM_VDP_CONNECT_G3_DHD0);
    }
#endif

    OPTM_GfxSetEnable          (enLayerId, HI_FALSE);
    OPTM_VDP_GFX_SetNoSecFlag  (g_stGfxDevice[enLayerId].enGfxHalId, HI_TRUE);
    OPTM_VDP_GFX_SetRegUp      (g_stGfxDevice[enLayerId].enGfxHalId);
    OPTM_GFX_ReleaseClutBuf    (enLayerId);

    g_stGfxDevice[enLayerId].bExtractLine = HI_FALSE;
    g_stGfxDevice[enLayerId].bOpened      = HI_FALSE;

    if(!OPTM_CheckGpState(enGPId)){
        if (g_enOptmGfxWorkMode == HIFB_GFX_MODE_HD_WBC){
            HIFB_WBC_DinitLayer  (OPTM_SLAVER_LAYERID);
            OPTM_GPClose         (OPTM_SLAVER_GPID);
        }
        OPTM_GPClose(enGPId);
    }

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    OPTM_GFX_CMP_Close(enLayerId);
#endif

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GfxSetEnable
* description   : CNcomment: 设置图层使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetEnable(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable)
{
    OPTM_GFX_GP_E enGpId = g_stGfxDevice[enLayerId].enGPId;

    g_stGfxDevice[enLayerId].bEnable = bEnable;
    /**
     **看是是否还有开机logo，有开机logo，
     ** 过渡完之后下一次运行底下才生效
     **/
    OPTM_CheckGPMask_BYGPID(enGpId);

    OPTM_VDP_GFX_SetLayerEnable(g_stGfxDevice[enLayerId].enGfxHalId, bEnable);
    OPTM_VDP_GFX_SetRegUp      (g_stGfxDevice[enLayerId].enGfxHalId);

    if(IS_MASTER_GP(enGpId)){
        HIFB_WBC2_SetEnable(bEnable);
    }

    return HI_SUCCESS;

}

/***************************************************************************************
* func            : OPTM_GfxSetLayerAddr
* description    : CNcomment: 设置显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerAddr(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Addr)
{
    g_stGfxDevice[enLayerId].NoCmpBufAddr = u32Addr;

    OPTM_CheckGPMask_BYLayerID(enLayerId);

    OPTM_VDP_GFX_SetLayerAddrEX(g_stGfxDevice[enLayerId].enGfxHalId, u32Addr);

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectReady = HI_FALSE;
#endif

    return HI_SUCCESS;
}

/***************************************************************************************
* func            : OPTM_GfxGetLayerAddr
* description    : CNcomment: 获取显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetLayerAddr(HIFB_LAYER_ID_E enLayerId, HI_U32 *pu32Addr)
{
#ifndef HI_BUILD_IN_BOOT
    OPTM_VDP_GFX_GetLayerAddr(enLayerId,pu32Addr);
#endif
    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GfxSetLayerStride
* description   : CNcomment: 设置图层stride CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerStride(HIFB_LAYER_ID_E enLayerId, HI_U32 u32Stride)
{
    g_stGfxDevice[enLayerId].Stride = (HI_U16)u32Stride;

    OPTM_CheckGPMask_BYLayerID(enLayerId);

    if(g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].bNeedExtractLine && OPTM_CURSOR_LAYERID != enLayerId){
        /** 需要抽行,抽行只和性能有关系 **/
        OPTM_VDP_GFX_SetLayerStride(g_stGfxDevice[enLayerId].enGfxHalId, u32Stride*2);
    }else{
        OPTM_VDP_GFX_SetLayerStride(g_stGfxDevice[enLayerId].enGfxHalId, u32Stride);
    }

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    /* when set G0 stride we should change current mode from cmp mode to no cmp mode*/
    g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.u32CmpStride = u32Stride/2;
#endif

    return HI_SUCCESS;

}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_HalFmtTransferToPixerFmt
* description   : CNcomment: 由VDP像素格式转换成hifb像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HIFB_COLOR_FMT_E OPTM_HalFmtTransferToPixerFmt(OPTM_VDP_GFX_IFMT_E enDataFmt)
{
    HIFB_COLOR_FMT_E ePixFmt = HIFB_FMT_BUTT;

    if(enDataFmt >= VDP_GFX_IFMT_BUTT){
        return HIFB_FMT_BUTT;
    }

    ePixFmt = OPTM_ClutHalFmtTransferToPixFmt(enDataFmt);
    if (HIFB_FMT_BUTT != ePixFmt)
    {
        return ePixFmt;
    }

    ePixFmt = OPTM_XYCbCrHalFmtTransferToPixFmt(enDataFmt);
    if (HIFB_FMT_BUTT != ePixFmt)
    {
        return ePixFmt;
    }

    ePixFmt = OPTM_RGBHalFmtTransferToPixFmt(enDataFmt);
    if (HIFB_FMT_BUTT != ePixFmt)
    {
        return ePixFmt;
    }

    ePixFmt = OPTM_XRGBHalFmtTransferToPixFmt(enDataFmt);
    if (HIFB_FMT_BUTT != ePixFmt)
    {
        return ePixFmt;
    }

    return VDP_GFX_IFMT_BUTT;
}

static inline HIFB_COLOR_FMT_E OPTM_ClutHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt)
{
     switch(enDataFmt)
     {
         case VDP_GFX_IFMT_CLUT_1BPP:
            return HIFB_FMT_1BPP;
         case VDP_GFX_IFMT_CLUT_2BPP:
            return HIFB_FMT_2BPP;
         case VDP_GFX_IFMT_CLUT_4BPP:
            return HIFB_FMT_4BPP;
         case VDP_GFX_IFMT_CLUT_8BPP:
            return HIFB_FMT_8BPP;
         case VDP_GFX_IFMT_ACLUT_44:
            return HIFB_FMT_ACLUT44;
         case VDP_GFX_IFMT_ACLUT_88:
            return HIFB_FMT_ACLUT88;
         default:
            return HIFB_FMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}

static inline HIFB_COLOR_FMT_E OPTM_RGBHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt)
{
     switch(enDataFmt)
     {
         case VDP_GFX_IFMT_RGB_565:
            return HIFB_FMT_RGB565;
         case VDP_GFX_IFMT_RGB_888:
            return HIFB_FMT_RGB888;
         default:
            return HIFB_FMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}

static inline HIFB_COLOR_FMT_E OPTM_XRGBHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt)
{
     switch(enDataFmt)
     {
        case VDP_GFX_IFMT_RGB_444:
           return HIFB_FMT_KRGB444;
        case VDP_GFX_IFMT_RGB_555:
           return HIFB_FMT_KRGB555;
        case VDP_GFX_IFMT_ARGB_8565:
           return HIFB_FMT_ARGB8565;
        case VDP_GFX_IFMT_KRGB_888:
           return HIFB_FMT_KRGB888;
        case VDP_GFX_IFMT_ARGB_8888:
            return HIFB_FMT_ARGB8888;
        case VDP_GFX_IFMT_ARGB_4444:
            return HIFB_FMT_ARGB4444;
        case VDP_GFX_IFMT_ARGB_1555:
            return HIFB_FMT_ARGB1555;
        case VDP_GFX_IFMT_RGBA_4444:
            return HIFB_FMT_RGBA4444;
        case VDP_GFX_IFMT_RGBA_5551:
            return HIFB_FMT_RGBA5551;
        case VDP_GFX_IFMT_RGBA_5658:
            return HIFB_FMT_RGBA5658;
        case VDP_GFX_IFMT_RGBA_8888:
            return HIFB_FMT_RGBA8888;
        default:
            return HIFB_FMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}

static inline HIFB_COLOR_FMT_E OPTM_XYCbCrHalFmtTransferToPixFmt(OPTM_VDP_GFX_IFMT_E enDataFmt)
{
     switch(enDataFmt)
     {
         case VDP_GFX_IFMT_PKG_UYVY:
            return HIFB_FMT_PUYVY;
         case VDP_GFX_IFMT_PKG_YUYV:
            return HIFB_FMT_PYUYV;
         case VDP_GFX_IFMT_PKG_YVYU:
            return HIFB_FMT_PYVYU;
         case VDP_GFX_IFMT_YCBCR_888:
            return HIFB_FMT_YUV888;
         case VDP_GFX_IFMT_AYCBCR_8888:
            return HIFB_FMT_AYUV8888;
         case VDP_GFX_IFMT_YCBCRA_8888:
            return HIFB_FMT_YUVA8888;
         default:
            return HIFB_FMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}
#endif


/***************************************************************************************
* func          : OPTM_PixerFmtTransferToHalFmt
* description   : CNcomment: 由hifb像素格式转换成图形层像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
OPTM_VDP_GFX_IFMT_E OPTM_PixerFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt)
{
    OPTM_VDP_GFX_IFMT_E eHalFmt = VDP_GFX_IFMT_BUTT;

    if (enDataFmt >= HIFB_FMT_BUTT)
    {
        return VDP_GFX_IFMT_BUTT;
    }

    eHalFmt = OPTM_ClutPixFmtTransferToHalFmt(enDataFmt);
    if (VDP_GFX_IFMT_BUTT != eHalFmt)
    {
        return eHalFmt;
    }

    eHalFmt = OPTM_XRGBPixFmtTransferToHalFmt(enDataFmt);
    if (VDP_GFX_IFMT_BUTT != eHalFmt)
    {
        return eHalFmt;
    }

    eHalFmt = OPTM_RGBPixFmtTransferToHalFmt(enDataFmt);
    if (VDP_GFX_IFMT_BUTT != eHalFmt)
    {
        return eHalFmt;
    }

    eHalFmt = OPTM_XYCbCrPixFmtTransferToHalFmt(enDataFmt);
    if (VDP_GFX_IFMT_BUTT != eHalFmt)
    {
        return eHalFmt;
    }

    return VDP_GFX_IFMT_BUTT;
}

static inline OPTM_VDP_GFX_IFMT_E OPTM_ClutPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt)
{
     switch(enDataFmt)
     {
        case HIFB_FMT_1BPP:
            return VDP_GFX_IFMT_CLUT_1BPP;
        case HIFB_FMT_2BPP:
            return VDP_GFX_IFMT_CLUT_2BPP;
        case HIFB_FMT_4BPP:
            return VDP_GFX_IFMT_CLUT_4BPP;
        case HIFB_FMT_8BPP:
            return VDP_GFX_IFMT_CLUT_8BPP;
        case HIFB_FMT_ACLUT44:
            return VDP_GFX_IFMT_ACLUT_44;
        default:
            return VDP_GFX_IFMT_BUTT;
     }

     return VDP_GFX_IFMT_BUTT;
}


static inline OPTM_VDP_GFX_IFMT_E OPTM_RGBPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt)
{
     switch(enDataFmt)
     {
        case HIFB_FMT_KRGB444:
           return VDP_GFX_IFMT_RGB_444;
        case HIFB_FMT_KRGB555:
           return VDP_GFX_IFMT_RGB_555;
        case HIFB_FMT_RGB565:
            return VDP_GFX_IFMT_RGB_565;
        case HIFB_FMT_RGB888:
           return VDP_GFX_IFMT_RGB_888;
        case HIFB_FMT_YUV888:
           return VDP_GFX_IFMT_YCBCR_888;
        case HIFB_FMT_KRGB888:
           return VDP_GFX_IFMT_KRGB_888;
        case HIFB_FMT_BGR565:
            return VDP_GFX_IFMT_RGB_565;
        case HIFB_FMT_BGR888:
            return VDP_GFX_IFMT_RGB_888;
        case HIFB_FMT_KBGR444:
            return VDP_GFX_IFMT_RGB_444;
        case HIFB_FMT_KBGR555:
            return VDP_GFX_IFMT_RGB_555;
        case HIFB_FMT_KBGR888:
            return VDP_GFX_IFMT_KRGB_888;
        default:
            return VDP_GFX_IFMT_BUTT;
     }

     return VDP_GFX_IFMT_BUTT;
}

static inline OPTM_VDP_GFX_IFMT_E OPTM_XRGBPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt)
{
     switch(enDataFmt)
     {
        case HIFB_FMT_ARGB8888:
            return VDP_GFX_IFMT_ARGB_8888;
        case HIFB_FMT_ARGB4444:
            return VDP_GFX_IFMT_ARGB_4444;
        case HIFB_FMT_ARGB1555:
            return VDP_GFX_IFMT_ARGB_1555;
        case HIFB_FMT_RGBA4444:
            return VDP_GFX_IFMT_RGBA_4444;
        case HIFB_FMT_RGBA5551:
            return VDP_GFX_IFMT_RGBA_5551;
        case HIFB_FMT_RGBA5658:
            return VDP_GFX_IFMT_RGBA_5658;
        case HIFB_FMT_RGBA8888:
            return VDP_GFX_IFMT_RGBA_8888;
        case HIFB_FMT_ABGR4444:
            return VDP_GFX_IFMT_ARGB_4444;
        case HIFB_FMT_ABGR1555:
            return VDP_GFX_IFMT_ARGB_1555;
        case HIFB_FMT_ABGR8888:
            return VDP_GFX_IFMT_ABGR_8888;
        case HIFB_FMT_ABGR8565:
            return VDP_GFX_IFMT_ARGB_8565;
        case HIFB_FMT_ARGB8565:
            return VDP_GFX_IFMT_ARGB_8565;
        default:
            return VDP_GFX_IFMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}

static inline OPTM_VDP_GFX_IFMT_E OPTM_XYCbCrPixFmtTransferToHalFmt(HIFB_COLOR_FMT_E enDataFmt)
{
     switch(enDataFmt)
     {
        case HIFB_FMT_PUYVY:
            return VDP_GFX_IFMT_PKG_UYVY;
        case HIFB_FMT_PYUYV:
            return VDP_GFX_IFMT_PKG_YUYV;
        case HIFB_FMT_PYVYU:
            return VDP_GFX_IFMT_PKG_YVYU;
        case HIFB_FMT_ACLUT88:
           return VDP_GFX_IFMT_ACLUT_88;
        case HIFB_FMT_YUVA8888:
            return VDP_GFX_IFMT_YCBCRA_8888;
        case HIFB_FMT_AYUV8888:
            return VDP_GFX_IFMT_AYCBCR_8888;
        default:
            return VDP_GFX_IFMT_BUTT;
     }
     return VDP_GFX_IFMT_BUTT;
}


/***************************************************************************************
* func          : OPTM_GfxSetLayerDataFmt
* description   : CNcomment: 设置图层格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerDataFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt)
{
    HI_S32 Ret = HI_SUCCESS;
    OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;

    if (!g_stGfxCap[enLayerId].bColFmt[enDataFmt])
    {
        HIFB_ERROR("unSupport color format.\n");
        return HI_FAILURE;
    }
    g_stGfxDevice[enLayerId].enDataFmt = enDataFmt;

    OPTM_CheckGPMask_BYGPID(enGPId);

    if (   (HIFB_FMT_ABGR8888 != enDataFmt)
        && ((enDataFmt >= HIFB_FMT_BGR565) && (HIFB_FMT_KBGR888 >= enDataFmt))
        && (g_stGfxDevice[enLayerId].CscState == OPTM_CSC_SET_PARA_RGB)){
        Ret = HIFB_ADP_SetLayerDataBigEndianFmt(enLayerId,enDataFmt);
    }
    else if (  ((HIFB_FMT_ABGR8888 == enDataFmt) || (HIFB_FMT_BGR565 > enDataFmt || enDataFmt > HIFB_FMT_KBGR888))
            && (g_stGfxDevice[enLayerId].CscState == OPTM_CSC_SET_PARA_BGR))
    {
        Ret = HIFB_ADP_SetLayerDataLittleEndianFmt(enLayerId,enDataFmt);
    }

    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    OPTM_VDP_GFX_SetInDataFmt(g_stGfxDevice[enLayerId].enGfxHalId,OPTM_PixerFmtTransferToHalFmt(enDataFmt));

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    if(g_stGfxDevice[enLayerId].bCmpOpened){
        if (HIFB_FMT_ARGB8888 != enDataFmt){
            g_stGfxDevice[enLayerId].stCmpInfo.enStatus = OPTM_CMP_STATUS_PARALYSED;
        }
    }
#endif

    return HI_SUCCESS;
}


static inline HI_S32 HIFB_ADP_SetLayerDataBigEndianFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt)
{
     HI_S32 s32Cnt = 0;
     HI_U32 u32LayerCount = 0;
     OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;

     u32LayerCount = (HIFB_LAYER_HD_3 >= enLayerId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;

     for (s32Cnt = 0; s32Cnt < u32LayerCount; s32Cnt++)
     {
         if ((s32Cnt != enLayerId) && (g_stGfxDevice[s32Cnt].bEnable) && (g_stGfxDevice[s32Cnt].CscState != OPTM_CSC_SET_PARA_BGR))
         {
             HIFB_ERROR("fail to set color format.\n");
             return HI_FAILURE;
         }
     }

     g_stGfxDevice[enLayerId].CscState = OPTM_CSC_SET_PARA_BGR;
     g_stGfxGPDevice[enGPId].bBGRState = HI_TRUE;

     OPTM_GPSetCsc(enGPId,  &g_stGfxGPDevice[enGPId].stCscPara, HI_TRUE);

     if (IS_MASTER_GP(enGPId))
     {
         OPTM_GPSetCsc(OPTM_SLAVER_GPID,  &g_stGfxGPDevice[OPTM_SLAVER_GPID].stCscPara, HI_TRUE);
         g_stGfxGPDevice[OPTM_SLAVER_GPID].bBGRState = HI_TRUE;
     }

     return HI_SUCCESS;
}

static inline HI_S32 HIFB_ADP_SetLayerDataLittleEndianFmt(HIFB_LAYER_ID_E enLayerId, HIFB_COLOR_FMT_E enDataFmt)
{
     HI_S32 s32Cnt = 0;
     HI_U32 u32LayerCount = 0;
     OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;

     u32LayerCount = (HIFB_LAYER_HD_3 >= enLayerId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;

     for (s32Cnt = 0; s32Cnt < u32LayerCount; s32Cnt++)
     {
         if ( (s32Cnt != enLayerId) && (g_stGfxDevice[s32Cnt].bEnable) && (g_stGfxDevice[s32Cnt].CscState != OPTM_CSC_SET_PARA_RGB))
         {
             HIFB_ERROR("fail to set color format.\n");
             return HI_FAILURE;
         }
     }
     g_stGfxDevice[enLayerId].CscState = OPTM_CSC_SET_PARA_RGB;
     g_stGfxGPDevice[enGPId].bBGRState = HI_FALSE;

     OPTM_GPSetCsc(enGPId,  &g_stGfxGPDevice[enGPId].stCscPara, HI_FALSE);

     if(IS_MASTER_GP(enGPId))
     {
         OPTM_GPSetCsc(OPTM_SLAVER_GPID,  &g_stGfxGPDevice[OPTM_SLAVER_GPID].stCscPara, HI_FALSE);
         g_stGfxGPDevice[OPTM_SLAVER_GPID].bBGRState = HI_FALSE;
     }
     return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GFX_SetClutColorReg
* description   : CNcomment: 设置调色板颜色寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_SetClutColorReg(HIFB_LAYER_ID_E enLayerId, HI_U32 u32OffSet, HI_U32 u32Color, HI_S32 UpFlag)
{
    HI_U32 *pCTab = (HI_U32 *)(g_stGfxDevice[enLayerId].stCluptTable.pu8StartVirAddr);

    if (HI_NULL == pCTab){
        HIFB_ERROR("Unable to handle null virtual address!\n");
        return HI_FAILURE;
    }

    pCTab[u32OffSet] = u32Color;
    if(UpFlag != 0){
        OPTM_VDP_GFX_SetParaUpd(g_stGfxDevice[enLayerId].enGfxHalId,VDP_DISP_COEFMODE_LUT);
    }

    return HI_SUCCESS;
}


#ifdef OPTM_HIFB_WVM_ENABLE
/***************************************************************************************
* func          : OPTM_GfxWVBCallBack
* description   : CNcomment: 等中断完成 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GfxWVBCallBack(HI_U32 enLayerId, HI_U32 u32Param1)
{
    g_stGfxDevice[enLayerId].vblflag = 1;
    wake_up_interruptible(&(g_stGfxDevice[enLayerId].vblEvent));
    return;
}

/***************************************************************************************
* func          : OPTM_GfxWaitVBlank
* description   : CNcomment: 等中断 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxWaitVBlank(HIFB_LAYER_ID_E enLayerId)
{
    HI_U32 u32TimeOutMs = 0;

    D_OPTM_HIFB_CheckGfxOpen(enLayerId);

    u32TimeOutMs = (200 * HZ)/1000;

    g_stGfxDevice[enLayerId].vblflag = 0;
    (HI_VOID)wait_event_interruptible_timeout(g_stGfxDevice[enLayerId].vblEvent, g_stGfxDevice[enLayerId].vblflag, u32TimeOutMs);

    return HI_SUCCESS;
}

#else

HI_S32 OPTM_GfxWaitVBlank(HIFB_LAYER_ID_E enLayerId)
{
    HIFB_ERROR("GFX ERROR! NOT enable wait v blank\n");
    return HI_FAILURE;
}

#endif

/***************************************************************************************
* func          : OPTM_GfxSetLayerDeFlicker
* description   : CNcomment: 设置图层抗闪，不支持 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerDeFlicker(HIFB_LAYER_ID_E enLayerId, HIFB_DEFLICKER_S *pstDeFlicker)
{
    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GfxSetLayerAlpha
* description   : CNcomment: 设置图层alpha CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerAlpha(HIFB_LAYER_ID_E enLayerId, HIFB_ALPHA_S *pstAlpha)
{
    HI_GFX_Memcpy(&g_stGfxDevice[enLayerId].stAlpha, pstAlpha, sizeof(HIFB_ALPHA_S));

    /** 是否已经开机过渡完 **/
    OPTM_CheckGPMask_BYLayerID(enLayerId);

    OPTM_VDP_GFX_SetPalpha(g_stGfxDevice[enLayerId].enGfxHalId,pstAlpha->bAlphaEnable,HI_TRUE,pstAlpha->u8Alpha0,pstAlpha->u8Alpha1);
    OPTM_VDP_GFX_SetLayerGalpha(g_stGfxDevice[enLayerId].enGfxHalId, pstAlpha->u8GlobalAlpha);

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : OPTM_GfxGetLayerGalphaSum
* description   : CNcomment: 获取设置全局alpha为0的和 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetLayerGalphaSum(HIFB_LAYER_ID_E enLayerId, HI_U32 *pu32GalphaSum)
{
#ifdef CONFIG_HIFB_GP0_GALPHA_SUM_SUPPORT
    OPTM_VDP_GP_GetLayerGalphaSum(g_stGfxDevice[enLayerId].enGPId,pu32GalphaSum);
#endif
    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_GfxGetLayerRect
* description   : CNcomment: 获取图层输入分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetLayerRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstRect)
{
    HI_GFX_Memcpy(pstRect, &g_stGfxDevice[enLayerId].stInRect, sizeof(HIFB_RECT));
    return HI_SUCCESS;
}

#if 0
/***************************************************************************************
* func            : OPTM_GfxConfigCursorRect
* description    : CNcomment: 配置硬件鼠标层区域 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxConfigCursorRect(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstRect)
{

    OPTM_GFX_GP_E enGpId = g_stGfxDevice[enLayerId].enGPId;
    OPTM_VDP_DISP_RECT_S stGfxRect;

    if(OPTM_CURSOR_LAYERID != enLayerId){
        return HI_FAILURE;
    }

    HI_GFX_Memset(&stGfxRect, 0, sizeof(OPTM_VDP_DISP_RECT_S));

    /** OutRect **/
    stGfxRect.u32DXS = g_stGfxGPDevice[enGpId].stOutRect.s32X      & 0xfffffffe;
    stGfxRect.u32DYS = g_stGfxGPDevice[enGpId].stOutRect.s32Y      & 0xfffffffe;
    stGfxRect.u32DXL = g_stGfxGPDevice[enGpId].stOutRect.s32Width  & 0xfffffffe;
    stGfxRect.u32DYL = g_stGfxGPDevice[enGpId].stOutRect.s32Height & 0xfffffffe;
    /** InRect **/
    stGfxRect.u32VX  = g_stGfxDevice[enLayerId].stInRect.x & 0xfffffffe;
    stGfxRect.u32VY  = g_stGfxDevice[enLayerId].stInRect.y & 0xfffffffe;
    stGfxRect.u32IWth= g_stGfxDevice[enLayerId].stInRect.w & 0xfffffffe;
    stGfxRect.u32IHgt= g_stGfxDevice[enLayerId].stInRect.h & 0xfffffffe;

    if (stGfxRect.u32VX > stGfxRect.u32DXL)
    {
        stGfxRect.u32VX = stGfxRect.u32DXL;
    }

    if (stGfxRect.u32VY > stGfxRect.u32DYL)
    {
        stGfxRect.u32VY = stGfxRect.u32DYL;
    }

    if ((stGfxRect.u32VX + stGfxRect.u32IWth) > stGfxRect.u32DXL)
    {
        stGfxRect.u32IWth = stGfxRect.u32DXL - stGfxRect.u32VX;
    }

    if ((stGfxRect.u32VY + stGfxRect.u32IHgt) > stGfxRect.u32DYL)
    {
        stGfxRect.u32IHgt = stGfxRect.u32DYL - stGfxRect.u32VY;
    }

    stGfxRect.u32OWth = stGfxRect.u32IWth;
    stGfxRect.u32OHgt = stGfxRect.u32IHgt;

    OPTM_VDP_GFX_SetLayerReso(g_stGfxDevice[enLayerId].enGfxHalId, stGfxRect);

    /**
     **更新寄存器
     **/
    OPTM_VDP_GFX_SetRegUp    (g_stGfxDevice[enLayerId].enGfxHalId);

    return HI_SUCCESS;

}
#endif

/***************************************************************************************
* func            : OPTM_GfxSetLayerRect
* description    : CNcomment: 设置图层输入矩形 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerRect(HIFB_LAYER_ID_E enLayerId, const HIFB_RECT *pstRect)
{
    OPTM_GFX_GP_E enGpId = g_stGfxDevice[enLayerId].enGPId;
    OPTM_VDP_DISP_RECT_S stGfxRect;

    /**
     **保存输入矩形
     **/
    g_stGfxDevice[enLayerId].stInRect.x = pstRect->x;
    g_stGfxDevice[enLayerId].stInRect.y = pstRect->y;
    g_stGfxDevice[enLayerId].stInRect.w = pstRect->w;
    g_stGfxDevice[enLayerId].stInRect.h = pstRect->h;

    /**
     **是否完成开机过渡
     **/
    OPTM_CheckGPMask_BYLayerID(enLayerId);

#if 0
    if (OPTM_CURSOR_LAYERID == enLayerId)
    {/**
      **硬件鼠标层，配置鼠标区域
      **/
        return OPTM_GfxConfigCursorRect(enLayerId, pstRect);
    }
#endif

    HI_GFX_Memset(&stGfxRect, 0, sizeof(OPTM_VDP_DISP_RECT_S));

    /**
     **V 和 D设置成一样的
     **/
    stGfxRect.u32VX  = g_stGfxDevice[enLayerId].stInRect.x;
    stGfxRect.u32DXS = g_stGfxDevice[enLayerId].stInRect.x;
    stGfxRect.u32VY  = g_stGfxDevice[enLayerId].stInRect.y;
    stGfxRect.u32DYS = g_stGfxDevice[enLayerId].stInRect.y;
    stGfxRect.u32IWth= g_stGfxDevice[enLayerId].stInRect.w;
    stGfxRect.u32IHgt= g_stGfxDevice[enLayerId].stInRect.h;

    /**
     ** when gfx's disprect beyond the region of gp_inrect, clip disprect before set it to hal
     **/
    if (g_stGfxDevice[enLayerId].stInRect.x > g_stGfxGPDevice[enGpId].stInRect.s32Width - g_stGfxCap[enLayerId].u32MinWidth)
    {
        g_stGfxDevice[enLayerId].stInRect.x = g_stGfxGPDevice[enGpId].stInRect.s32Width - g_stGfxCap[enLayerId].u32MinWidth;
    }

    if (g_stGfxDevice[enLayerId].stInRect.y > g_stGfxGPDevice[enGpId].stInRect.s32Height - g_stGfxCap[enLayerId].u32MinHeight)
    {
        g_stGfxDevice[enLayerId].stInRect.y = g_stGfxGPDevice[enGpId].stInRect.s32Height - g_stGfxCap[enLayerId].u32MinHeight;
    }

    if (!IS_SLAVER_GP(enGpId))
    {
        if ((g_stGfxDevice[enLayerId].stInRect.x + g_stGfxDevice[enLayerId].stInRect.w)
            > g_stGfxGPDevice[enGpId].stInRect.s32Width)
        {
            stGfxRect.u32IWth = g_stGfxGPDevice[enGpId].stInRect.s32Width - g_stGfxDevice[enLayerId].stInRect.x;
        }

        if ((g_stGfxDevice[enLayerId].stInRect.y + g_stGfxDevice[enLayerId].stInRect.h)
            > g_stGfxGPDevice[enGpId].stInRect.s32Height)
        {
            stGfxRect.u32IHgt = g_stGfxGPDevice[enGpId].stInRect.s32Height - g_stGfxDevice[enLayerId].stInRect.y;
        }
    }

    if (g_stGfxGPDevice[enGpId].bNeedExtractLine)
    {/** 是否抽行处理 **/
        stGfxRect.u32IHgt /= 2;
        stGfxRect.u32VY   /= 2;
        stGfxRect.u32DYS  /= 2;
        g_stGfxDevice[enLayerId].bExtractLine = HI_TRUE;
        OPTM_VDP_GFX_SetLayerStride(g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].Stride * 2);
    }
    else if (HI_TRUE == g_stGfxDevice[enLayerId].bExtractLine)
    {
        OPTM_GfxSetLayerStride(enLayerId, g_stGfxDevice[enLayerId].Stride);
        g_stGfxDevice[enLayerId].bExtractLine = HI_FALSE;
    }


    stGfxRect.u32OWth = stGfxRect.u32IWth;
    stGfxRect.u32OHgt = stGfxRect.u32IHgt;
    stGfxRect.u32DXL  = g_stGfxDevice[enLayerId].stInRect.x + stGfxRect.u32OWth;
    stGfxRect.u32DYL  = g_stGfxDevice[enLayerId].stInRect.y + stGfxRect.u32OHgt;

    /*************************************/
    if ( (g_enOptmGfxWorkMode == HIFB_GFX_MODE_NORMAL) && (enGpId == OPTM_GFX_GP_1))
    {
        stGfxRect.u32DXS = 0;
        stGfxRect.u32DYS = 0;
        stGfxRect.u32DXL = g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].stInRect.s32Width;
        stGfxRect.u32DYL = g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].stInRect.s32Height;

        if (g_stGfxGPDevice[enGpId].bNeedExtractLine)
        {
            stGfxRect.u32DYL /= 2;
        }
    }
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (HIFB_STEREO_SIDEBYSIDE_HALF == g_stGfxDevice[enLayerId].enTriDimMode
        || HIFB_STEREO_TOPANDBOTTOM == g_stGfxDevice[enLayerId].enTriDimMode)
    {
        stGfxRect.u32DXS = 0;
        stGfxRect.u32DYS = 0;
        stGfxRect.u32DXL = g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].stInRect.s32Width;
        stGfxRect.u32DYL = g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].stInRect.s32Height;
    }

    if (HIFB_STEREO_SIDEBYSIDE_HALF == g_stGfxDevice[enLayerId].enTriDimMode)
    {
        stGfxRect.u32IWth = stGfxRect.u32IWth/2;
        stGfxRect.u32IWth &= 0xfffffffe;

        stGfxRect.u32OWth = (stGfxRect.u32IWth)*2;
    }
    else if (HIFB_STEREO_TOPANDBOTTOM == g_stGfxDevice[enLayerId].enTriDimMode)
    {
        stGfxRect.u32IHgt = stGfxRect.u32IHgt/2;
        stGfxRect.u32IHgt &= 0xfffffffe;
        stGfxRect.u32OHgt = (stGfxRect.u32IHgt)*2;
    }
#endif
    stGfxRect.u32VX   &= 0xfffffffe;
    stGfxRect.u32VY   &= 0xfffffffe;

    stGfxRect.u32DXS  &= 0xfffffffe;
    stGfxRect.u32DYS  &= 0xfffffffe;

    stGfxRect.u32DXL  &= 0xfffffffe;
    stGfxRect.u32DYL  &= 0xfffffffe;

    stGfxRect.u32IWth &= 0xfffffffe;
    stGfxRect.u32IHgt &= 0xfffffffe;

    stGfxRect.u32OWth &= 0xfffffffe;
    stGfxRect.u32OHgt &= 0xfffffffe;

    OPTM_VDP_GFX_SetLayerReso(g_stGfxDevice[enLayerId].enGfxHalId, stGfxRect);

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    if (g_stGfxDevice[enLayerId].bCmpOpened)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.bLayerResChgFlag = HI_TRUE;
    }
#endif

    return HI_SUCCESS;

}


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func            : OPTM_GfxSetGpInPutSize
* description    : CNcomment: 设置GP输入大小 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetGpInPutSize(OPTM_GFX_GP_E enGpId, HI_U32 u32Width, HI_U32 u32Height)
{
    g_stGfxGPDevice[enGpId].stInRect.s32Width  = u32Width;
    g_stGfxGPDevice[enGpId].stInRect.s32Height = u32Height;
    return HI_SUCCESS;
}

/***************************************************************************************
* func            : OPTM_GfxGetDispFMTSize
* description    : CNcomment: 获取disp大小 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetDispFMTSize(OPTM_GFX_GP_E enGpId, HIFB_RECT *pstOutRect)
{
    pstOutRect->x = g_stGfxGPDevice[enGpId].stOutRect.s32X;
    pstOutRect->y = g_stGfxGPDevice[enGpId].stOutRect.s32Y;
    pstOutRect->w = g_stGfxGPDevice[enGpId].stOutRect.s32Width;
    pstOutRect->h = g_stGfxGPDevice[enGpId].stOutRect.s32Height;

    if(pstOutRect->w == 0 || pstOutRect->h == 0){
        pstOutRect->x = 0;
        pstOutRect->y = 0;
        pstOutRect->w = 1280;
        pstOutRect->h = 720;
    }

    if(pstOutRect->w == 1440 && (pstOutRect->h == 576 || pstOutRect->h == 480)){
        pstOutRect->w /= 2;
    }

    return HI_SUCCESS;
}
#endif


/***************************************************************************************
* func          : OPTM_GfxSetDispFMTSize
* description   : CNcomment: 设置display像素格式和大小 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetDispFMTSize(OPTM_GFX_GP_E enGpId, const HI_RECT_S *pstOutRect)
{

    HI_U32    u32Ratio    = 0;
    HIFB_RECT stInputRect = {0};

    if(pstOutRect->s32X < 0 || pstOutRect->s32Y < 0){
        return HI_FAILURE;
    }

    if(pstOutRect->s32Width <= 0 || pstOutRect->s32Height <= 0){
        return HI_FAILURE;
    }

    g_stGfxGPDevice[enGpId].stOutRect.s32X      = pstOutRect->s32X;
    g_stGfxGPDevice[enGpId].stOutRect.s32Y      = pstOutRect->s32Y;
    g_stGfxGPDevice[enGpId].stOutRect.s32Width  = pstOutRect->s32Width;
    g_stGfxGPDevice[enGpId].stOutRect.s32Height = pstOutRect->s32Height;

    OPTM_CheckGPMask_BYGPID(enGpId);

    stInputRect.x = g_stGfxGPDevice[enGpId].stInRect.s32X;
    stInputRect.y = g_stGfxGPDevice[enGpId].stInRect.s32Y;
    stInputRect.w = g_stGfxGPDevice[enGpId].stInRect.s32Width;
    stInputRect.h = g_stGfxGPDevice[enGpId].stInRect.s32Height;

    /*judge wether need to extract line for layer or not*/
    u32Ratio =  g_stGfxGPDevice[enGpId].stInRect.s32Height*2;
    u32Ratio /= g_stGfxGPDevice[enGpId].stOutRect.s32Height;

    if(g_stGfxGPDevice[enGpId].bInterface)
    {
        //u32Ratio *= 2;
    }

    if (u32Ratio >= OPTM_EXTRACTLINE_RATIO
        && !IS_SLAVER_GP(enGpId))
    {
        g_stGfxGPDevice[enGpId].bNeedExtractLine = HI_TRUE;
    }
    else
    {
        g_stGfxGPDevice[enGpId].bNeedExtractLine = HI_FALSE;
    }

    if(stInputRect.w && stInputRect.h)
    {
        OPTM_GfxSetGpRect(enGpId, &stInputRect);
    }

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : OPTM_GfxSetGpRect
* description   : CNcomment: 设置GP RECT CNend\n
*                 (1)回写更新需要设置GP寄存器
                  (2)显示制式发生变化的时候
                  (3)输入分辨率发生变化的时候
                  (4)待机打开图层需要重新配置GP
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#define SHARPEN_RATIO        3
#define SHARPEN_MAX_WIDTH    1920
HI_S32 OPTM_GfxSetGpRect(OPTM_GFX_GP_E enGpId, const HIFB_RECT * pstInputRect)
{
    HI_BOOL bGfxSharpen = HI_FALSE;
    OPTM_VDP_DISP_RECT_S stGfxRect        = {0};
    OPTM_ALG_GZME_DRV_PARA_S stZmeDrvPara = {0};
    OPTM_ALG_GZME_RTL_PARA_S stZmeRtlPara = {0};
    OPTM_ALG_GDTI_DRV_PARA_S stDtiDrvPara = {0};
    OPTM_ALG_GDTI_RTL_PARA_S stDtiRtlPara = {0};

    /**
     ** GP输入分辨率和虚拟分辨率保持一致，在display中断里面获取虚拟分辨率的值
     **/
    g_stGfxGPDevice[enGpId].stInRect.s32Width  = pstInputRect->w;
    g_stGfxGPDevice[enGpId].stInRect.s32Height = pstInputRect->h;


    OPTM_CheckGPMask_BYGPID(enGpId);

    stGfxRect.u32IWth = g_stGfxGPDevice[enGpId].stInRect.s32Width   & 0xfffffffe;
    stGfxRect.u32IHgt = g_stGfxGPDevice[enGpId].stInRect.s32Height  & 0xfffffffe;
    stGfxRect.u32OWth = g_stGfxGPDevice[enGpId].stOutRect.s32Width  & 0xfffffffe;
    stGfxRect.u32OHgt = g_stGfxGPDevice[enGpId].stOutRect.s32Height & 0xfffffffe;

    stGfxRect.u32DXS  = g_stGfxGPDevice[enGpId].stOutRect.s32X & 0xfffffffe;
    stGfxRect.u32DYS  = g_stGfxGPDevice[enGpId].stOutRect.s32Y & 0xfffffffe;
    stGfxRect.u32DXL  = stGfxRect.u32OWth + stGfxRect.u32DXS;
    stGfxRect.u32DYL  = stGfxRect.u32OHgt + stGfxRect.u32DYS;
    stGfxRect.u32VX   = stGfxRect.u32DXS;
    stGfxRect.u32VY   = stGfxRect.u32DYS;
    stGfxRect.u32VXL  = stGfxRect.u32OWth + stGfxRect.u32VX;
    stGfxRect.u32VYL  = stGfxRect.u32OHgt + stGfxRect.u32VY;

    if ((stGfxRect.u32IWth == 0) || (stGfxRect.u32IHgt == 0) || (stGfxRect.u32OWth == 0) || (stGfxRect.u32OHgt == 0))
    {
        return HI_SUCCESS;
    }

    if (g_stGfxGPDevice[enGpId].bNeedExtractLine == HI_TRUE)
    {
        stGfxRect.u32IHgt /= 2;
    }

    OPTM_VDP_GP_SetLayerReso(enGpId, stGfxRect);

    /**
     ** Frame format for zme : 0-field; 1-frame
     **/
    stZmeDrvPara.bZmeFrmFmtIn  = HI_TRUE;
    stZmeDrvPara.bZmeFrmFmtOut = g_stGfxGPDevice[enGpId].bInterface ? HI_FALSE : HI_TRUE;

    if (IS_SLAVER_GP(enGpId)){
    /** 回写模式并且打开的是标清层 **/

        stGfxRect.u32IWth  = g_stGfxWbc2.stInRect.w & 0xfffffffe;
        stGfxRect.u32IHgt  = g_stGfxWbc2.stInRect.h & 0xfffffffe;

        if(stGfxRect.u32IWth == 0 || stGfxRect.u32IHgt == 0){
            return HI_SUCCESS;
        }

        /*in wbc work mode, slavery channel show 2D image*/
        if (HIFB_STEREO_SIDEBYSIDE_HALF == g_stGfxGPDevice[OPTM_SLAVER_GPID].enTriDimMode){
            stGfxRect.u32IWth /= 2;
        }else if (HIFB_STEREO_TOPANDBOTTOM == g_stGfxGPDevice[OPTM_SLAVER_GPID].enTriDimMode){
            stGfxRect.u32IHgt /= 2;
        }

        if ((g_stGfxGPDevice[OPTM_MASTER_GPID].bNeedExtractLine == HI_TRUE)){
            stGfxRect.u32IHgt /= 2;
        }

        HIFB_WBC2_SetCropReso(stGfxRect);

    }

    stZmeDrvPara.u32ZmeFrmWIn  = stGfxRect.u32IWth;
    stZmeDrvPara.u32ZmeFrmHIn  = stGfxRect.u32IHgt;
    stZmeDrvPara.u32ZmeFrmWOut = stGfxRect.u32OWth;
    stZmeDrvPara.u32ZmeFrmHOut = stGfxRect.u32OHgt;

    if (OPTM_DISPCHANNEL_1 == g_stGfxGPDevice[enGpId].enDispCh){
        stZmeDrvPara.u32ZmeHdDeflicker = g_stGfxGPDevice[enGpId].u32ZmeDeflicker;
        OPTM_ALG_GZmeHDSet(&gs_stGPZme, &stZmeDrvPara, &stZmeRtlPara);
    }else if (OPTM_DISPCHANNEL_0 == g_stGfxGPDevice[enGpId].enDispCh){
        HIFB_WBC2_SetPreZmeEn(&stZmeDrvPara);
        stZmeDrvPara.u32ZmeSdDeflicker = g_stGfxGPDevice[enGpId].u32ZmeDeflicker;
        OPTM_ALG_GZmeSDSet(&gs_stGPZme, &stZmeDrvPara, &stZmeRtlPara);
    }else{
        return HI_FAILURE;
    }

    stDtiDrvPara.u32ZmeFrmWIn  = stGfxRect.u32IWth;
    stDtiDrvPara.u32ZmeFrmHIn  = stGfxRect.u32IHgt;
    stDtiDrvPara.u32ZmeFrmWOut = stGfxRect.u32OWth;
    stDtiDrvPara.u32ZmeFrmHOut = stGfxRect.u32OHgt;

    OPTM_ALG_GDtiSet(&stDtiDrvPara, &stDtiRtlPara);

    if (stZmeDrvPara.u32ZmeFrmWIn > SHARPEN_MAX_WIDTH){
        stZmeRtlPara.bZmeEnH = HI_FALSE;
        stZmeRtlPara.bZmeEnV = HI_FALSE;
    }

    /*zme enable horizontal*/
    OPTM_VDP_GP_SetZmeEnable(enGpId, VDP_ZME_MODE_HOR, stZmeRtlPara.bZmeEnH);
    OPTM_VDP_GP_SetZmeEnable(enGpId, VDP_ZME_MODE_VER, stZmeRtlPara.bZmeEnV);

    if (stZmeRtlPara.bZmeEnH && stZmeRtlPara.bZmeEnV)
    {
          bGfxSharpen = HI_TRUE;
    }

    if (   stDtiDrvPara.u32ZmeFrmWIn * 2 / stDtiDrvPara.u32ZmeFrmWOut > SHARPEN_RATIO \
        || stDtiDrvPara.u32ZmeFrmHIn * 2/ stDtiDrvPara.u32ZmeFrmHOut > SHARPEN_RATIO)
    {
        bGfxSharpen = HI_FALSE;
    }

    HIFB_ADP_SetZme(enGpId,bGfxSharpen,&stZmeRtlPara,&stDtiRtlPara);

    OPTM_VDP_GP_SetParaUpd(enGpId,VDP_ZME_MODE_HOR);
    OPTM_VDP_GP_SetParaUpd(enGpId,VDP_ZME_MODE_VER);

    HIFB_ADP_ResetLayerInRect(enGpId);

    return HI_SUCCESS;
}

static inline HI_VOID HIFB_ADP_SetZme(OPTM_GFX_GP_E enGpId, HI_BOOL bGfxSharpen, OPTM_ALG_GZME_RTL_PARA_S *pstZmeRtlPara, OPTM_ALG_GDTI_RTL_PARA_S *pstDtiRtlPara)
{

    HI_BOOL  bSlvGp = HI_FALSE;

    if (IS_SLAVER_GP(enGpId))
    {
       bSlvGp  = HI_TRUE;
    }

    if ((HI_FALSE == pstZmeRtlPara->bZmeEnH) && (HI_FALSE == pstZmeRtlPara->bZmeEnV))
    {
        OPTM_VDP_GP_SetTiEnable       (enGpId, VDP_TI_MODE_CHM, HI_FALSE);
        OPTM_VDP_GP_SetTiEnable       (enGpId, VDP_TI_MODE_LUM, HI_FALSE);
        OPTM_VDP_GP_SetIpOrder        (enGpId, bSlvGp, VDP_GP_ORDER_CSC);
        return;
    }

    OPTM_VDP_GP_SetIpOrder            (enGpId, bSlvGp, VDP_GP_ORDER_ZME_CSC);
    OPTM_VDP_GP_SetZmeHfirOrder       (enGpId, VDP_ZME_ORDER_HV);
    OPTM_VDP_GP_SetZmeCoefAddr        (enGpId, VDP_GP_PARA_ZME_HOR, pstZmeRtlPara->u32ZmeCoefAddrHL);
    OPTM_VDP_GP_SetZmeFirEnable       (enGpId, VDP_ZME_MODE_HOR,    pstZmeRtlPara->bZmeMdHLC);
    OPTM_VDP_GP_SetZmeFirEnable       (enGpId, VDP_ZME_MODE_ALPHA,  pstZmeRtlPara->bZmeMdHA);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_ALPHA,  pstZmeRtlPara->bZmeMedHA);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_HORL,   pstZmeRtlPara->bZmeMedHL);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_HORC,   pstZmeRtlPara->bZmeMedHC);
    OPTM_VDP_GP_SetZmePhase           (enGpId, VDP_ZME_MODE_HORL,   pstZmeRtlPara->s32ZmeOffsetHL);
    OPTM_VDP_GP_SetZmePhase           (enGpId, VDP_ZME_MODE_HORC,   pstZmeRtlPara->s32ZmeOffsetHC);
    OPTM_VDP_GP_SetZmeHorRatio        (enGpId, pstZmeRtlPara->u32ZmeRatioHL);
    OPTM_VDP_GP_SetZmeCoefAddr        (enGpId, VDP_GP_PARA_ZME_VER, pstZmeRtlPara->u32ZmeCoefAddrVL);
    OPTM_VDP_GP_SetZmeFirEnable       (enGpId, VDP_ZME_MODE_VER,    pstZmeRtlPara->bZmeMdVLC);
    OPTM_VDP_GP_SetZmeFirEnable       (enGpId, VDP_ZME_MODE_ALPHAV, pstZmeRtlPara->bZmeMdVA);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_ALPHAV, pstZmeRtlPara->bZmeMedVA);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_VERL,   pstZmeRtlPara->bZmeMedVL);
    OPTM_VDP_GP_SetZmeMidEnable       (enGpId, VDP_ZME_MODE_VERC,   pstZmeRtlPara->bZmeMedVC);
    OPTM_VDP_GP_SetZmePhase           (enGpId, VDP_ZME_MODE_VERL,   pstZmeRtlPara->s32ZmeOffsetVBtm);
    OPTM_VDP_GP_SetZmePhase           (enGpId, VDP_ZME_MODE_VERC,   pstZmeRtlPara->s32ZmeOffsetVTop);
    OPTM_VDP_GP_SetZmeVerRatio        (enGpId, pstZmeRtlPara->u32ZmeRatioVL);

    if (OPTM_DISPCHANNEL_1 == g_stGfxGPDevice[enGpId].enDispCh && bGfxSharpen)
    {
         OPTM_VDP_GP_SetTiEnable     (enGpId, VDP_TI_MODE_CHM, pstDtiRtlPara->bEnCTI);
         OPTM_VDP_GP_SetTiEnable     (enGpId, VDP_TI_MODE_LUM, pstDtiRtlPara->bEnLTI);
         OPTM_VDP_GP_SetZmeHfirOrder (enGpId, VDP_ZME_ORDER_VH);
    }
    else
    {
         OPTM_VDP_GP_SetTiEnable     (enGpId, VDP_TI_MODE_CHM, HI_FALSE);
         OPTM_VDP_GP_SetTiEnable     (enGpId, VDP_TI_MODE_LUM, HI_FALSE);
    }

    OPTM_VDP_GP_SetTiHpCoef       (enGpId, VDP_TI_MODE_CHM, (HI_S32 *)pstDtiRtlPara->s32CTIHPTmp);
    OPTM_VDP_GP_SetTiHpCoef       (enGpId, VDP_TI_MODE_LUM, (HI_S32 *)pstDtiRtlPara->s32LTIHPTmp);
    OPTM_VDP_GP_SetTiGainRatio    (enGpId, VDP_TI_MODE_CHM, (HI_S32)pstDtiRtlPara->s16CTICompsatRatio);
    OPTM_VDP_GP_SetTiGainRatio    (enGpId, VDP_TI_MODE_LUM, (HI_S32)pstDtiRtlPara->s16LTICompsatRatio);
    OPTM_VDP_GP_SetTiCoringThd    (enGpId, VDP_TI_MODE_CHM, (HI_U32)pstDtiRtlPara->u16CTICoringThrsh);
    OPTM_VDP_GP_SetTiCoringThd    (enGpId, VDP_TI_MODE_LUM, (HI_U32)pstDtiRtlPara->u16LTICoringThrsh);
    OPTM_VDP_GP_SetTiSwingThd     (enGpId, VDP_TI_MODE_CHM, (HI_U32)pstDtiRtlPara->u16CTIOverSwingThrsh, (HI_U32)pstDtiRtlPara->u16CTIUnderSwingThrsh);
    OPTM_VDP_GP_SetTiSwingThd     (enGpId, VDP_TI_MODE_LUM, (HI_U32)pstDtiRtlPara->u16LTIOverSwingThrsh, (HI_U32)pstDtiRtlPara->u16LTIUnderSwingThrsh);
    OPTM_VDP_GP_SetTiMixRatio     (enGpId, VDP_TI_MODE_CHM, (HI_U32)pstDtiRtlPara->u8CTIMixingRatio);
    OPTM_VDP_GP_SetTiMixRatio     (enGpId, VDP_TI_MODE_LUM, (HI_U32)pstDtiRtlPara->u8LTIMixingRatio);
    OPTM_VDP_GP_SetTiHfThd        (enGpId, VDP_TI_MODE_LUM, (HI_U32 *)pstDtiRtlPara->u32LTIHFreqThrsh);
    OPTM_VDP_GP_SetTiGainCoef     (enGpId, VDP_TI_MODE_LUM, (HI_U32 *)pstDtiRtlPara->u32LTICompsatMuti);

    return;
}

static inline HI_VOID HIFB_ADP_ResetLayerInRect(OPTM_GFX_GP_E enGpId)
{
    HI_S32 s32Cnt = 0;
    HI_U32 u32LayerCount  = 0;
    HIFB_LAYER_ID_E enLayerId      = HIFB_LAYER_ID_BUTT;
    HIFB_LAYER_ID_E enInitLayerId  = HIFB_LAYER_ID_BUTT;

    u32LayerCount = (OPTM_GFX_GP_0 == enGpId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    enInitLayerId = (OPTM_GFX_GP_0 == enGpId) ? HIFB_LAYER_HD_0 : HIFB_LAYER_SD_0;

    for (s32Cnt = 0; s32Cnt < u32LayerCount; s32Cnt++)
    {
        enLayerId = enInitLayerId + s32Cnt;
        if (!g_stGfxDevice[enLayerId].bOpened)
        {
            continue;
        }
        OPTM_GfxSetLayerRect(enLayerId, &g_stGfxDevice[enLayerId].stInRect);
    }

    return;
}


HI_S32 OPTM_GfxGetGpRect(OPTM_GFX_GP_E enGpId, HIFB_RECT * pstInputRect)
{
    OPTM_VDP_DISP_RECT_S stGfxRect = {0};
    OPTM_VDP_GP_GetLayerReso(enGpId, &stGfxRect);
    pstInputRect->w = stGfxRect.u32IWth;
    pstInputRect->h = stGfxRect.u32IHgt;
    pstInputRect->x = stGfxRect.u32VX;
    pstInputRect->y = stGfxRect.u32VY;
    return HI_SUCCESS;
}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GfxSetGpDeflicker
* description   : CNcomment: 设置GP抗闪 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetGpDeflicker(OPTM_GFX_GP_E enGpId, HI_BOOL bDeflicker)
{
    HIFB_RECT stInputRect = {0};

    stInputRect.x = g_stGfxGPDevice[enGpId].stInRect.s32X;
    stInputRect.y = g_stGfxGPDevice[enGpId].stInRect.s32Y;
    stInputRect.w = g_stGfxGPDevice[enGpId].stInRect.s32Width;
    stInputRect.h = g_stGfxGPDevice[enGpId].stInRect.s32Height;

    g_stGfxGPDevice[enGpId].u32ZmeDeflicker = (HI_U32)bDeflicker;

    OPTM_GfxSetGpRect(enGpId, &stInputRect);

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GfxGetOutRect
* description   : CNcomment: 获取输出分辨率 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetOutRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT * pstOutputRect)
{
    OPTM_GFX_GP_E enGpId = g_stGfxDevice[enLayerId].enGPId;

    pstOutputRect->x = 0;
    pstOutputRect->y = 0;
    pstOutputRect->w = g_stGfxGPDevice[enGpId].stInRect.s32Width;
    pstOutputRect->h = g_stGfxGPDevice[enGpId].stInRect.s32Height;

    return HI_SUCCESS;
}
#endif

/***************************************************************************************
* func          : OPTM_GfxSetLayKeyMask
* description   : CNcomment:设置图层color key 信息CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayKeyMask(HIFB_LAYER_ID_E enLayerId, const HIFB_COLORKEYEX_S *pstColorkey)
{
    OPTM_VDP_GFX_CKEY_S ckey_info;
    OPTM_VDP_GFX_MASK_S ckey_mask;

    /** 判断该图层是否打开 **/
    D_OPTM_HIFB_CheckGfxOpen(enLayerId);

    HI_GFX_Memset(&ckey_info, 0, sizeof(ckey_info));
    HI_GFX_Memset(&ckey_mask, 0, sizeof(ckey_mask));

    HI_GFX_Memcpy(&g_stGfxDevice[enLayerId].stColorkey, pstColorkey, sizeof(HIFB_COLORKEYEX_S));

    /** 是否已经开机logo过渡完 **/
    OPTM_CheckGPMask_BYLayerID(enLayerId);

    ckey_info.bKeyMode  = pstColorkey->u32KeyMode;

    if (g_stGfxDevice[enLayerId].CscState == OPTM_CSC_SET_PARA_BGR)
    {
        ckey_info.u32Key_r_min = pstColorkey->u8BlueMin;
        ckey_info.u32Key_g_min = pstColorkey->u8GreenMin;
        ckey_info.u32Key_b_min = pstColorkey->u8RedMin;

        ckey_info.u32Key_r_max = pstColorkey->u8BlueMax;
        ckey_info.u32Key_g_max = pstColorkey->u8GreenMax;
        ckey_info.u32Key_b_max = pstColorkey->u8RedMax;

        ckey_mask.u32Mask_r = pstColorkey->u8BlueMask;
        ckey_mask.u32Mask_g = pstColorkey->u8GreenMask;
        ckey_mask.u32Mask_b = pstColorkey->u8RedMask;
    }
    else
    {
        ckey_info.u32Key_r_min = pstColorkey->u8RedMin;
        ckey_info.u32Key_g_min = pstColorkey->u8GreenMin;
        ckey_info.u32Key_b_min = pstColorkey->u8BlueMin;

        ckey_info.u32Key_r_max = pstColorkey->u8RedMax;
        ckey_info.u32Key_g_max = pstColorkey->u8GreenMax;
        ckey_info.u32Key_b_max = pstColorkey->u8BlueMax;

        ckey_mask.u32Mask_r = pstColorkey->u8RedMask;
        ckey_mask.u32Mask_g = pstColorkey->u8GreenMask;
        ckey_mask.u32Mask_b = pstColorkey->u8BlueMask;
    }

    /**
     ** set key threshold / mode / enable of graphic layer
     **/
    OPTM_VDP_GFX_SetKeyMask(g_stGfxDevice[enLayerId].enGfxHalId, ckey_mask);
    OPTM_VDP_GFX_SetColorKey(g_stGfxDevice[enLayerId].enGfxHalId, pstColorkey->bKeyEnable, ckey_info);
    /** 配置完要更新寄存器 **/
    OPTM_VDP_GFX_SetRegUp(g_stGfxDevice[enLayerId].enGfxHalId);

    return HI_SUCCESS;

}


/***************************************************************************************
* func          : OPTM_GfxSetLayerPreMult
* description   : CNcomment: 设置图层预乘 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerPreMult(HIFB_LAYER_ID_E enLayerId, HI_BOOL bEnable)
{
    OPTM_GFX_GP_E enGpId  = g_stGfxDevice[enLayerId].enGPId;

    g_stGfxDevice[enLayerId].bPreMute = bEnable;

    OPTM_CheckGPMask_BYGPID(enGpId);

    OPTM_VDP_GFX_SetPreMultEnable(g_stGfxDevice[enLayerId].enGfxHalId, bEnable);

    return HI_SUCCESS;
}


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GfxGetOSDData
* description   : CNcomment: 获取对应图层得OSD数据 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GfxGetOSDData(HIFB_LAYER_ID_E enLayerId, HIFB_OSD_DATA_S *pstLayerData)
{
    HI_U32 u32Enable     = HI_FALSE;
    HI_U32 u32KeyEnable  = HI_FALSE;
    HI_U32 alpharange    = 0;
    OPTM_GFX_GP_E enGPId = OPTM_GFX_GP_BUTT;
    OPTM_VDP_DISP_RECT_S  stRect     = {0};
    OPTM_VDP_DISP_RECT_S stInRect    = {0};
    OPTM_VDP_GFX_MASK_S  stckey_mask = {0};
    OPTM_VDP_GFX_CKEY_S  stKey       = {0};
    OPTM_VDP_GFX_IFMT_E  enDataFmt   = VDP_GFX_IFMT_BUTT;

    if(enLayerId < HIFB_LAYER_SD_0){
        enGPId = OPTM_GFX_GP_0;
    }else{
        enGPId = OPTM_GFX_GP_1;
    }

    OPTM_VDP_GFX_GetLayerEnable(OPTM_GetGfxHalId(enLayerId), &u32Enable);

    if(u32Enable){
        pstLayerData->eState = HIFB_LAYER_STATE_ENABLE;
    }else{
        pstLayerData->eState = HIFB_LAYER_STATE_DISABLE;
    }

    /** 获取surface 帧buffer地址 **/
    OPTM_VDP_GFX_GetLayerAddr(OPTM_GetGfxHalId(enLayerId), &pstLayerData->u32RegPhyAddr);

    #ifdef CFG_HIFB_COMPRESSION_SUPPORT
        if (g_stGfxDevice[enLayerId].bCmpOpened){
            pstLayerData->u32RegPhyAddr = g_stGfxDevice[enLayerId].NoCmpBufAddr;
        }
    #endif

    OPTM_VDP_GFX_GetLayerStride(OPTM_GetGfxHalId(enLayerId), &pstLayerData->u32Stride);

    OPTM_VDP_GFX_GetLayerInRect(OPTM_GetGfxHalId(enLayerId), &stInRect);
    pstLayerData->stInRect.x = stInRect.u32DXS;
    pstLayerData->stInRect.y = stInRect.u32DYS;
    pstLayerData->stInRect.w = stInRect.u32IWth;
    pstLayerData->stInRect.h = stInRect.u32IHgt;

    OPTM_VDP_GP_GetRect(enGPId, &stRect);

    pstLayerData->stOutRect.x = 0;
    pstLayerData->stOutRect.y = 0;
    pstLayerData->stOutRect.w = stRect.u32IWth;
    pstLayerData->stOutRect.h = stRect.u32IHgt;

    pstLayerData->u32ScreenWidth = stRect.u32OWth;
    pstLayerData->u32ScreenHeight= stRect.u32OHgt;

    /** question **/
    OPTM_VDP_WBC_GetEnable(OPTM_VDP_LAYER_WBC_GP0, &u32Enable);
    if (u32Enable){
    /**同源**/
        pstLayerData->eGfxWorkMode = HIFB_GFX_MODE_HD_WBC;
        pstLayerData->enSlaveryLayerID = OPTM_SLAVER_LAYERID;
    }else{/**非同源**/
        pstLayerData->eGfxWorkMode = HIFB_GFX_MODE_NORMAL;
        pstLayerData->enSlaveryLayerID = HIFB_LAYER_ID_BUTT;
    }

    OPTM_VDP_GFX_GetPalpha( OPTM_GetGfxHalId(enLayerId),               \
                           &pstLayerData->stAlpha.bAlphaEnable,        \
                           &alpharange,&pstLayerData->stAlpha.u8Alpha0,\
                           &pstLayerData->stAlpha.u8Alpha1);

    OPTM_VDP_GFX_GetLayerGalpha(OPTM_GetGfxHalId(enLayerId),     \
                                &pstLayerData->stAlpha.u8GlobalAlpha);

    OPTM_VDP_GFX_GetKeyMask (OPTM_GetGfxHalId(enLayerId), &stckey_mask);
    OPTM_VDP_GFX_GetColorKey(OPTM_GetGfxHalId(enLayerId), &u32KeyEnable,&stKey);

    pstLayerData->stColorKey.u8RedMask   = stckey_mask .u32Mask_r;
    pstLayerData->stColorKey.u8GreenMask = stckey_mask .u32Mask_g;
    pstLayerData->stColorKey.u8BlueMask  = stckey_mask .u32Mask_b;

    pstLayerData->stColorKey.bMaskEnable = HI_TRUE;
    pstLayerData->stColorKey.bKeyEnable  = u32KeyEnable;
    pstLayerData->stColorKey.u32KeyMode  = stKey.bKeyMode;

    pstLayerData->stColorKey.u8RedMax    = stKey.u32Key_r_max;
    pstLayerData->stColorKey.u8GreenMax  = stKey.u32Key_g_max;
    pstLayerData->stColorKey.u8BlueMax   = stKey.u32Key_b_max;

    pstLayerData->stColorKey.u8RedMin    = stKey.u32Key_r_min;
    pstLayerData->stColorKey.u8GreenMin  = stKey.u32Key_g_min;
    pstLayerData->stColorKey.u8BlueMin   = stKey.u32Key_b_min;

    OPTM_VDP_GFX_GetPreMultEnable(OPTM_GetGfxHalId(enLayerId), &pstLayerData->bPreMul);

    OPTM_VDP_GFX_GetInDataFmt(OPTM_GetGfxHalId(enLayerId), &enDataFmt);

    pstLayerData->eFmt = OPTM_HalFmtTransferToPixerFmt(enDataFmt);

}


HI_VOID HIFB_ADP_GetLogoData(HIFB_LAYER_ID_E enLayerId, HIFB_LOGO_DATA_S *pstLogoData)
{
     HI_S32 Ret = HI_SUCCESS;
     HI_U32 LayerEnable = 0;
     OPTM_VDP_GFX_IFMT_E  enDataFmt   = VDP_GFX_IFMT_BUTT;
     HIFB_COLOR_FMT_E enFmt = HIFB_FMT_BUTT;
     DISP_EXPORT_FUNC_S *DispExportFuncs = NULL;
     HI_DISP_DISPLAY_INFO_S stDispInfo;
     HI_DRV_DISPLAY_E enDisp;
     HI_RECT_S stOutRect = {0};
     OPTM_VDP_DISP_RECT_S stInRect = {0};

     if (enLayerId < HIFB_LAYER_SD_0)
     {
        enDisp = HI_DRV_DISPLAY_1;
     }
     else
     {
        enDisp = HI_DRV_DISPLAY_0;
     }

     HI_GFX_Memset(&stDispInfo,0x0,sizeof(HI_DISP_DISPLAY_INFO_S));
     Ret = HI_DRV_MODULE_GetFunction(HI_ID_DISP, (HI_VOID**)&DispExportFuncs);
     if ( (HI_SUCCESS != Ret) || (NULL == DispExportFuncs))
     {
         HIFB_ERROR("disp export funcs get failure\n");
         return;
     }

     Ret = DispExportFuncs->pfnDispGetDispInfo(enDisp,&stDispInfo);
     if (HI_SUCCESS != Ret)
     {
         HIFB_ERROR("pfnDispGetDispInfo get failure\n");
         return;
     }
     OPTM_GetScreenRectFromDispInfo(&stDispInfo.stVirtaulScreen, (OPTM_GFX_OFFSET_S *)&stDispInfo.stOffsetInfo,
                                    &stDispInfo.stFmtResolution,&stDispInfo.stPixelFmtResolution,
                                    &stOutRect);

     OPTM_VDP_GFX_GetLayerInRect (OPTM_GetGfxHalId(enLayerId), &stInRect);
     OPTM_VDP_GFX_GetLayerEnable (OPTM_GetGfxHalId(enLayerId), &LayerEnable);
     OPTM_VDP_GFX_GetLayerAddr   (OPTM_GetGfxHalId(enLayerId), &pstLogoData->LogoYAddr);
     OPTM_VDP_GFX_GetInDataFmt   (OPTM_GetGfxHalId(enLayerId), &enDataFmt);
     enFmt = OPTM_HalFmtTransferToPixerFmt(enDataFmt);

     if (   (CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH == stInRect.u32IWth || CONFIG_GFX_DISP_4K_VIRTUAL_WIDTH == stInRect.u32IWth)
         && (CONFIG_GFX_DISP_REALLY4K_VIRTUAL_WIDTH == stDispInfo.stFmtResolution.s32Width || CONFIG_GFX_DISP_4K_VIRTUAL_WIDTH == stDispInfo.stFmtResolution.s32Width)
         && (CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT == stInRect.u32IHgt)
         && (CONFIG_GFX_DISP_4K_VIRTUAL_HEIGHT == stDispInfo.stFmtResolution.s32Height))
     {
         pstLogoData->Support4KLogo = HI_TRUE;
     }

     pstLogoData->LogoEnable     = (1 == LayerEnable) ? (HI_TRUE) : (HI_FALSE);
     pstLogoData->LogoCbCrAddr   = 0x0;

     if ((enLayerId < HIFB_LAYER_SD_0) && (HI_FALSE == pstLogoData->Support4KLogo))
     {
        pstLogoData->LogoWidth      = stDispInfo.stVirtaulScreen.s32Width;
        pstLogoData->LogoHeight     = stDispInfo.stVirtaulScreen.s32Height;
     }
     else
     {
        pstLogoData->LogoWidth      = stDispInfo.stFmtResolution.s32Width;
        pstLogoData->LogoHeight     = stDispInfo.stFmtResolution.s32Height;
     }
     pstLogoData->stOutRect.x    = stOutRect.s32X;
     pstLogoData->stOutRect.y    = stOutRect.s32Y;
     pstLogoData->stOutRect.w    = stOutRect.s32Width;
     pstLogoData->stOutRect.h    = stOutRect.s32Height;
     pstLogoData->eLogoPixFmt    = HIFB_LOGO_PIX_FMT_ARGB8888;
     pstLogoData->LogoYStride    = pstLogoData->LogoWidth * 4;
     if (HIFB_FMT_ARGB1555 == enFmt)
     {
        pstLogoData->eLogoPixFmt = HIFB_LOGO_PIX_FMT_ARGB1555;
        pstLogoData->LogoYStride = pstLogoData->LogoWidth * 2;
     }
     return;
}
#endif



/***************************************************************************************
* func          : OPTM_GfxUpLayerReg
* description   : CNcomment: 更新寄存器 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxUpLayerReg(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_GFX_GP_E enGpId = g_stGfxDevice[enLayerId].enGPId;
    /**
     **if the para has not setted finish,do not updata the register
     **如果相关寄存器没有配置完，不更新寄存器使相关配置暂时不生效
     **/
    OPTM_CheckGPMask_BYGPID(enGpId);

    OPTM_VDP_GFX_SetRegUp(g_stGfxDevice[enLayerId].enGfxHalId);
    OPTM_VDP_GP_SetRegUp (g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].enGpHalId);

    if(IS_MASTER_GP(enGpId)){
        HIFB_WBC2_Reset(HI_TRUE);
    }

    return HI_SUCCESS;
}


#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
/***************************************************************************************
* func          : OPTM_GfxSetTriDimEnable
* description   : CNcomment: 设置3D使能 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetTriDimEnable(HIFB_LAYER_ID_E enLayerId, HI_U32 bEnable)
{
    /**
     **保存图层的3D使能状态
     **/
    g_stGfxDevice[enLayerId].b3DEnable = bEnable;

    OPTM_CheckGPMask_BYLayerID(enLayerId);

    OPTM_VDP_GFX_SetThreeDimEnable(enLayerId, bEnable);

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GfxGetHalTriDimMode
* description   : CNcomment: 获取3D模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
OPTM_VDP_DISP_MODE_E OPTM_GfxGetHalTriDimMode(HIFB_STEREO_MODE_E enMode)
{
    switch(enMode)
    {
        case HIFB_STEREO_MONO:
            return VDP_DISP_MODE_2D;
        case HIFB_STEREO_SIDEBYSIDE_HALF:
            return VDP_DISP_MODE_SBS;
        case HIFB_STEREO_TOPANDBOTTOM:
            return VDP_DISP_MODE_TAB;
        case HIFB_STEREO_FRMPACKING:
            return VDP_DISP_MODE_FP;
        default:
            return VDP_DISP_MODE_BUTT;
    }

    return VDP_DISP_MODE_BUTT;
}


/***************************************************************************************
* func          : OPTM_GfxSetTriDimMode
* description   : CNcomment: 设置3D模式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetTriDimMode(HIFB_LAYER_ID_E enLayerId, HIFB_STEREO_MODE_E enMode, HIFB_STEREO_MODE_E enWbcSteroMode)
{
    HI_U32 u32GpId = (HI_U32)(g_stGfxDevice[enLayerId].enGPId);

    if(u32GpId >= (HI_U32)OPTM_GFX_GP_BUTT){
        return HI_FAILURE;
    }

    /**
     **保存GP和图层的3D模式
     **/
    g_stGfxDevice[enLayerId].enTriDimMode = enMode;
    g_stGfxGPDevice[u32GpId].enTriDimMode = enMode;

    OPTM_CheckGPMask_BYGPID(u32GpId);

    OPTM_VDP_GP_SetRegUp(g_stGfxDevice[enLayerId].enGPId);
    /**
     ** WBC_GP_INRECT == GP_INRECT
     **/
    if(IS_MASTER_GP(u32GpId)){
        g_stGfxGPDevice[OPTM_SLAVER_GPID].enTriDimMode = enWbcSteroMode;
        g_stGfxGPDevice[OPTM_SLAVER_GPID].stInRect.s32Width  = g_stGfxGPDevice[u32GpId].stInRect.s32Width;
        g_stGfxGPDevice[OPTM_SLAVER_GPID].stInRect.s32Height = g_stGfxGPDevice[u32GpId].stInRect.s32Height;

        if(HIFB_STEREO_MONO == enWbcSteroMode){
            g_stGfxWbc2.enWbcMode = OPTM_WBC_MODE_MONO;
        }else{
            g_stGfxWbc2.enWbcMode = OPTM_WBC_MODE_LFET_EYE;
        }
        /** 3d回写使能 **/
        g_stGfxGPDevice[OPTM_SLAVER_GPID].unUpFlag.bits.WbcMode          = 1;
        g_stGfxGPDevice[OPTM_SLAVER_GPID].unUpFlag.bits.IsNeedUpInRect   = 1;
        HIFB_WBC2_Reset(HI_TRUE);
    }

    return HI_SUCCESS;

}

/***************************************************************************************
* func            : OPTM_GfxSetTriDimAddr
* description    : CNcomment: 设置3D显示地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetTriDimAddr(HIFB_LAYER_ID_E enLayerId, HI_U32 u32TriDimAddr)
{

    g_stGfxDevice[enLayerId].u32TriDimAddr= u32TriDimAddr;

    OPTM_CheckGPMask_BYLayerID(enLayerId);

    OPTM_VDP_GFX_SetLayerNAddr(enLayerId, u32TriDimAddr);

    return HI_SUCCESS;
}
#endif



#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GfxGetLayerPriority
* description   : CNcomment: 获取图层在GP 中的优先级 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxGetLayerPriority(HIFB_LAYER_ID_E enLayerId, HI_U32 *pU32Priority)
{
    HI_U32 i            = 0;
    HI_U32 u32prio      = 0;
    HI_U32 u32Index     = 0;
    HI_U32 u32LayerPrio = 0;
    HI_U32 u32LayerIdIndex = 0;
    OPTM_VDP_CBM_MIX_E eCbmMixg;

    u32Index = (g_stGfxDevice[enLayerId].enGPId == OPTM_GFX_GP_0) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    eCbmMixg = (g_stGfxDevice[enLayerId].enGPId == OPTM_GFX_GP_0) ? VDP_CBM_MIXG0 : VDP_CBM_MIXG1;
    OPTM_VDP_CBM_GetMixerPrio(eCbmMixg, &u32prio);

    /**
     ** HIFB_LAYER_HD_0 -> 0
     ** HIFB_LAYER_HD_1 -> 1
     ** HIFB_LAYER_SD_0 -> 0
     ** HIFB_LAYER_SD_1 -> 1
     **/
    if(HIFB_LAYER_HD_2 == enLayerId){
        u32LayerIdIndex = 3; /** G3 **/
    }
    else{
        u32LayerIdIndex =  enLayerId - OPTM_GP0_GFX_COUNT * g_stGfxDevice[enLayerId].enGPId;
    }
    for(i = 0; i < u32Index; i++){
        u32LayerPrio = u32prio & 0xf;
        u32prio      = u32prio >> 4;
        if((u32LayerPrio-1) == u32LayerIdIndex){
            *pU32Priority = i+1;
            return HI_SUCCESS;
        }
    }
    HIFB_ERROR("info:fail to get z_order of gfx%d!\n",enLayerId);
    return HI_FAILURE;

}

/***************************************************************************************
* func          : OPTM_GfxSetLayerPriority
* description   : CNcomment: 设置图层在GP 中的优先级 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GfxSetLayerPriority(HIFB_LAYER_ID_E enLayerId, HIFB_ZORDER_E enZOrder)
{
    HI_U32  u32Prio        = 0;
    HI_U32  u32LayerPrio   = 0;
    HI_U32  u32MaskUpBit   = 0;
    HI_U32  u32MaskDownBit = 0;
    HI_U32  u32SwitchBit   = 0;
    HI_U32  u32MaskTopBit  = 0;
    HI_U32  u32MaskBit     = 0;
    HI_U32  u32LayerCount  = 0;
    HI_S32  count          = 0;
    OPTM_VDP_CBM_MIX_E eCbmMixg = VDP_CBM_MIX_BUTT;

    if (HI_SUCCESS != OPTM_GfxGetLayerPriority(enLayerId, &u32LayerPrio)){
        HIFB_ERROR("info:fail to set z_order of gfx%d!\n",enLayerId);
        return HI_FAILURE;
    }

    eCbmMixg = (g_stGfxDevice[enLayerId].enGPId == OPTM_GFX_GP_0) ? VDP_CBM_MIXG0 : VDP_CBM_MIXG1;
    OPTM_VDP_CBM_GetMixerPrio(eCbmMixg, &u32Prio);
    u32LayerCount = (g_stGfxDevice[enLayerId].enGPId == OPTM_GFX_GP_0) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;

    u32SwitchBit = (u32Prio >> (4*(u32LayerPrio-1))) & 0xf;

    for(count = u32LayerPrio; count > 0; count--)
    {
        u32MaskUpBit = 0xf | (u32MaskUpBit << 4);
    }

    for(count = u32LayerPrio; count <= u32LayerCount; count++)
    {
        u32MaskDownBit = 0xf | (u32MaskDownBit << 4);
    }
    u32MaskDownBit <<= 4*(u32LayerPrio-1);

    u32MaskBit = 0xf;
    u32MaskTopBit = 0xf;
    for(count = 1; count < u32LayerCount; count++)
    {
        u32MaskBit = 0xf | (u32MaskBit << 4);
        u32MaskTopBit = u32MaskTopBit << 4;
    }

    u32Prio &= u32MaskBit;
    switch (enZOrder)
    {
        case HIFB_ZORDER_MOVEBOTTOM:
        {
            if (u32LayerPrio == 1)
            {
               return HI_SUCCESS;
            }
            /*当前layer之上的Z序值保持不变|当前layer之下的Z序值上移|当前layer Z序值*/
            u32Prio = (u32Prio&(~u32MaskUpBit)) | \
                      ((u32MaskUpBit&((u32Prio&u32MaskUpBit)<<4))&(~0xf)) |
                      u32SwitchBit;
            break;
        }
        case HIFB_ZORDER_MOVEDOWN:
        {
            if (u32LayerPrio == 1)
            {
               return HI_SUCCESS;
            }

            u32Prio = ((u32Prio&(0xf<<4*(u32LayerPrio-1)))>>4) | \
                      ((u32Prio&(0xf<<4*(u32LayerPrio-2)))<<4) | \
                       (~((0xf<<4*(u32LayerPrio-1))|(0xf<<4*(u32LayerPrio-2)))&u32Prio);
            break;
        }
        case HIFB_ZORDER_MOVETOP:
        {
            if (u32LayerPrio == u32LayerCount)
            {
               return HI_SUCCESS;
            }
            /*当前layer之下的Z序值保持不变|当前layer之上的Z序值下移|当前layer Z序值*/
            u32Prio = (u32Prio&(~u32MaskDownBit)) | \
                      ((u32MaskDownBit&((u32Prio&u32MaskDownBit)>>4))&(~u32MaskTopBit)) | \
                      (u32SwitchBit<<(4*(u32LayerCount-1)));
            break;
        }
        case HIFB_ZORDER_MOVEUP:
        {

            if (u32LayerPrio == u32LayerCount)
            {
               return HI_SUCCESS;
            }
            u32Prio = ((u32Prio&(0xf<<4*(u32LayerPrio-1)))<<4) | \
                      ((u32Prio&(0xf<<4*u32LayerPrio))>>4) | \
                       (~((0xf<<4*(u32LayerPrio-1))|(0xf<<4*u32LayerPrio))&u32Prio);
            break;
        }
        default:
        {
            return HI_FAILURE;
        }
    }

    u32Prio &= u32MaskBit;

    OPTM_VDP_SetMixgPrio(eCbmMixg, u32Prio);

    g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].u32Prior = u32Prio;


    return HI_SUCCESS;

}
#endif


/***************************************************************************************
* func          : OPTM_DispInfoUpdate
* description   : CNcomment: 更新display信息 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_DispInfoUpdate(OPTM_VDP_LAYER_GP_E enGPId)
{
    HI_U32           i;
    HI_U32           u32LayerCount;
    HI_RECT_S        *pstDispRect;
    HIFB_LAYER_ID_E  enInitLayerId;
    HIFB_LAYER_ID_E  enLayerId;

    if(!g_stGfxGPDevice[enGPId].bOpen){
        return HI_SUCCESS;
    }

    OPTM_CheckGPMask_BYGPID(enGPId);

    u32LayerCount = (OPTM_VDP_LAYER_GP0 == enGPId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    enInitLayerId = (OPTM_VDP_LAYER_GP0 == enGPId) ? HIFB_LAYER_HD_0 : HIFB_LAYER_SD_0;

    pstDispRect = &g_stGfxGPDevice[enGPId].stOutRect;

    if(IS_SLAVER_GP((OPTM_GFX_GP_E)enGPId)){
        OPTM_GPSetCsc(enGPId, &g_stGfxGPDevice[enGPId].stCscPara,g_stGfxGPDevice[enGPId].bBGRState);
        HIFB_WBC2_CfgSlvLayer(HIFB_LAYER_SD_0, pstDispRect);
    }else{
        /*when recovery gp , set disp size to hardware first*/
        OPTM_GfxSetDispFMTSize((OPTM_GFX_GP_E)enGPId, pstDispRect);
        OPTM_GPSetCsc(enGPId, &g_stGfxGPDevice[enGPId].stCscPara,g_stGfxGPDevice[enGPId].bBGRState);

        OPTM_VDP_GP_SetRegUp   (enGPId);

        /**
         ** recover all gfx in the gp
         **/
        for (i = 0; i < u32LayerCount;i++)
        {
            enLayerId = enInitLayerId + i;

            if(!g_stGfxDevice[enLayerId].bOpened){
                continue;
            }

            OPTM_GfxSetLayerRect(enLayerId, &g_stGfxDevice[enLayerId].stInRect);
            OPTM_VDP_GFX_SetUpdMode (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enUpDateMode);
        }
    }

    return HI_SUCCESS;
}



/***************************************************************************************
* func          : OPTM_GPRecovery
* description   : CNcomment: 重新设置GP属性 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GPRecovery(OPTM_GFX_GP_E enGPId)
{
    HI_U32           i             = 0;
    HI_U32           u32LayerCount = 0;
    HI_RECT_S        *pstDispRect  = &g_stGfxGPDevice[enGPId].stOutRect;
    HIFB_LAYER_ID_E  enInitLayerId = HIFB_LAYER_HD_0;
    HIFB_LAYER_ID_E  enLayerId     = HIFB_LAYER_HD_0;

    if(!g_stGfxGPDevice[enGPId].bOpen){
        return HI_SUCCESS;
    }

    /**
     ** check whether should reset the gp property,false should reset
     **/
    OPTM_CheckGPMask_BYGPID(enGPId);

    /**
     **check reset which layer
     **/
    u32LayerCount   = (OPTM_GFX_GP_0 == enGPId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    enInitLayerId   = (OPTM_GFX_GP_0 == enGPId) ? HIFB_LAYER_HD_0    : HIFB_LAYER_SD_0;

    /**
     **GP0 1920 * 1080
     **GP1 720  * 576
     **/
    OPTM_VDP_CBM_SetMixerBkg    (g_stGfxGPDevice[enGPId].enMixg, g_stGfxGPDevice[enGPId].stBkg);
    OPTM_VDP_SetMixgPrio        (g_stGfxGPDevice[enGPId].enMixg, g_stGfxGPDevice[enGPId].u32Prior);
    OPTM_VDP_GP_SetLayerGalpha  (enGPId, g_stGfxGPDevice[enGPId].u32Alpha);
    OPTM_GPSetCsc               (enGPId, &g_stGfxGPDevice[enGPId].stCscPara,g_stGfxGPDevice[enGPId].bBGRState);

    if(IS_SLAVER_GP(enGPId)){
        HIFB_WBC2_Recovery(enGPId);
    }else{
        OPTM_GfxSetDispFMTSize (enGPId, pstDispRect);
        OPTM_VDP_GP_SetRegUp   (enGPId);
        /**
         **recover all gfx in the gp
         **/
        for(i = 0; i < u32LayerCount;i++)
        {
            enLayerId = enInitLayerId + i;
            if(!g_stGfxDevice[enLayerId].bOpened){
                /**
                 ** Close graphics layer if the state record is not the same with hardware.
                 ** scene:quickboot,close logo here if logo exist!
                 **/
                OPTM_VDP_GFX_SetLayerEnable (g_stGfxDevice[enLayerId].enGfxHalId, HI_FALSE);
                OPTM_VDP_GFX_SetRegUp       (g_stGfxDevice[enLayerId].enGfxHalId);
                continue;
            }

            OPTM_GfxSetEnable         (enLayerId, g_stGfxDevice[enLayerId].bEnable);
            OPTM_GfxSetLayerAlpha     (enLayerId, &g_stGfxDevice[enLayerId].stAlpha);
            OPTM_GfxSetLayKeyMask     (enLayerId, &g_stGfxDevice[enLayerId].stColorkey);

            #ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
            OPTM_GfxSetTriDimEnable   (enLayerId, g_stGfxDevice[enLayerId].b3DEnable);
            OPTM_GfxSetTriDimMode     (enLayerId, g_stGfxDevice[enLayerId].enTriDimMode, g_stGfxGPDevice[OPTM_SLAVER_GPID].enTriDimMode);
            OPTM_GfxSetTriDimAddr     (enLayerId, g_stGfxDevice[enLayerId].u32TriDimAddr);
            OPTM_GFX_SetStereoDepth   (enLayerId, g_stGfxDevice[enLayerId].s32Depth);
            #endif

            OPTM_GfxSetLayerPreMult   (enLayerId, g_stGfxDevice[enLayerId].bPreMute);
            OPTM_GfxSetLayerDataFmt   (enLayerId, g_stGfxDevice[enLayerId].enDataFmt);
            OPTM_GfxSetLayerStride    (enLayerId, g_stGfxDevice[enLayerId].Stride);
            OPTM_GfxSetLayerRect      (enLayerId, &g_stGfxDevice[enLayerId].stInRect);
            OPTM_GfxSetLayerAddr      (enLayerId, g_stGfxDevice[enLayerId].NoCmpBufAddr);

            OPTM_VDP_GFX_SetLutAddr   (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].stCluptTable.u32StartPhyAddr);
            OPTM_VDP_GFX_SetParaUpd   (g_stGfxDevice[enLayerId].enGfxHalId, VDP_DISP_COEFMODE_LUT);
            OPTM_VDP_GFX_SetLayerBkg  (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].stBkg);
            OPTM_VDP_GFX_SetBitExtend (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enBitExtend);

            #ifdef CONFIG_HIFB_GFX3_TO_GFX5
                /*gfx3 have no zme*/
                if(OPTM_VDP_LAYER_GFX3 == g_stGfxDevice[enLayerId].enGfxHalId){
                    g_stGfxDevice[enLayerId].enReadMode = g_stGfxGPDevice[enGPId].bInterface ? VDP_RMODE_SELF_ADAPTION : VDP_RMODE_PROGRESSIVE;
                }
            #endif
            OPTM_GfxSetLayerReadMode(enLayerId, g_stGfxDevice[enLayerId].enReadMode);
            OPTM_VDP_GFX_SetUpdMode (g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].enUpDateMode);
       }
    }

    return HI_SUCCESS;
}

#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_VO_Callback
* description   : CNcomment: VO CALLBACK CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_VO_Callback(HI_VOID* u32Param0, HI_VOID* u32Param1)
{
    HI_U32                      i;
    HI_U32                      u32CTypeFlag;
    HI_U32                      u32LayerCount;
    HIFB_LAYER_ID_E             enInitLayerId;
    OPTM_VDP_LAYER_GP_E         *pEnGpHalId  = (OPTM_VDP_LAYER_GP_E *)u32Param0;
    HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo = (HI_DRV_DISP_CALLBACK_INFO_S *)u32Param1;

    if ((NULL == pEnGpHalId) || (NULL == pstDispInfo))
    {
        return HI_FAILURE;
    }

    u32LayerCount = (OPTM_VDP_LAYER_GP0 == *pEnGpHalId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;
    enInitLayerId = (OPTM_VDP_LAYER_GP0 == *pEnGpHalId) ? HIFB_LAYER_HD_0 : HIFB_LAYER_SD_0;

    for (i = 0; i < u32LayerCount;i++)
    {
        u32CTypeFlag = g_stGfxGPIrq[*pEnGpHalId].stGfxCallBack[i].u32CTypeFlag;
        if (HIFB_CALLBACK_TYPE_VO != (u32CTypeFlag & HIFB_CALLBACK_TYPE_VO))
        {
           continue;
        }

        if (NULL != g_stGfxGPIrq[*pEnGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_VO].pFunc)
        {
            g_stGfxGPIrq[*pEnGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_VO].pFunc(
                g_stGfxGPIrq[*pEnGpHalId].stGfxCallBack[i].stGfxIrq[HIFB_CALLBACK_TYPE_VO].pParam0,HI_NULL);
        }

#ifdef OPTM_HIFB_WVM_ENABLE
        OPTM_GfxWVBCallBack(enInitLayerId + i, HI_NULL);
#endif
    }

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : OPTM_Distribute_Callback
* description   : CNcomment: 分发中断处理类型 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_Distribute_Callback(HI_VOID* u32Param0, HI_VOID* u32Param1)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    HIFB_STEREO_MODE_E enTriDimMode;
#endif
    OPTM_GFX_GP_E      *penGpId  = (OPTM_GFX_GP_E *)u32Param0;
    HI_DRV_DISP_CALLBACK_INFO_S *pstDispInfo = (HI_DRV_DISP_CALLBACK_INFO_S *)u32Param1;

    if(penGpId == HI_NULL || pstDispInfo == HI_NULL){
        return HI_FAILURE;
    }

    OPTM_VO_Callback(u32Param0, u32Param1);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    /**
     ** 3D Mode change
     **/
    enTriDimMode = OPTM_AdaptTriDimModeFromDisp(OPTM_DISP_GetDispMode(g_stGfxGPDevice[*penGpId].enGpHalId));
    if((enTriDimMode != g_stGfxGPDevice[*penGpId].enTriDimMode) && (HIFB_STEREO_BUTT > enTriDimMode))
    {
        if (OPTM_CheckGfxCallbackReg((OPTM_GFX_GP_E)(*penGpId), HIFB_CALLBACK_TYPE_3DMode_CHG) != HI_SUCCESS)
        {
           return HI_SUCCESS;
        }

        g_stGfxGPDevice[*penGpId].enTriDimMode = enTriDimMode;

        OPTM_VDP_GP_SetRegUp   (*penGpId);

        g_stGfxGPDevice[*penGpId].st3DModeChgWork.u32Data = *penGpId;

        /**
         **OPTM_3DMode_Callback
         **/
        queue_work(g_stGfxGPDevice[*penGpId].queue, &g_stGfxGPDevice[*penGpId].st3DModeChgWork.work);
    }
#endif

    return HI_SUCCESS;
}
#endif


#ifndef HI_BUILD_IN_BOOT
/***************************************************************************************
* func          : OPTM_GFX_SetGPMask
* description   : CNcomment: 这个接口只有开机logo才会用到，主要是用来过渡使用的
                             设置GP0和GP1掩码，并且在logo过渡完之后重新设置GP CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_SetGPMask(OPTM_GFX_GP_E enGpId, HI_BOOL bFlag)
{
    HIFB_OSD_DATA_S stLayerData;

    g_stGfxGPDevice[enGpId].bMaskFlag = bFlag;
    /**
     ** when logo is clear, should updata gp info, use app gp info
     ** 如果logo已经清除，就需要配置用户的信息，否则会一直保留着logo
     ** 的配置，除非状态发生变化才会重新跟新GP
     **/
    if(!bFlag){
        OPTM_GPRecovery(enGpId);
    }

    if(OPTM_MASTER_GPID == enGpId){
        OPTM_GfxGetOSDData(HIFB_LAYER_SD_0, &stLayerData);
        if(IS_MASTER_GP(enGpId) || HIFB_LAYER_STATE_ENABLE == stLayerData.eState){
            g_stGfxGPDevice[OPTM_SLAVER_GPID].bMaskFlag = bFlag;
            if(!bFlag && g_stGfxWbc2.IsOpen){
                OPTM_GPRecovery(OPTM_SLAVER_GPID);
            }
        }
    }
    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GFX_GetGfxMask
* description   : CNcomment: 获取GP设置掩码 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_GetGfxMask(OPTM_GFX_GP_E enGpId)
{
    return g_stGfxGPDevice[enGpId].bMaskFlag;
}


/***************************************************************************************
* func          : OPTM_GFX_ClearLogoOsd
* description   : CNcomment: 清logo CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_ClearLogoOsd(HIFB_LAYER_ID_E enLayerId)
{
    UNUSED(enLayerId);
    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_GFX_SetStereoDepth
* description   : CNcomment: 设置3D景深，与人眼的距离 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_SetStereoDepth(HIFB_LAYER_ID_E enLayerId, HI_S32 s32Depth)
{
    g_stGfxDevice[enLayerId].s32Depth = s32Depth;

    if(g_stGfxDevice[enLayerId].s32Depth != 0){
        OPTM_VDP_GFX_SetThreeDimDofEnable(OPTM_GetGfxHalId(enLayerId), HI_TRUE);
    }

    OPTM_VDP_GFX_SetThreeDimDofStep(OPTM_GetGfxHalId(enLayerId), s32Depth, (0-s32Depth));

    return HI_SUCCESS;
}
#endif

/***************************************************************************************
* func          : OPTM_GFX_SetTCFlag
* description   : CNcomment: 设置是不是TC使用场景标识 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_SetTCFlag(HI_BOOL bFlag)
{
    HIFB_WBC2_SetTcFlag(bFlag);
    return HI_SUCCESS;
}

#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(HI_BUILD_IN_BOOT)
HI_VOID OPTM_GFX_DECMP_Open(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_GFX_SetDcmpEnable(enLayerId,HI_TRUE);
}

HI_VOID OPTM_GFX_DECMP_Close(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_GFX_SetDcmpEnable(enLayerId,HI_FALSE);
}
#endif

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
HI_S32 OPTM_GFX_CMP_ResChg(HIFB_LAYER_ID_E enLayerId)
{
    HI_BOOL bUpdate;
    HI_U32 u32Width, u32Height, u32Stride,u32HeadSize,u32BufSize,u32HeadStride;
    OPTM_GFX_CMP_S *pstCmpInfo;
    OPTM_CMP_DATA_INFO_S *pstDataInfo;

    bUpdate   = HI_FALSE;
    u32Width  = g_stGfxDevice[enLayerId].stInRect.w;
    u32Height = g_stGfxDevice[enLayerId].stInRect.h;
    u32HeadStride = (u32Width > OPTM_COMPRESSION_WIDTH_MAX) ? OPTM_COMPRESSION_HEAD_STRIDE_32 : OPTM_COMPRESSION_HEAD_STRIDE_16;
    u32Stride = (g_stGfxDevice[enLayerId].Stride/2 + OPTM_COMPRESSION_ALIGNMENT-1)&(~(OPTM_COMPRESSION_ALIGNMENT-1));
    u32HeadSize = u32HeadStride*u32Height;
    u32BufSize  = u32Height*u32Stride + u32HeadSize;

    pstCmpInfo = &(g_stGfxDevice[enLayerId].stCmpInfo);
    pstDataInfo= &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo);

    if ((u32BufSize*2) > pstDataInfo->u32CMPBufSize)
    {
        pstCmpInfo->enStatus = OPTM_CMP_STATUS_PARALYSED;
        pstCmpInfo->bCMPQueueWorkStart = HI_TRUE;
        g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed = HI_TRUE;
        bUpdate   = HI_TRUE;
    }
    else if (u32Width != pstDataInfo->u32Width
            || u32Height != pstDataInfo->u32Height
            || u32HeadStride != pstDataInfo->u32HeadStride)
    {
        pstDataInfo->u32Width     = u32Width;
        pstDataInfo->u32Height    = u32Height;
        pstDataInfo->u32CmpStride = u32Stride;
        pstDataInfo->u32HeadStride= u32HeadStride;
        pstDataInfo->u32HeadSize  = u32HeadSize;

        pstDataInfo->u32ARCmpAddr = (pstDataInfo->stCMPBuffer.u32StartPhyAddr & 0xfffffff0);
        pstDataInfo->u32GBCmpAddr = ((pstDataInfo->stCMPBuffer.u32StartPhyAddr + u32BufSize) & 0xfffffff0);

        g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed = HI_TRUE;
        bUpdate   = HI_TRUE;
    }

    if (bUpdate)
    {
        pstDataInfo->stUpdateRect.x = 0;
        pstDataInfo->stUpdateRect.y = 0;
        pstDataInfo->stUpdateRect.w = u32Width;
        pstDataInfo->stUpdateRect.h = u32Height;
    }
    return HI_SUCCESS;
}

HI_VOID OPTM_GFX_CMP_ReOpen(struct work_struct *data)
{
    HIFB_LAYER_ID_E u32LayerID;
    OPTM_GFX_WORK_S *pstWork = container_of(data, OPTM_GFX_WORK_S, work);
    u32LayerID = (HIFB_LAYER_ID_E)(pstWork->u32Data);
    OPTM_GFX_CMP_Open(u32LayerID);
}

HI_S32 OPTM_GFX_CMP_Open(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_LAYER_WBC_E enCmpHalID;
    OPTM_VDP_RECT_S stRect;
    HI_U32 u32Width, u32Height, u32Stride,u32HeadSize,u32BufSize,u32HeadStride;
    OPTM_CMP_DATA_INFO_S *pstCmpData;

    u32Width  = g_stGfxDevice[enLayerId].stInRect.w;
    u32Height = g_stGfxDevice[enLayerId].stInRect.h;
    u32HeadStride = (u32Width > OPTM_COMPRESSION_WIDTH_MAX) ? OPTM_COMPRESSION_HEAD_STRIDE_32 : OPTM_COMPRESSION_HEAD_STRIDE_16;
    u32Stride = (g_stGfxDevice[enLayerId].Stride/2 + OPTM_COMPRESSION_ALIGNMENT-1)&(~(OPTM_COMPRESSION_ALIGNMENT-1));
    u32HeadSize = u32HeadStride*u32Height;
    u32BufSize  = u32Height*u32Stride + u32HeadSize;

    pstCmpData = &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo);

    pstCmpData->u32Width  = u32Width;
    pstCmpData->u32Height = u32Height;
    pstCmpData->u32CmpStride = u32Stride;
    pstCmpData->u32HeadStride= u32HeadStride;
    pstCmpData->u32HeadSize  = u32HeadSize;

    pstCmpData->stCmpRect.x = 0;
    pstCmpData->stCmpRect.y = 0;
    pstCmpData->stCmpRect.w = u32Width;
    pstCmpData->stCmpRect.h = u32Height;

    if (enLayerId == HIFB_LAYER_HD_0)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID = OPTM_VDP_LAYER_WBC_G0;
        g_stGfxDevice[enLayerId].stCmpInfo.enIntMsk = OPTM_VDP_INTMSK_WBC_G0_INT;
    }
    else if (enLayerId == HIFB_LAYER_SD_0)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID = OPTM_VDP_LAYER_WBC_G4;
        g_stGfxDevice[enLayerId].stCmpInfo.enIntMsk = OPTM_VDP_INTMSK_WBC_G4_INT;
    }
    else
    {
        return HI_FAILURE;
    }

    enCmpHalID = g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID;

    if ((u32BufSize*2) > pstCmpData->u32CMPBufSize)
    {
        if (pstCmpData->stCMPBuffer.pu8StartVirAddr != NULL)
        {
            OPTM_UnmapAndRelease(&(pstCmpData->stCMPBuffer));
            pstCmpData->stCMPBuffer.pu8StartVirAddr  = NULL;
            pstCmpData->stCMPBuffer.u32StartPhyAddr  = 0;
            pstCmpData->stCMPBuffer.u32StartSmmuAddr = 0;
            pstCmpData->stCMPBuffer.u32Size          = 0;
            pstCmpData->stCMPBuffer.bSmmu            = HI_FALSE;
            pstCmpData->u32CMPBufSize = 0;
        }

        if (OPTM_AllocAndMap(OPTM_GFX_CMP_BUFFER,   \
                             NULL,                  \
                             (u32BufSize*2),        \
                             0,                     \
                             &(pstCmpData->stCMPBuffer)) != 0)
        {
            HIFB_ERROR("fail to alloc compression buffer.\n");
            return HI_FAILURE;
        }

        pstCmpData->u32CMPBufSize = (u32BufSize*2);
    }

    pstCmpData->u32ARCmpAddr = pstCmpData->stCMPBuffer.u32StartPhyAddr & 0xfffffff0;
    pstCmpData->u32GBCmpAddr = (pstCmpData->stCMPBuffer.u32StartPhyAddr+ u32BufSize) & 0xfffffff0;

    OPTM_VDP_WBC_SetLossy (enCmpHalID, HI_FALSE);
    OPTM_VDP_WBC_SetCmpDrr(enCmpHalID, 0);
    OPTM_VDP_WBC_SetOutFmt(enCmpHalID, VDP_WBC_OFMT_ARGB8888);

    OPTM_VDP_WBC_SetOffSet(enCmpHalID, u32HeadSize);
    OPTM_VDP_SetCMPAddr(enCmpHalID, pstCmpData->u32ARCmpAddr, pstCmpData->u32GBCmpAddr);
    OPTM_VDP_SetCMPStride(enCmpHalID, pstCmpData->u32CmpStride);

    HI_GFX_Memset(&stRect, 0, sizeof(stRect));
    stRect.u32X   = 0;
    stRect.u32Y = 0;
    stRect.u32Wth = u32Width & 0xfffffffe;
    stRect.u32Hgt = u32Height & 0xfffffffe;

    //OPTM_VDP_WBC_SetAutoSt    (enCmpHalID, HI_TRUE);
    OPTM_VDP_SetCMPInRect       (enCmpHalID, &stRect);
    OPTM_VDP_WBC_SetCMPOutSize  (enCmpHalID, (pstCmpData->u32Width & 0xfffffffe), (pstCmpData->u32Height & 0xfffffffe));
    HIFB_WBC2_SetRegUp          (enCmpHalID);

    if (HI_NULL == g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.queue)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.queue = create_workqueue(OPTM_GFX_CMP_QUEUE);

        if (HI_NULL != g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.queue)
        {
            INIT_WORK(&g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.work,  OPTM_GFX_CMP_ReOpen);
        }
    }

    g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectReady = HI_FALSE;
    g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed = HI_TRUE;
    g_stGfxDevice[enLayerId].stCmpInfo.enStatus = OPTM_CMP_STATUS_STANDBY;
    g_stGfxDevice[enLayerId].bCmpOpened = HI_TRUE;
    return HI_SUCCESS;
}

HI_VOID OPTM_GFX_CMP_Clean(HIFB_LAYER_ID_E enLayerId)
{
    HI_U32 u32Count;
    OPTM_GFX_CMP_S    *pstCmpInfo;

    pstCmpInfo = &(g_stGfxDevice[enLayerId].stCmpInfo);

    if (!g_stGfxCap[enLayerId].bCompression)
    {
        return;
    }

    if (pstCmpInfo->stCMPWork.queue)
    {
        destroy_workqueue(pstCmpInfo->stCMPWork.queue);
        pstCmpInfo->stCMPWork.queue = HI_NULL;
    }

    if (pstCmpInfo->stDataInfo.stCMPBuffer.pu8StartVirAddr != NULL)
    {
        OPTM_UnmapAndRelease(&pstCmpInfo->stDataInfo.stCMPBuffer);
        pstCmpInfo->stDataInfo.stCMPBuffer.pu8StartVirAddr  = NULL;
        pstCmpInfo->stDataInfo.stCMPBuffer.u32StartPhyAddr  = 0;
        pstCmpInfo->stDataInfo.stCMPBuffer.u32StartSmmuAddr = 0;
        pstCmpInfo->stDataInfo.stCMPBuffer.u32Size          = 0;
        pstCmpInfo->stDataInfo.stCMPBuffer.bSmmu            = HI_FALSE;
        pstCmpInfo->stDataInfo.u32CMPBufSize= 0;
        pstCmpInfo->stDataInfo.u32ARCmpAddr = 0;
        pstCmpInfo->stDataInfo.u32GBCmpAddr = 0;
    }

    for (u32Count= 0; u32Count < OPTM_COMPRESSION_MDDRC_ZONE; u32Count++)
    {
        OPTM_MDDRC_SetZoneAddr(u32Count, 0, 0);
    }
}

HI_S32 OPTM_GFX_CMP_Close(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_GFX_CMP_S    *pstCmpInfo;

    pstCmpInfo = &(g_stGfxDevice[enLayerId].stCmpInfo);

    if (!g_stGfxDevice[enLayerId].bCmpOpened)
    {
        return HI_SUCCESS;
    }

    pstCmpInfo->enCMPMode   = HIFB_CMP_BUTT;
    pstCmpInfo->u32CmpIndex = 0;
    pstCmpInfo->bLayerRefreshed = HI_FALSE;
    pstCmpInfo->bLayerResChgFlag = HI_FALSE;
    pstCmpInfo->enStatus = OPTM_CMP_STATUS_PARALYSED;
    pstCmpInfo->stDataInfo.u32HeadSize = 0;
    pstCmpInfo->bCMPQueueWorkStart = HI_FALSE;
    HI_GFX_Memset(&pstCmpInfo->stDataInfo.stUpdateRect, 0, sizeof(HIFB_RECT));

    OPTM_GFX_CMP_ProcessNOCMP(enLayerId);

    g_stGfxDevice[enLayerId].bCmpOpened = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 OPTM_GFX_CMP_CheckInt(HIFB_LAYER_ID_E enLayerId)
{
    HI_U32 bCmpFinish;

    bCmpFinish = OPTM_VDP_DISP_GetIntSignal(g_stGfxDevice[enLayerId].stCmpInfo.enIntMsk);
    OPTM_ClearIntSignal(g_stGfxDevice[enLayerId].stCmpInfo.enIntMsk);
    return bCmpFinish;
}

HI_VOID OPTM_GFX_CMP_CombinRect(HIFB_RECT *pstSrcRect, HIFB_RECT *pstDstRect)
{
    HIFB_RECT stRect;

    if (0 == pstDstRect->w || 0 == pstDstRect->h)
    {
        HI_GFX_Memcpy(pstDstRect, pstSrcRect, sizeof(HIFB_RECT));
        return;
    }

    if (0 == pstSrcRect->w || 0 == pstSrcRect->h)
    {
        return;
    }

    stRect.x = (pstDstRect->x < pstSrcRect->x)? pstDstRect->x : pstSrcRect->x;
    stRect.y = (pstDstRect->y < pstSrcRect->y)? pstDstRect->y : pstSrcRect->y;
    stRect.w = ((pstDstRect->x + pstDstRect->w) > (pstSrcRect->x + pstSrcRect->w))?
        (pstDstRect->x + pstDstRect->w - stRect.x) : (pstSrcRect->x + pstSrcRect->w - stRect.x);
    stRect.h = ((pstDstRect->y + pstDstRect->h) > (pstSrcRect->y + pstDstRect->h))?
        (pstDstRect->y + pstDstRect->h - stRect.y) : (pstSrcRect->y + pstSrcRect->h - stRect.y);

    HI_GFX_Memcpy(pstDstRect, &stRect, sizeof(HIFB_RECT));

    return;
}


HI_VOID OPTM_GFX_CMP_WorkFinish(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_GFX_CMP_S    *pstCmpInfo;

    pstCmpInfo = &(g_stGfxDevice[enLayerId].stCmpInfo);

    if (OPTM_CMP_STATUS_COMPRESSING != pstCmpInfo->enStatus)
    {
        return;
    }

    HI_GFX_Memset(&pstCmpInfo->stDataInfo.stCmpRect, 0, sizeof(HIFB_RECT));
    OPTM_VDP_WBC_SetEnable      (pstCmpInfo->enCmpHalID, HI_FALSE);
    OPTM_VDP_WBC_SetCmpEnable   (pstCmpInfo->enCmpHalID, HI_FALSE);
    HIFB_WBC2_SetRegUp          (pstCmpInfo->enCmpHalID);
    pstCmpInfo->enStatus = OPTM_CMP_STATUS_COMPRESSFINISHED;

    return;
}

HI_S32 OPTM_GFX_CMP_GetSwitch(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxDevice[enLayerId].bCmpOpened;
}

HI_S32 OPTM_GFX_SetCmpRect(HIFB_LAYER_ID_E enLayerId, HIFB_RECT *pstRect)
{
    OPTM_CMP_DATA_INFO_S *pstDataInfo;

    if (0 == pstRect->w
        || 0 == pstRect->h)
    {
        return HI_FAILURE;
    }

    pstDataInfo = &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo);

    /*hardware compress data line by line*/
    pstDataInfo->stUpdateRect.x = 0;
    pstDataInfo->stUpdateRect.w = g_stGfxDevice[enLayerId].stInRect.w;
    pstDataInfo->stUpdateRect.y = pstRect->y;
    pstDataInfo->stUpdateRect.h = pstRect->h;

    if (g_stGfxDevice[enLayerId].stCmpInfo.enCMPMode == HIFB_CMP_GLOBAL)
    {
        pstDataInfo->stUpdateRect.y = 0;
        pstDataInfo->stUpdateRect.h = g_stGfxDevice[enLayerId].stInRect.h;
    }

    g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed = HI_TRUE;

    return HI_SUCCESS;
}

OPTM_CMP_STATUS_E OPTM_GFX_CMP_GetStatus(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_CMP_STATUS_E enStatus;
    OPTM_GFX_CMP_S    *pstCmpInfo;

    pstCmpInfo = &(g_stGfxDevice[enLayerId].stCmpInfo);

    if (OPTM_CMP_STATUS_PARALYSED == pstCmpInfo->enStatus)
    {
        return pstCmpInfo->enStatus;
    }

    /*user have updated data during this frame*/
    if (pstCmpInfo->bLayerRefreshed)
    {
        pstCmpInfo->enStatus = OPTM_CMP_STATUS_STANDBY;
        enStatus = OPTM_CMP_STATUS_STANDBY;
        return enStatus ;
    }

    if (OPTM_CMP_STATUS_COMPRESSING == pstCmpInfo->enStatus
        || OPTM_CMP_STATUS_DECOMPRESSING == pstCmpInfo->enStatus)
    {
        return pstCmpInfo->enStatus;
    }

    if (pstCmpInfo->enStatus == OPTM_CMP_STATUS_STANDBY)
    {
        if ((pstCmpInfo->stDataInfo.stCmpRect.w
            && pstCmpInfo->stDataInfo.stCmpRect.h) &&
            pstCmpInfo->u32CmpIndex >= OPTM_COMPRESSION_INTERVAL)
        {
            pstCmpInfo->enStatus = OPTM_CMP_STATUS_COMPRESSIONABLE;
        }
    }

    if (OPTM_CMP_STATUS_COMPRESSFINISHED == pstCmpInfo->enStatus)
    {
        pstCmpInfo->enStatus = OPTM_CMP_STATUS_DECOMPRESSIONABLE;
    }

    return pstCmpInfo->enStatus;
}

HI_VOID OPTM_GFX_CMP_ProcessCMP(HIFB_LAYER_ID_E enLayerId)
{
    HIFB_RECT *pstCmpRect;
    OPTM_CMP_DATA_INFO_S *pstCmpData;
    HI_U32 u32ARAddr, u32GBAddr,u32Offset;
    OPTM_VDP_RECT_S stRect;
    OPTM_VDP_LAYER_WBC_E enCmpHalID;

    pstCmpData = &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo);
    pstCmpRect = &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stCmpRect);
    enCmpHalID = g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID;

    HI_GFX_Memset(&stRect, 0, sizeof(stRect));
    stRect.u32X = pstCmpRect->x & 0xfffffffe;
    stRect.u32Y = pstCmpRect->y & 0xfffffffe;
    stRect.u32Wth = (pstCmpRect->w + 1) & 0xfffffffe;
    stRect.u32Hgt = (pstCmpRect->h + 1) & 0xfffffffe;

    u32ARAddr = pstCmpData->u32ARCmpAddr + stRect.u32Y*pstCmpData->u32HeadStride;
    u32GBAddr = pstCmpData->u32GBCmpAddr + stRect.u32Y*pstCmpData->u32HeadStride;

    u32Offset = pstCmpData->u32HeadSize + pstCmpData->u32CmpStride*stRect.u32Y
                - pstCmpData->u32HeadStride*stRect.u32Y;


    OPTM_VDP_SetCMPAddr(enCmpHalID, u32ARAddr, u32GBAddr);
    OPTM_VDP_SetCMPStride(enCmpHalID, pstCmpData->u32CmpStride);
    OPTM_VDP_WBC_SetOffSet(enCmpHalID, u32Offset);

    OPTM_VDP_SetCMPInRect(enCmpHalID, &stRect);
    //OPTM_VDP_WBC_SetCMPOutSize(enCmpHalID, (pstCmpData->u32Width & 0xfffffffe), (pstCmpData->u32Height & 0xfffffffe));
    OPTM_VDP_WBC_SetCMPOutSize(enCmpHalID, stRect.u32Wth, stRect.u32Hgt);
    OPTM_VDP_WBC_SetEnable(enCmpHalID, HI_TRUE);
    OPTM_VDP_WBC_SetCmpEnable(enCmpHalID, HI_TRUE);

    HIFB_WBC2_SetRegUp(enCmpHalID);

    g_stGfxDevice[enLayerId].stCmpInfo.u32CmpIndex = 0;
    g_stGfxDevice[enLayerId].stCmpInfo.enStatus = OPTM_CMP_STATUS_COMPRESSING;
}

HI_VOID OPTM_GFX_CMP_ProcessDECMP(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_CMP_DATA_INFO_S *pstDataInfo;
    OPTM_VDP_LAYER_GFX_E enGfxHalId;

    enGfxHalId  = g_stGfxDevice[enLayerId].enGfxHalId;
    pstDataInfo = &(g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo);

    OPTM_VDP_SetDCMPAddr  (enGfxHalId, pstDataInfo->u32ARCmpAddr, pstDataInfo->u32GBCmpAddr);
    OPTM_VDP_SetDCMPStride(enGfxHalId, pstDataInfo->u32CmpStride);
    OPTM_VDP_GFX_SetDcmpOffset(enGfxHalId, pstDataInfo->u32HeadSize);
    OPTM_VDP_GFX_SetDcmpEnable(enGfxHalId, HI_TRUE);

    g_stGfxDevice[enLayerId].stCmpInfo.enStatus = OPTM_CMP_STATUS_DECOMPRESSING;

    OPTM_VDP_GFX_SetRegUp(enGfxHalId);
    OPTM_VDP_GP_SetRegUp (g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].enGpHalId);
    OPTM_VDP_GFX_SetParaUpd(enGfxHalId, VDP_DISP_COEFMODE_LUT);
}

HI_VOID OPTM_GFX_CMP_ProcessNOCMP(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_VDP_GFX_SetDcmpEnable(g_stGfxDevice[enLayerId].enGfxHalId, HI_FALSE);
    OPTM_VDP_WBC_SetEnable(g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID, HI_FALSE);
    OPTM_VDP_WBC_SetCmpEnable(g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID, HI_FALSE);

    if (g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].bNeedExtractLine)
    {
        OPTM_VDP_GFX_SetLayerStride(g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].Stride*2);
    }
    else
    {
        OPTM_VDP_GFX_SetLayerStride(g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].Stride);
    }

    OPTM_VDP_GFX_SetLayerAddrEX(g_stGfxDevice[enLayerId].enGfxHalId, g_stGfxDevice[enLayerId].NoCmpBufAddr);
    OPTM_VDP_GFX_SetLayerNAddr (g_stGfxDevice[enLayerId].enGfxHalId, HI_NULL);

    OPTM_VDP_GFX_SetRegUp    (g_stGfxDevice[enLayerId].enGfxHalId);
    OPTM_VDP_GP_SetRegUp     (g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].enGpHalId);
    OPTM_VDP_GFX_SetParaUpd  (g_stGfxDevice[enLayerId].enGfxHalId, VDP_DISP_COEFMODE_LUT);
    HIFB_WBC2_SetRegUp       (g_stGfxDevice[enLayerId].stCmpInfo.enCmpHalID);
}

HI_VOID OPTM_GFX_CMP_GetDDRefreshRect(HIFB_LAYER_ID_E enLayerId)
{
    HI_U32 u32Status;
    OPTM_GFX_LAYER_S *pstLayer;

    pstLayer = &(g_stGfxDevice[enLayerId]);

    OPTM_MDDRC_GetStatus(&u32Status);

    if (0 == u32Status)
    {
        return;
    }

    if (pstLayer->stInRect.h < OPTM_COMPRESSION_MDDRC_ZONE)

    {
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.x = 0;
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.y = 0;
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.w = pstLayer->stInRect.w;
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.h = pstLayer->stInRect.h;
    }
    else
    {
        HI_U32 u32Count, u32FirstZone, u32LastZone;
        HI_U32 u32Divisor, u32Remainder;
        u32FirstZone = 0;
        u32LastZone  = 0;
        u32Divisor   = pstLayer->stInRect.h/OPTM_COMPRESSION_MDDRC_ZONE;
        u32Remainder = pstLayer->stInRect.h%OPTM_COMPRESSION_MDDRC_ZONE;

        for (u32Count = 0; u32Count < OPTM_COMPRESSION_MDDRC_ZONE; u32Count++)
        {
            if (u32Status&(1<<u32Count))
            {
                if (!u32FirstZone)
                {
                    u32FirstZone = u32Count + 1;
                }

                u32LastZone  = u32Count + 1;
            }
        }

        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.x = 0;
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.y = (u32FirstZone-1)*u32Divisor;

        if (u32FirstZone > u32Remainder)
        {
            pstLayer->stCmpInfo.stDataInfo.stUpdateRect.y += u32Remainder;
        }
        else
        {
            pstLayer->stCmpInfo.stDataInfo.stUpdateRect.y += (u32FirstZone-1);
        }

        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.w = pstLayer->stInRect.w;
        pstLayer->stCmpInfo.stDataInfo.stUpdateRect.h = 0;

        for (u32Count = u32FirstZone; u32Count <= u32LastZone; u32Count++)
        {
            pstLayer->stCmpInfo.stDataInfo.stUpdateRect.h += u32Divisor;

            if (u32Remainder >= u32Count)
            {
                pstLayer->stCmpInfo.stDataInfo.stUpdateRect.h += 1;
            }
        }

    }

    if (pstLayer->stCmpInfo.stDataInfo.stUpdateRect.h)
    {
        pstLayer->stCmpInfo.bLayerRefreshed = HI_TRUE;
    }
}

HI_VOID OPTM_GFX_CMP_SetDDRDetectAddr(HIFB_LAYER_ID_E enLayerId)
{
    HI_U32 u32Status;
    HI_U32 u32Width, u32Height, u32Stride;
    HI_U32 u32StartAddr, u32EndAddr, u32Section;
    OPTM_GFX_LAYER_S *pstLayer;
    HI_U32 u32Count;
    HI_U32 u32Divisor, u32Remainder;

    pstLayer  = &(g_stGfxDevice[enLayerId]);

    u32Divisor   = pstLayer->stInRect.h/OPTM_COMPRESSION_MDDRC_ZONE;
    u32Remainder = pstLayer->stInRect.h%OPTM_COMPRESSION_MDDRC_ZONE;

    u32Width  = pstLayer->stInRect.w;
    u32Height = pstLayer->stInRect.h;
    u32Stride = pstLayer->Stride;

    if (pstLayer->stInRect.h < OPTM_COMPRESSION_MDDRC_ZONE)
    {
        u32Section = 0;
        u32StartAddr = g_stGfxDevice[enLayerId].NoCmpBufAddr;
        u32EndAddr   = u32StartAddr + u32Height*u32Stride;
        OPTM_MDDRC_SetZoneAddr(u32Section, u32StartAddr, u32EndAddr);
    }
    else
    {
        for (u32Count= 0; u32Count < OPTM_COMPRESSION_MDDRC_ZONE; u32Count++)
        {
            u32Height = u32Count*u32Divisor;
            if (u32Count > (u32Remainder-1))
            {
                u32Height += u32Remainder-1;
            }
            else
            {
                u32Height += u32Count;
            }

            u32StartAddr =  g_stGfxDevice[enLayerId].NoCmpBufAddr;
            u32StartAddr += u32Height*u32Stride;
            u32EndAddr = u32StartAddr+u32Divisor*u32Stride;

            if ((u32Remainder-1) >= u32Count)
            {
                u32EndAddr +=  u32Stride;
            }

            OPTM_MDDRC_SetZoneAddr(u32Count, u32StartAddr, u32EndAddr);
        }
    }

    OPTM_MDDRC_GetStatus(&u32Status);

}

HI_VOID OPTM_GFX_CMP_Handle(HIFB_LAYER_ID_E enLayerId)
{
    OPTM_CMP_STATUS_E enStatus;

    if (!g_stGfxDevice[enLayerId].bCmpOpened)
    {
        return;
    }

    if (g_stGfxGPDevice[g_stGfxDevice[enLayerId].enGPId].enTriDimMode
        != HIFB_STEREO_MONO)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.enStatus =  OPTM_CMP_STATUS_PARALYSED;
        g_stGfxDevice[enLayerId].bCmpOpened = HI_FALSE;
    }

    if (g_stGfxDevice[enLayerId].stCmpInfo.bCMPQueueWorkStart)
    {
        if (g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.queue)
        {
            g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.u32Data = enLayerId;
            queue_work(g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.queue,
                        &g_stGfxDevice[enLayerId].stCmpInfo.stCMPWork.work);
        }
        g_stGfxDevice[enLayerId].stCmpInfo.bCMPQueueWorkStart = HI_FALSE;
    }

    /* layer resolution changed  */
    if (g_stGfxDevice[enLayerId].stCmpInfo.bLayerResChgFlag)
    {
        OPTM_GFX_CMP_ResChg(enLayerId);
        g_stGfxDevice[enLayerId].stCmpInfo.bLayerResChgFlag = HI_FALSE;
    }
    /*                 end                  */

    /*DDR check updated region*/
    if (g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectOpen)
    {
        if (!g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed
            && g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectReady)
        {
            /*ready ddr status and update ddr status*/
            OPTM_GFX_CMP_GetDDRefreshRect(enLayerId);
        }

        if (!g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectReady)
        {
            /*set ddr detect region*/
            OPTM_GFX_CMP_SetDDRDetectAddr(enLayerId);
            g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectReady= HI_TRUE;
        }
    }
    /*                 end                  */

    /*Check compression work*/
    if (OPTM_GFX_CMP_CheckInt(enLayerId))
    {
        OPTM_GFX_CMP_WorkFinish(enLayerId);
    }
    /*                 end                  */

    if (g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stUpdateRect.w
        && g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stUpdateRect.h)
    {
        OPTM_GFX_CMP_CombinRect(&g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stUpdateRect,
                                      &g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stCmpRect);
        HI_GFX_Memset(&g_stGfxDevice[enLayerId].stCmpInfo.stDataInfo.stUpdateRect,0,sizeof(HIFB_RECT));
    }

    /*get compression status*/
    enStatus = OPTM_GFX_CMP_GetStatus(enLayerId);
    /*                end                  */

    if (OPTM_CMP_STATUS_DECOMPRESSIONABLE == enStatus)
    {
        /*disable compression, enable decompression*/
        OPTM_GFX_CMP_ProcessDECMP(enLayerId);
    }
    else if (OPTM_CMP_STATUS_COMPRESSIONABLE == enStatus)
    {
        OPTM_GFX_CMP_ProcessNOCMP(enLayerId);
        OPTM_GFX_CMP_ProcessCMP(enLayerId);
    }
    else if (OPTM_CMP_STATUS_STANDBY == enStatus
            || OPTM_CMP_STATUS_PARALYSED == enStatus)
    {
        OPTM_GFX_CMP_ProcessNOCMP(enLayerId);
    }

    /*process compression info in the end of v blank*/
    g_stGfxDevice[enLayerId].stCmpInfo.bLayerRefreshed = HI_FALSE;
    g_stGfxDevice[enLayerId].stCmpInfo.u32CmpIndex++;
    if (g_stGfxDevice[enLayerId].stCmpInfo.u32CmpIndex > OPTM_COMPRESSION_INTERVAL)
    {
        g_stGfxDevice[enLayerId].stCmpInfo.u32CmpIndex = OPTM_COMPRESSION_INTERVAL;
    }
    /*    end    */
    return;
}

HI_VOID OPTM_GFX_CMP_Process(HI_VOID* u32Param0)
{
    HI_U32                      i;
    HI_U32                      u32LayerCount;
    HIFB_LAYER_ID_E             enInitLayerId;
    HIFB_LAYER_ID_E             enLayerId;
    OPTM_VDP_LAYER_GP_E         *pEnGpHalId;

    pEnGpHalId  = (OPTM_VDP_LAYER_GP_E *)u32Param0;

    if (pEnGpHalId == HI_NULL)
    {
        return;
    }

    u32LayerCount = (OPTM_VDP_LAYER_GP0 == *pEnGpHalId) ? OPTM_GP0_GFX_COUNT : OPTM_GP1_GFX_COUNT;

    if (OPTM_VDP_LAYER_GP0 == *pEnGpHalId)
    {
        enInitLayerId     = HIFB_LAYER_HD_0;
    }
    else if (OPTM_VDP_LAYER_GP1 == *pEnGpHalId)
    {
        enInitLayerId    = HIFB_LAYER_SD_0;
    }
    else
    {
        return;
    }

    for (i = 0; i < u32LayerCount;i++)
    {
        enLayerId = enInitLayerId + i;

        if (!g_stGfxCap[enLayerId].bCompression)
        {
            continue;
        }

        OPTM_GFX_CMP_Handle(enLayerId);
    }
}

/*Set compression Mode*/
HI_S32 OPTM_GFX_SetCmpMode(HIFB_LAYER_ID_E enLayerId, HIFB_CMP_MODE_E enCMPMode)
{
    g_stGfxDevice[enLayerId].stCmpInfo.enCMPMode = enCMPMode;
    return HI_SUCCESS;
}
/*Get compression Mode*/
HIFB_CMP_MODE_E OPTM_GFX_GetCmpMode(HIFB_LAYER_ID_E enLayerId)
{
    return g_stGfxDevice[enLayerId].stCmpInfo.enCMPMode;
}

HI_S32 OPTM_GFX_SetCmpDDROpen(HIFB_LAYER_ID_E enLayerId, HI_BOOL bOpen)
{
    g_stGfxDevice[enLayerId].stCmpInfo.bDDRDetectOpen = bOpen;
    return HI_FALSE;
}
#endif



/***************************************************************************************
* func          : OPTM_GFX_GetHaltDispStatus
* description   : CNcomment: 获取显示状态 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_GFX_GetHaltDispStatus(HIFB_LAYER_ID_E enLayerId, HI_BOOL *pbDispInit)
{
    OPTM_GFX_GP_E enGPId = g_stGfxDevice[enLayerId].enGPId;
    *pbDispInit =  g_stGfxGPDevice[enGPId].bGpClose?HI_FALSE:HI_TRUE;
    return HI_SUCCESS;
}

/***************************************************************************************
* func          : OPTM_AllocAndMap
* description   : CNcomment: alloc and map mem CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, GFX_MMZ_BUFFER_S *psMBuf)
{
    #ifndef HI_BUILD_IN_BOOT
        return HI_GFX_AllocAndMap(bufname, zone_name, size, align, psMBuf);
    #else
        if (HI_SUCCESS == HI_DRV_PDM_AllocReserveMem(bufname, size, &psMBuf->u32StartPhyAddr)){
            psMBuf->pu8StartVirAddr = (HI_U8*)psMBuf->u32StartPhyAddr;
            return HI_SUCCESS;
        }else{
            return HI_FAILURE;
        }
    #endif
}

/***************************************************************************************
* func          : OPTM_UnmapAndRelease
* description   : unmap the ddr and release it\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_UnmapAndRelease(GFX_MMZ_BUFFER_S *psMBuf)
{
    #ifdef HI_BUILD_IN_BOOT
        return;
    #else
        HI_GFX_UnmapAndRelease(psMBuf);
    #endif
}

/***************************************************************************************
* func          : OPTM_Adapt_AllocAndMap
* description   : CNcomment: 分配内存并映射地址 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 OPTM_Adapt_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, GFX_MMZ_BUFFER_S *psMBuf)
{
    #ifndef HI_BUILD_IN_BOOT
        return HI_GFX_AllocAndMap(bufname, zone_name, size, align, psMBuf);
    #else
        psMBuf->u32StartPhyAddr = (HI_U32)HI_GFX_Malloc(size,"hifb_wbc_buffer");
        if (HI_NULL == psMBuf->u32StartPhyAddr)
        {
            HIFB_ERROR("fail to alloc buffer.\n");
            return HI_FAILURE;
        }
        psMBuf->u32StartPhyAddr = psMBuf->u32StartPhyAddr;
        return HI_SUCCESS;
    #endif
}


HI_BOOL HIFB_ADP_GetIsGfxWorkAtWbc(HI_VOID)
{
    return (HIFB_GFX_MODE_HD_WBC == g_enOptmGfxWorkMode) ? (HI_TRUE) : (HI_FALSE);
}


/***************************************************************************************
* func          : OPTM_GFX_GetOps
* description   : CNcomment: 获取设备上下文 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID OPTM_GFX_GetOps(OPTM_GFX_OPS_S *ops)
{

    ops->OPTM_GfxCloseLayer         = OPTM_GfxCloseLayer;
    ops->OPTM_GfxDeInit             = OPTM_GfxDeInit;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GfxGetLayerPriority   = OPTM_GfxGetLayerPriority;
    ops->OPTM_GfxGetOSDData         = OPTM_GfxGetOSDData;
    ops->HIFB_ADP_GetLogoData       = HIFB_ADP_GetLogoData;
#endif
    ops->OPTM_GfxInit               = OPTM_GfxInit;
    ops->OPTM_GfxOpenLayer          = OPTM_GfxOpenLayer;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GfxSetCallback        = OPTM_GfxSetCallback;
#endif
    ops->OPTM_GfxSetColorReg        = OPTM_GFX_SetClutColorReg;
    ops->OPTM_GfxSetEnable          = OPTM_GfxSetEnable;
    ops->OPTM_GfxSetGpRect          = OPTM_GfxSetGpRect;
    ops->OPTM_GfxGetGpRect          = OPTM_GfxGetGpRect;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GfxSetGpInPutSize     = OPTM_GfxSetGpInPutSize;
#endif
    ops->OPTM_GfxSetLayerAddr       = OPTM_GfxSetLayerAddr;
    ops->OPTM_GfxGetLayerAddr       = OPTM_GfxGetLayerAddr;
    ops->OPTM_GfxSetLayerAlpha      = OPTM_GfxSetLayerAlpha;
    ops->OPTM_GfxGetLayerGalphaSum  = OPTM_GfxGetLayerGalphaSum;

    ops->OPTM_GfxSetLayerDataFmt    = OPTM_GfxSetLayerDataFmt;
    ops->OPTM_GfxSetLayerDeFlicker  = OPTM_GfxSetLayerDeFlicker;
    ops->OPTM_GfxSetLayerPreMult    = OPTM_GfxSetLayerPreMult;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GfxSetLayerPriority   = OPTM_GfxSetLayerPriority;
#endif
    ops->OPTM_GfxSetLayerRect       = OPTM_GfxSetLayerRect;
    ops->OPTM_GfxSetLayerStride     = OPTM_GfxSetLayerStride;
    ops->OPTM_GfxSetLayKeyMask      = OPTM_GfxSetLayKeyMask;
    ops->OPTM_GfxUpLayerReg         = OPTM_GfxUpLayerReg;
    ops->OPTM_GfxWaitVBlank         = OPTM_GfxWaitVBlank;
    ops->OPTM_GFX_GetDevCap         = OPTM_GFX_GetDevCap;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GfxGetOutRect         = OPTM_GfxGetOutRect;
#endif
    ops->OPTM_GfxGetLayerRect       = OPTM_GfxGetLayerRect;
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GFX_SetGPMask         = OPTM_GFX_SetGPMask;
    ops->OPTM_GFX_GetGfxMask        = OPTM_GFX_GetGfxMask;
    ops->OPTM_GfxGetDispFMTSize     = OPTM_GfxGetDispFMTSize;
    ops->OPTM_GFX_GetSlvLayerInfo   = HIFB_WBC_GetLayerInfo;
#endif
    ops->OPTM_GFX_SetTCFlag         = OPTM_GFX_SetTCFlag;

#ifndef HI_BUILD_IN_BOOT
    ops->HIFB_ADP_GetResumeStatus   = HIFB_ADP_GetResumeStatus;
#endif

#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(HI_BUILD_IN_BOOT)
    ops->HIFB_ADP_SetDeCmpDdrInfo   = HIFB_ADP_SetDeCmpDdrInfo;
    ops->HIFB_ADP_GetDeCmpStatus    = HIFB_ADP_GetDeCmpStatus;
    ops->OPTM_GFX_DECMP_Open        = OPTM_GFX_DECMP_Open;
    ops->OPTM_GFX_DECMP_Close       = OPTM_GFX_DECMP_Close;
#endif

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    ops->OPTM_GFX_CMP_Open          = OPTM_GFX_CMP_Open;
    ops->OPTM_GFX_CMP_Close         = OPTM_GFX_CMP_Close;
    ops->OPTM_GFX_CMP_GetSwitch     = OPTM_GFX_CMP_GetSwitch;
    ops->OPTM_GFX_SetCmpRect        = OPTM_GFX_SetCmpRect;
    ops->OPTM_GFX_SetCmpMode        = OPTM_GFX_SetCmpMode;
    ops->OPTM_GFX_GetCmpMode        = OPTM_GFX_GetCmpMode;
    ops->OPTM_GFX_SetCmpDDROpen     = OPTM_GFX_SetCmpDDROpen;
#endif

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    ops->OPTM_GfxSetTriDimEnable    = OPTM_GfxSetTriDimEnable;
    ops->OPTM_GfxSetTriDimMode      = OPTM_GfxSetTriDimMode;
    ops->OPTM_GfxSetTriDimAddr      = OPTM_GfxSetTriDimAddr;
    ops->OPTM_GFX_SetStereoDepth    = OPTM_GFX_SetStereoDepth;
#endif
#ifndef HI_BUILD_IN_BOOT
    ops->OPTM_GFX_ClearLogoOsd      = OPTM_GFX_ClearLogoOsd;
    ops->OPTM_GfxSetGpDeflicker     = OPTM_GfxSetGpDeflicker;
#endif
    ops->OPTM_GFX_GetHaltDispStatus = OPTM_GFX_GetHaltDispStatus;
    ops->HIFB_ADP_GetIsGfxWorkAtWbc = HIFB_ADP_GetIsGfxWorkAtWbc;
}
