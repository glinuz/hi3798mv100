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
#include <string.h>
#include "nand_chip.h"

#define _768K           (_256K + _512K)

/*****************************************************************************/

static struct nand_chip_info_t *hynix_probe_v02(struct nand_chip_info_t *info,
						unsigned char id[8])
{
	int pagesizes[]   = {_2K, _4K, _8K, 0};
	int oobsizes[]    = {128, 224, 448, 0, 0, 0, 0, 0};
	int blocksizes[]  = {_128K, _256K, _512K, _768K, _1M, _2M, 0, 0};

	int blocktype = (((id[3] >> 5) & 0x04) | ((id[3] >> 4) & 0x03));
	int oobtype   = (((id[3] >> 2) & 0x03) | ((id[3] >> 4) & 0x04));

	info->options   = 0;
	info->pagesize  = pagesizes[(id[3] & 0x03)];
	info->erasesize = blocksizes[blocktype];
	info->oobsize = oobsizes[oobtype];

	return info;
}
/*****************************************************************************/

static struct nand_chip_info_t *samsung_probe_v02(struct nand_chip_info_t *info,
						  unsigned char id[8])
{
	int pagesizes[]   = {_2K, _4K, _8K, 0};
	int oobsizes[]    = {0, 128, 218, 400, 436, 0, 0, 0};
	int blocksizes[]  = {_128K, _256K, _512K, _1M, 0, 0, 0, 0};

	int blocktype = (((id[3] >> 5) & 0x04) | ((id[3] >> 4) & 0x03));
	int oobtype   = (((id[3] >> 4) & 0x04) | ((id[3] >> 2) & 0x03));

	info->options   = 0;
	info->pagesize  = pagesizes[(id[3] & 0x03)];
	info->erasesize = blocksizes[blocktype];
	info->oobsize = oobsizes[oobtype];

	return info;
}
/*****************************************************************************/
/*
 * samsung:  27nm need randomizer, 21nm need read retry;
 * micron:   25nm need read retry, datasheet will explain read retry.
 * toshaba   32nm need randomizer, 24nm need read retry.
 * hynix:    2xnm need read retry.
 */
static struct nand_chip_info_t nand_chip_info[] =
{
	{        /* MLC 40bit/1k */
		.name      = "MT29F64G08CBABA",
		.id        = {0x2C, 0x64, 0x44, 0x4B, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 744,
		.options   = 0,
		.read_retry_type = NAND_RR_MICRON,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = NAND_RANDOMIZER | NAND_CHIP_MICRON,
	},
	{        /* MLC 40bit/1k */
		.name      = "MT29F32G08CBADA",
		.id        = {0x2C, 0x44, 0x44, 0x4B, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 744,
		.options   = 0,
		.read_retry_type = NAND_RR_MICRON,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{        /* SLC 4bit/512 */
		.name      = "MT29F8G08ABxBA",
		.id        = {0x2C, 0x38, 0x00, 0x26, 0x85, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _1G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _512K,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 12bit/512 */
		.name      = "MT29F16G08CBABx",
		.id        = {0x2C, 0x48, 0x04, 0x46, 0x85, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _1M,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k */
		.name      = "MT29F16G08CBACA",
		.id        = {0x2C, 0x48, 0x04, 0x4A, 0xA5, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _1M,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k */
		.name      = "MT29F32G08CBACA",
		.id        = {0x2C, 0x68, 0x04, 0x4A, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _1M,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k */
		.name      = "MT29F64G08CxxAA",
		.id        = {0x2C, 0x88, 0x04, 0x4B, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 448,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k 2CE */
		.name      = "MT29F256G08CJAAA",
		.id        = {0x2C, 0xA8, 0x05, 0xCB, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _16G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 448,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* MLC 40bit/1k */
		.name      = "MT29F256G08CMCBB",
		.id        = {0x2C, 0x64, 0x44, 0x4B, 0xA9, 0x00, 0x00, 0x00},
		.length    = 8,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 744,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* SLC 8bit/512 */
		.name      = "MT29F8G08ABACA",
		.id        = {0x2C, 0xD3, 0x90, 0xA6, 0x64, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _1G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _256K,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* SLC 8bit/512 */
		.name      = "MT29F4G08ABAEA",
		.id        = {0x2C, 0xDC, 0x90, 0xA6, 0x54, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _512M,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _256K,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* SLC 8bit/512 */
		.name      = "MT29F2G08ABAFA",
		.id        = {0x2C, 0xDA, 0x90, 0x95, 0x04, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _256M,
		.probe     = NULL,
		.pagesize  = _2K,
		.erasesize = _128K,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},
	{        /* SLC 8bit/512 */
		.name      = "MT29F16G08ABACA",
		.id        = {0x2C, 0x48, 0x00, 0x26, 0xA9, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _512K,
		.oobsize   = 224,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},

	/****************************** Toshaba *******************************/

	{       /* MLC 24bit/1k 32nm */
		.name      = "TC58NVG4D2FTA00",
		.id        = {0x98, 0xD5, 0x94, 0x32, 0x76, 0x55, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 448,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 24bit/1k 32nm 2CE*/
		.name      = "TH58NVG6D2FTA20",
		.id        = {0x98, 0xD7, 0x94, 0x32, 0x76, 0x55, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 448,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 40bit/1k 24nm */
		.name      = "TC58NVG5D2HTA00 24nm",
		.id        = {0x98, 0xD7, 0x94, 0x32, 0x76, 0x56, 0x08, 0x00},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2012,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{       /* MLC 40bit/1k */
		.name      = "TC58NVG6D2GTA00",
		.id        = {0x98, 0xDE, 0x94, 0x82, 0x76, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 19nm */
		.name      = "TC58NVG6DCJTA00 19nm",
		.id        = {0x98, 0xDE, 0x84, 0x93, 0x72, 0x57, 0x08, 0x04},
		.length    = 8,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _16K,
		.erasesize = _4M,
		.oobsize   = 1280,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2012,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{       /* MLC 19nm */
		.name      = "TC58TEG5DCJTA00 19nm",
		.id        = {0x98, 0xD7, 0x84, 0x93, 0x72, 0x57, 0x08, 0x04},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _16K,
		.erasesize = _4M,
		.oobsize   = 1280,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2012,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER | NAND_CHIP_TOSHIBA_TOGGLE_10,
	},
	{	/* TLC 60bit/1k 19nm */
		.name	   = "TC58TEG5DCKTAx0 19nm MLC",
		/* datasheet says 6 bytes id data, but really has 8 bytes. */
		.id	   = {0x98, 0xD7, 0x84, 0x93, 0x72, 0x50, 0x08, 0x04},
		.length    = 6,
		.chipsize  = _4G,
		.probe	   = NULL,
		.pagesize  = _16K,
		.erasesize = _4M,
		.oobsize   = 1280,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2013,
		.badblock_pos	 = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{
		.name	   = "Tx58TEGxDDKTAx0 19nm MLC",
		.id	   = {0x98, 0xDE, 0x94, 0x93, 0x76, 0x50},
		.length    = 6,
		.chipsize  = _4G,
		.probe	   = NULL,
		.pagesize  = _16K,
		.erasesize = _4M,
		.oobsize   = 1280,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2013,
		.badblock_pos	 = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},

	{       /* SLC 8bit/512 */
		.name      = "TC58NVG0S3HTA00",
		.id        = {0x98, 0xF1, 0x80, 0x15, 0x72, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _128M,
		.probe     = NULL,
		.pagesize  = _2K,
		.erasesize = _128K,
		.oobsize   = 128,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		/*
		 * Datasheet: read one column of any page in each block. If the
		 * data of the column is 00 (Hex), define the block as a bad
		 * block.
		 */
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},

	{       /* SLC 8bit/512 */
		.name      = "TC58NVG1S3HTA00",
		.id        = {0x98, 0xDA, 0x90, 0x15, 0x76, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _256M,
		.probe     = NULL,
		.pagesize  = _2K,
		.erasesize = _128K,
		.oobsize   = 128,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE,
		.flags = 0,
	},

	{       /* TLC 60bit/1k 19nm */
		.name      = "TC58NVG5T2JTA00 19nm TLC",
		.id        = {0x98, 0xD7, 0x98, 0x92, 0x72, 0x57, 0x08, 0x10},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _4M,
		.oobsize   = 1024,
		.options   = 0,
		.read_retry_type = NAND_RR_TOSHIBA_V2012,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},

	/******************************* Samsung ******************************/
	{       /* MLC 8bit/512B */
		.name     = "K9LB(HC/PD/MD)G08U0(1)D",
		.id       = {0xEC, 0xD7, 0xD5, 0x29, 0x38, 0x41, 0x00, 0x00},
		.length   = 6,
		.chipsize = _4G,
		.probe    = samsung_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 24bit/1KB */
		.name      = "K9GAG08U0E",
		.id        = {0xEC, 0xD5, 0x84, 0x72, 0x50, 0x42, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _2G,
		.probe     = samsung_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 24bit/1KB */
		.name     = "K9LBG08U0E",
		.id       = {0xEC, 0xD7, 0xC5, 0x72, 0x54, 0x42, 0x00, 0x00},
		.length   = 6,
		.chipsize = _4G,
		.probe    = samsung_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 24bit/1KB */
		.name     = "K9G8G08U0C",
		.id       = {0xEC, 0xD3, 0x84, 0x72, 0x50, 0x42, 0x00, 0x00},
		.length   = 6,
		.chipsize = _1G,
		.probe    = samsung_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k */
		.name      = "K9GAG08U0F",
		.id        = {0xEC, 0xD5, 0x94, 0x76, 0x54, 0x43, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 512,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{        /* MLC */
		.name      = "K9LBG08U0M",
		.id        = {0xEC, 0xD7, 0x55, 0xB6, 0x78, 0x00, 0x00, 0x00},
		.length    = 5,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _512K,
		.oobsize   = 128,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_LAST_PAGE,
		.flags = 0,
	},
	{        /* MLC 24bit/1k */
		.name      = "K9GBG08U0A 20nm",
		.id        = {0xEC, 0xD7, 0x94, 0x7A, 0x54, 0x43, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_SAMSUNG,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{        /* MLC 40bit/1k */
		.name      = "K9GBG08U0B",
		.id        = {0xEC, 0xD7, 0x94, 0x7E, 0x64, 0x44, 0x00, 0x00},
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _1M,
		.oobsize   = 1024,
		.options   = 0,
		.read_retry_type = NAND_RR_SAMSUNG,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},

	/*********************************** Hynix ****************************/
	{       /* MLC */
		.name     = "H27UAG8T2A",
		.id       = {0xAD, 0xD5, 0x94, 0x25, 0x44, 0x41, },
		.length   = 6,
		.chipsize = _2G,
		.probe    = hynix_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC */
		.name     = "H27UAG8T2B",
		.id       = {0xAD, 0xD5, 0x94, 0x9A, 0x74, 0x42, },
		.length   = 6,
		.chipsize = _2G,
		.probe    = hynix_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC */
		.name     = "H27UBG8T2A",
		.id       = {0xAD, 0xD7, 0x94, 0x9A, 0x74, 0x42, },
		.length   = 6,
		.chipsize = _4G,
		.probe    = hynix_probe_v02,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 24bit/1K, 26nm TODO: Need read retry, chip is EOS */
		.name      = "H27UBG8T2BTR 26nm",
		.id        = {0xAD, 0xD7, 0x94, 0xDA, 0x74, 0xC3, },
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_HYNIX_BG_BDIE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{        /* MLC 40bit/1k */
		.name      = "H27UCG8T2A",
		.id        = {0xAD, 0xDE, 0x94, 0xDA, 0x74, 0xC4, },
		.length    = 6,
		.chipsize  = _8G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_HYNIX_CG_ADIE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},
	{        /* MLC 40bit/1k */
		.name      = "H27UBG8T2C",
		.id        = {0xAD, 0xD7, 0x94, 0x91, 0x60, 0x44, },
		.length    = 6,
		.chipsize  = _4G,
		.probe     = NULL,
		.pagesize  = _8K,
		.erasesize = _2M,
		.oobsize   = 640,
		.options   = 0,
		.read_retry_type = NAND_RR_HYNIX_BG_CDIE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = NAND_RANDOMIZER,
	},

	/********************** MISC ******************************************/
	{        /* MLC 8bit/512 */
		.name      = "P1UAGA30AT-GCA",
		.id        = {0xC8, 0xD5, 0x14, 0x29, 0x34, 0x01, },
		.length    = 6,
		.chipsize  = _2G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _512K,
		.oobsize   = 218,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{       /* MLC 4bit/512 */
		/*
		 * PowerFlash ASU8GA30IT-G30CA ID and MIRA PSU8GA30AT-GIA ID are
		 * the same ID
		 */
		.name      = "PSU8GA30AT-GIA/ASU8GA30IT-G30CA",
		.id        = {0xC8, 0xD3, 0x90, 0x19, 0x34, 0x01, },
		.length    = 6,
		.chipsize  = _1G,
		.probe     = NULL,
		.pagesize  = _4K,
		.erasesize = _256K,
		.oobsize   = 218,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{        /* SLC 1bit/512 */
		.name      = "PSU2GA30AT",
		.id        = {0x7F, 0x7F, 0x7F, 0x7F, 0xC8, 0xDA, 0x00, 0x15, },
		.length    = 8,
		.chipsize  = _256M,
		.probe     = NULL,
		.pagesize  = _2K,
		.erasesize = _128K,
		.oobsize   = 64,
		.options   = 0,
		.read_retry_type = NAND_RR_NONE,
		.badblock_pos    = BBP_FIRST_PAGE | BBP_LAST_PAGE,
		.flags = 0,
	},
	{NULL,{0},0},
};
/*****************************************************************************/

struct nand_chip_info_t *find_spl_nand_chip(char id[8])
{
	struct nand_chip_info_t *info = nand_chip_info;

	while (info->name && memcmp(id, info->id, info->length))
		info++;

	if (!info->name)
		return NULL;

	return info->probe ? info->probe(info, (unsigned char *)id) : info;
}
