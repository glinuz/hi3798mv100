/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_3dtv.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/22
  Description   :
  History       :
  1.Date        : 2013/07/22
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
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "hi_go.h"
#include "hi_go_decoder.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

FILE               *g_pTsFile = HI_NULL;
static pthread_t g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL g_bStopTsThread = HI_FALSE;
HI_HANDLE g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#define PLAY_DMX_ID     0
#define DECODE_JPEG
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720


static HI_UNF_DISP_3D_E stringTo3DMode(HI_CHAR *p3dModeString)
{
    if (0 == strcmp(p3dModeString, "none"))
    {
        return HI_UNF_DISP_3D_NONE;
    }
    else if (0 == strcmp(p3dModeString, "fp"))
    {
        return HI_UNF_DISP_3D_FRAME_PACKING;
    }
    else if (0 == strcmp(p3dModeString, "sbs"))
    {
        return HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF;
    }
    else if (0 == strcmp(p3dModeString, "tab"))
    {
        return HI_UNF_DISP_3D_TOP_AND_BOTTOM;
    }
    else if (0 == strcmp(p3dModeString, "mvc"))
    {
        return HI_UNF_DISP_3D_FIELD_ALTERNATIVE;
    }

    printf("%s is unsupported 3d mode,3d mode is set to none by default\n", p3dModeString);

    return HI_UNF_DISP_3D_NONE;
}

static HI_UNF_VIDEO_FRAME_PACKING_TYPE_E stringToFrameType(HI_CHAR *string)
{
    if (0 == strcmp(string, "sbs"))
    {
        return HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE;
    }
    else if (0 == strcmp(string, "tab"))
    {
        return HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM;
    }
    else if (0 == strcmp(string, "mvc"))
    {
        return HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED;
    }
    else
    {
        return HI_UNF_FRAME_PACKING_TYPE_NONE;
    }
}

HI_S32 Draw_3DUI(HI_HANDLE hLayer)
{
    HI_S32      Ret;
    HI_HANDLE   hLayerSurface;
    HI_RECT     Rect;

    Ret = HI_GO_SetLayerAlpha(hLayer, 150);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_GO_SetLayerAlpha failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    Ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_GO_GetLayerSurface failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    Rect.w = 200;
    Rect.h = 200;
    Rect.x = (SCREEN_WIDTH / 2) - (Rect.w / 2);
    Rect.y = (SCREEN_HEIGHT / 2) - (Rect.h / 2);

    Ret = HI_GO_FillRect(hLayerSurface, &Rect, 0xff00ffff, HIGO_COMPOPT_NONE);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_GO_FillRect failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    Ret = HI_GO_RefreshLayer(hLayer, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_GO_RefreshLayer failed 0x%x\n", Ret);
        return HI_FAILURE;
    }

    return Ret;
}

HI_VOID* TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32 Readlen;
    HI_S32 Ret;

    while (!g_bStopTsThread)
    {
        pthread_mutex_lock(&g_TsMutex);
        Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188 * 50, &StreamBuf, 1000);
        if (Ret != HI_SUCCESS)
        {
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188 * 50, g_pTsFile);
        if (Readlen <= 0)
        {
            printf("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }

        pthread_mutex_unlock(&g_TsMutex);
    }

    return HI_NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hWin;
    HI_HANDLE hAvplay;
    HI_HANDLE hTrack;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR InputCmd[32];
    HI_UNF_ENC_FMT_E enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32 ProgNum;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr = {0};
    HI_UNF_DISP_3D_E en3DMode;
    HI_UNF_VIDEO_FRAME_PACKING_TYPE_E FrameType;
    HI_HANDLE         hLayer;
    HIGO_LAYER_INFO_S stLayerInfo;

    if (argc < 4)
    {
        printf("Usage: %s file <source format> <display format>\n", argv[0]);
        printf("\tsource format: none, sbs - side by side, tab - top and bottom, mvc\n");
        printf("\tdisplay format: none, fp - frame packing, sbs - side by side, tab - top and bottom\n");
        printf("Example: %s ./test.ts sbs sbs\n", argv[0]);

        return 0;
    }

    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
    {
        perror("open file error");

        return -1;
    }

    FrameType = stringToFrameType(argv[2]);
    en3DMode = stringTo3DMode(argv[3]);

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HI_GO_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_GO_Init failed 0x%x\n", Ret);
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_Init failed.\n");
        goto SYS_DEINIT;
    }

    if (HI_UNF_DISP_3D_NONE != en3DMode)
    {
        HI_UNF_ENC_FMT_E fmt;

        switch (en3DMode)
        {
            case HI_UNF_DISP_3D_FRAME_PACKING :
                fmt = HI_UNF_ENC_FMT_720P_50_FRAME_PACKING;
                break;

            case HI_UNF_DISP_3D_SIDE_BY_SIDE_HALF :
                fmt = HI_UNF_ENC_FMT_1080i_50;
                break;

            case HI_UNF_DISP_3D_TOP_AND_BOTTOM :
                fmt = HI_UNF_ENC_FMT_720P_50;
                break;

            default :
                fmt = HI_UNF_ENC_FMT_1080i_50;
        }

        Ret = HI_UNF_DISP_Set3DMode(HI_UNF_DISPLAY1, en3DMode, fmt);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_DISP_Set3DMode failed 0x%x\n", Ret);
            goto DISP_DEINIT;
        }
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_CreatWin failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto DESTROY_WIN;
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_DMX_Init failed 0x%x\n", Ret);
        goto SND_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID, HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_DMX_AttachTSPort failed 0x%x\n", Ret);
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        printf("HI_UNF_DMX_CreateTSBuffer failed 0x%x\n", Ret);
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("HI_UNF_AVPLAY_Init failed 0x%x\n", Ret);
        goto TSBUF_FREE;
    }

    HIADP_AVPlay_RegADecLib();

#ifdef DECODE_JPEG
    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);

    stLayerInfo.PixelFormat     = HIGO_PF_8888;
    stLayerInfo.CanvasWidth     = SCREEN_WIDTH;;
    stLayerInfo.CanvasHeight    = SCREEN_HEIGHT;;
    stLayerInfo.DisplayWidth    = SCREEN_WIDTH;
    stLayerInfo.DisplayHeight   = SCREEN_HEIGHT;

    Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_GO_CreateLayer failed.\n");
        goto AVPLAY_DEINIT;
    }

    Draw_3DUI(hLayer);
#endif

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DMX_ID;
    AvplayAttr.stStreamAttr.u32VidBufSize = (9 * 1024 * 1024);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed:%#x.\n",Ret);
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VCHN_CLOSE;
    }

    if (HI_UNF_FRAME_PACKING_TYPE_NONE != FrameType)
    {
        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMPACK_TYPE, (HI_VOID*)&FrameType);
        if (Ret != HI_SUCCESS)
        {
            printf("HI_UNF_AVPLAY_SetAttr FRMPACK_TYPE failed 0x%x\n", Ret);
            goto ACHN_CLOSE;
        }
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed:%#x.\n", Ret);
        goto ACHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    pthread_mutex_init(&g_TsMutex,NULL);
    g_bStopTsThread = HI_FALSE;
    pthread_create(&g_TsThd, HI_NULL, (HI_VOID *)TsTthread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID, &g_pProgTbl);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_Search_GetAllPmt failed.\n");
        goto PSISI_FREE;
    }

#if 0
    printf("select channel number to start play\n");
    SAMPLE_GET_INPUTCMD(InputCmd);
    ProgNum = atoi(InputCmd);
#endif

    ProgNum = 0;

    pthread_mutex_lock(&g_TsMutex);
    rewind(g_pTsFile);
    HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    pthread_mutex_unlock(&g_TsMutex);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_AVPlay_PlayProg failed.\n");
        goto AVPLAY_STOP;
    }

    while (1)
    {
        static HI_U32 u32TplaySpeed = 2;
        printf("please input 'q' to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);
        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        if ('t' == InputCmd[0])
        {
            HI_UNF_AVPLAY_TPLAY_OPT_S stTplayOpt;
            printf("%dx tplay\n", u32TplaySpeed);

            stTplayOpt.enTplayDirect   = HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD;
            stTplayOpt.u32SpeedInteger = u32TplaySpeed;
            stTplayOpt.u32SpeedDecimal = 0;

            HI_UNF_AVPLAY_SetDecodeMode(hAvplay, HI_UNF_VCODEC_MODE_I);
            HI_UNF_AVPLAY_Tplay(hAvplay, &stTplayOpt);
            u32TplaySpeed = (32 == u32TplaySpeed * 2) ? (2) : (u32TplaySpeed * 2);
            continue;
        }

        if ('r' == InputCmd[0])
        {
            printf("resume\n");
            HI_UNF_AVPLAY_SetDecodeMode(hAvplay, HI_UNF_VCODEC_MODE_NORMAL);
            HI_UNF_AVPLAY_Resume(hAvplay, HI_NULL);
            u32TplaySpeed = 2;
            continue;
        }

        if ('g' == InputCmd[0])
        {
            HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
            HI_UNF_AVPLAY_GetStatusInfo(hAvplay, &stStatusInfo);
            printf("localtime %u playtime %u\n", stStatusInfo.stSyncStatus.u32LocalTime,
                   stStatusInfo.stSyncStatus.u32PlayTime);
            continue;
        }

        ProgNum = atoi(InputCmd);
        if (ProgNum == 0)
        {
            ProgNum = 1;
        }

        pthread_mutex_lock(&g_TsMutex);
        rewind(g_pTsFile);
        HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
        Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
        pthread_mutex_unlock(&g_TsMutex);
        if (Ret != HI_SUCCESS)
        {
            printf("call SwitchProgfailed!\n");
            break;
        }
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    g_pProgTbl = HI_NULL;
    HIADP_Search_DeInit();

    g_bStopTsThread = HI_TRUE;
    pthread_join(g_TsThd, HI_NULL);
    pthread_mutex_destroy(&g_TsMutex);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

ACHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
#ifdef DECODE_JPEG
    HI_GO_DestroyLayer(hLayer);
#endif

    HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DMX_ID);
    HI_UNF_DMX_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

DESTROY_WIN:
    HI_UNF_VO_DestroyWindow(hWin);

VO_DEINIT:
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SYS_DEINIT:
    HI_GO_Deinit();

    HI_SYS_DeInit();

    if (g_pTsFile)
    {
        fclose(g_pTsFile);
        g_pTsFile = HI_NULL;
    }

    return 1;
}

