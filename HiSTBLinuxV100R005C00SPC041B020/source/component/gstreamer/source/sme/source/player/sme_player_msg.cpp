/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_player_cmds.cpp
 * @brief   sme player cmd s ¿‡ µœ÷
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

#include <stdio.h>
#include "osal_type.h"
#include "sme_macro.h"
#include "sme_log.h"
#include "sme_player_impl_gst.h"
#include "sme_player_msg.h"
#include "osal_mem.h"

//lint -esym(613, m_pobjPlayer)  //Possible use of null pointer
//lint -e731   //Boolean argument to equal/not equal
//lint -esym(838, u32Err, u32Ret) // not been used
//lint -efunc(1763, SmePlayerMsgGetProp::GetRespProp, SmePlayerMsgRegBtChangedCb::GetCbCtx)
//lint -efunc(1763, SmePlayerMsgRegManifestCompleteCb::GetCbCtx)

//----------------------------------------------------------------
//SmePlayerMsgBase

/*private*/
SmePlayerMsgBase::SmePlayerMsgBase()
:m_eType(E_TYPE_BUTT)
,m_u32Res(E_SME_PLAYER_ERROR_UNKOWN)
,m_hdlCond(NULL)
,m_bNotify(ICS_FALSE)
,m_pfnCb(NULL)
,m_pvCtx(NULL)
{
    ICS_LOGE("can not support this constructor!");
    m_hdlCond = VOS_CreateThdCond(NULL);
    if(NULL == m_hdlCond)
    {
        ICS_LOGE("VOS_CreateThdCond failed why?");
    }
}

SmePlayerMsgBase::SmePlayerMsgBase(IN E_TYPE eType,
    IN pfnSyncMsgCompletedCb pfnCb, IN V_VOID* pvCtx,
    IN V_BOOL bNotify /*= ICS_FALSE*/)
:m_eType(eType)
,m_u32Res(E_SME_PLAYER_ERROR_UNKOWN)
,m_hdlCond(NULL)
,m_bNotify(bNotify)
,m_pfnCb(pfnCb)
,m_pvCtx(pvCtx)
{
    m_hdlCond = VOS_CreateThdCond(NULL);
    if(NULL == m_hdlCond)
    {
        ICS_LOGE("VOS_CreateThdCond failed why?");
    }
}

SmePlayerMsgBase::~SmePlayerMsgBase()
{
    m_pvCtx = NULL;
    m_pfnCb = NULL;
    if(NULL != m_hdlCond)
    {
        VOS_DestroyThdCond(m_hdlCond);
        m_hdlCond = NULL;
    }

    m_u32Res = 0xABCDEFAB;
}

SmePlayerMsgBase::E_TYPE SmePlayerMsgBase::GetType() const
{
    return m_eType;
}

V_VOID SmePlayerMsgBase::SetResult(V_UINT32 u32Res)
{
    m_u32Res = u32Res;
    return;
}

V_UINT32 SmePlayerMsgBase::GetResult() const
{
    return m_u32Res;
}

V_VOID SmePlayerMsgBase::SyncMsgCompleted()
{
    if(NULL != this->m_pfnCb)
    {
        this->m_pfnCb(m_pvCtx);
    }

    return;
}

SmePlayerMsgBase::E_MSG_ERR SmePlayerMsgBase::WaitMsg(ST_VOS_THD_MUTEX* pszMutex, V_UINT32 u32TimeOut)
{
    V_UINT32 u32Err = ICS_FAIL;
    E_MSG_ERR eRet = E_MSG_ERR_NONE;

    if(NULL == pszMutex || NULL == m_hdlCond)
    {
        ICS_LOGE("WaitMsg invalid args!");
        eRet = E_MSG_ERR_INVALID_ARGS;
        return eRet;
    }

    u32Err = VOS_ThdCondTimeWait(m_hdlCond, pszMutex, u32TimeOut);
    if(u32Err == (V_UINT32)E_VOS_THD_ERR_NONE)
    {
        eRet = E_MSG_ERR_NONE;
    }
    else if (u32Err == (V_UINT32)E_VOS_THD_ERR_COND_TIMEOUT)
    {
        eRet = E_MSG_ERR_TIMEOUT;
    }
    else
    {
        eRet = E_MSG_ERR_UNKNOWN;
    }

    return eRet;
}

V_VOID SmePlayerMsgBase::SignalMsg(ST_VOS_THD_MUTEX* pszMutex)
{
    if(NULL != this->m_pfnCb)
    {
        if(NULL == pszMutex || NULL == m_hdlCond)
        {
            ICS_LOGE("SignalMsg invalid args!");
            return;
        }

        VOS_ThdMutexLock(pszMutex);
        VOS_ThdCondSignal(m_hdlCond);
        VOS_ThdMutexUnLock(pszMutex);
    }
    else
    {
        delete this;
    }

    return;
}


V_BOOL SmePlayerMsgBase::IsNotify() const
{
    return m_bNotify;
}

//-------------------------------------------------------------------
//SmePlayerMsgSetObs
SmePlayerMsgSetObs::SmePlayerMsgSetObs(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const ISmePlayerObs* pobjObs)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_OBS, pfnCb, pvCtx)
,m_pobjObs(pobjObs)
{

}

SmePlayerMsgSetObs::~SmePlayerMsgSetObs()
{
	m_pobjObs = NULL;
}

const ISmePlayerObs* SmePlayerMsgSetObs::GetPlayerObs() const
{
    return m_pobjObs;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetDs
SmePlayerMsgSetDs::SmePlayerMsgSetDs(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_CHAR* pszURI)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_DS, pfnCb, pvCtx)
,m_pszUri(pszURI)
{

}

SmePlayerMsgSetDs::~SmePlayerMsgSetDs()
{
	m_pszUri = NULL;
}

const V_CHAR* SmePlayerMsgSetDs::GetUri() const
{
    return m_pszUri;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetSf
SmePlayerMsgSetSf::SmePlayerMsgSetSf(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_VOID* pvSf)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_SF, pfnCb, pvCtx)
,m_pvSf(pvSf)
{

}

SmePlayerMsgSetSf::~SmePlayerMsgSetSf()
{
	m_pvSf = NULL;
}

const V_VOID* SmePlayerMsgSetSf::GetSf() const
{
    return m_pvSf;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetVDispRect
SmePlayerMsgSetVDispRect::SmePlayerMsgSetVDispRect(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN V_INT32 i32StartX, IN V_INT32 i32StartY,
    IN V_INT32 i32Width, IN V_INT32 i32Height)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_VDISP_RECT, pfnCb, pvCtx)
,m_i32StartX(i32StartX)
,m_i32StartY(i32StartY)
,m_i32Width(i32Width)
,m_i32Height(i32Height)
{

}

SmePlayerMsgSetVDispRect::~SmePlayerMsgSetVDispRect()
{

}

V_INT32 SmePlayerMsgSetVDispRect::GetStartX() const
{
    return m_i32StartX;
}

V_INT32 SmePlayerMsgSetVDispRect::GetStartY() const
{
    return m_i32StartY;
}

V_INT32 SmePlayerMsgSetVDispRect::GetWidth() const
{
    return m_i32Width;
}

V_INT32 SmePlayerMsgSetVDispRect::GetHeight() const
{
    return m_i32Height;
}

//--------------------------------------------------------------------
//SmePlayerMsgPrepareAsync
SmePlayerMsgPrepareAsync::SmePlayerMsgPrepareAsync(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_PREPARE_ASYNC, pfnCb, pvCtx)
{

}
SmePlayerMsgPrepareAsync::~SmePlayerMsgPrepareAsync()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgPrepare
SmePlayerMsgPrepare::SmePlayerMsgPrepare(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_PREPARE, pfnCb, pvCtx)
{

}

SmePlayerMsgPrepare::~SmePlayerMsgPrepare()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgStart
SmePlayerMsgStart::SmePlayerMsgStart(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_START, pfnCb, pvCtx)
{

}

SmePlayerMsgStart::~SmePlayerMsgStart()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgStop
SmePlayerMsgStop::SmePlayerMsgStop(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_STOP, pfnCb, pvCtx)
{

}

SmePlayerMsgStop::~SmePlayerMsgStop()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgPause
SmePlayerMsgPause::SmePlayerMsgPause(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_PAUSE, pfnCb, pvCtx)
{

}

SmePlayerMsgPause::~SmePlayerMsgPause()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgResume
SmePlayerMsgResume::SmePlayerMsgResume(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_RESUME, pfnCb, pvCtx)
{

}

SmePlayerMsgResume::~SmePlayerMsgResume()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgSeekTo
SmePlayerMsgSeekTo::SmePlayerMsgSeekTo(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN V_INT32 i32SeetToMs)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SEEKTO, pfnCb, pvCtx)
,m_i32SeekToMs(i32SeetToMs)
{

}

SmePlayerMsgSeekTo::~SmePlayerMsgSeekTo()
{

}

V_INT32 SmePlayerMsgSeekTo::GetSeekToMs() const
{
    return m_i32SeekToMs;
}

//--------------------------------------------------------------------
//SmePlayerMsgReset
SmePlayerMsgReset::SmePlayerMsgReset(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_RESET, pfnCb, pvCtx)
{

}

SmePlayerMsgReset::~SmePlayerMsgReset()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgGetDur
SmePlayerMsgGetDur::SmePlayerMsgGetDur(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_DURATION, pfnCb, pvCtx)
{

}

SmePlayerMsgGetDur::~SmePlayerMsgGetDur()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgGetCurPos
SmePlayerMsgGetCurPos::SmePlayerMsgGetCurPos(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_CURPOSITION, pfnCb, pvCtx)
{

}

SmePlayerMsgGetCurPos::~SmePlayerMsgGetCurPos()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgIsPlay
SmePlayerMsgIsPlay::SmePlayerMsgIsPlay(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_ISPLAYING, pfnCb, pvCtx)
{

}

SmePlayerMsgIsPlay::~SmePlayerMsgIsPlay()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgGetVideoWidth
SmePlayerMsgGetVideoWidth::SmePlayerMsgGetVideoWidth(
    IN pfnSyncMsgCompletedCb pfnCb, IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_VIDEO_WIDTH, pfnCb, pvCtx)
{

}

SmePlayerMsgGetVideoWidth::~SmePlayerMsgGetVideoWidth()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgGetVideoHeight
SmePlayerMsgGetVideoHeight::SmePlayerMsgGetVideoHeight(
    IN pfnSyncMsgCompletedCb pfnCb, IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_VIDEO_HEIGHT, pfnCb, pvCtx)
{

}

SmePlayerMsgGetVideoHeight::~SmePlayerMsgGetVideoHeight()
{

}

//SmePlayerMsgSetProp
SmePlayerMsgSetProp::SmePlayerMsgSetProp(IN pfnSyncMsgCompletedCb pfnCb,
                                    IN V_VOID* pvCtx,
                                    IN const SmeProperty* pobjSmeProp)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_PROP, pfnCb, pvCtx)
,m_pobjProp(pobjSmeProp)
{

}

SmePlayerMsgSetProp::~SmePlayerMsgSetProp()
{
	m_pobjProp = NULL;
}

const SmeProperty* SmePlayerMsgSetProp::GetProp() const
{
    return m_pobjProp;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetProp
SmePlayerMsgGetProp::SmePlayerMsgGetProp(IN pfnSyncMsgCompletedCb pfnCb,
                                    IN V_VOID* pvCtx,
                                    IN const SmeProperty* pobjReqProp, INOUT SmeProperty* pobjRespProp)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_PROP, pfnCb, pvCtx)
,m_pobjReqProp(pobjReqProp)
,m_pobjRespProp(pobjRespProp)
{

}

SmePlayerMsgGetProp::~SmePlayerMsgGetProp()
{
	m_pobjReqProp = NULL;
	m_pobjRespProp = NULL;
}

const SmeProperty* SmePlayerMsgGetProp::GetReqProp() const
{
    return m_pobjReqProp;
}

SmeProperty* SmePlayerMsgGetProp::GetRespProp() const
{
    return m_pobjRespProp;
}

//--------------------------------------------------------------------
//SmePlayerMsgCacheCtrl
SmePlayerMsgCacheCtrl::SmePlayerMsgCacheCtrl(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_CACHE_CTRL, pfnCb, pvCtx)
{

}
SmePlayerMsgCacheCtrl::~SmePlayerMsgCacheCtrl()
{

}

//--------------------------------------------------------------------
//SmePlayerMsgGetBandwidth
SmePlayerMsgGetBandwidth ::SmePlayerMsgGetBandwidth(IN pfnSyncMsgCompletedCb pfnCb,
  IN V_VOID* pvCtx)
  :SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_BANDWIDTH, pfnCb, pvCtx)
{

};

SmePlayerMsgGetBandwidth::~SmePlayerMsgGetBandwidth()
{
}

//--------------------------------------------------------------------
//SmePlayerMsgSetBitrate
SmePlayerMsgSetBitrate::SmePlayerMsgSetBitrate(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32Bitrate)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_STREAM_BITRATE, pfnCb, pvCtx)
,m_u32Bitrate(u32Bitrate)
{
    m_u32Bitrate = u32Bitrate;
}

SmePlayerMsgSetBitrate::~SmePlayerMsgSetBitrate()
{
    m_u32Bitrate = 0;
}

 V_UINT32 SmePlayerMsgSetBitrate::GetBitrate() const
{
    return m_u32Bitrate;
}

//SmePlayerMsgSetUserAgent
SmePlayerMsgSetUserAgent::SmePlayerMsgSetUserAgent(IN pfnSyncMsgCompletedCb pfnCb,
                        IN V_VOID* pvCtx,
                        IN const V_CHAR * pstUserAgent)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_USER_AGENT, pfnCb, pvCtx)
,m_pstUserAgent(pstUserAgent)
{
    m_pstUserAgent = pstUserAgent;
}

SmePlayerMsgSetUserAgent::~SmePlayerMsgSetUserAgent()
{
    m_pstUserAgent = NULL;
}

const V_CHAR* SmePlayerMsgSetUserAgent::GetUserAgent() const
{
    return m_pstUserAgent;
}

//SmePlayerMsgSetReferer
SmePlayerMsgSetReferer::SmePlayerMsgSetReferer(IN pfnSyncMsgCompletedCb pfnCb,
                        IN V_VOID* pvCtx,
                        IN const V_CHAR * pstSetReferer)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_REFERER, pfnCb, pvCtx)
,m_pstSetReferer(pstSetReferer)
{
    m_pstSetReferer = pstSetReferer;
}

SmePlayerMsgSetReferer::~SmePlayerMsgSetReferer()
{
    m_pstSetReferer = NULL;
}

const V_CHAR* SmePlayerMsgSetReferer::GetReferer() const
{
    return m_pstSetReferer;
}

//SmePlayerMsgSetCookies
SmePlayerMsgSetCookies::SmePlayerMsgSetCookies(IN pfnSyncMsgCompletedCb pfnCb,
                        IN V_VOID* pvCtx,
                        IN V_CHAR ** pstSetCookies)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_COOKIES, pfnCb, pvCtx)
,m_pstSetCookies(pstSetCookies)
{
    m_pstSetCookies = pstSetCookies;
}

SmePlayerMsgSetCookies::~SmePlayerMsgSetCookies()
{
    m_pstSetCookies = NULL;
}

V_CHAR** SmePlayerMsgSetCookies::GetCookies()
{
    return m_pstSetCookies;
}

//SmePlayerMsgSetByteRange
SmePlayerMsgSetByteRange::SmePlayerMsgSetByteRange(IN pfnSyncMsgCompletedCb pfnCb,
                     IN V_VOID* pvCtx,
                     IN V_BOOL bByteRange)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_BYTE_RANGE, pfnCb, pvCtx)
,m_bByteRange(bByteRange)
{
    m_bByteRange = bByteRange;
}

SmePlayerMsgSetByteRange::~SmePlayerMsgSetByteRange()
{
    m_bByteRange = ICS_TRUE;
}

V_BOOL SmePlayerMsgSetByteRange::GetByteRange() const
{
    return m_bByteRange;
}


//--------------------------------------------------------------------
//SmePlayerMsgRegManifestCompleteCb
SmePlayerMsgRegManifestCompleteCb::SmePlayerMsgRegManifestCompleteCb(
            IN pfnSyncMsgCompletedCb pfnCb,
            IN V_VOID* pvCtx,
            IN PFN_StreamPlayModeCb pfPlayModeCb,
            IN V_VOID* pvModeCbCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_REG_MANIFEST_COMPLETE_CB, pfnCb, pvCtx)
,m_pfPlayModeCb(pfPlayModeCb)
,m_pvModeCbCtx(pvModeCbCtx)
{
}

SmePlayerMsgRegManifestCompleteCb::~SmePlayerMsgRegManifestCompleteCb()
{
    m_pfPlayModeCb = NULL;
    m_pvModeCbCtx = NULL;
}

PFN_StreamPlayModeCb SmePlayerMsgRegManifestCompleteCb::GetManifestCompleteCb() const
{
    return m_pfPlayModeCb;
}

V_VOID* SmePlayerMsgRegManifestCompleteCb::GetCbCtx()const
{
    return m_pvModeCbCtx;
}


//--------------------------------------------------------------------
//SmePlayerMsgRegBtChangedCb
SmePlayerMsgRegBtChangedCb::SmePlayerMsgRegBtChangedCb(
            IN pfnSyncMsgCompletedCb pfnCb,
            IN V_VOID* pvCtx,
            IN PFN_BitrateChangedCb pfBtCb,
            IN V_VOID* pvCbCtx)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_REG_BITRATE_CHANGED_CB, pfnCb, pvCtx)
,m_pfBtChangedCb(pfBtCb)
,m_pvChangedCbCtx(pvCbCtx)
{
}

SmePlayerMsgRegBtChangedCb::~SmePlayerMsgRegBtChangedCb()
{
    m_pfBtChangedCb = NULL;
    m_pvChangedCbCtx = NULL;
}

PFN_BitrateChangedCb SmePlayerMsgRegBtChangedCb::GetBtChangedCb() const
{
    return m_pfBtChangedCb;
}

V_VOID* SmePlayerMsgRegBtChangedCb::GetCbCtx()const
{
    return m_pvChangedCbCtx;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetSubDs
SmePlayerMsgSetSubDs::SmePlayerMsgSetSubDs(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_CHAR* pszSubURI)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_SUB_DS, pfnCb, pvCtx)
,m_pszSubUri(pszSubURI)
{

}

SmePlayerMsgSetSubDs::~SmePlayerMsgSetSubDs()
{
    m_pszSubUri = NULL;
}

const V_CHAR* SmePlayerMsgSetSubDs::GetSubUri() const
{
    return m_pszSubUri;
}

//--------------------------------------------------------------------
//SmePlayerMsgRegSubCb
SmePlayerMsgRegSubCb::SmePlayerMsgRegSubCb(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const ST_SubCallback* pstSubCb)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_REG_SUB_CB, pfnCb, pvCtx)
,m_pstSubCb(pstSubCb)
{
}

SmePlayerMsgRegSubCb::~SmePlayerMsgRegSubCb()
{
    m_pstSubCb = NULL;
}

const ST_SubCallback* SmePlayerMsgRegSubCb::GetSubCb() const
{
    return m_pstSubCb;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetAudioMute
SmePlayerMsgSetAudioMute::SmePlayerMsgSetAudioMute(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_UINT32 mute)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_AUDIO_MUTE, pfnCb, pvCtx)
,m_mute(mute)
{

}

SmePlayerMsgSetAudioMute::~SmePlayerMsgSetAudioMute()
{
}

 const V_UINT32 SmePlayerMsgSetAudioMute::GetAudioMute() const
{
    return m_mute;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetAudioMute
SmePlayerMsgGetAudioMute::SmePlayerMsgGetAudioMute(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN V_UINT32* mute)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_AUDIO_MUTE, pfnCb, pvCtx)
,m_mute(mute)
{

}

SmePlayerMsgGetAudioMute::~SmePlayerMsgGetAudioMute()
{
    m_mute = NULL;
}

V_UINT32*  SmePlayerMsgGetAudioMute::GetAudioMute()
{
    return m_mute;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetAudioVolume
SmePlayerMsgSetAudioVolume::SmePlayerMsgSetAudioVolume(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_UINT32 volume)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_AUDIO_VOLUME, pfnCb, pvCtx)
,m_volume(volume)
{

}

SmePlayerMsgSetAudioVolume::~SmePlayerMsgSetAudioVolume()
{
}

const V_UINT32 SmePlayerMsgSetAudioVolume::GetAudioVolume() const
{
    return m_volume;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetAudioVolume
SmePlayerMsgGetAudioVolume::SmePlayerMsgGetAudioVolume(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN V_UINT32* volume)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_AUDIO_VOLUME, pfnCb, pvCtx)
,m_volume(volume)
{

}

SmePlayerMsgGetAudioVolume::~SmePlayerMsgGetAudioVolume()
{
    m_volume = NULL;
}

V_UINT32*  SmePlayerMsgGetAudioVolume::GetAudioVolume()
{
    return m_volume;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetVideoDispMode
SmePlayerMsgGetVideoDispMode::SmePlayerMsgGetVideoDispMode(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx,
    IN E_SME_PLAYER_DISPLAY_MODE& reMode,
    IN ST_DISP_RATIO& rpstRatio)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_DISPMODE, pfnCb, pvCtx)
{
    m_disp_mode = &reMode;
    m_disp_ratio = &rpstRatio;
}

SmePlayerMsgGetVideoDispMode::~SmePlayerMsgGetVideoDispMode()
{
    m_disp_mode = NULL;
    m_disp_ratio = NULL;
}

E_SME_PLAYER_DISPLAY_MODE* SmePlayerMsgGetVideoDispMode::GetVideoDispMode()
{
    return m_disp_mode;
}

ST_DISP_RATIO* SmePlayerMsgGetVideoDispMode::GetVideoDispRatio()
{
    return m_disp_ratio;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetVideoDispRect
SmePlayerMsgGetVideoDispRect::SmePlayerMsgGetVideoDispRect(IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx,
    IN ST_SME_DISP_RECT& rstRect)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_DISPRECT, pfnCb, pvCtx)
{
    m_disp_rect = &rstRect;
}

SmePlayerMsgGetVideoDispRect::~SmePlayerMsgGetVideoDispRect()
{
    m_disp_rect = NULL;
}

ST_SME_DISP_RECT* SmePlayerMsgGetVideoDispRect::GetVideoDispRect()
{
    return m_disp_rect;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetAudioChannelMode
SmePlayerMsgSetAudioChannelMode::SmePlayerMsgSetAudioChannelMode(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const A_TRACK_CHANNEL_MODE_E mode)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_AUDIO_CHANNEL_MODE, pfnCb, pvCtx)
,m_mode(mode)
{

}

SmePlayerMsgSetAudioChannelMode::~SmePlayerMsgSetAudioChannelMode()
{
}

const A_TRACK_CHANNEL_MODE_E SmePlayerMsgSetAudioChannelMode::GetAudioChannelMode() const
{
    return m_mode;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetAudioChannelMode
SmePlayerMsgGetAudioChannelMode::SmePlayerMsgGetAudioChannelMode(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN A_TRACK_CHANNEL_MODE_E* mode)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_AUDIO_CHANNEL_MODE, pfnCb, pvCtx)
,m_mode(mode)
{

}

SmePlayerMsgGetAudioChannelMode::~SmePlayerMsgGetAudioChannelMode()
{
    m_mode = NULL;
}

A_TRACK_CHANNEL_MODE_E*  SmePlayerMsgGetAudioChannelMode::GetAudioChannelMode()
{
    return m_mode;
}


//--------------------------------------------------------------------
//SmePlayerMsgSetAudioHdmiMode
SmePlayerMsgSetAudioHdmiMode::SmePlayerMsgSetAudioHdmiMode(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN const V_CHAR *pszAudioHdmiMode)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_AUDIO_HDMI_MODE, pfnCb, pvCtx)
,m_pszAudioHdmiMode(pszAudioHdmiMode)
{

}

SmePlayerMsgSetAudioHdmiMode::~SmePlayerMsgSetAudioHdmiMode()
{
    m_pszAudioHdmiMode = NULL;
}

const V_CHAR* SmePlayerMsgSetAudioHdmiMode::GetAudioHdmiMode() const
{
    return m_pszAudioHdmiMode;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetAudioHdmiMode
SmePlayerMsgGetAudioHdmiMode::SmePlayerMsgGetAudioHdmiMode(
    IN pfnSyncMsgCompletedCb pfnCb,
    IN V_VOID* pvCtx, IN V_CHAR **pszAudioHdmiMode)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_AUDIO_HDMI_MODE, pfnCb, pvCtx)
,m_pszAudioHdmiMode(pszAudioHdmiMode)
{

}

SmePlayerMsgGetAudioHdmiMode::~SmePlayerMsgGetAudioHdmiMode()
{
    m_pszAudioHdmiMode = NULL;
}

V_CHAR** SmePlayerMsgGetAudioHdmiMode::GetAudioHdmiMode()
{
    return m_pszAudioHdmiMode;
}


//--------------------------------------------------------------------
//SmePlayerMsgGetBufferStatus
SmePlayerMsgGetBufferStatus::SmePlayerMsgGetBufferStatus(
                           IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           INOUT ST_BUFFER_STATUS *pstBufferStatus)
:SmePlayerMsgBase(SmePlayerMsgBase:: E_TYPE_GET_BUFFER_STATUS,pfnCb,pvCtx)
,m_pstBufferStatus(pstBufferStatus)
{

}
SmePlayerMsgGetBufferStatus::~SmePlayerMsgGetBufferStatus()
{
    m_pstBufferStatus = NULL;
}
ST_BUFFER_STATUS *SmePlayerMsgGetBufferStatus::GetBufferStatus()
{
    return m_pstBufferStatus;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetBitrate
SmePlayerMsgGetBitrate ::SmePlayerMsgGetBitrate(IN pfnSyncMsgCompletedCb pfnCb,
                                                IN V_VOID* pvCtx,
                                                INOUT ST_CURRENT_BITRATE *pstCurrentBitrate)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_GET_BITRATE, pfnCb, pvCtx)
,m_pstCurrentBitrate(pstCurrentBitrate)
{

}
SmePlayerMsgGetBitrate::~SmePlayerMsgGetBitrate()
{
    m_pstCurrentBitrate = NULL;
}
ST_CURRENT_BITRATE *SmePlayerMsgGetBitrate::GetCurrentBitrate()
{
    return m_pstCurrentBitrate;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetDisplayMode
SmePlayerMsgSetDisplayMode::SmePlayerMsgSetDisplayMode(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32DispMode,
                           IN ST_DISP_RATIO* pstDispRatio)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_DISPMODE, pfnCb, pvCtx)
,m_u32DisplayMode(u32DispMode)
,m_pstDispRatio(pstDispRatio)
{
}

SmePlayerMsgSetDisplayMode::~SmePlayerMsgSetDisplayMode()
{
    m_u32DisplayMode = 0;
    m_pstDispRatio = NULL;
}

 V_VOID SmePlayerMsgSetDisplayMode::GetDisplayModeAndRadio(INOUT V_UINT32* u32DisplayMode,
        INOUT ST_DISP_RATIO** pstDispRatio) const
{

    *u32DisplayMode = m_u32DisplayMode;
    *pstDispRatio = m_pstDispRatio;
    return ;
}


SmePlayerMsgSetLoop::SmePlayerMsgSetLoop(IN pfnSyncMsgCompletedCb pfnCb,
                IN V_VOID* pvCtx,
                IN V_BOOL bLoop)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_LOOP, pfnCb, pvCtx)
,m_bLoop(bLoop)
{

}

SmePlayerMsgSetLoop::~SmePlayerMsgSetLoop()
{
    m_bLoop = ICS_FALSE;
}

V_BOOL SmePlayerMsgSetLoop::GetLoopVal() const
{
    return m_bLoop;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetVideoTrackInfo
SmePlayerMsgGetVideoTrackInfo::SmePlayerMsgGetVideoTrackInfo(
                           IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           INOUT ST_VIDEO_TRACK_INFO *pstVideoTrackInfo)
:SmePlayerMsgBase(SmePlayerMsgBase:: E_TYPE_GET_VIDEO_TRACK_INFO,pfnCb,pvCtx)
,m_pstVideoTrackInfo(pstVideoTrackInfo)
{

}
SmePlayerMsgGetVideoTrackInfo::~SmePlayerMsgGetVideoTrackInfo()
{
    m_pstVideoTrackInfo = NULL;
}
ST_VIDEO_TRACK_INFO *SmePlayerMsgGetVideoTrackInfo::GetVideoTrackInfo()
{
    return m_pstVideoTrackInfo;
}

//--------------------------------------------------------------------
//SmePlayerMsgGetAudioTrackInfo
SmePlayerMsgGetAudioTrackInfo::SmePlayerMsgGetAudioTrackInfo(
                           IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           INOUT ST_AUDIO_TRACK_INFO *pstAudioTrackInfo)
:SmePlayerMsgBase(SmePlayerMsgBase:: E_TYPE_GET_AUDIO_TRACK_INFO,pfnCb,pvCtx)
,m_pstAudioTrackInfo(pstAudioTrackInfo)
{

}
SmePlayerMsgGetAudioTrackInfo::~SmePlayerMsgGetAudioTrackInfo()
{
    m_pstAudioTrackInfo = NULL;
}
ST_AUDIO_TRACK_INFO *SmePlayerMsgGetAudioTrackInfo::GetAudioTrackInfo()
{
    return m_pstAudioTrackInfo;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetAudioTrack
SmePlayerMsgSetAudioTrack::SmePlayerMsgSetAudioTrack(
                           IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_INT32 i32AudioTrack)
:SmePlayerMsgBase(SmePlayerMsgBase:: E_TYPE_SET_AUDIO_TRACK,pfnCb,pvCtx)
,m_i32AudioTrack(i32AudioTrack)
{

}
SmePlayerMsgSetAudioTrack::~SmePlayerMsgSetAudioTrack()
{

}
V_INT32 SmePlayerMsgSetAudioTrack::GetAudioTrack() const
{
    return m_i32AudioTrack;
}

//--------------------------------------------------------------------
//SmePlayerMsgSetSwitchChannelMode
SmePlayerMsgSetSwitchChannelMode::SmePlayerMsgSetSwitchChannelMode(IN pfnSyncMsgCompletedCb pfnCb,
                           IN V_VOID* pvCtx,
                           IN V_UINT32 u32SwitchChannelMode)
:SmePlayerMsgBase(SmePlayerMsgBase::E_TYPE_SET_SWITCH_CHANNEL_MODE, pfnCb, pvCtx)
,m_u32SwitchChannelMode(u32SwitchChannelMode)
{
}

SmePlayerMsgSetSwitchChannelMode::~SmePlayerMsgSetSwitchChannelMode()
{
    m_u32SwitchChannelMode = 0;
}

 V_VOID SmePlayerMsgSetSwitchChannelMode::GetSwitchChannelMode(INOUT V_UINT32*u32SwitchChannelMode) const
{

    *u32SwitchChannelMode = m_u32SwitchChannelMode;
    return ;
}

//--------------------------------------------------------------------
//notify
SmePlayerNotifyBase::SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE eMsgType,
                                        E_SME_PLAYER_NOTIFY eNotify)
:SmePlayerMsgBase(eMsgType, NULL, NULL, ICS_TRUE)
,m_eNotifyType(eNotify)
{

}

SmePlayerNotifyBase::~SmePlayerNotifyBase()
{

}

E_SME_PLAYER_NOTIFY SmePlayerNotifyBase::GetNotifyType() const
{
    return m_eNotifyType;
}

//--------------------------------------------------------------------
//SmePlayerNotifyOnPrepared
SmePlayerNotifyOnPrepared::SmePlayerNotifyOnPrepared()
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_PREPARED,
    E_SME_PLAYER_NOTIFY_PREPARED)
{

}

SmePlayerNotifyOnPrepared::~SmePlayerNotifyOnPrepared()
{

}

//--------------------------------------------------------------------
//SmePlayerNotifyOnEos
SmePlayerNotifyOnEos::SmePlayerNotifyOnEos()
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_EOS,
    E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED)
{

}

SmePlayerNotifyOnEos::~SmePlayerNotifyOnEos()
{

}

//--------------------------------------------------------------------
//SmePlayerNotifyOnError
SmePlayerNotifyOnError::SmePlayerNotifyOnError(E_SME_PLAYER_ERROR eError)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_ERROR, E_SME_PLAYER_NOTIFY_ERROR)
,m_eError(eError)
{

}

SmePlayerNotifyOnError::~SmePlayerNotifyOnError()
{

}

E_SME_PLAYER_ERROR SmePlayerNotifyOnError::GetErrorVal() const
{
    return m_eError;
}

//--------------------------------------------------------------------
//SmePlayerNotifyOnSeekCompleted
SmePlayerNotifyOnSeekCompleted::SmePlayerNotifyOnSeekCompleted(
    E_SME_PLAYER_ERROR eError, V_INT32 i32Ms)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_SEEKCOMPLTED,
    E_SME_PLAYER_NOTIFY_SEEK_COMPLETED)
,m_eError(eError)
,m_i32Ms(i32Ms)
{

}

SmePlayerNotifyOnSeekCompleted::~SmePlayerNotifyOnSeekCompleted()
{

}

E_SME_PLAYER_ERROR SmePlayerNotifyOnSeekCompleted::GetErrorVal() const
{
    return m_eError;
}

V_INT32 SmePlayerNotifyOnSeekCompleted::GetSeekRes() const
{
    return m_i32Ms;
}

//--------------------------------------------------------------------
//SmePlayerNotifyOnInfo
SmePlayerNotifyOnInfo::SmePlayerNotifyOnInfo(V_UINT32 u32Ext1,
    V_UINT32 u32Ext2)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_INFO,
    E_SME_PLAYER_NOTIFY_INFO)
,m_u32Ext1(u32Ext1)
,m_u32Ext2(u32Ext2)
{

}

SmePlayerNotifyOnInfo::~SmePlayerNotifyOnInfo()
{

}

V_UINT32 SmePlayerNotifyOnInfo::GetExtVal1() const
{
    return m_u32Ext1;
}

V_UINT32 SmePlayerNotifyOnInfo::GetExtVal2() const
{
    return m_u32Ext2;
}

//--------------------------------------------------------------------
//SmePlayerNotifyOnBufferUpdate
SmePlayerNotifyOnBufferUpdate::SmePlayerNotifyOnBufferUpdate(V_UINT32 u32Percent)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_BUFFER_UPDATE,
    E_SME_PLAYER_NOTIFY_BUFFER_UPDATE)
,m_u32BufferedPercent(u32Percent)
{

}

SmePlayerNotifyOnBufferUpdate::~SmePlayerNotifyOnBufferUpdate()
{

}

V_UINT32 SmePlayerNotifyOnBufferUpdate::GetBufferedPercent() const
{
    return m_u32BufferedPercent;
}

//SmePlayerNotifyOnResolutionChange
SmePlayerNotifyOnResolutionChange::SmePlayerNotifyOnResolutionChange(V_UINT32 u32Width, V_UINT32 u32Height)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_RESOLUTION_CHANGED,
    E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED)
,m_u32Width(u32Width)
,m_u32Height(u32Height)
{
}
SmePlayerNotifyOnResolutionChange::~SmePlayerNotifyOnResolutionChange()
{
}
V_VOID SmePlayerNotifyOnResolutionChange::GetResolution(V_UINT32 & u32Width, V_UINT32 & u32Height) const
{
    u32Width = m_u32Width;
    u32Height = m_u32Height;
    return;
}

//SmePlayerNotifyOnHttpErr
SmePlayerNotifyOnHttpErr::SmePlayerNotifyOnHttpErr(V_UINT32 u32StatusCode)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_HTTP_ERR,
    E_SME_PLAYER_NOTIFY_HTTP_ERR)
,m_u32StatusCode(u32StatusCode)
{
}
SmePlayerNotifyOnHttpErr::~SmePlayerNotifyOnHttpErr()
{
}
V_VOID SmePlayerNotifyOnHttpErr::GetStatusCode(V_UINT32 & u32StatusCode) const
{
    u32StatusCode = m_u32StatusCode;
    return;
}

//SmePlayerNotifyOnFragmentChanged
SmePlayerNotifyOnFragmentChanged::SmePlayerNotifyOnFragmentChanged(const ST_FRAGMENT_INFO *pstFragmentInfo)
:SmePlayerNotifyBase(SmePlayerMsgBase::E_TYPE_ON_FRAGMENT_INFO_UPDATED,
    E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE)
{
    if (pstFragmentInfo)
    {
        if (E_SME_MEM_OK != VOS_Memcpy_S(
            &m_stFragmentInfo,
            sizeof(ST_FRAGMENT_INFO),
            pstFragmentInfo,
            sizeof(ST_FRAGMENT_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memcpy_S return failed.");
        }
    }
    else
    {
        if (E_SME_MEM_OK != VOS_Memset_S(
            &m_stFragmentInfo,
            sizeof(ST_FRAGMENT_INFO),
            0,
            sizeof(ST_FRAGMENT_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memset_S return failed.");
        }

    }
}
SmePlayerNotifyOnFragmentChanged::~SmePlayerNotifyOnFragmentChanged()
{
}
V_VOID SmePlayerNotifyOnFragmentChanged::GetFragmentInfo(ST_FRAGMENT_INFO *pstFragmentInfo) const
{
    if (pstFragmentInfo)
    {
        if (E_SME_MEM_OK != VOS_Memcpy_S(pstFragmentInfo,
            sizeof(ST_FRAGMENT_INFO), &m_stFragmentInfo, sizeof(ST_FRAGMENT_INFO)))
        {
            ICS_LOGE("SECURE: VOS_Memcpy_S return failed.");
        }
    }
    return;
}

//--------------------------------------------------------------------
//SmePlayerMsgRunner
SmePlayerMsgRunner::SmePlayerMsgRunner(IN SmePlayerImplGst* pobjPlayer)
:m_pobjPlayer(pobjPlayer)
,m_bInited(ICS_FALSE)
{
    V_UINT32 u32Err = ICS_SUCCESS;

    u32Err = VOS_InitThdMutex(&m_stSyncMsgMutex, NULL);
    UTILS_MLOGE_RET_IF(ICS_SUCCESS != u32Err, ("init mutex failed"));
}

SmePlayerMsgRunner::~SmePlayerMsgRunner()
{
    VOS_DeInitThdMutex(&m_stSyncMsgMutex);
    m_pobjPlayer = NULL;
}

//--------------------------------------------------------------------

V_UINT32 SmePlayerMsgRunner::Init()
{
    V_UINT32 u32Ret = ICS_SUCCESS;

    UTILS_MLOGE_RET_VAL_IF((NULL == m_pobjPlayer),
        ICS_FAIL,("m_pobjPlayer is null"));

    UTILS_RET_VAL_IF(ICS_FALSE != m_bInited, ICS_SUCCESS);

    u32Ret = this->Start();
    if(ICS_SUCCESS == u32Ret)
    {
        m_bInited = ICS_TRUE;
    }
    else
    {
        this->Stop();
    }

    return u32Ret;

}

V_VOID SmePlayerMsgRunner::DeInit(V_BOOL bFreeCmd/* = ICS_TRUE*/)
{

    UTILS_MLOGW_RET_IF(ICS_FALSE == m_bInited, ("not inited"));

    this->ClearMsgQueue(bFreeCmd);
    this->Stop();
    m_bInited = ICS_FALSE;

    return;
}

V_UINT32 SmePlayerMsgRunner::PostMsg(SmePlayerMsgBase* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    UTILS_MLOGW_RET_VAL_IF(ICS_FALSE == m_bInited, E_SME_PLAYER_ERROR_STATE,
        ("not inited"));

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, E_SME_PLAYER_ERROR_NOMEM,
         ("new failed"));

    VOS_ThdMutexLock(&m_stSyncMsgMutex);
    u32Ret = ICS::MsgQueue::PushMsg(pobjMsg);
    if(ICS_SUCCESS != u32Ret)
    {
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        ICS_LOGE("push cmd(%d) failed", pobjMsg->GetType());
    }
    else
    {
       pobjMsg->WaitMsg(&m_stSyncMsgMutex, (V_UINT32)-1);
       u32Ret = pobjMsg->GetResult();
    }

    VOS_ThdMutexUnLock(&m_stSyncMsgMutex);

    return u32Ret;
}

V_UINT32 SmePlayerMsgRunner::SendMsg(SmePlayerMsgBase* pobjMsg)
{
    V_UINT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    UTILS_MLOGW_RET_VAL_IF(ICS_FALSE == m_bInited, E_SME_PLAYER_ERROR_STATE,
        ("not inited"));

    UTILS_MLOGE_RET_VAL_IF(NULL == pobjMsg, E_SME_PLAYER_ERROR_NOMEM,
         ("new failed"));

    VOS_ThdMutexLock(&m_stSyncMsgMutex);
    u32Ret = ICS::MsgQueue::PushMsg(pobjMsg);
    if(ICS_SUCCESS != u32Ret)
    {
        u32Ret = E_SME_PLAYER_ERROR_UNKOWN;
        ICS_LOGE("push cmd(%d) failed", pobjMsg->GetType());
    }
    VOS_ThdMutexUnLock(&m_stSyncMsgMutex);

    return u32Ret;
}

V_VOID SmePlayerMsgRunner::OnGetAMsg(IN V_VOID* pvMsg)
{
    SmePlayerMsgBase* pobjMsg = static_cast<SmePlayerMsgBase*>(pvMsg);

    UTILS_MLOGE_RET_IF((NULL == m_pobjPlayer) || (ICS_FALSE == m_bInited),
        ("m_pobjPlayer is null or not inited,m_bInited=%d", m_bInited));

    m_pobjPlayer->OnMsgComing(pobjMsg);

    pobjMsg->SyncMsgCompleted();

    pobjMsg->SignalMsg(&m_stSyncMsgMutex);

    return;
}

V_VOID SmePlayerMsgRunner::OnFreeMsg(IN V_VOID* pvMsg)
{
    UTILS_MLOGE_RET_IF((NULL == m_pobjPlayer) || (ICS_FALSE == m_bInited),
        ("m_pobjPlayer is null or not inited,m_bInited=%d", m_bInited));

    m_pobjPlayer->FreeMsg(static_cast<SmePlayerMsgBase*>(pvMsg));

    return;
}
