/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_aenc_cast.c
  Version       : Initial Draft
  Author        : Hisilicon audio
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
#include "hi_unf_aenc.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.AAC.encode.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define CAST_SLEEP_TIME 5 * 1000     //5ms

FILE *g_pAudCastFile = HI_NULL;
static pthread_t g_CastThd;
static HI_BOOL g_StopThread = HI_FALSE;

static HI_CHAR* g_HA_EncoderDllName = {"libHA.AUDIO.AAC.encode.so"};

HI_VOID CastTthread(HI_VOID *args)
{
    HI_UNF_ES_BUF_S sAencFrame;
    HI_UNF_ES_BUF_S *pstAencFrame = &sAencFrame;
    HI_S32 Ret;
    HI_S32 nFrame   = 0;
    HI_HANDLE hAenc = *(HI_HANDLE *)args;

    while (!g_StopThread)
    {
        Ret = HI_UNF_AENC_AcquireStream(hAenc, pstAencFrame, 0);
        if (HI_SUCCESS == Ret)
        {
            if (g_pAudCastFile)
            {
                fwrite(pstAencFrame->pu8Buf, 1, pstAencFrame->u32BufLen, g_pAudCastFile);
            }

            nFrame++;
            if ((nFrame % 256 == 0))
            {
                sample_printf("Cast Get AAC Frame(%d)\n", nFrame);
            }

            Ret = HI_UNF_AENC_ReleaseStream(hAenc, pstAencFrame);
        }

        usleep(CAST_SLEEP_TIME);
    }

    if (g_pAudCastFile)
    {
        fclose(g_pAudCastFile);
        g_pAudCastFile = HI_NULL;
    }

    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hCast = HI_NULL;
    HI_UNF_SND_CAST_ATTR_S stCastAttr;
    HI_BOOL bCastEnable;
    AAC_ENC_CONFIG stPrivateConfig;
    HI_UNF_AENC_ATTR_S stAencAttr;
    HI_HANDLE hAenc = HI_NULL;
    HI_CHAR InputCmd[32];

    if (argc < 2)
    {
#if 1
        sample_printf(" usage:    ./sample_castaenc  filename(*.aac) \n");
        sample_printf(" examples: \n");
        sample_printf("           ./sample_castaenc  /mnt/cast.aac \n");
#endif
        return 0;
    }

    if (strcasecmp("null", argv[1]))
    {
        g_pAudCastFile = fopen(argv[1], "wb");
        if (!g_pAudCastFile)
        {
            sample_printf("open file %s error!\n", argv[1]);
            return -1;
        }
    }

    HI_SYS_Init();
    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call SndInit failed.\n");
        goto SYS_DEINIT;
    }

    /********* CAST  **********/
    Ret = HI_UNF_SND_GetDefaultCastAttr(HI_UNF_SND_0, &stCastAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Get Default Cast Attr Failed \n");
        goto SND_DEINIT;
    }

#if 1
    sample_printf("Cast  u32PcmSamplesPerFrame:  %d\n", stCastAttr.u32PcmSamplesPerFrame);
    sample_printf("Cast  u32PcmFrameMaxNum:  %d\n", stCastAttr.u32PcmFrameMaxNum);
#endif
    Ret = HI_UNF_SND_CreateCast(HI_UNF_SND_0, &stCastAttr, &hCast);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Cast Create Failed \n");
        goto CAST_DEINIT;
    }

    /********* AENC  **********/
    stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    HA_AAC_GetDefaultConfig(&stPrivateConfig);
    stPrivateConfig.sampleRate = HI_UNF_SAMPLE_RATE_48K;

    HA_AAC_GetEncDefaultOpenParam(&stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);
    sample_printf("DesiredSampleRate=%d\n", stAencAttr.sOpenParam.u32DesiredSampleRate);

    Ret = HI_UNF_AENC_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("AENC create Failed \n");
        goto ANEC_DEINIT;
    }

    Ret = HI_UNF_AENC_RegisterEncoder(g_HA_EncoderDllName);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("AENC create Failed \n");
        goto ANEC_DEINIT;
    }

    Ret = HI_UNF_AENC_Create(&(stAencAttr), &(hAenc));
    if (HI_SUCCESS != Ret)
    {
        sample_printf("AENC create Failed \n");
        goto ANEC_DEINIT;
    }

    Ret = HI_UNF_AENC_AttachInput(hAenc, hCast);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("AttachInput Cast  Failed \n");
        goto ANEC_DEINIT;
    }

    Ret = HI_UNF_AENC_Start(hAenc);

#if 1
    sample_printf("HI_UNF_SND_SetCastEnable   HI_TRUE\n");
    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
        goto ANEC_DEINIT;
    }
#endif


    g_StopThread = HI_FALSE;
    pthread_create(&g_CastThd, HI_NULL, (HI_VOID *)CastTthread, &hAenc);
    while (1)
    {
        sample_printf("please input the q to quit!, s to disable cast channel , r to resable cast channel\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        if (('s' == InputCmd[0]) || ('S' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_SetCastEnable(hCast, HI_FALSE);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("Cast Disable Failed \n");
            }
        }

        if (('r' == InputCmd[0]) || ('R' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_SetCastEnable(hCast, HI_TRUE);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("Cast Enable Failed \n");
            }
        }

        if (('g' == InputCmd[0]) || ('G' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_GetCastEnable(hCast, &bCastEnable);
            if (HI_SUCCESS != Ret)
            {
                sample_printf("Cast GetEnable Failed \n");
            }
            else
            {
                sample_printf("Cast ID=0x%x  Enable=0x%x\n", (HI_U32)hCast, (HI_U32)bCastEnable);
            }
        }
    }

    g_StopThread = HI_TRUE;
    pthread_join(g_CastThd, HI_NULL);

    Ret = HI_UNF_AENC_Stop(hAenc);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
    }

    Ret = HI_UNF_AENC_DetachInput(hAenc);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
    }

    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
    }

ANEC_DEINIT:
    HI_UNF_AENC_Destroy(hAenc);
    HI_UNF_AENC_DeInit();

CAST_DEINIT:
    HI_UNF_SND_DestroyCast(hCast);
SND_DEINIT:
    HIADP_Snd_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    if (g_pAudCastFile)
    {
        fclose(g_pAudCastFile);
        g_pAudCastFile = HI_NULL;
    }

    return 0;
}
