#ifndef __DRV_TZSMMU_H
#define __DRV_TZSMMU_H

#include <linux/hisi/tee_smmu.h>
#include "hi_drv_mmz.h"
#include "hi_type.h"

u32 secmem_alloc(mmb_addr_t phyaddr, unsigned int iommu);

u32 secmem_free(mmb_addr_t sec_addr, unsigned int iommu);

u32 secmem_map_to_secsmmu(HI_U32 phyaddr, int iommu);

int secmem_unmap_from_secsmmu(HI_U32 sec_addr, int iommu);

u32 hi_tee_smmumem_into_ta(u32 addr, int iommu);

void hi_tee_smmumem_outfrom_ta(u32 buf_phys);

int hi_tee_agent_end(void);
#endif


