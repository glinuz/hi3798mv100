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
 * Create By Cai Zhiying 2015.7.28
 *
******************************************************************************/

#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/sizes.h>

/******************************************************************************/

struct nand_flash_dev spinand_flash_ids_hisi[] = {
	{
		.name      = "ATO25D1GA", /* 24MHz */
		.id        = {0x9B, 0x12},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "PN26G01WSIUG", /* 120MHz */
		.id        = {0xA1, 0xF1},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "FM25G01A", /* 120MHz */
		.id        = {0xA1, 0xE1},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	}, { 
		.name      = "HYF1GQ4UAACAE", /* 24MHz */
		.id        = {0xc9, 0x51},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,

	}, { 
		.name      = "HYF1GQ4UAACA6-C",
		.id        = {0xc9, 0x31},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_256K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,

	}, {
		.name      = "AFS4GQ4UAGWC4", /* 60MHz */
		.id        = {0xc2, 0xd4},
		.id_len    = 2,
		.chipsize  = SZ_512,
		.erasesize = SZ_256K,
		.pagesize  = SZ_4K,
		.oobsize   = 256,
	}, {
		.name      = "AFS2GQ4UADWC2",  /* 60MHz */
		.id        = {0xc1, 0x52},
		.id_len    = 2,
		.chipsize  = SZ_256,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	}, {
		.name      = "AFS1GQ4UACWC2", /* 60MHz */
		.id        = {0xc1, 0x51},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	}, {
		.name      = "AFS1GQ4UAAWC2", /* 60MHz */
		.id        = {0xc8, 0x31},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_256K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "F50L512M41A",
		.id        = {0xC8, 0x20, 0x7F, 0x7F, 0x7F},
		.id_len    = 5,
		.chipsize  = SZ_64,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "F50L1G41A/PSU1GS20BN", /* 104MHz */
		.id        = {0xC8, 0x21, 0x7F, 0x7F, 0x7F},
		.id_len    = 5,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "GD5F1GQ4UAYIG",
		.id        = {0xc8, 0xf1},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "MX35LF1GE4AB-Z2I",
		.id        = {0xC2, 0x12},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "GD5F1GQ4UBYIG 3.3V", /* 120MHz */
		.id        = {0xc8, 0xd1},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	}, {
		.name      = "GD5F2GQ4UAYIG",
		.id        = {0xc8, 0xf2},
		.id_len    = 2,
		.chipsize  = SZ_256,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "GD5F2GQ4UBYIG 3.3V",  /* 120MHz */
		.id        = {0xc8, 0xd2},
		.id_len    = 2,
		.chipsize  = SZ_256,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	}, {
		.name      = "5F4GQ4UAYIG",
		.id        = {0xc8, 0xf4},
		.id_len    = 2,
		.chipsize  = SZ_512,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "5F4GQ4UBYIG",
		.id        = {0xc8, 0xd4},
		.id_len    = 2,
		.chipsize  = SZ_512,
		.erasesize = SZ_256K,
		.pagesize  = SZ_4K,
		.oobsize   = 256,
	}, {
		.name      = "W25N01GV",
		.id        = {0xef, 0xaa, 0x21},
		.id_len    = 3,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 64,
	}, {
		.name      = "TC58CVG0S3HRAIF",
		.id        = {0x98, 0xC2},
		.id_len    = 2,
		.chipsize  = SZ_128,
		.erasesize = SZ_128K,
		.pagesize  = SZ_2K,
		.oobsize   = 128,
	},

	{ NULL }
};


