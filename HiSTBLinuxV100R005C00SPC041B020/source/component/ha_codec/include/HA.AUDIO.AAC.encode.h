/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.AAC.encode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_ENCODER_AAC_H__
#define __HISI_AUDIO_ENCODER_AAC_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define    HA_AAC_ID 0x001         

typedef enum
{
    AAC_QualityExcellent = 0,
    AAC_QualityHigh   = 1,
    AAC_QualityMedium = 2,
    AAC_QualityLow = 3,
} AAC_AuQuality;

typedef enum
{
    AAC_FORMAT_LC = 0,                  /**< AAC LC */
    AAC_FORMAT_EAAC = 1,                    /**< eAAC  (HEAAC or AAC+  or aacPlusV1) */
    AAC_FORMAT_EAACPLUS = 2,            /**< eAAC+ (AAC++ or aacPlusV2) */
} AAC_AuEncoderFormat;

typedef  struct
{
    AAC_AuQuality       quality;
    AAC_AuEncoderFormat coderFormat;
    HI_S16          bitsPerSample;
    HI_S32          sampleRate;    /**< audio file sample rate */
    HI_S32          bitRate;            /**< encoder bit rate in bits/sec */
    HI_S16          nChannelsIn;    /**< number of channels on input (1,2) */
    HI_S16          nChannelsOut;  /**< number of channels on output (1,2) */
    HI_S16          bandWidth;       /**< targeted audio bandwidth in Hz */
} AAC_ENC_CONFIG;

#define HA_AAC_GetDefaultConfig(pstConfig) \
do{ ((AAC_ENC_CONFIG *)(pstConfig))->coderFormat = AAC_FORMAT_LC; \
    ((AAC_ENC_CONFIG *)(pstConfig))->bitsPerSample = 16; \
    ((AAC_ENC_CONFIG *)(pstConfig))->quality = AAC_QualityHigh; \
    ((AAC_ENC_CONFIG *)(pstConfig))->bitRate = 128000; \
    ((AAC_ENC_CONFIG *)(pstConfig))->sampleRate = 48000; \
    ((AAC_ENC_CONFIG *)(pstConfig))->bandWidth    = ((AAC_ENC_CONFIG *)(pstConfig))->sampleRate / 2; \
    ((AAC_ENC_CONFIG *)(pstConfig))->nChannelsIn  = 2; \
    ((AAC_ENC_CONFIG *)(pstConfig))->nChannelsOut = 2; \
}while(0)

#define HA_AAC_GetEncDefaultOpenParam(pOpenParam, pstPrvateConfig) \
do{ HI_S32 inSamplePerFrame; \
    inSamplePerFrame = 1024; \
    if ((((AAC_ENC_CONFIG *)pstPrvateConfig)->coderFormat == AAC_FORMAT_EAAC) | (((AAC_ENC_CONFIG *)pstPrvateConfig)->coderFormat == AAC_FORMAT_EAACPLUS)) \
    { \
        inSamplePerFrame <<= 1; \
    } \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredOutChannels = ((AAC_ENC_CONFIG *)pstPrvateConfig)->nChannelsOut; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->bInterleaved = HI_TRUE; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->s32BitPerSample = ((AAC_ENC_CONFIG *)pstPrvateConfig)->bitsPerSample; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32DesiredSampleRate = ((AAC_ENC_CONFIG *)pstPrvateConfig)->sampleRate; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32SamplePerFrame = inSamplePerFrame; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrvateConfig; \
    ((HI_HAENCODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(AAC_ENC_CONFIG); \
}while(0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_ENCODER_AAC_H__ */
