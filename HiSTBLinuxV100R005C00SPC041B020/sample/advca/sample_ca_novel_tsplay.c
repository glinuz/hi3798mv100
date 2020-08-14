/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_novel_tsplay.c
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

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "sample_ca_common.h"

FILE               *g_pTsFile = HI_NULL;
static pthread_t   g_TsThd;
static pthread_mutex_t g_TsMutex;
static HI_BOOL     g_bStopTsThread = HI_FALSE;
HI_HANDLE          g_TsBuf;
PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;
HI_S32             g_CwType = 0;
#define            PLAY_DMX_ID  0


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
static HI_U8 g_u8ClearOddKey[4][8] = {{0xFB,0xAC,0xF3,0x9A,0x42,0x2C,0x29,0x97},     /*the clear odd key of program2:   FBACF39A422C2997*/
                                  {0xA0,0x07,0xF7,0x9E,0x8E,0x7E,0xAD,0xB9},         /*the clear odd key of program3:   A007F79E8E7EADB9*/
                                  {0x02,0x4F,0x63,0xB4,0x9C,0x1F,0x52,0x0D},         /*the clear odd key of program4:   024F63B49C1F520D*/
                                  {0x97,0x41,0xA6,0x7E,0x86,0x0F,0x96,0x2B}};        /*the clear odd key of program5:   9741A67E860F962B*/
static HI_U8 g_u8ClearEvenKey[4][8] = {{0x02,0xA5,0x37,0xDE,0x26,0x97,0x7B,0x38},    /*the clear even key of program2:  02A537DE26977B38*/
                                  {0x54,0xA3,0x90,0x87,0x0D,0xE1,0x00,0xEE},         /*the clear even key of program3:  54A390870DE100EE*/
                                  {0x2C,0xE8,0x84,0x98,0x10,0xAC,0x7F,0x3B},         /*the clear even key of program4:  2CE8849810AC7F3B*/
                                  {0x55,0xBD,0x01,0x13,0xBB,0x8F,0x1A,0x64}};        /*the clear even key of program5:  55BD0113BB8F1A64*/
static HI_U8 g_u8EncrytedOddKey[4][8] = {{0x3b,0x91,0xfd,0xab,0x27,0xac,0x40,0x73},  /*the encrypted odd key of program2:  3b91fdab27ac4073*/
                                  {0x76,0x1f,0xcb,0x4e,0xcb,0x00,0x5c,0xd4},         /*the encrypted odd key of program3:  761fcb4ecb005cd4*/
                                  {0x5e,0xb9,0x1c,0xa4,0xe2,0x86,0x8e,0xcb},         /*the encrypted odd key of program4:  5eb91ca4e2868ecb*/
                                  {0x84,0x06,0xd6,0xa2,0x4a,0x8f,0x02,0xaa}};        /*the encrypted odd key of program5:  8406d6a24a8f02aa*/
static HI_U8 g_u8EncrytedEvenKey[4][8] = {{0x48,0x2b,0x45,0x86,0xea,0xec,0x00,0x80}, /*the encrypted even key of program2: 482b4586eaec0080*/
                                  {0x75,0xfa,0xa6,0x4f,0x05,0x17,0xa6,0xcd},         /*the encrypted even key of program3: 75faa64f0517a6cd*/
                                  {0x34,0x55,0x8d,0xd4,0x09,0xb4,0x7e,0x69},         /*the encrypted even key of program4: 34558dd409b47e69*/
                                  {0x66,0x65,0x28,0xb9,0x5e,0xf7,0x5a,0xeb}};        /*the encrypted even key of program5: 666528b95ef75aeb*/

/** stream: CW_0801_164918.ts
*** g_u8DVBRootkey[16] = {0xa5 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03}
*** g_u8ClearSessionkey[16] = {0xe1,0xa2,0xf3,0xe2,0x45,0x88,0x52,0x43,0x5a,0xa1,0x22,0x2d,0xe4,0x18,0x19,0x51};
*** Notes: If you want to test noevl CSA2 keyladder, please use g_u8DVBRootkey encrypt g_u8ClearSessionkey by TDES-ECB.
*** the output is g_u8EncrptedSessionKey. then rebuild this sample.
*/
//static HI_U8 g_u8DVBRootkey[16] = {0xa5 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03 0x03}
//static HI_U8 g_u8ClearSessionkey[16] = {0xe1,0xa2,0xf3,0xe2,0x45,0x88,0x52,0x43,0x5a,0xa1,0x22,0x2d,0xe4,0x18,0x19,0x51};
static HI_U8 g_u8EncrptedSessionKey[16] = {0x27,0x50,0x69,0x6f,0xfc,0x87,0xb1,0x73,0xd6,0x21,0x80,0x73,0x7d,0x88,0x9c,0x80};
//static HI_U8 g_u8Clearcwpk[16] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x00}; /*112233445566778899aabbccddeeff00*/
static HI_U8 g_u8EncrptedCwpk[16] = {0xdc,0x10,0x52,0xf2,0x4b,0x42,0x5b,0x09, 0x92,0x45,0x8a,0x11, 0x81,0x45,0x0a,0x8d};

HI_S32 AVPLAY_SetCW(HI_HANDLE hKey, HI_U32 ProgNum)
{
    HI_S32 Ret, index;

    if ((ProgNum < 2) || (ProgNum > 3))
    {
        return HI_SUCCESS;
    }

    if (0 == g_CwType)
    {
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8ClearOddKey[ProgNum - 2]);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8ClearEvenKey[ProgNum - 2]);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_SetDescramblerKey failed\n");
            return Ret;
        }
    }
    else
    {
        HI_DEBUG_ADVCA("Advance CA\n");
        Ret = HI_UNF_ADVCA_SetDVBAlg(HI_UNF_ADVCA_ALG_TYPE_TDES);
        /* first level */
        Ret |= HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV1,g_u8EncrptedSessionKey);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_SetDVBSessionKey failed\n");
            return HI_FAILURE;
        }
        Ret |= HI_UNF_ADVCA_SetDVBSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV2,g_u8EncrptedCwpk);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("call HI_UNF_ADVCA_SetDVBSessionKey failed\n");
            return HI_FAILURE;
        }
        HI_DEBUG_ADVCA("Odd Key:");
        for(index = 0; index < 8; index ++)
        {
            HI_DEBUG_ADVCA("%02x ", g_u8EncrytedOddKey[ProgNum - 2][index]);
        }
        HI_DEBUG_ADVCA("\n");
        HI_DEBUG_ADVCA("Even Key:");
        for(index = 0; index < 8; index ++)
        {
            HI_DEBUG_ADVCA("%02x ", g_u8EncrytedEvenKey[ProgNum - 2][index]);
        }
        HI_DEBUG_ADVCA("\n");
        Ret = HI_UNF_DMX_SetDescramblerOddKey(hKey,g_u8EncrytedOddKey[ProgNum - 2]);
        Ret |= HI_UNF_DMX_SetDescramblerEvenKey(hKey,g_u8EncrytedEvenKey[ProgNum - 2]);
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
    HI_UNF_AVPLAY_OPEN_OPT_S maxCapbility;
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
#ifdef __HI3716MV310__
    HI_HANDLE sthMixer;
    HI_UNF_MIXER_ATTR_S stMixerAttr;
#endif

    if(argc < 3)
    {
        printf("Usage: sample_tsplay file CwType\n"
               "CwType 0-ClearCw 1-TDES\n");
        return 0;
    }

    g_CwType = (0 == atoi(argv[2]))?(0):(1);
    enFormat = HI_UNF_ENC_FMT_1080i_50;
    maxCapbility.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
    maxCapbility.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
    maxCapbility.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;

    g_pTsFile = fopen(argv[1], "rb");
    if (!g_pTsFile)
	{
        HI_DEBUG_ADVCA("open file %s error!\n", argv[1]);
		return -1;
	}

    (HI_VOID)HI_SYS_Init();

#ifndef __HI3716MV310__
    HIADP_MCE_Exit();
#endif
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
#ifdef __HI3716MV310__
    Ret |= HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID, HI_UNF_DMX_PORT_3_RAM);
#else
    Ret = HI_UNF_DMX_AttachTSPort(PLAY_DMX_ID,HI_UNF_DMX_PORT_RAM_0);
#endif
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        goto VO_DEINIT;
    }
#ifdef __HI3716MV310__
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_3_RAM, 0x200000, &g_TsBuf);
#else
    Ret = HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_RAM_0, 0x200000, &g_TsBuf);
#endif
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

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, &maxCapbility);
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
#ifndef __HI3716MV310__
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &hTrack);
    //Ret = HI_UNF_SND_Mixer_Open(HI_UNF_SND_0, &hTrack, &stMixerAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }
#endif

#ifdef __HI3716MV310__
    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
#else
    Ret = HI_UNF_SND_Attach(hTrack, hAvplay);
#endif
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
    Ret = AVPLAY_SetCW(hKey, ProgNum);
    Ret |= HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,ProgNum,HI_TRUE);
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
        Ret = AVPLAY_SetCW(hKey, ProgNum);
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
#ifdef __HI3716MV310__
    HI_UNF_SND_Mixer_Close(hTrack);
#else
	HI_UNF_SND_DestroyTrack(hTrack);
#endif

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


