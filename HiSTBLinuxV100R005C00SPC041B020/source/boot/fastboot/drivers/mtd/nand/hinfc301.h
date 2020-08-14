/******************************************************************************
*    NAND Flash Controller V301 Device Driver
*    Copyright (c) 2009-2010 by Hisilicon.
*    All rights reserved.
* ***
*    written by Czyong  2010-11-04
*    
******************************************************************************/

#ifndef HINFCV301H
#define HINFCV301H
/******************************************************************************/

#ifndef CONFIG_HINFC301_REG_BASE_ADDRESS
#  define CONFIG_HINFC301_REG_BASE_ADDRESS            (0x10000000)
#  warning NOT config CONFIG_HINFC301_REG_BASE_ADDRESS, used default value, maybe invalid.
#endif /* CONFIG_HINFC301_REG_BASE_ADDRESS */

#ifndef CONFIG_HINFC301_BUFFER_BASE_ADDRESS
#  define CONFIG_HINFC301_BUFFER_BASE_ADDRESS         (0x50000000)
#  warning NOT config CONFIG_HINFC301_BUFFER_BASE_ADDRESS, used default value, maybe invalid.
#endif /* CONFIG_HINFC301_BUFFER_BASE_ADDRESS */

#ifndef CONFIG_HINFC301_W_LATCH
#  define CONFIG_HINFC301_W_LATCH                     (5)
#endif /* CONFIG_HINFC301_W_LATCH */

#ifndef CONFIG_HINFC301_R_LATCH
#  define CONFIG_HINFC301_R_LATCH                     (7)
#endif /* CONFIG_HINFC301_R_LATCH */

#ifndef CONFIG_HINFC301_RW_LATCH
#  define CONFIG_HINFC301_RW_LATCH                    (3)
#endif /* CONFIG_HINFC301_RW_LATCH */

#ifndef CONFIG_HINFC301_MAX_CHIP
#  define CONFIG_HINFC301_MAX_CHIP                    (1)
#  warning NOT config CONFIG_HINFC301_MAX_CHIP, used default value, maybe invalid.
#endif /* CONFIG_HINFC301_MAX_CHIP */

/*****************************************************************************/
#define HINFC301_REG_BASE_ADDRESS_LEN                 (0x100)
#define HINFC301_BUFFER_BASE_ADDRESS_LEN              (2048 + 128)

#define HINFC301_CHIP_DELAY                           (25)

#define HINFC301_ADDR_CYCLE_MASK                      0x4

/*****************************************************************************/
#define HINFC301_CON                                 0x00
#define HINFC301_CON_OP_MODE_NORMAL      (1U << 0)
#define HINFC301_CON_PAGEISZE_SHIFT      (1)
#define HINFC301_CON_PAGESIZE_MASK       (0x03)
#define HINFC301_CON_BUS_WIDTH           (1U << 3)
#define HINFC301_CON_READY_BUSY_SEL      (1U << 7)
#define HINFC301_CON_ECCTYPE_SHIFT       (8)
#define HINFC301_CON_ECCTYPE_MASK        (0x07)

#define HINFC301_PWIDTH                              0x04
#define SET_HINFC301_PWIDTH(_w_lcnt, _r_lcnt, _rw_hcnt) \
	((_w_lcnt) | (((_r_lcnt) & 0x0F) << 4) | (((_rw_hcnt) & 0x0F) << 8))

#define HINFC301_CMD                                 0x0C
#define HINFC301_ADDRL                               0x10
#define HINFC301_ADDRH                               0x14

#define HINFC301_OP                                  0x1C
#define HINFC301_OP_READ_STATUS_EN       (1U << 0)
#define HINFC301_OP_READ_DATA_EN         (1U << 1)
#define HINFC301_OP_WAIT_READY_EN        (1U << 2)
#define HINFC301_OP_CMD2_EN              (1U << 3)
#define HINFC301_OP_WRITE_DATA_EN        (1U << 4)
#define HINFC301_OP_ADDR_EN              (1U << 5)
#define HINFC301_OP_CMD1_EN              (1U << 6)
#define HINFC301_OP_NF_CS_SHIFT          (7)
#define HINFC301_OP_NF_CS_MASK           (3)
#define HINFC301_OP_ADDR_CYCLE_SHIFT     (9)
#define HINFC301_OP_ADDR_CYCLE_MASK      (7)

#define HINFC301_STATUS                               0x20

#define HINFC301_INTS                                 0x28
#define HINFC301_INTS_UE                 (1U << 6)
#define HINFC301_INTCLR                               0x2C
#define HINFC301_INTCLR_UE               (1U << 6)

#define HINFC301_DMA_CTRL                             0x60
#define HINFC301_DMA_CTRL_DMA_START      (1U << 0)
#define HINFC301_DMA_CTRL_WE             (1U << 1)
#define HINFC301_DMA_CTRL_DATA_AREA_EN   (1U << 2)
#define HINFC301_DMA_CTRL_OOB_AREA_EN    (1U << 3)
#define HINFC301_DMA_CTRL_BURST4_EN      (1U << 4)
#define HINFC301_DMA_CTRL_BURST8_EN      (1U << 5)
#define HINFC301_DMA_CTRL_BURST16_EN     (1U << 6)
#define HINFC301_DMA_CTRL_ADDR_NUM_SHIFT (7)
#define HINFC301_DMA_CTRL_ADDR_NUM_MASK  (1)
#define HINFC301_DMA_CTRL_CS_SHIFT       (8)
#define HINFC301_DMA_CTRL_CS_MASK        (0x03)

#define HINFC301_DMA_ADDR_DATA                        0x64
#define HINFC301_DMA_ADDR_OOB                         0x68

#define HINFC301_DMA_LEN                              0x6C
#define HINFC301_DMA_LEN_OOB_SHIFT       (16)
#define HINFC301_DMA_LEN_OOB_MASK        (0xFFF)

#define HINFC301_DMA_PARA                             0x70
#define HINFC301_DMA_PARA_DATA_RW_EN     (1U << 0)
#define HINFC301_DMA_PARA_OOB_RW_EN      (1U << 1)
#define HINFC301_DMA_PARA_DATA_EDC_EN    (1U << 2)
#define HINFC301_DMA_PARA_OOB_EDC_EN     (1U << 3)
#define HINFC301_DMA_PARA_DATA_ECC_EN    (1U << 4)
#define HINFC301_DMA_PARA_OOB_ECC_EN     (1U << 5)
#define HINFC301_DMA_PARA_EXT_LEN_SHIFT  (6)
#define HINFC301_DMA_PARA_EXT_LEN_MASK   (0x03)

#define HINFC301_VERSION                              0x74
#define HINFC301_LOG_READ_ADDR                        0x7C
#define HINFC301_LOG_READ_LEN                         0x80

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

	unsigned int offset;
	unsigned int command;

	int chipselect;

	unsigned int n24bit_ext_len;
	int ecctype;

	unsigned long NFC_CON;
	unsigned long NFC_CON_ECC_NONE;

	unsigned int addr_cycle;
	unsigned int addr_value[2];
	unsigned int column;

	unsigned int dma_oob;
	unsigned int dma_buffer;
	unsigned int pagesize;
	unsigned int oobsize;
	char *buffer;

	unsigned int uc_er;
};
/*****************************************************************************/

#define hinfc_read(_host, _reg) \
	readl((char *)_host->iobase + (_reg))

#define hinfc_write(_host, _value, _reg) \
	writel((_value), (char *)_host->iobase + (_reg))

/*****************************************************************************/

#undef	DBG_BUG
#define DBG_BUG(fmt, args...) do{\
	printk("%s(%d): !!! BUG " fmt, __FILE__, __LINE__, ##args); \
	while(1); \
} while (0)

/*****************************************************************************/

#define GET_PAGE_INDEX(host) \
	((host->addr_value[0] >> 16) | (host->addr_value[1] << 16))

#if !(defined(CONFIG_HINFC301_DBG_NAND_PROC_FILE) || defined(CONFIG_HINFC301_DBG_NAND_EC_NOTICE))
#  define dbg_nand_ec_notice(_p0)
#  define dbg_nand_ec_init()
#endif /* !(defined(CONFIG_HINFC301_DBG_NAND_PROC_FILE) || defined(CONFIG_HINFC301_DBG_NAND_EC_NOTICE)) */

#if !defined(CONFIG_HINFC301_DBG_NAND_PROC_FILE)
#  define dbg_nand_proc_save_logs(_p0, _p1)
#  define dbg_nand_proc_init()
#endif /* !defined(CONFIG_HINFC301_DBG_NAND_PROC_FILE) */

#if !defined(CONFIG_HINFC301_DBG_NAND_STAT_PROC_FILE)
#  define dbg_nand_stat_proc_init(_p0, _p1)
#  define dbg_nand_stat_operation(_p0, _p1)
#endif /* CONFIG_HINFC301_DBG_NAND_STAT_PROC_FILE */
/******************************************************************************/
#endif /* HINFCV301H */
