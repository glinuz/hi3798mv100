/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_aiao_alsa_func.h
 * Description: drv aiao alsa func h
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 ********************************************************************************/

#ifndef __DRV_AIAO_ALSA_FUNC_H__
#define __DRV_AIAO_ALSA_FUNC_H__

#include <linux/fs.h>

#include "hi_type.h"
#include "hi_unf_sound.h"

#include "hi_drv_ao.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

//Playback
extern HI_S32 HI_DRV_AO_SND_Init(struct file* filp);
extern HI_S32 HI_DRV_AO_SND_DeInit(struct file* filp);
extern HI_S32 HI_DRV_AO_SND_Open(HI_UNF_SND_E enSound, DRV_SND_ATTR_S* pstAttr, struct file* filp);
extern HI_S32 HI_DRV_AO_SND_Close(HI_UNF_SND_E enSound, struct file* filp);
extern HI_S32 HI_DRV_AO_Track_Create(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr, HI_BOOL bAlsaTrack, struct file*  pfile, HI_HANDLE* phTrack);
extern HI_S32 HI_DRV_AO_SND_GetDefaultOpenAttr(HI_UNF_SND_E enSound, DRV_SND_ATTR_S* pstAttr);
extern HI_S32 HI_DRV_AO_Track_GetDefaultOpenAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr);
extern HI_S32 HI_DRV_AO_Track_Destroy(HI_HANDLE hSndTrack);
extern HI_S32 HI_DRV_AO_Track_Flush(HI_HANDLE hSndTrack);
extern HI_S32 HI_DRV_AO_Track_Start(HI_HANDLE hSndTrack);
extern HI_S32 HI_DRV_AO_Track_Stop(HI_HANDLE hSndTrack);
extern HI_S32 HI_DRV_AO_Track_GetDelayMs(HI_HANDLE hSndTrack, HI_U32* pDelayMs);
extern HI_S32 HI_DRV_AO_Track_SendData(HI_HANDLE hSndTrack, AO_FRAMEINFO_S* pstAOFrame);
extern HI_S32 HI_DRV_AO_Track_AtomicSendData(HI_HANDLE hTrack, AO_FRAMEINFO_S* pstAOFrame);

//SND Control  set global port volume
int aoe_set_volume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain);
int aoe_set_mute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute);
int aoe_get_mute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL* bMute);
int aoe_get_delayms(HI_UNF_SND_E enSound, HI_U32* pu32DelayMs);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
