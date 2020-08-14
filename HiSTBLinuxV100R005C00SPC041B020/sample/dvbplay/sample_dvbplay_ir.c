/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : dvbplay_ir.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : w58735
    Modification: Created file

******************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;

static HI_S32 s_s32TaskRunning = 1;
static HI_S32 s_s32ChnNum  = 0;
static HI_S32 s_s32ProgNum = 0;
static HI_S32 s_s32Vol = 100;

HI_UNF_ENC_FMT_E g_enDefaultFmt  = HI_UNF_ENC_FMT_1080i_50;

#ifdef HI_KEYLED_SUPPORT
HI_UNF_KEYLED_TYPE_E g_KeyledType = 1;
#endif

HI_HANDLE g_hAvplay;
PMT_COMPACT_TBL              *g_pProgTbl = HI_NULL;
HI_UNF_AVPLAY_STOP_OPT_S g_Stop;

#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif

#define IR_CHN_UP 0x35caff00
#define IR_CHN_DOWN 0x2dd2ff00
#define IR_VOL_UP 0X3ec1ff00
#define IR_VOL_DOWN 0x6699ff00

#define KEY_CHN_UP 0x6
#define KEY_CHN_DOWN 0x1
#define KEY_VOL_UP 0x2
#define KEY_VOL_DOWN 0x4

#define THREAD_DELAY_TIME 50 * 1000 //50ms
#define VOL_MAX 100

HI_U8 DigDisCode_std[10] = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};

HI_U8 DigDisCode[10] = {0};

#ifdef HI_KEYLED_SUPPORT
void * LED_DisplayTask(void *args)
{
    HI_U32 u32Loop = 0;
    HI_S32 s32Ret;

    while (s_s32TaskRunning)
    {
        s32Ret = HI_UNF_LED_Display(DigDisCode[u32Loop + s_s32ProgNum + 1] << 24 | DigDisCode[u32Loop] << 16
                                    | DigDisCode[u32Loop] << 8 | DigDisCode[u32Loop]);

        if (HI_SUCCESS != s32Ret)
        {
            sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, s32Ret);
            break;
        }

        usleep(THREAD_DELAY_TIME);
    }

    return 0;
}
#endif

static HI_S32 DVB_ProgUpDown(HI_S32 flag)
{
    if (flag)         // program up
    {
        ++s_s32ProgNum;
        if (s_s32ChnNum == s_s32ProgNum)
        {
            s_s32ProgNum = 0;
        }
    }
    else             // program down
    {
        s_s32ProgNum--;
        if (-1 == s_s32ProgNum)
        {
            s_s32ProgNum = s_s32ChnNum - 1;
        }
    }

    return 0;
}

static HI_S32 DVB_VolSet(HI_S32 flag)
{
    HI_UNF_SND_GAIN_ATTR_S stGain;
    stGain.bLinearMode = HI_TRUE;

    if (flag)           // add up vol
    {
        s_s32Vol += 5;
        if (s_s32Vol > VOL_MAX)
        {
            s_s32Vol = VOL_MAX;
        }
    }
    else               //Vol down
    {
        s_s32Vol -= 5;
        if (s_s32Vol < 0)
        {
            s_s32Vol = 0;
        }
    }
    stGain.s32Gain = s_s32Vol;
    return HI_UNF_SND_SetVolume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL,&stGain);
}

#ifdef HI_KEYLED_SUPPORT
static void * KEY_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_S32 s32LoopFlag;
    HI_U32 u32PressStatus, u32KeyId;

    while (s_s32TaskRunning)
    {
        /*get KEY press value & press status*/
        s32Ret = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyId);
        if ((HI_SUCCESS != s32Ret) || (u32PressStatus == HI_UNF_KEY_STATUS_UP))
        {
            continue;
        }

        s32LoopFlag = 0;
        switch (u32KeyId)
        {
        case KEY_CHN_UP:
            DVB_ProgUpDown(1);
            s32LoopFlag = 1;
            break;
        case KEY_CHN_DOWN:
            DVB_ProgUpDown(0);
            s32LoopFlag = 1;
            break;
        case KEY_VOL_UP:
            DVB_VolSet(1);
            break;
        case KEY_VOL_DOWN:
            DVB_VolSet(0);
            break;
        default:
            break;
        }

        if (1 == s32LoopFlag)
        {
            HIADP_AVPlay_PlayProg(g_hAvplay, g_pProgTbl, s_s32ProgNum, HI_TRUE);
        }

        usleep(THREAD_DELAY_TIME);
    }
    return 0;
}
#endif

static void * IR_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_S32 s32LoopFlag = 0;
    HI_U64 u64KeyId;
    HI_UNF_KEY_STATUS_E PressStatus;

    while (s_s32TaskRunning)
    {
        /* get ir press codevalue & press status*/
        s32Ret = HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 0);
        if ((HI_SUCCESS != s32Ret) || (PressStatus == HI_UNF_KEY_STATUS_UP))
        {
            continue;
        }

        s32LoopFlag = 0;
        if (HI_SUCCESS == s32Ret)
        {
            switch (u64KeyId)
            {
            case IR_CHN_UP:
                DVB_ProgUpDown(1);
                s32LoopFlag = 1;
                break;

            case IR_CHN_DOWN:
                DVB_ProgUpDown(0);
                s32LoopFlag = 1;
                break;

            case IR_VOL_UP:
                DVB_VolSet(1);
                break;

            case IR_VOL_DOWN:
                DVB_VolSet(0);
                break;

            default:
                break;
            }

            if (1 == s32LoopFlag)
            {
                HIADP_AVPlay_PlayProg(g_hAvplay, g_pProgTbl, s_s32ProgNum, HI_TRUE);
            }

            s32Ret = HI_SUCCESS;
            usleep(THREAD_DELAY_TIME);
        }
    }
    return 0;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret;
    HI_HANDLE hWin;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
    HI_UNF_SYNC_ATTR_S SyncAttr;
    HI_CHAR InputCmd[32];

    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_U32                      Tuner;
    pthread_t IrThread;

#ifdef HI_KEYLED_SUPPORT
    pthread_t keyTaskid;
    pthread_t ledTaskid;
#endif

    if (7 == argc)
    {
        g_TunerFreq  = strtol(argv[2], NULL, 0);
        g_TunerSrate = strtol(argv[3], NULL, 0);
        g_ThirdParam = strtol(argv[4], NULL, 0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        Tuner = strtol(argv[6],NULL,0);
    }
    else if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[2], NULL, 0);
        g_TunerSrate = strtol(argv[3], NULL, 0);
        g_ThirdParam = strtol(argv[4], NULL, 0);
        g_enDefaultFmt = HIADP_Disp_StrToFmt(argv[5]);
        Tuner = 0;
    }
    else if (5 == argc)
    {
        g_TunerFreq  = strtol(argv[2], NULL, 0);
        g_TunerSrate = strtol(argv[3], NULL, 0);
        g_ThirdParam = strtol(argv[4], NULL, 0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if (4 == argc)
    {
        g_TunerFreq  = strtol(argv[2], NULL, 0);
        g_TunerSrate = strtol(argv[3], NULL, 0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else if (3 == argc)
    {
        g_TunerFreq  = strtol(argv[2], NULL, 0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
        Tuner = 0;
    }
    else
    {
        printf("Usage: %s [keyled_type] [freq] [srate] [qamtype or polarization] [vo_format] [tuner]\n"
                "       keyled_type: 0: STD, 1: PT6961, 2: CT1642, 3: PT6964, 4: FD650\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50  default HI_UNF_ENC_FMT_1080i_50\n"
                "       Tuner: value can be 0, 1, 2, 3\n",
                argv[0]);
        printf("Example:%s 1 610 6875 64 1080i_50 0\n", argv[0]);
        return HI_FAILURE;
    }

#ifdef HI_KEYLED_SUPPORT
    g_KeyledType  = strtol(argv[1], NULL, 0);
#endif

    Ret = HI_SYS_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return Ret;
    }

    HIADP_MCE_Exit();

    Ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto SYS_DEINIT;
    }

    Ret = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto IR_CLOSE;
    }

    Ret = HI_UNF_IR_SetRepKeyTimeoutAttr(300);   //???  108< parament   <65536
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto IR_CLOSE;
    }

    Ret = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto IR_CLOSE;
    }

    Ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto IR_CLOSE;
    }

#ifdef HI_KEYLED_SUPPORT
    Ret = HI_UNF_KEYLED_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto IR_CLOSE;
    }

    Ret = HI_UNF_KEYLED_SelectType(g_KeyledType);  /*standard hisi keyboard*//*CNcomment:标准海思键盘 0*/
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEYLED_DEINIT;
    }

    /*open LED device*/
    Ret = HI_UNF_LED_Open();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEYLED_DEINIT;
    }

    /*enable flash*/
    Ret = HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_1);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto LED_CLOSE;
    }

    /*config LED flash or not*/
    Ret = HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_NONE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto LED_CLOSE;
    }

    /*open KEY device*/
    Ret = HI_UNF_KEY_Open();   /*block mode*/
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto LED_CLOSE;
    }

    /*config keyup is valid*/
    Ret = HI_UNF_KEY_IsKeyUp(1);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEY_CLOSE;
    }

    /*config keyhold is valid*/
    Ret = HI_UNF_KEY_IsRepKey(1);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEY_CLOSE;
    }
#endif
    /******************************************************************************
                                 HI_AVPALY_NINT

    ******************************************************************************/

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto KEY_CLOSE;
    }

    /*HI_SYS_GetPlayTime(&u32Playtime);
    sample_printf("u32Playtime = %d\n", u32Playtime);*/
    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call  HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    Ret = HIADP_VO_CreatWin(HI_NULL, &hWin);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_CreatWin failed.\n");
        goto  VO_DEINIT;
    }

    Ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_DMX_Init failed.\n");
        goto  VO_DESTROYWIN;
    }

    Ret = HIADP_DMX_AttachTSPort(0, Tuner);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_DMX_AttachTSPort failed.\n");
        goto DMX_DEINIT;
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DETACH;
    }

    Ret = HIADP_Tuner_Connect(Tuner, g_TunerFreq, g_TunerSrate, g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(0, &g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Search_GetAllPmt failed\n");
        goto HIADP_Search_DeInit;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call  HIADP_AVPlay_RegADecLib failed.\n");
        goto  PSISI_FREE;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto  PSISI_FREE;
    }

    Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
    Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }

    Ret  = HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    Ret |= HI_UNF_AVPLAY_ChnOpen(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto AVPLAY_DESTROY;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, g_hAvplay);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    Ret = HI_UNF_SND_Attach(hTrack, g_hAvplay);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.bQuickOutput = HI_FALSE;
    Ret = HI_UNF_AVPLAY_SetAttr(g_hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    memcpy(DigDisCode, DigDisCode_std, sizeof(DigDisCode_std));

    Ret = HIADP_AVPlay_PlayProg(g_hAvplay, g_pProgTbl, s_s32ProgNum, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    s_s32ChnNum = g_pProgTbl->prog_num;

    /******************************************************************************
                                  create three threads.

    ******************************************************************************/

    Ret = pthread_create(&IrThread, NULL, IR_ReceiveTask, NULL);
    if (0 != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
        goto AVPLAY_STOP;
    }
#ifdef HI_KEYLED_SUPPORT
    Ret = pthread_create(&keyTaskid, NULL, KEY_ReceiveTask, NULL);
    if (0 != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
        goto ERR1;
    }

    Ret = pthread_create(&ledTaskid, NULL, LED_DisplayTask, NULL);
    if (0 != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
        goto ERR2;
    }
#endif
    while (1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
    }

    /*exit the three threads that ledTaskid and keyTaskid IrThread */
    s_s32TaskRunning = 0;
#ifdef HI_KEYLED_SUPPORT
    pthread_join(ledTaskid, 0);
ERR2:
    pthread_join(keyTaskid, 0);
ERR1:
#endif
    pthread_join(IrThread, 0);

AVPLAY_STOP:
    g_Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    g_Stop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &g_Stop);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, g_hAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    HI_UNF_VO_DetachWindow(hWin, g_hAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(g_hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

AVPLAY_DESTROY:
    HI_UNF_AVPLAY_Destroy(g_hAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);

HIADP_Search_DeInit:
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DETACH:
    HI_UNF_DMX_DetachTSPort(0);

DMX_DEINIT:
    HI_UNF_DMX_DeInit();

VO_DESTROYWIN:
    HI_UNF_VO_DestroyWindow(hWin);

VO_DEINIT:
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

KEY_CLOSE:
#ifdef HI_KEYLED_SUPPORT

    HI_UNF_KEY_Close();

LED_CLOSE:
    HI_UNF_LED_Close();

KEYLED_DEINIT:
    HI_UNF_KEYLED_DeInit();
#endif
IR_CLOSE:
    HI_UNF_IR_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}
