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

#ifndef NAND_CHIPH
#define NAND_CHIPH
/******************************************************************************/

#define NAND_MAX_OOBSIZE      1280
#define NAND_MAX_PAGESIZE     16384

#define NAND_NCE              0x01
#define NAND_CLE              0x02
#define NAND_ALE              0x04
#define NAND_CTRL_CLE         (NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE         (NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE      0x80

#define NAND_CMD_NONE         -1
#define NAND_CMD_READ0        0
#define NAND_CMD_READ1        1
#define NAND_CMD_RNDOUT       5
#define NAND_CMD_PAGEPROG     0x10
#define NAND_CMD_READOOB      0x50
#define NAND_CMD_ERASE1       0x60
#define NAND_CMD_STATUS       0x70
#define NAND_CMD_STATUS_MULTI 0x71
#define NAND_CMD_SEQIN        0x80
#define NAND_CMD_RNDIN        0x85
#define NAND_CMD_READID       0x90
#define NAND_CMD_ERASE2       0xd0
#define NAND_CMD_PARAM        0xec
#define NAND_CMD_SYNC_RESET   0xfc
#define NAND_CMD_RESET        0xff

/* Extended commands for large page devices */
#define NAND_CMD_READSTART    0x30

#define NAND_NO_AUTOINCR      0x00000001

/* Options set by nand scan */
#define NAND_BBT_2ND_PAGE     0x10000000
#define NAND_BBT_LAST_PAGE    0x20000000

#define NAND_BBT_SCANNED      0x40000000

#define NAND_ECC_NONE         0

#define NAND_STATUS_READY     0x40
#define NAND_STATUS_FAIL      0x01

/******************************************************************************/

#define NAND_MFR_TOSHIBA     0x98
#define NAND_MFR_SAMSUNG     0xec
#define NAND_MFR_FUJITSU     0x04
#define NAND_MFR_NATIONAL    0x8f
#define NAND_MFR_RENESAS     0x07
#define NAND_MFR_STMICRO     0x20
#define NAND_MFR_HYNIX       0xad
#define NAND_MFR_MICRON      0x2c
#define NAND_MFR_AMD         0x01
#define NAND_MFR_EON         0x92
#define NAND_MFR_GIGA        0xc8
#define NAND_MFR_WINBOND     0xef

const char *get_nand_vendor(int id0);

struct nand_chip_info_t
{
	char *name;
	unsigned char id[8];
	int length;             /* length of id. */
	unsigned long long chipsize;
	struct nand_chip_info_t *(*probe)(struct nand_chip_info_t *info,
					  unsigned char id[8]);
	unsigned long pagesize;
	unsigned long erasesize;
	unsigned long oobsize;  /* chip oob size */
	unsigned long options;

#define NAND_RR_NONE                         0x00
#define NAND_RR_HYNIX_BG_BDIE                0x10
#define NAND_RR_HYNIX_BG_CDIE                0x11
#define NAND_RR_HYNIX_CG_ADIE                0x12
#define NAND_RR_MICRON                       0x20
#define NAND_RR_SAMSUNG                      0x30
#define NAND_RR_TOSHIBA_V2012                0x40
#define NAND_RR_TOSHIBA_V2013                0x41
#define NAND_RR_MASK                         0xF0
	unsigned int read_retry_type;

#define BBP_LAST_PAGE                        0x01
#define BBP_FIRST_PAGE                       0x02
	unsigned int badblock_pos;

/* (NAND) need randomizer */
#define NAND_RANDOMIZER                      0x10
/* (NAND) is ONFI interface, combine with sync/async symble */
#define NAND_IS_ONFI                         0x20
/* (NAND) support async and sync, such micron onfi, toshiba toggle 1.0 */
#define NAND_MODE_SYNC_ASYNC                 0x40
/* (NAND) support only sync, such samsung sync. */
#define NAND_MODE_ONLY_SYNC                  0x80

/* This NAND is async, or sync/async, default is async mode, onfi interface */
#define NAND_CHIP_MICRON              (NAND_MODE_SYNC_ASYNC | NAND_IS_ONFI)
/* This NAND is async, or sync/async, default is async mode, toggle1.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_10   (NAND_MODE_SYNC_ASYNC)
/* This NAND is only sync mode, toggle2.0 interface */
#define NAND_CHIP_TOSHIBA_TOGGLE_20   (NAND_MODE_ONLY_SYNC)
/* This NAND is only sync mode */
#define NAND_CHIP_SAMSUNG             (NAND_MODE_ONLY_SYNC)
	int flags;
};

struct nand_chip_info_t *find_spl_nand_chip(char id[8]);

struct nand_chip_info_t *find_nand_chip(char id[8]);
/******************************************************************************/
#endif /* NAND_CHIPH */
