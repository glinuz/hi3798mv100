/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_tde_type.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : 
Function List       :                    
History             :
Date                        Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/
#ifndef  __HI_TDE_TYPE_H__
#define  __HI_TDE_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_tde_errcode.h"

/****************************************************************************/
/*                             TDE2 types define                             */
/****************************************************************************/

/*************************** Structure Definition ****************************/
/** \addtogroup      TDE */
/** @{ */  /** <!-- 【 TDE】 */  


/**Definition of the TDE handle*/
/**CNcomment:TDE句柄定义 */
typedef HI_S32 TDE_HANDLE;

/**TDE callback functions*/
/**CNcomment:TDE中断回调函数 */
typedef HI_VOID (* TDE_FUNC_CB) (HI_VOID *pParaml, HI_VOID *pParamr);

typedef HI_VOID (* TDE_TRIG_SEL)(HI_U32);

/**RGB and packet YUV formats*/
/**CNcomment:RGB和Packet YUV 颜色格式 */
typedef enum hiTDE2_COLOR_FMT_E
{
    TDE2_COLOR_FMT_RGB444 = 0,          /**<For the RGB444 format, red occupies four bits, green occupies four bits, and blue occupies four bits. Other formats may be deduced by analogy.*//**<CNcomment:RGB444格式, Red占4bits Green占4bits, Blue占4bits,其余格式依此类推 */
    TDE2_COLOR_FMT_BGR444,              /**<BGR444 format*//**<CNcomment:BGR444格式 */    
    TDE2_COLOR_FMT_RGB555,              /**<RGB555 format*//**<CNcomment:RGB555格式 */
    TDE2_COLOR_FMT_BGR555,              /**<BGR555 format*//**<CNcomment:BGR555格式 */
    TDE2_COLOR_FMT_RGB565,              /**<RGB565 format*//**<CNcomment:RGB565格式 */
    TDE2_COLOR_FMT_BGR565,              /**<BGR565 format*//**<CNcomment:BGR565格式 */
    TDE2_COLOR_FMT_RGB888,              /**<RGB888 format*//**<CNcomment:RGB888格式 */
    TDE2_COLOR_FMT_BGR888,              /**<BGR888 format*//**<CNcomment:BGR888格式 */
    TDE2_COLOR_FMT_ARGB4444,          /**<ARGB4444 format*//**<CNcomment:ARGB4444格式 */
    TDE2_COLOR_FMT_ABGR4444,           /**<ABGR4444 format*//**<CNcomment:ABGR4444格式 */
    TDE2_COLOR_FMT_RGBA4444,            /**<RGBA4444 format*//**<CNcomment:RGBA4444格式 */
    TDE2_COLOR_FMT_BGRA4444,            /**<BGRA4444 format*//**<CNcomment:BGRA4444格式 */
    TDE2_COLOR_FMT_ARGB1555,            /**<ARGB1555 format*//**<CNcomment:ARGB1555格式 */
    TDE2_COLOR_FMT_ABGR1555,            /**<ABGR1555 format*//**<CNcomment:ABGR1555格式 */
    TDE2_COLOR_FMT_RGBA1555,            /**<RGBA1555 format*//**<CNcomment:RGBA1555格式 */
    TDE2_COLOR_FMT_BGRA1555,            /**<BGRA1555 format*//**<CNcomment:BGRA1555格式 */
    TDE2_COLOR_FMT_ARGB8565,            /**<ARGB8565 format*//**<CNcomment:ARGB8565格式 */
    TDE2_COLOR_FMT_ABGR8565,            /**<ABGR8565 format*//**<CNcomment:ABGR8565格式 */
    TDE2_COLOR_FMT_RGBA8565,            /**<RGBA8565 format*//**<CNcomment:RGBA8565格式 */
    TDE2_COLOR_FMT_BGRA8565,            /**<BGRA8565 format*//**<CNcomment:BGRA8565格式 */
    TDE2_COLOR_FMT_ARGB8888,            /**<ARGB8888 format*//**<CNcomment:ARGB8888格式 */
    TDE2_COLOR_FMT_ABGR8888,            /**<ABGR8888 format*//**<CNcomment:ABGR8888格式 */
    TDE2_COLOR_FMT_RGBA8888,            /**<RGBA8888 format*//**<CNcomment:RGBA8888格式 */
    TDE2_COLOR_FMT_BGRA8888,            /**<BGRA8888 format*//**<CNcomment:BGRA8888格式 */
    TDE2_COLOR_FMT_RABG8888,            /**<RABG8888 format*//**<CNcomment:RABG8888格式 */
    TDE2_COLOR_FMT_CLUT1,               /**<1-bit palette format without alpha component. Each pixel occupies one bit.*//**<CNcomment:无Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    TDE2_COLOR_FMT_CLUT2,               /**<2-bit palette format without alpha component. Each pixel occupies two bits.*//**<CNcomment:无Alpha分量,调色板2bit格式,每个像用2个bit表示 */
    TDE2_COLOR_FMT_CLUT4,               /**<4-bit palette format without alpha component. Each pixel occupies four bits.*//**<CNcomment:无Alpha分量,调色板4bit格式,每个像用4个bit表示 */
    TDE2_COLOR_FMT_CLUT8,               /**<8-bit palette format without alpha component. Each pixel occupies eight bits.*//**<CNcomment:无Alpha分量,调色板8bit格式,每个像用8个bit表示 */
    TDE2_COLOR_FMT_ACLUT44,             /**<1-bit palette format with alpha component. Each pixel occupies one bit.*//**<CNcomment:有Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    TDE2_COLOR_FMT_ACLUT88,             /**<1-bit palette format with alpha component. Each pixel occupies one bit.*//**<CNcomment:有Alpha分量,调色板1bit格式,每个像用1个bit表示 */
    TDE2_COLOR_FMT_A1,                  /**<Alpha format. Each pixel occupies one bit.*//**<CNcomment:alpha格式，每个点用1bit */
    TDE2_COLOR_FMT_A8,                  /**<Alpha format. Each pixel occupies eight bits.*//**<CNcomment:alpha格式，每个点用8bit */
    TDE2_COLOR_FMT_YCbCr888,            /**<YUV packet format without alpha component*//**<CNcomment:YUV packet格式，无alpha分量*/
    TDE2_COLOR_FMT_AYCbCr8888,          /**<YUV packet format with alpha component*//**<CNcomment:YUV packet格式，有alpha分量*/
    TDE2_COLOR_FMT_YCbCr422,            /**<YUV packet422 format*//**<CNcomment:YUV packet422格式 */
    TDE2_COLOR_FMT_byte,                /**<Only for fast copy*//**<CNcomment:仅用于数据快速copy*/
    TDE2_COLOR_FMT_halfword,            /**<Only for fast copy*//**<CNcomment:仅用于数据快速copy*/
    TDE2_COLOR_FMT_JPG_YCbCr400MBP,     /**<Semi-planar YUV400 format, for JPG decoding*//**<CNcomment:Semi-planar YUV400格式 ,对应于JPG解码*/
    TDE2_COLOR_FMT_JPG_YCbCr422MBHP,    /**<Semi-planar YUV422 format, horizontal sampling, for JPG decoding*//**<CNcomment:Semi-planar YUV422格式,水平方向采样，对应于JPG解码 */
    TDE2_COLOR_FMT_JPG_YCbCr422MBVP,    /**<Semi-planar YUV422 format, vertical sampling, for JPG decoding*//**<CNcomment:Semi-planar YUV422格式,垂直方向采样，对应于JPG解码 */
    TDE2_COLOR_FMT_MP1_YCbCr420MBP,     /**<Semi-planar YUV420 format*//**<CNcomment:Semi-planar YUV420格式 */
    TDE2_COLOR_FMT_MP2_YCbCr420MBP,     /**<Semi-planar YUV420 format*//**<CNcomment:Semi-planar YUV420格式 */
    TDE2_COLOR_FMT_MP2_YCbCr420MBI,     /**<Semi-planar YUV400 format*//**<CNcomment:Semi-planar YUV400格式 */
    TDE2_COLOR_FMT_JPG_YCbCr420MBP,     /**<Semi-planar YUV400 format, for JPG decoding*//**<CNcomment:Semi-planar YUV400格式,对于应于JPG */
    TDE2_COLOR_FMT_JPG_YCbCr444MBP,     /**<Semi-planar YUV444 format*//**<CNcomment:Semi-planar YUV444格式 */
    TDE2_COLOR_FMT_BUTT                 /**<End of enumeration*//**<CNcomment: 枚举量结束*/
} TDE2_COLOR_FMT_E;

/**Definition of the semi-planar YUV format*/
/**CNcomment:Semi-planar YUV 格式定义 */
typedef enum hiTDE2_MB_COLORFMT_E
{
    TDE2_MB_COLOR_FMT_JPG_YCbCr400MBP = 0,/**<Semi-planar YUV400 format, for JPG decoding*//**<CNcomment:Semi-planar YUV400格式 ,对应于JPG解码*/
    TDE2_MB_COLOR_FMT_JPG_YCbCr422MBHP,   /**<Semi-planar YUV422 format, horizontal sampling, for JPG decoding*//**<CNcomment:Semi-planar YUV422格式,水平方向采样，对应于JPG解码 */
    TDE2_MB_COLOR_FMT_JPG_YCbCr422MBVP,   /**<Semi-planar YUV422 format, vertical sampling, for JPG decoding*//**<CNcomment:Semi-planar YUV422格式,垂直方向采样，对应于JPG解码 */
    TDE2_MB_COLOR_FMT_MP1_YCbCr420MBP,    /**<Semi-planar YUV420 format*//**<CNcomment:Semi-planar YUV420格式 */
    TDE2_MB_COLOR_FMT_MP2_YCbCr420MBP,    /**<Semi-planar YUV420 format*//**<CNcomment:Semi-planar YUV420格式 */
    TDE2_MB_COLOR_FMT_MP2_YCbCr420MBI,    /**<Semi-planar YUV400 format*//**<CNcomment:Semi-planar YUV400格式 */
    TDE2_MB_COLOR_FMT_JPG_YCbCr420MBP,    /**<Semi-planar YUV400 format, for JPG pictures*//**<CNcomment:Semi-planar YUV400格式,对于应于JPG */
    TDE2_MB_COLOR_FMT_JPG_YCbCr444MBP,    /**<Semi-planar YUV444 format, for JPG pictures*//**<CNcomment:Semi-planar YUV444格式,对于应于JPG */
    TDE2_MB_COLOR_FMT_BUTT
} TDE2_MB_COLOR_FMT_E;

/**Structure of the bitmap information set by customers*/
/**CNcomment:用户设置的位图信息结构 */
typedef struct hiTDE2_SURFACE_S
{
    HI_U32 u32PhyAddr;              /**<Header address of a bitmap or the Y component*//**<CNcomment:位图首地址或Y分量地址 */

    TDE2_COLOR_FMT_E enColorFmt;    /**<Color format*//**<CNcomment:颜色格式 */

    HI_U32 u32Height;               /**<Bitmap height*//**<CNcomment:位图高度 */

    HI_U32 u32Width;                /**<Bitmap width*//**<CNcomment:位图宽度 */

    HI_U32 u32Stride;               /**<Stride of a bitmap or the Y component*//**<CNcomment:位图跨度或Y分量跨度 */
    HI_U8* pu8ClutPhyAddr;          /**<Address of the color look-up table (CLUT), for color extension or color correction*//**<CNcomment:Clut表地址,用作颜色扩展或颜色校正*/

    HI_BOOL bYCbCrClut;             /**<Whether the CLUT is in the YCbCr space.*//**<CNcomment:Clut表是否位于YCbCr空间 */

    HI_BOOL bAlphaMax255;           /**<The maximum alpha value of a bitmap is 255 or 128.*//**<CNcomment:位图alpha最大值为255还是128 */

    HI_BOOL bAlphaExt1555;          /*<Whether to enable the alpha extension of an ARGB1555 bitmap.*//**<CNcomment:是否使能1555的Alpha扩展 */
    HI_U8 u8Alpha0;                 /**<Values of alpha0 and alpha1, used as the ARGB1555 format*//**<CNcomment:Alpha0、Alpha1值，用作ARGB1555格式 */
    HI_U8 u8Alpha1;                 /**<Values of alpha0 and alpha1, used as the ARGB1555 format*//**<CNcomment:Alpha0、Alpha1值，用作ARGB1555格式 */
    HI_U32 u32CbCrPhyAddr;          /**<Address of the CbCr component, pilot*//**<CNcomment:CbCr分量地址,pilot */        
    HI_U32 u32CbCrStride;           /**<Stride of the CbCr component, pilot*//**<CNcomment:CbCr分量跨度,pilot */
} TDE2_SURFACE_S;

/**Definition of the semi-planar YUV data*/
/**CNcomment:Semi-planar YUV格式数据定义 */
typedef struct hiTDE2_MB_S
{
    TDE2_MB_COLOR_FMT_E enMbFmt;        /**<YUV format*//**<CNcomment:YUV格式 */
    HI_U32              u32YPhyAddr;    /**<Physical address of the Y component*//**<CNcomment:Y分量物理地址 */
    HI_U32              u32YWidth;      /**<Width of the Y component*//**<CNcomment:Y分量宽度 */
    HI_U32              u32YHeight;     /**<Height of the Y component*//**<CNcomment:Y分量高度 */
    HI_U32              u32YStride;     /**< Stride of the Y component, indicating bytes in each row*//**<CNcomment:Y分量跨度，每行字节数 */
    HI_U32              u32CbCrPhyAddr; /**<Width of the UV component*//**<CNcomment:UV分量宽度 */
    HI_U32              u32CbCrStride;  /**<Stride of the UV component, indicating the bytes in each row*//**<CNcomment:UV分量跨度,每行字节数 */
} TDE2_MB_S;

/**Definition of the TDE rectangle*/
/**CNcomment:TDE矩形定义*/
typedef struct hiTDE2_RECT_S
{
    HI_S32 s32Xpos;     /**<Horizontal coordinate*//**<CNcomment:x坐标 */
    HI_S32 s32Ypos;     /**<Vertical coordinate*//**<CNcomment:y坐标 */
    HI_U32 u32Width;    /**<Width*//**<CNcomment:宽度 */
    HI_U32 u32Height;   /**<Height*//**<CNcomment:高度 */
} TDE2_RECT_S;

/**Logical operation type*/
/**CNcomment:逻辑运算方式 */
typedef enum hiTDE2_ALUCMD_E
{
    TDE2_ALUCMD_NONE = 0x0,         /**<No alpha and raster of operation (ROP) blending*//**<CNcomment:不进行alpha和rop叠加 */    
    TDE2_ALUCMD_BLEND = 0x1,        /**<Alpha blending*//**<CNcomment:Alpha混合*/
    TDE2_ALUCMD_ROP = 0x2,          /**<ROP blending*//**<CNcomment:进行rop叠加 */
    TDE2_ALUCMD_COLORIZE = 0x4,     /**<Colorize operation*//**<CNcomment:进行Colorize操作 */
    TDE2_ALUCMD_BUTT = 0x8          /**<End of enumeration*//**<CNcomment:枚举结束 */
} TDE2_ALUCMD_E;

/**Definition of ROP codes*/
/**CNcomment:ROP操作码定义 */
typedef enum hiTDE2_ROP_CODE_E
{
    TDE2_ROP_BLACK = 0,     /**<Blackness*/
    TDE2_ROP_NOTMERGEPEN,   /**<~(S2 | S1)*/
    TDE2_ROP_MASKNOTPEN,    /**<~S2&S1*/
    TDE2_ROP_NOTCOPYPEN,    /**< ~S2*/
    TDE2_ROP_MASKPENNOT,    /**< S2&~S1 */
    TDE2_ROP_NOT,           /**< ~S1 */
    TDE2_ROP_XORPEN,        /**< S2^S1 */
    TDE2_ROP_NOTMASKPEN,    /**< ~(S2 & S1) */
    TDE2_ROP_MASKPEN,       /**< S2&S1 */
    TDE2_ROP_NOTXORPEN,     /**< ~(S2^S1) */
    TDE2_ROP_NOP,           /**< S1 */
    TDE2_ROP_MERGENOTPEN,   /**< ~S2|S1 */
    TDE2_ROP_COPYPEN,       /**< S2 */
    TDE2_ROP_MERGEPENNOT,   /**< S2|~S1 */
    TDE2_ROP_MERGEPEN,      /**< S2|S1 */
    TDE2_ROP_WHITE,         /**< Whiteness */
    TDE2_ROP_BUTT
} TDE2_ROP_CODE_E;

/**Definition of the blit mirror*/
/**CNcomment:blit镜像定义 */
typedef enum hiTDE2_MIRROR_E
{
    TDE2_MIRROR_NONE = 0,       /**<No mirror*//**<CNcomment:不进行镜像 */
    TDE2_MIRROR_HORIZONTAL,     /**<Horizontal mirror*//**<CNcomment:水平镜像 */
    TDE2_MIRROR_VERTICAL,       /**<Vertical mirror*//**<CNcomment:垂直镜像 */
    TDE2_MIRROR_BOTH,           /**<Horizontal and vertical mirror*//**<CNcomment:垂直和水平镜像 */
    TDE2_MIRROR_BUTT
} TDE2_MIRROR_E;

/**Clip operation type*/
/**CNcomment:Clip操作类型*/
typedef enum hiTDE2_CLIPMODE_E
{
    TDE2_CLIPMODE_NONE = 0, /**<No clip*//**<CNcomment:无clip操作 */
    TDE2_CLIPMODE_INSIDE,   /**<Clip the data within the rectangle to output and discard others*//**<CNcomment:剪切矩形范围内的数据输出,其余扔掉*/
    TDE2_CLIPMODE_OUTSIDE,  /**<Clip the data outside the rectangle to output and discard others*//**<CNcomment:剪切矩形范围外的数据输出，其余扔掉*/
    TDE2_CLIPMODE_BUTT
} TDE2_CLIPMODE_E;

/**Scaling mode for the macroblock*/
/**CNcomment:宏块格式缩放类型*/
typedef enum hiTDE2_MBRESIZE_E
{
    TDE2_MBRESIZE_NONE = 0,         /**<No scaling*//**<CNcomment:不做缩放 */
    TDE2_MBRESIZE_QUALITY_LOW,      /**<Low-quality scaling*//**<CNcomment:低质量缩放 */
    TDE2_MBRESIZE_QUALITY_MIDDLE,   /**<Medium-quality scaling*//**<CNcomment:中质量缩放 */
    TDE2_MBRESIZE_QUALITY_HIGH,     /**<High-quality scaling*//**<CNcomment:高质量缩放 */
    TDE2_MBRESIZE_BUTT
} TDE2_MBRESIZE_E;

/**Definition of fill colors*/
/**CNcomment:填充色定义 */
typedef struct hiTDE2_FILLCOLOR_S
{
    TDE2_COLOR_FMT_E enColorFmt;    /**<TDE pixel format*//**<CNcomment:TDE像素格式 */
    HI_U32           u32FillColor;  /**<Fill colors that vary according to pixel formats*//**<CNcomment:填充颜色，根据像素格式而不同 */
} TDE2_FILLCOLOR_S;

/**Definition of colorkey modes*/
/**CNcomment:colorkey选择方向定义 */
typedef enum hiTDE2_COLORKEY_MODE_E
{
    TDE2_COLORKEY_MODE_NONE = 0,     /**<No colorkey*//**<CNcomment:不做color key */
    TDE2_COLORKEY_MODE_FOREGROUND,   /**<When performing the colorkey operation on the foreground bitmap, you need to perform this operation before the CLUT for color extension and perform this operation after the CLUT for color correction.*//**<CNcomment:对前景位图进行color key，说明:对于颜色扩展，在CLUT前做color key；对于颜色校正:在CLUT后做color key */
    TDE2_COLORKEY_MODE_BACKGROUND,   /**<Perform the colorkey operation on the background bitmap*//**<CNcomment:对背景位图进行color key*/
    TDE2_COLORKEY_MODE_BUTT
} TDE2_COLORKEY_MODE_E;

/**Definition of colorkey range*/
/**CNcomment:colorkey范围定义 */
typedef struct hiTDE2_COLORKEY_COMP_S
{
    HI_U8 u8CompMin;           /**<Minimum value of a component*//**<CNcomment:分量最小值*/
    HI_U8 u8CompMax;           /**<Maximum value of a component*//**<CNcomment:分量最大值*/
    HI_U8 bCompOut;            /**<The colorkey of a component is within or beyond the range.*//**<CNcomment:分量关键色在范围内/范围外*/
    HI_U8 bCompIgnore;         /**<Whether to ignore a component.*//**<CNcomment:分量是否忽略*/
    HI_U8 u8CompMask;          /**<Component mask*//**<CNcomment:分量掩码*/
    HI_U8 u8Reserved;
    HI_U8 u8Reserved1;
    HI_U8 u8Reserved2;
} TDE2_COLORKEY_COMP_S;

/**Definition of colorkey values*/
/**CNcomment:colorkey值定义 */
typedef union hiTDE2_COLORKEY_U
{
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;   /**<Alpha component*//**<CNcomment:alpha 分量信息 */
        TDE2_COLORKEY_COMP_S stRed;     /**<Red component*//**<CNcomment:红色分量信息 */
        TDE2_COLORKEY_COMP_S stGreen;   /**<Green component*//**<CNcomment:绿色分量信息 */
        TDE2_COLORKEY_COMP_S stBlue;    /**<Blue component*//**<CNcomment:蓝色分量信息 */
    } struCkARGB;
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;   /**<Alpha component*//**<CNcomment:alpha 分量信息 */
        TDE2_COLORKEY_COMP_S stY;       /**<Y component*//**<CNcomment:Y分量信息 */
        TDE2_COLORKEY_COMP_S stCb;      /**<Cb component*//**<CNcomment:Cb分量信息 */
        TDE2_COLORKEY_COMP_S stCr;      /**<Cr component*//**<CNcomment:Cr分量信息 */
    } struCkYCbCr;
    struct
    {
        TDE2_COLORKEY_COMP_S stAlpha;   /**<Alpha component*//**<CNcomment:alpha 分量信息 */
        TDE2_COLORKEY_COMP_S stClut;    /**<Palette component*//**<CNcomment:调色板分量信息 */
    } struCkClut;
} TDE2_COLORKEY_U;

/*Definition of alpha output sources*/
/**CNcomment:输出alpha定义 */
typedef enum hiTDE2_OUTALPHA_FROM_E
{
    TDE2_OUTALPHA_FROM_NORM = 0,    /**<Output from the result of alpha blending or anti-flicker*//**<CNcomment:来源于alpha blending的结果或者抗闪烁的结果 */
    TDE2_OUTALPHA_FROM_BACKGROUND,  /**<Output from the background bitmap*//**<CNcomment:来源于背景位图 */
    TDE2_OUTALPHA_FROM_FOREGROUND,  /**<Output from the foreground bitmap*//**<CNcomment:来源于前景位图 */
    TDE2_OUTALPHA_FROM_GLOBALALPHA, /**<Output from the global alpha*//**<CNcomment:来源于全局alpha */
    TDE2_OUTALPHA_FROM_BUTT
} TDE2_OUTALPHA_FROM_E;

/**Definition of filtering*/
/**CNcomment:缩放滤波定义 */
typedef enum hiTDE2_FILTER_MODE_E
{
    TDE2_FILTER_MODE_COLOR = 0, /**<Filter the color*//**<CNcomment:对颜色进行滤波 */
    TDE2_FILTER_MODE_ALPHA,     /**<Filter the alpha channel*//**<CNcomment:对alpha通道滤波 */
    TDE2_FILTER_MODE_BOTH,      /**<Filter the color and alpha channel*//**<CNcomment:对颜色和alpha通道同时滤波 */
    TDE2_FILTER_MODE_NONE,      /**<No filter *//**<CNcomment:不进行滤波 */
    TDE2_FILTER_MODE_BUTT
} TDE2_FILTER_MODE_E;

/**Configuration of the anti-flicker channel*/
/**CNcomment:抗闪烁处理通道配置 */
typedef enum hiTDE2_DEFLICKER_MODE_E
{
    TDE2_DEFLICKER_MODE_NONE = 0,   /*<No anti-flicker*//**<CNcomment:不做抗闪 */
    TDE2_DEFLICKER_MODE_RGB,        /**<Perform anti-flicker on the RGB component*//**<CNcomment:RGB分量抗闪 */
    TDE2_DEFLICKER_MODE_BOTH,       /**<Perform anti-flicker on the alpha component*//**<CNcomment:alpha分量抗闪 */
    TDE2_DEFLICKER_MODE_BUTT
}TDE2_DEFLICKER_MODE_E;

/* blend mode */
typedef enum hiTDE2_BLEND_MODE_E
{
    TDE2_BLEND_ZERO = 0x0,
    TDE2_BLEND_ONE,
    TDE2_BLEND_SRC2COLOR,
    TDE2_BLEND_INVSRC2COLOR,
    TDE2_BLEND_SRC2ALPHA,
    TDE2_BLEND_INVSRC2ALPHA,
    TDE2_BLEND_SRC1COLOR,
    TDE2_BLEND_INVSRC1COLOR,
    TDE2_BLEND_SRC1ALPHA,
    TDE2_BLEND_INVSRC1ALPHA,
    TDE2_BLEND_SRC2ALPHASAT,
    TDE2_BLEND_BUTT
}TDE2_BLEND_MODE_E;

/**Alpha blending command. You can set parameters or select Porter or Duff.*/
/**CNcomment:alpha混合命令,可以选择自己配置参数，也可以选择Porter/Duff中的一种 */
/* pixel = (source * fs + destination * fd),
   sa = source alpha,
   da = destination alpha */
typedef enum hiTDE2_BLENDCMD_E
{
    TDE2_BLENDCMD_NONE = 0x0,     /**< fs: sa      fd: 1.0-sa */
    TDE2_BLENDCMD_CLEAR,    /**< fs: 0.0     fd: 0.0 */
    TDE2_BLENDCMD_SRC,      /**< fs: 1.0     fd: 0.0 */
    TDE2_BLENDCMD_SRCOVER,  /**< fs: 1.0     fd: 1.0-sa */
    TDE2_BLENDCMD_DSTOVER,  /**< fs: 1.0-da  fd: 1.0 */
    TDE2_BLENDCMD_SRCIN,    /**< fs: da      fd: 0.0 */
    TDE2_BLENDCMD_DSTIN,    /**< fs: 0.0     fd: sa */
    TDE2_BLENDCMD_SRCOUT,   /**< fs: 1.0-da  fd: 0.0 */
    TDE2_BLENDCMD_DSTOUT,   /**< fs: 0.0     fd: 1.0-sa */
    TDE2_BLENDCMD_SRCATOP,  /**< fs: da      fd: 1.0-sa */
    TDE2_BLENDCMD_DSTATOP,  /**< fs: 1.0-da  fd: sa */
    TDE2_BLENDCMD_ADD,      /**< fs: 1.0     fd: 1.0 */
    TDE2_BLENDCMD_XOR,      /**< fs: 1.0-da  fd: 1.0-sa */
    TDE2_BLENDCMD_DST,      /**< fs: 0.0     fd: 1.0 */
    TDE2_BLENDCMD_CONFIG,   /**<You can set the parameteres.*//**<CNcomment:用户自己配置参数*/
    TDE2_BLENDCMD_BUTT
}TDE2_BLENDCMD_E;

/**Options for the alpha blending operation*/
/**CNcomment:alpha混合操作选项 */
typedef struct hiTDE2_BLEND_OPT_S
{
    HI_BOOL  bGlobalAlphaEnable;        /**<Global alpha enable*//**<CNcomment:是否使能全局alpha */
    HI_BOOL  bPixelAlphaEnable;         /**<Pixel alpha enable*//**<CNcomment:是否使能象素alpha */
    HI_BOOL bSrc1AlphaPremulti;         /**<Src1 alpha premultiply enable*//**<CNcomment:是否使能Src1 alpha预乘 */
    HI_BOOL bSrc2AlphaPremulti;         /**<Src2 alpha premultiply enable*//**<CNcomment:是否使能Src2 alpha预乘 */
    TDE2_BLENDCMD_E eBlendCmd;          /**<Alpha blending command*//**<CNcomment:alpha混合命令*/    
    TDE2_BLEND_MODE_E eSrc1BlendMode;   /**<Src1 blending mode select. It is valid when eBlendCmd is set to TDE2_BLENDCMD_CONFIG.*//**<CNcomment:Src1 blend模式选择,在eBlendCmd = TDE2_BLENDCMD_CONFIG时有效 */
    TDE2_BLEND_MODE_E eSrc2BlendMode;   /**<Src2 blending mode select. It is valid when eBlendCmd is set to TDE2_BLENDCMD_CONFIG.*//**<CNcomment:Src2 blend模式选择,在eBlendCmd = TDE2_BLENDCMD_CONFIG时有效 */
}TDE2_BLEND_OPT_S;

/**CSC parameter option*/
/**CNcomment:CSC参数选项*/
typedef struct hiTDE2_CSC_OPT_S
{
    HI_BOOL bICSCUserEnable;		/**User-defined ICSC parameter enable*//**CNcomment:用户自定义ICSC参数使能*/
    HI_BOOL bICSCParamReload;	/**User-defined ICSC parameter reload enable*//**CNcomment:重新加载用户自定义ICSC参数使能*/
    HI_BOOL bOCSCUserEnable;		/**User-defined OCSC parameter enable*//**CNcomment:用户自定义OCSC参数使能*/
    HI_BOOL bOCSCParamReload;	/**User-defined OCSC parameter reload enable*//**CNcomment:重新加载用户自定义OCSC参数使能*/
    HI_U32 u32ICSCParamAddr;		/**ICSC parameter address. The address must be 128-bit aligned.*//**CNcomment:ICSC参数地址，要求128bit对齐*/
    HI_U32 u32OCSCParamAddr;	/**OCSC parameter address. The address must be 128-bit aligned.*//**CNcomment:OCSC参数地址，要求128bit对齐*/
}TDE2_CSC_OPT_S;

/**Definition of blit operation options*/
/**CNcomment:blit操作选项定义 */
typedef struct hiTDE2_OPT_S
{
    TDE2_ALUCMD_E enAluCmd;                 /**<Logical operation type*//**<CNcomment:逻辑运算类型*/

    TDE2_ROP_CODE_E enRopCode_Color;        /**<ROP type of the color space*//**<CNcomment:颜色空间ROP类型*/

    TDE2_ROP_CODE_E enRopCode_Alpha;        /**<ROP type of the alpha component*//**<CNcomment:Alpha的ROP类型*/

    TDE2_COLORKEY_MODE_E enColorKeyMode;    /**<Colorkey mode*//**<CNcomment:color key方式*/

    TDE2_COLORKEY_U unColorKeyValue;        /**<Colorkey value*//**<CNcomment:color key设置值*/

    TDE2_CLIPMODE_E enClipMode;             /**<Perform the clip operation within or beyond the area*//**<CNcomment:区域内作clip还是区域外作clip*/

    TDE2_RECT_S stClipRect;                 /**<Definition of the clipping area*//**<CNcomment:clip区域定义*/

    TDE2_DEFLICKER_MODE_E enDeflickerMode;  /**<Anti-flicker mode*//**<CNcomment:抗闪烁模式*/

    HI_BOOL bResize;                        /**<Whether to scale*//**<CNcomment:是否缩放*/

    TDE2_FILTER_MODE_E enFilterMode;        /**<Filtering mode during scaling*//**< CNcomment:缩放时使用的滤波模式 */

    TDE2_MIRROR_E enMirror;                 /**<Mirror type*//**<CNcomment:镜像类型*/

    HI_BOOL bClutReload;                    /**<Whether to reload the CLUT*//**<CNcomment:是否重新加载Clut表*/

    HI_U8   u8GlobalAlpha;                  /**<Global alpha value*//**<CNcomment:全局Alpha值*/

    TDE2_OUTALPHA_FROM_E enOutAlphaFrom;    /**<Source of the output alpha*//**<CNcomment:输出alpha来源*/
    
    HI_U32 u32Colorize;                     /**<Colorize value*//**<CNcomment:Colorize值 */

    TDE2_BLEND_OPT_S stBlendOpt;

    TDE2_CSC_OPT_S stCscOpt;	
} TDE2_OPT_S;

/**Definition of macroblock operation options*/
/**CNcomment:宏块操作选项定义 */
typedef struct hiTDE2_MBOPT_S
{
    TDE2_CLIPMODE_E enClipMode;     /**<Clip mode*//**<CNcomment:Clip模式选择*/

    TDE2_RECT_S stClipRect;         /**<Definition of the clipping area*//**<CNcomment:clip区域定义*/

    HI_BOOL bDeflicker;             /**<Whether to perform anti-flicker*//**<CNcomment:是否抗闪烁*/

    TDE2_MBRESIZE_E enResize;       /**<Scaling information*//**<CNcomment:缩放信息*/

    HI_BOOL bSetOutAlpha;           /**<If the alpha value is not set, the maximum alpha value is output by default.*//**<CNcomment:如果不设置Alpha,则默认输出最大Alpha值*/
    
    HI_U8   u8OutAlpha;             /**<Global alpha for operation*//**<CNcomment:参加运算的全局alpha */
} TDE2_MBOPT_S;

/**Definition of the pattern filling operation*/
/**CNcomment:模式填充操作信息定义 */
typedef struct hiTDE2_PATTERN_FILL_OPT_S
{
    TDE2_ALUCMD_E enAluCmd;                 /**<Logical operation type*//**<CNcomment:逻辑运算类型*/

    TDE2_ROP_CODE_E enRopCode_Color;        /**<ROP type of the color space*//**<CNcomment:颜色空间ROP类型*/

    TDE2_ROP_CODE_E enRopCode_Alpha;        /**<ROP type of the alpha component*//**<CNcomment:alpha的ROP类型*/

    TDE2_COLORKEY_MODE_E enColorKeyMode;    /**<Colorkey mode*//**<CNcomment:color key方式*/

    TDE2_COLORKEY_U unColorKeyValue;        /**<Colorkey value*//**<CNcomment:color key值*/

    TDE2_CLIPMODE_E enClipMode;             /**<Clip mode*//**<CNcomment:clip模式*/

    TDE2_RECT_S stClipRect;                 /**<Clipping area*//**<CNcomment:clip区域*/

    HI_BOOL bClutReload;                    /**<Whether to reload the CLUT*//**<CNcomment:是否重载clut表*/

    HI_U8 u8GlobalAlpha;                    /**<Global alpha*//**<CNcomment:全局alpha*/

    TDE2_OUTALPHA_FROM_E enOutAlphaFrom;    /**<Source of the output alpha*//**<CNcomment:输出alpha来源*/

    HI_U32 u32Colorize;                     /**<Colorize value*//**<CNcomment:Colorize值*/

    TDE2_BLEND_OPT_S stBlendOpt;           /**<Options of the blending operation*//**<CNcomment:Blend操作选项 */

    TDE2_CSC_OPT_S stCscOpt;		/**<CSC parameter option*//**<CNcomment:Csc参数选项*/
    
}TDE2_PATTERN_FILL_OPT_S;

/**Definition of rotation directions*/
/**CNcomment:旋转方向定义 */
typedef enum hiTDE_ROTATE_ANGLE_E
{
    TDE_ROTATE_CLOCKWISE_90 = 0,    /**<Rotate 90° clockwise*//**< CNcomment:顺时针旋转90度 */
    TDE_ROTATE_CLOCKWISE_180,       /**<Rotate 180° clockwise*//**< CNcomment:顺时针旋转180度 */
    TDE_ROTATE_CLOCKWISE_270,       /**<Rotate 270° clockwise*//**< CNcomment:顺时针旋转270度 */
    TDE_ROTATE_BUTT
} TDE_ROTATE_ANGLE_E;

/**Definition of anti-flicker levels*/
/**CNcomment:抗闪烁级别定义 */
typedef enum hiTDE_DEFLICKER_LEVEL_E
{
    TDE_DEFLICKER_AUTO = 0, /**<Adaptation. The anti-flicker coefficient is selected by the TDE.*//**<CNcomment:自适应，由TDE选择抗闪烁系数*/
    TDE_DEFLICKER_LOW,      /**<Low-level anti-flicker*//**<CNcomment:低级别抗闪烁*/
    TDE_DEFLICKER_MIDDLE,   /**<Medium-level anti-flicker*//**CNcomment:中级别抗闪烁*/
    TDE_DEFLICKER_HIGH,     /**High-level anti-flicker*//**CNcomment:高级别抗闪烁*/
    TDE_DEFLICKER_BUTT
}TDE_DEFLICKER_LEVEL_E;

/* composed surface info */
typedef struct hiTDE_COMPOSOR_S
{
    TDE2_SURFACE_S stSrcSurface;
    TDE2_RECT_S stInRect;
    TDE2_RECT_S stOutRect;
    TDE2_OPT_S stOpt;
}TDE_COMPOSOR_S;

/* composed surface list */
typedef struct hiTDE_SURFACE_LIST_S
{
	HI_U32 u32SurfaceNum;
	TDE2_SURFACE_S *pDstSurface;
	TDE_COMPOSOR_S *pstComposor;
}TDE_SURFACE_LIST_S;


#ifdef __cplusplus
        #if __cplusplus
}
        #endif
   #endif /* __cplusplus */
#endif /*__HI_TDE_TYPE_H__ */
