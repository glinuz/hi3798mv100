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
#ifndef __HISI_AUDIO_DECODER_DTSM6_H__
#define __HISI_AUDIO_DECODER_DTSM6_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



/** DTSM6 HA_DOLBYPLUS_EVENT_SOURCE_CHANGE event, stream information user data defination
    u32StreamType:
        DTSDECSTRMTYPE_UNKNOWN              = 0x00000000,(indicator: Unkown)
        DTSDECSTRMTYPE_DTS_LEGACY           = 0x00000001,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_ES_MATRIX        = 0x00000002,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_ES_DISCRETE      = 0x00000004,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_9624             = 0x00000008,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_ES_8CH_DISCRETE  = 0x00000010,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_HIRES            = 0x00000020,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_MA               = 0x00000040,(indicator: DTS)
        DTSDECSTRMTYPE_DTS_LBR              = 0x00000080,(indicator: DTS Express)
        DTSDECSTRMTYPE_DTS_LOSSLESS         = 0x00000100,(indicator: DTS-HD Mater Audio/DTS-HD MSTR)
*/

typedef struct
{
    HI_U32 u32StreamType;
    HI_S32 s32BitRate;
    HI_S32 s32SampleRate;
} DTSM6_STREAM_INFO_S;

/** DTSM6 callback event defination */
typedef enum
{
    HA_DTSM6_EVENT_SOURCE_CHANGE = 0,     /**< stream information change event, DTSM6_STREAM_INFO_S */
    HA_DTSM6_EVENT_BUTT
} DTSM6_EVENT_E;

/** DTSM6 callback method definition */
typedef HI_VOID (*DTSM6_EVENT_CB_FN)(DTSM6_EVENT_E enEvent, HI_VOID* pAppData);


typedef struct
{
    HI_U32  spkrOut;              /* set as a result of spkrout ,default is 2(Lo/Ro) */
    HI_BOOL coreOnly;             /**< true: core decode only, false: dts hd decode,default value is false */
    HI_U32  outputBitWidth;       /**< 16: 16bit, 24: 24bit, 0,:native, defalut is 16 */
    HI_U32  DRCPercent;           /* 0~100 - default is 0 ,no DRC */
    HI_BOOL enableHDPassThrough;  /* true: enable , default value is true */
    HI_BOOL enableSPDIFOutput;    /* true: enable , default value is true */
    HI_BOOL enableTransEncode;    /* true: enable , default value is false */
    HI_U32  transEncodeMode;      /* 0,DTSTRANSCODEMODE_5_1_MIXOUT;1,DTSTRANSCODEMODE_FULL_MIXOUT;default 1 */
    HI_BOOL allowUalignWord;      /* true: allow , default value is true */
    DTSM6_EVENT_CB_FN  pfnEvtCbFunc[HA_DTSM6_EVENT_BUTT];   /* dtsm6 call back method */
    HI_VOID*           pAppData[HA_DTSM6_EVENT_BUTT];       /* application defined value for dtsm6 call back method */
} DTSM6_DECODE_OPENCONFIG_S;

#define HA_DTSM6_DecGetDefalutOpenConfig(pConfigParam) \
    do { HI_S32 n; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->spkrOut = 2; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->coreOnly = HI_FALSE; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->outputBitWidth  = 24; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->DRCPercent = 0; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->enableHDPassThrough = HI_TRUE; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->enableSPDIFOutput = HI_TRUE; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->enableTransEncode = HI_TRUE; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->transEncodeMode = 1; \
        ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->allowUalignWord = HI_TRUE; \
        for (n = 0; n < HA_DTSM6_EVENT_BUTT; n++) \
        { \
            ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->pfnEvtCbFunc[n] = HI_NULL; \
            ((DTSM6_DECODE_OPENCONFIG_S *)(pConfigParam))->pAppData[n] = HI_NULL; \
        } \
    } while (0)

#define HA_DTSM6_DecGetDefalutOpenParam(pOpenParam, pstPrvateConfig) \
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
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(DTSM6_DECODE_OPENCONFIG_S); \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HISI_AUDIO_DECODER_DTSM6_H__ */
