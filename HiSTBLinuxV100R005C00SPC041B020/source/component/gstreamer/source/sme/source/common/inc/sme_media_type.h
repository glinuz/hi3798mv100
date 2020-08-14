/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_media_type.h
 * @brief   sme media type 定义
 * @author
 * @date    2014/3/25
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/25
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __SME_MEDIA_TYPE_H__
#define __SME_MEDIA_TYPE_H__

#include <stdarg.h>
#include "osal_type.h"

#ifdef MOD_NAME
#undef MOD_NAME
#define MOD_NAME "SmePlayer"
#endif

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#define M_SME_STREAM_NUM_MAX                    (256)
#define M_SME_AVC_CFG_ITEM_MAX                  (256)
#define M_SME_HEVC_CFG_ITEM_MAX                 (256)
#define M_SME_MAKE_FOURCC_BE(a,b,c,d)           ((V_UINT32)((d)|(c)<<8|(b)<<16|(a)<<24))
#define M_SME_MAKE_FOURCC(a,b,c,d)              ((V_UINT32)((a)|(b)<<8|(c)<<16|(d)<<24))
#define M_SME_MS_TO_NS                          1000000
#define M_SME_SEC_TO_MS                         1000
#define M_SME_MS_TO_US                          1000

#define M_SME_TS_RESCALE(pts, unit, num, base)  \
    (V_INT64)(base > 0 ? ((((num * 1.0) / base) * unit) * pts) : pts)

#define M_SME_TS_SCALETOBASE(pts, unit, num, base) \
    (V_INT64)((unit > 0 && num > 0) ? ((((base * 1.0) / num) / unit) * pts) : pts)

#define M_SME_MIME_LEN_MAX                      64

#define M_SME_VIDEO_W_MIN                       32
#define M_SME_VIDOE_H_MIN                       32

#define M_SME_MIN(x, y)                         (x) < (y) ? (x) : (y)
#define M_SME_MAX(x, y)                         (x) < (y) ? (y) : (x)
#define M_SME_UINT64_MAX                        (0xFFFFFFFFFFFFFFFFULL)
#define M_SME_INT64_MAX                         (0x7FFFFFFFFFFFFFFFLL)
#define M_SME_UINT32_MAX                        (0xFFFFFFFF)
#define M_SME_ELEMENTS_ARRAY(array)             (sizeof(array)/sizeof(array[0]))

#define M_SME_LANG_MAX_LEN                      8
//与ffmpeg匹配
typedef enum _tagMediaCodecId
{
    E_SME_MEDIA_CODEC_ID_NONE = 0,
    E_SME_MEDIA_CODEC_ID_MPEG1VIDEO,
    E_SME_MEDIA_CODEC_ID_MPEG2VIDEO,
    E_SME_MEDIA_CODEC_ID_H263 = 5,
    E_SME_MEDIA_CODEC_ID_MJPEG = 8,
    E_SME_MEDIA_CODEC_ID_JPEG = 10,
    E_SME_MEDIA_CODEC_ID_MPEG4 = 13,
    E_SME_MEDIA_CODEC_ID_MSMPEG4V3 = 17,  /* CodecId of Divx3 */
    E_SME_MEDIA_CODEC_ID_H264 = 28,
    E_SME_MEDIA_CODEC_ID_RV30 = 69,
    E_SME_MEDIA_CODEC_ID_RV40,
    E_SME_MEDIA_CODEC_ID_VC1,
    E_SME_MEDIA_CODEC_ID_WMV3,
    E_SME_MEDIA_CODEC_ID_AVS=83,
    E_SME_MEDIA_CODEC_ID_CAVS=88,
    E_SME_MEDIA_CODEC_ID_VP6=92,
    E_SME_MEDIA_CODEC_ID_VP6F=93,
    E_SME_MEDIA_CODEC_ID_VP6A=107,
    E_SME_MEDIA_CODEC_ID_VP8=141,
    E_SME_MEDIA_CODEC_ID_PCM_S16LE = 0x10000,
    E_SME_MEDIA_CODEC_ID_PCM_S16BE,
    E_SME_MEDIA_CODEC_ID_PCM_U16LE,
    E_SME_MEDIA_CODEC_ID_PCM_U16BE,
    E_SME_MEDIA_CODEC_ID_PCM_S8,
    E_SME_MEDIA_CODEC_ID_PCM_U8,
    E_SME_MEDIA_CODEC_ID_PCM_MULAW,
    E_SME_MEDIA_CODEC_ID_PCM_ALAW,
    E_SME_MEDIA_CODEC_ID_PCM_S32LE,
    E_SME_MEDIA_CODEC_ID_PCM_S32BE,
    E_SME_MEDIA_CODEC_ID_PCM_U32LE,
    E_SME_MEDIA_CODEC_ID_PCM_U32BE,
    E_SME_MEDIA_CODEC_ID_PCM_S24LE,
    E_SME_MEDIA_CODEC_ID_PCM_S24BE,
    E_SME_MEDIA_CODEC_ID_PCM_U24LE,
    E_SME_MEDIA_CODEC_ID_PCM_U24BE,
    E_SME_MEDIA_CODEC_ID_PCM_DVD = 0x10013,
    E_SME_MEDIA_CODEC_ID_PCM_F32BE,
    E_SME_MEDIA_CODEC_ID_PCM_F32LE,
    E_SME_MEDIA_CODEC_ID_PCM_BLURAY = 0x10018,
    E_SME_MEDIA_CODEC_ID_ADPCM_IMA_WAV = 0x11001,  /* various ADPCM codecs */
    E_SME_MEDIA_CODEC_ID_ADPCM_MS = 0x11006,
    E_SME_MEDIA_CODEC_ID_AMR_NB = 0x12000,
    E_SME_MEDIA_CODEC_ID_AMR_WB,
    E_SME_MEDIA_CODEC_ID_MP2 = 0x15000,
    E_SME_MEDIA_CODEC_ID_MP3,
    E_SME_MEDIA_CODEC_ID_AAC,
    E_SME_MEDIA_CODEC_ID_AC3,
    E_SME_MEDIA_CODEC_ID_DTS,
    E_SME_MEDIA_CODEC_ID_VORBIS = 0x15005,
    E_SME_MEDIA_CODEC_ID_WMAV1 = 0x15007,
    E_SME_MEDIA_CODEC_ID_WMAV2 = 0x15008,
    E_SME_MEDIA_CODEC_ID_WMAVpro = 0x15026,
    E_SME_MEDIA_CODEC_ID_WMAVLOSSLESS = 0x15027,
    E_SME_MEDIA_CODEC_ID_FLAC = 0x1500C,
    E_SME_MEDIA_CODEC_ID_GSM = 0x15012,
    E_SME_MEDIA_CODEC_ID_GSM_MS = 0x1501E,
    E_SME_MEDIA_CODEC_ID_EAC3 = 0x15029,
    E_SME_MEDIA_CODEC_ID_MP1 = 0x1502B,
    E_SME_MEDIA_CODEC_ID_TRUEHD = 0x1502D,
    E_SME_MEDIA_CODEC_ID_HEVC = M_SME_MAKE_FOURCC_BE('H', '2', '6', '5'),
    E_SME_MEDIA_CODEC_ID_DRA  = M_SME_MAKE_FOURCC_BE('D', 'R', 'A', '1'),
    E_SME_MEDIA_CODEC_ID_BUTTON,
}E_SME_MEDIA_CODEC_ID;

typedef enum _tagVC1PROFILE
{
    E_SME_VC1_PROFILE_SIMPLE = 0,
    E_SME_VC1_PROFILE_MAIN,
    E_SME_VC1_PROFILE_COMPLEX,
    E_SME_VC1_PROFILE_ADVANCED,
}E_SME_VC1_PROFILE;

typedef enum _tagMediaType
{
    E_SME_MEDIA_TYPE_VIDEO = 0,
    E_SME_MEDIA_TYPE_AUDIO,
    E_SME_MEDIA_TYPE_IMG,
    E_SME_MEDIA_TYPE_TEXT,
    E_SME_MEDIA_TYPE_BUTTON
}E_SME_MEDIA_TYPE;

typedef enum _tagMediaFrameType
{
    E_SME_MEDIA_FRAME_TYPE_OTHER = 1,
    E_SME_MEDIA_FRAME_TYPE_IDR = 1<<1,
    E_SME_MEDIA_FRAME_TYPE_CONFIGDATA = 1<<16,
}E_SME_MEDIA_FRAME_TYPE;

typedef struct _tagFrameAllocator
{
    V_VOID* pvAllocCtx;                        //IN
    V_VOID* (*pfnLockBuf)(V_VOID* pvCtx, V_UINT32 u32BufSize, V_INT32 i32streamindex);//IN,pvCtx就是pvAllocCtx
    V_VOID  (*pfnUnLockBuf)(V_VOID* pvCtx, V_VOID* pvDataBuf);//IN
}ST_SME_FRAME_ALLOCATOR;

typedef struct _tagFrameInfo
{
    V_INT64         i64Pts;                            /*OUT *< 时间戳，单位i32TsBase，如果没有时间戳，设置成-1 */
    V_INT64         i64Dts;                            /*OUT *< 时间戳，单位i32TsBase，如果没有时间戳，设置成-1 */
    V_INT32         i32TsBaseNum;                      /*OUT *< pts 及 dts 换算成ms的单位 ，分子*/
    V_INT32         i32TsBaseDen;                      /*OUT *< pts 及 dts 换算成ms的单位 ，分母*/
    V_UINT32        u32Duration;                       /*OUT *< 帧显示时长，单位ms，没有duration，设置为0 */
    V_BOOL          bIsDiscont;
    E_SME_MEDIA_FRAME_TYPE  eFrameType;                /*OUT *< 帧类型 */
}ST_SME_FRAME_INFO;

typedef struct _tagMediaFrame
{
    ST_SME_FRAME_INFO   stInfo;
    E_SME_MEDIA_TYPE    eType;                          /*OUT *< 流类型*/
	  V_INT32             i32StreamIdx;                   /*OUT 帧所属的流索引  */
    V_UINT32            u32DataLen;                     /*OUT 数据长度*/
    V_UINT8             pu8DataBuf[0];                  /*OUT *< 帧数据地址 */
}ST_SME_MEDIA_FRAME,*PST_SME_MEDIA_FRAME;

typedef struct _tagMediaFrameRd
{
    ST_SME_FRAME_INFO   stInfo;
    E_SME_MEDIA_TYPE    eType;                          /*OUT *< 流类型*/
	V_INT32             i32StreamIdx;                   /*OUT 帧所属的流索引  */
    V_UINT32            u32DataLen;                     /*OUT 数据长度*/
    V_UINT8*            pu8DataBuf;                     /*OUT *< 帧数据地址 */
}ST_SME_MEDIA_FRAME_RD,*PST_SME_MEDIA_FRAME_RD;

typedef struct _tagFrameBuffer
{
    ST_SME_FRAME_ALLOCATOR stAllocator;                     /*可输入，NULL时赋值成默认*/
    PST_SME_MEDIA_FRAME    pstMediaFrame;                   /*输出*/
} ST_SME_FRAMEBUFFER, *PST_SME_FRAMEBUFFER;

typedef struct _tagConfigData
{
    V_UINT32 u32CfgDataSize;
    V_UINT8* pu8CfgData;
}ST_SME_CFG_DATA, *PST_SME_CFG_DATA;

typedef struct _tagVideoRes
{
    V_INT32             i32Width;              /**< 宽度，单位像素 */
    V_INT32             i32Height;             /**< 高度，单位像素 */
}ST_SME_VIDEORES, *PST_SME_VIDEORES;

typedef struct _tagMediaInfo
{
    E_SME_MEDIA_TYPE        eMediaType;
    V_INT32                 i32StreamIdx;                   /**< 流索引 */
    E_SME_MEDIA_CODEC_ID    eCodecId;                       /**< 音频编码格式*/
    V_UINT32                u32CodecForcc;                  /*FORCC,如'MP4V';'DIVX'*/
    V_CHAR                  acMime[M_SME_MIME_LEN_MAX];     /**<MIME类型>*/
    V_UINT32                u32Bps;                         /*bits/s */
    V_INT32                 i32Duration;
    V_INT32                 i32Profile;                     //profile
    V_INT32                 i32Level;                       //level
    V_INT64                 i64FirstPacketPos;
    ST_SME_CFG_DATA         stCfgData;                      /**< 配置数据 */
}ST_SME_MEDIAINFO, *PST_SME_MEDIAINFO;

//audio out
typedef enum _tagSmeAudioRawFmt
{
    E_SME_AUDIO_RAW_FMT_NONE = -1,
    E_SME_AUDIO_RAW_FMT_PCM_8BIT = 0,
    E_SME_AUDIO_RAW_FMT_PCM_S16BIT,
    E_SME_AUDIO_RAW_FMT_PCM_S32BIT,
    E_SME_AUDIO_RAW_FMT_PCM_FLT,
    E_SME_AUDIO_RAW_FMT_PCM_DBL,
    E_SME_AUDIO_RAW_FMT_PCM_8BITP, //planar, data sample is separated by channel
    E_SME_AUDIO_RAW_FMT_PCM_S16BITP,
    E_SME_AUDIO_RAW_FMT_PCM_S32BITP,
    E_SME_AUDIO_RAW_FMT_PCM_FLTP,
    E_SME_AUDIO_RAW_FMT_PCM_DBLP,
#ifdef __DDP_PT__
    E_SME_AUDIO_RAW_FMT_PT_AC3,
    E_SME_AUDIO_RAW_FMT_PT_E_AC3,
#endif
    E_SME_AUDIO_RAW_FMT_PCM_BUTTON
}E_SME_AUDIO_RAW_FMT;

typedef struct _tagParserAudioInfo
{
    ST_SME_MEDIAINFO    stBase;
    V_UINT32            u32Version;                    /**< 音频编码版本，某些音频需要*/
    V_UINT32            u32SampleRate;                 /**< 8000,11025,441000,... */
    E_SME_AUDIO_RAW_FMT eSampleFmt;               /**< 音频每个采样点所占的比特数，8bit,16bit */
    V_UINT32            u32Channels;                   /**< 声道数, 1 or 2 */
    V_UINT32            u32BlockAlign;
    V_CHAR              cLang[M_SME_LANG_MAX_LEN];
    V_UINT32            u32BitsPerCodecSample;     /* wav(adpcm) need it*/
}ST_SME_AUDIOINFO, *PST_SME_AUDIOINFO;

typedef struct _tagParserVideoInfo
{
    ST_SME_MEDIAINFO    stBase;
    ST_SME_VIDEORES     stVideoRes;
    V_INT32             i32FpsNum;                   /**< 帧率,分子*/
    V_INT32             i32FpsDen;                    /**< 帧率,分母*/
    V_UINT16            rotate;                         /* clock wise rotation: [0, 360] */
} ST_SME_VIDEOINFO, *PST_SME_VIDEOINFO;

typedef struct _tagStreamInfo
{
    V_INT64             i64FileSize;
    V_INT32             i32Duration;
    V_UINT32            u32BitRate;
    V_UINT32            u32NumStream;
    V_UINT32            u32IdxOrderInPos[M_SME_STREAM_NUM_MAX];
    ST_SME_MEDIAINFO*   pstMedia[M_SME_STREAM_NUM_MAX];
}ST_SME_STREAMINFO, *PST_SME_STREAMINFO;

typedef enum _tagSmeAudioStreamType
{
    E_SME_AUDIO_STREAM_TYPE_MUSIC = 3,
}E_SME_AUDIO_STREAM_TYPE;

//dec out
typedef struct _tagAudioRawFrame
{
    ST_SME_FRAME_INFO       stInfo;
    E_SME_AUDIO_RAW_FMT     eFmt;               /*OUT *< 流类型*/
    V_UINT32                u32DataLen;         /*OUT 数据长度*/
    V_UINT8                 pu8DataBuf[0];      /*OUT *< 帧数据地址 */
}ST_SME_ARAW_FRAME, *PST_SME_ARAW_FRAME;

typedef struct _tagAudioRawFrameBuffer
{
    ST_SME_FRAME_ALLOCATOR  stAllocator;
    PST_SME_ARAW_FRAME       pstARawFrame;
}ST_SME_ARAW_FRAMEBUFFER, *PST_SME_ARAW_FRAMEBUFFER;


#define M_SME_VIDEO_FRAME_PLANE_MAX 4
typedef enum _tagSmeVideoRawFmt
{
    E_SME_VIDEO_RAW_FMT_NONE = -1,
    E_SME_VIDEO_RAW_FMT_YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    E_SME_VIDEO_RAW_FMT_YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    E_SME_VIDEO_RAW_FMT_RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
    E_SME_VIDEO_RAW_FMT_BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
    E_SME_VIDEO_RAW_FMT_YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    E_SME_VIDEO_RAW_FMT_YUV444P,   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    E_SME_VIDEO_RAW_FMT_YUV410P,   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
    E_SME_VIDEO_RAW_FMT_YUV411P,   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
    E_SME_VIDEO_RAW_FMT_GRAY8,     ///<        Y        ,  8bpp
    E_SME_VIDEO_RAW_FMT_MONOWHITE, ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
    E_SME_VIDEO_RAW_FMT_MONOBLACK, ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
    E_SME_VIDEO_RAW_FMT_PAL8,      ///< 8 bit with E_SME_VIDEO_RAW_FMT_RGB32 palette
    E_SME_VIDEO_RAW_FMT_YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of E_SME_VIDEO_RAW_FMT_YUV420P and setting color_range
    E_SME_VIDEO_RAW_FMT_YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of E_SME_VIDEO_RAW_FMT_YUV422P and setting color_range
    E_SME_VIDEO_RAW_FMT_YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of E_SME_VIDEO_RAW_FMT_YUV444P and setting color_range
    E_SME_VIDEO_RAW_FMT_XVMC_MPEG2_MC,///< XVideo Motion Acceleration via common packet passing
    E_SME_VIDEO_RAW_FMT_XVMC_MPEG2_IDCT,
    E_SME_VIDEO_RAW_FMT_UYVY422,   ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
    E_SME_VIDEO_RAW_FMT_UYYVYY411, ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
    E_SME_VIDEO_RAW_FMT_BGR8,      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
    E_SME_VIDEO_RAW_FMT_BGR4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    E_SME_VIDEO_RAW_FMT_BGR4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
    E_SME_VIDEO_RAW_FMT_RGB8,      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
    E_SME_VIDEO_RAW_FMT_RGB4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
    E_SME_VIDEO_RAW_FMT_RGB4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)
    E_SME_VIDEO_RAW_FMT_NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
    E_SME_VIDEO_RAW_FMT_NV21,      ///< as above, but U and V bytes are swapped

    E_SME_VIDEO_RAW_FMT_ARGB,      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
    E_SME_VIDEO_RAW_FMT_RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
    E_SME_VIDEO_RAW_FMT_ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
    E_SME_VIDEO_RAW_FMT_BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

    E_SME_VIDEO_RAW_FMT_GRAY16BE,  ///<        Y        , 16bpp, big-endian
    E_SME_VIDEO_RAW_FMT_GRAY16LE,  ///<        Y        , 16bpp, little-endian
    E_SME_VIDEO_RAW_FMT_YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
    E_SME_VIDEO_RAW_FMT_YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of E_SME_VIDEO_RAW_FMT_YUV440P and setting color_range
    E_SME_VIDEO_RAW_FMT_YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
    E_SME_VIDEO_RAW_FMT_VDPAU_H264,///< H.264 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VDPAU_MPEG1,///< MPEG-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VDPAU_MPEG2,///< MPEG-2 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VDPAU_WMV3,///< WMV3 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VDPAU_VC1, ///< VC-1 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_RGB48BE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
    E_SME_VIDEO_RAW_FMT_RGB48LE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian

    E_SME_VIDEO_RAW_FMT_RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
    E_SME_VIDEO_RAW_FMT_RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
    E_SME_VIDEO_RAW_FMT_RGB555BE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0
    E_SME_VIDEO_RAW_FMT_RGB555LE,  ///< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0

    E_SME_VIDEO_RAW_FMT_BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
    E_SME_VIDEO_RAW_FMT_BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
    E_SME_VIDEO_RAW_FMT_BGR555BE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1
    E_SME_VIDEO_RAW_FMT_BGR555LE,  ///< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1

    E_SME_VIDEO_RAW_FMT_VAAPI_MOCO, ///< HW acceleration through VA API at motion compensation entry-point, Picture.data[3] contains a vaapi_render_state struct which contains macroblocks as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VAAPI_IDCT, ///< HW acceleration through VA API at IDCT entry-point, Picture.data[3] contains a vaapi_render_state struct which contains fields extracted from headers
    E_SME_VIDEO_RAW_FMT_VAAPI_VLD,  ///< HW decoding through VA API, Picture.data[3] contains a vaapi_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers

    E_SME_VIDEO_RAW_FMT_YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV422P16LE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV422P16BE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV444P16LE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV444P16BE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_VDPAU_MPEG4,  ///< MPEG4 HW decoding with VDPAU, data[0] contains a vdpau_render_state struct which contains the bitstream of the slices as well as various fields extracted from headers
    E_SME_VIDEO_RAW_FMT_DXVA2_VLD,    ///< HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer

    E_SME_VIDEO_RAW_FMT_RGB444LE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), little-endian, most significant bits to 0
    E_SME_VIDEO_RAW_FMT_RGB444BE,  ///< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), big-endian, most significant bits to 0
    E_SME_VIDEO_RAW_FMT_BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), little-endian, most significant bits to 1
    E_SME_VIDEO_RAW_FMT_BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), big-endian, most significant bits to 1
    E_SME_VIDEO_RAW_FMT_Y400A,     ///< 8bit gray, 8bit alpha
    E_SME_VIDEO_RAW_FMT_BGR48BE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
    E_SME_VIDEO_RAW_FMT_BGR48LE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian
    E_SME_VIDEO_RAW_FMT_YUV420P9BE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV420P9LE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV420P10BE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV420P10LE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV422P10BE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV422P10LE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV444P9BE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV444P9LE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV444P10BE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV444P10LE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_YUV422P9BE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
    E_SME_VIDEO_RAW_FMT_YUV422P9LE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
    E_SME_VIDEO_RAW_FMT_VDA_VLD,    ///< hardware decoding through VDA
    E_SME_VIDEO_RAW_FMT_GBRP,      ///< planar GBR 4:4:4 24bpp
    E_SME_VIDEO_RAW_FMT_GBRP9BE,   ///< planar GBR 4:4:4 27bpp, big endian
    E_SME_VIDEO_RAW_FMT_GBRP9LE,   ///< planar GBR 4:4:4 27bpp, little endian
    E_SME_VIDEO_RAW_FMT_GBRP10BE,  ///< planar GBR 4:4:4 30bpp, big endian
    E_SME_VIDEO_RAW_FMT_GBRP10LE,  ///< planar GBR 4:4:4 30bpp, little endian
    E_SME_VIDEO_RAW_FMT_GBRP16BE,  ///< planar GBR 4:4:4 48bpp, big endian
    E_SME_VIDEO_RAW_FMT_GBRP16LE,  ///< planar GBR 4:4:4 48bpp, little endian
    E_SME_VIDEO_RAW_FMT_BUTTON,
}E_SME_VIDEO_RAW_FMT;

typedef struct _tagVideoRawFrameBuffer
{
    V_UINT32                u32DataLen;
    V_UINT8*                pu8Data;
    V_UINT8*                pu8PhyPlane[M_SME_VIDEO_FRAME_PLANE_MAX];
    V_UINT8*                pu8VirPlane[M_SME_VIDEO_FRAME_PLANE_MAX];
    V_UINT32                u32Stride[M_SME_VIDEO_FRAME_PLANE_MAX];
}ST_SME_VRAW_FRAMEBUFFER;

typedef struct _tagVideoRawFrame
{
    ST_SME_FRAME_INFO       stInfo;
    E_SME_VIDEO_RAW_FMT     eFmt;               /*OUT *< 流类型*/
    ST_SME_VIDEORES         stRes;
    ST_SME_VRAW_FRAMEBUFFER stBuffer;
}ST_SME_VRAW_FRAME;

typedef struct _tagSmeDispRect
{
    V_INT32 i32StartX;
    V_INT32 i32StartY;
    V_INT32 i32Width;
    V_INT32 i32Height;
}ST_SME_DISP_RECT, *PST_SME_DISP_RECT;

typedef V_VOID (*pfnLogCb)(V_VOID* pvCtx,
    IN const V_CHAR* pszTag,
    IN V_INT32 i32Level,
    IN const V_CHAR* pszFunc,
    IN const V_CHAR* pszFile,
    IN V_UINT32 u32Line,
    IN const V_CHAR* pszFmt,
    IN va_list stValist);

typedef struct _tagLogCB
{
    pfnLogCb pfCb;
    V_VOID* pvCtx;
}ST_SME_LOG_CB,*PST_SME_LOG_CB;


typedef struct _tagMediaCodecIdentify
{
    E_SME_MEDIA_CODEC_ID    eCodecId;                       /**< 音视频编码格式*/
    V_UINT32                u32CodecForcc;                  /*FORCC,如'MP4V';'DIVX'*/
	V_CHAR                  acMime[M_SME_MIME_LEN_MAX];     /**<MIME类型>*/
}ST_SME_MEDIACODEC_IDTF, *PST_SME_MEDIACODEC_IDTF;


//sps or pps
typedef struct _tagAvcNalCfgItem
{
    V_UINT32 u32Len;
    V_UINT8 au8SpsOrPps[0];
}ST_AVCNALCFG_ITEM, *PST_AVCNALCFG_ITEM;

typedef struct _tagAvcNalCfgList
{
    V_UINT32 u32Count;
    PST_AVCNALCFG_ITEM pstItem[M_SME_AVC_CFG_ITEM_MAX];//每个Item都要独立释放
}ST_AVCNALCFG_LIST, *PST_AVCNALCFG_LIST;

//bla or sps pps
typedef struct _tagHevcNalCfgItem
{
    V_UINT32 u32Len;
    V_UINT8 au8Cfgs[0];
}ST_HEVCNALCFG_ITEM, *PST_HEVCNALCFG_ITEM;

typedef struct _tagHevcNalCfgList
{
    V_UINT32 u32Count;
    PST_HEVCNALCFG_ITEM pstItem[M_SME_HEVC_CFG_ITEM_MAX];//每个Item都要独立释放
}ST_HEVCNALCFG_LIST, *PST_HEVCNALCFG_LIST;

// copy from tvos_hal_aout
typedef enum _A_TRACK_MODE_E
{
    A_TRACK_MODE_STEREO = 0,          /**<Stereo*/ /**<CNcomment: 立体声*/
    A_TRACK_MODE_DOUBLE_MONO,         /**<Data is output after being mixed in the audio-left channel and audio-right channel.*/ /**<CNcomment: 左右声道混合后输出*/
    A_TRACK_MODE_DOUBLE_LEFT,         /**<The audio-left channel and audio-right channel output the data of the audio-left channel.*/ /**<CNcomment: 左右声道输出左声道数据*/
    A_TRACK_MODE_DOUBLE_RIGHT,        /**<The audio-left channel and audio-right channel output the data of the audio-right channel.*/ /**<CNcomment: 左右声道输出右声道数据*/
    A_TRACK_MODE_EXCHANGE,            /**<Data is output after being exchanged in the audio-left channel and audio-right channel.*/ /**<CNcomment: 左右声道数据交换输出 */
    A_TRACK_MODE_ONLY_RIGHT,          /**<Only the data in the audio-right channel is output.*/ /**<CNcomment: 只输出右声道数据*/
    A_TRACK_MODE_ONLY_LEFT,           /**<Only the data in the audio-left channel is output.*/ /**<CNcomment: 只输出左声道数据*/
    A_TRACK_MODE_MUTED,               /**<Mute*/ /**<CNcomment: 静音*/
    A_TRACK_MODE_BUTT
} A_TRACK_CHANNEL_MODE_E;


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SME_MEDIA_TYPE_H__
