/*=================================================

Open MAX   Component: Hisi Audio Decoder
This module contains
File:    OMX_DTS_Adec.c
Author:  Audio
Date:    2015

=================================================*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_ADEC_DTS"
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
#include "OMX_IndexExt.h"
#include "OMX_AudioExt.h"

#include "OMX_OSAL_Interfaces.h"
#include "OMX_CONF_StubbedComponent.h"
#include "ha_adec.h"
#include "omx_audio_base.h"
#include "HA.AUDIO.DTSHD.decode.h"

/*
 *  Declarations
 */

/* defination related to DTS decoder */
#define AUDIO_DEC_DTS_ROLE   "audio_decoder.dts"
#define GBL_MAXDDPFRMWRDS   2048
#define GBL_BYTESPERWRD     2

#define SIZE_IN_BUFFERS_DTS 2 * (GBL_MAXDDPFRMWRDS * GBL_BYTESPERWRD)
static const HI_CHAR* g_szHA_DTSDecoder = "libHA.AUDIO.DTSHD.decode.so";


typedef struct HA_DTS_PRIVDATA
{
    DTSHD_DECODE_OPENCONFIG_S  stConfig;
} HA_DTS_PRIVDATA;



/*
 *  Functions
 */

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_DTS_GetParameter(OMX_IN OMX_HANDLETYPE hComponent,
                                      OMX_IN OMX_INDEXTYPE  nParamIndex,
                                      OMX_INOUT OMX_PTR     ComponentParameterStructure)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
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
                memcpy(ComponentParameterStructure, &pHAData->sInPortDef,  sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else if (((OMX_PARAM_PORTDEFINITIONTYPE*)
                      (ComponentParameterStructure))->nPortIndex == pHAData->sOutPortDef.nPortIndex)
            {
                memcpy(ComponentParameterStructure, &pHAData->sOutPortDef, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorBadPortIndex\n", __FUNCTION__);
                eError = OMX_ErrorBadPortIndex;
            }
            break;

        case OMX_IndexParamAudioAndroidDts:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAndroidDts\n", __FUNCTION__);
            memcpy(ComponentParameterStructure, (OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam, sizeof(OMX_AUDIO_PARAM_ANDROID_DTSTYPE));
            break;

        case OMX_IndexParamAudioPcm:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams = (OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;
            if (pcmParams->nPortIndex > 1)
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
OMX_ERRORTYPE HI_OMX_DTS_SetParameter(OMX_IN OMX_HANDLETYPE hComponent,
                                      OMX_IN OMX_INDEXTYPE  nIndex,
                                      OMX_IN OMX_PTR        ComponentParameterStructure)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, ComponentParameterStructure, 1);

    if (pHAData->state != OMX_StateLoaded)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s -> OMX_ErrorIncorrectStateOperation\n", __FUNCTION__);
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    switch (nIndex)
    {
        case OMX_IndexParamStandardComponentRole:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamStandardComponentRole\n", __FUNCTION__);
            const OMX_PARAM_COMPONENTROLETYPE* roleParams =
                (const OMX_PARAM_COMPONENTROLETYPE*)ComponentParameterStructure;
            if (strncmp((const char*)roleParams->cRole, "audio_decoder.dts", OMX_MAX_STRINGNAME_SIZE - 1))
            {
                return OMX_ErrorUndefined;
            }
            break;

        case OMX_IndexParamAudioPcm:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            const OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams =
                (const OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;
            if (pcmParams->nPortIndex != 1)
            {
                return OMX_ErrorUndefined;
            }
            pHAData->sPcm.nChannels     = pcmParams->nChannels;
            pHAData->sPcm.nSamplingRate = pcmParams->nSamplingRate;
            break;

        case OMX_IndexParamPortDefinition:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamPortDefinition\n", __FUNCTION__);
            if (((OMX_PARAM_PORTDEFINITIONTYPE*)(ComponentParameterStructure))->nPortIndex
                == pHAData->sInPortDef.nPortIndex)
            {
                memcpy(&pHAData->sInPortDef, ComponentParameterStructure, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else if (((OMX_PARAM_PORTDEFINITIONTYPE*)
                      (ComponentParameterStructure))->nPortIndex ==
                     pHAData->sOutPortDef.nPortIndex)
            {
                memcpy(&pHAData->sOutPortDef, ComponentParameterStructure, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
            }
            else
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorBadPortIndex\n", __FUNCTION__);
                eError = OMX_ErrorBadPortIndex;
            }
            break;

        case OMX_IndexParamAudioAndroidDts:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioAndroidDts\n", __FUNCTION__);
            memcpy((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam, ComponentParameterStructure,
                   sizeof(OMX_AUDIO_PARAM_ANDROID_DTSTYPE));
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


OMX_ERRORTYPE HI_OMX_DTS_ComponentRoleEnum(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_U8*       cRole,
        OMX_IN OMX_U32        nIndex)
{
    TRP_IN();

    if (nIndex == 0)
    {
        strncpy((char*)cRole, AUDIO_DEC_DTS_ROLE, sizeof(AUDIO_DEC_DTS_ROLE));
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s -> Fail: Can not get HI_OMX_DTS_ComponentRoleEnum!\n", __FUNCTION__);
        return OMX_ErrorUnsupportedIndex;
    }

    TRP_OUT();
    return OMX_ErrorNone;
}


/*****************************************************************************/
OMX_ERRORTYPE component_init(OMX_HANDLETYPE phandle, OMX_STRING comp_name)
{
    HI_S32             s32Ret;
    OMX_COMPONENTTYPE* p_Component;
    HI_AUDDATATYPE*    pHAData  = NULL;
    OMX_ERRORTYPE      eError = OMX_ErrorNone;
    OMX_U32            err;
    OMX_U32            nIndex;
    DTSHD_DECODE_OPENCONFIG_S *pstConfig;
    TRP_IN();

    // Create private component data
    pHAData = (HI_AUDDATATYPE*)OMX_OSAL_Malloc(sizeof(HI_AUDDATATYPE));
    if (!pHAData)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "component_init OMX_OSAL_Malloc failed\n");
        return OMX_ErrorInsufficientResources;
    }
    memset(pHAData, 0x0, sizeof(HI_AUDDATATYPE));

    // Create private codec Param
    pHAData->pCodecParam = (OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)OMX_OSAL_Malloc(sizeof(OMX_AUDIO_PARAM_ANDROID_DTSTYPE));
    if (!pHAData->pCodecParam)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecData OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecParam, 0x0, sizeof(OMX_AUDIO_PARAM_ANDROID_DTSTYPE));

    // Create private codec Param
    pHAData->pCodecPrivData = (HA_DTS_PRIVDATA*)OMX_OSAL_Malloc(sizeof(HA_DTS_PRIVDATA));
    if (!pHAData->pCodecPrivData)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecPrivData OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecPrivData, 0x0, sizeof(HA_DTS_PRIVDATA));
    pstConfig = &((HA_DTS_PRIVDATA*)(pHAData->pCodecPrivData))->stConfig;

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
    p_Component->GetParameter           = HI_OMX_DTS_GetParameter;
    p_Component->SetParameter           = HI_OMX_DTS_SetParameter;
    p_Component->ComponentRoleEnum      = HI_OMX_DTS_ComponentRoleEnum;
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
    pHAData->sInPortDef.format.audio.pDeprecated0 = (OMX_PTR)"audio/mpeg";
    pHAData->sInPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sInPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAndroidDTS;
    pHAData->sInPortDef.eDir               = OMX_DirInput;
    pHAData->sInPortDef.nBufferCountMin    = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferCountActual = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferSize = (OMX_U32)(SIZE_IN_BUFFERS_DTS);

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

    pHAData->stAdec.bPassThrough   = HI_TRUE;
    pHAData->stAdec.bPacketDecoder = HI_FALSE;

    HA_DTSHD_DecGetDefalutOpenConfig(pstConfig);
    HA_DTSHD_DecGetDefalutOpenParam(&pHAData->stAdec.sOpenPram, pstConfig);
    pHAData->stAdec.sOpenPram.enDecMode = HD_DEC_MODE_SIMUL;

    pHAData->stAdec.pstHA = HA_OMX_RegisterDecoderLib(g_szHA_DTSDecoder);
    if (HI_NULL == pHAData->stAdec.pstHA)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_RegisterDecoderLib(%s) Failed!\n", g_szHA_DTSDecoder);
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

    OMX_CONF_INIT_STRUCT_PTR((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam, OMX_AUDIO_PARAM_ANDROID_DTSTYPE);
    //Default values for DTS audio param port
    ((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam)->nPortIndex      = 0x0;
    ((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam)->nChannels       = 2;
    ((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam)->nBitRate        = 0;
    ((OMX_AUDIO_PARAM_ANDROID_DTSTYPE*)pHAData->pCodecParam)->nSampleRate     = 44100;


    //Default values for pcm param
    OMX_CONF_INIT_STRUCT_PTR(&pHAData->sPcm, OMX_AUDIO_PARAM_PCMMODETYPE);
    pHAData->sPcm.nChannels     = 2;
    pHAData->sPcm.nSamplingRate = 44100;

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

    pHAData->CodecFrame = HA_OMX_DecodeFrame;

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
