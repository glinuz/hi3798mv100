/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_vdec_buf_mng.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/08/09
  Description   : Definitions of buffer manager.
  History       :
  1.Date        : 2012/08/09
    Author      : l00185424
    Modification: Created file

*******************************************************************************/

#ifndef __HI_VDEC_BUFFER_MNG_H__
#define __HI_VDEC_BUFFER_MNG_H__

/******************************* Include Files *******************************/

#include "hi_type.h"
#include "hi_error_mpi.h"
#include "hi_drv_video.h"
#ifdef HI_TEE_SUPPORT
#include "sec_mmz.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/****************************** Macro Definition *****************************/


#define BUFMNG_DEBUG                    (0)
#define BUFMNG_64BITS_PTS_SUPPORT       (0)
#define BUFMNG_MARKER_SUPPORT           (1)
#define BUFMNG_PARTED_FRAME_SUPPORT     (1)
#define BUFMNG_INDEX_SUPPORT            (0)
#define BUFMNG_MULTI_READ_SUPPORT       (1)
#define BUFMNG_GET_SMSP                 (0) /* Get As More As Possible */

/* Error Definition */
#define HI_ERR_BM_INVALID_PARA      (HI_ERR_VDEC_INVALID_PARA)
#define HI_ERR_BM_NO_MEMORY         (HI_ERR_VDEC_MALLOC_FAILED)
#define HI_ERR_BM_BUFFER_FULL       (HI_ERR_VDEC_BUFFER_FULL)
#define HI_ERR_BM_BUFFER_EMPTY      (HI_FAILURE)
#define HI_ERR_BM_FREE_ERR          (HI_FAILURE)
#define HI_ERR_BM_WRITE_FREE_ERR    (HI_FAILURE)
#define HI_ERR_BM_READ_FREE_ERR     (HI_FAILURE)
#define HI_ERR_BM_BUSY              (HI_ERR_VDEC_INVALID_STATE)

#define BUFMNG_NOT_END_FRAME_BIT     (0x00000001)
#define BUFMNG_END_OF_STREAM_BIT    (0x00000002)
#define BUFMNG_DISCONTINUOUS_BIT    (0x00000004)
#define BUFMNG_EXTERN_EOP_BIT       (0x00000008)
#ifdef HI_TEE_SUPPORT
#define BUFMNG_SEC_ZONE             "SEC-MMZ"
#endif
/*************************** Structure Definition ****************************/

typedef enum tagBUFMNG_ALLOC_TYPE_E
{
    BUFMNG_ALLOC_INNER = 0, /* Allocate by BUGMNG */
    BUFMNG_ALLOC_OUTER,     /* Allocate by user */
    BUFMNG_ALLOC_BUTT
}BUFMNG_ALLOC_TYPE_E;

typedef struct tagBUFMNG_INST_CONFIG_S
{
    BUFMNG_ALLOC_TYPE_E enAllocType;    /* MMZ allocate type */
    HI_U32 u32PhyAddr;      /* Start physical address. */
    HI_U8* pu8UsrVirAddr;   /* Start user virtual address. */
    HI_U8* pu8KnlVirAddr;   /* Start kernel virtual address. */
    HI_U32 u32Size;         /* Size */
    HI_CHAR aszName[16];    /* Buffer name */
#ifdef HI_TEE_SUPPORT
    HI_BOOL bTvp;
#endif
}BUFMNG_INST_CONFIG_S;

typedef struct tagBUFMNG_BUF_S
{
    HI_U32 u32PhyAddr;      /* Physical address */
    HI_U8* pu8UsrVirAddr;   /* User virtual address */
    HI_U8* pu8KnlVirAddr;   /* Kernel virtual address */
    HI_U32 u32Size;         /* Buffer size, in the unit of byte.*/
    HI_U64 u64Pts;          /* PTS of the data filled in a buffer.*/
    HI_U32 u32Index;        /* Index, always output, don't set */
    HI_U32 u32Marker;       /* bit0: 0:Frame over/1:Half frame; bit1: 1:Stream over */
}BUFMNG_BUF_S;

typedef struct tagBUF_VPSS_S
{
	MMZ_BUFFER_S MMZBuf_frm;
	MMZ_BUFFER_S MMZBuf_meta;
}BUF_VPSS_S;

typedef struct tagBM_STATUS_S
{
    HI_U32 u32Used;
    HI_U32 u32Free;
    HI_U32 u32DataNum;      /* For stream mode, it is undecoded packet number. 
                               For frame mode, it is undecoded frame number, support BUFMNG_NOT_END_FRAME_BIT.*/
    HI_U32 u32GetTry;       /* GetWriteBuf try times */
    HI_U32 u32GetOK;        /* GetWriteBuf ok times */
    HI_U32 u32PutTry;       /* PutWriteBuf try times */
    HI_U32 u32PutOK;        /* PutWriteBuf ok times */
    HI_U32 u32RecvTry;      /* GetReadBuf try times */
    HI_U32 u32RecvOK;       /* GetReadBuf ok times */
    HI_U32 u32RlsTry;       /* PutReadBuf try times */
    HI_U32 u32RlsOK;        /* PutReadBuf ok times */
}BUFMNG_STATUS_S;

/******************************* API Declaration *****************************/

HI_VOID BUFMNG_Init(HI_VOID);
HI_S32 BUFMNG_DeInit(HI_VOID);
HI_S32 BUFMNG_Create(HI_HANDLE *phBuf, BUFMNG_INST_CONFIG_S* pstConfig);
HI_S32 BUFMNG_SetUserAddr(HI_HANDLE hBuf, HI_U32 u32Addr);
HI_S32 BUFMNG_Destroy(HI_HANDLE hBuf);
HI_S32 BUFMNG_Get(HI_HANDLE hBuf, BUFMNG_INST_CONFIG_S* pstConfig);
HI_S32 BUFMNG_GetWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pBuf);
/* Can be put by pu8KnlVirAddr or pu8UsrVirAddr */
HI_S32 BUFMNG_PutWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pBuf);
HI_S32 BUFMNG_AcqReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pBuf);
/* Can be put by pu8KnlVirAddr or pu8UsrVirAddr */
HI_S32 BUFMNG_RlsReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pBuf);
HI_S32 BUFMNG_Reset(HI_HANDLE hBuf);
HI_S32 BUFMNG_GetStatus(HI_HANDLE hBuf, BUFMNG_STATUS_S* pstStatus);
HI_VOID BUFMNG_SaveInit(HI_VOID);
HI_BOOL BUFMNG_CheckFile(HI_S32 Handle, HI_S8 Flag);
HI_S32 BUFMNG_OpenFile(HI_S32 Handle, HI_S8 *FilePath, HI_S8 Flag);
HI_S32 BUFMNG_CloseFile(HI_S32 Handle, HI_S8 Flag);
HI_S32 BUFMNG_SaveRaw(HI_S32 Handle, HI_S8 *Addr, HI_S32 Length);
HI_S32 BUFMNG_SaveYuv(HI_S32 Handle, HI_DRV_VIDEO_FRAME_S *pstFrame,HI_UNF_VCODEC_TYPE_E enType);

#if (BUFMNG_DEBUG==1)
HI_S32 BUFMNG_Debug(HI_HANDLE hBuf);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_VDEC_BUFFER_MNG_H__ */

