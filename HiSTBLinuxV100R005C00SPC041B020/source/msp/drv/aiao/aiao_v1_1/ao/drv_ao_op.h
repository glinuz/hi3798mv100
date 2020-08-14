/******************************************************************************

Copyright (C), 2009-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hal_aiao_func.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/09/22
Last Modified :
Description   : aiao
Function List :
History       :
* main\1    2012-09-22   z40717     init.
******************************************************************************/
#ifndef __HI_AO_OP_FUNC_H__
#define __HI_AO_OP_FUNC_H__

#include "hi_unf_sound.h"
#include "hi_drv_ao.h"
#include "hal_aoe_func.h"
#include "hal_aiao_func.h"
#include "hal_aiao_common.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"
#include "hal_tianlai_adac_v500.h"
#include "hal_tianlai_adac_v510.h"
#include "drv_ao_private.h"

#ifdef HI_SND_AMP_SUPPORT
#include "drv_amp_ext.h"
#endif

#include "drv_ao_ioctl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define AO_SNDOP_PERIODBUFSIZE  4096
#define AO_SNDOP_LATENCY_THDMS  64

#define AO_SNDOP_GLOBAL_MUTE_BIT 1
#define AO_SNDOP_LOCAL_MUTE_BIT  0

#define AO_SNDOP_MAX_AOP_NUM 2  //max aop number per op

#define AO_SNDOP_MIN_AOP_PERIOD_SIZE 1024
/******************************Snd OP process FUNC*************************************/

typedef struct
{
    HI_U32 u32BitPerSample;
    HI_U32 u32Channels;
    HI_U32 u32SampleRate;
    HI_U32 u32DataFormat;
    HI_U32 u32LatencyThdMs;
    HI_U32 u32PeriodBufSize;
    HI_U32 u32PeriodNumber;
    union
    {
        HI_U32                u32DacAttr;
        HI_UNF_SND_I2S_ATTR_S stI2sAttr;
        HI_U32                u32SpdifAttr;
        HI_U32                u32HDMIAttr;
        HI_U32                u32ArcAttr;
    } unAttr;
} SND_OP_ATTR_S;

typedef enum
{
    SND_AOP_TYPE_I2S   = 0,     /* hbr or 2.0 pcm or 7.1 lpcm */
    SND_AOP_TYPE_SPDIF,         /* lbr or hbr(ddp) */
    SND_AOP_TYPE_CAST,          /* 2.0 16bit pcm only */

    SND_AOP_TYPE_BUTT
} SND_AOP_TYPE_E;

typedef enum
{
    SND_OUTPUT_TYPE_DAC,

    SND_OUTPUT_TYPE_I2S,

    SND_OUTPUT_TYPE_SPDIF,

    SND_OUTPUT_TYPE_HDMI,

    SND_OUTPUT_TYPE_CAST,

    SND_OUTPUT_TYPE_BUTT,
} SND_OUTPUT_TYPE_E;

typedef struct
{
    SND_OP_ATTR_S stSndPortAttr;

    HI_UNF_SND_GAIN_ATTR_S stUserGain;
#ifdef __DPT__
    HI_UNF_SND_PRECIGAIN_ATTR_S stUserPreciGain;
    HI_S32                 s32UserBalance;
#endif
    HI_UNF_TRACK_MODE_E    enUserTrackMode;
    HI_U32                 u32UserMute;   //bit[1]:global mute; bit[0]:local mute. if u32UserMute=0, real mute;else real unmute
    HI_U32                 u32LatencyMs;       //AOP Buffer threshold setting
    HI_BOOL                bBypass;
    HI_UNF_SAMPLE_RATE_E   enSampleRate;
#ifdef __DPT__
    HI_BOOL                bDrcEnable;
    HI_UNF_SND_DRC_ATTR_S  stDrcAttr;

    HI_BOOL                bPeqEnable;
    HI_UNF_SND_PEQ_ATTR_S  stPeqAttr;
#endif

    HI_BOOL                bADOutputEnable;

    /* internal state */
    SND_OP_STATUS_E         enCurnStatus;
    SND_OUTPUT_TYPE_E enOutType;
    HI_UNF_SND_OUTPUTPORT_E enOutPort;
    HI_S32                  ActiveId; /* 0 or 1*/
    HI_U32                  u32OpMask; /* bit0(0/1) and bit1(0/1)*/
    AIAO_PORT_ID_E          enPortID[AO_SNDOP_MAX_AOP_NUM];
    AOE_AOP_ID_E            enAOP[AO_SNDOP_MAX_AOP_NUM];
#ifdef HI_SND_AMP_SUPPORT
    AMP_EXPORT_FUNC_S*       pstAmpFunc;
#endif
    SND_ENGINE_TYPE_E       enEngineType[AO_SNDOP_MAX_AOP_NUM];
    MMZ_BUFFER_S            stRbfMmz[AO_SNDOP_MAX_AOP_NUM];
    AIAO_PORT_USER_CFG_S    stPortUserAttr[AO_SNDOP_MAX_AOP_NUM];
    AIAO_CAST_ID_E          CastId;
    AIAO_CAST_ATTR_S        stCastAttr;
    HI_UNF_SND_SPDIF_SCMSMODE_E     enUserSPDIFSCMSMode;
    HI_UNF_SND_SPDIF_CATEGORYCODE_E  enUserSPDIFCategoryCode;
    HI_U32 u32Delayms;
} SND_OP_STATE_S;

typedef struct
{
    HI_CHAR* pCmd;
    HI_S32 (*func) (SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_CHAR* pcBuf);
} SND_OP_WRITE_PROC_S;

/* internal state */
HI_VOID                 SND_GetPortDelayMs(SND_CARD_STATE_S* pCard, SND_OUTPUT_TYPE_E enOutType, HI_U32* pu32DelayMs);

HI_VOID                 SND_GetDelayMs(SND_CARD_STATE_S* pCard, HI_U32* pdelayms);
HI_VOID                 SND_DestroyOp(SND_CARD_STATE_S* pCard, HI_BOOL bSuspend);
HI_S32                  SND_CreateOp(SND_CARD_STATE_S* pCard, DRV_SND_ATTR_S* pstAttr, AO_ALSA_I2S_Param_S* pstAoI2sParam, HI_BOOL bResume);
HI_S32                  SND_SetOpMute(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute);
HI_S32                  SND_GetOpMute(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbMute);
HI_S32                  SND_SetOpHdmiMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enMode);
HI_S32                  SND_GetOpHdmiMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E* penMode);
HI_S32                  SND_SetOpSpdifMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E enMode);
HI_S32                  SND_GetOpSpdifMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_SPDIF_MODE_E* penMode);

#ifdef __DPT__
HI_S32                  SND_SetOpArcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable);
HI_S32                  SND_GetOpArcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable);
HI_S32                  SND_SetOpArcMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E enMode);
HI_S32                  SND_GetOpArcMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_MODE_E* penMode);
HI_S32                  SND_SetOpArcCap(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S stCap);
HI_S32                  SND_GetOpArcCap(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_ARC_AUDIO_CAP_S* pstCap);
#endif

HI_S32                  SND_SetOpVolume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                        HI_UNF_SND_GAIN_ATTR_S* pstGain);
HI_S32                  SND_GetOpVolume(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                        HI_UNF_SND_GAIN_ATTR_S* pstGain);

HI_S32                  SND_SetOpSpdifCategoryCode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                                   HI_UNF_SND_SPDIF_CATEGORYCODE_E enCategoryCode);

HI_S32                  SND_GetOpSpdifCategoryCode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                                   HI_UNF_SND_SPDIF_CATEGORYCODE_E* penCategoryCode);

HI_S32                  SND_SetOpSpdifSCMSMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                               HI_UNF_SND_SPDIF_SCMSMODE_E enSCMSMode);
HI_S32                  SND_GetOpSpdifSCMSMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                               HI_UNF_SND_SPDIF_SCMSMODE_E* penSCMSMode);
HI_S32                  SND_SetOpSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                            HI_UNF_SAMPLE_RATE_E enSampleRate);
HI_S32                  SND_GetOpSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                            HI_UNF_SAMPLE_RATE_E* penSampleRate);
HI_S32                  SND_SetOpTrackMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                           HI_UNF_TRACK_MODE_E enMode);
HI_S32                  SND_GetOpTrackMode(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                           HI_UNF_TRACK_MODE_E* penMode);
#ifdef __DPT__
HI_S32                  SND_SetOpPreciVol(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S stPreciGain);
HI_S32                  SND_GetOpPreciVol(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PRECIGAIN_ATTR_S* pstPreciGain);
HI_S32                  SND_SetOpBalance(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32 s32Balance);
HI_S32                  SND_GetOpBalance(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_S32* ps32Balance);

HI_S32                  SND_SetOpDrcEnable(SND_CARD_STATE_S* pCard,  HI_UNF_SND_OUTPUTPORT_E enOutPort,  HI_BOOL bEnable);
HI_S32                  SND_GetOpDrcEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable);
HI_S32                  SND_SetOpDrcAttr(SND_CARD_STATE_S* pCard,  HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr);
HI_S32                  SND_GetOpDrcAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_DRC_ATTR_S* pstDrcAttr);

HI_S32                  SND_SetOpPeqEnable(SND_CARD_STATE_S* pCard,  HI_UNF_SND_OUTPUTPORT_E enOutPort,  HI_BOOL bEnable);
HI_S32                  SND_GetOpPeqEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable);
HI_S32                  SND_SetOpPeqAttr(SND_CARD_STATE_S* pCard,  HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr);
HI_S32                  SND_GetOpPeqAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_PEQ_ATTR_S* pstPeqAttr);
#endif
HI_S32                  SND_SetOpAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                      SND_OP_ATTR_S* pstSndPortAttr);
HI_S32                  SND_GetOpAttr(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort,
                                      SND_OP_ATTR_S* pstSndPortAttr);

HI_S32                  SND_StopOp(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort);
HI_S32                  SND_StartOp(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort);
HI_U32                  SND_SetLowLatency(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32LatencyMs);
HI_U32                  SND_GetLowLatency(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32LatencyMs);
HI_S32                  SND_SetOpAefBypass(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bBypass);
HI_S32                  SND_GetOpAefBypass(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbBypass);

HI_S32                  SND_SetPortSampleRate(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32SampleRate);
HI_S32                  SND_GetPortInfo(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, SND_PORT_KERNEL_ATTR_S* pstPortKAttr);

#ifdef __DPT__
HI_S32                  SND_SetOpADOutputEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bEnable);
HI_S32                  SND_GetOpADOutputEnable(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* pbEnable);
#endif

HI_VOID*                SNDGetOpHandleByOutPort(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort);
HI_VOID*                SND_GetOpHandlebyOutType(SND_CARD_STATE_S* pCard, SND_OUTPUT_TYPE_E enOutType);
SND_ENGINE_TYPE_E       SND_GetOpEngineType(HI_VOID* pSndOp);
AOE_AOP_ID_E            SND_GetOpAopId(HI_VOID* pSndOp);
SND_ENGINE_TYPE_E       SND_GetOpGetOutType(HI_VOID* pSndOp);
HI_UNF_SND_OUTPUTPORT_E SND_GetOpOutputport(HI_VOID* pSndOp);
HI_S32                  SND_SetAdacEnable(SND_CARD_STATE_S* pCard, HI_BOOL bEnable);

#if defined (HI_SND_DRV_SUSPEND_SUPPORT)
HI_S32 SND_GetOpSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings);
HI_S32 SND_RestoreOpSetting(SND_CARD_STATE_S* pCard, SND_CARD_SETTINGS_S* pstSndSettings);
#endif

HI_S32 SND_StopCastOp(SND_CARD_STATE_S* pCard, HI_S32 s32CastID);
HI_S32 SND_StartCastOp(SND_CARD_STATE_S* pCard, HI_S32 s32CastID);
HI_S32 SND_CreateCastOp(SND_CARD_STATE_S* pCard,  HI_S32* ps32CastId, HI_UNF_SND_CAST_ATTR_S* pstAttr, MMZ_BUFFER_S* pstMMz);

HI_S32 SND_DestoryCastOp(SND_CARD_STATE_S *pCard,  HI_U32 CastId);
HI_U32 SND_ReadCastData(SND_CARD_STATE_S *pCard, HI_S32 u32CastId, AO_Cast_Data_Param_S *pstCastData);
HI_U32 SND_ReleaseCastData(SND_CARD_STATE_S *pCard, HI_S32 u32CastId, AO_Cast_Data_Param_S *pstCastData);

#if (1 == HI_PROC_SUPPORT)
HI_S32 SND_ReadOpProc(struct seq_file* p, SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enPort);
HI_S32 SND_WriteOpProc(SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf);
#endif

HI_VOID SND_GetXRunCount(SND_CARD_STATE_S* pCard, HI_U32* pu32Count);
HI_S32 SND_SetDelayCompensation(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32DelayMs);
HI_S32 SND_GetDelayCompensation(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32* pu32DelayMs);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  // __HI_AO_OP_FUNC_H__
