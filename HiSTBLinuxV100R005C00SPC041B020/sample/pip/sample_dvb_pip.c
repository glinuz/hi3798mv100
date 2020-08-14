/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_dvb_pip.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : w58735
    Modification: Created file
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#define AVPLAY_NUM  2
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#define PLAY_DEMUX_ID 0
#define PLAY_TUNER_ID 0

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                      Ret,i;
    HI_HANDLE                   hWin[AVPLAY_NUM];
    HI_HANDLE                   hAvplay[AVPLAY_NUM];
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_CHAR                     InputCmd[32];
    HI_UNF_ENC_FMT_E            VidFormat = HI_UNF_ENC_FMT_1080P_50;
    HI_U32                      ProgNum;
    HI_RECT_S                   stChildRect;
    HI_U32                      TunerFreq;
    HI_U32                      TunerSrate;
    HI_U32                      ThirdParam;
    HI_UNF_AVPLAY_STOP_OPT_S    stStopOpt;
    HI_BOOL                     bAudMix = 0;
    HI_BOOL                     bAudPlay[AVPLAY_NUM];
    HI_HANDLE                   hTrack[AVPLAY_NUM];
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
    HI_UNF_SND_GAIN_ATTR_S      stGain;
    HI_U32 u32TSPID = INVALID_TSPID;

    stGain.bLinearMode = HI_TRUE;

    if (argc < 2)
    {
        printf("Usage:\n%s freq [srate] [qamtype or polarization] [audiomix] [vo_format]\n", argv[0]);
        printf("\tqamtype or polarization:\n");
        printf("\t\tFor cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64]\n");
        printf("\t\tFor satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0]\n");
        printf("\taudiomix\t: 0: not mix audio, 1: mix audio\n");
        printf("\tvo_format\t: 2160P_60 2160P_50 2160P_30 2160P_25 2160P_24 1080P_60 1080P_50 1080i_60 1080i_50 720P_60 720P_50\n\n");
        printf("Example:%s 610 6875 64 0\n", argv[0]);
        return HI_FAILURE;
    }

    TunerFreq   = strtol(argv[1], NULL, 0);
    TunerSrate  = (TunerFreq > 1000) ? 27500 : 6875;
    ThirdParam  = (TunerFreq > 1000) ? 0 : 64;

    if (argc > 2)
    {
        TunerSrate = strtol(argv[2], NULL, 0);
    }

    if (argc > 3)
    {
        ThirdParam = strtol(argv[3], NULL, 0);
    }

    if (argc > 4)
    {
        bAudMix = strtol(argv[4], NULL, 0);
    }

    if (argc > 5)
    {
        VidFormat = HIADP_Disp_StrToFmt(argv[5]);
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(VidFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL,&hWin[0]);
    stChildRect.s32Width = 720;
    stChildRect.s32Height = 576;
    stChildRect.s32X = 0;
    stChildRect.s32Y = 0;
    Ret |= HIADP_VO_CreatWin(&stChildRect,&hWin[1]);
    if (HI_SUCCESS != Ret)
    {
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_Tuner_Connect(PLAY_TUNER_ID, TunerFreq, TunerSrate, ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DEMUX_ID, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        printf("===============avplay[%d]===============\n",i);

        HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        AvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
        Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay[i]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY_STOP;
        }

        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_STOP;
        }

        Ret = HI_UNF_VO_AttachWindow(hWin[i], hAvplay[i]);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_VO_AttachWindow failed.\n");
            goto AVPLAY_STOP;
        }
        Ret = HI_UNF_VO_SetWindowEnable(hWin[i], HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_VO_SetWindowEnable failed.\n");
            goto AVPLAY_STOP;
        }

        bAudPlay[i] = HI_FALSE;
        if (0 == i)
        {
            bAudPlay[i] = HI_TRUE;

            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            }
            Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack[i]);
            Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            Ret |= HI_UNF_SND_Attach(hTrack[i], hAvplay[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_SND_Attach failed.\n");
                goto AVPLAY_STOP;
            }
        }
        else if(1 == i && HI_TRUE == bAudMix)
        {
            bAudPlay[i] = HI_TRUE;

            stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_SLAVE;
            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            }
            Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack[i]);
            stGain.s32Gain = 60;
            Ret = HI_UNF_SND_SetTrackWeight(hTrack[i],&stGain);
            Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            Ret |= HI_UNF_SND_Attach( hTrack[i], hAvplay[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_SND_Attach failed.\n");
                goto AVPLAY_STOP;
            }
        }

        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto AVPLAY_STOP;
        }

        ProgNum = 0;
        Ret = HIADP_AVPlay_PlayProg(hAvplay[i], g_pProgTbl, ProgNum + i,bAudPlay[i]);
        if (HI_SUCCESS != Ret)
        {
            printf("call HIADP_AVPlay_SwitchProg failed\n");
        }
    }

    while(1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        ProgNum = atoi(InputCmd);

        for ( i = 0 ; i < AVPLAY_NUM; i++ )
        {
            stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
            stStopOpt.u32TimeoutMs = 0;
            Ret = HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
            if (HI_SUCCESS != Ret)
            {
                printf("call HI_UNF_AVPLAY_Stop failed.\n");
                break;
            }

            (HI_VOID)HI_UNF_AVPLAY_SetAttr(hAvplay[i],HI_UNF_AVPLAY_ATTR_ID_PCR_PID,&u32TSPID);
            (HI_VOID)HI_UNF_AVPLAY_SetAttr(hAvplay[i],HI_UNF_AVPLAY_ATTR_ID_VID_PID,&u32TSPID);

             if (HI_TRUE == bAudPlay[i])
             {
                (HI_VOID)HI_UNF_AVPLAY_SetAttr(hAvplay[i],HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&u32TSPID);
             }
        }

        for ( i = 0 ; i < AVPLAY_NUM; i++ )
        {
            Ret = HIADP_AVPlay_PlayProg(hAvplay[i],g_pProgTbl,ProgNum + i,bAudPlay[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call SwitchProgfailed!\n");
                break;
            }
        }
    }

AVPLAY_STOP:
    for(i = 0 ; i < AVPLAY_NUM;i++)
    {
        stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopOpt.u32TimeoutMs = 0;
        if (HI_FALSE == bAudMix && 1 == i)
        {
            HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stStopOpt);
        }
        else
        {
            HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
            HI_UNF_SND_Detach(hTrack[i], hAvplay[i]);
            HI_UNF_SND_DestroyTrack(hTrack[i]);
        }
        HI_UNF_VO_SetWindowEnable(hWin[i],HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin[i], hAvplay[i]);
        if (HI_FALSE == bAudMix && 1 == i)
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        }
        else
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        }
        HI_UNF_AVPLAY_Destroy(hAvplay[i]);
    }
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin[1]);
    HI_UNF_VO_DestroyWindow(hWin[0]);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}

