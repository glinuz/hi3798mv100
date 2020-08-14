/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : sample_ca_pvr_rec.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "sample_ca_adp_pvr.h"
#include "sample_ca_common.h"
#include "hi_unf_advca.h"

//#include "hi_pvr_priv.h"
/* when recording, playing the live program */
#define PVR_WITH_LIVE

/*#ifndef PVR_WITH_LIVE
#define ENABLE_IR_PRG
#else
#undef ENABLE_IR_PRG
#endif*/

#ifdef ENABLE_IR_PRG
#include "hi_unf_ecs.h"
#endif
#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#define MAX_REC_FILE_SIZE       (10*1024*1024*1024LLU)
//#define MAX_REC_FILE_SIZE       (100*1024*1024LLU)
HI_BOOL bRewind = HI_TRUE;
HI_BOOL bDoCipher = HI_FALSE;
HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_TunerQam;
static HI_UNF_ENC_FMT_E   g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;

PMT_COMPACT_TBL   *g_pProgTbl = HI_NULL;

extern HI_BOOL g_bIsRecStop;
static pthread_t   g_CmdThd = 0;
HI_U32 g_uRec2Chn = 0xffffffff;
HI_HANDLE g_hTrack;


#ifdef ENABLE_IR_PRG

pthread_t g_IrThread = 0;
HI_BOOL g_IrTaskRunning = HI_FALSE;
HI_HANDLE g_hAvplay = 0;


HI_S32  IRProcess(HI_S32 nProgNum);
HI_S32 StartRecSecondProgram(HI_U32 uPrgNum);
#endif

HI_U32 u32cycle = 0;


HI_S32 DMXInitAndSearch(HI_S32 nRecDmxID, HI_S32 sPlayDmxID)
{
    HI_S32   Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }

    if(sPlayDmxID != -1)
    {
        Ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_DVB);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort failed.\n");
            (HI_VOID)HI_UNF_DMX_DeInit();
            return Ret;
        }
    }

    Ret = HI_UNF_DMX_AttachTSPort((HI_U32)nRecDmxID, PVR_DMX_PORT_ID_DVB);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        (HI_VOID)HI_UNF_DMX_DeInit();
        return Ret;
    }
#if 0
    Ret = HI_UNF_DMX_AttachTSPort(HI_UNF_DMX_ID_2_REC, PVR_DMX_PORT_ID_DVB);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort for REC failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }
#endif

    HIADP_Search_Init();
    if ( g_pProgTbl != HI_NULL)
    {
        (HI_VOID)HIADP_Search_FreeAllPmt(g_pProgTbl);
        g_pProgTbl = HI_NULL;
    }
    Ret = HIADP_Search_GetAllPmt((HI_U32)nRecDmxID,&g_pProgTbl);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    (HI_VOID)HIADP_Search_FreeAllPmt(g_pProgTbl);
    g_pProgTbl = HI_NULL;

    Ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    Ret |= HI_UNF_DMX_DetachTSPort(1);
    Ret |= HI_UNF_DMX_DetachTSPort(2);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_DetachTSPort failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_DeInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 AvplayInit(HI_HANDLE hWin, HI_HANDLE *phAvplay)
{
    HI_S32                  Ret;
    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;

    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;       //++
#ifdef __HI3716MV310__
	HI_UNF_MIXER_ATTR_S stMixerAttr;
#endif

    Ret = HIADP_AVPlay_RegADecLib();
    Ret |= HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Init failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_GetDefaultConfig failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    AvplayAttr.u32DemuxId = PVR_DMX_ID_LIVE;
    Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Create failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        return Ret;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_AttachWindow failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
        (HI_VOID)HI_UNF_VO_DetachWindow(hWin, hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

#ifndef __HI3716MV310__
    Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        return Ret;
    }
    //Ret = HI_UNF_SND_Mixer_Open(HI_UNF_SND_0, &hTrack, &stMixerAttr);
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr, &g_hTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_CreateTrack failed.\n");
        return Ret;
    }
#endif
#ifdef 	__HI3716MV310__
    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
#else
    Ret = HI_UNF_SND_Attach(g_hTrack, hAvplay);
#endif
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_SND_Attach failed.\n");
        (HI_VOID)HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
        (HI_VOID)HI_UNF_VO_DetachWindow(hWin, hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        (HI_VOID)HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        (HI_VOID)HI_UNF_AVPLAY_Destroy(hAvplay);
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    *phAvplay = hAvplay;
    return HI_SUCCESS;
}

HI_S32 AvplayDeInit(HI_HANDLE hAvplay, HI_HANDLE hWin)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    /*lint -save -e655  */
    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    Ret = HI_UNF_SND_Detach(g_hTrack, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_Detach failed.\n");
    }
    Ret = HI_UNF_SND_DestroyTrack(g_hTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_DestroyTrack failed.\n");
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
    }
    Ret = HI_UNF_VO_DetachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_DetachWindow failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }
    Ret = HI_UNF_AVPLAY_Destroy(hAvplay);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Destroy failed.\n");
    }

    Ret = HI_UNF_AVPLAY_DeInit();
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_DeInit failed.\n");
    }

    return HI_SUCCESS;
}

typedef struct
{
    HI_U32                  RecChn;
    HI_CHAR *pargv;
    PMT_COMPACT_PROG        *pstCurrentProgInfo;
}CX_ADVCA_PVR_S;




HI_VOID * CmdThread(HI_VOID *args)
{
    HI_CHAR                 InputCmd[32];
    CX_ADVCA_PVR_S*    pstCAPvr = (CX_ADVCA_PVR_S*)args;
    HI_U32                  RecChn;
    while (HI_FALSE == g_bIsRecStop)
    {
        printf("please input the 'q' to quit. \n");
        if (!fgets(InputCmd, 30, stdin))
        {
            sleep(1);
            continue;
        }

        if ('q' == InputCmd[0])
        {
            HI_DEBUG_ADVCA("now exit!\n");
            g_bIsRecStop = HI_TRUE;
        }
        if ('r' == InputCmd[0])
        {
            (HI_VOID)PVR_RecStop(pstCAPvr->RecChn);
            (HI_VOID)PVR_RecStart( pstCAPvr->pargv, pstCAPvr->pstCurrentProgInfo,1, bRewind, HI_FALSE, MAX_REC_FILE_SIZE, &RecChn,u32cycle);
            u32cycle++;
            pstCAPvr->RecChn = RecChn;
            //g_bIsRecStop = HI_TRUE;
        }
        if ('f' == InputCmd[0])/*fingerprint test*/
        {

            (HI_VOID)CreateTestFP();
        }
        if ('m' == InputCmd[0])/*Maturity Rating (URI) test*/
        {
            (HI_VOID)CreateTestURI();
        }
        else
        {
            continue;
        }
    }

    return NULL;
}




HI_S32 main(HI_S32 argc, HI_CHAR* const argv[])
{
    HI_S32                  Ret;
    CX_ADVCA_PVR_S stCAPvr = {0};
    HI_HANDLE               hWin;
    HI_CHAR                 InputCmd[32];
    HI_U32                  u32ProgNum;
    HI_U32                  RecChn;
//    HI_U32                  uRec2Chn = 0;
    PMT_COMPACT_PROG        *pstCurrentProgInfo;
    HI_U32 i = 0;
    HI_UNF_PVR_REC_STATUS_S stRecStatus;

    HI_HANDLE               hAvplay;



    if (6 == argc)
    {
        g_TunerFreq  = (HI_U32)strtol(argv[2],NULL,0);
        g_TunerSrate = (HI_U32)strtol(argv[3],NULL,0);
        g_TunerQam   = (HI_U32)strtol(argv[4],NULL,0);
        g_enDefaultFmt = (HI_UNF_ENC_FMT_E)HIADP_Disp_StrToFmt(argv[5]);
    }
    else if (5 == argc)
    {
        g_TunerFreq  = (HI_U32)strtol(argv[2],NULL,0);
        g_TunerSrate = (HI_U32)strtol(argv[3],NULL,0);
        g_TunerQam   = (HI_U32)strtol(argv[4],NULL,0);
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if(4 == argc)
    {
        g_TunerFreq  = (HI_U32)strtol(argv[2],NULL,0);
        g_TunerSrate = (HI_U32)strtol(argv[3],NULL,0);
        g_TunerQam   = 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else if(3 == argc)
    {
        g_TunerFreq  = (HI_U32)strtol(argv[2],NULL,0);
        g_TunerSrate = 6875;
        g_TunerQam   = 64;
        g_enDefaultFmt = HI_UNF_ENC_FMT_1080i_50;
    }
    else
    {
        printf("Usage: %s file_path freq [srate] [qamtype] [vo_format]\n"
               "       qamtype:16|32|[64]|128|256|512\n"
               "       vo_format:2160P_30|2160P_24|1080P_60|1080P_50|1080i_60|[1080i_50]|720P_60|720P_50\n", argv[0]);
        return HI_FAILURE;
    }

    (HI_VOID)HI_SYS_Init();

     Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }
    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Demux_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Tuner_Connect(TUNER_USE,g_TunerFreq,g_TunerSrate,g_TunerQam);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Tuner_Connect failed.\n");
        return Ret;
    }

    Ret = DMXInitAndSearch(1, PVR_DMX_ID_LIVE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }


    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Snd_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Disp_Init(g_enDefaultFmt);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_Disp_DeInit failed.\n");
        return Ret;
    }

    Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    Ret |= HIADP_VO_CreatWin(HI_NULL,&hWin);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HIADP_VO_Init failed.\n");
        (HI_VOID)HIADP_VO_DeInit();
        return Ret;
    }
#ifdef PVR_WITH_LIVE
    Ret = AvplayInit(hWin, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return Ret;
    }

    (HI_VOID)HI_UNF_PVR_PlayInit();
#endif
#ifdef ENABLE_IR_PRG
    g_hAvplay = hAvplay;
#endif

    Ret = HI_UNF_PVR_RecInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_PVR_RecInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    printf("\nPlease input the number of program to record:: ");

    u32ProgNum = 0;
    while (0 == u32ProgNum)
    {
        memset(InputCmd, 0, sizeof(32));
        if (!fgets(InputCmd, 30, stdin))
        {
            sleep(1);
            continue;
        }

        if ('q' == InputCmd[0])
        {
            return 0;
        }


        //HI_DEBUG_ADVCA("get:%s\n", InputCmd);
        u32ProgNum = (HI_U32)atoi(InputCmd);
    }

    /*lint -save -e774  */
    if (u32ProgNum == 0)
    {
        u32ProgNum = 1;
    }


    HI_DEBUG_ADVCA(" To record program %d.\n", u32ProgNum);

    pstCurrentProgInfo = g_pProgTbl->proginfo + ((u32ProgNum-1) % g_pProgTbl->prog_num);

    (HI_VOID)PVR_RecStart(argv[1], pstCurrentProgInfo,1, bRewind, HI_FALSE, MAX_REC_FILE_SIZE, &RecChn,u32cycle);
    //HI_UNF_ADVCA_PVR_RecOpen(RecChn);
    u32cycle++;
    stCAPvr.pargv = argv[1];
    stCAPvr.pstCurrentProgInfo = pstCurrentProgInfo;
    stCAPvr.RecChn = RecChn;

#ifdef ENABLE_IR_PRG

    u32ProgNum += 1;
    u32ProgNum %= g_pProgTbl->prog_num;

    u32ProgNum |= (RecChn<<16);

    IRProcess(u32ProgNum);

    u32ProgNum -= 1;
#endif

#if 0
    u32ProgNum+=1;

    //DMXInitAndSearch(HI_UNF_DMX_ID_2_REC, -1);
    pstCurrentProgInfo = g_pProgTblRec2->proginfo + ((u32ProgNum-1) % g_pProgTblRec2->prog_num);
    PVR_RecStart(argv[1], pstCurrentProgInfo,HI_UNF_DMX_ID_2_REC, bRewind, HI_TRUE, MAX_REC_FILE_SIZE, &g_uRec2Chn);
#endif

#ifdef PVR_WITH_LIVE

	Ret = HIADP_AVPlay_PlayProg(hAvplay,g_pProgTbl,u32ProgNum-1,HI_TRUE);
	if (Ret != HI_SUCCESS)
	{
		HI_DEBUG_ADVCA("call SwitchProgfailed!\n");
		return 0;
	}
#endif

    //pthread_create(&g_CmdThd, HI_NULL, CmdThread, HI_NULL);stCAPvr
    (HI_VOID)pthread_create(&g_CmdThd, HI_NULL, CmdThread,(HI_VOID *)&stCAPvr);
    while (HI_FALSE == g_bIsRecStop)
    {
        (HI_VOID)sleep(1);
        if (0 == i%5)
        {
            Ret = HI_UNF_PVR_RecGetStatus(RecChn, &stRecStatus);
            if (Ret != HI_SUCCESS)
            {
                HI_DEBUG_ADVCA("call HI_UNF_PVR_RecGetStatus failed.\n");
                continue;
            }

            HI_DEBUG_ADVCA("== record status: \n");
            if (stRecStatus.u64CurWritePos < 1024*1024)
            {
                HI_DEBUG_ADVCA("      size:%lld K\n", stRecStatus.u64CurWritePos/1024);
            }
            else
            {
                HI_DEBUG_ADVCA("      size:%lld M\n", stRecStatus.u64CurWritePos / (1024 * 1024));
            }
            HI_DEBUG_ADVCA("      time:from %d to %d, length:%d s\n", stRecStatus.u32StartTimeInMs/1000, stRecStatus.u32EndTimeInMs/1000, stRecStatus.u32CurTimeInMs/1000);
            HI_DEBUG_ADVCA("      record buff:%d/%d \n", stRecStatus.stRecBufStatus.u32UsedSize, stRecStatus.stRecBufStatus.u32BufSize);
        }
        i++;
    }

    (HI_VOID)pthread_join(g_CmdThd, HI_NULL);


    (HI_VOID)PVR_RecStop(RecChn);

#ifdef ENABLE_IR_PRG
    if(g_uRec2Chn != 0xffffffff)
    {
        (HI_VOID)PVR_RecStop(g_uRec2Chn);
    }

    g_IrTaskRunning = HI_FALSE;

    (HI_VOID)pthread_join(g_IrThread, HI_NULL);

    (HI_VOID)HI_UNF_IR_Close();
#endif
    (HI_VOID)HI_UNF_PVR_RecDeInit();

#ifdef PVR_WITH_LIVE
    (HI_VOID)HI_UNF_PVR_PlayDeInit();
    (HI_VOID)AvplayDeInit(hAvplay, hWin);
#endif

    Ret = DmxDeInit();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call DmxDeInit failed.\n");
        return Ret;
    }

    (HI_VOID)HI_UNF_VO_DestroyWindow(hWin);
    (HI_VOID)HIADP_VO_DeInit();
    (HI_VOID)HIADP_Disp_DeInit();
    (HI_VOID)HIADP_Snd_DeInit();
    (HI_VOID)HI_UNF_ADVCA_DeInit();
    (HI_VOID)HI_SYS_DeInit();

    return 0;
}


#ifdef ENABLE_IR_PRG
HI_S32 StartRecSecondProgram(HI_U32 uPrgNum)
{
    HI_S32              nRet = 0;
    PMT_COMPACT_PROG    *pstCurrentProgInfo = NULL;

    HI_DEBUG_ADVCA(" To record program %d.\n", uPrgNum);

    nRet = DMXInitAndSearch(HI_UNF_DMX_ID_2_REC, -1);
    if (nRet != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return nRet;
    }

    pstCurrentProgInfo = g_pProgTbl->proginfo + ((uPrgNum-1) % g_pProgTbl->prog_num);
    PVR_RecStart("./", pstCurrentProgInfo,HI_UNF_DMX_ID_2_REC, bRewind, HI_FALSE, MAX_REC_FILE_SIZE, &g_uRec2Chn);

    return 0;
}

const HI_CHAR IR_Stuts_Str[3][10] =
{
    "DOWN",
    "HOLD",
    "UP",
};
typedef enum tagKEY{
    KEY_FF = 0x29d6ff00,
    KEY_RW = 0x25daff00,
    KEY_STOP = 0x2fd0ff00,
    KEY_PLAY = 0x3cc3ff00,
    KEY_F1 = 0x7b84ff00,
    KEY_F2 = 0x8689ff00,
    KEY_F3 = 0x26d9ff00,
    KEY_F4 = 0x6996ff00,
    KEY_SEEK = 0x7d82ff00
}KEY_PAD;
void * IR_ReceiveTask(void *args)
{
    HI_S32 Ret = 0;
    HI_U64 u64KeyId = 0;
    HI_UNF_KEY_STATUS_E PressStatus = HI_UNF_KEY_STATUS_DOWN;
	HI_UNF_IR_PROTOCOL_E Protocol = HI_UNF_IR_NEC;
    HI_S32 nProgNum = (HI_S32)args&0xffff;
    HI_U32 nRecChn = ((HI_S32)args>>16)&0xffff;
    HI_U32 PlayChn = 0;

    HI_U32 uRate = 0;
    HI_U32 uPauseFlag = 0xffffffff;
    HI_U32 uSeekFlag = 0xffffffff;
    HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo;

    HI_DEBUG_ADVCA("current playing program is %d\n", nProgNum);


    while (g_IrTaskRunning)
    {
        /*get ir press codevalue & press status*/
        Ret = HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 200);
        if (HI_SUCCESS == Ret)
        {
        	Ret = HI_UNF_IR_GetProtocol(&Protocol);
            if (PressStatus == 0)
            {
                switch(u64KeyId)
                {
                    case KEY_PLAY:
                    {
                        uRate = 0;

                        if(uPauseFlag == 0xffffffff)
                        {
                            HI_DEBUG_ADVCA("PVR pause now.\n");
                            Ret = HI_UNF_PVR_PlayPauseChn(PlayChn);
                            if (Ret != HI_SUCCESS)
                            {
                                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayPauseChn failed.\n");
                            }
                            uPauseFlag = 0;
                        }
                        else
                        {
                            HI_DEBUG_ADVCA("PVR resume now.\n");
                            Ret = HI_UNF_PVR_PlayResumeChn(PlayChn);
                            if (Ret != HI_SUCCESS)
                            {
                                HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayResumeChn failed.\n");
                            }

                            uPauseFlag = 0xffffffff;
                        }
                    }
                    break;
                    case KEY_FF:
                    case KEY_RW:
                    {
                        HI_UNF_PVR_PLAY_MODE_S stTrickMode;

                        uRate = uRate%5;
                        stTrickMode.enSpeed = (0x1 << (uRate+1)) * HI_UNF_PVR_PLAY_SPEED_NORMAL;

                        if(u64KeyId == KEY_RW)
                        {
                            stTrickMode.enSpeed *= -1;
                        }

                        HI_DEBUG_ADVCA("trick mod:%d\n", stTrickMode.enSpeed);
                        Ret = HI_UNF_PVR_PlayTPlay(PlayChn, &stTrickMode);
                        if (Ret != HI_SUCCESS)
                        {
                            HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayTPlay failed.\n");
                        }

                        uRate++;
                        uPauseFlag = 0;
                    }
                    break;
                    case KEY_STOP:
                        HI_DEBUG_ADVCA("====To stop player channel is %d\n", PlayChn);
                        PVR_StopPlayBack(PlayChn);
                        uPauseFlag = 0;
                    break;
                    case KEY_F1:
                    case KEY_F2:
                    case KEY_F3:
                    case KEY_F4:
                    {
                        HI_UNF_PVR_REC_ATTR_S stRecAttr;

                        uPauseFlag = 0xffffffff;

                        if(PlayChn != 0xffffffff)
                        {
                            PVR_StopPlayBack(PlayChn);
                        }

                        HI_UNF_PVR_RecGetChn(nRecChn,&stRecAttr);
                        stPvrPlayInfo.bIsTimeshiftPlay = HI_FALSE;
                        stPvrPlayInfo.u32RecChnID = 0;

                        PVR_StartPlayBack(stRecAttr.szFileName, &PlayChn, g_hAvplay);
                        HI_DEBUG_ADVCA("====Created Player channel is %d\n", PlayChn);
                    }
                    break;
                    case KEY_SEEK:
                    {
                        HI_UNF_PVR_PLAY_POSITION_S stPos;
                        uRate = 0;

                        uPauseFlag = 0xffffffff;

                        stPos.enPositionType = HI_UNF_PVR_PLAY_POS_TYPE_TIME;
                        stPos.s64Offset = 0;
                        if(uSeekFlag == 0xffffffff)
                        {
                            stPos.s32Whence = SEEK_SET;
                            uSeekFlag = 0;
                            HI_DEBUG_ADVCA("\033[5;31mseek to start\n\033[0m");
                        }
                        else
                        {
                            HI_DEBUG_ADVCA("\033[5;31mseek to end\n\033[0m");
                            stPos.s32Whence = SEEK_END;
                            uSeekFlag = 0xffffffff;
                        }

                        Ret = HI_UNF_PVR_PlaySeek(PlayChn, &stPos);
                        if (Ret != HI_SUCCESS)
                        {
                            HI_DEBUG_ADVCA("call HI_UNF_PVR_PlayStep failed.\n");
                        }
                    }
                    break;
                    default:
                        HI_DEBUG_ADVCA(" IR   KeyId : 0x%llx   PressStatus :%d[%s]\n", u64KeyId, PressStatus, IR_Stuts_Str[PressStatus]);
                    break;
                }
            }
        }
        usleep(10*1000);
    }
    return 0;
}

HI_S32  IRProcess(HI_S32 nProgNum)
{
    HI_S32 Ret;
    HI_UNF_IR_CODE_E ircode = HI_UNF_IR_CODE_NEC_SIMPLE;

    /*open ir device*/
    Ret = HI_UNF_IR_Open();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return Ret;
    }

    Ret = HI_UNF_IR_SetCodeType(ircode);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_Close();
        return Ret;
    }

    Ret = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_Close();
        return Ret;
    }

    Ret = HI_UNF_IR_SetRepKeyTimeoutAttr(108);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_Close();
        return Ret;
    }

    Ret = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_Close();
        return Ret;
    }

    Ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        HI_UNF_IR_Close();
        return Ret;
    }

    g_IrTaskRunning = HI_TRUE;

    /*create a thread for ir receive*/
    Ret = pthread_create(&g_IrThread, NULL, IR_ReceiveTask, (void*)nProgNum);
    if (0 != Ret)
    {
        HI_DEBUG_ADVCA("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        perror("pthread_create");
    }

    return HI_SUCCESS;
}
#endif


