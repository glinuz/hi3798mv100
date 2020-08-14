/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_tsplay.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_ecs.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_descrambler.h"
#include "hi_unf_advca.h"
#include "hi_unf_demux.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_mpi.h"
#include "sample_ca_common.h"

FILE               *g_pTsFile = HI_NULL;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL     g_bStopTsThread = HI_FALSE;
HI_HANDLE          g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
HI_S32             g_CwType = 0;
#define  PLAY_DMX_ID  0


HI_VOID *TsTthread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            Ret;

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
            HI_DEBUG_ADVCA("read ts file end and rewind!\n");
            rewind(g_pTsFile);
            pthread_mutex_unlock(&g_TsMutex);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
        pthread_mutex_unlock(&g_TsMutex);
    }

    Ret = HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_ResetTSBuffer failed.\n");
    }

    return HI_NULL;
}

/* intergration and sample stream */
static HI_U8 g_u8ClearOddKey[8] = {0xFE,0x17,0x2C,0x41,0xA2,0xE1,0xD4,0x57};
static HI_U8 g_u8ClearEvenKey[8] = {0xD4,0x22,0x82,0x78,0x1D,0x28,0x72,0xB7};
static HI_U8 g_u8EncrytedOddKey[8] = {0x44,0x2C,0xF6,0x2E,0x00,0xD0,0x77,0x96};
static HI_U8 g_u8EncrytedEvenKey[8] = {0xD8,0xDC,0x8E,0x22,0x04,0x1A,0xC4,0x9B};
#if 0
static HI_U8 g_u8ClearCwpk[16] = {0x01,0x23,0x45,0x67,   
                                  0x89,0xab,0xcd,0xef,   
                                  0x01,0x01,0x01,0x01,   
                                  0x01,0x01,0x01,0x01};  /*0123456789abcdef0101010101010101*/
#endif
#if 0
/*intergration stream*/
/*fa57ac29c80fe95839d29a7b88d12f93*/
static HI_U8 g_u8EncrptedCwpk[16] = {0xfa,0x57,0xac,0x29,
                                     0xc8,0x0f,0xe9,0x58,
                                     0x39,0xd2,0x9a,0x7b,
                                     0x88,0xd1,0x2f,0x93}; 
#else
/*sample stream*/
/*16c55b89920afcd5c6fc6db86dc1f178*/
static HI_U8 g_u8EncrptedCwpk[16] = {0x16,0xc5,0x5b,0x89,
                                     0x92,0x0a,0xfc,0xd5,
                                     0xc6,0xfc,0x6d,0xb8,
                                     0x6d,0xc1,0xf1,0x78}; 
#endif

HI_S32 AVPLAY_SetCW(HI_HANDLE hKey)
{
    HI_S32 Ret;

    if (0 == g_CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8ClearOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8ClearEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return Ret;
        }
    }
    else
    {
        Ret = HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_TDES);
        /* first level */
        Ret |= HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,g_u8EncrptedCwpk);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_SetDVBSessionKey failed\n");            
            return HI_FAILURE;
        }
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8EncrytedOddKey);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8EncrytedEvenKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerEvenKey failed\n");
            return HI_FAILURE;
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 AVPLAY_AttachDescramble(HI_HANDLE hAvplay,HI_HANDLE *phKey)
{
    HI_S32 Ret;
    HI_UNF_DMX_DESCRAMBLER_ATTR_S stDesramblerAttr;
    HI_HANDLE hKey;
    HI_HANDLE hDmxVidChn,hDmxAudChn;

	memset(&stDesramblerAttr,0,sizeof(HI_UNF_DMX_DESCRAMBLER_ATTR_S));
    if (0 == g_CwType)
    {
        stDesramblerAttr.enCaType = HI_UNF_DMX_CA_NORMAL;
    }
    else
    {
        stDesramblerAttr.enCaType = HI_UNF_DMX_CA_ADVANCE;
    }
    stDesramblerAttr.enDescramblerType = HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2;
	stDesramblerAttr.enEntropyReduction = HI_UNF_DMX_CA_ENTROPY_REDUCTION_CLOSE;
    Ret = HI_UNF_DMX_CreateDescramblerExt(0,&stDesramblerAttr,&hKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_CreateDescramblerExt failed %#x\n",Ret);
        return HI_FAILURE;
    }

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    Ret = HI_UNF_DMX_AttachDescrambler(hKey,hDmxVidChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachDescrambler hDmxVidChn failed\n");        
        (HI_VOID)HI_UNF_DMX_DestroyDescrambler(hKey);
        return HI_FAILURE;
    }

    Ret = HI_UNF_DMX_AttachDescrambler(hKey,hDmxAudChn);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachDescrambler hDmxAudChn failed\n");                
        (HI_VOID)HI_UNF_DMX_DetachDescrambler(hKey,hDmxVidChn);
        (HI_VOID)HI_UNF_DMX_DestroyDescrambler(hKey);
        return HI_FAILURE;
    }
    *phKey = hKey;

    return HI_SUCCESS;
}

HI_S32 AVPLAY_DetachDescramble(HI_HANDLE hAvplay,HI_HANDLE hKey)
{
    HI_S32 Ret;
    HI_HANDLE hDmxVidChn,hDmxAudChn;

    Ret = HI_UNF_AVPLAY_GetDmxVidChnHandle(hAvplay,&hDmxVidChn);
    Ret |= HI_UNF_AVPLAY_GetDmxAudChnHandle(hAvplay,&hDmxAudChn);
    Ret |= HI_UNF_DMX_DetachDescrambler(hKey,hDmxVidChn);
    Ret |= HI_UNF_DMX_DetachDescrambler(hKey,hDmxAudChn);
    Ret |= HI_UNF_DMX_DestroyDescrambler(hKey);
    
    return Ret;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_HANDLE               hWin;
    HI_HANDLE               hAvplay;
    HI_HANDLE               hKey;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;
    HI_CHAR                 InputCmd[32];
    HI_UNF_ENC_FMT_E   enFormat = HI_UNF_ENC_FMT_1080i_50;
    HI_U32             ProgNum;

    HI_HANDLE                   hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S    stTrackAttr;

    if(argc < 3)
    {
        printf("Usage: sample_tsplay file CwType\n"
               "CwType 0-ClearCw 1-EncrytedCw\n");
        return 0;
    }

    g_CwType = (0 == atoi(argv[2]))?(0):(1);
    enFormat = HI_UNF_ENC_FMT_1080i_50;
    
    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
	{
        HI_DEBUG_ADVCA("open file %s error!\n", argv[1]);
		return -1;
	}

    (HI_VOID)HI_SYS_Init();

    HIADP_MCE_Exit();
    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_Init failed\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call SndInit failed.\n");
        goto CA_DEINIT;
    }

    Ret = HIADP_Disp_Init(enFormat);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Disp_Init failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        (HI_VOID)HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
//    Ret |= HI_UNF_DMX_AttachTSPort(0, HI_UNF_DMX_PORT_3_RAM);
    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID,HI_UNF_DMX_PORT_RAM_0);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        goto VO_DEINIT;
    }

//    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_3_RAM, 0x200000, &g_TsBuf);
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call RegADecLib failed.\n");
        goto TSBUF_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Init failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto VCHN_CLOSE;
    }

    Ret = AVPLAY_AttachDescramble(hAvplay,&hKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call AVPLAY_AttachDescramble failed.\n");
        goto ACHN_CLOSE;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    Ret |= HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }
    
    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    (HI_VOID)pthread_mutex_init(&g_TsMutex,NULL);
    g_bStopTsThread = HI_FALSE;
    (HI_VOID)pthread_create(&g_TsThd, HI_NULL, TsTthread, HI_NULL);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0, &g_pProgTbl);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed.\n");
        goto PSISI_FREE;
    }

#if 0
    HI_DEBUG_ADVCA("select channel number to start play\n");
    memset(InputCmd, 0, 30);
    fgets(InputCmd, 30, stdin);
    ProgNum = atoi(InputCmd);
#endif
    ProgNum = 0;

    pthread_mutex_lock(&g_TsMutex);
    rewind(g_pTsFile);
    (HI_VOID)HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
    pthread_mutex_unlock(&g_TsMutex);
    Ret = AVPLAY_SetCW(hKey);
    Ret |= HIADP_AVPlay_PlayProg(hAvplay, g_pProgTbl, ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    while(1)
    {
        printf("please input 'q' to quit!\n");
        memset(InputCmd, 0, 30);
        (HI_VOID)fgets(InputCmd, 30, stdin);
        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("prepare to quit!\n");
            break;
        }

        ProgNum = atoi(InputCmd);
        if (ProgNum == 0)
            ProgNum = 1;

        pthread_mutex_lock(&g_TsMutex);
        rewind(g_pTsFile);
        (HI_VOID)HI_UNF_DMX_ResetTSBuffer(g_TsBuf);
        pthread_mutex_unlock(&g_TsMutex);
        Ret = AVPLAY_SetCW(hKey);
		Ret |= HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum-1,HI_TRUE);
		if (Ret != HI_SUCCESS)
		{
			HI_DEBUG_ADVCA("call SwitchProgfailed!\n");
			break;
		}
    }

AVPLAY_STOP:
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;
    (HI_VOID)HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);

PSISI_FREE:
    HIADP_Search_DeInit();

    g_bStopTsThread = HI_TRUE;
    (HI_VOID)pthread_join(g_TsThd, HI_NULL);  
    (HI_VOID)pthread_mutex_destroy(&g_TsMutex);

SND_DETACH:
    (HI_VOID)HI_UNF_SND_Detach(hTrack, hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);
    
WIN_DETACH:
    (HI_VOID)HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
    (HI_VOID)HI_UNF_VO_DetachWindow(hWin, hAvplay);

    (HI_VOID)AVPLAY_DetachDescramble(hAvplay,hKey);

ACHN_CLOSE:
    (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

VCHN_CLOSE:
    (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);

AVPLAY_DESTROY:
    (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
    
AVPLAY_DEINIT:
    (HI_VOID)HI_UNF_AVPLAY_DeInit();

TSBUF_FREE:
    (HI_VOID)HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);
    
DMX_DEINIT:
    (HI_VOID)HI_UNF_DMX_DetachTSPort(0);
    (HI_VOID)HI_UNF_DMX_DeInit();

VO_DEINIT:
    (HI_VOID)HI_UNF_VO_DestroyWindow(hWin);
    (HI_VOID)HIADP_VO_DeInit();

DISP_DEINIT:
	(HI_VOID)HIADP_Disp_DeInit();

SND_DEINIT:
    (HI_VOID)HIADP_Snd_DeInit();

CA_DEINIT:
    (HI_VOID)HI_UNF_ADVCA_DeInit();

SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();
    (HI_VOID)fclose(g_pTsFile);
    g_pTsFile = HI_NULL;

    return Ret;
}


