/******************************************************************************
 *    NAND Flash Controller V610 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *
******************************************************************************/

#ifndef HINFCV610H
#define HINFCV610H
/******************************************************************************/

#ifndef CONFIG_HINFC610_W_LATCH
#  define CONFIG_HINFC610_W_LATCH                     (0xa)
#endif /* CONFIG_HINFC610_W_LATCH */

#ifndef CONFIG_HINFC610_R_LATCH
#  define CONFIG_HINFC610_R_LATCH                     (0xa)
#endif /* CONFIG_HINFC610_R_LATCH */

#ifndef CONFIG_HINFC610_RW_LATCH
#  define CONFIG_HINFC610_RW_LATCH                    (0xa)
#endif /* CONFIG_HINFC610_RW_LATCH */

#ifndef CONFIG_HINFC610_MAX_CHIP
#  define CONFIG_HINFC610_MAX_CHIP                    (1)
#  warning NOT config CONFIG_HINFC610_MAX_CHIP, used default value, maybe invalid.
#endif /* CONFIG_HINFC610_MAX_CHIP */

/*****************************************************************************/
#define HINFC610_REG_BASE_ADDRESS_LEN                 (0x100)
#define HINFC610_BUFFER_BASE_ADDRESS_LEN              (2048 + 128)

#define HINFC610_CHIP_DELAY                           (25)

#define HINFC610_ADDR_CYCLE_MASK                      0x4
#define HINFC610_DMA_ADDR_OFFSET                      4096
/*****************************************************************************/
#define HINFC610_CON                                 0x00
#define HINFC610_CON_OP_MODE_NORMAL      (1U << 0)
#define HINFC610_CON_PAGEISZE_SHIFT      (1)
#define HINFC610_CON_PAGESIZE_MASK       (0x07)
#define HINFC610_CON_BUS_WIDTH           (1U << 4)
#define HINFC610_CON_READY_BUSY_SEL      (1U << 8)
#define HINFC610_CON_ECCTYPE_SHIFT       (9)
#define HINFC610_CON_ECCTYPE_MASK        (0x0f)
#define HINFC610_CON_RANDOMIZER_EN       (1 << 14)
#define HINFC610_CON_NF_MODE_SHIFT       15
#define HINFC610_CON_NF_MODE_MASK        (3 << HINFC610_CON_NF_MODE_SHIFT)
#define HINFC610_CON_NF_MODE_TOGGLE      (1 << HINFC610_CON_NF_MODE_SHIFT)
#define HINFC610_CON_NF_MODE_ONFI_23     (2 << HINFC610_CON_NF_MODE_SHIFT)
#define HINFC610_CON_NF_MODE_ONFI_30     (3 << HINFC610_CON_NF_MODE_SHIFT)

#define HINFC610_PWIDTH                              0x04
#define SET_HINFC610_PWIDTH(_w_lcnt, _r_lcnt, _rw_hcnt) \
	((_w_lcnt) | (((_r_lcnt) & 0x0F) << 4) | (((_rw_hcnt) & 0x0F) << 8))

#define HINFC610_CMD                                 0x0C
#define HINFC610_ADDRL                               0x10
#define HINFC610_ADDRH                               0x14
#define HINFC610_DATA_NUM                            0x18

#define HINFC610_OP                                  0x1C
#define HINFC610_OP_READ_STATUS_EN       (1U << 0)
#define HINFC610_OP_READ_DATA_EN         (1U << 1)
#define HINFC610_OP_WAIT_READY_EN        (1U << 2)
#define HINFC610_OP_CMD2_EN              (1U << 3)
#define HINFC610_OP_WRITE_DATA_EN        (1U << 4)
#define HINFC610_OP_ADDR_EN              (1U << 5)
#define HINFC610_OP_CMD1_EN              (1U << 6)
#define HINFC610_OP_NF_CS_SHIFT          (7)
#define HINFC610_OP_NF_CS_MASK           (3)
#define HINFC610_OP_ADDR_CYCLE_SHIFT     (9)
#define HINFC610_OP_ADDR_CYCLE_MASK      (7)
#define HINFC610_OP_READID_EN            (1U << 12)
#define HINFC610_OP_RW_REG_EN            (1U << 13)

#define HINFC610_STATUS                               0x20
#define HINFC610_STATUS_RANDOMIZER_EN    (1U << 20)

#define HINFC610_INTS                                 0x28
#define HINFC610_INTS_UE                 (1U << 6)
#define HINFC610_INTCLR                               0x2C
#define HINFC610_INTCLR_UE               (1U << 6)
#define HINFC610_INTCLR_CE               (1U << 5)

#define HINFC610_DMA_CTRL                             0x60
#define HINFC610_DMA_CTRL_DMA_START      (1U << 0)
#define HINFC610_DMA_CTRL_WE             (1U << 1)
#define HINFC610_DMA_CTRL_BURST4_EN      (1U << 4)
#define HINFC610_DMA_CTRL_BURST8_EN      (1U << 5)
#define HINFC610_DMA_CTRL_BURST16_EN     (1U << 6)
#define HINFC610_DMA_CTRL_ADDR_NUM_SHIFT (7)
#define HINFC610_DMA_CTRL_ADDR_NUM_MASK  (1)
#define HINFC610_DMA_CTRL_CS_SHIFT       (8)
#define HINFC610_DMA_CTRL_CS_MASK        (0x03)

#define HINFC610_DMA_ADDR_DATA                        0x64
#define HINFC610_DMA_ADDR_OOB                         0x68
#define HINFC610_DMA_ADDR_DATA1                       0xB4
#define HINFC610_DMA_ADDR_DATA2                       0xB8
#define HINFC610_DMA_ADDR_DATA3                       0xBC
#define HINFC610_DMA_ADDR_DATA4                       0xEC
#define HINFC610_DMA_ADDR_DATA5                       0xF0
#define HINFC610_DMA_ADDR_DATA6                       0xF4
#define HINFC610_DMA_ADDR_DATA7                       0xF8

#define HINFC610_DMA_LEN                              0x6C
#define HINFC610_DMA_LEN_OOB_SHIFT       (16)
#define HINFC610_DMA_LEN_OOB_MASK        (0x1FFF)

#define HINFC610_DMA_PARA                             0x70
#define HINFC610_DMA_PARA_DATA_RW_EN     (1U << 0)
#define HINFC610_DMA_PARA_OOB_RW_EN      (1U << 1)
#define HINFC610_DMA_PARA_DATA_EDC_EN    (1U << 2)
#define HINFC610_DMA_PARA_OOB_EDC_EN     (1U << 3)
#define HINFC610_DMA_PARA_EXT_LEN_SHIFT  (6)
#define HINFC610_DMA_PARA_EXT_LEN_MASK   (0x03)

#define HINFC610_VERSION                              0x74
#define HINFC610_LOG_READ_ADDR                        0x7C
#define HINFC610_LOG_READ_LEN                         0x80

#define HINFC610_LP_CTRL                              0X9C
#define HINFC610_LP_CTRL_EN              (1U << 0)

#define HINFC610_ECC_REG0                             0xA0
#define HINFC610_ECC_REG1                             0xA4
#define HINFC610_ECC_REG2                             0xA8
#define HINFC610_ECC_REG3                             0xAC
#define HINFC610_ECC_REG4                             0xDC
#define HINFC610_ECC_REG5                             0xE0
#define HINFC610_ECC_REG6                             0xE4
#define HINFC610_ECC_REG7                             0xE8

#define HINFC610_RANDOMIZER                           0xC0
#define HINFC610_RANDOMIZER_PAD           0x02
#define HINFC610_RANDOMIZER_ENABLE        0x01
/* read nand id or nand status, return from nand data length */
#define HINFC610_NANDINFO_LEN             0x10

#define HINFC610_BOOT_CFG                             0xC4
#define HINFC610_BOOT_CFG_RANDOMIZER_PAD  0x01
#define HINFC610_BOOT_CFG_SAVE_PIN_MODE_SHIFT    13
#define HINFC610_BOOT_CFG_SAVE_PIN_MODE   (1U << HINFC610_BOOT_CFG_SAVE_PIN_MODE_SHIFT)
#define HINFC610_BOOT_CFG_SYC_NAND_PAD_SHIFT     12
#define HINFC610_BOOT_CFG_SYC_NAND_PAD    (1U << HINFC610_BOOT_CFG_SYC_NAND_PAD_SHIFT)

#define HINFC610_SYNC_TIMING                          0xD0

/* ONFI: sync nand timing config */
#define HINFC610_SYNC_ONFI_T_CAD          (0xF << 24)
#define HINFC610_SYNC_ONFI_T_DQZ          (0xF << 20)

/* TOGGLE: sync nand timing config */
#define HINFC610_SYNC_TOGGLE_PRE_RDQS     (0xF << 16)
#define HINFC610_SYNC_TOGGLE_POST_RDQS    (0xF << 12)
#define HINFC610_SYNC_TOGGLE_PRE_WDQS     (0xF << 8)
#define HINFC610_SYNC_TOGGLE_POST_WDQS    (0xF << 4)
#define HINFC610_SYNC_TOGGLE_RW_PSTH      (0xF << 0)

/* DMA address align with 32 bytes. */
#define HINFC610_DMA_ALIGN                            64
/*****************************************************************************/
#include "hinfc_gen.h"

#undef  READ
#define READ           1

#undef  WRITE
#define WRITE          0

#undef  FALSE
#define FALSE          0

#undef  TRUE
#define TRUE           1

#undef ENABLE
#define ENABLE         1

#undef DISABLE
#define DISABLE        0
/*****************************************************************************/

struct hinfc_host
{
	struct nand_chip *chip;
	struct mtd_info  *mtd;
	void __iomem *iobase;
	struct device *dev;

	unsigned int offset;
	unsigned int command;

	int chipselect;

	unsigned int n24bit_ext_len;
	int ecctype;

/* Current system has already gone to sync mode */
#define HINFC610_IS_SYNC(_host) ((_host)->NFC_CON & HINFC610_CON_NF_MODE_MASK)
	unsigned long NFC_CON;
	unsigned long NFC_CON_ECC_NONE;

	unsigned int addr_cycle;
	unsigned int addr_value[2];
	unsigned int column;

	unsigned int block_page_mask;
	unsigned int page_per_chip;
	unsigned int page_per_block;
	unsigned int pagesize;
	unsigned int oobsize;

	char *fix_pagebuf;
	char *fix_oobbuf;

	char *pagebuf;
	char *oobbuf;

	int  need_rr_data;
#define HINFC_READ_RETRY_DATA_LEN         128
	char rr_data[HINFC_READ_RETRY_DATA_LEN];
	int  version;

	struct {
		/* when ecc number more than this value, return bitflip to mtd. */
		unsigned int threshold;
		int max_bitsflap;
		int section; /* the ecc correct unit */
		void (*dump)(void *host, unsigned char *ecc,
			     int *max_bitsflap);
	} ecc;

	/* BOOTROM read two bytes to detect the bad block flag */
	unsigned char *bbm;  /* nand bad block mark */
	unsigned short *epm; /* nand empty page mark */
	unsigned int flags;

#define HINFC610_PS_UC_ECC        0x01 /* page has ecc error */
#define HINFC610_PS_BAD_BLOCK     0x02 /* bad block */
#define HINFC610_PS_EMPTY_PAGE    0x04 /* page is empty */
#define HINFC610_PS_EPM_ERROR     0x0100 /* empty page mark word has ecc error. */
#define HINFC610_PS_BBM_ERROR     0x0200 /* bad block mark word has ecc error. */
	unsigned int page_status;

	struct clk *clk;

	int (*send_cmd_pageprog)(struct hinfc_host *host);
	int (*send_cmd_status)(struct hinfc_host *host);
	int (*send_cmd_readstart)(struct hinfc_host *host);
	int (*send_cmd_erase)(struct hinfc_host *host);
	int (*send_cmd_readid)(struct hinfc_host *host);
	int (*send_cmd_reset)(struct hinfc_host *host, int chipselect);
	int (*enable)(struct hinfc_host *host, int enable);

	int (*enable_ecc_randomizer)(struct hinfc_host *host,
				     int ecc_en, int randomizer_en);

	struct read_retry_t *read_retry;

	struct nand_sync *sync;

	void *cache;

	struct dentry *dbgfs_root;
};

#define HINFC610_UC_ECC               0x01
#define HINFC610_BAD_BLOCK            0x02
#define HINFC610_EMPTY_PAGE           0x04

#define IS_PS_EMPTY_PAGE(_host)  ((_host)->page_status & HINFC610_PS_EMPTY_PAGE)
#define IS_PS_BAD_BLOCK(_host)   ((_host)->page_status & HINFC610_PS_BAD_BLOCK)
#define IS_PS_UN_ECC(_host)      ((_host)->page_status & HINFC610_PS_UC_ECC)
#define IS_PS_EPM_ERR(_host)     ((_host)->page_status & HINFC610_PS_EPM_ERROR)
#define IS_PS_BBM_ERR(_host)     ((_host)->page_status & HINFC610_PS_BBM_ERROR)

/*****************************************************************************/

#define HINFC610_READ_1CMD_0ADD_NODATA(_host) \
	(HINFC610_OP_CMD1_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

#define HINFC610_READ_1CMD_1ADD_DATA(_host)    \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_READ_DATA_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_READ_1CMD_1ADD_DATA_WAIT_READY(_host)    \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_READ_DATA_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_READ_1CMD_1ADD_DATA_SYNC(_host) \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_READ_DATA_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| HINFC610_OP_RW_REG_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_READ_2CMD_5ADD(_host)    \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_CMD2_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_READ_DATA_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (5 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_0CMD_1ADD_DATA(_host) \
	(HINFC610_OP_ADDR_EN \
	| HINFC610_OP_WRITE_DATA_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_0CMD_1ADD_DATA_WAIT_READY(_host) \
	(HINFC610_OP_ADDR_EN \
	| HINFC610_OP_WRITE_DATA_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_0CMD_1ADD_DATA_SYNC(_host) \
	(HINFC610_OP_ADDR_EN \
	 | HINFC610_OP_WRITE_DATA_EN \
	 | HINFC610_OP_RW_REG_EN \
	 | ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_0CMD_1ADD_DATA_SYNC_WAIT_READY(_host) \
	(HINFC610_OP_ADDR_EN \
	 | HINFC610_OP_WRITE_DATA_EN \
	 | HINFC610_OP_RW_REG_EN \
	 | HINFC610_OP_WAIT_READY_EN \
	 | ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_1ADD_DATA(_host)  \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_WRITE_DATA_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_1ADD_DATA_WAIT_READY(_host)  \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_WRITE_DATA_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_1ADD_DATA_SYNC(_host)  \
	(HINFC610_OP_CMD1_EN \
	 | HINFC610_OP_ADDR_EN \
	 | HINFC610_OP_WRITE_DATA_EN \
	 | HINFC610_OP_RW_REG_EN \
	 | ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_1ADD_DATA_SYNC_WAIT_READY(_host)  \
	(HINFC610_OP_CMD1_EN \
	 | HINFC610_OP_ADDR_EN \
	 | HINFC610_OP_WRITE_DATA_EN \
	 | HINFC610_OP_WAIT_READY_EN \
	 | HINFC610_OP_RW_REG_EN \
	 | ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	 | (1 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_2ADD_DATA(_host)  \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_ADDR_EN \
	| HINFC610_OP_WRITE_DATA_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	| (2 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_1CMD_2ADD_DATA_SYNC(_host)  \
	(HINFC610_OP_CMD1_EN \
	 | HINFC610_OP_ADDR_EN \
	 | HINFC610_OP_WRITE_DATA_EN \
	 | HINFC610_OP_RW_REG_EN \
	 | ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT) \
	 | (2 << HINFC610_OP_ADDR_CYCLE_SHIFT))

#define HINFC610_WRITE_2CMD_0ADD_NODATA(_host) \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_CMD2_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

#define HINFC610_WRITE_2CMD_0ADD_NODATA_SYNC(_host) \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_CMD2_EN \
	| HINFC610_OP_RW_REG_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

#define HINFC610_WRITE_1CMD_0ADD_NODATA(_host) \
	(HINFC610_OP_CMD1_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

#define HINFC610_WRITE_1CMD_0ADD_NODATA_SYNC(_host) \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_RW_REG_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

#define HINFC610_WRITE_1CMD_0ADD_NODATA_WAIT_READY(_host) \
	(HINFC610_OP_CMD1_EN \
	| HINFC610_OP_WAIT_READY_EN \
	| ((_host->chipselect & HINFC610_OP_NF_CS_MASK) << HINFC610_OP_NF_CS_SHIFT))

/*****************************************************************************/

#define WAIT_CONTROLLER_FINISH(_host) \
do { \
	unsigned int timeout = 0xF0000000; \
	while ((hinfc_read(_host, HINFC610_STATUS) & 0x1) == 0x0 && timeout) \
		timeout--; \
	if (!timeout) \
		hinfc_pr_bug("Wait NAND controller finish timeout.\n"); \
} while (0)

/*****************************************************************************/

#define hinfc_read(_host, _reg) \
	readl((char *)_host->iobase + (_reg))

#define hinfc_write(_host, _value, _reg) \
	writel((_value), (char *)_host->iobase + (_reg))

#define HINFC_CMD_SEQ(_cmd0, _cmd1)        \
	(((_cmd0) & 0xFF) | ((_cmd1) & 0xFF) << 8)
/*****************************************************************************/

#define GET_RW_PAGE_INDEX(host) \
	(((host->addr_value[0] >> 16) | (host->addr_value[1] << 16)) + \
		host->page_per_chip * host->chipselect)

#define GET_ERASE_PAGE_INDEX(host) \
	(host->addr_value[0])

/*****************************************************************************/

void hinfc610_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl);
int hinfc610_dev_ready(struct mtd_info *mtd);
void hinfc610_select_chip(struct mtd_info *mtd, int chipselect);
uint8_t hinfc610_read_byte(struct mtd_info *mtd);
u16 hinfc610_read_word(struct mtd_info *mtd);
void hinfc610_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len);
void hinfc610_read_buf(struct mtd_info *mtd, uint8_t *buf, int len);
int hinfc610_nand_init(struct hinfc_host *host, struct nand_chip *chip);
int hinfc610_nand_destory(struct hinfc_host *host);
/******************************************************************************/
#endif /* HINFCV610H */
