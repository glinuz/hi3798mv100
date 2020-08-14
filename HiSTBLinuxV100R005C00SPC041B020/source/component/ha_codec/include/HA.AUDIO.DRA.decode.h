/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.DRA.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_DRA_H__
#define __HISI_AUDIO_DECODER_DRA_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define    HA_DRA_ID 0x007        

#define HA_CODEC_GET_DRA_DonwmixMultich_MODE_CMD  ((((HI_U32)HA_DRA_ID) << 16) | 0x1000)
#define HA_CODEC_GET_DRA_RAWCHANNEL_MODE_CMD      ((((HI_U32)HA_DRA_ID) << 16) | 0x1001)

typedef struct
{
    HI_U32  enCmd;              /* HA_DRA_GET_DonwmixMultich_MODE_S */
    HI_BOOL  *pbDonwmixMultichMode;
} HA_DRA_GET_DonwmixMultich_MODE_S;

typedef enum
{
    DRA_RAWCHANNEL_MODE10 = 0,
    DRA_RAWCHANNEL_MODE11,
    DRA_RAWCHANNEL_MODE20,
    DRA_RAWCHANNEL_MODE21,
    DRA_RAWCHANNEL_MODE30,
    DRA_RAWCHANNEL_MODE31,
    DRA_RAWCHANNEL_MODE40,
    DRA_RAWCHANNEL_MODE41,
    DRA_RAWCHANNEL_MODE50,
    DRA_RAWCHANNEL_MODE51,
    DRA_RAWCHANNEL_BUTT = 0x100,   /* Unsupported Channel Mode */
} HA_DRA_RAWCHANNEL_MODE;

typedef struct
{
    HI_U32  enCmd;              /* HA_CODEC_GET_DRA_RAWCHANNEL_MODE_CMD */
    HA_DRA_RAWCHANNEL_MODE  eRawChannelMode;
} HA_DRA_GET_RawChannel_MODE_S;

#define HA_DRA_DecGetDefalutOpenParam(pOpenParam) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_SIMUL; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = HI_NULL; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = 0; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 24; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
}while(0)

#define HA_DRA_DecGetOpenParam_MultichPcm(pOpenParam) \
    do{ HI_S32 i; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_SIMUL; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = HI_NULL; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = 0; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 8; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 24; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
        for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
        { \
            ((HI_HADECODE_OPENPARAM_S *)pOpenParam)->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
        } \
    }while(0)
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_DRA_H__ */

