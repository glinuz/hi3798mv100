#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hi_type.h"
#include "hi_drv_mem.h"
#include "mpi_mutils.h"
#include "drv_module_ioctl.h"
#include "mpi_mmgr.h"
#include "mpi_mem_base.h"


char* g_szPoolName[] = {"Module_Mem_Pool", "Adp_Mem_Pool", "Usr_Mem_Pool", "MMZ_Pool", "LOG_Pool", "UnknowType"};

HI_HANDLE Mem_Utils_Init(HI_U32 u32Count, MEM_UTILS_S stMem)
{
    MEM_UTILS_S* pUtils = NULL;
    HI_U32 u32ItemSize = 0;
    HI_HANDLE hResult = 0;

    pUtils = (MEM_UTILS_S*)malloc(sizeof(MEM_UTILS_S));
    if (NULL == pUtils)
    {
       HI_ERR_MEM("malloc %d size failure!\n", u32Count * u32ItemSize);

        return 0;
    }

    memset(pUtils, 0, sizeof(MEM_UTILS_S));
    pUtils->enType = stMem.enType;
    pUtils->u32ItemCount = u32Count;

    switch(stMem.enType)
    {
        case MEM_POOL_TYPE_MODULE:
            u32ItemSize = sizeof(MODULE_MEM_POOL_S)*u32Count;
            break;
        case MEM_POOL_TYPE_MODULE_MEMORY:
            u32ItemSize = sizeof(MODULE_MEM_POOL_S)*u32Count;
            break;
        case MEM_POOL_TYPE_USR_MEMORY:
            u32ItemSize = sizeof(USR_MEM_POOL_S)*u32Count;
            break;
        default:
            pUtils->enType = MEM_POOL_TYPE_BUTT;
            break;
    }

    pUtils->pMemBaseAddr = malloc(u32ItemSize);
    if (NULL == pUtils->pMemBaseAddr)
    {
        HI_ERR_MEM("failed to request %s memory size %d\n", g_szPoolName[pUtils->enType], u32ItemSize);
        free(pUtils);
    }
    else
    {
        memset(pUtils->pMemBaseAddr, 0, u32ItemSize);
        hResult = (HI_HANDLE)pUtils;

        HI_INFO_MEM("successfully, request %s, and base address is 0x%08x\n", g_szPoolName[pUtils->enType], pUtils->pMemBaseAddr);
    }

    return hResult;
}

HI_VOID Mem_Utils_DeInit(HI_HANDLE hUtils)
{
    MEM_UTILS_S* pUtils = (MEM_UTILS_S*)hUtils;

    if (NULL != pUtils)
    {
        if (NULL != pUtils->pMemBaseAddr)
        {
            HI_INFO_MEM("free %s memory, and address is 0x%08x\n", g_szPoolName[pUtils->enType], pUtils->pMemBaseAddr);
            free(pUtils->pMemBaseAddr);
        }

        free((HI_VOID*)hUtils);
    }
}

HI_U32 Mem_Utils_GetItemNo(HI_HANDLE hUtils)
{
    MEM_UTILS_S* pUtils = (MEM_UTILS_S*)hUtils;

    if (NULL != pUtils)
    {
        return pUtils->u32HasCount;
    }

    return 0;
}


HI_VOID* Mem_Utils_MALLOC(HI_HANDLE hUtils)
{
    HI_U32 u32Index = 0;

    MEM_UTILS_S* pUtils = (MEM_UTILS_S*)hUtils;
    HI_VOID* pResult = NULL;

    MODULE_POOL_S *     pModuleBase = NULL;
    MODULE_MEM_POOL_S*  pMemAdpBase = NULL;
    USR_MEM_POOL_S*     pMemBase    = NULL;

    if (NULL != pUtils && pUtils->pMemBaseAddr != NULL)
    {
        switch(pUtils->enType)
        {
            case MEM_POOL_TYPE_MODULE:
            {
                pModuleBase = (MODULE_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            case MEM_POOL_TYPE_MODULE_MEMORY:
            {
                pMemAdpBase = (MODULE_MEM_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            case MEM_POOL_TYPE_USR_MEMORY:
            {
                pMemBase = (USR_MEM_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            default:
            break;
        }


        for (u32Index=0; u32Index<pUtils->u32ItemCount; u32Index++)
        {
            if (NULL != pModuleBase && pModuleBase[u32Index].u32Idle == 0)
            {
                pModuleBase[u32Index].u32Idle = 1;

                pResult = &pModuleBase[u32Index];

                pUtils->u32HasCount++;

                HI_INFO_MEM("request %s and address[%d] is 0x%08x!\n", g_szPoolName[pUtils->enType], u32Index, pResult);
                break;
            }

            if (NULL != pMemAdpBase && pMemAdpBase[u32Index].u32Idle == 0)
            {
                pMemAdpBase[u32Index].u32Idle = 1;

                pResult = &pMemAdpBase[u32Index];

                pUtils->u32HasCount++;

                HI_INFO_MEM("request %s and address[%d] is 0x%08x!\n", g_szPoolName[pUtils->enType], u32Index, pResult);
                break;
            }

            if (NULL != pMemBase && pMemBase[u32Index].u32Idle == 0)
            {
                pMemBase[u32Index].u32Idle = 1;

                pResult = &pMemBase[u32Index];

                pUtils->u32HasCount++;

                HI_INFO_MEM("request %s and address[%d] is 0x%08x!\n", g_szPoolName[pUtils->enType], u32Index, pResult);
                break;
            }
        }

        return pResult;
    }


    HI_ERR_MEM("param is invalid!\n");

    return NULL;
}

HI_VOID Mem_Utils_FREE(HI_HANDLE hUtils, HI_VOID* pAddr)
{
    MEM_UTILS_S* pUtils = (MEM_UTILS_S*)hUtils;
    HI_U32 u32Index = 0;

    MODULE_POOL_S*     pModuleBase = NULL;
    MODULE_MEM_POOL_S* pMemAdpBase = NULL;
    USR_MEM_POOL_S*    pMemBase    = NULL;

    if (NULL != pUtils && pUtils->pMemBaseAddr != NULL)
    {
        switch(pUtils->enType)
        {
            case MEM_POOL_TYPE_MODULE:
            {
                pModuleBase = (MODULE_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            case MEM_POOL_TYPE_MODULE_MEMORY:
            {
                pMemAdpBase = (MODULE_MEM_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            case MEM_POOL_TYPE_USR_MEMORY:
            {
                pMemBase = (USR_MEM_POOL_S*)pUtils->pMemBaseAddr;
            }
            break;
            default:
            break;
        }


        for (u32Index=0; u32Index<pUtils->u32ItemCount; u32Index++)
        {
            if (NULL != pModuleBase && (&pModuleBase[u32Index] == pAddr) )
            {
                pModuleBase[u32Index].u32Idle = 0;

                memset(&pModuleBase[u32Index], 0, sizeof(pModuleBase[u32Index]));

                pUtils->u32HasCount--;

                HI_INFO_MEM("idle %s and address is 0x%08x, successfully!\n", g_szPoolName[pUtils->enType], pAddr);
                break;
            }

            if (NULL != pMemAdpBase && (&pMemAdpBase[u32Index] == pAddr) )
            {
                pMemAdpBase[u32Index].u32Idle = 0;

                memset(&pMemAdpBase[u32Index], 0, sizeof(pMemAdpBase[u32Index]));

                pUtils->u32HasCount--;

                HI_INFO_MEM("idle %s and address is 0x%08x, successfully!\n", g_szPoolName[pUtils->enType], pAddr);
                break;
            }

            if (NULL != pMemBase && (&pMemBase[u32Index] == pAddr) )
            {
                pMemBase[u32Index].u32Idle = 0;

                memset(&pMemBase[u32Index], 0, sizeof(pMemBase[u32Index]));

                pUtils->u32HasCount--;

                HI_INFO_MEM("idle %s and address is 0x%08x, successfully!\n", g_szPoolName[pUtils->enType], pAddr);
                break;
            }

        }

        return ;
    }


    HI_ERR_MEM("idle %s and address is 0x%08x, failure!!!\n", pUtils ? g_szPoolName[pUtils->enType] : g_szPoolName[MEM_POOL_TYPE_BUTT], pAddr);

    return;
}

