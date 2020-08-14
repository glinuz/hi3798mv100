/******************************************************************************
*    Copyright (c) 2009-2011 Hisilicon.
*    All rights reserved.
* ***
*    Create by Czyong. 2011-12-02
*
******************************************************************************/

#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/nand.h>
#include <hinfc_gen.h>
#include <match_table.h>

/****************************************************************************/
struct nand_flash_dev *(*nand_get_spl_flash_type)(
	struct mtd_info *mtd,
	struct nand_chip *chip,
	struct nand_dev_t *nand_dev) = NULL;

/****************************************************************************/

int (*hinfc_nand_param_adjust)(struct mtd_info *mtd, struct nand_chip *chip,
	struct nand_dev_t *nand_dev) = NULL;

int (*hisfc_nand_param_adjust)(struct mtd_info *mtd, struct nand_chip *chip,
	struct nand_dev_t *nand_dev) = NULL;

/*****************************************************************************/

static struct match_type_str ecc2name[] = {
	{NAND_ECC_NONE,     "none" },
	{NAND_ECC_1BIT_512, "1bit/512" },
	{NAND_ECC_4BIT_512, "4bit/512" },
	{NAND_ECC_8BIT,     "4bit/512" },
	{NAND_ECC_8BIT_512, "8bit/512" },
	{NAND_ECC_24BIT, "24bit/1k" },
	{NAND_ECC_40BIT, "40bit/1k" },
	{NAND_ECC_4BYTE, "4byte/1k" },
	{NAND_ECC_8BYTE, "8byte/1k" },
	{NAND_ECC_13BIT, "13bit/1k" },
	{NAND_ECC_16BIT, "16bit/1k" },
	{NAND_ECC_18BIT, "18bit/1k" },
	{NAND_ECC_27BIT, "27bit/1k" },
	{NAND_ECC_32BIT, "32bit/1k" },
	{NAND_ECC_41BIT, "41bit/1k" },
	{NAND_ECC_42BIT, "42bit/1k" },
	{NAND_ECC_48BIT, "48bit/1k" },
	{NAND_ECC_60BIT, "60bit/1k" },
	{NAND_ECC_64BIT, "64bit/1k" },
	{NAND_ECC_72BIT, "72bit/1k" },
	{NAND_ECC_80BIT, "80bit/1k" },
};

static struct match_reg_type ecc2size[] = {
	{NAND_ECC_NONE,     0 },
	{NAND_ECC_1BIT_512, 1 },
	{NAND_ECC_4BIT_512, 8 },
	{NAND_ECC_8BIT,     8 },
	{NAND_ECC_8BIT_512, 16 },
	{NAND_ECC_13BIT,    13 },
	{NAND_ECC_16BIT,    16 },
	{NAND_ECC_18BIT,    18 },
	{NAND_ECC_24BIT,    24 },
	{NAND_ECC_27BIT,    27 },
	{NAND_ECC_32BIT,    32 },
	{NAND_ECC_40BIT,    40 },
	{NAND_ECC_41BIT,    41 },
	{NAND_ECC_42BIT,    42 },
	{NAND_ECC_48BIT,    48 },
	{NAND_ECC_60BIT,    60 },
	{NAND_ECC_64BIT,    64 },
	{NAND_ECC_72BIT,    72 },
	{NAND_ECC_80BIT,    80 },
};

const char *nand_ecc_name(int type)
{
	return type2str(ecc2name, ARRAY_SIZE(ecc2name), type, "unknown");
}
/*****************************************************************************/

static struct match_type_str page2name[] = {
	{ NAND_PAGE_512B, "512" },
	{ NAND_PAGE_2K,   "2K" },
	{ NAND_PAGE_4K,   "4K" },
	{ NAND_PAGE_8K,   "8K" },
	{ NAND_PAGE_16K,  "16K" },
	{ NAND_PAGE_32K,  "32K" },
};

const char *nand_page_name(int type)
{
	return type2str(page2name, ARRAY_SIZE(page2name), type, "unknown");
}
/*****************************************************************************/

static struct match_reg_type page2size[] = {
	{ _512B, NAND_PAGE_512B },
	{ _2K, NAND_PAGE_2K },
	{ _4K, NAND_PAGE_4K },
	{ _8K, NAND_PAGE_8K },
	{ _16K, NAND_PAGE_16K },
	{ _32K, NAND_PAGE_32K },
};
/*****************************************************************************/

int nandpage_size2type(int size)
{
	return reg2type(page2size, ARRAY_SIZE(page2size), size, NAND_PAGE_2K);
}
/*****************************************************************************/

int nandecc_size2type(int size)
{
	return type2reg(ecc2size, ARRAY_SIZE(ecc2size), size, 0);
}
