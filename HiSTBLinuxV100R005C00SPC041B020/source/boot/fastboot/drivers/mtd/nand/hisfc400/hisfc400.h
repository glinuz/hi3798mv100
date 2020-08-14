/******************************************************************************
 *    SPI Nand Flash Controller V400 Device Driver
 *    Copyright (c) 2014 by Hisilicon.
 *    All rights reserved.
 * ***
 *    Create By Hisilicon
 *
******************************************************************************/

#ifndef __HISFCV400_H__
#define __HISFCV400_H__

#include <linux/compiler.h>
#include <linux/mtd/nand.h>
#include <asm/io.h>
#include <linux/stddef.h>
#include <common.h>

#include "../hisfc_spl_ids.h"

/******************************************************************************/
#ifndef CONFIG_HISFC400_REG_BASE_ADDRESS
#  define CONFIG_HISFC400_REG_BASE_ADDRESS		(0x60040000)
#endif /* CONFIG_HISFC400_REG_BASE_ADDRESS */

#define HISFC400_REG_BASE_ADDRESS_LEN		0x100

#ifndef CONFIG_HISFC400_BUFFER_BASE_ADDRESS
#  define CONFIG_HISFC400_BUFFER_BASE_ADDRESS		(0x2A000000)
#endif /* CONFIG_HISFC400_BUFFER_BASE_ADDRESS */

#define HISFC400_BUFFER_BASE_ADDRESS_LEN		(2048 + 256)

#ifndef CONFIG_HISFC400_PERIPHERY_REGBASE
#  define CONFIG_HISFC400_PERIPHERY_REGBASE		(0x101F5000)
#endif /* CONFIG_HISFC400_PERIPHERY_REGBASE*/

#ifndef CONFIG_HISFC400_MAX_CHIP
#  define CONFIG_HISFC400_MAX_CHIP			(1)
#endif /* CONFIG_HISFC400_MAX_CHIP */

/*****************************************************************************/
/* These macroes are for debug only, reg read is slower then dma read */
#undef HISFCV400_SUPPORT_REG_READ
/* #define HISFCV400_SUPPORT_REG_READ */
#undef HISFCV400_SUPPORT_REG_WRITE
/* #define HISFCV400_SUPPORT_REG_WRITE */

/*****************************************************************************/
#define CONFIG_YAFFS2_SUPPORT

#define HISFC400_CHIP_DELAY				25

#define HISFC400_ADDR_CYCLE_MASK			0x4
/*****************************************************************************/

#define HISFC400_REG_BASE_LEN                (0x100)
/*****************************************************************************/
#define HISFC400_CFG					0x00
#define HISFC400_CFG_BOOT_MODE(_mode)		((_mode) << 17)
#define HISFC400_CFG_INTERNAL_ECC       	(1 << 16)
#define HISFC400_CFG_FLASH_SIZE_CS1(_size)	(((_size) & 0xF) << 10)
#define HISFC400_CFG_FLASH_SIZE_CS0(_size)	(((_size) & 0xF) << 6)
#define HISFC400_CFG_ADDR_MODE(_mode)		((_mode) << 5)
#define HISFC400_CFG_ECC_TYPE(type)		(((type) & 0x3) << 3)
#define HISFC400_CFG_PAGE_SIZE(size)		((size) << 2)
#define HISFC400_CFG_DEVICE_TYPE(_type)		((_type) << 1)
#define HISFC400_CFG_OP_MODE(_mode)		((_mode) << 0)

#define BOOT_MODE_MASK				(0x1 << 17)

#define FLASH_SIZE_CS1_SHIFT			10
#define FLASH_SIZE_CS1_MASK			(0xF << 10)

#define FLASH_SIZE_CS0_SHIFT			6
#define FLASH_SIZE_CS0_MASK			(0xF << 6)

#define HISFC400_CFG_ECCTYPE_SHIFT		3
#define HISFC400_CFG_ECCTYPE_MASK		3

#define HISFC400_CFG_PAGEISZE_SHIFT		2
#define HISFC400_CFG_PAGEISZE_MASK		1

#define DEVICE_TYPE_MASK			(0x1 << 1)
#define DEVICE_TYPE_NAND_FLASH			(0x1 << 1)

#define OP_MODE_MASK				(0x1 << 0)
#define OP_MODE_BOOT				0
#define OP_MODE_NORMAL				1

/*****************************************************************************/
#define HISFC400_GLOBAL_CFG				0x04
#define HISFC400_GLOBAL_CFG_WP_ENABLE		(1 << 5)

/*****************************************************************************/
#define HISFC400_TIMING_CFG				0x08
#define HISFC400_TIMING_CFG_TCSH(_n)		(((_n) & 0xF) << 8)
#define HISFC400_TIMING_CFG_TCSS(_n)		(((_n) & 0xF) << 4)
#define HISFC400_TIMING_CFG_TSHSL(_n)		((_n) & 0xF)

/*****************************************************************************/
#define HISFC400_INTS					0x0c
#define HISFC400_INTS_OP_DONE			(1 << 0)

/*****************************************************************************/
#define HISFC400_INTS_EN				0x10
#define HISFC400_ALL_INTS_MASK			(0x7F)
#define HISFC400_INTS_EN_OP_DONE		(1U << 0)

/*****************************************************************************/

#define HISFC400_INTCLR					0x14
#define HISFC400_INTCLR_MASK			(0x7F)
#define HISFC400_INTCLR_DMA_ERR			(1 << 5)
#define HISFC400_INTCLR_OP_DONE			(1 << 0)

#define HISFC400_INTCLR_ALL				0x7F

/*****************************************************************************/
#define HISFC400_OPCODE					0x18

/*****************************************************************************/
#define HISFC400_OP_ADDR				0x1c
#define HISFC400_OP_ADDRH_BLOCK_MASK(_page)	(((_page) & 0xffff) << 16)
#define HISFC400_OP_ADDRL_BLOCK_MASK(_page)	((_page) & 0xffc0)

#define READ_ID_ADDR					0x00
#define PROTECTION_REGISTER				0xa0
#define FEATURE_REGISTER				0xb0
#define STATUS_REGISTER					0xc0

/*****************************************************************************/
#define HISFC400_OP					0x20
#define HISFC400_OP_SEL_CS(_cs)			((_cs) << 11)
#define HISFC400_OP_ADDR_NUM(_n)		(((_n) & 0x7) << 8)
#define HISFC400_OP_DUMMY_CMD_EN		(1 << 7)
#define HISFC400_OP_DUMMY_ADDR_EN		(1 << 6)
#define HISFC400_OP_OPCODE_EN			(1 << 5)
#define HISFC400_OP_ADDR_EN			(1 << 4)
#define HISFC400_OP_DATE_WRITE_EN		(1 << 3)
#define HISFC400_OP_DATE_READ_EN		(1 << 2)
#define HISFC400_OP_STATUS_READ_EN		(1 << 1)
#define HISFC400_OP_START			(1 << 0)

#define STD_OP_ADDR_NUM				3
#define READ_ID_ADDR_NUM			1

#define FEATURES_OP_ADDR_NUM			1

#define WRITE_PROTECTION_ENABLE_GIGA		((0x1f << 1) | (1 << 7))
#define WRITE_PROTECTION_ENABLE_WINBOND		(0xff)

/*****************************************************************************/
#define HISFC400_DATA_NUM				0x24
#define HISFC400_DATA_NUM_CNT(_n)		((_n) & 0x1FFF)

#define FEATURES_DATE_NUM			1
#define READ_OOB_BB_LEN				1

#define HISFC400_OOBSIZE_FOR_YAFFS			32

/*****************************************************************************/
#define HISFC400_OP_CFG					0x28
#define HISFC400_OP_CFG_DIR_TRANS_ENABLE	(1 << 11)
#define HISFC400_OP_CFG_RD_OP_TYPE_PAGE		0
#define HISFC400_OP_CFG_RD_OP_TYPE_DATA		0
#define HISFC400_OP_CFG_RD_OP_TYPE_OOB		0
#define HISFC400_OP_CFG_RD_OP_SEL(_type)	(((_type) & 0x3) << 9)
#define HISFC400_OP_CFG_MEM_IF_TYPE(_type)	(((_type) & 0x7) << 6)
#define HISFC400_OP_CFG_DUMMY_CMD_NUM(_n)	(((_n) & 0x7) << 3)
#define HISFC400_OP_CFG_DUMMY_ADDR_NUM(_n)	(((_n) & 0x7) << 0)

#define RD_OP_READ_PAGE					0x0
#define RD_OP_READ_DATE					0x1
#define RD_OP_READ_OOB					0x2

/*****************************************************************************/
#define HISFC400_ADDRH					0x2c
#define HISFC400_ADDRH_SET(_addr)		((_addr) & 0xff)

/*****************************************************************************/
#define HISFC400_ADDRL					0x30

/*****************************************************************************/
#define HISFC400_OP_CTRL				0x34
#define HISFC400_OP_CTRL_RD_OPCODE(_code)	(((_code) & 0xff) << 16)
#define HISFC400_OP_CTRL_WR_OPCODE(_code)	(((_code) & 0xff) << 8)
#define HISFC400_OP_CTRL_CS_OP(_cs)		((_cs) << 3)
#define HISFC400_OP_CTRL_OP_TYPE(_type)		((_type) << 2)
#define HISFC400_OP_CTRL_RW_OP(_op)		((_op) << 1)
#define HISFC400_OP_CTRL_OP_READY		(1 << 0)

#define OP_TYPE_REG					0
#define OP_TYPE_DMA					1

#define RW_OP_READ					0
#define RW_OP_WRITE					1

/*****************************************************************************/
#define HISFC400_DMA_CTRL				0x3c

#define HISFC400_DMA_BURST4_ENABLE		(1 << 0)
#define HISFC400_DMA_BURST8_ENABLE		(1 << 1)
#define HISFC400_DMA_BURST16_ENABLE		(1 << 2)

/*****************************************************************************/
#define HISFC400_DMA_ADDR_DATA				0x40
#define HISFC400_DMA_ADDR_OOB				0x44

#define HISFC400_DMA_LEN				0x48
#define HISFC400_DMA_LEN_SET(_len)		((_len) & 0xfffffff)

/*****************************************************************************/
#define HISFC400_STATUS					0x54

#define WP_UNLOCK					0x0
#define WP_LOCKED					0x38

#define GET_FEATURE					0
#define SET_FEATURE					1

/* only WINBOND chip support Buffer Read Mode and Continuous Read Mode */
#define FEATURE_BUF_ENABLE			(1 << 3)
#define FEATURE_ECC_ENABLE			(1 << 4)
#define FEATURE_OTP_ENABLE			(1 << 6)
#define FEATURE_QUAD_ENABLE			(1 << 0)

#define STATUS_ECC_MASK(_n)			(((_n) & 0x03) << 4)
#define STATUS_P_FAIL_MASK			(1 << 3)
#define STATUS_E_FAIL_MASK			(1 << 2)
#define STATUS_WEL_MASK				(1 << 1)
#define STATUS_OIP_MASK				(1 << 0)

#define PROTECTION_BRWD_MASK			(1 << 7)
#define PROTECTION_BP3_MASK			(1 << 6)
#define PROTECTION_BP2_MASK			(1 << 5)
#define PROTECTION_BP1_MASK			(1 << 4)
#define PROTECTION_BP0_MASK			(1 << 3)

#define ALL_BP_MASK				(PROTECTION_BP3_MASK \
						| PROTECTION_BP2_MASK \
						| PROTECTION_BP1_MASK \
						| PROTECTION_BP0_MASK)

/*****************************************************************************/
#define HISFC400_VERSION					0x68

/*****************************************************************************/
#define HISFC400_GLOBAL_CONFIG					0x0100
#define HISFC400_GLOBAL_CONFIG_READ_DELAY(_n)		(((_n) & 0x03) << 3)
#define HISFC400_GLOBAL_CONFIG_ADDR_MODE_DIS_4B		(0 << 2)
#define HISFC400_GLOBAL_CONFIG_WRITE_PROTECT		(1 << 1)
#define HISFC400_GLOBAL_CONFIG_SPI_MODE3		(1 << 0)

#define HISFC400_INT_RAW_STATUS					0x0120
#define HISFC400_INT_RAW_STATUS_DMA_DONE		(1<<1)
#define HISFC400_INT_STATUS				0x0124
#define HISFC400_INT_MASK				0x0128
#define HISFC400_INT_CLEAR				0x012C
#define HISFC400_INT_CLEAR_DMA_DONE			(1<<1)

#define HISFC400_BUS_CONFIG1					0x0200
#define HISFC400_BUS_CONFIG1_READ_EN			(1<<31)
#define HISFC400_BUS_CONFIG1_WRITE_EN			(1<<30)
#define HISFC400_BUS_CONFIG1_WRITE_INS(_n)		((_n & 0xFF) << 22)
#define HISFC400_BUS_CONFIG1_WRITE_DUMMY_CNT(_n)	((_n & 0x7) << 19)
#define HISFC400_BUS_CONFIG1_WRITE_IF_TYPE(_n)		((_n & 0x7) << 16)
#define HISFC400_BUS_CONFIG1_READ_INS(_n)		((_n & 0xFF) << 8)
#define HISFC400_BUS_CONFIG1_READ_PREF_CNT(_n)		((_n & 0x3) << 6)
#define HISFC400_BUS_CONFIG1_READ_DUMMY_CNT(_n)		((_n & 0x7) << 3)
#define HISFC400_BUS_CONFIG1_READ_IF_TYPE(_n)		(_n & 0x7)

#define HISFC400_BUS_CONFIG2					0x0204
#define HISFC400_BUS_CONFIG2_WIP_LOCATE(_n)		(_n & 0x7)

#define HISFC400_BUS_FLASH_SIZE					0x0210
#define HISFC400_BUS_BASE_ADDR_CS0				0x0214
#define HISFC400_BUS_BASE_ADDR_CS1				0x0218
#define HISFC400_BUS_ALIAS_ADDR					0x021C
#define HISFC400_BUS_ALIAS_CS					0x0220
#define HISFC400_BUS_DMA_CTRL					0x0240
#define HISFC400_BUS_DMA_CTRL_START			(1 << 0)
#define HISFC400_BUS_DMA_CTRL_RW(_rw)			((_rw)<<1)
#define HISFC400_BUS_DMA_CTRL_CS(_cs)			(((_cs) & 0x01) << 4)

#define HISFC400_BUS_DMA_FLASH_SADDR_H				0x0248
#define HISFC400_BUS_DMA_FLASH_SADDR_L				0x0248
#define HISFC400_BUS_DMA_LEN					0x024C
#define HISFC400_BUS_DMA_LEN_DATA_CNT(_n)		((_n - 1) & 0x0FFFFFFF)
#define HISFC400_BUS_DMA_AHB_CTRL				0x0250
#define HISFC400_BUS_DMA_AHB_CTRL_INCR4_EN		(1<<0)
#define HISFC400_BUS_DMA_AHB_CTRL_INCR8_EN		(1<<1)
#define HISFC400_BUS_DMA_AHB_CTRL_INCR16_EN		(1<<2)

#define HISFC400_CMD_ADDR					0x030C
#define HISFC400_CMD_ADDR_MASK				0x3FFFFFFF
#define HISFC400_CMD_DATABUF0				0x0400
#define HISFC400_CMD_DATABUF15				0x043C

#undef  FALSE
#define FALSE          0

#undef  TRUE
#define TRUE		1

/*****************************************************************************/
#define SPI_NAND_MAX_PAGESIZE				4096
#define SPI_NAND_MAX_OOBSIZE				256

#if 0
#  define DBG_MSG(_fmt, arg...)
#else
#  define DBG_MSG(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#endif

#define DBG_WARN(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);

#undef DBG_BUG
#define DBG_BUG(fmt, args...) do {\
	printf("%s(%d): BUG: " fmt, __FILE__, __LINE__, ##args); \
	asm("b ."); \
} while (0)


/*****************************************************************************/
struct hisfc_cmd_option {
	unsigned char chipselect;
	unsigned char command;
	unsigned char last_cmd;
	unsigned char address_h;
	unsigned int address_l;
	unsigned int date_num;
	unsigned short option;
	unsigned short op_config;
};

struct hisfc_host;

struct hisfc_host {
	struct nand_chip *chip;
	struct mtd_info  *mtd;
	void __iomem *iobase;
	void __iomem *regbase;
	void __iomem *sysreg;

	unsigned long NFC_CFG;

	struct hisfc_operation spi[CONFIG_HISFC400_MAX_CHIP];
	unsigned int command;

	int chipselect;

	unsigned int offset;

	struct device *dev;

	unsigned int addr_cycle;
	unsigned int addr_value[2];
	unsigned int cache_addr_value[2];
	unsigned int column;
	unsigned int block_page_mask;

	unsigned int dma_buffer;
	unsigned int dma_oob;

	unsigned int ecctype;
	unsigned int pagesize;
	unsigned int oobsize;

	/* This is maybe an un-aligment address, only for malloc or free */
	char *buforg;
	char *buffer;

	int add_partition;

	/* BOOTROM read two bytes to detect the bad block flag */
#define HISFC_BAD_BLOCK_POS              0
	unsigned char *bbm;  /* nand bad block mark */
	unsigned short *epm;  /* nand empty page mark */

	unsigned int uc_er;

	void (*set_system_clock)(struct hisfc_host *host,
				 struct spi_operation *op, int clk_en);

	void (*send_cmd_pageprog)(struct hisfc_host *host);
	void (*send_cmd_status)(struct hisfc_host *host);
	void (*send_cmd_readstart)(struct hisfc_host *host);
	void (*send_cmd_erase)(struct hisfc_host *host);
	void (*send_cmd_readid)(struct hisfc_host *host);
	void (*send_cmd_reset)(struct hisfc_host *host);
};

/*****************************************************************************/
#define hisfc_read(_host, _reg) \
	readl((char *)_host->regbase + (_reg))

#define hisfc_write(_host, _reg, _value) \
	writel((unsigned)(_value), ((char *)_host->regbase + (_reg)))

/*****************************************************************************/
#define HISFC400_CMD_WAIT_CPU_FINISH(_host) do { \
	unsigned timeout = 0xF0000000; \
	while ((hisfc_read((_host), HISFC400_OP) \
		& HISFC400_OP_START) && timeout) \
		--timeout; \
	if (!timeout) \
		DBG_BUG("cmd wait cpu finish timeout\n"); \
} while (0)

/*****************************************************************************/
#define HISFC400_DMA_WAIT_INT_FINISH(_host) do { \
	unsigned timeout = 0xF000000; \
	while ((!(hisfc_read((_host), HISFC400_INTS) \
		& HISFC400_INTS_OP_DONE) && timeout)) \
		--timeout; \
	if (!timeout) \
		DBG_BUG("dma wait int finish timeout\n"); \
} while (0)

/*****************************************************************************/
#define HISFC400_DMA_WAIT_CPU_FINISH(_host) do { \
	unsigned timeout = 0xF0000000; \
	while ((!(hisfc_read((_host), HISFC400_OP_CTRL) \
		& HISFC400_OP_CTRL_OP_READY) && timeout)) \
		--timeout; \
	if (!timeout) \
		DBG_BUG("dma wait cpu finish timeout\n"); \
} while (0)

/*****************************************************************************/
int hisfc400_host_init(struct hisfc_host *host);

void hisfc400_nand_init(struct nand_chip *chip);

void spi_feature_op(struct hisfc_host *host, unsigned char op,
		    unsigned char addr, unsigned *regval);

/******************************************************************************/
#endif /* __HISFCV400_H__ */

