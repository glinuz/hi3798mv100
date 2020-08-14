#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/errno.h>
#include <linux/err.h>
#include "ion.h"
#include "ion_priv.h"
#include <sec_mmz.h>

#define ION_TEE_ALLOCATE_FAILED -1
#define to_tee_heap(x) container_of(x, struct ion_tee_heap, heap)

struct ion_tee_heap {
	struct ion_heap heap;
};

struct ion_tee_buffer_info {
	dma_addr_t phys_addr;
	dma_addr_t cpu_addr;
	struct sg_table *table;
};

/*
 * Create scatter-list for the already allocated DMA buffer.
 * This function could be replaced by dma_common_get_sgtable
 * as soon as it will avalaible.
 */
static int ion_tee_get_sgtable(struct sg_table *sgt,
			dma_addr_t handle, size_t size)
{
	/*
	 * struct page *page is just adapted to ion framework
	 * here,and it does not have other meaning,so it should
	 * not be used out of this function.
	 */
	struct page *page = ZERO_PAGE(0);
	int ret;

	ret = sg_alloc_table(sgt, 1, GFP_KERNEL);
	if (unlikely(ret))
		return ret;

	sg_set_page(sgt->sgl, page, PAGE_ALIGN(size), 0);
	return 0;
}

static int ion_tee_allocate(struct ion_heap *heap,
				struct ion_buffer *buffer,
				unsigned long size,
				unsigned long align,
				unsigned long flags)
{
	struct ion_tee_buffer_info *info;

	info = kzalloc(sizeof(struct ion_tee_buffer_info), GFP_KERNEL);
	if (!info) {
		pr_err("Can't allocate buffer info\n");
		return ION_TEE_ALLOCATE_FAILED;
	}

	info->phys_addr = (dma_addr_t)HI_SEC_MMZ_New(size,
				(char *)"SEC-MMZ", (char *)"secure");

	if (!info->phys_addr) {
		pr_err("Failed to allocate buffer\n");
		goto err;
	}

	/* virt addr is the same as phys_addr in tee   */
	info->cpu_addr = info->phys_addr;

	info->table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!info->table) {
		pr_err("Fail to allocate sg table\n");
		goto free_mem;
	}

	if (ion_tee_get_sgtable(info->table, info->phys_addr, size))
		goto free_table;

	buffer->priv_virt = info;

	return 0;
free_table:
	kfree(info->table);

free_mem:
	HI_SEC_MMZ_Delete(info->phys_addr);

err:
	kfree(info);
	return ION_TEE_ALLOCATE_FAILED;
}

static void ion_tee_free(struct ion_buffer *buffer)
{
	struct ion_tee_buffer_info *info = buffer->priv_virt;

	HI_SEC_MMZ_Delete(info->phys_addr);
	sg_free_table(info->table);
	kfree(info->table);
	kfree(info);
}

struct sg_table *ion_tee_heap_map_dma(struct ion_heap *heap,
				struct ion_buffer *buffer)
{
	struct ion_tee_buffer_info *info = buffer->priv_virt;

	return info->table;
}

void *ion_tee_map_kernel(struct ion_heap *heap, struct ion_buffer *buffer)
{
	return NULL;
}

static int ion_tee_mmap(struct ion_heap *mapper, struct ion_buffer *buffer,
				struct vm_area_struct *vma)
{
	return 0;
}

void ion_tee_heap_unmap_dma(struct ion_heap *heap,
				struct ion_buffer *buffer)
{
	return;
}

/* return physical address in addr */
static int ion_tee_phys(struct ion_heap *heap, struct ion_buffer *buffer,
			ion_phys_addr_t *addr, size_t *len)
{
	struct ion_tee_buffer_info *info = buffer->priv_virt;

	*addr = info->phys_addr;
	*len = buffer->size;

	return 0;
}

static struct ion_heap_ops ion_tee_ops = {
	.allocate = ion_tee_allocate,
	.free = ion_tee_free,
	.map_dma = ion_tee_heap_map_dma,
	.unmap_dma = ion_tee_heap_unmap_dma,
	.phys = ion_tee_phys,
	.map_user = ion_tee_mmap,
	.map_kernel = ion_tee_map_kernel,
};

struct ion_heap *ion_tee_sec_heap_create(struct ion_platform_heap *data)
{
	struct ion_tee_heap *tee_heap;
	tee_heap = kzalloc(sizeof(struct ion_tee_heap), GFP_KERNEL);
	if (!tee_heap)
		return ERR_PTR(-ENOMEM);
	tee_heap->heap.ops = &ion_tee_ops;
	tee_heap->heap.type = ION_HEAP_TYPE_TEE_SEC_MEM;

	return &tee_heap->heap;
}

void ion_tee_sec_heap_destory(struct ion_heap *heap)
{
	struct ion_tee_heap *tee_heap = to_tee_heap(heap);

	kfree(tee_heap);
}
