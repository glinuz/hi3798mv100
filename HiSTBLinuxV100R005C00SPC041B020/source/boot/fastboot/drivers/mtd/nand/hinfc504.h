/******************************************************************************
 *    NAND Flash Controller V504 Device Driver
 *    Copyright (c) 2009-2010 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Czyong.
 *
******************************************************************************/

#ifndef HINFCV504H
#define HINFCV504H
/******************************************************************************/

#ifndef CONFIG_HINFC504_REG_BASE_ADDRESS
#  define CONFIG_HINFC504_REG_BASE_ADDRESS            (0x60010000)
#  warning NOT config CONFIG_HINFC504_REG_BASE_ADDRESS, used default value, maybe invalid.
#endif /* CONFIG_HINFC504_REG_BASE_ADDRESS */

#ifndef CONFIG_HINFC504_BUFFER_BASE_ADDRESS
#  define CONFIG_HINFC504_BUFFER_BASE_ADDRESS         (0x24000000)
#  warning NOT config CONFIG_HINFC504_BUFFER_BASE_ADDRESS, used default value, maybe invalid.
#endif /* CONFIG_HINFC504_BUFFER_BASE_ADDRESS */

#ifndef CONFIG_HINFC504_W_LATCH
#  define CONFIG_HINFC504_W_LATCH                     (5)
#endif /* CONFIG_HINFC504_W_LATCH */

#ifndef CONFIG_HINFC504_R_LATCH
#  define CONFIG_HINFC504_R_LATCH                     (7)
#endif /* CONFIG_HINFC504_R_LATCH */

#ifndef CONFIG_HINFC504_RW_LATCH
#  define CONFIG_HINFC504_RW_LATCH                    (3)
#endif /* CONFIG_HINFC504_RW_LATCH */

#ifndef CONFIG_HINFC504_MAX_CHIP
#  define CONFIG_HINFC504_MAX_CHIP                    (1)
#  warning NOT config CONFIG_HINFC504_MAX_CHIP, used default value, maybe invalid.
#endif /* CONFIG_HINFC504_MAX_CHIP */

/*****************************************************************************/
#define HINFC504_REG_BASE_ADDRESS_LEN                 (0x100)
#define HINFC504_BUFFER_BASE_ADDRESS_LEN              (2048 + 128)

#define HINFC504_CHIP_DELAY                           (25)

#define HINFC504_ADDR_CYCLE_MASK                      0x4
#define HINFC504_DMA_ADDR_OFFSET                      4096
/*****************************************************************************/
#define HINFC504_CON                                 0x00
#define HINFC600_CON                                 0x00
#define HINFC504_CON_OP_MODE_NORMAL      (1U << 0)
#define HINFC504_CON_PAGEISZE_SHIFT      (1)
#define HINFC504_CON_PAGESIZE_MASK       (0x07)
#define HINFC504_CON_BUS_WIDTH           (1U << 4)
#define HINFC504_CON_READY_BUSY_SEL      (1U << 8)
#define HINFC504_CON_ECCTYPE_SHIFT       (9)
#define HINFC504_CON_ECCTYPE_MASK        (0x07)
#define HINFC600_CON_RANDOMIZER_EN       (1<<13)

#define HINFC504_PWIDTH                              0x04
#define SET_HINFC504_PWIDTH(_w_lcnt, _r_lcnt, _rw_hcnt) \
	((_w_lcnt) | (((_r_lcnt) & 0x0F) << 4) | (((_rw_hcnt) & 0x0F) << 8))

#define HINFC504_CMD                                 0x0C
#define HINFC504_ADDRL                               0x10
#define HINFC504_ADDRH                               0x14
#define HINFC504_DATA_NUM                            0x18

#define HINFC504_OP                                  0x1C
#define HINFC504_OP_READ_STATUS_EN       (1U << 0)
#define HINFC504_OP_READ_DATA_EN         (1U << 1)
#define HINFC504_OP_WAIT_READY_EN        (1U << 2)
#define HINFC504_OP_CMD2_EN              (1U << 3)
#define HINFC504_OP_WRITE_DATA_EN        (1U << 4)
#define HINFC504_OP_ADDR_EN              (1U << 5)
#define HINFC504_OP_CMD1_EN              (1U << 6)
#define HINFC504_OP_NF_CS_SHIFT          (7)
#define HINFC504_OP_NF_CS_MASK           (3)
#define HINFC504_OP_ADDR_CYCLE_SHIFT     (9)
#define HINFC504_OP_ADDR_CYCLE_MASK      (7)

#define HINFC504_STATUS                               0x20

#define HINFC504_INTS                                 0x28
#define HINFC504_INTS_UE                 (1U << 6)
#define HINFC504_INTCLR                               0x2C
#define HINFC504_INTCLR_UE               (1U << 6)
#define HINFC504_INTCLR_CE               (1U << 5)

#define HINFC504_DMA_CTRL                             0x60
#define HINFC504_DMA_CTRL_DMA_START      (1U << 0)
#define HINFC504_DMA_CTRL_WE             (1U << 1)
#define HINFC504_DMA_CTRL_DATA_AREA_EN   (1U << 2)
#define HINFC504_DMA_CTRL_OOB_AREA_EN    (1U << 3)
#define HINFC504_DMA_CTRL_BURST4_EN      (1U << 4)
#define HINFC504_DMA_CTRL_BURST8_EN      (1U << 5)
#define HINFC504_DMA_CTRL_BURST16_EN     (1U << 6)
#define HINFC504_DMA_CTRL_ADDR_NUM_SHIFT (7)
#define HINFC504_DMA_CTRL_ADDR_NUM_MASK  (1)
#define HINFC504_DMA_CTRL_CS_SHIFT       (8)
#define HINFC504_DMA_CTRL_CS_MASK        (0x03)

#define HINFC504_DMA_ADDR_DATA                        0x64
#define HINFC504_DMA_ADDR_OOB                         0x68
#define HINFC504_DMA_ADDR_DATA1                       0xB4
#define HINFC504_DMA_ADDR_DATA2                       0xB8
#define HINFC504_DMA_ADDR_DATA3                       0xBC

#define HINFC504_DMA_LEN                              0x6C
#define HINFC504_DMA_LEN_OOB_SHIFT       (16)
#define HINFC504_DMA_LEN_OOB_MASK        (0xFFF)

#define HINFC504_DMA_PARA                             0x70
#define HINFC504_DMA_PARA_DATA_RW_EN     (1U << 0)
#define HINFC504_DMA_PARA_OOB_RW_EN      (1U << 1)
#define HINFC504_DMA_PARA_DATA_EDC_EN    (1U << 2)
#define HINFC504_DMA_PARA_OOB_EDC_EN     (1U << 3)
#define HINFC504_DMA_PARA_DATA_ECC_EN    (1U << 4)
#define HINFC504_DMA_PARA_OOB_ECC_EN     (1U << 5)
#define HINFC504_DMA_PARA_EXT_LEN_SHIFT  (6)
#define HINFC504_DMA_PARA_EXT_LEN_MASK   (0x03)

#define HINFC504_VERSION                              0x74
#define HINFC504_LOG_READ_ADDR                        0x7C
#define HINFC504_LOG_READ_LEN                         0x80

#define HINFC504_ECC_REG0                             0xA0
#define HINFC504_ECC_REG1                             0xA4
#define HINFC504_ECC_REG2                             0xA8
#define HINFC504_ECC_REG3                             0xAC

#define HINFC504_RANDOMIZER                           0xC0
#define HINFC504_RANDOMIZER_PAD           0x02
#define HINFC504_RANDOMIZER_ENABLE        0x01
/* read nand id or nand status, return from nand data length */
#define HINFC504_NANDINFO_LEN             0x10

#define HINFC600_BOOT_CFG                             0xC4
#define HINFC600_BOOT_CFG_RANDOMIZER_PAD  0x01

/* DMA address align with 32 bytes. */
#define HINFC504_DMA_ALIGN                            64
/*****************************************************************************/

#include <hinfc_gen.h>

/*****************************************************************************/

struct hinfc_host
{
	struct nand_chip *chip;
	struct mtd_info  *mtd;
	void __iomem *iobase;
	void __iomem *sysreg;
	struct device *dev;

	int flags; /* reference nand_dev_t->flags */
	unsigned int offset;
	unsigned int command;

	int chipselect;

	unsigned int n24bit_ext_len;
	int ecctype;

	unsigned long NFC_CON;
	unsigned long NFC_CON_ECC_NONE;

	unsigned int addr_cycle;
	unsigned int addr_value[2];
	unsigned int cache_addr_value[2];
	unsigned int column;
	unsigned int block_page_mask;

	unsigned int dma_oob;
	unsigned int dma_buffer;
	unsigned int pagesize;
	unsigned int oobsize;
	/* This is maybe an un-aligment address, only for malloc or free */
	char *buforg;
	char *buffer;

	int  need_rr_data;
#define HINFC_READ_RETRY_DATA_LEN         128
	char rr_data[HINFC_READ_RETRY_DATA_LEN];
	int  version;

	/* BOOTROM read two bytes to detect the bad block flag */
#define HINFC_BAD_BLOCK_POS              0
	unsigned char *bbm;       /* nand bad block mark */
	unsigned short *epm;      /* nand empty page mark */
	unsigned short epmvalue;  /* nand empty page flag value */

	unsigned int uc_er;

	int (*send_cmd_pageprog)(struct hinfc_host *host);
	int (*send_cmd_status)(struct hinfc_host *host);
	int (*send_cmd_readstart)(struct hinfc_host *host);
	int (*send_cmd_erase)(struct hinfc_host *host);
	int (*send_cmd_readid)(struct hinfc_host *host);
	int (*send_cmd_reset)(struct hinfc_host *host, int chipselect);

	int (*enable_ecc_randomizer)(struct hinfc_host *host,
				     int ecc_en, int randomizer_en);

	void (*detect_ecc)(struct hinfc_host *host);

	struct read_retry_t *read_retry;
};
/*****************************************************************************/
#define HINFC504_READ_1CMD_0ADD_NODATA \
	(HINFC504_OP_CMD1_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT))

#define HINFC504_READ_1CMD_1ADD_DATA    \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_READ_DATA_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_READ_1CMD_1ADD_DATA_WAIT_READY    \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_READ_DATA_EN \
	| HINFC504_OP_WAIT_READY_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_READ_2CMD_5ADD    \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_CMD2_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_READ_DATA_EN \
	| HINFC504_OP_WAIT_READY_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (5 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_0CMD_1ADD_DATA \
	(HINFC504_OP_ADDR_EN \
	| HINFC504_OP_WRITE_DATA_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_0CMD_1ADD_DATA_WAIT_READY \
	(HINFC504_OP_ADDR_EN \
	| HINFC504_OP_WRITE_DATA_EN \
	| HINFC504_OP_WAIT_READY_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_1CMD_1ADD_DATA  \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_WRITE_DATA_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_1CMD_1ADD_DATA_WAIT_READY  \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_WRITE_DATA_EN \
	| HINFC504_OP_WAIT_READY_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (1 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_1CMD_2ADD_DATA  \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_ADDR_EN \
	| HINFC504_OP_WRITE_DATA_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT) \
	| (2 << HINFC504_OP_ADDR_CYCLE_SHIFT))

#define HINFC504_WRITE_2CMD_0ADD_NODATA \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_CMD2_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT))

#define HINFC504_WRITE_1CMD_0ADD_NODATA \
	(HINFC504_OP_CMD1_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT))

#define HINFC504_WRITE_1CMD_0ADD_NODATA_WAIT_READY \
	(HINFC504_OP_CMD1_EN \
	| HINFC504_OP_WAIT_READY_EN \
	| ((host->chipselect & HINFC504_OP_NF_CS_MASK) << HINFC504_OP_NF_CS_SHIFT))

/*****************************************************************************/

#define WAIT_CONTROLLER_FINISH() \
do { \
	unsigned int timeout = 0xF0000000; \
	while ((hinfc_read(host, HINFC504_STATUS) & 0x1) == 0x0 && timeout) \
		timeout--; \
	if (!timeout) \
		DBG_BUG("Wait NAND controller finish timeout.\n"); \
} while (0)

#define DBG_OUT(fmt, args...) do{\
	printk("%s(%d): " fmt, __FILE__, __LINE__, ##args); \
} while (0)
/*****************************************************************************/

#define hinfc_read(_host, _reg) \
	readl((char *)_host->iobase + (_reg))

#define hinfc_write(_host, _value, _reg) \
	writel((_value), (char *)_host->iobase + (_reg))

#define HINFC_CMD_SEQ(_cmd0, _cmd1)        \
	(((_cmd0) & 0xFF) | ((_cmd1) & 0xFF) << 8)
/*****************************************************************************/

#undef  DBG_BUG
#define DBG_BUG(fmt, args...) do{\
	printk("%s(%d): !!! BUG " fmt, __FILE__, __LINE__, ##args); \
	while(1); \
} while (0)

/*****************************************************************************/

#define GET_PAGE_INDEX(host) \
	((host->addr_value[0] >> 16) | (host->addr_value[1] << 16))

#if !(defined(CONFIG_HINFC504_DBG_NAND_PROC_FILE) \
	|| defined(CONFIG_HINFC504_DBG_NAND_EC_NOTICE))
#  define dbg_nand_ec_notice(_p0)
#  define dbg_nand_ec_init()
#endif /* !(defined(CONFIG_HINFC504_DBG_NAND_PROC_FILE) \
	|| defined(CONFIG_HINFC504_DBG_NAND_EC_NOTICE)) */

#if !defined(CONFIG_HINFC504_DBG_NAND_PROC_FILE)
#  define dbg_nand_proc_save_logs(_p0, _p1)
#  define dbg_nand_proc_init()
#endif /* !defined(CONFIG_HINFC504_DBG_NAND_PROC_FILE) */

#if !defined(CONFIG_HINFC504_DBG_NAND_PE_PROC_FILE)
#  define dbg_nand_pe_erase(_p0)
#  define dbg_nand_pe_proc_init(_p0, _p1, _p2)
#endif /* !defined(CONFIG_HINFC504_DBG_NAND_PE_PROC_FILE) */

/******************************************************************************/
#endif /* HINFCV504H */
