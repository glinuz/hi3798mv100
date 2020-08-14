/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    task.c
 *
 * Purpose: omxvdec task functions
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#include "omxvdec.h"
#include "omxvdec_mem.h"


/*============== EXPORT FUNCTION =============*/

HI_S32 HI_DRV_OMXVDEC_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(stMMZBuf));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(stSMMUBuf));
#endif

    OMXVDEC_ASSERT_RETURN(psMBuf !=  HI_NULL, "invalid param");

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, size, align, &stMMZBuf);

    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
    psMBuf->u32Size          = stMMZBuf.u32Size;
#else
    s32Ret = HI_DRV_SMMU_AllocAndMap(bufname, size, align, &stSMMUBuf);

    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
    psMBuf->u32Size          = stSMMUBuf.u32Size;
#endif

    return s32Ret;
}

HI_VOID HI_DRV_OMXVDEC_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(stMMZBuf));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(stSMMUBuf));
#endif

    OMXVDEC_ASSERT_RETURN_NULL(psMBuf !=  HI_NULL, "invalid param");

#ifndef HI_SMMU_SUPPORT
    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;

    HI_DRV_MMZ_UnmapAndRelease(&stMMZBuf);
#else
    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;

    HI_DRV_SMMU_UnmapAndRelease(&stSMMUBuf);
#endif
    return;
}


HI_S32 HI_DRV_OMXVDEC_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf, HI_BOOL is_sec)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(stMMZBuf));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(stSMMUBuf));
#endif

    OMXVDEC_ASSERT_RETURN(psMBuf !=  HI_NULL, "invalid param");

#ifndef HI_SMMU_SUPPORT
#ifdef HI_OMX_TEE_SUPPORT
    if (is_sec)
    {
        s32Ret = HI_DRV_SECMMZ_Alloc(bufname, size, align, &stMMZBuf);
    }
    else
#endif
    {
        s32Ret = HI_DRV_MMZ_Alloc(bufname, zone_name, size, align, &stMMZBuf);
    }
    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
    psMBuf->u32Size          = stMMZBuf.u32Size;
#else

    if (is_sec)
    {
        s32Ret = HI_DRV_SECSMMU_Alloc(bufname, size, align, &stSMMUBuf);
    }
    else
    {
        s32Ret = HI_DRV_SMMU_Alloc(bufname, size, align, &stSMMUBuf);
    }

    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
    psMBuf->u32Size          = stSMMUBuf.u32Size;
#endif

    OmxPrint(OMX_INFO, "%s %s phy 0x%x size 0x%x secure:%d\n", __func__, bufname, psMBuf->u32StartPhyAddr, psMBuf->u32Size, is_sec);

    return s32Ret;
}


HI_VOID HI_DRV_OMXVDEC_Release(MMZ_BUFFER_S *psMBuf, HI_BOOL is_sec)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(stMMZBuf));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(stSMMUBuf));
#endif

    OMXVDEC_ASSERT_RETURN_NULL(psMBuf !=  HI_NULL, "invalid param");

#ifndef HI_SMMU_SUPPORT

    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;

#ifdef HI_OMX_TEE_SUPPORT
    if (is_sec)
    {
        HI_DRV_SECMMZ_Release(&stMMZBuf);
    }
    else
#endif
    {
        HI_DRV_MMZ_Release(&stMMZBuf);
    }
#else

    stSMMUBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size         = psMBuf->u32Size;

    if (is_sec)
    {
        HI_DRV_SECSMMU_Release(&stSMMUBuf);
    }
    else
    {
        HI_DRV_SMMU_Release(&stSMMUBuf);
    }

#endif

    OmxPrint(OMX_INFO, "%s phy 0x%x size 0x%x secure:%d\n", __func__, psMBuf->u32StartPhyAddr, psMBuf->u32Size, is_sec);
}


HI_S32 HI_DRV_OMXVDEC_Map(MMZ_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;

    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
#else
    SMMU_BUFFER_S stSMMUBuf;

    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
#endif

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_MMZ_Map(&stMMZBuf);

    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
#else
    s32Ret = HI_DRV_SMMU_Map(&stSMMUBuf);

    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
#endif

    return s32Ret;
}

HI_VOID HI_DRV_OMXVDEC_Unmap(MMZ_BUFFER_S *psMBuf)
{
#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;
    memset(&stMMZBuf, 0, sizeof(stMMZBuf));
#else
    SMMU_BUFFER_S stSMMUBuf;
    memset(&stSMMUBuf, 0, sizeof(stSMMUBuf));
#endif

    OMXVDEC_ASSERT_RETURN_NULL(psMBuf !=  HI_NULL, "invalid param");

#ifndef HI_SMMU_SUPPORT

    stMMZBuf.pu8StartVirAddr = psMBuf->pu8StartVirAddr;
    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;

    HI_DRV_MMZ_Unmap(&stMMZBuf);
#else

    stSMMUBuf.pu8StartVirAddr  = psMBuf->pu8StartVirAddr;
    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;

    HI_DRV_SMMU_Unmap(&stSMMUBuf);
#endif
}


HI_S32 HI_DRV_OMXVDEC_MapCache(MMZ_BUFFER_S *psMBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifndef HI_SMMU_SUPPORT
    MMZ_BUFFER_S stMMZBuf;

    stMMZBuf.u32StartPhyAddr = psMBuf->u32StartPhyAddr;
    stMMZBuf.u32Size         = psMBuf->u32Size;
#else
    SMMU_BUFFER_S stSMMUBuf;

    stSMMUBuf.u32StartSmmuAddr = psMBuf->u32StartPhyAddr;
    stSMMUBuf.u32Size          = psMBuf->u32Size;
#endif

#ifndef HI_SMMU_SUPPORT
    s32Ret = HI_DRV_MMZ_MapCache(&stMMZBuf);

    psMBuf->pu8StartVirAddr  = stMMZBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stMMZBuf.u32StartPhyAddr;
    psMBuf->u32Size          = stMMZBuf.u32Size;
#else
    s32Ret = HI_DRV_SMMU_MapCache(&stSMMUBuf);

    psMBuf->pu8StartVirAddr  = stSMMUBuf.pu8StartVirAddr;
    psMBuf->u32StartPhyAddr  = stSMMUBuf.u32StartSmmuAddr;
    psMBuf->u32Size          = stSMMUBuf.u32Size;
#endif

    return s32Ret;
}

/*********************************** for SMMU end ************************************/



