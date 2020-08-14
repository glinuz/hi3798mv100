/*****************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_aenc.c
* Description: Describe main functionality and purpose of this file.
*
* History:
* Version   Date         Author     DefectNum    Description
* 0.01
*
*****************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_drv_struct.h"
#include "hi_error_mpi.h"

#include "hi_mpi_ai.h"
#include "drv_ai_ioctl.h"

#include "hi_mpi_ao.h"
#include "hi_drv_aenc.h"
#include "hi_mpi_aenc.h"

#ifdef __DPT__
#include "iec61937_parser.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define AI_MAX_DST                     (4)
#define AI_SLEEP_TIME_MS               (5)
#define AI_CHECK_DELAY_SLEEP_TIME_MS   (1)
#define AO_TRACK_DELAY_FOR_AI_TIME     20
#define U32_MAX_VALUE                  0xffffffff

typedef struct
{
    HI_HANDLE           hAI;

    HI_HANDLE           hSlaTrack;       //ai->ao by aip buff
    HI_BOOL             bNeedStart;      //start track

    HI_U32              u32DstNum;
    HI_HANDLE           hDst[AI_MAX_DST];

    HI_BOOL             bPassThrough;
#ifdef __DPT__
    HI_PARSER_HANDLE    hParser;
#endif
    HI_UNF_AI_ATTR_S    stAttr;
    HI_U32              u32Channel;
    HI_U32              u32Bitdepth;

    AI_PROC_INFO_S*     pstAiProcInfo;

    HI_BOOL             AiThreadRun;
    pthread_t           AiDataThdInst;   /* run handle of ai thread */
    pthread_t           AiTrackThdInst;  /* run handle of start track */
    pthread_mutex_t     AiMutex;  /* mutex for data safety use */
} AI_MPISTATE_S;

typedef struct
{
    AI_MPISTATE_S* pstAI_S[AI_MAX_TOTAL_NUM];
} AI_MPIRESOURCE_S;

#define AI_DEV_NAME "/dev/" UMAP_DEVNAME_AI
static HI_S32 g_s32AIFd = -1;
static AI_MPIRESOURCE_S g_AiRes;

static HI_VOID AI_Mutex_Lock(pthread_mutex_t* ss)
{
    (HI_VOID)pthread_mutex_lock(ss);
}

static HI_VOID AI_Mutex_UnLock(pthread_mutex_t* ss)
{
    (HI_VOID)pthread_mutex_unlock(ss);
}

static HI_VOID AISetChnAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAiAttr, AI_MPISTATE_S* pstAiState)
{
    memcpy(&pstAiState->stAttr, pstAiAttr, sizeof(HI_UNF_AI_ATTR_S));

    switch (enAiPort)
    {
        case HI_UNF_AI_I2S0:
        case HI_UNF_AI_I2S1:
            pstAiState->u32Channel  = pstAiAttr->unAttr.stI2sAttr.stAttr.enChannel;
            pstAiState->u32Bitdepth = pstAiAttr->unAttr.stI2sAttr.stAttr.enBitDepth;
            return;

        case HI_UNF_AI_HDMI0:
        case HI_UNF_AI_HDMI1:
        case HI_UNF_AI_HDMI2:
        case HI_UNF_AI_HDMI3:
            pstAiState->u32Channel  = pstAiAttr->unAttr.stHDMIAttr.enChannel;
            pstAiState->u32Bitdepth = pstAiAttr->unAttr.stHDMIAttr.enBitDepth;

            if ((HI_UNF_AI_HDMI_FORMAT_LBR == pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataFormat) ||
                (HI_UNF_AI_HDMI_FORMAT_HBR == pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataFormat))
            {
                pstAiState->bPassThrough = HI_TRUE;
#ifdef __DPT__
                pstAiState->pstAiProcInfo->enDataType = pstAiAttr->unAttr.stHDMIAttr.enHdmiAudioDataType;
#endif
            }
            return;

        case HI_UNF_AI_ADC0:
        case HI_UNF_AI_ADC1:
        case HI_UNF_AI_ADC2:
        case HI_UNF_AI_ADC3:
        case HI_UNF_AI_ADC4:
        case HI_UNF_AI_SIF0:
            pstAiState->u32Channel  = HI_UNF_I2S_CHNUM_2;
            pstAiState->u32Bitdepth = HI_UNF_I2S_BIT_DEPTH_16;
            return;

        default:
            return;
    }
}

static HI_U32 AICalcFrameSize(HI_U32 u32Ch, HI_U32 u32BitDepth)
{
    HI_U32 uFrameSize = 0;

    switch (u32BitDepth)
    {
        case 16:
            uFrameSize = u32Ch * sizeof(HI_U16);
            break;
        case 24:
            uFrameSize = u32Ch * sizeof(HI_U32);
            break;
        default:
            break;
    }

    return uFrameSize;
}

static HI_S32 AIGetChnId(HI_HANDLE hAI, HI_U32* pu32ChnId)
{
    HI_U32 u32ChnId;

    for (u32ChnId = 0; u32ChnId < AI_MAX_TOTAL_NUM; u32ChnId++)
    {
        if ((HI_NULL != g_AiRes.pstAI_S[u32ChnId]) && (hAI == g_AiRes.pstAI_S[u32ChnId]->hAI))
        {
            break;
        }
    }

    if (AI_MAX_TOTAL_NUM == u32ChnId)
    {
        HI_ERR_AI("invalid Ai chn\n");
        return HI_FAILURE;
    }

    *pu32ChnId = u32ChnId;

    return HI_SUCCESS;
}

static HI_VOID AIBuildFrame(AI_MPISTATE_S* pstAiState, HI_UNF_AO_FRAMEINFO_S* pstUnfFrame)
{
    pstUnfFrame->u32PtsMs        = 0;
    pstUnfFrame->s32BitPerSample = pstAiState->u32Bitdepth;
    pstUnfFrame->bInterleaved    = HI_TRUE;
    pstUnfFrame->u32SampleRate   = pstAiState->stAttr.enSampleRate;
    pstUnfFrame->u32Channels     = pstAiState->u32Channel;
    pstUnfFrame->u32FrameIndex   = 0;
    pstUnfFrame->u32PcmSamplesPerFrame = pstAiState->stAttr.u32PcmSamplesPerFrame;
    pstUnfFrame->u32BitsBytesPerFrame  = 0;
    pstUnfFrame->u32IEC61937DataType   = 0;
    pstUnfFrame->ps32BitsBuffer  = HI_NULL;
    pstUnfFrame->ps32PcmBuffer   = HI_NULL;
}

static HI_S32 AIKernelReleaseFrame(HI_HANDLE hAI)
{
    HI_S32 s32Ret;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_RELEASEFRAME, &hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_RELEASEFRAME failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AIKernelAcquireFrame(AI_MPISTATE_S* pstAiState, HI_U32 u32RequestSize, HI_UNF_AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;
    AI_Buf_Param_S stAiBufInfo;
    AI_Frame_Param_S stAiGetFrame;

    stAiBufInfo.hAI = pstAiState->hAI;
    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETBUFINFO, &stAiBufInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETBUFINFO failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    stAiGetFrame.hAI = pstAiState->hAI;
    stAiGetFrame.u32NeedBytes = u32RequestSize;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_ACQUIREFRAME, &stAiGetFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_ACQUIREFRAME failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    AIBuildFrame(pstAiState, pstFrame);

    pstFrame->ps32PcmBuffer = (HI_S32*)((HI_U8*)HI_NULL + stAiBufInfo.stAiBuf.tUserVirAddr);
    return s32Ret;
}

#ifdef __DPT__
static HI_S32 AIParserSendFrame(AI_MPISTATE_S* pstAiState)
{
    HI_S32 s32Ret;
    HI_U32 u32NeedBytes = 0;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;
    IEC61937_PARSER_STREAM_BUF_S stStream;

    s32Ret = IEC61937_PARSER_GetBurstLen(pstAiState->hParser, &u32NeedBytes);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("IEC61937_PARSER_GetIECSyncLen failed 0x%x \n", s32Ret);
        return s32Ret;
    }

    s32Ret = IEC61937_PARSER_GetBuf(pstAiState->hParser, u32NeedBytes, &stStream);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("IEC61937_PARSER_GetBuf failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    if (0 == stStream.u32Size)
    {
        return HI_SUCCESS;
    }

    s32Ret = AIKernelAcquireFrame(pstAiState, stStream.u32Size, &stAOFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIKernelAcquireFrame failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    memcpy(stStream.pu8Data, stAOFrame.ps32PcmBuffer, stStream.u32Size);

    s32Ret = AIKernelReleaseFrame(pstAiState->hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIKernelReleaseFrame failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    s32Ret = IEC61937_PARSER_PutBuf(pstAiState->hParser, &stStream);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("IEC61937_PARSER_PutBuf failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AIParserAcquireFrame(AI_MPISTATE_S* pstAiState, HI_UNF_AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;

    s32Ret = AIParserSendFrame(pstAiState);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_AI("Call AIParserSendFrame failed(0x%x)\n", s32Ret);
    }

    if (HI_ERR_AI_NOT_ENOUGH_DATA == s32Ret)
    {
        return s32Ret;
    }

    s32Ret = IEC61937_PARSER_AcquireFrame(pstAiState->hParser, (HI_VOID*)(&pstFrame->ps32PcmBuffer), &pstFrame->u32BitsBytesPerFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_WARN_AI("Call IEC61937_PARSER_AcquireFrame failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}

static HI_S32 AIParserReleaseFrame(HI_PARSER_HANDLE hParser, HI_UNF_AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;

    s32Ret = IEC61937_PARSER_ReleaseFrame(hParser, pstFrame->u32BitsBytesPerFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("IEC61937_PARSER_ReleaseFrame failed(0x%x)\n", s32Ret);
    }

    return s32Ret;
}
#endif

HI_VOID* AITrackThread(HI_VOID* Arg)
{
    HI_S32 s32Ret;
    AI_MPISTATE_S* pstAiState = (AI_MPISTATE_S*)Arg;
    HI_BOOL bEnable;
    HI_U32 DelayMs = 0;
    HI_S32 s32Flag = 0;
    HI_U32 u32StartSysTime = 0;
    HI_U32 u32EndSysTime = 0;
    HI_U32 u32DelayTime = 0;
    HI_UNF_AI_DELAY_S stDelayCompensation;
    memset(&stDelayCompensation, 0, sizeof(HI_UNF_AI_DELAY_S));

    while (pstAiState->AiThreadRun)
    {
        AI_Mutex_Lock(&pstAiState->AiMutex);
        if (HI_INVALID_HANDLE != pstAiState->hSlaTrack)
        {
            s32Ret = HI_MPI_AI_GetEnable(pstAiState->hAI, &bEnable);
            if ((HI_SUCCESS == s32Ret) && (HI_TRUE == bEnable))
            {
                if (HI_TRUE == pstAiState->bNeedStart)   // SetEnable/Attach called
                {
                    if (0 == s32Flag)
                    {
                        HI_SYS_GetTimeStampMs(&u32StartSysTime);
                    }

                    s32Flag = 1;

                    HI_SYS_GetTimeStampMs(&u32EndSysTime);
                    if (u32EndSysTime >= u32StartSysTime)
                    {
                        u32DelayTime = u32EndSysTime - u32StartSysTime;
                    }
                    else
                    {
                        u32DelayTime = (U32_MAX_VALUE - u32StartSysTime ) + (u32EndSysTime + 1);
                    }
                    s32Ret = HI_MPI_AO_Track_GetDelayMs(pstAiState->hSlaTrack, &DelayMs);
                    s32Ret |= HI_MPI_AI_GetDelay(pstAiState->hAI, &stDelayCompensation);
                    if (((HI_SUCCESS == s32Ret) && (DelayMs + stDelayCompensation.u32DelayMs >= AO_TRACK_DELAY_FOR_AI_TIME))
                        || ((DelayMs + stDelayCompensation.u32DelayMs + u32DelayTime) >= AO_TRACK_DELAY_FOR_AI_TIME))
                    {
                        s32Ret = HI_MPI_AO_Track_Start(pstAiState->hSlaTrack);
                        if (s32Ret != HI_SUCCESS)
                        {
                            HI_ERR_AO("call HI_MPI_AO_Track_Start failed.\n");
                        }
                        HI_INFO_AO("HI_MPI_AO_Track_Start make bNeedStart HI_FALSE\n");
                        pstAiState->bNeedStart = HI_FALSE;
                    }
                }
                else
                {
                    s32Flag = 0;
                }
            }
            else
            {
                s32Flag = 0;
            }
        }
        else
        {
            s32Flag = 0;
        }

        AI_Mutex_UnLock(&pstAiState->AiMutex);
        usleep(AI_CHECK_DELAY_SLEEP_TIME_MS * 1000);
    }

    return HI_NULL;
}

HI_VOID* AIDataThread(HI_VOID* Arg)
{
    HI_S32 s32Ret;
    HI_U32 i;
    HI_BOOL bDataFlag = HI_FALSE;
    AI_MPISTATE_S* pstAiState = (AI_MPISTATE_S*)Arg;
    HI_UNF_AO_FRAMEINFO_S  stAiFrame;

    HI_U32 u32FrameSize   = AICalcFrameSize(pstAiState->u32Channel, pstAiState->u32Bitdepth);
    HI_U32 u32RequestSize = pstAiState->stAttr.u32PcmSamplesPerFrame * u32FrameSize;

    while (pstAiState->AiThreadRun)
    {
        AI_Mutex_Lock(&pstAiState->AiMutex);
        if ((HI_FALSE == bDataFlag) && (pstAiState->u32DstNum))
        {
            s32Ret = AIKernelAcquireFrame(pstAiState, u32RequestSize, &stAiFrame);
            if (HI_SUCCESS == s32Ret)
            {
                bDataFlag = HI_TRUE;
            }
            else
            {
                HI_WARN_AI("call HI_MPI_AI_AcquireFrame failed!\n");
                AI_Mutex_UnLock(&pstAiState->AiMutex);
                usleep(AI_SLEEP_TIME_MS * 1000);
                continue;
            }
        }

        if ((pstAiState->u32DstNum) && (HI_TRUE == bDataFlag))
        {
            if (HI_ID_AO == (pstAiState->hDst[0]) >> 16)
            {
                s32Ret = HI_MPI_AO_Track_SendData(pstAiState->hDst[0], &stAiFrame);
            }
            else
            {
                s32Ret = HI_MPI_AENC_SendBuffer(pstAiState->hDst[0], &stAiFrame);
            }

            if (HI_SUCCESS == s32Ret)
            {
                bDataFlag = HI_FALSE;
                if (1 < pstAiState->u32DstNum)
                {
                    for (i = 1; i < pstAiState->u32DstNum; i++)
                    {
                        if (HI_ID_AO == (pstAiState->hDst[i]) >> 16)
                        {
                            (HI_VOID)HI_MPI_AO_Track_SendData(pstAiState->hDst[i], &stAiFrame);
                        }
                        else
                        {
                            (HI_VOID)HI_MPI_AENC_SendBuffer(pstAiState->hDst[i], &stAiFrame);
                        }
                    }
                }

                s32Ret = AIKernelReleaseFrame(pstAiState->hAI);
            }
            else
            {
                bDataFlag = HI_TRUE;
            }
        }
        AI_Mutex_UnLock(&pstAiState->AiMutex);
        usleep(AI_SLEEP_TIME_MS * 1000);
    }

    return HI_NULL;
}

HI_S32 AIAttach(HI_HANDLE hAI, HI_HANDLE hDst)
{
    HI_S32 s32Ret;
    HI_U32 i;
    AI_MPISTATE_S* pstAiState;

    CHECK_AI_ID(hAI);
    if (HI_INVALID_HANDLE == hDst)
    {
        HI_ERR_AI("para hDst is invalid.\n");
        return HI_ERR_AI_INVALID_PARA;
    }

    if ((HI_ID_AO != (hDst >> 16)) && (HI_ID_AENC != (hDst >> 16)))
    {
        HI_ERR_AI("para hDst is invalid, just support attach sound_track or aenc.\n");
        return HI_ERR_AI_INVALID_PARA;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if ((HI_NULL != g_AiRes.pstAI_S[i]) && (hAI == g_AiRes.pstAI_S[i]->hAI))
        {
            break;
        }
    }

    if (AI_MAX_TOTAL_NUM == i)
    {
        HI_ERR_AI("AI chn not open\n");
        return HI_FAILURE;
    }
    pstAiState = g_AiRes.pstAI_S[i];

    if (( HI_ID_AO == hDst >> 16) && (AO_MAX_REAL_TRACK_NUM > (hDst & 0xff)))
    {
        if (HI_INVALID_HANDLE != pstAiState->hSlaTrack)
        {
            if (hDst == pstAiState->hSlaTrack)
            {
                return HI_SUCCESS;
            }
            else
            {
                HI_ERR_AI("AI can not attach more than one slave track!\n");
                return HI_FAILURE;
            }
        }

        if (0 != pstAiState->u32DstNum)
        {
            HI_ERR_AI("AI is already attached by virtual track or aenc!\n");
            return HI_ERR_AI_NOTSUPPORT;
        }

        s32Ret = HI_MPI_AO_Track_AttachAi(hAI, hDst);
        if (HI_SUCCESS == s32Ret)
        {
            pstAiState->hSlaTrack = hDst;
            HI_INFO_AI("pstAiState->bNeedStart = %d.\n", pstAiState->bNeedStart);
            pstAiState->bNeedStart = HI_TRUE;
        }

        return s32Ret;
    }

    AI_Mutex_Lock(&pstAiState->AiMutex);

    for (i = 0; i < AI_MAX_DST; i++)
    {
        if (pstAiState->hDst[i] == hDst)
        {
            AI_Mutex_UnLock(&pstAiState->AiMutex);
            return HI_SUCCESS;
        }
    }

    for (i = 0; i < AI_MAX_DST; i++)
    {
        if (HI_INVALID_HANDLE == pstAiState->hDst[i])
        {
            break;
        }
    }

    if (AI_MAX_DST == i)
    {
        HI_ERR_AI("AI has attached max dst.\n");
        AI_Mutex_UnLock(&pstAiState->AiMutex);
        return HI_FAILURE;
    }

    if (HI_INVALID_HANDLE != pstAiState->hSlaTrack)
    {
        HI_ERR_AI("AI is already attached by slave track!\n");
        AI_Mutex_UnLock(&pstAiState->AiMutex);
        return HI_ERR_AI_NOTSUPPORT;
    }

    pstAiState->hDst[i] = hDst;
    pstAiState->u32DstNum++;

    AI_Mutex_UnLock(&pstAiState->AiMutex);
    return HI_SUCCESS;
}

HI_S32 AIDetach(HI_HANDLE hAI, HI_HANDLE hDst)
{
    HI_U32 i;
    HI_S32 s32Ret;
    AI_MPISTATE_S* pstAiState;

    CHECK_AI_ID(hAI);

    if (HI_INVALID_HANDLE == hDst)
    {
        HI_ERR_AI("para hDst is invalid.\n");
        return HI_ERR_AI_INVALID_PARA;
    }

    if ((HI_ID_AO != (hDst >> 16)) && (HI_ID_AENC != (hDst >> 16)))
    {
        HI_ERR_AI("para hDst is invalid, just support attach sound_track or aenc.\n");
        return HI_ERR_AI_INVALID_PARA;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if ((HI_NULL != g_AiRes.pstAI_S[i]) && (hAI == g_AiRes.pstAI_S[i]->hAI))
        {
            break;
        }
    }

    if (AI_MAX_TOTAL_NUM == i)
    {
        HI_ERR_AI("AI chn not open\n");
        return HI_FAILURE;
    }

    pstAiState = g_AiRes.pstAI_S[i];

    if (( HI_ID_AO == hDst >> 16) && (AO_MAX_REAL_TRACK_NUM > (hDst & 0xff)))
    {
        if (hDst == pstAiState->hSlaTrack)
        {
            s32Ret = HI_MPI_AO_Track_DetachAi(hAI, hDst);
            if (HI_SUCCESS == s32Ret)
            {
                pstAiState->hSlaTrack = HI_INVALID_HANDLE;
            }
            return s32Ret;
        }
        else
        {
            HI_ERR_AI("This track is not attach Ai,can not detach!\n");
            return HI_FAILURE;
        }
    }

    AI_Mutex_Lock(&pstAiState->AiMutex);

    for (i = 0; i < pstAiState->u32DstNum; i++)
    {
        if (pstAiState->hDst[i] == hDst)
        {
            break;
        }
    }

    if (i == pstAiState->u32DstNum)
    {
        HI_ERR_AI("this track is not attached, can not detach.\n");
        AI_Mutex_UnLock(&pstAiState->AiMutex);
        return HI_FAILURE;
    }

    pstAiState->hDst[i] = pstAiState->hDst[pstAiState->u32DstNum - 1];
    pstAiState->hDst[pstAiState->u32DstNum - 1] = HI_INVALID_HANDLE;
    pstAiState->u32DstNum--;

    AI_Mutex_UnLock(&pstAiState->AiMutex);
    return HI_SUCCESS;
}

static HI_S32 AIProcInit(AI_MPISTATE_S* pstAiState)
{
    HI_S32 s32Ret;
    AI_ProcInit_Param_S stParam;

    CHECK_AI_NULL_PTR(pstAiState);
    CHECK_AI_ID(pstAiState->hAI);

    stParam.hAI = pstAiState->hAI;
    stParam.tProcPhyAddr = 0;
    s32Ret = ioctl(g_s32AIFd, CMD_AI_PROCINIT, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_PROCINIT failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    pstAiState->pstAiProcInfo = (AI_PROC_INFO_S*)HI_MMAP(stParam.tProcPhyAddr, sizeof(AI_PROC_INFO_S));
    if (HI_NULL == pstAiState->pstAiProcInfo)
    {
        HI_ERR_AI("HI_MMAP AI_PROC_INFO_S failed!\n");
        (HI_VOID)ioctl(g_s32AIFd, CMD_AI_PROCDEINIT, &pstAiState->hAI);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AIProcDeInit(AI_MPISTATE_S* pstAiState)
{
    HI_S32 s32Ret;

    CHECK_AI_NULL_PTR(pstAiState);
    CHECK_AI_ID(pstAiState->hAI);

    if (HI_NULL != pstAiState->pstAiProcInfo)
    {
        (HI_VOID)HI_MUNMAP(pstAiState->pstAiProcInfo);
    }

    s32Ret = ioctl(g_s32AIFd, CMD_AI_PROCDEINIT, &pstAiState->hAI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_PROCDEINIT failed(0x%x)!\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_Init(HI_VOID)
{
    HI_U32 i;
    if (g_s32AIFd < 0)
    {
        g_s32AIFd = open(AI_DEV_NAME, O_RDWR, 0);
        if (g_s32AIFd < 0)
        {
            HI_FATAL_AI("OpenAIDevice err\n");
            g_s32AIFd = -1;
            return HI_ERR_AI_NOT_INIT;
        }

        for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
        {
            g_AiRes.pstAI_S[i] = NULL;
        }
    }
    return HI_SUCCESS;
}

HI_S32   HI_MPI_AI_DeInit(HI_VOID)
{
    if (g_s32AIFd > 0)
    {
        close(g_s32AIFd);
        g_s32AIFd = -1;
    }

    return HI_SUCCESS;
}

HI_S32   HI_MPI_AI_GetDefaultAttr(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    AI_GetDfAttr_Param_S stAiGetDfAttr;

    CHECK_AI_NULL_PTR(pstAttr);
    stAiGetDfAttr.enAiPort = enAiPort;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETDEFAULTATTR, &stAiGetDfAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETDEFAULTATTR failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    memcpy(pstAttr, &stAiGetDfAttr.stAttr, sizeof(HI_UNF_AI_ATTR_S));

    return s32Ret;
}

HI_S32 HI_MPI_AI_SetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    HI_U32 i = 0;
    AI_Attr_Param_S stAiSetAttr;
    AI_MPISTATE_S* pstAiState = HI_NULL;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstAttr);

    stAiSetAttr.hAI = hAI;

    memcpy(&stAiSetAttr.stAttr, pstAttr, sizeof(HI_UNF_AI_ATTR_S));

    s32Ret = ioctl(g_s32AIFd, CMD_AI_SETATTR, &stAiSetAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_SETATTR failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (g_AiRes.pstAI_S[i] && (hAI == g_AiRes.pstAI_S[i]->hAI))
        {
            pstAiState = g_AiRes.pstAI_S[i];
            break;
        }
    }

    if (HI_NULL != pstAiState && HI_INVALID_HANDLE != pstAiState->hSlaTrack)
    {
        HI_HANDLE hTrack = pstAiState->hSlaTrack;
        //detach track for new ai attr
        HI_INFO_AI("AIDetach Track\n");
        s32Ret = AIDetach(hAI, pstAiState->hSlaTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AI_Detach Failed 0x%x \n", s32Ret);
        }

        HI_INFO_AI("HI_MPI_AO_Track_Stop called\n");
        s32Ret = HI_MPI_AO_Track_Stop(hTrack);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_AO("call HI_MPI_AO_Track_Stop failed.\n");
        }

        HI_INFO_AI("AIDetach Track\n");
        s32Ret = AIAttach(hAI, hTrack);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AI_Attach Failed 0x%x \n", s32Ret);
        }
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_GetAttr(HI_HANDLE hAI, HI_UNF_AI_ATTR_S* pstAttr)
{
    HI_S32 s32Ret;
    AI_Attr_Param_S stAiGetAttr;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstAttr);

    stAiGetAttr.hAI = hAI;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETATTR, &stAiGetAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETATTR failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    memcpy(pstAttr, &stAiGetAttr.stAttr, sizeof(HI_UNF_AI_ATTR_S));

    return s32Ret;

}

HI_S32 HI_MPI_AI_Create(HI_UNF_AI_E enAiPort, HI_UNF_AI_ATTR_S* pstAttr, HI_HANDLE* phandle)
{
    HI_S32 s32Ret;
    HI_U32 i;
    AI_MPISTATE_S* pstAiState;
    HI_VOID* pUserAddr;

    AI_Create_Param_S stAiParam;
    AI_Buf_Param_S stAiBufInfo;

    CHECK_AI_NULL_PTR(pstAttr);
    CHECK_AI_NULL_PTR(phandle);

    stAiParam.enAiPort = enAiPort;

    memcpy(&stAiParam.stAttr, pstAttr, sizeof(HI_UNF_AI_ATTR_S));

    s32Ret = ioctl(g_s32AIFd, CMD_AI_CREATE, &stAiParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_CREATE failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    stAiBufInfo.hAI = stAiParam.hAI;
    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETBUFINFO, &stAiBufInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETBUFINFO failed(0x%x)!\n", s32Ret);
        goto ERR_CREAT;
    }

    pUserAddr = HI_MEM_Map(stAiBufInfo.stAiBuf.tPhyBaseAddr, stAiBufInfo.stAiBuf.u32Size);
    if (HI_NULL == pUserAddr)
    {
        HI_ERR_AI("HI_MEM_Map failed!\n");
        s32Ret = HI_FAILURE;
        goto ERR_CREAT;
    }

    stAiBufInfo.stAiBuf.tUserVirAddr = (HI_U8*)pUserAddr - (HI_U8*)HI_NULL;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_SETBUFINFO, &stAiBufInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_SETBUFINFO failed(0x%x)!\n", s32Ret);
        goto ERR_MMAP;
    }

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if (HI_NULL == g_AiRes.pstAI_S[i])
        {
            break;
        }
    }

    if (AI_MAX_TOTAL_NUM == i)
    {
        HI_ERR_AI("too many Ai chn\n");
        s32Ret = HI_FAILURE;
        goto ERR_MMAP;
    }

    g_AiRes.pstAI_S[i] = (AI_MPISTATE_S*)HI_MALLOC(HI_ID_AI, sizeof(AI_MPISTATE_S));
    if (HI_NULL == g_AiRes.pstAI_S[i])
    {
        s32Ret = HI_FAILURE;
        goto ERR_MMAP;
    }

    pstAiState = g_AiRes.pstAI_S[i];
    memset(pstAiState, 0, sizeof(AI_MPISTATE_S));

    (HI_VOID)pthread_mutex_init(&pstAiState->AiMutex, NULL);

    pstAiState->hAI = stAiParam.hAI;
    pstAiState->hSlaTrack = HI_INVALID_HANDLE;

    for (i = 0; i < AI_MAX_DST; i++)
    {
        pstAiState->hDst[i] = HI_INVALID_HANDLE;
    }

    s32Ret = AIProcInit(pstAiState);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIProcInit failed(0x%x)\n", s32Ret);
        goto ERR_KFREE_MUTEX;
    }

    AISetChnAttr(enAiPort, pstAttr, pstAiState);

#ifdef __DPT__
    if (HI_TRUE == pstAiState->bPassThrough)
    {
        s32Ret = IEC61937_PARSER_Create(&pstAiState->hParser);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("IEC61937_PARSER_Create failed(0x%x)!\n", s32Ret);
            goto ERR_AI_PROC_DEINIT;
        }
    }
#endif

    pstAiState->AiThreadRun = HI_TRUE;
    s32Ret = pthread_create(&pstAiState->AiDataThdInst, HI_NULL, AIDataThread, (HI_VOID*)pstAiState);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("Ai pthread_create AI_DataThread  failed!\n");
        goto ERR_DESTROY_PARSER;
    }

    s32Ret = pthread_create(&pstAiState->AiTrackThdInst, HI_NULL, AITrackThread, (HI_VOID*)pstAiState);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("Ai pthread_create AI_TrackThread failed!\n");
        goto ERR_DESTROY_DATATHREAD;
    }

    *phandle = stAiParam.hAI;

    return s32Ret;

ERR_DESTROY_DATATHREAD:
    pstAiState->AiThreadRun = HI_FALSE;
    (HI_VOID)pthread_join(pstAiState->AiDataThdInst, HI_NULL);

ERR_DESTROY_PARSER:
#ifdef __DPT__
    if (HI_TRUE == pstAiState->bPassThrough)
    {
        (HI_VOID)IEC61937_PARSER_Destroy(pstAiState->hParser);
    }

ERR_AI_PROC_DEINIT:
#endif
    (HI_VOID)AIProcDeInit(pstAiState);

ERR_KFREE_MUTEX:
    (HI_VOID)pthread_mutex_destroy(&pstAiState->AiMutex);

    HI_FREE(HI_ID_AI, (HI_VOID*)(pstAiState));
ERR_MMAP:
    HI_MEM_Unmap((HI_VOID*)((HI_U8*)HI_NULL + stAiBufInfo.stAiBuf.tUserVirAddr));
ERR_CREAT:
    (HI_VOID)ioctl(g_s32AIFd, CMD_AI_DESTROY, &stAiParam);
    return s32Ret;
}

HI_S32 HI_MPI_AI_Destroy(HI_HANDLE hAI)
{
    HI_S32 s32Ret;
    HI_U32 i;
    AI_Buf_Param_S stAiBufInfo;
    AI_MPISTATE_S* pstAiState;

    CHECK_AI_ID(hAI);

    stAiBufInfo.hAI = hAI;

    for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
    {
        if ((HI_NULL != g_AiRes.pstAI_S[i]) && (hAI == g_AiRes.pstAI_S[i]->hAI))
        {
            break;
        }
    }

    if (AI_MAX_TOTAL_NUM == i)
    {
        HI_ERR_AI("This AI chn is not open!\n");
        return HI_FAILURE;
    }

    pstAiState = g_AiRes.pstAI_S[i];

    pstAiState->AiThreadRun = HI_FALSE;
    (HI_VOID)pthread_join(pstAiState->AiTrackThdInst, HI_NULL);
    (HI_VOID)pthread_join(pstAiState->AiDataThdInst, HI_NULL);
#ifdef __DPT__
    if (HI_TRUE == pstAiState->bPassThrough)
    {
        (HI_VOID)IEC61937_PARSER_Destroy(pstAiState->hParser);
    }
#endif
    s32Ret = AIProcDeInit(pstAiState);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIProcDeInit failed(0x%x)", s32Ret);
    }

    (HI_VOID)pthread_mutex_destroy(&pstAiState->AiMutex);

    HI_FREE(HI_ID_AI, (HI_VOID*)(pstAiState));
    g_AiRes.pstAI_S[i] = HI_NULL;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETBUFINFO, &stAiBufInfo);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETBUFINFO failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }
    else
    {
        (HI_VOID)HI_MEM_Unmap((HI_VOID*)((HI_U8*)HI_NULL + stAiBufInfo.stAiBuf.tUserVirAddr));
    }

    return ioctl(g_s32AIFd, CMD_AI_DESTROY, &hAI);
}

HI_S32 HI_MPI_AI_SetEnable(HI_HANDLE hAI, HI_BOOL bEnable)
{
    HI_S32 s32Ret;
    HI_U32 i = 0;
    AI_MPISTATE_S* pstAiState = HI_NULL;
    AI_Enable_Param_S stAiEnable;

    CHECK_AI_ID(hAI);

    stAiEnable.hAI = hAI;
    stAiEnable.bAiEnable = bEnable;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_SETENABLE, &stAiEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_SETENABLE failed(0x%x)!\n", s32Ret);
        return s32Ret;
    }

    if (HI_TRUE == bEnable)
    {
        for (i = 0; i < AI_MAX_TOTAL_NUM; i++)
        {
            if (g_AiRes.pstAI_S[i] && (hAI == g_AiRes.pstAI_S[i]->hAI))
            {
                HI_INFO_AI("pstAiState->bNeedStart = HI_TRUE\n");
                pstAiState = g_AiRes.pstAI_S[i];
                pstAiState->bNeedStart = HI_TRUE;
                break;
            }
        }
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_GetEnable(HI_HANDLE hAI, HI_BOOL* pbEnable)
{
    HI_S32 s32Ret;
    AI_Enable_Param_S stAiEnable;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pbEnable);

    stAiEnable.hAI = hAI;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETENABLE, &stAiEnable);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETENABLE failed(0x%x)!\n", s32Ret);
    }
    else
    {
        *pbEnable = stAiEnable.bAiEnable;
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_AcquireFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;
    HI_U32 u32Chn;
    AI_MPISTATE_S* pstAiState;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstFrame);

    s32Ret = AIGetChnId(hAI, &u32Chn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIGetChnId failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    pstAiState = g_AiRes.pstAI_S[u32Chn];
    CHECK_AI_NULL_PTR(pstAiState);

    if ((HI_INVALID_HANDLE != pstAiState->hDst[0]) || (HI_INVALID_HANDLE != pstAiState->hSlaTrack))
    {
        HI_ERR_AI("Aenc or Track attach this Ai chn,can not acquire frame!\n");
        return  HI_FAILURE;
    }
#ifdef __DPT__
    if (HI_TRUE == pstAiState->bPassThrough)
    {
        s32Ret = AIParserAcquireFrame(pstAiState, pstFrame);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIParserAcquireFrame failed(0x%x)\n", s32Ret);
        }
    }
    else
#endif
    {
        HI_U32 u32RequestSize = pstAiState->stAttr.u32PcmSamplesPerFrame * AICalcFrameSize(pstAiState->u32Channel, pstAiState->u32Bitdepth);

        s32Ret = AIKernelAcquireFrame(pstAiState, u32RequestSize, pstFrame);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIChnAcquireFrame failed(0x%x)\n", s32Ret);
        }
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_ReleaseFrame(HI_HANDLE hAI, HI_UNF_AO_FRAMEINFO_S* pstFrame)
{
    HI_S32 s32Ret;
    HI_U32 u32Chn;
    AI_MPISTATE_S* pstAiState;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstFrame);

    s32Ret = AIGetChnId(hAI, &u32Chn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIGetChnId failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    pstAiState = g_AiRes.pstAI_S[u32Chn];
    CHECK_AI_NULL_PTR(pstAiState);

    if ((HI_INVALID_HANDLE != pstAiState->hDst[0]) || (HI_INVALID_HANDLE != pstAiState->hSlaTrack))
    {
        HI_ERR_AI("Aenc or Track attach this Ai chn,can not release frame!\n");
        return  HI_FAILURE;
    }

#ifdef __DPT__
    if (HI_TRUE == pstAiState->bPassThrough)
    {
        s32Ret = AIParserReleaseFrame(pstAiState->hParser, pstFrame);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIParserReleaseFrame failed(0x%x)\n", s32Ret);
        }
    }
    else
#endif
    {
        s32Ret = AIKernelReleaseFrame(hAI);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIKernelReleaseFrame failed(0x%x)\n", s32Ret);
        }
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_Attach(HI_HANDLE hAI, HI_HANDLE hDst)
{
    return AIAttach(hAI, hDst);
}

HI_S32 HI_MPI_AI_Detach(HI_HANDLE hAI, HI_HANDLE hDst)
{
    return AIDetach(hAI, hDst);
}

HI_S32 HI_MPI_AI_SetDelay(HI_HANDLE hAI, const HI_UNF_AI_DELAY_S* pstDelay)
{
    HI_S32 s32Ret;
    AI_DelayComps_Param_S stParam;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstDelay);

    stParam.hAI = hAI;
    memcpy(&stParam.stDelayComps, pstDelay, sizeof(HI_UNF_AI_DELAY_S));

    s32Ret = ioctl(g_s32AIFd, CMD_AI_SETDELAYCOMPS, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_SETDELAYCOMPS failed(0x%x)!\n", s32Ret);
    }

    return s32Ret;
}

HI_S32 HI_MPI_AI_GetDelay(HI_HANDLE hAI, HI_UNF_AI_DELAY_S* pstDelay)
{
    HI_S32 s32Ret;
    AI_DelayComps_Param_S stParam;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstDelay);

    stParam.hAI = hAI;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETDELAYCOMPS, &stParam);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETDELAYCOMPS failed(0x%x)!\n", s32Ret);
    }
    else
    {
        memcpy(pstDelay, &stParam.stDelayComps, sizeof(HI_UNF_AI_DELAY_S));
    }

    return s32Ret;
}

#ifdef __DPT__
HI_S32 HI_MPI_AI_GetStreamType(HI_HANDLE hAI, HI_UNF_AI_HDMI_DATA_TYPE_E* penStreamType)
{
    HI_S32 s32Ret;
    HI_U32 u32Chn;
    AI_MPISTATE_S* pstAiState;
    AI_StreamType_Param_S stStreamType;
    IEC61937_PARSER_STREAM_TYPE_E enStreamType = IEC61937_PARSER_STREAM_UNKNOWN;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(penStreamType);

    s32Ret = AIGetChnId(hAI, &u32Chn);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("AIGetChnId failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    pstAiState = g_AiRes.pstAI_S[u32Chn];
    CHECK_AI_NULL_PTR(pstAiState);

    if ((HI_INVALID_HANDLE != pstAiState->hDst[0]) ||
        (HI_INVALID_HANDLE != pstAiState->hSlaTrack))
    {
        HI_ERR_AI("Aenc or Track attach this Ai chn!\n");
        return  HI_FAILURE;
    }

    stStreamType.hAI = hAI;

    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETSTREAMTYPE, &stStreamType);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("ioctl CMD_AI_GETSTREAMTYPE failed(0x%x)\n", s32Ret);
        return s32Ret;
    }

    if (HI_FALSE == pstAiState->bPassThrough ||
        HI_UNF_AI_HDMI_DATA_PCM == pstAiState->pstAiProcInfo->enDataType)
    {
        *penStreamType = HI_UNF_AI_HDMI_DATA_PCM;
        return HI_SUCCESS;
    }

    if (HI_UNF_AI_HDMI_DATA_UNKNOWN == pstAiState->pstAiProcInfo->enDataType ||
        HI_UNF_AI_HDMI_DATA_BUTT == pstAiState->pstAiProcInfo->enDataType)
    {
        s32Ret = AIParserSendFrame(pstAiState);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_AI("AIParserSendFrame failed(0x%x)\n", s32Ret);
        }
    }

    s32Ret = IEC61937_PARSER_GetStreamType(pstAiState->hParser, &enStreamType);
    if (HI_SUCCESS != s32Ret)
    {
        HI_INFO_AI("IEC61937_PARSER_GetStreamType failed(0x%x)", s32Ret);
    }

    *penStreamType = (HI_U32)enStreamType;
    pstAiState->pstAiProcInfo->enDataType = (HI_U32)enStreamType;

    return s32Ret;
}

HI_S32 HI_MPI_AI_SetNRAttr(HI_HANDLE hAI, const HI_UNF_AI_NR_ATTR_S *pstNRAttr)
{
    HI_S32 s32Ret;
    AI_NR_Param_S stNRParam;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstNRAttr);

    stNRParam.hAi = hAI;
    memcpy(&stNRParam.stNR, pstNRAttr, sizeof(HI_UNF_AI_NR_ATTR_S));
    s32Ret = ioctl(g_s32AIFd, CMD_AI_SETNRATTRS, &stNRParam);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("Set AI NR Attr Error 0x%x \n", s32Ret);
    }
    return s32Ret;
}

HI_S32 HI_MPI_AI_GetNRAttr(HI_HANDLE hAI, HI_UNF_AI_NR_ATTR_S *pstNRAttr)
{
    HI_S32 s32Ret;
    AI_NR_Param_S stNRParam;

    CHECK_AI_ID(hAI);
    CHECK_AI_NULL_PTR(pstNRAttr);

    stNRParam.hAi = hAI;
    s32Ret = ioctl(g_s32AIFd, CMD_AI_GETNRATTRS, &stNRParam);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_AI("Get AI NR Error 0x%x \n", s32Ret);
    }
    else
    {
        memcpy(pstNRAttr, &stNRParam.stNR, sizeof(HI_UNF_AI_NR_ATTR_S));
    }
    return s32Ret;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
