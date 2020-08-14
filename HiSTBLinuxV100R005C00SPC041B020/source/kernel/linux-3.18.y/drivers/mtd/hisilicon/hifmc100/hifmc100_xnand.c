/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.25
 *
******************************************************************************/

#define DEVNAME "hifmc100"
#define pr_fmt(fmt) "%s: " fmt

#include <linux/kernel.h>
#include <linux/clk.h>
#include <asm/io.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/hikapi.h>
#include <linux/delay.h>
#include <linux/hisilicon/freq.h>
#include <hiflash.h>

#include "hifmc100_reg.h"
#include "hifmc100_host.h"

struct page_info {
	int page_addr;

	char *pagebuf;
	dma_addr_t dma_pagebuf;
	int pagesize;

	char *oobbuf;
	dma_addr_t dma_oobbuf;
	int oobsize;

	int status;
};

#define TO_XNAND_ADDR(_xnand) \
	((u64)(_xnand)->cmdfunc->page_addr << (_xnand)->chip->page_shift)

#define EPM(_oobbuf) (*(u16 *)((u8 *)_oobbuf + 30))
#define BBM(_oobbuf) (*(u8 *)_oobbuf)

/******************************************************************************/

static void __maybe_unused nand_register_dump(struct hifmc_host *host)
{
	int ix;

	printk("Register dump:");
	for (ix = 0; ix <= 0xF0; ix += 0x04) {
		if (!(ix & 0x0F))
			printk("\n0x%08X: ", (0xf9950000 + ix));
		printk("%08X ", hifmc_read(host, ix));
	}
	printk("\n");
}
/******************************************************************************/

static int hifmc100_xnand_wait_device_ready(struct flash_regop *regop)
{
	int status;
	ulong deadline;

	deadline = jiffies + 4 * HZ;
	do {
		status = regop->read_sr(regop);
		if (status & NAND_STATUS_READY)
			return 0;

		schedule_timeout_uninterruptible(1);

	} while (!time_after_eq(jiffies, deadline));

	pr_err("wait device ready timeout.\n", DEVNAME);

	return -1;
}
/*****************************************************************************/

static u8 *hifmc100_spinand_read_raw_bbm(struct hifmc_xnand *xnand)
{
	struct hifmc_host *hifmc = xnand->host;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct flash_regop_info info = {0};

	info.priv = hifmc;
	info.cmd = SPINAND_OP_PGRD;
	info.nr_cmd = 1;
	info.addrl = cmdfunc->page_addr;
	info.addr_cycle = 3;
	xnand->regop->read_reg(&info);
	xnand->regop->wait_device_ready(xnand->regop);

	info.cmd = SPINAND_OP_READ;
	info.nr_cmd = 1;
	info.sz_buf = 2;
	info.addrl = xnand->pagesize;
	info.addr_cycle = 2;
	info.dummy = 1;
	xnand->regop->read_reg(&info);

	return (u8 *)hifmc->iobase;
}
/*****************************************************************************/

static u8 *hifmc100_nand_read_raw_bbm(struct hifmc_xnand *xnand)
{
	struct hifmc_host *hifmc = xnand->host;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct flash_regop_info info = {0};

	info.priv = hifmc;
	info.cmd = (NAND_CMD_READSTART << 8) | NAND_CMD_READ0;
	info.nr_cmd = 2;

	info.addrl = (cmdfunc->page_addr << 16) + xnand->pagesize;
	info.addrh = cmdfunc->page_addr >> 16;
	info.addr_cycle = xnand->read_addr_cycle;

	info.sz_buf = 2;
	info.wait_ready = true;
	xnand->regop->read_reg(&info);

	return (u8 *)hifmc->iobase;
}
/*****************************************************************************/

static int hifmc100_spinand_read_sr(struct flash_regop *drv)
{
	u8 retsr = 0;
	u8 status = 0;
	struct flash_regop_info info = {0};
	struct hifmc_xnand *xnand = container_of(drv, struct hifmc_xnand, regop[0]);
	struct nand_chip *chip = xnand->chip;

	info.priv = drv->priv;
	info.cmd = SPINAND_OP_RDSR;
	info.nr_cmd = 1;
	info.addr_cycle = 1;
	info.buf = &status;
	info.sz_buf = 1;

	info.addrl = 0xA0;
	drv->read_reg(&info);

	if (!(status & 0x38))
		retsr = NAND_STATUS_WP;

	info.addrl = 0xC0;
	drv->read_reg(&info);

	if (!(status & SPINAND_STATUS_OIP))
		retsr |= NAND_STATUS_READY;

	if (((chip->state == FL_WRITING) && (status & SPINAND_STATUS_P_FAIL))
	    || ((chip->state == FL_ERASING) && (status & SPINAND_STATUS_E_FAIL))) {
		retsr |= NAND_STATUS_FAIL;
	}

	return retsr;
}
/*****************************************************************************/

static int hifmc100_nand_read_sr(struct flash_regop *drv)
{
	u8 status = 0;
	struct flash_regop_info info = {0};

	info.priv = drv->priv;
	info.cmd = NAND_CMD_STATUS;
	info.nr_cmd = 1;
	info.buf = &status;
	info.sz_buf = 1;
	drv->read_reg(&info);

	return status;
}
/******************************************************************************/

static int hifmc100_xnand_dev_ready(struct mtd_info *mtd)
{
	u8 status;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;

	status = xnand->regop->read_sr(xnand->regop);

	return (status & NAND_STATUS_READY ? 1 : 0);
}
/******************************************************************************/

static bool hifmc100_xnand_is_page_empty(struct hifmc_xnand *xnand,
					 u8 oob_bitsflip)
{
	u16 epm = EPM(xnand->oobbuf);

	if (!epm)
		return false;

	if (epm == 0xFFFF)
		return true;

	/* ecc failed && bitflips < 4 bits */
	if (oob_bitsflip > xnand->ecc_strength && hweight16(epm) > 12)
		return true;

	return false;
}
/******************************************************************************/

static bool hifmc100_xnand_is_bad_block(struct hifmc_xnand *xnand,
					u8 oob_bitsflip)
{
	u8 bbm = BBM(xnand->oobbuf);

	if (!bbm)
		return true;

	if (bbm == 0xFF) {
		u16 epm = EPM(xnand->oobbuf);

		if (!epm) /* we have written this page, it is a good page. */
			return false;

		/*
		 * this is a empty page, maybe only bbm is 0x00, and corrected to
		 * 0xFF. only spansion nand meet this problem.
		 * other nand bad block page or oob is all zero.
		 */

		if (oob_bitsflip >= 8 && xnand->manuf == NAND_MFR_AMD) {
			bbm = *(u8 *)xnand->read_raw_bbm(xnand);
			if (!bbm)
				return true;
		}

		return false;
	}

	/* bbm != 0xFF && ecc success. */
	if (oob_bitsflip <= xnand->ecc_strength)
		return true;

	/* bbm != 0xFF && ecc fail && bbm > 2 bits fliping */
	if (hweight8(bbm) < 6)
		return true;

	/* bbm != 0xFF && ecc fail && bits fliping <= 2 */
	return false;
}
/******************************************************************************/

static inline int hifmc100_xnand_dma_map(struct hifmc_xnand *xnand,
					 struct page_info *page, u32 dmaaddr_reg,
					 enum dma_data_direction direction)
{
	struct hifmc_host *hifmc = xnand->host;

	page->dma_pagebuf = dma_map_single(xnand->dev, page->pagebuf,
		page->pagesize, direction);
	if (dma_mapping_error(xnand->dev, page->dma_pagebuf)) {
		pr_err("dma_map_single fail.\n", DEVNAME);
		return -ENOMEM;
	}

	hifmc_write(hifmc, (u32)page->dma_pagebuf, dmaaddr_reg);

	if (xnand->pagesize >= SZ_8K) {
		u32 phyaddr = (u32)page->dma_pagebuf + SZ_4K;

		hifmc_write(hifmc, phyaddr, dmaaddr_reg + 0x4);

		if (xnand->pagesize >= SZ_16K) {
			hifmc_write(hifmc, phyaddr + SZ_4K, dmaaddr_reg + 0x8);
			hifmc_write(hifmc, phyaddr + SZ_8K, dmaaddr_reg + 0xC);
		}
	}

	page->dma_oobbuf = dma_map_single(xnand->dev, page->oobbuf,
		page->oobsize, direction);
	if (dma_mapping_error(xnand->dev, page->dma_oobbuf)) {
		pr_err("dma_map_single fail.\n", DEVNAME);

		dma_unmap_single(xnand->dev, page->dma_pagebuf,
			page->pagesize, direction);

		return -ENOMEM;
	}
	hifmc_write(hifmc, (u32)page->dma_oobbuf, dmaaddr_reg + 0x10);

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_dma_transfer(struct hifmc_xnand *xnand,
				       enum dma_data_direction direction)
{
	int ret;
	u32 regval;
	struct hifmc_host *hifmc = xnand->host;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct hifmc_reg *reg;
	struct page_info page;

	if (direction == DMA_FROM_DEVICE)
		reg = &xnand->reg.read;
	else
		reg = &xnand->reg.write;

	page.pagebuf = xnand->pagebuf;
	page.pagesize = xnand->pagesize;
	page.oobbuf = xnand->oobbuf;
	page.oobsize = xnand->oobsize;

	ret = hifmc100_xnand_dma_map(xnand, &page, HIFMC100_DMA_SADDR_D0,
		direction);
	if (ret)
		return ret;

	if (xnand->ecc_strength == 0)
		hifmc_write(hifmc, HIFMC100_DMA_LEN_MASK(xnand->oobsize),
			HIFMC100_DMA_LEN);

	regval = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	if (xnand->ecc_strength)
		regval |= xnand->reg.fmc_cfg_ecc_type;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	/* set address cycle and chip select */
	regval = reg->op_cfg;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* set randomizer */
	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	if (xnand->randomizer_en)
		regval |= HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	else
		regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set address */
	hifmc_write(hifmc, cmdfunc->page_addr << 16, HIFMC100_ADDRL);
	hifmc_write(hifmc, cmdfunc->page_addr >> 16, HIFMC100_ADDRH);

	/* set command */
	hifmc_write(hifmc, reg->cmd, HIFMC100_CMD);

	hifmc->irq_enable(hifmc);

	hifmc_write(hifmc, reg->op_ctrl, HIFMC100_OP_CTRL);

	hifmc->wait_dma_finish(hifmc);

	/* disable dma done interrupt */
	regval = hifmc_read(hifmc, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_OP_DONE;
	hifmc_write(hifmc, regval, HIFMC100_INT_EN);

	dma_unmap_single(xnand->dev, page.dma_pagebuf, page.pagesize,
		direction);
	dma_unmap_single(xnand->dev, page.dma_oobbuf, page.oobsize,
		direction);

	return 0;
}
/******************************************************************************/

static int hifmc100_set_nand_fast_timing(struct hifmc_xnand *nand)
{
#ifndef CONFIG_HI3798MV2X_FPGA
#ifndef CONFIG_HI3796MV2X_FPGA
	u32 regval;
	struct hifmc_host *hifmc = nand->host;

	hifmc_write(hifmc, nand->reg.fmc_pnd_pwidth_cfg,
		HIFMC100_PND_PWIDTH_CFG);

	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval |= HIFMC100_GLOBAL_CFG_EDO_MODE;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	hifmc_write(hifmc, HIFMC100_READ_TIMING_TUNE_VALUE,
		    HIFMC100_READ_TIMING_TUNE);
#endif
#endif
	return 0;
}
/******************************************************************************/

static int hifmc100_set_nand_slow_timing(struct hifmc_xnand *nand)
{
	u32 regval;
	struct hifmc_host *hifmc = nand->host;

	hifmc_write(hifmc, 0xFFF, HIFMC100_PND_PWIDTH_CFG);

	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_EDO_MODE;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	hifmc_write(hifmc, 0, HIFMC100_READ_TIMING_TUNE);

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_do_read_retry(struct hifmc_xnand *xnand)
{
	int retry_mode = 1;
	struct nand_read_retry_info info = {0};

	info.drv = xnand->regop;

	/*
	 * 1. spinand not support read retry now 
	 * 2. some NAND will failed when config read retry register in fast timing.
	 */
	hifmc100_set_nand_slow_timing(xnand);

	if (xnand->read_retry->entry)
		xnand->read_retry->entry(&info);

	while (retry_mode < xnand->read_retry->count) {
		u32 regval;

		xnand->read_retry->setup(&info, retry_mode);

		hifmc100_set_nand_fast_timing(xnand);
		hifmc100_xnand_dma_transfer(xnand, DMA_FROM_DEVICE);
		hifmc100_set_nand_slow_timing(xnand);

		/* ecc success */
		regval = hifmc_read(xnand->host, HIFMC100_INT);
		if (!(regval & HIFMC100_INT_ERR_INVALID))
			break;

		retry_mode++;
	}

	xnand->stats->read_retry(xnand->stats, TO_XNAND_ADDR(xnand),
		retry_mode);

	if (xnand->read_retry->exit)
		xnand->read_retry->exit(&info);

	hifmc100_set_nand_fast_timing(xnand);

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_fix_read(struct hifmc_xnand *xnand)
{
	u8 ecc[16] = {0};
	int max_bitsflip = 0;

	xnand->ctrl_info->dump(xnand, ecc, &max_bitsflip);

	if (xnand->cmdfunc->command == NAND_CMD_READOOB) {
		xnand->is_bad_block = hifmc100_xnand_is_bad_block(xnand,
			ecc[xnand->ctrl_info->nr_ecc_sect - 1]);

		BBM(xnand->oobbuf) = xnand->is_bad_block ? 0x00 : 0xFF;

		if (xnand->is_bad_block)
			return 0;
	}

	xnand->is_empty_page = hifmc100_xnand_is_page_empty(xnand,
		ecc[xnand->ctrl_info->nr_ecc_sect - 1]);
	if (xnand->is_empty_page)
		return 0;

	if (max_bitsflip > xnand->ecc_strength && xnand->read_retry) {
		hifmc100_xnand_do_read_retry(xnand);
		xnand->ctrl_info->dump(xnand, ecc, &max_bitsflip);
	}

	xnand->stats->read(xnand->stats, TO_XNAND_ADDR(xnand),
		(u64)xnand->pagesize, ecc);

	if (max_bitsflip > xnand->ecc_strength)
		xnand->is_uc_err = true;

	return 0;
}
/******************************************************************************/

static void hifmc100_spinand_cmd_readid(struct hifmc_xnand *xnand)
{
	u8 id[FLASH_ID_LEN];
	static int cs[2] = {0};
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = SPINAND_OP_RDID;
	info.nr_cmd = 1;
	info.addr_cycle = 1;
	info.buf = id;
	info.sz_buf = sizeof(id);
	xnand->regop->read_reg(&info);

	/*
	 * only printf once each spi-nand chip, and support one spi-nand chip
	 */
	if (!cs[xnand->host->chipselect & 0x01] && id[0]) {
		cs[xnand->host->chipselect & 0x01] = 1;
		pr_info("id: 0x%02X 0x%02X 0x%02X 0x%02X\n",
			xnand->name, id[0], id[1], id[2], id[3]);

		memcpy(xnand->id, id, sizeof(id));
	}
}
/******************************************************************************/

static void hifmc100_nand_cmd_readid(struct hifmc_xnand *xnand)
{
	u8 id[FLASH_ID_LEN];
	static int cs[2] = {0};
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = NAND_CMD_READID;
	info.nr_cmd = 1;
	info.addr_cycle = 1;
	info.buf = id;
	info.sz_buf = sizeof(id);
	xnand->regop->read_reg(&info);

	/* only printk once each xnand chip  */
	if (!cs[xnand->host->chipselect & 0x01] && id[0]) {
		cs[xnand->host->chipselect & 0x01] = 1;
		pr_info("id: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
			xnand->name, id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
		memcpy(xnand->id, id, sizeof(id));
	}
}
/******************************************************************************/

static void hifmc100_nand_cmd_reset(struct hifmc_xnand *xnand)
{
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = NAND_CMD_RESET;
	info.nr_cmd = 1;
	info.wait_ready = true;
	xnand->regop->write_reg(&info);
}
/******************************************************************************/

static void hifmc100_spinand_cmd_reset(struct hifmc_xnand *xnand)
{
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = SPINAND_OP_RESET;
	info.nr_cmd = 1;
	xnand->regop->write_reg(&info);

	/* GigaDevice request after 300ns, read status register. */
	ndelay(300);

	xnand->regop->wait_device_ready(xnand->regop);

	/* before read id, there is no special spinand driver */
	if (xnand->spinand_drv)
		xnand->spinand_drv->chip_prepare(xnand->regop);
}
/******************************************************************************/

static void hifmc100_nand_cmd_erase(struct hifmc_xnand *xnand)
{
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1;
	info.nr_cmd = 2;
	info.addrl = xnand->cmdfunc->page_addr;
	info.addr_cycle = xnand->erase_addr_cycle;
	info.wait_ready = true;
	xnand->regop->write_reg(&info);
}
/******************************************************************************/

static void hifmc100_spinand_cmd_erase(struct hifmc_xnand *xnand)
{
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = SPINAND_OP_WREN;
	info.nr_cmd = 1;
	xnand->regop->write_reg(&info);

	info.cmd = SPINAND_OP_BE;
	info.nr_cmd = 1;
	info.addrl = xnand->cmdfunc->page_addr;
	info.addr_cycle = 3;
	xnand->regop->write_reg(&info);

	xnand->regop->wait_device_ready(xnand->regop);
}
/******************************************************************************/

static void __maybe_unused hifmc100_spinand_reg_read(struct hifmc_xnand *xnand)
{
	u32 regval;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct hifmc_host *hifmc = xnand->host;
	struct flash_regop_info info = {0};

	info.priv = xnand->host;
	info.cmd = SPINAND_OP_PGRD;
	info.nr_cmd = 1;
	info.addrl = cmdfunc->page_addr;
	info.addr_cycle = 3;
	xnand->regop->write_reg(&info);
	xnand->regop->wait_device_ready(xnand->regop);

	/* enable ecc and pagesize */
	regval = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	regval |= xnand->reg.fmc_cfg_ecc_type;
	regval &= ~HIFMC100_CFG_PAGE_SIZE_MASK;
	regval |= xnand->reg.fmc_cfg_page_size;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	/* disable randomizer */
	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set address */
	hifmc_write(hifmc, 0, HIFMC100_ADDRH);
	hifmc_write(hifmc, 0, HIFMC100_ADDRL);

	/* set chip select, address number, dummy */
	regval = HIFMC100_OP_CFG_ADDR_NUM(2);
	regval |= HIFMC100_OP_CFG_DUMMY_NUM(1);
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	hifmc_write(hifmc, 0, HIFMC100_OP_PARA);

	hifmc_write(hifmc, SPINAND_OP_READ, HIFMC100_CMD);

	regval = HIFMC100_OP_REG_OP_START |
		HIFMC100_OP_DUMMY_EN |
		HIFMC100_OP_CMD1_EN |
		HIFMC100_OP_READ_DATA_EN |
		HIFMC100_OP_ADDR_EN;

	hifmc_write(hifmc, regval, HIFMC100_OP);
	hifmc->wait_host_ready(hifmc);
}
/******************************************************************************/

static void hifmc100_xnand_read_cache(struct hifmc_xnand *xnand, int page_addr)
{
	int ret;
	char *pagebuf, *oobbuf;
	struct flash_cache *cache = xnand->cache;

	xnand->is_empty_page = false;
	xnand->is_bad_block = false;
	xnand->is_uc_err = false;

	ret = cache->get_status_lock_read_page(cache, page_addr);
	switch (ret) {
	default:
	case FLASHCACHE_PAGE_BADBLOCK:
	case FLASHCACHE_DISABLE:
		hifmc100_xnand_dma_transfer(xnand, DMA_FROM_DEVICE);
		hifmc100_xnand_fix_read(xnand);
		break;

	case FLASHCACHE_PAGE_CACHE:
		cache->get_cache(cache, page_addr, &pagebuf, &oobbuf);
		xnand->pagebuf = pagebuf;
		memcpy(xnand->oobbuf, oobbuf, cache->oobsize);
		break;

	case FLASHCACHE_PAGE_EMPTY:
		xnand->is_empty_page = true;
		break;

	case FLASHCACHE_PAGE_UNKNOWN:
		ret = cache->new_cache(cache, page_addr, &pagebuf, &oobbuf);
		if (!ret)
			xnand->pagebuf = pagebuf;

		hifmc100_xnand_dma_transfer(xnand, DMA_FROM_DEVICE);
		hifmc100_xnand_fix_read(xnand);

		if (ret)
			break;

		if (xnand->is_empty_page) {
			cache->discard_cache(cache, page_addr, 1,
				FLASHCACHE_PAGE_EMPTY);
			break;
		}

		if (xnand->is_bad_block) {
			cache->discard_cache(cache, page_addr, 1,
				FLASHCACHE_PAGE_BADBLOCK);
			break;
		}

		if (xnand->is_uc_err) {
			cache->discard_cache(cache, page_addr, 1,
				FLASHCACHE_PAGE_UNKNOWN);
			break;
		}

		memcpy(oobbuf, xnand->oobbuf, cache->oobsize);

		break;
	}
}
/******************************************************************************/

static void hifmc100_xnand_cmdfunc(struct mtd_info *mtd, unsigned int command,
				   int column, int page_addr)
{
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;

	cmdfunc->command = command;

	switch (command) {
	case NAND_CMD_READOOB:
		column += xnand->pagesize;

	case NAND_CMD_READ0:
		cmdfunc->column = column;
		cmdfunc->page_addr = page_addr;
		cmdfunc->offset = 0;

		hifmc100_xnand_read_cache(xnand, cmdfunc->page_addr);

		if (xnand->is_uc_err)
			mtd->ecc_stats.failed++;

		break;

	case NAND_CMD_ERASE1:
		cmdfunc->page_addr = page_addr;
		break;

	case NAND_CMD_ERASE2:
		xnand->cmd_erase(xnand);

		xnand->stats->erase(xnand->stats, TO_XNAND_ADDR(xnand),
			(u64)xnand->blocksize);

		xnand->cache->discard_cache(xnand->cache, cmdfunc->page_addr,
			xnand->page_per_block, FLASHCACHE_PAGE_EMPTY);

		break;

	case NAND_CMD_SEQIN:
		cmdfunc->column = column;
		cmdfunc->offset = 0;
		cmdfunc->page_addr = page_addr;

		break;

	case NAND_CMD_PAGEPROG:
		EPM(xnand->oobbuf) = 0;

		hifmc100_xnand_dma_transfer(xnand, DMA_TO_DEVICE);

		xnand->cache->discard_cache(xnand->cache, cmdfunc->page_addr,
			1, FLASHCACHE_PAGE_UNKNOWN);

		xnand->stats->write(xnand->stats, TO_XNAND_ADDR(xnand),
			(u64)xnand->pagesize);

		break;

	case NAND_CMD_STATUS:
		/* deal in read_byte */
		break;

	case NAND_CMD_READID:
		xnand->cmd_readid(xnand);
		cmdfunc->offset = 0;
		break;

	case NAND_CMD_RESET:
		xnand->cmd_reset(xnand);
		break;

	case NAND_CMD_RNDIN:
	case NAND_CMD_PARAM:
	case NAND_CMD_RNDOUT:
	default:
		cmdfunc->offset = 0;
		pr_err("not support command 0x%08x:\n", xnand->name, command);
		break;
	}
}
/*****************************************************************************/

static void hifmc100_xnand_select_chip(struct mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	xnand->host->chipselect = chipselect;
}
/*****************************************************************************/

static uint8_t hifmc100_xnand_read_byte(struct mtd_info *mtd)
{
	u8 retval;
	int offset;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;

	if (cmdfunc->command == NAND_CMD_STATUS)
		return xnand->regop->read_sr(xnand->regop);

	cmdfunc->offset++;

	if (cmdfunc->command == NAND_CMD_READID)
		return readb(xnand->host->iobase + cmdfunc->offset - 1);

	if (xnand->is_empty_page) {
		retval = 0xFF;
		goto exit;
	}

	offset = cmdfunc->column + cmdfunc->offset - 1;

	if (offset < mtd->writesize)
		retval = readb(xnand->pagebuf + offset);
	else
		retval = readb(xnand->oobbuf + offset - mtd->writesize);

exit:
	if (cmdfunc->command == NAND_CMD_READOOB) {
		xnand->cache->unlock_read_page(xnand->cache, xnand->cmdfunc->page_addr);
		xnand->pagebuf = xnand->org_pagebuf;
	}

	return retval;
}
/*****************************************************************************/

static void hifmc100_xnand_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int offset;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;

	if (xnand->is_empty_page) {
		memset(buf, 0xFF, len);
		goto exit;
	}

	offset = cmdfunc->column + cmdfunc->offset;

	if (offset >= xnand->pagesize)
		memcpy(buf, xnand->oobbuf + offset - xnand->pagesize, len);
	else if (offset + len <= xnand->pagesize)
		memcpy(buf, xnand->pagebuf + offset, len);
	else {
		int nr_copy = xnand->pagesize - offset;
		memcpy(buf, xnand->pagebuf + offset, nr_copy);
		memcpy(buf + nr_copy, xnand->oobbuf, len - nr_copy);
	}
exit:
	cmdfunc->offset += len;

	if (cmdfunc->command == NAND_CMD_READOOB) {
		xnand->cache->unlock_read_page(xnand->cache, xnand->cmdfunc->page_addr);
		xnand->pagebuf = xnand->org_pagebuf;
	}
}
/******************************************************************************/

static int hifmc100_xnand_read_page(struct mtd_info *mtd, struct nand_chip *chip,
				    uint8_t *buf, int oob_required, int page)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;

	if (xnand->is_empty_page) {
		memset(buf, 0xFF, xnand->pagesize);
		if (oob_required)
			memset(chip->oob_poi, 0xFF, xnand->oobsize);
	} else {
		memcpy(buf, xnand->pagebuf, xnand->pagesize);
		if (oob_required)
			memcpy(chip->oob_poi, xnand->oobbuf, xnand->oobsize);
	}

	xnand->cache->unlock_read_page(xnand->cache, xnand->cmdfunc->page_addr);
	xnand->pagebuf = xnand->org_pagebuf;

	return 0;
}
/******************************************************************************/

static void hifmc100_xnand_write_buf(struct mtd_info *mtd, const uint8_t *buf,
				     int len)
{
	int offset;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;

	offset = cmdfunc->column + cmdfunc->offset;

	if (offset >= xnand->pagesize)
		memcpy(xnand->oobbuf + offset - xnand->pagesize, buf, len);
	else if (offset + len <= xnand->pagesize)
		memcpy(xnand->pagebuf + offset, buf, len);
	else {
		int nr_copy = xnand->pagesize - offset;
		memcpy(xnand->pagebuf + offset, buf, nr_copy);
		memcpy(xnand->oobbuf, buf + nr_copy, len - nr_copy);
	}

	cmdfunc->offset += len;
}
/******************************************************************************/

static int hifmc100_xnand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
				     const uint8_t *buf, int oob_required)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;

	memcpy(xnand->pagebuf, buf, xnand->pagesize);
	if (oob_required)
		memcpy(xnand->oobbuf, chip->oob_poi, xnand->oobsize);

	return 0;
}
/******************************************************************************/

static u32 hifmc100_ecc_strength_reg(u32 value, bool _to_reg)
{
	int ix;
	u32 ecc_strength_reg[] = {
		0,  HIFMC100_CFG_ECC_TYPE_NONE,
		8,  HIFMC100_CFG_ECC_TYPE_8BIT,
		16, HIFMC100_CFG_ECC_TYPE_16BIT,
		24, HIFMC100_CFG_ECC_TYPE_24BIT,
		28, HIFMC100_CFG_ECC_TYPE_28BIT,
		40, HIFMC100_CFG_ECC_TYPE_40BIT,
		64, HIFMC100_CFG_ECC_TYPE_64BIT,
	};

	if (_to_reg) {
		for (ix = 0; ix < ARRAY_SIZE(ecc_strength_reg); ix += 2)
			if (ecc_strength_reg[ix] == value)
				return ecc_strength_reg[ix+1];
	} else {
		for (ix = 0; ix < ARRAY_SIZE(ecc_strength_reg); ix += 2)
			if (ecc_strength_reg[ix+1] == value)
				return ecc_strength_reg[ix];
	}

	panic(DEVNAME ": invalid ecc strength or register. value:0x%08x\n", value);

	return 0;
}
/******************************************************************************/

static u32 hifmc100_pagesize_reg(u32 value, bool _to_reg)
{
	int ix;
	u32 pagesize_reg[] = {
		SZ_2K,  HIFMC100_CFG_PAGE_SIZE_2K,
		SZ_4K,  HIFMC100_CFG_PAGE_SIZE_4K,
		SZ_8K,  HIFMC100_CFG_PAGE_SIZE_8K,
		SZ_16K, HIFMC100_CFG_PAGE_SIZE_16K,
	};

	if (_to_reg) {
		for (ix = 0; ix < ARRAY_SIZE(pagesize_reg); ix += 2)
			if (pagesize_reg[ix] == value)
				return pagesize_reg[ix+1];
	} else {
		for (ix = 0; ix < ARRAY_SIZE(pagesize_reg); ix += 2)
			if (pagesize_reg[ix+1] == value)
				return pagesize_reg[ix];
	}

	panic(DEVNAME ": invalid pagesize or register. value:0x%08x\n", value);

	return 0;
}
/******************************************************************************/

static void hifmc100_dump_bitflip(void *host, u8 ecc[16], int *max_bitsflip)
{
	u32 *e = (u32 *)ecc;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)host;
	struct hifmc_host *hifmc = xnand->host;

	e[0] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0);

	if (xnand->pagesize >= SZ_8K) {
		e[1] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0 + 0x4);

		if (xnand->pagesize >= SZ_16K) {
			e[2] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0 + 0x8);
			e[3] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0 + 0xC);
		}
	}

	if (max_bitsflip) {
		int ix;
		*max_bitsflip = 0;

		for (ix = 0; ix < xnand->ctrl_info->nr_ecc_sect; ix++) {
			if (*max_bitsflip < ecc[ix])
				*max_bitsflip = ecc[ix];
		}
	}
}
/******************************************************************************/

static struct nand_ecclayout hifmc100_xnand_ecclayout = {
	.oobfree = {{2, 30}},
};
/******************************************************************************/

#define SET_CTRL_INFO(_pagesize, _ecc_strength, _sz_data, _sz_ecc, _threshold) { \
	.pagesize = (_pagesize), \
	.ecc = { .strength = (_ecc_strength), .step = SZ_1K, }, \
	.section = { .sz_data = (_sz_data), .sz_ecc = (_sz_ecc), }, \
	.threshold = (_threshold), \
	.ecclayout = &hifmc100_xnand_ecclayout, \
	.dump = hifmc100_dump_bitflip, \
	.nr_ecc_sect = 0, }

/******************************************************************************/

static struct nand_ctrl_info hifmc100_nand_ctrl_info[] = {
	SET_CTRL_INFO(SZ_2K, 8,  1040, 14, 7),
	SET_CTRL_INFO(SZ_2K, 16, 1028, 28, 14),
	SET_CTRL_INFO(SZ_2K, 24, 1040, 42, 21),
	SET_CTRL_INFO(SZ_2K, 28, 1040, 50, 25),

	SET_CTRL_INFO(SZ_4K, 8,  1032, 14, 7),
	SET_CTRL_INFO(SZ_4K, 16, 1028, 28, 14),
	SET_CTRL_INFO(SZ_4K, 24, 1032, 42, 21),
	SET_CTRL_INFO(SZ_4K, 28, 1032, 50, 25),

	SET_CTRL_INFO(SZ_8K, 24, 1028, 42, 21),
	SET_CTRL_INFO(SZ_8K, 28, 1028, 50, 25),
	SET_CTRL_INFO(SZ_8K, 40, 1028, 70, 34),
	SET_CTRL_INFO(SZ_8K, 64, 1028, 112, 58),

	SET_CTRL_INFO(SZ_16K, 40, 1026, 70, 34),
	SET_CTRL_INFO(SZ_16K, 64, 1026, 112, 58),

	{0},
};
/******************************************************************************/

static struct nand_ctrl_info hifmc100_spinand_ctrl_info[] = {
	SET_CTRL_INFO(SZ_2K, 8,  1040, 14, 7),
	SET_CTRL_INFO(SZ_2K, 24, 1040, 42, 21),

	SET_CTRL_INFO(SZ_4K, 8,  1032, 14, 7),
	SET_CTRL_INFO(SZ_4K, 24, 1032, 42, 21),

	{0},
};
/******************************************************************************/

static int hifmc100_xnand_sanely_check(struct mtd_info *mtd, struct nand_chip *chip,
				       struct hifmc_xnand *xnand)
{
	char blockstr[8];
	char pagestr[8];
	char chipstr[8];

	if (mtd->writesize != xnand->pagesize) {
		panic(DEVNAME ": xnand chip pagesize:%d, controller pagesize:%d, conflic.\n",
			mtd->writesize, xnand->pagesize);
	}

	pr_info("Block:%sB Page:%sB OOB:%dB ",
		xnand->name,
		ultohstr(mtd->erasesize, blockstr, sizeof(blockstr)),
		ultohstr(mtd->writesize, pagestr, sizeof(pagestr)),
		mtd->oobsize);

	if (xnand->pagesize == SZ_2K && xnand->ecc_strength <= 8)
		printk("ECC:%dbits/512 ", xnand->ecc_strength >> 1);
	else
		printk("ECC:%dbits/1K ", xnand->ecc_strength);

	printk("Chip:%sB ", ultohstr(mtd->size, chipstr, sizeof(chipstr)));

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND)
		printk("maxclk:%luM ", xnand->clk_rate / 1000000);

	if (xnand->read_retry)
		printk("read-retry ");

	printk("\n");

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;
	void (*callback) (struct erase_info *) = instr->callback;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	instr->callback = NULL;
	ret = xnand->mtd_func->_erase(mtd, instr);
	instr->callback = callback;

	mutex_unlock(&host->lock);

	if (!ret && instr->callback)
		instr->callback(instr);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_read(struct mtd_info *mtd, loff_t from, size_t len,
				   size_t *retlen, u_char *buf)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_read(mtd, from, len, retlen, buf);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_write(struct mtd_info *mtd, loff_t to, size_t len,
				    size_t *retlen, const u_char *buf)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_write(mtd, to, len, retlen, buf);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_panic_write(struct mtd_info *mtd, loff_t to,
					  size_t len, size_t *retlen,
					  const u_char *buf)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_panic_write(mtd, to, len, retlen, buf);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_read_oob(struct mtd_info *mtd, loff_t from,
				       struct mtd_oob_ops *ops)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_read_oob(mtd, from, ops);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_write_oob(struct mtd_info *mtd, loff_t to,
					struct mtd_oob_ops *ops)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_write_oob(mtd, to, ops);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_block_isbad(struct mtd_info *mtd, loff_t ofs)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_block_isbad(mtd, ofs);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_mtd_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	int ret;
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;

	mutex_lock(&host->lock);
	host->set_ifmode(host, xnand->ifmode);

	ret = xnand->mtd_func->_block_markbad(mtd, ofs);

	mutex_unlock(&host->lock);

	return ret;
}
/******************************************************************************/

static int hifmc100_xnand_regop_open(void *priv)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)priv;
	struct hifmc_host *hifmc = xnand->host;

	mutex_lock(&hifmc->lock);
	hifmc->set_ifmode(hifmc, xnand->ifmode);
	xnand->chip->select_chip(xnand->mtd, 0);

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_regop_release(void *priv)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)priv;
	struct hifmc_host *hifmc = xnand->host;

	mutex_unlock(&hifmc->lock);

	return 0;
}
/******************************************************************************/

static int hifmc100_nand_read_mp_start(struct hifmc_xnand *nand, int page_addr,
				       int nr_pages)
{
	u32 regval;
	int fail_addr;
	struct hifmc_host *hifmc = nand->host;
	struct hifmc_reg *reg = &nand->reg.read;

	regval = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	if (nand->ecc_strength)
		regval |= nand->reg.fmc_cfg_ecc_type;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	/* set address cycle and chip select */
	regval = reg->op_cfg;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* set randomizer */
	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	if (nand->randomizer_en)
		regval |= HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	else
		regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set address */
	hifmc_write(hifmc, page_addr << 16, HIFMC100_ADDRL);
	hifmc_write(hifmc, page_addr >> 16, HIFMC100_ADDRH);

	regval = (HIFMC100_FMC_CACHE_PAGE_NUM_MASK & nr_pages) |
		HIFMC100_FMC_CACHE_EXIT_OP_EN;
	hifmc_write(hifmc, regval, HIFMC100_FMC_CACHE_CTRL);

	hifmc->irq_enable(hifmc);

	regval = HIFMC100_OP_CTRL_DMA_OP_READY |
		HIFMC100_OP_CTRL_RD_OP_SEL_CACHE;
	hifmc_write(hifmc, regval, HIFMC100_OP_CTRL);

	hifmc->wait_dma_finish(hifmc);

	/* disable dma done interrupt */
	regval = hifmc_read(hifmc, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_OP_DONE;
	hifmc_write(hifmc, regval, HIFMC100_INT_EN);

	regval = hifmc_read(hifmc, HIFMC100_FMC_ECC_INVALID_ADDRL);
	fail_addr = ((regval & 0xFFFF0000) >> 16);

	regval = hifmc_read(hifmc, HIFMC100_FMC_ECC_INVALID_ADDRH);
	fail_addr |= ((regval & 0xFF) << 16);

	return fail_addr;
}
/******************************************************************************/

static int hifmc100_nand_read_mp_prepare(struct hifmc_xnand *nand,
					 int page_addr, struct page_info *pages)
{
	int ix;
	struct flash_cache *cache = nand->cache;

	for (ix = 0; ix < 8; ix++) {
		int ret;
		int reg_addr;
		struct page_info *page = &pages[ix];

		page->page_addr = page_addr++;

		if (page->page_addr >= cache->max_pages) {
			page->status = FLASHCACHE_PAGE_UNKNOWN;
			break;
		}

		page->status = cache->get_status_lock_read_page(cache,
			page->page_addr);
		if (page->status != FLASHCACHE_PAGE_UNKNOWN) {
			cache->unlock_read_page(cache, page->page_addr);
			break;
		}

		ret = cache->new_cache(cache, page->page_addr, &page->pagebuf,
			&page->oobbuf);
		if (ret) {
			cache->unlock_read_page(cache, page->page_addr);
			break;
		}

		if (!ix)
			reg_addr = HIFMC100_DMA_SADDR_D0;
		else
			reg_addr = HIFMC100_FMC_DMA_SADDR_D0_P1 +
				(ix - 1) * 0x14;

		page->pagesize = nand->pagesize;
		page->oobsize = 32;

		ret = hifmc100_xnand_dma_map(nand, page, reg_addr,
			DMA_FROM_DEVICE);
		if (ret) {
			cache->discard_cache(cache, page->page_addr, 1,
				FLASHCACHE_PAGE_UNKNOWN);
			cache->unlock_read_page(cache, page->page_addr);
			break;
		}
	}

	if (ix == 8) {
		pages[8].status = FLASHCACHE_PAGE_UNKNOWN;
		pages[8].page_addr = page_addr;
	}

	return ix;
}
/******************************************************************************/

static int hifmc100_nand_read_mp_finish(struct hifmc_xnand *nand,
					int fail_addr, struct page_info *pages,
					int count)
{
	int ix;
	int discard;
	struct page_info *page;
	struct flash_cache *cache = nand->cache;
	u32 blockmask = cache->pages_per_block - 1;

	discard = nand->host->intr_status & HIFMC100_INT_ERR_INVALID;

	for (ix = 0; ix < count; ix++) {
		page = &pages[ix];

		dma_unmap_single(nand->dev, page->dma_pagebuf, page->pagesize,
			DMA_FROM_DEVICE);
		dma_unmap_single(nand->dev, page->dma_oobbuf, page->oobsize,
			DMA_FROM_DEVICE);
	}

	for (ix = 0; ix < count; ix++) {
		page = &pages[ix];

		if (!(page->page_addr & blockmask) &&
		    BBM(page->oobbuf) == 0x00) {
			cache->discard_cache(cache, page->page_addr, count - ix,
				FLASHCACHE_PAGE_BADBLOCK);

			pages[count].status = FLASHCACHE_PAGE_BADBLOCK;

			break;
		}

		if (EPM(page->oobbuf) == 0xFFFF) {
			int remainder = cache->pages_per_block -
				(page->page_addr & blockmask);

			if (remainder > (count - ix))
				remainder = count - ix;

			cache->discard_cache(cache, page->page_addr, remainder,
				FLASHCACHE_PAGE_EMPTY);

			ix += remainder - 1;

			pages[ix].status = FLASHCACHE_PAGE_EMPTY;

			continue;
		}

		if (discard && page->page_addr <= fail_addr) {
			int num = fail_addr - page->page_addr + 1;

			cache->discard_cache(cache, page->page_addr,
				num, FLASHCACHE_PAGE_UNKNOWN);

			ix += num - 1;

			continue;
		}
	}

	for (ix = 0; ix < count; ix++) {
		page = &pages[ix];
		cache->unlock_read_page(cache, page->page_addr);
	}

	return 0;
}
/******************************************************************************/

static int hifmc100_nand_read_ahead(void *arg, int pageindex, int *skip_pages)
{
	int count;
	int status;
	struct page_info pages[9];

	struct hifmc_xnand *nand = (struct hifmc_xnand *)arg;
	struct hifmc_host *hifmc = nand->host;
	struct flash_cache *cache = nand->cache;

	mutex_lock(&hifmc->lock);
	hifmc->set_ifmode(hifmc, nand->ifmode);
	nand->chip->select_chip(nand->mtd, 0);

	count = hifmc100_nand_read_mp_prepare(nand, pageindex, pages);
	if (count) {
		int fail_addr = hifmc100_nand_read_mp_start(nand, pageindex,
			count);
		hifmc100_nand_read_mp_finish(nand, fail_addr, pages, count);
	}

	mutex_unlock(&hifmc->lock);

	status = pages[count].status;

	if (status == FLASHCACHE_PAGE_EMPTY ||
	    status == FLASHCACHE_PAGE_BADBLOCK) {
		u32 blockmask = cache->pages_per_block - 1;
		int next_block_start = (pages[count].page_addr +
			cache->pages_per_block) & ~blockmask;

		*skip_pages = next_block_start - pageindex;
	} else {
		*skip_pages = count;

		if (!*skip_pages)
			*skip_pages = 1;

		while (status == FLASHCACHE_PAGE_CACHE &&
		       (pageindex + (*skip_pages) + 1) < cache->max_pages)
			status = cache->peek_status(cache,
				pageindex + (++(*skip_pages)));
	}

	return 0;
}
/******************************************************************************/

static void hifmc100_spinand_set_xfer(struct hifmc_xnand *xnand)
{
	struct spinand_chip_xfer *xfer;
	struct hifmc_reg *read, *write;

	xfer = spinand_get_xfer(hifmc100_spinand_chip_xfer,
		xnand->id, sizeof(xnand->id));

	xnand->clk_rate = xfer->clock;

	read = &xnand->reg.read;
	write = &xnand->reg.write;

	if (xfer->datapin == 1) {
		read->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_STD |
			HIFMC100_OP_CFG_DUMMY_NUM(1);
		read->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RD_OPCODE(SPINAND_OP_READ);

		write->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_STD;
		write->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RW_OP_WRITE |
			HIFMC100_OP_CTRL_WR_OPCODE(SPINAND_OP_PLD);
	} else if (xfer->datapin == 2) {
		read->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_DUAL |
			HIFMC100_OP_CFG_DUMMY_NUM(1);
		read->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RD_OPCODE(SPINAND_OP_READ_2);

		write->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_DUAL;
		write->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RW_OP_WRITE |
			HIFMC100_OP_CTRL_WR_OPCODE(SPINAND_OP_PLD);

	} else { /* default xfer->datapin == 4 */
		read->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_QUAD |
			HIFMC100_OP_CFG_DUMMY_NUM(1);
		read->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RD_OPCODE(SPINAND_OP_READ_4);

		write->op_cfg = HIFMC100_OP_CFG_MEM_IFTYPE_QUAD;
		write->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
			HIFMC100_OP_CTRL_RW_OP_WRITE |
			HIFMC100_OP_CTRL_WR_OPCODE(SPINAND_OP_PLD_4);
	}
}
/******************************************************************************/

static int hifmc100_xnand_probe_device(struct platform_device *pdev,
				       struct hifmc_xnand *xnand)
{
	int ret;
	struct mtd_info *mtd = xnand->mtd;
	struct nand_chip *chip = xnand->chip;
	struct nand_ecc_ctrl *ecc = &chip->ecc;
	struct nand_flash_dev *nand_id_table;

	chip->options = NAND_SKIP_BBTSCAN | NAND_NO_SUBPAGE_WRITE;
	chip->cmdfunc = hifmc100_xnand_cmdfunc;
	chip->dev_ready = hifmc100_xnand_dev_ready;
	chip->select_chip = hifmc100_xnand_select_chip;
	chip->read_byte = hifmc100_xnand_read_byte;
	chip->read_buf = hifmc100_xnand_read_buf;
	chip->write_buf = hifmc100_xnand_write_buf;

	if (xnand->ifmode == HIFMC_IFMODE_NAND)
		nand_id_table = nand_flash_ids_hisi;
	else
		nand_id_table = spinand_flash_ids_hisi;

	ret = nand_scan_ident(mtd, HIFMC100_OP_CFG_NUM_CS, nand_id_table);
	if (ret)
		return ret;

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND) {
		hifmc100_spinand_set_xfer(xnand);
	} else {
		xnand->reg.fmc_pnd_pwidth_cfg = nand_get_clock(
			hifmc100_nand_rwlatch,
			xnand->id, sizeof(xnand->id));

		hifmc100_set_nand_fast_timing(xnand);
	}

	if (chip->read_retry_type) {
		xnand->read_retry = nand_get_read_retry(chip->read_retry_type);
		if (!xnand->read_retry)
			panic(DEVNAME ": not support read retry type: %d\n", chip->read_retry_type);
	}

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND) {
		int ix;

		xnand->spinand_drv = spinand_get_drvier(xnand->id,
			sizeof(xnand->id));

		for (ix = 0; ix < chip->numchips; ix++) {
			chip->select_chip(mtd, ix);
			xnand->spinand_drv->chip_prepare(xnand->regop);
		}
		chip->select_chip(mtd, -1);
	}

	xnand->blocksize = mtd->erasesize;
	xnand->page_per_block = (xnand->blocksize >> chip->page_shift);

	hifmc100_xnand_sanely_check(mtd, chip, xnand);

	if (xnand->ecc_strength && xnand->pagesize >= SZ_8K)
		xnand->randomizer_en = true;
	else
		xnand->randomizer_en = false;

	/* One more address cycle for devices > 128MiB */
	if (chip->chipsize > (128 << 20))
		xnand->erase_addr_cycle = 3;
	else
		xnand->erase_addr_cycle = 2;

	xnand->read_addr_cycle = xnand->erase_addr_cycle + 2;

	ecc->mode = NAND_ECC_HW;
	ecc->layout = xnand->ctrl_info->ecclayout;

	/* nothing. only eliminate kernel ecc check warning */
	ecc->size = xnand->ctrl_info->ecc.step;
	ecc->strength = 60;

	ecc->read_page_raw = hifmc100_xnand_read_page;
	ecc->read_page = hifmc100_xnand_read_page;
	ecc->write_page = hifmc100_xnand_write_page;
	ecc->write_page_raw = hifmc100_xnand_write_page;

	xnand->pagebuf = devm_kmalloc(&pdev->dev, mtd->writesize, GFP_KERNEL);
	if (!xnand->pagebuf) {
		pr_err("out of memory.\n", DEVNAME);
		return -EIO;
	}
	xnand->org_pagebuf = xnand->pagebuf;

	xnand->oobbuf = devm_kmalloc(&pdev->dev, mtd->oobsize, GFP_KERNEL);
	if (!xnand->oobbuf) {
		devm_kfree(&pdev->dev, xnand->pagebuf);
		pr_err("out of memory.\n", DEVNAME);
		return -EIO;
	}

	xnand->dev = &pdev->dev;
	xnand->oobsize = mtd->oobsize;
	memset(xnand->oobbuf, 0xFF, xnand->oobsize);

	ret = nand_scan_tail(mtd);
	if (ret) {
		devm_kfree(&pdev->dev, xnand->pagebuf);
		devm_kfree(&pdev->dev, xnand->oobbuf);
		return ret;
	}

#define _SET_FUNC(_func)  \
	xnand->mtd_func->_func = mtd->_func, \
	mtd->_func = hifmc100_xnand_mtd##_func;

	/*
	 * mtd->_erase = nand_erase; {
	 *     nand_erase_nand(); {
	 *         nand_check_wp();
	 *         nand_block_checkbad();
	 *         chip->erase() = single_erase(); {
	 *             chip->cmdfunc(mtd, NAND_CMD_ERASE1, -1, page);
	 *             chip->cmdfunc(mtd, NAND_CMD_ERASE2, -1, -1);
	 *             chip->waitfunc(mtd, chip);
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_erase);

	/*
	 * mtd->_read = nand_read(); {
	 *     nand_do_read_ops(); {
	 *         chip->cmdfunc(mtd, NAND_CMD_READ0, 0x00, page) = hifmc100_xnand_cmdfunc();
	 *         chip->ecc.read_page = hifmc100_xnand_read_page();
	 *         nand_wait_ready(); {
	 *             chip->dev_ready = hifmc100_xnand_dev_ready();
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_read);

	/*
	 * mtd->_write = nand_write(); {
	 *     nand_do_write_ops(); {
	 *         nand_check_wp();
	 *         chip->write_page = nand_write_page(); {
	 *             chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, page);
	 *             chip->ecc.write_page = hifmc100_xnand_write_page();
	 *             chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
	 *             chip->waitfunc(mtd, chip) = nand_wait(); {
	 *                 chip->dev_ready();
	 *                 chip->read_byte() = hifmc100_xnand_read_byte();
	 *             }
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_write);
	_SET_FUNC(_panic_write);

	/*
	 * mtd->_read_oob = nand_read_oob() {
	 *     nand_do_read_oob(); {
	 *         chip->ecc.read_oob() = nand_read_oob_std(); {
	 *             chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
	 *             chip->read_buf(mtd, chip->oob_poi, mtd->oobsize) = hifmc100_xnand_read_buf();
	 *         }
	 *         nand_transfer_oob();
	 *         nand_wait_ready();
	 *     }
	 * }
	 */
	_SET_FUNC(_read_oob);


	/*
	 * mtd->_write_oob = nand_write_oob(); {
	 *     nand_do_write_oob(); {
	 *         chip->cmdfunc(mtd, NAND_CMD_RESET, -1, -1);
	 *         nand_fill_oob();
	 *         chip->ecc.write_oob() = nand_write_oob_std(); {
	 *             chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize, page);
	 *             chip->write_buf(mtd, buf, length);
	 *             chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
	 *             chip->waitfunc(mtd, chip);
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_write_oob);

	/*
	 * mtd->_block_isbad = nand_block_isbad(); {
	 *     nand_block_checkbad(); {
	 *         chip->block_bad() = nand_block_bad(); {
	 *             chip->cmdfunc(mtd, NAND_CMD_READOOB, chip->badblockpos, page);
	 *             chip->read_byte();
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_block_isbad);

	/*
	 * mtd->_block_markbad = nand_block_markbad(); {
	 *     nand_block_isbad(); {
	 *         nand_block_checkbad();
	 *     }
	 *     nand_block_markbad_lowlevel(); {
	 *         nand_erase_nand();
	 *         chip->block_markbad = nand_default_block_markbad(); {
	 *             nand_do_write_oob();
	 *         }
	 *     }
	 * }
	 */
	_SET_FUNC(_block_markbad);

#undef _SET_FUNC

	return 0;
}
/******************************************************************************/

static void hifmc100_spinand_resume(struct hifmc_xnand *xnand)
{
	int nrchip;
	u32 regval;
	struct hifmc_host *hifmc = xnand->host;
	struct nand_chip *chip = xnand->chip;

	mutex_lock(&hifmc->lock);

	regval = hifmc_read(hifmc, HIFMC100_CFG);
	regval |= HIFMC100_CFG_OP_MODE_NORMAL;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, 0x6F, HIFMC100_TIMING_SPI_CFG);

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINAND);
	clk_set_rate(hifmc->clk, hifmc->spinand->clk_rate);

	for (nrchip = 0; nrchip < chip->numchips; nrchip++) {
		chip->select_chip(xnand->mtd, nrchip);
		xnand->cmd_reset(xnand);
		xnand->spinand_drv->chip_prepare(xnand->regop);
	}

	mutex_unlock(&hifmc->lock);
}
/******************************************************************************/

static void hifmc100_nand_resume(struct hifmc_xnand *xnand)
{
	int nrchip;
	u32 regval;
	struct hifmc_host *hifmc = xnand->host;
	struct nand_chip *chip = xnand->chip;

	mutex_lock(&hifmc->lock);

	regval = hifmc_read(hifmc, HIFMC100_CFG);
	regval |= HIFMC100_CFG_OP_MODE_NORMAL;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, xnand->reg.fmc_pnd_pwidth_cfg,
		HIFMC100_PND_PWIDTH_CFG);

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_NAND);
	clk_set_rate(hifmc->clk, hifmc->nand->clk_rate);

	for (nrchip = 0; nrchip < chip->numchips; nrchip++) {
		chip->select_chip(xnand->mtd, nrchip);
		xnand->cmd_reset(xnand);
	}

	hifmc100_set_nand_fast_timing(xnand);

	mutex_unlock(&hifmc->lock);
}
/******************************************************************************/

static void hifmc100_spinand_setup(struct hifmc_xnand *xnand)
{
	xnand->regop->read_sr = hifmc100_spinand_read_sr;
	xnand->read_raw_bbm = hifmc100_spinand_read_raw_bbm;
	xnand->cmd_readid = hifmc100_spinand_cmd_readid;
	xnand->cmd_reset = hifmc100_spinand_cmd_reset;
	xnand->cmd_erase = hifmc100_spinand_cmd_erase;

	xnand->resume = hifmc100_spinand_resume;

	xnand->clk_rate = 0; /* min clock do read id, reset.*/
}
/******************************************************************************/

static void hifmc100_nand_setup(struct hifmc_xnand *xnand)
{
	struct hifmc_reg *reg;

	reg = &xnand->reg.read;

	reg->op_cfg = HIFMC100_OP_CFG_ADDR_NUM(xnand->read_addr_cycle);

	reg->cmd = (NAND_CMD_READSTART << 8) | NAND_CMD_READ0;

	reg->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY;

	reg = &xnand->reg.write;

	reg->op_cfg = HIFMC100_OP_CFG_ADDR_NUM(xnand->read_addr_cycle);

	reg->cmd = (NAND_CMD_PAGEPROG << 8) | NAND_CMD_SEQIN;

	reg->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
		HIFMC100_OP_CTRL_RW_OP_WRITE;

	xnand->regop->read_sr = hifmc100_nand_read_sr;
	xnand->read_raw_bbm = hifmc100_nand_read_raw_bbm;
	xnand->cmd_readid = hifmc100_nand_cmd_readid;
	xnand->cmd_reset = hifmc100_nand_cmd_reset;
	xnand->cmd_erase = hifmc100_nand_cmd_erase;

	xnand->resume = hifmc100_nand_resume;

	xnand->clk_rate = (ulong)(-1); /* max clock do read id, reset */
}
/******************************************************************************/

int hifmc100_xnand_probe(struct platform_device *pdev, struct hifmc_host *host,
			 int ifmode)
{
	int ret;
	struct hifmc_xnand *xnand;
	struct nand_ctrl_info *ctrl_info;
	struct flash_regop_intf *regop_intf;
	int (*read_ahead)(void *, int, int *) = NULL;

	xnand = devm_kzalloc(&pdev->dev, sizeof(struct hifmc_xnand), GFP_KERNEL);
	if (!xnand) {
		pr_err("failed to allocate hifmc_xnand structure.\n", DEVNAME);
		return -ENOMEM;
	}

	xnand->host = host;
	xnand->chip->priv = xnand;
	xnand->mtd->priv = xnand->chip;
	xnand->chip->IO_ADDR_R = xnand->chip->IO_ADDR_W = host->iobase;
	xnand->mtd->name = "hinand";
	xnand->mtd->owner = THIS_MODULE;

	xnand->ifmode = ifmode;
	xnand->regop->priv = host;
	xnand->regop->read_reg = hifmc100_read_reg;
	xnand->regop->write_reg = hifmc100_write_reg;
	xnand->regop->wait_device_ready = hifmc100_xnand_wait_device_ready;

	if (ifmode == HIFMC_IFMODE_NAND) {
		ctrl_info = hifmc100_nand_ctrl_info;

		host->nand = xnand;
		xnand->name = "nand";

		hifmc100_nand_setup(xnand);
	} else if (ifmode == HIFMC_IFMODE_SPINAND) {
		ctrl_info = hifmc100_spinand_ctrl_info;

		host->spinand = xnand;
		xnand->name = "spinand";

		hifmc100_spinand_setup(xnand);
	} else
		panic(DEVNAME ": invalid ifmode:%d\n", ifmode);

	host->set_ifmode(host, ifmode);

	/* force change clock rate when it call set_ifmode next time. */
	host->set_ifmode(host, HIFMC_IFMODE_INVALID);

	hifmc100_xnand_select_chip(xnand->mtd, 0);
	xnand->cmd_reset(xnand);
	xnand->cmd_readid(xnand);
	if (!xnand->id[0])
		goto fail;

	xnand->ecc_strength = 
		hifmc100_ecc_strength_reg(host->reg.fmc_cfg_ecc_type, false);
	xnand->pagesize = hifmc100_pagesize_reg(host->reg.fmc_cfg_page_size, false);

	xnand->reg.fmc_cfg_ecc_type = host->reg.fmc_cfg_ecc_type;
	xnand->reg.fmc_cfg_page_size = host->reg.fmc_cfg_page_size;

	xnand->ctrl_info = nand_get_ctrl_info(ctrl_info, xnand->pagesize,
		xnand->ecc_strength);
	if (!xnand->ctrl_info) {
		/*
		 * when spi-nor on board, host will find a invalid spi-nand, and goto here
		 * we ignore this error.
		  */
		printk(DEVNAME ": invalid pagesize(%d) or ecc_strength(%d).\n",
			xnand->pagesize, xnand->ecc_strength);
		goto fail;
	}

	if (hifmc100_xnand_probe_device(pdev, xnand))
		goto fail;

	if (ifmode == HIFMC_IFMODE_NAND && xnand->manuf != NAND_MFR_AMD)
		read_ahead = hifmc100_nand_read_ahead;

	xnand->cache = flash_cache_create(xnand->name, xnand->mtd->size,
		xnand->blocksize, xnand->pagesize, 32, 0,
		read_ahead, (void *)xnand);
	if (!xnand->cache)
		goto fail;

	xnand->stats = flash_stats_create(xnand->name, xnand->pagesize,
		xnand->mtd->erasesize, xnand->mtd->size,
		xnand->ctrl_info->nr_ecc_sect,
		(xnand->read_retry ? xnand->read_retry->count : 0));
	if (!xnand->stats) {
		flash_cache_destory(xnand->cache);
		goto fail;
	}

	ret = mtd_device_parse_register(xnand->mtd, NULL, NULL, NULL, 0);
	if (ret) {
		flash_cache_destory(xnand->cache);
		flash_stats_destory(xnand->stats);
		goto fail;
	}

	regop_intf = &xnand->regop_intf;

	strncpy(regop_intf->name, xnand->name, sizeof(regop_intf->name) - 1);

	regop_intf->sz_buf = host->sz_iobase;
	regop_intf->args = xnand;
	regop_intf->regop = xnand->regop;
	regop_intf->open = hifmc100_xnand_regop_open;
	regop_intf->release = hifmc100_xnand_regop_release;
	regop_intf_create(regop_intf);

	return 0;

fail:
	devm_kfree(&pdev->dev, xnand);

	if (ifmode == HIFMC_IFMODE_NAND)
		host->nand = NULL;
	else if (ifmode == HIFMC_IFMODE_SPINAND)
		host->spinand = NULL;

	return -ENODEV;
}
