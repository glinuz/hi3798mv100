/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.7.25
 *
******************************************************************************/

#include <linux/string.h>

#include "hiflash.h"

/*****************************************************************************/

u32 nand_get_clock(struct nand_chip_clock *clks, u8 *id, int id_len)
{
	while (clks->id_len) {
		if (clks->id_len <= id_len &&
		    !memcmp(id, clks->id, clks->id_len))
			return clks->value;
		clks++;
	}

	/* use default value */
	return clks->value;
}
/******************************************************************************/

struct nand_ctrl_info *nand_get_ctrl_info(struct nand_ctrl_info *info,
					  int pagesize, int ecc_strength)
{
	while (info->pagesize) {
		if (info->pagesize == pagesize &&
		    info->ecc.strength == ecc_strength) {
			if (!info->nr_ecc_sect)
				info->nr_ecc_sect = info->pagesize / info->ecc.step;
			return info;
		}
		info++;
	}

	return NULL;
}
/******************************************************************************/

static struct spiflash_xfer *spinor_xfer_read =
	SPIFLASH_XFER(SPINOR_OP_READ, 0, 0, 0);

static struct spiflash_xfer *spinor_xfer_write =
	SPIFLASH_XFER(SPINOR_OP_PP, 0, 256, 0);

static struct spiflash_xfer *spinor_xfer_erase =
	SPIFLASH_XFER(SPINOR_OP_SE, 0, SZ_64K, 0);

struct spinor_chip_xfer *spinor_get_xfer(struct spinor_chip_xfer *xfer,
					 u8 *id, int id_len)
{
	/* xfer->id_len == 0 means default value */
	for (; xfer->id_len; xfer++) {
		/* allow match a serial device. */
		if (xfer->id_len > id_len)
			continue;

		if (memcmp(xfer->id, id, xfer->id_len))
			continue;

		break;
	}

	if (!xfer->read)
		xfer->read = spinor_xfer_read;
	if (!xfer->write)
		xfer->write = spinor_xfer_write;
	if (!xfer->erase)
		xfer->erase = spinor_xfer_erase;

	return xfer;
}
/******************************************************************************/

struct spinand_chip_xfer *spinand_get_xfer(struct spinand_chip_xfer *xfer,
					   u8 *id, int id_len)
{
	struct spinand_chip_xfer *best = NULL;

	for (; xfer->id_len; xfer++) {
		/* allow match a serial device. */
		if (xfer->id_len > id_len ||
		    memcmp(xfer->id, id, xfer->id_len))
			continue;

		if (!best || best->id_len < xfer->id_len)
			best = xfer;
	}

	if (best)
		return best;
	else
		return xfer; /* xfer->id_len == 0 means default value */
}
/******************************************************************************/

int flash_regop_info_dump(struct flash_regop_info *info, char *buf, int sz_buf)
{
	int count;
	char *ptr = buf;

	if (info->nr_cmd) {
		if (info->nr_cmd == 1)
			count = snprintf(ptr, sz_buf, "cmd=0x%02x,", info->cmd);
		else if (info->nr_cmd == 2)
			count = snprintf(ptr, sz_buf, "cmd=0x%04x,", info->cmd);
		else
			count = snprintf(ptr, sz_buf, "cmd=0x%08x,", info->cmd);

		sz_buf -= count;
		ptr += count;

		count = snprintf(ptr, sz_buf, "nr_cmd=%d,", info->nr_cmd);
		sz_buf -= count;
		ptr += count;
	}

	if (info->addr_cycle) {
		if (info->addr_cycle > 4) {
			count = snprintf(ptr, sz_buf, "addrh=0x%02x,", info->addrh);
			sz_buf -= count;
			ptr += count;
		}

		if (info->addr_cycle > 2)
			count = snprintf(ptr, sz_buf, "addrl=0x%08x,", info->addrl);
		else
			count = snprintf(ptr, sz_buf, "addrl=0x%04x,", info->addrl);

		sz_buf -= count;
		ptr += count;

		count = snprintf(ptr, sz_buf, "addr_cycle=%d,", info->addr_cycle);
		sz_buf -= count;
		ptr += count;
	}

	if (info->dummy) {
		count = snprintf(ptr, sz_buf, "dummy=%d,", info->dummy);
		sz_buf -= count;
		ptr += count;
	}

	if (info->sz_buf) {
		count = snprintf(ptr, sz_buf, "sz_buf=%d,", info->sz_buf);
		sz_buf -= count;
		ptr += count;
	}

	return ptr - buf;
}
