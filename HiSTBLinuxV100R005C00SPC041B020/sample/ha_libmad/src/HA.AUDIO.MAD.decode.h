/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.MAD.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_MAD_H__
#define __HISI_AUDIO_DECODER_MAD_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HA_AUDIO_ID_MAD  0x10001001

static HI_S32 HA_MAD_DecGetDefalutOpenParam(HI_HADECODE_OPENPARAM_S * pOpenParam)
{
    HI_S32 i;

    if (!pOpenParam)
    {
        return HA_ErrorInvalidParameter;
    }

    pOpenParam->enDecMode = HD_DEC_MODE_RAWPCM;
    pOpenParam->pCodecPrivateData = HI_NULL;
    pOpenParam->u32CodecPrivateDataSize = 0;
    pOpenParam->sPcmformat.u32DesiredOutChannels = 2;
    pOpenParam->sPcmformat.bInterleaved  = HI_TRUE;
    pOpenParam->sPcmformat.u32BitPerSample = 16;
    pOpenParam->sPcmformat.u32DesiredSampleRate = 48000;
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++)
    {
        pOpenParam->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone;
    }
    return HA_ErrorNone;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_MAD_H__ */

