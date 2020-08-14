/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ts_mosaic.c
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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf   printf
#endif

#define PLAY_DMX_ID     0
#define AVPLAY_NUM      4

FILE               *g_pTsFile = HI_NULL;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL     g_bStopTsThread = HI_FALSE;
HI_HANDLE          g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

HI_VOID* TsThread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32                Readlen;
    HI_S32                Ret;

    while (!g_bStopTsThread)
    {
        pthread_mutex_lock(&g_TsMutex);
        Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188*50, &StreamBuf, 1000);
        if (Ret != HI_SUCCESS )
        {
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*50, g_pTsFile);
        if(Readlen <= 0)
        {
            sample_printf("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            sample_printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
        pthread_mutex_unlock(&g_TsMutex);
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        sample_printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return HI_NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                      Ret;
    HI_U32                      i;
    HI_HANDLE                   hWin[AVPLAY_NUM];
    HI_HANDLE                   hAvplay[AVPLAY_NUM];
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_CHAR                     InputCmd[32];
    HI_UNF_ENC_FMT_E            VidFormat = HI_UNF_ENC_FMT_1080P_50;
    HI_U32                      ProgNum;
    HI_BOOL                     bAudPlay[AVPLAY_NUM];
    HI_HANDLE                   hTrack;

    if (argc < 2)
    {
        sample_printf("Usage:\n%s file [vo_format]\n", argv[0]);
        sample_printf("\tvo_format: 2160P_60 2160P_50 2160P_30 2160P_25 2160P_24 1080P_60 1080P_50 1080i_60 1080i_50 720P_60 720P_50\n\n");
        sample_printf("Example: %s ./test.ts 1080i_50\n", argv[0]);
        return 0;
    }

    if (argc >= 3)
    {
        VidFormat = HIADP_Disp_StrToFmt(argv[2]);
    }

    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
    {
        sample_printf("open file %s error!\n", argv[1]);
        return -1;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        hWin[i]     = HI_INVALID_HANDLE;
        hAvplay[i]  = HI_INVALID_HANDLE;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(VidFormat);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_MOSAIC);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        HI_RECT_S WinRect;

        WinRect.s32Width    = 1280 / (AVPLAY_NUM / 2);
        WinRect.s32Height   = 720 / 2;
        WinRect.s32X = i % (AVPLAY_NUM / 2) * WinRect.s32Width;
        WinRect.s32Y = i / (AVPLAY_NUM / 2) * WinRect.s32Height;

        sample_printf("x=%d, y=%d, w=%d, h=%d\n", WinRect.s32X, WinRect.s32Y, WinRect.s32Width, WinRect.s32Height);

        Ret = HIADP_VO_CreatWin(&WinRect, &hWin[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HIADP_VO_CreatWin failed.\n");
            goto VO_DEINIT;
        }
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_Init failed.\n");
        goto VO_DEINIT;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        Ret = HI_UNF_DMX_AttachTSPort(i, HI_UNF_DMX_PORT_RAM_0);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_DMX_AttachTSPort failed.\n");
            goto DMX_DEINIT;
        }
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x800000, &g_TsBuf);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    pthread_mutex_init(&g_TsMutex, HI_NULL);

    g_bStopTsThread = HI_FALSE;
    pthread_create(&g_TsThd, HI_NULL, TsThread, HI_NULL);

    HIADP_Search_Init();

    Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed.\n");
        goto PSISI_FREE;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_AVPlay_RegADecLib failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed 0x%x\n", Ret);
        goto PSISI_FREE;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
        HI_UNF_AVPLAY_OPEN_OPT_S    AvPlayOpenOpt;

        HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);

        AvplayAttr.u32DemuxId = i;
        AvplayAttr.stStreamAttr.u32VidBufSize = 8 * 1024 * 1024;

        Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_Create failed 0x%x\n", Ret);
            goto AVPLAY_STOP;
        }

        AvPlayOpenOpt.enDecType         = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
        AvPlayOpenOpt.enCapLevel        = HI_UNF_VCODEC_CAP_LEVEL_D1;
        AvPlayOpenOpt.enProtocolLevel   = HI_UNF_VCODEC_PRTCL_LEVEL_H264;

        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &AvPlayOpenOpt);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_ChnOpen Vid failed 0x%x\n", Ret);
            goto AVPLAY_STOP;
        }

        Ret = HI_UNF_VO_AttachWindow(hWin[i], hAvplay[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_VO_AttachWindow failed 0x%x\n", Ret);
            goto AVPLAY_STOP;
        }

        Ret = HI_UNF_VO_SetWindowEnable(hWin[i], HI_TRUE);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_VO_SetWindowEnable failed 0x%x\n", Ret);
            goto AVPLAY_STOP;
        }

        bAudPlay[i] = HI_FALSE;
        if (0 == i)
        {
            HI_UNF_AUDIOTRACK_ATTR_S    TrackAttr;

            bAudPlay[i] = HI_TRUE;

            Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_AVPLAY_ChnOpen Aud failed 0x%x\n", Ret);
                goto AVPLAY_STOP;
            }

            TrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_MASTER;

            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &TrackAttr);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed 0x%x\n", Ret);
                goto AVPLAY_STOP;
            }

            Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &TrackAttr, &hTrack);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_SND_CreateTrack failed 0x%x\n", Ret);
                goto AVPLAY_STOP;
            }

            Ret = HI_UNF_SND_Attach(hTrack, hAvplay[i]);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HI_UNF_SND_Attach failed 0x%x\n", Ret);
                goto AVPLAY_STOP;
            }
        }

        HI_UNF_AVPLAY_GetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);

        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;

        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_SetAttr Sync failed 0x%x\n", Ret);
            goto AVPLAY_STOP;
        }

        Ret = HIADP_AVPlay_PlayProg(hAvplay[i], g_pProgTbl, i, bAudPlay[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HIADP_AVPlay_PlayProg failed\n");
        }
    }

    while (1)
    {
        sample_printf("please input 'q' to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        ProgNum = atoi(InputCmd);

        pthread_mutex_lock(&g_TsMutex);

        rewind(g_pTsFile);

        HI_UNF_DMX_ResetTSBuffer(g_TsBuf);

        for (i = 0; i < AVPLAY_NUM; i++)
        {
            Ret = HIADP_AVPlay_PlayProg(hAvplay[i], g_pProgTbl, ProgNum + i, bAudPlay[i]);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("call HIADP_AVPlay_PlayProg!\n");
                break;
            }
        }

        pthread_mutex_unlock(&g_TsMutex);
    }

AVPLAY_STOP:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        HI_UNF_AVPLAY_STOP_OPT_S Stop;

        Stop.enMode         = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        Stop.u32TimeoutMs   = 0;

        HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
        if (0 == i)
        {
            HI_UNF_SND_Detach(hTrack, hAvplay[i]);
            HI_UNF_SND_DestroyTrack(hTrack);
        }
        HI_UNF_VO_SetWindowEnable(hWin[i],HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin[i], hAvplay[i]);
        HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_Destroy(hAvplay[i]);
    }

    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    g_pProgTbl = HI_NULL;

    HIADP_Search_DeInit();

    g_bStopTsThread = HI_TRUE;
    pthread_join(g_TsThd, HI_NULL);

    pthread_mutex_destroy(&g_TsMutex);

    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);

DMX_DEINIT:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        HI_UNF_DMX_DetachTSPort(i);
    }

    HI_UNF_DMX_DeInit();

VO_DEINIT:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hWin[i])
        {
            HI_UNF_VO_DestroyWindow(hWin[i]);
        }
    }

    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    fclose(g_pTsFile);
    g_pTsFile = HI_NULL;

    return Ret;
}


