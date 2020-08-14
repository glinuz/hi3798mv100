/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tsplay.c
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
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"

typedef struct
{
    HI_HANDLE   hAvplay;
    FILE       *pTsFile;
    HI_HANDLE   g_TsBuf;
    
}TS_PARAM_S;

static pthread_t   g_TsThd[2] = {-1};
static HI_BOOL     g_bStopTsThread = HI_FALSE;
PMT_COMPACT_TBL   *g_pProgTbl[2] = {HI_NULL};

#define  PLAY_DMX_ID_0  0
#define  PLAY_DMX_ID_1  1 

HI_VOID TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32                Readlen;
    HI_S32                Ret;
    TS_PARAM_S           *pTsParam;

    pTsParam = (TS_PARAM_S *)args;
    
    while (!g_bStopTsThread)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(pTsParam->g_TsBuf, 188*50, &StreamBuf, 1000);
        if (Ret != HI_SUCCESS )
        {
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*50, pTsParam->pTsFile);
        if(Readlen <= 0)
        {
            printf("read ts file end and rewind!\n");
            rewind(pTsParam->pTsFile);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(pTsParam->g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(pTsParam->g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return;
}


HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32                             Ret;
    HI_HANDLE                          hWin[2] = {HI_INVALID_HANDLE};
    HI_HANDLE                          hAvplay[2] = {HI_INVALID_HANDLE};
    HI_UNF_AVPLAY_ATTR_S               AvplayAttr;
    HI_UNF_SYNC_ATTR_S                 SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S           Stop;
    HI_CHAR                            InputCmd[32];
    HI_UNF_ENC_FMT_E                   enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32                             ProgNum;
    HI_HANDLE                          hTrack[2] = {HI_INVALID_HANDLE};
    HI_UNF_AUDIOTRACK_ATTR_S           stTrackAttr;
    TS_PARAM_S                         stTsParam[2];
    FILE                              *TsFile[2];
    HI_HANDLE                          g_TsBuf[2] = {-1};
    HI_RECT_S                          stMasterRect;
    HI_RECT_S                          stChildRect;
    HI_UNF_AVPLAY_TVP_ATTR_S           TvpAttr;
    HI_U32                             i;
    HI_U32                             AVPLAY_NUM = 2;
    HI_UNF_AVPLAY_AUD_SECURITY_ATTR_S  stAudSecAttr;

    if (4 == argc)
    {
        enFormat = stringToUnfFmt(argv[3]);
    }
    else if (3 == argc)
    {
        enFormat = HI_UNF_ENC_FMT_1080i_50;

    }
    else
    {
        printf("Usage: sample_tee_tsplay_pip file file [vo_format]\n"
               "       vo_format:1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n");
        printf("Example:./sample_tee_tsplay_pip ./[HD]test.ts ./[SD]test.ts 1080i_50\n");
        return 0;
    }

    TsFile[0] = fopen(argv[1], "rb");
    if (!TsFile[0])
    {
        printf("open file %s error!\n", argv[1]);
        return -1;
    }

    TsFile[1] = fopen(argv[2], "rb");
    if (!TsFile[1])
    {
        printf("open file %s error!\n", argv[2]);
        return -1;
    }
    
    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    stMasterRect.s32Width  = 0;
    stMasterRect.s32Height = 0;
    stMasterRect.s32X      = 0;
    stMasterRect.s32Y      = 0;
    Ret |= HIADP_VO_CreatWin(&stMasterRect, &hWin[0]);
    if (HI_SUCCESS != Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    stChildRect.s32Width  = 258;
    stChildRect.s32Height = 144;
    stChildRect.s32X      = 72;
    stChildRect.s32Y      = 392;
    Ret |= HIADP_VO_CreatWin(&stChildRect, &hWin[1]);
    if (HI_SUCCESS != Ret)
    {
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_Init failed.\n");
        goto VO_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID_0, HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID_1, HI_UNF_DMX_PORT_RAM_1);
    if (HI_SUCCESS != Ret)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        goto DMX_DEINIT;
    }
    
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf[0]);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_1, 0x200000, &g_TsBuf[1]);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        printf("call RegADecLib failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    for (i = 0; i < AVPLAY_NUM; i++)
    {
        Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);

        switch (i)
        {
            case 0 :
            {
                AvplayAttr.u32DemuxId = PLAY_DMX_ID_0;
                break;
            }

            case 1 :
            {
                AvplayAttr.u32DemuxId = PLAY_DMX_ID_1; 
                break;
            }
            
            default:
                return HI_FAILURE;
        }
        
        Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay[i]);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY_DEINIT;
        }
        
        if (1 == i)
        {
            TvpAttr.bEnable = HI_TRUE;
            HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_TVP, &TvpAttr);
        }
       
        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_DESTROY;
        }

        if (1 == i)
        {
            stAudSecAttr.bEnable = HI_FALSE;
            HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_AUD_SECURITY, &stAudSecAttr);
        }
        
        Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto VCHN_CLOSE;
        }

        Ret = HI_UNF_VO_AttachWindow(hWin[i], hAvplay[i]);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_VO_AttachWindow failed:%#x.\n",Ret);
            goto ACHN_CLOSE;
        }

        Ret = HI_UNF_VO_SetWindowEnable(hWin[i], HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_VO_SetWindowEnable failed.\n");
            goto WIN_DETACH;
        }

        if (0 == i)
        {
            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
                goto WIN_DETACH;
            }
            Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_CreateTrack failed.\n");
                goto WIN_DETACH;
            }

            Ret = HI_UNF_SND_Attach(hTrack[i], hAvplay[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_Attach failed.\n");
                goto TRACK_DESTROY;
            }
        }

        if (1 == i)
        {
            stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_SLAVE;
            Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
                goto WIN_DETACH;
            }
            Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_CreateTrack failed.\n");
                goto WIN_DETACH;
            }

            Ret = HI_UNF_SND_Attach(hTrack[i], hAvplay[i]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HI_UNF_SND_Attach failed.\n");
                goto TRACK_DESTROY;
            }
        }

        Ret = HI_UNF_AVPLAY_GetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            goto SND_DETACH;
        }

        stTsParam[i].hAvplay = hAvplay[i];
        stTsParam[i].pTsFile = TsFile[i];
        stTsParam[i].g_TsBuf = g_TsBuf[i];
        printf("g_TsBuf[%d] = %d\n", i, g_TsBuf[i]);
        
        g_bStopTsThread = HI_FALSE;
        pthread_create(&g_TsThd[i], HI_NULL, (HI_VOID *)TsTthread, &stTsParam[i]);

        HIADP_Search_Init();
        if (0 == i)
        {
            Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID_0, &g_pProgTbl[0]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HIADP_Search_GetAllPmt failed.\n");
                goto SND_DETACH;
            }
        }

        if (1 == i)
        {
            Ret = HIADP_Search_GetAllPmt(PLAY_DMX_ID_1, &g_pProgTbl[1]);
            if (Ret != HI_SUCCESS)
            {
                printf("call HIADP_Search_GetAllPmt failed.\n");
                goto PSISI_FREE;
            }
        }

        ProgNum = 0;

        rewind(TsFile[i]);
        HI_UNF_DMX_ResetTSBuffer(g_TsBuf[i]);

        Ret = HIADP_AVPlay_PlayProg(hAvplay[i], g_pProgTbl[i], ProgNum, HI_TRUE);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_AVPlay_PlayProg failed.\n");
            goto AVPLAY_STOP;
        }
    }
    
    while (1)
    {
        printf("please input 'q' to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        if ('t' == InputCmd[0])
        {
            HI_UNF_AVPLAY_Stop(hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            HI_UNF_VO_FreezeWindow(hWin[0], HI_TRUE, HI_UNF_WINDOW_FREEZE_MODE_BLACK);
            HI_UNF_VO_SetWindowZorder(hWin[0],HI_LAYER_ZORDER_MOVEBOTTOM);
            HI_UNF_AVPLAY_Start(hAvplay[0], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);

            HI_UNF_VO_SetWindowZorder(hWin[1],HI_LAYER_ZORDER_MOVETOP);
            HI_UNF_VO_FreezeWindow(hWin[1], HI_FALSE, HI_UNF_WINDOW_FREEZE_MODE_BLACK);    
        }

        if ('b' == InputCmd[0])
        {
            HI_UNF_AVPLAY_Stop(hAvplay[1], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            HI_UNF_VO_FreezeWindow(hWin[1], HI_TRUE, HI_UNF_WINDOW_FREEZE_MODE_BLACK);
            HI_UNF_VO_SetWindowZorder(hWin[1],HI_LAYER_ZORDER_MOVEBOTTOM);
            HI_UNF_AVPLAY_Start(hAvplay[1], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);

            HI_UNF_VO_SetWindowZorder(hWin[0],HI_LAYER_ZORDER_MOVETOP);
            HI_UNF_VO_FreezeWindow(hWin[0], HI_FALSE, HI_UNF_WINDOW_FREEZE_MODE_BLACK);      
        }
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hAvplay[i])
        {
            HI_UNF_AVPLAY_Stop(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
        }
    }
PSISI_FREE:
    
    for (i = 0; i < AVPLAY_NUM; i++)
    {        
        if (HI_NULL != g_pProgTbl[i])
        {
            HIADP_Search_FreeAllPmt(g_pProgTbl[i]);
            g_pProgTbl[i] = HI_NULL;            
        }
    }
    HIADP_Search_DeInit();

    g_bStopTsThread = HI_TRUE;
    
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (-1 != g_TsThd[i])
        {
            pthread_join(g_TsThd[i], HI_NULL);
        }
    }
    
SND_DETACH:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if ((HI_INVALID_HANDLE != hTrack[i]) && (HI_INVALID_HANDLE != hAvplay[i]))
        {
            HI_UNF_SND_Detach(hTrack[i], hAvplay[i]);
        }
    }
    
TRACK_DESTROY:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hTrack[i])
        {
            HI_UNF_SND_DestroyTrack(hTrack[i]);
        }
    }
    
WIN_DETACH:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if ((HI_INVALID_HANDLE != hWin[i]) && (HI_INVALID_HANDLE != hAvplay[i]))
        {
            HI_UNF_VO_SetWindowEnable(hWin[i], HI_FALSE);
            HI_UNF_VO_DetachWindow(hWin[i], hAvplay[i]);
        }
    }
    
ACHN_CLOSE:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hAvplay[i])
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        }
    }

VCHN_CLOSE:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hAvplay[i])
        {
            HI_UNF_AVPLAY_ChnClose(hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        }
    }

AVPLAY_DESTROY:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hAvplay[i])
        {
            HI_UNF_AVPLAY_Destroy(hAvplay[i]);
        }
    }
    
AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (-1 != g_TsBuf[i])
        {
            HI_UNF_DMX_DestroyTSBuffer(g_TsBuf[i]);
        }
    }

DMX_DEINIT:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        HI_UNF_DMX_DetachTSPort(i);
    }
    
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    for (i = 0; i < AVPLAY_NUM; i++)
    {
        if (HI_INVALID_HANDLE != hWin[i])
        {
            HI_UNF_VO_DestroyWindow(hWin[i]);
        }
    }

    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    fclose(TsFile[0]);
    fclose(TsFile[1]);

    return Ret;
}


