/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.G722.codec.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_CODEC_G722_H__
#define __HISI_AUDIO_CODEC_G722_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define HA_G722_ID 0x0105        

#define G722_FRAME_BLOCK_LEN_10MS  160
#define G722_FRAME_BLOCK_LEN_20MS  320

#define HA_G722_GetEncDefaultOpenParam(pOpenParam) \
do{ ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredOutChannels = 1; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->bInterleaved = HI_TRUE; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->s32BitPerSample = 16; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredSampleRate = 16000; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32SamplePerFrame = G722_FRAME_BLOCK_LEN_10MS; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = HI_NULL; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = 0; \
}while(0)

typedef struct
{
    HI_S32  mode;  /* 1=64kbps, 2= 56kbps, 3= 48kbps  */
} G722_DECODE_OPENCONFIG_S;

#define HA_G722_GetDecDefalutOpenParam(pOpenParam, pstPrivateParams) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 1; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 16000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(G722_DECODE_OPENCONFIG_S); \
}while(0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


#endif /* __HISI_AUDIO_CODEC_G722_H__ */
