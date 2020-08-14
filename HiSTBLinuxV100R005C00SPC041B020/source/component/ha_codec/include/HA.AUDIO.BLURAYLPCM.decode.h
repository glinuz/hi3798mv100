/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.BLURAYLPCM.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_BLURAYLPCM_H__
#define __HISI_AUDIO_DECODER_BLURAYLPCM_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HA_BLYRAYLPCM_ID 0x021        

typedef struct  hiHA_BLURAYLPCM_DECODE_OPENCONFIG_S
{
    HI_BOOL                 bAutoDmxStereo; /* automatic downmix multi-ch to stereo. */
    HI_BOOL                 bConvert2fmt16bit; /* automatic convert none-16bit pcm to 16bit pcm. */
} HA_BLURAYLPCM_DECODE_OPENCONFIG_S;

#define HA_BLYRAYLPCM_DecGetDefalutOpenConfig(pConfigParam) \
    do { \
        ((HA_BLURAYLPCM_DECODE_OPENCONFIG_S *)(pConfigParam))->bAutoDmxStereo = HI_TRUE; \
        ((HA_BLURAYLPCM_DECODE_OPENCONFIG_S *)(pConfigParam))->bConvert2fmt16bit = HI_TRUE; \
    } while (0)


#define HA_BLYRAYLPCM_DecGetDefalutOpenParam(pOpenParam, pstPrivateParams) \
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
    ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->u32CodecPrivateDataSize = sizeof(HA_BLURAYLPCM_DECODE_OPENCONFIG_S); \
}while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_BLURAYLPCM_H__ */

