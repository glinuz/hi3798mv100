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

#include <stddef.h>
#include <errno.h>
#include <string.h>

#define MOULE_NAME                     "nfc_if: "
#include <debug.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "nand_drv.h"

/*****************************************************************************/

static void nand_fill_oob_user(struct nand_dev_t *nand_dev)
{
	struct nand_ctrl_oob_t *layout = nand_dev->layout;

	*(uint16 *)nand_dev->oobbuf = 0xFFFF;

	memset((char *)nand_dev->oobbuf + layout->offset, 0xFF,
		layout->length);
}
/*****************************************************************************/

static void nand_fill_oob_ecc(struct nand_dev_t *nand_dev)
{
	char *oobbuf = nand_dev->oobbuf;
	struct nand_ctrl_oob_t *layout = nand_dev->layout;
	uint32 size = layout->offset + layout->length;

	if (layout->offset > 2)
		memset(oobbuf + 2, 0xFF, layout->offset - 2);

	memset(oobbuf + size, 0xFF, nand_dev->oobsize - size);
}
/*****************************************************************************/

static void nand_bbt_set(struct nand_dev_t *nand_dev, uint64 address, int mark)
{
	uint8 *bbm;
	int pos = address >> nand_dev->blockshift;

	ASSERT(pos < nand_dev->sz_bbt);

	bbm = &nand_dev->bbt[pos];

	ASSERT(*bbm == NAND_BBT_INVALID || *bbm == NAND_BBT_GOOD || *bbm == NAND_BBT_BAD);

	*bbm = mark;
}
/*****************************************************************************/

static int nand_bbt_get(struct nand_dev_t *nand_dev, uint64 address)
{
	int pos = address >> nand_dev->blockshift;

	ASSERT(pos < nand_dev->sz_bbt);

	return nand_dev->bbt[pos];
}
/*****************************************************************************/
/*
 * input:
 *    address  - write address, align to pagesize
 *    buf      - the data buffer be written.
 *    sz_page  - the writing size, align with pagesize.
 *    withoob  - the buffer whether include oob data.
 * ouput:
 *    sz_page  - the real writing size, align with pagesize, the sz_page value
 *               always smaller than blocksize.
 *    sz_oob   - the real oob writing size,
 *    return   - -1 fail, because bad block.
 *                0 success.
 */
int hinfc_write_one_block(struct nand_dev_t *nand_dev, uint64 address, char *buf,
			  uint32 *sz_page, uint32 *sz_oob, int withoob,
			  int skipempty)
{
	int ret;
	uint32 real_sz_page = 0; /* real read page data size, align to page */
	uint32 real_sz_oob = 0;  /* real read oob data size */
	uint32 buflen = nand_dev->pagesize;

	ASSERT(!(address & nand_dev->pagemask) && !(*sz_page & nand_dev->pagemask));
	ASSERT(address < nand_dev->totalsize);

	if (withoob)
		buflen += nand_dev->oobused;
	else
		nand_fill_oob_user(nand_dev);

	do {
		if (skipempty) {
			int loop = buflen >> 2;
			uint32 *tmp = (uint32 *)buf;
			uint32 value = 0xFFFFFFFF;

			ASSERT(!((uint32)buf & 0x03) && !(buflen & 0x03));

			while (loop-- > 0)
				value &= *tmp++;

			if (value == 0xFFFFFFFF)
				goto next_page;
		}
		
		memcpy(nand_dev->pagebuf, buf, buflen);
		buf += buflen;

		if (withoob && NAND_DEV_BB(nand_dev) &&
		    !(address & nand_dev->blockmask))
			nand_bbt_set(nand_dev, address, NAND_BBT_BAD);

		ret = nand_dev->write_page(nand_dev, address);
		ASSERT(ret == 0 || ret == -1);
		if (ret)
			break;

next_page:
		address += nand_dev->pagesize;

		real_sz_page += nand_dev->pagesize;
		real_sz_oob += nand_dev->oobused;

	} while ((address & nand_dev->blockmask) && real_sz_page < *sz_page);

	ASSERT(real_sz_page <= *sz_page);

	*sz_page = real_sz_page;
	if (sz_oob)
		*sz_oob  = real_sz_oob;

	return ret;
}
/*****************************************************************************/
/*
 * input:
 *    address  - read address, align to pagesize
 *    buf      - the reading data buffer.
 *    sz_page  - the writing size, align with pagesize.
 *    withoob  - buffer whether include oob data.
 * ouput:
 *    sz_page  - the real reading size, align with pagesize
 *    sz_oob   - the real oob reading size,
 *    return   - 0 success.
 */
int hinfc_read_one_block(struct nand_dev_t *nand_dev, uint64 address, char *buf,
			 uint32 *sz_page, uint32 *sz_oob, int withoob)
{
	uint32 real_sz_page = 0;
	uint32 real_sz_oob = 0;
	uint32 buflen = nand_dev->pagesize;

	ASSERT(!(address & nand_dev->pagemask) && !(*sz_page & nand_dev->pagemask));
	ASSERT(address < nand_dev->totalsize);

	if (withoob)
		buflen += nand_dev->oobused;

	do {
		nand_dev->read_page(nand_dev, address);

		if (withoob)
			nand_fill_oob_ecc(nand_dev);

		memcpy(buf, nand_dev->pagebuf, buflen);
		buf += buflen;

		address += nand_dev->pagesize;
		real_sz_page += nand_dev->pagesize;
		real_sz_oob += nand_dev->oobused;
	} while ((address & nand_dev->blockmask) && real_sz_page < *sz_page);

	ASSERT(real_sz_page <= *sz_page);

	*sz_page = real_sz_page;

	if (sz_oob)
		*sz_oob  = real_sz_oob;

	return 0;
}
/*****************************************************************************/

int hinfc_bad_block(struct nand_dev_t *nand_dev, uint64 address)
{
	int mark;

	ASSERT(address < nand_dev->totalsize);

	mark = nand_bbt_get(nand_dev, address);
	if (mark == NAND_BBT_INVALID) {
		address &= ~nand_dev->blockmask;
		nand_dev->read_page(nand_dev, address);
		mark = NAND_DEV_BB(nand_dev) ? NAND_BBT_BAD : NAND_BBT_GOOD;
		nand_bbt_set(nand_dev, address, mark);
	}

	return (mark == NAND_BBT_BAD);
}
/*****************************************************************************/

int hinfc_mark_bad(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret = 0;
	int loop;

	address &= ~nand_dev->blockmask;

	ASSERT(address < nand_dev->totalsize);

	if (nand_bbt_get(nand_dev, address) == NAND_BBT_BAD)
		return 0;

	loop = 0;

mark_again:

	*(uint16 *)nand_dev->bbm = 0x0000;
	ret = nand_dev->write_page(nand_dev, address);
	if (!ret)
		goto mark_ok;

	nand_dev->read_page(nand_dev, address);
	if (NAND_DEV_BB(nand_dev)) {
		ret = 0;
		goto mark_ok;
	}

	if (loop++ > 0) {
		ret = -1;
		pr_error("NAND mark bad block fail.\n");
		goto mark_ok;
	}

	nand_dev->erase_block(nand_dev, address);

	goto mark_again;

mark_ok:
	nand_bbt_set(nand_dev, address, NAND_BBT_BAD);
	return ret;
}
/*****************************************************************************/
/*
 *  return:
 *    0  - pass
 *    -1 - NAND status register give FAIL bit.
 * 
 */
int hinfc_erase_one_block(struct nand_dev_t *nand_dev, uint64 address)
{
	int ret;

	ASSERT(address < nand_dev->totalsize && !(address & nand_dev->blockmask));

	nand_bbt_set(nand_dev, address, NAND_BBT_GOOD);

	ret = nand_dev->erase_block(nand_dev, address);
	ASSERT(ret == 0 || ret == -1);

	nand_bbt_set(nand_dev, address, ret ? NAND_BBT_INVALID : NAND_BBT_GOOD);

	return ret;
}
