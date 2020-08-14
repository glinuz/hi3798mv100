/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_osr.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_OSR_H__
#define __DRV_HIFB_OSR_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include <linux/fb.h>
#include "hifb_drv.h"
#include "drv_hifb_common.h"
#include "drv_hifb_scrolltext.h"
#include "drv_hifb_config.h"
#include "drv_hifb_blit.h"
#include "drv_hifb_proc.h"

/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/


/**
 ** support 32 layers most, the limit is from linux fb
 **/
#define MAX_FB  32

/**
 ** define the value of default set of each layer
 **/
#define HIFB_HD_DEF_WIDTH    1920                     /* unit: pixel            */
#define HIFB_HD_DEF_HEIGHT   1080                     /* unit: pixel            */
#define HIFB_HD_DEF_STRIDE   (HIFB_HD_DEF_WIDTH * 4)  /* unit: byte             */
#define HIFB_HD_DEF_VRAM     8100                     /* unit: KB 1280*720*4*2  */


#define HIFB_SD_DEF_WIDTH    1280
#define HIFB_SD_DEF_HEIGHT   720
#define HIFB_SD_DEF_STRIDE   (HIFB_SD_DEF_WIDTH*4)
#define HIFB_SD_DEF_VRAM     7200                     /* unit: KB 720*576*4*2   */


#define HIFB_AD_DEF_WIDTH    1280
#define HIFB_AD_DEF_HEIGHT   80
#define HIFB_AD_DEF_STRIDE   (HIFB_AD_DEF_WIDTH*4)
#define HIFB_AD_DEF_VRAM     800                      /* unit: KB 1280*80*4*2   */

#define HIFB_CURSOR_DEF_WIDTH    128
#define HIFB_CURSOR_DEF_HEIGHT   128
#define HIFB_CURSOR_DEF_STRIDE   (HIFB_CURSOR_DEF_WIDTH*4)
#define HIFB_CURSOR_DEF_VRAM     128                  /* unit: KB 128*128*4*2   */


#define HIFB_DEF_DEPTH        16
#define HIFB_DEF_XSTART       0
#define HIFB_DEF_YSTART       0
#define HIFB_DEF_ALPHA        0xff
#define HIFB_DEF_PIXEL_FMT    HIFB_FMT_ARGB1555

#define HIFB_IS_CLUTFMT(eFmt)     (HIFB_FMT_1BPP <= (eFmt) && (eFmt) <= HIFB_FMT_ACLUT88)
#define HIFB_ALPHA_OPAQUE         0xff
#define HIFB_ALPHA_TRANSPARENT    0x00

/**
 ** support level 5 deflicker most
 **/
#define HIFB_DEFLICKER_LEVEL_MAX  5

#define HIFB_MAX_LAYER_ID         (HIFB_LAYER_ID_BUTT-1)
#define HIFB_MAX_LAYER_NUM        HIFB_LAYER_ID_BUTT

#define HIFB_MAX_PIXFMT_NUM       28



#define FBIOGET_FSCREENINFO_HIFB           0x4693

#define HIFB_REFRESH_MODE_WITH_PANDISPLAY  HIFB_LAYER_BUF_STANDARD

/*************************** Structure Definition ****************************/

typedef enum
{
    HIFB_ANTIFLICKER_NONE,    /* no antiflicker.If scan mode is progressive, hifb will set antiflicker mode to none */
    HIFB_ANTIFLICKER_TDE,    /* tde antiflicker mode, it's effect for 1buf or 2buf only */
    HIFB_ANTIFLICKER_VO,    /* vo antiflicker mode, need hardware supprot */
    HIFB_ANTIFLICKER_BUTT
} HIFB_LAYER_ANTIFLICKER_MODE_E;


/**
 ** only use in logo transition
 **/
typedef enum
{
    HIFB_STATE_LOGO_IN         = 0x1,
    HIFB_STATE_PUT_VSCREENINFO = 0x2,
    HIFB_STATE_PAN_DISPLAY     = 0x4,
    HIFB_STATE_REFRESH         = 0x8,
    HIFB_STATE_BUTT
}HIFB_STATE_E;

/**
 ** frame info
 **/
typedef struct
{
    HI_U32  u32RefreshFrame;
    HI_U32  u32StartTimeMs;
    HI_U32  u32Fps;
    HI_BOOL bFrameHit;
}HIFB_FRAME_INFO_S;

/**
 ** 3D MEM INFO STRUCT
 **/
typedef struct
{
    HI_U32 u32StereoMemStart;
    HI_U32 u32StereoMemLen;
}HIFB_3DMEM_INFO_S;


/**
 ** 3D PAR INFO STRUCT
 **/
typedef struct
{
    HI_BOOL                IsStereo;
    HI_BOOL                IsStereoCallBack;
    HI_S32                 s32StereoDepth;
    HI_U32                 u32rightEyeAddr;
    HI_U32                 u32DisplayAddr[CONFIG_HIFB_LAYER_BUFFER_MAX_NUM];
    spinlock_t             stStereoLock;
    HIFB_STEREO_MODE_E     StereoMode;
    HIFB_STEREO_MODE_E     DataStereoMode;
    HIFB_RECT              st3DUpdateRect;
    HIFB_SURFACE_S         st3DSurface;
    HIFB_3DMEM_INFO_S      st3DMemInfo;
}HIFB_3D_PAR_S;


typedef struct
{
    HIFB_RECT       stCmpRect;
    HIFB_RECT       stUpdateRect;
    HI_U32          u32DisplayAddr[CONFIG_HIFB_LAYER_BUFFER_MAX_NUM];
    HIFB_SURFACE_S  stCanvasSur;
    HIFB_BUFFER_S   stUserBuffer;
}HIFB_DISP_INFO_S;


/** 运行相关的 **/
typedef struct
{
    HI_BOOL   bModifying;
    HI_U32    u32ParamModifyMask;
    HI_BOOL   bNeedFlip;
    HI_BOOL   bFliped;
    HI_U32    u32IndexForInt;
    HI_U32    u32BufNum;
    HI_U32    StereoBufNum;
    HI_U32    u32ScreenAddr;
    HI_S32    s32RefreshHandle;
}HIFB_RTIME_INFO_S;


/** 显示相关的 **/
typedef struct
{
    HI_BOOL                         bShow;               /* show status */
    HIFB_COLOR_FMT_E                enColFmt;            /* color format */
    HIFB_LAYER_BUF_E                enBufMode;           /* refresh mode*/
    HI_U32                          u32DisplayWidth;     /* width  of layer's display buffer*/
    HI_U32                          u32DisplayHeight;    /* height of layer's display buffer*/
    HI_U32                          u32ScreenWidth;      /* 无用 width  of layer's  show    area*/
    HI_U32                          u32ScreenHeight;     /* 无用 height of layer's  show    area*/
    HIFB_POINT_S                    stPos;               /* beginning position of layer*/
    HIFB_ALPHA_S                    stAlpha;             /* alpha attribution */
    HIFB_COLORKEYEX_S               stCkey;              /* colorkey attribution */
}HIFB_EXTEND_INFO_S;

typedef struct
{
    HI_U32             u32LayerID;       /* layer id */
    atomic_t           ref_count;        /* framebuffer reference count */
    spinlock_t         lock;             /* using in 2buf refresh */
    HI_BOOL            bPreMul;
    HI_BOOL            bNeedAntiflicker;
    HI_U32             u32HDflevel;      /* horizontal deflicker level */
    HI_U32             u32VDflevel;      /* vertical deflicker level */
    HI_UCHAR           ucHDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];  /* horizontal deflicker coefficients */
    HI_UCHAR           ucVDfcoef[HIFB_DEFLICKER_LEVEL_MAX - 1];  /* vertical deflicker coefficients */
    HIFB_LAYER_ANTIFLICKER_LEVEL_E  enAntiflickerLevel;          /* antiflicker level */
    HIFB_LAYER_ANTIFLICKER_MODE_E   enAntiflickerMode;           /* antiflicker mode */
}HIFB_BASE_INFO_S;


typedef struct
{
    HI_BOOL bStartRelease;
    HI_U32  u32PhyBuf;
    HI_U32  u32SmemStartPhy;
    HI_CHAR *pVirBuf;
    struct work_struct  bDisBufWork;
}HIFB_DISP_TMPBUF_S;

typedef struct
{
    HI_BOOL             bFrameBufferRegister;
    HI_BOOL             bSetPar;
    HI_BOOL             bPanFlag;
    HI_BOOL             bVblank;
    HI_BOOL             bHwcRefresh;
    HI_BOOL             bDeCompress;
#ifdef CFG_HIFB_FENCE_SUPPORT
    HI_BOOL             bHwcRefreshWithWorkQueue;
    HI_BOOL             bEndHwcRefresh;
    HI_U8               u8RefreshCount;
#endif
#ifdef CONFIG_HIFB_GP0_GALPHA_SUM_SUPPORT
    HI_BOOL             bLayerClose;
#endif
    HI_U32              u32SmemStartPhy;
    HIFB_BASE_INFO_S    stBaseInfo;
    HIFB_EXTEND_INFO_S  stExtendInfo;

    HIFB_3D_PAR_S       st3DInfo;
    HIFB_DISP_INFO_S    stDispInfo;

    HIFB_RTIME_INFO_S   stRunInfo;
    HIFB_FRAME_INFO_S   stFrameInfo;
#ifdef CFG_HIFB_PROC_SUPPORT
    DRV_HIFB_PROC_S     stProcInfo;
#endif
    HIFB_DISP_TMPBUF_S  stDispTmpBuf;

#ifdef CFG_HIFB_FENCE_SUPPORT
    struct workqueue_struct *pstHwcRefreshWorkqueue;
    wait_queue_head_t  WaiteEndHwcRefresh;
#endif
}HIFB_PAR_S;


typedef struct
{
    struct fb_info *pstInfo;
    HI_U32    u32LayerSize;     /*u32LayerSize = fb.smem_len*/
} HIFB_LAYER_S;



typedef enum
{
    HIFB_LAYER_TYPE_HD,
    HIFB_LAYER_TYPE_SD,
    HIFB_LAYER_TYPE_AD,
    HIFB_LAYER_TYPE_CURSOR,
    HIFB_LAYER_TYPE_BUTT,
}HIFB_LAYER_TYPE_E;


typedef struct
{
    struct fb_bitfield stRed;     /* bitfield in fb mem if true color, */
    struct fb_bitfield stGreen;   /* else only length is significant */
    struct fb_bitfield stBlue;
    struct fb_bitfield stTransp;  /* transparency    */
} HIFB_ARGB_BITINFO_S;



#ifdef CONFIG_COMPAT
struct fb_fix_compat_screeninfo
{
    HI_CHAR id[16];
    HI_U32 smem_start;
    HI_U32 smem_len;
    HI_U32 type;
    HI_U32 type_aux;
    HI_U32 visual;
    HI_U16 xpanstep;
    HI_U16 ypanstep;
    HI_U16 ywrapstep;
    HI_U32 line_length;
    HI_U32 mmio_start;
    HI_U32 mmio_len;
    HI_U32 accel;
    HI_U16 capabilities;
    HI_U16 reserved[2];
};
#endif


/********************** Global Variable declaration **************************/

extern HIFB_DRV_OPS_S    g_stDrvAdpCallBackFunction;
extern HIFB_DRV_TDEOPS_S g_stGfx2dCallBackFunction;
/**
 **to save layer id and layer size
 **/
extern HIFB_LAYER_S s_stLayer[HIFB_MAX_LAYER_NUM];

extern HIFB_ARGB_BITINFO_S s_stArgbBitField[HIFB_MAX_PIXFMT_NUM];

#ifdef CFG_HIFB_SCROLLTEXT_SUPPORT
extern HIFB_SCROLLTEXT_INFO_S s_stTextLayer[HIFB_LAYER_ID_BUTT];
#endif


/******************************* API declaration *****************************/
HI_U32  DRV_HIFB_GetPixDepth       (HI_U32 BitsPerPixel);
HI_VOID DRV_HIFB_WaitVBlank        (HIFB_LAYER_ID_E enLayerId);
HI_S32  DRV_HIFB_SteroDataUpdate   (HIFB_LAYER_ID_E enLayerId, HIFB_BUFFER_S *pstSrcBuffer, HIFB_BLIT_OPT_S *pstBlitOpt);
HI_S32  DRV_HIFB_CallBack          (HI_VOID *pParaml, HI_VOID *pParamr);
HI_VOID DRV_HIFB_SetDecmpLayerInfo (HI_U32 LayerId);
HI_VOID DRV_HIFB_Lock              (spinlock_t *pLock, HI_ULONG *pLockParFlag);
HI_VOID DRV_HIFB_UnLock            (spinlock_t *pLock, HI_ULONG *pLockParFlag);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
