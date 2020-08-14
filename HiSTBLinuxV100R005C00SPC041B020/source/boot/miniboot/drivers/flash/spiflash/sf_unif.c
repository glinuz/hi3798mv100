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
#include <sf_if.h>

#define MOULE_NAME                     "SPIFlash: "
#include <debug.h>

/*****************************************************************************/

static int spiflash_unif_open(uint64 from, uint64 length)
{
	uint32 _from = (uint32)from;
	uint32 _length = (uint32)length;

	if (from >= _4G || length >= _4G) {
		pr_error("from(0x%llx) or length(0x%llx) is invalid.\n",
			 from, length);
		return -EINVAL;
	}

	if (spiflash_check_align(_from, &_length))
		return -EINVAL;

	return 0;
}
/*****************************************************************************/

static void spiflash_unif_close(struct flash_intf_t *intf)
{
	return;
}
/*****************************************************************************/

static int spiflash_unif_check(struct flash_intf_t *intf, uint64 offset,
			       uint64 *length)
{
	if (!intf->info) {
		pr_error("please open this flash first.\n");
		return -EINVAL;
	}

	if (offset >= _4G || *length >= _4G || !*length) {
		pr_error("offset(0x%llx) or length(0x%llx) is invalid.\n",
			 offset, *length);
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

static int64 spiflash_unif_erase(struct flash_intf_t *intf, uint64 offset,
				 uint64 length)
{
	int ret;
	uint32 from;
	uint32 size;

	if (!length)
		return 0;

	ret = spiflash_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = (uint32)(intf->from + offset);
	size = (uint32)length;

	ret = spiflash_erase(from, size);
	if (ret) {
		pr_error("erase spiflash fail.\n");
		return ret;
	}
	return (int64)size;
}
/*****************************************************************************/

static int64 spiflash_unif_write(struct flash_intf_t *intf, uint64 offset,
				 uint64 length, char *buf, int withoob)
{
	int ret;
	uint32 from;
	uint32 size;

	if (!length)
		return 0;

	ret = spiflash_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = (uint32)(intf->from + offset);
	size = (uint32)length;

	ret = spiflash_write(from, buf, size);
	if (ret) {
		pr_error("write spiflash fail.\n");
		return ret;
	}
	return (int64)size;
}
/*****************************************************************************/

static int64 spiflash_unif_read(struct flash_intf_t *intf, uint64 offset,
				uint64 length, char *buf, int withoob)
{
	int ret;
	uint32 from;
	uint32 size;

	if (!length)
		return 0;

	ret = spiflash_unif_check(intf, offset, &length);
	if (ret)
		return ret;

	from = (uint32)(intf->from + offset);
	size = (uint32)length;

	ret = spiflash_read(from, buf, size);
	if (ret) {
		pr_error("read spiflash fail.\n");
		return ret;
	}
	return (int64)size;
}
/*****************************************************************************/

static struct flash_ops_t spiflash_ops = {
	.devname = "hi_sfc",
	.open = spiflash_unif_open,
	.close = spiflash_unif_close,
	.erase = spiflash_unif_erase,
	.write = spiflash_unif_write,
	.read = spiflash_unif_read,
	.get_info = get_spiflash_info,
};

static void sf_unif_module_init(void *ptr)
{
	flash_register(FT_SPIFLASH, &spiflash_ops);
}

static struct module_t module_sf_unif = {
	.priority = MODULE_PRI_DRV,
	.init = sf_unif_module_init,
};
MODULE(module_sf_unif);
