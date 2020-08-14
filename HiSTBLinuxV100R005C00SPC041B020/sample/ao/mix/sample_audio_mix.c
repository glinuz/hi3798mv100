/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_audio_mix.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp.h"

#define MAX_AVPLAY_NUM  16

static HI_BOOL     g_StopAudThread = HI_FALSE;
static FILE* g_pAudEsFile[MAX_AVPLAY_NUM - 1];
static HI_U32 g_AudType[MAX_AVPLAY_NUM - 1];
HI_HANDLE               g_hAvplay[MAX_AVPLAY_NUM];
HI_HANDLE               g_hTrack[MAX_AVPLAY_NUM];

HI_VOID AudEsTthread(HI_VOID* args)
{
    HI_HANDLE         hAvplay;
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            Ret;
    HI_U32            j;

    j = (HI_U8*)args - (HI_U8*)HI_NULL;
    hAvplay = (HI_HANDLE)g_hAvplay[1 + j];

    while (!g_StopAudThread)
    {
        Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x4000, &StreamBuf, 0);
        if (Ret != HI_SUCCESS )
        {
            usleep(10 * 1000) ;
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 0x4000, g_pAudEsFile[j]);
        if (Readlen <= 0)
        {
            printf("read aud file error!\n");
            rewind(g_pAudEsFile[j]);
            continue;
        }

        Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
        if (Ret != HI_SUCCESS )
        {
            printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
        }
        else
        {
        }
    }
}

/*DVB->master, MP3->slave*/
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                 InputCmd[32];

    HI_U32 g_TunerFreq;
    HI_U32 g_TunerSrate;
    HI_U32 ThirdParam;
    PMT_COMPACT_TBL*    g_pProgTbl = HI_NULL;
    HI_BOOL bDVBPlay = HI_TRUE;
    HI_U32                  ProgNum;
    pthread_t               audio_send[MAX_AVPLAY_NUM - 1];
    HI_U32                  i, j;

    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_UNF_SND_GAIN_ATTR_S stGain;
    stGain.bLinearMode = HI_TRUE;

    if (argc < 3)
    {
        printf("Usage:           ./sample_audiomix freq srate inputfile0 type0 inputfile1 type1 ...\n");
        printf("Audio file type: /aac/mp3/ddp/dts/dra\n");
        printf("examples:        ./sample_audiomix 618 6875 inputfile0 mp3 inputfile1 ddp\n");
        return HI_FAILURE;
    }
    for (i = 0; i < MAX_AVPLAY_NUM; i++)
    {
        g_hAvplay[i] = HI_INVALID_HANDLE;
    }

    for (j = 0; j < MAX_AVPLAY_NUM - 1; j++)
    {
        g_pAudEsFile[j] = HI_NULL;
        audio_send[j] = -1;
    }

    i = 1;
    g_TunerFreq  = strtol(argv[i++], NULL, 0);
    g_TunerSrate = strtol(argv[i++], NULL, 0);
    ThirdParam = (g_TunerFreq > 1000) ? 0 : 64;
    if ((0 == g_TunerFreq) || (0 == g_TunerSrate))
    {
        bDVBPlay = HI_FALSE;
    }

    j = 0;
    while ((i < argc) && (j < MAX_AVPLAY_NUM - 1))
    {
        g_pAudEsFile[j] = fopen(argv[i++], "rb");
        if (g_pAudEsFile[j] == HI_NULL)
        {
            printf("open audio file failed!\n");
            return HI_FAILURE;
        }

        if (!strcasecmp("aac", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_AAC;
        }
        else if (!strcasecmp("mp3", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_MP3;
        }
        else if (!strcasecmp("ddp", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_DOLBY_PLUS;
        }
        else if (!strcasecmp("dts", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_DTSHD;
        }
        else if (!strcasecmp("dra", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_DRA;
        }
        else if (!strcasecmp("pcm", argv[i]))
        {
            g_AudType[j] = HA_AUDIO_ID_PCM;
        }
        else
        {
            printf("unsupport aud codec type!\n");
            return -1;
        }
        i++;
        j++;
    }
    Ret = HI_SYS_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_SYS_Init failed.\n");
        return Ret;
    }

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080P_50);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_VO_CreatWin failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto VO_DEINIT;
    }

    if (HI_TRUE == bDVBPlay)
    {
        Ret = HIADP_Tuner_Init();
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_Tuner_Init failed.\n");
            goto AVPLAY_DEINIT;
        }

        HIADP_Search_Init();

        Ret = HI_UNF_DMX_Init();
        Ret |= HIADP_DMX_AttachTSPort(0, 0);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_Demux_Init failed.\n");
            goto SRCH_DEINIT;
        }

        Ret = HIADP_Tuner_Connect(TUNER_USE, g_TunerFreq, g_TunerSrate, ThirdParam);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_Tuner_Connect failed.\n");
            goto DMX_DEINIT;
        }

        Ret = HIADP_Search_GetAllPmt(0, &g_pProgTbl);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_Search_GetAllPmt failed.\n");
            goto DMX_DEINIT;
        }

        Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
        Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay[0]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto PSISI_FREE;
        }

        Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
            goto AVPLAY0_STOP;
        }

        Ret = HI_UNF_VO_AttachWindow(hWin, g_hAvplay[0]);
        Ret |= HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_VO_SetWindowEnable failed.\n");
            HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hWin, g_hAvplay[0]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
            goto AVPLAY0_STOP;
        }

        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto AVPLAY0_STOP;
        }
        Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &g_hTrack[0]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_CreateTrack failed.\n");
            HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hWin, g_hAvplay[0]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
            goto AVPLAY0_STOP;
        }

        Ret = HI_UNF_SND_Attach(g_hTrack[0], g_hAvplay[0]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_Attach failed.\n");
            HI_UNF_SND_DestroyTrack(g_hTrack[0]);
            HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hWin, g_hAvplay[0]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
            goto AVPLAY0_STOP;
        }

        ProgNum = 0;
        Ret = HIADP_AVPlay_PlayProg(g_hAvplay[0], g_pProgTbl, ProgNum, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_AVPlay_SwitchProg failed.\n");
            HI_UNF_SND_Detach(g_hTrack[0], g_hAvplay[0]);
            HI_UNF_SND_DestroyTrack(g_hTrack[0]);
            HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
            HI_UNF_VO_DetachWindow(hWin, g_hAvplay[0]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
            goto AVPLAY0_STOP;

        }
    }
    j = 0;

    while ((g_pAudEsFile[j] != HI_NULL) && (j < MAX_AVPLAY_NUM - 1))
    {
        Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
        AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
        Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay[1 + j]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY1_STOP;
        }

        Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            HI_UNF_AVPLAY_Destroy(g_hAvplay[1 + j]);
            goto AVPLAY1_STOP;
        }

        Ret = HIADP_AVPlay_SetAdecAttr(g_hAvplay[1 + j], g_AudType[j], HD_DEC_MODE_RAWPCM, 1);
        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto AVPLAY1_STOP;
        }
        Ret |= HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &g_hTrack[1 + j]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_CreateTrack failed.\n");
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[1 + j]);
            goto AVPLAY1_STOP;
        }

        Ret = HI_UNF_SND_Attach(g_hTrack[1 + j], g_hAvplay[1 + j]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_SND_Attach failed.\n");
            HI_UNF_SND_DestroyTrack(g_hTrack[1 + j]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[1 + j]);
            goto AVPLAY1_STOP;
        }

        Ret = HI_UNF_AVPLAY_Start(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Start failed.\n");
            HI_UNF_SND_Detach(g_hTrack[1 + j], g_hAvplay[1 + j]);
            HI_UNF_SND_DestroyTrack(g_hTrack[1 + j]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[1 + j]);
            goto AVPLAY1_STOP;
        }

        g_StopAudThread = HI_FALSE;
        pthread_create(&audio_send[j], HI_NULL, (HI_VOID*)AudEsTthread, (HI_VOID*)((HI_U8*)HI_NULL + j));
        j++;
    }

    while (1)
    {
        printf("\n");
        printf("please input the q to quit!\n");
        printf("please input 'vnxxx' to change the volume of audio channel 'n' to 'xxx'!\n");
        printf("\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
        else if ('v' == InputCmd[0])
        {
            HI_U32 i, vol, k;

            if ((InputCmd[1] >= '0') && (InputCmd[1] <= '9'))
            {
                i = InputCmd[1] - '0';
            }
            else if ((InputCmd[1] >= 'a') && (InputCmd[1] <= 'f'))
            {
                i = InputCmd[1] - 'a' + 10;
            }
            else if ((InputCmd[1] >= 'A') && (InputCmd[1] <= 'F'))
            {
                i = InputCmd[1] - 'A' + 10;
            }
            else
            {
                continue;
            }

            if ((i >= MAX_AVPLAY_NUM) || (HI_INVALID_HANDLE == g_hAvplay[i]))
            {
                printf("Invalid audio channel id %d .\n", i);
                continue;
            }

            k = 2;
            vol = 0;
            while ((InputCmd[k] != 0) && (k < 32))
            {
                if ((InputCmd[k] < '0') || (InputCmd[k] > '9'))
                {
                    k++;
                    continue;
                }
                vol = vol * 10 + (InputCmd[k] - '0');
                k++;
            }
            if (vol > 100)
            {
                printf("Invalid volume %d .\n", vol);
                continue;
            }
            stGain.s32Gain = vol;
            HI_UNF_SND_SetTrackWeight(g_hTrack[i], &stGain);
            printf("Set the volume of audio channel %d to be %d\n", i, stGain.s32Gain);
        }
        else if (HI_TRUE == bDVBPlay)
        {
            ProgNum = atoi(InputCmd);
            Ret = HIADP_AVPlay_PlayProg(g_hAvplay[0], g_pProgTbl, ProgNum, HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                printf("call HIADP_AVPlay_SwitchProg!\n");
                break;
            }
        }
    }

AVPLAY1_STOP:
    g_StopAudThread = HI_TRUE;
    j = 0;
    while ((g_pAudEsFile[j] != HI_NULL) && (j < MAX_AVPLAY_NUM - 1))
    {
        pthread_join(audio_send[j], HI_NULL);
        j++;
    }
    j = 0;
    while (j < MAX_AVPLAY_NUM - 1)
    {
        if (g_hAvplay[1 + j] != HI_INVALID_HANDLE)
        {
            Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
            Stop.u32TimeoutMs = 0;
            HI_UNF_AVPLAY_Stop(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
            HI_UNF_SND_Detach(g_hTrack[1 + j], g_hAvplay[1 + j]);
            HI_UNF_SND_DestroyTrack(g_hTrack[1 + j]);
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[1 + j], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
            HI_UNF_AVPLAY_Destroy(g_hAvplay[1 + j]);
        }
        j++;
    }

AVPLAY0_STOP:
    if (HI_TRUE == bDVBPlay)
    {
        Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        Stop.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
        HI_UNF_SND_Detach(g_hTrack[0], g_hAvplay[0]);
        HI_UNF_SND_DestroyTrack(g_hTrack[0]);
        HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin, g_hAvplay[0]);
        HI_UNF_AVPLAY_ChnClose(g_hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_Destroy(g_hAvplay[0]);
    }
    else
    {
        goto AVPLAY_DEINIT;
    }

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();
SRCH_DEINIT:
    HIADP_Search_DeInit();
    HIADP_Tuner_DeInit();
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();
VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
DISP_DEINIT:
    HIADP_Disp_DeInit();
SND_DEINIT:
    HIADP_Snd_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    return 0;
}


