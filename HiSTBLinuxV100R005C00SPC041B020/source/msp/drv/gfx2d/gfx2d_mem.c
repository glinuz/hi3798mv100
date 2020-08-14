/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_mem.c
* Description:
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-08-02              NULL         Create this file.
*******************************************************************************/

#include <linux/fs.h>

#include "hi_gfx2d_type.h"
#include "hi_gfx_comm_k.h"
#include "gfx2d_mem.h"
#include "hi_gfx_sys_k.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

typedef struct
{
    HI_U32          nSize;                   /*内存块大小*/
    HI_U16          nFree;                  /*空闲内存单元个数*/
    HI_U16          nFirst;                  /* 首个空闲内存单元标识*/
    HI_U32          nUnitSize;           /* 内存单元大小 */
    HI_VOID        *pStartAddr;         /*内存块起始地址*/
    HI_U16          nMaxUsed;          /*最大使用个数*/
    HI_U16          nUnitNum;           /*内存单元总个数*/
    HI_U16          nBeUsed;              /*历史申请次数*/
    spinlock_t      lock;                       /*spinlock标识*/
} MemoryBlock;

#define GFX2D_PROC 1
static HI_U32 g_u32MemPoolPhyAddr = 0;/*内存池物理地址*/
static HI_VOID *g_pMemPoolVrtAddr = HI_NULL;/*内存池虚拟地址*/
static HI_U32 g_u32MemPoolSize = 0;/*内存池大小*/

#define RegisterMaxTimes 16/*内存块注册最多次数*/
static HI_U32 g_uRegisterCount = 0;/*内存块已注册次数*/
static MemoryBlock g_struMemBlock[RegisterMaxTimes] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}\
    , {0}, {0}, {0}, {0}, {0}, {0}
};

/*获取自旋锁，锁住对g_struMemBlock的读写*/
void GFX2D_MEM_Lock (spinlock_t *lock, unsigned long *lockflags)
{
    spin_lock_irqsave(lock, *lockflags);
}
/*释放自旋锁*/
void GFX2D_MEM_Unlock (spinlock_t *lock, unsigned long *lockflags)
{
    spin_unlock_irqrestore(lock, *lockflags);
}


/*内存层初始化*/
HI_S32 GFX2D_MEM_Init(const HI_U32 u32Size)
{
    /*申请MMZ内存*/
    HI_BOOL bMmu;

    g_u32MemPoolPhyAddr = HI_GFX_AllocMem("GFX2D_MemPool", NULL, u32Size, &bMmu);
    /*申请失败*/
    if (0 == g_u32MemPoolPhyAddr)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "malloc mempool buffer failed!MemPoolSize = 0x%x\n", u32Size);
        return HI_FAILURE;
    }
    /*映射物理地址为虚拟地址*/
    g_pMemPoolVrtAddr = HI_GFX_Map(g_u32MemPoolPhyAddr, 0, bMmu);
    /*映射失败*/
    if (HI_NULL == g_pMemPoolVrtAddr)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "mempoolphyaddr map failed!\n");
        HI_GFX_FreeMem(g_u32MemPoolPhyAddr, bMmu);
        return HI_FAILURE;
    }
    /*初始化内存池大小*/
    g_u32MemPoolSize = u32Size;

    return HI_SUCCESS;
}

/*内存层去初始化*/
HI_S32 GFX2D_MEM_Deinit(HI_VOID)
{
    HI_U16 i;
    /*判断是否存在未释放的内存*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        if (g_struMemBlock[i].nFree != g_struMemBlock[i].nUnitNum)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "some buffer in use\n");
        }
    }
    /*解除内存池映射*/
    HI_GFX_Unmap (g_pMemPoolVrtAddr, HI_FALSE);
    /*释放内存池空间*/
    HI_GFX_FreeMem( g_u32MemPoolPhyAddr, HI_FALSE);
    /*注销内存池信息*/


    g_u32MemPoolPhyAddr = 0;
    g_pMemPoolVrtAddr = HI_NULL;
    g_u32MemPoolSize = 0;

    return HI_SUCCESS;
}

/*内存层打开*/
HI_S32 GFX2D_MEM_Open(HI_VOID)
{
    return HI_SUCCESS;
}

/*内存层关闭*/
HI_S32 GFX2D_MEM_Close(HI_VOID)
{
    return HI_SUCCESS;
}

/*注册固定大小的内存块*/
HI_S32 GFX2D_MEM_Register(const HI_U32 u32BlockSize, const HI_U32 u32BlockNum)
{
    HI_U16 i, j = 0;
    HI_U32 u32TotalSize;
    HI_U32 u32Size = 0;
    HI_U8 *pData = NULL;
    MemoryBlock TempMemBlock = {0};
    HI_U32 u32MemSize = u32BlockSize * u32BlockNum;

    /*判断内存池是否初始化*/
    if (0 == g_u32MemPoolSize)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "The memory pool is not initial!\n");
        return HI_FAILURE;
    }
    /*内存块大小范围不在内存单元与内存池之间*/
    if (u32MemSize > g_u32MemPoolSize)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "u32MemSize too large or too small!u32MemSize = %d,u32BlockSize=%d\n", u32MemSize, u32BlockSize);
        return HI_FAILURE;
    }
    /*判断注册次数是否超过规定次数*/
    if (g_uRegisterCount >= RegisterMaxTimes)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the times of register is more than the max times \n");
        return HI_FAILURE;
    }
    /*判断是否存在相同内存单元的内存块*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        if (u32BlockSize == g_struMemBlock [i].nUnitSize)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "already  register memory block of the same size \n");
            return HI_FAILURE;
        }
    }
    /*判断内存块总大小是否超出内存池大小*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        u32Size += g_struMemBlock[i].nSize;
        u32TotalSize = u32Size + u32MemSize;
        if (u32TotalSize > g_u32MemPoolSize)
        {
            HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the registered memory size is larger than the memory pool size\n");
            return HI_FAILURE;
        }
    }

    /*更新内存管理信息*/
    g_struMemBlock [g_uRegisterCount].nSize = u32MemSize;/*内存块大小*/
    g_struMemBlock [g_uRegisterCount].nFree = u32MemSize / u32BlockSize; /*空闲单元个数*/
    g_struMemBlock [g_uRegisterCount].nFirst = 0; /*初始化首个空闲单元标识*/
    g_struMemBlock [g_uRegisterCount].nUnitSize = u32BlockSize;/*内存单元大小*/
    g_struMemBlock [g_uRegisterCount].nUnitNum = u32BlockNum;/*内存单元个数*/
    g_struMemBlock [g_uRegisterCount].pStartAddr = (HI_U8 *)(g_pMemPoolVrtAddr + u32Size); /*  内存块起始地址*/
    pData = g_struMemBlock [g_uRegisterCount].pStartAddr;

    spin_lock_init(&(g_struMemBlock [g_uRegisterCount].lock));

    /*初始化每个内存单元的标识*/
    for (i = 1; i < g_struMemBlock [g_uRegisterCount].nUnitNum; i++)
    {

        /* 最后一个unit不做标记，分配到最后一个unit了就说明
        所有的unit都分配光了，因而没有下个可分配unit指示 */
        *(HI_U16 *)pData = i;
        pData += g_struMemBlock[g_uRegisterCount].nUnitSize;

    }
    TempMemBlock = g_struMemBlock[g_uRegisterCount];
    /*将注册的内存块递增排列*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        if (g_struMemBlock[g_uRegisterCount].nUnitSize < g_struMemBlock[i].nUnitSize)
        {
            for (j = g_uRegisterCount; j > i; j--)
            {
                g_struMemBlock[j] = g_struMemBlock[j - 1];
            }

            g_struMemBlock[j] = TempMemBlock;

        }
    }

    /*注册次数加1*/
    g_uRegisterCount++;
    return HI_SUCCESS;
}

/*注销固定大小的内存块*/
HI_S32 GFX2D_Mem_UnRegister(const HI_U32 u32BlockSize)
{
    HI_U16 i, pos = 0;
    MemoryBlock *pBlock = NULL;
    /*找出对应的内存块*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        if (u32BlockSize == g_struMemBlock[i].nUnitSize)
        {
            pBlock = &g_struMemBlock[i];
            /*记录数组位置*/
            pos = i;
            break;
        }
    }
    /*没有找到对应的内存块*/
    if (pBlock == NULL)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "There is no such memory block!\n");
        return HI_FAILURE;
    }
    /*存在未释放的内存*/
    if (g_struMemBlock[i].nFree != g_struMemBlock[i].nUnitNum)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "some memory is not free!\n");
        return HI_FAILURE;
    }

    /*更新全局内存管理数组信息*/
    for (i = pos; i < g_uRegisterCount - pos + 1; i++)
    {
        g_struMemBlock[i] = g_struMemBlock[i + 1]; /*调整数组位置*/
    }
    HI_GFX_Memset(&g_struMemBlock[i + 1], 0, sizeof(MemoryBlock));
    /*内存块个数-1*/
    g_uRegisterCount--;
    return HI_SUCCESS;
}

/*分配内存*/
HI_VOID *GFX2D_MEM_Alloc(const HI_U32 u32Size)
{
    HI_U16 i;
    unsigned long lockflags;
    HI_U8 *pFree = NULL;
    MemoryBlock *pBlock = NULL;
    /*申请的内存大小为0*/
    if (0 == u32Size)
    {
        /*打印warning信息*/
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the alloc memory size can't be 0!\n");
        return NULL;
    }

    /*找出申请的内存所在的内存块*/
    for (i = 0; i < g_uRegisterCount; i++)
    {
        if (u32Size <= g_struMemBlock[i].nUnitSize)
        {
            pBlock = &g_struMemBlock[i];
            break;
        }
    }
    /*没有找到内存块*/
    if (pBlock == NULL)
    {
        /*打印warning信息*/
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the memory block is not registered!\n");
        return NULL;
    }

    /*获取自旋锁*/
    GFX2D_MEM_Lock(&g_struMemBlock[i].lock, &lockflags);

    /*无空闲内存单元*/
    if (!pBlock->nFree)
    {
        GFX2D_MEM_Unlock(&g_struMemBlock[i].lock, &lockflags);
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "There is no free unit !\n");
        return NULL;
    }
    /*由首个空闲单元的标识计算出空闲内存单元地址*/
    pFree = pBlock->pStartAddr + pBlock->nFirst * pBlock->nUnitSize;
    /*修改首个空闲单元的标识*/
    pBlock->nFirst = *(HI_U16 *)pFree;
    /*空闲内存单元数减1*/
    pBlock->nFree--;
    /*内存块申请次数+1*/
    pBlock ->nBeUsed++;
    /*释放自旋锁*/
    GFX2D_MEM_Unlock(&g_struMemBlock[i].lock, &lockflags);
    return pFree;
}

HI_S32 GFX2D_MEM_Unit_Free(HI_U32 i, HI_VOID *pBuf)
{
    MemoryBlock *pBlock = NULL;
    unsigned long lockflags;

    pBlock = &g_struMemBlock[i];
    /*获取自旋锁*/
    GFX2D_MEM_Lock(&g_struMemBlock[i].lock, &lockflags);
    /*空闲单元数+1*/
    pBlock->nFree++;
    /*将申请释放的内存单元的标识指向下一个可以分配的单元*/
    *(HI_U16 *) pBuf = pBlock->nFirst;
    /*将申请释放的内存单元作为首个空闲内存单元*/
    pBlock->nFirst = (pBuf - pBlock->pStartAddr) / pBlock->nUnitSize;
    /*释放自旋锁*/

    pBlock->nMaxUsed++;
    GFX2D_MEM_Unlock(&g_struMemBlock[i].lock, &lockflags);
    return HI_SUCCESS;
}


/*释放内存*/
HI_S32 GFX2D_MEM_Free(HI_VOID *pBuf)
{
    HI_U16 i;
    /*空指针检测*/
    if (NULL == pBuf)
    {
        return HI_FAILURE;
    }

    if ((pBuf < g_pMemPoolVrtAddr)
        || (pBuf >= (g_pMemPoolVrtAddr + g_u32MemPoolSize)))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the addr is out of the range of the memory pool !\n");
        return HI_FAILURE;
    }

    /*由地址计算出对应的内存块*/

    for (i = 0; i < g_uRegisterCount; i++)
    {
        if ((pBuf < g_struMemBlock[i].pStartAddr)
            || (pBuf >= (g_struMemBlock[i].pStartAddr + g_struMemBlock[i].nSize)))
        {
            continue;
        }

        if (HI_SUCCESS == GFX2D_MEM_Unit_Free(i, pBuf))
        {
            return HI_SUCCESS;
        }
    }

    HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the addr is not the right addr of memory !\n");
    return HI_FAILURE;

}

/*获取内存物理地址*/
HI_U32 GFX2D_MEM_GetPhyaddr(HI_VOID *pBuf)
{
    /*地址不在内存池范围内*/
    if ((pBuf < g_pMemPoolVrtAddr)
        || (pBuf >= (g_pMemPoolVrtAddr + g_u32MemPoolSize)))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "the addr is out of the range of the memory pool ! 0x%x, %d\n", g_pMemPoolVrtAddr, g_u32MemPoolSize);
        return 0;
    }
    /*返回pBuf  对应的物理地址*/
    return (g_u32MemPoolPhyAddr + (pBuf - g_pMemPoolVrtAddr));
}

#ifndef GFX2D_PROC_UNSUPPORT
/*MEM proc信息*/
HI_VOID GFX2D_MEM_ReadProc(struct seq_file *p, HI_VOID *v)
{
    HI_U16 i = 0;

    SEQ_Printf(p, "--------- Hisilicon TDE Memory Pool Info ---------\n");
    SEQ_Printf(p, "     MemPoolSize   MemBlockNum       \n");
    SEQ_Printf(p, " %8u   %8u\n", g_u32MemPoolSize, g_uRegisterCount);
    SEQ_Printf(p, "     BlockSize   UnitSize   UnitNum    UnitFreeNum    FirstFreePos    AllocTimes    MaxUsed       \n");
    for (i = 0; i < g_uRegisterCount; i++)
    {
        SEQ_Printf(p, "%8u   %8u  %8u   %8u       %8u        %8u      %8u\n", g_struMemBlock[i].nSize, g_struMemBlock[i].nUnitSize, g_struMemBlock[i].nUnitNum, \
                   g_struMemBlock[i].nFree, g_struMemBlock[i].nFirst, g_struMemBlock[i].nBeUsed, g_struMemBlock[i].nMaxUsed);
    }
    return;
}

HI_S32 GFX2D_MEM_WriteProc(struct file *file, const char __user *buf, \
                           size_t count, loff_t *ppos)
{
    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
