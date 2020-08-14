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
#define LOG_TAG    "HIOMX_AENC_AAC"
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
#include <OMX_Image.h>
#include <OMX_Audio.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <OMX_Other.h>

#include "OMX_OSAL_Interfaces.h"
#include "OMX_CONF_StubbedComponent.h"
#include "ha_aenc.h"
#include "omx_audio_base.h"
#include "HA.AUDIO.AAC.encode.h"

/*
 *  Declarations
 */

/* defination related to aac decoder */
#define AUDIO_AACENC_ROLE        "audio_encoder.aac"
#define SIZE_INBUFFERS_AACENC    4096     // Input  Buffers Size

static const HI_CHAR* g_szHA_AACEncoder = "libHA.AUDIO.AAC.encode.so";

/*
 *  Struct
*/
typedef struct HA_AACENC_PRIVDATA
{
    AAC_ENC_CONFIG  sAACEncConfig;
} HA_AACENC_PRIVDATA;

/*
 *  Functions
*/

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_AACENC_GetParameter(OMX_IN OMX_HANDLETYPE hComponent,
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

        case OMX_IndexParamAudioPcm:
        {
            OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams = (OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;

            if (pcmParams->nPortIndex != 0)
            {
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

            pcmParams->nChannels     = ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nChannels;
            pcmParams->nSamplingRate = ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nSampleRate;
            break;
        }

        case OMX_IndexParamAudioAac:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAac\n", __FUNCTION__);
            memcpy(ComponentParameterStructure, (OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam,
                   sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
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
OMX_ERRORTYPE HI_OMX_AACENC_SetParameter(OMX_IN OMX_HANDLETYPE hComponent,
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
            if (strncmp((const char*)roleParams->cRole, "audio_encoder.aac", OMX_MAX_STRINGNAME_SIZE - 1))
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
                      (ComponentParameterStructure))->nPortIndex == pHAData->sOutPortDef.nPortIndex)
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
            //TODO, how to dynamic set parameters???
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAac\n", __FUNCTION__);
            memcpy((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam, ComponentParameterStructure,
                   sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE));
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

OMX_ERRORTYPE HI_OMX_AACENC_ComponentRoleEnum(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_U8*       cRole,
        OMX_IN OMX_U32        nIndex)
{
    TRP_IN();
    if (nIndex == 0)
    {
        strncpy((char*)cRole, AUDIO_AACENC_ROLE, sizeof(AUDIO_AACENC_ROLE));
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s -> Fail: Can not get HI_OMX_AACENC_ComponentRoleEnum!\n", __FUNCTION__);
        return OMX_ErrorUnsupportedIndex;
    }

    TRP_OUT();
    return OMX_ErrorNone;
}

/*****************************************************************************/
OMX_ERRORTYPE component_init(OMX_HANDLETYPE phandle, OMX_STRING comp_name)
{
    HI_S32  s32Ret;
    AAC_ENC_CONFIG*    pAACEncConfig;
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

    // Create private codec Param
    pHAData->pCodecPrivData = (HA_AACENC_PRIVDATA*)OMX_OSAL_Malloc(sizeof(HA_AACENC_PRIVDATA));
    if (!pHAData->pCodecPrivData)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecPrivData OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecPrivData, 0x0, sizeof(HA_AACENC_PRIVDATA));
    //pAACEncConfig poiter must be global variable
    pAACEncConfig = &((HA_AACENC_PRIVDATA*)(pHAData->pCodecPrivData))->sAACEncConfig;

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
    p_Component->GetParameter           = HI_OMX_AACENC_GetParameter;
    p_Component->SetParameter           = HI_OMX_AACENC_SetParameter;
    p_Component->ComponentRoleEnum      = HI_OMX_AACENC_ComponentRoleEnum;
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
    pHAData->sInPortDef.format.audio.pDeprecated0  = (OMX_PTR)"audio/raw";
    pHAData->sInPortDef.format.audio.pNativeRender = NULL;
    pHAData->sInPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sInPortDef.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
    pHAData->sInPortDef.eDir               = OMX_DirInput;
    pHAData->sInPortDef.nBufferCountMin    = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferCountActual = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferSize        = SIZE_INBUFFERS_AACENC;
    pHAData->sInPortDef.bBuffersContiguous = OMX_FALSE;
    pHAData->sInPortDef.nBufferAlignment   = 1;


    // Initialize the audio parameters for output port
    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sOutPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
    pHAData->sOutPortDef.nPortIndex = OMX_OUT_PORT_IDX;
    pHAData->sOutPortDef.bEnabled   = OMX_TRUE;
    pHAData->sOutPortDef.bPopulated = OMX_FALSE;
    pHAData->sOutPortDef.eDomain    = OMX_PortDomainAudio;
    pHAData->sOutPortDef.format.audio.pDeprecated0  = (OMX_PTR)"audio/aac";
    pHAData->sOutPortDef.format.audio.pNativeRender = NULL;
    pHAData->sOutPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sOutPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAAC;
    pHAData->sOutPortDef.eDir               = OMX_DirOutput;
    pHAData->sOutPortDef.nBufferCountMin    = NUM_OUT_BUFFERS;
    pHAData->sOutPortDef.nBufferCountActual = NUM_OUT_BUFFERS;
    pHAData->sOutPortDef.bBuffersContiguous = OMX_FALSE;
    pHAData->sOutPortDef.nBufferAlignment   = 2;

    HA_AAC_GetDefaultConfig(pAACEncConfig);
    //The following parameters decided by input stream, to modify dynamic
    pAACEncConfig->sampleRate  = 48000;
    pAACEncConfig->nChannelsIn = 2;

    HA_AAC_GetEncDefaultOpenParam(&pHAData->stAenc.sOpenPram, pAACEncConfig);

    pHAData->stAenc.pstHA = HA_OMX_RegisterEncoderLib(g_szHA_AACEncoder);
    if (HI_NULL == pHAData->stAenc.pstHA)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_RegisterEncoderLib(%s) Failed!\n", g_szHA_AACEncoder);
        goto EXIT;
    }

    s32Ret = HA_OMX_InitEncoder(pHAData);
    if (HI_SUCCESS != s32Ret)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_InitEncoder Failed!\n");
        goto EXIT;
    }

    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sPriorityMgmt, OMX_PRIORITYMGMTTYPE);


    OMX_CONF_INIT_STRUCT_PTR((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam, OMX_AUDIO_PARAM_AACPROFILETYPE);
    //Default values for Aac audio param port
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nPortIndex       = OMX_OUT_PORT_IDX;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nChannels        = pAACEncConfig->nChannelsOut;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nSampleRate      = pAACEncConfig->sampleRate;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nBitRate         = pAACEncConfig->bitRate;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAudioBandWidth  = pAACEncConfig->bandWidth;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nFrameLength     = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAACtools        = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->nAACERtools      = 0x0;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eAACProfile      = OMX_AUDIO_AACObjectLC;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eAACStreamFormat = OMX_AUDIO_AACStreamFormatMP4ADTS;
    ((OMX_AUDIO_PARAM_AACPROFILETYPE*)pHAData->pCodecParam)->eChannelMode     = OMX_AUDIO_ChannelModeStereo;

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

    pHAData->CodecFrame = HA_OMX_EncodeFrame;

    // Create the component thread
    err = pthread_create(&pHAData->thread_id, NULL, HI_OMX_AENC_ComponentThread, pHAData);
    if (err || !pHAData->thread_id)
    {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }

EXIT:
    if (OMX_ErrorNone != eError)
    {
        if (pHAData->hEncoder)
        {
            HA_OMX_DeInitEncoder(pHAData);
            HA_OMX_UnRegisterEncoderLib(pHAData);
            pHAData->hEncoder = NULL;
        }
        if (pHAData->pCodecParam)
        {
            OMX_OSAL_Free(pHAData->pCodecParam);
        }
        if (pHAData->pCodecPrivData)
        {
            OMX_OSAL_Free(pHAData->pCodecPrivData);
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
