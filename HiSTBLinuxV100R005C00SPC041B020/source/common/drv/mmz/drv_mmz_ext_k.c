/**
\file
\brief mmz ops
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author QuYaxin 46153
\date 2008-12-25
*/
/* add include here */
#include <linux/types.h>
#include <linux/smp.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "drv_mmz.h"
#include "hi_drv_mmz.h"
#include "drv_tzsmmu.h"

#define HI_ZERO	0

HI_S32 HI_DRV_MMZ_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;

	phyaddr = new_mmb(bufname, size, align, NULL);
	if (phyaddr == MMB_ADDR_INVALID) {
		HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
		goto err;
	}

	psMBuf->pu8StartVirAddr = remap_mmb (phyaddr, 0);
	if (HI_NULL == psMBuf->pu8StartVirAddr) {
		delete_mmb(phyaddr, 0);
		HI_PRINT("[%s] Remap %s failed\n", __FUNCTION__, bufname);
		goto err;
	}

	psMBuf->u32StartPhyAddr = (HI_U32)phyaddr;
	psMBuf->u32Size = size;

	return HI_SUCCESS;

err:
	psMBuf->pu8StartVirAddr = HI_NULL;
	psMBuf->u32StartPhyAddr = (HI_U32)MMB_ADDR_INVALID;
	psMBuf->u32Size = 0;

	return HI_FAILURE;
}

HI_VOID HI_DRV_MMZ_UnmapAndRelease(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;

	phyaddr = psMBuf->u32StartPhyAddr;

	if (MMB_ADDR_INVALID != phyaddr ) {
		unmap_mmb((HI_VOID*)psMBuf->pu8StartVirAddr);
		delete_mmb(phyaddr, 0);
	}
}

HI_S32 HI_DRV_MMZ_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;
	char *mmz_name = zone_name;

	phyaddr = new_mmb(bufname, size, align, mmz_name);
	if (phyaddr == MMB_ADDR_INVALID) {
		phyaddr = new_mmb(bufname, size, align, NULL);
		if (phyaddr == MMB_ADDR_INVALID) {
			HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
			return HI_FAILURE;
		}
	}

	psMBuf->pu8StartVirAddr = HI_NULL;
	psMBuf->u32StartPhyAddr = (HI_U32)phyaddr;
	psMBuf->u32Size = size;

	return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_MapCache(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;

	phyaddr = psMBuf->u32StartPhyAddr;

	psMBuf->pu8StartVirAddr = remap_mmb_cached(phyaddr, 0);
	if (HI_NULL == psMBuf->pu8StartVirAddr) {
		psMBuf->pu8StartVirAddr = (HI_U8*) NULL;
		HI_PRINT("[%s] Remap buf failed\n", __FUNCTION__);

		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_Flush(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;
	HI_U8* viraddr;
	mmb_addr_t len;

	if (!psMBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	viraddr = psMBuf->pu8StartVirAddr;
	len = psMBuf->u32Size;

	if (MMB_ADDR_INVALID != psMBuf->u32StartPhyAddr) {
		phyaddr = psMBuf->u32StartPhyAddr;
	} else {
		HI_PRINT("[%s] invalid MMZ param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	flush_cache_kern(phyaddr, (void *)viraddr, len, 0);
	return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_Map(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;

	psMBuf->pu8StartVirAddr = remap_mmb(phyaddr, 0);
	if (HI_NULL == psMBuf->pu8StartVirAddr) {
		psMBuf->pu8StartVirAddr = HI_NULL;	
		HI_PRINT("[%s] Remap buf failed\n", __FUNCTION__);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_VOID HI_DRV_MMZ_Unmap(MMZ_BUFFER_S *psMBuf)
{
	unmap_mmb((HI_VOID*)psMBuf->pu8StartVirAddr);
}

HI_VOID HI_DRV_MMZ_Release(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t phyaddr;

	if (!psMBuf) {
		HI_PRINT("[%s] err args and free failed!\n", __func__);
		return;
	}

	phyaddr = psMBuf->u32StartPhyAddr;
	if (MMB_ADDR_INVALID != phyaddr) {
		delete_mmb(phyaddr, 0);
	}
}

HI_S32 HI_DRV_MMZ_GetPhyByVirt(MMZ_BUFFER_S *psMBuf)
{
	if (!psMBuf->pu8StartVirAddr) {
		HI_PRINT("%s: err arg\n",__func__);
		return HI_FAILURE;
	}

	psMBuf->u32StartPhyAddr = (HI_U32)get_phyaddr_byvirt((HI_VOID*)psMBuf->pu8StartVirAddr, 0);
	if ((MMB_ADDR_INVALID != psMBuf->u32StartPhyAddr) ||
				(0 != psMBuf->u32StartPhyAddr))
		return HI_SUCCESS;

	return HI_FAILURE;
}

#ifdef HI_SMMU_SUPPORT
HI_S32 HI_DRV_SMMU_AllocAndMap(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!bufname) {
		HI_PRINT("[%s] bufname is null\n", __FUNCTION__);
		return HI_FAILURE;
	}

	if (0 == size) {
		HI_PRINT("[%s] bufname=%s, size is 0\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	if (!pSmmuBuf) {
		HI_PRINT("[%s] bufname=%s, pSmmuBuf is null\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
	if (MMB_ADDR_INVALID == smmuaddr) {
		HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
		goto err;
	}

	pSmmuBuf->pu8StartVirAddr = remap_mmb(smmuaddr, 1);
	if (HI_NULL == pSmmuBuf->pu8StartVirAddr) {
		delete_mmb(smmuaddr, 1);
		HI_PRINT("[%s] Remap %s failed\n", __FUNCTION__, bufname);
		goto err;
	}

	pSmmuBuf->u32StartSmmuAddr = (HI_U32)smmuaddr;
	pSmmuBuf->u32Size = size;

	return HI_SUCCESS;

err:
	pSmmuBuf->u32Size = 0;
	pSmmuBuf->pu8StartVirAddr = HI_NULL;
	pSmmuBuf->u32StartSmmuAddr = (HI_U32)MMB_ADDR_INVALID;

	return HI_FAILURE;
}

HI_VOID HI_DRV_SMMU_UnmapAndRelease(SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return;
	}

	smmuaddr = (mmb_addr_t)pSmmuBuf->u32StartSmmuAddr;
	if (MMB_ADDR_INVALID != smmuaddr ) {
		unmap_mmb(pSmmuBuf->pu8StartVirAddr);
		delete_mmb(smmuaddr, 1);
	}
}

HI_S32 HI_DRV_SMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, int align, SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!bufname) {
		HI_PRINT("[%s] bufname is null\n", __FUNCTION__);
		return HI_FAILURE;
	}

	if (0 == size) {
		HI_PRINT("[%s] bufname=%s, size is 0\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	if (!pSmmuBuf) {
		HI_PRINT("[%s] bufname=%s, pSmmuBuf is null\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	pSmmuBuf->u32Size = 0;
	pSmmuBuf->pu8StartVirAddr = HI_NULL;
	pSmmuBuf->u32StartSmmuAddr = (HI_U32)MMB_ADDR_INVALID;

	smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
	if (smmuaddr == MMB_ADDR_INVALID) {
		HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	pSmmuBuf->pu8StartVirAddr = HI_NULL;
	pSmmuBuf->u32StartSmmuAddr = (HI_U32)smmuaddr;
	pSmmuBuf->u32Size = size;

	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_MapCache(SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	smmuaddr = pSmmuBuf->u32StartSmmuAddr;
	if (MMB_ADDR_INVALID == smmuaddr) {
		HI_PRINT("[%s] invalid smmuaddr\n", __FUNCTION__);
		return HI_FAILURE;
	}

	pSmmuBuf->pu8StartVirAddr = remap_mmb_cached(smmuaddr, 1);
	if (HI_NULL == pSmmuBuf->pu8StartVirAddr) {
		pSmmuBuf->pu8StartVirAddr = HI_NULL;

		HI_PRINT("[%s] Remap buf failed\n", __FUNCTION__);

		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_Flush(SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t Smmuaddr;
	HI_U8* viraddr;
	mmb_addr_t len;

	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	viraddr = pSmmuBuf->pu8StartVirAddr;
	len = pSmmuBuf->u32Size;

	if (MMB_ADDR_INVALID != pSmmuBuf->u32StartSmmuAddr) {
		Smmuaddr = pSmmuBuf->u32StartSmmuAddr;
	} else {
		HI_PRINT("[%s] invalid MMZ param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	flush_cache_kern(Smmuaddr, (void *)viraddr, len, 1);
	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_Map(SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return HI_FAILURE;
	}

	smmuaddr = pSmmuBuf->u32StartSmmuAddr;
	if (MMB_ADDR_INVALID == smmuaddr) {
		HI_PRINT("[%s] invalid smmuaddr\n", __FUNCTION__);
		return HI_FAILURE;
	}

	pSmmuBuf->pu8StartVirAddr = remap_mmb(smmuaddr, 1);
	if (HI_NULL == pSmmuBuf->pu8StartVirAddr) {
		pSmmuBuf->pu8StartVirAddr = HI_NULL;	
		HI_PRINT("[%s] Remap buf failed\n", __FUNCTION__);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_VOID HI_DRV_SMMU_Unmap(SMMU_BUFFER_S *pSmmuBuf)
{
	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return;
	}
	if (pSmmuBuf->pu8StartVirAddr) {
		unmap_mmb(pSmmuBuf->pu8StartVirAddr);
	}
}

HI_VOID HI_DRV_SMMU_Release(SMMU_BUFFER_S *pSmmuBuf)
{
	mmb_addr_t smmuaddr;

	if (!pSmmuBuf) {
		HI_PRINT("[%s] invalid param\n", __FUNCTION__);
		return;
	}

	smmuaddr = (mmb_addr_t)pSmmuBuf->u32StartSmmuAddr;
	if (MMB_ADDR_INVALID != smmuaddr) {
		delete_mmb(smmuaddr, 1);
	}
}

HI_U32 HI_DRV_MMZ_MapToSmmu(MMZ_BUFFER_S *psMBuf)
{
	mmb_addr_t iommu_addr;
	mmb_addr_t phyaddr = psMBuf->u32StartPhyAddr;

	iommu_addr = cma_mapto_smmu(phyaddr, 0);
	if (iommu_addr == MMB_ADDR_INVALID) {
		HI_PRINT("phy:0x%x cma map to smmu failed! \n",
					(unsigned int)phyaddr);
		return (HI_U32)MMB_ADDR_INVALID;
	}

	return iommu_addr;
}

HI_S32 HI_DRV_MMZ_UnmapFromSmmu(SMMU_BUFFER_S *psMBuf)
{
	int ret;
	mmb_addr_t iommu_addr = psMBuf->u32StartSmmuAddr;

	ret = cma_unmapfrom_smmu(iommu_addr, 1);
	if (ret == HI_FAILURE) {
		HI_PRINT("smmu:0x%x cma unmap to smmu failed!\n",
					(unsigned int)iommu_addr);
		psMBuf->u32StartSmmuAddr = (HI_U32)MMB_ADDR_INVALID;
		return HI_FAILURE;
	}
	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_GetSmmuAddrByVirt(SMMU_BUFFER_S *psMBuf)
{
	if (!psMBuf->pu8StartVirAddr) {
		HI_PRINT("%s: err arg\n",__func__);
		return HI_FAILURE;
	}

	psMBuf->u32StartSmmuAddr = (HI_U32)get_phyaddr_byvirt((HI_VOID*)psMBuf->pu8StartVirAddr, 1);

	if (MMB_ADDR_INVALID != psMBuf->u32StartSmmuAddr)
		return HI_SUCCESS;

	return HI_FAILURE;
}

extern void hisi_get_iommu_ptable_addr(unsigned int *pt_addr, unsigned int *err_rdaddr, unsigned int *err_wraddr);

HI_VOID HI_DRV_SMMU_GetPageTableAddr(HI_U32 *pu32ptaddr, HI_U32 *pu32err_rdaddr, HI_U32 *pu32err_wraddr)
{
	hisi_get_iommu_ptable_addr(pu32ptaddr, pu32err_rdaddr, pu32err_wraddr);
}
#endif

HI_S32  HI_DRV_SECMMZ_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, MMZ_BUFFER_S *pSecMBuf)
{
	mmb_addr_t phyaddr;
	u32 addr;

	if (!bufname) {
		HI_PRINT("[%s] bufname is null\n", __FUNCTION__);
		return HI_FAILURE;
	}

	if (0 == size) {
		HI_PRINT("[%s] bufname=%s, size is 0\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	if (!pSecMBuf) {
		HI_PRINT("[%s] bufname=%s, pSecMBuf is null\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	pSecMBuf->u32Size = 0;
	pSecMBuf->pu8StartVirAddr = HI_NULL;
	pSecMBuf->u32StartPhyAddr = (HI_U32)MMB_ADDR_INVALID;

	phyaddr = new_mmb(bufname, size, align, NULL);
	if (phyaddr == MMB_ADDR_INVALID) {
		HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	addr = secmem_alloc(phyaddr, 0);
	if (MMB_ADDR_INVALID == addr) {
		delete_mmb(phyaddr, 0);
		return HI_FAILURE;
	}

	pSecMBuf->u32StartPhyAddr = (HI_U32)addr;
	pSecMBuf->u32Size = size;

	return HI_SUCCESS;

}

HI_S32 HI_DRV_SECMMZ_Release(const MMZ_BUFFER_S *pSecMBuf)
{
	u32 phys_addr;

	if (!pSecMBuf) {
		HI_PRINT("%s:invalidate params!\n", __func__);
		return HI_FAILURE;
	}

	phys_addr = secmem_free(pSecMBuf->u32StartPhyAddr, 0);
	if (MMB_ADDR_INVALID == phys_addr) {
		HI_PRINT("%s: secmem cannot to be free! phys:0x%x \n", __func__,
						pSecMBuf->u32StartPhyAddr);
		return HI_FAILURE;
	}

	delete_mmb(phys_addr, 0);

	return HI_SUCCESS;
}

HI_S32  HI_DRV_SECSMMU_Alloc(const HI_CHAR *bufname, HI_U32 size, HI_U32 align, SMMU_BUFFER_S *pSecSmmuBuf)
{
	mmb_addr_t smmuaddr;
	HI_U32 sec_smmu;

	if (!bufname) {
		HI_PRINT("[%s] bufname is null\n", __FUNCTION__);
		return HI_FAILURE;
	}

	if (0 == size) {
		HI_PRINT("[%s] bufname=%s, size is 0\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	if (!pSecSmmuBuf) {
		HI_PRINT("[%s] bufname=%s, pSecSmmuBuf is null\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	pSecSmmuBuf->u32Size = 0;
	pSecSmmuBuf->pu8StartVirAddr = HI_NULL;
	pSecSmmuBuf->u32StartSmmuAddr = (HI_U32)MMB_ADDR_INVALID;

	smmuaddr = new_mmb(bufname, size, align, MMZ_SMMU);
	if (smmuaddr == MMB_ADDR_INVALID) {
		HI_PRINT("[%s] Alloc %s failed\n", __FUNCTION__, bufname);
		return HI_FAILURE;
	}

	sec_smmu = secmem_alloc(smmuaddr, 1);
	if (MMB_ADDR_INVALID == sec_smmu) {
		delete_mmb(smmuaddr, 1);
		return HI_FAILURE;
	}

	pSecSmmuBuf->u32StartSmmuAddr = (HI_U32)sec_smmu;
	pSecSmmuBuf->u32Size = size;

	return HI_SUCCESS;
}

HI_S32 HI_DRV_SECSMMU_Release(const SMMU_BUFFER_S *pSecSmmuBuf)
{
	u32 smmuaddr;

	if (!pSecSmmuBuf) {
		HI_PRINT("%s:invalidate params!\n", __func__);
		return HI_FAILURE;
	}

	smmuaddr = secmem_free(pSecSmmuBuf->u32StartSmmuAddr, 1);
	if (MMB_ADDR_INVALID == smmuaddr) {
		HI_PRINT("%s: secmem cannot to be freed! secsmmu:0x%x \n", __func__,
						pSecSmmuBuf->u32StartSmmuAddr);
		return HI_FAILURE;
	}

	delete_mmb(smmuaddr, 1);

	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_MapToSecSmmu(HI_U32 nonSecsmmu, SMMU_BUFFER_S *pSecSmmuBuf)
{
	HI_U32 sec_smmu;

	if (!pSecSmmuBuf) {
		HI_PRINT("%s: invalidate param!\n", __func__);
		return HI_FAILURE;
	}

	sec_smmu = secmem_map_to_secsmmu(nonSecsmmu, 1);
	if (MMB_ADDR_INVALID == sec_smmu) {
		HI_PRINT("%s: map to sec smmu failed! nonSecsmmu:0x%x \n",
			__func__, nonSecsmmu);
		return HI_FAILURE;
	}

	pSecSmmuBuf->u32StartSmmuAddr = sec_smmu;
	pSecSmmuBuf->pu8StartVirAddr = HI_NULL;

	return HI_SUCCESS;
}

HI_S32 HI_DRV_SMMU_UnmapFromSecSmmu(const SMMU_BUFFER_S *pSecSmmuBuf)
{
	int ret;

	if (!pSecSmmuBuf) {
		HI_PRINT("%s: invalidate param!\n", __func__);
		return HI_FAILURE;
	}
	ret = secmem_unmap_from_secsmmu(pSecSmmuBuf->u32StartSmmuAddr, 1);
	if (ret) {

		HI_PRINT("%s: secmem_unmap_from_secsmmu failed!sec-smmu:0x%x \n",
				__func__, pSecSmmuBuf->u32StartSmmuAddr);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_MapToSecSmmu(const MMZ_BUFFER_S *psMBuf, SMMU_BUFFER_S *pSecSmmuBuf)
{
	HI_U32 sec_smmu;

	if (!psMBuf || !pSecSmmuBuf) {
		HI_PRINT("%s: invalidate param!\n", __func__);
		return HI_FAILURE;
	}

	sec_smmu = secmem_map_to_secsmmu(psMBuf->u32StartPhyAddr, 0);
	if (MMB_ADDR_INVALID == sec_smmu) {
		HI_PRINT("%s: map to sec smmu failed! phys_addr:0x%x \n",
			__func__, psMBuf->u32StartPhyAddr);
		return HI_FAILURE;
	}

	pSecSmmuBuf->u32StartSmmuAddr = sec_smmu;
	pSecSmmuBuf->pu8StartVirAddr = HI_NULL;

	return HI_SUCCESS;
}

HI_S32 HI_DRV_MMZ_UnmapFromSecSmmu(const SMMU_BUFFER_S *pSecSmmuBuf)
{
	int ret;

	if (!pSecSmmuBuf) {
		HI_PRINT("%s: invalidate param!\n", __func__);
		return HI_FAILURE;
	}
	ret = secmem_unmap_from_secsmmu(pSecSmmuBuf->u32StartSmmuAddr, 0);
	if (ret) {
		HI_PRINT("%s: secmem_unmap_from_secsmmu failed!sec-smmu:0x%x \n",
				__func__, pSecSmmuBuf->u32StartSmmuAddr);

		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

EXPORT_SYMBOL(HI_DRV_MMZ_AllocAndMap);
EXPORT_SYMBOL(HI_DRV_MMZ_UnmapAndRelease);
EXPORT_SYMBOL(HI_DRV_MMZ_Alloc);
EXPORT_SYMBOL(HI_DRV_MMZ_MapCache);
EXPORT_SYMBOL(HI_DRV_MMZ_Flush);
EXPORT_SYMBOL(HI_DRV_MMZ_Map);
EXPORT_SYMBOL(HI_DRV_MMZ_Unmap);
EXPORT_SYMBOL(HI_DRV_MMZ_Release);
EXPORT_SYMBOL(HI_DRV_MMZ_GetPhyByVirt);

#ifdef HI_SMMU_SUPPORT
EXPORT_SYMBOL(HI_DRV_SMMU_AllocAndMap);
EXPORT_SYMBOL(HI_DRV_SMMU_UnmapAndRelease);
EXPORT_SYMBOL(HI_DRV_SMMU_Alloc);
EXPORT_SYMBOL(HI_DRV_SMMU_Release);
EXPORT_SYMBOL(HI_DRV_SMMU_Map);
EXPORT_SYMBOL(HI_DRV_SMMU_MapCache);
EXPORT_SYMBOL(HI_DRV_SMMU_Unmap);
EXPORT_SYMBOL(HI_DRV_SMMU_Flush);
EXPORT_SYMBOL(HI_DRV_MMZ_MapToSmmu);
EXPORT_SYMBOL(HI_DRV_MMZ_UnmapFromSmmu);
EXPORT_SYMBOL(HI_DRV_SMMU_GetSmmuAddrByVirt);
EXPORT_SYMBOL(HI_DRV_SMMU_GetPageTableAddr);
#endif

EXPORT_SYMBOL(HI_DRV_SECMMZ_Alloc);
EXPORT_SYMBOL(HI_DRV_SECMMZ_Release);
EXPORT_SYMBOL(HI_DRV_SECSMMU_Alloc);
EXPORT_SYMBOL(HI_DRV_SECSMMU_Release);
EXPORT_SYMBOL(HI_DRV_SMMU_MapToSecSmmu);
EXPORT_SYMBOL(HI_DRV_SMMU_UnmapFromSecSmmu);
EXPORT_SYMBOL(HI_DRV_MMZ_MapToSecSmmu);
EXPORT_SYMBOL(HI_DRV_MMZ_UnmapFromSecSmmu);
