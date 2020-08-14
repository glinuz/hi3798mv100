/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/
#include <asm/sizes.h>

#include "hisfc400.h"
#include "hisfc400_gen.h"

/*****************************************************************************/
/*
 * 'host->epm' only use the first oobfree[0] field, it looks very simple, But...
 */
static struct nand_ecclayout nand_ecc_default =
{
	.oobfree = {{2, 30}}
};

#ifndef CONFIG_YAFFS2_SUPPORT
static struct nand_ecclayout nand_ecc_2k16bit =
{
	.oobfree = {{2, 6}}
};
#endif

#ifndef CONFIG_YAFFS2_SUPPORT
static struct nand_ecclayout nand_ecc_4k16bit =
{
	.oobfree = {{2, 14}}
};
#endif
/*****************************************************************************/

static struct nand_config_table hisfc400_hw_config_table[] =
{
	{NAND_PAGE_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
#ifndef CONFIG_YAFFS2_SUPPORT
	{NAND_PAGE_4K, NAND_ECC_16BIT, 128 /*128*/,  &nand_ecc_4k16bit},
#endif
	{NAND_PAGE_4K, NAND_ECC_8BIT,  88  /*88*/,   &nand_ecc_default},
	{NAND_PAGE_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{NAND_PAGE_2K, NAND_ECC_24BIT, 116 /*116*/,  &nand_ecc_default},
#ifndef CONFIG_YAFFS2_SUPPORT
	{NAND_PAGE_2K, NAND_ECC_16BIT, 64  /*64*/,   &nand_ecc_2k16bit},
#endif
	{NAND_PAGE_2K, NAND_ECC_8BIT,  60  /*60*/,   &nand_ecc_default},
	{NAND_PAGE_2K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{0,0,0,NULL},
};

/*****************************************************************************/
/* used the best correct arithmetic. */
struct nand_config_table *hisfc400_get_best_ecc(struct mtd_info *mtd)
{
	struct nand_config_table *best = NULL;
	struct nand_config_table *config = hisfc400_hw_config_table;

	for (; config->layout; config++) {
		if (nandpage_type2size(config->pagetype) != mtd->writesize)
			continue;

		if (mtd->oobsize < config->oobsize)
			continue;

		if (!best || (best->ecctype < config->ecctype))
			best = config;
	}

	if (!best)
		DBG_BUG(ERSTR_DRIVER "Driver does not support the pagesize(%d) and oobsize(%d).\n",
			     mtd->writesize, mtd->oobsize);

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct nand_config_table *hisfc400_force_ecc(struct mtd_info *mtd, int pagetype,
					     int ecctype, char *cfgmsg,
					     int allow_pagediv)
{
	int pagesize;
	struct nand_config_table *fit = NULL;
	struct nand_config_table *config = NULL; 

	config = hisfc400_hw_config_table;

	for (; config->layout; config++) {
		if (config->pagetype == pagetype
			&& config->ecctype == ecctype) {
			fit = config;
			break;
		}
	}

	pagesize = nandpage_type2size(pagetype);

	if (!fit) {
		DBG_BUG(ERSTR_DRIVER "Driver(%s mode) does not support this Nand Flash pagesize:%d, ecctype:%s\n",
			     cfgmsg, pagesize, nand_ecc_name(ecctype));
		return NULL;
	}

	if ((pagesize != mtd->writesize)
		&& (pagesize > mtd->writesize || !allow_pagediv)) {
		DBG_BUG(ERSTR_HARDWARE "Hardware (%s mode) configure pagesize %d, but the Nand Flash pageszie is %d\n",
			     cfgmsg, pagesize, mtd->writesize);
		return NULL;
	}

	if (fit->oobsize > mtd->oobsize) {
		DBG_BUG(ERSTR_HARDWARE "(%s mode) The Nand Flash offer space area is %d bytes, but the controller request %d bytes in ecc %s. "
			     "Please make sure the hardware ECC configuration is correct.",
			     cfgmsg, mtd->oobsize, fit->oobsize,
			     nand_ecc_name(ecctype));
		return NULL;
	}

	return fit;
}
