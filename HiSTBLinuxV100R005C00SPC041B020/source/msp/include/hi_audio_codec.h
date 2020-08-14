/******************************************************************************
Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_audio_codec.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009/11/01
Last Modified :
Description   : define audio common data structure
  History       :
  1.Date        : 2009/11/10
    Author      : zgjie
    Modification: Created file
******************************************************************************/

/**
 * \file
 * \brief Describes the information about the audio codec. CNcomment:提供AUDIO_CODEC的相关信息 CNend
 */

#ifndef __HI_AUDIO_CODEC_H__
#define __HI_AUDIO_CODEC_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**Define HA codec Print Level*/
/**CNcomment:定义HA codec打印级别*/
#define HA_TRACE_LEVEL_FATAL    (0)
#define HA_TRACE_LEVEL_ERROR    (1)
#define HA_TRACE_LEVEL_WARN     (2)
#define HA_TRACE_LEVEL_INFO     (3)
#define HA_TRACE_LEVEL_DBG      (4)


#ifndef HI_ADVCA_FUNCTION_RELEASE

#ifndef CFG_HA_LOG_LEVEL
#define CFG_HA_LOG_LEVEL         (HA_TRACE_LEVEL_ERROR)
#endif

#if (CFG_HA_LOG_LEVEL == HA_TRACE_LEVEL_FATAL)
#define HA_FATAL_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_ERR_PRINT(fmt...)
#define HA_WARN_PRINT(fmt...)
#define HA_INFO_PRIN(fmt...)
#define HA_DBG_PRINT(fmt...)

#elif (CFG_HA_LOG_LEVEL == HA_TRACE_LEVEL_ERROR)
#define HA_FATAL_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_ERR_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_WARN_PRIN(fmt...)
#define HA_INFO_PRIN(fmt...)
#define HA_DBG_PRINT(fmt...)

#elif(CFG_HA_LOG_LEVEL == HA_TRACE_LEVEL_WARN)
#define HA_FATAL_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_ERR_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_WARN_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_INFO_PRINT(fmt...)
#define HA_DBG_PRINT(fmt...)

#elif(CFG_HA_LOG_LEVEL == HA_TRACE_LEVEL_INFO)
#define HA_FATAL_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_ERR_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_WARN_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_INFO_PRINT(fmt...)    do{printf(fmt);}while(0)
#define HA_DBG_PRINT(fmt...)

#elif(CFG_HA_LOG_LEVEL == HA_TRACE_LEVEL_DBG)

#define HA_FATAL_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_ERR_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_WARN_PRINT(fmt...)   do{printf(fmt);}while(0)
#define HA_INFO_PRINT(fmt...)    do{printf(fmt);}while(0)
#define HA_DBG_PRINT(fmt...)     do{printf(fmt);}while(0)

#endif

#else

#define HA_FATAL_PRINT(fmt...)
#define HA_ERR_PRINT(fmt...)
#define HA_WARN_PRINT(fmt...)
#define HA_INFO_PRIN(fmt...)
#define HA_DBG_PRINT(fmt...)

#endif

/**Define software dual decode*/
/**CNcomment:定义软解双路解码*/
#define HA_SW_DUALDEC_SUPPORT

/**Define HA codec ID*/
/**CNcomment:定义HA codec ID*/
/*
*                HA CODEC_ID defination
*   |      7bit                9bit                3bit                   13bit  |
*   |---vendor---|---format---|---reserved---|---id---|
*/
#define VENDOR_HISI 0x040
#define VENDOR_NONE 0x010
#define HA_VENDOR_OFFSETK  25 /* (32-7) */
#define HA_VENDOR_MASK     (0x007fUL << HA_VENDOR_OFFSETK)
#define HA_FORMAT_OFFSETK  16 /* (32-7-9) */
#define HA_FORMAT_MASK     (0x01ffUL << HA_FORMAT_OFFSETK)
#define HA_RESERVED_OFFSET 13 /* (32-7-9-3) */
#define HA_RESERVED_MASK   (0x007UL << HA_RESERVED_OFFSET)
#define HA_ID_OFFSET       0 /* (32-7-9-3-13) */
#define HA_ID_MASK         (0x1fffUL<< HA_ID_OFFSET)

#define HA_BUILD_CODEC_ID(vendor, format, id) (((((HI_U32)vendor) << HA_VENDOR_OFFSETK) & HA_VENDOR_MASK) | ((((HI_U32)format) << HA_FORMAT_OFFSETK) & HA_FORMAT_MASK) | (((HI_U32)id) & HA_ID_MASK))
#define HA_GET_VENDOR(codec) ((HI_U32)(codec&HA_VENDOR_MASK)>>HA_VENDOR_OFFSETK)
#define HA_GET_FORMAT(codec) ((HI_U32)(codec&HA_FORMAT_MASK)>>HA_FORMAT_OFFSETK)
#define HA_GET_ID(codec)     ((HI_U32)(codec&HA_ID_MASK)>>HA_ID_OFFSET)

/********************************Macro Definition********************************/
/** \addtogroup      ACODEC */
/** @{ */  /** <!-- 【ACODEC】 */

/**Define HA codec common command ID*/
/**CNcomment:定义HA codec 公共CMD ID*/
#define HA_COMMON_CMD_ID   0xffff

/**Define HA codec format query command ID*/
/**CNcomment:定义HA codec 格式查询CMD ID*/
#define HA_CODEC_FORMAT_QUERY_CMD         ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1000)

/**Define HA codec decoder open parameter command ID*/
/**CNcomment:定义HA codec 解码器打开参数CMD ID*/
#define HA_CODEC_GET_DECOPENPARAM_CMD     ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1001)

/**Define HA codec encoder open parameter command ID*/
/**CNcomment:定义HA codec 编码器打开参数CMD ID*/
#define HA_CODEC_GET_ENCOPENPARAM_CMD     ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1002)

/**Define HA codec packet query command ID*/
/**CNcomment:定义HA codec packet解码模式CMD ID*/
#define HA_CODEC_PACKETDECODER_QUERY_CMD  ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1003)

/**Define HA codec hwdecode query command ID*/
/**CNcomment:定义HA codec hwdecode解码模式CMD ID*/
#define HA_CODEC_HARDWARECODEC_QUERY_CMD  ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1004)

/**Define HA codec set outbuf addr command ID  only for hardware codec*/
/**CNcomment:定义输出Buffer地址设置CMD ID*/
#define HA_CODEC_OUTBUFADDR_SET_CMD  ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1005)

/**Define dual decode capability support query command ID*/
/**CNcomment:定义双路解码能力查询CMD ID*/
#define HA_CODEC_DUAL_DECODE_QUERY_CMD  ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1006)

/**Define dual decode set ad output mode command ID*/
/**CNcomment:定义双路解码AD输出模式设置CMD ID*/
#define HA_CODEC_AD_OUTPUTMODE_SET_CMD  ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1007)

/**Define AD Balance Setting command ID*/
/**CNcomment:定义AD 权重设置CMD ID*/
#define HA_CODEC_AD_BALANCE_SET_CMD     ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1008)

/**Define PTS Delay command ID*/
/**CNcomment:定义PTS Delay CMD ID*/
#define HA_CODEC_PTS_DELAY_SET_CMD      ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x1009)

/**Define HA codec set whether work in compress mode command ID */
/**CNcomment:定义是否工作于在线压缩模式设置CMD ID*/
#define HA_CODEC_COMPRESSMODE_SET_CMD ((((HI_U32)HA_COMMON_CMD_ID) << 16) | 0x100A)

/**Define HA codec maximum audio channel*/
/**CNcomment:定义HA codec 最大音频通道 */
#define HA_AUDIO_MAXCHANNELS 8
/** @} */  /** <!-- ==== Macro Definition end ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      ACODEC */
/** @{ */  /** <!--  【ACODEC】 */

/**HA format definition*/
/**CNcomment:HA format 定义*/
typedef enum hiHA_FORMAT_E
{
    FORMAT_MP2 = 0x000,  /**<MPEG audio layer 1, 2.*/ /**<CNcomment:MPEG音频第一层、第二层 */
    FORMAT_MP3, /**<MPEG audio layer 1, 2, 3.*/ /**<CNcomment:MPEG音频第一层、第二层 、第三层*/
    FORMAT_AAC,
    FORMAT_AC3,
    FORMAT_DTS,
    FORMAT_VORBIS,
    FORMAT_DVAUDIO,
    FORMAT_WMAV1,
    FORMAT_WMAV2,
    FORMAT_MACE3,
    FORMAT_MACE6,
    FORMAT_VMDAUDIO,
    FORMAT_SONIC,
    FORMAT_SONIC_LS,
    FORMAT_FLAC,
    FORMAT_MP3ADU,
    FORMAT_MP3ON4,
    FORMAT_SHORTEN,
    FORMAT_ALAC,
    FORMAT_WESTWOOD_SND1,
    FORMAT_GSM,
    FORMAT_QDM2,
    FORMAT_COOK,
    FORMAT_TRUESPEECH,
    FORMAT_TTA,
    FORMAT_SMACKAUDIO,
    FORMAT_QCELP,
    FORMAT_WAVPACK,
    FORMAT_DSICINAUDIO,
    FORMAT_IMC,
    FORMAT_MUSEPACK7,
    FORMAT_MLP,
    FORMAT_GSM_MS, /**<as found in WAV.*/ /**<CNcomment:存在WAV格式中 */
    FORMAT_ATRAC3,
    FORMAT_VOXWARE,
    FORMAT_APE,
    FORMAT_NELLYMOSER,
    FORMAT_MUSEPACK8,
    FORMAT_SPEEX,
    FORMAT_WMAVOICE,
    FORMAT_WMAPRO,
    FORMAT_WMALOSSLESS,
    FORMAT_ATRAC3P,
    FORMAT_EAC3,
    FORMAT_SIPR,
    FORMAT_MP1,
    FORMAT_TWINVQ,
    FORMAT_TRUEHD,
    FORMAT_MP4ALS,
    FORMAT_ATRAC1,
    FORMAT_BINKAUDIO_RDFT,
    FORMAT_BINKAUDIO_DCT,
    FORMAT_DRA,

    FORMAT_PCM = 0x100,/**<various PCM codecs.*/ /**<CNcomment:PCM格式 */
    FORMAT_PCM_BLURAY = 0x121,

    FORMAT_ADPCM = 0x130,/**<various ADPCM codecs.*/ /**<CNcomment:ADPCM格式 */

    FORMAT_AMR_NB = 0x160,/**<various AMR codecs.*/ /**<CNcomment:AMR格式 */
    FORMAT_AMR_WB,
    FORMAT_AMR_AWB,

    FORMAT_RA_144 = 0x170,/**<RealAudio codecs.*/ /**<CNcomment:RealAudio格式 */
    FORMAT_RA_288,

    FORMAT_DPCM = 0x180,/**<various DPCM codecs.*/ /**<CNcomment:DPCM格式 */

    FORMAT_G711 = 0x190,/**<various G.7xx codecs.*/ /**<CNcomment:G.7xx格式 */
    FORMAT_G722,
    FORMAT_G7231,
    FORMAT_G726,
    FORMAT_G728,
    FORMAT_G729AB,


    FORMAT_MULTI = 0x1f0,/**<support multi codecs.*/ /**<CNcomment:多种格式 */

    FORMAT_BUTT = 0x1ff,
} HA_FORMAT_E;

/**HA format definition*/
/**CNcomment:HA_Codec定义*/
typedef enum hiHA_CODEC_ID_E
{
    /** HISI Codec*/
    HA_AUDIO_ID_PCM             = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_PCM, 0x0000),
    HA_AUDIO_ID_MP2             = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_MP2, 0x0002),
    HA_AUDIO_ID_MP3             = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_MULTI, 0x0003),
    HA_AUDIO_ID_AAC             = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_AAC, 0x001),
    HA_AUDIO_ID_BLYRAYLPCM      = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_PCM_BLURAY, 0x021),
    HA_AUDIO_ID_COOK            = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_COOK, 0x0009),
    HA_AUDIO_ID_DRA             = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_DRA, 0x007),
    HA_AUDIO_ID_WMA9STD         = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_MULTI, 0x0006),
    HA_AUDIO_ID_AMRNB           = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_AMR_NB, 0x0100),
    HA_AUDIO_ID_AMRWB           = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_AMR_WB, 0x0110),
    HA_AUDIO_ID_G711            = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_G711, 0x0102),
    HA_AUDIO_ID_G722            = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_G722, 0x0105),
    HA_AUDIO_ID_TRUEHD          = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_TRUEHD, 0x0008),
    HA_AUDIO_ID_AC3PASSTHROUGH  = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_MULTI, 0x0055),
    HA_AUDIO_ID_DTSPASSTHROUGH  = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_DTS, 0x1025),

    /** DOLBY/DTS IPR Codec*/
    HA_AUDIO_ID_DOLBY_PLUS      = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_MULTI, 0x1010),
    HA_AUDIO_ID_DOLBY_TRUEHD    = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_TRUEHD, 0x1011),
    HA_AUDIO_ID_DTSHD           = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_DTS, 0x1020),
    HA_AUDIO_ID_DTSM6           = HA_BUILD_CODEC_ID(VENDOR_NONE, FORMAT_DTS, 0x1030),

    /** FFMPEG Codec*/
    HA_AUDIO_ID_FFMPEG_DECODE   = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_MULTI,  0x03ff),
    HA_AUDIO_ID_FFMPEG_WMAPRO   = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_WMAPRO, 0x041f),

    /** CUSTOMER Codec*/
    HA_AUDIO_ID_CUSTOM_0        = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_MULTI, 0x0400),
    HA_AUDIO_ID_CUSTOM_1        = HA_BUILD_CODEC_ID(VENDOR_HISI, FORMAT_MULTI, 0x0401),
} HA_CODEC_ID_E;

/**HA Audio Output AD Mode definition*/
/**CNcomment:HA OUTPUT AD MODE 定义*/
typedef enum
{
    HA_OUTPUT_AD_OFF = 0,
    HA_OUTPUT_AD_ON,
    HA_OUTPUT_AD_OFF_WITHASSOC,
    HA_OUTPUT_AD_BUTT
} HA_CODEC_OUTPUT_AD_MODE_E;

/**Define query parameter of an HA codec.*/
/**CNcomment:定义HA codec 查询参数*/
typedef struct
{
    HI_U32  enCmd;
    HI_VOID *pPara;
} HA_CODEC_PARAMETER_QUERY_S;

/**Define format query parameter of an HA codec.*/
/**CNcomment:定义HA codec 格式查询参数*/
typedef struct
{
    HI_U32  enCmd;
    HA_FORMAT_E enFormat;
} HA_CODEC_FORMAT_QUERY_PARAM_S;

/**Define packet query parameter of an HA codec.*/
/**CNcomment:定义HA codec packet解码模式查询参数*/
typedef struct
{
    HI_U32  enCmd;              /**<packet query command.*/ /**<CNcomment:packet模式查询命令*/
    HI_BOOL bPacketDecoder;     /**<Packet mode.In this mode, the decoder can decode the next packet only after processing an entire packet. */
                                /**<CNcomment:packet模式.在这种模式下，只有处理完当前整个packet后解码器才能处理下一个packet包*/
} HA_CODEC_PACKETDECODER_QUERY_PARAM_S;

/**Define hardware decode query parameter of an HA codec.*/
/**CNcomment:定义HA codec hardware解码模式查询参数*/
typedef struct
{
    HI_U32  enCmd;      /**<hwdecode query command.*/ /**<CNcomment: hwdecode模式查询命令*/
    HI_BOOL bHwCodec;   /**<HI_TRUE: hwdecode mode else swdecode mode. */ /**<CNcomment:hwdecode模式*/
} HA_CODEC_HARDWARECODER_QUERY_PARAM_S;

/**Define query parameter of dual decode capability.*/
/**CNcomment:定义双路解码能力查询参数*/
typedef struct
{
    HI_U32  enCmd;                  /**<query dual decode capability command.*/ /**<CNcomment: 查询双路解码能力命令*/
    HI_BOOL bDualDecSupport;        /**<support dual decode or not. */  /**<CNcomment:是否支持双路解码*/
} HA_CODEC_DUALDECODE_QUERY_PARAM_S;

/**Define setting parameter of dual decode ad output mode.*/
/**CNcomment:设置双路解码AD输出模式**/
typedef struct
{
    HI_U32               enCmd;             /**<set AD Output Mode command.*/ /**<CNcomment: 设置AD输出模式命令*/
    HA_CODEC_OUTPUT_AD_MODE_E  enAdMode;    /**<AD Output Mode. */  /**<CNcomment:AD输出模式*/
} HA_CODEC_ADMODE_SET_PARAM_S;

/**Define setting parameter of AD Balance.*/
/**CNcomment:设置AD 权重**/
typedef struct
{
    HI_U32  enCmd;                      /**<set AD Balance command.*/ /**<CNcomment: 设置AD权重命令*/
    HI_S16  s16ADBalance;               /**<set the value of AD Balance */  /**<CNcomment:设置AD权重值*/
} HA_CODEC_ADBALANCE_SET_PARAM_S;

/**Define set parameter of whether work in compress mode.*/
/**CNcomment:定义是否工作于在线压缩模式设置参数*/
typedef struct
{
    HI_U32  enCmd;          /**<set the compress mode command. */ /**<CNcomment:设置是否在线压缩模式命令*/
    HI_BOOL bCompressMode;  /**<whether work in compress mode. */ /**<CNcomment:是否工作于在线压缩模式*/
} HA_CODEC_COMPRESSMODE_SET_PARAM_S;

/**Define the error codes of an HA codec.*/
/**CNcomment:定义HA codec 错误码*/
typedef enum hiHA_ERRORTYPE_E
{
    HA_ErrorNone = 0, /**<None error.*/ /**<CNcomment:没有错误 */

    HA_ErrorInsufficientResources = (HI_S32) 0x80001000,/**<The device fails to be created due to insufficient resources.*/ /**<CNcomment:资源不够，创建设备失败 */

    HA_ErrorInvalidParameter = (HI_S32) 0x80001001, /**<The input parameter is invalid.*/ /**<CNcomment:输入参数非法 */

    HA_ErrorStreamCorrupt = (HI_S32) 0x80001002, /**<The decoding fails due to incorrect input streams.*/ /**<CNcomment:输入码流错误，解码失败 */

    HA_ErrorNotEnoughData = (HI_S32) 0x80001003,/**<The decoding ends due to insufficient streams.*/ /**<CNcomment:输入码流不够，退出解码 */

    HA_ErrorDecodeMode = (HI_S32) 0x80001004,/**<The decoding mode is not supported.*/ /**<CNcomment:解码模式不支持 */

    HA_ErrorNotSupportCodec = (HI_S32) 0x80001005,/**<The codec is not supported.*/ /**<CNcomment:解码器不支持 */

    HA_ErrorInBufFull = (HI_S32) 0x80001006,/**<Input buffer is full.*/ /**<CNcomment:输入缓存满 */

    HA_ErrorOutBufEmpty = (HI_S32) 0x80001007,/**<Output buffer is empty.*/ /**<CNcomment:输出缓存空 */

#ifdef HA_SW_DUALDEC_SUPPORT
    HA_ErrorNotEnoughADData = (HI_S32) 0x80001008,/**<The decoding ends due to insufficient ad streams.*/ /**<CNcomment:ad输入码流不够，退出解码 */

    HA_ErrorADDecFailed = (HI_S32) 0x80001009,
#endif

    HA_ErrorMax = 0x9FFFFFFF
} HI_HA_ERRORTYPE_E;

/**Definition of the distribution mode of the channels of an HA codec.*/
/**CNcomment:定义HA codec 声道分布模式*/
typedef enum hiHA_CHANNELTYPE_E
{
    HA_AUDIO_ChannelNone = 0x0,    /**< Unused or empty.*/ /**<CNcomment:未使用或为空*/
    HA_AUDIO_ChannelLF  = 0x1,     /**< Left front.*/ /**<CNcomment:左前*/
    HA_AUDIO_ChannelRF  = 0x2,     /**< Right front.*/ /**<CNcomment:右前*/
    HA_AUDIO_ChannelCF  = 0x3,     /**< Center front.*/ /**<CNcomment:中置*/
    HA_AUDIO_ChannelLS  = 0x4,     /**< Left surround.*/ /**<CNcomment:左环绕*/
    HA_AUDIO_ChannelRS  = 0x5,     /**< Right surround.*/ /**<CNcomment:右环绕*/
    HA_AUDIO_ChannelLFE = 0x6,     /**< Low frequency effects.*/ /**<CNcomment:低音*/
    HA_AUDIO_ChannelCS  = 0x7,     /**< Back surround.*/ /**<CNcomment:后环绕*/
    HA_AUDIO_ChannelLR  = 0x8,     /**< Left rear.*/ /**<CNcomment:左后*/
    HA_AUDIO_ChannelRR  = 0x9,     /**< Right rear.*/ /**<CNcomment:右后*/
    HA_AUDIO_ChannelMax = 0x7FFFFFFF
} HI_HA_CHANNELTYPE_E;

/**Definition of the decoding mode of an HA decoder.*/
/**CNcomment:定义HA 解码器解码模式*/
typedef enum hiHA_DECODEMODE_E
{
    HD_DEC_MODE_RAWPCM = 0,        /**<PCM decoding mode.*/ /**<CNcomment:PCM 解码模式*/
    HD_DEC_MODE_THRU,              /**<SPIDF61937 passthrough decoding mode only, such as AC3/DTS.*/ /**<CNcomment:透传解码模式*/
    HD_DEC_MODE_SIMUL,             /**<PCM and passthrough decoding mode.*/ /**<CNcomment:PCM + 透传解码模式*/
    HD_DEC_MODE_BUTT = 0x7FFFFFFF
} HI_HA_DECODEMODE_E;

/**Definition of the HA codec version.*/
/**CNcomment:HA codec 版本定义*/
typedef union hiHI_HAAUDIO_VERSIONTYPE_U
{
    struct
    {
        HI_U8 u8VersionMajor;      /**< Major version.*/ /**<CNcomment:主版本号 */
        HI_U8 u8VersionMinor;      /**< Minor version.*/ /**<CNcomment:副版本号 */
        HI_U8 u8Revision;          /**< Revision version.*/ /**<CNcomment:修订版本 */
        HI_U8 u8Step;              /**< Step version.*/ /**<CNcomment:阶段性版本 */
    } s;
    HI_U32 u32Version;
} HI_HAAUDIO_VERSIONTYPE_U;

/**Definition of the data types of the PCM module in an HA codec HA codec.*/
/**CNcomment:pcm 数据类型结构体定义*/
typedef struct hiHA_PCMMODETYPE_S
{
    HI_U32              u32DesiredOutChannels;/**<Input,number of expected output channels.If the number of original channels is not equal to the number of expected output channels,
                                                  the decoded automatically performs down-mixing or up-mixing,In this way,
                                                  the number of output channels is equal to the value of u32DesiredOutChannels,1: mono; 2 stereo. \n*/
                                              /**<CNcomment:IN 期望输出声道数目. 如果码流原始声道不等于
                                                  期望输出声道，解码器自动进行down-mix 或up-mix 处理，使得输出
                                                  声道等于u32DesiredOutChannels.  1: mono, 2 for stereo*/
    HI_BOOL             bInterleaved;/**<Whether the PCM data is interleaved.
                                         HI_TRUE: interleaved mode (L/R/L/R.. .L/R/L/R).
                                         HI_FALSE: non-interleaved mode (L/L/L.../R/R/R...). \n*/
                                     /**<CNcomment:IN PCM 数据是否交织模式
                                         HI_TRUE:  交织模式: L/R/L/R...L/R/L/R.
                                         HI_FALSE: 非交织模式: L/L/L......../R/R/R....... */
    HI_U32              u32BitPerSample;/**<Input Bit width of the PCM data.Only 16-bit or 24-bit width is supported.For the data of 16-bit width, 16-bit memory is used,
                                            For the data of 24-bit width, 32-bit memory is used,The active bits of the 24-bit PCM data are upper bits, and the lower eight bits are padded with 0s. \n*/
                                        /**<CNcomment:IN PCM 数据位宽,仅支持16 或24 数据位宽 16bit 位宽占用16bit 内存,
                                            24bit 位宽占用32比特内存.24bit PCM数据有效为在高位，低8 位补0*/
    HI_U32              u32DesiredSampleRate; /**<Input, expected output sampling rate.*/ /**<CNcomment:IN 期望输出采样频率*/
    HI_HA_CHANNELTYPE_E enChannelMapping[HA_AUDIO_MAXCHANNELS]; /**<Input, distribution mode of output channels.*/ /**<CNcomment:IN 输出声道分布模式*/
} HI_HA_PCMMODETYPE_S;

/**Configuration parameters required by an HA codec for creating devices.*/
/**CNcomment:HA 解码器创建设备的配置参数*/
typedef struct hiHADECODE_OPENPARAM_S
{
    HI_HA_DECODEMODE_E  enDecMode;  /**<IN Decode Mode.*/ /**<CNcomment:解码模式*/
    HI_HA_PCMMODETYPE_S sPcmformat; /**<IN data types of pcm module.*/ /**<CNcomment:PCM数据类型结构体*/
    HI_VOID *           pCodecPrivateData;/**<Input, pointer to private data.If the decoder does not contain private data, this parameter is set to 0. */
                                          /**<CNcomment:IN  指向私有配置结构体的指针，如果解码器没有私有配置，设置为0*/
    HI_U32              u32CodecPrivateDataSize;/**<Input, size of the private data.If the decoder does not contain private data, this parameter is set to 0. */
                                                /**<CNcomment:IN 私有结构体大小, 如果解码器没有私有配置，设置为0*/
} HI_HADECODE_OPENPARAM_S;

/**Input streams of an HA decoder.*/
/**CNcomment:HA 解码器输入码流结构体*/
typedef struct hiHADECODE_INPACKET_S
{
    HI_U32  u32PtsMs;  /**<Input, presentation time stamp (PTS) (in ms).Note: The HA decoder must discard this information. */ /**<CNcomment:当前时间戳*/
    HI_U8  *pu8Data;   /**<Input/output, pointer to input streams.The decoder update this pointer after decoding. */ /**<CNcomment:IN/OUT 输入码流指针，解码完成后解码器更新该指针*/
    HI_S32  s32Size;   /**<Input, size (in byte) of input streams.*/ /**<CNcomment:IN 输入码流大小. unit: Byte*/
    HI_BOOL bEndOfFrm; /**<Input, the last frame or not.*/ /**<CNcomment:IN 是否最后一帧*/
} HI_HADECODE_INPACKET_S;

/**Get Open parameter of an HA decoder.*/
/**CNcomment:获取HA 解码器打开参数*/
typedef struct
{
    HI_U32  enCmd;
    HI_HADECODE_OPENPARAM_S *pstDecodeParams;
} HA_CODEC_GET_DECOPENPARAM_PARAM_S;

/**Definition of the decoder output pts infomations.*/
/**CNcomment:pts输出信息*/
typedef struct hiHI_HA_OUTPTSINFO_S
{
    union
    {
        HI_U32 u32SwDecoderBytesLeft;   /**<left bytes of software decoder.*/ /**<CNcomment:软解剩余字节数 */
        HI_U32 u32HwDecoderPtsReadPos;  /**<pts read position of software decoder.*/ /**<CNcomment:硬解pts读位置 */
    } unPts;
    
    union
    {
        HI_U32 u32SwAdDecoderBytesLeft;     /**<ad left bytes of software decoder.*/ /**<CNcomment:软解ad剩余字节数 */
        HI_U32 u32HwAdDecoderPtsReadPos;    /**<ad pts read position of software decoder.*/ /**<CNcomment:硬解ad pts读位置 */
    } unAdPts;
} HI_HA_OUTPTSINFO_S;

/**Output structure of the HA decoder.*/
/**CNcomment:HA 解码器输出结构体*/
typedef struct hiHI_HADECODE_OUTPUT_S
{
    HI_S32 *ps32PcmOutBuf;       /**<Input, pointer to the decoded PCM data.
                                     Note: 1) ps32PcmOutBuf must be 32-word aligned.
                                           2) ps32PcmOutBuf is NULL when the decoder is working in HD_DEC_MODE_THRU mode.\n*/
                                 /**<CNcomment:IN 指向PCM 解码输出的指针. */

    HI_U32  u32PcmOutBufSize;    /**<Input, size (in byte) of the buffer for storing the decoded PCM data
                                     Note: u32PcmOutBufSize is 0 when the decoder is working in HD_DEC_MODE_THRU mode.\n*/
                                 /**<CNcomment:IN  PCM 解码输出缓冲区大小. unit:Byte */

    HI_S32 *ps32BitsOutBuf;      /**<Input, pointer to the decoded passthrough data.
                                     Note: 1) ps32BitsOutBuf must be 32-word aligned.
                                           2) ps32BitsOutBuf is NULL when the decoder is working in HD_DEC_MODE_RAWPCM mode.\n*/
                                 /**<CNcomment:IN 指向透传解码输出的指针. */

    HI_U32  u32BitsOutBufSize;   /**<Input, size (in byte) of the buffer for storing the decoded passthrough data (IEC61937)
                                     Note: u32BitsOutBufSize is 0 when the decoder is working in HD_DEC_MODE_RAWPCM mode.\n*/
                                 /**<CNcomment:IN 透传(IEC61937) 解码输出缓冲区大小. unit:Byte */

    HI_U32  u32PcmOutSamplesPerFrame; /**<Output, number of output sampling points after the PCM data is decoded.*/ /**<CNcomment:OUT. 解码Pcm 输出样点数 */
    HI_BOOL bInterleaved;             /**<Output, interleaved mode for PCM decoding.*/ /**<CNcomment:OUT PCM  解码交织模式*/
    HI_U32  u32BitPerSample;          /**<Output, bit per sampling.*/ /**<CNcomment:OUT 位宽*/
    HI_U32  u32BitsOutBytesPerFrame;  /**<Output, size (in byte) of the passthrough frame.*/ /**<CNcomment:OUT 透传输出帧大小. unit: Byte*/
    HI_U32  u32OutChannels;           /**<Output, number of output channels.*/ /**<CNcomment:OUT 输出声道数*/
    HI_U32  u32OutSampleRate;         /**<Output, output sampling rate.*/ /**<CNcomment:OUT. 输出采样频率*/
    HI_U32  u32OrgChannels;           /**<Output, number of original channels.*/ /**<CNcomment:OUT 码流原始声道数*/
    HI_U32  u32OrgSampleRate;         /**<Output, original sampling rate.*/ /**<CNcomment:OUT 码流原始采样频率*/
    HI_U32  u32BitRate;               /**<Output, bit rate (in bit/s) of the compressed streams.*/ /**<CNcomment:OUT 码流压缩率unit: bit/S*/
    HI_HA_OUTPTSINFO_S stPtsInfo;     /**<Output, pts information.*/ /**<CNcomment:OUT. pts信息*/
    HI_U32  u32FrameIndex;   /**<Output, output frame index.*/ /**<CNcomment:OUT. 输出帧序号*/
} HI_HADECODE_OUTPUT_S;

/**Definition of the HA decoder.*/
/**CNcomment:HA 解码器设备定义*/
typedef struct hiHA_DECODE_S
{
    const HI_PCHAR szName;/**<Input, description information about a decoder, such as MP3 or AAC. */
                          /**<CNcomment:IN 解码器描述名字，如MP3,AAC*/

    const HI_U32 enCodecID;/**<Input, decoder ID.Note: This ID is the identifier of a decoder, and must be unique. */
                           /**<CNcomment:IN 解码器标识，注意: 此标识作为解码器唯一身份标识，必须唯一而不能有重复*/

    const HI_HAAUDIO_VERSIONTYPE_U uVersion;/**<Input, decoder version.*/ /**<CNcomment:IN 解码器版本描述*/

    const HI_PCHAR pszDescription;/**<Input, detailed information about a decoder.*/ /**<CNcomment:IN 解码器详细描述信息*/



    struct hiHA_DECODE_S *pstNext;/**<Output, pointer to the next decoder.This member variable is maintained by the client that calls the HA codec,
                                      Therefore, the developers of the HA decoder can ignore this member variable. \n*/
                                  /**<CNcomment:OUT  指向下个解码器设备的指针.  该成员变量由调用HA Codec 的客户端维护, HA 解码器开发者可以忽略该成员*/

    HI_VOID  * pDllModule;/**<Output, pointer to the dll symbol of an HA decoder,This member variable is maintained by the client that calls the HA codec,
                              Therefore, the developers of the HA decoder can ignore this member variable.\n*/
                          /**<CNcomment:OUT 该成员变量由调用HA Codec 的客户端维护, HA 解码器开发者可以忽略该成员*/

    /**
    \brief Initializes a decoder. CNcomment:初始化解码器 CNend
    \attention \n
    \param[in] pstOpenParam pointer of the open params CNcomment:open参数结构体指针 CNend
    \param[out] phDecoder   pointer of the decoder handle CNcomment:解码器句柄指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecInit)(HI_VOID * *phDecoder,
                                 const HI_HADECODE_OPENPARAM_S * pstOpenParam);

    /**
    \brief DeInitializes a decoder. CNcomment:去初始化解码器 CNend
    \attention \n
    \param[in] hDecoder   pointer of the decoder handle CNcomment:解码器句柄指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecDeInit)(HI_VOID* hDecoder);

    /**
    \brief Configure a decoder dynamically. You can call this API when a decoder works.
    CNcomment:解码器动态配置方法，用户可以在解码器运行时调用该接口 CNend
    \attention \n
    \param[in] hDecoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[in] pstConfigStructure pointer to application allocated structure to be used for initialization by the decoder CNcomment:解码器初始化结构体 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecSetConfig)(HI_VOID* hDecoder, HI_VOID* pstConfigStructure);

    /**
    \brief Maximum size of the buffer required for storing the PCM data decoded by the decoder,
    The memory needs to be allocated based on the size on the client.
    CNcomment:解码器PCM输出所需最大缓冲区size. 客户端需要根据该方法分配内存 CNend
    \attention \n
    \param[in] hDecoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[out] pu32OutSizes pointer to the max size of the pcm audio frame. unit:Byte CNcomment:最大PCM输出缓冲区大小 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecGetMaxPcmOutSize)(HI_VOID* hDecoder,
            HI_U32* pu32OutSizes);

    /**
    \brief Maximum size of the buffer required for storing the passthrough data decoded by the decoder,
    The memory needs to be allocated based on the size on the client.
    CNcomment:解码器透传输出所需最大缓冲区size.客户端需要根据该方法分配内存 CNend
    \attention \n
    \param[in] hDecoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[out] pu32OutSizes pointer to the max size of the iec61937 audio frame. unit:Byte CNcomment:最大透传输出缓冲区大小 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecGetMaxBitsOutSize)(HI_VOID* hDecoder,
            HI_U32* pu32OutSizes);

    /**
    \brief This method is used to decode a frame. CNcomment:解码器解码一帧方法 CNend
    \attention \n
    \param[in] hDecoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[in] pstApkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[out] pstAOut pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecDecodeFrame)(HI_VOID* hDecoder,
                                        HI_HADECODE_INPACKET_S* pstApkt,
                                        HI_HADECODE_OUTPUT_S* pstAOut);

#ifdef HA_SW_DUALDEC_SUPPORT
    /**
    \brief This method is used to dual decode a frame. CNcomment:解码器双路解码一帧方法 CNend
    \attention \n
    \param[in] hDecoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[in] pstApkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[in] pstADApkt pointer to AD audio stream packet CNcomment:AD音频输入流指针 CNend
    \param[out] pstAOut pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*DecDualDecodeFrame)(HI_VOID* hDecoder,
                                            HI_HADECODE_INPACKET_S* pstApkt,
                                            HI_HADECODE_INPACKET_S* pstADApkt,
                                            HI_HADECODE_OUTPUT_S* pstAOut);
#endif

} HI_HA_DECODE_S;

/**Parameters for creating an HA encoder.*/
/**CNcomment:HA 编码器创建配置参数*/
typedef struct hiHAENCODE_OPENPARAM_S
{
    HI_U32      u32DesiredOutChannels; /**<IN Number of channels (e.g 2 for stereo).*/ /**<CNcomment:声道数*/
    HI_BOOL     bInterleaved;       /**<IN interleave or not.*/ /**<CNcomment:交织模式*/
    HI_S32      s32BitPerSample;    /**<IN bitwidth per sample.*/ /**<CNcomment:位宽*/
    HI_U32      u32DesiredSampleRate; /**<IN desired samplerate.*/ /**<CNcomment:期望采样率*/
    HI_U32      u32SamplePerFrame;  /**<IN Pcm samples per frame for encoder.*/ /**<CNcomment:每帧采样点*/
    HI_VOID*    pCodecPrivateData;  /**<IN Pointer of decoder private open parameters note: if there is none private parameters, pCodecPrivateData=0.*/
                                    /**<CNcomment:私有参数*/
    HI_U32      u32CodecPrivateDataSize;/**<IN Size of decoder private open parameters note: if there is none private parameters, u32CodecPrivateDataSize=0.*/
                                        /**<CNcomment:私有参数大小*/
} HI_HAENCODE_OPENPARAM_S;

/**HA input pcm  packet struct.*/
/**CNcomment:packet模式结构体*/
typedef struct hiHAENCODE_INPACKET_S
{
    HI_U32  u32PtsMs;       /**<IN  PTS (unit:MS) Note: HA encoder should discard this infomation.*/
                            /**<CNcomment:PTS，单位:毫秒*/
    HI_U8*  pu8Data;        /**<IN/OUT pointer to input auduo pcm data note: HA encoder would update pu8Data after encode.*/
                            /**<CNcomment:输入数据指针*/
    HI_U32  u32Size;        /**<IN/OUT size of the input auduo data. unit: Byte.*/ /**<CNcomment:输入数据大小*/
} HI_HAENCODE_INPACKET_S;

/**HA encoder output struct.*/
/**CNcomment:编码器输出结构体*/
typedef struct hiHAENCODE_OUTPUT_S
{
    HI_S32* ps32BitsOutBuf;         /**<IN the pointer to encoded bitstream output buffer note: ps32BitsOutBuf must  be word32-aligned.*/
                                    /**<CNcomment:编码数据输出buffer*/
    HI_U32  u32BitsOutBufSize;      /**<IN the buffer size of bitstream output buffer. unit:Byte.*/ /**<CNcomment:编码器输出buffer大小*/
    HI_U32  u32BitsOutBytesPerFrame;/**<IN size of the encoded audio data frame ,unit: Byte.*/ /**<CNcomment:编码数据帧大小，单位:Byte*/
    HI_U32  u32BitRate;             /**<IN compress bit rate of the audio stream.*/ /**<CNcomment:比特率*/
} HI_HAENCODE_OUTPUT_S;

/**HA encoder struct define.*/
/**CNcomment:定义编码器结构体*/
typedef struct hiHA_ENCODE_S
{
    const HI_PCHAR szName;/**<Input, description information about audio encoder. */
    /**<CNcomment:IN 编码器描述名字*/

    const HI_U32 enCodecID;/**<Input, encoder ID.Note: This ID is the identifier of a encoder, and must be unique. */
    /**<CNcomment:IN 编码器标识，注意: 此标识作为编码器唯一身份标识，必须唯一而不能有重复*/

    const HI_HAAUDIO_VERSIONTYPE_U uVersion;/**<Input, encoder version.*/ /**<CNcomment:IN 编码器版本描述*/

    const HI_PCHAR pszDescription;/**<Input, Description infomation of the audio encoder. */ /**<CNcomment:IN 编码器详细描述信息*/

    struct hiHA_ENCODE_S* pstNext;/**<OUT pointer to next HA enocder.manager by client.client:The layer of software that invokes the methods of the HA encoder. */
    /**<CNcomment:OUT 下个编码器结构体指针，由客户管理 */

    HI_VOID*   pDllModule;/**<Output, pointer to the dll symbol of an HA encoder.This member variable is maintained by the client that calls the HA codec
                              Therefore, the developers of the HA encoder can ignore this member variable. \n*/
    /**<CNcomment:OUT 该成员变量由调用HA Codec 的客户端维护, HA 编码器开发者可以忽略该成员*/

    /**
    \brief Initializes a encoder. CNcomment:初始化编码器 CNend
    \attention \n
    \param[in] pstOpenParam pointer of the open params CNcomment:open参数结构体指针 CNend
    \param[out] phEncoder   pointer of the encoder handle CNcomment:编码器句柄指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*EncodeInit)(HI_VOID * *phEncoder,
                                    const HI_HAENCODE_OPENPARAM_S* pstOpenParam);

    /**
    \brief DeInitializes a encoder. CNcomment:去初始化编码器 CNend
    \attention \n
    \param[in] hEncoder   pointer of the encoder handle CNcomment:编码器句柄 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*EncodeDeInit)(HI_VOID* hEncoder);

    /**
    \brief Configure a encoder dynamically. It can be invoked anytime after the Encoder has been loaded.
    CNcomment:编码器动态配置方法。用户可以在编码器运行时调用该接口 CNend
    \attention \n
    \param[in] hEncoder   the encoder handle CNcomment:编码器句柄 CNend
    \param[in] pstConfigStructure pointer to application allocated structure to be used for initialization by the encoder CNcomment:编码器初始化结构体 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*EncodeSetConfig)(HI_VOID* hEncoder, HI_VOID* pstConfigStructure);

    /**
    \brief Maximum output size of an encoded bitstream CNcomment:编码器输出所需最大缓冲区size CNend
    \attention \n
    \param[in] hEncoder   the decoder handle CNcomment:解码器句柄 CNend
    \param[out] pu32OutSizes pointer to the max size of the encoded audio frame. unit:Byte CNcomment:最大编码帧输出缓冲区大小 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*EncodeGetMaxBitsOutSize)(HI_VOID* hEncoder,
            HI_U32* pu32OutSizes);

    /**
    \brief This method is used to encode a frame. CNcomment:编码器编码一帧方法 CNend
    \attention \n
    \param[in] hEncoder   the decoder handle CNcomment:编码器句柄 CNend
    \param[in] pstApkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[out] pstAOut pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:成功 CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:失败 CNend
    \see \n
    N/A
    */
    HI_HA_ERRORTYPE_E (*EncodeFrame)(HI_VOID* hEncoder,
                                     HI_HAENCODE_INPACKET_S* pstApkt,
                                     HI_HAENCODE_OUTPUT_S* pstAOut);
} HI_HA_ENCODE_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* #ifndef __HI_AUDIO_CODEC_H__ */
