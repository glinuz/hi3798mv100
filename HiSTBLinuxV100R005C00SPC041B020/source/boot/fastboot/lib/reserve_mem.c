/******************************************************************************
 *    Reserve memory manager
 *    Copyright (c) 2013-2014 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Lai Yingjun.
 *
******************************************************************************/

#include <config.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <asm/types.h>
#include <asm/sizes.h>
#include <common.h>
#include <linux/list.h>

#if 1
#  define DBG_MSG(_fmt, arg...)
#  define DBG_WARN(_fmt, arg...)
#else
#  define DBG_MSG(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);

#  define DBG_WARN(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

typedef struct {
	unsigned long addr;
	unsigned long size;
	struct list_head list;
} reserve_mem_head;

#define ALLOC_ALIGN             4096
#define __RESERVE_MEM_PAD       0x5A
#define HIBERNATE_ALLOC_ALIGN   0X100000 /* aligned with 1M */

extern int parse_mem_args(unsigned long *ret_start_addr,
			  unsigned long *ret_size);
extern unsigned long long memparse(const char *ptr, char **retptr);

DECLARE_GLOBAL_DATA_PTR;

LIST_HEAD(reserve_mem);

static unsigned long reserve_mem_start = 0;
static unsigned long reserve_mem_free = 0;
static unsigned long reserve_mem_bound = 0;
static unsigned long reserve_hibdrv_mem = 0;
static unsigned long reserve_userapi_mem = 0;
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static unsigned long reserve_qbboot_mem = 0;
#endif
static int reserve_mem_init_flag = 0;

int set_reserve_mem_bound(unsigned long bound)
{
	reserve_mem_bound = (unsigned long)bound;

	return 0;
}

int get_reserve_mem_bound(void)
{
	return reserve_mem_bound;
}

char *get_hidrv_reserve_mem(void)
{
	return (char *)reserve_hibdrv_mem;
}

char *get_userapi_reserve_mem(void)
{
	return (char *)reserve_userapi_mem;
}
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
char *get_qbboot_reserve_mem(void)
{
	return (char *)reserve_qbboot_mem;
}
#endif
void *reserve_mem_alloc(unsigned int size, unsigned int *allocsize)
{

	void *ptr = NULL;
	reserve_mem_head *tmp = NULL;

	if (!reserve_mem_init_flag) {
		printf("Reserve memory manager NOT initialized!\n");
		return NULL;
	}

	size = (size + ALLOC_ALIGN - 1) & ~(ALLOC_ALIGN - 1);

	if (reserve_mem_free < (size + ALLOC_ALIGN)
	    || reserve_mem_free - (size + ALLOC_ALIGN) < reserve_mem_bound) {
		printf("Not enough reserve memory!\n");
		return NULL;
	}

	reserve_mem_free -= size + ALLOC_ALIGN;
	ptr = (void *)reserve_mem_free;

	memset(ptr + size, __RESERVE_MEM_PAD, ALLOC_ALIGN);

	tmp = ptr + size + ALLOC_ALIGN - sizeof(reserve_mem_head);
	tmp->addr = (unsigned long)ptr;
	tmp->size = size;
	list_add_tail(&tmp->list, &reserve_mem);

	if (allocsize)
		*allocsize = size;

	return ptr;
}

void adjust_reserve_mem_start(unsigned int ddrsize, unsigned long *ret_start_addr)
{
	char args_merge_name[32];
	int i;
	char *param[4];
	char *args;
	char *mmz_param;
	unsigned long long size, offset;
	char tmp_args[100] = {0};

	snprintf(args_merge_name, sizeof(args_merge_name), "bootargs_%s",
		 ultohstr((unsigned long long)ddrsize));

	args = getenv(args_merge_name);
	if (!args) {
		return;
	}

	memcpy(tmp_args, args, sizeof(tmp_args));
	tmp_args[sizeof(tmp_args) - 1] = '\0';
	mmz_param = strstr(tmp_args, "mmz=");
	if (!mmz_param) {
		return;
	}

	/* eg: mmz=ddr,0,0,256M,  0-mmz=ddr, 1-0, 2-0, 3-256M */
	for (i = 0; (param[i] = strsep(&mmz_param, ",")) != NULL;) {
		if (++i == 4)
			break;
	}

	size = memparse(param[3], NULL);

	offset = ((ddrsize/3) * 2) - size;

	*ret_start_addr = offset;
}

int reserve_mem_init(void)
{
	unsigned long start = 0;
	unsigned long size = 0;
	unsigned int ddr_free = 0;
	int ret = -1;
	bd_t *boardinfo = gd->bd;
	unsigned int ddrsize = boardinfo->bi_dram[0].size;

	set_reserve_mem_bound(get_ddr_free(&ddr_free, ALLOC_ALIGN) +
			      CONFIG_RESERVE_MEM_GAP);

	ret = parse_mem_args(&start, &size);
	DBG_MSG("Parse mem: Start=0x%lX, Size=0x%lX\n", start, size);
	if (ret != 0) {
		start = boardinfo->bi_dram[0].start;
		size = boardinfo->bi_dram[0].size;
	}

#ifdef CONFIG_RESERVE_MEM_SIZE_MAX
	if (size > CONFIG_RESERVE_MEM_SIZE_MAX)
		size = CONFIG_RESERVE_MEM_SIZE_MAX;
#endif

	if ((start + size) >= ALLOC_ALIGN)
		reserve_mem_start = start + size - ALLOC_ALIGN;

	if((ddrsize == (SZ_512M + SZ_256M)) || (ddrsize == (SZ_1G + SZ_512M)))
		adjust_reserve_mem_start(ddrsize, &reserve_mem_start);

	DBG_MSG("Parsed reserve mem: Start=0x%lX\n", reserve_mem_start);

	reserve_mem_start = (reserve_mem_start - ALLOC_ALIGN) & ~(ALLOC_ALIGN - 1);

	if (get_reserve_mem_bound() >= reserve_mem_start) {
		printf("Not enough memory for reserve memory!\n");
		ret = -1;
		goto error_out;
	}

	reserve_mem_free = reserve_mem_start;

	reserve_mem_init_flag = 1;

#ifdef CONFIG_HIBERNATE
	if (CONFIG_HIBERNATE_HIBDRV_SIZE%ALLOC_ALIGN) {
		printf("CONFIG_HIBERNATE_HIBDRV_SIZE(%d) should align with ALLOC_ALIGN(%d)",
			CONFIG_HIBERNATE_HIBDRV_SIZE, ALLOC_ALIGN);
		return -1;
	}
	reserve_hibdrv_mem = (unsigned long)reserve_mem_alloc(CONFIG_HIBERNATE_HIBDRV_SIZE +
		HIBERNATE_ALLOC_ALIGN, NULL) + HIBERNATE_ALLOC_ALIGN;
	reserve_hibdrv_mem &= ~(HIBERNATE_ALLOC_ALIGN - 1);

	if (CONFIG_HIBERNATE_USERAPI_SIZE%ALLOC_ALIGN) {
		printf("CONFIG_HIBERNATE_USERAPI_SIZE(%d) should align with ALLOC_ALIGN(%d)",
			CONFIG_HIBERNATE_USERAPI_SIZE, ALLOC_ALIGN);
		return -1;
	}
	reserve_userapi_mem = (unsigned long)reserve_mem_alloc(CONFIG_HIBERNATE_USERAPI_SIZE +
		HIBERNATE_ALLOC_ALIGN, NULL) + HIBERNATE_ALLOC_ALIGN;
	reserve_userapi_mem &= ~(HIBERNATE_ALLOC_ALIGN - 1);
#endif
#ifdef CONFIG_HISI_SNAPSHOT_BOOT
	if (CONFIG_HIBERNATE_QBBOOT_SIZE%ALLOC_ALIGN) {
		printf("CONFIG_HIBERNATE_QBBOOT_SIZE(%d) should align with ALLOC_ALIGN(%d)",
			CONFIG_HIBERNATE_QBBOOT_SIZE, ALLOC_ALIGN);
		return -1;
	}
	reserve_qbboot_mem = (unsigned long)reserve_mem_alloc(CONFIG_HIBERNATE_QBBOOT_SIZE +
		HIBERNATE_ALLOC_ALIGN, NULL) + HIBERNATE_ALLOC_ALIGN;
	reserve_qbboot_mem &= ~(HIBERNATE_ALLOC_ALIGN - 1);
#endif
	return 0;

error_out:
	set_reserve_mem_bound(0);
	reserve_mem_free = reserve_mem_start = 0;
	reserve_mem_init_flag = 0;
	return ret;
}

#if 0
/* for debug */
void test_reserve_mem(void)
{
	unsigned int size = 10 * 1024 * 1024 + 1;
	char *p = NULL;
	unsigned int phy_addr = NULL;
	//reserve_mem_init();
	extern int HI_DRV_PDM_AllocReserveMem(const char *BufName,
					      unsigned int u32Len,
					      unsigned int *pu32PhyAddr);
	HI_DRV_PDM_AllocReserveMem("baseparam", 3 * 1024 * 1024, &phy_addr);
	memset(phy_addr, 0x5A, 3 * 1024 * 1024);
	HI_DRV_PDM_AllocReserveMem("logodata", 3 * 1024 * 1024 + 1, &phy_addr);
	memset(phy_addr, 0x5A, 3 * 1024 * 1024 + 1);
	/*
	p = reserve_mem_alloc(10*1024*1024 + 1, &size);
	printf("p=%X, size=%d\n", p, size);
	p = reserve_mem_alloc(10*1024*1024 + 1 + 1024*1024, &size);
	printf("p=%X, size=%d\n", p, size);
	p = reserve_mem_alloc(10*1024*1024 + 1 - 1024*1024, &size);
	printf("p=%X, size=%d\n", p, size);
	*/
}
#endif

void show_reserve_mem(void)
{
	struct list_head *tmp = NULL;
	reserve_mem_head *head = NULL;

	printf("Reserve Memory\n");
	printf("    Start Addr:          0x%lX\n", reserve_mem_start);
	printf("    Bound Addr:          0x%lX\n", reserve_mem_bound);
	printf("    Free  Addr:          0x%lX\n", reserve_mem_free);
	printf("    Alloc Block:  Addr         Size\n");
	list_for_each(tmp, &reserve_mem) {
		head = list_entry(tmp, reserve_mem_head, list);
		printf("                  0x%lX   0x%lX\n", head->addr, head->size);
	}

	printf("\n");
}

