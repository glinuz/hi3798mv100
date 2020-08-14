/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_pvr_rec_allts.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
  History       :
  1.Date        : 2010/01/26
    Author      : q46153
    Modification: Created file

******************************************************************************/

#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_pvr.h"


#define MAX_REC_FILE_SIZE       (2 * 1024 *1024*1024LLU) /*2G*/

HI_U32 g_TunerFreq;
HI_U32 g_TunerSrate;
HI_U32 g_ThirdParam;

extern HI_BOOL g_bIsRecStop;
static pthread_t   g_CmdThd;

HI_S32 DmxInit(HI_U32 tunerId)
{
    HI_S32   Ret;

    Ret = HI_UNF_DMX_Init();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_Init failed.\n");
        return Ret;
    }


    Ret = HIADP_DMX_AttachTSPort(PVR_DMX_ID_REC, tunerId);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HIADP_DMX_AttachTSPort for REC failed.\n");
        HI_UNF_DMX_DeInit();
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 DmxDeInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_REC);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DMX_DeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_DMX_DeInit failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 PVR_RecAllTsStart(char *path, HI_U32 u32DemuxID, HI_U32 freq, HI_U64 maxSize, HI_U32 *pRecChn)
{
    HI_U32 recChn;
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_PVR_REC_ATTR_S   attr;
    HI_CHAR                 szFileName[PVR_MAX_FILENAME_LEN];


    memset(&attr, 0x00, sizeof(attr));
    sprintf(szFileName, "rec_freq_%d.ts", freq);

    sprintf(attr.szFileName, "%s/", path);

    strcat(attr.szFileName, szFileName);

    PVR_SAMPLE_Printf("record file name:%s\n", attr.szFileName);

    attr.u32FileNameLen = strlen(attr.szFileName);
    attr.u32DemuxID    = u32DemuxID;
    attr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
    attr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
    attr.enStreamType  = HI_UNF_PVR_STREAM_TYPE_ALL_TS;
    attr.bRewind = HI_FALSE;
    attr.u64MaxFileSize= maxSize;//source;
    attr.bIsClearStream = HI_TRUE;
    attr.u32UsrDataInfoSize = 0;
    attr.stEncryptCfg.bDoCipher = HI_FALSE;
    attr.u32IndexPid   = 0x1fff;
    attr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_NONE;
    attr.enIndexVidType = HI_UNF_VCODEC_TYPE_MPEG2;
    SAMPLE_RUN(HI_UNF_PVR_RecCreateChn(&recChn, &attr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    SAMPLE_RUN(HI_UNF_PVR_RecStartChn(recChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_RecDestroyChn(recChn);
        return ret;
    }

    *pRecChn = recChn;

    return HI_SUCCESS;
}


HI_S32 PVR_RecAllTsStop(HI_U32 u32RecChnID)
{
    HI_S32 ret;
    HI_UNF_PVR_REC_ATTR_S recAttr;

    SAMPLE_RUN(HI_UNF_PVR_RecGetChn(u32RecChnID, &recAttr), ret) ;
    SAMPLE_RUN(HI_UNF_PVR_RecStopChn(u32RecChnID), ret) ;
    SAMPLE_RUN(HI_UNF_PVR_RecDestroyChn(u32RecChnID), ret) ;

    return ret;
}

HI_VOID * CmdThread(HI_VOID *args)
{
    HI_CHAR                 InputCmd[32];

    while (HI_FALSE == g_bIsRecStop)
    {
        PVR_SAMPLE_Printf("please input the 'q' to quit.\n");

#ifndef ANDROID
        if (!SAMPLE_GET_INPUTCMD(InputCmd))
        {
            sleep(1);
            continue;
        }
#else
        SAMPLE_GET_INPUTCMD(InputCmd);
#endif        
        if ('q' == InputCmd[0])
        {
            PVR_SAMPLE_Printf("now exit!\n");
            g_bIsRecStop = HI_TRUE;
        }
        else
        {
            sleep(1);
            continue;
        }
    }

    return NULL;
}


HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                  Ret;
    HI_U32                  RecChn;
    HI_UNF_PVR_REC_STATUS_S stRecStatus;
    HI_U32                  i = 0;
    HI_U32                  Tuner;
    
    if (6 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        Tuner = strtol(argv[5],NULL,0);
    }
    else if (5 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = strtol(argv[4],NULL,0);
        Tuner = 0;
    }
    else if(4 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = strtol(argv[3],NULL,0);
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        Tuner = 0;        
    }
    else if(3 == argc)
    {
        g_TunerFreq  = strtol(argv[2],NULL,0);
        g_TunerSrate = (g_TunerFreq>1000) ? 27500 : 6875;
        g_ThirdParam = (g_TunerFreq>1000) ? 0 : 64;
        Tuner = 0;        
    }
    else
    {
        printf("Usage: %s path freq [srate] [qamtype or polarization] [Tuner]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n"
                "       Tuner: value can be 0, 1, 2, 3\n",
                argv[0]);

        printf("Example: %s ./ 618 6875 64 0\n", argv[0]);
        printf("  Board HI3796CDMO1B support 3 tuners: \n");
        printf("    Satellite : %s ./ 3840 27500 0  0 0\n", argv[0]);
        printf("    Cable     : %s ./ 618 6875 64   1 0\n", argv[0]);
        printf("    Terestrial: %s ./ 474 8000 64   2 0\n", argv[0]);        
        return HI_FAILURE;
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    Ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Demux_Init failed.\n");
        return Ret;
    }

    Ret = HIADP_Tuner_Connect(Tuner,g_TunerFreq,g_TunerSrate,g_ThirdParam);
    if (HI_SUCCESS != Ret)
    {
        PVR_SAMPLE_Printf("call HIADP_Tuner_Connect failed.\n");
        return Ret;
    }


    Ret = DmxInit(Tuner);
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call DmxInit failed.\n");
        return Ret;
    }

    Ret = HI_UNF_PVR_RecInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call HI_UNF_PVR_RecInit failed.\n");
        return Ret;
    }

    Ret = PVR_RegisterCallBacks();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call PVR_RegisterCallBacks failed.\n");
        return Ret;
    }

    PVR_RecAllTsStart(argv[1], PVR_DMX_ID_REC, g_TunerFreq, MAX_REC_FILE_SIZE, &RecChn);
    pthread_create(&g_CmdThd, HI_NULL, CmdThread, HI_NULL);


    while (HI_FALSE == g_bIsRecStop)
    {
        sleep(1);

        if (0 == i%5)
        {
            Ret = HI_UNF_PVR_RecGetStatus(RecChn, &stRecStatus);
            if (Ret != HI_SUCCESS)
            {
                PVR_SAMPLE_Printf("call HI_UNF_PVR_RecGetStatus failed.\n");
                break;
            }

            PVR_SAMPLE_Printf("== now record: \n");
            PVR_SAMPLE_Printf("      size:%lld M\n", stRecStatus.u64CurWritePos/1024/1024);
            PVR_SAMPLE_Printf("      time:%d s\n",   stRecStatus.u32CurTimeInMs/1000);
            PVR_SAMPLE_Printf("      buff:%d/%d \n",  stRecStatus.stRecBufStatus.u32UsedSize, stRecStatus.stRecBufStatus.u32BufSize);
        }
        i++;


    }

    g_bIsRecStop = HI_TRUE;

    //pthread_join(g_CmdThd, HI_NULL);
    PVR_RecAllTsStop(RecChn);


    Ret = DmxDeInit();
    if (Ret != HI_SUCCESS)
    {
        PVR_SAMPLE_Printf("call DmxDeInit failed.\n");
        return Ret;
    }

    HI_SYS_DeInit();

    return 0;
}


