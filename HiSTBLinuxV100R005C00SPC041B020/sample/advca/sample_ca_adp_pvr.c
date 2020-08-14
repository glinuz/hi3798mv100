/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_ca_adp_pvr.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <sys/time.h>
#include "hi_adp.h"
#include "sample_ca_adp_pvr.h"
#include "hi_adp_mpi.h"
#include "sample_ca_common.h"
#include "hi_unf_advca.h"
//#define PVR_ADVCA_MODE /*if use ADVCA mode,define PVR_ADVCA_MODE*/
/***************************** Macro Definition ******************************/
extern HI_S32 HI_PVR_SetCAData(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32CADataLen);
extern HI_S32 HI_PVR_GetCAData(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* u32CADataLen);

/*lint -save -e818  */
HI_S32 ReadCallBack(HI_UNF_PVR_DATA_ATTR_S *pstDataAttr,
                            HI_U8 *pu8DataAddr,
                            HI_U32 u32PhyAddr,
                            HI_U32 u32Offset,
                            HI_U32 u32DataSize)
{
    HI_UNF_PVR_CA_CALLBACK_ARGS_S stCAPVRArgs;
    memset(&stCAPVRArgs,0,sizeof(HI_UNF_PVR_CA_CALLBACK_ARGS_S));
    stCAPVRArgs.pFileName = pstDataAttr->IdxFileName;
    stCAPVRArgs.u32ChnID = pstDataAttr->u32ChnID;
    stCAPVRArgs.u32DataSize = u32DataSize;
    stCAPVRArgs.u32PhyAddr = u32PhyAddr;
    stCAPVRArgs.u64GlobalOffset = pstDataAttr->u64GlobalOffset;
    return HI_UNF_ADVCA_PVR_ReadCallBack(&stCAPVRArgs);
}

HI_S32 WriteCallBack(HI_UNF_PVR_DATA_ATTR_S *pstDataAttr,
                            HI_U8 *pu8DataAddr,
                            HI_U32 u32PhyAddr,
                            HI_U32 u32Offset,
                            HI_U32 u32DataSize)
{
    HI_UNF_PVR_CA_CALLBACK_ARGS_S stCAPVRArgs;
    memset(&stCAPVRArgs,0,sizeof(HI_UNF_PVR_CA_CALLBACK_ARGS_S));
    stCAPVRArgs.pFileName = pstDataAttr->IdxFileName;
    stCAPVRArgs.u32ChnID = pstDataAttr->u32ChnID;
    stCAPVRArgs.u32DataSize = u32DataSize;
    stCAPVRArgs.u32PhyAddr = u32PhyAddr;
    stCAPVRArgs.u64GlobalOffset = pstDataAttr->u64GlobalOffset;
    return HI_UNF_ADVCA_PVR_WriteCallBack(&stCAPVRArgs);
}
/*************************** Structure Definition ****************************/
typedef struct hiTS_SEND_ARGS_S
{
    HI_U32 u32DmxId;
    HI_U32 u32PortId;
    FILE   *pTsFile;
} TS_SEND_ARGS_S;

typedef struct tagPVREventType
{
    HI_U8 szEventTypeName[128];
    HI_UNF_PVR_EVENT_E eEventID;
}PVR_EVENT_TYPE_ST;
/********************** Global Variable declaration **************************/
static HI_S32       g_s32PvrRecChnNum[DMX_CNT]; /* number of record channel for each demux */
static HI_HANDLE    g_hPvrRecChns[DMX_CNT][8];  /* handle of record channel for each demux */

HI_BOOL             g_bIsRecStop = HI_FALSE;
static HI_BOOL      g_bStopTsThread = HI_FALSE;
HI_HANDLE           g_hTsBufForPlayBack;


static PVR_EVENT_TYPE_ST g_stEventType[] = {
    {"HI_UNF_PVR_EVENT_PLAY_EOF",       HI_UNF_PVR_EVENT_PLAY_EOF},
    {"HI_UNF_PVR_EVENT_PLAY_SOF",       HI_UNF_PVR_EVENT_PLAY_SOF},
    {"HI_UNF_PVR_EVENT_PLAY_ERROR",     HI_UNF_PVR_EVENT_PLAY_ERROR},
    {"HI_UNF_PVR_EVENT_PLAY_REACH_REC", HI_UNF_PVR_EVENT_PLAY_REACH_REC},
    {"HI_UNF_PVR_EVENT_PLAY_RESV",      HI_UNF_PVR_EVENT_PLAY_RESV},
    {"HI_UNF_PVR_EVENT_REC_DISKFULL",   HI_UNF_PVR_EVENT_REC_DISKFULL},
    {"HI_UNF_PVR_EVENT_REC_ERROR",      HI_UNF_PVR_EVENT_REC_ERROR},
    {"HI_UNF_PVR_EVENT_REC_OVER_FIX",   HI_UNF_PVR_EVENT_REC_OVER_FIX},
    {"HI_UNF_PVR_EVENT_REC_REACH_PLAY", HI_UNF_PVR_EVENT_REC_REACH_PLAY},
    {"HI_UNF_PVR_EVENT_REC_DISK_SLOW",  HI_UNF_PVR_EVENT_REC_DISK_SLOW},
    {"HI_UNF_PVR_EVENT_REC_RESV",       HI_UNF_PVR_EVENT_REC_RESV},
    {"HI_UNF_PVR_EVENT_BUTT",           HI_UNF_PVR_EVENT_BUTT}
};

HI_U8* PVR_GetEventTypeStringByID(HI_UNF_PVR_EVENT_E eEventID);

/******************************* API declaration *****************************/
#if 1
HI_VOID *SearchFileTsSendThread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_S32            Readlen;
    HI_S32            Ret;
    HI_HANDLE         g_TsBuf;
    TS_SEND_ARGS_S    *pstPara = (TS_SEND_ARGS_S *)args;

    Ret = HI_UNF_DMX_AttachTSPort(pstPara->u32DmxId, (HI_UNF_DMX_PORT_E)pstPara->u32PortId);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call VoInit failed.\n");
        return HI_NULL;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer((HI_UNF_DMX_PORT_E)pstPara->u32PortId, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        return HI_NULL;
    }

    while (!g_bStopTsThread)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188*50, &StreamBuf, 0);
        if (Ret != HI_SUCCESS )
        {
            usleep(10 * 1000) ;
            continue;
        }

        Readlen = (HI_S32)fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*50, pstPara->pTsFile);
        if(Readlen <= 0)
        {
            HI_DEBUG_ADVCA("read ts file error!\n");
            rewind(pstPara->pTsFile);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, (HI_U32)Readlen);
        if (Ret != HI_SUCCESS )
        {
            HI_DEBUG_ADVCA("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
    }

    Ret = HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_DestroyTSBuffer failed.\n");
    }
    (HI_VOID)HI_UNF_DMX_DetachTSPort(pstPara->u32DmxId);

    return HI_NULL;
}

static HI_S32 PVR_SearchFile(HI_U32 u32DmxId, HI_U32 u32PortId, const HI_CHAR *pszFileName, PMT_COMPACT_TBL **ppProgTbl)
{
    HI_S32 Ret;
    pthread_t   TsThd;
    TS_SEND_ARGS_S    stPara;
    FILE *pTsFile;

    pTsFile = fopen(pszFileName, "rb");
    if (HI_NULL == pTsFile)
    {
        HI_DEBUG_ADVCA("Open file failed.\n");
        return HI_FAILURE;
    }

    stPara.u32DmxId = u32DmxId;
    stPara.u32PortId = u32PortId;
    stPara.pTsFile = (FILE *)pTsFile;

    g_bStopTsThread = HI_FALSE;
    (HI_VOID)pthread_create(&TsThd, HI_NULL, SearchFileTsSendThread, &stPara);

    (HI_VOID)sleep(1);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(u32DmxId, ppProgTbl);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HIADP_Search_GetAllPmt failed.\n");
    }
    HIADP_Search_DeInit();

    g_bStopTsThread =HI_TRUE;
    (HI_VOID)pthread_join(TsThd, HI_NULL);
    fclose(pTsFile);

    return Ret;
}

#endif
#if 0
static HI_S32 PVR_RecAddPid(HI_U32 u32DmxId, HI_U32 pid, HI_UNF_DMX_CHAN_TYPE_E chnType)
{
    HI_S32 ret      = HI_SUCCESS;
    HI_S32 chnIdx   = 0;
    HI_HANDLE       hPidChn;

    HI_UNF_DMX_CHAN_ATTR_S stChnAttr;

    if(u32DmxId >= DMX_CNT)
    {
        PRINT_SMP("IN PVR_RecAddPid the u32DmxId:%d is over than DMX_CNT:(%d)!!!\n", u32DmxId, DMX_CNT);
        return HI_FAILURE;
    }

    chnIdx   = g_s32PvrRecChnNum[u32DmxId];

    //HI_MPI_DMX_SetRecodeType(u32DmxId, 1); /* 1== DESCRAM TS */

    ret = HI_UNF_DMX_GetChannelHandle(u32DmxId, pid, &hPidChn);
    if (HI_SUCCESS == ret)
    {
        SAMPLE_RUN(HI_UNF_DMX_GetChannelAttr(hPidChn, &stChnAttr), ret);
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        if (stChnAttr.enOutputMode != HI_UNF_DMX_CHAN_OUTPUT_MODE_REC)
        {
            PRINT_SMP("Pid %d already open for PLAY, can NOT recorde\n", pid);
            return ret;
        }
    }


    /*demux get attribution of default channel *//*CNcomment:demux获取通道默认属性*/
    SAMPLE_RUN(HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    stChnAttr.enChannelType = chnType;
    //stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
    stChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    stChnAttr.u32BufSize   = 32;
    stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;


    /*demux create channel *//*CNcomment: demux新建通道*/
    SAMPLE_RUN(HI_UNF_DMX_CreateChannel(u32DmxId, &stChnAttr, &hPidChn), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /*demux set channel PID*//*CNcomment:demux 设置通道PID*/
    SAMPLE_RUN(HI_UNF_DMX_SetChannelPID(hPidChn, pid), ret);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(hPidChn);
        return ret;
    }

    /*demux open channel*//*CNcomment:demux打开通道*/
    SAMPLE_RUN(HI_UNF_DMX_OpenChannel(hPidChn), ret);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_DMX_DestroyChannel(hPidChn);
        return ret;
    }

    chnIdx %= 8; //by g_hPvrRecChns definition, the max column index should be less than 8

    g_hPvrRecChns[u32DmxId][chnIdx] = hPidChn;
    g_s32PvrRecChnNum[u32DmxId]++;

    HI_DEBUG_ADVCA("DMX:%d add PID OK, pid:%d/%#x, Type:%d\n", u32DmxId, pid, pid, chnType);

    return HI_SUCCESS;
}
#endif
static HI_S32 PVR_RecDelAllPid(HI_U32 u32DmxId)
{
    HI_S32 i;
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hPidChn;

    for (i = 0; i < g_s32PvrRecChnNum[u32DmxId]; i++)
    {
        hPidChn = g_hPvrRecChns[u32DmxId][i];
        SAMPLE_RUN(HI_UNF_DMX_CloseChannel(hPidChn),ret);
        SAMPLE_RUN(HI_UNF_DMX_DestroyChannel(hPidChn), ret);
        HI_DEBUG_ADVCA("DMX_DestroyChannel: %d.\n", hPidChn);
    }

    g_s32PvrRecChnNum[u32DmxId]= 0;

    return ret;
}

HI_S32 PVR_SavePorgInfo(PVR_PROG_INFO_S *pstProgInfo, HI_CHAR *pszPvrRecFile)
{
    HI_S32 ret;
    PVR_PROG_INFO_S userData;

    SAMPLE_CheckNullPTR(pstProgInfo);

    memcpy(&userData, pstProgInfo, sizeof(PVR_PROG_INFO_S));
    userData.u32MagicNumber = PVR_PROG_INFO_MAGIC;

    ret = HI_UNF_PVR_SetUsrDataInfoByFileName(pszPvrRecFile, (HI_U8*)&userData, sizeof(PVR_PROG_INFO_S));
    if (HI_SUCCESS != ret)
    {
        HI_DEBUG_ADVCA("PVR_SetUsrDataInfoByFileName ERR:%#x.\n", ret);
        return ret;
    }

    usleep(10*1000);
    HI_DEBUG_ADVCA("\n------------------\n");
    HI_DEBUG_ADVCA("File Info:\n");
    HI_DEBUG_ADVCA("Pid:  A=%d/%#x, V=%d/%#x.\n",pstProgInfo->stProgInfo.AElementPid,
                                         pstProgInfo->stProgInfo.AElementPid,
                                         pstProgInfo->stProgInfo.VElementPid,
                                         pstProgInfo->stProgInfo.VElementPid);
    HI_DEBUG_ADVCA("Type: A=%d, V=%d.\n", pstProgInfo->stProgInfo.AudioType, pstProgInfo->stProgInfo.VideoType);
    HI_DEBUG_ADVCA("isClearStream: %d, isEncrypt: %d.\n", pstProgInfo->stRecAttr.bIsClearStream, pstProgInfo->stRecAttr.stEncryptCfg.bDoCipher);
    HI_DEBUG_ADVCA("indexType: %d, bRewind: %d. maxSize: 0x%llx\n", pstProgInfo->stRecAttr.enIndexType, pstProgInfo->stRecAttr.bRewind, pstProgInfo->stRecAttr.u64MaxFileSize);
    HI_DEBUG_ADVCA("------------------\n\n");
    usleep(10*1000);


    return HI_SUCCESS;
}

HI_S32 PVR_GetPorgInfo(PVR_PROG_INFO_S *pstProgInfo, const HI_CHAR *pszPvrRecFile)
{
    HI_S32 ret;
    HI_U32 dataRead;

    PVR_PROG_INFO_S userData;

    SAMPLE_CheckNullPTR(pstProgInfo);
    SAMPLE_CheckNullPTR(pszPvrRecFile);

    ret = HI_UNF_PVR_GetUsrDataInfoByFileName(pszPvrRecFile, (HI_U8*)&userData,
                                sizeof(PVR_PROG_INFO_S), &dataRead);
    if (HI_SUCCESS != ret)
    {
        HI_DEBUG_ADVCA("PVR_SetUsrDataInfoByFileName ERR:%#x.\n", ret);
        return ret;
    }

    if (PVR_PROG_INFO_MAGIC == userData.u32MagicNumber)
    {
        memcpy(pstProgInfo, &(userData),  sizeof(PVR_PROG_INFO_S));
    }
    else
    {
        //HI_U32 temp1 = 0;
        HI_DEBUG_ADVCA("Can  only play the program record by sample. \n");

        return HI_FAILURE;
    }

    usleep(10*1000);
    HI_DEBUG_ADVCA("\n------------------\n");
    HI_DEBUG_ADVCA("File Info:\n");
    if (pstProgInfo->stProgInfo.AElementNum > 0)
    {
        HI_DEBUG_ADVCA("Audio:\n");
        HI_DEBUG_ADVCA("   PID = %#x\n",pstProgInfo->stProgInfo.AElementPid);
        HI_DEBUG_ADVCA("   Type= %d\n", pstProgInfo->stProgInfo.AudioType);
    }
    else
    {
        HI_DEBUG_ADVCA("Audio: none\n");
    }
    if (pstProgInfo->stProgInfo.VElementNum > 0)
    {
        HI_DEBUG_ADVCA("Video:\n");
        HI_DEBUG_ADVCA("   PID = %#x\n",pstProgInfo->stProgInfo.VElementPid);
        HI_DEBUG_ADVCA("   Type= %d\n", pstProgInfo->stProgInfo.VideoType);
    }
    else
    {
        HI_DEBUG_ADVCA("Video: none\n\n");
    }

    HI_DEBUG_ADVCA("isClearStream: %d, isEncrypt: %d.\n", pstProgInfo->stRecAttr.bIsClearStream, pstProgInfo->stRecAttr.stEncryptCfg.bDoCipher);
    HI_DEBUG_ADVCA("indexType: %d, bRewind: %d. maxSize: 0x%llx\n", pstProgInfo->stRecAttr.enIndexType, pstProgInfo->stRecAttr.bRewind, pstProgInfo->stRecAttr.u64MaxFileSize);
    HI_DEBUG_ADVCA("------------------\n");
    usleep(10*1000);

    return HI_SUCCESS;
}

HI_S32 PVR_checkIdx(char *pfileName)
{
#if 0
    int  i;
    PVR_INDEX_ENTRY_S entry;
    HI_U64 lastOffset = 0;
    HI_U32 sizeCalc;
    HI_U32 scbuf[10];
    HI_U32 LastSize=0;
#endif
    int ret = 0;
    int pos;
    int readNum;
    PVR_IDX_HEADER_INFO_S headInfo;
    FILE *fpIndex;
    int fileTs;

    char indexName[256];
    (HI_VOID)snprintf(indexName, sizeof(indexName), "%s.idx", pfileName);

    fpIndex = fopen(indexName, "rb");
    if (NULL == fpIndex)
    {
        HI_DEBUG_ADVCA("can't open file %s to read!\n", indexName);
        return 2;
    }
//    fileTs = open(pfileName, O_RDONLY | O_LARGEFILE);
    fileTs = open(pfileName, O_RDONLY);
    if (fileTs == -1)
    {
        HI_DEBUG_ADVCA("can't open ts file %s to read!\n", indexName);
        fclose(fpIndex);
        return 2;
    }
    fseek(fpIndex, 0, SEEK_END);
    pos = ftell(fpIndex);
    rewind(fpIndex);

    readNum = (int)fread(&headInfo, 1, sizeof(PVR_IDX_HEADER_INFO_S), fpIndex);
    if (readNum != (int)sizeof(PVR_IDX_HEADER_INFO_S))
    {
            perror("read failed:");
            HI_DEBUG_ADVCA("read head failed: want=%zu, get=%d\n", sizeof(PVR_IDX_HEADER_INFO_S), readNum);
            return 3;
    }
    else
    {
        if (headInfo.u32StartCode == 0x5A5A5A5A)
        {
            HI_DEBUG_ADVCA("This index file has head info: head size=%u, fileSize=%llu\n", headInfo.u32HeaderLen, headInfo.u64ValidSize);
            HI_DEBUG_ADVCA("IndexEntrySize: %zu, index file size:%d, headifo len:%d\n",
                     sizeof(PVR_INDEX_ENTRY_S), pos, headInfo.u32HeaderLen);
            fseek(fpIndex, (long)headInfo.u32HeaderLen, SEEK_SET);
        }
        else
        {
            HI_DEBUG_ADVCA("This index file has NO head info\n");
            return 3;
        }
    }
    HI_DEBUG_ADVCA("\nframe info:\n");
    HI_DEBUG_ADVCA("====frame start:%d\n", headInfo.stCycInfo.u32StartFrame);
    HI_DEBUG_ADVCA("====frame end:  %d\n", headInfo.stCycInfo.u32EndFrame);
    HI_DEBUG_ADVCA("====frame last: %d\n", headInfo.stCycInfo.u32LastFrame);

#if 0
    if (headInfo.stCycInfo.u32StartFrame > headInfo.stCycInfo.u32EndFrame)
    {
        fseek(fpIndex, (headInfo.stCycInfo.u32StartFrame * sizeof(PVR_INDEX_ENTRY_S) + headInfo.u32HeaderLen), SEEK_SET);

        for (i = headInfo.stCycInfo.u32StartFrame; i <= headInfo.stCycInfo.u32LastFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                HI_DEBUG_ADVCA("read failed: i =%u offset:%d want%d, get:%d\n", i, (headInfo.stCycInfo.u32StartFrame + i) * sizeof(PVR_INDEX_ENTRY_S), sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    HI_DEBUG_ADVCA("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                    //return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        HI_DEBUG_ADVCA("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset,entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                    //    HI_DEBUG_ADVCA("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }

                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if ((sizeCalc != LastSize) && (0 != LastSize))
                {
                    HI_DEBUG_ADVCA("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset, lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }

        fseek(fpIndex, headInfo.u32HeaderLen, SEEK_SET);
        for (i = 0; i < headInfo.stCycInfo.u32EndFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                HI_DEBUG_ADVCA("read failed: want%d, get:%d\n", sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    HI_DEBUG_ADVCA("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                //    return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        HI_DEBUG_ADVCA("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset, entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                    //    HI_DEBUG_ADVCA("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }

                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if (sizeCalc != LastSize)
                {
                    //HI_DEBUG_ADVCA("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset , lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }

    }
    else
    {
        for (i = headInfo.stCycInfo.u32StartFrame; i < headInfo.stCycInfo.u32EndFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                HI_DEBUG_ADVCA("read failed: want%d, get:%d\n", sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    HI_DEBUG_ADVCA("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                //    return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        HI_DEBUG_ADVCA("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset, entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                //        HI_DEBUG_ADVCA("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }
                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if (sizeCalc != LastSize)
                {
                    //HI_DEBUG_ADVCA("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset, lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }
    }
#endif

    close(fileTs);
    fclose(fpIndex);

    HI_DEBUG_ADVCA("\n------------End of file. index check ok-------\n");

    return ret;
}


extern HI_VOID PVR_Index_GetIdxFileName(HI_CHAR* pIdxFileName, HI_CHAR* pSrcFileName);


HI_U32  g_u32RecChnID = 0;
HI_CHAR    g_CAPrivateFileName[PVR_MAX_FILENAME_LEN] = {0};

typedef enum
{
   EMI_COPY_CTRL_FREEE = 0x0,    /*copy freely*/
   EMI_COPY_CTRL_NO_MORE = 0x1,  /*copy no more (the value will never transmitted in ECM)*/
   EMI_COPY_CTRL_ONCE= 0x2,      /*copy once*/
   EMI_COPY_CTRL_NEVER= 0x3      /*copy never*/
}CX_URI_EMI_E;

HI_U8 RawURI[2] = {0x20,0x00}; /*This is default value only for sample*/

HI_S32 CreateTestURI()
{
    HI_U32 u32DisplayOffsetTime = 0;
    HI_UNF_PVR_URI_S stPVRURI = {0};
    HI_UNF_PVR_REC_STATUS_S stPVRRecStatus;

    memset(&stPVRRecStatus, 0, sizeof(HI_UNF_PVR_REC_STATUS_S));
    (HI_VOID)HI_UNF_PVR_RecGetStatus(g_u32RecChnID, &stPVRRecStatus);
    u32DisplayOffsetTime =  (stPVRRecStatus.u32CurTimeInMs - stPVRRecStatus.u32StartTimeInMs)/1000;
    stPVRURI.u32OffsetTime =u32DisplayOffsetTime;
    memcpy(stPVRURI.Reserved, "URI", 3);
    memcpy((stPVRURI.Reserved + 3), RawURI, 2);
    if(((RawURI[0] >> 4) & 0x03) == EMI_COPY_CTRL_ONCE)
    {
       stPVRURI.Reserved[3] = (stPVRURI.Reserved[3] & 0xCF)|(EMI_COPY_CTRL_NO_MORE<<4);
    }

    (HI_VOID)HI_UNF_ADVCA_PVR_SaveURI( g_u32RecChnID,g_CAPrivateFileName,&stPVRURI);
    return 0;
}


HI_S32 CreateTestFP()
{
    HI_U32 u32DisplayOffsetTime = 0;
    HI_UNF_PVR_FP_INFO_S stPVRFPInfo = {0};

    HI_UNF_PVR_REC_STATUS_S stPVRRecStatus;

    memset(&stPVRRecStatus, 0, sizeof(HI_UNF_PVR_REC_STATUS_S));
    (HI_VOID)HI_UNF_PVR_RecGetStatus(g_u32RecChnID, &stPVRRecStatus);
    u32DisplayOffsetTime =  (stPVRRecStatus.u32CurTimeInMs - stPVRRecStatus.u32StartTimeInMs)/1000;

    stPVRFPInfo.stFP.stCentiTime.u8Centisecond = 127;
    stPVRFPInfo.stFP.stCentiTime.u8Minute = 63;
    stPVRFPInfo.stFP.stCentiTime.u8Second = 63;
    stPVRFPInfo.stFP.stCentiTime.u8Hour = 31;
    stPVRFPInfo.stFP.u16Position_X = 300;
    stPVRFPInfo.stFP.u16Position_Y = 300;
    stPVRFPInfo.stFP.u8Height = 60;
    stPVRFPInfo.stFPID.u16DataLength = 255;
    stPVRFPInfo.stFP.u16Duration = 2500;
    memcpy(stPVRFPInfo.stFPID.u8ID, "test",4);
    stPVRFPInfo.bisFromLPData = HI_TRUE;
    stPVRFPInfo.u32DisplayOffsetTime = u32DisplayOffsetTime;

    (HI_VOID)HI_UNF_ADVCA_PVR_SaveFP( g_u32RecChnID,g_CAPrivateFileName,&stPVRFPInfo);
    return 0;
}

HI_S32 PVR_RecStart(char *path, PMT_COMPACT_PROG *pstProgInfo, HI_U32 u32DemuxID,
            HI_BOOL bRewind, HI_BOOL bDoCipher, HI_U64 maxSize, HI_U32 *pRecChn,HI_U32 cycle)
{
    HI_U32 recChn;
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_PVR_REC_ATTR_S   attr;
    HI_U32                  VidPid = 0;
    HI_U32                  AudPid = 0;
    PVR_PROG_INFO_S         fileInfo;
    HI_CHAR                 szFileName[PVR_MAX_FILENAME_LEN];
    HI_CHAR                 szIndexFileName[PVR_MAX_FILENAME_LEN] = {0};
    HI_CHAR                 CAPrivateFileName[PVR_MAX_FILENAME_LEN] = {0};
    time_t                  time_now;
    struct tm             * pSystime = HI_NULL;
    HI_UNF_ADVCA_Time_S     CurTime;

    attr.u32DemuxID    = u32DemuxID;

    if (pstProgInfo->AElementNum > 0)
    {
        AudPid  = pstProgInfo->AElementPid;
    }

    if (pstProgInfo->VElementNum > 0 )
    {
        VidPid = pstProgInfo->VElementPid;
        attr.u32IndexPid   = VidPid;
        attr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_VIDEO;
        attr.enIndexVidType = (HI_UNF_VCODEC_TYPE_E)pstProgInfo->VideoType;
    }
    else
    {
        attr.u32IndexPid   = AudPid;
        attr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_AUDIO;
        attr.enIndexVidType = HI_UNF_VCODEC_TYPE_BUTT;
    }
    (HI_VOID)snprintf(szFileName, sizeof(szFileName), "rec_v%d_a%d_%d.ts",
                        pstProgInfo->VElementPid,
                        pstProgInfo->AElementPid,
                        cycle);

    (HI_VOID)snprintf(attr.szFileName, sizeof(attr.szFileName), "%s/", path);

    strncat(attr.szFileName, szFileName, strlen(szFileName));
    attr.u32FileNameLen = strlen(attr.szFileName);
    attr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
    attr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
    attr.enStreamType  = HI_UNF_PVR_STREAM_TYPE_TS;
    attr.bRewind = bRewind;
    attr.u64MaxFileSize= maxSize;//source;
    attr.u64MaxTimeInMs= 0;

    attr.bIsClearStream = HI_TRUE;
    attr.u32UsrDataInfoSize = sizeof(PVR_PROG_INFO_S) + 100;/*the one in index file is a multipleit of 40 bytes*//*CNcomment:索引文件里是40个字节对齐的*/

    attr.stEncryptCfg.bDoCipher = bDoCipher;
    attr.stEncryptCfg.enType = HI_UNF_CIPHER_ALG_AES;
    attr.stEncryptCfg.u32KeyLen = 16;          /*strlen(PVR_CIPHER_KEY)*/
    SAMPLE_RUN(HI_UNF_PVR_RecCreateChn(&recChn, &attr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    SAMPLE_RUN(HI_UNF_PVR_RecAddPID(recChn, 0), ret);
    SAMPLE_RUN(HI_UNF_PVR_RecAddPID(recChn, pstProgInfo->PmtPid), ret);

    if (pstProgInfo->AElementNum > 0)
    {
        SAMPLE_RUN(HI_UNF_PVR_RecAddPID(recChn, AudPid), ret);
    }

    if (pstProgInfo->VElementNum > 0)
    {
        SAMPLE_RUN(HI_UNF_PVR_RecAddPID(recChn, VidPid), ret);
    }

    SAMPLE_RUN(HI_UNF_PVR_RecStartChn(recChn), ret);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_PVR_RecDestroyChn(recChn);
        return ret;
    }

    //===================for ADVCA========================
    g_u32RecChnID = recChn;
    SAMPLE_RUN(HI_UNF_ADVCA_PVR_RecOpen(recChn),ret);
    PVR_Index_GetIdxFileName(szIndexFileName,attr.szFileName);
    SAMPLE_RUN(HI_UNF_ADVCA_PVR_GetCAPrivateFileName(szIndexFileName,CAPrivateFileName), ret);

    memset(&CurTime, 0, sizeof(CurTime));
    //time_t time(time_t *t);
    // struct tm *gmtime(const time_t *timep);  since 00:00:00 on January 1, 1970
    time_now = time(NULL);
    pSystime = gmtime(&time_now);
    if(pSystime != HI_NULL)
    {
        CurTime.s32Year = pSystime->tm_year  + 1900;
        CurTime.s32Mon  = pSystime->tm_mon   + 1;
        CurTime.s32Mday = pSystime->tm_mday;
        CurTime.s32Hour = pSystime->tm_hour;
        CurTime.s32Min  = pSystime->tm_min;
        CurTime.s32Sec  = pSystime->tm_sec;
    }
    if (HI_SUCCESS != HI_UNF_ADVCA_PVR_CreateCAPrivateFile(recChn,CAPrivateFileName, &CurTime))
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_PVR_CreateCAPrivateFile faild\n");
        (HI_VOID)PVR_RecStop(recChn);
        return HI_FAILURE;
    }

    /*Shall first call HI_UNF_ADVCA_PVR_RecOpen, then call HI_UNF_PVR_RegisterExtraCallback*/
    SAMPLE_RUN(HI_UNF_PVR_RegisterExtraCallback(recChn, HI_UNF_PVR_EXTRA_WRITE_CALLBACK, (ExtraCallBack)WriteCallBack, NULL),ret);

    memset(g_CAPrivateFileName,0x00,PVR_MAX_FILENAME_LEN);
    memcpy(g_CAPrivateFileName,CAPrivateFileName,strlen(CAPrivateFileName));

    memcpy(&(fileInfo.stProgInfo), pstProgInfo, sizeof(fileInfo.stProgInfo));
    memcpy(&(fileInfo.stRecAttr), &attr, sizeof(fileInfo.stRecAttr));

    SAMPLE_RUN(PVR_SavePorgInfo(&fileInfo, attr.szFileName), ret);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_PVR_RecStopChn(recChn);
        (HI_VOID)HI_UNF_PVR_RecDestroyChn(recChn);
        return ret;
    }

    *pRecChn = recChn;

    return HI_SUCCESS;
}

HI_S32 PVR_RecStop(HI_U32 u32RecChnID)
{
    HI_S32 ret;
    HI_S32 ret2;
    HI_UNF_PVR_REC_ATTR_S recAttr;

    SAMPLE_RUN(HI_UNF_PVR_RecGetChn(u32RecChnID, &recAttr), ret2) ;
    SAMPLE_RUN(HI_UNF_PVR_RecStopChn(u32RecChnID), ret);
    if (HI_SUCCESS != HI_UNF_PVR_UnRegisterExtraCallBack(u32RecChnID, HI_UNF_PVR_EXTRA_WRITE_CALLBACK))
    {
        HI_DEBUG_ADVCA("call HI_UNF_PVR_UnRegisterExtraCallBack failed\n");
    }
    printf("PVR_RecStop: stop HI_UNF_ADVCA_PVR_RecClose--->\n");
    SAMPLE_RUN(HI_UNF_ADVCA_PVR_RecClose(u32RecChnID),ret);
    SAMPLE_RUN(HI_UNF_PVR_RecDestroyChn(u32RecChnID), ret);
    if (HI_SUCCESS == ret2)
    {
        if (!recAttr.stEncryptCfg.bDoCipher)
        {
            //SAMPLE_RUN(PVR_checkIdx(recAttr.szFileName), ret);
        }
    }
    (HI_VOID)PVR_RecDelAllPid(PVR_DMX_ID_REC);

    return ret;
}


HI_S32 PVR_SwitchDmxSource(HI_U32 dmxId, HI_U32 protId)
{
    HI_S32 ret;

    (HI_VOID)HI_UNF_DMX_DetachTSPort(dmxId);

    SAMPLE_RUN(HI_UNF_DMX_AttachTSPort(dmxId, (HI_UNF_DMX_PORT_E)protId), ret);

    return ret;
}

HI_S32 PVR_SetAvplayPidAndCodecType(HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    //HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    HI_U32                  VidPid;
    HI_U32                  AudPid;

    HI_UNF_VCODEC_ATTR_S        VdecAttr;
    HI_UNF_ACODEC_ATTR_S        AdecAttr;
//    HI_U32                  i;
    HI_S32                  Ret;
    HI_UNF_VCODEC_TYPE_E    enVidType;
    HI_U32                  u32AudType;

    /*
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Stop failed.\n");
        //return Ret;
    }
    */
    (HI_VOID)HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    (HI_VOID)HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr);

    if (pProgInfo->VElementNum > 0 )
    {
        VidPid = pProgInfo->VElementPid;
        enVidType = (HI_UNF_VCODEC_TYPE_E)pProgInfo->VideoType;
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

    if (VidPid != INVALID_TSPID)
    {
        VdecAttr.enType = enVidType;
        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_SetAttr failed.\n");
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&VidPid);
        if (Ret != HI_SUCCESS)
        {
            HI_DEBUG_ADVCA("call HIADP_AVPlay_SetVdecAttr failed.\n");
            return Ret;
        }
    }

    if (AudPid != INVALID_TSPID)
    {
        Ret = HIADP_AVPlay_SetAdecAttr(hAvplay,u32AudType,HD_DEC_MODE_RAWPCM,1);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&AudPid);
        if (HI_SUCCESS != Ret)
        {
            HI_DEBUG_ADVCA("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}



/*set audio and video PID attribution,start to play*//*CNcomment:设置音视频PID属性,开始播放*/
HI_S32 PVR_StartLivePlay(HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    HI_S32 ret = HI_SUCCESS;

    (HI_VOID)PVR_SetAvplayPidAndCodecType(hAvplay, pProgInfo);

    /*start to play audio and video*//*CNcomment:开始音视频播放*/
    /*lint -save -e655 */
    SAMPLE_RUN(HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID , NULL), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 PVR_StopLivePlay(HI_HANDLE hAvplay)
{
    HI_UNF_AVPLAY_STOP_OPT_S option;

    option.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    option.u32TimeoutMs = 0;

    HI_DEBUG_ADVCA("stop live play ...\n");

    /*stop playing audio and video*//*CNcomment:停止音视频设备*/
    return HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &option);
}


/*start playback*//*CNcomment:开始回放*/
HI_S32 PVR_StartPlayBack(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn, HI_HANDLE hAvplay, HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo)
{
    HI_S32 ret;
    HI_U32 playChn;
    PVR_PROG_INFO_S  fileInfo;
    HI_UNF_PVR_PLAY_ATTR_S attr;
    PMT_COMPACT_TBL  *pPmttal;
    HI_CHAR  szIndexFileName[PVR_MAX_FILENAME_LEN] = {0};
    HI_CHAR  CAPrivateFileName[128] = {0};
    HI_U32  u32URINum = 0;
    HI_U32  u32FPNum = 0;
    HI_U32  pid = 0;
    HI_UNF_AVPLAY_MEDIA_CHAN_E enMediaType = 0;
    HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
    HI_UNF_ADVCA_Time_S     CurTime;;

    SAMPLE_CheckNullPTR(pszFileName);
    SAMPLE_CheckNullPTR(pu32PlayChn);

    SAMPLE_RUN(PVR_GetPorgInfo(&fileInfo, pszFileName), ret);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("Can NOT get prog INFO, can't play.\n");
        (HI_VOID)PVR_SearchFile(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK, pszFileName, &pPmttal);
        (HI_VOID)PVR_SetAvplayPidAndCodecType(hAvplay, &(pPmttal->proginfo[0]));

        memcpy(attr.szFileName, pszFileName, strlen(pszFileName) + 1);
        attr.u32FileNameLen = strlen(pszFileName);
        attr.enStreamType = HI_UNF_PVR_STREAM_TYPE_TS;
        attr.bIsClearStream = HI_TRUE;
    }
    else
    {
        (HI_VOID)PVR_SetAvplayPidAndCodecType(hAvplay, &(fileInfo.stProgInfo));

        memcpy(attr.szFileName, pszFileName, strlen(pszFileName) + 1);
        attr.u32FileNameLen = strlen(pszFileName);
        attr.enStreamType = fileInfo.stRecAttr.enStreamType;
        attr.bIsClearStream = fileInfo.stRecAttr.bIsClearStream;
    }

    if (1 == fileInfo.stRecAttr.stEncryptCfg.bDoCipher&&ret==HI_SUCCESS)
    {
        memcpy(&(attr.stDecryptCfg), &(fileInfo.stRecAttr.stEncryptCfg), sizeof(HI_UNF_PVR_CIPHER_S));

        HI_DEBUG_ADVCA("cipher info:\n");
        HI_DEBUG_ADVCA("  enType:%d\n", attr.stDecryptCfg.enType);
    }
    else
    {
        HI_DEBUG_ADVCA("cipher info: not encrypt\n");
        attr.stDecryptCfg.bDoCipher = HI_FALSE;
    }

    ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK);
    if (ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_DMX_AttachTSPort failed.\n");
        (HI_VOID)HI_UNF_DMX_DeInit();
        return ret;
    }

//    SAMPLE_RUN(HI_UNF_DMX_CreateTSBuffer(PVR_DMX_PORT_ID_PLAYBACK, 8*1024*1024, &g_hTsBufForPlayBack), ret);
    SAMPLE_RUN(HI_UNF_DMX_CreateTSBuffer(PVR_DMX_PORT_ID_PLAYBACK, 768*1024, &g_hTsBufForPlayBack), ret);
    if (ret != HI_SUCCESS)
    {
        (HI_VOID)HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
        (HI_VOID)HI_UNF_DMX_DeInit();
        return ret;
    }
    /*create new play channel*//*CNcomment:申请新的播放通道*/
    SAMPLE_RUN(HI_UNF_PVR_PlayCreateChn(&playChn, &attr, hAvplay, g_hTsBufForPlayBack), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &pid);
    if ((HI_SUCCESS != ret) || (0x1fff == pid))
    {
        printf("has no audio stream!\n");
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_AUD;
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &pid);
    if ((HI_SUCCESS != ret) || (0x1fff == pid))
    {
        printf("has no video stream!\n");
    }
    else
    {
        enMediaType |= HI_UNF_AVPLAY_MEDIA_CHAN_VID;
    }

    /*enable avplay A/V sync*//*CNcomment:使能avplay音视频同步*/
    if (HI_SUCCESS != HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
    {
        printf("get avplay sync attr fail!\n");
        return HI_FAILURE;
    }
    stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    stSyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    stSyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    stSyncAttr.u32PreSyncTimeoutMs = 1000;
    stSyncAttr.bQuickOutput = HI_FALSE;

    if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
    {
        printf("set avplay sync attr fail!\n");
        return HI_FAILURE;
    }

    SAMPLE_RUN(HI_UNF_PVR_PlayStartChn(playChn), ret);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_UNF_PVR_PlayDestroyChn(playChn);
        return ret;
    }

    //===================for ADVCA========================
    SAMPLE_RUN(HI_UNF_ADVCA_PVR_PlayOpen(playChn, stPvrPlayInfo),ret);

    PVR_Index_GetIdxFileName(szIndexFileName,(HI_CHAR*)pszFileName);
    HI_DEBUG_ADVCA("szIndexFileName is %s \n", szIndexFileName);
    SAMPLE_RUN(HI_UNF_ADVCA_PVR_GetCAPrivateFileName(szIndexFileName,CAPrivateFileName),ret);
    HI_DEBUG_ADVCA("in CA private file %s, u32URINum = %d, u32FPNum = %d\n", CAPrivateFileName,u32URINum,u32FPNum);
    if (HI_SUCCESS != HI_UNF_ADVCA_PVR_CheckCAPrivateFileMAC(CAPrivateFileName, &CurTime))
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_PVR_CheckCAPrivateFileMAC faild\n");
        return HI_FAILURE;
    }

    SAMPLE_RUN(HI_UNF_PVR_RegisterExtraCallback(playChn, HI_UNF_PVR_EXTRA_READ_CALLBACK, ReadCallBack, NULL),ret);

    (HI_VOID)HI_UNF_ADVCA_PVR_GetURIAndFPNum( CAPrivateFileName,&u32URINum,&u32FPNum);

    *pu32PlayChn = playChn;
    return HI_SUCCESS;
}


/*stop playback*//*CNcomment: 停止回放 */
HI_VOID PVR_StopPlayBack(HI_U32 playChn)
{
    /*lint -save -e550 */
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_AVPLAY_STOP_OPT_S stopOpt;
    HI_UNF_PVR_PLAY_ATTR_S PlayAttr;

    stopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stopOpt.u32TimeoutMs = 0;
    SAMPLE_RUN(HI_UNF_PVR_PlayGetChn(playChn,&PlayAttr), ret);
    SAMPLE_RUN(HI_UNF_PVR_UnRegisterExtraCallBack(playChn, HI_UNF_PVR_EXTRA_READ_CALLBACK),ret);
    (HI_VOID)HI_UNF_PVR_PlayStopChn(playChn, &stopOpt);

    (HI_VOID)HI_UNF_ADVCA_PVR_PlayClose(playChn);
    (HI_VOID)HI_UNF_PVR_PlayDestroyChn(playChn);
    SAMPLE_RUN(HI_UNF_DMX_DestroyTSBuffer(g_hTsBufForPlayBack), ret);
    SAMPLE_RUN(HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE), ret);

    if (ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call PVR_StopPlayBack failed.\n");
    }
}

HI_S32 PVR_AvplayInit(HI_HANDLE hWin, HI_HANDLE *phAvplay, HI_HANDLE* phSoundTrack)
{
    HI_S32                  Ret;
    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
    HI_UNF_AUDIOTRACK_ATTR_S  stTrackAttr;
#ifdef __HI3716MV310__
    HI_UNF_MIXER_ATTR_S stMixerAttr;
    stMixerAttr.u32MixerWeight = 100;
#endif

    Ret = HIADP_AVPlay_RegADecLib();
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HIADP_AVPlay_RegADecLib failed.\n");
        return Ret;
    }


    Ret = HI_UNF_AVPLAY_Init();
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

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_GetAttr failed.\n");
        (HI_VOID)HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 60;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -20;
    SyncAttr.u32PreSyncTimeoutMs = 1000;
    SyncAttr.bQuickOutput = HI_FALSE;

    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_SetAttr failed.\n");
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
    //Ret = HI_UNF_SND_Mixer_Open(HI_UNF_SND_0, phSoundTrack, &stMixerAttr);
    Ret = HI_UNF_SND_CreateTrack(HI_UNF_SND_0,&stTrackAttr, phSoundTrack);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_CreateTrack failed.\n");
        return Ret;
    }
#endif
#ifdef  __HI3716MV310__
    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
#else
    Ret = HI_UNF_SND_Attach(*phSoundTrack, hAvplay);
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

HI_S32  PVR_AvplayDeInit(HI_HANDLE hAvplay, HI_HANDLE hWin, HI_HANDLE hSoundTrack)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_AVPLAY_STOP_OPT_S Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    Ret = HI_UNF_SND_Detach(hSoundTrack, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        HI_DEBUG_ADVCA("call HI_UNF_SND_Detach failed.\n");
    }
#ifdef __HI3716MV310__
    Ret = HI_UNF_SND_Mixer_Close(hSoundTrack);
#else
    Ret = HI_UNF_SND_DestroyTrack(hSoundTrack);
#endif
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_SND_DestroyTrack failed.\n");
    }
    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_VO_SetWindowEnable failed.\n");
    }

    Ret = HI_UNF_VO_DetachWindow(hWin, hAvplay);
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

HI_VOID PVR_CallBack(HI_U32 u32ChnID, HI_UNF_PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args)
{

    HI_DEBUG_ADVCA("==============call back================\n");

    if (EventType > HI_UNF_PVR_EVENT_REC_RESV)
    {
        HI_DEBUG_ADVCA("====callback error!!!\n");
        return;
    }

    HI_DEBUG_ADVCA("====channel     %d\n", u32ChnID);
    HI_DEBUG_ADVCA("====event:%s    %d\n", PVR_GetEventTypeStringByID(EventType), EventType);
    HI_DEBUG_ADVCA("====event value %d\n", s32EventValue);


    if (EventType == HI_UNF_PVR_EVENT_PLAY_EOF)
    {
        HI_DEBUG_ADVCA("==========play to end of file======\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_SOF)
    {
        HI_DEBUG_ADVCA("==========play to start of file======\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_ERROR)
    {
        HI_DEBUG_ADVCA("==========play internal error, check if the disk is insert to the box======\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_REACH_REC)
    {
        HI_DEBUG_ADVCA("==========play reach to record ======\n");
    }

    if (EventType == HI_UNF_PVR_EVENT_REC_DISKFULL)
    {
        HI_DEBUG_ADVCA("\n====disk full,  stop record=====\n\n");

        (HI_VOID)HI_UNF_PVR_RecStopChn(u32ChnID);
        g_bIsRecStop = HI_TRUE;
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_ERROR)
    {
        HI_DEBUG_ADVCA("======disk write error, please check if the disk is insert to the box.====\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_OVER_FIX)
    {
        HI_DEBUG_ADVCA("\n======reach the fixed size.==========\n\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_REACH_PLAY)
    {
        HI_DEBUG_ADVCA("\n======record reach to play.==========\n\n");
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_DISK_SLOW)
    {
        HI_DEBUG_ADVCA("======disk is too slow, the stream record would be error.====\n");
    }

    HI_DEBUG_ADVCA("=======================================\n\n");

    return;
}

HI_S32 PVR_RegisterCallBacks(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_EOF, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_SOF, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_ERROR, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISKFULL, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_OVER_FIX, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISK_SLOW, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_ERROR, PVR_CallBack, NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_DEBUG_ADVCA("call HI_UNF_PVR_RegisterEvent failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}

HI_U8* PVR_GetEventTypeStringByID(HI_UNF_PVR_EVENT_E eEventID)
{
    HI_U32 nNum = sizeof(g_stEventType)/sizeof(g_stEventType[0]);
    HI_U32 nIndex = 0;
    HI_U8* pszRet = HI_NULL;

    for(nIndex=0; nIndex<nNum; nIndex++)
    {
        if( eEventID == g_stEventType[nIndex].eEventID)
        {
            pszRet = g_stEventType[nIndex].szEventTypeName;
            break;
        }
    }

    if(nIndex == nNum)
    {
       pszRet = g_stEventType[nNum-1].szEventTypeName;
    }

    return pszRet;
}




