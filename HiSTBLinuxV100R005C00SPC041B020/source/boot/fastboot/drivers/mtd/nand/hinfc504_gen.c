/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-08-15
 *
******************************************************************************/

#include <match_table.h>
#include <common.h>

#include "hinfc504_gen.h"

/*****************************************************************************/

static struct match_reg_type page_type2reg[] = {
	{
		hinfc504_pagesize_512, NAND_PAGE_512B,
	}, {
		hinfc504_pagesize_2K, NAND_PAGE_2K,
	}, {
		hinfc504_pagesize_4K, NAND_PAGE_4K,
	}, {
		hinfc504_pagesize_8K, NAND_PAGE_8K,
	}, {
		hinfc504_pagesize_16K, NAND_PAGE_16K,
	}
};

enum hinfc504_page_reg hinfc504_page_type2reg(int type)
{
	return type2reg(page_type2reg, ARRAY_SIZE(page_type2reg), type, 0);
}

int hinfc504_page_reg2type(enum hinfc504_page_reg reg)
{
	return reg2type(page_type2reg, ARRAY_SIZE(page_type2reg), reg, 0);
}
/*****************************************************************************/

static struct match_reg_type ecc_type2reg[] = {
	{
		hinfc504_ecc_none, NAND_ECC_NONE,
	}, {
		hinfc504_ecc_1bit, NAND_ECC_1BIT_512,
	}, {
		hinfc504_ecc_4bit, NAND_ECC_4BIT_512,
	}, {
		hinfc504_ecc_8bytes, NAND_ECC_8BIT_512,
	}, {
		hinfc504_ecc_24bit1k, NAND_ECC_24BIT,
	}, {
		hinfc504_ecc_40bit1k, NAND_ECC_40BIT,
	}
};

enum hinfc504_ecc_reg hinfc504_ecc_type2reg(int type)
{
	return type2reg(ecc_type2reg, ARRAY_SIZE(ecc_type2reg), type, 0);
}

int hinfc504_ecc_reg2type(enum hinfc504_ecc_reg reg)
{
	return reg2type(ecc_type2reg, ARRAY_SIZE(ecc_type2reg), reg, 0);
}

