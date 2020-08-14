/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : fbc_mosaic.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/12/23
  Description   : demo for full band capture on board hi3716cdmo2c
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
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "hi_adp_mpi.h"
#include "hi_adp_search.h"
#include "hi_adp_tuner.h"

#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif

#ifndef HI_TUNER_NUMBER
#define HI_TUNER_NUMBER   4
#endif

#define AVPLAY_NUM  HI_TUNER_NUMBER
#define CONNECT_TIMEOUT  2000

static HI_U32  g_DmxID[HI_TUNER_NUMBER] = {0};
static HI_U32  g_TunerID[HI_TUNER_NUMBER] = {0};
static HI_U32  g_u32TunerSrate[HI_TUNER_NUMBER] = {0};
static HI_U32  g_u32TunerQamType[HI_TUNER_NUMBER] = {0};
static HI_U32  g_TunerFreq[HI_TUNER_NUMBER] = {0};
static HI_U32  g_startTime[HI_TUNER_NUMBER] = {0};
static HI_U32  g_curTime[HI_TUNER_NUMBER] = {0};

static HI_UNF_DMX_PORT_E  g_enDmxPort[HI_TUNER_NUMBER] = {0};
static HI_UNF_DMX_PORT_ATTR_S g_stDmxPortAttr[HI_TUNER_NUMBER];

static pthread_t   g_TunerThd[HI_TUNER_NUMBER];
static PMT_COMPACT_TBL *g_pProgTbl[HI_TUNER_NUMBER];
static HI_UNF_TUNER_STATUS_S g_stTunerStatus[HI_TUNER_NUMBER];
static HI_BOOL g_bProgam = HI_FALSE;
static HI_HANDLE      g_hAvplay[AVPLAY_NUM];
static HI_BOOL        g_bAudPlay[AVPLAY_NUM];


HI_VOID TunerTthread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_U32 tid;

    tid = *(HI_U32*)args;
    if(tid >= HI_TUNER_NUMBER)
    {
        sample_printf(" TunerId[%d] in bigger than HI_TUNER_NUMBER:%d\n",tid,HI_TUNER_NUMBER);
        return;
    }
    
    g_startTime[tid] = 0;
    g_curTime[tid] = 0;
    
    sample_printf("[Line:%d]connect in thread[%d],freq:%d, sr:%d, qam:%d \n",__LINE__,tid,g_TunerFreq[tid],g_u32TunerSrate[tid],g_u32TunerQamType[tid]);
    HI_SYS_GetTimeStampMs(&g_startTime[tid]);
    Ret = HIADP_Tuner_Connect(g_TunerID[tid],g_TunerFreq[tid],g_u32TunerSrate[tid],g_u32TunerQamType[tid]);
    if ( HI_SUCCESS != Ret )
    {
        sample_printf("call HIADP_Tuner_Connect(in thread %d) failed:0x%x \n",tid,Ret);
        return;
    }
    
    do{
        Ret = HI_UNF_TUNER_GetStatus(g_TunerID[tid], &g_stTunerStatus[tid]);
        if(HI_UNF_TUNER_SIGNAL_LOCKED == g_stTunerStatus[tid].enLockStatus)
        {
            HI_SYS_GetTimeStampMs(&g_curTime[tid]);           
            Ret = HIADP_Search_GetAllPmt(g_DmxID[tid],&g_pProgTbl[tid]);
            if ( HI_SUCCESS != Ret )
            {
                sample_printf("call HIADP_Search_GetAllPmt(in thread %d) failed:0x%x \n",tid,Ret);
                return;
            }
            else
            {
                g_bProgam = HI_TRUE;
                if(g_pProgTbl[tid])
                {
                    sample_printf("(in thread %d)ProgNum:%d\n",tid,g_pProgTbl[tid]->prog_num);
                    Ret = HIADP_AVPlay_PlayProg(g_hAvplay[tid], g_pProgTbl[tid], tid+1, g_bAudPlay[tid]);
                    if (HI_SUCCESS != Ret)
                    {
                        sample_printf("call HIADP_AVPlay_SwitchProg(in thread %d) failed\n",tid);
                    }
                }
            }
            break;
        }
        else
        {
            usleep(10 * 1000);
            HI_SYS_GetTimeStampMs(&g_curTime[tid]);
        }        
    }while((0 != g_curTime[tid]) && (g_curTime[tid]-g_startTime[tid]) <= CONNECT_TIMEOUT);
   
    if(HI_UNF_TUNER_SIGNAL_LOCKED == g_stTunerStatus[tid].enLockStatus)
    {
        sample_printf("-->tuner[%d] lock success,timecost:%d ms.\n",tid,g_curTime[tid]-g_startTime[tid]);
    }
    else
    {
        sample_printf("-->tuner[%d] lock failed, timeout:%d ms.\n",tid,g_curTime[tid]-g_startTime[tid]);
    }
    return;
}
    
HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                   Ret,i;
    HI_HANDLE                hWin[AVPLAY_NUM];
    HI_UNF_AVPLAY_ATTR_S     AvplayAttr;
    HI_UNF_SYNC_ATTR_S       SyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;
    HI_CHAR                  InputCmd[32];
    HI_U32                   ProgNum = 0;
    HI_RECT_S                stWinRect;
    HI_UNF_AVPLAY_OPEN_OPT_S stVidOpenOpt;

    HI_HANDLE                hAvplayWithSnd = HI_INVALID_HANDLE;
    HI_HANDLE                hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    HI_U32                    program_p;
    HI_U32                    program_n;

    if ( argc < HI_TUNER_NUMBER + 1 )
    {
        sample_printf("usage:%s ",argv[0]);
        for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
        {
            sample_printf("freq%d ",i);
        }
        sample_printf("\n");
        return HI_FAILURE;
    }
    
    for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
    {
        g_DmxID[i] = i;
        g_TunerID[i] = i;
        g_TunerFreq[i]  = strtol(argv[i + 1],NULL,0);
        g_u32TunerSrate[i] = 6875;
        g_u32TunerQamType[i] = 64;  
        g_startTime[i] = 0;
        g_curTime[i] = 0;

        if(0 == i)
        {
            g_enDmxPort[i] = HI_DEMUX_PORT;
        }
        else if(1 == i)
        {
            g_enDmxPort[i] = HI_DEMUX1_PORT;
        }
        else if(2 == i)
        {
            g_enDmxPort[i] = HI_DEMUX2_PORT;
        }
        else if(3 == i)
        {
            g_enDmxPort[i] = HI_DEMUX3_PORT;
        }
    }
    
    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }

    Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_720P_50);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_MOSAIC);
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_VO_Init failed.\n");
        goto DISP_DEINIT;
    }

    for (i=0; i<AVPLAY_NUM; i++)
    {
        stWinRect.s32Width = 1280 /(int)((AVPLAY_NUM+1)/2);
        stWinRect.s32Height = 720/ 2;
        stWinRect.s32X = i % (int)((AVPLAY_NUM+1)/2) * stWinRect.s32Width;
        stWinRect.s32Y = i / (int)((AVPLAY_NUM+1)/2) * stWinRect.s32Height;

        Ret = HIADP_VO_CreatWin(&stWinRect,&hWin[i]);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call WinInit failed.\n");
            goto VO_DEINIT;
        }
        sample_printf("---->hwin[%d]:(%d,%d),%dx%d.\n",i,stWinRect.s32X,stWinRect.s32Y,stWinRect.s32Width,stWinRect.s32Height);
    }

    Ret = HI_UNF_DMX_Init();
    Ret |= HIADP_Tuner_Init();
    HIADP_Search_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DEINIT;
    }

    for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
    {
        Ret = HI_UNF_DMX_GetTSPortAttr(g_enDmxPort[i],&g_stDmxPortAttr[i]);
        switch (g_enDmxPort[i])
        {
        case HI_UNF_DMX_PORT_IF_0:
        default:
            g_stDmxPortAttr[i].enPortType = HI_IF0_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_IF0_BIT_SELECTOR;
            break;
        case HI_UNF_DMX_PORT_TSI_0:
            g_stDmxPortAttr[i].enPortType = HI_TSI0_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI0_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI0_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_1:
            g_stDmxPortAttr[i].enPortType = HI_TSI1_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI1_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI1_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_2:
            g_stDmxPortAttr[i].enPortType = HI_TSI2_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI2_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI2_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_3:
            g_stDmxPortAttr[i].enPortType = HI_TSI3_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI3_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI3_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_4:
            g_stDmxPortAttr[i].enPortType = HI_TSI4_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI4_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI4_CLK_PHASE_REVERSE;
            break;
        case HI_UNF_DMX_PORT_TSI_5:
            g_stDmxPortAttr[i].enPortType = HI_TSI5_TYPE;
            g_stDmxPortAttr[i].u32SerialBitSelector = HI_TSI5_BIT_SELECTOR;
            g_stDmxPortAttr[i].u32TunerInClk = HI_TSI5_CLK_PHASE_REVERSE;
            break;
        }
        Ret = HI_UNF_DMX_SetTSPortAttr(g_enDmxPort[i],&g_stDmxPortAttr[i]);
        Ret = HI_UNF_DMX_AttachTSPort(g_DmxID[i],g_enDmxPort[i]);

    }

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        sample_printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    for(i = 0;i < AVPLAY_NUM;i++)
    {
       sample_printf("======avplay[%d] demux[%d]-port:%d======\n",i,g_DmxID[i],g_enDmxPort[i]);
       
       HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
       AvplayAttr.u32DemuxId = g_DmxID[i];
       AvplayAttr.stStreamAttr.u32VidBufSize = 0x200000;
       Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &g_hAvplay[i]);
       if (Ret != HI_SUCCESS)
       {
           sample_printf("call HI_UNF_AVPLAY_Create failed.\n");
           goto AVPLAY_STOP;
       }

       stVidOpenOpt.enDecType = HI_UNF_VCODEC_DEC_TYPE_NORMAL;
       stVidOpenOpt.enCapLevel = HI_UNF_VCODEC_CAP_LEVEL_D1;
       stVidOpenOpt.enProtocolLevel = HI_UNF_VCODEC_PRTCL_LEVEL_H264;
       Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &stVidOpenOpt);
       if (HI_SUCCESS != Ret)
       {
           sample_printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
           goto AVPLAY_STOP;
       }

       Ret = HI_UNF_VO_AttachWindow(hWin[i], g_hAvplay[i]);
       if (HI_SUCCESS != Ret)
       {
           sample_printf("call HI_UNF_VO_AttachWindow failed.\n");
           goto AVPLAY_STOP;
       }
       Ret = HI_UNF_VO_SetWindowEnable(hWin[i], HI_TRUE);
       if (HI_SUCCESS != Ret)
       {
           sample_printf("call HI_UNF_VO_SetWindowEnable failed.\n");
           goto AVPLAY_STOP;
       }

       g_bAudPlay[i] = HI_FALSE;
       if (0 == i)
       {
           g_bAudPlay[i] = HI_TRUE;
           Ret = HI_UNF_AVPLAY_ChnOpen(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);

           Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
           if (Ret != HI_SUCCESS)
           {
               sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
               goto AVPLAY_STOP;
           }
           Ret |= HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
           Ret |= HI_UNF_SND_Attach(hTrack, g_hAvplay[i]);
           if (Ret != HI_SUCCESS)
           {
               sample_printf("call HI_UNF_SND_Attach failed.\n");
               goto AVPLAY_STOP;
           }
           hAvplayWithSnd = g_hAvplay[i];
       }

       Ret = HI_UNF_AVPLAY_GetAttr(g_hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
       SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
       Ret |= HI_UNF_AVPLAY_SetAttr(g_hAvplay[i], HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
       if (Ret != HI_SUCCESS)
       {
           sample_printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
           goto AVPLAY_STOP;
       }
    }


    for ( i = 0 ; i < HI_TUNER_NUMBER ; i++ )
    {
       pthread_create(&g_TunerThd[i], HI_NULL, (HI_VOID *)TunerTthread,&g_TunerID[i]);
    }

        
    while(1)
    {
        printf("please input the q to quit!\n");
        SAMPLE_GET_INPUTCMD(InputCmd);

        if ('q' == InputCmd[0])
        {
            printf("prepare to quit!\n");
            break;
        }
#if 1
        ProgNum = atoi(InputCmd);
        program_p = ProgNum/10;
        if(program_p >= AVPLAY_NUM)
        {
            program_p = AVPLAY_NUM-1;
        }
        program_n = ProgNum%10;
        sample_printf("change:%d to program:%d\n",program_p,program_n);
        HI_UNF_AVPLAY_Stop(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD,HI_NULL);
        HI_UNF_SND_Detach(hTrack,hAvplayWithSnd);
        HI_UNF_SND_DestroyTrack(hTrack);
        HI_UNF_AVPLAY_ChnClose(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        hAvplayWithSnd = g_hAvplay[program_p];
        HI_UNF_AVPLAY_ChnOpen(hAvplayWithSnd, HI_UNF_AVPLAY_MEDIA_CHAN_AUD,HI_NULL);
        
        Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
        if (Ret != HI_SUCCESS)
        {
            sample_printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
            goto AVPLAY_STOP;
        }
        HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
        HI_UNF_SND_Attach(hTrack, hAvplayWithSnd); 
        if(g_pProgTbl[program_p])
        {
            sample_printf("---->havplay: 0x%x,progtbl:%p\n",hAvplayWithSnd,g_pProgTbl[program_p]);
            //HIADP_AVPlay_PlayAud(hAvplayWithSnd,g_pProgTbl[program_p],program_n);
            HIADP_AVPlay_PlayProg(hAvplayWithSnd,g_pProgTbl[program_p],program_n,HI_TRUE);
        }

#endif
    }

    for(i = 0 ; i < HI_TUNER_NUMBER;i++)
    {
         pthread_join(g_TunerThd[i], NULL);
    }
AVPLAY_STOP:
    for(i = 0 ; i < AVPLAY_NUM;i++)
    {
        Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
        Stop.u32TimeoutMs = 0;
        if ( hAvplayWithSnd == g_hAvplay[i] )
        {
            HI_UNF_AVPLAY_Stop(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
            HI_UNF_SND_Detach(hTrack, g_hAvplay[i]);
            HI_UNF_SND_DestroyTrack(hTrack);
        }
        else
        {
            HI_UNF_AVPLAY_Stop(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID, &Stop);
        }
        HI_UNF_VO_SetWindowEnable(hWin[i],HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin[i], g_hAvplay[i]);
        if ( hAvplayWithSnd == g_hAvplay[i] )
        {
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        }
        else
        {
            HI_UNF_AVPLAY_ChnClose(g_hAvplay[i], HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        }

        HI_UNF_AVPLAY_Destroy(g_hAvplay[i]);
    }
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    for ( i  = 0 ; i  < HI_TUNER_NUMBER ; i ++ )
    {
        if ( g_pProgTbl[i] )
        {
            HIADP_Search_FreeAllPmt(g_pProgTbl[i]);
            HI_UNF_DMX_DetachTSPort(i);
        }
    }

DMX_DEINIT:
    HIADP_Search_DeInit();
    HIADP_Tuner_DeInit();
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    for ( i = 0 ; i < AVPLAY_NUM ; i++ )
    {
        HI_UNF_VO_DestroyWindow(hWin[i]);
    }
    HIADP_VO_DeInit();

DISP_DEINIT:
    HIADP_Disp_DeInit();

SND_DEINIT:
    HIADP_Snd_DeInit();

SYS_DEINIT:
    HI_SYS_DeInit();

    return Ret;
}

