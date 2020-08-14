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
#ifdef __cplusplus
#if __cplusplus
extern "C" {
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

// move from drv_vdec_buf_mng.c
// change by l00228308
#define HI_FATAL_BUFMNG(fmt...)     HI_FATAL_PRINT(HI_ID_VDEC, fmt)
#define HI_ERR_BUFMNG(fmt...)       HI_ERR_PRINT(HI_ID_VDEC, fmt)
#define HI_WARN_BUFMNG(fmt...)      HI_WARN_PRINT(HI_ID_VDEC, fmt)
#define HI_INFO_BUFMNG(fmt...)      HI_INFO_PRINT(HI_ID_VDEC, fmt)

#define BUFMNG_LOCK(sema) \
    do \
    { \
        if (down_interruptible(&sema)) \
        { \
            HI_ERR_BUFMNG("Lock fail"); \
        } \
    } while (0)
#define BUFMNG_UNLOCK(sema) do {up(&sema);} while (0)

#define BUFMNG_SPIN_LOCK(spinlock) \
    do \
    { \
        spin_lock_irqsave(&spinlock, ulFlags);\
    } while (0)
#define BUFMNG_SPIN_UNLOCK(spinlock) \
    do \
    { \
        spin_unlock_irqrestore(&spinlock, ulFlags);\
    } while (0)
// change end

/*************************** Structure Definition ****************************/

/*define VDEC public buffer structure for MMZ/SMMU*/
typedef struct
{
    HI_U8* pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
} VDEC_BUFFER_S;

typedef struct
{
    HI_U32 BufFd;
    HI_U8* pu8StartVirAddr;
    HI_U8* pu8DmaBuf;
    HI_U32 u32Offset;
} VDEC_ION_BUF_S;


typedef enum tagBUFMNG_ALLOC_TYPE_E
{
    BUFMNG_ALLOC_INNER = 0, /* Allocate by BUGMNG */
    BUFMNG_ALLOC_OUTER,     /* Allocate by user */
    BUFMNG_ALLOC_BUTT
} BUFMNG_ALLOC_TYPE_E;

typedef struct tagBUFMNG_INST_CONFIG_S
{
    BUFMNG_ALLOC_TYPE_E enAllocType;    /* MMZ allocate type */
    HI_U32 u32PhyAddr;      /* Start physical address. */
    HI_U8* pu8UsrVirAddr;   /* Start user virtual address. */
    HI_U8* pu8KnlVirAddr;   /* Start kernel virtual address. */
    HI_U32 u32Size;         /* Size */
    HI_CHAR aszName[16];    /* Buffer name */
    HI_BOOL bTvp;
} BUFMNG_INST_CONFIG_S;

typedef struct tagBUFMNG_BUF_S
{
    HI_U32 u32PhyAddr;      /* Physical address */
    HI_U8* pu8UsrVirAddr;   /* User virtual address */
    HI_U8* pu8KnlVirAddr;   /* Kernel virtual address */
    HI_U32 u32Size;         /* Buffer size, in the unit of byte.*/
    HI_U64 u64Pts;          /* PTS of the data filled in a buffer.*/
    HI_U32 u32Index;        /* Index, always output, don't set */
    HI_U32 u32Marker;       /* bit0: 0:Frame over/1:Half frame; bit1: 1:Stream over */
} BUFMNG_BUF_S;

typedef struct tagBUF_VPSS_S
{
    VDEC_BUFFER_S VDECBuf_frm;
    VDEC_BUFFER_S VDECBuf_meta;
} BUF_VPSS_S;

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
} BUFMNG_STATUS_S;

typedef struct BUFMNG_VPSS_LOCK_S
{
    spinlock_t     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} BUFMNG_VPSS_IRQ_LOCK_S;

#ifdef VFMW_VPSS_BYPASS_EN
//add by l00225186
/* vdec remain frame List */
#define VDEC_MAX_REMAIN_FRM_NUM (30)

typedef struct tagVDEC_SPECIAL_INFO_S
{
    VDEC_BUFFER_S        frmBufRec;
    BUFFER_NODE_STATUS   enbufferNodeStatus;
#ifdef HI_TEE_SUPPORT
    HI_BOOL              bIsSecFrm;
#endif
    VDEC_BUFFER_S        hdrBufRec;
    BUFFER_NODE_STATUS   enhdrNodeStatus;
    HI_BOOL              bCanRls;
} VDEC_SPECIAL_FRM_INFO_S;

typedef struct tagVDEC_LIST_S
{
    HI_BOOL          bInit;
    HI_S32                    s32Num;
    VDEC_SPECIAL_FRM_INFO_S   stSpecialFrmRec[VDEC_MAX_REMAIN_FRM_NUM];
    BUFMNG_VPSS_IRQ_LOCK_S    stIrq;
} VDEC_List_S;
#endif

/******************************* API Declaration *****************************/

HI_VOID BUFMNG_Init(HI_VOID);
HI_S32 BUFMNG_DeInit(HI_VOID);
HI_S32 BUFMNG_Create(HI_HANDLE* phBuf, BUFMNG_INST_CONFIG_S* pstConfig);
HI_S32 BUFMNG_SetUserAddr(HI_HANDLE hBuf, HI_U64 u64Addr);
HI_S32 BUFMNG_Destroy(HI_HANDLE hBuf);
HI_S32 BUFMNG_Get(HI_HANDLE hBuf, BUFMNG_INST_CONFIG_S* pstConfig);
HI_S32 BUFMNG_GetWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S* pBuf);
/* Can be put by pu8KnlVirAddr or pu8UsrVirAddr */
HI_S32 BUFMNG_PutWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S* pBuf);
HI_S32 BUFMNG_AcqReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S* pBuf);
/* Can be put by pu8KnlVirAddr or pu8UsrVirAddr */
HI_S32 BUFMNG_RlsReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S* pBuf);
HI_S32 BUFMNG_Reset(HI_HANDLE hBuf);
HI_S32 BUFMNG_GetStatus(HI_HANDLE hBuf, BUFMNG_STATUS_S* pstStatus);
HI_VOID BUFMNG_SaveInit(HI_VOID);
HI_BOOL BUFMNG_CheckFile(HI_S32 Handle, HI_S8 Flag);
HI_S32 BUFMNG_OpenFile(HI_S32 Handle, HI_S8* FilePath, HI_S8 Flag);
HI_S32 BUFMNG_CloseFile(HI_S32 Handle, HI_S8 Flag);
HI_S32 BUFMNG_SaveRaw(HI_S32 Handle, HI_S8* Addr, HI_S32 Length);
HI_S32 BUFMNG_SaveRPU(HI_S32 Handle, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 BUFMNG_SaveYuv(HI_S32 Handle, HI_DRV_VIDEO_FRAME_S* pstFrame, HI_UNF_VCODEC_TYPE_E enType);

HI_S32 HI_DRV_VDEC_AllocAndMap(const char* bufname, char* zone_name, HI_U32 size, int align, VDEC_BUFFER_S* psMBuf);
HI_VOID HI_DRV_VDEC_UnmapAndRelease(VDEC_BUFFER_S* psMBuf);
HI_S32 HI_DRV_VDEC_AllocMem(const char* bufname, char* zone_name, HI_U32 size, int align, VDEC_BUFFER_S* psMBuf,
                            HI_BOOL bTvp, HI_BOOL bMap);
HI_VOID HI_DRV_VDEC_ReleaseMem(VDEC_BUFFER_S* psMBuf, HI_BOOL bTvp, HI_BOOL bMap);
#ifdef HI_TEE_SUPPORT
HI_S32 HI_DRV_VDEC_Alloc_TVP(const char* bufname, char* zone_name, HI_U32 size, int align, VDEC_BUFFER_S* psMBuf);
HI_VOID HI_DRV_VDEC_Release_TVP(VDEC_BUFFER_S* psMBuf);
#endif
HI_S32 HI_DRV_VDEC_Alloc(const char* bufname, char* zone_name, HI_U32 size, int align, VDEC_BUFFER_S* psMBuf);
HI_S32 HI_DRV_VDEC_MapCache(VDEC_BUFFER_S* psMBuf);
HI_S32 HI_DRV_VDEC_Flush(VDEC_BUFFER_S* psMBuf);
HI_S32 HI_DRV_VDEC_Map(VDEC_BUFFER_S* psMBuf);
HI_VOID HI_DRV_VDEC_Unmap(VDEC_BUFFER_S* psMBuf);
HI_VOID HI_DRV_VDEC_Release(VDEC_BUFFER_S* psMBuf);


#if (BUFMNG_DEBUG==1)
HI_S32 BUFMNG_Debug(HI_HANDLE hBuf);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_VDEC_BUFFER_MNG_H__ */

