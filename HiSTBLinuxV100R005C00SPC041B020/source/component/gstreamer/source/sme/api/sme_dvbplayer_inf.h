/**************************************************************************
  Copyright (C), 2016-2026, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_dvbplayer_interface.h
 * @brief   sme  dvb player 接口定义
 * @date    2016/4/19
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/4/19
 * Desc   : Created file
 *
******************************************************************************/
#ifndef __SME_DVB_PLAYER_INF_H__
#define __SME_DVB_PLAYER_INF_H__

#include "osal_type.h"
#include "sme_dvbplayer_type.h"
#include "sme_media_type.h"

class ISmeDvbPlayerObs
{
public:
    virtual ~ISmeDvbPlayerObs(){};
    virtual V_UINT32 PlayerNotify(V_UINT32 u32What,
                                V_UINT32 u32Extra1,
                                V_UINT32 u32Extra2) = 0;
};

class ISmeDvbPlayer
{
public:
    static ISmeDvbPlayer*  CreateSmeDvbPlayer(E_DVB_TYPE eDvbType = E_DVB_TYPE_LIVE);
    static V_VOID          DestroySmeDvbPlayer(INOUT ISmeDvbPlayer* pobjPlayer);
    static const V_CHAR*   GetVersionInfo();
    virtual                ~ISmeDvbPlayer(){};
    virtual V_UINT32       SetObs(IN const ISmeDvbPlayerObs* pobjObs) = 0;
    virtual V_UINT32       SetProgMediaInfo(IN const ST_DVBProgMediaInfo& rstProgMediaInfo) = 0;
    virtual V_UINT32       SetProgAudioInfo(IN const ST_DVBProgAudioInfo& rstProgAudioInfo) = 0;
    virtual V_UINT32       SetVDispRect(IN const ST_SME_DVBDISP_RECT& rstDispRect) = 0;
    virtual V_UINT32       PrepareAsync() = 0;
    virtual V_UINT32       Prepare() = 0;
    virtual V_UINT32       Start() = 0;
    virtual V_UINT32       Pause() = 0;
    virtual V_UINT32       Resume() = 0;
    virtual V_UINT32       SeekTo(IN V_UINT64 seekTime) = 0;//second
    virtual V_UINT32       StartAsync() = 0;
    virtual V_UINT32       Stop(E_DVB_CC_MODE eCCMode = E_DVB_CC_MODE_LAST) = 0;
    virtual V_UINT32       SetPace(IN V_UINT32 pace) = 0;
    virtual V_UINT32       Reset() = 0;
    virtual V_INT32        IsPlaying() = 0;
    virtual V_UINT32       SetDisplayMode(IN E_SME_DVBPLAYER_DISPLAY_MODE eMode) = 0;
    virtual V_UINT32       SetDisplayRatio(IN ST_DispRatio* pstRatio) = 0;
    virtual V_UINT32       SetProgCaInfo(IN const ST_DVBProgCAInfo& rProgCAInfo) = 0;
    virtual V_UINT32       SetProgKeyInfo(IN E_DVB_CA_KEY_ID ekeyId, IN const ST_CAKeyParam& rKeyInfo) = 0;
    virtual V_UINT32       SetProgKeyIV(IN E_DVB_CA_KEY_IV_ID ekeyIvId, IN const ST_CAIVParam& rKeyIV) = 0;
    virtual E_SME_DVBPLAYER_STATE GetDvbPlayerState() = 0;
    virtual V_UINT32       GetDisplayMode(OUT E_SME_DVBPLAYER_DISPLAY_MODE& reMode) = 0;
    virtual V_UINT32       GetDisplayRatio(OUT ST_DispRatio& rstDispRatio) = 0;
    virtual V_UINT32       GetDisplayRect(OUT ST_SME_DVBDISP_RECT& rstDispRect) = 0;
    virtual V_UINT32       SetVFreezeMode(IN E_DVB_FREEZE_MODE eMode) = 0;
#ifdef __LINUX__
    virtual V_UINT32       SetAudioChannelMode(IN const A_TRACK_CHANNEL_MODE_E mode) = 0;
    virtual V_UINT32       GetAudioChannelMode(INOUT A_TRACK_CHANNEL_MODE_E* mode) = 0;
#endif
};
#endif//__SME_DVB_PLAYER_INF_H__
