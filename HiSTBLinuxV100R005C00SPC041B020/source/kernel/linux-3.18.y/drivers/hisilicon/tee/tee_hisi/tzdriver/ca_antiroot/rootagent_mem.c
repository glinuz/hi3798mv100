/*
 * FileName:      rootagent_mem.c
 * Author:        z00321956
 * Description:   rootagent memory initial
 * Version:       0.0.1
 * Date:          2016-04-01
 */
#include <linux/mm.h>
#include "teek_client_type.h"
#include "rootagent_mem.h"
#include "rootagent_common.h"

static void *g_rwmem;
static void *g_swap;
static int g_mem_flag;

int rm_mem_init(void)
{
	if (!g_mem_flag) {
		g_rwmem = (void *) __get_free_pages(GFP_KERNEL,
				get_order(RM_PRE_ALLOCATE_SIZE));
		if (!g_rwmem) {
			antiroot_error("mem allocated failed\n");
			return -ENOMEM;
		}
		g_swap = (void *) __get_free_pages(GFP_KERNEL,
				get_order(RM_PRE_ALLOCATE_SIZE));
		if (!g_swap) {
			antiroot_error("Swap mem allocated failed\n");
			free_pages((unsigned long)g_rwmem,
				get_order(RM_PRE_ALLOCATE_SIZE));
			g_rwmem = NULL;
			return -ENOMEM;
		}
		g_mem_flag = 1;
	}
	antiroot_debug(ROOTAGENT_DEBUG_MEM, "rm_mem_init successful!\n");
	return 0;
}

void rm_mem_destroy(void)
{
	if (g_rwmem) {
		free_pages((unsigned long)g_rwmem,
			get_order(RM_PRE_ALLOCATE_SIZE));
		g_rwmem = NULL;
	}
	if (g_swap) {
		free_pages((unsigned long)g_swap,
			get_order(RM_PRE_ALLOCATE_SIZE));
		g_swap = NULL;
	}
}

int initial_rm_shared_mem(TEEC_TempMemoryReference *rwMem,
		TEEC_TempMemoryReference *swapMem)
{
	if (!swapMem || !rwMem) {
		antiroot_error("Bad param!\n");
		return -EINVAL;
	}

	if (g_swap) {
		swapMem->buffer = g_swap;
		swapMem->size = RM_PRE_ALLOCATE_SIZE;
	} else {
		antiroot_error("initial_rm_shared_mem failed!\n");
		return -ENOMEM;
	}

	if (g_rwmem) {
		rwMem->buffer = g_rwmem;
		rwMem->size = RM_PRE_ALLOCATE_SIZE;
	} else {
		antiroot_error("initial_rm_shared_mem failed!\n");
		return -ENOMEM;
	}
	antiroot_debug(ROOTAGENT_DEBUG_MEM, "initial_rm_shared_mem successful!\n");
	return 0;
}
