/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_impl_gst.h
 * @brief   基于gstreamer实现的sme player controller
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
#ifndef __SMEC_MEDIAPLAYER_H__
#define __SMEC_MEDIAPLAYER_H__

#include "osal_type.h"
#include "osal_thread.h"
#include "sme_timer.h"
#include "sme_media_type.h"
#include "sme_player_msg.h"
#include "sme_player_inf.h"
#include "sme_benchmark.h"

#ifdef __ENABLE_OTTOPT__
#include "msg_builder_cache_query.h"
#include "imsgchannel_app.h"
#endif

class SmeMsgBuilderObsQueryCache;

//todo: pepare时考虑stop及reset速度返回。
class SmePlayerImplGst : public ISmePlayer
{
    friend class SmePlayerMsgRunner;
#ifdef __ENABLE_OTTOPT__
    friend class SmeMsgBuilderObsQueryCache;
#endif
private:
    typedef enum _tagPlayerImplGstState
    {
        E_PLAYER_STATE_UNINIT = 0,
        E_PLAYER_STATE_INITED,
        E_PLAYER_STATE_PREPARING,
        E_PLAYER_STATE_PREPARED,
        E_PLAYER_STATE_STARTED,
        E_PLAYER_STATE_PAUSED,
        E_PLAYER_STATE_ERROR,
    }E_PLAYER_STATE;

public:
    SmePlayerImplGst();
    virtual ~SmePlayerImplGst();

    //Begin derive from ISmePlayer
    virtual V_UINT32 SetObs(IN const ISmePlayerObs* pobjObs);

    //先设置datasource，再设置surface或者rect
    virtual V_UINT32 SetDataSource(IN const V_CHAR* pstURI);
    virtual V_UINT32 SetSubDataSource(IN const V_CHAR* pstSubURI);
    virtual V_UINT32 RegisterSubCallback (IN ST_SubCallback & stSubCB);
    virtual V_UINT32 SetSurface(IN const V_VOID* pvSurface);
    virtual V_UINT32 SetVDispRect(IN V_INT32 i32StartX,
                                IN V_INT32 i32StartY,
                                IN V_INT32 i32Width,
                                IN V_INT32 i32Height);
    virtual V_UINT32 PrepareAsync();
    virtual V_UINT32 Prepare();
    virtual V_UINT32 Start();
    virtual V_UINT32 Stop();
    virtual V_UINT32 Pause();
    virtual V_UINT32 Resume();
    virtual V_UINT32 Reset();
    virtual V_UINT32 SeekTo(IN V_INT32 i32Ms);
    virtual V_INT32 GetDuration();
    virtual V_INT32 GetCurrentPosition();
    virtual V_INT32 IsPlaying();
    virtual V_INT32 GetVideoWidth();
    virtual V_INT32 GetVideoHeight();
    virtual E_SME_PLAYER_STATE GetPlayerState();
    virtual V_UINT32 SetDisplayMode(IN E_SME_PLAYER_DISPLAY_MODE eMode, IN const ST_DISP_RATIO* pstDispRatio);
    virtual V_UINT32 SetChannelChangeMode (IN E_SME_PLAYER_SWITCH_CHANNEL_MODE eSwitchMode);
    virtual V_UINT32 SetDisplayAngle(IN E_SME_PLAYER_DISPLAY_ANGLE eAngle);
    virtual V_UINT32 GetMetadata(INOUT SmeMetadata& robjSmeMetadata);
    virtual V_UINT32 SetProperty(IN SmeProperty& robjProp);
    virtual V_UINT32 GetProperty(IN SmeProperty& robjRequest,
                                INOUT SmeProperty& robjReply);
    virtual V_UINT32 SetPlayMode(IN E_SME_PLAYER_MODE eMode);
    virtual V_UINT32 GetCurrentVideoFrame(INOUT V_INT32 ai32Array[],
                                IN V_INT32 i32Size,
                                IN V_INT32 i32Type);
    virtual V_UINT32 SetBitrate(IN V_UINT32 bitrate);
    virtual V_UINT32 SetUserAgent(IN const V_CHAR* pstUserAgent);
    virtual V_UINT32 SetByteRange(IN const V_BOOL bByteRange);
    virtual V_UINT32 SetCookies(IN V_CHAR** pstSetCookies);
    virtual V_UINT32 SetReferer(IN const V_CHAR* pstSetReferer);
    virtual V_UINT32 GetBandwidth();
    virtual V_UINT32 UpdateProcControlInfo(IN ST_PROC_COMMON_INFO * pstControlPointer);
    virtual V_UINT32 RegManifestCompleteCb (IN PFN_StreamPlayModeCb pfnStreamStartModeCb,
                                IN V_VOID* pvCtx);
    virtual V_UINT32 RegBitrateChangedCb (IN PFN_BitrateChangedCb pfnBtChangedCb,
                                          IN V_VOID* pvCtx);
#ifdef __LINUX__
    virtual V_UINT32 SetAudioMute(IN const V_UINT32 mute);
    virtual V_UINT32 GetAudioMute(INOUT V_UINT32*mute);
    virtual V_UINT32 SetAudioVolume(IN const V_UINT32 volume);
    virtual V_UINT32 GetAudioVolume(INOUT V_UINT32*volume);
    virtual V_UINT32 SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode);
    virtual V_UINT32 GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E* mode);
#endif
    virtual V_UINT32 SetAudioHdmiMode(IN const V_CHAR *pstAudioHdmiMode);
    virtual V_UINT32 GetAudioHdmiMode(INOUT V_CHAR **pstAudioHdmiMode);

    virtual V_UINT32 SetBufferTime(IN V_UINT32 u32BufferingTime);
    virtual V_UINT32 SetBufferWaterMark(IN V_UINT32 u32HighPercent, IN V_UINT32 u32LowPercent);
    virtual V_UINT32 GetBufferStatus(INOUT ST_BUFFER_STATUS *BufferStatus);
    virtual V_UINT32 GetVideoTrackInfo(
        INOUT ST_VIDEO_TRACK_INFO *VideoTrackInfo);
    virtual V_UINT32 GetAudioTrackInfo(
        INOUT ST_AUDIO_TRACK_INFO *AudioTrackInfo);
    virtual V_UINT32 SetAudioTrack(V_INT32 i32AudioTrack);
    virtual V_UINT32 GetCurrentBitrate(INOUT ST_CURRENT_BITRATE *CurrentBitrate);
    virtual V_UINT32 GetVideoDispMode(INOUT E_SME_PLAYER_DISPLAY_MODE& reMode, INOUT ST_DISP_RATIO& rstRatio);
    virtual V_UINT32 GetVideoDispRect(INOUT ST_SME_DISP_RECT& rstRect);

    //End derive from ISmePlayer
private:
    static V_VOID SyncCmdCompletedCb(V_VOID* pvCtx);

    V_VOID OnMsgComing(SmePlayerMsgBase* pobjMsg);
    V_VOID FreeMsg(SmePlayerMsgBase* pobjMsg) const;

    V_VOID OnCmdComing(SmePlayerMsgBase* pobjMsg);
    V_VOID OnNotifyComing(SmePlayerMsgBase* pobjMsg);

    V_UINT32 HandleSetObs(const SmePlayerMsgSetObs* pobjMsg);
    V_UINT32 HandleSetDs(const SmePlayerMsgSetDs* pobjMsg);
    V_UINT32 HandleSetSf(const SmePlayerMsgSetSf* pobjMsg);
    V_UINT32 HandleSetVDispRect(const SmePlayerMsgSetVDispRect* pobjMsg);
    V_UINT32 HandlePrepareAsync(SmePlayerMsgPrepareAsync* pobjMsg);
    V_UINT32 HandlePrepare(SmePlayerMsgPrepare* pobjMsg);
    V_UINT32 HandleStart(SmePlayerMsgStart* pobjMsg);
    V_UINT32 HandleStop(SmePlayerMsgStop* pobjMsg);
    V_UINT32 HandlePause(SmePlayerMsgPause* pobjMsg);
    V_UINT32 HandleResume(SmePlayerMsgResume* pobjMsg);
    V_UINT32 HandleReset(SmePlayerMsgReset* pobjMsg);
    V_UINT32 HandleSeekTo(const SmePlayerMsgSeekTo* pobjMsg);
    V_UINT32 HandleGetDuration(SmePlayerMsgGetDur* pobjMsg);
    V_UINT32 HandleGetCurPosition(SmePlayerMsgGetCurPos* pobjMsg);
    V_UINT32 HandleIsPlaying(SmePlayerMsgIsPlay* pobjMsg)  const;
    V_UINT32 HandleGetVideoWidth(SmePlayerMsgGetVideoWidth* pobjMsg) const;
    V_UINT32 HandleGetVideoHeight(SmePlayerMsgGetVideoHeight* pobjMsg) const;
    V_UINT32 HandleSetProp(const SmePlayerMsgSetProp* pobjMsg);
    V_UINT32 HandleGetProp(const SmePlayerMsgGetProp* pobjMsg);
    V_UINT32 HandleSetSubDs(const SmePlayerMsgSetSubDs* pobjMsg);
    V_UINT32 HandleRegSubCb(const SmePlayerMsgRegSubCb* pobjMsg);
    V_UINT32 HandleSetBitrate(const SmePlayerMsgSetBitrate* pobjMsg);
    V_UINT32 HandleSetUserAgent(const SmePlayerMsgSetUserAgent* pobjMsg);
    V_UINT32 HandleSetByteRange(const SmePlayerMsgSetByteRange* pobjMsg);
    V_UINT32 HandleSetCookies(SmePlayerMsgSetCookies* pobjMsg);
    V_UINT32 HandleSetReferer(const SmePlayerMsgSetReferer* pobjMsg);
    V_UINT32 HandleRegManifestCompleteCb(const SmePlayerMsgRegManifestCompleteCb* pobjMsg);
    V_UINT32 HandleGetBandwidth(SmePlayerMsgGetBandwidth* pobjMsg);
    V_UINT32 HandleRegBtChangedCb(const SmePlayerMsgRegBtChangedCb* pobjMsg);
    V_UINT32 HandleSetAudioMute(const SmePlayerMsgSetAudioMute *pobjMsg);
    V_UINT32 HandleGetAudioMute(SmePlayerMsgGetAudioMute *pobjMsg);
    V_UINT32 HandleSetAudioVolume(const SmePlayerMsgSetAudioVolume *pobjMsg);
    V_UINT32 HandleGetAudioVolume(SmePlayerMsgGetAudioVolume *pobjMsg);
    V_UINT32 HandleSetAudioChannelMode(const SmePlayerMsgSetAudioChannelMode *pobjMsg);
    V_UINT32 HandleGetAudioChannelMode(SmePlayerMsgGetAudioChannelMode *pobjMsg);
    V_UINT32 HandleSetAudioHdmiMode(const SmePlayerMsgSetAudioHdmiMode *pobjMsg);
    V_UINT32 HandleGetAudioHdmiMode(SmePlayerMsgGetAudioHdmiMode *pobjMsg);
    V_UINT32 HandleGetBufferStatus(SmePlayerMsgGetBufferStatus* pobjMsg);
    V_UINT32 HandleSetDisplayMode(const SmePlayerMsgSetDisplayMode* pobjMsg);
    V_UINT32 HandleSetLoop(const SmePlayerMsgSetLoop* pobjMsg);
    V_UINT32 HandleGetVideoTrackInfo(SmePlayerMsgGetVideoTrackInfo* pobjMsg);
    V_UINT32 HandleGetAudioTrackInfo(SmePlayerMsgGetAudioTrackInfo* pobjMsg);
    V_UINT32 HandleSetAudioTrack(SmePlayerMsgSetAudioTrack *pobjMsg);
    V_UINT32 HandleSetChannelChangeMode(const SmePlayerMsgSetSwitchChannelMode* pobjMsg);
    V_UINT32 HandleGetCurrentBitrate(SmePlayerMsgGetBitrate* pobjMsg);
    V_UINT32 HandleGetVideoDispMode(SmePlayerMsgGetVideoDispMode* pobjMsg);
    V_UINT32 HandleGetVideoDispRect(SmePlayerMsgGetVideoDispRect* pobjMsg);


    static V_VOID GstMsgHandler(IN V_VOID* pvCtx,
                                IN V_UINT32 u32Msg,
                                IN V_UINT32 u32Ext1,
                                IN V_UINT32 u32Ext2);
    static V_BOOL IsCacheCtrlEnable(IN V_VOID* pvCtx, IN const V_CHAR* pszURI);
    V_BOOL IsPlayingHls();
    V_UINT32 NoteCacheCtrl();
    V_UINT32 HandleCacheCtrl(SmePlayerMsgCacheCtrl* pobjMsg);
    V_UINT32 StartCacheProcTimer();
    V_VOID StopCacheProcTimer();
    static V_VOID CacheTimerProcFunc(IN V_VOID* pvCtx);
    V_VOID CacheTimerProc();
    /* BEGIN: Added for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */
    V_UINT32 CheckForSeek(const SmePlayerMsgSeekTo* pobjMsg);
    /* END:   Added for DTS2014091706218 by liurongliang(l00180035), 2014/9/22 */
    V_VOID SetPlayerState(E_PLAYER_STATE eState);
    virtual V_UINT32 SetLoop(IN V_BOOL bLoop);

private:
    ISmePlayerObs*                  m_pobjPlayerObs;    //应用的oberserver
    V_CHAR*                         m_pszURI;           //URI
    V_VOID*                         m_pvSurface;        //显示的surface对象
    ST_SME_DISP_RECT                m_stVDispRect;      //显示的区域，与m_pvSurface互斥存在，优先m_pvSurface,
                                                        //如3719M上就是这个方式，标准的Android就是surface方式
    E_SME_PLAYER_DISPLAY_MODE       m_eVDispMode;       //显示模式，fitin fitout等
    ST_DISP_RATIO                   m_stDispRatio;

    E_SME_PLAYER_DISPLAY_ANGLE      m_eVDispAngle;      //显示旋转，镜像等
    E_PLAYER_STATE                  m_eState;           //播放器当前状态.
    SmePlayerMsgRunner*             m_pobjCmdRunner;    //命令队列处理器
    V_VOID*                         m_pvGstPipeline;    //gst wrap pipeline
    SmePlayerMsgRunner*             m_pobjMsgRunner;    //通知消息处理器
    E_SME_PLAYER_LOG_LEVEL          m_eLogLevel;        //日志级别
    V_CHAR*                         m_pszPlgFullPath;   //加载库全路径
    V_BOOL                          m_bCacheCtrlEnable; //是否开启缓存控制。网络播放时开启。
    V_BOOL                          m_bBuffering;       //是否缓存。
    V_UINT32                        m_u32BufferFull;    //最大缓存量。
    V_UINT32                        m_u32BufferThdDown; //缓存下门限。
    V_UINT32                        m_u32BufferThdUp;   //缓存上门限。
    V_UINT64                        m_u64LastUpdateThdUpTime;//上一次更新ThdUp的时间
    V_INT32                         m_i32SeekTo;        //seek时间，单位MS。
    V_UINT32                        m_mute_status;
    ST_VOS_THD_MUTEX                m_stMutex;          //内部成员操作锁，目前主要是seek
    ICS::Timer                      m_objTimer;         //缓存信息处理定时器。
    V_BOOL                          m_bPreParedNormal;  //wrap notify了prepared，此时由buffering->buffered才上报prepared。
    V_BOOL                          m_bSeeking;         //是否正在seeking.
    V_BOOL                          m_bPausedInternal;  //是否内部pause了，seek之前要start 由内部pause的。
    /** BEGIN: Added for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */
    V_INT32                         m_i32Duration;      //码流duration，直播为-1
    /** END:   Added for DTS2014081802192 by liurongliang(l00180035), 2014/8/18 */
    V_CHAR*                         m_pszSubURI;        //subtitle URI
    ST_SubCallback                  m_stSubCb;          //sub callback func.
    V_UINT32                        m_u32Bandwidth;      //
    V_UINT32                        m_u32BufferLowPercent;
    V_UINT32                        m_u32BufferHighPercent;
    /** BEGIN: Added for DTS2015102708085 by baihaili(b00311320), 2015/10/26 */
    V_UINT32                        m_u32NotifyPercent;  //make sure percent increases in buffering state
    /** END:   Added by baihaili(b00311320), 2015/10/26 */
#ifdef __ENABLE_OTTOPT__
    IMsgChannelApp*                 m_pobjMsgChnApp;    //消息通道句柄
    MsgBuilderCacheQuery*           m_pobjCacheQuery;   //查询是否缓存的消息构造器
    /** BEGIN: Added for DTS2014101707996 by lizimian(l00251625), 2014/10/25 */
    IMsgBuilderObsCacheQuery*       m_pobjCacheQueryObs;
    V_BOOL                          m_bIsConnected;
    /** END  : Added for DTS2014101707996 by lizimian(l00251625), 2014/10/25 */
#endif
    V_BOOL                          m_bLoop;
    V_BOOL                          m_bLoopSeeking;//loop seeking cannot callback to app.
#ifdef  SMEPLAYER_BENCHMARK_OPEN
    SmeBenchMark                    m_objBenchMark;
#endif
    ST_PROC_COMMON_INFO *           m_procCommonInfo;
    V_UINT32                        m_u32ResolutionWidth;
    V_UINT32                        m_u32ResolutionHeight;
    V_UINT64                        m_u64StarTime;
    V_UINT64                        m_u64ResetTime;
    V_UINT64                        m_u64PrepareTime;
    V_UINT64                        m_u64FccTime;
    V_UINT64                        m_u64SeekTime;
};

#endif //__SMEC_MEDIAPLAYER_H__
