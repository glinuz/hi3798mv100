/**
 \file
 \brief play a mp3 file and show wave
 \copyright  Hisilicon Co., Ltd.
 \version draft
 \author j00140427
 \date 2009-5-8
 */

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
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"

#ifdef ANDROID
#include "hi_adp_osd.h"
#else
#include "hi_go.h"
#endif

#define MAX_BAND_NUM 512
#define ES_READ_BLOCK (1024 * 4)

HI_U16 u16Datatmp[MAX_BAND_NUM] = {0};
HI_U32 g_u32BandNum = 0;

#define WIN_X     100
#define WIN_Y     50
#define WIN_WIDTH 512
#define WIN_HEIGHT 400
#define MAX_SPEC_DATA 96

HI_BOOL s_bStopSendStream = HI_FALSE;
FILE *fp = NULL;
HI_UNF_SND_E enSound = HI_UNF_SND_0;


HI_VOID draw_spectrum_win(HI_HANDLE hSurface, HI_HANDLE hLayer,HI_U16 *pSpectrum, HI_U32 u32BandNum)
{
    HI_U32 i;
    HI_S32 ret;
    HI_U32 height, width;
    HI_U32 x, y;

#ifdef ANDROID

    HI_RECT_S               stRect;

    HIADP_OSD_ClearSurface(hSurface);

    srand((int)time(0));

    for (i = 0; i < u32BandNum; i++)
    {
        height = (HI_U32)pSpectrum[i] * WIN_HEIGHT / MAX_SPEC_DATA;
        height = height > WIN_HEIGHT ? WIN_HEIGHT : height;
        width = WIN_WIDTH / u32BandNum;
        x = width * i + WIN_X;
        y = WIN_HEIGHT - height + WIN_Y;
        width = width > 1 ? (width - 1) : width;

        stRect.s32X = x;
        stRect.s32Y = y;
        stRect.s32Height = height;
        stRect.s32Width = width;

        HIADP_OSD_FillRect(hSurface, &stRect, 0xff6db6f9);

        usleep(10*1000);
    }

    HIADP_OSD_Refresh(hSurface);
#else
    HI_RECT pRect;

    ret = HI_GO_FillRect(hSurface,NULL,0x00000000,HIGO_COMPOPT_NONE);
    if(ret!=HI_SUCCESS)
    {
        printf("call HI_GO_FillRect failed!\n");
    }

    for (i = 0; i < u32BandNum; i++)
    {
        height = (HI_U32)pSpectrum[i] * WIN_HEIGHT / MAX_SPEC_DATA;
        height = height > WIN_HEIGHT ? WIN_HEIGHT : height;
        width = WIN_WIDTH / u32BandNum;
        x = width * i + WIN_X;
        y = WIN_HEIGHT - height + WIN_Y;
        width = width > 1 ? (width - 1) : width;
        pRect.x = x;
        pRect.y = y;
        pRect.h = height;
        pRect.w = width;
        ret = HI_GO_FillRect(hSurface,&pRect,0xff6db6f9,HIGO_COMPOPT_NONE);
        if(ret!=HI_SUCCESS&&ret!=0xb0048007)
        {
            printf("call HI_GO_FillRect failed!\n");
        }
    }
    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if(ret!=HI_SUCCESS)
    {
        printf("call HI_GO_RefreshLayer_hd failed!\n");
    }
#endif
}


HI_S32 spectrum_draw_task(void *date)
{
    HI_S32 ret;
    HI_HANDLE hLayer,hSurface;
    HI_HANDLE red_handle;
    red_handle = *(HI_HANDLE*)date;

#ifdef ANDROID
    HIADP_SURFACE_ATTR_S    stSurAttr;

    stSurAttr.u32Width = 1280;
    stSurAttr.u32Height = 720;
    stSurAttr.enPixelFormat = HIADP_PF_8888;
    HIADP_OSD_CreateSurface(&stSurAttr, &hSurface);

    while(!s_bStopSendStream)
    {

        if (HI_SUCCESS != HI_UNF_AVPLAY_GetAudioSpectrum(red_handle,u16Datatmp, g_u32BandNum))
        {
            usleep(200000); /* GetAudioSpectrum too fast, wait */
            continue;
        }
        draw_spectrum_win(hSurface, hLayer,u16Datatmp, g_u32BandNum);
        usleep(100000);
    }

    HIADP_OSD_DestroySurface(hSurface);

#else
    HIGO_LAYER_INFO_S stLayerInfo;
    ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }

    /** Create Layer */
    ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }
    stLayerInfo.LayerID = HIGO_LAYER_HD_0;
    ret = HI_GO_CreateLayer(&stLayerInfo,&hLayer);
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }

    /** Get Layer Surface */
    ret = HI_GO_GetLayerSurface (hLayer,&hSurface);
    if (HI_SUCCESS != ret)
    {
       HI_GO_DestroyLayer(hLayer);
       HI_GO_Deinit();
       return HI_FAILURE;
    }

    while(!s_bStopSendStream)
    {

        if (HI_SUCCESS != HI_UNF_AVPLAY_GetAudioSpectrum(red_handle,u16Datatmp, g_u32BandNum))
        {
            usleep(200000); /* GetAudioSpectrum too fast, wait */
            continue;
        }
        draw_spectrum_win(hSurface, hLayer,u16Datatmp, g_u32BandNum);
        usleep(100000);
    }

    HI_GO_DestroyLayer(hLayer);
    HI_GO_Deinit();
#endif
    return 0;
}

HI_S32 MP3_process(void *date)
{
    HI_S32 ret;
    HI_HANDLE red_handle;
    HI_UNF_STREAM_BUF_S AVBuf = {0};
    HI_U32 Readlen;
    HI_S32 packNum = 0;
    red_handle = *((HI_HANDLE*)date);
    while (!s_bStopSendStream)
    {
        ret = HI_UNF_AVPLAY_GetBuf(red_handle, HI_UNF_AVPLAY_BUF_ID_ES_AUD, ES_READ_BLOCK, &AVBuf,0);
        if (ret != HI_SUCCESS)
        {
            usleep(10*1000);
            continue;
        }

        Readlen = fread(AVBuf.pu8Data, sizeof(HI_S8), ES_READ_BLOCK, fp);
        if (Readlen <= 0)
        {
            rewind(fp);
            continue;
        }

        ret = HI_UNF_AVPLAY_PutBuf(red_handle, HI_UNF_AVPLAY_BUF_ID_ES_AUD, Readlen, 0);

        if ((packNum % 100 == 2) || (packNum < 10))
        {
            //printf( " fread pack num %d .\n", packNum++);
        }
    }
    return 0;
}

HI_S32 main(int argc, char *argv[])
{
    HI_S32 ret = 0;
    HI_UNF_AVPLAY_ATTR_S red_AvAttr;
    HI_HANDLE red_handle;
    HI_UNF_ENC_FMT_E   enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    pthread_t Mp3_thread;
    pthread_t Spectrum_thread;

    if (4 == argc)
    {
        enFormat = stringToUnfFmt(argv[3]);
        g_u32BandNum = strtol(argv[2], NULL, 10);
    }
    else if (3 == argc)
    {
        enFormat = HI_UNF_ENC_FMT_1080i_50;
        g_u32BandNum = strtol(argv[2], NULL, 10);
    }
    else if (2 == argc)
    {
        enFormat = HI_UNF_ENC_FMT_1080i_50;
        g_u32BandNum = 80;
    }
    else
    {
        printf("Usage:       ./sample_mp3_spectrum inputfile [band_number] [vo_format]\n"
               "band_number: 20|80|512\n"
               "vo_format:   1080P_60|1080P_50|1080i_60|1080i_50|720P_60|720P_50\n");
        printf("Example:     ./sample_mp3_spectrum ./bigsea.mp3 80 1080i_50\n");
        return 0;
    }

    if ((fp = fopen(argv[1], "rb")) == NULL)
    {
        printf("open %s failed!\n",argv[1]);
        return -1;
    }

    ret = HI_SYS_Init();
    if (ret != HI_SUCCESS)
    {
        printf("call HI_SYS_Init failed!\n");
        return ret;
    }

    ret = HIADP_Snd_Init();
    if (ret != HI_SUCCESS)
    {
        printf("call HIADP_Snd_Init failed!\n");
        goto SYS_DEINIT;
    }

    ret = HIADP_Disp_Init(enFormat);
    if (ret != HI_SUCCESS)
    {
        printf("call HIADP_Disp_Init failed!\n");
        goto SND_DEINIT;
    }

    ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (ret != HI_SUCCESS)
    {
        printf("call HIADP_VO_Init failed!\n");
        goto DISP_DEINIT;
    }

    ret = HIADP_AVPlay_RegADecLib();
    if (ret != HI_SUCCESS)
    {
        printf("call HIADP_AVPlay_RegADecLib failed!\n");
        goto VO_DEINIT;
    }

    ret = HI_UNF_AVPLAY_Init();
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed!\n");
        goto VO_DEINIT;
    }

    red_AvAttr.stStreamAttr.enStreamType = HI_UNF_AVPLAY_STREAM_TYPE_ES;
    ret = HI_UNF_AVPLAY_GetDefaultConfig(&red_AvAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_GetDefaultConfig failed!\n");
        goto AVPLAY_DEINIT;
    }
    red_AvAttr.stStreamAttr.u32VidBufSize = 0x300000;
    ret = HI_UNF_AVPLAY_Create(&red_AvAttr, &red_handle);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed!\n");
        goto AVPLAY_DEINIT;
    }

    ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto AVPLAY_DESTROY;
    }
    ret = HI_UNF_SND_CreateTrack(enSound,&stTrackAttr,&hTrack);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto AVPLAY_DESTROY;
    }

    ret = HI_UNF_SND_Attach(hTrack, red_handle);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    ret = HI_UNF_AVPLAY_ChnOpen(red_handle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed!\n");
        goto AVPLAY_DETACH_SND;
    }

    ret = HIADP_AVPlay_SetAdecAttr(red_handle,HA_AUDIO_ID_MP3,HD_DEC_MODE_RAWPCM,1);
    if (ret != HI_SUCCESS)
    {
        printf("call HIADP_AVPlay_SetAdecAttr failed!\n");
        goto AVPLAY_CLOSE;
    }
    ret = HI_UNF_AVPLAY_Start(red_handle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Start failed!\n");
        goto AVPLAY_CLOSE;
    }


    pthread_create(&Mp3_thread,NULL, (void *)MP3_process, (void *)(&red_handle));
    pthread_create(&Spectrum_thread, NULL, (void*)spectrum_draw_task, (void *)(&red_handle));

    printf("Press any key to exit\n");
    getchar();

    s_bStopSendStream = HI_TRUE;
    pthread_join(Mp3_thread, HI_NULL);
    pthread_join(Spectrum_thread, HI_NULL);

    ret = HI_UNF_AVPLAY_Stop(red_handle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Stop failed!\n");
        goto AVPLAY_CLOSE;
    }

AVPLAY_DETACH_SND:
    HI_UNF_SND_Detach(hTrack, red_handle);
TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);
AVPLAY_CLOSE:
    HI_UNF_AVPLAY_ChnClose(red_handle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(red_handle);
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();
VO_DEINIT:
    HIADP_VO_DeInit();
DISP_DEINIT:
    HIADP_Disp_DeInit();
SND_DEINIT:
    HIADP_Snd_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();
    return 0;
}
