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
#include <linux/string.h>
#include <linux/list.h>
#include <asm/cacheflush.h>
#include <linux/time.h>
#include <asm/setup.h>

#include <asm/memory.h>

#include <linux/dma-mapping.h>
#include <asm/memory.h>

#ifndef CONFIG_64BIT
#include <linux/highmem.h>
#endif

#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/seq_file.h>
#include <linux/err.h>
#include "drv_media_mem.h"
#include "hi_kernel_adapt.h"
#include "hi_debug.h"

#define DEFAULT_ALLOC 0
#define SLAB_ALLOC 1
#define EQ_BLOCK_ALLOC 2

#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

#define MMZ_DBG_LEVEL 0x0
#define mmz_trace(level, s, params...) do { if (level & MMZ_DBG_LEVEL) \
		PRINTK_CA(KERN_INFO "[%s, %d]: " s "\n", \
			__FUNCTION__, __LINE__, params);\
} while (0)
#define mmz_trace_func() mmz_trace(0x02, "%s", __FILE__)

#define MMZ_GRAIN PAGE_SIZE
#define mmz_bitmap_size(p) (mmz_align2(mmz_length2grain((p)->nbytes), 8) / 8)

#define mmz_get_bit(p, n) (((p)->bitmap[(n) / 8] >> ((n) & 0x7)) & 0x1)
#define mmz_set_bit(p, n) (p)->bitmap[(n) / 8] |= 1 << ((n) & 0x7)
#define mmz_clr_bit(p, n) (p)->bitmap[(n) / 8] &= ~(1 << ((n) & 0x7))

#define mmz_pos2phy_addr(p, n) ((p)->phys_start + (n) * MMZ_GRAIN)
#define mmz_phy_addr2pos(p, a) (((a) - (p)->phys_start) / MMZ_GRAIN)

#define mmz_align2low(x, g) (((x) / (g)) * (g))
#define mmz_align2(x, g) ((((x) + (g) - 1) / (g)) * (g))
#define mmz_grain_align(x) mmz_align2(x, MMZ_GRAIN)
#define mmz_length2grain(len) (mmz_grain_align(len) / MMZ_GRAIN)

#define begin_list_for_each_mmz(p, gfp, mmz_name) list_for_each_entry(p, &mmz_list, list) {\
        if (gfp == 0 ? 0 : (p)->gfp != (gfp)) \
            continue;\
        if (mmz_name != NULL) { \
            if ((*mmz_name != '\0') && strncmp(mmz_name, p->name,HIL_MAX_NAME_LEN)) \
                continue;\
        } \
        if ((mmz_name == NULL) && (anony == 1)){ \
            if (strncmp("anonymous", p->name,HIL_MAX_NAME_LEN)) \
                continue;\
        } \
        mmz_trace(1, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(p));

#define end_list_for_each_mmz() }

char line[COMMAND_LINE_SIZE];



int zone_number = 0;
int block_number = 0;

//the following 3 definations are duplicate from kernel 
//because no where to locate them

#define NAME_LEN_MAX	64

struct iommu_zone {
	unsigned int  iova_start;
	unsigned int  iova_end;
	unsigned int  iova_align;
};

#ifdef CONFIG_HISI_IOMMU
extern struct iommu_zone *hisi_get_iommu_zone(void);
#else
struct iommu_zone hisi_iommu;
static inline struct iommu_zone *hisi_get_iommu_zone(void)
{
	hisi_iommu.iova_start = 0x100000;
	hisi_iommu.iova_end = 0xffefffff;
	hisi_iommu.iova_align = 4096;

	return &hisi_iommu;
}
#endif


#define ZONE_MAX                64
struct mmz_iommu mmz_iommu;

LIST_HEAD(mmz_list);
HI_DECLARE_MUTEX(mmz_lock);

static int anony = 0;

module_param(anony, int, S_IRUGO);

static void __dma_clear_buffer(struct ion_handle *handle);

static struct mmz_iommu *get_mmz_iommu_root(void)
{
	return &mmz_iommu;
}

hil_mmz_t *hil_mmz_create(const char *name, HI_U32 gfp,
			HI_U32 zone_start, HI_U32 nbytes)
{
	hil_mmz_t *p = NULL;

	mmz_trace_func();
	if (name == NULL) {
		PRINTK_CA(KERN_ERR "%s: 'name' can not be zero!", __func__);
		return NULL;
	}

	p = kmalloc(sizeof(hil_mmz_t) + 1, GFP_KERNEL);
	if (p == NULL) {
		return NULL;
	}

	memset(p, 0, sizeof(hil_mmz_t) + 1);
	strlcpy(p->name, name, HIL_MAX_NAME_LEN);
	p->gfp = gfp;
	p->zone_start = zone_start;
	p->nbytes = nbytes;

	INIT_LIST_HEAD(&p->list);

	p->destructor = kfree;

	return p;
}

void flush_inner_cache(void *viraddr, unsigned int len)
{
	if ((NULL == viraddr) || (0 == len)) {
		HI_PRINT("%s failed, viraddr:%p len:0x%x!\n", __func__, \
							viraddr, len);
		return;
	}

	if (len > L2_CACHE_SIZE)
#ifdef CONFIG_SMP
		on_each_cpu((smp_call_func_t)mmz_flush_dcache_all, NULL, 1);
#else
		mmz_flush_dcache_all();
#endif
	else
		mmz_flush_dcache_area((void *)viraddr,(size_t)len);
}

#ifndef CONFIG_64BIT
static void flush_outer(hil_mmb_t *mmb)
{
	struct scatterlist *sg;
	int i;
	unsigned long size;
	struct sg_table *table;
	struct mmz_iommu *common = &mmz_iommu;

	table = get_pages_from_buffer(common->client, mmb->handle, &size);
	if (!table) {
		HI_PRINT("%s: get pages failed!\n", __func__);
		return;
	}

	for_each_sg(table->sgl, sg, table->nents, i) {
		struct page *page = sg_page(sg);
		HI_U32 len = PAGE_ALIGN(sg->length);
		HI_U32 phys = __pfn_to_phys(page_to_pfn(page));

		outer_flush_range(phys, phys + len);
	}
}

/* just for A9    */
void flush_outer_cache_range(mmb_addr_t phyaddr, mmb_addr_t len,
					unsigned int iommu)
{
	hil_mmb_t *mmb;

	if (!iommu) {
		outer_flush_range(phyaddr, phyaddr + len);
		return;
	}

	mmb = hil_mmb_getby_phys(phyaddr, iommu);
	if (NULL == mmb) {
		HI_PRINT("%s:invalid args!\n", __func__);
		return;
	}

	if (!mmb->iommu) {
		outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
	} else {
		flush_outer(mmb);
	}
}
#else
void flush_outer_cache_range(mmb_addr_t phyaddr, mmb_addr_t len,
						unsigned int iommu)
{
	return;
}
#endif

int hil_mmz_destroy(hil_mmz_t *zone)
{
	if (zone == NULL) {
		return -1;
	}

	if (zone->destructor) {
		zone->destructor(zone);
	}

	return 0;
}


int hil_mmz_register(hil_mmz_t *zone)
{
	mmz_trace(1, HIL_MMZ_FMT_S, hil_mmz_fmt_arg(zone));

	if (zone == NULL) {
		return -1;
	}

	down(&mmz_lock);

	list_add(&zone->list, &mmz_list);

	up(&mmz_lock);

	return 0;
}

int hil_mmz_unregister(hil_mmz_t *zone)
{
	int losts = 0;
	hil_mmb_t *p = NULL;
	struct rb_node *n;

	if (zone == NULL) {
		return -1;
	}

	mmz_trace_func();

	down(&mmz_lock);

	for (n = rb_first(&zone->root); n; n = rb_next(n)) {
		if (zone->iommu)
			p = rb_entry(n, hil_mmb_t, s_node);
		else
			p = rb_entry(n, hil_mmb_t, node);

		PRINTK_CA(KERN_WARNING "Lost: " HIL_MMB_FMT_S "\n",
							hil_mmb_fmt_arg(p));
		losts++;
	}

	if (losts) {
		PRINTK_CA(KERN_ERR "  %d mmbs not free, mmz<%s> can not be \
					deregistered!\n", losts, zone->name);
		up(&mmz_lock);
		return -1;
	}

	list_del(&zone->list);
	hil_mmz_destroy(zone);
	up(&mmz_lock);

	return 0;
}

hil_mmb_t *hil_mmb_getby_phys(HI_U32 addr, HI_U32 iommu)
{
	hil_mmz_t *zone;
	hil_mmb_t *mmb = NULL;

	if (addr == 0) {
		HI_PRINT("%s %d err args\n", __func__, __LINE__);
		return NULL;
	}

	down(&mmz_lock);
	list_for_each_entry(zone, &mmz_list, list) {
		struct rb_node *n;

		if (zone->iommu != iommu)
			continue;

		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m = NULL;
			if (!iommu) {
				m = rb_entry(n, hil_mmb_t, node);
				if (m->phys_addr == addr) {
					mmb = m;
					goto result;
				}
			} else {
				m = rb_entry(n, hil_mmb_t, s_node);
				if (m->iommu_addr == addr) {
					mmb = m;
					goto result;
				}
			}
		}
	}
result:
	up(&mmz_lock);
	if (NULL == mmb) {
		if (iommu)
			HI_PRINT("%s smmu:0x%x err args\n", __func__, addr);
		else
			HI_PRINT("%s phys:0x%x err args\n", __func__, addr);
#ifndef HI_ADVCA_FUNCTION_RELEASE
		dump_stack();
#endif
	}
	return mmb;

}

hil_mmb_t *hil_mmb_getby_sec_addr(HI_U32 sec_addr, HI_U32 iommu)
{
	hil_mmz_t *zone;
	hil_mmb_t *mmb = NULL;

	if (sec_addr == 0) {
		HI_PRINT("%s %d err args\n", __func__, __LINE__);
		return NULL;
	}
	down(&mmz_lock);
	list_for_each_entry(zone, &mmz_list, list) {
		struct rb_node *n;

		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m = NULL;
			if (!iommu) {
				/* is sec_addr is phys_addr, so it just in
				 * ddr zone
				 */
				m = rb_entry(n, hil_mmb_t, node);
				if (m->phys_addr == sec_addr) {
					mmb = m;
					goto result;
				}
			} else {
				/* if sec_addr is sec_smmu, then it maybe both
				 * in ddr zone and smmu zone*/
				m = rb_entry(n, hil_mmb_t, s_node);
				if (m->sec_smmu == sec_addr) {
					mmb = m;
					goto result;
				}
				m = rb_entry(n, hil_mmb_t, node);
				if (m->sec_smmu == sec_addr) {
					mmb = m;
					goto result;
				}
			}
		}
	}
result:
	up(&mmz_lock);
	if (NULL == mmb) {
		if (iommu)
			HI_PRINT("%s smmu:0x%x err args\n", __func__, sec_addr);
		else
			HI_PRINT("%s phys:0x%x err args\n", __func__, sec_addr);

	}
	return mmb;

}

static void mmb_add_to_rbtree(hil_mmb_t *mmb, hil_mmz_t *mmz, int iommu)
{
	struct rb_node **p = &mmz->root.rb_node;
	struct rb_node *parent = NULL;
	hil_mmb_t *entry;

	if (iommu) {
		while (*p) {
			parent = *p;
			entry = rb_entry(parent, hil_mmb_t, s_node);

			if (mmb->iommu_addr < entry->iommu_addr) {
				p = &(*p)->rb_left;
			} else if (mmb->iommu_addr > entry->iommu_addr) {
				p = &(*p)->rb_right;
			} else {
				HI_PRINT("%s: buffer already found.\n",
								__func__);
				BUG();
				return;
			}
		}
		rb_link_node(&mmb->s_node, parent, p);
		rb_insert_color(&mmb->s_node, &mmz->root);
	} else {
		while (*p) {
			parent = *p;
			entry = rb_entry(parent, hil_mmb_t, node);

			if (mmb->phys_addr < entry->phys_addr) {
				p = &(*p)->rb_left;
			} else if (mmb->phys_addr > entry->phys_addr) {
				p = &(*p)->rb_right;
			} else {
				HI_PRINT("%s: buffer already found.\n",
								__func__);
				BUG();
				return;
			}
		}

		rb_link_node(&mmb->node, parent, p);
		rb_insert_color(&mmb->node, &mmz->root);
	}
}

hil_mmb_t *hil_mmb_alloc(const char *name, HI_U32 size,
			HI_U32 align, const char *mmz_name, int flag)
{
	hil_mmb_t *mmb;
	hil_mmz_t *p;
	hil_mmz_t *mmz = NULL;
	unsigned int heap_id_mask;
	int iommu;
	unsigned int flags = 0;
	struct mmz_iommu *common = get_mmz_iommu_root();
	struct ion_client *client = common->client;
	int ret;
	struct iommu_map_format *format;
	HI_U32 gfp = 0;  /* gfp is always 0 in later version */

	down(&mmz_lock);

	begin_list_for_each_mmz(p, gfp, mmz_name)
		mmz = p;
	end_list_for_each_mmz()

	if (NULL == mmz) {
		HI_PRINT("can't find zone:%s\n", mmz_name);
		up(&mmz_lock);
		return NULL;
	}
	heap_id_mask = mmz->heap_id_mask;
	iommu = mmz->iommu;

	mmb = kmalloc(sizeof(hil_mmb_t), GFP_KERNEL);
	if (!mmb) {
		up(&mmz_lock);
		return NULL;
	}
	memset(mmb, 0, sizeof(hil_mmb_t));

	/* alloc real page buffer via ion interface  */
	mmb->handle = ion_alloc(client, size, align, heap_id_mask, flags);
	if (IS_ERR(mmb->handle)) {
		HI_PRINT("%s :mem alloc failed !\n", __func__);
		goto handle_err;
	}
	__dma_clear_buffer(mmb->handle);

	if (iommu) {
		format = kzalloc(sizeof(struct iommu_map_format), GFP_KERNEL);
		if (NULL == format) {
			HI_PRINT("no mem!\n");
			goto err;
		}
		/* map real page buffer to smmu space   */
		ret = ion_map_iommu(client, mmb->handle, format);
		if (ret) {
			HI_PRINT("alloc iommu failed!\n");
			kfree(format);
			goto err;
		}

		mmb->phys_addr = MMB_ADDR_INVALID;
		mmb->iommu_addr = format->iova_start;
		mmb->length = format->iova_size;
		kfree(format);
		format = NULL;
	} else {
		size_t len;
		unsigned long phys;
		/* get cma phy adress and len of real page buffer   */
		ret = ion_phys(client, mmb->handle, &phys,
							&len);
		if (ret) {
			HI_PRINT("cannot get phys_addr!\n");
			goto err;
		}

		mmb->phys_addr = (HI_U32)phys;
		mmb->iommu_addr = MMB_ADDR_INVALID;
		mmb->length = (HI_U32)len;
	}

	if (name) {
		strlcpy(mmb->name, name, HIL_MAX_NAME_LEN);
	} else {
		strlcpy(mmb->name, "<null>", HIL_MAX_NAME_LEN);
	}

	mmb->zone = mmz;
	mmb->iommu = iommu;
	mmb->phy_ref++;
	mmb->flag = flag;		/* used when release      */
	mmb->kdata = NULL;
	mmb->client = client;
	mmb->sec_smmu = MMB_ADDR_INVALID;	/* for secure smmu  */

	INIT_LIST_HEAD(&mmb->ulist);
	mmb_add_to_rbtree(mmb, mmz, iommu);

	up(&mmz_lock);

	return mmb;

err:
	ion_free(client, mmb->handle);

handle_err:
	kfree(mmb);
	up(&mmz_lock);
	return NULL;
}

static void __dma_clear_buffer(struct ion_handle *handle)
{
	struct scatterlist *sg;
	int i;
	HI_U32 size;
	struct sg_table *table;
	struct mmz_iommu *common = &mmz_iommu;
	unsigned long len;

	table = get_pages_from_buffer(common->client, handle, &len);
	if (!table) {
		HI_PRINT("%s: get pages failed!\n", __func__);
		return;
	}

	size = (HI_U32)len;
	size = PAGE_ALIGN(size);
	if (size < L2_CACHE_SIZE) {
		for_each_sg(table->sgl, sg, table->nents, i) {
			struct page *page = sg_page(sg);
			HI_U32 length = PAGE_ALIGN(sg->length);
#ifdef CONFIG_64BIT
			void *ptr = page_address(page);
			/* memset(ptr, 0, size); */
			mmz_flush_dcache_area(ptr, length);
#else
			HI_U32 phys = __pfn_to_phys(page_to_pfn(page));

			if (PageHighMem(page)) {
				while(length > 0) {
					void *ptr = kmap_atomic(page);

					/* memset(ptr, 0, PAGE_SIZE); */
					mmz_flush_dcache_area(ptr, PAGE_SIZE);
					__kunmap_atomic(ptr);
					page++;
					length -= PAGE_SIZE;
				}
			} else {
				void *ptr = page_address(page);
				/* memset(ptr, 0, size); */
				mmz_flush_dcache_area(ptr, length);
			}
			outer_flush_range(phys, phys + length);
#endif
		}
	} else {
#ifdef CONFIG_SMP
	on_each_cpu((smp_call_func_t)mmz_flush_dcache_all, NULL, 1);
#else
	mmz_flush_dcache_all();
#endif

#ifndef CONFIG_64BIT
	outer_flush_all();
#endif
	}
}

static void *_map2kern(hil_mmb_t *mmb, int cached)
{
	struct scatterlist *sg;
	int i, j;
	void *vaddr;
	pgprot_t pgprot;
	int npages;
	HI_U32 size;
	struct sg_table *table;
	struct page **pages;
	struct page **tmp;
	struct mmz_iommu *common = &mmz_iommu;
	unsigned long len;

	table = get_pages_from_buffer(common->client, mmb->handle, &len);
	if (!table) {
		HI_PRINT("%s: get pages failed!\n", __func__);
		return NULL;
	}
	size = (HI_U32)len;
	npages = PAGE_ALIGN(size) / PAGE_SIZE;
	pages = vmalloc(sizeof(struct page *) * npages);
	tmp = pages;

	if (!pages) {
		HI_PRINT("%s:no mem!\n", __func__);
		return NULL;
	}
	pgprot = PAGE_KERNEL_EXEC;

	if (!cached)
	{
		pgprot = pgprot_writecombine(PAGE_KERNEL_EXEC);
	}

	for_each_sg(table->sgl, sg, table->nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);
		BUG_ON(i >= npages);
		for (j = 0; j < npages_this_entry; j++)
			*(tmp++) = page++;
	}
	vaddr = vmap(pages, npages, VM_MAP, pgprot);
	vfree(pages);
	return vaddr;
}

static void *_mmb_map2kern(hil_mmb_t *mmb, int cached)
{
	struct mmb_kdata *kdata = NULL;

	kdata = mmb->kdata;
	if (NULL != kdata) {
		if (kdata->map_cached != cached) {
			HI_PRINT("mmb<%s> already kernel-mapped one \
				cache attr, now another cache attr re-mapped \
				requested.But the first remap is returned\n",
				 mmb->name);
		}
		mmb->phy_ref++;
		mmb->map_ref++;
		kdata->kmap_ref++;
		return kdata->kvirt;
	}
	kdata = kzalloc(sizeof(struct mmb_kdata), GFP_KERNEL);
	if (NULL == kdata) {
		HI_PRINT("%s : remap failed!\n", __func__);
		return NULL;
	}
	kdata->kvirt = _map2kern(mmb, cached);

	if (NULL == kdata->kvirt) {
		HI_PRINT("remap failed in ion!\n");
		kfree(kdata);
		return NULL;
	}
	kdata->map_cached = cached;
	kdata->kmap_ref++;

	mmb->kdata = kdata;
	mmb->phy_ref++;
	mmb->map_ref++;

	return kdata->kvirt;
}

void *hil_mmb_map2kern(hil_mmb_t *mmb)
{
    void *p;

    if (mmb == NULL)
    {
        return NULL;
    }

    down(&mmz_lock);
    p = _mmb_map2kern(mmb, 0);
    up(&mmz_lock);

    return p;
}

void *hil_mmb_map2kern_cached(hil_mmb_t *mmb)
{
    void *p;

    if (mmb == NULL)
    {
        return NULL;
    }

    down(&mmz_lock);
    p = _mmb_map2kern(mmb, 1);
    up(&mmz_lock);

    return p;
}

static int _mmb_free(hil_mmb_t *mmb);

int hil_mmb_unmap(hil_mmb_t *mmb, void *addr)
{
	struct mmb_kdata *kdata = NULL;
	mmb_addr_t phyaddr;

	if (mmb == NULL) {
		return -1;
	}
	down(&mmz_lock);

	kdata = mmb->kdata;
	if (NULL == kdata) {
		HI_PRINT("%s :cannot find userdata!\n", __func__);
		up(&mmz_lock);
		return HI_FAILURE;
	}
	if (mmb->iommu)
		phyaddr = mmb->iommu_addr;
	else
		phyaddr = mmb->phys_addr;

	if (kdata->map_cached) {
		up(&mmz_lock);
#ifndef CONFIG_64BIT
		__cpuc_flush_dcache_area((void *)kdata->kvirt,
						(size_t)mmb->length);
		flush_outer_cache_range(phyaddr, mmb->length, mmb->iommu);
#else
		__flush_dcache_area((void *)kdata->kvirt, (size_t)mmb->length);
#endif
		down(&mmz_lock);
	}

	kdata->kmap_ref--;
	if (!kdata->kmap_ref) {
		vunmap(kdata->kvirt);
		kfree(kdata);
		mmb->kdata = NULL;
	}

	mmb->map_ref--;
	mmb->phy_ref--;

	if ((0 == (mmb->phy_ref)) && (0 == mmb->map_ref) &&
		(0 == mmb->cma_smmu_ref) && (0 == mmb->sec_smmu_ref)) {
		up(&mmz_lock);
		/* free mmb   */
		hil_mmb_free(mmb);
		return 0;
	}

	up(&mmz_lock);

	return 0;
}

static int _mmb_free(hil_mmb_t *mmb)
{
	__dma_clear_buffer(mmb->handle);

	if (mmb->iommu)
		ion_unmap_iommu(mmb->client, mmb->handle);

	ion_free(mmb->client, mmb->handle);

	if (mmb->iommu)
		rb_erase(&mmb->s_node, &mmb->zone->root);
	else
		rb_erase(&mmb->node, &mmb->zone->root);
	kfree(mmb);

	return 0;
}

int hil_mmb_free(hil_mmb_t *mmb)
{
	struct mmb_udata *p, *q;

	if (NULL == mmb) {
		return HI_FAILURE;
	}

	down(&mmz_lock);
	if (mmb->phy_ref > 0)
		mmb->phy_ref--;

	if ((mmb->map_ref) || (mmb->phy_ref) || (mmb->cma_smmu_ref) ||
						(mmb->sec_smmu_ref)) {
		HI_PRINT("%s name=%s, mmz(0x%x) smmu(0x%x) is still used: \
			phy_ref:%d map_ref:%d smmu_ref:%d  sec_smmu_ref:0x%d\n",
			__func__, mmb->name, mmb->phys_addr, mmb->iommu_addr,
			mmb->phy_ref, mmb->map_ref, mmb->cma_smmu_ref,
			mmb->sec_smmu_ref);
		up(&mmz_lock);
		return 0;
	}
	if (!list_empty(&mmb->ulist)) {
		/*
		 * if we are in exception mode like killing process by ctrl+c
		 * the udata in mmb maybe not free, we free them there.
		 * And unmap is called normally, udata has been free before and
		 * this branch do not get in.
		 */
		list_for_each_entry_safe(p, q, &mmb->ulist, list) {
			list_del(&p->list);
			kfree(p);
		}
	}

	_mmb_free(mmb);

	up(&mmz_lock);

	return 0;
}

hil_mmb_t *hil_mmb_getby_kvirt(void *virt)
{
	hil_mmb_t *mmb = NULL;
	hil_mmz_t *zone;

	if (virt == NULL) {
		HI_PRINT("%s virt:%p err args\n", __func__, virt);
		return NULL;
	}

	down(&mmz_lock);
	list_for_each_entry(zone, &mmz_list, list) {
		struct rb_node *n;
		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m;

			if (zone->iommu)
				m = rb_entry(n, hil_mmb_t, s_node);
			else
				m = rb_entry(n, hil_mmb_t, node);
			if (!m->kdata)
				continue;
			if ((unsigned long)(m->kdata->kvirt) == (unsigned long)virt) {
				mmb = m;
				goto end;
			}
		}
	}
end:
	up(&mmz_lock);

	if (NULL == mmb)
		HI_PRINT("%s virt:%p cannot find mem\n", __func__, virt);

	return mmb;
}


hil_mmb_t *hil_mmbinfo_getby_kvirt(void *virt)
{
	hil_mmb_t *mmb = NULL;
	hil_mmz_t *zone;

	if (virt == NULL) {
		HI_PRINT("%s virt:%p err args\n", __func__, virt);
		return NULL;
	}

	down(&mmz_lock);
	list_for_each_entry(zone, &mmz_list, list) {
		struct rb_node *n;
		for (n = rb_first(&zone->root); n; n = rb_next(n)) {
			hil_mmb_t *m;

			if (zone->iommu)
				m = rb_entry(n, hil_mmb_t, s_node);
			else
				m = rb_entry(n, hil_mmb_t, node);
			if (!m->kdata)
				continue;
			if (((unsigned long)(m->kdata->kvirt) <= (unsigned long)virt) &&
		((unsigned long)(m->kdata->kvirt + m->length) >= (unsigned long)virt)) {
				mmb = m;
				goto end;
			}
		}
	}
end:
	up(&mmz_lock);
	if (NULL == mmb)
		HI_PRINT("%s virt:%p cannot find mem\n", __func__, virt);

	return mmb;
}


mmb_addr_t hil_mmb_cma_mapto_iommu(mmb_addr_t addr, int iommu)
{
	hil_mmb_t *mmb;
	struct iommu_map_format *format;
	hil_mmz_t *mmz = NULL;
	hil_mmz_t *p;
	HI_U32 gfp = 0; /* gfp is 0 in later version */
	int ret;
	struct mmz_iommu *common = &mmz_iommu;

	if (iommu) {
		HI_PRINT("%s:err args,iommu must be 0,and addr must be \
				cma phy_addr\n", __func__);
		return MMB_ADDR_INVALID;		
	}

	mmb = hil_mmb_getby_phys((HI_U32)addr, 0);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return MMB_ADDR_INVALID;
	}
	if (MMB_ADDR_INVALID != mmb->iommu_addr) {
		HI_PRINT("It's already mapped to iommu.\n");
		mmb->cma_smmu_ref++;
		mmb->phy_ref++;
		return mmb->iommu_addr;
	}

	down(&mmz_lock);

	format = kzalloc(sizeof(struct iommu_map_format), GFP_KERNEL);
	if (NULL == format) {
		HI_PRINT("no mem!\n");
		up(&mmz_lock);
		return MMB_ADDR_INVALID;
	}
	ret = ion_map_iommu(common->client, mmb->handle, format);
	if (ret) {
		HI_PRINT("alloc iommu failed!\n");
		kfree(format);
		up(&mmz_lock);
		return MMB_ADDR_INVALID;
	}

	mmb->iommu_addr = format->iova_start;
	kfree(format);
	format = NULL;

	mmb->cma_smmu_ref++;
	mmb->phy_ref++;

	begin_list_for_each_mmz(p, gfp, "iommu")
		mmz = p;
	end_list_for_each_mmz()
	if (NULL == mmz) {
		HI_PRINT("%s:cannot find iommu zone!\n", __func__);
		up(&mmz_lock);
		return MMB_ADDR_INVALID;
	}

	mmb_add_to_rbtree(mmb, mmz, 1);
	up(&mmz_lock);
	return mmb->iommu_addr;
}

int hil_mmb_cma_unmapfrom_iommu(mmb_addr_t addr, int iommu)
{
	hil_mmb_t *mmb;
	hil_mmz_t *p;
	hil_mmz_t *mmz = NULL;
	HI_U32 gfp = 0;

	if (!iommu) {
		HI_PRINT("%s:err args,iommu must be 1,and addr must be \
				cma phy_addr\n", __func__);
		return HI_FAILURE;
	}

	mmb = hil_mmb_getby_phys((HI_U32)addr, 1);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}

	if (MMB_ADDR_INVALID != mmb->iommu_addr) {
		mmb->cma_smmu_ref--;
		mmb->phy_ref--;
	}

	if (mmb->cma_smmu_ref)
		return HI_SUCCESS;

	down(&mmz_lock);
	ion_unmap_iommu(mmb->client, mmb->handle);
	begin_list_for_each_mmz(p, gfp, "iommu")
		mmz = p;
	end_list_for_each_mmz()
	if (NULL == mmz) {
		HI_PRINT("%s:cannot find iommu zone!\n", __func__);
		up(&mmz_lock);
		return HI_FAILURE;
	}

	mmb->iommu_addr = MMB_ADDR_INVALID;
	rb_erase(&mmb->s_node, &mmz->root);
	up(&mmz_lock);

	if ((0 == (mmb->phy_ref)) && (0 == mmb->map_ref) &&
		(0 == mmb->cma_smmu_ref) && (0 == mmb->sec_smmu_ref)) {
		/* free mmb   */
		hil_mmb_free(mmb);
	}

	return HI_SUCCESS;
}

struct sg_table *hil_get_meminfo(hil_mmb_t *mmb)
{
	unsigned long size;
	struct sg_table *table = NULL;
	struct mmz_iommu *common = &mmz_iommu;

	table = get_pages_from_buffer(common->client, mmb->handle, &size);
	if (!table) {
		HI_PRINT("%s: get pages failed!\n", __func__);
		return NULL;
	}
	return table;
}

int sec_mmb_get(u32 addr, int iommu, u32 sec_smmu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}
	down(&mmz_lock);

	if (!mmb->sec_smmu_ref)
		mmb->sec_smmu = sec_smmu;

	mmb->sec_smmu_ref ++;
	mmb->phy_ref ++;

	up(&mmz_lock);
	return HI_SUCCESS;
}

int sec_mmb_put(u32 addr, int iommu)
{
	hil_mmb_t *mmb;
	int ref;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args, addr:0x%x iommu:0x%x!\n", __func__, addr, iommu);
		return HI_FAILURE;
	}
	down(&mmz_lock);

	if (!mmb->sec_smmu_ref || !mmb->phy_ref) {
		HI_PRINT("%s: wrong operation.mmb->sec_smmu_ref :%d \
			mmb->phy_ref: %d \n", __func__, mmb->sec_smmu_ref, mmb->phy_ref);
		up(&mmz_lock);
		return HI_FAILURE;
	}

	mmb->sec_smmu_ref --;
	mmb->phy_ref --;
	if (!mmb->sec_smmu_ref)
		mmb->sec_smmu = MMB_ADDR_INVALID;

	ref = mmb->sec_smmu_ref;

	up(&mmz_lock);
	return ref;
}

int set_sec_mmb_flag(u32 addr, int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}
	down(&mmz_lock);
	mmb->sec_flag = 1;
	up(&mmz_lock);
	return HI_SUCCESS;
}

int clr_sec_mmb_flag(u32 addr, int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}
	down(&mmz_lock);
	mmb->sec_flag = 0;
	up(&mmz_lock);
	return HI_SUCCESS;
}

/* return 0 : normal mem ; rerurn 1 :sec mem    */
int is_sec_mem(u32 addr, int iommu)
{
	hil_mmb_t *mmb;
	int ret = 0;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}

	down(&mmz_lock);
	ret = mmb->sec_flag;
	up(&mmz_lock);

	return ret;
}

int sec_delay_release_for_mem(u32 addr, int iommu)
{
	hil_mmb_t *mmb;

	mmb = hil_mmb_getby_phys((HI_U32)addr, (HI_U32)iommu);
	if (!mmb) {
		HI_PRINT("%s:err args!\n", __func__);
		return HI_FAILURE;
	}
	if ((0 == (mmb->phy_ref)) && (0 == mmb->map_ref) &&
		(0 == mmb->cma_smmu_ref) && (0 == mmb->sec_smmu_ref)) {
		/* free mmb   */
		hil_mmb_free(mmb);
	}

	return 0;
}

hil_mmz_t *hil_mmz_find(HI_U32 gfp, const char *mmz_name)
{
	hil_mmz_t *p;

	down(&mmz_lock);
	begin_list_for_each_mmz(p, gfp, mmz_name)
	up(&mmz_lock);
	return p;
	end_list_for_each_mmz()
	up(&mmz_lock);

	return NULL;
}

/*
 * name,gfp,phys_start,nbytes,alloc_type;...
 * All param in hex mode, except name.
 */
static int media_mem_parse_cmdline(char *s)
{
	hil_mmz_t *zone = NULL;
	char *line;
	struct ion_platform_heap *heap;

	while ((line = strsep(&s, ":")) != NULL) {
		int i;
		char *argv[6];

		/*
		 * FIXME: We got 4 args in "line", formated "argv[0],argv[1],
		 * argv[2],argv[3],argv[4]". eg: "<mmz_name>,<gfp>,
		 * <phys_start_addr>,<size>,<alloc_type>" For more convenient,
		 * "hard code" are used such as "arg[0]", i.e.
		 */
		for (i = 0; (argv[i] = strsep(&line, ",")) != NULL;) {
			if (++i == ARRAY_SIZE(argv)) {
				break;
			}
		}

		heap = hisi_get_cma_heap(argv[0]);
		if (!heap) {
			HI_PRINT("can't get cma zone info:%s\n", argv[0]);
			continue;
		}

		if (i == 4) {
			zone = hil_mmz_create("null", 0, 0, 0);
			if (NULL == zone) {
				continue;
			}
			strlcpy(zone->name, argv[0], HIL_MAX_NAME_LEN);
			zone->gfp = 0;
			zone->zone_start = heap->base;
			zone->nbytes = heap->size;
			zone->heap_id_mask = 1 << (heap->id);
			/*
			 * all cma zones share one root,all mmb from cma zones
			 * are added to this root.
			 */
			zone->root = RB_ROOT;
			zone->iommu = 0;
		} else {
			PRINTK_CA(KERN_ERR "MMZ: your parameter num is not correct!\n");
			continue;
		}

		if (hil_mmz_register(zone)) {
			PRINTK_CA(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
			hil_mmz_destroy(zone);
		}

		zone = NULL;
	}
		return 0;
}

#define MAX_MMZ_INFO_LEN 64*1024

#define CHECK_BUFF_OVERFLOW() \
do{\
	if (m->count > MAX_MMZ_INFO_LEN){\
		PRINTK_CA(KERN_ERR"mmz_info_buff overflow(0x%x), more than 20k data!\n", (unsigned int)m->count);\
		break;\
	};\
}while(0)

#define SPLIT_LINE "-------------------------------------------------------------------------------------------------------\n"
#if !(0 == HI_PROC_SUPPORT)
static int mmz_total_size = 0;
int mmz_read_proc(struct seq_file *m, void *v)
{
	int nZoneCnt = 0;
	hil_mmz_t *p;
	unsigned int used_size = 0, free_size = 0;
	unsigned int u32Number = 0;
	char *smmu_name = "SMMU";
	char *phy_name = "DDR";


	down(&mmz_lock);

	// Collect all mmb info into mmz_info_buff
	list_for_each_entry(p, &mmz_list, list) {
		hil_mmb_t *mmb;
		struct rb_node *n;
		char *mem_type = NULL;

#ifndef HI_SMMU_SUPPORT
		if (p->iommu)
			continue;
#endif

		if (p->iommu)
			mem_type = smmu_name;
		else
			mem_type = phy_name;

		seq_puts(m, SPLIT_LINE);
		seq_printf(m, "|                   %s           |  ID  | ZONE  |  KVIRT  |  FLAGS  |  LENGTH(KB)  |       NAME        |\n", mem_type);
		seq_puts(m, SPLIT_LINE);

		for (n = rb_first(&p->root); n; n = rb_next(n)) {
			u32Number++;
		}

		seq_printf(m, "|ZONE[%d]: (0x%08x, 0x%08x)   %d         %d        0x%08x      %-10u   \"%s%-14s|\n", nZoneCnt, \
			(p)->zone_start, (p)->zone_start+(p)->nbytes-1, u32Number, (p)->iommu, (p)->gfp, (p)->nbytes/SZ_1K, (p)->name, "\"");
		CHECK_BUFF_OVERFLOW();

		mmz_total_size = p->nbytes / 1024;
		zone_number++;

		n = NULL;
		mmb = NULL;
		for (n = rb_first(&p->root); n; n = rb_next(n)) {
			if (p->iommu) {
				mmb = rb_entry(n, hil_mmb_t, s_node);
				if (!mmb->iommu)
					continue;
			} else {
				mmb = rb_entry(n, hil_mmb_t, node);
			}
			seq_printf(m, "|" HIL_MMB_FMT_S "|\n", hil_mmb_fmt_arg(mmb));
			CHECK_BUFF_OVERFLOW();
			used_size += mmb->length/1024;
			block_number++;
		}

		seq_puts(m, SPLIT_LINE);
		seq_printf(m, "|%-102s|\n", "Summary:");
		seq_puts(m, SPLIT_LINE);
		if ((p)->iommu)
			seq_puts(m, "|  SMMU Total Size  |    Iommu Used     |     Idle     |  Zone Number  |   BLock Number                 |\n");
		else
			seq_puts(m, "|  MMZ Total Size  |     CMA Used      |     Idle     |  Zone Number  |   BLock Number                 |\n");

		seq_puts(m, SPLIT_LINE);
		free_size = mmz_total_size - used_size;
		seq_printf(m, "|       %d%-8s       %d%-8s          %d%-8s         %d              %d                |\n",
				 mmz_total_size/1024, "MB", used_size/1024, "MB", free_size/1024, "MB", nZoneCnt, block_number);
		seq_puts(m, SPLIT_LINE);
		seq_puts(m, "\n");
		CHECK_BUFF_OVERFLOW();

		mmz_total_size = 0;
		block_number = 0;
		used_size = 0;
		free_size = 0;
		nZoneCnt++;

	}

	up(&mmz_lock);


	return 0;
}
#endif
#define MMZ_SETUP_CMDLINE_LEN 256
static char __initdata setup_zones[MMZ_SETUP_CMDLINE_LEN] = "ddr,0,0,160M";

static void mmz_exit_check(void)
{
	hil_mmz_t *p;

	mmz_trace_func();
	for (p = hil_mmz_find(0, NULL); p != NULL; p = hil_mmz_find(0, NULL)) {
		hil_mmz_unregister(p);
	}
}

static int mmz_zone_init(void)
{
	char *s;
	char *p = NULL;
	char *q;

	strlcpy(line, saved_command_line, COMMAND_LINE_SIZE);
	q = strstr(line, "mmz=");
	if (q) {
		s = strsep(&q, "=");
		if (s) {
			p = strsep(&q, " ");
		}
		if (p) {
			strlcpy(setup_zones, p, MMZ_SETUP_CMDLINE_LEN);
		}
	}

	media_mem_parse_cmdline(setup_zones);

	return HI_SUCCESS;
}

#ifdef HI_SMMU_SUPPORT
static int iommu_zone_init(void)
{
	hil_mmz_t *zone = NULL;
	struct iommu_zone *iommu_zone;

	iommu_zone = hisi_get_iommu_zone();
	if (NULL == iommu_zone) {
		HI_PRINT("iommu zone can not be used!\n");
		return HI_FAILURE;
	}

	zone = hil_mmz_create("null", 0, 0, 0);
	if (NULL == zone) {
		HI_PRINT("iommu zone created failed,iommu zone may not be used!\n");
		return HI_FAILURE;
	}
	strlcpy(zone->name, "iommu", HIL_MAX_NAME_LEN);
	zone->gfp = 0;
	zone->zone_start = iommu_zone->iova_start;
	zone->nbytes = iommu_zone->iova_end - iommu_zone->iova_start + 1;
	zone->heap_id_mask = ION_HEAP_SYSTEM_MASK;
	zone->root = RB_ROOT;
	zone->iommu = 1;
	if (hil_mmz_register(zone)) {
		PRINTK_CA(KERN_WARNING "Add MMZ failed: " HIL_MMZ_FMT_S "\n", hil_mmz_fmt_arg(zone));
		hil_mmz_destroy(zone);
	}

	return HI_SUCCESS;
}
#endif

static int client_init(void)
{
	struct mmz_iommu *common = &mmz_iommu;

	common->client = hisi_ion_client_create("cma-iommu");
	if (NULL == common->client) {
		HI_PRINT("ion client is created failed!\n");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

static int init_done;

int HI_DRV_MMZ_Init(void)
{
	int ret;

	if (init_done)
		return 0;

	ret = client_init();
	if (ret) {
		HI_PRINT("client init is failed!\n");
		return HI_FAILURE;
	}

	ret = mmz_zone_init();
	if (ret) {
		HI_PRINT("mmz zone init is failed!\n");
		return HI_FAILURE;
	}

#ifdef HI_SMMU_SUPPORT
	ret = iommu_zone_init();
	if (ret) {
		HI_PRINT("iommu zone init is failed!\n");
		return HI_FAILURE;
	}
#endif

	init_done = 1;

	return HI_SUCCESS;
}

void HI_DRV_MMZ_Exit(void)
{
	struct mmz_iommu *common = &mmz_iommu;

	mmz_exit_check();
	init_done = 0;
	hisi_ion_client_destroy(common->client);

	return;
}

EXPORT_SYMBOL(HI_DRV_MMZ_Init);
EXPORT_SYMBOL(HI_DRV_MMZ_Exit);
