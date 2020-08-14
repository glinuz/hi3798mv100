/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.FFMPEG_DECODE.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_FFMPEG_DECODE_H__
#define __HISI_AUDIO_DECODER_FFMPEG_DECODE_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define HA_FFMPEG_ID 0x03ff        

/**define status parameter of update information*/ /**CNcomment:状态信息更新，事件回调用户数据定义*/
typedef struct  hiFFMPEG_STATUS_INFO_S
{
    HI_CHAR *name;
    HI_S32  codec_id;
    HI_S32  codec_type;
    HI_S32  sample_fmt;
    HI_S32  channels;
    HI_S32  sample_rate;
    HI_S32  bit_rate;
    HI_S32  block_align;
} FFMPEG_STATUS_INFO_S;


/** Audio stream information */
/** CNcomment:音频流信息 */
typedef struct hiFFMPEG_AUD_INFO_S
{
    HI_S32   codec_id;
    HI_S32   s32StreamIndex;
    HI_U32   u32Format;
    HI_U32   u32Profile;
    HI_U32   u32SampleRate;
    HI_U16   u16BitPerSample;
    HI_U16   u16Channels;
    HI_U32   u32BlockAlign;
    HI_U32   u32Bitrate;
    HI_BOOL  bBigEndian;
    HI_U32   u32ExtradataSize;
    HI_U8   *pu8Extradata;
} FFMPEG_AUD_INFO_S;

/**define the type of callback function*/ /**CNcomment:事件回调函数类型*/
typedef enum
{
    HA_FFMPEG_EVENT_STATUS_CHANGE = 0,                   /**<update status information by every frame*/ /**<CNcomment:状态信息更新, 每帧更新一次*/
    HA_FFMPEG_EVENT_BUTT
} HA_FFMPEG_EVENT_E;

/**define callback function*/ /**CNcomment:定义事件回调函数枚举类型*/
typedef HI_VOID (*FFMPEG_EVENT_CB_FN)(HI_VOID* pAppData1, HI_VOID* pAppData2, HA_FFMPEG_EVENT_E enEvent);

typedef struct  hiHA_FFMPEG_DECODE_OPENCONFIG_S
{
    HI_VOID *               hAvCtx; /* Same as libavcodec AVCodecContext, Set/allocated/freed by user. */
    HI_BOOL                 bAutoDmxStereo; /* automatic downmix multi-ch to stereo. */
    HI_BOOL                 bConvert2fmt16bit; /* automatic convert none-16bit pcm to 16bit pcm. */
    FFMPEG_EVENT_CB_FN      pfnEvtCbFunc[HA_FFMPEG_EVENT_BUTT];    /* call back method */
    HI_VOID*                pAppData1[HA_FFMPEG_EVENT_BUTT];       /* application handle,   Set/allocated/freed by user. */
    HI_VOID*                pAppData2[HA_FFMPEG_EVENT_BUTT];       /* application defined value for call back method,  allocated/freed by user, set by ha_codec,*/
    FFMPEG_AUD_INFO_S       stFfmpeg_AudInfo;
} HA_FFMPEG_DECODE_OPENCONFIG_S;

#define HA_FFMPEG_DecGetDefalutOpenConfig(pConfigParam) \
    do { \
        ((HA_FFMPEG_DECODE_OPENCONFIG_S *)(pConfigParam))->bAutoDmxStereo = HI_TRUE; \
        ((HA_FFMPEG_DECODE_OPENCONFIG_S *)(pConfigParam))->bConvert2fmt16bit = HI_TRUE; \
    } while (0)

#define HA_FFMPEGC_DecGetDefalutOpenParam(pOpenParam, pFfmpegConfig) \
    do { HI_S32 i; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved = HI_TRUE; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
         for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
         { \
             ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
         } \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = pFfmpegConfig; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(HA_FFMPEG_DECODE_OPENCONFIG_S); \
         for (i = 0; i < HA_FFMPEG_EVENT_BUTT; i++) \
         { \
             ((HA_FFMPEG_DECODE_OPENCONFIG_S *)(pFfmpegConfig))->pfnEvtCbFunc[i] = HI_NULL; \
             ((HA_FFMPEG_DECODE_OPENCONFIG_S *)(pFfmpegConfig))->pAppData1[i] = HI_NULL; \
             ((HA_FFMPEG_DECODE_OPENCONFIG_S *)(pFfmpegConfig))->pAppData2[i] = HI_NULL; \
         } \
    } while (0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_FFMPEG_DECODE_H__ */
