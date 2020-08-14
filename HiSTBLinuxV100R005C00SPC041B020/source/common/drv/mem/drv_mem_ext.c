#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <linux/fs.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "drv_mmgr.h"
#include "drv_mem.h"

typedef struct tagKMEM_INFO_S
{
    HI_VOID* pMemAddr;
    HI_U32   u32Size;
}KMEM_INFO_S;

typedef struct tagKMEM_ITEM_S
{
    KMEM_INFO_S stMemInfo;

    struct tagKMEM_ITEM_S* pNext;
}KMEM_ITEM_S;

typedef struct tagKMEM_POOL_S
{
    HI_U32 u32Idle;

    KMEM_ITEM_S stMemItem;
}KMEM_POOL_S;

static KMEM_POOL_S* g_KMemBaseAddr = NULL;
static KMEM_ITEM_S* g_KMemHeader = NULL;
static HI_U32       g_u32KMemItemCount = 0;

static fnKMemStatCallback g_fnKMemCallback = NULL;

DEFINE_SEMAPHORE(g_KMemMutex);

#define kmem_lock(RET, func, param)                       \
do{                                                       \
    HI_S32 s32LockRet = down_interruptible(&g_KMemMutex); \
    if ( s32LockRet != 0 )                                \
    {                                                     \
        func(param);                                      \
        return RET;                                       \
    }                                                     \
}while(0)

#define kmem_lock_nonret()                                \
do{                                                       \
    HI_S32 s32LockRet = down_interruptible(&g_KMemMutex); \
    if ( s32LockRet != 0 )                                \
    {                                                     \
        return;                                           \
    }                                                     \
}while(0)


#define kmem_unlock() up(&g_KMemMutex)


HI_S32 KMEM_Pool_Init(HI_U32 u32Count)
{
    if (NULL == g_KMemBaseAddr)
    {
        g_KMemBaseAddr = (KMEM_POOL_S*)kmalloc((u32Count+1) * sizeof(KMEM_POOL_S), GFP_KERNEL);
        if (NULL == g_KMemBaseAddr)
        {
            HI_ERR_MEM("malloc %d size failure!\n", u32Count * sizeof(KMEM_POOL_S));

            return HI_FAILURE;
        }

        memset(g_KMemBaseAddr, 0, (u32Count+1) * sizeof(KMEM_POOL_S));

        g_KMemHeader = &g_KMemBaseAddr[0].stMemItem;

        g_KMemBaseAddr[0].u32Idle = 1;

        g_u32KMemItemCount = u32Count+1;
    }

    return HI_SUCCESS;
}

HI_VOID KMEM_Pool_DeInit(HI_VOID)
{
    if (NULL != g_KMemBaseAddr)
    {
        kfree(g_KMemBaseAddr);

        g_KMemHeader = NULL;

        g_KMemBaseAddr = NULL;

        g_u32KMemItemCount = 0;
    }
}

static HI_VOID* KMEM_POOL_MALLOC(HI_U32 u32Size)
{
    HI_U32 u32Index = 0;

    for (u32Index=1; u32Index<g_u32KMemItemCount; u32Index++)
    {
        if (g_KMemBaseAddr[u32Index].u32Idle == 0)
        {
            g_KMemBaseAddr[u32Index].u32Idle = 1;
            break;
        }
    }

    if (u32Index != g_u32KMemItemCount)
    {
        return &g_KMemBaseAddr[u32Index].stMemItem;
    }

    return NULL;
}

static HI_VOID KMEM_POOL_FREE(HI_VOID* pAddr)
{
    HI_U32 u32Index = 0;

    for (u32Index=1; u32Index<g_u32KMemItemCount; u32Index++)
    {
        if (&g_KMemBaseAddr[u32Index].stMemItem == pAddr)
        {
            g_KMemBaseAddr[u32Index].u32Idle = 0;

            memset(&g_KMemBaseAddr[u32Index].stMemItem, 0, sizeof(g_KMemBaseAddr[u32Index].stMemItem));
            break;
        }
    }

    return;
}

HI_S32 KMEM_FindNode(KMEM_ITEM_S* pNodeHeader, HI_VOID* pAddr, KMEM_INFO_S** pstNode)
{
    KMEM_ITEM_S* pItrNode = NULL;

    if (NULL == pNodeHeader || pstNode == NULL)
    {
        HI_ERR_MEM("find node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pItrNode = pNodeHeader->pNext;

    while ( NULL != pItrNode )
    {
        if (pItrNode->stMemInfo.pMemAddr == pAddr)
        {
            break;
        }

        pItrNode = pItrNode->pNext;
    }

    if (pItrNode != NULL)
    {
        *pstNode = &pItrNode->stMemInfo;

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

// Make one node and add it to the link tail
HI_S32 KMEM_AddNode(KMEM_ITEM_S* pNodeHeader, KMEM_ITEM_S* pstNode)
{
    KMEM_ITEM_S* pItrNode = NULL;
    KMEM_ITEM_S* pstAddNode = NULL;

    if (NULL == pNodeHeader || pstNode == NULL)
    {
        HI_ERR_MEM("add node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pstAddNode = (KMEM_ITEM_S*)KMEM_POOL_MALLOC(sizeof(KMEM_ITEM_S));
    if ( NULL == pstAddNode)
    {
        HI_ERR_MEM("add node failure, malloc node failure.\n");
        return HI_FAILURE;
    }

    memset(pstAddNode, 0, sizeof(KMEM_ITEM_S));

    pstAddNode->stMemInfo.pMemAddr = pstNode->stMemInfo.pMemAddr;
    pstAddNode->stMemInfo.u32Size  = pstNode->stMemInfo.u32Size;
    pstAddNode->pNext = NULL;

    pItrNode = pNodeHeader;

    while ( NULL != pItrNode->pNext )
    {
        pItrNode = pItrNode->pNext;
    }

    pItrNode->pNext = pstAddNode;

    return HI_SUCCESS;
}

// Delete the node from link
HI_S32 KMEM_DelNode(KMEM_ITEM_S* pNodeHeader, KMEM_ITEM_S* pstNode)
{
    KMEM_ITEM_S* pItrNode = NULL;
    KMEM_ITEM_S* pDelNode = NULL;

    if (NULL == pNodeHeader || pNodeHeader->pNext == NULL)
    {
        HI_ERR_MEM("delete node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pItrNode = pNodeHeader;

    //if pstNode is NULL, delete all the node, exclusive the header node;
    if (NULL == pstNode)
    {
        pItrNode = pItrNode->pNext;

        while ( NULL != pItrNode )
        {
            pDelNode = pItrNode;
            pItrNode = pItrNode->pNext;

            HI_INFO_MEM("delete module addr:%p\n", pDelNode->stMemInfo.pMemAddr);

            KMEM_POOL_FREE(pDelNode);
        }

        pNodeHeader->pNext = NULL;

        return HI_SUCCESS;
    }

    while ( NULL != pItrNode->pNext)
    {
        if (pItrNode->pNext->stMemInfo.pMemAddr == pstNode->stMemInfo.pMemAddr)
        {
            break;
        }
        pItrNode = pItrNode->pNext;
    }

    if (NULL != pItrNode && NULL != pItrNode->pNext)
    {
        //found out the next node to delete.
        pDelNode = pItrNode->pNext;
        pItrNode->pNext = pItrNode->pNext->pNext;

        HI_INFO_MEM("delete module node:%p\n", pDelNode->stMemInfo.pMemAddr);

        KMEM_POOL_FREE(pDelNode);

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_VOID* hi_kzalloc(HI_U32 module_id, HI_U32 size, HI_S32 flags)
{
    void* pkAddr = kzalloc(size, flags);
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL != pkAddr)
    {
        KMEM_ITEM_S stItem = {{0}};

        //TODO: add kmalloc info into the module manager.
        stItem.stMemInfo.pMemAddr = pkAddr;
        stItem.stMemInfo.u32Size = size;

        kmem_lock(NULL, kfree, pkAddr);

        //lookup the module info.
        s32Ret = g_fnKMemCallback(module_id, KMEM_TYPE_KMEM, 0);
        if (s32Ret != HI_SUCCESS)
        {
            kfree(pkAddr);

            kmem_unlock();

            return NULL;
        }

        s32Ret = KMEM_AddNode(g_KMemHeader, &stItem);

        kmem_unlock();

        if (s32Ret != HI_SUCCESS)
        {
            kfree(pkAddr);

            return NULL;
        }

        s32Ret = g_fnKMemCallback(module_id, KMEM_TYPE_KMEM, size);
    }

    return pkAddr;
}

HI_VOID* hi_kmalloc(HI_U32 module_id, HI_U32 size, HI_S32 flags)
{
    void* pkAddr = kmalloc(size, flags);
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL != pkAddr)
    {
        KMEM_ITEM_S stItem = {{0}};

        //TODO: add kmalloc info into the module manager.
        stItem.stMemInfo.pMemAddr = pkAddr;
        stItem.stMemInfo.u32Size = size;

        kmem_lock(NULL, kfree, pkAddr);

        //lookup the module info.
        s32Ret = g_fnKMemCallback(module_id, KMEM_TYPE_KMEM, 0);
        if (s32Ret != HI_SUCCESS)
        {
            kfree(pkAddr);

            kmem_unlock();

            return NULL;
        }

        s32Ret = KMEM_AddNode(g_KMemHeader, &stItem);

        kmem_unlock();

        if (s32Ret != HI_SUCCESS)
        {
            kfree(pkAddr);

            return NULL;
        }

        s32Ret = g_fnKMemCallback(module_id, KMEM_TYPE_KMEM, size);
    }

    return pkAddr;
}

HI_VOID hi_kfree(HI_U32 module_id, HI_VOID *ptr)
{
    if (NULL != ptr)
    {
        KMEM_ITEM_S stItem = {{0}};
        KMEM_INFO_S *pNode = NULL;
        HI_S32      s32Ret = HI_FAILURE;
        HI_U32     u32Size = 0;

        stItem.stMemInfo.pMemAddr = ptr;

        kfree(ptr);

        //TODO: updae pAddr memory info into module manager.

        kmem_lock_nonret();

        s32Ret = KMEM_FindNode(g_KMemHeader, stItem.stMemInfo.pMemAddr, &pNode);

        if (s32Ret != HI_SUCCESS)
        {
            kmem_unlock();
            return;
        }

        u32Size = pNode->u32Size;

        KMEM_DelNode(g_KMemHeader, &stItem);

        kmem_unlock();

        g_fnKMemCallback(module_id, KMEM_TYPE_KMEM, u32Size*(-1) );
    }
}

HI_VOID* hi_vmalloc(HI_U32 module_id, HI_U32 size)
{
    void* pvAddr = vmalloc(size);
    HI_S32 s32Ret = HI_FAILURE;

    if (NULL != pvAddr)
    {
        KMEM_ITEM_S stItem = {{0}};

        //TODO: Add vmalloc memory info into the module manager.
        stItem.stMemInfo.pMemAddr = pvAddr;
        stItem.stMemInfo.u32Size  = size;

        //stItem.stMemInfo.u32ModuleID = u32ModuleID;

        //lookup the module info.
        s32Ret = g_fnKMemCallback(module_id, KMEM_TYPE_VMEM, 0);
        if (s32Ret != HI_SUCCESS)
        {
            vfree(pvAddr);

            return NULL;
        }

        kmem_lock(NULL, vfree, pvAddr);

        s32Ret = KMEM_AddNode(g_KMemHeader, &stItem);

        kmem_unlock();

        if (s32Ret != HI_SUCCESS)
        {
            vfree(pvAddr);

            return NULL;
        }

        g_fnKMemCallback(module_id, KMEM_TYPE_VMEM, size);
    }

    return pvAddr;
}

HI_VOID hi_vfree(HI_U32 module_id, HI_VOID *ptr)
{
    if (NULL != ptr)
    {
        KMEM_ITEM_S stItem = {{0}};
        KMEM_INFO_S *pNode = NULL;
        HI_S32      s32Ret = HI_FAILURE;
        HI_U32      u32Size = 0;

        stItem.stMemInfo.pMemAddr = ptr;

        vfree(ptr);

        //TODO: update pAddr memory info into the module manager.

        kmem_lock_nonret();

        s32Ret = KMEM_FindNode(g_KMemHeader, stItem.stMemInfo.pMemAddr, &pNode);

        if (s32Ret != HI_SUCCESS)
        {
            kmem_unlock();
            return;
        }

        u32Size = pNode->u32Size;

        KMEM_DelNode(g_KMemHeader, &stItem);

        kmem_unlock();

        g_fnKMemCallback(module_id, KMEM_TYPE_VMEM, u32Size*(-1) );
    }
}

HI_S32 KModule_Mem_Init(HI_U32 u32Count, fnKMemStatCallback fnCallback)
{
    HI_S32 s32Ret = KMEM_Pool_Init(u32Count);

    g_fnKMemCallback = fnCallback;

    return s32Ret;
}

HI_S32 KModule_Mem_DeInit(HI_VOID)
{
    KMEM_Pool_DeInit();

    g_fnKMemCallback = NULL;

    return HI_SUCCESS;
}

EXPORT_SYMBOL(hi_kzalloc);
EXPORT_SYMBOL(hi_kmalloc);
EXPORT_SYMBOL(hi_kfree);
EXPORT_SYMBOL(hi_vmalloc);
EXPORT_SYMBOL(hi_vfree);

