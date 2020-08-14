/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : dvb_mosaic.c
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
#include "hi_adp_search.h"
#include "hi_adp_tuner.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define PLAY_DEMUX_ID 0
#define PLAY_TUNER_ID 0

#define AVPLAY_NUM  4
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                   Ret,i;
    HI_HANDLE                hWin[AVPLAY_NUM];
    HI_HANDLE                hAvplay[AVPLAY_NUM];
    HI_UNF_AVPLAY_ATTR_S     AvplayAttr;
    HI_UNF_SYNC_ATTR_S       SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR                  InputCmd[32];
    HI_U32                   ProgNum = 0;
    HI_RECT_S                stWinRect;
    HI_U32                   TunerFreq;
    HI_U32                   TunerSrate;
    HI_U32                   ThirdParam;
    HI_UNF_AVPLAY_OPEN_OPT_S stVidOpenOpt;
    HI_BOOL                  bAudPlay[AVPLAY_NUM];
    HI_HANDLE                hAvplayWithSnd = HI_INVALID_HANDLE;
    HI_HANDLE                hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;

    if (argc < 2)
    {
        sample_printf("Usage: %s freq [srate] [qamtype or polarization]\n",argv[0]);
        return HI_FAILURE;
    }

    if (argc >= 2)
    {
        TunerFreq  = strtol(argv[1], NULL, 0);
    }

    TunerSrate = (TunerFreq > 1000) ? 27500 : 6875;
    ThirdParam = (TunerFreq > 1000) ? 0 : 64;

    if (argc >= 3)
    {
        TunerSrate = strtol(argv[2], NULL, 0);
    }

    if (argc >= 4)
    {
        ThirdParam = strtol(argv[3], NULL, 0);
    }
   
    HI_SYS_Init();

    HIADP_MCE_Exit();
    
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_MOSAIC);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    for (i=0; i<AVPLAY_NUM; i++)
    {
        stWinRect.s32Width = 1280 / (AVPLAY_NUM / 2);
        stWinRect.s32Height = 720 / 2;
        stWinRect.s32X = i % (AVPLAY_NUM / 2) * stWinRect.s32Width;
        stWinRect.s32Y = i / (AVPLAY_NUM / 2) * stWinRect.s32Height;

        Ret = HIADP_VO_CreatWin(&stWinRect,&hWin[i]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call WinInit failed.\n");
            goto VO_DEINIT;
        }
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_Tuner_Connect(PLAY_TUNER_ID, TunerFreq, TunerSrate, ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DEMUX_ID, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    for(i = 0;i < AVPLAY_NUM;i++)
    {
        sample_printf("===============avplay[%d]===============\n",i);
        HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        AvplayAttr.stStreamAttr.u32VidBufSize = 0x200000;
        AvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
        Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay[i]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY_STOP;
        }

        stVidOpenOpt.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
		stVidOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
        stVidOpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stVidOpenOpt);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_STOP;
        }

        Ret = HI_UNF_VO_AttachWindow(hWin[i], hAvplay[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
            goto AVPLAY_STOP;
        }
        Ret = HI_UNF_VO_SetWindowEnable(hWin[i], HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
            goto AVPLAY_STOP;
        }
        
        bAudPlay[i] = HI_FALSE;
        if (0 == i)
        {
            bAudPlay[i] = HI_TRUE;
            Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            
            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
                goto AVPLAY_STOP;
            }
            Ret |= HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
            Ret |= HI_UNF_SND_Attach(hTrack, hAvplay[i]);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_SND_Attach failed.\n");
                goto AVPLAY_STOP;
            }
            hAvplayWithSnd = hAvplay[i];
        }
        
        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto AVPLAY_STOP;
        }

        ProgNum = 0;
        Ret = HIADP_AVPlay_PlayProg(hAvplay[i], g_pProgTbl, ProgNum + i,bAudPlay[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HIADP_AVPlay_SwitchProg failed\n");
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
        ProgNum = ProgNum % AVPLAY_NUM;
        HI_UNF_AVPLAY_Stop(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD,HI_NULL);
        HI_UNF_SND_Detach(hTrack,hAvplayWithSnd);
        HI_UNF_SND_DestroyTrack(hTrack);
        HI_UNF_AVPLAY_ChnClose(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        hAvplayWithSnd = hAvplay[ProgNum];
        HI_UNF_AVPLAY_ChnOpen(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD,HI_NULL);
        
        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto AVPLAY_STOP;
        }
        HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
        HI_UNF_SND_Attach(hTrack, hAvplayWithSnd);
        HIADP_AVPlay_PlayAud(hAvplayWithSnd,g_pProgTbl,ProgNum);
    }
    
AVPLAY_STOP:
    for(i = 0 ; i < AVPLAY_NUM;i++)
    {
        Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        Stop.u32TimeoutMs = 0;
        if ( ProgNum == i )
        {
            HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);            
            HI_UNF_SND_Detach(hTrack, hAvplay[i]);
            HI_UNF_SND_DestroyTrack(hTrack);
        }
        else
        {
            HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);            
        }
        HI_UNF_VO_SetWindowEnable(hWin[i],HI_FALSE);    
        HI_UNF_VO_DetachWindow(hWin[i], hAvplay[i]);
        if( ProgNum == i)
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);            
        }
        else
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID);            
        }

        HI_UNF_AVPLAY_Destroy(hAvplay[i]);         
    }
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    for ( i = 0 ; i < AVPLAY_NUM ; i++ )
    {
        HI_UNF_VO_DestroyWindow(hWin[i]);        
    }
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();
 
SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}

