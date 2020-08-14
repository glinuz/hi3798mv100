/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_aenc_track.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/05/22
  Description   :
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
#include "hi_unf_aenc.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.AAC.encode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AMRWB.codec.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSPASSTHROUGH.decode.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"
#include "hi_error_mpi.h"


#if defined (DOLBYPLUS_HACODEC_SUPPORT)
 #include "HA.AUDIO.DOLBYPLUS.decode.h"
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif
FILE   *g_pAudEsFile;
FILE   *g_pAencFile;

static pthread_t g_EsThd;
static HI_HANDLE hAvplay;
HI_S32 g_AudEsFileOffest;
static HI_CHAR* g_HA_EncoderDllName = {"libHA.AUDIO.AAC.encode.so"};

static HI_BOOL g_StopThread = HI_FALSE;
static HI_BOOL g_bStopAencThread = HI_FALSE;
volatile HI_BOOL g_bSaveStream = HI_TRUE;


typedef struct
{
    pthread_t pthreadSendES;
    HI_HANDLE hAenc;
    HI_UNF_AENC_ATTR_S stAencAttr;
} AudAenc_S;

static AudAenc_S g_stAudAenc;

HI_VOID* AencThread(HI_VOID *pArgs)
{
    HI_HANDLE hAenc;
    HI_S32 Ret;
    AudAenc_S* pstAenc = (AudAenc_S*)pArgs;
    hAenc = pstAenc->hAenc;


    while(!g_bStopAencThread)
    {
        /*save audio stream*/
        if(g_bSaveStream)
        {
            HI_BOOL bGotStream = HI_FALSE;

            HI_UNF_ES_BUF_S stAencStream;

            Ret = HI_UNF_AENC_AcquireStream(hAenc,&stAencStream,0);
            if ( HI_SUCCESS == Ret )
            {
                fwrite(stAencStream.pu8Buf, 1, stAencStream.u32BufLen, g_pAencFile);
                HI_UNF_AENC_ReleaseStream(hAenc,&stAencStream);
                bGotStream = HI_TRUE;
            }
            else if ( HI_ERR_AENC_OUT_BUF_EMPTY != Ret)
            {
                printf("HI_UNF_AENC_AcquireStream failed:%#x\n",Ret);
            }

            if ( HI_FALSE == bGotStream )
            {
                usleep(10 * 1000);
            }
        }
        else
            usleep(5 * 1000);
    }

    fclose(g_pAencFile);
    return HI_NULL;
}


HI_VOID AudioTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32 Readlen;
    HI_S32 Ret;
    HI_BOOL bAudBufAvail = HI_TRUE;

    bAudBufAvail = HI_FALSE;

    while (!g_StopThread)
    {
        Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x1000, &StreamBuf, 0);
        if (HI_SUCCESS == Ret)
        {
            bAudBufAvail = HI_TRUE;
            Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 0x1000, g_pAudEsFile);
            if (Readlen > 0)
            {
                Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                }
            }
            else if (Readlen <= 0)
            {
                sample_printf("read aud file error!\n");
                rewind(g_pAudEsFile);
                if (g_AudEsFileOffest)
                {
                    fseek(g_pAudEsFile, g_AudEsFileOffest, SEEK_SET);
                }
            }
        }
        else if (Ret != HI_SUCCESS)
        {
            bAudBufAvail = HI_FALSE;
        }

        /* wait for buffer */
        if (HI_FALSE == bAudBufAvail)
        {
            usleep(1000 * 10);
        }
    }

    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hTrack;
    HI_HANDLE hVirTrack;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
    HI_UNF_SND_ATTR_S stAttr;

    HI_U32 AdecType;
    HI_U32 AencType;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S AvSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR InputCmd[32];
    HI_HA_DECODEMODE_E enAudioDecMode = HD_DEC_MODE_RAWPCM;
    HI_S32 s32DtsDtsCoreOnly = 0;
    HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_720P_50;

    if (argc != 5)
    {
        sample_printf(" usage:     ./sample_aenctrack infile intype outfile outtype\n");
        sample_printf(" type:      aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3)\n");
        sample_printf(" examples: \n");
        sample_printf("            ./sample_aenctrack infile mp3 outfile aac\n");
        return -1;
    }

    g_pAudEsFile = fopen(argv[1], "rb");
    if (!g_pAudEsFile)
    {
        sample_printf("open file %s error!\n", argv[1]);
        return -1;
    }

    if (!strcasecmp("aac", argv[2]))
    {
        AdecType = HA_AUDIO_ID_AAC;
    }
    else if (!strcasecmp("mp3", argv[2]))
    {
        AdecType = HA_AUDIO_ID_MP3;
    }

    else if (!strcasecmp("ac3raw", argv[2]))
    {
        AdecType = HA_AUDIO_ID_AC3PASSTHROUGH;
    }
    else if (!strcasecmp("dtsraw", argv[2]))
    {
        AdecType = HA_AUDIO_ID_DTSPASSTHROUGH;
    }
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
    else if (!strcasecmp("ddp", argv[2]))
    {
        AdecType = HA_AUDIO_ID_DOLBY_PLUS;
    }
#endif
    else if (!strcasecmp("dts", argv[2]))
    {
        AdecType = HA_AUDIO_ID_DTSHD;
    }
    else if (!strcasecmp("dra", argv[2]))
    {
        AdecType = HA_AUDIO_ID_DRA;
    }
    else if (!strcasecmp("pcm", argv[2]))
    {
        AdecType = HA_AUDIO_ID_PCM;
    }
    else if (!strcasecmp("mlp", argv[2]))
    {
        AdecType = HA_AUDIO_ID_TRUEHD;
    }
    else if (!strcasecmp("amr", argv[2]))
    {
        AdecType = HA_AUDIO_ID_AMRNB;
    }
    else if (!strcasecmp("amrwb", argv[2]))
    {
        AdecType = HA_AUDIO_ID_AMRWB;
#if 1
        /*read header of file for MIME file format */
        extern HI_U8 u8DecOpenBuf[1024];
        AMRWB_DECODE_OPENCONFIG_S *pstConfig = (AMRWB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
        if (pstConfig->enFormat == AMRWB_FORMAT_MIME)
        {
            HI_U8 u8MagicBuf[8];

            fread(u8MagicBuf, sizeof(HI_S8), strlen(AMRWB_MAGIC_NUMBER), g_pAudEsFile);/* just need by amr file storage format (xxx.amr) */
            if (strncmp((const char *)u8MagicBuf, AMRWB_MAGIC_NUMBER, strlen(AMRWB_MAGIC_NUMBER)))
            {
                sample_printf( "%s Invalid amr wb file magic number ", u8MagicBuf);
                return HI_FAILURE;
            }
        }
        g_AudEsFileOffest = strlen(AMRWB_MAGIC_NUMBER);
#endif
    }
    else
    {
        sample_printf("unsupport aud codec type!\n");
        return -1;
    }

    g_pAencFile = fopen(argv[3], "wb");
    if (!g_pAencFile)
    {
        sample_printf("open file %s error!\n", argv[3]);
        return -1;
    }

    if (!strcasecmp("aac", argv[4]))
    {
        AencType = HA_AUDIO_ID_AAC;
    }
    else
    {
        sample_printf("unsupport aud encode type!\n");
        return -1;
    }

    HI_SYS_Init();
    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HI_UNF_SND_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Init failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AENC_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AENC_Init failed.\n");
        goto SND_DEINIT;
    }

    HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &stAttr);
    Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Open failed.\n");
        goto AENC_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_RegisterAcodecLib failed.\n");
        goto SND_CLOSE;
    }

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);

    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto SND_CLOSE;
    }
    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
      sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
      goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto ACHN_CLOSE;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_VIRTUAL, &stTrackAttr);

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hVirTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto SND_DETACH;
    }

    HI_UNF_AENC_RegisterEncoder(g_HA_EncoderDllName);

    g_stAudAenc.stAencAttr.enAencType = AencType;
    AAC_ENC_CONFIG stPrivateConfig;
    HA_AAC_GetDefaultConfig(&stPrivateConfig);

    if (HA_AUDIO_ID_AAC == g_stAudAenc.stAencAttr.enAencType)
    {
        HA_AAC_GetEncDefaultOpenParam(&g_stAudAenc.stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);
        sample_printf("DesiredSampleRate=%d\n", g_stAudAenc.stAencAttr.sOpenParam.u32DesiredSampleRate);
    }

    Ret = HI_UNF_AENC_Create(&(g_stAudAenc.stAencAttr), &(g_stAudAenc.hAenc));

    Ret = HI_UNF_AENC_AttachInput(g_stAudAenc.hAenc, hVirTrack);

    Ret = HI_UNF_SND_Attach(hVirTrack, hAvplay);

    Ret = HIADP_AVPlay_SetAdecAttr(hAvplay, AdecType, enAudioDecMode, s32DtsDtsCoreOnly);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_AVPlay_SetAdecAttr failed.\n");
        goto AENC_STOP;
    }
    Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto AVPLAY_STOP;
    }

    g_StopThread = HI_FALSE;
    pthread_create(&g_EsThd, HI_NULL, (HI_VOID *)AudioTthread, (HI_VOID *)HI_NULL);
    g_bSaveStream = HI_TRUE;
    g_bStopAencThread = HI_FALSE;
    pthread_create(&(g_stAudAenc.pthreadSendES), HI_NULL, (HI_VOID *)AencThread, (HI_VOID *)&(g_stAudAenc));

    while (1)
    {
        sample_printf("\nplease input the q to quit!, r/s to toggle start/stop aenc\n\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        if ('r' == InputCmd[0])
        {
            HI_UNF_AENC_AttachInput(g_stAudAenc.hAenc, hVirTrack);
            g_bSaveStream = HI_TRUE;
            sample_printf("start aenc_track\n");
            continue;
        }

        if ('s' == InputCmd[0])
        {
            g_bSaveStream = HI_FALSE;
            HI_UNF_AENC_DetachInput(g_stAudAenc.hAenc);
            sample_printf("stop aenc_track\n");
            continue;
        }
    }

    g_bStopAencThread = HI_TRUE;
    pthread_join(g_stAudAenc.pthreadSendES, HI_NULL);

    g_StopThread = HI_TRUE;
    pthread_join(g_EsThd, HI_NULL);

    HIADP_Disp_DeInit();

AVPLAY_STOP:
    Stop.u32TimeoutMs = 0;
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

AENC_STOP:
    HI_UNF_SND_Detach(hVirTrack, hAvplay);
    HI_UNF_AENC_DetachInput(g_stAudAenc.hAenc);
    HI_UNF_AENC_Destroy(g_stAudAenc.hAenc);
    HI_UNF_SND_DestroyTrack(hVirTrack);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

ACHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

SND_CLOSE:
    HI_UNF_SND_Close(HI_UNF_SND_0);

AENC_DEINIT:
    HI_UNF_AENC_DeInit();

SND_DEINIT:
    HI_UNF_SND_DeInit();


AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    fclose(g_pAudEsFile);
    g_pAudEsFile = HI_NULL;

    return 0;
}
