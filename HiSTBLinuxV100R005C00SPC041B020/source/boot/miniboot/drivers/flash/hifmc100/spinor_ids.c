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

#include <stddef.h>

#include "hiflash.h"

/******************************************************************************/

struct spinor_flash_dev spinor_flash_ids_hisi[] = {
	{"at25fs010",  {0x1f, 0x66, 0x01}, 3, _128K, _32K},
	{"at25fs040",  {0x1f, 0x66, 0x04}, 3, _512K, _64K},
	{"at25df041a", {0x1f, 0x44, 0x01}, 3, _512K, _64K},
	{"at25df641",  {0x1f, 0x48, 0x00}, 3, _8M,   _64K},
	{"at26f004",   {0x1f, 0x04, 0x00}, 3, _512K, _64K},
	{"at26df081a", {0x1f, 0x45, 0x01}, 3, _1M,   _64K},
	{"at26df161a", {0x1f, 0x46, 0x01}, 3, _2M,   _64K},
	{"at26df321",  {0x1f, 0x47, 0x01}, 3, _4M,   _64K},

	/* MXIC QE = bit6 */
	{"MX25L4005A",  {0xc2, 0x20, 0x13}, 3, _512K, _64K},
	{"MX25L8006E",  {0xc2, 0x20, 0x14}, 3, _1M,   _64K},
	{"MX25L1606E",  {0xc2, 0x20, 0x15}, 3, _2M,   _64K},
	{"MX25L3205D",  {0xc2, 0x20, 0x16}, 3, _4M,   _64K},
	{"MX25L6406E",  {0xc2, 0x20, 0x17}, 3, _8M,   _64K},
	{"MX25L128xx",  {0xc2, 0x20, 0x18}, 3, _16M,  _64K},
	{"MX25L1655D",  {0xc2, 0x26, 0x15}, 3, _2M,   _64K},
	{"MX25L12855E", {0xc2, 0x26, 0x18}, 3, _16M,  _64K},
	{"MX25L25x35x", {0xc2, 0x20, 0x19}, 3, _32M,  _64K},

	/* SPANSION QE=bit2, CMD=35h */
	{"S25SL004A",  {0x01, 0x02, 0x12}, 3, _512K, _64K},
	{"S25SL008A",  {0x01, 0x02, 0x13}, 3, _1M,   _64K},
	{"S25SL016A",  {0x01, 0x02, 0x14}, 3, _2M,   _64K},
	{"S25FL032A",  {0x01, 0x02, 0x15}, 3, _4M,   _64K},
	{"S25SL064A",  {0x01, 0x02, 0x16}, 3, _8M,   _64K},
	{"S25FL032P",  {0x01, 0x02, 0x15, 0x4d}, 4, _4M, _64K},
	{"S25FL064P",  {0x01, 0x02, 0x16, 0x4d}, 4, _8M, _64K},
	{"S25FL128P0", {0x01, 0x20, 0x18, 0x03, 0x00}, 5, _16M, _256K}, /* NO EQ 8bits I/O */
	{"S25FL128P1", {0x01, 0x20, 0x18, 0x03, 0x01}, 5, _16M, _64K }, /* NO EQ 8bits I/O */
	{"S25FL129P0", {0x01, 0x20, 0x18, 0x4d, 0x00}, 5, _16M, _256K},
	{"S25FL129P1", {0x01, 0x20, 0x18, 0x4d, 0x01}, 5, _16M, _64K},

	{"sst25wf512",  {0xbf, 0x25, 0x01}, 3, _64K,  _64K},
	{"sst25wf010",  {0xbf, 0x25, 0x02}, 3, _128K, _64K},
	{"sst25wf020",  {0xbf, 0x25, 0x03}, 3, _256K, _64K},
	{"sst25wf040",  {0xbf, 0x25, 0x04}, 3, _512K, _64K},
	{"sst25vf040b", {0xbf, 0x25, 0x8d}, 3, _512K, _64K},
	{"sst25vf080b", {0xbf, 0x25, 0x8e}, 3, _1M,   _64K},
	{"sst25vf016b", {0xbf, 0x25, 0x41}, 3, _2M,   _64K},
	{"sst25vf032b", {0xbf, 0x25, 0x4a}, 3, _4M,   _64K},

	{"M25P05",  {0x20, 0x20, 0x10}, 3, _64K,  _32K},
	{"M25P10",  {0x20, 0x20, 0x11}, 3, _128K, _32K},
	{"M25P20",  {0x20, 0x20, 0x12}, 3, _256K, _64K},
	{"M25P40",  {0x20, 0x20, 0x13}, 3, _512K, _64K},
	{"M25P80",  {0x20, 0x20, 0x14}, 3, _1M,   _64K},
	{"M25P16",  {0x20, 0x20, 0x15}, 3, _2M,   _64K},
	{"M25P32",  {0x20, 0x20, 0x16, 0x10}, 4, _4M, _64K},
	{"M25P64",  {0x20, 0x20, 0x17}, 3, _8M,   _64K},
	{"M25P128", {0x20, 0x20, 0x18}, 3, _16M,  _256K},
	{"M45PE10", {0x20, 0x40, 0x11}, 3, _128K, _64K},
	{"M45PE80", {0x20, 0x40, 0x14}, 3, _1M,   _64K},
	{"M45PE16", {0x20, 0x40, 0x15}, 3, _2M,   _64K},
	{"M25PE80", {0x20, 0x80, 0x14}, 3, _1M,   _64K},
	{"M25PE16", {0x20, 0x80, 0x15}, 3, _2M,   _64K},
	{"N25Q032", {0x20, 0xba, 0x16}, 3, _4M,   _64K},
	{"N25Q064", {0x20, 0xba, 0x17}, 3, _8M,   _64K},
	{"N25Q128", {0x20, 0xba, 0x18}, 3, _16M,  _64K},
	{"M25PX16", {0x20, 0x71, 0x15}, 3, _2M,   _64K},
	{"M25PX32", {0x20, 0x71, 0x16}, 3, _4M,   _64K},
	{"M25PX64", {0x20, 0x71, 0x17}, 3, _8M,   _64K},

	/* Winbond no QE */
	{"w25x10", {0xef, 0x30, 0x11}, 3, _128K, _64K},
	{"w25x20", {0xef, 0x30, 0x12}, 3, _256K, _64K},
	{"w25x40", {0xef, 0x30, 0x13}, 3, _512K, _64K},
	{"w25x80", {0xef, 0x30, 0x14}, 3, _1M,   _64K},
	{"w25x16", {0xef, 0x30, 0x15}, 3, _2M,   _64K},
	{"w25x32", {0xef, 0x30, 0x16}, 3, _4M,   _64K},
	{"w25x64", {0xef, 0x30, 0x17}, 3, _8M,   _64K},

	/* Winbond QE=bit2, CMD=35h, the sample as spansion */
	{"W25Q80BV",  {0xef, 0x40, 0x14}, 3, _1M,  _64K},
	{"W25Q16xV",  {0xef, 0x40, 0x15}, 3, _2M,  _64K},
	{"W25Q32BV",  {0xef, 0x40, 0x16}, 3, _4M,  _64K},
	{"W25Q64FV",  {0xef, 0x40, 0x17}, 3, _8M,  _64K},
	{"W25Q128BV", {0xef, 0x40, 0x18}, 3, _16M, _64K},
	{"W25Q256FV", {0xef, 0x40, 0x19}, 3, _32M, _64K},

	/* EoN Not support QE */
	{"EN25F80",  {0x1c, 0x31, 0x14}, 3, _1M,  _64K},
	{"EN25F16",  {0x1c, 0x31, 0x15}, 3, _2M,  _64K},
	{"EN25Q32B", {0x1c, 0x30, 0x16}, 3, _4M,  _64K},
	{"EN25Q64",  {0x1c, 0x30, 0x17}, 3, _8M,  _64K},
	{"EN25Q128", {0x1c, 0x30, 0x18}, 3, _16M, _64K},

	/* GIGA QE=bit2, CMD=35h, the sample as spansion */
	{"GD25Q128", {0xC8, 0x40, 0x18}, 3, _16M, _64K},
	{"GD25Q64B", {0xC8, 0x40, 0x17}, 3, _8M,  _64K},
	{"GD25Q32B", {0xC8, 0x40, 0x16}, 3, _4M,  _64K},

	{ NULL, },
};

