/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
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
#include <cpuinfo.h>
#include <debug.h>

#include "nand_drv.h"
#include "hinfc610_gen.h"
#include "hinfc620_gen.h"
#include "hinfc610.h"
#include "hinfc610_read_retry.h"
#include "hinfc610_sync.h"
/*****************************************************************************/

extern void hinfc610_controller_enable(struct hinfc_host *host, int enable);

/*****************************************************************************/

#if defined(CONFIG_ARCH_S40) || defined(CONFIG_ARCH_S5) || defined(CONFIG_ARCH_S40V3) || defined(CONFIG_ARCH_HI3798MX)
#  include "hinfc610_s40.c"
#endif

#if defined(CONFIG_ARCH_HI3716MV310)
#  include "hinfc610_hi3716mv310.c"
#endif

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
/*****************************************************************************/

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

static void nand_register_dump(void)
{
	int ix;
	unsigned int regbase = CONFIG_HINFC610_REG_BASE_ADDRESS;

	printf("Register dump:");
	for (ix = 0; ix <= 0x98; ix += 0x04) {
		if (!(ix & 0x0F))
			printf("\n0x%08X: ", (regbase + ix));
		printf("%08X ", readl(regbase + ix));
	}
	printf("\n");
}
/*****************************************************************************/

static void hinfc610_dma_transfer(struct hinfc_host *host, int todev)
{
	unsigned long reg_val;
	unsigned int dma_addr = (unsigned int)host->dma_buffer;

	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA1);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA2);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA3);

	/* 32K PAGESIZE need below. */
	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA4);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA5);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA6);

	dma_addr += HINFC610_DMA_ADDR_OFFSET;
	hinfc_write(host, dma_addr, HINFC610_DMA_ADDR_DATA7);

	hinfc_write(host, host->dma_oob, HINFC610_DMA_ADDR_OOB);

	if (host->ecctype == NAND_ECC_NONE) {
		hinfc_write(host,
			((host->oobsize & HINFC610_DMA_LEN_OOB_MASK)
			  << HINFC610_DMA_LEN_OOB_SHIFT),
			HINFC610_DMA_LEN);

		hinfc_write(host,
			HINFC610_DMA_PARA_DATA_RW_EN
			| HINFC610_DMA_PARA_OOB_RW_EN,
			HINFC610_DMA_PARA);
	} else
		hinfc_write(host,
			HINFC610_DMA_PARA_DATA_RW_EN
			| HINFC610_DMA_PARA_OOB_RW_EN
			| HINFC610_DMA_PARA_DATA_EDC_EN
			| HINFC610_DMA_PARA_OOB_EDC_EN,
			HINFC610_DMA_PARA);

	reg_val = (HINFC610_DMA_CTRL_DMA_START
		| HINFC610_DMA_CTRL_BURST4_EN
		| HINFC610_DMA_CTRL_BURST8_EN
		| HINFC610_DMA_CTRL_BURST16_EN
		| ((host->addr_cycle == 4 ? 1 : 0)
		   << HINFC610_DMA_CTRL_ADDR_NUM_SHIFT)
		| ((host->chipselect & HINFC610_DMA_CTRL_CS_MASK)
		   << HINFC610_DMA_CTRL_CS_SHIFT));

	if (todev)
		reg_val |= HINFC610_DMA_CTRL_WE;

	hinfc_write(host, reg_val, HINFC610_DMA_CTRL);

	do {
		unsigned int timeout = 0xF0000000;
		while ((hinfc_read(host, HINFC610_DMA_CTRL))
			& HINFC610_DMA_CTRL_DMA_START && timeout)
			timeout--;
		if (!timeout)
			PR_BUG("Wait DMA finish timeout.\n");
	} while (0);
}
/*****************************************************************************/

static void hinfc610_sync_entry(struct hinfc_host *host)
{
	struct nand_sync *sync = host->sync;

	if (!sync) {
		PR_BUG("this NAND not support sync feature.\n");
		return;
	}

	if (HINFC610_IS_SYNC(host)) {
		PR_BUG("Current already entry sync feature.\n");
		return;
	}

	if (sync->enable)
		sync->enable(host);

	hinfc610_sync_clock(host, ENABLE);

	switch (sync->type) {
	case NAND_TYPE_TOGGLE_10:
		host->NFC_CON |= HINFC610_CON_NF_MODE_TOGGLE;
		host->NFC_CON_ECC_NONE |= HINFC610_CON_NF_MODE_TOGGLE;
		break;

	case NAND_TYPE_ONFI_23:
		host->NFC_CON |= HINFC610_CON_NF_MODE_ONFI_23;
		host->NFC_CON_ECC_NONE |= HINFC610_CON_NF_MODE_ONFI_23;
		break;

	case NAND_TYPE_ONFI_30:
		host->NFC_CON |= HINFC610_CON_NF_MODE_ONFI_30;
		host->NFC_CON_ECC_NONE |= HINFC610_CON_NF_MODE_ONFI_30;
		break;

	default:
		PR_BUG("Unsupport sync type 0x%08X.\n", sync->type);
		break;
	}
}
/*****************************************************************************/

static void hinfc610_sync_exit(struct hinfc_host *host)
{
	struct nand_sync *sync = host->sync;

	if (!HINFC610_IS_SYNC(host)) {
		PR_BUG("Current already exit from sync feature.\n");
		return;
	}

	if (sync->disable)
		sync->disable(host);

	host->NFC_CON &= ~HINFC610_CON_NF_MODE_MASK;
	host->NFC_CON_ECC_NONE &= ~HINFC610_CON_NF_MODE_MASK;

	hinfc610_sync_clock(host, DISABLE);
}
/*****************************************************************************/

static int hinfc610_send_cmd_pageprog(struct hinfc_host *host)
{
	if (*host->bbm != 0xFF && *host->bbm != 0x00)
		printf("WARNING: attempt to write an invalid bbm. "
		       "page: 0x%08x, mark: 0x%02x,\n",
		       GET_PAGE_INDEX(host), *host->bbm);

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_entry(host);

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC610_ADDRL);
	hinfc_write(host, host->addr_value[1], HINFC610_ADDRH);
	hinfc_write(host,
		((NAND_CMD_STATUS << 16) | (NAND_CMD_PAGEPROG << 8) |
		 NAND_CMD_SEQIN),
		HINFC610_CMD);

	*host->epm = 0x0000;

	hinfc610_dma_transfer(host, 1);

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_exit(host);

	return 0;
}
/*****************************************************************************/

static int hinfc610_get_data_status(struct hinfc_host *host)
{
	unsigned int page_status = 0;

	if (IS_PS_UN_ECC(host))
		page_status = HINFC610_PS_UC_ECC;

	/* this is block start address */
	if (!((host->addr_value[0] >> 16) & host->block_page_mask)) {

		/* it is a bad block */
		if (*host->bbm == 0x00) {
			page_status |= HINFC610_PS_BAD_BLOCK;
			goto out;
		}

		if (*host->bbm != 0xFF) {
			page_status |= HINFC610_PS_BBM_ERROR;

			/*
			 * if there are more than 2 bits flipping, it is
			 * maybe a bad block
			 */
			if (!IS_PS_UN_ECC(host) || get_8bits(*host->bbm) < 6) {
				page_status |= HINFC610_PS_BAD_BLOCK;
				goto out;
			}
		}
	}

	if (*host->epm == 0x0000)
		goto out;

	if (*host->epm == 0xFFFF) {
		page_status |= HINFC610_PS_EMPTY_PAGE;
		goto out;
	}

	page_status |= HINFC610_PS_EPM_ERROR;

	if (IS_PS_UN_ECC(host) && get_16bits(*host->epm) > 12) {
		page_status |= HINFC610_PS_EMPTY_PAGE;
		goto out;
	}

out:
	return page_status;
}
/*****************************************************************************/

static int hinfc610_do_read_retry(struct hinfc_host *host)
{
	int ix;

	for (ix = 1; IS_PS_UN_ECC(host) && ix < host->read_retry->count; ix++) {

		hinfc_write(host, HINFC610_INTCLR_UE | HINFC610_INTCLR_CE,
			    HINFC610_INTCLR);

		host->set_ecc_randomizer(host, DISABLE, DISABLE);
		host->read_retry->set_rr_param(host, ix);

		/* enable ecc and randomizer */
		host->set_ecc_randomizer(host, ENABLE, ENABLE);

		hinfc_write(host, HINFC610_INTCLR_UE | HINFC610_INTCLR_CE,
			    HINFC610_INTCLR);
		// TODO: NEED UPDATE
		hinfc_write(host, host->NFC_CON, HINFC610_CON);
		hinfc_write(host, host->addr_value[0] & 0xffff0000,
			    HINFC610_ADDRL);
		hinfc_write(host, host->addr_value[1], HINFC610_ADDRH);
		hinfc_write(host,
			    HINFC_CMD_SEQ(NAND_CMD_READ0, NAND_CMD_READSTART),
			    HINFC610_CMD);

		hinfc610_dma_transfer(host, 0);

		if (hinfc_read(host, HINFC610_INTS) & HINFC610_INTS_UE)
			host->page_status |= HINFC610_PS_UC_ECC;
		else
			host->page_status &= ~HINFC610_PS_UC_ECC;
	}

#ifdef CONFIG_NAND_NOTICE_RR
	printf("\nPage %d Do RR(%d/%d) %s.\n", GET_PAGE_INDEX(host),
	       ix-1, host->read_retry->count, (IS_PS_UN_ECC(host) ? "Fail" : "Success"));
#endif

	host->set_ecc_randomizer(host, DISABLE, DISABLE);
	host->read_retry->reset_rr_param(host);

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_readstart(struct hinfc_host *host)
{
	if ((host->addr_value[0] == host->cache_addr_value[0]) &&
	    (host->addr_value[1] == host->cache_addr_value[1]))
		return 0;

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_entry(host);

	host->page_status = 0;

	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	hinfc_write(host, HINFC610_INTCLR_UE | HINFC610_INTCLR_CE,
		    HINFC610_INTCLR);
	hinfc_write(host, host->NFC_CON, HINFC610_CON);
	hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC610_ADDRL);
	hinfc_write(host, host->addr_value[1], HINFC610_ADDRH);
	hinfc_write(host, NAND_CMD_READSTART << 8 | NAND_CMD_READ0,
		    HINFC610_CMD);

	hinfc610_dma_transfer(host, 0);

	if (hinfc_read(host, HINFC610_INTS) & HINFC610_INTS_UE)
		host->page_status |= HINFC610_PS_UC_ECC;

	if (host->read_retry || IS_NAND_RANDOM(host)) {
		host->page_status |= hinfc610_get_data_status(host);

		if (IS_PS_EMPTY_PAGE(host)) {
			/*
			 * oob area used by yaffs2 only 32 bytes,
			 * so we only fill 32 bytes.
			 */
			if (IS_NAND_RANDOM(host))
				memset(host->buffer, 0xFF,
				       host->pagesize + host->oobsize);

		} else if (!IS_PS_BAD_BLOCK(host)) {
			/* if NAND chip support read retry */
			if (IS_PS_UN_ECC(host) && host->read_retry)
				hinfc610_do_read_retry(host);

		} /* 'else' NAND have a bad block, do nothing. */
	}

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_exit(host);

	host->cache_addr_value[0] = host->addr_value[0];
	host->cache_addr_value[1] = host->addr_value[1];

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_erase(struct hinfc_host *host)
{
	unsigned int regval;

	/* Don't case the read retry config */
	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, host->addr_value[0], HINFC610_ADDRL);
	hinfc_write(host, (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1,
		HINFC610_CMD);

	/* need to config WAIT_READY_EN */
	regval = HINFC610_OP_WAIT_READY_EN
		 | HINFC610_OP_CMD2_EN
		 | HINFC610_OP_CMD1_EN
		 | HINFC610_OP_ADDR_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | ((host->addr_cycle & HINFC610_OP_ADDR_CYCLE_MASK)
		    << HINFC610_OP_ADDR_CYCLE_SHIFT);

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_sync_readid(struct hinfc_host *host)
{
	unsigned int regval;

	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC610_NANDINFO_LEN, HINFC610_DATA_NUM);
	hinfc_write(host, NAND_CMD_READID, HINFC610_CMD);
	hinfc_write(host, 0, HINFC610_ADDRL);

	/* no need to config HINFC610_OP_WAIT_READY_EN, here not config. */
	regval = HINFC610_OP_CMD1_EN
		 | HINFC610_OP_ADDR_EN
		 | HINFC610_OP_READ_DATA_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT)
		 | HINFC610_OP_READID_EN
		 | HINFC610_OP_RW_REG_EN;

	hinfc_write(host, regval, HINFC610_OP);

	host->addr_cycle = 0x0;

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_async_readid(struct hinfc_host *host)
{
	unsigned int regval;

	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC610_NANDINFO_LEN, HINFC610_DATA_NUM);
	hinfc_write(host, NAND_CMD_READID, HINFC610_CMD);
	hinfc_write(host, 0, HINFC610_ADDRL);

	/* no need to config HINFC610_OP_WAIT_READY_EN, here not config. */
	regval = HINFC610_OP_CMD1_EN
		 | HINFC610_OP_ADDR_EN
		 | HINFC610_OP_READ_DATA_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT);

	hinfc_write(host, regval, HINFC610_OP);

	host->addr_cycle = 0x0;

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_readid(struct hinfc_host *host)
{
	if (HINFC610_IS_SYNC(host))
		return hinfc610_send_cmd_sync_readid(host);
	else
		return hinfc610_send_cmd_async_readid(host);
}
/*****************************************************************************/

static int hinfc610_set_ecc_randomizer(struct hinfc_host *host, int ecc_en,
					  int randomizer_en)
{
	unsigned int nfc_con;

	if (IS_NAND_RANDOM(host)) {
		if (randomizer_en) {
			host->NFC_CON |= HINFC610_CON_RANDOMIZER_EN;
			host->NFC_CON_ECC_NONE |= HINFC610_CON_RANDOMIZER_EN;
		} else {
			host->NFC_CON &= ~HINFC610_CON_RANDOMIZER_EN;
			host->NFC_CON_ECC_NONE &= ~HINFC610_CON_RANDOMIZER_EN;
		}
	}

	nfc_con = (ecc_en ? host->NFC_CON : host->NFC_CON_ECC_NONE);

	hinfc_write(host, nfc_con, HINFC610_CON);

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_status(struct hinfc_host *host)
{
	unsigned int regval;

	host->set_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC610_NANDINFO_LEN, HINFC610_DATA_NUM);
	hinfc_write(host, NAND_CMD_STATUS, HINFC610_CMD);

	/* no need config HINFC610_OP_WAIT_READY_EN, here not config */
	regval = HINFC610_OP_CMD1_EN
		 | HINFC610_OP_READ_DATA_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT);

 	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_async_reset(struct hinfc_host *host,
					 int chipselect)
{
	unsigned int regval;

	hinfc_write(host, NAND_CMD_RESET, HINFC610_CMD);

	/* need to config HINFC610_OP_WAIT_READY_EN */
	regval = HINFC610_OP_CMD1_EN
		 | ((chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | HINFC610_OP_WAIT_READY_EN;

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_sync_reset(struct hinfc_host *host,
					int chipselect)
{
	unsigned int regval;

	/*
	 * Regarding the ONFI chip sync mode,
	 * NAND_CMD_SYNC_RESET make chip remain sync mode.
	 * But NAND_CMD_RESET will change chip mode to async mode.
	 */
	hinfc_write(host, NAND_CMD_SYNC_RESET, HINFC610_CMD);

	/* need to config HINFC610_OP_WAIT_READY_EN */
	regval = HINFC610_OP_CMD1_EN
		 | ((chipselect & HINFC610_OP_NF_CS_MASK)
		     << HINFC610_OP_NF_CS_SHIFT)
		 | HINFC610_OP_WAIT_READY_EN;

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH();

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_reset(struct hinfc_host *host, int chipselect)
{
	if (HINFC610_IS_SYNC(host))
		return hinfc610_send_cmd_sync_reset(host, chipselect);
	else
		return hinfc610_send_cmd_async_reset(host, chipselect);
}
/*****************************************************************************/
/*
 * 'host->epm' only use the first oobfree[0] field, it looks very simple, But...
 */
static struct nand_ctrl_oob_t nand_ecc_default = {
	2, 30
};

static struct nand_ctrl_oob_t nand_ecc_2k_13bit = {
	2, 14
};
/*****************************************************************************/
/* ecc/pagesize get from CPU PIN. */
static struct nand_ctrl_info_t hinfc610_hw_pin_config_table[] =
{
	{_32K, NAND_ECC_60BIT, 3520/*3520*/, &nand_ecc_default},
	{_32K, NAND_ECC_41BIT, 2368/*2368*/, &nand_ecc_default},
	{_32K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_16K, NAND_ECC_80BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_72BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_41BIT, 1184/*1184*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_8K, NAND_ECC_80BIT, 1152/*1152*/, &nand_ecc_default},
	{_8K, NAND_ECC_72BIT, 1040/*1040*/, &nand_ecc_default},
	{_8K, NAND_ECC_60BIT, 896 /*896*/,  &nand_ecc_default},
	{_8K, NAND_ECC_41BIT, 608 /*608*/,  &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_27BIT, 224 /*224*/,  &nand_ecc_default},
	{_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
	{_4K, NAND_ECC_18BIT, 160 /*160*/,  &nand_ecc_default},
	{_4K, NAND_ECC_13BIT, 112 /*112*/,  &nand_ecc_2k_13bit},
	{_4K, NAND_ECC_8BIT,  88  /*88*/,   &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_2K, NAND_ECC_27BIT, 128 /*128*/, &nand_ecc_default},
	{_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
	{_2K, NAND_ECC_18BIT, 96  /*96*/,  &nand_ecc_default},
	{_2K, NAND_ECC_13BIT, 64  /*64*/,  &nand_ecc_2k_13bit},
	{_2K, NAND_ECC_8BIT,  60  /*60*/,  &nand_ecc_default},
	{_2K, NAND_ECC_NONE,  32,          &nand_ecc_default},

	{0,0,0,NULL},
};

/*****************************************************************************/
/* ecc/pagesize get from NAND controller */
static struct nand_ctrl_info_t hinfc610_hw_auto_config_table[] =
{
	{_32K, NAND_ECC_80BIT, 4544/*4544*/, &nand_ecc_default},
	{_32K, NAND_ECC_60BIT, 3520/*3520*/, &nand_ecc_default},
	{_32K, NAND_ECC_41BIT, 2368/*2368*/, &nand_ecc_default},
	{_32K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_16K, NAND_ECC_80BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_60BIT, 1760/*1760*/, &nand_ecc_default},
	{_16K, NAND_ECC_41BIT, 1184/*1184*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_8K, NAND_ECC_80BIT, 1152/*1152*/, &nand_ecc_default},
	{_8K, NAND_ECC_60BIT, 896 /*896*/,  &nand_ecc_default},
	{_8K, NAND_ECC_41BIT, 608 /*608*/,  &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
	{_4K, NAND_ECC_8BIT,  88 /*88*/,    &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
	{_2K, NAND_ECC_8BIT,  60  /*60*/,  &nand_ecc_default},
	{_2K, NAND_ECC_NONE,  32,          &nand_ecc_default},

	{0,0,0,NULL},
};

/*****************************************************************************/
/* ecc/pagesize get from NAND controller */
static struct nand_ctrl_info_t hinfc620_hw_auto_config_table[] =
{
	{_16K, NAND_ECC_64BIT, 1824/*1824*/, &nand_ecc_default},
	{_16K, NAND_ECC_40BIT, 1152/*1152*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_8K, NAND_ECC_64BIT, 928 /*928*/,  &nand_ecc_default},
	{_8K, NAND_ECC_40BIT, 592 /*592*/,  &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_4K, NAND_ECC_16BIT,  128 /*128*/,  &nand_ecc_default},
#endif
	{_4K, NAND_ECC_8BIT,  88 /*88*/,    &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_2K, NAND_ECC_16BIT,  64 /*64*/, &nand_ecc_default},
#endif
	{_2K, NAND_ECC_8BIT,  60  /*60*/,  &nand_ecc_default},
	{_2K, NAND_ECC_NONE,  32,          &nand_ecc_default},

	{0,0,0,NULL},
};
/*****************************************************************************/
/* ecc/pagesize get from NAND id */
static struct nand_ctrl_info_t hinfc610_soft_auto_config_table[] =
{
	{_32K, NAND_ECC_80BIT, 4544/*4544*/, &nand_ecc_default},
	{_32K, NAND_ECC_72BIT, 4096/*4096*/, &nand_ecc_default},
	{_32K, NAND_ECC_60BIT, 3520/*3520*/, &nand_ecc_default},
	{_32K, NAND_ECC_48BIT, 2752/*2752*/, &nand_ecc_default},
	{_32K, NAND_ECC_41BIT, 2368/*2368*/, &nand_ecc_default},
	{_32K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_16K, NAND_ECC_80BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_72BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_60BIT, 1760/*1760*/, &nand_ecc_default},
	{_16K, NAND_ECC_48BIT, 1376/*1376*/, &nand_ecc_default},
	{_16K, NAND_ECC_41BIT, 1184/*1184*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_8K, NAND_ECC_80BIT, 1152/*1152*/, &nand_ecc_default},
	{_8K, NAND_ECC_72BIT, 1040/*1040*/, &nand_ecc_default},
	{_8K, NAND_ECC_60BIT, 896 /*896*/,  &nand_ecc_default},
	{_8K, NAND_ECC_48BIT, 704 /*704*/,  &nand_ecc_default},
	{_8K, NAND_ECC_41BIT, 608 /*608*/,  &nand_ecc_default},
	{_8K, NAND_ECC_32BIT, 480 /*480*/,  &nand_ecc_default},
	{_8K, NAND_ECC_27BIT, 416 /*416*/,  &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_32BIT, 256 /*256*/,  &nand_ecc_default},
	{_4K, NAND_ECC_27BIT, 224 /*224*/,  &nand_ecc_default},
	{_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
	{_4K, NAND_ECC_18BIT, 160 /*160*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_4K, NAND_ECC_13BIT, 112 /*112*/,  &nand_ecc_2k_13bit},
#endif
	{NAND_PAGE_4K, NAND_ECC_8BIT,  88  /*88*/,   &nand_ecc_default},
	{NAND_PAGE_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

#ifndef CONFIG_SUPPORT_YAFFS
	{_2K, NAND_ECC_32BIT, 128 /*128*/, &nand_ecc_2k_13bit},
#endif
	{_2K, NAND_ECC_27BIT, 128 /*128*/, &nand_ecc_default},
	{_2K, NAND_ECC_24BIT, 116 /*116*/, &nand_ecc_default},
	{_2K, NAND_ECC_18BIT, 96  /*96*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_2K, NAND_ECC_13BIT, 64  /*64*/,  &nand_ecc_2k_13bit},
#endif
	{_2K, NAND_ECC_8BIT,  60  /*60*/,  &nand_ecc_default},
	{_2K, NAND_ECC_NONE,  32,          &nand_ecc_default},

	{0,0,0,NULL},
};
/*****************************************************************************/
/* ecc/pagesize get from NAND id */
static struct nand_ctrl_info_t hinfc620_soft_auto_config_table[] =
{
	{_16K, NAND_ECC_64BIT, 1824/*1824*/, &nand_ecc_default},
	{_16K, NAND_ECC_42BIT, 1216/*1216*/, &nand_ecc_default},
	{_16K, NAND_ECC_40BIT, 1152/*1152*/, &nand_ecc_default},
	{_16K, NAND_ECC_NONE,  32 ,          &nand_ecc_default},

	{_8K, NAND_ECC_64BIT, 928 /*928*/,  &nand_ecc_default},
	{_8K, NAND_ECC_42BIT, 624 /*624*/,  &nand_ecc_default},
	{_8K, NAND_ECC_40BIT, 592 /*592*/,  &nand_ecc_default},
	{_8K, NAND_ECC_28BIT, 432 /*432*/,  &nand_ecc_default},
	{_8K, NAND_ECC_24BIT, 368 /*368*/,  &nand_ecc_default},
	{_8K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_28BIT, 232 /*232*/,  &nand_ecc_default},
	{_4K, NAND_ECC_24BIT, 200 /*200*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_4K, NAND_ECC_16BIT,  128 /*128*/, &nand_ecc_default},
#endif
	{_4K, NAND_ECC_8BIT,  88 /*88*/,    &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_4K, NAND_ECC_28BIT, 132 /*132*/,  &nand_ecc_default},
	{_4K, NAND_ECC_24BIT, 116 /*116*/,  &nand_ecc_default},
#ifndef CONFIG_SUPPORT_YAFFS
	{_4K, NAND_ECC_16BIT,  64 /*64*/,   &nand_ecc_default},
#endif
	{_4K, NAND_ECC_8BIT,  60  /*60*/,   &nand_ecc_default},
	{_4K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{_2K, NAND_ECC_8BIT,  60  /*60*/,   &nand_ecc_default},
	{_2K, NAND_ECC_NONE,  32,           &nand_ecc_default},

	{0,0,0,NULL},
};
/*****************************************************************************/
/*
 *  0 - This NAND NOT support randomizer
 *  1 - This NAND support randomizer.
 */
static int hinfc610_support_randomizer(unsigned int pageisze,
				       unsigned int ecctype)
{
	switch (pageisze) {
	case _8K:
		return (ecctype >= NAND_ECC_24BIT && ecctype <= NAND_ECC_80BIT);
	case _16K:
		return (ecctype >= NAND_ECC_41BIT && ecctype <= NAND_ECC_80BIT);
	case _32K:
		return (ecctype >= NAND_ECC_41BIT && ecctype <= NAND_ECC_80BIT);
	default:
		return 0;
	}
}
/*****************************************************************************/

static struct nand_ctrl_info_t *
hinfc610_get_best_ecc(struct nand_ctrl_info_t *config, 
		      struct nand_chip_info_t *nand_info)
{
	struct nand_ctrl_info_t *best = NULL;

	for (; config->layout; config++) {
		if (config->pagesize != nand_info->pagesize)
			continue;

		if (nand_info->oobsize < config->oobsize)
			continue;

		if (!best || (best->ecctype < config->ecctype))
			best = config;
	}

	if (!best)
		PR_BUG(ERSTR_DRIVER
			"Driver does not support the pagesize(%d) and oobsize(%d).\n",
			nand_info->pagesize, nand_info->oobsize);

	return best;
}
/*****************************************************************************/
/* force the pagesize and ecctype */
struct nand_ctrl_info_t *
hinfc610_force_ecc(struct nand_ctrl_info_t *config, 
		   struct nand_chip_info_t *nand_info, int pagesize,
		   int ecctype, char *cfgmsg, int allow_pagediv)
{
	struct nand_ctrl_info_t *fit = NULL;

	for (; config->layout; config++) {
		if (config->pagesize == pagesize &&
		    config->ecctype == ecctype) {
			fit = config;
			break;
		}
	}

	if (!fit) {
		nand_register_dump();
		PR_BUG(ERSTR_DRIVER
		        "Driver(%s mode) does not support this Nand Flash "
		        "pagesize:0x%x, ecctype:%s\n",
		        cfgmsg,
		        pagesize,
		        nand_ecc_name(ecctype));
		return NULL;
	}

	if ((pagesize != nand_info->pagesize) &&
	    (pagesize > nand_info->pagesize || !allow_pagediv)) {
		nand_register_dump();
		PR_BUG(ERSTR_HARDWARE
		        "Hardware (%s mode) configure pagesize %d, "
		        "but the Nand Flash pageszie is %d\n",
		        cfgmsg,
		        pagesize,
		        nand_info->pagesize);
		return NULL;
	}

	if (fit->oobsize > nand_info->oobsize) {
		PR_BUG(ERSTR_HARDWARE
		        "(%s mode) The Nand Flash offer space area is %d bytes, "
		        "but the controller request %d bytes in ecc %s. "
		        "Please make sure the hardware ECC configuration is correct.",
		        cfgmsg, nand_info->oobsize, fit->oobsize,
		        nand_ecc_name(ecctype));
		return NULL;
	}

	return fit;
}
/*****************************************************************************/

int set_ecctype(struct hinfc_host *host, int type)
{
	if (HINFC_VER_620 == host->version)
		return hinfc620_set_ecctype(host, type);
	else
		return hinfc610_set_ecctype(host, type);
}
/*****************************************************************************/

int get_ecctype(struct hinfc_host *host)
{
	if (HINFC_VER_620 == host->version)
		return hinfc620_get_ecctype(host);
	else
		return hinfc610_get_ecctype(host);
}
/*****************************************************************************/

int set_pagesize(struct hinfc_host *host, int pagesize)
{
	if (HINFC_VER_620 == host->version)
		return hinfc620_set_pagesize(host, pagesize);
	else
		return hinfc610_set_pagesize(host, pagesize);
}
/*****************************************************************************/

static int hinfc610_check_config(struct hinfc_host *host,
				 struct nand_dev_t *nand_dev,
				 struct nand_ctrl_info_t *best, int is_hw_auto)
{
	int isdump = 0;
	int ecctype;
	int pagesize;

	pagesize = hinfc610_get_pagesize(host);

	ecctype = get_ecctype(host);

	if (pagesize != best->pagesize) {
		if (!isdump++)
			nand_register_dump();
		if (is_hw_auto) {
			PR_BUG("!!! warning: "
				"Hardware configure pagesize %s, "
				"but the Nand Flash pageszie is 0x%x. "
				"Update fastboot will resolve this problem.\n",
				pagesize,
				best->pagesize);
		} else {
			PR_BUG("!!! warning: "
				"Hardware configure pagesize %s, "
				"but the Nand Flash pageszie is %s. "
				"Modify hardware pin will resolve this problem.\n",
				pagesize,
				best->pagesize);
		}
	}

	if (ecctype < best->ecctype) {
		if (!is_hw_auto)
			best->ecctype = ecctype;
	} else if (ecctype > best->ecctype) {
		PR_BUG("!!! warning: "
			"Hardware configure ecctype %s, "
			"but %s is more suitable for this Nand Flash."
			"Modify hardware pin will resolve this problem.\n",
			nand_ecc_name(ecctype),
			nand_ecc_name(best->ecctype));
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
/*****************************************************************************/

static int hinfc610_params_adjust(struct nand_dev_t *nand_dev)
{
	int regval;
	char *start_type = "unknown";
	struct nand_ctrl_info_t *best = NULL;
	struct nand_chip_info_t *nand_info = nand_dev->nand_info;
	struct nand_ctrl_info_t *config = NULL;
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;
	unsigned int pagenum;

	if (get_bootmedia(NULL, NULL) == BOOT_MEDIA_NAND) {
		if (IS_NANDC_HW_AUTO(host)) {
			if (host->version == HINFC_VER_610)
				config = hinfc610_hw_auto_config_table;
			else if (host->version == HINFC_VER_620)
				config = hinfc620_hw_auto_config_table;

			start_type = "HW-Auto";
			best = hinfc610_get_best_ecc(config, nand_info);
			if (IS_NANDC_CONFIG_DONE(host))
				hinfc610_check_config(host, nand_dev, best, 1);
			else if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
				host->flags &= ~NAND_RANDOMIZER;
		} else {
			config = hinfc610_hw_pin_config_table;
			start_type = "HW-Pin";
			best = hinfc610_get_best_ecc(config, nand_info);
			hinfc610_check_config(host, nand_dev, best, 0);
		}
	} else {
		if (host->version == HINFC_VER_610)
			config = hinfc610_soft_auto_config_table;
		else if (host->version == HINFC_VER_620)
			config = hinfc620_soft_auto_config_table;
		start_type = "Soft-Auto";
		best = hinfc610_get_best_ecc(config, nand_info);
		if (!IS_NAND_RANDOM(nand_dev) && IS_NAND_RANDOM(host))
			host->flags &= ~NAND_RANDOMIZER;
	}

	host->flags |= (IS_NAND_RANDOM(nand_dev) |
		IS_NAND_SYNC_ASYNC(nand_dev) |
		IS_NAND_ONLY_SYNC(nand_dev) |
		IS_NAND_ONFI(nand_dev));

	nand_dev->flags |= (IS_NANDC_HW_AUTO(host) |
		IS_NANDC_CONFIG_DONE(host) |
		IS_NANDC_SYNC_BOOT(host));

	/* Controller has configured sync boot mode */
	if (IS_NANDC_SYNC_BOOT(host)) {
		/* But NAND do not support sync mode, warning ! */
		if (!IS_NAND_ONLY_SYNC(nand_dev))
			PR_BUG(ERSTR_HARDWARE
				"Hardware SYNC BOOT PIN has configured sync mode, "
				"but the Nand Flash is async mode.\n"
				"DO NOT configure SYNC BOOT PIN. \n");
	} else {
		if (IS_NAND_ONLY_SYNC(nand_dev))
			PR_BUG(ERSTR_HARDWARE
				"Hardware SYNC BOOT PIN has configured async mode, "
				"but the Nand Flash only support sync mode. \n"
				"Please configure SYNC BOOT PIN. \n");
	}

	if (IS_NAND_SYNC_ASYNC(nand_dev))
		hinfc610_get_sync_info(host);

#ifdef CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE
	{
		int ecctype = NAND_ECC_NONE;
		best = hinfc610_force_ecc(config, nand_info, 
					  nand_info->pagesize, 
					  ecctype,
					  "force config", 0);
		start_type = "NoECC";
	}
#endif /* CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
		PR_BUG(ERSTR_HARDWARE
		        "Please configure Nand Flash pagesize and ecctype!\n");

	if (best->ecctype != NAND_ECC_NONE)
		nand_dev->oobsize = best->oobsize;
	nand_dev->oobused = HINFC610_OOBSIZE_FOR_YAFFS;
	nand_dev->layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = best->pagesize;
	host->oobsize  = nand_dev->oobsize;
	host->block_page_mask = 
		((nand_info->erasesize / nand_info->pagesize) - 1);

	host->buffer = devmemalign(HINFC610_DMA_ALIGN, host->pagesize
		+ host->oobsize);
	if (!host->buffer) {
		PR_BUG(ERSTR_DRIVER
			"Can't malloc memory for NAND driver.");
	}

	host->oobbuf = host->buffer + host->pagesize;
	host->dma_buffer = (unsigned int)host->buffer;

	memset(host->iobase, 0xff, HINFC610_BUFFER_BASE_ADDRESS_LEN);
	memset(host->buffer, 0xff, (host->pagesize + host->oobsize));

	host->dma_oob = host->dma_buffer + host->pagesize;
	host->bbm = (unsigned char *)(host->buffer
		+ host->pagesize + HINFC_BAD_BLOCK_POS);

	host->epm = (unsigned short *)(host->buffer
		+ host->pagesize + best->layout->offset + 28);

	nand_dev->bbm = host->bbm;
	nand_dev->pagebuf = host->buffer;
	nand_dev->oobbuf = host->oobbuf;

	set_ecctype(host, best->ecctype);
	set_pagesize(host, best->pagesize);

	/* pass pagesize and ecctype to kernel when spiflash startup. */
	host->set_ecc_randomizer(host, ENABLE, ENABLE);

	if (nand_info->pagesize > NAND_MAX_PAGESIZE ||
	    nand_info->oobsize > NAND_MAX_OOBSIZE) {
		PR_BUG(ERSTR_DRIVER
			"Driver does not support this Nand Flash. "
			"Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
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

	regval = hinfc_read(host, HINFC610_BOOT_SET);
	regval &= ~HINFC610_BOOT_SET_PAGENUM_MASK;
	pagenum = (nand_dev->blocksize / nand_info->pagesize);
	switch(pagenum)
	{
		case 64:
			hinfc_write(host, regval, HINFC610_BOOT_SET);
			break;
		case 128:
			regval |= 0x1;
			hinfc_write(host, regval, HINFC610_BOOT_SET);
			break;
		case 256:
			regval |= 0x2;
			hinfc_write(host, regval, HINFC610_BOOT_SET);
			break;
		case 512:
			regval |= 0x3;
			hinfc_write(host, regval, HINFC610_BOOT_SET);
			break;
		default:
			printf("unsupported pagenum(%d)\n", pagenum);
			break;
	}

	nand_dev->bootmsg = start_type;
	nand_dev->ecctype = host->ecctype;

	host->read_retry = NULL;
	if (nand_info->read_retry_type != NAND_RR_NONE) {
		host->read_retry
			= hinfc610_find_read_retry(nand_info->read_retry_type);
		if (!host->read_retry) {
			PR_BUG(ERSTR_DRIVER
				"This Nand Flash need to enable the 'read retry' feature. "
				"But the driver dose not offer the feature");
		}
	}

	/*
	 * If it want to support the 'read retry' feature, the 'randomizer'
	 * feature must be support first.
	 */
	if (host->read_retry && !IS_NAND_RANDOM(host)) {
		PR_BUG(ERSTR_HARDWARE
			"This Nand flash need to enable 'randomizer' feature. "
			"Please configure hardware randomizer PIN.");
	}

	/*
	 * Check if hardware enable randomizer PIN, But NAND does not need
	 * randomizer. We will notice user.
	 */
	if (IS_NAND_RANDOM(host) &&
	    !hinfc610_support_randomizer(host->pagesize, host->ecctype))
		PR_BUG(ERSTR_HARDWARE
			"This NAND flash does not support `randomizer`, "
			"Please don't configure hardware randomizer PIN.");

	return 0;
}
/*****************************************************************************/

static int hinfc610_version_check(void)
{
	unsigned long regval;
	regval = readl(CONFIG_HINFC610_REG_BASE_ADDRESS + HINFC610_VERSION);
	return ((regval == HINFC_VER_610) || (regval == HINFC_VER_620));
}
/*****************************************************************************/
/*
 *  return:
 *     1   - write/erase/reset fail.
 *     0   - write/erase/reset pass.
 *    -1   - write/erase/reset wait RB timeout.
 */
static int hinfc610_wait_ready(struct hinfc_host *host)
{
	uint8 *iobase = (uint8 *)host->iobase;
	int timeout = 0x10000;

	do {
		*iobase = 0;
		hinfc610_send_cmd_status(host);
	} while (!(*iobase & NAND_STATUS_READY) && timeout--);

	if (!timeout) {
		PR_BUG("Wait controll reset status timeout.\n");
		return -1;
	}

	return (*iobase & NAND_STATUS_FAIL);
}
/*****************************************************************************/

static void hinfc610_set_address(struct nand_dev_t *nand_dev, uint64 address,
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

static int hinfc610_read_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else {
		host->chipselect = 0;
	}

	hinfc610_set_address(nand_dev, address, FALSE);
	hinfc610_send_cmd_readstart(host);

	hinfc610_wait_ready(host);

	return 0;
}
/*****************************************************************************/
/*
 *  return:
 *     0 - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc610_write_page(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->pagemask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 0;

	hinfc610_set_address(nand_dev, address, FALSE);
	hinfc610_send_cmd_pageprog(host);

	return hinfc610_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc610_erase_block(struct nand_dev_t *nand_dev, uint64 address)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->blockmask));

	if (address >= nand_dev->chipsize) {
		host->chipselect = address >> nand_dev->chipshift;
		address &= nand_dev->chipmask;
	} else
		host->chipselect = 0;

	hinfc610_set_address(nand_dev, address, TRUE);
	hinfc610_send_cmd_erase(host);

	return hinfc610_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
static int hinfc610_reset(struct nand_dev_t *nand_dev, int chipselect)
{
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	host->chipselect = chipselect;

	hinfc610_send_cmd_reset(host, host->chipselect);

	return hinfc610_wait_ready(host) ? -1 : 0;
}
/*****************************************************************************/

static int hinfc610_read_id(struct nand_dev_t *nand_dev, int chipselect,
			    char *buf, int buflen)
{
	uint8 *iobase;
	struct hinfc_host *host = (struct hinfc_host *)nand_dev->priv;

	iobase = (uint8 *)host->iobase;

	host->chipselect = chipselect;

	memset(iobase, 0, HINFC610_NANDINFO_LEN);
	hinfc610_send_cmd_readid(host);

	buflen = min(buflen, HINFC610_NANDINFO_LEN);
	memcpy(buf, iobase, buflen);

	return 0;
}
/*****************************************************************************/
#if 0
int hinfc610_nand_get_rr_param(char *param)
{
	int ret = 0;

	if ((host.read_retry->type == NAND_RR_HYNIX_BG_CDIE) ||
	    (host.read_retry->type == NAND_RR_HYNIX_CG_ADIE)) {
		ret = 64;
		memcpy(param, host.rr_data, ret);
	}
	return ret;
}
#endif
/*****************************************************************************/

static struct hinfc_host hinfc610_host;

static struct nand_dev_t hinfc610_nand_dev = {
	.bootmsg = "Unknown",
	.read_page = hinfc610_read_page,
	.write_page = hinfc610_write_page,
	.erase_block = hinfc610_erase_block,
	.reset = hinfc610_reset,
	.read_id = hinfc610_read_id,
	.adjust_param = hinfc610_params_adjust,
	.priv = (void *)&hinfc610_host,
};
/*****************************************************************************/

int hinfc610_init(struct hinfc_host *host)
{
	printf("\nCheck NAND flash controller v610...");
	if (!hinfc610_version_check()) {
		printf("\n");
		return -ENODEV;
	}
	printf("found\n");

#ifdef CONFIG_NAND_NOTICE_RR
	printf("Nand CONFIG_NAND_NOTICE_RR is defined\n");
#endif /* CONFIG_NAND_NOTICE_RR */

	host->regbase = (void *)CONFIG_HINFC610_REG_BASE_ADDRESS;
	host->iobase = (void *)CONFIG_HINFC610_BUFFER_BASE_ADDRESS;

	hinfc610_controller_enable(host, ENABLE);

	return 0;
}
/*****************************************************************************/

static struct nand_dev_t *hinfc610_driver_init(void)
{
	unsigned int regval;
	struct hinfc_host *host;
	static struct nand_dev_t *nand_dev = NULL;

	if (nand_dev)
		return nand_dev;

	nand_dev = &hinfc610_nand_dev;
	host = (struct hinfc_host *)nand_dev->priv;

	memset((char *)host, 0, sizeof(struct hinfc_host));
	host->regbase = (void *)CONFIG_HINFC610_REG_BASE_ADDRESS;
	host->iobase = (void *)CONFIG_HINFC610_BUFFER_BASE_ADDRESS;

	host->addr_cycle    = 0;
	host->addr_value[0] = 0;
	host->addr_value[1] = 0;
	host->cache_addr_value[0] = (unsigned int)~0;
	host->cache_addr_value[1] = (unsigned int)~0;
	host->chipselect    = 0;

	host->send_cmd_pageprog  = hinfc610_send_cmd_pageprog;
	host->send_cmd_readstart = hinfc610_send_cmd_readstart;
	host->send_cmd_erase     = hinfc610_send_cmd_erase;
	host->send_cmd_readid    = hinfc610_send_cmd_readid;
	host->send_cmd_status    = hinfc610_send_cmd_status;
	host->send_cmd_reset     = hinfc610_send_cmd_reset;

	host->flags = 0;

	host->version = hinfc_read(host, HINFC610_VERSION);

	if (host->version == HINFC_VER_620) {
		regval = hinfc_read(host, HINFC610_STATUS);
		if (regval & HINFC610_RANDOMIZER_EN)
			host->flags |= NAND_RANDOMIZER;
	} else if (host->version == HINFC_VER_610) {
		regval = hinfc_read(host, HINFC610_CON);
		if (regval & HINFC610_CON_RANDOMIZER_EN)
			host->flags |= NAND_RANDOMIZER;
	}

	regval = hinfc_read(host, HINFC610_CON);
	host->NFC_CON = (regval
		| HINFC610_CON_OP_MODE_NORMAL
		| HINFC610_CON_READY_BUSY_SEL);

	host->NFC_CON_ECC_NONE = host->NFC_CON
		& ~(HINFC610_CON_ECCTYPE_MASK
		     << HINFC610_CON_ECCTYPE_SHIFT)
		& ~HINFC610_CON_RANDOMIZER_EN;

	hinfc_write(host,
		(SET_HINFC610_PWIDTH(CONFIG_HINFC610_W_LATCH,
				     CONFIG_HINFC610_R_LATCH,
				     CONFIG_HINFC610_RW_LATCH)),
		HINFC610_PWIDTH);

	/* TODO */
	regval = hinfc_read(host, HINFC610_BOOT_CFG);
	if (host->version == HINFC_VER_620)
		host->flags |= NANDC_HW_AUTO;
	else if (regval & HINFC610_BOOT_CFG_SAVE_PIN_MODE)
		host->flags |= NANDC_HW_AUTO;

	/* check if chip is sync mode. */
	if ((host->version != HINFC_VER_620) && 
		(regval & HINFC610_BOOT_CFG_SYC_NAND_PAD)) {
		host->flags |= NANDC_IS_SYNC_BOOT;

		/*
		 * NAND default is sync mode, and read id, reset in sync mode.
		 */
		host->NFC_CON |= HINFC610_CON_NF_MODE_TOGGLE;
		host->NFC_CON_ECC_NONE |= HINFC610_CON_NF_MODE_TOGGLE;

		/* set synchronous clock and timing. */
		hinfc610_sync_clock(host, ENABLE);
	}

	/*
	 * check if start from nand.
	 * This register REG_SC_GEN15 is set in start.S
	 * When start in NAND (Auto), the ECC/PAGESIZE driver don't detect.
	 */
	if (readl(REG_BASE_SCTL + REG_SC_GEN15) == BOOT_FROM_NAND)
		host->flags |= NANDC_CONFIG_DONE;

	host->set_ecc_randomizer = hinfc610_set_ecc_randomizer;

	hinfc610_init(host);

	return nand_dev;
}
/*****************************************************************************/
#if 0
static unsigned char nand_otp_buf[0x4000];

int hinfc610_nand_rr_param_init(void)
{
	if (!host.read_retry)
		return 0;

	if (host.read_retry->type == NAND_RR_HYNIX_BG_CDIE
	    || host.read_retry->type == NAND_RR_HYNIX_CG_ADIE) {
#define NAND_RR_PARAM_TAG       0xB31E1F8C

		int ix, ret;
		unsigned char *ss = nand_otp_buf;

		/*
		 * do something(ex:read) before reading nand otp,
		 * otherwise hynix maybe wrong.
		 */
		struct nand_logic_t *nand_logic
			= nand_logic_open(0ULL, 0x200000ULL);
		if (!nand_logic) {
			printf("Error: Can't open nand flash. "
				"Init otp parameters failed.\n");
			return -1;
		}
		ret = nand_logic_read(nand_logic, 0x0,
				0x4000, nand_otp_buf, 0);
		if (ret <= 0) {
			printf("Error: Can't read nand flash. "
				"Init otp parameters failed.\n");
			return -1;
		}
		nand_logic_close(nand_logic);

		struct spiflash_logic_t *spiflash_logic = spiflash_logic_open(0,
			0x100000ULL);
		if (!spiflash_logic) {
			printf("Error: Can't open spi flash. "
				"Init otp parameters failed.\n");
			return -1;
		}

		printf("Save Nand read retry paramter to SPIFlash offset 0x%llx\n",
			NAND_RR_PARAM_OFFSET);

		ret = spiflash_logic_read(spiflash_logic, NAND_RR_PARAM_OFFSET,
				8192, nand_otp_buf);
		if (ret != 8192) {
			printf("Error: Can't read spi flash. "
				"Init otp parameters failed.\n");
			return -1;
		}

		if ((*(unsigned int *)ss) == NAND_RR_PARAM_TAG) {
			/* get parameter in NAND/SPIFlash */
			unsigned char chr;

			for (ix = 0, ss += sizeof(int); ix < 64 && *ss; ix++) {

				chr = *ss - ((*ss >= 'A') ? ('A' - 10) : '0');
				host.rr_data[ix] = ((chr << 4) & 0xF0);
				ss++;
				chr = *ss - ((*ss >= 'A') ? ('A' - 10) : '0');
				host.rr_data[ix] |= (chr & 0xF);
				ss++;
				ss++;
			}

		} else {
			/* Get RR parameter from NAND OTP table. */
			if (host.read_retry->get_rr_param(&host)) {
				spiflash_logic_close(spiflash_logic);
				return -1;
			}

			*(int *)ss = NAND_RR_PARAM_TAG;
			ss += sizeof(int);

			for (ix = 0; ix < 64; ix++)
				ss += snprintf((char *)ss,
					       nand_otp_buf + sizeof(nand_otp_buf)/sizeof(char) - ss,
					       "%02X ",
					       (unsigned int)host.rr_data[ix]);

			ret = spiflash_logic_write(spiflash_logic, NAND_RR_PARAM_OFFSET,
				8192, nand_otp_buf);
			if (ret != 8192) {
				printf("Error: Can't write spi flash. "
					"Init otp parameters failed.\n");
				return -1;
			}
		}

		spiflash_logic_close(spiflash_logic);

#undef NAND_RR_PARAM_TAG
	}

	return 0;
}
#endif
/*****************************************************************************/

static void hinfc610_shutdown(struct hinfc_host *host)
{
	/* pass pagesize&ecctype to kernel */
	host->set_ecc_randomizer(host, ENABLE, ENABLE);
}
/*****************************************************************************/

static void hinfc610_module_exit(void *ptr)
{
	struct hinfc_host *host = (struct hinfc_host *)ptr;

	hinfc610_shutdown(host);
}
/*****************************************************************************/

static void hinfc610_module_init(void *ptr)
{
	nand_driver_init = hinfc610_driver_init;
}
/*****************************************************************************/

static struct module_t module_hinfc610 = {
	.priority = MODULE_PRI_DRV,
	.ptr = &hinfc610_host,
	.init = hinfc610_module_init,
	.exit = hinfc610_module_exit,
};
MODULE(module_hinfc610);
