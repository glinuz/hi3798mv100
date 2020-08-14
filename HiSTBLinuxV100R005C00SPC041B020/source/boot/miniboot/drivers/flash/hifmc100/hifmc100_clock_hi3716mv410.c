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

#include <asm/io.h>

#include "hifmc100_host.h"
#include "hiflash.h"
/******************************************************************************/

#define HIFMC100_CRG23				0x5C
#define CRG23_SPI_NAND_CLK_SEL(_clk)		(((_clk) & 0x7) << 4)
#define CRG23_SPI_NAND_CLK_EN			(1 << 0)
#define CRG23_SPI_NAND_SOFT_RST_REQ		(1 << 1)

#define HIFMC100_SPI_NAND_CLK_SEL_MASK		(0x7 << 4)
#define HIFMC100_SPI_NAND_CLK_SEL_12M		0x40
#define HIFMC100_SPI_NAND_CLK_SEL_37M		0x70
#define HIFMC100_SPI_NAND_CLK_SEL_50M		0x60
#define HIFMC100_SPI_NAND_CLK_SEL_62M		0x50
#define HIFMC100_SPI_NAND_CLK_SEL_75M		0x30
#define HIFMC100_SPI_NAND_CLK_SEL_86M		0x20
#define HIFMC100_SPI_NAND_CLK_SEL_100M		0x10
#define HIFMC100_SPI_NAND_CLK_SEL_200M		0x00
/******************************************************************************/

struct nand_chip_clock hifmc100_nand_rwlatch[] = {
	/* id_len == 0, means default clock, don't remove. */
	{ .id     = {0x00}, .id_len = 0, .value  = 0x457, }
};
/******************************************************************************/

struct spinand_chip_xfer hifmc100_spinand_chip_xfer[] = {
	{"AFS4GQ4UAGWC4",            {0xC2, 0xD4}, 2, 4, 24},
	{"AFS2GQ4UADWC2",            {0xC1, 0x52}, 2, 4, 24},
	{"AFS1GQ4UACWC2",            {0xC1, 0x51}, 2, 4, 50},
	{"AFS1GQ4UAAWC2",            {0xC8, 0x31}, 2, 4, 24},
	{"GD5F2GQ4UAYIG",            {0xC8, 0xF2}, 2, 4, 40},
	{"F50L1G41A/PSU1GS20BN",     {0xC8, 0x21}, 2, 4, 40},
	{"HYF1GQ4UAACAE",            {0xC9, 0x51}, 2, 4, 24},
	{"ATO25D1GA",                {0x9B, 0x12}, 2, 4, 24},
	{"MX35LF1GE4AB-Z2I",         {0xC2, 0x12}, 2, 4, 24},
	{"W25N01GV",                 {0xEF, 0xAA, 0x21}, 3, 4, 24},
	{"FM25G01A",                 {0xA1, 0xE1}, 2, 4, 24},
	{"HYF1GQ4UAACA6-C",          {0xC9, 0x31}, 2, 4, 24},
	/* id_len == 0, means default value, don't remove. */
	{NULL, {0x00}, 0, 4, 100},
};
/******************************************************************************/

struct spinor_chip_xfer hifmc100_spinor_chip_xfer[] = {
	{ "MX25L8006E", {0xc2, 0x20, 0x14}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80), },

	{ "MX25L6406E", {0xc2, 0x20, 0x17}, 3,
		/* .read = SPINOR_XFER(SPINOR_OP_READ_QUAD, 1, 0, 0), */ },

	{ "GD25Q128", {0xC8, 0x40, 0x18}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 50),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 50),},

	{ "GD25Q64", {0xC8, 0x40, 0x17}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 80),},

	{ "GD25Q32", {0xC8, 0x40, 0x16}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 80),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 80),},

	{ "W25Q128BV", {0xEF, 0x40, 0x18}, 3,
		.read = SPIFLASH_XFER(SPINOR_OP_READ_QUAD, 1, 0, 50),
		.write =  SPIFLASH_XFER(SPINOR_OP_PP_QUAD, 0, 256, 50),},

	/* id_len == 0, means default value, don't remove. */
	{ NULL, {0}, 0, NULL }
};
/*****************************************************************************/

int hifmc100_clock2reg(unsigned int value, int _to_reg)
{
	int ix;
	unsigned int sysclk[] = {
		12,  HIFMC100_SPI_NAND_CLK_SEL_12M,
		38,  HIFMC100_SPI_NAND_CLK_SEL_37M,
		50,  HIFMC100_SPI_NAND_CLK_SEL_50M,
		62,  HIFMC100_SPI_NAND_CLK_SEL_62M,
		75,  HIFMC100_SPI_NAND_CLK_SEL_75M,
		86,  HIFMC100_SPI_NAND_CLK_SEL_86M,
		100, HIFMC100_SPI_NAND_CLK_SEL_100M,
		200, HIFMC100_SPI_NAND_CLK_SEL_200M,
	};

	if (_to_reg) {
		if (value < 12)
			return HIFMC100_SPI_NAND_CLK_SEL_12M;
		else if (value >= 200)
			return HIFMC100_SPI_NAND_CLK_SEL_200M;

		for (ix = 0; ix < ARRAY_SIZE(sysclk); ix += 2)
			if (value < sysclk[ix])
				return sysclk[ix-1];
	} else {
		for (ix = 0; ix < ARRAY_SIZE(sysclk); ix += 2)
			if (sysclk[ix+1] == value)
				return sysclk[ix];
		return HIFMC100_SPI_NAND_CLK_SEL_12M;
	}

	return -1;
}
/******************************************************************************/

int hifmc100_set_system_clock(struct hifmc_host *host, unsigned int clk, int clk_en)
{
	u32 regval = HIFMC100_SPI_NAND_CLK_SEL_12M;
	if (clk) {
		clk /= 1000000;
		regval = hifmc100_clock2reg(clk, true);
	}

	if (clk_en) {
		regval |= CRG23_SPI_NAND_CLK_EN;

		if (readl(REG_BASE_CRG + HIFMC100_CRG23) != regval)
			writel(regval, (REG_BASE_CRG + HIFMC100_CRG23));
	} else {
		regval &= ~CRG23_SPI_NAND_CLK_EN;
		writel(regval, (REG_BASE_CRG + HIFMC100_CRG23));
	}

	return 0;
}
