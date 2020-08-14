/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.PCM.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_PCM_H__
#define __HISI_AUDIO_DECODER_PCM_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HA_PCM_ID 0x0000        

typedef struct hiWAV_FORMAT_S
{
    HI_U16 wFormatTag;          /* format category */
    HI_U16 nChannels;            /* number of channels (i.e. mono, stereo...) */
    HI_U32 nSamplesPerSec;   /* sample rate */
    HI_U32 nAvgBytesPerSec;  /* for buffer estimation */
    HI_U16 nBlockAlign;          /* the block alignment (in bytes) of the
                                      waveform data */
    HI_U16 wBitsPerSample;   /* number of bits per sample of mono data */
    HI_U16 cbSize;                /* number of bytes of wave raw data cbExtWord*/
    HI_U16 cbExtWord[16];       /* extra information (after cbSize).                      */
   /* note: big-endian pcm supprt(microsoft wav file only support little-endian pcm format):
        cbSize = 4;
        cbExtWord[0] = 1;
        cbExtWord[1] ; //reserved
   */
} WAV_FORMAT_S;

#define HA_PCM_DecGetDefalutOpenParam(pOpenParam, pstPrivateParams) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
    ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->u32CodecPrivateDataSize = sizeof(WAV_FORMAT_S); \
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_PCM_H__ */

