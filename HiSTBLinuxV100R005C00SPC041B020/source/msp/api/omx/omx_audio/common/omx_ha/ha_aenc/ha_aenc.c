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
#define LOG_TAG    "HIOMX_HA_AENC"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <dlfcn.h>
#include <dirent.h>
#endif

#include <OMX_Core.h>

#include "HA.AUDIO.AAC.encode.h"
#include "hi_type.h"
#include "ha_aenc.h"
#include "hi_audio_codec.h"
#include "omx_audio_base.h"

#define PACKETINSIZE_MAXNUM    2
#define CHECK_AENC_NULL_PTR(ptr)                        \
    do                                                  \
    {                                                   \
        if (NULL == ptr)                                \
        {                                               \
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "invalid NULL poiner!\n"); \
            return HI_FAILURE;                          \
        }                                               \
    } while (0)


static HI_S32 AENCCheckHaEncoder(const HI_HA_ENCODE_S* pEntry)
{
    CHECK_AENC_NULL_PTR(pEntry->EncodeInit);
    CHECK_AENC_NULL_PTR(pEntry->EncodeDeInit);
    CHECK_AENC_NULL_PTR(pEntry->EncodeSetConfig);
    CHECK_AENC_NULL_PTR(pEntry->EncodeGetMaxBitsOutSize);
    CHECK_AENC_NULL_PTR(pEntry->EncodeFrame);

    return HI_SUCCESS;
}


static HI_VOID AENCUnRegisterEncoderLib(HI_HA_ENCODE_S* pEntry)
{
    HI_S32 dlok;
    dlok = dlclose(pEntry->pDllModule);
    if (dlok)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "dlclose fali\n");
    }
    return;
}

static HI_S32 AENCInit(HI_VOID** hEncoder, HA_AENC_S* pstAenc)
{
    HI_HA_ENCODE_S* pstHA = pstAenc->pstHA;

    pstHA->EncodeInit(hEncoder, &pstAenc->sOpenPram);
    if (HI_NULL == *hEncoder)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 AENCDeInit(HI_VOID* hEncoder, HA_AENC_S* pstAenc)
{
    HI_HA_ENCODE_S* pstHA = pstAenc->pstHA;

    pstHA->EncodeDeInit(hEncoder);

    return HI_SUCCESS;
}

static HI_S32 AENCGetMaxOutSize(HI_VOID*   hEncoder,
                                HA_AENC_S* pstAenc,
                                HI_U32*    u32MaxOutSize)
{
    HI_U32 s32BitsOutbufSize = 0;
    HI_HA_ENCODE_S* pstHA    = pstAenc->pstHA;

    if (!hEncoder)
    {
        return HA_ErrorInvalidParameter;
    }

    pstHA->EncodeGetMaxBitsOutSize(hEncoder, &s32BitsOutbufSize);

    *u32MaxOutSize = s32BitsOutbufSize;

    return HI_SUCCESS;
}

//TODO
static HI_S32 AENCResetEncoder(HI_AUDDATATYPE* pHAData)
{
    HI_S32      s32Ret;

    if (pHAData->hEncoder)
    {
        AENCDeInit(pHAData->hEncoder, &pHAData->stAenc);

        s32Ret = AENCInit(&pHAData->hEncoder, &pHAData->stAenc);
        if (HI_SUCCESS != s32Ret)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "AENCInit Failed\n");
            return HI_FALSE;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID AENCUpdateEOSState(HI_AUDDATATYPE*       pHAData,
                                  OMX_BUFFERHEADERTYPE* pInBufHdr,
                                  OMX_BUFFERHEADERTYPE* pOutBufHdr,
                                  HI_S32                s32Ret)
{
    HA_AENC_S*        pstAenc     = &pHAData->stAenc;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAenc->sInternalBuf;

    if (HI_TRUE == pHAData->bInnerBufFlag)
    {
        if (HA_ErrorNone != s32Ret)
        {
            /* Flush Inner Buffer */
            pstInnerBuf->s32Insize = 0;
            pHAData->bInnerBufFlag = HI_FALSE;

            pHAData->mEndOfInput   = HI_TRUE;
        }
    }
    else
    {
        if (0 == pInBufHdr->nFilledLen)
        {
            pOutBufHdr->nFilledLen = 0;
            pHAData->mEndOfInput   = HI_TRUE;
        }
    }

    return;
}

static HI_U32 AENCGetEncodeInDataSize(HI_HAENCODE_OPENPARAM_S* pstOpenPram)
{
    HI_U32 u32EncodeInDataSize = 0;

    if (16 == pstOpenPram->s32BitPerSample)
    {
        u32EncodeInDataSize = pstOpenPram->u32SamplePerFrame * pstOpenPram->u32DesiredOutChannels * sizeof(HI_U16);
    }
    else
    {
        u32EncodeInDataSize = pstOpenPram->u32SamplePerFrame * pstOpenPram->u32DesiredOutChannels * sizeof(HI_U32);
    }

    return u32EncodeInDataSize;
}

/*****************************************************************************
      The following functions with HA_XXX can be called by other files

*****************************************************************************/

HI_S32  HA_OMX_InitEncoder(HI_AUDDATATYPE* pHAData)
{
    HI_S32      s32Ret;
    HI_U32      u32MaxOutSizes;
    HA_AENC_S*  pstAenc = &pHAData->stAenc;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAenc->sInternalBuf;

    s32Ret = AENCInit(&pHAData->hEncoder, pstAenc);
    if (HI_SUCCESS != s32Ret)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "AENCInit Failed!\n");
        return HI_FAILURE;
    }

    if (pstInnerBuf->pInBuffer == NULL)
    {
        pstInnerBuf->pInBuffer = (HI_VOID*)malloc(PACKETINSIZE_MAXNUM * pHAData->sInPortDef.nBufferSize);
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "FAIL: pstInnerBuf->pInBuffer is not Null\n");
    }
    pstInnerBuf->s32Insize = 0;
    pstInnerBuf->s32Offset = 0;

    s32Ret = AENCGetMaxOutSize(pHAData->hEncoder, pstAenc, &u32MaxOutSizes);
    if (HI_SUCCESS != s32Ret)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "AENCGetMaxOutSize Failed!\n");
        return HI_FAILURE;
    }
    pHAData->sOutPortDef.nBufferSize = (OMX_U32)(u32MaxOutSizes);

    return HI_SUCCESS;
}

HI_VOID  HA_OMX_DeInitEncoder(HI_AUDDATATYPE* pHAData)
{
    HA_AENC_S*        pstAenc     = &pHAData->stAenc;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAenc->sInternalBuf;

    AENCDeInit(pHAData->hEncoder, pstAenc);

    if (pstInnerBuf->pInBuffer)
    {
        free(pstInnerBuf->pInBuffer);
        pstInnerBuf->pInBuffer = NULL;
    }

    return;
}

HI_VOID HA_OMX_FlushAencInnerState(HI_AUDDATATYPE* pHAData)
{
    TRP_IN();

    HA_AENC_S* pstAenc = &pHAData->stAenc;

    pHAData->enInBufState   = OWNED_BY_NULL;
    pHAData->bNewPacketIn   = HI_FALSE;
    pHAData->bInnerBufFlag  = HI_FALSE;
    pstAenc->sInternalBuf.s32Insize = 0;
    pstAenc->sInternalBuf.s32Offset = 0;

    AENCResetEncoder(pHAData);

    TRP_OUT();
    return;
}

HI_S32 HA_OMX_EncodeFrame(HI_AUDDATATYPE*       pHAData,
                          OMX_BUFFERHEADERTYPE* pInBufHdr,
                          OMX_BUFFERHEADERTYPE* pOutBufHdr)
{
    HI_S32                 s32Ret  = HA_ErrorNone;
    HI_U32                 u32EncodeInDataSize;
    HA_AENC_S*             pstAenc = &pHAData->stAenc;
    HI_HA_ENCODE_S*        pstHA   = pstAenc->pstHA;
    HA_INTERNALBUF_S*      pstInnerBuf = &pstAenc->sInternalBuf;
    HI_HAENCODE_INPACKET_S avpkt;
    HI_HAENCODE_OUTPUT_S   avOut;

    avOut.ps32BitsOutBuf = (HI_S32*)pOutBufHdr->pBuffer;

    /* Get the time stamp */
    if (HI_TRUE == pHAData->bNewPacketIn)
    {
        pHAData->bNewPacketIn  = HI_FALSE;
        if(0 <= pInBufHdr->nTimeStamp)
        {
            pHAData->mAnchorTimeUs = pInBufHdr->nTimeStamp;
        }
    }

    pHAData->enInBufState = OWNED_BY_US;

    /* check input buf have enough data to ecoder */
    u32EncodeInDataSize = AENCGetEncodeInDataSize(&pstAenc->sOpenPram);
    if (HI_FALSE == pHAData->bInnerBufFlag)
    {
        if (u32EncodeInDataSize > pInBufHdr->nFilledLen)
        {
            memcpy(pstInnerBuf->pInBuffer, pInBufHdr->pBuffer + pInBufHdr->nOffset, pInBufHdr->nFilledLen);
            pstInnerBuf->s32Insize = pInBufHdr->nFilledLen;
            pHAData->bInnerBufFlag = HI_TRUE;
            pInBufHdr->nOffset    += pInBufHdr->nFilledLen;
            pInBufHdr->nFilledLen  = 0;
            pHAData->enInBufState  = OWNED_BY_COMPONENT;
            s32Ret = HA_ErrorNotEnoughData;
            goto END;
        }

        avpkt.u32Size = pInBufHdr->nFilledLen;
        avpkt.pu8Data = pInBufHdr->pBuffer + pInBufHdr->nOffset;
    }
    else
    {
        if (pstInnerBuf->s32Insize + pInBufHdr->nFilledLen > PACKETINSIZE_MAXNUM * pHAData->sInPortDef.nBufferSize)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "FAILED: Inner Buffer OVERFLOW\n");
            s32Ret = HA_ErrorInBufFull;
            goto END;
        }

        if (0 < pInBufHdr->nFilledLen)
        {
            memcpy((HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Insize, pInBufHdr->pBuffer + pInBufHdr->nOffset, pInBufHdr->nFilledLen);
            pstInnerBuf->s32Insize += pInBufHdr->nFilledLen;
            pstInnerBuf->s32Offset  = 0;
            pInBufHdr->nOffset      = pInBufHdr->nFilledLen;
            pInBufHdr->nFilledLen   = 0;
        }

        if (u32EncodeInDataSize > pstInnerBuf->s32Insize)
        {
            if ((0 != pstInnerBuf->s32Insize) && (0 != pstInnerBuf->s32Offset))
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Inner Buffer NotEnoughData to Encode\n");
                memmove((HI_U8*)pstInnerBuf->pInBuffer, (HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Offset, pstInnerBuf->s32Insize);
            }
            pHAData->enInBufState = OWNED_BY_COMPONENT;
            s32Ret = HA_ErrorNotEnoughData;
            goto END;
        }

        avpkt.u32Size = pstInnerBuf->s32Insize;
        avpkt.pu8Data = (HI_U8*)pstInnerBuf->pInBuffer + pstInnerBuf->s32Offset;
    }


    /* ecoder one frame */
    s32Ret = pstHA->EncodeFrame(pHAData->hEncoder, &avpkt, &avOut);
    if (HA_ErrorNone == s32Ret)
    {
        pOutBufHdr->nFilledLen  = avOut.u32BitsOutBytesPerFrame;
        pOutBufHdr->nTimeStamp  = pHAData->mAnchorTimeUs;
        pOutBufHdr->nFlags      = OMX_BUFFERFLAG_ENDOFFRAME;
        pHAData->mAnchorTimeUs += (u32EncodeInDataSize * 1000000 / pstAenc->sOpenPram.u32DesiredSampleRate)
                                  / (pstAenc->sOpenPram.u32DesiredOutChannels * sizeof(HI_S16));
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "AENCEncodeFrame Success\n");
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "AENCEncodeFrame fail errCode=0x%x\n", s32Ret);
        s32Ret = HA_ErrorStreamCorrupt;
    }

    if (HI_FALSE == pHAData->bInnerBufFlag)
    {
        pInBufHdr->nOffset     += (pInBufHdr->nFilledLen - avpkt.u32Size);
        pInBufHdr->nFilledLen   = avpkt.u32Size;
        if (0 == pInBufHdr->nFilledLen)
        {
            pHAData->enInBufState  = OWNED_BY_COMPONENT;
        }
    }
    else
    {
        pstInnerBuf->s32Offset += (pstInnerBuf->s32Insize - avpkt.u32Size);
        pstInnerBuf->s32Insize  = avpkt.u32Size;
        if (0 == pstInnerBuf->s32Insize)
        {
            pHAData->enInBufState  = OWNED_BY_COMPONENT;
            pHAData->bInnerBufFlag = HI_FALSE;
        }
    }

END:
    /* OMX_BUFFERFLAG_EOS PROCESS */
    if (pInBufHdr->nFlags & OMX_BUFFERFLAG_EOS)
    {
        AENCUpdateEOSState(pHAData, pInBufHdr, pOutBufHdr, s32Ret);
    }

    return s32Ret;
}


HI_HA_ENCODE_S* HA_OMX_RegisterEncoderLib(const HI_CHAR* pszEncoderDllName)
{
    HI_VOID*        pDllModule;
    HI_HA_ENCODE_S* pEntry;

    /* If there is an error, we can't go on, so set the error code and exit */
    pDllModule = dlopen(pszEncoderDllName, RTLD_LAZY | RTLD_GLOBAL);
    if (pDllModule == HI_NULL)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed because dlopen fail %s\n\n", pszEncoderDllName, dlerror());
        return HI_NULL;
    }

    /* Get a entry pointer to the "ha_audio_decode_entry" .  If
     * there is an error, we can't go on, so set the error code and exit */
    pEntry = dlsym(pDllModule, "ha_audio_encode_entry");
    if (pEntry == HI_NULL)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed because dlsym fail %s\n", pszEncoderDllName, dlerror());
        (HI_VOID)dlclose(pDllModule);
        return HI_NULL;
    }

    if (HI_SUCCESS != AENCCheckHaEncoder(pEntry))
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Register %s Failed\n", pszEncoderDllName);
        (HI_VOID)dlclose(pDllModule);
        return HI_NULL;
    }

    pEntry->pDllModule = pDllModule;
    return (pEntry);
}

HI_VOID HA_OMX_UnRegisterEncoderLib(HI_AUDDATATYPE* pHAData)
{
    AENCUnRegisterEncoderLib(pHAData->stAenc.pstHA);

    return;
}

