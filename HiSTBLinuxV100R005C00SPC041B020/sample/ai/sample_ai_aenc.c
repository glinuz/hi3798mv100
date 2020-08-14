/******************************************************************************

  Copyright (C), 2001-2013, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_ai_aenc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/22
  Description   :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_ai.h"
#include "hi_unf_aenc.h"
#include "HA.AUDIO.AAC.encode.h"
#include "hi_adp_mpi.h"
#include "hi_error_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

static FILE* g_pAencFile;
static HI_BOOL g_bStopAencThread = HI_FALSE;
static HI_BOOL g_bSaveStream = HI_TRUE;
static pthread_t g_AencAiThd;

static HI_VOID* AencAiThread(HI_VOID *pArgs)
{
    HI_S32 Ret;
    HI_UNF_ES_BUF_S stAencStream;
    HI_HANDLE hAenc = *(HI_HANDLE*)pArgs;

    while (!g_bStopAencThread)
    {
        if (g_bSaveStream)
        {
            Ret = HI_UNF_AENC_AcquireStream(hAenc, &stAencStream, 10);
            if (HI_SUCCESS == Ret)
            {
                fwrite(stAencStream.pu8Buf, 1, stAencStream.u32BufLen, g_pAencFile);
                Ret = HI_UNF_AENC_ReleaseStream(hAenc, &stAencStream);
                if (HI_SUCCESS != Ret)
                {
                    sample_printf("call HI_UNF_AENC_ReleaseStream failed(0x%x)\n", Ret);
                    break;
                }
            }
            else if (HI_ERR_AENC_OUT_BUF_EMPTY != Ret)
            {
                sample_printf("call HI_UNF_AENC_AcquireStream failed(0x%x)\n", Ret);
                break;
            }
        }
        else
        {
            usleep(5 * 1000);
        }
    }

    return HI_NULL;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hAenc;
    HI_HANDLE hAi;
    HI_UNF_AENC_ATTR_S stAencAttr;
    HI_UNF_AI_ATTR_S stAiAttr;
    HI_CHAR InputCmd[32];

    AAC_ENC_CONFIG stPrivateConfig;

    if (argc != 3)
    {
        sample_printf(" usage:     ./sample_ai_aenc aencfile aenctype\n");
        sample_printf(" aenctype:   aac\n");
        sample_printf(" examples: \n");
        sample_printf("            ./sample_ai_aenc aencfile aac\n");
        return -1;
    }

    g_pAencFile = fopen(argv[1], "wb");
    if (!g_pAencFile)
    {
        sample_printf("open file %s error!\n", argv[1]);
        return -1;
    }

    if (!strcasecmp("aac", argv[2]))
    {
        stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    }
    else
    {
        sample_printf("unsupport aud encode type!\n");
        return -1;
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

    Ret = HI_UNF_AENC_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AENC_Init failed\n");
        goto AI_DESTROY;
    }

    Ret = HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AAC.encode.so");
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AENC_Init failed\n");
        goto AENC_DEINIT;
    }

    HA_AAC_GetDefaultConfig(&stPrivateConfig);
    HA_AAC_GetEncDefaultOpenParam(&(stAencAttr.sOpenParam), &stPrivateConfig);

    Ret = HI_UNF_AENC_Create(&stAencAttr, &hAenc);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AENC_Create failed\n");
        goto AENC_DEINIT;
    }

    Ret = HI_UNF_AENC_AttachInput(hAenc, hAi);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AENC_AttachInput failed\n");
        goto AENC_DESTROY;
    }

    Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AI_SetEnable failed\n");
        goto AENC_DETACH;
    }

    g_bStopAencThread = HI_FALSE;
    g_bSaveStream = HI_TRUE;
    pthread_create(&g_AencAiThd, HI_NULL, (HI_VOID*)AencAiThread, (HI_VOID*)(&hAenc));

    while (1)
    {
        sample_printf("\nplease input the q to quit!, r/s to toggle start/stop aenc\n\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }


        if ('r' == InputCmd[0])
        {
            HI_UNF_AENC_AttachInput(hAenc, hAi);
            g_bSaveStream = HI_TRUE;
            printf("start ai_aenc\n");
            continue;
        }

        if ('s' == InputCmd[0])
        {
            g_bSaveStream = HI_FALSE;
            HI_UNF_AENC_DetachInput(hAenc);
            printf("stop ai_aenc\n");
            continue;
        }

    }

    g_bStopAencThread = HI_TRUE;
    pthread_join(g_AencAiThd, HI_NULL);

    HI_UNF_AI_SetEnable(hAi, HI_FALSE);

AENC_DETACH:
    HI_UNF_AENC_DetachInput(hAenc);
AENC_DESTROY:
    HI_UNF_AENC_Destroy(hAenc);
AENC_DEINIT:
    HI_UNF_AENC_DeInit();
AI_DESTROY:
    HI_UNF_AI_Destroy(hAi);
AI_DEINIT:
    HI_UNF_AI_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    if (g_pAencFile)
    {
        fclose(g_pAencFile);
        g_pAencFile = HI_NULL;
    }

    return 0;
}
