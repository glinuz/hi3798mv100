/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-02-07
 *
******************************************************************************/

#define pr_fmt(fmt) "hinfc610: " fmt

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg_inf.h"
#include "hinfc610_gen.h"
#include "hinfc620_gen.h"
#include "hinfc610_rw_latch.h"
#include "hinfc610_sync.h"
#include "hinfc610_read_retry.h"
#include "hinfc610_ecc_info.h"
#include "hinfc610_cache.h"

/*****************************************************************************/
#ifdef CONFIG_ARCH_HIFONE
#include "hinfc610_hifone.c"
#else
#include "hinfc610_s40.c"
#endif

/******************************************************************************/

static void hinfc610_dma_transfer(struct hinfc_host *host, int todev)
{
	unsigned long reg_val;
	unsigned int phyaddr;
	unsigned int phy_pageaddr;
	unsigned int phy_oobaddr;
	enum dma_data_direction direction =
		todev ? DMA_TO_DEVICE : DMA_FROM_DEVICE;

	phy_oobaddr = dma_map_single(host->dev, host->oobbuf,
		host->oobsize, direction);
	if (unlikely(dma_mapping_error(host->dev, phy_oobaddr))) {
		hinfc_pr_bug("dma mapping failed.\n");
		return;
	}

	hinfc_write(host, phy_oobaddr, HINFC610_DMA_ADDR_OOB);

	phyaddr = phy_pageaddr = dma_map_single(host->dev, host->pagebuf,
		host->pagesize, direction);
	if (unlikely(dma_mapping_error(host->dev, phy_pageaddr))) {
		hinfc_pr_bug("dma mapping failed.\n");
		return;
	}

	hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA);

	if (host->pagesize >= SZ_8K) {
		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA1);
	}

	if (host->pagesize >= SZ_16K) {
		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA2);

		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA3);
	}

	if (host->pagesize >= SZ_32K) {
		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA4);

		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA5);

		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA6);

		phyaddr += HINFC610_DMA_ADDR_OFFSET;
		hinfc_write(host, phyaddr, HINFC610_DMA_ADDR_DATA7);
	}

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
			& HINFC610_DMA_CTRL_DMA_START && timeout) {
			_cond_resched();
			timeout--;
		}
		if (!timeout)
			hinfc_pr_bug("Wait DMA finish timeout.\n");
	} while (0);

	dma_unmap_single(host->dev, phy_pageaddr, host->pagesize, direction);
	dma_unmap_single(host->dev, phy_oobaddr, host->oobsize, direction);
}
/*****************************************************************************/

static void hinfc610_sync_entry(struct hinfc_host *host)
{
	struct nand_sync *sync = host->sync;
	struct nand_chip *chip = host->chip;

	if (!sync) {
		hinfc_pr_bug("this NAND not support sync feature.\n");
		return;
	}

	if (HINFC610_IS_SYNC(host)) {
		hinfc_pr_bug("this NAND not support sync feature.\n");
		return;
	}

	if (sync->enable)
		sync->enable(chip);

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
		hinfc_pr_bug("Unsupport sync type 0x%08X.\n", sync->type);
		break;
	}
}
/*****************************************************************************/

static void hinfc610_sync_exit(struct hinfc_host *host)
{
	struct nand_sync *sync = host->sync;
	struct nand_chip *chip = host->chip;

	if (!HINFC610_IS_SYNC(host)) {
		hinfc_pr_bug("Current already exit from sync feature.\n");
		return;
	}

	if (sync->disable)
		sync->disable(chip);

	host->NFC_CON &= ~HINFC610_CON_NF_MODE_MASK;
	host->NFC_CON_ECC_NONE &= ~HINFC610_CON_NF_MODE_MASK;

	hinfc610_sync_clock(host, DISABLE);
}
/*****************************************************************************/

void hinfc610_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (ctrl & NAND_ALE) {
		unsigned int addr_value = 0;
		unsigned int addr_offset = 0;

		if (ctrl & NAND_CTRL_CHANGE) {
			host->addr_cycle = 0x0;
			host->addr_value[0] = 0x0;
			host->addr_value[1] = 0x0;
		}
		addr_offset = host->addr_cycle << 3;

		if (host->addr_cycle >= HINFC610_ADDR_CYCLE_MASK) {
			addr_offset =
			    (host->addr_cycle - HINFC610_ADDR_CYCLE_MASK) << 3;
			addr_value = 1;
		}

		host->addr_value[addr_value] |=
			((dat & 0xff) << addr_offset);

		host->addr_cycle++;
	}

	if ((ctrl & NAND_CLE) && (ctrl & NAND_CTRL_CHANGE)) {
#ifdef CONFIG_HINFC610_CACHE_ENABLE
		unsigned int pageindex;
#endif /* CONFIG_HINFC610_CACHE_ENABLE */

		host->command = dat & 0xff;
		if (host->page_status)
			host->page_status = 0;

		switch (host->command) {
		case NAND_CMD_PAGEPROG:
#ifdef CONFIG_HINFC610_CACHE_ENABLE
			pageindex = GET_RW_PAGE_INDEX(host);

			hinfc_remove_cache(host->cache, pageindex, 1, 1,
				HINFC_CACHE_REMOVE_CAUSE_WRITE);
#endif /* CONFIG_HINFC610_CACHE_ENABLE */
			host->send_cmd_pageprog(host);

			hinfc610_dbg_write(host);

			break;

		case NAND_CMD_READSTART: {
#ifdef CONFIG_HINFC610_CACHE_ENABLE
			int ret;
			char *pagebuf;
			char *oobbuf;
			int status;
			pageindex = GET_RW_PAGE_INDEX(host);

			ret = hinfc_read_cache(host->cache, pageindex,
				&pagebuf, &oobbuf, &status);

			if (status & HINFC_CACHE_STATUS_EMPTY_PAGE) {
				host->page_status |= HINFC610_PS_EMPTY_PAGE;
				break;
			}

			if (ret) {
				ret = hinfc_save_cache(host->cache, pageindex,
					&pagebuf, &oobbuf);

				host->pagebuf = ret ? host->fix_pagebuf : pagebuf;
#endif /* CONFIG_HINFC610_CACHE_ENABLE */
				host->send_cmd_readstart(host);
				hinfc610_dbg_read(host);

				if (IS_PS_UN_ECC(host) &&
				    !IS_PS_EMPTY_PAGE(host) &&
				    !IS_PS_BAD_BLOCK(host))
					mtd->ecc_stats.failed++;
#ifdef CONFIG_HINFC610_CACHE_ENABLE
				if (!ret) {
					if (IS_PS_EMPTY_PAGE(host)) {
						hinfc_remove_cache(host->cache,
							pageindex, 1, 1,
							HINFC_CACHE_REMOVE_CAUSE_EMPTY_PAGE);
						hinfc_set_cache_status(host->cache,
							pageindex, HINFC_CACHE_STATUS_EMPTY_PAGE);

						host->pagebuf = NULL;
					} else {
						memcpy(oobbuf, host->oobbuf + HINFC610_OOB_OFFSET,
							HINFC610_OOB_SIZE);
						hinfc_cache_save_crc(host->cache, pageindex);
					}
				}

			} else {
				hinfc_cache_check_crc(host->cache, pageindex);
				host->pagebuf = pagebuf;
				memcpy(host->oobbuf + HINFC610_OOB_OFFSET,
					oobbuf, HINFC610_OOB_SIZE);
			}
#endif /* CONFIG_HINFC610_CACHE_ENABLE */
			break;

		}
		case NAND_CMD_ERASE2:
#ifdef CONFIG_HINFC610_CACHE_ENABLE
			pageindex = GET_ERASE_PAGE_INDEX(host);

			host->pagebuf = NULL;
			hinfc_remove_cache(host->cache, pageindex,
				host->page_per_block, 1,
				HINFC_CACHE_REMOVE_CAUSE_ERASE);
#endif /* CONFIG_HINFC610_CACHE_ENABLE */

			host->send_cmd_erase(host);
			hinfc610_dbg_erase(host);

			break;

		case NAND_CMD_READID:
			memset((unsigned char *)(chip->IO_ADDR_R), 0, 0x10);
			host->send_cmd_readid(host);
			break;

		case NAND_CMD_STATUS:
			host->send_cmd_status(host);
			break;

		case NAND_CMD_SEQIN:
			host->pagebuf = host->fix_pagebuf;
			break;

		case NAND_CMD_ERASE1:
		case NAND_CMD_READ0:
			break;

		case NAND_CMD_RESET:
			host->send_cmd_reset(host, host->chipselect);
			break;

		default :
			break;
		}
	}

	if ((dat == NAND_CMD_NONE) && host->addr_cycle) {
		if (host->command == NAND_CMD_SEQIN ||
		    host->command == NAND_CMD_READ0 ||
		    host->command == NAND_CMD_READID) {
			host->offset = 0x0;
			host->column = (host->addr_value[0] & 0xffff);
		}
	}
}
/*****************************************************************************/

static int hinfc610_send_cmd_pageprog(struct hinfc_host *host)
{
	if (*host->bbm != 0xFF && *host->bbm != 0x00)
		pr_warn("Attempt to write an invalid bbm. page: 0x%08x, mark: 0x%02x, current process(pid): %s(%d).\n",
			GET_RW_PAGE_INDEX(host), *host->bbm,
			current->comm, current->pid);

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_entry(host);

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);

	hinfc_write(host, host->addr_value[0] & 0xffff0000, HINFC610_ADDRL);
	hinfc_write(host, host->addr_value[1], HINFC610_ADDRH);
	hinfc_write(host,
		((NAND_CMD_STATUS << 16) | (NAND_CMD_PAGEPROG << 8) |
		 NAND_CMD_SEQIN),
		HINFC610_CMD);

	if (IS_NAND_RANDOM(host))
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
			if (!IS_PS_UN_ECC(host) || get_bits(*host->bbm) < 6) {
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

	if (IS_PS_UN_ECC(host) && get_bits(*host->epm) > 12) {
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

		host->enable_ecc_randomizer(host, DISABLE, DISABLE);
		host->read_retry->set_rr_param(host, ix);

		/* enable ecc and randomizer */
		host->enable_ecc_randomizer(host, ENABLE, ENABLE);

		hinfc_write(host, HINFC610_INTCLR_UE | HINFC610_INTCLR_CE,
			    HINFC610_INTCLR);
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

	host->page_status = hinfc610_get_data_status(host);

	hinfc610_dbg_read_retry(host, ix - 1);

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);
	host->read_retry->reset_rr_param(host);

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_readstart(struct hinfc_host *host)
{
	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_entry(host);

	host->enable_ecc_randomizer(host, ENABLE, ENABLE);

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
			/* return 0xFF to MTD. */
		} else if (!IS_PS_BAD_BLOCK(host)) {
			/* if NAND chip support read retry */
			if (IS_PS_UN_ECC(host) && host->read_retry)
				hinfc610_do_read_retry(host);

		} /* 'else' NAND have a bad block, do nothing. */
	}

	if (IS_NAND_SYNC_ASYNC(host))
		hinfc610_sync_exit(host);

	if (!IS_PS_UN_ECC(host))
		host->ecc.dump(host, NULL, &host->ecc.max_bitsflap);
	else
		host->ecc.max_bitsflap = -1;

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_erase(struct hinfc_host *host)
{
	unsigned int regval;

	/* Don't case the read retry config */
	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, host->addr_value[0], HINFC610_ADDRL);
	hinfc_write(host, (NAND_CMD_ERASE2 << 8) | NAND_CMD_ERASE1,
		HINFC610_CMD);

	regval = HINFC610_OP_WAIT_READY_EN
		 | HINFC610_OP_CMD2_EN
		 | HINFC610_OP_CMD1_EN
		 | HINFC610_OP_ADDR_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | ((host->addr_cycle & HINFC610_OP_ADDR_CYCLE_MASK)
		    << HINFC610_OP_ADDR_CYCLE_SHIFT);

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH(host);

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_sync_readid(struct hinfc_host *host)
{
	unsigned int regval;

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

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

	WAIT_CONTROLLER_FINISH(host);

	return 0;
}
/*****************************************************************************/

static int hinfc610_send_cmd_async_readid(struct hinfc_host *host)
{
	unsigned int regval;

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

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

	WAIT_CONTROLLER_FINISH(host);

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

static int hinfc610_enable_ecc_randomizer(struct hinfc_host *host, int ecc_en,
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

	host->enable_ecc_randomizer(host, DISABLE, DISABLE);

	hinfc_write(host, HINFC610_NANDINFO_LEN, HINFC610_DATA_NUM);
	hinfc_write(host, NAND_CMD_STATUS, HINFC610_CMD);

	/* no need config HINFC610_OP_WAIT_READY_EN, here not config */
	regval = HINFC610_OP_CMD1_EN
		 | HINFC610_OP_READ_DATA_EN
		 | ((host->chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT);

 	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH(host);

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
		 | (((chipselect & HINFC610_OP_NF_CS_MASK)
		    << HINFC610_OP_NF_CS_SHIFT)
		 | HINFC610_OP_WAIT_READY_EN);

	hinfc_write(host, regval, HINFC610_OP);

	WAIT_CONTROLLER_FINISH(host);

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

	WAIT_CONTROLLER_FINISH(host);

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

int hinfc610_dev_ready(struct mtd_info *mtd)
{
	return 0x1;
}
/*****************************************************************************/

void hinfc610_select_chip(struct mtd_info *mtd, int chipselect)
{
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (chipselect < 0) {
#ifdef CONFIG_HINFC610_CACHE_ENABLE
		if (host->cache)
			hinfc_read_cache_finish(host->cache);
#endif /* CONFIG_HINFC610_CACHE_ENABLE */
		return;
	}

	if (chipselect > CONFIG_HINFC610_MAX_CHIP)
		hinfc_pr_bug("invalid chipselect: %d\n", chipselect);

	host->chipselect = chipselect;
}
/*****************************************************************************/

uint8_t hinfc610_read_byte(struct mtd_info *mtd)
{
	int offset;
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (host->command == NAND_CMD_STATUS)
		return readb(chip->IO_ADDR_R);

	host->offset++;

	if (host->command == NAND_CMD_READID)
		return readb(chip->IO_ADDR_R + host->offset - 1);

	if (IS_PS_EMPTY_PAGE(host))
		return 0xFF;

	offset = host->column + host->offset - 1;

	if (offset < host->pagesize)
		return readb(host->pagebuf + offset);
	else
		return readb(host->oobbuf + offset - host->pagesize);
}
/*****************************************************************************/

u16 hinfc610_read_word(struct mtd_info *mtd)
{
	int offset;
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	host->offset += 2;

	if (IS_PS_EMPTY_PAGE(host))
		return 0xFFFF;

	offset = host->column + host->offset - 2;

	if (offset < host->pagesize)
		return readw(host->pagebuf + offset);
	else
		return readw(host->oobbuf + offset - host->pagesize);
}
/*****************************************************************************/

void hinfc610_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	int offset;
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	offset = host->column + host->offset;

	if (offset >= host->pagesize)
		memcpy(host->oobbuf + offset - host->pagesize, buf, len);
	else if (offset + len <= host->pagesize)
		memcpy(host->pagebuf + offset, buf, len);
	else {
		int nr_copy = host->pagesize - offset;

		memcpy(host->pagebuf + offset, buf, nr_copy);
		memcpy(host->oobbuf, buf + nr_copy, len - nr_copy);
	}

	host->offset += len;
}
/*****************************************************************************/

void hinfc610_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	int offset;
	struct nand_chip *chip = mtd->priv;
	struct hinfc_host *host = chip->priv;

	if (IS_PS_EMPTY_PAGE(host)) {
		memset(buf, 0xFF, len);
		host->offset += len;
		return;
	}

	offset = host->column + host->offset;

	if (offset >= host->pagesize)
		memcpy(buf, host->oobbuf + offset - host->pagesize, len);
	else if (offset + len <= host->pagesize)
		memcpy(buf, host->pagebuf + offset, len);
	else {
		int nr_copy = host->pagesize - offset;

		memcpy(buf, host->pagebuf + offset, nr_copy);
		memcpy(buf + nr_copy, host->oobbuf, len - nr_copy);
	}

	host->offset += len;
}
/*****************************************************************************/

static unsigned int  nand_otp_len = 0;
static unsigned char nand_otp[128] = {0};

/* Get NAND parameter table. */
static int __init parse_nand_param(const struct tag *tag)
{
	if (tag->hdr.size <= 2)
		return 0;

	nand_otp_len = ((tag->hdr.size << 2) - sizeof(struct tag_header));

	if (nand_otp_len > sizeof(nand_otp)) {
		printk("%s(%d): Get Nand OTP from tag fail.\n",
			__FUNCTION__, __LINE__);
		return 0;
	}
	memcpy(nand_otp, &tag->u, nand_otp_len);
	return 0;
}
/* 0x48694E77 equal to fastoot ATAG_NAND_PARAM */
__tagtable(0x48694E77, parse_nand_param);
/*****************************************************************************/

int hinfc610_get_ecctype_intf(struct hinfc_host *host)
{
	if (HINFC_VER_620 == host->version)
		return hinfc620_get_ecctype(host);
	else
		return hinfc610_get_ecctype(host);
}
/*****************************************************************************/

void hinfc610_set_ecctype_intf(struct hinfc_host *host, int ecctype)
{
	if (HINFC_VER_620 == host->version)
		hinfc620_set_ecctype(host, ecctype);
	else
		hinfc610_set_ecctype(host, ecctype);
}
/*****************************************************************************/

int hinfc610_config_rw_latch(struct nand_chip *chip, unsigned int rw_latch)
{
	struct hinfc_host *host = chip->priv;

	/* set pwidth register */
	hinfc_write(host, rw_latch, HINFC610_PWIDTH);

	return 0;
}
/*****************************************************************************/

static int hinfc610_param_adjust(struct mtd_info *mtd, struct nand_chip *chip,
				 struct nand_dev_t *nand_dev)
{
	int ret = 0;
	int pagesize;
	int ecctype;
	int page_shift;
	char *start_type = "unknown";
	struct nand_ctrl_info_t *best = NULL;
	struct hinfc_host *host = chip->priv;
	struct hinfc_rw_latch *hinfc610_rw_latch;

	if (IS_NANDC_HW_AUTO(host))
		start_type = "HW-Auto";
	else
		start_type = "HW-Reg";

	pagesize = hinfc610_get_pagesize(host);
	ecctype = hinfc610_get_ecctype_intf(host);
	best = hinfc610_force_ecc(mtd, pagesize, ecctype, start_type, 0);

#ifdef CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE
#  ifdef CONFIG_HINFC610_AUTO_PAGESIZE_ECC
#  error you SHOULD NOT define CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE and CONFIG_HINFC610_AUTO_PAGESIZE_ECC at the same time
#  endif
#  ifdef CONFIG_HINFC610_HARDWARE_PAGESIZE_ECC
#  error you SHOULD NOT define CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE and CONFIG_HINFC610_HARDWARE_PAGESIZE_ECC at the same time
#  endif

	best = hinfc610_force_ecc(mtd, mtd->writesize, NAND_ECC_NONE,
				  "force config", 0);
	start_type = "AutoForce";

#endif /* CONFIG_HINFC610_PAGESIZE_AUTO_ECC_NONE */

	if (!best)
		hinfc_pr_bug(ERSTR_HARDWARE "Please configure Nand Flash pagesize and ecctype!\n");

	/* only in case fastboot check randomizer failed. 
	 * Update fastboot or configure hardware randomizer pin fix this problem.
	 */
	if (IS_NAND_RANDOM(nand_dev) && !(IS_NAND_RANDOM(host)))
		hinfc_pr_bug(ERSTR_HARDWARE "Hardware is not configure randomizer, but it is more suitable for this Nand Flash. "
			     "1. Please configure hardware randomizer PIN. "
			     "2. Please updata fastboot.\n");

	host->flags |= (IS_NAND_RANDOM(nand_dev) |
		IS_NAND_SYNC_ASYNC(nand_dev) |
		IS_NAND_ONLY_SYNC(nand_dev) |
		IS_NAND_ONFI(nand_dev));

	/* only for print nand info. */
	nand_dev->flags |= (IS_NANDC_HW_AUTO(host) |
		IS_NANDC_SYNC_BOOT(host));

	/* only in case fastboot check sync boot pin failed. 
	 * Update fastboot or configure hardware sync boot pin fix this problem.
	 */
	if (IS_NANDC_SYNC_BOOT(host)) {
		/* But NAND do not support sync mode, warning ! */
		if (!IS_NAND_ONLY_SYNC(nand_dev))
			hinfc_pr_bug(ERSTR_HARDWARE "Hardware SYNC BOOT PIN has configured sync mode, but the Nand Flash is async mode.\n"
				     "1. DO NOT configure SYNC BOOT PIN. "
				     "2. Update fastboot.\n");
	} else {
		if (IS_NAND_ONLY_SYNC(nand_dev))
			hinfc_pr_bug(ERSTR_HARDWARE "Hardware SYNC BOOT PIN has configured async mode, but the Nand Flash only support sync mode.\n"
				     "1. Please configure SYNC BOOT PIN. "
				     "2. Update fastboot.\n");
	}

	if (IS_NAND_SYNC_ASYNC(nand_dev))
		hinfc610_get_sync_info(host);

	if (best->ecctype != NAND_ECC_NONE)
		mtd->oobsize = best->oobsize;
	chip->ecc.layout = best->layout;

	host->ecctype  = best->ecctype;
	host->pagesize = best->pagesize;
	host->oobsize  = mtd->oobsize;
	host->block_page_mask = ((mtd->erasesize / mtd->writesize) - 1);

	host->fix_pagebuf = kmalloc(host->pagesize, GFP_KERNEL);
	if (!host->fix_pagebuf) {
		hinfc_pr_bug("out of memory.\n");
		return -EIO;
	}

	host->fix_oobbuf = kmalloc(host->oobsize, GFP_KERNEL);
	if (!host->fix_oobbuf) {
		hinfc_pr_bug("out of memory.\n");
		return -EIO;
	}

	host->pagebuf = host->fix_pagebuf;
	host->oobbuf = host->fix_oobbuf;
	host->epm = (unsigned short *)(host->oobbuf
		+ chip->ecc.layout->oobfree[0].offset + HINFC610_EPM_OFFSET);
	host->bbm = (unsigned char *)(host->oobbuf + HINFC610_BB_OFFSET);

	hinfc610_set_pagesize(host, best->pagesize);

	hinfc610_set_ecctype_intf(host, best->ecctype);

	if (mtd->writesize > NAND_MAX_PAGESIZE ||
	    mtd->oobsize > NAND_MAX_OOBSIZE) {
		hinfc_pr_bug(ERSTR_DRIVER
		       "Driver does not support this Nand Flash. "
		       "Please increase NAND_MAX_PAGESIZE and NAND_MAX_OOBSIZE.\n");
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
		hinfc_pr_msg("Nand divide into 1/%u\n", (1 << shift));
	}

	page_shift = ffs(host->pagesize) - 1;
	host->page_per_chip = chip->chipsize >> page_shift;
	host->page_per_block = mtd->erasesize >> page_shift;

	nand_dev->start_type = start_type;
	nand_dev->ecctype = host->ecctype;

	host->read_retry = NULL;
	if (nand_dev->read_retry_type != NAND_RR_NONE) {
		host->read_retry
		    = hinfc610_find_read_retry(nand_dev->read_retry_type);
		if (!host->read_retry) {
			hinfc_pr_bug(ERSTR_DRIVER "This Nand Flash need to enable the 'read retry' feature. "
				     "but the driver dose not offer the feature");
		}

		if (nand_otp_len) {
			memcpy(host->rr_data, nand_otp, nand_otp_len);
		}
	}

	/*
	 * If it want to support the 'read retry' feature, the 'randomizer'
	 * feature must be support first.
	 */
	if (host->read_retry && !IS_NAND_RANDOM(host)) {
		hinfc_pr_bug(ERSTR_HARDWARE "This Nand flash need to enable 'randomizer' feature. "
			     "Please configure hardware randomizer PIN.");
	}

	if (hinfc610_ecc_info_init(host))
		hinfc_pr_bug(ERSTR_DRIVER "No found ecc dump function. pagesize:%d ecctype:%d.",
			host->pagesize, host->ecctype);

	hinfc610_rw_latch = get_hinfc_rw_latch(nand_dev->ids);
	if (!hinfc610_rw_latch) {
		hinfc_pr_bug(ERSTR_DRIVER
			     "RW Latch table is not implemented.\n");
	}
	hinfc610_config_rw_latch(chip, hinfc610_rw_latch->rw_latch);

	host->dbgfs_root = debugfs_create_dir("nand", NULL);
	if (!host->dbgfs_root) {
		pr_err("Can't create debugfs 'nand' dir.\n");
		return -ENOENT;
	}
#ifdef CONFIG_HINFC610_CACHE_ENABLE
	ret = hinfc_cache_init(&host->cache,
		host->page_per_chip, host->page_per_chip,
		host->pagesize, HINFC610_OOB_SIZE, host->dbgfs_root);
	if (ret)
		hinfc_pr_bug(ERSTR_DRIVER "cache module init fail.");
#endif /* CONFIG_HINFC610_CACHE_ENABLE */

	if (hinfc610_dbg_init(host))
		hinfc_pr_bug(ERSTR_DRIVER "create debugfs fail.");

	return ret;
}
/*****************************************************************************/

int hinfc610_nand_init(struct hinfc_host *host, struct nand_chip *chip)
{
	unsigned int regval;
	host->enable(host, ENABLE);
	host->version = hinfc_read(host, HINFC610_VERSION);

	host->addr_cycle    = 0;
	host->addr_value[0] = 0;
	host->addr_value[1] = 0;
	host->chipselect    = 0;

	host->send_cmd_pageprog = hinfc610_send_cmd_pageprog;
	host->send_cmd_readstart = hinfc610_send_cmd_readstart;
	host->send_cmd_erase = hinfc610_send_cmd_erase;
	host->send_cmd_readid = hinfc610_send_cmd_readid;
	host->send_cmd_status = hinfc610_send_cmd_status;
	host->send_cmd_reset = hinfc610_send_cmd_reset;

	/* enable low power control */
	regval = hinfc_read(host, HINFC610_LP_CTRL);
	regval |= HINFC610_LP_CTRL_EN;
	hinfc_write(host, regval, HINFC610_LP_CTRL);

	host->flags = 0;

	if (host->version == HINFC_VER_620) {
		regval = hinfc_read(host, HINFC610_STATUS);
		if (regval & HINFC610_STATUS_RANDOMIZER_EN)
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
		& (~(HINFC610_CON_ECCTYPE_MASK
		     << HINFC610_CON_ECCTYPE_SHIFT))
		& (~HINFC610_CON_RANDOMIZER_EN);

	hinfc_write(host,
		(SET_HINFC610_PWIDTH(CONFIG_HINFC610_W_LATCH,
				     CONFIG_HINFC610_R_LATCH,
				     CONFIG_HINFC610_RW_LATCH)),
		HINFC610_PWIDTH);

	hinfc_write(host, 0, HINFC610_DMA_ADDR_OOB);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA1);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA2);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA3);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA4);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA5);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA6);
	hinfc_write(host, 0, HINFC610_DMA_ADDR_DATA7);

	if (host->version == HINFC_VER_620)
		host->flags |= NANDC_HW_AUTO;
	else if (host->version == HINFC_VER_610) {
		regval = hinfc_read(host, HINFC610_BOOT_CFG);
		if (regval & HINFC610_BOOT_CFG_SAVE_PIN_MODE)
			host->flags |= NANDC_HW_AUTO;
	}

	/* check if chip is sync mode. */
	if ((host->version != HINFC_VER_620) 
	    && (regval & HINFC610_BOOT_CFG_SYC_NAND_PAD)) {
		host->flags |= NANDC_IS_SYNC_BOOT;

		/*
		 * NAND default is sync mode, and read id, reset in sync mode.
		 */
		host->NFC_CON |= HINFC610_CON_NF_MODE_TOGGLE;
		host->NFC_CON_ECC_NONE |= HINFC610_CON_NF_MODE_TOGGLE;

		/* set synchronous clock and timing. */
		hinfc610_sync_clock(host, ENABLE);
	}

	memset((char *)chip->IO_ADDR_R,
		0xff, HINFC610_BUFFER_BASE_ADDRESS_LEN);

	host->enable_ecc_randomizer = hinfc610_enable_ecc_randomizer;
	hinfc_param_adjust = hinfc610_param_adjust;

	return 0;
}
/******************************************************************************/

int hinfc610_nand_destory(struct hinfc_host *host)
{
	if (host->fix_pagebuf) {
		kfree(host->fix_pagebuf);
		host->fix_pagebuf = NULL;
	}

	if (host->fix_oobbuf) {
		kfree(host->fix_oobbuf);
		host->fix_pagebuf = NULL;
	}
#ifdef CONFIG_HINFC610_CACHE_ENABLE 
	if (host->cache) {
		hinfc_cache_destory(host->cache);
		host->cache = NULL;
	}
#endif /* CONFIG_HINFC610_CACHE_ENABLE */
	return 0;
}
