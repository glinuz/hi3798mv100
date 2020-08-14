/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_mmz.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2006/05/17
  Description   :
  History       :
  1.Date        : 2006/05/17
    Author      : g45345
    Modification: Created file

******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "mpi_mmz.h"

#include "hi_drv_mem.h"
#include "drv_mmz_ioctl.h"

#define MEM_LOCK_DECLARE(p_mutex)   static pthread_mutex_t p_mutex = PTHREAD_MUTEX_INITIALIZER
#define MEM_LOCK_DESTROY(p_mutex)   (void)pthread_mutex_destroy(p_mutex)
#define MEM_LOCK(p_mutex)           (void)pthread_mutex_lock(p_mutex)
#define MEM_UNLOCK(p_mutex)         (void)pthread_mutex_unlock(p_mutex)

#define MMZ_DEVNAME     "/dev/mmz_userdev"

#define SMMU_FLAG       "iommu"

static HI_S32 g_Mmzfd = -1;

MEM_LOCK_DECLARE(g_mem_mutex);

static HI_S32 MEMDeviceCheckOpen(HI_VOID)
{
    MEM_LOCK(&g_mem_mutex);

    if (-1 == g_Mmzfd)
    {
        g_Mmzfd = open(MMZ_DEVNAME, O_RDWR);
        if (-1 == g_Mmzfd)
        {
            MEM_UNLOCK(&g_mem_mutex);
            HI_FATAL_MEM("Open mem device failed\n");
            return HI_FAILURE;
        }
    }

    MEM_UNLOCK(&g_mem_mutex);

    return HI_SUCCESS;
}

#if 0
static HI_S32 MEMDeviceCheckClose(HI_VOID)
{
    MEM_LOCK(&g_mem_mutex);

    if (-1 != g_Mmzfd)
    {
        (HI_VOID)close(g_Mmzfd);
        g_Mmzfd = -1;
    }

    MEM_UNLOCK(&g_mem_mutex);

    MEM_LOCK_DESTROY(&g_mem_mutex);

    return HI_SUCCESS;
}
#endif

#define CHECK_MEM_OPEN_STATE(result) \
    do { \
        if (HI_SUCCESS != MEMDeviceCheckOpen()) \
            return (result) ? HI_FAILURE : HI_NULL; \
    } while (0)

/*****************************************************************************
 Prototype    : HI_MPI_MMZ_Malloc
 Description  : ...
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 HI_MPI_MMZ_Malloc(HI_MMZ_BUF_S *pstBuf)
{
    HI_S32 ret;
    struct mmb_info mmi = {0};

    if (NULL == pstBuf)
    {
        HI_WARN_MEM("invalid param\n");
        return HI_FAILURE;
    }

    if (strlen(pstBuf->bufname) >= MAX_BUFFER_NAME_SIZE)
    {
        HI_WARN_MEM("the buffer name len is overflow!\n");
        return HI_FAILURE;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.size = pstBuf->bufsize;
    mmi.align = HI_MEM_ALGIN_SIZE;
    strncpy(mmi.mmb_name, pstBuf->bufname, HIL_MAX_NAME_LEN);
    strncpy(mmi.mmz_name, "", HIL_MAX_NAME_LEN);

    ret = ioctl(g_Mmzfd, IOC_MMB_ALLOC, &mmi);
    if (0 != ret)
    {
        return HI_FAILURE;
    }

    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;

    ret = ioctl(g_Mmzfd, IOC_MMB_USER_REMAP, &mmi);
    if (0 != ret)
    {
        ioctl(g_Mmzfd, IOC_MMB_FREE, &mmi);
        return HI_FAILURE;
    }

    pstBuf->phyaddr = mmi.phys_addr;
    pstBuf->user_viraddr = (HI_U8 *)mmi.mapped;
    pstBuf->overflow_threshold = 100;
    pstBuf->underflow_threshold = 0;

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype    : HI_MPI_MMZ_Free
 Description  : ...
 Input        : None
 Output       : None
 Return Value :
*****************************************************************************/
HI_S32 HI_MPI_MMZ_Free(HI_MMZ_BUF_S *pstBuf)
{
    HI_S32 ret;
    struct mmb_info mmi = {0};

    if (NULL == pstBuf)
    {
        HI_WARN_MEM("invalid param\n");
        return HI_FAILURE;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.phys_addr = pstBuf->phyaddr;
    mmi.mapped = (void *)pstBuf->user_viraddr;

    ret = ioctl(g_Mmzfd, IOC_MMB_USER_UNMAP, &mmi);
    if (0 != ret)
    {
        return HI_FAILURE;
    }

    ret = ioctl(g_Mmzfd, IOC_MMB_FREE, &mmi);
    if (0 != ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_U32 HI_MPI_MMZ_New(HI_U32 u32Size , HI_U32 u32Align, HI_CHAR *ps8MMZName, HI_CHAR *ps8BufName)
{
    HI_S32 ret;
    struct mmb_info mmi = {0};

    if (0 == u32Size)
    {
        HI_WARN_MEM("invalid param\n");
        return HI_INVALID_MEM_ADDR;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.size = u32Size;
    mmi.align = u32Align;

    if (ps8BufName)
    {
        strncpy(mmi.mmb_name, ps8BufName, sizeof(mmi.mmb_name) - 1);
    }

    if (ps8MMZName)
    {
        strncpy(mmi.mmz_name, ps8MMZName, sizeof(mmi.mmz_name) - 1);
    }

    ret = ioctl(g_Mmzfd, IOC_MMB_ALLOC, &mmi);
    if (ret != 0)
    {
        return HI_INVALID_MEM_ADDR;
    }

    return mmi.phys_addr;
}

HI_S32 HI_MPI_MMZ_Delete(HI_U32 u32PhysAddr)
{
    struct mmb_info mmi = {0};

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.phys_addr = u32PhysAddr;

    return ioctl(g_Mmzfd, IOC_MMB_FREE, &mmi);
}

HI_S32 HI_MPI_MMZ_GetPhyAddr(HI_VOID *pRefAddr, HI_U32 *pu32PhyAddr, HI_U32 *pu32Size)
{
    int ret;
    struct mmb_info mmi = {0};

    if (!pRefAddr || !pu32PhyAddr || !pu32Size)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.mapped = pRefAddr;

    ret = ioctl(g_Mmzfd, IOC_MMB_USER_GETPHYADDR, &mmi);
    if (ret)
    {
        return -1;
    }

    *pu32PhyAddr = mmi.phys_addr;
    *pu32Size = mmi.size;

    return 0;
}

HI_VOID *HI_MPI_MMZ_Map(HI_U32 u32PhysAddr, HI_BOOL bCached)
{
    HI_S32 ret;
    struct mmb_info mmi = {0};

    CHECK_MEM_OPEN_STATE(HI_FALSE);

    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
    mmi.phys_addr = u32PhysAddr;

    ret = ioctl(g_Mmzfd, bCached ? IOC_MMB_USER_REMAP_CACHED : IOC_MMB_USER_REMAP, &mmi);
    if (0 != ret)
    {
        return HI_NULL;
    }

    return (HI_VOID *)mmi.mapped;
}

HI_S32 HI_MPI_MMZ_Unmap(HI_U32 u32PhysAddr)
{
    struct mmb_info mmi = {0};

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    mmi.phys_addr = u32PhysAddr;

    return ioctl(g_Mmzfd, IOC_MMB_USER_UNMAP, &mmi);
}

HI_S32 HI_MPI_MMZ_Flush(HI_U32 u32PhysAddr)
{
    CHECK_MEM_OPEN_STATE(HI_TRUE);

    if (HI_INVALID_MEM_ADDR != u32PhysAddr)
    {
        struct mmb_info mmi;

        memset(&mmi, 0, sizeof(struct mmb_info));

        mmi.phys_addr = u32PhysAddr;

        return ioctl(g_Mmzfd, IOC_MMB_FLUSH_DCACHE, &mmi);
    }

    return ioctl(g_Mmzfd, IOC_MMB_FLUSH_DCACHE, HI_NULL);
}

#ifdef HI_SMMU_SUPPORT
HI_U32 HI_MPI_SMMU_New(HI_CHAR *pBufName, HI_U32 Size)
{
    HI_S32 ret;
    struct mmb_info mmi;

    if (!pBufName || (0 == Size))
    {
        HI_WARN_MEM("invalid param\n");
        return HI_INVALID_MEM_ADDR;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    strncpy(mmi.mmb_name, pBufName, sizeof(mmi.mmb_name) - 1);
    strncpy(mmi.mmz_name, SMMU_FLAG, sizeof(mmi.mmz_name) - 1);

    mmi.size = Size;
    mmi.align = HI_MEM_ALGIN_SIZE;

    ret = ioctl(g_Mmzfd, IOC_MMB_ALLOC, &mmi);
    if (0 != ret)
    {
        return HI_INVALID_MEM_ADDR;
    }

    return mmi.smmu_addr;
}

HI_S32 HI_MPI_SMMU_Delete(HI_U32 SmmuAddr)
{
    HI_S32 ret;
    struct mmb_info mmi;

    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        HI_WARN_MEM("invalid param\n");
        return HI_FAILURE;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.smmu_addr = SmmuAddr;

    ret = ioctl(g_Mmzfd, IOC_MMB_FREE, &mmi);
    if (0 != ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_SMMU_GetSmmuAddrByMapAddr(HI_VOID *pMapAddr, HI_U32 *pSmmuAddr, HI_U32 *pSize)
{
    struct mmb_info mmi;

    if (!pMapAddr || !pSmmuAddr || !pSize)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.mapped = pMapAddr;

    if (0 == ioctl(g_Mmzfd, IOC_MMB_USER_GETPHYADDR, &mmi))
    {
        if (HI_INVALID_MEM_ADDR != mmi.smmu_addr)
        {
            *pSmmuAddr = mmi.smmu_addr;
            *pSize = mmi.size;

            return 0;
        }
    }

    return -1;
}

HI_VOID *HI_MPI_SMMU_Map(HI_U32 SmmuAddr, HI_BOOL bCached)
{
    HI_S32 ret;
    struct mmb_info mmi;

    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        HI_WARN_MEM("invalid param\n");
        return HI_NULL;
    }

    CHECK_MEM_OPEN_STATE(HI_FALSE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.prot = PROT_READ | PROT_WRITE;
    mmi.flags = MAP_SHARED;
    mmi.smmu_addr = SmmuAddr;

    if (bCached)
    {
        ret = ioctl(g_Mmzfd, IOC_MMB_USER_REMAP_CACHED, &mmi);
        if (0 != ret)
        {
            return HI_NULL;
        }
    }
    else
    {
        ret = ioctl(g_Mmzfd, IOC_MMB_USER_REMAP, &mmi);
        if (0 != ret)
        {
            return HI_NULL;
        }
    }

    return (HI_VOID*)mmi.mapped;
}

HI_S32 HI_MPI_SMMU_Unmap(HI_U32 SmmuAddr)
{
    struct mmb_info mmi;

    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.smmu_addr = SmmuAddr;

    return ioctl(g_Mmzfd, IOC_MMB_USER_UNMAP, &mmi);
}

HI_S32 HI_MPI_SMMU_Flush(HI_U32 SmmuAddr)
{
    CHECK_MEM_OPEN_STATE(HI_TRUE);

    if (HI_INVALID_MEM_ADDR != SmmuAddr)
    {
        struct mmb_info mmi;

        memset(&mmi, 0, sizeof(struct mmb_info));

        mmi.smmu_addr = SmmuAddr;

        return ioctl(g_Mmzfd, IOC_MMB_FLUSH_DCACHE, &mmi);
    }

    return ioctl(g_Mmzfd, IOC_MMB_FLUSH_DCACHE, HI_NULL);
}

HI_S32 HI_MPI_SMMU_MapPhytoSmmu(HI_U32 PhyAddr, HI_U32 *pSmmuAddr)
{
    HI_S32 ret;
    struct mmb_info mmi;

    if ((HI_INVALID_MEM_ADDR == PhyAddr) || !pSmmuAddr)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.phys_addr = PhyAddr;

    ret = ioctl(g_Mmzfd, IOC_MMB_USER_CMA_MAPTO_SMMU, &mmi);
    if (0 != ret)
    {
        return -1;
    }

    *pSmmuAddr = mmi.smmu_addr;

    return 0;
}

HI_S32 HI_MPI_SMMU_UnmapPhyfromSmmu(HI_U32 SmmuAddr)
{
    struct mmb_info mmi;

    if (HI_INVALID_MEM_ADDR == SmmuAddr)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&mmi, 0, sizeof(struct mmb_info));

    mmi.smmu_addr = SmmuAddr;

    return ioctl(g_Mmzfd, IOC_MMB_USER_CMA_UNMAPTO_SMMU, &mmi);
}

#ifdef HI_TEE_SUPPORT
HI_S32 HI_MPI_SMMU_SecMemAlloc(const HI_MPI_SMMU_SEC_ATTR_S *pSecAttr, HI_MPI_SMMU_SEC_BUF_S *pSecBuf)
{
    HI_S32 ret;
    struct sec_info info;

    if (!pSecAttr || !pSecBuf)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&info, 0, sizeof(struct sec_info));

    strncpy(info.mmb_name, pSecAttr->bufname, sizeof(info.mmb_name) - 1);
    strncpy(info.mmz_name, SMMU_FLAG, sizeof(info.mmz_name) - 1);

    info.size       = pSecAttr->bufsize;
    info.align      = HI_MEM_ALGIN_SIZE;
  //  info.authflag   = (HI_U32)pSecAttr->auth_flag;
 //   info.secmapflag = (HI_U32)pSecAttr->sec_map_flag;

    ret = ioctl(g_Mmzfd, IOC_MMB_SECSMMU_ALLOC, &info);
    if (0 == ret)
    {
        pSecBuf->smmu_addr      = info.sec_smmu;
        pSecBuf->smmu_size      = info.size;
   //     pSecBuf->auth_flag      = info.authflag;
    //    pSecBuf->sec_map_flag   = info.secmapflag;
   //     pSecBuf->sec_virt       = info.sec_virt;
   //     pSecBuf->fixed_buf_phy  = info.buf_phy;
   //     pSecBuf->fixed_buf_size = info.buf_size;
    }

    return ret;
}

HI_S32 HI_MPI_SMMU_SecMemFree(HI_MPI_SMMU_SEC_BUF_S *pSecBuf)
{
    struct sec_info info;

    if (!pSecBuf)
    {
        HI_WARN_MEM("invalid param\n");
        return -1;
    }

    CHECK_MEM_OPEN_STATE(HI_TRUE);

    memset(&info, 0, sizeof(struct sec_info));

    info.sec_smmu  = pSecBuf->smmu_addr;
    info.size       = pSecBuf->smmu_size;
//    info.authflag   = pSecBuf->auth_flag;
//    info.secmapflag = pSecBuf->sec_map_flag;
//    info.sec_virt   = pSecBuf->sec_virt;
//    info.buf_phy    = pSecBuf->fixed_buf_phy;
  //  info.buf_size   = pSecBuf->fixed_buf_size;

    return ioctl(g_Mmzfd, IOC_MMB_SECSMMU_FREE, &info);
}
#endif  // HI_TEE_SUPPORT
#endif  // HI_SMMU_SUPPORT

