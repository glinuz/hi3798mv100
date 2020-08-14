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
#include <sys/stat.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_mce.h"
#include "hi_unf_common.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_avplay.h"
#include "hi_unf_vo.h"
#include "hi_unf_pdm.h"
#include "hi_unf_demux.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
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

HI_U32              g_TunerFreq;
HI_U32              g_TunerSrate;
HI_U32              g_ThirdParam;
HI_UNF_ENC_FMT_E    g_enDefaultFmt = HI_UNF_ENC_FMT_720P_50;

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#define PLAY_DEMUX_ID 0
#define PLAY_TUNER_ID 0

HI_S32 MceDispInit(HI_UNF_ENC_FMT_E enFormat)
{
    HI_S32                  Ret;
    HI_UNF_DISP_BG_COLOR_S  BgColor;
    HI_UNF_DISP_INTF_S      DispIntf;

    Ret = HI_UNF_DISP_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DISP_Init failed.\n");
        return Ret;
    }

    BgColor.u8Red = 0;
    BgColor.u8Green = 0;
    BgColor.u8Blue = 0;
    HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY0, &BgColor);
    HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1, &BgColor);
    HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY1, enFormat);  

    if ((HI_UNF_ENC_FMT_1080P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_60 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_480P_60 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, HI_UNF_ENC_FMT_NTSC);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_DISP_SetFormat failed.\n");
            HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
            HI_UNF_DISP_DeInit();
            return Ret;
        }
    }

    if ((HI_UNF_ENC_FMT_1080P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_50 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_576P_50 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, HI_UNF_ENC_FMT_PAL);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_DISP_SetFormat failed.\n");
            HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
            HI_UNF_DISP_DeInit();
            return Ret;
        }
    }

#ifdef HI_DAC_CVBS
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_CVBS;
    DispIntf.unIntf.stCVBS.u8Dac = HI_DAC_CVBS;
    HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY0, &DispIntf, 1);
#endif

#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    DispIntf.unIntf.stYPbPr.u8DacY  = HI_DAC_YPBPR_Y;
    DispIntf.unIntf.stYPbPr.u8DacPb = HI_DAC_YPBPR_PB;
    DispIntf.unIntf.stYPbPr.u8DacPr = HI_DAC_YPBPR_PR;
    HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
#endif

    Ret = HI_UNF_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DISP_Attach failed.\n");
        HI_UNF_DISP_DeInit();
        return Ret;
    }
    
    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DISP_Open failed.\n");
        HI_UNF_DISP_DeInit();
        return Ret;
    }
    
#ifndef S_SUPPORT_4PLUS64
    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DISP_Open SD failed.\n");
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    Ret = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0);
    if (HI_SUCCESS != Ret)    
    {
        printf("call HI_UNF_DISP_Open SD failed.\n");
        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    return HI_SUCCESS;
}

HI_VOID LogoTransToGrc(HI_VOID)
{
#ifndef ANDROID
    HIGO_LAYER_INFO_S           stLayerInfoHD;
    HI_HANDLE                   hLayerHD,hLayerSurfaceHD;
#endif    
    HI_UNF_PDM_DISP_PARAM_S     stDispParam;

    HI_SYS_Init();
    
    HI_UNF_MCE_Init(HI_NULL);

    HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);

    MceDispInit(stDispParam.enFormat);

#ifdef ANDROID

    HI_HANDLE               hSurface;
    HI_RECT_S               stRect;
    HIADP_SURFACE_ATTR_S    stSurAttr;

    HIADP_OSD_Init();

    stSurAttr.u32Width = 1280;
    stSurAttr.u32Height = 720;
    stSurAttr.enPixelFormat = HIADP_PF_8888;
    HIADP_OSD_CreateSurface(&stSurAttr, &hSurface);  
    
    stRect.s32X = 0;
    stRect.s32Y = 0;
    stRect.s32Width = 1280;
    stRect.s32Height = 720;
    HIADP_OSD_FillRect(hSurface, &stRect, 0xff0000ff);

    HIADP_OSD_Refresh(hSurface);

    printf("press any key to exit logo play!\n");
    getchar();

    HIADP_OSD_DestroySurface(hSurface);

    HIADP_OSD_DeInit();

#else
    HI_GO_Init();
	HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfoHD);

	stLayerInfoHD.DisplayWidth = stDispParam.u32VirtScreenWidth;
	stLayerInfoHD.DisplayHeight =  stDispParam.u32VirtScreenHeight;

    stLayerInfoHD.PixelFormat = stDispParam.enPixelFormat;

    #if  defined (CHIP_TYPE_hi3712)
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    #else
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    #endif 
    stLayerInfoHD.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;
    stLayerInfoHD.LayerID = HIGO_LAYER_HD_0;
    
    HI_GO_CreateLayer(&stLayerInfoHD,&hLayerHD);

    HI_GO_GetLayerSurface(hLayerHD,&hLayerSurfaceHD); 

    HI_GO_FillRect(hLayerSurfaceHD,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);

    HI_GO_RefreshLayer(hLayerHD, NULL);

    printf("press any key to exit logo play!\n");
    getchar();

    HI_GO_DestroyLayer(hLayerHD);
    HI_GO_Deinit();
#endif   

    HIADP_Disp_DeInit();
    HI_UNF_MCE_DeInit();    
    HI_SYS_DeInit();

    return;
}

HI_VOID LogoTransToVid(HI_VOID)
{
    HI_S32                      Ret;
    HI_HANDLE                   hWin;
    HI_HANDLE                   hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                     InputCmd[32];
    HI_U32                      ProgNum;
    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
    HI_UNF_PDM_DISP_PARAM_S     stDispParam;

    HI_SYS_Init();

    HI_UNF_MCE_Init(HI_NULL);
    
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);

    Ret = MceDispInit(stDispParam.enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }
    
    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
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

    Ret = HIADP_Tuner_Connect(PLAY_TUNER_ID, g_TunerFreq, g_TunerSrate, g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DEMUX_ID, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
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
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
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
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    ProgNum = 0;
    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    HI_UNF_MCE_ClearLogo();

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

    	if(ProgNum == 0)
    	    ProgNum = 1;

		Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
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
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_UNF_MCE_DeInit();
    HI_SYS_DeInit();

    return;
}

HI_VOID PlayTransToGrc(HI_VOID)
{
#ifndef ANDROID
    HIGO_LAYER_INFO_S           stLayerInfoHD;
    HI_HANDLE                   hLayerHD,hLayerSurfaceHD;
#endif
    HI_UNF_PDM_DISP_PARAM_S     stDispParam;
    HI_UNF_MCE_STOPPARM_S       stStop;

    HI_SYS_Init();
    
    HI_UNF_MCE_Init(HI_NULL);

    HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);

    MceDispInit(stDispParam.enFormat);

#ifdef ANDROID

    HI_HANDLE               hSurface;
    HI_RECT_S               stRect;
    HIADP_SURFACE_ATTR_S    stSurAttr;

    HIADP_OSD_Init();

    stSurAttr.u32Width = 1280;
    stSurAttr.u32Height = 720;
    stSurAttr.enPixelFormat = HIADP_PF_8888;
    HIADP_OSD_CreateSurface(&stSurAttr, &hSurface);  
    
    stRect.s32X = 0;
    stRect.s32Y = 0;
    stRect.s32Width = 1280;
    stRect.s32Height = 720;
    HIADP_OSD_FillRect(hSurface, &stRect, 0xff0000ff);

    HIADP_OSD_Refresh(hSurface);

    printf("press any key to exit logo play!\n");
    getchar();

    HIADP_OSD_DestroySurface(hSurface);

    HIADP_OSD_DeInit();

#else
    HI_GO_Init();
    
	HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfoHD);

    #if  defined (CHIP_TYPE_hi3712)
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    #else
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    #endif 
    stLayerInfoHD.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;
    stLayerInfoHD.LayerID = HIGO_LAYER_HD_0;
    
    HI_GO_CreateLayer(&stLayerInfoHD,&hLayerHD);

    HI_GO_GetLayerSurface(hLayerHD,&hLayerSurfaceHD); 

    HI_GO_FillRect(hLayerSurfaceHD,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);

    HI_GO_RefreshLayer(hLayerHD, NULL);

    stStop.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stStop.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_TIME;
    stStop.u32PlayTimeMs = 0;
    HI_UNF_MCE_Stop(&stStop);

    HI_UNF_MCE_Exit(HI_NULL);
    
    printf("press any key to exit logo play!\n");
    getchar();

    HI_GO_DestroyLayer(hLayerHD);
    HI_GO_Deinit();
#endif

    HIADP_Disp_DeInit();
    HI_UNF_MCE_DeInit();
    HI_SYS_DeInit();

    return;
}

HI_VOID PlayTransToVid(HI_VOID)
{
    HI_S32                      Ret;
    HI_HANDLE                   hWin;
    HI_HANDLE                   hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                     InputCmd[32];
    HI_U32                      ProgNum;
    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
    HI_UNF_MCE_STOPPARM_S       stMceStop;
    HI_UNF_MCE_EXITPARAM_S      stMceExit;
    HI_UNF_PDM_DISP_PARAM_S     stDispParam;

    HI_SYS_Init();

    HI_UNF_MCE_Init(HI_NULL);

    stMceStop.enCtrlMode = HI_UNF_MCE_PLAYCTRL_BY_TIME;
    stMceStop.enStopMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stMceStop.u32PlayTimeMs = 0;
    HI_UNF_MCE_Stop(&stMceStop);
    
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);

    Ret = MceDispInit(stDispParam.enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }
    
    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
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

    Ret = HIADP_Tuner_Connect(TUNER_USE,g_TunerFreq,g_TunerSrate,g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
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

    stMceExit.hNewWin = hWin;
    HI_UNF_MCE_Exit(&stMceExit);    
    
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
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
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DEMUX_ID, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    ProgNum = 0;
    Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
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

    	if(ProgNum == 0)
    	    ProgNum = 1;

		Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
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
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_UNF_MCE_DeInit();
    HI_SYS_DeInit();

    return;

}

static HI_VOID PrintHelp(HI_VOID)
{
    printf("Usage: sample_mce_trans transtype [freq] [srate] [qamtype or polarization]\n");
    printf("       transtype: 1:logo->graphics, 2:logo->video, 3:mce->graphics, 4:mce->dvb \n");
    printf("       example: sample_mce_trans 1 \n");
    printf("       example: sample_mce_trans 2 3840 27500 0\n");
    printf("       example: sample_mce_trans 3 \n");
    printf("       example: sample_mce_trans 4 3840 27500 0\n");
    return;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_U32                          TransType = 0;
   
    if(argc > 5 || argc < 2)
    {
        PrintHelp();
        return 0;
    }  

    TransType = atoi(argv[1]);

    if(TransType > 4)
    {
        PrintHelp();
        return 0;    
    }

    if (2 == TransType || 4 == TransType)
    {
        if (5 == argc)
        {
            g_TunerFreq  = strtol(argv[2],NULL,0);
            g_TunerSrate = strtol(argv[3],NULL,0);
            g_ThirdParam = strtol(argv[4],NULL,0);
        }
        else if (4 == argc)
        {
            g_TunerFreq  = strtol(argv[2],NULL,0);
            g_TunerSrate = strtol(argv[3],NULL,0);
            g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        }
        else if(3 == argc)
        {
            g_TunerFreq  = strtol(argv[2],NULL,0);
            g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
            g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;

        }
        else
        {
            PrintHelp();
            return 0;         
        }
    }

    if (1 == TransType)
    {
        LogoTransToGrc();
    }
    else if (2 == TransType)
    {
        LogoTransToVid();
    }
    else if (3 == TransType)
    {
        PlayTransToGrc();
    }
    else if (4 == TransType)
    {
        PlayTransToVid();
    }

    
    return HI_SUCCESS;
}


