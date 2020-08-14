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

#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <match.h>
#include <platform.h>
#include <string.h>
#include <stdio.h>
#include <delay.h>
#include <cpuinfo.h>
#include <debug.h>
#include <module.h>
#include <mmu.h>

#include "hiflash.h"
#include "hifmc100_reg.h"
#include "hifmc100_host.h"
#include "../nand/nand_chip.h"
#include "../nand/nand_drv.h"
#include "../nand/nand_ctrl.h"

struct page_info {
	int page_addr;

	char *pagebuf;
	u32 dma_pagebuf;
	int pagesize;

	char *oobbuf;
	u32 dma_oobbuf;
	int oobsize;

	int status;
};

#define TO_XNAND_ADDR(_xnand) \
	((u64)(_xnand)->cmdfunc->page_addr << (_xnand)->chip->page_shift)

#define EPM(_oobbuf) (*(u16 *)((u8 *)_oobbuf + 30))
#define BBM(_oobbuf) (*(u8 *)_oobbuf)

/******************************************************************************/
extern struct hifmc_host host;
extern unsigned int cache_enabled;
extern nand_driver_init_fn xnand_driver_init;
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
/*****************************************************************************/

static unsigned int get_8bits(unsigned char byte)
{
	int ix = 0;
	int num = 0;

	if (byte == 0xFF) return 8;
	if (!byte) return 0;

	while (ix++ < 8) {
		if ((byte & 1))
			num++;
		byte = (byte >> 1);
	}
	return num;
}
/******************************************************************************/

static unsigned int get_16bits(unsigned short byte)
{
	int ix = 0;
	int num = 0;

	if (byte == 0xFFFF) return 16;
	if (!byte) return 0;

	while (ix++ < 16) {
		if ((byte & 1))
			num++;
		byte = (byte >> 1);
	}
	return num;
}
/*****************************************************************************/

static int hifmc100_xnand_wait_device_ready(struct flash_regop *regop)
{
	int status;
	u32 deadline;

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

	info.priv = drv->priv;
	info.cmd = SPINAND_OP_RDSR;
	info.nr_cmd = 1;
	info.addr_cycle = 1;
	info.buf = &status;
	info.sz_buf = 1;

	info.addrl = 0xA0;
	drv->read_reg(&info);

	info.addrl = 0xC0;
	drv->read_reg(&info);

	if (!(status & SPINAND_STATUS_OIP))
		retsr |= NAND_STATUS_READY;

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
/*****************************************************************************/

static void hifmc100_xnand_set_address(struct nand_dev_t *nand_dev, 
				       uint64 address, int only_row)
{
	
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	address >>= (uint64)nand_dev->pageshift;

	xnand->cmdfunc->page_addr = (uint32)(address & 0xFFFFFFFF);
}
/******************************************************************************/

static int hifmc100_xnand_dev_ready(struct hifmc_xnand *xnand)
{
	u8 status;
	int timeout = 0x10000;

	do {
		status = xnand->regop->read_sr(xnand->regop);
	} while (!(status & NAND_STATUS_READY) && timeout--);

	if (!timeout) {
		PR_OUT("Wait xnand dev ready timeout.\n");
		return -1;
	}

	return status;
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
	if (oob_bitsflip > xnand->ecc_strength && get_16bits(epm) > 12)
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

		if (oob_bitsflip >= 8 && xnand->ids.manuf == NAND_MFR_AMD) {
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
	if (get_8bits(bbm) < 6)
		return true;

	/* bbm != 0xFF && ecc fail && bits fliping <= 2 */
	return false;
}
/******************************************************************************/

static inline int hifmc100_xnand_dma_map(struct hifmc_xnand *xnand,
					 struct page_info *page, u32 dmaaddr_reg)
{
	struct hifmc_host *hifmc = xnand->host;

	page->dma_pagebuf = (u32)page->pagebuf;

	hifmc_write(hifmc, (u32)page->dma_pagebuf, dmaaddr_reg);

	if (xnand->pagesize >= _8K) {
		u32 phyaddr = (u32)page->dma_pagebuf + _4K;

		hifmc_write(hifmc, phyaddr, dmaaddr_reg + 0x4);

		if (xnand->pagesize >= _16K) {
			hifmc_write(hifmc, phyaddr + _4K, dmaaddr_reg + 0x8);
			hifmc_write(hifmc, phyaddr + _8K, dmaaddr_reg + 0xC);
		}
	}

	page->dma_oobbuf = (u32)page->oobbuf;

	hifmc_write(hifmc, (u32)page->dma_oobbuf, dmaaddr_reg + 0x10);

	return 0;
}
/******************************************************************************/

static int hifmc100_xnand_dma_transfer(struct hifmc_xnand *xnand, u32 todev)
{
	int ret;
	u32 regval;
	struct hifmc_host *hifmc = xnand->host;
	struct cmdfunc_data *cmdfunc = xnand->cmdfunc;
	struct hifmc_reg *reg;
	struct page_info page;

	if (todev)
		reg = &xnand->reg.write;
	else
		reg = &xnand->reg.read;

	page.pagebuf = xnand->pagebuf;
	page.pagesize = xnand->pagesize;
	page.oobbuf = xnand->oobbuf;
	page.oobsize = xnand->oobsize;

	ret = hifmc100_xnand_dma_map(xnand, &page, HIFMC100_DMA_SADDR_D0);
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

	return 0;
}
/******************************************************************************/

static int hifmc100_set_nand_fast_timing(struct hifmc_xnand *nand)
{
	u32 regval;
	struct hifmc_host *hifmc = nand->host;

	hifmc_write(hifmc, nand->reg.fmc_pnd_pwidth_cfg,
		HIFMC100_PND_PWIDTH_CFG);

	regval = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval |= HIFMC100_GLOBAL_CFG_EDO_MODE;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	hifmc_write(hifmc, HIFMC100_READ_TIMING_TUNE_VALUE,
		    HIFMC100_READ_TIMING_TUNE);

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
		hifmc100_xnand_dma_transfer(xnand, 0);
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
		printf("%s id: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02x\n",
			xnand->name, id[0], id[1], id[2], id[3], id[4]);
		memcpy(xnand->ids.id, id, sizeof(id));
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
		printf("%s id: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
			xnand->name, id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7]);
		memcpy(xnand->ids.id, id, sizeof(id));
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
	udelay(1);

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
/*****************************************************************************/

static void hifmc100_xnand_select_chip(struct hifmc_xnand *xnand, int chipselect)
{
	xnand->host->chipselect = chipselect;
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

	PR_BUG(DEVNAME ": invalid ecc strength or register. value:0x%08x\n", value);

	return 0;
}
/******************************************************************************/

static u32 hifmc100_pagesize_reg(u32 value, u32 _to_reg)
{
	int ix;
	u32 pagesize_reg[] = {
		_2K,  HIFMC100_CFG_PAGE_SIZE_2K,
		_4K,  HIFMC100_CFG_PAGE_SIZE_4K,
		_8K,  HIFMC100_CFG_PAGE_SIZE_8K,
		_16K, HIFMC100_CFG_PAGE_SIZE_16K,
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

	PR_BUG(DEVNAME ": invalid pagesize or register. value:0x%08x\n", value);

	return 0;
}
/******************************************************************************/

static void hifmc100_dump_bitflip(void *host, u8 ecc[16], int *max_bitsflip)
{
	u32 *e = (u32 *)ecc;
	struct hifmc_xnand *xnand = (struct hifmc_xnand *)host;
	struct hifmc_host *hifmc = xnand->host;

	e[0] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0);

	if (xnand->pagesize >= _8K) {
		e[1] = hifmc_read(hifmc, HIFMC100_FMC_ERR_NUM0_BUF0 + 0x4);

		if (xnand->pagesize >= _16K) {
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

static struct nand_ctrl_oob_t hifmc100_xnand_ecclayout = {
	2, 30
};
/******************************************************************************/

#define SET_CTRL_INFO(_pagesize, _oobsize, _ecc_strength, _sz_data, _sz_ecc, _threshold) { \
	.pagesize = (_pagesize), \
	.ecc = { .strength = (_ecc_strength), .step = _1K, }, \
	.section = { .sz_data = (_sz_data), .sz_ecc = (_sz_ecc), }, \
	.threshold = (_threshold), \
	.ecclayout = &hifmc100_xnand_ecclayout, \
	.dump = hifmc100_dump_bitflip, \
	.nr_ecc_sect = 0, \
	.oobsize = _oobsize, }

/******************************************************************************/

static struct nand_ctrl_info hifmc100_nand_ctrl_info[] = {
	SET_CTRL_INFO(_2K, 60,  8,  1040, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(_2K, 64,  16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(_2K, 116, 24, 1040, 42, 21),

	SET_CTRL_INFO(_4K, 88,  8,  1032, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(_4K, 128, 16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(_4K, 200, 24, 1032, 42, 21),

	SET_CTRL_INFO(_8K, 368, 24, 1028, 42, 21),
	SET_CTRL_INFO(_8K, 592, 40, 1028, 70, 34),
	SET_CTRL_INFO(_8K, 928, 64, 1028, 112, 58),

	SET_CTRL_INFO(_16K, 1152, 40, 1026, 70, 34),
	SET_CTRL_INFO(_16K, 1824, 64, 1026, 112, 58),

	{0},
};
/******************************************************************************/

static struct nand_ctrl_info hifmc100_spinand_ctrl_info[] = {
	SET_CTRL_INFO(_2K, 60,  8,  1040, 14, 7),
	SET_CTRL_INFO(_2K, 116, 24, 1040, 42, 21),

	SET_CTRL_INFO(_4K, 88,  8,  1032, 14, 7),
	SET_CTRL_INFO(_4K, 200, 24, 1032, 42, 21),

	{0},
};
/******************************************************************************/

static struct nand_ctrl_info hifmc100_auto_nand_ctrl_info[] = {
	SET_CTRL_INFO(_2K, 60,  8,  1040, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(_2K, 64,  16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(_2K, 116, 24, 1040, 42, 21),
	SET_CTRL_INFO(_2K, 132, 28, 1040, 50, 25),

	SET_CTRL_INFO(_4K, 88,  8,  1032, 14, 7),
#ifndef CONFIG_SUPPORT_YAFFS
	SET_CTRL_INFO(_4K, 128, 16, 1028, 28, 14),
#endif
	SET_CTRL_INFO(_4K, 200, 24, 1032, 42, 21),
	SET_CTRL_INFO(_4K, 232, 28, 1032, 50, 25),

	SET_CTRL_INFO(_8K, 368, 24, 1028, 42, 21),
	SET_CTRL_INFO(_8K, 432, 28, 1028, 50, 25),
	SET_CTRL_INFO(_8K, 592, 40, 1028, 70, 34),
	SET_CTRL_INFO(_8K, 928, 64, 1028, 112, 58),

	SET_CTRL_INFO(_16K, 1152, 40, 1026, 70, 34),
	SET_CTRL_INFO(_16K, 1824, 64, 1026, 112, 58),

	{0},
};
/******************************************************************************/

static int hifmc100_xnand_sanely_check(struct nand_chip_info_t *nand_info,
				       struct hifmc_xnand *xnand)
{
	struct nand_dev_t *nand_dev = xnand->nand_dev;

	if (nand_info->pagesize != xnand->pagesize) {
		unsigned int shift = 0;
		unsigned int writesize = nand_info->pagesize;
		while (writesize > xnand->pagesize) {
			writesize >>= 1;
			shift++;
		}
		nand_dev->chipsize = nand_info->chipsize >> shift;
		nand_dev->blocksize = nand_info->erasesize >> shift;
		nand_dev->pagesize = xnand->pagesize;
		printf("Nand divide into 1/%u\n", (1 << shift));
	}

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND)
		printf("maxclk:%luM ", xnand->clk_rate / 1000000);

	printf("\n");

	return 0;
}

/******************************************************************************/

static void hifmc100_spinand_set_xfer(struct hifmc_xnand *xnand)
{
	struct spinand_chip_xfer *xfer;
	struct hifmc_reg *read, *write;

	xfer = spinand_get_xfer(hifmc100_spinand_chip_xfer,
		xnand->ids.id, sizeof(xnand->ids.id));

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

struct nand_flash_special_dev *find_xnand_spl_dev(struct nand_dev_t *nand_dev)
{
	struct nand_flash_special_dev *spl_dev;
	struct nand_flash_special_dev *flash_ids_hisi;
	struct hifmc_host *host = (struct hifmc_host *)nand_dev->priv;
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	printf("Nand ID: 0x%02X 0x%02X 0x%02X 0x%02X"
		" 0x%02X 0x%02X 0x%02X 0x%02X\n",
		xnand->ids.id[0], xnand->ids.id[1], xnand->ids.id[2], xnand->ids.id[3],
		xnand->ids.id[4], xnand->ids.id[5], xnand->ids.id[6], xnand->ids.id[7]);

	if (nand_dev->is_spi_nand)
		flash_ids_hisi = spinand_flash_ids_hisi;

	else
		flash_ids_hisi = nand_flash_ids_hisi;

	for (spl_dev = flash_ids_hisi; spl_dev->name; spl_dev++) {
		if (spl_dev->id_len) {
			if (!memcmp(xnand->ids.id, spl_dev->ids.id, spl_dev->id_len)) {
				break;
			}
		} else {
			if (spl_dev->ids.dev_id == xnand->ids.id[1]) {
				if (spl_dev->pagesize) {
					spl_dev->oobsize = spl_dev->pagesize >> 5;
				} else {
					int extid = xnand->ids.id[3];
				
					spl_dev->pagesize = 1024 << (extid & 0x03);
					extid >>= 2;
					spl_dev->oobsize = (8 << (extid & 0x01)) * (spl_dev->pagesize >> 9);
					extid >>= 2;
					spl_dev->erasesize = (64 * 1024) << (extid & 0x03);
				}
				memcpy(spl_dev->ids.id, xnand->ids.id, sizeof(spl_dev->ids.id));

				break;
			}
		}
	}

	if (!spl_dev->name)
		return NULL;

	return spl_dev;
}
/*****************************************************************************/
/* used the best correct arithmetic. */
static struct nand_ctrl_info *
hifmc100_get_best_ecc(struct nand_ctrl_info *config, 
		      struct nand_chip_info_t *nand_info)
{
	struct nand_ctrl_info *best = NULL;

	for (; config->ecclayout; config++) {

		if (config->pagesize != nand_info->pagesize)
			continue;

		if (nand_info->oobsize < config->oobsize)
			continue;

		if (!best || (best->ecc.strength < config->ecc.strength))
			best = config;
	}

	if (!best)
		PR_BUG(ERSTR_DRIVER
		       "Driver does not support the pagesize(%d) "
		       "and oobsize(%d).\n",
		       nand_info->pagesize, nand_info->oobsize);

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
	case _2K:
		regval |= HIFMC100_CFG_PAGE_SIZE_2K;
		break;
	case _4K:
		regval |= HIFMC100_CFG_PAGE_SIZE_4K;
		break;
	case _8K:
		regval |= HIFMC100_CFG_PAGE_SIZE_8K;
		break;
	case _16K:
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

static int hifmc100_xnand_param_adjust(struct nand_dev_t *nand_dev)
{
	int bootmedia;
	char *start_type = "unknown";
	struct nand_ctrl_info *best = NULL;
	struct nand_ctrl_info *config = NULL;
	struct nand_chip_info_t *nand_info = nand_dev->nand_info;
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	bootmedia = get_bootmedia(NULL, NULL);

	if ((bootmedia == BOOT_MEDIA_NAND) || (bootmedia == BOOT_MEDIA_SPI_NAND)) {
		if (nand_dev->is_spi_nand)
			config = hifmc100_spinand_ctrl_info;
		else
			config = hifmc100_nand_ctrl_info;

		start_type = "HW-Auto";
		best = hifmc100_get_best_ecc(config, nand_info);
		if (IS_NANDC_CONFIG_DONE(host))
			hifmc100_check_config(host, nand_dev, best, 1);
		else if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
			host->flags &= ~NAND_RANDOMIZER;
	} else {
		if (nand_dev->is_spi_nand)
			config = hifmc100_spinand_ctrl_info;
		else
			config = hifmc100_auto_nand_ctrl_info;
		start_type = "Soft-Auto";
		best = hifmc100_get_best_ecc(config, nand_info);
		if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
			host->flags &= ~NAND_RANDOMIZER;
	}

	nand_dev->flags |= (IS_NANDC_HW_AUTO(host) |
			    IS_NANDC_CONFIG_DONE(host)); 

	if (!best)
		PR_BUG(ERSTR_HARDWARE
			"Please configure Nand Flash pagesize and ecctype!\n");

	if (best->ecc.strength)
		nand_dev->oobsize = best->oobsize;
	nand_dev->oobused = HIFMC100_NAND_OOBSIZE_FOR_YAFFS;
	nand_dev->layout = best->ecclayout;

	xnand->pagesize = best->pagesize;
	xnand->ecc_strength = best->ecc.strength;
	xnand->oobsize = nand_dev->oobsize;

	xnand->ctrl_info = nand_get_ctrl_info(config, xnand->pagesize, xnand->ecc_strength);
	if (!xnand->ctrl_info)
		PR_BUG(DEVNAME ": invalid pagesize(%d) or ecc_strength(%d).\n",
			xnand->pagesize, xnand->ecc_strength);

	if (nand_dev->pagesize > NAND_MAX_PAGESIZE ||
	    nand_dev->oobsize > NAND_MAX_OOBSIZE) {
		PR_BUG(ERSTR_DRIVER
			"Driver does not support this Nand Flash. "
			"Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
	}

	nand_dev->bootmsg= start_type;
	nand_dev->ecctype = nandecc_size2type(xnand->ecc_strength);

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND) {
		hifmc100_spinand_set_xfer(xnand);
	} else {
		xnand->reg.fmc_pnd_pwidth_cfg = nand_get_clock(
			hifmc100_nand_rwlatch,
			xnand->ids.id, sizeof(xnand->ids.id));

		hifmc100_set_nand_fast_timing(xnand);
	}

	if (nand_info->read_retry_type) {
		xnand->read_retry = nand_get_read_retry(nand_info->read_retry_type);
		if (!xnand->read_retry)
			PR_BUG(DEVNAME ": not support read retry type: %d\n", 
				nand_info->read_retry_type);
	}

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND) {
		int ix;

		xnand->spinand_drv = spinand_get_drvier(xnand->ids.id,
			sizeof(xnand->ids.id));

		for (ix = 0; ix < nand_dev->nr_chip; ix++) {
			hifmc100_xnand_select_chip(xnand, ix);
			xnand->spinand_drv->chip_prepare(xnand->regop);
		}
		hifmc100_xnand_select_chip(xnand, -1);
	}

	xnand->blocksize = nand_dev->blocksize;
	xnand->page_per_block = (xnand->blocksize / xnand->pagesize);

	xnand->reg.fmc_cfg_ecc_type = hifmc100_ecc_strength_reg(xnand->ecc_strength, true);
	xnand->reg.fmc_cfg_page_size = hifmc100_pagesize_reg(xnand->pagesize, true);

	if (xnand->ecc_strength && xnand->pagesize >= _8K)
		xnand->randomizer_en = true;
	else
		xnand->randomizer_en = false;

	/* One more address cycle for devices > 128MiB */
	if (nand_dev->chipsize > (128 << 20))
		xnand->erase_addr_cycle = 3;
	else
		xnand->erase_addr_cycle = 2;

	xnand->read_addr_cycle = xnand->erase_addr_cycle + 2;

	hifmc100_xnand_sanely_check(nand_info, xnand);

	hifmc100_set_param(xnand);

	xnand->oobsize = nand_dev->oobsize;

	/* must malloc pagebuf and oobbuf at the same time to ensure 
	 * oobbuf = pagebuf + oobsize.
	 * or nand dump oob will fail.
	 */
	xnand->pagebuf = malloc(nand_dev->pagesize + nand_dev->oobsize);
	if (!xnand->pagebuf) {
		PR_OUT("%s out of memory.\n", DEVNAME);
		return -EIO;
	}
	xnand->oobbuf = xnand->pagebuf + xnand->pagesize;
	memset(xnand->oobbuf, 0xFF, xnand->oobsize);

	nand_dev->bbm	  = (unsigned char *)xnand->oobbuf;
	nand_dev->pagebuf = xnand->pagebuf;
	nand_dev->oobbuf  = xnand->oobbuf;

	return 0;
}

/******************************************************************************/

static int
hifmc100_xnand_probe_device(struct nand_dev_t *nand_dev, struct hifmc_xnand *xnand)
{
	int ix;
	int manfid, devid;
	struct nand_flash_special_dev *flash_ids_hisi;
	struct nand_flash_special_dev *spl_dev;

	manfid = xnand->ids.id[0];
	devid = xnand->ids.id[1];

	if (xnand->ifmode == HIFMC_IFMODE_SPINAND)
		nand_dev->is_spi_nand = 1;

	if (nand_dev->is_spi_nand)
		flash_ids_hisi = spinand_flash_ids_hisi;
	else
		flash_ids_hisi = nand_flash_ids_hisi;

	for (spl_dev = flash_ids_hisi; spl_dev->name; spl_dev++) {
		if (spl_dev->id_len) {
			if (memcmp(xnand->ids.id, spl_dev->ids.id, spl_dev->id_len))
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
		memset(xnand->ids.id, 0, sizeof(xnand->ids.id));
		hifmc100_xnand_select_chip(xnand, ix);
		xnand->cmd_reset(xnand);
		xnand->cmd_readid(xnand);
		if ((xnand->ids.id[0] != manfid) || (xnand->ids.id[1] != devid))
			break;
	}

	nand_dev->nr_chip = ix;

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


	xnand->clk_rate = (u32)(-1); /* max clock do read id, reset */
}
/******************************************************************************/

int hifmc100_xnand_probe(struct nand_dev_t *nand_dev, int ifmode)
{
	struct hifmc_xnand *xnand;
	struct hifmc_host *hifmc_host = &host;

	xnand = malloc(sizeof(struct hifmc_xnand));
	if (!xnand) {
		PR_OUT("%s failed to allocate hifmc_xnand structure.\n", DEVNAME);
		return -ENOMEM;
	}
	memset(xnand, 0, sizeof(struct hifmc_xnand));
	xnand->nand_dev = nand_dev;

	xnand->host = hifmc_host;
	xnand->ifmode = ifmode;
	xnand->regop->priv = hifmc_host;
	xnand->regop->read_reg = hifmc100_read_reg;
	xnand->regop->write_reg = hifmc100_write_reg;
	xnand->regop->wait_device_ready = hifmc100_xnand_wait_device_ready;

	if (ifmode == HIFMC_IFMODE_NAND) {
		hifmc_write(hifmc_host, 0xaaa, HIFMC100_PND_PWIDTH_CFG);

		hifmc_host->nand = xnand;
		xnand->name = "nand";

		hifmc100_nand_setup(xnand);
		
	} else if (ifmode == HIFMC_IFMODE_SPINAND) {
		hifmc_write(hifmc_host, 0x6F, HIFMC100_TIMING_SPI_CFG);

		hifmc_host->spinand = xnand;
		xnand->name = "spinand";

		hifmc100_spinand_setup(xnand);
	} else
		PR_BUG(DEVNAME ": invalid ifmode:%d\n", ifmode);

	hifmc_host->set_ifmode(hifmc_host, ifmode);
	hifmc_host->ifmode = -1;

	hifmc100_xnand_select_chip(xnand, 0);
	xnand->cmd_readid(xnand);
	if (!xnand->ids.id[0] 
		|| (xnand->ids.id[0] == 0xff) 
		|| (xnand->ids.id[0] == xnand->ids.id[1])) {
		free(xnand);
		return -ENODEV;
	}

	if (hifmc100_xnand_probe_device(nand_dev, xnand)) {
		free(xnand);
		return -ENODEV;
	}

	hifmc_host->ifmode = xnand->ifmode;

	xnand->ecc_strength = 
		hifmc100_ecc_strength_reg(hifmc_host->reg.fmc_cfg_ecc_type, false);
	xnand->pagesize = hifmc100_pagesize_reg(hifmc_host->reg.fmc_cfg_page_size, false);

	xnand->reg.fmc_cfg_ecc_type = hifmc_host->reg.fmc_cfg_ecc_type;
	xnand->reg.fmc_cfg_page_size = hifmc_host->reg.fmc_cfg_page_size;

	return 0;
}
/*****************************************************************************/

static int hifmc100_xnand_read_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));
	host->set_ifmode(host, xnand->ifmode);
	host->set_clock(host, xnand->clk_rate, ENABLE);

	xnand->is_empty_page = 0;
	xnand->is_bad_block = 0;
	xnand->is_uc_err = 0;

	if (address >= nand_dev->chipsize) {
		hifmc100_xnand_select_chip(xnand, address >> nand_dev->chipshift);
		address &= nand_dev->chipmask;
	} else
		hifmc100_xnand_select_chip(xnand, 0);

	hifmc100_xnand_set_address(nand_dev, address, FALSE);

	hifmc100_xnand_dma_transfer(xnand, 0);

	hifmc100_xnand_fix_read(xnand);

	hifmc100_xnand_dev_ready(xnand);

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *     0 - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hifmc100_xnand_write_page(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret;
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	host->set_ifmode(host, xnand->ifmode);
	host->set_clock(host, xnand->clk_rate, ENABLE);

	xnand->is_empty_page = 0;
	xnand->is_bad_block = 0;
	xnand->is_uc_err = 0;

	/* bbm mark must be set to 0 */
	EPM(xnand->oobbuf) = 0;

	if (address >= nand_dev->chipsize) {
		hifmc100_xnand_select_chip(xnand, address >> nand_dev->chipshift);
		address &= nand_dev->chipmask;
	} else
		hifmc100_xnand_select_chip(xnand, 0);

	hifmc100_xnand_set_address(nand_dev, address, FALSE);
	hifmc100_xnand_dma_transfer(xnand, 1);

	ret = hifmc100_xnand_dev_ready(xnand);
	if (((nand_dev->is_spi_nand) && (ret & SPINAND_STATUS_P_FAIL))
	    || (!nand_dev->is_spi_nand && (ret & NAND_STATUS_FAIL)))
		return -1;

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hifmc100_xnand_erase_block(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret;
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->blockmask));

	host->set_ifmode(host, xnand->ifmode);
	host->set_clock(host, xnand->clk_rate, ENABLE);

	xnand->is_empty_page = 0;
	xnand->is_bad_block = 0;
	xnand->is_uc_err = 0;

	if (address >= nand_dev->chipsize) {
		hifmc100_xnand_select_chip(xnand, address >> nand_dev->chipshift);
		address &= nand_dev->chipmask;
	} else
		hifmc100_xnand_select_chip(xnand, 0);

	hifmc100_xnand_set_address(nand_dev, address, TRUE);
	xnand->cmd_erase(xnand);

	ret = hifmc100_xnand_dev_ready(xnand);
	if (((nand_dev->is_spi_nand) && (ret & SPINAND_STATUS_E_FAIL))
	    || (!nand_dev->is_spi_nand && (ret & NAND_STATUS_FAIL)))
		return -1;

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hifmc100_xnand_reset(struct nand_dev_t *nand_dev, int chipselect)
{
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	host->set_ifmode(host, xnand->ifmode);
	host->set_clock(host, xnand->clk_rate, ENABLE);

	hifmc100_xnand_select_chip(xnand, chipselect);
	xnand->cmd_reset(xnand);

	return hifmc100_xnand_wait_device_ready(xnand->regop) ? -1 : 0;
}
/*****************************************************************************/

static int hifmc100_xnand_read_id(struct nand_dev_t *nand_dev, int chipselect,
				  char *buf, int buflen)
{
	struct hifmc_host *host = (struct hifmc_host *)(nand_dev->priv);
	struct hifmc_xnand *xnand = nand_dev->is_spi_nand ? host->spinand : host->nand;

	host->set_ifmode(host, xnand->ifmode);
	host->set_clock(host, xnand->clk_rate, ENABLE);

	hifmc100_xnand_select_chip(xnand, chipselect);
	xnand->cmd_readid(xnand);

	buflen = min(buflen, FLASH_ID_LEN);
	memcpy(buf, xnand->ids.id, buflen);

	return 0;
}
/*****************************************************************************/

static struct nand_dev_t hifmc100_xnand_dev = {
	.bootmsg = "Unknown",
	.read_page = hifmc100_xnand_read_page,
	.write_page = hifmc100_xnand_write_page,
	.erase_block = hifmc100_xnand_erase_block,
	.reset = hifmc100_xnand_reset,
	.read_id = hifmc100_xnand_read_id,
	.adjust_param = hifmc100_xnand_param_adjust,
	.priv = (void *)&host,
};
/******************************************************************************/

static struct nand_dev_t *hifmc100_xnand_init(void)
{
	unsigned int cache_enabled_old = cache_enabled;
	static struct nand_dev_t *nand_dev = NULL;
	int ret;
	
	if (nand_dev)
		return nand_dev;

	/* first disable mmu cache. */
	if (cache_enabled)
		mmu_cache_disable();

	if (hifmc100_host_init()) {
		printf("failed to init hifmc100 host.\n");
		goto init_failed;
	}

	nand_dev = &hifmc100_xnand_dev;

#ifdef CONFIG_GENERIC_HIFMC100_NAND
	ret = hifmc100_xnand_probe(nand_dev, HIFMC_IFMODE_NAND);
	if (!ret)
		return nand_dev;
#endif

#ifdef CONFIG_GENERIC_HIFMC100_SPI_NAND
	ret = hifmc100_xnand_probe(nand_dev, HIFMC_IFMODE_SPINAND);
	if (!ret)
		return nand_dev;
#endif

init_failed:
	/* if no xnand probed, enable cache again. */
	if (cache_enabled_old)
		mmu_cache_enable();
	return NULL;
}
/******************************************************************************/

void hifmc100_xnand_shutdown(void)
{
	/* nothing to do. */
}
/*****************************************************************************/

static void hifmc100_module_exit(void *ptr)
{
//	struct hifmc_host *host = (struct hifmc_host *)ptr;

	hifmc100_xnand_shutdown();
}
/*****************************************************************************/

static void hifmc100_module_init(void *ptr)
{
	xnand_driver_init = hifmc100_xnand_init;
}
/*****************************************************************************/

static struct module_t module_hifmc100_xnand = {
	.priority = MODULE_PRI_DRV,
	.init = hifmc100_module_init,
	.exit = hifmc100_module_exit,
};
MODULE(module_hifmc100_xnand);
