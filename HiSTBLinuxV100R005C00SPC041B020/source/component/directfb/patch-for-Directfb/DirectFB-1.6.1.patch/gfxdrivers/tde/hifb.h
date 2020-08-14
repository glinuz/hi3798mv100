/** 
 * \file
 * \brief Describes the information about HiSilicion frame buffer (HiFB). CNcomment:提供HIBF的相关信息
 */
 
#ifndef __HIFB_H__
#define __HIFB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



#include <linux/fb.h>

#include "hi_type.h"
#include "hi_common.h"



/*************************** Macro Definition ****************************/
/** \addtogroup           HIFB   */
/** @{ */  /** <!-- [HIFB] */
#define IOC_TYPE_HIFB       'F'

/**Obtains the colorkey of a graphics layer.*/
/**CNcomment:获取图层colorkey信息 */
#define FBIOGET_COLORKEY_HIFB       _IOR(IOC_TYPE_HIFB, 90, HIFB_COLORKEY_S)

/**Sets the colorkey of a graphics layer.*/
/**CNcomment:设置图层colorkey信息 */
#define FBIOPUT_COLORKEY_HIFB       _IOW(IOC_TYPE_HIFB, 91, HIFB_COLORKEY_S)

/**Obtains the alpha values of a graphics layer, including the pixel alpha and global alpha.*/
/**CNcomment:获取图层alpha信息,包括像素和全局alpha */
#define FBIOGET_ALPHA_HIFB          _IOR(IOC_TYPE_HIFB, 92, HIFB_ALPHA_S)

/**Sets the alpha values of a graphics layer, including the pixel alpha and global alpha.*/
/**CNcomment:设置图层alpha信息,包括像素和全局alpha */
#define FBIOPUT_ALPHA_HIFB          _IOW(IOC_TYPE_HIFB, 93, HIFB_ALPHA_S)

/**Obtains the origin of a graphics layer.*/
/**CNcomment:获取图层屏幕原点信息 */
#define FBIOGET_SCREEN_ORIGIN_HIFB  _IOR(IOC_TYPE_HIFB, 94, HIFB_POINT_S)

/**Sets the origin of a graphics layer.*/
/**CNcomment:设置图层屏幕原点信息 */
#define FBIOPUT_SCREEN_ORIGIN_HIFB  _IOW(IOC_TYPE_HIFB, 95, HIFB_POINT_S)

/**Obtains the anti-flicker status of a graphics layer.*/
/**CNcomment:获取图层抗闪烁*/
#define FBIOGET_DEFLICKER_HIFB       _IOR(IOC_TYPE_HIFB, 98, HIFB_DEFLICKER_S)

/**Sets the anti-flicker status of a graphics layer.*/
/**CNcomment:设置图层抗闪烁 */
#define FBIOPUT_DEFLICKER_HIFB       _IOW(IOC_TYPE_HIFB, 99, HIFB_DEFLICKER_S)

/**Waits for the vertical blanking interrupt of a graphics layer.*/
/**CNcomment:等待图层垂直水隐中断 */
#define FBIOGET_VBLANK_HIFB         _IO(IOC_TYPE_HIFB, 100)

/**Sets to display a graphics layer.*/
/**CNcomment:设置图层显示 */
#define FBIOPUT_SHOW_HIFB           _IOW(IOC_TYPE_HIFB, 101, HI_BOOL)

/**Obtains the display information about a graphics layer.*/
/**CNcomment:获取图层显示显示信息 */
#define FBIOGET_SHOW_HIFB           _IOR(IOC_TYPE_HIFB, 102, HI_BOOL)

/**Obtains the capability of a graphics layer.*/
/**CNcomment:获取图层能力 */
#define FBIOGET_CAPABILITY_HIFB    _IOR(IOC_TYPE_HIFB, 103, HIFB_CAPABILITY_S)

/* crusor handle */
/* Attention:surface in cursor will be released by user*/
#define FBIOPUT_CURSOR_INFO		      _IOW(IOC_TYPE_HIFB, 104, HIFB_CURSOR_S *)
#define FBIOGET_CURSOR_INFO		      _IOW(IOC_TYPE_HIFB, 105, HIFB_CURSOR_S *)

#define FBIOPUT_CURSOR_STATE		  _IOW(IOC_TYPE_HIFB, 106, HI_BOOL *)
#define FBIOGET_CURSOR_STATE		  _IOW(IOC_TYPE_HIFB, 107, HI_BOOL *)

#define FBIOPUT_CURSOR_POS		      _IOW(IOC_TYPE_HIFB, 108, HIFB_POINT_S *)	
#define FBIOGET_CURSOR_POS		      _IOR(IOC_TYPE_HIFB, 109, HIFB_POINT_S *)

#define FBIOPUT_CURSOR_COLORKEY       _IOR(IOC_TYPE_HIFB, 110, HIFB_COLORKEY_S *)
#define FBIOGET_CURSOR_COLORKEY       _IOW(IOC_TYPE_HIFB, 111, HIFB_COLORKEY_S *)
#define FBIOPUT_CURSOR_ALPHA          _IOR(IOC_TYPE_HIFB, 112, HIFB_ALPHA_S *)
#define FBIOGET_CURSOR_ALPHA          _IOW(IOC_TYPE_HIFB, 113, HIFB_ALPHA_S *)

/** cursor will be separated from attached layer automatically if you attach cursor to another layer,that means
cursor can be attached to only one layer at any time*/
#define FBIOPUT_CURSOR_ATTCHCURSOR    _IOW(IOC_TYPE_HIFB, 114, HIFB_LAYER_ID_E *)
#define FBIOPUT_CURSOR_DETACHCURSOR   _IOW(IOC_TYPE_HIFB, 115, HIFB_LAYER_ID_E *)


/**Sets the information about a graphics layer.*/
/**CNcomment:设置图层信息 */
#define FBIOPUT_LAYER_INFO                _IOW(IOC_TYPE_HIFB, 120, HIFB_LAYER_INFO_S*)

/**Obtains the information about a graphics layer.*/
/**CNcomment:获取图层信息 */
#define FBIOGET_LAYER_INFO                _IOR(IOC_TYPE_HIFB, 121, HIFB_LAYER_INFO_S*)

/**Obtains a canvas buffer.*/
/**CNcomment:获取canvas buf */
#define FBIOGET_CANVAS_BUFFER             _IOR(IOC_TYPE_HIFB, 123, HIFB_BUFFER_S*) 

/**Refreshes a graphics layer.*/
/**CNcomment:刷新图层 */
#define FBIO_REFRESH                      _IOW(IOC_TYPE_HIFB, 124, HIFB_BUFFER_S*) 

/**sync refresh*/
#define FBIO_WAITFOR_FREFRESH_DONE        _IO(IOC_TYPE_HIFB, 125)

/** set Encoder picture delivery method*/
#define FBIOPUT_ENCODER_PICTURE_FRAMING   _IOW(IOC_TYPE_HIFB, 126, HIFB_STEREO_MODE_E*)
/** get Encoder picture delivery method */
#define FBIOGET_ENCODER_PICTURE_FRAMING   _IOR(IOC_TYPE_HIFB, 127, HIFB_STEREO_MODE_E*)

/*set 3D Stereo work mode*/
#define FBIOPUT_STEREO_MODE _IOW(IOC_TYPE_HIFB, 128, HIFB_STEREO_WORKMODE_E*)
/*get 3D Stereo work mode*/
#define FBIOGET_STEREO_MODE _IOW(IOC_TYPE_HIFB, 129, HIFB_STEREO_WORKMODE_E*)


/**Sets the output range of the screen.*/
/**CNcomment:设置屏幕输出范围*/
#define FBIOPUT_SCREENSIZE          _IOW(IOC_TYPE_HIFB, 130, HIFB_SIZE_S*)

/**Obtains the output range of the screen.*/
/**CNcomment:获取屏幕输出范围*/
#define FBIOGET_SCREENSIZE          _IOR(IOC_TYPE_HIFB, 131, HIFB_SIZE_S*)

/*set compression mode, only support pixel format argb8888, and stereo mode not support top and bottom*/
#define FBIOPUT_COMPRESSION  _IOW(IOC_TYPE_HIFB, 132, HI_BOOL*)
/*get compression mode */
#define FBIOGET_COMPRESSION  _IOR(IOC_TYPE_HIFB, 133, HI_BOOL*)

/**create scroll text*/
#define FBIO_SCROLLTEXT_CREATE   _IOW(IOC_TYPE_HIFB, 134, HIFB_SCROLLTEXT_CREATE_S*) 
/**send to scroll text buffer*/
#define FBIO_SCROLLTEXT_FILL     _IOW(IOC_TYPE_HIFB, 135, HIFB_SCROLLTEXT_DATA_S*) 
/**pause scroll text*/
#define FBIO_SCROLLTEXT_PAUSE    _IOW(IOC_TYPE_HIFB, 136, HI_U32*) 
/**resume scroll text*/
#define FBIO_SCROLLTEXT_RESUME   _IOW(IOC_TYPE_HIFB, 137, HI_U32*) 
/**destory scroll text*/
#define FBIO_SCROLLTEXT_DESTORY  _IOW(IOC_TYPE_HIFB, 138, HI_U32*)

/*set fb stereo depth*/
#define FBIOPUT_STEREO_DEPTH  _IOW(IOC_TYPE_HIFB, 139, HI_U32*)
/*get fb stereo depth*/
#define FBIOGET_STEREO_DEPTH  _IOR(IOC_TYPE_HIFB, 140, HI_U32*)

/**set the priority of layer in gp*/
#define FBIOPUT_ZORDER          _IOW(IOC_TYPE_HIFB, 141, HIFB_ZORDER_E *)
/**gett the priority of layer in gp*/
/**default setting:
	HD0: G0(hifb0) ---- zorder == 1  -->bottom
	HD0: G1(hifb1) ---- zorder == 2
	HD0: G2(hifb2) ---- zorder == 3 -->top
	--------------------------
	HD1: G4(hifb4) ---- zorder == 1*/
#define FBIOGET_ZORDER          _IOW(IOC_TYPE_HIFB, 142, HI_U32 *)

/**free logo*/
#define FBIO_FREE_LOGO          _IO(IOC_TYPE_HIFB, 143)

/** @} */  /** <!-- ==== Macro Definition end ==== */



/*************************** Structure Definition ****************************/
/** \addtogroup           HIFB   */
/** @{ */  /** <!-- [HIFB] */


/**Definition of the range type*/
/**CNcomment:范围类型定义*/
typedef struct
{
    HI_U32  u32Width;         /**<Output width of the screen*//**<CNcomment:屏幕上输出的宽度 */
    HI_U32  u32Height;        /**<Output height of the screen*//**<CNcomment:屏幕上输出的高度 */
}HIFB_SIZE_S;

/**layer ID */
/**CNcomment:图层ID定义*/
typedef enum 
{
    HIFB_LAYER_HD_0 = 0x0,
    HIFB_LAYER_HD_1,
    HIFB_LAYER_HD_2,
    HIFB_LAYER_HD_3,
    HIFB_LAYER_SD_0,    
    HIFB_LAYER_SD_1,    
    HIFB_LAYER_SD_2,     
    HIFB_LAYER_SD_3, 
    HIFB_LAYER_AD_0,  
    HIFB_LAYER_AD_1,
    HIFB_LAYER_AD_2,
    HIFB_LAYER_AD_3,
    HIFB_LAYER_CURSOR,
    HIFB_LAYER_ID_BUTT
} HIFB_LAYER_ID_E;


/**Obtains the extended 32-bit color value based on color components. The upper bits are padded with lower bits during color extension.*/
/**CNcomment:根据颜色分量，获取其扩展到32位后值。扩展规则:低位补其高位*/
static inline HI_U8  hifb_rgb(const struct fb_bitfield* pBit, HI_S32 color)
{
    return ((HI_U8)((((HI_U32)color)>>pBit->offset) << (8-pBit->length)) +
             ((HI_U8)(((HI_U32)(color)>>pBit->offset) << (8-pBit->length)) >> pBit->length));
}

/**Converts a colorkey to a 32-bit colorkey based on the pixel format. A lower-bit color (such as 16 bits) is extended to an upper-bit color (such as 32 bits) as follows:
   For each chip, upper bits can be padded with lower bits in multiple mode. The padding mode of the chip must be consistent with the preceding extension mode. Otherwise, the colorkey of the graphics layer is invalid.*/
/**CNcomment:根据像素格式信息，将一个颜色值，转换成一个32位的颜色key值,将低色深如16bit扩展为高色深如32bit的策略是:
   低位补高几位，芯片有多种方式。芯片的方式必须如这个方式一致，否则图层colorkey无效*/
static inline HI_S32 hifb_color2key(const struct fb_var_screeninfo* pVar, HI_S32 color)
{
   if (pVar->bits_per_pixel <= 8)
   {
       return color; 
   }
   else
   {
      HI_U8 r, g, b;
      r = hifb_rgb(&pVar->red, color);
      g = hifb_rgb(&pVar->green, color);      
      b = hifb_rgb(&pVar->blue, color);
      return (r<<16) + (g<<8) + b;
   }
}


typedef struct
{
    HI_BOOL bKeyEnable;         /**<Colorkey enable*//**<CNcomment:colorkey 是否使能 */
    HI_U32 u32Key;              /**<The value is obtained by calling ::hifb_color2key.*//**<CNcomment:该值应该通过::hifb_color2key得到*/
}HIFB_COLORKEY_S;

/**Rectangle information*/
/**CNcomment:矩形信息 */
typedef struct
{
    HI_S32 x, y;    /**<x: horizontal coordinate of the upper left point of the rectangle; y: vertical coordinate of the upper left point of the rectangle*//**<CNcomment: x:矩形左上点横坐标 y:矩形左上点纵坐标*/
    HI_S32 w, h;    /**< w: rectangle width; h: rectangle height*//**<CNcomment: w:矩形宽度 h:矩形高度*/
} HIFB_RECT;

typedef struct
{
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
}HIFB_REGION;

/**Coordinate of a point*/
/**CNcomment:点坐标信息 */
typedef struct
{
    HI_S32 s32XPos;         /**<  horizontal position *//**<CNcommnet:水平位置 */
    HI_S32 s32YPos;         /**<  vertical position *//**<CNcomment:垂直位置 */
}HIFB_POINT_S;

/**Anti-flicker information*/
/**CNcommnet:抗闪烁信息 */
typedef struct hiHIFB_DEFLICKER_S
{
    HI_U32  u32HDfLevel;    /**<  horizontal deflicker level *//**<CNcomment:水平抗闪烁级别 */     
    HI_U32  u32VDfLevel;    /**<  vertical deflicker level *//**<CNcomment:垂直抗闪烁级别 */
    HI_U8   *pu8HDfCoef;    /**<  horizontal deflicker coefficient *//**<CNcomment:水平抗闪烁系数 */
    HI_U8   *pu8VDfCoef;    /**<  vertical deflicker coefficient *//**<CNcomment:垂直抗闪烁系数 */
}HIFB_DEFLICKER_S;

/**Alpha information*/
/**CNcomment:Alpha信息 */
typedef struct
{
    HI_BOOL bAlphaEnable;   /**<  alpha enable flag *//**<CNcomment:alpha使能标识*/
    HI_BOOL bAlphaChannel;  /**<  alpha channel enable flag *//**<CNcomment:alpha通道使能*/
    HI_U8 u8Alpha0;         /**<  alpha0 value, used in ARGB1555 *//**CNcomment:alpha0取值，ARGB1555格式时使用*/
    HI_U8 u8Alpha1;         /**<  alpha1 value, used in ARGB1555 *//**CNcomment:alpha1取值，ARGB1555格式时使用*/
    HI_U8 u8GlobalAlpha;    /**<  global alpha value *//**<CNcomment:全局alpha取值*/
    HI_U8 u8Reserved;
}HIFB_ALPHA_S;

/**Pixel format*/
/**CNcomment:像素格式枚举 */
typedef enum
{        
    HIFB_FMT_RGB565 = 0,        
    HIFB_FMT_RGB888,		      /**<  RGB888 24bpp */

    HIFB_FMT_KRGB444,       /**<  RGB444 16bpp */
    HIFB_FMT_KRGB555,       /**<  RGB555 16bpp */
    HIFB_FMT_KRGB888,       /**<  RGB888 32bpp */

    HIFB_FMT_ARGB4444,      /**< ARGB4444 */     
    HIFB_FMT_ARGB1555,      /**< ARGB1555 */  
    HIFB_FMT_ARGB8888,      /**< ARGB8888 */   
    HIFB_FMT_ARGB8565,      /**< ARGB8565 */

    HIFB_FMT_RGBA4444,      /**< ARGB4444 */
    HIFB_FMT_RGBA5551,      /**< RGBA5551 */
    HIFB_FMT_RGBA5658,      /**< RGBA5658 */
    HIFB_FMT_RGBA8888,      /**< RGBA8888 */

    HIFB_FMT_BGR565,        /**< BGR565 */   
    HIFB_FMT_BGR888,        /**< BGR888 */   
    HIFB_FMT_ABGR4444,      /**< ABGR4444 */   
    HIFB_FMT_ABGR1555,      /**< ABGR1555 */   
    HIFB_FMT_ABGR8888,      /**< ABGR8888 */   
    HIFB_FMT_ABGR8565,      /**< ABGR8565 */      
    HIFB_FMT_KBGR444,       /**< BGR444 16bpp */
    HIFB_FMT_KBGR555,       /**< BGR555 16bpp */
    HIFB_FMT_KBGR888,       /**< BGR888 32bpp */

    HIFB_FMT_1BPP,          /**<  clut1 */
    HIFB_FMT_2BPP,          /**<  clut2 */    
    HIFB_FMT_4BPP,          /**<  clut4 */ 
    HIFB_FMT_8BPP,          /**< clut8 */
    HIFB_FMT_ACLUT44,       /**< AClUT44*/
    HIFB_FMT_ACLUT88,         /**< ACLUT88 */
    HIFB_FMT_PUYVY,         /**< UYVY */
    HIFB_FMT_PYUYV,         /**< YUYV */
    HIFB_FMT_PYVYU,         /**< YVYU */
    HIFB_FMT_YUV888,        /**< YUV888 */
    HIFB_FMT_AYUV8888,      /**< AYUV8888 */
    HIFB_FMT_YUVA8888,      /**< YUVA8888 */
    HIFB_FMT_BUTT
}HIFB_COLOR_FMT_E;

/**HiFB capability set*/
/**CNcomment:hifb能力集 */
typedef struct
{
    HI_BOOL bKeyAlpha;      /**< whether support colorkey alpha */
    HI_BOOL bGlobalAlpha;   /**< whether support global alpha */
    HI_BOOL bCmap;          /**< whether support color map */
    HI_BOOL bHasCmapReg;    /**< whether has color map register*/
    HI_BOOL bColFmt[HIFB_FMT_BUTT]; /**< support which color format */
    HI_BOOL bVoScale;       /**< support vo scale*/
    HI_BOOL bLayerSupported;    /**< whether support a certain layer, for example:x5 HD support HIFB_SD_0 not support HIFB_SD_1*/
    HI_BOOL bCompression;    /**< whether support compression */
    HI_BOOL bStereo;        /**< whether support 3D Stereo*/
    HI_U32  u32MaxWidth;    /**< the max pixels per line */
    HI_U32  u32MaxHeight;   /**< the max lines */
    HI_U32  u32MinWidth;    /**< the min pixels per line */
    HI_U32  u32MinHeight;   /**< the min lines */ 
    HI_U32  u32VDefLevel;   /**< vertical deflicker level, 0 means vertical deflicker is unsupported */
    HI_U32  u32HDefLevel;   /**< horizontal deflicker level, 0 means horizontal deflicker is unsupported  */
}HIFB_CAPABILITY_S;

/*refresh mode*/
typedef enum 
{
    HIFB_LAYER_BUF_DOUBLE = 0x0,         /**< 2 display buf in fb */
    HIFB_LAYER_BUF_ONE    = 0x1,         /**< 1 display buf in fb */
    HIFB_LAYER_BUF_NONE   = 0x2,         /**< no display buf in fb,the buf user refreshed will be directly set to VO*/    
    HIFB_LAYER_BUF_DOUBLE_IMMEDIATE=0x3, /**< 2 display buf in fb, each refresh will be displayed*/
    HIFB_LAYER_BUF_STANDARD = 0x4,       /**< standard refresh*/
    HIFB_LAYER_BUF_BUTT
} HIFB_LAYER_BUF_E;

/* surface info */
typedef struct
{
    HI_U32  u32PhyAddr;     /**<  start physical address */
    HI_U32  u32Width;       /**<  width pixels */
    HI_U32  u32Height;      /**<  height pixels */   
    HI_U32  u32Pitch;       /**<  line pixels */         
    HIFB_COLOR_FMT_E enFmt; /**<  color format */     
}HIFB_SURFACE_S;


/* refresh surface info */
typedef struct
{
    HIFB_SURFACE_S stCanvas;    
    HIFB_RECT UpdateRect;       /**< refresh region*/
}HIFB_BUFFER_S;

/* cursor info */
typedef struct
{
    HIFB_SURFACE_S stCursor;
    HIFB_POINT_S stHotPos;
} HIFB_CURSOR_S;

/** Encoder picture delivery method */
typedef enum
{
    HIFB_STEREO_MONO   = 0x0,             /**< Normal output to non-stereoscopic (3D) TV. No L/R content provided to TV*/
    HIFB_STEREO_SIDEBYSIDE_HALF,          /**< L/R frames are downscaled horizontally by 2 andpacked side-by-side into a single frame, left on lefthalf of frame*/
    HIFB_STEREO_TOPANDBOTTOM,             /**< L/R frames are downscaled vertically by 2 andpacked into a single frame, left on top*/
    HIFB_STEREO_FRMPACKING,               /**< one frames are copyed */    
    HIFB_STEREO_BUTT
}HIFB_STEREO_MODE_E;

/** 3D stereo mode*/
typedef enum
{
    HIFB_STEREO_WORKMODE_HW_FULL = 0x0,      /**< 3d stereo function use hardware and transfer full frame to vo, note: hardware doesn't support this mode if encoder picture delivery method is top and bottom */   
    HIFB_STEREO_WORKMODE_HW_HALF,            /**< 3d stereo function use hardware and transfer half frame to vo*/
    HIFB_STEREO_WORKMODE_SW_EMUL,            /**< 3d stereo function use software emulation */
    HIFB_STEREO_WORKMODE_BUTT
}HIFB_STEREO_WORKMODE_E;

/**antiflicker level*/
/**Auto means fb will choose a appropriate antiflicker level automatically according to the color info of map*/
typedef enum
{
    HIFB_LAYER_ANTIFLICKER_NONE = 0x0,	/**< no antiflicker*/
    HIFB_LAYER_ANTIFLICKER_LOW = 0x1,	/**< low level*/
    HIFB_LAYER_ANTIFLICKER_MIDDLE = 0x2,/**< middle level*/
    HIFB_LAYER_ANTIFLICKER_HIGH = 0x3,  /**< high level*/
    HIFB_LAYER_ANTIFLICKER_AUTO = 0x4,  /**< auto*/
    HIFB_LAYER_ANTIFLICKER_BUTT
}HIFB_LAYER_ANTIFLICKER_LEVEL_E;

/*layer info maskbit*/
typedef enum
{
    HIFB_LAYERMASK_BUFMODE = 0x1,           /**< Whether the buffer mode in HIFB_LAYER_INFO_S is masked when the graphics layer information is set.*//**<CNcomment:设置图层信息时，HIFB_LAYER_INFO_S中buf模式是否有效掩码 */
    HIFB_LAYERMASK_ANTIFLICKER_MODE = 0x2,  /**< Whether the anti-flicker mode is masked.*//**<CNcomment:抗闪烁模式是否有效掩码 */
    HIFB_LAYERMASK_POS = 0x4,               /**< Whether the graphics layer position is masked.*//**<CNcomment:图层位置是否有效掩码 */
    HIFB_LAYERMASK_CANVASSIZE = 0x8,        /**< Whether the canvas size is masked.*//**<CNcomment:canvassize是否有效掩码 */
    HIFB_LAYERMASK_DISPSIZE = 0x10,         /**< Whether the display size is masked.*//**<CNcomment:displaysize是否有效掩码 */
    HIFB_LAYERMASK_SCREENSIZE = 0x20,       /**< Whether the screen size is masked.*//**<CNcomment:screensize是否有效掩码 */
    HIFB_LAYERMASK_BMUL = 0x40,             /**< Whether the premultiplexed mode is masked.*//**<CNcomment:是否预乘是否有效掩码 */
    HIFB_LAYERMASK_BUTT
}HIFB_LAYER_INFO_MASKBIT;


/**layer info*/
typedef struct
{
    HIFB_LAYER_BUF_E BufMode;
    HIFB_LAYER_ANTIFLICKER_LEVEL_E eAntiflickerLevel;	
    HI_S32 s32XPos;           /**<  the x pos of origion point in screen */
    HI_S32 s32YPos;	          /**<  the y pos of origion point in screen */
    HI_S32 u32CanvasWidth;    /**<  the width of canvas buffer */
    HI_S32 u32CanvasHeight;   /**<  the height of canvas buffer */
    HI_U32 u32DisplayWidth;	  /**<  the width of display buf in fb.for 0 buf ,there is no display buf in fb, so it's effectless*/
    HI_U32 u32DisplayHeight;  /**<  the height of display buf in fb. */
    HI_U32 u32ScreenWidth;    /**<  the width of screen */
    HI_U32 u32ScreenHeight;   /**<  the height of screen */
    HI_BOOL bPreMul;          /**<  The data drawed in buf is premul data or not*/
    HI_BOOL bUseNewScreen;    /**<  whether use new screen*/
    HI_U32 u32Mask;			  /**<  param modify mask bit*/
}HIFB_LAYER_INFO_S;


typedef struct 
{
    HIFB_RECT           stRect;        /**< the position you wanted to show on the screen */
    HIFB_COLOR_FMT_E    ePixelFmt;     /**< the color format of scrolltext content */
    HI_U16              u16CacheNum;   /**< The cached buffer number for store scrolltext content */
    HI_U16              u16RefreshFreq;/**< The refresh frequency you wanted */
    HI_BOOL             bDeflicker;    /**< Whether enable antificker */
}HIFB_SCROLLTEXT_ATTR_S;

typedef struct
{
    HIFB_SCROLLTEXT_ATTR_S stAttr;/**< The scrolltext attribution */
    HI_U32  u32Handle;            /**< The output ID for the scrolltext */
}HIFB_SCROLLTEXT_CREATE_S;

typedef struct
{
    HI_U32  u32Handle;     /**< ID of the scrolltext */
    HI_U32  u32PhyAddr;    /**< The physical address of the scrolltext content buffer */
    HI_U8   *pu8VirAddr;   /**< The virtual address of the scrolltext content buffer */
    HI_U32  u32Stride;     /**< The stride of the scrolltext content buffer */
}HIFB_SCROLLTEXT_DATA_S;


#define GP_LAYER_NUM 4
/*virtual window info*/
typedef enum
{
    HIFB_ZORDER_MOVETOP = 0,  /**< Move to the top*//**<CNcomment:移到最顶部*/
    HIFB_ZORDER_MOVEUP,       /**< Move upwards*//**<CNcomment:向上移*/
    HIFB_ZORDER_MOVEBOTTOM,   /**< Move to the bottom*//**<CNcomment:移到最底部*/
    HIFB_ZORDER_MOVEDOWN,     /**< Move downwards*//**<CNcomment:向下移*/
    HIFB_ZORDER_BUTT
}HIFB_ZORDER_E;

/** @} */  /** <!-- ==== Structure Definition end ==== */


#if 0
/******************************* API declaration *****************************/
/** \addtogroup      H_1_3_2 */
/** @{ */  /** <!-- 【HiFB】 */

/**-----标准功能--------*/
/**
\brief Obtains the variable information about the screen. CNcomment:获取屏幕的可变信息.
\attention \n
By default, the resolution and pixel format of the standard-definition (SD) graphics layer are 720x576 and ARGB1555 respectively; the resolution and pixel format of the high-definition (HD) graphics layer are 1280x720 and ARGB888 respectively;
CNcomment:系统默认的标清图层分辨率为720×576，象素格式为ARGB1555。系统默认的高清图层分辨率为1280×720，象素格式为ARGB888。

\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_VSCREENINFO  ioctl number. CNcomment:ioctl号
\param[out] var  Pointer to the variable information. CNcomment:可变信息结构体指针
\retval 0  SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_VSCREENINFO
\par example
\code
struct fb_var_screeninfo vinfo;
if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
{
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOGET_VSCREENINFO, struct fb_var_screeninfo *var);


/**
\brief Sets the screen resolution and pixel format of the FB. CNcomment:设置Framebuffer的屏幕分辨率和象素格式等。
\attention \n
The resolution must be supported by each overlay layer. You can query the maximum resolution and minimum resolution supported by each overlay layer by calling FBIOGET_CAPABILITY_HIFB.\n
The sum of the actual resolution and offset value must be within the range of the virtual resolution. Otherwise, the system automatically adjusts the actual resolution.
CNcomment:分辨率的大小必须在各叠加层支持的分辨率范围内，各叠加层支持的最大分辨率和最小分辨率可通过FBIOGET_CAPABILITY_HIFB获取。\n
必须保证实际分辨率与偏移的和在虚拟分辨率范围内，否则系统会自动调整实际分辨率的大小让其在虚拟分辨率范围内。

\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_VSCREENINFO ioctl number. CNcomment:ioctl号
\param[in] var Pointer to the variable information. CNcomment:可变信息结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_VSCREENINFO
\par example
\code
//Assume that the actual resolution is 720x576, the virtual resolution is 720x576, the offset value is (0, 0), and the pixel format is ARGB8888. In this case, the sample code is as follows:
//CNcommnet:设置实际分辨率为720×576，虚拟分辨率为720×576，偏移为（0，0），象素格式为ARGB8888的示例代码如下：
struct fb_bitfield r32 = {16, 8, 0};
struct fb_bitfield g32 = {8, 8, 0};
struct fb_bitfield b32 = {0, 8, 0}; 
struct fb_bitfield a32 = {24, 8, 0};
struct fb_var_screeninfo vinfo;
if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
{
    return -1;
}
vinfo.xres_virtual = 720;
vinfo.yres_virtual = 576;
vinfo.xres = 720;
vinfo.yres = 576;
vinfo.activate = FB_ACTIVATE_NOW;
vinfo.bits_per_pixel = 32;
vinfo.xoffset = 0;
vinfo.yoffset = 0;
vinfo.red = r32;
vinfo.green = g32;
vinfo.blue = b32;
vinfo.transp= a32;
if (ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo) < 0)
{
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOPUT_VSCREENINFO, struct fb_var_screeninfo *var);



/**
\brief Obtains the fixed information about the FB. CNcomment:获取Framebuffer的固定信息。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_FSCREENINFO  ioctl number. CNcomment:ioctl号
\param[out] fix Pointer to the fixed information. CNcomment:固定信息结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
无
*/
int ioctl (int fd, FBIOGET_FSCREENINFO, struct fb_fix_screeninfo *fix);



/**
\brief Sets to display contents at different offset positions of the virtual resolution. CNcommnet:设置从虚拟分辨率中的不同偏移处开始显示。
\attention \n
The sum of the actual resolution and offset value must be within the range of the virtual resolution. Otherwise, the setting is unsuccessful.
CNcomment:必须保证实际分辨率与偏移的和在虚拟分辨率范围内，否则设置不成功。

\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPAN_DISPLAY  ioctl number. CNcomment:ioctl号
\param[out] var  Pointer to the variable information. CNcomment:可变信息结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid.  CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
无
\par example
\code
//Assume that the actual resolution is 300x300, the virtual resolution is 720x576, the start offset value is (50, 50), and the display position is (300, 0). In this case, the code for the PAN setting is as follows:
//CNcomment:设置实际分辨率为300×300，虚拟分辨率为720×576，起始偏移为（50，50），然后偏移到（300，0）处开始显示的PAN设置代码如下：
struct fb_bitfield r32 = {16, 8, 0};
struct fb_bitfield g32 = {8, 8, 0};
struct fb_bitfield b32 = {0, 8, 0};
struct fb_bitfield a32 = {24, 8, 0};
struct fb_var_screeninfo vinfo;

vinfo.xres_virtual = 720;
vinfo.yres_virtual = 576;
vinfo.xres = 300;
vinfo.yres = 300;
vinfo.activate = FB_ACTIVATE_NOW;
vinfo.bits_per_pixel = 32;
vinfo.xoffset = 50;
vinfo.yoffset = 50;
vinfo.red = r32;
vinfo.green = g32;
vinfo.blue = b32;
vinfo.transp= a32;
if (ioctl(fd, FBIOPUT_VSCREENINFO, &vinfo) < 0)
{
    return -1;
}
vinfo.xoffset = 300;
vinfo.yoffset = 0;
if (ioctl(fd, FBIOPAN_DISPLAY, &vinfo) < 0)
{
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOPAN_DISPLAY, struct fb_var_screeninfo *var);



/**
\brief Obtains the information about the palette. CNcommnet:获取调色板信息。
\attention \n
无

\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGETCMAP  ioctl number. CNcomment:ioctl号
\param[out] cmap Pointer to the palette. CNcomment:调色板结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUTCMAP
\par example
\code
//only low 8bits valid 
unsigned short Red256[256];   
//only low 8bits valid 
unsigned short Green256[256];  
// only low 8bits valid 
unsigned short Blue256[256];   
struct fb_cmap cmap;

cmap.start = 0;
cmap.len = 256;
cmap.red = Red256;
cmap.green = Green256;
cmap.blue = Blue256;
cmap.transp = 0;

if (ioctl(fd, FBIOGETCMAP, &cmap) < 0)
{
    printf("fb ioctl get cmap err!\n");
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOGETCMAP, struct fb_cmap *cmap);



/**
\brief Sets the information about the palette. CNcomment:设置调色板信息。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUTCMAP  ioctl number. CNcomment:ioctl号
\param[in] cmap Pointer to the palette. CNcomment:调色板结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGETCMAP
\par example
\code
//only low 8bits valid 
unsigned short Red256[256];     
//only low 8bits valid 
unsigned short Green256[256]; 
//only low 8bits valid 
unsigned short Blue256[256];    
struct fb_cmap cmap;

//create a palette which contains 256 color 
Palette_Create(Red256, Green256, Blue256);

cmap.start = 0;
cmap.len = 256;
cmap.red = Red256;
cmap.green = Green256;
cmap.blue = Blue256;
cmap.transp = 0;

if (ioctl(fd, FBIOPUTCMAP, &cmap) < 0)
{
    printf("fb ioctl put cmap err!\n");
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOPUTCMAP, struct fb_cmap *cmap);



/**-----Extended Functions--------*/
/**CNcomment:-----扩展功能--------*/
/**
\brief Obtains the capability of an overlay layer. CNcomment:获取叠加层的支持能力。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_CAPABILITY_HIFB  ioctl number. CNcomment:ioctl号
\param[out] pstCap Pointer to the capability. CNcomment:支持能力结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
无
*/
int ioctl (int fd, FBIOGET_CAPABILITY_HIFB, HIFB_CAPABILITY_S *pstCap);



/**
\brief Obtains the coordinates of the start display point of an overlay layer on the screen. CNcomment:获取叠加层在屏幕上显示的起始点坐标。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_SCREEN_ORIGIN_HIFB  ioctl number. CNcomment:ioctl号
\param[out] pstPoint Pointer to the coordinate origin. CNcomment:坐标原点结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_SCREEN_ORIGIN_HIFB
*/
int ioctl (int fd, FBIOGET_SCREEN_ORIGIN_HIFB, HIFB_POINT_S *pstPoint);



/**
\brief Sets the coordinates of the start display point of an overlay layer on the screen. CNcomment:设置叠加层在屏幕上显示的起始点坐标。
\attention \n
If the coordinate origin of an overlay layer is beyond the range, the coordinate origin is set to (u32MaxWidth - u32MinWidth, u32MaxHeight - u32MinHeight) by default.
CNcomment:如果叠加层坐标原点超出了范围，默认将坐标原点设置为（u32MaxWidth –u32MinWidth,，u32MaxHeight –u32MinHeight）。
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_SCREEN_ORIGIN_HIFB  ioctl number. CNcomment:ioctl号
\param[in] pstPoint Pointer to the coordinate origin. CNcomment:坐标原点结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_SCREEN_ORIGIN_HIFB
*/
int ioctl (int fd, FBIOPUT_SCREEN_ORIGIN_HIFB, HIFB_POINT_S *pstPoint);



/**
\brief Obtains the display status of the current overlay layer. CNcomment:获取当前叠加层的显示状态。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_SHOW_HIFB  ioctl number. CNcomment:ioctl号
\param[out] bShow Status of the current overlay layer. If *bShow is set to HI_TRUE, it indicates that the current overlay layer is shown; if *bShow is set to HI_FALSE, it indicates that the current overlay layer is hidden. CNcomment:指示当前叠加层的状态：*bShow = HI_TRUE：当前叠加层处于显示状态；*bShow = HI_FALSE：当前叠加层处于隐藏状态
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_SHOW_HIFB
*/
int ioctl (int fd, FBIOGET_SHOW_HIFB, HI_BOOL *bShow);



/**
\brief Shows or hides the current overlay layer. CNcomment:显示或隐藏该叠加层。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_SHOW_HIFB  ioctl number. CNcomment:ioctl号
\param[in] bShow Display status of the current overlay layer. If *bShow is set to HI_TRUE, it indicates that the current overlay layer is shown; if *bShow is set to HI_FALSE, it indicates that the current overlay layer is hidden. CNcomment:该叠加层的显示状态：*bShow = HI_TRUE：显示当前叠加层；*bShow = HI_FALSE：隐藏当前叠加层
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_SHOW_HIFB
*/
int ioctl (int fd, FBIOPUT_SHOW_HIFB, HI_BOOL *bShow);



/**
\brief Obtains the colorkey of an overlay layer. CNcomment:获取叠加层的colorkey。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_COLORKEY_HIFB  ioctl number. CNcomment:ioctl号
\param[out] pstColorKey Pointer to the colorkey. CNcomment:colorkey结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_COLORKEY_HIFB
*/
int ioctl (int fd, FBIOGET_COLORKEY_HIFB, HIFB_COLORKEY_S *pstColorKey);



/**
\brief Sets the colorkey of an overlay layer. CNcomment:设置叠加层的colorkey。
\attention \n
无
\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_COLORKEY_HIFB  ioctl number. CNcomment:ioctl号
\param[in] pstColorKey  Pointer to the colorkey. CNcomment:colorkey结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_COLORKEY_HIFB
\par example
\code
//Assume that the pixel format is ARGB8888. To filter the red color, perform the following setting:
//CNcomment:假设当前象素格式为ARGB8888，则要过滤掉红色,具体设置如下：
HIFB_COLORKEY_S stColorKey;

stColorKey.bKeyEnable = HI_TRUE;
stColorKey.u32Key = 0xFF0000;
if (ioctl(fd, FBIOPUT_COLORKEY_HIFB, &stColorKey) < 0)
{
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOPUT_COLORKEY_HIFB, HIFB_COLORKEY_S *pstColorKey);



/**
\brief Obtains the alpha value of an overlay layer. CNcomment:获取叠加层Alpha。
\attention \n
For details, see the description of ::HIFB_ALPHA_S.
CNcomment:请参见::HIFB_ALPHA_S的说明。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_ALPHA_HIFB  ioctl number. CNcomment:ioctl号
\param[out] pstAlpha  Pointer to the alpha value. CNcomment:Alpha结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_ALPHA_HIFB
*/
int ioctl (int fd, FBIOGET_ALPHA_HIFB, HIFB_ALPHA_S *pstAlpha);




/**
\brief Sets the alpha value of an overlay layer. CNcomment:设置叠加层的Alpha。
\attention \n
For details, see the description of ::HIFB_ALPHA_S.
CNcomment:请参见::HIFB_ALPHA_S的说明。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_ALPHA_HIFB  ioctl number. CNcomment:ioctl号
\param[in] pstAlpha  Pointer to the alpha value. CNcomment:Alpha结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_ALPHA_HIFB
*/
int ioctl (int fd, FBIOPUT_ALPHA_HIFB, HIFB_ALPHA_S *pstAlpha);



/**
\brief Obtains the anti-flicker setting of an overlay layer. The HD platform does not support this function. CNcomment:获取叠加层的抗闪烁设置。高清平台不支持设置。
\attention \n
Before obtaining the values of anti-flicker parameters, you must set the maximum anti-flicker level that can be obtained, and allocate sufficient memory for storing anti-flicker coefficients. This API is invalid for the HD platform currently.
CNcomment:在获取抗闪烁参数时，必须设置能够获取抗闪烁的最大级别，且为抗闪烁系数分配足够的内存。目前该接口在高清无效。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_DEFLICKER_HIFB  ioctl number. CNcomment:ioctl号
\param[out] pstDeflicker  Pointer to the anti-flicker setting. CNcomment:抗闪烁结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_DEFLICKER_HIFB
\par example
\code
//Assume that the maximum horizontal and vertical anti-flicker levels that can be obtained are 2. In this case, the sample code is as follows:
//CNcomment:获取水平和垂直抗闪烁最大级别为2的示例代码如下：
HI_U8 u8HDefCoef;
HI_U8 u8VDefCoef;
HIFB_DEFLICKER_S stDeflicker;

stDeflicker.u32HDfLevel = 2;
stDeflicker.u32VDfLevel = 2;
stDeflicker.pu8HDfCoef = &u8HDefCoef;
stDeflicker.pu8VDfCoef = &u8VDefCoef;

if (ioctl(fd, FBIOGET_DEFLICKER_HIFB, &stDeflicker) < 0)
{
    return -1;
}

\endcode
*/
int ioctl (int fd, FBIOGET_DEFLICKER_HIFB, HIFB_DEFLICKER_S *pstDeflicker);



/**
\brief Sets the anti-flicker function of an overlay layer. CNcomment:设置叠加层的抗闪烁功能。
\attention \n
For details, see the description of ::HIFB_DEFLICKER_S. This API is invalid for the HD platform currently.
CNcomment:请参见::HIFB_DEFLICKER_S的说明。目前该接口在高清无效。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_DEFLICKER_HIFB  ioctl number. CNcomment:ioctl号
\param[in] pstDeflicker  Pointer to the anti-flicker setting. CNcomment:抗闪烁结构体指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_DEFLICKER_HIFB
\par example
\code
//Assume that the 2-tap horizontal and vertical anti-flicker functions are required. In this case, the sample code is as follows:
//CNcomment:设置水平和垂直2阶抗闪烁的代码如下：
HI_U8 u8HDefCoef = 0x80;
HI_U8 u8VDefCoef = 0x80;
HIFB_DEFLICKER_S stDeflicker;

stDeflicker.u32HDfLevel = 2;
stDeflicker.u32VDfLevel = 2;
stDeflicker.pu8HDfCoef = &u8HDefCoef;
stDeflicker.pu8VDfCoef = &u8VDefCoef;

if (ioctl(fd, FBIOPUT_DEFLICKER_HIFB, &stDeflicker) < 0)
{
    return -1;
}
\endcode
*/
int ioctl (int fd, FBIOPUT_DEFLICKER_HIFB, HIFB_DEFLICKER_S *pstDeflicker);



/**
\brief Waits for the vertical blanking region of an overlay layer. To operate the display buffer without tearing, you can operate it in the vertical blanking region. CNcomment:为了操作显存时而不引起撕裂现象，一般可以在该叠加层的垂直消隐区对显存进行操作，通过该接口可以等待该叠加层垂直消隐区的到来。
\attention \n
无
\param[in] fd ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_VBLANK_HIFB  ioctl number. CNcomment:ioctl号
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
无
*/
int ioctl (int fd, FBIOGET_VBLANK_HIFB);


/**
\brief Obtains the stereo frame encode format. CNcomment:获取Stereo帧编码格式功能。
\attention \n
For details, see the description of ::HIFB_STEREO_MODE_E. 

\param[in] fd  Framebuffer设备号
\param[in] FBIOGET_STEREO_MODE  ioctl号
\param[out] penStereoMode  stereo模式枚举指针
\retval  0 SUCCESS 成功
\retval ::EPERM  1,不支持该操作
\retval ::ENOMEM  12,内存不够
\retval ::EFAULT  14,传入参数指针地址无效
\retval ::EINVAL  22,传入参数无效
\see \n
::FBIOPUT_STEREO_MODE
*/
int ioctl (int fd, FBIOGET_STEREO_MODE, HIFB_STEREO_MODE_E *penStereoMode);


/**
\brief 设置Stereo模式功能。
\attention \n
请参见::HIFB_STEREO_MODE_E的说明。

\param[in] fd  Framebuffer设备号
\param[in] FBIOPUT_STEREO_MODE  ioctl号
\param[in] penStereoMode  stereo模式枚举指针
\retval  0 SUCCESS 成功
\retval ::EPERM  1,不支持该操作
\retval ::ENOMEM  12,内存不够
\retval ::EFAULT  14,传入参数指针地址无效
\retval ::EINVAL  22,传入参数无效
\see \n
::FBIOGET_STEREO_MODE
*/
int ioctl (int fd, FBIOPUT_STEREO_MODE, HIFB_STEREO_MODE_E *penStereoMode);


/**
\brief 获取Stereo帧编码格式功能。
\attention \n
CNcomment:请参见::HIFB_ENCODER_PICTURE_FRAMING_E的说明。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOGET_STEREO_MODE  ioctl number. CNcomment:ioctl号
\param[out] penStereoMode  Pointer to the frame encode format. CNcomment:帧编码格式枚举指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOPUT_ENCODER_PICTURE_FRAMING
*/
int ioctl (int fd, FBIOGET_ENCODER_PICTURE_FRAMING, HIFB_STEREO_MODE_E *penEncPicFrm);


/**
\brief Sets the stereo frame encode format. CNcomment:设置Stereo帧编码格式功能。
\attention \n
For details, see the description of ::HIFB_STEREO_MODE_E. 
CNcomment:请参见::HIFB_ENCODER_PICTURE_FRAMING_E的说明。

\param[in] fd  ID of an FB device. CNcomment:Framebuffer设备号
\param[in] FBIOPUT_STEREO_MODE  ioctl number. CNcomment:ioctl号
\param[in] penStereoMode  Pointer to the frame encode format. CNcomment:帧编码格式枚举指针
\retval  0 SUCCESS Success. CNcomment:成功
\retval ::EPERM  1. The operation is not supported. CNcomment:1,不支持该操作
\retval ::ENOMEM  12. The memory is insufficient. CNcomment:12,内存不够
\retval ::EFAULT  14. The input pointer is invalid. CNcomment:14,传入参数指针地址无效
\retval ::EINVAL  22. The input parameter is invalid. CNcomment:22,传入参数无效
\see \n
::FBIOGET_ENCODER_PICTURE_FRAMING
*/
int ioctl (int fd, FBIOPUT_ENCODER_PICTURE_FRAMING, HIFB_STEREO_MODE_E *penEncPicFrm);


/** @} */  /** <!-- ==== API declaration end ==== */
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HIFB_H__ */

