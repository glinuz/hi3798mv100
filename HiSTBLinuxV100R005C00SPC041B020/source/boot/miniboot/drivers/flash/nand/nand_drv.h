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

#ifndef NAND_DRVH
#define NAND_DRVH
/******************************************************************************/

#include <stddef.h>
#include <flash.h>

#include "nand_ctrl.h"
#include "nand_chip.h"
#include "spi_nand_chip.h"

/* nand driver interface. */
struct nand_dev_t {
	uint64 totalsize; /* chipsize * nr_chip */
	uint64 chipsize;  /* single NAND chip size */
	int blocksize;
	int pagesize;
	int ecctype;
	int oobsize;  /* controller use oobsize; chip oobsize is in "nand_chip_info_t" */
	int oobused;  /* yaffs2 filesystem use 32B for NANDCV610 */
	int nr_chip;
	struct nand_ctrl_oob_t *layout;

	int pageshift;
	int pagemask;
	int blockshift;
	int blockmask;
	int chipshift;
	uint64 chipmask;
	int is_spi_nand;

	unsigned int options;

	char *bootmsg;

	struct nand_chip_info_t *nand_info;
	struct spi_nand_chip_info_t *spi_nand_info;

/* (Controller) Current support ecc/page detect, in some condition it does not support. */
#define NANDC_HW_AUTO                         0x01
/* (Controller) support ecc/page detect, and current controller ecc/page has configured finish */
#define NANDC_CONFIG_DONE                     0x02
/* (Controller) has configured sync mode by hardware PIN, default is async */
#define NANDC_IS_SYNC_BOOT                    0x04
	int flags;

#define NAND_BBT_INVALID             0
#define NAND_BBT_GOOD                1
#define NAND_BBT_BAD                 2
	uint8 *bbt;
	int sz_bbt;

	char *pagebuf;
	char *oobbuf;

#define NAND_DEV_BB(_nand_dev)  (*(_nand_dev)->bbm != 0xFF)
	uint8 *bbm;

	/*
	 * read one page, include oob area
	 *
	 * input:
	 *    address - address, should be aligned to pagesize.
	 *
	 * output:
	 *    return - 0 success.
	 *
	 * note:
	 *    after reading, get data from nand_dev->pagebuf and nand_dev->oobbuf.
	 */
	int (*read_page)(struct nand_dev_t *nand_dev, uint64 address);

	/*
	 * write one page, include oob area
	 *
	 * input:
	 *    address - address, should be aligned to pagesize.
	 *
	 * output:
	 *    return - 0 success.
	 *             1 fail. because of NAND statue register set fail bit.
	 *
	 * note:
	 *    before writing, set data to nand_dev->pagebuf and nand_dev->oobbuf.
	 */
	int (*write_page)(struct nand_dev_t *nand_dev, uint64 address);

	/*
	 * erase one block
	 *
	 * input:
	 *    address - address, should be aligned to blocksize.
	 *
	 * output:
	 *    return - 0 success.
	 *             1 fail. because of NAND statue register set fail bit.
	 *
	 * note:
	 *    erase, even if the block is bad
	 */
	int (*erase_block)(struct nand_dev_t *nand_dev, uint64 address);

	/*
	 * output:
	 *    return - 0 success.
	 *             1 fail. because of NAND statue register set fail bit.
	 */
	int (*reset)(struct nand_dev_t *nand_dev, int chipselect);

	int (*read_id)(struct nand_dev_t *nand_dev, int chipselect,
		       char *buf, int buflen);

	int (*adjust_param)(struct nand_dev_t *nand_dev);

	void *priv;
};

typedef struct nand_dev_t *(*nand_driver_init_fn)(void);

extern nand_driver_init_fn nand_driver_init;
extern nand_driver_init_fn spi_nand_driver_init;

int set_nand_info(struct nand_dev_t *nand_dev);

struct nand_dev_t *get_nand_dev(void);

/*****************************************************************************/

int hinfc_write_one_block(struct nand_dev_t *nand_dev, uint64 address, char *buf,
			  uint32 *sz_page, uint32 *sz_oob, int withoob,
			  int skipempty);

int hinfc_read_one_block(struct nand_dev_t *nand_dev, uint64 address, char *buf,
			 uint32 *sz_page, uint32 *sz_oob, int withoob);

int hinfc_bad_block(struct nand_dev_t *nand_dev, uint64 address);

int hinfc_mark_bad(struct nand_dev_t *nand_dev, uint64 address);

int hinfc_erase_one_block(struct nand_dev_t *nand_dev, uint64 address);

/*****************************************************************************/

int nand_check_range64(uint64 address, uint64 border, uint64 *size,
		       int truncated);

int nand_check_align64(uint64 address, uint64 border, uint64 *size,
		       int blockalign);

int nand_write64(uint64 *address, uint64 border, char *buf, uint64 size,
		 uint32 *oobsize, int withoob, int skipempty);

int nand_read64(uint64 *address, uint64 border, char *buf, uint64 size,
		uint32 *oobsize, int withoob, int skipbad);

int nand_mark_bad64(uint64 address);

int nand_bad_block64(uint64 address);

int64 nand_erase_block64(uint64 address, uint64 border, uint64 size, int force);

int64 nand_size_withbad64(uint64 *address, uint64 border, uint64 puresize);

/******************************************************************************/
#endif /* NAND_DRVH */
