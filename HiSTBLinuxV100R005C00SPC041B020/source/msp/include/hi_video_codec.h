/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_video_codec.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/07/31
  Description   : Common definitions of HI_CODEC(video).
                  The codec wants to register to HI_CODEC need to adapt to HI_CODEC_S.
  History       :
  1.Date        : 2012/07/31
    Author      : l00185424
    Modification: Created file

*******************************************************************************/

#ifndef __HI_VIDEO_CODEC_H__
#define __HI_VIDEO_CODEC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"

/*************************** Structure Definition ****************************/
/** \addtogroup  VCODEC  */
/** @{ */  /** <!--[VCODEC]  */

/*Codec error defination */
/**Operate fail due to insufficient resources(eg. no enough memory) */
/**CNcomment: 资源不足，(编)解码失败*/ 
#define HI_ERR_CODEC_NOENOUGHRES    (HI_S32)(0x80510001)
/**Parameter invalid */
/**CNcomment: 参数无效*/ 
#define HI_ERR_CODEC_INVALIDPARAM   (HI_S32)(0x80510002)
/**The (en)decoding fails due to incorrect input(streams or frames) */
/**CNcomment: 输入数据有误，(编)解码结束*/ 
#define HI_ERR_CODEC_INPUTCORRUPT   (HI_S32)(0x80510003)
/**The (en)decoding ends due to insufficient input data(streams or frames) */
/**CNcomment: 输入数据不足，(编)解码结束*/ 
#define HI_ERR_CODEC_NOENOUGHDATA   (HI_S32)(0x80510004)
/**The (en)decoding mode is not supported */
/**CNcomment: 不支持的(编)解码类型*/ 
#define HI_ERR_CODEC_INVALIDMODE    (HI_S32)(0x80510005)
/**Operate fail */
/**CNcomment: 操作失败*/ 
#define HI_ERR_CODEC_OPERATEFAIL    (HI_S32)(0x80510006)
/**Version unmatch*/
/**CNcomment: 版本不匹配*/ 
#define HI_ERR_CODEC_VERSIONUNMATCH (HI_S32)(0x80510007)
/**Unsupport operation */
/**CNcomment: 不支持的操作*/
#define HI_ERR_CODEC_UNSUPPORT      (HI_S32)(0x80510008)
/**Decorder busy */
/**CNcomment: 解码器忙*/ 
#define HI_ERR_CODEC_BUSY           (HI_S32)(0x80510009)
/**Unknown error */
/**CNcomment: 未知错误*/ 
#define HI_ERR_CODEC_UNKNOWN        (HI_S32)(0x80510010)

/**Codec ID */
/**CNcomment: 协议类型 */
typedef enum hiCODEC_ID_E
{
    HI_CODEC_ID_NONE = 0, 
        
    HI_CODEC_ID_VIDEO_MPEG1,
    HI_CODEC_ID_VIDEO_MPEG2,
    HI_CODEC_ID_VIDEO_MPEG2_XVMC,
    HI_CODEC_ID_VIDEO_MPEG4,      /**<MPEG4 DIVX4 DIVX5*/
    HI_CODEC_ID_VIDEO_MSMPEG4V1,
    HI_CODEC_ID_VIDEO_MSMPEG4V2,
    HI_CODEC_ID_VIDEO_MSMPEG4V3,

    HI_CODEC_ID_VIDEO_DIVX3,
    HI_CODEC_ID_VIDEO_XVID,

    HI_CODEC_ID_VIDEO_H261,
    HI_CODEC_ID_VIDEO_H263,
    HI_CODEC_ID_VIDEO_H263P,
    HI_CODEC_ID_VIDEO_H263I,
    HI_CODEC_ID_VIDEO_H264,
    HI_CODEC_ID_VIDEO_FFH264,

    HI_CODEC_ID_VIDEO_AVS,

    HI_CODEC_ID_VIDEO_REAL8,
    HI_CODEC_ID_VIDEO_REAL9,

    HI_CODEC_ID_VIDEO_VC1,
    HI_CODEC_ID_VIDEO_WMV1,
    HI_CODEC_ID_VIDEO_WMV2,
    HI_CODEC_ID_VIDEO_WMV3,

    HI_CODEC_ID_VIDEO_MSVIDEO1,

    HI_CODEC_ID_VIDEO_VP3,
    HI_CODEC_ID_VIDEO_VP5,
    HI_CODEC_ID_VIDEO_VP6,
    HI_CODEC_ID_VIDEO_VP6F,
    HI_CODEC_ID_VIDEO_VP6A,
    HI_CODEC_ID_VIDEO_VP8,
    HI_CODEC_ID_VIDEO_VP9,

    HI_CODEC_ID_VIDEO_RAW,
    HI_CODEC_ID_VIDEO_SORENSON,   /**<SORENSON SPARK*/

	HI_CODEC_ID_VIDEO_HEVC,
    HI_CODEC_ID_VIDEO_RV10,
    HI_CODEC_ID_VIDEO_RV20,
    HI_CODEC_ID_VIDEO_RV30,
    HI_CODEC_ID_VIDEO_RV40,
    
    HI_CODEC_ID_VIDEO_SVQ1,
    HI_CODEC_ID_VIDEO_SVQ3,

    HI_CODEC_ID_VIDEO_CINEPAK,
    HI_CODEC_ID_VIDEO_INDEO2,
    HI_CODEC_ID_VIDEO_INDEO3,
    HI_CODEC_ID_VIDEO_INDEO4,
    HI_CODEC_ID_VIDEO_INDEO5,

    HI_CODEC_ID_VIDEO_JPEG,
    HI_CODEC_ID_VIDEO_JPEGLS,
    HI_CODEC_ID_VIDEO_JPEG2000,
    HI_CODEC_ID_VIDEO_MJPEG,
    HI_CODEC_ID_VIDEO_MJPEGB,
    HI_CODEC_ID_VIDEO_LJPEG,

    HI_CODEC_ID_VIDEO_TIFF,
    HI_CODEC_ID_VIDEO_GIF,
    HI_CODEC_ID_VIDEO_PNG,

    HI_CODEC_ID_VIDEO_SP5X,
    
    HI_CODEC_ID_VIDEO_FLV1,
    HI_CODEC_ID_VIDEO_HUFFYUV,
    HI_CODEC_ID_VIDEO_CYUV,
    HI_CODEC_ID_VIDEO_THEORA,
    HI_CODEC_ID_VIDEO_ASV1,
    HI_CODEC_ID_VIDEO_ASV2,
    HI_CODEC_ID_VIDEO_FFV1,
    HI_CODEC_ID_VIDEO_4XM,
    HI_CODEC_ID_VIDEO_VCR1,
    HI_CODEC_ID_VIDEO_CLJR,
    HI_CODEC_ID_VIDEO_MDEC,
    HI_CODEC_ID_VIDEO_ROQ,
    HI_CODEC_ID_VIDEO_INTERPLAY_VIDEO,
    HI_CODEC_ID_VIDEO_XAN_WC3,
    HI_CODEC_ID_VIDEO_XAN_WC4,
    HI_CODEC_ID_VIDEO_RPZA,
    HI_CODEC_ID_VIDEO_WS_VQA,
    HI_CODEC_ID_VIDEO_MSRLE,
    HI_CODEC_ID_VIDEO_IDCIN,
    HI_CODEC_ID_VIDEO_8BPS,
    HI_CODEC_ID_VIDEO_SMC,
    HI_CODEC_ID_VIDEO_FLIC,
    HI_CODEC_ID_VIDEO_TRUEMOTION1,
    HI_CODEC_ID_VIDEO_VMDVIDEO,
    HI_CODEC_ID_VIDEO_MSZH,
    HI_CODEC_ID_VIDEO_ZLIB,
    HI_CODEC_ID_VIDEO_QTRLE,
    HI_CODEC_ID_VIDEO_SNOW,
    HI_CODEC_ID_VIDEO_TSCC,
    HI_CODEC_ID_VIDEO_ULTI,
    HI_CODEC_ID_VIDEO_QDRAW,
    HI_CODEC_ID_VIDEO_VIXL,
    HI_CODEC_ID_VIDEO_QPEG,
    HI_CODEC_ID_VIDEO_PPM,
    HI_CODEC_ID_VIDEO_PBM,
    HI_CODEC_ID_VIDEO_PGM,
    HI_CODEC_ID_VIDEO_PGMYUV,
    HI_CODEC_ID_VIDEO_PAM,
    HI_CODEC_ID_VIDEO_FFVHUFF,
    
    HI_CODEC_ID_VIDEO_LOCO,
    HI_CODEC_ID_VIDEO_WNV1,
    HI_CODEC_ID_VIDEO_AASC,
    HI_CODEC_ID_VIDEO_FRAPS,
    HI_CODEC_ID_VIDEO_TRUEMOTION2,
    HI_CODEC_ID_VIDEO_BMP,
    HI_CODEC_ID_VIDEO_CSCD,
    HI_CODEC_ID_VIDEO_MMVIDEO,
    HI_CODEC_ID_VIDEO_ZMBV,
    HI_CODEC_ID_VIDEO_SMACKVIDEO,
    HI_CODEC_ID_VIDEO_NUV,
    HI_CODEC_ID_VIDEO_KMVC,
    HI_CODEC_ID_VIDEO_FLASHSV,
    HI_CODEC_ID_VIDEO_CAVS,
    
    HI_CODEC_ID_VIDEO_VMNC,
    
    HI_CODEC_ID_VIDEO_TARGA,
    HI_CODEC_ID_VIDEO_DSICINVIDEO,
    HI_CODEC_ID_VIDEO_TIERTEXSEQVIDEO,
    
    HI_CODEC_ID_VIDEO_DXA,
    HI_CODEC_ID_VIDEO_DNXHD,
    HI_CODEC_ID_VIDEO_THP,
    HI_CODEC_ID_VIDEO_SGI,
    HI_CODEC_ID_VIDEO_C93,
    HI_CODEC_ID_VIDEO_BETHSOFTVID,
    HI_CODEC_ID_VIDEO_PTX,
    HI_CODEC_ID_VIDEO_TXD,
    HI_CODEC_ID_VIDEO_AMV,
    HI_CODEC_ID_VIDEO_VB,
    HI_CODEC_ID_VIDEO_PCX,
    HI_CODEC_ID_VIDEO_SUNRAST,
    HI_CODEC_ID_VIDEO_MIMIC,
    HI_CODEC_ID_VIDEO_RL2,
    HI_CODEC_ID_VIDEO_8SVX_EXP,
    HI_CODEC_ID_VIDEO_8SVX_FIB,
    HI_CODEC_ID_VIDEO_ESCAPE124,
    HI_CODEC_ID_VIDEO_DIRAC,
    HI_CODEC_ID_VIDEO_BFI,
    HI_CODEC_ID_VIDEO_CMV,
    HI_CODEC_ID_VIDEO_MOTIONPIXELS,
    HI_CODEC_ID_VIDEO_TGV,
    HI_CODEC_ID_VIDEO_TGQ,
    HI_CODEC_ID_VIDEO_TQI,
    HI_CODEC_ID_VIDEO_AURA,
    HI_CODEC_ID_VIDEO_AURA2,
    HI_CODEC_ID_VIDEO_V210X,
    HI_CODEC_ID_VIDEO_TMV,
    HI_CODEC_ID_VIDEO_V210,
    HI_CODEC_ID_VIDEO_DPX,
    HI_CODEC_ID_VIDEO_MAD,
    HI_CODEC_ID_VIDEO_FRWU,
    HI_CODEC_ID_VIDEO_FLASHSV2,
    HI_CODEC_ID_VIDEO_CDGRAPHICS,
    HI_CODEC_ID_VIDEO_R210,
    HI_CODEC_ID_VIDEO_ANM,
    HI_CODEC_ID_VIDEO_BINKVIDEO,
    HI_CODEC_ID_VIDEO_IFF_ILBM,
    HI_CODEC_ID_VIDEO_IFF_BYTERUN1,
    HI_CODEC_ID_VIDEO_KGV1,
    HI_CODEC_ID_VIDEO_YOP,
    HI_CODEC_ID_VIDEO_DV,
    HI_CODEC_ID_BUTT
}HI_CODEC_ID_E;

/*BIT0 of HI_CODEC_SUPPORT_S.u32Type: Driven type */
/**Need be driven by codec manager. Like FFMPEG. */ /**CNcomment:编解码器需要管理器驱动 */
#define HI_CODEC_CAP_DRIVENOUTSIDE      (0x00000000)
/**Self-driven, only need call Start()/Stop(). Like VFMW. */ /**CNcomment:编解码器有自驱动能力 */
#define HI_CODEC_CAP_DRIVENSELF         (0x00000001)

/*BIT1 of HI_CODEC_SUPPORT_S.u32Type: Output type */
/**Codec outputs frame or stream to specified address directly.  */ /**CNcomment:编解码器支持将帧数据或流数据输出到指定地址 */
#define HI_CODEC_CAP_OUTPUT2SELFADDR    (0x00000000)
/**Codec outputs frame or stream to the address self allocated . */ /**CNcomment:编解码器将帧数据或流数据输出到自身地址，需要外部拷贝 */
#define HI_CODEC_CAP_OUTPUT2SPECADDR    (0x00000002)

/**Defines codec type: encoder or decoder  */
/**CNcomment:Codec类型:编码或解码*/ 
typedef enum hiCODEC_TYPE_E
{
    HI_CODEC_TYPE_DEC = 0x01,       /**<decoder, value is b00000001*/ /**< CNcomment:解码器 */
    HI_CODEC_TYPE_ENC = 0x02,       /**<encoder, value is b00000010*/ /**< CNcomment:编码器 */
    
    HI_CODEC_TYPE_BUTT
}HI_CODEC_TYPE_E;

/**Defines codec types and formats */
/**CNcomment:Codec类型与支持的协议*/ 
typedef struct hiCODEC_SUPPORT_S
{
    HI_U32          u32Type;            /**< Codec type, OR value of HI_CODEC_TYPE_E */ /**< CNcomment:Codec类型，HI_CODEC_TYPE_E的或值 */
    HI_CODEC_ID_E   enID;               /**< Codec ID*/ /**< CNcomment:Codec支持的协议类型 */
    struct hiCODEC_SUPPORT_S* pstNext;  /**< Pointer to next node*/ /**< CNcomment:指向下一个能力结点的指针 */
}HI_CODEC_SUPPORT_S;

/**Defines codec capability */
/**CNcomment:Codec能力*/ 
typedef struct hiCODEC_CAP_S
{
    HI_U32              u32CapNumber;   /**< Codec capability index, OR value of several HI_CODEC_CAP_XXX */ /**< CNcomment:Codec能力指示值，HI_CODEC_CAP_XXX的或值 */
    HI_CODEC_SUPPORT_S* pstSupport;     /**< Pointer to the support type and formats*/ /**< CNcomment:Codec能力结构体指针 */
}HI_CODEC_CAP_S;

/**Defines the codec version*/
/**CNcomment:版本定义*/ 
typedef union hiCODEC_VERSION_U
{
    struct
    {
        HI_U8 u8VersionMajor;           /**< Major version */ /**< CNcomment:主版本号 */
        HI_U8 u8VersionMinor;           /**< Minor version */ /**< CNcomment:次版本号 */
        HI_U8 u8Revision;               /**< Revision version */ /**< CNcomment:修订版本号 */
        HI_U8 u8Step;                   /**< Step version */ /**< CNcomment:步进版本号 */
    } stVersion;
    HI_U32 u32Version;
} HI_CODEC_VERSION_U;

/**Defines video decoder open parameter*/
/**CNcomment:视频解码器打开参数*/ 
typedef struct hiCODEC_VDEC_OPENPARAM_S
{
    HI_U32 u32Reserve;
    HI_VOID* pPlatformPriv;         /**< Special parameter for platform hardware codec */ /**< CNcomment:硬解码器私有参数 */
}HI_CODEC_VDEC_OPENPARAM_S;

/**Defines video encoder open parameter*/
/**CNcomment:视频编码器打开参数*/ 
typedef struct hiCODEC_VENC_OPENPARAM_S
{
    HI_U32 u32Reserve;
}HI_CODEC_VENC_OPENPARAM_S;

/**Defines the codec open parameters*/
/**CNcomment:实例创建参数*/
typedef struct hiCODEC_OPENPARAM_S
{
    HI_CODEC_TYPE_E enType;       /** Encode or Decode */ /**< CNcomment:创建编码器还是解码器 */
    HI_CODEC_ID_E enID;           /** Format */ /**< CNcomment:需要编码或解码的协议类型 */
    union {
        HI_CODEC_VDEC_OPENPARAM_S stVdec; /** Open parameters of video decoder */ /**< CNcomment:视频解码器创建参数 */
        HI_CODEC_VENC_OPENPARAM_S stVenc; /** Open parameters of video encoder */ /**< CNcomment:视频编码器创建参数 */
    }unParam;
} HI_CODEC_OPENPARAM_S;

/**Defines the codec max priority */
/**CNcomment:编解码器最大优先级定义*/
#define HI_CODEC_MAX_PRIORITY (16)

/**Defines attribute of video decoder */
/**CNcomment:视频解码器实例属性*/
typedef struct hiCODEC_VDEC_ATTR_S
{
    HI_VOID*                pCodecContext;  /**< Codec context */ /**< CNcomment:编解码器上下文，可以传递解码器特定参数 */
    HI_VOID*                pPlatformPriv;  /**< Only used by platform */ /**< CNcomment:硬解码器私有参数 */
} HI_CODEC_VDEC_ATTR_S;

/**Defines attribute of codec instance */
/**CNcomment:实例属性*/
typedef struct hiCODEC_ATTR_S
{
    HI_CODEC_ID_E           enID;   /**< Codec ID*/ /**< CNcomment:编码或解码的协议类型*/
    union {
        HI_CODEC_VDEC_ATTR_S stVdec;/**< Instance attribute*/ /**< CNcomment:视频解码器属性参数*/
    } unAttr;
} HI_CODEC_ATTR_S;

/**Defines video sub standard */
/**CNcomment: 视频协议分支标准 */
typedef enum hiCODEC_VIDEO_SUB_STANDARD_E
{
    HI_CODEC_VIDEO_SUB_STANDARD_UNKNOWN,       /**<Unknown*/ /**<CNcomment: 未知协议分支*/
    HI_CODEC_VIDEO_SUB_STANDARD_MPEG2_MPEG1,   /**<The MPEG2 protocol family is compatible with MPEG1.*/ /**<CNcomment: MPEG2协议族可以兼容MPEG1 */
    HI_CODEC_VIDEO_SUB_STANDARD_MPEG4_SHV,     /**<The MPEG4 protocol family is compatible with SHV.*/ /**<CNcomment: MPEG4协议族可以兼容SHV*/
    HI_CODEC_VIDEO_SUB_STANDARD_MPEG4_XVID,    /**<The MPEG4 protocol family includes the sub protocol XVID.*/ /**<CNcomment: MPEG4协议包含XVID分支*/
    HI_CODEC_VIDEO_SUB_STANDARD_MPEG4_DIVX,    /**<The MPEG4 protocol family includes the sub protocol DIVX.*/ /**<CNcomment: MPEG4协议包含DIVX分支*/

    HI_CODEC_VIDEO_SUB_STANDARD_BUTT
}HI_CODEC_VIDEO_SUB_STANDARD_E;

/**Defines the video norm*/
/**CNcomment: 定义视频制式枚举*/
typedef enum hiCODEC_ENC_FMT_E
{
    HI_CODEC_ENC_FMT_1080P_60 = 0,     /**<1080p 60 Hz*/ /**< CNcomment:1080p 60赫兹*/
    HI_CODEC_ENC_FMT_1080P_50,         /**<1080p 50 Hz*/ /**< CNcomment:1080p 50赫兹*/
    HI_CODEC_ENC_FMT_1080P_30,         /**<1080p 30 Hz*/ /**< CNcomment:1080p 30赫兹*/
    HI_CODEC_ENC_FMT_1080P_25,         /**<1080p 25 Hz*/ /**< CNcomment:1080p 25赫兹*/
    HI_CODEC_ENC_FMT_1080P_24,         /**<1080p 24 Hz*/ /**< CNcomment:1080p 24赫兹*/

    HI_CODEC_ENC_FMT_1080i_60,         /**<1080i 60 Hz*/ /**< CNcomment:1080i 60赫兹*/
    HI_CODEC_ENC_FMT_1080i_50,         /**<1080i 50 Hz*/ /**< CNcomment:1080i 50赫兹*/

    HI_CODEC_ENC_FMT_720P_60,          /**<720p 60 Hz*/  /**< CNcomment:720p 60赫兹*/
    HI_CODEC_ENC_FMT_720P_50,          /**<720p 50 Hz */ /**< CNcomment:720p 50赫兹*/

    HI_CODEC_ENC_FMT_576P_50,          /**<576p 50 Hz*/  /**< CNcomment:576p 50赫兹*/
    HI_CODEC_ENC_FMT_480P_60,          /**<480p 60 Hz*/  /**< CNcomment:480p 60赫兹*/

    HI_CODEC_ENC_FMT_PAL,              /**<B D G H I PAL */ /**< CNcomment:B D G H I PAL制式*/
    HI_CODEC_ENC_FMT_PAL_N,            /**<(N)PAL        */ /**< CNcomment:(N)PAL制式*/
    HI_CODEC_ENC_FMT_PAL_Nc,           /**<(Nc)PAL       */ /**< CNcomment:(Nc)PAL制式*/

    HI_CODEC_ENC_FMT_NTSC,             /**<(M)NTSC       */ /**< CNcomment:(M)NTSC制式*/
    HI_CODEC_ENC_FMT_NTSC_J,           /**<NTSC-J        */ /**< CNcomment:NTSC-J制式*/
    HI_CODEC_ENC_FMT_NTSC_PAL_M,       /**<(M)PAL        */ /**< CNcomment:(M)PAL制式*/

    HI_CODEC_ENC_FMT_SECAM_SIN,        /**< SECAM_SIN*/ /**< CNcomment:SECAM_SIN制式*/
    HI_CODEC_ENC_FMT_SECAM_COS,        /**< SECAM_COS*/ /**< CNcomment:SECAM_COS制式*/

    HI_CODEC_ENC_FMT_1080P_24_FRAME_PACKING,
    HI_CODEC_ENC_FMT_720P_60_FRAME_PACKING,
    HI_CODEC_ENC_FMT_720P_50_FRAME_PACKING,
    
    HI_CODEC_ENC_FMT_861D_640X480_60,
    HI_CODEC_ENC_FMT_VESA_800X600_60,
    HI_CODEC_ENC_FMT_VESA_1024X768_60,
    HI_CODEC_ENC_FMT_VESA_1280X720_60,
    HI_CODEC_ENC_FMT_VESA_1280X800_60,
    HI_CODEC_ENC_FMT_VESA_1280X1024_60,
    HI_CODEC_ENC_FMT_VESA_1360X768_60,
    HI_CODEC_ENC_FMT_VESA_1366X768_60,
    HI_CODEC_ENC_FMT_VESA_1400X1050_60,
    HI_CODEC_ENC_FMT_VESA_1440X900_60,
    HI_CODEC_ENC_FMT_VESA_1440X900_60_RB,
    HI_CODEC_ENC_FMT_VESA_1600X900_60_RB,
    HI_CODEC_ENC_FMT_VESA_1600X1200_60,
    HI_CODEC_ENC_FMT_VESA_1680X1050_60,
    HI_CODEC_ENC_FMT_VESA_1920X1080_60,
    HI_CODEC_ENC_FMT_VESA_1920X1200_60,
    HI_CODEC_ENC_FMT_VESA_2048X1152_60,

    HI_CODEC_ENC_FMT_BUTT
}HI_CODEC_ENC_FMT_E;

/**Defines the video YUV format*/
/**CNcomment: YUV格式枚举*/
typedef enum hiCODEC_COLOR_FORMAT_E
{
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_400 = 0,   /**<The YUV spatial sampling format is SEMIPLANAR 4:0:0.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:0:0*/
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_411,       /**<The YUV spatial sampling format is SEMIPLANAR 4:1:1.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:1:1*/
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_420,       /**<The YUV spatial sampling format is SEMIPLANAR 4:2:0.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:2:0*/
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_422_1X2,   /**<The YUV spatial sampling format is SEMIPLANAR 4:2:2,two Y correspond to one U and V at vertical direction.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:2:2，垂直方向两个亮度采样点共用一对色度采样点*/
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_422_2X1,   /**<The YUV spatial sampling format is SEMIPLANAR 4:2:2,two Y correspond to one U and V at horizontal direction.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:2:2，水平方向两个亮度采样点共用一对色度采样点*/
    HI_CODEC_COLOR_FORMAT_YUV_SEMIPLANAR_444,       /**<The YUV spatial sampling format is SEMIPLANAR 4:4:4.*/ /**<CNcomment: YUV空间采样格式为SEMIPLANAR 4:4:4*/

    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_400 = 0x10,    /**<The YUV spatial sampling format is PLANAR 4:0:0.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:0:0*/   
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_411,           /**<The YUV spatial sampling format is PLANAR 4:1:1.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:1:1*/   
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_420,           /**<The YUV spatial sampling format is PLANAR 4:2:0.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:2:0*/   
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_422_1X2,       /**<The YUV spatial sampling format is PLANAR 4:2:2,two Y correspond to one U and V at vertical direction.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:2:2，垂直方向两个亮度采样点共用一对色度采样点*/
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_422_2X1,       /**<The YUV spatial sampling format is PLANAR 4:2:2,two Y correspond to one U and V at horizontal direction.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:2:2，水平方向两个亮度采样点共用一对色度采样点*/
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_444,           /**<The YUV spatial sampling format is PLANAR 4:4:4.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:4:4*/   
    HI_CODEC_COLOR_FORMAT_YUV_PLANAR_410,           /**<The YUV spatial sampling format is PLANAR 4:1:0.*/ /**<CNcomment: YUV空间采样格式为PLANAR 4:1:0*/   
    
    HI_CODEC_COLOR_FORMAT_YUV_PACKAGE_UYVY422 = 0x20,   /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is UYVY.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为UYVY*/
    HI_CODEC_COLOR_FORMAT_YUV_PACKAGE_YUYV422,          /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YUYV.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为YUYV*/
    HI_CODEC_COLOR_FORMAT_YUV_PACKAGE_YVYU422,          /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YVYU.*/ /**<CNcomment: YUV空间采样格式为package,内存排列为YVYU*/
    HI_CODEC_COLOR_FORMAT_YUV_BUTT
}HI_CODEC_COLOR_FORMAT_E;

/**Defines the type of the video frame*/
/**CNcomment: 定义视频帧的类型枚举*/
typedef enum hiCODEC_VIDEO_FRAME_TYPE_E
{
    HI_CODEC_VIDEO_FRAME_TYPE_UNKNOWN,   /**<Unknown*/ /**<CNcomment: 未知的帧类型*/
    HI_CODEC_VIDEO_FRAME_TYPE_I,         /**<I frame*/ /**<CNcomment: I帧*/
    HI_CODEC_VIDEO_FRAME_TYPE_P,         /**<P frame*/ /**<CNcomment: P帧*/
    HI_CODEC_VIDEO_FRAME_TYPE_B,         /**<B frame*/ /**<CNcomment: B帧*/
    HI_CODEC_VIDEO_FRAME_TYPE_BUTT
}HI_CODEC_VIDEO_FRAME_TYPE_E;

/**Defines the video frame/field mode*/
/**CNcomment: 定义视频帧场模式枚举*/
typedef enum hiCODEC_VIDEO_FIELD_MODE_E
{
    HI_CODEC_VIDEO_FIELD_ALL,        /**<Frame mode*/ /**<CNcomment: 帧模式*/
    HI_CODEC_VIDEO_FIELD_TOP,        /**<Top field mode*/ /**<CNcomment: 顶场模式*/
    HI_CODEC_VIDEO_FIELD_BOTTOM,     /**<Bottom field mode*/ /**<CNcomment: 底场模式*/
    HI_CODEC_VIDEO_FIELD_BUTT
}HI_CODEC_VIDEO_FIELD_MODE_E;

/**Defines 3D frame packing type*/
/**CNcomment: 3D帧打包方式 */
typedef enum hiCODEC_VIDEO_FRAME_PACKING_TYPE_E
{
    HI_CODEC_VIDEO_FRAME_PACKING_NONE,              /**< normal frame, not a 3D frame */ /**<CNcomment: 非3D帧*/
    HI_CODEC_VIDEO_FRAME_PACKING_SIDE_BY_SIDE,      /**< side by side */ /**<CNcomment:左右模式3D帧 */
    HI_CODEC_VIDEO_FRAME_PACKING_TOP_AND_BOTTOM,    /**< top and bottom */ /**<CNcomment:上下模式3D帧 */
    HI_CODEC_VIDEO_FRAME_PACKING_TIME_INTERLACED,   /**< time interlaced: one frame for left eye, the next frame for right eye */ /**<CNcomment:时间差模式3D帧 */
    HI_CODEC_VIDEO_FRAME_PACKING_BUTT             
}HI_CODEC_VIDEO_FRAME_PACKING_TYPE_E;

/**Defines address information*/
/**CNcomment: 地址描述结构体*/
typedef struct hiCODEC_ADDRESS_S
{
    HI_U8   *pu8Vir;             /**<Virtual address of (frame or stream) buffer */ /**<CNcomment: 帧或流数据的虚拟地址 */
    HI_U32  u32Phy;             /**<Physical address of (frame or stream) buffer */ /**<CNcomment: 帧或流数据的物理地址 */
    HI_U32  u32Size;            /**<Size of (frame or stream) buffer*/ /**<CNcomment: 帧或流数据的大小 */
}HI_CODEC_ADDRESS_S;

/**Defines video stream info */
/** CNcomment: 视频流信息结构体*/
typedef struct hiCODEC_VIDEO_STREAMINFO_S
{
    HI_CODEC_ID_E                   enCodecID;      /**<Stream type*/ /**<CNcomment: 码流类型 */
    HI_CODEC_VIDEO_SUB_STANDARD_E   enSubStandard;  /**<Sub stream protocol*/ /**<CNcomment: 码流的协议分支 */
    HI_U32                          u32SubVersion;  /**<Version of the sub stream protocol*/ /**<CNcomment: 码流子协议版本号 */
    HI_U32                          u32Profile;     /**<Stream profile*/ /**<CNcomment: 码流的profile */
    HI_U32                          u32Level;       /**<Stream level*/ /**<CNcomment: 码流的level */
    HI_CODEC_ENC_FMT_E              enDisplayNorm;  /**<Display norm*/ /**<CNcomment: 显示标准 */
    HI_BOOL                         bProgressive;   /**<Sampling type (progressive or interlaced)*/ /**<CNcomment: 采样方式(逐行/隔行) */

    HI_U32                          u32AspectWidth; /**<Aspect width*/ /**<CNcomment: 最佳显示幅型比之宽度比值*/
    HI_U32                          u32AspectHeight;/**<Aspect height*/ /**<CNcomment: 最佳显示幅型比之高度比值*/

    HI_U32 u32bps;            /**<Bit rate, in kbit/s*/ /**<CNcomment: 码流的码率, Kbps */
    HI_U32 u32FrameRateInt;   /**<Integral part of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的整数部分, fps */
    HI_U32 u32FrameRateDec;   /**<Fractional part (calculated to three decimal places) of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的小数部分（保留3位）, fps */
    HI_U32 u32Width;          /**<Width of the decoded picture*/ /**<CNcomment: 解码图像宽 */
    HI_U32 u32Height;         /**<Height of the decoded picture*/ /**<CNcomment: 解码图像高 */
    HI_U32 u32DisplayWidth;   /**<Width of the displayed picture*/ /**<CNcomment: 显示图像宽 */
    HI_U32 u32DisplayHeight;  /**<Height of the displayed picture*/ /**<CNcomment: 显示图像高 */
    HI_U32 u32DisplayCenterX; /**<Horizontal coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/
                              /**<CNcomment: 显示图像中心横坐标，以原始图像的左上角为坐标原点 */
    HI_U32 u32DisplayCenterY; /**<Vertical coordinate of the center of the displayed picture (the upper left point of the source picture serves as the coordinate origin)*/
                              /**<CNcomment: 显示图像中心纵坐标，以原始图像的左上角为坐标原点 */
}HI_CODEC_VIDEO_STREAMINFO_S;

/**Defines stream information */
/**CNcomment: 码流信息结构体*/
typedef union hiCODEC_STREAMINFO_S
{
    HI_CODEC_VIDEO_STREAMINFO_S stVideo;    /**<video stream information*/ /**<CNcomment: 视频流信息*/
}HI_CODEC_STREAMINFO_S;

typedef struct hiCODEC_FRAME_BUF_S
{
	HI_U32 u32PhyAddr;
	HI_U32 u32Size;
}HI_CODEC_FRAME_BUF_S;

/**Defines stream descriptor */
/**CNcomment: 码流数据结构体*/
typedef struct hiCODEC_STREAM_S
{
    HI_U8* pu8Addr;     /**<Stream buffer address*/ /**<CNcomment: 流数据地址*/
	HI_U32 u32PhyAddr;  /**<Stream buffer address*/ /**<CNcomment: 流数据物理地址*/
    HI_U32 u32Size;     /**<Stream buffer size*/ /**<CNcomment: 流数据大小*/
    HI_S64 s64PtsMs;    /**<PTS(ms)*/ /**<CNcomment: 流数据对应PTS，毫秒为单位*/
}HI_CODEC_STREAM_S;

/**Defines video codec comonds */
/**CNcomment: 视频编解码器控制命令结构体*/
typedef struct hiCODEC_VIDEO_CMD_S
{
    HI_U32      u32CmdID;   /**<Commond ID*/ /**<CNcomment: 命令ID*/
    HI_VOID     *pPara;     /**<Control parameter*/ /**<CNcomment: 命令携带参数*/
}HI_CODEC_VIDEO_CMD_S;

/**Defines video frame information*/
/**CNcomment: 视频帧信息结构体*/
typedef struct hiCODEC_VIDEO_FRAME_S
{
    HI_CODEC_COLOR_FORMAT_E             enColorFormat;        /**<Video format*/ /**<CNcomment: 视频格式*/
    HI_CODEC_VIDEO_FRAME_TYPE_E         enFrameType;          /**<Frame type*/ /**<CNcomment: 帧类型*/
    HI_BOOL                             bProgressive;         /**<Sampling type*/ /**<CNcomment: 采样类型*/
    
    HI_CODEC_VIDEO_FIELD_MODE_E         enFieldMode;          /**<Frame or field encoding mode*/ /**<CNcomment: 帧或场编码模式*/
    HI_BOOL                             bTopFieldFirst;       /**<Top field first flag*/ /**<CNcomment: 顶场优先标志*/

    HI_CODEC_VIDEO_FRAME_PACKING_TYPE_E enFramePackingType;   /**<3D frame packing type*/ /**<CNcomment: 3D帧打包方式*/
    
    HI_U32                              u32FrameRate;         /**<Frame rate*//**<CNcomment: 帧率*/
    
    HI_U32                              u32Width;             /**<Width of the source picture*/ /**<CNcomment: 原始图像宽*/
    HI_U32                              u32Height;            /**<Height of the source picture*/ /**<CNcomment: 原始图像高*/
    HI_U32                              u32AspectWidth;       /**<Aspect width*/ /**<CNcomment: 最佳显示宽度*/
    HI_U32                              u32AspectHeight;      /**<Aspect height*/ /**<CNcomment: 最佳显示高度*/

    HI_U32                              u32YAddr;             /**<Address of the Y component in the current frame*/ /**<CNcomment: 当前帧Y分量数据的地址*/
    HI_U32                              u32UAddr;             /**<Address of the U component in the current frame*/ /**<CNcomment: 当前帧U分量数据的地址*/
    HI_U32                              u32VAddr;             /**<Address of the V component in the current frame*/ /**<CNcomment: 当前帧V分量数据的地址*/
    HI_U32                              u32YStride;           /**<Stride of the Y component*/ /**<CNcomment: Y分量数据的跨幅*/
    HI_U32                              u32UStride;           /**<Stride of the U component*/ /**<CNcomment: C分量数据的跨幅*/
    HI_U32                              u32VStride;           /**<Stride of the V component*/ /**<CNcomment: C分量数据的跨幅*/

    HI_U8*                              pu8UserData;          /**<User data *//**<CNcomment: 用户数据*/
    HI_U32                              u32UserDataSize;      /**<User data size *//**<CNcomment: 用户数据长度*/
}HI_CODEC_VIDEO_FRAME_S;

/**Defines frame descriptor */
/**CNcomment: 帧描述结构体*/
typedef struct hiCODEC_FRAME_S
{
    HI_CODEC_ADDRESS_S          stOutputAddr;   /**< Always [in], only used by the codecs which support HI_CODEC_OUTPUT_TOSPECADDR, Only used by decoder  */
                                                /**< CNcomment: 帧输出地址，一直是输入参数，仅适用于支持HI_CODEC_OUTPUT_TOSPECADDR的解码器 */
    HI_S64                      s64SrcPtsMs;    /**< Decoder [out], encoder [in], The source pts(ms) */
                                                /**< CNcomment: 帧源PTS，毫秒为单位，对解码器来说是输出参数，对编码器来说是输入参数*/
    HI_S64                      s64PtsMs;       /**< Decoder [out], encoder [in], The pts(ms)*/
                                                /**< CNcomment: 帧PTS，毫秒为单位，对解码器来说是输出参数，对编码器来说是输入参数*/
    union{
        HI_CODEC_VIDEO_FRAME_S  stVideo;        /**< Decoder [out], encoder [in], The video frame information*/ 
                                                /**< CNcomment: 视频帧信息，对解码器来说是输出参数，对编码器来说是输入参数*/
    }unInfo;
}HI_CODEC_FRAME_S;

/**Defines codec*/
/**CNcomment: CODEC结构体定义*/
typedef struct hiCODEC_S
{
    /**<Description information about a codec */
    /**<CNcomment: 编解码器描述名字 */ 
    const HI_CHAR *pszName;

    /**<Codec version */
    /**<CNcomment: 编解码器版本描述 */ 
    const HI_CODEC_VERSION_U unVersion;

    /**<Detailed information about a codec */
    /**<CNcomment: 编解码器详细描述信息 */
    const HI_CHAR *pszDescription;

    /**
    \brief Get codec capability. CNcomment:获取编解码器能力 CNend
    \attention \n
    N/A
    \param [out] pstCodecCap, Pointer of the codec capability. CNcomment:指针类型，指向编解码器能力 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */
    HI_S32 (*GetCap)(HI_CODEC_CAP_S *pstCodecCap);
  
    /**
    \brief Create a codec instance. CNcomment:创建编解码器实例 CNend
    \attention \n
    N/A
    \param [in] pstParam, Pointer of the open params. CNcomment:指针类型，指向实例打开参数 CNend
    \param [out] phInst, Pointer to instance handle. CNcomment:指针类型，指向实例句柄 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */
    HI_S32 (*Create)(HI_HANDLE* phInst, const HI_CODEC_OPENPARAM_S * pstParam);

    /**
    \brief Destroy a codec instance. CNcomment:销毁编解码器实例 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */     
    HI_S32 (*Destroy)(HI_HANDLE hInst);

    /**
    \brief Start a codec instance. CNcomment:启动编解码器实例 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */       
    HI_S32 (*Start)(HI_HANDLE hInst);

    /**
    \brief Stop a codec instance. CNcomment:停止编解码器实例 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */       
    HI_S32 (*Stop)(HI_HANDLE hInst);

    /**
    \brief Reset a codec instance. CNcomment:复位编解码器实例 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */        
    HI_S32 (*Reset)(HI_HANDLE hInst);

    /**
    \brief Set attribute to a codec instance. CNcomment:设置编解码器属性 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [in] pstAttr, Pointer to the instance attribute. CNcomment:指针类型，指向实例属性 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */      
    HI_S32 (*SetAttr)(HI_HANDLE hInst, const HI_CODEC_ATTR_S * pstAttr);

    /**
    \brief Get attribute to a codec instance. CNcomment:获取编解码器属性 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [out] pstAttr, Pointer to the instance attribute. CNcomment:指针类型，指向实例属性 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */        
    HI_S32 (*GetAttr)(HI_HANDLE hInst, HI_CODEC_ATTR_S * pstAttr);

    /**
    \brief Decode a frame.. CNcomment:解码一帧 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [in] pstIn, Pointer to stream data descriptor. CNcomment:指针类型，指向码流数据信息结构体 CNend
    \param [out] pstOut, Pointer to frame data descriptor. CNcomment:指针类型，指向帧数据信息结构体 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */     
    HI_S32 (*DecodeFrame)(HI_HANDLE hInst, HI_CODEC_STREAM_S * pstIn, HI_CODEC_FRAME_S * pstOut);

    /**
    \brief Encode a frame. CNcomment:编码一帧 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [in] pstIn, Pointer to stream data descriptor. CNcomment:指针类型，指向码流数据信息结构体 CNend
    \param [out] pstOut, Pointer to frame data descriptor. CNcomment:指针类型，指向帧数据信息结构体 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */     
    HI_S32 (*EncodeFrame)(HI_HANDLE hInst, HI_CODEC_FRAME_S * pstIn, HI_CODEC_STREAM_S * pstOut);

    /**
    \brief Get stream information. CNcomment:获取码流信息 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [out] pstAttr, Pointer to stream information. CNcomment:指针类型，指向码流信息结构体 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */     
    HI_S32 (*GetStreamInfo)(HI_HANDLE hInst, HI_CODEC_STREAMINFO_S * pstAttr);

	HI_S32 (*RegFrameBuffer)(HI_HANDLE hInst, HI_CODEC_STREAM_S *pstRawPacket);
    /**
    \brief Other control, can be extended. CNcomment:其他控制选项，可扩展 CNend
    \attention \n
    N/A
    \param [in] hInst, Instance handle. CNcomment:实例句柄 CNend
    \param [out] u32CMD, The commond ID. CNcomment:命令ID CNend
    \param [out] pParam, Pointer to control parameter. CNcomment:指针类型，指向控制参数 CNend
    \retval ::HI_SUCCESS CNcomment:操作成功 CNend
    \retval ::HI_FAILURE CNcomment:操作失败 CNend
    \see \n
    N/A
    */     
    HI_S32 (*Control)(HI_HANDLE hInst, HI_U32 u32CMD, HI_VOID * pParam);
}HI_CODEC_S;

/** @} */  /** <!-- ==== Structure Definition End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_VIDEO_CODEC_H__ */
