/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-03-05
 *
******************************************************************************/

#include <common.h>
#include <watchdog.h>
#include <asm/errno.h>
#include <malloc.h>
#include <nand.h>

#include <nand_logif.h>

#ifdef CONFIG_CMD_NAND

/*****************************************************************************/
#include <debug.h>

#define BLOCK_ALIGN(_off)    \
	((_off) & ~((typeof(_off))(nand_logic->erasesize) - 1))

#define BLOCK_ALIGN_ROUND_OUT(_off)    \
	(((_off) + nand_logic->erasesize - 1) \
		& ~((typeof(_off))(nand_logic->erasesize) - 1))

#define IS_BLOCK_ALIGN(_handle, _off)  \
	(!((_off) & ((typeof(_off))((_handle)->erasesize) - 1)))

#define IS_PAGE_ALIGN(_nand, _off) \
	(!((_off) & ((typeof(_off))((_nand)->writesize) - 1)))

/*****************************************************************************/

struct bbt_cache_t {
	uint64_t length; /* offset by partition */
	uint32_t shift;
	u_char   *bbt; /* 1 bad block, 0 good block */

#define BBT_POS(_bbt_cache, _off_by_ptn, _pos, _bit) do { \
	uint32_t idx = (uint32_t)((_off_by_ptn) >> (_bbt_cache)->shift); \
	(_pos) = &(_bbt_cache)->bbt[idx >> 3]; \
	(_bit) = (1 << (idx & 0x07)); \
} while (0)

};

#define DEFINE_BBT_CACHE(_bbt_cache, _nand_logic) \
	struct bbt_cache_t *_bbt_cache \
		= (struct bbt_cache_t *)(_nand_logic)->bbt_cache

/*****************************************************************************/

static int bbt_cache_init(nand_logic_t *nand_logic)
{
	uint32_t bytes;
	uint32_t shift = 0;
	struct bbt_cache_t *bbt_cache;
	uint32_t erasesize = (uint32_t)nand_logic->erasesize;

	while (!(erasesize & 0x01)) {
		shift++;
		erasesize >>= 1;
	}

	/* 8bits/Byte, a char cover 8 block */
	bytes = (uint32_t)(((nand_logic->length >> shift) + 7) >> 3);
	bbt_cache = (struct bbt_cache_t *)malloc(sizeof(struct bbt_cache_t) + bytes);

	if (!bbt_cache)
		return -1;

	nand_logic->bbt_cache = (void *)bbt_cache;

	bbt_cache->bbt  = (u_char *)&bbt_cache[1];
	bbt_cache->length = 0;
	bbt_cache->shift  = shift;
	memset(bbt_cache->bbt, 0, bytes);

	return 0;
}
/*****************************************************************************/

static int bbt_cache_deinit(nand_logic_t *nand_logic)
{
	if (nand_logic->bbt_cache) {
		free(nand_logic->bbt_cache);
		nand_logic->bbt_cache = NULL;
	}

	return 0;
}
/*****************************************************************************/

static int bbt_cache_isbad_block(nand_logic_t *nand_logic,
				 uint64_t offset/* offset by NAND chip */)
{
	int32_t bit;
	u_char *pos;
	DEFINE_BBT_CACHE(bbt_cache, nand_logic);
	uint64_t off_by_ptn = (offset - nand_logic->address);

	ASSERT(!(offset & (nand_logic->erasesize-1)));
	ASSERT(offset < (nand_logic->address + nand_logic->length));

	while (off_by_ptn >= bbt_cache->length) {

		if (nand_block_isbad(nand_logic->nand,
			bbt_cache->length + nand_logic->address)) {

			BBT_POS(bbt_cache, bbt_cache->length, pos, bit);
			(*pos) |= bit;

			PR_MSG("bbt_cache:%p, badblock:0x%llx "
				" offset by 0x%llx\n",
				bbt_cache->bbt,
				bbt_cache->length,
				nand_logic->address);
		}

		bbt_cache->length += nand_logic->erasesize;
	}

	BBT_POS(bbt_cache, off_by_ptn, pos, bit);

	return !!(*pos & bit);
}
/*****************************************************************************/

static int bbt_cache_markbad_block(nand_logic_t *nand_logic,
				   uint64_t offset/* offset by NAND chip */)
{
	int32_t bit;
	u_char *pos;
	DEFINE_BBT_CACHE(bbt_cache, nand_logic);
	uint64_t off_by_ptn = (offset - nand_logic->address);
	nand_info_t *nand = (nand_info_t *)nand_logic->nand;

	ASSERT(!(offset & (nand_logic->erasesize-1)));
	ASSERT(offset < (nand_logic->address + nand_logic->length));

	BBT_POS(bbt_cache, off_by_ptn, pos, bit);

	(*pos) |= bit;

	return nand->block_markbad(nand, offset);
}
/*****************************************************************************/
/*
 * logiclength   - The length exclude bad block. should be alignment with
 *                 block size.
 *
 * return        - The length include bad block.
 */
static uint64_t logic_to_phylength(nand_logic_t *nand_logic,
				   uint64_t logiclength,
				   int *truncate)
{
	int __truncate;
	uint64_t len_incl_bad = 0;
	uint64_t len_excl_bad = 0;
	uint64_t offset  = nand_logic->address;
	uint64_t ptn_end = nand_logic->address + nand_logic->length;

	ASSERT(!(logiclength & (nand_logic->erasesize-1)));

	while (len_excl_bad < logiclength) {
		if (!bbt_cache_isbad_block(nand_logic, offset))
			len_excl_bad += nand_logic->erasesize;

		len_incl_bad += nand_logic->erasesize;
		offset       += nand_logic->erasesize;

		if (offset >= ptn_end)
			break;
	}

	__truncate = (len_excl_bad < logiclength);

	if (truncate)
		(*truncate) = __truncate;

	return len_incl_bad;
}
/*****************************************************************************/
/*
 * address   - NAND partition start address. It should be alignemnt with
 *             block size.
 * length    - NAND partition length. It should be alignment with block size
 *
 * return    - return a handle if success. read/write/earse use this handle.
 */
nand_logic_t *nand_logic_open(unsigned long long address,
			      unsigned long long length)
{
	nand_info_t  *nand;
	nand_logic_t *nand_logic;

	/* the following commands operate on the current device */
	if (nand_curr_device < 0
		|| nand_curr_device >= CONFIG_SYS_MAX_NAND_DEVICE
		|| !nand_info[nand_curr_device].name) {
		printf("No devices available\n");
		return NULL;
	}
	nand = &nand_info[nand_curr_device];

	/* Reject open, which are not block aligned */
	if (!IS_BLOCK_ALIGN(nand, address)
	    || !IS_BLOCK_ALIGN(nand, length)) {
		printf("Attempt to open non block aligned, "
			"nand blocksize: 0x%08x, address: 0x%08llx,"
			" length: 0x%08llx\n",
			nand->erasesize, address, length);
		return NULL;
	}

	if ((address > nand->size)
		|| (length > nand->size)
		|| ((address + length) > nand->size)) {
		printf("Attempt to open outside the flash area, "
			"nand chipsize: 0x%08llx, address: 0x%08llx,"
			" length: 0x%08llx\n",
			nand->size, address, length);
		return NULL;
	}

	if ((nand_logic = malloc(sizeof(nand_logic_t))) == NULL) {
		printf("Out of memory.\n");
		return NULL;
	}

	nand_logic->nand      = nand;
	nand_logic->address   = address;
	nand_logic->length    = length;
	nand_logic->erasesize = nand->erasesize;

	if (bbt_cache_init(nand_logic)) {
		printf("Out of memory.\n");
		free(nand_logic);
		nand_logic = NULL;
		return NULL;
	}

	return nand_logic;
}
/*****************************************************************************/

void nand_logic_close(nand_logic_t *nand_logic)
{
	bbt_cache_deinit(nand_logic);
	free(nand_logic);
}
/*****************************************************************************/
/*
 * offset  - NAND erase logic start address. You don't case bad block.
 *           It should be alignment with NAND block size.
 * length  - NAND erase length. You don't case bad block.
 *           It should be alignment with NAND block size.
 *
 * return  - 0: success.
 *           other: fail.
 * NOTES:
 *        If erase fail, it will mark bad block.
 *        Do NOT modify the printf string, it may be used by pc tools.
 */
long long nand_logic_erase(nand_logic_t *nand_logic,
		     unsigned long long offset,
		     unsigned long long length)
{
	int truncate;
	struct erase_info erase;
	uint64_t phylength;
	uint64_t totalerase = 0;
	uint64_t request_length = length;

	nand_info_t *nand = nand_logic->nand;

	if (!length) {
		printf("Attempt to erase 0 Bytes!\n");
		return -1;
	}

	if (!IS_BLOCK_ALIGN(nand, offset)
	    || !IS_BLOCK_ALIGN(nand, length)) {
		printf("Attempt to erase non block aligned, "
			"nand blocksize: 0x%08x, offset: 0x%08llx,"
			" length: 0x%08llx\n",
			nand->erasesize, offset, length);
		return -1;
	}

	/* "offset" is offset by partition, the first address is 0 */
	if (offset > nand_logic->length) {
		printf("Attempt to erase from out of the flash paratition, "
		       "paratition size: 0x%08llx, erase offset: 0x%08llx\n",
		       nand_logic->length, offset);
		return -1;
	}

	phylength = logic_to_phylength(nand_logic, offset, &truncate);
	if (truncate) {
		printf("Offset out of partition, because of bad block.\n");
		return -1;
	}

	if ((offset + length) > nand_logic->length) {

		length = nand_logic->length - offset;

		printf("Erase length is too large, "
		       "paratition size: 0x%08llx, erase offset: 0x%08llx\n"
		       "Try to erase 0x%08llx instead!\n",
		       nand_logic->length, offset, length);

	}

	memset(&erase, 0, sizeof(erase));
	erase.mtd  = nand;
	erase.len  = nand->erasesize;
	erase.addr = nand_logic->address + phylength;

	for (; length > 0; erase.addr += nand->erasesize) {

		WATCHDOG_RESET ();

		if (erase.addr >= (nand_logic->address + nand_logic->length)) {
			printf("bad block cause erase end"
			       "(beyond nand_logic->length =%#llx)!"
			       "Requested to erase 0x%08llx, "
			       "and erase 0x%08llx successfully!\n",
			       nand_logic->length,
			       request_length,
			       totalerase);
			/* if erase out of open region, return erase length instead! */
			return totalerase;
		}

		int ret = nand->block_isbad (nand, erase.addr);
		if (ret > 0) {
			printf("\rSkipping bad block at  "
				"0x%08llx                   "
				"                         \n",
				erase.addr);
			continue;
		} else if (ret < 0) {
			printf("\n%s: MTD get bad block at 0x%08llx"
				" failed: %d\n",
				nand->name, erase.addr, ret);
			return -1;
		}
		/* for PC tools(usb burn command) to know burn status, DO NOT modify */
		printf("\rErasing at 0x%08llx ", erase.addr);
		ret = nand->erase(nand, &erase);
		if (ret) {

			printf("\n%s: MTD Erase at 0x%08llx failure: %d, ",
				nand->name, erase.addr, ret);

			if (nand->block_markbad && (ret == -EIO)) {
				printf("Mark bad block.");
				ret = bbt_cache_markbad_block(nand_logic,
					erase.addr);
				if (ret < 0) {
					printf("\n%s: MTD block_markbad at"
						" 0x%08llx failed: %d, aborting\n",
						nand->name, erase.addr, ret);
					return -1;
				}
				printf("Block at 0x%08llx is marked bad block\n",
					erase.addr);
			}
			continue;
		}

		length     -= nand->erasesize;
		totalerase += nand->erasesize;
	}
	if (totalerase < request_length)
		printf("\nRequest erase 0x%08llx, real erase 0x%08llx successfully!\n",
		       request_length, totalerase);
	return totalerase;
}
/******************************************************************************/
#define DATABUF_MAX_LEN                         0x5000 /* 20KB */
static unsigned char databuf[DATABUF_MAX_LEN] = {0};

/*
 * phyaddress is a write fail address
 * 1. find the next good block
 * 2. read from the block start when write fail
 * 3. write this page to "the next good block"
 */
static int move_data(nand_logic_t *nand_logic,
		     unsigned long long *phyaddress,
		     int withoob)
{
	int ret;
	struct mtd_oob_ops ops;
	nand_info_t *nand = nand_logic->nand;
	struct nand_chip *chip = nand->priv;
	uint64_t phyaddr_org = *phyaddress;
	uint64_t newaddr; /* next good block start address */
	uint64_t badaddr; /* the prev bad block start address */
	unsigned int move_length;   /* data length need to be move */

	badaddr = phyaddr_org;
	move_length = (((unsigned int)phyaddr_org) & (nand_logic->erasesize - 1));
	if (!move_length) {
		(*phyaddress) += nand_logic->erasesize;
		return 0;
	}

find_next_block:
	/* find next good block */
	newaddr = BLOCK_ALIGN(badaddr);
	do {
		newaddr += nand->erasesize;
		if (newaddr >= nand_logic->address + nand_logic->length) {
			printf("ERROR: Not found just one good block\n");
			return -1;
		}
	} while (bbt_cache_isbad_block(nand_logic, newaddr));

	badaddr = BLOCK_ALIGN(phyaddr_org);
	move_length = (((unsigned int)phyaddr_org) & (nand_logic->erasesize - 1));

	do {
		/*
		 * read one page from the bad block.
		 * The old block address should be marked bad after move finish.
		 * otherwise, the BB will be moved to the new block.
		 */
		ops.retlen    = 0;
		ops.oobretlen = 0;
		ops.ooboffs   = 0;

		ops.datbuf = databuf;
		ops.oobbuf = databuf + nand->writesize;
		ops.len    = nand->writesize;
		ops.ooblen = nand->oobsize;
		ops.mode   = MTD_OOB_RAW;

		ret = nand->read_oob(nand, badaddr, &ops);
		if (ret < 0) {
			printf("Error (%d) reading page 0x%08llx\n",
				ret, newaddr);
			return -1;
		}

		if (withoob) {
			nand_fill_ecc(chip, ops.oobbuf, ops.ooblen);
			/* write this page to "the next good block" */
			ret = nand->write_oob(nand, newaddr, &ops);
		} else {
			size_t pagesize = (size_t)nand->writesize;
			ret = nand_write(nand, newaddr, &pagesize, databuf);
		}

		if (ret < 0) {

			badaddr = BLOCK_ALIGN(newaddr);

			printf("Mark bad block (0x%08llx) ", badaddr);
			ret = bbt_cache_markbad_block(nand_logic, badaddr);
			if (ret) {
				printf("fail.\n");
				return -1;
			}
			printf("success.\n");
			goto find_next_block;
		}

		badaddr     += nand->writesize;
		newaddr     += nand->writesize;
		move_length -= nand->writesize;

	} while (move_length);

	(*phyaddress) = newaddr;
	return 0;
}
/******************************************************************************/

static int nand_logic_do_write(nand_logic_t *nand_logic,
			       unsigned long long phyaddress,
			       unsigned int len_excl_oob,
			       /*
			        * with oob:    len of buf = len_excl_oob
			        *                  + oobsize * NumOfPage
			        * without oob: len of buf = len_excl_oob
			        */
			       unsigned char *buf,
			       int withoob)
{
	int ret;
	u_char *bbp;
	unsigned int retlen;
	struct mtd_oob_ops ops;
	unsigned int write_length;
	unsigned int block_offset;
	nand_info_t *nand = nand_logic->nand;
	struct nand_chip *chip = nand->priv;

	if (withoob)
		retlen = len_excl_oob / nand->writesize
			* (nand->writesize + nand->oobused);
	else
		retlen = len_excl_oob;

	while (len_excl_oob > 0) {

		block_offset = phyaddress & (nand->erasesize - 1);

		WATCHDOG_RESET ();

		if (phyaddress >= nand_logic->address + nand_logic->length) {
			printf("bad block cause write end"
			       "(beyond partition length 0x%08llX)!\n",
			       nand_logic->length);
			return -1;
		}

		if (bbt_cache_isbad_block(nand_logic,
			BLOCK_ALIGN(phyaddress))) {

			printf("Skipping bad block 0x%08llX\n",
			       BLOCK_ALIGN(phyaddress));

			phyaddress += nand->erasesize;

			continue;
		}

		if (len_excl_oob < (nand->erasesize - block_offset))
			write_length = len_excl_oob;
		else
			write_length = nand->erasesize - block_offset;

		/* write a block size */
		while (write_length > 0) {

			if (withoob) {
				ops.datbuf    = buf;
				ops.oobbuf    = buf + nand->writesize;
				ops.len       = nand->writesize;
				ops.ooblen    = nand->oobused;
				ops.mode      = MTD_OOB_RAW;
				ops.retlen    = 0;
				ops.oobretlen = 0;
				ops.ooboffs   = 0;

				bbp = ops.oobbuf + chip->badblockpos;
				if (*bbp != 0xFF)
					printf("WARNING: mistaken mark bad block.\n");

				ret = nand->write_oob(nand,
					(int)phyaddress, &ops);
			} else {
				size_t pagesize = (size_t)nand->writesize;

				ops.len    = nand->writesize;
				ops.ooblen = 0;

				ret = nand_write(nand, (loff_t)phyaddress,
					&pagesize, buf);
			}

			if (ret < 0) {
				uint64_t badaddr
					= BLOCK_ALIGN(phyaddress);

				ret = move_data(nand_logic, &phyaddress,
					withoob);

				if (ret)
					return ret;

				printf("Mark bad block (0x%08llx) ", badaddr);
				ret = bbt_cache_markbad_block(nand_logic,
					badaddr);
				if (ret) {
					printf("fail. \n");
					return -1;
				}
				printf("success. \n");
				continue;
			}
			phyaddress   += nand->writesize;
			write_length -= nand->writesize;
			len_excl_oob -= nand->writesize;
			buf          += (ops.len + ops.ooblen);
		}
	}

	return (int)retlen;
}
/******************************************************************************/

/*
 * offset  - NAND write logic start address. You don't case bad block.
 *           It should be alignment with NAND page size.
 * length  - NAND write logic length. You don't case bad block.
 *           It should be alignment with NAND page size.
 * buf     - data buffer.
 *           Notes: if withoob = 1, the buf length > length, it include oob
 *                  length. withoob = 0, the buf length = length.
 * withoob - If write yaffs2 data, withoob=1, otherwise withoob = 0.
 *
 * return  - 0: success.
 *           other: fail.
 * NOTES:
 *    There is a restrict in uboot origin code, read/write should be
 *    alignment with block size, not page size.
 *    If you read/write data meet a bad block, such as: read the the 4 page
 *    of a bad block, it will skip to the next good block start address(the
 *    0 page of a block), you real want the 4 page, not 0 page.
 *    After this function (logic_to_phylength), it was impossible of the
 *    read/write first block is bad block, so we can't meet this uboot
 *    restrict.
 *
 */
int nand_logic_write(nand_logic_t *nand_logic,
		     unsigned long long offset,
		     unsigned int   length, /* page length, exclude oob data length */
		     unsigned char *buf,    /* buf length larger than 
					     * length when with oob
					     */
		     int withoob)
{
	int truncate;
	uint64_t phylength;
	uint64_t phyaddress;
	nand_info_t *nand = nand_logic->nand;

	if (!length) {
		printf("Attempt to write 0 Bytes!\n");
		return -1;
	}

	/* Reject write, which are not page aligned */
	if (!IS_PAGE_ALIGN(nand, offset)
	    || !IS_PAGE_ALIGN(nand, length)) {

		printf("Attempt to write non page aligned data, "
			"nand page size: 0x%08x, offset:"
			" 0x%08llx, length: 0x%08x\n",
			nand->writesize, offset, length);

		return -1;
	}

	/*
	 * There is a restrict in uboot origin code, read/write should be
	 * alignment with block size, not page size.
	 * If you read/write data meet a bad block, such as: read the the 4 page
	 * of a bad block, it will skip to the next good block start address(the
	 * 0 page of a block), you real want the 4 page, not 0 page.
	 * After this function (logic_to_phylength), it was impossible of the
	 * read/write first block is bad block, so we can't meet this uboot
	 * restrict.
	 */
	logic_to_phylength(nand_logic, BLOCK_ALIGN_ROUND_OUT(offset + length),
		&truncate);

	if ((offset > nand_logic->length)
		|| (length > nand_logic->length)
		|| truncate) {
		printf("Attempt to write outside the flash handle area, "
			"flash handle size: 0x%08llx, offset: 0x%08llx, "
			"length: 0x%08x\n",
			nand_logic->length, offset, length);
		return -1;
	}

	phylength = logic_to_phylength(nand_logic,
		BLOCK_ALIGN_ROUND_OUT(offset), NULL);

	if (!IS_BLOCK_ALIGN(nand_logic, offset)) {
		phyaddress = phylength - nand->erasesize
			+ (offset & (nand_logic->erasesize - 1))
			+ nand_logic->address;
	} else {
		phyaddress = phylength + nand_logic->address;
	}

	return nand_logic_do_write(nand_logic, phyaddress,
		length, buf, withoob);
}
/*****************************************************************************/
/*
 * offset  - NAND read logic start address. You don't case bad block.
 *           It should be alignment with NAND page size.
 * length  - NAND read logic length. You don't case bad block.
 *           It should be alignment with NAND page size.
 * buf     - data buffer.
 *           Notes: if withoob = 1, the buf length > length, it include oob
 *                  length. withoob = 0, the buf length = length.
 * withoob - If read yaffs2 data, withoob=1, otherwise withoob = 0.
 *
 * return  - 0: success.
 *           other: fail.
 * NOTES:
 *    There is a restrict in uboot origin code, read/write should be
 *    alignment with block size, not page size.
 *    If you read/write data meet a bad block, such as: read the the 4 page
 *    of a bad block, it will skip to the next good block start address(the
 *    0 page of a block), you real want the 4 page, not 0 page.
 *    After this function (logic_to_phylength), it was impossible of the
 *    read/write first block is bad block, so we can't meet this uboot
 *    restrict.
 *
 */
int nand_logic_read(nand_logic_t *nand_logic, unsigned long long offset,
		    unsigned int length, unsigned char *buf, int withoob)
{
	int ret;
	int truncate;
	uint64_t phylength;
	uint64_t phyaddress;
	uint64_t totalread = 0;
	unsigned int request_length = length;
	nand_info_t *nand = nand_logic->nand;
	struct nand_chip *chip = nand->priv;

	if (!length) {
		printf("Attempt to read 0 Bytes!\n");
		return -1;
	}

	/* Reject read, which are not page aligned */
	if (!IS_PAGE_ALIGN(nand, offset)
	    || !IS_PAGE_ALIGN(nand, length)) {

		printf("Attempt to read non page aligned data, "
			"nand page size: 0x%08x, offset:"
			" 0x%08llx, length: 0x%08x\n",
			nand->writesize, offset, length);

		return -1;
	}

	/* if try to read from out of flash handle area, return -1 */
	if (offset >= nand_logic->length) {
		printf("Attempt to read from outside the flash handle area, "
			"flash handle size: 0x%08llx, offset: 0x%08llx\n",
			nand_logic->length, offset);
		return -1;
	}

	phylength = logic_to_phylength(nand_logic,
		BLOCK_ALIGN_ROUND_OUT(offset), &truncate);
	if (truncate) {
		printf("Offset out of partition, because of bad block.\n");
		return -1;
	}

	if ((offset + length) > nand_logic->length) {
		length = nand_logic->length - offset;
		printf("Read length is too large, "
		       "paratition size: 0x%08llx, read offset: 0x%08llx. "
		       "Try to read 0x%08x instead!\n",
		       nand_logic->length,
		       offset,
		       length);
	}

	if (!IS_BLOCK_ALIGN(nand_logic, offset))
		phyaddress = phylength - nand->erasesize
			+ (offset & (nand_logic->erasesize - 1))
			+ nand_logic->address;
	else
		phyaddress = phylength + nand_logic->address;

	if (withoob) {
		uint64_t block_offset;
		uint64_t read_length;
		unsigned int len_withoob_org = request_length / 
			nand->writesize * (nand->writesize + nand->oobused);

		while (length > 0) {
			block_offset = phyaddress & (nand->erasesize - 1);

			WATCHDOG_RESET ();

			if (phyaddress >= (nand_logic->address
			    + nand_logic->length)) {
				printf("bad block cause write end"
				       "(beyond nand_logic->length =%#llx)!"
				       "Requested to read 0x%08x, "
				       "and read 0x%08llx successfully!\n",
				       nand_logic->length,
				       len_withoob_org,
				       totalread);
				return totalread;
			}

			if (bbt_cache_isbad_block(nand_logic,
				BLOCK_ALIGN(phyaddress))) {

				printf("Skipping bad block 0x%08llx\n",
					phyaddress & ~(nand_logic->erasesize - 1));
				phyaddress += nand->erasesize;

				continue;
			}

			if (length < (nand->erasesize - block_offset))
				read_length = length;
			else
				read_length = nand->erasesize - block_offset;

			while (read_length > 0) {
				struct mtd_oob_ops ops;

				memset(&ops, 0, sizeof(ops));
				ops.datbuf = buf;
				ops.oobbuf = buf + nand->writesize;
				ops.len    = nand->writesize;
				ops.ooblen = nand->oobused;
				ops.mode   = MTD_OOB_RAW;

				ret = nand->read_oob(nand, phyaddress, &ops);
				if (ret < 0) {
					printf("Error (%d) reading page"
						" 0x%08llx\n",
						ret, phyaddress);
					return -1;
				}

				nand_fill_ecc(chip, ops.oobbuf, ops.ooblen);

				phyaddress  += nand->writesize;
				read_length -= nand->writesize;
				length      -= nand->writesize;
				buf         += nand->writesize + nand->oobused;
				totalread   += nand->writesize + nand->oobused;
			}
		}
		if (totalread < len_withoob_org)
			printf("request to read 0x%08x, "
				"and read 0x%08llx successfully!\n",
				len_withoob_org, totalread);
		return totalread;
	} else {
		uint64_t block_offset;
		size_t read_length;

		while (length > 0) {
			block_offset = phyaddress & (nand->erasesize - 1);

			WATCHDOG_RESET ();

			if (phyaddress >= (nand_logic->address
			    + nand_logic->length)) {
				printf("Bad block cause read end address beyond"
					" nand_logic->length(0x%#llx). "
					"Requested to read 0x%08x, "
					"real read 0x%08llx.\n",
					nand_logic->length,
					request_length,
					totalread);
				return totalread;
			}

			if (bbt_cache_isbad_block(nand_logic,
				BLOCK_ALIGN(phyaddress))) {
				printf("Skipping bad block 0x%08llx\n",
				       BLOCK_ALIGN(phyaddress));
				phyaddress += nand->erasesize;
				continue;
			}

			if (length < (nand->erasesize - block_offset))
				read_length = length;
			else
				read_length = nand->erasesize - block_offset;

			ret = nand_read(nand, phyaddress, &read_length, buf);
			if (ret < 0)
				return ret;

			phyaddress  += read_length;
			length      -= read_length;
			buf         += read_length;
			totalread   += read_length;
		}
		if (totalread < request_length)
			printf("request to read 0x%08x, "
				"and read 0x%08llx successfully!\n",
				request_length, totalread);
		return totalread;
	}
}
/*****************************************************************************/

#endif /* CONFIG_CMD_NAND */
