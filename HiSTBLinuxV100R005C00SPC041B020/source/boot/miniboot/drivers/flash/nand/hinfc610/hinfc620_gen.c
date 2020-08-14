/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/
#include <compile.h>
#include <stddef.h>
#include <match.h>

#include "hinfc620_gen.h"

/*****************************************************************************/

static struct match_t hinfc620_pagesize[] = {
	MATCH_SET_TYPE_REG(_2K,  hinfc620_pagesize_2K),
	MATCH_SET_TYPE_REG(_4K,  hinfc620_pagesize_4K),
	MATCH_SET_TYPE_REG(_8K,  hinfc620_pagesize_8K),
	MATCH_SET_TYPE_REG(_16K, hinfc620_pagesize_16K),
	MATCH_SET_TYPE_REG(_32K, hinfc620_pagesize_32K),
};
/*****************************************************************************/

int hinfc620_get_pagesize(struct hinfc_host *host)
{
	uint32 regval = host->NFC_CON >> HINFC610_CON_PAGEISZE_SHIFT;
	regval &= HINFC610_CON_PAGESIZE_MASK;

	return match_reg_to_type(hinfc620_pagesize,
		ARRAY_SIZE(hinfc620_pagesize), regval, _2K);
}
/*****************************************************************************/

int hinfc620_set_pagesize(struct hinfc_host *host, int pagesize)
{
	uint32 mask = ~(HINFC610_CON_PAGESIZE_MASK << HINFC610_CON_PAGEISZE_SHIFT);
	uint32 regval = match_type_to_reg(hinfc620_pagesize,
		ARRAY_SIZE(hinfc620_pagesize), pagesize, hinfc620_pagesize_2K);

	regval = (regval & HINFC610_CON_PAGESIZE_MASK) << HINFC610_CON_PAGEISZE_SHIFT;

	host->NFC_CON &=  mask;
	host->NFC_CON |=  regval;

	return 0;
}
/*****************************************************************************/

static struct match_t hinfc620_ecc[] = {
	MATCH_SET_TYPE_REG(NAND_ECC_NONE,  hinfc620_ecc_none),
	MATCH_SET_TYPE_REG(NAND_ECC_8BIT,  hinfc620_ecc_8bit),
	MATCH_SET_TYPE_REG(NAND_ECC_16BIT, hinfc620_ecc_16bit),
	MATCH_SET_TYPE_REG(NAND_ECC_24BIT, hinfc620_ecc_24bit),
	MATCH_SET_TYPE_REG(NAND_ECC_28BIT, hinfc620_ecc_28bit),
	MATCH_SET_TYPE_REG(NAND_ECC_40BIT, hinfc620_ecc_40bit),
	MATCH_SET_TYPE_REG(NAND_ECC_42BIT, hinfc620_ecc_42bit),
	MATCH_SET_TYPE_REG(NAND_ECC_64BIT, hinfc620_ecc_64bit),
};
/*****************************************************************************/

int hinfc620_get_ecctype(struct hinfc_host *host)
{
	int regval = host->NFC_CON >> HINFC610_CON_ECCTYPE_SHIFT;

	regval &= HINFC610_CON_ECCTYPE_MASK;

	return match_reg_to_type(hinfc620_ecc, ARRAY_SIZE(hinfc620_ecc),
		regval, NAND_ECC_8BIT);
}
/*****************************************************************************/

int hinfc620_set_ecctype(struct hinfc_host *host, int ecctype)
{
	int regval = match_type_to_reg(hinfc620_ecc, ARRAY_SIZE(hinfc620_ecc),
				       ecctype, hinfc620_ecc_8bit);

	regval = (regval & HINFC610_CON_ECCTYPE_MASK) << HINFC610_CON_ECCTYPE_SHIFT;

	host->NFC_CON = HINFC610_CLEAR_ECC(host->NFC_CON) | regval;

	return 0;
}
