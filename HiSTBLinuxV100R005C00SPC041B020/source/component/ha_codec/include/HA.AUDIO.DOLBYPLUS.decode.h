/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : HA.AUDIO.DOLBYPLUS.decode.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HA_AUDIO_DOLBYPLUS_DECODE_H__
#define __HA_AUDIO_DOLBYPLUS_DECODE_H__

#include "hi_type.h"
#include "hi_audio_codec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



#define HA_DOLBYPLUS_ID 0x1010

/* The Command of online setting DRC mode, etc. */
#define HA_CODEC_DOLBYPLUS_SET_DRC_MODE_CMD       ((((HI_U32)HA_DOLBYPLUS_ID) << 16) | 0x1000)
#define HA_CODEC_DOLBYPLUS_SET_LFE_MODE_CMD       ((((HI_U32)HA_DOLBYPLUS_ID) << 16) | 0x1001)
#define HA_CODEC_DOLBYPLUS_SET_DMX_MODE_CMD       ((((HI_U32)HA_DOLBYPLUS_ID) << 16) | 0x1002)
#define HA_CODEC_DOLBYPLUS_SET_DYNSCALE_MODE_CMD  ((((HI_U32)HA_DOLBYPLUS_ID) << 16) | 0x1003)

/* The Command of set stop flag (normally only used in chip certification). */
#define HA_CODEC_SET_STOP_FLAG_CMD                ((((HI_U32)HA_DOLBYPLUS_ID) << 16) | 0x1004)

#define IEC61937_PAYLOAD_SIZE_EAC3_HD             (6144 * 4 + 1536 * 4)



/*!< Decoder dynamic range compression mode
     - 0 = custom mode, analog dialnorm
     - 1 = custom mode, digital dialnorm
     - 2 = line out mode
     - 3 = RF remod mode (default)
     - 4 = Portable mode -8dB (output reference level is -8dB)
     - 5 = Portable mode -11dB (output reference level is -11dB)
     - 6 = Portable mode -14dB (output reference level is -14dB)
     - t = Portable mode test (use this ONLY for bit-for-bit validation)
     - x = Dynamic range control suppression mode */
typedef enum
{
    DOLBYPLUS_DRC_CUSTOM_0 = 0,
    DOLBYPLUS_DRC_CUSTOM_1,
    DOLBYPLUS_DRC_LINE,
    DOLBYPLUS_DRC_RF,            // (default)
    DOLBYPLUS_DRC_PORTABLE_L8,
    DOLBYPLUS_DRC_PORTABLE_L11,
    DOLBYPLUS_DRC_PORTABLE_L14,
    DOLBYPLUS_DRC_PORTABLE_TEST
} DOLBYPLUS_DRC_MODE_E;

/*!< Define LFE Downmixing mode
     - 0 = Properly downmixes the LFE channel according to the lfemixlevcod parameter
           when DRC is set to Line mode
     - 1 = LFE channel is not downmixed when DRC is set to RF mode
     - 2 = .2 channels (only available for 7.1 PCM output) */
typedef enum
{
    DOLBYPLUS_LFEOUTOFF = 0,
    DOLBYPLUS_LFEOUTON,
    DOLBYPLUS_LFEDOWNMIX    = DOLBYPLUS_LFEOUTOFF,
    DOLBYPLUS_LFENOTDOWNMIX = DOLBYPLUS_LFEOUTON,    // (default)
    DOLBYPLUS_LFEOUTDUAL,
} DOLBYPLUS_LFEOUT_MODE;

/*!< Decoder output mode. If the output mode does not equal the bitstream audio 
     coding mode (acmod), then the decoder will perform downmixing or fill channels
     with zero values to meet the desired output configuration.

     Note: only the first main output can support values > 7
     - r = raw mode (outputs all standard (non-replaced) channels in 7.1-channel
           bitstream; no channel mixing or remapping performed)
     - 0 = reserved
     - 1 = 1/0 (C)
     - 2 = 2/0 (L, R)
     - 3 = 3/0 (L, R, C)
     - 4 = 2/1 (L, R, l)
     - 5 = 3/1 (L, R, C, l)
     - 6 = 2/2 (L, R, l, r)
     - 7 = 3/2 (L, R, C, l, r)
     - 9 = 2/2/1 (L, R, l, r, Ts)
     - 10 = 3/2/1 (L, R, C, l, r, Ts)
     - 11 = 3/2/1 (L, R, C, l, r, Cvh)
     - 12 = 3/0/2 (L, R, C, Lc, Rc)
     - 13 = 2/2/2 (L, R, l, r, Lw, Rw)
     - 14 = 2/2/2 (L, R, l, r, Lvh, Rvh)
     - 15 = 2/2/2 (L, R, l, r, Lsd, Rsd)
     - 16 = 2/2/2 (L, R, l, r, Lrs, Rrs)
     - 17 = 3/2/2 (L, R, C, l, r, Lc, Rc)
     - 18 = 3/2/2 (L, R, C, l, r, Lw, Rw)
     - 19 = 3/2/2 (L, R, C, l, r, Lvh, Rvh)
     - 20 = 3/2/2 (L, R, C, l, r, Lsd, Rsd)
     - 21 = 3/2/2 (L, R, C, l, r, Lrs, Rrs)
     - 22 = 3/2/2 (L, R, C, l, r, Ts, Cvh)
     - 23 = Follow (not yet implemented)  */
typedef enum
{
    DOLBYPLUS_OUT_MODE11 = 0,
    DOLBYPLUS_OUT_MODE10,
    DOLBYPLUS_OUT_MODE20,          // (default)
    DOLBYPLUS_OUT_MODE30,
    DOLBYPLUS_OUT_MODE21,
    DOLBYPLUS_OUT_MODE31,
    DOLBYPLUS_OUT_MODE22,
    DOLBYPLUS_OUT_MODE32,
} DOLBYPLUS_OUTPUT_MODE_E;

/*!< Decoder dual mono mode (Only when <i>acmod</i> is 1/1)
     - 0 = Stereo
     - 1 = Left mono
     - 2 = Right mono
     - 3 = Mixed mono */
typedef enum
{
    DOLBYPLUS_DUAL_STEREO = 0,   // (default)
    DOLBYPLUS_DUAL_LEFTMONO,
    DOLBYPLUS_DUAL_RGHTMONO,
    DOLBYPLUS_DUAL_MIXMONO,
} DOLBYPLUS_DUAL_MODE_E;

/*!< Decoder stereo output mode.  This parameter specifies the
     reproduction mode associated with stereo output(output mode = 2/0).
     - 0 = Automatically detects stereo mode based on setting in the bit stream.
     - 1 = 2/0 Dolby Surround compatible (Lt/Rt)
     - 2 = 2/0 Stereo (Lo/Ro)
     - 3 = Reserved */
typedef enum
{
    DOLBYPLUS_DMX_AUTO = 0,
    DOLBYPLUS_DMX_SRND,
    DOLBYPLUS_DMX_STEREO,
    DOLBYPLUS_DMX_LTRT = DOLBYPLUS_DMX_SRND,    // (default)
    DOLBYPLUS_DMX_LORO = DOLBYPLUS_DMX_STEREO,
} DOLBYPLUS_STEREODMX_MODE_E;

/*!< Define DDP convert on/off */
typedef enum
{
    DOLBYPLUS_CONVERTER_OFF = 0,
    DOLBYPLUS_CONVERTER_ON                      // (default)
} DOLBYPLUS_CONVERTER_MODE;


typedef struct
{
    HI_U32  enCmd;          /* HA_CODEC_SET_STOP_FLAG_CMD */
    HI_BOOL bStopFlag;
    HI_BOOL bADStopFlag;
} HA_SET_STOP_FLAG_S;

/** DDP  HA_DOLBYPLUS_EVENT_SOURCE_CHANGE event, stream information user data definition */
typedef struct  hiDOLBYPLUS_STREAM_INFO_S
{
    HI_S16 s16StreamType;   /* 0: dolby digital, 1: dolby digital plus */
    HI_S16 s16Acmod;
    HI_S32 s32BitRate;
    HI_S32 s32SampleRateRate;
} DOLBYPLUS_STREAM_INFO_S;

/* Define the struct for set drc mode from .DecSetConfig */
typedef struct
{
    HI_U32  enCmd;
    DOLBYPLUS_DRC_MODE_E eDrcMode;
} HA_DOLBYPLUS_SET_DRC_MODE_S;

/* Define the struct for set lfe out mode from .DecSetConfig */
typedef struct
{
    HI_U32  enCmd;
    DOLBYPLUS_LFEOUT_MODE eLfeMode;
} HA_DOLBYPLUS_SET_LFE_MODE_S;

/* Define the struct for set downmix mode from .DecSetConfig */
typedef struct
{
    HI_U32  enCmd;
    DOLBYPLUS_STEREODMX_MODE_E eDmxMode;
} HA_DOLBYPLUS_SET_DMX_MODE_S;

/* Define the struct for set dynamic scale value from .DecSetConfig */
typedef struct
{
    HI_U32  enCmd;
    HI_S16  s16DynScaleHigh;
    HI_S16  s16DynScaleLow;
} HA_DOLBYPLUS_SET_DYNSCALE_MODE_S;

/** DDP callback event defination */
typedef enum
{
    HA_DOLBYPLUS_EVENT_SOURCE_CHANGE = 0,     /**< stream information change event, DOLBYPLUS_STREAM_INFO_S */
    HA_DOLBYPLUS_EVENT_BUTT
} DOLBYPLUS_EVENT_E;


/** DDP callback method definition */
typedef HI_VOID (*DOLBYPLUS_EVENT_CB_FN)(DOLBYPLUS_EVENT_E enEvent, HI_VOID* pAppData);


typedef struct  hiDOLBYPLUS_DECODE_OPENCONFIG_S
{
    DOLBYPLUS_DRC_MODE_E       enDrcMode;      /* dynamic range compression mode */
    DOLBYPLUS_LFEOUT_MODE      enOutLfe;       /* output LFE channel present     */
    DOLBYPLUS_OUTPUT_MODE_E    enOutputMode;   /* output channel configuration   */
    DOLBYPLUS_STEREODMX_MODE_E enDmxMode;      /* stereo output mode    */
    DOLBYPLUS_DUAL_MODE_E      enDualMode;     /* dual mono reproduction mode   */
    HI_S16                     s16PcmScale;    /* PCM scale factor, 0~100,default:100,  100 means 1.0, 33 means 0.33,etc */
    HI_S16                     s16DynScaleHigh;/* dynamic range compression cut scale factor, 0~100,default:100,  100 means 1.0, 33 means 0.33,etc*/
    HI_S16                     s16DynScaleLow; /* dynamic range compression boost scale factor, 0~100,default:100, 100 means 1.0, 33 means 0.33,etc   */
    DOLBYPLUS_CONVERTER_MODE   enConvertMode;  /* ddp decoder-converter on/off     */
    HI_S16                     PirvateControl; /* private test control(0~255), default:0. */
    HI_CHAR                    szInputFileName[256]; /*  private test params */
    DOLBYPLUS_EVENT_CB_FN      pfnEvtCbFunc[HA_DOLBYPLUS_EVENT_BUTT];      /* ddp call back method */
    HI_VOID*                   pAppData[HA_DOLBYPLUS_EVENT_BUTT];          /* application defined value for ddp call back method */
} DOLBYPLUS_DECODE_OPENCONFIG_S;


#define HA_DOLBYPLUS_DecGetDefalutOpenConfig(pConfigParam) \
    do { HI_S32 n; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enDrcMode = DOLBYPLUS_DRC_RF; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enOutLfe  = DOLBYPLUS_LFEOUTON; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enOutputMode = DOLBYPLUS_OUT_MODE20; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enDmxMode    = DOLBYPLUS_DMX_SRND; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enDualMode   = DOLBYPLUS_DUAL_STEREO; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->s16PcmScale  = 100; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->s16DynScaleHigh = 100; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->s16DynScaleLow  = 100; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->enConvertMode   = DOLBYPLUS_CONVERTER_OFF; \
        ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->PirvateControl  = 0; \
        for (n = 0; n < HA_DOLBYPLUS_EVENT_BUTT; n++) \
        { \
            ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->pfnEvtCbFunc[n] = HI_NULL; \
            ((DOLBYPLUS_DECODE_OPENCONFIG_S *)(pConfigParam))->pAppData[n] = HI_NULL; \
        } \
    } while (0)


#define HA_DOLBYPLUS_DecGetDefalutOpenParam(pOpenParam, pstPrivateParams) \
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
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->pCodecPrivateData = (HI_VOID*)pstPrivateParams; \
        ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->u32CodecPrivateDataSize = sizeof(DOLBYPLUS_DECODE_OPENCONFIG_S); \
    } while (0)


#define HA_DOLBYPLUS_DecSetMode(pOpenParam, eDecMode) \
    do { ((HI_HADECODE_OPENPARAM_S *)(pOpenParam))->enDecMode = eDecMode; } while (0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HA_AUDIO_DOLBYPLUS_DECODE_H__ */

