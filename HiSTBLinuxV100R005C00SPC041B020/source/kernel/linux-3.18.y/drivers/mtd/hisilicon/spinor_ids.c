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

#include <linux/sizes.h>

#include "hiflash.h"

/******************************************************************************/

struct spinor_flash_dev spinor_flash_ids_hisi[] = {
	{"at25fs010",  {0x1f, 0x66, 0x01}, 3, SZ_128K, SZ_32K},
	{"at25fs040",  {0x1f, 0x66, 0x04}, 3, SZ_512K, SZ_64K},
	{"at25df041a", {0x1f, 0x44, 0x01}, 3, SZ_512K, SZ_64K},
	{"at25df641",  {0x1f, 0x48, 0x00}, 3, SZ_8M,   SZ_64K},
	{"at26f004",   {0x1f, 0x04, 0x00}, 3, SZ_512K, SZ_64K},
	{"at26df081a", {0x1f, 0x45, 0x01}, 3, SZ_1M,   SZ_64K},
	{"at26df161a", {0x1f, 0x46, 0x01}, 3, SZ_2M,   SZ_64K},
	{"at26df321",  {0x1f, 0x47, 0x01}, 3, SZ_4M,   SZ_64K},

	/* MXIC QE = bit6 */
	{"MX25L4005A",  {0xc2, 0x20, 0x13}, 3, SZ_512K, SZ_64K},
	{"MX25L8006E",  {0xc2, 0x20, 0x14}, 3, SZ_1M,   SZ_64K},
	{"MX25L1606E",  {0xc2, 0x20, 0x15}, 3, SZ_2M,   SZ_64K},
	{"MX25L3205D",  {0xc2, 0x20, 0x16}, 3, SZ_4M,   SZ_64K},
	{"MX25L6406E",  {0xc2, 0x20, 0x17}, 3, SZ_8M,   SZ_64K},
	{"MX25L128xx",  {0xc2, 0x20, 0x18}, 3, SZ_16M,  SZ_64K},
	{"MX25L1655D",  {0xc2, 0x26, 0x15}, 3, SZ_2M,   SZ_64K},
	{"MX25L12855E", {0xc2, 0x26, 0x18}, 3, SZ_16M,  SZ_64K},
	{"MX25L25x35x", {0xc2, 0x20, 0x19}, 3, SZ_32M,  SZ_64K},

	/* SPANSION QE=bit2, CMD=35h */
	{"S25SL004A",  {0x01, 0x02, 0x12}, 3, SZ_512K, SZ_64K},
	{"S25SL008A",  {0x01, 0x02, 0x13}, 3, SZ_1M,   SZ_64K},
	{"S25SL016A",  {0x01, 0x02, 0x14}, 3, SZ_2M,   SZ_64K},
	{"S25FL032A",  {0x01, 0x02, 0x15}, 3, SZ_4M,   SZ_64K},
	{"S25SL064A",  {0x01, 0x02, 0x16}, 3, SZ_8M,   SZ_64K},
	{"S25FL032P",  {0x01, 0x02, 0x15, 0x4d}, 4, SZ_4M, SZ_64K},
	{"S25FL064P",  {0x01, 0x02, 0x16, 0x4d}, 4, SZ_8M, SZ_64K},
	{"S25FL128P0", {0x01, 0x20, 0x18, 0x03, 0x00}, 5, SZ_16M, SZ_256K}, /* NO EQ 8bits I/O */
	{"S25FL128P1", {0x01, 0x20, 0x18, 0x03, 0x01}, 5, SZ_16M, SZ_64K }, /* NO EQ 8bits I/O */
	{"S25FL129P0", {0x01, 0x20, 0x18, 0x4d, 0x00}, 5, SZ_16M, SZ_256K},
	{"S25FL129P1", {0x01, 0x20, 0x18, 0x4d, 0x01}, 5, SZ_16M, SZ_64K},

	{"sst25wf512",  {0xbf, 0x25, 0x01}, 3, SZ_64K,  SZ_64K},
	{"sst25wf010",  {0xbf, 0x25, 0x02}, 3, SZ_128K, SZ_64K},
	{"sst25wf020",  {0xbf, 0x25, 0x03}, 3, SZ_256K, SZ_64K},
	{"sst25wf040",  {0xbf, 0x25, 0x04}, 3, SZ_512K, SZ_64K},
	{"sst25vf040b", {0xbf, 0x25, 0x8d}, 3, SZ_512K, SZ_64K},
	{"sst25vf080b", {0xbf, 0x25, 0x8e}, 3, SZ_1M,   SZ_64K},
	{"sst25vf016b", {0xbf, 0x25, 0x41}, 3, SZ_2M,   SZ_64K},
	{"sst25vf032b", {0xbf, 0x25, 0x4a}, 3, SZ_4M,   SZ_64K},

	{"M25P05",  {0x20, 0x20, 0x10}, 3, SZ_64K,  SZ_32K},
	{"M25P10",  {0x20, 0x20, 0x11}, 3, SZ_128K, SZ_32K},
	{"M25P20",  {0x20, 0x20, 0x12}, 3, SZ_256K, SZ_64K},
	{"M25P40",  {0x20, 0x20, 0x13}, 3, SZ_512K, SZ_64K},
	{"M25P80",  {0x20, 0x20, 0x14}, 3, SZ_1M,   SZ_64K},
	{"M25P16",  {0x20, 0x20, 0x15}, 3, SZ_2M,   SZ_64K},
	{"M25P32",  {0x20, 0x20, 0x16, 0x10}, 4, SZ_4M, SZ_64K},
	{"M25P64",  {0x20, 0x20, 0x17}, 3, SZ_8M,   SZ_64K},
	{"M25P128", {0x20, 0x20, 0x18}, 3, SZ_16M,  SZ_256K},
	{"M45PE10", {0x20, 0x40, 0x11}, 3, SZ_128K, SZ_64K},
	{"M45PE80", {0x20, 0x40, 0x14}, 3, SZ_1M,   SZ_64K},
	{"M45PE16", {0x20, 0x40, 0x15}, 3, SZ_2M,   SZ_64K},
	{"M25PE80", {0x20, 0x80, 0x14}, 3, SZ_1M,   SZ_64K},
	{"M25PE16", {0x20, 0x80, 0x15}, 3, SZ_2M,   SZ_64K},
	{"N25Q032", {0x20, 0xba, 0x16}, 3, SZ_4M,   SZ_64K},
	{"N25Q128", {0x20, 0xba, 0x18}, 3, SZ_16M,  SZ_64K},
	{"M25PX16", {0x20, 0x71, 0x15}, 3, SZ_2M,   SZ_64K},
	{"M25PX32", {0x20, 0x71, 0x16}, 3, SZ_4M,   SZ_64K},
	{"M25PX64", {0x20, 0x71, 0x17}, 3, SZ_8M,   SZ_64K},

	/* Winbond no QE */
	{"w25x10", {0xef, 0x30, 0x11}, 3, SZ_128K, SZ_64K},
	{"w25x20", {0xef, 0x30, 0x12}, 3, SZ_256K, SZ_64K},
	{"w25x40", {0xef, 0x30, 0x13}, 3, SZ_512K, SZ_64K},
	{"w25x80", {0xef, 0x30, 0x14}, 3, SZ_1M,   SZ_64K},
	{"w25x16", {0xef, 0x30, 0x15}, 3, SZ_2M,   SZ_64K},
	{"w25x32", {0xef, 0x30, 0x16}, 3, SZ_4M,   SZ_64K},
	{"w25x64", {0xef, 0x30, 0x17}, 3, SZ_8M,   SZ_64K},

	/* Winbond QE=bit2, CMD=35h, the sample as spansion */
	{"W25Q80BV",  {0xef, 0x40, 0x14}, 3, SZ_1M,  SZ_64K},
	{"W25Q16xV",  {0xef, 0x40, 0x15}, 3, SZ_2M,  SZ_64K},
	{"W25Q32BV",  {0xef, 0x40, 0x16}, 3, SZ_4M,  SZ_64K},
	{"W25Q64FV",  {0xef, 0x40, 0x17}, 3, SZ_8M,  SZ_64K},
	{"W25Q128BV", {0xef, 0x40, 0x18}, 3, SZ_16M, SZ_64K},
	{"W25Q256FV", {0xef, 0x40, 0x19}, 3, SZ_32M, SZ_64K},

	/* EoN Not support QE */
	{"EN25F80",  {0x1c, 0x31, 0x14}, 3, SZ_1M,  SZ_64K},
	{"EN25F16",  {0x1c, 0x31, 0x15}, 3, SZ_2M,  SZ_64K},
	{"EN25Q32B", {0x1c, 0x30, 0x16}, 3, SZ_4M,  SZ_64K},
	{"EN25Q64",  {0x1c, 0x30, 0x17}, 3, SZ_8M,  SZ_64K},
	{"EN25Q128", {0x1c, 0x30, 0x18}, 3, SZ_16M, SZ_64K},

	/* GIGA QE=bit2, CMD=35h, the sample as spansion */
	{"GD25Q128", {0xC8, 0x40, 0x18}, 3, SZ_16M, SZ_64K},
	{"GD25Q64B", {0xC8, 0x40, 0x17}, 3, SZ_8M,  SZ_64K},
	{"GD25Q32B", {0xC8, 0x40, 0x16}, 3, SZ_4M,  SZ_64K},

	{ NULL, },
};

