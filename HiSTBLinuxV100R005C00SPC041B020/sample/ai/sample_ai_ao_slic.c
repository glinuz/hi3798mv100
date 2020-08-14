/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_ai_ao_slic.c
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
#include "hi_unf_ai.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
#include "hi_unf_gpio.h"
#include "hi_unf_spi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define CAST_FILE_SAVE_MAX_SIZE  500*1024*1024     //MB

HI_U32 g_u32FileSaveLimitSize;
FILE *g_pAIFile = HI_NULL;
static pthread_t g_AiThd;
static HI_BOOL g_StopThread = HI_FALSE;
HI_HANDLE hTrackSlave = 0;


#define AUDIO_SUPPORT_OUT  0
#define SLAC_RESET_GPIO_GNUM 2
#define SLAC_RESET_GPIO_PNUM 4
static HI_S32 g_s32SlicFd = -1;
static const HI_CHAR g_acSlicDevName[] = "/dev/le89116" ;

HI_VOID AiTthread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_UNF_AO_FRAMEINFO_S  stAiFrame;
    HI_UNF_AO_FRAMEINFO_S *pstAiFrame = &stAiFrame;
    HI_U32 FrameSize = 0;
    HI_U32 u32TotalCounter=0;
    HI_HANDLE hAi = *(HI_HANDLE *)args;

    while (!g_StopThread)
    {
        Ret = HI_UNF_AI_AcquireFrame(hAi, pstAiFrame, 0);
        FrameSize = pstAiFrame->u32PcmSamplesPerFrame*pstAiFrame->u32Channels*pstAiFrame->s32BitPerSample /8;
        u32TotalCounter += FrameSize;
        if(u32TotalCounter <= g_u32FileSaveLimitSize*1024*1024)
        {
            //sample_printf("----%s----%d------,*pstAiFrame->ps32PcmBuffer = 0x%x,FrameSize = %d\n",__FUNCTION__,__LINE__,*pstAiFrame->ps32PcmBuffer,FrameSize);
            fwrite(pstAiFrame->ps32PcmBuffer, 1, FrameSize, g_pAIFile);
        }
#ifdef AUDIO_SUPPORT_OUT
        if(FrameSize > 0)
        {

            Ret = HI_UNF_SND_SendTrackData(hTrackSlave, pstAiFrame);
            if(HI_SUCCESS != Ret)
            {
                if(HI_ERR_AO_OUT_BUF_FULL == Ret)
                {
                    sample_printf("SendTrackData Buf Full  !\n");
                }
                else
                    sample_printf("SendTrackData Failed  !\n");
            }

        }
#endif

        HI_UNF_AI_ReleaseFrame(hAi, pstAiFrame);
    }
    if(g_pAIFile)
    {
        fclose(g_pAIFile);
        g_pAIFile = HI_NULL;
    }
    return;
}

HI_S32  SlicDevReset(HI_VOID)
{
    HI_UNF_GPIO_Init();
    HI_UNF_GPIO_SetDirBit(SLAC_RESET_GPIO_GNUM * 8 + SLAC_RESET_GPIO_PNUM, HI_FALSE);
    HI_UNF_GPIO_WriteBit(SLAC_RESET_GPIO_GNUM * 8 + SLAC_RESET_GPIO_PNUM, HI_FALSE);
    usleep(3 * 1000);
    HI_UNF_GPIO_WriteBit(SLAC_RESET_GPIO_GNUM * 8 + SLAC_RESET_GPIO_PNUM, HI_FALSE);
    usleep(30 * 1000);
    HI_UNF_GPIO_WriteBit(SLAC_RESET_GPIO_GNUM * 8 + SLAC_RESET_GPIO_PNUM, HI_TRUE);
    HI_UNF_GPIO_DeInit();
    return HI_SUCCESS;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hAi;
    HI_UNF_AI_ATTR_S stAitAttr;

    HI_CHAR InputCmd[32];

    if (argc < 3)
    {
#if 1
        sample_printf(" usage: sample_ai_ao_slic  size(MB)   File(/**/**.pcm) \n");
        sample_printf(" examples: \n");
        sample_printf(" sample_ai_ao_slic 100 /mnt/ai.pcm \n");
#endif
        return 0;
    }

    g_u32FileSaveLimitSize = strtol(argv[1],NULL,0);
    if((g_u32FileSaveLimitSize <=0)||(g_u32FileSaveLimitSize > 500))
    {
        sample_printf("File Save Length Limit %d is not set or larger than 500MB\n", g_u32FileSaveLimitSize);
        return 0;
    }
    else
    {
        sample_printf("File Save Length Limit %d(MB)\n", g_u32FileSaveLimitSize);
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
        sample_printf("call AiInit failed.\n");
        goto SYS_DEINIT;
    }
    Ret = HI_UNF_AI_GetDefaultAttr(HI_UNF_AI_I2S0,&stAitAttr);
    stAitAttr.u32PcmFrameMaxNum = 8;
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Get Default Ai Attr Failed \n");
        goto AI_DEINIT;
    }
    stAitAttr.unAttr.stI2sAttr.stAttr.enBclkSel = HI_UNF_I2S_BCLK_2_DIV;
    stAitAttr.enSampleRate = HI_UNF_SAMPLE_RATE_8K;
    stAitAttr.unAttr.stI2sAttr.stAttr.enI2sMode = HI_UNF_I2S_PCM_MODE;
    stAitAttr.unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_1;
    stAitAttr.unAttr.stI2sAttr.stAttr.enPcmDelayCycle = HI_UNF_I2S_PCM_1_DELAY;
    stAitAttr.unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = HI_FALSE;

    Ret = HI_UNF_AI_Create(HI_UNF_AI_I2S0, &stAitAttr, &hAi);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Ai Create Failed \n");
        goto AI_DEINIT;
    }

#ifdef AUDIO_SUPPORT_OUT
    HI_UNF_SND_ATTR_S stAttr;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    Ret = HI_UNF_SND_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Init failed.\n");
        goto AI_DEINIT;
    }

    Ret = HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_1, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_GetDefaultOpenAttr failed.\n");
        goto  SND_DEINIT;
    }

    stAttr.u32PortNum = 1;
    stAttr.stOutport[0].enOutPort = HI_UNF_SND_OUTPUTPORT_I2S0;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.bMaster = HI_TRUE;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enI2sMode = HI_UNF_I2S_PCM_MODE;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enMclkSel = HI_UNF_I2S_MCLK_256_FS;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enBclkSel = HI_UNF_I2S_BCLK_2_DIV;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_1;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = HI_FALSE;
    stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enPcmDelayCycle = HI_UNF_I2S_PCM_17_DELAY;
    stAttr.enSampleRate = HI_UNF_SAMPLE_RATE_8K;

    Ret = HI_UNF_SND_Open(HI_UNF_SND_1, &stAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Open  HI_UNF_SND_1 failed.\n");
        goto SND_DEINIT;
    }

    HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);

    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_1,&stTrackAttr,&hTrackSlave);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("HI_UNF_SND_CreateTrack err:0x%x\n", Ret);
        goto SND_DEINIT;
    }
#endif

    SlicDevReset();
    usleep(100* 1000);
    g_s32SlicFd = open(g_acSlicDevName, O_RDWR, 0);
    if (g_s32SlicFd < 0)
    {
        sample_printf("OpenSlicDevice err\n");
        g_s32SlicFd = -1;
        goto SND_DEINIT;
    }

    Ret = HI_UNF_AI_SetEnable(hAi, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto AI_DISABLE;
    }
    g_StopThread = HI_FALSE;
    pthread_create(&g_AiThd, HI_NULL, (HI_VOID *)AiTthread, &hAi);

    while (1)
    {
        sample_printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            sample_printf("prepare to quit!\n");
            break;
        }

        if ('s' == InputCmd[0] || 'S' == InputCmd[0])
        {
            static int s_toggle =0;
            s_toggle++;

            Ret = HI_UNF_AI_SetEnable(hAi, HI_FALSE);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call HI_UNF_SND_Attach failed.\n");
            }
        }
    }

    g_StopThread = HI_TRUE;
    pthread_join(g_AiThd, HI_NULL);
    HI_UNF_AI_SetEnable(hAi, HI_FALSE);

AI_DISABLE:
    if(g_s32SlicFd)
    {
        close(g_s32SlicFd);
        g_s32SlicFd = HI_NULL;
    }
SND_DEINIT:
    HI_UNF_SND_Close(HI_UNF_SND_1);
    HI_UNF_SND_DeInit();
AI_DEINIT:
    HI_UNF_AI_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    if(g_pAIFile)
    {
        fclose(g_pAIFile);
        g_pAIFile = HI_NULL;
    }
    return 0;
}

