/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : es_ts_switch.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/26
  Description   :
  History       :
  1.Date        : 2011/01/26
    Author      : y00190777
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
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

//#define AUTOSWITCH

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

static HI_HANDLE   g_hAvplayEs;
static HI_HANDLE   g_hAvplayTs;

static pthread_t   g_EsThd;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static pthread_mutex_t g_EsMutex;

static HI_HANDLE   g_hTrack;


typedef struct hiThread_STATUS_S
{
    HI_BOOL           bPauseEsThread;
    HI_BOOL           bStopEsThread;
    HI_BOOL           bPauseTsThread;
    HI_BOOL           bStopTsThread;
}THREAD_STATUS_S;

static THREAD_STATUS_S  g_struThreadCtx = {0};

typedef enum hiPLAY_TYPE_E
{
    PLAY_TYPE_ES,
    PLAY_TYPE_TS,
    PLAY_TYPE_BUTT             
}PLAY_TYPE_E;

FILE               *g_pTsFile = HI_NULL;
FILE               *g_pVidEsFile = HI_NULL;

static HI_BOOL g_bReadFrameSize = HI_FALSE;

HI_HANDLE           g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

#define  PLAY_DMX_ID  0

HI_VOID EsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S StreamBuf;
    HI_U32 Readlen;
    HI_S32 Ret;   
    HI_U32 FrameSize = 0;

    while (HI_TRUE)
    {
        if (HI_TRUE == g_struThreadCtx.bStopEsThread)
        {
            break;
        }

        if (HI_TRUE == g_struThreadCtx.bPauseEsThread)
        {
            usleep(1000 * 10);
            continue;
        }

        pthread_mutex_lock(&g_EsMutex);

        if (HI_FALSE != g_bReadFrameSize)
        {
            Ret = HI_UNF_AVPLAY_GetBuf(g_hAvplayEs, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x100000, &StreamBuf, 0);
        }
        else
        {
            Ret = HI_UNF_AVPLAY_GetBuf(g_hAvplayEs, HI_UNF_AVPLAY_BUF_ID_ES_VID, 0x4000, &StreamBuf, 0);
        }

        if (HI_SUCCESS == Ret)
        {
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
                Ret = HI_UNF_AVPLAY_PutBuf(g_hAvplayEs, HI_UNF_AVPLAY_BUF_ID_ES_VID, Readlen, 0);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call HI_UNF_AVPLAY_PutBuf failed.\n");
                }

                FrameSize = 0;
            }
            else if (Readlen <= 0)
            {
                sample_printf("read vid file error!\n");
                rewind(g_pVidEsFile);
            }
        }
        else
        {
           /* wait for buffer */
            usleep(1000 * 10);
        }

        pthread_mutex_unlock(&g_EsMutex);
    }

    return;
}

HI_VOID TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32                Readlen;
    HI_S32                Ret;

    while (HI_TRUE)
    {
        if (HI_TRUE == g_struThreadCtx.bStopTsThread)
        {
            break;
        }

        if (HI_TRUE == g_struThreadCtx.bPauseTsThread)
        {
            usleep(1000 * 10);
            continue;
        }

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
            sample_printf("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            sample_printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
        pthread_mutex_unlock(&g_TsMutex);
    }

    if (HI_FALSE == g_struThreadCtx.bPauseTsThread)
    {
        Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
        if (Ret != HI_SUCCESS )
        {
            sample_printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
        }
    }

    return;
}

HI_S32 CreateCommonResource(HI_UNF_ENC_FMT_E enFormat, HI_HANDLE *hWin)
{
    HI_S32   Ret = HI_SUCCESS;
    Ret = HI_SYS_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_SYS_Init failed.\n");
        return Ret;
    }

    HIADP_MCE_Exit();

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_Init failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    return Ret;

DISP_DEINIT:
     HIADP_Disp_DeInit();

AVPLAY_DEINIT:
     HI_UNF_AVPLAY_DeInit();

SYS_DEINIT:
     HI_SYS_DeInit();

     return Ret;
}

HI_S32 DestroyCommonResource(HI_HANDLE hWin)
{
    HI_UNF_VO_DestroyWindow(hWin);
    HIADP_VO_DeInit();
    HIADP_Disp_DeInit();
    HI_UNF_AVPLAY_DeInit();
    HI_SYS_DeInit();

    return HI_SUCCESS;
}

HI_S32 CreateTsResource(HI_HANDLE hWin)
{
    HI_S32                 Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_ATTR_S   AvplayAttr;
    HI_UNF_SYNC_ATTR_S     SyncAttr;

    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call SndInit failed.\n");
        return Ret;
    }
    
    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call RegADecLib failed.\n");
        goto SND_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID,HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
	AvplayAttr.u32DemuxId = PLAY_DMX_ID;
    AvplayAttr.stStreamAttr.u32VidBufSize = (3*1024*1024);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplayTs);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, g_hAvplayTs);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed:%#x.\n",Ret);
        goto ACHN_CLOSE;
    }
    Ret |= HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
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
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&g_hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }
   
    Ret = HI_UNF_SND_Attach(g_hTrack, g_hAvplayTs);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplayTs, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    Ret |= HI_UNF_AVPLAY_SetAttr(g_hAvplayTs, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    return Ret;

SND_DETACH:
    HI_UNF_SND_Detach(g_hTrack, g_hAvplayTs);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(g_hTrack);     

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, g_hAvplayTs);

ACHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(g_hAvplayTs);

TSBUF_FREE:
    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

    return Ret;
}

HI_S32 DestroyTsResource(HI_HANDLE hWin)
{
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

    HIADP_Search_FreeAllPmt(g_pProgTbl);
    g_pProgTbl = HI_NULL;
    HIADP_Search_DeInit();

    HI_UNF_SND_Detach(g_hTrack, g_hAvplayTs);
    
    HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, g_hAvplayTs);

    HI_UNF_AVPLAY_ChnClose(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    HI_UNF_AVPLAY_ChnClose(g_hAvplayTs, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    HI_UNF_AVPLAY_Destroy(g_hAvplayTs);

    HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);
    HI_UNF_DMX_DetachTSPort(0);
    HI_UNF_DMX_DeInit();

    HIADP_Snd_DeInit();

    return HI_SUCCESS;
}

HI_S32 CreateEsResource(HI_HANDLE hWin, HI_UNF_VCODEC_TYPE_E VdecType)
{
    HI_S32                 Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_ATTR_S   AvplayAttr;
    HI_UNF_SYNC_ATTR_S     SyncAttr;

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_ES);
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplayEs);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplayEs, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(g_hAvplayEs, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto AVPLAY_DESTROY;
    }
 
    Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplayEs, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, g_hAvplayEs);
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

    Ret = HIADP_AVPlay_SetVdecAttr(g_hAvplayEs, VdecType, HI_UNF_VCODEC_MODE_NORMAL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HIADP_AVPlay_SetVdecAttr failed.\n");
        goto WIN_DETATCH;
    }

    Ret = HI_UNF_AVPLAY_Start(g_hAvplayEs, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Start failed.\n");
        goto WIN_DETATCH;
    }

    return Ret;

WIN_DETATCH:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, g_hAvplayEs);

VCHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplayEs, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(g_hAvplayEs);

    return Ret;
}

HI_S32 DestroyEsResource(HI_HANDLE hWin)
{
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(g_hAvplayEs, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);
    
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, g_hAvplayEs);
    
    HI_UNF_AVPLAY_ChnClose(g_hAvplayEs, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    HI_UNF_AVPLAY_Destroy(g_hAvplayEs);

    return HI_SUCCESS;
}

HI_BOOL TsThreadIsPause()
{
    return g_struThreadCtx.bPauseTsThread;
}

HI_BOOL EsThreadIsPause()
{
    return g_struThreadCtx.bPauseEsThread;
}

HI_S32 Es_Ts_Switch_Mode(HI_HANDLE hWin, PLAY_TYPE_E enplay_type, HI_UNF_VCODEC_TYPE_E VdecType)
{
    HI_S32   Ret = HI_SUCCESS;

    if (PLAY_TYPE_ES == enplay_type)
    {
        //destroy ts resource
        if (HI_TRUE != TsThreadIsPause())
        {
            g_struThreadCtx.bPauseTsThread = HI_TRUE;
            pthread_mutex_lock(&g_TsMutex);
            rewind(g_pTsFile);
            HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
            DestroyTsResource(hWin);
            pthread_mutex_unlock(&g_TsMutex);

        }
        //create es resource
        Ret = CreateEsResource(hWin, VdecType);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call CreateEsResource failed.\n");
            return Ret;
        }
        //run
        g_struThreadCtx.bPauseEsThread = HI_FALSE;   
    }

    if (PLAY_TYPE_TS == enplay_type)
    {
        //destroy es resource
        if (HI_TRUE != EsThreadIsPause())
        {
            g_struThreadCtx.bPauseEsThread = HI_TRUE;
            pthread_mutex_lock(&g_EsMutex);
            rewind(g_pVidEsFile);
            DestroyEsResource(hWin);
            pthread_mutex_unlock(&g_EsMutex);
        }
        //create ts resource
        Ret = CreateTsResource(hWin);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call CreateTsResource failed.\n");
            return Ret;
        }
        
        //run
        g_struThreadCtx.bPauseTsThread = HI_FALSE;
    }

    return Ret;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_CHAR                 InputCmd[32];
    HI_UNF_ENC_FMT_E        enFormat = HI_UNF_ENC_FMT_1080i_50; 
    HI_UNF_VCODEC_TYPE_E    VdecType = HI_UNF_VCODEC_TYPE_BUTT;
    HI_U32                  ProgNum;

    if (5 == argc)
    {
        enFormat = HIADP_Disp_StrToFmt(argv[4]);
    }
    else if (4 == argc)
    {
        enFormat = HI_UNF_ENC_FMT_1080i_50;

    }
    else
    {
        printf("Usage: sample_es_ts_switch esvidfile vidtype tsfile [vo_format]\n"
              "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n");      
        printf(" vtype: mpeg2/mpeg4/h263/sor/vp6/vp6f/vp6a/h264/avs/real8/real9/vc1\n");
        printf("Example:./sample_es_ts_switch esvidfile h264 tsfile 1080i_50\n");
       
        return 0;
    }

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
    else if (!strcasecmp("vc1", argv[2]))
    {
        VdecType = HI_UNF_VCODEC_TYPE_VC1;
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

    g_pTsFile = fopen(argv[3], "rb");
    if (!g_pTsFile)
	{
        fclose(g_pVidEsFile);
        g_pVidEsFile = HI_NULL;
        sample_printf("open file %s error!\n", argv[1]);
		return -1;
	}

    Ret = CreateCommonResource(enFormat, &hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call CreateCommonResource failed.\n");
        goto FILE_CLOSE;
    }

    pthread_mutex_init(&g_TsMutex,NULL);
    pthread_mutex_init(&g_EsMutex,NULL);

    g_struThreadCtx.bStopTsThread= HI_FALSE;
    g_struThreadCtx.bPauseTsThread = HI_TRUE;
    g_struThreadCtx.bStopEsThread= HI_FALSE;
    g_struThreadCtx.bPauseEsThread = HI_TRUE;

    pthread_create(&g_TsThd, HI_NULL, (HI_VOID *)TsTthread, HI_NULL);
    pthread_create(&g_EsThd, HI_NULL, (HI_VOID *)EsTthread, HI_NULL);

    printf("please input e to play esvidfile, t to play tsfile\n");

    while (1)
    {
        printf("please input the q to quit!, e to play esvidfile, t to play tsfile\n");

#ifndef AUTOSWITCH       
        SAMPLE_GET_INPUTCMD(InputCmd);
#endif
        if ('q' == InputCmd[0])
        { 
           printf("prepare to quit!\n");
           break;
        }

        if ('e' == InputCmd[0])
        {
            if (HI_TRUE != EsThreadIsPause())
            {
                printf("playing esvidfile!\n");
            }
            else
            { 
                printf("prepare to play esvidfile!\n");
                Ret = Es_Ts_Switch_Mode(hWin, PLAY_TYPE_ES, VdecType);
                if (HI_SUCCESS != Ret)
                {
                    sample_printf("call Es_Ts_Switch_Mode ES failed.\n");
                    goto DestroyAllRes;
                }
            }
        }
        else if ('t' == InputCmd[0] )
        {
            if (HI_TRUE != TsThreadIsPause())
            {
                printf("playing tsvidfile!\n");
            }
            else
            {
                printf("prepare to play tsfile!\n");
                Ret = Es_Ts_Switch_Mode(hWin, PLAY_TYPE_TS, VdecType);
                if (HI_SUCCESS != Ret)
                {
                    sample_printf("call Es_Ts_Switch_Mode TS failed.\n");
                    goto DestroyAllRes;
                }

                HIADP_Search_Init();
                Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID, &g_pProgTbl);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call HIADP_Search_GetAllPmt failed.\n");
                    HIADP_Search_DeInit();
                    goto DestroyAllRes;
                }

                ProgNum = 0;
                
                pthread_mutex_lock(&g_TsMutex);
                rewind(g_pTsFile);
                HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
                pthread_mutex_unlock(&g_TsMutex);
                Ret = HIADP_AVPlay_PlayProg(g_hAvplayTs,g_pProgTbl,ProgNum,HI_TRUE);
                if (Ret != HI_SUCCESS)
                {
                    sample_printf("call SwitchProg failed.\n");
                    goto DestroyAllRes;;
                }
            }
        }

#ifdef AUTOSWITCH
        sleep(1);
        if ('e' == InputCmd[0])
        {
            InputCmd[0] = 't';
        }
        else
        {
            InputCmd[0] = 'e';
        }  
#endif
        if (HI_TRUE != TsThreadIsPause())
        {
            static HI_U32 u32TplaySpeed = 2;
            if ('s' == InputCmd[0])
            {
    			HI_UNF_AVPLAY_TPLAY_OPT_S stTplayOpt;
    			printf("%dx tplay\n",u32TplaySpeed);
    			
    			stTplayOpt.enTplayDirect = HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD;
    			stTplayOpt.u32SpeedInteger = u32TplaySpeed;
    			stTplayOpt.u32SpeedDecimal = 0;
    			
    		    HI_UNF_AVPLAY_SetDecodeMode(g_hAvplayTs, HI_UNF_VCODEC_MODE_I);
    			HI_UNF_AVPLAY_Tplay(g_hAvplayTs,&stTplayOpt);
        		u32TplaySpeed = (32 == u32TplaySpeed * 2) ? (2) : (u32TplaySpeed * 2);
    			continue;                
            }
            
            if ('r' == InputCmd[0])
            {
                printf("resume\n");
                HI_UNF_AVPLAY_SetDecodeMode(g_hAvplayTs, HI_UNF_VCODEC_MODE_NORMAL);
                HI_UNF_AVPLAY_Resume(g_hAvplayTs,HI_NULL);
                u32TplaySpeed = 2;
                continue;
            }
            
            ProgNum = atoi(InputCmd);
            if (ProgNum == 0)
                ProgNum = 1;
            
            pthread_mutex_lock(&g_TsMutex);
            rewind(g_pTsFile);
            HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
            pthread_mutex_unlock(&g_TsMutex);
            Ret = HIADP_AVPlay_PlayProg(g_hAvplayTs,g_pProgTbl,ProgNum-1,HI_TRUE);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call SwitchProgfailed!\n");
                break;
            }
        }
     }

DestroyAllRes:
    g_struThreadCtx.bStopEsThread= HI_TRUE;
    g_struThreadCtx.bStopTsThread= HI_TRUE;
    pthread_join(g_EsThd, HI_NULL);
    pthread_join(g_TsThd, HI_NULL);
    pthread_mutex_destroy(&g_EsMutex);
    pthread_mutex_destroy(&g_TsMutex);

    if (HI_TRUE != g_struThreadCtx.bPauseEsThread)
    {
        DestroyEsResource(hWin);
    }
    
    if (HI_TRUE != g_struThreadCtx.bPauseTsThread)
    {
        DestroyTsResource(hWin);
    }

    DestroyCommonResource(hWin);
    
FILE_CLOSE:
    fclose(g_pVidEsFile);
    g_pVidEsFile = HI_NULL;

    fclose(g_pTsFile);
    g_pTsFile = HI_NULL;

    return Ret;
}

