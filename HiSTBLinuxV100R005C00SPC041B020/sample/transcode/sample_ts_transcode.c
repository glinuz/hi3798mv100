/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ts_transcode.c
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
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"

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


FILE               *g_pTsFile = HI_NULL;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL     g_bStopTsThread = HI_FALSE;
HI_HANDLE          g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#define  PLAY_DMX_ID  0

HI_VOID TsTthread(HI_VOID *args)
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
            printf("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
        pthread_mutex_unlock(&g_TsMutex);
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return;
}

volatile HI_BOOL g_bStopSaveThread = HI_TRUE;
volatile HI_BOOL g_bSaveStream = HI_FALSE;

#define VID_FILENAME_PATH  "./vid.h264"
#define AUD_FILENAME_PATH  "./aud.aac"
#define VID_FILE_MAX_LEN   (20*1024*1024)
//#define FILE_LEN_LIMIT

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

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin,hEncWin;
    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                 InputCmd[32];
    HI_UNF_ENC_FMT_E   enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32             ProgNum;
    THREAD_ARGS_S           stSaveArgs;
    pthread_t               thdSave;

    HI_HANDLE                   hTrack,hEncTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    HI_UNF_WINDOW_ATTR_S stWinAttrVoVirtual;
    HI_HANDLE hVenc;
    HI_UNF_VENC_CHN_ATTR_S stVencChnAttr;

    HI_HANDLE hAenc;
    AAC_ENC_CONFIG stPrivateConfig;
    HI_UNF_AENC_ATTR_S stAencAttr;

    if (argc < 2)
    {
        printf("Usage: %s file\n", argv[0]);
        printf("Example: %s ./test.ts\n", argv[0]);
        return 0;
    }

    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
    {
        printf("open file %s error!\n", argv[1]);
        return -1;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call SndInit failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("call VoInit failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID,HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        printf("call RegADecLib failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DMX_ID;
    AvplayAttr.stStreamAttr.u32VidBufSize = (3*1024*1024);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
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

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed:%#x.\n",Ret);
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    //stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_MASTER;
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_SND_Attach failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_SND_Attach failed.\n");
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

    /*create virtual window */
    stWinAttrVoVirtual.enDisp = HI_UNF_DISPLAY0;
    stWinAttrVoVirtual.bVirtual = HI_TRUE;
    stWinAttrVoVirtual.enVideoFormat = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;
    stWinAttrVoVirtual.stWinAspectAttr.bUserDefAspectRatio = HI_FALSE;
    stWinAttrVoVirtual.stWinAspectAttr.enAspectCvrs = HI_UNF_VO_ASPECT_CVRS_IGNORE;
    stWinAttrVoVirtual.bUseCropRect = HI_FALSE;
    memset(&stWinAttrVoVirtual.stInputRect,0,sizeof(HI_RECT_S));
    memset(&stWinAttrVoVirtual.stOutputRect,0,sizeof(HI_RECT_S));
    HI_UNF_VO_CreateWindow(&stWinAttrVoVirtual, &hEncWin);

    /*create video encoder*/
    HI_UNF_VENC_Init();
    HI_UNF_VENC_GetDefaultAttr(&stVencChnAttr);
    stVencChnAttr.enVencType = HI_UNF_VCODEC_TYPE_H264;
    stVencChnAttr.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_720P;
    stVencChnAttr.u32Width = 1280;
    stVencChnAttr.u32Height = 720;
    stVencChnAttr.u32StrmBufSize   = 1280 * 720 * 2;
    stVencChnAttr.u32TargetBitRate = 4 * 1024 * 1024;
    /*?????*/
    stVencChnAttr.u32InputFrmRate  = 50;
    stVencChnAttr.u32TargetFrmRate = 25;
    HI_UNF_VENC_Create(&hVenc, &stVencChnAttr);
    HI_UNF_VENC_AttachInput(hVenc, hEncWin);

    /*create virtual track */
    HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);
    stTrackAttr.u32OutputBufSize = 256 * 1024;
    HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hEncTrack);

    /*create aenc*/
    HI_UNF_AENC_Init();
    HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AAC.encode.so");

    /*create audio decoder*/
    stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    HA_AAC_GetDefaultConfig(&stPrivateConfig);
    HA_AAC_GetEncDefaultOpenParam(&(stAencAttr.sOpenParam),(HI_VOID *)&stPrivateConfig);
    HI_UNF_AENC_Create(&stAencAttr,&hAenc);
    /*attach audio decoder and virtual track*/
    HI_UNF_AENC_AttachInput(hAenc, hEncTrack);

    g_bStopSaveThread = HI_FALSE;
    stSaveArgs.hVenc = hVenc;
    stSaveArgs.hAenc = hAenc;
    pthread_create(&thdSave, HI_NULL,(HI_VOID *)SaveThread,&stSaveArgs);

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

    ProgNum = 0;

    pthread_mutex_lock(&g_TsMutex);
    rewind(g_pTsFile);
    HI_UNF_DMX_ResetTSBuffer(g_TsBuf);

    Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    pthread_mutex_unlock(&g_TsMutex);

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
                   "       input r to start transcode\n" \
                   "       input s to stop transcode\n" \
                   "       input 0~9 to change channel\n"\
                   "       input h to print this message\n");
            continue;
        }

        if ('r' == InputCmd[0])
        {
            HI_UNF_SND_Attach(hEncTrack, hAvplay);
            HI_UNF_VO_AttachWindow(hEncWin, hAvplay);
            HI_UNF_VO_SetWindowEnable(hEncWin, HI_TRUE);
            g_bSaveStream = HI_TRUE;
            printf("start transcode\n");
            continue;
        }

        if ('s' == InputCmd[0])
        {
            g_bSaveStream = HI_FALSE;
            HI_UNF_SND_Detach(hEncTrack, hAvplay);
            HI_UNF_VO_SetWindowEnable(hEncWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hEncWin, hAvplay);
            printf("stop transcode\n");
            continue;
        }

        if ( HI_TRUE == g_bSaveStream )
        {
            g_bSaveStream = HI_FALSE;
            HI_UNF_SND_Detach(hEncTrack, hAvplay);
            HI_UNF_VO_SetWindowEnable(hEncWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hEncWin, hAvplay);
            printf("stop transcode\n");
        }

        ProgNum = atoi(InputCmd);

        if (ProgNum == 0)
        {
            ProgNum = 1;
        }

        pthread_mutex_lock(&g_TsMutex);
        rewind(g_pTsFile);
        HI_UNF_DMX_ResetTSBuffer(g_TsBuf);

        Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum - 1, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call SwitchProgfailed!\n");
            break;
        }

        pthread_mutex_unlock(&g_TsMutex);
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

//ENC_DESTROY:
    g_bStopSaveThread = HI_TRUE;
    pthread_join(thdSave,HI_NULL);

    if (g_bSaveStream)
    {
        HI_UNF_SND_Detach(hEncTrack, hAvplay);
    }

    HI_UNF_AENC_DetachInput(hAenc);
    HI_UNF_AENC_Destroy(hAenc);
    HI_UNF_SND_DestroyTrack(hEncTrack);

    HI_UNF_VO_SetWindowEnable(hEncWin, HI_FALSE);

    if (g_bSaveStream)
    {
        HI_UNF_VO_DetachWindow(hEncWin, hAvplay);
    }

    HI_UNF_VENC_DetachInput(hVenc);
    HI_UNF_VENC_Destroy(hVenc);
    HI_UNF_VO_DestroyWindow(hEncWin);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);

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

    fclose(g_pTsFile);
    g_pTsFile = HI_NULL;

    return Ret;
}


