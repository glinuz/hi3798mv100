/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
 File Name     : hi_mpi_adec.h
 Version       : Initial Draft
 Author        : Hisilicon multimedia software group
 Created       :
 Last Modified :
 Description   :
 Function List :
 History       :
******************************************************************************/
#ifndef __HI_MPI_ADEC_H__
#define __HI_MPI_ADEC_H__

#include "hi_type.h"
#include "hi_unf_sound.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define HA_DECODE_LIB_NAME_LEN 50

typedef enum
{
    HI_MPI_ADEC_STATUSINFO = 0,
    HI_MPI_ADEC_DEBUGINFO,
    HI_MPI_ADEC_STREAMINFO,
    HI_MPI_ADEC_BUFFERSTATUS,
    HI_MPI_ADEC_HaSzNameInfo
} HI_MPI_ADEC_INFO_E;

typedef struct
{
    HI_BOOL              bEnable;           /* Enable ADEC module */
    HI_BOOL              bEosState;         /* Eos Flag */
    HI_U32               u32CodecID;        /* Codec type */
    HI_U32               u32InBufSize;      /* Input buffer size */
    HI_U32               u32OutBufNum;      /* Output buffer number, buffer size depends on u32CodecID */
    HI_HADECODE_OPENPARAM_S sOpenPram;      /* Decoder open param */
    HI_BOOL              bDualDec;
} ADEC_ATTR_S;

typedef struct
{
    HI_U32 u32BufferSize;            /* Total buffer size, in the unit of byte.*/
    HI_U32 u32BufferAvailable;       /* Available buffer, in the unit of byte.*/
    HI_U32 u32BufferUsed;            /* Used buffer, in the unit of byte.*/
    HI_S32 s32BufReadPos;            /*buffer s32BufReadPos ptr*/
    HI_U32 u32BufWritePos;           /*buffer u32BufWritePos ptr*/
    HI_U32 u32TotDecodeFrame;        /* Total Deocded Frame number.*/
    HI_BOOL bEndOfFrame;             /*EOS flag*/
    HI_U32 u32SamplesPerFrame;
} ADEC_BUFSTATUS_S, *PTR_ADEC_BUFSTATUS_S;

/* Audio output information structure                                                   */
typedef struct
{
    HI_BOOL bWorking;

    /* OutBuf Status */
    HI_U32  u32OutBufNum;
    HI_U32  u32UsedBufNum;
    HI_U32  u32OutBufDurationMs;

    /* InBuf Status */
    HI_U32  u32BufferSize;           /* Total buffer size, in the unit of byte.*/
    HI_U32  u32BufferAvailable;      /* Available buffer, in the unit of byte.*/
    HI_U32  u32BufferUsed;           /* Used buffer, in the unit of byte.*/
    /* ADInBuf Status */
    HI_U32  u32ADBufferSize;        /* Total buffer size, in the unit of byte.*/
    HI_U32  u32ADBufferAvailable;   /* Available buffer, in the unit of byte.*/
    HI_U32  u32ADBufferUsed;        /* Used buffer, in the unit of byte.*/

    /* statistical Status */
    HI_U32  u32TotDecodeFrame;          /* Total Deocded Frame number.*/
    HI_U32  u32FrameDurationMs;
    HI_U32  u32CodecID;
    HI_BOOL enFmt;
    HI_U32  u32Channels;
    HI_UNF_SAMPLE_RATE_E enSampleRate;
    HI_UNF_BIT_DEPTH_E   enBitDepth;
} ADEC_STATUSINFO_S;

/* Audio output debug information structure */
typedef struct
{
    HI_U32 u32DecFrameNum;
    HI_U32 u32ErrDecFrameNum;
} ADEC_DEBUGINFO_S;

typedef struct
{
    HI_CHAR szHaCodecName[32];
} ADEC_SzNameINFO_S;

/* Outputting audio stream structure */
typedef struct
{
    HI_U32               u32CodecID;
    HI_UNF_SAMPLE_RATE_E enSampleRate;
} ADEC_STREAMINFO_S;

/* Outputting audio frame extend infomation */
typedef struct
{
    HI_U32               u32FrameLeftNum;            /* frame umber left at adef buffer */
    HI_U32               u32FrameDurationMs;         /* frame duration  */
    HI_U32               u32OrgPtsMs;
} ADEC_EXTFRAMEINFO_S;

typedef struct
{
    HI_BOOL  bFrameInfoChange;
    HI_BOOL  bUnSupportFormat;
    HI_BOOL  bStreamCorrupt;

    HI_UNF_ACODEC_STREAMINFO_S stStreamInfo;
} ADEC_EVENT_S;

typedef struct hiHA_DECODE_LIB_S
{
    HI_U32 enCodecID;
    HI_CHAR szLibName[HA_DECODE_LIB_NAME_LEN];
    struct hiHA_DECODE_LIB_S *pstNext;
} HI_HA_DECODE_LIB_S;

HI_S32 HI_MPI_ADEC_RegisterDeoderLib(const HI_CHAR* pszCodecDllName, HI_U32 u32Length);
HI_S32 HI_MPI_ADEC_FoundSupportDeoder(HA_FORMAT_E enFormat, HI_U32* penDstCodecID);
HI_S32 HI_MPI_ADEC_GetDecoderList(HI_HA_DECODE_LIB_S** ppHaLib);

HI_S32 HI_MPI_ADEC_SetConfigDeoder(const HI_U32 enDstCodecID, HI_VOID* pstConfigStructure);
HI_S32 HI_MPI_ADEC_Init(const HI_CHAR* pszCodecNameTable[]);
HI_S32 HI_MPI_ADEC_deInit(HI_VOID);
HI_S32 HI_MPI_ADEC_Open(HI_HANDLE* phAdec);
HI_S32 HI_MPI_ADEC_Close(HI_HANDLE hAdec);
HI_S32 HI_MPI_ADEC_Start(HI_HANDLE hAdec);
HI_S32 HI_MPI_ADEC_Stop(HI_HANDLE hAdec);
HI_S32 HI_MPI_ADEC_SetAllAttr(HI_HANDLE hAdec, ADEC_ATTR_S* pstAllAttr);
HI_S32 HI_MPI_ADEC_GetDelayMs(HI_HANDLE hAdec, HI_U32* pDelay);
HI_S32 HI_MPI_ADEC_GetAllAttr(HI_HANDLE hAdec, ADEC_ATTR_S* pstAllAttr);
HI_S32 HI_MPI_ADEC_SendStream(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S* pstStream, HI_U32 u32PtsMs);
HI_S32 HI_MPI_ADEC_GetBuffer(HI_HANDLE hAdec, HI_U32 u32RequestSize, HI_UNF_STREAM_BUF_S* pstStream);
HI_S32 HI_MPI_ADEC_PutBuffer(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S* pstStream, HI_U32 u32PtsMs);
HI_S32 HI_MPI_ADEC_ReleaseFrame(HI_HANDLE hAdec, const HI_UNF_AO_FRAMEINFO_S* pstAOFrame);
HI_S32 HI_MPI_ADEC_GetADBuffer(HI_HANDLE hAdec, HI_U32 u32RequestSize, HI_UNF_STREAM_BUF_S* pstStream);
HI_S32 HI_MPI_ADEC_PutADBuffer(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S* pstStream, HI_U32 u32PtsMs);
HI_S32 HI_MPI_ADEC_SendADStream(HI_HANDLE hAdec, const HI_UNF_STREAM_BUF_S* pstStream, HI_U32 u32PtsMs);
HI_S32 HI_MPI_ADEC_SetADMixAttr(HI_HANDLE hAdec, HI_BOOL bAdEnable, HI_BOOL bAdToSomePort);
HI_S32 HI_MPI_ADEC_GetADMixAttr(HI_HANDLE hAdec, HI_BOOL* bAdEnable, HI_BOOL* bAdToSomePort);
HI_S32 HI_MPI_ADEC_SetADBalance(HI_HANDLE hAdec, HI_S16 s16ADBalance);
HI_S32 HI_MPI_ADEC_GetADBalance(HI_HANDLE hAdec, HI_S16* ps16ADBalance);
HI_S32 HI_MPI_ADEC_GetADDelayMs(HI_HANDLE hAdec, HI_U32* pDelay);
HI_S32 HI_MPI_ADEC_SetADEosFlag(HI_HANDLE hAdec);
HI_S32 HI_MPI_ADEC_GetInfo(HI_HANDLE hAdec, HI_MPI_ADEC_INFO_E enAdecInfo, void* pstadecinfo);
HI_S32 HI_MPI_ADEC_GetAudSpectrum(HI_HANDLE hAdec, HI_U16* pSpectrum , HI_U32 u32BandNum);
HI_S32 HI_MPI_ADEC_ReceiveFrame(HI_HANDLE hAdec, HI_UNF_AO_FRAMEINFO_S* pstAOFrame, ADEC_EXTFRAMEINFO_S* pstExtInfo);
HI_S32 HI_MPI_ADEC_SetEosFlag(HI_HANDLE hAdec);
HI_S32 HI_MPI_ADEC_DropStream(HI_HANDLE hAdec, HI_U32 u32SeekPts);
HI_S32 HI_MPI_ADEC_SetCodecCmd(HI_HANDLE hAdec, HI_VOID* pstCodecCmd);
HI_S32 HI_MPI_ADEC_CheckNewEvent(HI_HANDLE hAdec, ADEC_EVENT_S* pstNewEvent);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_MPI_ADEC_H__ */
