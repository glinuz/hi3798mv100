/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

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

//#include "hi_unf_common.h"
//#include "hi_unf_audio.h"
//#include "hi_unf_video.h"

#include "hi_type.h"
#include "hi_unf_disp.h"
#include "hi_debug.h"
#include <uboot.h>
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "exports.h"
#ifdef HI_ADVCA_FUNCTION_RELEASE
#define HI_ERR_HDMI(format, arg...)    
#define HI_INFO_HDMI(format, arg...)  
#else
#define HI_ERR_HDMI(format, arg...)   
#define HI_INFO_HDMI(format, arg...)   
#endif

/**extern begin**/
/**CNcomment: 定义音频采样率枚举*/
typedef enum hiUNF_SAMPLE_RATE_E
{
    HI_UNF_SAMPLE_RATE_UNKNOWN=0,       /**<Unknown*/ /**<CNcomment: 未知采样频率 */
    HI_UNF_SAMPLE_RATE_8K    = 8000,    /**<8 kHz sampling rate*/ /**<CNcomment: 8K采样频率 */
    HI_UNF_SAMPLE_RATE_11K   = 11025,   /**<11.025 kHz sampling rate*/ /**<CNcomment: 11.025K采样频率 */
    HI_UNF_SAMPLE_RATE_12K   = 12000,   /**<12 kHz sampling rate*/ /**<CNcomment: 12K采样频率 */
    HI_UNF_SAMPLE_RATE_16K   = 16000,   /**<16 kHz sampling rate*/ /**<CNcomment: 16K采样频率 */
    HI_UNF_SAMPLE_RATE_22K   = 22050,   /**<22.050 kHz sampling rate*/ /**<CNcomment: 22.050K采样频率 */
    HI_UNF_SAMPLE_RATE_24K   = 24000,   /**<24 kHz sampling rate*/ /**<CNcomment: 24K采样频率 */
    HI_UNF_SAMPLE_RATE_32K   = 32000,   /**<32 kHz sampling rate*/ /**<CNcomment: 32K采样频率 */
    HI_UNF_SAMPLE_RATE_44K   = 44100,   /**<44.1 kHz sampling rate*/ /**<CNcomment: 44.1K采样频率 */
    HI_UNF_SAMPLE_RATE_48K   = 48000,   /**<48 kHz sampling rate*/ /**<CNcomment: 48K采样频率 */
    HI_UNF_SAMPLE_RATE_88K   = 88200,   /**<88.2 kHz sampling rate*/ /**<CNcomment: 88.2K采样频率 */
    HI_UNF_SAMPLE_RATE_96K   = 96000,   /**<96 kHz sampling rate*/ /**<CNcomment: 96K采样频率 */
    HI_UNF_SAMPLE_RATE_176K  = 176400,	/**<176 kHz sampling rate*/ /**<CNcomment: 176K采样频率 */
    HI_UNF_SAMPLE_RATE_192K  = 192000,  /**<192 kHz sampling rate*/ /**<CNcomment: 192K采样频率 */

    HI_UNF_SAMPLE_RATE_BUTT
}HI_UNF_SAMPLE_RATE_E;

/**Defines the bit depth during audio sampling.*/
/**CNcomment: 定义音频采样位宽枚举*/
typedef enum hiUNF_BIT_DEPTH_E
{
    HI_UNF_BIT_DEPTH_UNKNOWN =0,/**<Unknown*/ /**<CNcomment: 未知采样位宽  */
    HI_UNF_BIT_DEPTH_8  = 8,    /**< 8-bit depth*/ /**<CNcomment: 8位采样位宽  */
    HI_UNF_BIT_DEPTH_16 = 16,   /**<16-bit depth*/ /**<CNcomment: 16位采样位宽 */
    HI_UNF_BIT_DEPTH_18 = 18,   /**<18-bit depth*/ /**<CNcomment: 18位采样位宽 */
    HI_UNF_BIT_DEPTH_20 = 20,   /**<20-bit depth*/ /**<CNcomment: 20位采样位宽 */
    HI_UNF_BIT_DEPTH_24 = 24,   /**<24-bit depth*/ /**<CNcomment: 24位采样位宽 */
    HI_UNF_BIT_DEPTH_32 = 32,   /**<32-bit depth*/ /**<CNcomment: 32位采样位宽 */

    HI_UNF_BIT_DEPTH_BUTT
}HI_UNF_BIT_DEPTH_E;

/**extern end**/

typedef enum hiUNF_EDID_AUDIO_FORMAT_CODE_E
{
    HI_UNF_EDID_AUDIO_FORMAT_CODE_RESERVED  = 0x00,        
    HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG1,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MP3,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MPEG2,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_AAC,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_ONE_BIT,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DDP,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DTS_HD,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_MAT,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_DST,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_WMA_PRO,
    HI_UNF_EDID_AUDIO_FORMAT_CODE_BUTT,
  
}HI_UNF_EDID_AUDIO_FORMAT_CODE_E;

#define MAX_SAMPE_RATE_NUM 8
#define MAX_BIT_DEPTH_NUM  6

typedef struct hiUNF_EDID_AUDIO_INFO_S
{
    HI_UNF_EDID_AUDIO_FORMAT_CODE_E enAudFmtCode;
    HI_UNF_SAMPLE_RATE_E            enSupportSampleRate[MAX_SAMPE_RATE_NUM];   /*sample rate*/
    HI_U32                          u32SupportSampleRateNum;
    HI_U8                           u8AudChannel;      /*audio_channel*/
    /*sample bit depth,when audio format code is HI_UNF_EDID_AUDIO_FORMAT_CODE_PCM*/
    HI_UNF_BIT_DEPTH_E              bSupportBitDepth[MAX_BIT_DEPTH_NUM];
    HI_U32                          u32SupportBitDepthNum;
    /**enter max bit rate,when audio format code is HI_UNF_EDID_AUDIO_FORMAT_CODE_AC3 - HI_UNF_EDID_AUDIO_FORMAT_CODE_ATRAC**/
    HI_U32                          u32MaxBitRate;               
}HI_UNF_EDID_AUDIO_INFO_S;

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

typedef struct hiUNF_EDID_MANUFACTURE_INFO_E
{
    HI_U8               u8MfrsName[4];   /**<Manufacture name*//**<CNcomment:设备厂商标识 */
    HI_U32              u32ProductCode;  /**<Product code*//**<CNcomment:设备ID */
    HI_U32              u32SerialNumber; /**<Serial numeber of Manufacture*//**<CNcomment:设备序列号 */
    HI_U32              u32Week;        /**<the week of manufacture*//**<CNcomment:设备生产日期(周) */
    HI_U32              u32Year;        /**<the year of manufacture*//**<CNcomment:设备生产日期(年) */
}HI_UNF_EDID_MANUFACTURE_INFO_S;

typedef struct hiUNF_EDID_COLORIMETRY_S
{
    HI_BOOL    bxvYCC601      ; 
    HI_BOOL    bxvYCC709      ; 
    HI_BOOL    bsYCC601       ; 
    HI_BOOL    bAdobleYCC601  ; 
    HI_BOOL    bAdobleRGB     ;  
} HI_UNF_EDID_COLORIMETRY_S;

typedef struct hiUNF_EDID_COLOR_SPACE_S
{
    HI_BOOL    bRGB444        ;
    HI_BOOL    bYCbCr422      ; 
    HI_BOOL    bYCbCr444      ; 
} HI_UNF_EDID_COLOR_SPACE_S;

typedef struct hiUNF_EDID_CEC_ADDRESS_S
{
    HI_BOOL    bPhyAddrValid   ; 
    HI_U8      u8PhyAddrA      ; 
    HI_U8      u8PhyAddrB      ; 
    HI_U8      u8PhyAddrC      ; 
    HI_U8      u8PhyAddrD      ;  
} HI_UNF_EDID_CEC_ADDRESS_S;

typedef struct hiUNF_EDID_DEEP_COLOR_S
{
    HI_BOOL    bDeepColorY444   ; 
    HI_BOOL    bDeepColor30Bit  ; 
    HI_BOOL    bDeepColor36Bit  ; 
    HI_BOOL    bDeepColor48Bit  ; 
} HI_UNF_EDID_DEEP_COLOR_S;


/**HDMI InfoFrame struct type*/
/**CNcomment: HDMI 信息帧数据结构类型 */
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
    HI_UNF_EDID_3D_BUTT,                           //set 2d mode
}HI_UNF_EDID_3D_TYPE_E;

typedef struct hiUNF_EDID_3D_INFO_S
{
    HI_BOOL    bSupport3D  ; 
    HI_BOOL    bSupport3DType[HI_UNF_EDID_3D_BUTT] ; 
} HI_UNF_EDID_3D_INFO_S;

typedef struct hiUNF_EDID_TIMING_S
{
    HI_U32                        u32VFB;            /**<vertical front blank*//**<CNcomment:垂直前消隐*/
    HI_U32                        u32VBB;            /**<vertical back blank*//**<CNcomment:垂直后消隐*/
    HI_U32                        u32VACT;           /**<vertical active area*//**<CNcomment:垂直有效区*/
    HI_U32                        u32HFB;            /**<horizonal front blank*//**<CNcomment:水平前消隐*/
    HI_U32                        u32HBB;            /**<horizonal back blank*//**<CNcomment:水平后消隐*/
    HI_U32                        u32HACT;           /**<horizonal active area*/ /**<CNcomment:水平有效区*/
    HI_U32                        u32VPW;            /**<vertical sync pluse width*//**<CNcomment:垂直脉冲宽度*/
    HI_U32                        u32HPW;            /**<horizonal sync pluse width*/ /**<CNcomment:水平脉冲宽度*/
    HI_BOOL                       bIDV;            /**< flag of data valid signal is needed flip*//**<CNcomment:有效数据信号是否翻转*/
    HI_BOOL                       bIHS;            /**<flag of horizonal sync pluse is needed flip*//**<CNcomment:水平同步脉冲信号是否翻转*/
    HI_BOOL                       bIVS;            /**<flag of vertical sync pluse is needed flip*//**<CNcomment:垂直同步脉冲信号是否翻转*/
    HI_U32                        u32ImageWidth;
    HI_U32                        u32ImageHeight;
    HI_BOOL                       bInterlace;  
    HI_S32                        u32PixelClk;
} HI_UNF_EDID_TIMING_S;


#define HI_UNF_EDID_MAX_AUDIO_CAP_COUNT  16

/**HDMI sink capability of interface*/
/**CNcomment: HDMI sink 接口能力集 */
typedef struct hiUNF_EDID_BASE_INFO_S
{
    HI_BOOL                         bSupportHdmi;             /**<The Device suppot HDMI or not,the device is DVI when nonsupport HDMI*//**<CNcomment:设备是否支持HDMI，如果不支持，则为DVI设备.*/
    HI_UNF_ENC_FMT_E                enNativeFormat;      /**<The sink native video format*//**<CNcomment:显示设备物理分辨率 */
    HI_BOOL                         bSupportFormat[HI_UNF_ENC_FMT_BUTT]; /**<video capability,HI_TRUE:support the video display format;HI_FALSE:nonsupport the video display foramt*//**<CNcomment:视频能力集,HI_TRUE表示支持这种显示格式，HI_FALSE表示不支持 */
    HI_UNF_EDID_3D_INFO_S           st3DInfo;
    HI_UNF_EDID_DEEP_COLOR_S        stDeepColor;
    HI_UNF_EDID_COLORIMETRY_S       stColorMetry;
    HI_UNF_EDID_COLOR_SPACE_S       stColorSpace;            
    
    HI_UNF_EDID_AUDIO_INFO_S        stAudioInfo[HI_UNF_EDID_MAX_AUDIO_CAP_COUNT];
    HI_U32                          u32AudioInfoNum;
    HI_BOOL                         bSupportAudioSpeaker[HI_UNF_EDID_AUDIO_SPEAKER_BUTT];
    
    HI_U8                           u8ExtBlockNum;
    
    HI_U8                           u8Version;                /**<the version of manufacture*//**<CNcomment:设备版本号 */
    HI_U8                           u8Revision;               /**<the revision of manufacture*//**<CNcomment:设备子版本号 */
    HI_UNF_EDID_MANUFACTURE_INFO_S  stMfrsInfo;
 
    HI_UNF_EDID_CEC_ADDRESS_S       stCECAddr;
    HI_BOOL                         bSupportDVIDual;
    HI_BOOL                         bSupportsAI; 
    HI_UNF_EDID_TIMING_S            stPerferTiming; //for tc

} HI_UNF_EDID_BASE_INFO_S;



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_EDID_H__ */
