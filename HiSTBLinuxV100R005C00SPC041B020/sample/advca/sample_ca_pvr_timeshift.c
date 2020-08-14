/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_pvr_timeshift.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "sample_ca_adp_pvr.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"
#define MAX_TIMESHIFT_REC_FILE_SIZE       (2000*1024*1024LLU)

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_TunerQam;
static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
HI_U32 u32cycle = 0;

PMT_COMPACT_TBL   *g_pProgTbl = HI_NULL;
HI_U32 g_PlayChn = 0xffffffff;


HI_HANDLE               g_hTsBufForPvrPlayBack;

HI_S32 DmxInitAndSearch(HI_VOID)
{
    HI_S32   Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_DVB);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort failed.\n");
        (HI_VOID)HI_UNF_DMX_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_REC, PVR_DMX_PORT_ID_DVB);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        (HI_VOID)HI_UNF_DMX_DeInit();
        return Ret;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0,&g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    (HI_VOID)HIADP_Search_FreeAllPmt(g_pProgTbl);

    Ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    Ret |= HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_DetachTSPort failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_DeInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}


HI_S32 SwitchToShiftPlay(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn,
                            HI_HANDLE hAvplay, HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo)
{
    (HI_VOID)PVR_StopLivePlay(hAvplay);

    (HI_VOID)PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK);
    (HI_VOID)PVR_StartPlayBack(pszFileName, pu32PlayChn, hAvplay, stPvrPlayInfo);

    return HI_SUCCESS;
}


HI_S32 SwitchToLivePlay(HI_U32 u32PlayChn, HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    PVR_StopPlayBack(u32PlayChn);

    (HI_VOID)PVR_SwitchDmxSource(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_DVB);
    (HI_VOID)PVR_StartLivePlay(hAvplay, pProgInfo);
    return HI_SUCCESS;
}


typedef struct 
{
    HI_U32                  RecChn;
    HI_CHAR *pargv;
    PMT_COMPACT_PROG        *pstCurrentProgInfo;
}CX_ADVCA_PVR_S;
HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    /*CX_ADVCA_PVR_S stCAPvr = {0};*/
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_CHAR                 InputCmd[32];
    HI_U32                  u32ProgNum;
    HI_U32                  RecChn;
    HI_HANDLE               hSoundTrack;

    HI_HANDLE               hAvplay;
    HI_UNF_PVR_REC_ATTR_S 		RecAttr;
    PMT_COMPACT_PROG            *pstCurrentProgInfo;
    HI_BOOL                bLive = HI_TRUE;
    HI_BOOL                bPause = HI_FALSE;
    HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo;

    if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_TunerQam   = strtol(argv[4],NULL,0);
        g_enDefaultFmt = (HI_UNF_ENC_FMT_E)HIADP_Disp_StrToFmt(argv[5]);
    }
    else if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_TunerQam   = strtol(argv[4],NULL,0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if(4 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_TunerQam   = 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if(3 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = 6875;
        g_TunerQam   = 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else
    {
        printf("Usage: %s file_path freq [srate] [qamtype] [vo_format]\n"
               "       qamtype:16|32|[64]|128|256|512\n"
               "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n", argv[0]);
        return HI_FAILURE;
    }


    (HI_VOID)HI_SYS_Init();
#ifndef __HI3716MV310__
    HIADP_MCE_Exit();
#endif

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }
    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Tuner_Connect(TUNER_USE,g_TunerFreq,g_TunerSrate,g_TunerQam);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Tuner_Connect failed.\n");
        return Ret;
    }

    Ret = DmxInitAndSearch();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Snd_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Disp_DeInit failed.\n");
        return Ret;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_VO_Init failed.\n");
        (HI_VOID)HIADP_VO_DeInit();
        return Ret;
    }

    Ret = PVR_AvplayInit(hWin, &hAvplay, &hSoundTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }

    Ret = HI_UNF_PVR_RecInit();
    Ret |= HI_UNF_PVR_PlayInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_PVR_RecInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    printf("please input the number of program to timeshift:\n");
    u32ProgNum = 0;

    while (0 == u32ProgNum)
    {
        memset(InputCmd, 0, sizeof(32));
        if (!fgets(InputCmd, 30, stdin))
        {
            sleep(1);
            continue;
        }
        
        if ('q' == InputCmd[0])
        {
            return 0;
        }
        //HI_DEBUG_ADVCA("get:%s\n", InputCmd);
        u32ProgNum = atoi(InputCmd);
    }

    pstCurrentProgInfo = g_pProgTbl->proginfo
                         + ((u32ProgNum-1)% g_pProgTbl->prog_num);
    (HI_VOID)PVR_RecStart(argv[1], pstCurrentProgInfo,PVR_DMX_ID_REC, HI_TRUE,HI_FALSE, MAX_TIMESHIFT_REC_FILE_SIZE, &RecChn,u32cycle);
    u32cycle++;
#if 0
    stCAPvr.pargv = argv[1];
    stCAPvr.pstCurrentProgInfo = pstCurrentProgInfo;
    stCAPvr.RecChn = RecChn;
#endif
    bLive = HI_TRUE;
    bPause = HI_FALSE;
    Ret = PVR_StartLivePlay(hAvplay, pstCurrentProgInfo);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call SwitchProg failed.\n");
        return Ret;
    }

    while(1)
    {
        printf("please input the q to quit!\n");
        if (!fgets(InputCmd, 30, stdin))
        {
            sleep(1);
            continue;
        }

        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("prepare to exit!\n");
            break;
        }
        if ('a' == InputCmd[0])
        {
            (HI_VOID)PVR_RecStop(RecChn);
            RecChn = 0;
            (HI_VOID)PVR_RecStart(argv[1], pstCurrentProgInfo,PVR_DMX_ID_REC, HI_TRUE,HI_FALSE, MAX_TIMESHIFT_REC_FILE_SIZE, &RecChn,u32cycle);
            u32cycle++;
            //pstCAPvr->RecChn = RecChn;
            //g_bIsRecStop = HI_TRUE;
            bLive = HI_FALSE;
            bPause = HI_FALSE;
        }
        else if ('l' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("live play now.\n");
            if (!bLive)
            {
                (HI_VOID)SwitchToLivePlay(g_PlayChn, hAvplay, pstCurrentProgInfo);
                bLive   = HI_TRUE;
                bPause  = HI_FALSE;
                g_PlayChn = 0xffffffff;
            }
        }
        else if ('o' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("only stop,but still being on playback.\n");
            (HI_VOID)PVR_RecStop(RecChn);
        }
        else if ('p' == InputCmd[0])
        {
            if (bPause)  /* when pause ,to shift play */
            {
                if (bLive)  /*when live, switch to shift play */
                {
					HI_DEBUG_ADVCA("switch to timeshift...\n");
                    HI_UNF_PVR_RecGetChn(RecChn,&RecAttr);

                    stPvrPlayInfo.bIsTimeshiftPlay = HI_TRUE;
                    stPvrPlayInfo.u32RecChnID = RecChn;
                    SwitchToShiftPlay(RecAttr.szFileName, &g_PlayChn, hAvplay, stPvrPlayInfo);
                    bLive = HI_FALSE;
                }
                else  /* when shift, just to resume*/
                {
                    HI_DEBUG_ADVCA("PVR resume now.\n");
                    Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                        return Ret;
                    }
                }
                bPause = HI_FALSE;
            }
            else  /* to pause */
            {
                if (bLive)  /*when live, stop live still last picture */
                {
                    PVR_StopLivePlay(hAvplay);

                    Ret = HI_UNF_PVR_PlayPauseChn(RecChn);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                        return Ret;
                    }
                }
                else  /* when shift, just to pause*/
                {
                    HI_DEBUG_ADVCA("PVR playback pause now.\n");
                    Ret = HI_UNF_PVR_PlayPauseChn(g_PlayChn);
                    if (Ret != HI_SUCCESS)
                    {
                        HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                        return Ret;
                    }
                }
                bPause = HI_TRUE;
            }
        }
        else if ('f' == InputCmd[0])
        {
            if (bLive)  /*when live, can not fast forword */
            {
                HI_DEBUG_ADVCA("now live play, not support fast forword\n");

            }
            else
            {
                HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_4X_FAST_FORWARD;
                HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
                Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
                    return Ret;
                }
                bPause = HI_FALSE;
            }
        }
        else if ('r' == InputCmd[0])
        {
            if (bLive)  /*when live, switch to shift and fast reword play */
            {
                HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                /*now, stop live play*/
                PVR_StopLivePlay(hAvplay);
                Ret = HI_UNF_PVR_PlayPauseChn(RecChn);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                    return Ret;
                }

                /*rewind from pause position */
                HI_UNF_PVR_RecGetChn(RecChn,&RecAttr);
                stPvrPlayInfo.bIsTimeshiftPlay = HI_TRUE;
                stPvrPlayInfo.u32RecChnID = RecChn;
                SwitchToShiftPlay(RecAttr.szFileName, &g_PlayChn, hAvplay, stPvrPlayInfo);
                bLive = HI_FALSE;
                stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_2X_FAST_BACKWARD;  
                HI_DEBUG_ADVCA("shift play now, trick mod:%d\n", stTrickMode.enSpeed);
                Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
                    return Ret;
                }
                bPause = HI_FALSE;
            }
            else
            {
                HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_4X_FAST_BACKWARD;
                HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
                Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
                    return Ret;
                }
                bPause = HI_FALSE;
            }
        }
        else if ('n' == InputCmd[0])
        {
            
            HI_DEBUG_ADVCA("PVR normal play now.\n");
            if (g_PlayChn == 0xffffffff)
            {
                (HI_VOID)HI_UNF_PVR_RecGetChn(RecChn,&RecAttr);
                HI_DEBUG_ADVCA("switch to timeshift:%s\n", RecAttr.szFileName);
                stPvrPlayInfo.bIsTimeshiftPlay = HI_TRUE;
                stPvrPlayInfo.u32RecChnID = RecChn;
                (HI_VOID)SwitchToShiftPlay(RecAttr.szFileName, &g_PlayChn, hAvplay, stPvrPlayInfo);
                bLive = HI_FALSE;
            }
            else
            {
                Ret = HI_UNF_PVR_PlayResumeChn(g_PlayChn);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayResumeChn failed.\n");
                    return Ret;
                }
            }
            bLive = HI_FALSE;
            bPause = HI_FALSE;
        }
        else if ('s' == InputCmd[0])
        {
            if (bLive)  /*when live, can not slow forword */
            {
                HI_DEBUG_ADVCA("now live play, not support slow forward\n");

            }
            else
            {
                HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_4X_SLOW_FORWARD;
                HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
                Ret = HI_UNF_PVR_PlayTPlay(g_PlayChn, &stTrickMode);
                if (Ret != HI_SUCCESS)
                {
                    HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
                    return Ret;
                }
                bPause = HI_FALSE;
            }
        }
        else
        {
            printf("commond:\n");
            printf("    l: live play\n");
            printf("    p: pause/play\n");
            printf("    f: fast foreword(4x)\n");
            printf("    r: fast rewind(4x)\n");
            printf("    n: normal play\n");
            printf("    s: slow play(4x)\n");
            printf("    o: only stop recording, but paly is still on playback\n");
            printf("    h: help\n");
            printf("    a:  start a new recording again\n");
            printf("    q: quit\n");
            continue;
        }
    }

    (HI_VOID)PVR_RecStop(RecChn);

    (HI_VOID)PVR_StopPlayBack(g_PlayChn);

    (HI_VOID)PVR_AvplayDeInit(hAvplay, hWin, hSoundTrack);

    (HI_VOID)HI_UNF_VO_DestroyWindow(hWin);
    (HI_VOID)HIADP_VO_DeInit();
    (HI_VOID)HIADP_Disp_DeInit();
    (HI_VOID)HIADP_Snd_DeInit();
    (HI_VOID)HI_UNF_ADVCA_DeInit();

    (HI_VOID)HI_SYS_DeInit();

    return 0;
}


