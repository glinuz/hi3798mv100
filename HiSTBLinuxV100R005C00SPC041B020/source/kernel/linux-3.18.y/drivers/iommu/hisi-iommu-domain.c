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
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/genalloc.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/sizes.h>
#include <linux/iommu.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/hisi/hisi-iommu.h>

#ifdef DEBUG
#define dbg(format, arg...)    \
	do {                 \
		pr_info("[iommu]"format, ##arg); \
	} while (0)
#else
#define dbg(format, arg...)
#endif

#define ADDR_INVALID	(0)
#define ADDR_INVALID2	(~0)
#define RESERVE_SIZE	0x100000

struct hisi_iommu_debug_info {
	unsigned int alloc_iova_count;
	unsigned int free_iova_count;
} dbg_inf;

struct map_result {
	unsigned long iova_start;
	unsigned long iova_size;
	unsigned long iommu_ptb_base;
	unsigned long iommu_iova_base;
};

struct hisi_iommu_domain {
	struct iommu_domain *domain;
	struct gen_pool *iova_pool;
};

static struct hisi_iommu_domain *hisi_iommu_domain_p;

DEFINE_MUTEX(iova_pool_mutex);

/**
 * get the whole size of genpool
 */
static size_t hisi_iova_size(struct gen_pool *pool)
{
	size_t size;
	mutex_lock(&iova_pool_mutex);
	size = gen_pool_size(pool);
	mutex_unlock(&iova_pool_mutex);
	return size;
}

size_t hisi_iommu_iova_size(void)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	return hisi_iova_size(hisi_domain->iova_pool);
}
EXPORT_SYMBOL_GPL(hisi_iommu_iova_size);


/**
 * get the available size of genpool
 */
static size_t hisi_iova_available(struct gen_pool *pool)
{
	size_t avail;
	mutex_lock(&iova_pool_mutex);
	avail = gen_pool_avail(pool);
	mutex_unlock(&iova_pool_mutex);
	return avail;
}

static void dump_chunk(struct gen_pool *pool,
		struct gen_pool_chunk *chunk, void *data)
{
	int i;
	int nbits = 0;
	int nlong = BITS_TO_LONGS(nbits);

	nbits = (chunk->end_addr - chunk->start_addr) >> pool->min_alloc_order;
	pr_info("chunk allocate map: nlong: %d\n", nlong);
	for (i = nlong; i > 0; i -= 4) {
		pr_info(" %08lx %08lx %08lx %08lx\n",
			chunk->bits[i-1],
			chunk->bits[i-2],
			chunk->bits[i-3],
			chunk->bits[i-4]);
	}
}

size_t hisi_iommu_iova_available(void)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	gen_pool_for_each_chunk(hisi_domain->iova_pool, dump_chunk, NULL);
	return hisi_iova_available(hisi_domain->iova_pool);
}
EXPORT_SYMBOL_GPL(hisi_iommu_iova_available);


/**
 * free a rage of space back to genpool
 */
static void hisi_free_iova(struct gen_pool *pool,
		unsigned long iova, size_t size)
{
	dbg("[%s]iova: 0x%lx, size: 0x%x\n", __func__, iova, size);
	mutex_lock(&iova_pool_mutex);
	gen_pool_free(pool, iova, size);

	dbg_inf.free_iova_count++;

	mutex_unlock(&iova_pool_mutex);
}

void hisi_iommu_free_iova(unsigned long iova, size_t size)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	hisi_free_iova(hisi_domain->iova_pool, iova, size);
}
EXPORT_SYMBOL_GPL(hisi_iommu_free_iova);


/**
 * get a rage of space from genpool
 */
static unsigned long hisi_alloc_iova(struct gen_pool *pool,
		unsigned long size, unsigned long align)
{
	unsigned long iova = 0;

	mutex_lock(&iova_pool_mutex);

	iova = gen_pool_alloc(pool, size);

	dbg("[%s]iova: 0x%lx, size: 0x%lx\n", __func__, iova, size);

	dbg_inf.alloc_iova_count++;

	mutex_unlock(&iova_pool_mutex);
	return iova;
}

unsigned long hisi_iommu_alloc_iova(size_t size, unsigned long align)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	return hisi_alloc_iova(hisi_domain->iova_pool, size, align);
}
EXPORT_SYMBOL_GPL(hisi_iommu_alloc_iova);


/**
 * map range on hisi iommu domain.
 */
int hisi_iommu_map_range(unsigned long iova_start, struct scatterlist *sgl,
		unsigned long iova_size)
{
	struct iommu_domain *domain;
	if ((NULL == hisi_iommu_domain_p) ||
			(NULL == hisi_iommu_domain_p->domain)) {
		pr_err("hisi iommu domain not setup\n");
		return -ENODEV;
	}
	domain = hisi_iommu_domain_p->domain;
	return iommu_map_range(domain, iova_start, sgl, iova_size, 0);
}
/**
 * map range on hisi iommu domain.
 */
int hisi_iommu_unmap_range(unsigned long iova_start,
		unsigned long iova_size)
{
	struct iommu_domain *domain;
	if ((NULL == hisi_iommu_domain_p) ||
			(NULL == hisi_iommu_domain_p->domain)) {
		pr_err("hisi iommu domain not setup\n");
		return -ENODEV;
	}
	domain = hisi_iommu_domain_p->domain;
	return iommu_unmap_range(domain, iova_start, iova_size);
}


static struct gen_pool *iova_pool_setup(unsigned long start,
		unsigned long end, unsigned long align)
{
	struct gen_pool *pool = NULL;
	int ret = 0;

	pool = gen_pool_create(order_base_2(align), -1);
	if (!pool) {
		pr_err("Create gen pool failed!\n");
		return NULL;
	}

	ret = gen_pool_add(pool, start, (end - start), -1);
	if (ret) {
		pr_err("Gen pool add failed!\n");
		gen_pool_destroy(pool);
		return NULL;
	}

	return pool;
}

static void iova_pool_destory(struct gen_pool *pool)
{
	gen_pool_destroy(pool);
}

int hisi_iommu_get_info(unsigned int *iova_start, unsigned int *pgtbl_base)
{
	struct iommu_domain_capablity data;
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;

	if (!hisi_domain || iommu_domain_get_attr(hisi_domain->domain,
					DOMAIN_ATTR_CAPABLITY, &data)) {
		return 1;
	}
	*iova_start = data.iova_start;
	*pgtbl_base = data.pgtbl_base;

	return 0;
}
EXPORT_SYMBOL_GPL(hisi_iommu_get_info);

static int __hisi_iommu_domain_map(struct scatterlist *sgl,
		struct tile_format *format, struct map_result *result)
{
	int ret;
	unsigned long phys_len, iova_size;
	unsigned long iova_start;
	struct hisi_iommu_domain *hisi_domain;
	struct gen_pool *pool;
	struct iommu_domain *domain;
	struct scatterlist *sg;
	struct iommu_domain_capablity data;

	/* calculate whole phys mem length */
	for (phys_len = 0, sg = sgl; sg; sg = sg_next(sg)) {
		phys_len += (unsigned long)ALIGN(sg->length, SZ_4K);
	}

	/* get iova lenth needed */
	if (format->is_tile) {
		unsigned long lines;

		lines = phys_len / (format->phys_page_line * SZ_4K);
		iova_size = lines * format->virt_page_line * SZ_4K;
	} else {
		iova_size = phys_len;
	}

	/* we use a extra page as a gap between adjacent iommu addr space.
	 * for map_range
	 */
	if (!format->is_tile)
		iova_size = iova_size + SZ_4K;

	/* alloc iova */
	hisi_domain = hisi_iommu_domain_p;
	pool = hisi_domain->iova_pool;
	domain = hisi_domain->domain;

	/* we need some iommu's attribution */
	if (iommu_domain_get_attr(hisi_domain->domain,
				DOMAIN_ATTR_CAPABLITY, &data)) {
		pr_info("iommu_domain_get_attr is failed\n");
		return -EINVAL;
	}
	iova_start = hisi_alloc_iova(pool, iova_size, data.iova_align);
	if (ADDR_INVALID == iova_start) {
		pr_info("[%s]hisi_alloc_iova alloc 0x%lx failed!\n", __func__,
								iova_size);
		pr_info("[%s]dump iova pool begain--------------\n", __func__);
		pr_info("iova available: 0x%lx\n", (unsigned long)hisi_iommu_iova_available());
		pr_info("alloc count: %d, free count: %d\n",
			dbg_inf.alloc_iova_count, dbg_inf.free_iova_count);
		pr_info("[%s]dump iova pool end   ---------------\n", __func__);
		return -EINVAL;
	}

	/* out put result */
	result->iova_start = iova_start;

	/* we alloc and map iommu buffer size one page size larger then
	 * requeseted just for avoiding  reading or writing out of mem
	 * boundary.for map_range
	 */
	if (!format->is_tile)
		result->iova_size = iova_size - SZ_4K;
	else
		result->iova_size = iova_size;

	/* do map */
	if (format->is_tile) {
		dbg("to map tile\n");
		ret = iommu_map_tile(domain, iova_start, sgl, iova_size, 0,
				format);
	} else {
		dbg("to map range\n");
		ret = iommu_map_range(domain, iova_start, sgl,
						(size_t)iova_size, 0);
	}
	if (ret) {
		pr_err("[%s]map failed!\n", __func__);
		hisi_free_iova(pool, iova_start, iova_size);
		return ret;
	}

	return 0;
}

int hisi_iommu_map_domain(struct scatterlist *sgl,
		struct iommu_map_format *format)
{
	int ret;
	struct tile_format fmt;
	struct map_result result;
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;

	memset(&result, 0, sizeof(result));

	dbg("[%s]+\n", __func__);

	fmt.is_tile = format->is_tile;
	fmt.phys_page_line = format->phys_page_line;
	fmt.virt_page_line = format->virt_page_line;

	ret = __hisi_iommu_domain_map(sgl, &fmt, &result);

	format->iova_start = result.iova_start;
	format->iova_size = result.iova_size;

	/* get value which write into iommu register */
	ret = iommu_get_pgtbl_base(hisi_domain->domain, format->iova_start,
			&(format->iommu_ptb_base), &(format->iommu_iova_base));

	if (0 != ret) {
		dbg("get format is failed\n");
		return ret;
	}

	dbg("[%s]-\n", __func__);
	return ret;
}
EXPORT_SYMBOL_GPL(hisi_iommu_map_domain);

int __hisi_iommu_domain_unmap(struct map_result *result)
{
	unsigned long unmaped_size;
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	struct gen_pool *pool = hisi_domain->iova_pool;

	/* unmap tile equals to unmpa range */
	unmaped_size = iommu_unmap_range(hisi_domain->domain,
			result->iova_start, result->iova_size);
	if (unmaped_size != result->iova_size) {
		pr_err("[%s]unmap failed!\n", __func__);
		return -EINVAL;
	}

	/* free iova */
	hisi_free_iova(pool, result->iova_start, result->iova_size);

	return 0;
}

#ifdef CONFIG_ARM64_64K_PAGES
#error hisi iommu can not deal with 64k pages!
#endif

int hisi_iommu_unmap_domain(struct iommu_map_format *format)
{
	struct map_result result;

	result.iova_start = format->iova_start;
	result.iova_size = format->iova_size;

	/* the gap between buffer should be unmap and free ,for map_range  */
	if (!format->is_tile)
		result.iova_size = result.iova_size + SZ_4K;

	return __hisi_iommu_domain_unmap(&result);
}
EXPORT_SYMBOL_GPL(hisi_iommu_unmap_domain);

phys_addr_t hisi_iommu_domain_iova_to_phys(unsigned long iova)
{
	struct iommu_domain *domain = hisi_iommu_domain_p->domain;
	return iommu_iova_to_phys(domain, iova);
}
EXPORT_SYMBOL_GPL(hisi_iommu_domain_iova_to_phys);

unsigned int hisi_iommu_page_size(void)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	struct iommu_domain_capablity data;
	if (iommu_domain_get_attr(hisi_domain->domain,
					DOMAIN_ATTR_CAPABLITY, &data)) {
		pr_err("here return 0!!!!!!!\n");
		return 0;
	}

	pr_info("here return size %x\n", data.pg_sz);
	return data.pg_sz;
}
EXPORT_SYMBOL_GPL(hisi_iommu_page_size);

bool hisi_iommu_off_on(void)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;
	struct iommu_domain_capablity data;
	if (iommu_domain_get_attr(hisi_domain->domain,
					DOMAIN_ATTR_CAPABLITY, &data)) {
		return false;
	}

	return data.off_on;
}
EXPORT_SYMBOL_GPL(hisi_iommu_off_on);

int hisi_iommu_open(struct inode *inode, struct file *file)
{
	return 0;
}

int hisi_iommu_map(struct file *file, struct vm_area_struct *vma)
{
	unsigned long iova = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long addr = vma->vm_start;
	unsigned long phy_addr;

	while (addr < vma->vm_end) {
		/* iova should be aligned with SZ_4K  */
		phy_addr = hisi_iommu_domain_iova_to_phys(iova);
		/* iova is illegal  */
		if ((!phy_addr))
			return -1;
		remap_pfn_range(vma, addr, __phys_to_pfn(phy_addr), SZ_4K,
					vma->vm_page_prot);
		addr = addr + SZ_4K;
		iova = iova + SZ_4K;
	}
	return 0;
}

int hisi_iommu_release(struct inode *inode, struct file *file)
{
	return 0;
}

/* for smmu tool    */
static struct file_operations hisi_iommu_fops = {
	.owner		= THIS_MODULE,
	.open		= hisi_iommu_open,
	.release	= hisi_iommu_release,
	.mmap		= hisi_iommu_map,
};

static struct miscdevice smmu_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= "hi_smmu",
	.fops		= &hisi_iommu_fops,
};

static int __init hisi_iommu_domain_init(void)
{
	int ret;
	struct iommu_domain_capablity data;
	struct hisi_iommu_domain *hisi_domain;
	unsigned long reserve_start = ADDR_INVALID2;

	pr_info("in %s start \n", __func__);
	hisi_domain = kzalloc(sizeof(*hisi_domain), GFP_KERNEL);
	if (!hisi_domain) {
		return -ENOMEM;
	}

	/* create iommu domain */
	hisi_domain->domain = iommu_domain_alloc(&platform_bus_type);
	if (!hisi_domain->domain) {
		ret = -EINVAL;
		goto error;
	}

	/* init the iova pool */
	if (iommu_domain_get_attr(hisi_domain->domain, DOMAIN_ATTR_CAPABLITY,
								&data)) {
		ret = -EINVAL;
		goto error;
	}

	/* align mean in this pool allocation buffer is aligned
	 * by iommu align request
	 */
	hisi_domain->iova_pool = iova_pool_setup(data.iova_start,
			data.iova_end, data.iova_align);
	if (!hisi_domain->iova_pool) {
		ret = -EINVAL;
		goto error;
	}

	/*
	 * Smmu address space of 0x0---0xfffff should be reserved,it can not
	 * be used by logic. And the firt smmu address in this reserved area
	  * must be 0x0, it cannot be used by peripheral.
	 */
	reserve_start = hisi_alloc_iova(hisi_domain->iova_pool, RESERVE_SIZE,
						data.iova_align);
	if (0 != reserve_start) {
		pr_info("[%s] the first smmu area reserve 0x%x failed!\n",
				__func__, RESERVE_SIZE);
		ret = -EINVAL;
		goto error;
	}

	/* this is a global pointer */
	hisi_iommu_domain_p = hisi_domain;

	/* register miscdevice */
	ret = misc_register(&smmu_device);
	if (ret) {
		pr_info("smmu: failed to register misc device.\n");
		goto error;
	}

	pr_info("in %s end \n", __func__);
	return 0;

error:
	WARN(1, "hisi_iommu_domain_init failed!\n");
	if (reserve_start != ADDR_INVALID2)
		hisi_free_iova(hisi_domain->iova_pool, reserve_start,
							RESERVE_SIZE);
	if (hisi_domain->iova_pool)
		iova_pool_destory(hisi_domain->iova_pool);
	if (hisi_domain->domain)
		iommu_domain_free(hisi_domain->domain);
	if (hisi_domain)
		kfree(hisi_domain);

	return ret;
}

static void __exit hisi_iommu_domain_exit(void)
{
	struct hisi_iommu_domain *hisi_domain = hisi_iommu_domain_p;

	misc_deregister(&smmu_device);
	if (hisi_domain->iova_pool) {
		hisi_free_iova(hisi_domain->iova_pool, 0x0, RESERVE_SIZE);
		iova_pool_destory(hisi_domain->iova_pool);
	}
	if (hisi_domain->domain)
		iommu_domain_free(hisi_domain->domain);
	if (hisi_domain)
		kfree(hisi_domain);

	hisi_iommu_domain_p = NULL;
}

subsys_initcall(hisi_iommu_domain_init);
module_exit(hisi_iommu_domain_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("l00196665");
