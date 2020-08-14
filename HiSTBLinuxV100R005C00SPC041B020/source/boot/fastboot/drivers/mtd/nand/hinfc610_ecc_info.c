/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/
#include <asm/io.h>
#include <common.h>

#include "hinfc610.h"
#include "hinfc610_gen.h"
#include "hinfc610_ecc_info.h"

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

static void hinfc610_read_bitsflags_32k(struct hinfc_host *host,
					unsigned char *bitsflap,
					int *max_bitsflags)
{
	int ix, jx, kx;
	unsigned char __bitsflap[32] = {0};

	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
				      hinfc_read(host, 0xA0 + jx));
	kx = jx;
	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, kx, kx + 4,
				      hinfc_read(host, 0xDC + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 32);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));

	host->bbm_bitsflap = __bitsflap[31];
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_16k(struct hinfc_host *host,
				       unsigned char *bitsflap,
				       int *max_bitsflags)
{
	int ix, jx;
	unsigned char __bitsflap[16] = {0};

	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
					    hinfc_read(host, 0xA0 + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 16);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));

	host->bbm_bitsflap = __bitsflap[15];
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_8k(struct hinfc_host *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	int ix, jx;
	unsigned char __bitsflap[8] = {0};

	for (ix = 0, jx = 0; ix < 2; ix ++, jx += 4)
		hinfc610_read_bitsflaps_reg(__bitsflap, jx, jx + 4,
					    hinfc_read(host, 0xA0 + jx));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 8);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));

	host->bbm_bitsflap = __bitsflap[7];
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_4k(struct hinfc_host *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	unsigned char __bitsflap[4] = {0};

	hinfc610_read_bitsflaps_reg(__bitsflap, 0, 4, hinfc_read(host, 0xA0));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 4);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));

	host->bbm_bitsflap = __bitsflap[3];
}
/*****************************************************************************/

static void hinfc610_read_bitsflap_2k(struct hinfc_host *host,
				      unsigned char *bitsflap,
				      int *max_bitsflags)
{
	unsigned char __bitsflap[2] = {0};

	hinfc610_read_bitsflaps_reg(__bitsflap, 0, 2, hinfc_read(host, 0xA0));

	if (max_bitsflags)
		*max_bitsflags = hinfc610_max_bitsflags(__bitsflap, 2);
	if (bitsflap)
		memcpy(bitsflap, __bitsflap, sizeof(__bitsflap));

	host->bbm_bitsflap = __bitsflap[1];
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
