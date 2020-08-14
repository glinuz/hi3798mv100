/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-06
 *
******************************************************************************/

#include <linux/moduleparam.h>
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

/*****************************************************************************/
static void inline hinfc610_detect_ecc(unsigned char ecc[], int begin, 
				       int end, unsigned int reg)
{
	while (begin < end) {
		ecc[begin] = (reg & 0xff);
		reg = (reg >> 8);
		begin++;
	}
}
/*****************************************************************************/

static void hinfc610_ecc_32k(struct hinfc_host *host, unsigned char ecc[])
{
	int ix, jx, kx;
	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_detect_ecc(ecc, jx, jx + 4, 
				    hinfc_read(host, 0xA0 + jx));
	kx = jx;
	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_detect_ecc(ecc, kx, kx + 4, 
				    hinfc_read(host, 0xDC + jx));
}
/*****************************************************************************/

static void hinfc610_ecc_16k(struct hinfc_host *host, unsigned char ecc[])
{
	int ix, jx;
	for (ix = 0, jx = 0; ix < 4; ix ++, jx += 4)
		hinfc610_detect_ecc(ecc, jx, jx + 4, 
				    hinfc_read(host, 0xA0 + jx));
}
/*****************************************************************************/

static void hinfc610_ecc_8k(struct hinfc_host *host, unsigned char ecc[])
{
	int ix, jx;
	for (ix = 0, jx = 0; ix < 2; ix ++, jx += 4)
		hinfc610_detect_ecc(ecc, jx, jx + 4, 
				    hinfc_read(host, 0xA0 + jx));
}
/*****************************************************************************/

static void hinfc610_ecc_4k(struct hinfc_host *host, unsigned char ecc[])
{
	hinfc610_detect_ecc(ecc, 0, 4, hinfc_read(host, 0xA0));
}
/*****************************************************************************/

static void hinfc610_ecc_2k(struct hinfc_host *host, unsigned char ecc[])
{
	hinfc610_detect_ecc(ecc, 0, 2, hinfc_read(host, 0xA0));
}
/*****************************************************************************/

static struct hinfc610_ecc_inf_t hinfc610_ecc_inf[] = {

	{SZ_32K, NAND_ECC_80BIT, 32, hinfc610_ecc_32k},
	{SZ_32K, NAND_ECC_72BIT, 32, hinfc610_ecc_32k},
	{SZ_32K, NAND_ECC_60BIT, 32, hinfc610_ecc_32k},
	{SZ_32K, NAND_ECC_48BIT, 32, hinfc610_ecc_32k},
	{SZ_32K, NAND_ECC_41BIT, 32, hinfc610_ecc_32k},

	{SZ_16K, NAND_ECC_80BIT, 16, hinfc610_ecc_16k},
	{SZ_16K, NAND_ECC_72BIT, 16, hinfc610_ecc_16k},
	{SZ_16K, NAND_ECC_60BIT, 16, hinfc610_ecc_16k},
	{SZ_16K, NAND_ECC_48BIT, 16, hinfc610_ecc_16k},
	{SZ_16K, NAND_ECC_41BIT, 16, hinfc610_ecc_16k},

	{SZ_8K, NAND_ECC_80BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_72BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_60BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_48BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_41BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_32BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_27BIT, 8, hinfc610_ecc_8k},
	{SZ_8K, NAND_ECC_24BIT, 8, hinfc610_ecc_8k},

	{SZ_4K, NAND_ECC_32BIT, 4, hinfc610_ecc_4k},
	{SZ_4K, NAND_ECC_27BIT, 4, hinfc610_ecc_4k},
	{SZ_4K, NAND_ECC_24BIT, 4, hinfc610_ecc_4k},
	{SZ_4K, NAND_ECC_18BIT, 4, hinfc610_ecc_4k},
	{SZ_4K, NAND_ECC_13BIT, 4, hinfc610_ecc_4k},
	{SZ_4K, NAND_ECC_8BIT,  4, hinfc610_ecc_4k},

	{SZ_2K, NAND_ECC_32BIT, 2, hinfc610_ecc_2k},
	{SZ_2K, NAND_ECC_27BIT, 2, hinfc610_ecc_2k},
	{SZ_2K, NAND_ECC_24BIT, 2, hinfc610_ecc_2k},
	{SZ_2K, NAND_ECC_18BIT, 2, hinfc610_ecc_2k},
	{SZ_2K, NAND_ECC_13BIT, 2, hinfc610_ecc_2k},
	{SZ_2K, NAND_ECC_8BIT,  2, hinfc610_ecc_2k},
	{0, 0, 0},
};
/*****************************************************************************/

struct hinfc610_ecc_inf_t *hinfc610_get_ecc_inf(struct hinfc_host *host, 
						int pagesize, int ecctype)
{
	struct hinfc610_ecc_inf_t *inf;

	for (inf = hinfc610_ecc_inf; inf->pagesize; inf++)
		if (inf->pagesize == pagesize && inf->ecctype == ecctype)
			return inf;

	return NULL;
}
