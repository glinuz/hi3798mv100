#include <linux/slab.h>

#include "hi_type.h"
#include "drv_mutils.h"
#include "drv_module_ioctl.h"
#include "hi_drv_mem.h"

#ifdef USING_KMEM
#include "common_mem_mgr.h"
#include "mem_mgr_drv.h"
#include "common_mem_base.h"
#endif

#ifdef LOG_PRIVATE
#include "common_log_base.h"
#endif

static HI_U8* g_szPoolName[] __attribute__((unused))= {"Module_Mem_Pool", "Adp_Mem_Pool", "Usr_Mem_Pool", "MMZ_Pool", "UnknowType"};

HI_VOID *KMem_Utils_Init(HI_U32 u32Count, KMEM_UTILS_S stMem)
{
    KMEM_UTILS_S* pUtils = HI_NULL;
    HI_U32 u32ItemSize = 0;

    pUtils = (KMEM_UTILS_S*)kmalloc(sizeof(KMEM_UTILS_S), GFP_KERNEL);
    if (HI_NULL == pUtils)
    {
        HI_ERR_MEM("malloc %d size failure!\n", u32Count * u32ItemSize);

        return HI_NULL;
    }

    memset(pUtils, 0, sizeof(KMEM_UTILS_S));
    pUtils->enType = stMem.enType;
    pUtils->u32ItemCount = u32Count;

    switch(stMem.enType)
    {
        case KMEM_POOL_TYPE_MODULE:
            u32ItemSize = sizeof(MODULE_POOL_S)*u32Count;
            break;

    #ifdef USING_KMEM
        case KMEM_POOL_TYPE_MODULE_MEMORY:
            u32ItemSize = sizeof(MODULE_MEM_POOL_S)*u32Count;
            break;

        case KMEM_POOL_TYPE_USR_MEMORY:
            u32ItemSize = sizeof(USR_MEM_POOL_S)*u32Count;
            break;
    #endif

    #ifdef LOG_PRIVATE
        case KMEM_POOL_TYPE_LOG:
            u32ItemSize = sizeof(MODULE_LOG_POOL_S)*u32Count;
            break;
    #endif

        default:
            pUtils->enType = KMEM_POOL_TYPE_BUTT;
            break;;
    }

    pUtils->pMemBaseAddr = kmalloc(u32ItemSize, GFP_KERNEL);
    if (HI_NULL == pUtils->pMemBaseAddr)
    {
        HI_ERR_MEM("failed to request %s memory size %d\n", g_szPoolName[pUtils->enType], u32ItemSize);
        kfree(pUtils);

        return HI_NULL;
    }

    memset(pUtils->pMemBaseAddr, 0, u32ItemSize);

    return (HI_VOID*)pUtils;
}

HI_VOID KMem_Utils_DeInit(HI_VOID *pUtils)
{
    KMEM_UTILS_S* pKmemUtils = (KMEM_UTILS_S*)pUtils;

    if (HI_NULL != pKmemUtils)
    {
        if (HI_NULL != pKmemUtils->pMemBaseAddr)
        {
            HI_INFO_MEM("free %s memory, and address is %p\n", g_szPoolName[pKmemUtils->enType], pKmemUtils->pMemBaseAddr);
            kfree(pKmemUtils->pMemBaseAddr);
        }

        kfree(pKmemUtils);
    }
}

HI_VOID* KMem_Utils_MALLOC(HI_VOID *pUtils)
{
    HI_U32 u32Index = 0;

    KMEM_UTILS_S* pKmemUtils = (KMEM_UTILS_S*)pUtils;
    HI_VOID* pResult = NULL;

    MODULE_POOL_S * pModuleBase = NULL;

#ifdef USING_KMEM
    MODULE_MEM_POOL_S*     pMemAdpBase = NULL;
    USR_MEM_POOL_S*     pMemBase    = NULL;
#endif

#ifdef LOG_PRIVATE
    MODULE_LOG_POOL_S*  pLogBase    = NULL;
#endif

    if (NULL != pKmemUtils && pKmemUtils->pMemBaseAddr != NULL)
    {
        HI_INFO_MEM("pool type:%d, maxsize:%u, size:%u, membase:0x%x.\n",
            pKmemUtils->enType, pKmemUtils->u32ItemCount, pKmemUtils->u32HasCount, pKmemUtils->pMemBaseAddr);

        switch (pKmemUtils->enType)
        {
            case KMEM_POOL_TYPE_MODULE:
            {
                pModuleBase = (MODULE_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }

        #ifdef USING_KMEM
            case KMEM_POOL_TYPE_MODULE_MEMORY:
            {
                pMemAdpBase = (MODULE_MEM_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }

            case KMEM_POOL_TYPE_USR_MEMORY:
            {
                pMemBase = (USR_MEM_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }
        #endif

        #ifdef LOG_PRIVATE
            case KMEM_POOL_TYPE_LOG:
            {
                pLogBase = (MODULE_LOG_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }
        #endif

            default:
                break;
        }

        for (u32Index = 0; u32Index < pKmemUtils->u32ItemCount; u32Index++)
        {
            if (NULL != pModuleBase && pModuleBase[u32Index].u32Idle == 0)
            {
                pModuleBase[u32Index].u32Idle = 1;

                pResult = &pModuleBase[u32Index];

                pKmemUtils->u32HasCount++;

                HI_INFO_MEM("request %s and address[%d] is %p!\n", g_szPoolName[pKmemUtils->enType], u32Index, pResult);
                break;
            }

        #ifdef USING_KMEM
            if (NULL != pMemAdpBase && pMemAdpBase[u32Index].u32Idle == 0)
            {
                pMemAdpBase[u32Index].u32Idle = 1;

                pResult = &pMemAdpBase[u32Index];

                pKmemUtils->u32HasCount++;

                HI_INFO_MEM("request %s and address[%d] is %p!\n", g_szPoolName[pKmemUtils->enType], u32Index, pResult);
                break;
            }

            if (NULL != pMemBase && pMemBase[u32Index].u32Idle == 0)
            {
                pMemBase[u32Index].u32Idle = 1;

                pResult = &pMemBase[u32Index];

                pKmemUtils->u32HasCount++;

                HI_INFO_MEM("... request %s and address[%d] is %p!\n", g_szPoolName[pKmemUtils->enType], u32Index, pResult);
                break;
            }
        #endif

        #ifdef LOG_PRIVATE
            if (NULL != pLogBase && pLogBase[u32Index].u32Idle == 0)
            {
                pLogBase[u32Index].u32Idle = 1;

                pResult = &pLogBase[u32Index];

                pKmemUtils->u32HasCount++;

                HI_INFO_MEM("... request %s and address[%d] is %p!\n", g_szPoolName[pKmemUtils->enType], u32Index, pResult);
                break;
            }
        #endif
        }

        return pResult;
    }

    HI_ERR_MEM("... param is invalid!\n");

    return NULL;
}

HI_VOID KMem_Utils_FREE(HI_VOID *pUtils, HI_VOID* pAddr)
{
    KMEM_UTILS_S* pKmemUtils = (KMEM_UTILS_S*)pUtils;
    HI_U32 u32Index = 0;

    MODULE_POOL_S* pModuleBase = NULL;

#ifdef USING_KMEM
    MODULE_MEM_POOL_S*    pMemAdpBase = NULL;
    USR_MEM_POOL_S*    pMemBase    = NULL;
#endif

#ifdef LOG_PRIVATE
    MODULE_LOG_POOL_S* pLogBase    = NULL;
#endif

    if (NULL != pKmemUtils && pKmemUtils->pMemBaseAddr != NULL)
    {
        HI_INFO_MEM("pool type:%d, maxsize:%u, size:%u, addr:0x%x.\n", pKmemUtils->enType, pKmemUtils->u32ItemCount, pKmemUtils->u32HasCount,  pAddr);

        switch(pKmemUtils->enType)
        {
            case KMEM_POOL_TYPE_MODULE:
            {
                pModuleBase = (MODULE_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }

        #ifdef USING_KMEM
            case KMEM_POOL_TYPE_MODULE_MEMORY:
            {
                pMemAdpBase = (MODULE_MEM_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }

            case KMEM_POOL_TYPE_USR_MEMORY:
            {
                pMemBase = (USR_MEM_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }
        #endif

        #ifdef LOG_PRIVATE
            case KMEM_POOL_TYPE_LOG:
            {
                pLogBase = (MODULE_LOG_POOL_S*)pKmemUtils->pMemBaseAddr;
                break;
            }
        #endif

            default:
                break;
        }

        for (u32Index = 0; u32Index < pKmemUtils->u32ItemCount; u32Index++)
        {
            if (NULL != pModuleBase && (&pModuleBase[u32Index] == pAddr) )
            {
                pModuleBase[u32Index].u32Idle = 0;

                memset(&pModuleBase[u32Index], 0, sizeof(pModuleBase[u32Index]));

                pKmemUtils->u32HasCount--;

                HI_INFO_MEM("... idle %s and address is %p, successfully!\n", g_szPoolName[pKmemUtils->enType], pAddr);
                break;
            }

        #ifdef USING_KMEM
            if (NULL != pMemAdpBase && (&pMemAdpBase[u32Index] == pAddr) )
            {
                pMemAdpBase[u32Index].u32Idle = 0;

                memset(&pMemAdpBase[u32Index], 0, sizeof(pMemAdpBase[u32Index]));

                pKmemUtils->u32HasCount--;

                HI_INFO_MEM("... idle %s and address is %p, successfully!\n", g_szPoolName[pKmemUtils->enType], pAddr);
                break;
            }

            if (NULL != pMemBase && (&pMemBase[u32Index] == pAddr) )
            {
                pMemBase[u32Index].u32Idle = 0;

                memset(&pMemBase[u32Index], 0, sizeof(pMemBase[u32Index]));

                pKmemUtils->u32HasCount--;

                HI_INFO_MEM("... idle %s and address is %p, successfully!\n", g_szPoolName[pKmemUtils->enType], pAddr);
                break;
            }
        #endif

        #ifdef LOG_PRIVATE
            if (NULL != pLogBase && (&pLogBase[u32Index] == pAddr) )
            {
                pLogBase[u32Index].u32Idle = 0;

                memset(&pLogBase[u32Index], 0, sizeof(pLogBase[u32Index]));

                pKmemUtils->u32HasCount--;

                HI_INFO_MEM("... idle %s and address is %p, successfully!\n", g_szPoolName[pKmemUtils->enType], pAddr);
                break;
            }
        #endif
        }

        return;
    }

    HI_ERR_MEM("... idle %s and address is %p, failure!!!\n", pKmemUtils ? g_szPoolName[pKmemUtils->enType] : g_szPoolName[KMEM_POOL_TYPE_BUTT], pAddr);

    return;
}

