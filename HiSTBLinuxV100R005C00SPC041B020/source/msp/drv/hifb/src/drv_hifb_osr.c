/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_osr.c
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/


/*********************************add include here******************************/
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/file.h>
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
#include "hi_drv_proc.h"
#include "drv_pdm_ext.h"


#include "hifb_drv.h"
#include "hifb.h"
#include "drv_hifb_osr.h"
#include "drv_hifb_mem.h"

#ifdef CFG_HIFB_LOGO_SUPPORT
#include "drv_hifb_logo.h"
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
#include "drv_hifb_fence.h"
#endif

#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
#include "drv_hifb_scrolltext.h"
#endif

#include "drv_hifb_ext.h"
#include "drv_hifb_err.h"

/**
 **所有发生变化的所在的头文件
 **/
#include "drv_hifb_config.h"
#include "hi_gfx_sys_k.h"

/***************************** Macro Definition ******************************/

//#define CFG_HIFB_SUPPORT_CONSOLE
//#define CFG_HIFB_PROC_DEBUG

/**
 **mod init this var
 **/
#define HIFB_MAX_WIDTH(u32LayerId)     gs_pstCapacity[u32LayerId].u32MaxWidth
#define HIFB_MAX_HEIGHT(u32LayerId)    gs_pstCapacity[u32LayerId].u32MaxHeight
#define HIFB_MIN_WIDTH(u32LayerId)     gs_pstCapacity[u32LayerId].u32MinWidth
#define HIFB_MIN_HEIGHT(u32LayerId)    gs_pstCapacity[u32LayerId].u32MinHeight

#define IS_STEREO_SBS(par)  ((par->st3DInfo.StereoMode == HIFB_STEREO_SIDEBYSIDE_HALF))
#define IS_STEREO_TAB(par)  ((par->st3DInfo.StereoMode == HIFB_STEREO_TOPANDBOTTOM))
#define IS_STEREO_FPK(par)  ((par->st3DInfo.StereoMode == HIFB_STEREO_FRMPACKING))

#define IS_2BUF_MODE(par)  ((par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE || par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
#define IS_1BUF_MODE(par)  ((par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_ONE))


#define FBNAME "HI_FB"

#define DRV_HIFB_IOCTLCMD_NUM_MAX            150
#define DRV_HIFB_IOCTLFUNC_ITEM_NUM_MAX      50

#define DRV_HIFB_CHECK_IOCTL_CMD(CmdNum)\
    if ((CmdNum < 1) || (CmdNum > DRV_HIFB_IOCTLCMD_NUM_MAX))\
    {\
        return HI_FAILURE;\
    }\

#define DRV_HIFB_CHECK_IOCTL_NUM(CmdFunc)\
        if ((CmdFunc < 1) || (CmdFunc > DRV_HIFB_IOCTLFUNC_ITEM_NUM_MAX))\
        {\
            return HI_FAILURE;\
        }\

typedef HI_S32(*DRV_HIFB_IoctlFunc)(struct fb_info *info, HI_ULONG arg);


/*************************** Structure Definition ****************************/
static char* video = HI_NULL;
module_param(video, charp, S_IRUGO);

static char* tc_wbc = "off";
module_param(tc_wbc, charp, S_IRUGO);


HIFB_DRV_OPS_S    g_stDrvAdpCallBackFunction;
HIFB_DRV_TDEOPS_S g_stGfx2dCallBackFunction;

/* to save layer id and layer size */
HIFB_LAYER_S s_stLayer[HIFB_MAX_LAYER_NUM];

const static HIFB_CAPABILITY_S *gs_pstCapacity;

#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    /*define this array to save the private info of scrolltext layer*/
    HIFB_SCROLLTEXT_INFO_S s_stTextLayer[HIFB_LAYER_ID_BUTT];
#endif



typedef struct tagDrvHifbIoctlFunc
{
    HI_U32 Cmd;
    DRV_HIFB_IoctlFunc DrvHifbIoctlFunc;
}DRV_HIFB_IOCTLFUNC_ITEM;


/********************** Global Variable declaration **************************/
static HI_U32 gs_u32MenuconfigLayerSize[HIFB_MAX_LAYER_NUM] =
{
    #ifdef CFG_HI_HD0_FB_VRAM_SIZE
        CFG_HI_HD0_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_HD1_FB_VRAM_SIZE
        CFG_HI_HD1_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_HD2_FB_VRAM_SIZE
        CFG_HI_HD2_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_HD3_FB_VRAM_SIZE
        CFG_HI_HD3_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_SD0_FB_VRAM_SIZE
        CFG_HI_SD0_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_SD1_FB_VRAM_SIZE
        CFG_HI_SD1_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_SD2_FB_VRAM_SIZE
        CFG_HI_SD2_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_SD3_FB_VRAM_SIZE
        CFG_HI_SD3_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_AD0_FB_VRAM_SIZE
        CFG_HI_AD0_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_AD1_FB_VRAM_SIZE
        CFG_HI_AD1_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_AD2_FB_VRAM_SIZE
        CFG_HI_AD2_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_AD3_FB_VRAM_SIZE
        CFG_HI_AD3_FB_VRAM_SIZE,
    #else
        0,
    #endif
    #ifdef CFG_HI_CURSOR_FB_VRAM_SIZE
        CFG_HI_CURSOR_FB_VRAM_SIZE,
    #else
        0,
    #endif
};

static struct fb_fix_screeninfo gs_stDefFix =
{
    .id          = "hifb",
    .type        = FB_TYPE_PACKED_PIXELS,
    .visual      = FB_VISUAL_TRUECOLOR,
    .xpanstep    = 1,
    .ypanstep    = 1,
    .ywrapstep   = 0,
    .line_length = HIFB_HD_DEF_STRIDE,
    .accel       = FB_ACCEL_NONE,
    .mmio_len    = 0,
    .mmio_start  = 0,
};


static struct fb_var_screeninfo s_stDefVar[HIFB_LAYER_TYPE_BUTT] =
{
    {
        .xres           = HIFB_HD_DEF_WIDTH,
        .yres           = HIFB_HD_DEF_HEIGHT,
        .xres_virtual   = HIFB_HD_DEF_WIDTH,
        .yres_virtual   = HIFB_HD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = { 5, 5, 0},
        .blue           = { 0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1,
        .left_margin    = -1,
        .right_margin   = -1,
        .upper_margin   = -1,
        .lower_margin   = -1,
        .hsync_len      = -1,
        .vsync_len      = -1,
    },
    {
        .xres           = HIFB_SD_DEF_WIDTH,
        .yres           = HIFB_SD_DEF_HEIGHT,
        .xres_virtual   = HIFB_SD_DEF_WIDTH,
        .yres_virtual   = HIFB_SD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = { 5, 5, 0},
        .blue           = { 0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1,
        .left_margin    = -1,
        .right_margin   = -1,
        .upper_margin   = -1,
        .lower_margin   = -1,
        .hsync_len      = -1,
        .vsync_len      = -1,
    },
    {
        .xres           = HIFB_AD_DEF_WIDTH,
        .yres           = HIFB_AD_DEF_HEIGHT,
        .xres_virtual   = HIFB_AD_DEF_WIDTH,
        .yres_virtual   = HIFB_AD_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = { 5, 5, 0},
        .blue           = { 0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1,
        .left_margin    = -1,
        .right_margin   = -1,
        .upper_margin   = -1,
        .lower_margin   = -1,
        .hsync_len      = -1,
        .vsync_len      = -1,
    },
    {
        .xres           = HIFB_CURSOR_DEF_WIDTH,
        .yres           = HIFB_CURSOR_DEF_HEIGHT,
        .xres_virtual   = HIFB_CURSOR_DEF_WIDTH,
        .yres_virtual   = HIFB_CURSOR_DEF_HEIGHT,
        .xoffset        = 0,
        .yoffset        = 0,
        .bits_per_pixel = HIFB_DEF_DEPTH,
        .red            = {10, 5, 0},
        .green          = { 5, 5, 0},
        .blue           = { 0, 5, 0},
        .transp         = {15, 1, 0},
        .activate       = FB_ACTIVATE_NOW,
        .pixclock       = -1,
        .left_margin    = -1,
        .right_margin   = -1,
        .upper_margin   = -1,
        .lower_margin   = -1,
        .hsync_len      = -1,
        .vsync_len      = -1,
    }
};


/**
 ** bit filed info of color fmt, the order must be the same as HIFB_COLOR_FMT_E
 **/
HIFB_ARGB_BITINFO_S s_stArgbBitField[HIFB_MAX_PIXFMT_NUM] =
{   /*RGB565*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*RGB888*/
    {
        .stRed    = {16, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB444*/
    {
        .stRed    = {8, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {0, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB555*/
    {
        .stRed    = {10, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*KRGB888*/
    {
        .stRed    = {16,8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ARGB4444*/
    {
        .stRed    = {8, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {0, 4, 0},
        .stTransp = {12, 4, 0},
    },
    /*ARGB1555*/
    {
        .stRed    = {10, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {15, 1, 0},
    },
    /*ARGB8888*/
    {
        .stRed    = {16, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {24, 8, 0},
    },
    /*ARGB8565*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {0, 5, 0},
        .stTransp = {16, 8, 0},
    },
    /*RGBA4444*/
    {
        .stRed    = {12, 4, 0},
        .stGreen  = {8, 4, 0},
        .stBlue   = {4, 4, 0},
        .stTransp = {0, 4, 0},
    },
    /*RGBA5551*/
    {
        .stRed    = {11, 5, 0},
        .stGreen  = {6, 5, 0},
        .stBlue   = {1, 5, 0},
        .stTransp = {0, 1, 0},
    },
    /*RGBA5658*/
    {
        .stRed    = {19, 5, 0},
        .stGreen  = {13, 6, 0},
        .stBlue   = {8, 5, 0},
        .stTransp = {0, 8, 0},
    },
    /*RGBA8888*/
    {
        .stRed    = {24, 8, 0},
        .stGreen  = {16, 8, 0},
        .stBlue   = {8, 8, 0},
        .stTransp = {0, 8, 0},
    },
    /*BGR565*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {11, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*BGR888*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ABGR4444*/
    {
        .stRed    = {0, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {8, 4, 0},
        .stTransp = {12, 4, 0},
    },
    /*ABGR1555*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {10, 5, 0},
        .stTransp = {15, 1, 0},
    },
    /*ABGR8888*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {24, 8, 0},
    },
    /*ABGR8565*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 6, 0},
        .stBlue   = {11, 5, 0},
        .stTransp = {16, 8, 0},
    },
    /*KBGR444 16bpp*/
    {
        .stRed    = {0, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {8, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*KBGR555 16bpp*/
    {
        .stRed    = {0, 5, 0},
        .stGreen  = {5, 5, 0},
        .stBlue   = {10, 5, 0},
        .stTransp = {0, 0, 0},
    },
    /*KBGR888 32bpp*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {16, 8, 0},
        .stTransp = {0, 0, 0},
    },

    /*1bpp*/
    {
        .stRed    = {0, 1, 0},
        .stGreen  = {0, 1, 0},
        .stBlue   = {0, 1, 0},
        .stTransp = {0, 0, 0},
    },
    /*2bpp*/
    {
        .stRed    = {0, 2, 0},
        .stGreen  = {0, 2, 0},
        .stBlue   = {0, 2, 0},
        .stTransp = {0, 0, 0},
    },
    /*4bpp*/
    {
        .stRed    = {0, 4, 0},
        .stGreen  = {0, 4, 0},
        .stBlue   = {0, 4, 0},
        .stTransp = {0, 0, 0},
    },
    /*8bpp*/
    {
        .stRed    = {0, 8, 0},
        .stGreen  = {0, 8, 0},
        .stBlue   = {0, 8, 0},
        .stTransp = {0, 0, 0},
    },
    /*ACLUT44*/
    {
        .stRed    = {4, 4, 0},
        .stGreen  = {4, 4, 0},
        .stBlue   = {4, 4, 0},
        .stTransp = {0, 4, 0},
    },
    /*ACLUT88*/
    {
        .stRed    = {8, 8, 0},
        .stGreen  = {8, 8, 0},
        .stBlue   = {8, 8, 0},
        .stTransp = {0, 8, 0},
    }
};


static HI_S32 DRV_HIFB_Open    (struct fb_info *info, HI_S32 user);
static HI_S32 DRV_HIFB_Close   (struct fb_info *info, HI_S32 user);
static HI_S32 hifb_check_var   (struct fb_var_screeninfo *var, struct fb_info *info);
static HI_S32 hifb_set_par     (struct fb_info *info);
static HI_S32 hifb_ioctl       (struct fb_info *info, HI_U32 cmd, unsigned long arg);
static HI_S32 hifb_setcmap     (struct fb_cmap *cmap, struct fb_info *info);
#ifdef CONFIG_DMA_SHARED_BUFFER
static struct dma_buf* hifb_dmabuf_export (struct fb_info *info);
#endif
#ifdef CONFIG_GFX_MMU_SUPPORT
static HI_S32 hifb_map_mem_kernel_to_user (struct fb_info *info, struct vm_area_struct *vma);
#endif
static HI_S32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green,unsigned blue, unsigned transp, struct fb_info *info);
#ifdef CFG_HIFB_SUPPORT_CONSOLE
static HI_VOID hifb_fillrect   (struct fb_info *p, const struct fb_fillrect *rect);
static HI_VOID hifb_copyarea   (struct fb_info *p, const struct fb_copyarea *area);
static HI_VOID hifb_imageblit  (struct fb_info *p, const struct fb_image *image);
#endif

extern HI_S32 DRV_HIFB_PanDisplay(struct fb_var_screeninfo *var, struct fb_info *info);

#ifdef CFG_HIFB_FENCE_SUPPORT
extern HI_S32 DRV_HIFB_HwcRefresh(HIFB_PAR_S* pstPar, HI_VOID *pArgs);
#endif

#ifdef CONFIG_COMPAT
static HI_S32 hifb_compat_ioctl (struct fb_info *info, HI_U32 cmd, unsigned long arg);
#endif


static struct fb_ops s_sthifbops =
{
    .owner          = THIS_MODULE,
    .fb_open        = DRV_HIFB_Open,
    .fb_release     = DRV_HIFB_Close,
    .fb_check_var   = hifb_check_var,
    .fb_set_par     = hifb_set_par,
    .fb_pan_display = DRV_HIFB_PanDisplay,
#ifdef CONFIG_COMPAT
    .fb_compat_ioctl= hifb_compat_ioctl,
#endif
    .fb_ioctl       = hifb_ioctl,
    .fb_setcolreg   = hifb_setcolreg,
    .fb_setcmap     = hifb_setcmap,
#ifdef CFG_HIFB_SUPPORT_CONSOLE
    .fb_fillrect    = hifb_fillrect,
    .fb_copyarea    = hifb_copyarea,
    .fb_imageblit   = hifb_imageblit,
#endif
#ifdef CONFIG_DMA_SHARED_BUFFER
    .fb_dmabuf_export = hifb_dmabuf_export,
#endif
#ifdef CONFIG_GFX_MMU_SUPPORT
    .fb_mmap          = hifb_map_mem_kernel_to_user
#endif
};



static HI_S32 HIFB_OSR_GetColorKey         (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetColorKey         (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetLayerAlpha       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetLayerAlpha       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetScreenOriginPos  (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetScreenOriginPos  (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetDeFlicker        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetDeFlicker        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetVblank           (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ShowLayer           (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetLayerShowState   (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetCapablity        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetDecompress       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetLayerInfo        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetLayerInfo        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetCanvasBuffer     (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_RefreshLayer        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_WaiteRefreshFinish  (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetSteroFraming     (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetSteroFraming     (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetSteroMode        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetSteroMode        (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetScreenSize       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetScreenSize       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetCompress         (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetCompress         (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ScrolltextCreate    (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ScrolltextFill      (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ScrolltextPause     (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ScrolltextResume    (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ScrolltextDestory   (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetSteroDepth       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetSteroDepth       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetLayerZorder      (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetLayerZorder      (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_ReleaseLogo         (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_SetCompressionMode  (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetCompressionMode  (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_HwcRefresh          (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_GetSmemStartPhy     (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_FixScreenInfo       (struct fb_info *info, HI_ULONG arg);
static HI_S32 HIFB_OSR_HwcGetLayerInfo     (struct fb_info *info, HI_ULONG arg);


static HI_S32 gs_DrvHifbCtlNum[DRV_HIFB_IOCTLCMD_NUM_MAX] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,/** index = 89 **/
     1,  2,  3,  4,  5,  6,  0,  0,  7,  8, 9,
     11, 12, 13, 15,/** index = 104 **/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,/** index = 119 **/
     17, 18, 0,  19, 21, 22, 23, 24, 25, 26, /** index = 129 **/
     27, 28, 29, 30, 31, 32, 33, 34, 35, 37, /** index = 139 **/
     38, 39, 40, 41, 43, 44, 45, 47, 48, 49
   };

static DRV_HIFB_IOCTLFUNC_ITEM gs_DrvHifbCtlFunc[DRV_HIFB_IOCTLFUNC_ITEM_NUM_MAX] = {{0,NULL},
   {FBIOGET_COLORKEY_HIFB,      HIFB_OSR_GetColorKey},            {FBIOPUT_COLORKEY_HIFB,      HIFB_OSR_SetColorKey},
   {FBIOGET_ALPHA_HIFB,         HIFB_OSR_GetLayerAlpha},          {FBIOPUT_ALPHA_HIFB,         HIFB_OSR_SetLayerAlpha},
   {FBIOGET_SCREEN_ORIGIN_HIFB, HIFB_OSR_GetScreenOriginPos},     {FBIOPUT_SCREEN_ORIGIN_HIFB, HIFB_OSR_SetScreenOriginPos},
   {FBIOGET_DEFLICKER_HIFB,     HIFB_OSR_GetDeFlicker},           {FBIOPUT_DEFLICKER_HIFB,     HIFB_OSR_SetDeFlicker},
   {FBIOGET_VBLANK_HIFB,        HIFB_OSR_GetVblank},              {0, NULL},/** index = 10**/
   {FBIOPUT_SHOW_HIFB,          HIFB_OSR_ShowLayer},              {FBIOGET_SHOW_HIFB,          HIFB_OSR_GetLayerShowState},
   {FBIOGET_CAPABILITY_HIFB,    HIFB_OSR_GetCapablity},           {0, NULL},
   {FBIOPUT_DECOMPRESS_HIFB,    HIFB_OSR_SetDecompress},          {0, NULL},
   {FBIOPUT_LAYER_INFO,         HIFB_OSR_SetLayerInfo},           {FBIOGET_LAYER_INFO,         HIFB_OSR_GetLayerInfo},
   {FBIOGET_CANVAS_BUFFER,      HIFB_OSR_GetCanvasBuffer},        {0, NULL},/** index = 20**/
   {FBIO_REFRESH,               HIFB_OSR_RefreshLayer},           {FBIO_WAITFOR_FREFRESH_DONE, HIFB_OSR_WaiteRefreshFinish},
   {FBIOPUT_ENCODER_PICTURE_FRAMING, HIFB_OSR_SetSteroFraming},   {FBIOGET_ENCODER_PICTURE_FRAMING,  HIFB_OSR_GetSteroFraming},
   {FBIOPUT_STEREO_MODE,        HIFB_OSR_SetSteroMode},           {FBIOGET_STEREO_MODE,        HIFB_OSR_GetSteroMode},
   {FBIOPUT_SCREENSIZE,         HIFB_OSR_SetScreenSize},          {FBIOGET_SCREENSIZE,         HIFB_OSR_GetScreenSize},
   {FBIOPUT_COMPRESSION,        HIFB_OSR_SetCompress},            {FBIOGET_COMPRESSION,        HIFB_OSR_GetCompress},/** index = 30**/
   {FBIO_SCROLLTEXT_CREATE,     HIFB_OSR_ScrolltextCreate},       {FBIO_SCROLLTEXT_FILL,       HIFB_OSR_ScrolltextFill},
   {FBIO_SCROLLTEXT_PAUSE,      HIFB_OSR_ScrolltextPause},        {FBIO_SCROLLTEXT_RESUME,     HIFB_OSR_ScrolltextResume},
   {FBIO_SCROLLTEXT_DESTORY,    HIFB_OSR_ScrolltextDestory},      {0, NULL},
   {FBIOPUT_STEREO_DEPTH,       HIFB_OSR_SetSteroDepth},          {FBIOGET_STEREO_DEPTH,       HIFB_OSR_GetSteroDepth},
   {FBIOPUT_ZORDER,             HIFB_OSR_SetLayerZorder},         {FBIOGET_ZORDER,             HIFB_OSR_GetLayerZorder},/** index = 40**/
   {FBIO_FREE_LOGO,             HIFB_OSR_ReleaseLogo},            {0,NULL},
   {FBIOPUT_COMPRESSIONMODE,    HIFB_OSR_SetCompressionMode},     {FBIOGET_COMPRESSIONMODE,    HIFB_OSR_GetCompressionMode},
   {FBIO_HWC_REFRESH,           HIFB_OSR_HwcRefresh},             {0,NULL},
   {FBIOGET_SMEMSTART_PHY,      HIFB_OSR_GetSmemStartPhy},        {FBIO_HWC_GETLAYERINFO,      HIFB_OSR_HwcGetLayerInfo},
   {FBIOGET_FSCREENINFO_HIFB,   HIFB_OSR_FixScreenInfo}};


/******************************* API declaration *****************************/

static HI_S32  hifb_refresh(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf, HIFB_LAYER_BUF_E enBufMode);
static HI_VOID hifb_select_antiflicker_mode(HIFB_PAR_S *pstPar);
static HI_S32  hifb_refreshall(struct fb_info *info);
static HI_VOID hifb_assign_dispbuf(HI_U32 u32LayerId);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
static HI_S32  DRV_OSR_StereoCallBack   (HI_VOID * pParaml,HI_VOID * pParamr);
#endif

#ifdef CONFIG_DMA_SHARED_BUFFER
extern struct dma_buf *hifb_memblock_export(phys_addr_t base, size_t size, int flags);
#endif

static HI_S32 DRV_HIFB_ParseInsmodParameter(HI_VOID);
static inline unsigned long DRV_HIFB_GetVramSize(HI_CHAR* pstr);
static inline HI_S32  DRV_HIFB_RegisterFrameBuffer   (HI_U32 u32LayerId);
static inline HI_VOID DRV_HIFB_UnRegisterFrameBuffer (HI_U32 u32LayerId);
static inline HI_S32 DRV_HIFB_ReAllocLayerBuffer     (HI_U32 u32LayerId, HI_U32 BufSize);
static inline HI_S32 DRV_HIFB_AllocLayerBuffer       (HI_U32 u32LayerId, HI_U32 BufSize);
static inline HI_VOID DRV_HIFB_FreeLayerBuffer       (HI_U32 u32LayerId);
static inline HI_S32  DRV_HIFB_InitAllocCmapBuffer   (HI_U32 u32LayerId);
static inline HI_VOID DRV_HIFB_DInitAllocCmapBuffer  (HI_U32 u32LayerId);
static inline HI_BOOL DRV_HIFB_CheckSmmuSupport      (HI_U32 LayerId);
static inline HI_VOID DRV_HIFB_PrintVersion(HI_VOID);


static HI_S32  DRV_HIFB_InitLayerInfo      (HI_U32 u32LayerID);
static HI_S32  DRV_HIFB_GetLayerBufferInfo (HI_U32 u32LayerID);
static HI_VOID DRV_HIFB_SetDisplayBuffer   (HI_U32 u32LayerId);
static inline  HI_S32 DRV_HIFB_RegisterCallBackFunction (HI_U32 u32LayerId);
static inline  HI_VOID DRV_HIFB_ReleaseScrollText (struct fb_info *info);


static inline HI_VOID HIFB_OSR_UpdataRefreshInfo(HI_U32 LayerId);
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
static inline HI_VOID HIFB_OSR_UpdataCmp(HI_U32 LayerId);
static inline HI_VOID HIFB_OSR_UpdataInputDataCmpRect(HI_U32 LayerId);
#endif
static inline HI_VOID HIFB_OSR_UpdataInputDataFmt    (HI_U32 LayerId);
static inline HI_VOID HIFB_OSR_UpdataInputDataStride (HI_U32 LayerId);
static inline HI_VOID HIFB_OSR_UpdataInputDataDDR    (HI_U32 LayerId);
static inline HI_VOID HIFB_OSR_SwitchDispBuffer      (HI_U32 LayerId);

static inline HI_S32  HIFB_OSR_CheckWhetherResolutionSupport(HI_U32 LayerId, HI_U32 Width, HI_U32 Height);
static inline HI_VOID HIFB_OSR_SetDispInfo(HI_U32 LayerId);
static inline HI_S32  HIFB_OSR_CheckWhetherLayerSizeSupport(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo);
static inline HI_S32  HIFB_OSR_CheckWhetherLayerPosSupport(HIFB_LAYER_INFO_S *pstLayerInfo);
static inline HI_VOID HIFB_OSR_CheckWhetherDispSizeChange(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo);
static inline HI_S32  HIFB_OSR_CheckWhetherMemSizeEnough(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo);
static inline HI_S32  HIFB_OSR_CheckWhetherScreenSizeEqualToZero(HIFB_LAYER_INFO_S *pstLayerInfo);


#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
static inline HI_S32 HIFB_OSR_SCROLLTEXT_Enable(HI_BOOL bEnable, HI_ULONG arg);
#endif

#ifdef CFG_HIFB_PROC_SUPPORT
extern HI_VOID HI_UNF_HIFB_CaptureImgFromLayer(HI_U32 u32LayerID, HI_BOOL bAlphaEnable);
#endif

/******************************* API realization *****************************/


/***************************************************************************************
* func          : hifb_getfmtbyargb
* description   : CNcomment: 从argb中判断像素格式 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HIFB_COLOR_FMT_E hifb_getfmtbyargb(
                                          struct fb_bitfield *red,
                                          struct fb_bitfield *green,
                                          struct fb_bitfield *blue,
                                          struct fb_bitfield *transp,
                                          HI_U32 u32ColorDepth)
{

    HI_U32 i      = 0;
    HI_U32 u32Bpp = 0;
#if 0
    if(8 == u32ColorDepth){
        return HIFB_FMT_8BPP;
    }
#endif

    for (i = 0; i < sizeof(s_stArgbBitField)/sizeof(HIFB_ARGB_BITINFO_S); i++){
        if (   (hifb_bitfieldcmp(*red, s_stArgbBitField[i].stRed)       == 0)
            && (hifb_bitfieldcmp(*green, s_stArgbBitField[i].stGreen)   == 0)
            && (hifb_bitfieldcmp(*blue, s_stArgbBitField[i].stBlue)     == 0)
            && (hifb_bitfieldcmp(*transp, s_stArgbBitField[i].stTransp) == 0)){
            u32Bpp = DRV_HIFB_GetBppByFmt(i);
            if (u32Bpp == u32ColorDepth){
                return i;
            }
        }
    }

    switch(u32ColorDepth)
    {
        case 1:
            i = HIFB_FMT_1BPP;
            break;
        case 2:
            i = HIFB_FMT_2BPP;
            break;
        case 4:
            i = HIFB_FMT_4BPP;
            break;
        case 8:
            i = HIFB_FMT_8BPP;
            break;
        case 16:
            i = HIFB_FMT_RGB565;
            break;
        case 24:
            i = HIFB_FMT_RGB888;
            break;
        case 32:
            i = HIFB_FMT_ARGB8888;
            break;
        default:
            i = HIFB_FMT_BUTT;
            break;
    }
    if(HIFB_FMT_BUTT != i)
    {
        *red    = s_stArgbBitField[i].stRed;
        *green  = s_stArgbBitField[i].stGreen;
        *blue   = s_stArgbBitField[i].stBlue;
        *transp = s_stArgbBitField[i].stTransp;
    }
    return i;
}


/***************************************************************************************
* func          : HIFB_ReAllocWork
* description   : CNcomment: 重新分配内存，旧内存释放处理,放这里的原因是为了
                             保证这快内存不再被逻辑使用的时候才释放，这样就不会发生访问页表错误
                             CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID HIFB_ReAllocWork(struct work_struct *work)
{
    HIFB_DISP_TMPBUF_S  *pstDispTmpBuf = NULL;
    pstDispTmpBuf = (HIFB_DISP_TMPBUF_S *)container_of(work, HIFB_DISP_TMPBUF_S, bDisBufWork);
    if(NULL != pstDispTmpBuf->pVirBuf){
        hifb_buf_ummap(pstDispTmpBuf->pVirBuf);
    }
    if(0 != pstDispTmpBuf->u32SmemStartPhy){
        hifb_buf_unmapsmmu(pstDispTmpBuf->u32PhyBuf);
        hifb_buf_freemem(pstDispTmpBuf->u32SmemStartPhy,HI_FALSE);
    }else{
        hifb_buf_freemem(pstDispTmpBuf->u32PhyBuf,HI_TRUE);
    }
    pstDispTmpBuf->pVirBuf         = NULL;
    pstDispTmpBuf->u32PhyBuf       = 0;
    pstDispTmpBuf->u32SmemStartPhy = 0;
}


/***************************************************************************************
* func          : hifb_checkmem_enough
* description   : CNcomment: 判断内存是否足够 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_S32 hifb_checkmem_enough(struct fb_info *info,HI_U32 u32Stride,HI_U32 u32Height)
{
    HI_U32 u32BufferNum = 0;
    HI_U32 u32BufSize   = 0;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    switch(par->stExtendInfo.enBufMode){
        case HIFB_LAYER_BUF_DOUBLE:
        case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
            u32BufferNum = 2;
            break;
        case HIFB_LAYER_BUF_ONE:
        case HIFB_REFRESH_MODE_WITH_PANDISPLAY:
            u32BufferNum = 1;
            break;
        default:
            u32BufferNum = 0;
            break;
    }
    u32BufSize = HI_HIFB_GetMemSize(u32Stride,u32Height);
    u32BufSize = u32BufferNum * u32BufSize;
    if(info->fix.smem_len >= u32BufSize){
        return HI_SUCCESS;
    }else{
        HIFB_ERROR("memory is not enough!  now is %d u32Pitch %d u32Height %d expect %d\n",info->fix.smem_len,u32Stride,u32Height,u32BufSize);
        return HI_FAILURE;
    }
}


/***************************************************************************************
* func          : hifb_check_fmt
* description   : CNcomment: 判断像素格式是否合法 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_check_fmt(struct fb_var_screeninfo *var, struct fb_info *info)
{

    HI_U32 u32MaxXRes = 0;
    HI_U32 u32MaxYRes = 0;
    HIFB_PAR_S *par    = NULL;
    HI_U32 u32LayerID  = 0;
    HIFB_COLOR_FMT_E enFmt = HIFB_FMT_BUTT;

    par = (HIFB_PAR_S *)info->par;
    u32LayerID = par->stBaseInfo.u32LayerID;

    /**
     **用户会设置可变屏幕信息
     **/
    enFmt = hifb_getfmtbyargb(&var->red,
                                &var->green,
                                &var->blue,
                                &var->transp,
                                var->bits_per_pixel);
    if (enFmt == HIFB_FMT_BUTT)
    {
        HIFB_ERROR("Unknown fmt(offset, length) \
                     r:(%d,%d,%d) ,              \
                     g:(%d,%d,%d),               \
                     b(%d,%d,%d),                \
                     a(%d,%d,%d),                \
                     bpp:%d!\n",                 \
                    var->red.offset,    var->red.length,    var->red.msb_right,   \
                    var->green.offset,  var->green.length,  var->green.msb_right, \
                    var->blue.offset,   var->blue.length,   var->blue.msb_right,  \
                    var->transp.offset, var->transp.length, var->transp.msb_right,\
                    var->bits_per_pixel);
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return -EINVAL;
    }

    /**
     **初始化的时候已经获取到gs_pstCapacity的值信息
     **/
    if (  (!gs_pstCapacity[par->stBaseInfo.u32LayerID].bColFmt[enFmt])
        || (!g_stGfx2dCallBackFunction.HIFB_DRV_TdeSupportFmt(enFmt) && par->stExtendInfo.enBufMode != HIFB_REFRESH_MODE_WITH_PANDISPLAY))
    {
        HIFB_ERROR("Unsupported PIXEL FORMAT!\n");
        return -EINVAL;
    }

    /**
     ** virtual resolution must be no less than minimal resolution
     **/
    if (var->xres_virtual < HIFB_MIN_WIDTH(u32LayerID))
    {
        var->xres_virtual = HIFB_MIN_WIDTH(u32LayerID);
    }
    if (var->yres_virtual < HIFB_MIN_HEIGHT(u32LayerID))
    {
        var->yres_virtual = HIFB_MIN_HEIGHT(u32LayerID);
    }

    /**
     ** just needed to campare display resolution with virtual resolution,
     ** because VO graphic layer can do scaler,display resolution >current
     ** standard resolution
     **/
    u32MaxXRes = var->xres_virtual;
    if (var->xres > u32MaxXRes)
    {
        var->xres = u32MaxXRes;
    }
    else if (var->xres < HIFB_MIN_WIDTH(u32LayerID))
    {
        var->xres = HIFB_MIN_WIDTH(u32LayerID);
    }

    u32MaxYRes = var->yres_virtual;
    if (var->yres > u32MaxYRes)
    {
        var->yres = u32MaxYRes;
    }
    else if (var->yres < HIFB_MIN_HEIGHT(u32LayerID))
    {
        var->yres = HIFB_MIN_HEIGHT(u32LayerID);
    }

    HIFB_INFO("xres:%d,   yres:%d,    xres_virtual:%d,    yres_virtual:%d\n",
                var->xres, var->yres, var->xres_virtual,  var->yres_virtual);

    /**
     ** check if the offset is valid
     **/
    if (   (var->xoffset > var->xres_virtual)
        || (var->yoffset > var->yres_virtual)
        || (var->xoffset + var->xres > var->xres_virtual)
        || (var->yoffset + var->yres > var->yres_virtual))
    {
        HIFB_ERROR("offset is invalid! xoffset:%d, yoffset:%d\n", var->xoffset, var->yoffset);
        return -EINVAL;
    }

    return HI_SUCCESS;

}



/***************************************************************************************
* func          : hifb_check_var
* description   : CNcomment: 判断参数是否合法 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    if (pstPar->stBaseInfo.u32LayerID == HIFB_LAYER_CURSOR)
    {
        HIFB_ERROR("cursor layer doesn't support this operation!\n");
        return HI_FAILURE;
    }
    return hifb_check_fmt(var, info);
}

/***************************************************************************************
* func          : hifb_assign_dispbuf
* description   : CNcomment: 分配display buffer CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID hifb_assign_dispbuf(HI_U32 u32LayerId)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)(info->par);
    HI_U32 u32BufSize  = 0;
    HI_U32 u32PixDepth = 0;

    if (pstPar->st3DInfo.IsStereo)
    {
        HI_U32 CmpStride = 0;
        HI_U32 Stride = 0;
        HI_U32 u32StartAddr = 0;

        u32PixDepth = info->var.bits_per_pixel >> 3;
        Stride      = CONIFG_HIFB_GetMaxStride(info->var.xres,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
        u32BufSize  = HI_HIFB_GetMemSize(Stride,info->var.yres);

        if (IS_2BUF_MODE(pstPar) || IS_1BUF_MODE(pstPar))
        {
            u32StartAddr = info->fix.smem_start;
            if (1 == pstPar->stRunInfo.StereoBufNum)
            {
                pstPar->st3DInfo.u32DisplayAddr[0] = u32StartAddr;
                pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr;
            }
            else if (2 == pstPar->stRunInfo.StereoBufNum)
            {
                pstPar->st3DInfo.u32DisplayAddr[0] = u32StartAddr;
                pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr + u32BufSize;
            }
            return;
        }
        else if (  (0 == pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart)
                || (0 == pstPar->st3DInfo.st3DMemInfo.u32StereoMemLen)
                || (0 == pstPar->stRunInfo.StereoBufNum))
        {
            return;
        }
        else
        {/**<-- android used pandisplay refresh **/
            u32StartAddr = pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart;
        }

        if (1 == pstPar->stRunInfo.StereoBufNum)
        {
            pstPar->st3DInfo.u32DisplayAddr[0] = u32StartAddr;
            pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr;
        }
        else if (2 == pstPar->stRunInfo.StereoBufNum)
        {/**<-- generally is two buffer **/
            pstPar->st3DInfo.u32DisplayAddr[0] = u32StartAddr;
            if (HIFB_STEREO_SIDEBYSIDE_HALF == pstPar->st3DInfo.StereoMode)
            {
                pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr + pstPar->st3DInfo.st3DSurface.u32Pitch / 2;
            }
            else if (HIFB_STEREO_TOPANDBOTTOM == pstPar->st3DInfo.StereoMode)
            {
                pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr + pstPar->st3DInfo.st3DSurface.u32Pitch * pstPar->stExtendInfo.u32DisplayHeight/ 2;
            }
            else
            {/**<-- frame packing user display buffer, no user 3d buffer **/
                pstPar->st3DInfo.u32DisplayAddr[1] = u32StartAddr + u32BufSize;
            }
        }
    }
    else
#endif
    {
        DRV_HIFB_SetDisplayBuffer(u32LayerId);
    }

    return;
}


/***************************************************************************************
* func          : hifb_getupdate_rect
* description   : CNcomment: 获取图层的更新区域 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_getupdate_rect(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf, HIFB_RECT *pstUpdateRect)
{
    HIFB_PAR_S *pstPar;
    struct fb_info *info;

    TDE2_RECT_S SrcRect   = {0};
    TDE2_RECT_S DstRect   = {0};
    TDE2_RECT_S InSrcRect = {0};
    TDE2_RECT_S InDstRect = {0};

    info     = s_stLayer[u32LayerId].pstInfo;
    pstPar   = (HIFB_PAR_S *)info->par;

    HI_GFX_Memset(&InDstRect, 0, sizeof(TDE2_RECT_S));

    SrcRect.u32Width  = pstCanvasBuf->stCanvas.u32Width;
    SrcRect.u32Height = pstCanvasBuf->stCanvas.u32Height;
    if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_SIDEBYSIDE_HALF)
    {
        DstRect.u32Width  = pstPar->stExtendInfo.u32DisplayWidth >> 1;
        DstRect.u32Height = pstPar->stExtendInfo.u32DisplayHeight;
    }
    else if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_TOPANDBOTTOM)
    {
        DstRect.u32Width  = pstPar->stExtendInfo.u32DisplayWidth;
        DstRect.u32Height = pstPar->stExtendInfo.u32DisplayHeight >> 1;
    }
    else
    {
        DstRect.u32Width  = pstPar->stExtendInfo.u32DisplayWidth;
        DstRect.u32Height = pstPar->stExtendInfo.u32DisplayHeight;
    }

    if(    SrcRect.u32Width  != DstRect.u32Width
        || SrcRect.u32Height != DstRect.u32Height)
    {
        HI_GFX_Memcpy(&InSrcRect, &pstCanvasBuf->UpdateRect, sizeof(HIFB_RECT));
        g_stGfx2dCallBackFunction.HIFB_DRV_CalScaleRect(&SrcRect, &DstRect, &InSrcRect, &InDstRect);
        HI_GFX_Memcpy(pstUpdateRect, &InDstRect, sizeof(HIFB_RECT));
    }
    else
    {
        HI_GFX_Memcpy(pstUpdateRect, &pstCanvasBuf->UpdateRect, sizeof(HIFB_RECT));
    }

    return HI_SUCCESS;

}

/***************************************************************************************
* func            : hifb_backup_forebuf
* description    : CNcomment: 更新前景数据CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
static HI_S32 hifb_backup_forebuf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstBackBuf)
{
    HI_S32 s32Ret;
    HI_U32 u32ForePhyAddr;
    HIFB_PAR_S *pstPar;
    HIFB_RECT  *pstForeUpdateRect;
    struct fb_info *info;
    HIFB_BUFFER_S stForeBuf;
    HIFB_BUFFER_S stBackBuf;
    HIFB_BLIT_OPT_S stBlitTmp;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    HI_GFX_Memcpy(&stBackBuf, pstBackBuf, sizeof(HIFB_BUFFER_S));

    if (   pstPar->st3DInfo.StereoMode != HIFB_STEREO_MONO
        && pstPar->st3DInfo.StereoMode != HIFB_STEREO_BUTT)
    {
        pstForeUpdateRect = &pstPar->st3DInfo.st3DUpdateRect;
        u32ForePhyAddr= pstPar->st3DInfo.u32DisplayAddr[1-pstPar->stRunInfo.u32IndexForInt];
    }
    else
    {
        pstForeUpdateRect = &pstPar->stDispInfo.stUpdateRect;
        u32ForePhyAddr= pstPar->stDispInfo.u32DisplayAddr[1-pstPar->stRunInfo.u32IndexForInt];
    }

    if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_SIDEBYSIDE_HALF)
    {
        stBackBuf.stCanvas.u32Width  = stBackBuf.stCanvas.u32Width >> 1;
    }
    else if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_TOPANDBOTTOM)
    {
        stBackBuf.stCanvas.u32Height = stBackBuf.stCanvas.u32Height >> 1;
    }

    if (0 == pstForeUpdateRect->w)
    {
        pstForeUpdateRect->w = stBackBuf.stCanvas.u32Width;
    }

    if (0 == pstForeUpdateRect->h)
    {
        pstForeUpdateRect->h = stBackBuf.stCanvas.u32Height;
    }
    /** backup fore buffer **/
    if (!hifb_iscontain(&stBackBuf.UpdateRect, pstForeUpdateRect))
    {
        HI_GFX_Memcpy(&stForeBuf, &stBackBuf, sizeof(HIFB_BUFFER_S));
        stForeBuf.stCanvas.u32PhyAddr = u32ForePhyAddr;
        HI_GFX_Memcpy(&stForeBuf.UpdateRect, pstForeUpdateRect, sizeof(HIFB_RECT));
        HI_GFX_Memcpy(&stBackBuf.UpdateRect, &stForeBuf.UpdateRect , sizeof(HIFB_RECT));
        HI_GFX_Memset(&stBlitTmp, 0x0, sizeof(stBlitTmp));

        s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(&stForeBuf, &stBackBuf, &stBlitTmp, HI_TRUE);
        if (s32Ret <= 0){
            HIFB_ERROR("2buf  blit err 4!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


/***************************************************************************************
* func          : hifb_disp_setdispsize
* description   : CNcomment: set display size CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_disp_setdispsize(HI_U32 u32LayerId, HI_U32 u32Width, HI_U32 u32Height)
{
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    HI_U32 u32Stride = 0;
    HI_U32 CmpStride = 0;
    HI_U32 u32Depth  = 0;
    if ((pstPar->stExtendInfo.u32DisplayWidth == u32Width) && (pstPar->stExtendInfo.u32DisplayHeight == u32Height))
    {
        return HI_SUCCESS;
    }

    u32Depth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(u32Width,u32Depth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    if(HI_FAILURE == hifb_checkmem_enough(info, u32Stride, u32Height)){
       HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
       return HI_FAILURE;
    }

    pstPar->stExtendInfo.u32DisplayWidth  = u32Width;
    pstPar->stExtendInfo.u32DisplayHeight = u32Height;

    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;

    /** here we need to think about how to resist flicker again,
     ** we use VO do flicker resist before , but now if the display H size is the
     ** same as the screen, VO will not do flicker resist, so should choose TDE
     ** to do flicker resist
     **/
    hifb_select_antiflicker_mode(pstPar);

    return HI_SUCCESS;

}


  /* we handle it by two case:
      case 1 : if VO support Zoom, we only change screeen size, display size keep not change
      case 2: if VO can't support zoom, display size should keep the same as screen size*/
static HI_S32 hifb_disp_setscreensize(HI_U32 u32LayerId, HI_U32 u32Width, HI_U32 u32Height)
{
#ifndef CFG_HIFB_VIRTUAL_COORDINATE_SUPPORT
    HIFB_RECT stDispRect;
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

    if (0 == u32Width || 0 == u32Height)
    {
        return HI_FAILURE;
    }

    pstPar->stExtendInfo.u32ScreenWidth  = u32Width;
    pstPar->stExtendInfo.u32ScreenHeight = u32Height;

    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_OUTRECT;

    /* Here  we need to think about how to resist flicker again, we use VO do flicker resist before , but now if the display H size is the
             same as the screen, VO will not do flicker resist, so should choose TDE to do flicker resist*/
    hifb_select_antiflicker_mode(pstPar);
#endif
    return HI_SUCCESS;
}


/***************************************************************************************
* func          : hifb_set_canvasbufinfo
* description   : CNcomment: 设置canvas buffer 信息 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID hifb_set_canvasbufinfo(struct fb_info *info, HIFB_LAYER_INFO_S * pLayerInfo)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_U32 u32DepTh  = 0;
    HI_U32 u32Stride = 0;
    HI_U32 CmpStride = 0;

    if(!(pLayerInfo->u32Mask & HIFB_LAYERMASK_CANVASSIZE)){
        return;
    }
    u32DepTh = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(pLayerInfo->u32CanvasWidth,u32DepTh,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    par->stDispInfo.stCanvasSur.u32Width  = pLayerInfo->u32CanvasWidth;
    par->stDispInfo.stCanvasSur.u32Height = pLayerInfo->u32CanvasHeight;
    par->stDispInfo.stCanvasSur.enFmt     = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp, info->var.bits_per_pixel);
    par->stDispInfo.stCanvasSur.u32Pitch  = u32Stride;

    return;
}


#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
static HI_VOID HIFB_OSR_FreeStereoBuf(HIFB_PAR_S *par)
{
    g_stGfx2dCallBackFunction.HIFB_DRV_WaitAllTdeDone(HI_TRUE);

    hifb_buf_freemem(par->st3DInfo.st3DMemInfo.u32StereoMemStart,HI_TRUE);

    par->st3DInfo.st3DMemInfo.u32StereoMemStart = 0;
    par->st3DInfo.st3DMemInfo.u32StereoMemLen   = 0;
    par->st3DInfo.st3DSurface.u32PhyAddr        = 0;

    return;
}


static HI_S32 HIFB_OSR_AllocStereoBuf(struct fb_info *info, HI_U32 u32BufSize)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_CHAR name[32] = "\0";

    if (0 == u32BufSize)
    {
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    if (u32BufSize == par->st3DInfo.st3DMemInfo.u32StereoMemLen)
    {
        return HI_SUCCESS;
    }

    if (par->st3DInfo.st3DMemInfo.u32StereoMemStart)
    {
        HIFB_INFO("free old stereo buffer\n");
        HIFB_OSR_FreeStereoBuf(par);
    }

    snprintf(name, sizeof(name), "HIFB_StereoBuf%d", par->stBaseInfo.u32LayerID);
    par->st3DInfo.st3DMemInfo.u32StereoMemStart = hifb_buf_allocmem(name, "iommu", u32BufSize);

    if (0 == par->st3DInfo.st3DMemInfo.u32StereoMemStart)
    {
        HIFB_ERROR("alloc stereo buffer no mem, u32BufSize:%d\n", u32BufSize);
        return HI_FAILURE;
    }

    par->st3DInfo.st3DMemInfo.u32StereoMemLen = u32BufSize;

    return HI_SUCCESS;
}

static HI_VOID HIFB_OSR_ClearStereoBuf(struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (par->st3DInfo.st3DMemInfo.u32StereoMemStart && par->st3DInfo.st3DMemInfo.u32StereoMemLen)
    {
        HIFB_BLIT_OPT_S stOpt;
        HI_GFX_Memset(&stOpt, 0x0, sizeof(stOpt));
        par->st3DInfo.st3DSurface.u32PhyAddr = par->st3DInfo.st3DMemInfo.u32StereoMemStart;
        g_stGfx2dCallBackFunction.HIFB_DRV_ClearRect(&(par->st3DInfo.st3DSurface), &stOpt);
    }

    return;

}


static HI_S32 HIFB_OSR_ClearUnUsedStereoBuf(struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (par->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY || par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE)
    {
        HIFB_OSR_ClearStereoBuf(info);
    }
    else
    {
        HIFB_BLIT_OPT_S stOpt;
        HIFB_SURFACE_S Surface;
        HI_U32 u32PixDepth = 0;
        HI_U32 u32Stride   = 0;
        HI_U32 CmpStride   = 0;
        HI_GFX_Memset(&stOpt, 0x0, sizeof(stOpt));

        Surface.enFmt     = par->stExtendInfo.enColFmt;
        Surface.u32Height = par->stExtendInfo.u32DisplayHeight;
        Surface.u32Width  = par->stExtendInfo.u32DisplayWidth;

        u32PixDepth = info->var.bits_per_pixel >> 3;
        u32Stride = CONIFG_HIFB_GetMaxStride(par->stExtendInfo.u32DisplayWidth,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
        Surface.u32Pitch  = u32Stride;

        if (par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE
                ||par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE)
        {
            Surface.u32PhyAddr = par->st3DInfo.u32DisplayAddr[par->stRunInfo.u32IndexForInt];
        }
        else
        {
            Surface.u32PhyAddr = par->st3DInfo.u32DisplayAddr[0];
        }

        if (HI_NULL == Surface.u32PhyAddr)
        {
            HIFB_ERROR("fail to clear stereo rect.\n");
            return HI_FAILURE;
        }
        g_stGfx2dCallBackFunction.HIFB_DRV_ClearRect(&Surface, &stOpt);
    }

    return HI_SUCCESS;
}


static HI_S32 HIFB_OSR_ReAllocStereoBuf(HIFB_LAYER_ID_E enLayerId, HI_U32 u32BufferSize)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HIFB_PAR_S *pstPar = NULL;
    struct fb_info *info = NULL;

    info = s_stLayer[enLayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    if (HIFB_STEREO_MONO == pstPar->st3DInfo.StereoMode)
    {
        return HI_SUCCESS;
    }

    if ((HIFB_LAYER_BUF_NONE != pstPar->stExtendInfo.enBufMode) && (HIFB_REFRESH_MODE_WITH_PANDISPLAY != pstPar->stExtendInfo.enBufMode))
    {
        return HI_SUCCESS;
    }

    if ((HIFB_REFRESH_MODE_WITH_PANDISPLAY == pstPar->stExtendInfo.enBufMode) && (HIFB_STEREO_FRMPACKING  == pstPar->st3DInfo.StereoMode))
    {
        return HI_SUCCESS;
    }

    if (u32BufferSize > pstPar->st3DInfo.st3DMemInfo.u32StereoMemLen)
    {
        s32Ret = HIFB_OSR_AllocStereoBuf(info, u32BufferSize);
        if (s32Ret != HI_SUCCESS)
        {
            HIFB_ERROR("alloc 3D buffer failure!, expect mem size: %d\n", u32BufferSize);
            return s32Ret;
        }

        pstPar->st3DInfo.st3DSurface.u32PhyAddr = pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart;
        pstPar->st3DInfo.u32rightEyeAddr        = pstPar->st3DInfo.st3DSurface.u32PhyAddr;
        pstPar->stRunInfo.u32IndexForInt        = 0;

        HIFB_OSR_ClearStereoBuf  (info);
        hifb_assign_dispbuf(pstPar->stBaseInfo.u32LayerID);
    }

    return HI_SUCCESS;

}

#endif

/***************************************************************************************
* func          : hifb_set_par
* description   : CNcomment: 配置参数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_set_par(struct fb_info *info)
{
    HI_S32 s32Ret          = HI_SUCCESS;
    HI_U32 u32Stride       = 0;
    HI_U32 CmpStride       = 0;
    HI_U32 u32BufSize      = 0;
    HI_U32 u32PixDepth     = 0;
    HIFB_PAR_S *pstPar     = NULL;
    HIFB_COLOR_FMT_E enFmt = HIFB_FMT_ARGB8888;
    pstPar = (HIFB_PAR_S *)info->par;

    pstPar->bSetPar = HI_TRUE;

    s32Ret = HIFB_OSR_CheckWhetherResolutionSupport(pstPar->stBaseInfo.u32LayerID, info->var.xres, info->var.yres);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    enFmt = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp, info->var.bits_per_pixel);
    u32PixDepth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(info->var.xres_virtual,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    if (!pstPar->bPanFlag)
    {
        u32BufSize  = HI_HIFB_GetMemSize(u32Stride,info->var.yres_virtual);
        if (u32BufSize > info->fix.smem_len)
        {
            HI_GFX_Memset(&(pstPar->stDispTmpBuf),0,sizeof(HIFB_DISP_TMPBUF_S));
            pstPar->stDispTmpBuf.u32SmemStartPhy  = pstPar->u32SmemStartPhy;
            pstPar->stDispTmpBuf.u32PhyBuf        = info->fix.smem_start;
            pstPar->stDispTmpBuf.pVirBuf          = info->screen_base;

            s32Ret = DRV_HIFB_AllocLayerBuffer(pstPar->stBaseInfo.u32LayerID,u32BufSize);
            if (HI_FAILURE == s32Ret)
            {
                HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
                return HI_FAILURE;
            }
            info->fix.smem_len = u32BufSize;

            pstPar->stRunInfo.u32IndexForInt = 0;
            hifb_assign_dispbuf(pstPar->stBaseInfo.u32LayerID);
            pstPar->stRunInfo.bModifying          = HI_TRUE;
            pstPar->stRunInfo.u32ScreenAddr       = info->fix.smem_start;
            pstPar->st3DInfo.u32rightEyeAddr      = info->fix.smem_start;
            pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
            pstPar->stRunInfo.bModifying          = HI_FALSE;

            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(pstPar->stBaseInfo.u32LayerID, info->fix.smem_start);

            INIT_WORK(&(pstPar->stDispTmpBuf.bDisBufWork), HIFB_ReAllocWork);
            pstPar->stDispTmpBuf.bStartRelease = HI_TRUE;
        }
    }
    else
    {
        s32Ret = hifb_checkmem_enough(info, u32Stride, info->var.yres_virtual);
        if (HI_FAILURE == s32Ret)
        {
            HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
            return HI_FAILURE;
        }
    }

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if ((pstPar->st3DInfo.IsStereo) && (HIFB_REFRESH_MODE_WITH_PANDISPLAY == pstPar->stExtendInfo.enBufMode))
    {
        u32PixDepth = info->var.bits_per_pixel >> 3;
        u32Stride = CONIFG_HIFB_GetMaxStride(info->var.xres,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

        pstPar->st3DInfo.st3DSurface.enFmt     = enFmt;
        pstPar->st3DInfo.st3DSurface.u32Width  = info->var.xres;
        pstPar->st3DInfo.st3DSurface.u32Height = info->var.yres;

        pstPar->stRunInfo.bModifying           = HI_TRUE;
        pstPar->st3DInfo.st3DSurface.u32Pitch  = u32Stride;
        pstPar->stRunInfo.u32ParamModifyMask  |= HIFB_LAYER_PARAMODIFY_STRIDE;
        pstPar->stRunInfo.bModifying           = HI_FALSE;
        info->fix.line_length = u32Stride;
    }
    else
#endif
    {
        u32PixDepth = info->var.bits_per_pixel >> 3;
        u32Stride = CONIFG_HIFB_GetMaxStride(info->var.xres_virtual,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
        if ((u32Stride != info->fix.line_length) ||(info->var.yres != pstPar->stExtendInfo.u32DisplayHeight))
        {
            pstPar->stRunInfo.bModifying     = HI_TRUE;
            info->fix.line_length            = u32Stride;
            hifb_assign_dispbuf(pstPar->stBaseInfo.u32LayerID);
            pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
            pstPar->stRunInfo.bModifying      = HI_FALSE;
        }
    }

    if ((pstPar->stExtendInfo.enColFmt != enFmt))
    {
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
        if (s_stTextLayer[pstPar->stBaseInfo.u32LayerID].bAvailable)
        {
            HI_U32 i = 0;
            for (i = 0; i < SCROLLTEXT_NUM; i++)
            {
                if (s_stTextLayer[pstPar->stBaseInfo.u32LayerID].stScrollText[i].bAvailable)
                {
                    hifb_freescrolltext_cachebuf(&(s_stTextLayer[pstPar->stBaseInfo.u32LayerID].stScrollText[i]));
                    HI_GFX_Memset(&s_stTextLayer[pstPar->stBaseInfo.u32LayerID].stScrollText[i],0,sizeof(HIFB_SCROLLTEXT_S));
                }
            }
            s_stTextLayer[pstPar->stBaseInfo.u32LayerID].bAvailable      = HI_FALSE;
            s_stTextLayer[pstPar->stBaseInfo.u32LayerID].u32textnum      = 0;
            s_stTextLayer[pstPar->stBaseInfo.u32LayerID].u32ScrollTextId = 0;
        }
#endif
        pstPar->stRunInfo.bModifying   = HI_TRUE;
        pstPar->stExtendInfo.enColFmt = enFmt;
        pstPar->stRunInfo.u32ParamModifyMask  |= HIFB_LAYER_PARAMODIFY_FMT;
        pstPar->stRunInfo.bModifying = HI_FALSE;

    }

    HIFB_OSR_SetDispInfo(pstPar->stBaseInfo.u32LayerID);

    pstPar->bPanFlag = HI_TRUE;

    return HI_SUCCESS;
}


static inline HI_S32 HIFB_OSR_CheckWhetherResolutionSupport(HI_U32 LayerId, HI_U32 Width, HI_U32 Height)
{
    switch(LayerId)
    {
       case HIFB_LAYER_HD_0:
       case HIFB_LAYER_HD_1:
       case HIFB_LAYER_HD_2:
       case HIFB_LAYER_HD_3:
           if (Width > CONFIG_HIFB_HD_LAYER_MAXWIDTH || Height > CONFIG_HIFB_HD_LAYER_MAXHEIGHT)
           {
               HIFB_INFO("the hd input ui size is not support\n ");
               return HI_FAILURE;
           }
           break;
       case HIFB_LAYER_SD_0:
       case HIFB_LAYER_SD_1:
       case HIFB_LAYER_SD_2:
       case HIFB_LAYER_SD_3:
           if (Width > CONFIG_HIFB_SD_LAYER_MAXWIDTH || Height > CONFIG_HIFB_SD_LAYER_MAXHEIGHT)
           {
               HIFB_INFO("the sd input ui size is not support\n ");
               return HI_FAILURE;
           }
           break;
       default:
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static inline HI_VOID HIFB_OSR_SetDispInfo(HI_U32 LayerId)
{
    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    if ((info->var.xres == pstPar->stExtendInfo.u32DisplayWidth) && (info->var.yres == pstPar->stExtendInfo.u32DisplayHeight))
    {
        return;
    }

    if ((0 == info->var.xres) || (0 == info->var.yres))
    {
        if (HI_TRUE == pstPar->stExtendInfo.bShow)
        {
            pstPar->stRunInfo.bModifying          = HI_TRUE;
            pstPar->stExtendInfo.bShow            = HI_FALSE;
            pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
            pstPar->stRunInfo.bModifying          = HI_FALSE;
        }
    }
    hifb_disp_setdispsize  (LayerId, info->var.xres, info->var.yres);
    hifb_assign_dispbuf    (LayerId);

   return;
}

static HI_VOID hifb_disp_setlayerpos(HI_U32 u32LayerId, HI_S32 s32XPos, HI_S32 s32YPos)
{
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

    pstPar->stRunInfo.bModifying          = HI_TRUE;

    pstPar->stExtendInfo.stPos.s32XPos = s32XPos;
    pstPar->stExtendInfo.stPos.s32YPos = s32YPos;

    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;
    pstPar->stRunInfo.bModifying          = HI_FALSE;
    return;
}

static HI_VOID hifb_buf_setbufmode(HI_U32 u32LayerId, HIFB_LAYER_BUF_E enLayerBufMode)
{
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

    /* in 0 buf mode ,maybe the stride or fmt will be changed! */
    if ((pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE)
        && (pstPar->stExtendInfo.enBufMode != enLayerBufMode))
    {
        pstPar->stRunInfo.bModifying = HI_TRUE;

        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;

        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;

        pstPar->stRunInfo.bModifying = HI_FALSE;
    }

    pstPar->stExtendInfo.enBufMode = enLayerBufMode;

}


/***************************************************************************************
* func          : hifb_disp_setdispsize
* description   : CNcomment: choose the module to do  flicker resiting,
                             TDE or VOU ? the rule is as this ,the moudle
                             should do flicker resisting who has do scaling CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID hifb_select_antiflicker_mode(HIFB_PAR_S *pstPar)
{
    HIFB_RECT stOutputRect;
    /**
     ** if the usr's configuration is no needed to do flicker resisting,
     ** so no needed to do it
     **/
   if (pstPar->stBaseInfo.enAntiflickerLevel == HIFB_LAYER_ANTIFLICKER_NONE)
   {
       pstPar->stBaseInfo.enAntiflickerMode = HIFB_ANTIFLICKER_NONE;
   }
   else
   {
       /**
        ** current standard no needed to do flicker resisting
        **/
       if (!pstPar->stBaseInfo.bNeedAntiflicker)
       {
           pstPar->stBaseInfo.enAntiflickerMode = HIFB_ANTIFLICKER_NONE;
       }
       else
       {
               g_stDrvAdpCallBackFunction.HIFB_DRV_GetLayerOutRect(pstPar->stBaseInfo.u32LayerID, &stOutputRect);
           /**
            ** VO has don scaling , so should do flicker resisting at the same time
            **/
           if ( (pstPar->stExtendInfo.u32DisplayWidth  != stOutputRect.w)
             || (pstPar->stExtendInfo.u32DisplayHeight != stOutputRect.h))
           {
               pstPar->stBaseInfo.enAntiflickerMode = HIFB_ANTIFLICKER_VO;
           }
           else
           {
               pstPar->stBaseInfo.enAntiflickerMode = HIFB_ANTIFLICKER_TDE;
           }
       }
   }

}


/***************************************************************************************
* func          : hifb_disp_setantiflickerlevel
* description   : CNcomment: 设置抗闪级别 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_VOID hifb_disp_setantiflickerlevel(HI_U32 u32LayerId, HIFB_LAYER_ANTIFLICKER_LEVEL_E enAntiflickerLevel)
{
    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)info->par;

    pstPar->stBaseInfo.enAntiflickerLevel = enAntiflickerLevel;
    hifb_select_antiflicker_mode(pstPar);

    return;
}

#define HIFB_CHECK_LAYERID(u32LayerId) do\
{\
    if (!gs_pstCapacity[u32LayerId].bLayerSupported)\
    {\
        HIFB_ERROR("not support layer %d\n", u32LayerId);\
        return HI_FAILURE;\
    }\
}while(0);

#ifdef CFG_HIFB_CURSOR_SUPPORT
#define HIFB_CHECK_CURSOR_LAYERID(u32LayerId) do\
{\
 if (u32LayerId != HIFB_LAYER_CURSOR)\
    {\
        HIFB_ERROR("layer %d is not cursor layer!\n", u32LayerId);\
        return HI_FAILURE;\
    }\
}while(0)
#endif



static HI_S32 DRV_HIFB_SetDispAddressr(HI_U32 u32LayerId)
{

    HI_U32 u32Index;
    struct fb_info *info;
    HIFB_PAR_S *pstPar;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)(info->par);

    u32Index = pstPar->stRunInfo.u32IndexForInt;

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (pstPar->st3DInfo.IsStereo)
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(u32LayerId, pstPar->st3DInfo.u32DisplayAddr[u32Index]);
        pstPar->stRunInfo.u32ScreenAddr  = pstPar->st3DInfo.u32DisplayAddr[u32Index];
        pstPar->st3DInfo.u32rightEyeAddr = pstPar->stRunInfo.u32ScreenAddr;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimAddr(u32LayerId, pstPar->st3DInfo.u32rightEyeAddr);
    }
    else
#endif
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(u32LayerId, pstPar->stDispInfo.u32DisplayAddr[u32Index]);
        pstPar->stRunInfo.u32ScreenAddr  = pstPar->stDispInfo.u32DisplayAddr[u32Index];
    }

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    if (g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(u32LayerId))
    {
        HI_GFX_Memcpy(&(pstPar->stDispInfo.stCmpRect), &pstPar->stDispInfo.stUpdateRect, sizeof(HIFB_RECT));
    }
#endif

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (HI_TRUE == pstPar->st3DInfo.IsStereo)
    {
        pstPar->stRunInfo.u32IndexForInt = (++u32Index) % pstPar->stRunInfo.StereoBufNum;
    }
    else
#endif
    {
        pstPar->stRunInfo.u32IndexForInt = (++u32Index) % pstPar->stRunInfo.u32BufNum;
    }

    pstPar->stRunInfo.bFliped   = HI_TRUE;
    pstPar->stRunInfo.bNeedFlip = HI_FALSE;

    return HI_SUCCESS;

}


/***************************************************************************************
* func          : hifb_vo_callback
* description   : CNcomment: vo中断处理 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_vo_callback(HI_VOID *pParaml, HI_VOID *pParamr)
{

    HI_U32 *pu32LayerId  = NULL;
    struct fb_info *info = NULL;
    HIFB_PAR_S *pstPar   = NULL;
    struct timeval tv;
    HI_U32 u32NowTimeMs;
    HIFB_RECT stInRect   = {0};
#ifdef CFG_HIFB_PROC_SUPPORT
    DRV_HIFB_DECMPSTATUS_S stDeCmpStatus;
#endif
    if (NULL == pParaml)
    {
       return HI_FAILURE;
    }

    pu32LayerId  = (HI_U32*)pParaml;
    HIFB_CHECK_LAYERID_SUPPORT(*pu32LayerId);

    info = s_stLayer[*pu32LayerId].pstInfo;
    pstPar   = (HIFB_PAR_S *)(info->par);

    do_gettimeofday(&tv);
    u32NowTimeMs = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    if ((u32NowTimeMs - pstPar->stFrameInfo.u32StartTimeMs) >= 1000)
    {
        pstPar->stFrameInfo.u32StartTimeMs = u32NowTimeMs;
        pstPar->stFrameInfo.u32Fps =  pstPar->stFrameInfo.u32RefreshFrame;
        pstPar->stFrameInfo.u32RefreshFrame = 0;
    }
    pstPar->stFrameInfo.bFrameHit = HI_FALSE;

#ifdef CFG_HIFB_PROC_SUPPORT
    HI_GFX_Memset(&stDeCmpStatus, 0x0, sizeof(DRV_HIFB_DECMPSTATUS_S));
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetDeCmpStatus(*pu32LayerId, &stDeCmpStatus,pstPar->stProcInfo.bDebugByMoreMessage);

    if (HI_TRUE == pstPar->stProcInfo.bDebugByMoreMessage)
    {
       pstPar->stProcInfo.bHasDeCmpErr = (stDeCmpStatus.bDeCmpARDataErr | stDeCmpStatus.bDeCmpGBDataErr);
    }
    pstPar->stProcInfo.DeCmpARDataErrCnt += (HI_U32)stDeCmpStatus.bDeCmpARDataErr;
    pstPar->stProcInfo.DeCmpGBDataErrCnt += (HI_U32)stDeCmpStatus.bDeCmpGBDataErr;
    pstPar->stProcInfo.DeCmpARDataStatus[0] = stDeCmpStatus.DeCmpARDataStatus[0];
    pstPar->stProcInfo.DeCmpARDataStatus[1] = stDeCmpStatus.DeCmpARDataStatus[1];
    pstPar->stProcInfo.DeCmpGBDataStatus[0] = stDeCmpStatus.DeCmpGBDataStatus[0];
    pstPar->stProcInfo.DeCmpGBDataStatus[1] = stDeCmpStatus.DeCmpGBDataStatus[1];

    if (HI_TRUE == pstPar->stProcInfo.bHasDeCmpErr)
    {
       g_stDrvAdpCallBackFunction.HIFB_DRV_SetVdpIntMask(0x0);
    }
#endif

    if (!pstPar->stRunInfo.bModifying)
    {

        if (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_SHOW)
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_EnableLayer(*pu32LayerId, pstPar->stExtendInfo.bShow);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_SHOW;
        }
        if (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_ALPHA)
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAlpha(*pu32LayerId, &pstPar->stExtendInfo.stAlpha);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_ALPHA;
        }
        if (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_COLORKEY)
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerKeyMask(*pu32LayerId, &pstPar->stExtendInfo.stCkey);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_COLORKEY;
        }
        if (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_BMUL)
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerPreMult(*pu32LayerId, pstPar->stBaseInfo.bPreMul);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_BMUL;
        }
        if (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL)
        {
            HIFB_DEFLICKER_S stDeflicker;
            stDeflicker.pu8HDfCoef  = pstPar->stBaseInfo.ucHDfcoef;
            stDeflicker.pu8VDfCoef  = pstPar->stBaseInfo.ucVDfcoef;
            stDeflicker.u32HDfLevel = pstPar->stBaseInfo.u32HDflevel;
            stDeflicker.u32VDfLevel = pstPar->stBaseInfo.u32VDflevel;

            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerDeFlicker(*pu32LayerId, &stDeflicker);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_ANTIFLICKERLEVEL;
        }
        if (   pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_INRECT
            || pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_OUTRECT)
        {
            stInRect.x = pstPar->stExtendInfo.stPos.s32XPos;
            stInRect.y = pstPar->stExtendInfo.stPos.s32YPos;

            stInRect.w = (HI_S32)pstPar->stExtendInfo.u32DisplayWidth;
            stInRect.h = (HI_S32)pstPar->stExtendInfo.u32DisplayHeight;

            g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerInRect(*pu32LayerId, &stInRect);
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_INRECT;
            pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_OUTRECT;
        }

        HIFB_OSR_UpdataInputDataFmt(*pu32LayerId);

        HIFB_OSR_UpdataInputDataStride(*pu32LayerId);

        HIFB_OSR_UpdataRefreshInfo(*pu32LayerId);

    }

    HIFB_OSR_SwitchDispBuffer(*pu32LayerId);

    g_stDrvAdpCallBackFunction.HIFB_DRV_UpdataLayerReg(*pu32LayerId);


#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    hifb_scrolltext_blit(*pu32LayerId);
#endif

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HIFB_OSR_UpdataCmp(*pu32LayerId);
#endif

    return HI_SUCCESS;
}


static inline HI_VOID HIFB_OSR_UpdataRefreshInfo(HI_U32 LayerId)
{

    HI_BOOL bResume = HI_FALSE;
    HI_U32 IsInputDataHasRfresh = 0x0;
    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    IsInputDataHasRfresh = (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_REFRESH);

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetResumeStatus(LayerId,&bResume);
    if (HI_TRUE == bResume)
    {
        IsInputDataHasRfresh = 0x1;
    }

    if (!IsInputDataHasRfresh)
    {
       return;
    }

    HIFB_OSR_UpdataInputDataDDR(LayerId);

#if defined(CFG_HIFB_FENCE_SUPPORT) || defined(CFG_HI_FB_DECOMPRESS_SUPPORT)
    DRV_HIFB_SetDecmpLayerInfo(LayerId);
#endif

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HIFB_OSR_UpdataInputDataCmpRect(LayerId);
#endif

    pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_REFRESH;

    return;
}


#ifdef CFG_HIFB_COMPRESSION_SUPPORT
static inline HI_VOID HIFB_OSR_UpdataCmp(HI_U32 LayerId)
{
    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    if (g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(LayerId))
    {
        if (pstPar->stDispInfo.stCmpRect.w && pstPar->stDispInfo.stCmpRect.h)
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetCmpRect(LayerId, &pstPar->stDispInfo.stCmpRect);
            HI_GFX_Memset(&pstPar->stDispInfo.stCmpRect, 0, sizeof(HIFB_RECT));
        }

        if ((pstPar->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY) || (pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE))
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetCmpDDROpen(LayerId, HI_TRUE);
        }
        else
        {
            g_stDrvAdpCallBackFunction.HIFB_DRV_SetCmpDDROpen(LayerId, HI_FALSE);
        }
    }
    return;
}


static inline HI_VOID HIFB_OSR_UpdataInputDataCmpRect(HI_U32 LayerId)
{
     struct fb_info *info = s_stLayer[LayerId].pstInfo;
     HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

     if (g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(LayerId))
     {
         if (  (pstPar->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY)
             ||(pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE))
         {
             pstPar->stDispInfo.stCmpRect.x = 0;
             pstPar->stDispInfo.stCmpRect.y = 0;
             pstPar->stDispInfo.stCmpRect.w = pstPar->stExtendInfo.u32DisplayWidth;
             pstPar->stDispInfo.stCmpRect.h = pstPar->stExtendInfo.u32DisplayHeight;
         }
     }

     return;
}
#endif

static inline HI_VOID HIFB_OSR_UpdataInputDataFmt(HI_U32 LayerId)
{

    HI_U32 IsInputDataFmtHasChanged = 0x0;
    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    IsInputDataFmtHasChanged = (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_FMT);
    if (!IsInputDataFmtHasChanged)
    {
         return;
    }

     if (  (pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE) && pstPar->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr)
     {
         g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerDataFmt(LayerId, pstPar->stDispInfo.stUserBuffer.stCanvas.enFmt);
     }
     else
     {
         g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerDataFmt(LayerId, pstPar->stExtendInfo.enColFmt);
     }
     pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FMT;

    return;
}

static inline HI_VOID HIFB_OSR_UpdataInputDataStride(HI_U32 LayerId)
{
     HI_BOOL NeedScaleInputStride = HI_FALSE;
     HI_U32 InputDataStride = 0;
     HI_U32 IsInputDataStrideHasChanged = 0x0;
     struct fb_info *info = s_stLayer[LayerId].pstInfo;
     HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

     IsInputDataStrideHasChanged = (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_STRIDE);
     if (!IsInputDataStrideHasChanged)
     {
         return;
     }

 #ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
     if (pstPar->st3DInfo.IsStereo)
     {
         /**<-- when stereo callback, no refresh again. but now, data buffer is tbb mode, when read data, the stride should scale 2 times **/
         NeedScaleInputStride = (HI_TRUE == pstPar->st3DInfo.IsStereoCallBack)\
                             && (HIFB_STEREO_TOPANDBOTTOM == pstPar->st3DInfo.DataStereoMode)\
                             && (HIFB_STEREO_SIDEBYSIDE_HALF == pstPar->st3DInfo.StereoMode || HIFB_STEREO_FRMPACKING == pstPar->st3DInfo.StereoMode);

         pstPar->st3DInfo.IsStereoCallBack = HI_FALSE;

         if (HI_TRUE == NeedScaleInputStride)
         {
            pstPar->st3DInfo.DataStereoMode = HIFB_STEREO_MONO;
            InputDataStride = pstPar->st3DInfo.st3DSurface.u32Pitch >> 1;
         }
         else
         {
            InputDataStride = pstPar->st3DInfo.st3DSurface.u32Pitch;
         }
     }
     else
 #endif
    {
         if ((pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE) && (pstPar->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr))
         {
             InputDataStride = pstPar->stDispInfo.stUserBuffer.stCanvas.u32Pitch;
         }
         else
         {
             InputDataStride = info->fix.line_length;
         }
    }

    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerStride(LayerId, InputDataStride);

    pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_STRIDE;

    return;
}


static inline HI_VOID HIFB_OSR_UpdataInputDataDDR(HI_U32 LayerId)
{
    HI_U32 IsInputDataDDRHasChanged = 0x0;
    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    IsInputDataDDRHasChanged = (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_DISPLAYADDR);
    if (!IsInputDataDDRHasChanged)
    {
         return;
    }

    pstPar->stFrameInfo.u32RefreshFrame++;
    pstPar->stFrameInfo.bFrameHit = HI_TRUE;

    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(LayerId, pstPar->stRunInfo.u32ScreenAddr);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (pstPar->st3DInfo.IsStereo)
    {
        if ((pstPar->st3DInfo.StereoMode == HIFB_STEREO_FRMPACKING)
            || (0 != pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart)
            || (HIFB_REFRESH_MODE_WITH_PANDISPLAY != pstPar->stExtendInfo.enBufMode))
        {
            pstPar->st3DInfo.u32rightEyeAddr = pstPar->stRunInfo.u32ScreenAddr;
        }
        else if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_SIDEBYSIDE_HALF)
        {
            pstPar->st3DInfo.u32rightEyeAddr = pstPar->stRunInfo.u32ScreenAddr + pstPar->st3DInfo.st3DSurface.u32Pitch / 2;
        }
        else if (pstPar->st3DInfo.StereoMode == HIFB_STEREO_TOPANDBOTTOM)
        {
            pstPar->st3DInfo.u32rightEyeAddr = pstPar->stRunInfo.u32ScreenAddr + pstPar->st3DInfo.st3DSurface.u32Pitch * pstPar->stExtendInfo.u32DisplayHeight / 2;
        }
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimAddr(LayerId, pstPar->st3DInfo.u32rightEyeAddr);
    }
#endif

    pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_DISPLAYADDR;

    return;
}


static inline HI_VOID HIFB_OSR_SwitchDispBuffer(HI_U32 LayerId)
{

    struct fb_info *info = s_stLayer[LayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);

    if ((pstPar->stExtendInfo.enBufMode == HIFB_LAYER_BUF_DOUBLE) && (pstPar->stRunInfo.bNeedFlip == HI_TRUE))
    {
        DRV_HIFB_SetDispAddressr(LayerId);
    }

    if (HI_TRUE == pstPar->stDispTmpBuf.bStartRelease)
    {
        schedule_work(&(pstPar->stDispTmpBuf.bDisBufWork));
        pstPar->stDispTmpBuf.bStartRelease = HI_FALSE;
    }

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    else if ((pstPar->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY)
            && pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart
            && pstPar->st3DInfo.IsStereo
            && (pstPar->stRunInfo.bNeedFlip == HI_TRUE))
    {
        DRV_HIFB_SetDispAddressr(LayerId);
    }
#endif

    return;
}


/***************************************************************************************
* func          : hifb_refresh_0buf
* description   : CNcomment: no display buffer refresh CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_0buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{

    HI_U32 u32StartAddr;
    HIFB_PAR_S *pstPar;
    struct fb_info *info;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    /**
     **显示起始地址为canvas 地址
     **/
    u32StartAddr = pstCanvasBuf->stCanvas.u32PhyAddr;

    /**
     ** when you change para, the register not be change
     **/
    pstPar->stRunInfo.bModifying = HI_TRUE;
    pstPar->stRunInfo.u32ScreenAddr       = u32StartAddr;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;

    pstPar->stDispInfo.stUserBuffer.stCanvas.u32Pitch = pstCanvasBuf->stCanvas.u32Pitch;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;

    pstPar->stDispInfo.stUserBuffer.stCanvas.enFmt = pstCanvasBuf->stCanvas.enFmt;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;

    hifb_disp_setdispsize(u32LayerId, pstCanvasBuf->stCanvas.u32Width, pstCanvasBuf->stCanvas.u32Height);

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;

    pstPar->stRunInfo.bModifying = HI_FALSE;

    DRV_HIFB_WaitVBlank(u32LayerId);

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : hifb_refresh_1buf
* description   : CNcomment: one canvas buffer,one display buffer refresh CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_1buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HI_S32 s32Ret;
    HIFB_PAR_S *pstPar;
    HIFB_BUFFER_S stDisplayBuf;
    struct fb_info *info;
    HIFB_OSD_DATA_S stOsdData;
    HIFB_BLIT_OPT_S stBlitOpt;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    HI_GFX_Memset(&stBlitOpt,    0, sizeof(HIFB_BLIT_OPT_S));
    HI_GFX_Memset(&stDisplayBuf, 0, sizeof(HIFB_BUFFER_S));

    stDisplayBuf.stCanvas.enFmt      = pstPar->stExtendInfo.enColFmt;
    stDisplayBuf.stCanvas.u32Height  = pstPar->stExtendInfo.u32DisplayHeight;
    stDisplayBuf.stCanvas.u32Width   = pstPar->stExtendInfo.u32DisplayWidth;
    stDisplayBuf.stCanvas.u32Pitch   = info->fix.line_length;
    stDisplayBuf.stCanvas.u32PhyAddr = pstPar->stDispInfo.u32DisplayAddr[0];

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    /**
     ** if display address is not the same as inital address,
     ** please config it use old address
     **/
    if (   stOsdData.u32RegPhyAddr != pstPar->stDispInfo.u32DisplayAddr[0]
        && pstPar->stDispInfo.u32DisplayAddr[0])
    {
        pstPar->stRunInfo.bModifying = HI_TRUE;
        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
        pstPar->stRunInfo.u32ScreenAddr       = pstPar->stDispInfo.u32DisplayAddr[0];
        HI_GFX_Memset(info->screen_base, 0x00, info->fix.smem_len);
        pstPar->stRunInfo.bModifying = HI_FALSE;
    }

    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    if (   pstCanvasBuf->stCanvas.u32Height != pstPar->stExtendInfo.u32DisplayHeight
        || pstCanvasBuf->stCanvas.u32Width != pstPar->stExtendInfo.u32DisplayWidth)
    {
        stBlitOpt.bScale          = HI_TRUE;
        /** 只做为TDE内部参数检查使用**/
        stDisplayBuf.UpdateRect.x = 0;
        stDisplayBuf.UpdateRect.y = 0;
        stDisplayBuf.UpdateRect.w = stDisplayBuf.stCanvas.u32Width;
        stDisplayBuf.UpdateRect.h = stDisplayBuf.stCanvas.u32Height;
    }
    else
    {
        stDisplayBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    stBlitOpt.bRegionDeflicker = HI_TRUE;

    s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(pstCanvasBuf, &stDisplayBuf, &stBlitOpt, HI_TRUE);
    if (s32Ret <= 0){
        HIFB_ERROR("hifb_refresh_1buf blit err 5!\n");
        return HI_FAILURE;
    }

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HI_GFX_Memcpy(&(pstPar->stDispInfo.stCmpRect), &stDisplayBuf.UpdateRect, sizeof(HIFB_RECT));
#endif

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : hifb_refresh_2buf
* description   : CNcomment: 异步刷新 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_2buf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Index;
    HIFB_PAR_S *pstPar;
    struct fb_info *info;
    unsigned long lockflag;
    HIFB_BUFFER_S stForeBuf;
    HIFB_BUFFER_S stBackBuf;
    HIFB_BLIT_OPT_S stBlitOpt;
    HIFB_OSD_DATA_S stOsdData;

    info     = s_stLayer[u32LayerId].pstInfo;
    pstPar   = (HIFB_PAR_S *)info->par;
    u32Index = pstPar->stRunInfo.u32IndexForInt;

    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    HI_GFX_Memset(&stForeBuf, 0, sizeof(HIFB_BUFFER_S));
    HI_GFX_Memset(&stBackBuf, 0, sizeof(HIFB_BUFFER_S));

    stBlitOpt.bCallBack = HI_TRUE;
    stBlitOpt.pfnCallBack = (IntCallBack)DRV_HIFB_CallBack;
    stBlitOpt.pParam = &(pstPar->stBaseInfo.u32LayerID);

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&lockflag);
    pstPar->stRunInfo.bNeedFlip = HI_FALSE;
    pstPar->stRunInfo.s32RefreshHandle = 0;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&lockflag);

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    stBackBuf.stCanvas.enFmt      = pstPar->stExtendInfo.enColFmt;
    stBackBuf.stCanvas.u32Width   = pstPar->stExtendInfo.u32DisplayWidth;
    stBackBuf.stCanvas.u32Height  = pstPar->stExtendInfo.u32DisplayHeight;
    stBackBuf.stCanvas.u32Pitch   = info->fix.line_length;
    stBackBuf.stCanvas.u32PhyAddr = pstPar->stDispInfo.u32DisplayAddr[u32Index];

    /**
     ** according to the hw arithemetic, calculate source and Dst fresh rectangle
     **/
    if (  (pstCanvasBuf->stCanvas.u32Height != pstPar->stExtendInfo.u32DisplayHeight)
        ||(pstCanvasBuf->stCanvas.u32Width  != pstPar->stExtendInfo.u32DisplayWidth))
    {

        stBlitOpt.bScale = HI_TRUE;
    }

    hifb_getupdate_rect(u32LayerId, pstCanvasBuf, &stBackBuf.UpdateRect);


    /**
     ** We should check is address changed, for make sure that the address
     ** configed to the hw reigster is in effect
     **/
    if (   (pstPar->stRunInfo.bFliped)
        && (stOsdData.u32RegPhyAddr == pstPar->stDispInfo.u32DisplayAddr[1-u32Index]))
    {
        /**
         ** when fill background buffer, we need to backup fore buffer first
         **/
        hifb_backup_forebuf(u32LayerId, &stBackBuf);
        /**
         ** clear union rect
         **/
        HI_GFX_Memset(&(pstPar->stDispInfo.stUpdateRect), 0, sizeof(HIFB_RECT));
        pstPar->stRunInfo.bFliped = HI_FALSE;
    }

    /* update union rect */
    if ((pstPar->stDispInfo.stUpdateRect.w == 0) || (pstPar->stDispInfo.stUpdateRect.h == 0))
    {
        HI_GFX_Memcpy(&pstPar->stDispInfo.stUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));
    }
    else
    {
        HIFB_UNITE_RECT(pstPar->stDispInfo.stUpdateRect, stBackBuf.UpdateRect);
    }

    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    if (stBlitOpt.bScale == HI_TRUE)
    {
        /*actual area, calculate by TDE, here is just use for let pass the test */
        stBackBuf.UpdateRect.x = 0;
        stBackBuf.UpdateRect.y = 0;
        stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
        stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
    }
    else
    {
        stBackBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    stBlitOpt.bRegionDeflicker = HI_TRUE;
    /**
     ** blit with refresh rect
     **/
    s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(pstCanvasBuf, &stBackBuf,&stBlitOpt, HI_TRUE);
    if (s32Ret <= 0){
        HIFB_ERROR("2buf blit err7!\n");
        goto RET;
    }

    pstPar->stRunInfo.s32RefreshHandle = s32Ret;

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

RET:
    return HI_SUCCESS;

}
/***************************************************************************************
* func          : hifb_refresh_2buf_immediate_display
* description   : CNcomment: 同步刷新 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_2buf_immediate_display(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{

    HI_S32 s32Ret    = 0;
    HI_U32 u32Index  = 0;
    HIFB_PAR_S *pstPar = NULL;
    struct fb_info *info;
    unsigned long lockflag = 0;
    HIFB_BUFFER_S stForeBuf;
    HIFB_BUFFER_S stBackBuf;
    HIFB_BLIT_OPT_S stBlitOpt;
    HIFB_OSD_DATA_S stOsdData;

    info     = s_stLayer[u32LayerId].pstInfo;
    pstPar     = (HIFB_PAR_S *)info->par;
    u32Index = pstPar->stRunInfo.u32IndexForInt;

    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    HI_GFX_Memset(&stForeBuf, 0, sizeof(HIFB_BUFFER_S));
    HI_GFX_Memset(&stBackBuf, 0, sizeof(HIFB_BUFFER_S));

    stBlitOpt.bCallBack = HI_FALSE;
    stBlitOpt.pParam = &(pstPar->stBaseInfo.u32LayerID);

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&lockflag);
    pstPar->stRunInfo.bNeedFlip = HI_FALSE;
    pstPar->stRunInfo.s32RefreshHandle = 0;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&lockflag);

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    stBackBuf.stCanvas.enFmt      = pstPar->stExtendInfo.enColFmt;
    stBackBuf.stCanvas.u32Width   = pstPar->stExtendInfo.u32DisplayWidth;
    stBackBuf.stCanvas.u32Height  = pstPar->stExtendInfo.u32DisplayHeight;
    stBackBuf.stCanvas.u32Pitch   = info->fix.line_length;
    stBackBuf.stCanvas.u32PhyAddr = pstPar->stDispInfo.u32DisplayAddr[u32Index];

    /* according to the hw arithemetic, calculate  source and Dst fresh rectangle */
    if (   (pstCanvasBuf->stCanvas.u32Height != pstPar->stExtendInfo.u32DisplayHeight)
        || (pstCanvasBuf->stCanvas.u32Width  != pstPar->stExtendInfo.u32DisplayWidth))
    {
        stBlitOpt.bScale = HI_TRUE;
    }

    hifb_getupdate_rect(u32LayerId, pstCanvasBuf, &stBackBuf.UpdateRect);

    /**
     ** when fill background buffer, we need to backup fore buffer first
     **/
    hifb_backup_forebuf(u32LayerId, &stBackBuf);


    /**
     ** update union rect
     **/
    HI_GFX_Memcpy(&pstPar->stDispInfo.stUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));

    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    if (stBlitOpt.bScale == HI_TRUE)
    {
        /**
         ** actual area, calculate by TDE, here is just use for let pass the test
         **/
        stBackBuf.UpdateRect.x = 0;
        stBackBuf.UpdateRect.y = 0;
        stBackBuf.UpdateRect.w = stBackBuf.stCanvas.u32Width;
        stBackBuf.UpdateRect.h = stBackBuf.stCanvas.u32Height;
    }
    else
    {
        stBackBuf.UpdateRect = pstCanvasBuf->UpdateRect;
    }

    stBlitOpt.bRegionDeflicker = HI_TRUE;
    stBlitOpt.bBlock           = HI_TRUE;
    /**
     ** blit with refresh rect
     **/
    s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(pstCanvasBuf, &stBackBuf,&stBlitOpt, HI_TRUE);
    if (s32Ret <= 0){
        HIFB_ERROR("2buf blit err 0x%x!\n",s32Ret);
        goto RET;
    }

    /**
     **set the backup buffer to register and show it
     **/
    pstPar->stRunInfo.bModifying = HI_TRUE;
    pstPar->stRunInfo.u32ScreenAddr       = pstPar->stDispInfo.u32DisplayAddr[u32Index];
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    pstPar->stRunInfo.bModifying = HI_FALSE;

    pstPar->stRunInfo.u32IndexForInt = 1 - u32Index;

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    if (g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(u32LayerId))
    {
        HI_GFX_Memcpy(&(pstPar->stDispInfo.stCmpRect), &pstPar->stDispInfo.stUpdateRect, sizeof(HIFB_RECT));
    }
#endif

    /**
     ** wait the address register's configuration take effect before return
     **/
    DRV_HIFB_WaitVBlank(u32LayerId);

RET:
    return HI_SUCCESS;

}


/***************************************************************************************
* func          : hifb_refresh_panbuf
* description   : CNcomment: 给android使用 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
static HI_S32 hifb_refresh_panbuf(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HI_S32 s32Ret;
    HI_U32 u32TmpAddr;
    HIFB_RECT UpdateRect;
    HIFB_BLIT_OPT_S stBlitOpt;
    HIFB_BUFFER_S stCanvasBuf;
    HIFB_BUFFER_S stDisplayBuf;

    HIFB_PAR_S *par;
    struct fb_info *info;
    struct fb_var_screeninfo *var;

    info = s_stLayer[u32LayerId].pstInfo;
    par = (HIFB_PAR_S *)info->par;
    var = &(info->var);

    UpdateRect = pstCanvasBuf->UpdateRect;

    if ((UpdateRect.x >=  par->stExtendInfo.u32DisplayWidth)
        || (UpdateRect.y >= par->stExtendInfo.u32DisplayHeight)
        || (UpdateRect.w == 0) || (UpdateRect.h == 0))
    {
        HIFB_ERROR("hifb_refresh_panbuf upate rect invalid\n");
        return HI_FAILURE;
    }

    if (par->st3DInfo.IsStereo)
    {
        if (HI_NULL == par->st3DInfo.st3DMemInfo.u32StereoMemStart)
        {
            HIFB_ERROR("you should pan first\n");
            return HI_FAILURE;
        }

        HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
        stBlitOpt.bScale = HI_TRUE;

        if (HIFB_ANTIFLICKER_TDE == par->stBaseInfo.enAntiflickerMode)
        {
            stBlitOpt.enAntiflickerLevel = par->stBaseInfo.enAntiflickerLevel;
        }

        stBlitOpt.bBlock = HI_TRUE;
        stBlitOpt.bRegionDeflicker = HI_TRUE;

        if (var->bits_per_pixel >= 8)
        {
            u32TmpAddr = info->fix.smem_start + info->fix.line_length * var->yoffset
                           + var->xoffset* (var->bits_per_pixel >> 3);
        }
        else
        {
            u32TmpAddr = (info->fix.smem_start + info->fix.line_length * var->yoffset
                           + var->xoffset * var->bits_per_pixel / 8);
        }

        if((var->bits_per_pixel == 24)&&((var->xoffset !=0)||(var->yoffset !=0)))
        {
            HI_U32 TmpData;

            TmpData = (info->fix.smem_start + info->fix.line_length * var->yoffset
                           + var->xoffset * (var->bits_per_pixel >> 3))/16/3;
            u32TmpAddr = TmpData*16*3;
        }

        /********************config pan buffer*******************/
        HI_GFX_Memset(&stCanvasBuf, 0, sizeof(HIFB_BUFFER_S));
        stCanvasBuf.stCanvas.enFmt      = par->stExtendInfo.enColFmt;
        stCanvasBuf.stCanvas.u32Pitch   = info->fix.line_length;
        stCanvasBuf.stCanvas.u32PhyAddr = u32TmpAddr;
        stCanvasBuf.stCanvas.u32Width   = par->stExtendInfo.u32DisplayWidth;
        stCanvasBuf.stCanvas.u32Height  = par->stExtendInfo.u32DisplayHeight;
        stCanvasBuf.UpdateRect          = UpdateRect;
        /***********************end**************************/

        /*******************config 3D buffer********************/
        HI_GFX_Memset(&stDisplayBuf, 0, sizeof(HIFB_BUFFER_S));
        stDisplayBuf.stCanvas.enFmt      = par->st3DInfo.st3DSurface.enFmt;
        stDisplayBuf.stCanvas.u32Pitch   = par->st3DInfo.st3DSurface.u32Pitch;
        stDisplayBuf.stCanvas.u32PhyAddr = par->stRunInfo.u32ScreenAddr;
        stDisplayBuf.stCanvas.u32Width   = par->st3DInfo.st3DSurface.u32Width;
        stDisplayBuf.stCanvas.u32Height  = par->st3DInfo.st3DSurface.u32Height;
        /***********************end**************************/

        if (HIFB_STEREO_SIDEBYSIDE_HALF == par->st3DInfo.StereoMode)
        {
            stDisplayBuf.stCanvas.u32Width >>= 1;
        }
        else if (HIFB_STEREO_TOPANDBOTTOM == par->st3DInfo.StereoMode)
        {
            stDisplayBuf.stCanvas.u32Height >>= 1;
        }

        stDisplayBuf.UpdateRect.x = 0;
        stDisplayBuf.UpdateRect.y = 0;
        stDisplayBuf.UpdateRect.w = stDisplayBuf.stCanvas.u32Width;
        stDisplayBuf.UpdateRect.h = stDisplayBuf.stCanvas.u32Height;


        s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(&stCanvasBuf, &stDisplayBuf, &stBlitOpt, HI_TRUE);
        if (s32Ret <= 0){
            HIFB_ERROR("stereo blit error!\n");
            return HI_FAILURE;
        }

    }

    return HI_SUCCESS;
}

/***************************************************************************************
* func          : hifb_refresh_0buf_3D
* description   : CNcomment: 单buffer刷新 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_0buf_3D(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{

    HI_S32 s32Ret;
    HIFB_PAR_S *pstPar;
    HI_U32 u32BufferSize;
    struct fb_info *info;
    HIFB_BLIT_OPT_S stBlitOpt;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    /**
     ** config 3D surface par
     **/
    pstPar->st3DInfo.st3DSurface.enFmt     = pstCanvasBuf->stCanvas.enFmt;
    pstPar->st3DInfo.st3DSurface.u32Pitch  = pstCanvasBuf->stCanvas.u32Pitch;
    pstPar->st3DInfo.st3DSurface.u32Width  = pstCanvasBuf->stCanvas.u32Width;
    pstPar->st3DInfo.st3DSurface.u32Height = pstCanvasBuf->stCanvas .u32Height;


    u32BufferSize = HI_HIFB_GetMemSize(pstCanvasBuf->stCanvas.u32Pitch,pstCanvasBuf->stCanvas.u32Height);

    /**
     ** 分配3Dbuffer
     **/
    s32Ret = HIFB_OSR_ReAllocStereoBuf(u32LayerId, u32BufferSize);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return s32Ret;
    }

    /**
     ** config 3D surface par，使用display buffer0
     **/
    pstPar->st3DInfo.st3DSurface.u32PhyAddr= pstPar->st3DInfo.u32DisplayAddr[0];

    /**
     ** config 3D buffer
     **/
    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    stBlitOpt.bRegionDeflicker = HI_TRUE;
    stBlitOpt.bScale           = HI_TRUE;

    /**
     **要是逐行处理就需要抗闪，使得行与行之间的像素差别不会那么大，防止
     **那种条纹出现
     **/
    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    DRV_HIFB_SteroDataUpdate(u32LayerId, pstCanvasBuf, &stBlitOpt);

    /** 更新数据 **/
    pstPar->stRunInfo.bModifying = HI_TRUE;
    /** 更新显示地址 **/
    pstPar->stRunInfo.u32ScreenAddr       = pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    /** 更新stride **/
    pstPar->stDispInfo.stUserBuffer.stCanvas.u32Pitch = pstCanvasBuf->stCanvas.u32Pitch;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    /** 更新像素格式 **/
    pstPar->stDispInfo.stUserBuffer.stCanvas.enFmt = pstCanvasBuf->stCanvas.enFmt;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_FMT;

    hifb_disp_setdispsize(u32LayerId,
                            pstCanvasBuf->stCanvas.u32Width,
                            pstCanvasBuf->stCanvas.u32Height);

    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;

    pstPar->stRunInfo.bModifying = HI_FALSE;

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    DRV_HIFB_WaitVBlank(u32LayerId);

    return HI_SUCCESS;

}
/***************************************************************************************
* func          : hifb_refresh_1buf_3D
* description   : CNcomment: 双buffer刷新 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_1buf_3D(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HIFB_PAR_S *pstPar;
    struct fb_info *info;
    HIFB_BLIT_OPT_S stBlitOpt;
    HIFB_OSD_DATA_S stOsdData;
    HI_U32 u32Depth  = 0;
    HI_U32 u32Stride = 0;
    HI_U32 CmpStride   = 0;

    info   = s_stLayer[u32LayerId].pstInfo;
    pstPar = (HIFB_PAR_S *)info->par;

    /**
     **1.获取上一帧显示的OSD数据
     **/
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    /**2
     ** if display address is not the same as inital address,
     ** please config it use old address,如果显?镜牡刂凡皇浅跏蓟牡刂吩蚯谢幌允镜???
     **/
    if( (stOsdData.u32RegPhyAddr != pstPar->stDispInfo.u32DisplayAddr[0]) &&
        (pstPar->stDispInfo.u32DisplayAddr[0]))
    {
        pstPar->stRunInfo.bModifying = HI_TRUE;
        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
        pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
        pstPar->stRunInfo.u32ScreenAddr = pstPar->stDispInfo.u32DisplayAddr[0];
        HI_GFX_Memset(info->screen_base, 0x00, info->fix.smem_len);
        pstPar->stRunInfo.bModifying = HI_FALSE;
    }

    u32Depth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(pstPar->stExtendInfo.u32DisplayWidth,u32Depth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    pstPar->st3DInfo.st3DSurface.enFmt     = pstPar->stExtendInfo.enColFmt;
    pstPar->st3DInfo.st3DSurface.u32Pitch  = u32Stride;
    pstPar->st3DInfo.st3DSurface.u32Width  = pstPar->stExtendInfo.u32DisplayWidth;
    pstPar->st3DInfo.st3DSurface.u32Height = pstPar->stExtendInfo.u32DisplayHeight;
    pstPar->st3DInfo.st3DSurface.u32PhyAddr= pstPar->st3DInfo.u32DisplayAddr[0];

    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));

    /**
     **TDE内部做局部操作。会根据src srcrect dst自己计算dstrect,传入的dstrect无效
     **/
    stBlitOpt.bRegionDeflicker = HI_TRUE;
    stBlitOpt.bScale           = HI_TRUE;
    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    DRV_HIFB_SteroDataUpdate(u32LayerId, pstCanvasBuf, &stBlitOpt);

    /*backup usr buffer*/
    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;

}

/***************************************************************************************
* func          : hifb_refresh_2buf_3D
* description   : CNcomment: 3 buffer刷新 异步，刷新不等更新完，允许丢帧 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh_2buf_3D(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HI_U32 u32Index;
    HIFB_PAR_S *pstPar;
    struct fb_info *info;
    unsigned long lockflag;
    HIFB_BUFFER_S stBackBuf;
    HIFB_BLIT_OPT_S stBlitOpt;
    HIFB_OSD_DATA_S stOsdData;
    HI_U32 u32Depth  = 0;
    HI_U32 u32Stride = 0;
    HI_U32 CmpStride   = 0;

    info     = s_stLayer[u32LayerId].pstInfo;
    pstPar   = (HIFB_PAR_S *)info->par;
    u32Index = pstPar->stRunInfo.u32IndexForInt;

    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    HI_GFX_Memset(&stBackBuf, 0, sizeof(HIFB_BUFFER_S));

    /**
     **TDE操作完之后回调TDE注册的回调函数hifb_tde_callback
     **/
    stBlitOpt.bCallBack = HI_TRUE;
    stBlitOpt.pfnCallBack = (IntCallBack)DRV_HIFB_CallBack;
    stBlitOpt.pParam = &(pstPar->stBaseInfo.u32LayerID);

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&lockflag);
    pstPar->stRunInfo.bNeedFlip        = HI_FALSE;
    pstPar->stRunInfo.s32RefreshHandle = 0;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&lockflag);

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetOSDData(u32LayerId, &stOsdData);

    u32Depth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(pstPar->stExtendInfo.u32DisplayWidth,u32Depth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    pstPar->st3DInfo.st3DSurface.enFmt     = pstPar->stExtendInfo.enColFmt;
    pstPar->st3DInfo.st3DSurface.u32Pitch  = u32Stride;
    pstPar->st3DInfo.st3DSurface.u32Width  = pstPar->stExtendInfo.u32DisplayWidth;
    pstPar->st3DInfo.st3DSurface.u32Height = pstPar->stExtendInfo.u32DisplayHeight;
    pstPar->st3DInfo.st3DSurface.u32PhyAddr= pstPar->st3DInfo.u32DisplayAddr[u32Index];

    HI_GFX_Memcpy(&stBackBuf.stCanvas, &pstPar->st3DInfo.st3DSurface, sizeof(HIFB_SURFACE_S));

    /**
     ** according to the hw arithemetic, calculate  source and Dst fresh rectangle
     **/
    hifb_getupdate_rect(u32LayerId, pstCanvasBuf, &stBackBuf.UpdateRect);

    /**
     ** We should check is address changed, for make sure
     ** that the address configed to the hw reigster is in effect
     **/
    if(pstPar->stRunInfo.bFliped && (stOsdData.u32RegPhyAddr== pstPar->st3DInfo.u32DisplayAddr[1-u32Index]))
    {
        /**
         ** when fill background buffer, we need to backup fore buffer first
         **/
        hifb_backup_forebuf(u32LayerId, &stBackBuf);
        /** clear union rect **/
        HI_GFX_Memset(&(pstPar->st3DInfo.st3DUpdateRect), 0, sizeof(HIFB_RECT));
        pstPar->stRunInfo.bFliped = HI_FALSE;
    }

    /* update union rect */
    if ((pstPar->st3DInfo.st3DUpdateRect.w == 0) || (pstPar->st3DInfo.st3DUpdateRect.h == 0))
    {
        HI_GFX_Memcpy(&pstPar->st3DInfo.st3DUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));
    }
    else
    {
        HIFB_UNITE_RECT(pstPar->st3DInfo.st3DUpdateRect, stBackBuf.UpdateRect);
    }

    stBlitOpt.bScale = HI_TRUE;
    stBlitOpt.bRegionDeflicker = HI_TRUE;
    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    DRV_HIFB_SteroDataUpdate(u32LayerId, pstCanvasBuf, &stBlitOpt);

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    return HI_SUCCESS;

}


/**
 ** In this function we should wait the new contain has
 ** been show on the screen before return, and the operations
 ** such as address configuration no needed do in interrupt handle
 **/
/***************************************************************************************
* func            : hifb_refresh_2buf_immediate_display_3D
* description    : CNcomment: 3 buffer 同步，刷新等待更新完 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:        : NA
***************************************************************************************/
static HI_S32 hifb_refresh_2buf_immediate_display_3D(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf)
{
    HI_U32 u32Index = 0;
    HIFB_PAR_S *pstPar;
    struct fb_info *info;
    unsigned long lockflag;
    HIFB_BUFFER_S stBackBuf;
    HIFB_BLIT_OPT_S stBlitOpt;
    HI_U32 u32Depth  = 0;
    HI_U32 u32Stride = 0;
    HI_U32 CmpStride   = 0;

    info     = s_stLayer[u32LayerId].pstInfo;
    pstPar     = (HIFB_PAR_S *)info->par;
    u32Index = pstPar->stRunInfo.u32IndexForInt;

    HI_GFX_Memset(&stBlitOpt, 0, sizeof(HIFB_BLIT_OPT_S));
    HI_GFX_Memset(&stBackBuf, 0, sizeof(HIFB_BUFFER_S));


    stBlitOpt.bCallBack = HI_FALSE; /** TDE工作完不需要回调注册的TDE CALLBACK函数 **/
    stBlitOpt.pParam = &(pstPar->stBaseInfo.u32LayerID);

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&lockflag);
    pstPar->stRunInfo.bNeedFlip        = HI_FALSE;
    pstPar->stRunInfo.s32RefreshHandle = 0;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&lockflag);

    u32Depth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(pstPar->stExtendInfo.u32DisplayWidth,u32Depth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

    pstPar->st3DInfo.st3DSurface.enFmt       = pstPar->stExtendInfo.enColFmt;
    pstPar->st3DInfo.st3DSurface.u32Pitch  = u32Stride;
    pstPar->st3DInfo.st3DSurface.u32Width  = pstPar->stExtendInfo.u32DisplayWidth;
    pstPar->st3DInfo.st3DSurface.u32Height = pstPar->stExtendInfo.u32DisplayHeight;
    pstPar->st3DInfo.st3DSurface.u32PhyAddr= pstPar->st3DInfo.u32DisplayAddr[u32Index];

    HI_GFX_Memcpy(&stBackBuf.stCanvas, &pstPar->st3DInfo.st3DSurface, sizeof(HIFB_SURFACE_S));

    /**
     ** according to the hw arithemetic, calculate  source and Dst fresh rectangle
     **/
    hifb_getupdate_rect(u32LayerId, pstCanvasBuf, &stBackBuf.UpdateRect);

    /**
     ** when fill background buffer, we need to backup fore buffer first
     **/
    hifb_backup_forebuf(u32LayerId, &stBackBuf);

    /* update union rect */
    HI_GFX_Memcpy(&pstPar->st3DInfo.st3DUpdateRect, &stBackBuf.UpdateRect, sizeof(HIFB_RECT));


    stBlitOpt.bScale = HI_TRUE;
    stBlitOpt.bBlock = HI_TRUE;
    stBlitOpt.bRegionDeflicker = HI_TRUE;
    if (pstPar->stBaseInfo.enAntiflickerMode == HIFB_ANTIFLICKER_TDE)
    {
        stBlitOpt.enAntiflickerLevel = pstPar->stBaseInfo.enAntiflickerLevel;
    }

    DRV_HIFB_SteroDataUpdate(u32LayerId, pstCanvasBuf, &stBlitOpt);

    pstPar->stRunInfo.bModifying = HI_TRUE;
    pstPar->stRunInfo.u32ScreenAddr       = pstPar->st3DInfo.u32DisplayAddr[u32Index];
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    pstPar->stRunInfo.bModifying = HI_FALSE;

    pstPar->stRunInfo.u32IndexForInt = 1 - u32Index;

    HI_GFX_Memcpy(&(pstPar->stDispInfo.stUserBuffer), pstCanvasBuf, sizeof(HIFB_BUFFER_S));

    /**
     ** wait the address register's configuration take effect before return
     **/

    DRV_HIFB_WaitVBlank(u32LayerId);

    return HI_SUCCESS;

}
#endif



/***************************************************************************************
* func          : hifb_refresh
* description   : CNcomment: 刷新 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_refresh(HI_U32 u32LayerId, HIFB_BUFFER_S *pstCanvasBuf, HIFB_LAYER_BUF_E enBufMode)
{

    HI_S32 s32Ret;
    HIFB_PAR_S *par;
    struct fb_info *info;

    s32Ret = HI_FAILURE;
    info   = s_stLayer[u32LayerId].pstInfo;
    par    = (HIFB_PAR_S *)(info->par);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (par->st3DInfo.IsStereo)
    {
        switch (enBufMode)
        {
            case HIFB_LAYER_BUF_DOUBLE:
                s32Ret = hifb_refresh_2buf_3D(u32LayerId, pstCanvasBuf);
                break;
            case HIFB_LAYER_BUF_ONE:
                s32Ret = hifb_refresh_1buf_3D(u32LayerId, pstCanvasBuf);
                break;
            case HIFB_LAYER_BUF_NONE:
               s32Ret = hifb_refresh_0buf_3D(u32LayerId, pstCanvasBuf);
               break;
            case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
                s32Ret = hifb_refresh_2buf_immediate_display_3D(u32LayerId, pstCanvasBuf);
                break;
            default:
                break;
        }
    }
    else
#endif
    {
        switch (enBufMode)
        {
            case HIFB_LAYER_BUF_DOUBLE:
                s32Ret = hifb_refresh_2buf(u32LayerId, pstCanvasBuf);
                break;
            case HIFB_LAYER_BUF_ONE:
                s32Ret = hifb_refresh_1buf(u32LayerId, pstCanvasBuf);
                break;
            case HIFB_LAYER_BUF_NONE:
               s32Ret = hifb_refresh_0buf(u32LayerId, pstCanvasBuf);
               break;
            case HIFB_LAYER_BUF_DOUBLE_IMMEDIATE:
                s32Ret = hifb_refresh_2buf_immediate_display(u32LayerId, pstCanvasBuf);
                break;
            default:
                break;
        }
    }

    return s32Ret;

}


static HI_S32 DRV_HIFB_Refresh(HIFB_PAR_S* par, HI_VOID __user *argp)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HIFB_BUFFER_S stCanvasBuf;

    if(par->stBaseInfo.u32LayerID == HIFB_LAYER_CURSOR){
        HIFB_WARNING("you shouldn't refresh cursor layer!");
        return HI_SUCCESS;
    }

    if(copy_from_user(&stCanvasBuf, argp, sizeof(HIFB_BUFFER_S))){
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return -EFAULT;
    }

    /**
     ** when user data  update in 3d mode ,
     ** blit pan buffer to 3D buffer to config 3d data
     **/
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (  (par->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY)
         &&((par->st3DInfo.st3DMemInfo.u32StereoMemStart != 0) && (par->st3DInfo.IsStereo)))
    {/**
      ** 标准刷新模式，3D模式，起始地址不为0
      **/
        return hifb_refresh_panbuf(par->stBaseInfo.u32LayerID, &stCanvasBuf);
    }
    /**
     ** when user refresh in pan display , just return
     **/
    if (par->stExtendInfo.enBufMode == HIFB_REFRESH_MODE_WITH_PANDISPLAY)
    {
        return HI_FAILURE;
    }
#endif

    if (   (0 == stCanvasBuf.stCanvas.u32Width)
        || (0 == stCanvasBuf.stCanvas.u32Height))
    {
        HIFB_ERROR("canvas buffer's width or height can't be zero.\n");
        return HI_FAILURE;
    }

    if (stCanvasBuf.stCanvas.enFmt >= HIFB_FMT_BUTT)
    {
        HIFB_ERROR("color format of canvas buffer unsupported.\n");
        return HI_FAILURE;
    }

    if (  (stCanvasBuf.UpdateRect.x >=  stCanvasBuf.stCanvas.u32Width)
        ||(stCanvasBuf.UpdateRect.y >= stCanvasBuf.stCanvas.u32Height)
        ||(stCanvasBuf.UpdateRect.w == 0) || (stCanvasBuf.UpdateRect.h == 0))
    {
        HIFB_ERROR("rect error: update rect:(%d,%d,%d,%d), canvas range:(%d,%d)\n",
                  stCanvasBuf.UpdateRect.x, stCanvasBuf.UpdateRect.y,
                  stCanvasBuf.UpdateRect.w, stCanvasBuf.UpdateRect.h,
                  stCanvasBuf.stCanvas.u32Width, stCanvasBuf.stCanvas.u32Height);
        return HI_FAILURE;
    }

    if (stCanvasBuf.UpdateRect.x + stCanvasBuf.UpdateRect.w > stCanvasBuf.stCanvas.u32Width)
    {
        stCanvasBuf.UpdateRect.w = stCanvasBuf.stCanvas.u32Width - stCanvasBuf.UpdateRect.x;
    }
    if (stCanvasBuf.UpdateRect.y + stCanvasBuf.UpdateRect.h > stCanvasBuf.stCanvas.u32Height)
    {
        stCanvasBuf.UpdateRect.h =  stCanvasBuf.stCanvas.u32Height - stCanvasBuf.UpdateRect.y;
    }

    if (par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE)
    {/** 只有canvas buffer **/
        /**
         ** there's a limit from hardware that the start address of screen buf
         ** should be 16byte aligned!
         **/
        if ((stCanvasBuf.stCanvas.u32PhyAddr & 0xf) || (stCanvasBuf.stCanvas.u32Pitch & 0xf))
        {
            HIFB_ERROR("addr 0x%x or pitch: 0x%x is not 16 bytes align !\n",
                stCanvasBuf.stCanvas.u32PhyAddr,
                stCanvasBuf.stCanvas.u32Pitch);
            return HI_FAILURE;
        }
    }

    s32Ret = hifb_refresh(par->stBaseInfo.u32LayerID, &stCanvasBuf, par->stExtendInfo.enBufMode);

    return s32Ret;
}



static HI_S32 DRV_HIFB_SetLayerInfo(struct fb_info *info, HIFB_PAR_S* par, HI_VOID __user *argp)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HIFB_LAYER_INFO_S stLayerInfo;

    if(HIFB_LAYER_CURSOR == par->stBaseInfo.u32LayerID){
       HIFB_WARNING("you shouldn't put cursor layer info!");
       return HI_SUCCESS;
    }

    if(copy_from_user(&stLayerInfo, argp, sizeof(HIFB_LAYER_INFO_S))){
       HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
       return -EFAULT;
    }

    hifb_set_canvasbufinfo(info, &stLayerInfo);

    s32Ret = HIFB_OSR_CheckWhetherMemSizeEnough(info,&stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("HIFB_OSR_CheckWhetherMemSizeEnough failed\n");
        return HI_FAILURE;
    }

    s32Ret = HIFB_OSR_CheckWhetherScreenSizeEqualToZero(&stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("HIFB_OSR_CheckWhetherScreenSizeEqualToZero failed\n");
        return HI_FAILURE;
    }


#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
     if((stLayerInfo.u32Mask & HIFB_LAYERMASK_DISPSIZE) && par->st3DInfo.IsStereo)
     {
        HIFB_OSR_ClearUnUsedStereoBuf(info);
     }
#endif

    s32Ret = HIFB_OSR_CheckWhetherLayerSizeSupport(info, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("HIFB_OSR_CheckWhetherLayerSizeSupport failed\n");
        return HI_FAILURE;
    }

    s32Ret = HIFB_OSR_CheckWhetherLayerPosSupport(&stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("HIFB_OSR_CheckWhetherLayerPosSupport failed\n");
        return HI_FAILURE;
    }

    if((stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL) && par->stExtendInfo.stCkey.bKeyEnable)
    {
       HIFB_ERROR("Colorkey and premul couldn't take effect at same time!\n");
       return HI_FAILURE;
    }

    par->stRunInfo.bModifying = HI_TRUE;

    if(stLayerInfo.u32Mask & HIFB_LAYERMASK_BMUL)
    {
        par->stBaseInfo.bPreMul            = stLayerInfo.bPreMul;
        par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_BMUL;
    }

    if(stLayerInfo.u32Mask & HIFB_LAYERMASK_BUFMODE)
    {
        hifb_buf_setbufmode(par->stBaseInfo.u32LayerID, stLayerInfo.BufMode);
    }

    if(stLayerInfo.u32Mask & HIFB_LAYERMASK_POS)
    {
        hifb_disp_setlayerpos(par->stBaseInfo.u32LayerID, stLayerInfo.s32XPos, stLayerInfo.s32YPos);
    }

    if(stLayerInfo.u32Mask & HIFB_LAYERMASK_ANTIFLICKER_MODE)
    {
        hifb_disp_setantiflickerlevel(par->stBaseInfo.u32LayerID, stLayerInfo.eAntiflickerLevel);
    }

    if(stLayerInfo.u32Mask & HIFB_LAYERMASK_SCREENSIZE)
    {
        s32Ret = hifb_disp_setscreensize(par->stBaseInfo.u32LayerID, stLayerInfo.u32ScreenWidth, stLayerInfo.u32ScreenHeight);
    }

    HIFB_OSR_CheckWhetherDispSizeChange(info,&stLayerInfo);

    par->stRunInfo.bModifying = HI_FALSE;

    return s32Ret;

}


static inline HI_S32 HIFB_OSR_CheckWhetherMemSizeEnough(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo)
{

     HI_U32 PixDepth = 0;
     HI_U32 Stride   = 0;
     HI_U32 CmpStride   = 0;
     HI_U32 IsInputDataSizeHasChanged = 0x0;
     IsInputDataSizeHasChanged = (pstLayerInfo->u32Mask & HIFB_LAYERMASK_DISPSIZE);

     if (!IsInputDataSizeHasChanged)
     {
        return HI_SUCCESS;
     }

     PixDepth = info->var.bits_per_pixel >> 3;
     Stride = CONIFG_HIFB_GetMaxStride(pstLayerInfo->u32DisplayWidth, PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

     if (pstLayerInfo->u32DisplayWidth == 0 || pstLayerInfo->u32DisplayHeight == 0)
     {
         HIFB_ERROR("display witdh/height shouldn't be 0!\n");
         return HI_FAILURE;
     }

     if (HI_FAILURE == hifb_checkmem_enough(info, Stride, pstLayerInfo->u32DisplayHeight))
     {
         HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
         return HI_FAILURE;
     }

     return HI_SUCCESS;
}


static inline HI_S32 HIFB_OSR_CheckWhetherScreenSizeEqualToZero(HIFB_LAYER_INFO_S *pstLayerInfo)
{
     HI_U32 IsInputDataScreenSizeHasChanged = 0x0;
     IsInputDataScreenSizeHasChanged = (pstLayerInfo->u32Mask & HIFB_LAYERMASK_SCREENSIZE);

     if (!IsInputDataScreenSizeHasChanged)
     {
        return HI_SUCCESS;
     }

     if ((pstLayerInfo->u32ScreenWidth == 0) || (pstLayerInfo->u32ScreenHeight == 0))
     {
         HIFB_ERROR("screen width/height shouldn't be 0\n");
         return HI_FAILURE;
     }

     return HI_SUCCESS;
}


static inline HI_S32 HIFB_OSR_CheckWhetherLayerSizeSupport(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo)
{
    HI_U32 LayerSize = 0;

    if (pstLayerInfo->u32Mask & HIFB_LAYERMASK_BUFMODE)
    {
        LayerSize = HI_HIFB_GetMemSize(info->fix.line_length,info->var.yres);
        if (pstLayerInfo->BufMode == HIFB_LAYER_BUF_ONE)
        {
           LayerSize = 1 * LayerSize;
        }
        else if ((pstLayerInfo->BufMode == HIFB_LAYER_BUF_DOUBLE) || (pstLayerInfo->BufMode == HIFB_LAYER_BUF_DOUBLE_IMMEDIATE))
        {
           LayerSize = 2 * LayerSize;
        }
        else
        {
           LayerSize = 0 * LayerSize;
        }

        if (LayerSize > info->fix.smem_len){
           HIFB_ERROR("No enough mem! layer real memory size:%d KBytes, expected:%d KBtyes\n",info->fix.smem_len/1024, LayerSize/1024);
           return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}


static inline HI_S32 HIFB_OSR_CheckWhetherLayerPosSupport(HIFB_LAYER_INFO_S *pstLayerInfo)
{
    if ((pstLayerInfo->u32Mask & HIFB_LAYERMASK_POS) && ((pstLayerInfo->s32XPos < 0) || (pstLayerInfo->s32YPos < 0)))
    {
       HIFB_ERROR("Pos err!\n");
       return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static inline HI_VOID HIFB_OSR_CheckWhetherDispSizeChange(struct fb_info *info, HIFB_LAYER_INFO_S *pstLayerInfo)
{
     HI_S32 Ret = HI_SUCCESS;
     HI_U32 IsInputDataSizeHasChanged = 0x0;
     HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;

     IsInputDataSizeHasChanged = (pstLayerInfo->u32Mask & HIFB_LAYERMASK_DISPSIZE);

     if (!IsInputDataSizeHasChanged)
     {
        return;
     }

     if ((pstLayerInfo->u32DisplayWidth > info->var.xres_virtual) || (pstLayerInfo->u32DisplayHeight > info->var.yres_virtual))
     {
          return;
     }

     Ret = hifb_disp_setdispsize(pstPar->stBaseInfo.u32LayerID, pstLayerInfo->u32DisplayWidth, pstLayerInfo->u32DisplayHeight);
     if (Ret == HI_SUCCESS)
     {
         info->var.xres = pstLayerInfo->u32DisplayWidth;
         info->var.yres = pstLayerInfo->u32DisplayHeight;
         hifb_assign_dispbuf(pstPar->stBaseInfo.u32LayerID);
     }

     hifb_refreshall(info);

     return;
}


static HI_S32 hifb_refreshuserbuffer(HI_U32 u32LayerId)
{
    HIFB_PAR_S *par;
    struct fb_info *info;

    info = s_stLayer[u32LayerId].pstInfo;
    par = (HIFB_PAR_S *)info->par;

    if (par->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr)
    {
        HIFB_BUFFER_S stCanvas;
        stCanvas = par->stDispInfo.stUserBuffer;
        stCanvas.UpdateRect.x = 0;
        stCanvas.UpdateRect.y = 0;
        stCanvas.UpdateRect.w = stCanvas.stCanvas.u32Width;
        stCanvas.UpdateRect.h = stCanvas.stCanvas.u32Height;

        hifb_refresh(par->stBaseInfo.u32LayerID, &stCanvas, par->stExtendInfo.enBufMode);
    }

    return HI_SUCCESS;

}

static HI_S32 hifb_refreshall(struct fb_info *info)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    if (par->st3DInfo.IsStereo)
    {
        if (HIFB_REFRESH_MODE_WITH_PANDISPLAY == par->stExtendInfo.enBufMode)
        {
            DRV_HIFB_PanDisplay(&info->var, info);
        }

        if (HIFB_LAYER_BUF_NONE == par->stExtendInfo.enBufMode)
        {
            hifb_refreshuserbuffer(par->stBaseInfo.u32LayerID);
        }
    }
#endif

    if (HIFB_REFRESH_MODE_WITH_PANDISPLAY != par->stExtendInfo.enBufMode
         && HIFB_LAYER_BUF_NONE != par->stExtendInfo.enBufMode)
    {
        hifb_refreshuserbuffer(par->stBaseInfo.u32LayerID);
    }

    return HI_SUCCESS;
}


#ifdef CONFIG_COMPAT
static HI_S32 hifb_compat_ioctl(struct fb_info *info, HI_U32 cmd, unsigned long arg)
{
    struct fb_fix_screeninfo *fix = &(info->fix);
    struct fb_fix_compat_screeninfo *fix32 = NULL;
    HIFB_PAR_S *pstPar = NULL;
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 VirMemStart = 0;

    if ((NULL == info) || (NULL == info->par))
    {
        HIFB_ERROR("input para error\n");
        return -EINVAL;
    }

    fix = &(info->fix);
    pstPar = (HIFB_PAR_S *)info->par;

    if (FBIOGET_FSCREENINFO_HIFB == cmd)
    {
         if (0 == arg)
         {
            HIFB_ERROR("input para error\n");
            return -EINVAL;
         }

         fix32 = compat_ptr(arg);
         VirMemStart = (HI_U32)(unsigned long)fix->mmio_start;

         Ret = copy_to_user(&fix32->id, &fix->id, sizeof(fix32->id));
         Ret |= copy_to_user(fix32->reserved, fix->reserved, sizeof(fix->reserved));

         Ret |= put_user(pstPar->u32SmemStartPhy, &fix32->smem_start);
         Ret |= put_user(fix->smem_len,        &fix32->smem_len);
         Ret |= put_user(fix->type,            &fix32->type);
         Ret |= put_user(fix->type_aux,        &fix32->type_aux);
         Ret |= put_user(fix->visual,          &fix32->visual);
         Ret |= put_user(fix->xpanstep,        &fix32->xpanstep);
         Ret |= put_user(fix->ypanstep,        &fix32->ypanstep);
         Ret |= put_user(fix->ywrapstep,       &fix32->ywrapstep);
         Ret |= put_user(fix->line_length,     &fix32->line_length);
         Ret |= put_user(VirMemStart,          &fix32->mmio_start);
         Ret |= put_user(fix->mmio_len,        &fix32->mmio_len);
         Ret |= put_user(fix->accel,           &fix32->accel);
         Ret |= put_user(fix->capabilities,    &fix32->capabilities);

         if (Ret){
             return -EFAULT;
         }

         return Ret;
    }

    return hifb_ioctl(info, cmd, arg);
}

#endif



static HI_S32 hifb_ioctl(struct fb_info *info, HI_U32 cmd, unsigned long arg)
{

    HI_S32 s32Cnt = 0;
    HI_U8 u8Cmd = _IOC_NR (cmd);
    HIFB_PAR_S *pstPar = NULL;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    if ((NULL == info) || (NULL == info->par))
    {
        HIFB_ERROR("input para error\n");
        return -EINVAL;
    }

    if ((NULL == argp) && (cmd != FBIOGET_VBLANK_HIFB) && (cmd != FBIO_WAITFOR_FREFRESH_DONE) && (cmd != FBIO_FREE_LOGO))
    {
        return -EINVAL;
    }

    pstPar = (HIFB_PAR_S *)info->par;

    s32Cnt = atomic_read(&pstPar->stBaseInfo.ref_count);
    if (s32Cnt <= 0)
    {
        HIFB_ERROR("hifb%d not open\n",pstPar->stBaseInfo.u32LayerID);
        return -EINVAL;
    }

    if ((!gs_pstCapacity[pstPar->stBaseInfo.u32LayerID].bLayerSupported) && (pstPar->stBaseInfo.u32LayerID != HIFB_LAYER_CURSOR))
    {
        HIFB_ERROR("not supprot layer %d!\n", pstPar->stBaseInfo.u32LayerID);
        return HI_FAILURE;
    }

    if (FBIOGET_FSCREENINFO_HIFB == cmd)
    {
        return gs_DrvHifbCtlFunc[49].DrvHifbIoctlFunc(info,arg);
    }

    DRV_HIFB_CHECK_IOCTL_CMD(u8Cmd);
    DRV_HIFB_CHECK_IOCTL_NUM(gs_DrvHifbCtlNum[u8Cmd]);

    if (NULL == gs_DrvHifbCtlFunc[gs_DrvHifbCtlNum[u8Cmd]].DrvHifbIoctlFunc)
    {
        HIFB_ERROR("this cmd not support\n");
        return HI_FAILURE;
    }

    if (cmd != gs_DrvHifbCtlFunc[gs_DrvHifbCtlNum[u8Cmd]].Cmd)
    {
        HIFB_ERROR("this cmd not support\n");
        return HI_FAILURE;
    }

    return gs_DrvHifbCtlFunc[gs_DrvHifbCtlNum[u8Cmd]].DrvHifbIoctlFunc(info,arg);
}


static HI_S32 HIFB_OSR_GetColorKey(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_COLORKEY_S ck;

    ck.bKeyEnable = par->stExtendInfo.stCkey.bKeyEnable;
    ck.u32Key = par->stExtendInfo.stCkey.u32Key;
    if (copy_to_user(argp, &ck, sizeof(HIFB_COLORKEY_S)))
    {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetColorKey(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_COLORKEY_S ckey;

    if (copy_from_user(&ckey, argp, sizeof(HIFB_COLORKEY_S)))
    {
        return -EFAULT;
    }

    if (ckey.bKeyEnable && pstPar->stBaseInfo.bPreMul)
    {
        HIFB_ERROR("colorkey and premul couldn't take effect at the same time!\n");
        return HI_FAILURE;
    }

    pstPar->stRunInfo.bModifying = HI_TRUE;

    pstPar->stExtendInfo.stCkey.u32Key = ckey.u32Key;
    pstPar->stExtendInfo.stCkey.bKeyEnable = ckey.bKeyEnable;


    if (info->var.bits_per_pixel <= 8)
    {
        if (ckey.u32Key >= (2 << info->var.bits_per_pixel))
        {
            HIFB_ERROR("The key :%d is out of range the palette: %d!\n",
                        ckey.u32Key, 2 << info->var.bits_per_pixel);
            return HI_FAILURE;
        }

        pstPar->stExtendInfo.stCkey.u8BlueMax  = pstPar->stExtendInfo.stCkey.u8BlueMin = info->cmap.blue[ckey.u32Key];
        pstPar->stExtendInfo.stCkey.u8GreenMax = pstPar->stExtendInfo.stCkey.u8GreenMin = info->cmap.green[ckey.u32Key];
        pstPar->stExtendInfo.stCkey.u8RedMax   = pstPar->stExtendInfo.stCkey.u8RedMin = info->cmap.red[ckey.u32Key];
    }
    else
    {
        HI_U8 u8RMask, u8GMask, u8BMask;
        g_stDrvAdpCallBackFunction.HIFB_DRV_ColorConvert(&info->var, &pstPar->stExtendInfo.stCkey);
        u8BMask  = (0xff >> s_stArgbBitField[pstPar->stExtendInfo.enColFmt].stBlue.length);
        u8GMask  = (0xff >> s_stArgbBitField[pstPar->stExtendInfo.enColFmt].stGreen.length);
        u8RMask  = (0xff >> s_stArgbBitField[pstPar->stExtendInfo.enColFmt].stRed.length);

        pstPar->stExtendInfo.stCkey.u8BlueMin  = (pstPar->stExtendInfo.stCkey.u32Key & (~u8BMask));
        pstPar->stExtendInfo.stCkey.u8GreenMin = ((pstPar->stExtendInfo.stCkey.u32Key >> 8) & (~u8GMask));
        pstPar->stExtendInfo.stCkey.u8RedMin   = ((pstPar->stExtendInfo.stCkey.u32Key >> 16) & (~u8RMask));

        pstPar->stExtendInfo.stCkey.u8BlueMax  = pstPar->stExtendInfo.stCkey.u8BlueMin | u8BMask;
        pstPar->stExtendInfo.stCkey.u8GreenMax = pstPar->stExtendInfo.stCkey.u8GreenMin | u8GMask;
        pstPar->stExtendInfo.stCkey.u8RedMax   = pstPar->stExtendInfo.stCkey.u8RedMin | u8RMask;
    }

    pstPar->stExtendInfo.stCkey.u8RedMask   = 0xff;
    pstPar->stExtendInfo.stCkey.u8BlueMask  = 0xff;
    pstPar->stExtendInfo.stCkey.u8GreenMask = 0xff;

    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_COLORKEY;
    pstPar->stRunInfo.bModifying          = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetLayerAlpha(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    if (copy_to_user(argp, &par->stExtendInfo.stAlpha, sizeof(HIFB_ALPHA_S)))
    {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetLayerAlpha(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_ALPHA_S stAlpha = {0};

    if (copy_from_user(&par->stExtendInfo.stAlpha, argp, sizeof(HIFB_ALPHA_S)))
    {
        return -EFAULT;
    }

    stAlpha = par->stExtendInfo.stAlpha;
    if (!par->stExtendInfo.stAlpha.bAlphaChannel)
    {
        stAlpha.u8GlobalAlpha |= 0xff;
        par->stExtendInfo.stAlpha.u8GlobalAlpha |= 0xff;
    }

    par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_ALPHA;

    return HI_SUCCESS;
}


static HI_S32 HIFB_OSR_GetScreenOriginPos(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    if (copy_to_user(argp, &par->stExtendInfo.stPos, sizeof(HIFB_POINT_S)))
    {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetScreenOriginPos(struct fb_info *info, HI_ULONG arg)
{
     HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
     HI_VOID __user *argp = (HI_VOID __user *)arg;
     HIFB_POINT_S origin;

#ifndef CONFIG_HIFB_CURSOR_LAYER_NEGATIVE_SUPPORT
     HI_U32 u32Bpp, u32Addr;
     HIFB_RECT   stOutputRect;
     HI_U32 u32LayerId;
#endif

     if (copy_from_user(&origin, argp, sizeof(HIFB_POINT_S)))
     {
         return -EFAULT;
     }

#ifdef CONFIG_HIFB_CURSOR_LAYER_NEGATIVE_SUPPORT
     /** 鼠标层支持负坐标处理 **/
     if (par->stBaseInfo.u32LayerID != HIFB_LAYER_HD_3)
     {/** 非鼠标层不支持负坐标处理 **/
         if (origin.s32XPos < 0 || origin.s32YPos < 0)
         {
             HIFB_ERROR("It's not supported to set start pos of layer to negative!\n");
             return HI_FAILURE;
         }
     }
     par->stRunInfo.bModifying = HI_TRUE;
     par->stExtendInfo.stPos.s32XPos = origin.s32XPos;
     par->stExtendInfo.stPos.s32YPos = origin.s32YPos;
#else
     u32LayerId = par->stBaseInfo.u32LayerID;
     if (par->stExtendInfo.enBufMode != HIFB_LAYER_BUF_NONE)
     {
         if (origin.s32XPos < 0 || origin.s32YPos < 0)
         {
             HIFB_ERROR("It's not supported to set start pos of layer to negative!\n");
             return HI_FAILURE;
         }
     }

     g_stDrvAdpCallBackFunction.HIFB_DRV_GetLayerOutRect(u32LayerId, &stOutputRect);

     par->stRunInfo.bModifying = HI_TRUE;
     par->stExtendInfo.stPos.s32XPos  = origin.s32XPos;
     par->stExtendInfo.stPos.s32YPos  = origin.s32YPos;

     if (origin.s32XPos > stOutputRect.w - HIFB_MIN_WIDTH(u32LayerId))
     {
         par->stExtendInfo.stPos.s32XPos = stOutputRect.w - HIFB_MIN_WIDTH(u32LayerId);
     }

     if (origin.s32YPos > stOutputRect.h - HIFB_MIN_HEIGHT(u32LayerId))
     {
         par->stExtendInfo.stPos.s32YPos = stOutputRect.h - HIFB_MIN_HEIGHT(u32LayerId);
     }

     if (origin.s32XPos < 0 || origin.s32YPos < 0)
     {
         HI_U32 u32XPos, u32YPos;

         u32Bpp = DRV_HIFB_GetBppByFmt(par->stDispInfo.stUserBuffer.stCanvas.enFmt);
         u32Addr= par->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr;
         if (origin.s32XPos < 0)
         {
             u32XPos = 0-origin.s32XPos;
             if (u32XPos > par->stDispInfo.stUserBuffer.stCanvas.u32Width)
             {
                 u32XPos = par->stDispInfo.stUserBuffer.stCanvas.u32Width;
             }
             par->stExtendInfo.u32DisplayWidth = par->stDispInfo.stUserBuffer.stCanvas.u32Width-u32XPos;
             u32Addr +=  (u32XPos*u32Bpp/8);
             par->stExtendInfo.stPos.s32XPos = 0;
         }

         if (origin.s32YPos < 0)
         {
             u32YPos = 0-origin.s32YPos;
             if (u32YPos > par->stDispInfo.stUserBuffer.stCanvas.u32Height)
             {
                 u32YPos = par->stDispInfo.stUserBuffer.stCanvas.u32Height;
             }
             par->stExtendInfo.u32DisplayHeight = par->stDispInfo.stUserBuffer.stCanvas.u32Height-u32YPos;
             u32Addr +=  par->stDispInfo.stUserBuffer.stCanvas.u32Pitch*u32YPos;
             par->stExtendInfo.stPos.s32YPos = 0;
         }

         par->stRunInfo.u32ScreenAddr = (u32Addr&0xfffffff0);
         par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
         par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
     }

     if (par->stExtendInfo.enBufMode == HIFB_LAYER_BUF_NONE)
     {
         if (origin.s32XPos >= 0)
         {
             par->stExtendInfo.u32DisplayWidth = par->stDispInfo.stUserBuffer.stCanvas.u32Width;
         }

         if (origin.s32YPos >= 0)
         {
             par->stExtendInfo.u32DisplayHeight= par->stDispInfo.stUserBuffer.stCanvas.u32Height;
         }

         if (origin.s32XPos >= 0 && origin.s32YPos >= 0)
         {
             par->stRunInfo.u32ScreenAddr = par->stDispInfo.stUserBuffer.stCanvas.u32PhyAddr;
             par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
             par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
         }

     }
#endif
     par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;
     par->stRunInfo.bModifying = HI_FALSE;

     return HI_SUCCESS;
}


static HI_S32 HIFB_OSR_GetDeFlicker(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_FAILURE;
}

static HI_S32 HIFB_OSR_SetDeFlicker(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_FAILURE;
}

static HI_S32 HIFB_OSR_GetVblank(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (g_stDrvAdpCallBackFunction.HIFB_DRV_WaitVBlank(par->stBaseInfo.u32LayerID) < 0)
    {
        HIFB_WARNING("It is not support VBL!\n");
        return -EPERM;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ShowLayer(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_BOOL bShow = HI_FALSE;

    if (par->stBaseInfo.u32LayerID == HIFB_LAYER_CURSOR)
    {
        HIFB_WARNING("you shouldn't show cursor by this cmd!");
        return HI_SUCCESS;
    }

    if (copy_from_user(&bShow, argp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }

    /* reset the same status */
    if (bShow == par->stExtendInfo.bShow)
    {
        HIFB_INFO("The layer is show(%d) now!\n", par->stExtendInfo.bShow);
        return 0;
    }

    par->stRunInfo.bModifying          = HI_TRUE;
    par->stExtendInfo.bShow            = bShow;
    par->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_SHOW;
    par->stRunInfo.bModifying          = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetLayerShowState(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    if (copy_to_user(argp, &par->stExtendInfo.bShow, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetCapablity(struct fb_info *info, HI_ULONG arg)
{

    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    if (copy_to_user(argp, (HI_VOID *)&gs_pstCapacity[par->stBaseInfo.u32LayerID], sizeof(HIFB_CAPABILITY_S)))
    {
        HIFB_ERROR("FBIOGET_CAPABILITY_HIFB error\n");
        return -EFAULT;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetDecompress(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_BOOL bDeComp = HI_FALSE;
    if (copy_from_user(&bDeComp, argp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetDeCmpSwitch (par->stBaseInfo.u32LayerID, bDeComp);
    par->bDeCompress = bDeComp;
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetLayerInfo(struct fb_info *info, HI_ULONG arg)
{
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    Ret = DRV_HIFB_SetLayerInfo(info, par, argp);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetLayerInfo(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_LAYER_INFO_S stLayerInfo = {0};

    DRV_HIFB_WaitVBlank(par->stBaseInfo.u32LayerID);

    stLayerInfo.bPreMul           = par->stBaseInfo.bPreMul;
    stLayerInfo.BufMode           = par->stExtendInfo.enBufMode;
    stLayerInfo.eAntiflickerLevel = par->stBaseInfo.enAntiflickerLevel;
    stLayerInfo.s32XPos           = par->stExtendInfo.stPos.s32XPos;
    stLayerInfo.s32YPos           = par->stExtendInfo.stPos.s32YPos;
    stLayerInfo.u32DisplayWidth   = par->stExtendInfo.u32DisplayWidth;
    stLayerInfo.u32DisplayHeight  = par->stExtendInfo.u32DisplayHeight;
    stLayerInfo.u32ScreenWidth    = par->stExtendInfo.u32ScreenWidth;
    stLayerInfo.u32ScreenHeight   = par->stExtendInfo.u32ScreenHeight;

    if (copy_to_user(argp, &stLayerInfo, sizeof(HIFB_LAYER_INFO_S)))
    {
       return -EFAULT;
    }
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetCanvasBuffer(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    if (copy_to_user(argp, &(par->stDispInfo.stCanvasSur), sizeof(HIFB_SURFACE_S)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_RefreshLayer(struct fb_info *info, HI_ULONG arg)
{
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    Ret = DRV_HIFB_Refresh(par, argp);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

#ifdef CONFIG_HIFB_TRANSIT_LOGO_BY_MASK
    DRV_HIFB_CloseLogoByMask(par->stBaseInfo.u32LayerID);
#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_WaiteRefreshFinish(struct fb_info *info, HI_ULONG arg)
{
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if ((par->stRunInfo.s32RefreshHandle) && (par->stExtendInfo.enBufMode != HIFB_LAYER_BUF_ONE))
    {
        Ret = g_stGfx2dCallBackFunction.HIFB_DRV_WaitForDone(par->stRunInfo.s32RefreshHandle, 1000);
    }

    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetSteroFraming(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_FAILURE;
}

static HI_S32 HIFB_OSR_GetSteroFraming(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_FAILURE;
}

static HI_S32 HIFB_OSR_SetSteroMode(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetSteroMode(struct fb_info *info, HI_ULONG arg)
{
    UNUSED(info);
    UNUSED(arg);
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetScreenSize(struct fb_info *info, HI_ULONG arg)
{
#ifndef CFG_HIFB_VIRTUAL_COORDINATE_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_SIZE_S stScreenSize;

    if (par->stBaseInfo.u32LayerID == HIFB_LAYER_CURSOR)
    {
        HIFB_WARNING("you shouldn't set cursor origion!");
        return HI_SUCCESS;
    }

    if (copy_from_user(&stScreenSize, argp, sizeof(HIFB_SIZE_S)))
    {
        return -EFAULT;
    }
    Ret = hifb_disp_setscreensize(par->stBaseInfo.u32LayerID, stScreenSize.u32Width, stScreenSize.u32Height);
    if (HI_SUCCESS != Ret)
    {
       return HI_FAILURE;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetScreenSize(struct fb_info *info, HI_ULONG arg)
{
    HIFB_SIZE_S stScreenSize;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
#ifndef CFG_HIFB_VIRTUAL_COORDINATE_SUPPORT

    DRV_HIFB_WaitVBlank(par->stBaseInfo.u32LayerID);

    stScreenSize.u32Width  = par->stExtendInfo.u32ScreenWidth;
    stScreenSize.u32Height = par->stExtendInfo.u32ScreenHeight;

    if (copy_to_user(argp, &stScreenSize, sizeof(HIFB_SIZE_S)))
    {
        return -EFAULT;
    }

#else

    HIFB_RECT   stOutputRect;
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetLayerOutRect(par->stBaseInfo.u32LayerID, &stOutputRect);
    stScreenSize.u32Width = stOutputRect.w;
    stScreenSize.u32Height = stOutputRect.h;
    if (copy_to_user(argp, &stScreenSize, sizeof(HIFB_SIZE_S)))
    {
        return -EFAULT;
    }

#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetCompress(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
   HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
   HI_VOID __user *argp = (HI_VOID __user *)arg;
   HI_BOOL bComp = HI_FALSE;
   HI_BOOL bComp_pre = HI_FALSE;

   if (copy_from_user(&bComp, argp, sizeof(HI_BOOL)))
   {
       return -EFAULT;
   }

   bComp_pre = g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(par->stBaseInfo.u32LayerID);

   if (bComp == bComp_pre)
   {
       return HI_SUCCESS;
   }

   if (bComp == HI_TRUE)
   {
       if (!gs_pstCapacity[par->stBaseInfo.u32LayerID].bCompression)
       {
           HIFB_ERROR("hifb% don't support compression\n",par->stBaseInfo.u32LayerID);
           return HI_FAILURE;
       }

       if (par->stExtendInfo.enColFmt != HIFB_FMT_ARGB8888)
       {
           HIFB_ERROR("compression only support pixel format (ARGB8888)\n");
           return HI_FAILURE;
       }

       if (par->st3DInfo.IsStereo)
       {
           HIFB_ERROR("not support compression in 3d mode\n");
           return HI_FAILURE;
       }
   }
   g_stDrvAdpCallBackFunction.HIFB_DRV_SetCmpSwitch(par->stBaseInfo.u32LayerID, bComp);
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetCompress(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_BOOL bComp = HI_FALSE;

    bComp = g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpSwitch(par->stBaseInfo.u32LayerID);
    if (copy_to_user(argp, &bComp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ScrolltextCreate(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_SCROLLTEXT_CREATE_S stScrollText;

    HI_GFX_Memset(&stScrollText, 0x0, sizeof(HIFB_SCROLLTEXT_CREATE_S));
    if (copy_from_user(&stScrollText, argp, sizeof(HIFB_SCROLLTEXT_CREATE_S)))
    {
        return -EFAULT;
    }

    if (stScrollText.stAttr.ePixelFmt >= HIFB_FMT_BUTT)
    {
        HIFB_ERROR("Invalid attributes.\n");
        return HI_FAILURE;
    }

    if (stScrollText.stAttr.stRect.w < 0 || stScrollText.stAttr.stRect.h < 0)
    {
        HIFB_ERROR("Invalid attributes.\n");
        return HI_FAILURE;
    }

    Ret = hifb_create_scrolltext(par->stBaseInfo.u32LayerID, &stScrollText);
    if (HI_SUCCESS != Ret)
    {
        return -EFAULT;
    }

    if (copy_to_user(argp, &stScrollText, sizeof(HIFB_SCROLLTEXT_CREATE_S)))
    {
       return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ScrolltextFill(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_SCROLLTEXT_DATA_S stScrollTextData;
    HIFB_SCROLLTEXT_DATA_64BITS_S stScrollText64BitsData;

    HI_GFX_Memset(&stScrollTextData, 0x0, sizeof(HIFB_SCROLLTEXT_DATA_S));
    HI_GFX_Memset(&stScrollText64BitsData, 0x0, sizeof(HIFB_SCROLLTEXT_DATA_64BITS_S));
    if (copy_from_user(&stScrollText64BitsData, argp, sizeof(HIFB_SCROLLTEXT_DATA_64BITS_S)))
    {
        return -EFAULT;
    }
    stScrollTextData.u32Handle  = stScrollText64BitsData.u32Handle;
    stScrollTextData.u32PhyAddr = stScrollText64BitsData.u32PhyAddr;
    stScrollTextData.pu8VirAddr = (HI_U8*)(unsigned long)stScrollText64BitsData.u64VirAddr;
    stScrollTextData.u32Stride  = stScrollText64BitsData.u32Stride;

    if ((HI_NULL == stScrollTextData.u32PhyAddr) && (HI_NULL == stScrollTextData.pu8VirAddr))
    {
        HIFB_ERROR("invalid usr data!\n");
        return -EFAULT;
    }

    Ret = hifb_fill_scrolltext(&stScrollTextData);
    if (HI_SUCCESS != Ret)
    {
        return -EFAULT;
    }

#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ScrolltextPause(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_S32 Ret = HI_SUCCESS;

    Ret = HIFB_OSR_SCROLLTEXT_Enable(HI_TRUE, arg);
    if (HI_SUCCESS != Ret)
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ScrolltextResume(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_S32 Ret = HI_SUCCESS;

    Ret = HIFB_OSR_SCROLLTEXT_Enable(HI_FALSE, arg);
    if (HI_SUCCESS != Ret)
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}

#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
static inline HI_S32 HIFB_OSR_SCROLLTEXT_Enable(HI_BOOL bEnable, HI_ULONG arg)
{
     HI_S32 Ret = HI_SUCCESS;
     HI_U32 u32LayerId;
     HI_U32 u32ScrollTextID;
     HI_U32 u32Handle;
     HIFB_SCROLLTEXT_S *pstScrollText = NULL;

     HI_VOID __user *argp = (HI_VOID __user *)arg;

     if (copy_from_user(&u32Handle, argp, sizeof(HI_U32)))
     {
         return HI_FAILURE;
     }

     Ret = hifb_parse_scrolltexthandle(u32Handle,&u32LayerId,&u32ScrollTextID);
     if (HI_SUCCESS != Ret)
     {
         HIFB_ERROR("invalid scrolltext handle!\n");
         return HI_FAILURE;
     }

     pstScrollText = &(s_stTextLayer[u32LayerId].stScrollText[u32ScrollTextID]);

     pstScrollText->bPause = bEnable;

     return HI_SUCCESS;
}

#endif

static HI_S32 HIFB_OSR_ScrolltextDestory(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_U32 u32LayerId, u32ScrollTextID, u32Handle;

    if (copy_from_user(&u32Handle, argp, sizeof(HI_U32)))
    {
        return -EFAULT;
    }

    Ret = hifb_parse_scrolltexthandle(u32Handle,&u32LayerId,&u32ScrollTextID);
    if (HI_SUCCESS != Ret)
    {
        HIFB_ERROR("invalid scrolltext handle!\n");
        return -EFAULT;
    }

    Ret = hifb_destroy_scrolltext(u32LayerId,u32ScrollTextID);
    if (HI_SUCCESS != Ret)
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}


static HI_S32 HIFB_OSR_SetSteroDepth(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_S32 s32StereoDepth;
    if (copy_from_user(&s32StereoDepth, argp, sizeof(HI_S32)))
    {
        return -EFAULT;
    }

    if (!par->st3DInfo.IsStereo)
    {
        HIFB_ERROR("u need to set disp stereo mode first.\n");
        return HI_FAILURE;
    }

    g_stDrvAdpCallBackFunction.HIFB_DRV_SetStereoDepth(par->stBaseInfo.u32LayerID, s32StereoDepth);

    par->st3DInfo.s32StereoDepth = s32StereoDepth;
#else
    UNUSED(info);
    UNUSED(arg);
#endif

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetSteroDepth(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    if (!par->st3DInfo.IsStereo)
    {
        HIFB_ERROR("u need to set disp stereo mode first.\n");
        return HI_FAILURE;
    }

    if (copy_to_user(argp, &(par->st3DInfo.s32StereoDepth), sizeof(HI_S32)))
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetLayerZorder(struct fb_info *info, HI_ULONG arg)
{
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_ZORDER_E enZorder;
    if(copy_from_user(&enZorder, argp, sizeof(HIFB_ZORDER_E)))
    {
        return -EFAULT;
    }

    Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerPriority(par->stBaseInfo.u32LayerID, enZorder);
    if (HI_SUCCESS != Ret)
    {
       return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetLayerZorder(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HI_U32  u32Zorder;
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetLayerPriority(par->stBaseInfo.u32LayerID, &u32Zorder);
    if (copy_to_user(argp, &(u32Zorder), sizeof(HI_U32)))
    {
        return HI_FAILURE;
    }
     return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_ReleaseLogo(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    UNUSED(arg);
    DRV_HIFB_CloseLogo(par->stBaseInfo.u32LayerID);
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_SetCompressionMode(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_CMP_MODE_E enMode;

    if (copy_from_user(&enMode, argp, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }

    if (enMode < HIFB_CMP_BUTT)
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetCmpMode(par->stBaseInfo.u32LayerID, enMode);
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetCompressionMode(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_COMPRESSION_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_CMP_MODE_E enMode;

    enMode = g_stDrvAdpCallBackFunction.HIFB_DRV_GetCmpMode(par->stBaseInfo.u32LayerID);

    if (copy_to_user(argp, &enMode, sizeof(HI_BOOL)))
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_HwcRefresh(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HIFB_FENCE_SUPPORT
    HI_S32 Ret = HI_SUCCESS;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    Ret = DRV_HIFB_HwcRefresh(par, argp);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }
   #ifdef CONFIG_HIFB_TRANSIT_LOGO_BY_MASK
      DRV_HIFB_CloseLogoByMask(par->stBaseInfo.u32LayerID);
   #endif
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_GetSmemStartPhy(struct fb_info *info, HI_ULONG arg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    if (copy_to_user(argp, &(par->u32SmemStartPhy), sizeof(HI_U32)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_FixScreenInfo(struct fb_info *info, HI_ULONG arg)
{
    struct fb_fix_screeninfo fix;
#ifdef CONFIG_GFX_MMU_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
#endif
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    HI_GFX_Memcpy(&fix, &(info->fix), sizeof(fix));
#ifdef CONFIG_GFX_MMU_SUPPORT
    fix.smem_start = par->u32SmemStartPhy;
#endif
    if (copy_to_user(argp, &fix, sizeof(fix)))
    {
        return -EFAULT;
    }

    return HI_SUCCESS;
}

static HI_S32 HIFB_OSR_HwcGetLayerInfo(struct fb_info *info, HI_ULONG arg)
{
#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    HI_U32 Stride = 0;
    HI_U32 CmpStride = 0;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    HIFB_HWC_LAYERINFO_S stLayerInfo;
    HI_U32 Depth = 0;

    if ((NULL == par) || (NULL == argp))
    {
        return -EFAULT;
    }

    if (copy_from_user(&stLayerInfo, argp, sizeof(HIFB_HWC_LAYERINFO_S)))
    {
       return -EFAULT;
    }

    if ((HIFB_FMT_ABGR8888 == stLayerInfo.eFmt) || (HIFB_FMT_ARGB8888 == stLayerInfo.eFmt) || (HIFB_FMT_RGBA8888 == stLayerInfo.eFmt))
    {
        Depth = 4;
    }
    else if ((HIFB_FMT_RGB888 == stLayerInfo.eFmt) || (HIFB_FMT_BGR888 == stLayerInfo.eFmt))
    {
        Depth = 3;
    }
    else if ((HIFB_FMT_ABGR1555 == stLayerInfo.eFmt) || (HIFB_FMT_ARGB1555 == stLayerInfo.eFmt) || (HIFB_FMT_RGBA5551 == stLayerInfo.eFmt))
    {
        Depth = 2;
    }
    else
    {
        return -EFAULT;
    }

    Stride = CONIFG_HIFB_GetMaxStride(par->stExtendInfo.u32DisplayWidth,Depth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
    stLayerInfo.u32NoCmpStride = Stride;
    stLayerInfo.u32Stride = CmpStride;

    if (copy_to_user(argp, &stLayerInfo, sizeof(stLayerInfo)))
    {
        return -EFAULT;
    }
#else
    UNUSED(info);
    UNUSED(arg);
#endif
    return HI_SUCCESS;
}


#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
static HI_S32 DRV_OSR_StereoCallBack(HI_VOID * pParaml,HI_VOID * pParamr)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 LayerId = 0;
    HI_U32 u32BufferSize = 0;
    HI_U32 u32Stride     = 0;
    HI_U32 CmpStride     = 0;
    HI_U32 u32PixDepth   = 0;
    struct fb_info *info = NULL;
    HIFB_PAR_S *pstPar   = NULL;

    if ((NULL == pParaml) || (NULL == pParamr))
    {
         HIFB_ERROR("DRV_OSR_StereoCallBack input para point null\n");
         return HI_FAILURE;
    }

    LayerId = *((HI_U32*)pParaml);
    info    = s_stLayer[LayerId].pstInfo;
    pstPar  = (HIFB_PAR_S *)(info->par);

    pstPar->st3DInfo.StereoMode = *((HIFB_STEREO_MODE_E*)pParamr);
    pstPar->st3DInfo.IsStereoCallBack = HI_TRUE;

    if (HIFB_STEREO_MONO == pstPar->st3DInfo.StereoMode)
    {/** is not stereo, should free stereo buffer **/
        pstPar->st3DInfo.IsStereo = HI_FALSE;
        HIFB_OSR_FreeStereoBuf(pstPar);
    }
    else
    {
        if (HIFB_REFRESH_MODE_WITH_PANDISPLAY == pstPar->stExtendInfo.enBufMode)
        {
            u32PixDepth = info->var.bits_per_pixel >> 3;
            u32Stride   = CONIFG_HIFB_GetMaxStride(info->var.xres,u32PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);

            /**<-- one buffer need size **/
            u32BufferSize = HI_HIFB_GetMemSize(u32Stride,info->var.yres);
            u32BufferSize = u32BufferSize / 2;
            /**<-- generally need two buffer **/
            u32BufferSize *= pstPar->stRunInfo.StereoBufNum;

            Ret = HIFB_OSR_ReAllocStereoBuf(pstPar->stBaseInfo.u32LayerID, u32BufferSize);
            if (Ret < 0)
            {
                HIFB_INFO("fail to alloc 3d memory.\n ");
            }
            pstPar->st3DInfo.st3DSurface.u32Pitch = u32Stride;
        }
        pstPar->st3DInfo.IsStereo = HI_TRUE;
    }

    if (   (0 == pstPar->st3DInfo.st3DMemInfo.u32StereoMemStart)
        && (pstPar->st3DInfo.StereoMode == HIFB_STEREO_SIDEBYSIDE_HALF || pstPar->st3DInfo.StereoMode == HIFB_STEREO_TOPANDBOTTOM)
        && (HIFB_REFRESH_MODE_WITH_PANDISPLAY == pstPar->stExtendInfo.enBufMode))
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimMode(pstPar->stBaseInfo.u32LayerID, pstPar->st3DInfo.StereoMode, HIFB_STEREO_MONO);
    }
    else
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimMode(pstPar->stBaseInfo.u32LayerID, pstPar->st3DInfo.StereoMode, pstPar->st3DInfo.StereoMode);
    }

    pstPar->stRunInfo.bModifying          = HI_TRUE;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_INRECT;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_DISPLAYADDR;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    pstPar->stRunInfo.bModifying          = HI_FALSE;

    hifb_assign_dispbuf(pstPar->stBaseInfo.u32LayerID);
    HIFB_OSR_ClearUnUsedStereoBuf(info);

    if (HIFB_REFRESH_MODE_WITH_PANDISPLAY == pstPar->stExtendInfo.enBufMode)
    {
        //DRV_HIFB_PanDisplay(&info->var, info);
    }
    else
    {
        hifb_refreshuserbuffer(pstPar->stBaseInfo.u32LayerID);
    }



    return HI_SUCCESS;
}
#endif


#ifdef CONFIG_GFX_MMU_SUPPORT
extern phys_addr_t hisi_iommu_domain_iova_to_phys(unsigned long iova);
/***************************************************************************
* func          : hifb_map_mem_kernel_to_user
* description   :
                  CNcomment: 将内核态分配得smmu地址映射用户态的虚拟地址，
                             要是MMZ使用内核态标准实现，要是SMMU这里需要自己做映射页表 CNend\n
* retval        :
* others:       : NA
****************************************************************************/
static HI_S32 hifb_map_mem_kernel_to_user(struct fb_info *info, struct vm_area_struct *vma)
{

    unsigned long iova = 0;
    unsigned long addr = 0;
    unsigned long phy_addr;

    if ((NULL == info) || (NULL == vma))
    {
       HIFB_ERROR("+++input para point is null\n");
       return -EINVAL;
    }

    iova = (unsigned long)info->fix.smem_start;
    addr = vma->vm_start;

    if (0 == info->fix.smem_start)
    {
       HIFB_ERROR("+++info->fix.smem_start is zero\n");
       return -EINVAL;
    }

    if (info->fix.smem_len < (vma->vm_end - vma->vm_start))
    {
        HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
        return -EINVAL;
    }

    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

    if (info->fix.smem_len == (vma->vm_end - vma->vm_start))
    {
        return HI_GFX_MapToUser((HI_U32)info->fix.smem_start,vma);
    }

    while (addr < vma->vm_end)
    {
        phy_addr = (unsigned long)hisi_iommu_domain_iova_to_phys(iova);
        if (!phy_addr)
        {
            HIFB_ERROR("===func : %s line : %d error\n",__FUNCTION__,__LINE__);
            return HI_FAILURE;
        }
        remap_pfn_range(vma, addr, __phys_to_pfn(phy_addr), SZ_4K,vma->vm_page_prot);
        addr = addr + SZ_4K;
        iova = iova + SZ_4K;
    }

    return HI_SUCCESS;
}
#if 0/** 刷映射到用户态的虚拟地址cache **/
#define L2_CACHE_SIZE  (512 * 1024)
#ifdef CONFIG_64BIT
#define mmz_flush_dcache_all            flush_cache_all
#define mmz_flush_dcache_area           __flush_dcache_area
#else
#define mmz_flush_dcache_all            __cpuc_flush_kern_all
#define mmz_flush_dcache_area           __cpuc_flush_dcache_area
#endif
HI_S32 hifb_map_vir_flsuh(size,vir)
{
    if ((NULL == viraddr) || (0 == len)) {
    HI_PRINT("%s failed, viraddr:%p len:0x%x!\n", __func__, \
                                        viraddr, len);
    return;
    }
    if (len > L2_CACHE_SIZE)
#ifdef CONFIG_SMP
    on_each_cpu((smp_call_func_t)mmz_flush_dcache_all, NULL, 1);
#else
    mmz_flush_dcache_all();
#endif
    else{
      mmz_flush_dcache_area((void *)viraddr,(size_t)len);
    }
}
#endif
#endif



static HI_S32 DRV_HIFB_Close(struct fb_info *info, HI_S32 user)
{
    HIFB_PAR_S *par = NULL;
    HI_U32 s32Cnt = 0;

    if ((NULL == info) || (NULL == info->par))
    {
       return HI_FAILURE;
    }

    par = (HIFB_PAR_S *)info->par;
    s32Cnt = atomic_read(&par->stBaseInfo.ref_count);

    HIFB_CHECK_LAYERID_SUPPORT(par->stBaseInfo.u32LayerID);

    if (s32Cnt <= 0){
        return -EINVAL;
    }

    if (s32Cnt > 1){
        atomic_dec(&par->stBaseInfo.ref_count);
        return HI_SUCCESS;
    }

    g_stGfx2dCallBackFunction.HIFB_DRV_WaitAllTdeDone(HI_TRUE);

    par->stExtendInfo.bShow = HI_FALSE;
    if (HIFB_LAYER_CURSOR != par->stBaseInfo.u32LayerID){

        g_stDrvAdpCallBackFunction.HIFB_DRV_EnableLayer     (par->stBaseInfo.u32LayerID, HI_FALSE);
        g_stDrvAdpCallBackFunction.HIFB_DRV_UpdataLayerReg  (par->stBaseInfo.u32LayerID);

        g_stDrvAdpCallBackFunction.HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TYPE_VO,         HI_NULL, par->stBaseInfo.u32LayerID);
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TYPE_3DMode_CHG, HI_NULL, par->stBaseInfo.u32LayerID);

        DRV_HIFB_ReleaseScrollText(info);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
        HIFB_OSR_FreeStereoBuf(par);
        par->st3DInfo.StereoMode = HIFB_STEREO_MONO;
        par->st3DInfo.IsStereo        = HI_FALSE;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimMode(par->stBaseInfo.u32LayerID, HIFB_STEREO_MONO, HIFB_STEREO_MONO);
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimAddr(par->stBaseInfo.u32LayerID, HI_NULL);
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr (par->stBaseInfo.u32LayerID, HI_NULL);
#endif

        g_stDrvAdpCallBackFunction.HIFB_DRV_CloseLayer(par->stBaseInfo.u32LayerID);
    }


#ifdef CFG_HIFB_PROC_SUPPORT
    DRV_HIFB_DestoryProc(par->stBaseInfo.u32LayerID);
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
    DRV_HIFB_FenceDInit(par);
    par->bEndHwcRefresh = HI_FALSE;
#endif

    if (NULL != info->screen_base){
       HI_GFX_Memset(info->screen_base,0x0,info->fix.smem_len);
    }

    atomic_dec(&par->stBaseInfo.ref_count);

    return HI_SUCCESS;
}


static inline HI_VOID DRV_HIFB_ReleaseScrollText(struct fb_info *info)
{
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_U32 ScrollTextCnt = 0;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (HI_TRUE != s_stTextLayer[par->stBaseInfo.u32LayerID].bAvailable){
        return;
    }

    for (ScrollTextCnt = 0; ScrollTextCnt < SCROLLTEXT_NUM; ScrollTextCnt++){
        if (HI_TRUE != s_stTextLayer[par->stBaseInfo.u32LayerID].stScrollText[ScrollTextCnt].bAvailable){
           continue;
        }
        hifb_freescrolltext_cachebuf(&(s_stTextLayer[par->stBaseInfo.u32LayerID].stScrollText[ScrollTextCnt]));
        HI_GFX_Memset(&s_stTextLayer[par->stBaseInfo.u32LayerID].stScrollText[ScrollTextCnt],0,sizeof(HIFB_SCROLLTEXT_S));
    }
    s_stTextLayer[par->stBaseInfo.u32LayerID].bAvailable = HI_FALSE;
    s_stTextLayer[par->stBaseInfo.u32LayerID].u32textnum = 0;
    s_stTextLayer[par->stBaseInfo.u32LayerID].u32ScrollTextId = 0;
    flush_work(&s_stTextLayer[par->stBaseInfo.u32LayerID].blitScrollTextWork);
#else
    UNUSED(info);
#endif
    return;
}


static HI_S32 hifb_dosetcolreg(unsigned regno, unsigned red, unsigned green,
                          unsigned blue, unsigned transp, struct fb_info *info, HI_BOOL bUpdateReg)
{
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;
    //HI_U32 *pCmap;

    HI_U32 argb = ((transp & 0xff) << 24) | ((red & 0xff) << 16) | ((green & 0xff) << 8) | (blue & 0xff);

    if (regno > 255)
    {
        HIFB_WARNING("regno: %d, larger than 255!\n", regno);
        return HI_FAILURE;
    }

    g_stDrvAdpCallBackFunction.HIFB_DRV_SetColorReg(par->stBaseInfo.u32LayerID, regno, argb, bUpdateReg);
    return HI_SUCCESS;
}


/***************************************************************************************
* func          : _setcolreg
* description   : CNcomment: 设置调色板信息 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 _setcolreg(unsigned regno, unsigned red, unsigned green,
                          unsigned blue, unsigned transp, struct fb_info *info)
{
    HI_S32 Ret = HI_SUCCESS;

    switch (info->var.bits_per_pixel)
    {
        case 8:
            Ret = hifb_dosetcolreg(regno, red, green, blue, transp, info, HI_TRUE);
            break;
        case 16:
            if (regno >= 16)
            {
                break;
            }
            if (info->var.red.offset == 10)
            {
                /* 1:5:5:5 */
                ((u32*) (info->pseudo_palette))[regno] =
                    ((red   & 0xf800) >>  1) |
                    ((green & 0xf800) >>  6) |
                    ((blue  & 0xf800) >> 11);
            }
            else
            {
                /* 0:5:6:5 */
                ((u32*) (info->pseudo_palette))[regno] =
                    ((red   & 0xf800)      ) |
                    ((green & 0xfc00) >>  5) |
                    ((blue  & 0xf800) >> 11);
            }
            break;
        case 24:
        case 32:
            red   >>= 8;
            green >>= 8;
            blue  >>= 8;
            transp >>= 8;
            ((u32 *)(info->pseudo_palette))[regno] =
                (red   << info->var.red.offset)   |
                (green << info->var.green.offset) |
                (blue  << info->var.blue.offset)  |
                (transp  << info->var.transp.offset) ;
            break;
    }

    return Ret;

}

static HI_S32 hifb_setcolreg(unsigned regno, unsigned red, unsigned green,
                          unsigned blue, unsigned transp, struct fb_info *info)
{

    return _setcolreg(regno, red, green, blue, transp, info);

}


/***************************************************************************************
* func          : hifb_setcmap
* description   : CNcomment: 设置调色板 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
static HI_S32 hifb_setcmap(struct fb_cmap *cmap, struct fb_info *info)
{
    HI_S32 i = 0, start = 0;
    unsigned short *red, *green, *blue, *transp;
    unsigned short hred, hgreen, hblue, htransp = 0xffff;
    HIFB_PAR_S *par = (HIFB_PAR_S *)info->par;

    if (par->stBaseInfo.u32LayerID == HIFB_LAYER_CURSOR)
    {
        return -EINVAL;
    }

    if (!gs_pstCapacity[par->stBaseInfo.u32LayerID].bCmap)
    {
        /* AE6D03519, delete this color map warning! */
        HIFB_INFO("Layer%d is not support color map!\n", par->stBaseInfo.u32LayerID);
        return -EPERM;
    }

    if (cmap->len > 256)
    return -EINVAL;

    red    = cmap->red;
    green  = cmap->green;
    blue   = cmap->blue;
    transp = cmap->transp;
    start  = cmap->start;

    for (i = 0; i < cmap->len; i++)
    {
        hred   = *red++;
        hgreen = *green++;
        hblue  = *blue++;
        htransp = (transp != NULL)?*transp++:0xffff;
        _setcolreg(start++, hred, hgreen, hblue, htransp, info);
    }

    return 0;
}

#ifdef CFG_HIFB_SUPPORT_CONSOLE
static HI_VOID hifb_fillrect(struct fb_info *p, const struct fb_fillrect *rect)
{
    cfb_fillrect(p, rect);
}
static HI_VOID hifb_copyarea(struct fb_info *p, const struct fb_copyarea *area)
{
    cfb_copyarea(p, area);
}
static HI_VOID hifb_imageblit(struct fb_info *p, const struct fb_image *image)
{
    cfb_imageblit(p, image);
}
#endif

#ifdef CONFIG_DMA_SHARED_BUFFER
static struct dma_buf * hifb_dmabuf_export(struct fb_info *info)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
     HIFB_PAR_S *pstPar = (HIFB_PAR_S *)info->par;
     switch(pstPar->stBaseInfo.u32LayerID){
         case 0:
     #ifdef CONFIG_GFX_HI_FB0_SMMU_SUPPORT
            HIFB_ERROR("export dmabuf failure, should set smmu mem support through make menuconfig\n");
            return NULL;
     #else
            return hifb_memblock_export(pstPar->u32SmemStartPhy, info->fix.smem_len, 0);
     #endif
         case 1:
     #ifdef CONFIG_GFX_HI_FB1_SMMU_SUPPORT
            HIFB_ERROR("export dmabuf failure, should set smmu mem support through make menuconfig\n");
            return NULL;
     #else
            return hifb_memblock_export(pstPar->u32SmemStartPhy, info->fix.smem_len, 0);
     #endif
         case 2:
     #ifdef CONFIG_GFX_HI_FB2_SMMU_SUPPORT
            HIFB_ERROR("export dmabuf failure, should set smmu mem support through make menuconfig\n");
            return NULL;
     #else
            return hifb_memblock_export(pstPar->u32SmemStartPhy, info->fix.smem_len, 0);
     #endif
         default:
            HIFB_ERROR("export dmabuf failure, should set smmu mem support through make menuconfig\n");
            return NULL;
     }
#else
     return hifb_memblock_export(info->fix.smem_start, info->fix.smem_len, 0);
#endif
}
#endif


static HI_S32 DRV_HIFB_Open(struct fb_info *info, HI_S32 user)
{
    HI_S32 HifbDevOpenCnt = 0;
    HI_S32 s32Ret   = HI_SUCCESS;
    HIFB_PAR_S *par = NULL;

    if ((NULL == info) || (NULL == info->par))
    {
       return HI_FAILURE;
    }

    par = (HIFB_PAR_S *)info->par;
    HIFB_CHECK_LAYERID_SUPPORT(par->stBaseInfo.u32LayerID);

    HifbDevOpenCnt = atomic_read(&par->stBaseInfo.ref_count);
    atomic_inc(&par->stBaseInfo.ref_count);

    if (HI_TRUE != gs_pstCapacity[par->stBaseInfo.u32LayerID].bLayerSupported)
    {
        HIFB_ERROR("the fb%d layer is not supported!\n", par->stBaseInfo.u32LayerID);
        return HI_FAILURE;
    }

    if (0 != HifbDevOpenCnt)
    {
        return HI_SUCCESS;
    }

    s32Ret = g_stGfx2dCallBackFunction.HIFB_DRV_TdeOpen();
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_INFO("tde was not avaliable!\n");
        return HI_FAILURE;
    }

#ifdef CFG_HIFB_LOGO_SUPPORT
    DRV_HIFB_GetLogoData(par->stBaseInfo.u32LayerID);
#endif

    s32Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_OpenLayer(par->stBaseInfo.u32LayerID);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("failed to open layer%d !\n", par->stBaseInfo.u32LayerID);
        return s32Ret;
    }

#ifdef CFG_HIFB_FENCE_SUPPORT
    s32Ret = DRV_HIFB_FenceInit(par);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("DRV_HIFB_FenceInit failure\n");
        return s32Ret;
    }
    par->bEndHwcRefresh = HI_FALSE;
    par->u8RefreshCount = 0;
    par->bHwcRefreshWithWorkQueue = HI_TRUE;
#endif

    s32Ret = DRV_HIFB_InitLayerInfo(par->stBaseInfo.u32LayerID);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_ERROR("hifb layer init failed\n");
        return HI_FAILURE;
    }

#ifdef CFG_HIFB_LOGO_SUPPORT
    DRV_HIFB_GetBaseData(par->stBaseInfo.u32LayerID);
#endif

    s32Ret = DRV_HIFB_GetLayerBufferInfo(par->stBaseInfo.u32LayerID);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_INFO("DRV_HIFB_GetLayerBufferInfo failed\n");
        return HI_FAILURE;
    }
    par->stRunInfo.u32ScreenAddr  = info->fix.smem_start;
    par->st3DInfo.u32rightEyeAddr = par->stRunInfo.u32ScreenAddr;
    par->stRunInfo.u32IndexForInt = 0;
    DRV_HIFB_SetDisplayBuffer(par->stBaseInfo.u32LayerID);

#ifdef CFG_HIFB_LOGO_SUPPORT
    DRV_HIFB_LogoToApp(par->stBaseInfo.u32LayerID);
#endif
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerStride (par->stBaseInfo.u32LayerID, info->fix.line_length);
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr   (par->stBaseInfo.u32LayerID, info->fix.smem_start);

    s32Ret = DRV_HIFB_RegisterCallBackFunction(par->stBaseInfo.u32LayerID);
    if (HI_SUCCESS != s32Ret)
    {
        HIFB_INFO("register callback function failed\n");
        return HI_FAILURE;
    }

#ifdef CFG_HIFB_PROC_SUPPORT
    DRV_HIFB_CreateProc(par->stBaseInfo.u32LayerID);
#endif

    par->stExtendInfo.bShow = HI_TRUE;
    par->bVblank            = HI_TRUE;

    g_stDrvAdpCallBackFunction.HIFB_DRV_EnableLayer(par->stBaseInfo.u32LayerID, HI_TRUE);

#ifdef CFG_HIFB_LOGO_SUPPORT
    DRV_HIFB_CloseLogo(par->stBaseInfo.u32LayerID);
#endif

    return HI_SUCCESS;
}


static HI_S32 DRV_HIFB_InitLayerInfo(HI_U32 u32LayerID)
{

    struct fb_info *info        = NULL;
    HIFB_PAR_S *par             = NULL;
    HIFB_COLOR_FMT_E enColorFmt = HIFB_FMT_BUTT;
    HIFB_RECT stInRect   = {0};
    HI_U32 u32PixDepth   = 0;
    HI_U32 u32Stride     = 0;
    HI_U32 CmpStride     = 0;

    info = s_stLayer[u32LayerID].pstInfo;
    par  = (HIFB_PAR_S *)(info->par);

    if (IS_HD_LAYER(u32LayerID))
    {
        info->var = s_stDefVar[HIFB_LAYER_TYPE_HD];
    }
    else if (IS_SD_LAYER(u32LayerID))
    {
        info->var = s_stDefVar[HIFB_LAYER_TYPE_SD];
    }
    else if(IS_AD_LAYER(u32LayerID) || IS_MINOR_HD_LAYER(u32LayerID) || IS_MINOR_SD_LAYER(u32LayerID))
    {
        info->var = s_stDefVar[HIFB_LAYER_TYPE_AD];
    }
    else
    {
        return HI_FAILURE;
    }

    HI_GFX_Memset(&(par->stDispInfo.stUserBuffer), 0, sizeof(HIFB_BUFFER_S));
    HI_GFX_Memset(&(par->stDispInfo.stCanvasSur),  0, sizeof(HIFB_SURFACE_S));
    HI_GFX_Memset(&(par->stExtendInfo.stCkey),     0, sizeof(HIFB_COLORKEYEX_S));
    HI_GFX_Memset(&par->stExtendInfo.stPos,        0, sizeof(HIFB_POINT_S));

    par->stBaseInfo.bNeedAntiflicker = HI_FALSE;
    hifb_disp_setantiflickerlevel(par->stBaseInfo.u32LayerID, HIFB_LAYER_ANTIFLICKER_AUTO);

    par->stRunInfo.bModifying               = HI_FALSE;
    par->stRunInfo.u32ParamModifyMask       = 0;
    par->stExtendInfo.stAlpha.bAlphaEnable  = HI_TRUE;
    par->stExtendInfo.stAlpha.bAlphaChannel = HI_FALSE;
    par->stExtendInfo.stAlpha.u8Alpha0      = HIFB_ALPHA_TRANSPARENT;
    par->stExtendInfo.stAlpha.u8Alpha1      = HIFB_ALPHA_OPAQUE;
    par->stExtendInfo.stAlpha.u8GlobalAlpha = HIFB_ALPHA_OPAQUE;
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAlpha(par->stBaseInfo.u32LayerID, &par->stExtendInfo.stAlpha);


    par->stExtendInfo.stCkey.u8RedMask   = 0xff;
    par->stExtendInfo.stCkey.u8GreenMask = 0xff;
    par->stExtendInfo.stCkey.u8BlueMask  = 0xff;
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerKeyMask(par->stBaseInfo.u32LayerID, &par->stExtendInfo.stCkey);

    enColorFmt = hifb_getfmtbyargb(&info->var.red, &info->var.green, &info->var.blue, &info->var.transp, info->var.bits_per_pixel);
    par->stExtendInfo.enColFmt = enColorFmt;
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerDataFmt(par->stBaseInfo.u32LayerID, par->stExtendInfo.enColFmt);

    u32PixDepth = info->var.bits_per_pixel >> 3;
    u32Stride = CONIFG_HIFB_GetMaxStride(info->var.xres_virtual, u32PixDepth, &CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
    info->fix.line_length = u32Stride;

    par->stExtendInfo.u32DisplayWidth           = info->var.xres;
    par->stExtendInfo.u32DisplayHeight          = info->var.yres;

    par->st3DInfo.st3DSurface.u32Pitch          = info->fix.line_length;
    par->st3DInfo.st3DSurface.enFmt             = par->stExtendInfo.enColFmt;
    par->st3DInfo.st3DSurface.u32Width          = info->var.xres;
    par->st3DInfo.st3DSurface.u32Height         = info->var.yres;
    par->st3DInfo.st3DMemInfo.u32StereoMemLen   = HI_NULL;
    par->st3DInfo.st3DMemInfo.u32StereoMemStart = HI_NULL;

    stInRect.x = 0;
    stInRect.y = 0;
    stInRect.w = info->var.xres;
    stInRect.h = info->var.yres;
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerInRect(par->stBaseInfo.u32LayerID, &stInRect);
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerStride(par->stBaseInfo.u32LayerID, info->fix.line_length);

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimMode(par->stBaseInfo.u32LayerID, HIFB_STEREO_MONO, HIFB_STEREO_MONO);
#endif

    par->stExtendInfo.enBufMode = HIFB_REFRESH_MODE_WITH_PANDISPLAY;
    par->stRunInfo.u32BufNum    = CONFIG_HIFB_LAYER_BUFFER_MAX_NUM;
    par->stRunInfo.StereoBufNum = CONFIG_HIFB_STEREO_BUFFER_MAX_NUM;

    par->bPanFlag  = HI_FALSE;
    spin_lock_init(&par->stBaseInfo.lock);
    spin_lock_init(&par->st3DInfo.stStereoLock);

    return HI_SUCCESS;
}


static HI_S32 DRV_HIFB_GetLayerBufferInfo(HI_U32 u32LayerID)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_U32 PixDepth  = 0;
    HI_U32 MemStride = 0;
    HI_U32 CmpStride     = 0;
    HI_U32 LayerBufferNewSize = 0;
    HI_U32 LayerBufferOldSize = 0;
    struct fb_info *info = s_stLayer[u32LayerID].pstInfo;

    PixDepth = info->var.bits_per_pixel >> 3;
    MemStride = CONIFG_HIFB_GetMaxStride(info->var.xres_virtual,PixDepth,&CmpStride,CONFIG_HIFB_STRIDE_16ALIGN);
    LayerBufferNewSize = HI_HIFB_GetMemSize(MemStride,info->var.yres_virtual);
    LayerBufferOldSize = info->fix.smem_len;

    if (LayerBufferOldSize >= LayerBufferNewSize)
    {
       return HI_SUCCESS;
    }

    Ret = DRV_HIFB_ReAllocLayerBuffer(u32LayerID,LayerBufferNewSize);
    if (HI_SUCCESS != Ret)
    {
        HIFB_ERROR("Failed to ReAlloc mem\n");
        return HI_FAILURE;
    }

    info->fix.line_length = MemStride;
    info->fix.smem_len    = LayerBufferNewSize;

    return HI_SUCCESS;
}


static HI_VOID DRV_HIFB_SetDisplayBuffer(HI_U32 u32LayerId)
{

    struct fb_info *info = s_stLayer[u32LayerId].pstInfo;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)(info->par);
    HI_U32 OneBufSize = 0;
    HI_U32 BufferNum  = 0;

    OneBufSize = HI_HIFB_GetMemSize(info->fix.line_length,info->var.yres);
    if (0 == OneBufSize){
       return;
    }

    BufferNum = info->fix.smem_len / OneBufSize;
    if (CONFIG_HIFB_LAYER_0BUFFER == BufferNum){
       return;
    }else if (CONFIG_HIFB_LAYER_1BUFFER == BufferNum){
       pstPar->stDispInfo.u32DisplayAddr[0] = info->fix.smem_start;
       pstPar->stDispInfo.u32DisplayAddr[1] = info->fix.smem_start;
    }else{
       pstPar->stDispInfo.u32DisplayAddr[0] = info->fix.smem_start;
       pstPar->stDispInfo.u32DisplayAddr[1] = info->fix.smem_start + OneBufSize;
    }
    return;
}


static inline HI_S32 DRV_HIFB_RegisterCallBackFunction(HI_U32 u32LayerId)
{
    HI_S32 Ret = HI_SUCCESS;

    Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TYPE_VO, (IntCallBack)hifb_vo_callback, u32LayerId);
    if (HI_SUCCESS != Ret)
    {
        HIFB_ERROR("failed to set vo callback function, open layer%d failure\n", u32LayerId);
        return HI_FAILURE;
    }

#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
    Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TYPE_3DMode_CHG, (IntCallBack)DRV_OSR_StereoCallBack, u32LayerId);
    if (HI_SUCCESS != Ret)
    {
        HIFB_ERROR("failed to set stereo mode change callback function, open layer%d failure\n", u32LayerId);
        return HI_FAILURE;
    }
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
    Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_SetIntCallback(HIFB_CALLBACK_TYPE_FRAME_END, (IntCallBack)DRV_HIFB_FENCE_FrameEndCallBack, u32LayerId);
    if (HI_SUCCESS != Ret)
    {
        HIFB_ERROR("failed to set frame end callback function, open layer%d failure\n", u32LayerId);
        return HI_FAILURE;
    }
#endif

   return HI_SUCCESS;
}


HI_U32 DRV_HIFB_GetPixDepth(HI_U32 BitsPerPixel)
{
     HI_U32 PerPixDepth = (BitsPerPixel >= 8) ? (BitsPerPixel >> 3) : (BitsPerPixel / 8);
     return PerPixDepth;
}


HI_VOID DRV_HIFB_WaitVBlank(HIFB_LAYER_ID_E enLayerId)
{
    g_stDrvAdpCallBackFunction.HIFB_DRV_WaitVBlank(enLayerId);
    return;
}


HI_S32 DRV_HIFB_SteroDataUpdate(HIFB_LAYER_ID_E enLayerId, HIFB_BUFFER_S *pstSrcBuffer, HIFB_BLIT_OPT_S *pstBlitOpt)
{
#ifdef CFG_HIFB_STEREO3D_HW_SUPPORT

    struct fb_info *info = s_stLayer[enLayerId].pstInfo;
    HIFB_PAR_S *pstPar   = (HIFB_PAR_S *)(info->par);
    HI_S32 Ret = HI_SUCCESS;
    HI_ULONG LockFlag = 0;
    HIFB_BUFFER_S stDstBuf;

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&LockFlag);
      pstPar->stRunInfo.bNeedFlip        = HI_FALSE;
      pstPar->stRunInfo.s32RefreshHandle = 0;
    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockFlag);

    HI_GFX_Memcpy(&stDstBuf.stCanvas, &pstPar->st3DInfo.st3DSurface, sizeof(HIFB_SURFACE_S));

    stDstBuf.stCanvas.u32Width  = (HIFB_STEREO_SIDEBYSIDE_HALF == pstPar->st3DInfo.StereoMode) ? (stDstBuf.stCanvas.u32Width >> 1) : (stDstBuf.stCanvas.u32Width);
    stDstBuf.stCanvas.u32Height = (HIFB_STEREO_TOPANDBOTTOM == pstPar->st3DInfo.StereoMode) ? (stDstBuf.stCanvas.u32Height >> 1) : (stDstBuf.stCanvas.u32Height);

    stDstBuf.UpdateRect.x = 0;
    stDstBuf.UpdateRect.y = 0;
    stDstBuf.UpdateRect.w = stDstBuf.stCanvas.u32Width;
    stDstBuf.UpdateRect.h = stDstBuf.stCanvas.u32Height;

    Ret = g_stGfx2dCallBackFunction.HIFB_DRV_Blit(pstSrcBuffer, &stDstBuf, pstBlitOpt, HI_TRUE);
    if (Ret <= 0)
    {
        HIFB_ERROR("tde blit error!\n");
        return HI_FAILURE;
    }

    pstPar->stRunInfo.bModifying          = HI_TRUE;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_STRIDE;
    pstPar->stRunInfo.u32ParamModifyMask |= HIFB_LAYER_PARAMODIFY_REFRESH;
    pstPar->stRunInfo.bModifying          = HI_FALSE;

    pstPar->stRunInfo.s32RefreshHandle = Ret;
#else
    UNUSED(enLayerId);
    UNUSED(pstSrcBuffer);
    UNUSED(pstBlitOpt);
#endif
    return HI_SUCCESS;
}


HI_S32 DRV_HIFB_CallBack(HI_VOID *pParaml, HI_VOID *pParamr)
{
    HI_U32 u32LayerId  = *(HI_U32 *)pParaml;
    HIFB_PAR_S *pstPar = (HIFB_PAR_S *)(s_stLayer[u32LayerId].pstInfo->par);

#if defined(CFG_HIFB_FENCE_SUPPORT) && defined(CFG_HIFB_STEREO3D_HW_SUPPORT)
    HI_U32 u32Index = 0;
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
    #ifdef CFG_HIFB_STEREO3D_HW_SUPPORT
        if (HI_FALSE == pstPar->st3DInfo.IsStereo)
        {
            return HI_SUCCESS;
        }

        u32Index = pstPar->stRunInfo.u32IndexForInt;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(u32LayerId, pstPar->st3DInfo.u32DisplayAddr[u32Index]);
        pstPar->stRunInfo.u32ScreenAddr  = pstPar->st3DInfo.u32DisplayAddr[u32Index];
        pstPar->st3DInfo.u32rightEyeAddr = pstPar->stRunInfo.u32ScreenAddr;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTriDimAddr(u32LayerId, pstPar->st3DInfo.u32rightEyeAddr);
        g_stDrvAdpCallBackFunction.HIFB_DRV_UpdataLayerReg(pstPar->stBaseInfo.u32LayerID);
        pstPar->stRunInfo.u32IndexForInt = (++u32Index) % pstPar->stRunInfo.StereoBufNum;
    #endif
#else
    pstPar->stRunInfo.bNeedFlip = HI_TRUE;
#endif

    return HI_SUCCESS;
}


HI_S32 HIFB_DRV_ModInit(HI_VOID)
{

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32LayerId = HIFB_LAYER_HD_0;

    HI_GFX_Memset(&s_stLayer, 0x0, sizeof(s_stLayer));

    DRV_HIFB_GetAdpCallBackFunction   (&g_stDrvAdpCallBackFunction);
    DRV_HIFB_GetGfx2dCallBackFunction (&g_stGfx2dCallBackFunction);


    s32Ret = g_stDrvAdpCallBackFunction.HIFB_DRV_GfxInit();
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    g_stDrvAdpCallBackFunction.HIFB_DRV_GetGFXCap(&gs_pstCapacity);

    if (!strncmp("on", tc_wbc, 2))
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetTCFlag(HI_TRUE);
    }

    s32Ret = DRV_HIFB_ParseInsmodParameter();
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    for (u32LayerId = HIFB_LAYER_HD_0; u32LayerId <= HIFB_LAYER_SD_1; u32LayerId++)
    {
        s32Ret = DRV_HIFB_RegisterFrameBuffer(u32LayerId);
        if (HI_SUCCESS != s32Ret)
        {
            goto ERR_EXIT;
        }

        s32Ret = DRV_HIFB_AllocLayerBuffer(u32LayerId,s_stLayer[u32LayerId].u32LayerSize);
        if (HI_SUCCESS != s32Ret)
        {
            continue;
        }

        s32Ret = DRV_HIFB_InitAllocCmapBuffer(u32LayerId);
        if (HI_SUCCESS != s32Ret)
        {
            continue;
        }

        #ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
            HI_GFX_Memset(&s_stTextLayer[u32LayerId], 0, sizeof(HIFB_SCROLLTEXT_INFO_S));
        #endif
    }

#ifndef HI_MCE_SUPPORT
    DRV_HIFB_ModInit_K();
#endif

#ifdef CFG_HIFB_LOGO_SUPPORT
    DRV_HIFB_ResetLogoState();
    DRV_HIFB_WhetherHasLogo();
#endif

    DRV_HIFB_PrintVersion();

    return HI_SUCCESS;

ERR_EXIT:

    HIFB_DRV_ModExit();

    return HI_FAILURE;

}


HI_VOID HIFB_DRV_ModExit(HI_VOID)
{
    HI_U32 u32LayerId    = HIFB_LAYER_HD_0;
#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
    HI_U32 ScrollTextNum = 0;
#endif

    g_stGfx2dCallBackFunction.HIFB_DRV_SetTdeCallBack(NULL);

    g_stDrvAdpCallBackFunction.HIFB_DRV_GfxDeInit();

    for(u32LayerId = HIFB_LAYER_HD_0; u32LayerId <= HIFB_LAYER_SD_1; u32LayerId++)
    {
        DRV_HIFB_DInitAllocCmapBuffer  (u32LayerId);
        DRV_HIFB_FreeLayerBuffer       (u32LayerId);
        DRV_HIFB_UnRegisterFrameBuffer (u32LayerId);

        #ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
           if (s_stTextLayer[u32LayerId].bAvailable){
               continue;
           }
           for (ScrollTextNum = 0; ScrollTextNum < SCROLLTEXT_NUM; ScrollTextNum++){
               if (s_stTextLayer[u32LayerId].stScrollText[ScrollTextNum].bAvailable){
                   hifb_freescrolltext_cachebuf(&(s_stTextLayer[u32LayerId].stScrollText[ScrollTextNum]));
                   HI_GFX_Memset(&s_stTextLayer[u32LayerId].stScrollText[ScrollTextNum],0,sizeof(HIFB_SCROLLTEXT_S));
               }
           }
           s_stTextLayer[u32LayerId].bAvailable = HI_FALSE;
           s_stTextLayer[u32LayerId].u32textnum = 0;
           s_stTextLayer[u32LayerId].u32ScrollTextId = 0;
           flush_work(&s_stTextLayer[u32LayerId].blitScrollTextWork);
        #endif
    }
    g_stGfx2dCallBackFunction.HIFB_DRV_TdeClose();

#ifndef HI_MCE_SUPPORT
    DRV_HIFB_ModExit_K();
#endif

    return;
}


static HI_S32 DRV_HIFB_ParseInsmodParameter(HI_VOID)
{

    HI_CHAR *pInputStr     = NULL;
    HI_CHAR number[4]   = {0};
    HI_U32 LayerCnt     = 0;
    HI_U32 u32LayerId   = 0;

    if (video == HI_NULL)
    {
       for (u32LayerId = HIFB_LAYER_HD_0; u32LayerId <= HIFB_LAYER_SD_1; u32LayerId++)
       {
          s_stLayer[u32LayerId].u32LayerSize = HI_HIFB_GetMemSize(gs_u32MenuconfigLayerSize[u32LayerId] * 1024,1);
       }
       return HI_SUCCESS;
    }

    pInputStr = strstr(video, "vram");
    while (pInputStr != NULL)
    {
        LayerCnt   = 0;
        pInputStr += 4;
        while (*pInputStr != '_')
        {
            if (LayerCnt > 1)
            {
                HIFB_ERROR("layer id is out of range!\n");
                return HI_FAILURE;
            }
            number[LayerCnt] = *pInputStr;
            LayerCnt++;
            pInputStr++;
        }
        number[LayerCnt] = '\0';

        u32LayerId = simple_strtoul(number, (char **)NULL, 10);
        if (u32LayerId > HIFB_MAX_LAYER_ID)
        {
            HIFB_ERROR("layer id is out of range!\n");
            return HI_FAILURE;
        }

        if ( (HI_FALSE == gs_pstCapacity[u32LayerId].bLayerSupported) && (u32LayerId != HIFB_LAYER_CURSOR))
        {
            HIFB_ERROR("chip doesn't support layer %d!\n", u32LayerId);
            return HI_FAILURE;
        }

        pInputStr   += sizeof("size") + LayerCnt;
        s_stLayer[u32LayerId].u32LayerSize = DRV_HIFB_GetVramSize(pInputStr);

        pInputStr = strstr(pInputStr, "vram");
    }

    return HI_SUCCESS;
}

static inline unsigned long DRV_HIFB_GetVramSize(HI_CHAR* pstr)
{

    HI_BOOL str_is_valid = HI_TRUE;
    unsigned long vram_size = 0;
    HI_CHAR* ptr = pstr;

    if ((ptr == NULL) || (*ptr == '\0'))
    {
        return 0;
    }

    while (*ptr != '\0')
    {
        if (*ptr == ',')
        {
            break;
        }
        else if ((!isdigit(*ptr)) && ('X' != *ptr) && ('x' != *ptr) && ((*ptr > 'f' && *ptr <= 'z') || (*ptr > 'F' && *ptr <= 'Z')))
        {
            str_is_valid = HI_FALSE;
            break;
        }
        ptr++;
    }

    if (HI_TRUE == str_is_valid)
    {
        vram_size = simple_strtoul(pstr, (char **)NULL, 0);
        vram_size = ((vram_size * 1024 + PAGE_SIZE - 1) & PAGE_MASK)/1024;
    }

    vram_size = HI_HIFB_GetMemSize(vram_size * 1024,1);

    return vram_size;
}


static inline HI_S32 DRV_HIFB_RegisterFrameBuffer(HI_U32 u32LayerId)
{
    HI_S32 s32Ret = 0;
    HIFB_PAR_S *pstPar = NULL;
    struct fb_info *FbInfo = NULL;

    if (u32LayerId > HIFB_LAYER_SD_2)
    {
       return HI_SUCCESS;
    }

    FbInfo = framebuffer_alloc( ( (sizeof(HIFB_PAR_S)) + (sizeof(HI_U32) * 256) ), NULL);
    if (NULL == FbInfo)
    {
        HIFB_ERROR("failed to malloc the fb_info!\n");
        return -ENOMEM;
    }

    pstPar = (HIFB_PAR_S *)(FbInfo->par);
    s_stLayer[u32LayerId].pstInfo = FbInfo;

    FbInfo->fix    = gs_stDefFix;
    FbInfo->fbops  = &s_sthifbops;
    FbInfo->flags  = FBINFO_FLAG_DEFAULT | FBINFO_HWACCEL_YPAN | FBINFO_HWACCEL_XPAN;
    FbInfo->pseudo_palette = (HI_U8*)(FbInfo->par) + sizeof(HIFB_PAR_S);
    FbInfo->fix.smem_len   = s_stLayer[u32LayerId].u32LayerSize;

    s32Ret = register_framebuffer(FbInfo);
    if (s32Ret < 0)
    {
        HIFB_ERROR("failed to register_framebuffer!\n");
        return HI_FAILURE;
    }

    pstPar->bFrameBufferRegister = HI_TRUE;

#ifdef CFG_HIFB_FENCE_SUPPORT
    init_waitqueue_head(&pstPar->WaiteEndHwcRefresh);
#endif

    return HI_SUCCESS;
}


static inline HI_VOID DRV_HIFB_UnRegisterFrameBuffer(HI_U32 u32LayerId)
{
    HIFB_PAR_S *pstPar   = NULL;
    struct fb_info* FbInfo = NULL;

    FbInfo = s_stLayer[u32LayerId].pstInfo;
    if (NULL == FbInfo){
        return;
    }

    pstPar = (HIFB_PAR_S *)FbInfo->par;
    if (NULL == pstPar){
       return;
    }

    if (HI_TRUE == pstPar->bFrameBufferRegister){
        unregister_framebuffer(FbInfo);
    }

    framebuffer_release(FbInfo);

    s_stLayer[u32LayerId].pstInfo = NULL;

    return;
}

static inline HI_S32 DRV_HIFB_ReAllocLayerBuffer(HI_U32 u32LayerId, HI_U32 BufSize)
{
    HI_S32 Ret = HI_SUCCESS;
    DRV_HIFB_FreeLayerBuffer(u32LayerId);
    Ret = DRV_HIFB_AllocLayerBuffer(u32LayerId,BufSize);
    return Ret;
}

static inline HI_S32 DRV_HIFB_AllocLayerBuffer(HI_U32 u32LayerId, HI_U32 BufSize)
{

    HI_CHAR LayerBufferName[256] = {'\0'};
    HIFB_PAR_S *pstPar     = NULL;
    struct fb_info *FbInfo = NULL;

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_BOOL bSmmu = HI_FALSE;
#endif

    FbInfo = s_stLayer[u32LayerId].pstInfo;
    if (NULL == FbInfo)
    {
       return HI_FAILURE;
    }

    pstPar = (HIFB_PAR_S *)FbInfo->par;
    if (NULL == pstPar)
    {
       return HI_FAILURE;
    }

    pstPar->u32SmemStartPhy        = 0;
    pstPar->stBaseInfo.u32LayerID  = u32LayerId;
    pstPar->stDispInfo.stCanvasSur.u32PhyAddr = 0;

    snprintf(LayerBufferName, sizeof(LayerBufferName), "HIFB_Fb%d", u32LayerId);
    LayerBufferName[sizeof(LayerBufferName) -1] = '\0';

#ifdef CONFIG_GFX_MMU_SUPPORT
    bSmmu = DRV_HIFB_CheckSmmuSupport(u32LayerId);
    if (HI_FALSE == bSmmu)
    {
        pstPar->u32SmemStartPhy = hifb_buf_allocmem(LayerBufferName, NULL, BufSize);
        FbInfo->fix.smem_start  = hifb_buf_map2smmu(pstPar->u32SmemStartPhy);
    }
    else
#endif
    {
        pstPar->u32SmemStartPhy = 0;
        FbInfo->fix.smem_start  = hifb_buf_allocmem(LayerBufferName, "iommu", BufSize);
    }

    if (0 == FbInfo->fix.smem_start)
    {
       return HI_FAILURE;
    }

    FbInfo->screen_base = hifb_buf_map(FbInfo->fix.smem_start);
    if (HI_NULL == FbInfo->screen_base)
    {
       return HI_FAILURE;
    }
    HI_GFX_Memset(FbInfo->screen_base, 0x0, BufSize);

    return HI_SUCCESS;
}


static inline HI_VOID DRV_HIFB_FreeLayerBuffer(HI_U32 u32LayerId)
{
    struct fb_info* FbInfo = NULL;
    HIFB_PAR_S *pstPar     = NULL;

    FbInfo = s_stLayer[u32LayerId].pstInfo;
    if (NULL == FbInfo){
         return;
    }

    pstPar = (HIFB_PAR_S *)FbInfo->par;
    if (NULL == pstPar){
         return;
    }

    if (NULL != FbInfo->screen_base){
        hifb_buf_ummap(FbInfo->screen_base);
        FbInfo->screen_base = NULL;
    }

    if (0 == FbInfo->fix.smem_start){
       return;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    if (0 != pstPar->u32SmemStartPhy){
        hifb_buf_unmapsmmu(FbInfo->fix.smem_start);
        hifb_buf_freemem(pstPar->u32SmemStartPhy,HI_FALSE);
     }else{
        hifb_buf_freemem(FbInfo->fix.smem_start,HI_TRUE);
     }
#else
     hifb_buf_freemem(FbInfo->fix.smem_start,HI_FALSE);
#endif

     pstPar->u32SmemStartPhy = 0;
     FbInfo->fix.smem_start  = 0;

     return;
}


static inline HI_S32 DRV_HIFB_InitAllocCmapBuffer(HI_U32 u32LayerId)
{
    HI_U32 CmapLen = 256;
    struct fb_info* FbInfo = NULL;

    FbInfo = s_stLayer[u32LayerId].pstInfo;
    if (NULL == FbInfo){
         return HI_FAILURE;
    }

    if (HI_TRUE != gs_pstCapacity[u32LayerId].bCmap){
         return HI_SUCCESS;
    }

    if (fb_alloc_cmap(&FbInfo->cmap, CmapLen, 1) < 0){
        HIFB_WARNING("fb_alloc_cmap failed!\n");
        FbInfo->cmap.len = 0;
        return HI_FAILURE;
    }

    FbInfo->cmap.len = CmapLen;

    return HI_SUCCESS;
}


static inline HI_VOID DRV_HIFB_DInitAllocCmapBuffer(HI_U32 u32LayerId)
{
    struct fb_cmap *cmap   = NULL;
    struct fb_info *FbInfo = NULL;

    FbInfo = s_stLayer[u32LayerId].pstInfo;
    if (NULL == FbInfo){
         return;
    }

    cmap = &FbInfo->cmap;
    if (0 != cmap->len){
        fb_dealloc_cmap(cmap);
    }

    return;
}


static inline HI_BOOL DRV_HIFB_CheckSmmuSupport(HI_U32 LayerId)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
      switch(LayerId){
          case HIFB_LAYER_HD_0:
          {
             #ifdef CONFIG_GFX_HI_FB0_SMMU_SUPPORT
               return HI_TRUE;
             #else
               return HI_FALSE;
             #endif
          }
          case HIFB_LAYER_HD_1:
          {
             #ifdef CONFIG_GFX_HI_FB1_SMMU_SUPPORT
                return HI_TRUE;
             #else
                return HI_FALSE;
             #endif
          }
          case HIFB_LAYER_HD_2:
          {
             #ifdef CONFIG_GFX_HI_FB2_SMMU_SUPPORT
                return HI_TRUE;
             #else
                return HI_FALSE;
             #endif
          }
          default:
          {
             return HI_FALSE;
          }
      }
 #else
      return HI_FALSE;
 #endif
}


static inline HI_VOID DRV_HIFB_PrintVersion(HI_VOID)
{
#if !defined(CONFIG_GFX_COMM_VERSION_DISABLE) && !defined(CONFIG_GFX_COMM_DEBUG_DISABLE) && defined(MODULE)
     GFX_Printk("Load hi_fb.ko success.\t\t(%s)\n", VERSION_STRING);
#endif
     return;
}

HI_VOID DRV_HIFB_SetDecmpLayerInfo(HI_U32 LayerId)
{
#ifdef CFG_HIFB_FENCE_SUPPORT
    HI_BOOL bDispEnable = HI_FALSE;
#endif
    HI_U32 IsFenceRefresh = 0x0;
    HI_ULONG LockParFlag = 0;
    HI_U32 CmpStride   = 0;
    HI_U32 UnCmpStride = 0;
    HIFB_PAR_S *pstPar = NULL;

#if defined(CFG_HI_FB_DECOMPRESS_SUPPORT) && !defined(CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE)
    HI_U32 HeadSize  = 0;
    HI_U32 ARHeadDdr = 0;
    HI_U32 ARDataDdr = 0;
    HI_U32 GBHeadDdr = 0;
    HI_U32 GBDataDdr = 0;
#endif

    if ((NULL == s_stLayer[LayerId].pstInfo) || (NULL == s_stLayer[LayerId].pstInfo->par))
    {
        return;
    }

    pstPar = (HIFB_PAR_S *)s_stLayer[LayerId].pstInfo->par;

    DRV_HIFB_Lock(&pstPar->stBaseInfo.lock,&LockParFlag);


    IsFenceRefresh = (pstPar->stRunInfo.u32ParamModifyMask & HIFB_LAYER_PARAMODIFY_FENCE);
    if (!IsFenceRefresh)
    {
       pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FENCE;
       DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockParFlag);
       return;
    }

    if (HI_TRUE == pstPar->st3DInfo.IsStereo)
    {
        pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FENCE;
        DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockParFlag);
        return;
    }

#ifdef CFG_HIFB_PROC_SUPPORT
    if (HI_TRUE == pstPar->stProcInfo.bHasDeCmpErr)
    {
       pstPar->stProcInfo.bHasDeCmpErr = HI_FALSE;
       pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FENCE;
       DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockParFlag);
       return;
    }
#endif

    UnCmpStride = CONIFG_HIFB_GetMaxStride(pstPar->stExtendInfo.u32DisplayWidth,4,&CmpStride,CONFIG_HIFB_DECOMPRESS_DATA_STRIDE_ALIGN);

#ifdef CFG_HI_FB_DECOMPRESS_SUPPORT
    #ifndef CONFIG_HIFB_CALCSTRIDE_WITH_ONEPOINTTHREE
    if (HI_TRUE == pstPar->bDeCompress)
    {
        DRV_HIFB_MEM_GetCmpHeadInfo(pstPar->stExtendInfo.u32DisplayHeight,&HeadSize,NULL);
        ARHeadDdr = pstPar->stRunInfo.u32ScreenAddr;
        ARDataDdr = ARHeadDdr + HeadSize;
        GBHeadDdr = ARDataDdr + CmpStride * pstPar->stExtendInfo.u32DisplayHeight;
        GBDataDdr = GBHeadDdr + HeadSize;
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetDeCmpDdrInfo(LayerId, ARHeadDdr,ARDataDdr,GBHeadDdr,GBDataDdr,CmpStride);
    }
    else
    #endif
    {
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerStride(LayerId, UnCmpStride);
        g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(LayerId, pstPar->stRunInfo.u32ScreenAddr);
    }
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetDeCmpSwitch (LayerId, pstPar->bDeCompress);
#else
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerStride(LayerId, UnCmpStride);
    g_stDrvAdpCallBackFunction.HIFB_DRV_SetLayerAddr(LayerId, pstPar->stRunInfo.u32ScreenAddr);
#endif

#ifdef CFG_HIFB_FENCE_SUPPORT
    g_stDrvAdpCallBackFunction.HIFB_DRV_GetHaltDispStatus(pstPar->stBaseInfo.u32LayerID, &bDispEnable);
    while (pstPar->u8RefreshCount > 0)
    {
        DRV_HIFB_IncRefreshTime(bDispEnable);
        pstPar->u8RefreshCount--;
    }
#endif

    pstPar->stRunInfo.u32ParamModifyMask &= ~HIFB_LAYER_PARAMODIFY_FENCE;

    DRV_HIFB_UnLock(&pstPar->stBaseInfo.lock,&LockParFlag);

    return;
}


HI_VOID DRV_HIFB_Lock(spinlock_t *pLock, HI_ULONG *pLockParFlag)
{
    spin_lock_irqsave(pLock,*pLockParFlag);
    return;
}

HI_VOID DRV_HIFB_UnLock(spinlock_t *pLock, HI_ULONG *pLockParFlag)
{
    spin_unlock_irqrestore(pLock,*pLockParFlag);
    return;
}

HI_BOOL HI_DRV_HIFB_IfFrameRefresh(HI_VOID)
{
    HIFB_PAR_S *pstPar = s_stLayer[HIFB_LAYER_HD_0].pstInfo->par;

    if (NULL == pstPar){
       return HI_FALSE;
    }

#ifdef CFG_HIFB_FENCE_SUPPORT
    if (HI_TRUE == pstPar->bHwcRefresh){
        return DRV_HIFB_FENCE_IsRefresh();
    }
#endif

    return pstPar->stFrameInfo.bFrameHit;
}

/*****************************************************************************/
/*****************************************************************************/
#define HIFB_DRV_IsFrameHit  HI_DRV_HIFB_IfFrameRefresh
EXPORT_SYMBOL(HIFB_DRV_IsFrameHit);

#ifdef MODULE
module_init(HIFB_DRV_ModInit);
module_exit(HIFB_DRV_ModExit);
MODULE_LICENSE("GPL");
#endif
