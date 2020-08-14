/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_pvr_play.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include <stdlib.h>

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "sample_ca_adp_pvr.h"
#include "hi_go.h"
#include "sample_ca_common.h"
#include "hi_unf_advca.h"

static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;

HI_BOOL             g_bIsPlayStop = HI_FALSE;
HI_U32              PlayChn;
//static pthread_t   g_StaThd;

/*HI_S32 GetRealFileName(HI_CHAR * pFileName)
{
    if(NULL == pFileName)
    {
            HI_DEBUG_ADVCA("file name error\n");
            return HI_FAILURE;
    }
    HI_CHAR *p;
    HI_U32 u32PathNameLen = strlen(pFileName);
    HI_U32 u32RealNameLen = 0;
    HI_CHAR ReturnName[128] = {0};
    HI_CHAR TmpName[128] = {0};
    p = strrchr(pFileName,'/');
    if(NULL != p)
    {
        if (*(p-1) == '/')
        {
            u32RealNameLen = u32PathNameLen - (p - pFileName);
            memcpy(TmpName,p+1,u32RealNameLen);
            memcpy(p,TmpName,u32RealNameLen + 1);
        }
        
    }
    return HI_SUCCESS;
}*/


HI_S32 PvrPlayDmxInit(HI_VOID)
{
    HI_S32   Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }


    return HI_SUCCESS;
}

HI_S32 PvrPlayDmxDeInit(HI_VOID)
{
    (HI_VOID)HI_UNF_DMX_DeInit();
    return HI_SUCCESS;
}

HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;
HI_RECT rc_Str = {100, 200, 100, 30};


HI_S32 OsdInit(HI_VOID)
{
    HI_S32   s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo = {0};

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    (HI_VOID)HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.LayerID =  HIGO_LAYER_HD_0;
    stLayerInfo.PixelFormat = HIGO_PF_8888;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("failed to create the layer sd 0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer_pvr, &hLayerSurface_pvr);
    if  (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    (HI_VOID)HI_GO_FillRect(hLayerSurface_pvr, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    s32Ret = HI_GO_CreateText("../higo/res/simhei.ttf", NULL,  &hFont_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("failed to create the font:./res/simhei.ttf !\n");
        return HI_FAILURE;
    }
    (HI_VOID)HI_GO_RefreshLayer(hLayer_pvr, NULL);

    return HI_SUCCESS;
}

HI_S32 DrawTimeSring(const HI_CHAR *szText)
{
    HI_S32   s32Ret;

    (HI_VOID)HI_GO_FillRect(hLayerSurface_pvr, &rc_Str, 0x80000000, HIGO_COMPOPT_NONE);
    (HI_VOID)HI_GO_DrawRect(hLayerSurface_pvr, &rc_Str, 0xffffffff);

    (HI_VOID)HI_GO_SetTextColor(hFont_pvr, 0xff00ff00);

    s32Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, &rc_Str, HIGO_LAYOUT_RIGHT);
    if (HI_SUCCESS != s32Ret)
    {
        HI_DEBUG_ADVCA("failed to text out char!\n");
        return HI_FAILURE;
    }

    (HI_VOID)HI_GO_RefreshLayer(hLayer_pvr, NULL);

    return HI_SUCCESS;
}

/*lint -save -e818*/
HI_VOID * StatuThread(HI_VOID *args)
{
#if 0
    HI_S32 Ret;
    HI_UNF_PVR_PLAY_STATUS_S stStatus;
    HI_CHAR timestr[20];
#endif

    while (HI_FALSE == g_bIsPlayStop)
    {
        usleep(100000);
#if 0
        Ret = HI_UNF_PVR_PlayGetStatus(PlayChn, &stStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(timestr, sizeof(timestr), "%d", stStatus.u32CurPlayTimeInMs/1000);
            (HI_VOID)DrawTimeSring(timestr);
        }
#endif
    }

    return NULL;
}

HI_S32 main(int argc, char* const argv[])
{
    HI_S32                  Ret;

    HI_HANDLE               hWin;
    HI_HANDLE               hSoundTrack;
    HI_HANDLE               hAvplay;
    HI_CHAR                 InputCmd[32];
    HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo;


    if (argc < 2)
    {
        printf("Usage: %s recTsFile [vo_format]\n", argv[0]);
        printf("       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n");
        return 0;
    }
    else if(3 == argc)
    {
        g_enDefaultFmt = (HI_UNF_ENC_FMT_E)HIADP_Disp_StrToFmt(argv[2]);
    }

#if 1
    HI_DEBUG_ADVCA("checking idx file of '%s'...\n", argv[1]);
    Ret = PVR_checkIdx(argv[1]);
    if (HI_SUCCESS == Ret)
    {
        HI_DEBUG_ADVCA("checking idx file done, start play...\n");
    }
    else
    {
        HI_DEBUG_ADVCA("checking idx file error, try play...\n");
    }
#endif

    (HI_VOID)HI_SYS_Init();
#ifndef __HI3716MV310__
    (HI_VOID)HIADP_MCE_Exit();
#endif

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
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
        HIADP_Snd_DeInit();
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


    Ret = PvrPlayDmxInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }

    Ret = PVR_AvplayInit(hWin, &hAvplay, &hSoundTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }


    Ret = HI_UNF_PVR_PlayInit();
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

    stPvrPlayInfo.bIsTimeshiftPlay = HI_FALSE;
    stPvrPlayInfo.u32RecChnID = 0;
    
    Ret = PVR_StartPlayBack(argv[1], &PlayChn, hAvplay, stPvrPlayInfo);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call PVR_StartPlayBack failed.\n");
        return Ret;
    }
    
    #if 0
    (HI_VOID)OsdInit();
    (HI_VOID)pthread_create(&g_StaThd, HI_NULL, StatuThread, HI_NULL);
    #endif
    
    while (HI_FALSE == g_bIsPlayStop)
    {
        static HI_U32 u32FTimes = 0;
        static HI_U32 u32STimes = 0;
        static HI_U32 u32RTimes = 0;

        printf("Press q to exit, h for help...\n");

        if (!fgets(InputCmd, 30, stdin))
        {
            sleep(1);
            continue;
        }

        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("prepare to exit!\n");
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

            u32FTimes = u32FTimes%5;
            stTrickMode.enSpeed = (HI_UNF_PVR_PLAY_SPEED_E)((0x1 << (u32FTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL);
            HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
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

            u32STimes = u32STimes%5;
            stTrickMode.enSpeed = (HI_UNF_PVR_PLAY_SPEED_E)(HI_UNF_PVR_PLAY_SPEED_NORMAL / (0x1 << (u32STimes+1)));

            HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
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

            u32RTimes = u32RTimes%5;
            stTrickMode.enSpeed = (HI_UNF_PVR_PLAY_SPEED_E)(-(0x1 << (u32RTimes+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL);
            HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
            Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
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

            HI_DEBUG_ADVCA("PVR normal play now.\n");
            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);          
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayResumeChn failed.\n");
                continue;
            }

            continue;
        }

        if ('p' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            HI_DEBUG_ADVCA("PVR pause now.\n");
            Ret = HI_UNF_PVR_PlayPauseChn(PlayChn);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                continue;
            }

            continue;
        }

        if ('t' == InputCmd[0])
        {
            u32FTimes = 0;
            u32STimes = 0;
            u32RTimes = 0;

            HI_DEBUG_ADVCA("PVR play step now.\n");
            Ret = HI_UNF_PVR_PlayStep(PlayChn, 1);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
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
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }


            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = stFAttr.u32StartTimeInMs;
            stPos.s32Whence = SEEK_SET;
            HI_DEBUG_ADVCA("seek to start\n");
            

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
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

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = 2000;
            stPos.s32Whence = SEEK_CUR;
            HI_DEBUG_ADVCA("seek forward 5 Second\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
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

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = -2000;
            stPos.s32Whence = SEEK_CUR;
            HI_DEBUG_ADVCA("seek reward 5 Second\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
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

            stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
            stPos.s64Offset = 0;
            stPos.s32Whence = SEEK_END;
            HI_DEBUG_ADVCA("seek end\n");

            Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
                continue;
            }

            continue;
        }

        if ('x' == InputCmd[0])
        {
            PVR_StopPlayBack(PlayChn);

            stPvrPlayInfo.bIsTimeshiftPlay = HI_FALSE;
            stPvrPlayInfo.u32RecChnID = 0;
            Ret = PVR_StartPlayBack(argv[1], &PlayChn, hAvplay, stPvrPlayInfo);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call PVR_StartPlayBack failed.\n");
                continue;
            }
            continue;
        }
    }
	
    PVR_StopPlayBack(PlayChn);
//    (HI_VOID)pthread_join(g_StaThd, HI_NULL);
    (HI_VOID)PVR_AvplayDeInit(hAvplay, hWin, hSoundTrack);

    PRINT_SMP(">>==DmxDeInit.\n");
    Ret = PvrPlayDmxDeInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call DmxDeInit failed.\n");
        return Ret;
    }

    (HI_VOID)HI_UNF_VO_DestroyWindow(hWin);
    (HI_VOID)HIADP_VO_DeInit();
    (HI_VOID)HIADP_Disp_DeInit();
    (HI_VOID)HIADP_Snd_DeInit();
    (HI_VOID)HI_UNF_ADVCA_DeInit();
    (HI_VOID)HI_SYS_DeInit();

    return 0;
}



