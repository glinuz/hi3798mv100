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
#include <flash.h>

#define MOULE_NAME                     "NAND: "
#include <debug.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "nand_drv.h"

static struct flash_info_t nand_info = {0,};

/*****************************************************************************/

int set_nand_info(struct nand_dev_t *nand_dev)
{
	struct nand_chip_info_t *nand_chip_info = nand_dev->nand_info;
	struct spi_nand_chip_info_t *spi_nand_chip_info = nand_dev->spi_nand_info;

	if (nand_chip_info) {
		nand_info.type = FT_NAND;
		strncpy(nand_info.name, nand_chip_info->name, 
			sizeof(nand_info.name));
		nand_info.nr_ids = min(nand_chip_info->length, 
				       sizeof(nand_info.ids));
		memcpy(nand_info.ids, nand_chip_info->id, nand_info.nr_ids);

	} else if (spi_nand_chip_info) {
		nand_info.type = FT_NAND;
		strncpy(nand_info.name, spi_nand_chip_info->name, 
			sizeof(nand_info.name));
		nand_info.nr_ids = min(spi_nand_chip_info->length, 
				       sizeof(nand_info.ids));
		memcpy(nand_info.ids, spi_nand_chip_info->id, nand_info.nr_ids);
	}

	nand_info.name[sizeof(nand_info.name) - 1] = '\0';
	nand_info.pagesize = nand_dev->pagesize;
	nand_info.blocksize = nand_dev->blocksize;
	nand_info.chipsize = nand_dev->chipsize;
	nand_info.oobsize = nand_dev->oobsize;
	nand_info.oobused = nand_dev->oobused;
	nand_info.nr_chips = nand_dev->nr_chip;

	return 0;
}
/*****************************************************************************/

int nand_check_range64(uint64 address, uint64 border, uint64 *size,
		       int truncated)
{
	struct nand_dev_t *nand_dev = get_nand_dev();

	if (!nand_dev)
		return -ENODEV;

	if (!border || border > nand_dev->totalsize)
		border = nand_dev->totalsize;

	if (address >= border) {
		pr_error("address(0x%08llx) is out of range(0x%08llx).\n",
			address, border);
		return -EINVAL;
	}

	if (size && (address + (*size) > border)) {
		if (truncated) {
			*size = border - address;
			pr_warn("length is out of range, it is truncated to 0x%08llx.\n",
				*size);
		} else {
			pr_error("size 0x%08llx) is out of range(0x%08llx).\n",
				*size, border);
			return -EINVAL;
		}
	}

	return 0;
}
/*****************************************************************************/

int nand_check_range(const uint64 *address, uint64 *size)
{
	return nand_check_range64(*address, 0, size, TRUE);
}
/*****************************************************************************/

int nand_check_align64(uint64 address, uint64 border, uint64 *size,
		       int blockalign)
{
	int ret;
	int alignsize;
	uint32 mask;
	char *mask_name;
	struct nand_dev_t *nand_dev = get_nand_dev();

	if (!nand_dev)
		return -ENODEV;

	ASSERT(border <= nand_dev->totalsize);

	ret = nand_check_range64(address, border, size, TRUE);
	if (ret)
		return ret;

	if (blockalign) {
		mask = nand_dev->blockmask;
		mask_name = "blocksize";
		alignsize = nand_dev->blocksize;
	} else {
		mask = nand_dev->pagemask;
		mask_name = "pagesize";
		alignsize = nand_dev->pagesize;
	}

	if ((uint32)address & mask) {
		pr_error("address(0x%08llx) must align to %s 0x%08x.\n",
			 address, mask_name, alignsize);
		return -EINVAL;
	}

	if (size && ((uint32)(*size) & mask)) {
		pr_error("size(0x%08llx) must align to %s 0x%08x.\n",
			 *size, mask_name, alignsize);
		return -EINVAL;
	}

	return 0;
}
/*****************************************************************************/

int nand_check_align(const uint64 *address, uint64 *size, int blockalign)
{
	return nand_check_align64(*address, 0, size, blockalign);
}
/*****************************************************************************/
/*
 * input:
 *   address   - write start address.
 *   border    - write stop address, 0 means NAND totalsize
 *   buf       - the buf be written. if withoob = 1, buflen = size + oobsize
 *                                      withoob = 0, buflen = size
 *   size      - write length, align with pagesize. not include oobsize
 *   withoob   - write data include oob data.
 *   skipempty - skip empty page.
 *
 * output:
 *   address  - real address after writing, include bad block length.
 *   oobsize  - total oobsize, not include bad block length.
 *   return:
 *    >=  0   - the real write size (align with pagesize).
 *               the buf should smaller 2G.
 *    <  0    - write fail.
 *
 * notice:
 *    1. when write fail, this function will mark NAND bad block.
 *
 */
int nand_write64(uint64 *address, uint64 border, char *buf, uint64 size,
		 uint32 *oobsize, int withoob, int skipempty)
{
	int ret;
	uint32 sz_page;
	uint32 sz_oob;
	uint32 real_sz_page = 0;
	uint64 real_address = *address;
	struct nand_dev_t *nand_dev;

	if (!size)
		return 0;

	if (size >= _1G) {
		size = _1G;
		pr_warn("size is too larger, it is truncated to 0x%08x.\n",
			size);
	}

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return -ENODEV;

	if (!border || border > nand_dev->totalsize)
		border = nand_dev->totalsize;

	ret = nand_check_align64(real_address, border, &size, FALSE);
	if (ret)
		return ret;

	if (withoob)
		*oobsize = 0;

	while (real_address < border && size > 0) {
		if (hinfc_bad_block(nand_dev, real_address)) {
			pr_info("skip bad block at 0x%ll08x.\n", real_address);
			real_address += nand_dev->blocksize;
			continue;
		}

		sz_page = (uint32)size;

		ret = hinfc_write_one_block(nand_dev, real_address, buf, &sz_page,
					    &sz_oob, withoob, skipempty);
		if (ret) {
			pr_info("write 0x%ll08x fail, mark bad block.\n",
				real_address);
			hinfc_mark_bad(nand_dev, real_address);
			real_address += nand_dev->blocksize;
			continue;
		}

		ASSERT((uint32)size >= sz_page);

		real_sz_page += sz_page;
		size -= sz_page;

		buf += sz_page;
		if (withoob) {
			buf += sz_oob;
			*oobsize += sz_oob;
		}
		real_address += sz_page;
	}

	*address = real_address;

	return (int)real_sz_page;
}
/*****************************************************************************/

int nand_write(uint64 *address, char *buf, const uint64 *size, uint32 *oobsize,
	       int withoob, int skipempty)
{
	return nand_write64(address, 0, buf, *size, oobsize, withoob, skipempty);
}
/*****************************************************************************/
/*
 * input:
 *   address     - write start address.
 *   border      - read stop address, 0 means NAND totalsize
 *   buf         - the buf be read. if withoob = 1, buflen = size + oobsize
 *                                     withoob = 0, buflen = size
 *   size        - read length, align with pagesize;
 *   withoob     - read data include oob data.
 *
 * output:
 *   address - real address after reading, include skip bad block length.
 *   oobsize - total oobsize, not include bad block length.
 *   return:
 *    >  0   - the real read size (align with pagesize), not include bad block.
 *             the buf should smaller 2G.
 *    <= 0   - read fail.
 *
 */
int nand_read64(uint64 *address, uint64 border, char *buf, uint64 size,
		uint32 *oobsize, int withoob, int skipbad)
{
	int ret;
	uint32 sz_page;
	uint32 sz_oob;
	uint32 real_sz_page = 0;
	uint64 real_address = *address;
	struct nand_dev_t *nand_dev;

	if (!size)
		return 0;

	if (size >= _1G) {
		size = _1G;
		pr_warn("size is too larger, it is truncated to 0x%08llx.\n",
			size);
	}

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return -ENODEV;

	if (!border || border > nand_dev->totalsize)
		border = nand_dev->totalsize;

	ret = nand_check_align64(real_address, border, &size, FALSE);
	if (ret)
		return ret;

	if (oobsize)
		*oobsize = 0;

	while (real_address < border && size > 0) {
		if (skipbad && hinfc_bad_block(nand_dev, real_address)) {
			pr_info("skip bad block at 0x%ll08x.\n",
				real_address);
			real_address += nand_dev->blocksize;
			continue;
		}

		sz_page = (uint32)size;

		hinfc_read_one_block(nand_dev, real_address, buf, &sz_page,
				     &sz_oob, withoob);

		ASSERT((uint32)size >= sz_page);

		real_sz_page += sz_page;
		size -= sz_page;

		buf += sz_page;
		if (withoob) {
			buf += sz_oob;
			*oobsize += sz_oob;
		}

		real_address += sz_page;
	}

	*address = real_address;

	return (int)real_sz_page;
}
/*****************************************************************************/

int nand_read(uint64 *address, char *buf, const uint64 *size, uint32 *oobsize,
	      int withoob, int skipbad)
{
	return nand_read64(address, 0, buf, *size, oobsize, withoob, skipbad);
}
/*****************************************************************************/
/*
 * return:
 *    -1 - mark fail.
 *     0 - mark ok.
 */
int nand_mark_bad64(uint64 address)
{
	int ret;
	struct nand_dev_t *nand_dev;

	ret = nand_check_range64(address, 0, NULL, TRUE);
	if (ret)
		return ret;

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return -ENODEV;

	return hinfc_mark_bad(nand_dev, address);
}
/*****************************************************************************/

int nand_mark_bad(const uint64 *address)
{
	return nand_mark_bad64(*address);
}
/*****************************************************************************/
/*
 * return:
 *    1 - nand block is bad
 *    0 - nand block is good.
 */
int nand_bad_block64(uint64 address)
{
	int ret;
	struct nand_dev_t *nand_dev;

	ret = nand_check_range64(address, 0, NULL, TRUE);
	if (ret)
		return 1;

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return 1;

	return hinfc_bad_block(nand_dev, address);
}
/*****************************************************************************/

int nand_bad_block(const uint64 *address)
{
	return nand_bad_block64(*address);
}
/*****************************************************************************/
/*
 * @border      - erase stop address, 0 means NAND totalsize
 * @size        - erase size, not include bad block.
 * notice:
 *    1. when write fail, this function will mark NAND bad block.
 */
int64 nand_erase_block64(uint64 address, uint64 border, uint64 size, int force)
{
	int ret;
	int64 real_sz_page = 0;
	struct nand_dev_t *nand_dev;

	if (!size)
		return 0;

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return -ENODEV;

	if (!border || border > nand_dev->totalsize)
		border = nand_dev->totalsize;

	ret = nand_check_align64(address, border, &size, TRUE);
	if (ret)
		return ret;

	while (address < border && size > 0) {
		if (!force && hinfc_bad_block(nand_dev, address)) {
			pr_info("skip bad block at 0x%ll08x.\n", address);
			goto next_block;
		}

		ret = hinfc_erase_one_block(nand_dev, address);
		if (ret) {
			pr_info("erase 0x%ll08x fail, mark bad block.\n",
				address);
			hinfc_mark_bad(nand_dev, address);
			goto next_block;
		}

		real_sz_page += nand_dev->blocksize ;
		size -= nand_dev->blocksize;
next_block:
		address += nand_dev->blocksize;
	}

	return real_sz_page;
}
/*****************************************************************************/

int64 nand_erase_block(const uint64 *address, const uint64 *size, int force)
{
	return nand_erase_block64(*address, *size, *size, force);
}
/*****************************************************************************/

struct flash_info_t *get_nand_info(void)
{
	if (nand_info.type != FT_NAND)
		get_nand_dev();
	return &nand_info;
}
/*****************************************************************************/
/*
 * input:
 *    address  - start address. should align to blocksize.
 *    border   - stop address, should align to blocksize,
 *    puresize - valid data size, not include bad block. should align to pagesize
 *
 * output:
 *    address  - stop address include bad block and with puresize of good block.
 *    return  < 0 fail
 *            > 0 real size without bad block. if not out border,
 *                it always equal to puresize.
 */
int64 nand_size_withbad64(uint64 *address, uint64 border, uint64 puresize)
{
	int ret;
	int64 real_sz_page = 0;
	uint32 sz_block_mask = 0;
	uint64 real_address = *address;
	struct nand_dev_t *nand_dev;

	nand_dev = get_nand_dev();
	if (!nand_dev)
		return -ENODEV;

	if (!border || border > nand_dev->totalsize)
		border = nand_dev->totalsize;

	sz_block_mask = puresize & nand_dev->blockmask;
	puresize -= sz_block_mask;

	ret = nand_check_align64(real_address, border, &puresize, TRUE);
	if (ret)
		return ret;

	while (real_address < border && puresize > 0) {
		if (hinfc_bad_block(nand_dev, real_address))
			goto next_block;

		real_sz_page += nand_dev->blocksize ;
		puresize -= nand_dev->blocksize;
next_block:
		real_address += nand_dev->blocksize;
	}

	*address = real_address + sz_block_mask;

	return (real_sz_page + sz_block_mask);
}
