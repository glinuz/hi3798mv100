#include "hi_tee_mem.h"
#include "sre_syscalls_chip.h"
#include "hi_type.h"

#define HI_SUCCESS      0
#define HI_FAILED       -1

#define SMMU_MEM        1
#define CMA_MEM         0

#define SHARE_MMZ       1

HI_ULONG HI_TEE_SMMU_Alloc(const HI_CHAR *pBufName, HI_ULONG ulSize, HI_ULONG ulAlign)
{
	HI_ULONG sec_smmu = 0;
	if (!ulSize) {
		uart_printf_func("%s, err args, size:0x%x\n", ulSize);
		return sec_smmu;
	}

	sec_smmu = __hisi_sec_alloc(pBufName, ulSize, ulAlign, SMMU_MEM);
	if (!sec_smmu)
		uart_printf_func("%s, alloc mem failed!\n", __func__);

	return sec_smmu;
}

HI_S32 HI_TEE_SMMU_Free(HI_ULONG ulSecSmmu)
{
	int ret;

	if (!ulSecSmmu) {
		uart_printf_func("%s, err args, sec smmu:0x%x\n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	ret = __hisi_sec_free(ulSecSmmu, SMMU_MEM);
	if (ret) {
		uart_printf_func("%s, free mem failed, sec-smmu:0x%x!\n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_VOID *HI_TEE_SMMU_MapToCpu(HI_ULONG ulSecSmmu, HI_BOOL bCached)
{
	void *virt;

	if (!ulSecSmmu) {
		uart_printf_func("%s, err args, sec smmu:0x%x\n", __func__, ulSecSmmu);
		return NULL;
	}
	virt = (void *)__hisi_sec_map_to_cpu(ulSecSmmu, SMMU_MEM, bCached);
	if (NULL == virt) {
		uart_printf_func("%s, map to cpu failed, sec-smmu:0x%x!\n", __func__, ulSecSmmu);
		return NULL;
	}

	return virt;
}

HI_S32 HI_TEE_SMMU_UnmapFromCpu(HI_VOID *pSecVirt)
{
	int ret = 0;

	if (NULL == pSecVirt) {
		uart_printf_func("%s, err args, sec virt:0x%x\n", __func__, (unsigned long)pSecVirt);
		return HI_FAILED;
	}

	ret = __hisi_sec_unmap_from_cpu(pSecVirt);
	if (ret) {
		uart_printf_func("%s, unmap mem failed, sec-virt:0x%x!\n", __func__, (unsigned long)pSecVirt); 
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_ULONG HI_TEE_MMZ_Alloc(const HI_CHAR *pBufName, HI_ULONG ulSize, HI_ULONG ulAlign, HI_TEE_MMZ_TYEP_E eMemType)
{
	HI_ULONG phys_addr = 0;
	if (!ulSize) {
		uart_printf_func("%s, err args, size:0x%x\n", ulSize);
		return phys_addr;
	}
	if (eMemType) {
		phys_addr = __hisi_sec_alloc(pBufName, ulSize, ulAlign, CMA_MEM);
		if (!phys_addr)
			uart_printf_func("%s, alloc mmz mem failed!\n", __func__);
	} else { 
		phys_addr = HI_TEE_MMZ_New(ulSize, NULL, pBufName);
		if (!phys_addr)
			uart_printf_func("%s, alloc sec mmz mem failed!\n", __func__);
	}

	return phys_addr;
}

HI_S32 HI_TEE_MMZ_Free(HI_ULONG ulPhysAddr)
{
	int ret;

	if (!ulPhysAddr) {
		uart_printf_func("%s, err args, phys addr:0x%x\n", __func__, ulPhysAddr);
		return HI_FAILED;
	}

	if (!HI_TEE_MMZ_IsSecMMZ(ulPhysAddr)) {
		ret = __hisi_sec_free(ulPhysAddr, CMA_MEM);
	} else {
		ret = HI_TEE_MMZ_Delete(ulPhysAddr);
	}
	if (ret) {
		uart_printf_func("%s, free mem failed, sec-phys:0x%x!\n", __func__, ulPhysAddr);
		return HI_FAILED;
	}
	return HI_SUCCESS;
}

/*
 * parameter bCached is only used in sec mmz zone.
 *
 */
HI_VOID *HI_TEE_MMZ_MapToCpu(HI_ULONG ulPhysAddr, HI_BOOL bCached)
{
	HI_VOID *virt;

	if (!ulPhysAddr) {
		uart_printf_func("%s, err args, phys addr:0x%x\n", __func__, ulPhysAddr);
		return NULL;
	}
	if (!HI_TEE_MMZ_IsSecMMZ(ulPhysAddr)) {
		virt = (HI_VOID *)__hisi_sec_map_to_cpu(ulPhysAddr, CMA_MEM, bCached);
	} else {
		virt = HI_TEE_MMZ_Map(ulPhysAddr, bCached);
	}
	if (NULL == virt) {
		uart_printf_func("%s, map to cpu failed, phys addr:0x%x!\n", __func__, ulPhysAddr);
		return NULL;
	}

	return virt;
}

/* the func is only used for sec mmz zone  */
HI_S32 HI_TEE_MMZ_FlushMem(HI_ULONG ulPhysAddr)
{
	if (HI_TEE_MMZ_IsSecMMZ(ulPhysAddr))
		return HI_TEE_MMZ_Flush(ulPhysAddr);

	uart_printf_func("%s, phys addr: 0x%x ,the phys must be in sec mmz!\n", __func__, ulPhysAddr);
	return HI_FAILED;
}

/* flush dcache  */
HI_S32 HI_TEE_FlushMem(HI_VOID *pVirt, HI_ULONG ulSize)
{
	__hisi_mem_flush(pVirt, ulSize);

	return 0;
}

HI_S32 HI_TEE_MMZ_UnmapFromCpu(HI_VOID *pSecVirt, HI_ULONG ulPhysAddr)
{
	int ret = 0;

	if (NULL == pSecVirt) {
		uart_printf_func("%s, err args, sec virt:0x%x\n", __func__, (unsigned long)pSecVirt);
		return HI_FAILED;
	}

	if (!HI_TEE_MMZ_IsSecMMZ(ulPhysAddr)) {
		ret = __hisi_sec_unmap_from_cpu(pSecVirt);
	} else {
		ret = HI_TEE_MMZ_Unmap(pSecVirt);
	}
	if (ret) {
		uart_printf_func("%s, unmap mem failed, sec-virt:0x%x!\n", __func__, (unsigned long)pSecVirt); 
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_ULONG HI_TEE_MMZ_MapToSmmu(HI_ULONG ulPhysAddr, HI_ULONG ulSize)
{
	HI_ULONG sec_smmu = 0;

	if (!ulPhysAddr || !ulSize) {
		uart_printf_func("%s err args, phys addr:0x%x size:0x%x\n", ulPhysAddr, ulSize);
		return sec_smmu;
	}

	if (!HI_TEE_MMZ_IsSecMMZ(ulPhysAddr)) {
		sec_smmu = __hisi_sec_map_to_sec_smmu(ulPhysAddr, ulSize, SHARE_MMZ);
	} else {
		sec_smmu = HI_TEE_MMZ_MapToSecSmmu(ulPhysAddr, ulSize);
	}
	if (!sec_smmu) {
		uart_printf_func("%s map to sec smmu failed, phys:0x%x\n", __func__, ulPhysAddr);
	}

	return sec_smmu;
}

HI_S32 HI_TEE_MMZ_UnmapFromSmmu(HI_ULONG ulSecSmmu, HI_ULONG ulPhysAddr)
{
	int ret = 0;

	if (!ulSecSmmu) {
		uart_printf_func("%s err args, sec smmu:0x%x \n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	if (!HI_TEE_MMZ_IsSecMMZ(ulPhysAddr)) {
		ret = __hisi_sec_unmap_from_sec_smmu(ulSecSmmu, SHARE_MMZ);
	} else {
		ret = HI_TEE_MMZ_UnmapFromSecSmmu(ulSecSmmu);
	}
	if (ret) {
		uart_printf_func("%s unmap from sec smmu failed, sec smmu:0x%x \n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_VOID *HI_TEE_SMMU_NonsecSmmu_MapToCpu(HI_ULONG ulNormalSmmu, HI_BOOL bCached)
{
	HI_VOID *sec_virt = NULL;

	if (!ulNormalSmmu) {
		uart_printf_func("%s err args, normal smmu:0x%x \n", __func__, ulNormalSmmu);
		return HI_FAILED;
	}

	sec_virt = (HI_VOID *)__hisi_nonsec_mem_map_to_sec_cpu(ulNormalSmmu, SMMU_MEM, bCached);
	if (NULL == sec_virt) {
		uart_printf_func("%s failed, normal smmu:0x%x\n", __func__, ulNormalSmmu);
		return NULL;
	}

	return sec_virt;
}

HI_S32 HI_TEE_SMMU_NonsecSmmu_UnmapFromCpu(HI_VOID *pSecVirt)
{
	int ret = 0;

	if (NULL == pSecVirt) {
		uart_printf_func("%s err args, sec virt:0x%x \n", __func__, (HI_ULONG)pSecVirt);
		return HI_FAILED;
	}

	ret = __hisi_nosec_mem_unmap_from_sec_cpu(pSecVirt);
	if (ret) {
		uart_printf_func("%s failed,sec virt:0x%x \n", __func__, (HI_ULONG)pSecVirt);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_ULONG HI_TEE_SMMU_NonsecSmmu_MapToSecSmmu(HI_ULONG ulNormalSmmu)
{
	HI_ULONG sec_smmu = 0;

	if (!ulNormalSmmu) {
		uart_printf_func("%s err args, normal smmu:0x%x \n", __func__, ulNormalSmmu);
		return sec_smmu;
	}

	sec_smmu = __hisi_nosec_mem_map_to_sec_smmu(ulNormalSmmu, SMMU_MEM);
	if (!sec_smmu) {
		uart_printf_func("%s failed, normal smmu:0x%x \n", __func__, ulNormalSmmu);
		return sec_smmu;
	}

	return sec_smmu;
}

HI_S32 HI_TEE_SMMU_NonsecSmmu_UnmapFromSecSmmu(HI_ULONG ulSecSmmu)
{
	int ret = 0;

	if (!ulSecSmmu) {
		uart_printf_func("%s err args, sec smmu:0x%x \n", __func__, ulSecSmmu);
		return ulSecSmmu;
	}

	ret = __hisi_nosec_mem_unmap_from_sec_smmu(ulSecSmmu);
	if (ret) {
		uart_printf_func("%s failed, sec smmu:0x%x \n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}


HI_VOID *HI_TEE_SMMU_NonsecMMZ_MapToCpu(HI_ULONG ulNormalPhys, HI_BOOL bCached)
{
	HI_VOID *sec_virt = NULL;

	if (!ulNormalPhys) {
		uart_printf_func("%s err args, normal phys:0x%x \n", __func__, ulNormalPhys);
		return NULL;
	}

	sec_virt = (HI_VOID *)__hisi_nonsec_mem_map_to_sec_cpu(ulNormalPhys, CMA_MEM, bCached);
	if (NULL == sec_virt) {
		uart_printf_func("%s failed, normal phys:0x%x\n", __func__, ulNormalPhys);
		return NULL;
	}

	return sec_virt;
}

HI_S32 HI_TEE_SMMU_NonsecMMZ_UnmapFromCpu(HI_VOID *pSecVirt)
{
	int ret = 0;

	if (NULL == pSecVirt) {
		uart_printf_func("%s err args, sec virt:0x%x \n", __func__, (unsigned long)pSecVirt);
		return HI_FAILED;
	}

	ret = __hisi_nosec_mem_unmap_from_sec_cpu(pSecVirt);
	if (ret) {
		uart_printf_func("%s failed,sec virt:0x%x \n", __func__, (unsigned long)pSecVirt);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}

HI_ULONG HI_TEE_SMMU_NonsecMMZ_MapToSecSmmu(HI_ULONG ulNormalPhys)
{
	HI_ULONG sec_smmu = 0;

	if (!ulNormalPhys) {
		uart_printf_func("%s err args, normal phys:0x%x \n", __func__, ulNormalPhys);
		return sec_smmu;
	}

	sec_smmu = __hisi_nosec_mem_map_to_sec_smmu(ulNormalPhys, CMA_MEM);
	if (!sec_smmu) {
		uart_printf_func("%s failed, normal phys:0x%x \n", __func__, ulNormalPhys);
		return sec_smmu;
	}

	return sec_smmu;
}

HI_S32 HI_TEE_SMMU_NonsecMMZ_UnmapFromSecSmmu(HI_ULONG ulSecSmmu)
{
	int ret = 0;

	if (!ulSecSmmu) {
		uart_printf_func("%s err args, sec_smmu:0x%x \n", __func__, ulSecSmmu);
		return ulSecSmmu;
	}

	ret = __hisi_nosec_mem_unmap_from_sec_smmu(ulSecSmmu);
	if (ret) {
		uart_printf_func("%s failed, secsmmu:0x%x \n", __func__, ulSecSmmu);
		return HI_FAILED;
	}

	return HI_SUCCESS;
}
