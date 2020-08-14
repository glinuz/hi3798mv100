/******************************************************************************

  Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_video.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2009/12/17
    Author      : w58735
    Modification: Created file

*******************************************************************************/

#ifndef __HI_UNF_VIDEO_H__
#define __HI_UNF_VIDEO_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "hi_common.h"

/*************************** Structure Definition ****************************/
/** \addtogroup      VIDEO */
/** @{ */  /** <!-- [VIDEO] */

/**Defines the oversample mode.*/
/**CNcomment: 定义过采样模式*/
typedef enum hiUNF_OVERSAMPLE_MODE_E
{
    HI_UNF_OVERSAMPLE_1X = 0,
    HI_UNF_OVERSAMPLE_2X,
    HI_UNF_OVERSAMPLE_4X,
    HI_UNF_OVERSAMPLE_BUTT,
} HI_UNF_OVERSAMPLE_MODE_E;

/**Defines the pixel width.*/
/**CNcomment: 定义像素位宽*/
typedef enum hiUNF_PIXEL_BITWIDTH_E
{
    HI_UNF_PIXEL_BITWIDTH_8BIT = 0,
    HI_UNF_PIXEL_BITWIDTH_10BIT,
    HI_UNF_PIXEL_BITWIDTH_12BIT,
    HI_UNF_PIXEL_BITWIDTH_BUTT,
} HI_UNF_PIXEL_BITWIDTH_E;

/**Defines color space enum*/
/**CNcomment: 定义颜色空间枚举*/
typedef enum hiUNF_COLOR_SPACE_E
{
    HI_UNF_COLOR_SPACE_UNKNOWN = 0,

    HI_UNF_COLOR_SPACE_BT601_YUV_LIMITED = 0x10,/* ::::Current Used:::: BT.601  */
    HI_UNF_COLOR_SPACE_BT601_YUV_FULL,
    HI_UNF_COLOR_SPACE_BT601_RGB_LIMITED,
    HI_UNF_COLOR_SPACE_BT601_RGB_FULL,

    HI_UNF_COLOR_SPACE_NTSC1953 = 0x20,

    /* These should be useful.  Assume 601 extents. */
    HI_UNF_COLOR_SPACE_BT470_SYSTEM_M = 0x30,
    HI_UNF_COLOR_SPACE_BT470_SYSTEM_BG,

    HI_UNF_COLOR_SPACE_BT709_YUV_LIMITED = 0x40,/* ::::Current Used:::: BT.709 */
    HI_UNF_COLOR_SPACE_BT709_YUV_FULL,
    HI_UNF_COLOR_SPACE_BT709_RGB_LIMITED,
    HI_UNF_COLOR_SPACE_BT709_RGB_FULL,   /* ::::Current Used:::: */

    HI_UNF_COLOR_SPACE_REC709 = 0x50,      /* HD and modern captures. */

    HI_UNF_COLOR_SPACE_SMPT170M= 0x60, /* ITU-R 601 -- broadcast NTSC/PAL */
    HI_UNF_COLOR_SPACE_SMPT240M, /* 1125-Line (US) HDTV */

    HI_UNF_COLOR_SPACE_BT878 = 0x70,    /* broken BT878 extents
                           (601, luma range 16-253 instead of 16-235) */

    HI_UNF_COLOR_SPACE_XVYCC = 0x80,

    /* I know there will be cameras that send this.  So, this is
     * unspecified chromaticities and full 0-255 on each of the
     * Y'CbCr components
     */
    HI_UNF_COLOR_SPACE_JPEG = 0x90,
    HI_UNF_COLOR_SPACE_RGB = 0xa0,

    HI_UNF_COLOR_SPACE_BUTT
} HI_UNF_COLOR_SPACE_E;

/**Defines the RGB range.*/
/**CNcomment: 定义RGB 范围*/
typedef enum hiUNF_RGB_RANGE_E
{
    HI_UNF_RGB_DEFAULT,
    HI_UNF_RGB_LIMIT_RANGE,
    HI_UNF_RGB_FULL_RANGE,
    HI_UNF_RGB_RANGE_BUTT
} HI_UNF_RGB_RANGE_E;
/**Defines the video norm.*/
/**CNcomment: 定义视频制式枚举*/
typedef enum hiUNF_ENC_FMT_E
{
    HI_UNF_ENC_FMT_1080P_60 = 0,     /**<1080p 60 Hz*/
    HI_UNF_ENC_FMT_1080P_50,         /**<1080p 50 Hz*/
    HI_UNF_ENC_FMT_1080P_30,         /**<1080p 30 Hz*/
    HI_UNF_ENC_FMT_1080P_25,         /**<1080p 25 Hz*/
    HI_UNF_ENC_FMT_1080P_24,         /**<1080p 24 Hz*/

    HI_UNF_ENC_FMT_1080i_60,         /**<1080i 60 Hz*/
    HI_UNF_ENC_FMT_1080i_50,         /**<1080i 50 Hz*/

    HI_UNF_ENC_FMT_720P_60,          /**<720p 60 Hz*/
    HI_UNF_ENC_FMT_720P_50,          /**<720p 50 Hz */

    HI_UNF_ENC_FMT_576P_50,          /**<576p 50 Hz*/
    HI_UNF_ENC_FMT_480P_60,          /**<480p 60 Hz*/

    HI_UNF_ENC_FMT_PAL,              /* B D G H I PAL */
    HI_UNF_ENC_FMT_PAL_N,            /* (N)PAL        */
    HI_UNF_ENC_FMT_PAL_Nc,           /* (Nc)PAL       */

    HI_UNF_ENC_FMT_NTSC,             /* (M)NTSC       */
    HI_UNF_ENC_FMT_NTSC_J,           /* NTSC-J        */
    HI_UNF_ENC_FMT_NTSC_PAL_M,       /* (M)PAL        */

    HI_UNF_ENC_FMT_SECAM_SIN,        /**< SECAM_SIN*/
    HI_UNF_ENC_FMT_SECAM_COS,        /**< SECAM_COS*/

    HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING,
    HI_UNF_ENC_FMT_720P_60_FRAME_PACKING,
    HI_UNF_ENC_FMT_720P_50_FRAME_PACKING,

    HI_UNF_ENC_FMT_861D_640X480_60,
    HI_UNF_ENC_FMT_VESA_800X600_60,
    HI_UNF_ENC_FMT_VESA_1024X768_60,
    HI_UNF_ENC_FMT_VESA_1280X720_60,
    HI_UNF_ENC_FMT_VESA_1280X800_60,
    HI_UNF_ENC_FMT_VESA_1280X1024_60,
    HI_UNF_ENC_FMT_VESA_1360X768_60,
    HI_UNF_ENC_FMT_VESA_1366X768_60,
    HI_UNF_ENC_FMT_VESA_1400X1050_60,
    HI_UNF_ENC_FMT_VESA_1440X900_60,
    HI_UNF_ENC_FMT_VESA_1440X900_60_RB,
    HI_UNF_ENC_FMT_VESA_1600X900_60_RB,
    HI_UNF_ENC_FMT_VESA_1600X1200_60,
    HI_UNF_ENC_FMT_VESA_1680X1050_60,
    HI_UNF_ENC_FMT_VESA_1680X1050_60_RB,
    HI_UNF_ENC_FMT_VESA_1920X1080_60,
    HI_UNF_ENC_FMT_VESA_1920X1200_60,
    HI_UNF_ENC_FMT_VESA_1920X1440_60,
    HI_UNF_ENC_FMT_VESA_2048X1152_60,
    HI_UNF_ENC_FMT_VESA_2560X1440_60_RB,
    HI_UNF_ENC_FMT_VESA_2560X1600_60_RB,

    HI_UNF_ENC_FMT_3840X2160_24 = 0x40,
    HI_UNF_ENC_FMT_3840X2160_25,
    HI_UNF_ENC_FMT_3840X2160_30,
    HI_UNF_ENC_FMT_3840X2160_50,
    HI_UNF_ENC_FMT_3840X2160_60,

    HI_UNF_ENC_FMT_4096X2160_24,
    HI_UNF_ENC_FMT_4096X2160_25,
    HI_UNF_ENC_FMT_4096X2160_30,
    HI_UNF_ENC_FMT_4096X2160_50,
    HI_UNF_ENC_FMT_4096X2160_60,

    HI_UNF_ENC_FMT_3840X2160_23_976,
    HI_UNF_ENC_FMT_3840X2160_29_97,
    HI_UNF_ENC_FMT_720P_59_94,
    HI_UNF_ENC_FMT_1080P_59_94,
    HI_UNF_ENC_FMT_1080P_29_97,
    HI_UNF_ENC_FMT_1080P_23_976,
    HI_UNF_ENC_FMT_1080i_59_94,
    HI_UNF_ENC_FMT_BUTT
}HI_UNF_ENC_FMT_E;

/**Defines the mode of the video decoder.*/
/**CNcomment: 定义视频解码器模式枚举*/
typedef enum hiUNF_VCODEC_MODE_E
{
    HI_UNF_VCODEC_MODE_NORMAL = 0,   /**<Decode all frames.*/ /**<CNcomment: 解所有帧*/
    HI_UNF_VCODEC_MODE_IP,           /**<Decode only I frames and P frames.*/ /**<CNcomment: 只解IP帧*/
    HI_UNF_VCODEC_MODE_I,            /**<Decode only I frames.*/ /**<CNcomment: 只解I帧*/
    HI_UNF_VCODEC_MODE_DROP_INVALID_B,  /**<Decode all frames except the first B frame sequence behind I frames */ /**<CNcomment: 解所有帧，除了紧跟着I帧后面的B帧*/
    HI_UNF_VCODEC_MODE_BUTT
} HI_UNF_VCODEC_MODE_E;

/**Defines special control operation of decoder*/
/**CNcomment: 定义解码器解码的特殊控制选项 */
typedef enum hiHI_UNF_VCODEC_CTRL_OPTION_E
{
    HI_UNF_VCODEC_CTRL_OPTION_NORMAL_DPB = 0x0,
    HI_UNF_VCODEC_CTRL_OPTION_SIMPLE_DPB = 0x1
} HI_UNF_VCODEC_CTRL_OPTION_E;

/**Defines the decoding capability (resolution) of the decoder.*/
/**CNcomment: 定义解码器解码能力(分辨率) */
typedef enum hiUNF_VCODEC_CAP_LEVEL_E
{
    HI_UNF_VCODEC_CAP_LEVEL_NULL = 0, /**<Do not decode.*/ /**<CNcomment: 不需要解码 */
    HI_UNF_VCODEC_CAP_LEVEL_QCIF = 0, /**<The resolution of the picture to be decoded is less than or equal to 176x144.*/ /**<CNcomment: 解码的图像大小不超过176*144 */
    HI_UNF_VCODEC_CAP_LEVEL_CIF,      /**<The resolution of the picture to be decoded less than or equal to 352x288.*/ /**<CNcomment: 解码的图像大小不超过352*288 */
    HI_UNF_VCODEC_CAP_LEVEL_D1,       /**<The resolution of the picture to be decoded less than or equal to 720x576.*/ /**<CNcomment: 解码的图像大小不超过720*576 */
    HI_UNF_VCODEC_CAP_LEVEL_720P,     /**<The resolution of the picture to be decoded is less than or equal to 1280x720.*/ /**<CNcomment: 解码的图像大小不超过1280*720 */
    HI_UNF_VCODEC_CAP_LEVEL_FULLHD,   /**<The resolution of the picture to be decoded is less than or equal to 1920x1080.*/ /**<CNcomment: 解码的图像大小不超过1920*1080 */

    HI_UNF_VCODEC_CAP_LEVEL_1280x800, /**<The resolution of the picture to be decoded is less than or equal to 1280x800.*/ /**<CNcomment: 解码的图像大小不超过1280x800*/
    HI_UNF_VCODEC_CAP_LEVEL_800x1280, /**<The resolution of the picture to be decoded is less than or equal to 800x1280.*/ /**<CNcomment: 解码的图像大小不超过800x1280*/
    HI_UNF_VCODEC_CAP_LEVEL_1488x1280, /**<The resolution of the picture to be decoded is less than or equal to 1488x1280.*/ /**<CNcomment: 解码的图像大小不超过1488x1280 */
    HI_UNF_VCODEC_CAP_LEVEL_1280x1488, /**<The resolution of the picture to be decoded is less than or equal to 1280x1488.*/ /**<CNcomment: 解码的图像大小不超过1280x1488 */
    HI_UNF_VCODEC_CAP_LEVEL_2160x1280, /**<The resolution of the picture to be decoded is less than or equal to 2160x1280.*/ /**<CNcomment: 解码的图像大小不超过2160x1280 */
    HI_UNF_VCODEC_CAP_LEVEL_1280x2160, /**<The resolution of the picture to be decoded is less than or equal to 1280x2160.*/ /**<CNcomment: 解码的图像大小不超过1280x2160 */
    HI_UNF_VCODEC_CAP_LEVEL_2160x2160, /**<The resolution of the picture to be decoded is less than or equal to 2160x2160.*/ /**<CNcomment: 解码的图像大小不超过2160x2160 */
    HI_UNF_VCODEC_CAP_LEVEL_4096x2160, /**<The resolution of the picture to be decoded is less than or equal to 4096x2160.*/ /**<CNcomment: 解码的图像大小不超过4096x2160 */
    HI_UNF_VCODEC_CAP_LEVEL_2160x4096, /**<The resolution of the picture to be decoded is less than or equal to 2160x4096.*/ /**<CNcomment: 解码的图像大小不超过2160x4096 */
    HI_UNF_VCODEC_CAP_LEVEL_4096x4096, /**<The resolution of the picture to be decoded is less than or equal to 4096x4096.*/ /**<CNcomment: 解码的图像大小不超过4096x4096 */
    HI_UNF_VCODEC_CAP_LEVEL_8192x4096, /**<The resolution of the picture to be decoded is less than or equal to 8192x4096.*/ /**<CNcomment: 解码的图像大小不超过8192x4096 */
    HI_UNF_VCODEC_CAP_LEVEL_4096x8192, /**<The resolution of the picture to be decoded is less than or equal to 4096x8192.*/ /**<CNcomment: 解码的图像大小不超过4096x8192 */
    HI_UNF_VCODEC_CAP_LEVEL_8192x8192, /**<The resolution of the picture to be decoded is less than or equal to 8192x8192.*/ /**<CNcomment: 解码的图像大小不超过8192x8192 */

    HI_UNF_VCODEC_CAP_LEVEL_BUTT
} HI_UNF_VCODEC_CAP_LEVEL_E;

/**Defines the stream type supported by the decoder.*/
/**CNcomment: 定义解码器支持的码流类型 */
typedef enum hiUNF_VCODEC_TYPE_E
{
    HI_UNF_VCODEC_TYPE_MPEG2,       /**<MPEG2*/
    HI_UNF_VCODEC_TYPE_MPEG4,       /**<MPEG4 DIVX4 DIVX5*/
    HI_UNF_VCODEC_TYPE_AVS,         /**<AVS*/
    HI_UNF_VCODEC_TYPE_H263,        /**<H263*/
    HI_UNF_VCODEC_TYPE_H264,        /**<H264*/
    HI_UNF_VCODEC_TYPE_REAL8,       /**<REAL*/
    HI_UNF_VCODEC_TYPE_REAL9,       /**<REAL*/
    HI_UNF_VCODEC_TYPE_VC1,         /**<VC-1*/
    HI_UNF_VCODEC_TYPE_VP6,         /**<VP6*/
    HI_UNF_VCODEC_TYPE_VP6F,        /**<VP6F*/
    HI_UNF_VCODEC_TYPE_VP6A,        /**<VP6A*/
    HI_UNF_VCODEC_TYPE_MJPEG,       /**<MJPEG*/
    HI_UNF_VCODEC_TYPE_SORENSON,    /**<SORENSON SPARK*/
    HI_UNF_VCODEC_TYPE_DIVX3,       /**<DIVX3*/
    HI_UNF_VCODEC_TYPE_RAW,         /**RAW*/
    HI_UNF_VCODEC_TYPE_JPEG,        /**JPEG,added for VENC*/
    HI_UNF_VCODEC_TYPE_VP8,         /**<VP8*/
    HI_UNF_VCODEC_TYPE_MSMPEG4V1,   /**< MS private MPEG4 */
    HI_UNF_VCODEC_TYPE_MSMPEG4V2,
    HI_UNF_VCODEC_TYPE_MSVIDEO1,    /**< MS video */
    HI_UNF_VCODEC_TYPE_WMV1,
    HI_UNF_VCODEC_TYPE_WMV2,
    HI_UNF_VCODEC_TYPE_RV10,
    HI_UNF_VCODEC_TYPE_RV20,
    HI_UNF_VCODEC_TYPE_SVQ1,        /**< Apple video */
    HI_UNF_VCODEC_TYPE_SVQ3,        /**< Apple video */
    HI_UNF_VCODEC_TYPE_H261,
    HI_UNF_VCODEC_TYPE_VP3,
    HI_UNF_VCODEC_TYPE_VP5,
    HI_UNF_VCODEC_TYPE_CINEPAK,
    HI_UNF_VCODEC_TYPE_INDEO2,
    HI_UNF_VCODEC_TYPE_INDEO3,
    HI_UNF_VCODEC_TYPE_INDEO4,
    HI_UNF_VCODEC_TYPE_INDEO5,
    HI_UNF_VCODEC_TYPE_MJPEGB,
    HI_UNF_VCODEC_TYPE_MVC,
    HI_UNF_VCODEC_TYPE_HEVC,
    HI_UNF_VCODEC_TYPE_DV,
    HI_UNF_VCODEC_TYPE_VP9,
    HI_UNF_VCODEC_TYPE_BUTT
}HI_UNF_VCODEC_TYPE_E;

/**Defines the H264 profile of the video encoder.*/
/**CNcomment: 定义视频解码器 H264协议档次*/
typedef enum hiUNF_H264_PROFILE_E
{
    HI_UNF_H264_PROFILE_BASELINE = 0,   /**<Encode H264 stream by baseline profile.*/ /**<CNcomment: 采用基线档次编码H264码流*/
    HI_UNF_H264_PROFILE_MAIN,           /**<Encode H264 stream by main profile.*/     /**<CNcomment:采用主要档次编码H264码流*/
    HI_UNF_H264_PROFILE_EXTENDED,       /**<Encode H264 stream by extended profile.*/ /**<CNcomment:采用扩展档次编码H264码流*/
    HI_UNF_H264_PROFILE_HIGH,           /**<Encode H264 stream by high profile.*/     /**<CNcomment: 采用高级档次编码H264码流*/
    HI_UNF_H264_PROFILE_BUTT
}HI_UNF_H264_PROFILE_E;


typedef struct hiUNF_VCODEC_VC1_ATTR_S
{
    HI_BOOL  bAdvancedProfile;   /**<Whether the profile is an advanced profile*/ /**<CNcomment: 是否Advanced Profile*/
    HI_U32   u32CodecVersion;    /**<Version number*/ /**<CNcomment: 版本号*/
}HI_UNF_VCODEC_VC1_ATTR_S;

typedef struct hiUNF_VCODEC_VP6_ATTR_S
{
    HI_BOOL  bReversed;     /**<To reverse a picture, set this parameter to 1. In this cases, set it to 0.*/ /**<CNcomment: 图像需要倒转时置1，否则置0*/
    HI_U32   u32DispWidth;  /**<Display Width.*/ /**<CNcomment: 图像的显示宽度*/
    HI_U32   u32DispHeight; /**<Display Height.*/ /**<CNcomment: 图像的显示高度*/
}HI_UNF_VCODEC_VP6_ATTR_S;

typedef union hiUNF_VCODEC_EXTATTR_U
{
    HI_UNF_VCODEC_VC1_ATTR_S stVC1Attr;
    HI_UNF_VCODEC_VP6_ATTR_S stVP6Attr;
}HI_UNF_VCODEC_EXTATTR_U;

/**Defines the attributes of a video decoder.*/
/**CNcomment: 定义视频解码器属性结构*/
typedef struct hiUNF_VCODEC_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E        enType;          /**<Video encoding type*/ /**<CNcomment: 视频编码类型*/
    HI_UNF_VCODEC_EXTATTR_U     unExtAttr;       /**<Extra attributes related to the video encoding type*/ /**<CNcomment: 视频编码类型相关的额外属性*/
    HI_UNF_VCODEC_MODE_E        enMode;          /**<Mode of a video decoder*/ /**<CNcomment: 视频解码器模式模式*/
    HI_U32                      u32ErrCover;     /**<Error concealment threshold of the output frames of a video decoder. The value 0 indicates that no frames are output if an error occurs; the value 100 indicates that all frames are output no matter whether errors occur.*/
                                                 /**<CNcomment: 视频解码器的输出帧错误隐藏门限，0:出现错误即不输出；100:不管错误比例全部输出*/
    HI_U32                      u32Priority;     /**<Priority of a video decoder. The value range is [1, HI_UNF_VCODEC_MAX_PRIORITY]. The value 0 is a reserved value. If you set the value to 0, no error message is displayed, but the value 1 is used automatically. The smaller the value, the lower the priority.*/
                                                 /**<CNcomment: 视频解码器优先级, 取值范围: 大于等于1，小于等于HI_UNF_VCODEC_MAX_PRIORITY, 0是保留值，配置为0不会报错，但是会自动取值为1,数值越小优先级越低 */
    HI_BOOL                     bOrderOutput;    /**<Whether the videos are output by the decoding sequence. You are advised to set this parameter to HI_TRUE in VP mode, and HI_FALSE in other modes.*/
                                                 /**<CNcomment: 是否按解码序输出，VP模式下推荐配置为HI_TRUE, 一般模式下配置为HI_FALSE */
    HI_S32                      s32CtrlOptions;  /**<The value is HI_UNF_VCODEC_CTRL_OPTION_E, or the 'or' value of several enum type*/
                                                 /**<CNcomment: 取值为HI_UNF_VCODEC_CTRL_OPTION_E，或者几个枚举的'或'  */
    HI_VOID*                    pCodecContext;   /**<Private codec context */
}HI_UNF_VCODEC_ATTR_S;


/**Defines the sub stream protocol.*/
/**CNcomment: 定义码流的协议分支枚举*/
typedef enum hiUNF_VIDEO_SUB_STANDARD_E
{
    HI_UNF_VIDEO_SUB_STANDARD_UNKNOWN,       /**<Unknown*/ /**<CNcomment: 未知协议分支*/
    HI_UNF_VIDEO_SUB_STANDARD_MPEG2_MPEG1,   /**<The MPEG2 protocol family is compatible with MPEG1.*/ /**<CNcomment: MPEG2协议族可以兼容MPEG1 */
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_SHV,     /**<The MPEG4 protocol family is compatible with SHV.*/ /**<CNcomment: MPEG4协议族可以兼容SHV*/
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_XVID,    /**<The MPEG4 protocol family includes the sub protocol XVID.*/ /**<CNcomment: MPEG4协议包含XVID分支*/
    HI_UNF_VIDEO_SUB_STANDARD_MPEG4_DIVX,    /**<The MPEG4 protocol family includes the sub protocol DIVX.*/ /**<CNcomment: MPEG4协议包含DIVX分支*/

    HI_UNF_VIDEO_SUB_STANDARD_BUTT
}HI_UNF_VIDEO_SUB_STANDARD_E;

/**Defines the frame rate of the video stream.*/
/**CNcomment: 定义视频码流帧率结构*/
typedef struct hiUNF_VCODEC_FRMRATE_S
{
    HI_U32 u32fpsInteger;     /**<Integral part of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的整数部分, fps */
    HI_U32 u32fpsDecimal;     /**<Fractional part (calculated to three decimal places) of the frame rate (in frame/s)*/
                              /**<CNcomment: 码流的帧率的小数部分（保留3位）, fps */
}HI_UNF_VCODEC_FRMRATE_S;

/**Defines the information about video streams.*/
/**CNcomment: 定义视频码流信息结构*/
typedef struct hiUNF_VCODEC_STREAMINFO_S
{
    HI_UNF_VCODEC_TYPE_E        enVCodecType;   /**<Stream type*/ /**<CNcomment: 码流类型 */
    HI_UNF_VIDEO_SUB_STANDARD_E enSubStandard;  /**<Sub stream protocol*/ /**<CNcomment: 码流的协议分支 */
    HI_U32                      u32SubVersion;  /**<Version of the sub stream protocol*/ /**<CNcomment: 码流子协议版本号 */
    HI_U32                      u32Profile;     /**<Stream profile*/ /**<CNcomment: 码流的profile */
    HI_U32                      u32Level;       /**<Stream level*/ /**<CNcomment: 码流的level */
    HI_UNF_ENC_FMT_E            enDisplayNorm;  /**<Display norm (PAL or NTSC)*/ /**<CNcomment: 显示标准(P/N) */
    HI_BOOL                     bProgressive;   /**<Sampling type (progressive or interlaced)*/ /**<CNcomment: 采样方式(逐行/隔行) */
    HI_U32                      u32AspectWidth; /**<Output aspect ratio: width*/ /**<CNcomment: 输出宽高比之宽值 */
    HI_U32                      u32AspectHeight;/**<Output aspect ratio: height*/ /**<CNcomment: 输出宽高比之高值 */

    HI_U32 u32bps;            /**<Bit rate, in kbit/s*/ /**<CNcomment: 码流的码率, Kbps */
    HI_U32 u32fpsInteger;     /**<Integral part of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的整数部分, fps */
    HI_U32 u32fpsDecimal;     /**<Fractional part (calculated to three decimal places) of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的小数部分（保留3位）, fps */
    HI_U32 u32Width;          /**<Width of the decoded picture*/ /**<CNcomment: 解码图像宽 */
    HI_U32 u32Height;         /**<Height of the decoded picture*/ /**<CNcomment: 解码图像高 */
    HI_U32 u32DisplayWidth;   /**<Width of the displayed picture*/ /**<CNcomment: 显示图像宽 */
    HI_U32 u32DisplayHeight;  /**<Height of the displayed picture*/ /**<CNcomment: 显示图像高 */
    HI_U32 u32DisplayCenterX; /**<Horizontal coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/
                              /**<CNcomment: 显示图像中心横坐标，以原始图像的左上角为坐标原点 */
    HI_U32 u32DisplayCenterY; /**<Vertical coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/
                              /**<CNcomment: 显示图像中心纵坐标，以原始图像的左上角为坐标原点 */
}HI_UNF_VCODEC_STREAMINFO_S;


/**Defines the types of the user data in the video information.*/
/**CNcomment: 定义视频信息中的用户数据类型 */
typedef enum hiHI_UNF_VIDEO_USERDATA_TYPE_E
{
    HI_UNF_VIDEO_USERDATA_UNKNOWN = 0,      /**<Unknown type*/ /**<CNcomment: 未知类型*/
    HI_UNF_VIDEO_USERDATA_DVB1_CC = 0x1,    /**<Closed Caption Data*/ /**<CNcomment: 字幕数据*/
    HI_UNF_VIDEO_USERDATA_DVB1_BAR = 0x2,   /**<Bar Data*/ /**<CNcomment: Bar数据*/
    HI_UNF_VIDEO_USERDATA_AFD = 0x10000,    /**<Active Format Description*/ /**<CNcomment: 模式描述数据*/
    HI_UNF_VIDEO_USERDATA_BUTT
}HI_UNF_VIDEO_USERDATA_TYPE_E;

/**Defines the profile of video broadcasting.*/
/**CNcomment: 定义视频广播的profile枚举*/
typedef enum hiUNF_VIDEO_BROADCAST_PROFILE_E
{
    HI_UNF_VIDEO_BROADCAST_DVB,        /**<Digital video broadcasting (DVB)*/ /**<CNcomment: 数字视频广播DVB*/
    HI_UNF_VIDEO_BROADCAST_DIRECTV,    /**<American live broadcast operator DirecTV*/ /**<CNcomment: 美国直播运营商DirecTV*/
    HI_UNF_VIDEO_BROADCAST_ATSC,       /**<Advanced Television Systems Committee (ATSC)*/ /**<CNcomment: 先进电视制式委员会ATSC（Advanced Television Systems Committee）*/
    HI_UNF_VIDEO_BROADCAST_DVD,        /**<Digital video disc (DVD)*/ /**<CNcomment: 数字视频光盘*/
    HI_UNF_VIDEO_BROADCAST_ARIB,       /**<Association of Radio Industries and Businesses (ARIB)*/ /**<CNcomment: 无线电工业及商业协会规格*/
    HI_UNF_VIDEO_BROADCAST_BUTT
}HI_UNF_VIDEO_BROADCAST_PROFILE_E;

/**Defines the position of the user data in the video information.*/
/**CNcomment: 定义视频信息中用户数据的位置枚举*/
typedef enum hiUNF_VIDEO_USER_DATA_POSITION_E
{
    HI_UNF_VIDEO_USER_DATA_POSITION_UNKNOWN,       /**<Unknown*/ /**<CNcomment: 未知位置*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_SEQ,     /**<The data is parsed from sequences under the MPEG2 protocol.*/ /**<CNcomment: MPEG2协议下，从序列中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_GOP,     /**<The data is parsed from the group of pictures (GOP) under the MPEG2 protocol.*/ /**<CNcomment: MPEG2协议下，从GOP（Group Of Pictures）中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG2_FRAME,   /**<The data is parsed from picture frames under the MPEG2 protocol.*/ /**<CNcomment: MPEG2协议下，从图像帧中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VSOS,    /**<The data is parsed from the sequences of visible objects under the MPEG4 protocol.*/ /**<CNcomment: MPEG4协议下，从可视对像序列中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VSO,     /**<The data is parsed from visible objects under the MPEG4 protocol.*/ /**<CNcomment: MPEG4协议下，从可视对像中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_VOL,     /**<The data is parsed from the video object layer under the MPEG4 protocol.*/ /**<CNcomment: MPEG4协议下，从视频对像层中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_MPEG4_GOP,     /**<The data is parsed from the GOP under the MPEG4 protocol.*/ /**<CNcomment: MPEG4协议下，从GOP中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_H264_REG,      /**<The data is parsed from the user_data_regestered_itu_t_t35() syntax under the H.264 protocol.*/ /**<CNcomment: 从H.264协议的user_data_regestered_itu_t_t35()语法中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_H264_UNREG,    /**<The data is parsed from the user_data_unregestered() syntax under the H.264 protocol.*/ /**<CNcomment: 从H.264协议的user_data_unregestered()语法中解出*/
    HI_UNF_VIDEO_USER_DATA_POSITION_BUTT
}HI_UNF_VIDEO_USER_DATA_POSITION_E;

/**Defines the structure of the user data in the video information.*/
/**CNcomment: 定义视频信息中的用户数据结构*/
typedef struct hiUNF_VIDEO_USERDATA_S
{
    HI_UNF_VIDEO_BROADCAST_PROFILE_E   enBroadcastProfile;   /**<Broadcasting profile of the user data*/ /**<CNcomment: 用户数据的广播profile*/
    HI_UNF_VIDEO_USER_DATA_POSITION_E  enPositionInStream;   /**<Position of the user data in video streams*/ /**<CNcomment: 用户数据在视频流中的位置*/
    HI_U32                             u32Pts;               /**<PTS corresponding to the user data*/ /**<CNcomment: 用户数据对应的时间戳*/
    HI_U32                             u32SeqCnt;            /**<Sequence ID of the user data*/ /**<CNcomment: 用户数据的前一序列数*/
    HI_U32                             u32SeqFrameCnt;       /**<Frame number of the user data*/ /**<CNcomment: 用户数据的前一帧数*/
    HI_U8                              *pu8Buffer;           /**<Initial address of the user data memory, output parameter*/ /**<CNcomment: 用户数据内存区的初始地址,输出参数*/
    HI_U32                             u32Length;            /**<User data size (a multiple of 1 byte)*/ /**<CNcomment: 用户数据的大小，以1byte为单位*/
    HI_BOOL                            bBufferOverflow;      /**<Indicate that whether the user data size exceeds the maximum size defined by MAX_USER_DATA_LEN.*/ /**<CNcomment: 标志用户数据的长度是否超过了MAX_USER_DATA_LEN定义的最大值*/
    HI_BOOL                            bTopFieldFirst;       /**<Top field first flag*/ /**<CNcomment: 顶场优先标志*/
}HI_UNF_VIDEO_USERDATA_S;

/**Defines the parameters of format changing */
/**CNcomment: 定义制式切换时的相关参数*/
typedef struct hiUNF_NORMCHANGE_PARAM_S
{
    HI_UNF_ENC_FMT_E            enNewFormat;            /**<New format*/ /**<CNcomment: 新的制式*/
    HI_U32                      u32ImageWidth;          /**<Width of image*/ /**<CNcomment: 图像宽度*/
    HI_U32                      u32ImageHeight;         /**<Height of image*/ /**<CNcomment: 图像高度*/
    HI_BOOL                     bProgressive;           /**<Sampling type (progressive or interlaced)*/ /**<CNcomment: 采样方式(逐行/隔行) */
    HI_U32                      u32FrameRate;           /**<Frame rate*//**<CNcomment:帧率*/
}HI_UNF_NORMCHANGE_PARAM_S;

/**Defines the video format.*/
/**CNcomment: 定义视频格式枚举*/
typedef enum hiUNF_VIDEO_FORMAT_E
{
    /* Semi-Planner */
    HI_UNF_FORMAT_YUV_SEMIPLANAR_422,       /**<The YUV spatial sampling format is 4:2:2.*/ /**<CNcomment: YUV空间采样格式为4:2:2*/
    HI_UNF_FORMAT_YUV_SEMIPLANAR_420,       /**<The YUV spatial sampling format is 4:2:0, V first.*/ /**<CNcomment: YUV空间采样格式为4:2:0，V在低位*/
    HI_UNF_FORMAT_YUV_SEMIPLANAR_400,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_411,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_444,
    HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV,   /**<The YUV spatial sampling format is 4:2:0,U first.*/ /**<CNcomment: YUV空间采样格式为4:2:0, U在低位*/


    /* Package */
    HI_UNF_FORMAT_YUV_PACKAGE_UYVY,         /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is UYVY.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为UYVY*/
    HI_UNF_FORMAT_YUV_PACKAGE_YUYV,         /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YUYV.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为YUYV*/
    HI_UNF_FORMAT_YUV_PACKAGE_YVYU,         /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YVYU.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为YVYU*/

    /* Planner */
    HI_UNF_FORMAT_YUV_PLANAR_400,
    HI_UNF_FORMAT_YUV_PLANAR_411,
    HI_UNF_FORMAT_YUV_PLANAR_420,
    HI_UNF_FORMAT_YUV_PLANAR_422_1X2,
    HI_UNF_FORMAT_YUV_PLANAR_422_2X1,
    HI_UNF_FORMAT_YUV_PLANAR_444,
    HI_UNF_FORMAT_YUV_PLANAR_410,
    HI_UNF_FORMAT_YUV_TILE_420,	            /**<The YUV spatial sampling format is TILE 4:2:0, V first.*/
    HI_UNF_FORMAT_YUV_TILE_420_UV,          /**<The YUV spatial sampling format is TILE 4:2:0, U first.*/
    HI_UNF_FORMAT_YUV_BUTT,

    HI_UNF_FORMAT_RGB_SEMIPLANAR_444,
    HI_UNF_FORMAT_RGB_BUTT
}HI_UNF_VIDEO_FORMAT_E;

/**Defines the type of the video frame.*/
/**CNcomment: 定义视频帧的类型枚举*/
typedef enum hiUNF_VIDEO_FRAME_TYPE_E
{
    HI_UNF_FRAME_TYPE_UNKNOWN,   /**<Unknown*/ /**<CNcomment: 未知的帧类型*/
    HI_UNF_FRAME_TYPE_I,         /**<I frame*/ /**<CNcomment: I帧*/
    HI_UNF_FRAME_TYPE_P,         /**<P frame*/ /**<CNcomment: P帧*/
    HI_UNF_FRAME_TYPE_B,         /**<B frame*/ /**<CNcomment: B帧*/
	HI_UNF_FRAME_TYPE_IDR,       /**<IDR frame*/ /**<CNcomment: IDR帧*/
    HI_UNF_FRAME_TYPE_BUTT
}HI_UNF_VIDEO_FRAME_TYPE_E;

/**Defines the video frame/field mode.*/
/**CNcomment: 定义视频帧场模式枚举*/
typedef enum hiUNF_VIDEO_FIELD_MODE_E
{
    HI_UNF_VIDEO_FIELD_ALL,        /**<Frame mode*/ /**<CNcomment: 帧模式*/
    HI_UNF_VIDEO_FIELD_TOP,        /**<Top field mode*/ /**<CNcomment: 顶场模式*/
    HI_UNF_VIDEO_FIELD_BOTTOM,     /**<Bottom field mode*/ /**<CNcomment: 底场模式*/
    HI_UNF_VIDEO_FIELD_BUTT
}HI_UNF_VIDEO_FIELD_MODE_E;


/**Defines 3D frame packing type*/
typedef enum hiUNF_VIDEO_FRAME_PACKING_TYPE_E
{
    HI_UNF_FRAME_PACKING_TYPE_NONE,             /**< Normal frame, not a 3D frame */
    HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE,     /**< Side by side */
    HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM,   /**< Top and bottom */
    HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED,  /**< Time interlaced: one frame for left eye, the next frame for right eye */
    HI_UNF_FRAME_PACKING_TYPE_FRAME_PACKING,    /**< frame packing */
    HI_UNF_FRAME_PACKING_TYPE_3D_TILE,          /**< Tile 3D */
    HI_UNF_FRAME_PACKING_TYPE_BUTT
}HI_UNF_VIDEO_FRAME_PACKING_TYPE_E;

typedef struct hiUNF_VIDEO_FRAME_ADDR_S
{
    HI_U32             u32YAddr;    /**<Address of the Y component in the current frame*/ /**<CNcomment: 当前帧Y分量数据的地址*/
    HI_U32             u32CAddr;    /**<Address of the C component in the current frame*/ /**<CNcomment: 当前帧C分量数据的地址*/
    HI_U32             u32CrAddr;   /**<Address of the Cr component in the current frame*/ /**<CNcomment: 当前帧Cr分量数据的地址*/

    HI_U32             u32YStride;  /**<Stride of the Y component*/ /**<CNcomment: Y分量数据的跨幅*/
    HI_U32             u32CStride;  /**<Stride of the C component*/ /**<CNcomment: C分量数据的跨幅*/
    HI_U32             u32CrStride; /**<Stride of the Cr component*/ /**<CNcomment: Cr分量数据的跨幅*/
}HI_UNF_VIDEO_FRAME_ADDR_S;

typedef struct hiUNF_VIDEO_FRAME_INFO_S
{
    HI_U32                              u32FrameIndex;      /**<Frame index ID of a video sequence*/ /**<CNcomment: 视频序列中的帧索引号*/
    HI_UNF_VIDEO_FRAME_ADDR_S           stVideoFrameAddr[2];
    HI_U32                              u32Width;           /**<Width of the source picture*/ /**<CNcomment: 原始图像宽*/
    HI_U32                              u32Height;          /**<Height of the source picture*/ /**<CNcomment: 原始图像高*/
    HI_U32                              u32SrcPts;          /**<Original PTS of a video frame*/ /**<CNcomment: 视频帧的原始时间戳*/
    HI_U32                              u32Pts;             /**<PTS of a video frame*/ /**<CNcomment: 视频帧的时间戳*/
    HI_U32                              u32AspectWidth;
    HI_U32                              u32AspectHeight;
    HI_UNF_VCODEC_FRMRATE_S             stFrameRate;

    HI_UNF_VIDEO_FORMAT_E               enVideoFormat;      /**<Video YUV format*/ /**<CNcomment: 视频YUV格式*/
    HI_BOOL                             bProgressive;       /**<Sampling type (progressive or interlaced)*/ /**<CNcomment: 采样方式(逐行/隔行) */
    HI_UNF_VIDEO_FIELD_MODE_E           enFieldMode;        /**<Frame or field encoding mode*/ /**<CNcomment: 帧或场编码模式*/
    HI_BOOL                             bTopFieldFirst;     /**<Top field first flag*/ /**<CNcomment: 顶场优先标志*/
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E   enFramePackingType; /**<3D frame packing type*/
    HI_U32                              u32Circumrotate;    /**<Need circumrotate, 1 need */
    HI_BOOL                             bVerticalMirror;
    HI_BOOL                             bHorizontalMirror;
    HI_U32                              u32DisplayWidth;    /**<Width of the displayed picture*/ /**<CNcomment: 显示图像宽*/
    HI_U32                              u32DisplayHeight;   /**<Height of the displayed picture*/ /**<CNcomment: 显示图像高*/
    HI_U32                              u32DisplayCenterX;  /**<Horizontal coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/ /**<CNcomment: 显示中心x坐标，原始图像左上角为坐标原点*/
    HI_U32                              u32DisplayCenterY;  /**<Vertical coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/ /**<CNcomment: 显示中心y坐标，原始图像左上角为坐标原点*/
    HI_U32                              u32ErrorLevel;      /**<Error percentage of a decoded picture, ranging from 0% to 100%*/ /**<CNcomment: 一幅解码图像中的错误比例，取值为0～100*/
    HI_U32                              u32Private[64];     /**<Private data for every frame: 
                                                                u32Private[0]: securty flag, if u32Private[0] == 1, this is a secure frame;
                                                                */
    
    
}HI_UNF_VIDEO_FRAME_INFO_S;

typedef struct hiUNF_VIDEO_FRM_STATUS_INFO_S
{
    HI_UNF_VIDEO_FRAME_TYPE_E enVidFrmType; /**<Type of video frame*/ /**<CNcomment: 视频帧类型*/ 
    HI_U32 u32VidFrmStreamSize;             /**<Size of video frame*/ /**<CNcomment: 视频帧大小*/ 
    HI_U32 u32VidFrmQP;                     /**<QP of video frame*/ /**<CNcomment: 视频帧QP*/ 
    HI_U32 u32VidFrmPTS;                    /**<PTS of video frame*/ /**<CNcomment: 视频帧pts*/
    HI_U32 u32MaxMV;                        /**<MAX MV*/ /**<CNcomment: 视频帧最大MV*/
    HI_U32 u32MinMV;                        /**<MIN MV*/ /**<CNcomment: 视频帧最小MV*/
    HI_U32 u32AvgMV;                        /**<AVG MV*/ /**<CNcomment: 视频帧MV平均值*/
    HI_U32 u32SkipRatio;                    /**<MB ratio */ /**<CNcomment: 视频帧MB比例0~100*/
}HI_UNF_VIDEO_FRM_STATUS_INFO_S;


/** @} */  /** <!-- ==== Structure Definition End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __HI_UNF_VIDEO_ H*/
