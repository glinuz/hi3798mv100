/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/freezer.h>
#include <linux/module.h>
#include <linux/mempool.h>
#ifdef CONFIG_DEVCHIP_PLATFORM
#include <linux/err.h>//for hisi_linux : by fangjian 00208632
#endif
#include "mem.h"
#include "smc.h"
#include "tc_ns_client.h"
#include "teek_ns_client.h"
#include "agent.h"
#include "securec.h"
#include "tc_ns_log.h"

#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))

static DEFINE_MUTEX(prealloc_lock);
/************global reference end*************/
static void *g_mem_pre_allocated;
static mempool_t *tc_sharemem_page_pool = NULL;
int tc_mem_free(TC_NS_Shared_MEM *shared_mem)
{
	if (NULL == shared_mem)
		return -1;
	if (1 == shared_mem->from_mem_pool) {
		tlogw("release mem to mem pool\n");
		mempool_free((void*)shared_mem->kernel_addr, tc_sharemem_page_pool);
	} else if (shared_mem->kernel_addr &&
	    (shared_mem->kernel_addr != g_mem_pre_allocated)) {
		free_pages((unsigned long)shared_mem->kernel_addr,
			   get_order(ROUND_UP(shared_mem->len, SZ_4K))); /*lint !e647 !e866 !e747 !e647  !e834 !e732*/
	} else if (shared_mem->kernel_addr == g_mem_pre_allocated) {
		mutex_unlock(&prealloc_lock);
	} else {
	    tloge("failed to release the memory pages of share mem!\n");
	}
	kfree(shared_mem);
	return 0;
}

TC_NS_Shared_MEM *tc_mem_allocate(TC_NS_DEV_File *dev, size_t len)
{
	TC_NS_Shared_MEM *shared_mem = NULL;
	void *addr = NULL;
	if (!dev) {
		tloge("can not find dev in malloc shared buffer!\n");
		return ERR_PTR(-EFAULT);
	}

	shared_mem = kmalloc(sizeof(TC_NS_Shared_MEM), GFP_KERNEL|__GFP_ZERO);
	if (!shared_mem) {
		tloge("shared_mem kmalloc failed\n");
		return ERR_PTR(-ENOMEM);
	}
	addr = (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO,
						get_order(ROUND_UP(len, SZ_4K)));
	if (!addr && tc_sharemem_page_pool && (len <=MEM_POOL_ELEMENT_SIZE)) {
		tlogw("get free pages failed, try to alloc from mempool\n");
		addr = (void*)mempool_alloc(tc_sharemem_page_pool, GFP_KERNEL);
		if (addr) {
			shared_mem->from_mem_pool = 1;
			tlogw("success alloc mem  from mempool\n");
		} else {
			tlogw("failed to alloc mem  from mempool\n");
		}
	}
	if (!addr) {
		tlogw("get free pages from mempool also failed, try pre allocted mem, len is %zx\n", len);
		if (mutex_trylock(&prealloc_lock)) {
			if (g_mem_pre_allocated && (len <= PRE_ALLOCATE_SIZE)) {
				tlogd("use pre allocted mem to work\n");
				if(memset_s(g_mem_pre_allocated, PRE_ALLOCATE_SIZE, 0, PRE_ALLOCATE_SIZE)) {
					tloge("failed to memset_s g_mem_pre_allocated buffer!\n");
				}
				addr = g_mem_pre_allocated;
				/* In case we could not use the preallocated
				 * memory unlock the mutex */
			} else
				mutex_unlock(&prealloc_lock);
		}
		/* If we couldn't use the preallocated memory then return */
		if (!addr) {
			tloge("g_mem_pre_allocated failed\n");
			kfree(shared_mem);
			return ERR_PTR(-ENOMEM);
		}
	}

	shared_mem->kernel_addr = addr;
	shared_mem->len = len;

	return shared_mem;
}

int tc_mem_init(void)
{
	tlogi("tc_mem_init\n");
	/* pre-allocated memory for large use */
	g_mem_pre_allocated = (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO,
			      get_order(ROUND_UP
					(PRE_ALLOCATE_SIZE, SZ_4K)));
	if (!g_mem_pre_allocated)
		tloge("g_mem_pre_allocated failed\n");
	tc_sharemem_page_pool = mempool_create_node(MEM_POOL_ELEMENT_NR, (mempool_alloc_t *)__get_free_pages,
		                 (mempool_free_t *)free_pages,(void*)MEM_POOL_ELEMENT_ORDER,
				   GFP_KERNEL | __GFP_ZERO, NUMA_NO_NODE);
      if (!tc_sharemem_page_pool)
		tloge("tc_sharemem_page_pool failed\n");

	return 0;
}

void tc_mem_destroy(void)
{
	tlogi("tc_client exit\n");
	mutex_lock(&prealloc_lock);

	if (g_mem_pre_allocated) {
		free_pages((unsigned long)g_mem_pre_allocated, /*lint !e778 !e866 !e747 !e732*/
			   get_order(ROUND_UP(PRE_ALLOCATE_SIZE, SZ_4K)));
		g_mem_pre_allocated = NULL;
	}
	if (tc_sharemem_page_pool) {
		mempool_destroy(tc_sharemem_page_pool);
		tc_sharemem_page_pool = NULL;
	}
	mutex_unlock(&prealloc_lock);
}
#ifdef CONFIG_DEVCHIP_PLATFORM
/* for hisi SDK hi_media.ko */
EXPORT_SYMBOL(tc_mem_allocate);
EXPORT_SYMBOL(tc_mem_free);
#endif
