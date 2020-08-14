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
#include <module.h>
#include <mmc_if.h>

#define MOULE_NAME                     "MMC: "
#include <debug.h>

/*****************************************************************************/

static int mmcdev_unif_open(uint64 from, uint64 length)
{
	if (mmcdev_check_align(from, &length))
		return -EINVAL;

	return 0;
}
/*****************************************************************************/

static void mmcdev_unif_close(struct flash_intf_t *intf)
{
	return;
}
/*****************************************************************************/

static int mmcdev_unif_check(struct flash_intf_t *intf, uint64 offset,
			       uint64 *length)
{
	if (!intf->info) {
		pr_error("please open this flash first.\n");
		return -EINVAL;
	}

	if (offset >= intf->length) {
		pr_error("offset(0x%llx) is out of partition size(0x%llx).\n",
			 offset, intf->length);
		return -EINVAL;
	}

	if (offset + (*length) > intf->length) {
		*length = intf->length - offset;

		pr_warn("length include offset is out of range, "
			"it is truncated to 0x%08llx.\n",
			*length);
	}

	return 0;
}
/*****************************************************************************/

static int64 mmcdev_unif_erase(struct flash_intf_t *intf, uint64 offset,
				 uint64 length)
{
	int ret;
	uint64 from;
	uint64 size;

	if (!length)
		return 0;

	ret = mmcdev_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = intf->from + offset;
	size = length;

	ret = mmcdev_erase(from, size);
	if (ret) {
		pr_error("erase mmcdev fail.\n");
		return ret;
	}
	return size;
}
/*****************************************************************************/

static int64 mmcdev_unif_write(struct flash_intf_t *intf, uint64 offset,
				 uint64 length, char *buf, int withoob)
{
	int64 ret;
	uint64 from;
	uint64 size;

	if (!length)
		return 0;

	ret = mmcdev_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = intf->from + offset;
	size = length;

	ret = mmcdev_write(from, buf, size);
	if (size != ret) {
		pr_error("write mmcdev fail.\n");
		return ret;
	}
	return size;
}
/*****************************************************************************/

static int64 mmcdev_unif_read(struct flash_intf_t *intf, uint64 offset,
				uint64 length, char *buf, int withoob)
{
	int64 ret;
	uint64 from;
	uint64 size;

	if (!length)
		return 0;

	ret = mmcdev_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = intf->from + offset;
	size = length;

	ret = mmcdev_read(from, buf, size);
	if (size != ret) {
		pr_error("read mmcdev fail.\n");
		return ret;
	}
	return size;
}
/*****************************************************************************/

static struct flash_ops_t mmcdev_ops = {
	.devname  = "mmcblk0",
	.open     = mmcdev_unif_open,
	.close    = mmcdev_unif_close,
	.erase    = mmcdev_unif_erase,
	.write    = mmcdev_unif_write,
	.read     = mmcdev_unif_read,
	.get_info = get_mmcdev_info,
};

static void mmc_unif_module_init(void *ptr)
{
	flash_register(FT_EMMC, &mmcdev_ops);
}

static struct module_t module_mmc_unif = {
	.priority = MODULE_PRI_DRV,
	.init = mmc_unif_module_init,
};
MODULE(module_mmc_unif);
