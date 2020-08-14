/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: sample_dolby_dvbplay.c
 * Description:
 *       this sample finish the dvbplay function.
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2011-07-11   184737
 ******************************************************************************/
#include <pthread.h>
#include "dolby_dvbplay.h"
#include "dolby_common.h"
#include "hi_go.h"
#include "dolby_win.h"
#include "hi_unf_gpio.h"

#define PLAY_DEMUX_ID 0
#define PLAY_TUNER_ID 0

extern HI_S32 HI_MPI_AVPLAY_SetDDPTestMode(HI_HANDLE hAvplay, HI_BOOL bEnable);

/*tuner device type*/

/*the handle of VO window*/
static HI_HANDLE  s_hWin;

/*the Mutex to protect shared variable*/
extern pthread_mutex_t g_Mutex;

QAM_S stQAMMap[MAX_QAM_MAP_LENGTH] =
{
    {16,"QAM16 ",},
    {32,"QAM32 ",},
    {64,"QAM64 ",},
    {128,"QAM128",},
    {256,"QAM256"},
};

static HI_S32 DOLBY_Tuner_Init(HI_VOID)
{
    return HIADP_Tuner_Init();
}

/* ture:mute, false:unmute */
static HI_VOID  DOLBY_AudioHardwareMute(HI_BOOL bMute)
{
    printf("%s: %s \n", __FUNCTION__, bMute ? "Mute" : "UnMute");

    /* config pin multiplex */
    HI_SYS_WriteRegister(0x10203150, 0x03);

    HI_UNF_GPIO_Init();
    HI_UNF_GPIO_SetDirBit(84, HI_FALSE);
    if (bMute) /*Set Mute*/
    {
        HI_UNF_GPIO_WriteBit(84, HI_TRUE);
    }
    else /* UnMute */
    {
        HI_UNF_GPIO_WriteBit(84, HI_FALSE);
    }
    HI_UNF_GPIO_DeInit();
}

static HI_S32  DOLBY_Search_GetAllPmt(HI_U32 u32DmxId, PMT_COMPACT_TBL **ppProgTable)
{
    HI_S32   s32Ret;
    PAT_TB   pat_tb;

    if ( HI_NULL == ppProgTable)
    {
        return HI_FAILURE;
    }

    memset(&pat_tb, 0, sizeof(pat_tb));

    /* parse PAT, save program information in pat_tb */
    s32Ret = SRH_PATRequest(u32DmxId, &pat_tb);
    if (HI_SUCCESS != s32Ret)
    {
        printf("failed to search PAT\n");
        return HI_FAILURE;
    }

    if (0 == pat_tb.u16ProgNum)
    {
        printf("no Program searched!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_DOLBY_ParsePMT(0,pat_tb,ppProgTable);
    if (HI_SUCCESS != s32Ret)
    {
        printf(" DOLBY_Search_GetAllPmt call HI_DOLBY_ParsePMT failed\n");
        return HI_FAILURE;
    }

    return s32Ret;
}


HI_S32 HI_DOLBY_ParsePMT(HI_U32 u32DmxId, PAT_TB pat_tb,PMT_COMPACT_TBL **ppProgTable)
{
    HI_S32 i, s32Ret;
    PMT_TB pmt_tb;
    PMT_COMPACT_TBL *pProgTable = HI_NULL;
    HI_U32 u32Promnum = 0;

    memset(&pmt_tb, 0, sizeof(pmt_tb));

    pProgTable = (PMT_COMPACT_TBL*)malloc(sizeof(PMT_COMPACT_TBL));
    if (HI_NULL == pProgTable)
    {
        printf("have no memory for pat\n");
        return HI_FAILURE;
    }

    pProgTable->prog_num = pat_tb.u16ProgNum;
    pProgTable->proginfo = (PMT_COMPACT_PROG*)malloc(pat_tb.u16ProgNum * sizeof(PMT_COMPACT_PROG));
    if (HI_NULL == pProgTable->proginfo)
    {
        printf("have no memory for pat\n");
        free(pProgTable);
        pProgTable = HI_NULL;
        return HI_FAILURE;
    }

    memset(pProgTable->proginfo, 0, pat_tb.u16ProgNum * sizeof(PMT_COMPACT_PROG));

   //printf("\n\nALL Program Infomation:\n");

    for (i = 0; i < pat_tb.u16ProgNum; i++)
    {
        if ((pat_tb.PatInfo[i].u16ServiceID == 0) || (pat_tb.PatInfo[i].u16PmtPid == 0x1fff))
        {
            continue;
        }

        memset(&pmt_tb, 0, sizeof(PMT_TB));
        s32Ret = SRH_PMTRequest(u32DmxId, &pmt_tb, pat_tb.PatInfo[i].u16PmtPid, pat_tb.PatInfo[i].u16ServiceID);
        if (HI_SUCCESS != s32Ret)
        {
            printf("failed to search PMT[%d]\n", i);
            free(pProgTable->proginfo);
            free(pProgTable);
            return HI_FAILURE;
            //continue;
        }

        pProgTable->proginfo[u32Promnum].ProgID = pat_tb.PatInfo[i].u16ServiceID;
        pProgTable->proginfo[u32Promnum].PmtPid = pat_tb.PatInfo[i].u16PmtPid;
        pProgTable->proginfo[u32Promnum].PcrPid = pmt_tb.u16PcrPid;
        pProgTable->proginfo[u32Promnum].VideoType   = pmt_tb.Videoinfo[0].u32VideoEncType;
        pProgTable->proginfo[u32Promnum].VElementNum = pmt_tb.u16VideoNum;
        pProgTable->proginfo[u32Promnum].VElementPid = pmt_tb.Videoinfo[0].u16VideoPid;
        pProgTable->proginfo[u32Promnum].AudioType   = pmt_tb.Audioinfo[0].u32AudioEncType;
        pProgTable->proginfo[u32Promnum].AElementNum = pmt_tb.u16AudoNum;
        pProgTable->proginfo[u32Promnum].AElementPid = pmt_tb.Audioinfo[0].u16AudioPid;
#if 0

        printf("\t!!! AudioNum = %d , VideoNum = %d !!!\n",pmt_tb.u16AudoNum,pmt_tb.u16VideoNum);
        printf("Channel Num = %d, Program ID = %d PMT PID = 0x%x,\n", u32Promnum + 1, pat_tb.PatInfo[i].u16ServiceID,
               pat_tb.PatInfo[i].u16PmtPid);

        for (j = 0; j < pmt_tb.u16AudoNum; j++)
        {
            printf("\tAudio Stream PID   = 0x%x\n",pmt_tb.Audioinfo[j].u16AudioPid);
            switch (pmt_tb.Audioinfo[j].u32AudioEncType)
            {
            case HA_AUDIO_ID_MP3:
                printf("\tAudio Stream Type MP3\n");
                break;
            case HA_AUDIO_ID_AAC:
                printf("\tAudio Stream Type AAC\n");
                break;
            case HA_AUDIO_ID_DOLBY_PLUS:
                printf("\tAudio Stream Type AC3\n");
                break;
            default:
                printf("\tAudio Stream Type error\n");
            }
        }

        printf("\t!!! Audio Stream end !!!\n");

        for (j = 0; j < pmt_tb.u16VideoNum; j++)
        {
            printf("\tVideo Stream PID   = 0x%x\n",pmt_tb.Videoinfo[j].u16VideoPid);
            switch (pmt_tb.Videoinfo[j].u32VideoEncType)
            {
            case HI_UNF_VCODEC_TYPE_H264:
                printf("\tVideo Stream Type H264\n");
                break;
            case HI_UNF_VCODEC_TYPE_MPEG2:
                printf("\tVideo Stream Type MP2\n");
                break;
            case HI_UNF_VCODEC_TYPE_MPEG4:
                printf("\tVideo Stream Type MP4\n");
                break;
            default:
                printf("\tVideo Stream Type error\n");
            }
        }

        printf("\t!!! Video Stream end !!!\n");
#endif
        if ((pmt_tb.u16VideoNum > 0) || (pmt_tb.u16AudoNum > 0))
        {
            u32Promnum++;
        }
    }

    pProgTable->prog_num = u32Promnum;
    //printf("\t!!! u32Promnum =  %d !!!\n",pProgTable->prog_num);

    if(HI_NULL !=(*ppProgTable))
    {
        if (HI_NULL != (*ppProgTable)->proginfo)
        {
            free((*ppProgTable)->proginfo);
            //printf("\t!!! free  proginfo!!!\n");
            (*ppProgTable)->proginfo = HI_NULL;
        }
        free(*ppProgTable);
        //printf("\t!!! free *ppProgTable!!!\n");
    }

    *ppProgTable = pProgTable;
    return HI_SUCCESS;
}

HI_S32 HI_DOLBY_DvbPlay(HI_CHAR *pcProgSearchArg[],HI_HANDLE *phAvplay,PMT_COMPACT_TBL  **ppstProgTbl)
{
    HI_S32                      s32Ret;
    HI_UNF_AVPLAY_ATTR_S        stAvplayAttr;
    HI_UNF_SYNC_ATTR_S          stSyncAttr;
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;
    HI_U32                      u32TunerFreq;
    HI_U32                      u32TunerSrate;
    HI_U32                      u32ThirdParam;
//    HI_U32                      u32Playtime;
    HI_U32                      i = 0;
    HI_HANDLE hTrack;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;

    Hi_Dolby_Debug();

    if (HI_NULL != pcProgSearchArg)
    {
        u32TunerFreq  = strtol(pcProgSearchArg[0],NULL,0);
        u32TunerSrate = strtol(pcProgSearchArg[1],NULL,0);
        for(i = 0; i< MAX_QAM_MAP_LENGTH; i++)
        {
            if(HI_SUCCESS == strcmp(stQAMMap[i].acQam,pcProgSearchArg[2]))
            {
                break;
            }
        }

        if(i< MAX_QAM_MAP_LENGTH)
        {
            u32ThirdParam   = stQAMMap[i].u32Qam;
        }
        else
        {
            printf("HI_DOLBY_DvbPlay QAM Wrong ! \n");
            return HI_FAILURE;
        }
    }
    else
    {
        printf("HI_DOLBY_DvbPlay Arg Error ! \n");
        return HI_FAILURE;
    }

    printf("Freq = %d , Srate = %d , QAM = %d\n",u32TunerFreq,u32TunerSrate,u32ThirdParam);

    s32Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Snd_Init failed.\n");
        goto SYS_DEINIT;
    }
    DOLBY_AudioHardwareMute(HI_FALSE);


   // HI_SYS_GetPlayTime(&u32Playtime);
    //printf("u32Playtime = %d\n",u32Playtime);

#if 0
    s32Ret = HIADP_Disp_Init(HI_UNF_ENC_FMT_1080i_50);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Disp_DeInit failed.\n");
        goto SND_DEINIT;
    }
#endif

    s32Ret = HIADP_VO_Init(HI_UNF_VO_DEV_MODE_NORMAL);
    s32Ret |= HIADP_VO_CreatWin(HI_NULL,&s_hWin);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_VO_Init failed.\n");
        HIADP_VO_DeInit();
        goto DISP_DEINIT;
    }

    s32Ret = HI_UNF_DMX_Init();
    Hi_Dolby_Debug();
    s32Ret |= HIADP_DMX_AttachTSPort(PLAY_DEMUX_ID, PLAY_TUNER_ID);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto VO_DEINIT;
    }
    Hi_Dolby_Debug();


    Hi_Dolby_Debug();
    s32Ret = DOLBY_Tuner_Init();
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto DMX_DEINIT;
    }
    Hi_Dolby_Debug();

    s32Ret = HIADP_Tuner_Connect(PLAY_TUNER_ID, u32TunerFreq, u32TunerSrate, u32ThirdParam);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }
    Hi_Dolby_Debug();

    HIADP_Search_Init();

    s32Ret = DOLBY_Search_GetAllPmt(PLAY_DEMUX_ID, ppstProgTbl);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    s32Ret = HIADP_AVPlay_RegADecLib();

    s32Ret |= HI_UNF_AVPLAY_Init();
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        goto PSISI_FREE;
    }

    s32Ret = HI_UNF_AVPLAY_GetDefaultConfig(&stAvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    stAvplayAttr.u32DemuxId = PLAY_DEMUX_ID;
    s32Ret |= HI_UNF_AVPLAY_Create(&stAvplayAttr, phAvplay);
    Hi_Dolby_Debug();
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        goto AVPLAY_DEINIT;
    }
    HI_MPI_AVPLAY_SetDDPTestMode(*phAvplay,HI_TRUE);

    s32Ret = HI_UNF_AVPLAY_ChnOpen(*phAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    s32Ret |= HI_UNF_AVPLAY_ChnOpen((*phAvplay), HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        goto CHN_CLOSE;
    }

    s32Ret = HI_UNF_VO_AttachWindow(s_hWin, *phAvplay);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_UNF_VO_AttachWindow failed.\n");
        goto CHN_CLOSE;
    }

   s32Ret = HI_UNF_VO_SetWindowEnable(s_hWin, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        goto WIN_DETACH;
    }

    //stTrackAttr.enTrackType = HI_UNF_SND_TRACK_TYPE_MASTER;
    s32Ret = HI_UNF_SND_GetDefaultTrackAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_GetDefaultTrackAttr failed.\n");
        goto WIN_DETACH;
    }
    s32Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr,&hTrack);
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_CreateTrack failed.\n");
        goto WIN_DETACH;
    }

    s32Ret = HI_UNF_SND_Attach(hTrack, *phAvplay);
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_SND_Attach failed.\n");
        goto TRACK_DESTROY;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(*phAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
    stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    stSyncAttr.stSyncStartRegion.s32VidPlusTime = 30;
    stSyncAttr.stSyncStartRegion.s32VidNegativeTime = -15;
    stSyncAttr.bQuickOutput = HI_FALSE;
    s32Ret = HI_UNF_AVPLAY_SetAttr(*phAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
    if (s32Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        goto SND_DETACH;
    }

    s32Ret = HIADP_AVPlay_PlayProg(*phAvplay,*ppstProgTbl,0,HI_TRUE);
    if (s32Ret != HI_SUCCESS)
    {
        printf("call SwitchProg failed.\n");
        goto AVPLAY_STOP;
    }

    return HI_SUCCESS;

AVPLAY_STOP:
    stStop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    stStop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(*phAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStop);

SND_DETACH:
    HI_UNF_SND_Detach(hTrack, *phAvplay);

TRACK_DESTROY:
    HI_UNF_SND_DestroyTrack(hTrack);

WIN_DETACH:
    HI_UNF_VO_SetWindowEnable(s_hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(s_hWin, *phAvplay);

CHN_CLOSE:
    HI_UNF_AVPLAY_ChnClose(*phAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);

    HI_UNF_AVPLAY_Destroy(*phAvplay);

AVPLAY_DEINIT:
    HI_UNF_AVPLAY_DeInit();

PSISI_FREE:
    HIADP_Search_FreeAllPmt(*ppstProgTbl);
    HIADP_Search_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

VO_DEINIT:
    HI_UNF_VO_DestroyWindow(s_hWin);
    HIADP_VO_DeInit();

DISP_DEINIT:
    //HIADP_Disp_DeInit();

#if 0
SND_DEINIT:
    HIADP_Snd_DeInit();
#endif

SYS_DEINIT:
    //HI_SYS_DeInit();

    return s32Ret;
}

HI_S32 HI_DOLBY_DvbPlayDeinit(HI_HANDLE *phAvplay)
{
    HI_UNF_AVPLAY_STOP_OPT_S    stStop;

    /*AVPLAY_STOP:*/
    stStop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    stStop.u32TimeoutMs = 0;
    HI_UNF_AVPLAY_Stop(*phAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &stStop);

    /*SND_DETACH:*/
    HI_UNF_SND_Detach(HI_UNF_SND_0, *phAvplay);

    /*WIN_DETACH:*/
    HI_UNF_VO_SetWindowEnable(s_hWin,HI_FALSE);
    HI_UNF_VO_DetachWindow(s_hWin, *phAvplay);

    /*CHN_CLOSE:*/
    HI_UNF_AVPLAY_ChnClose(*phAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    HI_UNF_AVPLAY_Destroy(*phAvplay);

    /*AVPLAY_DEINIT:*/
    HI_UNF_AVPLAY_DeInit();

    /*PSISI_FREE:*/
    //HIADP_Search_FreeAllPmt(g_pstProgTbl);
    HIADP_Search_DeInit();

    /*TUNER_DEINIT:*/
    HIADP_Tuner_DeInit();

    /*DMX_DEINIT:*/
    HI_UNF_DMX_DetachTSPort(PLAY_DEMUX_ID);
    HI_UNF_DMX_DeInit();

    /*VO_DEINIT:*/
    HI_UNF_VO_DestroyWindow(s_hWin);
    HIADP_VO_DeInit();

    *phAvplay = 0;

    return HI_SUCCESS;
#if 0
    DISP_DEINIT:
        HIADP_Disp_DeInit();

    SND_DEINIT:
        HIADP_Snd_DeInit();

    SYS_DEINIT:
        HI_SYS_DeInit();
#endif
}
