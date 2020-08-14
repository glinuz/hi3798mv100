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

#include <compile.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <flash.h>
#include <string.h>

#define MOULE_NAME                     "flash: "
#include <debug.h>

#include "parts.h"
/*****************************************************************************/

struct flash_intf_ctrl_t {
	struct flash_intf_t intf[10];
};

static struct flash_intf_ctrl_t __intf_ctrl;
static struct flash_intf_ctrl_t *intf_ctrl = NULL;

struct flash_dev_t {
	const char *name;
	enum flash_type_t type;
	const char *devpart;
	struct flash_ops_t *ops;
};

static struct flash_dev_t flash_dev[] = {
	{ "SPIFlash", FT_SPIFLASH, "mtdparts", NULL },
	{ "NAND",     FT_NAND,     "mtdparts", NULL },
	{ "eMMC",     FT_EMMC,     "blkdevparts", NULL },
	{0},
};
/*****************************************************************************/

static struct flash_intf_t *get_new_flash_intf(void)
{
	int ix;

	if (!intf_ctrl) {
		intf_ctrl = &__intf_ctrl;
		memset(&__intf_ctrl, 0, sizeof(__intf_ctrl));
	}

	for (ix = 0; ix < ARRAY_SIZE(intf_ctrl->intf); ix++) {
		struct flash_intf_t *intf = &intf_ctrl->intf[ix];
		if (!intf->ops)
			return intf;
	}

	return NULL;
}
/*****************************************************************************/

static struct flash_dev_t *get_flash_dev(enum flash_type_t type)
{
	struct flash_dev_t *dev = flash_dev;
	while (dev->name) {
		if (dev->type == type) {
			return dev;
		}
		dev++;
	}
	return NULL;
}
/*****************************************************************************/

int flash_register(enum flash_type_t type, struct flash_ops_t *ops)
{
	struct flash_dev_t *dev = flash_dev;
	while (dev->name) {
		if (dev->type == type) {
			dev->ops = ops;
			return 0;
		}
		dev++;
	}
	return -1;
}
/*****************************************************************************/

const char *flash_get_name(enum flash_type_t type)
{
	struct flash_dev_t *dev = flash_dev;
	while (dev->name) {
		if (dev->type == type) {
			return dev->name;
		}
		dev++;
	}
	return "Unknown";
}
/*****************************************************************************/

void *flash_open_range(uint32 flash, uint64 from, uint64 length)
{
	struct flash_ops_t *flash_ops;
	struct flash_intf_t *flashif;
	struct flash_dev_t *flash_dev = get_flash_dev((enum flash_type_t)flash);

	if (!flash_dev || !flash_dev->ops) {
		pr_error("Not support this flash\n");
		return NULL;
	}
	flash_ops = flash_dev->ops;

	flashif = get_new_flash_intf();
	if (!flashif || flashif->ops) {
		pr_error("it has opened too many flash device.\n");
		return NULL;
	}

	if (flash_ops->open(from, length))
		return NULL;

	flashif->ops = flash_ops;
	flashif->from = from;
	flashif->length = length;
	flashif->info = flash_ops->get_info();

	return flashif;
}
/*****************************************************************************/

void *flash_open_part(uint32 flash, const char *partname)
{
	int ret;
	uint64 start;
	uint64 size;
	uint64 totalsize;
	struct flash_intf_t *flashif;
	struct flash_ops_t *flash_ops;
	struct flash_info_t *flash_info;
	struct flash_dev_t *flash_dev = get_flash_dev((enum flash_type_t)flash);

	if (!flash_dev || !flash_dev->ops) {
		return NULL;
	}
	flash_ops = flash_dev->ops;

	flashif = get_new_flash_intf();
	if (!flashif || flashif->ops) {
		pr_error("it has opened too many flash device.\n");
		return NULL;
	}

	ret = flash_find_part(flash_dev->devpart, flash_ops->devname,
		partname, &start, &size);
	if (ret)
		return NULL;

	flash_info = flash_ops->get_info();
	totalsize = flash_info->chipsize * flash_info->nr_chips;
	if (size > totalsize)
		size = totalsize;

	if (flash_ops->open(start, size))
		return NULL;

	flashif->ops = flash_ops;
	flashif->from = start;
	flashif->length = size;
	flashif->info = flash_info;

	return flashif;
}
/*****************************************************************************/

void flash_close(void *intf)
{
	struct flash_intf_t *flashif = (struct flash_intf_t *)intf;

	flashif->ops->close(flashif);

	flashif->from = 0;
	flashif->length = 0;
	flashif->info = NULL;
	flashif->ops = NULL;
}
/*****************************************************************************/

int64 flash_erase(void *intf, uint64 offset, uint64 length)
{
	struct flash_intf_t *flashif = (struct flash_intf_t *)intf;
	return flashif->ops->erase(flashif, offset, length);
}
/*****************************************************************************/

int64 flash_write(void *intf, uint64 offset, uint64 length,
		  char *buf, int withoob)
{
	struct flash_intf_t *flashif = (struct flash_intf_t *)intf;
	return flashif->ops->write(flashif, offset, length, buf, withoob);
}
/*****************************************************************************/

int64 flash_read(void *intf, uint64 offset, uint64 length,
		 char *buf, int withoob)
{
	struct flash_intf_t *flashif = (struct flash_intf_t *)intf;
	return flashif->ops->read(flashif, offset, length, buf, withoob);
}
/*****************************************************************************/

void flash_show_info(struct flash_info_t *info)
{
	int ix;

	switch(info->type) {
	case FT_SPIFLASH:
		pr_tool("Block:%sB ", u32tohstr(info->blocksize, NULL));
		pr_tool("Chip:%sB*%d ",
			u64tohstr(info->chipsize, NULL),
			info->nr_chips);
		pr_tool("\nID:");

		for (ix = 0; ix < info->nr_ids; ix++)
			pr_tool("0x%02X ", info->ids[ix]);

		pr_tool("\nName:\"%s\"\n", info->name);

		break;
	case FT_NAND:
		pr_tool("Block:%sB ", u32tohstr(info->blocksize, NULL));
		pr_tool("Chip:%sB*%d ",
			u64tohstr(info->chipsize, NULL),
			info->nr_chips);

		pr_tool("Page:%sB ", u32tohstr(info->pagesize, NULL));
		pr_tool("OOB:%sB ", u32tohstr(info->oobused, NULL));

		pr_tool("\nID:");

		for (ix = 0; ix < info->nr_ids; ix++)
			pr_tool("0x%02X ", info->ids[ix]);

		pr_tool("\nName:\"%s\"\n", info->name);

		break;
	default:
		printf("Unknown flash type: %d\n", info->type);
		break;
	}
}
/*****************************************************************************/

const void *flash_get_info(uint32 flash)
{
	struct flash_dev_t *flash_dev = get_flash_dev(flash);

	if (!flash_dev || !flash_dev->ops) {
		pr_error("Not support this flash.\n");
		return NULL;
	}

	return (void *)flash_dev->ops->get_info();
}
