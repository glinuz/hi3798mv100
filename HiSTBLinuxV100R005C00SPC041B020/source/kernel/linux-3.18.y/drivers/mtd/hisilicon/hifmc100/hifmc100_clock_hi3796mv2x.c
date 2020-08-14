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

#include <hiflash.h>
#include <linux/hisilicon/freq.h>
/******************************************************************************/

struct nand_chip_clock hifmc100_nand_rwlatch[] = {
	/* id_len == 0, means default clock, don't remove. */
	{ .id     = {0x00}, .id_len = 0, .value  = 0x457, }
};
/******************************************************************************/

struct spinand_chip_xfer hifmc100_spinand_chip_xfer[] = {
	{"AFS4GQ4UAGWC4", {0xc2, 0xd4}, 2, 4, _50MHz},
	{"AFS2GQ4UADWC2", {0xc1, 0x52}, 2, 4, _50MHz},
	{"AFS1GQ4UACWC2", {0xc1, 0x51}, 2, 4, _50MHz},
	{"AFS1GQ4UAAWC2", {0xc8, 0x31}, 2, 4, _50MHz},
	{"GD5F2GQ4UAYIG", {0xC8, 0xF2}, 2, 4, _50MHz},

	/* id_len == 0, means default value, don't remove. */
	{NULL, {0x00}, 0, 4, _100MHz},
};
/******************************************************************************/

struct spinor_chip_xfer hifmc100_spinor_chip_xfer[] = {
	{ "MX25L8006E", {0xc2, 0x20, 0x14}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80), },

	{ "MX25L6406E", {0xc2, 0x20, 0x17}, 3,
		/* .read = SPINOR_XFER(SPINOR_OP_READ_QUAD, 1, 0, 0), */ },

	{ "GD25Q128", {0xC8, 0x40, 0x18}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 80),},

	{ "GD25Q64", {0xC8, 0x40, 0x17}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 80),},

	{ "GD25Q32", {0xC8, 0x40, 0x16}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 80),},

	/* id_len == 0, means default value, don't remove. */
	{ NULL, {0}, 0, NULL }
};
