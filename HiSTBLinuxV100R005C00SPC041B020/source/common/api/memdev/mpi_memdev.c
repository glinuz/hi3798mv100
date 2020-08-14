/******************************************************************************

  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_memdev.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/01
  Description   : Support /dev/hi_mem device.
  History       :
  1.Date        : 2013/07/01
    Author      : l00185424
    Modification: Created file

******************************************************************************/

/******************************* Include Files *******************************/

/* Sys headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "list.h"

/* Unf headers */

/* Drv headers */
#include "hi_drv_memdev.h"

/* Local headers */
#include "mpi_memdev.h"
#include "hi_mpi_mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define MEMDEV_LOCK(Mutex)                                 \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_lock(&Mutex); 	      \
    if ( s32Ret != 0 )                                    \
    {													  \
    	HI_ERR_MEMDEV("MEMDEV_LOCK error\n"); \
    }                                                     \
}while(0)

#define MEMDEV_UNLOCK(Mutex)                               \
do{                                                       \
    HI_S32 s32Ret = pthread_mutex_unlock(&Mutex);         \
    if ( s32Ret != 0 )                                    \
    {    												  \
    	HI_ERR_MEMDEV("MEMDEV_UNLOCK error\n");             \
    }                                                     \
}while(0)

#define MEMDEV_MALLOC(size)       HI_MALLOC(HI_ID_MEMDEV, size)
#define MEMDEV_FREE(addr)         HI_FREE(HI_ID_MEMDEV, addr)

#define MEMDEV_PAGE_ALIGN_MASK    (0xfff)

#define MEMDEV_FIND_NODE_BY_ADDR(pAddr, pMapNode) \
    { \
        struct list_head* pos; \
        struct list_head* n; \
        MEMDEV_MAP_NODE_S* pstTmp; \
        if (!list_empty(&g_stMemdevParam.stList)) \
        { \
            list_for_each_safe(pos, n, &g_stMemdevParam.stList) \
            { \
                pstTmp = list_entry(pos, MEMDEV_MAP_NODE_S, stNode); \
                if (pAddr == pstTmp->pVirAddr) \
                { \
                    pMapNode = pstTmp; \
                    break; \
                } \
            } \
        } \
    }


/*************************** Structure Definition ****************************/

typedef struct tagMEMDEV_MAP_NODE_S
{
    HI_VOID* pVirAddr;
    HI_VOID* pActualVirAddr;
    HI_U32 u32Len;
    struct list_head stNode;
}MEMDEV_MAP_NODE_S;

typedef struct tagMEMDEV_PARAM_S
{
    HI_S32 s32Fd;
    pthread_mutex_t stMutex;
    struct list_head stList;
}MEMDEV_PARAM_S;

/***************************** Global Definition *****************************/


/***************************** Static Definition *****************************/

static MEMDEV_PARAM_S g_stMemdevParam = 
{
    .s32Fd = -1,
    .stMutex = PTHREAD_MUTEX_INITIALIZER,
    .stList = {&g_stMemdevParam.stList,&g_stMemdevParam.stList}
};

/*********************************** Code ************************************/

HI_S32 MPI_MEMDEV_Init(HI_VOID)
{
    MEMDEV_LOCK(g_stMemdevParam.stMutex);

    if (-1 == g_stMemdevParam.s32Fd)
    {
        g_stMemdevParam.s32Fd = open("/dev/"UMAP_DEVNAME_MEMDEV, O_RDWR | O_NONBLOCK | O_SYNC);
        if (-1 == g_stMemdevParam.s32Fd)
        {
            MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
            HI_FATAL_MEMDEV("Open %s err!\n", UMAP_DEVNAME_MEMDEV);
            return HI_FAILURE;
        }
    }

    MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
    return HI_SUCCESS;        
}

HI_S32 MPI_MEMDEV_DeInit(HI_VOID)
{
    MEMDEV_LOCK(g_stMemdevParam.stMutex);

    if (-1 != g_stMemdevParam.s32Fd)
    {
        (HI_VOID)close(g_stMemdevParam.s32Fd);
        g_stMemdevParam.s32Fd = -1;
    }

    MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
    return HI_SUCCESS;
}

HI_S32 MPI_MEMDEV_WriteRegister(HI_U32 u32RegAddr, HI_U32 u32Value)
{
    HI_S32 s32Ret;
    HI_U32 *pu32VirAddr;
    
    s32Ret = MPI_MEMDEV_MapRegister(u32RegAddr, 4, (HI_VOID*)&pu32VirAddr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    *(volatile HI_U32*)pu32VirAddr = u32Value;
    (HI_VOID)MPI_MEMDEV_UnmapRegister((HI_VOID*)pu32VirAddr);
    return HI_SUCCESS;
}

HI_S32 MPI_MEMDEV_ReadRegister(HI_U32 u32RegAddr, HI_U32 *pu32Value)
{
    HI_S32 s32Ret;
    HI_U32 *pu32VirAddr;

    if (HI_NULL == pu32Value)
    {
        return HI_FAILURE;
    }
    
    s32Ret = MPI_MEMDEV_MapRegister(u32RegAddr, 4, (HI_VOID*)&pu32VirAddr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    *pu32Value = *(volatile HI_U32*)pu32VirAddr;
    (HI_VOID)MPI_MEMDEV_UnmapRegister((HI_VOID*)pu32VirAddr);
    return HI_SUCCESS;
}

HI_S32 MPI_MEMDEV_MapRegister(HI_U32 u32RegAddr, HI_U32 u32Length, HI_VOID *pVirAddr)
{
    MEMDEV_MAP_NODE_S* pstMapNode = HI_NULL;
    HI_U32 u32PageAddr = u32RegAddr & (~MEMDEV_PAGE_ALIGN_MASK);
    HI_U32 u32SubAddr = u32RegAddr & MEMDEV_PAGE_ALIGN_MASK;
    HI_U8 *pu8MapAddr = HI_NULL;

    HI_INFO_MEMDEV("u32PageAddr = %#x, u32SubAddr = %#x\n", u32PageAddr, u32SubAddr);

    /* Check param */
    if (HI_NULL == pVirAddr)
    {
        HI_ERR_MEMDEV("Invalid param!\n");
        return HI_FAILURE;
    }

    /* Check init */
    MEMDEV_LOCK(g_stMemdevParam.stMutex); 
    if (g_stMemdevParam.s32Fd == -1)
    {
        MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
        HI_ERR_MEMDEV("MEMDEV not init!\n");
        return HI_FAILURE;
    }
    
    /* Alloc node */
    pstMapNode = (MEMDEV_MAP_NODE_S*)MEMDEV_MALLOC(sizeof(*pstMapNode));
    if (HI_NULL == pstMapNode)
    {
        MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
        HI_ERR_MEMDEV("Malloc %dB fail!\n", sizeof(*pstMapNode));
        return HI_FAILURE;
    }

    /* map */
    pu8MapAddr = (HI_U8 *)mmap(HI_NULL_PTR, u32Length+u32SubAddr, 
            PROT_READ|PROT_WRITE, MAP_SHARED, g_stMemdevParam.s32Fd, (long)u32PageAddr);
    if (pu8MapAddr == MAP_FAILED)
    {
        MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
        MEMDEV_FREE(pstMapNode);
        HI_ERR_MEMDEV("Map 0x%08x failed!\n", u32PageAddr);
        return HI_FAILURE;
    }

    /* Save address and length to node */
    pstMapNode->pActualVirAddr = (HI_VOID*)pu8MapAddr;
    *((HI_U8* *)pVirAddr) = (HI_U8*)(pu8MapAddr + u32SubAddr);
    pstMapNode->pVirAddr = pu8MapAddr + u32SubAddr;
    pstMapNode->u32Len = u32Length+u32SubAddr;

    /* Add node to list */
    list_add_tail(&(pstMapNode->stNode), &(g_stMemdevParam.stList));
    MEMDEV_UNLOCK(g_stMemdevParam.stMutex);

    HI_INFO_MEMDEV("mmap %#x(%#x) to %p(%p), len %d(%d)\n", u32RegAddr, u32PageAddr, 
        pstMapNode->pVirAddr, pstMapNode->pActualVirAddr, u32Length, pstMapNode->u32Len);

    return HI_SUCCESS;
}

HI_S32 MPI_MEMDEV_UnmapRegister(HI_VOID *pVirAddr)
{
    MEMDEV_MAP_NODE_S* pstMapNode = HI_NULL;

    /* Check init */
    MEMDEV_LOCK(g_stMemdevParam.stMutex); 
    if (g_stMemdevParam.s32Fd == -1)
    {
        MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
        HI_ERR_MEMDEV("MEMDEV not init!\n");
        return HI_FAILURE;
    }

    /* Find node */
    MEMDEV_FIND_NODE_BY_ADDR(pVirAddr, pstMapNode);
    if (HI_NULL == pstMapNode)
    {
        MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
        HI_ERR_MEMDEV("Invalid addr:%p!\n", pVirAddr);
        return HI_FAILURE;
    }

    /* munmap */
    (HI_VOID)munmap(pstMapNode->pActualVirAddr, pstMapNode->u32Len);

    /* Delete node */
    list_del(&pstMapNode->stNode);

    /* Free memory */
    MEMDEV_FREE(pstMapNode);
    
    MEMDEV_UNLOCK(g_stMemdevParam.stMutex);
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */

