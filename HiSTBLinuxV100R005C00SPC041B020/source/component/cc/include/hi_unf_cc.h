/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name       : hi_unf_cc.h
Version         : Initial draft
Author          :
Created Date    :
Last Modified by:
Description     :
Function List   :
Change History  :
******************************************************************************/

/**
 * \file
 * \brief Describes the information about CC module (CC).
*/

#ifndef _HI_UNF_CC_H_
#define _HI_UNF_CC_H_

#include "hi_type.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
#define ARIBCC_MAX_LANGUAGE 2

/*************************** Structure Definition ****************************/
/** \addtogroup      CC */
/** @{*/  /** <!-- [CC]*/

/**CC data type*//** CNcomment:CC数据类型分类 */
typedef enum hiUNF_CC_DATA_TYPE_E
{
    HI_UNF_CC_DATA_TYPE_608,  /**<CC608*//**<CNcomment:CC608数据 */
    HI_UNF_CC_DATA_TYPE_708,        /**<CC708*//**<CNcomment:CC708数据 */
    HI_UNF_CC_DATA_TYPE_ARIB,       /**<ARIB CC*//**<CNcomment:ARIB CC数据 */
    HI_UNF_CC_DATA_TYPE_BUTT
}HI_UNF_CC_DATA_TYPE_E;

/**708CC service channel*//** CNcomment:708CC的服务通道号*/
typedef enum hiUNF_CC_708_SERVICE_NUM_E
{
    HI_UNF_CC_708_SERVICE1 = 0x1, /**<708CC service 1*//**<CNcomment:708CC服务1 */
    HI_UNF_CC_708_SERVICE2,       /**<708CC service 2*//**<CNcomment:708CC服务2 */
    HI_UNF_CC_708_SERVICE3,       /**<708CC service 3*//**<CNcomment:708CC服务3 */
    HI_UNF_CC_708_SERVICE4,       /**<708CC service 4*//**<CNcomment:708CC服务4 */
    HI_UNF_CC_708_SERVICE5,       /**<708CC service 5*//**<CNcomment:708CC服务5 */
    HI_UNF_CC_708_SERVICE6,       /**<708CC service 6*//**<CNcomment:708CC服务6 */
    HI_UNF_CC_708_SERVICE_BUTT
}HI_UNF_CC_708_SERVICE_NUM_E;

/**CC608 data type*//** CNcomment:CC608在line 21的数据类型分类 */
typedef enum hiUNF_CC_608_DATATYPE_E
{
    HI_UNF_CC_608_DATATYPE_CC1,       /**<CC1*//**<CNcomment:CC1 */
    HI_UNF_CC_608_DATATYPE_CC2,       /**<CC2*//**<CNcomment:CC2 */
    HI_UNF_CC_608_DATATYPE_CC3,       /**<CC3*//**<CNcomment:CC3 */
    HI_UNF_CC_608_DATATYPE_CC4,       /**<CC4*//**<CNcomment:CC4 */
    HI_UNF_CC_608_DATATYPE_TEXT1,     /**<TEXT1*//**<CNcomment:TEXT1 */
    HI_UNF_CC_608_DATATYPE_TEXT2,     /**<TEXT2*//**<CNcomment:TEXT2 */
    HI_UNF_CC_608_DATATYPE_TEXT3,     /**<TEXT3*//**<CNcomment:TEXT3 */
    HI_UNF_CC_608_DATATYPE_TEXT4,     /**<TEXT4*//**<CNcomment:TEXT4 */
    HI_UNF_CC_608_DATATYPE_BUTT
}HI_UNF_CC_608_DATATYPE_E;

/**standard color*//** CNcomment:标准颜色 */
typedef enum hiUNF_CC_COLOR_E
{
    HI_UNF_CC_COLOR_DEFAULT=0x00000000,       /**<default color*//**<CNcomment:默认颜色 */
    HI_UNF_CC_COLOR_BLACK=0xff000000,         /**<black*//**<CNcomment:黑色 */
    HI_UNF_CC_COLOR_WHITE=0xffffffff,         /**<white*//**<CNcomment:白色 */
    HI_UNF_CC_COLOR_RED=0xffff0000,           /**<red*//**<CNcomment:红色 */
    HI_UNF_CC_COLOR_GREEN=0xff00ff00,         /**<green*//**<CNcomment:绿色 */
    HI_UNF_CC_COLOR_BLUE=0xff0000ff,          /**<blue*//**<CNcomment:蓝色 */
    HI_UNF_CC_COLOR_YELLOW=0xffffff00,        /**<yellow*//**<CNcomment:黄色 */
    HI_UNF_CC_COLOR_MAGENTA=0xffff00ff,       /**<magenta*//**<CNcomment:品红 */
    HI_UNF_CC_COLOR_CYAN=0xff00ffff,          /**<cyan*//**<CNcomment:青色 */
}HI_UNF_CC_COLOR_E;

/**opacity*//** CNcomment:透明度 */
typedef enum hiUNF_CC_OPACITY_E
{
    HI_UNF_CC_OPACITY_DEFAULT,         /**<default*//**<CNcomment:默认透明度 */
    HI_UNF_CC_OPACITY_SOLID,           /**<opaque*//**<CNcomment:不透明 */
    HI_UNF_CC_OPACITY_FLASH,           /**<flash*//**<CNcomment:闪烁 */
    HI_UNF_CC_OPACITY_TRANSLUCENT,     /**<translucent*//**<CNcomment:半透明 */
    HI_UNF_CC_OPACITY_TRANSPARENT,     /**<transparent*//**<CNcomment:透明 */
    HI_UNF_CC_OPACITY_BUTT
}HI_UNF_CC_OPACITY_E;

/**justify*//** CNcomment:排版*/
typedef enum hiUNF_CC_JUSTIFY_E
{
    HI_UNF_CC_JUSTIFY_LEFT,        /**<left*//**<CNcomment:居左 */
    HI_UNF_CC_JUSTIFY_RIGHT,       /**<rigth*//**<CNcomment:居右 */
    HI_UNF_CC_JUSTIFY_CENTER,      /**<center*//**<CNcomment:居中 */
    HI_UNF_CC_JUSTIFY_FULL,        /**<full*//**<CNcomment:两端对齐 */
    HI_UNF_CC_JUSTIFY_BUTT
}HI_UNF_CC_JUSTIFY_E;

/**word wrap*//** CNcomment:自动换行*/
typedef enum hiUNF_CC_WORDWRAP_E
{
    HI_UNF_CC_WW_DISABLE,         /**<disable*//**<CNcomment:不自动换行 */
    HI_UNF_CC_WW_ENABLE,          /**<enable*//**<CNcomment:自动换行 */
    HI_UNF_CC_WW_BUTT
}HI_UNF_CC_WORDWRAP_E;

/**font style*//** CNcomment:字体风格 */
typedef enum hiUNF_CC_FONTSTYLE_E
{
    HI_UNF_CC_FONTSTYLE_DEFAULT,         /**<default*//**<CNcomment:默认字体风格 */
    HI_UNF_CC_FONTSTYLE_NORMAL,          /**<normal*//**<CNcomment:正常 */
    HI_UNF_CC_FONTSTYLE_ITALIC,          /**<italic*//**<CNcomment:斜体 */
    HI_UNF_CC_FONTSTYLE_UNDERLINE,       /**<underline*//**<CNcomment:下划线 */
    HI_UNF_CC_FONTSTYLE_ITALIC_UNDERLINE,/**<italic&underline*//**<CNcomment:斜体并且带下滑线 */
    HI_UNF_CC_FONTSTYLE_BUTT
}HI_UNF_CC_FONTSTYLE_E;

/**font size*//** CNcomment:字体大小 */
typedef enum hiUNF_CC_FONTSIZE_E
{
    HI_UNF_CC_FONTSIZE_DEFAULT,       /**<default font size *//**<CNcomment:默认字体大小 */
    HI_UNF_CC_FONTSIZE_SMALL,         /**<small*//**<CNcomment:小 */
    HI_UNF_CC_FONTSIZE_STANDARD,      /**<standard*//**<CNcomment:标准 */
    HI_UNF_CC_FONTSIZE_LARGE,         /**<large*//**<CNcomment:大 */
    HI_UNF_CC_FONTSIZE_BUTT
} HI_UNF_CC_FONTSIZE_E;

/**font name*//** CNcomment:字体样式 */
typedef enum  hiUNF_CC_FONTNAME_E
{
    HI_UNF_CC_FN_DEFAULT,                  /**<default *//**<CNcomment:默认字体样式 */
    HI_UNF_CC_FN_MONOSPACED,               /**<monospaced*//**<CNcomment:monospaced字体 */
    HI_UNF_CC_FN_PROPORT,                  /**<proport*//**<CNcomment:proport字体 */
    HI_UNF_CC_FN_MONOSPACED_NO_SERIAFS,    /**<monospaced with no seriafs*//**<CNcomment:monospaced字体(无衬线) */
    HI_UNF_CC_FN_PROPORT_NO_SERIAFS,       /**<proport with no seriafs*//**<CNcomment:proport字体(无衬线) */
    HI_UNF_CC_FN_CASUAL,                   /**<casual*//**<CNcomment:casual字体 */
    HI_UNF_CC_FN_CURSIVE,                  /**<cursive*//**<CNcomment:cursive字体 */
    HI_UNF_CC_FN_SMALL_CAPITALS,           /**<small capitals*//**<CNcomment:小写字体 */
    HI_UNF_CC_FN_BUTT
}HI_UNF_CC_FONTNAME_E;

/**font edge type*//** CNcomment:字体边缘类型 */
typedef enum hiUNF_CC_EdgeType_E
{
    HI_UNF_CC_EDGETYPE_DEFAULT,            /**<default *//**<CNcomment:默认字体边缘类型 */
    HI_UNF_CC_EDGETYPE_NONE,               /**<none edge type *//**<CNcomment:没有边缘 */
    HI_UNF_CC_EDGETYPE_RAISED,             /**<raised *//**<CNcomment:边缘突起 */
    HI_UNF_CC_EDGETYPE_DEPRESSED,          /**<depressed *//**<CNcomment:边缘凹下 */
    HI_UNF_CC_EDGETYPE_UNIFORM,            /**<uniform *//**<CNcomment:边缘统一 */
    HI_UNF_CC_EDGETYPE_LEFT_DROP_SHADOW,   /**<left drop shadow *//**<CNcomment:左下阴影 */
    HI_UNF_CC_EDGETYPE_RIGHT_DROP_SHADOW,  /**<right drop shadow *//**<CNcomment:右下阴影 */
    HI_UNF_CC_EDGETYPE_BUTT
}HI_UNF_CC_EdgeType_E;

/**display format of caption display screen*//** CNcomment:显示窗口的显示模式 */
typedef enum hiUNF_CC_DF_E
{
    HI_UNF_CC_DF_720X480,       /**<caption display screen is 720*480*//**<CNcomment:显示窗口的宽高是720*480 */
    HI_UNF_CC_DF_720X576,       /**<caption display screen is 720*576*//**<CNcomment:显示窗口的宽高是720*576 */
    HI_UNF_CC_DF_960X540,       /**<caption display screen is 960*540*//**<CNcomment:显示窗口的宽高是960*540 */
    HI_UNF_CC_DF_1280X720,      /**<caption display screen is 1280*720*//**<CNcomment:显示窗口的宽高是1280*720 */
    HI_UNF_CC_DF_1920X1080,     /**<caption display screen is 1920*1080*//**<CNcomment:显示窗口的宽高是1920*1080 */
    HI_UNF_CC_DF_BUTT
} HI_UNF_CC_DF_E;

/**CC display operation*//** CNcomment:CC显示操作 */
typedef enum hiUNF_CC_OPT_E
{
     HI_UNF_CC_OPT_DRAWTEXT = 0x1, /**<draw text *//**<CNcomment:绘制文本 */
     HI_UNF_CC_OPT_DRAWBITMAP,     /**<draw bitmap *//**<CNcomment:绘制位图 */
     HI_UNF_CC_OPT_FILLRECT,       /**<fill rect *//**<CNcomment:填充矩形区域 */
     HI_UNF_CC_OPT_BUTT
} HI_UNF_CC_OPT_E;

/**Arib CC rollup mode*//** CNcomment:arib CC rollup模式*/
typedef enum hiUNF_CC_ARIB_ROLLUP_E
{
    HI_UNF_CC_ARIB_NON_ROLLUP,        /**<Non roll-up *//**<CNcomment:非rollup模式*/
    HI_UNF_CC_ARIB_ROLLUP,      /**<roll-up*//**<CNcomment:rollup模式*/
    HI_UNF_CC_ARIB_ROLLUP_BUTT
}HI_UNF_CC_ARIB_ROLLUP_E;


/**Arib CC character coding*//** CNcomment:arib CC 字符编码方式*/
typedef enum hiUNF_CC_ARIB_TCS_E
{
    HI_UNF_CC_ARIB_TCS_8BIT,         /**<the type of character codig is 8bit-code*//**<CNcomment:字符编码方式(8位)*/
    HI_UNF_CC_ARIB_TCS_BUTT
}HI_UNF_CC_ARIB_TCS_E;

/**Arib CC display format*//** CNcomment:arib CC显示方式*/
typedef enum hiUNF_CC_ARIB_DF_E
{
    HI_UNF_CC_ARIB_DF_HORIZONTAL_SD,                /**<horizontal writing in stardard density*/    /**<CNcomment:水平显示，标清*/
    HI_UNF_CC_ARIB_DF_VERTICAL_SD,                  /**<vertical writing in standard density*/      /**<CNcomment:垂直显示，标清*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_HD,                /**<horizontal writing in high density*/        /**<CNcomment:水平显示，高清*/
    HI_UNF_CC_ARIB_DF_VERTICAL_HD,                  /**<vertical writing in high density*/          /**<CNcomment:垂直显示，高清*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_WESTERN,           /**<horizontal writing of western language*/    /**<CNcomment:水平显示，西文*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_1920X1080,         /**<horizontal writing in 1920X1080*/           /**<CNcomment:水平显示，屏幕1920X1080*/
    HI_UNF_CC_ARIB_DF_VERTICAL_1920X1080,           /**<vertical writing in 1920X1080*/             /**<CNcomment:垂直显示，屏幕1920X1080*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_960X540,           /**<horizontal writing in 960X540*/             /**<CNcomment:水平显示，屏幕960X540*/
    HI_UNF_CC_ARIB_DF_VERTICAL_960X540,             /**<vertical writing in 960X540*/               /**<CNcomment:垂直显示，屏幕960X540*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_1280X720,          /**<horizontal writing in 1280X720*/            /**<CNcomment:水平显示，屏幕1280X720*/
    HI_UNF_CC_ARIB_DF_VERTICAL_1280X720,            /**<vertical writing in 1280X720*/              /**<CNcomment:垂直显示，屏幕1280X720*/
    HI_UNF_CC_ARIB_DF_HORIZONTAL_720X480,           /**<horizontal writing in 720X480*/             /**<CNcomment:水平显示，屏幕720X480*/
    HI_UNF_CC_ARIB_DF_VERTICAL_720X480,             /**<vertical writing in 720X480*/               /**<CNcomment:垂直显示，屏幕720X480*/
    HI_UNF_CC_ARIB_DF_BUTT
}HI_UNF_CC_ARIB_DF_E;

/**Arib CC display mode*//** CNcomment:arib CC显示模式*/
typedef enum hiUNF_CC_ARIB_DMF_E
{
    HI_UNF_CC_ARIB_DMF_AUTO_AND_AUTO=0x0,           /**<atomatic display when received ,automatic display when recording and playback */                    /**<CNcomment:接收后自动播放，录制和回放时自动播放*/
    HI_UNF_CC_ARIB_DMF_AUTO_AND_NOT,                /**<atomatic display when received ,Non-displayed automatically when recording and playback*/           /**<CNcomment:接收后自动播放，录制和回放时不自动播放*/
    HI_UNF_CC_ARIB_DMF_AUTO_AND_SELECT,             /**<atomatic display when received ,Selectable display when recording and playback*/                    /**<CNcomment:接收后自动播放，录制和回放时可选择播放*/
    HI_UNF_CC_ARIB_DMF_NON_AND_AUTO=0x4,            /**<non-displayed automatically when received, automatic display when recording and playback */         /**<CNcomment:接收时不自动播放，录制和回放时自动播放*/
    HI_UNF_CC_ARIB_DMF_NON_AND_NON,                 /**<non-displayed automatically when received,Non-displayed automatically when recording and playback*/ /**<CNcomment:接收时不自动播放，录制和回放时不自动播放*/
    HI_UNF_CC_ARIB_DMF_NON_AND_SELECT,              /**<non-displayed automatically when received,Selectable display when recording and playback*/          /**<CNcomment:接收时不自动播放，录制和回放时可选择播放*/
    HI_UNF_CC_ARIB_DMF_SELECT_AND_AUTO=0x8,         /**<selectable display when received, automatic display when recording and playback */                  /**<CNcomment:接收时可选择播放，录制和回放时可自动播放*/
    HI_UNF_CC_ARIB_DMF_SELECT_AND_NON,              /**<selectable display when received,Non- displayed automatically when recording and playback*/         /**<CNcomment:接收时可选择播放，录制和回放时不自动播放*/
    HI_UNF_CC_ARIB_DMF_SELECT_AND_SELECT,           /**<selectable display when received,Selectable display when recording and playback*/                   /**<CNcomment:接收时可选择播放，录制和回放时可选择播放*/
    HI_UNF_CC_ARIB_DMF_SPECIAL_AND_AUTO=0xc,        /**<automatic display/non-display under specific condition when received,automatic display when recording and playback */           /**<CNcomment:接收时特定情况下自动/非自动播放，录制和回放时自动播放*/
    HI_UNF_CC_ARIB_DMF_SPECIAL_AND_NON,             /**<automatic display/non-display under specific condition when received,Non-displayed automatically when recording and playback*/  /**<CNcomment:接收时特定情况下自动/非自动播放，录制和回放时不自动播放*/
    HI_UNF_CC_ARIB_DMF_SPECIAL_AND_SELECT,          /**<automatic display/non-display under specific condition when received,Selectable display when recording and playback*/           /**<CNcomment:接收时特定情况下自动/非自动播放，录制和回放时可选择播放*/
    HI_UNF_CC_ARIB_DMF_BUTT
}HI_UNF_CC_ARIB_DMF_E;

/**Arib CC time control mode*//** CNcomment:arib CC时间控制模式*/
typedef enum hiUNF_CC_ARIB_TMD_E
{
    HI_UNF_CC_ARIB_TMD_FREE,            /**<playback time is not restricted to synchronize to the clock*//**<CNcomment:重放时间和时钟无关*/
    HI_UNF_CC_ARIB_TMD_REAL_TIME,       /**<playback time is given by PTS*//**<CNcomment:重放时间由PTS给出*/
    HI_UNF_CC_ARIB_TMD_OFFSET_TIME,     /**<playback time added with offset time should be the new playback time*//**<CNcomment:重放时间加上偏移时间将是新的重放时间*/
    HI_UNF_CC_ARIB_TMD_BUTT
}HI_UNF_CC_ARIB_TMD_E;

/**CC608 VBI data*//** CNcomment:CC608 VBI数据 */
typedef struct hiUNF_CC_VBI_DADA_S
{
    HI_U8 u8FieldParity; /**<parity field *//**<CNcomment:奇偶场 */
    HI_U8 u8Data1;       /**<first byte *//**<CNcomment:第一个字节 */
    HI_U8 u8Data2;       /**<second byte *//**<CNcomment:第二个字节 */
}HI_UNF_CC_VBI_DADA_S;

/**CC rect*//** CNcomment:定义CC矩形区域*/
typedef struct hiUNF_CC_RECT_S
{
    HI_U16 x;           /**<x cordinate *//**<CNcomment:矩形左上角的x坐标 */
    HI_U16 y;           /**<y cordinate *//**<CNcomment:矩形左上角的y坐标 */
    HI_U16 width;       /**<rect width *//**<CNcomment:矩形的宽度 */
    HI_U16 height;      /**<rect heigth *//**<CNcomment:矩形的高度 */
}HI_UNF_CC_RECT_S;

/**CC color components*//** CNcomment:CC像素颜色结构 */
typedef struct hiUNF_CC_COLOR_S
{
    HI_U8 u8Blue;       /**<blue component *//**<CNcomment:B分量颜色值 */
    HI_U8 u8Green;      /**<green component *//**<CNcomment:G分量颜色值 */
    HI_U8 u8Red;        /**<red component *//**<CNcomment:R分量颜色值 */
    HI_U8 u8Alpha;      /**<alpha component *//**<CNcomment:透明度，值为0为透明，0xFF为不透明 */
} HI_UNF_CC_COLOR_S;

/**CC data with text format*//** CNcomment:文本格式的CC数据 */
typedef struct hiUNF_CC_TEXT_S
{
    HI_U16                 *pu16Text;    /**<cc data,2 bytes,unicode *//**<CNcomment:cc数据，unicode编码，2个字节 */
    HI_U8                  u8TextLen;    /**<cc data length *//**<CNcomment:要显示的cc数据长度 */

    HI_UNF_CC_COLOR_S      stFgColor;    /**<cc foregroud color*//**<CNcomment:要显示的cc数据前景色 */
    HI_UNF_CC_COLOR_S      stBgColor;    /**<cc backgroud color*//**<CNcomment:要显示的cc数据字体背景色 */
    HI_UNF_CC_COLOR_S      stEdgeColor;   /**<cc edge color*//**<CNcomment:要显示的cc数据字体边缘颜色*/

    HI_U8                  u8Justify;    /*see HI_UNF_CC_JUSTIFY_E,used when decode cc708*//**<CNcomment:cc数据显示时的排版方式 */
    HI_U8                  u8WordWrap;   /*see HI_UNF_CC_WORDWRAP_E,used when decode cc708*//**<CNcomment:自动换行 */
    HI_UNF_CC_FONTSTYLE_E  enFontStyle;  /*font style ,see HI_UNF_CC_FONTSTYLE_E*//**<CNcomment:要显示的cc数据的字体风格 */
    HI_UNF_CC_FONTSIZE_E   enFontSize;   /*font size ,see HI_UNF_CC_FONTSIZE_E*//**<CNcomment:要显示的cc数据的字体大小 */
    HI_UNF_CC_EdgeType_E    enEdgetype;  /**<cc edge type*//**<CNcomment:要显示的cc数据字体边缘类型*/
} HI_UNF_CC_TEXT_S;

/**CC data with bitmap format*//** CNcomment:位图格式的CC数据 */
typedef struct hiUNF_CC_BITMAP_S
{
    HI_S32                 s32BitWidth;         /**<bit width, is 2/4/8/16/24/32 *//**<CNcomment:位宽,值是2/4/8/16/24/32 */
    HI_U8                  *pu8BitmapData;      /**<pixel data *//**<CNcomment:图像数据 */
    HI_U32                 u32BitmapDataLen;    /**<length of pixel data *//**<CNcomment:图像数据长度 */
    HI_UNF_CC_COLOR_S      astPalette[256];     /**<palette data *//**<CNcomment:调色板数据 */
    HI_U32                 u32PaletteLen;       /**<length of palette data *//**<CNcomment:调色板数据长度 */
} HI_UNF_CC_BITMAP_S;

/**parameter of fill rect*//** CNcomment:填充矩形区域的参数 */
typedef struct hiUNF_CC_FILLRECT_S
{
    HI_UNF_CC_COLOR_S      stColor;      /**<color *//**<CNcomment:颜色值 */
} HI_UNF_CC_FILLRECT_S;

/**CC display param*//** CNcomment:定义CC数据显示时的参数 */
typedef struct hiUNF_CC_DISPLAY_PARAM_S
{
    HI_UNF_CC_OPT_E        enOpt;
    HI_U32                 u32DisplayWidth; /**<display screen width*//**<CNcomment:画布宽度 */
    HI_U32                 u32DisplayHeight;/**<display screen height*//**<CNcomment:画布高度 */
    HI_UNF_CC_RECT_S       stRect;          /**<cc display location*//**<CNcomment:要显示的cc数据的位置信息 */
    union
    {
        HI_UNF_CC_TEXT_S   stText;          /**<CC data with text format*//**<CNcomment:文本格式的CC数据 */
        HI_UNF_CC_BITMAP_S stBitmap;        /**<CC data with bitmap format*//**<CNcomment:位图格式的CC数据 */
        HI_UNF_CC_FILLRECT_S stFillRect;    /**<parameter of fill rect*//**<CNcomment:填充矩形区域的参数 */
    } unDispParam;
} HI_UNF_CC_DISPLAY_PARAM_S;

/**
\brief cc get text size callback function. CNcomment: cc用于获得字体大小(宽高)的回调函数。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in] pUserData user data. CNcomment: HI_UNF_CC_Create传入的用户私有数据。CNend
\param[in] u16Str  string. CNcomment: 数据地址。CNend
\param[in] StrNum  number of character. CNcomment: 数据长度。CNend
\param[out] width  total width of character. CNcomment: 得到的字体宽。CNend
\param[out] heigth heigth of character. CNcomment: 得到的字体高。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
none. CNcomment: 无。CNend
*/
typedef HI_S32 (*HI_UNF_CC_GETTEXTSIZE_CB_FN)(HI_VOID* pUserData, HI_U16 *u16Str,HI_S32 s32StrNum, HI_S32 *ps32Width, HI_S32 *ps32Heigth);


/**
\brief cc display callback function. CNcomment: cc显示数据回调。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in] pUserData user data. CNcomment: HI_UNF_CC_Create传入的用户私有数据。CNend
\param[in] pstCCdisplayParam parameter of display. CNcomment: cc显示数据结构。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
none. CNcomment: 无。CNend
*/
typedef HI_S32 (*HI_UNF_CC_DISPLAY_CB_FN)(HI_VOID* pUserData, HI_UNF_CC_DISPLAY_PARAM_S *pstCCdisplayParam);


/**
\brief cc get avplay pts callback function. CNcomment: cc获取当前节目的PTS值的回调。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in] pUserData user data. CNcomment: HI_UNF_CC_Create传入的用户私有数据。CNend
\param[out] ps64CurrentPts current pts. CNcomment: 存储当前PTS值的变量指针。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
none. CNcomment: 无。CNend
*/
typedef HI_S32 (*HI_UNF_CC_GETPTS_CB_FN)(HI_VOID* pUserData, HI_S64 *ps64CurrentPts);

/**
\brief cc data blit callback function. CNcomment: cc数据搬移回调。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in] pUserData user data. CNcomment: HI_UNF_CC_Create传入的用户私有数据。CNend
\param[in] SrcRect source rect. CNcomment: 需要搬移的原区域。CNend
\param[in] DstRect destination rect. CNcomment: 需要搬移的目标区域。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
none. CNcomment: 无。CNend
*/
typedef HI_S32 (*HI_UNF_CC_BLIT_CB_FN)(HI_VOID* pUserData, HI_UNF_CC_RECT_S *pstSrcRect, HI_UNF_CC_RECT_S *pstDstRect);

/**
\brief cc data blit callback function. CNcomment: cc vbi 数据输出回调。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in] pUserData user data. CNcomment: HI_UNF_CC_Create传入的用户私有数据。CNend
\param[out] pstVBIOddDataField1 VBI data,odd field data. CNcomment: VBI数据结构,奇场数据。CNend
\param[out] pstVBIEvenDataField2 VBI data,even field data. CNcomment: VBI数据结构,偶场数据。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
none. CNcomment: 无。CNend
*/
typedef HI_S32 (*HI_UNF_CC_VBI_CB_FN)(HI_VOID* pUserData, HI_UNF_CC_VBI_DADA_S *pstVBIOddDataField1,HI_UNF_CC_VBI_DADA_S *pstVBIEvenDataField2);

/**
\brief output XDS packets in CC608. CNcomment: 输出CC608中的XDS包数据。CNend
\attention \n
XDS packet is a third data service intended to supply program related and other information.
CNcomment: XDS包是第三方数据服务，用来提供节目或者其他相关信息。CNend
\param[in] u8XDSClass XDS class. CNcomment: XDS的分类。CNend
\param[in] u8XDSPacketType  XDS packet type. CNcomment: XDS数据包的类型。CNend
\param[in] pu8Data  XDS data. CNcomment: XDS数据。CNend
\param[in] u8DataLen  XDS data length. CNcomment: XDS数据长度。CNend
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
Decoding of XDS packet data, see sample/cc/sample_cc_xds.c.
CNcomment: XDS数据包的解析，请参看sdk包sample/cc/sample_cc_xds.c。CNend
*/
typedef HI_S32 (*HI_UNF_CC_XDS_CB_FN)(HI_U8 u8XDSClass, HI_U8 u8XDSPacketType, HI_U8 *pu8Data, HI_U8 u8DataLen);


/**user data in MPEG*//** CNcomment:MPEG中的USERDATA数据 */
typedef struct hiUNF_CC_USERDATA_S
{
    HI_U8       *pu8userdata;  /**<cc user data in mpeg *//**<CNcomment:mpeg视频帧中的cc用户数据 */
    HI_U32      u32dataLen;    /**<cc user data length *//**<CNcomment:cc数据长度 */
    HI_BOOL     bTopFieldFirst;/**<cc top field first flag *//**<CNcomment:cc顶场优先标记 */
} HI_UNF_CC_USERDATA_S;

/**CC608 config param *//** CNcomment:CC608 配置信息参数 */
/**CC608 just support 8 stardard colors*//**CNcomment:CC608只支持8种标准颜色*/
typedef struct hiUNF_CC_608_CONFIGPARAM_S
{
    HI_UNF_CC_608_DATATYPE_E    enCC608DataType;        /**<CC608 data type *//**<CNcomment:配置cc608数据类型 */
    HI_U32                      u32CC608TextColor;      /**<CC608 text color *//**<CNcomment:配置cc608字体颜色 */
    HI_UNF_CC_OPACITY_E         enCC608TextOpac;        /**<CC608 text opacity *//**<CNcomment:配置cc608字体透明度 */
    HI_U32                      u32CC608BgColor;        /**<CC608 background color *//**<CNcomment:配置cc608背景色 */
    HI_UNF_CC_OPACITY_E         enCC608BgOpac;          /**<CC608 background opacity *//**<CNcomment:配置cc608背景透明度 */
    HI_UNF_CC_FONTSTYLE_E       enCC608FontStyle;       /**<CC608 font style *//**<CNcomment:配置cc608字体风格 */
    HI_UNF_CC_DF_E              enCC608DispFormat;      /**<CC608 display format of caption display screen *//**<CNcomment:配置cc608显示模式 */
    HI_BOOL                     bLeadingTailingSpace;   /**< CC608 leading/tailing space flag*//**<CNcomment: 是否显示leading/tailing space*/
} HI_UNF_CC_608_CONFIGPARAM_S;

/**CC708 config param *//** CNcomment:CC708 配置信息参数 */
/**CC708 color specification CEA-708-B-1999.pdf section 9.20 Color Representation*//**CNcomment:CC708颜色说明请参考协议CEA-708-B-1999.pdf 9.20 颜色呈现 章节*/ 
/**The specification does not define the specific color component, using the following component values in our version*/
/**协议中没有定义具体的颜色分量，在我们版本中采用如下分量值*/
/**CC708 support 64 colors.one of RGB color components can be 0x00,0x5f,0xaf,0xff*//**CNcomment:CC708支持64种颜色,一个RGB分量能取的值有0x00,0x5f,0xaf,0xff共4种*/
/** 0x    FF          FF          FF          FF*/       /**CNcomment: 0x FF     FF         FF          FF*/
/**    reserve    red value   green value  blue value*/  /**              保留   红色分量   绿色分量    蓝色分量*/
typedef struct hiUNF_CC_708_CONFIGPARAM_S
{
    HI_UNF_CC_708_SERVICE_NUM_E enCC708ServiceNum;      /**<CC708 service number *//**<CNcomment:配置cc708服务通道号 */
    HI_UNF_CC_FONTNAME_E        enCC708FontName;        /**<CC708 font name *//**<CNcomment:配置cc708字体 */
    HI_UNF_CC_FONTSTYLE_E       enCC708FontStyle;       /**<CC708 font style *//**<CNcomment:配置cc708字体风格 */
    HI_UNF_CC_FONTSIZE_E        enCC708FontSize;        /**<CC708 font size *//**<CNcomment:配置cc708字体大小 */
    HI_U32                      u32CC708TextColor;      /**<CC708 text color *//**<CNcomment:配置cc708字体颜色 */
    HI_UNF_CC_OPACITY_E         enCC708TextOpac;        /**<CC708 text opacity *//**<CNcomment:配置cc708字体透明度 */
    HI_U32                      u32CC708BgColor;        /**<CC708 background color *//**<CNcomment:配置cc708背景颜色 */
    HI_UNF_CC_OPACITY_E         enCC708BgOpac;          /**<CC708 background opacity *//**<CNcomment:配置cc708背景透明度 */
    HI_U32                      u32CC708WinColor;       /**<CC708 window color *//**<CNcomment:配置cc708窗口颜色 */
    HI_UNF_CC_OPACITY_E         enCC708WinOpac;         /**<CC708 window opacity *//**<CNcomment:配置cc708窗口透明度 */
    HI_UNF_CC_EdgeType_E        enCC708TextEdgeType;    /**<CC708 text egde type *//**<CNcomment:配置cc708字体边缘类型 */
    HI_U32                      u32CC708TextEdgeColor;  /**<CC708 text edge color *//**<CNcomment:配置cc708字体边缘颜色 */
    HI_UNF_CC_DF_E              enCC708DispFormat;      /**<CC708 display format of caption display screen *//**<CNcomment:配置cc708显示模式 */
} HI_UNF_CC_708_CONFIGPARAM_S;

/**ARIB CC config param *//** CNcomment:ARIB CC 配置信息参数 */
typedef struct hiUNF_CC_ARIB_CONFIGPARAM_S
{
    HI_U32      u32BufferSize;   /**<size of buffer which used to cache pes data,Recommends its value is 64K ~ 512K.note:This value can only be set when created,does not support dynamic setting*/
                                 /**<CNcomment:缓存PES数据的缓冲区大小，取值为64k~512K。注意:这个值只能在创建时设置，不支持动态设置*/
} HI_UNF_CC_ARIB_CONFIGPARAM_S;

/**CC data attribution *//** CNcomment:CC属性信息 */
typedef struct hiUNF_CC_ATTR_S
{
    HI_UNF_CC_DATA_TYPE_E enCCDataType;   /**<cc data type *//**<CNcomment:cc数据类型 */
    union
    {
        HI_UNF_CC_608_CONFIGPARAM_S  stCC608ConfigParam;   /**<CC608 config param *//**<CNcomment:CC608 配置信息参数 */
        HI_UNF_CC_708_CONFIGPARAM_S  stCC708ConfigParam;   /**<CC708 config param *//**<CNcomment:CC708 配置信息参数 */
        HI_UNF_CC_ARIB_CONFIGPARAM_S stCCARIBConfigParam;  /**<ARIB CC config param *//**<CNcomment:ARIB CC 配置信息参数 */
    } unCCConfig;
} HI_UNF_CC_ATTR_S;

/**CC instance param *//** CNcomment:创建实例时需要的参数信息 */
typedef struct hiUNF_CC_PARAM_S
{
    HI_UNF_CC_ATTR_S             stCCAttr;           /**<cc attribution *//**<CNcomment:cc属性信息 */

    HI_UNF_CC_GETPTS_CB_FN       pfnCCGetPts;        /**<get current pts callback function *//**<CNcomment:获取当前pts的回调函数 */
    HI_UNF_CC_DISPLAY_CB_FN      pfnCCDisplay;       /**<cc display callback function *//**<CNcomment:cc显示的回调函数 */
    HI_UNF_CC_GETTEXTSIZE_CB_FN  pfnCCGetTextSize;   /**<cc get text size callback function *//**<CNcomment:cc用于获得字体大小(宽高)的回调函数  */
    HI_UNF_CC_BLIT_CB_FN         pfnBlit;            /**<cc data blit callback function *//**<CNcomment:用于cc数据在屏幕上搬移的回调函数 */
    HI_UNF_CC_VBI_CB_FN          pfnVBIOutput;       /**<output VBI data callback function *//**<CNcomment:输出VBI数据回调函数 */
    HI_UNF_CC_XDS_CB_FN          pfnXDSOutput;       /**<output XDS packets function *//**<CNcomment:输出CC608中的XDS包数据 */
    HI_VOID*                     pUserData;        /**<user data,used in callback function *//**<CNcomment:用户私有数据，用于回调函数 */
} HI_UNF_CC_PARAM_S;

/*ARIB CC info node*//** CNcomment:arib cc字幕信息节点 */
typedef struct hiUNF_CC_ARIB_INFONODE_S
{
    HI_U8 u8LanguageTag;                   /**<identification of language*//**<CNcomment:字幕语言标记 */
    HI_UNF_CC_ARIB_DMF_E    enCCAribDMF;   /**<display mode *//**<CNcomment:显示模式 */
    HI_CHAR    acISO639LanguageCode[4];    /**<language code *//**<CNcomment:字幕语言代码 */
    HI_UNF_CC_ARIB_DF_E enCCAribDF;        /**<display format *//**<CNcomment:显示方式 */
    HI_UNF_CC_ARIB_TCS_E enCCAribTCS;         /**<character coding *//**<CNcomment:字符编码 */
    HI_UNF_CC_ARIB_ROLLUP_E enCCAribRollup;    /**<roll-up mode *//**<CNcomment:roll-up模式 */
}HI_UNF_CC_ARIB_INFONODE_S;

/*ARIB CC info struct*//** CNcomment:arib cc字幕信息结构体 */
typedef struct hiUNF_CC_ARIB_INFO_S
{
    HI_UNF_CC_ARIB_TMD_E enCCAribTMD;   /**<time control mode*//**<CNcomment:时间控制模式 */
    HI_U32 u32NumLanguage;       /**<number of languages *//**<CNcomment:字幕语言个数 */
    HI_UNF_CC_ARIB_INFONODE_S stCCAribInfonode[ARIBCC_MAX_LANGUAGE];     /**<array of arib cc info *//**<CNcomment:arib cc字幕信息数组 */
}HI_UNF_CC_ARIB_INFO_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API Declaration *****************************/
/**
\brief Initialize cc module. CNcomment: 初始化CC模块。CNend
\attention \n
none. CNcomment: 无。CNend
\retval ::HI_SUCCESS initialize success. CNcomment: 初始化成功。CNend
\retval ::HI_FAILURE initialize failure. CNcomment: 初始化失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Init(HI_VOID);

/**
\brief DeInitialize cc module. CNcomment: 去初始化CC模块。CNend
\attention \n
none. CNcomment: 无。CNend
\retval ::HI_SUCCESS deinitialize success. CNcomment: 去初始化成功。CNend
\retval ::HI_FAILURE deinitialize failure. CNcomment: 去初始化失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_DeInit(HI_VOID);

/**
\brief Get default attribution in cc module. CNcomment: 获取CC模块的默认属性值。CNend
\attention \n
none. CNcomment: 无。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_GetDefaultAttr(HI_UNF_CC_ATTR_S *pstDefaultAttr);

/**
\brief open cc module. CNcomment: 创建cc实例。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  pstAttr  cc attribution. CNcomment: 创建时传入的解码器属性。CNend
\param[out]  phCC  cc handle. CNcomment: cc模块句柄。CNend
\retval ::HI_SUCCESS success. CNcomment: 创建成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 创建失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Create(HI_UNF_CC_PARAM_S *pstCCParam, HI_HANDLE *phCC);

/**
\brief close cc module. CNcomment: 销毁cc实例。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\retval ::HI_SUCCESS success. CNcomment: 销毁成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 销毁失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Destroy(HI_HANDLE hCC);

/**
\brief start cc module. CNcomment: 开始cc模块。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Start(HI_HANDLE hCC);

/**
\brief stop cc module. CNcomment: 结束cc模块。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Stop(HI_HANDLE hCC);

/**
\brief reset cc module. CNcomment: 复位cc模块。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_Reset(HI_HANDLE hCC);

/**
\brief inject mpeg userdata to  cc module. CNcomment: 注入mpeg用户数据到cc模块。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\param[in]  pstUserData  cc userdata structure used in inject cc data. CNcomment: 用户数据结构体。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_InjectUserData(HI_HANDLE hCC, HI_UNF_CC_USERDATA_S *pstUserData);

/**
\brief inject cc pes data to cc module. CNcomment: 注入pes数据到cc模块(暂不支持)。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment:模块句柄。CNend
\param[in]  pu8PesData  pes data address. CNcomment: pes数据首地址。CNend
\param[in]  u32DataLen  pes data length. CNcomment: pes数据长度。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_InjectPESData(HI_HANDLE hCC, HI_U8 *pu8PesData, HI_U32 u32DataLen);

/**
\brief get cc attribution. CNcomment: 获取cc属性信息。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment: 模块句柄。CNend
\param[out]  pstCCAttr  cc attribution structure. CNcomment: 属性信息结构体。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_GetAttr(HI_HANDLE hCC, HI_UNF_CC_ATTR_S *pstCCAttr);

/**
\brief set cc attribution. CNcomment:设置cc属性信息。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  cc handle. CNcomment. CNcomment: 模块句柄。CNend
\param[in]  pstCCAttr  cc attribution structure. CNcomment:属性信息结构体。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_SetAttr(HI_HANDLE hCC, HI_UNF_CC_ATTR_S *pstCCAttr);


/**
\brief get cc arib info. CNcomment: 获取arib cc字幕信息。CNend
\attention \n
none. CNcomment: 无。CNend
\param[in]  hCC  arib cc handle. CNcomment: 模块句柄。CNend
\param[out]  pstCCAttr  arib cc info structure. CNcomment:arib cc字幕信息结构体。CNend
\retval ::HI_SUCCESS success. CNcomment: 成功。CNend
\retval ::HI_FAILURE failure. CNcomment: 失败。CNend
\see \n
none. CNcomment: 无。CNend
*/
HI_S32 HI_UNF_CC_GetARIBCCInfo(HI_HANDLE hCC,HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
