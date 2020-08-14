/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_mixengine.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/03/15
  Description   :
******************************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_unf_sound.h"
#include "hi_adp.h"
#include "hi_adp_mpi.h"

#include "hi_unf_common.h"

typedef struct
{
    HI_U32              portIdx;

    HI_HANDLE           hTrack;
    HI_U32              SampleRate;
    HI_U32              Channels;
    HI_U32              u32Weight;

    FILE*               pAudMixFile;
    pthread_t           MixerThd;
    HI_BOOL             StopThread;
} AudioMixer_State_S;

#define HI_SND_MIXER_NUMBER 8
static AudioMixer_State_S stMixer[HI_SND_MIXER_NUMBER];

static HI_S32 MIXENGINE_Snd_Init(HI_VOID)
{
    HI_S32 Ret;
    HI_UNF_SND_ATTR_S stAttr;

    Ret = HI_UNF_SND_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Init failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultOpenAttr failed.\n");
        return Ret;
    }

    stAttr.u32SlaveOutputBufSize = 1024 * 16; /* in order to increase the reaction of stop/start, the buf cannot too big*/

    Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Open failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 MIXENGINE_Snd_DeInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_SND_Close(HI_UNF_SND_0);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Close failed.\n");
        return Ret;
    }

    Ret = HI_UNF_SND_DeInit();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 AudioMixerThread(HI_VOID* args)
{
    HI_VOID* PmcBuf = HI_NULL;
    HI_S32 s32Ret;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;
    HI_BOOL Interleaved = HI_TRUE;
    HI_S32 BitPerSample = 16;
    HI_S32 PcmSamplesPerFrame = 1024;
    AudioMixer_State_S* pstMixer;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
    HI_UNF_SND_GAIN_ATTR_S stMixWeight;

    HI_BOOL bSendPending = HI_FALSE;
    HI_U32 u32NeedSize = 0, Readlen = 0;

    pstMixer = (AudioMixer_State_S*)args;

    if (0 == pstMixer->portIdx)
    {
        s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    }
    else
    {
        s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_SLAVE, &stTrackAttr);
    }
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_SND_GetDefaultTrackAttr failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0, &stTrackAttr, &pstMixer->hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_SND_CreateTrack failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    stMixWeight.bLinearMode = HI_TRUE;
    stMixWeight.s32Gain = (HI_S32)pstMixer->u32Weight;
    s32Ret = HI_UNF_SND_SetTrackWeight(pstMixer->hTrack, &stMixWeight);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_SND_CreateTrack failed(0x%x)!\n", s32Ret);
        goto TRACK_DEINIT;
    }

    stAOFrame.s32BitPerSample = BitPerSample;
    stAOFrame.u32Channels   = pstMixer->Channels;
    stAOFrame.bInterleaved  = Interleaved;
    stAOFrame.u32SampleRate = (HI_U32)(pstMixer->SampleRate);
    stAOFrame.u32PtsMs = 0xffffffff;
    stAOFrame.ps32BitsBuffer = HI_NULL;
    stAOFrame.u32BitsBytesPerFrame = 0;
    stAOFrame.u32FrameIndex = 0;
    stAOFrame.u32PcmSamplesPerFrame = PcmSamplesPerFrame;

    u32NeedSize = PcmSamplesPerFrame * pstMixer->Channels * sizeof(HI_S16);

    PmcBuf = (HI_VOID*)malloc(u32NeedSize);
    if (HI_NULL == PmcBuf)
    {
        printf("malloc pcm buffer failed!\n");
        goto TRACK_DEINIT;
    }
    stAOFrame.ps32PcmBuffer = (HI_S32*)(PmcBuf);

    while (HI_TRUE != pstMixer->StopThread)
    {
        if (HI_FALSE == bSendPending)
        {
            Readlen = fread(PmcBuf, 1, u32NeedSize, pstMixer->pAudMixFile);
            if (Readlen != u32NeedSize)
            {
                printf("mixer(%d) reach end of mix file\n", pstMixer->portIdx);
                rewind(pstMixer->pAudMixFile);
                continue;
            }
        }

        s32Ret = HI_UNF_SND_SendTrackData(pstMixer->hTrack, &stAOFrame);
        if (HI_SUCCESS == s32Ret)
        {
            bSendPending = HI_FALSE;
            continue;
        }
        else if (HI_FAILURE == s32Ret)
        {
            printf("HI_UNF_SND_SendTrackData failed(0x%x)!\n", s32Ret);
            break;
        }
        else
        {
            usleep(5 * 1000);
            bSendPending = HI_TRUE;
            continue;
        }
    }

    if (HI_NULL != PmcBuf)
    {
        free(PmcBuf);
        PmcBuf = HI_NULL;
    }
TRACK_DEINIT:
    s32Ret = HI_UNF_SND_DestroyTrack(pstMixer->hTrack);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_UNF_SND_DestroyTrack failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    return s32Ret;
}

HI_S32 main(int argc, char* argv[])
{
    HI_U32 portIdx;
    HI_CHAR InputCmd[32];
    HI_U32 MixNumber;
    HI_UNF_ENC_FMT_E enDefaultFmt = HI_UNF_ENC_FMT_720P_50;
    HI_S32 s32Ret = HI_SUCCESS;

    //sample_mixengine only support pcm stream
    if (argc < 5)
    {
        printf("Usage:    ./sample_mixengine inputfile0 trackweight0 samplerate0 inchannels0 inputfile1 trackweight1 samplerate1 inchannels1 ...\n");
        printf("examples:\n");
        printf("          ./sample_mixengine inputfile0 100 48000 2 inputfile1 100 44100 1\n");
        return HI_FAILURE;
    }

    HI_SYS_Init();

    s32Ret = HIADP_Disp_Init(enDefaultFmt);
    if (s32Ret != HI_SUCCESS)
    {
        printf("HIADP_Disp_Init failed(0x%x)!\n", s32Ret);
        goto SYS_DEINIT;
    }

    s32Ret = MIXENGINE_Snd_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("MIXENGINE_Snd_Init failed(0x%x)!\n", s32Ret);
        goto DISP_DEINIT;
    }

    MixNumber  = argc - 1;
    MixNumber /= 4;

    for (portIdx = 0; portIdx < MixNumber; portIdx++)
    {
        stMixer[portIdx].pAudMixFile = fopen(argv[4 * portIdx + 0 + 1], "rb");
        if (HI_NULL == stMixer[portIdx].pAudMixFile)
        {
            printf("open file %s error!\n", argv[portIdx + 1]);
            s32Ret = HI_FAILURE;
            goto SND_DEINIT;
        }

        stMixer[portIdx].u32Weight = atoi(argv[4 * portIdx + 1 + 1]);
        stMixer[portIdx].SampleRate = atoi(argv[4 * portIdx + 2 + 1]);
        stMixer[portIdx].Channels = atoi(argv[4 * portIdx + 3 + 1]);
        stMixer[portIdx].portIdx = portIdx;
        stMixer[portIdx].StopThread = HI_FALSE;
        stMixer[portIdx].MixerThd = (pthread_t)NULL;
        printf("\n create mixer(%d) , sampleRate(%d) ch(%d)\n", portIdx,
               stMixer[portIdx].SampleRate, stMixer[portIdx].Channels);

        pthread_create(&(stMixer[portIdx].MixerThd), (const pthread_attr_t*)HI_NULL, (HI_VOID*)AudioMixerThread,
                       (HI_VOID*)(&stMixer[portIdx]));
    }

    while (1)
    {
        printf("\n please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);
        portIdx = (HI_U32)((InputCmd[1] - 0x30));
        if (portIdx >= HI_SND_MIXER_NUMBER)
        {
            portIdx = HI_SND_MIXER_NUMBER - 1;
        }

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            for (portIdx = 0; portIdx < MixNumber; portIdx++)
            {
                stMixer[portIdx].StopThread = 0;
            }

            break;
        }
    }

    sleep(1);

    for (portIdx = 0; portIdx < MixNumber; portIdx++)
    {
        stMixer[portIdx].StopThread = HI_TRUE;
        pthread_join(stMixer[portIdx].MixerThd, HI_NULL);
        fclose(stMixer[portIdx].pAudMixFile);
    }

SND_DEINIT:
    MIXENGINE_Snd_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return s32Ret;
}
