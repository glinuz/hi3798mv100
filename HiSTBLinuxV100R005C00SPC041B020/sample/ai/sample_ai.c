/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_ai.c
  Version       : Initial Draft
  Author        : Hisilicon audio
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_ai.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define CAST_FILE_SAVE_MAX_SIZE  (500 * 1024 * 1024)

static HI_U32 g_u32FileSaveLimitSize;
static FILE* g_pAIFile = HI_NULL;
static pthread_t g_AiThd;
static HI_BOOL g_StopThread = HI_FALSE;

static HI_VOID AiThread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_UNF_AO_FRAMEINFO_S stAiFrame;
    HI_U32 FrameSize = 0;
    HI_U32 u32TotalCounter=0;
    HI_HANDLE hAi = *(HI_HANDLE*)args;

    while (!g_StopThread)
    {
        Ret = HI_UNF_AI_AcquireFrame(hAi, &stAiFrame, 0);
        if (HI_SUCCESS != Ret)
        {
            usleep(10 * 1000);
            continue;
        }

        FrameSize = stAiFrame.u32PcmSamplesPerFrame * stAiFrame.u32Channels * stAiFrame.s32BitPerSample / 8;
        u32TotalCounter += FrameSize;

        if (u32TotalCounter <= g_u32FileSaveLimitSize * 1024 * 1024)
        {
            fwrite(stAiFrame.ps32PcmBuffer, 1, FrameSize, g_pAIFile);
        }

        Ret = HI_UNF_AI_ReleaseFrame(hAi, &stAiFrame);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("[%s] HI_UNF_AI_ReleaseFrame failed 0x%x\n", __FUNCTION__, Ret);
        }
    }
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hAi;
    HI_UNF_AI_ATTR_S stAiAttr;

    HI_CHAR InputCmd[32];

    if (argc < 3)
    {
        printf(" usage: sample_ai  size(MB)   File(/**/**.pcm) \n");
        printf(" examples: \n");
        printf(" sample_ai 100 /mnt/ai.pcm \n");
        return 0;
    }

    g_u32FileSaveLimitSize = strtol(argv[1], HI_NULL, 0);
    if ((g_u32FileSaveLimitSize <= 0) || (g_u32FileSaveLimitSize > 500))
    {
        printf("File Save Length Limit %d is not set or larger than 500MB\n", g_u32FileSaveLimitSize);
        return 0;
    }
    else
    {
        printf("File Save Length Limit %d(MB)\n", g_u32FileSaveLimitSize);
    }

    if (strcasecmp("null", argv[2]))
    {
        g_pAIFile = fopen(argv[2], "wb");
        if (!g_pAIFile)
        {
            sample_printf("open file %s error!\n", argv[2]);
            return -1;
        }
    }

    HI_SYS_Init();

    Ret = HI_UNF_AI_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_Init failed\n");
        goto SYS_DEINIT;
    }

    Ret = HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_I2S0, &stAiAttr);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_GetDefaultAttr failed\n");
        goto AI_DEINIT;
    }

    Ret = HI_UNF_AI_Create(HI_UNF_AI_I2S0, &stAiAttr, &hAi);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_Create failed\n");
        goto AI_DEINIT;
    }

    Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_SetEnable failed\n");
        goto AI_DESTROY;
    }

    g_StopThread = HI_FALSE;
    pthread_create(&g_AiThd, HI_NULL, (HI_VOID*)AiThread, &hAi);

    while (1)
    {
        sample_printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }
    }

    g_StopThread = HI_TRUE;
    pthread_join(g_AiThd, HI_NULL);

    Ret = HI_UNF_AI_SetEnable(hAi, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_SetEnable failed\n");
    }

AI_DESTROY:
    HI_UNF_AI_Destroy(hAi);
AI_DEINIT:
    HI_UNF_AI_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    if (g_pAIFile)
    {
        fclose(g_pAIFile);
        g_pAIFile = HI_NULL;
    }

    return 0;
}
