/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/

#include <linux/hisilicon/himisc.h>

#include "hinfc620_gen.h"

/*****************************************************************************/

static struct match_t hinfc620_pagesize[] = {
	MATCH_SET_TYPE_REG(SZ_2K,  hinfc620_pagesize_2K),
	MATCH_SET_TYPE_REG(SZ_4K,  hinfc620_pagesize_4K),
	MATCH_SET_TYPE_REG(SZ_8K,  hinfc620_pagesize_8K),
	MATCH_SET_TYPE_REG(SZ_16K, hinfc620_pagesize_16K),
	MATCH_SET_TYPE_REG(SZ_32K, hinfc620_pagesize_32K),
};

int hinfc620_get_pagesize(struct hinfc_host *host)
{
	int regval = host->NFC_CON >> HINFC610_CON_PAGEISZE_SHIFT;
	regval &= HINFC610_CON_PAGESIZE_MASK;

	return match_reg_to_type(hinfc620_pagesize,
		ARRAY_SIZE(hinfc620_pagesize), regval, SZ_2K);
}

void hinfc620_set_pagesize(struct hinfc_host *host, int pagesize)
{
	int mask = ~(HINFC610_CON_PAGESIZE_MASK << HINFC610_CON_PAGEISZE_SHIFT);
	int regval = match_type_to_reg(hinfc620_pagesize,
		ARRAY_SIZE(hinfc620_pagesize), pagesize, hinfc620_pagesize_2K);

	regval = (regval & HINFC610_CON_PAGESIZE_MASK) << HINFC610_CON_PAGEISZE_SHIFT;

	host->NFC_CON &=  mask;
	host->NFC_CON |=  regval;

	host->NFC_CON_ECC_NONE &=  mask;
	host->NFC_CON_ECC_NONE |=  regval;
}
/*****************************************************************************/

static struct match_t hinfc620_ecc[] = {
	MATCH_SET_TYPE_REG(NAND_ECC_NONE,  hinfc620_ecc_none),
	MATCH_SET_TYPE_REG(NAND_ECC_8BIT,  hinfc620_ecc_8bit),
	MATCH_SET_TYPE_REG(NAND_ECC_16BIT, hinfc620_ecc_16bit),
	MATCH_SET_TYPE_REG(NAND_ECC_24BIT, hinfc620_ecc_24bit),
	MATCH_SET_TYPE_REG(NAND_ECC_40BIT, hinfc620_ecc_40bit),
	MATCH_SET_TYPE_REG(NAND_ECC_64BIT, hinfc620_ecc_64bit),
	MATCH_SET_TYPE_REG(NAND_ECC_28BIT, hinfc620_ecc_28bit),
	MATCH_SET_TYPE_REG(NAND_ECC_42BIT, hinfc620_ecc_42bit),
};

int hinfc620_get_ecctype(struct hinfc_host *host)
{
	int regval = host->NFC_CON >> HINFC610_CON_ECCTYPE_SHIFT;

	regval &= HINFC610_CON_ECCTYPE_MASK;

	return match_reg_to_type(hinfc620_ecc, ARRAY_SIZE(hinfc620_ecc),
		regval, NAND_ECC_8BIT);
}

void hinfc620_set_ecctype(struct hinfc_host *host, int ecctype)
{
	int regval = match_type_to_reg(hinfc620_ecc, ARRAY_SIZE(hinfc620_ecc),
		ecctype, hinfc620_ecc_8bit);

	host->NFC_CON &= ~(HINFC610_CON_ECCTYPE_MASK << HINFC610_CON_ECCTYPE_SHIFT);

	host->NFC_CON |= (regval & HINFC610_CON_ECCTYPE_MASK) << HINFC610_CON_ECCTYPE_SHIFT;
}
