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
#define DEVNAME "hifmc100: "
#define pr_fmt(fmt) "spinor: " fmt

#include <malloc.h>
#include <asm/io.h>
#include <errno.h>
#include <platform.h>
#include <delay.h>
#include <string.h>
#include <bits.h>
#include <module.h>
#include <mmu.h>

#include "hifmc100_reg.h"
#include "hifmc100_host.h"
#include "../spiflash/sf_drv.h"

/******************************************************************************/
extern struct hifmc_host host;
extern unsigned int cache_enabled;
static struct spiflash_dev_t *__spiflash = NULL;

/******************************************************************************/


static int hifmc100_spinor_read_sr(struct flash_regop *regop)
{
	u8 status;
	struct flash_regop_info info = {0};

	info.priv = regop->priv;
	info.cmd = SPINOR_OP_RDSR;
	info.nr_cmd = 1;
	info.buf = &status;
	info.sz_buf = 1;
	regop->read_reg(&info);

	return status;
}
/******************************************************************************/

static int hifmc100_spinor_wait_device_ready(struct flash_regop *regop)
{
	u32 regval = 0;
	unsigned int timeout = 0;

	do {
		regval = regop->read_sr(regop);
		if (!(regval & SPINOR_STATUS_WIP))
			return 0;

		udelay(1);
	} while (timeout++ < (40 << 20));

	PR_OUT("wait spi-nor chip ready timeout.\n");

	return 1;
}
/******************************************************************************/

static void hifmc100_spinor_select_chip(struct hifmc_spinor *spinor, int chipselect)
{
	struct hifmc_host *hifmc = spinor->host;
	hifmc->chipselect = spinor->chipselect[chipselect];
}
/******************************************************************************/

static int hifmc100_spinor_erase(struct spiflash_dev_t *flash, u32 offset, size_t len)
{
	int nrchip = 0;
	struct hifmc_host *hifmc = SPIFLASH_TO_HOST(flash);
	struct flash_regop_info info = {0};
	struct hifmc_spinor *spinor = hifmc->spinor;
	struct flash_regop *regop = spinor->regop;
	struct hifmc_reg *erase = &spinor->xfer.erase;

	if (offset & (spinor->erasesize - 1)) {
		PR_OUT("erase address(0x%08x) not block aligned.\n", offset);
		return -EINVAL;
	}

	if (len & (spinor->erasesize - 1)) {
		PR_OUT("erase len(0x%08x) not block aligned.\n", offset);
		return -EINVAL;
	}

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINOR);
	hifmc->set_clock(hifmc, erase->clock, ENABLE);

	nrchip = (int)(offset >> spinor->chipsize_shift);

	hifmc100_spinor_select_chip(spinor, nrchip);

	offset = offset & spinor->chipsize_mask;

	info.priv = regop->priv;
	info.cmd = erase->cmd;
	info.nr_cmd = 1;
	info.addr_cycle = spinor->addr_cycle;

	while (len > 0) {
		regop->wait_device_ready(regop);
		spinor->drv->write_enable(regop);

		info.addrl = offset;
		regop->read_reg(&info);

		offset += spinor->erasesize;
		len -= spinor->erasesize;

		if (offset > spinor->chipsize) {
			offset -= spinor->chipsize;
			hifmc100_spinor_select_chip(spinor, ++nrchip);
		}
	}

	return 0;
}
/******************************************************************************/

static void hifmc100_spinor_dma_transfer(struct hifmc_spinor *spinor,
					 struct hifmc_reg *xfer, u32 offset,
					 int size)
{
	u32 regval;
	struct hifmc_host *hifmc = spinor->host;

	/* set dummy */
	regval = 0;
	if (xfer->dummy)
		regval |= HIFMC100_OP_DUMMY_EN;
	hifmc_write(hifmc, regval, HIFMC100_OP);

	hifmc_write(hifmc, (u32)spinor->dma_paddr, HIFMC100_DMA_SADDR_D0);
	hifmc_write(hifmc, HIFMC100_DMA_LEN_MASK(size), HIFMC100_DMA_LEN);

	/* set 3/4 bytes address mode */
	regval = hifmc_read(hifmc, HIFMC100_CFG);
	if (spinor->addr_cycle >= 4)
		regval |= HIFMC100_CFG_SPI_NOR_ADDR_MODE_4B;
	else
		regval &= ~HIFMC100_CFG_SPI_NOR_ADDR_MODE_4B;
	hifmc_write(hifmc, regval, HIFMC100_CFG);

	/* set address cycle and chip select */
	regval = xfer->op_cfg;
	if (hifmc->chipselect)
		regval |= HIFMC100_OP_CFG_FM_CS;
	hifmc_write(hifmc, regval, HIFMC100_OP_CFG);

	/* set address */
	hifmc_write(hifmc, 0, HIFMC100_ADDRH);
	hifmc_write(hifmc, offset, HIFMC100_ADDRL);

	hifmc->irq_enable(hifmc);

	hifmc_write(hifmc, xfer->op_ctrl, HIFMC100_OP_CTRL);

	FMC_DMA_WAIT_INT_FINISH(hifmc);

	/* disable dma done interrupt */
	regval = hifmc_read(hifmc, HIFMC100_INT_EN);
	regval &= ~HIFMC100_INT_EN_OP_DONE;
	hifmc_write(hifmc, regval, HIFMC100_INT_EN);
}
/******************************************************************************/

static int hifmc100_spinor_dma_read(struct spiflash_dev_t *flash, u32 offset,
				    size_t len, void *buf)
{
	int size;
	int nrchip = 0;
	u8 *ptr = (u8 *)buf;
	struct hifmc_host *hifmc = SPIFLASH_TO_HOST(flash);
	struct hifmc_spinor *spinor = hifmc->spinor;
	struct hifmc_reg *read = &spinor->xfer.read;

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINOR);
	hifmc->set_clock(hifmc, read->clock, ENABLE);

	nrchip = (int)(offset >> spinor->chipsize_shift);

	hifmc100_spinor_select_chip(spinor, nrchip);
	spinor->regop->wait_device_ready(spinor->regop);

	offset = offset & spinor->chipsize_mask;

	while (len > 0) {
		if (offset & HIFMC100_SPINOR_DMA_SIZE_MASK) {
			size = HIFMC100_SPINOR_DMA_SIZE -
				(offset & HIFMC100_SPINOR_DMA_SIZE_MASK);
			if (size > len)
				size = len;
		} else {
			if (len > HIFMC100_SPINOR_DMA_SIZE)
				size = HIFMC100_SPINOR_DMA_SIZE;
			else
				size = len;
		}

		hifmc100_spinor_dma_transfer(spinor, read, offset, size);

		memcpy(ptr, spinor->dma_vaddr, size);

		offset += size;
		ptr += size;
		len -= size;

		if (offset > spinor->chipsize) {
			offset -= spinor->chipsize;

			hifmc100_spinor_select_chip(spinor, ++nrchip);
			spinor->regop->wait_device_ready(spinor->regop);
		}
	}

	return 0;
}
/******************************************************************************/

static int hifmc100_spinor_dma_write(struct spiflash_dev_t *flash, u32 offset,
				     size_t len, const void *buf)
{
	int size;
	int nrchip = 0;
	u8 *ptr = (u8 *)buf;
	struct hifmc_host *hifmc = SPIFLASH_TO_HOST(flash);
	struct hifmc_spinor *spinor = hifmc->spinor;
	struct hifmc_reg *write = &spinor->xfer.write;

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINOR);
	hifmc->set_clock(hifmc, write->clock, ENABLE);

	nrchip = (int)(offset >> spinor->chipsize_shift);

	hifmc100_spinor_select_chip(spinor, nrchip);
	spinor->regop->wait_device_ready(spinor->regop);
	spinor->drv->write_enable(spinor->regop);

	offset = offset & spinor->chipsize_mask;

	while (len > 0) {
		if (offset & HIFMC100_SPINOR_DMA_SIZE_MASK) {
			size = HIFMC100_SPINOR_DMA_SIZE -
				(offset & HIFMC100_SPINOR_DMA_SIZE_MASK);
			if (size > len)
				size = len;
		} else {
			if (len > HIFMC100_SPINOR_DMA_SIZE)
				size = HIFMC100_SPINOR_DMA_SIZE;
			else
				size = len;
		}

		memcpy(spinor->dma_vaddr, ptr, size);

		hifmc100_spinor_dma_transfer(spinor, write, offset, size);

		offset += size;
		ptr += size;
		len -= size;

		if (offset > spinor->chipsize) {
			offset -= spinor->chipsize;

			hifmc100_spinor_select_chip(spinor, ++nrchip);
			spinor->regop->wait_device_ready(spinor->regop);
			spinor->drv->write_enable(spinor->regop);
		}
	}

	return 0;
}
/******************************************************************************/

static u32 hifmc100_spinor_op_iftype(u32 op)
{
	int ix;
	u32 op_iftype[] = {
		SPINOR_OP_READ,      HIFMC100_OP_CFG_MEM_IFTYPE_STD,
		SPINOR_OP_READ_FAST, HIFMC100_OP_CFG_MEM_IFTYPE_STD,
		SPINOR_OP_PP,        HIFMC100_OP_CFG_MEM_IFTYPE_STD,
		SPINOR_OP_READ_DUAL, HIFMC100_OP_CFG_MEM_IFTYPE_DUAL,
		SPINOR_OP_READ_QUAD, HIFMC100_OP_CFG_MEM_IFTYPE_QUAD,
		SPINOR_OP_PP_QUAD,   HIFMC100_OP_CFG_MEM_IFTYPE_QUAD,
	};

	for (ix = 0; ix < ARRAY_SIZE(op_iftype); ix += 2) {
		if (op_iftype[ix] == op)
			return op_iftype[ix+1];
	}

	PR_BUG(DEVNAME "invalid spi-nor op cmd: 0x%02x\n", op);

	return 0;
}
/******************************************************************************/

static void hifmc100_spinor_setup(struct hifmc_spinor *spinor,
				  struct spinor_chip_xfer *chip_xfer)
{
	struct hifmc_reg *reg;
	struct spiflash_xfer *xfer;

	/* setup erase config */
	xfer = chip_xfer->erase;
	reg = &spinor->xfer.erase;

	spinor->erasesize = xfer->size;
	reg->cmd = xfer->cmd;
	reg->clock = xfer->clock * 1000000;
	reg->dummy = xfer->dummy;

	/* setup read config */
	xfer = chip_xfer->read;
	reg = &spinor->xfer.read;

	reg->clock = xfer->clock * 1000000;
	reg->cmd = xfer->cmd;
	reg->dummy = xfer->dummy;

	reg->op_cfg = HIFMC100_OP_CFG_ADDR_NUM(spinor->addr_cycle) |
		HIFMC100_OP_CFG_DUMMY_NUM(reg->dummy) |
		hifmc100_spinor_op_iftype(xfer->cmd);

	reg->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
		HIFMC100_OP_CTRL_RD_OPCODE(xfer->cmd);

	/* setup write config */
	xfer = chip_xfer->write;
	reg = &spinor->xfer.write;

	reg->clock = xfer->clock * 1000000;
	reg->cmd = xfer->cmd;
	reg->dummy = xfer->dummy;

	reg->op_cfg = HIFMC100_OP_CFG_ADDR_NUM(spinor->addr_cycle) |
		HIFMC100_OP_CFG_DUMMY_NUM(reg->dummy) |
		hifmc100_spinor_op_iftype(xfer->cmd);

	reg->op_ctrl = HIFMC100_OP_CTRL_DMA_OP_READY |
		HIFMC100_OP_CTRL_RW_OP_WRITE |
		HIFMC100_OP_CTRL_WR_OPCODE(xfer->cmd);
}
/******************************************************************************/

static void hifmc100_spinor_chip_init(struct hifmc_spinor *spinor)
{
	int nrchip;

	for (nrchip = 0; nrchip < spinor->nr_chips; nrchip++) {
		hifmc100_spinor_select_chip(spinor, nrchip);

		if (spinor->drv->quad_enable(spinor->regop))
			printf("quad enable fail!\n");

		if (spinor->addr_cycle >= 4)
			spinor->drv->entry_4addr(spinor->regop);
	}
}
/******************************************************************************/

static int hifmc100_spinor_probe_device(struct hifmc_spinor *spinor)
{
	int nrchip;
	u8 id[FLASH_ID_LEN];
	struct flash_regop_info info;
	struct spinor_chip_xfer *chip_xfer;
	struct spinor_flash_dev *flash_dev = NULL;
	struct spinor_flash_dev *first_flash_dev = NULL;

	spinor->chipsize = 0;
	spinor->nr_chips = 0;
	spinor->erasesize = 0;

	spinor->host->set_clock(spinor->host, 0, ENABLE);

	for (nrchip = 0; nrchip < HIFMC100_OP_CFG_NUM_CS; nrchip++) {
		memset(&info, 0, sizeof(info));

		spinor->host->chipselect = nrchip;

		info.priv = spinor->host;
		info.cmd = SPINOR_OP_RDID;
		info.nr_cmd = 1;
		info.buf = id;
		info.sz_buf = sizeof(id);
		spinor->regop->read_reg(&info);

		if (!id[0])
			continue;

		printf("%s id: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
			spinor->name, id[0], id[1], id[2], id[3], id[4], id[5]);

		spinor->chipselect[spinor->nr_chips] = nrchip;

		if (!spinor->nr_chips) {
			flash_dev = spinor_match_id(id, sizeof(id));
			if (!flash_dev) {
				printf("Can't not recognize this chip(cs%d).\n", nrchip);
				return -EINVAL;
			}

			spinor->nr_chips = 1;
			spinor->chipsize = flash_dev->chipsize;
			spinor->chipsize_shift = ffs(spinor->chipsize) - 1;
			spinor->chipsize_mask = spinor->chipsize - 1;

			spinor->drv = spinor_get_driver(id, sizeof(id));

			if (spinor->chipsize >= _32M)
				spinor->addr_cycle = 4;
			else
				spinor->addr_cycle = 3;

			if (!first_flash_dev)
				first_flash_dev = flash_dev;

			continue;
		}

		if (memcmp(id, flash_dev->id, flash_dev->id_len))
			break;

		spinor->nr_chips++;
	}

	if (!flash_dev)
		return -EINVAL;

	if (first_flash_dev)
		set_spiflash_info(first_flash_dev->chipsize, first_flash_dev->erasesize,
			 	  spinor->nr_chips, first_flash_dev->name,
				  first_flash_dev->id, first_flash_dev->id_len);


	/* anytime, there is a default value return. */
	chip_xfer = spinor_get_xfer(hifmc100_spinor_chip_xfer,
				    first_flash_dev->id, 
				    first_flash_dev->id_len);

	hifmc100_spinor_setup(spinor, chip_xfer);

	printf("Block:%sB ", ultohstr((u64)spinor->erasesize));
	printf("Chip:%sB ", ultohstr((u64)spinor->chipsize));
	printf("Name:\"%s\"\n", first_flash_dev->name);

	hifmc100_spinor_chip_init(spinor);

	return 0;
}
/******************************************************************************/

static void hifmc100_spinor_shutdown(void)
{
	int nrchip;
	struct hifmc_host *hifmc = &host;
	struct hifmc_spinor *spinor = hifmc->spinor;

	if (!spinor)
		return;

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINOR);
	hifmc->set_clock(hifmc, 0, ENABLE);

	for (nrchip = 0; nrchip < spinor->nr_chips; nrchip++) {
		hifmc100_spinor_select_chip(spinor, nrchip);

		spinor->drv->quad_disable(spinor->regop);
		if (spinor->addr_cycle >= 4)
			spinor->drv->exit_4addr(spinor->regop);
	}

}
/******************************************************************************/

struct spiflash_dev_t *hifmc100_spinor_probe(void)
{
	int ret;
	struct hifmc_spinor *spinor;
	struct hifmc_host *hifmc = &host;
	static struct spiflash_dev_t *spiflash;

	if (spiflash)
		return spiflash;

	spinor = malloc(sizeof(struct hifmc_spinor));
	if (!spinor) {
		PR_OUT("failed to allocate hifmc_spinor structure.\n");
		return NULL;
	}

	spinor->name = "spinor";
	spinor->host = hifmc;
	spinor->regop->priv = hifmc;
	spinor->regop->read_reg = hifmc100_read_reg;
	spinor->regop->write_reg = hifmc100_write_reg;
	spinor->regop->read_sr = hifmc100_spinor_read_sr;
	spinor->regop->wait_device_ready = hifmc100_spinor_wait_device_ready;

	hifmc->spinor = spinor;

	hifmc->set_ifmode(hifmc, HIFMC_IFMODE_SPINOR);
	hifmc_write(hifmc, 0x6F, HIFMC100_TIMING_SPI_CFG);

	ret = hifmc100_spinor_probe_device(spinor);
	if (ret) {
		free(spinor);
		return NULL;
	}

	spiflash = hifmc->spiflash;

	spiflash->name = "hi_sfc";
	spiflash->size = spinor->chipsize;
	spiflash->erase = hifmc100_spinor_erase;
	spiflash->write = hifmc100_spinor_dma_write;
	spiflash->read  = hifmc100_spinor_dma_read;

	spinor->dma_paddr = (unsigned int)spinor->dma_vaddr;

	return spiflash;
}

/*****************************************************************************/
static struct spiflash_dev_t *hifmc100_spinor_init(void)
{
	/* first diable cache. */
	unsigned int cache_enabled_old = cache_enabled;
	if (cache_enabled)
		mmu_cache_disable();

	if (hifmc100_host_init())
		goto init_failed;

	__spiflash = hifmc100_spinor_probe();
	if (!__spiflash)
		goto init_failed;

	return __spiflash;

init_failed:
	if (cache_enabled_old)
		mmu_cache_enable();

	return NULL;
}
/*****************************************************************************/

static void hifmc100_module_init(void *ptr)
{
	spiflash_driver_init = hifmc100_spinor_init;
}
/*****************************************************************************/

static void hifmc100_module_reboot(void *ptr)
{
	if (__spiflash)
		hifmc100_spinor_shutdown();
}
/*****************************************************************************/

static struct module_t module_hifmc100_spinor = {
	.priority = MODULE_PRI_DRV,
	.init = hifmc100_module_init,
	.reboot = hifmc100_module_reboot,
};
MODULE(module_hifmc100_spinor);
