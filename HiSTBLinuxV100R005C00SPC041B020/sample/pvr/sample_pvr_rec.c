/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pvr_rec.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : q46153
    Modification: Created file

******************************************************************************/
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_pvr.h"

#ifndef ANDROID
#include "hi_go_comm.h"
#include "hi_go_gdev.h"
#include "hi_go_bliter.h"
#include "hi_go_text.h"
#endif

/* when recording, playing the live program */
#define PVR_WITH_LIVE
#define MAX_REC_FILE_SIZE       (10*1024*1024*1024LLU)


HI_U32 g_u32TunerID = 0;
HI_U32 g_u32TunerFreq;
HI_U32 g_u32TunerRate;
HI_U32 g_u32ThirdParam;
extern HI_BOOL g_bIsRecStop;
pthread_t g_StatusThread;
HI_UNF_ENC_FMT_E g_enDefaultFmt = HI_UNF_ENC_FMT_1080P_60;
PMT_COMPACT_TBL  *g_pstProgTbl = HI_NULL;

HI_HANDLE hLayer_pvr = HI_INVALID_HANDLE;
HI_HANDLE hLayerSurface_pvr;
HI_HANDLE hFont_pvr = HI_INVALID_HANDLE;


HI_S32 DMXInitAndSearch(HI_U32 u32TunerID)
{
    HI_S32 Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_DMX_AttachTSPort(PVR_DMX_ID_LIVE, u32TunerID);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HIADP_DMX_AttachTSPort failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }

    Ret = HIADP_DMX_AttachTSPort(PVR_DMX_ID_REC, u32TunerID);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HIADP_DMX_AttachTSPort for REC failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }

    HIADP_Search_Init();
    if (g_pstProgTbl != HI_NULL)
    {
        HIADP_Search_FreeAllPmt(g_pstProgTbl);
        g_pstProgTbl = HI_NULL;
    }

    Ret = HIADP_Search_GetAllPmt(PVR_DMX_ID_REC, &g_pstProgTbl);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Search_GetAllPmt failed\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    HIADP_Search_FreeAllPmt(g_pstProgTbl);
    g_pstProgTbl = HI_NULL;

    Ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    Ret |= HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_DeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AvplayInit(HI_HANDLE hWin, HI_HANDLE *phAvplay, HI_HANDLE *phTrack)
{
    HI_S32                  Ret;
    HI_HANDLE               hAvplay;
    HI_HANDLE               hTrack;
    HI_UNF_AVPLAY_ATTR_S    AvplayAttr;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_Init failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_GetDefaultConfig failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    AvplayAttr.u32DemuxId = PVR_DMX_ID_LIVE;
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_Create failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        return Ret;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_VO_AttachWindow failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return Ret;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_UNF_SND_DestroyTrack(hTrack);
        PVR_SAMPLE_Printf("call HI_SND_Attach failed.\n");
        HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    *phAvplay = hAvplay;
    *phTrack = hTrack;
    return HI_SUCCESS;
}

HI_S32 AvplayDeInit(HI_HANDLE hAvplay, HI_HANDLE hWin, HI_HANDLE hTrack)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    Ret = HI_UNF_SND_Detach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_SND_Detach failed.\n");
    }

    HI_UNF_SND_DestroyTrack(hTrack);
	
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
	
    Ret = HI_UNF_VO_DetachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HI_UNF_VO_DetachWindow failed.ret = %#x\n", Ret);
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_Destroy(hAvplay);
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_Destroy failed.\n");
    }

    Ret = HI_UNF_AVPLAY_DeInit();
    if (Ret != HI_SUCCESS )
    {
        PVR_SAMPLE_Printf("call HI_UNF_AVPLAY_DeInit failed.\n");
    }

    return HI_SUCCESS;
}

HI_S32 OsdInit(HI_CHAR *pu8CurrentPath)
{
#ifndef ANDROID
    HI_S32 s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HIGO_TEXT_INFO_S stTextInfo;

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to init higo! ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.PixelFormat  = HIGO_PF_1555;
    stLayerInfo.ScreenWidth  = 1920;
    stLayerInfo.ScreenHeight = 1080;
    stLayerInfo.CanvasWidth = 1920;
    stLayerInfo.CanvasHeight = 1080;
    stLayerInfo.DisplayWidth  = 1280;
    stLayerInfo.DisplayHeight = 720;

    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the layer sd 0, ret = 0x%x !\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret =  HI_GO_GetLayerSurface(hLayer_pvr, &hLayerSurface_pvr);
    if  (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to get layer surface! s32Ret = 0x%x \n", s32Ret);
        return HI_FAILURE;
    }

    HI_GO_FillRect(hLayerSurface_pvr, NULL, 0x00000000, HIGO_COMPOPT_NONE);

    stTextInfo.pMbcFontFile = NULL;
    pu8CurrentPath = strncat(pu8CurrentPath, "../res/font/DroidSansFallbackLegacy.ttf", sizeof("../res/font/DroidSansFallbackLegacy.ttf"));
    stTextInfo.pSbcFontFile = pu8CurrentPath;
    stTextInfo.u32Size = 20;

    s32Ret = HI_GO_CreateTextEx(&stTextInfo,  &hFont_pvr);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to create the font:%s, ret = 0x%08x !\n", pu8CurrentPath, s32Ret);
        return HI_FAILURE;
    }
    HI_GO_RefreshLayer(hLayer_pvr, NULL);
#endif

    return HI_SUCCESS;
}

HI_S32 DrawString(HI_U32 u32TimeType, HI_CHAR *szText)
{

#ifndef ANDROID

    HI_S32 s32Ret;
    HI_RECT rc_Str[6] = {{100, 200, 150, 30},
                         {100, 230, 180, 30},
                         {100, 260, 180, 30},
                         {100, 290, 180, 30},
                         {100, 320, 180, 30},
                         {100, 350, 180, 30}};

    if (5 < u32TimeType)
    {
        PVR_SAMPLE_Printf("Out of time type!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_GO_FillRect(hLayerSurface_pvr, &rc_Str[u32TimeType], 0x80000000, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to calling HI_GO_FillRect, ret = 0x%x !\n", s32Ret);
    }
    s32Ret = HI_GO_DrawRect(hLayerSurface_pvr, &rc_Str[u32TimeType], 0xffffffff);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to calling HI_GO_DrawRect, ret = 0x%x !\n", s32Ret);
    }    
    s32Ret = HI_GO_SetTextColor(hFont_pvr, 0xff00ff00);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to calling HI_GO_SetTextColor, ret = 0x%x !\n", s32Ret);
    }
    s32Ret = HI_GO_TextOutEx(hFont_pvr, hLayerSurface_pvr, szText, &rc_Str[u32TimeType], HIGO_LAYOUT_RIGHT);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to text out char!ret = 0x%08x\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_GO_RefreshLayer(hLayer_pvr, NULL);
    if (HI_SUCCESS != s32Ret)
    {
        PVR_SAMPLE_Printf("failed to calling HI_GO_RefreshLayer, ret = 0x%x !\n", s32Ret);
    }
#else
    PVR_SAMPLE_Printf("current time:%s\n", szText);
#endif
    return HI_SUCCESS;
}

HI_VOID ConverTimeString(HI_CHAR* pBuf, HI_U32 u32BufLen, HI_U32 u32TimeInMs)
{
    HI_U32 u32Hour = 0;
    HI_U32 u32Minute = 0;
    HI_U32 u32Second = 0;
    HI_U32 u32MSecond = 0;

    u32Hour = u32TimeInMs/(60*60*1000);

    u32Minute = (u32TimeInMs - u32Hour*60*60*1000)/(60*1000);
    u32Second = (u32TimeInMs - u32Hour*60*60*1000 - u32Minute*60*1000)/1000;

    u32MSecond = (u32TimeInMs - u32Hour*60*60*1000 - u32Minute*60*1000 - u32Second*1000);

    snprintf(pBuf, u32BufLen, "%d:%02d:%02d:%04d", u32Hour, u32Minute, u32Second, u32MSecond);

    return;
}

HI_VOID *StatuThread(HI_VOID *args)
{
    HI_U32 u32RecChn = *(HI_U32 *)args;
    HI_S32 Ret;
    HI_UNF_PVR_REC_ATTR_S stRecAttr;
    HI_UNF_PVR_REC_STATUS_S RecstStatus;
    HI_UNF_PVR_FILE_ATTR_S FileStatus;
    HI_CHAR Rectimestr[40];

    while (HI_FALSE == g_bIsRecStop)
    {
        sleep(1);
        (HI_VOID)HI_UNF_PVR_RecGetChn(u32RecChn, &stRecAttr);
#ifdef ANDROID
        PVR_SAMPLE_Printf("----------------------------------\n");
#endif
        Ret = HI_UNF_PVR_RecGetStatus(u32RecChn, &RecstStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec time:%ds", RecstStatus.u32CurTimeInMs/1000);
            DrawString(1, Rectimestr);
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec len:%ds", (RecstStatus.u32EndTimeInMs-RecstStatus.u32StartTimeInMs)/1000);
            DrawString(2, Rectimestr);
        }
        else
        {
            PVR_SAMPLE_Printf("HI_UNF_PVR_RecGetStatus failed %x\n", Ret);
        }

        Ret = HI_UNF_PVR_GetFileAttrByFileName(stRecAttr.szFileName, &FileStatus);
        if (HI_SUCCESS == Ret)
        {
            snprintf(Rectimestr, sizeof(Rectimestr),"Rec size:%lld.%lldM",
                     (FileStatus.u64ValidSizeInByte)/0x100000,
                     ((FileStatus.u64ValidSizeInByte)%0x100000)/0x400);
            DrawString(3, Rectimestr);
        }
        else
        {
            PVR_SAMPLE_Printf("HI_UNF_PVR_GetFileAttrByFileName failed %x\n", Ret);
        }

#ifdef ANDROID
        PVR_SAMPLE_Printf("----------------------------------\n\n");
#endif
    }

    return NULL;
}


HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_U32                  i = 0;
    HI_U32                  u32Tuner;
    HI_U32                  u32ProgCount = 0;
    HI_U32                  aProgNum[8] = {0};        //one demux can support  creating 8 record channel
    HI_U32                  aRecChn[8];
    HI_S32                  Ret;
    HI_CHAR                 InputCmd[32];
    HI_HANDLE               hWin;
    PMT_COMPACT_PROG        *pstCurrentProgInfo;
    HI_CHAR                 aCurrentPath[256] = {0};
#ifdef PVR_WITH_LIVE
    HI_HANDLE               hAvplay;
    HI_HANDLE               hTrack;
#endif
    HI_BOOL                 bSupportTee = HI_FALSE;

    if (8 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = strtol(argv[3],NULL,0);
        g_u32ThirdParam     = strtol(argv[4],NULL,0);
        g_enDefaultFmt      = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner            = strtol(argv[6],NULL,0);
        bSupportTee         = strtol(argv[7],NULL,0);
    }
    else if (7 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = strtol(argv[3],NULL,0);
        g_u32ThirdParam     = strtol(argv[4],NULL,0);
        g_enDefaultFmt      = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner            = strtol(argv[6],NULL,0);
    }
    else if (6 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = strtol(argv[3],NULL,0);
        g_u32ThirdParam     = strtol(argv[4],NULL,0);
        g_enDefaultFmt      = HIADP_Disp_StrToFmt(argv[5]);
        u32Tuner = 0;
    }
    else if (5 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = strtol(argv[3],NULL,0);
        g_u32ThirdParam     = strtol(argv[4],NULL,0);
        g_enDefaultFmt      = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;
    }
    else if(4 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = strtol(argv[3],NULL,0);
        g_u32ThirdParam     = (g_u32TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt      = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;
    }
    else if(3 == argc)
    {
        g_u32TunerFreq      = strtol(argv[2],NULL,0);
        g_u32TunerRate      = (g_u32TunerFreq>1000) ? 27500 : 6875;
        g_u32ThirdParam     = (g_u32TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt      = HI_UNF_ENC_FMT_1080i_50;
        u32Tuner = 0;        
    }
    else
    {
        printf("Usage: %s file_path freq [srate] [qamtype or polarization] [vo_format] [u32Tuner][bSupportTee]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n"
                "       u32Tuner: value can be 0, 1, 2, 3\n"
                "       bSupportTee: 1 -- support; 0 -- not support\n",
                argv[0]);

        printf("Example: %s ./ 618 6875 64 1080i_50 0 0\n", argv[0]);
        printf("    Satellite : %s ./ 3840 27500 0 1080i_50 0 0\n", argv[0]);
        printf("    Cable     : %s ./ 618 6875 64 1080i_50 0 0\n", argv[0]);
        printf("    Terestrial: %s ./ 474 8000 64 1080i_50 0 0\n", argv[0]);
        return HI_FAILURE;
    }

    g_u32TunerID = u32Tuner;
    HI_SYS_Init();
    if (HI_FALSE != bSupportTee)
    {
        PVR_SetTvpFlag(HI_TRUE);
        PVR_SetCallbackFlag(HI_FALSE, HI_TRUE);
    }
    
    HIADP_MCE_Exit();
    
    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Demux_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Tuner_Connect(u32Tuner,g_u32TunerFreq, g_u32TunerRate, g_u32ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Tuner_Connect failed.\n");
        return Ret;
    }

    Ret = DMXInitAndSearch(u32Tuner);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        return Ret;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Snd_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Disp_DeInit failed.\n");
        return Ret;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        return Ret;
    }

    memset(aCurrentPath, 0, sizeof(aCurrentPath));
    memcpy(aCurrentPath, argv[0], (HI_U32)(strstr(argv[0], "sample_pvr_rec") - argv[0]));
    OsdInit(aCurrentPath);
    
    Ret = HI_UNF_PVR_RecInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_PVR_RecInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    PVR_SAMPLE_Printf("\nPlease input the number of program to record, each num is separated by blank:\n");

    do
    {
        sleep(1);
        SAMPLE_GET_INPUTCMD(InputCmd);
        HI_U32 u32Strlen = strlen(InputCmd);
        HI_CHAR *pTmp = InputCmd;
        for (i = 0; i < u32Strlen; i++)
        {
            if (*pTmp >= '0' && *pTmp <= '7')
            {
                aProgNum[u32ProgCount] = atoi(pTmp);
                u32ProgCount++;
            }
            else if (*pTmp == 'q')
            {
                return 0;
            }
            pTmp++;
        }

        if (u32ProgCount == 0)
        {
            PVR_SAMPLE_Printf("input invalid! pleasd input again\n");
            continue;
        }
    }while(0);

    for (i = 0; i < u32ProgCount; i++)
    {
        PVR_SAMPLE_Printf("create recording for %d\n", i);
        pstCurrentProgInfo = g_pstProgTbl->proginfo + (aProgNum[i]%g_pstProgTbl->prog_num);
        PVR_RecStart(argv[1], pstCurrentProgInfo, PVR_DMX_ID_REC, 1,0, MAX_REC_FILE_SIZE, &aRecChn[i]);
    }

#ifdef PVR_WITH_LIVE
    Ret = AvplayInit(hWin, &hAvplay, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call VoInit failed.\n");
        return Ret;
    }

    Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pstProgTbl, aProgNum[0], HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call SwitchProgfailed!\n");
        return 0;
    }
#endif

    pthread_create(&g_StatusThread, HI_NULL, StatuThread, (HI_VOID *)&aRecChn[0]);

    while (HI_FALSE == g_bIsRecStop)
    {
        PVR_SAMPLE_Printf("please input the 'q' to quit. \n");

#ifndef ANDROID
        if (!SAMPLE_GET_INPUTCMD(InputCmd))
        {
            sleep(1);
            continue;
        }
#else
        SAMPLE_GET_INPUTCMD(InputCmd);
#endif

        if ('q' == InputCmd[0])
        {
            PVR_SAMPLE_Printf("now exit!\n");
            g_bIsRecStop = HI_TRUE;
        }
        else
        {
            sleep(1);
            continue;
        }
    }

    pthread_join(g_StatusThread, HI_NULL);

    for (i = 0; i < u32ProgCount; i++)
    {
        PVR_RecStop(aRecChn[i]);
    }

    HI_UNF_PVR_RecDeInit();

#ifdef PVR_WITH_LIVE
    AvplayDeInit(hAvplay, hWin, hTrack);
#endif

    Ret = DmxDeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call DmxDeInit failed.\n");
        return Ret;
    }

    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HIADP_Snd_DeInit();
    HI_SYS_DeInit();

    return 0;
}


