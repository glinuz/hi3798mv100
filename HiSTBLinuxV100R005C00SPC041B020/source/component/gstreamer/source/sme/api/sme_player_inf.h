/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_interface.h
 * @brief   sme player 接口定义
 * @date    2014/3/25
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/25
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SME_PLAYER_INF_H__
#define __SME_PLAYER_INF_H__

#include "osal_type.h"
#include "sme_property.h"
#include "sme_metadata.h"
#include "sme_player_type.h"
#include "sme_media_type.h"



class ISmePlayerObs
{
public:
    virtual ~ISmePlayerObs(){};
    virtual V_UINT32 PlayerNotify(V_UINT32 u32What,
                                V_UINT32 u32Extra1,
                                V_UINT32 u32Extra2) = 0;
};

class ISmePlayer
{
public:
    static ISmePlayer* CreateSmePlayer();
    static V_VOID DestroySmePlayer(INOUT ISmePlayer* pobjPlayer);
    static const V_CHAR* GetSmeVersionInfo();
    virtual ~ISmePlayer(){};
    virtual V_UINT32 SetObs(IN const ISmePlayerObs* pobjObs) = 0;
    virtual V_UINT32 SetDataSource(IN const V_CHAR* pstURI) = 0;
    virtual V_UINT32 SetSubDataSource(IN const V_CHAR* pstSubURI) = 0;
    virtual V_UINT32 RegisterSubCallback (IN ST_SubCallback & stSubCB) = 0;
    virtual V_UINT32 SetSurface(IN const V_VOID* pvSurface) = 0;
    virtual V_UINT32 SetVDispRect(IN V_INT32 i32StartX,
                                IN V_INT32 i32StartY,
                                IN V_INT32 i32Width,
                                IN V_INT32 i32Height) = 0;
    virtual V_UINT32 PrepareAsync() = 0;
    virtual V_UINT32 Prepare() = 0;
    virtual V_UINT32 Start() = 0;
    virtual V_UINT32 Stop() = 0;
    virtual V_UINT32 Pause() = 0;
    virtual V_UINT32 Resume() = 0;
    virtual V_UINT32 Reset() = 0;
    virtual V_UINT32 SeekTo(IN V_INT32 i32Ms) = 0;
    virtual V_INT32 GetDuration() = 0;
    virtual V_INT32 GetCurrentPosition() = 0;
    virtual V_INT32 IsPlaying() = 0;
    virtual V_INT32 GetVideoWidth() = 0;
    virtual V_INT32 GetVideoHeight() = 0;
    virtual E_SME_PLAYER_STATE GetPlayerState() = 0;
    virtual V_UINT32 SetDisplayMode(IN E_SME_PLAYER_DISPLAY_MODE eMode, IN const ST_DISP_RATIO* pstDispRatio) = 0;
    virtual V_UINT32 SetChannelChangeMode(IN E_SME_PLAYER_SWITCH_CHANNEL_MODE eSwitchMode) = 0;
    virtual V_UINT32 SetDisplayAngle(IN E_SME_PLAYER_DISPLAY_ANGLE eAngle) = 0;
    virtual V_UINT32 GetMetadata(INOUT SmeMetadata& robjSmeMetadata) = 0;
    virtual V_UINT32 SetProperty(IN SmeProperty& robjProp) = 0;
    virtual V_UINT32 GetProperty(IN SmeProperty& robjRequest,
                                INOUT SmeProperty& robjReply) = 0;
    virtual V_UINT32 SetPlayMode(IN E_SME_PLAYER_MODE eMode) = 0;
    virtual V_UINT32 GetCurrentVideoFrame(INOUT V_INT32 ai32Array[],
                                        IN V_INT32 i32Size,
                                        IN V_INT32 i32Type) = 0;

    virtual V_UINT32 SetBitrate(IN V_UINT32 bitrate) = 0;
    virtual V_UINT32 GetBandwidth() = 0;
    virtual V_UINT32 RegManifestCompleteCb (IN PFN_StreamPlayModeCb pfnStreamStartModeCb,
                                            IN V_VOID* pvCtx) = 0;
    virtual V_UINT32 RegBitrateChangedCb (IN PFN_BitrateChangedCb pfnBtChangedCb,
                                           IN V_VOID* pvCtx) = 0;
#ifdef __LINUX__
    virtual V_UINT32 SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode) = 0;
    virtual V_UINT32 GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E* mode) = 0;
#endif
    virtual V_UINT32 SetAudioHdmiMode(IN const V_CHAR *pstAudioHdmiMode) = 0;
    virtual V_UINT32 GetAudioHdmiMode(INOUT V_CHAR **pstAudioHdmiMode) = 0;

    virtual V_UINT32 SetBufferTime(IN V_UINT32 u32BufferingTime) = 0;
    virtual V_UINT32 SetBufferWaterMark(IN V_UINT32 u32HighPercent, IN V_UINT32 u32LowPercent) = 0;
    virtual V_UINT32 GetBufferStatus(INOUT ST_BUFFER_STATUS *BufferStatus) = 0;
    virtual V_UINT32 GetAudioTrackInfo(
        INOUT ST_AUDIO_TRACK_INFO *AudioTrackInfo) = 0;
    virtual V_UINT32 SetAudioTrack(IN V_INT32 i32AudioTrack) = 0;
    virtual V_UINT32 GetVideoTrackInfo(
        INOUT ST_VIDEO_TRACK_INFO *VideoTrackInfo) = 0;

    /*
    @bLoop:ICS_TRUE for loop playback.
           ICS_FALSE for non loop playback.
           default if non loop.
    @return:ICS_SUCCESS, OTHERS for failed.
      this api can call by app after 'SetObs' and before 'Reset',
    if reset the flag will be cleared(loop will set to ICS_FALSE).
    when EOS comes, if the duration <= 0, it will callback unsupport loop playback.
    then callback EOS.
    */
    virtual V_UINT32 SetLoop(IN V_BOOL bLoop) = 0;
    virtual V_UINT32 SetUserAgent(IN const V_CHAR* pstUserAgent) = 0;
    virtual V_UINT32 SetByteRange(IN const V_BOOL bByteRange) = 0;
    virtual V_UINT32 SetCookies(IN  V_CHAR ** pstSetCookies) = 0;
    virtual V_UINT32 SetReferer(IN const V_CHAR* pstSetReferer) = 0;
    virtual V_UINT32 GetCurrentBitrate(INOUT ST_CURRENT_BITRATE *CurrentBitrate) = 0;
    virtual V_UINT32 GetVideoDispMode(INOUT E_SME_PLAYER_DISPLAY_MODE& reMode, INOUT ST_DISP_RATIO& rstRatio) = 0;
    virtual V_UINT32 GetVideoDispRect(INOUT ST_SME_DISP_RECT& rstRect) = 0;
};
#endif//__SME_PLAYER_INF_H__
