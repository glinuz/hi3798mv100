/**
 \file
 \brief Describes the header file of the bit block transfer (Bliter) module.CNcomment:bliter头文件 CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \version 1.0
 \author s37678
 \date 2008-5-23
 */

#ifndef __HI_GO_BLITER_H__
#define __HI_GO_BLITER_H__

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/***************************** Macro Definition ******************************/


/****************************** Error Number ********************************/


/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_BLIT */
/** @{ */  /** <!-- [HIGO_BLIT] */

/** Pixel-level alpha blending operation*/
/** CNcomment: 像素级alpha混合操作运算 */
typedef enum
{
    HIGO_COMPOPT_NONE = 0, /**<Porter/Duff None-blending operation*//**<CNcomment:Porter/Duff None混合操作 */
    HIGO_COMPOPT_CLEAR,    /**<Porter/Duff clear blending operation*//**<CNcomment:Porter/Duff clear混合操作 */
    HIGO_COMPOPT_SRC     , /**<Porter/Duff SRC blending operation*//**<CNcomment:Porter/Duff Src混合操作 */ 
    HIGO_COMPOPT_SRCOVER , /**<Porter/Duff Srcover blending operation*//**<CNcomment:Porter/Duff Srcover混合操作 */ 
    HIGO_COMPOPT_DSTOVER , /**<Porter/Duff DstOver blending operation*/ /**<CNcomment:Porter/Duff DstOver混合操作 */ 
    HIGO_COMPOPT_SRCIN   , /**<Porter/Duff SrcIn blending operation*//**<CNcomment:Porter/Duff SrcIn混合操作 */ 
    HIGO_COMPOPT_DSTIN   , /**<Porter/Duff DstIn blending operation*/ /**<CNcomment:Porter/Duff DstIn混合操作 */ 
    HIGO_COMPOPT_SRCOUT  , /**<Porter/Duff SrcOut blending operation*//**<CNcomment:Porter/Duff SrcOut混合操作 */ 
    HIGO_COMPOPT_DSTOUT  , /**<Porter/Duff DstOut blending operation*/ /** CNcomment:Porter/Duff DstOut混合操作 */ 
    HIGO_COMPOPT_SRCATOP , /**<Porter/Duff SrcAtop blending operation*/ /**<CNcomment:Porter/Duff SrcAtop混合操作 */ 
    HIGO_COMPOPT_DSTATOP , /**<Porter/Duff DstAtop blending operation*//**<CNcomment: Porter/Duff DstAtop混合操作 */ 
    HIGO_COMPOPT_ADD     ,  /**<Porter/Duff DstAtop blending operation*/ /**<CNcomment: Porter/Duff DstAtop混合操作 */ 
    HIGO_COMPOPT_XOR     , /**<Porter/Duff Xor blending operation*/ /**<CNcomment: Porter/Duff Xor混合操作 */ 
    HIGO_COMPOPT_DST     , /**<Porter/Duff DstAtop blending operation*/ /**<CNcomment: Porter/Duff DstAtop混合操作 */ 
    HIGO_COMPOPT_AKS,      /**<Assume that the destination surface is not transparent. After the alpha blending is performed, the source alpha is retained.*//**<CNcomment: 假设目标surface为不透明，简单alpha混合，结果保留源alpha */
    HIGO_COMPOPT_AKD,      /**<Assume that the destination surface is not transparent. After the alpha blending is performed, the destination alpha is retained.*//**<CNcomment: 假设目标surface为不透明，简单alpha混合，结果保留目标alpha */

    HIGO_COMPOPT_BUTT
} HIGO_COMPOPT_E;

/** Operation mode corresponding to colorkey*//** CNcomment: Colorkey对应的操作方式 */
typedef enum
{
    HIGO_CKEY_NONE = 0, /**<Do not use the colorkey.*//**<CNcomment: 不使用colorkey */
    HIGO_CKEY_SRC,      /**<Use the source colorkey.*//**<CNcomment: 使用源colorkey */
    HIGO_CKEY_DST,     /**<Use the destination colorkey.*//**<CNcomment: 使用目标colorkey */

    HIGO_CKEY_BUTT
} HIGO_CKEY_E;

/** Two raster of operations (ROPs)*//** CNcomment: 2元ROP操作 */
typedef enum
{
    HIGO_ROP_BLACK = 0, /**< Blackness */
    HIGO_ROP_PSDon,     /**< ~(PS+D) */
    HIGO_ROP_PSDna,     /**< ~PS & D */
    HIGO_ROP_PSn,       /**< ~PS */
    HIGO_ROP_DPSna,     /**< PS & ~D */
    HIGO_ROP_Dn,        /**< ~D */
    HIGO_ROP_PSDx,      /**< PS^D */
    HIGO_ROP_PSDan,     /**< ~(PS&D) */
    HIGO_ROP_PSDa,      /**< PS & D */
    HIGO_ROP_PSDxn,     /**< ~(PS^D) */
    HIGO_ROP_D,         /**< D */
    HIGO_ROP_PSDno,     /**< ~PS + D */
    HIGO_ROP_PS,        /**< PS */
    HIGO_ROP_DPSno,     /**< PS + ~D */
    HIGO_ROP_PSDo,      /**< PS+D */
    HIGO_ROP_WHITE,     /**< Whiteness */

    HIGO_ROP_BUTT
} HIGO_ROP_E;

typedef enum
{
    HIGO_ROTATE_NONE = 0,
    HIGO_ROTATE_90,     /**<Rotate 90 degrees clockwise*//**<CNcomment: 顺时针旋转90度 */
    HIGO_ROTATE_180,    /**<Rotate 180 degrees clockwise*//**<CNcomment: 顺时针旋转180度 */
    HIGO_ROTATE_270,    /**<Rotate 270 degrees clockwise*//**<CNcomment: 顺时针旋转270度 */

    HIGO_ROTATE_BUTT
} HIGO_ROTATE_E;

typedef enum
{
    HIGO_MIRROR_NONE = 0,

    HIGO_MIRROR_LR,     /**<Mirror the left and the right*//**<CNcomment: 左右镜像 */
    HIGO_MIRROR_TB,     /**<Mirror the top and the bottom*//**<CNcomment: 上下镜像 */

    HIGO_MIRROR_BUTT
} HIGO_MIRROR_E;

typedef struct 
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag*//**<CNcomment: 全局alpha使能标志 */
    HI_BOOL EnablePixelAlpha;       /**<Pixel alpha enable flag*//**<CNcomment: 像素alpha使能标志 */
    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**<CNcomment: 像素alpha操作 */
    HIGO_CKEY_E    ColorKeyFrom;    /**<Colorkey operation*//**<CNcomment:ColorKey操作 */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**<CNcomment: 启用ROP2操作 */
    HIGO_ROP_E     Rop;              /**<ROP2 operation type*//**<CNcomment: ROP2操作类型 */
    HIGO_ROP_E     RopAlpha;         /**<Type of the ROP alpha operation*//**<CNcomment: ROP alpha操作类型*/
 }HIGO_BLTOPT2_S;

/** CNcomment: blit操作属性 */
typedef struct
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag. If this flag is enabled, the PixelAlphaComp blending mode must be specified.*//**<CNcomment: 全局alpha使能标志,打开此开关之外还必须指定PixelAlphaComp的混合方式*/
    HI_BOOL EnablePixelAlpha;       /**<Pixel alpha enable flag*//**<CNcomment: 像素alpha使能标志 */
    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**<CNcomment: 像素alpha操作 */
    HIGO_CKEY_E    ColorKeyFrom;    /**<colorkey operation*//**<CNcomment: ColorKey操作 */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**<CNcomment: 启用ROP2操作 */
    HIGO_ROP_E     Rop;             /**<ROP2 operation type*//**<CNcomment: ROP2操作类型 */
    HIGO_ROP_E     RopAlpha;        /**<Type of the ROP alpha operation*/    /**<CNcomment: ROP alpha操作类型*/    
    HI_BOOL        EnableScale;      /**<Enable the scaling function*//**<CNcomment: 启用缩放 */
    HIGO_ROTATE_E  RotateType;      /**<Rotation type*//**<CNcomment: 旋转方式 */
    HIGO_MIRROR_E  MirrorType;      /**<Mirror type*//**<CNcomment: 镜像方式 */
} HIGO_BLTOPT_S;

typedef struct
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag*//**<CNcomment: 全局alpha使能标志 */

    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**<CNcomment: 像素alpha操作 */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**<CNcomment: 启用ROP2操作 */
    HIGO_ROP_E     RopColor;        /**<ROP2 operation type*//**<CNcomment: ROP2操作类型 */
	HIGO_ROP_E     RopAlpha;
		
} HIGO_MASKOPT_S;

/**Anti-flicker level*//** CNcomment: 抗闪烁级别 */
typedef enum
{
    HIGO_DEFLICKER_AUTO = 0, /*Anti-flicker level, ranging from low to high. The higher the level, the better the anti-flicker effect, but the more blurred the picture.*//** CNcomment:抗闪烁级别，值为LOW~HIGH,值越大抗闪烁效果越好，但越模糊*/
    HIGO_DEFLICKER_LOW,
    HIGO_DEFLICKER_MIDDLE,
    HIGO_DEFLICKER_HIGH,
    HIGO_DEFLICKER_BUTT
}HIGO_DEFLICKEROPT_E;

/**Anti-flicker level*//** CNcomment: 抗闪烁级别 */
typedef struct
{
    HIGO_DEFLICKEROPT_E DefLevel;
}HIGO_DEFLICKEROPT_S;
/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_BLIT */
/** @{ */  /** <!-- 【HIGO_BLIT】 */

/** 
\brief Initializes the Bliter module.CNcomment: 初始化Bliter模块 CNend
\attention \n
When ::HI_GO_Init is called, this application programming interface (API) is also called.CNcomment: ::HI_GO_Init中已包含了对该接口的调用 CNend
\param  N/A.CNcomment: 无 CNend

\retval ::HI_SUCCESS  
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
::HI_GO_Init \n
::HI_GO_DeinitBliter
*/
HI_S32 HI_GO_InitBliter(HI_VOID);

/** 
\brief Deinitializes the Bliter module.CNcomment:去初始化Bliter模块 CNend
\attention \n
When ::HI_GO_Deinit is called, this API is also called.CNcomment:::HI_GO_Deinit中已包含了对该接口的调用 CNend
\param N/A. CNcomment:无 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_Deinit \n
::HI_GO_InitBliter
*/

HI_S32 HI_GO_DeinitBliter(HI_VOID);

/**  
\brief Fills in a rectangle.CNcomment:矩形填充 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] Surface Surface handle.CNcomment:Surface句柄 CNend
\param[in] pRect Size of the rectangle to be filled in. If the parameter is not set, it indicates that the entire 
surface is filled in.CNcomment: 填充矩形大小，为空表示填充整个surface CNend
\param[in] Color Color Fill color. For the RGB format, 32-bit color is filled in; for the palette, the color index (0-255) 
is filled in.CNcomment:填充颜色,注意如果是RGB格式，统一填充32位色，如果调色板，就直接填充颜色索引(0 ~ 255)。CNend 
\param[in] CompositeOpt Blending mode.CNcomment:混合方式 CNend

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_OUTOFPAL
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_FillRect(HI_HANDLE Surface, const HI_RECT* pRect, HI_COLOR Color, HIGO_COMPOPT_E CompositeOpt);

/**  
\brief Transfers bit blocks. During the transfer, color space conversion (CSC), scaling, and rotation are supported.CNcomment:位块搬移，在搬移过程中，可以实现色彩空间转换、缩放、旋转功能 CNend
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
Scaling, rotation, and mirror cannot be combined. \n
For scaling, rotation, and mirror, the source and destination pixel formats must be the same, but the format cannot 
be YUV or CLUT.\n
CNcomment:色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP\
缩放、旋转或镜像不可组合使用 \n
缩放、旋转或镜像要求源与目标像素格式完全相同，但不能是YUV格式和CLUT格式 \n CNend

\param[in] SrcSurface Source surface handle.CNcomment:源surface句柄 CNend
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:搬移的源区域，为空表示整个源surface区域 CNend
\param[in] DstSurface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pBlitOpt Blending mode.CNcomment:混合方式 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_Blit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                   HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                   const HIGO_BLTOPT_S* pBlitOpt);

/**  
\brief Draws a line segment. Cropping is not supported.CNcomment:绘制线段，不支持裁减 CNend
\attention \n
Cropping is not supported. Users must keep the entire line within the surface region.
CNcomment:不支持裁减，使用者必须保证整条直线在surface区域内 CNend 

\param[in] Surface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] x0 Horizontal coordinate of the start point.CNcomment:起点x坐标 CNend
\param[in] y0 Vertical coordinate of the start point.CNcomment:起点y坐标 CNend
\param[in] x1 Horizontal coordinate of the end point.CNcomment:终点x坐标 CNend
\param[in] y1 Vertical coordinate of the end point .CNcomment:终点y坐标 CNend 
\param[in] color Line segment color.CNcomment:线段颜色 CNend

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_DrawLine(HI_HANDLE Surface, HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_COLOR color);

/**  
\brief Draws an ellipse.CNcomment:绘制椭圆 CNend
\attention \n
Cropping is not supported. Users must keep the entire ellipse within the surface region.
CNcomment:不支持裁减，使用者必须保证整个椭圆在surface区域内 CNend

\param[in] Surface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] sx Horizontal coordinate of the ellipse center.CNcomment:圆心x坐标 CNend
\param[in] sy Vertical coordinate of the ellipse center.CNcomment:圆心y坐标 CNend
\param[in] rx X axis radius .CNcomment:x轴半径 CNend 
\param[in] ry Y axis radius.CNcomment:y轴半径 CNend
\param[in] color Ellipse color.CNcomment:椭圆颜色 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_DrawEllipse(HI_HANDLE Surface, HI_S32 sx, HI_S32 sy, HI_S32 rx, HI_S32 ry, HI_COLOR color);

/**  
\brief Draws a circle. Cropping is not supported.CNcomment:绘制圆，不支持裁减 CNend
\attention \n
Cropping is not supported. Users must keep the entire circle within the surface region.
CNcomment:不支持裁减，使用者必须保证整个圆在surface区域内 CNend

\param[in] Surface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] x Horizontal coordinate of the circle center.CNcomment:圆心x坐标 CNend
\param[in] y Vertical coordinate of the circle center.CNcomment:圆心y坐标 CNend
\param[in] r Radius.CNcomment:半径 CNend
\param[in] color Circle color. The circle is filled in with 32-bit colors.CNcomment:圆颜色, 颜色按照32位色进行填充 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_DrawCircle(HI_HANDLE Surface, HI_S32 x, HI_S32 y, HI_S32 r, HI_COLOR color);

/**  
\brief Draws a rectangle.CNcomment:绘制矩形 CNend
\attention \n
If the rectangle is empty, it indicates that the rectangle size is the same as surface.
Cropping is not supported; therefore, you must ensure that the entire rectangle is within the surface.
CNcomment:矩形为空表示输出矩形大小与surface相同
不支持裁减，使用者必须保证整个矩形在surface区域内 CNend

\param[in] Surface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] pRect Rectangle region.CNcomment:矩形区域 CNend
\param[in] color Rectangle color.CNcomment:矩形颜色 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_DrawRect(HI_HANDLE Surface, const HI_RECT* pRect, HI_COLOR color);

/**  
\brief Performs the mask ROP or mask blend transfer on the raster bitmap.CNcomment:对光栅位图进行Mask Rop或者Mask Blend搬移操作 CNend
\attention \n
The mask bitmap is in A1 or A8 format.
You need to overlay the mask bitmap with the source bitmap, overlay the result with the destination bitmap, and then 
output the final result to the destination bitmap.
If the ROP and blending operation are perform at the same time, only the ROP takes effect.
CNcomment:Mask是A1或A8的位图Surface。 
首先将Mask位图与源位图做一次叠加，然后用叠加的结果和目标做叠加输出到目标位图 
Rop和Blend混合同时只能有一个是有效的，两者都选只有Rop生效 CNend

\param[in] SrcSurface Source surface handle.CNcomment:源surface句柄 CNend
\param[in] pSrcRect Source rectangle.CNcomment:源矩形 CNend
\param[in] DstSurface Destination surface handle.CNcomment:目标surface句柄 CNend
\param[in] pDstRect Destination rectangle.CNcomment:目标矩形 CNend
\param[in] MaskSurface Mask surface handle.CNcomment:MASK surface句柄 CNend
\param[in] pMaskRect Mask rectangle.CNcomment:MASK矩形 CNend
\param[in] pOpt Operation option.CNcomment:操作选项 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVRECT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:无 CNend
*/

HI_S32 HI_GO_MaskBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                      HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                      HI_HANDLE MaskSurface, const HI_RECT* pMaskRect,
                      const HIGO_MASKOPT_S* pOpt);

/**  
\brief Transfers bit blocks. During the transfer, CSC is supported.CNcomment:位块搬移，在搬移过程中，可以实现色彩空间转换 CNend
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
CNcomment: 色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP\ CNend

\param[in] SrcSurface Source surface handle.CNcomment:源surface句柄 CNend
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:搬移的源区域，为空表示整个源surface区域 CNend
\param[in] DstSurface Destination surface handle.CNcomment:目的surface句柄 CNend
\param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used. CNcomment:混合方式，参数为空使用默认参数操作 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_BitBlit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                       HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                       const HIGO_BLTOPT2_S* pBlitOpt);

/**  
\brief Transfers bit blocks. During the transfer, CSC and scaling are supported. CNcomment:位块搬移，在搬移过程中，可以实现色彩空间转换, 缩放 CNend
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
CNcomment:色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP\ CNend

\param[in] SrcSurface  Source surface handle. CNcomment:源surface句柄 CNend
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:搬移的源区域，为空表示整个源surface区域 CNend
\param[in] DstSurface Destination surface handle. CNcomment:目的surface句柄 CNend
\param[in] pDstRect pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface. CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used. CNcomment:混合方式,参数为空使用默认参数操作 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_StretchBlit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                       HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                       const HIGO_BLTOPT2_S* pBlitOpt);

/** 
\brief  Fills in the pattern.CNcomment:进行模式填充 CNend
\attention \n
\param[in] SrcSurface  Source surface handle. CNcomment:源surface句柄 CNend
\param[in] pSrcRect  Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:搬移的源区域，为空表示整个源surface区域 CNend
\param[in] DstSurface  Destination surface handle. CNcomment:目的surface句柄 CNend
\param[in] pDstRect  Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pParOpt  Blending mode. If the parameter is not set, default settings are used. CNcomment:混合方式,参数为空使用默认参数操作 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_PatternBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect, 
                              HI_HANDLE DstSurface, const HI_RECT * pDstRect, 
                              const HIGO_BLTOPT2_S* pParOpt);

/** 
\brief Performs anti-flicker and transfer. This API is used for the second anti-flicker.CNcomment:抗闪烁搬移，这个接口用于第二次抗闪烁，CNend
\attention \n
This API is used in the following two application scenarios:
1. The automatic anti-flicker effect of a graphics layer is poor.
2. The buffer mode of a graphics layer is single-buffer mode (HIGO_LAYER_BUFFER_SINGLE). In this case, users need to 
perform anti-flicker by themselves.
When the sizes of the source bitmap and destination bitmap are different, scaling is performed automatically.
CNcomment:两种情况用到该接口 
1 当图层的自动抗闪烁效果不是很好时，可以使用这个接口再做一次。
2 当图层的buffer模式是单buffer模式(HIGO_LAYER_BUFFER_SINGLE)时，用户需要自己做抗闪烁，
当源和目标大小不同的时候自动进行缩放。CNend

\param[in] SrcSurface  SrcSurface Source surface handle.CNcomment: 源surface句柄 CNend
\param[in] pSrcRect  pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:搬移的源区域，为空表示整个源surface区域 CNend
\param[in] DstSurface  DstSurface Destination surface handle. CNcomment:目的surface句柄 CNend
\param[in] pDstRect  pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface. CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pDefOpt  pDefOpt Anti-flicker level. If this parameter is not set, it indicates the automatic level. CNcomment:抗闪烁级别选项，为空表示AUTO 级别 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_DeflickerBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect, 
                                 HI_HANDLE DstSurface, const HI_RECT * pDstRect, 
                                 const HIGO_DEFLICKEROPT_S* pDefOpt);

/**  
\brief Do the alpha blending between the forground surface and the backgroud surface, transfers the result to the destination surface. The background surface and the destination surface must be in the RGB color space. 
You can performs color space coversion, scale, or mirror, colorkey with alpha blending.CNcomment:该函数实现前景和背景叠加输出到目标功能，背景和目标surface必须位于RGB空间，在叠加过程中可以附加颜色空间转换、缩放、镜像等操作，不支持旋转。CNend
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported, rotation is not supported.\
Scaling, and mirror cannot be combined. \n
For scaling, and mirror, the source and destination pixel formats must be the same, but the format cannot 
be YUV or CLUT.\n
CNcomment:色彩空间转换仅支持YUV到RGB转换 \n
可以支持操作如下colorkey、alpha、ROP、colorkey+ROP，不支持缩放\
缩放、镜像不可组合使用 \n
缩放或镜像要求源与目标像素格式完全相同，但不能是YUV格式和CLUT格式 \n CNend

\param[in] BckSurface background surface handle.CNcomment:背景surface句柄 CNend
\param[in] pBckRect backgound region for transfer. If the parameter is not set, it indicates the entire background surface.CNcomment:背景surface操作区域，为空表示整个背景surface区域 CNend
\param[in] ForSurface forground surface handle.CNcomment:前景surface句柄 CNend
\param[in] pForRect forground region for transfer. If the parameter is not set, it indicates the entire forground surface. 
surface.CNcomment:前景surface操作区域，为空表示整个前景surface区域 CNend
\param[in] DstSurface destination surface handle.CNcomment:目标surface句柄 CNend
\param[in] pDstRect destination region for transfer. If the parameter is not set, it indicates the entire destination surface. 
surface.CNcomment:搬移的目的区域，为空表示整个目标surface区域 CNend
\param[in] pBlitOpt Blending mode.CNcomment:混合方式 CNend

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:无 CNend
*/
HI_S32 HI_GO_Blit3Source(HI_HANDLE BckSurface, const HI_RECT* pBckRect,
                   HI_HANDLE ForSurface, const HI_RECT* pForRect,
                   HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                   const HIGO_BLTOPT_S* pBlitOpt);
/**  
\brief Fills in a rounded rectangle.CNcomment:圆角矩形填充 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[in] Surface Surface handle.CNcomment:Surface句柄 CNend
\param[in] pRect Size of the rectangle to be filled in. If the parameter is not set, it indicates that the entire 
surface is filled in.CNcomment: 填充矩形大小，为空表示填充整个surface CNend
\param[in] Color Color Fill color. For the RGB format, 32-bit color is filled in; for the palette, the color index (0-255) 
is filled in.CNcomment:填充颜色,注意如果是RGB格式，统一填充32位色，如果调色板，就直接填充颜色索引(0 ~ 255)。CNend 
\param[in] s32Radius radius.CNcomment:圆角半径 CNend

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_OUTOFPAL
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\see \n
N/A. CNcomment:无 CNend
*/

HI_S32 HI_GO_FillRoundRect(HI_HANDLE Surface, const HI_RECT* pRect, HI_COLOR Color, HI_S32 s32Radius);

/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /* __HI_GO_BLITER_H__ */
