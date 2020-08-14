#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_module.h"
#include "hi_debug.h"
#include "hi_osal.h"
#include "drv_mmgr.h"
#include "drv_module_ioctl.h"
#include "mpi_mmgr.h"
#include "hi_drv_proc.h"
#include "hi_drv_dev.h"
#include "hi_drv_module.h"
#include "hi_drv_mem.h"
#include "drv_mem.h"

static UMAP_DEVICE_S    g_stModuleMemDev = {{0}};

#define COLOR_START      "\33[32m"
#define COLOR_START_HEAD "\33[35m" /* pink text color*/

#define COLOR_START_RED "\33[31m"  /* red text color */
#define COLOR_END       "\33[0m"

#define ABS_SIZE(s) ( (s) > 0 ? (s) : (-1)*(s) )

DEFINE_SEMAPHORE(g_KModuleMemMutex);

#define kmodule_mem_lock(RET)                                   \
do{                                                             \
    HI_S32 s32LockRet = down_interruptible(&g_KModuleMemMutex); \
    if ( s32LockRet != 0 )                                      \
    {                                                           \
        return RET;                                             \
    }                                                           \
}while(0)

#define kmodule_mem_unlock() up(&g_KModuleMemMutex)


typedef struct tagMemoryInfo
{
    HI_U32   u32PID;
    HI_U32   u32ModuleID; /* this may be same with another item */

    // the size in kernel
    HI_U32   u32KKSize;
    HI_U32   u32KVSize;

    // the size in user mode;
    HI_U32   u32UsrSize;
    HI_U32   u32MMZSize;

    // the max size used by this module.
    HI_U32   u32MaxSize;
}KMODULE_MEM_INFO_S;

typedef struct tagKModuleMemory
{
    KMODULE_MEM_INFO_S stMemInfo;
    HI_U32             u32ItemCnt;

    struct tagKModuleMemory* pNext;
}KMODULE_MEM_ITEM_S;

typedef struct tagKModuleMemoryPOOL
{
    HI_U32 u32Idle;

    KMODULE_MEM_ITEM_S stModuleMemItem;
}KMODULE_MEM_POOL_S;

static KMODULE_MEM_POOL_S* g_KModuleMemBaseAddr = NULL;
static HI_U32       g_u32ModuleMemItemCount = 0;

static KMODULE_MEM_ITEM_S*  g_pstMemItemHeader = NULL;
#if (1 == HI_PROC_SUPPORT)
static HI_U32  g_ModuleMemModInit = 0;
#endif


HI_VOID KMODULE_MEM_Pool_Init(HI_U32 u32Count)
{
    if (NULL == g_KModuleMemBaseAddr)
    {
        g_KModuleMemBaseAddr = (KMODULE_MEM_POOL_S*)kmalloc(u32Count * sizeof(KMODULE_MEM_POOL_S), GFP_KERNEL);

        if (NULL == g_KModuleMemBaseAddr)
        {
            HI_ERR_MEM("malloc %d size failure!\n", u32Count * sizeof(KMODULE_MEM_POOL_S));

            return;
        }

        memset(g_KModuleMemBaseAddr, 0, u32Count * sizeof(KMODULE_MEM_POOL_S));

        g_u32ModuleMemItemCount = u32Count;
    }

    if (NULL == g_pstMemItemHeader)
    {
        g_pstMemItemHeader = (KMODULE_MEM_ITEM_S*)kmalloc(sizeof(KMODULE_MEM_ITEM_S), GFP_KERNEL);

        if (NULL != g_pstMemItemHeader)
        {
            memset(g_pstMemItemHeader, 0, sizeof(KMODULE_MEM_ITEM_S));
            g_pstMemItemHeader->pNext = NULL;
        }
    }
}

HI_VOID KMODULE_MEM_Pool_DeInit(HI_VOID)
{
    if (NULL != g_KModuleMemBaseAddr)
    {
        kfree(g_KModuleMemBaseAddr);

        g_u32ModuleMemItemCount = 0;
    }

    if (NULL != g_pstMemItemHeader)
    {
        kfree(g_pstMemItemHeader);
    }
}

static HI_VOID* KMODULE_MEM_POOL_MALLOC(HI_U32 u32Size)
{
    HI_U32 u32Index = 0;

    for (u32Index=0; u32Index<g_u32ModuleMemItemCount; u32Index++)
    {
        if (g_KModuleMemBaseAddr[u32Index].u32Idle == 0)
        {
            g_KModuleMemBaseAddr[u32Index].u32Idle = 1;
            break;
        }
    }

    if (u32Index != g_u32ModuleMemItemCount)
    {
        return &g_KModuleMemBaseAddr[u32Index].stModuleMemItem;
    }

    return NULL;
}

static HI_VOID KMODULE_MEM_POOL_FREE(HI_VOID* pAddr)
{
    HI_U32 u32Index = 0;

    for (u32Index=0; u32Index<g_u32ModuleMemItemCount; u32Index++)
    {
        if (&g_KModuleMemBaseAddr[u32Index].stModuleMemItem == pAddr)
        {
            g_KModuleMemBaseAddr[u32Index].u32Idle = 0;

            memset(&g_KModuleMemBaseAddr[u32Index].stModuleMemItem, 0, sizeof(g_KModuleMemBaseAddr[u32Index].stModuleMemItem));
            break;
        }
    }

    return;
}

HI_S32 KMODULE_MEM_POOL_FindNode(KMODULE_MEM_ITEM_S* pNodeHeader, HI_U32 u32ModuleID, KMODULE_MEM_ITEM_S** pstNode)
{
    KMODULE_MEM_ITEM_S* pItrNode = NULL;

    if (NULL == pNodeHeader || pstNode == NULL)
    {
        HI_ERR_MEM("add node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    pItrNode = pNodeHeader->pNext;

    while ( NULL != pItrNode )
    {
        if (pItrNode->stMemInfo.u32ModuleID == u32ModuleID)
        {
            break;
        }

        pItrNode = pItrNode->pNext;
    }

    if (pItrNode != NULL)
    {
        *pstNode = pItrNode;

        return HI_SUCCESS;
    }

    return HI_FAILURE;

}

// Make one node and add it to the link tail
HI_S32 KMODULE_MEM_POOL_AddNode(KMODULE_MEM_ITEM_S* pNodeHeader, KMODULE_MEM_ITEM_S* pstNode)
{
    KMODULE_MEM_ITEM_S* pItrNode   = NULL;
    KMODULE_MEM_ITEM_S* pstAddNode = NULL;
    HI_S32      s32Ret     = HI_FAILURE;
    KMODULE_MEM_ITEM_S* pMemItem   = NULL;

    HI_U32 u32TotalSize = 0;

    if (NULL == pNodeHeader || pstNode == NULL)
    {
        HI_ERR_MEM("add node failure, node header is NULL\n");

        return HI_FAILURE;
    }

    HI_INFO_MEM("........ pNodeHeader address is %p, and next address is %p\n", pNodeHeader, pNodeHeader->pNext);

    s32Ret = KMODULE_MEM_POOL_FindNode(pNodeHeader, pstNode->stMemInfo.u32ModuleID, &pMemItem);
    if (HI_SUCCESS == s32Ret)
    {
        pMemItem->stMemInfo.u32KKSize += pstNode->stMemInfo.u32KKSize;
        pMemItem->stMemInfo.u32KVSize += pstNode->stMemInfo.u32KVSize;

        pMemItem->stMemInfo.u32UsrSize += pstNode->stMemInfo.u32UsrSize;
        pMemItem->stMemInfo.u32MMZSize += pstNode->stMemInfo.u32MMZSize;

        u32TotalSize = pMemItem->stMemInfo.u32KKSize + pMemItem->stMemInfo.u32KVSize;
        u32TotalSize += pMemItem->stMemInfo.u32UsrSize + pMemItem->stMemInfo.u32MMZSize;

        if ( u32TotalSize > pMemItem->stMemInfo.u32MaxSize)
        {
            pMemItem->stMemInfo.u32MaxSize = u32TotalSize;
        }

        return HI_SUCCESS;
    }

    pstAddNode = (KMODULE_MEM_ITEM_S*)KMODULE_MEM_POOL_MALLOC(sizeof(KMODULE_MEM_ITEM_S));
    if ( NULL == pstAddNode)
    {
        HI_ERR_MEM("add node failure, malloc node failure.\n");
        return HI_FAILURE;
    }

    memset(pstAddNode, 0, sizeof(KMODULE_MEM_ITEM_S));

    pstAddNode->stMemInfo.u32ModuleID = pstNode->stMemInfo.u32ModuleID;

    pstAddNode->stMemInfo.u32KKSize = pstNode->stMemInfo.u32KKSize;
    pstAddNode->stMemInfo.u32KVSize = pstNode->stMemInfo.u32KVSize;

    pstAddNode->stMemInfo.u32UsrSize = pstNode->stMemInfo.u32UsrSize;
    pstAddNode->stMemInfo.u32MMZSize = pstNode->stMemInfo.u32MMZSize;
    pstAddNode->stMemInfo.u32PID = pstNode->stMemInfo.u32PID;

    pstAddNode->pNext = NULL;

    u32TotalSize = pstAddNode->stMemInfo.u32KKSize + pstAddNode->stMemInfo.u32KVSize;
    u32TotalSize += pstAddNode->stMemInfo.u32UsrSize + pstAddNode->stMemInfo.u32MMZSize;
    if ( u32TotalSize > pstAddNode->stMemInfo.u32MaxSize)
    {
        pstAddNode->stMemInfo.u32MaxSize = u32TotalSize;
    }

    pItrNode = pNodeHeader;

    while ( NULL != pItrNode && NULL != pItrNode->pNext )
    {
        pItrNode = pItrNode->pNext;
    }

    pItrNode->pNext = pstAddNode;
    pNodeHeader->u32ItemCnt++;

    HI_INFO_MEM("++++++++++ pNodeHeader address is %p, and next address is %p\n", pNodeHeader, pNodeHeader->pNext);

    return HI_SUCCESS;
}

HI_S32 KMODULE_MEM_POOL_AddModule(HI_U32 u32ModuleID)
{
    KMODULE_MEM_ITEM_S stNode = {{0}};

    stNode.stMemInfo.u32ModuleID = u32ModuleID;
    stNode.stMemInfo.u32PID = current->pid;

    return KMODULE_MEM_POOL_AddNode(g_pstMemItemHeader, &stNode);
}


// Delete the node from link
HI_S32 KMODULE_MEM_POOL_DelNode(KMODULE_MEM_ITEM_S* pNodeHeader, KMODULE_MEM_ITEM_S* pstNode)
{
    KMODULE_MEM_ITEM_S* pItrNode = NULL;
    KMODULE_MEM_ITEM_S* pDelNode = NULL;

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

            HI_INFO_MEM("delete module node:%#x\n", pDelNode->stMemInfo.u32ModuleID);

            KMODULE_MEM_POOL_FREE(pDelNode);
        }

        pNodeHeader->pNext = NULL;
        pNodeHeader->u32ItemCnt = 0;

        return HI_SUCCESS;
    }

    while ((NULL != pItrNode) && (NULL != pItrNode->pNext))
    {
        if (pItrNode->pNext->stMemInfo.u32ModuleID == pstNode->stMemInfo.u32ModuleID)
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

        HI_INFO_MEM("delete module node:%#x\n", pDelNode->stMemInfo.u32ModuleID);

        KMODULE_MEM_POOL_FREE(pDelNode);

        pNodeHeader->u32ItemCnt--;

        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 KMODULE_MEM_POOL_DelModule(HI_U32 u32ModuleID)
{
    KMODULE_MEM_ITEM_S stNode = {{0}};

    stNode.stMemInfo.u32ModuleID = u32ModuleID;

    return KMODULE_MEM_POOL_DelNode(g_pstMemItemHeader, &stNode);
}


// Make one node and add it to the link tail
HI_S32 KMODULE_MEM_POOL_UpdateNode(KMODULE_MEM_ITEM_S* pNodeHeader, KMODULE_MEM_ITEM_S* pstNode, HI_BOOL bAddMemory)
{
    HI_S32      s32Ret     = HI_FAILURE;
    KMODULE_MEM_ITEM_S* pMemItem   = NULL;

    HI_U32 u32TotalSize = 0;

    if (NULL == pNodeHeader || pstNode == NULL)
    {
        return HI_FAILURE;
    }

    s32Ret = KMODULE_MEM_POOL_FindNode(pNodeHeader, pstNode->stMemInfo.u32ModuleID, &pMemItem);
    if (HI_SUCCESS == s32Ret)
    {
        if (bAddMemory == HI_TRUE)
        {
            HI_INFO_MEM("++++ ++++, update module[0x%08x] memory info:add %d to old:%d\n", pstNode->stMemInfo.u32ModuleID, \
                pstNode->stMemInfo.u32UsrSize, pMemItem->stMemInfo.u32UsrSize);

            pMemItem->stMemInfo.u32KKSize += pstNode->stMemInfo.u32KKSize;
            pMemItem->stMemInfo.u32KVSize += pstNode->stMemInfo.u32KVSize;

            pMemItem->stMemInfo.u32UsrSize += pstNode->stMemInfo.u32UsrSize;
            pMemItem->stMemInfo.u32MMZSize += pstNode->stMemInfo.u32MMZSize;

            u32TotalSize = pMemItem->stMemInfo.u32KKSize + pMemItem->stMemInfo.u32KVSize;
            u32TotalSize += pMemItem->stMemInfo.u32UsrSize + pMemItem->stMemInfo.u32MMZSize;
        }
        else
        {
            HI_INFO_MEM("---- ----, update module[0x%08x] memory info:del:%d from old:%d\n", pstNode->stMemInfo.u32ModuleID, \
                pstNode->stMemInfo.u32MMZSize, pMemItem->stMemInfo.u32MMZSize);

            u32TotalSize = pMemItem->stMemInfo.u32KKSize + pMemItem->stMemInfo.u32KVSize;
            u32TotalSize += pMemItem->stMemInfo.u32UsrSize + pMemItem->stMemInfo.u32MMZSize;

            pMemItem->stMemInfo.u32KKSize -= pstNode->stMemInfo.u32KKSize;
            pMemItem->stMemInfo.u32KVSize -= pstNode->stMemInfo.u32KVSize;
            pMemItem->stMemInfo.u32UsrSize -= pstNode->stMemInfo.u32UsrSize;
            pMemItem->stMemInfo.u32MMZSize -= pstNode->stMemInfo.u32MMZSize;

            u32TotalSize = pMemItem->stMemInfo.u32KKSize + pMemItem->stMemInfo.u32KVSize;
            u32TotalSize += pMemItem->stMemInfo.u32UsrSize + pMemItem->stMemInfo.u32MMZSize;
        }

        HI_INFO_MEM("total size:%d and max size:%d\n", u32TotalSize, pMemItem->stMemInfo.u32MaxSize);

        if ( u32TotalSize > pMemItem->stMemInfo.u32MaxSize)
        {
            pMemItem->stMemInfo.u32MaxSize = u32TotalSize;
        }

        return HI_SUCCESS;
    }

    HI_ERR_MEM("update module memory failure, id = 0x%x!!!\n", pstNode->stMemInfo.u32ModuleID);

    return HI_FAILURE;
}

static HI_S32 Mem_KStatCallback(HI_U32 u32ModuleID, KMEM_TYPE_E enType, HI_S32 s32Size)
{
    HI_S32 s32Ret = HI_FAILURE;
    KMODULE_MEM_ITEM_S stNode = {{0}};

    stNode.stMemInfo.u32ModuleID = u32ModuleID;

    switch(enType)
    {
        case KMEM_TYPE_MMZ:
        {
            stNode.stMemInfo.u32MMZSize = ABS_SIZE(s32Size);
        }
        break;

        case KMEM_TYPE_VMEM:
        {
            stNode.stMemInfo.u32KVSize = ABS_SIZE(s32Size);
        }
        break;

        case KMEM_TYPE_KMEM:
        {
            stNode.stMemInfo.u32KKSize = ABS_SIZE(s32Size);
        }
        break;

        default:
        break;
    }

    kmodule_mem_lock(HI_FAILURE);

    if (s32Size >= 0)
    {
        s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stNode, HI_TRUE);
    }
    else
    {
        s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stNode, HI_FALSE);
    }

    kmodule_mem_unlock();

    return s32Ret;
}

static HI_S32 CMPI_Mem_Ioctl(struct inode *inode, struct file *file,HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    kmodule_mem_lock(HI_FAILURE);

    switch(cmd)
    {
        case CMD_ADD_MODULE_INFO:
        {
            MODULE_INFO_S* pModule = (MODULE_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            // HI_INFO_MEM("++++Add module info and module id=0x%08x name = %s\n", pModule->u32ModuleID, pModule->u8ModuleName);

            stModuleMem.stMemInfo.u32ModuleID = pModule->u32ModuleID;
            stModuleMem.stMemInfo.u32PID = current->pid;
            HI_INFO_MEM("stModuleMem.stMemInfo.u32PID = 0x%08x.\n", stModuleMem.stMemInfo.u32PID);

            s32Ret = KMODULE_MEM_POOL_AddNode(g_pstMemItemHeader, &stModuleMem);
        }
        break;
        case CMD_DEL_MODULE_INFO:
        {
            MODULE_INFO_S* pModule = (MODULE_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            stModuleMem.stMemInfo.u32ModuleID = pModule->u32ModuleID;
            stModuleMem.stMemInfo.u32PID = current->pid;

            HI_INFO_MEM("stModuleMem.stMemInfo.u32PID = 0x%08x.\n", stModuleMem.stMemInfo.u32PID);

            // HI_INFO_MEM("----Del module info and module id=0x%08x name = %s \n", pModule->u32ModuleID, pModule->u8ModuleName);

            s32Ret = KMODULE_MEM_POOL_DelNode(g_pstMemItemHeader, &stModuleMem);
        }
        break;
        case CMD_MEM_ADD_INFO:
        {
            MODULE_MEM_INFO_S* pMemInfo = (MODULE_MEM_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            HI_INFO_MEM("Add module memory with id=0x%08x mmz size:%u, user size:%u\n", pMemInfo->u32ModuleID, pMemInfo->u32SizeMMZ, \
                    pMemInfo->u32SizeUsrMem);

            stModuleMem.stMemInfo.u32ModuleID = pMemInfo->u32ModuleID;
            stModuleMem.stMemInfo.u32MMZSize  = pMemInfo->u32SizeMMZ;
            stModuleMem.stMemInfo.u32UsrSize  = pMemInfo->u32SizeUsrMem;

            //stModuleMem.stMemInfo.u32MaxSize  = pMemInfo->u32MaxMemSize;

            s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stModuleMem, HI_TRUE);
        }
        break;
        case CMD_MEM_DEL_INFO:
        {
            MODULE_MEM_INFO_S* pMemInfo = (MODULE_MEM_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            HI_INFO_MEM("del module memory info ... with id=0x%08x\n", pMemInfo->u32ModuleID);
            HI_INFO_MEM("del user size is %d, and mmz size is %d\n", pMemInfo->u32SizeUsrMem, \
                    pMemInfo->u32SizeMMZ);

            stModuleMem.stMemInfo.u32ModuleID = pMemInfo->u32ModuleID;
            stModuleMem.stMemInfo.u32MMZSize  = pMemInfo->u32SizeMMZ;
            stModuleMem.stMemInfo.u32UsrSize  = pMemInfo->u32SizeUsrMem;

            //stModuleMem.stMemInfo.u32MaxSize  = pMemInfo->u32MaxMemSize;

            s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stModuleMem, HI_FALSE);
        }
        break;
        default:
            kmodule_mem_unlock();
            HI_ERR_MEM("==============Unknown cmd:%#x\n", cmd);
        return HI_FAILURE;
    }

    UNUSED(file);
    UNUSED(inode);

    kmodule_mem_unlock();

    return s32Ret;
}

static long MEM_DRV_Ioctl(struct file *file,
                            HI_U32 cmd,
                            unsigned long arg)
{
    int ret;
    ret=HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_Mem_Ioctl);

    return (long)ret;
}

#ifdef CONFIG_COMPAT
static HI_S32 CMPI_Mem_Compat_Ioctl(struct inode *inode, struct file *file,HI_U32 cmd, HI_VOID *arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    kmodule_mem_lock(HI_FAILURE);

    switch(cmd)
    {
        case CMD_ADD_Compat_MODULE_INFO:
        {
            Compat_MODULE_INFO_S* pModule = (Compat_MODULE_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            // HI_INFO_MEM("++++Add module info and module id=0x%08x name = %s\n", pModule->u32ModuleID, pModule->u8ModuleName);

            stModuleMem.stMemInfo.u32ModuleID = pModule->u32ModuleID;
            stModuleMem.stMemInfo.u32PID = current->pid;
            HI_INFO_MEM("stModuleMem.stMemInfo.u32PID = 0x%08x.\n", stModuleMem.stMemInfo.u32PID);

            s32Ret = KMODULE_MEM_POOL_AddNode(g_pstMemItemHeader, &stModuleMem);
        }
        break;
        case CMD_DEL_Compat_MODULE_INFO:
        {
            Compat_MODULE_INFO_S* pModule = (Compat_MODULE_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            stModuleMem.stMemInfo.u32ModuleID = pModule->u32ModuleID;
            stModuleMem.stMemInfo.u32PID = current->pid;

            HI_INFO_MEM("stModuleMem.stMemInfo.u32PID = 0x%08x.\n", stModuleMem.stMemInfo.u32PID);

            // HI_INFO_MEM("----Del module info and module id=0x%08x name = %s \n", pModule->u32ModuleID, pModule->u8ModuleName);

            s32Ret = KMODULE_MEM_POOL_DelNode(g_pstMemItemHeader, &stModuleMem);
        }
        break;
        case CMD_MEM_ADD_INFO:
        {
            MODULE_MEM_INFO_S* pMemInfo = (MODULE_MEM_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            HI_INFO_MEM("Add module memory with id=0x%08x mmz size:%u, user size:%u\n", pMemInfo->u32ModuleID, pMemInfo->u32SizeMMZ, \
                    pMemInfo->u32SizeUsrMem);

            stModuleMem.stMemInfo.u32ModuleID = pMemInfo->u32ModuleID;
            stModuleMem.stMemInfo.u32MMZSize  = pMemInfo->u32SizeMMZ;
            stModuleMem.stMemInfo.u32UsrSize  = pMemInfo->u32SizeUsrMem;

            //stModuleMem.stMemInfo.u32MaxSize  = pMemInfo->u32MaxMemSize;

            s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stModuleMem, HI_TRUE);
        }
        break;
        case CMD_MEM_DEL_INFO:
        {
            MODULE_MEM_INFO_S* pMemInfo = (MODULE_MEM_INFO_S*)arg;
            KMODULE_MEM_ITEM_S stModuleMem = {{0}};

            HI_INFO_MEM("del module memory info ... with id=0x%08x\n", pMemInfo->u32ModuleID);
            HI_INFO_MEM("del user size is %d, and mmz size is %d\n", pMemInfo->u32SizeUsrMem, \
                    pMemInfo->u32SizeMMZ);

            stModuleMem.stMemInfo.u32ModuleID = pMemInfo->u32ModuleID;
            stModuleMem.stMemInfo.u32MMZSize  = pMemInfo->u32SizeMMZ;
            stModuleMem.stMemInfo.u32UsrSize  = pMemInfo->u32SizeUsrMem;

            //stModuleMem.stMemInfo.u32MaxSize  = pMemInfo->u32MaxMemSize;

            s32Ret = KMODULE_MEM_POOL_UpdateNode(g_pstMemItemHeader, &stModuleMem, HI_FALSE);
        }
        break;
        default:
            kmodule_mem_unlock();
            HI_ERR_MEM("===============Unknown cmd:%#x\n", cmd);
        return HI_FAILURE;
    }

    UNUSED(file);
    UNUSED(inode);

    kmodule_mem_unlock();

    return s32Ret;
}

static long MEM_DRV_Compat_Ioctl(struct file *file,
                            HI_U32 cmd,
                            unsigned long arg)
{
    int ret;
    ret=HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_Mem_Compat_Ioctl);

    return (long)ret;
}
#endif

static HI_S32 MEM_DRV_Release(struct inode * inode, struct file * file)
{
    return 0;
}

static HI_S32 MEM_DRV_Open(struct inode * inode, struct file * file)
{
    return 0;
}

static struct file_operations DRV_mem_Fops=
{
    .owner          = THIS_MODULE,
    .open           = MEM_DRV_Open,
    .unlocked_ioctl = MEM_DRV_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = MEM_DRV_Compat_Ioctl,
#endif
    .release        = MEM_DRV_Release,
};
#if (1 == HI_PROC_SUPPORT)
HI_S32 MemProcWrite( struct file * file,  const char __user * buf,
                     size_t count, loff_t *ppos)
{
    return 0;
}

HI_S32 MemProcRead(struct seq_file *s, HI_VOID *pArg)
{
    HI_U32 u32Count = 0;
    HI_U32 u32Total = 0;
    HI_U8* pModuleName = NULL;
    KMODULE_MEM_ITEM_S* pItr = NULL;
    HI_U32 u32TotalSize = 0;
    HI_U32 u32SizeK = 0;

    if (0 == g_ModuleMemModInit)
    {
        PROC_PRINT(s,"    Mem module not init\n");
        return 0;
    }


    kmodule_mem_lock(HI_FAILURE);

    PROC_PRINT(s, COLOR_START_HEAD);
    PROC_PRINT(s, "----------------------------------------------------------------------------------------------\n");
    PROC_PRINT(s, "|Module Name |    ID    |   MMZ   |   USR_MEM   |   KERNEL_MEM   |   MAX_MEM   |   CUR_MEM   |\n");
    PROC_PRINT(s, "----------------------------------------------------------------------------------------------\n");
    PROC_PRINT(s, COLOR_END);

    if (NULL != g_pstMemItemHeader )
    {
        pItr = g_pstMemItemHeader->pNext;
        u32Total = g_pstMemItemHeader->u32ItemCnt;

        while (NULL != pItr)
        {
            pModuleName = HI_DRV_MODULE_GetNameByID(pItr->stMemInfo.u32ModuleID);

            if (NULL == pModuleName)
            {
                pModuleName = "UnknownModule";
            }

            u32SizeK = pItr->stMemInfo.u32KKSize + pItr->stMemInfo.u32KVSize;

            u32TotalSize = pItr->stMemInfo.u32MMZSize + pItr->stMemInfo.u32UsrSize;
            u32TotalSize += u32SizeK;

            if (u32Count%2 == 0)
            {
                PROC_PRINT(s, COLOR_START_HEAD"|"COLOR_END);
                PROC_PRINT(s, " %-8.8s    0x%-8x   %-8u      %-8u      %-8u      %-8u      %-8u  ", pModuleName, \
                     pItr->stMemInfo.u32ModuleID, pItr->stMemInfo.u32MMZSize, pItr->stMemInfo.u32UsrSize, \
                     u32SizeK, pItr->stMemInfo.u32MaxSize, u32TotalSize);
                PROC_PRINT(s, COLOR_START_HEAD"|\n"COLOR_END);

                u32Count = 1;
            }
            else
            {
                PROC_PRINT(s, COLOR_START_HEAD"|"COLOR_END COLOR_START_RED);
                PROC_PRINT(s, " %-8.8s    0x%-8x   %-8u      %-8u      %-8u      %-8u      %-8u  ", pModuleName, \
                     pItr->stMemInfo.u32ModuleID, pItr->stMemInfo.u32MMZSize, pItr->stMemInfo.u32UsrSize, \
                     u32SizeK, pItr->stMemInfo.u32MaxSize, u32TotalSize);
                PROC_PRINT(s, COLOR_END COLOR_START_HEAD"|"COLOR_END"\n");

               u32Count = 0;
            }

            pItr = pItr->pNext;
        }
    }
    PROC_PRINT(s, COLOR_START_HEAD);
    PROC_PRINT(s, "----------------------------------------------------------------------------------------------\n");
    PROC_PRINT(s, COLOR_END);

    kmodule_mem_unlock();

    return 0;
}
#endif
HI_U32 KModule_MemMgr_GetUsedSize(HI_U32 u32ModuleID)
{
    KMODULE_MEM_ITEM_S* pMemItem   = NULL;

    HI_U32 u32TotalSize = 0;

    kmodule_mem_lock(0);

    KMODULE_MEM_POOL_FindNode(g_pstMemItemHeader, u32ModuleID, &pMemItem);

    kmodule_mem_unlock();

    if ( NULL != pMemItem)
    {
        u32TotalSize = pMemItem->stMemInfo.u32KKSize + pMemItem->stMemInfo.u32KVSize;
        u32TotalSize += pMemItem->stMemInfo.u32MMZSize + pMemItem->stMemInfo.u32UsrSize;
    }

    return u32TotalSize;
}

HI_S32 KModule_MemMgr_Init(HI_U32 u32ModuleCount, HI_U32 u32ModuleMemCount)
{
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S *item ;
#endif
    KModule_Mem_Init(u32ModuleMemCount, Mem_KStatCallback);

    KMODULE_MEM_Pool_Init(u32ModuleCount);
    // 0
    HI_OSAL_Snprintf(g_stModuleMemDev.devfs_name, sizeof(g_stModuleMemDev.devfs_name), "%s", UMAP_DEVNAME_MEM2);
    g_stModuleMemDev.fops = &DRV_mem_Fops;
    g_stModuleMemDev.minor = UMAP_MIN_MINOR_MEM2;
    g_stModuleMemDev.owner  = THIS_MODULE;
    g_stModuleMemDev.drvops = NULL;

    if(HI_DRV_DEV_Register(&g_stModuleMemDev) < 0)
    {
        HI_ERR_MEM("Unable to register dbg dev\n");
        return -1;
    }
    // 1
#if (1 == HI_PROC_SUPPORT)
    item = HI_DRV_PROC_AddModule(HI_MOD_MEM2, NULL, NULL);
    if (! item)
    {
        HI_DRV_DEV_UnRegister(&g_stModuleMemDev);
        return -1;
    }
    //proc read and write interface.
    item->read = MemProcRead;
    item->write = MemProcWrite;
    g_ModuleMemModInit = 1;
#endif
    return 0;
}

HI_VOID KModule_MemMgr_Exit(HI_VOID)
{
    KModule_Mem_DeInit();
    KMODULE_MEM_Pool_DeInit();
#if (1 == HI_PROC_SUPPORT)
    g_ModuleMemModInit = 0;
    HI_DRV_PROC_RemoveModule(HI_MOD_MEM2);
#endif
    HI_DRV_DEV_UnRegister(&g_stModuleMemDev);
    return;
}

