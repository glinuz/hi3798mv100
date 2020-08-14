/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pvr_play.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : q46153
    Modification: Created file

******************************************************************************/
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_pvr.h"
#include "hi_go.h"

static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;

HI_BOOL             g_bIsPlayStop = HI_FALSE;
HI_U32              PlayChn;
static pthread_t   g_StaThd;

HI_S32 PvrPlayDmxInit(HI_VOID)
{
    HI_S32   Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }


    return HI_SUCCESS;
}

HI_S32 PvrPlayDmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    SAMPLE_RUN(HI_UNF_DMX_DeInit(), Ret);
    return Ret;
}

HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;
HI_RECT rc_Str = {100, 200, 100, 30};


HI_S32 OsdInit(HI_CHAR *pu8CurrentPath)
{
#ifndef ANDROID
    HI_S32   s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_TEXT_INFO_S stTextInfo;

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.PixelFormat  = HIGO_PF_1555;
    stLayerInfo.ScreenWidth  = 1920;
    stLayerInfo.ScreenHeight = 1080;
    stLayerInfo.CanvasWidth = 1920;
    stLayerInfo.CanvasHeight = 1080;
    stLayerInfo.DisplayWidth  = 1280;
    stLayerInfo.DisplayHeight = 720;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the layer sd 0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer_pvr, &hLayerSurface_pvr);
    if  (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_FillRect(hLayerSurface_pvr, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    stTextInfo.pMbcFontFile = NULL;
	pu8CurrentPath = strncat(pu8CurrentPath, "../res/font/DroidSansFallbackLegacy.ttf", sizeof("../res/font/DroidSansFallbackLegacy.ttf"));
    stTextInfo.pSbcFontFile = pu8CurrentPath;
    stTextInfo.u32Size = 20;

    s32Ret = HI_GO_CreateTextEx(&stTextInfo,  &hFont_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the font:%s !\n", pu8CurrentPath);
        return HI_FAILURE;
    }
    HI_GO_RefreshLayer(hLayer_pvr, NULL);
#endif
    return HI_SUCCESS;
}

HI_S32 DrawTimeSring(HI_CHAR *szText)
{
#ifndef ANDROID
    HI_S32   s32Ret;

    HI_GO_FillRect(hLayerSurface_pvr, &rc_Str, 0x80000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &rc_Str, 0xffffffff);

    HI_GO_SetTextColor(hFont_pvr, 0xff00ff00);

    s32Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, &rc_Str, HIGO_LAYOUT_RIGHT);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to text out char!\n");
        return HI_FAILURE;
    }

    HI_GO_RefreshLayer(hLayer_pvr, NULL);
#else
    PVR_SAMPLE_Printf("current Time:%s\n", szText);
#endif
    return HI_SUCCESS;
}

HI_S32 DrawStatusString(HI_CHAR *szText)
{
#ifndef ANDROID
    HI_S32 Ret;
    HI_RECT Rect={100,230,100,30};

    HI_GO_FillRect(hLayerSurface_pvr, &Rect, 0x80000000, HIGO_COMPOPT_NONE);
    HI_GO_DrawRect(hLayerSurface_pvr, &Rect, 0xffffffff);

    HI_GO_SetTextColor(hFont_pvr, 0xff00ff00);

    
    Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, &Rect, HIGO_LAYOUT_RIGHT);
    if (HI_SUCCESS != Ret) 
    {
        PVR_SAMPLE_Printf("call HI_GO_TextOut faild!\n");     
    }

    Ret = HI_GO_RefreshLayer(hLayer_pvr, NULL);
    if (HI_SUCCESS != Ret) 
    {
        PVR_SAMPLE_Printf("call HI_GO_RefreshLayer faild!\n");     
    }
#else
    PVR_SAMPLE_Printf("STATUS:%s\n", szText);
#endif
    return 0;
}

HI_VOID * StatuThread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_UNF_PVR_PLAY_STATUS_S stStatus;
    HI_CHAR timestr[20];
    HI_CHAR                 StatString[32];
    while (HI_FALSE == g_bIsPlayStop)
    {
        sleep(1);

        Ret = HI_UNF_PVR_PlayGetStatus(PlayChn, &stStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(timestr, sizeof(timestr),"%d", stStatus.u32CurPlayTimeInMs/1000);
            DrawTimeSring(timestr);
            switch(stStatus.enState)
            {
                memset(StatString, 0x00, 32*sizeof(HI_CHAR));
                case HI_UNF_PVR_PLAY_STATE_INIT:
                    snprintf(StatString, sizeof(StatString),"Init");
                    break;
                case HI_UNF_PVR_PLAY_STATE_PLAY:
                    snprintf(StatString, sizeof(StatString),"Play");
                    break;
                case HI_UNF_PVR_PLAY_STATE_PAUSE:
                    snprintf(StatString, sizeof(StatString),"Pause");
                    break;
                case HI_UNF_PVR_PLAY_STATE_FF:
                    snprintf(StatString, sizeof(StatString),"FF %dX", stStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                    break;
                case HI_UNF_PVR_PLAY_STATE_FB:
                    snprintf(StatString, sizeof(StatString),"FB %dX", stStatus.enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
                    break;
                case HI_UNF_PVR_PLAY_STATE_SF:
                    snprintf(StatString, sizeof(StatString),"SF 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/stStatus.enSpeed);
                    break;
                case HI_UNF_PVR_PLAY_STATE_SB:
                    snprintf(StatString, sizeof(StatString),"SB 1/%dX", HI_UNF_PVR_PLAY_SPEED_NORMAL/stStatus.enSpeed);
                    break;
                case HI_UNF_PVR_PLAY_STATE_STEPF:
                    snprintf(StatString, sizeof(StatString),"Step F");
                    break;
                case HI_UNF_PVR_PLAY_STATE_STEPB:
                    snprintf(StatString, sizeof(StatString),"Step B");
                    break;
                case HI_UNF_PVR_PLAY_STATE_STOP:
                    snprintf(StatString, sizeof(StatString),"Stop");
                    break;
                default:
                    snprintf(StatString, sizeof(StatString),"InValid(%d)", stStatus.enState);
                    break;
            }
            DrawStatusString(StatString);
        }

    }

    return NULL;
}

HI_S32 main(int argc, char *argv[])
{
    HI_S32                  Ret;
    HI_BOOL                 bSupportTee = HI_FALSE;
    HI_HANDLE               hWin;
    HI_HANDLE               hAvplay;
    HI_HANDLE               hSoundTrack;
    HI_CHAR                 InputCmd[32];
    HI_CHAR                 aCurrentPath[256] = {0};


    if (argc < 2)
    {
        printf("Usage: %s recTsFile [vo_format]\n", argv[0]);
        printf("       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n");
        return 0;
    }
    else if(3 == argc)
    {
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[2]);
    }
    else if (4 == argc)
    {
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[2]);
        bSupportTee = strtol(argv[3],NULL,0);
    }

#if 1
    PVR_SAMPLE_Printf("checking idx file of '%s'...\n", argv[1]);
    Ret = PVR_checkIdx(argv[1]);
    if (HI_SUCCESS == Ret)
    {
        PVR_SAMPLE_Printf("checking idx file done, start play...\n");
    }
    else
    {
        PVR_SAMPLE_Printf("checking idx file error, try play...\n");
    }
#endif
    
    HI_SYS_Init();
    if (HI_FALSE != bSupportTee)
    {
        PVR_SetTvpFlag(HI_TRUE);
        PVR_SetCallbackFlag(HI_FALSE, HI_TRUE);
    }
    
    HIADP_MCE_Exit();
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Snd_Init failed.\n");
        return 0;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Disp_Init failed.\n");
        return 0;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        return 0;
    }


    Ret = PvrPlayDmxInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        return Ret;
    }

    Ret = PVR_AvplayInit(hWin, &hAvplay, &hSoundTrack);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        return Ret;
    }


    Ret = HI_UNF_PVR_PlayInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    Ret = PVR_StartPlayBack(argv[1], &PlayChn, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call PVR_StartPlayBack failed.\n");
        return Ret;
    }

    memset(aCurrentPath, 0, sizeof(aCurrentPath));
    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_play") - argv[0]));
    OsdInit(aCurrentPath);
    pthread_create(&g_StaThd, HI_NULL, StatuThread, HI_NULL);

    while (HI_FALSE == g_bIsPlayStop)
    {
        static HI_U32 u32FTimes = 0;
        static HI_U32 u32STimes = 0;
        static HI_U32 u32RTimes = 0;

        PVR_SAMPLE_Printf("Press q to exit, h for help...\n");

#ifndef ANDROID
        if (!SAMPLE_GET_INPUTCMD(InputCmd))
        {
            sleep(1);
            continue;
        }
#else
        SAMPLE_GET_INPUTCMD(InputCmd);
#endif

        if ('q' == InputCmd[0])
        {
            PVR_SAMPLE_Printf("prepare to exit!\n");
            g_bIsPlayStop = HI_TRUE;
        }

        if ('h' == InputCmd[0])
        {
            printf("commond:\n");
            printf("    n: normal play\n");
            printf("    p: pause\n");
            printf("    f: fast forward(2x/4x/8x/16x/32x)\n");
            printf("    s: slow forward(2x/4x/8x/16x/32x)\n");
            printf("    r: fast rewind(2x/4x/8x/16x/32x)\n");
            printf("    t: step forward one frame\n");
            printf("    k: seek to start\n");
            printf("    e: seek to end\n");
            printf("    l: seek forward 5 second\n");
            printf("    j: seek rewwind 5 second\n");
            printf("    x: destroy play channel and create again\n");
            printf("    h: help\n");
            printf("    q: quit\n");
            continue;
        }

        if ('f' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32STimes = 0;
            u32RTimes = 0;

            u32FTimes = u32FTimes%6;
            stTrickMode.enSpeed = (0x1 << (u32FTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;
            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                continue;
            }

            u32FTimes++;
            continue;
        }
        if ('s' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32FTimes = 0;
            u32RTimes = 0;

            u32STimes = u32STimes%6;
            stTrickMode.enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (u32STimes+1));

            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                continue;
            }

            u32STimes++;
            continue;
        }
        if ('r' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_MODE_S stTrickMode;
            u32FTimes = 0;
            u32STimes = 0;

            u32RTimes = u32RTimes%6;
            stTrickMode.enSpeed = -(0x1 << (u32RTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;
            PVR_SAMPLE_Printf("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayTPlay failed.\n");
                continue;
            }

            u32RTimes++;
            continue;
        }

        if ('n' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            PVR_SAMPLE_Printf("PVR normal play now.\n");
            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            continue;
        }

        if ('p' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            PVR_SAMPLE_Printf("PVR pause now.\n");
            Ret = HI_UNF_PVR_PlayPauseChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayPauseChn failed.\n");
                continue;
            }

            continue;
        }

        if ('t' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            PVR_SAMPLE_Printf("PVR play step now.\n");
            Ret = HI_UNF_PVR_PlayStep(PlayChn, 1);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }

            continue;
        }

        if ('k' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            HI_UNF_PVR_FILE_ATTR_S stFAttr;
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            Ret = HI_UNF_PVR_PlayGetFileAttr(PlayChn,&stFAttr);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }

            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = stFAttr.u32StartTimeInMs;
            stPos.s32Whence = SEEK_SET;
            PVR_SAMPLE_Printf("seek to start\n");


            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }
            continue;
        }

        if ('l' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = 5000;
            stPos.s32Whence = SEEK_CUR;
            PVR_SAMPLE_Printf("seek forward 5 Second\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }
            continue;
        }
        if ('j' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = -5000;
            stPos.s32Whence = SEEK_CUR;
            PVR_SAMPLE_Printf("seek reward 5 Second\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }
            continue;
        }

        if ('e' == InputCmd[0])
        {
            HI_UNF_PVR_PLAY_POSITION_S stPos;
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = 0;
            stPos.s32Whence = SEEK_END;
            PVR_SAMPLE_Printf("seek end\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }
            continue;
        }

        if ('x' == InputCmd[0])
        {
            PVR_StopPlayBack(PlayChn);

            Ret = PVR_StartPlayBack(argv[1], &PlayChn, hAvplay);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call PVR_StartPlayBack failed.\n");
                continue;
            }
            continue;
        }
    }

    PVR_StopPlayBack(PlayChn);
    if(g_StaThd != 0)
    {
        pthread_join(g_StaThd, HI_NULL);
    }
    PVR_AvplayDeInit(hAvplay, hWin, hSoundTrack);

    PRINT_SMP(">>==DmxDeInit.\n");
    Ret = PvrPlayDmxDeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call DmxDeInit failed.\n");
        return Ret;
    }

    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HIADP_Snd_DeInit();
    HI_SYS_DeInit();

    return 0;
}


