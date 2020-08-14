
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_unf_ecs.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"

#define DEMUX_ID        0
#define PORT_ID         DEFAULT_DVB_PORT

#define PAT_PID         0
#define PAT_TABLEID     0

#define INVALID_PID     0x1FFF
//#define USE_ORIGINAL_SCD

PMT_COMPACT_TBL    *g_pProgTbl = HI_NULL;

typedef struct
{
    HI_HANDLE   RecHandle;
    HI_CHAR     FileName[256];
    HI_BOOL     ThreadRunFlag;
} TsFileInfo;

#ifdef USE_ORIGINAL_SCD
typedef struct hiDMX_DATA_S
{
    HI_U8   *pDataAddr;
    HI_U32   u32PhyAddr;
    HI_U32   u32Len;
} DMX_DATA_S;
extern HI_S32 HI_MPI_DMX_AcquireRecScdBuf(HI_U32 u32DmxId, DMX_DATA_S *pstBuf, HI_U32 u32TimeoutMs);
extern HI_S32 HI_MPI_DMX_ReleaseRecScdBuf(HI_U32 u32DmxId, const DMX_DATA_S *pstBuf);
#endif

HI_VOID* SaveRecDataThread(HI_VOID *arg)
{
    HI_S32      ret;
    TsFileInfo *Ts      = (TsFileInfo*)arg;
    FILE       *RecFile = HI_NULL;

    RecFile = fopen(Ts->FileName, "w");
    if (!RecFile)
    {
        perror("fopen error");

        Ts->ThreadRunFlag = HI_FALSE;

        return HI_NULL;
    }

    printf("[%s] open file %s\n", __FUNCTION__, Ts->FileName);

    while (Ts->ThreadRunFlag)
    {
        #ifndef USE_ORIGINAL_SCD
        HI_UNF_DMX_REC_DATA_S RecData;

        ret = HI_UNF_DMX_AcquireRecData(Ts->RecHandle, &RecData, 100);
        #else
        DMX_DATA_S RecData;
        ret = HI_MPI_DMX_AcquireRecScdBuf(DEMUX_ID, &RecData, 1000);
        #endif
        if (HI_SUCCESS != ret)
        {
            if (HI_ERR_DMX_TIMEOUT == ret)
            {
                continue;
            }

            if (HI_ERR_DMX_NOAVAILABLE_DATA == ret)
            {
                continue;
            }

            printf("[%s] HI_UNF_DMX_AcquireRecData failed 0x%x\n", __FUNCTION__, ret);

            break;
        }

        if (RecData.u32Len != fwrite(RecData.pDataAddr, 1, RecData.u32Len, RecFile))
        {
            perror("[SaveRecDataThread] fwrite error");

            break;
        }
        #ifndef USE_ORIGINAL_SCD
        ret = HI_UNF_DMX_ReleaseRecData(Ts->RecHandle, &RecData);
        #else
        ret = HI_MPI_DMX_ReleaseRecScdBuf(DEMUX_ID, &RecData);
        #endif
        if (HI_SUCCESS != ret)
        {
            printf("[%s] HI_UNF_DMX_ReleaseRecData failed 0x%x\n", __FUNCTION__, ret);

            break;
        }

    }

    fclose(RecFile);

    Ts->ThreadRunFlag = HI_FALSE;

    return HI_NULL;
}

HI_VOID* SaveIndexDataThread(HI_VOID *arg)
{
    HI_S32                  ret;
    TsFileInfo             *Ts          = (TsFileInfo*)arg;
    FILE                   *IndexFile   = HI_NULL;
    HI_U32                  IndexCount  = 0;
    HI_UNF_DMX_REC_INDEX_S  RecIndex[64];

    IndexFile = fopen(Ts->FileName, "w");
    if (!IndexFile)
    {
        perror("fopen error");

        Ts->ThreadRunFlag = HI_FALSE;

        return HI_NULL;
    }

    printf("[%s] open file %s\n", __FUNCTION__, Ts->FileName);

    while (Ts->ThreadRunFlag)
    {
        ret = HI_UNF_DMX_AcquireRecIndex(Ts->RecHandle, &RecIndex[IndexCount], 100);
        if (HI_SUCCESS != ret)
        {
            if ((HI_ERR_DMX_NOAVAILABLE_DATA == ret) || (HI_ERR_DMX_TIMEOUT == ret))
            {
                continue;
            }

            printf("[%s] HI_UNF_DMX_AcquireRecIndex failed 0x%x\n", __FUNCTION__, ret);

            break;
        }

        if (++IndexCount >= (sizeof(RecIndex) / sizeof(RecIndex[0])))
        {
            IndexCount = 0;

            if (sizeof(RecIndex) != fwrite(&RecIndex, 1, sizeof(RecIndex), IndexFile))
            {
                perror("[SaveIndexDataThread] fwrite error");

                break;
            }

            fflush(IndexFile);
        }
    }

    fclose(IndexFile);

    Ts->ThreadRunFlag = HI_FALSE;

    return HI_NULL;
}

HI_S32 DmxStartRecord(HI_CHAR *Path, PMT_COMPACT_PROG *ProgInfo)
{
    HI_S32                  ret;
    HI_UNF_DMX_REC_ATTR_S   RecAttr;
    HI_HANDLE               RecHandle;
    HI_HANDLE               ChanHandle[8];
    HI_U32                  ChanCount       = 0;
    HI_BOOL                 RecordStatus    = HI_FALSE;
    pthread_t               RecThreadId     = -1;
    pthread_t               IndexThreadId   = -1;
    TsFileInfo              TsRecInfo;
    TsFileInfo              TsIndexInfo;
    HI_CHAR                 FileName[256];
    HI_U32                  i;

    RecAttr.u32DmxId        = DEMUX_ID;
    RecAttr.u32RecBufSize   = 4 * 1024 * 1024;
    RecAttr.enRecType       = HI_UNF_DMX_REC_TYPE_SELECT_PID;
    RecAttr.bDescramed      = HI_TRUE;

    if (ProgInfo->VElementPid < INVALID_PID)
    {
        RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_VIDEO;
        RecAttr.u32IndexSrcPid  = ProgInfo->VElementPid;
        RecAttr.enVCodecType    = HI_UNF_VCODEC_TYPE_MPEG2;
    }
    else if (ProgInfo->AElementPid < INVALID_PID)
    {
        RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_AUDIO;
        RecAttr.u32IndexSrcPid  = ProgInfo->AElementPid;
    }
    else
    {
        RecAttr.enIndexType     = HI_UNF_DMX_REC_INDEX_TYPE_NONE;
    }

    ret = HI_UNF_DMX_CreateRecChn(&RecAttr, &RecHandle);
    if (HI_SUCCESS != ret)
    {
        printf("[%s - %u] HI_UNF_DMX_CreateRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);

        return ret;;
    }

    ret = HI_UNF_DMX_AddRecPid(RecHandle, PAT_PID, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret)
    {
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);

        goto exit;
    }

    ++ChanCount;

    ret = HI_UNF_DMX_AddRecPid(RecHandle, ProgInfo->PmtPid, &ChanHandle[ChanCount]);
    if (HI_SUCCESS != ret)
    {
        printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);

        goto exit;
    }

    ++ChanCount;

    sprintf(FileName, "%s/rec", Path);

    if (ProgInfo->VElementPid < INVALID_PID)
    {
        ret = HI_UNF_DMX_AddRecPid(RecHandle, ProgInfo->VElementPid, &ChanHandle[ChanCount]);
        if (HI_SUCCESS != ret)
        {
            printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);

            goto exit;
        }

        sprintf(FileName, "%s_v%u", FileName, ProgInfo->VElementPid);

        ++ChanCount;
    }

    if (ProgInfo->AElementPid < INVALID_PID)
    {
        ret = HI_UNF_DMX_AddRecPid(RecHandle, ProgInfo->AElementPid, &ChanHandle[ChanCount]);
        if (HI_SUCCESS != ret)
        {
            printf("[%s - %u] HI_UNF_DMX_AddRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);

            goto exit;
        }

        sprintf(FileName, "%s_a%u", FileName, ProgInfo->AElementPid);

        ++ChanCount;
    }

    ret = HI_UNF_DMX_StartRecChn(RecHandle);
    if (HI_SUCCESS != ret)
    {
        printf("[%s - %u] HI_UNF_DMX_StartRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);

        goto exit;
    }

    RecordStatus = HI_TRUE;

    TsRecInfo.RecHandle     = RecHandle;
    TsRecInfo.ThreadRunFlag = HI_TRUE;
    sprintf(TsRecInfo.FileName, "%s.ts", FileName);

    ret = pthread_create(&RecThreadId, HI_NULL, SaveRecDataThread, (HI_VOID*)&TsRecInfo);
    if (0 != ret)
    {
        perror("[DmxStartRecord] pthread_create record error");

        goto exit;
    }

    TsIndexInfo.RecHandle       = RecHandle;
    TsIndexInfo.ThreadRunFlag   = HI_TRUE;
    sprintf(TsIndexInfo.FileName, "%s.ts.idx", FileName);

    ret = pthread_create(&IndexThreadId, HI_NULL, SaveIndexDataThread, (HI_VOID*)&TsIndexInfo);
    if (0 != ret)
    {
        perror("[DmxStartRecord] pthread_create index error");

        goto exit;
    }

    sleep(1);

    while (1)
    {
        HI_CHAR InputCmd[256]   = {0};

        printf("please input the q to quit!\n");

        SAMPLE_GET_INPUTCMD(InputCmd);
        if ('q' == InputCmd[0])
        {
            break;
        }
    }

exit :
    if (-1 != RecThreadId)
    {
        TsRecInfo.ThreadRunFlag = HI_FALSE;
        pthread_join(RecThreadId, HI_NULL);
    }

    if (-1 != IndexThreadId)
    {
        TsIndexInfo.ThreadRunFlag = HI_FALSE;
        pthread_join(IndexThreadId, HI_NULL);
    }

    if (RecordStatus)
    {
        ret = HI_UNF_DMX_StopRecChn(RecHandle);
        if (HI_SUCCESS != ret)
        {
            printf("[%s - %u] HI_UNF_DMX_StopRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        }
    }

    for (i = 0; i < ChanCount; i++)
    {
        ret = HI_UNF_DMX_DelRecPid(RecHandle, ChanHandle[i]);
        if (HI_SUCCESS != ret)
        {
            printf("[%s - %u] HI_UNF_DMX_DelRecPid failed 0x%x\n", __FUNCTION__, __LINE__, ret);
        }
    }

    ret = HI_UNF_DMX_DestroyRecChn(RecHandle);
    if (HI_SUCCESS != ret)
    {
        printf("[%s - %u] HI_UNF_DMX_DestroyRecChn failed 0x%x\n", __FUNCTION__, __LINE__, ret);
    }

    return ret;
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32      ret;
    HI_CHAR    *Path            = HI_NULL;
    HI_U32      Freq            = 610;
    HI_U32      SymbolRate      = 6875;
    HI_U32      ThridParam      = 64;
    HI_U32      ProgNum         = 0;
    HI_CHAR     InputCmd[256]   = {0};

    if (argc < 3)
    {
        printf( "Usage: %s path freq [srate] [qamtype or polarization]\n"
                "       qamtype or polarization: \n"
                "           For cable, used as qamtype, value can be 16|32|[64]|128|256|512 defaut[64] \n"
                "           For satellite, used as polarization, value can be [0] horizontal | 1 vertical defaut[0] \n",
                argv[0]);

        return -1;
    }

    Path        = argv[1];
    Freq        = strtol(argv[2], NULL, 0);
    SymbolRate  = (Freq > 1000) ? 27500 : 6875;
    ThridParam  = (Freq > 1000) ? 0 : 64;

    if (argc >= 4)
    {
        SymbolRate  = strtol(argv[3], NULL, 0);
    }

    if (argc >= 5)
    {
        ThridParam = strtol(argv[4], NULL, 0);
    }

    HI_SYS_Init();

    HIADP_MCE_Exit();

    ret = HIADP_Tuner_Init();
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Demux_Init failed.\n");
        goto TUNER_DEINIT;
    }

    ret = HIADP_Tuner_Connect(TUNER_USE, Freq, SymbolRate, ThridParam);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Tuner_Connect failed.\n");
        goto TUNER_DEINIT;
    }

    ret = HI_UNF_DMX_Init();
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_DMX_Init failed 0x%x\n", ret);
        goto TUNER_DEINIT;
    }

    ret = HI_UNF_DMX_AttachTSPort(DEMUX_ID, PORT_ID);
    if (HI_SUCCESS != ret)
    {
        printf("HI_UNF_DMX_AttachTSPort failed 0x%x\n", ret);
        goto DMX_DEINIT;
    }

    HIADP_Search_Init();

    ret = HIADP_Search_GetAllPmt(DEMUX_ID, &g_pProgTbl);
    if (HI_SUCCESS != ret)
    {
        printf("call HIADP_Search_GetAllPmt failed\n");
        goto PSISI_FREE;
    }

    printf("\nPlease input the number of program to record: ");

    SAMPLE_GET_INPUTCMD(InputCmd);

    ProgNum = (atoi(InputCmd) - 1) % g_pProgTbl->prog_num;

    DmxStartRecord(Path, g_pProgTbl->proginfo + ProgNum);

PSISI_FREE:
    HIADP_Search_FreeAllPmt(g_pProgTbl);
    HIADP_Search_DeInit();

DMX_DEINIT:
    HI_UNF_DMX_DetachTSPort(DEMUX_ID);
    HI_UNF_DMX_DeInit();

TUNER_DEINIT:
    HIADP_Tuner_DeInit();
    HI_SYS_DeInit();

    return ret;
}


