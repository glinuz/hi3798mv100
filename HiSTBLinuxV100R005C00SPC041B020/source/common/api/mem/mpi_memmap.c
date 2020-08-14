/******************************************************************************/
/* file name : mpi_memmap.c                                                   */
/* linux /dev/mem mmap support func                                           */
/*                                                                            */
/*                                                                            */
/* Copyright 2005 Hisilicon Tech. Co., Ltd.                                   */
/* Author :zhouaidi(42136)                                                    */
/* Create date: 2005-04-07                                                    */
/* Modify history                                                             */
/* 2005-06-12:                                                                */
/* Create manage link for the memory mapped successful to avoid remap         */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#include "hi_module.h"
#include "hi_mpi_mem.h"
#include "hi_debug.h"

#define OS_LINUX

#define WRITE_LOG_ERROR(fmt...) HI_ERR_PRINT(HI_ID_MEM, fmt)
#define WRITE_LOG_INFO(fmt...)  HI_INFO_PRINT(HI_ID_MEM, fmt)

#ifdef OS_LINUX
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct tag_MMAP_Node
{
    HI_U32   Start_P;
    HI_U8 *  Start_V;
    HI_U32   length;
    HI_U32   refcount;  /*the count of the memory after mapped*//*CNcomment: map后的空间段的引用计数 */
    struct tag_MMAP_Node * next;
}MMAP_Node_t;

MMAP_Node_t * pMMAPNode = NULL;

#define MEM_PAGE_SIZE  0x1000
#define PAGE_SIZE_MASK 0xfffff000

#define mmmp_dev "/dev/hi_mem"

#endif

static pthread_mutex_t   g_MemmapMutex = PTHREAD_MUTEX_INITIALIZER;
#define HI_MEMMAP_LOCK()     (void)pthread_mutex_lock(&g_MemmapMutex);
#define HI_MEMMAP_UNLOCK()   (void)pthread_mutex_unlock(&g_MemmapMutex);

/* no need considering page_size of 4K */
HI_VOID * HI_MMAP(HI_U32 phy_addr, HI_U32 size)
{
#ifndef OS_LINUX
    return (void *)phy_addr;
#else
    int             mmmp_fd = -1;
    HI_U32          phy_addr_in_page;
    HI_U32          page_diff;
    HI_U32          size_in_page;
    MMAP_Node_t    *pTmp;
    MMAP_Node_t    *pNew;
    HI_U8          *addr = NULL;

    if (size == 0)
    {
        WRITE_LOG_ERROR("size can't be zero!\n");
        return NULL;
    }

    HI_MEMMAP_LOCK();

    /* check if the physical memory space have been mmaped */
    pTmp = pMMAPNode;
    while (pTmp != NULL)
    {
        if( (phy_addr >= pTmp->Start_P) &&
            ( (phy_addr + size) <= (pTmp->Start_P + pTmp->length) ) )
        {
            pTmp->refcount++;   /* reference count increase by 1  */
            HI_MEMMAP_UNLOCK() ;

            return (void *)(pTmp->Start_V + phy_addr - pTmp->Start_P);
        }

        pTmp = pTmp->next;
    }

    HI_MEMMAP_UNLOCK();

    mmmp_fd = open (mmmp_dev, O_RDWR | O_NONBLOCK | O_SYNC); /*without cache*/
    if (mmmp_fd < 0)
    {
        WRITE_LOG_ERROR("open %s error\n", mmmp_dev);
        return NULL;
    }

    /* addr align in page_size(4K) */
    phy_addr_in_page = phy_addr & PAGE_SIZE_MASK;
    page_diff = phy_addr - phy_addr_in_page;

    /* size in page_size */
    size_in_page =((size + page_diff - 1) & PAGE_SIZE_MASK) + MEM_PAGE_SIZE;

    addr = mmap((void *)0, size_in_page, PROT_READ | PROT_WRITE, MAP_SHARED, mmmp_fd, (long)phy_addr_in_page);
    if (addr == MAP_FAILED)
    {
        WRITE_LOG_ERROR("mmap 0x%x error\n", phy_addr_in_page);

        close(mmmp_fd);

        return NULL;
    }

    close(mmmp_fd);

    /* add this mmap to MMAP Node */
    pNew = (MMAP_Node_t *)HI_MALLOC(HI_ID_MEM, sizeof(MMAP_Node_t));
    if (NULL == pNew)
    {
        WRITE_LOG_ERROR("malloc new node failed!\n");
        return NULL;
    }

    pNew->Start_P = phy_addr_in_page;
    pNew->Start_V = addr;
    pNew->length = size_in_page;
    pNew->refcount = 1;
    pNew->next = NULL;

    HI_MEMMAP_LOCK();

    if (pMMAPNode == NULL)
    {
        pMMAPNode = pNew;
    }
    else
    {
        pTmp = pMMAPNode;
        while (pTmp->next != NULL)
        {
            pTmp = pTmp->next;
        }

        pTmp->next = pNew;
    }

    HI_MEMMAP_UNLOCK() ;
     return (HI_VOID *)(addr + page_diff);
#endif
}

/*****************************************************************************
 Prototype    : memunmap
 Description  :
 Input        : void * addr_mapped
 Output       : None
 Return Value : On success, returns 0, on failure -1
 Calls        :
 Called By    :

  History        :
  1.Date         : 2005/12/21
    Author       : Z42136
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MUNMAP(HI_VOID * addr_mapped)
{
    MMAP_Node_t * pPre;
    MMAP_Node_t * pTmp;

    if (pMMAPNode == NULL)
    {
        WRITE_LOG_ERROR("address have not been mmaped!\n");
        return -1;
    }

    HI_MEMMAP_LOCK() ;

    /* check if the physical memory space have been mmaped */
    pTmp = pMMAPNode;
    pPre = pMMAPNode;

    do
    {
        if( ((HI_U8 *)addr_mapped >= pTmp->Start_V) &&
            ((HI_U8 *)addr_mapped < (pTmp->Start_V + pTmp->length)))/*modify by jianglei(40671), should be '<' instead of '<=' */
        {
            pTmp->refcount--;   /* referrence count decrease by 1  */
            if (0 == pTmp->refcount)
            {
                /*when the count is 0 the mapped memory is not in use, use memunmap to reclaim*/
                /*CNcomment:引用计数变为0, 被map的内存空间不再使用,此时需要进行munmap回收 */

                //WRITE_LOG_INFO("map node will be remove:0x%x!\n", pTmp);

                /* delete this map node from pMMAPNode */
                if(pTmp == pMMAPNode)
                {
                    pMMAPNode = pTmp->next;
                }
                else
                {
                    pPre->next = pTmp->next;
                }

                if (munmap((void *)pTmp->Start_V, pTmp->length) != 0 )
                {
                    /* Don't call LOG print between HI_MEMMAP_LOCK and HI_MEMMAP_UNLOCK */
                    //WRITE_LOG_INFO("munmap failed!\n");
                }

                HI_FREE(HI_ID_MEM, pTmp);
            }

            HI_MEMMAP_UNLOCK();
            return 0;
        }

        pPre = pTmp;
        pTmp = pTmp->next;
    } while (pTmp != NULL);

    HI_MEMMAP_UNLOCK() ;

    WRITE_LOG_ERROR("address have not been mmaped!\n");

    return -1;
}

