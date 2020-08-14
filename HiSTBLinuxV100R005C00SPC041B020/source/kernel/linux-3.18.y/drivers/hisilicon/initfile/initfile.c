/******************************************************************************
 *  Copyright (C) 2014 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2014.12.22
 * multi init file
******************************************************************************/
#define pr_fmt(fmt) "initfile: " fmt

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/memblock.h>

#define INITFILE_NAME	"hisi.initfile"
#define SPLIT_SIZE	0x1400000

struct initfile {
	ulong start;
	ulong size;

#define FL_STATUS_INVALID            0x00
#define FL_STATUS_SETUP              0x01
#define FL_STATUS_RESERVE            0x02
#define FL_STATUS_MOVED              0x03
	int status;
};

static struct initfile initfiles[CONFIG_BLK_DEV_LOOP_MIN_COUNT] = {{0}};

/*
 * initfile=3,0x10000000,0x50000000
 */
static int __init early_initfile(char *p)
{
	unsigned int index;
	struct initfile *lp;
	char *endp;

	index = memparse(p, &endp);
	if (*endp != ',')
		return 0;

	if (index >= CONFIG_BLK_DEV_LOOP_MIN_COUNT) {
		pr_notice("index %d out of range, CONFIG_BLK_DEV_LOOP_MIN_COUNT is %d.\n", index, CONFIG_BLK_DEV_LOOP_MIN_COUNT);
		return 0;
	}

	lp = &initfiles[index];

	lp->start = memparse(endp + 1, &endp);
	if (*endp != ',')
		return 0;

	lp->size = memparse(endp + 1, NULL);

	if (lp->size)
		lp->status = FL_STATUS_SETUP;

	return 0;
}
early_param("initfile", early_initfile);
/*****************************************************************************/

static int __init reserve_one_initfile(int index, struct initfile *lp)
{
	if (!memblock_is_region_memory(lp->start, lp->size)) {
		pr_err("0x%08lx+0x%08lx is not a memory region, disabling initfile:%d\n",
			   lp->start, lp->size, index);
		goto fail;
	}

	if (memblock_is_region_reserved(lp->start, lp->size)) {
		pr_err("0x%08lx+0x%08lx overlaps in-use memory region, disabling initfile:%d\n",
			   lp->start, lp->size, index);
		goto fail;
	}

	memblock_reserve(lp->start, lp->size);
	pr_info("reserved %ld MiB at %08lx\n", (unsigned long)lp->size / SZ_1M,
			(unsigned long)lp->start);

	lp->status = FL_STATUS_RESERVE;
	lp->start = __phys_to_virt(lp->start);

	return 0;
fail:
	lp->status = FL_STATUS_INVALID;

	return -1;
}
/*****************************************************************************/

void __init initfile_reserve_memory(void)
{
	int ix;
	struct initfile *lp;

	for (ix = 0; ix < CONFIG_BLK_DEV_LOOP_MIN_COUNT; ix++) {
		lp = &initfiles[ix];
		if (lp->status == FL_STATUS_SETUP)
			reserve_one_initfile(ix, lp);
	}
}
/******************************************************************************/

static void __init move_one_file(int index, struct initfile *lp)
{
	int fd;
	unsigned long offset, count;
	char name[16];

	snprintf(name, sizeof(name), "/%s%d", INITFILE_NAME, index);
	fd = sys_open(name, O_WRONLY|O_CREAT, 0700);
	if (fd < 0) {
		pr_err("open %s failed.\n", name);
		free_reserved_area((char *)lp->start, (char *)lp->start + lp->size, 0, name);
		return;
	}

	count = SPLIT_SIZE;
	for (offset = 0; offset < lp->size; offset += SPLIT_SIZE) {
		if ((offset + SPLIT_SIZE) > lp->size)
			count = lp->size - offset;

		sys_write(fd, (char *)(lp->start + offset), count);
		free_reserved_area((char *)lp->start + offset, (char *)lp->start + offset + count, 0, name);
	}
	sys_close(fd);

	lp->status = FL_STATUS_MOVED;
}
/******************************************************************************/

static int __init move_all_files(void)
{
	int ix;
	struct initfile *lp;

	for (ix = 0; ix < CONFIG_BLK_DEV_LOOP_MIN_COUNT; ix++) {
		lp = &initfiles[ix];

		if (lp->status == FL_STATUS_RESERVE)
			move_one_file(ix, lp);
	}

	return 0;
}
rootfs_initcall(move_all_files);
