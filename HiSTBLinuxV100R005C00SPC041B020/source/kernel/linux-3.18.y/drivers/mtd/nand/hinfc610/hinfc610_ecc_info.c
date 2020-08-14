/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_gen.h"

/*****************************************************************************/
/*
 * 'host->epm' only use the first oobfree[0] field, it looks very simple, But...
 */
static struct nand_ecclayout nand_ecc_default =
{
	.oobfree = {{2, 30}}
};
/*****************************************************************************/

static struct nand_ecclayout nand_ecc_2k_13bit =
{
	.oobfree = {{2, 14}}
};
/*****************************************************************************/

static struct nand_ctrl_info_t hinfc610_soft_auto_config_table[] =
{
	{SZ_32K, NAND_ECC_80BIT, 4544/*4544*/, &nand_ecc_default},
	{SZ_32K, NAND_ECC_72BIT, 4096/*4096*/, &nand_ecc_default},
	{SZ_32K, NAND_ECC_60BIT, 3520/*3520*/, &nand_ecc_default},
	{SZ_32K, NAND_ECC_48BIT, 2752/*2752*/, &nand_ecc_default},
	{SZ_32K, NAND_ECC_41BIT, 2368/*2368*/, &nand_ecc_default},
	{SZ_32K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{SZ_16K, NAND_ECC_80BIT, 1376/*1376*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_72BIT, 1376/*1376*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_60BIT, 1760/*1760*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_48BIT, 1376/*1376*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_41BIT, 1184/*1184*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{SZ_8K, NAND_ECC_80BIT, 1152/*1152*/, &nand_ecc_default},
	{SZ_8K, NAND_ECC_72BIT, 1040/*1040*/, &nand_ecc_default},
	{SZ_8K, NAND_ECC_60BIT, 896 /*896*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_48BIT, 704 /*704*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_41BIT, 608 /*608*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_32BIT, 480 /*480*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_27BIT, 416 /*416*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{SZ_4K, NAND_ECC_32BIT, 256 /*256*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_27BIT, 224 /*224*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_18BIT, 160 /*160*/,  &nand_ecc_default},

#ifndef CONFIG_SUPPORT_YAFFS
	{SZ_4K, NAND_ECC_13BIT, 112 /*112*/,  &nand_ecc_2k_13bit},
#endif
	{SZ_4K, NAND_ECC_8BIT,  88  /*88*/,   &nand_ecc_default},
	{SZ_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{SZ_2K, NAND_ECC_32BIT, 128 /*128*/, &nand_ecc_2k_13bit},
#endif
	{SZ_2K, NAND_ECC_27BIT, 128 /*128*/, &nand_ecc_default},
	{SZ_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
	{SZ_2K, NAND_ECC_18BIT, 96  /*96*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{SZ_2K, NAND_ECC_13BIT, 64  /*64*/,  &nand_ecc_2k_13bit},
#endif
	{SZ_2K, NAND_ECC_8BIT,  60  /*60*/,  &nand_ecc_default},
	{SZ_2K, NAND_ECC_NONE,  32,          &nand_ecc_default},

	{0,0,0,NULL},

};

static struct nand_ctrl_info_t hinfc620_soft_auto_config_table[] =
{
	{SZ_16K, NAND_ECC_64BIT, 1824/*1824*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_42BIT, 1216/*1216*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_40BIT, 1152/*1152*/, &nand_ecc_default},
	{SZ_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{SZ_8K, NAND_ECC_64BIT, 928 /*928*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_42BIT, 624 /*624*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_40BIT, 592 /*592*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_28BIT, 432 /*432*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{SZ_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{SZ_4K, NAND_ECC_28BIT, 232 /*232*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_16BIT, 128 /*128*/,  &nand_ecc_default},
	{SZ_4K, NAND_ECC_8BIT,  88 /*88*/,    &nand_ecc_default},
	{SZ_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{SZ_2K, NAND_ECC_28BIT, 132 /*132*/, &nand_ecc_default},
	{SZ_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
	{SZ_2K, NAND_ECC_16BIT,  64 /*64*/,  &nand_ecc_default},
	{SZ_2K, NAND_ECC_8BIT,   60 /*60*/,  &nand_ecc_default},
	{SZ_2K, NAND_ECC_NONE,   32,         &nand_ecc_default},

	{0,0,0,NULL},
};

/*****************************************************************************/
/* used the best correct arithmetic. */
struct nand_ctrl_info_t *hinfc610_get_best_ecc(struct mtd_info *mtd)
{
	struct nand_ctrl_info_t *best = NULL;
	struct nand_ctrl_info_t *config = hinfc610_soft_auto_config_table;

	for (; config->layout; config++) {
		if (config->pagesize != mtd->writesize)
			continue;

		if (mtd->oobsize < config->oobsize)
			continue;

		if (!best || (best->ecctype < config->ecctype))
			best = config;
	}

	if (!best)
		hinfc_pr_bug(ERSTR_DRIVER "Driver does not support the pagesize(%d) and oobsize(%d).\n",
			     mtd->writesize, mtd->oobsize);

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct nand_ctrl_info_t *hinfc610_force_ecc(struct mtd_info *mtd, int pagesize,
					     int ecctype, char *cfgmsg,
					     int allow_pagediv)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;
	struct nand_ctrl_info_t *fit = NULL;
	struct nand_ctrl_info_t *config = NULL;

	if (host->version == HINFC_VER_610)
		config = hinfc610_soft_auto_config_table;
	else if (host->version == HINFC_VER_620)
		config = hinfc620_soft_auto_config_table;

	for (; config->layout; config++) {
		if (config->pagesize == pagesize
			&& config->ecctype == ecctype) {
			fit = config;
			break;
		}
	}

	if (!fit) {
		hinfc_pr_bug(ERSTR_DRIVER "Driver(%s mode) does not support this Nand Flash pagesize:%d, ecctype:%s\n",
			     cfgmsg, pagesize, nand_ecc_name(ecctype));
		return NULL;
	}

	if ((pagesize != mtd->writesize)
		&& (pagesize > mtd->writesize || !allow_pagediv)) {
		hinfc_pr_bug(ERSTR_HARDWARE "Hardware (%s mode) configure pagesize %d, but the Nand Flash pageszie is %d\n",
			     cfgmsg, pagesize, mtd->writesize);
		return NULL;
	}

	if (fit->oobsize > mtd->oobsize) {
		hinfc_pr_bug(ERSTR_HARDWARE "(%s mode) The Nand Flash offer space area is %d bytes, but the controller request %d bytes in ecc %s. "
			     "Please make sure the hardware ECC configuration is correct.",
			     cfgmsg, mtd->oobsize, fit->oobsize,
			     nand_ecc_name(ecctype));
		return NULL;
	}

	return fit;
}
/*****************************************************************************/

static void hinfc610_read_bitsflaps_reg(unsigned char ecc[], int begin, int end,
					unsigned int reg)
{
	while (begin < end) {
		ecc[begin] = (reg & 0xff);
		reg = (reg >> 8);
		begin++;
	}
}
/*****************************************************************************/

static int hinfc610_max_bitsflags(unsigned char *ecc, int section)
{
	int num = 0;

	while (section-- > 0) {
		if (ecc[section] > num)
			num = ecc[section];
	}

	return num;
}
/*****************************************************************************/

static void hinfc610_read_bitsflags_32k(void *host,
					unsigned char *bitsflap,
					int *max_bitsflags)
{
	int ix, jx, kx;
	unsigned char __bitsflap[32];
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
				      hinfc_read(hinfc_host, 0xA0 + jx));
	kx = jx;
	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, kx, kx + 4,
				      hinfc_read(hinfc_host, 0xDC + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 32);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_16k(void *host,
				       unsigned char *bitsflap,
				       int *max_bitsflags)
{
	int ix, jx;
	unsigned char __bitsflap[16] = {0};
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
					    hinfc_read(hinfc_host, 0xA0 + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 16);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_8k(void *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	int ix, jx;
	unsigned char __bitsflap[8] = {0};
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	for (ix = 0, jx = 0; ix < 2; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
					    hinfc_read(hinfc_host, 0xA0 + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 8);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_4k(void *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	unsigned char __bitsflap[4] = {0};
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	hinfc610_read_bitsflaps_reg(__bitsflap, 0, 4, hinfc_read(hinfc_host, 0xA0));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 4);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_2k(void *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	unsigned char __bitsflap[2] = {0};
	struct hinfc_host *hinfc_host = (struct hinfc_host *)host;

	hinfc610_read_bitsflaps_reg(__bitsflap, 0, 2, hinfc_read(hinfc_host, 0xA0));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 2);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));
}
/*****************************************************************************/

static struct ecc_info_t hinfc610_ecc_info[] = {
	/* pagesize  ecctype   threshold section dump */
	{32768,   NAND_ECC_80BIT,  70, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_72BIT,  63, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_60BIT,  54, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_48BIT,  40, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_42BIT,  36, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_41BIT,  35, 32,   hinfc610_read_bitsflags_32k},
	{32768,   NAND_ECC_40BIT,  34, 32,   hinfc610_read_bitsflags_32k},

	{16384,   NAND_ECC_80BIT,  70, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_72BIT,  63, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_60BIT,  54, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_48BIT,  40, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_42BIT,  36, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_41BIT,  35, 16,   hinfc610_read_bitsflap_16k},
	{16384,   NAND_ECC_40BIT,  34, 16,   hinfc610_read_bitsflap_16k},

	{8192,    NAND_ECC_80BIT, 70,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_72BIT, 63,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_60BIT, 54,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_48BIT, 40,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_42BIT, 36,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_41BIT, 35,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_40BIT, 34,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_32BIT, 24,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_28BIT, 25,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_27BIT, 24,  8,   hinfc610_read_bitsflap_8k},
	{8192,    NAND_ECC_24BIT, 21,  8,   hinfc610_read_bitsflap_8k},

	{4096,    NAND_ECC_32BIT, 24,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_28BIT, 25,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_27BIT, 24,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_24BIT, 21,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_18BIT, 15,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_16BIT, 13,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_13BIT, 11,  4,   hinfc610_read_bitsflap_4k},
	{4096,    NAND_ECC_8BIT,  7,   4,   hinfc610_read_bitsflap_4k},

	{2048,    NAND_ECC_32BIT, 24, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_28BIT, 25, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_27BIT, 24, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_24BIT, 21, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_18BIT, 15, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_16BIT, 13, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_13BIT, 11, 2, hinfc610_read_bitsflap_2k},
	{2048,    NAND_ECC_8BIT,  7,  2, hinfc610_read_bitsflap_2k},
	{0, 0, 0},
};
/******************************************************************************/

int hinfc610_ecc_info_init(struct hinfc_host *host)
{
	struct ecc_info_t *info = hinfc610_ecc_info;

	for (; info->ecctype; info++)
		if (info->ecctype == host->ecctype &&
		    info->pagesize == host->pagesize)
			goto found;
	return -1;

found:
	host->ecc.threshold = info->threshold;
	host->ecc.section = info->section;
	host->ecc.dump = info->dump;

	return 0;
}
