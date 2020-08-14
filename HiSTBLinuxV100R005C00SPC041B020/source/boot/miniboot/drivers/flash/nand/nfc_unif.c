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
#include <nfc_if.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "nand_drv.h"

#define MOULE_NAME                     "NAND: "
#include <debug.h>

/*****************************************************************************/

static int nfc_unif_open(uint64 from, uint64 length)
{
	if (nand_check_align64(from, 0, &length, TRUE))
		return -EINVAL;
	return 0;
}
/*****************************************************************************/

static int nfc_unif_check(struct flash_intf_t *intf, uint64 offset,
			  uint64 *length, int alignblock)
{
	uint64 address;
	uint64 border;

	if (!intf->info) {
		pr_error("please open this flash first.\n");
		return -EINVAL;
	}

	address = intf->from + offset;
	border = intf->from + intf->length;

	return nand_check_align64(address, border, length, alignblock);
}
/*****************************************************************************/

static void nfc_unif_close(struct flash_intf_t *intf)
{
	return;
}
/*****************************************************************************/

static int64 nfc_unif_erase(struct flash_intf_t *intf, uint64 offset,
			    uint64 length)
{
	int ret;
	uint64 address;
	uint64 puresize;
	uint64 partborder;

	if (!length)
		return 0;

	ret = nfc_unif_check(intf, offset, &length, TRUE);
	if (ret)
		return ret;

	address = intf->from;
	partborder = intf->from + intf->length;

	puresize = nand_size_withbad64(&address, partborder, offset);
	if (puresize != offset) {
		pr_warn("offset out of range due to bad block.\n");
		return -EINVAL;
	}

	return nand_erase_block64(address, partborder, length, FALSE);
}
/*****************************************************************************/

static int64 nfc_unif_write(struct flash_intf_t *intf, uint64 offset,
			    uint64 length, char *buf, int withoob)
{
	int ret;
	uint64 address;
	uint64 puresize;
	uint64 partborder;
	uint32 oobsize = 0;

	if (!length)
		return 0;

	ret = nfc_unif_check(intf, offset, &length, FALSE);
	if (ret)
		return ret;

	address = intf->from;
	partborder = intf->from + intf->length;

	puresize = nand_size_withbad64(&address, partborder, offset);
	if (puresize != offset) {
		pr_warn("offset out of range due to bad block.\n");
		return -EINVAL;
	}

	ret = nand_write64(&address, partborder, buf, length, &oobsize,
		withoob, TRUE);
	if (ret <= 0)
		return ret;

	return (ret + oobsize);
}
/*****************************************************************************/

static int64 nfc_unif_read(struct flash_intf_t *intf, uint64 offset,
			   uint64 length, char *buf, int withoob)
{
	int ret;
	uint64 address;
	uint64 puresize;
	uint64 partborder;
	uint32 oobsize = 0;

	if (!length)
		return 0;

	ret = nfc_unif_check(intf, offset, &length, FALSE);
	if (ret)
		return ret;

	address = intf->from;
	partborder = intf->from + intf->length;

	puresize = nand_size_withbad64(&address, partborder, offset);
	if (puresize != offset) {
		pr_warn("offset out of range due to bad block.\n");
		return -EINVAL;
	}

	ret = nand_read64(&address, partborder, buf, length, &oobsize,
		withoob, TRUE);
	if (ret <= 0)
		return ret;

	return (ret + oobsize);
}
/*****************************************************************************/

static struct flash_ops_t nfc_ops = {
	.devname = "hinand",
	.open = nfc_unif_open,
	.close = nfc_unif_close,
	.erase = nfc_unif_erase,
	.write = nfc_unif_write,
	.read = nfc_unif_read,
	.get_info = get_nand_info,
};

static void nfc_unif_module_init(void *ptr)
{
	flash_register(FT_NAND, &nfc_ops);
}

static struct module_t module_nfc_unif = {
	.priority = MODULE_PRI_DRV,
	.init = nfc_unif_module_init,
};
MODULE(module_nfc_unif);
