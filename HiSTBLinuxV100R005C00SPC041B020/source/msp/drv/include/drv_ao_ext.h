#ifndef __HI_DRV_AO_EXT_H__
#define __HI_DRV_AO_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_drv_dev.h"
#include "hi_drv_ao.h"

typedef HI_S32 (*FN_AO_DrvResume)(PM_BASEDEV_S*);
typedef HI_S32 (*FN_AO_DrvSuspend)(PM_BASEDEV_S*, pm_message_t);
//for voip
typedef HI_S32  (*FN_AO_TrackGetDefAttr)(HI_UNF_AUDIOTRACK_ATTR_S * );
typedef HI_S32  (*FN_AO_TrackAllocHandle)(HI_UNF_SND_E, HI_UNF_SND_TRACK_TYPE_E, struct file*, HI_HANDLE*);
typedef HI_VOID (*FN_AO_TrackFreeHandle)(HI_HANDLE);
typedef HI_S32  (*FN_AO_TrackCreate)(HI_UNF_SND_E, HI_UNF_AUDIOTRACK_ATTR_S*, HI_BOOL, AO_BUF_ATTR_S*, HI_HANDLE);
typedef HI_S32  (*FN_AO_TrackStart)(HI_U32);
typedef HI_S32  (*FN_AO_TrackStop)(HI_U32);
typedef HI_S32  (*FN_AO_TrackDestory)(HI_U32);
typedef HI_S32  (*FN_AO_TrackSendData)(HI_U32, AO_FRAMEINFO_S*);

typedef struct
{
    FN_AO_DrvResume  pfnAO_DrvResume;
    FN_AO_DrvSuspend pfnAO_DrvSuspend;

    FN_AO_TrackGetDefAttr  pfnAO_TrackGetDefAttr;
    FN_AO_TrackAllocHandle pfnAO_TrackAllocHandle;
    FN_AO_TrackFreeHandle  pfnAO_TrackFreeHandle;
    FN_AO_TrackCreate      pfnAO_TrackCreate;
    FN_AO_TrackDestory     pfnAO_TrackDestory;
    FN_AO_TrackStart       pfnAO_TrackStart;
    FN_AO_TrackStop        pfnAO_TrackStop;
    FN_AO_TrackSendData    pfnAO_TrackSendData;
} AIAO_EXPORT_FUNC_S;

HI_S32 AIAO_DRV_ModInit(HI_VOID);
HI_VOID AIAO_DRV_ModExit(HI_VOID);

#if defined(HI_MCE_SUPPORT)
HI_S32  HI_DRV_AO_Init(HI_VOID);
HI_VOID HI_DRV_AO_DeInit(HI_VOID);
HI_S32  HI_DRV_AO_SND_Init(struct file  *pfile);
HI_S32  HI_DRV_AO_SND_DeInit(struct file  *pfile);
HI_S32  HI_DRV_AO_SND_GetDefaultOpenAttr(HI_UNF_SND_E enSound, DRV_SND_ATTR_S *pstAttr);
HI_S32  HI_DRV_AO_SND_Open(HI_UNF_SND_E enSound, DRV_SND_ATTR_S *pstAttr, struct file *pfile);
HI_S32  HI_DRV_AO_SND_Close(HI_UNF_SND_E enSound, struct file  *pfile);
HI_S32  HI_DRV_AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S* pstGain);
HI_S32  HI_DRV_AO_Track_GetDefaultOpenAttr(HI_UNF_SND_TRACK_TYPE_E enTrackType, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr);
HI_S32  HI_DRV_AO_Track_Create(HI_UNF_SND_E enSound, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr, HI_BOOL bAlsaTrack, struct file *pfile, HI_HANDLE *phTrack);
HI_S32  HI_DRV_AO_Track_Destroy(HI_HANDLE hSndTrack);
HI_S32  HI_DRV_AO_Track_Flush(HI_HANDLE hSndTrack);
HI_S32  HI_DRV_AO_Track_Start(HI_HANDLE hSndTrack);
HI_S32  HI_DRV_AO_Track_Stop(HI_HANDLE hSndTrack);
HI_S32  HI_DRV_AO_Track_GetDelayMs(HI_HANDLE hSndTrack, HI_U32 *pDelayMs);
HI_S32  HI_DRV_AO_Track_SendData(HI_HANDLE hSndTrack, AO_FRAMEINFO_S *pstAOFrame);
HI_S32  HI_DRV_AO_Track_AttachAi(HI_HANDLE hSndTrack, HI_HANDLE hAi);
HI_S32  HI_DRV_AO_Track_DetachAi(HI_HANDLE hSndTrack, HI_HANDLE hAi);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif//__HI_DRV_AO_EXT_H__