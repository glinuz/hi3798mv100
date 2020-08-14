/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai Yingjun
 *
 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/


#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <module.h>
#include <flash.h>
#include "mmc_drv.h"
#include <debug.h>

#define MMC_RW_STEP_BLKS (65536 - 1) /* 0xFFFF is the max blocks of CMD23 */

static struct mmc_dev_t *mmcdev = NULL;
mmc_driver_init_fn mmcdev_driver_init = NULL;

static struct flash_info_t mmcdev_info = {.type = FT_NONE};
/******************************************************************************/

static int set_mmcdev_info(uint64 chipsize, uint32 blocksize, uint32 nr_chips,
						   const char *name)
{
	mmcdev_info.type = FT_EMMC;
	strncpy(mmcdev_info.name, name, sizeof(mmcdev_info.name));
	mmcdev_info.name[sizeof(mmcdev_info.name) - 1] = '\0';
	mmcdev_info.blocksize = blocksize;
	mmcdev_info.pagesize = blocksize;
	mmcdev_info.chipsize = chipsize;
	mmcdev_info.nr_chips = nr_chips;

	return 0;
}


/******************************************************************************/

static int mmcdev_init(void)
{
	if (mmcdev)
		return 0;

	if (!mmcdev_driver_init)
		return -ENODEV;

	mmcdev = mmcdev_driver_init();

	if (!mmcdev)
		return -ENODEV;

	if (set_mmcdev_info(mmcdev->capacity, mmcdev->blocksize, 1, mmcdev->name) < 0 )
		return -EINVAL;

	return 0;
}

/******************************************************************************/

struct flash_info_t *get_mmcdev_info(void)
{
	if (mmcdev_info.type != FT_EMMC)
		mmcdev_init();
	return &mmcdev_info;
}
/******************************************************************************/

int mmcdev_check_range(uint64 offset, uint64 *length)
{
	uint64 totalsize;
	struct flash_info_t *mmcinfo;

	ASSERT(length != NULL);

	mmcinfo = get_mmcdev_info();
	if (mmcinfo->type != FT_EMMC)
		return -ENODEV;

	totalsize = mmcinfo->chipsize * mmcinfo->nr_chips;
	if (offset >= totalsize) {
		pr_error("offset(0x%llX) is out of mmcdev range(0x%llX).\n",
			offset, totalsize);
		return -EINVAL;
	}

	if (offset + (*length) > totalsize) {
		*length = totalsize - offset;
		pr_warn("length is out of range, it is truncated to 0x%llX.\n",
			*length);
	}

	return 0;
}
/******************************************************************************/

int mmcdev_check_align(uint64 offset, uint64 *length)
{
	int ret;
	uint32 blksize_mask;

	ret = mmcdev_check_range(offset, length);
	if (ret)
		return ret;

	blksize_mask = get_mmcdev_info()->blocksize - 1;

	if (offset & (uint64)blksize_mask) {
		pr_error("offset(0x%016llx) must align with blocksize(0x%08x).\n",
			offset, get_mmcdev_info()->blocksize);
		return -EINVAL;
	}

	return 0;
}

/******************************************************************************/

int64 mmcdev_write(uint64 from, const char *buf, uint64 size)
{
	int ret;
	uint32 blk_start;
	uint32 blk_cnt;
	uint32 last_bytes = 0;
	uint64 write_bytes = 0;
	int cnt;
	char *databuf = (char *)buf;
	uint32 blkaddr;

	ret = mmcdev_check_align(from, &size);
	if (ret)
		return ret;

	blk_start = from / mmcdev->blocksize;
	blk_cnt   = size / mmcdev->blocksize;

	cnt = blk_cnt;
	blkaddr = blk_start;
	write_bytes = 0;
	while (cnt > 0) {
		uint32 step;

		if (cnt > MMC_RW_STEP_BLKS) {
			step = MMC_RW_STEP_BLKS;
			cnt -= MMC_RW_STEP_BLKS;
		} else {
			step = cnt;
			cnt = 0;
		}

		ret = mmcdev->write(mmcdev, blkaddr, step, databuf);
		if (ret != step)
			goto out;

		write_bytes += mmcdev->blocksize * step;
		databuf += mmcdev->blocksize * step;
		blkaddr += step;
	}

	last_bytes = size % mmcdev->blocksize;	
	if (last_bytes) {
		const char *last_data = buf + mmcdev->blocksize * blk_cnt;
		char *tmp = zmalloc(mmcdev->blocksize);
		if (!tmp) {
			pr_error("Fail to alloc %d\n", mmcdev->blocksize);
			return -ENOMEM;
		}
		memcpy(tmp, last_data, last_bytes);
		mmcdev->write(mmcdev, blk_start + blk_cnt, 1, tmp);
		free(tmp);
		write_bytes += last_bytes;
	}

out:
	return write_bytes;
}
/******************************************************************************/

int64 mmcdev_read(uint64 from, char *buf, uint64 size)
{
	int ret;
	uint32 blk_start;
	uint32 blk_cnt;
	uint32 last_bytes = 0;
	uint64 read_bytes = 0;
	int cnt;
	char *databuf = buf;
	uint32 blkaddr;

	ret = mmcdev_check_align(from, &size);
	if (ret)
		return ret;

	blk_start = from / mmcdev->blocksize;
	blk_cnt   = size / mmcdev->blocksize;

	cnt = blk_cnt;
	blkaddr = blk_start;
	read_bytes = 0;
	while (cnt > 0) {
		uint32 step;
	
		if (cnt > MMC_RW_STEP_BLKS) {
			step = MMC_RW_STEP_BLKS;
			cnt -= MMC_RW_STEP_BLKS;
		} else {
			step = cnt;
			cnt = 0;
		}

		ret = mmcdev->read(mmcdev, blkaddr, step, databuf);
		if (ret != step)
			goto out;

		read_bytes += mmcdev->blocksize * step;
		databuf += mmcdev->blocksize * step;
		blkaddr += step;
	}

	last_bytes = size % mmcdev->blocksize;	
	if (last_bytes) {
		char *last_data = buf + mmcdev->blocksize * blk_cnt;
		char *tmp = malloc(mmcdev->blocksize);
		if (!tmp) {
			pr_error("Fail to alloc %d\n", mmcdev->blocksize);
			return -ENOMEM;
		}
		mmcdev->read(mmcdev, blk_start + blk_cnt, 1, tmp);
		memcpy(last_data, tmp, last_bytes);
		free(tmp);
		read_bytes += last_bytes;
	}

out:	
	return read_bytes;
}
/******************************************************************************/

int mmcdev_erase(uint64 from, uint64 size)
{
	return 0;
}

/******************************************************************************/
struct mmc_dev_t *get_mmcdev(int devnum)
{
	return mmcdev;
}

/******************************************************************************/

static void mmc_if_module_init(void *ptr)
{
	mmcdev_init();
}

static struct module_t module_mmc_if = {
	.priority = MODULE_PRI_DRV,
	.init = mmc_if_module_init,
};
MODULE(module_mmc_if);
