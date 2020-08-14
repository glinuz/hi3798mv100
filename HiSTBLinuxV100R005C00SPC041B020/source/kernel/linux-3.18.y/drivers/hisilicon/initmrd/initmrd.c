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
 * init multi ram disk
******************************************************************************/
#define pr_fmt(fmt) "initmrd: " fmt

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/syscalls.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/memblock.h>
#include <linux/hisilicon/himisc.h>

#define MRD_NAME      "hisi.mrd"
#define MRD_RAMDISK_DIR "/dev/block"
//#define MRD_RAMDISK_DIR "/dev"
#define SPLIT_SIZE	0x1400000

struct ramdisk {
	ulong start;
	ulong size;

#define RD_STATUS_INVALID            0x00
#define RD_STATUS_SETUP              0x01
#define RD_STATUS_RESERVE            0x02
#define RD_STATUS_VFS                0x03
	int status;
};

static struct ramdisk ramdisks[CONFIG_BLK_DEV_RAM_COUNT] = {{0}};
extern void dump_stack(void);

/*
 * initmrd=3,0x10000000,0x50000000
 */
/*****************************************************************************/
int parse_initmrd_bootargs(char *bootargs)
{
	unsigned int index;
	struct ramdisk *rd;
	char *endp;
	char *p;

	if (!bootargs) {
		pr_notice("invalid bootargs.\n");
		return 0;
	}

	p = strstr(bootargs, "initmrd=");
	if (!p) {
		pr_notice("found no initmrd.\n");
		return 0;
	}

	p += strlen("initmrd=");
	index = memparse(p, &endp);
	if (*endp != ',')
		return 0;

	if (index >= CONFIG_BLK_DEV_RAM_COUNT) {
		pr_notice("index %d out of range, CONFIG_BLK_DEV_RAM_COUNT is %d.\n", index, CONFIG_BLK_DEV_RAM_COUNT);
		return 0;
	}

	rd = &ramdisks[index];

	rd->start = memparse(endp + 1, &endp);
	if (*endp != ',')
		return 0;

	rd->size = memparse(endp + 1, NULL);
	if (rd->size)
		rd->status = RD_STATUS_SETUP;

	return 0;
}

/*****************************************************************************/

static int __init reserve_one_ramdisk(int index, struct ramdisk *rd)
{
	int ret;

	ret = fdt_add_memory_reserve((u64)rd->start, (u64)rd->size);
	if(ret) {
		pr_err("reserve memory(0x%lx~0x%lx) failed.\n", rd->start, rd->size);
		goto fail;
		
	}

	rd->status = RD_STATUS_RESERVE;
	rd->start = __phys_to_virt(rd->start);

	return 0;
fail:
	rd->status = RD_STATUS_INVALID;

	return -1;
}
/*****************************************************************************/

void __init initmrd_reserve_memory(void)
{
	int ix;
	struct ramdisk *rd;

	for (ix = 0; ix < CONFIG_BLK_DEV_RAM_COUNT; ix++) {
		rd = &ramdisks[ix];
		if (rd->status == RD_STATUS_SETUP)
			reserve_one_ramdisk(ix, rd);
	}
}
/******************************************************************************/

static int __init create_ramdisk(char *name, int n)
{
	sys_unlink(name);
	sys_mkdir(MRD_RAMDISK_DIR, 0600);
	return sys_mknod(name, S_IFBLK|0600, new_encode_dev(MKDEV(RAMDISK_MAJOR, n)));
}
/******************************************************************************/

static int __init init_one_ramdisk(int index, struct ramdisk *rd)
{
	int ret = -1;
	int fd_out;
	int sz_out;
	ulong remainder;
	ulong devsize = ~0UL;
	unsigned long offset, count;

	ret = create_ramdisk(MRD_RAMDISK_DIR"/ram", index);
	if (ret) {
		pr_err("create ramdisk %s device %d failed.\n", MRD_RAMDISK_DIR"/ram", index);
		return ret;
	}

	fd_out = sys_open(MRD_RAMDISK_DIR"/ram", O_RDWR, 0);
	if (fd_out < 0) {
		pr_err("open %s failed.\n", MRD_RAMDISK_DIR"/ram");
		ret = -1;
		goto out_fail;
	}

	if (!sys_ioctl(fd_out, BLKGETSIZE, (ulong)&devsize))
		devsize = devsize >> (BLOCK_SIZE_BITS - 9);

	remainder = devsize;
	offset = 0;
	count = SPLIT_SIZE;
	do {
		if ((offset + SPLIT_SIZE) > rd->size)
				count = rd->size - offset;

		sz_out = sys_write(fd_out, (char *)(rd->start + offset), count);
		if (sz_out != count) {
			pr_err("write fail");
			dump_stack();
			break;
		}

		free_reserved_area((char *)rd->start + offset, (char *)rd->start 
				   + offset + count, 0, MRD_NAME);
		offset += SPLIT_SIZE;
	} while (offset < rd->size);

	ret = 0;

out_fail:
	sys_close(fd_out);
	sys_unlink(MRD_RAMDISK_DIR"/ram");
	return ret;
}
/******************************************************************************/

static int __init init_ramdisk(void)
{
	int ix;
	struct ramdisk *rd;

	for (ix = 0; ix < CONFIG_BLK_DEV_RAM_COUNT; ix++) {
		rd = &ramdisks[ix];
		if (rd->status == RD_STATUS_RESERVE)
			init_one_ramdisk(ix, rd);
	}
	return 0;
}
late_initcall(init_ramdisk);
