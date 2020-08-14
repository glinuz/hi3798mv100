/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.AMRNB.codec.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_CODEC_AMRNB_H__
#define __HISI_AUDIO_CODEC_AMRNB_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define    HA_AMRNB_ID 0x0100        


#ifndef AMR_MAGIC_NUMBER
 #define AMR_MAGIC_NUMBER "#!AMR\n"/*magic number for AMR-NB*/
#endif

typedef enum
{
    AMRNB_MR475 = 0,     /*4.75kbit/s*/
    AMRNB_MR515,
    AMRNB_MR59,
    AMRNB_MR67,
    AMRNB_MR74,
    AMRNB_MR795,
    AMRNB_MR102,
    AMRNB_MR122,
    AMRNB_MRDTX,             /*SID mode*/
    AMRNB_N_MODES                /* number of (SPC) modes */
} AMRNB_MODE_E;

typedef enum
{
    AMRNB_MIME = 0,     /*4.75kbit/s*/
    AMRNB_IF1,
    AMRNB_IF2,
} AMRNB_FORMAT_E;

typedef struct
{
    AMRNB_FORMAT_E enFormat;

    AMRNB_MODE_E enMode;

    HI_BOOL bDTX;  /* HI_TRUE=enable dtx, HI_FALSE=disable dtx  */
} AMRNB_ENCODE_OPENCONFIG_S;

typedef struct
{
    AMRNB_FORMAT_E enFormat;
} AMRNB_DECODE_OPENCONFIG_S;

typedef enum
{
    AMRNB_CONFIGCMD_MODE = 0,
}  AMRNB_CONFIG_COMMAND_E;

/* struct for 
    HI_HA_ERRORTYPE_E (*EncodeSetConfig)(HI_VOID * hEncoder, HI_VOID *pstConfigStructure);
*/
typedef struct
{
    AMRNB_CONFIG_COMMAND_E enCmd;

    HI_VOID *pstPrivateParams;
} AMRNB_ENCODE_CONFIG_MODE_S;

#define HA_AMRNB_GetEncDefaultOpenParam(pOpenParam, pstPrivateParams) \
do{ ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredOutChannels = 1; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->bInterleaved  = HI_TRUE; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->s32BitPerSample = 16; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredSampleRate = 8000; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32SamplePerFrame = 160; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(AMRNB_ENCODE_OPENCONFIG_S); \
}while(0)

#define HA_AMRNB_GetDecDefalutOpenParam(pOpenParam, pstPrivateParams) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 1; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 8000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(AMRNB_DECODE_OPENCONFIG_S); \
}while(0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_CODEC_AMRNB_H__ */
