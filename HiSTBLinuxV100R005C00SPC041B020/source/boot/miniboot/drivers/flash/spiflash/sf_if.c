/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
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
#include <module.h>
#include <mmu.h>
#include <sf_if.h>

#include "sf_drv.h"

#define MOULE_NAME                     "SPIFlash: "
#include <debug.h>

static struct spiflash_dev_t *spiflash = NULL;
spiflash_driver_init_fn spiflash_driver_init = NULL;
static struct flash_info_t spiflash_info = { 0 };

extern unsigned int cache_enabled;
/******************************************************************************/

static int spiflash_init(void)
{
	unsigned int cache_enabled_old = cache_enabled;

	if (spiflash)
		return 0;

	if (!spiflash_driver_init)
		goto init_failed;

	if (cache_enabled)
		mmu_cache_disable();

	spiflash = spiflash_driver_init();
	if (!spiflash)
		goto init_failed;

	return 0;

init_failed:
	if (cache_enabled_old)
		mmu_cache_enable();

	return -ENODEV;
}
/******************************************************************************/

int spiflash_write(uint32 from, const char *buf, uint32 size)
{
	int ret;

	ret = spiflash_check_range(from, &size);
	if (ret)
		return ret;

	return spiflash->write(spiflash, from, size, buf);
}
/******************************************************************************/

int spiflash_read(uint32 from, char *buf, uint32 size)
{
	int ret;

	ret = spiflash_check_range(from, &size);
	if (ret)
		return ret;

	return spiflash->read(spiflash, from, size, buf);
}
/******************************************************************************/

int spiflash_erase(uint32 from, uint32 size)
{
	int ret;

	ret = spiflash_check_align(from, &size);
	if (ret)
		return ret;

	return spiflash->erase(spiflash, from, size);
}
/******************************************************************************/

struct flash_info_t *get_spiflash_info(void)
{
	if (spiflash_info.type != FT_SPIFLASH)
		spiflash_init();
	return &spiflash_info;
}
/******************************************************************************/

int set_spiflash_info(uint32 chipsize, uint32 blocksize, uint32 nr_chips,
		      const char *name, u_char *ids, uint32 nr_ids)
{
	spiflash_info.type = FT_SPIFLASH;
	strncpy(spiflash_info.name, name, sizeof(spiflash_info.name));
	spiflash_info.name[sizeof(spiflash_info.name) - 1] = '\0';
	spiflash_info.pagesize = 1;
	spiflash_info.blocksize = blocksize;
	spiflash_info.chipsize = chipsize;
	spiflash_info.nr_chips = nr_chips;
	spiflash_info.nr_ids = min(nr_ids, sizeof(spiflash_info.ids));
	memcpy(spiflash_info.ids, ids, nr_ids);

	return 0;
}
/******************************************************************************/

int spiflash_check_range(uint32 offset, uint32 *length)
{
	uint32 totalsize;
	struct flash_info_t *spiinfo;

	ASSERT(length != NULL);

	spiinfo = get_spiflash_info();
	if (spiinfo->type != FT_SPIFLASH)
		return -ENODEV;

	totalsize = spiinfo->chipsize * spiinfo->nr_chips;
	if (offset >= totalsize) {
		pr_error("offset(0x%08x) is out of spiflash range(0x%08x).\n",
			 offset, totalsize);
		return -EINVAL;
	}

	if (offset + (*length) > totalsize) {
		*length = totalsize - offset;
		pr_warn("length is out of range, it is truncated to 0x%08x.\n",
			*length);
	}

	return 0;
}
/******************************************************************************/

int spiflash_check_align(uint32 offset, uint32 *length)
{
	int ret;
	uint32 blksize_mask;

	ret = spiflash_check_range(offset, length);
	if (ret)
		return ret;

	blksize_mask = get_spiflash_info()->blocksize - 1;

	if (offset & blksize_mask) {
		pr_error("offset(0x%08x) must align with blocksize.\n",
			 offset);
		return -EINVAL;
	}

	if (*length & blksize_mask) {
		pr_error("length(0x%08x) must align with blocksize.\n",
			 *length);
		return -EINVAL;
	}

	return 0;
}
/******************************************************************************/

static void sf_if_module_init(void *ptr)
{
	spiflash_init();
}

static struct module_t module_sf_if = {
	.priority = MODULE_PRI_ENV,
	.init = sf_if_module_init,
};
MODULE(module_sf_if);
