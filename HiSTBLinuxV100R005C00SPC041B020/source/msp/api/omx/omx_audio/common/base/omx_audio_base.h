/******************************************************************************
Copyright (C), 2001-2021, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : omx_audio_base.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/02/01
Last Modified :
Description   : define audio common data structure
  History       :
  1.Date        : 2012/02/01
    Author      : zgjie
    Modification: Created file
******************************************************************************/

#ifndef __OMX_AUDIO_BASE_H__
#define __OMX_AUDIO_BASE_H__

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
#include "hi_type.h"
#include "hi_audio_codec.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "unistd.h"

#define DOLBY_OMX_PASSTHRU_SUPPORT 1

#ifdef ANDROID
#include<utils/Log.h>
//#define LOG_NDEBUG 0
#define OMX_OSAL_Trace(level, fmt...) \
    do {  \
        switch(level) \
        { \
            case OMX_OSAL_TRACE_FATAL: \
            case OMX_OSAL_TRACE_WARNING: \
            case OMX_OSAL_TRACE_ERROR: \
                ALOGE(fmt);                                  \
                break;\
            case OMX_OSAL_TRACE_DEBUG: \
                ALOGV(fmt);                                  \
                break;\
            case OMX_OSAL_TRACE_INFO: \
                ALOGV(fmt);                                  \
                break;\
            default: \
                ALOGV(fmt);                                  \
                break;\
        } \
    } while (0)

#define max(x, y) (x >= y ? x : y)
#define TRP() ALOGV("%s->%d\n", __FUNCTION__, __LINE__)
#define TRP_IN() ALOGV("%s -> IN Line:%d  \n", __FUNCTION__, __LINE__)
#define TRP_OUT() ALOGV("%s -> OUT Line:%d  \n", __FUNCTION__, __LINE__)

#else
#if 0
#define OMX_OSAL_Trace(level, fmt...) \
    do {                                                     \
        printf("%s line[%d]:", __FUNCTION__, __LINE__); \
        printf(fmt);                                  \
    } while (0)
#define max(x, y) (x >= y ? x : y)

#define TRP() printf("%s->%d\n", __FUNCTION__, __LINE__)

#define TRP_IN() printf("%s -> IN Line:%d  \n", __FUNCTION__, __LINE__)
#define TRP_OUT() printf("%s -> OUT Line:%d  \n", __FUNCTION__, __LINE__)
#else
#define OMX_OSAL_Trace(level, fmt...) \
    do {                                                     \
    } while (0)
#define max(x, y) (x >= y ? x : y)
#define TRP()
#define TRP_IN()
#define TRP_OUT()
#endif

#endif

/*
 *  Declarations
 */
#define OMX_NOPORT             0xFFFFFFFE
#define OMX_TIMEOUT            10    // Timeout value in milisecond
#define OMX_MAX_TIMEOUTS       40      // Count of Maximum number of times the component can time out

#define NUM_MAX_BUFFERS        4      // MAX  Buffers (include input & output)
#define NUM_IN_BUFFERS         4      // Input  Buffers
#define NUM_OUT_BUFFERS        4      // Output Buffers

#define NON_ZERO               1

#define OMX_USEBUFFER          0      // UseBuffer manner
#define OMX_ALLOCATEBUFFER     1      // AllocateBuffer manner

#define OMX_IN_PORT_IDX        0      // InPort  Index
#define OMX_OUT_PORT_IDX       1      // OutPort Index

typedef enum DECODERID
{
    DECODER_MP3 = 0,
    DECODER_AAC,
    DECODER_DDP,
    DECODER_BUTT
} DECODERID;


typedef enum PIPEPROP
{
    PIPE_READ = 0,
    PIPE_WRITE,
    PIPE_BUTT
} PIPEPROP;

typedef enum BufferStatus
{
    OWNED_BY_NULL,
    OWNED_BY_US,
    OWNED_BY_COMPONENT,
    OWNED_BY_CLIENT,
} BufferStatus;


/*
 *  Definitions
 */

typedef struct _BufferList BufferList;

/*
 * The main structure for buffer management.
 *
 *   pBufHdr     - An array of pointers to buffer headers.
 *                 The size of the array is set dynamically using the nBufferCountActual value
 *                   send by the client.
 *   nListEnd    - Marker to the boundary of the array. This points to the last index of the
 *                   pBufHdr array.
 *   nSizeOfList - Count of valid data in the list.
 *   nAllocSize  - Size of the allocated list. This is equal to (nListEnd + 1) in most of
 *                   the times. When the list is freed this is decremented and at that
 *                   time the value is not equal to (nListEnd + 1). This is because
 *                   the list is not freed from the end and hence we cannot decrement
 *                   nListEnd each time we free an element in the list. When nAllocSize is zero,
 *                   the list is completely freed and the other paramaters of the list are
 *                   initialized.
 *                 If the client crashes before freeing up the buffers, this parameter is
 *                   checked (for nonzero value) to see if there are still elements on the list.
 *                   If yes, then the remaining elements are freed.
 *    nWritePos  - The position where the next buffer would be written. The value is incremented
 *                   after the write. It is wrapped around when it is greater than nListEnd.
 *    nReadPos   - The position from where the next buffer would be read. The value is incremented
 *                   after the read. It is wrapped around when it is greater than nListEnd.
 *    eDir       - Type of BufferList.
 *                            OMX_DirInput  =  Input  Buffer List
 *                           OMX_DirOutput  =  Output Buffer List
 *    bufferOwner       - ells about owner of each buffer for UseBuffer or AllocateBuffer.  add by z40717
 *                            0  =  UseBuffer
 *                           1   =  AllocateBuffer
 */
struct _BufferList
{
    OMX_BUFFERHEADERTYPE** pBufHdr;
    OMX_S32                nListEnd;
    OMX_S32                nSizeOfList;
    OMX_U32                nAllocSize;
    OMX_U32                addralloclist[NUM_MAX_BUFFERS];
    OMX_S32                nWritePos;
    OMX_S32                nReadPos;
    OMX_U32                bufferindex[NUM_MAX_BUFFERS];
    OMX_DIRTYPE            eDir;
    OMX_U32                bufferOwner[NUM_MAX_BUFFERS];
};


typedef enum
{
    OMX_HA_CommandStopThread = OMX_CommandVendorStartUnused,
    OMX_HA_CommandFillBuf,
    OMX_HA_CommandEmptyBuf,
    OMX_HA_CommandUnpopulatedPort
} OMX_HA_COMMANDTYPE;


/*
 * Private data of the component.
 * It includes input and output port related information (Port definition, format),
 *   buffer related information, specifications for the audio decoder, the command and data pipes
 *   and the BufferList structure for storing input and output buffers.
 */

typedef struct
{
    HI_VOID*  pInBuffer;
    HI_S32    s32Insize;
    HI_S32    s32Offset;
} HA_INTERNALBUF_S;


typedef struct
{
    HI_HA_DECODE_S*         pstHA;
    HI_HADECODE_OPENPARAM_S sOpenPram;
    HA_INTERNALBUF_S        sInternalBuf;
    HI_BOOL                 bPassThrough;
    HI_BOOL                 bPassThroughOutputEnable;
    HI_BOOL                 bPacketDecoder;
    HI_U32                  u32BitstreamOffset;
    HI_HANDLE               hTrack;
    HI_U16                 *pWkbuf;    //work buffer: Use to unify 24 bitdepth to 16 bitdepth
} HA_ADEC_S;


typedef struct
{
    HI_HA_ENCODE_S*         pstHA;
    HI_HAENCODE_OPENPARAM_S sOpenPram;
    HA_INTERNALBUF_S        sInternalBuf;
} HA_AENC_S;


typedef struct
{
    OMX_STATETYPE                  state;
    OMX_CALLBACKTYPE*              pCallbacks;
    OMX_PTR                        pAppData;
    OMX_HANDLETYPE                 hSelf;
    OMX_PARAM_PORTDEFINITIONTYPE   sInPortDef;
    OMX_PARAM_PORTDEFINITIONTYPE   sOutPortDef;
    OMX_PRIORITYMGMTTYPE           sPriorityMgmt;
    OMX_AUDIO_PARAM_PCMMODETYPE    sPcm;
    pthread_t                      thread_id;
    fd_set                         rfds;
    OMX_U32                        datapipe[PIPE_BUTT];
    OMX_U32                        cmdpipe[PIPE_BUTT];
    OMX_U32                        cmddatapipe[PIPE_BUTT];
    BufferList                     sInBufList;
    BufferList                     sOutBufList;
    OMX_HANDLETYPE                 hTimeout;
    OMX_BUFFERHEADERTYPE*          pInBufHdr;
    OMX_BUFFERHEADERTYPE*          pOutBufHdr;
    /*private data*/
    HI_BOOL                        bPortSetChange;
    HI_BOOL                        bInnerBufFlag;
    HI_BOOL                        bFirstFrame;
    OMX_TICKS                      mAnchorTimeUs;
    HA_ADEC_S                      stAdec;
    HI_VOID*                       hDecoder;
    OMX_PTR                        pCodecParam;
    HI_VOID*                       pCodecPrivData;

    BufferStatus                   enInBufState;
    HI_BOOL                        bNewPacketIn;
    HI_BOOL                        mEndOfInput;
    OMX_U32                        mInputBufferCnt;
    /**
    \brief This method is used to decode/encode a frame. CNcomment:½âÂëÆ÷½âÂë/±àÂëÆ÷±àÂëÒ»Ö¡·½·¨ CNend
    \attention \n
    \param[in] pHAData   the OMX audo data handle CNcomment:OMXÒôÆµÊý¾Ý¾ä±ú CNend
    \param[in] pInBufHdr pointer to audio input packet CNcomment:ÒôÆµÊäÈëÖ¸Õë CNend
    \param[out] pOutBufHdr pointer to audio output packet CNcomment:ÒôÆµÊä³öÖ¸Õë CNend
    \retval ::HA_ErrorNone  SUCCESS CNcomment:³É¹¦ CNend
    \retval ::HI_HA_ERRORTYPE_E FAILURE CNcomment:Ê§°Ü CNend
    \see \n
    N/A
    */
    HI_S32 (*CodecFrame)(HI_VOID* pHAData,
                         OMX_BUFFERHEADERTYPE* pInBufHdr,
                         OMX_BUFFERHEADERTYPE* pOutBufHdr);

    HI_VOID*                       hEncoder;
    HA_AENC_S                      stAenc;

#ifdef ADEC_FILE_SAVE
    FILE*                          pInFile;
    FILE*                          pOutFile;
#endif
} HI_AUDDATATYPE;


/*
 *   Macros
 */

/*
 * Initializes a data structure using a pointer to the structure.
 * The initialization of OMX structures always sets up the nSize and nVersion fields
 *   of the structure.
 */
#define OMX_CONF_INIT_STRUCT_PTR(_s_, _name_) \
    do {                                                     \
        memset((_s_), 0x0, sizeof(_name_)); \
        (_s_)->nSize = sizeof(_name_);      \
        (_s_)->nVersion.s.nVersionMajor = 0x1;  \
        (_s_)->nVersion.s.nVersionMinor = 0x0;  \
        (_s_)->nVersion.s.nRevision = 0x0;      \
        (_s_)->nVersion.s.nStep = 0x0; \
    } while (0)

/*
 * Checking for version compliance.
 * If the nSize of the OMX structure is not set, raises bad parameter error.
 * In case of version mismatch, raises a version mismatch error.
 */
#define OMX_CONF_CHK_VERSION(_s_, _name_, _e_) \
    do {                                                     \
        if ((_s_)->nSize != sizeof(_name_)) _e_ = OMX_ErrorBadParameter;\
        if (((_s_)->nVersion.s.nVersionMajor != 0x1)    \
            || ((_s_)->nVersion.s.nVersionMinor != 0x0)    \
            || ((_s_)->nVersion.s.nRevision != 0x0)    \
            || ((_s_)->nVersion.s.nStep != 0x0)) _e_ = OMX_ErrorVersionMismatch;\
        if (_e_ != OMX_ErrorNone) goto OMX_CONF_CMD_BAIL;\
    } while (0)

/*
 * Checking paramaters for non-NULL values.
 * The macro takes three parameters because inside the code the highest
 *   number of parameters passed for checking in a single instance is three.
 * In case one or two parameters are passed, the ramaining parameters
 *   are set to 1 (or a nonzero value).
 */
#define OMX_CONF_CHECK_CMD(_ptr1, _ptr2, _ptr3) \
    do {                                                     \
        if (!_ptr1 || !_ptr2 || !_ptr3){     \
            eError = OMX_ErrorBadParameter;     \
            goto OMX_CONF_CMD_BAIL;         \
        }                       \
    } while (0)

/*
 * Redirects control flow in an error situation.
 * The OMX_CONF_CMD_BAIL label is defined inside the calling function.
 */
#define OMX_CONF_BAIL_IF_ERROR(_eError) \
    do {                                                     \
        if (_eError != OMX_ErrorNone) \
            goto OMX_CONF_CMD_BAIL;\
    } while (0)

/*
 * Sets error type and redirects control flow to error handling and cleanup section
 */
#define OMX_CONF_SET_ERROR_BAIL(_eError, _eCode) \
    do {                                                     \
        _eError = _eCode;               \
        goto OMX_CONF_CMD_BAIL;         \
    } while (0)

/*
 * Allocates a new entry in a BufferList.
 * Finds the position where memory has to be allocated.
 * Actual allocation happens in the caller function.
 */
#define ListAllocate(_pH, _nIndex) \
    do {                                                     \
        if (_pH.nListEnd == -1){                     \
            _pH.nListEnd  = 0;                         \
            _pH.nWritePos = 0;                        \
        }                                         \
        else \
            _pH.nListEnd++;\
        _pH.nAllocSize++;                            \
        _nIndex = _pH.nListEnd; \
    } while (0)

/*
 * Sets an entry in the BufferList.
 * The entry set is a BufferHeader.
 * The nWritePos value is incremented after the write.
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListSetEntry(_pH, _pB) \
    do {                                                     \
        OMX_U32 nIndex;       \
        for (nIndex = 0; nIndex < NUM_OUT_BUFFERS; nIndex++) \
        { \
            if (_pB == _pH.addralloclist[nIndex])  \
            { \
                _pH.bufferindex[_pH.nWritePos] = nIndex;  \
            } \
        } \
        if (_pH.nSizeOfList < (_pH.nListEnd + 1)){   \
            _pH.nSizeOfList++;                        \
            _pH.pBufHdr[_pH.bufferindex[_pH.nWritePos++]] = _pB;       \
            if (_pH.nReadPos == -1) \
                _pH.nReadPos = 0;\
            if (_pH.nWritePos > _pH.nListEnd) \
                _pH.nWritePos = 0;\
        } \
    } while (0)

#define ListDump(_pH) \
    do { \
        int i = 0; \
        for(i = 0; i < _pH.nSizeOfList; i++) \
        { \
            OMX_OSAL_Trace(OMX_OSAL_TRACE_DEBUG, "ListDump pH.pBufHdr[%d] =%p\n", i, _pH.pBufHdr[i]); \
        } \
    } while (0)

/*
 * Gets an entry from the BufferList
 * The entry is a BufferHeader
 * The nReadPos value is incremented after the read.
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListGetEntry(_pH, _pB) \
    do {                                                     \
        if (_pH.nSizeOfList > 0){                    \
            _pH.nSizeOfList--;                        \
            _pB = _pH.pBufHdr[_pH.bufferindex[_pH.nReadPos++]];        \
            if (_pH.nReadPos > _pH.nListEnd) \
                _pH.nReadPos = 0;\
        } \
    } while (0)


#define ListReleaseEntry(_pH, _pB) \
    do {                                                     \
        if (_pH.nSizeOfList > 0){                    \
            _pH.nSizeOfList--;                        \
            _pB = _pH.pBufHdr[_pH.bufferindex[_pH.nReadPos++]];        \
            if (_pH.nReadPos > _pH.nListEnd) \
                _pH.nReadPos = 0;\
        } \
    } while (0)


/*
 * Cache an entry from the BufferList
 * The entry is a BufferHeader
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListCacheEntry(_pH, _pB) \
    do {                                                     \
        if (_pH.nSizeOfList > 0){                    \
            _pB = _pH.pBufHdr[_pH.bufferindex[_pH.nReadPos]]; \
        } \
        else {\
            _pB = NULL; \
        } \
    } while (0)

/*
 * Flushes all entries from the BufferList structure.
 * The nSizeOfList gives the number of valid entries in the list.
 * The nReadPos value is incremented after the read.
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListFlushEntries(_pH, _pC) \
    do {                                            \
        while (_pH.nSizeOfList > 0){                \
            _pH.nSizeOfList--;                      \
            if (_pH.eDir == OMX_DirInput) \
                _pC->pCallbacks->EmptyBufferDone(_pC->hSelf, _pC->pAppData, _pH.pBufHdr[_pH.bufferindex[_pH.nReadPos++]]);\
            else if (_pH.eDir == OMX_DirOutput) \
                _pC->pCallbacks->FillBufferDone(_pC->hSelf, _pC->pAppData, _pH.pBufHdr[_pH.bufferindex[_pH.nReadPos++]]);\
            if (_pH.nReadPos > _pH.nListEnd) \
                _pH.nReadPos = 0;\
        } \
    } while (0)

/*
 * Frees the memory allocated for BufferList entries
 *   by comparing with client supplied buffer header.
 * The nAllocSize value gives the number of allocated (i.e. not free'd) entries in the list.
 * When nAllocSize is zero, the list is completely freed
 *   and the other paramaters of the list are initialized.
 */
#define ListFreeBuffer(_pH, _pB, _pP, _nIndex) \
    do {                                                     \
        for (_nIndex = 0; _nIndex <= _pH.nListEnd; _nIndex++){           \
            if (_pH.pBufHdr[_nIndex] == _pB){                            \
                _pH.nAllocSize--;                                         \
                if (_pH.pBufHdr[_nIndex]){                                \
                    if (_pH.pBufHdr[_nIndex]->pBuffer)   {         \
                        if (_pH.bufferOwner[_nIndex] == OMX_ALLOCATEBUFFER) \
                            OMX_OSAL_Free(_pH.pBufHdr[_nIndex]->pBuffer);\
                        _pH.pBufHdr[_nIndex]->pBuffer = NULL;               \
                    } \
                    OMX_BUFFERHEADERTYPE *bufhdr = (OMX_BUFFERHEADERTYPE *)_pH.pBufHdr[_nIndex]; \
                    OMX_OSAL_Free(bufhdr);                                 \
                    _pH.pBufHdr[_nIndex] = NULL;                           \
                }                                                      \
                if (_pH.nAllocSize == 0){                                 \
                    _pH.nWritePos = -1;                                    \
                    _pH.nReadPos = -1;                                     \
                    _pH.nListEnd = -1;                                     \
                    _pH.nSizeOfList = 0;                                   \
                    _pP->bPopulated = OMX_FALSE;                  \
                }                                                      \
                break;                                                    \
            }                                                         \
        } \
    } while (0)

/*
 * Frees the memory allocated for BufferList entries.
 * This is called in case the client crashes suddenly before freeing all the component buffers.
 * The nAllocSize parameter is
 *   checked (for nonzero value) to see if there are still elements on the list.
 * If yes, then the remaining elements are freed.
 */
#define ListFreeAllBuffers(_pH, _nIndex) \
    do {                                                     \
        for (_nIndex = 0; _nIndex <= _pH.nListEnd; _nIndex++){           \
            if (_pH.pBufHdr[_nIndex]){                                   \
                _pH.nAllocSize--;                                         \
                if (_pH.pBufHdr[_nIndex]->pBuffer)   {         \
                    if (_pH.bufferOwner[_nIndex] == OMX_ALLOCATEBUFFER) \
                        OMX_OSAL_Free(_pH.pBufHdr[_nIndex]->pBuffer);\
                    _pH.pBufHdr[_nIndex]->pBuffer = NULL;               \
                }  \
                OMX_BUFFERHEADERTYPE *bufhdr = (OMX_BUFFERHEADERTYPE *)_pH.pBufHdr[_nIndex]; \
                OMX_OSAL_Free(bufhdr);                                    \
                _pH.pBufHdr[_nIndex] = NULL;                              \
                if (_pH.nAllocSize == 0){                                 \
                    _pH.nWritePos = -1;                                    \
                    _pH.nReadPos = -1;                                     \
                    _pH.nListEnd = -1;                                     \
                    _pH.nSizeOfList = 0;                                   \
                }                                                      \
            }                                                         \
        } \
    } while (0)

/*
 * Loads the parameters of the buffer header.
 * When the list has nBufferCountActual elements allocated
 *   then the bPopulated value of port definition is set to true.
 */
#define LoadBufferHeader(_pList, _pBufHdr, _pAppPrivate, _nSizeBytes, _nPortIndex, \
                         _ppBufHdr, _pPortDef) \
do {                                                     \
    _pBufHdr->nAllocLen   = _nSizeBytes;                                                \
    _pBufHdr->pAppPrivate = _pAppPrivate;                                             \
    if (_pList.eDir == OMX_DirInput){                                                 \
        _pBufHdr->nInputPortIndex  = _nPortIndex;                                       \
        _pBufHdr->nOutputPortIndex = OMX_NOPORT;                                       \
    }                                                                              \
    else {                                                                             \
        _pBufHdr->nInputPortIndex  = OMX_NOPORT;                                        \
        _pBufHdr->nOutputPortIndex = _nPortIndex;                                      \
    }                                                                              \
    _ppBufHdr = _pBufHdr;                                                             \
    if (_pList.nListEnd == (_pPortDef->nBufferCountActual - 1)) \
        _pPortDef->bPopulated = OMX_TRUE;\
} while (0)

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_GetState(OMX_IN OMX_HANDLETYPE  hComponent,
                                    OMX_OUT OMX_STATETYPE* pState);

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_SendCommand(OMX_IN OMX_HANDLETYPE  hComponent,
                                       OMX_IN OMX_COMMANDTYPE Cmd,
                                       OMX_IN OMX_U32         nParam1,
                                       OMX_IN OMX_PTR         pCmdData);

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_SetCallbacks(OMX_IN OMX_HANDLETYPE    hComponent,
                                        OMX_IN OMX_CALLBACKTYPE* pCallbacks,
                                        OMX_IN OMX_PTR           pAppData);

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_UseBuffer(OMX_IN OMX_HANDLETYPE            hComponent,
                                     OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
                                     OMX_IN OMX_U32                   nPortIndex,
                                     OMX_IN OMX_PTR                   pAppPrivate,
                                     OMX_IN OMX_U32                   nSizeBytes,
                                     OMX_IN OMX_U8*                   pBuffer);
OMX_ERRORTYPE HI_OMX_CODEC_AllocateBuffer(OMX_IN OMX_HANDLETYPE            hComponent,
        OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
        OMX_IN OMX_U32                   nPortIndex,
        OMX_IN OMX_PTR                   pAppPrivate,
        OMX_IN OMX_U32                   nSizeBytes);
OMX_ERRORTYPE HI_OMX_CODEC_FreeBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
                                      OMX_IN OMX_U32               nPortIndex,
                                      OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);

OMX_ERRORTYPE HI_OMX_CODEC_EmptyThisBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
        OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);
OMX_ERRORTYPE HI_OMX_CODEC_FillThisBuffer(OMX_IN OMX_HANDLETYPE        hComponent,
        OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);

/*****************************************************************************/
OMX_ERRORTYPE HI_OMX_CODEC_ComponentDeInit(OMX_IN OMX_HANDLETYPE hComponent);

/*
 *  Component Thread
 *    The HI_OMX_ADEC_ComponentThread function is exeuted in a separate pThread and
 *    is used to implement the actual component functions.
 */
void*         HI_OMX_ADEC_ComponentThread(void* pThreadData);

/*
 *  Component Thread
 *    The HI_OMX_AENC_ComponentThread function is exeuted in a separate pThread and
 *    is used to implement the actual component functions.
 */
void*         HI_OMX_AENC_ComponentThread(void* pThreadData);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* #ifndef __HISI_AUDIO_CODEC_H__ */

/* File EOF */
