/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_audio_play.c
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
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
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
#include "hi_drv_ao.h"


#if defined (DOLBYPLUS_HACODEC_SUPPORT)
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif
static FILE* g_pAudEsFile[(HI_U32)HI_UNF_SND_BUTT];

static pthread_t g_EsThd[(HI_U32)HI_UNF_SND_BUTT];
static HI_HANDLE hAvplay[(HI_U32)HI_UNF_SND_BUTT];
static HI_S32 g_AudEsFileOffest[(HI_U32)HI_UNF_SND_BUTT];

static HI_BOOL g_StopThread = HI_FALSE;

static HI_VOID AudioTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32 Readlen;
    HI_S32 Ret;
    HI_BOOL bAudBufAvail = HI_FALSE;
    HI_U32 u32Snd = (HI_U8*)args - (HI_U8*)HI_NULL;

    while (!g_StopThread)
    {
        Ret = HI_UNF_AVPLAY_GetBuf(hAvplay[u32Snd], HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x1000, &StreamBuf, 0);
        if (HI_SUCCESS == Ret)
        {
            bAudBufAvail = HI_TRUE;
            Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 0x1000, g_pAudEsFile[u32Snd]);
            if (Readlen > 0)
            {
                Ret = HI_UNF_AVPLAY_PutBuf(hAvplay[u32Snd], HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                }
            }
            else if (Readlen <= 0)
            {
                sample_printf("read aud file error!\n");
                rewind(g_pAudEsFile[u32Snd]);
                if (g_AudEsFileOffest[u32Snd])
                {
                    fseek(g_pAudEsFile[u32Snd], g_AudEsFileOffest[u32Snd], SEEK_SET);
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
    HI_U32 u32SndNum;
    HI_HANDLE hTrack[(HI_U32)HI_UNF_SND_BUTT];
#if 0   //v1r3
    HI_HANDLE hAef[(HI_U32)HI_UNF_SND_BUTT];
#ifdef MULTI_AEF
    HI_HANDLE hAef1;
#endif
#endif
    HI_U32 u32Snd;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
    HI_UNF_SND_ATTR_S stAttr;

    HI_U32 AdecType[(HI_U32)HI_UNF_SND_BUTT];
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S AvSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR InputCmd[32];
    HI_U32 u32InVal = 0;
    HI_HA_DECODEMODE_E enAudioDecMode = HD_DEC_MODE_RAWPCM;
    HI_S32 s32DtsDtsCoreOnly = 0;
    HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
#if 0   //v1r3
    HI_AEF_BASE_OPEN_PARAM_S stOpenDfParam;
#endif
    if (argc < 3)
    {
        sample_printf(" usage: ./sample_audioplay afile1 atype1\n"
                      "        ./sample_audioplay afile1 atype1 afile2 atype2\n"
                      "        ./sample_audioplay afile1 atype1 afile2 atype2 afile3 atype3\n");
        sample_printf(" atype: aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3)\n");
        sample_printf(" examples: \n");
        sample_printf("        ./sample_audioplay afile1 mp3 afile2 aac afile3 ddp\n");
        return -1;
    }
    else if(argc < 5)
    {
        u32SndNum = 1;
    }
    else if(argc < 7)
    {
        u32SndNum = 2;
    }
    else
    {
        u32SndNum = 3;
    }

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        g_pAudEsFile[u32Snd] = fopen(argv[2*u32Snd+1], "rb");
        if (!g_pAudEsFile[u32Snd])
        {
            sample_printf("open file %s error!\n", argv[2*u32Snd+1]);
            return -1;
        }
    }

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        if (!strcasecmp("aac", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_AAC;
        }
        else if (!strcasecmp("mp3", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_MP3;
        }

        else if (!strcasecmp("ac3raw", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_AC3PASSTHROUGH;
        }
        else if (!strcasecmp("dtsraw", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_DTSPASSTHROUGH;
        }
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
        else if (!strcasecmp("ddp", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_DOLBY_PLUS;
        }
#endif
        else if (!strcasecmp("dts", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_DTSHD;
        }
        else if (!strcasecmp("dra", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_DRA;
        }
        else if (!strcasecmp("pcm", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_PCM;
        }
        else if (!strcasecmp("mlp", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_TRUEHD;
        }
        else if (!strcasecmp("amr", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_AMRNB;
        }
        else if (!strcasecmp("amrwb", argv[2*(u32Snd + 1)]))
        {
            AdecType[u32Snd] = HA_AUDIO_ID_AMRWB;
#if 1
            /*read header of file for MIME file format */
            extern HI_U8 u8DecOpenBuf[1024];
            AMRWB_DECODE_OPENCONFIG_S *pstConfig = (AMRWB_DECODE_OPENCONFIG_S *)u8DecOpenBuf;
            if (pstConfig->enFormat == AMRWB_FORMAT_MIME)
            {
                HI_U8 u8MagicBuf[8];

                fread(u8MagicBuf, sizeof(HI_S8), strlen(AMRWB_MAGIC_NUMBER), g_pAudEsFile[u32Snd]);/* just need by amr file storage format (xxx.amr) */
                if (strncmp((const char *)u8MagicBuf, AMRWB_MAGIC_NUMBER, strlen(AMRWB_MAGIC_NUMBER)))
                {
                    sample_printf( "%s Invalid amr wb file magic number ", u8MagicBuf);
                    return HI_FAILURE;
                }
            }
            g_AudEsFileOffest[u32Snd] = strlen(AMRWB_MAGIC_NUMBER);
#endif
        }
        else
        {
            sample_printf("unsupport aud codec type!\n");
            return -1;
        }
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

    if(u32SndNum == 1)
    {
        HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &stAttr);
        Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &stAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_Open failed.\n");
            goto SND_CLOSE;
        }
    }

    if(u32SndNum == 2)
    {
        HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &stAttr);
        stAttr.u32PortNum = 2;
        stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
        stAttr.stOutport[1].enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
        Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &stAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_Open failed.\n");
            goto SND_CLOSE;
        }
        HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_1, &stAttr);
        stAttr.u32PortNum = 1;
        stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
        Ret = HI_UNF_SND_Open(HI_UNF_SND_1, &stAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_Open failed.\n");
            goto SND_CLOSE;
        }
    }

    if(u32SndNum == 3)
    {
        for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
        {
            HI_UNF_SND_GetDefaultOpenAttr(u32Snd, &stAttr);
            stAttr.u32PortNum = 1;
            if(HI_UNF_SND_0 == u32Snd)
            {
                stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_DAC0;
            }
            else if(HI_UNF_SND_1 == u32Snd)
            {
                stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
            }
            else if(HI_UNF_SND_2 == u32Snd)
            {
                stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
            }
            Ret = HI_UNF_SND_Open(u32Snd, &stAttr);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_SND_Open failed.\n");
                goto SND_CLOSE;
            }
        }
    }
#if 0   //v1r3
    Ret = HIADP_Snd_RegAefAuthLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_RegisterAefAuthLib failed.\n");
        goto SND_CLOSE;
    }
#endif
    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_RegisterAcodecLib failed.\n");
        goto SND_CLOSE;
    }

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay[u32Snd]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY_DESTROY;
        }
        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay[u32Snd], HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
        AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay[u32Snd], HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto AVPLAY_DESTROY;
        }
    }

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[u32Snd], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
          sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
          goto ACHN_CLOSE;
        }
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        Ret = HI_UNF_SND_CreateTrack(u32Snd,&stTrackAttr,&hTrack[u32Snd]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
            goto TRACK_DESTROY;
        }
    }

#if 0   //v1r3
    HI_AEF_BASE_GetDefaultOpenParams(&stOpenDfParam);
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        Ret = HI_UNF_SND_CreateAef(u32Snd, HI_UNF_SND_AEF_TYPE_BASE, (HI_VOID *)(&stOpenDfParam), &hAef[u32Snd]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_CreateAef failed.\n");
            goto AEF_DESTROY;
        }
        Ret = HI_UNF_SND_SetAefEnable(hAef[u32Snd], HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_SetAefEnable failed.\n");
            goto AEF_DISABLE;
        }
#ifdef MULTI_AEF
         Ret = HI_UNF_SND_CreateAef(u32Snd, HI_UNF_SND_AEF_TYPE_BASE, (HI_VOID *)(&stOpenDfAttr), &hAef1);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_CreateAef failed.\n");
            goto AEF_DESTROY;
        }
        Ret = HI_UNF_SND_SetAefEnable(hAef1, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_SetAefEnable failed.\n");
            goto AEF_DISABLE;
        }
#endif
    }
#endif

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        Ret = HI_UNF_SND_Attach(hTrack[u32Snd], hAvplay[u32Snd]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_Attach failed.\n");
            goto SND_DETACH;
        }
        Ret = HIADP_AVPlay_SetAdecAttr(hAvplay[u32Snd], AdecType[u32Snd], enAudioDecMode, s32DtsDtsCoreOnly);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HIADP_AVPlay_SetAdecAttr failed.\n");
            goto SND_DETACH;
        }
        Ret = HI_UNF_AVPLAY_Start(hAvplay[u32Snd], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
        }
    }


    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto SND_DETACH;
    }

    g_StopThread = HI_FALSE;
    for(u32Snd = 0; u32Snd < u32SndNum; u32Snd++)
    {
        pthread_create(&g_EsThd[u32Snd], HI_NULL, (HI_VOID*)AudioTthread, (HI_VOID*)((HI_U8*)HI_NULL + u32Snd));
    }

    while (1)
    {
        sample_printf("\nplease input the q to quit!, s to toggle spdif pass-through, h to toggle hdmi mode\n"
                      "mh/s/a/l to toggle hdmi/spdif/adac/all mute, uh/s/a/l to toggle hdmi/spdif/adac/all unmute\n"
                      "please input the tn to toggle trackMode, v to decrease volume, e to increase volume\n"
#if 0   //v1r3
                      "wd to decrease track weight, wi to increase track weight\n"
#endif
                      "p to toggle pause, o to toggle stop/start, r to toggle reset\n\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        static HI_UNF_SND_E enSnd = HI_UNF_SND_0;
        if (('s' == InputCmd[0]) || ('S' == InputCmd[0]))
        {
            static int spdif_toggle = 0;
            if(u32SndNum == 1 || u32SndNum == 2)
            {
                enSnd = HI_UNF_SND_0;
            }
            else if(u32SndNum == 3)
            {
                enSnd = HI_UNF_SND_1;
            }
            spdif_toggle++;
            if (spdif_toggle & 1)
            {
                HI_UNF_SND_SetSpdifMode(enSnd, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_RAW);
                sample_printf("spdif pass-through on!\n");
            }
            else
            {
                HI_UNF_SND_SetSpdifMode(enSnd, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_LPCM);
                sample_printf("spdif pass-through off!\n");
            }
        }

        if (('h' == InputCmd[0]) || ('H' == InputCmd[0]))
        {
            static int hdmi_toggle = 0;
            if(u32SndNum == 1)
            {
                enSnd = HI_UNF_SND_0;
            }
            else if(u32SndNum == 2)
            {
                enSnd = HI_UNF_SND_1;
            }
            else if(u32SndNum == 3)
            {
                enSnd = HI_UNF_SND_2;
            }
            hdmi_toggle++;
            if(hdmi_toggle >= HI_UNF_SND_HDMI_MODE_BUTT)
            {
                hdmi_toggle = HI_UNF_SND_HDMI_MODE_LPCM;
            }

            HI_UNF_SND_SetHdmiMode(enSnd, HI_UNF_SND_OUTPUTPORT_HDMI0, hdmi_toggle);
            sample_printf("hdmi mode %d!\n", hdmi_toggle);
        }

        if (('t' == InputCmd[0]) || ('T' == InputCmd[0]))
        {
            static HI_U32 trackmode_toggle[(HI_U32)HI_UNF_SND_BUTT] = {0};
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            trackmode_toggle[u32InVal]++;
            if(trackmode_toggle[u32InVal] > 7)
            {
                trackmode_toggle[u32InVal] = 0;
            }

            HI_UNF_SND_SetTrackMode((HI_UNF_SND_E)u32InVal, HI_UNF_SND_OUTPUTPORT_ALL, trackmode_toggle[u32InVal]);
            sample_printf("Snd %d trackmode %d!\n", u32InVal, trackmode_toggle[u32InVal]);
        }

        if (('m' == InputCmd[0]) || ('u' == InputCmd[0]))
        {
            static HI_UNF_SND_OUTPUTPORT_E enPort;
            if('a' == InputCmd[1])
            {
                enPort = HI_UNF_SND_OUTPUTPORT_DAC0;
            }
            else if('s' == InputCmd[1])
            {
                enPort = HI_UNF_SND_OUTPUTPORT_SPDIF0;
            }
            else if('h' == InputCmd[1])
            {
                enPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
            }
            else if('l' == InputCmd[1])
            {
                enPort = HI_UNF_SND_OUTPUTPORT_ALL;
            }
            else
            {
                sample_printf("Invalid port!\n");
                continue;
            }

            u32InVal = atoi(&InputCmd[2]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            if ('m' == InputCmd[0])
            {
                HI_UNF_SND_SetMute((HI_UNF_SND_E)u32InVal, enPort, HI_TRUE);
                sample_printf("Snd %d Mute on!\n", u32InVal);
            }
            else if('u' == InputCmd[0])
            {
                HI_UNF_SND_SetMute((HI_UNF_SND_E)u32InVal, enPort, HI_FALSE);
                sample_printf("Snd %d Mute off!\n", u32InVal);
            }
        }

        static HI_UNF_SND_GAIN_ATTR_S volume_toggle[(HI_U32)HI_UNF_SND_BUTT] = {{HI_TRUE, 100},{HI_TRUE, 100},{HI_TRUE, 100}};

        if ('v' == InputCmd[0])
        {
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }
            volume_toggle[u32InVal].s32Gain -= 10;
            if(volume_toggle[u32InVal].s32Gain < 0)
            {
                volume_toggle[u32InVal].s32Gain = 0;
            }
            HI_UNF_SND_SetVolume((HI_UNF_SND_E)u32InVal, HI_UNF_SND_OUTPUTPORT_ALL, &volume_toggle[u32InVal]);
            sample_printf("Snd %d volume[%d]!\n", u32InVal, volume_toggle[u32InVal].s32Gain);
        }

        if ('V' == InputCmd[0])
        {
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }
            volume_toggle[u32InVal].s32Gain = 0;
            HI_UNF_SND_SetVolume((HI_UNF_SND_E)u32InVal, HI_UNF_SND_OUTPUTPORT_ALL, &volume_toggle[u32InVal]);
            sample_printf("Snd %d volume[%d]!\n", u32InVal, volume_toggle[u32InVal].s32Gain);
        }

        if ('e' == InputCmd[0])
        {
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }
            volume_toggle[u32InVal].s32Gain += 10;
            if(volume_toggle[u32InVal].s32Gain > 100)
            {
                volume_toggle[u32InVal].s32Gain = 100;
            }
            HI_UNF_SND_SetVolume((HI_UNF_SND_E)u32InVal, HI_UNF_SND_OUTPUTPORT_ALL, &volume_toggle[u32InVal]);
            sample_printf("Snd %d volume[%d]!\n", u32InVal, volume_toggle[u32InVal].s32Gain);
        }

        if ('E' == InputCmd[0])
        {
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }
            volume_toggle[u32InVal].s32Gain = 100;
            HI_UNF_SND_SetVolume((HI_UNF_SND_E)u32InVal, HI_UNF_SND_OUTPUTPORT_ALL, &volume_toggle[u32InVal]);
            sample_printf("Snd %d volume[%d]!\n", u32InVal, volume_toggle[u32InVal].s32Gain);
        }

        static HI_UNF_SND_ABSGAIN_ATTR_S trackgain_toggle[] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
        if ('w' == InputCmd[0] || 'W' == InputCmd[0])
        {
            if('d' == InputCmd[1])
            {
                u32InVal = atoi(&InputCmd[2]);
                trackgain_toggle[u32InVal].s32GainL -= 1;
                trackgain_toggle[u32InVal].s32GainR -= 1;
                if(trackgain_toggle[u32InVal].s32GainL < AO_MIN_ABSOLUTEVOLUMEEXT)
                {
                    trackgain_toggle[u32InVal].s32GainL = AO_MIN_ABSOLUTEVOLUMEEXT;
                    trackgain_toggle[u32InVal].s32GainR = AO_MIN_ABSOLUTEVOLUMEEXT;
                }
                HI_UNF_SND_SetTrackAbsWeight((HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | u32InVal, &trackgain_toggle[u32InVal]);
                sample_printf("track %d gain[%d]!\n", u32InVal, trackgain_toggle[u32InVal].s32GainL);
            }
            if ('D' == InputCmd[1])
            {
                u32InVal = atoi(&InputCmd[2]);
                trackgain_toggle[u32InVal].s32GainL = -40;
                trackgain_toggle[u32InVal].s32GainR = -40;
                HI_UNF_SND_SetTrackAbsWeight((HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | u32InVal, &trackgain_toggle[u32InVal]);
                sample_printf("track %d gain[%d]!\n", u32InVal, trackgain_toggle[u32InVal].s32GainL);
            }
            if('i' == InputCmd[1])
            {
                u32InVal = atoi(&InputCmd[2]);
                trackgain_toggle[u32InVal].s32GainL += 1;
                trackgain_toggle[u32InVal].s32GainR += 1;
                if(trackgain_toggle[u32InVal].s32GainL > AO_MAX_ABSOLUTEVOLUMEEXT)
                {
                    trackgain_toggle[u32InVal].s32GainL = AO_MAX_ABSOLUTEVOLUMEEXT;
                    trackgain_toggle[u32InVal].s32GainR = AO_MAX_ABSOLUTEVOLUMEEXT;
                }
                HI_UNF_SND_SetTrackAbsWeight((HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | u32InVal, &trackgain_toggle[u32InVal]);
                sample_printf("track %d gain[%d]!\n", u32InVal, trackgain_toggle[u32InVal].s32GainL);
            }
            if ('I' == InputCmd[1])
            {
                u32InVal = atoi(&InputCmd[2]);
                trackgain_toggle[u32InVal].s32GainL = 0;
                trackgain_toggle[u32InVal].s32GainR = 0;
                HI_UNF_SND_SetTrackAbsWeight((HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | u32InVal, &trackgain_toggle[u32InVal]);
                sample_printf("track %d gain[%d]!\n", u32InVal, trackgain_toggle[u32InVal].s32GainL);
            }
        }

        if (('p' == InputCmd[0]) || ('P' == InputCmd[0]))
        {
            static HI_U32 pause_toggle[(HI_U32)HI_UNF_SND_BUTT] = {0};
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            pause_toggle[u32InVal]++;
            if (pause_toggle[u32InVal] & 1)
            {
                HI_UNF_AVPLAY_Pause(hAvplay[u32InVal], HI_NULL);
                sample_printf("Track of snd %d pause on!\n", u32InVal);
            }
            else
            {
                HI_UNF_AVPLAY_Resume(hAvplay[u32InVal], HI_NULL);
                sample_printf("Track of snd %d pause off!\n", u32InVal);
            }
        }

        if (('o' == InputCmd[0]) || ('O' == InputCmd[0]))
        {
            static HI_U32 stop_toggle[(HI_U32)HI_UNF_SND_BUTT] = {0};
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            stop_toggle[u32InVal]++;
            if (stop_toggle[u32InVal] & 1)
            {
                HI_UNF_AVPLAY_Stop(hAvplay[u32InVal], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
                //HI_MPI_AO_Track_Stop(hTrack[u32InVal]);
                sample_printf("Track of snd %d stop!\n", u32InVal);
            }
            else
            {
                HI_UNF_AVPLAY_Start(hAvplay[u32InVal], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
                //HI_MPI_AO_Track_Start(hTrack[u32InVal]);
                sample_printf("Track of snd %d start!\n", u32InVal);
            }
        }
        if (('r' == InputCmd[0]) || ('R' == InputCmd[0]))
        {
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            HI_UNF_AVPLAY_Reset(hAvplay[u32InVal], HI_NULL);
            sample_printf("Track of snd %d reset!\n", u32InVal);
        }
#if 0   //v1r3
        if (('b' == InputCmd[0]) || ('B' == InputCmd[0]))
        {
            #if 0
            static HI_U32 bypass_toggle[(HI_U32)HI_UNF_SND_BUTT] = {0};
            u32InVal = atoi(&InputCmd[1]);
            if(u32InVal >=  u32SndNum)
            {
                sample_printf("snd %d don't open!\n", u32InVal);
                continue;
            }

            HI_UNF_SND_SetAefBypass(hAef[u32InVal], HI_NULL);
            sample_printf("Track of snd %d reset!\n", u32InVal);
            #else
            static HI_U32 bypass_toggle[HI_UNF_SND_OUTPUTPORT_ARC0+1] = {0};
            u32InVal = atoi(&InputCmd[1]);
            bypass_toggle[u32InVal]++;
            if(bypass_toggle[u32InVal] & 1)
            {
                HI_UNF_SND_SetAefBypass(HI_UNF_SND_0, (HI_UNF_SND_OUTPUTPORT_E)u32InVal, HI_TRUE);
                sample_printf("enport %d aef bypass\n", u32InVal);
            }
            else
            {
                HI_UNF_SND_SetAefBypass(HI_UNF_SND_0, (HI_UNF_SND_OUTPUTPORT_E)u32InVal, HI_FALSE);
                sample_printf("enport %d aef not bypass\n", u32InVal);
            }
            #endif
        }
#endif
    }

    g_StopThread = HI_TRUE;
    for(u32Snd = 0; u32Snd < u32SndNum; u32Snd++)
    {
        pthread_join(g_EsThd[u32Snd], HI_NULL);
    }

    Stop.u32TimeoutMs = 0;
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_AVPLAY_Stop(hAvplay[u32Snd], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    }

    HIADP_Disp_DeInit();

SND_DETACH:
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_SND_Detach(hTrack[u32Snd], hAvplay[u32Snd]);
    }
#if 0   //v1r3
AEF_DISABLE:
#ifdef MULTI_AEF
    HI_UNF_SND_SetAefEnable(hAef1, HI_FALSE);
#endif
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_SND_SetAefEnable(hAef[u32Snd], HI_FALSE);
    }

AEF_DESTROY:
#ifdef MULTI_AEF
    HI_UNF_SND_DestroyAef(hAef1);
#endif
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_SND_DestroyAef(hAef[u32Snd]);
    }
#endif
TRACK_DESTROY:
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_SND_DestroyTrack(hTrack[u32Snd]);
    }

ACHN_CLOSE:
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_AVPLAY_ChnClose(hAvplay[u32Snd], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    }

AVPLAY_DESTROY:
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_AVPLAY_Destroy(hAvplay[u32Snd]);
    }

SND_CLOSE:
    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        HI_UNF_SND_Close(u32Snd);
    }

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    for(u32Snd = 0;u32Snd < u32SndNum; u32Snd++)
    {
        fclose(g_pAudEsFile[u32Snd]);
        g_pAudEsFile[u32Snd] = HI_NULL;
    }

    return 0;
}
