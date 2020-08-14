#ifndef __IMEDIA_COMMON_H__
#define __IMEDIA_COMMON_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#include "basetype.h"

#if defined(_MSC_VER)
#if defined(EXPORT_LIB)
#define EXPORT_API
#elif defined(EXPORT_DLL)
#define EXPORT_API extern __declspec(dllexport)
#else
#define EXPORT_API extern __declspec(dllimport)
#endif

#include <string.h>
#include <memory.h>
#include <windows.h>

//#pragma  warning(disable:4786)
#pragma  warning(disable:4996)

#ifndef inline
#define inline __inline
#endif

#define PRE64 "I64"

#elif defined(__GNUC__)
#define EXPORT_API extern

#define stricmp  strcasecmp
#define strnicmp strncasecmp

#define PRE64 "ll"
#else
#define EXPORT_API
#define PRE64
#endif

#ifndef IMEDIA_DECLARE_ALIGNED
#ifdef _MSC_VER
#define IMEDIA_DECLARE_ALIGNED(var, n) __declspec(align(n)) var
#elif defined(__GNUC__)
#define IMEDIA_DECLARE_ALIGNED(var, n) var __attribute__((aligned(n)))
#endif

#define IMEDIA_DECLARE_ALIGNED_16(var) IMEDIA_DECLARE_ALIGNED(var, 16)
#define IMEDIA_DECLARE_ALIGNED_8(var)  IMEDIA_DECLARE_ALIGNED(var, 8)
#define IMEDIA_DECLARE_ALIGNED_4(var)  IMEDIA_DECLARE_ALIGNED(var, 4)
#endif

#pragma pack(push, 1)

/****************************************************************************************
 * 公共部分数据结构
 ****************************************************************************************/
/** 各模块公共常用标记，第1个字节，可表示8种状态 */
#define IMEDIA_FLAG_DISABLE_ASM         0x00000001 /**< 汇编优化关闭标记（调试时使用） .D.E.M */
#define IMEDIA_FLAG_CALC_FPS            0x00000002 /**< 通道性能统计使能标记，开启后会自动统计性能数据FPS（调试时使用） .D.E */

/** 解码模块常用标记，第2个字节，可表示8种状态 */
#define IMEDIA_FLAG_DEBLOCK_FILTER      0x00000100 /**< MPEG2等解码器后处理Deblock滤波使能标记，H264解码无需此功能，保留字段 .D */
#define IMEDIA_FLAG_ERROR_RESILIENCE    0x00000200 /**< 解码器错误隐藏使能标记，开启该功能后性能有一定的下降，保留字段 .D */

/** 编码模块常用标记，第3个字节，可表示8种状态 */
#define IMEDIA_FLAG_CALC_PSNR           0x00010000 /**< 编码器客观质量PSNR统计标记，该功能对性能影响较大，仅在质量测试时才可开启（调试时使用） .E */

/** 控制命令枚举类型定义 */
typedef enum ENUM_IMEDIA_CMD
{
    IMEDIA_GET_VERSION = 0,        /**< 获取版本信息 .D.E.M */
    IMEDIA_GET_STATUS,             /**< 获取通道状态信息 .D.E */
    IMEDIA_GET_STREAM_INFO,        /**< 获取码流信息 .D.E */
    IMEDIA_GET_PARAMS,             /**< 获取参数信息 .D.E */

    /**< 以下命令只在编码模块有效 */
    IMEDIA_GET_DYN_PARAMS,         /**< 获取动态参数 .E */
    IMEDIA_GET_SPS_PPS,            /**< 获取序列头图像头NALU码流 .E */
    IMEDIA_SET_DYN_PARAMS,         /**< 设置动态参数 .E */
    IMEDIA_SET_FUNCTION,           /**< 设置回调函数，保留字段，一期没实现 */

    /**< 以下命令只在解码模块有效 */
    IMEDIA_SET_DEBUG_FLAG,         /**< 设置调试信息标记 .D */

    IMEDIA_FLUSH,                  /**< 清空算法通道的cache信息（帧YUV数据） .D */
    IMEDIA_RESET,                  /**< 复位算法通道 .D */

    IMEDIA_PICTURE_RELEASE,        /**< 释放解码模块输出的YUV帧控制权 */
} ENUM_IMEDIA_CMD;

/** 调试级别枚举类型定义 */
typedef enum ENUM_IMEDIA_MSG_LEVEL
{
    IMEDIA_ERROR = 0,
    IMEDIA_WARNING,
    IMEDIA_INFO,
    IMEDIA_DEBUG,
} ENUM_IMEDIA_MSG_LEVEL;

typedef void *IMEDIA_CODEC_CTX;
typedef void *IMEDIA_IMAGE_CTX;

#define IMEDIA_VERSION_LENGTH  48    /* 版本号字符串长度 */
#define IMEDIA_TIME_LENGTH     28    /* 发布时间字符串长度 */

/*!版本结构*/
typedef struct STRU_IMEDIA_VERSION
{
    char cVersionChar[IMEDIA_VERSION_LENGTH];  /**< 版本号字符串表示 */
    char cReleaseTime[IMEDIA_TIME_LENGTH];     /**< 版本发布时间字符串表示 */
    UINT32 uiCompileVersion;                   /**< 编译器版本，4个字符组成 */
} STRU_IMEDIA_VERSION;

typedef void *funMalloc(unsigned int len);
typedef void  funFree(void *ptr);
typedef void  funMsgOutput(int level, const char *msg); /**< level定义见ENUM_IMEDIA_MSG_LEVEL */

/*!系统信息结构 */
typedef struct STRU_IMEDIA_SYSINFO
{
    funMsgOutput  *pfnPrintf;       /**< 日志消息输出回调函数，信息空间长度建议4096 Bytes */
    funMalloc     *pfnMalloc;       /**< 内存分配回调函数 */
    funFree       *pfnFree;         /**< 内存释放回调函数 */
} STRU_IMEDIA_SYSINFO;

#define MAX_BUF_NUM     64  /**< just a example ,will be set to the max buf num with codec's need */

/*!单缓存结构类型 */
typedef struct STRU_IMEDIA_SINGLE_BUFFER
{
    UINT8  *pucBuf;        /**< 缓存基地址 */
    UINT32  uiSize;        /**< 缓存大小 */
} STRU_IMEDIA_SINGLE_BUFFER;

typedef struct STRU_IMEDIA_MEMBUF
{
    UINT32  uiSize;        /**< 内存大小 */
    UINT32  uiAlignment;   /**< 内存对齐大小 */
    UINT8   *pucbase;      /**< 内存地址 */
} STRU_IMEDIA_MEMBUF;

typedef struct STRU_IMEDIA_RESTAB
{
    UINT32 uiNumBuf;
    STRU_IMEDIA_MEMBUF stMemTab[MAX_BUF_NUM];
} STRU_IMEDIA_RESTAB;

/*****************************************************************************************
 * 视频部分公共数据结构
 *****************************************************************************************/
#define IVIDEO_PIXEL_ALIGN_VALUE 16

/** 图像宽高对齐值，宽必须为16的倍数，高必须为2的倍数 */
#define IVIDEO_WIDTH_STRIDE(w)  ((((w)+15)>>4)<<4)
#define IVIDEO_HEIGHT_STRIDE(h) ((((h)+1)>>1)<<1)

/** 图像YUV大小计算 */
#define IVIDEO_YUV_SIZE(w, h)  ((IVIDEO_WIDTH_STRIDE(w) * IVIDEO_HEIGHT_STRIDE(h) * 3) >> 1)
#define IVIDEO_Y_SIZE(w, h)     (IVIDEO_WIDTH_STRIDE(w) * IVIDEO_HEIGHT_STRIDE(h))
#define IVIDEO_UV_SIZE(w, h)    (IVIDEO_Y_SIZE(w, h) >> 2)

#define IVIDEO_MIN_WIDTH  32
#define IVIDEO_MAX_WIDTH  1920
#define IVIDEO_MAX_PITCH  131072

#define IVIDEO_MIN_HEIGHT 16
#define IVIDEO_MAX_HEIGHT 1088

/** 视频算法通道工作状态 */
typedef enum ENUM_IVIDEO_CODEC_STATUS
{
    IVIDEO_CODEC_INVALID = 0,   /**< 无效状态 */
    IVIDEO_CODEC_READY,         /**< 准备状态，缓存已准备，但相关参数还未设置 */
    IVIDEO_CODEC_RUNNING,       /**< 运行状态 */
    IVIDEO_CODEC_STOPPED,       /**< 停止状态 */
} ENUM_IVIDEO_CODEC_STATUS;

/*!视频算法类型 */
typedef enum ENUM_IVIDEO_CODEC_TYPE
{
    IVIDEO_CODEC_UNKNOWN = -1,
    IVIDEO_CODEC_H264 = 0,
    IVIDEO_CODEC_MPEG2,
    IVIDEO_CODEC_H263,
    IVIDEO_CODEC_MPEG4,
    IVIDEO_CODEC_WMV3,          /* VC1的simple profile和main profile */
    IVIDEO_CODEC_VC1,           /* VC1的advanced profile */
    IVIDEO_CODEC_VP6F,          /* On2 VP6 (Flash version), .flv/.f4v/.avi(strf.biCompression=VP6F)等容器使用 */
    IVIDEO_CODEC_VP6,           /* On2 VP6 (Flip version), .avi(strf.biCompression=VP60/VP61/VP62)等容器使用 */
    IVIDEO_CODEC_SORENSON,      /* 码流常在flv容器中*/
} ENUM_IVIDEO_CODEC_TYPE;

/*!视频算法profile类型 */
typedef enum ENUM_IVIDEO_CODEC_PROFILE
{
    IVIDEO_PROFILE_UNKNOWN = -1,

    /**< MPEG2 */
    IVIDEO_PROFILE_MPEG2_HIGH      = 1,
    IVIDEO_PROFILE_MPEG2_SPATIALLY = 2, //reserved, not support now
    IVIDEO_PROFILE_MPEG2_SNR       = 3, //reserved, not support now
    IVIDEO_PROFILE_MPEG2_MAIN      = 4,
    IVIDEO_PROFILE_MPEG2_SIMPLE    = 5,

    /**< H264 */
    IVIDEO_PROFILE_H264_BASELINE = 66,
    IVIDEO_PROFILE_H264_MAIN     = 77,
    IVIDEO_PROFILE_H264_EXTENTED = 88,  //reserved, not support now
    IVIDEO_PROFILE_H264_HIGH     = 100,
    IVIDEO_PROFILE_H264_HIGH10   = 110, //reserved, not support now
    IVIDEO_PROFILE_H264_HIGH422  = 122, //reserved, not support now
    IVIDEO_PROFILE_H264_HIGH444  = 144, //reserved, not support now

    /**< H263 */
    IVIDEO_PROFILE_H263          = 6,
    IVIDEO_PROFILE_H263_PLUS     = 7,

    /**< MPEG4 */
    IVIDEO_PROFILE_MPEG4_SP      = 0,
    IVIDEO_PROFILE_MPEG4_MP      = 3,
    IVIDEO_PROFILE_MPEG4_ASP     = 15,


    /**< WMV/VC-1 */
    IVIDEO_PROFILE_VC1_SP        = 0,
    IVIDEO_PROFILE_VC1_MP        = 4,
    IVIDEO_PROFILE_VC1_AP        = 3,

    /**< VP6 */

    /**< Sorenson Spark */
    IVIDEO_VERSION_0             = 0,
    IVIDEO_VERSION_1             = 1,
} ENUM_IVIDEO_CODEC_PROFILE;

/*!视频算法level类型 */
typedef enum ENUM_IVIDEO_CODEC_LEVEL
{
    IVIDEO_LEVEL_UNKNOWN = -1,
    IVIDEO_LEVEL_AUTO = IVIDEO_LEVEL_UNKNOWN,

    /**< MPEG2 */
    IVIDEO_LEVEL_MPEG2_HIGH      = 4,
    IVIDEO_LEVEL_MPEG2_HIGH_1440 = 6,
    IVIDEO_LEVEL_MPEG2_MAIN      = 8,
    IVIDEO_LEVEL_MPEG2_LOW       = 10,

    /**< H264 */
    IVIDEO_LEVEL_H264_10 = 10,
    IVIDEO_LEVEL_H264_11 = 11,
    IVIDEO_LEVEL_H264_12 = 12,
    IVIDEO_LEVEL_H264_13 = 13,
    IVIDEO_LEVEL_H264_20 = 20,
    IVIDEO_LEVEL_H264_21 = 21,
    IVIDEO_LEVEL_H264_22 = 22,
    IVIDEO_LEVEL_H264_30 = 30,
    IVIDEO_LEVEL_H264_31 = 31,
    IVIDEO_LEVEL_H264_32 = 32,
    IVIDEO_LEVEL_H264_40 = 40,
    IVIDEO_LEVEL_H264_41 = 41,
    IVIDEO_LEVEL_H264_42 = 42,
    IVIDEO_LEVEL_H264_50 = 50,
    IVIDEO_LEVEL_H264_51 = 51,

    /**< H263 */
    IVIDEO_LEVEL_H263_10 = 10,
    IVIDEO_LEVEL_H263_20 = 20,
    IVIDEO_LEVEL_H263_30 = 30,
    IVIDEO_LEVEL_H263_40 = 40,
    IVIDEO_LEVEL_H263_50 = 50,
    IVIDEO_LEVEL_H263_60 = 60,
    IVIDEO_LEVEL_H263_70 = 70,

    /**< MPEG4 */
    IVIDEO_LEVEL_MPEG4_0  = 0x00,
    IVIDEO_LEVEL_MPEG4_1  = 0x01,
    IVIDEO_LEVEL_MPEG4_2  = 0x02,
    IVIDEO_LEVEL_MPEG4_3  = 0x03,
    IVIDEO_LEVEL_MPEG4_4  = 0x04,
    IVIDEO_LEVEL_MPEG4_5  = 0x05,
    IVIDEO_LEVEL_MPEG4_3b = 0x06,  /*only for MPEG-4 ASP*/

    /**< WMV3 */
    IVIDEO_LEVEL_WMV3_LOW  = 0,
    IVIDEO_LEVEL_WMV3_MAIN = 2,
    IVIDEO_LEVEL_WMV3_HIGH = 4,

    /**< VC-1 */
    IVIDEO_LEVEL_VC1_0 = 0,
    IVIDEO_LEVEL_VC1_1 = 1,
    IVIDEO_LEVEL_VC1_2 = 2,
    IVIDEO_LEVEL_VC1_3 = 3,
    IVIDEO_LEVEL_VC1_4 = 4,

    /**< VP6 */
} ENUM_IVIDEO_CODEC_LEVEL;

/*!视频内容格式 */
typedef enum ENUM_IVIDEO_CONTENT_TYPE
{
    IVIDEO_CONTENT_UNKNOW = -1,
    IVIDEO_PROGRESSIVE = 0,         /**< 帧，Progressive frame. */
    IVIDEO_INTERLACED,              /**< 场，Interlaced frame, 交织存储. */
    IVIDEO_AUTO,                    /**< 仅在编码模块使用 */
} ENUM_IVIDEO_CONTENT_TYPE;

/*!视频帧类型 */
typedef enum ENUM_IVIDEO_FRAME_TYPE
{
    IVIDEO_FRAME_UNKNOWN = -1,
    IVIDEO_FRAME_P,     /*包括MPEG-4中的S(GMC)类型*/
    IVIDEO_FRAME_B,
    IVIDEO_FRAME_I,
    IVIDEO_FRAME_SI,
    IVIDEO_FRAME_SP,
    IVIDEO_FRAME_IDR,
    IVIDEO_FRAME_BREF,
} ENUM_IVIDEO_FRAME_TYPE;

/*!视频速率编码类型 */
typedef enum ENUM_IVIDEO_FRAME_RATE_CODE
{
    IVIDEO_FRAME_RATE_UNKNOWN = 0,
    IVIDEO_FRAME_RATE_23_976,    /**< 24 000/1001 */
    IVIDEO_FRAME_RATE_24,
    IVIDEO_FRAME_RATE_25,
    IVIDEO_FRAME_RATE_29_97,     /**< 30 000/1001 */
    IVIDEO_FRAME_RATE_30,
    IVIDEO_FRAME_RATE_50,
    IVIDEO_FRAME_RATE_59_94,     /**< 60 000/1001 */
    IVIDEO_FRAME_RATE_60,
} ENUM_IVIDEO_FRAME_RATE_CODE;

/*!视频色度格式类型 */
typedef enum ENUM_IVIDEO_COLOR_SPACE_TYPE
{
    IVIDEO_CSP_UNKNOWN = -1,

    /**< YUV 420/411 - Planar Format */
    IVIDEO_CSP_IYUV = 0,
    IVIDEO_CSP_YV12,

    /**< YUV 422 - Packed Format */
    IVIDEO_CSP_UYVY, /**< Most popular of the various YUV 4:2:2 formats */
    IVIDEO_CSP_YUY2,
    IVIDEO_CSP_YVYU,

    /**< YUV 444 - Packed Format */
    IVIDEO_CSP_YUV444,

    /**< RGB Format */
    IVIDEO_CSP_RGB32,
    IVIDEO_CSP_RGB24,
    IVIDEO_CSP_RGB555,
    IVIDEO_CSP_RGB565,

    /**< Y only */
    IVIDEO_CSP_MONOCHROME,

    /**< None */
    IVIDEO_CSP_NONE,

    /**< Duplication Name */
    IVIDEO_CSP_YUV420 = IVIDEO_CSP_IYUV,
    IVIDEO_CSP_YUV422 = IVIDEO_CSP_UYVY,
    IVIDEO_CSP_YUYV   = IVIDEO_CSP_YUY2,
} ENUM_IVIDEO_COLOR_SPACE_TYPE;

/*!视频色度格式类型 */
typedef enum ENUM_IVIDEO_YUV_BUF_ORDER
{
    IVIDEO_Y = 0,
    IVIDEO_U,
    IVIDEO_V,
} ENUM_IVIDEO_YUV_BUF_ORDER;

/*!图像像素宽高比结构信息 */
typedef struct STRU_IVIDEO_ASPECT_RATIO
{
    UINT16  usSarWidth;         /**< 图像像素的样点宽，Range[0-65535] */
    UINT16  usSarHeight;        /**< 图像像素的样点高，Range[0-65535] */
} STRU_IVIDEO_ASPECT_RATIO;

/*!视频图像结构信息 */
typedef struct STRU_IVIDEO_PICTURE
{
    UINT8  *apucBuf[4];         /**< 指向当前图像数据的指针(0-Y 1-U 2-V 3-private)，中间模块：Y平面地址需16字节对齐，U、V平面地址需8字节对齐；其他模块无限制 */
    UINT16  usWidth;            /**< 图像宽度的实际值，单位像素，Range[32-1920] */
    UINT16  usHeight;           /**< 图像高度的实际值，单位像素，Range[16-1088] */
    UINT16  usWidthPitch;       /**< 图像宽度的内存跨度，单位像素，Range[>=usWidth] (中间模块中resize算法需为16的倍数，编码模块需为2的倍数且最大支持到3840)  */
    ENUM_IVIDEO_CONTENT_TYPE  eContentType;         /**< 图像的类型（IVIDEO_PROGRESSIVE or IVIDEO_INTERLACED) */
    STRU_IVIDEO_ASPECT_RATIO  stAspectRatio;        /**< 图像像素宽高比 */
    //ENUM_IVIDEO_COLOR_SPACE_TYPE eColorSpaceType; /**< reserved, 输出图像的格式，默认为YUV420格式 */
} STRU_IVIDEO_PICTURE;

/*!视频码流信息 */
typedef struct STRU_IVIDEO_STREAM_INFO
{
    UINT16 usWidth;                                /**< 图像宽度，单位象素，Range[32-1920] */
    UINT16 usHeight;                               /**< 图像高度，单位象素，Range[16-1088] */

    ENUM_IVIDEO_CODEC_TYPE       eCodecType;
    ENUM_IVIDEO_CODEC_PROFILE    eProfile;         /**< profile idc */
    ENUM_IVIDEO_CODEC_LEVEL      eLevel;           /**< level idc */
    ENUM_IVIDEO_CONTENT_TYPE     eContentType;     /**< 图像类型 (IVIDEO_PROGRESSIVE or IVIDEO_INTERLACED) */
    ENUM_IVIDEO_COLOR_SPACE_TYPE eColorSpaceType;  /**< 图像格式 (YUV420, YUV422, etc) */

    UINT32 uiRefFrameNum;                          /**< 参考帧个数，解码器 Range[0-16]，编码器 Range[1-16] */
    UINT32 uiFrameRate;                            /**< 码流帧率 x 1000，Range[5000-60000]，仅编码器有效 */
    UINT32 uiBitrate;                              /**< 码流比特率，单位kbps, Range[10-30000]，仅编码器有效 */
    UINT32 uiVbvBuf;                               /**< VBV缓存，单位ms，Range[500-10000]，仅编码器有效 */

    STRU_IVIDEO_ASPECT_RATIO stAspectRatio;        /**< 图像像素宽高比 */
} STRU_IVIDEO_STREAM_INFO;

/*****************************************************************************************
 * H264数据结构
 *****************************************************************************************/
/*!NALU包类型 */
typedef enum ENUM_H264_NALU_TYPE
{
    H264_NAL_UNKNOWN     = 0,
    H264_NAL_SLICE       = 1,
    H264_NAL_SLICE_DPA   = 2,
    H264_NAL_SLICE_DPB   = 3,
    H264_NAL_SLICE_DPC   = 4,
    H264_NAL_SLICE_IDR   = 5,          /* ref_idc != 0 */
    H264_NAL_SEI         = 6,          /* ref_idc == 0 */
    H264_NAL_SPS         = 7,
    H264_NAL_PPS         = 8,
    H264_NAL_AUD         = 9,
    /* ref_idc == 0 for 6,9,10,11,12 */
} ENUM_H264_NALU_TYPE;

/*!NALU包优先级 */
typedef enum ENUM_H264_NALU_PRIORITY
{
    H264_NAL_PRIORITY_DISPOSABLE = 0,  /* SEI,B-NoRef */
    H264_NAL_PRIORITY_LOW        = 1,
    H264_NAL_PRIORITY_HIGH       = 2,  /* I, P, B-ref */
    H264_NAL_PRIORITY_HIGHEST    = 3   /* SPS, PPS, IDR */
} ENUM_H264_NALU_PRIORITY;

/*!H264 NALU码流结构 */
typedef struct STRU_H264_NALU_STREAM
{
    UINT8  *pucData;                   /**< 地址 */
    UINT32  uiSize;                    /**< 大小 */
    ENUM_H264_NALU_TYPE     eType;     /**< 类型 */
    ENUM_H264_NALU_PRIORITY ePriority; /**< 优先级 */
} STRU_H264_NALU_STREAM;

#define MAX_NALU_COUNT_OF_FRAME 200

/*!H264 帧码流数据结构 */
typedef struct STRU_H264_FRAME_STREAM
{
    UINT8  *pucStream;                 /**< 帧数据缓存，与第一个NALU地址相同 */

    STRU_H264_NALU_STREAM stNalu[MAX_NALU_COUNT_OF_FRAME]; /**< NALU数据 */

    BOOL16  bSkipFlag;                 /**< 当前帧是否被丢弃 */
    UINT16  usNaluCount;               /**< NALU个数 */
    UINT32  uiDataSize;                /**< 帧数据大小，可直接从第一个NALU地址开始取该大小的数据 */
    UINT32  uiFrameID;                 /**< 帧编码顺序编号，可换算为DTS，每个segment从0开始计数 */
    UINT32  uiDisplayID;               /**< 帧显示顺序编号，可换算为PTS，每个segment从0开始计数 */
    ENUM_IVIDEO_FRAME_TYPE eFrameType; /**< 当前帧编码类型 */
} STRU_H264_FRAME_STREAM;

/*!比特流位置状态 */
typedef enum ENUM_BITSTREAM_STATUS
{
    BITSTREAM_UNKNOWN = 0,
    BITSTREAM_SEGMENT_BEGIN,           /**< 当前分片的第一批数据 */
    BITSTREAM_SEGMENT_END,             /**< 当前分片的最后一批数据 */
    BITSTREAM_OTHER,
} ENUM_BITSTREAM_STATUS;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __IMEDIA_COMMON_H__ */

/************************************************************************
* $Log$
************************************************************************/
