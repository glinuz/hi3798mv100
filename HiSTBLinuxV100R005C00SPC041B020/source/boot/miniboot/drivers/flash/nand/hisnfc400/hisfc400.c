/******************************************************************************
 *    SPI NAND Flash Controller v400 Device Driver
 *    Copyright (c) 2014 Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By hisilicon
 *
******************************************************************************/
#include <config.h>
#include <stddef.h>
#include <errno.h>
#include <malloc.h>
#include <module.h>
#include <mmu.h>
#include <stdio.h>
#include <string.h>
#include <bits.h>
#include <cpuinfo.h>
#include <debug.h>
#include <errno.h>
#include <nand_drv.h>

#include "hisfc400.h"
#include "hisfc400_gen.h"
#include "hisfc400_ecc_info.h"
#include "spi_nand_spl_ids.h"

#ifdef CONFIG_ARCH_HI3716MV310
#  include "hisfc400_hi3716mv310.c"
#endif

#ifdef CONFIG_ARCH_HIFONE
#  include "hisfc400_hifone.c"
#endif

#define HISFC400_NANDINFO_LEN             0x10

extern unsigned int cache_enabled;

/*****************************************************************************/
#ifdef HISFCV400_SUPPORT_REG_WRITE
static void hisfc400_reg_pageprog(struct hisfc_host *host)
{
	unsigned int regval;
	struct hisfc_operation *spi = host->spi;

	spi->driver->wait_ready(spi);

	spi->driver->write_enable(spi);

	host->set_system_clock(host, spi->write, TRUE);

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);
	hisfc_write(host, HISFC400_OP_CFG, 
		    HISFC400_OP_CFG_MEM_IF_TYPE(spi->write->iftype));

	hisfc_write(host, HISFC400_ADDRL, host->addr_value[0] & 0xffff0000);
	hisfc_write(host, HISFC400_ADDRH, host->addr_value[1]);

	regval = HISFC400_OP_CTRL_WR_OPCODE(spi->write->cmd)
		| HISFC400_OP_CTRL_CS_OP(host->chipselect)
		| HISFC400_OP_CTRL_OP_TYPE(OP_TYPE_REG)
		| HISFC400_OP_CTRL_RW_OP(RW_OP_WRITE)
		| HISFC400_OP_CTRL_OP_READY;
	hisfc_write(host, HISFC400_OP_CTRL, regval);

//	HISFC400_DMA_WAIT_CPU_FINISH(host);
	HISFC400_DMA_WAIT_INT_FINISH(host);
}
#else
/*****************************************************************************/
static void hisfc400_dma_pageprog(struct hisfc_host *host)
{
	unsigned regval;
	struct hisfc_operation *spi = host->spi;

	spi->driver->wait_ready(spi);

	spi->driver->write_enable(spi);

	host->set_system_clock(host, spi->write, TRUE);

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);

	regval = HISFC400_OP_CFG_MEM_IF_TYPE(spi->write->iftype);
	hisfc_write(host, HISFC400_OP_CFG, regval);

	hisfc_write(host, HISFC400_ADDRL, host->addr_value[0] & 0xffff0000);
	hisfc_write(host, HISFC400_ADDRH, host->addr_value[1]);

	hisfc_write(host, HISFC400_DMA_CTRL, 
		HISFC400_DMA_BURST4_ENABLE
		| HISFC400_DMA_BURST8_ENABLE
		| HISFC400_DMA_BURST16_ENABLE);

	hisfc_write(host, HISFC400_DMA_ADDR_DATA, host->dma_buffer);
	hisfc_write(host, HISFC400_DMA_ADDR_OOB, host->dma_oob);

	regval = HISFC400_OP_CTRL_WR_OPCODE(spi->write->cmd)
		| HISFC400_OP_CTRL_CS_OP(host->chipselect)
		| HISFC400_OP_CTRL_OP_TYPE(OP_TYPE_DMA)
		| HISFC400_OP_CTRL_RW_OP(RW_OP_WRITE)
		| HISFC400_OP_CTRL_OP_READY;

	hisfc_write(host, HISFC400_OP_CTRL, regval);

//	HISFC400_DMA_WAIT_CPU_FINISH(host);
	HISFC400_DMA_WAIT_INT_FINISH(host);
}
#endif /* HISFCV400_SUPPORT_REG_WRITE */

/*****************************************************************************/
#ifdef HISFCV400_SUPPORT_REG_READ
static void hisfc400_reg_readstart(struct hisfc_host *host)
{
	unsigned int regval;
	struct hisfc_operation *spi = host->spi;

	if ((host->addr_value[0] == host->cache_addr_value[0])
		&& (host->addr_value[1] == host->cache_addr_value[1]))
		return;

	spi->driver->wait_ready(spi);

	host->set_system_clock(host, spi->read, TRUE);

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);

	regval = HISFC400_OP_CFG_RD_OP_SEL(HISFC400_OP_CFG_RD_OP_TYPE_PAGE)
		| HISFC400_OP_CFG_MEM_IF_TYPE(spi->read->iftype)
		| HISFC400_OP_CFG_DUMMY_ADDR_NUM(spi->read->dummy);

	hisfc_write(host, HISFC400_OP_CFG, regval);

	hisfc_write(host, HISFC400_ADDRL, host->addr_value[0] & 0xffff0000);
	hisfc_write(host, HISFC400_ADDRH, host->addr_value[1]);	

	regval = HISFC400_OP_CTRL_RD_OPCODE(spi->read->cmd)
		| HISFC400_OP_CTRL_CS_OP(host->chipselect)
		| HISFC400_OP_CTRL_OP_TYPE(OP_TYPE_REG)
		| HISFC400_OP_CTRL_RW_OP(RW_OP_READ)
		| HISFC400_OP_CTRL_OP_READY;
	hisfc_write(host, HISFC400_OP_CTRL, regval);

//	HISFC400_DMA_WAIT_CPU_FINISH(host);
	HISFC400_DMA_WAIT_INT_FINISH(host);

	host->cache_addr_value[0] = host->addr_value[0];
	host->cache_addr_value[1] = host->addr_value[1];
}
#else
/*****************************************************************************/
static void hisfc400_dma_readstart(struct hisfc_host *host)
{
	unsigned int regval;
	struct hisfc_operation *spi = host->spi;

	if ((host->addr_value[0] == host->cache_addr_value[0])
		&& (host->addr_value[1] == host->cache_addr_value[1]))
		return;

	spi->driver->wait_ready(spi);

	host->set_system_clock(host, spi->read, TRUE);

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);

	regval = HISFC400_OP_CFG_RD_OP_SEL(HISFC400_OP_CFG_RD_OP_TYPE_PAGE)
		| HISFC400_OP_CFG_MEM_IF_TYPE(spi->read->iftype)
		| HISFC400_OP_CFG_DUMMY_ADDR_NUM(spi->read->dummy);
	hisfc_write(host, HISFC400_OP_CFG, regval);

	hisfc_write(host, HISFC400_ADDRL, host->addr_value[0] & 0xffff0000);
	hisfc_write(host, HISFC400_ADDRH, host->addr_value[1]);	

	hisfc_write(host, HISFC400_DMA_CTRL, 
		HISFC400_DMA_BURST4_ENABLE
		| HISFC400_DMA_BURST8_ENABLE
		| HISFC400_DMA_BURST16_ENABLE);

	hisfc_write(host, HISFC400_DMA_ADDR_DATA, host->dma_buffer);
	hisfc_write(host, HISFC400_DMA_ADDR_OOB, host->dma_oob);

	regval = HISFC400_OP_CTRL_RD_OPCODE(spi->read->cmd)
		| HISFC400_OP_CTRL_CS_OP(host->chipselect)
		| HISFC400_OP_CTRL_OP_TYPE(OP_TYPE_DMA)
		| HISFC400_OP_CTRL_RW_OP(RW_OP_READ)
		| HISFC400_OP_CTRL_OP_READY;
	hisfc_write(host, HISFC400_OP_CTRL, regval);

//	HISFC400_DMA_WAIT_CPU_FINISH(host);
	HISFC400_DMA_WAIT_INT_FINISH(host);

	host->cache_addr_value[0] = host->addr_value[0];
	host->cache_addr_value[1] = host->addr_value[1];
}
#endif /* HISFCV400_SUPPORT_REG_READ */

/*****************************************************************************/
static void hisfc400_send_cmd_erase(struct hisfc_host *host)
{
	struct hisfc_operation *spi = host->spi;
	unsigned int regval;

	spi->driver->wait_ready(spi);

	spi->driver->write_enable(spi);

	host->set_system_clock(host, spi->erase, TRUE);

	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);
	hisfc_write(host, HISFC400_OPCODE, spi->erase->cmd);

	hisfc_write(host, HISFC400_OP_ADDR, host->addr_value[0]);

	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);

	regval = (HISFC400_OP_SEL_CS(host->chipselect)
		| HISFC400_OP_ADDR_NUM(STD_OP_ADDR_NUM)
		| HISFC400_OP_OPCODE_EN
		| HISFC400_OP_ADDR_EN
		| HISFC400_OP_START);

	hisfc_write(host, HISFC400_OP, regval);

	HISFC400_CMD_WAIT_CPU_FINISH(host);
}
/*****************************************************************************/

static void hisfc400_send_cmd_status(struct hisfc_host *host)
{
	/* read status register should not clear all ints. */
	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_OP_DONE);
	hisfc_write(host, HISFC400_OPCODE, SPI_CMD_GET_FEATURES);

	/* nand_check_wp is not supported. */
	hisfc_write(host, HISFC400_OP_ADDR, STATUS_REGISTER);

	hisfc_write(host, HISFC400_DATA_NUM,
			HISFC400_DATA_NUM_CNT(FEATURES_DATE_NUM));
	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);
	hisfc_write(host, HISFC400_OP,
		    HISFC400_OP_SEL_CS(host->chipselect)
		    | HISFC400_OP_ADDR_NUM(1)
		    | HISFC400_OP_OPCODE_EN
		    | HISFC400_OP_ADDR_EN
		    | HISFC400_OP_STATUS_READ_EN
		    | HISFC400_OP_START);

	HISFC400_CMD_WAIT_CPU_FINISH(host);
}

/*****************************************************************************/
static void hisfc400_send_cmd_readid(struct hisfc_host *host)
{
	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);
	hisfc_write(host, HISFC400_OPCODE, SPI_CMD_RDID);
	hisfc_write(host, HISFC400_OP_ADDR, READ_ID_ADDR);
	hisfc_write(host, HISFC400_DATA_NUM, HISFC400_DATA_NUM_CNT(READ_ID_LEN));
	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);
	hisfc_write(host, HISFC400_OP,
			HISFC400_OP_SEL_CS(host->chipselect)
			| HISFC400_OP_ADDR_NUM(READ_ID_ADDR_NUM)
			| HISFC400_OP_OPCODE_EN
			| HISFC400_OP_ADDR_EN
			| HISFC400_OP_DATE_READ_EN
			| HISFC400_OP_START);

	HISFC400_CMD_WAIT_CPU_FINISH(host);
}
/*****************************************************************************/
static void hisfc400_send_cmd_reset(struct hisfc_host *host)
{
	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_ALL);
	hisfc_write(host, HISFC400_OPCODE, SPI_CMD_RESET);
	hisfc_write(host, HISFC400_OP_CFG, HISFC400_OP_CFG_DIR_TRANS_ENABLE);
	hisfc_write(host, HISFC400_OP,
		HISFC400_OP_SEL_CS(host->chipselect)
		| HISFC400_OP_OPCODE_EN
		| HISFC400_OP_START);

	HISFC400_CMD_WAIT_CPU_FINISH(host);
}

/*****************************************************************************/
/*
 *  return:
 *     1   - write/erase/reset fail.
 *     0   - write/erase/reset pass.
 *    -1   - write/erase/reset wait RB timeout.
 */
static int hisfc400_wait_ready(struct hisfc_host *host)
{
	uint8 *iobase = (uint8 *)host->iobase;
	int timeout = 0x10000;

	do {
		*iobase = 0;
		hisfc400_send_cmd_status(host);
	} while ((*iobase & SPI_NAND_STATUS_READY) && timeout--);

	if (!timeout) {
		DBG_BUG("Wait controll reset status timeout.\n");
		return -1;
	}

	return (*iobase);
}

/*****************************************************************************/

static void hisfc400_set_address(struct nand_dev_t *nand_dev, uint64 address,
				 int only_row)
{
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	address >>= nand_dev->pageshift;

	// TODO: 这个地址应该是动态识别.
	if (!only_row) {
		address <<= 16;
		host->addr_cycle = 5;
	} else {
		host->addr_cycle = 3;
	}

	host->addr_value[0] = (uint32)(address & 0xFFFFFFFF);
	host->addr_value[1] = (uint32)((address >> 32) & 0xFFFFFFFF);
}
/*****************************************************************************/

static int hisfc400_read_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = (address >> nand_dev->chipshift) + 1;
		address &= nand_dev->chipmask;
	} else {
		host->chipselect = 1;
	}

	hisfc400_set_address(nand_dev, address, FALSE);
	hisfc400_dma_readstart(host);

	hisfc400_wait_ready(host);

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *     0 - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hisfc400_write_page(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret;
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = (address >> nand_dev->chipshift) + 1;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 1;

	hisfc400_set_address(nand_dev, address, FALSE);
	hisfc400_dma_pageprog(host);

	ret = hisfc400_wait_ready(host);
	if (ret & SPI_NAND_STATUS_PFAIL)
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
static int hisfc400_erase_block(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret;
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->blockmask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = (address >> nand_dev->chipshift) + 1;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 1;

	hisfc400_set_address(nand_dev, address, TRUE);
	hisfc400_send_cmd_erase(host);

	ret = hisfc400_wait_ready(host);
	if (ret & SPI_NAND_STATUS_EFAIL)
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
static int hisfc400_reset(struct nand_dev_t *nand_dev, int chipselect)
{
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	host->chipselect = chipselect + 1;

	hisfc400_send_cmd_reset(host);

	hisfc400_wait_ready(host);

	return 0;
}
/*****************************************************************************/

static int hisfc400_read_id(struct nand_dev_t *nand_dev, int chipselect,
			    char *buf, int buflen)
{
	uint8 *iobase;
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

	iobase = (uint8 *)host->iobase;

	host->chipselect = chipselect + 1;

	memset(iobase, 0, HISFC400_NANDINFO_LEN);
	hisfc400_send_cmd_readid(host);

	buflen = min(buflen, HISFC400_NANDINFO_LEN);
	memcpy(buf, iobase, buflen);

	return 0;
}

/*****************************************************************************/
static int hisfc400_params_adjust(struct nand_dev_t *nand_dev)
{
	char *start_type = "unknown";
	int pagetype, ecctype;
	struct nand_ctrl_info_t *best = NULL;
	struct spi_nand_chip_info_t *nand_info = nand_dev->spi_nand_info;
	struct hisfc_host *host = (struct hisfc_host *)nand_dev->priv;

#ifdef CONFIG_HISFC400_AUTO_PAGESIZE_ECC
	best = hisfc400_get_best_ecc(nand_info);
	start_type = "Auto";
#endif /* CONFIG_HISFC400_AUTO_PAGESIZE_ECC */

#ifdef CONFIG_HISFC400_HARDWARE_PAGESIZE_ECC
#  ifdef CONFIG_HISFC400_AUTO_PAGESIZE_ECC
#  error you SHOULD NOT define CONFIG_HISFC400_AUTO_PAGESIZE_ECC \
	and CONFIG_HISFC400_HARDWARE_PAGESIZE_ECC at the same time
#  endif

	pagetype = hisfc400_page_reg2type((host->NFC_CFG >>
		HISFC400_CFG_PAGEISZE_SHIFT) & HISFC400_CFG_PAGEISZE_MASK);
	ecctype = hisfc400_ecc_reg2type((host->NFC_CFG >>
		HISFC400_CFG_ECCTYPE_SHIFT) & HISFC400_CFG_ECCTYPE_MASK);

	start_type = "Hardware";
	best = hisfc400_force_ecc(nand_info, pagetype, ecctype, 
				  (char *)"Hardware Config", 0);
#endif /* CONFIG_HISFC400_HARDWARE_PAGESIZE_ECC */

#ifdef CONFIG_HISFC400_PAGESIZE_AUTO_ECC_NONE
#  ifdef CONFIG_HISFC400_AUTO_PAGESIZE_ECC
#  error you SHOULD NOT define CONFIG_HISFC400_PAGESIZE_AUTO_ECC_NONE \
	and CONFIG_HISFC400_AUTO_PAGESIZE_ECC at the same time
#  endif
#  ifdef CONFIG_HISFC400_HARDWARE_PAGESIZE_ECC
#  error you SHOULD NOT define CONFIG_HISFC400_PAGESIZE_AUTO_ECC_NONE \
	and CONFIG_HISFC400_HARDWARE_PAGESIZE_ECC at the same time
#  endif
	pagetype = nandpage_size2type(nand_info->pagesize);
	best = hisfc400_force_ecc(nand_info, pagetype, NAND_ECC_NONE,
				  "force config", 0);
	start_type = "AutoForce";
#endif /* CONFIG_HISFC400_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
		DBG_BUG(ERSTR_HARDWARE
			"Please configure Nand Flash pagesize and ecctype!\n");

	if (best->ecctype != NAND_ECC_NONE)
		nand_dev->oobsize = best->oobsize;
	nand_dev->oobused = HISFC400_OOBSIZE_FOR_YAFFS;
	nand_dev->layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = best->pagesize;
	host->oobsize  = nand_dev->oobsize;
	host->block_page_mask = 
		((nand_info->erasesize / nand_info->pagesize) - 1);

	host->buffer = devmemalign(HISFC400_DMA_ALIGN, host->pagesize
		+ host->oobsize);
	if (!host->buffer) {
		DBG_BUG(ERSTR_DRIVER
			"Can't malloc memory for NAND driver.");
	}

	host->oobbuf = host->buffer + host->pagesize;
	host->dma_buffer = (unsigned int)host->buffer;

	memset(host->iobase, 0xff, HISFC400_BUFFER_BASE_ADDRESS_LEN);
	memset(host->buffer, 0xff, (host->pagesize + host->oobsize));

	host->dma_oob = host->dma_buffer + host->pagesize;
	host->bbm = (unsigned char *)(host->buffer
		+ host->pagesize + HISFC_BAD_BLOCK_POS);

	host->epm = (unsigned short *)(host->buffer
		+ host->pagesize + best->layout->offset + 28);

	nand_dev->bbm = host->bbm;
	nand_dev->pagebuf = host->buffer;
	nand_dev->oobbuf = host->oobbuf;

	hisfc400_set_pagesize(host);
	hisfc400_set_ecctype(host);

	if (nand_info->pagesize > SPI_NAND_MAX_PAGESIZE
	    || nand_info->oobsize > SPI_NAND_MAX_OOBSIZE) {
		DBG_BUG(ERSTR_DRIVER
			"Driver does not support this Nand Flash. Please "
			"increase SPI_NAND_MAX_PAGESIZE and SPI_NAND_MAX_OOBSIZE.\n");
	}

	if (nand_info->pagesize != host->pagesize) {
		unsigned int shift = 0;
		unsigned int writesize = nand_info->pagesize;
		while (writesize > host->pagesize) {
			writesize >>= 1;
			shift++;
		}
		nand_dev->chipsize >>= shift;
		nand_dev->blocksize >>= shift;
		printf("Nand divide into 1/%u\n", (1 << shift));
	}


	nand_dev->bootmsg = start_type;
	nand_dev->ecctype = host->ecctype;

	return 0;
}
/*****************************************************************************/
int hisfc400_host_init(struct hisfc_host *host)
{
	unsigned int regval;

	regval = hisfc_read(host, HISFC400_CFG);
	if ((regval & DEVICE_TYPE_MASK) != DEVICE_TYPE_NAND_FLASH) {
		DBG_BUG("UNKNOWN flash chip type!\n");
		return -ENXIO;
	}

	if ((regval & OP_MODE_MASK) == OP_MODE_BOOT)
		regval |= HISFC400_CFG_OP_MODE(OP_MODE_NORMAL);

	if (regval & HISFC400_CFG_INTERNAL_ECC)
		regval &= ~HISFC400_CFG_INTERNAL_ECC;

	hisfc_write(host, HISFC400_CFG, regval);

	host->NFC_CFG = regval;

	host->addr_cycle    = 0;
	host->addr_value[0] = 0;
	host->addr_value[1] = 0;
	host->cache_addr_value[0] = ~0;
	host->cache_addr_value[1] = ~0;

	host->set_system_clock   = hisfc400_set_system_clock;

#ifdef HISFCV400_SUPPORT_REG_WRITE
	host->send_cmd_pageprog  = hisfc400_reg_pageprog;
#else
	host->send_cmd_pageprog  = hisfc400_dma_pageprog;
#endif

#ifdef HISFCV400_SUPPORT_REG_READ
	host->send_cmd_readstart = hisfc400_reg_readstart;
#else
	host->send_cmd_readstart = hisfc400_dma_readstart;
#endif

	host->send_cmd_erase     = hisfc400_send_cmd_erase;
	host->send_cmd_status    = hisfc400_send_cmd_status;

	host->send_cmd_readid    = hisfc400_send_cmd_readid;
	host->send_cmd_reset     = hisfc400_send_cmd_reset;

	host->set_system_clock(host, NULL, TRUE);

	hisfc_write(host, HISFC400_TIMING_CFG, 
		    HISFC400_TIMING_CFG_TCSH(0x6)
		    | HISFC400_TIMING_CFG_TCSS(0x6)
		    | HISFC400_TIMING_CFG_TSHSL(0xf));


	return 0;
}

/*****************************************************************************/
static int hisfc400_version_check(void)
{
	unsigned long regval;

	regval = readl(CONFIG_HISFC400_REG_BASE_ADDRESS + HISFC400_VERSION);
	return (regval == 0x400);
}

/*****************************************************************************/
static struct hisfc_host hisfc400_host;

static struct nand_dev_t hisfc400_nand_dev = {
	.bootmsg = "Unknown",
	.read_page = hisfc400_read_page,
	.write_page = hisfc400_write_page,
	.erase_block = hisfc400_erase_block,
	.reset = hisfc400_reset,
	.read_id = hisfc400_read_id,
	.adjust_param = hisfc400_params_adjust,
	.priv = (void *)&hisfc400_host,
};
/*****************************************************************************/

static struct nand_dev_t *hisfc400_driver_init(void)
{
	unsigned int cache_enabled_old = cache_enabled;
	struct hisfc_host *host;
	static struct nand_dev_t *nand_dev = NULL;

	if (nand_dev)
		return nand_dev;

	if (cache_enabled)
		mmu_cache_disable();

	nand_dev = &hisfc400_nand_dev;
	host = (struct hisfc_host *)nand_dev->priv;

	memset((char *)host, 0, sizeof(struct hisfc_host));

	host->chipselect = 1;

	printf("spiflash: Check Spi-Nand Flash Controller V400. ");
	if (!hisfc400_version_check()) {
		printf("\n");
		goto init_failed;
	}
	printf("Found\n");

	host->regbase = (void *)CONFIG_HISFC400_REG_BASE_ADDRESS;
	host->iobase = (void *)CONFIG_HISFC400_BUFFER_BASE_ADDRESS;
	memset((char *)host->iobase, 0xff, HISFC400_BUFFER_BASE_ADDRESS_LEN);

	host->sysreg = (void *)CONFIG_HISFC400_PERIPHERY_REGBASE;

	hisfc400_host_init(host);

	find_spl_spi_nand_chip = hisfc400_find_spl_spi_nand_chip;

	return nand_dev;

init_failed:
	if (cache_enabled_old)
		mmu_cache_enable();

	return NULL;
}
/*****************************************************************************/

static void hisfc400_shutdown(struct hisfc_host *host)
{
	// todo: xxx
}
/*****************************************************************************/

static void hisfc400_module_exit(void *ptr)
{
	struct hisfc_host *host = (struct hisfc_host *)ptr;

	hisfc400_shutdown(host);
}
/*****************************************************************************/

static void hisfc400_module_init(void *ptr)
{
	spi_nand_driver_init = hisfc400_driver_init;
}
/*****************************************************************************/

static struct module_t module_hisfc400 = {
	.priority = MODULE_PRI_DRV,
	.init = hisfc400_module_init,
	.exit = hisfc400_module_exit,
};
MODULE(module_hisfc400);

