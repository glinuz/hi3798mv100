/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : fbc_xswitch.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/12/23
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
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_adp.h"
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

//#define DEBUG sample_printf("%s %d\n",__FUNCTION__,__LINE__);
#define DEBUG


static HI_S32 s_s32TaskRunning = 1;

HI_UNF_ENC_FMT_E g_enDefaultFmt  = HI_UNF_ENC_FMT_1080i_50;

#ifdef HI_KEYLED_SUPPORT
static HI_UNF_KEYLED_TYPE_E g_KeyledType = HI_UNF_KEYLED_TYPE_CT1642;
#endif

#define IR_CHN_UP    0xf40bec40
#define IR_CHN_DOWN  0xf10eec40

#define IR_0 0xff00ec40
#define IR_1 0xfe01ec40
#define IR_2 0xfd02ec40
#define IR_3 0xfc03ec40
#define IR_4 0xfb04ec40
#define IR_5 0xfa05ec40
#define IR_6 0xf906ec40
#define IR_7 0xf807ec40
#define IR_8 0xf708ec40
#define IR_9 0xf609ec40
#define IR_SEARCH 0xb847ec40
#define IR_OK 0xf20dec40


#define KEY_CHN_UP   0x6
#define KEY_CHN_DOWN 0x1

#define THREAD_DELAY_TIME  (50 * 1000)

#ifndef HI_TUNER_NUMBER
#define HI_TUNER_NUMBER 4
#endif
#define MAX_FREQ_NUM   HI_TUNER_NUMBER
#define MAX_AVPLAY_NUM 3
#define MAX_PLAYPROG_NUM 3
#define MAX_PROG_NUM   50
#define TUNER_SRATE    6875
#define TUNER_MODTYPE  64

typedef struct
{
    HI_U32 TunerFreq;
    PMT_COMPACT_PROG *pProgInfo;
}PROG_INFO_S;

/*
Tuner----TunerID, LockFreq, DmxPort, PmtTable
Avplay--- DmxID, ProgIdx,
*/

static HI_U32       g_ProgNum;
static HI_U32       g_PlayProgIdx;
static PROG_INFO_S  g_ProgTable[MAX_PROG_NUM];
static HI_U32       g_FreqTable[MAX_FREQ_NUM] = {0};
static HI_U32       g_TunerID[HI_TUNER_NUMBER] = {0};
static HI_U32       g_LockedFreq[HI_TUNER_NUMBER] = {0};
static HI_UNF_DMX_PORT_E  g_DmxPort[4] = {HI_DEMUX_PORT, HI_DEMUX1_PORT, HI_DEMUX2_PORT, HI_DEMUX3_PORT};

static PMT_COMPACT_TBL *g_pPmtTable[MAX_FREQ_NUM];
static HI_HANDLE    g_hAvplay[MAX_AVPLAY_NUM];
static HI_U32       g_DmxID[MAX_AVPLAY_NUM];
static HI_HANDLE    g_hWindow = HI_INVALID_HANDLE;
static HI_HANDLE    g_hTrack = HI_INVALID_HANDLE;

static HI_U32       g_ProgIdx[MAX_PLAYPROG_NUM]; /*0-Cur 1-Last 2-Next 3-History*/
static HI_U32       g_AvplayIdx[MAX_PLAYPROG_NUM];

static HI_S32 DVB_Lock(HI_U32 Freq,HI_U32 *pTunerID)
{
    HI_S32 i,j;
    /*judge if the freq is locked*/
    for ( i = 0 ; i < HI_TUNER_NUMBER; i++ )
    {
        if ( g_LockedFreq[i] == Freq)
        {
            break;
        }
    }

    if ( i < HI_TUNER_NUMBER )
    {
        *pTunerID = i;
        return HI_SUCCESS;
    }

    /*lock*/
    for ( i = 0 ; i < HI_TUNER_NUMBER; i++ )
    {
        for ( j = 0 ; j < MAX_PLAYPROG_NUM; j++ )
        {
            HI_U32 ProgIdx = g_ProgIdx[j];
            if ( g_LockedFreq[i] == g_ProgTable[ProgIdx].TunerFreq)
            {
                break;
            }
        }

        if ( MAX_PLAYPROG_NUM == j )
        {
            *pTunerID = i;
            g_LockedFreq[i] = Freq;
            break;
        }
    }

    return HIADP_Tuner_Connect(*pTunerID,Freq,TUNER_SRATE,TUNER_MODTYPE);
}

static HI_S32 DVB_SearchProg()
{
    HI_S32 Ret,i,j;
    HI_U32 TunerID = g_TunerID[0];
    HI_U32 u32DmxId = 0;
    HI_UNF_DMX_PORT_E enPortId = g_DmxPort[0];
    HI_U32 ProgIdx = 0;

    Ret = HI_UNF_DMX_AttachTSPort(u32DmxId,enPortId);
    if ( HI_SUCCESS != Ret )
    {
        sample_printf("call HI_UNF_DMX_AttachTSPort failed:%#x.\n",Ret);
        return Ret;
    }

    for ( i = 0 ; i < MAX_FREQ_NUM; i++ )
    {
        sample_printf("===================Freq:%d======================\n",g_FreqTable[i]);
        Ret = HIADP_Tuner_Connect(TunerID, g_FreqTable[i], TUNER_SRATE, TUNER_MODTYPE);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HIADP_Tuner_Connect failed:%#x.\n",Ret);
            break;
        }

        Ret = HIADP_Search_GetAllPmt(u32DmxId, &g_pPmtTable[i]);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HIADP_Search_GetAllPmt failed\n");
            break;
        }

        g_ProgNum += g_pPmtTable[i]->prog_num;
        //printf("program number : total:%d, current:%d, fre:%d\n", g_ProgNum, g_pPmtTable[i]->prog_num, g_FreqTable[i]);
        j = 0;
        while(ProgIdx < g_ProgNum)
        {
            g_ProgTable[ProgIdx].TunerFreq = g_FreqTable[i];
            g_ProgTable[ProgIdx].pProgInfo = g_pPmtTable[i]->proginfo + j ;
            ProgIdx++;
            j++;
        }
    }

    return Ret;
}
//
HI_S32 DVB_StartProg(HI_HANDLE hAvplay,PMT_COMPACT_PROG *pProgInfo,HI_BOOL bPreStart)
{
    HI_U32                   VidPid,AudPid;
    HI_UNF_VCODEC_TYPE_E     enVidType;
    HI_U32                   u32AudType;
    HI_S32                   Ret;
    HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;

    if (pProgInfo->VElementNum > 0 )
    {
        VidPid = pProgInfo->VElementPid;
        enVidType = pProgInfo->VideoType;
    }
    else
    {
        VidPid = INVALID_TSPID;
        enVidType = HI_UNF_VCODEC_TYPE_BUTT;
    }

    if (pProgInfo->AElementNum > 0)
    {
        AudPid  = pProgInfo->AElementPid;
        u32AudType = pProgInfo->AudioType;
    }
    else
    {
        AudPid = INVALID_TSPID;
        u32AudType = 0xffffffff;
    }

    /*TODO: PcrPid*/
#if 0
    PcrPid = pProgInfo->PcrPid;
    if (INVALID_TSPID != PcrPid)
    {
        printf("%s %d\n",__FUNCTION__,__LINE__);
        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay,HI_UNF_AVPLAY_ATTR_ID_PCR_PID,&PcrPid);
        if (HI_SUCCESS != Ret)
        {
            printf("call HI_UNF_AVPLAY_SetAttr failed.\n");

            PcrPid = INVALID_TSPID;
            Ret = HI_UNF_AVPLAY_SetAttr(hAvplay,HI_UNF_AVPLAY_ATTR_ID_PCR_PID,&PcrPid);
            if (HI_SUCCESS != Ret)
            {
                printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
                return Ret;
            }
        }
    }
#endif

    Ret = HI_UNF_AVPLAY_GetStatusInfo(hAvplay,&stStatusInfo);

    if (VidPid != INVALID_TSPID)
    {
        Ret = HIADP_AVPlay_SetVdecAttr(hAvplay,enVidType,HI_UNF_VCODEC_MODE_NORMAL);
        if(HI_UNF_AVPLAY_STATUS_STOP == stStatusInfo.enRunStatus)
        {
                Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&VidPid);
        }

        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_AVPlay_SetVdecAttr failed %#x\n",Ret);
            //return Ret;
        }

        if ( HI_FALSE == bPreStart )
        {
            Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        }
        else
        {
            Ret = HI_UNF_AVPLAY_PreStart(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        }
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Start failed.\n");
            return Ret;
        }
    }

    if (AudPid != INVALID_TSPID)
    {
        Ret  = HIADP_AVPlay_SetAdecAttr(hAvplay, u32AudType, HD_DEC_MODE_RAWPCM, 1);
        if(HI_UNF_AVPLAY_STATUS_STOP == stStatusInfo.enRunStatus)
        {
            Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &AudPid);
        }
        if (HI_SUCCESS != Ret)
        {
            //printf("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            //return Ret;
        }

        if ( HI_FALSE == bPreStart )
        {
            Ret = HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        }
        else
        {
            Ret = HI_UNF_AVPLAY_PreStart(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        }
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_Start to start audio failed.\n");
            //return Ret;
        }
    }

    return HI_SUCCESS;
}

static  HI_S32 DVB_PlayProg(HI_U32 u32ProgIdx)
{
    HI_U32              u32CurIdx,u32LastIdx,u32NextIdx;
    HI_U32              TunerID,DmxID,ProgIdx,AvplayIdx,ProgFreq;
    HI_HANDLE           hTrack = g_hTrack;
    HI_HANDLE           hWin = g_hWindow;
    HI_HANDLE           hAvplay;
    HI_UNF_DMX_PORT_E   enDmxPort;
    PMT_COMPACT_PROG    *pProgInfo = HI_NULL;
    HI_U32              i,j;
    HI_U32  flag=0;

    /*0. check if the program is playing*/
    if ( u32ProgIdx == g_ProgIdx[0])
    {
        return HI_SUCCESS;
    }
    /*1. stop the current program*/
    if ( -1 != g_AvplayIdx[0])
    {
        hAvplay = g_hAvplay[g_AvplayIdx[0]];
        HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID |HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        HI_UNF_SND_Detach(hTrack, hAvplay);
        HI_UNF_VO_SetWindowEnable(hWin,HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        g_AvplayIdx[0] = -1;
    }

    /*2. Init g_ProgIdx && g_AvplayIdx */
    u32CurIdx =   u32ProgIdx % g_ProgNum; /*g_ProgNum must not be zero*/
    u32LastIdx = (u32CurIdx + g_ProgNum - 1) % g_ProgNum;
    u32NextIdx = (u32CurIdx + 1) % g_ProgNum;
    /*find a avplay to use*/
    for ( i = 1 ; i < MAX_PLAYPROG_NUM; i++ )
    {
        if ( u32CurIdx == g_ProgIdx[i])
        {
            g_AvplayIdx[0] = g_AvplayIdx[i];
            flag = 1;
        }
        else if((u32LastIdx!=g_ProgIdx[i])&&(u32NextIdx!=g_ProgIdx[i]))
        {

            hAvplay = g_hAvplay[g_AvplayIdx[i]];
            HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID |HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        }
    }

    if(!flag)
    {
        g_AvplayIdx[0] = 0;
    }
    g_ProgIdx[0] = u32CurIdx;
    g_ProgIdx[1] = u32LastIdx;
    g_ProgIdx[2] = u32NextIdx;

    i = 0;
    j = 1;
    while( i < MAX_AVPLAY_NUM )
    {
        if ( i != g_AvplayIdx[0])
        {
            g_AvplayIdx[j] = i;
            i++;
            j++;
        }
        else
        {
            i++;
        }
    }

    /*3. start play program g_ProgIdx[0]*/
    DmxID = g_DmxID[g_AvplayIdx[0]];
    hAvplay = g_hAvplay[g_AvplayIdx[0]];
    ProgFreq = g_ProgTable[u32CurIdx].TunerFreq;
    DVB_Lock(ProgFreq,&TunerID);
    enDmxPort = g_DmxPort[TunerID];
    HI_UNF_DMX_AttachTSPort(DmxID,enDmxPort);
    HI_UNF_VO_AttachWindow(hWin, hAvplay);
    HI_UNF_VO_SetWindowEnable(hWin,HI_TRUE);
    HI_UNF_SND_Attach(hTrack,hAvplay);
    DVB_StartProg(hAvplay, g_ProgTable[u32CurIdx].pProgInfo,HI_FALSE);

    /*3 start buffer program g_ProgIdx[1...MAX_PLAYPROG_NUM]*/
    for ( i  = 1 ; i  < MAX_PLAYPROG_NUM; i ++ )
    {
        ProgIdx = g_ProgIdx[i];
        AvplayIdx = g_AvplayIdx[i];
        hAvplay = g_hAvplay[AvplayIdx];
        DmxID = g_DmxID[AvplayIdx];
        pProgInfo = g_ProgTable[ProgIdx].pProgInfo;
            HI_UNF_AVPLAY_Stop(hAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_VID|HI_UNF_AVPLAY_MEDIA_CHAN_AUD,HI_NULL);
        DVB_Lock(g_ProgTable[ProgIdx].TunerFreq,&TunerID);
        enDmxPort = g_DmxPort[TunerID];
        HI_UNF_DMX_AttachTSPort(DmxID,enDmxPort);
        DVB_StartProg(hAvplay,pProgInfo,HI_TRUE);
    }

    return HI_SUCCESS;
}

static HI_S32 DVB_ProgUpDown(HI_S32 flag)
{
    if (flag)         // program up
    {
        ++g_PlayProgIdx;
        if (g_ProgNum == g_PlayProgIdx)
        {
            g_PlayProgIdx = 0;
        }
    }
    else             // program down
    {
        g_PlayProgIdx--;
        if (-1 == g_PlayProgIdx)
        {
            g_PlayProgIdx = g_ProgNum - 1;
        }
    }

    return 0;
}

#ifdef HI_KEYLED_SUPPORT
static HI_S32 KEY_Init()
{
    HI_S32 Ret;

    Ret = HI_UNF_KEYLED_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return HI_FAILURE;
    }

    Ret = HI_UNF_KEYLED_SelectType(g_KeyledType);  /*standard hisi keyboard*//*CNcomment:标准海思键盘 0*/
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEYLED_DEINIT;
    }

    /*open KEY device*/
    Ret = HI_UNF_KEY_Open();   /*block mode*/
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEYLED_DEINIT;
    }

    /*config keyup is valid*/
    Ret = HI_UNF_KEY_IsKeyUp(1);
    Ret |= HI_UNF_KEY_IsRepKey(1);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        goto KEY_CLOSE;
    }

    return HI_SUCCESS;

KEY_CLOSE:
    HI_UNF_KEY_Close();
KEYLED_DEINIT:
    HI_UNF_KEYLED_DeInit();

    return Ret;
}

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
        default:
            break;
        }

        if (1 == s32LoopFlag)
        {
        DVB_PlayProg(g_PlayProgIdx);
        }

        usleep(THREAD_DELAY_TIME);
    }
    return 0;
}
#endif

static HI_S32 IR_Init()
{
    HI_S32 Ret;
    Ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return HI_FAILURE;
    }

    Ret = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    Ret |= HI_UNF_IR_SetRepKeyTimeoutAttr(300);  //???  108< parament   <65536
    Ret |= HI_UNF_IR_EnableRepKey(HI_TRUE);
    Ret |= HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_DeInit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
static void * IR_ReceiveTask(void *args)
{
    HI_S32 s32Ret;
    HI_S32 s32LoopFlag = 0;
    HI_U64 u64KeyId;
    HI_S32 s32IrValue=-1;
    char str[10];
    int num = 0;
    int bOK = 0;
    HI_UNF_KEY_STATUS_E PressStatus;

    while (s_s32TaskRunning)
    {
        usleep(THREAD_DELAY_TIME);

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
            case IR_0:
        s32IrValue = 0;
        break;
            case IR_1:
        s32IrValue = 1;
        break;
            case IR_2:
        s32IrValue = 2;
        break;
            case IR_3:
        s32IrValue = 3;
        break;
            case IR_4:
        s32IrValue = 4;
        break;
            case IR_5:
        s32IrValue = 5;
        break;
            case IR_6:
        s32IrValue = 6;
        break;
            case IR_7:
        s32IrValue = 7;
        break;
            case IR_8:
        s32IrValue = 8;
        break;
            case IR_9:
        s32IrValue = 9;
        break;
            case IR_SEARCH:
        break;
            case IR_OK:
        bOK=1;
        break;
            default:
                break;
            }
            if (1 == s32LoopFlag)
            {
        DVB_PlayProg(g_PlayProgIdx);
        continue;
            }

        if(0==bOK)
        {
        if(s32IrValue<10)
        {
            if(0==num)
            {
                sprintf(str,"%d",s32IrValue);
            }
            else
                sprintf(str,"%s%d",str,s32IrValue);

            num++;
        }
        }
        else
        {
            int Ret;
            g_PlayProgIdx = atoi(str);
            printf("--------------select %d------------\n",g_PlayProgIdx);
            Ret = DVB_PlayProg(g_PlayProgIdx);
            if (Ret != HI_SUCCESS)
            {
                sample_printf("call DVB_PlayProg failed.\n");
            }
            num = 0;
            bOK = 0;
        }

        s32Ret = HI_SUCCESS;
    }
    }

    return 0;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 Ret,i;
//   HI_HANDLE hWin;
    HI_UNF_AVPLAY_ATTR_S AvplayAttr;
//   HI_UNF_SYNC_ATTR_S SyncAttr;
    HI_CHAR InputCmd[32];

//   HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;


    pthread_t IrThread;
#ifdef HI_KEYLED_SUPPORT
    pthread_t keyTaskid;
#endif

    if ( argc < MAX_FREQ_NUM + 1 )
    {
        sample_printf("usage:%s ",argv[0]);
        for ( i = 0 ; i < MAX_FREQ_NUM ; i++ )
        {
            sample_printf("freq%d ",i);
        }
        sample_printf("\n");
        return HI_FAILURE;
    }

    DEBUG
    for ( i = 0 ; i < MAX_FREQ_NUM ; i++ )
    {
        g_FreqTable[i] = strtol(argv[i + 1],NULL,0);
    }

    for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
    {
        g_TunerID[i] = i;
        g_LockedFreq[i] = 0;
    }

    for ( i  = 0 ; i  < MAX_PLAYPROG_NUM; i ++ )
    {
       g_ProgIdx[i] = -1;
       g_AvplayIdx[i] = -1;
    }

    DEBUG
    Ret = HI_SYS_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return Ret;
    }

    HIADP_MCE_Exit();

    IR_Init();
#ifdef HI_KEYLED_SUPPORT
    KEY_Init();
#endif

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto KEY_CLOSE;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_50);
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

    Ret = HIADP_VO_CreatWin(HI_NULL, &g_hWindow);
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

    for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
    {
        HI_UNF_DMX_PORT_ATTR_S stDmxPortAttr;
        Ret = HI_UNF_DMX_GetTSPortAttr(g_DmxPort[i],&stDmxPortAttr);
        switch (g_DmxPort[i])
        {
        case HI_UNF_DMX_PORT_IF_0:
        default:
            stDmxPortAttr.enPortType = HI_IF0_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_IF0_BIT_SELECTOR;
            break;
        case HI_UNF_DMX_PORT_TSI_0:
            stDmxPortAttr.enPortType = HI_TSI0_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI0_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI0_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_1:
            stDmxPortAttr.enPortType = HI_TSI1_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI1_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI1_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_2:
            stDmxPortAttr.enPortType = HI_TSI2_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI2_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI2_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_3:
            stDmxPortAttr.enPortType = HI_TSI3_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI3_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI3_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_4:
            stDmxPortAttr.enPortType = HI_TSI4_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI4_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI4_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_5:
            stDmxPortAttr.enPortType = HI_TSI5_TYPE;
            stDmxPortAttr.u32SerialBitSelector = HI_TSI5_BIT_SELECTOR;
            stDmxPortAttr.u32TunerInClk = HI_TSI5_CLK_PHASE_REVERSE;
            break;
        }
        Ret = HI_UNF_DMX_SetTSPortAttr(g_DmxPort[i],&stDmxPortAttr);
    }

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DETACH;
    }

    Ret = DVB_SearchProg();
    if ( HI_SUCCESS != Ret )
    {
        sample_printf("call DVB_SearchProg failed\n");
        goto  TUNER_DEINIT;
    }

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call  HIADP_AVPlay_RegADecLib failed.\n");
        goto  TUNER_DEINIT;
    }

    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto  TUNER_DEINIT;
    }

    DEBUG
    /*create avplay*/
    for ( i = 0 ; i < MAX_AVPLAY_NUM; i++ )
    {
        g_DmxID[i] = i;
        Ret  = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
        AvplayAttr.u32DemuxId = i;
        AvplayAttr.stStreamAttr.u32VidBufSize = 0x300000;
        Ret |= HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay[i]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
            goto AVPLAY_DEINIT;
        }

        Ret  = HI_UNF_AVPLAY_ChnOpen(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
        Ret |= HI_UNF_AVPLAY_ChnOpen(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
        if (HI_SUCCESS != Ret)
        {
            sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
            goto AVPLAY_DESTROY;
        }
    }
    DEBUG

    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&g_hTrack);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    DEBUG
    Ret = DVB_PlayProg(g_PlayProgIdx);
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call DVB_PlayProg failed.\n");
        goto AVPLAY_STOP;
    }
    DEBUG

    Ret = pthread_create(&IrThread, NULL, IR_ReceiveTask, NULL);
    if (0 != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
        goto AVPLAY_STOP;
    }
    DEBUG
#ifdef HI_KEYLED_SUPPORT
    Ret = pthread_create(&keyTaskid, NULL, KEY_ReceiveTask, NULL);
    if (0 != Ret)
    {
        sample_printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
        goto ERR1;
    }
#endif

    DEBUG
    while (1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }

        g_PlayProgIdx = atoi(InputCmd);
        Ret = DVB_PlayProg(g_PlayProgIdx);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call DVB_PlayProg!\n");
            break;
        }
    }

    /*exit the three threads that ledTaskid and keyTaskid IrThread */
    s_s32TaskRunning = 0;
#ifdef HI_KEYLED_SUPPORT
//ERR2:
    pthread_join(keyTaskid, 0);
ERR1:
#endif
    pthread_join(IrThread, 0);

AVPLAY_STOP:
    for ( i = 0 ; i < MAX_AVPLAY_NUM; i++ )
    {
        stStopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        stStopOpt.u32TimeoutMs = 0;
        HI_UNF_AVPLAY_Stop(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStopOpt);
    }

//SND_DETACH:
    HI_UNF_SND_Detach(g_hTrack, g_hAvplay[g_AvplayIdx[0]]);

//TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(g_hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(g_hWindow, HI_FALSE);
    HI_UNF_VO_DetachWindow(g_hWindow, g_hAvplay[g_AvplayIdx[0]]);

//CHN_CLOSE:
    for ( i = 0 ; i < MAX_AVPLAY_NUM; i++ )
    {
        HI_UNF_AVPLAY_ChnClose(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    }

AVPLAY_DESTROY:
    for ( i = 0 ; i < MAX_AVPLAY_NUM ; i++ )
    {
        HI_UNF_AVPLAY_Destroy(g_hAvplay[i]);
    }

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DETACH:
    HI_UNF_DMX_DetachTSPort(0);

//DMX_DEINIT:
    HI_UNF_DMX_DeInit();

VO_DESTROYWIN:
    HI_UNF_VO_DestroyWindow(g_hWindow);

VO_DEINIT:
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

KEY_CLOSE:
#ifdef HI_KEYLED_SUPPORT

    HI_UNF_KEY_Close();

//LED_CLOSE:
    HI_UNF_LED_Close();

//KEYLED_DEINIT:
    HI_UNF_KEYLED_DeInit();
#endif
//IR_CLOSE:
    HI_UNF_IR_DeInit();

//SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}
