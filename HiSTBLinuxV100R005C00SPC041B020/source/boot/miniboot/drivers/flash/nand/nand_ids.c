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
#include <string.h>
#include <match.h>

#include "nand_chip.h"

/*****************************************************************************/

static struct nand_chip_info_t nand_chip_info[] =
{
	{
		.name = "NAND 1MiB 5V 8-bit",
		.id = {0x00, 0x6e,},
		.pagesize = 256,
		.chipsize = (1 << 20),
		.erasesize = 0x1000,
		.options = 0,
	},
	{
		.name = "NAND 2MiB 5V 8-bit",
		.id = {0x00, 0x64,},
		.pagesize = 256,
		.chipsize = (2 << 20),
		.erasesize = 0x1000,
		.options = 0,
	},
	{
		.name = "NAND 4MiB 5V 8-bit",
		.id = {0x00, 0x6b,},
		.pagesize = 512,
		.chipsize = (4 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 1MiB 3,3V 8-bit",
		.id = {0x00, 0xe8,},
		.pagesize = 256,
		.chipsize = (1 << 20),
		.erasesize = 0x1000,
		.options = 0,
	},
	{
		.name = "NAND 1MiB 3,3V 8-bit",
		.id = {0x00, 0xec,},
		.pagesize = 256,
		.chipsize = (1 << 20),
		.erasesize = 0x1000,
		.options = 0,
	},
	{
		.name = "NAND 2MiB 3,3V 8-bit",
		.id = {0x00, 0xea,},
		.pagesize = 256,
		.chipsize = (2 << 20),
		.erasesize = 0x1000,
		.options = 0,
	},
	{
		.name = "NAND 4MiB 3,3V 8-bit",
		.id = {0x00, 0xd5,},
		.pagesize = 512,
		.chipsize = (4 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 4MiB 3,3V 8-bit",
		.id = {0x00, 0xe3,},
		.pagesize = 512,
		.chipsize = (4 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 4MiB 3,3V 8-bit",
		.id = {0x00, 0xe5,},
		.pagesize = 512,
		.chipsize = (4 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 8MiB 3,3V 8-bit",
		.id = {0x00, 0xd6,},
		.pagesize = 512,
		.chipsize = (8 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 8MiB 1,8V 8-bit",
		.id = {0x00, 0x39,},
		.pagesize = 512,
		.chipsize = (8 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 8MiB 3,3V 8-bit",
		.id = {0x00, 0xe6,},
		.pagesize = 512,
		.chipsize = (8 << 20),
		.erasesize = 0x2000,
		.options = 0,
	},
	{
		.name = "NAND 16MiB 1,8V 8-bit",
		.id = {0x00, 0x33,},
		.pagesize = 512,
		.chipsize = (16 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 16MiB 3,3V 8-bit",
		.id = {0x00, 0x73,},
		.pagesize = 512,
		.chipsize = (16 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 32MiB 1,8V 8-bit",
		.id = {0x00, 0x35,},
		.pagesize = 512,
		.chipsize = (32 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 32MiB 3,3V 8-bit",
		.id = {0x00, 0x75,},
		.pagesize = 512,
		.chipsize = (32 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 64MiB 1,8V 8-bit",
		.id = {0x00, 0x36,},
		.pagesize = 512,
		.chipsize = (64 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 64MiB 3,3V 8-bit",
		.id = {0x00, 0x76,},
		.pagesize = 512,
		.chipsize = (64 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 1,8V 8-bit",
		.id = {0x00, 0x78,},
		.pagesize = 512,
		.chipsize = (128 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 1,8V 8-bit",
		.id = {0x00, 0x39,},
		.pagesize = 512,
		.chipsize = (128 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 3,3V 8-bit",
		.id = {0x00, 0x79,},
		.pagesize = 512,
		.chipsize = (128 << 20),
		.erasesize = 0x4000,
		.options = 0,
	},
	{
		.name = "NAND 64MiB 1,8V 8-bit",
		.id = {0x00, 0xA2,},
		.pagesize = 0,
		.chipsize = (64 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 64MiB 3,3V 8-bit",
		.id = {0x00, 0xF2,},
		.pagesize = 0,
		.chipsize = (64 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 1,8V 8-bit",
		.id = {0x00, 0xA1,},
		.pagesize = 0,
		.chipsize = (128 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 3,3V 8-bit",
		.id = {0x00, 0xF1,},
		.pagesize = 0,
		.chipsize = (128 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 128MiB 3,3V 8-bit",
		.id = {0x00, 0xD1,},
		.pagesize = 0,
		.chipsize = (128 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 256MiB 1,8V 8-bit",
		.id = {0x00, 0xAA,},
		.pagesize = 0,
		.chipsize = (256 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 256MiB 3,3V 8-bit",
		.id = {0x00, 0xDA,},
		.pagesize = 0,
		.chipsize = (256 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 512MiB 1,8V 8-bit",
		.id = {0x00, 0xAC,},
		.pagesize = 0,
		.chipsize = (512 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 512MiB 3,3V 8-bit",
		.id = {0x00, 0xDC,},
		.pagesize = 0,
		.chipsize = (512 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 1GiB 1,8V 8-bit",
		.id = {0x00, 0xA3,},
		.pagesize = 0,
		.chipsize = (1024 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 1GiB 3,3V 8-bit",
		.id = {0x00, 0xD3,},
		.pagesize = 0,
		.chipsize = (1024 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 2GiB 1,8V 8-bit",
		.id = {0x00, 0xA5,},
		.pagesize = 0,
		.chipsize = (2048 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{
		.name = "NAND 2GiB 3,3V 8-bit",
		.id = {0x00, 0xD5,},
		.pagesize = 0,
		.chipsize = (2048 << 20),
		.erasesize = 0,
		.options = 0,
	},
	{NULL, {0}},
};
/*****************************************************************************/

struct nand_chip_info_t *find_nand_chip(char id[8])
{
	struct nand_chip_info_t *info = nand_chip_info;

	while (info->name && info->id[1] != (unsigned char)id[1])
		info++;

	if (!info->name)
		return NULL;

	memcpy(info->id, id, 8);

	if (info->pagesize) {
		info->oobsize = info->pagesize >> 5;
	} else {
		int extid = id[3];

		info->pagesize = 1024 << (extid & 0x03);
		extid >>= 2;
		info->oobsize = (8 << (extid & 0x01)) * (info->pagesize >> 9);
		extid >>= 2;
		info->erasesize = (64 * 1024) << (extid & 0x03);
	}

	return info;
}
