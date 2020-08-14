/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    test_sme_player.cpp
 * @brief   ≤‚ ‘ smeplayer
 * @author
 * @date    2014/4/9
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/4/9
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "osal_thread.h"
#include "osal_type.h"
#include "sme_macro.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "sme_timer.h"
#include "sme_init.h"

#include <unistd.h>
#include <time.h>

#ifdef __USE_ANDROID_SURFACE__
#include "native_surface_inf.h"
#endif

#ifdef __ANDROID__
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#endif

#include "sme_media_type.h"
#include "sme_player_inf.h"

#if defined (__LINUX__)
#include "test_hdmi_init.h"
#include "test_subtile_disp.h"
#endif

#ifdef __ANDROID__
using namespace android;
#endif


#define STD_OUT printf
#define TEST_URI "/data/test.mp3"
//#define TEST_URI "/data/test.mp4"

#define DISPLAY_STARTX 0
#define DISPLAY_STARTY 0

#define DISPLAY_WIDTH  (1920)
#define DISPLAY_HEIGHT (1080)

#define SUB_DISPLAY_STARTX 280
#define SUB_DISPLAY_STARTY 560
#define SUB_DISPLAY_WIDTH 720
#define SUB_DISPLAY_HEIGHT 128
#define INTERVAL_CHARICTER '-'

#define DURATION_POSTION_WIDTH 128
#define DURATION_POSTION_HEIGHT 80
#define TIME_LEN 64
#define RESOLUTION_LEN 100

#define BANDWIDTH_POSTION_WIDTH 320
#define BANDWIDTH_POSTION_HEIGHT 60


static V_BOOL g_sbOnPrepared = ICS_FALSE;
static V_BOOL g_sbOnPlaybackCompleted = ICS_FALSE;
static V_BOOL g_sbOnErr = ICS_FALSE;

static VOSTHDHDL g_shdlRunner = NULL;
static V_CHAR g_scInputType = 0;
static V_BOOL g_sbQuitPlay = ICS_FALSE;

#if defined (__LINUX__)
static SUB_DISP_HDL g_shdlSubDisp = NULL;
static SUB_DISP_HDL g_shdlDurationDisp = NULL;
static SUB_DISP_HDL g_shdlCurPosDisp = NULL;
static SUB_DISP_HDL g_shdlCurBwDisp = NULL;
static SUB_DISP_HDL g_shdlCurVBtDisp = NULL;
static SUB_DISP_HDL g_shdlCurABtDisp = NULL;
static SUB_DISP_HDL g_shdlResolutionDisp = NULL;
#endif


static V_CHAR* g_spszUri = NULL;
static V_CHAR* g_spszSubUri = NULL;

static ST_SME_DISP_RECT g_sstSubDispRect={SUB_DISPLAY_STARTX, SUB_DISPLAY_STARTY,
    SUB_DISPLAY_WIDTH, SUB_DISPLAY_HEIGHT};
static ST_SME_DISP_RECT g_sstDurationDispRect={DISPLAY_WIDTH-DURATION_POSTION_WIDTH,
    DISPLAY_HEIGHT - DURATION_POSTION_HEIGHT, DURATION_POSTION_WIDTH, DURATION_POSTION_HEIGHT};
static ST_SME_DISP_RECT g_sstCurPosDispRect={64,
    DISPLAY_HEIGHT - DURATION_POSTION_HEIGHT, DURATION_POSTION_WIDTH, DURATION_POSTION_HEIGHT};

static ST_SME_DISP_RECT g_sstBandwidthDispRect={64,64, BANDWIDTH_POSTION_WIDTH, BANDWIDTH_POSTION_HEIGHT};
static ST_SME_DISP_RECT g_sstVideoBtDispRect={64,128, BANDWIDTH_POSTION_WIDTH, BANDWIDTH_POSTION_HEIGHT};
static ST_SME_DISP_RECT g_sstAudioBtDispRect={64,192, BANDWIDTH_POSTION_WIDTH, BANDWIDTH_POSTION_HEIGHT};
static ST_SME_DISP_RECT g_sstResolutionChangedDispRect = {64,DISPLAY_HEIGHT - DURATION_POSTION_HEIGHT*2,
    DURATION_POSTION_WIDTH*2, DURATION_POSTION_HEIGHT};



static V_VOID* g_pvSurfaceHdl = NULL;

static V_UINT32 g_u32TestCount = 1;
static V_UINT32 g_u32SleepInterval = 5;//ms
static V_CHAR   g_scLogLevel = 'W';//
static V_UINT64 g_su64StartCost = 0;//ms,player start time cost
static V_UINT64 g_su64ResetCost = 0;//ms,player start time cost
static ST_VOS_THD_MUTEX g_sstMutex = VOS_PTHREAD_MUTEX_INITIALIZER;
static V_BOOL g_bEnableSetBitrate = ICS_FALSE;
static V_BOOL g_bStreamInfoDisplaySwtich = ICS_FALSE;
static V_BOOL g_bShowFragmentInfo = ICS_FALSE;
static V_BOOL g_bEnableSetCookies = ICS_FALSE;
static V_CHAR *g_bSetUserAgent = NULL;
static V_BOOL g_bSetByteRange = ICS_TRUE;
static V_CHAR * g_spsSetCookies[256] = {0};
static V_CHAR *g_bSetReferer = NULL;
static V_UINT32 g_u32BufferTime = 5000;//5s
static V_UINT32 g_u32BufferLowPercent = 1;
static V_UINT32 g_u32BufferHighPercent = 4;

static V_CHAR *g_spszAudioHdmiMode = NULL;
static V_CHAR *g_spszGetAudioHdmiMode = NULL;

static V_UINT32 g_a_mute = 0;
static V_UINT32 g_a_volume = 0;
static A_TRACK_CHANNEL_MODE_E g_a_channel_mode = A_TRACK_MODE_STEREO;

static ST_VOS_THD_MUTEX     g_mutex = VOS_PTHREAD_MUTEX_INITIALIZER;
#define COMMAND_LOCK  VOS_ThdMutexLock(&g_mutex);
#define COMMAND_UNLOCK VOS_ThdMutexUnLock (&g_mutex);

class SmePlayerObsTest;
static SmePlayerObsTest* g_spobjPlayerObs = NULL;
static ISmePlayer* g_spobjPlayer = NULL;
static V_VOID MsTicksToTime(V_INT32 i32MiliTicks, V_CHAR pszTime[], V_INT32 i32Len);

const V_CHAR *SmePlayerStateName[] ={"PLAYING","PAUSE","BUFFERING","STOP","ERROR"};

V_UINT64 u64StartTime = 0;
V_UINT64 u64ResetTime = 0;
#if defined (__LINUX__)
static V_VOID InitSubInfo(ST_SUB_INFO &stData)
{
    VOS_Memset(&stData, 0, sizeof(stData));
    stData.U_SUB_INFO.stText.i32PosX = -1;
    stData.U_SUB_INFO.stText.i32PosY = -1;
    stData.U_SUB_INFO.stText.i32W = -1;
    stData.U_SUB_INFO.stText.i32H = -1;
    stData.U_SUB_INFO.stText.u32Color = M_SME_SUB_COLOR_DFT;

    return;
}

static V_VOID PFN_DrawCurPos(IN V_VOID* pvCtx)
{
    ISmePlayer* pobjPlayer = (ISmePlayer*)pvCtx;
    V_INT32 i32Pos = 0;
    V_CHAR acCurpos[TIME_LEN]={0,};
    ST_SUB_INFO stData;

    if(NULL != pobjPlayer)
    {
        i32Pos = pobjPlayer->GetCurrentPosition();
        MsTicksToTime(i32Pos, acCurpos, sizeof(acCurpos) - 1);
        if(g_shdlCurPosDisp)
        {
            Sub_ClearCb(g_shdlCurPosDisp, 0);
            InitSubInfo(stData);
            stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acCurpos;
            stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acCurpos) - 1;
            Sub_DispCb(g_shdlCurPosDisp, stData);
        }
    }

    return;
}


static V_VOID PFN_DrawCurBw(IN V_VOID* pvCtx)
{
    ISmePlayer* pobjPlayer = (ISmePlayer*)pvCtx;
    V_UINT32 u32Bitrate = 0;
    V_CHAR acCurBw[128]={0,};
    ST_SUB_INFO stData;


    if (ICS_FALSE == g_bStreamInfoDisplaySwtich) {
        Sub_ClearCb(g_shdlCurBwDisp, 0);
        return;
    }

    if(NULL != pobjPlayer)
    {
        u32Bitrate = g_spobjPlayer->GetBandwidth();

        if(g_shdlCurBwDisp)
        {
            Sub_ClearCb(g_shdlCurBwDisp, 0);
            InitSubInfo(stData);
            sprintf(acCurBw, "download bitrate:%d kbps",u32Bitrate/1000);
            stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acCurBw;
            stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acCurBw) - 1;
            Sub_DispCb(g_shdlCurBwDisp, stData);
        }
    }

    return;
}


static ICS::Timer* g_spobjDrawCurPosTimer = NULL;
static ICS::Timer* g_spobjDrawBwTimer = NULL;

#endif

static V_VOID SetICSLogLevel(V_CHAR cLogLevel)
{
    switch(cLogLevel)
    {
        case 'L'://no break;
        case 'V':
            ICS_SetLogLevel(ICS_LOG_VERBOSE);
            break;
        case 'D':
            ICS_SetLogLevel(ICS_LOG_DEBUG);
            break;
        case 'I':
            ICS_SetLogLevel(ICS_LOG_DEBUG);
            break;
        case 'W':
            ICS_SetLogLevel(ICS_LOG_WARNING);
            break;
        case 'E':
            ICS_SetLogLevel(ICS_LOG_ERROR);
            break;
        default:
            ICS_SetLogLevel(ICS_LOG_INFO);
            break;
    }

    return;
}

static V_VOID SetSmePlayerLogLevel(ISmePlayer* pobjPlayer, V_CHAR cLogLevel)
{
    V_UINT32 u32Ret = ICS_SUCCESS;
    V_UINT32 u32LogLevel = E_SME_PLAYER_LOG_LEVEL_INFO;
    SmeProperty objProp(SmeProperty::ConstStringKey::m_spszKeyLog);

    switch(cLogLevel)
    {
        case 'L'://no break;
        case 'V':
            STD_OUT("set log level:VERBOSE.\n");
            u32LogLevel = E_SME_PLAYER_LOG_LEVEL_VERBOSE;
            break;
        case 'D':
            STD_OUT("set log level:DEBUG.\n");
            u32LogLevel = E_SME_PLAYER_LOG_LEVEL_DEBUG;
            break;
        case 'I':
            STD_OUT("set log level:INFO.\n");
            u32LogLevel = E_SME_PLAYER_LOG_LEVEL_INFO;
            break;
        case 'W':
            STD_OUT("set log level:WARNING.\n");
            u32LogLevel = E_SME_PLAYER_LOG_LEVEL_WARNING;
            break;
        case 'E':
            STD_OUT("set log level:ERROR.\n");
            u32LogLevel = E_SME_PLAYER_LOG_LEVEL_ERROR;
            break;
        default:
            break;
    }
    objProp.SetUint32Val(u32LogLevel);
    u32Ret = pobjPlayer->SetProperty(objProp);
    if(u32Ret != ICS_SUCCESS)
    {
        STD_OUT("set log property failed(%#x)\n",u32Ret);
    }

    return;
}

static V_VOID MsTicksToTime(V_INT32 i32MiliTicks, V_CHAR pszTime[], V_INT32 i32Len)
{
    V_UINT32 u32Hour = 0;
    V_UINT32 u32Min = 0;


    if(i32MiliTicks < 0 || i32Len <= 3)
    {
        VOS_Snprintf_S(pszTime, i32Len+1, i32Len, "-1");
        ICS_LOGW("duration is %d", i32MiliTicks);

        return ;
    }

    i32MiliTicks = (i32MiliTicks + 500) /1000;//to second.
    u32Hour = i32MiliTicks / 3600;
    i32MiliTicks %= 3600;
    u32Min = i32MiliTicks / 60;
    i32MiliTicks %= 60;

    VOS_Snprintf_S(pszTime, i32Len+1, i32Len, "%02d:%02d:%02d", u32Hour, u32Min, i32MiliTicks);

    return ;
}

class SmePlayerObsTest:public ISmePlayerObs
{
public:
    SmePlayerObsTest(){};
    virtual ~SmePlayerObsTest(){};
    V_UINT32 PlayerNotify(V_UINT32 u32What,
                                V_UINT32 u32Extra1,
                                V_UINT32 u32Extra2)
    {
        ICS_LOGV("PlayerNotify in, u32What=%u, u32Extra1=%u, u32Extra2=%u",
            u32What, u32Extra1, u32Extra2);
        if(E_SME_PLAYER_NOTIFY_PREPARED == u32What)
        {
            STD_OUT("/*******SME-INFO:OnPrepared!*******/");STD_OUT("\n");
            ICS_LOGI("SME-INFO:OnPrepared!");
            STD_OUT("onPrepared cost:%llu ms",VOS_GetTimeTicks64()-u64StartTime);STD_OUT("\n");
            g_sbOnPrepared = ICS_TRUE;
        }
        else if(E_SME_PLAYER_NOTIFY_PLAYBACK_COMPLETED == u32What)
        {
            STD_OUT("/*******SME-INFO:EOS!*******/");STD_OUT("\n");
            ICS_LOGI("SME-INFO:OnPlaybackCompleted!");
            g_sbOnPlaybackCompleted = ICS_TRUE;
        }
        else if(E_SME_PLAYER_NOTIFY_SEEK_COMPLETED == u32What)
        {
            ICS_LOGI("SME-INFO:OnSeekCompleted,eErr=%u,seekres=%ums", u32Extra1, u32Extra2);
        }
        else if(E_SME_PLAYER_NOTIFY_ERROR == u32What)
        {
            ICS_LOGE("SME-INFO:OnErr,eErr=%u,u32Extra2=%u", u32Extra1, u32Extra2);
            if(E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_AUDIO_CODEC");
                g_sbOnErr = ICS_FALSE;
            }
            else if(E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_VIDEO_CODEC");
                g_sbOnErr = ICS_FALSE;
            }
            else if(E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_TEXT_CODEC");
                g_sbOnErr = ICS_FALSE;
            }
            else if(E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_UNSUPPORT_FORMAT");
                g_sbOnErr = ICS_TRUE;
            }
            else if (E_SME_PLAYER_ERROR_TIME_OUT == u32Extra1)
            {
                STD_OUT("/******SME-INFO:TIME_OUT*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnInfo-E_SME_PLAYER_ERROR_TIME_OUT");
                g_sbOnErr = ICS_TRUE;
            }
            else if (E_SME_PLAYER_ERROR_SUBTILE_NOT_FOUND == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_SUBTILE_NOT_FOUND*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_SUBTILE_NOT_FOUND");
                g_sbOnErr = ICS_FALSE;
            }
            else if (E_SME_PLAYER_ERROR_SUBTILE_ACCESS == u32Extra1)
            {
                STD_OUT("/*******SME-INFO:OnErr:E_SME_PLAYER_ERROR_SUBTILE_ACCESS*******/");STD_OUT("\n");
                ICS_LOGE("SME-INFO:OnErr:E_SME_PLAYER_ERROR_SUBTILE_ACCESS");
                g_sbOnErr = ICS_FALSE;
            }
            else
            {
                 g_sbOnErr = ICS_TRUE;
            }
        }
        else if(E_SME_PLAYER_NOTIFY_INFO == u32What)
        {
            ICS_LOGV("SME-INFO:OnInfo,u32Extra1=%u,u32Extra2=%u", u32Extra1, u32Extra2);
            if(E_SME_PLAYER_INFO_BUFFERING_START == u32Extra1)
            {
                STD_OUT("/******SME-INFO:E_SME_PLAYER_INFO_BUFFERING_START*******/");STD_OUT("\n");
                ICS_LOGI("SME-INFO:OnInfo-E_SME_PLAYER_INFO_BUFFERING_START");
            }
            else if (E_SME_PLAYER_INFO_BUFFERING_STOP == u32Extra1)
            {
                STD_OUT("/******SME-INFO:E_SME_PLAYER_INFO_BUFFERING_STOP*******/");STD_OUT("\n");
                //STD_OUT("buffer stop cost:%llu ms",VOS_GetTimeTicks64()-u64StartTime);STD_OUT("\n");
                ICS_LOGI("SME-INFO:OnInfo-E_SME_PLAYER_INFO_BUFFERING_STOP");
            }
        }
        else if(E_SME_PLAYER_NOTIFY_BUFFER_UPDATE == u32What)
        {
            static V_UINT32  u32LastPercent = 0;
            if (u32Extra1 != u32LastPercent)
            {
                STD_OUT("/******SME-INFO:OnBufferUpdate,%d,%d*******/",u32Extra1, u32Extra2);STD_OUT("\n");
                ICS_LOGI("SME-INFO:OnBufferUpdate,u32Extra1=%u,u32Extra2=%u",u32Extra1, u32Extra2);
                u32LastPercent = u32Extra1;
            }
        }
        else if(E_SME_PLAYER_NOTIFY_VIDEO_SIZE_CHANGED == u32What)
        {
            ICS_LOGI("SME-INFO:OnVideoSizeChanged,the resolution is %u * %u",
                u32Extra1, u32Extra2);
#if defined (__LINUX__)
            // Display th solution
            V_CHAR acDuration[RESOLUTION_LEN]={0};
            VOS_Snprintf_S(acDuration, sizeof(acDuration)-1, sizeof(acDuration)-2, "%u*%u", u32Extra1, u32Extra2);
            ST_SUB_INFO stData;
            InitSubInfo(stData);
            stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acDuration;
            stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acDuration) + 1;
            if(g_shdlResolutionDisp)
            {
                Sub_ClearCb(g_shdlResolutionDisp, 0);
                Sub_DispCb(g_shdlResolutionDisp, stData);
            }
            else
                ICS_LOGE("g_sstResolutionChangedDispRect Init Failed");
#endif
        }
        else if(E_SME_PLAYER_NOTIFY_HTTP_ERR == u32What)
        {
            ICS_LOGI("SME-INFO:OnHttpError");
            STD_OUT("/*******HTTP ERROR, statuscode is %u*******/", u32Extra1);STD_OUT("\n");
        }
        else if( E_SME_PLAYER_NOTIFY_FRAGMENT_UPDATE == u32What)
        {
            ICS_LOGI("SME-INFO:OnFragmentInfo updated");
            if(g_bShowFragmentInfo)
            {
                STD_OUT("===============BEGIN============================");STD_OUT("\n");
                STD_OUT("SME-INFO:OnFragmentInfo updated:");STD_OUT("\n");

                ST_FRAGMENT_INFO *pstFragmentInfo = (ST_FRAGMENT_INFO *)u32Extra1;
                if (pstFragmentInfo) {
                    STD_OUT("manifest uri:%s",pstFragmentInfo->i8ManifestUri);STD_OUT("\n");
                    STD_OUT("uri:%s",pstFragmentInfo->i8FragmentUri);STD_OUT("\n");
                    STD_OUT("size:%llu bytes",pstFragmentInfo->u64FragmentSize);STD_OUT("\n");
                    STD_OUT("start time:%llu",pstFragmentInfo->u64DownloadStartTime);STD_OUT("\n");
                    STD_OUT("end time:%llu",pstFragmentInfo->u64DownloadEndTime);STD_OUT("\n");
                    STD_OUT("total download time:%llu us",pstFragmentInfo->u64DownloadTotalTime);STD_OUT("\n");
                }
                STD_OUT("=================end==========================");STD_OUT("\n");
            }
        }
        else if(E_SME_PLAYER_NOTIFY_PLAYER_STATE_CHANGED == u32What)
        {
            E_SME_PLAYER_STATE  eOldState = (E_SME_PLAYER_STATE)u32Extra1;
            E_SME_PLAYER_STATE  eNewState = (E_SME_PLAYER_STATE)u32Extra2;

            STD_OUT("/*******Player state change from [%s] to [%s]*******/",
                    SmePlayerStateName[eOldState],
                    SmePlayerStateName[eNewState]);STD_OUT("\n");
        }
        else
        {
            ;//donothing
        }

        return 0;
    }
};


V_VOID PrintHelpOfTestCase()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:1 for normal player test(use prepareAsync)");STD_OUT("\n");
    STD_OUT("Input:2 for normal player test(use prepare)");STD_OUT("\n");
    STD_OUT("Input:3 for MemLeak");STD_OUT("\n");
    STD_OUT("Input:4 for MemLeak_forbtv");STD_OUT("\n");
    STD_OUT("Input:5 for StartResetTest for sme player");STD_OUT("\n");
    STD_OUT("Input:6 for seek test for sme player");STD_OUT("\n");
    STD_OUT("Input:7 for Muilt solution test ");STD_OUT("\n");
    STD_OUT("Input:8 for Pause/Resume test for sme player");STD_OUT("\n");
    STD_OUT("Input:9 for StartStop test for sme player");STD_OUT("\n");
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
    STD_OUT("Input:st for start");STD_OUT("\n");
    STD_OUT("Input:sp for stop");STD_OUT("\n");
    STD_OUT("Input:sm for Muilt solution tests");STD_OUT("\n");
    STD_OUT("Input:\"sk xxx\" for seek to xxx s");STD_OUT("\n");
    STD_OUT("Input:\"sl xxx\" for set loop playback, 0 disable, others enable");STD_OUT("\n");
    STD_OUT("Input:p for pause");STD_OUT("\n");
    STD_OUT("Input:r for resume");STD_OUT("\n");
    STD_OUT("Input:g1 for get duration");STD_OUT("\n");
    STD_OUT("Input:g2 for get isplaying");STD_OUT("\n");
    STD_OUT("Input:g3 for get current position");STD_OUT("\n");
    STD_OUT("Input:g4 for get start cost");STD_OUT("\n");
    STD_OUT("Input:g5 for get Cur Fps&Avg Fps");STD_OUT("\n");
    STD_OUT("Input:g6 display Cur Stream Info on UI");STD_OUT("\n");
    STD_OUT("Input:g7 for get player state");STD_OUT("\n");
    STD_OUT("Input:g8 for get audio hdmi mode");STD_OUT("\n");
    STD_OUT("Input:g9 for get buffer status");STD_OUT("\n");
    STD_OUT("Input:ga for get audio track info");STD_OUT("\n");
    STD_OUT("Input:gm for get media info");STD_OUT("\n");
    STD_OUT("Input:gb for get video display mode info");STD_OUT("\n");
    STD_OUT("Input:gc for get video display rect info");STD_OUT("\n");
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
    STD_OUT("Input:cc for get hls live channel change");STD_OUT("\n");
    STD_OUT("========================================");STD_OUT("\n");
    STD_OUT("Input:\"s1 xxx\" for set log level, xxx may be:L|V|D|I|W|E");STD_OUT("\n");
    STD_OUT("Input:\"s2 xxx\" for setting bitrate xxx kbps");STD_OUT("\n");
    STD_OUT("Input:\"s3 xxx\" for setting audio hdmi mode xxx");STD_OUT("\n");
    STD_OUT("Input:\"s4 xx ww hh\" for setting video display mode and W&H radio,xx may be 0|1,ww/hh may be 16/9|4/3");STD_OUT("\n");
    STD_OUT("Input:\"s5 xx yy width height\" for setting windows rect ");STD_OUT("\n");
    STD_OUT("Input:\"s6 xxx\" for selecting audio track id xxx");STD_OUT("\n");
    STD_OUT("Input:\"s7 xxx\" for selecting switch channel mode xxx");STD_OUT("\n");
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}


V_VOID StreamStartModeCb(V_VOID* pvCtx,V_UINT64* u64BitrateList,
         V_UINT32 u32BitrateNum,
         V_BOOL* bIsAdaptive,
         V_UINT64* u64ConnectSpeed)
{
   V_UINT32 i = 0;
   V_CHAR acPlayOpCmd[256] = {0,};
   V_UINT32 u32StartupBitrateIdx = 0;

   //COMMAND_LOCK;
   VOS_ThdMutexLock(&g_mutex);
   STD_OUT("***************Bitrate Information********************");STD_OUT("\n");
   for (i = 0; i < u32BitrateNum;i++) {
     STD_OUT("Input:%d for %llu",i+1,u64BitrateList[i]);STD_OUT("\n");
   }
   STD_OUT("Input:0 for default");STD_OUT("\n");
   STD_OUT("*******************************************************");STD_OUT("\n");

   fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
   u32StartupBitrateIdx = atoi(&acPlayOpCmd[0]);
   if (u32StartupBitrateIdx >= 0 && u32StartupBitrateIdx < u32BitrateNum) {
       STD_OUT("You select the number %d",u32StartupBitrateIdx);STD_OUT("\n");
       STD_OUT("=========================");STD_OUT("\n");
   }
   //COMMAND_UNLOCK;

   if (u32StartupBitrateIdx == 0)
   {
      *bIsAdaptive = 1;
      *u64ConnectSpeed = 0;
   }
   else {
       if (u32StartupBitrateIdx >= 0 && u32StartupBitrateIdx <= u32BitrateNum) {
           *u64ConnectSpeed = u64BitrateList[u32StartupBitrateIdx - 1];
       }
       else {
           *u64ConnectSpeed = (V_UINT64)u32StartupBitrateIdx;
           STD_OUT("please notice, set download bitrate to :%lld",*u64ConnectSpeed);STD_OUT("\n");
       }
       STD_OUT("Do you need adaptive bitrate? n:No,y:Yes");STD_OUT("\n");
       memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
       fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
       if ('n' == acPlayOpCmd[0] || 'N' == acPlayOpCmd[0]) {
           *bIsAdaptive = 0;
       }
       else
       {
           *bIsAdaptive = 1;
       }
   }

   VOS_ThdMutexUnLock(&g_mutex);
}

V_VOID BitrateChangedCb(V_VOID* pvCtx,V_UINT64 u64VBitrate,V_UINT64 u64ABitrate)
{

    ISmePlayer* pobjPlayer = (ISmePlayer*)pvCtx;
    V_CHAR acCurData[256]={0,};
    ST_SUB_INFO stData;
    static V_UINT64 videoBt = 0;
    static V_UINT64 audioBt = 0;

    videoBt = u64VBitrate ? u64VBitrate:videoBt;
    audioBt = u64ABitrate ? u64ABitrate:audioBt;

    STD_OUT("BitrateChangedCb,v:%lld,a:%lld",u64VBitrate,u64ABitrate); STD_OUT("\n");
#if defined (__LINUX__)
    if (ICS_FALSE == g_bStreamInfoDisplaySwtich) {
        Sub_ClearCb(g_shdlCurVBtDisp, 0);
        Sub_ClearCb(g_shdlCurABtDisp, 0);
        return;
    }

    if(NULL != pobjPlayer)
    {

        if(g_shdlCurVBtDisp)
        {
            Sub_ClearCb(g_shdlCurVBtDisp, 0);
            InitSubInfo(stData);
            if (videoBt)
            {
                sprintf(acCurData, "video bitrate:%lld kbps",videoBt/1000);
                stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acCurData;
                stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acCurData) - 1;

                Sub_DispCb(g_shdlCurVBtDisp, stData);
            }
        }

        if(g_shdlCurABtDisp)
        {
            Sub_ClearCb(g_shdlCurABtDisp, 0);
            InitSubInfo(stData);
            VOS_Memset(acCurData,0,sizeof(acCurData));
            if (audioBt)
            {
                sprintf(acCurData, "audio bitrate:%lld kbps",audioBt/1000);
                stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acCurData;
                stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acCurData) - 1;
                Sub_DispCb(g_shdlCurABtDisp, stData);
            }
        }
    }

#endif
    return;

}



static V_UINT32 TestRunner(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;
    //V_UINT64 u64StartTime = 0;
    ST_SubCallback stSubCb;
    V_CHAR acDuration[TIME_LEN]={0,};
    V_INT32 i32Duration = 0;
    ST_SUB_INFO stData;

    ICS_LOGI("TestRunner in");

    g_sbOnErr = ICS_FALSE;
    g_sbOnPlaybackCompleted = ICS_FALSE;
    g_sbOnPrepared = ICS_FALSE;

    g_su64StartCost = 0;
    u64StartTime = VOS_GetTimeTicks64();
    ICS_LOGI("wh-add start time=%llu", u64StartTime);
    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    //set sme playe loglevel
    SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);

    u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetDataSource failed(%u)", u32Err);
        goto EXIT;
    }
    g_spobjPlayer->SetBufferTime(g_u32BufferTime);
    g_spobjPlayer->SetUserAgent(g_bSetUserAgent);
    g_spobjPlayer->SetByteRange(g_bSetByteRange);
    g_spobjPlayer->SetReferer(g_bSetReferer);
    g_spobjPlayer->SetBufferWaterMark(g_u32BufferHighPercent, g_u32BufferLowPercent);
#if defined __LINUX__
    if(NULL != g_spszSubUri)
    {
        g_shdlSubDisp = Sub_Disp_Init(g_sstSubDispRect);
        if(NULL == g_shdlSubDisp)
        {
            ICS_LOGE("Sub_Disp_Init failed why?", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->SetSubDataSource(g_spszSubUri);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetDataSource_sub failed(%u)", u32Err);
            goto EXIT;
        }

        VOS_Memset(&stSubCb, 0, sizeof(stSubCb));
        stSubCb.pfnSubDisplayCb = Sub_DispCb;
        stSubCb.pfnSubClearCb = Sub_ClearCb;
        stSubCb.pvCtx = g_shdlSubDisp;
        u32Err = g_spobjPlayer->RegisterSubCallback(stSubCb);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set RegisterSubCallback failed(%u)", u32Err);
            goto EXIT;
        }
    }

    g_shdlDurationDisp = Sub_Disp_Init(g_sstDurationDispRect);
    if(NULL == g_shdlDurationDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstDurationDispRect failed why?");
    }

    g_shdlCurPosDisp = Sub_Disp_Init(g_sstCurPosDispRect);
    if(NULL == g_shdlCurPosDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstCurPosDispRect failed why?");
    }

    g_shdlCurBwDisp = Sub_Disp_Init(g_sstBandwidthDispRect);
    if(NULL == g_shdlCurBwDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstBandwidthDispRect failed why?");
    }

    g_shdlCurVBtDisp = Sub_Disp_Init(g_sstVideoBtDispRect);
    if(NULL == g_shdlCurVBtDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstVideoBtDispRect failed why?");
    }

    g_shdlCurABtDisp = Sub_Disp_Init(g_sstAudioBtDispRect);
    if(NULL == g_shdlCurABtDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstAudioBtDispRect failed why?");
    }

    g_shdlResolutionDisp = Sub_Disp_Init(g_sstResolutionChangedDispRect);
    if(NULL == g_shdlResolutionDisp)
    {
        ICS_LOGE("Sub_Disp_Init g_sstResolutionChangedDispRect failed why?", u32Err);
    }

    g_spobjDrawCurPosTimer = new ICS::Timer("draw cur pos", PFN_DrawCurPos,
        g_spobjPlayer, 400);
    if(NULL == g_spobjDrawCurPosTimer)
    {
        ICS_LOGE("g_spobjDrawCurPosTimer failed ");
    }

    g_spobjDrawBwTimer = new ICS::Timer("draw bandwidth", PFN_DrawCurBw,
        g_spobjPlayer, 1000);
    if(NULL == g_spobjDrawBwTimer)
    {
        ICS_LOGE("g_spobjDrawBwTimer failed ");
    }
#endif

#ifndef __USE_ANDROID_SURFACE__
    u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
        goto EXIT;
    }
#else
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
    u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
        g_pvSurfaceHdl));
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    g_spobjPlayer->RegBitrateChangedCb(BitrateChangedCb,g_spobjPlayer);

    if('1' == g_scInputType)
    {
        if (g_bEnableSetCookies)
        {
          VOS_ThdMutexLock(&g_mutex);
          V_CHAR acPlayOpCmd[256] = {0,};
          V_UINT32 i = 0, j;
          STD_OUT("***************Cookies Information********************");STD_OUT("\n");
          do {
            memset (acPlayOpCmd, '\0', 256);
            fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
            acPlayOpCmd[strlen(acPlayOpCmd) - 1] = '\0';
            for (j = 0; j< strlen(acPlayOpCmd); j++)
              g_spsSetCookies[i] = strdup(acPlayOpCmd);
            i++;
          } while (acPlayOpCmd[0] != 'q');
          if (!strcmp(g_spsSetCookies[i-1], "q"))
            g_spsSetCookies[i-1] = NULL;
          g_spobjPlayer->SetCookies(g_spsSetCookies);

          VOS_ThdMutexUnLock(&g_mutex);
        }

        u32Err = g_spobjPlayer->PrepareAsync();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
            goto EXIT;
        }

        if (g_bEnableSetBitrate)
            g_spobjPlayer->RegManifestCompleteCb(StreamStartModeCb,NULL);

        while(ICS_FALSE == g_sbOnPrepared)
        {
            VOS_MSleep(20);
            if((ICS_FALSE != g_sbOnErr) || (ICS_FALSE != g_sbQuitPlay))
            {
                goto EXIT;
            }
        }
    }
    else if ('2' == g_scInputType)
    {
        u32Err = g_spobjPlayer->Prepare();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
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
    i32Duration = g_spobjPlayer->GetDuration();
    ICS_LOGD("duration:%dms", i32Duration);

#if defined __LINUX__

    MsTicksToTime(i32Duration, acDuration, sizeof(acDuration) -1 );
    Sub_ClearCb(g_shdlDurationDisp, 0);

    InitSubInfo(stData);
    stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acDuration;
    stData.U_SUB_INFO.stText.u32SubDataLen = strlen(acDuration) + 1;

    Sub_DispCb(g_shdlDurationDisp, stData);
#endif

    u32Err = g_spobjPlayer->Start();
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }
    g_su64StartCost = VOS_GetTimeTicks64() - u64StartTime;
    ICS_LOGI("wh-add stop time=%llu", VOS_GetTimeTicks64());
    STD_OUT("SMEPlayer:start cost:%lldms\n", g_su64StartCost);

    if (g_spszAudioHdmiMode)
    {
        g_spobjPlayer->SetAudioHdmiMode(g_spszAudioHdmiMode);
    }

#if defined __LINUX__
    g_spobjDrawCurPosTimer->Open();
    g_spobjDrawCurPosTimer->Start();
    g_spobjDrawBwTimer->Open();
    g_spobjDrawBwTimer->Start();
#endif

    while((ICS_FALSE == g_sbOnErr) && (ICS_FALSE == g_sbQuitPlay))
    {
        VOS_MSleep(100);
        UTILS_M_BRK_IF(ICS_TRUE == g_sbOnPlaybackCompleted);
    }

EXIT:
#if defined __LINUX__
    g_spobjDrawCurPosTimer->Pause();
    g_spobjDrawCurPosTimer->Close();
    g_spobjDrawBwTimer->Pause();
    g_spobjDrawBwTimer->Close();
#endif

    STD_OUT("reset begin"); STD_OUT("\n");
    u64ResetTime = VOS_GetTimeTicks64();
    u32Err = g_spobjPlayer->Reset();
    g_su64ResetCost = VOS_GetTimeTicks64() - u64ResetTime;
    STD_OUT("reset end"); STD_OUT("\n");
    STD_OUT("reset cost %llums\n", g_su64ResetCost);
    VOS_ThdMutexLock(&g_sstMutex);
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    VOS_ThdMutexUnLock(&g_sstMutex);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif

#if defined __LINUX__
    if(g_shdlSubDisp)
    {
        Sub_Disp_DeInit(g_shdlSubDisp);
        g_shdlSubDisp = NULL;
    }

    if(g_shdlDurationDisp)
    {
        Sub_Disp_DeInit(g_shdlDurationDisp);
        g_shdlDurationDisp = NULL;
    }

    if(g_shdlCurPosDisp)
    {
        Sub_Disp_DeInit(g_shdlCurPosDisp);
        g_shdlCurPosDisp = NULL;
    }

    if(g_shdlCurBwDisp)
    {
        Sub_Disp_DeInit(g_shdlCurBwDisp);
        g_shdlCurBwDisp = NULL;
    }

    if(g_shdlCurVBtDisp)
    {
        Sub_Disp_DeInit(g_shdlCurVBtDisp);
        g_shdlCurVBtDisp = NULL;
    }

    if(g_shdlCurABtDisp)
    {
        Sub_Disp_DeInit(g_shdlCurABtDisp);
        g_shdlCurABtDisp = NULL;
    }

    if(g_shdlResolutionDisp)
    {
        Sub_Disp_DeInit(g_shdlResolutionDisp);
        g_shdlResolutionDisp = NULL;
    }

    UTILS_MSAFEDEL(g_spobjDrawCurPosTimer);
    g_spobjDrawCurPosTimer = NULL;

    UTILS_MSAFEDEL(g_spobjDrawBwTimer);
    g_spobjDrawBwTimer = NULL;

#endif


    ICS_LOGI("TestRunner out!!!");

    return 0;
}

static void
save_mem_info()
{
    V_UINT32 pid = getpid();
    V_CHAR cmd[256];

    sprintf(cmd, "cat /proc/%d/status |grep VmRSS >> /mnt/smelog/mem_stat_%d.dat",pid,pid);
    system(cmd);
}

//t_smeplayer /data/test.mp4
//t_smeplayer /data/test.mp4
static V_UINT32 StartResetTestRunner(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;

    ICS_LOGI("StartResetTestRunner in");

    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

#ifdef __USE_ANDROID_SURFACE__
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    for(V_UINT32 i = 0 ; (i < g_u32TestCount) && (!g_sbQuitPlay); i++)
    {
        g_sbOnErr = ICS_FALSE;
        g_sbOnPlaybackCompleted = ICS_FALSE;
        g_sbOnPrepared = ICS_FALSE;

        ICS_LOGE("===================Test count:%d", i);
        STD_OUT("===================Test count:%d", i);STD_OUT("\n");

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetDataSource failed(%u)", u32Err);
            goto EXIT;
        }
        g_spobjPlayer->SetBufferTime(g_u32BufferTime);
        g_spobjPlayer->SetBufferWaterMark(g_u32BufferHighPercent, g_u32BufferLowPercent);
        if(!g_pvSurfaceHdl)
        {
            u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                DISPLAY_WIDTH, DISPLAY_HEIGHT);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
                goto EXIT;
            }
        }
#ifdef __USE_ANDROID_SURFACE__
        else
        {
            u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                g_pvSurfaceHdl));
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetSurface failed(%u)", u32Err);
                goto EXIT;
            }
        }
#endif
        if('1' == g_scInputType)
        {
            u32Err = g_spobjPlayer->PrepareAsync();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
                goto EXIT;
            }

            while(!g_sbOnPrepared)
            {
                VOS_MSleep(10);
                if(ICS_FALSE != g_sbQuitPlay)
                {
                    goto EXIT;
                }
                if(g_sbOnErr)
                {
                    break;
                }
            }
        }
        else if ('2' == g_scInputType)
        {
            u32Err = g_spobjPlayer->Prepare();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
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

        ICS_LOGD("duration:%dms", g_spobjPlayer->GetDuration());

        if(!g_sbOnErr)
        {
            u32Err = g_spobjPlayer->Start();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("start failed(%u)", u32Err);
                goto EXIT;
            }
        }

        if(g_u32SleepInterval <= 0)
        {
            g_u32SleepInterval = 5;
        }

        VOS_MSleep(g_u32SleepInterval);

        u32Err = g_spobjPlayer->Reset();
        save_mem_info();
    }
EXIT:
    u32Err = g_spobjPlayer->Reset();

    STD_OUT("destroy now"); STD_OUT("\n");
    VOS_ThdMutexLock(&g_sstMutex);
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
    VOS_ThdMutexUnLock(&g_sstMutex);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif
    ICS_LOGI("StartResetTestRunner out!!!");
    STD_OUT("StartResetTestRunner out!!!"); STD_OUT("\n");

    return 0;
}

static V_UINT32 TestMuiltResolution(V_CHAR* pszCmd)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;

    switch(pszCmd[0])
    {
        case 's':
        {
            if('t' == pszCmd[1])
            {
                //start
                u32Err = g_spobjPlayer->Start();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("Start failed(%u)", u32Err);
                }
#if defined __LINUX__
                else
                {
                    g_spobjDrawCurPosTimer->Open();
                    g_spobjDrawCurPosTimer->Start();
                }
#endif
                break;
            }

            if('p' == pszCmd[1])
            {
                //stop
                u32Err = g_spobjPlayer->Stop();
                u32Err = g_spobjPlayer->Reset();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("Stop failed(%u)", u32Err);
                }
#if defined __LINUX__
                else
                {
                    g_spobjDrawCurPosTimer->Pause();
                    g_spobjDrawCurPosTimer->Close();
                }
#endif

                u32Err = ICS_FAIL;
                g_sbQuitPlay = ICS_TRUE;
                break;
            }

            if(('k' == pszCmd[1]) && (strlen(pszCmd) > 3))
            {
                V_INT32 i32SeekMs = atoi(&pszCmd[3]);

                i32SeekMs *= 1000;
#if defined __LINUX__
                if(g_shdlCurPosDisp)
                {
                    V_CHAR acTime[TIME_LEN] = {0,};
                    ST_SUB_INFO stData;

                    MsTicksToTime(i32SeekMs, acTime, sizeof(acTime) - 1);
                    InitSubInfo(stData);
                    stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acTime;
                    stData.U_SUB_INFO.stText.u32SubDataLen = sizeof(acTime) - 1;
                    Sub_ClearCb(g_shdlCurPosDisp, 0);
                    Sub_DispCb(g_shdlCurPosDisp, stData);
                }
#endif
                ICS_LOGD("seek to:%d ms", i32SeekMs);
                u32Err = g_spobjPlayer->SeekTo(i32SeekMs);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("SeekTo %dms,failed(%u)", i32SeekMs, u32Err);
                }
            }

            if(('1' == pszCmd[1]) && (strlen(pszCmd) > 4))
            {
                //"s1 xxx" to set log level
                SetSmePlayerLogLevel(g_spobjPlayer, pszCmd[3]);
            }

            if('m' == pszCmd[1])
            {
                //set bitrate for streaming media file
                int num = 1;
                while(num <= 3)
                {
                    //for steel
                    /*
                    ICS_LOGW("Set Muilt solution 11000");
                    g_spobjPlayer->SetBitrate(11000);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 270");
                    g_spobjPlayer->SetBitrate(270);
                    sleep(10);
                    */
                    //for clock
                    ICS_LOGW("Set Muilt solution 800");
                    g_spobjPlayer->SetBitrate(800);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 600");
                    g_spobjPlayer->SetBitrate(600);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 200");
                    g_spobjPlayer->SetBitrate(200);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 60");
                    g_spobjPlayer->SetBitrate(60);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 200");
                    g_spobjPlayer->SetBitrate(200);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 600");
                    g_spobjPlayer->SetBitrate(600);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 800");
                    g_spobjPlayer->SetBitrate(800);

                    /*
                    //for car
                    ICS_LOGW("Set Muilt solution 4190");
                    g_spobjPlayer->SetBitrate(4190);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 2070");
                    g_spobjPlayer->SetBitrate(2070);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 869");
                    g_spobjPlayer->SetBitrate(869);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 686");
                    g_spobjPlayer->SetBitrate(686);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 264");
                    g_spobjPlayer->SetBitrate(264);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 100");
                    g_spobjPlayer->SetBitrate(100);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 264");
                    g_spobjPlayer->SetBitrate(264);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 686");
                    g_spobjPlayer->SetBitrate(686);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 869");
                    g_spobjPlayer->SetBitrate(869);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 2070");
                    g_spobjPlayer->SetBitrate(2070);
                    sleep(10);
                    ICS_LOGW("Set Muilt solution 5000");
                    g_spobjPlayer->SetBitrate(5000);
                    */
                    num++;
                }
                break;
            }
            break;
        }
        case 'p':
#if defined __LINUX__
            if(g_spobjDrawCurPosTimer)
            {
                g_spobjDrawCurPosTimer->Pause();
            }
#endif

            u32Err = g_spobjPlayer->Pause();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("Pause failed(%u)", u32Err);
#if defined __LINUX__
                if(g_spobjDrawCurPosTimer)
                {
                    g_spobjDrawCurPosTimer->Start();
                }
#endif
            }
            break;
        case 'r':
            u32Err = g_spobjPlayer->Resume();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("Resume failed(%u)", u32Err);
            }
#if defined __LINUX__
            else
            {
                g_spobjDrawCurPosTimer->Start();
            }
#endif
            break;
        case 'g':
        {
            switch(pszCmd[1])
            {
                case '1':
                    ICS_LOGI("duration=%dms", g_spobjPlayer->GetDuration());
                    STD_OUT("duration=%dms\n", g_spobjPlayer->GetDuration());STD_OUT("\n");
                    break;
                case '2':
                    ICS_LOGI("isPlaying=%d", g_spobjPlayer->IsPlaying());
                    STD_OUT("isPlaying=%d\n", g_spobjPlayer->IsPlaying());STD_OUT("\n");

                    break;
                case '3':
                    ICS_LOGI("CurPosition=%dms", g_spobjPlayer->GetCurrentPosition());
                    STD_OUT("CurPosition=%dms\n", g_spobjPlayer->GetCurrentPosition());STD_OUT("\n");
                    break;
                case '4':
                    ICS_LOGI("SMEPlayer:start cost:%lldms", g_su64StartCost);
                    STD_OUT("SMEPlayer:start cost:%lldms\n", g_su64StartCost);
                    break;
                case '5':
                {
                    SmeProperty objReqAvg(SmeProperty::ConstStringKey::m_spszKeyFps);
                    SmeProperty objRespAvg(SmeProperty::ConstStringKey::m_spszKeyFps);
                    objReqAvg.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    objRespAvg.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    g_spobjPlayer->GetProperty(objReqAvg, objRespAvg);

                    SmeProperty objReq(SmeProperty::ConstStringKey::m_spszKeyCurrFps);
                    SmeProperty objResp(SmeProperty::ConstStringKey::m_spszKeyCurrFps);
                    objReq.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    objResp.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    g_spobjPlayer->GetProperty(objReq, objResp);

                    STD_OUT("SMEPlayer:CurFps:%s,AvgFps:%s\n", objResp.GetStringVal(),
                        objRespAvg.GetStringVal());
                    break;
                }
                case '6':
                {
                    g_bStreamInfoDisplaySwtich = !g_bStreamInfoDisplaySwtich;
                    BitrateChangedCb(g_spobjPlayer,0,0);
                    break;
                }
                default:
                    ICS_LOGW("Unkown g%c XX", pszCmd[1]);
                    break;
            }
            break;
        }
        default:
            ICS_LOGW("Unkown %c XX", pszCmd[0]);
            break;
    }

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

static V_UINT32 TestPlayerOp(V_CHAR* pszCmd)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;

    switch(pszCmd[0])
    {
#ifdef __LINUX__
        case 'a':
#if 0
            if ('1' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_UINT32 mute = atoi(&pszCmd[3]);

                mute = (0 == mute) ? 0 : 1;

                ICS_LOGI("set audio mute:%d", mute);
                u32Err = g_spobjPlayer->SetAudioMute(mute);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set mute %u (%d), failed", mute, u32Err);
                }
                break;
            }
            if ('2' == pszCmd[1])
            {
                V_UINT32 mute = 0;

                u32Err = g_spobjPlayer->GetAudioMute(&mute);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
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
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set volume %u (%d), failed", volume, u32Err);
                }

                break;
            }
            if ('4' == pszCmd[1])
            {
                V_UINT32 volume = 0;

                u32Err = g_spobjPlayer->GetAudioVolume(&volume);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
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
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set audio channel mode %s (%d), failed", audio_channel_mode_to_str(mode), u32Err);
                }

                break;
            }
            if ('6' == pszCmd[1])
            {
                A_TRACK_CHANNEL_MODE_E mode;

                u32Err = g_spobjPlayer->GetAudioChannelMode(&mode);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
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
            if('t' == pszCmd[1])
            {
                //start
                u32Err = g_spobjPlayer->Start();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("Start failed(%u)", u32Err);
                }
#if defined __LINUX__
                else
                {
                    g_spobjDrawCurPosTimer->Open();
                    g_spobjDrawCurPosTimer->Start();
                }
#endif
                break;
            }

            if('p' == pszCmd[1])
            {
                //stop
                u32Err = g_spobjPlayer->Stop();
                // u32Err = g_spobjPlayer->Reset();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("Stop failed(%u)", u32Err);
                }
#if defined __LINUX__
                else
                {
                    g_spobjDrawCurPosTimer->Pause();
                    g_spobjDrawCurPosTimer->Close();
                }
#endif

                u32Err = ICS_FAIL;
                // g_sbQuitPlay = ICS_TRUE;
                break;
            }

            if(('k' == pszCmd[1]) && (strlen(pszCmd) > 3))
            {
                V_INT32 i32SeekMs = atoi(&pszCmd[3]);

                i32SeekMs *= 1000;
#if defined __LINUX__
                if(g_shdlCurPosDisp)
                {
                    V_CHAR acTime[TIME_LEN] = {0,};
                    ST_SUB_INFO stData;

                    MsTicksToTime(i32SeekMs, acTime, sizeof(acTime) - 1);
                    InitSubInfo(stData);
                    stData.U_SUB_INFO.stText.pu8SubData = (V_UINT8*)acTime;
                    stData.U_SUB_INFO.stText.u32SubDataLen = sizeof(acTime) - 1;
                    Sub_ClearCb(g_shdlCurPosDisp, 0);
                    Sub_DispCb(g_shdlCurPosDisp, stData);
                }
#endif
                ICS_LOGD("seek to:%d ms", i32SeekMs);
                u32Err = g_spobjPlayer->SeekTo(i32SeekMs);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("SeekTo %dms,failed(%u)", i32SeekMs, u32Err);
                }

                break;
            }

            if(('l' == pszCmd[1]) && (strlen(pszCmd) > 3))
            {
                V_INT32 i32Loop = atoi(&pszCmd[3]);

                i32Loop = (0 == i32Loop) ? 0 : 1;

                ICS_LOGI("set loop:%d", i32Loop);
                u32Err = g_spobjPlayer->SetLoop((V_BOOL)i32Loop);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set loop(%d), failed", i32Loop, u32Err);
                }
                break;
            }

            if(('1' == pszCmd[1]) && (strlen(pszCmd) > 3))
            {
                //"s1 xxx" to set log level
                ICS_LOGI("set log-level:%c", pszCmd[3]);
                SetSmePlayerLogLevel(g_spobjPlayer, pszCmd[3]);
            }

            if('2' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                //set bitrate for streaming media file
                V_UINT32 u32Bitrate = atoi(&pszCmd[3]);
                ICS_LOGI("set bitrate:%d bps", u32Bitrate);
                g_spobjPlayer->SetBitrate(u32Bitrate);

                break;
            }

            if ('3' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                if (g_spszAudioHdmiMode)
                {
                    STD_OUT("Current Audio Hdmi Mode %s\n",
                            g_spszAudioHdmiMode);
                    ICS_LOGI("set audio hdmi mode to %s", g_spszAudioHdmiMode);
                    free(g_spszAudioHdmiMode);
                }

                g_spszAudioHdmiMode = strdup(&pszCmd[3]);
                ICS_LOGI("set audio hdmi mode to %s", g_spszAudioHdmiMode);
                g_spobjPlayer->SetAudioHdmiMode(g_spszAudioHdmiMode);

                break;
            }

            if('4' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                //set Display Mode,and W&H
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
                    ICS_LOGW("the format of s4 is wrong:no display mode");
                    break;
                }
                u32DisplayMode = atoi(res);

                if(u32DisplayMode == 0)
                {
                    g_spobjPlayer->SetDisplayMode(E_SME_PLAYER_DIS_MODE_FULL, NULL);
                }
                else if(u32DisplayMode == 1)
                {
                    res = strtok(NULL, " ");
                    if(res != NULL)
                    {
                        u32UIWidth = atoi(res);
                        res = strtok(NULL, " ");
                        if(res != NULL)
                            u32UIHeight = atoi(res);
                        else
                            ICS_LOGW("the format of s4 is wrong:no height");
                    }
                    else
                        ICS_LOGW("the format of s4 is wrong:no width");

                    ICS_LOGI("displaymode : %d,  W & H is %d & %d", u32DisplayMode, u32UIWidth, u32UIHeight);
                    ST_DISP_RATIO stDispRatio;
                    stDispRatio.u32DispRatioW = u32UIWidth;
                    stDispRatio.u32DispRatioH = u32UIHeight;
                    g_spobjPlayer->SetDisplayMode(E_SME_PLAYER_DIS_MODE_FITIN, &stDispRatio);

                }
                else
                {
                    g_spobjPlayer->SetDisplayMode((E_SME_PLAYER_DISPLAY_MODE)u32DisplayMode, NULL);
                    break;
                }

                break;
            }

            if('5' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_INT32 i32StartX = 0;
                V_INT32 i32StartY = 0;
                V_INT32 i32Width = 0;
                V_INT32 i32Height = 0;

                V_CHAR *res = NULL;
	            res = strtok(pszCmd, " ");
                if(res != NULL)
                    res = strtok(NULL, " ");
                if(res == NULL)
                {
                    ICS_LOGW("the format of s5 is wrong:no startX");
                    break;
                }
                i32StartX = atoi(res);

                res = strtok(NULL, " ");
                if(res != NULL)
                    i32StartY = atoi(res);
                else
                {
                    ICS_LOGW("the format of s5 is wrong:no startY");
                    break;
                }

                res = strtok(NULL, " ");
                if(res != NULL)
                    i32Width = atoi(res);
                else
                {
                    ICS_LOGW("the format of s5 is wrong:no width");
                    break;
                }

                res = strtok(NULL, " ");
                if(res != NULL)
                    i32Height = atoi(res);
                else
                {
                    ICS_LOGW("the format of s5 is wrong:no height");
                    break;
                }

                ICS_LOGI("UI Set Windows is [%d, %d, %d, %d] ", i32StartX, i32StartY, i32Width, i32Height);
                g_spobjPlayer->SetVDispRect(i32StartX, i32StartY, i32Width, i32Height);
            }

            if ('6' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_INT32 i32AudioTrack = atoi(&pszCmd[3]);
                ICS_LOGI("select audio track:%d", i32AudioTrack);
                g_spobjPlayer->SetAudioTrack(i32AudioTrack);

                break;
            }

            if('7' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                V_UINT32 u32SwitchChannelMode = 0;
                V_CHAR *res = NULL;
	            res = strtok(pszCmd, " ");
                if(res != NULL)
                    res = strtok(NULL, " ");
                if(res == NULL)
                {
                    ICS_LOGW("the format of switch_channel_mode  is wrong:no switch_channel_mode");
                    break;
                }
                u32SwitchChannelMode = atoi(res);
                if(u32SwitchChannelMode == 0)
                {
                    g_spobjPlayer->SetChannelChangeMode(E_SME_PLAYER_SWITCH_CHANNEL_MODE_BLACK);
                }
                else if(u32SwitchChannelMode == 1)
                {
                    g_spobjPlayer->SetChannelChangeMode(E_SME_PLAYER_SWITCH_CHANNEL_MODE_LAST);
                }
                else
                {
                    ICS_LOGE("input para is invaild,switch channel mode only support black and static now");
                }
                break;
            }

            break;
        }
        case 'c':
        {
            if('c' == pszCmd[1] && (strlen(pszCmd) > 3))
            {
                //reset
                u64ResetTime = VOS_GetTimeTicks64();
                ICS_LOGI("wh-add demo reset start time=%llu", u64ResetTime);
                u32Err = g_spobjPlayer->Reset();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("Reset failed(%u)", u32Err);
                }
                g_su64ResetCost = VOS_GetTimeTicks64() - u64ResetTime;
                STD_OUT("Reset cost %llums\n", g_su64ResetCost);

                g_sbOnErr = ICS_FALSE;
                g_sbOnPlaybackCompleted = ICS_FALSE;
                g_sbOnPrepared = ICS_FALSE;
                u64StartTime = VOS_GetTimeTicks64();

                //set sme playe loglevel
                SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);

                if(g_spszUri)
                {
                    free(g_spszUri);
                    g_spszUri = NULL;
                }
                g_spszUri = strdup(&pszCmd[3]);
                STD_OUT("new channel uri %s\n", g_spszUri);

                u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set SetDataSource failed(%u)", u32Err);
                }
                g_spobjPlayer->SetBufferTime(g_u32BufferTime);
                g_spobjPlayer->SetBufferWaterMark(g_u32BufferHighPercent, g_u32BufferLowPercent);

                u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                        DISPLAY_WIDTH, DISPLAY_HEIGHT);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
                }

                g_spobjPlayer->RegBitrateChangedCb(BitrateChangedCb,g_spobjPlayer);

                u32Err = g_spobjPlayer->PrepareAsync();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
                }

                if (g_bEnableSetBitrate)
                    g_spobjPlayer->RegManifestCompleteCb(StreamStartModeCb,NULL);

                while(ICS_FALSE == g_sbOnPrepared)
                {
                    VOS_MSleep(20);
                    if((ICS_FALSE != g_sbOnErr) || (ICS_FALSE != g_sbQuitPlay))
                    {
                        return 0;
                    }
                }

                u32Err = g_spobjPlayer->Start();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
                }
                g_su64StartCost = VOS_GetTimeTicks64() - u64StartTime;
                STD_OUT("start cost:%llums\n", g_su64StartCost);
                STD_OUT("cc cost:%llums\n", g_su64StartCost + g_su64ResetCost);
            }
            break;
        }
        case 'p':
#if defined __LINUX__
            if(g_spobjDrawCurPosTimer)
            {
                g_spobjDrawCurPosTimer->Pause();
            }
#endif

            u32Err = g_spobjPlayer->Pause();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("Pause failed(%u)", u32Err);
#if defined __LINUX__
                if(g_spobjDrawCurPosTimer)
                {
                    g_spobjDrawCurPosTimer->Start();
                }
#endif
            }
            break;
        case 'r':
            u32Err = g_spobjPlayer->Resume();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("Resume failed(%u)", u32Err);
            }
#if defined __LINUX__
            else
            {
                g_spobjDrawCurPosTimer->Start();
            }
#endif
            break;
        case 'g':
        {
            switch(pszCmd[1])
            {
                case '1':
                    ICS_LOGI("duration=%dms", g_spobjPlayer->GetDuration());
                    STD_OUT("duration=%dms\n", g_spobjPlayer->GetDuration());STD_OUT("\n");
                    break;
                case '2':
                    ICS_LOGI("isPlaying=%d", g_spobjPlayer->IsPlaying());
                    STD_OUT("isPlaying=%d\n", g_spobjPlayer->IsPlaying());STD_OUT("\n");

                    break;
                case '3':
                    ICS_LOGI("CurPosition=%dms", g_spobjPlayer->GetCurrentPosition());
                    STD_OUT("CurPosition=%dms\n", g_spobjPlayer->GetCurrentPosition());STD_OUT("\n");
                    break;
                case '4':
                    ICS_LOGI("SMEPlayer:start cost:%lldms", g_su64StartCost);
                    STD_OUT("SMEPlayer:start cost:%lldms\n", g_su64StartCost);
                    break;
                case '5':
                {
                    SmeProperty objReqAvg(SmeProperty::ConstStringKey::m_spszKeyFps);
                    SmeProperty objRespAvg(SmeProperty::ConstStringKey::m_spszKeyFps);
                    objReqAvg.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    objRespAvg.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    g_spobjPlayer->GetProperty(objReqAvg, objRespAvg);

                    SmeProperty objReq(SmeProperty::ConstStringKey::m_spszKeyCurrFps);
                    SmeProperty objResp(SmeProperty::ConstStringKey::m_spszKeyCurrFps);
                    objReq.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    objResp.SetValType(SmeProperty::E_VAL_TYPE_STRING);
                    g_spobjPlayer->GetProperty(objReq, objResp);

                    STD_OUT("SMEPlayer:CurFps:%s,AvgFps:%s\n", objResp.GetStringVal(),
                        objRespAvg.GetStringVal());
                    break;
                }
                case '6':
                {
                    g_bStreamInfoDisplaySwtich = !g_bStreamInfoDisplaySwtich;
                    BitrateChangedCb(g_spobjPlayer,0,0);
                    break;
                }
                case '7':
                {
                    E_SME_PLAYER_STATE  eState;
                    eState =  g_spobjPlayer->GetPlayerState();
                    STD_OUT("%s\n", SmePlayerStateName[eState]);
                    break;
                }
                case '8':
                {
                    if (g_spszGetAudioHdmiMode != NULL)
                    {
                        STD_OUT("The last get hdmi mode %s\n",
                                g_spszGetAudioHdmiMode);
                        ICS_LOGI("The last get hdmi mode %s",
                                 g_spszGetAudioHdmiMode);
                        free(g_spszGetAudioHdmiMode);
                        g_spszGetAudioHdmiMode = NULL;
                    }

                    g_spobjPlayer->GetAudioHdmiMode(&g_spszGetAudioHdmiMode);
                    STD_OUT("The current get hdmi mode %s\n",
                            g_spszGetAudioHdmiMode);
                    ICS_LOGI("The current get hdmi mode %s",
                             g_spszGetAudioHdmiMode);

                    break;
                }
                case '9':
                {
                    ST_BUFFER_STATUS stBufferStatus;
                    VOS_Memset(&stBufferStatus, 0, sizeof(ST_BUFFER_STATUS));
                    g_spobjPlayer->GetBufferStatus(&stBufferStatus);
                    STD_OUT(" Buffer max time:%dms,"
                             "Low percent:%d, High percent:%d \n",
                            stBufferStatus.u32MaxBufferTime,
                            stBufferStatus.u32LowPercent,
                            stBufferStatus.u32HighPercent);
                    STD_OUT(" Video Buffer size:%d, time:%lldms, last_pts:%lld\n",
                            stBufferStatus.stVBufferStatus.u32BufferSize,
                            stBufferStatus.stVBufferStatus.u64BufferPlayingTime/1000000,
                            stBufferStatus.stVBufferStatus.u64LastBufferPTS);
                    STD_OUT(" Audio Buffer size:%d, time:%lldms, last_pts:%lld\n",
                            stBufferStatus.stABufferStatus.u32BufferSize,
                            stBufferStatus.stABufferStatus.u64BufferPlayingTime/1000000,
                            stBufferStatus.stABufferStatus.u64LastBufferPTS);
                    break;
                }
                case 'a':
                {
                    ST_AUDIO_TRACK_INFO stAudTrcInfo;
                    ST_SINGLE_AUDIO_TRACK_INFO *pSingleAudTrcInfo;
                    V_UINT32 u32TrackIdx = 0;

                    VOS_Memset(&stAudTrcInfo,
                               0,
                               sizeof(ST_AUDIO_TRACK_INFO));
                    g_spobjPlayer->GetAudioTrackInfo(&stAudTrcInfo);
                    STD_OUT("Audio Track Num %d\n",
                            stAudTrcInfo.u32TrackNum +
                            stAudTrcInfo.u32UnSupportedTrackNum);
                    for (u32TrackIdx = 0;
                         u32TrackIdx < stAudTrcInfo.u32TrackNum;
                         u32TrackIdx++)
                    {
                        pSingleAudTrcInfo =
                            &stAudTrcInfo.stSingleAudioTrackInfo[u32TrackIdx];

                        STD_OUT("%saudio-track-id=%d, audio-type=%s, "
                                "audio-bitrate=%d, audio-sample-rate=%d, "
                                "audio-channels=%d, audio-lang=%s \n",
                                stAudTrcInfo.u32CurrentTrackId == u32TrackIdx ?
                                "*" : " ",
                                pSingleAudTrcInfo->u32TrackId,
                                pSingleAudTrcInfo->cAudioType,
                                pSingleAudTrcInfo->u32Bitrate,
                                pSingleAudTrcInfo->u32SampleRate,
                                pSingleAudTrcInfo->u32ChannelNum,
                                pSingleAudTrcInfo->cLang);
                    }

                    for (u32TrackIdx = 0;
                         u32TrackIdx < stAudTrcInfo.u32UnSupportedTrackNum;
                         u32TrackIdx++)
                    {
                        pSingleAudTrcInfo =
                            &stAudTrcInfo.stUnSprtSingleAudioTrackInfo[u32TrackIdx];

                        STD_OUT("#audio-track-id=%d, audio-type=%s, "
                                "audio-bitrate=%d, audio-sample-rate=%d, "
                                "audio-channels=%d, audio-lang=%s \n",
                                pSingleAudTrcInfo->u32TrackId,
                                pSingleAudTrcInfo->cAudioType,
                                pSingleAudTrcInfo->u32Bitrate,
                                pSingleAudTrcInfo->u32SampleRate,
                                pSingleAudTrcInfo->u32ChannelNum,
                                pSingleAudTrcInfo->cLang);
                    }
                    break;
                }
                case 'm':   // Get media info
                {
                    ST_VIDEO_TRACK_INFO stVideoTrcInfo;
                    ST_AUDIO_TRACK_INFO stAudTrcInfo;
                    ST_SINGLE_AUDIO_TRACK_INFO *pSingleAudTrcInfo;
                    V_UINT32 u32TrackIdx = 0;

                    VOS_Memset(&stVideoTrcInfo,
                               0,
                               sizeof(ST_VIDEO_TRACK_INFO));
                    VOS_Memset(&stAudTrcInfo,
                               0,
                               sizeof(ST_AUDIO_TRACK_INFO));

                    // Get video info
                    g_spobjPlayer->GetVideoTrackInfo(&stVideoTrcInfo);
                    STD_OUT("\nVideo info: \n");
                    STD_OUT("%s video-track-id=%d, video-type=%s, "
                                //"media-bitrate=%d, "
                                "width=%d, height=%d \n",
                                stVideoTrcInfo.bIsSupported ? "*" : "#",
                                stVideoTrcInfo.u32TrackId,
                                stVideoTrcInfo.cVideoType,
                                //stVideoTrcInfo.u32Bps,
                                //FIXME: not accurate, don't show it now.
                                //stVideoTrcInfo.u32Bps_media,
                                stVideoTrcInfo.u32Width,
                                stVideoTrcInfo.u32Height);

                    // Get audio info
                    g_spobjPlayer->GetAudioTrackInfo(&stAudTrcInfo);
                    STD_OUT("Audio info: Track Num %d\n",
                            stAudTrcInfo.u32TrackNum +
                            stAudTrcInfo.u32UnSupportedTrackNum);
                    for (u32TrackIdx = 0;
                         u32TrackIdx < stAudTrcInfo.u32TrackNum;
                         u32TrackIdx++)
                    {
                        pSingleAudTrcInfo =
                            &stAudTrcInfo.stSingleAudioTrackInfo[u32TrackIdx];

                        STD_OUT("%s audio-track-id=%d, audio-type=%s, "
                                "audio-bitrate=%d, audio-sample-rate=%d, "
                                "audio-channels=%d, audio-lang=%s \n",
                                stAudTrcInfo.u32CurrentTrackId == u32TrackIdx ?
                                "*" : " ",
                                pSingleAudTrcInfo->u32TrackId,
                                pSingleAudTrcInfo->cAudioType,
                                pSingleAudTrcInfo->u32Bitrate,
                                pSingleAudTrcInfo->u32SampleRate,
                                pSingleAudTrcInfo->u32ChannelNum,
                                pSingleAudTrcInfo->cLang);
                    }

                    for (u32TrackIdx = 0;
                         u32TrackIdx < stAudTrcInfo.u32UnSupportedTrackNum;
                         u32TrackIdx++)
                    {
                        pSingleAudTrcInfo =
                            &stAudTrcInfo.stUnSprtSingleAudioTrackInfo[u32TrackIdx];

                        STD_OUT("#audio-track-id=%d, audio-type=%s, "
                                "audio-bitrate=%d, audio-sample-rate=%d, "
                                "audio-channels=%d, audio-lang=%s \n",
                                pSingleAudTrcInfo->u32TrackId,
                                pSingleAudTrcInfo->cAudioType,
                                pSingleAudTrcInfo->u32Bitrate,
                                pSingleAudTrcInfo->u32SampleRate,
                                pSingleAudTrcInfo->u32ChannelNum,
                                pSingleAudTrcInfo->cLang);
                    }
                    break;
                }
                case 'b':
                {
                    V_UINT32 u32Error = E_SME_PLAYER_ERROR_NONE;
                    E_SME_PLAYER_DISPLAY_MODE emode = E_SME_PLAYER_DIS_MODE_FULL;
                     E_SME_PLAYER_DISPLAY_MODE& rmode = emode;
                    ST_DISP_RATIO st_disp_ratio;
                    VOS_Memset(&st_disp_ratio,
                               0,
                               sizeof(ST_DISP_RATIO));
                    ST_DISP_RATIO& rst_disp_ratio = st_disp_ratio;

                    u32Error = g_spobjPlayer->GetVideoDispMode(rmode, rst_disp_ratio);
                    if(u32Error != E_SME_PLAYER_ERROR_NONE)
                    {
                        ICS_LOGE("get display mode  failed, error:%u", u32Error);
                        break;
                    }
                    STD_OUT("video display mode : %u\n", (V_UINT32)emode);
                    STD_OUT("video display ratio:[%u %u]\n",
                            rst_disp_ratio.u32DispRatioW,
                            rst_disp_ratio.u32DispRatioH);
                    break;
                }
                case 'c':
                {

                    ST_SME_DISP_RECT st_win_rect;
                    VOS_Memset(&st_win_rect,
                               0,
                               sizeof(ST_SME_DISP_RECT));
                    ST_SME_DISP_RECT& rst_win_rect = st_win_rect;
                    V_UINT32 u32Error = E_SME_PLAYER_ERROR_NONE;

                    u32Error = g_spobjPlayer->GetVideoDispRect(rst_win_rect);
                    if(u32Error != E_SME_PLAYER_ERROR_NONE)
                    {
                        ICS_LOGE("get display rect  failed, error:%u", u32Error);
                        break;
                    }
                    STD_OUT("video display rect : [%d %d %d %d]\n",
                        rst_win_rect.i32StartX,
                        rst_win_rect.i32StartY,
                        rst_win_rect.i32Width,
                        rst_win_rect.i32Height);
                    break;

                }
                default:
                    ICS_LOGW("Unkown g%c XX", pszCmd[1]);
                    break;
            }
            break;
        }
        default:
            ICS_LOGW("Unkown %c XX", pszCmd[0]);
            break;
    }

    return 0;
}

#define DELAY_TIME(x)\
for(V_UINT32 j = 0 ; j < x; j++)\
{\
    VOS_Sleep(1);\
    if(g_sbQuitPlay)\
    {\
        goto EXIT;\
    }\
}

#define SEEK_BACKWARD(x)\
    i32CurPos = g_spobjPlayer->GetCurrentPosition();\
    g_spobjPlayer->SeekTo(i32CurPos + x);

#define SEEK_FORWARD(x)\
    i32CurPos = g_spobjPlayer->GetCurrentPosition();\
    g_spobjPlayer->SeekTo(i32CurPos - x);


static V_UINT32 TestRunner_MemLeak_1(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;
    V_INT32 i32CurPos = 0;

    ICS_LOGI("TestRunner_MemLeak_1 in");

#ifdef __USE_ANDROID_SURFACE__
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

    for(V_UINT32 i = 0 ; /*(i < g_u32TestCount) &&*/ (!g_sbQuitPlay); i++)
    {
        g_sbOnErr = ICS_FALSE;
        g_sbOnPlaybackCompleted = ICS_FALSE;
        g_sbOnPrepared = ICS_FALSE;

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetDataSource failed(%u)", u32Err);
            goto EXIT;
        }

        if(!g_pvSurfaceHdl)
        {
            u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                DISPLAY_WIDTH, DISPLAY_HEIGHT);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
                goto EXIT;
            }
        }
#ifdef 	__USE_ANDROID_SURFACE__
        else
        {
            u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                g_pvSurfaceHdl));
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetSurface failed(%u)", u32Err);
                goto EXIT;
            }
        }
#endif
        u32Err = g_spobjPlayer->PrepareAsync();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
            goto EXIT;
        }

        while(!g_sbOnPrepared)
        {
            VOS_MSleep(10);
            if(ICS_FALSE != g_sbQuitPlay)
            {
                goto EXIT;
            }
            if(g_sbOnErr)
            {
                break;
            }
        }

        ICS_LOGD("duration:%dms", g_spobjPlayer->GetDuration());

        if(!g_sbOnErr)
        {
            u32Err = g_spobjPlayer->Start();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("start failed(%u)", u32Err);
                goto EXIT;
            }
        }

        //delay 20s
        DELAY_TIME(20);

        //seek backward 10s.
        SEEK_BACKWARD(10000);

        //delay 20s
        DELAY_TIME(20);

        //seek backward 10s.
        SEEK_BACKWARD(10000);

        //delay 20s
        DELAY_TIME(20);

        //seek backward 10s.
        SEEK_BACKWARD(10000);

        //delay 5s
        DELAY_TIME(5);

        g_spobjPlayer->Pause();

        //delay 10s
        DELAY_TIME(10);
        g_spobjPlayer->Resume();

        //delay 5s
        DELAY_TIME(5);

        SEEK_FORWARD(10000);
        VOS_MSleep(10);

        SEEK_FORWARD(10000);
        VOS_MSleep(10);

        SEEK_FORWARD(10000);
        VOS_MSleep(10);

         //delay 5s
        DELAY_TIME(5);
        g_spobjPlayer->Pause();

        //delay 10s
        DELAY_TIME(10);
        //seek backward 10s.
        SEEK_BACKWARD(10000);
        //delay 10s
        DELAY_TIME(10);
        SEEK_FORWARD(10000);

        //delay 10s
        DELAY_TIME(10);
        SEEK_FORWARD(10000);
        g_spobjPlayer->Resume();

        //delay 5s
        DELAY_TIME(5);

        u32Err = g_spobjPlayer->Reset();

        //delay 5s
        DELAY_TIME(5);
    }
EXIT:
    u32Err = g_spobjPlayer->Reset();

    STD_OUT("destroy now"); STD_OUT("\n");
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif
    ICS_LOGI("TestRunner_MemLeak_1 out!!!");
    STD_OUT("TestRunner_MemLeak_1 out!!!"); STD_OUT("\n");

    return 0;
}

static V_UINT32 TestRunner_MemLeak_forbtv(INOUT V_VOID* pvCtx)
{
        V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;

        ICS_LOGI("TestRunner_MemLeak_forbtv in");

#ifdef __USE_ANDROID_SURFACE__
        g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
            DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if(NULL == g_pvSurfaceHdl)
        {
            ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
            goto EXIT;
        }
#endif


        for(V_UINT32 i = 0 ; /*(i < g_u32TestCount) &&*/ (!g_sbQuitPlay); i++)
        {

            g_spobjPlayerObs = new SmePlayerObsTest();
            g_spobjPlayer = ISmePlayer::CreateSmePlayer();
            g_sbOnErr = ICS_FALSE;
            g_sbOnPlaybackCompleted = ICS_FALSE;
            g_sbOnPrepared = ICS_FALSE;

            u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetObs failed(%u)", u32Err);
                goto EXIT;
            }

            u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetDataSource failed(%u)", u32Err);
                goto EXIT;
            }

            if(!g_pvSurfaceHdl)
            {
                u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                    DISPLAY_WIDTH, DISPLAY_HEIGHT);
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
                    goto EXIT;
                }
            }
#ifdef __USE_ANDROID_SURFACE__
            else
            {
                u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                    g_pvSurfaceHdl));
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("set SetSurface failed(%u)", u32Err);
                    goto EXIT;
                }
            }
#endif
            u32Err = g_spobjPlayer->PrepareAsync();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
                goto EXIT;
            }

            while(!g_sbOnPrepared)
            {
                VOS_MSleep(10);
                if(ICS_FALSE != g_sbQuitPlay)
                {
                    goto EXIT;
                }
                if(g_sbOnErr)
                {
                    break;
                }
            }

            ICS_LOGD("duration:%dms", g_spobjPlayer->GetDuration());

            if(!g_sbOnErr)
            {
                u32Err = g_spobjPlayer->Start();
                if(E_SME_PLAYER_ERROR_NONE != u32Err)
                {
                    ICS_LOGE("start failed(%u)", u32Err);
                    goto EXIT;
                }
            }

            //delay 30s
            DELAY_TIME(30);
            DELAY_TIME(30);
            u32Err = g_spobjPlayer->Reset();
            STD_OUT("destroy now"); STD_OUT("\n");
            ISmePlayer::DestroySmePlayer(g_spobjPlayer);
            g_spobjPlayer = NULL;
            UTILS_MSAFEDEL(g_spobjPlayerObs);
        }
    EXIT:
        if(g_spobjPlayer)
        {
            u32Err = g_spobjPlayer->Reset();

            STD_OUT("destroy now"); STD_OUT("\n");
            ISmePlayer::DestroySmePlayer(g_spobjPlayer);
            g_spobjPlayer = NULL;
            UTILS_MSAFEDEL(g_spobjPlayerObs);
        }
#ifdef __USE_ANDROID_SURFACE__
        SME_DestroyNativeSurface(g_pvSurfaceHdl);
        g_pvSurfaceHdl = NULL;
#endif
        ICS_LOGI("TestRunner_MemLeak_1 out!!!");
        STD_OUT("TestRunner_MemLeak_1 out!!!"); STD_OUT("\n");

        return 0;

}

static V_UINT32 TestRunner_Seek(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;
    V_INT32 i32Count = 0, i32Duration = 0, i32SeekCount = 0;
    V_INT32 i32SeekTo = 0;

    ICS_LOGI("TestRunner_Seek in");

#ifdef __USE_ANDROID_SURFACE__
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

    //for(V_UINT32 i = 0 ; (!g_sbQuitPlay); i++)
    {
        g_sbOnErr = ICS_FALSE;
        g_sbOnPlaybackCompleted = ICS_FALSE;
        g_sbOnPrepared = ICS_FALSE;

        u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetObs failed(%u)", u32Err);
            goto EXIT;
        }

        u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetDataSource failed(%u)", u32Err);
            goto EXIT;
        }

        if(!g_pvSurfaceHdl)
        {
            u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                DISPLAY_WIDTH, DISPLAY_HEIGHT);
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
                goto EXIT;
            }
        }
#ifdef 	__USE_ANDROID_SURFACE__
        else
        {
            u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                g_pvSurfaceHdl));
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("set SetSurface failed(%u)", u32Err);
                goto EXIT;
            }
        }
#endif
        u32Err = g_spobjPlayer->PrepareAsync();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
            goto EXIT;
        }

        while(!g_sbOnPrepared)
        {
            VOS_MSleep(10);
            if(ICS_FALSE != g_sbQuitPlay)
            {
                goto EXIT;
            }
            if(g_sbOnErr)
            {
                break;
            }
        }

        i32Duration = g_spobjPlayer->GetDuration();

        ICS_LOGW("duration:%dms", i32Duration);
        STD_OUT("duration:%dms", i32Duration);

        if(!g_sbOnErr)
        {
            u32Err = g_spobjPlayer->Start();
            if(E_SME_PLAYER_ERROR_NONE != u32Err)
            {
                ICS_LOGE("start failed(%u)", u32Err);
                goto EXIT;
            }
        }

#define ONE_SEEK_TIME 10000

        i32SeekCount = i32Duration / ONE_SEEK_TIME;


        for (i32Count = 0;
             (i32Count < g_u32TestCount)/* (i32Count < i32SeekCount)*/
                 && (!g_sbQuitPlay);
             i32Count++)
        {
            //delay 1s
            V_INT32 i32MinDuration = 15000;
            srand((V_INT32)time(NULL));
            if(i32Duration > i32MinDuration)
            {
                i32SeekTo =  rand() % (i32Duration - i32MinDuration);
            }
            else
            {
               STD_OUT("content duration must longer than:%dms", i32MinDuration);
               ICS_LOGW("content duration must longer than:%dms", i32MinDuration);
               break;
            }
            // i32CurPos = g_spobjPlayer->GetCurrentPosition();
            //g_spobjPlayer->SeekTo(i32Count * ONE_SEEK_TIME);
            g_spobjPlayer->SeekTo(i32SeekTo);

            ICS_LOGW("=========seek count:%d,seek to :%dms",i32Count, i32SeekTo);
            STD_OUT("==========seek count :%d, seek to :%dms", i32Count,i32SeekTo); STD_OUT("\n");
            DELAY_TIME(1);
            save_mem_info();
        }

        u32Err = g_spobjPlayer->Reset();

        //delay 1s
        DELAY_TIME(1);
    }
EXIT:
    u32Err = g_spobjPlayer->Reset();

    STD_OUT("destroy now"); STD_OUT("\n");
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif
    ICS_LOGI("TestRunner_Seek out!!!");
    STD_OUT("TestRunner_Seek out!!!"); STD_OUT("\n");

    return 0;
}

static V_UINT32 TestRunner_PauseResume(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;
    V_INT32 i32CurPos = 0;
    V_INT32 i32Count = 0,  i32Duration = 0;

    STD_OUT("TestRunner_PauseResume in!!!"); STD_OUT("\n");
    ICS_LOGI("TestRunner_PauseResume in");

#ifdef __USE_ANDROID_SURFACE__
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

    g_sbOnErr = ICS_FALSE;
    g_sbOnPlaybackCompleted = ICS_FALSE;
    g_sbOnPrepared = ICS_FALSE;

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetDataSource failed(%u)", u32Err);
        goto EXIT;
    }

    if(!g_pvSurfaceHdl)
    {
        u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                                             DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
            goto EXIT;
        }
    }
#ifdef 	__USE_ANDROID_SURFACE__
    else
    {
        u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                                               g_pvSurfaceHdl));
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetSurface failed(%u)", u32Err);
            goto EXIT;
        }
    }
#endif
    u32Err = g_spobjPlayer->PrepareAsync();
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    while(!g_sbOnPrepared)
    {
        VOS_MSleep(10);
        if(ICS_FALSE != g_sbQuitPlay)
        {
            goto EXIT;
        }
        if(g_sbOnErr)
        {
            break;
        }
    }

    i32Duration = g_spobjPlayer->GetDuration();

    ICS_LOGW("duration:%dms", i32Duration);
    STD_OUT("duration:%dms", i32Duration); STD_OUT("\n");

    if(!g_sbOnErr)
    {
        u32Err = g_spobjPlayer->Start();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("start failed(%u)", u32Err);
            goto EXIT;
        }
    }

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
         i32Count++)
    {
        //delay 2s
        g_spobjPlayer->Pause();
        VOS_MSleep(500);
        g_spobjPlayer->Resume();
        ICS_LOGW("========Pause count:%d",i32Count);
        STD_OUT("========Pause count:%d",i32Count); STD_OUT("\n");
        DELAY_TIME(1);
        save_mem_info();
        if (g_spobjPlayer->GetCurrentPosition() + 5000 > i32Duration)
        {
            g_spobjPlayer->SeekTo(0);
        }
    }

    u32Err = g_spobjPlayer->Reset();

    //delay 1s
    DELAY_TIME(1);

EXIT:
    u32Err = g_spobjPlayer->Reset();

    STD_OUT("destroy now"); STD_OUT("\n");
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif
    ICS_LOGI("TestRunner_PauseResume out!!!");
    STD_OUT("TestRunner_PauseResume!!!"); STD_OUT("\n");

    return 0;
}

static V_UINT32 TestRunner_StartStop(INOUT V_VOID* pvCtx)
{
    V_UINT32 u32Err = E_SME_PLAYER_ERROR_NONE;
    V_INT32 i32CurPos = 0;
    V_INT32 i32Count = 0,  i32Duration = 0;

    STD_OUT("TestRunner_StartStop in!!!"); STD_OUT("\n");
    ICS_LOGI("TestRunner_StartStop in");

#ifdef __USE_ANDROID_SURFACE__
    g_pvSurfaceHdl = SME_CreateNativeSurface(DISPLAY_STARTX, DISPLAY_STARTY,
        DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(NULL == g_pvSurfaceHdl)
    {
        ICS_LOGE("set SME_CreateNativeSurface failed(%u)", u32Err);
        goto EXIT;
    }
#endif

    g_spobjPlayerObs = new SmePlayerObsTest();
    g_spobjPlayer = ISmePlayer::CreateSmePlayer();

    g_sbOnErr = ICS_FALSE;
    g_sbOnPlaybackCompleted = ICS_FALSE;
    g_sbOnPrepared = ICS_FALSE;

    u32Err = g_spobjPlayer->SetObs(g_spobjPlayerObs);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetObs failed(%u)", u32Err);
        goto EXIT;
    }

    u32Err = g_spobjPlayer->SetDataSource(g_spszUri);
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set SetDataSource failed(%u)", u32Err);
        goto EXIT;
    }

    if(!g_pvSurfaceHdl)
    {
        u32Err = g_spobjPlayer->SetVDispRect(DISPLAY_STARTX, DISPLAY_STARTY,
                                             DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetVDispRect failed(%u)", u32Err);
            goto EXIT;
        }
    }
#ifdef 	__USE_ANDROID_SURFACE__
    else
    {
        u32Err = g_spobjPlayer->SetSurface((const V_VOID *)SME_GetNativeSurfaceObj(
                                               g_pvSurfaceHdl));
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("set SetSurface failed(%u)", u32Err);
            goto EXIT;
        }
    }
#endif
    u32Err = g_spobjPlayer->PrepareAsync();
    if(E_SME_PLAYER_ERROR_NONE != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    while(!g_sbOnPrepared)
    {
        VOS_MSleep(10);
        if(ICS_FALSE != g_sbQuitPlay)
        {
            goto EXIT;
        }
        if(g_sbOnErr)
        {
            break;
        }
    }

    i32Duration = g_spobjPlayer->GetDuration();

    ICS_LOGW("duration:%dms", i32Duration);
    STD_OUT("duration:%dms", i32Duration); STD_OUT("\n");

    if(!g_sbOnErr)
    {
        u32Err = g_spobjPlayer->Start();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("start failed(%u)", u32Err);
            goto EXIT;
        }
    }

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
         i32Count++)
    {
        //delay 2s
        u32Err = g_spobjPlayer->Stop();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("stop failed(%u)", u32Err);
            STD_OUT("stop failed(%u)", u32Err);STD_OUT("\n");
        }
        VOS_MSleep(500);
        u32Err = g_spobjPlayer->Start();
        if(E_SME_PLAYER_ERROR_NONE != u32Err)
        {
            ICS_LOGE("start failed(%u)", u32Err);
            STD_OUT("start failed(%u)", u32Err);STD_OUT("\n");
        }
        ICS_LOGW("========Start count:%d",i32Count);
        STD_OUT("========Start count:%d",i32Count); STD_OUT("\n");
        DELAY_TIME(1);
        save_mem_info();

    }

    //delay 1s
    DELAY_TIME(1);

EXIT:
    u32Err = g_spobjPlayer->Reset();

    STD_OUT("destroy now"); STD_OUT("\n");
    ISmePlayer::DestroySmePlayer(g_spobjPlayer);
    g_spobjPlayer = NULL;
    UTILS_MSAFEDEL(g_spobjPlayerObs);
#ifdef __USE_ANDROID_SURFACE__
    SME_DestroyNativeSurface(g_pvSurfaceHdl);
    g_pvSurfaceHdl = NULL;
#endif
    ICS_LOGI("TestRunner_StartStop out!!!");
    STD_OUT("TestRunner_StartStop!!!"); STD_OUT("\n");

    return 0;
}


V_VOID StartSmePlayerOfNormal()
{
    V_CHAR acPlayOpCmd[256] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner, NULL);
    UTILS_MLOGE_RET_IF(NULL == g_shdlRunner, ("Create Thd failed"));
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q'  && acPlayOpCmd[0] != 'Q')
    {
        //COMMAND_LOCK;
        VOS_ThdMutexLock(&g_mutex);
        PrintPlayerOpHelp();
        memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
        fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);

        /* replace the last \n to \0 */
        acPlayOpCmd[strlen(acPlayOpCmd) - 1] = 0;

        VOS_ThdMutexUnLock(&g_mutex);
        //COMMAND_UNLOCK;
        //scanf("%s", acPlayOpCmd);
        VOS_ThdMutexLock(&g_sstMutex);
        if(NULL != g_spobjPlayer)
        {
            V_UINT32 u32Err = TestPlayerOp(acPlayOpCmd);
            if(E_SME_PLAYER_ERROR_NONE == u32Err)
            {
                VOS_ThdMutexUnLock(&g_sstMutex);
                continue;
            }
        }
        VOS_ThdMutexUnLock(&g_sstMutex);
        ICS_LOGW("player is destroyed!");
        break;
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    return;
}

V_VOID StartSmePlayerOfMuiltResolution()
{
    V_CHAR acPlayOpCmd[256] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner, NULL);
    UTILS_MLOGE_RET_IF(NULL == g_shdlRunner, ("Create Thd failed"));
    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q'  && acPlayOpCmd[0] != 'Q')
    {
        //COMMAND_LOCK;
        VOS_ThdMutexLock(&g_mutex);
        PrintPlayerOpHelp();
        memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
        fgets((char *)(acPlayOpCmd), (sizeof(acPlayOpCmd) - 1), stdin);
        VOS_ThdMutexUnLock(&g_mutex);
        //COMMAND_UNLOCK;
        //scanf("%s", acPlayOpCmd);
        VOS_ThdMutexLock(&g_sstMutex);
        if(NULL != g_spobjPlayer)
        {
            V_UINT32 u32Err = TestMuiltResolution(acPlayOpCmd);
            if(E_SME_PLAYER_ERROR_NONE == u32Err)
            {
                VOS_ThdMutexUnLock(&g_sstMutex);
                continue;
            }
        }
        VOS_ThdMutexUnLock(&g_sstMutex);
        ICS_LOGW("player is destroyed!");
        break;
    }
    g_sbQuitPlay = ICS_TRUE;
    VOS_JoinThread(g_shdlRunner);
    g_shdlRunner = NULL;
    return;
}

V_VOID StartSmePlayerOfMemLeak_Normal()
{
    V_CHAR acPlayOpCmd[256] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_MemLeak_1, NULL);

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

V_VOID StartSmePlayerOfMemLeak_btvlive()
{
    V_CHAR acPlayOpCmd[256] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_MemLeak_forbtv, NULL);

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

V_VOID StartSmePlayerOfStartRest_Test()
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
    VOS_CreateThread(&g_shdlRunner, NULL, StartResetTestRunner, NULL);

    memset(acPlayOpCmd, 0, sizeof(acPlayOpCmd));
    while(acPlayOpCmd[0] != 'q' && acPlayOpCmd[0] != 'Q')
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


V_VOID StartSmePlayerOfSeek()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_Seek, NULL);

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


V_VOID StartSmePlayerOfPauseResume()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_PauseResume, NULL);

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

V_VOID StartSmePlayerOfStartStop()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    g_sbQuitPlay = ICS_FALSE;
    STD_OUT("pls input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    VOS_CreateThread(&g_shdlRunner, NULL, TestRunner_StartStop, NULL);

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


static V_VOID PrintHelpOfArg()
{
    STD_OUT("Options are below:\n");
    STD_OUT("--uri [val];uri to play;Mandatory;\n"
        "           egg:--uri /test.mp4\n");
    STD_OUT("--disp-mode [val];display resolution;Optional;\n"
        "           egg:--disp-mode 1080P_60;\n"
        "           May be:1080P_60|1080P_50|2160P_30;default:1080P_60\n");
    STD_OUT("--log-level [val];loglevel;Optional;\n"
        "           egg:--log-level L;\n"
        "           May be:L|V|D|I|W|E;Optional;default:W\n");
    STD_OUT("--select-play;need select to play;Optional;\n"
        "           egg:--select-play;default:not\n");
    STD_OUT("--suburi [val];external subtitle uri to play;Optional;\n"
        "           egg:--suburi /test.srt\n");
    STD_OUT("--subrect;subtitle display rect;Optional;\n"
        "           egg:--subrect x-y-w-h;default:896-900-128-64\n");
    STD_OUT("--enable-setbt;user will be asked to select bt before play file;Optional;\n"
        "           egg:--enable-setbt;default:not\n");
    STD_OUT("--audio-hdmi-mode [val];set hdmi mode;\n"
        "           egg:--audio-hdmi-mode lpcm\n"
        "           May be:lpcm|auto\n");
    STD_OUT("--show-fragment;fragment download information will be displayed;\n"
        "           egg:--show-fragment;default:not\n");
    STD_OUT("--buffer-time;set max buffer time,unit:ms;Optional\n"
        "           egg:--buffer-time 5000;default:5000ms\n");
    STD_OUT("--low-percent;set low water mark of buffer;Optional;\n"
        "           egg:--low-percent 1;\n"
        "           May be:0-100,should smaller than high percent; default:1\n");
    STD_OUT("--high-percent;set high water mark of buffer;Optional;\n"
        "           egg:--high-percent 99;\n"
        "           May be:0-100; should larger than low percent default:1\n");
    STD_OUT("--set-useragent;user will be asked to set user-agent before play file;Optional;\n"
        "           egg:--set-useragent user-agent;default:NULL\n");
    STD_OUT("--set-byterange;user will be asked to set byte-range before play file;Optional;\n"
        "           egg:--set-useragent 1;default:1\n");
    return;
}

static V_VOID parseSubRect(ST_SME_DISP_RECT& stRect, V_CHAR* pszRect)
{
    V_CHAR* pszTmp = strdup(pszRect);
    V_CHAR* pszStart = pszTmp;

    do
    {
        if(!pszTmp)break;
        stRect.i32StartX = atoi(pszStart);
        ICS_LOGW("subrect x=%d\n", stRect.i32StartX);

        pszStart=strchr(pszTmp, INTERVAL_CHARICTER);
        if(!pszStart || !*pszStart)break;
        pszStart++;
        stRect.i32StartY = atoi(pszStart);
        ICS_LOGW("subrect y=%d\n", stRect.i32StartY);

        pszStart=strchr(pszStart, INTERVAL_CHARICTER);
        if(!pszStart || !*pszStart)break;
        pszStart++;
        stRect.i32Width = atoi(pszStart);
        ICS_LOGW("subrect w=%d\n", stRect.i32Width);

        pszStart=strchr(pszStart, INTERVAL_CHARICTER);
        if(!pszStart || !*pszStart)break;
        pszStart++;
        stRect.i32Height = atoi(pszStart);
        ICS_LOGW("subrect h=%d\n", stRect.i32Height);
    } while ( ICS_FALSE );

    if(pszTmp)
    {
        free(pszTmp);
    }

    return;
}


#include <getopt.h>

V_INT32 main(V_INT32 i32Argc, V_CHAR* ppszArgv[])
{
    V_BOOL bExit = ICS_FALSE;
    V_CHAR acInputType[128] = {0,};
    V_BOOL bSelectToPlay = ICS_FALSE;
    V_INT32 i32OptRes = 0;
    static struct option stLongOptions[] = {
        {"uri", required_argument, NULL, 'u'},
        {"disp-mode", required_argument, NULL, 'm'},
        {"log-level", required_argument, NULL, 'l'},
        {"select-play", no_argument, NULL, 's'},
        {"suburi", required_argument, NULL, 't'},
        {"subrect", required_argument, NULL, 'r'},
        {"enable-setbt", no_argument, NULL, 'e'},
        {"audio-hdmi-mode", required_argument, NULL, 'h'},
        {"show-fragment", no_argument, NULL, 'f'},
        {"buffer-time", required_argument, NULL, 'b'},
        {"low-percent", required_argument, NULL, 'o'},
        {"high-percent", required_argument, NULL, 'i'},
        {"set-useragent", required_argument, NULL, 'a'},
        {"set-byterange", required_argument, NULL, 'n'},
        {"set-cookies", no_argument, NULL, 'c'},
        {"set-referer", required_argument, NULL, 'g'},
        {0, 0, 0, 0}
    };
    V_CHAR* pszDispMode = NULL;
    g_spszUri = NULL;
    if(i32Argc < 2)
    {
        PrintHelpOfArg();
        return -1;
    }

    if (ICS_SUCCESS != SmeInit(E_SME_PLAYER_LOG_LEVEL_INFO))
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
            case 'm':
                if(NULL != optarg)
                {
                    if(pszDispMode)
                    {
                        free(pszDispMode);
                        pszDispMode = NULL;
                    }
                    pszDispMode = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no disp-mode? use default\n",
                        ppszArgv[optind - 1]);
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
            case 't':
                if(NULL != optarg)
                {
                    if(g_spszSubUri)
                    {
                        free(g_spszSubUri);
                        g_spszSubUri = NULL;
                    }
                    g_spszSubUri = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no suburl?\n", ppszArgv[optind - 1]);
                }
                break;
            case 'r':
                if(NULL != optarg)
                {
                    parseSubRect(g_sstSubDispRect, optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no subtitle rect? use default\n", ppszArgv[optind - 1]);
                }
                break;
            case 'e':
                g_bEnableSetBitrate = ICS_TRUE;
                break;
            case 'h':
                if(NULL != optarg)
                {
                    if(g_spszAudioHdmiMode)
                    {
                        free(g_spszAudioHdmiMode);
                        g_spszAudioHdmiMode = NULL;
                    }
                    g_spszAudioHdmiMode = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no url?\n", ppszArgv[optind - 1]);
                }
                break;
            case 'f':
                g_bShowFragmentInfo = ICS_TRUE;
                break;
            case 'b':
                 if(NULL != optarg)
                {
                    g_u32BufferTime = atoi(optarg);
                    STD_OUT("Set buffer max time to %d s\n",g_u32BufferTime);
                }
                else
                {
                    STD_OUT("%s:val is NULL? use default 5 s\n",
                        ppszArgv[optind - 1]);
                }

                break;
            case 'o':
                 if(NULL != optarg)
                {
                    g_u32BufferLowPercent = atoi(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL? use default 1 s\n",
                        ppszArgv[optind - 1]);
                }

                break;
            case 'i':
                 if(NULL != optarg)
                {
                    g_u32BufferHighPercent = atoi(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL? use default 99 s\n",
                        ppszArgv[optind - 1]);
                }
                break;
             case 'a':
                if(NULL != optarg)
                {
                    if(g_bSetUserAgent)
                    {
                        free(g_bSetUserAgent);
                        g_bSetUserAgent = NULL;
                    }
                    g_bSetUserAgent = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no useragent?\n", ppszArgv[optind - 1]);
                }
                break;
             case 'g':
                if(NULL != optarg)
                {
                    if(g_bSetReferer)
                    {
                        free(g_bSetReferer);
                        g_bSetReferer = NULL;
                    }
                    g_bSetReferer = strdup(optarg);
                }
                else
                {
                    STD_OUT("%s:val is NULL?no referer?\n", ppszArgv[optind - 1]);
                }
                break;
             case 'n':
                if (atoi(optarg))
                  g_bSetByteRange = ICS_TRUE;
                else {
                  g_bSetByteRange = ICS_FALSE;
                  STD_OUT("WRANING:byterange val is false and button will close, which will lead to \n"
                      "        part of HTTP resources can't be played. \n");
                }
                break;
             case 'c':
                g_bEnableSetCookies = ICS_TRUE;
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

    STD_OUT("LogLevel=%c\n", g_scLogLevel);
    ICS_LOGI("LogLevel=%c", g_scLogLevel);

    STD_OUT("uri=%s\n", g_spszUri);
    ICS_LOGD("uri=%s", g_spszUri);
    STD_OUT("display resolution=%s\n", pszDispMode);
    ICS_LOGD("display resolution=%s", pszDispMode);
    STD_OUT("ICS_LOG_VERBOSE=%d,ICS_LOG_DEBUG=%d\n", ICS_LOG_VERBOSE, ICS_LOG_DEBUG);
    SetICSLogLevel(g_scLogLevel);

#if defined __LINUX__
    TestHDMIInit(pszDispMode);
#endif

#ifdef __ANDROID__
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();
#endif

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
                StartSmePlayerOfNormal();
                if(!bSelectToPlay)
                {
                    STD_OUT("play completed,uri=%s\n", g_spszUri);
                    goto EXIT;
                }
                break;
            }
            case '3':
            {
                StartSmePlayerOfMemLeak_Normal();
                break;
            }
            case '4':
            {
                StartSmePlayerOfMemLeak_btvlive();
                break;
            }
            case '5':
            {
                StartSmePlayerOfStartRest_Test();
                break;
            }
            case '6':
            {
                StartSmePlayerOfSeek();
                break;
            }
            case '7':
            {
                ICS_LOGW("Muilt solution begins");
                g_scInputType = '1';
                StartSmePlayerOfMuiltResolution();
                if(!bSelectToPlay)
                {
                    STD_OUT("play completed,uri=%s\n", g_spszUri);
                    goto EXIT;
                }
                break;
            }
            case '8':
            {
                StartSmePlayerOfPauseResume();
                break;
            }
            case '9':
            {
                StartSmePlayerOfStartStop();
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
#if defined __LINUX__
    TestHDMIDeinit();
#endif

#ifdef __ANDROID__
    IPCThreadState::self()->stopProcess();
#endif


    if(g_spszUri)
    {
        free(g_spszUri);
        g_spszUri = NULL;
    }

    if (g_spszAudioHdmiMode)
    {
        free(g_spszAudioHdmiMode);
        g_spszAudioHdmiMode = NULL;
    }

    if (g_spszGetAudioHdmiMode)
    {
        free(g_spszGetAudioHdmiMode);
        g_spszGetAudioHdmiMode = NULL;
    }

    if(pszDispMode)
    {
       free(pszDispMode);
       pszDispMode = NULL;
    }

    if (g_bSetUserAgent)
    {
        free(g_bSetUserAgent);
        g_bSetUserAgent = NULL;
    }

    if (g_bSetReferer)
    {
        free(g_bSetReferer);
        g_bSetReferer = NULL;
    }

#if defined __LINUX__
    if (g_spsSetCookies)
    {
#endif
        for (V_INT32 i = 0; i < 256; i++)
        {
            if (g_spsSetCookies [i])
            {
                free (g_spsSetCookies[i]);
                g_spsSetCookies[i] = NULL;
            }
        }
#if defined __LINUX__
    }
#endif

    SmeDeInit();
    return 0;
}
