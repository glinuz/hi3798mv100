/******************************************************************************
*    Copyright (c) 2014 by Hisilicon.
*    All rights reserved.
* ***
*    Create By hisilicon
******************************************************************************/

#ifndef __HISFC_SPI_NAND_SPL_IDS_H__
#define __HISFC_SPI_NAND_SPL_IDS_H__

/*****************************************************************************/
#define SPI_NAND_DRV_VERSION     "1.0"

#define HISFC400_SUPPORT_READ		(SPI_IF_READ_STD \
					| SPI_IF_READ_DUAL \
					| SPI_IF_READ_DUAL_ADDR \
					| SPI_IF_READ_QUAD \
					| SPI_IF_READ_QUAD_ADDR)

#define HISFC400_SUPPORT_WRITE		(SPI_IF_WRITE_STD \
					| SPI_IF_WRITE_DUAL \
					| SPI_IF_WRITE_DUAL_ADDR \
					| SPI_IF_WRITE_QUAD \
					| SPI_IF_WRITE_QUAD_ADDR)

#define HISFC400_SUPPORT_MAX_DUMMY	8

/*****************************************************************************/
#define SPI_IF_READ_STD                 (0x01)
#define SPI_IF_READ_FAST                (0x02)
#define SPI_IF_READ_DUAL                (0x04)
#define SPI_IF_READ_DUAL_ADDR           (0x08)
#define SPI_IF_READ_QUAD                (0x10)
#define SPI_IF_READ_QUAD_ADDR           (0x20)

#define SPI_IF_WRITE_STD                (0x01)
#define SPI_IF_WRITE_DUAL               (0x02)
#define SPI_IF_WRITE_DUAL_ADDR          (0x04)
#define SPI_IF_WRITE_QUAD               (0x08)
#define SPI_IF_WRITE_QUAD_ADDR          (0x10)

#define SPI_IF_ERASE_SECTOR_128K        (0x08)  /* 128K */
#define SPI_IF_ERASE_SECTOR_256K        (0x10)  /* 256K */

/*****************************************************************************/
#define SPI_CMD_WREN			0x06   /* Write Enable */
#define SPI_CMD_WRDI			0x04   /* Write Disable */

#define SPI_CMD_GET_FEATURES		0x0F   /* Get Features */
#define SPI_CMD_SET_FEATURE		0x1F   /* Set Feature */

#define SPI_CMD_PAGE_READ		0x13   /* Page Read to Cache */
#define SPI_CMD_READ_STD	0x03   /* Read From Cache at Standard Speed */
#define SPI_CMD_READ_FAST	0x0B   /* Read From Cache at Higher Speed */
#define SPI_CMD_READ_DUAL	0x3B   /* Read From Cache at Dual Output */
#define SPI_CMD_READ_QUAD	0x6B   /* Read From Cache at Quad Output */
#define SPI_CMD_READ_DUAL_ADDR	0xBB   /* Read From Cache at Dual I/O */
#define SPI_CMD_READ_QUAD_ADDR	0xEB   /* Read From Cache at Quad I/O */

#define SPI_CMD_RDID			0x9F   /* Read Identification */

#define SPI_CMD_WRITE_STD	0x02   /* Page Load at Standard Input */
#define SPI_CMD_WRITE_QUAD	0x32   /* Page Load at Quad Input */

#define SPI_CMD_SE_128K			0xD8   /* 128KB sector Erase */
#define SPI_CMD_SE_256K			0xD8   /* 256KB sector Erase */

#define SPI_CMD_RESET			0xff   /* Reset the device */

/*****************************************************************************/

#define HISFC400_REG_IFCYCLE_STD			0
#define HISFC400_REG_IFCYCLE_DUAL			1
#define HISFC400_REG_IFCYCLE_DUAL_ADDR			2
#define HISFC400_REG_IFCYCLE_QUAD			3
#define HISFC400_REG_IFCYCLE_QUAD_ADDR			4

/*****************************************************************************/
#define SPI_NAND_STATUS_READY      0x01
#define SPI_NAND_STATUS_PFAIL      0x08
#define SPI_NAND_STATUS_EFAIL      0x04

/*****************************************************************************/
struct spi_operation {
	unsigned char  iftype;
	unsigned char  cmd;
	unsigned char  dummy;
	unsigned int   size;
	unsigned int   clock;
};

/*****************************************************************************/
struct spi_nand_driver;

struct hisfc_operation {
	void *host;
	struct spi_nand_driver *driver;
	struct spi_operation  read[1];
	struct spi_operation  write[1];
	struct spi_operation  erase[MAX_SPI_NAND_OP];
};

struct spi_nand_driver {
	int (*wait_ready)(struct hisfc_operation *spi);
	int (*write_enable)(struct hisfc_operation *spi);
	int (*quad_enable)(struct hisfc_operation *spi);
};

/*****************************************************************************/
void hisfc400_get_best_clock(unsigned int *clock);

struct spi_nand_chip_info_t *hisfc400_find_spl_spi_nand_chip(void *nand_dev, 
							     char id[8]);

#endif /* __HISFC_SPI_NAND_SPL_IDS_H__ */
