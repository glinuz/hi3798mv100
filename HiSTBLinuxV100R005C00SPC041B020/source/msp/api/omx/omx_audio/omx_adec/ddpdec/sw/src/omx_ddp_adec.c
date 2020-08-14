/*=================================================

Open MAX   Component: Hisi Audio Decoder
This module contains
File:    OMX_DDP_Adec.c
Author:  Audio
Date:    2015

=================================================*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_ADEC_DDP"
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
#include <OMX_AudioExt.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <OMX_Other.h>

#include "OMX_OSAL_Interfaces.h"
#include "OMX_CONF_StubbedComponent.h"
#include "ha_adec.h"
#include "omx_audio_base.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "OMX_Audio_Hisi.h"

/*
 *  Declarations
 */

/* defination related to ddp decoder */
#define AUDIO_DEC_DDP_ROLE1 "audio_decoder.ac3"
#define AUDIO_DEC_DDP_ROLE2 "audio_decoder.ec3"

#define GBL_MAXDDPFRMWRDS   2048
#define GBL_BYTESPERWRD     2

/* SIZE_IN_BUFFERS_DDP is defined by assuming that the maximum amount of data that
 * could be carried over to the next buffer read cycle is 1 frame. It is set
 * to 2 frames for safety. This value is the
 * 2 * ( size of DD+ word (larger than DD) * num. bytes/word)
 */
#define SIZE_IN_BUFFERS_DDP 2 * (GBL_MAXDDPFRMWRDS * GBL_BYTESPERWRD)
static const HI_CHAR* g_szHA_DDPDecoder = "libHA.AUDIO.DOLBYPLUS.decode.so";
static const HI_CHAR* g_szHA_AC3PassThrough = "libHA.AUDIO.AC3PASSTHROUGH.decode.so";


typedef struct HA_DOLBY_PRIVDATA
{
    DOLBYPLUS_DECODE_OPENCONFIG_S  sDDPConfig;
    DOLBYPLUS_STREAM_INFO_S    sDDPStreamInfo;
} HA_DOLBY_PRIVDATA;

/*
 *  Functions
*/

static HI_VOID DDPlusCallBack(DOLBYPLUS_EVENT_E Event, HI_VOID* pUserData)
{
    DOLBYPLUS_STREAM_INFO_S* pstInfo = (DOLBYPLUS_STREAM_INFO_S*)pUserData;
#if 0
    printf( "DDPlusCallBack show info:\n \
                s16StreamType          = %d\n \
                s16Acmod               = %d\n \
                s32BitRate             = %d\n \
                s32SampleRateRate      = %d\n \
                Event                  = %d\n",
            pstInfo->s16StreamType, pstInfo->s16Acmod, pstInfo->s32BitRate, pstInfo->s32SampleRateRate, Event);
#endif

    return;
}


/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_DDP_GetParameter(OMX_IN OMX_HANDLETYPE hComponent,
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
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorBadPortIndex\n", __FUNCTION__);
                eError = OMX_ErrorBadPortIndex;
            }
            break;

        case OMX_HA_IndexParamAudioDolby:
            ((OMX_HA_PARAM_DOLBYTYPE*)(pHAData->pCodecParam))->eStreamType =
                (OMX_HA_DOLBYSTREAMTYPE)(((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s16StreamType);
            ((OMX_HA_PARAM_DOLBYTYPE*)(pHAData->pCodecParam))->eAcmod =
                (OMX_HA_DOLBYCHANCFG) (((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s16Acmod);

            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_HA_IndexParamAudioDolby StreamType:%d, ChanConfig:%d\n", __FUNCTION__,
                           ((OMX_HA_PARAM_DOLBYTYPE*)(pHAData->pCodecParam))->eStreamType,
                           ((OMX_HA_PARAM_DOLBYTYPE*)(pHAData->pCodecParam))->eAcmod);

            memcpy(ComponentParameterStructure, (OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam, sizeof(OMX_HA_PARAM_DOLBYTYPE));
            break;


        case OMX_IndexParamAudioPcm:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams =
                (OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;
            if (pcmParams->nPortIndex > OMX_OUT_PORT_IDX)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_ErrorUndefined\n", __FUNCTION__);
                return OMX_ErrorUndefined;
            }
            pcmParams->eNumData = OMX_NumericalDataSigned;
            pcmParams->eEndian = OMX_EndianLittle;
            pcmParams->bInterleaved = OMX_TRUE;
            pcmParams->nBitPerSample = 16;
            pcmParams->ePCMMode = OMX_AUDIO_PCMModeLinear;
            pcmParams->eChannelMapping[0] = OMX_AUDIO_ChannelLF;
            pcmParams->eChannelMapping[1] = OMX_AUDIO_ChannelRF;
            pcmParams->eChannelMapping[2] = OMX_AUDIO_ChannelCF;
            pcmParams->eChannelMapping[3] = OMX_AUDIO_ChannelLFE;
            pcmParams->eChannelMapping[4] = OMX_AUDIO_ChannelLS;
            pcmParams->eChannelMapping[5] = OMX_AUDIO_ChannelRS;
            pcmParams->eChannelMapping[6] = OMX_AUDIO_ChannelLR;
            pcmParams->eChannelMapping[7] = OMX_AUDIO_ChannelRR;
            pcmParams->nChannels     = pHAData->sPcm.nChannels;
            pcmParams->nSamplingRate = pHAData->sPcm.nSamplingRate;
            break;

        case OMX_IndexParamAudioAndroidAc3:
        case OMX_IndexParamAudioAndroidEac3:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> TOCHECK  0x%x\n", __FUNCTION__, nParamIndex);
            break;
        case OMX_IndexParamAudioDolbyDrcMode:
        case OMX_IndexParamAudioDolbyDrcRange:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> TOCHECK  0x%x\n", __FUNCTION__, nParamIndex);
            break;
        case OMX_IndexParamAudioDolbyStreamInfo:
            {
                OMX_HA_PARAM_DOLBYSTREAMINFO* pDolbyStreamInfo = (OMX_HA_PARAM_DOLBYSTREAMINFO* )ComponentParameterStructure;
                pDolbyStreamInfo->nAcmod = (OMX_HA_DOLBYSTREAMTYPE)(((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s16Acmod);
                pDolbyStreamInfo->nBitRate = (OMX_HA_DOLBYSTREAMTYPE)(((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s32BitRate);
                pDolbyStreamInfo->nSampleRate = (OMX_HA_DOLBYSTREAMTYPE)(((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s32SampleRateRate);
                pDolbyStreamInfo->nStreamType = (OMX_HA_DOLBYSTREAMTYPE)(((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo.s16StreamType);
            }
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
OMX_ERRORTYPE HI_OMX_DDP_SetParameter(OMX_IN OMX_HANDLETYPE hComponent,
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
            //if (strncmp((const char*)roleParams->cRole, "audio_decoder.ec3", OMX_MAX_STRINGNAME_SIZE - 1))
            if (strncmp((const char*)roleParams->cRole, "audio_decoder.ac3", OMX_MAX_STRINGNAME_SIZE - 1)
                && strncmp((const char*)roleParams->cRole, "audio_decoder.eac3", OMX_MAX_STRINGNAME_SIZE - 1))
            {
                return OMX_ErrorUndefined;
            }
            break;

        case OMX_IndexParamAudioPcm:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_IndexParamAudioPcm\n", __FUNCTION__);
            const OMX_AUDIO_PARAM_PCMMODETYPE* pcmParams =
                (OMX_AUDIO_PARAM_PCMMODETYPE*)ComponentParameterStructure;
            pHAData->sPcm.nChannels     = pcmParams->nChannels;
            pHAData->sPcm.nSamplingRate = pcmParams->nSamplingRate;
            if (pcmParams->nPortIndex != OMX_OUT_PORT_IDX)
            {
                return OMX_ErrorUndefined;
            }
            if (pcmParams->nChannels < 1 || pcmParams->nChannels > 8)
            {
                return OMX_ErrorUndefined;
            }
            break;

        case OMX_HA_IndexParamAudioDolby:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_HA_IndexParamAudioDolby\n", __FUNCTION__);
            memcpy((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam, ComponentParameterStructure,
                   sizeof(OMX_HA_PARAM_DOLBYTYPE));
            //TODO DRC Setting
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
                eError = OMX_ErrorBadPortIndex;
            }
            break;

         case OMX_IndexParamAudioAndroidAc3:
         case OMX_IndexParamAudioAndroidEac3:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> TOCHECK  0x%x\n", __FUNCTION__, nIndex);
            break;
         case OMX_IndexParamAudioDolbyDrcMode:
             {
                HI_S32  s32Ret = HA_ErrorNone;
                OMX_HA_PARAM_DOLBYDRCMODE* pstDolbyDrcMode = (OMX_HA_PARAM_DOLBYDRCMODE* )ComponentParameterStructure;
                HA_DOLBYPLUS_SET_DRC_MODE_S stDrcMode;

                stDrcMode.enCmd = HA_CODEC_DOLBYPLUS_SET_DRC_MODE_CMD;
                stDrcMode.eDrcMode = pstDolbyDrcMode->nDrcMode;
                s32Ret = HA_OMX_SetConfig(pHAData, &stDrcMode);
                if (HI_SUCCESS !=  s32Ret)
                {
                    return OMX_ErrorUndefined;
                }
             }
            break;
         case OMX_IndexParamAudioDolbyDrcRange:
             {
                HI_S32  s32Ret = HA_ErrorNone;
                OMX_HA_PARAM_DOLBYDRCRANGE* pstDolbyDrcRange = (OMX_HA_PARAM_DOLBYDRCRANGE* )ComponentParameterStructure;
                HA_DOLBYPLUS_SET_DYNSCALE_MODE_S stDrcRange;

                stDrcRange.enCmd = HA_CODEC_DOLBYPLUS_SET_DYNSCALE_MODE_CMD;
                stDrcRange.s16DynScaleHigh = pstDolbyDrcRange->nDrcRange;
                stDrcRange.s16DynScaleLow = pstDolbyDrcRange->nDrcRange;
                s32Ret = HA_OMX_SetConfig(pHAData, &stDrcRange);
                if (HI_SUCCESS !=  s32Ret)
                {
                    return OMX_ErrorUndefined;
                }
             }
            break;
         case OMX_IndexParamAudioDolbyStreamInfo:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> TOCHECK  0x%x\n", __FUNCTION__, nIndex);
            break;
         case OMX_HA_EnablePassThroughOutput:
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s -> OMX_HA_EnablePassThroughOutput  0x%x\n", __FUNCTION__, nIndex);
                OMX_HA_PARAME_PASSTHROUGHOUTPUTSTATUS *pPassThroughStatus = (OMX_HA_PARAME_PASSTHROUGHOUTPUTSTATUS*)ComponentParameterStructure;
                pHAData->stAdec.bPassThroughOutputEnable = (pPassThroughStatus->nStatus == 1) ? HI_TRUE : HI_FALSE;
            }
        default:
            eError = OMX_ErrorUnsupportedIndex;
            break;
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}


OMX_ERRORTYPE HI_OMX_DDP_ComponentRoleEnum(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_U8*       cRole,
        OMX_IN OMX_U32        nIndex)
{
    TRP_IN();
    if (nIndex == 0)
    {
        //TODO
        strncpy((char*)cRole, AUDIO_DEC_DDP_ROLE1, sizeof(AUDIO_DEC_DDP_ROLE1));
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s -> Fail: Can not get HI_OMX_DDP_ComponentRoleEnum!\n", __FUNCTION__);
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
    DOLBYPLUS_DECODE_OPENCONFIG_S*  pstDDPConfig;
    DOLBYPLUS_STREAM_INFO_S*    pstDDPStreamInfo;

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
    pHAData->pCodecParam = (OMX_HA_PARAM_DOLBYTYPE*)OMX_OSAL_Malloc(sizeof(OMX_HA_PARAM_DOLBYTYPE));
    if (!pHAData->pCodecParam)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecData OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecParam, 0x0, sizeof(OMX_HA_PARAM_DOLBYTYPE));

    // Create private codec Param
    pHAData->pCodecPrivData = (HA_DOLBY_PRIVDATA*)OMX_OSAL_Malloc(sizeof(HA_DOLBY_PRIVDATA));
    if (!pHAData->pCodecPrivData)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "pCodecPrivData OMX_OSAL_Malloc failed\n");
        goto EXIT;
    }
    memset(pHAData->pCodecPrivData, 0x0, sizeof(HA_DOLBY_PRIVDATA));
    pstDDPConfig = &((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPConfig;
    pstDDPStreamInfo = &((HA_DOLBY_PRIVDATA*)(pHAData->pCodecPrivData))->sDDPStreamInfo;

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
    p_Component->GetParameter           = HI_OMX_DDP_GetParameter;
    p_Component->SetParameter           = HI_OMX_DDP_SetParameter;
    p_Component->ComponentRoleEnum      = HI_OMX_DDP_ComponentRoleEnum;
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
    //TODO
    pHAData->sInPortDef.format.audio.pDeprecated0 = (OMX_PTR)"audio/ac3";
    pHAData->sInPortDef.format.audio.bFlagErrorConcealment = OMX_FALSE;
    pHAData->sInPortDef.format.audio.eEncoding = OMX_AUDIO_CodingAndroidEAC3; 
    pHAData->sInPortDef.eDir               = OMX_DirInput;
    pHAData->sInPortDef.nBufferCountMin    = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferCountActual = NUM_IN_BUFFERS;
    pHAData->sInPortDef.nBufferSize        = SIZE_IN_BUFFERS_DDP;

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
    pHAData->stAdec.bPassThroughOutputEnable = HI_TRUE;
    pHAData->stAdec.pstHA = HA_OMX_RegisterDecoderLib(g_szHA_DDPDecoder);
    if (HI_NULL == pHAData->stAdec.pstHA)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_RegisterDecoderLib(%s) Failed!\n", g_szHA_DDPDecoder);
        pHAData->stAdec.pstHA = HA_OMX_RegisterDecoderLib(g_szHA_AC3PassThrough);
        if (pHAData->stAdec.pstHA)
        {
            HA_AC3PASSTHROUGH_DecGetDefalutOpenParam(&pHAData->stAdec.sOpenPram);
        }
    }
    else
    {
        HA_DOLBYPLUS_DecGetDefalutOpenConfig(pstDDPConfig);

        pstDDPConfig->pfnEvtCbFunc[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE] = DDPlusCallBack;
        pstDDPConfig->pAppData[HA_DOLBYPLUS_EVENT_SOURCE_CHANGE]     = pstDDPStreamInfo;
        /* Dolby DVB Broadcast default settings */
        pstDDPConfig->enDrcMode = DOLBYPLUS_DRC_LINE;
        pstDDPConfig->enDmxMode = DOLBYPLUS_DMX_SRND;
        pstDDPConfig->enOutLfe  = DOLBYPLUS_LFEOUTOFF;

        HA_DOLBYPLUS_DecGetDefalutOpenParam(&pHAData->stAdec.sOpenPram, pstDDPConfig);
        pHAData->stAdec.sOpenPram.enDecMode = HD_DEC_MODE_SIMUL;
    }

    if (HI_NULL == pHAData->stAdec.pstHA)
    {
        eError = OMX_ErrorInsufficientResources;
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HA_OMX_RegisterDecoderLib(%s) Failed!\n", g_szHA_DDPDecoder);
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
    OMX_CONF_INIT_STRUCT_PTR((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam, OMX_HA_PARAM_DOLBYTYPE);
    //Default values for Dolby audio param port
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nPortIndex  = 0x0;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nChannels   = 2;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nBitRate    = 0;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nSampleRate = 44100;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nDRCPercent = 0;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->nDRCPercent = DOLBYPLUS_DRC_LINE;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->eStreamType = OMX_DOLBY_STREAMTYPE_DD;
    ((OMX_HA_PARAM_DOLBYTYPE*)pHAData->pCodecParam)->eAcmod      = OMX_DOLBY_CHANCFG_OUT_MODE20;

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
