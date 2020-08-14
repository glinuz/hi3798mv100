/**
\file
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author xxxx
\date 2015-02-06
*/
#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_HA_ADEC"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <dlfcn.h>
#include <dirent.h>
#endif

#include <OMX_Core.h>

#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "hi_type.h"
#include "ha_adec.h"
#include "hi_audio_codec.h"
#include "omx_audio_base.h"
#ifdef DOLBY_OMX_PASSTHRU_SUPPORT
#include "hi_unf_audio.h"
#include "hi_unf_sound.h"
#include "hi_mpi_ao.h"
#endif

#define DOLBY_PCM_PER_FRAME (1536)

#ifdef ADEC_FILE_SAVE
#ifdef ANDROID
#include <cutils/properties.h>
#define OMX_ADEC_FILE_DEBUG    "persist.sys.audio.omx.filesave"
#endif
#define OMX_ADEC_IN_ES_FILE    "/mnt/save/omx_in.es"
#define OMX_ADEC_OUT_PCM_FILE  "/mnt/save/omx_out.pcm"

static HI_U32 AUTIL_CalcFrameSize(HI_U32 uCh, HI_U32 uBitDepth)
{
    HI_U32 uFrameSize = 0;
    switch (uBitDepth)
    {
        case 16:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U16);
            break;
        case 24:
            uFrameSize = ((HI_U32)uCh) * sizeof(HI_U32);
            break;
    }
    return uFrameSize;
}

static HI_VOID CLOSEDumpFile(HI_AUDDATATYPE* pHAData)
{
    if (pHAData->pInFile)
    {
        fclose(pHAData->pInFile);
        pHAData->pInFile = HI_NULL;
    }

    if (pHAData->pOutFile)
    {
        fclose(pHAData->pOutFile);
        pHAData->pOutFile = HI_NULL;
    }
}

static HI_VOID OPENDumpFile(HI_AUDDATATYPE* pHAData)
{
#ifdef ADEC_FILE_SAVE
#ifdef ANDROID
    char value[PROPERTY_VALUE_MAX];
    memset(value, 0, PROPERTY_VALUE_MAX);
    property_get(OMX_ADEC_FILE_DEBUG, value, "");

    if (strncmp(value, "true", 4) == 0)
    {
#endif
        if (!pHAData->pInFile)
        {
            pHAData->pInFile = fopen(OMX_ADEC_IN_ES_FILE, "wb");
            if (!pHAData->pInFile)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Omx adec input buf debug open %s failed!\n", OMX_ADEC_IN_ES_FILE);
            }
        }

        if (!pHAData->pOutFile)
        {
            pHAData->pOutFile = fopen(OMX_ADEC_OUT_PCM_FILE, "wb");
            if (!pHAData->pOutFile)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Omx adec output buf debug open %s failed!\n", OMX_ADEC_OUT_PCM_FILE);
            }
        }
    }
#ifdef ANDROID
    else if (strncmp(value, "false", 5))
    {
        CLOSEDumpFile(pHAData);
    }
#endif
#endif
}

static HI_VOID ADECDumpFile(HI_AUDDATATYPE* pHAData, HI_HADECODE_INPACKET_S* avpkt,
                            HI_HADECODE_OUTPUT_S* avOut, OMX_BUFFERHEADERTYPE* pInBufHdr)
{
    HA_INTERNALBUF_S* pstInnerBuf = &pHAData->stAdec.sInternalBuf;

#ifdef ADEC_DYNAMIC_SAVE_SUPPORT    //for dynamic filesave
    OPENDumpFile(pHAData);
#endif

    if (pHAData->pOutFile)
    {
        HI_U32 u32Size = avOut->u32PcmOutSamplesPerFrame * AUTIL_CalcFrameSize(avOut->u32OutChannels, avOut->u32BitPerSample);
        fwrite((char*)avOut->ps32PcmOutBuf, 1, u32Size, pHAData->pOutFile);
    }

    if ((HI_FALSE == pHAData->stAdec.bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
    {
        if (pHAData->pInFile)
        {
            HI_U8* pbuffer = (HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Offset;
            fwrite((char*)pbuffer, 1, pstInnerBuf->s32Insize - avpkt->s32Size, pHAData->pInFile);
        }
    }
    else
    {
        if (pHAData->pInFile)
        {
            HI_U8* pbuffer = pInBufHdr->pBuffer + pInBufHdr->nOffset;
            fwrite((char*)pbuffer, 1, pInBufHdr->nFilledLen - avpkt->s32Size, pHAData->pInFile);
        }
    }
}
#endif


#define PACKETINSIZE_MAXNUM    2
#define CHECK_ADEC_NULL_PTR(ptr)                        \
    do                                                  \
    {                                                   \
        if (NULL == ptr)                                \
        {                                               \
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "invalid NULL poiner!\n"); \
            return HI_FAILURE;                          \
        }                                               \
    } while (0)


static HI_S32 ADECCheckHaDecoder(const HI_HA_DECODE_S* pEntry)
{
    CHECK_ADEC_NULL_PTR(pEntry->DecInit);
    CHECK_ADEC_NULL_PTR(pEntry->DecDeInit);
    CHECK_ADEC_NULL_PTR(pEntry->DecSetConfig);
    CHECK_ADEC_NULL_PTR(pEntry->DecGetMaxPcmOutSize);
    CHECK_ADEC_NULL_PTR(pEntry->DecGetMaxBitsOutSize);
    CHECK_ADEC_NULL_PTR(pEntry->DecDecodeFrame);

    return  HI_SUCCESS;
}


static HI_VOID ADECUnRegisterDecoderLib(HI_HA_DECODE_S* pEntry)
{
    HI_S32 dlok;
    dlok = dlclose(pEntry->pDllModule);
    if (dlok)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "dlclose fail\n");
    }
    return;
}

static HI_S32 ADECInit(HI_VOID** hDecoder, HA_ADEC_S* pstAdec)
{
    HI_HA_DECODE_S* pstHA = pstAdec->pstHA;

    pstHA->DecInit(hDecoder, &pstAdec->sOpenPram);
    if (HI_NULL == *hDecoder)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pstHA->DecInit fail\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 ADECDeInit(HI_VOID* hDecoder, HA_ADEC_S* pstAdec)
{
    HI_HA_DECODE_S* pstHA = pstAdec->pstHA;

    pstHA->DecDeInit(hDecoder);

    return HI_SUCCESS;
}

static HI_S32 ADECGetMaxOutSize(HI_VOID* hDecoder,
                                HA_ADEC_S* pstAdec,
                                HI_U32* u32MaxOutSize)
{
    HI_U32 s32PcmOutBufSize  = 0;
    HI_U32 s32BitsOutbufSize = 0;
    HI_HA_DECODE_S* pstHA    = pstAdec->pstHA;
    if (!hDecoder)
    {
        return HA_ErrorInvalidParameter;
    }

    pstHA->DecGetMaxPcmOutSize(hDecoder, &s32PcmOutBufSize);

    /* for passthrough */
    pstHA->DecGetMaxBitsOutSize(hDecoder, &s32BitsOutbufSize);
    if (s32BitsOutbufSize != 0)
    {
        pstAdec->u32BitstreamOffset = s32PcmOutBufSize;
    }

    *u32MaxOutSize = s32PcmOutBufSize + s32BitsOutbufSize;

    return HI_SUCCESS;
}


static HI_S32 ADECResetDecoder(HI_AUDDATATYPE* pHAData)
{
    HI_S32      s32Ret;

    if (pHAData->hDecoder)
    {
        ADECDeInit(pHAData->hDecoder, &pHAData->stAdec);

        s32Ret = ADECInit(&pHAData->hDecoder, &pHAData->stAdec);
        if (HI_SUCCESS != s32Ret)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "ADECInit Failed!\n");
            return HI_FALSE;
        }
    }
    return HI_SUCCESS;
}


static HI_VOID ADECUpdateEOSState(HI_AUDDATATYPE*       pHAData,
                                  OMX_BUFFERHEADERTYPE* pInBufHdr,
                                  OMX_BUFFERHEADERTYPE* pOutBufHdr,
                                  HI_S32                s32Ret)
{
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;

    if ((HI_FALSE == pstAdec->bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
    {
        if (HA_ErrorNone != s32Ret)
        {
            /* Flush Inner Buffer */
            if (0 != pstInnerBuf->s32Insize)
            {
                pstInnerBuf->s32Insize = 0;
                pHAData->bInnerBufFlag = HI_FALSE;
                pOutBufHdr->nFilledLen = pHAData->sPcm.nSize * pHAData->sPcm.nChannels * sizeof(HI_S16);
                memset(pOutBufHdr->pBuffer, 0, pOutBufHdr->nFilledLen);
            }

            pHAData->mEndOfInput = HI_TRUE;
        }
    }
    else
    {
        if (0 == pInBufHdr->nFilledLen)
        {
            pOutBufHdr->nFilledLen = 0;
            pHAData->mEndOfInput = HI_TRUE;
        }
    }

    return;
}

static HI_S32 ADECSetConfig(HI_AUDDATATYPE* pHAData, HI_VOID* pstConfigStructure)
{
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HI_HA_DECODE_S*   pstHA       = pstAdec->pstHA;

    if (!pstAdec || !pstConfigStructure)
    {
        return HA_ErrorInvalidParameter;
    }
    pstHA->DecSetConfig(pHAData->hDecoder, pstConfigStructure);

    return HI_SUCCESS;
}

//Unify PCM 24 bitdepth to 16 bitdepth
static HI_VOID ADECUnifyPCMBitdepth(HI_AUDDATATYPE* pHAData, HI_HADECODE_OUTPUT_S* pavOut, OMX_BUFFERHEADERTYPE* pOutBufHdr)
{
    HI_U32 i, Data;
    HI_U32* pu32Src;
    HI_U16* pu16Data;

    if (24 == pavOut->u32BitPerSample)
    {
        if (HI_NULL == pHAData->stAdec.pWkbuf)
        {
            pHAData->stAdec.pWkbuf = (HI_U16*)malloc(pavOut->u32PcmOutSamplesPerFrame * pHAData->sPcm.nChannels * sizeof(HI_U16));
        }

        if (pHAData->stAdec.pWkbuf)
        {
            pu16Data = pHAData->stAdec.pWkbuf;
            pu32Src  = (HI_U32*)(pOutBufHdr->pBuffer);
            for (i = 0; i < pavOut->u32PcmOutSamplesPerFrame * pHAData->sPcm.nChannels; i++)
            {
                Data   = *pu32Src;
                Data >>= 16;
                *pu16Data = (HI_U16)Data;
                pu16Data++;
                pu32Src++;
            }

            memcpy(pOutBufHdr->pBuffer, pHAData->stAdec.pWkbuf, pOutBufHdr->nFilledLen);
        }
    }
}

//Output bitstream passthrough data from sdk
static HI_VOID ADECOutputPassthrough(HI_AUDDATATYPE* pHAData, HI_HADECODE_OUTPUT_S* pavOut, OMX_BUFFERHEADERTYPE* pOutBufHdr)
{
#ifdef DOLBY_OMX_PASSTHRU_SUPPORT
    HI_S32 s32Ret;
    HI_UNF_AO_FRAMEINFO_S stAOFrame;
    memset(&stAOFrame, 0, sizeof(HI_UNF_AO_FRAMEINFO_S));
    while (1)
    {
        stAOFrame.u32FrameIndex         = 0;
        stAOFrame.s32BitPerSample       = pavOut->u32BitPerSample;
        stAOFrame.bInterleaved          = HI_TRUE;
        stAOFrame.u32PcmSamplesPerFrame = 0;
        stAOFrame.u32Channels           = pHAData->sPcm.nChannels;
        stAOFrame.u32SampleRate         = pHAData->sPcm.nSamplingRate;
        stAOFrame.u32PtsMs              = pOutBufHdr->nTimeStamp;
        //stAOFrame.ps32PcmBuffer         = pavOut->ps32PcmOutBuf;
        stAOFrame.ps32BitsBuffer        = pavOut->ps32BitsOutBuf;
        stAOFrame.u32BitsBytesPerFrame  = pavOut->u32BitsOutBytesPerFrame;

        s32Ret = HI_MPI_AO_Track_SendData(pHAData->stAdec.hTrack, &stAOFrame);
        if (HI_SUCCESS == s32Ret)
        {
            //memset(pOutBufHdr->pBuffer, 0, pOutBufHdr->nFilledLen);
            break;
        }
        else if (HI_FAILURE == s32Ret)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HI_UNF_SND_SendTrackData error\n");
            break;
        }
        else
        {
            usleep(1000);
        }
    }
#endif
}


/*****************************************************************************
      The following functions with HA_XXX can be called by other files

*****************************************************************************/
HI_S32 HA_ADEC_DecodeFrame(HI_AUDDATATYPE* pHAData,     HI_HADECODE_INPACKET_S* avpkt,
                           HI_HADECODE_OUTPUT_S* avOut, OMX_BUFFERHEADERTYPE* pInBufHdr)
{
    HI_S32            s32Ret;
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HI_HA_DECODE_S*   pstHA       = pstAdec->pstHA;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;

    if ((HI_FALSE == pstAdec->bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
    {
        if (pstInnerBuf->s32Insize + pInBufHdr->nFilledLen > PACKETINSIZE_MAXNUM * pHAData->sInPortDef.nBufferSize)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "FAILED: Inner Buffer OVERFLOW\n");
            return HA_ErrorInBufFull;
        }
        if (pInBufHdr->nFilledLen > 0)
        {
            memcpy((HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Insize, pInBufHdr->pBuffer + pInBufHdr->nOffset, pInBufHdr->nFilledLen);
            pstInnerBuf->s32Insize += pInBufHdr->nFilledLen;
            pstInnerBuf->s32Offset  = 0;
            pInBufHdr->nOffset      = pInBufHdr->nFilledLen;
            pInBufHdr->nFilledLen   = 0;
        }
        avpkt->s32Size = pstInnerBuf->s32Insize;
        avpkt->pu8Data = (HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Offset;
    }
    else
    {
        avpkt->s32Size = pInBufHdr->nFilledLen;
        avpkt->pu8Data = pInBufHdr->pBuffer + pInBufHdr->nOffset;
    }

    s32Ret = pstHA->DecDecodeFrame(pHAData->hDecoder, avpkt, avOut);

#ifdef ADEC_FILE_SAVE
    ADECDumpFile(pHAData, avpkt, avOut, pInBufHdr);
#endif

    if ((HI_FALSE == pstAdec->bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
    {
        pstInnerBuf->s32Offset += (pstInnerBuf->s32Insize - avpkt->s32Size);
        pstInnerBuf->s32Insize  = avpkt->s32Size;
    }
    else
    {
        pInBufHdr->nOffset   += (pInBufHdr->nFilledLen - avpkt->s32Size);
        pInBufHdr->nFilledLen = avpkt->s32Size;
    }

    return s32Ret;
}


HI_S32  HA_OMX_InitDecoder(HI_AUDDATATYPE* pHAData)
{
    HI_S32            s32Ret;
    HI_U32            u32MaxOutSizes;
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;

    pHAData->bFirstFrame   = HI_TRUE;
    pHAData->bInnerBufFlag = HI_FALSE;
    pHAData->stAdec.pWkbuf = HI_NULL;
    s32Ret = ADECInit(&pHAData->hDecoder, pstAdec);
    if (HI_SUCCESS != s32Ret)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "ADECInit Failed!\n");
        return HI_FAILURE;
    }

    if (HI_FALSE == pstAdec->bPacketDecoder)
    {
        if (pstInnerBuf->pInBuffer == NULL)
        {
            pstInnerBuf->pInBuffer = (HI_VOID*)malloc(PACKETINSIZE_MAXNUM * pHAData->sInPortDef.nBufferSize);
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HADEC_Init pInBuffer is not Null!\n");
        }
        pstInnerBuf->s32Insize = 0;
        pstInnerBuf->s32Offset = 0;
    }

    s32Ret = ADECGetMaxOutSize(pHAData->hDecoder, pstAdec, &u32MaxOutSizes);
    if (HI_SUCCESS != s32Ret)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "ADECGetMaxOutSize Failed!\n");
        return HI_FAILURE;
    }
    pHAData->sOutPortDef.nBufferSize = (OMX_U32)(u32MaxOutSizes);

#ifdef ADEC_FILE_SAVE
    OPENDumpFile(pHAData);
#endif

    OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_InitDecoder Success!\n");

    return HI_SUCCESS;
}

HI_VOID  HA_OMX_DeInitDecoder(HI_AUDDATATYPE* pHAData)
{
    HI_S32            s32Ret;
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;

#ifdef ADEC_FILE_SAVE
    CLOSEDumpFile(pHAData);
#endif

    ADECDeInit(pHAData->hDecoder, pstAdec);

    if (pHAData->stAdec.pWkbuf)
    {
        free(pHAData->stAdec.pWkbuf);
        pHAData->stAdec.pWkbuf = HI_NULL;
    }

    if (HI_FALSE == pstAdec->bPacketDecoder)
    {
        if (pstInnerBuf->pInBuffer)
        {
            free(pstInnerBuf->pInBuffer);
            pstInnerBuf->pInBuffer = HI_NULL;
        }
    }

    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_OMX_DeInitDecoder Success!\n");

    return;
}

HI_VOID HA_OMX_FlushInnerState(HI_AUDDATATYPE* pHAData)
{
    TRP_IN();

    HA_ADEC_S* pstAdec = &pHAData->stAdec;

    pHAData->bFirstFrame    = HI_TRUE;
    pHAData->enInBufState   = OWNED_BY_NULL;
    pHAData->bNewPacketIn   = HI_FALSE;
    pHAData->bInnerBufFlag  = HI_FALSE;
    pHAData->mEndOfInput    = HI_FALSE;
    pHAData->mInputBufferCnt   = 0;
    if (HI_FALSE == pstAdec->bPacketDecoder)
    {
        pstAdec->sInternalBuf.s32Insize = 0;
        pstAdec->sInternalBuf.s32Offset = 0;
    }

    ADECResetDecoder(pHAData);

    TRP_OUT();
    return;
}

HI_S32 HA_OMX_SetConfig(HI_AUDDATATYPE*       pHAData,
                        HI_VOID* pstConfigStructure)
{
    return ADECSetConfig(pHAData, pstConfigStructure);
}

HI_S32 HA_OMX_DecodeFrame(HI_AUDDATATYPE*       pHAData,
                          OMX_BUFFERHEADERTYPE* pInBufHdr,
                          OMX_BUFFERHEADERTYPE* pOutBufHdr)
{
    HI_S32            s32Ret      = HA_ErrorNone;
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HI_HA_DECODE_S*   pstHA       = pstAdec->pstHA;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;
    HI_HADECODE_INPACKET_S avpkt;
    HI_HADECODE_OUTPUT_S   avOut;

    /* Get the time stamp */
    if (HI_TRUE == pHAData->bNewPacketIn)
    {
        pHAData->bNewPacketIn  = HI_FALSE;
        if(0 <= pInBufHdr->nTimeStamp)
        {
            pHAData->mAnchorTimeUs = pInBufHdr->nTimeStamp;
        }
    }

    pOutBufHdr->nFlags    = pInBufHdr->nFlags & (~OMX_BUFFERFLAG_EOS);
    pHAData->enInBufState = OWNED_BY_US;

    /*
       if 'pstAdec->u32BitstreamOffset = 0' means that only output pcm or passthrough
       data at the same time
    */
    /* The memory for output pcm data */
    avOut.ps32PcmOutBuf  = (HI_S32*)pOutBufHdr->pBuffer;
    /* The memory for output passthrough data */
    avOut.ps32BitsOutBuf = (HI_S32*)(pOutBufHdr->pBuffer + pstAdec->u32BitstreamOffset);

    /* Begin to decode */
    s32Ret = HA_ADEC_DecodeFrame(pHAData, &avpkt, &avOut, pInBufHdr);
    if (s32Ret == HA_ErrorNone)
    {

        /* Change channel and samplerate information */
        if ((pHAData->sPcm.nSamplingRate != avOut.u32OutSampleRate) || (pHAData->sPcm.nChannels != avOut.u32OutChannels))
        {
            pHAData->sPcm.nSamplingRate = avOut.u32OutSampleRate;
            pHAData->sPcm.nChannels     = avOut.u32OutChannels;
            pHAData->bPortSetChange     = HI_TRUE;
        }

        if ((pstAdec->bPassThrough == HI_TRUE) && (pstAdec->bPassThroughOutputEnable == HI_TRUE))
        {
            //Do passthrough for dolby/dts
            ADECOutputPassthrough(pHAData, &avOut, pOutBufHdr);
        }
        //Unify PCM to 16 bitdepth
        ADECUnifyPCMBitdepth(pHAData, &avOut, pOutBufHdr);
        if (0 == avOut.u32PcmOutSamplesPerFrame)
        {
            if (avOut.u32OutChannels)
            {
                avOut.u32PcmOutSamplesPerFrame = DOLBY_PCM_PER_FRAME;
                memset(avOut.ps32BitsOutBuf, 0, DOLBY_PCM_PER_FRAME * sizeof(HI_S16) * avOut.u32OutChannels);
            }
        }

        pOutBufHdr->nFilledLen  = avOut.u32PcmOutSamplesPerFrame * sizeof(HI_S16) * avOut.u32OutChannels;
        pHAData->sPcm.nSize     = avOut.u32PcmOutSamplesPerFrame;
        pOutBufHdr->nTimeStamp  = pHAData->mAnchorTimeUs;
        pHAData->mAnchorTimeUs += avOut.u32PcmOutSamplesPerFrame * 1000000 / avOut.u32OutSampleRate;
    }
    else
    {
        pHAData->enInBufState = OWNED_BY_COMPONENT;

        if ((HI_FALSE == pstAdec->bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
        {
            if ((0 != pstInnerBuf->s32Insize) && (0 != pstInnerBuf->s32Offset))
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Inner Buffer NotEnoughData\n");
                memmove((HI_U8*)pstInnerBuf->pInBuffer, (HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Offset, pstInnerBuf->s32Insize);
            }
        }
        else
        {
            if ((0 != pInBufHdr->nFilledLen) && (HI_FALSE == pstAdec->bPacketDecoder))
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "New packet -> Inner (length=%ld)\n", pInBufHdr->nFilledLen);
                memcpy(pstInnerBuf->pInBuffer, pInBufHdr->pBuffer + pInBufHdr->nOffset, pInBufHdr->nFilledLen);
                pstInnerBuf->s32Insize = pInBufHdr->nFilledLen;
                pHAData->bInnerBufFlag = HI_TRUE;
            }
            pInBufHdr->nOffset   += pInBufHdr->nFilledLen;
            pInBufHdr->nFilledLen = 0;
        }

        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "ADECDecodeFrame NotEnoughData or Error s32Ret=0x%x\n", s32Ret);
    }

    /* Get a packet when all inbuf consumed to decode a frame */
    if ((HI_FALSE == pstAdec->bPacketDecoder) && (HI_TRUE == pHAData->bInnerBufFlag))
    {
        if (0 == pstInnerBuf->s32Insize)
        {
            pHAData->enInBufState  = OWNED_BY_COMPONENT;
            pHAData->bInnerBufFlag = HI_FALSE;
        }
    }
    else
    {
        if (0 == pInBufHdr->nFilledLen)
        {
            pHAData->enInBufState  = OWNED_BY_COMPONENT;
        }
    }

    /* OMX_BUFFERFLAG_EOS PROCESS */
    if (pInBufHdr->nFlags & OMX_BUFFERFLAG_EOS)
    {
        ADECUpdateEOSState(pHAData, pInBufHdr, pOutBufHdr, s32Ret);
    }

    return s32Ret;
}

HI_HA_DECODE_S* HA_OMX_RegisterDecoderLib(const HI_CHAR* pszDecoderDllName)
{
    HI_VOID*        pDllModule;
    HI_HA_DECODE_S* pEntry;

    /* load the decoder  */
    pDllModule = dlopen(pszDecoderDllName, RTLD_LAZY | RTLD_GLOBAL);
    if (pDllModule == HI_NULL)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed because dlopen fail %s\n",
                       pszDecoderDllName, dlerror());
        return HI_NULL;
    }

    /* Get a entry pointer to the "ha_audio_decode_entry" .  */
    pEntry = (HI_HA_DECODE_S*)dlsym(pDllModule, "ha_audio_decode_entry");
    if (pEntry == HI_NULL)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed because dlsym fail %s\n",
                       pszDecoderDllName, dlerror());
        (HI_VOID)dlclose(pDllModule);
        return HI_NULL;
    }

    /* check the loaded decoder  */
    if (HI_SUCCESS != ADECCheckHaDecoder(pEntry))
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed\n", pszDecoderDllName);
        (HI_VOID)dlclose(pDllModule);
        return HI_NULL;
    }

    pEntry->pDllModule = pDllModule;
    return (pEntry);
}


HI_VOID HA_OMX_UnRegisterDecoderLib(HI_AUDDATATYPE* pHAData)
{
    ADECUnRegisterDecoderLib(pHAData->stAdec.pstHA);

    return;
}

