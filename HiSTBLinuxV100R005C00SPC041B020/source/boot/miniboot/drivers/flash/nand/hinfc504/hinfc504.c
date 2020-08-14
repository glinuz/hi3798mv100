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

extern void hinfc504_controller_enable(struct hinfc_host *host, int enable);
extern struct read_retry_t *hinfc504_get_read_retry(int type);
extern struct nand_ctrl_info_t *hinfc504_detect_ecc(struct nand_chip_info_t *nand_info);
extern struct nand_ctrl_info_t *hinfc504_force_ecc(struct nand_chip_info_t *nand_info,
						   int pagesize, int ecctype,
						   char *msg, int pagediv);

/*****************************************************************************/

static void hinfc504_dma_transfer(struct hinfc_host *host, int todev)
{
	unsigned long reg_val;
	unsigned int dma_addr = (unsigned int)host->dma_buffer;

	hinfc_write(host, HINFC504_INTCLR_UE | HINFC504_INTCLR_CE,
		HINFC504_INTCLR);

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC504_ADDRL);
	hinfc_write(host, host->addr_value[1], HINFC504_ADDRH);

	hinfc_write(host, dma_addr, HINFC504_DMA_ADDR_DATA);

	dma_addr += HINFC504_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC504_DMA_ADDR_DATA1);

	dma_addr += HINFC504_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC504_DMA_ADDR_DATA2);

	dma_addr += HINFC504_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC504_DMA_ADDR_DATA3);

	hinfc_write(host, host->dma_oob, HINFC504_DMA_ADDR_OOB);

	if (host->ecctype == NAND_ECC_NONE) {
		hinfc_write(host,
			((host->oobsize & HINFC504_DMA_LEN_OOB_MASK)
				<< HINFC504_DMA_LEN_OOB_SHIFT),
			HINFC504_DMA_LEN);

		hinfc_write(host,
			HINFC504_DMA_PARA_DATA_RW_EN
			| HINFC504_DMA_PARA_OOB_RW_EN
			| HINFC504_SET_24BIT_EXT(host),
			HINFC504_DMA_PARA);
	} else
		hinfc_write(host,
			HINFC504_DMA_PARA_DATA_RW_EN
			| HINFC504_DMA_PARA_OOB_RW_EN
			| HINFC504_DMA_PARA_DATA_EDC_EN
			| HINFC504_DMA_PARA_OOB_EDC_EN
			| HINFC504_DMA_PARA_DATA_ECC_EN
			| HINFC504_DMA_PARA_OOB_ECC_EN
			| HINFC504_SET_24BIT_EXT(host),
			HINFC504_DMA_PARA);


	reg_val = (HINFC504_DMA_CTRL_DMA_START
		| HINFC504_DMA_CTRL_BURST4_EN
		| HINFC504_DMA_CTRL_BURST8_EN
		| HINFC504_DMA_CTRL_BURST16_EN
		| HINFC504_DMA_CTRL_DATA_AREA_EN
		| HINFC504_DMA_CTRL_OOB_AREA_EN
		| ((host->addr_cycle == 4 ? 1 : 0)
		   << HINFC504_DMA_CTRL_ADDR_NUM_SHIFT)
		| ((host->chipselect & HINFC504_DMA_CTRL_CS_MASK)
		   << HINFC504_DMA_CTRL_CS_SHIFT));

	if (todev) { /* write */
		reg_val |= HINFC504_DMA_CTRL_WE;
		hinfc_write(host, NAND_CMD_PAGEPROG << 8 | NAND_CMD_SEQIN,
			    HINFC504_CMD);
	} else { /* read  */
		hinfc_write(host, NAND_CMD_READSTART << 8 | NAND_CMD_READ0,
			    HINFC504_CMD);
		hinfc_write(host, 0, HINFC504_LOG_READ_ADDR);
		hinfc_write(host, (host->pagesize + host->oobsize),
			    HINFC504_LOG_READ_LEN);
	}

	hinfc_write(host, reg_val, HINFC504_DMA_CTRL);

	do {
		unsigned int timeout = 0x10000;
		while ((hinfc_read(host, HINFC504_DMA_CTRL))
			& HINFC504_DMA_CTRL_DMA_START && timeout)
				timeout--;
		if (!timeout) {
			HINFC504_DUMP(host);
			DBG_BUG("NAND Wait DMA finish timeout.\n");
		}
	} while (0);
}
/*****************************************************************************/

static int hinfc504_send_cmd_pageprog(struct hinfc_host *host)
{
	if (*host->bbm != 0xFF && *host->bbm != 0x00)
		printf("WARNING: attempt to write an invalid bbm. "
		       "page: 0x%08x, mark: 0x%02x,\n",
		       GET_PAGE_INDEX(host), *host->bbm);

	*host->epm = host->epmvalue;

	hinfc504_dma_transfer(host, TRUE);

	return 0;
}
/*****************************************************************************/
#define NAND_BAD_BLOCK              1
#define NAND_EMPTY_PAGE             2
#define NAND_VALID_DATA             3

static int hinfc504_get_data_status(struct hinfc_host *host)
{
	/* this is block start address */
	if (!((host->addr_value[0] >> 16) & host->block_page_mask)) {

		/* it is a bad block */
		if (*host->bbm == 0)
			return NAND_BAD_BLOCK;
		/*
		 * if there are more than 2 bits flipping, it is
		 * maybe a bad block
		 */
		if (host->uc_er && *host->bbm != 0xFF &&
		    bits_count(*host->bbm) <= 6)
			return NAND_BAD_BLOCK;
	}

	/* it is an empty page */
	if (*(host->epm) != host->epmvalue && IS_NAND_RANDOM(host))
		return NAND_EMPTY_PAGE;

	return NAND_VALID_DATA;
}
/*****************************************************************************/

static int hinfc504_do_read_retry(struct hinfc_host *host)
{
	int ix;

	for (ix = 1; host->uc_er && ix < host->read_retry->count; ix++) {
		hinfc_write(host, HINFC504_INTCLR_UE | HINFC504_INTCLR_CE,
			HINFC504_INTCLR);

		host->read_retry->set_rr_param(host, ix);

		hinfc504_dma_transfer(host, 0);

		host->uc_er = (hinfc_read(host, HINFC504_INTS)
				& HINFC504_INTS_UE) ? 1 : 0;
	}

#ifdef CONFIG_NAND_NOTICE_RR
	printf("\nPage %d Do RR(%d/%d) %s.\n", GET_PAGE_INDEX(host),
	       ix-1, host->read_retry->count, (host->uc_er ? "Fail" : "Success"));
#endif

	host->read_retry->set_rr_param(host, 0);

	return 0;
}
/*****************************************************************************/

static int hinfc504_send_cmd_readstart(struct hinfc_host *host)
{
	hinfc504_dma_transfer(host, 0);

	host->uc_er = (hinfc_read(host, HINFC504_INTS)
			& HINFC504_INTS_UE) ? 1 : 0;

	if (host->read_retry || IS_NAND_RANDOM(host)) {

		int status = hinfc504_get_data_status(host);
		if (status == NAND_EMPTY_PAGE) {
			/*
			 * oob area used by yaffs2 only 32 bytes,
			 * so we only fill 32 bytes.
			 */
			if (IS_NAND_RANDOM(host))
				memset(host->buffer, 0xFF,
				       host->pagesize + host->oobsize);
			host->uc_er = 2;

		} else if (status == NAND_VALID_DATA) {

			/* if NAND chip support read retry */
			if (host->uc_er && host->read_retry)
				hinfc504_do_read_retry(host);

		} /* 'else' NAND have a bad block, do nothing. */
	}

	return 0;
}
/*****************************************************************************/

static int hinfc504_send_cmd_erase(struct hinfc_host *host)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, host->addr_value[0], HINFC504_ADDRL);
	hinfc_write(host, (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1,
		    HINFC504_CMD);

	hinfc_write(host,
		HINFC504_OP_WAIT_READY_EN
		| HINFC504_OP_CMD2_EN
		| HINFC504_OP_CMD1_EN
		| HINFC504_OP_ADDR_EN
		| HINFC504_OP_CS(host)
		| HINFC504_OP_ADDR_CYCLE(host),
		HINFC504_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc504_send_cmd_readid(struct hinfc_host *host)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC504_NANDINFO_LEN, HINFC504_DATA_NUM);
	hinfc_write(host, NAND_CMD_READID, HINFC504_CMD);
	hinfc_write(host, 0, HINFC504_ADDRL);
	host->addr_cycle = 1;

	hinfc_write(host,
		HINFC504_OP_CMD1_EN
		| HINFC504_OP_ADDR_EN
		| HINFC504_OP_READ_DATA_EN
		| HINFC504_OP_WAIT_READY_EN
		| HINFC504_OP_CS(host)
		| HINFC504_OP_ADDR_CYCLE(host),
		HINFC504_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc504_set_ecc_randomizer(struct hinfc_host *host,
				       int ecc_en, int randomizer_en)
{
	unsigned int regval;

	if (IS_NAND_RANDOM(host)) {
		regval = hinfc_read(host, HINFC504_RANDOMIZER);
		if (randomizer_en)
			regval |= HINFC504_RANDOMIZER_ENABLE;
		else
			regval &= ~HINFC504_RANDOMIZER_ENABLE;
		hinfc_write(host, regval, HINFC504_RANDOMIZER);
	}

	regval = host->NFC_CON;
	if (!ecc_en)
		regval = HINFC504_CLEAR_ECC(regval);

	hinfc_write(host, regval, HINFC504_CON);

	return 0;
}
/*****************************************************************************/

static int hinfc600_set_ecc_randomizer(struct hinfc_host *host,
				       int ecc_en, int randomizer_en)
{
	unsigned int nfc_con = host->NFC_CON;

	if (IS_NAND_RANDOM(host)) {
		if (randomizer_en)
			nfc_con |= HINFC600_CON_RANDOMIZER_EN;
		else
			nfc_con &= ~HINFC600_CON_RANDOMIZER_EN;
	}

	if (!ecc_en)
		nfc_con = HINFC504_CLEAR_ECC(nfc_con);

	hinfc_write(host, nfc_con, HINFC600_CON);

	return 0;
}
/*****************************************************************************/

static int hinfc504_send_cmd_status(struct hinfc_host *host)
{
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC504_NANDINFO_LEN, HINFC504_DATA_NUM);
	hinfc_write(host, NAND_CMD_STATUS, HINFC504_CMD);
	hinfc_write(host,
		HINFC504_OP_CMD1_EN
		| HINFC504_OP_READ_DATA_EN
		| HINFC504_OP_WAIT_READY_EN
		| HINFC504_OP_CS(host),
		HINFC504_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc504_send_cmd_reset(struct hinfc_host *host, int chipselect)
{
	hinfc_write(host,
		NAND_CMD_RESET, HINFC504_CMD);

	hinfc_write(host,
		(HINFC504_OP_CMD1_EN
		| HINFC504_OP_CS(host)
		| HINFC504_OP_WAIT_READY_EN),
		HINFC504_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc504_params_adjust(struct nand_dev_t *nand_dev)
{
	struct nand_ctrl_info_t *best = NULL;
	struct nand_chip_info_t *nand_info = nand_dev->nand_info;
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

#ifdef CONFIG_HINFC504_AUTO_PAGESIZE_ECC
	best = hinfc504_detect_ecc(nand_info);
	nand_dev->bootmsg = "Auto";
#endif

#ifdef CONFIG_HINFC504_HARDWARE_PAGESIZE_ECC
	best = hinfc504_force_ecc(nand_info, hinfc504_get_pagesize(host),
				  hinfc504_get_ecctype(host),
				  "hardware config", 0);
	nand_dev->bootmsg = "Hardware";

#endif /* CONFIG_HINFC504_HARDWARE_PAGESIZE_ECC */

#ifdef CONFIG_HINFC504_PAGESIZE_AUTO_ECC_NONE
	best = hinfc504_force_ecc(nand_info, nand_info->pagesize,
				  NAND_ECC_NONE, "force config", 0);
	nand_dev->bootmsg = "AutoForce";
#endif /* CONFIG_HINFC504_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
		DBG_BUG(ERSTR_HARDWARE
			"Please configure Nand Flash pagesize and ecctype!\n");

	if (IS_NAND_RANDOM(nand_dev) && !(IS_NAND_RANDOM(host)))
		DBG_BUG("Hardware is not configure randomizer, "
			"but it is more suitable for this Nand Flash. "
			"Modify hardware pin will resolve this problem.\n");

	if (best->ecctype != NAND_ECC_NONE)
		nand_dev->oobsize = best->oobsize;

	nand_dev->oobused = nand_dev->oobsize;
	nand_dev->layout = best->layout;
	nand_dev->ecctype = best->ecctype;
	nand_dev->pagesize = best->pagesize;

	host->pagesize = best->pagesize;
	host->oobsize  = nand_dev->oobsize;
	host->ecctype  = best->ecctype;
	host->block_page_mask = (nand_info->erasesize / nand_info->pagesize) - 1;

	if (host->ecctype == NAND_ECC_24BIT) {
		if (host->pagesize == _4K)
			host->n24bit_ext_len = 0x03; /* 8bytes; */
		else if (host->pagesize == _8K)
			host->n24bit_ext_len = 0x01; /* 4bytes; */
	}

	hinfc504_set_ecctype(host, best->ecctype);
	hinfc504_set_pagesize(host, best->pagesize);

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	host->dma_oob = host->dma_buffer + host->pagesize;
	host->oobbuf = host->buffer + host->pagesize;
	host->bbm = (unsigned char *)(host->buffer
		+ host->pagesize + HINFC_BAD_BLOCK_POS);
	host->epm = (unsigned short *)(host->buffer
		+ host->pagesize + best->layout->offset + 28);

	nand_dev->bbm = host->bbm;
	nand_dev->pagebuf = host->buffer;
	nand_dev->oobbuf = host->oobbuf;

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

	host->flags |= IS_NAND_RANDOM(nand_dev);

	host->read_retry = NULL;
	if (nand_info->read_retry_type != NAND_RR_NONE) {
		host->read_retry = hinfc504_get_read_retry(nand_info->read_retry_type);

		if (!host->read_retry) {
			DBG_BUG(ERSTR_DRIVER
			        "This Nand Flash need to enable the 'read retry' feature. "
			        "but the driver dose not offer the feature");
		}
	}

	/*
	 * If it want to support the 'read retry' feature, the 'randomizer'
	 * feature must be support first.
	 */
	if (host->read_retry && !IS_NAND_RANDOM(host)) {
		DBG_BUG(ERSTR_HARDWARE
		        "This Nand flash need to enable 'randomizer' feature. "
			"Please configure hardware randomizer PIN.");
	}

	/*
	 * Check if hardware enable randomizer PIN, But NAND no need randomizer.
	 * We will notice user.
	 */
	if (IS_NAND_RANDOM(host)) {
		while (HINFC_VER_505 == host->version) {

			if (host->pagesize == _4K
			    && host->ecctype == NAND_ECC_24BIT)
				break;

			if (host->pagesize == _8K
			    && (host->ecctype == NAND_ECC_24BIT
			        || host->ecctype == NAND_ECC_40BIT))
				break;

			if (host->pagesize == _16K
			    && host->ecctype == NAND_ECC_40BIT)
				break;

			DBG_BUG(ERSTR_HARDWARE
			        "This NAND flash does not support `randomizer`, "
			        "Please don't configure hardware randomizer PIN.");
		}
	}

	return 0;
}
/*****************************************************************************/

static int hinfc504_get_randomizer(struct hinfc_host *host)
{
	unsigned int regval;

	regval = hinfc_read(host, HINFC504_RANDOMIZER);
	if (regval & HINFC504_RANDOMIZER_PAD)
		host->flags |= NAND_RANDOMIZER;

	regval |= (IS_NAND_RANDOM(host) ? HINFC504_RANDOMIZER_ENABLE : 0);
	hinfc_write(host, regval, HINFC504_RANDOMIZER);

	return IS_NAND_RANDOM(host);
}
/*****************************************************************************/

static int hinfc600_get_randomizer(struct hinfc_host *host)
{
	unsigned int regval;

	regval = hinfc_read(host, HINFC600_BOOT_CFG);
	if (regval & HINFC600_BOOT_CFG_RANDOMIZER_PAD)
		host->flags |= NAND_RANDOMIZER;

	return IS_NAND_RANDOM(host);
}
/*****************************************************************************/

static int hinfc504_version_check(void)
{
	// TODO: 需要完善此函数
	return 1;
}
/*****************************************************************************/
/*
 *  return:
 *     1   - write/erase/reset fail.
 *     0   - write/erase/reset pass.
 *    -1   - write/erase/reset wait RB timeout.
 */
static int hinfc504_wait_ready(struct hinfc_host *host)
{
	uint8 *iobase = (uint8 *)host->iobase;
	int timeout = 0x10000;

	do {
		*iobase = 0;
		hinfc504_send_cmd_status(host);
	} while (!(*iobase & NAND_STATUS_READY) && timeout--);

	if (!timeout) {
		DBG_BUG("Wait controll reset status timeout.\n");
		return -1;
	}

	return (*iobase & NAND_STATUS_FAIL);
}
/*****************************************************************************/

static void hinfc504_set_address(struct nand_dev_t *nand_dev, uint64 address,
				 int only_row)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

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

static int hinfc504_read_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else {
		host->chipselect = 0;
	}

	hinfc504_set_address(nand_dev, address, FALSE);
	hinfc504_send_cmd_readstart(host);

	hinfc504_wait_ready(host);

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *     0 - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc504_write_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 0;

	hinfc504_set_address(nand_dev, address, FALSE);
	hinfc504_send_cmd_pageprog(host);

	return hinfc504_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc504_erase_block(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->blockmask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 0;

	hinfc504_set_address(nand_dev, address, TRUE);
	hinfc504_send_cmd_erase(host);

	return hinfc504_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc504_reset(struct nand_dev_t *nand_dev, int chipselect)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	host->chipselect = chipselect;

	hinfc504_send_cmd_reset(host, host->chipselect);

	return hinfc504_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/

static int hinfc504_read_id(struct nand_dev_t *nand_dev, int chipselect,
			    char *buf, int buflen)
{
	uint8 *iobase;
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	iobase = (uint8 *)host->iobase;

	host->chipselect = chipselect;

	memset(iobase, 0, HINFC504_NANDINFO_LEN);
	hinfc504_send_cmd_readid(host);

	buflen = min(buflen, HINFC504_NANDINFO_LEN);
	memcpy(buf, iobase, buflen);

	return 0;
}
/*****************************************************************************/

int hinfc504_init(struct hinfc_host *host)
{
	printf("Check NAND controller version 504...");
	if (!hinfc504_version_check()) {
		printf("\n");
		return;
	}
	printf("found.\n");
}
/*****************************************************************************/

static struct hinfc_host hinfc504_host;

static struct nand_dev_t hinfc504_nand_dev = {
	.bootmsg = "Unknown",
	.read_page = hinfc504_read_page,
	.write_page = hinfc504_write_page,
	.erase_block = hinfc504_erase_block,
	.reset = hinfc504_reset,
	.read_id = hinfc504_read_id,
	.adjust_param = hinfc504_params_adjust,
	.priv = (void *)&hinfc504_host,
};

static struct nand_dev_t *hinfc504_driver_init(void)
{
	struct hinfc_host *host;
	static struct nand_dev_t *nand_dev = NULL;

	if (nand_dev)
		return nand_dev;

	nand_dev = &hinfc504_nand_dev;
	host = (struct hinfc_host *)nand_dev->priv;

	memset((char *)host, 0, sizeof(struct hinfc_host));
	host->regbase = (void *)CONFIG_HINFC504_REG_BASE_ADDRESS;
	host->iobase = (void *)CONFIG_HINFC504_BUFFER_BASE_ADDRESS;

	hinfc504_controller_enable(host, ENABLE);

	host->addr_cycle    = 0;
	host->addr_value[0] = 0;
	host->addr_value[1] = 0;
	host->cache_addr_value[0] = (unsigned int)~0;
	host->cache_addr_value[1] = (unsigned int)~0;
	host->chipselect    = 0;

	host->send_cmd_pageprog = hinfc504_send_cmd_pageprog;
	host->send_cmd_readstart = hinfc504_send_cmd_readstart;
	host->send_cmd_erase = hinfc504_send_cmd_erase;
	host->send_cmd_readid = hinfc504_send_cmd_readid;
	host->send_cmd_status = hinfc504_send_cmd_status;
	host->send_cmd_reset = hinfc504_send_cmd_reset;

	host->NFC_CON = (hinfc_read(host, HINFC504_CON)
		| HINFC504_CON_OP_MODE_NORMAL
		| HINFC504_CON_READY_BUSY_SEL);

	host->buffer = devmemalign(HINFC504_DMA_ALIGN, NAND_MAX_PAGESIZE
		+ NAND_MAX_OOBSIZE);
	if (!host->buffer) {
		DBG_BUG(ERSTR_DRIVER
			"Can't malloc memory for NAND driver.");
		return NULL;
	}
	host->dma_buffer = (unsigned int)host->buffer;

	memset(host->iobase, 0xff, HINFC504_BUFFER_BASE_ADDRESS_LEN);
	memset(host->buffer, 0xff, (NAND_MAX_PAGESIZE + NAND_MAX_OOBSIZE));

	hinfc_write(host,
		SET_HINFC504_PWIDTH(CONFIG_HINFC504_W_LATCH,
		CONFIG_HINFC504_R_LATCH,
		CONFIG_HINFC504_RW_LATCH),
		HINFC504_PWIDTH);

	host->version = hinfc_read(host, HINFC504_VERSION);
	if (HINFC_VER_600 == host->version) {
		hinfc600_get_randomizer(host);
		host->set_ecc_randomizer = hinfc600_set_ecc_randomizer;
		host->epmvalue = 0x0000;
	} else {
		hinfc504_get_randomizer(host);
		host->set_ecc_randomizer = hinfc504_set_ecc_randomizer;
		host->epmvalue = 0xffff;
	}

	hinfc504_init(host);

	return nand_dev;
}
/*****************************************************************************/

static void hinfc504_shutdown(struct hinfc_host *host)
{
	/* pass pagesize&ecctype to kernel */
	host->set_ecc_randomizer(host, ENABLE, ENABLE);
}
/*****************************************************************************/

static void hinfc504_module_exit(void *ptr)
{
	struct hinfc_host *host = (struct hinfc_host *)ptr;

	hinfc504_shutdown(host);
}
/*****************************************************************************/

static void hinfc504_module_init(void *ptr)
{
	nand_driver_init = hinfc504_driver_init;
}
/*****************************************************************************/

static struct module_t module_hinfc504 = {
	.priority = MODULE_PRI_DRV,
	.ptr = &hinfc504_host,
	.init = hinfc504_module_init,
	.exit = hinfc504_module_exit,
};
MODULE(module_hinfc504);
