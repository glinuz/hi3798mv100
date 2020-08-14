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

#include <hiflash.h>
#include <flash_cache.h>
#include <linux/hisilicon/flash_stats.h>

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
	struct nand_chip chip[1];
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

	bool randomizer_en;

	bool is_empty_page;
	bool is_bad_block;
	bool is_uc_err;

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
	void (*suspend)(struct hifmc_xnand *xnand);
	void (*resume)(struct hifmc_xnand *xnand);

	struct flash_regop_intf regop_intf;

	struct flash_stats *stats;

	struct flash_cache *cache;
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
	void *dma_vaddr;
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
	void (*suspend)(struct hifmc_spinor *spinor);
	void (*resume)(struct hifmc_spinor *spinor);

	struct flash_regop_intf regop_intf;
};
/******************************************************************************/

struct hifmc_host {
	void __iomem *regbase;
	void __iomem *iobase;
	int sz_iobase;

	struct mutex lock;
	struct device *dev;
	unsigned int irq;
	struct completion dma_done;
	struct clk *clk;

	u32 intr_status;

	int ifmode;
	void (*set_ifmode)(struct hifmc_host *host, int ifmode);
	void (*irq_enable)(struct hifmc_host *host);
	int (*wait_dma_finish)(struct hifmc_host *host);
	int (*wait_host_ready)(struct hifmc_host *host);
	int chipselect;

	struct {
		u32 fmc_cfg_ecc_type;
		u32 fmc_cfg_page_size;
	} reg;

	struct hifmc_xnand *nand;
	struct hifmc_xnand *spinand;
	struct hifmc_spinor *spinor;
};

#define HIFMC_IFMODE_INVALID         0
#define HIFMC_IFMODE_SPINOR          1
#define HIFMC_IFMODE_NAND            2
#define HIFMC_IFMODE_SPINAND         3

int hifmc100_write_reg(struct flash_regop_info *info);

int hifmc100_read_reg(struct flash_regop_info *info);

int hifmc100_spinor_probe(struct platform_device *pdev, struct hifmc_host *host);

int hifmc100_xnand_probe(struct platform_device *pdev, struct hifmc_host *host,
			 int ifmode);

extern struct nand_chip_clock hifmc100_nand_rwlatch[];

extern struct spinand_chip_xfer hifmc100_spinand_chip_xfer[];

extern struct spinor_chip_xfer hifmc100_spinor_chip_xfer[];

/******************************************************************************/
#endif /* HIFMC100_HOSTH */
