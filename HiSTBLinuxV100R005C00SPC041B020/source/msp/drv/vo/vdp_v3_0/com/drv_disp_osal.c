
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_osal.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "drv_disp_osal.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


HI_VOID DISP_OS_GetTime(HI_U32 *t_ms)
{
#ifdef __DISP_PLATFORM_SDK__

    HI_DRV_SYS_GetTimeStampMs(t_ms);
    return;
#else

    return;
#endif
}

HI_S32  DISP_OS_MMZ_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    HI_S32      Ret = HI_FAILURE;
    HI_BOOL     Smmu = HI_FALSE;

#ifdef CFG_VDP_MMU_SUPPORT
    if (zone_name)
    {
        if (0 == strncmp(zone_name, "iommu", strlen("iommu")))
        {
            Smmu = HI_TRUE;
        }
    }
#endif

    if (!Smmu)
    {
        MMZ_BUFFER_S MmzBuf;

        Ret = HI_DRV_MMZ_Alloc(bufname, zone_name, size, align, &MmzBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = MmzBuf.pu8StartVirAddr;
            pstMBuf->u32StartPhyAddr = MmzBuf.u32StartPhyAddr;
            pstMBuf->u32Size = MmzBuf.u32Size;
            pstMBuf->bSmmu = HI_FALSE;
        }
    }
#ifdef CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf;

        Ret = HI_DRV_SMMU_Alloc(bufname, size, align, &SmmuBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = SmmuBuf.pu8StartVirAddr;
            pstMBuf->u32StartPhyAddr = SmmuBuf.u32StartSmmuAddr;
            pstMBuf->u32Size = SmmuBuf.u32Size;
            pstMBuf->bSmmu = HI_TRUE;
        }
    }
#endif
    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return Ret;
#else
    return HI_FAILURE;
#endif
}

HI_S32  DISP_OS_MMZ_Map(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    HI_S32 Ret = HI_FAILURE;

    if (!pstMBuf->bSmmu)
    {
        MMZ_BUFFER_S MmzBuf = {0};

        MmzBuf.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
        MmzBuf.u32Size = pstMBuf->u32Size;

        Ret = HI_DRV_MMZ_Map(&MmzBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = MmzBuf.pu8StartVirAddr;
        }
    }
#ifdef  CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf = {0};

        SmmuBuf.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
        SmmuBuf.u32Size = pstMBuf->u32Size;

        Ret = HI_DRV_SMMU_Map(&SmmuBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = SmmuBuf.pu8StartVirAddr;
        }
    }
#endif

    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return Ret;
#else

    return HI_FAILURE;
#endif
}

HI_S32  DISP_OS_MMZ_UnMap(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    if (!pstMBuf->bSmmu)
    {
        MMZ_BUFFER_S MmzBuf = {0};

        MmzBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        MmzBuf.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
        MmzBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_MMZ_Unmap(&MmzBuf);
    }
#ifdef  CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf = {0};

        SmmuBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        SmmuBuf.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
        SmmuBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_SMMU_Unmap(&SmmuBuf);
    }
#endif

    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return HI_SUCCESS;
#else

    return HI_FAILURE;
#endif
}

HI_VOID DISP_OS_MMZ_Release(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    if (!pstMBuf->bSmmu)
    {
        MMZ_BUFFER_S MmzBuf = {0};

        MmzBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        MmzBuf.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
        MmzBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_MMZ_Release(&MmzBuf);
    }
#ifdef  CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf = {0};

        SmmuBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        SmmuBuf.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
        SmmuBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_SMMU_Release(&SmmuBuf);
    }
#endif
    return;
#else
    return;
#endif
}

HI_S32 DISP_OS_MMZ_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    HI_S32      Ret = HI_FAILURE;
    HI_BOOL     Smmu = HI_FALSE;

#ifdef CFG_VDP_MMU_SUPPORT
    if (zone_name)
    {
        if (0 == strncmp(zone_name, "iommu", strlen("iommu")))
        {
            Smmu = HI_TRUE;
        }
    }
#endif

    if (!Smmu)
    {
        MMZ_BUFFER_S MmzBuf;

        Ret = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, size, align, &MmzBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = MmzBuf.pu8StartVirAddr;
            pstMBuf->u32StartPhyAddr = MmzBuf.u32StartPhyAddr;
            pstMBuf->u32Size = MmzBuf.u32Size;
            pstMBuf->bSmmu = HI_FALSE;
        }
    }
#ifdef CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf;

        Ret = HI_DRV_SMMU_AllocAndMap(bufname, size, align, &SmmuBuf);
        if (HI_SUCCESS == Ret)
        {
            pstMBuf->pu8StartVirAddr = SmmuBuf.pu8StartVirAddr;
            pstMBuf->u32StartPhyAddr = SmmuBuf.u32StartSmmuAddr;
            pstMBuf->u32Size = SmmuBuf.u32Size;
            pstMBuf->bSmmu = HI_TRUE;
        }
    }
#endif
    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return Ret;
#else
    return HI_FAILURE;
#endif
}

HI_VOID DISP_OS_MMZ_UnmapAndRelease(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    if (!pstMBuf->bSmmu)
    {
        MMZ_BUFFER_S MmzBuf;

        MmzBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        MmzBuf.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
        MmzBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_MMZ_UnmapAndRelease(&MmzBuf);
    }
#ifdef CFG_VDP_MMU_SUPPORT
    else
    {
        SMMU_BUFFER_S SmmuBuf;

        SmmuBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
        SmmuBuf.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
        SmmuBuf.u32Size = pstMBuf->u32Size;

        HI_DRV_SMMU_UnmapAndRelease(&SmmuBuf);
    }
#endif
    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return;
#else
    return;
#endif
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




