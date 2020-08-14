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

#include "osal_thread.h"
#include "osal_type.h"
#include "sme_log.h"
#include "sme_macro.h"
#include "osal_mem.h"
#include "osal_str.h"
#include "sme_timer.h"
#include "sme_media_type.h"
#include "sme_init.h"


#include "DVBPlayer.h"
#ifdef __ANDROID__
#include "IMediaHTTPService.h"
#endif

//class IMediaHTTPService;
#define STD_OUT printf

#define DISPLAY_STARTX 0
#define DISPLAY_STARTY 0
#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720


static V_BOOL g_sbOnErr = ICS_FALSE;
static V_BOOL g_sbOnPrepared = ICS_FALSE;

static VOSTHDHDL g_shdlRunner = NULL;
static V_CHAR g_scInputType = 0;
static V_BOOL g_sbQuitPlay = ICS_FALSE;

static V_CHAR* g_spszUri = NULL;

static V_UINT32 g_u32TestCount = 1;
static V_UINT32 g_u32SleepInterval = 5;//ms
static V_CHAR   g_scLogLevel = 'W';//


static ST_VOS_THD_MUTEX     g_mutex = VOS_PTHREAD_MUTEX_INITIALIZER;
#define COMMAND_LOCK  VOS_ThdMutexLock(&g_mutex);
#define COMMAND_UNLOCK VOS_ThdMutexUnLock (&g_mutex);

using namespace android;
static DVBPlayer* g_spobjPlayer = NULL;

#if 0
const V_CHAR *g_DvbChannelList[] =
{
  "dvb://888.8.1",
  "dvb://888.8.2"
};
#endif

const V_CHAR *g_DvbChannelList[] =
{
  "dvb://888.3.12d",
  "dvb://888.3.12e",
  //"dvb://888.3.303",
  //"dvb://888.3.304",
  //"dvb://888.3.305",
  //"dvb://888.3.306",
};


static V_UINT32 g_curChannelIndex = 0;
static V_UINT32 g_TotalChannelNum = 0;


#define DELAY_TIME(x)\
for(V_UINT32 j = 0 ; j < x; j++)\
{\
    VOS_Sleep(1);\
    if(g_sbQuitPlay)\
    {\
        goto EXIT;\
    }\
}



V_VOID PrintHelpOfTestCase()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:1 for normal dvbplayer test(use prepareAsync)");STD_OUT("\n");
    STD_OUT("Input:2 for normal dvbplayer test(use prepare)");STD_OUT("\n");
    STD_OUT("Input:3 for MemLeak Switch channel");STD_OUT("\n");
    STD_OUT("Input:4 for StartResetTest for dvb player");STD_OUT("\n");
    STD_OUT("Input:5 for StartExitTest for dvb player");STD_OUT("\n");
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}

V_VOID PrintPlayerOpHelp()
{
    STD_OUT("*****************************************************************");STD_OUT("\n");
    STD_OUT("Input:sc for switch channel");STD_OUT("\n");
    STD_OUT("Input:q for quit");STD_OUT("\n");
    STD_OUT("*****************************************************************");STD_OUT("\n");

    return;
}

static void
save_mem_info()
{
    V_UINT32 pid = getpid();
    V_CHAR cmd[256];
    static int print_flag = 1;
#ifndef __ANDROID__
    sprintf(cmd, "cat /proc/%d/status |grep VmRSS >> /data/mem_stat_%d.dat",pid,pid);
#else
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
#ifdef __ANDROID__
    sp<IMediaHTTPService> tmp = 0;
#endif

    STD_OUT("url:%s\n", url);
    u32Err = g_spobjPlayer->stop();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("StopChannel failed(%u)", u32Err);
        STD_OUT("StopChannel failed(%u)\n", u32Err);
        if(g_spobjPlayer->reset()){
            ICS_LOGE("reset return error in libmedia\n");
            goto error;
        }
    }
#ifdef __ANDROID__
    u32Err = g_spobjPlayer->setDataSource(tmp, url, NULL);
#else
    u32Err = g_spobjPlayer->setDataSource(url);
#endif
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("SetDataSource failed(%u)", u32Err);
      STD_OUT("SetDataSource failed(%u)\n", u32Err);
      goto error;
    }

    u32Err = g_spobjPlayer->prepare();
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("prepare failed(%u)", u32Err);
      STD_OUT("prepare failed(%u) \n",u32Err);
      goto error;
    }

    u32Err = g_spobjPlayer->start();
    if(ICS_SUCCESS != u32Err)
    {
      ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
      goto error;
    }

 error:
    return u32Err;
}


static V_UINT32 ForwardChannel()
{
    V_UINT32 u32Err = ICS_FAIL;

    g_TotalChannelNum = sizeof(g_DvbChannelList) / sizeof(g_DvbChannelList[0]);
    //STD_OUT("g_TotalChannelNum :%d\n",g_TotalChannelNum);
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
#ifdef __ANDROID__
    sp<IMediaHTTPService> tmp = 0;
#endif
    ICS_LOGI("TestRunnerNormal in");
    STD_OUT("TestRunnerNormal in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = new DVBPlayer();
    if (g_spobjPlayer == NULL)
    {
        ICS_LOGE("new DVBPlayer failed(%u)", u32Err);
        STD_OUT("new DVBPlayer failed(%u)\n", u32Err);
        goto EXIT;
    }

    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    //set sme playe loglevel
    //SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);
#ifdef __ANDROID__
    u32Err = g_spobjPlayer->setDataSource(tmp,g_spszUri,NULL);
#else
    u32Err = g_spobjPlayer->setDataSource(g_spszUri);
#endif
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("SetDataSource failed(%u)", u32Err);
        STD_OUT("SetDataSource failed(%u)\n", u32Err);
        goto EXIT;
    }

    if('1' == g_scInputType)
    {
        u32Err = g_spobjPlayer->prepareAsync();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("PrepareAsync failed(%u)", u32Err);
            STD_OUT("PrepareAsync failed(%u) \n",u32Err);
            goto EXIT;
        }
#if 0
        while(ICS_FALSE == g_sbOnPrepared)
        {
            VOS_MSleep(20);
            if((ICS_FALSE != g_sbOnErr) || (ICS_FALSE != g_sbQuitPlay))
            {
                goto EXIT;
            }
        }
#endif

    }
    else if ('2' == g_scInputType)
    {
        u32Err = g_spobjPlayer->prepare();
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

    g_spobjPlayer->setClip(0,0,1280,720);

    u32Err = g_spobjPlayer->start();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("set PrepareAsync failed(%u)", u32Err);
        goto EXIT;
    }

    while((ICS_FALSE == g_sbOnErr) && (ICS_FALSE == g_sbQuitPlay))
    {
        VOS_MSleep(500);
    }

EXIT:

    STD_OUT("stop begin");
    u32Err = g_spobjPlayer->stop();
    STD_OUT("reset begin"); STD_OUT("\n");
    u32Err = g_spobjPlayer->reset();
    delete g_spobjPlayer;
    g_spobjPlayer = NULL;

    ICS_LOGI("TestRunner out!!!");

    return 0;
}


static V_UINT32 TestRunnerStartReset(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;
    sp<IMediaHTTPService> tmp = 0;

    ICS_LOGI("TestRunnerStartReset in");
    STD_OUT("TestRunnerStartReset in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = new DVBPlayer();
    if (g_spobjPlayer == NULL)
    {
        ICS_LOGE("new DVBPlayer failed(%u)", u32Err);
        STD_OUT("new DVBPlayer failed(%u)\n", u32Err);
        goto EXIT;
    }

    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    //set sme playe loglevel
    //SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);

    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
         i32Count++)
    {

        STD_OUT(" begin %d setDataSource... \n", i32Count);
        u32Err = g_spobjPlayer->setDataSource(tmp,g_spszUri,NULL);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }


        STD_OUT(" begin %d prepare... \n", i32Count);
        u32Err = g_spobjPlayer->prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            STD_OUT("prepare failed(%u) \n",u32Err);
            goto EXIT;
        }

        g_spobjPlayer->setClip(0,0,1280,720);
        STD_OUT(" begin %d start... \n", i32Count);
        u32Err = g_spobjPlayer->start();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("start failed(%u)", u32Err);
            goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);
        STD_OUT(" begin %d stop... \n", i32Count);
        u32Err = g_spobjPlayer->stop();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("StopChannel failed(%u)", u32Err);
            STD_OUT("StopChannel failed(%u)\n", u32Err);
            if(g_spobjPlayer->reset()){
                ICS_LOGE("reset return error in libmedia\n");
                goto EXIT;
            }
        }
        STD_OUT(" begin %d reset... \n",i32Count);
        u32Err = g_spobjPlayer->reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }

        ICS_LOGW("========reset count:%d",i32Count);
        STD_OUT("========reset count:%d",i32Count); STD_OUT("\n");
        save_mem_info();

    }

    //delay 1s
    DELAY_TIME(1);

EXIT:
    STD_OUT("reset begin"); STD_OUT("\n");

    delete g_spobjPlayer;
    g_spobjPlayer = NULL;
    ICS_LOGI("TestRunnerStartReset out!!!");
    STD_OUT("TestRunnerStartReset!!!"); STD_OUT("\n");
    return 0;
}


static V_UINT32 TestRunnerStartExit(INOUT V_VOID* pvCtx)
{

    V_UINT32 u32Err = ICS_FAIL;
    V_INT32 i32Count = 0;
    sp<IMediaHTTPService> tmp = 0;

    ICS_LOGI("TestRunnerStartExit in");
    STD_OUT("TestRunnerStartExit in!\n");
    g_sbOnErr = ICS_FALSE;



    for (i32Count = 0;(i32Count < g_u32TestCount)&& (!g_sbQuitPlay);
         i32Count++)
    {
        g_spobjPlayer = new DVBPlayer();
        if (g_spobjPlayer == NULL)
        {
            ICS_LOGE("new DVBPlayer failed(%u)", u32Err);
            STD_OUT("new DVBPlayer failed(%u)\n", u32Err);
            goto EXIT;
        }

        STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

        u32Err = g_spobjPlayer->setDataSource(tmp,g_spszUri,NULL);
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("SetDataSource failed(%u)", u32Err);
            STD_OUT("SetDataSource failed(%u)\n", u32Err);
            goto EXIT;
        }


       STD_OUT(" begin prepare... \n");
        u32Err = g_spobjPlayer->prepare();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("set Prepare failed(%u)", u32Err);
            STD_OUT("prepare failed(%u) \n",u32Err);
            goto EXIT;
        }

        g_spobjPlayer->setClip(0,0,1280,720);

        u32Err = g_spobjPlayer->start();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("start failed(%u)", u32Err);
            goto EXIT;
        }

        VOS_MSleep(g_u32SleepInterval);
        save_mem_info();

        u32Err = g_spobjPlayer->stop();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("StopChannel failed(%u)", u32Err);
            STD_OUT("StopChannel failed(%u)\n", u32Err);
            if(g_spobjPlayer->reset()){
                ICS_LOGE("reset return error in libmedia\n");
                goto EXIT;
            }
        }

        STD_OUT("reset begin"); STD_OUT("\n");
        u32Err = g_spobjPlayer->reset();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("reset failed(%u)", u32Err);
            goto EXIT;
        }

        ICS_LOGW("======== delete player:%d",i32Count);
        STD_OUT("======== delete player:%d\n",i32Count);
        delete g_spobjPlayer;
        g_spobjPlayer = NULL;

        ICS_LOGW("======== count:%d",i32Count);
        STD_OUT("======== count:%d \n",i32Count);

    }

    //delay 1s
    DELAY_TIME(1);

EXIT:
    ICS_LOGI("TestRunnerStartExit out!!!");
    STD_OUT("TestRunnerStartExit!!!"); STD_OUT("\n");

    return 0;
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
        case 's':
        {
            switch(pszCmd[1])
            {
                case 'c':
                {
                    SwtichChannel();
                    break;
                }
            }
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
#ifdef __ANDROID__
    sp<IMediaHTTPService> tmp = 0;
#endif

    ICS_LOGI("TestRunner_MemLeak_SwitchCC in");
    STD_OUT("TestRunner_MemLeak_SwitchCC in!\n");
    g_sbOnErr = ICS_FALSE;

    g_spobjPlayer = new DVBPlayer();
    if (g_spobjPlayer == NULL)
    {
        ICS_LOGE("new DVBPlayer failed(%u)", u32Err);
        STD_OUT("new DVBPlayer failed(%u)\n", u32Err);
        goto EXIT;
    }

    STD_OUT("CreateSmeDvbPlayer end ! g_spobjPlayer:%p\n",g_spobjPlayer);

    //set sme playe loglevel
    //SetSmePlayerLogLevel(g_spobjPlayer, g_scLogLevel);
#ifdef __ANDROID__
    u32Err = g_spobjPlayer->setDataSource(tmp,g_spszUri,NULL);
#else
    u32Err = g_spobjPlayer->setDataSource(g_spszUri);
#endif
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("SetDataSource failed(%u)", u32Err);
        STD_OUT("SetDataSource failed(%u)\n", u32Err);
        goto EXIT;
    }

    if('1' == g_scInputType)
    {
        u32Err = g_spobjPlayer->prepareAsync();
        if(ICS_SUCCESS != u32Err)
        {
            ICS_LOGE("PrepareAsync failed(%u)", u32Err);
            STD_OUT("PrepareAsync failed(%u) \n",u32Err);
            goto EXIT;
        }
    }
    else if ('2' == g_scInputType)
    {
        u32Err = g_spobjPlayer->prepare();
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

    g_spobjPlayer->setClip(0,0,1280,720);

    u32Err = g_spobjPlayer->start();
    if(ICS_SUCCESS != u32Err)
    {
        ICS_LOGE("start failed(%u)", u32Err);
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
        }
        VOS_MSleep(g_u32SleepInterval);

        ICS_LOGW("========switch count:%d",i32Count);
        STD_OUT("========switch count:%d",i32Count); STD_OUT("\n");
        save_mem_info();

    }

    //delay 1s
    DELAY_TIME(1);

EXIT:
    STD_OUT("reset begin"); STD_OUT("\n");
    u32Err = g_spobjPlayer->reset();
    delete g_spobjPlayer;
    g_spobjPlayer = NULL;
    ICS_LOGI("TestRunner_MemLeak_SwitchCC out!!!");
    STD_OUT("TestRunner_MemLeak_SwitchCC!!!"); STD_OUT("\n");
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

    STD_OUT("pls input testcount:");
    g_scInputType = '2';
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


V_VOID
    StartDvbPlayerOfStartReset_Test()
{
    V_CHAR acPlayOpCmd[256] = {0,};
    V_CHAR acInputArg[128] = {0,};

    memset(acInputArg, 0, sizeof(acInputArg));
    g_scInputType = '1';

    STD_OUT("please input testcount:");
    fgets((char *)(acInputArg), (sizeof(acInputArg) - 1), stdin);
    g_u32TestCount = atoi(acInputArg);
    STD_OUT("%d\n", g_u32TestCount);

    STD_OUT("please input sleepms:");
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
            acInputType[0] = '2';
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
                if(!bSelectToPlay)
                {
                    STD_OUT("test completed\n");
                    goto EXIT;
                }
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

    return 0;
}


