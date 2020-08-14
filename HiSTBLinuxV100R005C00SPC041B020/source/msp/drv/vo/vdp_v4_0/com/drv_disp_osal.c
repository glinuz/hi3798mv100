
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

#ifdef HI_SMMU_SUPPORT
HI_U32 DISP_OS_SmmuToPhyAddr( HI_U32 u32SmmuAddr)
{
#ifndef __DISP_PLATFORM_BOOT__

    HI_U32 PageIndex = 0;
    HI_U32 PageOffset = 0;
    HI_U32 PhyAddr = 0;
    HI_U32  *pPage;
    HI_U32 Hige20Bit,Low12Bit;
    HI_U32 PageTabBaseAddr;
    HI_U32 u32err_rdaddr;
    HI_U32 u32err_wraddr;

    HI_DRV_SMMU_GetPageTableAddr(&PageTabBaseAddr,&u32err_rdaddr, &u32err_wraddr);

    //DISP_PRINT("%s,%d,PageTabBaseAddr = 0x%x,smmuAddr = 0x%x\n",__func__,__LINE__,PageTabBaseAddr,u32SmmuAddr);

    if (PageTabBaseAddr == 0)
    {
        DISP_FATAL("%s,%d, pageTabBaseAddr = %d Error!\n",__func__,__LINE__,PageTabBaseAddr);
    }

    PageOffset = u32SmmuAddr / 4096 * 4;

    PageIndex = PageTabBaseAddr + PageOffset;
    //HI_INFO_DISP("%s,%d,PageIndex = 0x%x\n",__func__,__LINE__,PageIndex);

    pPage = (HI_U32 *)phys_to_virt(PageIndex);//(HI_U32 *)ioremap(PageIndex, 4);

    Hige20Bit = ((*pPage) & 0xfffff000);
    //HI_INFO_DISP("%s,%d,Hige20Bit = 0x%x(0x%x)\n",__func__,__LINE__,Hige20Bit,*pPage);

    Low12Bit = u32SmmuAddr & 0xfff;
    //HI_INFO_DISP("%s,%d,Low12Bit = 0x%x\n",__func__,__LINE__,Low12Bit);

    PhyAddr = Hige20Bit + Low12Bit;

    //DISP_PRINT("\n%s,%d,~ PhyAddr = 0x%x\n",__func__,__LINE__,PhyAddr);
    return PhyAddr;
#else
    return 0;
#endif

}
#endif

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
            pstMBuf->bSecure= HI_FALSE;
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
            pstMBuf->bSecure= HI_FALSE;
        }
    }
#endif
    //printk("mmz addr=0x%x, size=%d, nRet = 0x%x\n", stMMZ.u32StartPhyAddr, stMMZ.u32Size, nRet);

    return Ret;
#else
    return HI_FAILURE;
#endif
}

#if 0
HI_U32  DISP_OS_IORemap( HI_U32 u32PhyAddr,HI_U32 u32Size)
{
#ifdef __DISP_PLATFORM_SDK__

    return  ioremap(u32PhyAddr,u32Size);

#else
    return u32PhyAddr;
#endif
}
#endif

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



HI_S32  DISP_OS_SECSMMU_Alloc(const char *bufname, HI_U32 size, int align, DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__
    HI_S32      Ret = HI_FAILURE;
    SMMU_BUFFER_S SmmuBuf;

    Ret = HI_DRV_SECSMMU_Alloc(bufname, size, align, &SmmuBuf);
    if (HI_SUCCESS == Ret)
    {
        pstMBuf->pu8StartVirAddr = SmmuBuf.pu8StartVirAddr;
        pstMBuf->u32StartPhyAddr = SmmuBuf.u32StartSmmuAddr;
        pstMBuf->u32Size = SmmuBuf.u32Size;
        pstMBuf->bSmmu = HI_TRUE;
        pstMBuf->bSecure= HI_TRUE;
    }

    return Ret;
#else
    return HI_SUCCESS;
#endif
}

HI_VOID DISP_OS_SECSMMU_Release(DISP_MMZ_BUF_S *pstMBuf)
{
#ifdef __DISP_PLATFORM_SDK__

    SMMU_BUFFER_S SmmuBuf = {0};

    SmmuBuf.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
    SmmuBuf.u32StartSmmuAddr = pstMBuf->u32StartPhyAddr;
    SmmuBuf.u32Size = pstMBuf->u32Size;

    HI_DRV_SECSMMU_Release(&SmmuBuf);
#endif

    return ;
}



#ifdef HI_MINIBOOT_SUPPORT
extern uint64 __udivmoddi4(uint64 a, uint64 b, uint64* rem);
HI_VOID DISP_OS_DIV64(HI_U64 numerator, HI_U32 denominator, HI_U64 *pu64Result)
{
    HI_U64  rem = 0;

    *pu64Result = __udivmoddi4(numerator, denominator, &rem);

    return;
}

#else
HI_VOID DISP_OS_DIV64(HI_U64 numerator, HI_U32 denominator, HI_U64 *pu64Result)
{
    (HI_VOID)do_div(numerator, denominator);
    *pu64Result = numerator;
    return;
}
#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */




