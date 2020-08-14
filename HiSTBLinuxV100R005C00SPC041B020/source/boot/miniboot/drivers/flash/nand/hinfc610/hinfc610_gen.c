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

#include "hinfc610_gen.h"

/*****************************************************************************/

static struct match_t hinfc610_pagesize[] = {
	MATCH_SET_TYPE_REG(_2K,  hinfc610_pagesize_2K),
	MATCH_SET_TYPE_REG(_4K,  hinfc610_pagesize_4K),
	MATCH_SET_TYPE_REG(_8K,  hinfc610_pagesize_8K),
	MATCH_SET_TYPE_REG(_16K, hinfc610_pagesize_16K),
	MATCH_SET_TYPE_REG(_32K, hinfc610_pagesize_32K),
};
/*****************************************************************************/

int hinfc610_get_pagesize(struct hinfc_host *host)
{
	uint32 regval = host->NFC_CON >> HINFC610_CON_PAGEISZE_SHIFT;
	regval &= HINFC610_CON_PAGESIZE_MASK;

	return match_reg_to_type(hinfc610_pagesize,
		ARRAY_SIZE(hinfc610_pagesize), regval, _2K);
}
/*****************************************************************************/

int hinfc610_set_pagesize(struct hinfc_host *host, int pagesize)
{
	uint32 mask = ~(HINFC610_CON_PAGESIZE_MASK << HINFC610_CON_PAGEISZE_SHIFT);
	uint32 regval = match_type_to_reg(hinfc610_pagesize,
		ARRAY_SIZE(hinfc610_pagesize), pagesize, hinfc610_pagesize_2K);

	regval = (regval & HINFC610_CON_PAGESIZE_MASK) << HINFC610_CON_PAGEISZE_SHIFT;

	host->NFC_CON &=  mask;
	host->NFC_CON |=  regval;

	return 0;
}
/*****************************************************************************/

static struct match_t hinfc610_ecc[] = {
	MATCH_SET_TYPE_REG(NAND_ECC_NONE,  hinfc610_ecc_none),
	MATCH_SET_TYPE_REG(NAND_ECC_8BIT,  hinfc610_ecc_8bit),
	MATCH_SET_TYPE_REG(NAND_ECC_13BIT, hinfc610_ecc_13bit),
	MATCH_SET_TYPE_REG(NAND_ECC_24BIT, hinfc610_ecc_24bit),
	MATCH_SET_TYPE_REG(NAND_ECC_27BIT, hinfc610_ecc_27bit),
	MATCH_SET_TYPE_REG(NAND_ECC_32BIT, hinfc610_ecc_32bit),
	MATCH_SET_TYPE_REG(NAND_ECC_41BIT, hinfc610_ecc_41bit),
	MATCH_SET_TYPE_REG(NAND_ECC_48BIT, hinfc610_ecc_48bit),
	MATCH_SET_TYPE_REG(NAND_ECC_60BIT, hinfc610_ecc_60bit),
	MATCH_SET_TYPE_REG(NAND_ECC_72BIT, hinfc610_ecc_72bit),
	MATCH_SET_TYPE_REG(NAND_ECC_80BIT, hinfc610_ecc_80bit),
};
/*****************************************************************************/

int hinfc610_get_ecctype(struct hinfc_host *host)
{
	int regval = host->NFC_CON >> HINFC610_CON_ECCTYPE_SHIFT;

	regval &= HINFC610_CON_ECCTYPE_MASK;

	return match_reg_to_type(hinfc610_ecc, ARRAY_SIZE(hinfc610_ecc),
		regval, NAND_ECC_8BIT);
}
/*****************************************************************************/

int hinfc610_set_ecctype(struct hinfc_host *host, int ecctype)
{
	int regval = match_type_to_reg(hinfc610_ecc, ARRAY_SIZE(hinfc610_ecc),
				       ecctype, hinfc610_ecc_8bit);

	regval = (regval & HINFC610_CON_ECCTYPE_MASK) << HINFC610_CON_ECCTYPE_SHIFT;

	host->NFC_CON = HINFC610_CLEAR_ECC(host->NFC_CON) | regval;

	return 0;
}