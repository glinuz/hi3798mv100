/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_gst_wrap.h
 * @brief   sme player gstsmeplaybin 封装静态类定义
 * @author
 * @date    2014/4/7
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/7
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SME_PLAYER_GST_WRAP_H__
#define __SME_PLAYER_GST_WRAP_H__

#include "osal_type.h"
#include "sme_player_type.h"
#include "sme_media_type.h"

class SmePlayerGstWrap
{
public:
    typedef V_VOID (*pfnGstMsgHandler)(IN V_VOID* pvCtx, IN V_UINT32 u32Msg,
        IN V_UINT32 u32Ext1, IN V_UINT32 u32Ext2);

    static V_UINT32 InitGst(INOUT V_VOID*& pvPipeline,
        IN pfnGstMsgHandler pfnHandler, IN V_VOID* pvCtx,
        IN E_SME_PLAYER_LOG_LEVEL eLevel, IN const V_CHAR* pszLogFileName = NULL,
        IN const V_CHAR* pszDynamicLoadLibs = NULL);

    static V_VOID DeInitGst(IN V_VOID* pvPipeline);

    static V_VOID ExitGstBlock(IN V_VOID* pvPipeline);

    static V_UINT32 Prepare(IN V_VOID* pvPipeline, IN const V_CHAR* pszUri,
        IN const V_VOID* pvSurface, IN V_UINT32 u32BufferFull,
        IN V_UINT32 u32LowPercent, IN V_UINT32 u32HighPercent,
        IN V_BOOL bRect = ICS_FALSE, IN V_BOOL bSync = ICS_FALSE,
        IN const ST_SubCallback* pstSubCb = NULL, IN const V_CHAR* pszSubUri = NULL);
    static V_UINT32 RegisterSubCb(IN V_VOID* pvPipeline, IN const ST_SubCallback* pstSubCb);
    static V_VOID Reset(IN V_VOID* pvPipeline);

    static V_UINT32 Start(IN V_VOID* pvPipeline);

    static V_UINT32 Pause(IN V_VOID* pvPipeline);

    static V_UINT32 Stop(IN V_VOID* pvPipeline);

    static V_UINT32 SeekTo(IN V_VOID* pvPipeline, INOUT V_INT32 i32Ms);

    static V_INT32 GetDuration(IN V_VOID* pvPipeline);//ms

    static V_INT32 GetCurPosition(IN V_VOID* pvPipeline);//ms

    static V_UINT32 GetCacheInfo(IN V_VOID* pvPipeline, INOUT V_UINT32& u32CurBw,
        INOUT V_UINT32& u32StreamBps, INOUT V_UINT32& u32ABuffered,
        INOUT V_UINT32& u32VBuffered, INOUT V_UINT32& u32VDecInterval,
        INOUT V_UINT32& u32VDecInDuration);

    static V_INT32 GetVideoWidth(IN V_VOID* pvPipeline);

    static V_INT32 GetVideoHeight(IN V_VOID* pvPipeline);

    static V_UINT32 SetDispRect(IN V_VOID* pvPipeline, IN const ST_SME_DISP_RECT& stDispRest);

    // audio mute
    static V_INT32 MuteAudio(IN V_VOID *pvPipeline);
    static V_INT32 UnmuteAudio(IN V_VOID *pvPipeline);
    static V_INT32 SetAudioMute(IN V_VOID *pvPipeline, IN V_UINT32 mute);
    static V_INT32 GetAudioMute(IN V_VOID *pvPipeline, INOUT V_UINT32* mute);

    // audio volume
    static V_INT32 SetAudioVolume(IN V_VOID *pvPipeline, IN V_UINT32 volume);
    static V_INT32 GetAudioVolume(IN V_VOID *pvPipeline, INOUT V_UINT32* volume);

    // audio channel mode
    static V_INT32 SetAudioChannelMode(IN V_VOID *pvPipeline, IN A_TRACK_CHANNEL_MODE_E mode);
    static V_INT32 GetAudioChannelMode(IN V_VOID *pvPipeline, INOUT A_TRACK_CHANNEL_MODE_E* mode);

    static V_UINT32 SetLogLevel(IN V_VOID* pvPipeline, E_SME_PLAYER_LOG_LEVEL eLevel);
    static V_UINT32 SetStreamBitrate(IN V_VOID* pvPipeline, IN V_UINT32 u32Bitrate);

    static V_VOID SetManifestCompleteCb(IN V_VOID* pvPipeline,
        IN PFN_StreamPlayModeCb pFnCb, IN V_VOID* pvCtx);
    static V_UINT32 GetBandwidth(IN V_VOID* pvPipeline);
    static V_VOID SetBtChangedCb(IN V_VOID* pvPipeline,
        IN PFN_BitrateChangedCb  pFnCb, IN V_VOID* pvCtx);

    static V_UINT32 SetAudioHdmiMode(IN V_VOID *pvPipeline,
                                    IN const V_INT8 *pi8HdmiMode);
    static V_UINT32 GetAudioHdmiMode(IN V_VOID *pvPipeline,
                                    INOUT V_INT8 **pi8HdmiMode);
    static V_UINT32 GetBufferStatus(IN V_VOID *pvPipeline,
                                    INOUT ST_BUFFER_STATUS *pstBufferStatus);
    static V_UINT32 SetDispMode(IN V_VOID *pvPipeline,
                                    IN V_UINT32 u32DisplayMode,
                                    IN ST_DISP_RATIO* pstDispRatio);
    static V_UINT32 SetSwitchChannelMode(IN V_VOID *pvPipeline, IN V_UINT32 u32SwitchChannelMode);
    static V_UINT32 GetDispMode(IN V_VOID *pvPipeline, IN V_UINT32* pDisplayMode, IN ST_DISP_RATIO** ppstDispRatio);

    static V_UINT32 GetVideoTrackInfo(
        IN V_VOID *pvPipeline,
        INOUT ST_VIDEO_TRACK_INFO *pstVideoTrackInfo);

    static V_UINT32 GetAudioTrackInfo(
        IN V_VOID *pvPipeline,
        INOUT ST_AUDIO_TRACK_INFO *pstAudioTrackInfo);

    static V_UINT32 SetActiveAudioTrack(
        IN V_VOID *pvPipeline,
        IN V_UINT32 u32AudioTrack);
    static V_UINT32 SetWrapUserAgent(IN V_VOID* pvPipeline, IN const V_CHAR* pstUserAgent);
    static V_UINT32 SetWrapByteRange(IN V_VOID* pvPipeline, IN V_BOOL bByteRange);
    static V_UINT32 SetWrapCookies(IN V_VOID* pvPipeline, IN V_CHAR** pstCookies);
    static V_UINT32 SetWrapReferer(IN V_VOID* pvPipeline, IN const V_CHAR* pstSetReferer);
    static V_UINT32 GetCurrentBitrate(IN V_VOID* pvPipeline,
                                      INOUT ST_CURRENT_BITRATE *pstCurrentBitrate);
    static V_VOID *GetPlaybin(IN V_VOID* pvPipeline);
    static V_INT32 GetVideoDispMode(IN V_VOID *pvPipeline,
    INOUT E_SME_PLAYER_DISPLAY_MODE* peMode,
    INOUT ST_DISP_RATIO* pstRatio);
    static V_INT32 GetVideoDispRect(IN V_VOID *pvPipeline, INOUT ST_SME_DISP_RECT* ppstRect);
};

#endif //__SME_PLAYER_GST_WRAP_H__
