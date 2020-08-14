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
#include <compile.h>
#include <match.h>
#include <stdio.h>
#include <asm/io.h>

#include "nand_ctrl.h"
#include "nand_chip.h"

struct spi_nand_chip_info_t *(*find_spl_spi_nand_chip)(void *nand_dev, 
							char id[8]) = NULL;

/*****************************************************************************/

static struct match_t nand_vendor[] = {
	MATCH_SET_TYPE_DATA(NAND_MFR_TOSHIBA, "Toshiba"),
	MATCH_SET_TYPE_DATA(NAND_MFR_SAMSUNG, "Samsung"),
	MATCH_SET_TYPE_DATA(NAND_MFR_FUJITSU, "Fujitsu"),
	MATCH_SET_TYPE_DATA(NAND_MFR_NATIONAL, "National"),
	MATCH_SET_TYPE_DATA(NAND_MFR_RENESAS, "Renesas"),
	MATCH_SET_TYPE_DATA(NAND_MFR_STMICRO, "ST Micro"),
	MATCH_SET_TYPE_DATA(NAND_MFR_HYNIX, "Hynix"),
	MATCH_SET_TYPE_DATA(NAND_MFR_MICRON, "Micron"),
	MATCH_SET_TYPE_DATA(NAND_MFR_AMD, "AMD"),
	MATCH_SET_TYPE_DATA(NAND_MFR_EON, "EON"),
};
/*****************************************************************************/

const char *get_nand_vendor(int id0)
{
	return match_type_to_data(nand_vendor, ARRAY_SIZE(nand_vendor), id0,
			"Unknown");
}
/*****************************************************************************/

static struct match_t match_ecc[] = {
	MATCH_SET_TYPE_DATA(NAND_ECC_NONE, "none"),
	MATCH_SET_TYPE_DATA(NAND_ECC_1BIT_512, "1bit/512" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_4BIT_512, "4bit/512" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_8BIT, "4bit/512" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_8BIT_512, "8bit/512" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_24BIT, "24bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_40BIT, "40bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_4BYTE, "4byte/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_8BYTE, "8byte/1K" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_13BIT, "13bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_16BIT, "16bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_18BIT, "18bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_27BIT, "27bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_32BIT, "32bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_41BIT, "41bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_42BIT, "42bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_48BIT, "48bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_60BIT, "60bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_72BIT, "72bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_80BIT, "80bit/1k" ),
};
static struct match_t match_pagesize[] = {
	MATCH_SET_TYPE_DATA(NAND_PAGE_2K,  "2K"),
	MATCH_SET_TYPE_DATA(NAND_PAGE_4K,  "4K"),
	MATCH_SET_TYPE_DATA(NAND_PAGE_8K,  "8K"),
	MATCH_SET_TYPE_DATA(NAND_PAGE_16K, "16K"),
	MATCH_SET_TYPE_DATA(NAND_PAGE_16K, "32K"),
};

const char *nand_ecc_name(int type)
{
	return (char *)match_type_to_data(match_ecc, ARRAY_SIZE(match_ecc),
					  type, "unknown");
}

const char *nand_page_name(int type)
{
	return (char *)match_type_to_data(match_pagesize, 
				ARRAY_SIZE(match_pagesize), type, "unknown");
}

static struct match_t page2size[] = {
	{ NAND_PAGE_512B, _512B, 0},
	{ NAND_PAGE_2K,   _2K,   0},
	{ NAND_PAGE_4K,   _4K,   0},
	{ NAND_PAGE_8K,   _8K,   0},
	{ NAND_PAGE_16K,  _16K,  0},
	{ NAND_PAGE_32K,  _32K,  0},
};
/*****************************************************************************/

int nandpage_size2type(int size)
{
	return match_reg_to_type(page2size, ARRAY_SIZE(page2size), 
				 size, NAND_PAGE_2K);
}

static struct match_t ecc2size[] = {
	{NAND_ECC_NONE,     0,   0},
	{NAND_ECC_1BIT_512, 1,   0},
	{NAND_ECC_4BIT_512, 8,   0},
	{NAND_ECC_8BIT,     8,   0},
	{NAND_ECC_8BIT_512, 16,   0},
	{NAND_ECC_13BIT,    13,   0},
	{NAND_ECC_16BIT,    16,   0},
	{NAND_ECC_18BIT,    18,   0},
	{NAND_ECC_24BIT,    24,   0},
	{NAND_ECC_27BIT,    27,   0},
	{NAND_ECC_32BIT,    32,   0},
	{NAND_ECC_40BIT,    40,   0},
	{NAND_ECC_41BIT,    41,   0},
	{NAND_ECC_42BIT,    42,   0},
	{NAND_ECC_48BIT,    48,   0},
	{NAND_ECC_60BIT,    60,   0},
	{NAND_ECC_64BIT,    64,   0},
	{NAND_ECC_72BIT,    72,   0},
	{NAND_ECC_80BIT,    80,   0},
};

int nandecc_size2type(int size)
{
	return match_reg_to_type(ecc2size, ARRAY_SIZE(ecc2size), size, 0);
}

/*****************************************************************************/
