#ifndef __MEDIA_MEM_INTF_H__
#define __MEDIA_MEM_INTF_H__

#include "hi_type.h"

typedef unsigned int  mmb_addr_t;

#define MMB_ADDR_INVALID (0)

mmb_addr_t new_mmb(const char *name, int size, unsigned int align, const char *zone_name);
void delete_mmb(mmb_addr_t addr, unsigned int iommu);
void *remap_mmb(mmb_addr_t addr, unsigned int iommu);
void *remap_mmb_cached(mmb_addr_t addr, unsigned int iommu);
int unmap_mmb(void *mapped_addr);
mmb_addr_t cma_mapto_smmu(mmb_addr_t addr, int iommu);
int cma_unmapfrom_smmu(mmb_addr_t addr, int iommu);
mmb_addr_t get_phyaddr_byvirt(void *mapped_addr, int iommu);
struct sg_table *get_meminfo(u32 addr, u32 iommu, u32 *size);
void flush_cache_kern(mmb_addr_t phyaddr, void *viraddr,
			mmb_addr_t len, unsigned int iommu);
mmb_addr_t get_sec_smmu_by_nosmmu(HI_U32 nonsmmu);
mmb_addr_t get_sec_smmu_by_phys(HI_U32 phys_addr);
mmb_addr_t get_phys_by_secsmmu(HI_U32 sec_smmu);
mmb_addr_t get_nonsecsmmu_by_secsmmu(HI_U32 sec_smmu);

#endif

