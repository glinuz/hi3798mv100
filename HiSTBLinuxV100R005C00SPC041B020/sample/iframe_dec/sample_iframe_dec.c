/**
\file
\brief I frame decode Sample
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author x57522
\date 2010-8-31
*/

/***************************** Macro Definition ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_unf_avplay.h"
#include "hi_unf_demux.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_unf_disp.h"

#ifdef ANDROID
#include <utils/Log.h>
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#endif

#ifdef ANDROID
static HI_HANDLE g_hSurface;
#endif

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define PLAY_DEMUX_ID 0
#define PLAY_TUNER_ID 0

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32 prepare_Iframe(HI_CHAR *pFileName,HI_S32 Protocol,HI_UNF_AVPLAY_I_FRAME_S *pIFrame)
{
    HI_S32 Ret;
    HI_S32 IFrameFd,ReadLen;
    struct stat FileStat;

    switch (Protocol)
    {
        case 0 :
            pIFrame->enType = HI_UNF_VCODEC_TYPE_MPEG2;
            break;
        case 1 :
            pIFrame->enType = HI_UNF_VCODEC_TYPE_MPEG4;
            break;
        case 2 :
            pIFrame->enType = HI_UNF_VCODEC_TYPE_AVS;
            break;
        case 3 :
            pIFrame->enType = HI_UNF_VCODEC_TYPE_H264;
            break;
        default:
            sample_printf("unsupported protocol\n");
            return HI_FAILURE;
    }

    Ret = stat(pFileName,&FileStat);
    if (HI_SUCCESS != Ret || 0 == FileStat.st_size)
    {
        perror("stat");
        return HI_FAILURE;
    }
    pIFrame->u32BufSize = FileStat.st_size;

    pIFrame->pu8Addr = (HI_U8*)malloc(pIFrame->u32BufSize);
    if (HI_NULL == pIFrame->pu8Addr)
    {
        perror("malloc");
        return HI_FAILURE;
    }

    IFrameFd = open(pFileName,O_RDONLY);
    if (HI_NULL == IFrameFd)
    {
        perror("open");
        free(pIFrame->pu8Addr);
        pIFrame->pu8Addr = HI_NULL;
        return HI_FAILURE;
    }

    ReadLen = read(IFrameFd,pIFrame->pu8Addr,pIFrame->u32BufSize);
    if (ReadLen < pIFrame->u32BufSize)
    {
        perror("read");
        close(IFrameFd);
        free(pIFrame->pu8Addr);
        pIFrame->pu8Addr = HI_NULL;
        return HI_FAILURE;
    }

    close(IFrameFd);
    return HI_SUCCESS;
}

HI_VOID release_Iframe(HI_UNF_AVPLAY_I_FRAME_S *pIFrame)
{
    if (HI_NULL != pIFrame->pu8Addr)
    {
        free(pIFrame->pu8Addr);
        pIFrame->pu8Addr = HI_NULL;
    }
    pIFrame->u32BufSize = 0;
}

HI_S32  prepare_avplay(HI_HANDLE *phAvplay,HI_HANDLE *phWin, HI_HANDLE *phTrack)
{
    HI_S32 Ret;
    HI_HANDLE hWin,hAvplay;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S   SyncAttr;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        return HI_FAILURE;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
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
    Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
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
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, phTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(*phTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    *phAvplay = hAvplay;
    *phWin = hWin;
    return HI_SUCCESS;

SND_DETACH:
    HI_UNF_SND_Detach(HI_UNF_SND_0, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(*phTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    HI_UNF_AVPLAY_Destroy(hAvplay);
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

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
    return Ret;
}

HI_S32  release_avplay(HI_HANDLE hAvplay,HI_HANDLE hWin, HI_HANDLE hTrack)
{
    HI_UNF_SND_Detach(hTrack, hAvplay);
    HI_UNF_SND_DestroyTrack(hTrack);
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, hAvplay);
    HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    HI_UNF_AVPLAY_Destroy(hAvplay);
    HI_UNF_AVPLAY_DeInit();
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HIADP_Snd_DeInit();
    return HI_SUCCESS;
}

HI_S32 receive_prog(HI_S32 TunerFreq,HI_S32 TunerSrate,HI_U32 ThirdParam,PMT_COMPACT_TBL **ppProgTbl)
{
    HI_S32 Ret;

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Init failed.\n");
        return HI_FAILURE;
    }

    Ret = HIADP_Tuner_Connect(PLAY_TUNER_ID, TunerFreq, TunerSrate, ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(PLAY_DEMUX_ID, ppProgTbl);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }
    return HI_SUCCESS;

PSISI_FREE:
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();
    return Ret;
}

HI_S32 release_prog(PMT_COMPACT_TBL *pProgTbl)
{
    HIADP_Search_FreeAllPmt(pProgTbl);
    HIADP_Search_DeInit();
    HIADP_Tuner_DeInit();
    return HI_SUCCESS;
}

HI_S32  prepare_higo(HI_HANDLE *phLayer)
{
    HI_S32 Ret;

#ifdef ANDROID

    HIADP_SURFACE_ATTR_S    stSurAttr;

    stSurAttr.u32Width = 1280;
    stSurAttr.u32Height = 720;
    stSurAttr.enPixelFormat = HIADP_PF_8888;
    Ret = HIADP_OSD_CreateSurface(&stSurAttr, &g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

#else
    HIGO_LAYER_INFO_S stLayerInfo ;
    HI_HANDLE hLayer;

    Ret  = HI_GO_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_GO_Init failed:%#x\n",Ret);
        return HI_FAILURE;
    }

    HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    stLayerInfo.LayerID =  HIGO_LAYER_HD_0;
    stLayerInfo.ScreenWidth = 1280;
    stLayerInfo.CanvasHeight = 720;
    #if  defined (CHIP_TYPE_hi3712)
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    #endif
    Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("-->HI_GO_CreateLayer failed Ret = 0x%x line %d \n",Ret,__LINE__);
        HI_GO_Deinit();
        return HI_FAILURE;
    }
    *phLayer = hLayer;

    Ret = HI_GO_SetLayerAlpha(hLayer,0);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_GO_SetLayerAlpha failed\n");
        HI_GO_DestroyLayer(hLayer);
        HI_GO_Deinit();
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}


HI_S32  process_picture(HI_HANDLE hLayer,HI_UNF_VIDEO_FRAME_INFO_S *pCapPic)
{
    HI_S32 Ret;

#ifdef ANDROID

    Ret = HIADP_OSD_BlitFrameToSurface(pCapPic, g_hSurface, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = HIADP_OSD_Refresh(g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }
#else

    HI_HANDLE hLayerSurface,hMemSurface;
    HIGO_SURINFO_S    stSurInfo;
    HIGO_BLTOPT2_S    stBltOpt;
    #if 0
    sample_printf("=======IFrame Picture Info=============\n"
            "u32TopYStartAddr:%x\n"
            "u32TopCStartAddr:%x\n"
            "u32BottomYStartAddr:%x\n"
            "u32BottomCStartAddr:%x\n"
            "u32Width:%d\n"
            "u32Height:%d\n"
            "u32Stride:%d\n"
            "enVideoFormat:%d\n"
            "enFieldMode:%d\n"
            "enSampleType:%d\n",
            pCapPic->u32TopYStartAddr,
            pCapPic->u32TopCStartAddr,
            pCapPic->u32BottomYStartAddr,
            pCapPic->u32BottomCStartAddr,
            pCapPic->u32Width,
            pCapPic->u32Height,
            pCapPic->u32Stride,
            pCapPic->enVideoFormat,
            pCapPic->enFieldMode,
            pCapPic->enSampleType);
    #endif
    memset(&stSurInfo,0,sizeof(stSurInfo));
    stSurInfo.Width = pCapPic->u32Width;
    stSurInfo.Height = pCapPic->u32Height;
    stSurInfo.PixelFormat = HIGO_PF_YUV420;
    stSurInfo.pPhyAddr[0] = (HI_CHAR*)((HI_CHAR *)HI_NULL + pCapPic->stVideoFrameAddr[0].u32YAddr);
    stSurInfo.pPhyAddr[1] = (HI_CHAR*)((HI_CHAR *)HI_NULL + pCapPic->stVideoFrameAddr[0].u32CAddr);
    stSurInfo.Pitch[0] = pCapPic->stVideoFrameAddr[0].u32YStride;
    stSurInfo.Pitch[1] = pCapPic->stVideoFrameAddr[0].u32YStride;
    stSurInfo.pVirAddr[0] = (HI_CHAR*)((HI_CHAR *)HI_NULL + pCapPic->stVideoFrameAddr[0].u32YAddr);
    stSurInfo.pVirAddr[1] = (HI_CHAR*)((HI_CHAR *)HI_NULL + pCapPic->stVideoFrameAddr[0].u32CAddr);

    Ret= HI_GO_CreateSurfaceFromMem(&stSurInfo, &hMemSurface);
    if (HI_SUCCESS!= Ret)
    {
        sample_printf("\n HI_GO_CreateSurfaceFromMem failed\n");
        return HI_FAILURE;
    }

    Ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("-->HI_GO_GetLayerSurface failed Ret = 0x%x line %d \n",Ret,__LINE__);
        goto OUT;
    }

    memset(&stBltOpt,0,sizeof(HIGO_BLTOPT2_S));
    Ret = HI_GO_StretchBlit(hMemSurface,HI_NULL,hLayerSurface,HI_NULL,&stBltOpt);
    if (HI_SUCCESS!= Ret)
    {
        sample_printf("\n HI_GO_StretchBlit failed\n");
        goto OUT;
    }

    Ret = HI_GO_RefreshLayer(hLayer, HI_NULL);
    if (HI_SUCCESS!= Ret)
    {
        sample_printf("\n HI_GO_FreeSurface failed\n");
        HI_GO_DestroyLayer(hLayer);
        goto OUT;
    }

OUT:
    Ret |= HI_GO_FreeSurface(hMemSurface);
    if (HI_SUCCESS!= Ret)
    {
        sample_printf("\n HI_GO_FreeSurface failed\n");
    }
#endif

    return Ret;
}

HI_S32 resume_osd(HI_HANDLE hLayer)
{
    HI_S32 Ret;

#ifdef ANDROID

    Ret = HIADP_OSD_ClearSurface(g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = HIADP_OSD_Refresh(g_hSurface);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

#else
    HI_HANDLE hLayerSurface;

    Ret = HI_GO_SetLayerAlpha(hLayer,0);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_GO_SetLayerAlpha failed\n");
        return HI_FAILURE;
    }

    Ret = HI_GO_GetLayerSurface(hLayer,&hLayerSurface);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_GO_GetLayerSurface failed:%#x\n",Ret);
        return HI_FAILURE;
    }

    Ret = HI_GO_FillRect(hLayerSurface,HI_NULL,0xFFFFFF,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_GO_FillRect failed:%#x\n",Ret);
        return HI_FAILURE;
    }

    Ret = HI_GO_RefreshLayer(hLayer,HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

HI_S32  release_higo(HI_HANDLE hLayer)
{
#ifdef ANDROID
    HIADP_OSD_DestroySurface(g_hSurface);
#else
    HI_GO_DestroyLayer(hLayer);
    HI_GO_Deinit();
#endif

    return HI_SUCCESS;
}

HI_S32 process_Iframe(HI_HANDLE hAvplay,HI_UNF_AVPLAY_I_FRAME_S *pIframe,HI_HANDLE hLayer,HI_S32 ProcType)
{
    HI_S32 Ret;
    HI_UNF_VIDEO_FRAME_INFO_S stCapPic;

    if (0 == ProcType)
    {
        Ret = HI_UNF_AVPLAY_DecodeIFrame(hAvplay,pIframe,HI_NULL);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call DecodeIFrame failed\n");
            return HI_FAILURE;
        }
    }
    else if (1 == ProcType)
    {
#ifndef ANDROID
        Ret = HI_GO_SetLayerAlpha(hLayer,255);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_GO_SetLayerAlpha failed\n");
            return HI_FAILURE;
        }
#endif

        Ret = HI_UNF_AVPLAY_DecodeIFrame(hAvplay,pIframe,&stCapPic);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call DecodeIFrame failed\n");
            return HI_FAILURE;
        }

        Ret = process_picture(hLayer,&stCapPic);

        if (HI_SUCCESS != Ret)
        {
            return HI_FAILURE;
        }

        Ret = HI_UNF_AVPLAY_ReleaseIFrame(hAvplay, &stCapPic);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call ReleaseIFrame failed\n");
        }
    }

    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32                      Ret;
    HI_HANDLE                   hWindow,hAvplay,hLayer, hTrack;
    HI_S32                      TunerFreq = 610,TunerSrate = 6875,Protocol,ProcType;
    HI_U32                      ThirdParam = 0;
    HI_CHAR                     *pFileName;
    HI_UNF_AVPLAY_I_FRAME_S     stIFrame;
    HI_UNF_AVPLAY_STOP_OPT_S    stStopOpt;
    HI_BOOL                     bDvbPlay = HI_FALSE;
    PMT_COMPACT_TBL             *pProgTbl = HI_NULL;
    HI_CHAR                     InputCmd[32];

    if (argc < 4)
    {
        printf("usage:%s file protocol type [freq] [srate] [qamtype or polarization]\n"
                "protocol 0-mpeg2 1-mpeg4 2-avs 3-h264\n"
                "type     0-display on vo 1-display on osd \n"
                "qamtype or polarization: \n"
                "    For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "    For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n",
                argv[0]);

        return HI_FAILURE;
    }

    pFileName = argv[1];
    Protocol = atoi(argv[2]);
    ProcType = atoi(argv[3]);

    if (argc > 6)
    {
        bDvbPlay = HI_TRUE;
        TunerFreq = atoi(argv[4]);
        TunerSrate = atoi(argv[5]);
        ThirdParam = atoi(argv[6]);
    }
    else if (argc == 6)
    {
        bDvbPlay = HI_TRUE;
        TunerFreq = atoi(argv[4]);
        TunerSrate = atoi(argv[5]);
        ThirdParam = (TunerFreq>1000) ? 0 : 64;
    }
    else if (argc == 5)
    {
        bDvbPlay = HI_TRUE;
        TunerFreq = atoi(argv[4]);
        TunerSrate = (TunerFreq>1000) ? 27500 : 6875;
        ThirdParam = (TunerFreq>1000) ? 0 : 64;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = prepare_Iframe(pFileName,Protocol,&stIFrame);
    if (HI_SUCCESS != Ret)
    {
        HI_SYS_DeInit();
        return HI_FAILURE;
    }

    Ret = prepare_avplay(&hAvplay,&hWindow, &hTrack);
    if (HI_SUCCESS != Ret)
    {
        goto OUT0;
    }

    Ret = prepare_higo(&hLayer);
    if (HI_SUCCESS != Ret)
    {
        goto OUT1;
    }

    /*lock frequency, receive table, play program*/
    if (HI_TRUE == bDvbPlay)
    {
        Ret = receive_prog(TunerFreq,TunerSrate,ThirdParam,&pProgTbl);
        if (HI_SUCCESS != Ret)
        {
            goto OUT2;
        }

        Ret = HIADP_AVPlay_PlayProg(hAvplay,pProgTbl,0,HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call PlayProg failed.\n");
            goto OUT3;
        }

        while(1)
        {
            printf("please input: q - quit!\n");
            printf("              i - display i frame!\n");
            printf("              d - play dvb!\n");

            SAMPLE_GET_INPUTCMD(InputCmd);

            if ('q' == InputCmd[0])
            {
                printf("prepare to quit!\n");
                break;
            }

            if ('i' == InputCmd[0])
            {
                stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
                stStopOpt.u32TimeoutMs = 0;
                HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
                Ret = process_Iframe(hAvplay,&stIFrame,hLayer,ProcType);
                if (HI_SUCCESS != Ret)
                {
                    continue;
                }

                continue;
            }

            if ('d' == InputCmd[0])
            {
                resume_osd(hLayer);

                Ret = HI_UNF_AVPLAY_Start(hAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
                if (HI_SUCCESS != Ret)
                {
                    sample_printf("call AVPLAY_Start failed\n");
                    continue;
                }

                continue;
            }
        }
    }
    else
    {
        stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopOpt.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);

        Ret = process_Iframe(hAvplay,&stIFrame,hLayer,ProcType);
        if (HI_SUCCESS != Ret)
        {
            goto OUT2;
        }

        while (1)
        {
            printf("please input: q - quit!\n");
            SAMPLE_GET_INPUTCMD(InputCmd);

            if ('q' == InputCmd[0])
            {
                printf("prepare to quit!\n");
                break;
            }
        }
    }

OUT3:
    if (HI_TRUE == bDvbPlay)
    {
        stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopOpt.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
        release_prog(pProgTbl);
    }

OUT2:
    release_higo(hLayer);
OUT1:
    release_avplay(hAvplay,hWindow, hTrack);
OUT0:
    release_Iframe(&stIFrame);

    HI_SYS_DeInit();
    return Ret;
}

