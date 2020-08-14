/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ts_snapshot.c
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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#endif

#include "hi_adp.h"
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

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define MAX_FILENAME_LEN        256

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

#ifdef ANDROID 


HI_VOID ScreenToJpeg(HI_CHAR *FileName)
{
    HI_S32                      ret;
    HI_UNF_VIDEO_FRAME_INFO_S   VidFrame;

    if (!FileName)
    {
        return;
    }

    ret = HI_UNF_DISP_AcquireSnapshot(HI_UNF_DISPLAY1, &VidFrame);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_UNF_DISP_AcquireSnapshot failed 0x%x\n", __FUNCTION__, ret);

        return;
    }

    ret = HIADP_OSD_EncodeFrame(&VidFrame, 1, FileName);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HIADP_OSD_EncodeFrame failed 0x%x\n", __FUNCTION__, ret);

        return;
    }
    
    ret = HI_UNF_DISP_ReleaseSnapshot(HI_UNF_DISPLAY1, &VidFrame);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_UNF_DISP_ReleaseSnapshot failed 0x%x\n", __FUNCTION__, ret);
    }
}
#else
HI_S32 HigoCreateSurface(HI_U32 Width, HI_U32 Height, HI_HANDLE *Layer, HI_HANDLE *Surface)
{
    HI_S32              ret;
    HIGO_LAYER_INFO_S   LayerInfo;

    if ((HI_NULL == Layer) || (HI_NULL == Surface))
    {
        return HI_FAILURE;
    }

    *Layer      = HI_NULL;
    *Surface    = HI_NULL;

    ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &LayerInfo);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_GetLayerDefaultParam failed 0x%x\n", __FUNCTION__, ret);

        goto exit;
    }

    LayerInfo.CanvasWidth   = Width;
    LayerInfo.CanvasHeight  = Height;
    LayerInfo.DisplayWidth  = Width;
    LayerInfo.DisplayHeight = Height;

    ret = HI_GO_CreateLayer(&LayerInfo, Layer);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_CreateLayer failed 0x%x\n", __FUNCTION__, ret);

        goto exit;
    }

    ret = HI_GO_GetLayerSurface(*Layer, Surface);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_GetLayerSurface failed 0x%x\n", __FUNCTION__, ret);

        goto exit;
    }

    ret = HI_GO_SetLayerAlpha(*Layer, 100);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_SetLayerAlpha failed 0x%x\n", __FUNCTION__, ret);

        goto exit;
    }

    return HI_SUCCESS;

exit:
    if (*Layer)
    {
        HI_GO_DestroyLayer(*Layer);
        *Layer = HI_NULL;
    }

    return HI_FAILURE;
}

HI_VOID HigoDestroySurface(HI_HANDLE Layer)
{
    HI_S32 ret;

    ret = HI_GO_DestroyLayer(Layer);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_DestroyLayer failed 0x%x\n", __FUNCTION__, ret);
    }
}

HI_S32 HigoFillRect(HI_HANDLE Layer, HI_HANDLE Surface, HI_RECT *Rect)
{
    HI_S32 ret;

    ret = HI_GO_FillRect(Surface, Rect, 0xff00ffff, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_FillRect failed 0x%x\n", __FUNCTION__, ret);

        return HI_FAILURE;
    }

    ret = HI_GO_RefreshLayer(Layer, HI_NULL);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_GO_RefreshLayer failed 0x%x\n", __FUNCTION__, ret);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID ScreenToPicture(HIGO_IMGTYPE_E ImgType, HI_CHAR *FileName)
{
    HI_S32                      ret;
    HI_UNF_VIDEO_FRAME_INFO_S   VidFrame;
    HI_HANDLE                   MemSurface;
    HIGO_SURINFO_S              MemSurfaceInfo = {0};
    HIGO_ENC_ATTR_S             EncAttr;

    if (!FileName)
    {
        sample_printf("[%s] filename error\n", __FUNCTION__);
        return;
    }

    switch (ImgType)
    {
        case HIGO_IMGTYPE_JPEG :
        case HIGO_IMGTYPE_BMP :
            break;

        default :
            sample_printf("[%s] not support format 0x%x\n", __FUNCTION__, ImgType);
            return;
    }

    ret = HI_UNF_DISP_AcquireSnapshot(HI_UNF_DISPLAY1, &VidFrame);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_UNF_DISP_AcquireSnapshot failed 0x%x\n", __FUNCTION__, ret);

        return;
    }

    MemSurfaceInfo.Width        = VidFrame.u32Width;
    MemSurfaceInfo.Height       = VidFrame.u32Height;
    MemSurfaceInfo.PixelFormat  = HIGO_PF_BUTT;

    switch (VidFrame.enVideoFormat)
    {
        case HI_UNF_FORMAT_YUV_SEMIPLANAR_422 :
            MemSurfaceInfo.PixelFormat = HIGO_PF_YUV422;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_420 :
            MemSurfaceInfo.PixelFormat = HIGO_PF_YUV420;
            break;

        case HI_UNF_FORMAT_YUV_SEMIPLANAR_400 :
            MemSurfaceInfo.PixelFormat = HIGO_PF_YUV400;
            break;

        default :
            sample_printf("[%s] not support format %d\n", __FUNCTION__, VidFrame.enVideoFormat);
    }

    if (HIGO_PF_BUTT != MemSurfaceInfo.PixelFormat)
    {
        MemSurfaceInfo.Pitch[0]     = VidFrame.stVideoFrameAddr[0].u32YStride;
        MemSurfaceInfo.Pitch[1]     = VidFrame.stVideoFrameAddr[0].u32CStride;
        MemSurfaceInfo.pPhyAddr[0]  = (HI_CHAR*)HI_NULL + VidFrame.stVideoFrameAddr[0].u32YAddr;
        MemSurfaceInfo.pPhyAddr[1]  = (HI_CHAR*)HI_NULL + VidFrame.stVideoFrameAddr[0].u32CAddr;
        MemSurfaceInfo.pVirAddr[0]  = HI_NULL;
        MemSurfaceInfo.pVirAddr[1]  = HI_NULL;
        MemSurfaceInfo.MemType      = HIGO_MEMTYPE_MMZ;
        MemSurfaceInfo.bPubPalette  = HI_FALSE;

        ret = HI_GO_CreateSurfaceFromMem(&MemSurfaceInfo, &MemSurface);
        if (HI_SUCCESS != ret)
        {
            sample_printf("[%s] HI_GO_CreateSurface failed 0x%x\n", __FUNCTION__, ret);
        }

        EncAttr.ExpectType      = ImgType;
        EncAttr.QualityLevel    = 99;

        ret = HI_GO_EncodeToFile(MemSurface, FileName, &EncAttr);
        if (HI_SUCCESS != ret)
        {
            sample_printf("[%s] HI_GO_EncodeToFile failed 0x%x\n", __FUNCTION__, ret);
        }
        else
        {
            sample_printf("snapshot to %s\n", FileName);
        }

        ret = HI_GO_FreeSurface(MemSurface);
        if (HI_SUCCESS != ret)
        {
            sample_printf("[%s] HI_GO_FreeSurface failed 0x%x\n", __FUNCTION__, ret);
        }
    }

    ret = HI_UNF_DISP_ReleaseSnapshot(HI_UNF_DISPLAY1, &VidFrame);
    if (HI_SUCCESS != ret)
    {
        sample_printf("[%s] HI_UNF_DISP_ReleaseSnapshot failed 0x%x\n", __FUNCTION__, ret);
    }
}
#endif

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                      Ret;
    HI_HANDLE                   hWin;
    HI_HANDLE                   hAvplay;
    HI_HANDLE                   Layer;
    HI_HANDLE                   LayerSurface;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                     InputCmd[32];
    HI_UNF_ENC_FMT_E            enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32                      ProgNum;
    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;
    HI_CHAR                     FileName[MAX_FILENAME_LEN];
    HI_U32                      Count       = 1;
    HI_U32                      VirWidth    = 1280;
    HI_U32                      VirHeight   = 720;
#ifndef ANDROID 
    HIGO_IMGTYPE_E              ImgType     = HIGO_IMGTYPE_BMP;
    HI_RECT                     Rect;
#endif

    if (argc < 2)
    {
        sample_printf("Usage: %s file [vo_format] [enc_format]\n", argv[0]);
        sample_printf("\tvo_format: 2160P_30 | 2160P_24 | 1080P_60 | 1080P_50 | 1080i_60 | 1080i_50 | 720P_60 | 720P_50\n");
        sample_printf("\tenc_format: bmp | jpg\n");
        sample_printf("Example:\n\t%s ./test.ts 1080i_50\n", argv[0]);

        return 0;
    }

    if (argc >= 3)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[2]);
    }

#ifndef ANDROID 
    if (argc >= 4)
    {
        if (strcasestr(argv[3], "jpg"))
        {
            ImgType = HIGO_IMGTYPE_JPEG;
        }
        else if (strcasestr(argv[3], "bmp"))
        {
            ImgType = HIGO_IMGTYPE_BMP;
        }
    }
#endif

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
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID, HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto DMX_DEINIT;
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
        sample_printf("call RegADecLib failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DMX_ID;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed:%#x.\n",Ret);
        goto ACHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
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

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
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
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

#ifndef ANDROID 
    Ret = HI_GO_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("[%s] HI_GO_Init failed 0x%x\n", __FUNCTION__, Ret);
        goto SND_DETACH;
    }
#endif

    Ret = HI_UNF_DISP_GetVirtualScreen(HI_UNF_DISPLAY1, &VirWidth, &VirHeight);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("HI_UNF_DISP_GetVirtualScreen failed 0x%x\n", Ret);
    }

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
    Ret = HigoCreateSurface(VirWidth, VirHeight, &Layer, &LayerSurface);
    if (Ret != HI_SUCCESS)
    {
        goto HIGO_DEINIT;
    }

    Rect.w = 200;
    Rect.h = 200;
    Rect.x = VirWidth - Rect.w;
    Rect.y = VirHeight - Rect.h;

    HigoFillRect(Layer, LayerSurface, &Rect);
#endif

    pthread_mutex_init(&g_TsMutex,NULL);
    g_bStopTsThread = HI_FALSE;
    pthread_create(&g_TsThd, HI_NULL, (HI_VOID *)TsTthread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID, &g_pProgTbl);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed.\n");
        goto PSISI_FREE;
    }

    ProgNum = 0;

    pthread_mutex_lock(&g_TsMutex);
    rewind(g_pTsFile);
    HI_UNF_DMX_ResetTSBuffer(g_TsBuf);

    Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_AVPlay_PlayProg failed!\n");
        goto AVPLAY_STOP;
    }

    pthread_mutex_unlock(&g_TsMutex);

    sample_printf("please input q to quit\n");
    sample_printf("       input 0~%u to change channel\n", g_pProgTbl->prog_num - 1);
    sample_printf("       input s to snapshot\n");

    while (1)
    {
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        if ('s' == InputCmd[0])
        {
#ifdef ANDROID
			sprintf(FileName, "snapshot_%u.jpg", Count++);
			ScreenToJpeg(FileName);
#else		
            if (HIGO_IMGTYPE_JPEG == ImgType)
            {
                sprintf(FileName, "snapshot_%u.jpg", Count++);
            }
            else if (HIGO_IMGTYPE_BMP == ImgType)
            {
                sprintf(FileName, "snapshot_%u.bmp", Count++);
            }

            ScreenToPicture(ImgType, FileName);
#endif
            continue;
        }

        ProgNum = atoi(InputCmd);
        if (ProgNum == 0)
            ProgNum = 1;

        pthread_mutex_lock(&g_TsMutex);
        rewind(g_pTsFile);
        HI_UNF_DMX_ResetTSBuffer(g_TsBuf);

        Ret = HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum - 1, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HIADP_AVPlay_PlayProg failed!\n");
            break;
        }

        pthread_mutex_unlock(&g_TsMutex);
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

HIGO_DEINIT:
#ifndef ANDROID
    HI_GO_Deinit();
#endif

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
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

ACHN_CLOSE:
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


