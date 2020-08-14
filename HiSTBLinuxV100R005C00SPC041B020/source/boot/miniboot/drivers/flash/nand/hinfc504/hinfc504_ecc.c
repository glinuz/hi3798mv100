/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>
#include <stddef.h>
#include <errno.h>
#include <malloc.h>
#include <module.h>
#include <stdio.h>
#include <string.h>
#include <bits.h>

#include <asm/io.h>
#include <debug.h>

#include "nand_drv.h"
#include "hinfc504.h"
#include "hinfc504_gen.h"

/*****************************************************************************/

static struct nand_ctrl_oob_t nand_ecc_default = {
	2, 30
};

static struct nand_ctrl_oob_t nand_ecc_2K_1bit = {
	22, 30
};

static struct nand_ctrl_info_t hinfc504_ctrl_info[] =
{
	{_16K, NAND_ECC_40BIT, 1200 /*1152*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32,            &nand_ecc_default},

	{_8K, NAND_ECC_40BIT, 600/*592*/, &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368,        &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,         &nand_ecc_default},

	{_4K, NAND_ECC_24BIT, 200,        &nand_ecc_default},
	{_4K, NAND_ECC_4BIT,  128/*104*/, &nand_ecc_default},
	{_4K, NAND_ECC_1BIT_512,  128,        &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,         &nand_ecc_default},

	{_2K, NAND_ECC_24BIT, 128/*116*/, &nand_ecc_default},
	{_2K, NAND_ECC_4BIT,  64,         &nand_ecc_default},
	{_2K, NAND_ECC_1BIT_512,  64,         &nand_ecc_2K_1bit},
	{_2K, NAND_ECC_NONE,  32,         &nand_ecc_default},

	{0,0,0,NULL},
};
/*****************************************************************************/
/* used the suitable correct arithmetic. */
struct nand_ctrl_info_t *hinfc504_detect_ecc(struct nand_chip_info_t *nand_info)
{
	struct nand_ctrl_info_t *best = NULL;
	struct nand_ctrl_info_t *ctrl_info = hinfc504_ctrl_info;

	for (; ctrl_info->layout; ctrl_info++) {
		if (ctrl_info->pagesize != nand_info->pagesize)
			continue;

		if (nand_info->oobsize < ctrl_info->oobsize)
			continue;

		if (!best || (best->ecctype < ctrl_info->ecctype))
			best = ctrl_info;
	}

	if (!best)
		DBG_BUG(ERSTR_DRIVER
			"Driver does not support the pagesize(%d) and oobsize(%d).\n",
			nand_info->pagesize, nand_info->oobsize);

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct nand_ctrl_info_t *hinfc504_force_ecc(struct nand_chip_info_t *nand_info,
					    int pagesize, int ecctype,
					    char *msg, int pagediv)
{
	struct nand_ctrl_info_t *fit = NULL;
	struct nand_ctrl_info_t *ctrl_info = hinfc504_ctrl_info;

	for (; ctrl_info->layout; ctrl_info++) {
		if (ctrl_info->pagesize == pagesize &&
		    ctrl_info->ecctype == ecctype) {
			fit = ctrl_info;
			break;
		}
	}

	if (!fit) {
		dump_hex(CONFIG_HINFC504_REG_BASE_ADDRESS,
			 (char *)CONFIG_HINFC504_REG_BASE_ADDRESS, 0xD8, sizeof(int));
		DBG_BUG(ERSTR_DRIVER
			"Driver(%s mode) does not support this Nand Flash pagesize:%d, ecctype:%s\n",
			msg, pagesize, nand_ecc_name(ecctype));
		return NULL;
	}

	if ((pagesize != nand_info->pagesize) &&
	    (pagesize > nand_info->pagesize || !pagediv)) {
		dump_hex(CONFIG_HINFC504_REG_BASE_ADDRESS,
			 (char *)CONFIG_HINFC504_REG_BASE_ADDRESS, 0xD8, sizeof(int));
		DBG_BUG(ERSTR_HARDWARE
			"Hardware (%s mode) configure pagesize %d, but the Nand Flash pageszie is %d\n",
			msg, pagesize, nand_info->pagesize);
		return NULL;
	}

	if (fit->oobsize > nand_info->oobsize) {
		DBG_BUG(ERSTR_HARDWARE
			"(%s mode) The Nand Flash offer space area is %d bytes,"
			" but the controller request %d bytes in ecc %s. "
			"Please make sure the hardware ECC configuration is correct.",
			msg, nand_info->oobsize, fit->oobsize,
			nand_ecc_name(ecctype));
		return NULL;
	}

	return fit;
}
