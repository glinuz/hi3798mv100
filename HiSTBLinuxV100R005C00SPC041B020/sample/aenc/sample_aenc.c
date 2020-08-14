/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : sample_g711_aenc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/03/15
  Description   :
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_adp_mpi.h"
#include "hi_unf_aenc.h"

#include "HA.AUDIO.AAC.encode.h"
#include "HA.AUDIO.G711.codec.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AMRWB.codec.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

static HI_U32 g_u32SampleRateIn = 0;
static HI_U32 g_u32ChannelsIn = 0;

static HI_HANDLE g_hAenc = HI_NULL;
static FILE* pOutFile = HI_NULL;
static FILE* pInFile = HI_NULL;

static pthread_t pthSend;
static pthread_t pthAcquire;
static HI_BOOL   bStopThread = HI_FALSE;

#define AENC_IN_PACKET_SIZE  (1024 * 3)

static HI_VOID AencFrameInit(HI_UNF_AO_FRAMEINFO_S* pstAencFrame)
{
    memset(pstAencFrame, 0, sizeof(HI_UNF_AO_FRAMEINFO_S));

    // g711 encoder only support 8k/16bit/mono
    pstAencFrame->u32SampleRate = g_u32SampleRateIn;
    pstAencFrame->s32BitPerSample = 16;
    pstAencFrame->u32Channels = g_u32ChannelsIn;
    pstAencFrame->u32PcmSamplesPerFrame = AENC_IN_PACKET_SIZE / (pstAencFrame->u32Channels * 2);
    pstAencFrame->bInterleaved = HI_TRUE;
}

static HI_VOID* AencAcquireFrameThread(HI_VOID* arg)
{
    HI_S32 s32Ret;
    HI_U32 u32Frames = 0;
    HI_UNF_ES_BUF_S stFrame = {0};

    while (HI_FALSE == bStopThread)
    {
        s32Ret = HI_UNF_AENC_AcquireStream(g_hAenc, &stFrame, 0);
        if (HI_SUCCESS == s32Ret)
        {
            if (pOutFile)
            {
                fwrite(stFrame.pu8Buf, 1, stFrame.u32BufLen, pOutFile);
            }

            u32Frames++;
            if (0 == u32Frames % 10240)
            {
                sample_printf("HI_UNF_AENC_AcquireStream times = %d\n", u32Frames);
            }

            s32Ret = HI_UNF_AENC_ReleaseStream(g_hAenc, &stFrame);
            if (HI_SUCCESS != s32Ret)
            {
                sample_printf("HI_UNF_AENC_ReleaseStream failed(0x%x)\n", s32Ret);
                return HI_NULL;
            }
        }
        else
        {
            usleep(5 * 1000);
        }
    }

    return HI_NULL;
}

static HI_VOID* AencSendFrameThread(HI_VOID* arg)
{
    HI_S32 s32Ret;
    HI_S32 s32ReadSize;
    HI_U32 u32EncodeFrame = 0;
    HI_BOOL bSend = HI_TRUE;
    HI_VOID* pData = HI_NULL;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;

    AencFrameInit(&stAOFrame);

    pData = (HI_VOID*)malloc(AENC_IN_PACKET_SIZE);
    if (HI_NULL == pData)
    {
        sample_printf("malloc buffer failed!\n");
        return HI_NULL;
    }

    stAOFrame.ps32PcmBuffer = (HI_S32*)pData;

    while (HI_FALSE == bStopThread)
    {
        if (HI_TRUE == bSend)
        {
            s32ReadSize = fread(pData, 1, AENC_IN_PACKET_SIZE, pInFile);
            if (AENC_IN_PACKET_SIZE != s32ReadSize)
            {
                sample_printf("read file end and rewind!\n");
                rewind(pInFile);
                continue;
            }
        }

        stAOFrame.u32FrameIndex = u32EncodeFrame;
        s32Ret = HI_UNF_AENC_SendFrame(g_hAenc, &stAOFrame);
        if (HI_SUCCESS == s32Ret)
        {
            bSend = HI_TRUE;
            u32EncodeFrame++;
        }
        else
        {
            bSend = HI_FALSE;
            usleep(10 * 1000);
        }
    }

    return HI_NULL;
}

static HI_S32 AACAencCreate(HI_HANDLE* phAenc)
{
    HI_S32 s32Ret;
    HI_UNF_AENC_ATTR_S stAencAttr;
    AAC_ENC_CONFIG stPrivateConfig;

    if (1 != g_u32ChannelsIn && 2 != g_u32ChannelsIn)
    {
        sample_printf("aac encoder only support stero/mono input!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AAC.encode.so");
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_RegisterEncoder failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    sample_printf("register \"libHA.AUDIO.AAC.encode.so\" success!\n");

    memset(&stAencAttr, 0, sizeof(HI_UNF_AENC_ATTR_S));
    memset(&stPrivateConfig, 0, sizeof(AAC_ENC_CONFIG));

    stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    HA_AAC_GetDefaultConfig(&stPrivateConfig);
    stPrivateConfig.sampleRate = g_u32SampleRateIn;

    HA_AAC_GetEncDefaultOpenParam(&stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);

    s32Ret = HI_UNF_AENC_Create(&stAencAttr, phAenc);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Create failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 G711AencCreate(HI_HANDLE* phAenc)
{
    HI_S32 s32Ret;
    HI_UNF_AENC_ATTR_S stAencAttr;
    G711_ENCODE_OPENCONFIG_S stPrivateConfig;

    if (1 != g_u32ChannelsIn || 8000 != g_u32SampleRateIn)
    {
        sample_printf("g711 encoder only support 8k/16bit/mono input!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.G711.codec.so");
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_RegisterEncoder failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    sample_printf("register \"libHA.AUDIO.G711.codec.so\" success!\n");

    memset(&stAencAttr, 0, sizeof(HI_UNF_AENC_ATTR_S));
    memset(&stPrivateConfig, 0, sizeof(G711_ENCODE_OPENCONFIG_S));

    stAencAttr.enAencType = HA_AUDIO_ID_G711;
    stPrivateConfig.isAlaw = 1;
    stPrivateConfig.bVAD   = HI_TRUE;

    HA_G711_GetEncDefaultOpenParam(&stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);
    stAencAttr.sOpenParam.u32SamplePerFrame = G711_FRAME_LEN;

    s32Ret = HI_UNF_AENC_Create(&stAencAttr, phAenc);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Create failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AMRWBAencCreate(HI_HANDLE* phAenc)
{
    HI_S32 s32Ret;
    HI_UNF_AENC_ATTR_S stAencAttr;
    AMRWB_ENCODE_OPENCONFIG_S stPrivateConfig;

    if (1 != g_u32ChannelsIn || 16000 != g_u32SampleRateIn)
    {
        sample_printf("amw-wb encoder only support 16k/16bit/mono input!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AMRWB.codec.so");
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_RegisterEncoder failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    sample_printf("register \"libHA.AUDIO.AMRWB.codec.so\" success!\n");

    memset(&stAencAttr, 0, sizeof(HI_UNF_AENC_ATTR_S));
    memset(&stPrivateConfig, 0, sizeof(AMRWB_ENCODE_OPENCONFIG_S));

    stAencAttr.enAencType = HA_AUDIO_ID_AMRWB;
    stPrivateConfig.enFormat = AMRWB_FORMAT_MIME;
    stPrivateConfig.enMode = AMRWB_MR2385;
    stPrivateConfig.bDTX = HI_FALSE;

    HA_AMRWB_GetEncDefaultOpenParam(&stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);

    s32Ret = HI_UNF_AENC_Create(&stAencAttr, phAenc);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Create failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AMRNBAencCreate(HI_HANDLE* phAenc)
{
    HI_S32 s32Ret;
    HI_UNF_AENC_ATTR_S stAencAttr;
    AMRNB_ENCODE_OPENCONFIG_S stPrivateConfig;

    if (1 != g_u32ChannelsIn || 8000 != g_u32SampleRateIn)
    {
        sample_printf("amw-nb encoder only support 8k/16bit/mono input!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AENC_RegisterEncoder("libHA.AUDIO.AMRNB.codec.so");
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_RegisterEncoder failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    sample_printf("register \"libHA.AUDIO.AMRNB.codec.so\" success!\n");

    memset(&stAencAttr, 0, sizeof(HI_UNF_AENC_ATTR_S));
    memset(&stPrivateConfig, 0, sizeof(AMRNB_ENCODE_OPENCONFIG_S));

    stAencAttr.enAencType = HA_AUDIO_ID_AMRNB;
    stPrivateConfig.enFormat = AMRNB_MIME;
    stPrivateConfig.enMode = AMRNB_MR475;
    stPrivateConfig.bDTX = HI_FALSE;

    HA_AMRNB_GetEncDefaultOpenParam(&stAencAttr.sOpenParam, (HI_VOID*)&stPrivateConfig);

    s32Ret = HI_UNF_AENC_Create(&stAencAttr, phAenc);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Create failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AencCreate(HA_CODEC_ID_E enAencType)
{
    switch (enAencType)
    {
        case HA_AUDIO_ID_AAC:
            return AACAencCreate(&g_hAenc);

        case HA_AUDIO_ID_G711:
            return G711AencCreate(&g_hAenc);

        case HA_AUDIO_ID_AMRWB:
            return AMRWBAencCreate(&g_hAenc);

        case HA_AUDIO_ID_AMRNB:
            return AMRNBAencCreate(&g_hAenc);

        default:
            sample_printf("unsupport aud codec type!\n");
            return HI_FAILURE;
    }
}

HI_S32 main(int argc, char* argv[])
{
    HI_S32 s32Ret = HI_FAILURE;
    HA_CODEC_ID_E enAencType;

    if (6 != argc)
    {
        printf("Usage:   ./sample_aenc infileName inChannel inSamplerate outfileName outfileType\n"
               "         infileName: input file name\n"
               "         inChannel:1 2\n"
               "         insamplerate:8000 16000\n"
               "         outfileName: output file name\n"
               "         outfileType:aac g711 amrwb awrnb\n"
              );
        printf("Example: ./sample_aenc ./test.wav 1 8000 ./test.g711 g711\n");

        return HI_FAILURE;
    }

    pInFile = fopen(argv[1], "rb");
    if (!pInFile)
    {
        sample_printf("open file %s error!\n", argv[1]);
        return -1;
    }

    pOutFile = fopen(argv[4], "wb");
    if (!pOutFile)
    {
        sample_printf("open file %s error!\n", argv[4]);
        goto CLOSE_FILES;
    }

    g_u32ChannelsIn = atoi(argv[2]);
    g_u32SampleRateIn = atoi(argv[3]);

    if (!strcasecmp("aac", argv[5]))
    {
        enAencType = HA_AUDIO_ID_AAC;
    }
    else if (!strcasecmp("g711", argv[5]))
    {
        enAencType = HA_AUDIO_ID_G711;
    }
    else if (!strcasecmp("amrwb", argv[5]))
    {
        enAencType = HA_AUDIO_ID_AMRWB;
        /* write magic number AMR file */
        fwrite(AMRWB_MAGIC_NUMBER, 1, strlen(AMRWB_MAGIC_NUMBER), pOutFile);
    }
    else if (!strcasecmp("amrnb", argv[5]))
    {
        enAencType = HA_AUDIO_ID_AMRNB;
        /* write magic number AMR file */
        fwrite(AMR_MAGIC_NUMBER, 1, strlen(AMR_MAGIC_NUMBER), pOutFile);
    }
    else
    {
        sample_printf("unsupport aud codec type!\n");
        goto CLOSE_FILES;
    }

    s32Ret = HI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_SYS_Init failed(0x%x)\n", s32Ret);
        goto CLOSE_FILES;
    }

    s32Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HIADP_Snd_Init failed(0x%x)\n", s32Ret);
        goto SYS_DEINIT;
    }

    s32Ret = HI_UNF_AENC_Init();
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Init failed(0x%x)\n", s32Ret);
        goto SND_DEINIT;
    }

    s32Ret = AencCreate(enAencType);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("AencCreate failed(0x%x)\n", s32Ret);
        goto AENC_DEINIT;
    }

    s32Ret = HI_UNF_AENC_Start(g_hAenc);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("HI_UNF_AENC_Start failed(0x%x)\n", s32Ret);
        goto AENC_DESTORY;
    }

    s32Ret = pthread_create(&pthAcquire, HI_NULL, AencAcquireFrameThread, HI_NULL);
    s32Ret |= pthread_create(&pthSend, HI_NULL, AencSendFrameThread, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        sample_printf("pthread_create failed(0x%x)\n", s32Ret);
        goto AENC_STOP;
    }

    printf("\nPress any key to exit\n");
    getchar();

    bStopThread = HI_TRUE;
    pthread_join(pthSend, HI_NULL);
    pthread_join(pthAcquire, HI_NULL);

AENC_STOP:
    (HI_VOID)HI_UNF_AENC_Stop(g_hAenc);

AENC_DESTORY:
    (HI_VOID)HI_UNF_AENC_Destroy(g_hAenc);

AENC_DEINIT:
    (HI_VOID)HI_UNF_AENC_DeInit();

SND_DEINIT:
    (HI_VOID)HIADP_Snd_DeInit();

SYS_DEINIT:
    (HI_VOID)HI_SYS_DeInit();

CLOSE_FILES:
    if (HI_NULL != pInFile)
    {
        fclose(pInFile);
    }

    if (HI_NULL != pOutFile)
    {
        fclose(pOutFile);
    }

    return s32Ret;
}
