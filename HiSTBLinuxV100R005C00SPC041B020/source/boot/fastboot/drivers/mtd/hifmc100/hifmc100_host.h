/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.6.29
 *
******************************************************************************/

#ifndef HIFMC100_HOSTH
#define HIFMC100_HOSTH
/******************************************************************************/
#include <spi_flash.h>

#include "hifmc100_reg.h"
#include "hiflash.h"
/******************************************************************************/

struct hifmc_host;

struct hifmc_reg {
	u32 op_cfg;
	u32 cmd;
	u32 op_ctrl;

	int clock;
	int dummy;
};

struct hifmc_xnand {
	int ifmode;
	char *name;
	struct mtd_info mtd[1];
	struct nand_chip *chip;
	struct device *dev;

	struct cmdfunc_data cmdfunc[1];   /* mtd operation function parameter */
	struct mtd_func mtd_func[1];      /* mtd call function */

	struct hifmc_host *host;

	struct nand_ctrl_info *ctrl_info;   /* nand controller information */

	struct nand_read_retry *read_retry; /* read retry function */

	struct flash_regop regop[1];

	struct spinand_driver *spinand_drv;

	int ecc_strength;
	int pagesize;
	int oobsize;
	int blocksize;
	int page_per_block;

	struct {
		u32 fmc_cfg_ecc_type;
		u32 fmc_cfg_page_size;
		struct hifmc_reg read;
		struct hifmc_reg write;
		u32 fmc_pnd_pwidth_cfg;
	} reg;

	u32 randomizer_en;

	u32 is_empty_page;
	u32 is_bad_block;
	u32 is_uc_err;

	char *pagebuf, *org_pagebuf;
	char *oobbuf;

	ulong clk_rate;

	int erase_addr_cycle; /* erase addr cycle */
	int read_addr_cycle;  /* read/write addr cycle */
	union {
		u8 manuf;
		u8 id[8];
	};

	u8 *(*read_raw_bbm)(struct hifmc_xnand *xnand);
	void (*cmd_readid)(struct hifmc_xnand *xnand);
	void (*cmd_reset)(struct hifmc_xnand *xnand);
	void (*cmd_erase)(struct hifmc_xnand *xnand);

	void (*shutdown)(struct hifmc_xnand *xnand);
	struct flash_regop_intf regop_intf;
};

struct hifmc_spinor {
	struct mtd_info mtd[1];
	struct device *dev;

	char *name;
	int nr_chips;
	u32 chipsize; /* one chip size, when nr_chips > 1 */
	int chipsize_shift;
	int chipsize_mask;
	u32 erasesize;

	u32 addr_cycle;

	int chipselect[HIFMC100_OP_CFG_NUM_CS];

#define HIFMC100_SPINOR_DMA_SIZE                (4096)
#define HIFMC100_SPINOR_DMA_SIZE_MASK           (HIFMC100_SPINOR_DMA_SIZE - 1)
	char dma_vaddr[HIFMC100_SPINOR_DMA_SIZE];
	dma_addr_t dma_paddr;

	struct flash_regop regop[1];

	struct spinor_driver *drv;

	struct {
		struct hifmc_reg read;
		struct hifmc_reg write;
		struct hifmc_reg erase;
	} xfer;

	struct hifmc_host *host;

	void (*shutdown)(struct hifmc_spinor *spinor);
	struct flash_regop_intf regop_intf;
};
/******************************************************************************/

struct hifmc_host {
	struct spi_flash spiflash[1];
	void __iomem *regbase;
	void __iomem *iobase;
	int sz_iobase;

	struct device *dev;

	u32 intr_status;

	int flags;

	int ifmode;
	void (*set_ifmode)(struct hifmc_host *host, int ifmode);
	void (*irq_enable)(struct hifmc_host *host);
	int (*wait_host_ready)(struct hifmc_host *host);
	int (*set_clock)(struct hifmc_host *host, unsigned int clock, int clk_en);
	int chipselect;

	struct {
		u32 fmc_cfg_ecc_type;
		u32 fmc_cfg_page_size;
	} reg;

	struct hifmc_xnand *nand;
	struct hifmc_xnand *spinand;
	struct hifmc_spinor *spinor;
};

#define SPIFLASH_TO_HOST(_spiflash)     ((struct hifmc_host *)(_spiflash))

extern struct hifmc_host *host;

#define HIFMC_IFMODE_INVALID         0
#define HIFMC_IFMODE_SPINOR          1
#define HIFMC_IFMODE_NAND            2
#define HIFMC_IFMODE_SPINAND         3

int hifmc100_write_reg(struct flash_regop_info *info);

int hifmc100_read_reg(struct flash_regop_info *info);

struct spi_flash *hifmc100_spinor_probe(void);

int hifmc100_xnand_probe(struct nand_chip *chip, int ifmode);

extern struct nand_chip_clock hifmc100_nand_rwlatch[];

extern struct spinand_chip_xfer hifmc100_spinand_chip_xfer[];

extern struct spinor_chip_xfer hifmc100_spinor_chip_xfer[];

extern int hifmc100_set_system_clock(struct hifmc_host *host, 
				     unsigned int clk, int clk_en);
int hifmc100_host_init(void);

void hifmc100_set_ecc_randomizer(struct hifmc_xnand *xnand, int enable);
/******************************************************************************/
#define FMC_WAIT_TIMEOUT 10000000
#define FMC_DMA_WAIT_INT_FINISH(_host) \
	do { \
		unsigned regval, timeout = FMC_WAIT_TIMEOUT; \
		do { \
			regval = hifmc_read((_host), HIFMC100_INT); \
			--timeout; \
		} while ((!(regval & HIFMC100_INT_OP_DONE) && timeout)); \
		if (!timeout) { \
			PR_BUG("Wait dma int finish timeout!\n"); \
		} \
	} while (0)

/******************************************************************************/
#endif /* HIFMC100_HOSTH */
