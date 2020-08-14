/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/

#include <match_table.h>
#include <asm/sizes.h>

#include "hisfc400_gen.h"

/*****************************************************************************/

static struct match_reg_type page_type2reg[] = {
	{
		hisfc400_pagesize_2K, NAND_PAGE_2K,
	}, {
		hisfc400_pagesize_4K, NAND_PAGE_4K,
	}
};

enum hisfc400_page_reg hisfc400_page_type2reg(int type)
{
	return type2reg(page_type2reg, ARRAY_SIZE(page_type2reg), type, 0);
}

int hisfc400_page_reg2type(enum hisfc400_page_reg reg)
{
	return reg2type(page_type2reg, ARRAY_SIZE(page_type2reg), reg, 0);
}
/*****************************************************************************/

static struct match_reg_type ecc_type2reg[] = {
	{
		hisfc400_ecc_none, NAND_ECC_NONE,
	}, {
		hisfc400_ecc_8bit, NAND_ECC_8BIT,
	}, {
		hisfc400_ecc_16bit, NAND_ECC_16BIT,
	}, {
		hisfc400_ecc_24bit, NAND_ECC_24BIT,
	}
};

enum hisfc400_ecc_reg hisfc400_ecc_type2reg(int type)
{
	return type2reg(ecc_type2reg, ARRAY_SIZE(ecc_type2reg), type, 0);
}
/*****************************************************************************/

int hisfc400_ecc_reg2type(enum hisfc400_ecc_reg reg)
{
	return reg2type(ecc_type2reg, ARRAY_SIZE(ecc_type2reg), reg, 0);
}
/*****************************************************************************/

void hisfc400_set_ecctype(struct hisfc_host *host)
{
	int regval = type2reg(ecc_type2reg, ARRAY_SIZE(ecc_type2reg),
		host->ecctype, hisfc400_ecc_8bit);

	host->NFC_CFG &= ~(HISFC400_CFG_ECCTYPE_MASK 
			   << HISFC400_CFG_ECCTYPE_SHIFT);

	host->NFC_CFG |= (regval & HISFC400_CFG_ECCTYPE_MASK)
			  << HISFC400_CFG_ECCTYPE_SHIFT;
}
/*****************************************************************************/

void hisfc400_set_pagesize(struct hisfc_host *host)
{
	int pagetype = nandpage_size2type(host->pagesize);
	int mask = ~HISFC400_CFG_PAGEISZE_MASK;
	int regval = type2reg(page_type2reg,
		ARRAY_SIZE(page_type2reg), pagetype, hisfc400_pagesize_2K);

	regval = (regval & HISFC400_CFG_PAGEISZE_MASK)
		  << HISFC400_CFG_PAGEISZE_SHIFT;

	host->NFC_CFG &=  mask;
	host->NFC_CFG |=  regval;
}
