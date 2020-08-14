/*=================================================

Open MAX   Component: Hisi Audio Decoder
This module contains
File:    OMX_Audio_Hisi.h
Author:  Audio
Date:    2015

=================================================*/
#ifndef OMX_Audio_Hisi_h
#define OMX_Audio_Hisi_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <OMX_Core.h>
#include <OMX_Index.h>

typedef enum
{
    OMX_HA_IndexParamAudioDolby = OMX_IndexVendorStartUnused,
    OMX_HA_EnablePassThroughOutput,
} OMX_HA_INDEXTYPE;

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
    OMX_DOLBY_CHANCFG_OUT_MODE11 = 0,
    OMX_DOLBY_CHANCFG_OUT_MODE10,
    OMX_DOLBY_CHANCFG_OUT_MODE20,      /* default */
    OMX_DOLBY_CHANCFG_OUT_MODE30,
    OMX_DOLBY_CHANCFG_OUT_MODE21,
    OMX_DOLBY_CHANCFG_OUT_MODE31,
    OMX_DOLBY_CHANCFG_OUT_MODE22,
    OMX_DOLBY_CHANCFG_OUT_MODE32,
    OMX_DOLBY_CHANCFG_OUT_BUTT = 0x7fffffff,
} OMX_HA_DOLBYCHANCFG;

typedef enum  {
    OMX_DOLBY_STREAMTYPE_DD,
    OMX_DOLBY_STREAMTYPE_DDP,
    OMX_DOLBY_STREAMTYPE_BUTT,
} OMX_HA_DOLBYSTREAMTYPE;

typedef enum
{
    OMX_DOLBYPLUS_DRC_CUSTOM_0 = 0,
    OMX_DOLBYPLUS_DRC_CUSTOM_1,
    OMX_DOLBYPLUS_DRC_LINE,
    OMX_DOLBYPLUS_DRC_RF,            // (default)
    OMX_DOLBYPLUS_DRC_PORTABLE_L8,
    OMX_DOLBYPLUS_DRC_PORTABLE_L11,
    OMX_DOLBYPLUS_DRC_PORTABLE_L14,
    OMX_DOLBYPLUS_DRC_PORTABLE_TEST
}  OMX_HA_DOLBYDRCMODE;

typedef struct OMX_HA_PARAM_DOLBYTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nChannels;
    OMX_U32 nBitRate;
    OMX_U32 nSampleRate;
    OMX_U32 nDRCPercent;                    /**< Percentage of DRC to be applied, 0~100. Default is 0. */
    OMX_HA_DOLBYDRCMODE      eDRCMode;
    OMX_HA_DOLBYSTREAMTYPE  eStreamType;
    OMX_HA_DOLBYCHANCFG       eAcmod;
} OMX_HA_PARAM_DOLBYTYPE;

typedef struct OMX_HA_PARAM_DOLBYDRCMODE {
    OMX_U32 nDrcMode;              /**< 0 for line mode, 1 for rf mode*/
} OMX_HA_PARAM_DOLBYDRCMODE;

typedef struct OMX_HA_PARAM_DOLBYDRCRANGE {
    OMX_U32 nDrcRange;             /**< Only used for line mode, 0 is daytime mode, 100 is night mode*/
} OMX_HA_PARAM_DOLBYDRCRANGE;

typedef struct OMX_HA_PARAM_DOLBYSTREAMINFO {
    OMX_U32 nStreamType;           /**< Stream type, 0: dolby digital, 1: dolby digital plus */
    OMX_U32 nAcmod;                /**< Channel config of the stream */
    OMX_U32 nBitRate;              /**< Bit rate of the stream */
    OMX_U32 nSampleRate;           /**< Sampling rate of the stream */
} OMX_HA_PARAM_DOLBYSTREAMINFO;

typedef struct OMX_HA_PARAME_PASSTHROUGHOUTPUTSTATUS {
    OMX_U32 nStatus;                /**< 0 for disable, 1 for enable*/
} OMX_HA_PARAME_PASSTHROUGHOUTPUTSTATUS;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */

