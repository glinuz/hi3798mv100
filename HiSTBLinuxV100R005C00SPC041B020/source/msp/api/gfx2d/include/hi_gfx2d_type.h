/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_gfx2d_type.h
* Description: Describe graphic 2D engine data type.
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#ifndef _HI_GFX_TYPE_H_
#define _HI_GFX_TYPE_H_

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/******************************** Macro Definition ******************************/
#define HI_GFX_SURFACE_ADDR_CNT 3
#define HI_GFX_SURFACE_STRIDE_CNT 3
#define HI_GFX_FILTERPARAM_ADDR_CNT 2

/*************************** Structure Definition ****************************/

/** \addtogroup      GFX TYPE */
/** @{ */  /** <!-- 【GFX TYPE】 */

typedef enum hiGFX2D_DEV_ID_E
{
    HI_GFX2D_DEV_ID_0 = 0x0,
    HI_GFX2D_DEV_ID_BUTT
} HI_GFX2D_DEV_ID_E;

/** enum of the color format */
/** CNcomment:图像格式枚举 */
typedef enum hiGFX2D_FMT_E
{
    HI_GFX2D_FMT_RGB444 = 0,          /**< RGB444格式, Red占4bits Green占4bits, Blue占4bits,其余格式依此类推 */
    HI_GFX2D_FMT_BGR444,              /**< BGR444格式 */
    HI_GFX2D_FMT_RGB555,              /**< RGB555格式 */
    HI_GFX2D_FMT_BGR555,              /**< BGR555格式 */
    HI_GFX2D_FMT_RGB565,              /**< RGB565格式 */
    HI_GFX2D_FMT_BGR565,              /**< BGR565格式 */
    HI_GFX2D_FMT_RGB888,              /**< RGB888格式 */
    HI_GFX2D_FMT_BGR888,              /**< BGR888格式 */
    HI_GFX2D_FMT_KRGB888,
    HI_GFX2D_FMT_KBGR888,
    HI_GFX2D_FMT_ARGB4444,            /**< ARGB4444格式 */
    HI_GFX2D_FMT_ABGR4444,            /**< ABGR4444格式 */
    HI_GFX2D_FMT_RGBA4444,            /**< RGBA4444格式 */
    HI_GFX2D_FMT_BGRA4444,            /**< BGRA4444格式 */
    HI_GFX2D_FMT_ARGB1555,            /**< ARGB1555格式 */
    HI_GFX2D_FMT_ABGR1555,            /**< ABGR1555格式 */
    HI_GFX2D_FMT_RGBA1555,            /**< RGBA1555格式 */
    HI_GFX2D_FMT_BGRA1555,            /**< BGRA1555格式 */
    HI_GFX2D_FMT_ARGB8565,            /**< ARGB8565格式 */
    HI_GFX2D_FMT_ABGR8565,            /**< ABGR8565格式 */
    HI_GFX2D_FMT_RGBA8565,            /**< RGBA8565格式 */
    HI_GFX2D_FMT_BGRA8565,            /**< BGRA8565格式 */
    HI_GFX2D_FMT_ARGB8888,            /**< ARGB8888格式 */
    HI_GFX2D_FMT_ABGR8888,            /**< ABGR8888格式 */
    HI_GFX2D_FMT_RGBA8888,            /**< RGBA8888格式 */
    HI_GFX2D_FMT_BGRA8888,            /**< BGRA8888格式 */
    HI_GFX2D_FMT_CLUT1,               /**<无Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    HI_GFX2D_FMT_CLUT2,               /**<无Alpha分量,调色板2bit格式,每个像用2个bit表示 */
    HI_GFX2D_FMT_CLUT4,               /**<无Alpha分量,调色板4bit格式,每个像用4个bit表示 */
    HI_GFX2D_FMT_CLUT8,               /**<无Alpha分量,调色板8bit格式,每个像用8个bit表示 */
    HI_GFX2D_FMT_ACLUT44,             /**<有Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    HI_GFX2D_FMT_ACLUT88,             /**<有Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    HI_GFX2D_FMT_YUV888,            /**<YUV packet格式，无alpha分量*/
    HI_GFX2D_FMT_AYUV8888,          /**<YUV packet格式，有alpha分量*/
    HI_GFX2D_FMT_YUYV422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_YVYU422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_UYVY422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_YYUV422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_VYUY422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_VUYY422, /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    HI_GFX2D_FMT_SEMIPLANAR400,     /**<Semi-planar YUV400格式 ,对应于JPG解码*/
    HI_GFX2D_FMT_SEMIPLANAR420UV,     /**<Semi-planar YUV420格式 */
    HI_GFX2D_FMT_SEMIPLANAR420VU,     /**<Semi-planar YUV420格式 */
    HI_GFX2D_FMT_SEMIPLANAR422UV_H,    /**<Semi-planar YUV422格式,水平方向采样，对应于JPG解码 */
    HI_GFX2D_FMT_SEMIPLANAR422VU_H,    /**<Semi-planar YUV422格式,水平方向采样，对应于JPG解码 */
    HI_GFX2D_FMT_SEMIPLANAR422UV_V,    /**<Semi-planar YUV422格式,垂直方向采样，对应于JPG解码 */
    HI_GFX2D_FMT_SEMIPLANAR422VU_V,    /**<Semi-planar YUV422格式,垂直方向采样，对应于JPG解码 */
    HI_GFX2D_FMT_SEMIPLANAR444UV,     /**<Semi-planar YUV444格式 */
    HI_GFX2D_FMT_SEMIPLANAR444VU,
    HI_GFX2D_FMT_PLANAR400,
    HI_GFX2D_FMT_PLANAR420,
    HI_GFX2D_FMT_PLANAR411,
    HI_GFX2D_FMT_PLANAR410,
    HI_GFX2D_FMT_PLANAR422H,
    HI_GFX2D_FMT_PLANAR422V,
    HI_GFX2D_FMT_PLANAR444,
} HI_GFX2D_FMT_E;

/** enum of the compress type */
/** CNcomment:图象压缩类型枚举 */
typedef enum hiGFX2D_COMPRESS_TYPE_E
{
    HI_GFX2D_COMPRESS_NONE = 0x0,     /**<CNcomment:非压缩*/
    HI_GFX2D_COMPRESS_NOLOSS,          /**<CNcomment:无损压缩*/
} HI_GFX2D_COMPRESS_TYPE_E;

/** enum of the surface type */
/** CNcomment:surface类型枚举 */
typedef enum hiGFX2D_SURFACE_TYPE_E
{
    HI_GFX2D_SURFACE_TYPE_MEM = 0x0,  /**<CNcomment:内存类型surface */
    HI_GFX2D_SURFACE_TYPE_COLOR,       /**<CNcomment:纯色类型surface */
} HI_GFX2D_SURFACE_TYPE_E;

/** enum of the filter mode */
/** CNcomment:滤波模式枚举 */
typedef enum hiGFX2D_FILTER_MODE_E
{
    HI_GFX2D_FILTER_ALL = 0x0,      /**<CNcomment:对颜色和alpha通道同时滤波*/
    HI_GFX2D_FILTER_COLOR,            /**<CNcomment:对颜色进行滤波*/
    HI_GFX2D_FILTER_ALPHA,     /**<CNcomment:对alpha通道滤波*/
    HI_GFX2D_FILTER_NONE,       /**<CNcomment:不进行滤波*/
} HI_GFX2D_FILTER_MODE_E;

/** enum of the clip mode */
/** CNcomment:剪切模式枚举 */
typedef enum hiGFX2D_CLIP_MODE_E
{
    HI_GFX2D_CLIP_NONE = 0x0,
    HI_GFX2D_CLIP_INSIDE,
    HI_GFX2D_CLIP_OUTSIDE,
} HI_GFX2D_CLIP_MODE_E;

/** enum of the rop mode */
/** CNcomment:ROP模式枚举 */
typedef enum hiGFX2D_ROP_MODE_E
{
    HI_GFX2D_ROP_BLACK = 0x0,     /**<Blackness*/
    HI_GFX2D_ROP_NOTMERGEPEN,   /**<~(S2 | S1)*/
    HI_GFX2D_ROP_MASKNOTPEN,    /**<~S2&S1*/
    HI_GFX2D_ROP_NOTCOPYPEN,    /**< ~S2*/
    HI_GFX2D_ROP_MASKPENNOT,    /**< S2&~S1 */
    HI_GFX2D_ROP_NOT,           /**< ~S1 */
    HI_GFX2D_ROP_XORPEN,        /**< S2^S1 */
    HI_GFX2D_ROP_NOTMASKPEN,    /**< ~(S2 & S1) */
    HI_GFX2D_ROP_MASKPEN,       /**< S2&S1 */
    HI_GFX2D_ROP_NOTXORPEN,     /**< ~(S2^S1) */
    HI_GFX2D_ROP_NOP,           /**< S1 */
    HI_GFX2D_ROP_MERGENOTPEN,   /**< ~S2|S1 */
    HI_GFX2D_ROP_COPYPEN,       /**< S2 */
    HI_GFX2D_ROP_MERGEPENNOT,   /**< S2|~S1 */
    HI_GFX2D_ROP_MERGEPEN,      /**< S2|S1 */
    HI_GFX2D_ROP_WHITE,         /**< Whiteness */
} HI_GFX2D_ROP_MODE_E;

/** structure of rect */
/** CNcomment:矩形结构体 */
typedef struct hiGFX2D_REST_S
{
    HI_S32 s32XPos;             /**<CNcomment:矩形的左上点横坐标*/
    HI_S32 s32YPos;              /**<CNcomment:矩形的左上点纵坐标*/
    HI_U32 u32Width;             /**<CNcomment:矩形的宽*/
    HI_U32 u32Height;           /**<CNcomment:矩形的高*/
} HI_GFX2D_RECT_S;

/** structure of compress info */
/** CNcomment:压缩信息结构体 */
typedef struct hiGFX2D_COMPRESS_S
{
    HI_GFX2D_COMPRESS_TYPE_E enCompressType;  /**<CNcomment:压缩类型*/
} HI_GFX2D_COMPRESS_S;

/** structure of alpha expand info */
/** CNcomment:ARGB1555格式alpha扩展信息结构体*/
typedef struct hiGFX2D_ALPHAEXT_S
{
    HI_BOOL bExtAlpha;          /**<CNcomment:是否采用扩展alpha.*/
    HI_U8   u8Alpha0;               /**<CNcomment:采用扩展alpha时,0对应的alpha值.*/
    HI_U8   u8Alpha1;               /**<CNcomment:采用扩展alpha时,1对应的alpha值.*/
} HI_GFX2D_ALPHAEXT_S;

/** structure of memory surface */
/** CNcomment:内存类型surface信息结构体 */
typedef struct hiGFX2D_SURFACE_S
{
    HI_GFX2D_SURFACE_TYPE_E enType; /**<CNcomment:surface类型*/
    HI_GFX2D_FMT_E enFmt;         /**<CNcomment:像素格式,只对MEM类型surface有效*/
    HI_U32 u32Width;          /**<CNcomment:宽度*/
    HI_U32 u32Height;         /**<CNcomment:高度*/
    HI_U32 u32Color;    /**<CNcomment:surface颜色值,只对COLOR类型surface有效*/

    /**<CNcomment:内存地址信息,只对mem类型的surface有效
    package格式:u32Phyaddr[0]表示数据地址
    semi-planar格式:u32Phyaddr[0]表示Y分量地址,u32Phyaddr[1]表示CbCr分量地址
    planar格式:u32Phyaddr[0]表示Y分量地址,u32Phyaddr[1]表示Cb分量地址,
    u32Phyaddr[2]表示Cr分量地址*/
    HI_U32 u32Phyaddr[HI_GFX_SURFACE_ADDR_CNT];

    /**<CNcomment:行跨度,只对mem类型的surface有效
    package格式:u32Stride[0]表示数据跨度
    semi-planar格式:u32Stride[0]表示Y分量行跨度,u32Stride[1]表示CbCr分量行跨度
    planar格式:u32Stride[0]表示Y分量行跨度,u32Stride[1]表示Cb分量行跨度,
    u32Stride[2]表示Cr分量行跨度*/
    HI_U32 u32Stride[HI_GFX_SURFACE_STRIDE_CNT];

    HI_BOOL bPremulti;   /**<CNcomment:是否为预乘数据*/
    HI_GFX2D_COMPRESS_S stCompressInfo;      /**<CNcomment:压缩信息，只对MEM类型surface有效*/
    HI_U32  u32HistogramPhyaddr;     /**<CNcomment:直方图物理地址，只对MEM类型surface有效*/
    HI_GFX2D_ALPHAEXT_S stAlphaExt; /**<CNcomment:alpha扩展信息,只对MEM类型surface的ARGB1555格式有效*/
    HI_U32  u32PalettePhyaddr; /**<CNcomment:调色板地址,只对MEM类型surface的调色板格式有效*/

    HI_S32 s32AcquireFenceFd;
    HI_S32 s32ReleaseFenceFd;
} HI_GFX2D_SURFACE_S;

/** structure of surface */
/** CNcomment:RGB格式colorkey信息结构体*/
typedef struct hiGFX2D_COLORKEY_RGB_S
{
    HI_BOOL bAEnable;
    HI_BOOL bAOut;
    HI_U8   u8AMin;
    HI_U8   u8AMax;
    HI_U8   u8AMask;

    HI_BOOL bREnable;
    HI_BOOL bROut;
    HI_U8   u8RMin;
    HI_U8   u8RMax;
    HI_U8   u8RMask;

    HI_BOOL bGEnable;
    HI_BOOL bGOut;
    HI_U8   u8GMin;
    HI_U8   u8GMax;
    HI_U8   u8GMask;

    HI_BOOL bBEnable;
    HI_BOOL bBOut;
    HI_U8   u8BMin;
    HI_U8   u8BMax;
    HI_U8   u8BMask;
} HI_GFX2D_COLORKEY_RGB_S;

/** structure of surface */
/** CNcomment:clut格式 colorkey信息结构体*/
typedef struct hiGFX2D_COLORKEY_CLUT_S
{
    HI_BOOL bAEnable;
    HI_BOOL bAOut;
    HI_U8   u8AMin;
    HI_U8   u8AMax;
    HI_U8   u8AMask;

    HI_BOOL bClutEnable;
    HI_BOOL bClutOut;
    HI_U8   u8ClutMin;
    HI_U8   u8ClutMax;
    HI_U8   u8ClutMask;
} HI_GFX2D_COLORKEY_CLUT_S;

typedef union hiGFX2D_COLORKEY_U
{
    HI_GFX2D_COLORKEY_RGB_S stRgb;
    HI_GFX2D_COLORKEY_CLUT_S stClut;
} HI_GFX2D_COLORKEY_U;

/** structure of color space */
/** CNcomment:颜色空间转换信息结构体*/
typedef struct hiGFX2D_CSC_S
{
    HI_BOOL bUserParamEnable;       /**<CNcomment:是否使用用户自定义系数*/
    HI_U32  u32UserParamPhyaddr;    /**<CNcomment:用户自定义系数物理地址*/
} HI_GFX2D_CSC_S;

/** structure of filter info */
/** CNcomment:滤波信息结构体*/
typedef struct hiGFX2D_FILTER_S
{
    HI_GFX2D_FILTER_MODE_E enFilterMode;      /**<CNcomment:滤波模式*/
} HI_GFX2D_FILTER_S;

/** structure of clip info */
/** CNcomment:剪切域信息结构体*/
typedef struct hiGFX2D_CLIP_S
{
    HI_GFX2D_CLIP_MODE_E enClipMode;      /**<CNcomment:剪切模式*/
    HI_GFX2D_RECT_S      stClipRect;              /**<CNcomment:剪切矩形*/
} HI_GFX2D_CLIP_S;

/** structure of resize info */
/** CNcomment:缩放信息结构体*/
typedef struct hiGFX2D_RESIZE_S
{
    HI_BOOL         bResizeEnable; /**<CNcomment:是否使能缩放.*/
    HI_GFX2D_FILTER_S stFilter;       /**<CNcomment:滤波信息.*/
} HI_GFX2D_RESIZE_S;

/** structure of alpha blend info */
/** CNcomment:叠加信息结构体*/
typedef struct hiGFX2D_COMPOSE_BLEND_S
{
    HI_BOOL bCovBlend;
    HI_BOOL bPixelAlphaEnable;  /**<CNcomment:是否使能像素alpha.*/
    HI_BOOL bGlobalAlphaEnable; /**<CNcomment:是否使能全局alpha.*/
    HI_U8   u8GlobalAlpha;           /**<CNcomment:全局alpha值.*/
} HI_GFX2D_COMPOSE_BLEND_S;

/** structure of rop info */
/** CNcomment:rop信息结构体*/
typedef struct hiGFX2D_ROP_S
{
    HI_BOOL           bEnable;
    HI_GFX2D_ROP_MODE_E enAlphaRopMode;
    HI_GFX2D_ROP_MODE_E enRGBRopMode;
} HI_GFX2D_ROP_S;

/** structure of key info */
/** CNcomment:colorkey信息结构体*/
typedef struct hiGFX2D_COLORKEY_S
{
    HI_BOOL           bEnable;
    HI_GFX2D_COLORKEY_U enKeyValue;
} HI_GFX2D_COLORKEY_S;

/** structure of operation info */
/** CNcomment:操作选项信息结构体*/
typedef struct hiGFX2D_COMPOSE_OPT_S
{
    HI_GFX2D_COMPOSE_BLEND_S stBlend;    /**<CNcomment:alpha叠加信息.*/
    HI_GFX2D_RESIZE_S        stResize; /**<CNcomment:缩放信息.*/
    HI_GFX2D_CLIP_S          stClip;  /**<CNcomment:剪切域信息.*/
    HI_GFX2D_ROP_S           stRop;/**<CNcomment:ROP信息.*/
    HI_GFX2D_COLORKEY_S      stKey; /**<CNcomment:key信息.*/
} HI_GFX2D_COMPOSE_OPT_S;

/** structure of composed surface and operation info */
/** CNcomment:多层叠加surface及操作选项结构体*/
typedef struct hiGFX2D_COMPOSE_S
{
    HI_GFX2D_SURFACE_S     stSurface; /**<CNcomment:surface信息.*/
    HI_GFX2D_RECT_S        stInRect;      /**<CNcomment:输入操作矩形.*/
    HI_GFX2D_RECT_S        stOutRect;     /**<CNcomment:输出矩形.*/
    HI_GFX2D_COMPOSE_OPT_S stOpt;     /**<CNcomment:操作选项.*/
} HI_GFX2D_COMPOSE_S;

/* composed surface list */
typedef struct hiGFX2D_COMPOSE_LIST_S
{
    HI_GFX2D_COMPOSE_S *pstCompose;      /**<CNcomment:.叠加层链表,按照由下到上的顺序排列*/
    HI_U32             u32ComposeCnt;   /**<CNcomment:.叠加层数量*/
    HI_U32             u32BgColor;      /**<CNcomment:叠加背景色.*/
} HI_GFX2D_COMPOSE_LIST_S;

/** @} */  /*! <!-- Structure Definition end */

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif  /*_HI_GFX_TYPE_H_*/
