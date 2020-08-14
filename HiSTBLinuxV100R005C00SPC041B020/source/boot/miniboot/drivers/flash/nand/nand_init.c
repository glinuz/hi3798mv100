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

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <bits.h>
#include <malloc.h>
#include <module.h>
#include <mmu.h>

#define MOULE_NAME                     "NAND: "
#include <debug.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "spi_nand_chip.h"
#include "nand_drv.h"
#include "../hifmc100/hiflash.h"

static struct nand_dev_t *nand_dev = NULL;

nand_driver_init_fn nand_driver_init     = NULL;
nand_driver_init_fn spi_nand_driver_init = NULL;
nand_driver_init_fn xnand_driver_init    = NULL;

extern unsigned int cache_enabled;

/*****************************************************************************/

static void show_nand_info(struct nand_dev_t *nand_dev)
{
	struct nand_chip_info_t *nand_info = nand_dev->nand_info;
	struct spi_nand_chip_info_t *spi_nand_info = nand_dev->spi_nand_info;

	printf("Nand: %s \"%s\"", 
		spi_nand_info
		? get_nand_vendor(spi_nand_info->id[0]) 
		: get_nand_vendor(nand_info->id[0]),
		spi_nand_info
		? spi_nand_info->name 
		: nand_info->name);

	if (nand_info && IS_NAND_RANDOM(nand_dev))
		printf("Randomizer ");

	if (nand_info && (nand_info->read_retry_type != NAND_RR_NONE))
		printf("Read-Retry ");

	if (nand_info && IS_NANDC_SYNC_BOOT(nand_dev))
		printf("Synchronous ");

	printf("\n");

	printf("Nand(%s): ", nand_dev->bootmsg);
	printf("Block:%sB ", u32tohstr(nand_dev->blocksize, NULL));
	printf("Page:%sB ",  u32tohstr(nand_dev->pagesize, NULL));
	printf("OOB:%sB ", spi_nand_info
			   ? u32tohstr(spi_nand_info->oobsize, NULL) 
			   : u32tohstr(nand_info->oobsize, NULL));
	printf("ECC:%s ", nand_ecc_name(nand_dev->ecctype));
	printf("Chip:%sB*%d\n", u64tohstr(nand_dev->chipsize, NULL),
	       nand_dev->nr_chip);
}
/*****************************************************************************/

static int nand_bbt_init(struct nand_dev_t *nand_dev)
{
	int nr_blocks;

	nr_blocks = 1 << (nand_dev->chipshift - nand_dev->blockshift);
	nand_dev->sz_bbt = nr_blocks * nand_dev->nr_chip;

	nand_dev->bbt = (uint8 *)malloc(nand_dev->sz_bbt);
	if (!nand_dev->bbt) {
		pr_error("init bbt fail: can't malloc memory.\n");
		return -1;
	}

	memset(nand_dev->bbt, NAND_BBT_INVALID, nand_dev->sz_bbt);

	return 0;
}
/*****************************************************************************/
#ifdef CONFIG_GENERIC_NAND
struct nand_dev_t *nand_get_dev(void)
{
	char id[8];
	char tmp[8];
	int nr_chip;
	struct nand_chip_info_t *nand_info;

	if (!nand_driver_init)
		return NULL;

	nand_dev = nand_driver_init();
	if (!nand_dev)
		return NULL;

	nand_dev->reset(nand_dev, 0);
	nand_dev->read_id(nand_dev, 0, id, sizeof(id));
	nand_dev->read_id(nand_dev, 0, tmp, sizeof(tmp));

	if (memcmp(id, tmp, sizeof(id))) {
		pr_warn("nand does not stabilization. id change when second read id.\n");
		return NULL;
	}
	// TODO: 0xFF FF FF FF	... 和 0x00 00 00 00 应该被过滤掉.
	printf("\nNand ID: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
	       id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);

	nand_info = find_spl_nand_chip(id);
	if (!nand_info) {
		nand_info = find_nand_chip(id);
		if (nand_info) {
			nand_info->length = min(sizeof(tmp),
				sizeof(nand_info->id));
			memcpy(nand_info->id, id, nand_info->length);
		}
	}

	if (!nand_info) {
		pr_warn("It does not recognise this NAND chip.\n");
		return NULL;
	}
	nand_dev->nand_info = nand_info;

	nand_dev->flags = nand_info->flags;
	nand_dev->chipsize = nand_info->chipsize;
	nand_dev->oobsize = nand_info->oobsize;
	nand_dev->pagesize = nand_info->pagesize;
	nand_dev->blocksize = nand_info->erasesize;

	nand_dev->adjust_param(nand_dev);

	if (nand_dev->pagesize > NAND_MAX_PAGESIZE ||
	    nand_dev->oobsize > NAND_MAX_OOBSIZE) {
		pr_error("Nand pagesize or oobsize to larger, "
			 "Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
		return NULL;
	}

	nand_dev->pageshift = ffs(nand_dev->pagesize) - 1;
	nand_dev->blockshift = ffs(nand_dev->blocksize) - 1;
	nand_dev->chipshift = ffs64(nand_dev->chipsize) - 1;

	nand_dev->pagemask = (1 << nand_dev->pageshift) - 1;
	nand_dev->blockmask = (1 << nand_dev->blockshift) - 1;
	nand_dev->chipmask = (1 << nand_dev->chipshift) - 1;

	for (nr_chip = 1; nr_chip < CONFIG_NAND_MAX_CHIP; nr_chip++) {
		if (nand_dev->reset(nand_dev, nr_chip))
			break;
		nand_dev->read_id(nand_dev, nr_chip, tmp, sizeof(tmp));
		if (memcmp(id, tmp, sizeof(id)))
			break;
	}
	nand_dev->nr_chip = nr_chip;
	nand_dev->totalsize = nand_dev->nr_chip * nand_dev->chipsize;
	nand_dev->is_spi_nand = 0;

	if (nand_bbt_init(nand_dev))
		return NULL;

	set_nand_info(nand_dev);

	show_nand_info(nand_dev);

	return nand_dev;
}
#endif /* CONFIG_GENERIC_NAND */
/*****************************************************************************/
#ifdef CONFIG_GENERIC_SPI_NAND
struct nand_dev_t *spi_nand_get_dev(void)
{
	char id[8];
	char tmp[8];
	int nr_chip;
	struct spi_nand_chip_info_t *spi_nand_info;

	if (!spi_nand_driver_init)
		return NULL;

	nand_dev = spi_nand_driver_init();
	if (!nand_dev)
		return NULL;

	nand_dev->reset(nand_dev, 0);
	nand_dev->read_id(nand_dev, 0, id, sizeof(id));
	nand_dev->read_id(nand_dev, 0, tmp, sizeof(tmp));

	if (memcmp(id, tmp, sizeof(id))) {
		pr_warn("nand does not stabilization. id change when second read id.\n");
		return NULL;
	}
	// TODO: 0xFF FF FF FF	... 和 0x00 00 00 00 应该被过滤掉.
	printf("\nNand ID: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
	       id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);

	spi_nand_info = find_spl_spi_nand_chip(nand_dev, id);
	if (!spi_nand_info) {
		pr_warn("It does not recognise this NAND chip.\n");
		return NULL;
	}

	nand_dev->spi_nand_info = spi_nand_info;

	nand_dev->chipsize = spi_nand_info->chipsize;
	nand_dev->oobsize = spi_nand_info->oobsize;
	nand_dev->pagesize = spi_nand_info->pagesize;
	nand_dev->blocksize = spi_nand_info->erasesize;

	nand_dev->adjust_param(nand_dev);

	if (nand_dev->pagesize > NAND_MAX_PAGESIZE ||
	    nand_dev->oobsize > NAND_MAX_OOBSIZE) {
		pr_error("Nand pagesize or oobsize to larger, "
			 "Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
		return NULL;
	}

	nand_dev->pageshift = ffs(nand_dev->pagesize) - 1;
	nand_dev->blockshift = ffs(nand_dev->blocksize) - 1;
	nand_dev->chipshift = ffs64(nand_dev->chipsize) - 1;

	nand_dev->pagemask = (1 << nand_dev->pageshift) - 1;
	nand_dev->blockmask = (1 << nand_dev->blockshift) - 1;
	nand_dev->chipmask = (1 << nand_dev->chipshift) - 1;

	for (nr_chip = 1; nr_chip < CONFIG_NAND_MAX_CHIP; nr_chip++) {
		if (nand_dev->reset(nand_dev, nr_chip))
			break;
		nand_dev->read_id(nand_dev, nr_chip, tmp, sizeof(tmp));
		if (memcmp(id, tmp, sizeof(id)))
			break;
	}
	nand_dev->nr_chip = nr_chip;
	nand_dev->totalsize = nand_dev->nr_chip * nand_dev->chipsize;
	nand_dev->is_spi_nand = 1;

	if (nand_bbt_init(nand_dev))
		return NULL;

	set_nand_info(nand_dev);

	show_nand_info(nand_dev);

	return nand_dev;

}
#endif /* CONFIG_GENERIC_SPI_NAND */
/*****************************************************************************/
#if (defined CONFIG_GENERIC_HIFMC100_NAND) || (defined CONFIG_GENERIC_HIFMC100_SPI_NAND)
struct nand_dev_t *xnand_get_dev()
{
	struct nand_chip_info_t *xnand_info;
	struct nand_flash_special_dev *spl_dev;
	extern struct nand_flash_special_dev *
			find_xnand_spl_dev(struct nand_dev_t *nand_dev);

	if (!xnand_driver_init)
		return NULL;

	nand_dev = xnand_driver_init();
	if (!nand_dev)
		return NULL;	

	spl_dev = find_xnand_spl_dev(nand_dev);
	if (!spl_dev) {
		pr_warn("It does not recognise this NAND chip.\n");
		return NULL;
	}

	xnand_info = (struct nand_chip_info_t *)malloc(sizeof(struct nand_chip_info_t));
	if (!xnand_info) {
		pr_error("Can't malloc memory.\n");
		return NULL;
	}
	memset(xnand_info, 0, sizeof(struct nand_chip_info_t));

	nand_dev->nand_info = xnand_info;

	xnand_info->name = spl_dev->name;
	xnand_info->chipsize = spl_dev->chipsize;
	xnand_info->oobsize = spl_dev->oobsize;
	xnand_info->pagesize = spl_dev->pagesize;
	xnand_info->erasesize = spl_dev->erasesize;
	xnand_info->read_retry_type = spl_dev->read_retry_type;
	xnand_info->badblock_pos = spl_dev->badblock_pos;
	xnand_info->flags = spl_dev->flags;

	if (spl_dev->id_len)
		xnand_info->length = spl_dev->id_len;
	else
		xnand_info->length = sizeof(xnand_info->id);
	memcpy(xnand_info->id, spl_dev->ids.id, xnand_info->length);

	nand_dev->chipsize = xnand_info->chipsize;
	nand_dev->oobsize = xnand_info->oobsize;
	nand_dev->pagesize = xnand_info->pagesize;
	nand_dev->blocksize = xnand_info->erasesize;

	nand_dev->adjust_param(nand_dev);

	nand_dev->pageshift = ffs(nand_dev->pagesize) - 1;
	nand_dev->blockshift = ffs(nand_dev->blocksize) - 1;
	nand_dev->chipshift = ffs64(nand_dev->chipsize) - 1;

	nand_dev->pagemask = (1 << nand_dev->pageshift) - 1;
	nand_dev->blockmask = (1 << nand_dev->blockshift) - 1;
	nand_dev->chipmask = (1 << nand_dev->chipshift) - 1;
	nand_dev->totalsize = nand_dev->nr_chip * nand_dev->chipsize;

	if (nand_bbt_init(nand_dev))
		return NULL;

	set_nand_info(nand_dev);

	show_nand_info(nand_dev);

	return nand_dev;
}
#endif /* (defined CONFIG_GENERIC_HIFMC100_NAND) || (defined CONFIG_GENERIC_HIFMC100_SPI_NAND) */
/*****************************************************************************/

struct nand_dev_t *get_nand_dev(void)
{
	unsigned int cache_enabled_old = cache_enabled;

	if (nand_dev)
		return nand_dev;

	if (cache_enabled)
		mmu_cache_disable();

#ifdef CONFIG_GENERIC_NAND
	nand_dev = nand_get_dev();
	if (nand_dev)
		return nand_dev;
#endif
#ifdef CONFIG_GENERIC_SPI_NAND
	nand_dev = spi_nand_get_dev();
	if (nand_dev)
		return nand_dev;
#endif
#if (defined CONFIG_GENERIC_HIFMC100_NAND) || (defined CONFIG_GENERIC_HIFMC100_SPI_NAND)
	nand_dev = xnand_get_dev();
	  if (nand_dev)
		  return nand_dev;
#endif

	if (cache_enabled_old)
		mmu_cache_enable();

	return NULL;
}
/*****************************************************************************/

static void nand_module_init(void *ptr)
{
	get_nand_dev();
}
/*****************************************************************************/

static struct module_t module_nand_init = {
	.priority = MODULE_PRI_ENV,
	.init = nand_module_init,
};
MODULE(module_nand_init);
