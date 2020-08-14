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

#define DEVNAME  "hifmc100"
#define pr_fmt(fmt) DEVNAME": " fmt

#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <match.h>
#include <platform.h>
#include <flash_gen.h>
#include <string.h>
#include <delay.h>

#include "hifmc100_reg.h"
#include "hifmc100_host.h"
#include "../nand/nand_chip.h"
#include "../nand/nand_drv.h"

/******************************************************************************/

struct hifmc_host host;
/******************************************************************************/
#ifdef DEBUG_HIFMC100
static void nand_register_dump(void)
{
	int ix;
	u32 regbase = CONFIG_HIFMC100_REG_BASE_ADDRESS;

	printf("Register dump:");
	for (ix = 0; ix <= 0xDC; ix += 0x04) {
		if (!(ix & 0x0F))
			printf("\n0x%08X: ", (regbase + ix));
		printf("%08X ", readl(regbase + ix));
	}
	printf("\n");
}
#endif
/******************************************************************************/

static void hifmc100_set_ifmode(struct hifmc_host *host, int ifmode)
{
	u32 regval;

	if (host->ifmode == HIFMC_IFMODE_INVALID /*|| host->ifmode == ifmode*/)
		return;

	regval = hifmc_read(host, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_FLASH_SEL_MASK;

	switch (ifmode) {
	case HIFMC_IFMODE_SPINOR:
		regval |= HIFMC100_CFG_FLASH_SEL_SPINOR;
		/* dynamic clock, in every read/write/erase operation */
		break;
	case HIFMC_IFMODE_NAND:
		regval |= HIFMC100_CFG_FLASH_SEL_NAND;
		host->set_clock(host, host->nand->clk_rate, ENABLE);
		break;
	case HIFMC_IFMODE_SPINAND:
		regval |= HIFMC100_CFG_FLASH_SEL_SPINAND;
		host->set_clock(host, host->spinand->clk_rate, ENABLE);
		break;
	default:
		PR_BUG("invalide ifmode.\n");
	}

	/* change to normal mode. */
	regval |= HIFMC100_CFG_OP_MODE_NORMAL;

	hifmc_write(host, regval, HIFMC100_CFG);

	/* delay 1ms, change ifmode need delay. */
	udelay(1000);
}
/******************************************************************************/

static void hifmc100_irq_enable(struct hifmc_host *host)
{
	/* clear all interrupt */
	hifmc_write(host, HIFMC100_INT_CLR_ALL, HIFMC100_INT_CLR);

	/* only enable dma done interrupt. */
	hifmc_write(host, HIFMC100_INT_EN_OP_DONE, HIFMC100_INT_EN);
}

/******************************************************************************/

static int hifmc100_wait_host_ready(struct hifmc_host *host)
{
	u32 regval;
	uint32 deadline = 10000000;

	do {
		regval = hifmc_read(host, HIFMC100_OP);
		if (!(regval & HIFMC100_OP_REG_OP_START))
			return 0;
	} while (deadline--);

	PR_BUG("wait host ready timeout, hifmc100 register:\n");

	return -1;
}
/******************************************************************************/

int hifmc100_write_reg(struct flash_regop_info *info)
{
	u32 regval;
	u32 cfg_val;
	u32 global_cfg_val;
	struct hifmc_host *hifmc = (struct hifmc_host *)info->priv;

	if ((hifmc->chipselect != 0) && (hifmc->chipselect != 1))
		PR_BUG("invalid chipselect.\n");

	/* set chip select and address number */
	regval = 0;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	regval |= HIFMC100_OP_CFG_ADDR_NUM(info->addr_cycle);
	regval |= HIFMC100_OP_CFG_DUMMY_NUM(info->dummy);
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* disable ecc */
	regval = cfg_val = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, 0, HIFMC100_OP_PARA);

	/* disable randomizer */
	regval = global_cfg_val = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set command */
	hifmc_write(hifmc, (u32)info->cmd, HIFMC100_CMD);

	/* set address */
	if (info->addr_cycle > 0) {
		if (info->addr_cycle > 4)
			hifmc_write(hifmc, info->addrh, HIFMC100_ADDRH);
		else
			hifmc_write(hifmc, 0, HIFMC100_ADDRH);

		hifmc_write(hifmc, info->addrl, HIFMC100_ADDRL);
	} else {
		/* no address parameter */
		hifmc_write(hifmc, 0, HIFMC100_ADDRH);
		hifmc_write(hifmc, 0, HIFMC100_ADDRL);
	}

	regval = HIFMC100_OP_REG_OP_START;

	if (info->dummy)
		regval |= HIFMC100_OP_DUMMY_EN;

	/* only nand have wait ready feature, spinand/spinor not support */
	if (info->wait_ready)
		regval |= HIFMC100_OP_WAIT_READY_EN;

	if (info->nr_cmd >= 1) {
		regval |= HIFMC100_OP_CMD1_EN;
		if (info->nr_cmd >= 2)
			regval |= HIFMC100_OP_CMD2_EN;
	}

	/* set data */
	if (info->sz_buf) {
		if (info->sz_buf > hifmc->sz_iobase)
			info->sz_buf = hifmc->sz_iobase;
		regval |= HIFMC100_OP_WRITE_DATA_EN;
		if (info->buf)
			memcpy(hifmc->iobase, info->buf, info->sz_buf);
	}

	hifmc_write(hifmc, HIFMC100_DATA_NUM_CNT(info->sz_buf),
		HIFMC100_DATA_NUM);

	if (info->addr_cycle)
		regval |= HIFMC100_OP_ADDR_EN;

	hifmc_write(hifmc, regval, HIFMC100_OP);

	hifmc100_wait_host_ready(hifmc);

	/* restore ecc configure for debug */
	hifmc_write(hifmc, cfg_val, HIFMC100_CFG);
	/* restore randomizer config for debug */
	hifmc_write(hifmc, global_cfg_val, HIFMC100_GLOBAL_CFG);

	return 0;
}
/******************************************************************************/

int hifmc100_read_reg(struct flash_regop_info *info)
{
	u32 regval;
	u32 cfg_val;
	u32 global_cfg_val;
	struct hifmc_host *hifmc = (struct hifmc_host *)info->priv;

	if ((hifmc->chipselect != 0) && (hifmc->chipselect != 1))
		PR_BUG("invalid chipselect.\n");

	/* set chip select, address number, dummy */
	regval = 0;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	regval |= HIFMC100_OP_CFG_ADDR_NUM(info->addr_cycle);
	regval |= HIFMC100_OP_CFG_DUMMY_NUM(info->dummy);
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* disable ecc */
	regval = cfg_val = hifmc_read(hifmc, HIFMC100_CFG);
	regval &= ~HIFMC100_CFG_ECC_TYPE_MASK;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	hifmc_write(hifmc, 0, HIFMC100_OP_PARA);

	/* disable randomizer */
	regval = global_cfg_val = hifmc_read(hifmc, HIFMC100_GLOBAL_CFG);
	regval &= ~HIFMC100_GLOBAL_CFG_RANDOMIZER_EN;
	hifmc_write(hifmc, regval, HIFMC100_GLOBAL_CFG);

	/* set command */
	hifmc_write(hifmc, (u32)info->cmd, HIFMC100_CMD);

	/* set address */
	if (info->addr_cycle > 0) {
		if (info->addr_cycle > 4)
			hifmc_write(hifmc, info->addrh, HIFMC100_ADDRH);
		else
			hifmc_write(hifmc, 0, HIFMC100_ADDRH);

		hifmc_write(hifmc, info->addrl, HIFMC100_ADDRL);
	} else {
		hifmc_write(hifmc, 0, HIFMC100_ADDRH);
		hifmc_write(hifmc, 0, HIFMC100_ADDRL);
	}

	/* set data length */
	if (info->sz_buf > hifmc->sz_iobase)
		info->sz_buf = hifmc->sz_iobase;

	hifmc_write(hifmc, HIFMC100_DATA_NUM_CNT(info->sz_buf),
		HIFMC100_DATA_NUM);

	regval = HIFMC100_OP_REG_OP_START;

	if (info->dummy)
		regval |= HIFMC100_OP_DUMMY_EN;

	/* only nand have wait ready feature, spinand/spinor not support */
	if (info->wait_ready)
		regval |= HIFMC100_OP_WAIT_READY_EN;

	if (info->nr_cmd >= 1) {
		regval |= HIFMC100_OP_CMD1_EN;
		if (info->nr_cmd >= 2)
			regval |= HIFMC100_OP_CMD2_EN;
	}

	if (info->sz_buf)
		regval |= HIFMC100_OP_READ_DATA_EN;

	if (info->addr_cycle)
		regval |= HIFMC100_OP_ADDR_EN;

	hifmc_write(hifmc, regval, HIFMC100_OP);

	hifmc100_wait_host_ready(hifmc);

	if (info->sz_buf && info->buf)
		memcpy(info->buf, hifmc->iobase, info->sz_buf);

	/* restore ecc configure for debug */
	hifmc_write(hifmc, cfg_val, HIFMC100_CFG);
	/* restore randomizer config for debug */
	hifmc_write(hifmc, global_cfg_val, HIFMC100_GLOBAL_CFG);

	return 0;
}
/******************************************************************************/

int hifmc100_host_init(void)
{
	u32 regval;
	struct hifmc_host *hifmc_host = &host;

	if (host.nand || host.spinand || host.spinor)
		return 0;

	memset(hifmc_host, 0x00, sizeof(struct hifmc_host));

	hifmc_host->regbase = (void *)CONFIG_HIFMC100_REG_BASE_ADDRESS;
	hifmc_host->iobase = (void *)CONFIG_HIFMC100_BUFFER_BASE_ADDRESS;
	hifmc_host->sz_iobase = CONFIG_HIFMC100_BUFFER_SIZE;

	hifmc100_set_system_clock(hifmc_host, 0, ENABLE);

	regval = hifmc_read(hifmc_host, HIFMC100_VERSION);
	if (regval != HIFMC100_VERSION_VALUE)
		return -ENODEV;

	printf("Found flash memory controller hifmc100.\n");

	/* disable all interrupt */
	regval = hifmc_read(hifmc_host, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_ALL;
	hifmc_write(hifmc_host, regval, HIFMC100_INT_EN);
#if 0
	/* enable low power control. */
	regval = hifmc_read(host, HIFMC100_LP_CTRL);
	regval |= HIFMC100_LP_CTRL_CLK_GATE_EN;
	hifmc_write(host, regval, HIFMC100_LP_CTRL);
#endif
	/* clean all interrupt */
	hifmc_write(hifmc_host, HIFMC100_INT_CLR_ALL, HIFMC100_INT_CLR);

	/* configure dma burst width */
	hifmc_write(hifmc_host, 0xF, HIFMC100_DMA_AHB_CTRL);

	regval = hifmc_read(hifmc_host, HIFMC100_GLOBAL_CFG);
	if (regval & HIFMC100_GLOBAL_CFG_RANDOMIZER_EN)
		hifmc_host->flags |= NAND_RANDOMIZER;

	/*
	* check if start from nand.
	* This register REG_SC_GEN15 is set in start.S
	* When start in NAND (Auto), the ECC/PAGESIZE driver don't detect.
	*/
	regval = readl(REG_BASE_SCTL + REG_SC_GEN15);
	if (regval == BOOT_FROM_NAND || regval == BOOT_FROM_SPI_NAND)
		hifmc_host->flags |= NANDC_CONFIG_DONE;

	hifmc_write(hifmc_host, 0, HIFMC100_DMA_SADDR_OOB);
	hifmc_write(hifmc_host, 0, HIFMC100_DMA_SADDR_D0);
	hifmc_write(hifmc_host, 0, HIFMC100_DMA_SADDR_D1);
	hifmc_write(hifmc_host, 0, HIFMC100_DMA_SADDR_D2);
	hifmc_write(hifmc_host, 0, HIFMC100_DMA_SADDR_D3);

	hifmc_host->ifmode = -1;
	hifmc_host->set_ifmode = hifmc100_set_ifmode;
	hifmc_host->irq_enable = hifmc100_irq_enable;
	hifmc_host->wait_host_ready = hifmc100_wait_host_ready;
	hifmc_host->set_clock       = hifmc100_set_system_clock;

	/*
	 * get ecctype and pagesize from controller,
	 * controller should not reset after boot.
	 */
	regval = hifmc_read(hifmc_host, HIFMC100_CFG);
	hifmc_host->reg.fmc_cfg_ecc_type = HIFMC100_CFG_ECC_TYPE_MASK & regval;
	hifmc_host->reg.fmc_cfg_page_size = HIFMC100_CFG_PAGE_SIZE_MASK & regval;

	return 0;
}
