/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_vdec_buf_mng.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/08/09
  Description   : Definitions of buffer manager.
  History       :
  1.Date        : 2012/08/09
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <linux/list.h>

/* Common headers */
#include "hi_kernel_adapt.h"
#include "hi_common.h"
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "drv_vdec_ext.h"
#include "hi_drv_sys.h"
/* Local headers */
#include "drv_vdec_buf_mng.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

extern HI_BOOL bSaveoneyuv;
extern HI_U32  u32SaveCnt;

/****************************** Macro Definition *****************************/

#define BUFMNG_INVALID_64BITS_PTS   ((HI_U64)(-1))
#define BUFMNG_INVALID_PTS          ((HI_U32)(-1))

#define HI_VMALLOC_BUFMNG(size)     HI_VMALLOC(HI_ID_VDEC, size)
#define HI_VFREE_BUFMNG(addr)       HI_VFREE(HI_ID_VDEC, addr)
#define HI_KMALLOC_ATOMIC_BUFMNG(size)     HI_KMALLOC(HI_ID_VDEC, size, GFP_ATOMIC)
#define HI_KFREE_BUFMNG(addr)       HI_KFREE(HI_ID_VDEC, addr)


#define IS_HEAD_OF_LIST(pBlock, pInst) (&pBlock->stBlockNode == pInst->stBlockHead.next)
#define IS_READING(pBlock) (BUFMNG_BLOCK_READING == pBlock->enstatus)
#define IS_WRITING(pBlock) (BUFMNG_BLOCK_WRITING == pBlock->enstatus)
#define IS_FREE(pBlock) (BUFMNG_BLOCK_FREE == pBlock->enstatus)

#define IS_FULL(pInst, size) (pInst->u32Free < size)

#define BUFMNG_ASSERT_RETURN(Condition, RetValue)                  \
do                                                                 \
{                                                                  \
    if (!Condition)                                                \
    {                                                              \
        HI_ERR_BUFMNG("[%s %d]assert warning\n",__func__,__LINE__);\
        return RetValue;                                           \
    }                                                              \
}while(0)


#define BUFMNG_FIND_INST(hInst, pInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        BUFMNG_INST_S* pstTmp; \
        pInst = HI_NULL; \
        BUFMNG_LOCK(s_stBMParam.stSem);\
        list_for_each_safe(pos, n, &s_stBMParam.stInstHead) \
        { \
            pstTmp = list_entry(pos, BUFMNG_INST_S, stInstNode); \
            if (hInst == pstTmp->hBuf) \
            { \
                pInst = pstTmp; \
                break; \
            } \
        } \
        BUFMNG_UNLOCK(s_stBMParam.stSem);\
    }

#define BUFMNG_FIND_BLOCK_BY_ADDR(addr, pInst, pBlock) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        BUFMNG_BLOCK_S* pstTmp; \
        list_for_each_safe(pos, n, &pInst->stBlockHead) \
        { \
            pstTmp = list_entry(pos, BUFMNG_BLOCK_S, stBlockNode); \
            if (addr == pstTmp->u32Addr) \
            { \
                pBlock = pstTmp; \
                break; \
            } \
        } \
    }

#define BUFMNG_FREE_BLOCK_LIST(pInst) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        BUFMNG_BLOCK_S* pstBlock; \
        list_for_each_safe(pos, n, &pInst->stBlockHead) \
        { \
            pstBlock = list_entry(pos, BUFMNG_BLOCK_S, stBlockNode); \
            list_del(pos); \
            HI_KFREE_BUFMNG(pstBlock); \
        } \
    }

#define BUFMNG_FIND_TAIL_BLOCK(pInst, pBlock) \
    { \
        pBlock = list_entry(pInst->stBlockHead.prev, BUFMNG_BLOCK_S, stBlockNode); \
    }

#define BUFMNG_FIND_HEAD_BLOCK(pInst, pBlock) \
    { \
        pBlock = list_entry(pInst->stBlockHead.next, BUFMNG_BLOCK_S, stBlockNode); \
    }

#define BUFMNG_FIND_NEXT_BLOCK(pBlock, pNextBlock) \
    { \
        pNextBlock = list_entry(pBlock->stBlockNode.next, BUFMNG_BLOCK_S, stBlockNode); \
    }

#define BUFMNG_FIND_FIRST_FREE_BLOCK(pInst, pBlock) \
    { \
        BUFMNG_BLOCK_S* pstTmp = HI_NULL; \
        struct list_head* pstList = pInst->stBlockHead.next; \
        while (pstList != &(pInst->stBlockHead)) \
        { \
            pstTmp = list_entry(pstList, BUFMNG_BLOCK_S, stBlockNode); \
            if (IS_FREE(pstTmp)) \
            { \
                pBlock = pstTmp; \
                break; \
            } \
            pstList = pstList->next; \
        } \
    }

#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)
extern VDEC_BUFFER_S g_stESBUFMMZ;
extern HI_BOOL g_bVdecPreESBUFMMZUsed;
#endif


/************************ Static Structure Definition ************************/

typedef enum tagBUFMNG_BLOCK_STATUS_E
{
    BUFMNG_BLOCK_FREE = 0,
    BUFMNG_BLOCK_READING,
    BUFMNG_BLOCK_WRITING,
    BUFMNG_BLOCK_BUTT
} BUFMNG_BLOCK_STATUS_E;

/* Describe a buffer block */
typedef struct tagBUFMNG_BLOCK_S
{
    HI_U32 u32Addr;                 /* Only save one address in block, use physical address */
    HI_U32 u32Size;                 /* Size of block */
#if (BUFMNG_64BITS_PTS_SUPPORT == 1)
    HI_U64 u64Pts;                  /* PTS of the data filled in a buffer.*/
#else
    HI_U32 u32Pts;                  /* PTS of the data filled in a buffer.*/
#endif
#if (BUFMNG_INDEX_SUPPORT == 1)
    HI_U32 u32Index;                /* Index */
#endif
#if (BUFMNG_MARKER_SUPPORT == 1)
    HI_U32 u32Marker;               /* bit0: 0:End of Frame/1:Half frame
                                     * bit1: 0:Normal/1:End of stream
                                     * bit2: 0:Continuous stream/1:Discontinuous
                                     */
#endif
    BUFMNG_BLOCK_STATUS_E enstatus; /* Status of block */
    struct list_head stBlockNode;   /* Block list node */
} BUFMNG_BLOCK_S;

/* Describe a buffer instance */
typedef struct tagBUFMNG_INST_S
{
    HI_HANDLE   hBuf;               /* Handle of this buffer instance */
    HI_U32      u32PhyAddr;           /* Start physical address of the buffer instance. */
    HI_U8      *pu8UsrVirAddr;      /* Start user virtual address of the buffer instance. */
    HI_U8      *pu8KnlVirAddr;      /* Start kerenl virtual address of the buffer instance. */
    HI_U32      u32Size;            /* Size of the buffer instance */
    HI_U32      u32Used;            /* Used size */
    HI_U32      u32Free;            /* Free size */
    HI_U32      u32Freeze;          /* Freeze size */
    HI_U32      u32DataNum;         /* For stream mode, it is undecoded packet number.
                                       For frame mode, it is undecoded frame number, support BUFMNG_NOT_END_FRAME_BIT.*/
    HI_U32      u32GetTry;          /* GetWriteBuf try times */
    HI_U32      u32GetOK;           /* GetWriteBuf ok times */
    HI_U32      u32PutTry;          /* PutWriteBuf try times */
    HI_U32      u32PutOK;           /* PutWriteBuf ok times */
    HI_U32      u32RecvTry;         /* GetReadBuf try times */
    HI_U32      u32RecvOK;          /* GetReadBuf ok times */
    HI_U32      u32RlsTry;          /* PutReadBuf try times */
    HI_U32      u32RlsOK;           /* PutReadBuf ok times */
#if (BUFMNG_INDEX_SUPPORT == 1)
    HI_U32      u32Index;           /* Index */
#endif
    BUFMNG_ALLOC_TYPE_E enAllocType;/* MMZ alloc type */
#ifdef HI_TEE_SUPPORT
    HI_BOOL     bTvp;
#endif
    HI_BOOL bMMZMap;                /* Need unmap when destroy */
    spinlock_t stSpinLock;          /* Spin lock */
    struct list_head stBlockHead;   /* Buffer manager block list head */
    struct list_head stInstNode;    /* Instance node */
} BUFMNG_INST_S;

/* Global parameter of this module */
typedef struct tagBUFMNG_GLOBAL_S
{
    HI_U16 u16InstNum;              /* Instance count */
    HI_U16 u16InstHandle;           /* Allocate handle according to this number */
    struct semaphore stSem;         /* Semaphore */
    struct list_head stInstHead;    /* Instance list head */
} BUFMNG_GLOBAL_S;

/***************************** Global Definition *****************************/

struct file *VdecSaveRawFile = HI_NULL;
HI_S32 VdecRawChanNum = -1;
struct semaphore stRawSem;

struct file *g_VdecSaveRPUFile = HI_NULL;
HI_S32 g_VdecRPUChanNum = -1;
struct semaphore stRPUSem;
struct file *VdecSaveYuvFile = HI_NULL;
HI_S32 VdecYuvChanNum = -1;
HI_U8 *U_Array = NULL;
HI_U8 *V_Array = NULL;
HI_U8 *YUV_Array = NULL;
struct semaphore stYuvSem;


/***************************** Static Definition *****************************/

static BUFMNG_GLOBAL_S s_stBMParam =
{
    .u16InstNum = 0,
    .u16InstHandle = 0,
    .stInstHead = LIST_HEAD_INIT(s_stBMParam.stInstHead)
};

/*********************************** Code ************************************/


/*********************************** for SMMU begain************************************/
/*申请内存函数，bSec(1:安全内存，0:非安全内存),bMap(1:做cpu映射，0:不做cpu映射)*/
HI_S32 HI_DRV_VDEC_AllocMem(const char *bufname, char *zone_name, HI_U32 size, int align, VDEC_BUFFER_S *psMBuf,
                            HI_BOOL bTvp, HI_BOOL bMap)
{
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == bTvp)
    {
        return HI_DRV_VDEC_Alloc_TVP(bufname, zone_name, size, align, psMBuf);
    }
    else
#endif
    {
        if (HI_TRUE == bMap)
        {
            return HI_DRV_VDEC_AllocAndMap(bufname, zone_name, size, align, psMBuf);
        }
        else
        {
            return HI_DRV_VDEC_Alloc(bufname, zone_name, size, align, psMBuf);
        }
    }
}

/*释放内存函数，bSec(1:安全内存，0:非安全内存),bMap(1:做cpu映射，0:不做cpu映射)*/
HI_VOID HI_DRV_VDEC_ReleaseMem(VDEC_BUFFER_S *psMBuf, HI_BOOL bTvp, HI_BOOL bMap)
{
#ifdef HI_TEE_SUPPORT
    if (HI_TRUE == bTvp)
    {
        HI_DRV_VDEC_Release_TVP(psMBuf);
    }
    else
#endif
    {
        if (HI_TRUE == bMap)
        {
            HI_DRV_VDEC_UnmapAndRelease(psMBuf);
        }
        else
        {
            HI_DRV_VDEC_Release(psMBuf);
        }
    }
}

HI_S32 HI_DRV_VDEC_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(MMZ_BUFFER_S));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(SMMU_BUFFER_S));
#endif

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, size, align, &stMMZBuf);

    if (s32Ret == HI_SUCCESS)
    {
        psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
        psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
        psMBuf->u32Size          = stMMZBuf.u32Size;
    }
#else
    s32Ret = HI_DRV_SMMU_AllocAndMap(bufname, size, align, &stSMMUBuf);

    if (s32Ret == HI_SUCCESS)
    {
        psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
        psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
        psMBuf->u32Size          = stSMMUBuf.u32Size;
    }
#endif

    return s32Ret;
}

HI_VOID HI_DRV_VDEC_UnmapAndRelease(VDEC_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

    if (HI_NULL == psMBuf)
    {
        HI_ERR_VDEC("invalid param!\n");
        return;
    }

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    HI_DRV_MMZ_UnmapAndRelease(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    HI_DRV_SMMU_UnmapAndRelease(&stSMMUBuf);
#endif
    return;
}

#ifdef HI_TEE_SUPPORT
HI_S32 HI_DRV_VDEC_Alloc_TVP(const char *bufname, char *zone_name, HI_U32 size, int align, VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_FAILURE;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif
    if(HI_NULL == psMBuf)
    {
        HI_ERR_VDEC("invalid param!\n");
        return s32Ret;
    }

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_SECMMZ_Alloc(bufname, zone_name, size, align, &stMMZBuf);
    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
    psMBuf->u32Size          = stMMZBuf.u32Size;
#else
    s32Ret = HI_DRV_SECSMMU_Alloc(bufname, size, align, &stSMMUBuf);
    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
    psMBuf->u32Size          = stSMMUBuf.u32Size;
#endif

    return s32Ret;

}
HI_VOID HI_DRV_VDEC_Release_TVP(VDEC_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif
    if(HI_NULL == psMBuf)
    {
        HI_ERR_VDEC("invalid param!\n");
        return;
    }

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    HI_DRV_SECMMZ_Release(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    HI_DRV_SECSMMU_Release(&stSMMUBuf);
#endif
}
#endif

HI_S32 HI_DRV_VDEC_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(MMZ_BUFFER_S));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(SMMU_BUFFER_S));
#endif

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_MMZ_Alloc(bufname, zone_name, size, align, &stMMZBuf);

    if (s32Ret == HI_SUCCESS)
    {
        psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
        psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
        psMBuf->u32Size          = stMMZBuf.u32Size;
    }
#else
    s32Ret = HI_DRV_SMMU_Alloc(bufname, size, align, &stSMMUBuf);

    if (s32Ret == HI_SUCCESS)
    {
        psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
        psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
        psMBuf->u32Size          = stSMMUBuf.u32Size;
    }
#endif

    return s32Ret;
}


HI_S32 HI_DRV_VDEC_MapCache(VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    s32Ret = HI_DRV_MMZ_MapCache(&stMMZBuf);
    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
#else
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    s32Ret = HI_DRV_SMMU_MapCache(&stSMMUBuf);
    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
#endif

    return s32Ret;
}


HI_S32 HI_DRV_VDEC_Flush(VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    s32Ret = HI_DRV_MMZ_Flush(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    s32Ret = HI_DRV_SMMU_Flush(&stSMMUBuf);
#endif

    return s32Ret;
}

HI_S32 HI_DRV_VDEC_Map(VDEC_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    s32Ret = HI_DRV_MMZ_Map(&stMMZBuf);
    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
#else
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    s32Ret = HI_DRV_SMMU_Map(&stSMMUBuf);
    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
#endif

    return s32Ret;
}

HI_VOID HI_DRV_VDEC_Unmap(VDEC_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    HI_DRV_MMZ_Unmap(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    HI_DRV_SMMU_Unmap(&stSMMUBuf);
#endif
}

HI_VOID HI_DRV_VDEC_Release(VDEC_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
#else
    SMMU_BUFFER_S stSMMUBuf;
#endif

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
    HI_DRV_MMZ_Release(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
    HI_DRV_SMMU_Release(&stSMMUBuf);
#endif
}



/*********************************** for SMMU end ************************************/




HI_VOID BUFMNG_Init(HI_VOID)
{
    /* Init global parameter */
    s_stBMParam.u16InstNum = 0;
    s_stBMParam.u16InstHandle = 0;

    /* Init global mutex */
    HI_INIT_MUTEX(&s_stBMParam.stSem);

    INIT_LIST_HEAD(&s_stBMParam.stInstHead);

    return;
}

HI_S32 BUFMNG_DeInit(HI_VOID)
{
    struct list_head *pos;
    struct list_head *n;
    struct list_head *head;
    BUFMNG_INST_S *pstInst;

    BUFMNG_LOCK(s_stBMParam.stSem);
    head = &s_stBMParam.stInstHead;

    if (!list_empty(head))
    {
        list_for_each_safe(pos, n, head)
        {
            pstInst = list_entry(pos, BUFMNG_INST_S, stInstNode);
            BUFMNG_UNLOCK(s_stBMParam.stSem);
            BUFMNG_Destroy(pstInst->hBuf);
            BUFMNG_LOCK(s_stBMParam.stSem);
        }
    }

    s_stBMParam.u16InstNum = 0;
    s_stBMParam.u16InstHandle = 0;
    BUFMNG_UNLOCK(s_stBMParam.stSem);

    //BUFMNG_CloseFile(-1, 2);

    return HI_SUCCESS;
}

static HI_S32 BUFMNG_CreateCheckPara(HI_HANDLE *phBuf, BUFMNG_INST_CONFIG_S *pstConfig)
{
    if ((HI_NULL == phBuf) || (HI_NULL == pstConfig))
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    if ((BUFMNG_ALLOC_BUTT <= pstConfig->enAllocType) || (0 == pstConfig->u32Size))
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    if ((BUFMNG_ALLOC_OUTER == pstConfig->enAllocType) && (0 == pstConfig->u32PhyAddr))
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    return HI_SUCCESS;
}

static HI_VOID BUFMNG_DestroyBuffer(HI_BOOL bTvp, VDEC_BUFFER_S* pstVDECAllocBuf)
{
#ifdef HI_TEE_SUPPORT
    if (bTvp)
    {
        HI_DRV_VDEC_Release_TVP(pstVDECAllocBuf);
    }
    else
#endif
    {
        HI_DRV_VDEC_UnmapAndRelease(pstVDECAllocBuf);
    }

    return;
}

static HI_S32 BUFMNG_CreatBuffer(BUFMNG_INST_CONFIG_S *pstConfig,
                                 VDEC_BUFFER_S* pstVDECAllocBuf)
{
    HI_S32 s32Ret = HI_ERR_BM_FREE_ERR;

#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)

    if ((HI_FALSE == g_bVdecPreESBUFMMZUsed) && (pstConfig->u32Size <= g_stESBUFMMZ.u32Size))
    {
        g_bVdecPreESBUFMMZUsed = HI_TRUE;
        pstConfig->u32PhyAddr = g_stESBUFMMZ.u32StartPhyAddr;
        pstConfig->pu8KnlVirAddr = (HI_U8*)g_stESBUFMMZ.pu8StartVirAddr;
    }
    else
#endif
    {
#ifdef HI_TEE_SUPPORT

        if (pstConfig->bTvp == HI_TRUE)
        {
            s32Ret = HI_DRV_VDEC_Alloc_TVP(pstConfig->aszName, HI_NULL, pstConfig->u32Size, 0, pstVDECAllocBuf);

            if (s32Ret != HI_SUCCESS)
            {
                HI_FATAL_BUFMNG("%s: Alloc SEC MMU(%s) failed.\n", __func__, pstConfig->aszName);
                return HI_ERR_BM_NO_MEMORY;
            }

            pstConfig->u32PhyAddr = pstVDECAllocBuf->u32StartPhyAddr;
            pstConfig->pu8KnlVirAddr = (HI_U8*)(HI_SIZE_T)pstVDECAllocBuf->u32StartPhyAddr;
            pstConfig->pu8UsrVirAddr = (HI_U8*)(HI_SIZE_T)pstVDECAllocBuf->u32StartPhyAddr;
        }
        else
#endif
        {
            s32Ret = HI_DRV_VDEC_AllocAndMap(pstConfig->aszName, HI_NULL, pstConfig->u32Size, 0, pstVDECAllocBuf);

            if (s32Ret != HI_SUCCESS)
            {
                HI_FATAL_BUFMNG("Alloc VDEC fail:0x%x.\n", s32Ret);
                return HI_ERR_BM_NO_MEMORY;
            }

            pstConfig->u32PhyAddr = pstVDECAllocBuf->u32StartPhyAddr;
            pstConfig->pu8KnlVirAddr = (HI_U8*)pstVDECAllocBuf->pu8StartVirAddr;
        }
    }

    return s32Ret;
}

HI_S32 BUFMNG_Create(HI_HANDLE *phBuf, BUFMNG_INST_CONFIG_S *pstConfig)
{
    HI_S32 s32Ret;
    BUFMNG_INST_S *pstInst;
    VDEC_BUFFER_S stVDECAllocBuf = {0};
    VDEC_BUFFER_S stVDECBuf = {0};

    s32Ret = BUFMNG_CreateCheckPara(phBuf, pstConfig);
    if (s32Ret == HI_ERR_BM_INVALID_PARA)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    /* If BUFMNG_ALLOC_INNER, alloc MMZ here */
    if (BUFMNG_ALLOC_INNER == pstConfig->enAllocType)
    {
        s32Ret = BUFMNG_CreatBuffer(pstConfig, &stVDECAllocBuf);
        if (s32Ret == HI_ERR_BM_NO_MEMORY)
        {
            return  HI_ERR_BM_NO_MEMORY;
        }
    }

    /* Allocate an instance */
    //    pstInst = HI_VMALLOC_BUFMNG(sizeof(BUFMNG_INST_S));
    pstInst = HI_KMALLOC_ATOMIC_BUFMNG(sizeof(BUFMNG_INST_S));
    if (HI_NULL == pstInst)
    {
        HI_FATAL_BUFMNG("No memory.\n");

        if (BUFMNG_ALLOC_INNER == pstConfig->enAllocType)
        {
            BUFMNG_DestroyBuffer(pstConfig->bTvp, &stVDECAllocBuf);
        }

        return HI_ERR_BM_NO_MEMORY;
    }

    BUFMNG_LOCK(s_stBMParam.stSem);

    /* Allocate handle */
    *phBuf = s_stBMParam.u16InstHandle;

    /* Init instance parameter */
    pstInst->hBuf = *phBuf;
#ifdef HI_TEE_SUPPORT
    pstInst->bTvp = pstConfig->bTvp;
#endif
    pstInst->u32PhyAddr = pstConfig->u32PhyAddr;
    pstInst->pu8UsrVirAddr = pstConfig->pu8UsrVirAddr;
    if (HI_NULL == pstConfig->pu8KnlVirAddr)
    {
        /* Map to kernel virtual address */
        stVDECBuf.u32Size = pstConfig->u32Size;
        stVDECBuf.u32StartPhyAddr = pstConfig->u32PhyAddr;
        if (HI_SUCCESS != HI_DRV_VDEC_Map(&stVDECBuf))
        {
            BUFMNG_UNLOCK(s_stBMParam.stSem);
            HI_ERR_BUFMNG("HI_DRV_VDEC_Map fail!\n");
            if (BUFMNG_ALLOC_INNER == pstConfig->enAllocType)
            {
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)

                if (g_bVdecPreESBUFMMZUsed)
                {
                    g_bVdecPreESBUFMMZUsed = HI_FALSE;
                }
                else
#endif
                {
                    BUFMNG_DestroyBuffer(pstConfig->bTvp, &stVDECAllocBuf);
                }
            }
            HI_KFREE_BUFMNG(pstInst);
            return HI_FAILURE;
        }

        pstInst->pu8KnlVirAddr = pstConfig->pu8KnlVirAddr = (HI_U8 *)stVDECBuf.pu8StartVirAddr;
        HI_INFO_BUFMNG("stMMZBuf.pu8StartVirAddr:%p\n", stVDECBuf.pu8StartVirAddr);
        pstInst->bMMZMap = HI_TRUE;
    }
    else
    {
        pstInst->pu8KnlVirAddr = pstConfig->pu8KnlVirAddr;
        pstInst->bMMZMap = HI_FALSE;
    }

    pstInst->u32Size = pstConfig->u32Size;
    pstInst->enAllocType = pstConfig->enAllocType;
#if (BUFMNG_INDEX_SUPPORT == 1)
    pstInst->u32Index = 0;
#endif
    HI_INFO_BUFMNG("[BUFMNG_Create] PHY:%x, U-VIR:%llx, K-VIR:%llx, SIZE:%d\n",
                   pstInst->u32PhyAddr, pstInst->pu8UsrVirAddr, pstInst->pu8KnlVirAddr, pstInst->u32Size);
    spin_lock_init(&pstInst->stSpinLock);

    /* Init block list parameter */
    pstInst->u32Free   = pstInst->u32Size;
    pstInst->u32Freeze = 0;
    pstInst->u32Used    = 0;
    pstInst->u32DataNum = 0;
    pstInst->u32GetTry  = 0;
    pstInst->u32GetOK   = 0;
    pstInst->u32PutTry  = 0;
    pstInst->u32PutOK   = 0;
    pstInst->u32RecvTry = 0;
    pstInst->u32RecvOK  = 0;
    pstInst->u32RlsTry  = 0;
    pstInst->u32RlsOK   = 0;
    INIT_LIST_HEAD(&pstInst->stBlockHead);

    /* Add this instance to instance list */
    list_add_tail(&pstInst->stInstNode, &s_stBMParam.stInstHead);
    s_stBMParam.u16InstHandle++;
    s_stBMParam.u16InstNum++;
    BUFMNG_UNLOCK(s_stBMParam.stSem);
    return HI_SUCCESS;
}

HI_S32 BUFMNG_SetUserAddr(HI_HANDLE hBuf, HI_U64 u64Addr)
{
    BUFMNG_INST_S *pstInst = HI_NULL;
    //HI_SIZE_T ulFlags;
    /* Find instance by handle */
    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }


    //pstInst->pu8UsrVirAddr = (HI_U8*)u64Addr;
    pstInst->pu8UsrVirAddr = (HI_U8 *)(HI_SIZE_T)u64Addr;

    return HI_SUCCESS;
}

HI_S32 BUFMNG_Get(HI_HANDLE hBuf, BUFMNG_INST_CONFIG_S *pstConfig)
{
    BUFMNG_INST_S *pstInst = HI_NULL;
    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    pstConfig->u32PhyAddr = pstInst->u32PhyAddr;
    pstConfig->u32Size = pstInst->u32Size;
    return HI_SUCCESS;
}

HI_S32 BUFMNG_Destroy(HI_HANDLE hBuf)
{
    BUFMNG_INST_S *pstInst = HI_NULL;
    VDEC_BUFFER_S stVDECBuf;
    HI_SIZE_T ulFlags;

    /* Find instance by handle */
    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* If has blocks, free them */
    if (!list_empty(&pstInst->stBlockHead))
    {
        BUFMNG_FREE_BLOCK_LIST(pstInst);
    }

#if (BUFMNG_DEBUG == 1)
    HI_ERR_BUFMNG("Before destroy, used %d, free %d, freeze %d\n",
                  pstInst->u32Used, pstInst->u32Free, pstInst->u32Freeze);
#endif
    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);

    /* If need, Unmap */
    if (pstInst->bMMZMap)
    {
        // TODO: REMOVE BUFMNG_LOCK
        BUFMNG_SPIN_LOCK(pstInst->stSpinLock);
        stVDECBuf.u32StartPhyAddr = pstInst->u32PhyAddr;
        stVDECBuf.pu8StartVirAddr = pstInst->pu8KnlVirAddr;
        stVDECBuf.u32Size = pstInst->u32Size;
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
#ifdef HI_TEE_SUPPORT

        if (HI_TRUE == pstInst->bTvp)
        {
        }
        else
#endif
        {
            HI_DRV_VDEC_Unmap(&stVDECBuf);
        }
    }

    /* If need, free */
    if (BUFMNG_ALLOC_INNER == pstInst->enAllocType)
    {
#if (1 == PRE_ALLOC_VDEC_ESBUF_MMZ)

        if (g_bVdecPreESBUFMMZUsed)
        {
            g_bVdecPreESBUFMMZUsed = HI_FALSE;
        }
        else
#endif
        {
            BUFMNG_SPIN_LOCK(pstInst->stSpinLock);
            stVDECBuf.u32StartPhyAddr = pstInst->u32PhyAddr;
            stVDECBuf.pu8StartVirAddr = pstInst->pu8KnlVirAddr;
            stVDECBuf.u32Size = pstInst->u32Size;
            BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
#ifdef HI_TEE_SUPPORT

            if (HI_TRUE == pstInst->bTvp)
            {
                HI_DRV_VDEC_Release_TVP(&stVDECBuf);
            }
            else
#endif
            {
                HI_DRV_VDEC_UnmapAndRelease(&stVDECBuf);
            }
        }
    }

    BUFMNG_LOCK(s_stBMParam.stSem);

    /* Remove instance from list */
    list_del(&pstInst->stInstNode);

    if (s_stBMParam.u16InstNum > 0)
    {
        s_stBMParam.u16InstNum--;
    }

    BUFMNG_UNLOCK(s_stBMParam.stSem);

    /* Free resource */
    HI_KFREE_BUFMNG(pstInst);

    return HI_SUCCESS;
}

HI_S32 BUFMNG_GetWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pstBuf)
{
    BUFMNG_INST_S *pstInst   = HI_NULL;
    BUFMNG_BLOCK_S *pstBlock = HI_NULL;
    BUFMNG_BLOCK_S *pstBlockRead = HI_NULL;
    HI_U32 u32ReadOffset;
    HI_U32 u32WriteOffset;
    HI_U32 u32TailFree;
    HI_U32 u32HeadFree;
    HI_BOOL bAlloc = HI_FALSE;
    HI_SIZE_T ulFlags;

    BUFMNG_ASSERT_RETURN(pstBuf != HI_NULL, HI_ERR_BM_INVALID_PARA);

    BUFMNG_FIND_INST(hBuf, pstInst);
    BUFMNG_ASSERT_RETURN(pstInst != HI_NULL, HI_ERR_BM_INVALID_PARA);

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* Get buffer try times increase */
    pstInst->u32GetTry++;

    /* Buffer full */
    if (IS_FULL(pstInst, pstBuf->u32Size))
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_BUFFER_FULL;
    }

    /* If empty, write from start */
    if (list_empty(&pstInst->stBlockHead))
    {
        u32WriteOffset = u32ReadOffset = 0;
    }
    /* Else, find write offset and read offset */
    else
    {
        /* Only support one writer */
        BUFMNG_FIND_TAIL_BLOCK(pstInst, pstBlock);

        if (IS_WRITING(pstBlock))
        {
            pstBuf->u32PhyAddr = pstBlock->u32Addr;
            pstBuf->u32Size = pstBlock->u32Size;
            pstBuf->pu8KnlVirAddr = (HI_NULL == pstInst->pu8KnlVirAddr) ? HI_NULL :
                                    pstBlock->u32Addr - pstInst->u32PhyAddr + pstInst->pu8KnlVirAddr;
            pstBuf->pu8UsrVirAddr = (HI_NULL == pstInst->pu8UsrVirAddr) ? HI_NULL :
                                    pstBlock->u32Addr - pstInst->u32PhyAddr + pstInst->pu8UsrVirAddr;
            BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
            HI_ERR_BUFMNG("BUFMNG BUSY\n");
            return HI_ERR_BM_BUSY;
        }

        u32WriteOffset = pstBlock->u32Addr + pstBlock->u32Size - pstInst->u32PhyAddr;
#if (BUFMNG_DEBUG == 1)

        if (u32WriteOffset > pstInst->u32Size)
        {
            HI_FATAL_BUFMNG("!!!!!!!!!!! [ u32WriteOffset ERROR ] !!!!!!!!!!!!\n");
            HI_FATAL_BUFMNG("u32WriteOffset = %d\n", u32WriteOffset);
            HI_FATAL_BUFMNG("pstBlock->u32Addr = %p\n", (HI_VOID *)pstBlock->u32Addr);
            HI_FATAL_BUFMNG("pstBlock->u32Size = %d\n", pstBlock->u32Size);
            HI_FATAL_BUFMNG("pstInst->u32PhyAddr = %p\n", (HI_VOID *)pstInst->u32PhyAddr);
        }

#endif

        BUFMNG_FIND_HEAD_BLOCK(pstInst, pstBlockRead);
        u32ReadOffset = pstBlockRead->u32Addr - pstInst->u32PhyAddr;
#if (BUFMNG_DEBUG == 1)

        if (u32ReadOffset > pstInst->u32Size)
        {
            HI_FATAL_BUFMNG("!!!!!!!!!!!! [ u32ReadOffset ERROR ] !!!!!!!!!!!!!!\n");
            HI_FATAL_BUFMNG("u32ReadOffset = %d\n", u32ReadOffset);
            HI_FATAL_BUFMNG("pstBlockRead->u32Addr = %p\n", (HI_VOID *)pstBlockRead->u32Addr);
            HI_FATAL_BUFMNG("pstBlockRead->u32Size = %d\n", pstBlockRead->u32Size);
            HI_FATAL_BUFMNG("pstInst->u32PhyAddr = %p\n", (HI_VOID *)pstInst->u32PhyAddr);
        }

#endif

    }

    /* Reverse: write pointer before read pointer, the free area is continuous */
    if (u32WriteOffset <= u32ReadOffset)
    {
        bAlloc = HI_TRUE;

#if (BUFMNG_GET_SMSP == 1)
        /* For return size */
        u32TailFree = pstInst->u32Free;
#endif
    }
    /* Normal: write pointer after read pointer, the free area isn't continuous */
    else
    {
        u32TailFree = pstInst->u32Size - u32WriteOffset;
        u32HeadFree = pstInst->u32Free - u32TailFree;

#if (BUFMNG_DEBUG == 1)

        if ((u32TailFree > pstInst->u32Free) || (u32TailFree > pstInst->u32Size))
        {
            HI_FATAL_BUFMNG("!!!!!!!!!!!! [ u32TailFree ERROR ] !!!!!!!!!!!!!!\n");

            if (pstBlock)
            {
                HI_FATAL_BUFMNG("u32WriteOffset = %d\n", u32WriteOffset);
                HI_FATAL_BUFMNG("pstBlock->u32Addr = %p\n", (HI_VOID *)pstBlock->u32Addr);
                HI_FATAL_BUFMNG("pstBlock->u32Size = %d\n", pstBlock->u32Size);
            }

            if (pstBlockRead)
            {
                HI_FATAL_BUFMNG("u32ReadOffset = %d\n", u32ReadOffset);
                HI_FATAL_BUFMNG("pstBlockRead->u32Addr = %p\n", (HI_VOID *)pstBlockRead->u32Addr);
                HI_FATAL_BUFMNG("pstBlockRead->u32Size = %d\n", pstBlockRead->u32Size);
            }

            HI_FATAL_BUFMNG("pstInst->u32PhyAddr = %p\n", (HI_VOID *)pstInst->u32PhyAddr);
            HI_FATAL_BUFMNG("u32WriteOffset = %d\n", u32WriteOffset);
            HI_FATAL_BUFMNG("u32ReadOffset = %d\n", u32ReadOffset);
            HI_FATAL_BUFMNG("pstInst->u32Size = %d\n", pstInst->u32Size);
            HI_FATAL_BUFMNG("pstInst->u32Free = %d\n", pstInst->u32Free);
            HI_FATAL_BUFMNG("u32TailFree = %d\n", u32TailFree);
            HI_FATAL_BUFMNG("u32HeadFree = %d\n", u32HeadFree);
        }

        if ((u32HeadFree > pstInst->u32Size) || (u32HeadFree > pstInst->u32Size))
        {
            HI_FATAL_BUFMNG("!!!!!!!!!!!! [ u32HeadFree ERROR ] !!!!!!!!!!!!!!\n");
            HI_FATAL_BUFMNG("u32WriteOffset = %d\n", u32WriteOffset);
            HI_FATAL_BUFMNG("u32ReadOffset = %d\n", u32ReadOffset);
            HI_FATAL_BUFMNG("pstInst->u32Size = %d\n", pstInst->u32Size);
            HI_FATAL_BUFMNG("pstInst->u32Free = %d\n", pstInst->u32Free);
            HI_FATAL_BUFMNG("u32TailFree = %d\n", u32TailFree);
            HI_FATAL_BUFMNG("u32HeadFree = %d\n", u32HeadFree);
        }

#endif

        if (u32TailFree >= pstBuf->u32Size)
        {
            bAlloc = HI_TRUE;
        }
        else if (u32HeadFree >= pstBuf->u32Size)
        {
            bAlloc = HI_TRUE;

            /* Alloc from head */
            u32WriteOffset = 0;

            /* Freeze the last area */
            pstInst->u32Freeze = u32TailFree;
            pstInst->u32Free -= u32TailFree;

#if (BUFMNG_GET_SMSP == 1)
            /* For return size */
            u32TailFree = pstInst->u32Free;
#endif
        }
    }

    /* Allocate fail, return */
    if (!bAlloc)
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_BUFFER_FULL;
    }

    /* Allocate new block */
    pstBlock = HI_KMALLOC_ATOMIC_BUFMNG(sizeof(BUFMNG_BLOCK_S));
    if (pstBlock == HI_NULL)
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_NO_MEMORY;
    }

    /* Init block parameter */
#if (BUFMNG_GET_SMSP == 1)
    pstBlock->u32Size = pstBuf->u32Size = u32TailFree;
#else
    pstBlock->u32Size = pstBuf->u32Size;
#endif
    pstBuf->u32PhyAddr = pstBlock->u32Addr = pstInst->u32PhyAddr + u32WriteOffset;
    pstBuf->pu8UsrVirAddr = (pstInst->pu8UsrVirAddr != 0) ? (pstInst->pu8UsrVirAddr + u32WriteOffset) : HI_NULL;

    pstBuf->pu8KnlVirAddr = pstInst->pu8KnlVirAddr + u32WriteOffset;
#if (BUFMNG_64BITS_PTS_SUPPORT == 1)
    pstBuf->u64Pts = pstBlock->u64Pts = BUFMNG_INVALID_64BITS_PTS;
#else
    pstBuf->u64Pts = pstBlock->u32Pts = BUFMNG_INVALID_PTS;
#endif
#if (BUFMNG_INDEX_SUPPORT == 1)
    pstBuf->u32Index = pstBlock->u32Index = pstInst->u32Index++;
#endif
#if (BUFMNG_MARKER_SUPPORT == 1)
    pstBuf->u32Marker = pstBlock->u32Marker = 0;
#else
    pstBuf->u32Marker = 0;
#endif
    pstBlock->enstatus = BUFMNG_BLOCK_WRITING;

    /* Add block to list */
    list_add_tail(&pstBlock->stBlockNode, &pstInst->stBlockHead);

    /* Get buffer OK times increase */
    pstInst->u32GetOK++;

    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);

    return HI_SUCCESS;
}

HI_S32 BUFMNG_PutWriteBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pstBuf)
{
    HI_U32 u32PhyAddr;
    BUFMNG_INST_S *pstInst   = HI_NULL;
    BUFMNG_BLOCK_S *pstBlock = HI_NULL;
    HI_SIZE_T ulFlags;

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* Put buffer try times increase */
    pstInst->u32PutTry++;

    /* Find the tail block */
    BUFMNG_FIND_TAIL_BLOCK(pstInst, pstBlock);

    if (pstBuf->u32Size > pstBlock->u32Size)
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        HI_ERR_BUFMNG("Put size err:%d>%d\n", pstBuf->u32Size, pstBlock->u32Size);
        return HI_ERR_BM_WRITE_FREE_ERR;
    }

    if (0 != pstBuf->pu8UsrVirAddr)
    {
        u32PhyAddr = pstBuf->pu8UsrVirAddr - pstInst->pu8UsrVirAddr + pstInst->u32PhyAddr;
        pstBuf->pu8KnlVirAddr = pstBuf->pu8UsrVirAddr - pstInst->pu8UsrVirAddr + pstInst->pu8KnlVirAddr;
    }
    else
    {
        u32PhyAddr = pstBuf->pu8KnlVirAddr - pstInst->pu8KnlVirAddr + pstInst->u32PhyAddr;
        pstBuf->pu8UsrVirAddr = pstBuf->pu8KnlVirAddr - pstInst->pu8KnlVirAddr + pstInst->pu8UsrVirAddr;
    }

    /* The block must be WRITING status and its address must be right */
    if (IS_WRITING(pstBlock) && (u32PhyAddr == pstBlock->u32Addr))
    {
        /* If size=0, drop this block */
        if (0 == pstBuf->u32Size)
        {
            /* Delete block from list */
            list_del(&pstBlock->stBlockNode);

            /* Free block resource */
            HI_KFREE_BUFMNG(pstBlock);

            pstInst->u32PutOK++;
            BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
            return HI_SUCCESS;
        }

#if (BUFMNG_MARKER_SUPPORT == 1)
        pstBlock->u32Marker = pstBuf->u32Marker;
#endif
#if (BUFMNG_64BITS_PTS_SUPPORT == 1)
        pstBlock->u64Pts = pstBuf->u64Pts;
#else
        pstBlock->u32Pts = (HI_U32)(pstBuf->u64Pts);
#endif
        pstBlock->u32Size  = pstBuf->u32Size;
        pstBlock->enstatus = BUFMNG_BLOCK_FREE;
    }
    else
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        HI_ERR_BUFMNG("Put fail.\n");
        return HI_ERR_BM_WRITE_FREE_ERR;
    }

    /* Manage instance */
    pstInst->u32Used += pstBuf->u32Size;
    pstInst->u32Free -= pstBuf->u32Size;

#if (BUFMNG_MARKER_SUPPORT == 1)

    /* Add up frame number */
    if (0 == (pstBlock->u32Marker & BUFMNG_NOT_END_FRAME_BIT))
    {
        pstInst->u32DataNum++;
    }

#endif

#if (BUFMNG_DEBUG == 1)

    if (pstInst->u32Size != pstInst->u32Used + pstInst->u32Free + pstInst->u32Freeze)
    {
        HI_FATAL_BUFMNG("!!!!!!!!!!!!!!!!!UNmatch when write!!!!!!!!!!!!!!!!!!\n");
        HI_FATAL_BUFMNG("pstInst->u32Size = %d\n", pstInst->u32Size);
        HI_FATAL_BUFMNG("pstInst->u32Used = %d\n", pstInst->u32Used);
        HI_FATAL_BUFMNG("pstInst->u32Free = %d\n", pstInst->u32Free);
        HI_FATAL_BUFMNG("pstInst->u32Freeze = %d\n", pstInst->u32Freeze);
    }

#endif

    /* Put buffer OK times increase */
    pstInst->u32PutOK++;

    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
    return HI_SUCCESS;
}

HI_S32 BUFMNG_AcqReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pstBuf)
{
    BUFMNG_INST_S *pstInst   = HI_NULL;
    BUFMNG_BLOCK_S *pstBlock = HI_NULL;
    HI_SIZE_T ulFlags;

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    pstBuf->u32Size = 0;

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* Request buffer try times increase */
    pstInst->u32RecvTry++;

    /* Buffer empty */
    if ((0 == pstInst->u32Used) || (list_empty(&pstInst->stBlockHead)))
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_BUFFER_EMPTY;
    }

#if (BUFMNG_MULTI_READ_SUPPORT == 0)
    /* Find head block */
    BUFMNG_FIND_HEAD_BLOCK(pstInst, pstBlock);

    /* The block must be FREE status */
    if (!IS_FREE(pstBlock))
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_BUFFER_EMPTY;
    }

#else
    /* Find first free block */
    BUFMNG_FIND_FIRST_FREE_BLOCK(pstInst, pstBlock);

    if (HI_NULL == pstBlock)
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_BUFFER_EMPTY;
    }

#endif


    /* Read from the block */
    pstBuf->u32PhyAddr = pstBlock->u32Addr;
    pstBuf->pu8KnlVirAddr = pstBlock->u32Addr - pstInst->u32PhyAddr + pstInst->pu8KnlVirAddr;

    if (HI_NULL != pstInst->pu8UsrVirAddr)
    {
        pstBuf->pu8UsrVirAddr = pstBlock->u32Addr - pstInst->u32PhyAddr + pstInst->pu8UsrVirAddr;
    }
    else
    {
        pstBuf->pu8UsrVirAddr = HI_NULL;
    }

#if (BUFMNG_64BITS_PTS_SUPPORT == 1)
    pstBuf->u64Pts = pstBlock->u64Pts;
#else
    pstBuf->u64Pts = pstBlock->u32Pts;
#endif
    pstBuf->u32Size = pstBlock->u32Size;
#if (BUFMNG_INDEX_SUPPORT == 1)
    pstBuf->u32Index = pstBlock->u32Index;
#endif
#if (BUFMNG_MARKER_SUPPORT == 1)
    pstBuf->u32Marker = pstBlock->u32Marker;
#else
    pstBuf->u32Marker = 0;
#endif

    /* Change status */
    pstBlock->enstatus = BUFMNG_BLOCK_READING;

    /* Request buffer OK times increase */
    pstInst->u32RecvOK++;

    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
    return HI_SUCCESS;
}

HI_S32 BUFMNG_RlsReadBuffer(HI_HANDLE hBuf, BUFMNG_BUF_S *pstBuf)
{
    HI_U32 u32PhyAddr;
    BUFMNG_INST_S *pstInst   = HI_NULL;
    BUFMNG_BLOCK_S *pstBlock = HI_NULL;
    HI_SIZE_T ulFlags;

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* Release buffer try times increase */
    pstInst->u32RlsTry++;

    /* Find block by address */
    if (HI_NULL != pstBuf->pu8UsrVirAddr)
    {
        u32PhyAddr = pstBuf->pu8UsrVirAddr - pstInst->pu8UsrVirAddr + pstInst->u32PhyAddr;
    }
    else
    {
        u32PhyAddr = pstBuf->pu8KnlVirAddr - pstInst->pu8KnlVirAddr + pstInst->u32PhyAddr;
    }

    BUFMNG_FIND_BLOCK_BY_ADDR(u32PhyAddr, pstInst, pstBlock);

    if (HI_NULL == pstBlock)
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_READ_FREE_ERR;
    }

    /* Unfreeze */
    if ((0 != pstInst->u32Freeze) &&
        (pstBlock->u32Addr + pstBlock->u32Size == pstInst->u32PhyAddr + pstInst->u32Size - pstInst->u32Freeze))
    {
        pstInst->u32Free  += pstInst->u32Freeze;
        pstInst->u32Freeze = 0;
    }

#if (BUFMNG_MULTI_READ_SUPPORT == 0)

    /* Only support free orderly */
    if (IS_HEAD_OF_LIST(pstBlock, pstInst) && IS_READING(pstBlock))
    {
        /* Change parameter of instance */
        pstInst->u32Used -= pstBlock->u32Size;
        pstInst->u32Free += pstBlock->u32Size;

#if (BUFMNG_MARKER_SUPPORT == 1)

        /* Sub frame number */
        if (0 == (pstBlock->u32Marker & BUFMNG_NOT_END_FRAME_BIT))
        {
            if (pstInst->u32DataNum > 0)
            {
                pstInst->u32DataNum--;
            }
        }

#endif

        /* Delete block from list */
        list_del(&pstBlock->stBlockNode);

        /* Free block resource */
        HI_KFREE_BUFMNG(pstBlock);
    }
    else
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_READ_FREE_ERR;
    }

#else

    /* Support free out-of-order */
    if (IS_READING(pstBlock))
    {
        /* Change parameter of instance */
        pstInst->u32Used -= pstBlock->u32Size;
        pstInst->u32Free += pstBlock->u32Size;

#if (BUFMNG_MARKER_SUPPORT == 1)

        /* Sub frame number */
        if (0 == (pstBlock->u32Marker & BUFMNG_NOT_END_FRAME_BIT))
        {
            if (pstInst->u32DataNum > 0)
            {
                pstInst->u32DataNum--;
            }
        }

#endif

        /* Delete block from list */
        list_del(&pstBlock->stBlockNode);

        /* Free block resource */
        HI_KFREE_BUFMNG(pstBlock);
    }
    else
    {
        BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
        return HI_ERR_BM_READ_FREE_ERR;
    }

#endif

#if (BUFMNG_DEBUG == 1)

    if (pstInst->u32Size != pstInst->u32Used + pstInst->u32Free + pstInst->u32Freeze)
    {
        HI_FATAL_BUFMNG("!!!!!!!!!!!!!!!!UNmatch when read!!!!!!!!!!!!!!!!!!!!\n");
        HI_FATAL_BUFMNG("pstInst->u32Size = %d\n", pstInst->u32Size);
        HI_FATAL_BUFMNG("pstInst->u32Used = %d\n", pstInst->u32Used);
        HI_FATAL_BUFMNG("pstInst->u32Free = %d\n", pstInst->u32Free);
        HI_FATAL_BUFMNG("pstInst->u32Freeze = %d\n", pstInst->u32Freeze);
    }

#endif

    /* Release buffer OK times increase */
    pstInst->u32RlsOK++;
    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
    return HI_SUCCESS;
}

HI_S32 BUFMNG_Reset(HI_HANDLE hBuf)
{
    BUFMNG_INST_S *pstInst = HI_NULL;
    HI_SIZE_T ulFlags;

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    /* Free all block */
    BUFMNG_FREE_BLOCK_LIST(pstInst);

    /* Set used/free/freeze size */
    pstInst->u32Used    = 0;
    pstInst->u32Free    = pstInst->u32Size;
    pstInst->u32Freeze  = 0;
    pstInst->u32DataNum = 0;
    pstInst->u32GetTry  = 0;
    pstInst->u32GetOK   = 0;
    pstInst->u32PutTry  = 0;
    pstInst->u32PutOK   = 0;
    pstInst->u32RecvTry = 0;
    pstInst->u32RecvOK  = 0;
    pstInst->u32RlsTry  = 0;
    pstInst->u32RlsOK   = 0;

    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);
    return HI_SUCCESS;
}

HI_S32 BUFMNG_GetStatus(HI_HANDLE hBuf, BUFMNG_STATUS_S *pstStatus)
{
    BUFMNG_INST_S *pstInst = HI_NULL;
    HI_SIZE_T ulFlags;

    if (HI_NULL == pstStatus)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);
    pstStatus->u32Used    = pstInst->u32Used + pstInst->u32Freeze;
    pstStatus->u32Free    = pstInst->u32Free;
    pstStatus->u32DataNum = pstInst->u32DataNum;
    pstStatus->u32GetTry  = pstInst->u32GetTry;
    pstStatus->u32GetOK   = pstInst->u32GetOK;
    pstStatus->u32PutTry  = pstInst->u32PutTry;
    pstStatus->u32PutOK   = pstInst->u32PutOK;
    pstStatus->u32RecvTry = pstInst->u32RecvTry;
    pstStatus->u32RecvOK  = pstInst->u32RecvOK;
    pstStatus->u32RlsTry  = pstInst->u32RlsTry;
    pstStatus->u32RlsOK   = pstInst->u32RlsOK;
    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);

    return HI_SUCCESS;
}

#if (BUFMNG_DEBUG == 1)
HI_S32 BUFMNG_Debug(HI_HANDLE hBuf)
{
    BUFMNG_INST_S *pstInst   = HI_NULL;
    BUFMNG_BLOCK_S *pstBlock = HI_NULL;
    struct list_head *pos;
    struct list_head *n;
    HI_S32 i = 0;
    HI_SIZE_T ulFlags;

    BUFMNG_FIND_INST(hBuf, pstInst);

    if (HI_NULL == pstInst)
    {
        return HI_ERR_BM_INVALID_PARA;
    }

    BUFMNG_SPIN_LOCK(pstInst->stSpinLock);

    HI_INFO_BUFMNG("----------------------------------------\n");
    HI_INFO_BUFMNG("SIZE  :%d\n", pstInst->u32Size);
    HI_INFO_BUFMNG("P-ADDR:%d\n", pstInst->u32PhyAddr);
    HI_INFO_BUFMNG("V-ADDR:%p\n", pstInst->pu8VirAddr);
    HI_INFO_BUFMNG("USED  :%d\n", pstInst->u32Used);
    HI_INFO_BUFMNG("FREE  :%d\n", pstInst->u32Free);
    HI_INFO_BUFMNG("FREEZE:%d\n", pstInst->u32Freeze);
    HI_INFO_BUFMNG("FRAME :%d\n", pstInst->u32DataNum);

    if (list_empty(&pstInst->stBlockHead))
    {
        HI_INFO_BUFMNG("\tNONE NODE\n");
    }
    else
    {
        list_for_each_safe(pos, n, &pstInst->stBlockHead)
        {
            pstBlock = list_entry(pos, BUFMNG_BLOCK_S, stBlockNode);
            HI_INFO_BUFMNG("\tNODE %d: V-ADDR=%p SIZE=%d STATUS=%d\n", i++, pstBlock->pu8VirAddr, pstBlock->u32Size,
                           pstBlock->enstatus);
        }
    }

    HI_INFO_BUFMNG("----------------------------------------\n");

    BUFMNG_SPIN_UNLOCK(pstInst->stSpinLock);

    return HI_SUCCESS;
}

#endif


HI_VOID BUFMNG_SaveInit(HI_VOID)
{
    HI_INIT_MUTEX(&stRawSem);
    HI_INIT_MUTEX(&stYuvSem);
    HI_INIT_MUTEX(&stRPUSem);

    return;
}


HI_BOOL BUFMNG_CheckFile(HI_S32 Handle, HI_S8 Flag)
{
    HI_BOOL ret = HI_FALSE;
    struct semaphore *p_stSem;

    switch (Flag)
    {
        case 0:
            p_stSem = &stRawSem;
            break;

        case 1:
            p_stSem = &stYuvSem;
            break;

        case 2:
            p_stSem = &stRPUSem;
            break;

        default:
            HI_ERR_BUFMNG("%s unkown flag(%d).\n", __func__, Flag);
            return -1;
    }


    BUFMNG_LOCK(*p_stSem);

    switch (Flag)
    {
        case 0:
            if (HI_NULL != VdecSaveRawFile && Handle == VdecRawChanNum)
            {
                ret = HI_TRUE;
            }

            break;

        case 1:
            if (HI_NULL != VdecSaveYuvFile && Handle == VdecYuvChanNum)
            {
                ret = HI_TRUE;
            }

            break;

        case 2:
            if (g_VdecSaveRPUFile != HI_NULL && g_VdecRPUChanNum == Handle)
            {
                ret = HI_TRUE;
            }

            break;
    }

    BUFMNG_UNLOCK(*p_stSem);


    return ret;

}


HI_S32 BUFMNG_OpenFile(HI_S32 Handle, HI_S8 *FilePath, HI_S8 Flag)
{
    HI_S32 ret = HI_FAILURE;
    struct semaphore *p_stSem;

    switch (Flag)
    {
        case 0:
            p_stSem = &stRawSem;
            break;

        case 1:
            p_stSem = &stYuvSem;
            break;

        case 2:
            p_stSem = &stRPUSem;
            break;

        default:
            HI_ERR_BUFMNG("%s unkown flag(%d).\n", __func__, Flag);
            return -1;
    }


    BUFMNG_LOCK(*p_stSem);

    switch (Flag)
    {
        case 0:
            VdecSaveRawFile = filp_open(FilePath, O_RDWR | O_CREAT | O_TRUNC, 0);

            if (IS_ERR(VdecSaveRawFile))
            {
                VdecSaveRawFile = HI_NULL;
            }
            else
            {
                VdecRawChanNum = Handle;
                ret = HI_SUCCESS;
            }

            break;

        case 1:
            VdecSaveYuvFile = filp_open(FilePath, O_RDWR | O_CREAT | O_TRUNC, 0);

            if (IS_ERR(VdecSaveYuvFile))
            {
                VdecSaveYuvFile = HI_NULL;
            }
            else
            {
                VdecYuvChanNum = Handle;
                ret = HI_SUCCESS;
            }

            break;

        case 2:
            g_VdecSaveRPUFile = filp_open(FilePath, O_RDWR | O_CREAT | O_TRUNC, 0);

            if (IS_ERR(g_VdecSaveRPUFile))
            {
                g_VdecSaveRPUFile = HI_NULL;
            }
            else
            {
                g_VdecRPUChanNum = Handle;
                ret = HI_SUCCESS;
            }

            break;
    }

    BUFMNG_UNLOCK(*p_stSem);

    if (HI_SUCCESS == ret)
    {
        HI_ERR_BUFMNG("Open file %s of inst%d success.\n", FilePath, Handle);
    }
    else
    {
        HI_ERR_BUFMNG("Open file %s of inst%d failed.\n", FilePath, Handle);
    }

    return ret;

}


HI_S32 BUFMNG_CloseFile(HI_S32 Handle, HI_S8 Flag)
{
    HI_S32 ret = HI_FAILURE;
    struct semaphore *p_stSem;
#if 0

    //Flag==2 dead code
    if (2 == Flag) // Close ALL
    {
        BUFMNG_LOCK(stRawSem);

        if (HI_NULL != VdecSaveRawFile)
        {
            filp_close(VdecSaveRawFile, HI_NULL);
            HI_ERR_BUFMNG("Close raw file of inst%d.\n", VdecRawChanNum);
            VdecSaveRawFile = HI_NULL;
            VdecRawChanNum = -1;
        }

        BUFMNG_UNLOCK(stRawSem);

        BUFMNG_LOCK(stYuvSem);

        if (HI_NULL != VdecSaveYuvFile)
        {
            filp_close(VdecSaveYuvFile, HI_NULL);
            HI_ERR_BUFMNG("Close yuv file of inst%d.\n", VdecYuvChanNum);
            VdecSaveYuvFile = HI_NULL;
            HI_VFREE_BUFMNG(YUV_Array);
            VdecYuvChanNum = -1;
            YUV_Array = U_Array = V_Array = HI_NULL;
        }

        BUFMNG_UNLOCK(stYuvSem);

        return HI_SUCCESS;
    }

#endif

    switch (Flag) // Close Raw/Yuv
    {
        case 0:
            p_stSem = &stRawSem;
            break;

        case 1:
            p_stSem = &stYuvSem;
            break;

        case 2:
            p_stSem = &stRPUSem;
            break;

        default:
            HI_ERR_BUFMNG("%s unkown flag(%d).\n", __func__, Flag);
            return -1;
    }


    BUFMNG_LOCK(*p_stSem);

    switch (Flag)
    {
        case 0:
            if (Handle == VdecRawChanNum)
            {
                filp_close(VdecSaveRawFile, HI_NULL);
                HI_ERR_BUFMNG("Close raw file of inst%d.\n", VdecRawChanNum);
                VdecSaveRawFile = HI_NULL;
                VdecRawChanNum = -1;
                ret = HI_SUCCESS;
            }

            break;

        case 1:
            if (Handle == VdecYuvChanNum)
            {
                filp_close(VdecSaveYuvFile, HI_NULL);
                HI_ERR_BUFMNG("Close yuv file of inst%d.\n", VdecYuvChanNum);
                HI_VFREE_BUFMNG(YUV_Array);
                VdecSaveYuvFile = HI_NULL;
                VdecYuvChanNum = -1;
                YUV_Array = U_Array = V_Array = HI_NULL;
                ret = HI_SUCCESS;
            }

            break;

        case 2:
            if (g_VdecRPUChanNum == Handle)
            {
                filp_close(g_VdecSaveRPUFile, HI_NULL);
                HI_ERR_BUFMNG("Close RPU file of inst%d.\n", g_VdecRPUChanNum);
                g_VdecSaveRPUFile = HI_NULL;
                g_VdecRPUChanNum = -1;

                ret = HI_SUCCESS;
            }

            break;
    }

    BUFMNG_UNLOCK(*p_stSem);

    return ret;

}


HI_S32 BUFMNG_SaveRaw(HI_S32 Handle, HI_S8 *Addr, HI_S32 Length)
{
    HI_S32 len = 0;
    mm_segment_t oldfs;

    BUFMNG_LOCK(stRawSem);

    if (HI_NULL == VdecSaveRawFile || Handle != VdecRawChanNum)
    {
        BUFMNG_UNLOCK(stRawSem);
        return -1;
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    len = VdecSaveRawFile->f_op->write(VdecSaveRawFile, Addr, Length, &VdecSaveRawFile->f_pos);

    if (len != Length)
    {
        HI_ERR_BUFMNG("Save Raw Error return :%d,length :%d", len, Length);
        {
            set_fs(oldfs);
            filp_close(VdecSaveRawFile, HI_NULL);
            HI_ERR_BUFMNG("Close RAW file of inst%d.\n", VdecRawChanNum);
            VdecSaveRawFile = HI_NULL;
            VdecRawChanNum = -1;
            BUFMNG_UNLOCK(stRawSem);
            return -1;
        }
    }

    set_fs(oldfs);

    BUFMNG_UNLOCK(stRawSem);

    return len;

}

HI_S32 BUFMNG_SaveRPU(HI_S32 Handle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S8 *Addr = HI_NULL;
    HI_S32 Length = 0;
    VDEC_BUFFER_S stMBuf = {0};
    HI_S32 len = 0;
    mm_segment_t oldfs;

    BUFMNG_LOCK(stRPUSem);

    if (g_VdecSaveRPUFile == HI_NULL || g_VdecRPUChanNum != Handle)
    {
        BUFMNG_UNLOCK(stRPUSem);

        return 0;
    }

    stMBuf.u32StartPhyAddr = pstFrame->unHDRInfo.stDolbyInfo.stMetadata.u32Addr;
    stMBuf.u32Size = pstFrame->unHDRInfo.stDolbyInfo.stMetadata.u32Length;

    if (HI_DRV_VDEC_Map(&stMBuf) != HI_SUCCESS)
    {
        BUFMNG_UNLOCK(stRPUSem);

        return -1;
    }

    Addr = stMBuf.pu8StartVirAddr;
    Length = stMBuf.u32Size;

    oldfs = get_fs();
    set_fs(KERNEL_DS);
    len = g_VdecSaveRPUFile->f_op->write(g_VdecSaveRPUFile, Addr, Length, &g_VdecSaveRPUFile->f_pos);

    if (Length != len)
    {
        HI_ERR_BUFMNG("Save Rpu Error return :%d,length :%d", len, Length);
        {
            set_fs(oldfs);
            filp_close(g_VdecSaveRPUFile, HI_NULL);
            HI_ERR_BUFMNG("Close RPU file of inst%d.\n", g_VdecRPUChanNum);
            g_VdecSaveRPUFile = HI_NULL;
            g_VdecRPUChanNum = -1;
            BUFMNG_UNLOCK(stRPUSem);
            HI_DRV_VDEC_Unmap(&stMBuf);

            return -1;
        }
    }

    set_fs(oldfs);

    BUFMNG_UNLOCK(stRPUSem);
    HI_DRV_VDEC_Unmap(&stMBuf);

    return len;

}

#ifdef CHIP_TYPE_hi3798cv200
static HI_S32 row_map_table_y[2][4][16] =
{
    {
        //偶数tile
        //{0,2,4,6,8,10,12,14,  1,3,5,7,9,11,13,15},//第0列
        {0, 8, 1, 9, 2, 10, 3, 11,  4, 12, 5, 13, 6, 14, 7, 15},
        //{1,3,5,7,9,11,13,15,  8,10,12,14,0,2,4,6},//第1列
        {12, 0, 13, 1, 14, 2, 15, 3, 8, 4, 9, 5, 10, 6, 11, 7 },
        //{8,10,12,14,0,2,4,6,  9,11,13,15,1,3,5,7},//第2列
        {4, 12, 5, 13, 6, 14, 7, 15,  0, 8, 1, 9, 2, 10, 3, 11},
        //{9,11,13,15,1,3,5,7,  0,2,4,6,8,10,12,14} //第3列
        {8, 4, 9, 5, 10, 6, 11, 7,  12, 0, 13, 1, 14, 2, 15, 3}
    },
    {
        //奇数tile
        //{1,3,5,7,9,11,13,15,  0,2,4,6,8,10,12,14},
        {8, 0, 9, 1, 10, 2, 11, 3,  12, 4, 13, 5, 14, 6, 15, 7},
        //{0,2,4,6,8,10,12,14,  9,11,13,15,1,3,5,7},
        {0, 12, 1, 13, 2, 14, 3, 15,  4, 8, 5, 9, 6, 10, 7, 11},
        //{9,11,13,15,1,3,5,7,  8,10,12,14,0,2,4,6},
        {12, 4, 13, 5, 14, 6, 15, 7,  8, 0, 9, 1, 10, 2, 11, 3},
        //{8,10,12,14,0,2,4,6,  1,3,5,7,9,11,13,15}
        {4, 8, 5, 9, 6, 10, 7, 11,  0, 12, 1, 13, 2, 14, 3, 15}
    }
};

static HI_S32 row_map_table_uv[2][4][8] =
{
    {
        //偶数tile
        //{0,2,4,6,  1,3,5,7},
        {0, 4, 1, 5,  2, 6, 3, 7},
        //{1,3,5,7,  0,2,4,6},
        {4, 0, 5, 1,  6, 2, 7, 3},
        //{1,3,5,7,  0,2,4,6},
        {4, 0, 5, 1,  6, 2, 7, 3},
        //{0,2,4,6,  1,3,5,7},
        {0, 4, 1, 5,  2, 6, 3, 7},
    },
    {
        //奇数tile
        //{1,3,5,7,  0,2,4,6},
        {4, 0, 5, 1,  6, 2, 7, 3},
        //{0,2,4,6,  1,3,5,7},
        {0, 4, 1, 5,  2, 6, 3, 7},
        //{0,2,4,6,  1,3,5,7},
        {0, 4, 1, 5,  2, 6, 3, 7},
        //{1,3,5,7,  0,2,4,6}
        {4, 0, 5, 1,  6, 2, 7, 3}
    }
};

static HI_S32 row_map_table_y_2b[4][32] =
{
    //{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,  1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31},//第0列
    {0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23,   8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31},
    //{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30},//第1列
    {16, 0, 17, 1, 18, 2, 19, 3, 20, 4, 21, 5, 22, 6, 23, 7,   24, 8, 25, 9, 26, 10, 27, 11, 28, 12, 29, 13, 30, 14, 31, 15},
    //{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30},//第2列
    {16, 0, 17, 1, 18, 2, 19, 3, 20, 4, 21, 5, 22, 6, 23, 7,   24, 8, 25, 9, 26, 10, 27, 11, 28, 12, 29, 13, 30, 14, 31, 15},
    //{0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,  1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31} //第3列
    {0, 16, 1, 17, 2, 18, 3, 19, 4, 20, 5, 21, 6, 22, 7, 23,   8, 24, 9, 25, 10, 26, 11, 27, 12, 28, 13, 29, 14, 30, 15, 31}
};

static HI_S32 row_map_table_uv_2b[16] =
{
    //{0,2,4,6,8,10,12,14   1,3,5,7,9,11,13,15},
    0, 8, 1, 9, 2, 10, 3, 11,   4, 12, 5, 13, 6, 14, 7, 15
};
#endif

#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
HI_U32 row_map_table_y[2][4][16] =
{
    {
        //偶数tile
        {0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15},
        {12, 13, 14, 15, 8, 9, 10, 11,  0, 1, 2, 3, 4, 5, 6, 7} ,
        {4, 5, 6, 7, 0, 1, 2, 3,  12, 13, 14, 15, 8, 9, 10, 11},
        {8, 9, 10, 11, 12, 13, 14, 15,  4, 5, 6, 7, 0, 1, 2, 3}
    },
    {
        //奇数tile
        {8, 9, 10, 11, 12, 13, 14, 15,  0, 1, 2, 3, 4, 5, 6, 7},
        {0, 1, 2, 3, 4, 5, 6, 7,  12, 13, 14, 15, 8, 9, 10, 11},
        {12, 13, 14, 15, 8, 9, 10, 11,  4, 5, 6, 7, 0, 1, 2, 3},
        {4, 5, 6, 7, 0, 1, 2, 3,  8, 9, 10, 11, 12, 13, 14, 15}
    }
};

HI_U32 row_map_table_uv[2][4][8] =
{
    {
        //偶数tile
        {0, 1, 2, 3,  4, 5, 6, 7},
        {4, 5, 6, 7,  0, 1, 2, 3},
        {4, 5, 6, 7,  0, 1, 2, 3},
        {0, 1, 2, 3,  4, 5, 6, 7},
    },
    {
        //奇数tile
        {4, 5, 6, 7,  0, 1, 2, 3},
        {0, 1, 2, 3,  4, 5, 6, 7},
        {0, 1, 2, 3,  4, 5, 6, 7},
        {4, 5, 6, 7,  0, 1, 2, 3},
    }
};

HI_U32 row_map_table_y_2b[4][16] =
{
    {0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15}, //第0列
    {8, 9, 10, 11, 12, 13, 14, 15,  0, 1, 2, 3, 4, 5, 6, 7}, //第1列
    {8, 9, 10, 11, 12, 13, 14, 15,  0, 1, 2, 3, 4, 5, 6, 7}, //第2列
    {0, 1, 2, 3, 4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 15}, //第3列
};

HI_U32 row_map_table_uv_2b[8] =
{
    0, 1, 2, 3, 4, 5, 6, 7
};
#endif

HI_U32 CheckIsSoftDec(HI_UNF_VCODEC_TYPE_E enType)
{
    if (enType == HI_UNF_VCODEC_TYPE_MJPEG
        || enType == HI_UNF_VCODEC_TYPE_SORENSON
        || enType == HI_UNF_VCODEC_TYPE_H263)
    {
        return 1;
    }

    else
    {
        return 0;
    }
}

HI_U32 GetFrameBuffer(HI_DRV_VIDEO_FRAME_S *pstFrame,
                             VDEC_BUFFER_S *pstFrmBuf,
                             HI_U8 **Y_Addr,
                             HI_U8 **C_Addr,
                             HI_UNF_VCODEC_TYPE_E enType)
{
    HI_S32 Ret = HI_SUCCESS;

    HI_DRV_VIDEO_PRIVATE_S *pstVideoPriv = (HI_DRV_VIDEO_PRIVATE_S *)(pstFrame->u32Priv);

    if (CheckIsSoftDec(enType))
    {
        *Y_Addr = (HI_S8 *)(HI_SIZE_T)(pstFrame->stBufAddr[0].vir_addr_y);
        *C_Addr = (HI_S8 *)(HI_SIZE_T)(pstFrame->stBufAddr[0].vir_addr_c);
    }
    else
    {
        pstFrmBuf->u32StartPhyAddr = pstFrame->stBufAddr[0].u32PhyAddr_YHead;
        pstFrmBuf->u32Size = (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead) * 3 / 2;
        if (HI_DRV_VDEC_Map(pstFrmBuf) != HI_SUCCESS)
        {
            return HI_FAILURE;
        }

        *Y_Addr = pstFrmBuf->pu8StartVirAddr;
        *C_Addr = *Y_Addr + (pstFrame->stBufAddr[0].u32PhyAddr_CHead - pstFrame->stBufAddr[0].u32PhyAddr_YHead);
    }

    if ((*Y_Addr == NULL) || (*C_Addr == NULL) || (pstVideoPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_ERROR_FLAG))
    {
        if (!CheckIsSoftDec(enType))
        {
            HI_DRV_VDEC_Unmap(pstFrmBuf);
        }
        return HI_FAILURE;
    }

    return Ret;

}

HI_S32 SaveYuvNV21(HI_DRV_VIDEO_FRAME_S *pstFrame, HI_U8 *Y_Addr, HI_U8 *C_Addr, mm_segment_t oldfs)
{
    HI_U32 i, j;
    HI_S32 len = 0;

    if (YUV_Array == HI_NULL)
    {
        YUV_Array = HI_VMALLOC_BUFMNG(pstFrame->u32Width * pstFrame->u32Height / 2);
        if (YUV_Array == HI_NULL)
        {
            set_fs(oldfs);
            filp_close(VdecSaveYuvFile, HI_NULL);
            VdecSaveYuvFile = HI_NULL;
            VdecYuvChanNum = -1;
            len = -1;

            return len;
        }

        U_Array = YUV_Array;
        V_Array = U_Array + pstFrame->u32Width * pstFrame->u32Height / 4;
    }

    //Stride = pstFrame->stBufAddr[0].u32Stride_Y;
    for (i = 0; i < pstFrame->u32Height; i++)
    {
        len = VdecSaveYuvFile->f_op->write(VdecSaveYuvFile, Y_Addr, pstFrame->u32Width, &VdecSaveYuvFile->f_pos);

        if (len < pstFrame->u32Width)
        {
            HI_ERR_BUFMNG("%s write y failed.\n", __func__);
        }

        Y_Addr += pstFrame->stBufAddr[0].u32Stride_Y;
    }

    for (i = 0; i < pstFrame->u32Height / 2; i++)
    {
        for (j = 0; j < pstFrame->u32Width / 2; j++)
        {
            V_Array[i * pstFrame->u32Width / 2 + j] = C_Addr[2 * j];
            U_Array[i * pstFrame->u32Width / 2 + j] = C_Addr[2 * j + 1];
        }

        C_Addr += pstFrame->stBufAddr[0].u32Stride_C;
    }

    len = VdecSaveYuvFile->f_op->write(VdecSaveYuvFile, U_Array,
          pstFrame->u32Height * pstFrame->u32Width / 4, &VdecSaveYuvFile->f_pos);

    if (len != pstFrame->u32Height * pstFrame->u32Width / 4)
    {
        HI_ERR_BUFMNG("%s write u failed.\n", __func__);
    }

    len = VdecSaveYuvFile->f_op->write(VdecSaveYuvFile, V_Array,
          pstFrame->u32Height * pstFrame->u32Width / 4, &VdecSaveYuvFile->f_pos);

    if (len != pstFrame->u32Height * pstFrame->u32Width / 4)
    {
        HI_ERR_BUFMNG("%s write v failed.\n", __func__);
    }

    set_fs(oldfs);

    HI_ERR_BUFMNG("Saving 2D YUV(%dx%d)...\n", pstFrame->u32Width, pstFrame->u32Height);

    return len;
}

HI_S32 WriteFile(HI_U8 *Array, HI_U32 Size, mm_segment_t oldfs, HI_BOOL bVcmpFlag)
{
    HI_S32 len = 0;

    len = VdecSaveYuvFile->f_op->write(VdecSaveYuvFile, Array, Size, &VdecSaveYuvFile->f_pos);
    if (len != Size)
    {
        HI_ERR_BUFMNG("%s write Array failed.len : %d,W*H :%d\n", __func__, len, Size);
        {
            set_fs(oldfs);
            filp_close(VdecSaveYuvFile, HI_NULL);
            HI_ERR_BUFMNG("Close yuv file of inst%d.\n", VdecYuvChanNum);
            VdecSaveYuvFile = HI_NULL;
            len = -1;

            if (bVcmpFlag == HI_TRUE)
            {
                HI_VFREE_BUFMNG(YUV_Array);
                VdecYuvChanNum = -1;
                YUV_Array = U_Array = V_Array = HI_NULL;
            }
        }
    }

    return len;
}

static HI_VOID ConvertYPIXForNV21TileCmpOff(HI_DRV_VIDEO_FRAME_S *pstFrame,
                                            HI_U8 *Y_Addr,
                                            VDEC_BUFFER_S *pstFrmBuf)
{
    HI_U32 i = 0, j = 0, k = 0;
    HI_U8 *dst, *src;
    HI_U32 Stride = 0;
    HI_U32 srcIdx, dstIdx;
    HI_U32 tileX, tileY;
    HI_U8  u8b[64], u2b[64];
    HI_U8* src2b, *dst2b;

    HI_U16 u10b[64];
    HI_U32 Stride_2b =0;

    HI_U16 SlotWidth;

    HI_U8 *Yaddress_2b;
    HI_U8 TileHeight_2b;


    Stride = pstFrame->stBufAddr[0].u32Stride_Y * 16;
    SlotWidth = (pstFrame->u32Width + (256 - 1)) & (~(256 - 1));
#if defined(CHIP_TYPE_hi3798cv200)
    TileHeight_2b = 32;
#endif
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    TileHeight_2b = 16;
#endif
    Stride_2b = SlotWidth / 4 * TileHeight_2b;
    Yaddress_2b = (pstFrame->stBufAddr_LB[0].u32PhyAddr_Y - pstFrame->stBufAddr[0].u32PhyAddr_YHead) + pstFrmBuf->pu8StartVirAddr;

    for (i = 0; i < pstFrame->u32Height; i++)
    {
        for (j = 0; j < pstFrame->u32Width; j += 64)
        {
            if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
            {
                //dst  = (unsigned char*)(YUV_Array+ pstFrame->u32Width*i + j);
                dst = u8b;
                tileY = (i / 16) % 2;
                tileX = (j / 64) % 4;
                srcIdx = i % 16;
                dstIdx = row_map_table_y[tileY][tileX][srcIdx];
                src =  Y_Addr + Stride * (i / 16) + dstIdx * 64 + (j / 64) * 64 * 16;
                memcpy(dst, src, 64);

                dst2b = u2b;
                tileX = (j / 64) % 4;
                srcIdx = i % TileHeight_2b;
                dstIdx = row_map_table_y_2b[tileX][srcIdx];
                src2b = Yaddress_2b + Stride_2b * (i / TileHeight_2b) + (dstIdx * 64 + (j / 64) * 64 * TileHeight_2b) / 4;
                memcpy(dst2b, src2b, 64 / 4);

                for (k = 0; k < 64; k += 4)
                {
                    u10b[k + 0] = (((HI_U16)(dst[k + 0])) << 2) + (((u2b[k >> 2]) >> 0) & 0x03);
                    u10b[k + 1] = (((HI_U16)(dst[k + 1])) << 2) + (((u2b[k >> 2]) >> 2) & 0x03);
                    u10b[k + 2] = (((HI_U16)(dst[k + 2])) << 2) + (((u2b[k >> 2]) >> 4) & 0x03);
                    u10b[k + 3] = (((HI_U16)(dst[k + 3])) << 2) + (((u2b[k >> 2]) >> 6) & 0x03);
                }

                dst  = (unsigned char*)(YUV_Array + (pstFrame->u32Width * i + j) * 2);
                memcpy(dst, u10b, 64 * 2);
            }
            else
            {
                dst  = (unsigned char*)(YUV_Array + pstFrame->u32Width * i + j);
                tileY = (i / 16) % 2;
                tileX = (j / 64) % 4;
                srcIdx = i % 16;
                dstIdx = row_map_table_y[tileY][tileX][srcIdx];
                src =  Y_Addr + Stride * (i / 16) + dstIdx * 64 + (j / 64) * 64 * 16;
                memcpy(dst, src, 64);
            }
        }
    }

    return;
}

static HI_VOID ConvertUVPIXForNV21TileCmpOff(HI_DRV_VIDEO_FRAME_S *pstFrame,
                                             HI_U8 *C_Addr,
                                             VDEC_BUFFER_S *pstFrmBuf)
{
    HI_U32 i, j, k;
    HI_U8 *dst, *src, *tmp;
    HI_U32 Stride = 0;
    HI_U32 srcIdx, dstIdx;
    HI_U32 tileX, tileY;
    HI_U8  u8b[64], u2b[64];
    HI_U8* src2b, *dst2b;
    HI_U16 SlotWidth;
    HI_U16 SlotHeight;
    HI_U16 u10b[64];
    HI_U32 Stride_2b =0;
    HI_U32 SlotHeight_2b;
    HI_U32 UVOffset_2b;
    HI_U8 *Yaddress_2b;
    HI_U8 *Caddress_2b;
    HI_U8 TileHeight_2b;


    Stride = pstFrame->stBufAddr[0].u32Stride_Y * 16;
    SlotWidth = (pstFrame->u32Width + (256 - 1)) & (~(256 - 1));
    SlotHeight = pstFrame->u32Height;
#if defined(CHIP_TYPE_hi3798cv200)
    TileHeight_2b = 16;
#endif
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a) || defined(CHIP_TYPE_hi3796mv200)
    TileHeight_2b = 8;
#endif

    //Stride_2b = SlotWidth / 4 * TileHeight_2b;
    Stride_2b = SlotWidth / 4 * 32;
    SlotHeight_2b = (SlotHeight + 31) & (~31);
    UVOffset_2b = SlotWidth / 4 * SlotHeight_2b;

    Yaddress_2b = (pstFrame->stBufAddr_LB[0].u32PhyAddr_Y - pstFrame->stBufAddr[0].u32PhyAddr_YHead) + pstFrmBuf->pu8StartVirAddr;
    Caddress_2b = Yaddress_2b + UVOffset_2b;

    for (i = 0; i < pstFrame->u32Height / 2; i++)
    {
        for (j = 0; j < pstFrame->u32Width; j += 64)
        {
            if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
            {
                dst   = u8b;
                tileY = (i / 8) % 2;
                tileX = (j / 64) % 4;
                srcIdx = i % 8;
                dstIdx = row_map_table_uv[tileY][tileX][srcIdx];
                src =  C_Addr + (Stride / 2) * (i / 8) + dstIdx * 64 + (j / 64) * 64 * 8;
                memcpy(dst, src, 64);

                dst2b = u2b;
                srcIdx = i % TileHeight_2b;
                dstIdx = row_map_table_uv_2b[srcIdx];
                src2b = Caddress_2b + (Stride_2b / 2) * (i / TileHeight_2b) + (dstIdx * 64 + (j / 64) * 64 * TileHeight_2b) / 4;
                memcpy(dst2b, src2b, 64 / 4);

                for (k = 0; k < 64; k += 4)
                {
                    u10b[k + 0] = (((HI_U16)(u8b[k + 0])) << 2) + (((u2b[k >> 2]) >> 0) & 0x03);
                    u10b[k + 1] = (((HI_U16)(u8b[k + 1])) << 2) + (((u2b[k >> 2]) >> 2) & 0x03);
                    u10b[k + 2] = (((HI_U16)(u8b[k + 2])) << 2) + (((u2b[k >> 2]) >> 4) & 0x03);
                    u10b[k + 3] = (((HI_U16)(u8b[k + 3])) << 2) + (((u2b[k >> 2]) >> 6) & 0x03);
                }

                dst = (unsigned char*)(YUV_Array + (pstFrame->u32Width * i + j) * 2);
                src = (HI_U8*)u10b;
                memcpy(dst, src, 64 * 2);
            }
            else
            {
                dst  = (unsigned char*)(YUV_Array + pstFrame->u32Width * i + j);
                tileY = (i / 8) % 2;
                tileX = (j / 64) % 4;
                srcIdx = i % 8;
                dstIdx = row_map_table_uv[tileY][tileX][srcIdx];
                src =  C_Addr + (Stride / 2) * (i / 8) + dstIdx * 64 + (j / 64) * 64 * 8;
                memcpy(dst, src, 64);
            }
        }
    }

    tmp = YUV_Array;
    {
        for (i = 0; i < pstFrame->u32Height / 2; i++)
        {
            if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
            {
                for (j = 0; j < pstFrame->u32Width / 2; j++)
                {
                    *(HI_U16*)(V_Array + (i * pstFrame->u32Width / 2 + j) * 2) = *(HI_U16*)(tmp + (2 * j) * 2);
                    *(HI_U16*)(U_Array + (i * pstFrame->u32Width / 2 + j) * 2) = *(HI_U16*)(tmp + (2 * j + 1) * 2);
                }

                tmp += (pstFrame->u32Width) * 2;
            }
            else
            {
                for (j = 0; j < pstFrame->u32Width / 2; j++)
                {
                    V_Array[i * pstFrame->u32Width / 2 + j] = tmp[2 * j];
                    U_Array[i * pstFrame->u32Width / 2 + j] = tmp[2 * j + 1];
                }

                tmp += pstFrame->u32Width;
            }
        }
    }

    return;
}

static HI_VOID SaveYUVNV21TileCmpOff(HI_DRV_VIDEO_FRAME_S *pstFrame,
                                 HI_U8 *Y_Addr,
                                 HI_U8 *C_Addr,
                                 VDEC_BUFFER_S *pstFrmBuf,
                                 mm_segment_t oldfs)
{
    HI_S32 len = 0;
    HI_U32 WriteSize;

    if (YUV_Array == HI_NULL)
    {
        pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT ?
        (WriteSize = (pstFrame->u32Width * pstFrame->u32Height * 3 / 2 * 2)) :
        (WriteSize = (pstFrame->u32Width * pstFrame->u32Height * 3 / 2));

        YUV_Array = HI_VMALLOC_BUFMNG(WriteSize);
        if (YUV_Array == HI_NULL)
        {
            set_fs(oldfs);
            filp_close(VdecSaveYuvFile, HI_NULL);
            VdecSaveYuvFile = HI_NULL;
            VdecYuvChanNum = -1;

            return;
        }

        if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
        {
            U_Array = YUV_Array + (pstFrame->u32Width * pstFrame->u32Height * 2);
            V_Array = U_Array + pstFrame->u32Width * pstFrame->u32Height / 2;
        }
        else
        {
            U_Array = YUV_Array + pstFrame->u32Width * pstFrame->u32Height;
            V_Array = U_Array + pstFrame->u32Width * pstFrame->u32Height / 4;
        }
    }

    ConvertYPIXForNV21TileCmpOff(pstFrame, Y_Addr, pstFrmBuf);

    if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
    {
        len = WriteFile(YUV_Array, pstFrame->u32Width * pstFrame->u32Height * 2, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }
    }
    else
    {
        len = WriteFile(YUV_Array, pstFrame->u32Width * pstFrame->u32Height, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }
    }

    ConvertUVPIXForNV21TileCmpOff(pstFrame, C_Addr, pstFrmBuf);

    if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
    {
        len = WriteFile(U_Array, pstFrame->u32Width * pstFrame->u32Height / 2, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }

        len = WriteFile(V_Array, pstFrame->u32Width * pstFrame->u32Height / 2, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }
    }
    else
    {
        len = WriteFile(U_Array, pstFrame->u32Width * pstFrame->u32Height / 4, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }

        len = WriteFile(V_Array, pstFrame->u32Width * pstFrame->u32Height / 4, oldfs, HI_FALSE);
        if (len < 0)
        {
            return;
        }
    }

    HI_ERR_BUFMNG("Saving YUV(%dx%d), cmpFlag=%d, enBitWidth=%d...\n", pstFrame->u32Width, pstFrame->u32Height, 0, pstFrame->enBitWidth);

    return;
}


static HI_VOID SaveYUVNV21TileCmpOn(HI_DRV_VIDEO_FRAME_S *pstFrame,
                                    HI_U8 *Y_Addr,
                                    HI_U8 *C_Addr,
                                    VDEC_BUFFER_S *pstFrmBuf,
                                    mm_segment_t oldfs)
{
    HI_S32 len = 0;
    HI_U32 Stride = 0;
    HI_U8 u8PicInfo[32] = {0};
    HI_U32 head_stride;
    HI_U32 head_inf_size;
    HI_U8 YuvBitDepth;
    HI_U8 CurBitDepth;
    HI_U16 Width;
    HI_U16 Height;
    HI_U16 SlotWidth;
    HI_U16 SlotHeight;
    HI_U8 PicStructure;
    HI_U8 ChromaIdc;
    HI_U8 YuvCmpEn;
    HI_U8 Yuv2DEn;
    HI_U32 WriteSize;

    HI_U32 YOffset_2b;
    HI_U32 SlotHeight_2b;
    HI_U32 UVOffset_2b;

    HI_U8 *Yaddress_2b;
    HI_U8 *Caddress_2b;

    if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
    {
        YuvBitDepth = 10;
        CurBitDepth = 10;
    }
    else
    {
        YuvBitDepth = 8;
        CurBitDepth = 8;
    }

    PicStructure = 0;
    ChromaIdc = 1;
    YuvCmpEn = 1;
    Yuv2DEn = (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_CMP) || (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP);
    Height = pstFrame->u32Height;
    Width = pstFrame->u32Width;
    SlotHeight = pstFrame->u32Height;
    SlotWidth =  pstFrame->stBufAddr[0].u32Stride_Y;
    Stride = pstFrame->stBufAddr[0].u32Stride_Y * 16;
    head_stride = pstFrame->stBufAddr[0].u32Head_Stride;
    head_inf_size = pstFrame->stBufAddr[0].u32Head_Size;

    *(HI_U16*)(u8PicInfo + 0) = Width;
    *(HI_U16*)(u8PicInfo + 2) = Height;
    *(HI_U16*)(u8PicInfo + 4) = SlotWidth;
    *(HI_U16*)(u8PicInfo + 6) = SlotHeight;
    *(HI_U32*)(u8PicInfo + 8) = Stride;
    *(HI_U8*)(u8PicInfo + 12) = PicStructure;
    *(HI_U8*)(u8PicInfo + 13) = ChromaIdc;
    *(HI_U8*)(u8PicInfo + 14) = YuvBitDepth;
    *(HI_U8*)(u8PicInfo + 15) = CurBitDepth;
    *(HI_U8*)(u8PicInfo + 16) = YuvCmpEn;
    *(HI_U8*)(u8PicInfo + 17) = Yuv2DEn;
    *(HI_U32*)(u8PicInfo + 20) = head_stride;
    *(HI_U32*)(u8PicInfo + 24) = head_inf_size;

    HI_ERR_BUFMNG("W:%d, H:%d, S:%#x, Stc:%d, Idc:%d, YBit:%d, CBit:%d, Cmp:%d, head_stride=%#x, head_inf_size=%#x\n",
                  Width, Height, Stride, PicStructure, ChromaIdc, YuvBitDepth, CurBitDepth, YuvCmpEn, head_stride, head_inf_size);

    WriteSize = 32;
    len = WriteFile(u8PicInfo, WriteSize, oldfs, HI_TRUE);
    if (len < 0)
    {
        return;
    }

    WriteSize = head_inf_size + (Stride / 16) * (((Height + 63) / 64) * 64);
    len = WriteFile(Y_Addr, WriteSize, oldfs, HI_TRUE);
    if (len < 0)
    {
        HI_ERR_VDEC("[%s %d]\n",__func__,__LINE__);
        return;
    }

    WriteSize = head_inf_size / 2 + (Stride / 16) * (((Height + 63) / 64) * 64) / 2;
    len = WriteFile(C_Addr, WriteSize, oldfs, HI_TRUE);
    if (len < 0)
    {
        HI_ERR_VDEC("[%s %d]\n",__func__,__LINE__);
        return;
    }

    if (pstFrame->enBitWidth == HI_DRV_PIXEL_BITWIDTH_10BIT)
    {
        SlotHeight = (C_Addr - Y_Addr - head_inf_size) / SlotWidth;
        YOffset_2b = (SlotWidth * SlotHeight + head_inf_size) * 3 / 2;

        SlotHeight_2b = (SlotHeight + 31) & (~31);
        UVOffset_2b = SlotWidth / 4 * SlotHeight_2b;

        Yaddress_2b = Y_Addr + YOffset_2b;
        Caddress_2b = Yaddress_2b + UVOffset_2b;
        WriteSize = (Stride / 16 / 4) * (((Height + 63) / 64) * 64);

        len = WriteFile(Yaddress_2b, WriteSize, oldfs, HI_TRUE);
        if (len < 0)
        {
            HI_ERR_VDEC("[%s %d]\n",__func__,__LINE__);
            return;
        }

        WriteSize = (Stride / 16 / 4) * (((Height + 63) / 64) * 64) / 2;
        len = WriteFile(Caddress_2b, WriteSize, oldfs, HI_TRUE);
        if (len < 0)
        {
            HI_ERR_VDEC("[%s %d]\n",__func__,__LINE__);
            return;
        }
    }

    return;
}

HI_S32 BUFMNG_SaveYuv(HI_S32 Handle, HI_DRV_VIDEO_FRAME_S *pstFrame, HI_UNF_VCODEC_TYPE_E enType)
{
    HI_S32 len = 0;
    HI_BOOL bVcmpFlag = HI_TRUE;
    HI_U8 *Y_Addr = HI_NULL;
    HI_U8 *C_Addr = HI_NULL;
    VDEC_BUFFER_S stFrmBuf = {0};
    mm_segment_t oldfs;

    if (bSaveoneyuv && u32SaveCnt > 0)
    {
        return HI_SUCCESS;
    }
    else if (bSaveoneyuv)
    {
        u32SaveCnt ++;
    }

    BUFMNG_LOCK(stYuvSem);

    if (HI_NULL == VdecSaveYuvFile || Handle != VdecYuvChanNum || HI_NULL == pstFrame)
    {
        BUFMNG_UNLOCK(stYuvSem);
        return -1;
    }

    memset(&stFrmBuf, 0 , sizeof(VDEC_BUFFER_S));
    if (GetFrameBuffer(pstFrame, &stFrmBuf, &Y_Addr, &C_Addr, enType) != HI_SUCCESS)
    {
        BUFMNG_UNLOCK(stYuvSem);

        return HI_FAILURE;
    }

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
    {
        len = SaveYuvNV21(pstFrame, Y_Addr, C_Addr, oldfs);
        if (len < 0)
        {
            goto EXIT;
        }
    }
    else
    {
        HI_DRV_VDEC_GetVcmpFlag(&bVcmpFlag);

        if (!bVcmpFlag)
        {
            SaveYUVNV21TileCmpOff(pstFrame, Y_Addr, C_Addr, &stFrmBuf, oldfs);
        }
        else
        {
            SaveYUVNV21TileCmpOn(pstFrame, Y_Addr, C_Addr, &stFrmBuf, oldfs);
        }

        set_fs(oldfs);
    }

    if (bSaveoneyuv)
    {
        if (BUFMNG_CloseFile(Handle, 1) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed close file for vdec%2d yuv save!\n", Handle);
        }
    }

EXIT:
    if (!CheckIsSoftDec(enType))
    {
        HI_DRV_VDEC_Unmap(&stFrmBuf);
    }
    BUFMNG_UNLOCK(stYuvSem);

    return len;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
