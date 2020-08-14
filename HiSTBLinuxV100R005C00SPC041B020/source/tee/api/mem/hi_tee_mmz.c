
#include "sre_syscalls_chip.h"
#include "tee_internal_api.h"

#include "hi_tee_mmz.h"
#include "mmz_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

HI_ULONG HI_TEE_MMZ_New(HI_U32 size, HI_CHAR *mmz_name, HI_CHAR *buf_name)
{
	HI_ULONG ret;
	HI_MMZ_BUF_S Buf;

	if (0 == size || size > 0x40000000)
	{
		return HI_NULL;
	}

	Buf.bufsize = size;
	Buf.alloc_type = SECURE_MEM;

	ret = __MMZ_NEW(&Buf, mmz_name, buf_name);;
	if (-1 == ret)
	{
		return HI_NULL;
	}
	else
	{
		return ret;
	}
}

HI_S32 HI_TEE_MMZ_Delete(HI_ULONG phys_addr)
{
	return __MMZ_DEL(phys_addr);
}

HI_VOID *HI_TEE_MMZ_Map(HI_ULONG phys_addr, HI_BOOL cached)
{
	return __MMZ_MAP(phys_addr, cached);
}

HI_S32 HI_TEE_MMZ_Unmap(HI_VOID *virt_addr)
{
	return __MMZ_UNMAP((HI_ULONG)virt_addr);
}

HI_S32 HI_TEE_MMZ_Flush(HI_ULONG phys_addr)
{
	return __MMZ_FLUSH(phys_addr);
}

#ifdef WITH_HISI_SMMU
HI_ULONG HI_TEE_MMZ_MapToSecSmmu(HI_ULONG phys_addr, HI_ULONG size)
{
	HI_ULONG sec_smmu = 0;

	if (!phys_addr || !size) {
		uart_printf_func("%s err args, phys_addr:0x%x size:0x%x\n", phys_addr, size);
		return sec_smmu;
	}

	sec_smmu = __hisi_sec_map_to_sec_smmu(phys_addr, size, 0);
	if (!sec_smmu) {
		uart_printf_func("%s map to sec smmu failed, phys:0x%x\n", __func__, phys_addr);
	}

	return sec_smmu;
}

HI_S32 HI_TEE_MMZ_UnmapFromSecSmmu(HI_ULONG sec_smmu)
{
	HI_S32 ret = -1;

	if (!sec_smmu) {
		uart_printf_func("%s err args, sec_smmu:0x%x \n", __func__, sec_smmu);
		return ret;
	}

	ret = __hisi_sec_unmap_from_sec_smmu(sec_smmu, 0);
	if (ret) {
		uart_printf_func("%s unmap from sec smmu failed, sec_smmu:0x%x \n", __func__, sec_smmu);
		return ret;
	}

	return ret;
}
#endif

HI_S32 HI_TEE_MMZ_IsSecMMZ(HI_ULONG phys_addr)
{
	return __TEE_IsSecMMZ(phys_addr);
}

int HI_TEE_IsNonSecureMem(unsigned long phys_addr, unsigned long size)
{
        int ret = 0;
        ret = __TEE_IsNonSecMem(phys_addr, size);
        return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

