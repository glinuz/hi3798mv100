/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Update by Czyong 2013-03-01
 *
******************************************************************************/
/*
 * drivers/mtd/nand/nand_util.c
 *
 * Copyright (C) 2006 by Weiss-Electronic GmbH.
 * All rights reserved.
 *
 * @author:	Guido Classen <clagix@gmail.com>
 * @descr:	NAND Flash support
 * @references: borrowed heavily from Linux mtd-utils code:
 *		flash_eraseall.c by Arcom Control System Ltd
 *		nandwrite.c by Steven J. Hill (sjhill@realitydiluted.com)
 *			       and Thomas Gleixner (tglx@linutronix.de)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <common.h>
#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <div64.h>

#include <asm/errno.h>
#include <linux/mtd/mtd.h>
#include <nand.h>
#include <jffs2/jffs2.h>

typedef struct erase_info erase_info_t;
typedef struct mtd_info	  mtd_info_t;

/* support only for native endian JFFS2 */
#define cpu_to_je16(x) (x)
#define cpu_to_je32(x) (x)

#define ES_ERROR          "ERROR: "
#define ES_WARNING        "WARNING: "
#define ES_LENZERO(_str)  "Attempt " _str " 0 Bytes.\n"
#define ES_OUTRANGE(_str) "Attempt " _str " beyond end of device. "
#define ES_ALIGN(_str)    "Attempt to " _str " not aligned data. "

#define PR_SKIP_BB(_offset, _nand) \
	printf ("Skip bad block 0x%08llx\n", (_offset) & ~((loff_t)(_nand)->erasesize - 1));
#define PR_OPFAIL(_str, offset, ret) \
	printf("NAND " _str " from offset 0x%llx failed(%d).\n", offset, ret);
#define PR_RESULT(_str, orglen, reallen) \
	printf("request to " _str " 0x%x, and " _str " 0x%x successfully!\n", orglen, reallen);

/*****************************************************************************/
static int nand_block_bad_scrub(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	return 0;
}

/**
 * nand_erase_opts: - erase NAND flash with support for various options
 *		      (jffs2 formating)
 *
 * @param meminfo	NAND device to erase
 * @param opts		options,  @see struct nand_erase_options
 * @return		0 in case of success
 *
 * This code is ported from flash_eraseall.c from Linux mtd utils by
 * Arcom Control System Ltd.
 */
int nand_erase_opts(nand_info_t *meminfo, const nand_erase_options_t *opts)
{
	struct jffs2_unknown_node cleanmarker;
	erase_info_t erase;
	loff_t erase_length;
	loff_t totalerase = 0;
	int bbtest = 1;
	int result;
	int percent_complete = -1;
	int (*nand_block_bad_old)(struct mtd_info *, loff_t, int) = NULL;
	const char *mtd_device = meminfo->name;
	struct mtd_oob_ops oob_opts;
	struct nand_chip *chip = meminfo->priv;

	memset(&erase, 0, sizeof(erase));
	memset(&oob_opts, 0, sizeof(oob_opts));

	erase.mtd = meminfo;
	erase.len  = meminfo->erasesize;
	erase.addr = opts->offset;
	erase_length = opts->length;

	cleanmarker.magic = cpu_to_je16 (JFFS2_MAGIC_BITMASK);
	cleanmarker.nodetype = cpu_to_je16 (JFFS2_NODETYPE_CLEANMARKER);
	cleanmarker.totlen = cpu_to_je32(8);

	/* scrub option allows to erase badblock. To prevent internal
	 * check from erase() method, set block check method to dummy
	 * and disable bad block table while erasing.
	 */
	if (opts->scrub) {
		struct nand_chip *priv_nand = meminfo->priv;

		nand_block_bad_old = priv_nand->block_bad;
		priv_nand->block_bad = nand_block_bad_scrub;
		/* we don't need the bad block table anymore...
		 * after scrub, there are no bad blocks left!
		 */
		if (priv_nand->bbt) {
			kfree(priv_nand->bbt);
		}
		priv_nand->bbt = NULL;
	}

	if (!erase_length) {
		printf("Attempt to erase 0 Bytes!\n");
		return -1;
	}

	if (erase_length < meminfo->erasesize) {
		printf("Warning: Erase size 0x%08llx smaller than one "	\
		       "erase block 0x%08x\n",erase_length, meminfo->erasesize);
		printf("         Erasing 0x%08x instead\n", meminfo->erasesize);
		erase_length = meminfo->erasesize;
	}

	if ((erase.addr + erase_length) > meminfo->size) {
		erase_length = meminfo->size - erase.addr;
		printf("Erase length is too large, "
		       "Flash size: 0x%08llx, erase offset: 0x%08llx. "
		       "Try to erase 0x%08llx instead!\n",
		       meminfo->size,
		       erase.addr,
		       erase_length);
	}
	for (;
	     erase.addr < opts->offset + erase_length;
	     erase.addr += meminfo->erasesize) {

		WATCHDOG_RESET ();

		if (!opts->scrub && bbtest) {
			int ret = meminfo->block_isbad(meminfo, erase.addr);
			if (ret > 0) {
				if (!opts->quiet)
					printf("\rSkipping bad block at  "
					       "0x%08llx                 "
					       "                         \n",
					       erase.addr);
				continue;

			} else if (ret < 0) {
				printf("\n%s: MTD get bad block failed: %d\n",
				       mtd_device,
				       ret);
				return -1;
			}
		}

		if (erase.addr >= meminfo->size) {

			printf("bad block cause erase end"
			       "(beyond flash size =%#llx)!"
			       "Requested to erase 0x%08llx, "
			       "and erase 0x%08llx successfully!\n",
			       meminfo->size,
			       opts->length,
			       totalerase);

			/* if erase out of range, return real erase length */
			return totalerase;
		}

		result = meminfo->erase(meminfo, &erase);
		if (result != 0) {
			printf("\n%s: MTD Erase failure: %d\n",
			       mtd_device, result);

			if (meminfo->block_markbad && (result == -EIO)) {
				result = meminfo->block_markbad(meminfo,
					erase.addr);
				if (result < 0) {
					int ret;

					if (nand_block_bad_old) {
						struct nand_chip *priv_nand = meminfo->priv;
						priv_nand->block_bad = nand_block_bad_old;
					}

					ret = meminfo->block_isbad(meminfo, erase.addr);

					if (nand_block_bad_old) {
						struct nand_chip *priv_nand = meminfo->priv;
						priv_nand->block_bad = nand_block_bad_scrub;
					}

					if (ret > 0) {
						printf("\n%s: MTD block_markbad at"
							" 0x%08llx failed: %d\n",
							mtd_device, erase.addr, result);
						continue;
					}
				}
				printf("Block at 0x%08llx is marked bad block\n",
					erase.addr);
			}
			continue;
		}

		/* format for JFFS2 ? */
		if (opts->jffs2 && chip->ecc.layout->oobavail >= 8) {
			chip->ops.ooblen = 8;
			chip->ops.datbuf = NULL;
			chip->ops.oobbuf = (uint8_t *)&cleanmarker;
			chip->ops.ooboffs = 0;
			chip->ops.mode = MTD_OOB_AUTO;

			result = meminfo->write_oob(meminfo,
				erase.addr, &chip->ops);
			if (result != 0) {
				printf("\n%s: MTD writeoob failure: %d\n",
				       mtd_device, result);
				continue;
			}
		}

		if (!opts->quiet) {
			unsigned long long n =(unsigned long long)
				(erase.addr + meminfo->erasesize - opts->offset)
				* 100;
			int percent;
			loff_t length = erase_length;
			
			while (length & 0xFFFFFFFF00000000ULL) {
				n = n >> 1;
				length = length >> 1;
			}

			do_div(n, (ulong)length);
			percent = (int)n;

			/* output progress message only at whole percent
			 * steps to reduce the number of messages printed
			 * on (slow) serial consoles
			 */
			if (percent != percent_complete) {
				percent_complete = percent;

				printf("\rErasing at 0x%llx -- %3d%% complete.",
				       erase.addr, percent);

				if (opts->jffs2 && result == 0)
					printf(" Cleanmarker written at 0x%llx.",
					       erase.addr);
			}
		}
		totalerase += meminfo->erasesize;
	}
	if (!opts->quiet)
		printf("\n");

	if (nand_block_bad_old) {
		struct nand_chip *priv_nand = meminfo->priv;

		priv_nand->block_bad = nand_block_bad_old;
		if (!(chip->options & NAND_BBT_SCANNED))
			priv_nand->scan_bbt(meminfo);
	}

	printf("request to erase 0x%08llx, and erase 0x%08llx successfully!\n",
	       opts->length, totalerase);
	return 0;
}

/* XXX U-BOOT XXX */
#ifdef CONFIG_CMD_NAND_LOCK_UNLOCK

/******************************************************************************
 * Support for locking / unlocking operations of some NAND devices
 *****************************************************************************/

#define NAND_CMD_LOCK		0x2a
#define NAND_CMD_LOCK_TIGHT	0x2c
#define NAND_CMD_UNLOCK1	0x23
#define NAND_CMD_UNLOCK2	0x24
#define NAND_CMD_LOCK_STATUS	0x7a

/**
 * nand_lock: Set all pages of NAND flash chip to the LOCK or LOCK-TIGHT
 *	      state
 *
 * @param mtd		nand mtd instance
 * @param tight		bring device in lock tight mode
 *
 * @return		0 on success, -1 in case of error
 *
 * The lock / lock-tight command only applies to the whole chip. To get some
 * parts of the chip lock and others unlocked use the following sequence:
 *
 * - Lock all pages of the chip using nand_lock(mtd, 0) (or the lockpre pin)
 * - Call nand_unlock() once for each consecutive area to be unlocked
 * - If desired: Bring the chip to the lock-tight state using nand_lock(mtd, 1)
 *
 *   If the device is in lock-tight state software can't change the
 *   current active lock/unlock state of all pages. nand_lock() / nand_unlock()
 *   calls will fail. It is only posible to leave lock-tight state by
 *   an hardware signal (low pulse on _WP pin) or by power down.
 */
int nand_lock(struct mtd_info *mtd, int tight)
{
	int ret = 0;
	int status;
	struct nand_chip *chip = mtd->priv;

	/* select the NAND device */
	chip->select_chip(mtd, 0);

	chip->cmdfunc(mtd,
		      (tight ? NAND_CMD_LOCK_TIGHT : NAND_CMD_LOCK),
		      -1, -1);

	/* call wait ready function */
	status = chip->waitfunc(mtd, chip);

	/* see if device thinks it succeeded */
	if (status & 0x01) {
		ret = -1;
	}

	/* de-select the NAND device */
	chip->select_chip(mtd, -1);
	return ret;
}

/**
 * nand_get_lock_status: - query current lock state from one page of NAND
 *			   flash
 *
 * @param mtd		nand mtd instance
 * @param offset	page address to query (muss be page aligned!)
 *
 * @return		-1 in case of error
 *			>0 lock status:
 *			  bitfield with the following combinations:
 *			  NAND_LOCK_STATUS_TIGHT: page in tight state
 *			  NAND_LOCK_STATUS_LOCK:  page locked
 *			  NAND_LOCK_STATUS_UNLOCK: page unlocked
 *
 */
int nand_get_lock_status(struct mtd_info *mtd, loff_t offset)
{
	int ret = 0;
	int chipnr;
	int page;
	struct nand_chip *chip = mtd->priv;

	/* select the NAND device */
	chipnr = (int)(offset >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);


	if ((offset & (mtd->writesize - 1)) != 0) {
		printf ("nand_get_lock_status: "
			"Start address must be beginning of "
			"nand page!\n");
		ret = -1;
		goto out;
	}

	/* check the Lock Status */
	page = (int)(offset >> chip->page_shift);
	chip->cmdfunc(mtd, NAND_CMD_LOCK_STATUS, -1, page & chip->pagemask);

	ret = chip->read_byte(mtd) & (NAND_LOCK_STATUS_TIGHT
					  | NAND_LOCK_STATUS_LOCK
					  | NAND_LOCK_STATUS_UNLOCK);

 out:
	/* de-select the NAND device */
	chip->select_chip(mtd, -1);
	return ret;
}

/**
 * nand_unlock: - Unlock area of NAND pages
 *		  only one consecutive area can be unlocked at one time!
 *
 * @param mtd		nand mtd instance
 * @param start		start byte address
 * @param length	number of bytes to unlock (must be a multiple of
 *			page size nand->writesize)
 *
 * @return		0 on success, -1 in case of error
 */
int nand_unlock(struct mtd_info *mtd, ulong start, ulong length)
{
	int ret = 0;
	int chipnr;
	int status;
	int page;
	struct nand_chip *chip = mtd->priv;
	printf ("nand_unlock: start: %08x, length: %d!\n",
		(int)start, (int)length);

	/* select the NAND device */
	chipnr = (int)(start >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);

	/* check the WP bit */
	chip->cmdfunc(mtd, NAND_CMD_STATUS, -1, -1);
	if (!(chip->read_byte(mtd) & NAND_STATUS_WP)) {
		printf ("nand_unlock: Device is write protected!\n");
		ret = -1;
		goto out;
	}

	if ((start & (mtd->erasesize - 1)) != 0) {
		printf ("nand_unlock: Start address must be beginning of "
			"nand block!\n");
		ret = -1;
		goto out;
	}

	if (length == 0 || (length & (mtd->erasesize - 1)) != 0) {
		printf ("nand_unlock: Length must be a multiple of nand block "
			"size %08x!\n", mtd->erasesize);
		ret = -1;
		goto out;
	}

	/*
	 * Set length so that the last address is set to the
	 * starting address of the last block
	 */
	length -= mtd->erasesize;

	/* submit address of first page to unlock */
	page = (int)(start >> chip->page_shift);
	chip->cmdfunc(mtd, NAND_CMD_UNLOCK1, -1, page & chip->pagemask);

	/* submit ADDRESS of LAST page to unlock */
	page += (int)(length >> chip->page_shift);
	chip->cmdfunc(mtd, NAND_CMD_UNLOCK2, -1, page & chip->pagemask);

	/* call wait ready function */
	status = chip->waitfunc(mtd, chip);
	/* see if device thinks it succeeded */
	if (status & 0x01) {
		/* there was an error */
		ret = -1;
		goto out;
	}

 out:
	/* de-select the NAND device */
	chip->select_chip(mtd, -1);
	return ret;
}
#endif
/*****************************************************************************/
/**
 * get_len_incl_bad
 *
 * Check if length including bad blocks fits into device.
 *
 * @param nand NAND device
 * @param offset offset in flash
 * @param length pointer of image length
 * @return image length including bad blocks
 */
static size_t get_len_incl_bad (nand_info_t *nand, loff_t offset,
				size_t length, int *truncate)
{
	int __truncate;
	size_t len_incl_bad = 0;
	size_t len_excl_bad = 0;
	size_t block_len;
	uint32_t align = ((offset + length) & (nand->erasesize - 1));

	while (len_excl_bad < length) {
		block_len = nand->erasesize
			- (offset & (nand->erasesize - 1));

		if (!nand_block_isbad (nand,
			offset & ~((loff_t)nand->erasesize - 1)))
			len_excl_bad += block_len;

		len_incl_bad += block_len;
		offset       += block_len;

		if (offset >= nand->size)
			break;
	}

	__truncate = (len_excl_bad < length);

	if (truncate)
		(*truncate) = __truncate;

	if (align && !__truncate)
		len_incl_bad -= (nand->erasesize - align);

	return len_incl_bad;
}
/*****************************************************************************/
/**
 * nand_write_skip_bad:
 *
 * Write image to NAND flash.
 * Blocks that are marked bad are skipped and the is written to the next
 * block instead as long as the image is short enough to fit even after
 * skipping the bad blocks.
 *
 * @param nand   NAND device
 * @param offset offset in flash
 * @param length buffer length
 * @param buf           buffer to read from
 * @return       0 in case of success
 */
int nand_write_yaffs_skip_bad(nand_info_t *nand, loff_t offset,
			      size_t *length, u_char *buffer)
{
	int i;
	int ret = -EINVAL;
	int truncate;
	u_char *pbuf = buffer;
	size_t len_incl_bad;
	mtd_oob_ops_t ops = {0};
	uint64_t __offset = offset;
	uint32_t len_excl_oob;

	if (offset >= nand->size) {
		printf(ES_ERROR ES_OUTRANGE("write")
			"offset(0x%llx) beyond flash size(0x%llx)\n",
			__offset, nand->size);
		goto out;
	}

	if ((offset & (nand->writesize - 1)) != 0) {
		printf(ES_ERROR ES_ALIGN("write")
			"offset(0x%llx) not aligned with pagesize(0x%x)\n",
			__offset, nand->writesize);
		goto out;
	}

	if (((*length) % (nand->writesize  + nand->oobused)) != 0) {
		printf(ES_ERROR ES_ALIGN("write")
			"length(0x%x) not aligned with pagesize(0x%x)"
			" + oobsize(0x%x)\n",
			(*length), nand->writesize, nand->oobused);
		goto out;
	}

	len_excl_oob = (*length) / (nand->writesize + nand->oobused)
		* nand->writesize;

	len_incl_bad = get_len_incl_bad (nand, offset,
		len_excl_oob, &truncate);

	printf("pure data length is %d, len_incl_bad is %d\n",
		len_excl_oob, len_incl_bad);

	if (truncate) {
		printf(ES_ERROR ES_OUTRANGE("write")
			"offset(0x%llx) + length(0x%x) beyond"
			" flash size(0x%llx)\n",
			__offset, (*length), nand->size);
		goto out;
	}

	while (len_excl_oob > 0) {
		size_t block_offset = offset & (nand->erasesize - 1);
		size_t write_size;

		if (offset >= nand->size) {
			printf(ES_ERROR ES_OUTRANGE("write")
				"offset(0x%llx) + length(0x%x) beyond"
				" flash size(0x%llx)\n",
				__offset, (*length), nand->size);
			goto out;
		}

		if (nand_block_isbad (nand,
			offset & ~((loff_t)nand->erasesize - 1))) {
			PR_SKIP_BB(offset, nand);
			offset += nand->erasesize;
			continue;
		}

		if (len_excl_oob < (nand->erasesize - block_offset))
			write_size = len_excl_oob;
		else
			write_size = nand->erasesize - block_offset;

		for (i = 0; i < (write_size / nand->writesize); i++) {

			ops.datbuf  = pbuf;
			ops.oobbuf  = pbuf + nand->writesize;
			ops.len     = nand->writesize;
			ops.ooblen  = nand->oobused;
			ops.ooboffs = 0;
			ops.mode    = MTD_OOB_RAW;

			ret = nand->write_oob(nand, offset, &ops);
			if (ret != 0) {
				PR_OPFAIL("write", offset, ret);
				/* try to mark bad. */
				if (nand_block_markbad(nand, offset) < 0) {
					PR_OPFAIL("mark bad", offset, ret);
					goto out;
				}

				/* This print is used by PC HiTool, Do not modify! */
				printf("WARNING: block is marked as bad(0x%llx).\n", 
					offset & ~(nand->erasesize - 1));

				goto out;
			}
			pbuf   += (nand->writesize + nand->oobused);
			offset += nand->writesize;
		}
		len_excl_oob -= write_size;
	}
	ret = 0;
	printf("\n");
out:
	return ret;
}
/*****************************************************************************/
/**
 * nand_fill_ecc - [Internal] Process oob to client buffer
 * @chip:	nand chip structure
 * @oob:	oob destination address
 * @len:	size of oob to process
 */
void nand_fill_ecc(struct nand_chip *chip, uint8_t *oob, size_t len)
{
	struct nand_oobfree *free = chip->ecc.layout->oobfree;
	uint32_t offset = 0, bytes;

	for (; free->length && offset < len; free++) {
		if (offset < free->offset) {
			bytes = min_t(size_t, len, free->offset) - offset;
			memset(oob + offset, 0xff, bytes);
		}

		offset = free->offset + free->length;
	}

	if (offset < len)
		memset(oob + offset, 0xff, len - offset);
}
/*****************************************************************************/
/**
 * nand_read_yaffs_skip_bad:
 *
 * Read image from NAND flash.
 * Blocks that are marked bad are skipped and the next block is readen
 * instead as long as the image is short enough to fit even after skipping the
 * bad blocks.
 *
 * @param nand NAND device
 * @param offset offset in flash, alignment with pagesize
 * @param length buffer length, on return holds remaining bytes to read,
 *               alignment with (pagesize + oobsize)
 * @param buffer buffer to read to
 * @return 0 in case of success
 */
int nand_read_yaffs_skip_bad(nand_info_t *nand, loff_t offset, size_t *length,
			     u_char *buffer)
{
	int i;
	int ret = -EINVAL;
	size_t len_incl_bad;
	uint32_t len_excl_oob;
	u_char  *pbuf = buffer;
	uint64_t __offset = offset;
	uint32_t len_incl_oob = (*length);

	mtd_oob_ops_t ops = {0};
	struct nand_chip *chip = nand->priv;

	if (!(*length)) {
		printf(ES_ERROR ES_LENZERO("read"));
		goto out;
	}

	if (offset >= nand->size) {
		printf(ES_ERROR ES_OUTRANGE("read")
			"offset(0x%llx) beyond flash size(0x%llx)\n",
			__offset, nand->size);
		goto out;
	}

	if ((offset & (nand->writesize - 1)) != 0) {
		printf(ES_ERROR ES_ALIGN("read")
			"offset(0x%llx) not aligned with pagesize(0x%x)\n",
			__offset, nand->writesize);
		goto out;
	}

	/* align read length. */
	if ((len_incl_oob % (nand->writesize  + nand->oobused)) != 0) {

		len_incl_oob -=
			len_incl_oob % (nand->writesize + nand->oobused);

		printf(ES_WARNING ES_ALIGN("read")
			"length(0x%x) not aligned with pagesize(0x%x)"
			" + oobsize(0x%x), truncated length(0x%x).\n",
			(*length), nand->writesize, nand->oobused,
			len_incl_oob);
	}

	len_excl_oob = len_incl_oob / (nand->writesize + nand->oobused)
		* nand->writesize;

	/* truncate to nand size */
	if ((offset + len_excl_oob) > nand->size) {

		len_excl_oob = nand->size - offset;
		len_incl_oob = len_excl_oob / nand->writesize
			* (nand->writesize + nand->oobused);

		printf(ES_WARNING ES_OUTRANGE("read")
			"offset(0x%llx) + length(0x%x) beyond flash"
			" size(0x%llx), truncated length(0x%x).\n",
			__offset, (*length), nand->size, len_incl_oob);
	}

	if (!len_excl_oob) {
		printf(ES_ERROR ES_LENZERO("read"));
		goto out;
	}

	len_incl_bad = get_len_incl_bad(nand, offset, len_excl_oob, NULL);

	printf("pure data length is %d, len_incl_bad is %d\n",
		len_excl_oob, len_incl_bad);

	while (len_excl_oob > 0) {
		size_t block_offset = offset & (nand->erasesize - 1);
		size_t read_size;

		if (offset >= nand->size) {
			printf(ES_WARNING ES_OUTRANGE("read")
				"offset(0x%llx) + length(0x%x) beyond flash"
				" size(0x%llx), truncated length(0x%x).\n",
				__offset, (*length), nand->size,
				(pbuf - buffer));
			ret = 0;
			goto out;
		}

		if (nand_block_isbad(nand,
			offset & ~((loff_t)nand->erasesize - 1))) {
			PR_SKIP_BB(offset, nand);
			offset += nand->erasesize;
			continue;
		}

		if (len_excl_oob < (nand->erasesize - block_offset))
			read_size = len_excl_oob;
		else
			read_size = nand->erasesize - block_offset;

		for (i = 0; i < (read_size / nand->writesize); i++) {

			ops.datbuf  = pbuf;
			ops.oobbuf  = pbuf + nand->writesize;
			ops.len     = nand->writesize;
			ops.ooblen  = nand->oobused;
			ops.ooboffs = 0;
			ops.mode    = MTD_OOB_RAW;

			ret = nand->read_oob(nand, offset, &ops);
			if (ret != 0) {
				PR_OPFAIL("read", offset, ret);
				goto out;
			}
			nand_fill_ecc(chip, ops.oobbuf, ops.ooblen);

			pbuf   += (nand->writesize + nand->oobused);
			offset += nand->writesize;
		}
		len_excl_oob -= read_size;
	}

	PR_RESULT("read", (*length), (pbuf - buffer));
	ret = 0;
out:
	(*length) = (pbuf - buffer);
	return ret;
}

static inline int page_empty(u_char *buf, int pagesize)
{
	int * nn = (int *)buf;

	pagesize = (pagesize >> 2);
	while (pagesize--) {
		if (~(*nn++))
			return 1;
	}
	return 0;
}

static int nand_write_skip_empty(nand_info_t *info, loff_t ofs,
				 size_t *len, u_char *buf)
{
	int ret = 0;
	size_t retlen = 0;
	size_t total = (*len);
	int pagesize = info->writesize;

	while (total > 0) {
		(*len) = pagesize;
		if (page_empty(buf, pagesize)) {
			ret = nand_write(info, ofs, len, buf);
			if (ret)
				break;
		}

		total  -= pagesize;
		ofs    += pagesize;
		buf    += pagesize;
		retlen += (*len);
	}

	(*len) = retlen;
	return ret;
}
/*****************************************************************************/
/**
 * nand_write_skip_bad:
 *
 * Write image to NAND flash.
 * Blocks that are marked bad are skipped and the is written to the next
 * block instead as long as the image is short enough to fit even after
 * skipping the bad blocks.
 *
 * @param nand  	NAND device
 * @param offset	offset in flash
 * @param length	buffer length
 * @param buf           buffer to read from
 * @param skipempty     skip empty page (0xFF)
 * @return		0 in case of success
 */
int nand_write_skip_bad(nand_info_t *nand, loff_t offset, size_t *length,
			u_char *buffer, int skipempty)
{
	int ret = -EINVAL;
	size_t len_incl_bad;
	u_char *pbuf;
	int truncate;
	uint64_t __offset = offset;
	uint32_t len_excl_bad = (*length);

	if (offset >= nand->size) {
		printf(ES_ERROR ES_OUTRANGE("write")
			"offset(0x%llx) beyond flash size(0x%llx)\n",
			__offset, nand->size);
		goto out;
	}

	if ((offset & (nand->writesize - 1)) != 0 ||
	    ((*length) & (nand->writesize - 1)) != 0) {
		printf(ES_ERROR ES_ALIGN("write")
			"offset(0x%llx) or length(0x%x) not aligned"
			" with pagesize(0x%x)\n",
			__offset, (*length), nand->writesize);
		goto out;
	}

	len_incl_bad = get_len_incl_bad(nand, offset,
		len_excl_bad, &truncate);

	printf("pure data length is %d, len_incl_bad is %d\n",
	       len_excl_bad, len_incl_bad);

	if (truncate) {
		printf(ES_ERROR ES_OUTRANGE("write")
			"offset(0x%llx) + length(0x%x) beyond"
			" flash size(0x%llx)\n",
			__offset, (*length), nand->size);
		goto out;
	}

	pbuf = buffer;
	while (len_excl_bad > 0) {
		size_t block_offset = offset & (nand->erasesize - 1);
		size_t write_size;

		if (offset >= nand->size) {
			printf(ES_ERROR ES_OUTRANGE("write")
				"offset(0x%llx) + length(0x%x) beyond"
				" flash size(0x%llx)\n",
				__offset, (*length), nand->size);
			goto out;
		}

		if (nand_block_isbad (nand,
			offset & ~((loff_t)nand->erasesize - 1))) {
			PR_SKIP_BB(offset, nand);
			offset += nand->erasesize;
			continue;
		}

		if (len_excl_bad < (nand->erasesize - block_offset))
			write_size = len_excl_bad;
		else
			write_size = nand->erasesize - block_offset;

		if (skipempty)
			ret = nand_write_skip_empty(nand, offset,
				&write_size, pbuf);
		else
			ret = nand_write(nand, offset, &write_size, pbuf);
		if (ret != 0) {
			PR_OPFAIL("write", offset, ret);
			/* try to mark bad. */
			if (nand_block_markbad(nand, offset) < 0) {
				PR_OPFAIL("mark bad", offset, ret);
				goto out;
			}

			/* This print is used by PC HiTool, Do not modify! */
			printf("WARNING: block is marked as bad(0x%llx).\n", 
				offset & ~(nand->erasesize - 1));

			goto out;
		}

		len_excl_bad -= write_size;
		offset       += write_size;
		pbuf         += write_size;
	}
	ret = 0;
out:
	return ret;
}
/*****************************************************************************/
/**
 * nand_read_skip_bad:
 *
 * Read image from NAND flash.
 * Blocks that are marked bad are skipped and the next block is readen
 * instead as long as the image is short enough to fit even after skipping the
 * bad blocks.
 *
 * @param nand NAND device
 * @param offset offset in flash
 * @param length buffer length, on return holds remaining bytes to read
 * @param buffer buffer to write to
 * @return 0 in case of success
 */
int nand_read_skip_bad(nand_info_t *nand, loff_t offset, size_t *length,
		       u_char *buffer)
{
	int ret = -EINVAL;
	size_t len_incl_bad;
	u_char *pbuf = buffer;
	uint64_t __offset = offset;
	uint32_t len_excl_bad = (*length);

	if (!(*length)){
		printf(ES_ERROR ES_LENZERO("read"));
		goto out;
	}

	if (offset >= nand->size) {
		printf(ES_ERROR ES_OUTRANGE("read")
			"offset(0x%llx) beyond flash size(0x%llx)\n",
			__offset, nand->size);
		goto out;
	}

	if (offset & (nand->writesize - 1)) {
		printf(ES_ERROR ES_ALIGN("read")
			"offset(0x%llx) not aligned with pagesize(0x%x)\n",
			__offset, nand->writesize);
		goto out;
	}

	if (len_excl_bad & (nand->writesize - 1)) {

		len_excl_bad = (len_excl_bad & ~(nand->writesize -1));

		printf(ES_WARNING ES_ALIGN("read")
			"length(0x%x) not aligned with pagesize(0x%x),"
			" truncated length(0x%x).\n",
			(*length), nand->writesize, len_excl_bad);
	}

	if ((offset + len_excl_bad) > nand->size) {
		len_excl_bad = nand->size - offset;

		printf(ES_WARNING ES_OUTRANGE("read")
			"offset(0x%llx) + length(0x%x) beyond flash "
			"size(0x%llx), truncated length(0x%x).\n",
			__offset, (*length), nand->size, len_excl_bad);
	}

	if (!len_excl_bad) {
		printf(ES_ERROR ES_LENZERO("read"));
		goto out;
	}

	len_incl_bad = get_len_incl_bad(nand, offset, len_excl_bad, NULL);

	printf("pure data length is %d, len_incl_bad is %d\n",
		len_excl_bad, len_incl_bad);

	pbuf = buffer;
	while (len_excl_bad > 0) {
		size_t block_offset = offset & (nand->erasesize - 1);
		size_t read_length;

		if (offset >= nand->size) {
			printf(ES_WARNING ES_OUTRANGE("read")
				"offset(0x%llx) + length(0x%x) beyond flash "
				"size(0x%llx), truncated length(0x%x).\n",
				__offset, (*length), nand->size,
				(pbuf - buffer));
			ret = 0;
			goto out;
		}

		if (nand_block_isbad(nand,
			offset & ~((loff_t)nand->erasesize - 1))) {
			PR_SKIP_BB(offset, nand);
			offset += nand->erasesize;
			continue;
		}

		if (len_excl_bad < (nand->erasesize - block_offset))
			read_length = len_excl_bad;
		else
			read_length = nand->erasesize - block_offset;

		ret = nand_read(nand, offset, &read_length, pbuf);
		if (ret && ret != -EUCLEAN) {
			PR_OPFAIL("read", offset, ret);
			goto out;
		}

		len_excl_bad -= read_length;
		offset       += read_length;
		pbuf         += read_length;
	}

	PR_RESULT("read", (*length), (pbuf - buffer));
	ret = 0;
out:
	(*length) = (pbuf - buffer);
	return ret;
}
