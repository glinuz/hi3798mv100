/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    test_dvb_player.cpp
 * @brief   ≤‚ ‘ smedvbplayer
 * @author
 * @date    2016/5/9
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2016/5/9
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>

#include <utils/Log.h>
#include <DTVAL.h>
#include <DTVALDATA.h>
#define DCAS_CONTROL
#ifdef DCAS_CONTROL //need open when DCAS is surpport
#include "DCASAL.h"
#include "DCASALDATA.h"
#endif

#include "osal_thread.h"
#include "osal_type.h"
#include "sme_log.h"
#include "sme_macro.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "sme_timer.h"
#include "sme_media_type.h"
#include "sme_dvbplayer_type.h"
#include "sme_dvbplayer_inf.h"
#include "sme_init.h"

#ifdef __USE_ANDROID_SURFACE__
#include "native_surface_inf.h"
#endif

#define STD_OUT printf


#define DVB_PLAYER_TAG               "dvb://"
#define DVB_ATRACK_TAG               "atrack://"

#define DVB_ID_TYPE_NUM_MAX          (5)
#define DVB_ID_CHAR_NUM_MAX          (4)
#define DVB_CA_MODE_0                (0)
#define DVB_CA_MODE_1                (1)
#define DVB_CA_MODE_2                (2)
#define DVB_CA_MODE_3                (3)


#define DISPLAY_STARTX 0
#define DISPLAY_STARTY 0
#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720
#define DVB_PLAYER_CA_DATA_GROP_NUM (8)


static V_BOOL g_sbOnErr = ICS_FALSE;
static V_BOOL g_sbOnPrepared = ICS_FALSE;

static VOSTHDHDL g_shdlRunner = NULL;
static V_CHAR g_scInputType = 0;
static V_BOOL g_sbQuitPlay = ICS_FALSE;

static V_CHAR* g_spszUri = NULL;
static VideoCodec_e g_seDTVVidCodec = VideoCodec_eMax;
static AudioCodec_e g_seDTVAudCodec = AudioCodec_eMax;
static ST_DVBProgAudioInfo g_rstProgAudioInfo;

static V_UINT32 g_u32TestCount = 1;
static V_UINT32 g_u32SleepInterval = 5;//ms
static V_CHAR   g_scLogLevel = 'W';//


static ST_VOS_THD_MUTEX     g_mutex = VOS_PTHREAD_MUTEX_INITIALIZER;
#define COMMAND_LOCK  VOS_ThdMutexLock(&g_mutex);
#define COMMAND_UNLOCK VOS_ThdMutexUnLock (&g_mutex);

static V_INT32  g_i32TunerID = 0;
static V_INT32  g_i32DemuxID = 0;
static V_INT32  g_i32ServiceID;
static V_INT32  g_i32OrgNetID;
static V_INT32  g_i32TsID;
static V_INT32  g_i32CaFlag;

ST_DVBProgMediaInfo g_stMediaInfo;
ST_DVBProgCAInfo    g_stCaInfo;
CA_DATA             g_stVideoCAData[DVB_PLAYER_CA_DATA_GROP_NUM];
CA_DATA             g_stAudioCAData[DVB_PLAYER_CA_DATA_GROP_NUM];


static V_UINT32 GetAudioInfoFromUri(const char* uri ,
    ST_DVBProgAudioInfo* pAudioInfo, V_BOOL bHavePrefix = ICS_FALSE);

static V_UINT32 SetProgCaInfo();

const V_CHAR *g_DvbChannelList[] =
{
#if 0
  "dvb://23f.23.dad",
  "dvb://23f.23.dae"
#else
  "dvb://1.2.15",
  "dvb://1.2.16",
  "dvb://1.2.17",
  "dvb://1.2.18",
  "dvb://1.2.19",
  "dvb://1.2.1a",
  "dvb://1.2.1b",
  "dvb://1.2.1c"
#endif
};

#define MAX_CHANNEL_NUM 64
#define MAX_URI_LEN 1024

static V_UINT32 g_su32ValidChannel = 0;
static V_CHAR g_sacDvbChannelList[MAX_CHANNEL_NUM][MAX_URI_LEN];

static V_UINT32 g_su32ValidAudioTrack = 0;
static V_CHAR g_sacDvbAudioTrackList[MAX_CHANNEL_NUM][MAX_URI_LEN];

static V_UINT32 g_curChannelIndex = 0;
static V_UINT32 g_TotalChannelNum = 0;
static V_UINT32 TestRunner_MemLeak_SwitchCC_Destroy(INOUT V_VOID* pvCtx);
static V_VOID StartDvbPlayerOfMemLeak_SwitchCC_Destroy();


#define DELAY_TIME(x)\
for(V_UINT32 j = 0 ; j < x; j++)\
{\
    VOS_Sleep(1);\
    if(g_sbQuitPlay)\
    {\
        goto EXIT;\
    }\
}


#define TRANS_CODEC_VDEIO(src,des) \
do \
{  \
    des = E_DVB_VID_CODEC_TYPE_UNKNOWN; \
    switch (src) \
    { \
        case VideoCodec_eMpeg1: \
            des = E_DVB_VID_CODEC_TYPE_MPEG1; \
            break; \
        case VideoCodec_eMpeg2: \
            des = E_DVB_VID_CODEC_TYPE_MPEG2; \
            break; \
        case VideoCodec_eMpeg4Part2: \
            des = E_DVB_VID_CODEC_TYPE_MPEG4; \
            break; \
        case VideoCodec_eH263: \
            break; \
        case VideoCodec_eH264: \
            des = E_DVB_VID_CODEC_TYPE_H264; \
            break; \
        case VideoCodec_eH264_Svc: \
            break; \
        case VideoCodec_eH264_Mvc: \
            des = E_DVB_VID_CODEC_TYPE_MVC; \
            break; \
        case VideoCodec_eVc1: \
        case VideoCodec_eVc1SimpleMain: \
            break; \
        case VideoCodec_eDivx311: \
            break; \
        case VideoCodec_eAvs: \
            des = E_DVB_VID_CODEC_TYPE_AVS; \
            break; \
        case VideoCodec_eRv40: \
            break; \
        case VideoCodec_eVp6: \
            break; \
        case VideoCodec_eVp7: \
            break; \
        case VideoCodec_eVp8: \
            break; \
        case VideoCodec_eSpark: \
            break; \
        default: \
            break; \
    } \
}while(0)


#define TRANS_CODEC_AUDIO(src,des)\
do \
{ \
    des = E_DVB_AUD_CODEC_TYPE_UNKNOWN; \
    switch (src) \
    { \
        case AudioCodec_eMpeg: \
        case AudioCodec_eMp3: \
            des = E_DVB_AUD_CODEC_TYPE_MP2; \
            break; \
        case AudioCodec_eAac: \
        case AudioCodec_eAacLoas: \
        case AudioCodec_eAacPlus: \
        case AudioCodec_eAacPlusAdts: \
            des = E_DVB_AUD_CODEC_TYPE_AAC; \
            break; \
        case AudioCodec_eAc3: \
            des = E_DVB_AUD_CODEC_TYPE_AC3; \
            break; \
        case AudioCodec_eAc3Plus: \
            des = E_DVB_AUD_CODEC_TYPE_EAC3; \
            break; \
        case AudioCodec_eDts: \
            des = E_DVB_AUD_CODEC_TYPE_DTS; \
            break; \
        case AudioCodec_eLpcmDvd: \
        case AudioCodec_eLpcmHdDvd: \
        case AudioCodec_eLpcmBluRay: \
            des = E_DVB_AUD_CODEC_TYPE_LPCM; \
            break; \
        case AudioCodec_eDtsHd: \
            des = E_DVB_AUD_CODEC_TYPE_DTS; \
            break; \
        case AudioCodec_eWmaStd: \
            break; \
        case AudioCodec_eWmaStdTs: \
            break; \
        case AudioCodec_eWmaPro: \
            break; \
        case AudioCodec_eAvs: \
            break; \
        case AudioCodec_ePcm: \
            break; \
        case AudioCodec_ePcmWav: \
            break; \
        case AudioCodec_eAmr: \
            break; \
        case AudioCodec_eDra: \
            des = E_DVB_AUD_CODEC_TYPE_DRA; \
            break; \
        case AudioCodec_eCook: \
            break; \
        case AudioCodec_eAdpcm: \
            break; \
        case AudioCodec_eSbc: \
            break; \
        case AudioCodec_eDtsLegacy: \
            break; \
        case AudioCodec_eVorbis: \
            break; \
    } \
}while(0)

static VideoCodec_e inputVcodec2DTVVidCodec(const V_CHAR* input_type)
{
    VideoCodec_e eRet = VideoCodec_eMax;

    if(!input_type)
    {
        STD_OUT("no input video type!");STD_OUT("\n");
        ICS_LOGE("no input video type!");
        return eRet;
    }
    STD_OUT("input video type:%s", input_type);STD_OUT("\n");
    ICS_LOGE("input video type:%s", input_type);

    if(!strcmp(input_type, "avs"))
    {
        eRet = VideoCodec_eAvs;
    }

    STD_OUT("input video type:%s, eRet:%d", input_type, eRet);STD_OUT("\n");
    ICS_LOGE("input video type:%s, eRet:%d", input_type, eRet);

    return eRet;
}

static AudioCodec_e inputVcodec2DTVAudCodec(const V_CHAR* input_type)
{
    AudioCodec_e eRet = AudioCodec_eMax;

    if(!input_type)
    {
        STD_OUT("no input audio type!");STD_OUT("\n");
        ICS_LOGE("no input audio type!");
        return eRet;
    }
    STD_OUT("input audio type:%s", input_type);STD_OUT("\n");
    ICS_LOGE("input audio type:%s", input_type);

    if(!strcmp(input_type, "mp1"))
    {
        eRet = AudioCodec_eMp3;
    }

    STD_OUT("input audio type:%s, eRet:%d", input_type, eRet);STD_OUT("\n");
    ICS_LOGE("input audio type:%s, eRet:%d", input_type, eRet);

    return eRet;
}

class GSmeDvbPlayerObs;
static GSmeDvbPlayerObs* g_spobjPlayerObs = NULL;
static ISmeDvbPlayer* g_spobjPlayer = NULL;

class GSmeDvbPlayerObs : public ISmeDvbPlayerObs
{
public:
    GSmeDvbPlayerObs() {};
    virtual      ~GSmeDvbPlayerObs() {};

    V_UINT32 PlayerNotify(  V_UINT32 u32What,
                            V_UINT32 u32Extra1,
                            V_UINT32 u32Extra2)
    {
        ICS_LOGE("PlayerNotify in, u32What=%u, u32Extra1=%u, u32Extra2=%u",
                u32What, u32Extra1, u32Extra2);

        switch (u32What)
        {
            case E_SME_DVBPLAYER_NOTIFY_ERROR:
            {
                ICS_LOGE("SME-INFO:OnErr,eErr=%u,u32Extra2=%u", u32Extra1, u32Extra2);
                if (u32Extra1 == E_SME_DVBPLAYER_ERROR_UNKOWN)
                {
                    STD_OUT("/*******SMEDVB-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNKOWN*******/\n");
                    ICS_LOGE("SME-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNKOWN");
                    g_sbOnErr = ICS_TRUE;
                }
                else if (u32Extra1 == E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC)
                {
                    STD_OUT("/*******SMEDVB-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC*******/\n");
                    ICS_LOGE("SME-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNSUPPORT_AUDIO_CODEC");
                    g_sbOnErr = ICS_FALSE;
                }
                else if (u32Extra1 == E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC)
                {
                    STD_OUT("/*******SMEDVB-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC*******/\n");
                    ICS_LOGE("SME-INFO:OnErr:E_SME_DVBPLAYER_ERROR_UNSUPPORT_VIDEO_CODEC");
                    g_sbOnErr = ICS_FALSE;
                }
                else
                {
                    g_sbOnErr = ICS_TRUE;
                }

                break;
            }
            case E_SME_DVBPLAYER_NOTIFY_PREPARED:
            {
                STD_OUT("/*******DVB-INFO:OnPrepared*******/");
                ICS_LOGI("SMEDVB-INFO:OnPrepared!");
                g_sbOnPrepared = ICS_TRUE;
                break;
            }
            case E_SME_DVBPLAYER_NOTIFY_HAVE_NO_STREAM:
            {
                ICS_LOGI("have_no_stream come to test_sme_dvbplayer in");
                E_DVB_FREEZE_MODE efree_mode = E_DVB_FREEZE_MODE_BLACK;
                g_spobjPlayer->SetVFreezeMode(efree_mode);
                ICS_LOGI("have_no_stream come to test_sme_dvbplayer end");
                break;
            }
            default:
                break;
        }

        return 0;
    }
};


V_VOID PrintHelpOfTestCase()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:1 for normal dvbplayer test(use prepareAsync)");STD_OUT("\n");
    STD_OUT("Input:2 for normal dvbplayer test(use prepare)");STD_OUT("\n");
    STD_OUT("Input:3 for MemLeak Switch channel");STD_OUT("\n");
    STD_OUT("Input:4 for StartResetTest for dvb player");STD_OUT("\n");
    STD_OUT("Input:5 for StartExit Test for dvb player");STD_OUT("\n");
    STD_OUT("Input:6 for SwitchAudioTrack Test for dvb player(131freq)");STD_OUT("\n");
    STD_OUT("Input:7 for CC-Stop-Reset-Destroy for longtime test");STD_OUT("\n");
    STD_OUT("Input:8 for StartStopResetDestroy Test for dvb player");STD_OUT("\n");
    STD_OUT("Input:9 for StartStopReset not Destroy Test for dvb player");STD_OUT("\n");
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}

V_VOID PrintHelp()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:1 for test prepareAync");STD_OUT("\n");
    STD_OUT("Input:2 for test prepare");STD_OUT("\n");
    STD_OUT("Input:3 for test start->reset press");STD_OUT("\n");
    STD_OUT("Input:4 for test memleak,case:start,seek,pause,resume,seek, pause,resume, close.");STD_OUT("\n");
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}

V_VOID PrintPlayerOpHelp()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:sc for switch channel");STD_OUT("\n");
    STD_OUT("Input:sa for switch audio track");STD_OUT("\n");
    STD_OUT("Input:st for start test for dvb player");STD_OUT("\n");
    STD_OUT("Input:sp for stop test for dvb player");STD_OUT("\n");
    STD_OUT("Input:sm for set display mode,such as: sm k x y, k=1/0;x/y=16/9,5/4,4/3,3/2");STD_OUT("\n");
    STD_OUT("Input:sr for set display rect,such as: sr x y w h");STD_OUT("\n");
    STD_OUT("Input:gm for get display mode");STD_OUT("\n");
    STD_OUT("Input:gr for get display rect");STD_OUT("\n");
#ifdef __LINUX__
#if 0
    STD_OUT("Input:\"a1 1/0\" for set audio mute");STD_OUT("\n");
    STD_OUT("Input:\"a2 \" for get audio mute");STD_OUT("\n");
    STD_OUT("Input:\"a3 0-100\" for set audio volume");STD_OUT("\n");
    STD_OUT("Input:\"a4 \" for get audio volume");STD_OUT("\n");
#endif
    STD_OUT("Input:\"a5 stereo|left|right|mono|exchange|left_only|right_only|mute\" for set audio channel mode");STD_OUT("\n");
    STD_OUT("Input:\"a6 \" for get audio channel mode");STD_OUT("\n");
#endif
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}


void dcasCallback(void* pECWData, void* pPlayer)
{
#ifdef DCAS_CONTROL
    ST_CAKeyParam stCaOddKeyInfo;
    ST_CAKeyParam stCaEvenKeyInfo;
    bool          bValidCw = false;
    DCAS_NOTIFY_RET_DATA* pCWInfo = NULL;
    ISmeDvbPlayer*    player = (ISmeDvbPlayer*)pPlayer;

    if ((!player) || (!pECWData))
    {
        ICS_LOGE("Callback from DCAS param error !");
        return;
    }

    pCWInfo = (DCAS_NOTIFY_RET_DATA*)pECWData;

    if (g_i32CaFlag)
    {
        for (unsigned int i = 0; i < pCWInfo->cw_key.key.length; i++)
        {
            if (pCWInfo->cw_key.key.value[i])
            {
                bValidCw = true;
                break;
            }
        }

        if (!bValidCw)
        {
            ALOGD("DVBPlayerProcess DCAS CW is invalid!");
            return;
        }

        if (0 == pCWInfo->scheme_id)
        {
            g_stCaInfo.audio_ca_type = DVB_CA_MODE_2;
            g_stCaInfo.video_ca_type = DVB_CA_MODE_2;
            g_stCaInfo.audio_stream_descramble_type = 0;
            g_stCaInfo.video_stream_descramble_type = 0;
        }
        else if (1 == pCWInfo->scheme_id)
        {
            g_stCaInfo.audio_ca_type = DVB_CA_MODE_3;
            g_stCaInfo.video_ca_type = DVB_CA_MODE_3;
            g_stCaInfo.audio_stream_descramble_type = 0;
            g_stCaInfo.video_stream_descramble_type = 0;
        }
        else
        {
            g_stCaInfo.audio_ca_type =
                pCWInfo->level_keys[0].encrypted == false ?
                DVB_CA_MODE_1 : DVB_CA_MODE_0;
            g_stCaInfo.video_ca_type =
                pCWInfo->level_keys[0].encrypted == false ?
                DVB_CA_MODE_1 : DVB_CA_MODE_0;
        }

        g_stCaInfo.ca_vendor_id = pCWInfo->vendor_id;

        V_UINT32 ret = SetProgCaInfo();
        UTILS_MLOGE_IF(ret == ICS_FAIL, ("player->SetProgCaInfo failed"));
        g_i32CaFlag = 0;
    }

    memset(&stCaOddKeyInfo, 0, sizeof(stCaOddKeyInfo));
    memset(&stCaEvenKeyInfo, 0, sizeof(stCaEvenKeyInfo));

    if (g_stMediaInfo.apid == (unsigned int)pCWInfo->data_pid)
    {
        if (pCWInfo->cw_key.parity == PARITY_ODD)
        {
            if (true == pCWInfo->level_keys[0].encrypted)
            {
                memcpy(stCaOddKeyInfo.au8_ek1, pCWInfo->level_keys[0].value,
                    pCWInfo->level_keys[0].length);
                memcpy(stCaOddKeyInfo.au8_ek2, pCWInfo->level_keys[1].value,
                    pCWInfo->level_keys[1].length);
            }

            memcpy(stCaOddKeyInfo.au8_ecw, pCWInfo->cw_key.key.value,
                pCWInfo->cw_key.key.length);
            player->SetProgKeyInfo(E_DVB_CA_SET_AUDIO_ODD_KEY, stCaOddKeyInfo);
        }
        else if (pCWInfo->cw_key.parity == PARITY_EVEN)
        {
            if (true == pCWInfo->level_keys[0].encrypted)
            {
                memcpy(stCaEvenKeyInfo.au8_ek1, pCWInfo->level_keys[0].value,
                    pCWInfo->level_keys[0].length);
                memcpy(stCaEvenKeyInfo.au8_ek2, pCWInfo->level_keys[1].value,
                    pCWInfo->level_keys[1].length);
            }

            memcpy(stCaEvenKeyInfo.au8_ecw, pCWInfo->cw_key.key.value,
                pCWInfo->cw_key.key.length);
            player->SetProgKeyInfo(E_DVB_CA_SET_AUDIO_EVEN_KEY, stCaEvenKeyInfo);
        }
    }
    else if (g_stMediaInfo.vpid == (unsigned int)pCWInfo->data_pid)
    {
        if (pCWInfo->cw_key.parity == PARITY_ODD)
        {
            if (true == pCWInfo->level_keys[0].encrypted)
            {
                memcpy(stCaOddKeyInfo.au8_ek1, pCWInfo->level_keys[0].value,
                    pCWInfo->level_keys[0].length);
                memcpy(stCaOddKeyInfo.au8_ek2, pCWInfo->level_keys[1].value,
                    pCWInfo->level_keys[1].length);
            }

            memcpy(stCaOddKeyInfo.au8_ecw, pCWInfo->cw_key.key.value,
                pCWInfo->cw_key.key.length);
            player->SetProgKeyInfo(E_DVB_CA_SET_VIDEO_ODD_KEY, stCaOddKeyInfo);
        }
        else if (pCWInfo->cw_key.parity == PARITY_EVEN)
        {
            if (true == pCWInfo->level_keys[0].encrypted)
            {
                memcpy(stCaEvenKeyInfo.au8_ek1, pCWInfo->level_keys[0].value,
                    pCWInfo->level_keys[0].length);
                memcpy(stCaEvenKeyInfo.au8_ek2, pCWInfo->level_keys[1].value,
                    pCWInfo->level_keys[1].length);
            }

            memcpy(stCaEvenKeyInfo.au8_ecw, pCWInfo->cw_key.key.value,
                pCWInfo->cw_key.key.length);
            player->SetProgKeyInfo(E_DVB_CA_SET_VIDEO_EVEN_KEY, stCaEvenKeyInfo);
        }
    }
#endif
}



static V_UINT32 SetVideoArea(int x, int y, int width, int height)
{
    ST_SME_DVBDISP_RECT mDispRect;
    V_UINT32 err;

    mDispRect.i32StartX = x;
    mDispRect.i32StartY = y;
    mDispRect.i32Width = width;
    mDispRect.i32Height = height;

    err = g_spobjPlayer->SetVDispRect(mDispRect);
    ICS_LOGI("set video display rect: %d, %d, %d, %d",
        x, y, width, height);

    return err;

}

static V_UINT32 GetProgInfoFromUri(const char* url)
{
    unsigned int char_index = 0;
    unsigned int id_index = 0;
    int length = 0;
    char id_str[DVB_ID_TYPE_NUM_MAX][DVB_ID_CHAR_NUM_MAX + 1] = {{0}};
    char *str = NULL;

    if (!url || !strstr(url, DVB_PLAYER_TAG))
    {
        ICS_LOGE("url is invalid!");
        return ICS_FAIL;
    }

    length = strlen(url);

    if (length <= strlen(DVB_PLAYER_TAG))
    {
        ICS_LOGE("url is invalid, too short!");
        return ICS_FAIL;
    }

    str = (char *)(url + strlen(DVB_PLAYER_TAG));

    for (int i = 0; i < length; i++)
    {
        if (isxdigit(str[i]))
        {
            if (char_index > DVB_ID_CHAR_NUM_MAX)
            {
                ICS_LOGE("every id should only have 4 hex id_indexs");
                return ICS_FAIL;
            }

            id_str[id_index][char_index] = str[i];
            char_index ++;
        }
        else if ('.' == str[i])
        {
            if (!char_index)
            {
                ICS_LOGE("You should have numbers before '.'");
                return ICS_FAIL;
            }

            char_index = 0;
            id_index ++;

            if (id_index > DVB_ID_TYPE_NUM_MAX - 1)
            {
                ICS_LOGE("We can only process 4 ids");
                break;
            }
        }
        else
        {
            ICS_LOGE("unkown char str[%d]:%c", i, str[i]);
            break;
        }
    }

    if (id_index < 2)
    {
        ICS_LOGE("We need at least 3 ids");
        return ICS_FAIL;
    }

    ICS_LOGE("id_str[0]=%s, id_str[1]=%s, id_str[2]=%s, id_str[3]=%s, id_str[4]=%s",
        id_str[0], id_str[1], id_str[2], id_str[3], id_str[4]);

    g_i32OrgNetID = (int)strtol(id_str[0], NULL, 16);
    g_i32TsID = (int)strtol(id_str[1], NULL, 16);
    g_i32ServiceID = (int)strtol(id_str[2], NULL, 16);

    if (id_index > 2)
    {
        g_stMediaInfo.vpid = (int)strtol(id_str[3], NULL, 16);
    }

    if (id_index > 3)
    {
        g_stMediaInfo.apid = (int)strtol(id_str[4], NULL, 16);
    }

    return ICS_SUCCESS;
}

static V_VOID GetProgInfo()
{
    int i;

    NGB_SERVICELOCATOR locator;
    live_config TV_live;

    locator.ri_NetworkId = -1;
    locator.ri_OrgNetId = g_i32OrgNetID;
    locator.ri_ServiceID = g_i32ServiceID;
    locator.ri_TsID = g_i32TsID;

    memset(&TV_live, 0x0, sizeof(live_config));

    DTVAL_getLiveConfig(locator, &TV_live);
#if 0 // test code
    if (TV_live.Audio.pid == 0 || TV_live.Audio.codec == 0) {
        TV_live.Audio.pid = 0x201;
        TV_live.Audio.codec = AudioCodec_eDra;
    }
#endif

    if(VideoCodec_eMax != g_seDTVVidCodec)
    {
        TV_live.Video.codec = g_seDTVVidCodec;
        STD_OUT("use input video type:%d", g_seDTVVidCodec);STD_OUT("\n");
        ICS_LOGE("use input video type:%d", g_seDTVVidCodec);
    }

    if(AudioCodec_eMax != g_seDTVAudCodec)
    {
        TV_live.Audio.codec = g_seDTVAudCodec;
        STD_OUT("use input audio type:%d", g_seDTVAudCodec);STD_OUT("\n");
        ICS_LOGE("use input audio type:%d", g_seDTVAudCodec);
    }

    g_stMediaInfo.apid = TV_live.Audio.pid;
    g_stMediaInfo.vpid = TV_live.Video.pid;
    TRANS_CODEC_VDEIO(TV_live.Video.codec, g_stMediaInfo.e_vtype);
    TRANS_CODEC_AUDIO(TV_live.Audio.codec, g_stMediaInfo.e_atype);
    g_stMediaInfo.pcrpid = TV_live.pcr_pid;
    g_stMediaInfo.demux_id = g_i32DemuxID;
    g_stMediaInfo.audio_is_ca = 0;
    g_stMediaInfo.video_is_ca = 0;

    memcpy(g_stVideoCAData, TV_live.Video.ca_data,
        DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));
    memcpy(g_stAudioCAData, TV_live.Audio.ca_data,
        DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));

    for (i = 0; i < DVB_PLAYER_CA_DATA_GROP_NUM; i++)
    {
        if ((g_stVideoCAData[i].cas_id != 0)
            && (g_stVideoCAData[i].ecm_pid > 0)
            && (g_stVideoCAData[i].ecm_pid < 0x1FFF))
        {
            g_stMediaInfo.video_is_ca = 1;
            ICS_LOGE("use ca mode\n");
            break;
        }
    }

    for (i = 0; i < DVB_PLAYER_CA_DATA_GROP_NUM; i++)
    {
        if ((g_stAudioCAData[i].cas_id != 0)
            && (g_stAudioCAData[i].ecm_pid > 0)
            && (g_stAudioCAData[i].ecm_pid < 0x1FFF))
        {
            g_stMediaInfo.audio_is_ca = 1;
            ICS_LOGE("use ca mode\n");
            break;
        }
    }


    ICS_LOGI("vid[%d] vtype[%d] aid[%d] atype[%d], audio_is_ca:%d, video_is_ca:%d",
        g_stMediaInfo.vpid,TV_live.Video.codec, g_stMediaInfo.apid,
        TV_live.Audio.codec, g_stMediaInfo.audio_is_ca,  g_stMediaInfo.video_is_ca);
    STD_OUT("vid[%d] vtype[%d] aid[%d] atype[%d], audio_is_ca:%d, video_is_ca:%d\n",
        g_stMediaInfo.vpid,TV_live.Video.codec, g_stMediaInfo.apid,
        TV_live.Audio.codec, g_stMediaInfo.audio_is_ca, g_stMediaInfo.video_is_ca);

    DTVAL_tuneStream(g_i32TunerID, g_i32OrgNetID, g_i32TsID);

    return ;
}

static V_UINT32 SetProgCaInfo()
{
    V_UINT32  err;
    ST_DVBProgCAInfo stCaInfo;

    UTILS_MLOGE_RET_VAL_IF(g_spobjPlayer == NULL, ICS_FAIL, ("dvbplayer not init"));

    memset(&stCaInfo, 0, sizeof(ST_DVBProgCAInfo));
    stCaInfo.ca_vendor_id = g_stCaInfo.ca_vendor_id;
    stCaInfo.audio_ca_type = g_stCaInfo.audio_ca_type;
    stCaInfo.video_ca_type = g_stCaInfo.video_ca_type;
    stCaInfo.audio_stream_descramble_type = g_stCaInfo.audio_stream_descramble_type;
    stCaInfo.video_stream_descramble_type = g_stCaInfo.video_stream_descramble_type;

    ALOGD("vendor_id[%d],audio_ca_type[%d],audio_stream_desc[%d],"
        "video_ca_type[%d],video_stream_desc[%d]",
        stCaInfo.ca_vendor_id, stCaInfo.audio_ca_type,
        stCaInfo.audio_stream_descramble_type,
        stCaInfo.video_ca_type, stCaInfo.video_stream_descramble_type);

    err = g_spobjPlayer->SetProgCaInfo(stCaInfo);
    UTILS_MLOGE_RET_VAL_IF(err == ICS_FAIL, err, ("SetProgCaInfo failed"));

    return err;
}



static V_UINT32 SetProgMediaInfo()
{
    V_UINT32  err;

    UTILS_MLOGE_RET_VAL_IF(g_spobjPlayer == NULL, ICS_FAIL, ("dvbplayer not init"));

    ICS_LOGI("apid: %d, atype: %d, vpid: %d, vtype: %d, demuxid: %d",
        g_stMediaInfo.apid, g_stMediaInfo.e_atype, g_stMediaInfo.vpid,
        g_stMediaInfo.e_vtype, g_stMediaInfo.demux_id);

    STD_OUT("apid: %d, atype: %d, vpid: %d, vtype: %d, demuxid: %d\n",
        g_stMediaInfo.apid, g_stMediaInfo.e_atype, g_stMediaInfo.vpid,
        g_stMediaInfo.e_vtype, g_stMediaInfo.demux_id);


#if 0
    g_stMediaInfo.apid = 0x28a;
    g_stMediaInfo.e_atype = E_DVB_AUD_CODEC_TYPE_MP3;
    g_stMediaInfo.vpid = 0x200;
    g_stMediaInfo.e_vtype = E_DVB_VID_CODEC_TYPE_MPEG2;
    g_stMediaInfo.pcrpid = 0x80;
    g_stMediaInfo.is_ca = 0;
#endif
    err = g_spobjPlayer->SetProgMediaInfo(g_stMediaInfo);
    UTILS_MLOGE_RET_VAL_IF(err == ICS_FAIL, err, ("SetProgMediaInfo failed"));

    return err;
}

void StartDescrambling(bool isVideo)
{
#ifdef DCAS_CONTROL

    if (isVideo)
    {
        DCAS_NOTIFY_DATA NotifyCASData = {0};
        NotifyCASData.demux_id = g_i32DemuxID;
        NotifyCASData.service_id = g_i32ServiceID;
        NotifyCASData.data_pid = g_stMediaInfo.vpid;
        memcpy(NotifyCASData.ca_data, g_stVideoCAData,
            DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));

        DCASAL_startDescrambling((void*)&NotifyCASData, dcasCallback, (void*)g_spobjPlayer);
    }
    else
    {
        DCAS_NOTIFY_DATA NotifyCASData = {0};
        NotifyCASData.demux_id = g_i32DemuxID;
        NotifyCASData.service_id = g_i32ServiceID;
        NotifyCASData.data_pid = g_stMediaInfo.apid;
        memcpy(NotifyCASData.ca_data, g_stAudioCAData,
            DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));
        DCASAL_startDescrambling((void*)&NotifyCASData, dcasCallback, (void*)g_spobjPlayer);
    }

#endif
}

void StopDescrambling(bool isVideo)
{
#ifdef DCAS_CONTROL

    if (isVideo)
    {
        DCAS_NOTIFY_DATA NotifyCASData = {0};
        NotifyCASData.demux_id = g_i32DemuxID;
        NotifyCASData.service_id = g_i32ServiceID;
        NotifyCASData.data_pid = g_stMediaInfo.vpid;
        memcpy(NotifyCASData.ca_data, g_stVideoCAData,
            DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));
        DCASAL_stopDescrambling((void*)&NotifyCASData, dcasCallback, (void*)g_spobjPlayer);
    }
    else
    {
        DCAS_NOTIFY_DATA NotifyCASData = {0};
        NotifyCASData.demux_id = g_i32DemuxID;
        NotifyCASData.service_id = g_i32ServiceID;
        NotifyCASData.data_pid =  g_stMediaInfo.apid;
        memcpy(NotifyCASData.ca_data, g_stAudioCAData,
            DVB_PLAYER_CA_DATA_GROP_NUM * sizeof(CA_DATA));
        DCASAL_stopDescrambling((void*)&NotifyCASData, dcasCallback, (void*)g_spobjPlayer);
    }
#endif
}

void StopDcasGroupDescrambling()
{
    ICS_LOGI("stop video_ca_type=%d", g_stCaInfo.video_ca_type);
    if (g_stCaInfo.video_ca_type != 0)
    {
        StopDescrambling(true);
    }

    ICS_LOGI("stop audio_ca_type=%d", g_stCaInfo.audio_ca_type);
    if (g_stCaInfo.audio_ca_type != 0)
    {
        StopDescrambling(false);
    }

    if (g_stCaInfo.audio_ca_type != 0
        && g_stCaInfo.video_ca_type != 0)
    {
        ICS_LOGI("stop mCaFlag[%d]\n",g_i32CaFlag);
        g_i32CaFlag = 0;
    }
}


void StartDcasGroupDescrambling()
{
#ifdef DCAS_CONTROL
    if (g_stMediaInfo.audio_is_ca)
    {
        g_i32CaFlag = 1;
        ICS_LOGI("start ca startDescrambling mCaFlag[%d]\n", g_i32CaFlag);
        StartDescrambling(0);
    }

    if(g_stMediaInfo.video_is_ca){
        g_i32CaFlag = 1;
        ICS_LOGI("start ca startDescrambling mCaFlag[%d]\n", g_i32CaFlag);
        StartDescrambling(1);
    }

    if (!g_stMediaInfo.audio_is_ca && !g_stMediaInfo.video_is_ca)
    {
        g_i32CaFlag = 0;
        g_stCaInfo.audio_ca_type = 0;
        g_stCaInfo.video_ca_type = 0;
        ICS_LOGI("start ca startDescrambling mCaFlag[%d]\n",g_i32CaFlag);
    }

#endif
}



static V_UINT32 SetDataSource(const char* url)
{
    V_UINT32 ret;

    STD_OUT("SetDataSource in!\n");
    ret = GetProgInfoFromUri(url);
    UTILS_MLOGE_RET_VAL_IF(ret == ICS_FAIL, ret, ("Get program info from url fail"));

    ICS_LOGE("Service ids: %d, %d, %d, %d, %d",
        g_i32OrgNetID, g_i32TsID, g_i32ServiceID,
        g_stMediaInfo.vpid, g_stMediaInfo.apid);

    STD_OUT("Service ids: 0x%x(%d), 0x%x(%d), 0x%x(%d), 0x%x(%d), 0x%x(%d)\n",
        g_i32OrgNetID, g_i32OrgNetID,
        g_i32TsID, g_i32TsID,
        g_i32ServiceID,g_i32ServiceID,
        g_stMediaInfo.vpid, g_stMediaInfo.vpid,
        g_stMediaInfo.apid, g_stMediaInfo.apid);
    //ret = SetVideoArea(0,0,0,0);
    //STD_OUT("SetVideoArea ret :%d\n", ret);
    // UTILS_MLOGE_RET_VAL_IF(ret == ICS_FAIL, ret, ("set Video area fail"));

    GetProgInfo();
    //now cannot support E_DVB_AUD_CODEC_TYPE_LPCM,&& here DTV will return E_DVB_AUD_CODEC_TYPE_LPCM
    //but actualy is E_DVB_AUD_CODEC_TYPE_MP1
    //20160809,currently we modify it as below:
    if(g_stMediaInfo.e_atype == E_DVB_AUD_CODEC_TYPE_LPCM){
        g_stMediaInfo.e_atype = E_DVB_AUD_CODEC_TYPE_MP1;
    }

    ret = SetProgMediaInfo();
    STD_OUT("SetProgMediaInfo ret :%d\n", ret);
    UTILS_MLOGE_RET_VAL_IF(ret == ICS_FAIL, ret, ("SetProgMediaInfo fail"));

    StartDcasGroupDescrambling();
    return ret;
}

static V_UINT32 StopChannel()
{
    V_UINT32 ret;

    ICS_LOGI("CC-DVB StopChannel in,ca_type=%d", g_stCaInfo.video_ca_type);
    STD_OUT("CC-DVB StopChannel in,ca_type=%d", g_stCaInfo.video_ca_type);

    UTILS_MLOGE_RET_VAL_IF(g_spobjPlayer == NULL, ICS_FAIL, ("dvbplayer not init"));

    StopDcasGroupDescrambling();

    ret = g_spobjPlayer->Stop();
    UTILS_MLOGE_RET_VAL_IF(ret == ICS_FAIL, ret, ("Stop failed"));

    ICS_LOGI("CC-DVB StopChannel out");
    STD_OUT("CC-DVB StopChannel out");

    return ret;
}


static void
save_mem_info()
{
    V_UINT32 pid = getpid();
    V_CHAR cmd[256];
    static int print_flag = 1;

    sprintf(cmd, "cat /proc/%d/status |grep VmRSS >> /data/smelog/mem_stat_%d.dat",pid,pid);
#if 0
    if (print_flag) {
       sprintf(cmd, "echo PID------Vss------Rss-------Pss------Uss----cmdline >> /data/mem_stat_%d.dat", pid);
       system(cmd);
       print_flag = 0;
    }
    VOS_Memset_S(cmd,sizeof(cmd),0,sizeof(cmd));
    sprintf(cmd, "procrank |grep %d >> /data/mem_stat_%d.dat",pid,pid);
#endif
    system(cmd);
}

static V_UINT32 PlayChannel(const char* url)
{
    V_UINT32 u32Err = ICS_FAIL;


    u32Err = StopChannel();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("StopChannel failed(%u)", u32Err);
        STD_OUT("StopChannel failed(%u)\n", u32Err);
        if(g_spobjPlayer->Reset()){
            ICS_LOGE("reset return error in libmedia\n");
            goto error;
        }

    }

    u32Err = SetDataSource(url);
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("SetDataSource failed(%u)", u32Err);
      STD_OUT("SetDataSource failed(%u)\n", u32Err);
      goto error;
    }

    u32Err = g_spobjPlayer->Prepare();
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("PrepareAsync failed(%u)", u32Err);
      STD_OUT("PrepareAsync failed(%u) \n",u32Err);
      goto error;
    }

    u32Err = g_spobjPlayer->StartAsync();
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
      goto error;
    }

    static int sleep_us = 0;

    if (sleep_us > 0) {
        usleep(sleep_us);   // 100ms~1s
    }

    ICS_LOGE("PlayChannel usleep %d ms after StartAsync", sleep_us/1000);
    STD_OUT("PlayChannel usleep %d ms after StartAsync", sleep_us/1000);

    if (sleep_us < 200*1000) {
        sleep_us += 5*1000;
    } else {
        sleep_us += 100*1000;
    }

    if (sleep_us > 1000*1000) {
        sleep_us = 0;
    }

 error:
    return u32Err;
}


static V_UINT32 ForwardChannel()
{
    V_UINT32 u32Err = ICS_FAIL;

    g_TotalChannelNum = sizeof(g_DvbChannelList) / sizeof(g_DvbChannelList[0]);
    g_curChannelIndex++;

    g_curChannelIndex = (g_curChannelIndex >= g_TotalChannelNum) ? 0: g_curChannelIndex;

    u32Err = PlayChannel(g_DvbChannelList[g_curChannelIndex]);
    UTILS_MLOGE_RET_VAL_IF(u32Err  == ICS_FAIL, u32Err , ("PlayChannel failed"));

    return u32Err;
}

static V_UINT32 BackwardChannel()
{
    V_UINT32 u32Err = ICS_FAIL;

    g_TotalChannelNum = sizeof(g_DvbChannelList) / sizeof(g_DvbChannelList[0]);

    g_curChannelIndex = (g_curChannelIndex == 0) ? (g_TotalChannelNum - 1) : --g_curChannelIndex;

    u32Err = PlayChannel(g_DvbChannelList[g_curChannelIndex]);
    UTILS_MLOGE_RET_VAL_IF(u32Err  == ICS_FAIL, u32Err , ("PlayChannel failed"));

    return u32Err;
}



static V_UINT32 TestRunnerNormal(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = ICS_FAIL;

    ICS_LOGI("TestRunnerNormal in");
    STD_OUT("TestRunnerNormal in!\n");
    g_sbOnErr = ICS_FALSE;
    E_SME_DVBPLAYER_STATE estate = E_SME_DVBPLAYER_STATE_ERROR;

    g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

    g_spobjPlayerObs = new GSmeDvbPlayerObs();
    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(ICS_SUCCESS != u32Err)
    {
        STD_OUT("set SetObs failed(%u)", u32Err);
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    //set sme playe loglevel
    //SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);


    u32Err = SetDataSource(g_spszUri);
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("SetDataSource failed(%u)", u32Err);
        STD_OUT("SetDataSource failed(%u)\n", u32Err);
        goto EXIT;
    }

    g_scInputType = '2';

    if('1' == g_scInputType)
    {
        u32Err = g_spobjPlayer->PrepareAsync();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("PrepareAsync failed(%u)", u32Err);
            STD_OUT("PrepareAsync failed(%u) \n",u32Err);
            goto EXIT;
        }
       /*
        while(ICS_FALSE == g_sbOnPrepared)
        {
            VOS_MSleep(20);
            if((ICS_FALSE != g_sbOnErr) || (ICS_FALSE != g_sbQuitPlay))
            {
                goto EXIT;
            }
        }
        */

    }
    else if ('2' == g_scInputType)
    {
        u32Err = g_spobjPlayer->Prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            goto EXIT;
        }
    }
    else
    {
        STD_OUT("g_scInputType=%c", g_scInputType);STD_OUT("\n");
        goto EXIT;
    }

    SetVideoArea(0,0,1280,720);

    u32Err = g_spobjPlayer->Start();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    estate = g_spobjPlayer->GetDvbPlayerState();
    ICS_LOGE("after Start:GetDvbPlayerState(%d)\n", estate);
    STD_OUT("after Start:GetDvbPlayerState(%d)\n", estate);

    while((ICS_FALSE == g_sbOnErr) && (ICS_FALSE == g_sbQuitPlay))
    {
        VOS_MSleep(500);
    }

EXIT:
    u32Err = g_spobjPlayer->Stop();
    STD_OUT("Stop ret=%d", u32Err); STD_OUT("\n");

    estate = g_spobjPlayer->GetDvbPlayerState();
    ICS_LOGE("after Stop:GetDvbPlayerState(%d)", estate);
    STD_OUT("after Stop:GetDvbPlayerState(%d)\n", estate);

    STD_OUT("reset begin"); STD_OUT("\n");
    u32Err = g_spobjPlayer->Reset();
    ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    delete g_spobjPlayerObs;
    g_spobjPlayerObs = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);

    ICS_LOGI("TestRunner out!!!");

    return 0;
}


static V_UINT32 TestRunnerStartReset(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunnerStartReset in");
    STD_OUT("TestRunnerStartReset in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

    g_spobjPlayerObs = new GSmeDvbPlayerObs();
    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(ICS_SUCCESS != u32Err)
    {
        STD_OUT("set SetObs failed(%u)", u32Err);
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }


    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
             i32Count++)
    {
        ICS_LOGW("========test count:%d\n",i32Count);
        STD_OUT("========test count:%d\n",i32Count);


        STD_OUT(" begin %d setDataSource... \n", i32Count);
        u32Err = SetDataSource(g_spszUri);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }

        g_scInputType = '2';
        STD_OUT(" begin %d prepare... \n", i32Count);
        if('1' == g_scInputType)
        {
            u32Err = g_spobjPlayer->PrepareAsync();
            if(ICS_SUCCESS != u32Err)
            {
                ICS_LOGE("PrepareAsync failed(%u)", u32Err);
                STD_OUT("PrepareAsync failed(%u) \n",u32Err);
                goto EXIT;
            }

        }
        else if ('2' == g_scInputType)
        {
            u32Err = g_spobjPlayer->Prepare();
            if(ICS_SUCCESS != u32Err)
            {
                ICS_LOGE("set Prepare failed(%u)", u32Err);
                goto EXIT;
            }
        }
        else
        {
            STD_OUT("g_scInputType=%c", g_scInputType);STD_OUT("\n");
            goto EXIT;
        }

        SetVideoArea(0,0,1280,720);

        STD_OUT(" begin %d start... \n", i32Count);
        u32Err = g_spobjPlayer->Start();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" start failed err:%d... \n", u32Err);
            ICS_LOGE("Start failed(%u)", u32Err);
            goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);

        STD_OUT(" begin %d reset... \n",i32Count);
        u32Err = g_spobjPlayer->Reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }

        save_mem_info();
    }


EXIT:

    STD_OUT("==reset begin\n");
    u32Err = g_spobjPlayer->Reset();
    STD_OUT("destroy dvbplayer");
    ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    delete g_spobjPlayerObs;
    g_spobjPlayerObs = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    g_sbQuitPlay = ICS_TRUE;
    STD_OUT("TestRunnerStartReset out!!!");
    ICS_LOGI("TestRunnerStartReset out!!!");

    return 0;
}


static V_UINT32 TestRunnerStartExit(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunnerStartReset in");
    STD_OUT("TestRunnerStartReset in!\n");
    g_sbOnErr = ICS_FALSE;

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
             i32Count++)
    {

        ICS_LOGW("========test count:%d\n",i32Count);
        STD_OUT("========test count:%d\n",i32Count);

         STD_OUT(" begin %d CreateSmeDvbPlayer... \n", i32Count);
        g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

        g_spobjPlayerObs = new GSmeDvbPlayerObs();
        STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT("set SetObs failed(%u)", u32Err);
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        STD_OUT(" begin %d setDataSource... \n", i32Count);
        u32Err = SetDataSource(g_spszUri);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->Prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            goto EXIT;
        }

        SetVideoArea(0,0,1280,720);

        STD_OUT(" begin %d start... \n", i32Count);
        u32Err = g_spobjPlayer->Start();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" start failed err:%d... \n", u32Err);
            ICS_LOGE("Start failed(%u)", u32Err);
            goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);

        STD_OUT(" begin %d reset... \n",i32Count);
        u32Err = g_spobjPlayer->Reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }
        STD_OUT("destroy dvbplayer");
        ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
        g_spobjPlayer = NULL;
        delete g_spobjPlayerObs;
        g_spobjPlayerObs = NULL;
        save_mem_info();
    }


EXIT:
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    g_sbQuitPlay = ICS_TRUE;
    STD_OUT("TestRunnerStartExit out!!!");
    ICS_LOGI("TestRunnerStartExit out!!!");

    return 0;
}


static V_UINT32 TestRunnerSwitchAuidoTrack(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = ICS_FAIL;
    ICS_LOGI("TestRunnerSwitchAuidoTrack in");
    STD_OUT("TestRunnerSwitchAuidoTrack in!\n");
    g_sbOnErr = ICS_FALSE;
    V_INT32 i32Count = 0;

    g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();


    g_spobjPlayerObs = new GSmeDvbPlayerObs();
    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(ICS_SUCCESS != u32Err)
    {
        STD_OUT("set SetObs failed(%u)", u32Err);
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }
    if (g_spobjPlayer == NULL)
    {
        ICS_LOGE("new DVBPlayer failed(%u)", u32Err);
        STD_OUT("new DVBPlayer failed(%u)\n", u32Err);
        goto EXIT;
    }

    STD_OUT(" begin setDataSource... \n");
    u32Err = SetDataSource(g_spszUri);
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("SetDataSource failed(%u)", u32Err);
        STD_OUT("SetDataSource failed(%u)\n", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->Prepare();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set Prepare failed(%u)", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->Start();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
             i32Count++)
    {
        V_UINT32 index = i32Count % g_su32ValidAudioTrack;

        ICS_LOGW("========test count:%d,switch to mp1\n",i32Count);
        STD_OUT("========test count:%d,switch to mp1\n",i32Count);
        memset(&g_rstProgAudioInfo, 0, sizeof(g_rstProgAudioInfo));
        GetAudioInfoFromUri(g_sacDvbAudioTrackList[index] , &g_rstProgAudioInfo, ICS_TRUE);
        g_spobjPlayer->SetProgAudioInfo(g_rstProgAudioInfo);

        VOS_MSleep(g_u32SleepInterval);
        save_mem_info();
    }
EXIT:

    STD_OUT(" begin reset... \n");
    u32Err = g_spobjPlayer->Reset();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("reset failed(%u)", u32Err);
        goto EXIT;
    }
    STD_OUT("destroy dvbplayer\n");
    ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    delete g_spobjPlayerObs;
    g_spobjPlayerObs = NULL;

    STD_OUT("TestRunnerSwitchAuidoTrack out!!!\n");
    ICS_LOGI("TestRunnerSwitchAuidoTrack out!!!");

    return 0;
}

static V_UINT32 TestRunnerStartStopResetDestroy(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunnerStartStopResetDestroy in");
    STD_OUT("TestRunnerStartStopResetDestroy in!\n");
    g_sbOnErr = ICS_FALSE;

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
             i32Count++)
    {

        ICS_LOGW("========StartStopResetDestroy test count:%d\n",i32Count);
        STD_OUT("========StartStopResetDestroy test count:%d\n",i32Count);

         STD_OUT(" begin %d CreateSmeDvbPlayer... \n", i32Count);
        g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

        g_spobjPlayerObs = new GSmeDvbPlayerObs();
        STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT("set SetObs failed(%u)", u32Err);
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        STD_OUT(" begin %d setDataSource... \n", i32Count);
        u32Err = SetDataSource(g_spszUri);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->Prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            goto EXIT;
        }

        SetVideoArea(0,0,1280,720);

        STD_OUT(" begin %d start... \n", i32Count);
        u32Err = g_spobjPlayer->Start();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" start failed err:%d... \n", u32Err);
            ICS_LOGE("Start failed(%u)", u32Err);
            //goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);

        STD_OUT(" begin %d stop... \n", i32Count);
        u32Err = g_spobjPlayer->Stop();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" stop failed err:%d... \n", u32Err);
            ICS_LOGE("Stop failed(%u)", u32Err);
            goto EXIT;
        }

        STD_OUT(" begin %d reset... \n",i32Count);
        u32Err = g_spobjPlayer->Reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }

        STD_OUT("destroy dvbplayer\n");
        ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
        g_spobjPlayer = NULL;
        delete g_spobjPlayerObs;
        g_spobjPlayerObs = NULL;
        save_mem_info();
    }


EXIT:
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    g_sbQuitPlay = ICS_TRUE;
    STD_OUT("TestRunnerStartExit out!!!");
    ICS_LOGI("TestRunnerStartExit out!!!");

    return 0;
}


static V_UINT32 TestRunnerStartStopReset(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunnerStartStopReset in");
    STD_OUT("TestRunnerStartStopReset in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

    g_spobjPlayerObs = new GSmeDvbPlayerObs();
    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(ICS_SUCCESS != u32Err)
    {
        STD_OUT("set SetObs failed(%u)", u32Err);
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
             i32Count++)
    {
        ICS_LOGW("========StartStopReset test count:%d\n",i32Count);
        STD_OUT("========StartStopReset test count:%d\n",i32Count);

        STD_OUT(" begin %d setDataSource... \n", i32Count);
        u32Err = SetDataSource(g_spszUri);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->Prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            goto EXIT;
        }

        SetVideoArea(0,0,1280,720);

        STD_OUT(" begin %d start... \n", i32Count);
        u32Err = g_spobjPlayer->Start();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" start failed err:%d... \n", u32Err);
            ICS_LOGE("Start failed(%u)", u32Err);
            //goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);

        STD_OUT(" begin %d stop... \n", i32Count);
        u32Err = g_spobjPlayer->Stop();
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT(" stop failed err:%d... \n", u32Err);
            ICS_LOGE("Stop failed(%u)", u32Err);
            goto EXIT;
        }

        STD_OUT(" begin %d reset... \n",i32Count);
        u32Err = g_spobjPlayer->Reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }

        save_mem_info();
    }


EXIT:

    STD_OUT("destroy dvbplayer");
    ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    delete g_spobjPlayerObs;
    g_spobjPlayerObs = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    g_sbQuitPlay = ICS_TRUE;
    STD_OUT("TestRunnerStartReset out!!!");
    ICS_LOGI("TestRunnerStartReset out!!!");

    return 0;
}


static char *audio_channel_mode_to_str(A_TRACK_CHANNEL_MODE_E mode)
{
    char * str = "unkown";
    switch (mode)
    {
        case A_TRACK_MODE_STEREO:
            str = "A_TRACK_MODE_STEREO";
            break;
        case A_TRACK_MODE_DOUBLE_MONO:
            str = "A_TRACK_MODE_DOUBLE_MONO";
            break;
        case A_TRACK_MODE_DOUBLE_LEFT:
            str = "A_TRACK_MODE_DOUBLE_LEFT";
            break;
        case A_TRACK_MODE_DOUBLE_RIGHT:
            str = "A_TRACK_MODE_DOUBLE_RIGHT";
            break;
        case A_TRACK_MODE_EXCHANGE:
            str = "A_TRACK_MODE_EXCHANGE";
            break;
        case A_TRACK_MODE_ONLY_RIGHT:
            str = "A_TRACK_MODE_ONLY_RIGHT";
            break;
        case A_TRACK_MODE_ONLY_LEFT:
            str = "A_TRACK_MODE_ONLY_LEFT";
            break;
        case A_TRACK_MODE_MUTED:
            str = "A_TRACK_MODE_MUTED";
            break;
        default:
            str = "unknown";
            break;
    }
    return str;
}

static A_TRACK_CHANNEL_MODE_E audio_channel_mode_from_str(const char * str)
{
    A_TRACK_CHANNEL_MODE_E mode = A_TRACK_MODE_STEREO;
    ICS_LOGI("set audio channel mode: [%s]", str);
    if (!strcmp(str,"stereo"))
    {
        return A_TRACK_MODE_STEREO;
    }
    if (!strcmp(str,"mono"))
    {
        return A_TRACK_MODE_DOUBLE_MONO;
    }
    if (!strcmp(str,"left"))
    {
        return A_TRACK_MODE_DOUBLE_LEFT;
    }
    if (!strcmp(str,"right"))
    {
        return A_TRACK_MODE_DOUBLE_RIGHT;
    }
    if (!strcmp(str,"exchange"))
    {
        return A_TRACK_MODE_EXCHANGE;
    }
    if (!strcmp(str,"left_only"))
    {
        return A_TRACK_MODE_ONLY_LEFT;
    }
    if (!strcmp(str,"right_only"))
    {
        return A_TRACK_MODE_ONLY_RIGHT;
    }
    if (!strcmp(str,"mute"))
    {
        return A_TRACK_MODE_MUTED;
    }


    return mode;
}

static V_VOID SetVideoDisplayMode(V_CHAR* pszCmd)
{
    V_UINT32 u32LenOfCmd = strlen(pszCmd);
    V_UINT32 u32DisplayMode = 0;
    V_UINT32 u32UIWidth = 0;
    V_UINT32 u32UIHeight = 0;
    V_FLOAT fUIWidth = 0;
    V_FLOAT fUIHeight = 0;
    V_FLOAT fStdUIRadio1 = (V_FLOAT)16 / (V_FLOAT)9;
    V_FLOAT fStdUIRadio2 = (V_FLOAT)4 / (V_FLOAT)3;
    V_FLOAT fStdUIRadio3 = (V_FLOAT)5 / (V_FLOAT)4;
    V_FLOAT fStdUIRadio4 = (V_FLOAT)3 / (V_FLOAT)2;
    V_UINT32 u32Len = 0;

    V_CHAR *res = NULL;
    res = strtok(pszCmd, " ");
    if(res != NULL)
        res = strtok(NULL, " ");
    if(res == NULL)
    {
        ICS_LOGW("the format of sm is wrong:no display mode");
        return;
    }
    u32DisplayMode = atoi(res);

    if(u32DisplayMode == 0)
    {
        g_spobjPlayer->SetDisplayMode((E_SME_DVBPLAYER_DISPLAY_MODE)u32DisplayMode);
    }
    else if(u32DisplayMode == 1)
    {
        res = strtok(NULL, " ");
        if(res != NULL)
        {
            u32UIWidth = atoi(res);
            res = strtok(NULL, " ");
            if(res != NULL)
            {
                u32UIHeight = atoi(res);
            }
            else
            {
                ICS_LOGW("the format of sm is wrong:no height");
                return;
            }
        }
        else
        {
            ICS_LOGW("the format of sm is wrong:no width");
            return;
        }

        ICS_LOGI("displaymode : %d,  W & H is %d & %d", u32DisplayMode, u32UIWidth, u32UIHeight);
        ST_DispRatio stDispRatio;
        stDispRatio.u32DispRatioW = u32UIWidth;
        stDispRatio.u32DispRatioH = u32UIHeight;
        g_spobjPlayer->SetDisplayMode((E_SME_DVBPLAYER_DISPLAY_MODE)u32DisplayMode);
        g_spobjPlayer->SetDisplayRatio(&stDispRatio);

    }
    else
    {
        g_spobjPlayer->SetDisplayMode((E_SME_DVBPLAYER_DISPLAY_MODE)u32DisplayMode);
    }
    return;
}

static V_VOID SetVideoDisplayRect(V_CHAR* pszCmd)
{
    V_INT32 i32StartX = 0;
    V_INT32 i32StartY = 0;
    V_INT32 i32Width = 0;
    V_INT32 i32Height = 0;
    ST_SME_DVBDISP_RECT st_dvbdisp_rect;

    V_CHAR *res = NULL;
    res = strtok(pszCmd, " ");
    if(res != NULL)
        res = strtok(NULL, " ");
    if(res == NULL)
    {
        ICS_LOGW("the format of s5 is wrong:no startX");
        return;
    }
    i32StartX = atoi(res);

    res = strtok(NULL, " ");
    if(res != NULL)
        i32StartY = atoi(res);
    else
    {
        ICS_LOGW("the format of s5 is wrong:no startY");
        return;
    }

    res = strtok(NULL, " ");
    if(res != NULL)
        i32Width = atoi(res);
    else
    {
        ICS_LOGW("the format of s5 is wrong:no width");
        return;
    }

    res = strtok(NULL, " ");
    if(res != NULL)
        i32Height = atoi(res);
    else
    {
        ICS_LOGW("the format of s5 is wrong:no height");
        return;
    }
    st_dvbdisp_rect.i32StartX = i32StartX;
    st_dvbdisp_rect.i32StartY = i32StartY;
    st_dvbdisp_rect.i32Width = i32Width;
    st_dvbdisp_rect.i32Height = i32Height;

    ICS_LOGI("UI Set Windows is [%d, %d, %d, %d] ", i32StartX, i32StartY, i32Width, i32Height);
    g_spobjPlayer->SetVDispRect(st_dvbdisp_rect);
    return;
}

static V_VOID GetVideoDisplayMode()
{
    V_UINT32 u32Error;
    E_SME_DVBPLAYER_DISPLAY_MODE emode = E_SME_DVBPLAYER_DIS_MODE_FULL;
    E_SME_DVBPLAYER_DISPLAY_MODE& rmode = emode;
    ST_DispRatio st_disp_ratio;
    VOS_Memset(&st_disp_ratio,
               0,
               sizeof(ST_DispRatio));
    ST_DispRatio& rst_disp_ratio = st_disp_ratio;

    u32Error = g_spobjPlayer->GetDisplayMode(rmode);
    if(u32Error == ICS_FAIL)
    {
        ICS_LOGE("get display mode  failed, error:%u", u32Error);
        return;
    }
    STD_OUT("video display mode : %u\n", (V_UINT32)emode);

    u32Error = g_spobjPlayer->GetDisplayRatio(rst_disp_ratio);
    if(u32Error == ICS_FAIL)
    {
        ICS_LOGE("get display ratio  failed, error:%u", u32Error);
        return;
    }
    STD_OUT("video display ratio:[%u %u]\n",
            rst_disp_ratio.u32DispRatioW,
            rst_disp_ratio.u32DispRatioH);
    return;
}

static V_VOID GetVideoDisplayRect()
{
    V_UINT32 u32Error;
    ST_SME_DVBDISP_RECT st_dvb_disp_rect;
    VOS_Memset(&st_dvb_disp_rect,
               0,
               sizeof(ST_SME_DVBDISP_RECT));
    ST_SME_DVBDISP_RECT& rst_dvb_disp_rect = st_dvb_disp_rect;

    u32Error = g_spobjPlayer->GetDisplayRect(rst_dvb_disp_rect);
    if(u32Error == ICS_FAIL)
    {
        ICS_LOGE("get display rect  failed, error:%u", u32Error);
        return;
    }
    STD_OUT("video display rect :[%u %u %u %u] \n",
        rst_dvb_disp_rect.i32StartX,
        rst_dvb_disp_rect.i32StartY,
        rst_dvb_disp_rect.i32Width,
        rst_dvb_disp_rect.i32Height);
    return;
}

static V_UINT32 GetAudioInfoFromUri(const char* uri ,
    ST_DVBProgAudioInfo* pAudioInfo, V_BOOL bHavePrefix /*= ICS_FALSE*/)
{
    int length = 0;
    const char *str = uri;
    unsigned int char_index = 0;
    unsigned int id_index = 0;
    int a_type = 0;
    char id_str[8][DVB_ID_CHAR_NUM_MAX + 1] = {{0}};

    if (!pAudioInfo || !uri) {
        STD_OUT("uri is null,uri:%p, pAudioinfo:%p",uri, pAudioInfo);
        return ICS_FAIL;
    }

    if(bHavePrefix)
    {
        if (!strstr(uri, DVB_ATRACK_TAG))
        {
            ICS_LOGE("atrack is invalid!");
            return ICS_FAIL;
        }

        length = strlen(uri);

        if (length <= strlen(DVB_ATRACK_TAG))
        {
            ICS_LOGE("atrack is invalid, too short!");
            return ICS_FAIL;
        }

        str = (char *)(uri + strlen(DVB_ATRACK_TAG));
    }
    else
    {
        length = strlen(uri);
    }

    for (int i = 0; i < length; i++)
    {
        if (isxdigit(str[i]))
        {
            id_str[id_index][char_index] = str[i];
            char_index ++;
        }
        else if ('.' == str[i])
        {
            if (!char_index)
            {
                ICS_LOGE("You should have numbers before '.'");
                return ICS_FAIL;
            }

            char_index = 0;
            id_index ++;

            if (id_index > DVB_ID_TYPE_NUM_MAX - 1)
            {
                ICS_LOGE("We can only process 4 ids");
                break;
            }
        }
        else
        {
            ICS_LOGE("unkown char str[%]:%c", i, str[i]);
            break;
        }
    }

    if (id_index < 2)
    {
        STD_OUT("We need at least 3 ids");
        return ICS_FAIL;
    }


    STD_OUT("id_str[0]=%s, id_str[1]=%s, id_str[2]=%s",
        id_str[0], id_str[1], id_str[2]);

    pAudioInfo->apid = (int)strtol(id_str[0], NULL, 16);
    a_type = (int)strtol(id_str[1], NULL, 16);
    if (a_type >= E_DVB_AUD_CODEC_TYPE_UNKNOWN && a_type < E_DVB_AUD_CODEC_TYPE_MAX)
    {
        pAudioInfo->e_atype  = (E_DVB_AUD_CODEC_ID)a_type;
    }
    else
    {
        STD_OUT("audio type error");
        return ICS_FAIL;
    }

    pAudioInfo->is_ca = (int)strtol(id_str[2], NULL, 16);

    return ICS_SUCCESS;
}


static V_VOID SetAudioInfo()
{

    ST_DVBProgAudioInfo st_audioInfo;
    V_CHAR acInputArg[256] = {0,};
    V_UINT32 ret;

    VOS_Memset(&st_audioInfo, 0, sizeof(ST_DVBProgAudioInfo));
    VOS_Memset(acInputArg, 0, sizeof(acInputArg));

    STD_OUT("Audio info:\n");
    STD_OUT("Input format is: apid.atype.isca (fd3.1.0)\n");

    STD_OUT("============audio codec=========\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP1:  1\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP2:  2\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP3:  3\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_LPCM: 4\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_AC3:  5\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_EAC3: 6\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_DTS:  7\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_DRA:  8\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_AAC:  9\n");
    STD_OUT("================================\n\n");

    STD_OUT("pleas input audio info:");

    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);

    STD_OUT("acInputArg:%s\n",acInputArg);

    ret = GetAudioInfoFromUri(acInputArg, &st_audioInfo);
    if (ret == ICS_FAIL)
        return;

    STD_OUT("Input apid:0x%x, atype:%d, isca:%d\n",
        st_audioInfo.apid,st_audioInfo.e_atype,st_audioInfo.is_ca);

    if (g_spobjPlayer)
        g_spobjPlayer->SetProgAudioInfo(st_audioInfo);

}

static V_VOID SwtichChannel()
{
    V_CHAR acInputArg[256] = {0,};
    VOS_Memset(acInputArg, 0, sizeof(acInputArg));
    V_UINT32 ret;

    STD_OUT("pleas input uri:");

    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    STD_OUT("acInputArg:%s\n",acInputArg);


    ret = PlayChannel(acInputArg);
    if (ret == ICS_FAIL)
        STD_OUT("play failed");


}


static V_UINT32 TestDvbPlayerOp(V_CHAR* pszCmd)
{
    V_UINT32 u32Err = ICS_FAIL;

    switch(pszCmd[0])
    {
        case 'f':
        {
            u32Err = ForwardChannel();
            if (u32Err == ICS_FAIL)
                STD_OUT("UpChannel failed (%d)\n",u32Err);
            else
                STD_OUT("UpChannel ok (%d)\n",u32Err);
            break;
        }
        case 'b':
        {
            u32Err = BackwardChannel();
            if (u32Err == ICS_FAIL)
                STD_OUT("BackwardChannel failed (%d)\n",u32Err);
            else
                STD_OUT("BackwardChannel ok (%d)\n",u32Err);
            break;
        }
#ifdef __LINUX__
        case 'a':
#if 0
            if ('1' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_UINT32 mute = atoi(&pszCmd[3]);

                mute = (0 == mute) ? 0 : 1;

                ICS_LOGI("set audio mute:%d", mute);
                u32Err = g_spobjPlayer->SetAudioMute(mute);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set mute %u (%d), failed", mute, u32Err);
                }
                break;
            }
            if ('2' == pszCmd[1])
            {
                V_UINT32 mute = 0;

                u32Err = g_spobjPlayer->GetAudioMute(&mute);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("get mute %u (%d), failed", mute, u32Err);
                }
                STD_OUT("get audio mute: %d/%s\n", mute,mute?"mute":"unmute");
                ICS_LOGI("get audio mute: %d/%s", mute,mute?"mute":"unmute");

                break;
            }
            if ('3' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_UINT32 volume = atoi(&pszCmd[3]);

                if (volume < 0)
                {
                    volume = 0;
                }
                if (volume > 100)
                {
                    volume = 100;
                }

                ICS_LOGI("set audio volume:%d", volume);
                u32Err = g_spobjPlayer->SetAudioVolume(volume);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set volume %u (%d), failed", volume, u32Err);
                }

                break;
            }
            if ('4' == pszCmd[1])
            {
                V_UINT32 volume = 0;

                u32Err = g_spobjPlayer->GetAudioVolume(&volume);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("get volume %u (%d), failed", volume, u32Err);
                }
                STD_OUT("get audio volume: %d\n", volume);
                ICS_LOGI("get audio volume: %d", volume);

                break;
            }
#endif
            if ('5' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                A_TRACK_CHANNEL_MODE_E mode = audio_channel_mode_from_str(strtok(pszCmd+3, " "));

                ICS_LOGI("set audio channel mode:%s", audio_channel_mode_to_str(mode));
                u32Err = g_spobjPlayer->SetAudioChannelMode(mode);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set audio channel mode %s (%d), failed", audio_channel_mode_to_str(mode), u32Err);
                }

                break;
            }
            if ('6' == pszCmd[1])
            {
                A_TRACK_CHANNEL_MODE_E mode;

                u32Err = g_spobjPlayer->GetAudioChannelMode(&mode);
                if(E_SME_DVBPLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("get audio channel mode %s (%d), failed", audio_channel_mode_to_str(mode), u32Err);
                }
                STD_OUT("get audio channel mode: %s\n", audio_channel_mode_to_str(mode));
                ICS_LOGI("get audio channel mode: %s", audio_channel_mode_to_str(mode));
                break;
            }
            break;
#endif
        case 's':
        {
            switch(pszCmd[1])
            {
                case 'a':
                {
                    SetAudioInfo();
                    break;
                }
                case 'c':
                {
                    SwtichChannel();
                    break;
                }
                case 'm':
                {
                    SetVideoDisplayMode(pszCmd);
                    break;
                }
                case 'r':
                {
                    SetVideoDisplayRect(pszCmd);
                    break;
                }
                case 'p':
                {
                    u32Err = g_spobjPlayer->Stop();
                    if(ICS_SUCCESS != u32Err)
                    {
                        ICS_LOGE("StopChannel failed(%u)", u32Err);
                        STD_OUT("StopChannel failed(%u)\n", u32Err);
                    }
                    break;
                }
                case 't':
                {
                    u32Err = g_spobjPlayer->Start();
                    if(ICS_SUCCESS != u32Err)
                    {
                        ICS_LOGE("set start failed(%u)", u32Err);
                        STD_OUT("start failed(%u)\n", u32Err);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case 'g':
        {
            switch(pszCmd[1])
            {
                case 'm':
                {
                    GetVideoDisplayMode();
                    break;
                }
                case 'r':
                {
                    GetVideoDisplayRect();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            ICS_LOGW("Unkown %c XX", pszCmd[0]);
            break;
    }

    return ICS_TRUE;
}

static V_UINT32 TestRunner_MemLeak_SwitchCC(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunner_MemLeak_SwitchCC in");
    STD_OUT("TestRunner_MemLeak_SwitchCC in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

    g_spobjPlayerObs = new GSmeDvbPlayerObs();
    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(ICS_SUCCESS != u32Err)
    {
        STD_OUT("set SetObs failed(%u)", u32Err);
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    u32Err = SetDataSource(g_spszUri);
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("SetDataSource failed(%u)", u32Err);
        STD_OUT("SetDataSource failed(%u)\n", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->Prepare();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set Prepare failed(%u)", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->Start();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
         i32Count++)
    {
        //delay 2s
        u32Err = ForwardChannel();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("ForwardChannel failed(%u)", u32Err);
            STD_OUT("ForwardChannel failed(%u)", u32Err);STD_OUT("\n");
            goto EXIT;
        }
        VOS_MSleep(g_u32SleepInterval);

        ICS_LOGW("========switch count:%d",i32Count);
        STD_OUT("========switch count:%d",i32Count); STD_OUT("\n");
        save_mem_info();

    }


EXIT:

    STD_OUT("reset begin"); STD_OUT("\n");
    u32Err = g_spobjPlayer->Reset();
    STD_OUT("DestroySmeDvbPlayer begin"); STD_OUT("\n");
    ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    delete g_spobjPlayerObs;
    g_spobjPlayerObs = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);

    STD_OUT("TestRunner_MemLeak_SwitchCC out!!!\n");
    ICS_LOGI("TestRunner_MemLeak_SwitchCC out!!!");

    return 0;
}

static V_UINT32 TestRunner_MemLeak_SwitchCC_Destroy(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;

    ICS_LOGI("TestRunner_MemLeak_SwitchCC_Destroy in");
    STD_OUT("TestRunner_MemLeak_SwitchCC_Destroy in!\n");
    g_sbOnErr = ICS_FALSE;

    for(i32Count = 0; i32Count < g_u32TestCount && !g_sbQuitPlay; i32Count++)
    {
        g_spobjPlayer = ISmeDvbPlayer::CreateSmeDvbPlayer();

        g_spobjPlayerObs = new GSmeDvbPlayerObs();
        STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(ICS_SUCCESS != u32Err)
        {
            STD_OUT("set SetObs failed(%u)", u32Err);
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        u32Err = SetDataSource(g_spszUri);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->Prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->Start();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
            goto EXIT;
        }
        VOS_MSleep(g_u32SleepInterval);
        for(V_UINT32 idx = 1; idx < g_su32ValidChannel && !g_sbQuitPlay; idx++)
        {
            V_UINT32 u32Error = PlayChannel((const char *)g_sacDvbChannelList[idx]);
            if(ICS_SUCCESS != u32Error)
            {
                ICS_LOGE("PlayChannel failed(%u)", u32Error);
                STD_OUT("PlayChannel failed(%u)", u32Error);STD_OUT("\n");
                goto EXIT;
            }
            VOS_MSleep(g_u32SleepInterval);
        }
        STD_OUT("Stop begin"); STD_OUT("\n");
        u32Err = g_spobjPlayer->Stop();
        STD_OUT("reset begin"); STD_OUT("\n");
        u32Err = g_spobjPlayer->Reset();
        STD_OUT("DestroySmeDvbPlayer begin"); STD_OUT("\n");
        ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
        g_spobjPlayer = NULL;
        delete g_spobjPlayerObs;
        g_spobjPlayerObs = NULL;

        ICS_LOGW("========cc-destoy count:%d",i32Count);
        STD_OUT("========cc-destoy:%d",i32Count); STD_OUT("\n");

        save_mem_info();
        VOS_MSleep(g_u32SleepInterval);
    }

EXIT:
    if(NULL != g_spobjPlayer)
    {
        STD_OUT("reset begin"); STD_OUT("\n");
        u32Err = g_spobjPlayer->Reset();
        STD_OUT("DestroySmeDvbPlayer begin"); STD_OUT("\n");
        ISmeDvbPlayer::DestroySmeDvbPlayer(g_spobjPlayer);
        g_spobjPlayer = NULL;
    }

    if(g_spobjPlayerObs)
    {
        delete g_spobjPlayerObs;
        g_spobjPlayerObs = NULL;
    }

    STD_OUT("TestRunner_MemLeak_SwitchCC_Destroy out!!!\n");
    ICS_LOGI("TestRunner_MemLeak_SwitchCC_Destroy out!!!");

    return 0;
}

V_VOID StartDvbPlayerOfNormal_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerNormal, NULL);
    UTILS_MLOGE_RET_IF(NULL == g_shdlRunner, ("Create Thd failed"));
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q'  && acPlayOpCmd[0] != 'Q')
    {
        VOS_ThdMutexLock(&g_mutex);
        PrintPlayerOpHelp();
        memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
        fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);

        /* replace the last \n to \0 */
        acPlayOpCmd[strlen(acPlayOpCmd) - 1] = 0;

        VOS_ThdMutexUnLock(&g_mutex);
        if(NULL != g_spobjPlayer)
        {
            V_UINT32 u32Err = TestDvbPlayerOp(acPlayOpCmd);
            if(ICS_TRUE == u32Err)
            {
                continue;
            }
        }
        ICS_LOGW("player is destroyed!");
        break;
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    return;
}


V_VOID StartDvbPlayerOfMemLeak_SwitchCC()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_MemLeak_SwitchCC, NULL);

    do
    {
        STD_OUT("input:q to quit test\n");
        memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
        fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q');

    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;

    return;
}

static V_VOID StartDvbPlayerOfMemLeak_SwitchCC_Destroy()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    g_sacDvbChannelList[0][0] = 0;
    strncpy(g_sacDvbChannelList[0], g_spszUri, MAX_URI_LEN - 1);
    if(strlen(g_spszUri) >= MAX_URI_LEN - 1)
    {
        g_sacDvbChannelList[0][MAX_URI_LEN - 1] = 0;
    }
    else
    {
        g_sacDvbChannelList[0][strlen(g_spszUri)] = 0;
    }
    g_su32ValidChannel = 1;
    for(V_UINT32 idx = g_su32ValidChannel;
        idx < sizeof(g_sacDvbChannelList)/sizeof(g_sacDvbChannelList[0]); idx++)
    {
        g_sacDvbChannelList[idx][0] = 0;
        STD_OUT("pls input uri[%d] to CC,input 'q' to end input:uri example:dvb://1.1.1", idx);STD_OUT("\n");
        fgets(g_sacDvbChannelList[idx], (sizeof(g_sacDvbChannelList[idx]) - 1), stdin);
        if(!strncmp(g_sacDvbChannelList[idx], "q", 1))
        {
            STD_OUT("end of input uri !");STD_OUT("\n");
            break;
        }

        if(strncmp(g_sacDvbChannelList[idx], "dvb://", strlen("dvb://")))
        {
            STD_OUT("wrong uri:%s,please again.", g_sacDvbChannelList[idx]);STD_OUT("\n");
            idx--;
            continue;
        }
        g_su32ValidChannel++;
        STD_OUT("cc uri:%s", g_sacDvbChannelList[idx]);STD_OUT("\n");
    }

    STD_OUT("channel list num:%d,uri below:", g_su32ValidChannel);STD_OUT("\n");
    for(V_UINT32 idx = 0; idx < g_su32ValidChannel; idx++)
    {
        STD_OUT("uri[%03d]:%s", idx, g_sacDvbChannelList[idx]);STD_OUT("\n");
    }

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_MemLeak_SwitchCC_Destroy, NULL);

    do
    {
        STD_OUT("input:q to quit test\n");
        memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
        fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q');

    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;

    return;
}


V_VOID
    StartDvbPlayerOfStartReset_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    g_scInputType = '2';

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    if(g_u32TestCount <=0 || g_u32SleepInterval <= 0)
    {
       STD_OUT("input arg invalid!\n");
       return ;
    }

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerStartReset, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q' && !g_sbQuitPlay)
    {
       STD_OUT("input:q to quit test thread\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q'  && acPlayOpCmd[0] != 'Q')
    {
       STD_OUT("input:q to quit test\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }

    return;
}

V_VOID
    StartDvbPlayerOfStartExit_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    g_scInputType = '1';

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    if(g_u32TestCount <=0 || g_u32SleepInterval <= 0)
    {
       STD_OUT("input arg invalid!\n");
       return ;
    }

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerStartExit, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q' && !g_sbQuitPlay)
    {
       STD_OUT("input:q to quit test thread\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));

    return;
}

V_VOID
StartDvbPlayerOfSwitchAudioTrack_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    g_scInputType = '1';

    STD_OUT("Input format is: atrack://apid.atype.isca (atrack://fd3.1.0)\n");

    STD_OUT("============audio codec=========\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP1:  1\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP2:  2\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_MP3:  3\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_LPCM: 4\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_AC3:  5\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_EAC3: 6\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_DTS:  7\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_DRA:  8\n");
    STD_OUT("E_DVB_AUD_CODEC_TYPE_AAC:  9\n");
    STD_OUT("================================\n\n");


    g_su32ValidAudioTrack = 0;
    for(V_UINT32 idx = g_su32ValidAudioTrack;
        idx < sizeof(g_sacDvbAudioTrackList)/sizeof(g_sacDvbAudioTrackList[0]); idx++)
    {
        g_sacDvbAudioTrackList[idx][0] = 0;
        STD_OUT("pls input atrack[%d] to sa,input 'q' to end input:example:atrack://fd3.1.0", idx);STD_OUT("\n");
        fgets(g_sacDvbAudioTrackList[idx], (sizeof(g_sacDvbAudioTrackList[idx]) - 1), stdin);
        if(!strncmp(g_sacDvbAudioTrackList[idx], "q", 1))
        {
            STD_OUT("end of input atrack !");STD_OUT("\n");
            break;
        }

        if(strncmp(g_sacDvbAudioTrackList[idx], DVB_ATRACK_TAG, strlen(DVB_ATRACK_TAG)))
        {
            STD_OUT("wrong atrack:%s,please again.", g_sacDvbAudioTrackList[idx]);STD_OUT("\n");
            idx--;
            continue;
        }
        g_su32ValidAudioTrack++;
        STD_OUT("sa uri:%s", g_sacDvbAudioTrackList[idx]);STD_OUT("\n");
    }

    STD_OUT("audiotrack list num:%d,uri below:", g_su32ValidAudioTrack);STD_OUT("\n");
    for(V_UINT32 idx = 0; idx < g_su32ValidAudioTrack; idx++)
    {
        STD_OUT("atrack[%03d]:%s", idx, g_sacDvbAudioTrackList[idx]);STD_OUT("\n");
    }

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    if(g_u32TestCount <=0 || g_u32SleepInterval <= 0)
    {
       STD_OUT("input arg invalid!\n");
       return ;
    }

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerSwitchAuidoTrack, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q' && !g_sbQuitPlay)
    {
       STD_OUT("input:q to quit test thread\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));

    return;
}

V_VOID
    StartDvbPlayerOfStartStopResetDestroy_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    //g_scInputType = '1';

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    if(g_u32TestCount <=0 || g_u32SleepInterval <= 0)
    {
       STD_OUT("input arg invalid!\n");
       return ;
    }

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerStartStopResetDestroy, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q' && !g_sbQuitPlay)
    {
       STD_OUT("input:q to quit test thread\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));

    return;
}

V_VOID
    StartDvbPlayerOfStartStopReset_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    //g_scInputType = '1';

    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("pls input sleepms:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32SleepInterval = atoi(acInputArg);
    STD_OUT("%d\n", g_u32SleepInterval);

    if(g_u32TestCount <=0 || g_u32SleepInterval <= 0)
    {
       STD_OUT("input arg invalid!\n");
       return ;
    }

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunnerStartStopReset, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q' && !g_sbQuitPlay)
    {
       STD_OUT("input:q to quit test thread\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));

    return;
}



static V_VOID PrintHelpOfArg()
{
    STD_OUT("Options are below:\n");
    STD_OUT("--uri [val];uri to play;Mandatory;\n"
            "           egg:--uri dvb://888.8.1\n");

    STD_OUT("--select-play;need select to play;Optional;\n"
        "           egg:--select-play;default:not\n");
    return;
}

V_INT32 main(V_INT32 i32Argc, V_CHAR* ppszArgv[])
{
    V_BOOL bExit = ICS_FALSE;
    V_CHAR acInputType[128] = {0,};
    V_BOOL bSelectToPlay = ICS_FALSE;
    V_INT32 i32OptRes = 0;
    static struct option stLongOptions[] = {
        {"uri", required_argument, NULL, 'u'},
        {"log-level", required_argument, NULL, 'l'},
        {"vcodec-type", required_argument, NULL, 'v'},
        {"acodec-type", required_argument, NULL, 'a'},
        {"select-play", no_argument, NULL, 's'},
        {0, 0, 0, 0}
    };
    V_CHAR* pszDispMode = NULL;
    g_spszUri = NULL;
    if(i32Argc < 2)
    {
        PrintHelpOfArg();
        return -1;
    }

    if (ICS_SUCCESS !=SmeInit(E_SME_PLAYER_LOG_LEVEL_INFO))
    {
        STD_OUT("sme init failed");
        return -1;
    }

    while ( (i32OptRes = getopt_long(i32Argc, ppszArgv, "u", stLongOptions, NULL)) != -1)
    {
        STD_OUT("i32OptRes = %c\n", i32OptRes);
        STD_OUT("optarg = %s\n", optarg);
        STD_OUT("optind = %d\n", optind);
        STD_OUT("argv[optind - 1] = %s\n",  ppszArgv[optind - 1]);
        switch(i32OptRes)
        {
            case 'u':
                if(NULL != optarg)
                {
                    if(g_spszUri)
                    {
                        free(g_spszUri);
                        g_spszUri = NULL;
                    }
                    g_spszUri = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no url?\n", ppszArgv[optind - 1]);
                }
                break;
            case 'l':
                if(NULL != optarg)
                {
                    g_scLogLevel = optarg[0];
                }
                else
                {
                    STD_OUT("%s:val is NULL?no log-level? use default\n",
                        ppszArgv[optind - 1]);
                }
                break;
            case 's':
                bSelectToPlay = ICS_TRUE;
                break;
            case 'v':
               if(NULL != optarg)
                {
                    g_seDTVVidCodec = inputVcodec2DTVVidCodec((const V_CHAR*)optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no vtype?\n", ppszArgv[optind - 1]);
                }
                break;
            case 'a':
               if(NULL != optarg)
                {
                    g_seDTVAudCodec = inputVcodec2DTVAudCodec((const V_CHAR*)optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no atype?\n", ppszArgv[optind - 1]);
                }
                break;
             case '?'://no break;
            default:
                STD_OUT("%s:not recognise!!!\n", ppszArgv[optind - 1]);
                break;
        }
    }

    if(!g_spszUri)
    {
        STD_OUT("you must input uri,arg:--uri=xxx\n");
        PrintHelpOfArg();
        return -1;
    }

    STD_OUT("uri=%s\n", g_spszUri);
    ICS_LOGI("uri=%s", g_spszUri);

    system("stop ir_user");
    while(ICS_FALSE == bExit)
    {
        if(bSelectToPlay)
        {
            PrintHelpOfTestCase();
            memset(acInputType, 0, sizeof(acInputType));
            fgets((char *)(acInputType), (sizeof(acInputType) - 1), stdin);
        }
        else
        {
            acInputType[0] = '1';
        }

        switch(acInputType[0])
        {
            case '1':
            case '2':
            {
                g_scInputType = acInputType[0];
                StartDvbPlayerOfNormal_Test();
                if(!bSelectToPlay)
                {
                    STD_OUT("play completed,uri=%s\n", g_spszUri);
                    goto EXIT;
                }
                break;
            }
            case '3':
            {
                StartDvbPlayerOfMemLeak_SwitchCC();
                break;
            }
            case '4':
            {
                StartDvbPlayerOfStartReset_Test();
                break;
            }
            case '5':
            {
                StartDvbPlayerOfStartExit_Test();
                break;
            }
            case '6':
            {
                StartDvbPlayerOfSwitchAudioTrack_Test();
                break;
            }
            case '7':
            {
                StartDvbPlayerOfMemLeak_SwitchCC_Destroy();
                break;
            }
	    case '8':
            {
                StartDvbPlayerOfStartStopResetDestroy_Test();
                break;
            }
            case '9':
            {
                StartDvbPlayerOfStartStopReset_Test();
                break;
            }
            case 'q':
            case 'Q':
                goto EXIT;
                //break;
            default:
                PrintHelpOfTestCase();
                break;
        }
    }

EXIT:
    if(g_spszUri)
    {
        free(g_spszUri);
        g_spszUri = NULL;

    }
    if(pszDispMode)
    {
       free(pszDispMode);
       pszDispMode = NULL;
    }
    SmeDeInit();
    system("start ir_user");
    return 0;
}


