/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.FFMPEG_WMAPRO.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_FFMPEG_WMAPRO_H__
#define __HISI_AUDIO_DECODER_FFMPEG_WMAPRO_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define HA_WMAPRO_ID 0x041f
#define HA_AUDIO_ID_FFMPEG_WMAPRO   HA_BUILD_CODEC_ID(VENDOR_HISI,FORMAT_WMAPRO,HA_WMAPRO_ID)  /* support FORMAT_WMAPRO */


typedef struct hiWMAPro_FORMAT_S
{
    HI_VOID *               hAvCtx; /* Same as libavcodec AVCodecContext, Set/allocated/freed by user. */

    HI_U16 wFormatTag;          /* format type,0x160->WMAV1,0x161->WMAV2, 0x162->WMAV3 */
    HI_U16 nChannels;            /* number of channels (i.e. mono, stereo...) */
    HI_U32 nSamplesPerSec;   /* sample rate */
    HI_U32 nAvgBytesPerSec;  /* for buffer estimation */
    HI_U16 nBlockAlign;          /* block size of data */
    HI_U16 wBitsPerSample;   /* number of bits per sample of mono data */
    HI_U16 cbSize;                /* the count in bytes of the size of */
    HI_U8 cbExtWord[32];       /* extra information (after cbSize).
                                WMAV1: need  4 Bytes extra information at least
                                    WMAV2: need 10 Bytes extra information at least
                                WMAV3: need 18 Bytes extra information at least
                                 */
} WMAPro_FORMAT_S;


#define HA_FFMPEGC_WMAPROC_DecGetDefalutOpenParam(pOpenParam, pstWmaProConfig) \
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
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = pstWmaProConfig; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(WMAPro_FORMAT_S); \
    } while (0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_FFMPEG_WMAPRO_H__ */
