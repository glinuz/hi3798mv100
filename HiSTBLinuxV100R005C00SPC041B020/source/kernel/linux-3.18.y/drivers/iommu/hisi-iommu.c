/*
 * Copyright (C) 20013-2013 hisilicon. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/iommu.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>

/*
 * 1 Iommu base: 0x0
 * SMMU logic accessing address is the range frome 0x0 to 0x100000000(4G).When
 * a smmu virt address(v_smmu) coming to smmu logic, smmu module calculates
 * the offset with (v_smmu - 0x0), and then finds locate of the exact page
 * table item via (page_table_base_address + offset),where the real phy addr
 * could be found.
 * The offset is very important.And it depends on smmu logic beginning access
 * address.So HISI_IOMMU_SPACE_BASE configged by software must be the same with
 * the smmu logic beginning access address, that's to say it must be 0.
 * 2 reserve area
 * Normally, smmu virt address is from 0x0 to 0xffffffff.But in some reason of
 * of logic, 1M address space should be reserved both at the beginning of the
 * virt address space and at the end of the virt address space.The range from
 * 0x0 to 0x100000 is effective and should have position in pagetable.It is
 * is reserved when init.The range from 0xfff00000 to 0xffffffff is reserved,
 * but we alse could not use those area.
 *
 */
#define HISI_IOMMU_SPACE_BASE 0x0
#define HISI_IOMMU_SPACE_SIZE 0xfff00000
#define HISI_IOMMU_IOVA_START HISI_IOMMU_SPACE_BASE
#define HISI_IOMMU_IOVA_END	(HISI_IOMMU_SPACE_BASE +\
				 HISI_IOMMU_SPACE_SIZE - 1)
#define HISI_IOVA_POOL_ALIGN SZ_4K

#define HISI_IOMMU_PE_PA_MASK 0xFFFFF000
#define HISI_IOMMU_PE_V_MASK (1 << 0)
#define ENABLE_CACHE 0

#ifdef DEBUG
#define dbg(format, arg...)    \
	do {                 \
		pr_info("[iommu]"format, ##arg); \
	} while (0)
#else
#define dbg(format, arg...)
#endif

#define SMMU_SCB_TTBR_OFFSET	(0x0208)
#define SMMU_CB_TTBR_OFFSET	(0x020C)
#define SMMU_ERR_RADDR_OFFSET	(0x0304)
#define SMMU_ERR_WRADDR_OFFSET	(0x0308)
#define SMMU_ERR_RW_SIZE	(0x200)
#define SMMU_ERR_RW_SIZE_ALIGN	(0x100)
#define SMMU_REG_SIZE		(0x1000)

#define HISI_IO_ADDRESS(x)  ((void __iomem *)(IO_ADDRESS(x)))

/*
 *0xf8c8f000 -- VEDU
 *0xff34f000 -- VPSS
 *0xF9BF0000 -- PVR
 *0xf8c1f000 -- TDE
 */
static const u32 reg_base[] = {
0x0,
};

u32 hisi_iommu_ptable_addr;
u32 hisi_iommu_ptable_size;
u32 hisi_iommu_err_rdaddr;
u32 hisi_iommu_err_wraddr;

DEFINE_MUTEX(hisi_iommu_mutex);

struct __hisi_iommu_domain {
	struct iommu_domain *domain;
	uint32_t *pgtbl_addr;
	size_t pgtbl_size;
	unsigned long iova_start;
	unsigned long iova_end;
};

struct iommu_zone {
	unsigned int  iova_start;
	unsigned int  iova_end;
	unsigned int  iova_align;
};
struct iommu_zone hisi_iommu;

struct iommu_zone *hisi_get_iommu_zone(void)
{
	hisi_iommu.iova_start = HISI_IOMMU_IOVA_START;
	hisi_iommu.iova_end = HISI_IOMMU_IOVA_END;
	hisi_iommu.iova_align = HISI_IOVA_POOL_ALIGN;

	/* 0x0 -- 0xfffff is reserved because logic cannot used  this area */
	hisi_iommu.iova_start = hisi_iommu.iova_start + 0x100000;

	return &hisi_iommu;

}
EXPORT_SYMBOL(hisi_get_iommu_zone);

void hisi_get_iommu_ptable_addr(unsigned int *pt_addr,
			unsigned int *err_rdaddr, unsigned int *err_wraddr)
{
	*pt_addr = hisi_iommu_ptable_addr;
	*err_rdaddr = hisi_iommu_err_rdaddr;
	*err_wraddr = hisi_iommu_err_wraddr;

	return;
}
EXPORT_SYMBOL(hisi_get_iommu_ptable_addr);

static int iommu_init(void)
{
	unsigned long p_size;
	void *addr;
	void *err_rd;
	void *err_wr;

	/*
	 * calculate tlb slze depend on HISI_IOMMU_SPACE_SIZE :
	 * total pages of mem: HISI_IOMMU_SPACE_SIZE >> PAGE_SHIFT
	 * each page need 32bit(4bytes) to store
	 * so whole tlbs need (HISI_IOMMU_SPACE_SIZE >> PAGE_SHIFT) << 2
	 * mem to store.
	 */
	p_size = (HISI_IOMMU_SPACE_SIZE >> PAGE_SHIFT) << 2;

	/* pgtable must be aligned with 1M   */
	p_size = ALIGN(p_size, 0x100000);

	addr = kmalloc(p_size, GFP_KERNEL);
	if (!addr) {
		pr_err("[%s]alloc_pages failed!\n", __func__);
		goto no_mem;
	}

	hisi_iommu_ptable_addr = virt_to_phys(addr);
	hisi_iommu_ptable_size = p_size;

	err_rd = kmalloc(SMMU_ERR_RW_SIZE, GFP_KERNEL);
	if (!err_rd) {
		pr_err("[%s] kmalloc failed!\n", __func__);
		goto rd_err;
	}
	memset(err_rd, 0, SMMU_ERR_RW_SIZE);
	hisi_iommu_err_rdaddr = virt_to_phys(err_rd);
	hisi_iommu_err_rdaddr = ALIGN(hisi_iommu_err_rdaddr, SMMU_ERR_RW_SIZE_ALIGN);


	err_wr = kmalloc(SMMU_ERR_RW_SIZE, GFP_KERNEL);
	if (!err_wr) {
		pr_err("[%s] kmalloc failed!\n", __func__);
		goto wr_err;
	}
	memset(err_wr, 0, SMMU_ERR_RW_SIZE);
	hisi_iommu_err_wraddr = virt_to_phys(err_wr);
	hisi_iommu_err_wraddr = ALIGN(hisi_iommu_err_wraddr, SMMU_ERR_RW_SIZE_ALIGN);
	return 0;

wr_err:
	kfree(err_rd);

rd_err:
	kfree(addr);

no_mem:
	pr_err("[%s]error: page table not ready!\n", __func__);
	return -ENOMEM;
}

static void device_init(void)
{
	int i = 0;
	int count;
	void *virt = NULL;

	count = sizeof(reg_base) / sizeof(reg_base[0]);

	pr_info("hisi_iommu_ptable_addr:phy 0x%x    size:0x%x\n",
			hisi_iommu_ptable_addr, hisi_iommu_ptable_size);
	pr_info("hisi_iommu_err_rdaddr :phy 0x%x    size:0x%x\n",
				hisi_iommu_err_rdaddr, SMMU_ERR_RW_SIZE);
	pr_info("hisi_iommu_err_wraddr :phy 0x%x     size:0x%x\n",
				hisi_iommu_err_wraddr, SMMU_ERR_RW_SIZE);

	for (; i < count; i++) {
		if (!reg_base[i])
			continue;
		virt = ioremap(reg_base[i], SMMU_REG_SIZE);
		if (!virt) {
			pr_err("\n %s  ioremap err!\n", __func__);
			return;
		}
		writel((u32)hisi_iommu_ptable_addr, virt + SMMU_SCB_TTBR_OFFSET);
		writel((u32)hisi_iommu_ptable_addr, virt + SMMU_CB_TTBR_OFFSET);
		writel((u32)hisi_iommu_err_rdaddr, virt + SMMU_ERR_RADDR_OFFSET);
		writel((u32)hisi_iommu_err_wraddr, virt + SMMU_ERR_WRADDR_OFFSET);
		iounmap(virt);
		virt = NULL;
	}
}


static void dump_domain(struct iommu_domain *domain)
{
#ifdef DEBUG
	struct __hisi_iommu_domain *hisi_domain = domain->priv;

	dbg("domain: %p, pgtbl_addr: %p, pgtbl_size: 0x%x,"
		"iova_start: 0x%lx, iova_end: 0x%lx\n",
		hisi_domain->domain, hisi_domain->pgtbl_addr,
		hisi_domain->pgtbl_size, hisi_domain->iova_start,
		hisi_domain->iova_end);
#endif
}

static void *map_pgtbl(unsigned long pgtbl_addr, size_t pgtbl_size)
{
	pgprot_t prot;
	struct page **pages;
	void *addr;
	int i = 0 , count;
	unsigned long pfn = __phys_to_pfn(pgtbl_addr);

	dbg("map_pgtbl: pgtbl_addr: 0x%lx, pgtbl_size: 0x%x\n",
			pgtbl_addr, pgtbl_size);

#if ENABLE_CACHE
	prot = PAGE_KERNEL_EXEC;
#else
	prot = pgprot_writecombine(PAGE_KERNEL_EXEC);
#endif
	count = pgtbl_size >> PAGE_SHIFT;
	pages = vmalloc(sizeof(struct page *) * count);
	if (!pages) {
		pr_info("%s :no mem !\n", __func__);
		return NULL;
	}
	for (; i < count; i++) {
		pages[i] = pfn_to_page(pfn);
		pfn++;
	}

	addr = vmap(pages, count, VM_MAP, prot);
	vfree(pages);
	if (addr == NULL) {
		pr_info("%s : no mem !\n", __func__);
		return NULL;
	}

	return addr;
}

static void unmap_pgtbl(void *pgtbl_virt_addr, unsigned long size)
{
#if ENABLE_CACHE
	unsigned long phys = virt_to_phys(pgtbl_virt_addr);
#ifdef CONFIG_ARM64
	__flush_dcache_area((void *)pgtbl_virt_addr, (size_t)size);
#else
	dmac_flush_range((void *)pgtbl_virt_addr,
				(void *)(pgtbl_virt_addr + size));

	outer_flush_range(phys, phys + size);
#endif
#endif
	vunmap(pgtbl_virt_addr);
	pgtbl_virt_addr = NULL;
}

static inline void flush_pgtbl(struct __hisi_iommu_domain *hisi_domain,
	unsigned long iova, size_t size)
{
#if ENABLE_CACHE
	unsigned long *start = ((unsigned long *)hisi_domain->pgtbl_addr) +
			((iova - hisi_domain->iova_start) >> PAGE_SHIFT);
	unsigned long *end = start + (size >> PAGE_SHIFT);

	dbg("flush pftbl range: %p -- %p\n", start, end);
#ifdef CONFIG_ARM64
	__flush_dcache_area((void *)start, size);
#else
	dmac_flush_range((void *)start, (void *)end);

	outer_flush_range(hisi_iommu_ptable_addr,
		  hisi_iommu_ptable_addr + hisi_iommu_ptable_size);
#endif
#else
	dbg("flush page table: dmb()\n");
	smp_wmb();
#endif
}

static int hisi_domain_init(struct iommu_domain *domain)
{
	struct __hisi_iommu_domain *hisi_domain;

	hisi_domain = kzalloc(sizeof(*hisi_domain), GFP_KERNEL);
	if (!hisi_domain) {
		pr_err("[%s]no mem!\n", __func__);
		return -ENOMEM;
	}

	/* set iova range */
	hisi_domain->iova_start = HISI_IOMMU_IOVA_START;
	hisi_domain->iova_end = HISI_IOMMU_IOVA_END;

	/* init the page table */
	hisi_domain->pgtbl_addr = map_pgtbl(hisi_iommu_ptable_addr,
			hisi_iommu_ptable_size);
	if (!hisi_domain->pgtbl_addr) {
		kfree(hisi_domain);
		return -EINVAL;
	}

	hisi_domain->pgtbl_size = hisi_iommu_ptable_size;

	memset(hisi_domain->pgtbl_addr, 0, hisi_domain->pgtbl_size);
	flush_pgtbl(hisi_domain, hisi_domain->iova_start,
		(hisi_domain->iova_end - hisi_domain->iova_start));

	domain->capablity.iova_start = HISI_IOMMU_IOVA_START;
	domain->capablity.iova_end   = HISI_IOMMU_IOVA_END;
	domain->capablity.iova_align = HISI_IOVA_POOL_ALIGN;
	domain->capablity.pg_sz = PAGE_SIZE;
	domain->capablity.pgtbl_base = hisi_iommu_ptable_addr;
	domain->capablity.off_on = true;

	domain->priv = hisi_domain;
	hisi_domain->domain = domain;

	/* just for debug */
	dump_domain(domain);

	return 0;
}

static void hisi_domain_destroy(struct iommu_domain *domain)
{
	struct __hisi_iommu_domain *hisi_domain;

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;

	/* free the page table */
	unmap_pgtbl(hisi_domain->pgtbl_addr, hisi_domain->pgtbl_size);

	domain->capablity.iova_start = 0;
	domain->capablity.iova_end   = 0;
	domain->capablity.iova_align = 0;
	domain->capablity.pg_sz = 0;
	domain->capablity.off_on = false;
	domain->capablity.pgtbl_base = 0;

	/* free the hisi_domain */
	kfree(domain->priv);
	domain->priv = NULL;

	mutex_unlock(&hisi_iommu_mutex);
}

int get_iova_range(struct iommu_domain *domain, unsigned long *iova_start,
		unsigned long *iova_end)
{
	struct __hisi_iommu_domain *hisi_domain = domain->priv;
	*iova_start = hisi_domain->iova_start;
	*iova_end = hisi_domain->iova_end;
	return 0;
}
EXPORT_SYMBOL_GPL(get_iova_range);

static inline void set_pg_entry(volatile uint32_t *pe_addr,
		uint32_t phys_addr)
{
	*pe_addr = (phys_addr & HISI_IOMMU_PE_PA_MASK) |
			HISI_IOMMU_PE_V_MASK;
}
static inline void clear_pg_entry(volatile uint32_t *pe_addr)
{
	*pe_addr = 0;
}

static int __invalidate_gap(struct __hisi_iommu_domain *hisi_domain,
	   unsigned long iova, phys_addr_t paddr, size_t size)
{
	uint32_t *pgtable;
	unsigned long offset = 0;

	/* get the pgtable address */
	pgtable = (uint32_t *)hisi_domain->pgtbl_addr;
	if (!pgtable) {
		pr_err("[%s]error: page table not ready!\n", __func__);
		return -EINVAL;
	}

	/* get the offset of first page entry */
	offset = (iova - hisi_domain->iova_start) >> order_base_2(SZ_4K);

	/* invalidate gap */
	*(pgtable + offset) = HISI_IOMMU_PE_PA_MASK;

	return 0;
}

static int __hisi_map(struct __hisi_iommu_domain *hisi_domain,
	   unsigned long iova, phys_addr_t paddr, size_t size)
{
	uint32_t *pgtable;
	unsigned long offset = 0;
	size_t maped_size = 0;

	/* get the pgtable address */
	pgtable = (uint32_t *)hisi_domain->pgtbl_addr;
	if (!pgtable) {
		pr_err("[%s]error: page table not ready!\n", __func__);
		return -EINVAL;
	}

	/* get the offset of first page entry */
	offset = (iova - hisi_domain->iova_start) >> order_base_2(SZ_4K);

	/* write page address into the table */
	for (maped_size = 0; maped_size < size; maped_size += SZ_4K, offset++) {
		set_pg_entry((pgtable + offset),
					(uint32_t)(paddr + maped_size));
	}

	dbg("[%s]iova: 0x%lx, paddr: 0x%x, size: 0x%x\n", __func__,
			iova, paddr, size);

	return 0;
}

static int hisi_map(struct iommu_domain *domain, unsigned long iova,
	   phys_addr_t paddr, size_t size, int prot)
{
	struct __hisi_iommu_domain *hisi_domain;
	int ret = 0;

	mutex_lock(&hisi_iommu_mutex);

	/* check domain */
	hisi_domain = domain->priv;
	if (!hisi_domain) {
		pr_err("hisi_domain NULL!\n");
		ret = -EINVAL;
		goto error;
	}

	if ((iova < hisi_domain->iova_start) || (size > hisi_domain->iova_end)) {
		pr_err("iova out of range!\n");
		ret = -EINVAL;
		goto error;
	}


	if (!IS_ALIGNED(paddr, SZ_4K) || !IS_ALIGNED(iova, SZ_4K)) {
		pr_err("addr is not aligend to SZ_4K!\n");
		ret = -EINVAL;
		goto error;
	}

	ret = __hisi_map(hisi_domain, iova, paddr, size);
	if (ret) {
		goto error;
	}

	flush_pgtbl(hisi_domain, iova, size);

error:
	mutex_unlock(&hisi_iommu_mutex);
	return ret;
}

static int __hisi_unmap(struct __hisi_iommu_domain *hisi_domain,
	   unsigned long iova, size_t size)
{
	uint32_t *pgtable;
	unsigned long offset = 0;
	size_t maped_size = 0;

	/* get the pgtable address */
	pgtable = (uint32_t *)hisi_domain->pgtbl_addr;
	if (!pgtable) {
		pr_err("[%s]error: page table not ready!\n", __func__);
		return -EINVAL;
	}

	/* get the offset of first page entry */
	offset = (iova - hisi_domain->iova_start) >> order_base_2(SZ_4K);

	/* write page address into the table */
	for (maped_size = 0; maped_size < size; maped_size += SZ_4K, offset++) {
		clear_pg_entry(pgtable + offset);
	}
	dbg("[%s]iova: 0x%lx, size: 0x%x\n", __func__, iova, size);

	return 0;
}

static size_t hisi_unmap(struct iommu_domain *domain, unsigned long iova,
	     size_t size)
{
	struct __hisi_iommu_domain *hisi_domain;

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;
	if (!hisi_domain) {
		pr_err("[%s]domain is not ready!\n", __func__);
		goto error;
	}

	if (!IS_ALIGNED(iova, SZ_4K)) {
		pr_err("iova is not aligned to SZ_4K!\n");
		goto error;
	}

	if (__hisi_unmap(hisi_domain, iova, size)) {
		goto error;
	}

	flush_pgtbl(hisi_domain, iova, size);

	mutex_unlock(&hisi_iommu_mutex);
	return size;

error:
	mutex_unlock(&hisi_iommu_mutex);
	return 0;
}

static dma_addr_t get_phys_addr(struct scatterlist *sg)
{
	/*
	 * Try sg_dma_address first so that we can
	 * map carveout regions that do not have a
	 * struct page associated with them.
	 */
	dma_addr_t dma_addr = sg_dma_address(sg);
	if (dma_addr == 0)
		dma_addr = sg_phys(sg);
	return dma_addr;
}

static int hisi_map_range(struct iommu_domain *domain, unsigned long iova,
	    struct scatterlist *sg, size_t size, int prot)
{
	struct __hisi_iommu_domain *hisi_domain;
	phys_addr_t phys_addr;
	size_t maped_size = 0;
	int ret = 0;

	dbg("[%s]+\n", __func__);

	dbg("domain: %p, iova: 0x%lx, sg: %p, size: 0x%x\n",
		domain, iova, sg, size);

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;
	if (!hisi_domain) {
		ret = -EINVAL;
		goto error;
	}

	if (!IS_ALIGNED(iova, SZ_4K)) {
		pr_info("iova is not aligned to SZ_4K!\n");
		ret = -EINVAL;
		goto error;
	}

	while (sg) {
		dbg("to map a sg: iova: 0x%lx, sg: %p, maped size: 0x%x\n",
			iova, sg, maped_size);

		phys_addr = get_phys_addr(sg);
		dbg("phys_addr: 0x%x\n", phys_addr);

		ret = __hisi_map(hisi_domain, iova, phys_addr, sg->length);
		if (ret) {
			pr_err("__hisi_map failed!\n");
			break;
		}

		/* unsigned int --> unsigned long */
		iova += (unsigned long)ALIGN(sg->length, SZ_4K);
		/* unsigned int --> size_t */
		maped_size += (size_t)ALIGN(sg->length, SZ_4K);
		if (maped_size >= (size - SZ_4K)) {
			/* invalidate gap   */
			__invalidate_gap(hisi_domain, iova, phys_addr, SZ_4K);

			break;
		}

		sg = sg_next(sg);
	};

	flush_pgtbl(hisi_domain, iova, size);

error:
	mutex_unlock(&hisi_iommu_mutex);

	dbg("[%s]-\n", __func__);

	return ret;
}
static size_t hisi_unmap_range(struct iommu_domain *domain, unsigned long iova,
	      size_t size)
{
	struct __hisi_iommu_domain *hisi_domain;
	int ret = 0;

	dbg("[%s]+\n", __func__);
	dbg("domain: %p, iova: 0x%lx, size: 0x%x\n", domain, iova, size);

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;
	if (!hisi_domain) {
		pr_err("hisi_domain NULL!\n");
		goto error;
	}

	if (!IS_ALIGNED(iova, SZ_4K)) {
		pr_err("iova is not aligned to 4K!\n");
		goto error;
	}

	ret = __hisi_unmap(hisi_domain, iova, size);
	if (ret) {
		pr_err("__hisi_unmap failed!\n");
		goto error;
	}

	flush_pgtbl(hisi_domain, iova, size);

	mutex_unlock(&hisi_iommu_mutex);
	dbg("[%s]-\n", __func__);
	return size;
error:
	mutex_unlock(&hisi_iommu_mutex);
	return 0;
}

static size_t hisi_map_tile_row(struct iommu_domain *domain, unsigned long iova,
	   size_t size, struct scatterlist *sg, size_t sg_offset)
{
	struct __hisi_iommu_domain *hisi_domain = domain->priv;
	unsigned long map_size;
	unsigned long phys_addr;
	unsigned long mapped_size = 0;
	int ret;

	map_size = ((sg->length - sg_offset) < size) ?
			(sg->length - sg_offset) : size;

	phys_addr = (unsigned long)get_phys_addr(sg) + sg_offset;

	while (mapped_size < size) {

		map_size = size - mapped_size;
		if (map_size > (sg->length - sg_offset)) {
			map_size = (sg->length - sg_offset);
		}

		phys_addr = (unsigned long)get_phys_addr(sg) + sg_offset;

		ret = __hisi_map(hisi_domain, iova + mapped_size,
				phys_addr, map_size);
		if (ret) {
			pr_err("[%s]__hisi_map failed!\n", __func__);
			break;
		}

		mapped_size += map_size;
		sg_offset = 0;
		sg = sg_next(sg);
		if (!sg) {
			break;
		}
	}

	return mapped_size;
}

static int hisi_map_tile(struct iommu_domain *domain, unsigned long iova,
	   struct scatterlist *sg, size_t size, int prot,
	   struct tile_format *format)
{
	struct __hisi_iommu_domain *hisi_domain;
	struct scatterlist *sg_node;
	unsigned int rows, row;
	unsigned int sg_mapped_size;
	unsigned int mapped_size;
	/* virt len and phys len in one row */
	unsigned int size_virt, size_phys;
	unsigned int sg_offset; /* mapped len of the first sg node */
	unsigned int phys_length;
	int ret = 0;

	dbg("[%s]+\n", __func__);

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;
	if (!hisi_domain) {
		ret = -EINVAL;
		goto error;
	}

	/* calculate the whole len of phys mem */
	for (phys_length = 0, sg_node = sg; sg_node;
				sg_node = sg_next(sg_node)) {
		phys_length += ALIGN(sg_node->length, SZ_4K);
	}
	dbg("phys_length: 0x%x\n", phys_length);

	/* calculate rows number */
	rows = (phys_length / SZ_4K) / format->phys_page_line;
	dbg("rows: 0x%x\n", rows);

	sg_mapped_size = 0;
	sg_offset = 0;
	sg_node = sg;
	size_phys = (format->phys_page_line * SZ_4K);
	size_virt = (format->virt_page_line * SZ_4K);

	/* map row by row */
	for (row = 0; row < rows; row++) {
		/* populate the phys addr */
		while (sg_node) {
			if (sg_mapped_size >= sg_node->length) {
				sg_mapped_size -= sg_node->length;
				sg_node = sg_next(sg_node);
			} else {
				sg_offset = sg_mapped_size;
				break;
			}
		}
		dbg("sg_offset: 0x%x\n", sg_offset);

		if (!sg_node) {
			pr_info("phys mem is not enough!\n");
			ret = -EINVAL;
			break;
		}

		/* excute map operation */
		mapped_size = hisi_map_tile_row(domain, iova + (size_virt * row),
			    size_phys, sg_node, sg_offset);
		if (mapped_size != size_phys) {
			WARN(1, "hisi_map_tile_row failed!\n");
			ret = -EINVAL;
			break;
		}

		dbg("mapped_size: 0x%x\n", mapped_size);

		sg_mapped_size = mapped_size + sg_offset;
		sg_offset = 0;
	};

	flush_pgtbl(hisi_domain, iova, size);

error:
	mutex_unlock(&hisi_iommu_mutex);

	dbg("[%s]-\n", __func__);

	return ret;
}

static size_t hisi_unmap_tile(struct iommu_domain *domain, unsigned long iova,
	     size_t size)
{
	return hisi_unmap_range(domain, iova, size);
}


static phys_addr_t hisi_iova_to_phys(struct iommu_domain *domain,
			    dma_addr_t iova)
{
	struct __hisi_iommu_domain *hisi_domain;
	phys_addr_t phys_addr = 0;
	uint32_t *pgtable;
	unsigned long pg_index;
	int ret;

	mutex_lock(&hisi_iommu_mutex);

	hisi_domain = domain->priv;
	if (!hisi_domain) {
		ret = -EINVAL;
		goto error;
	}

	pgtable = hisi_domain->pgtbl_addr;
	if (!pgtable) {
		ret = -EINVAL;
		goto error;
	}

	if (iova < hisi_domain->iova_start || iova > hisi_domain->iova_end) {
		pr_info("iova 0x%lx is not belongs to this domain!\n", (unsigned long)iova);
		ret = -EINVAL;
		goto error;
	}

	pg_index = (iova - hisi_domain->iova_start) >> order_base_2(SZ_4K);

	if (0 == *(pgtable + pg_index)) {
		pr_err("iova 0x%lx is illegal!\n", (unsigned long)iova);
		goto error;
	}

	phys_addr = (*(pgtable + pg_index) & HISI_IOMMU_PE_PA_MASK) +
						(iova & (SZ_4K - 1));

error:
	mutex_unlock(&hisi_iommu_mutex);

	dbg("[%s]iova: 0x%lx, phys: 0x%x\n", __func__, (unsigned long)iova, phys_addr);
	return phys_addr;
}

static int hisi_get_pgtbl_base(struct iommu_domain *domain,
	unsigned long iova_start, unsigned long *ptb_base,
	unsigned long *iova_base)
{
	phys_addr_t ptb_addr;
	struct __hisi_iommu_domain *hisi_domain = domain->priv;

	dbg("iova_start: 0x%lx, hisi_domain->iova_start: 0x%lx\n",
				iova_start, hisi_domain->iova_start);

	/* calculate page entry phys address */
	ptb_addr = hisi_iommu_ptable_addr;
	ptb_addr += (((iova_start - hisi_domain->iova_start) >> PAGE_SHIFT) * 4);

	/* attach addr should align to 128Byte */
	if (ptb_addr & (SZ_128 - 1)) {
		iova_start -= (((ptb_addr & (SZ_128 - 1)) >> 2) * SZ_4K);
			ptb_addr &= ~(SZ_128 - 1);
	}
	/* output the result */
	*ptb_base = ptb_addr;
	*iova_base = iova_start;

	return 0;
}

static struct iommu_ops hisi_iommu_ops = {
	.domain_init = hisi_domain_init,
	.domain_destroy = hisi_domain_destroy,

	.map = hisi_map,
	.unmap = hisi_unmap,

	.map_range = hisi_map_range,
	.unmap_range = hisi_unmap_range,

	.map_tile = hisi_map_tile,
	.unmap_tile = hisi_unmap_tile,

	.iova_to_phys = hisi_iova_to_phys,
	.get_pgtbl_base = hisi_get_pgtbl_base,

	.pgsize_bitmap = SZ_4K,
};

static int hisi_iommu_probe(struct platform_device *pdev)
{
	int ret = 0;
	ret = iommu_init();
	if (ret)
		return ret;
	device_init();

	bus_set_iommu(&platform_bus_type, &hisi_iommu_ops);
	return 0;
}

static u64 smmu_dmamask = DMA_BIT_MASK(32);
static struct platform_device hisi_iommu_device = {
	.name	= "hisi-iommu",
	.id	= 0,
	.dev	= {
		/* dma_mask and conherent_dma_mask mean that what range of phy
		 * mem this device can access.dma_mask and coherent_dma_mask
		 * is also the largest phy_address for device to access.The sys
		 * will allocate mem for the device depending on such parameters.
		 * dma_mask: used when dma'able device
		 * coherent_dma_mask:like dma_mask, but for alloc_coherent
		 * mapping and not all hardware supports 64bit addresses for
		 * consistent allocations such descriptors.
		 */
		.dma_mask = &(smmu_dmamask),
		.coherent_dma_mask = DMA_BIT_MASK(32),
	}
};

static struct platform_driver hisi_iommu_driver = {
	.probe		= hisi_iommu_probe,
	.driver		= {
		.owner		= THIS_MODULE,
		.name		= "hisi-iommu",
	}
};

#ifdef CONFIG_HISI_IOMMU_TEST
extern int init_iommt_test(void);
#else
static inline int init_iommt_test(void){return 0; }
#endif

static int __init hisi_iommu_init(void)
{
	int ret;

	ret = platform_device_register(&hisi_iommu_device);
	if (ret) {
		pr_info("%s : register device failed,ret:%d\n", __func__, ret);
		return ret;
	}

	ret = platform_driver_register(&hisi_iommu_driver);
	if (ret) {
		pr_info("%s : register driver failed,ret:%d\n", __func__, ret);
		platform_device_unregister(&hisi_iommu_device);
		return ret;
	}

	dbg("[%s]hisi iommu init done!\n", __func__);

	/* init the iommu test module */
	init_iommt_test();

	return 0;
}

static void __exit hisi_iommu_exit(void)
{
	platform_driver_unregister(&hisi_iommu_driver);
	platform_device_unregister(&hisi_iommu_device);
}

subsys_initcall(hisi_iommu_init);
module_exit(hisi_iommu_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("l00196665");
