/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.AMRWB.codec.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_CODEC_AMRWB_H__
#define __HISI_AUDIO_CODEC_AMRWB_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define    HA_AMRWB_ID 0x0110

#ifndef AMRWB_MAGIC_NUMBER
 #define AMRWB_MAGIC_NUMBER "#!AMR-WB\n" /*magic number for AMR-WB*/
#endif

#if 1
typedef enum
{
    AMRWB_MR66 = 0,                /* AMR-WB 6.60 kbit/s  */
    AMRWB_MR885,                    /* AMR-WB 8.85 kbit/s  */
    AMRWB_MR1265,                  /* AMR-WB 12.65 kbit/s */
    AMRWB_MR1425,                  /* AMR-WB 14.25 kbit/s */
    AMRWB_MR1585,                  /* AMR-WB 15.85 kbit/s */
    AMRWB_MR1825,                  /* AMR-WB 18.25 kbit/s */
    AMRWB_MR1985,                  /* AMR-WB 19.85 kbit/s */
    AMRWB_MR2305,                  /* AMR-WB 23.05 kbit/s */
    AMRWB_MR2385                   /* AMR-WB 23.85 kbit/s */
}AMRWB_MODE_E;
#endif

typedef enum
{
    AMRWB_FORMAT_MIME, 
    AMRWB_FORMAT_IF2
} AMRWB_FORMAT_E;

typedef struct
{
    AMRWB_FORMAT_E enFormat;

    AMRWB_MODE_E enMode;

    HI_BOOL bDTX;  /* HI_TRUE=enable dtx, HI_FALSE=disable dtx  */
} AMRWB_ENCODE_OPENCONFIG_S;

typedef struct
{
    AMRWB_FORMAT_E enFormat;
} AMRWB_DECODE_OPENCONFIG_S;

typedef enum
{
    AMRWB_CONFIGCMD_MODE = 0,
}  AMRWB_CONFIG_COMMAND_E;

/* struct for 
    HI_HA_ERRORTYPE_E (*EncodeSetConfig)(HI_VOID * hEncoder, HI_VOID *pstConfigStructure);
*/
typedef struct
{
    AMRWB_CONFIG_COMMAND_E enCmd;

    HI_VOID *pstPrivateParams;
} AMRWB_ENCODE_CONFIG_MODE_S;

#define HA_AMRWB_GetEncDefaultOpenParam(pOpenParam, pstPrivateParams) \
do{ ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredOutChannels = 1; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->bInterleaved  = HI_TRUE; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->s32BitPerSample = 16; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredSampleRate = 16000; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32SamplePerFrame = 320; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(AMRWB_ENCODE_OPENCONFIG_S); \
}while(0)

#define HA_AMRWB_GetDecDefalutOpenParam(pOpenParam, pstPrivateParams) \
do{ HI_S32 i; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_RAWPCM; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 1; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved  = HI_TRUE; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 16000; \
    for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
    { \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
    } \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
    ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(AMRWB_DECODE_OPENCONFIG_S); \
}while(0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_CODEC_AMRWB_H__ */
