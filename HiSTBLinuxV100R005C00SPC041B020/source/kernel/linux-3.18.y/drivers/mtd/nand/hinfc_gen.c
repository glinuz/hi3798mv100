/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*    Create by Czyong. 2011-12-03
*
******************************************************************************/

#include <linux/hisilicon/himisc.h>
#include <linux/hikapi.h>

#include "hinfc_gen.h"

/*****************************************************************************/
struct nand_flash_dev *(*get_spi_nand_flash_type)(struct mtd_info *mtd,
		unsigned char *id, int *busw) = NULL;

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
	MATCH_SET_TYPE_DATA(NAND_ECC_8BYTE, "8byte/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_13BIT, "13bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_16BIT, "16bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_18BIT, "18bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_27BIT, "27bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_32BIT, "32bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_41BIT, "41bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_48BIT, "48bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_60BIT, "60bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_72BIT, "72bit/1k" ),
	MATCH_SET_TYPE_DATA(NAND_ECC_80BIT, "80bit/1k" ),
};

const char *nand_ecc_name(int type)
{
	return (char *)match_type_to_data(match_ecc, ARRAY_SIZE(match_ecc),
		type, "unknown");
}
/*****************************************************************************/

int get_bits(unsigned int n)
{
	int loop;
	int ret = 0;

	if (!n)
		return 0;

	if (n > 0xFFFF)
		loop = n > 0xFFFFFF ? 32 : 24;
	else
		loop = n > 0xFF ? 16 : 8;

	while (loop-- > 0 && n) {
		if (n & 1)
			ret++;
		n >>= 1;
	}
	return ret;
}

/*****************************************************************************/

int (*hinfc_param_adjust)(struct mtd_info *, struct nand_chip *, struct nand_dev_t *) = NULL;

static struct nand_dev_t __nand_dev;
/*****************************************************************************/

struct nand_flash_dev *hinfc_get_flash_type(struct mtd_info *mtd,
					    struct nand_chip *chip,
					    u8 *id_data, int *busw)
{
	struct nand_flash_dev *type;
	struct nand_dev_t *nand_dev = &__nand_dev;

	memset(nand_dev, 0, sizeof(struct nand_dev_t));
	memcpy(nand_dev->ids, id_data, 8);

#ifdef CONFIG_MTD_NAND_HISFC400
	if (!hisfc_nand_probe(mtd, chip, nand_dev))
		return NULL;
#else
	if (!hinfc_nand_probe(mtd, chip, nand_dev))
		return NULL;	
#endif

	type = &nand_dev->flash_dev;

	if (!mtd->name)
		mtd->name = type->name;

	chip->chipsize = (uint64_t)type->chipsize << 20;
	mtd->erasesize = type->erasesize;
	mtd->writesize = type->pagesize;
	mtd->oobsize   = nand_dev->oobsize;
	*busw = (type->options & NAND_BUSWIDTH_16);

	return type;
}
/*****************************************************************************/

void hinfc_nand_param_adjust(struct mtd_info *mtd, struct nand_chip *chip)
{
	struct nand_dev_t *nand_dev = &__nand_dev;

	if (!nand_dev->oobsize)
		nand_dev->oobsize = mtd->oobsize;

	if (hinfc_param_adjust)
		hinfc_param_adjust(mtd, chip, nand_dev);
}
/*****************************************************************************/

void hinfc_show_info(struct mtd_info *mtd, char *vendor, char *chipname)
{
	char buf[20];
	struct nand_dev_t *nand_dev = &__nand_dev;

	hinfc_pr_msg("Nand: %s %s ", vendor, chipname);

	if (IS_NAND_RANDOM(nand_dev))
		hinfc_pr_msg("Randomizer ");

	if (nand_dev->read_retry_type != NAND_RR_NONE)
		hinfc_pr_msg("Read-Retry ");

	hinfc_pr_msg("\n");

	if (nand_dev->start_type)
		hinfc_pr_msg("Nand(%s): ", nand_dev->start_type);
	else
		hinfc_pr_msg("Nand: ");

	hinfc_pr_msg("Block:%sB ", ultohstr(mtd->erasesize, buf, sizeof(buf)));
	hinfc_pr_msg("Page:%sB ", ultohstr(mtd->writesize, buf, sizeof(buf)));
	hinfc_pr_msg("OOB:%dB ", nand_dev->oobsize);
	hinfc_pr_msg("ECC:%s ", nand_ecc_name(nand_dev->ecctype));
}
/*****************************************************************************/

void hinfc_show_chipsize(struct nand_chip *chip)
{
	char buf[20];

	hinfc_pr_msg("Chip:%sB*%d\n",
		     ultohstr(chip->chipsize, buf, sizeof(buf)),
		     chip->numchips);
}

/*****************************************************************************/

char *nand_dbgfs_options = NULL;

static int __init dbgfs_options_setup(char *s)
{
	nand_dbgfs_options = s;
	return 1;
}
__setup("nanddbgfs=", dbgfs_options_setup);
