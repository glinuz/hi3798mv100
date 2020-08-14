/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.DTSHD.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HISI_AUDIO_DECODER_DTSHD_H__
#define __HISI_AUDIO_DECODER_DTSHD_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


typedef  struct
{
    HI_U32  spkrOut;              /* set as a result of spkrout ,default is 2(Lo/Ro) */
    HI_BOOL enableDownmix; /* default true */
    HI_BOOL coreOnly;       /**< true: core decode only, false: dts hd decode,default value is true */
	HI_BOOL lfeMixedToFrontWhenNoDedicatedLFEOutput;         /**< true: enable lfe,default value is false */    
    HI_U32 outputBitWidth;        /**< 16: 16bit, 24: 24bit, 0,:native, defalut is 24 */
    HI_BOOL enableDialNorm; /* true: enable , default value is true */
    HI_U32  DRCPercent;      /* 0~100 - default is 0 ,no DRC */
    HI_BOOL enableHDPassThrough;       /* true: enable , default value is true */
    HI_U32  PirvateControl;     
} DTSHD_DECODE_OPENCONFIG_S;

#define HA_DTSHD_DecGetDefalutOpenConfig(pConfigParam) \
    do {  \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->spkrOut = 2; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->enableDownmix = HI_TRUE; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->coreOnly = HI_TRUE; \
		 ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->lfeMixedToFrontWhenNoDedicatedLFEOutput = HI_FALSE; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->outputBitWidth   = 24; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->enableDialNorm  = HI_TRUE; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->DRCPercent = 0; \
		((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->enableHDPassThrough = HI_TRUE; \
         ((DTSHD_DECODE_OPENCONFIG_S *)(pConfigParam))->PirvateControl = 0; \
    } while (0)

#define HA_DTSHD_DecGetDefalutOpenParam(pOpenParam, pstPrvateConfig) \
    do { HI_S32 i; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = HD_DEC_MODE_SIMUL; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredOutChannels = 2; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.bInterleaved = HI_TRUE; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32BitPerSample = 16; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.u32DesiredSampleRate = 48000; \
         for (i = 0; i < HA_AUDIO_MAXCHANNELS; i++) \
         { \
             ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->sPcmformat.enChannelMapping[i] = HA_AUDIO_ChannelNone; \
         } \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrvateConfig; \
         ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(DTSHD_DECODE_OPENCONFIG_S); \
    } while (0)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_DTSHD_H__ */
