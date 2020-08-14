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


#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <malloc.h>
#include <match_table.h>
#include <asm/arch/platform.h>
#include <linux/mtd/nand.h>
#include <hinfc_gen.h>
#include <asm/dma-mapping.h>
#include <hinfc_gen.h>

#include "hiflash.h"
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
static void nand_register_dump(void)
{
	int ix;
	u32 regbase = CONFIG_HIFMC100_REG_BASE_ADDRESS;

	printf("Register dump:");
	for (ix = 0; ix <= 0xF0; ix += 0x04) {
		if (!(ix & 0x0F))
			printf("\n0x%08X: ", (regbase + ix));
		printf("%08X ", readl(regbase + ix));
	}
	printf("\n");
}
/******************************************************************************/

static int hifmc100_xnand_wait_device_ready(struct flash_regop *regop)
{
	int status;
	ulong deadline;

	deadline = 1 << 15;
	do {
		status = regop->read_sr(regop);
		if (status & NAND_STATUS_READY)
			return 0;

	} while (deadline--);

	PR_OUT("wait device ready timeout.\n");

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

static u32 hifmc100_xnand_is_page_empty(struct hifmc_xnand *xnand,
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

static u32 hifmc100_xnand_is_bad_block(struct hifmc_xnand *xnand,
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

	page->dma_pagebuf = dma_map_single(page->pagebuf, page->pagesize, direction);

	hifmc_write(hifmc, (u32)page->dma_pagebuf, dmaaddr_reg);

	if (xnand->pagesize >= SZ_8K) {
		u32 phyaddr = (u32)page->dma_pagebuf + SZ_4K;

		hifmc_write(hifmc, phyaddr, dmaaddr_reg + 0x4);

		if (xnand->pagesize >= SZ_16K) {
			hifmc_write(hifmc, phyaddr + SZ_4K, dmaaddr_reg + 0x8);
			hifmc_write(hifmc, phyaddr + SZ_8K, dmaaddr_reg + 0xC);
		}
	}

	page->dma_oobbuf = dma_map_single(page->oobbuf, page->oobsize, direction);

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

	FMC_DMA_WAIT_INT_FINISH(hifmc);

	/* disable dma done interrupt */
	regval = hifmc_read(hifmc, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_OP_DONE;
	hifmc_write(hifmc, regval, HIFMC100_INT_EN);

	dma_unmap_single((void *)page.dma_pagebuf, page.pagesize, direction);
	dma_unmap_single((void *)page.dma_oobbuf, page.oobsize, direction);

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
#if 0
static void hifmc100_spinand_reg_read(struct hifmc_xnand *xnand)
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
#endif
/******************************************************************************/
void hifmc100_set_ecc_randomizer(struct hifmc_xnand *xnand, int enable)
{
	int regval;
	struct hifmc_host *host = xnand->host;

	if (enable) {
		regval = hifmc_read(host, HIFMC100_CFG);
		regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
		regval |= xnand->reg.fmc_cfg_ecc_type;
		hifmc_write(host, regval, HIFMC100_CFG);

		if (xnand->randomizer_en) {
			regval = hifmc_read(host, HIFMC100_GLOBAL_CFG);
			regval |= HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
			hifmc_write(host, regval, HIFMC100_GLOBAL_CFG);
		}
	} else {
		regval = hifmc_read(host, HIFMC100_CFG);
		regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
		hifmc_write(host, regval, HIFMC100_CFG);

		regval = hifmc_read(host, HIFMC100_GLOBAL_CFG);
		regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
		hifmc_write(host, regval, HIFMC100_GLOBAL_CFG);
	}
}

/******************************************************************************/

static void hifmc100_xnand_cmdfunc(struct mtd_info *mtd, unsigned int command,
				   int column, int page_addr)
{
	struct nand_chip *chip = (struct nand_chip *)mtd->priv;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct hifmc_host *host = xnand->host;

	host->set_ifmode(host, xnand->ifmode);

	xnand->is_empty_page = 0;
	xnand->is_bad_block = 0;
	xnand->is_uc_err = 0;

	cmdfunc->command = command;

	switch (command) {
	case NAND_CMD_READOOB:
		column += xnand->pagesize;

	case NAND_CMD_READ0:
		cmdfunc->column = column;
		cmdfunc->page_addr = page_addr;
		cmdfunc->offset = 0;

		hifmc100_xnand_dma_transfer(xnand, DMA_FROM_DEVICE);
		hifmc100_xnand_fix_read(xnand);

		break;

	case NAND_CMD_ERASE1:
		cmdfunc->page_addr = page_addr;
		break;

	case NAND_CMD_ERASE2:
		xnand->cmd_erase(xnand);
		break;

	case NAND_CMD_SEQIN:
		cmdfunc->column = column;
		cmdfunc->offset = 0;
		cmdfunc->page_addr = page_addr;

		break;

	case NAND_CMD_PAGEPROG:
		EPM(xnand->oobbuf) = 0;
		hifmc100_xnand_dma_transfer(xnand, DMA_TO_DEVICE);

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
		printf("%s not support command 0x%08x:\n", xnand->name, command);
		break;
	}

	/* pass pagesize and ecctype to kernel when startup. */
	hifmc100_set_ecc_randomizer(xnand, ENABLE);
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
		xnand->pagebuf = xnand->org_pagebuf;
	}
}
/******************************************************************************/

static int hifmc100_xnand_read_page(struct mtd_info *mtd, struct nand_chip *chip,
				    uint8_t *buf, int page)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;

	if (xnand->is_empty_page) {
		memset(buf, 0xFF, xnand->pagesize);
		memset(chip->oob_poi, 0xFF, xnand->oobsize);
	} else {
		memcpy(buf, xnand->pagebuf, xnand->pagesize);
		memcpy(chip->oob_poi, xnand->oobbuf, xnand->oobsize);
	}

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

static void hifmc100_xnand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
				     const uint8_t *buf)
{
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;

	memcpy(xnand->pagebuf, buf, xnand->pagesize);
	memcpy(xnand->oobbuf, chip->oob_poi, xnand->oobsize);
}
/******************************************************************************/

static u32 hifmc100_ecc_strength_reg(u32 value, u32 _to_reg)
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

static u32 hifmc100_pagesize_reg(u32 value, u32 _to_reg)
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

#define SET_CTRL_INFO(_pagesize, _oobsize, _ecc_strength, _sz_data, _sz_ecc, _threshold) { \
	.pagesize = (_pagesize), \
	.ecc = { .strength = (_ecc_strength), .step = SZ_1K, }, \
	.section = { .sz_data = (_sz_data), .sz_ecc = (_sz_ecc), }, \
	.threshold = (_threshold), \
	.ecclayout = &hifmc100_xnand_ecclayout, \
	.dump = hifmc100_dump_bitflip, \
	.nr_ecc_sect = 0, \
	.oobsize = _oobsize, }

/******************************************************************************/

static struct nand_ctrl_info hifmc100_nand_ctrl_info[] = {
	SET_CTRL_INFO(SZ_2K, 60,  8,  1040, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(SZ_2K, 64,  16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(SZ_2K, 116, 24, 1040, 42, 21),

	SET_CTRL_INFO(SZ_4K, 88,  8,  1032, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(SZ_4K, 128, 16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(SZ_4K, 200, 24, 1032, 42, 21),

	SET_CTRL_INFO(SZ_8K, 368, 24, 1028, 42, 21),
	SET_CTRL_INFO(SZ_8K, 592, 40, 1028, 70, 34),
	SET_CTRL_INFO(SZ_8K, 928, 64, 1028, 112, 58),

	SET_CTRL_INFO(SZ_16K, 1152, 40, 1026, 70, 34),
	SET_CTRL_INFO(SZ_16K, 1824, 64, 1026, 112, 58),

	{0},
};
/******************************************************************************/

static struct nand_ctrl_info hifmc100_spinand_ctrl_info[] = {
	SET_CTRL_INFO(SZ_2K, 60,  8,  1040, 14, 7),
	SET_CTRL_INFO(SZ_2K, 116, 24, 1040, 42, 21),

	SET_CTRL_INFO(SZ_4K, 88,  8,  1032, 14, 7),
	SET_CTRL_INFO(SZ_4K, 200, 24, 1032, 42, 21),

	{0},
};
/******************************************************************************/

static struct nand_ctrl_info hifmc100_auto_nand_ctrl_info[] = {
	SET_CTRL_INFO(SZ_2K, 60,  8,  1040, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(SZ_2K, 64,  16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(SZ_2K, 116, 24, 1040, 42, 21),
	SET_CTRL_INFO(SZ_2K, 132, 28, 1040, 50, 25),

	SET_CTRL_INFO(SZ_4K, 88,  8,  1032, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(SZ_4K, 128, 16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(SZ_4K, 200, 24, 1032, 42, 21),
	SET_CTRL_INFO(SZ_4K, 232, 28, 1032, 50, 25),

	SET_CTRL_INFO(SZ_8K, 368, 24, 1028, 42, 21),
	SET_CTRL_INFO(SZ_8K, 432, 28, 1028, 50, 25),
	SET_CTRL_INFO(SZ_8K, 592, 40, 1028, 70, 34),
	SET_CTRL_INFO(SZ_8K, 928, 64, 1028, 112, 58),

	SET_CTRL_INFO(SZ_16K, 1152, 40, 1026, 70, 34),
	SET_CTRL_INFO(SZ_16K, 1824, 64, 1026, 112, 58),

	{0},
};
/******************************************************************************/

static int hifmc100_xnand_sanely_check(struct mtd_info *mtd, struct nand_chip *chip,
				       struct hifmc_xnand *xnand)
{
	if (mtd->writesize != xnand->pagesize) {
		unsigned int shift = 0;
		unsigned int writesize = mtd->writesize;
		while (writesize > xnand->pagesize) {
			writesize >>= 1;
			shift++;
		}
		chip->chipsize = chip->chipsize >> shift;
		mtd->erasesize = mtd->erasesize >> shift;
		mtd->writesize = xnand->pagesize;
		printk("Nand divide into 1/%u\n", (1 << shift));
	}

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND)
		printk("maxclk:%luM ", xnand->clk_rate / 1000000);

	printk("\n");

	return 0;
}

/******************************************************************************/

static void hifmc100_spinand_set_xfer(struct hifmc_xnand *xnand)
{
	struct spinand_chip_xfer *xfer;
	struct hifmc_reg *read, *write;

	xfer = spinand_get_xfer(hifmc100_spinand_chip_xfer,
		xnand->id, sizeof(xnand->id));

	xnand->clk_rate = xfer->clock * 1000000;

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

struct nand_tag nandtag[1] = {{{0},0,0,0,0,0,0,{0}}};
/******************************************************************************/

static struct nand_flash_dev *
xnand_get_special_flash_type(struct mtd_info *mtd,
			     struct nand_chip *chip,
			     struct nand_dev_t *nand_dev)
{
	int ix;
	struct nand_flash_special_dev *spl_dev;
	unsigned char *byte = nand_dev->ids;
	struct nand_flash_dev *flash_type = &nand_dev->flash_dev;
	struct nand_flash_special_dev *flash_ids_hisi;

	for (ix = 2; ix < 8; ix++)
		byte[ix] = chip->read_byte(mtd);

	printk("Nand ID: 0x%02X 0x%02X 0x%02X 0x%02X"
		" 0x%02X 0x%02X 0x%02X 0x%02X\n",
		byte[0], byte[1], byte[2], byte[3],
		byte[4], byte[5], byte[6], byte[7]);

	if (chip->is_spi_nand)
		flash_ids_hisi = spinand_flash_ids_hisi;
	else
		flash_ids_hisi = nand_flash_ids_hisi;

	for (spl_dev = flash_ids_hisi; spl_dev->id_len; spl_dev++) {
		if (memcmp(byte, spl_dev->id, spl_dev->id_len))
			continue;

		flash_type->options   = spl_dev->options;
		flash_type->pagesize  = spl_dev->pagesize;
		flash_type->erasesize = spl_dev->erasesize;
		nand_dev->oobsize = spl_dev->oobsize;

		nand_dev->read_retry_type = spl_dev->read_retry_type;
		nand_dev->flags = spl_dev->flags;
		chip->read_retry_type = spl_dev->read_retry_type;

		flash_type->id = byte[1];
		flash_type->chipsize = (unsigned long)(spl_dev->chipsize);
		flash_type->name = spl_dev->name;

		memcpy(nandtag->id, byte, 8);
		nandtag->length    = spl_dev->id_len;
		nandtag->chipsize  = spl_dev->chipsize;
		nandtag->pagesize  = spl_dev->pagesize;
		nandtag->erasesize = spl_dev->erasesize;
		nandtag->oobsize   = spl_dev->oobsize;
		nandtag->options   = flash_type->options;
		strncpy(nandtag->name, flash_type->name, 16);
		nandtag->name[sizeof(nandtag->name)-1] = '\0';

		return flash_type;
	}
	nand_dev->read_retry_type = NAND_RR_NONE;

	chip->cmdfunc(mtd, NAND_CMD_READID, 0x00, -1);
	chip->read_byte(mtd);
	chip->read_byte(mtd);

	return NULL;
}
/*****************************************************************************/
/* used the best correct arithmetic. */
static struct nand_ctrl_info *
hifmc100_get_best_ecc(struct nand_ctrl_info *config, struct mtd_info *mtd)
{
	struct nand_ctrl_info *best = NULL;

	for (; config->ecclayout; config++) {

		if (config->pagesize != mtd->writesize)
			continue;

		if (mtd->oobsize < config->oobsize)
			continue;

		if (!best || (best->ecc.strength < config->ecc.strength))
			best = config;
	}

	if (!best)
		PR_BUG(ERSTR_DRIVER
		       "Driver does not support the pagesize(%d) "
		       "and oobsize(%d).\n",
		       mtd->writesize, mtd->oobsize);

	return best;
}

/*****************************************************************************/

static int hifmc100_check_config(struct hifmc_host *host,
				 struct nand_dev_t *nand_dev,
				 struct nand_ctrl_info *best, int is_hw_auto)
{
	int isdump = 0;
	struct hifmc_xnand *xnand = host->nand;

	if (xnand->pagesize != best->pagesize) {
		if (!isdump++)
			nand_register_dump();
		if (is_hw_auto) {
			PR_BUG("!!! warning: "
				"Hardware configure pagesize %s, "
				"but the Nand Flash pageszie is %s. "
				"Update fastboot will resolve this problem.\n",
				nand_page_name(nandpage_size2type(xnand->pagesize)),
				nand_page_name(nandpage_size2type(best->pagesize)));
		} else {
			PR_BUG("!!! warning: "
				"Hardware configure pagesize %s, "
				"but the Nand Flash pageszie is %s. "
				"Modify hardware pin will resolve this problem.\n",
				nand_page_name(nandpage_size2type(xnand->pagesize)),
				nand_page_name(nandpage_size2type(best->pagesize)));
		}
	}

	if (xnand->ecc_strength < best->ecc.strength) {
		if (!is_hw_auto)
			best->ecc.strength = xnand->ecc_strength;
	} else if (xnand->ecc_strength > best->ecc.strength) {
		PR_BUG("!!! warning: "
			"Hardware configure ecctype %dbits/1K, "
			"but %dbits/1K is more suitable for this Nand Flash."
			"Modify hardware pin will resolve this problem.\n",
			xnand->ecc_strength,
			best->ecc.strength);
	}

	if (IS_NAND_RANDOM(nand_dev) && !IS_NAND_RANDOM(host)) {
		if (is_hw_auto) {
			PR_BUG("Hardware is not configure randomizer, "
				"but it is more suitable for this Nand Flash. "
				"Update fastboot will resolve this problem.\n");
		} else {
			PR_BUG("Hardware is not configure randomizer, "
				"but it is more suitable for this Nand Flash. "
				"Modify hardware pin will resolve this problem.\n");
		}
	}

	return 0;
}
/******************************************************************************/
static void hifmc100_set_param(struct hifmc_xnand *xnand)
{
	int regval;
	struct hifmc_host *host = xnand->host;

	regval = hifmc_read(host, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_PAGE_SIZE_MASK;

	switch (xnand->pagesize) {
	case SZ_2K:
		regval |= HIFMC100_CFG_PAGE_SIZE_2K;
		break;
	case SZ_4K:
		regval |= HIFMC100_CFG_PAGE_SIZE_4K;
		break;
	case SZ_8K:
		regval |= HIFMC100_CFG_PAGE_SIZE_8K;
		break;
	case SZ_16K:
		regval |= HIFMC100_CFG_PAGE_SIZE_16K;
		break;
	default:
		break;
	}
	hifmc_write(host, regval, HIFMC100_CFG);

	regval = hifmc_read(host, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_PAGE_PER_BLOCK_MASK;

	switch (xnand->page_per_block) {
	case 64:
		regval |= HIFMC100_CFG_PAGE_PER_BLOCK_64;
		break;
	case 128:
		regval |= HIFMC100_CFG_PAGE_PER_BLOCK_128;
		break;
	case 256:
		regval |= HIFMC100_CFG_PAGE_PER_BLOCK_256;
		break;
	case 512:
		regval |= HIFMC100_CFG_PAGE_PER_BLOCK_512;
		break;
	default:
		break;
	}

	hifmc_write(host, regval, HIFMC100_CFG);
}

/******************************************************************************/

static int hifmc100_xnand_param_adjust(struct mtd_info *mtd, struct nand_chip *chip,
				       struct nand_dev_t *nand_dev)
{
	int bootmedia;
	char *start_type = "unknown";
	struct nand_ctrl_info *best = NULL;
	struct nand_ctrl_info *config = NULL;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)chip->priv;
	struct hifmc_host *host = xnand->host;
	struct nand_ecc_ctrl *ecc = &chip->ecc;

	bootmedia = get_bootmedia(NULL, NULL);

	if ((bootmedia == BOOT_MEDIA_NAND) || (bootmedia == BOOT_MEDIA_SPI_NAND)) {
		if (chip->is_spi_nand)
			config = hifmc100_spinand_ctrl_info;
		else
			config = hifmc100_nand_ctrl_info;

		start_type = "HW-Auto";
		best = hifmc100_get_best_ecc(config, mtd);
		if (IS_NANDC_CONFIG_DONE(host))
			hifmc100_check_config(host, nand_dev, best, 1);
		else if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
			host->flags &= ~NAND_RANDOMIZER;
	} else {
		if (chip->is_spi_nand)
			config = hifmc100_spinand_ctrl_info;
		else
			config = hifmc100_auto_nand_ctrl_info;
		start_type = "Soft-Auto";
		best = hifmc100_get_best_ecc(config, mtd);
		if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
			host->flags &= ~NAND_RANDOMIZER;
	}

	nand_dev->flags |= (IS_NANDC_HW_AUTO(host) |
			    IS_NANDC_CONFIG_DONE(host)); 

	if (!best)
		PR_BUG(ERSTR_HARDWARE
			"Please configure Nand Flash pagesize and ecctype!\n");

	if (best->ecc.strength)
		mtd->oobsize = best->oobsize;
	mtd->oobused = HIFMC100_NAND_OOBSIZE_FOR_YAFFS;
	chip->ecc.layout = best->ecclayout;

	xnand->pagesize = best->pagesize;
	xnand->ecc_strength = best->ecc.strength;
	xnand->oobsize = mtd->oobsize;

	xnand->ctrl_info = nand_get_ctrl_info(config, xnand->pagesize, xnand->ecc_strength);
	if (!xnand->ctrl_info)
		panic(DEVNAME ": invalid pagesize(%d) or ecc_strength(%d).\n",
			xnand->pagesize, xnand->ecc_strength);

	if (mtd->writesize > NAND_MAX_PAGESIZE ||
	    mtd->oobsize > NAND_MAX_OOBSIZE) {
		PR_BUG(ERSTR_DRIVER
			"Driver does not support this Nand Flash. "
			"Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
	}

	nand_dev->start_type = start_type;
	nand_dev->ecctype = nandecc_size2type(xnand->ecc_strength);

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
	xnand->page_per_block = (xnand->blocksize / xnand->pagesize);

	xnand->reg.fmc_cfg_ecc_type = hifmc100_ecc_strength_reg(xnand->ecc_strength, true);
	xnand->reg.fmc_cfg_page_size = hifmc100_pagesize_reg(xnand->pagesize, true);

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

	hifmc100_xnand_sanely_check(mtd, chip, xnand);

	hifmc100_set_param(xnand);

	ecc->mode = NAND_ECC_HW;
	ecc->layout = xnand->ctrl_info->ecclayout;

	ecc->size = xnand->ctrl_info->ecc.step;
	ecc->bytes = xnand->ctrl_info->ecc.strength;

	ecc->read_page_raw = hifmc100_xnand_read_page;
	ecc->read_page = hifmc100_xnand_read_page;
	ecc->write_page = hifmc100_xnand_write_page;
	ecc->write_page_raw = hifmc100_xnand_write_page;

	xnand->pagebuf = kmalloc(mtd->writesize, GFP_KERNEL);
	if (!xnand->pagebuf) {
		PR_OUT("%s out of memory.\n", DEVNAME);
		return -EIO;
	}
	xnand->org_pagebuf = xnand->pagebuf;

	xnand->oobbuf = kmalloc(mtd->oobsize, GFP_KERNEL);
	if (!xnand->oobbuf) {
		kfree(xnand->pagebuf);
		PR_OUT("%s out of memory.\n", DEVNAME);
		return -EIO;
	}

	xnand->oobsize = mtd->oobsize;
	memset(xnand->oobbuf, 0xFF, xnand->oobsize);

	nand_dev->hostver = HIFMC100_VERSION_VALUE;

	return 0;
}
/*****************************************************************************/

void nand_spl_ids_register(void)
{
	nand_get_spl_flash_type = xnand_get_special_flash_type;
}
/******************************************************************************/


static int hifmc100_xnand_probe_device(struct hifmc_xnand *xnand)
{
	int ix;
	int manfid, devid;
	struct nand_chip *chip = xnand->chip;
	struct nand_flash_special_dev *flash_ids_hisi;
	struct nand_flash_special_dev *spl_dev;

	manfid = xnand->id[0];
	devid = xnand->id[1];

	chip->options = NAND_SKIP_BBTSCAN | NAND_NO_SUBPAGE_WRITE;
	chip->cmdfunc = hifmc100_xnand_cmdfunc;
	chip->dev_ready = hifmc100_xnand_dev_ready;
	chip->select_chip = hifmc100_xnand_select_chip;
	chip->read_byte = hifmc100_xnand_read_byte;
	chip->read_buf = hifmc100_xnand_read_buf;
	chip->write_buf = hifmc100_xnand_write_buf;

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND)
		chip->is_spi_nand = 1;

	if (chip->is_spi_nand)
		flash_ids_hisi = spinand_flash_ids_hisi;
	else
		flash_ids_hisi = nand_flash_ids_hisi;

	for (spl_dev = flash_ids_hisi; spl_dev->name; spl_dev++) {
		if (spl_dev->id_len) {
			if (memcmp(xnand->id, spl_dev->id, spl_dev->id_len))
				continue;
		} else {
			if (spl_dev->ids.dev_id != devid)
				continue;
		}

		break;
	}

	if (!spl_dev->name)
		return -ENODEV;

	for (ix = 1; ix < HIFMC100_OP_CFG_NUM_CS; ix++) {
		memset(xnand->id, 0, sizeof(xnand->id));
		hifmc100_xnand_select_chip(xnand->mtd, ix);
		xnand->cmd_reset(xnand);
		xnand->cmd_readid(xnand);
		if ((xnand->id[0] != manfid) || (xnand->id[1] != devid))
			break;
	}

	chip->numchips = ix;

	nand_spl_ids_register();

	hinfc_nand_param_adjust = hifmc100_xnand_param_adjust;

	return 0;
}
/******************************************************************************/

static void hifmc100_spinand_setup(struct hifmc_xnand *xnand)
{
	xnand->regop->read_sr = hifmc100_spinand_read_sr;
	xnand->read_raw_bbm = hifmc100_spinand_read_raw_bbm;
	xnand->cmd_readid = hifmc100_spinand_cmd_readid;
	xnand->cmd_reset = hifmc100_spinand_cmd_reset;
	xnand->cmd_erase = hifmc100_spinand_cmd_erase;

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


	xnand->clk_rate = (ulong)(-1); /* max clock do read id, reset */
}
/******************************************************************************/

int hifmc100_xnand_probe(struct nand_chip *chip, int ifmode)
{
	struct hifmc_xnand *xnand;

	xnand = kmalloc(sizeof(struct hifmc_xnand), GFP_KERNEL);
	if (!xnand) {
		PR_OUT("%s failed to allocate hifmc_xnand structure.\n", DEVNAME);
		return -ENOMEM;
	}

	xnand->chip = chip;
	xnand->host = host;
	xnand->chip->priv = xnand;
	xnand->mtd->priv = xnand->chip;
	xnand->chip->IO_ADDR_R = xnand->chip->IO_ADDR_W = host->iobase;
	xnand->mtd->name = "hinand";

	xnand->ifmode = ifmode;
	xnand->regop->priv = host;
	xnand->regop->read_reg = hifmc100_read_reg;
	xnand->regop->write_reg = hifmc100_write_reg;
	xnand->regop->wait_device_ready = hifmc100_xnand_wait_device_ready;

	if (ifmode == HIFMC_IFMODE_NAND) {
		hifmc_write(host, 0xaaa, HIFMC100_PND_PWIDTH_CFG);

		host->nand = xnand;
		xnand->name = "nand";

		hifmc100_nand_setup(xnand);
		
	} else if (ifmode == HIFMC_IFMODE_SPINAND) {
		hifmc_write(host, 0x6F, HIFMC100_TIMING_SPI_CFG);

		host->spinand = xnand;
		xnand->name = "spinand";

		hifmc100_spinand_setup(xnand);
	} else
		panic(DEVNAME ": invalid ifmode:%d\n", ifmode);

	host->set_ifmode(host, ifmode);
	host->ifmode = -1;

	hifmc100_xnand_select_chip(xnand->mtd, 0);
	xnand->cmd_reset(xnand);
	xnand->cmd_readid(xnand);
	if (!xnand->id[0] 
		|| (xnand->id[0] == 0xff) 
		|| (xnand->id[0] == xnand->id[1])) {
		kfree(xnand);
		return -ENODEV;
	}

	if (hifmc100_xnand_probe_device(xnand)) {
		kfree(xnand);
		return -ENODEV;
	}

	host->ifmode = xnand->ifmode;

	xnand->ecc_strength = 
		hifmc100_ecc_strength_reg(host->reg.fmc_cfg_ecc_type, false);
	xnand->pagesize = hifmc100_pagesize_reg(host->reg.fmc_cfg_page_size, false);

	xnand->reg.fmc_cfg_ecc_type = host->reg.fmc_cfg_ecc_type;
	xnand->reg.fmc_cfg_page_size = host->reg.fmc_cfg_page_size;

	return 0;
}
/******************************************************************************/

void hifmc100_xnand_shutdown(void)
{
	/* nothing to do. */
}
/******************************************************************************/
int hifmc100_xnand_init(struct nand_chip *chip, int if_mode)
{
	int ret;

	if (hifmc100_host_init())
		return -ENODEV;

	ret = hifmc100_xnand_probe(chip, if_mode);
	if (ret) {
		printf("no found nand device.\n");
		return -ENODEV;
	}

	return 0;
}
