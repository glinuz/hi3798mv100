/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_cmds.h
 * @brief   sme player cmds 定义
 * @author
 * @date    2014/4/6
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/6
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SME_PLAYER_MSG_H__
#define __SME_PLAYER_MSG_H__

#include "osal_type.h"
#include "sme_msg_queue.h"
#include "sme_player_inf.h"


class SmePlayerImplGst;

class SmePlayerMsgBase
{
public:
    typedef enum _tagMsgType
    {
        E_TYPE_SET_OBS = 0,
        E_TYPE_SET_DS,
        E_TYPE_SET_SF,
        E_TYPE_SET_VDISP_RECT,
        E_TYPE_PREPARE_ASYNC,
        E_TYPE_PREPARE,
        E_TYPE_START,
        E_TYPE_STOP,
        E_TYPE_PAUSE,
        E_TYPE_RESUME,
        E_TYPE_SEEKTO,
        E_TYPE_RESET,
        E_TYPE_GET_DURATION,
        E_TYPE_GET_CURPOSITION,
        E_TYPE_GET_ISPLAYING,
        E_TYPE_GET_VIDEO_WIDTH,
        E_TYPE_GET_VIDEO_HEIGHT,
        E_TYPE_SET_PROP,
        E_TYPE_GET_PROP,
        E_TYPE_CACHE_CTRL,
        E_TYPE_SET_SUB_DS,
        E_TYPE_REG_SUB_CB,
        E_TYPE_SET_STREAM_BITRATE,
        E_TYPE_SET_REG_MANIFEST_COMPLETE_CB,
        E_TYPE_SET_REG_BITRATE_CHANGED_CB,
        E_TYPE_GET_BANDWIDTH,
        E_TYPE_SET_AUDIO_MUTE,
        E_TYPE_GET_AUDIO_MUTE,
        E_TYPE_SET_AUDIO_VOLUME,
        E_TYPE_GET_AUDIO_VOLUME,
        E_TYPE_SET_AUDIO_CHANNEL_MODE,
        E_TYPE_GET_AUDIO_CHANNEL_MODE,
        E_TYPE_SET_AUDIO_HDMI_MODE,
        E_TYPE_GET_AUDIO_HDMI_MODE,
        E_TYPE_GET_BUFFER_STATUS,
        E_TYPE_SET_DISPMODE,
        E_TYPE_SET_LOOP,
        E_TYPE_GET_VIDEO_TRACK_INFO,
        E_TYPE_GET_AUDIO_TRACK_INFO,
        E_TYPE_SET_AUDIO_TRACK,
        E_TYPE_SET_SWITCH_CHANNEL_MODE,
        E_TYPE_GET_DISPMODE,
        E_TYPE_GET_DISPRECT,
//------------------------------------------------------------------//
//NOTIFY
        E_TYPE_ON_PREPARED,
        E_TYPE_ON_EOS,
        E_TYPE_ON_SEEKCOMPLTED,
        E_TYPE_ON_ERROR,
        E_TYPE_ON_INFO,
        E_TYPE_ON_BUFFER_UPDATE,
        E_TYPE_ON_RESOLUTION_CHANGED,
        E_TYPE_ON_HTTP_ERR,
        E_TYPE_ON_FRAGMENT_INFO_UPDATED,
        E_TYPE_BUTT,
        E_TYPE_SET_USER_AGENT,
        E_TYPE_SET_BYTE_RANGE,
        E_TYPE_SET_COOKIES,
        E_TYPE_SET_REFERER,
        E_TYPE_GET_BITRATE,
    }E_TYPE;

    typedef enum _tagMsgErr
    {
        E_MSG_ERR_NONE = 0,
        E_MSG_ERR_TIMEOUT,
        E_MSG_ERR_INVALID_ARGS,
        E_MSG_ERR_UNKNOWN,
    }E_MSG_ERR;

    typedef V_VOID (*pfnSyncMsgCompletedCb)(INOUT V_VOID* pvCtx);

    SmePlayerMsgBase(IN E_TYPE eType,
                    IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_BOOL bNotify = ICS_FALSE);
    virtual ~SmePlayerMsgBase();

    virtual E_TYPE GetType() const;
    virtual V_VOID SetResult(V_UINT32 u32Res);
    virtual V_UINT32 GetResult() const;
    virtual V_VOID SyncMsgCompleted();
    virtual V_BOOL IsNotify() const;
    virtual E_MSG_ERR WaitMsg(ST_VOS_THD_MUTEX* pszMutex, V_UINT32 u32TimeOut);
    virtual V_VOID SignalMsg(ST_VOS_THD_MUTEX* pszMutex);

protected:
    E_TYPE                  m_eType;        //命令类型
    V_UINT32                m_u32Res;       //同步命令执行结果
    VOSCONDHDL              m_hdlCond;      //同步命令条件事件
    V_BOOL                  m_bNotify;      //是否向上通知
private:
    SmePlayerMsgBase();

private:
    pfnSyncMsgCompletedCb   m_pfnCb;        //同步命令回调
    V_VOID*                 m_pvCtx;        //同步命令回调context

};

class SmePlayerMsgSetObs : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetObs(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const ISmePlayerObs* pobjObs);
    virtual ~SmePlayerMsgSetObs();

    const ISmePlayerObs* GetPlayerObs() const;

private:
    const ISmePlayerObs* m_pobjObs;
};

class SmePlayerMsgSetDs : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetDs(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_CHAR* pszURI);
    virtual ~SmePlayerMsgSetDs();

    const V_CHAR* GetUri() const;

private:
    const V_CHAR* m_pszUri;
};

class SmePlayerMsgSetSf : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetSf(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_VOID* pvSf);
    virtual ~SmePlayerMsgSetSf();

    const V_VOID* GetSf() const;

private:
    const V_VOID* m_pvSf;
};

class SmePlayerMsgSetVDispRect : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetVDispRect(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx,
                            IN V_INT32 i32StartX,
                            IN V_INT32 i32StartY,
                            IN V_INT32 i32Width,
                            IN V_INT32 i32Height);
    virtual ~SmePlayerMsgSetVDispRect();

    V_INT32 GetStartX() const;
    V_INT32 GetStartY() const;
    V_INT32 GetWidth() const;
    V_INT32 GetHeight() const;

private:
    V_INT32 m_i32StartX;
    V_INT32 m_i32StartY;
    V_INT32 m_i32Width;
    V_INT32 m_i32Height;
};

class SmePlayerMsgPrepareAsync : public SmePlayerMsgBase
{
public:
    SmePlayerMsgPrepareAsync(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgPrepareAsync();
};

class SmePlayerMsgPrepare : public SmePlayerMsgBase
{
public:
    SmePlayerMsgPrepare(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgPrepare();
};

class SmePlayerMsgStart : public SmePlayerMsgBase
{
public:
    SmePlayerMsgStart(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgStart();
};

class SmePlayerMsgStop : public SmePlayerMsgBase
{
public:
    SmePlayerMsgStop(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgStop();
};

class SmePlayerMsgPause : public SmePlayerMsgBase
{
public:
    SmePlayerMsgPause(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgPause();
};

class SmePlayerMsgResume : public SmePlayerMsgBase
{
public:
    SmePlayerMsgResume(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgResume();
};

class SmePlayerMsgSeekTo : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSeekTo(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx,
                            IN V_INT32 i32SeekToMs);
    virtual ~SmePlayerMsgSeekTo();

    V_INT32 GetSeekToMs() const;

private:
    V_INT32 m_i32SeekToMs;
};

class SmePlayerMsgReset : public SmePlayerMsgBase
{
public:
    SmePlayerMsgReset(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgReset();
};

class SmePlayerMsgGetDur : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetDur(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgGetDur();
};

class SmePlayerMsgGetCurPos : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetCurPos(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgGetCurPos();
};

class SmePlayerMsgIsPlay : public SmePlayerMsgBase
{
public:
    SmePlayerMsgIsPlay(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgIsPlay();
};

class SmePlayerMsgGetVideoWidth : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetVideoWidth(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgGetVideoWidth();
};

class SmePlayerMsgGetVideoHeight : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetVideoHeight(IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgGetVideoHeight();
};

class SmePlayerMsgSetBitrate : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetBitrate(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32Bitrate );
    virtual ~SmePlayerMsgSetBitrate();
    V_UINT32 GetBitrate() const;
private:
    V_UINT32 m_u32Bitrate;
};

class SmePlayerMsgSetUserAgent : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetUserAgent(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN const V_CHAR * pstUserAgent);
    virtual ~SmePlayerMsgSetUserAgent();
    const V_CHAR * GetUserAgent() const;
private:
    const V_CHAR * m_pstUserAgent;
};
class SmePlayerMsgSetReferer : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetReferer(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN const V_CHAR * pstUserAgent);
    virtual ~SmePlayerMsgSetReferer();
    const V_CHAR * GetReferer() const;
private:
    const V_CHAR * m_pstSetReferer;
};

class SmePlayerMsgSetCookies : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetCookies(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_CHAR ** pstSetCookies);
    virtual ~SmePlayerMsgSetCookies();
    V_CHAR ** GetCookies();
private:
    V_CHAR ** m_pstSetCookies;
};


class SmePlayerMsgSetByteRange: public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetByteRange(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_BOOL bByteRange);
    virtual ~SmePlayerMsgSetByteRange();
    V_BOOL GetByteRange() const;
private:
    V_BOOL m_bByteRange;
};

class SmePlayerMsgRegManifestCompleteCb : public SmePlayerMsgBase
{
public:
    SmePlayerMsgRegManifestCompleteCb(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN PFN_StreamPlayModeCb pfPlayModeCb,
                           IN V_VOID* pvModeCbCtx);
    virtual ~SmePlayerMsgRegManifestCompleteCb();
    PFN_StreamPlayModeCb GetManifestCompleteCb()const;
    V_VOID* GetCbCtx()const;
private:
    PFN_StreamPlayModeCb m_pfPlayModeCb;
    V_VOID* m_pvModeCbCtx;
};


class SmePlayerMsgRegBtChangedCb : public SmePlayerMsgBase
{
public:
    SmePlayerMsgRegBtChangedCb(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN PFN_BitrateChangedCb pfnBtCb,
                           IN V_VOID* pvCbCtx);
    virtual ~SmePlayerMsgRegBtChangedCb();
    PFN_BitrateChangedCb GetBtChangedCb()const;
    V_VOID* GetCbCtx()const;
private:
    PFN_BitrateChangedCb   m_pfBtChangedCb;
    V_VOID* m_pvChangedCbCtx;
};

class SmePlayerMsgGetBandwidth : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetBandwidth (IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgGetBandwidth ();
};

class SmePlayerMsgSetProp : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetProp(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const SmeProperty* pobjSmeProp);
    virtual ~SmePlayerMsgSetProp();

    const SmeProperty* GetProp() const;

private:
    const SmeProperty* m_pobjProp;
};

class SmePlayerMsgGetProp : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetProp(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const SmeProperty* pobjReqProp, INOUT SmeProperty* pobjRespProp);
    virtual ~SmePlayerMsgGetProp();

    const SmeProperty* GetReqProp() const;
    SmeProperty* GetRespProp() const;

private:
    const SmeProperty* m_pobjReqProp;
    SmeProperty* m_pobjRespProp;
};

class SmePlayerMsgCacheCtrl : public SmePlayerMsgBase
{
public:
    SmePlayerMsgCacheCtrl(IN pfnSyncMsgCompletedCb pfnCb, IN V_VOID* pvCtx);
    virtual ~SmePlayerMsgCacheCtrl();
};

class SmePlayerMsgSetSubDs : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetSubDs(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_CHAR* pszSubURI);
    virtual ~SmePlayerMsgSetSubDs();

    const V_CHAR* GetSubUri() const;

private:
    const V_CHAR* m_pszSubUri;
};

class SmePlayerMsgRegSubCb : public SmePlayerMsgBase
{
public:
    SmePlayerMsgRegSubCb(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const ST_SubCallback* pstSubCb);
    virtual ~SmePlayerMsgRegSubCb();

    const ST_SubCallback* GetSubCb() const;

private:
    const ST_SubCallback* m_pstSubCb;
};

class SmePlayerMsgSetAudioMute : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetAudioMute(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_UINT32 mute);
    virtual ~SmePlayerMsgSetAudioMute();

    const V_UINT32 GetAudioMute() const;

private:
    const V_UINT32 m_mute;
};

class SmePlayerMsgGetAudioMute : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetAudioMute(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_UINT32* mute);
    virtual ~SmePlayerMsgGetAudioMute();

    V_UINT32* GetAudioMute();

private:
    V_UINT32* m_mute;
};

class SmePlayerMsgSetAudioVolume : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetAudioVolume(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_UINT32 volume);
    virtual ~SmePlayerMsgSetAudioVolume();

    const V_UINT32 GetAudioVolume() const;

private:
    const V_UINT32 m_volume;
};

class SmePlayerMsgGetAudioVolume : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetAudioVolume(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_UINT32* volume);
    virtual ~SmePlayerMsgGetAudioVolume();

    V_UINT32* GetAudioVolume();

private:
    V_UINT32* m_volume;
};

class SmePlayerMsgSetAudioChannelMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetAudioChannelMode(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const A_TRACK_CHANNEL_MODE_E mode);
    virtual ~SmePlayerMsgSetAudioChannelMode();

    const A_TRACK_CHANNEL_MODE_E GetAudioChannelMode() const;

private:
    const A_TRACK_CHANNEL_MODE_E m_mode;
};

class SmePlayerMsgGetAudioChannelMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetAudioChannelMode(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN A_TRACK_CHANNEL_MODE_E* mode);
    virtual ~SmePlayerMsgGetAudioChannelMode();

    A_TRACK_CHANNEL_MODE_E* GetAudioChannelMode();

private:
    A_TRACK_CHANNEL_MODE_E* m_mode;
};


class SmePlayerMsgSetAudioHdmiMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetAudioHdmiMode(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN const V_CHAR* pszAudioHdmiMode);
    virtual ~SmePlayerMsgSetAudioHdmiMode();

    const V_CHAR* GetAudioHdmiMode() const;

private:
    const V_CHAR *m_pszAudioHdmiMode;
};

class SmePlayerMsgGetAudioHdmiMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetAudioHdmiMode(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_CHAR **pszAudioHdmiMode);
    virtual ~SmePlayerMsgGetAudioHdmiMode();

    V_CHAR **GetAudioHdmiMode();

private:
    V_CHAR **m_pszAudioHdmiMode;
};


class SmePlayerMsgGetBufferStatus: public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetBufferStatus(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    INOUT ST_BUFFER_STATUS *pstBufferStatus);

    virtual ~SmePlayerMsgGetBufferStatus();

    ST_BUFFER_STATUS *GetBufferStatus();
private:
    ST_BUFFER_STATUS *m_pstBufferStatus;
};

class SmePlayerMsgGetBitrate : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetBitrate (IN pfnSyncMsgCompletedCb pfnCb,
                            IN V_VOID* pvCtx,
                            INOUT ST_CURRENT_BITRATE *pstCurrentBitrate);
    virtual ~SmePlayerMsgGetBitrate ();

    ST_CURRENT_BITRATE *GetCurrentBitrate();
private:
    ST_CURRENT_BITRATE *m_pstCurrentBitrate;
};

class SmePlayerMsgGetVideoDispMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetVideoDispMode(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN E_SME_PLAYER_DISPLAY_MODE& reMode,
                    IN ST_DISP_RATIO& rstRatio);
    virtual ~SmePlayerMsgGetVideoDispMode();

    E_SME_PLAYER_DISPLAY_MODE* GetVideoDispMode();
    ST_DISP_RATIO* GetVideoDispRatio();

private:
    E_SME_PLAYER_DISPLAY_MODE* m_disp_mode;
    ST_DISP_RATIO* m_disp_ratio;
};

class SmePlayerMsgGetVideoDispRect : public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetVideoDispRect(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN ST_SME_DISP_RECT& rstRect);
    virtual ~SmePlayerMsgGetVideoDispRect();

    ST_SME_DISP_RECT* GetVideoDispRect();

private:
    ST_SME_DISP_RECT* m_disp_rect;
};

/* add by lidanjing for set W&H and display mode */
class SmePlayerMsgSetDisplayMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetDisplayMode(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32DispMode,
                           IN ST_DISP_RATIO* pstDispRatio);
    virtual ~SmePlayerMsgSetDisplayMode();
    V_VOID GetDisplayModeAndRadio(INOUT V_UINT32*u32DisplayMode,
        INOUT ST_DISP_RATIO** pstDispRatio) const;
private:
    V_UINT32 m_u32DisplayMode;
    ST_DISP_RATIO* m_pstDispRatio;

};

class SmePlayerMsgSetLoop: public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetLoop(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_BOOL bLoop);

    virtual ~SmePlayerMsgSetLoop();

    V_BOOL GetLoopVal() const;

private:
    V_BOOL m_bLoop;
};

class SmePlayerMsgGetVideoTrackInfo: public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetVideoTrackInfo(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    INOUT ST_VIDEO_TRACK_INFO *pstVideoTrackInfo);

    virtual ~SmePlayerMsgGetVideoTrackInfo();

    ST_VIDEO_TRACK_INFO *GetVideoTrackInfo();
private:
    ST_VIDEO_TRACK_INFO *m_pstVideoTrackInfo;
};

class SmePlayerMsgGetAudioTrackInfo: public SmePlayerMsgBase
{
public:
    SmePlayerMsgGetAudioTrackInfo(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    INOUT ST_AUDIO_TRACK_INFO *pstAudioTrackInfo);

    virtual ~SmePlayerMsgGetAudioTrackInfo();

    ST_AUDIO_TRACK_INFO *GetAudioTrackInfo();
private:
    ST_AUDIO_TRACK_INFO *m_pstAudioTrackInfo;
};

class SmePlayerMsgSetAudioTrack: public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetAudioTrack(IN pfnSyncMsgCompletedCb pfnCb,
                    IN V_VOID* pvCtx,
                    IN V_INT32 i32AudioTrack);

    virtual ~SmePlayerMsgSetAudioTrack();

    V_INT32 GetAudioTrack() const;
private:
    V_INT32 m_i32AudioTrack;
};

class SmePlayerMsgSetSwitchChannelMode : public SmePlayerMsgBase
{
public:
    SmePlayerMsgSetSwitchChannelMode(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32SwitchChannelMode);
    virtual ~SmePlayerMsgSetSwitchChannelMode();
    V_VOID GetSwitchChannelMode(INOUT V_UINT32*u32SwitchChannelMode) const;
private:
    V_UINT32 m_u32SwitchChannelMode;

};

//-----------------------------------------------------------
//Notify
class SmePlayerNotifyBase : public SmePlayerMsgBase
{
public:
    SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE eMsgType, E_SME_PLAYER_NOTIFY eNotify);
    virtual ~SmePlayerNotifyBase();

    virtual E_SME_PLAYER_NOTIFY GetNotifyType() const;

protected:
    E_SME_PLAYER_NOTIFY m_eNotifyType;
};

class SmePlayerNotifyOnPrepared : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnPrepared();
    virtual ~SmePlayerNotifyOnPrepared();
};

class SmePlayerNotifyOnEos : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnEos();
    virtual ~SmePlayerNotifyOnEos();
};

class SmePlayerNotifyOnError : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnError(E_SME_PLAYER_ERROR eError);
    virtual ~SmePlayerNotifyOnError();

    E_SME_PLAYER_ERROR GetErrorVal() const;

private:
    E_SME_PLAYER_ERROR m_eError;
};

class SmePlayerNotifyOnSeekCompleted : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnSeekCompleted(E_SME_PLAYER_ERROR eError, V_INT32 i32Ms);
    virtual ~SmePlayerNotifyOnSeekCompleted();

    E_SME_PLAYER_ERROR GetErrorVal() const;
    V_INT32 GetSeekRes() const;

private:
    E_SME_PLAYER_ERROR m_eError;
    V_INT32 m_i32Ms;
};

class SmePlayerNotifyOnInfo : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnInfo(V_UINT32 u32Ext1, V_UINT32 u32Ext2);
    virtual ~SmePlayerNotifyOnInfo();

    V_UINT32 GetExtVal1() const;
    V_UINT32 GetExtVal2() const;

private:
    V_UINT32 m_u32Ext1;
    V_UINT32 m_u32Ext2;
};

class SmePlayerNotifyOnBufferUpdate : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnBufferUpdate(V_UINT32 u32Percent);
    virtual ~SmePlayerNotifyOnBufferUpdate();

    V_UINT32 GetBufferedPercent() const;

private:
    V_UINT32 m_u32BufferedPercent;
};
class SmePlayerNotifyOnResolutionChange : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnResolutionChange(V_UINT32 u32Width, V_UINT32 u32Height);
    virtual ~SmePlayerNotifyOnResolutionChange();
    V_VOID GetResolution(V_UINT32 & u32Width, V_UINT32 & u32Height) const;
private:
    V_UINT32 m_u32Width;
    V_UINT32 m_u32Height;
};

class SmePlayerNotifyOnHttpErr: public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnHttpErr(V_UINT32 u32StatudCode);
    virtual ~SmePlayerNotifyOnHttpErr();
    V_VOID GetStatusCode(V_UINT32 & u32StatusCode) const;
private:
    V_UINT32 m_u32StatusCode;
};

//notify fragment info updated
class SmePlayerNotifyOnFragmentChanged : public SmePlayerNotifyBase
{
public:
    SmePlayerNotifyOnFragmentChanged(const ST_FRAGMENT_INFO *pstFragmentInfo);
    virtual ~SmePlayerNotifyOnFragmentChanged();
    V_VOID GetFragmentInfo(ST_FRAGMENT_INFO *pstFragmentInfo) const;
private:
    ST_FRAGMENT_INFO m_stFragmentInfo;
};


//---------------------------------------------------------------------------------
//msg runner
class SmePlayerMsgRunner : protected ICS::MsgQueue
{
public:
    SmePlayerMsgRunner(SmePlayerImplGst* pobjPlayer);
    virtual ~SmePlayerMsgRunner();
    V_UINT32 Init();
    V_VOID DeInit(V_BOOL bFreeCmd = ICS_TRUE);

    //同步
    V_UINT32 PostMsg(SmePlayerMsgBase* pobjCmd);

    //异步
    V_UINT32 SendMsg(SmePlayerMsgBase* pobjCmd);

private:

    //Begin derive from ICS::MsgQueue
    virtual V_VOID OnGetAMsg(IN V_VOID* pvMsg);

    /* 当消息队列被销毁时，如果还有剩余的消息，使用该接口释放消息 */
    virtual V_VOID OnFreeMsg(IN V_VOID* pvMsg);
    //End derive from ICS::MsgQueue

private:
    SmePlayerImplGst*   m_pobjPlayer;
    V_BOOL              m_bInited;
    ST_VOS_THD_MUTEX    m_stSyncMsgMutex;   //同步命令锁
};
#endif //__SME_PLAYER_CMDS_H__
