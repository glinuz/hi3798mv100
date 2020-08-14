/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_cgms.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
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
#include "HA.AUDIO.DOLBYTRUEHD.decode.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSPASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSM6.decode.h"
#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
 #include "HA.AUDIO.DOLBYPLUS.decode.h"
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif
FILE               *g_pVidEsFile = HI_NULL;
FILE               *g_pAudEsFile = HI_NULL;

static pthread_t g_EsThd;
static HI_BOOL g_StopThread = HI_FALSE;
static HI_BOOL g_bAudPlay = HI_FALSE;
static HI_BOOL g_bVidPlay = HI_FALSE;
static HI_BOOL g_PlayOnce = HI_FALSE;

static HI_BOOL g_bReadFrameSize = HI_FALSE;

HI_S32 g_AudEsFileOffest;

HI_VOID* EsThread(HI_VOID *args)
{
    static int counter = 0;

    HI_HANDLE hAvplay = *((HI_HANDLE*)args);
    HI_U32 FrameSize = 0;
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32 Readlen;
    HI_S32 Ret;
    HI_BOOL bVidBufAvail = HI_TRUE, bAudBufAvail = HI_TRUE;

    bVidBufAvail = HI_FALSE;
    bAudBufAvail = HI_FALSE;

    while (!g_StopThread)
    {
        if (g_bVidPlay)
        {
            if (HI_FALSE != g_bReadFrameSize)
            {
                Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x100000, &StreamBuf, 0);
            }
            else
            {
                Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x4000, &StreamBuf, 0);
            }

            if (HI_SUCCESS == Ret)
            {
                bVidBufAvail = HI_TRUE;

                if (HI_FALSE != g_bReadFrameSize)
                {
                    Readlen = fread(&FrameSize, 1, 4, g_pVidEsFile);
                    if (Readlen == 4)
                    {}
                    else
                    {
                        FrameSize = 0x4000;
                    }
                }
                else
                {
                    FrameSize = 0x4000;
                }

                Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), FrameSize, g_pVidEsFile);

                if (Readlen > 0)
                {
                    Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, 0);
                    if (Ret != HI_SUCCESS)
                    {
                        sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                    }

                    FrameSize = 0;
                }
                else if (Readlen == 0)
                {
                    if (HI_TRUE == g_PlayOnce)
                    {
                        HI_BOOL bIsEmpty = HI_FALSE;
                        HI_UNF_AVPLAY_FLUSH_STREAM_OPT_S stFlushOpt;

                        sample_printf("Esplay flush stream.\n");
                        HI_UNF_AVPLAY_FlushStream(hAvplay, &stFlushOpt);
                        do {
                            Ret = HI_UNF_AVPLAY_IsBuffEmpty(hAvplay, &bIsEmpty);
                            if (Ret != HI_SUCCESS)
                            {
                                sample_printf("call HI_UNF_AVPLAY_IsBuffEmpty failed.\n");
                                break;
                            }
                        }while(bIsEmpty != HI_TRUE);
                        sleep(5);
                        sample_printf("Finish, esplay exit!\n");
                        exit(0);
                    }
                    else
                    {
                        sample_printf("read vid file end and rewind!\n");
                        rewind(g_pVidEsFile);
                    }
                }
                else
                {
                    perror("read vid file error\n");
                }
            }
            else
            {
                bVidBufAvail = HI_FALSE;
            }
        }

        if (g_bAudPlay)
        {
            Ret = HI_UNF_AVPLAY_GetBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, 0x1000, &StreamBuf, 0);
            if (HI_SUCCESS == Ret)
            {
                bAudBufAvail = HI_TRUE;
                Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 0x1000, g_pAudEsFile);
                if (Readlen > 0)
                {
                    counter++;
                    Ret = HI_UNF_AVPLAY_PutBuf(hAvplay, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);
                    if (Ret != HI_SUCCESS)
                    {
                        sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                    }
                }
                else if (Readlen == 0)
                {
                    sample_printf("read aud file end and rewind!\n");
                    rewind(g_pAudEsFile);
                    if (g_AudEsFileOffest)
                    {
                        fseek(g_pAudEsFile, g_AudEsFileOffest, SEEK_SET);
                    }
                }
                else
                {
                    perror("read aud file error\n");
                }
            }
            else if (Ret != HI_SUCCESS)
            {
                bAudBufAvail = HI_FALSE;
            }
        }

        /* wait for buffer */
        if ((HI_FALSE == bAudBufAvail) && (HI_FALSE == bVidBufAvail))
        {
            usleep(1000 * 10);
        }
    }

    return HI_NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret, index;
    HI_HANDLE hWin;
    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    HI_UNF_VCODEC_TYPE_E VdecType = HI_UNF_VCODEC_TYPE_BUTT;
    HI_U32 AdecType = 0;
    HI_HANDLE hAvplay;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S AvSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR InputCmd[32];
    HI_HA_DECODEMODE_E enAudioDecMode = HD_DEC_MODE_RAWPCM;
    HI_S32 s32DtsDtsCoreOnly = 0;
    HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
    HI_BOOL bAdvancedProfil = 1;
    HI_U32  u32CodecVersion = 8;
    HI_U32 frame_rate = 0;

    if (argc < 5)
    {
#if 1
        printf(" usage: sample_cgms vfile vtype null null [fmt] \n");
        printf(" vtype: h264/h265/mpeg2/mpeg4/avs/real8/real9/vc1ap/vc1smp5(WMV3)/vc1smp8/vp6/vp6f/vp6a/vp8/divx3/h263/sor\n");
        printf(" fmt: 1080p_50/1080p_60/1080i_60/1080i_50/720p_50/720p_60/480p_60/576p_50/ntsc/pal_n\n");
        printf(" examples: \n");
        printf("   sample_cgms vfile h264 null null\n");
#endif
        return 0;
    }

    if (strcasecmp("null", argv[1]))
    {
        g_bVidPlay = HI_TRUE;
        if (!strcasecmp("mpeg2", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_MPEG2;
        }
        else if (!strcasecmp("mpeg4", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_MPEG4;
        }
        else if (!strcasecmp("h263", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_H263;
        }
        else if (!strcasecmp("sor", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_SORENSON;
        }
        else if (!strcasecmp("vp6", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VP6;
        }
        else if (!strcasecmp("vp6f", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VP6F;
        }
        else if (!strcasecmp("vp6a", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VP6A;
        }
        else if (!strcasecmp("h264", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_H264;
        }
        else if (!strcasecmp("h265", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_HEVC;
        }
        else if (!strcasecmp("mvc", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_MVC;
        }
        else if (!strcasecmp("avs", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_AVS;
        }
        else if (!strcasecmp("real8", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_REAL8;
        }
        else if (!strcasecmp("real9", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_REAL9;
        }
        else if (!strcasecmp("vc1ap", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_VC1;
            bAdvancedProfil = 1;
            u32CodecVersion = 8;
        }
        else if (!strcasecmp("vc1smp5", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VC1;
            bAdvancedProfil = 0;
            u32CodecVersion = 5;  //WMV3
        }
        else if (!strcasecmp("vc1smp8", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VC1;
            bAdvancedProfil = 0;
            u32CodecVersion = 8;  //not WMV3
        }
        else if (!strcasecmp("vp8", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_VP8;
        }
        else if (!strcasecmp("divx3", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_DIVX3;
        }
        else if (!strcasecmp("mvc", argv[2]))
        {
            VdecType = HI_UNF_VCODEC_TYPE_MVC;
        }
           else if (!strcasecmp("mjpeg", argv[2]))
        {
            g_bReadFrameSize = HI_TRUE;
            VdecType = HI_UNF_VCODEC_TYPE_MJPEG;
        }
        else
        {
            sample_printf("unsupport vid codec type!\n");
            return -1;
        }

        g_pVidEsFile = fopen(argv[1], "rb");
        if (!g_pVidEsFile)
        {
            sample_printf("open file %s error!\n", argv[1]);
            return -1;
        }
    }

    if (strcasecmp("null", argv[3]))
    {
        g_pAudEsFile = fopen(argv[3], "rb");
        if (!g_pAudEsFile)
        {
            sample_printf("open file %s error!\n", argv[3]);
            return -1;
        }

        if (argc > 5)
        {
            if (!strcasecmp("-mode0", argv[5]))
            {
                enAudioDecMode = HD_DEC_MODE_RAWPCM;
            }
            else if (!strcasecmp("-mode1", argv[5]))
            {
                enAudioDecMode = HD_DEC_MODE_THRU;
            }
            else if (!strcasecmp("-mode2", argv[5]))
            {
                enAudioDecMode = HD_DEC_MODE_SIMUL;
            }
        }


        g_bAudPlay = HI_TRUE;
        if (!strcasecmp("aac", argv[4]))
        {
            AdecType = HA_AUDIO_ID_AAC;
        }
        else if (!strcasecmp("mp3", argv[4]))
        {
            AdecType = HA_AUDIO_ID_MP3;
        }
        else if (!strcasecmp("truehd", argv[4]) )
        {
            AdecType = HA_AUDIO_ID_DOLBY_TRUEHD;
        }
        else if (!strcasecmp("ac3raw", argv[4]))
        {
            AdecType = HA_AUDIO_ID_AC3PASSTHROUGH;
        }
        else if (!strcasecmp("dtsraw", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DTSPASSTHROUGH;
        }
#if defined (DOLBYPLUS_HACODEC_SUPPORT)
        else if (!strcasecmp("ddp", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DOLBY_PLUS;
        }
#endif
        else if (!strcasecmp("dts", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DTSHD;
        }
        else if (!strcasecmp("dtsm6", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DTSM6;
        }
        else if (!strcasecmp("dra", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DRA;
        }
        else if (!strcasecmp("pcm", argv[4]))
        {
            AdecType = HA_AUDIO_ID_PCM;
        }
        else if (!strcasecmp("mlp", argv[4]))
        {
            AdecType = HA_AUDIO_ID_TRUEHD;
        }
        else if (!strcasecmp("amr", argv[4]))
        {
            AdecType = HA_AUDIO_ID_AMRNB;
        }
        else if (!strcasecmp("amrwb", argv[4]))
        {
            AdecType = HA_AUDIO_ID_AMRWB;
#if 1
            {
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
            }
#endif
        }
        else
        {
            sample_printf("unsupport aud codec type!\n");
            return -1;
        }
    }

    for (index = 0; index < argc; index++)
    {
        if (strcasestr("1080p_50", argv[index]))
        {
            sample_printf("sample use 1080p_50 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_50;
            break;
        }
        else if (strcasestr("1080p_60", argv[index]))
        {
            sample_printf("sample use 1080p_60 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;
            break;
        }
        else if (strcasestr("1080i_60", argv[index]))
        {
            sample_printf("sample use 1080i_60 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_60;
            break;
        }
        else if (strcasestr("1080i_50", argv[index]))
        {
            sample_printf("sample use 1080i_50 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
            break;
        }
        else if (strcasestr("720p_50", argv[index]))
        {
            sample_printf("sample use 720p_50 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
            break;
        }
        else if (strcasestr("720p_60", argv[index]))
        {
            sample_printf("sample use 720p_60 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_720P_60;
            break;
        }
        else if (strcasestr("480p_60", argv[index]))
        {
            sample_printf("sample use 480p_60 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_480P_60;
            break;
        }
        else if (strcasestr("576p_50", argv[index]))
        {
            sample_printf("sample use 576p_60 output\n");
            g_enDefaultFmt = HI_UNF_ENC_FMT_576P_50;
            break;
        }
        else if (strcasestr("ntsc", argv[index]))
        {
            sample_printf("sample use ntsc output\n");
			g_enDefaultFmt = HI_UNF_ENC_FMT_NTSC; 
			break;
		}
        else if (strcasestr("pal_n", argv[index]))
        {
            sample_printf("sample use pal_n output\n");
			g_enDefaultFmt = HI_UNF_ENC_FMT_PAL_N; 
			break;
		}
        else if (strcasestr("-once", argv[index]))
        {
            sample_printf("Play once only.\n");
            g_PlayOnce = HI_TRUE;
        }
        else if (strcasestr("-fps", argv[index]))
        {
            if((frame_rate = atoi(argv[index+1])) < 0)
            {
                sample_printf("Invalid fps.\n");
                return -1;
            }
        }
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto SND_DEINIT;
    }

    if (g_bAudPlay)
    {
        Ret = HIADP_Snd_Init();
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call SndInit failed.\n");
            goto VO_DEINIT;
        }

        Ret = HIADP_AVPlay_RegADecLib();
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_RegisterAcodecLib failed.\n");
            goto SND_DEINIT;
        }
    }

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);

    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &AvSyncAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto AVPLAY_DEINIT;
    }

    if (g_bAudPlay)
    {
        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_VSTOP;
        }

        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto ACHN_CLOSE;
        }
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
        Ret = HIADP_AVPlay_SetAdecAttr(hAvplay, AdecType, enAudioDecMode, s32DtsDtsCoreOnly);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto SND_DETACH;
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
        }
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DispInit failed.\n");
        goto SYS_DEINIT;
    }

    if (g_bVidPlay)
    {
        Ret  = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
        Ret |= HIADP_VO_CreatWin(HI_NULL, &hWin);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call VoInit failed.\n");
            HIADP_VO_DeInit();
            goto DISP_DEINIT;
        }
    }

    if (g_bVidPlay)
    {
        HI_UNF_AVPLAY_OPEN_OPT_S   *pMaxCapbility = HI_NULL;
        HI_UNF_AVPLAY_OPEN_OPT_S    stMaxCapbility;

        if (HI_UNF_VCODEC_TYPE_MVC == VdecType)
        {
            stMaxCapbility.enCapLevel      = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
            stMaxCapbility.enDecType       = HI_UNF_VCODEC_DEC_TYPE_BUTT;
            stMaxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_MVC;

            pMaxCapbility = &stMaxCapbility;
        }

        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, pMaxCapbility);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_DESTROY;
        }

        /*set compress attr*/
        HI_UNF_VCODEC_ATTR_S VcodecAttr;
        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VcodecAttr);

        if (HI_UNF_VCODEC_TYPE_VC1 == VdecType)
        {
            VcodecAttr.unExtAttr.stVC1Attr.bAdvancedProfile = bAdvancedProfil;
            VcodecAttr.unExtAttr.stVC1Attr.u32CodecVersion = u32CodecVersion;
        }

        if (HI_UNF_VCODEC_TYPE_VP6 == VdecType)
        {
            VcodecAttr.unExtAttr.stVP6Attr.bReversed = 0;
        }

        VcodecAttr.enType = VdecType;

        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VcodecAttr);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto AVPLAY_DEINIT;
        }
        Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
            goto VCHN_CLOSE;
        }

        Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
            goto WIN_DETATCH;
        }

        Ret = HIADP_AVPlay_SetVdecAttr(hAvplay, VdecType, HI_UNF_VCODEC_MODE_NORMAL);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HIADP_AVPlay_SetVdecAttr failed.\n");
            goto WIN_DETATCH;
        }

        if (0 != frame_rate)
        {
           HI_UNF_AVPLAY_FRMRATE_PARAM_S stFramerate;
           stFramerate.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_USER;
           stFramerate.stSetFrmRate.u32fpsInteger = frame_rate;
           stFramerate.stSetFrmRate.u32fpsDecimal = 0;
           HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFramerate);
        }

        Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
            goto WIN_DETATCH;
        }
    }

    if (g_pVidEsFile == NULL)
    {
        HI_UNF_DISP_BG_COLOR_S BgColor;

        BgColor.u8Red   = 0;
        BgColor.u8Green = 200;
        BgColor.u8Blue  = 200;
        HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1, &BgColor);
    }

    g_StopThread = HI_FALSE;
    pthread_create(&g_EsThd, HI_NULL, EsThread, (HI_VOID*)&hAvplay);

    while (1)
    {
        printf("please input the q to quit!, s to toggle spdif pass-through, h to toggle hdmi pass-through\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        if ('a' == InputCmd[0] && '1' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_FREELY;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY0,&stCgmsCfg);

			continue;
		}

        if ('a' == InputCmd[0] && '2' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY0,&stCgmsCfg);

			continue;
		}

        if ('a' == InputCmd[0] && '3' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_ONCE;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY0,&stCgmsCfg);

			continue;
		}
		
        if ('a' == InputCmd[0] && '4' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NEVER;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY0,&stCgmsCfg);

			continue;
		}

        if ('a' == InputCmd[0] && '0' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_FALSE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NEVER;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY0,&stCgmsCfg);

			continue;
		}

        if ('b' == InputCmd[0] && '1' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_FREELY;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY1,&stCgmsCfg);

			continue;
		}

        if ('b' == InputCmd[0] && '2' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY1,&stCgmsCfg);

			continue;
		}

        if ('b' == InputCmd[0] && '3' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_ONCE;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY1,&stCgmsCfg);

			continue;
		}
		
        if ('b' == InputCmd[0] && '4' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_TRUE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NEVER;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY1,&stCgmsCfg);

			continue;
		}

        if ('b' == InputCmd[0] && '0' == InputCmd[1])
		{
			HI_UNF_DISP_CGMS_CFG_S stCgmsCfg;

			stCgmsCfg.bEnable = HI_FALSE;
			stCgmsCfg.enType = HI_UNF_DISP_CGMS_TYPE_A;
			stCgmsCfg.enMode = HI_UNF_DISP_CGMS_MODE_COPY_NEVER;

			HI_UNF_DISP_SetCgms(HI_UNF_DISPLAY1,&stCgmsCfg);

			continue;
		}

        //FOR MVC TEST
        if ('3' == InputCmd[0] && 'd' == InputCmd[1]  )
        {
            HI_UNF_DISP_Set3DMode(HI_UNF_DISPLAY1,
            HI_UNF_DISP_3D_FRAME_PACKING,
            HI_UNF_ENC_FMT_1080P_24_FRAME_PACKING);
        }

        if (('s' == InputCmd[0]) || ('S' == InputCmd[0]))
        {
            static int spdif_toggle = 0;
            spdif_toggle++;
            if (spdif_toggle & 1)
            {
                HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_RAW);
                sample_printf("spdif pass-through on!\n");
            }
            else
            {
                HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_LPCM);
                sample_printf("spdif pass-through off!\n");
            }
        }


        if ('h' == InputCmd[0] || 'H' == InputCmd[0])
        {
            static int hdmi_toggle =0;
            hdmi_toggle++;
            if(hdmi_toggle&1)
            {
                HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_RAW);
                printf("hmdi pass-through on!\n");
            }
            else
            {
                HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_LPCM);
                printf("hmdi pass-through off!\n");
            }
        }
    }

    g_StopThread = HI_TRUE;
    pthread_join(g_EsThd, HI_NULL);

    if (g_bAudPlay)
    {
        Stop.u32TimeoutMs = 0;
        Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    }

SND_DETACH:
    if (g_bAudPlay)
    {
        HI_UNF_SND_Detach(hTrack, hAvplay);
    }

TRACK_DESTROY:
    if (g_bAudPlay)
    {
        HI_UNF_SND_DestroyTrack(hTrack);
    }

ACHN_CLOSE:
    if (g_bAudPlay)
    {
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    }

AVPLAY_VSTOP:
    if (g_bVidPlay)
    {
        Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        Stop.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);
    }

WIN_DETATCH:
    if (g_bVidPlay)
    {
        HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
    }

VCHN_CLOSE:
    if (g_bVidPlay)
    {
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    }

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

SND_DEINIT:
    if (g_bAudPlay)
    {
        HIADP_Snd_DeInit();
    }

VO_DEINIT:
    if (g_bVidPlay)
    {
        HI_UNF_VO_DestroyWindow(hWin);
        HIADP_VO_DeInit();
    }

DISP_DEINIT:
    HIADP_Disp_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();
    if (g_bVidPlay)
    {
        fclose(g_pVidEsFile);
        g_pVidEsFile = HI_NULL;
    }

    if (g_bAudPlay)
    {
        fclose(g_pAudEsFile);
        g_pAudEsFile = HI_NULL;
    }

    return 0;
}
