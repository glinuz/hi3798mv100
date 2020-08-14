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
#ifndef __HI_AO_TRACK_H__
#define __HI_AO_TRACK_H__

#include "hi_unf_sound.h"
#include "hi_drv_ao.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define AO_TRACK_LATENCYMS_DEFAULT 256

#ifdef HI_SND_HBRA_PASSTHROUGH_SUPPORT
#define AO_TRACK_PCM_BUFSIZE_MS_MAX 512
#else
#define AO_TRACK_PCM_BUFSIZE_MS_MAX 256
#endif

#define AO_TRACK_LBR_BUFSIZE_MS_MAX 512
#define AO_TRACK_HBR_BUFSIZE_MS_MAX 512
#define AO_TRACK_BUF_EMPTY_THRESHOLD_MS 10 //aip 5ms + engine 5ms

#define AO_TRACK_PCM_BUFSIZE_BYTE_MAX (((192000 * 2 * sizeof(HI_U32)) / 1000) * AO_TRACK_PCM_BUFSIZE_MS_MAX)
#define AO_TRACK_LBR_BUFSIZE_BYTE_MAX (((192000 * 2 * sizeof(HI_U32)) / 1000) * AO_TRACK_PCM_BUFSIZE_MS_MAX)

#ifdef HI_SND_HBRA_PASSTHROUGH_SUPPORT
#define AO_TRACK_HBR_BUFSIZE_BYTE_MAX (((192000 * 8 * sizeof(HI_U32)) / 1000) * AO_TRACK_PCM_BUFSIZE_MS_MAX)
#else
#define AO_TRACK_HBR_BUFSIZE_BYTE_MAX (((192000 * 2 * sizeof(HI_U32)) / 1000) * AO_TRACK_PCM_BUFSIZE_MS_MAX)
#endif

#define AO_TRACK_NORMAL_CHANNELNUM 2
#define AO_TRACK_MAX_CHANNELNUM 8
#define AO_TRACK_MUTILPCM_CHANNELNUM 8
#define AO_TRACK_MC_CHANNELNUM       8   // max chnnum is 8,add addtional 2chn datasize
#define AO_TRACK_BITDEPTH_LOW 16
#define AO_TRACK_BITDEPTH_HIGH 24
#define AO_TRACK_DEFATTR_BUFSIZE 1024
#define AO_TRACK_MASTER_DEFATTR_BUFLEVELMS 400
#define AO_TRACK_MASTER_MIN_BUFLEVELMS 30    //8k,aac MaxPcmOutSampleSize:2048,  2048/8000 = 0.258s = 258ms
#define AO_TRACK_MASTER_MAX_BUFLEVELMS 800
#define AO_TRACK_MASTER_DEFATTR_FADEINMS 16
#define AO_TRACK_MASTER_DEFATTR_FADEOUTMS 8
#define AO_TRACK_MASTER_DEFATTR_BUFSIZE (4 * AO_TRACK_DEFATTR_BUFSIZE)
#define AO_TRACK_SLAVE_DEFATTR_BUFLEVELMS 400
#define AO_TRACK_SLAVE_DEFATTR_FADEINMS 1
#define AO_TRACK_SLAVE_DEFATTR_FADEOUTMS 1
#define AO_TRACK_SLAVE_DEFATTR_BUFSIZE (4 * AO_TRACK_DEFATTR_BUFSIZE)
#define AO_TRACK_VIRTUAL_DEFATTR_BUFSIZE (256 * AO_TRACK_DEFATTR_BUFSIZE)
#define AO_TRACK_DEFATTR_START_THRESHOLDMS 50
#define AO_TRACK_DUPLICATE_DELAY_THRESHOLDMS 70
#define AO_TRACK_DUPLICATE_BUF_RESERVEMS 60

#define AO_TRACK_PATH_NAME_MAXLEN 256
#define AO_TRACK_FILE_NAME_MAXLEN 256

typedef enum
{
    TRACK_STREAMMODE_CHANGE_NONE = 0,
    TRACK_STREAMMODE_CHANGE_PCM2PCM,
    TRACK_STREAMMODE_CHANGE_PCM2LBR,
    TRACK_STREAMMODE_CHANGE_PCM2HBR,
    TRACK_STREAMMODE_CHANGE_LBR2PCM,
    TRACK_STREAMMODE_CHANGE_LBR2LBR,
    TRACK_STREAMMODE_CHANGE_LBR2HBR,
    TRACK_STREAMMODE_CHANGE_HBR2PCM,
    TRACK_STREAMMODE_CHANGE_HBR2LBR,
    TRACK_STREAMMODE_CHANGE_HBR2HBR,
} TRACK_STREAMMODE_CHANGE_E;


/******************************Snd Track process FUNC*************************************/

/*Audio output attribute */
typedef struct
{
    HI_U32 u32PcmSampleRate;
    HI_U32 u32LbrSampleRate;
    HI_U32 u32HbrSampleRate;

    HI_U32 u32PcmChannels;
    HI_U32 u32LbrChannels; /* 2 */
    HI_U32 u32HbrChannels; /* 2(DDP) or 8 */
    HI_U32 u32OrgMultiPcmChannels; /* 6 or 8 */

    HI_U32 u32PcmBitDepth; /* 16 or 24 */
    HI_U32 u32LbrBitDepth; /* 16 */
    HI_U32 u32HbrBitDepth; /* 16(Iec61937) or 24(blue-ray LPCM) */

    HI_U32 u32PcmSamplesPerFrame;
    HI_U32 u32PcmBytesPerFrame;
    HI_U32 u32LbrBytesPerFrame;
    HI_U32 u32HbrBytesPerFrame;

    HI_U32 u32LbrFormat;  /* DD/DTS */
    HI_U32 u32HbrFormat;  /* (DDP/DTSHD/TRUEHD or 8ch-LPCM */

    HI_VOID *pPcmDataBuf;       /**<I/O, pointer to the decoded PCM data.
                                  Note: 1) pDataBuf must be 32-word aligned. */
    HI_VOID *pLbrDataBuf;       /**<I/O, pointer to the decoded LBR data.
                                Note: 1) pDataBuf must be 32-word aligned. */
    HI_VOID *pHbrDataBuf;       /**<I/O, pointer to the decoded HBR data.
                                  Note: 1) pDataBuf must be 32-word aligned. */

#ifdef DRV_SND_MULTICH_AEF_SUPPORT
    HI_U32 u32McPcmSampleRate;
    HI_U32 u32McPcmChannels;
    HI_U32 u32McPcmBitDepth;
    HI_U32 u32McPcmBytesPerFrame;
    HI_VOID *pMcPcmDataBuf;       /**<I/O, pointer to the decoded Multichannel PCM data.
                                  Note: 1) pDataBuf must be 32-word aligned. */
#endif

#ifdef DRV_SND_AD_OUTPUT_SUPPORT
    HI_U32 u32AssocPcmSampleRate;
    HI_U32 u32AssocPcmChannels;
    HI_U32 u32AssocPcmBitDepth;
    HI_U32 u32AssocPcmBytesPerFrame;
    HI_VOID *pAssocPcmDataBuf;       /**<I/O, pointer to the decoded Multichannel PCM data.
                                  Note: 1) pDataBuf must be 32-word aligned. */
#endif

} SND_TRACK_STREAM_ATTR_S;

typedef struct
{
    TRACK_STREAMMODE_CHANGE_E enPcmChange;
    TRACK_STREAMMODE_CHANGE_E enSpdifChange;
    TRACK_STREAMMODE_CHANGE_E enHdmiChnage;
} STREAMMODE_CHANGE_ATTR_S;

typedef struct
{
    HI_BOOL                 bThreadRun;
    MMZ_BUFFER_S            stRbfMmz;  //alsa mmz buf
    CIRC_BUF_S              stCB;
    HI_U32                  uBufWptr;
    HI_U32                  uBufRptr;
} TRACK_SAVE_ALSA_ATTR_S;


typedef struct
{
    HI_UNF_AUDIOTRACK_ATTR_S stUserTrackAttr;
    HI_UNF_SND_ABSGAIN_ATTR_S  stTrackAbsGain;  // L/R Gain
#ifdef __DPT__
    HI_UNF_SND_PRECIGAIN_ATTR_S  stUserPrescale;  // for prescale
    HI_UNF_SND_SOURCE_E enSource;
#endif
    HI_BOOL bMute;
    HI_UNF_TRACK_MODE_E enChannelMode;
    AO_SND_SPEEDADJUST_TYPE_E enUserSpeedType;
    HI_S32 s32UserSpeedRate;
    HI_U32 u32AddMuteFrameNum;
    HI_U32 u32AddPauseResumeNum;
    HI_BOOL bEosFlag;

    /* internal state */
    HI_U32                  TrackId;
    HI_BOOL                 bAlsaTrack;
    SND_TRACK_STATUS_E      enCurnStatus;
    SND_TRACK_STREAM_ATTR_S stStreamAttr;
#ifdef HI_AUDIO_AI_SUPPORT
    HI_BOOL                 bAttAi;
    HI_HANDLE               hAi;
#endif
    HI_U32                  u32PauseDelayMs;

#if (1 == HI_PROC_SUPPORT)
    /*save pcm*/
    SND_DEBUG_CMD_CTRL_E    enSaveState;
    HI_U32                  u32SaveCnt;
    struct file *           fileHandle;
#endif

    /*track send statistics*/
    HI_U32                  u32SendTryCnt;
    HI_U32                  u32SendCnt;

    AOE_AIP_ID_E            enAIP[SND_AIP_TYPE_BUTT];
    MMZ_BUFFER_S            stAipRbfMmz[SND_AIP_TYPE_BUTT];
    HI_BOOL                 bAipRbfExtDmaMem[SND_AIP_TYPE_BUTT];
    HI_BOOL                 bFifoBypass;
    HI_BOOL                 bAipPriority;
#ifdef __DPT__
    HI_BOOL                 bDuplicate;
    HI_UNF_SND_OUTPUTPORT_E enDuplicatePort;
#endif

    TRACK_SAVE_ALSA_ATTR_S  stSaveAlsa;
} SND_TRACK_STATE_S;

typedef enum
{
    AO_HDMI_CAPABILITY_Reserved = 0,
    AO_HDMI_CAPABILITY_PCM,
    AO_HDMI_CAPABILITY_AC3,
    AO_HDMI_CAPABILITY_MPEG1,
    AO_HDMI_CAPABILITY_MP3,
    AO_HDMI_CAPABILITY_MPEG2,
    AO_HDMI_CAPABILITY_AAC,
    AO_HDMI_CAPABILITY_DTS,
    AO_HDMI_CAPABILITY_ATRAC,
    AO_HDMI_CAPABILITY_ONEBITAUDIO,
    AO_HDMI_CAPABILITY_DDP,
    AO_HDMI_CAPABILITY_DTSHD,
    AO_HDMI_CAPABILITY_MAT,
    AO_HDMI_CAPABILITY_DST,
    AO_HDMI_CAPABILITY_WMAPRO,
    AO_HDMI_CAPABILITY_ReservedFmt15,
    AO_HDMI_CAPABILITY_BUTT
} AO_HDMI_CAPABILITY_E;

HI_U32    TRACK_RS_GetFreeId(HI_VOID);

HI_VOID TRACK_RS_Init(HI_VOID);

HI_VOID TRACK_RS_DeInit(HI_VOID);

HI_VOID TRACK_RS_RegisterId(HI_U32 TrackId);

HI_VOID TRACK_RS_DeRegisterId(HI_U32 TrackId);

HI_S32  TRACK_Create(SND_CARD_STATE_S *pCard, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr,
                     HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf, HI_HANDLE *phTrack);

HI_S32  TRACK_CreateNew(SND_CARD_STATE_S *pCard, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr,
                    HI_BOOL bAlsaTrack, AO_BUF_ATTR_S *pstBuf, HI_U32 TrackId);

HI_S32  TRACK_Destroy(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
HI_S32  TRACK_Start(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
HI_S32  TRACK_Stop(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
HI_S32  TRACK_Pause(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
HI_S32  TRACK_Flush(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
HI_S32  TRACK_SetAipFiFoBypass(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL bEnable);
HI_VOID TRACK_DestroyEngine(SND_CARD_STATE_S *pCard);
HI_S32  TRACK_CheckAttr(HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr);
HI_S32  TRACK_SetAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr);
HI_S32  TRACK_GetAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr);
HI_S32  TRACK_SendData(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, AO_FRAMEINFO_S * pstAOFrame);
HI_S32  TRACK_AtomicSendData(SND_CARD_STATE_S* pCard, HI_U32 u32TrackID, AO_FRAMEINFO_S* pstAOFrame);
HI_S32  TRACK_Mmap(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, AO_Track_Mmap_Param_S *pstParam);
HI_S32  TRACK_SetWeight(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S *pstTrackGain);
HI_S32  TRACK_GetWeight(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_GAIN_ATTR_S *pstTrackGain);
HI_S32  TRACK_SetSpeedAdjust(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, AO_SND_SPEEDADJUST_TYPE_E enType, HI_S32 s32Speed);
HI_S32  TRACK_GetDelayMs(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32DelayMs);
HI_U32  TRACK_GetAdacDelayMs(SND_CARD_STATE_S* pCard, SND_TRACK_STATE_S* pTrack);
HI_U32  TRACK_GetSpdifDelayMs(SND_CARD_STATE_S* pCard, SND_TRACK_STATE_S* pTrack);
HI_U32  TRACK_GetHDMIDelayMs(SND_CARD_STATE_S* pCard, SND_TRACK_STATE_S* pTrack);

HI_S32  TRACK_IsBufEmpty(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL *pbBufEmpty);
HI_S32  TRACK_SetEosFlag(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL bEosFlag);
HI_S32  TRACK_GetStatus(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_VOID *pstParam);
HI_S32  TRACK_GetDefAttr(HI_UNF_AUDIOTRACK_ATTR_S * pstDefAttr);

#if (1 == HI_PROC_SUPPORT)
HI_S32  TRACK_ReadProc(struct seq_file* p, SND_CARD_STATE_S *pCard);
HI_S32  TRACK_WriteProc(HI_UNF_SND_E enSound, SND_CARD_STATE_S* pCard, HI_CHAR* pcBuf);
HI_S32  TRACK_WriteProc_SaveData(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, SND_DEBUG_CMD_CTRL_E enCmd);
#endif

HI_S32  TRACK_DetectAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_AUDIOTRACK_ATTR_S * pstTrackAttr, SND_TRACK_ATTR_SETTING_E *penAttrSetting);
HI_U32  TRACK_GetMasterId(SND_CARD_STATE_S *pCard);
#ifdef HI_AUDIO_AI_SUPPORT
HI_S32  TRACK_SetPcmAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_HANDLE hAi);
HI_S32  TRACK_AttachAi(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_HANDLE hAi);
HI_S32  TRACK_DetachAi(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);
#endif

HI_S32  TRACK_GetSetting(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, SND_TRACK_SETTINGS_S* pstSndSettings);
HI_S32  TRACK_RestoreSetting(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, SND_TRACK_SETTINGS_S* pstSndSettings);
HI_S32  TRACK_SetAbsGain(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_ABSGAIN_ATTR_S *pstTrackAbsGain);
HI_S32  TRACK_GetAbsGain(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_ABSGAIN_ATTR_S *pstTrackAbsGain);
HI_S32  TRACK_SetPrescale(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_PRECIGAIN_ATTR_S *pstPreciGain);
HI_S32  TRACK_GetPrescale(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_SND_PRECIGAIN_ATTR_S *pstPreciGain);
HI_S32  TRACK_SetMute(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL bMute);
HI_S32  TRACK_GetMute(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL *pbMute);
HI_S32  TRACK_SetAllMute(SND_CARD_STATE_S *pCard, HI_BOOL bMute);
HI_S32  TRACK_SetChannelMode(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_TRACK_MODE_E *penMode);
HI_S32  TRACK_GetChannelMode(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_UNF_TRACK_MODE_E *penMode);
//for ALSA
HI_S32  TRACK_SetBufAttr(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 u32PeriodSize);    //  NO  USED
HI_S32  TRACK_UpdateWptrPos(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32WptrLen);  //for alsa
HI_S32  TRACK_UpdateRptrPos(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32RptrLen);    //for alsa
HI_S32  TRACK_FlushBuf(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID);    //for alsa
HI_S32  TRACK_GetReadPos(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_U32 *pu32ReadPos);    //for alsa
HI_S32  TRACK_SetPriority(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL bEnable);
HI_S32  TRACK_GetPriority(SND_CARD_STATE_S *pCard, HI_U32 u32TrackID, HI_BOOL *pbEnable);

#ifdef __DPT__
HI_S32  TRACK_SetNRAttr(SND_CARD_STATE_S* pCard, HI_U32 u32TrackID,HI_UNF_AI_NR_ATTR_S *pstNRParam);
HI_S32  TRACK_Duplicate2Aop(SND_CARD_STATE_S* pCard, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_U32 u32TrackID, HI_BOOL bEnable);
HI_S32  TRACK_SetSource(SND_CARD_STATE_S* pCard, HI_U32 u32TrackID, HI_UNF_SND_SOURCE_E enSource);
#endif

HI_S32 Track_ForceAllToPcm(SND_CARD_STATE_S* pCard);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  // __HI_AO_TRACK_H__
