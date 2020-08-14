/**
 \file
 \brief Codec type. CNcomment:Codec 类型CNend
 \author HiSilicon Technologies Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author
 \date 2010-02-10
 */

#ifndef __HI_SVR_CODEC_H__
#define __HI_SVR_CODEC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Structs Definition ******************************/
/** \addtogroup     Hiplayer */
/** @{ */  /** <!--[Hiplayer] */

/** Subtitle data type */
/** CNcomment:字幕数据类型 */
typedef enum hiFORMAT_SUBTITLE_TYPE_E
{
    HI_FORMAT_SUBTITLE_ASS = 0x0,    /**< ASS subtitle *//**< CNcomment:ASS字幕*/
    HI_FORMAT_SUBTITLE_LRC,       /**< LRC subtitle *//**< CNcomment:LRC字幕*/
    HI_FORMAT_SUBTITLE_SRT,       /**< SRT subtitle *//**< CNcomment:SRT字幕*/
    HI_FORMAT_SUBTITLE_SMI,       /**< SMI subtitle *//**< CNcomment:SMI字幕*/
    HI_FORMAT_SUBTITLE_SUB,       /**< SUB subtitle *//**< CNcomment:SUB字幕*/
    HI_FORMAT_SUBTITLE_TXT,       /**< RAW UTF8 subtitle *//**< CNcomment:RAW UTF8字幕*/

    HI_FORMAT_SUBTITLE_HDMV_PGS,      /**< pgs subtitle *//**< CNcomment:pgs字幕 */
    HI_FORMAT_SUBTITLE_DVB_SUB,       /**< DVB subtitle *//**< CNcomment:DVBsub字幕 */
    HI_FORMAT_SUBTITLE_DVD_SUB,       /**< DVD subtitle *//**< CNcomment:DVDsub字幕 */
    HI_FORMAT_SUBTITLE_TTML,          /**< TTML subtitle *//**< CNcomment:TTML字幕 */
    HI_FORMAT_SUBTITLE_WEBVTT,
    HI_FORMAT_SUBTITLE_BUTT
} HI_FORMAT_SUBTITLE_TYPE_E;

/** Audio data type */
/** CNcomment:音频数据类型 */
typedef enum hiFORMAT_AUDIO_TYPE_E
{
    HI_FORMAT_AUDIO_MP2 = 0x000,  /**< MPEG audio layer 1, 2.*//**<CNcomment:MPEG音频第一层、第二层 */
    HI_FORMAT_AUDIO_MP3,          /**< MPEG audio layer 1, 2, 3.*//**<CNcomment:MPEG音频第一层、第二层 、第三层 */
    HI_FORMAT_AUDIO_AAC,
    HI_FORMAT_AUDIO_AC3,
    HI_FORMAT_AUDIO_DTS,
    HI_FORMAT_AUDIO_VORBIS,
    HI_FORMAT_AUDIO_DVAUDIO,
    HI_FORMAT_AUDIO_WMAV1,
    HI_FORMAT_AUDIO_WMAV2,
    HI_FORMAT_AUDIO_MACE3,
    HI_FORMAT_AUDIO_MACE6,
    HI_FORMAT_AUDIO_VMDAUDIO,
    HI_FORMAT_AUDIO_SONIC,
    HI_FORMAT_AUDIO_SONIC_LS,
    HI_FORMAT_AUDIO_FLAC,
    HI_FORMAT_AUDIO_MP3ADU,
    HI_FORMAT_AUDIO_MP3ON4,
    HI_FORMAT_AUDIO_SHORTEN,
    HI_FORMAT_AUDIO_ALAC,
    HI_FORMAT_AUDIO_WESTWOOD_SND1,
    HI_FORMAT_AUDIO_GSM,
    HI_FORMAT_AUDIO_QDM2,
    HI_FORMAT_AUDIO_COOK,
    HI_FORMAT_AUDIO_TRUESPEECH,
    HI_FORMAT_AUDIO_TTA,
    HI_FORMAT_AUDIO_SMACKAUDIO,
    HI_FORMAT_AUDIO_QCELP,
    HI_FORMAT_AUDIO_WAVPACK,
    HI_FORMAT_AUDIO_DSICINAUDIO,
    HI_FORMAT_AUDIO_IMC,
    HI_FORMAT_AUDIO_MUSEPACK7,
    HI_FORMAT_AUDIO_MLP,
    HI_FORMAT_AUDIO_GSM_MS, /**< as found in WAV.*//**<CNcomment:存在WAV格式中 */
    HI_FORMAT_AUDIO_ATRAC3,
    HI_FORMAT_AUDIO_VOXWARE,
    HI_FORMAT_AUDIO_APE,
    HI_FORMAT_AUDIO_NELLYMOSER,
    HI_FORMAT_AUDIO_MUSEPACK8,
    HI_FORMAT_AUDIO_SPEEX,
    HI_FORMAT_AUDIO_WMAVOICE,
    HI_FORMAT_AUDIO_WMAPRO,
    HI_FORMAT_AUDIO_WMALOSSLESS,
    HI_FORMAT_AUDIO_ATRAC3P,
    HI_FORMAT_AUDIO_EAC3,
    HI_FORMAT_AUDIO_SIPR,
    HI_FORMAT_AUDIO_MP1,
    HI_FORMAT_AUDIO_TWINVQ,
    HI_FORMAT_AUDIO_TRUEHD,
    HI_FORMAT_AUDIO_MP4ALS,
    HI_FORMAT_AUDIO_ATRAC1,
    HI_FORMAT_AUDIO_BINKAUDIO_RDFT,
    HI_FORMAT_AUDIO_BINKAUDIO_DCT,
    HI_FORMAT_AUDIO_DRA,
    HI_FORMAT_AUDIO_DTS_EXPRESS,

    HI_FORMAT_AUDIO_PCM = 0x100,   /**< various PCM codecs. *//**<CNcomment:PCM格式 */
    HI_FORMAT_AUDIO_PCM_BLURAY = 0x121,

    HI_FORMAT_AUDIO_ADPCM = 0x130, /**< various ADPCM codecs. *//**<CNcomment:ADPCM格式 */

    HI_FORMAT_AUDIO_AMR_NB = 0x160,/**< various AMR codecs. *//**<CNcomment:AMR格式 */
    HI_FORMAT_AUDIO_AMR_WB,
    HI_FORMAT_AUDIO_AMR_AWB,

    HI_FORMAT_AUDIO_RA_144 = 0x170, /**< RealAudio codecs. *//**<CNcomment:RealAudio格式 */
    HI_FORMAT_AUDIO_RA_288,

    HI_FORMAT_AUDIO_DPCM = 0x180, /**< various DPCM codecs. *//**<CNcomment:DPCM格式 */

    HI_FORMAT_AUDIO_G711 = 0x190, /**< various G.7xx codecs. *//**<CNcomment:G.7xx格式 */
    HI_FORMAT_AUDIO_G722,
    HI_FORMAT_AUDIO_G7231,
    HI_FORMAT_AUDIO_G726,
    HI_FORMAT_AUDIO_G728,
    HI_FORMAT_AUDIO_G729AB,

    HI_FORMAT_AUDIO_MULTI = 0x1f0, /**< support multi codecs. *//**<CNcomment:多种格式 */

    HI_FORMAT_AUDIO_BUTT = 0x1ff,
} HI_FORMAT_AUDIO_TYPE_E;

/** Video data type */
/** CNcomment:视频数据类型 */
typedef enum hiFORMAT_VIDEO_TYPE_E
{
    HI_FORMAT_VIDEO_MPEG2 = 0x0, /**< MPEG2*/
    HI_FORMAT_VIDEO_MPEG4,       /**< MPEG4 DIVX4 DIVX5*/
    HI_FORMAT_VIDEO_AVS,         /**< AVS*/
    HI_FORMAT_VIDEO_H263,        /**< H263*/
    HI_FORMAT_VIDEO_H264,        /**< H264*/
    HI_FORMAT_VIDEO_REAL8,       /**< REAL*/
    HI_FORMAT_VIDEO_REAL9,       /**< REAL*/
    HI_FORMAT_VIDEO_VC1,         /**< VC-1*/
    HI_FORMAT_VIDEO_VP6,         /**< VP6*/
    HI_FORMAT_VIDEO_VP6F,        /**< VP6F*/
    HI_FORMAT_VIDEO_VP6A,        /**< VP6A*/
    HI_FORMAT_VIDEO_MJPEG,       /**< MJPEG*/
    HI_FORMAT_VIDEO_SORENSON,    /**< SORENSON SPARK*/
    HI_FORMAT_VIDEO_DIVX3,       /**< DIVX3, not supported*/
    HI_FORMAT_VIDEO_RAW,         /**< RAW*/
    HI_FORMAT_VIDEO_JPEG,        /**< JPEG added for VENC*/
    HI_FORMAT_VIDEO_VP8,         /**<VP8*/
    HI_FORMAT_VIDEO_MSMPEG4V1,   /**< MS private MPEG4 */
    HI_FORMAT_VIDEO_MSMPEG4V2,
    HI_FORMAT_VIDEO_MSVIDEO1,    /**< MS video */
    HI_FORMAT_VIDEO_WMV1,
    HI_FORMAT_VIDEO_WMV2,
    HI_FORMAT_VIDEO_RV10,
    HI_FORMAT_VIDEO_RV20,
    HI_FORMAT_VIDEO_SVQ1,        /**< Apple video */
    HI_FORMAT_VIDEO_SVQ3,        /**< Apple video */
    HI_FORMAT_VIDEO_H261,
    HI_FORMAT_VIDEO_VP3,
    HI_FORMAT_VIDEO_VP5,
    HI_FORMAT_VIDEO_CINEPAK,
    HI_FORMAT_VIDEO_INDEO2,
    HI_FORMAT_VIDEO_INDEO3,
    HI_FORMAT_VIDEO_INDEO4,
    HI_FORMAT_VIDEO_INDEO5,
    HI_FORMAT_VIDEO_MJPEGB,
    HI_FORMAT_VIDEO_MVC,
    HI_FORMAT_VIDEO_HEVC,        /**< HEVC(H265)*/
    HI_FORMAT_VIDEO_DV,
    HI_FORMAT_VIDEO_HUFFYUV,
    HI_FORMAT_VIDEO_DIVX,           /**< DIVX,not supported*/
    HI_FORMAT_VIDEO_REALMAGICMPEG4, /**< REALMAGIC MPEG4,not supported*/
    HI_FORMAT_VIDEO_VP9,         /**<VP9*/
    HI_FORMAT_VIDEO_WMV3,
    HI_FORMAT_VIDEO_BUTT
} HI_FORMAT_VIDEO_TYPE_E;


/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* #ifndef __HI_SVR_CODEC_H__ */
