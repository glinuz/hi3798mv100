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
#ifndef __HI_AO_CAST_H__
#define __HI_AO_CAST_H__

#include "hi_unf_sound.h"
#include "hi_drv_ao.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define AO_CAST_DEFATTR_FRAMEMAXNUM  8
#define AO_CAST_DEFATTR_SAMPLESPERFRAME  1024
#define AO_CAST_DEFATTR_CHANNEL  2
#define AO_CAST_DEFATTR_BITSPERSAMPLE   16
#define AO_CAST_DEFATTR_SAMPLERATE  48000


/******************************Snd Cast process FUNC*************************************/

typedef enum
{
    SND_CAST_STATUS_STOP = 0,
    SND_CAST_STATUS_START,
    SND_CAST_STATUS_PAUSE,
    SND_CAST_STATUS_BUTT,
} SND_CAST_STATUS_E;

typedef struct
{
    HI_UNF_SND_ABSGAIN_ATTR_S  stCastAbsGain;  // L/R Gain
    HI_BOOL bMute;
    HI_UNF_SND_CAST_ATTR_S stUserCastAttr;
    HI_VIRT_ADDR_T  tUserVirtAddr;
    HI_VIRT_ADDR_T  tKernelVirtAddr;    //for kernel use cast
    HI_PHYS_ADDR_T  tPhyAddr;
    HI_BOOL bUserEnableSetting;

    HI_U32                  u32Channels;
    HI_U32                  u32SampleRate;
    HI_S32                  s32BitPerSample;
    HI_BOOL                 bAddMute;
    /* internal state */
    HI_U32                  hCast;
    HI_U32                  CastId;
    HI_U32                  u32FrameBytes;
    HI_U32                  u32FrameSamples;
    HI_U32                  u32SampleBytes;

    HI_BOOL                 bAcquireCastFrameFlag;
    SND_CAST_STATUS_E      enCurnStatus;

} SND_CAST_STATE_S;

#ifdef __DPT__
HI_S32 CAST_SetAllMute(SND_CARD_STATE_S* pCard, HI_BOOL bMute);
#endif

HI_S32 CAST_GetDefAttr(HI_UNF_SND_CAST_ATTR_S * pstDefAttr);
HI_S32 CAST_CreateNew(SND_CARD_STATE_S *pCard, HI_UNF_SND_CAST_ATTR_S *pstCastAttr, MMZ_BUFFER_S *pstMMz, HI_U32 hCast);
HI_S32 CAST_DestroyCast(SND_CARD_STATE_S *pCard, HI_U32 hCast);
HI_S32 CAST_SetInfo(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_VIRT_ADDR_T tUserVirtAddr);
HI_S32 CAST_GetInfo(SND_CARD_STATE_S *pCard, HI_U32 hCast, AO_Cast_Info_Param_S *pstInfo);
HI_S32 CAST_SetEnable(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_BOOL bEnable);
HI_S32 CAST_GetEnable(SND_CARD_STATE_S *pCard, HI_U32 hCast, HI_BOOL *pbEnable);
HI_S32 CAST_ReadData(SND_CARD_STATE_S *pCard, HI_U32 u32CastId, AO_Cast_Data_Param_S *pstCastData);
HI_S32 CAST_ReleaseData(SND_CARD_STATE_S *pCard, HI_U32 u32CastId,  AO_Cast_Data_Param_S *pstCastData);
HI_VOID CAST_GetSettings(SND_CARD_STATE_S *pCard, HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings);
HI_VOID CAST_RestoreSettings(SND_CARD_STATE_S *pCard, HI_HANDLE hCast, SND_CAST_SETTINGS_S* pstCastSettings);

HI_S32 CAST_GetAbsGain(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S *pstCastAbsGain);
HI_S32 CAST_SetAbsGain(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_UNF_SND_ABSGAIN_ATTR_S *pstTrackAbsGain);
HI_S32 CAST_SetMute(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_BOOL bMute);
HI_S32 CAST_GetMute(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_BOOL *pbMute);
HI_S32 Cast_SetAefBypass(SND_CARD_STATE_S *pCard, HI_U32 u32CastID, HI_BOOL bBypass);

#if (1 == HI_PROC_SUPPORT)
HI_VOID CAST_ReadProc(struct seq_file* p, SND_CARD_STATE_S *pCard);
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  // __HI_AO_TRACK_H__
