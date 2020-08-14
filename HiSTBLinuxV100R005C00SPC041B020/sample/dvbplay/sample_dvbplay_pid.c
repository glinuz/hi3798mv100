/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mplayer.c
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

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#if defined(DOLBYPLUS_HACODEC_SUPPORT)
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#endif
#include "HA.AUDIO.DTSHD.decode.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                      Ret;
    HI_HANDLE                   hWin;

    HI_HANDLE                   hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                     InputCmd[32];
    HI_UNF_ENC_FMT_E            enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_UNF_VCODEC_TYPE_E        VdecType = HI_UNF_VCODEC_TYPE_BUTT;
    HI_U32                      AdecType = 0;
    HI_U32                      VPid=0,APid=0;
    HI_BOOL                     bAudPlay = HI_FALSE;
    HI_BOOL                     bVidPlay = HI_FALSE;
    HI_U32                      ProgNum;
    HI_U32                      u32TunerFreq = 0;
    HI_U32                      u32TunerSrate = 0;
    HI_U32                      u32ThirdParam = 0;
    PMT_COMPACT_TBL             pProgTbl;
    PMT_COMPACT_PROG            proginfo;

    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_U32                      Tuner = 0;
    
    if (argc < 6 || argc > 10)
    {
        printf("Usage: %s [vpid] [vtype] [apid] [atype] [freq] [srate] [qamtype or polarization] [vo_format] [tuner]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50  default HI_UNF_ENC_FMT_1080i_50\n"
                "       vtype: mpeg2/mpeg4/h263/sor/vp6/vp6f/vp6a/h264/avs/real8/real9/vc1\n"
                "       atype: aac/mp3/dts/dra/mlp/pcm/ddp(ac3/eac3)\n"
				"       Tuner: value can be 0, 1, 2, 3\n",
                argv[0]);
        printf(" examples: \n");
        printf("   %s vpid h264 \"null\" \"null\" 610 6875 64 1080i_50 0\n", argv[0]);
        printf("   %s \"null\" \"null\" apid mp3 610 6875 64 1080i_50 0\n", argv[0]);
        printf("   %s vpid h264 apid mp3 610 6875 64 1080i_50 0\n", argv[0]);
        return HI_FAILURE;
    }
    if (10 == argc)
    {
        u32TunerFreq  = strtol(argv[5],NULL,0);
        u32TunerSrate = strtol(argv[6],NULL,0);
        u32ThirdParam = strtol(argv[7],NULL,0);
        enFormat = HIADP_Disp_StrToFmt(argv[8]);
		Tuner = strtol(argv[9],NULL,0);
    }
	else if (9 == argc)
    {
        u32TunerFreq  = strtol(argv[5],NULL,0);
        u32TunerSrate = strtol(argv[6],NULL,0);
        u32ThirdParam = strtol(argv[7],NULL,0);
        enFormat = HIADP_Disp_StrToFmt(argv[8]);
        Tuner = 0;
    }
    else if (8 == argc)
    {
        u32TunerFreq  = strtol(argv[5],NULL,0);
        u32TunerSrate = strtol(argv[6],NULL,0);
        u32ThirdParam = strtol(argv[7],NULL,0);
        enFormat = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if(7 == argc)
    {
        u32TunerFreq  = strtol(argv[5],NULL,0);
        u32TunerSrate = strtol(argv[6],NULL,0);
        u32ThirdParam = (u32TunerFreq>1000) ? 0 : 64;
        enFormat = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if(6 == argc)
    {
        u32TunerFreq  = strtol(argv[5],NULL,0);
        u32TunerSrate = (u32TunerFreq>1000) ? 27500 : 6875;
        u32ThirdParam = (u32TunerFreq>1000) ? 0 : 64;
        enFormat = HI_UNF_ENC_FMT_1080i_50;
	    Tuner = 0;
    }

    if (strcasecmp("null",argv[1]))
    {
        bVidPlay = HI_TRUE;
        VPid = strtol(argv[1],NULL,0);
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
    		VdecType = HI_UNF_VCODEC_TYPE_H263;
    	}
    	else if (!strcasecmp("sor", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_SORENSON;
    	}
    	else if (!strcasecmp("vp6", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_VP6;
    	}
    	else if (!strcasecmp("vp6f", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_VP6F;
    	}
    	else if (!strcasecmp("vp6a", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_VP6A;
    	}
    	else if (!strcasecmp("h264", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_H264;
    	}
    	else if (!strcasecmp("avs", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_AVS;
    	}
    	else if (!strcasecmp("real8", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_REAL8;
    	}
    	else if (!strcasecmp("real9", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_REAL9;
    	}
    	else if (!strcasecmp("vc1", argv[2]))
    	{
    		VdecType = HI_UNF_VCODEC_TYPE_VC1;
    	}
    	else
    	{
    		sample_printf("unsupport vid codec type!\n");
    		return -1;
    	}
    }
    
    if (strcasecmp("null",argv[3]))
    {
        bAudPlay = HI_TRUE;
        APid = strtol(argv[3],NULL,0);
        if (!strcasecmp("aac", argv[4]))
        {
            AdecType = HA_AUDIO_ID_AAC;
        }   
        else if (!strcasecmp("mp3", argv[4]))
        {
            AdecType = HA_AUDIO_ID_MP3;
        }
        #if defined(DOLBYPLUS_HACODEC_SUPPORT)
        else if (!strcasecmp("ddp", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DOLBY_PLUS;
        }
        #endif
        else if (!strcasecmp("ac3", argv[4]))
        {
           AdecType = HA_AUDIO_ID_AC3PASSTHROUGH;
        }
        else if (!strcasecmp("dts", argv[4]))
        {
            AdecType = HA_AUDIO_ID_DTSHD;
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
        else
        {
            sample_printf("unsupport aud codec type!\n");
            return -1;
        }
    }
    if (!(bVidPlay ||bAudPlay))
    {
        return 0;
    }

    HI_SYS_Init();
	HIADP_MCE_Exit();
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }
    /*HI_SYS_GetPlayTime(& u32Playtime);
    sample_printf("u32Playtime = %d\n",u32Playtime);*/
    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(0, Tuner);
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

    Ret = HIADP_Tuner_Connect(Tuner, u32TunerFreq, u32TunerSrate, u32ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    Ret |= HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    } 
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }
   
    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    memset(&proginfo, 0, sizeof(PMT_COMPACT_PROG));
   
    if (bVidPlay)
    {
         proginfo.VElementNum = 1;
         proginfo.VElementPid = VPid;
         proginfo.VideoType = VdecType;
    }
    if (bAudPlay)
    {
         proginfo.AElementNum = 1;
         proginfo.AElementPid = APid;
         proginfo.AudioType = AdecType;
    }

    pProgTbl.prog_num = 1;
    pProgTbl.proginfo = &proginfo;
    g_pProgTbl = &pProgTbl;
   
    ProgNum = 0;

    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    HI_UNF_SND_SetHdmiMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, HI_UNF_SND_HDMI_MODE_LPCM);
    
    while(1)
    {
        printf("please input the q to quit!, s to toggle spdif pass-through, h to toggle hdmi pass-through,v20(set volume 20),vxx(set volume xx)\n");
        SAMPLE_GET_INPUTCMD(InputCmd);
        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }
		if ('v' == InputCmd[0])
		{
            HI_UNF_SND_GAIN_ATTR_S stGain;
            stGain.bLinearMode = HI_TRUE;
			
			stGain.s32Gain = atoi(InputCmd+1);
			if(stGain.s32Gain>100)
				stGain.s32Gain = 100;
				
			printf("Volume=%d\n",stGain.s32Gain);
			HI_UNF_SND_SetVolume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL, &stGain);
		}
		
        if ('s' == InputCmd[0] || 'S' == InputCmd[0])
        {
            static int spdif_toggle =0;
            spdif_toggle++;
            if(spdif_toggle&1)
            {
                HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_RAW);
                printf("spdif pass-through on!\n");
            }
            else
            {
                HI_UNF_SND_SetSpdifMode(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, HI_UNF_SND_SPDIF_MODE_LPCM);
                printf("spdif pass-through off!\n");
            }
            continue;
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
            continue;
        } 
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);       

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    HI_UNF_AVPLAY_Destroy(hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
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


