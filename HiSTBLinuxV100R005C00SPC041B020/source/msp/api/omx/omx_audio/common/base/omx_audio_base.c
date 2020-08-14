#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef ANDROID
#undef  LOG_TAG
#define LOG_TAG    "HIOMX_AUDIO_BASE"
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
#include "omx_audio_base.h"
#include "ha_adec.h"
#include "ha_aenc.h"

#ifdef DOLBY_OMX_PASSTHRU_SUPPORT
#include "hi_unf_audio.h"
#include "hi_unf_sound.h"
#include "hi_mpi_ao.h"
#endif


/*
 *   Functions
 */
OMX_ERRORTYPE HI_OMX_CODEC_GetState(OMX_IN OMX_HANDLETYPE  hComponent,
                                    OMX_OUT OMX_STATETYPE* pState)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, pState, NON_ZERO);
    *pState = pHAData->state;

    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->  Cmd=0x%lx\n",  __func__, (OMX_U32)*pState);

OMX_CONF_CMD_BAIL:

    TRP_OUT();

    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_SendCommand(OMX_IN OMX_HANDLETYPE  hComponent,
                                       OMX_IN OMX_COMMANDTYPE Cmd,
                                       OMX_IN OMX_U32         nParam1,
                                       OMX_IN OMX_PTR         pCmdData)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);

    OMX_CONF_CHECK_CMD(pHAData, NON_ZERO, NON_ZERO);

    if (Cmd == OMX_CommandMarkBuffer)
    {
        OMX_CONF_CHECK_CMD(pCmdData, NON_ZERO, NON_ZERO);
    }

    if (pHAData->state == OMX_StateReserved_0x00000000)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorReserved_0x8000100A);
    }

    switch (Cmd)
    {
        case OMX_CommandStateSet:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   OMX_CommandStateSet\n",  __func__);
            break;

        case OMX_CommandFlush:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   OMX_CommandFlush\n",  __func__);
            if ((nParam1 > OMX_OUT_PORT_IDX) && (nParam1 != OMX_ALL))
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
            }
            break;

        case OMX_CommandPortDisable:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   OMX_CommandPortDisable\n",  __func__);
            break;

        case OMX_CommandPortEnable:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   OMX_CommandPortEnable\n",  __func__);
            break;

        case OMX_CommandMarkBuffer:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   OMX_CommandMarkBuffer\n",  __func__);
            if (nParam1 > OMX_IN_PORT_IDX)
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
            }
            break;

        default:
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ->   Unkown Commant !\n",  __func__);
            break;
    }

    write(pHAData->cmdpipe[PIPE_WRITE], &Cmd, sizeof(Cmd));
    // In case of OMX_CommandMarkBuffer, the pCmdData parameter is used to carry the data.
    // In other cases, the nParam1 parameter carries the data.
    if (Cmd == OMX_CommandMarkBuffer)
    {
        write(pHAData->cmddatapipe[PIPE_WRITE], &pCmdData, sizeof(OMX_PTR));
    }
    else
    {
        write(pHAData->cmddatapipe[PIPE_WRITE], &nParam1, sizeof(nParam1));
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_SetCallbacks(OMX_IN OMX_HANDLETYPE    hComponent,
                                        OMX_IN OMX_CALLBACKTYPE* pCallbacks,
                                        OMX_IN OMX_PTR           pAppData)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, pCallbacks, pAppData);

    pHAData->pCallbacks = pCallbacks;
    pHAData->pAppData   = pAppData;

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_UseBuffer(OMX_IN OMX_HANDLETYPE            hComponent,
                                     OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
                                     OMX_IN OMX_U32                   nPortIndex,
                                     OMX_IN OMX_PTR                   pAppPrivate,
                                     OMX_IN OMX_U32                   nSizeBytes,
                                     OMX_IN OMX_U8*                   pBuffer)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDef;
    OMX_U32         nIndex = 0x0;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, ppBufferHdr, pBuffer);

    if (nPortIndex == pHAData->sInPortDef.nPortIndex)
    {
        pPortDef = &pHAData->sInPortDef;
    }
    else if (nPortIndex == pHAData->sOutPortDef.nPortIndex)
    {
        pPortDef = &pHAData->sOutPortDef;
    }
    else
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
    }

    //TODO
#if 0
    if (!pPortDef->bEnabled)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }
#endif

    if ((nSizeBytes != pPortDef->nBufferSize) || pPortDef->bPopulated)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
    }

    // Find an empty position in the BufferList and allocate memory for the buffer header.
    // Use the buffer passed by the client to initialize the actual buffer
    // inside the buffer header.
    if (nPortIndex == pHAData->sInPortDef.nPortIndex)
    {
        ListAllocate(pHAData->sInBufList, nIndex);
        if (pHAData->sInBufList.pBufHdr[nIndex] == NULL)
        {
            pHAData->sInBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*)
                                                  OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
            if (!pHAData->sInBufList.pBufHdr[nIndex])
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
            }

            OMX_CONF_INIT_STRUCT_PTR (pHAData->sInBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s  ---> sInBufList.pBufHdr(index=%ld) had malloc, return fail!\n" , __func__, nIndex);
            eError = OMX_ErrorInsufficientResources;
            goto OMX_CONF_CMD_BAIL;
        }

        pHAData->sInBufList.pBufHdr[nIndex]->pBuffer = pBuffer;
        pHAData->sInBufList.bufferOwner[nIndex]      = OMX_USEBUFFER;
        LoadBufferHeader(pHAData->sInBufList, pHAData->sInBufList.pBufHdr[nIndex], pAppPrivate,
                         nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
        pHAData->sInBufList.addralloclist[nIndex] = (OMX_U32)(*ppBufferHdr);
    }
    else
    {
        ListAllocate(pHAData->sOutBufList, nIndex);
        if (pHAData->sOutBufList.pBufHdr[nIndex] == NULL)
        {
            pHAData->sOutBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*)
                                                   OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
            if (!pHAData->sOutBufList.pBufHdr[nIndex])
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
            }

            OMX_CONF_INIT_STRUCT_PTR (pHAData->sOutBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s  ---> sOutBufList.pBufHdr(index=%ld) had malloc, return fail!\n" , __func__, nIndex);
            eError = OMX_ErrorInsufficientResources;
            goto OMX_CONF_CMD_BAIL;
        }

        pHAData->sOutBufList.pBufHdr[nIndex]->pBuffer = pBuffer;
        pHAData->sInBufList.bufferOwner[nIndex]       = OMX_USEBUFFER;
        LoadBufferHeader(pHAData->sOutBufList, pHAData->sOutBufList.pBufHdr[nIndex],
                         pAppPrivate, nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
        pHAData->sOutBufList.addralloclist[nIndex] = (OMX_U32)(*ppBufferHdr);
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_AllocateBuffer(OMX_IN OMX_HANDLETYPE            hComponent,
        OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
        OMX_IN OMX_U32                   nPortIndex,
        OMX_IN OMX_PTR                   pAppPrivate,
        OMX_IN OMX_U32                   nSizeBytes)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    OMX_S8          nIndex = 0x0;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDef;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, ppBufferHdr, NON_ZERO);

    if (nPortIndex == pHAData->sInPortDef.nPortIndex)
    {
        pPortDef = &pHAData->sInPortDef;
    }
    else
    {
        if (nPortIndex == pHAData->sOutPortDef.nPortIndex)
        {
            pPortDef = &pHAData->sOutPortDef;
        }
        else
        {
            OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
        }
    }

    //TODO
#if 0
    if (!pPortDef->bEnabled)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }
#endif

    if ((nSizeBytes != pPortDef->nBufferSize) || pPortDef->bPopulated)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
    }

    // Find an empty position in the BufferList and allocate memory for the buffer header
    // and the actual buffer
    if (nPortIndex == pHAData->sInPortDef.nPortIndex)
    {
        ListAllocate(pHAData->sInBufList, nIndex);
        if (pHAData->sInBufList.pBufHdr[nIndex] == NULL)
        {
            pHAData->sInBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*)
                                                  OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
            if (!pHAData->sInBufList.pBufHdr[nIndex])
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
            }

            OMX_CONF_INIT_STRUCT_PTR (pHAData->sInBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s  ---> sInBufList.pBufHdr(index=%d) had malloc, return fail!\n" , __func__, nIndex);
            eError = OMX_ErrorInsufficientResources;
            goto OMX_CONF_CMD_BAIL;
        }

        pHAData->sInBufList.pBufHdr[nIndex]->pBuffer = (OMX_U8*)OMX_OSAL_Malloc(nSizeBytes);
        pHAData->sInBufList.bufferOwner[nIndex] = OMX_ALLOCATEBUFFER;

        if (!pHAData->sInBufList.pBufHdr[nIndex]->pBuffer)
        {
            OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
        }

        LoadBufferHeader(pHAData->sInBufList, pHAData->sInBufList.pBufHdr[nIndex], pAppPrivate,
                         nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
        pHAData->sInBufList.addralloclist[nIndex] = (OMX_U32)(*ppBufferHdr);
    }
    else
    {
        ListAllocate(pHAData->sOutBufList, nIndex);
        if (pHAData->sOutBufList.pBufHdr[nIndex] == NULL)
        {
            pHAData->sOutBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*)
                                                   OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));

            if (!pHAData->sOutBufList.pBufHdr[nIndex])
            {
                OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
            }

            OMX_CONF_INIT_STRUCT_PTR(pHAData->sOutBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s  ---> sOutBufList.pBufHdr(index=%d) had malloc, return fail!\n" , __func__, nIndex);
            eError = OMX_ErrorInsufficientResources;
            goto OMX_CONF_CMD_BAIL;
        }

        pHAData->sOutBufList.pBufHdr[nIndex]->pBuffer = (OMX_U8*)OMX_OSAL_Malloc(nSizeBytes);
        pHAData->sOutBufList.bufferOwner[nIndex] = OMX_ALLOCATEBUFFER;
        if (!pHAData->sOutBufList.pBufHdr[nIndex]->pBuffer)
        {
            OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
        }

        LoadBufferHeader(pHAData->sOutBufList, pHAData->sOutBufList.pBufHdr[nIndex],
                         pAppPrivate, nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
        pHAData->sOutBufList.addralloclist[nIndex] = (OMX_U32)(*ppBufferHdr);
    }

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_FreeBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
                                      OMX_IN OMX_U32               nPortIndex,
                                      OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_PARAM_PORTDEFINITIONTYPE* pPortDef;
    OMX_S32 nIndex;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, pBufferHdr, NON_ZERO);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    // Match the pBufferHdr to the appropriate entry in the BufferList
    // and free the allocated memory
    if (nPortIndex == pHAData->sInPortDef.nPortIndex)
    {
        pPortDef = &pHAData->sInPortDef;
        ListFreeBuffer(pHAData->sInBufList, pBufferHdr, pPortDef, nIndex);
    }
    else if (nPortIndex == pHAData->sOutPortDef.nPortIndex)
    {
        pPortDef = &pHAData->sOutPortDef;
        ListFreeBuffer(pHAData->sOutBufList, pBufferHdr, pPortDef, nIndex);
    }
    else
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
    }

#if  0   //same as android  TOCHECK
    if (pPortDef->bEnabled && (pHAData->state != OMX_StateIdle))
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }
#endif

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_EmptyThisBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
        OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr)
{
    HI_AUDDATATYPE* pHAData;
    OMX_COMMANDTYPE eCmd   = OMX_HA_CommandEmptyBuf;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, pBufferHdr, NON_ZERO);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    if (!pHAData->sInPortDef.bEnabled)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    if ((pBufferHdr->nInputPortIndex != OMX_DirInput) || (pBufferHdr->nOutputPortIndex != OMX_NOPORT))
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
    }

    if ((pHAData->state != OMX_StateExecuting) && (pHAData->state != OMX_StatePause))
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    // Put the command and data in the pipe
    write(pHAData->cmdpipe[PIPE_WRITE], &eCmd, sizeof(eCmd));
    write(pHAData->cmddatapipe[PIPE_WRITE], &pBufferHdr, sizeof(OMX_BUFFERHEADERTYPE*));

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_FillThisBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
        OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr)
{
    HI_AUDDATATYPE* pHAData;
    OMX_COMMANDTYPE eCmd   = OMX_HA_CommandFillBuf;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pHAData, pBufferHdr, NON_ZERO);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    if (!pHAData->sOutPortDef.bEnabled)
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    if ((pBufferHdr->nOutputPortIndex != OMX_DirOutput) || (pBufferHdr->nInputPortIndex != OMX_NOPORT))
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
    }

    if ((pHAData->state != OMX_StateExecuting) && (pHAData->state != OMX_StatePause))
    {
        OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);
    }

    // Put the command and data in the pipe
    write(pHAData->cmdpipe[PIPE_WRITE], &eCmd, sizeof(eCmd));
    write(pHAData->cmddatapipe[PIPE_WRITE], &pBufferHdr, sizeof(OMX_BUFFERHEADERTYPE*));

OMX_CONF_CMD_BAIL:
    TRP_OUT();
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_ComponentDeInit(OMX_IN OMX_HANDLETYPE hComponent)
{
    HI_AUDDATATYPE* pHAData;
    OMX_ERRORTYPE   eError = OMX_ErrorNone;
    OMX_COMMANDTYPE eCmd   = OMX_HA_CommandStopThread;
    OMX_S32         nIndex = 0;
    TRP_IN();

    pHAData = (HI_AUDDATATYPE*)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);

    // In case the client crashes, check for nAllocSize parameter.
    // If this is greater than zero, there are elements in the list that are not free'd.
    // In that case, free the elements.
    if (pHAData->sInBufList.nAllocSize > 0)
    {
        ListFreeAllBuffers(pHAData->sInBufList, nIndex);
        if (pHAData->sOutBufList.nAllocSize > 0)
        {
            ListFreeAllBuffers(pHAData->sOutBufList, nIndex);
            // Put the command and data in the pipe
            write(pHAData->cmdpipe[PIPE_WRITE], &eCmd, sizeof(eCmd));
        }
    }
    else
    {
        // Put the command and data in the pipe
        write(pHAData->cmdpipe[PIPE_WRITE], &eCmd, sizeof(eCmd));
    }

    write(pHAData->cmddatapipe[PIPE_WRITE], &eCmd, sizeof(eCmd));

    // Wait for thread to exit so we can get the status into "error"
    pthread_join(pHAData->thread_id, (void*)&eError);

    // close the pipe handles
    close(pHAData->cmdpipe[PIPE_READ]);
    close(pHAData->cmdpipe[PIPE_WRITE]);
    close(pHAData->cmddatapipe[PIPE_READ]);
    close(pHAData->cmddatapipe[PIPE_WRITE]);
    if (pHAData->hDecoder)
    {
        HA_OMX_DeInitDecoder(pHAData);
        HA_OMX_UnRegisterDecoderLib(pHAData);
        pHAData->hDecoder = NULL;
    }
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

    TRP_OUT();
    return eError;
}

HI_VOID TraceState(OMX_STATETYPE state, OMX_STATETYPE newstate)
{
    char state0[] = "OMX_StateReserved_0x00000000";
    char state1[] = "OMX_StateLoaded";
    char state2[] = "OMX_StateIdle";
    char state3[] = "OMX_StateExecuting";
    char state4[] = "OMX_StatePause";
    char state5[] = "OMX_StateWaitForResources";
    char state6[] = "OMX_StateKhronosExtensions";
    char state7[] = "OMX_StateVendorStartUnused";
    char state8[] = "OMX_StateMax";
    char* old = HI_NULL;
    char* new = HI_NULL;

    switch (state)
    {
        case OMX_StateReserved_0x00000000:
            old = state0;
            break;
        case OMX_StateLoaded:
            old = state1;
            break;
        case OMX_StateIdle:
            old = state2;
            break;
        case OMX_StateExecuting:
            old = state3;
            break;
        case OMX_StatePause:
            old = state4;
            break;
        case OMX_StateWaitForResources:
            old = state5;
            break;
        case OMX_StateKhronosExtensions:
            old = state6;
            break;
        case OMX_StateVendorStartUnused:
            old = state7;
            break;
        case OMX_StateMax:
            old = state8;
            break;

        default :
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, " Unknown Old State %d \n", (HI_U32)state);
            break;
    }

    switch (newstate)
    {
        case OMX_StateReserved_0x00000000:
            new = state0;
            break;
        case OMX_StateLoaded:
            new = state1;
            break;
        case OMX_StateIdle:
            new = state2;
            break;
        case OMX_StateExecuting:
            new = state3;
            break;
        case OMX_StatePause:
            new = state4;
            break;
        case OMX_StateWaitForResources:
            new = state5;
            break;
        case OMX_StateKhronosExtensions:
            new = state6;
            break;
        case OMX_StateVendorStartUnused:
            new = state7;
            break;
        case OMX_StateMax:
            new = state8;
            break;

        default :
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, " Unknown New State %d \n", (HI_U32)newstate);
            break;
    }

    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, " [OMX Audio State]  %s  ---> %s \n", old, new);
}

static OMX_ERRORTYPE CmdProcess(HI_AUDDATATYPE* pHAData)
{
    OMX_U32         cmddata;
    OMX_COMMANDTYPE cmd;
    /* Variables related to decoder buffer handling */
    OMX_MARKTYPE*   pMarkBuf = NULL;
    /* Variables related to decoder timeouts */
    OMX_BOOL        bTimeout;
    OMX_U32         nTimeout;
    TRP_IN();

    // retrieve command and data from pipe
    read(pHAData->cmdpipe[PIPE_READ], &cmd, sizeof(cmd));
    read(pHAData->cmddatapipe[PIPE_READ], &cmddata, sizeof(cmddata));

    // State transition command
    if (cmd == OMX_CommandStateSet)
    {
        // If the parameter states a transition to the same state
        // raise a same state transition error.
        TraceState(pHAData->state, cmddata);
        if (pHAData->state == (OMX_STATETYPE)(cmddata))
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ---> OMX_ErrorSameState, do nothings\n" , __func__);
            pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                              OMX_EventError, OMX_ErrorSameState, 0, NULL);
        }
        else
        {
            // transitions/callbacks made based on state transition table
            // cmddata contains the target state
            switch ((OMX_STATETYPE)(cmddata))
            {
                case OMX_StateReserved_0x00000000:
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  execute OMX_StateReserved_0x00000000\n" , __func__);
                    pHAData->state = OMX_StateReserved_0x00000000;
                    pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                      OMX_EventError, OMX_ErrorReserved_0x8000100A, 0, NULL);
                    pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                      OMX_EventCmdComplete, OMX_CommandStateSet, pHAData->state,
                                                      NULL);
                    break;
                case OMX_StateLoaded:
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  execute OMX_StateLoaded\n" , __func__);
                    if ((pHAData->state == OMX_StateIdle)
                        || (pHAData->state == OMX_StateWaitForResources))
                    {
                        nTimeout = 0x0;
                        while (1)
                        {
                            // Transition happens only when the ports are unpopulated
                            if (!pHAData->sInPortDef.bPopulated
                                && !pHAData->sOutPortDef.bPopulated)
                            {
                                pHAData->state = OMX_StateLoaded;
                                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                                  pHAData->pAppData, OMX_EventCmdComplete,
                                                                  OMX_CommandStateSet, pHAData->state, NULL);
                                // todo, close decoder
                                break;
                            }
                            else if (nTimeout++ > OMX_MAX_TIMEOUTS)
                            {
                                pHAData->pCallbacks->EventHandler(
                                    pHAData->hSelf, pHAData->pAppData, OMX_EventError,
                                    OMX_ErrorInsufficientResources, 0, NULL);
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_INFO,
                                               "Transition to loaded failed\n");
                                break;
                            }

                            OMX_OSAL_EventReset(pHAData->hTimeout);
                            OMX_OSAL_EventWait(pHAData->hTimeout, OMX_TIMEOUT, &bTimeout);
                        }
                    }
                    else
                    {
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventError, OMX_ErrorIncorrectStateTransition, 0,
                                                          NULL);
                    }
                    break;
                case OMX_StateIdle:
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  execute OMX_StateIdle\n" , __func__);
                    /*
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "aa   %d  %d %d %d !\n",
                                        pHAData->sInPortDef.bEnabled, pHAData->sInBufList.nSizeOfList,
                                        pHAData->sOutPortDef.bEnabled,  pHAData->sOutBufList.nSizeOfList );
                    */
#ifdef DOLBY_OMX_PASSTHRU_SUPPORT
                    if ((pHAData->state == OMX_StatePause) || (pHAData->state == OMX_StateExecuting))
                    {
                        if (HI_TRUE == pHAData->stAdec.bPassThrough)
                        {
                            HI_S32 s32Ret;
                            s32Ret = HI_MPI_AO_Track_Stop(pHAData->stAdec.hTrack);
                            if (s32Ret != HI_SUCCESS)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HI_MPI_AO_Track_Stop err: 0x%x\n", s32Ret);
                            }

                            s32Ret = HI_MPI_AO_Track_Destroy(pHAData->stAdec.hTrack);
                            if (HI_SUCCESS != s32Ret)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HI_MPI_AO_Track_Destroy err: 0x%x\n", s32Ret);
                            }

                            HI_UNF_SND_Close(HI_UNF_SND_0);
                            HI_UNF_SND_DeInit();
                        }
                    }
#endif

                    if (pHAData->state == OMX_StateReserved_0x00000000)
                    {
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventError, OMX_ErrorIncorrectStateTransition, 0,
                                                          NULL);
                    }
                    else
                    {
                        // Return buffers if currently in pause and executing
                        if ((pHAData->state == OMX_StatePause)
                            || (pHAData->state == OMX_StateExecuting))
                        {
                            ListFlushEntries(pHAData->sInBufList, pHAData);
                            ListFlushEntries(pHAData->sOutBufList, pHAData);
                            /*
                            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "   %d  %d %d %d !\n",
                                                pHAData->sInPortDef.bEnabled, pHAData->sInBufList.nSizeOfList,
                                                pHAData->sOutPortDef.bEnabled,  pHAData->sOutBufList.nSizeOfList );
                            */
                            HA_OMX_FlushInnerState(pHAData);
                            HA_OMX_FlushAencInnerState(pHAData);
                        }
                        nTimeout = 0x0;
                        while (1)
                        {
                            // Ports have to be populated before transition completes
                            if ((!pHAData->sInPortDef.bEnabled
                                 && !pHAData->sOutPortDef.bEnabled)
                                || (pHAData->sInPortDef.bPopulated
                                    && pHAData->sOutPortDef.bPopulated))
                            {
                                pHAData->state = OMX_StateIdle;
                                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                                  pHAData->pAppData, OMX_EventCmdComplete,
                                                                  OMX_CommandStateSet, pHAData->state, NULL);
                                break;
                            }
                            else if (nTimeout++ > OMX_MAX_TIMEOUTS)
                            {
                                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                                  pHAData->pAppData, OMX_EventError,
                                                                  OMX_ErrorInsufficientResources, 0, NULL);
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR,  "%s  ---> Idle transition failed\n", __func__);

                                break;
                            }
                            OMX_OSAL_EventReset(pHAData->hTimeout);
                            OMX_OSAL_EventWait(pHAData->hTimeout, OMX_TIMEOUT, &bTimeout);
                        }
                    }
                    break;
                case OMX_StateExecuting:
                    // Transition can only happen from pause or idle state
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  execute OMX_StateExecuting\n" , __func__);
                    /*
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, " %d  %d %d %d !\n",
                                        pHAData->sInPortDef.bEnabled, pHAData->sInBufList.nSizeOfList,
                                        pHAData->sOutPortDef.bEnabled,  pHAData->sOutBufList.nSizeOfList );
                    */
#ifdef DOLBY_OMX_PASSTHRU_SUPPORT
                    if (pHAData->state == OMX_StateIdle)
                    {
                        if (HI_TRUE == pHAData->stAdec.bPassThrough)
                        {
                            HI_S32 s32Ret;
                            HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
                            HI_UNF_SND_ATTR_S pAttr;
                            HI_UNF_SND_Init();

                            s32Ret = HI_UNF_SND_GetDefaultOpenAttr(HI_UNF_SND_0, &pAttr);
                            if (s32Ret != HI_SUCCESS)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Call HI_UNF_SND_GetDefaultOpenAttr failed\n");
                            }

                            s32Ret = HI_UNF_SND_Open(HI_UNF_SND_0, &pAttr);
                            if (s32Ret != HI_SUCCESS)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Call HI_UNF_SND_Open failed\n");
                            }

                            s32Ret = HI_MPI_AO_Track_GetDefaultOpenAttr(HI_UNF_SND_TRACK_TYPE_MASTER, &stTrackAttr);
                            if (HI_SUCCESS != s32Ret)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Call HI_MPI_AO_Track_GetDefaultOpenAttr failed\n");
                                //goto EXIT;
                            }

                            s32Ret = HI_MPI_AO_Track_Create(HI_UNF_SND_0, &stTrackAttr, &pHAData->stAdec.hTrack);
                            if (s32Ret != HI_SUCCESS)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Call HI_MPI_AO_Track_Create failed\n");
                                //goto EXIT;
                            }

                            s32Ret = HI_MPI_AO_Track_Start(pHAData->stAdec.hTrack);
                            if (HI_SUCCESS != s32Ret)
                            {
                                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Call HI_MPI_AO_Track_Start failed\n");
                            }
                        }
                    }
#endif

                    if ((pHAData->state == OMX_StateIdle)
                        || (pHAData->state == OMX_StatePause))
                    {
                        // Return buffers if currently in pause
                        if (pHAData->state == OMX_StatePause)
                        {
                            ListFlushEntries(pHAData->sInBufList, pHAData);
                            ListFlushEntries(pHAData->sOutBufList, pHAData);
                            HA_OMX_FlushInnerState(pHAData);
                            HA_OMX_FlushAencInnerState(pHAData);
                        }
                        pHAData->state = OMX_StateExecuting;
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventCmdComplete, OMX_CommandStateSet, pHAData->state,
                                                          NULL);
                    }
                    else
                    {
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventError, OMX_ErrorIncorrectStateTransition, 0,
                                                          NULL);
                    }

                    break;
                case OMX_StatePause:
                    // Transition can only happen from idle or executing state
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_StatePause\n" , __func__);

                    if ((pHAData->state == OMX_StateIdle)
                        || (pHAData->state == OMX_StateExecuting))
                    {
                        pHAData->state = OMX_StatePause;
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventCmdComplete, OMX_CommandStateSet, pHAData->state,
                                                          NULL);
                    }
                    else
                    {
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventError, OMX_ErrorIncorrectStateTransition, 0,
                                                          NULL);
                    }

                    break;
                case OMX_StateWaitForResources:
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_StateWaitForResources\n" , __func__);
                    if (pHAData->state == OMX_StateLoaded)
                    {
                        pHAData->state = OMX_StateWaitForResources;
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventCmdComplete, OMX_CommandStateSet, pHAData->state,
                                                          NULL);
                    }
                    else
                    {
                        pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                          OMX_EventError, OMX_ErrorIncorrectStateTransition, 0,
                                                          NULL);
                    }

                default:
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> Unknown State %ld\n" , __func__, cmddata);
                    break;
            }
        }
    }
    else if (cmd == OMX_CommandPortDisable)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_CommandPortDisable\n" , __func__);

        // Stop Port(s)
        // cmddata contains the port index to be stopped.
        // It is assumed that 0 is input and 1 is output port for this component
        // The cmddata value -1 means that both input and output ports will be stopped.
        if ((cmddata == OMX_IN_PORT_IDX) || (cmddata == OMX_ALL))
        {
            // Return all input buffers
            ListFlushEntries(pHAData->sInBufList, pHAData);
            HA_OMX_FlushInnerState(pHAData);
            HA_OMX_FlushAencInnerState(pHAData);
            // Disable port
            pHAData->sInPortDef.bEnabled = OMX_FALSE;
        }

        if ((cmddata == OMX_OUT_PORT_IDX) || (cmddata == OMX_ALL))
        {
            // Return all output buffers
            ListFlushEntries(pHAData->sOutBufList, pHAData);
            // Disable port
            pHAData->sOutPortDef.bEnabled = OMX_FALSE;
        }

        // Wait for all buffers to be freed
        nTimeout = 0x0;
        while (1)
        {
            if ((cmddata == OMX_IN_PORT_IDX) && !pHAData->sInPortDef.bPopulated)
            {
                // Return cmdcomplete event if input unpopulated
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventCmdComplete, OMX_CommandPortDisable, 0x0, NULL);
                break;
            }

            if ((cmddata == OMX_OUT_PORT_IDX) && !pHAData->sOutPortDef.bPopulated)
            {
                // Return cmdcomplete event if output unpopulated
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventCmdComplete, OMX_CommandPortDisable, 0x1, NULL);
                break;
            }

            if ((cmddata == OMX_ALL) && !pHAData->sInPortDef.bPopulated
                && !pHAData->sOutPortDef.bPopulated)
            {
                // Return cmdcomplete event if inout & output unpopulated
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventCmdComplete, OMX_CommandPortDisable, 0x0, NULL);
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventCmdComplete, OMX_CommandPortDisable, 0x1, NULL);
                break;
            }

            if (nTimeout++ > OMX_MAX_TIMEOUTS)
            {
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventError, OMX_ErrorReserved_0x80001015,
                                                  0, NULL);
                break;
            }

            OMX_OSAL_EventReset(pHAData->hTimeout);
            OMX_OSAL_EventWait(pHAData->hTimeout, OMX_TIMEOUT, &bTimeout);
        }
    }
    else if (cmd == OMX_CommandPortEnable)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_CommandPortEnable\n" , __func__);

        // Restart Port(s)
        // cmddata contains the port index to be restarted.
        // It is assumed that 0 is input and 1 is output port for this component.
        // The cmddata value -1 means both input and output ports will be restarted.
        if ((cmddata == OMX_IN_PORT_IDX) || (cmddata == OMX_ALL))
        {
            pHAData->sInPortDef.bEnabled = OMX_TRUE;
        }

        if ((cmddata == OMX_OUT_PORT_IDX) || (cmddata == OMX_ALL))
        {
            pHAData->sOutPortDef.bEnabled = OMX_TRUE;
        }

        // Wait for port to be populated
        nTimeout = 0x0;
        while (1)
        {
            // Return cmdcomplete event if input port populated
            if ((cmddata == OMX_IN_PORT_IDX) && ((pHAData->state == OMX_StateLoaded)
                                                 || pHAData->sInPortDef.bPopulated))
            {
                pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                                  OMX_EventCmdComplete, OMX_CommandPortEnable, 0x0, NULL);
                break;
            }
            // Return cmdcomplete event if output port populated
            else if ((cmddata == OMX_OUT_PORT_IDX) && ((pHAData->state == OMX_StateLoaded)
                     ||  pHAData->sOutPortDef.bPopulated))
            {
                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                  pHAData->pAppData, OMX_EventCmdComplete,
                                                  OMX_CommandPortEnable, 0x1, NULL);
                break;
            }
            // Return cmdcomplete event if input and output ports populated
            else if ((cmddata == OMX_ALL) && ((pHAData->state == OMX_StateLoaded)
                                              || (pHAData->sInPortDef.bPopulated
                                                  && pHAData->sOutPortDef.bPopulated)))
            {
                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                  pHAData->pAppData, OMX_EventCmdComplete,
                                                  OMX_CommandPortEnable, 0x0, NULL);
                pHAData->pCallbacks->EventHandler(pHAData->hSelf,
                                                  pHAData->pAppData, OMX_EventCmdComplete,
                                                  OMX_CommandPortEnable, 0x1, NULL);
                break;
            }
            else if (nTimeout++ > OMX_MAX_TIMEOUTS)
            {
                pHAData->pCallbacks->EventHandler(
                    pHAData->hSelf, pHAData->pAppData,
                    OMX_EventError,
                    OMX_ErrorReserved_0x80001014, 0, NULL);
                break;
            }

            OMX_OSAL_EventReset(pHAData->hTimeout);
            OMX_OSAL_EventWait(pHAData->hTimeout, OMX_TIMEOUT, &bTimeout);
        }
    }
    else if (cmd == OMX_CommandFlush)
    {
        // Flush port(s)
        // cmddata contains the port index to be flushed.
        // It is assumed that 0 is input and 1 is output port for this component
        // The cmddata value -1 means that both input and output ports will be flushed.
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute Flush\n" , __func__);

        if ((cmddata == OMX_IN_PORT_IDX) || (cmddata == OMX_ALL))
        {
            // Return all input buffers and send cmdcomplete
            ListFlushEntries(pHAData->sInBufList, pHAData);
            pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                              OMX_EventCmdComplete, OMX_CommandFlush, 0x0, NULL);
            HA_OMX_FlushInnerState(pHAData);
            HA_OMX_FlushAencInnerState(pHAData);
        }

        if ((cmddata == OMX_OUT_PORT_IDX) || (cmddata == OMX_ALL))
        {
            // Return all output buffers and send cmdcomplete
            ListFlushEntries(pHAData->sOutBufList, pHAData);
            pHAData->pCallbacks->EventHandler(pHAData->hSelf, pHAData->pAppData,
                                              OMX_EventCmdComplete, OMX_CommandFlush, 0x1, NULL);
        }
    }
    else if (cmd == OMX_HA_CommandStopThread)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_HA_CommandStopThread\n" , __func__);
        goto EXIT;
    }
    else if (cmd == OMX_HA_CommandFillBuf)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_HA_CommandFillBuf\n" , __func__);

        // Fill buffer
        ListSetEntry(pHAData->sOutBufList,  cmddata);  //cmddata  OMX_BUFFERHEADERTYPE POINTER
    }
    else if (cmd == OMX_HA_CommandEmptyBuf)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  ---> execute OMX_HA_CommandEmptyBuf\n" , __func__);

        // Empty buffer
        ListSetEntry(pHAData->sInBufList, cmddata);
        // Mark current buffer if there is outstanding command
        if (pMarkBuf)
        {
            ((OMX_BUFFERHEADERTYPE*)(cmddata))->hMarkTargetComponent =
                pMarkBuf->hMarkTargetComponent;
            ((OMX_BUFFERHEADERTYPE*)(cmddata))->pMarkData = pMarkBuf->pMarkData;
            pMarkBuf = NULL;
        }
    }
    else if (cmd == OMX_CommandMarkBuffer)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> execute OMX_CommandMarkBuffer\n" , __func__);
        if (!pMarkBuf)
        {
            //OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> \n" , __func__);
            pMarkBuf = (OMX_MARKTYPE*)(cmddata);
        }
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s ---> Unkonwn CMD\n" , __func__);
    }

    TRP_OUT();
    return OMX_ErrorNone;

EXIT:
    TRP_OUT();
    return OMX_ErrorUndefined;
}

static OMX_ERRORTYPE OMX_CODEC_CommandMgmtProcess(HI_AUDDATATYPE* pHAData)
{
    int i;
    OMX_S32 nRetValue = OMX_ErrorNone;
    TRP_IN();

    FD_ZERO(&pHAData->rfds);
    FD_SET(pHAData->cmdpipe[PIPE_READ], &pHAData->rfds);

    // Check for new command
    i = select(pHAData->cmdpipe[PIPE_READ] + 1, &pHAData->rfds, NULL, NULL, NULL);
    if (FD_ISSET(pHAData->cmdpipe[PIPE_READ], &pHAData->rfds))
    {
        nRetValue = CmdProcess(pHAData);
    }
    else
    { OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s  ---> Check for new command Failed !\n" , __func__); }

    TRP_OUT();
    return nRetValue;
}

static int  HA_PreBuffer(HI_AUDDATATYPE* pHAData)
{
    TRP_IN();

    //pre input buffer
    if ((OWNED_BY_NULL == pHAData->enInBufState)   ||
        ((OWNED_BY_COMPONENT ==  pHAData->enInBufState)))  //init or decode clear
    {
        ListCacheEntry(pHAData->sInBufList, pHAData->pInBufHdr);
        if (pHAData->pInBufHdr == NULL)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_PreBuffer fetch Input Bufffer Failed\n");
            return HI_FAILURE;
        }
        else
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_PreBuffer fetch Input Bufffer Success\n");
            pHAData->enInBufState = OWNED_BY_CLIENT;  //update by decodeframe
            pHAData->bNewPacketIn = HI_TRUE;          //update by decodeframe
        }
    }

    ListCacheEntry(pHAData->sOutBufList, pHAData->pOutBufHdr);
    if (pHAData->pOutBufHdr == NULL)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_PreBuffer fetch  Out Bufffer Failed , return\n");
        return HI_FAILURE;
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_PreBuffer fetch Out Bufffer Success\n");
    }

    TRP_OUT();
    return  HI_SUCCESS;
}

static void  HA_PostOutputBuffer(HI_AUDDATATYPE* pHAData)
{
    //called by decodeframe success
    TRP_IN();

    //ListDump(pHAData->sOutBufList);
    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_PostOutputBuffer  Out Bufffer & pHAData->pCallbacks->FillBufferDone  size=%ld %p \n",
                   pHAData->pOutBufHdr->nFilledLen, pHAData->pOutBufHdr);

    // releast Output Bufffer
    ListReleaseEntry(pHAData->sOutBufList, pHAData->pOutBufHdr);
    pHAData->pCallbacks->FillBufferDone(pHAData->hSelf, pHAData->pAppData, pHAData->pOutBufHdr);
    pHAData->pOutBufHdr = HI_NULL;
    //ListDump(pHAData->sOutBufList);
    TRP_OUT();
}

static void  HA_PostInputBuffer(HI_AUDDATATYPE* pHAData)
{
    TRP_IN();
    //inbuf hold by compoent
    if (OWNED_BY_COMPONENT ==  pHAData->enInBufState)
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s  -->  pHAData->pCallbacks->EmptyBufferDone  %p\n", __func__, pHAData->pInBufHdr);

        // releast input Bufffer
        ListReleaseEntry(pHAData->sInBufList, pHAData->pInBufHdr);
        pHAData->pCallbacks->EmptyBufferDone(pHAData->hSelf, pHAData->pAppData, pHAData->pInBufHdr);
        pHAData->pInBufHdr = HI_NULL;
    }
    TRP_OUT();
}

static HI_BOOL HA_CheckBufferState(HI_AUDDATATYPE* pHAData)
{
    if (pHAData->sInPortDef.bEnabled  &&
        pHAData->sOutPortDef.bEnabled && (pHAData->sOutBufList.nSizeOfList > 0) )
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_CheckBufferState Buf_Ready  %d %d %d %d\n",
                       (int)pHAData->sInPortDef.bEnabled, (int)pHAData->sInBufList.nSizeOfList,
                       (int)pHAData->sOutPortDef.bEnabled,  (int)pHAData->sOutBufList.nSizeOfList );
        return HI_TRUE;
    }
    else
    {
        OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HA_CheckBufferState Buf_NOT_Ready  %d %d %d %d\n",
                       (int)pHAData->sInPortDef.bEnabled, (int)pHAData->sInBufList.nSizeOfList,
                       (int)pHAData->sOutPortDef.bEnabled,  (int)pHAData->sOutBufList.nSizeOfList );
        return HI_FALSE;
    }
}


static void OMX_ADEC_BufferMgmtProcess(HI_AUDDATATYPE* pHAData)
{
    HI_S32 s32Ret;
    HI_S32 s32ResourcesRet;
    TRP_IN();

    // Buffer processing
    if (HI_TRUE == HA_CheckBufferState(pHAData))
    {
        //consider once decode to nodatastate
        do
        {
            if (HI_TRUE == pHAData->bPortSetChange)
            {
                if (NUM_OUT_BUFFERS == pHAData->sOutBufList.nSizeOfList)
                {
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, ">>>OMX_EventPortSettingsChanged before HA_OMX_DecodeFrame\n");
                    /* Send Port Settings changed call back */
                    (*(pHAData->pCallbacks->EventHandler))(pHAData->hSelf, pHAData->pAppData,
                                                           OMX_EventPortSettingsChanged,    /* The command was completed */
                                                           OMX_DirOutput,                   /* This is the port index */
                                                           0, NULL);
                    pHAData->bPortSetChange = HI_FALSE;
                }
                return;
            }

            s32ResourcesRet = HA_PreBuffer(pHAData);
            if (HA_ErrorNone != s32ResourcesRet)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "HI_PreBuffer Failed\n");
                return;
            }

            if (pHAData->CodecFrame)
            {
                s32Ret = pHAData->CodecFrame(pHAData, pHAData->pInBufHdr, pHAData->pOutBufHdr);
            }
            else
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "DecodeFrame func Unregister\n");
                return;
            }

            if (HI_TRUE == pHAData->bPortSetChange)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, ">>>OMX_EventPortSettingsChanged pHAData->sOutBufList.nSizeOfList=%ld\n", pHAData->sOutBufList.nSizeOfList);
                if (NUM_OUT_BUFFERS == pHAData->sOutBufList.nSizeOfList)
                {
                    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, ">>>OMX_EventPortSettingsChanged after HA_OMX_DecodeFrame\n");
                    /* Send Port Settings changed call back */
                    (*(pHAData->pCallbacks->EventHandler))(pHAData->hSelf, pHAData->pAppData,
                                                           OMX_EventPortSettingsChanged,    /* The command was completed */
                                                           OMX_DirOutput,                   /* This is the port index */
                                                           0, NULL);
                    pHAData->bPortSetChange = HI_FALSE;
                }
                return;
            }

            if (HI_FALSE == pHAData->mEndOfInput)
            {
                if (HA_ErrorNone == s32Ret)
                {
                    HA_PostOutputBuffer(pHAData);
                }
            }
            else
            {
                pHAData->pOutBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
                HA_PostOutputBuffer(pHAData);
                // Trigger EOS event handler
                OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  pCallbacks->EventHandler   OMX_BUFFERFLAG_EOS\n", __func__);
                (*(pHAData->pCallbacks->EventHandler))(pHAData->hSelf, pHAData->pAppData,
                                                       OMX_EventBufferFlag, OMX_DirOutput,
                                                       OMX_BUFFERFLAG_EOS, NULL);
            }

            HA_PostInputBuffer(pHAData);
        }
        while (HA_ErrorNone == s32ResourcesRet);
    }
    TRP_OUT();
}


static void OMX_AENC_BufferMgmtProcess(HI_AUDDATATYPE* pHAData)
{
    HI_S32 s32Ret;
    HI_S32 s32ResourcesRet;
    TRP_IN();

    // Buffer processing
    if (HI_TRUE == HA_CheckBufferState(pHAData))
    {
        //consider once decode to nodatastate
        do
        {
            s32ResourcesRet = HA_PreBuffer(pHAData);
            if (HA_ErrorNone != s32ResourcesRet)
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "HI_PreBuffer Failed");
                return;
            }

            if (pHAData->CodecFrame)
            {
                s32Ret = pHAData->CodecFrame(pHAData, pHAData->pInBufHdr, pHAData->pOutBufHdr);
            }
            else
            {
                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "DecodeFrame func Unregister\n");
                return;
            }

            if (HI_FALSE == pHAData->mEndOfInput)
            {
                if (HA_ErrorNone == s32Ret)
                {
                    HA_PostOutputBuffer(pHAData);
                }
            }
            else
            {
                pHAData->pOutBufHdr->nFlags = OMX_BUFFERFLAG_EOS;
                HA_PostOutputBuffer(pHAData);
                // Trigger EOS event handler
                OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s --->  pCallbacks->EventHandler   OMX_BUFFERFLAG_EOS\n", __func__);
                (*(pHAData->pCallbacks->EventHandler))(pHAData->hSelf, pHAData->pAppData,
                                                       OMX_EventBufferFlag, OMX_DirOutput,
                                                       OMX_BUFFERFLAG_EOS, NULL);
            }

            HA_PostInputBuffer(pHAData);
        }
        while (HA_ErrorNone == s32ResourcesRet);
    }
    TRP_OUT();
}

/*
 *  Component Thread
 *    The HI_OMX_ADEC_ComponentThread function is exeuted in a separate pThread and
 *    is used to implement the actual component functions.
 */

/*****************************************************************************/
void* HI_OMX_ADEC_ComponentThread(void* pThreadData)
{
    OMX_S32 nRetValue;
    HI_AUDDATATYPE* pHAData = (HI_AUDDATATYPE*)pThreadData;
    TRP_IN();

    OMX_OSAL_EventCreate(&pHAData->hTimeout);

    while (1)
    {
        nRetValue = OMX_CODEC_CommandMgmtProcess(pHAData);
        if (nRetValue == OMX_ErrorUndefined)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s --->  OMX_CODEC_CommandMgmtProcess stop,  >>>Exit<<< \n", __func__);
            goto EXIT;
        }

        if (pHAData->state == OMX_StateExecuting)
        {
            OMX_ADEC_BufferMgmtProcess(pHAData);
        }
    }

EXIT:
    OMX_OSAL_EventDestroy(pHAData->hTimeout);
    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s   >>>Exit<<< \n", __func__);
    TRP_OUT();
    return (void*)OMX_ErrorNone;
}


/*
 *  Component Thread
 *    The HI_OMX_AENC_ComponentThread function is exeuted in a separate pThread and
 *    is used to implement the actual component functions.
 */

/*****************************************************************************/
void* HI_OMX_AENC_ComponentThread(void* pThreadData)
{
    OMX_S32 nRetValue;
    HI_AUDDATATYPE* pHAData = (HI_AUDDATATYPE*)pThreadData;
    TRP_IN();

    OMX_OSAL_EventCreate(&pHAData->hTimeout);

    while (1)
    {
        nRetValue = OMX_CODEC_CommandMgmtProcess(pHAData);
        if (nRetValue == OMX_ErrorUndefined)
        {
            OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "%s --->  OMX_CODEC_CommandMgmtProcess stop,  >>>Exit<<< \n", __func__);
            goto EXIT;
        }

        if (pHAData->state == OMX_StateExecuting)
        {
            OMX_AENC_BufferMgmtProcess(pHAData);
        }
    }

EXIT:
    OMX_OSAL_EventDestroy(pHAData->hTimeout);
    OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "%s   >>>Exit<<< \n", __func__);
    TRP_OUT();
    return (void*)OMX_ErrorNone;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* File EOF */
