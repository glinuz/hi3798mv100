/*=================================================

Open MAX   Component: Hisi Audio Decoder
This module contains
File:    OMX_AAC_Adec.c
Author:  Audio
Date:    2015

=================================================*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_ADEC_AAC"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "unistd.h"

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Index.h>
#include <OMX_IndexExt.h>
#include <OMX_Image.h>
#include <OMX_Audio.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <OMX_Other.h>

#include "OMX_OSAL_Interfaces.h"
#include "OMX_CONF_StubbedComponent.h"
#include "ha_adec.h"
#include "omx_audio_base.h"
#include "HA.AUDIO.AAC.decode.h"

/*
 *  Declarations
 */

/* defination related to aac decoder */
#define AUDIO_DEC_AAC_ROLE    "audio_decoder.aac"
#define SIZE_IN_BUFFERS_AAC   2048     // Input  Buffers Size
static const HI_CHAR* g_szHA_AACDecoder = "libHA.AUDIO.AAC.decode.so";

/*
 *  Functions
*/

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_AAC_GetParameter(OMX_IN OMX_HANDLETYPE hComponent,
                                      OMX_IN OMX_INDEXTYPE  nParamIndex,
                                      OMX_INOUT OMX_PTR     ComponentParameterStructure)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, ComponentParameterStructure, 1);

    if (pHAData->state == OMX_StateReserved_0x00000000)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    switch (nParamIndex)
    {
        case OMX_IndexParamPortDefinition:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamPortDefinition\n", __FUNCTION__);
            if (((OMX_PARAM_PORTDEFINITIONTYPE*)(ComponentParameterStructure))->nPortIndex
                == pHAData->sInPortDef.nPortIndex)
            {
                memcpy(ComponentParameterStructure, &pHAData->sInPortDef, sizeof
                       (OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else if (((OMX_PARAM_PORTDEFINITIONTYPE*)
                      (ComponentParameterStructure))->nPortIndex == pHAData->sOutPortDef.nPortIndex)
            {
                memcpy(ComponentParameterStructure, &pHAData->sOutPortDef, sizeof
                       (OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else
            {
                eError = OMX_ErrorBadPortIndex;
            }
            break;

        case OMX_IndexParamAudioAac:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAac\n", __FUNCTION__);
            memcpy(ComponentParameterStructure, (OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam,
                   sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
            break;

        case OMX_IndexParamAudioPcm:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams = (OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;
            if (pcmParams->nPortIndex != OMX_OUT_PORT_IDX)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorUndefined\n", __FUNCTION__);
                return OMX_ErrorUndefined;
            }
            pcmParams->eNumData           = OMX_NumericalDataSigned;
            pcmParams->eEndian            = OMX_EndianLittle;
            pcmParams->bInterleaved       = OMX_TRUE;
            pcmParams->nBitPerSample      = 16;
            pcmParams->ePCMMode           = OMX_AUDIO_PCMModeLinear;
            pcmParams->eChannelMapping[0] = OMX_AUDIO_ChannelLF;
            pcmParams->eChannelMapping[1] = OMX_AUDIO_ChannelRF;
            pcmParams->eChannelMapping[2] = OMX_AUDIO_ChannelCF;
            pcmParams->eChannelMapping[3] = OMX_AUDIO_ChannelLFE;
            pcmParams->eChannelMapping[4] = OMX_AUDIO_ChannelLS;
            pcmParams->eChannelMapping[5] = OMX_AUDIO_ChannelRS;
            pcmParams->nChannels          = pHAData->sPcm.nChannels;
            pcmParams->nSamplingRate      = pHAData->sPcm.nSamplingRate;
            break;

        default:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorUnsupportedIndex\n", __FUNCTION__);
            eError = OMX_ErrorUnsupportedIndex;
            break;
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_AAC_SetParameter(OMX_IN OMX_HANDLETYPE hComponent,
                                      OMX_IN OMX_INDEXTYPE  nIndex,
                                      OMX_IN OMX_PTR        ComponentParameterStructure)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, ComponentParameterStructure, 1);

    if (pHAData->state != OMX_StateLoaded)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    switch (nIndex)
    {
        case OMX_IndexParamStandardComponentRole:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamStandardComponentRole\n", __FUNCTION__);
            const OMX_PARAM_COMPONENTROLETYPE* roleParams =
                (const OMX_PARAM_COMPONENTROLETYPE*)ComponentParameterStructure;
            if (strncmp((const char*)roleParams->cRole, "audio_decoder.aac", OMX_MAX_STRINGNAME_SIZE - 1))
            {
                eError = OMX_ErrorUndefined;
            }
            break;

        case OMX_IndexParamPortDefinition:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamPortDefinition\n", __FUNCTION__);
            if (((OMX_PARAM_PORTDEFINITIONTYPE*)(ComponentParameterStructure))->nPortIndex
                == pHAData->sInPortDef.nPortIndex)
            {
                memcpy(&pHAData->sInPortDef, ComponentParameterStructure, sizeof
                       (OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else if (((OMX_PARAM_PORTDEFINITIONTYPE*)
                      (ComponentParameterStructure))->nPortIndex ==
                     pHAData->sOutPortDef.nPortIndex)
            {
                memcpy(&pHAData->sOutPortDef, ComponentParameterStructure, sizeof
                       (OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorBadPortIndex\n", __FUNCTION__);
                eError = OMX_ErrorBadPortIndex;
            }
            break;

        case OMX_IndexParamAudioAac:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAac\n", __FUNCTION__);
            OMX_AUDIO_AACSTREAMFORMATTYPE enAACStreamFormat =
                   ((OMX_AUDIO_PARAM_AACPROFILETYPE*)ComponentParameterStructure)->eAACStreamFormat;
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s   AACStreamFormat:0x%x\n", __FUNCTION__, enAACStreamFormat);
            memcpy((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam, ComponentParameterStructure,
                   sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
#if 0 //set streamtype by decodeframe, because of the behavior of reset when seek.
            if (OMX_AUDIO_AACStreamFormatMP4FF == enAACStreamFormat)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ADIF without header, but need configraw\n", __FUNCTION__);
            }
            else //if (OMX_AUDIO_AACStreamFormatMP4ADTS == enAACStreamFormat)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ADTS need header\n", __FUNCTION__);
            }
#endif
            break;
        case OMX_IndexParamAudioPcm:
            //TODO
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            break;

        case OMX_IndexParamAudioAndroidAacPresentation:
            //TODO
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAndroidAacPresentation  ------TODO\n", __FUNCTION__);
            break;


        default:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorUnsupportedIndex\n", __FUNCTION__);
            eError = OMX_ErrorUnsupportedIndex;
            break;
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

OMX_ERRORTYPE HI_OMX_AAC_ComponentRoleEnum(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_U8*       cRole,
        OMX_IN OMX_U32        nIndex)
{
    TRP_IN();
    if (nIndex == 0)
    {
        strncpy((char*)cRole, AUDIO_DEC_AAC_ROLE, sizeof(AUDIO_DEC_AAC_ROLE));
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s -> Fail: Can not get HI_OMX_AAC_ComponentRoleEnum!\n", __FUNCTION__);
        return OMX_ErrorUnsupportedIndex;
    }

    TRP_OUT();
    return OMX_ErrorNone;
}

static HI_VOID HI_OMX_AAC_UpdateEOSState(HI_AUDDATATYPE*       pHAData,
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

HI_S32 HI_OMX_AAC_DecodeFrame(HI_AUDDATATYPE*       pHAData,
                          OMX_BUFFERHEADERTYPE* pInBufHdr,
                          OMX_BUFFERHEADERTYPE* pOutBufHdr)
{
    HI_S32            s32Ret      = HA_ErrorNone;
    HA_ADEC_S*        pstAdec     = &pHAData->stAdec;
    HI_HA_DECODE_S*   pstHA       = pstAdec->pstHA;
    HA_INTERNALBUF_S* pstInnerBuf = &pstAdec->sInternalBuf;
    HI_HADECODE_INPACKET_S avpkt;
    HI_HADECODE_OUTPUT_S   avOut;

    avOut.ps32PcmOutBuf  = (HI_S32*)pOutBufHdr->pBuffer;
    /* To check whether output passthrough data */
    if (pstAdec->bPassThrough == HI_TRUE)
    {
        avOut.ps32BitsOutBuf = (HI_S32*)(pOutBufHdr->pBuffer + pstAdec->u32BitstreamOffset);
    }

    if(0 == pHAData->mInputBufferCnt && !(OMX_BUFFERFLAG_CODECCONFIG & pInBufHdr->nFlags))
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "first buffer should have OMX_BUFFERFLAG_CODECCONFIG set");
        pInBufHdr->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
    }

    if(0 != (pInBufHdr->nFlags & OMX_BUFFERFLAG_CODECCONFIG))
    {
        if(0 == pHAData->mInputBufferCnt) //likely
        {
            HA_AAC_SET_TRANSPORT_TYPE_S stType;
            OMX_AUDIO_AACSTREAMFORMATTYPE enAACStreamFormat =
                ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eAACStreamFormat;

            if (OMX_AUDIO_AACStreamFormatMP4FF == enAACStreamFormat)
            { //without adts, default adif
                //step 1
                HI_UCHAR* inBuffer[FILEREAD_MAX_LAYERS];
                HI_U32 inBufferLength[FILEREAD_MAX_LAYERS] = {0};

                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s setStreamFormat TT_MP4_ADIF\n", __FUNCTION__);
                stType.enCmd = HA_CODEC_AAC_SET_TRANSPORT_TYPE_CMD;
                stType.stType = HA_TT_MP4_ADIF;
                s32Ret = HA_OMX_SetConfig(pHAData, &stType);
                if(HI_SUCCESS != s32Ret)
                {
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Config Code failed 0x%x", s32Ret);
                    return s32Ret;
                }
                //step 2, configraw only for adif
                inBuffer[0] = pInBufHdr->pBuffer + pInBufHdr->nOffset;
                inBufferLength[0] = pInBufHdr->nFilledLen;
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Config Code   0x%x 0x%x 0x%x 0x%x   size=0x%x",
                            *(inBuffer[0]), *(inBuffer[0] + 1), *(inBuffer[0] + 2), *(inBuffer[0] + 3), inBufferLength[0]);

                HA_AAC_SET_CONFIGRAW_S    stConfigData;
                memset(&stConfigData, 0, sizeof(HA_AAC_SET_CONFIGRAW_S));
                stConfigData.enCmd = HA_CODEC_AAC_SET_CONFIGRAW_CMD;
                stConfigData.conf[0] = inBuffer[0];
                stConfigData.length[0] = inBufferLength[0];

                s32Ret = HA_OMX_SetConfig(pHAData, &stConfigData);
                if(HI_SUCCESS != s32Ret)
                {
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Config Code failed 0x%x", s32Ret);
                    return s32Ret;
                }
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "Config Code Ok");
            }
            else //if (OMX_AUDIO_AACStreamFormatMP4ADTS == enAACStreamFormat)
            {//with adts, discard configraw
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ADTS need header\n", __FUNCTION__);
            }
        }

        pInBufHdr->nOffset      = pInBufHdr->nFilledLen;
        pInBufHdr->nFilledLen   = 0;
        pHAData->enInBufState  = OWNED_BY_COMPONENT;
    }

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


    //OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "DecDecodeFrame in avpkt.s32Size:0x%x  pInBufHdr->nFilledLen:0x%x\n", avpkt.s32Size, pInBufHdr->nFilledLen);

    /* Begin to decode */
    s32Ret = HA_ADEC_DecodeFrame(pHAData, &avpkt, &avOut, pInBufHdr);
    if (s32Ret == HA_ErrorNone)
    {
        pOutBufHdr->nFilledLen  = avOut.u32PcmOutSamplesPerFrame * sizeof(HI_S16) * avOut.u32OutChannels;
        pHAData->sPcm.nSize     = avOut.u32PcmOutSamplesPerFrame;
        pOutBufHdr->nTimeStamp  = pHAData->mAnchorTimeUs;
        pHAData->mAnchorTimeUs += avOut.u32PcmOutSamplesPerFrame * 1000000 / avOut.u32OutSampleRate;

        /* Change channel and samplerate information */
        if ((pHAData->sPcm.nSamplingRate != avOut.u32OutSampleRate) || (pHAData->sPcm.nChannels != avOut.u32OutChannels))
        {
            pHAData->sPcm.nSamplingRate = avOut.u32OutSampleRate;
            pHAData->sPcm.nChannels     = avOut.u32OutChannels;
            pHAData->bPortSetChange     = HI_TRUE;
        }

        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "ADECDecodeFrame Success\n");
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
    pHAData->mInputBufferCnt++;

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
        HI_OMX_AAC_UpdateEOSState(pHAData, pInBufHdr, pOutBufHdr, s32Ret);
    }

    return s32Ret;
}

/*****************************************************************************/
OMX_ERRORTYPE component_init(OMX_HANDLETYPE phandle, OMX_STRING comp_name)
{
    HI_S32  s32Ret;
    OMX_COMPONENTTYPE* p_Component;
    HI_AUDDATATYPE*    pHAData  = NULL;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_U32 err;
    OMX_U32 nIndex;
    TRP_IN();

    // Create private component data
    pHAData = (HI_AUDDATATYPE*)OMX_OSAL_Malloc(sizeof(HI_AUDDATATYPE));
    if (!pHAData)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "component_init OMX_OSAL_Malloc failed\n");
        return OMX_ErrorInsufficientResources;
    }
    memset(pHAData, 0x0, sizeof(HI_AUDDATATYPE));

    // Create private codec data
    pHAData->pCodecParam = (OMX_AUDIO_PARAM_AACPROFILETYPE*)OMX_OSAL_Malloc(sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
    if (!pHAData->pCodecParam)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecParam OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecParam, 0x0, sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));

    p_Component = (OMX_COMPONENTTYPE*)phandle;
    p_Component->pComponentPrivate      = (OMX_PTR)pHAData;
    pHAData->state                      = OMX_StateLoaded;
    pHAData->hSelf                      = phandle;

    // Fill in function pointers
    p_Component->SetCallbacks           = HI_OMX_CODEC_SetCallbacks;
    p_Component->UseBuffer              = HI_OMX_CODEC_UseBuffer;
    p_Component->AllocateBuffer         = HI_OMX_CODEC_AllocateBuffer;
    p_Component->FreeBuffer             = HI_OMX_CODEC_FreeBuffer;
    p_Component->EmptyThisBuffer        = HI_OMX_CODEC_EmptyThisBuffer;
    p_Component->FillThisBuffer         = HI_OMX_CODEC_FillThisBuffer;
    p_Component->ComponentDeInit        = HI_OMX_CODEC_ComponentDeInit;
    p_Component->SendCommand            = HI_OMX_CODEC_SendCommand;
    p_Component->GetState               = HI_OMX_CODEC_GetState;
    p_Component->GetParameter           = HI_OMX_AAC_GetParameter;
    p_Component->SetParameter           = HI_OMX_AAC_SetParameter;
    p_Component->ComponentRoleEnum      = HI_OMX_AAC_ComponentRoleEnum;
    p_Component->GetComponentVersion    = StubbedGetComponentVersion;
    p_Component->GetConfig              = StubbedGetConfig;
    p_Component->SetConfig              = StubbedSetConfig;
    p_Component->GetExtensionIndex      = StubbedGetExtensionIndex;
    p_Component->ComponentTunnelRequest = StubbedComponentTunnelRequest;
    p_Component->UseEGLImage            = StubbedUseEGLImage;

    // Initialize the audio parameters for input port
    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sInPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
    pHAData->sInPortDef.nPortIndex = OMX_IN_PORT_IDX;
    pHAData->sInPortDef.bEnabled   = OMX_TRUE;
    pHAData->sInPortDef.bPopulated = OMX_FALSE;
    pHAData->sInPortDef.eDomain    = OMX_PortDomainAudio;
    pHAData->sInPortDef.format.audio.pDeprecated0 = (OMX_PTR)"audio/aac";
    pHAData->sInPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sInPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAAC;
    pHAData->sInPortDef.eDir               = OMX_DirInput;
    pHAData->sInPortDef.nBufferCountMin    = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferCountActual = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferSize = (OMX_U32)(SIZE_IN_BUFFERS_AAC);

    // Initialize the audio parameters for output port
    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sOutPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
    pHAData->sOutPortDef.nPortIndex = OMX_OUT_PORT_IDX;
    pHAData->sOutPortDef.bEnabled   = OMX_TRUE;
    pHAData->sOutPortDef.bPopulated = OMX_FALSE;
    pHAData->sOutPortDef.eDomain    = OMX_PortDomainAudio;
    pHAData->sOutPortDef.format.audio.pDeprecated0 = (OMX_PTR)"audio/linearpcm";
    pHAData->sOutPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sOutPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
    pHAData->sOutPortDef.eDir               = OMX_DirOutput;
    pHAData->sOutPortDef.nBufferCountMin    = NUM_OUT_BUFFERS;
    pHAData->sOutPortDef.nBufferCountActual = NUM_OUT_BUFFERS;

    pHAData->stAdec.bPassThrough   = HI_FALSE;
    pHAData->stAdec.bPacketDecoder = HI_FALSE;

    pHAData->mInputBufferCnt = 0;

    HA_AAC_DecGetDefalutOpenParam(&pHAData->stAdec.sOpenPram);

    pHAData->stAdec.pstHA = HA_OMX_RegisterDecoderLib(g_szHA_AACDecoder);
    if (HI_NULL == pHAData->stAdec.pstHA)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_RegisterDecoderLib(%s) Failed!\n", g_szHA_AACDecoder);
        goto EXIT;
    }

    s32Ret = HA_OMX_InitDecoder(pHAData);
    if (HI_SUCCESS != s32Ret)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_InitDecoder Failed!\n");
        goto EXIT;
    }

    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sPriorityMgmt, OMX_PRIORITYMGMTTYPE);

    OMX_CONF_INIT_STRUCT_PTR((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam, OMX_AUDIO_PARAM_AACPROFILETYPE);
    //Default values for Aac audio param port
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nPortIndex       = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nChannels        = 2;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nSampleRate      = 48000;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nBitRate         = 0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAudioBandWidth  = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nFrameLength     = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAACtools        = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAACERtools      = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eAACProfile      = OMX_AUDIO_AACObjectMain;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eAACStreamFormat = OMX_AUDIO_AACStreamFormatMP4ADTS;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eChannelMode     = OMX_AUDIO_ChannelModeStereo;

    //Default values for pcm param
    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sPcm, OMX_AUDIO_PARAM_PCMMODETYPE);
    pHAData->sPcm.nChannels     = 2;
    pHAData->sPcm.nSamplingRate = 48000;

    // Initialize the input buffer list.
    memset(&(pHAData->sInBufList), 0x0, sizeof(BufferList));
    pHAData->sInBufList.pBufHdr = (OMX_BUFFERHEADERTYPE**)
                                  OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE*) * pHAData->sInPortDef.nBufferCountActual);
    for (nIndex = 0; nIndex < pHAData->sInPortDef.nBufferCountActual; nIndex++)
    {
        pHAData->sInBufList.pBufHdr[nIndex] = NULL;
    }
    pHAData->sInBufList.nSizeOfList = 0;
    pHAData->sInBufList.nAllocSize  = 0;
    pHAData->sInBufList.nListEnd    = -1;
    pHAData->sInBufList.nWritePos   = -1;
    pHAData->sInBufList.nReadPos    = -1;
    pHAData->sInBufList.eDir        = OMX_DirInput;

    // Initialize the output buffer list
    memset(&(pHAData->sOutBufList), 0x0, sizeof(BufferList));
    pHAData->sOutBufList.pBufHdr = (OMX_BUFFERHEADERTYPE**)
                                   OMX_OSAL_Malloc (sizeof(OMX_BUFFERHEADERTYPE*) * pHAData->sOutPortDef.nBufferCountActual);
    for (nIndex = 0; nIndex < pHAData->sOutPortDef.nBufferCountActual; nIndex++)
    {
        pHAData->sOutBufList.pBufHdr[nIndex] = NULL;
    }
    pHAData->sOutBufList.nSizeOfList = 0;
    pHAData->sOutBufList.nAllocSize  = 0;
    pHAData->sOutBufList.nListEnd    = -1;
    pHAData->sOutBufList.nWritePos   = -1;
    pHAData->sOutBufList.nReadPos    = -1;
    pHAData->sOutBufList.eDir        = OMX_DirOutput;

    // Create the pipe used to send commands to the thread
    err = pipe((int*)(pHAData->cmdpipe));
    if (err)
    {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
    // Create the pipe used to send data to the thread
    err = pipe((int*)(pHAData->cmddatapipe));
    if (err)
    {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

    pHAData->CodecFrame = HI_OMX_AAC_DecodeFrame;

    // Create the component thread
    err = pthread_create(&pHAData->thread_id, NULL, HI_OMX_ADEC_ComponentThread, pHAData);
    if (err || !pHAData->thread_id)
    {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

EXIT:
    if (OMX_ErrorNone != eError)
    {
        if (pHAData->hDecoder)
        {
            HA_OMX_DeInitDecoder(pHAData);
            HA_OMX_UnRegisterDecoderLib(pHAData);
            pHAData->hDecoder = NULL;
        }
        if (pHAData->pCodecParam)
        {
            OMX_OSAL_Free(pHAData->pCodecParam);
        }
        if (pHAData)
        {
            OMX_OSAL_Free(pHAData);
        }
    }

    TRP_OUT();
    return eError;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* File EOF */
