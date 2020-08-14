/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name           : mpi_vir.c
  Version             : Initial Draft
  Author              : Hisilicon multimedia software group
  Created             : 2012/12/12
  Last Modified       :
  Description         :
  Function List       :
  History             :
  1.Date              : 2012/12/12
    Author            : w00196555
    Modification      : Created file

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "mpi_vir.h"
#include "hi_mpi_mem.h"
#include "hi_error_mpi.h"

#define VIRTRACK_LOCK(Mutex)        (void)pthread_mutex_lock(&Mutex)
#define VIRTRACK_UNLOCK(Mutex)      (void)pthread_mutex_unlock(&Mutex)

typedef struct
{
    pthread_mutex_t  stMutex;               /* Mutex */
    HI_U32           uTrackFlag;
    HI_VOID*         pTrack[AO_MAX_VIRTUAL_TRACK_NUM];
} VIR_TRACK_RS_STATE_S;

static VIR_TRACK_RS_STATE_S g_stVirTrack;

static HI_VOID VIRResetPTSQue(VIR_PTS_QUE_S* pstPTSQue)
{
    if (!pstPTSQue)
    {
        return;
    }

    memset(pstPTSQue->stPTSArry, 0, sizeof(VIR_PTS_S) * VIR_MAX_STORED_PTS_NUM);
    pstPTSQue->u32PTSreadIdx  = 0;
    pstPTSQue->u32PTSwriteIdx = 0;
    pstPTSQue->u32LastPtsMs = (HI_U32) - 1;
    return;
}

static HI_VOID VIRFindPTS(VIR_BUFFUR_S* pstBuf, HI_U32* pu32FoundPts, HI_U32* pu32FoundPos)
{
    HI_U32 u32FoundPos, u32RdPos, u32WtPos, u32FoundPts;
    VIR_PTS_S* pstPTS = HI_NULL;
    VIR_PTS_QUE_S* pstPTSQue;
    HI_U32 u32ReadPtr;

    u32FoundPts = (HI_U32) - 1;
    u32ReadPtr = pstBuf->u32Read;
    pstPTSQue = &(pstBuf->stPTSQue);
    u32RdPos = pstPTSQue->u32PTSreadIdx;
    u32WtPos = pstPTSQue->u32PTSwriteIdx;
    pstPTS = pstPTSQue->stPTSArry;

    for (u32FoundPos = u32RdPos; u32FoundPos != u32WtPos;  u32FoundPos = (u32FoundPos + 1) % VIR_MAX_STORED_PTS_NUM)
    {
        if (pstPTS[u32FoundPos].u32BegPtr < pstPTS[u32FoundPos].u32EndPtr)
        {
            if ((pstPTS[u32FoundPos].u32BegPtr <= u32ReadPtr) && (pstPTS[u32FoundPos].u32EndPtr >= u32ReadPtr))
            {
                u32FoundPts = pstPTS[u32FoundPos].u32PtsMs;
                break;
            }
        }
        else
        {
            if ((pstPTS[u32FoundPos].u32BegPtr <= u32ReadPtr) || (pstPTS[u32FoundPos].u32EndPtr >= u32ReadPtr))
            {
                u32FoundPts = pstPTS[u32FoundPos].u32PtsMs;
                break;
            }
        }
    }

    if (u32FoundPos == u32WtPos)
    {
        u32FoundPos = (HI_U32) - 1;
    }

    *pu32FoundPos = u32FoundPos;
    *pu32FoundPts = u32FoundPts;
    return;
}

static HI_U32 VIRAcquirePTS(VIR_BUFFUR_S* pstBuf, HI_U32 u32PcmSamplesPerFrame,
                            HI_U32 u32PcmSampleRate)
{
    HI_U32 u32PtsMs;
    HI_U32 u32FoundPos, u32FoundPts;
    VIR_PTS_QUE_S* pstPTSQue;

    pstPTSQue = &(pstBuf->stPTSQue);
    VIRFindPTS(pstBuf, &u32FoundPts, &u32FoundPos);

    if (((HI_U32) - 1) == u32FoundPts)
    {
        /*can not find a valid PTS*/
        if (((HI_U32) - 1) != pstPTSQue->u32LastPtsMs)
        {
            HI_U32 u32Delta;
            u32Delta = (u32PcmSamplesPerFrame * 1000) / u32PcmSampleRate;
            u32PtsMs = pstPTSQue->u32LastPtsMs + u32Delta;

            if (((HI_U32) - 1) == u32PtsMs)
            {
                u32PtsMs = 0;
            }
        }
        else
        {
            u32PtsMs = (HI_U32) - 1;
        }
    }
    else
    {
        /* Found a valid PTS */
        u32PtsMs = u32FoundPts;
    }

    return u32PtsMs;
}

static HI_VOID VIRReleasePTS(VIR_BUFFUR_S* pstBuf, HI_U32 u32PtsMs)

{
    HI_U32 u32FoundPos, u32FoundPts;
    VIR_PTS_QUE_S* pstPTSQue;
    VIR_PTS_S* pstPTS;

    pstPTSQue = &(pstBuf->stPTSQue);
    pstPTS = pstPTSQue->stPTSArry;
    pstPTSQue->u32LastPtsMs = u32PtsMs;
    VIRFindPTS(pstBuf, &u32FoundPts, &u32FoundPos);

    if ((HI_U32)(-1) != u32FoundPos)
    {
        pstPTS[u32FoundPos].u32PtsMs = (HI_U32)(-1);
        pstPTSQue->u32PTSreadIdx = (u32FoundPos + 1) % VIR_MAX_STORED_PTS_NUM;
    }

    return;
}

static HI_VOID VIRStorePTS (VIR_BUFFUR_S* pstBuf, HI_U32 u32PtsMs, HI_U32 u32Size)
{
    VIR_PTS_QUE_S* pstPTSQue = &(pstBuf->stPTSQue);
    VIR_PTS_S* pstPTSArray = pstPTSQue->stPTSArry;
    HI_U32 u32CalcEndPtr;

    /* make sure there are space to store */
    if ((pstPTSQue->u32PTSwriteIdx + 1) % VIR_MAX_STORED_PTS_NUM != pstPTSQue->u32PTSreadIdx)
    {
        if ((pstBuf->u32Write + u32Size) < pstBuf->u32End)
        {
            u32CalcEndPtr = pstBuf->u32Write + u32Size;
        }
        else
        {
            u32CalcEndPtr = pstBuf->u32Write + u32Size - (pstBuf->u32End - pstBuf->u32Start);
        }

        pstPTSArray[pstPTSQue->u32PTSwriteIdx].u32PtsMs  = u32PtsMs;
        pstPTSArray[pstPTSQue->u32PTSwriteIdx].u32BegPtr = pstBuf->u32Write;
        pstPTSArray[pstPTSQue->u32PTSwriteIdx].u32EndPtr = u32CalcEndPtr;
        pstPTSQue->u32PTSwriteIdx = (pstPTSQue->u32PTSwriteIdx + 1) % VIR_MAX_STORED_PTS_NUM;
    }
    else
    {
        HI_WARN_AO("Not enough PTS buffer, discard current PTS(%d)\n", u32PtsMs);
    }

    return;
}

HI_S32 VirInitBuf(VIR_BUFFUR_S* pstBuf, HI_U32 u32Size)
{
    if ((u32Size < VIR_MIN_OUTBUF_SIZE) || (u32Size > VIR_MAX_OUTBUF_SIZE))
    {
        HI_ERR_AO(" invalid input buffer size(%d) minsize(%d) maxsize(%d)!\n", u32Size,
                  VIR_MIN_OUTBUF_SIZE, VIR_MAX_OUTBUF_SIZE);
        return HI_FAILURE;
    }

    pstBuf->pu8BufBase = (HI_U8*)HI_MALLOC(HI_ID_AO, u32Size + VIR_MAX_FRAME_SIZE);

    if (HI_NULL_PTR == pstBuf->pu8BufBase)
    {
        HI_FATAL_AO("MALLOC pstBuf error\n");
        return HI_FAILURE;
    }

    pstBuf->u32Start = 0;
    pstBuf->u32End = u32Size;
    pstBuf->u32Read = 0;
    pstBuf->u32Write = 0;
    pstBuf->s32BitPerSample = 16;
    pstBuf->u32SampleRate = (HI_U32)HI_UNF_SAMPLE_RATE_48K;
    pstBuf->u32Channel = 2;
    pstBuf->u32PcmSamplesPerFrame = 1024;

    VIRResetPTSQue(&(pstBuf->stPTSQue));

    return HI_SUCCESS;
}

static HI_VOID VirDeInitBuf(VIR_BUFFUR_S* pstBuf)
{
    VIRResetPTSQue(&(pstBuf->stPTSQue));

    if (pstBuf->pu8BufBase)
    {
        HI_FREE(HI_ID_AO, (HI_VOID*)pstBuf->pu8BufBase);
        pstBuf->pu8BufBase = HI_NULL;
    }

    pstBuf->u32Start = 0;
    pstBuf->u32End = 0;
    pstBuf->u32Read = 0;
    pstBuf->u32Write = 0;
    return;
}

static HI_U32  VIRGetDataSize(VIR_BUFFUR_S* pstBuf)
{
    if (pstBuf->u32Read > pstBuf->u32Write)
    {
        return (pstBuf->u32End - pstBuf->u32Start) - (pstBuf->u32Read - pstBuf->u32Write);
    }
    else
    {
        return pstBuf->u32Write - pstBuf->u32Read;
    }
}

static HI_U32  VIRGetIdleSize(VIR_BUFFUR_S* pstBuf)
{
    if (pstBuf->u32Read > pstBuf->u32Write)
    {
        return (pstBuf->u32Read - pstBuf->u32Write);
    }
    else
    {
        return ((pstBuf->u32End - pstBuf->u32Start) + pstBuf->u32Read) - pstBuf->u32Write;
    }
}

static HI_VOID  VIRFlushBuf(VIR_BUFFUR_S* pstBuf)
{
    pstBuf->u32Write = pstBuf->u32Read;
    VIRResetPTSQue(&(pstBuf->stPTSQue));
    return;
}

static HI_U32 VirGetFreeID(HI_VOID)
{
    HI_U32 i;

    for (i = 0; i < AO_MAX_VIRTUAL_TRACK_NUM; i++)
    {
        if (!(g_stVirTrack.uTrackFlag & ((HI_U32)1L << i)))
        {
            return i;
        }
    }

    return AO_MAX_VIRTUAL_TRACK_NUM;
}

static HI_U32 VIRTrack2VirID(HI_HANDLE hTrack)
{
    return ((hTrack & AO_TRACK_CHNID_MASK) - AO_MAX_REAL_TRACK_NUM);
}

HI_VOID VIR_InitRS(HI_VOID)
{
    HI_U32 i;

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    g_stVirTrack.uTrackFlag = 0;

    for (i = 0; i < AO_MAX_VIRTUAL_TRACK_NUM; i++)
    {
        g_stVirTrack.pTrack[i] = HI_NULL;
    }

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return;
}

HI_VOID VIR_DeInitRS(HI_VOID)
{
    HI_U32 i;

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    g_stVirTrack.uTrackFlag = 0;

    for (i = 0; i < AO_MAX_VIRTUAL_TRACK_NUM; i++)
    {
        g_stVirTrack.pTrack[i] = HI_NULL;
    }

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return;
}

HI_S32 VIR_CreateTrack(const HI_UNF_AUDIOTRACK_ATTR_S* pstTrackAttr, HI_HANDLE* phTrack)
{
    HI_S32 s32Ret;
    VIR_TRACK_STATE_S* pstVir;
    HI_U32 u32VirID;

    CHECK_AO_NULL_PTR(phTrack);
    CHECK_AO_NULL_PTR(pstTrackAttr);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VirGetFreeID();
    if (AO_MAX_VIRTUAL_TRACK_NUM == u32VirID)
    {
        HI_ERR_AO("have not free virtual track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)HI_MALLOC(HI_ID_AO, sizeof(VIR_TRACK_STATE_S));
    if (HI_NULL == pstVir)
    {
        HI_ERR_AO("malloc virtual track fail\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    memset(pstVir, 0, sizeof(VIR_TRACK_STATE_S));
    memcpy(&pstVir->stTrackAttr, pstTrackAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));
    pstVir->u32BufSize = pstTrackAttr->u32OutputBufSize;

    /*output buf*/
    pstVir->pstBuf = (VIR_BUFFUR_S*)HI_MALLOC(HI_ID_AO, sizeof(VIR_BUFFUR_S));
    if (HI_NULL == pstVir->pstBuf)
    {
        HI_FREE(HI_ID_AO, (HI_VOID*)pstVir);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    s32Ret = VirInitBuf(pstVir->pstBuf, pstVir->u32BufSize);
    if (HI_SUCCESS != s32Ret)
    {
        HI_FREE(HI_ID_AO, (HI_VOID*)(pstVir->pstBuf));
        HI_FREE(HI_ID_AO, (HI_VOID*)pstVir);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    g_stVirTrack.uTrackFlag |= (HI_U32)1L << u32VirID;
    g_stVirTrack.pTrack[u32VirID] = (HI_VOID*)pstVir;

    *phTrack = (HI_ID_AO << 16) | (HI_ID_MASTER_SLAVE_TRACK << 8) | (AO_MAX_REAL_TRACK_NUM + u32VirID);

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}

HI_S32 VIR_DestroyTrack(HI_HANDLE hTrack)
{
    VIR_TRACK_STATE_S* pstVir = HI_NULL;
    HI_U32 u32VirID;

    CHECK_Track(hTrack);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VIRTrack2VirID(hTrack);
    if (u32VirID >= AO_MAX_VIRTUAL_TRACK_NUM)
    {
        HI_ERR_AO("Invalid Virtual Track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)g_stVirTrack.pTrack[u32VirID];
    if (pstVir)
    {
        if (pstVir->pstBuf)
        {
            VirDeInitBuf(pstVir->pstBuf);
            HI_FREE(HI_ID_AO, (HI_VOID*)(pstVir->pstBuf));
            pstVir->pstBuf = HI_NULL;
        }

        HI_FREE(HI_ID_AO, (HI_VOID*)(pstVir));
        g_stVirTrack.pTrack[u32VirID] = HI_NULL;
        g_stVirTrack.uTrackFlag &= ~((HI_U32)1L << u32VirID);
    }

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}

HI_S32  VIR_GetAttr(HI_HANDLE hTrack, HI_UNF_AUDIOTRACK_ATTR_S* pstAttr)
{
    VIR_TRACK_STATE_S* pstVir = HI_NULL;
    HI_U32 u32VirID;

    CHECK_Track(hTrack);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VIRTrack2VirID(hTrack);
    if (u32VirID >= AO_MAX_VIRTUAL_TRACK_NUM)
    {
        HI_ERR_AO("Invalid Virtual Track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)g_stVirTrack.pTrack[u32VirID];
    if (HI_NULL == pstVir)
    {
        HI_ERR_AO("virtual track(%d) is null!\n", u32VirID);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    memcpy(pstAttr, &pstVir->stTrackAttr, sizeof(HI_UNF_AUDIOTRACK_ATTR_S));

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}

HI_S32  VIR_SendData(HI_HANDLE hTrack, const HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    HI_U8* pu8Src, *pu8Dst;
    HI_U32 u32Size, u32Idle, u32TailSpace;
    VIR_TRACK_STATE_S* pstVir = HI_NULL;
    HI_U32 u32VirID;
    VIR_BUFFUR_S* pstBuf;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;

    CHECK_Track(hTrack);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VIRTrack2VirID(hTrack);
    if (u32VirID >= AO_MAX_VIRTUAL_TRACK_NUM)
    {
        HI_ERR_AO("Invalid Virtual Track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)g_stVirTrack.pTrack[u32VirID];
    if (HI_NULL == pstVir)
    {
        HI_ERR_AO("virtual track(%d) is null!\n", u32VirID);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstBuf = pstVir->pstBuf;

    memcpy(&stAOFrame, pstAOFrame, sizeof(HI_UNF_AO_FRAMEINFO_S));

    if (0 == stAOFrame.u32PcmSamplesPerFrame)
    {
        HI_WARN_AO("There is no PcmData!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    if (stAOFrame.u32Channels > HI_UNF_I2S_CHNUM_2)
    {
        stAOFrame.u32Channels = HI_UNF_I2S_CHNUM_2;
    }

    if (pstBuf->u32Channel != stAOFrame.u32Channels ||
        pstBuf->s32BitPerSample != stAOFrame.s32BitPerSample ||
        pstBuf->u32SampleRate != stAOFrame.u32SampleRate)
    {
        VIRFlushBuf(pstBuf);   //samplerate or channle or bitpersampler is modified , track buffer will reset.
        pstBuf->u32Channel = stAOFrame.u32Channels;
        pstBuf->s32BitPerSample = stAOFrame.s32BitPerSample;
        pstBuf->u32SampleRate = stAOFrame.u32SampleRate;
    }

    if (HI_UNF_BIT_DEPTH_16 == stAOFrame.s32BitPerSample)
    {
        u32Size  = stAOFrame.u32PcmSamplesPerFrame * stAOFrame.u32Channels * sizeof(HI_S16);
    }
    else
    {
        u32Size  = stAOFrame.u32PcmSamplesPerFrame * stAOFrame.u32Channels * sizeof(HI_S32);
    }

    u32Idle  = VIRGetIdleSize(pstBuf);
    if (u32Idle <= u32Size)
    {
        HI_INFO_AO("Track buf is full, clear buffer");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_ERR_AO_OUT_BUF_FULL;   //verify
        //HIAOFlushVirtualBuf(pstBuf);
        //u32Idle = pstBuf->u32End - pstBuf->u32Start;
    }

    if (pstBuf->u32Write < pstBuf->u32Read)
    {
        u32TailSpace = u32Idle;
    }
    else
    {
        u32TailSpace = pstBuf->u32End - pstBuf->u32Write;
    }

    pu8Src = (HI_U8*)(stAOFrame.ps32PcmBuffer);
    pu8Dst = (HI_U8*)(pstBuf->pu8BufBase + (pstBuf->u32Write - pstBuf->u32Start));

    if (u32Size <= u32TailSpace)
    {
        memcpy((HI_VOID*)pu8Dst, (HI_VOID*)pu8Src, u32Size);
    }
    else
    {
        memcpy((HI_VOID*)pu8Dst, (HI_VOID*)pu8Src, u32TailSpace);

        memcpy((HI_VOID*)pstBuf->pu8BufBase, (HI_VOID*)(pu8Src + u32TailSpace), u32Size - u32TailSpace);
    }

    VIRStorePTS(pstBuf, stAOFrame.u32PtsMs, u32Size);
    /* update WritePtr */
    pstBuf->u32Write += u32Size;

    if (pstBuf->u32Write >= pstBuf->u32End)
    {
        pstBuf->u32Write -= (pstBuf->u32End - pstBuf->u32Start);
    }

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}

HI_S32  VIR_AcquireFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame)
{
    HI_U32 u32AcqSize, u32DataSize, u32TailData;
    HI_U8* pu8Src, *pu8Dst;
    static HI_U32 u32FrameIdx = 0;
    VIR_TRACK_STATE_S* pstVir = HI_NULL;
    HI_U32 u32VirID;
    VIR_BUFFUR_S* pstBuf;

    CHECK_Track(hTrack);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VIRTrack2VirID(hTrack);
    if (u32VirID >= AO_MAX_VIRTUAL_TRACK_NUM)
    {
        HI_ERR_AO("Invalid Virtual Track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)g_stVirTrack.pTrack[u32VirID];
    if (HI_NULL == pstVir)
    {
        HI_ERR_AO("virtual track(%d) is null!\n", u32VirID);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstBuf = pstVir->pstBuf;

    pstAOFrame->u32SampleRate = pstBuf->u32SampleRate;
    pstAOFrame->s32BitPerSample = pstBuf->s32BitPerSample;
    pstAOFrame->u32Channels = pstBuf->u32Channel;
    pstAOFrame->u32PcmSamplesPerFrame = pstBuf->u32PcmSamplesPerFrame;

    //pstAOFrame->u32PcmSamplesPerFrame = pstAOFrame->u32SampleRate * 10 / 1000; //default 10ms
    if (HI_UNF_BIT_DEPTH_16 == pstAOFrame->s32BitPerSample)
    {
        u32AcqSize = pstAOFrame->u32PcmSamplesPerFrame * pstAOFrame->u32Channels * sizeof(HI_S16);
    }
    else
    {
        u32AcqSize = pstAOFrame->u32PcmSamplesPerFrame * pstAOFrame->u32Channels * sizeof(HI_S32);
    }

    u32DataSize = VIRGetDataSize(pstBuf);
    if (u32AcqSize > u32DataSize)
    {
        HI_WARN_AO("Acquire size(%d) exceed buf data size(%d)\n", u32AcqSize, u32DataSize);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_ERR_AO_VIRTUALBUF_EMPTY;
    }

    pstAOFrame->ps32PcmBuffer = (HI_S32*)(pstBuf->pu8BufBase + (pstBuf->u32Read - pstBuf->u32Start));

    if (pstBuf->u32Write < pstBuf->u32Read)
    {
        u32TailData = pstBuf->u32End - pstBuf->u32Read;

        if (u32AcqSize > u32TailData)
        {
            pu8Src = pstBuf->pu8BufBase + pstBuf->u32Start;
            pu8Dst = pstBuf->pu8BufBase + pstBuf->u32End;
            memcpy((HI_VOID*)pu8Dst, (HI_VOID*)pu8Src, u32AcqSize - u32TailData);
        }
    }

    pstAOFrame->u32PtsMs = VIRAcquirePTS(pstBuf, pstAOFrame->u32PcmSamplesPerFrame, pstAOFrame->u32SampleRate);

    pstAOFrame->bInterleaved = HI_TRUE;
    pstAOFrame->u32FrameIndex = u32FrameIdx++;
    pstAOFrame->u32BitsBytesPerFrame = 0;
    pstAOFrame->ps32BitsBuffer = HI_NULL;
    pstAOFrame->u32IEC61937DataType = 0;

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}

HI_S32  VIR_ReleaseFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S* pstAOFrame)

{
    VIR_TRACK_STATE_S* pstVir = HI_NULL;
    HI_U32 u32VirID;
    VIR_BUFFUR_S* pstBuf;

    CHECK_Track(hTrack);

    VIRTRACK_LOCK(g_stVirTrack.stMutex);

    u32VirID = VIRTrack2VirID(hTrack);

    if (u32VirID >= AO_MAX_VIRTUAL_TRACK_NUM)
    {
        HI_ERR_AO("Invalid Virtual Track ID!\n");
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstVir = (VIR_TRACK_STATE_S*)g_stVirTrack.pTrack[u32VirID];

    if (HI_NULL == pstVir)
    {
        HI_ERR_AO("virtual track(%d) is null!\n", u32VirID);
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_FAILURE;
    }

    pstBuf = pstVir->pstBuf;

    if (pstBuf->u32Channel != pstAOFrame->u32Channels
        || pstBuf->s32BitPerSample != pstAOFrame->s32BitPerSample
        || pstBuf->u32SampleRate != pstAOFrame->u32SampleRate)
    {
        VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
        return HI_SUCCESS; //verify
    }

    VIRReleasePTS(pstBuf, pstAOFrame->u32PtsMs);

    /* update ReadPtr */
    if (16 == pstAOFrame->s32BitPerSample)
    {
        pstBuf->u32Read += pstAOFrame->u32PcmSamplesPerFrame * pstAOFrame->u32Channels * sizeof(HI_S16);
    }
    else
    {
        pstBuf->u32Read += pstAOFrame->u32PcmSamplesPerFrame * pstAOFrame->u32Channels * sizeof(HI_S32);
    }

    if (pstBuf->u32Read >= pstBuf->u32End)
    {
        pstBuf->u32Read -= (pstBuf->u32End - pstBuf->u32Start);
    }

    pstAOFrame->ps32PcmBuffer = HI_NULL;

    VIRTRACK_UNLOCK(g_stVirTrack.stMutex);
    return HI_SUCCESS;
}


