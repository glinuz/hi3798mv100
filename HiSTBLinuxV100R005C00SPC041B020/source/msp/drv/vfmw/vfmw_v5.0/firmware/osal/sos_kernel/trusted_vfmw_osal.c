/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpAVvVw4ERKDSiimgdao4nwOhxbHUZ3TWiJ5iG78
Gfd3klH8W3iaztb4NzHmB+kO9A55s19I1X4ihTFvQTOFv9KYxHpMawiPCR3Ka4lU0d5+uH0P
0JNjHhSetps09OiVzXEHZTIGje4/VmRvCSQegRZsBbk5JlwJXQzTZze6ExhXVw==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/

/******************************************************************************

  版权所有 (C), 2001-2015, 华为技术有限公司

******************************************************************************
    文 件 名   : trusted_vfmw_osal.c
    版 本 号   : 初稿
    作    者   :
    生成日期   :
    最近修改   :
    功能描述   : 为vfmw定制的TEE环境抽象模块


    修改历史   :
    1.日    期 : 2015-09-18
    作    者   :
    修改内容   :

******************************************************************************/
#include "trusted_vfmw_osal.h"
#include "public.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;
#define SECURE_NOTIFY_IRQ_NUM (158+32)

#ifdef ENV_SOS_KERNEL

/* Wang Yun */
/* SpinLock */
SOS_IRQ_SPIN_LOCK g_TaskLock;
SOS_IRQ_SPIN_LOCK g_RecordLock;
SOS_IRQ_SPIN_LOCK g_SpinLock;
SOS_IRQ_SPIN_LOCK g_SpinLock_fsp;
SOS_IRQ_SPIN_LOCK g_SpinLockIrq_Destroy;
SOS_IRQ_SPIN_LOCK g_ActivateLock;
SOS_IRQ_SPIN_LOCK g_SpinLockIsr_MsgQueue;
SOS_IRQ_SPIN_LOCK g_SpinLockIsr_Image2D;
SOS_IRQ_SPIN_LOCK g_FrameBuf_SpinLock;


/* Wang Yun */
SOS_IRQ_SPIN_LOCK *GetSpinLockByEnum(SpinLockType spinLockType)
{
    SOS_IRQ_SPIN_LOCK *pSosSpinLock = NULL;

    switch (spinLockType)
    {
        case G_TASKLOCK:
            pSosSpinLock = &g_TaskLock;
            break;

        case G_RECORDLOCK:
            pSosSpinLock = &g_RecordLock;
            break;

        case G_SPINLOCK:
            pSosSpinLock = &g_SpinLock;
            break;

        case G_SPINLOCK_FSP:
            pSosSpinLock = &g_SpinLock_fsp;
            break;

        case G_SPINLOCKIRQ_DESTROY:
            pSosSpinLock = &g_SpinLockIrq_Destroy;
            break;

        case G_ACTIVATELOCK:
            pSosSpinLock = &g_ActivateLock;
            break;

        case G_SPINLOCKISR_MSGQUE:
            pSosSpinLock = &g_SpinLockIsr_MsgQueue;
            break;

        case G_SPINLOCKISR_IMAGE2D:
            pSosSpinLock = &g_SpinLockIsr_Image2D;
            break;

        case G_FRAMEBUF:
            pSosSpinLock = &g_FrameBuf_SpinLock;
            break;

        default:
            dprint(PRN_ERROR, "%s unkown spinLockType %d\n", __func__, spinLockType);
            break;
    }

    return pSosSpinLock;
}

/*************redefine unused function************/
SINT32 SOS_CreateTask( SOS_TASK *pTask, SINT8 TaskName[], VOID *pTaskFunction )
{
    return SOS_OK;
}

VOID SOS_Mb(VOID)
{
    return;
}

UINT8 *SOS_Register_Map(UINT32 PhyAddr)
{
    return (UINT8*)PhyAddr;
}

VOID SOS_Register_UnMap(UINT8 *VirAddr)
{
    return;
}

VOID SOS_udelay(UINT32 usecs)
{
    return;
}

SINT32 SOS_InitWaitQue(MutexType mutextType, SINT32 initVal)
{
    return SOS_OK;
}
extern UINT32 bit_set_by_offset_base(UINT32 *base,UINT32 offset, UINT32 set);
SINT32 SOS_WakeupWaitQue(MutexType mutexType)
{
    UINT32 HwNum = SECURE_NOTIFY_IRQ_NUM;
    UINT32 *pHwiSetPendingBase = ( UINT32 *)(GIC_ID_PHY_BASE + GICD_PENDINGSET);
    bit_set_by_offset_base(pHwiSetPendingBase, HwNum, 1);
    return SOS_OK;
}

SINT32 SOS_WaitWaitQue(MutexType mutexType, SINT32 waitTimeInMs)
{
    return SOS_OK;
}

/************************************************************************/
/* SOS_SpinLockInit(): defined in secure OS                             */
/************************************************************************/
VOID SOS_SpinLockInit(SpinLockType spinLockType)
{
    SOS_IRQ_SPIN_LOCK *pSosSpinLock = NULL;

    pSosSpinLock = GetSpinLockByEnum(spinLockType);

    if (pSosSpinLock != NULL)
    {
        memset(pSosSpinLock, 0, sizeof(SOS_IRQ_SPIN_LOCK));
    }
}

/************************************************************************/
/* SOS_SpinLock(): defined in secure OS                             */
/************************************************************************/
SINT32 SOS_SpinLock(SpinLockType spinLockType)
{
    SOS_IRQ_SPIN_LOCK *pSosSpinLock = NULL;

    pSosSpinLock = GetSpinLockByEnum(spinLockType);

    if (pSosSpinLock != NULL)
    {
        pSosSpinLock->irq_lock = SRE_IntLock();
        return SOS_OK;
    }
    else
    {
        return SOS_ERR;
    }
}

/************************************************************************/
/* SOS_SpinUnLock(): defined in secure OS                             */
/************************************************************************/
SINT32 SOS_SpinUnLock(SpinLockType spinLockType)
{
    SOS_IRQ_SPIN_LOCK *pSosSpinLock = NULL;

    pSosSpinLock = GetSpinLockByEnum(spinLockType);

    if (pSosSpinLock != NULL)
    {
        SRE_IntRestore(pSosSpinLock->irq_lock);
        return SOS_OK;
    }
    else
    {
        return SOS_ERR;
    }
}

/************************************************************************/
/* SOS_GetTimeInMs(): defined in secure OS                             */
/************************************************************************/
UINT32 SOS_GetTimeInMs(VOID)
{
	UINT32 CurrMs = 0;
    timeval_t CurrentTime;
    UINT64 tmp;

    tmp = SRE_ReadTimestamp();
    CurrentTime.tval.sec = (tmp>>32);
    CurrentTime.tval.nsec = tmp & 0xffffffff;
    CurrMs = (UINT32)(CurrentTime.tval.sec*1000 + CurrentTime.tval.nsec/1000000);

	return CurrMs;
}

UINT32 SOS_GetTimeInUs(VOID)
{
	UINT32 CurrUs = 0;
    timeval_t CurrentTime;
    UINT64 tmp;

    tmp = SRE_ReadTimestamp();
    CurrentTime.tval.sec = (tmp>>32);
    CurrentTime.tval.nsec = tmp & 0xffffffff;
    CurrUs = (UINT32)(CurrentTime.tval.sec*1000000 + CurrentTime.tval.nsec/1000);

	return CurrUs;
}

/************************************************************************/
/* LOCK & SEMA                                                          */
/************************************************************************/
SINT32 SOS_SpinLockIRQ( SOS_IRQ_SPIN_LOCK *pIntrMutex )
{
    pIntrMutex->irq_lock = SRE_IntLock();
	return SOS_OK;
}

SINT32 SOS_SpinUnLockIRQ( SOS_IRQ_SPIN_LOCK *pIntrMutex )
{
    SRE_IntRestore(pIntrMutex->irq_lock);
	return SOS_OK;
}

VOID SOS_SEMA_INTIT(SemType semType,int chanID)
{
    //SRE_SemCreate(0, &g_stSem);
	return;
}

SINT32 SOS_DOWN_INTERRUPTIBLE(SemType semType,int chanID)
{
    //UINT32 s32Ret;
	//s32Ret = SRE_SemPend(g_stSem, 0xffffffff);
	return SOS_OK;
}

VOID SOS_UP(SemType semType,int chanID)
{
    //SRE_SemPost(g_stSem);
	return;
}

/************************************************************************/
/* Map/Unmap to Sec 仅用作非安全和安全接口交互使用                      */
/* SecMode  0-映射安全内存  1-映射非安全内存                       ******/
/***************************start****************************************/
SINT32 SOS_Mmap(UINT32 phy, SINT32 size, UINT32 SecMode, UINT32 IsCached)
{
    SINT32 ret = 0;
    UINT32 va_addr;
    ret = sre_mmap(phy, size, &va_addr, SecMode, IsCached);

    if (ret != 0)
    {
        dprint(PRN_FATAL, "%s %d, map_section_entry failed ret=%d\n", __func__,__LINE__,ret);
        return NULL;
    }

    return va_addr;
}

SINT32 SOS_Unmap(UINT32 vir, SINT32 size)
{
    SINT32 ret = 0;
    ret = sre_unmap(vir, size);

    if (ret != 0)
    {
        dprint(PRN_FATAL, "%s %d, unmap_section_entry failed ret=%d\n", __func__,__LINE__,ret);
    }

    return ret;
}
/***************************end****************************************/

/************************************************************************/
/* Secure IRQ                                                           */
/************************************************************************/
SINT32 SOS_Request_Irq(UINT32 IrqNum, HWI_PROC_FUNC pfnHandler, SINT32 flags, SINT8 *name, VOID *dev)
{
    UINT32 ret;

    ret = SRE_HwiCreate(IrqNum, 0xa0, 0x0, pfnHandler, flags);
    if (ret != 0)
    {
        dprint(PRN_FATAL, "SRE_HwiCreate failed, IrqNum=%d\n", IrqNum);
        return ret;
    }
    ret = SRE_HwiEnable(IrqNum);
    if (ret != 0)
    {
        dprint(PRN_FATAL, "SRE_HwiEnable failed, IrqNum=%d\n", IrqNum);
    }
    return ret;
}

SINT32 SOS_Delete_Irq(UINT32 IrqNum, VOID *dev)
{
    UINT32 ret;
    ret = SRE_HwiDelete(IrqNum);
    if (ret != 0)
    {
        dprint(PRN_FATAL, "SRE_HwiDelete failed, IrqNum=%d\n", IrqNum);
    }
    return ret;
}

/************************************************************************/
/* Secure MMZ Alloc/Release/Map/Unmap                                   */
/************************************************************************/
SINT32 SOS_Mem_Malloc(SINT8 * MemName, SINT32 len, SINT32 align, SINT32 IsCached, MEM_DESC_S *pMemDesc)
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuffer;

    memset(&stSMMUBuffer, 0, sizeof(TEE_SMMU_BUFFER_S));
    if(SOS_OK != DRV_TEE_SMMU_Alloc(MemName, len, align, &stSMMUBuffer))
    {
        memset(pMemDesc, 0, sizeof(MEM_DESC_S));
        return SOS_ERR;
    }

    if( 0 != stSMMUBuffer.u32StartSmmuAddr)
    {
        if(SOS_OK != DRV_TEE_SMMU_MapCpu(&stSMMUBuffer, IsCached))
        {
            memset(pMemDesc, 0, sizeof(MEM_DESC_S));
            return SOS_ERR;
        }
    }
    else
    {
        memset(pMemDesc, 0, sizeof(MEM_DESC_S));
        return SOS_ERR;
    }

    pMemDesc->MmuAddr = (UADDR)stSMMUBuffer.u32StartSmmuAddr;
    pMemDesc->VirAddr = PTR_UINT64(stSMMUBuffer.pu8StartVirAddr);
    pMemDesc->Length  = (SINT32)stSMMUBuffer.u32Size;
#else
    TEE_MMZ_BUFFER_S stMmzBuffer;

    memset(&stMmzBuffer, 0, sizeof(TEE_MMZ_BUFFER_S));
    if(SOS_OK != DRV_TEE_MMZ_Alloc(MemName, len, align, 1, &stMmzBuffer))
    {
        memset(pMemDesc, 0, sizeof(MEM_DESC_S));
        return SOS_ERR;
    }

    if( 0 != stMmzBuffer.u32StartPhyAddr )
    {
        if(SOS_OK != DRV_TEE_MMZ_MapCpu(&stMmzBuffer, IsCached))
        {
            memset(pMemDesc, 0, sizeof(MEM_DESC_S));
            return SOS_ERR;
        }
    }
    else
    {
        memset(pMemDesc, 0, sizeof(MEM_DESC_S));
        return SOS_ERR;
    }

    pMemDesc->PhyAddr = (UADDR)stMmzBuffer.u32StartPhyAddr;
    pMemDesc->VirAddr = PTR_UINT64(stMmzBuffer.pu8StartVirAddr);
    pMemDesc->Length  = (SINT32)stMmzBuffer.u32Size;
#endif

    return SOS_OK;
}

UINT8 *SOS_Mem_Map(MEM_RECORD_S *pMemRec)
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuf;

    memset(&stSMMUBuf, 0, sizeof(TEE_SMMU_BUFFER_S));
    stSMMUBuf.u32StartSmmuAddr = pMemRec->PhyAddr;
    if(DRV_TEE_SMMU_MapCpu(&stSMMUBuf, 0) != HI_SUCCESS)
    {
        dprint(PRN_FATAL, "vfmw_osal.c, line %d: HI_DRV_SMMU_Map ERR\n", __LINE__);
    }
    return stSMMUBuf.pu8StartVirAddr;
#else
    TEE_MMZ_BUFFER_S stMemBuf;

    memset(&stMemBuf, 0, sizeof(TEE_MMZ_BUFFER_S));
    stMemBuf.u32StartPhyAddr = pMemRec->PhyAddr;
    if(DRV_TEE_MMZ_MapCpu(&stMemBuf) != HI_SUCCESS)
    {
        dprint(PRN_FATAL, "vfmw_osal.c, line %d: HI_DRV_MMZ_Map ERR\n", __LINE__);
    }
    return stMemBuf.pu8StartVirAddr;
#endif
}

SINT32 SOS_Mem_Free(MEM_DESC_S *pMemDesc)
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuf;

    memset(&stSMMUBuf, 0, sizeof(TEE_SMMU_BUFFER_S));
    stSMMUBuf.u32StartSmmuAddr = (UINT32)pMemDesc->PhyAddr;
    stSMMUBuf.pu8StartVirAddr  = UINT64_PTR(pMemDesc->VirAddr);
    stSMMUBuf.u32Size          = (UINT32)pMemDesc->Length;
    DRV_TEE_SMMU_UnmapCpu(&stSMMUBuf);
    DRV_TEE_SMMU_Free(&stSMMUBuf);
#else
    TEE_MMZ_BUFFER_S stMBuf;

    memset(&stMBuf, 0, sizeof(TEE_MMZ_BUFFER_S));
    stMBuf.u32StartPhyAddr  = (UINT32)pMemDesc->PhyAddr;
    stMBuf.pu8StartVirAddr  = UINT64_PTR(pMemDesc->VirAddr);
    stMBuf.u32Size          = (UINT32)pMemDesc->Length;
    DRV_TEE_MMZ_UnmapCpu(&stMBuf);
    DRV_TEE_MMZ_Free(&stMBuf);
#endif

    return SOS_OK;
}


UINT8 *SOS_Mem_MapCache(MEM_RECORD_S *pMemRec)
{
    return NULL;
}

VOID SOS_Mem_Unmap(UINT8 *p )
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuf;

    memset(&stSMMUBuf, 0, sizeof(TEE_SMMU_BUFFER_S));
    stSMMUBuf.pu8StartVirAddr = p;
    DRV_TEE_SMMU_UnmapCpu(&stSMMUBuf);
#else
    TEE_MMZ_BUFFER_S stMemBuf;

    memset(&stMemBuf, 0, sizeof(TEE_MMZ_BUFFER_S));
    stMemBuf.pu8StartVirAddr = p;
    DRV_TEE_MMZ_UnmapCpu(&stMemBuf);
#endif
}

/**********************************************/
/********Map/Unmap 非安全侧申请的内存**********/
/**********************************************/
UINT8 *SOS_NsMem_Map(UADDR phyaddr)
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuf;

    memset(&stSMMUBuf, 0, sizeof(TEE_SMMU_BUFFER_S));
    stSMMUBuf.u32StartSmmuAddr = phyaddr;
    if(DRV_TEE_NSSMMU_MapCpu(&stSMMUBuf, 0) != HI_SUCCESS)
    {
        dprint(PRN_FATAL, "vfmw_osal.c, line %d: HI_DRV_SMMU_Map ERR\n", __LINE__);
    }
    return stSMMUBuf.pu8StartVirAddr;
#else
    TEE_MMZ_BUFFER_S stMemBuf;

    memset(&stMemBuf, 0, sizeof(TEE_MMZ_BUFFER_S));
    stMemBuf.u32StartPhyAddr = phyaddr;
    if(DRV_TEE_NSMMZ_MapCpu(&stMemBuf) != HI_SUCCESS)
    {
        dprint(PRN_FATAL, "vfmw_osal.c, line %d: HI_DRV_MMZ_Map ERR\n", __LINE__);
    }
    return stMemBuf.pu8StartVirAddr;
#endif
}

VOID SOS_NsMem_Unmap(UINT8 *p)
{
#ifdef HI_SMMU_SUPPORT
    TEE_SMMU_BUFFER_S stSMMUBuf;

    memset(&stSMMUBuf, 0, sizeof(TEE_SMMU_BUFFER_S));
    stSMMUBuf.pu8StartVirAddr = p;
    DRV_TEE_NSSMMU_UmapCpu(&stSMMUBuf);
#else
    TEE_MMZ_BUFFER_S stMemBuf;

    memset(&stMemBuf, 0, sizeof(TEE_MMZ_BUFFER_S));
    stMemBuf.pu8StartVirAddr = p;
    DRV_TEE_NSMMZ_UmapCpu(&stMemBuf);
#endif
}

VOID SOS_GetPageTableAddr(UINT32 *pu32ptaddr, UINT32 *pu32err_rdaddr, UINT32 *pu32err_wraddr)
{
 #ifdef HI_SMMU_SUPPORT
    DRV_TEE_SMMU_GetPgtbladdr(pu32err_rdaddr, pu32err_wraddr, pu32ptaddr);
 #endif
}

SINT32 InitTrustedVfmwInterface(VOID)
{
    memset(&vfmw_Osal_Func_Ptr_S, 0, sizeof(Vfmw_Osal_Func_Ptr));

    vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInMs = SOS_GetTimeInMs;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_GetTimeInUs = SOS_GetTimeInUs;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLockInit = SOS_SpinLockInit;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinLock     = SOS_SpinLock;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SpinUnlock   = SOS_SpinUnLock;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaInit = SOS_SEMA_INTIT;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaDown = SOS_DOWN_INTERRUPTIBLE;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_SemaUp   = SOS_UP;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemSet = memset;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCpy = memcpy;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemMov = memmove;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_MemCmp = NULL;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_printk = SOS_printk;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_snprintf = snprintf;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_sscanf   = NULL;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_strncmp  = NULL;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_strlcpy_command_line = NULL;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_strlcpy       = NULL;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_strsep        = NULL;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_simple_strtol = NULL;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_strstr        = NULL;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_msleep = SOS_udelay; //SRE_DelayMs;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_mb     = SOS_Mb;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_udelay = SOS_udelay;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_InitEvent = SOS_InitWaitQue;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_GiveEvent = SOS_WakeupWaitQue;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_WaitEvent = SOS_WaitWaitQue;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMemAlloc = SOS_Mem_Malloc;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMemFree  = SOS_Mem_Free;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelRegisterMap   = SOS_Register_Map;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelRegisterUnMap = SOS_Register_UnMap;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMmap      = SOS_Mem_Map;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMunMap    = SOS_Mem_Unmap;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelMmapCache = SOS_Mem_MapCache;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMmap   = SOS_NsMem_Map;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelNsMunMap = SOS_NsMem_Unmap;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_KernelGetPageTableAddr = SOS_GetPageTableAddr;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_CreateTask = SOS_CreateTask;

    vfmw_Osal_Func_Ptr_S.pfun_Osal_request_irq = SOS_Request_Irq;
    vfmw_Osal_Func_Ptr_S.pfun_Osal_free_irq    = SOS_Delete_Irq;

    return SOS_OK;
}

#endif
