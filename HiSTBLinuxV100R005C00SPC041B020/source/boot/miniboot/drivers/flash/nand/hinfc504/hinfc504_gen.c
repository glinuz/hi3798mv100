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
#include <match.h>

#include "hinfc504_gen.h"

/*****************************************************************************/

static struct match_t hinfc504_pagesize[] = {
	MATCH_SET_TYPE_REG(512,  hinfc504_pagesize_512),
	MATCH_SET_TYPE_REG(_2K,  hinfc504_pagesize_2K),
	MATCH_SET_TYPE_REG(_4K,  hinfc504_pagesize_4K),
	MATCH_SET_TYPE_REG(_8K,  hinfc504_pagesize_8K),
	MATCH_SET_TYPE_REG(_16K, hinfc504_pagesize_16K),
};
/*****************************************************************************/

int hinfc504_get_pagesize(struct hinfc_host *host)
{
	uint32 regval = host->NFC_CON >> HINFC504_CON_PAGEISZE_SHIFT;
	regval &= HINFC504_CON_PAGESIZE_MASK;

	return match_reg_to_type(hinfc504_pagesize,
		ARRAY_SIZE(hinfc504_pagesize), regval, _2K);
}
/*****************************************************************************/

void hinfc504_set_pagesize(struct hinfc_host *host, int pagesize)
{
	uint32 mask = ~(HINFC504_CON_PAGESIZE_MASK << HINFC504_CON_PAGEISZE_SHIFT);
	uint32 regval = match_type_to_reg(hinfc504_pagesize,
		ARRAY_SIZE(hinfc504_pagesize), pagesize, hinfc504_pagesize_2K);

	regval = (regval & HINFC504_CON_PAGESIZE_MASK) << HINFC504_CON_PAGEISZE_SHIFT;

	host->NFC_CON &=  mask;
	host->NFC_CON |=  regval;
}
/*****************************************************************************/

static struct match_t hinfc504_ecc[] = {
	MATCH_SET_TYPE_REG(NAND_ECC_NONE,  hinfc504_ecc_none),
	MATCH_SET_TYPE_REG(NAND_ECC_1BIT_512,  hinfc504_ecc_1bit),
	MATCH_SET_TYPE_REG(NAND_ECC_4BIT,  hinfc504_ecc_4bit),
	MATCH_SET_TYPE_REG(NAND_ECC_8BIT,  hinfc504_ecc_8bytes),
	MATCH_SET_TYPE_REG(NAND_ECC_24BIT, hinfc504_ecc_24bit1k),
	MATCH_SET_TYPE_REG(NAND_ECC_40BIT, hinfc504_ecc_40bit1k),
};
/*****************************************************************************/

int hinfc504_get_ecctype(struct hinfc_host *host)
{
	int regval = host->NFC_CON >> HINFC504_CON_ECCTYPE_SHIFT;

	regval &= HINFC504_CON_ECCTYPE_MASK;

	return match_reg_to_type(hinfc504_ecc, ARRAY_SIZE(hinfc504_ecc),
		regval, NAND_ECC_1BIT_512);
}
/*****************************************************************************/

void hinfc504_set_ecctype(struct hinfc_host *host, int ecctype)
{
	int regval = match_type_to_reg(hinfc504_ecc, ARRAY_SIZE(hinfc504_ecc),
				       ecctype, hinfc504_ecc_1bit);

	regval = (regval & HINFC504_CON_ECCTYPE_MASK) << HINFC504_CON_ECCTYPE_SHIFT;

	host->NFC_CON = HINFC504_CLEAR_ECC(host->NFC_CON) | regval;
}
