/******************************************************************************
 *    SPI NAND Flash Controller v400 Device Driver
 *    Copyright (c) 2014 Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By hisilicon
 *
******************************************************************************/
#include <nand.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/mtd/nand.h>

#include "hisfc400.h"
#include "hisfc400_gen.h"
#include "hisfc400_ecc_info.h"
#include <asm-generic/errno.h>

#ifdef CONFIG_ARCH_HI3716MV310
#  include "hisfc400_hi3716mv310.c"
#endif

#ifdef CONFIG_ARCH_HIFONE
#  include "hisfc400_hifone.c"
#endif

/*****************************************************************************/
static struct hisfc_host host = {
	.chip = NULL,
};

extern void hisfc_nand_spl_ids_register(void);

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
	struct nand_chip *chip = host->chip;

	/* read status register should not clear all ints. */
	hisfc_write(host, HISFC400_INTCLR, HISFC400_INTCLR_OP_DONE);
	hisfc_write(host, HISFC400_OPCODE, SPI_CMD_GET_FEATURES);

	/* in case nand_check_wp, should read protection register. */
	if (chip->state == FL_READING)
		hisfc_write(host, HISFC400_OP_ADDR, PROTECTION_REGISTER);
	else
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
static void status_register_reorganize(struct mtd_info *mtd, 
					unsigned char *value)
{
	unsigned char status_register;
	struct nand_chip *chip = mtd->priv;

	/* init: no protect, no error. */
	status_register = NAND_STATUS_WP;
	/* in case nand_check_wp, should read protection register. */
	if (chip->state == FL_READING) {
		if (*value & ALL_BP_MASK)
			status_register &= ~NAND_STATUS_WP;

	} else {
		if (!(*value & STATUS_OIP_MASK))
			status_register |= NAND_STATUS_READY;
		
		if (((chip->state == FL_WRITING) && (*value & STATUS_P_FAIL_MASK))
		    || ((chip->state == FL_ERASING) && (*value & STATUS_E_FAIL_MASK))) {
			status_register |= NAND_STATUS_FAIL;
		}
	}

	*value = status_register;
}

/*****************************************************************************/
static uint8_t hisfc400_read_byte(struct mtd_info *mtd)
{
	unsigned char value = 0;
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

	if (host->command == NAND_CMD_STATUS) {
		value = hisfc_read(host, HISFC400_STATUS);
		status_register_reorganize(mtd, &value);
		return value;
	}

	host->offset++;

	if (host->command == NAND_CMD_READID)
		return readb(chip->IO_ADDR_R + host->offset - 1);

	return readb(host->buffer + host->column + host->offset - 1);

}

/*****************************************************************************/
static u16 hisfc400_read_word(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

	host->offset += 2;
	return readw(host->buffer + host->column + host->offset - 2);
}

/*****************************************************************************/
static void hisfc400_write_buf(struct mtd_info *mtd, const uint8_t *buf, 
			       int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

#ifdef HISFCV400_SUPPORT_REG_WRITE
	memcpy((char *)host->iobase + host->column + host->offset, buf, len);
	host->offset += len;
#else
	memcpy(host->buffer + host->column + host->offset, buf, len);
	host->offset += len;
#endif
}

/*****************************************************************************/
static void hisfc400_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

#ifdef HISFCV400_SUPPORT_REG_READ
	memcpy(buf, (char *)host->iobase + host->column + host->offset, len);
	host->offset += len;
#else
	memcpy(buf, host->buffer + host->column + host->offset, len);
	host->offset += len;
#endif
}

/*****************************************************************************/
static void hisfc400_select_chip(struct mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

	if (chipselect < 0)
		return;

	if (chipselect > CONFIG_HISFC400_MAX_CHIP)
		DBG_BUG("invalid chipselect: %d\n", chipselect);

	host->chipselect = chipselect + 1;
}

/*****************************************************************************/
static void hisfc400_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	int is_cache_invalid = 1;
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

	if (ctrl & NAND_ALE) {
		unsigned int addr_value = 0;
		unsigned int addr_offset = 0;

		if (ctrl & NAND_CTRL_CHANGE) {
			host->addr_cycle = 0x0;
			host->addr_value[0] = 0x0;
			host->addr_value[1] = 0x0;
		}
		addr_offset = host->addr_cycle << 3;

		if (host->addr_cycle >= HISFC400_ADDR_CYCLE_MASK) {
			addr_offset = (host->addr_cycle -
					HISFC400_ADDR_CYCLE_MASK) << 3;
			addr_value = 1;
		}

		host->addr_value[addr_value] |=
			((dat & 0xff) << addr_offset);

		host->addr_cycle++;
	}

	if ((ctrl & NAND_CLE) && (ctrl & NAND_CTRL_CHANGE)) {
		host->command = dat & 0xff;
		switch (host->command) {
		case NAND_CMD_PAGEPROG:
			host->offset = 0;
			host->send_cmd_pageprog(host);
			break;

		case NAND_CMD_READSTART:
			is_cache_invalid = 0;
			host->send_cmd_readstart(host);
			break;

		case NAND_CMD_ERASE2:
			host->send_cmd_erase(host);
			break;

		case NAND_CMD_READID:
			memset((unsigned char *)(chip->IO_ADDR_R), 0,
				READ_ID_LEN);
			host->send_cmd_readid(host);
			break;

		case NAND_CMD_STATUS:
			host->send_cmd_status(host);
			break;

		case NAND_CMD_SEQIN:
			break;

		case NAND_CMD_ERASE1:
			break;

		case NAND_CMD_READ0:
			break;

		case NAND_CMD_RESET:
			host->send_cmd_reset(host);
			break;

		default:
			break;
		}
	}

	if ((dat == NAND_CMD_NONE) && host->addr_cycle) {
		if (host->command == NAND_CMD_SEQIN
		    || host->command == NAND_CMD_READ0
		    || host->command == NAND_CMD_READID) {
			host->offset = 0x0;
			host->column = (host->addr_value[0] & 0xffff);
		}
	}

	if (is_cache_invalid) {
		host->cache_addr_value[0] = ~0;
		host->cache_addr_value[1] = ~0;
	}
}

/*****************************************************************************/
static int hisfc400_dev_ready(struct mtd_info *mtd)
{
	unsigned regval = 0;
	unsigned deadline = 0;
	struct nand_chip *chip = mtd->priv;
	struct hisfc_host *host = chip->priv;

	do {
		spi_feature_op(host, GET_FEATURE, STATUS_REGISTER, &regval);
		if (!(regval & STATUS_OIP_MASK))
			return 1;
		udelay(1);
	} while (deadline++ < (40 << 20));

	printk("Wait spi flash ready timeout.\n");

	return 0;
}

/*****************************************************************************/
static int hisfc400_ecc_probe(struct mtd_info *mtd, struct nand_chip *chip,
			struct nand_dev_t *nand_dev)
{
	char *start_type = "unknown";
	struct nand_config_table *best = NULL;
	struct hisfc_host *host = chip->priv;
	int pagetype;
	int ecctype;

#ifdef CONFIG_HISFC400_AUTO_PAGESIZE_ECC
	best = hisfc400_get_best_ecc(mtd);
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
	best = hisfc400_force_ecc(mtd, pagetype, ecctype, 
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
	pagetype = nandpage_size2type(mtd->writesize);
	best = hisfc400_force_ecc(mtd, pagetype, NAND_ECC_NONE,
				  "force config", 0);
	start_type = "AutoForce";
#endif /* CONFIG_HISFC400_PAGESIZE_AUTO_ECC_NONE */

	if (!best) {
		DBG_BUG(ERSTR_HARDWARE
			"Please configure Nand Flash pagesize and ecctype!\n");
		return 0;
	}

	if (best->ecctype != NAND_ECC_NONE)
		mtd->oobsize = best->oobsize;

	mtd->oobused = HISFC400_OOBSIZE_FOR_YAFFS;

	chip->ecc.layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = nandpage_type2size(best->pagetype);
	host->oobsize  = mtd->oobsize;
	host->block_page_mask = ((mtd->erasesize / mtd->writesize) - 1);

	host->dma_oob = host->dma_buffer + host->pagesize;

	host->bbm = (unsigned char *)(host->buffer + host->pagesize
		    + HISFC_BAD_BLOCK_POS);

	host->epm = (unsigned short *)(host->buffer + host->pagesize
			+ chip->ecc.layout->oobfree[0].offset + 28);

	hisfc400_set_pagesize(host);
	hisfc400_set_ecctype(host);

	if (mtd->writesize > SPI_NAND_MAX_PAGESIZE
	    || mtd->oobsize > SPI_NAND_MAX_OOBSIZE) {
		DBG_BUG(ERSTR_DRIVER
			"Driver does not support this Nand Flash. Please "
			"increase SPI_NAND_MAX_PAGESIZE and SPI_NAND_MAX_OOBSIZE.\n");
	}

	if (mtd->writesize != host->pagesize) {
		unsigned int shift = 0;
		unsigned int writesize = mtd->writesize;
		while (writesize > host->pagesize) {
			writesize >>= 1;
			shift++;
		}
		chip->chipsize = chip->chipsize >> shift;
		mtd->erasesize = mtd->erasesize >> shift;
		mtd->writesize = host->pagesize;
		printk(KERN_INFO "Nand divide into 1/%u\n", (1 << shift));
	}

	nand_dev->start_type = start_type;
	nand_dev->ecctype = host->ecctype;
	nand_dev->hostver = HISFC_VER_400;

	return 0;
}

/*****************************************************************************/
void hisfc400_nand_init(struct nand_chip *chip)
{
	chip->read_byte   = hisfc400_read_byte;
	chip->read_word   = hisfc400_read_word;
	chip->write_buf   = hisfc400_write_buf;
	chip->read_buf    = hisfc400_read_buf;

	chip->select_chip = hisfc400_select_chip;

	chip->cmd_ctrl    = hisfc400_cmd_ctrl;
	chip->dev_ready   = hisfc400_dev_ready;

	chip->chip_delay  = HISFC400_CHIP_DELAY;

	chip->options	  = NAND_NO_AUTOINCR | NAND_SKIP_BBTSCAN;

	chip->ecc.layout  = NULL;
	chip->ecc.mode    = NAND_ECC_NONE;
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

	hisfc_nand_param_adjust = hisfc400_ecc_probe;

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
int hisfc400_init(struct nand_chip *chip)
{
	size_t size;

	if (host.chip)
		return 0;

	printk("spiflash: Check Spi-Nand Flash Controller V400. ");
	if (!hisfc400_version_check()) {
		printk("\n");
		return -ENODEV;
	}
	printk("Found\n");

	memset((char *)&host, 0, sizeof(struct hisfc_host));

	host.regbase = (void __iomem *)CONFIG_HISFC400_REG_BASE_ADDRESS;
	host.iobase = chip->IO_ADDR_R = chip->IO_ADDR_W =
		(void __iomem *)CONFIG_HISFC400_BUFFER_BASE_ADDRESS;

	memset((char *)host.iobase, 0xff, HISFC400_BUFFER_BASE_ADDRESS_LEN);
	
	host.sysreg = (void __iomem *)CONFIG_HISFC400_PERIPHERY_REGBASE;

	size = SPI_NAND_MAX_PAGESIZE + SPI_NAND_MAX_OOBSIZE;
	host.buffer = kmalloc(size, GFP_KERNEL);
	if (!host.buffer) {
		DBG_BUG("Can't malloc memory for SPI nand driver.");
		return -ENOMEM;
	}

	memset(host.buffer, 0xff, size); 
	host.dma_buffer = (unsigned int)host.buffer;
	
	hisfc400_host_init(&host);

	host.chip = chip;
	chip->priv = &host;
	
	hisfc400_nand_init(chip);

	hisfc_nand_spl_ids_register();

	return 0;
}
