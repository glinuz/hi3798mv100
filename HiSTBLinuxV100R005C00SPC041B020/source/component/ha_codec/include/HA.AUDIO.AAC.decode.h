/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
 File Name     : HA.AUDIO.AAC.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       : 
 Last Modified :
 Description   : 
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_AAC_H__
#define __HISI_AUDIO_DECODER_AAC_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define    HA_AAC_ID 0x001         

#define HA_CODEC_AAC_SET_TRANSPORT_TYPE_CMD  ((((HI_U32)HA_AAC_ID) << 16) | 0x1000)
#define HA_CODEC_AAC_SET_CONFIGRAW_CMD       ((((HI_U32)HA_AAC_ID) << 16) | 0x1001)
#define FILEREAD_MAX_LAYERS 2

/**
 * Transport type identifiers.
 */
typedef enum
{
  HA_TT_UNKNOWN           = -1, /**< Unknown format.            */
  HA_TT_MP4_RAW           = 0,  /**< "as is" access units (packet based since there is obviously no sync layer) */
  HA_TT_MP4_ADIF          = 1,  /**< ADIF bitstream format.     */
  HA_TT_MP4_ADTS          = 2,  /**< ADTS bitstream format.     */

  HA_TT_MP4_LATM_MCP1     = 6,  /**< Audio Mux Elements with muxConfigPresent = 1 */
  HA_TT_MP4_LATM_MCP0     = 7,  /**< Audio Mux Elements with muxConfigPresent = 0, out of band StreamMuxConfig */

  HA_TT_MP4_LOAS          = 10, /**< Audio Sync Stream.         */

  HA_TT_DRM               = 12, /**< Digital Radio Mondial (DRM30/DRM+) bitstream format. */

  HA_TT_MP1_L1            = 16, /**< MPEG 1 Audio Layer 1 audio bitstream. */
  HA_TT_MP1_L2            = 17, /**< MPEG 1 Audio Layer 2 audio bitstream. */
  HA_TT_MP1_L3            = 18, /**< MPEG 1 Audio Layer 3 audio bitstream. */

  HA_TT_RSVD50            = 50 /**< */

} HA_AAC_TRANSPORT_TYPE;


typedef struct
{
    HI_U32  enCmd;
    HA_AAC_TRANSPORT_TYPE stType;
} HA_AAC_SET_TRANSPORT_TYPE_S;

typedef struct
{
    HI_U32  enCmd;
    HI_UCHAR *conf[FILEREAD_MAX_LAYERS];
    HI_U32 length[FILEREAD_MAX_LAYERS];
} HA_AAC_SET_CONFIGRAW_S;


#define HA_AAC_DecGetDefalutOpenParam(pOpenParam) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = HI_NULL; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = 0; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 8; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_FALSE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
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

#endif /* __HISI_AUDIO_DECODER_AAC_H__ */

