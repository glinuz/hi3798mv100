/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_audio_cast.c
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
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#define CAST_FILE_SAVE_MAX_SIZE  500*1024*1024     //MB
#define CAST_SLEEP_TIME  5*1000     //5ms

HI_U32 g_u32FileSaveLimitSize;
FILE *g_pAudCastFile = HI_NULL;
static pthread_t g_CastThd;
static HI_BOOL g_StopThread = HI_FALSE;

HI_VOID CastTthread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_UNF_AO_FRAMEINFO_S  sAoFrame;
    HI_UNF_AO_FRAMEINFO_S *pstAOFrame = &sAoFrame;
    HI_U32 FrameSize = 0;
    HI_U32 u32TotalCounter=0;
    HI_U32 Frame=0;
    HI_HANDLE hCast = *(HI_HANDLE *)args;	

    while (!g_StopThread)
    {
        Ret = HI_UNF_SND_AcquireCastFrame(hCast, pstAOFrame, 0);
        if(HI_SUCCESS != Ret)
        {
            //printf("Cast Maybe Not Enable !\n");
            usleep(CAST_SLEEP_TIME);   //<=5ms  1024(PcmSamplesPerFrame)/192K(Max Samplerate)*1000
                                        // <= 20ms if Samplerate = 48K;
            continue;
        }  
#if 0
        printf("\npstAOFrame->u32Channels  = %d\n", pstAOFrame->u32Channels );
        printf("pstAOFrame->u32SampleRate  = %d\n", pstAOFrame->u32SampleRate);
        printf("pstAOFrame->s32BitPerSample  = %d\n", pstAOFrame->s32BitPerSample);
        printf("pstAOFrame->u32PcmSamplesPerFrame  = %d\n", pstAOFrame->u32PcmSamplesPerFrame);
        printf("pstAOFrame->ps32PcmBuffer  = %p\n", pstAOFrame->ps32PcmBuffer);
#endif
        
        FrameSize = pstAOFrame->u32PcmSamplesPerFrame*pstAOFrame->u32Channels*pstAOFrame->s32BitPerSample /8;
        u32TotalCounter += FrameSize;
        Frame++;
        if(!(Frame&0x3f)||Frame<4)
         printf("Frame = 0x%x\n", Frame);
        if(u32TotalCounter <= g_u32FileSaveLimitSize*1024*1024)
        {
            fwrite(pstAOFrame->ps32PcmBuffer, 1, FrameSize, g_pAudCastFile);
        }
        HI_UNF_SND_ReleaseCastFrame(hCast, pstAOFrame);
        usleep(CAST_SLEEP_TIME);   
    }
    
    if(g_pAudCastFile)
    {
        fclose(g_pAudCastFile);  
        g_pAudCastFile = HI_NULL;
    }

    return;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hCast;
    HI_UNF_SND_CAST_ATTR_S stCastAttr;
    HI_BOOL bCastEnable;
    
    HI_CHAR InputCmd[32];

    if (argc < 3)
    {
#if 1
        printf(" usage:    ./sample_audiocast  size(MB)   File(/**/**.pcm) \n");
        printf(" examples: \n");
        printf("           ./sample_audiocast 100 /mnt/cast.pcm \n");
#endif
        return 0;
    }

    g_u32FileSaveLimitSize = strtol(argv[1],NULL,0);
    if((g_u32FileSaveLimitSize <=0)||(g_u32FileSaveLimitSize > 500))
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
        g_pAudCastFile = fopen(argv[2], "wb");
        if (!g_pAudCastFile)
        {
            sample_printf("open file %s error!\n", argv[2]);
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

    Ret = HI_UNF_SND_GetDefaultCastAttr(HI_UNF_SND_0, &stCastAttr);
    if(HI_SUCCESS != Ret)
    {
        printf("Get Default Cast Attr Failed \n");
        goto SND_DEINIT;
    }

#if 1
    printf("Cast  u32PcmSamplesPerFrame:  %d\n", stCastAttr.u32PcmSamplesPerFrame);
    printf("Cast  u32PcmFrameMaxNum:  %d\n", stCastAttr.u32PcmFrameMaxNum);
#endif

    Ret = HI_UNF_SND_CreateCast(HI_UNF_SND_0,  &stCastAttr, &hCast);
    if(HI_SUCCESS != Ret)
    {
        printf("Cast Create Failed \n");
        goto SND_DEINIT;
    }

#if 1    
    printf("HI_UNF_SND_SetCastEnable   HI_TRUE\n");
    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_TRUE);
    if(HI_SUCCESS != Ret)
    {
        printf("Cast Enable Failed \n");
        goto CAST_CREATE;
    }
#endif

    g_StopThread = HI_FALSE;
    pthread_create(&g_CastThd, HI_NULL, (HI_VOID *)CastTthread, &hCast);
    while (1)
    {
        printf("please input the q to quit!, s to disable cast channel , r to resable cast channel\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
		
        if (('s' == InputCmd[0]) || ('S' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_SetCastEnable(hCast, HI_FALSE);
            if(HI_SUCCESS != Ret)
            {
                printf("Cast Disable Failed \n");
            }
        }

        if (('r' == InputCmd[0]) || ('R' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_SetCastEnable(hCast, HI_TRUE);
            if(HI_SUCCESS != Ret)
            {
                printf("Cast Enable Failed \n");
            }
        }
        
        if (('g' == InputCmd[0]) || ('G' == InputCmd[0]))
        {
            Ret = HI_UNF_SND_GetCastEnable(hCast, &bCastEnable);
            if(HI_SUCCESS != Ret)
            {
                printf("Cast GetEnable Failed \n");
            }
            else
                printf("Cast ID=0x%x  Enable=0x%x\n", (HI_U32)hCast, (HI_U32)bCastEnable);
                
        }
    }

    g_StopThread = HI_TRUE;
    pthread_join(g_CastThd, HI_NULL);

    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_FALSE);
    if(HI_SUCCESS != Ret)
    {
        printf("Cast Enable Failed \n");
    }

CAST_CREATE:
    HI_UNF_SND_DestroyCast(hCast);
SND_DEINIT:
    HIADP_Snd_DeInit();
SYS_DEINIT:
    HI_SYS_DeInit();

    if(g_pAudCastFile)
    {
        fclose(g_pAudCastFile);  
        g_pAudCastFile = HI_NULL;
    }

    return 0;
}
