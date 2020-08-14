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
#ifndef __HI_AENC_H__
#define __HI_AENC_H__

#include "hi_type.h"
#include "hi_unf_sound.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
typedef struct hiAENC_ATTR_S
{
    HI_U32               u32CodecID;
    HI_U32               u32InBufSize;               /* Input buffer  size              */
    HI_U32               u32OutBufNum;             /* Output buffer number, buffer size depend on  u32CodecID       */
    HI_HAENCODE_OPENPARAM_S sOpenParam;
} AENC_ATTR_S;

/* Inputting audio stream structure                                                     */
typedef struct hiAENC_STREAM_S
{
    HI_U8  *pu8Data;
    HI_U32  u32Bytes;
    HI_U32  u32PtsMs;
} AENC_STREAM_S;


HI_S32 HI_MPI_AENC_RegisterEncoder(const HI_CHAR *pszCodecDllName);
HI_S32 HI_MPI_AENC_SetConfigEncoder(HI_HANDLE hAenc, HI_VOID *pstConfigStructure);
HI_S32 HI_MPI_AENC_Init(const HI_CHAR* pszCodecNameTable[]);
HI_S32 HI_MPI_AENC_DeInit(HI_VOID);
HI_S32 HI_MPI_AENC_Open(HI_HANDLE *phAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);
HI_S32 HI_MPI_AENC_Close (HI_HANDLE hAenc);
HI_S32 HI_MPI_AENC_SendBuffer(HI_U32 hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
HI_S32 HI_MPI_AENC_ReceiveStream(HI_HANDLE hAenc, AENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs);
HI_S32 HI_MPI_AENC_ReleaseStream(HI_HANDLE hAenc, const AENC_STREAM_S *pstStream);
HI_S32 HI_MPI_AENC_SetEnable(HI_HANDLE hAenc, HI_BOOL bEnable);
HI_S32 HI_MPI_AENC_AttachInput(HI_HANDLE hAenc, HI_HANDLE hSource);
HI_S32 HI_MPI_AENC_GetAttachSrc(HI_HANDLE hAenc, HI_HANDLE *hSrc);
HI_S32 HI_MPI_AENC_DetachInput(HI_HANDLE hAenc);
HI_S32 HI_MPI_AENC_SetAttr(HI_HANDLE hAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);
HI_S32 HI_MPI_AENC_GetAttr(HI_HANDLE hAenc, HI_UNF_AENC_ATTR_S *pstAencAttr);


#if 0
HI_S32 HI_MPI_AENC_ShowRegisterEncoder(HI_VOID);
HI_S32 HI_MPI_AENC_SetAutoSRC(HI_HANDLE hAenc, HI_BOOL bEnable);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_AENC_H__ */

