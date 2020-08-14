/**************************************************************************//**

  Copyright (C), 2016-2026, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_dvbplayer_impl_gst.h
 * @brief   基于gstreamer实现的sme dvb player controller
 * @date    2016/4/20
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/4/20
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SMEC_DVBPLAYER_H__
#define __SMEC_DVBPLAYER_H__

#include "gst.h"
#include "gstsme.h"
#include "osal_type.h"
#include "osal_thread.h"
#include "sme_dvbplayer_inf.h"
#include "sme_benchmark.h"

class SmeDvbPlayerImpl : public ISmeDvbPlayer
{
private:
    typedef enum _tagDvbPlayerImplState
    {
        E_DVBPLAYER_STATE_UNINIT = 0,
        E_DVBPLAYER_STATE_INITED,
        E_DVBPLAYER_STATE_SRCSET,
        E_DVBPLAYER_STATE_PREPARING,
        E_DVBPLAYER_STATE_PREPARED,
        E_DVBPLAYER_STATE_STARTING,
        E_DVBPLAYER_STATE_STARTED,
        E_DVBPLAYER_STATE_PAUSED,
        E_DVBPLAYER_STATE_STOPPED,
        E_DVBPLAYER_STATE_RESETTING,
        E_DVBPLAYER_STATE_ERROR,
    }E_DVBPLAYER_STATE;

public:
    SmeDvbPlayerImpl(E_DVB_TYPE eDvbType = E_DVB_TYPE_LIVE);
    virtual                ~SmeDvbPlayerImpl();
    virtual V_UINT32       SetObs(IN const ISmeDvbPlayerObs* pobjObs);
    virtual V_UINT32       SetProgMediaInfo(IN const ST_DVBProgMediaInfo& rstProgMediaInfo);
    virtual V_UINT32       SetProgAudioInfo(IN const ST_DVBProgAudioInfo& rstProgAudioInfo);
    virtual V_UINT32       SetVDispRect(IN const ST_SME_DVBDISP_RECT& rstDispRect);
    virtual V_UINT32       PrepareAsync();
    virtual V_UINT32       Prepare();
    virtual V_UINT32       Start() ;
    virtual V_UINT32       Pause();
    virtual V_UINT32       Resume();
    virtual V_UINT32       SeekTo(IN V_UINT64 seekTime);//second
    virtual V_UINT32       StartAsync();
    virtual V_UINT32       Stop(E_DVB_CC_MODE eCCMode = E_DVB_CC_MODE_LAST);
    virtual V_UINT32       SetPace(IN V_UINT32 pace);
    virtual V_UINT32       Reset();
    virtual V_INT32        IsPlaying();
    virtual V_UINT32       SetDisplayMode(IN E_SME_DVBPLAYER_DISPLAY_MODE eMode);
    virtual V_UINT32       SetDisplayRatio(IN ST_DispRatio* pstRatio);
    virtual V_UINT32       SetProgCaInfo(IN const ST_DVBProgCAInfo& rProgCAInfo);
    virtual V_UINT32       SetProgKeyInfo(IN E_DVB_CA_KEY_ID ekeyId, IN const ST_CAKeyParam& rKeyInfo);
    virtual V_UINT32       SetProgKeyIV(IN E_DVB_CA_KEY_IV_ID ekeyIvId, IN const ST_CAIVParam& rKeyIV);
    virtual E_SME_DVBPLAYER_STATE GetDvbPlayerState();
    virtual V_UINT32       GetDisplayMode(OUT E_SME_DVBPLAYER_DISPLAY_MODE& reMode);
    virtual V_UINT32       GetDisplayRatio(OUT ST_DispRatio& rstDispRatio);
    virtual V_UINT32       GetDisplayRect(OUT ST_SME_DVBDISP_RECT& rstDispRect);
#ifdef __LINUX__
    virtual V_UINT32       SetAudioMute(IN const V_UINT32 mute);
    virtual V_UINT32       GetAudioMute(INOUT V_UINT32*mute);
    virtual V_UINT32       SetAudioVolume(IN const V_UINT32 volume);
    virtual V_UINT32       GetAudioVolume(INOUT V_UINT32*volume);
    virtual V_UINT32       SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode);
    virtual V_UINT32       GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E* mode);
#endif
#ifdef  SMEPLAYER_BENCHMARK_OPEN
    virtual V_VOID         UpdateDvbProcInfo(ST_PROC_COMMON_INFO * pstCommonPointer);
#endif
    virtual V_UINT32       SetVFreezeMode(IN E_DVB_FREEZE_MODE eMode);

private:

    V_VOID SetPlayerState(E_DVBPLAYER_STATE eState);
    E_DVBPLAYER_STATE GetPlayerState()const;
    static gboolean bus_call(GstBus * bus, GstMessage * msg, gpointer data);
    static V_UINT32 RunMainLoopThd(V_VOID* pvCtx);
    gboolean FlushPipeline();

private:
    ISmeDvbPlayerObs*               m_pobjPlayerObs;    //应用的oberserver
    E_SME_DVBPLAYER_DISPLAY_MODE    m_eVDispMode;       //显示模式，fitin fitout等
    E_DVBPLAYER_STATE               m_eState;           //播放器当前状态.
    GstElement*                     m_pobjGstPipeline;  //gst wrap pipeline
    E_SME_DVBPLAYER_LOG_LEVEL       m_eLogLevel;        //日志级别
    ST_SME_DVBDISP_RECT             m_stVDispRect;      //display rect
    ST_VOS_THD_MUTEX                m_stCmdMutex;       //cmdMutex
    VOSCONDHDL                      m_hdlCmdCond;
    guint                           m_u32BusWatchId;
    GMainLoop*                      m_pstMainloop;
    VOSTHDHDL                       m_hdlRunMainLoopThd;
    V_BOOL                          m_bWaitStartedSig;
    E_SME_DVBPLAYER_DISPLAY_MODE    m_eDisplayMode;
    ST_DVBProgMediaInfo             m_stDVBProgMediaInfo;
    V_BOOL                          m_bNeedSetProgMediaInfo;
#ifdef  SMEPLAYER_BENCHMARK_OPEN
    SmeBenchMark                    m_objBenchMark;
#endif
    V_UINT64                        m_u64StartTime;
    V_UINT64                        m_u64FccTime;
    V_UINT32                        m_u32ResWidth;
    V_UINT32                        m_u32ResHeight;
    E_DVB_FREEZE_MODE               m_eFreezeMode;
    ST_DispRatio                    m_stDispRatio;
    E_DVB_TYPE                      m_eDvbType;

public:
    V_UINT64                        m_u64TimeLineStartBg;//begin time of start
    V_UINT64                        m_u64TimeLineCCBg;//begin time of cc
};

#endif //__SMEC_DVBPLAYER_H__
