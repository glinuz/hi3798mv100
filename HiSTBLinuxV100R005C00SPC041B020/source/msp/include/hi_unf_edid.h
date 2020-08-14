/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_edid.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013-8-28
  Description   :
  History       :
  1.Date        : 2013-8-28
    Author      : wx 178178
    Modification: Created file

*******************************************************************************/
#ifndef __HI_UNF_EDID_H__
#define __HI_UNF_EDID_H__

#include "hi_unf_common.h"
#include "hi_unf_audio.h"
#include "hi_unf_video.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      EDID */
/** @{*/  /** <!-- [EDID] */

/**EDID Audio type enum*//**CNcomment:HDMI 音频类型 枚举 */
typedef enum hiUNF_EDID_AUDIO_FORMAT_CODE_E
{
    HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED  = 0x00,  /**<Audio coding type, refer stream,default type*//**<CNcomment:音频编码类型，参考数据流，缺省格式 */    
    HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM,               /**<Audio coding PCM type*//**<CNcomment:音频编码PCM格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3,               /**<Audio coding AC3 type*//**<CNcomment:音频编码AC3格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1,             /**<Audio coding MPEG1 type*//**<CNcomment:音频编码MPEG1格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3,               /**<Audio coding MP3 type*//**<CNcomment:音频编码MP3格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2,             /**<Audio coding MPEG2 type*//**<CNcomment:音频编码MPEG2格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC,               /**<Audio coding AAC type*//**<CNcomment:音频编码AAC格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS,               /**<Audio coding DTS type*//**<CNcomment:音频编码DTS格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC,             /**<Audio coding ATRAC type*//**<CNcomment:音频编码ATRAC格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT,           /**<Audio coding ONE BIT AUDIO type*//**<CNcomment:音频编码ONE_BIT格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP,               /**<Audio coding DDPLUS type*//**<CNcomment:音频编码DDPLUS格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD,            /**<Audio coding DTS HD type*//**<CNcomment:音频编码DTS HD格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT,               /**<Audio coding MAT type*//**<CNcomment:音频编码MAT格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DST,               /**<Audio coding DST type*//**<CNcomment:音频编码DST格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO,           /**<Audio coding WMA PRO type*//**<CNcomment:音频编码WMA PRO格式 */
    HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT,
}HI_UNF_EDID_AUDIO_FORMAT_CODE_E;

/**the max audio smprate count*//**CNcomment:最大音频采样率个数 */
#define MAX_SAMPE_RATE_NUM 8

/**the max audio bit depth count*//**CNcomment:最大音频比特深度个数 */
#define MAX_BIT_DEPTH_NUM  6

/**EDID Audio Info struct*//**CNcomment:EDID 音频信息结构体 */
typedef struct hiUNF_EDID_AUDIO_INFO_S
{
    HI_UNF_EDID_AUDIO_FORMAT_CODE_E enAudFmtCode; /**<Audio coding type*//**<CNcomment:音频编码类型 */ 
    HI_UNF_SAMPLE_RATE_E            enSupportSampleRate[MAX_SAMPE_RATE_NUM]; /**<Audio sample rate*//**<CNcomment:音频采样率 */ 
    HI_U32                          u32SupportSampleRateNum; /**<Audio sample rate num*//**<CNcomment:音频采样率类型 */ 
    HI_U8                           u8AudChannel;      /**<Aud Channel of the coding type*//**<CNcomment:编码类型对应的音频通道能力 */ 
    
    HI_UNF_BIT_DEPTH_E              bSupportBitDepth[MAX_BIT_DEPTH_NUM];/*sample bit depth,when audio format code is HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM*//**<CNcomment:PCM类型下扩展信息为比特深度 */
    HI_U32                          u32SupportBitDepthNum; /*sample bit depth Num,when audio format code is HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM*//**<CNcomment:PCM类型下扩展信息为比特深度个数 */
    HI_U32                          u32MaxBitRate;  /**enter max bit rate,when audio format code is HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3 - HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC**//**<CNcomment:AC3-ATRAC时 扩展信息类型为最大比特速率*/             
}HI_UNF_EDID_AUDIO_INFO_S;

/**EDID Audio speaker enum*//**CNcomment:EDID 音频扬声器配置枚举 */
typedef enum hiUNF_EDID_AUDIO_SPEAKER_E
{
    HI_UNF_EDID_AUDIO_SPEAKER_FL_FR   ,
    HI_UNF_EDID_AUDIO_SPEAKER_LFE     , 
    HI_UNF_EDID_AUDIO_SPEAKER_FC      , 
    HI_UNF_EDID_AUDIO_SPEAKER_RL_RR   ,
    HI_UNF_EDID_AUDIO_SPEAKER_RC      ,
    HI_UNF_EDID_AUDIO_SPEAKER_FLC_FRC , 
    HI_UNF_EDID_AUDIO_SPEAKER_RLC_RRC ,
    HI_UNF_EDID_AUDIO_SPEAKER_FLW_FRW ,
    HI_UNF_EDID_AUDIO_SPEAKER_FLH_FRH ,
    HI_UNF_EDID_AUDIO_SPEAKER_TC      ,
    HI_UNF_EDID_AUDIO_SPEAKER_FCH     ,
    HI_UNF_EDID_AUDIO_SPEAKER_BUTT    ,
} HI_UNF_EDID_AUDIO_SPEAKER_E;

/**EDID MANUFACTURE Info struct*//**CNcomment:EDID制造商信息 */
typedef struct hiUNF_EDID_MANUFACTURE_INFO_E
{
    HI_U8      u8MfrsName[4];            /**<Manufacture name*//**<CNcomment:设备厂商标识 */
    HI_U32     u32ProductCode;           /**<Product code*//**<CNcomment:设备ID */
    HI_U32     u32SerialNumber;          /**<Serial numeber of Manufacture*//**<CNcomment:设备序列号 */
    HI_U32     u32Week;                  /**<the week of manufacture*//**<CNcomment:设备生产日期(周) */
    HI_U32     u32Year;                  /**<the year of manufacture*//**<CNcomment:设备生产日期(年) */
    HI_U8      u8pSinkName[14];          /**<Sink Name*//**<CNcomment:设备名称*/
}HI_UNF_EDID_MANUFACTURE_INFO_S;


/**EDID Display Base Parameters struct*//**CNcomment:EDID 显示屏基本参数 */
typedef struct hiUNF_EDID_BASE_PARA_S
{
    HI_U8      u8MaxImageWidth;               /**<The disp image max width  (0~255)cm*//**<CNcomment: 可显示图像最大宽 */
    HI_U8      u8MaxImageHeight;              /**<The disp image max height (0~255)cm*//**<CNcomment: 可显示图像最大高 */
} HI_UNF_EDID_BASE_DISP_PARA_S;


/**EDID Colorimety Info struct*//**CNcomment:EDID 色调信息结构体 */
typedef struct hiUNF_EDID_COLORIMETRY_S
{
    HI_BOOL    bxvYCC601      ;          /**<The sink is support xcYCC601 or not flag*//**<CNcomment:是否支持xvYCC601颜色格式 */
    HI_BOOL    bxvYCC709      ;          /**<The sink is support xvYCC709 or not flag*//**<CNcomment:是否支持xvYCC709颜色格式 */
    HI_BOOL    bsYCC601       ;          /**<The sink is support sYCC601 or not flag*//**<CNcomment:是否支持sYCC601颜色格式 */
    HI_BOOL    bAdobleYCC601  ;          /**<The sink is support AdobleYCC601 or not flag*//**<CNcomment:是否支持AdobleYCC601颜色格式 */ 
    HI_BOOL    bAdobleRGB     ;          /**<The sink is support AdobleRGB or not flag*//**<CNcomment:是否支持AdobleRGB颜色格式 */  
    HI_BOOL    bBT2020cYCC    ;          /**<The sink is support BT2020cYCC or not flag*//**<CNcomment:是否支持BT2020cYCC颜色格式 */  
    HI_BOOL    bBT2020YCC     ;          /**<The sink is support BT2020YCC or not flag*//**<CNcomment:是否支持BT2020YCC颜色格式 */  
    HI_BOOL    bBT2020RGB     ;          /**<The sink is support BT2020RGB or not flag*//**<CNcomment:是否支持BT2020RGB颜色格式 */  
} HI_UNF_EDID_COLORIMETRY_S;

/**EDID color space Info struct*//**CNcomment:EDID 色彩空间信息结构体 */
typedef struct hiUNF_EDID_COLOR_SPACE_S
{
    HI_BOOL    bRGB444        ;          /**<The sink is support RGB444 or not flag*//**<CNcomment:< 是否支持RGB444显示 */
    HI_BOOL    bYCbCr422      ;          /**<The sink is support YCbCr422 or not flag*//**<CNcomment:< 是否支持YCbCr422显示 */ 
    HI_BOOL    bYCbCr444      ;          /**<The sink is support YCbCr444 or not flag*//**<CNcomment:< 是否支持YCbCr444显示 */
    HI_BOOL    bYCbCr420      ;          /**<The sink is support YCbCr420 or not flag*//**<CNcomment:< 是否支持YCbCr420显示 */
} HI_UNF_EDID_COLOR_SPACE_S;

/**EDID cec address Info struct*//**CNcomment:EDID cec地址信息结构体 */
typedef struct hiUNF_EDID_CEC_ADDRESS_S
{
    HI_BOOL    bPhyAddrValid   ;          /**<the flag of phyiscs address is valid or not*//**<CNcomment:CEC物理地址是否有效标志 */  
    HI_U8      u8PhyAddrA      ;          /**<phyiscs address A of CEC*//**<CNcomment:CEC物理地址A */ 
    HI_U8      u8PhyAddrB      ;          /**<phyiscs address B of CEC*//**<CNcomment:CEC物理地址B */ 
    HI_U8      u8PhyAddrC      ;          /**<phyiscs address C of CEC*//**<CNcomment:CEC物理地址C */ 
    HI_U8      u8PhyAddrD      ;          /**<phyiscs address D of CEC*//**<CNcomment:CEC物理地址D */  
} HI_UNF_EDID_CEC_ADDRESS_S;

/**EDID deep color Info struct*//**CNcomment:EDID 深色信息结构体 */
typedef struct hiUNF_EDID_DEEP_COLOR_S
{
    HI_BOOL    bDeepColorY444   ;         /**<the Deep Color support YCBCR444-bits same with RGB-bits or not*//**<CNcomment:是否支持 YCBCR 4:4:4  Deep Color 与RGB的bit数一致 */
    HI_BOOL    bDeepColor30Bit  ;         /**<the Deep Color support RGB 30 bit  or not *//**<CNcomment:是否支持RGB Deep Color 30bit 模式 */
    HI_BOOL    bDeepColor36Bit  ;         /**<the Deep Color support RGB 36 bit  or not *//**<CNcomment:是否支持RGB Deep Color 36bit 模式 */
    HI_BOOL    bDeepColor48Bit  ;         /**<the Deep Color support RGB 48 bit  or not *//**<CNcomment:是否支持RGB Deep Color 48bit 模式 */
} HI_UNF_EDID_DEEP_COLOR_S;

typedef struct hiUNF_EDID_DEEP_COLOR_Y420_S
{
    HI_BOOL    bY420DeepColor30Bit  ;     /**<the Deep Color support YCBCR420  30 bit  or not *//**<CNcomment:是否支持YCBCR420 Deep Color 30bit 模式 */
    HI_BOOL    bY420DeepColor36Bit  ;     /**<the Deep Color support YCBCR420  36 bit  or not *//**<CNcomment:是否支持YCBCR420 Deep Color 36bit 模式 */
    HI_BOOL    bY420DeepColor48Bit  ;     /**<the Deep Color support YCBCR420  48 bit  or not *//**<CNcomment:是否支持YCBCR420 Deep Color 48bit 模式 */
} HI_UNF_EDID_DEEP_COLOR_Y420_S;

typedef struct hiUNF_EDID_HDCP_S
{
	HI_BOOL     bHdcp14Support  ;		  /**<the HDCP support HDCP1.4 or not *//**<CNcomment:是否支持HDCP1.4 */
    HI_BOOL     bHdcp22Support  ;		  /**<the HDCP support HDCP2.2 or not *//**<CNcomment:是否支持HDCP2.2 */
} HI_UNF_EDID_HDCP_S;

/**HDMI 3d enum type*/
/**CNcomment: HDMI 3d 枚举类型 */
typedef enum hiUNF_EDID_3D_TYPE_E
{
    HI_UNF_EDID_3D_FRAME_PACKETING                 = 0x00, /**<3d type:Frame Packing*//**<CNcomment:3d 模式:帧封装*/
    HI_UNF_EDID_3D_FIELD_ALTERNATIVE               = 0x01, /**<3d type:Field alternative*//**<CNcomment:3d 模式:场交错*/
    HI_UNF_EDID_3D_LINE_ALTERNATIVE                = 0x02, /**<3d type:Line alternative*//**<CNcomment:3d 模式:行交错*/
    HI_UNF_EDID_3D_SIDE_BY_SIDE_FULL               = 0x03, /**<3d type:Side by side full*//**<CNcomment:3d 模式:并排式 左右全场*/
    HI_UNF_EDID_3D_L_DEPTH                         = 0x04, /**<3d type:L+depth*//**<CNcomment:3d 模式:L+DEPTH*/
    HI_UNF_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH = 0x05, /**<3d type:L+depth+Graphics+Graphics-depth*//**<CNcomment:3d 模式:L+depth+Graphics+Graphics-depth*/
    HI_UNF_EDID_3D_TOP_AND_BOTTOM                  = 0x06, /**<3d type:Top and Bottom*//**<CNcomment:3d 模式:上下模式*/
    HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF               = 0x08, /**<3d type:Side by side half*//**<CNcomment:3d 模式:并排式 左右半边*/
    HI_UNF_EDID_3D_BUTT,                          
}HI_UNF_EDID_3D_TYPE_E;

/**3d Info struct*//**CNcomment: 3d 信息结构体 */
typedef struct hiUNF_EDID_3D_INFO_S
{
    HI_BOOL    bSupport3D  ;                         /**<flag of 3d*//**<CNcomment:是否支持3d*/
    HI_BOOL    bSupport3DType[HI_UNF_EDID_3D_BUTT] ; /**<supported 3d type*//**<CNcomment:支持的3d类型*/
} HI_UNF_EDID_3D_INFO_S;

/**EDID detailed Timing Info struct*//**CNcomment: EDID 详细时序*/
typedef struct hiUNF_EDID_TIMING_S
{
    HI_U32                        u32VFB;          /**<vertical front blank*//**<CNcomment:垂直前消隐*/
    HI_U32                        u32VBB;          /**<vertical back blank*//**<CNcomment:垂直后消隐*/
    HI_U32                        u32VACT;         /**<vertical active area*//**<CNcomment:垂直有效区*/
    HI_U32                        u32HFB;          /**<horizonal front blank*//**<CNcomment:水平前消隐*/
    HI_U32                        u32HBB;          /**<horizonal back blank*//**<CNcomment:水平后消隐*/
    HI_U32                        u32HACT;         /**<horizonal active area*/ /**<CNcomment:水平有效区*/
    HI_U32                        u32VPW;          /**<vertical sync pluse width*//**<CNcomment:垂直脉冲宽度*/
    HI_U32                        u32HPW;          /**<horizonal sync pluse width*/ /**<CNcomment:水平脉冲宽度*/
    HI_BOOL                       bIDV;            /**< flag of data valid signal is needed flip*//**<CNcomment:有效数据信号是否翻转*/
    HI_BOOL                       bIHS;            /**<flag of horizonal sync pluse is needed flip*//**<CNcomment:水平同步脉冲信号是否翻转*/
    HI_BOOL                       bIVS;            /**<flag of vertical sync pluse is needed flip*//**<CNcomment:垂直同步脉冲信号是否翻转*/
    HI_U32                        u32ImageWidth;   /**<image width *//**<CNcomment:图像宽*/
    HI_U32                        u32ImageHeight;  /**<image height *//**<CNcomment:图像高 */
    HI_U32                        u32AspectRatioW; /**<aspect ratio width *//**<CNcomment:宽高比 */
    HI_U32                        u32AspectRatioH; /**<aspect ratio height *//**<CNcomment:宽高比 */
    HI_BOOL                       bInterlace;      /**<flag of interlace *//**<CNcomment:逐隔行检测标记 */
    HI_S32                        u32PixelClk;     /**<pixelc clk for this timing *//**<CNcomment:当前制式对应的时钟 */
} HI_UNF_EDID_TIMING_S;

/**the max audio capability count*//**CNcomment:最大音频能力集个数 */
#define HI_UNF_EDID_MAX_AUDIO_CAP_COUNT  16

/**EDID Dolby capability in VSVDB version 0*//**CNcomment: EDID VSVDB版本0中Dolby能力描述*/
typedef struct hiUNF_EDID_DOLBY_CAPS_VER_0_S
{
    HI_U8       u8DMmajorVersion;       /**<the major version of display management implemented. only support when u8VSVDBVersion = 0.*/
    HI_U8       u8DMminorVersion;       /**<the minor version of display management implemented. only support when u8VSVDBVersion = 0.*/    
    HI_U16      u16White_X;             /**<white point chromaticity coordinate x   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N), only support when u8VSVDBVersion = 0. */
    HI_U16      u16White_Y;             /**<white point  chromaticity coordinate y  ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N), only support when u8VSVDBVersion = 0.*/

}HI_UNF_EDID_DOLBY_CAPS_VER_0_S;

/**EDID Dolby capability in VSVDB version 1*//**CNcomment: EDID VSVDB版本1中Dolby能力描述*/
typedef struct hiUNF_EDID_DOLBY_CAPS_VER_1_S
{
    HI_U8       u8DMVersion;            /**<0:based on display management v2.x; 1:based on the video and blending pipeline v3.x; 2-7: reserved. only support when u8VSVDBVersion = 1. */
    HI_BOOL     bColorimetry;           /**<this bit is valid only u8VSVDBVersion = 1. 0: Dolby Vision HDMI sink's colorimetry is close to Rec.709, 1: EDR HDMI sink's colorimetry is close to P3, if Byte[9] to Byte[14] are present, ignores this bit. *//**<CNcomment:若为0，Dolby Vision Sink设备色域接近Rec.709，若为1，Dolby Vision Sink设备色域接近P3，但如果Byte[9]到Byte[14]有声明，则忽略此标志。*/
}HI_UNF_EDID_DOLBY_CAPS_VER_1_S;

/**EDID union of Dolby capability.*//**CNcomment: EDID Dolby能力集联合体*/
typedef union hiUNF_EDID_DOLBY_CAPS_U
{
    HI_UNF_EDID_DOLBY_CAPS_VER_0_S stDolbyCapsVer0;
    HI_UNF_EDID_DOLBY_CAPS_VER_1_S stDolbyCapsVer1;
}HI_UNF_EDID_DOLBY_CAPS_U;

/**EDID Dolby capability.see <Dolby  Vision  HDMI Transmission Specification> *//**CNcomment: EDID Dolby能力集，参考Dolby  Vision  HDMI Transmission Specification*/
typedef struct hiUNF_EDID_DOLBY_CAP_S
{
    /* CEA-861-F VCDB */
    HI_BOOL     bYccQrangeSelectable;   /**<flag of RGB Quantization Range selectable.when HI_TRUE,sink expect full/limited range is selectable;or sink expect default range. *//**<CNcomment:Sink要求RGB量化范围是否满/限制 范围可选。HI_TRUE表示sink要求范围可选，HI_FALSE表示sink要求默认范围。*/
    HI_BOOL     bRgbQrangeSelectable;   /**<flag of YCC Quantization Range selectable.when HI_TRUE,sink expect full/limited range is selectable;or sink expect default range. *//**<CNcomment:Sink要求YCC量化范围是否满/限制 范围可选。HI_TRUE表示sink要求范围可选，HI_FALSE表示sink要求默认范围。*/    

    /* Dolby  Vision  HDMI Transmission Specification */
    HI_U8       u8VSVDBVersion;         /**<the version of VSVDB, if this equal to 1, the value of u16MinLuminance and u16MaxLuminance only 7 bits, the Rx, Ry, Gx, Gy, Bx, By only 8 bits. see <Dolby Vision HDMI Transmission Specification issue 2.6>*//**<CNcomment:VSVDB版本，如果等于1，u16MinLuminance和u16MaxLuminance有效数据为7bit，Rx, Ry, Gx, Gy, Bx, By均为8bit。*/
	HI_BOOL     bYUV422_12Bit;          /**<support(HI_TRUE) or not support(HI_FALSE) a YUV422-12Bit dolby singal.*//**<CNcomment:是否支持YUV422-12Bit的Dolby信号*/
    HI_BOOL     b2160P60;               /**<capable of processing a max timming 3840X2160p60(HI_TRUE) /3840X2160p30(HI_FALSE) *//**<CNcomment:是否支持YUV422-12Bit的Dolby信号*/
    HI_BOOL     bGlobalDimming;         /**<support(HI_TRUE) or not support(HI_FALSE) global dimming. *//**<CNcomment:是否支持global dimming，支持(HI_TRUE)，不支持(HI_FALSE)*/

    HI_U16      u16Red_X;               /**<red primary chromaticity coordinate x   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Red_Y;               /**<red primary chromaticity coordinate y   ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Green_X;             /**<green primary chromaticity coordinate x ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Green_Y;             /**<green primary chromaticity coordinate y ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Blue_X;              /**<blue primary chromaticity coordinate x  ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */
    HI_U16      u16Blue_Y;              /**<blue primary chromaticity coordinate y  ,bit[11:0]valid.Real value =SUM OF bit[N]*2^-(12-N) */

    HI_U16      u16MinLuminance;        /**<minimum luminance value of the target display ,[0,4095] cd/m^2 */
    HI_U16      u16MaxLuminance;        /**<maximum luminance value of the target display,[0,4095] cd/m^2 */

    HI_UNF_EDID_DOLBY_CAPS_U uOtherCaps; /**<other caps in Dolby VSVDB. distinction between V0(u8VSVDBVersion=0) and V1(u8VSVDBVersion=1).*//**<CNcomment:Dolby的其他能力，依据Dolby VSVDB版本V0(u8VSVDBVersion=0)或V1来区分(u8VSVDBVersion=1)*/

}HI_UNF_EDID_DOLBY_CAP_S;

/**EDID EOTF support. *//**CNcomment:EDID 支持的EOTF类型 */
typedef struct hiUNF_EDID_EOTF_S
{
    HI_BOOL bEotfSdr;               /**<Traditional gamma - SDR Luminance Range. *//**<CNcomment:Sink设备是否支持SDR EOTF。*/
    HI_BOOL bEotfHdr;               /**<Traditional gamma - HDR Luminance Range. *//**<CNcomment:Sink设备是否支持HDR EOTF。*/
    HI_BOOL bEotfSmpteSt2048;       /**<SMPTE ST 2084 *//**<CNcomment:Sink设备是否支持SmpteSt2048 EOTF。*/
    HI_BOOL bEotfFuture;            /**<Future EOTF *//**<CNcomment:Sink设备是否支持其他EOTF类型。*/
}HI_UNF_EDID_EOTF_S;

/**EDID Supported Static Metadata Descriptor *//**CNcomment:EDID 支持的Metadata类型 */
typedef struct hiUNF_EDID_HDR_METADATA_TYPE_S
{
    HI_BOOL bDescriptorType1;       /**<Support Static Metadata Type 1 or not.*//**<CNcomment:Sink设备是否支持metadata类型1。*/
}HI_UNF_EDID_HDR_METADATA_TYPE_S;

/**EDID HDR capability,see <CEA-861.3-2015> *//**CNcomment:EDID HDR能力集，参考CEA-861.3-2015 */
typedef struct hiUNF_EDID_HDR_CAP_S{

    HI_UNF_EDID_EOTF_S                  stEotf;                 /**<EOTF support. *//**<CNcomment:Sink设备支持的EOTF类型。*/
    HI_UNF_EDID_HDR_METADATA_TYPE_S     stMetadata;             /**<static metadata descriptor type(ID) support. *//**<CNcomment:Sink设备支持的metadata类型。*/
    HI_U8                               u8MaxLuminance_CV;      /**<Desired Content Max Luminance Data. Real value(in 1cd/m^2) = 50.2^(CV/32) *//**<CNcomment:Sink设备支持最大亮度值。*/
    HI_U8                               u8AverageLumin_CV;      /**<Desired Content Max Frame-average Luminance. Real value(in 1cd/m^2) = 50.2^(CV/32) *//**<CNcomment:Sink设备支持平均亮度值。*/
    HI_U8                               u8MinLuminance_CV;      /**<Desired Content Min Luminance Data. Real value(in 1cd/m^2) = u8MaxLuminance_CV * (u8MinLuminance_CV/255)^2 / 100  *//**<CNcomment:Sink设备支持最小亮度值。*/  
}HI_UNF_EDID_HDR_CAP_S;

/**HDMI sink capability of interface*/
/**CNcomment: HDMI sink 接口能力集 */
typedef struct hiUNF_EDID_BASE_INFO_S
{
    HI_BOOL                         bSupportHdmi;           /**<The Device suppot HDMI or not,the device is DVI when nonsupport HDMI*//**<CNcomment:设备是否支持HDMI，如果不支持，则为DVI设备.*/
    HI_UNF_ENC_FMT_E                enNativeFormat;         /**<The sink native video format*//**<CNcomment:显示设备物理分辨率 */
    HI_BOOL                         bSupportFormat[HI_UNF_ENC_FMT_BUTT]; /**<video capability,HI_TRUE:support the video display format;HI_FALSE:nonsupport the video display foramt*//**<CNcomment:视频能力集,HI_TRUE表示支持这种显示格式，HI_FALSE表示不支持 */
    HI_UNF_EDID_3D_INFO_S           st3DInfo;               /**<3d Info*//**<CNcomment:3d 能力集 */
    HI_UNF_EDID_DEEP_COLOR_S        stDeepColor;            /**<YCBCR/RGB deep color Info*//**<CNcomment:YCBCR/RGB deep color 能力集*/
    HI_UNF_EDID_COLORIMETRY_S       stColorMetry;           /**<colorimetry Info*//**<CNcomment:色域能力集 */
    HI_UNF_EDID_COLOR_SPACE_S       stColorSpace;           /**<color space Info*//**<CNcomment:颜色空间能力集 */            
    
    HI_UNF_EDID_AUDIO_INFO_S        stAudioInfo[HI_UNF_EDID_MAX_AUDIO_CAP_COUNT];   /**<audio Info*//**<CNcomment:音频能力集 */
    HI_U32                          u32AudioInfoNum;                                /**<num of audio Info*//**<CNcomment:音频 能力集个数 */
    HI_BOOL                         bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_BUTT];/**<speaker Info*//**<CNcomment:speaker 能力集 */
    
    HI_U8                           u8ExtBlockNum;          /**<edid extend block num Info*//**<CNcomment:edid扩展块个数 */
    
    HI_U8                           u8Version;              /**<the version of manufacture*//**<CNcomment:设备版本号 */
    HI_U8                           u8Revision;             /**<the revision of manufacture*//**<CNcomment:设备子版本号 */
    HI_UNF_EDID_MANUFACTURE_INFO_S  stMfrsInfo;
 
    HI_UNF_EDID_CEC_ADDRESS_S       stCECAddr;              /**<cec address Info*//**<CNcomment:cec地址信息 */
    HI_BOOL                         bSupportDVIDual;        /**<the DVI support dual-link or not *//**<CNcomment:是否支持 DVI dual-link 操作 */
    HI_BOOL                         bSupportsAI;            /**<support AI or not *//**<CNcomment:是否支持 Supports_AI 模式 */
    HI_UNF_EDID_TIMING_S            stPerferTiming;         /**<first detailed timing Info*//**<CNcomment:VESA最佳详细制式信息 */          
    HI_U32                          u32MaxTMDSClock;        /**<Max TMDS clock in MHz*//**<CNcomment:支持的最大TMDS时钟(MHz) */           
    HI_BOOL                         bSupportY420Format[HI_UNF_ENC_FMT_BUTT];/**<video capability,HI_TRUE:support the video display format in YCBCR420;HI_FALSE:nonsupport the video display foramt  in YCBCR420*//**<CNcomment:视频能力集,HI_TRUE表示支持这种显示格式的YCBCR420模式，HI_FALSE表示不支持 */
    HI_UNF_EDID_DEEP_COLOR_Y420_S   stY420DeepColor;        /**<YCBCR deep color Info*//**<CNcomment:YCBCR420 deep color 能力集 */

    HI_BOOL                         bDolbySupport;          /**<Dolby support flag.When VSVDB is indicated,flag HI_TRUE;or,HI_FALSE.*//**<CNcomment:读到VSVDB，为HI_TRUE;否则为HI_FALSE.*/
    HI_UNF_EDID_DOLBY_CAP_S         stDolby;                /**<Dolby capability*//**<CNcomment:Dolby能力集 */
    HI_BOOL                         bHdrSupport;            /**<HDR support flag.When HDR Static Metadata Data Block is indicated,flag HI_TRUE;or,HI_FALSE.*//**<CNcomment:读到HDR 静态元数据块，为HI_TRUE;否则为HI_FALSE.*/
    HI_UNF_EDID_HDR_CAP_S           stHdr;                  /**<HDR capability*//**<CNcomment:HDR能力集 */
    HI_UNF_EDID_HDCP_S				stHDCPSupport;			/**<HDCP capability,HI_TRUE:support the HDCP version;HI_FALSE:nonsupport the HDCP version*//**<CNcomment:HDCP能力,HI_TRUE表示支持这种HDCP版本，HI_FALSE表示不支持 */
    HI_UNF_EDID_BASE_DISP_PARA_S	stBaseDispPara;		    /**<the parameters of display*//**<CNcomment: 显示屏参数 */
} HI_UNF_EDID_BASE_INFO_S;

/**enum define about Video port*/
/**CNcomment:定义视频接口*/
typedef enum hiUNF_EDID_VIDEO_PORT_E
{
    HI_UNF_EDID_VIDEO_PORT_VGA   = 0x00,/**<VGA port *//**<CNcomment: VGA接口 */
    HI_UNF_EDID_VIDEO_PORT_DVI,         /**<DVI port *//**<CNcomment: DVI接口 */
    HI_UNF_EDID_VIDEO_PORT_HDMI,        /**<HDMI port *//**<CNcomment: HDMI 接口 */
    HI_UNF_EDID_VIDEO_PORT_BUTT
} HI_UNF_EDID_VIDEO_PORT_E;

/**enum define about timing protocol type*/
/**CNcomment:定义时序的协议类型*/
typedef enum hiUNF_EDID_TIMING_TYPE_E
{
    HI_UNF_EDID_TIMING_TYPE_DMT= 0x00,/**< DMT  protocol Timing *//**<CNcomment: DMT协议时序 */
    HI_UNF_EDID_TIMING_TYPE_861,/**< 861D  protocol Timing *//**<CNcomment: 861D协议时序 */
    HI_UNF_EDID_TIMING_TYPE_CVT,/**< CVT  protocol Timing *//**<CNcomment: CVT协议时序 */
    HI_UNF_EDID_TIMING_TYPE_CVT_RB,/**< CVT_RB  protocol Timing *//**<CNcomment: CVT_RB协议时序 */
    HI_UNF_EDID_TIMING_TYPE_GTF,/**< GTF  protocol Timing *//**<CNcomment: GTF协议时序 */
    HI_UNF_EDID_TIMING_TYPE_BUTT
} HI_UNF_EDID_TIMING_TYPE_E;

/**enum define about timing type*/
/**CNcomment:定义时序的类型*/
typedef enum hiUNF_EDID_TIMING_ATTR_E
{
    HI_UNF_EDID_TIMING_ATTR_NONE = 0x00,/**< None *//**<CNcomment: 无 */
    HI_UNF_EDID_TIMING_ATTR_PREFERRED_TIMING,/**< None *//**<CNcomment: 最佳时序*/
    HI_UNF_EDID_TIMING_ATTR_PREFERRED_VERTICAL_FREQ,/**< None *//**<CNcomment: 最佳刷新率*/
    HI_UNF_EDID_TIMING_ATTR_BUTT
} HI_UNF_EDID_TIMING_ATTR_E;

/**Detailed Timing Definitions Sync Signal Definitions*/
/**CNcomment:详细时序中定义同步类型*/
typedef enum hiUNF_EDID_SYNC_TYPE_E
{
    /*Analog Sync Signal Definitions*/
    HI_UNF_EDID_SYNC_ACS_WS_GREEN    = 0x00,     /**<Analog Composite Sync - Without Serrations - Sync On Green Signal only*//**<CNcomment: 模拟复合同步去垂直同步时期的水平锯齿，绿色通道叠加同步*/
    HI_UNF_EDID_SYNC_ACS_WS_ALL,                 /**<Analog Composite Sync - Without Serrations - Sync On all three (RGB) video signals*//**<CNcomment: 模拟复合同步去垂直同步时期的水平锯齿，三个通道叠加同步*/
    HI_UNF_EDID_SYNC_ACS_DS_GREEN,               /**<Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*//**<CNcomment: 模拟复合同步保持垂直同步时期的水平锯齿，绿色通道叠加同步*/
    HI_UNF_EDID_SYNC_ACS_DS_ALL,                 /**<Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*//**<CNcomment: 模拟复合同步保持垂直同步时期的水平锯齿，三个通道叠加同步*/
    HI_UNF_EDID_SYNC_BACS_WS_GREEN,              /**<Bipolar Analog Composite Sync - Without Serrations; - Sync On Green Signal only*//**<CNcomment: 模拟复合同步去垂直同步时期的水平锯齿，三个通道叠加同步*/
    HI_UNF_EDID_SYNC_BACS_WS_ALL,                /**<Bipolar Analog Composite Sync - Without Serrations; - Sync On all three (RGB) video signals*//**<CNcomment: 模拟复合同步去垂直同步时期的水平锯齿，三个通道叠加同步*/
    HI_UNF_EDID_SYNC_BACS_DS_GREEN,              /**<Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On Green Signal only*//**<CNcomment:  极性模拟复合同步保持垂直同步时期的水平锯齿，绿色通道叠加同步*/
    HI_UNF_EDID_SYNC_BACS_DS_ALL,                /**<Bipolar Analog Composite Sync - With Serrations (H-sync during V-sync); - Sync On all three (RGB) video signals*//**<CNcomment:  极性模拟复合同步保持垂直同步时期的水平锯齿，绿色通道叠加同步*/
    /*Digital Sync Signal Definitions*/
    HI_UNF_EDID_SYNC_DCS_WS,                     /**<Digital Composite Sync - Without Serrations*//**<CNcomment: 数字复合同步，去垂直同步期间行同步锯齿信号*/
    HI_UNF_EDID_SYNC_DCS_DS,                     /**<Digital Composite Sync - With Serrations (H-sync during V-sync)*//**<CNcomment: 数字复合同步，保持垂直同步期间行同步锯齿信号*/
    HI_UNF_EDID_SYNC_DSS_VN_HN,                  /**<Digital Separate Sync Vsync(-) Hsync(-)*//**<CNcomment: 数字复合同步，水平和垂直同步极性(-)(-)*/
    HI_UNF_EDID_SYNC_DSS_VN_HP,                  /**<Digital Separate Sync Vsync(-) Hsync(+)*//**<CNcomment: 数字复合同步，水平和垂直同步极性(-)(+)*/
    HI_UNF_EDID_SYNC_DSS_VP_HN,                  /**<Digital Separate Sync Vsync(+) Hsync(-)*//**<CNcomment: 数字复合同步，水平和垂直同步极性(+)(-)*/
    HI_UNF_EDID_SYNC_DSS_VP_HP,                  /**<Digital Separate Sync Vsync(+) Hsync(+)*//**<CNcomment: 数字复合同步，水平和垂直同步极性(+)(+)*/
    HI_UNF_EDID_SYNC_BUTT
} HI_UNF_EDID_SYNC_TYPE_E;


/**Simple Timing Definitions*/
/**CNcomment:定义简要时序*/
typedef struct hiUNF_EDID_SIMPLE_TIMING_S
{
    HI_U32              u32Vact;    /**<  Active Line *//**<CNcomment: 有效行*/
    HI_U32              u32Hact;/**< Active Pixels *//**<CNcomment: 有效像素*/
    HI_U32              u32VerFreq;/**<Ver Frequency *//**<CNcomment: 刷新率 */
    HI_UNF_EDID_TIMING_TYPE_E          enTimingType;/**< Timing protocol type *//**<CNcomment: 定义时序的协议类型*/
    HI_UNF_EDID_TIMING_ATTR_E          enTimingAttr;/**<timing type *//**<CNcomment: :定义时序的类型*/
} HI_UNF_EDID_SIMPLE_TIMING_S;

/**Detailed Timing Definitions*/
/**CNcomment:定义详细时序*/
typedef struct hiUNF_EDID_DETAIL_TIMING_S		
{
    HI_UNF_EDID_TIMING_S	            stTiming; /**<  Detailed Timing *//**<CNcomment: 详细时序*/
    HI_UNF_EDID_SYNC_TYPE_E             enSyncType;/**Detailed Timing Definitions Sync Signal Definitions*//**CNcomment:详细时序中定义同步类型*/
    HI_UNF_EDID_TIMING_ATTR_E           enTimingAttr;/**<timing type *//**<CNcomment: :定义时序的类型*/
} HI_UNF_EDID_DETAIL_TIMING_S;

/**EDID information*/
/**CNcomment:EDID解析信息*/
typedef struct hiUNF_EDID_INFO_S
{
    HI_UNF_EDID_BASE_INFO_S 		stEDIDBaseInfo;/**< EDID base information *//**<CNcomment: EDID基本信息 */
    HI_UNF_EDID_VIDEO_PORT_E        enVideoPort;                  /**<video port  *//**<CNcomment: 视频接口类型 */
    HI_UNF_EDID_SIMPLE_TIMING_S*	pstSimpleTiming;	/**<Simple timing   *//**<CNcomment: 简要时序*/
    HI_U32                          u32SimpleTimingNum;/**<Simple timing number *//**<CNcomment: 简要时序个数*/
    HI_UNF_EDID_DETAIL_TIMING_S*  	pstDetailTiming;	/**<Simple timing *//**<CNcomment: 详细时序个数*/
    HI_U32                          u32DetailTimingNum;/**<Simple timing number *//**<CNcomment: 详细时序个数*/
} HI_UNF_EDID_INFO_S;



/** @}*/  /** <!-- ==== Structure Definition End ====*/




/******************************* API Declaration *****************************/
/** \addtogroup      EDID*/
/** @{*/  /** <!-- [EDID] */


/** 
\brief parse the edid information. CNcomment:解析EDID数据 CNend
\attention \n
\param[in]  u32EdidLength EDID data length . CNcomment:EDID数据 的结构 长度 CNend
\param[in]  *pu8Edid  EDID data point . CNcomment:EDID数据 的指针CNend
\param[in]  *pstEdidInfo  EDID parse information . CNcomment:EDID解析信息指针CNend

\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_S32 HI_UNF_EDID_EdidParse(HI_U8* pu8Edid, HI_U32 u32EdidLength, HI_UNF_EDID_INFO_S* pstEdidInfo);

/** 
\brief release the edid information. CNcomment:释放EDID信息 CNend
\attention \n
\param[in]  *pstEdidInfo  EDID parse information . CNcomment:EDID解析信息指针CNend
\retval HI_SUCCESS   success.   CNcomment:成功 CNend
\retval please refer to the err code definitino of mpi.CNcomment:请参考MPI错误码 CNend
\see \n
CNcomment:无 CNend
*/
HI_VOID HI_UNF_EDID_EdidRelease(HI_UNF_EDID_INFO_S * pstEdidInfo);


/** @} */  /** <!-- ==== API declaration end ==== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_EDID_H__ */

