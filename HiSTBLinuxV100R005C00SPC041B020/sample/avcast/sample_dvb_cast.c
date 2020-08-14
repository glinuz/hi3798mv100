/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_dvb_cast.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AAC.encode.h"
#include "hi_unf_aenc.h"
#include "hi_unf_venc.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#endif

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;
static HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif


volatile HI_BOOL g_bStopSaveThread = HI_TRUE;
volatile HI_BOOL g_bSaveStream = HI_FALSE;

#define VID_FILENAME_PATH  "./vid.h264"
#define AUD_FILENAME_PATH  "./aud.aac"
#define VID_FILE_MAX_LEN   (20*1024*1024)
//#define FILE_LEN_LIMIT
#define DISP_CAST_WIDTH    1280
#define DISP_CAST_HEIGHT   720


typedef struct
{
    HI_HANDLE hVenc;
    HI_HANDLE hAenc;
}THREAD_ARGS_S;

HI_VOID* SaveThread(HI_VOID *pArgs)
{
    THREAD_ARGS_S *pThreadArgs = (THREAD_ARGS_S*)pArgs;
    HI_HANDLE hVencChn,hAencChn;
    FILE *pVidFile,*pAudFile;
    HI_S32 Ret;

    hVencChn = pThreadArgs->hVenc;
    hAencChn = pThreadArgs->hAenc;
    pVidFile = fopen(VID_FILENAME_PATH,"w");
    if ( HI_NULL == pVidFile )
    {
        printf("open file %s failed\n",VID_FILENAME_PATH);
        return HI_NULL;
    }

    pAudFile = fopen(AUD_FILENAME_PATH,"w");
    if ( HI_NULL == pVidFile )
    {
        printf("open file %s failed\n",AUD_FILENAME_PATH);
        fclose(pVidFile);
        return HI_NULL;
    }

    while(!g_bStopSaveThread)
    {
        HI_BOOL bGotStream = HI_FALSE;

        if (g_bSaveStream)
        {
            HI_UNF_VENC_STREAM_S stVencStream;
            HI_UNF_ES_BUF_S stAencStream;
#ifdef FILE_LEN_LIMIT
            if (ftell(pVidFile) >= VID_FILE_MAX_LEN)
            {
                fclose(pVidFile);
                fclose(pAudFile);
                pVidFile = fopen(VID_FILENAME_PATH,"w");
                pAudFile = fopen(AUD_FILENAME_PATH,"w");
                printf("stream files are truncated to zero\n");
            }
#endif

            /*save video stream*/
            Ret = HI_UNF_VENC_AcquireStream(hVencChn,&stVencStream,0);
            if ( HI_SUCCESS == Ret )
            {
                //printf("HI_UNF_VENC_AcquireStream %#x\n",stVencStream.u32SlcLen);
                fwrite(stVencStream.pu8Addr, 1, stVencStream.u32SlcLen, pVidFile);
                HI_UNF_VENC_ReleaseStream(hVencChn,&stVencStream);
                fflush(pVidFile);
                bGotStream = HI_TRUE;
            }
            else if ( HI_ERR_VENC_BUF_EMPTY != Ret)
            {
                printf("HI_UNF_VENC_AcquireStream failed:%#x\n",Ret);
            }

            /*save audio stream*/
            Ret = HI_UNF_AENC_AcquireStream(hAencChn,&stAencStream,0);
            if ( HI_SUCCESS == Ret )
            {
                fwrite(stAencStream.pu8Buf, 1, stAencStream.u32BufLen, pAudFile);
                fflush(pAudFile);
                HI_UNF_AENC_ReleaseStream(hAencChn,&stAencStream);
                bGotStream = HI_TRUE;
            }
            else if ( HI_ERR_AENC_OUT_BUF_EMPTY != Ret)
            {
                printf("HI_UNF_AENC_AcquireStream failed:%#x\n",Ret);
            }
        }

        if ( HI_FALSE == bGotStream )
        {
            usleep(10 * 1000);
        }
    }

    fclose(pVidFile);
    fclose(pAudFile);
    return HI_NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hWin;
    HI_HANDLE hAvplay;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR InputCmd[32];
    HI_U32 ProgNum;
    THREAD_ARGS_S           stSaveArgs;
    pthread_t               thdSave;

    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;

    HI_UNF_DISP_CAST_ATTR_S  stDispCastAttr;
    HI_HANDLE                hDispCast;
    HI_HANDLE hVenc;
    HI_UNF_VENC_CHN_ATTR_S stVencChnAttr;
    HI_HANDLE hSndCast;
    HI_UNF_SND_CAST_ATTR_S stSndCastAttr;
    HI_HANDLE hAenc;
    AAC_ENC_CONFIG stPrivateConfig;
    HI_UNF_AENC_ATTR_S stAencAttr;

    if (5 == argc)
    {
        g_TunerFreq  = strtol(argv[1], NULL, 0);
        g_TunerSrate = strtol(argv[2], NULL, 0);
        g_ThirdParam = strtol(argv[3], NULL, 0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[4]);
    }
    else if (4 == argc)
    {
        g_TunerFreq  = strtol(argv[1], NULL, 0);
        g_TunerSrate = strtol(argv[2], NULL, 0);
        g_ThirdParam = strtol(argv[3], NULL, 0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if (3 == argc)
    {
        g_TunerFreq  = strtol(argv[1], NULL, 0);
        g_TunerSrate = strtol(argv[2], NULL, 0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if (2 == argc)
    {
        g_TunerFreq  = strtol(argv[1], NULL, 0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else
    {
        printf("Usage: %s freq [srate] [qamtype or polarization] [vo_format]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50  default HI_UNF_ENC_FMT_1080i_50\n",
                argv[0]);
        printf("Example: %s 610 6875 64 1080i_50\n", argv[0]);
        return HI_FAILURE;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HI_UNF_DMX_AttachTSPort(0,DEFAULT_DVB_PORT);
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

    Ret = HIADP_Tuner_Connect(TUNER_USE, g_TunerFreq, g_TunerSrate, g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    Ret  = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret  = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
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
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    /*create video cast */
    HI_UNF_DISP_GetDefaultCastAttr(HI_UNF_DISPLAY1,&stDispCastAttr);
    stDispCastAttr.u32Width  = DISP_CAST_WIDTH;
    stDispCastAttr.u32Height = DISP_CAST_HEIGHT;
    HI_UNF_DISP_CreateCast(HI_UNF_DISPLAY1,&stDispCastAttr,&hDispCast);

    /*create video encoder*/
    HI_UNF_VENC_Init( );
    HI_UNF_VENC_GetDefaultAttr(&stVencChnAttr);
    stVencChnAttr.enVencType = HI_UNF_VCODEC_TYPE_H264;
    stVencChnAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_720P;
    stVencChnAttr.u32Width = DISP_CAST_WIDTH;
    stVencChnAttr.u32Height = DISP_CAST_HEIGHT;
    stVencChnAttr.u32StrmBufSize   = DISP_CAST_WIDTH * DISP_CAST_HEIGHT * 2;
    stVencChnAttr.u32TargetBitRate = 4 * 1024 * 1024;
    /*?????*/
    stVencChnAttr.u32InputFrmRate  = 25;
    stVencChnAttr.u32TargetFrmRate = 25;
    HI_UNF_VENC_Create(&hVenc, &stVencChnAttr);
    /*attach video encoder with cast*/
    HI_UNF_VENC_AttachInput(hVenc,hDispCast);

    /*create audio cast */
    HI_UNF_SND_GetDefaultCastAttr(HI_UNF_SND_0,&stSndCastAttr);
    HI_UNF_SND_CreateCast(HI_UNF_SND_0,&stSndCastAttr,&hSndCast);

    /*create audio encoder*/
    HI_UNF_AENC_Init();
    HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AAC.encode.so");
    stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    HA_AAC_GetDefaultConfig(&stPrivateConfig);
    HA_AAC_GetEncDefaultOpenParam(&(stAencAttr.sOpenParam),(HI_VOID *)&stPrivateConfig);
    HI_UNF_AENC_Create(&stAencAttr,&hAenc);
    /*attach audio encoder and cast*/
    HI_UNF_AENC_AttachInput(hAenc,hSndCast);

#ifdef ANDROID

    HI_HANDLE               hSurface;
    HI_RECT_S               stRect;
    HIADP_SURFACE_ATTR_S    stSurAttr;

    stSurAttr.u32Width = 1280;
    stSurAttr.u32Height = 720;
    stSurAttr.enPixelFormat = HIADP_PF_8888;
    HIADP_OSD_CreateSurface(&stSurAttr, &hSurface);

    stRect.s32X = 0;
    stRect.s32Y = 0;
    stRect.s32Width = 400;
    stRect.s32Height = 300;
    HIADP_OSD_FillRect(hSurface, &stRect, 0xff0000ff);

    HIADP_OSD_Refresh(hSurface);

#else
    /*draw ui*/
    HI_HANDLE hLayer,hLayerSurface;
    HIGO_LAYER_INFO_S stLayerInfo;
    HI_RECT stRect;
    HI_GO_Init();
    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0,&stLayerInfo);
    stLayerInfo.DisplayWidth = 1280;
    stLayerInfo.DisplayHeight = 720;
    HI_GO_CreateLayer(&stLayerInfo,&hLayer);
    HI_GO_GetLayerSurface(hLayer,&hLayerSurface);
    stRect.w = 200;
    stRect.h = 200;
    stRect.x = 1280 - stRect.w;
    stRect.y = 720 - stRect.h;
    HI_GO_FillRect(hLayerSurface,&stRect,0xff00ffff, HIGO_COMPOPT_NONE);
    HI_GO_SetLayerAlpha(hLayer,100);
    HI_GO_RefreshLayer(hLayer,HI_NULL);
#endif

    g_bStopSaveThread = HI_FALSE;
    stSaveArgs.hVenc = hVenc;
    stSaveArgs.hAenc = hAenc;
    pthread_create(&thdSave, HI_NULL,(HI_VOID *)SaveThread,&stSaveArgs);

    ProgNum = 0;
    Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    printf("please input q to quit\n" \
           "       input r to start transcode\n" \
           "       input s to stop transcode\n" \
           "       input 0~9 to change channel\n"\
           "       input h to print this message\n");
    while (1)
    {
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        if ('h' == InputCmd[0])
        {
            printf("please input q to quit\n" \
                   "       input r to start avcast\n" \
                   "       input s to stop avcast\n" \
                   "       input 0~9 to change channel\n"\
                   "       input h to print this message\n");
            continue;
        }

        if ('r' == InputCmd[0])
        {
            Ret = HI_UNF_SND_SetCastEnable(hSndCast, HI_TRUE);
            Ret |= HI_UNF_DISP_SetCastEnable(hDispCast, HI_TRUE);
            g_bSaveStream = HI_TRUE;
            printf("start avcast\n");
            continue;
        }

        if ('s' == InputCmd[0])
        {
            g_bSaveStream = HI_FALSE;
            Ret = HI_UNF_SND_SetCastEnable(hSndCast, HI_FALSE);
            Ret |= HI_UNF_DISP_SetCastEnable(hDispCast, HI_FALSE);
            printf("stop avcast\n");
            continue;
        }

        if ( HI_TRUE == g_bSaveStream )
        {
            g_bSaveStream = HI_FALSE;
            Ret = HI_UNF_SND_SetCastEnable(hSndCast, HI_FALSE);
            Ret |= HI_UNF_DISP_SetCastEnable(hDispCast, HI_FALSE);
            printf("stop avcast\n");
        }

        ProgNum = atoi(InputCmd);
        if (ProgNum == 0)
        {
            ProgNum = 1;
        }

        Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum - 1, HI_TRUE);
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

#ifdef ANDROID
    HIADP_OSD_DestroySurface(hSurface);
#else
    HI_GO_DestroyLayer(hLayer);
    HI_GO_Deinit();
#endif

    g_bStopSaveThread = HI_TRUE;
    pthread_join(thdSave,HI_NULL);

    HI_UNF_SND_SetCastEnable(hSndCast, HI_FALSE);
    HI_UNF_AENC_DetachInput(hAenc);
    HI_UNF_AENC_Destroy(hAenc);
    HI_UNF_AENC_DeInit();
    HI_UNF_SND_DestroyCast(hSndCast);

    HI_UNF_DISP_SetCastEnable(hDispCast, HI_FALSE);
    HI_UNF_VENC_DetachInput(hVenc);
    HI_UNF_VENC_Destroy(hVenc);
    HI_UNF_DISP_DestroyCast(hDispCast);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}


