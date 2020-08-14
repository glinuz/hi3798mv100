#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "hi_type.h"
#include "hi_drv_mem.h"

#ifdef CMN_MMGR_SUPPORT
#include "drv_module_ioctl.h"
#include "mpi_mem_base.h"
#include "mpi_mutils.h"


#define HLEN sizeof(struct head)

static struct head *first = NULL;
static struct head *last = NULL;

//!! Reserved
HI_U8* mem_file_name = NULL;
HI_U32 mem_line_number = 0;

//!! Global variable
static pthread_mutex_t g_MemMutex;
static fnModuleInfoCBK g_fnModuleCallback = NULL;
static HI_BOOL g_bMemInitFlag = HI_FALSE;


#define mem_lock(x) pthread_mutex_lock(x)
#define mem_unlock(x) pthread_mutex_unlock(x)


HI_HANDLE g_hMemMgr = 0;

HI_S32 MEM_POOL_Init(HI_U32 u32Count, fnModuleInfoCBK fnCallback)
{
    MEM_UTILS_S stMemMgrPool = {0};
    
    if (g_bMemInitFlag == HI_TRUE)
    {
        HI_INFO_MEM("has been initialized!\n");
        return HI_SUCCESS;
    }

    HI_INFO_MEM("count is %u, and malloc request size is %d\n", u32Count, u32Count * sizeof(USR_MEM_POOL_S));
        
    stMemMgrPool.enType = MEM_POOL_TYPE_USR_MEMORY;

    g_hMemMgr = Mem_Utils_Init(u32Count, stMemMgrPool);

    if (0 == g_hMemMgr)
    {
        return HI_FAILURE;
    }

    g_bMemInitFlag = HI_TRUE;

    g_fnModuleCallback = fnCallback;
        
    pthread_mutex_init(&g_MemMutex, NULL);

    return HI_SUCCESS;
}

HI_VOID MEM_POOL_DeInit(HI_VOID)
{
    if (g_bMemInitFlag == HI_FALSE)
    {
        HI_INFO_MEM("has been deinitialized!\n");
        return;
    }
    
    Mem_Utils_DeInit(g_hMemMgr);

    g_bMemInitFlag = HI_FALSE;

    g_fnModuleCallback = NULL;
        
    pthread_mutex_destroy(&g_MemMutex);
}

static HI_VOID* MEM_POOL_MALLOC(HI_U32 u32Size)
{
    return Mem_Utils_MALLOC(g_hMemMgr);
}

static HI_VOID MEM_POOL_FREE(HI_VOID* pAddr)
{
    Mem_Utils_FREE(g_hMemMgr, pAddr);

    return;
}

static struct head *MEM_Add(void *buf, size_t s)
{
    struct head *p = NULL;
    
    p = MEM_POOL_MALLOC(HLEN);
    if(p)
    {
        p->addr = buf;
        p->size = s;
        p->prev = last;
        p->next = NULL;

        if(last)
            last->next = p;
        else
            first = p;

        last = p;
    }

    return p;
}

static void MEM_Del(struct head *p)
{
    struct head *prev, *next;

    prev = p->prev;
    next = p->next;


    if(prev)
        prev->next = next;
    else
        first = next;

    if(next)
        next->prev = prev;
    else
        last = prev;

    MEM_POOL_FREE(p);
}

static void MEM_Replace(struct head *p, void *buf, size_t s)
{
    p->addr = buf;
    p->size = s;
}

static struct head *MEM_Find(void *addr)
{
    struct head *p;
    
    /* start search from lately allocated blocks */
    for(p = last; p; p = p->prev)
    {
        if(p->addr == addr)
            return p;
    }

    return NULL;
}
#endif

HI_VOID* HI_MALLOC(HI_U32 u32ModuleID, HI_U32 u32Size)
{
    HI_VOID* pMemAddr = NULL;

    if (0 == u32Size)
    {
        HI_ERR_MEM("malloc size is 0!\n");
        return HI_NULL;
    }
    
    pMemAddr = malloc(u32Size);
   
#ifdef CMN_MMGR_SUPPORT
    if (NULL != pMemAddr && g_fnModuleCallback)
    { 
        struct head* pHead = NULL;
        HI_S32 s32MallocSize = 0;
        HI_S32 s32Ret = 0;

        mem_lock(&g_MemMutex);

        //lookup the module info.
        s32Ret = g_fnModuleCallback(u32ModuleID, MEM_TYPE_USR, 0);
        if(s32Ret != HI_SUCCESS)
        {
            mem_unlock(&g_MemMutex);

            free(pMemAddr);
            
    	    return NULL;
        }

        pHead = MEM_Add(pMemAddr, u32Size);
        
        if(NULL != pHead)
        {
            // Add memory info to MODULE MGR
            s32MallocSize = (HI_S32)u32Size;
            
            g_fnModuleCallback(u32ModuleID, MEM_TYPE_USR, s32MallocSize);

            mem_unlock(&g_MemMutex);
            
    	    return pMemAddr;
        }
        else
        {
            mem_unlock(&g_MemMutex);

    	    free(pMemAddr);

            return NULL;
        }
    }
#endif

    return pMemAddr;
}

HI_VOID HI_FREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
#ifdef CMN_MMGR_SUPPORT
    if (NULL != pMemAddr && g_fnModuleCallback)
    {
        struct head *p = NULL;
        HI_S32 s32MallocSize = 0;

        mem_lock(&g_MemMutex);

        p = MEM_Find(pMemAddr);

        if ( NULL != p )
        {
            // Update memory info for MODULE MGR 
            s32MallocSize = (HI_S32)p->size;
            s32MallocSize *= -1;
            
            g_fnModuleCallback(u32ModuleID, MEM_TYPE_USR, s32MallocSize);
            
            MEM_Del(p);
        }

        mem_unlock(&g_MemMutex);      
    }
#endif
 
    free(pMemAddr);   
    return;
}

HI_VOID* HI_CALLOC(HI_U32 u32ModuleID, HI_U32 u32MemBlock, HI_U32 u32Size)
{
    HI_VOID* pMemAddr = NULL;

    if (0 == u32Size)
    {
        HI_ERR_MEM("alloc size is 0!\n");
        return HI_NULL;
    }
    
    pMemAddr = calloc(u32MemBlock, u32Size);
    
#ifdef CMN_MMGR_SUPPORT
    if (NULL != pMemAddr && g_fnModuleCallback)
    {
        struct head* pHead = NULL;
        HI_S32 s32MallocSize = 0;
        HI_S32 s32Ret = HI_FAILURE;

        mem_lock(&g_MemMutex);

        //lookup the module info.
        s32Ret = g_fnModuleCallback(u32ModuleID, MEM_TYPE_USR, 0);
        if(s32Ret != HI_SUCCESS)
        {
            mem_unlock(&g_MemMutex);

            free(pMemAddr);
            
    	    return NULL;
        }

        pHead = MEM_Add(pMemAddr, u32Size*u32MemBlock);
        
        if(NULL != pHead)
        {
            // Add memory info to MODULE MGR
            s32MallocSize = (HI_S32)pHead->size;
            g_fnModuleCallback(u32ModuleID, MEM_TYPE_USR, s32MallocSize);

            mem_unlock(&g_MemMutex);
            
    	    return pMemAddr;
        }
        else
        {
            mem_unlock(&g_MemMutex);
            
    	    free(pMemAddr);

            return NULL;
        }
    }
#endif

    return pMemAddr;
}

