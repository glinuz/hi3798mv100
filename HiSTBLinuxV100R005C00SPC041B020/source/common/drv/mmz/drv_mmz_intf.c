/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <linux/kernel.h>

#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>
#include <linux/string.h>
#include <linux/list.h>

#include <linux/time.h>

#include "drv_media_mem.h"
#include "drv_mmz.h"

mmb_addr_t new_mmb(const char *name, int size, unsigned int align, 
		const char *zone_name)
{
	hil_mmb_t *mmb;
	
	mmb = hil_mmb_alloc(name, size, align, zone_name, HI_KERNEL_ALLOC);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;
	if (mmb->iommu)
		return mmb->iommu_addr;
	else
		return mmb->phys_addr;
}

void delete_mmb(mmb_addr_t addr, unsigned int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, iommu);
	if(mmb ==NULL)
		return;

	hil_mmb_free(mmb);
}

void *remap_mmb(mmb_addr_t addr, unsigned int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, iommu);
	if(mmb ==NULL)
		return NULL;

	return hil_mmb_map2kern(mmb);
}

void *remap_mmb_cached(mmb_addr_t addr, unsigned int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, iommu);
	if(mmb ==NULL)
		return NULL;

	return hil_mmb_map2kern_cached(mmb);
}

int unmap_mmb(void *mapped_addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_kvirt(mapped_addr);
	if(mmb == NULL)
		return -1;

	return hil_mmb_unmap(mmb, mapped_addr);
}

void flush_cache_kern(mmb_addr_t phyaddr, void *viraddr,
			mmb_addr_t len, unsigned int iommu)
{
	flush_inner_cache((void *)viraddr, len);
	flush_outer_cache_range(phyaddr, len, iommu);
}

mmb_addr_t cma_mapto_smmu(mmb_addr_t addr, int iommu)
{
	return hil_mmb_cma_mapto_iommu(addr, iommu);
}

int cma_unmapfrom_smmu(mmb_addr_t addr, int iommu)
{
	return hil_mmb_cma_unmapfrom_iommu(addr, iommu);
}

mmb_addr_t get_phyaddr_byvirt(void *mapped_addr, int iommu)
{
	hil_mmb_t *mmb;
	mmb_addr_t phyaddr = MMB_ADDR_INVALID;
	mmb_addr_t iommuaddr = MMB_ADDR_INVALID;

	mmb = hil_mmbinfo_getby_kvirt(mapped_addr);
	if(mmb == NULL)
		return MMB_ADDR_INVALID;

	if (mmb->iommu_addr != MMB_ADDR_INVALID)
		iommuaddr = mmb->iommu_addr + ((unsigned long)mapped_addr -
			(unsigned long)mmb->kdata->kvirt);

	if (mmb->phys_addr != MMB_ADDR_INVALID)
		phyaddr = mmb->phys_addr + ((unsigned long)mapped_addr -
			(unsigned long)mmb->kdata->kvirt);

	if (iommu)
		return iommuaddr;

	else
		return phyaddr;

}

struct sg_table *get_meminfo(u32 addr, u32 iommu, u32 *size)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys(addr, iommu);
	if(mmb ==NULL)
		return NULL;
	*size = mmb->length;
	return hil_get_meminfo(mmb);
}

mmb_addr_t get_nonsecsmmu_by_secsmmu(HI_U32 sec_smmu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_sec_addr(sec_smmu, 1);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;
	return mmb->iommu_addr;
}

mmb_addr_t get_phys_by_secsmmu(HI_U32 sec_smmu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_sec_addr(sec_smmu, 1);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;
	return mmb->phys_addr;
}

mmb_addr_t get_sec_smmu_by_phys(HI_U32 phys_addr)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys(phys_addr, 0);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;
	return mmb->sec_smmu;
}

mmb_addr_t get_sec_smmu_by_nosmmu(HI_U32 nonsmmu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys(nonsmmu, 1);
	if(mmb ==NULL)
		return MMB_ADDR_INVALID;
	return mmb->sec_smmu;
}
EXPORT_SYMBOL(new_mmb);
EXPORT_SYMBOL(delete_mmb);
EXPORT_SYMBOL(remap_mmb);
EXPORT_SYMBOL(remap_mmb_cached);
EXPORT_SYMBOL(unmap_mmb);
EXPORT_SYMBOL(flush_cache_kern);
EXPORT_SYMBOL(get_phyaddr_byvirt);
EXPORT_SYMBOL(get_meminfo);
EXPORT_SYMBOL(get_nonsecsmmu_by_secsmmu);
EXPORT_SYMBOL(get_phys_by_secsmmu);
EXPORT_SYMBOL(get_sec_smmu_by_phys);
EXPORT_SYMBOL(get_sec_smmu_by_nosmmu);
