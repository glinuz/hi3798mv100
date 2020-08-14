/******************************************************************************
*    Copyright (c) 2014 by Hisilicon.
*    All rights reserved.
* ***
*    Create By hisilicon
******************************************************************************/

#ifndef __HISFC_SPI_IDS_H__
#define __HISFC_SPI_IDS_H__

/*****************************************************************************/
#define INFINITE			(0xFFFFFFFF)

#define READ_ID_LEN			8
#define MAX_SPI_NAND_OP			8

#define BBP_LAST_PAGE			0x01
#define BBP_FIRST_PAGE			0x02

/******************************************************************************/

#define SPI_NAND_READ			0
#define SPI_NAND_WRITE			1

/******************************************************************************/

struct spi_nand_chip_info_t
{
	char *name;
	unsigned char id[READ_ID_LEN];
	unsigned int length;
	unsigned long long chipsize;
	unsigned int erasesize;
	unsigned int pagesize;
	unsigned int oobsize;
	unsigned int badblock_pos;
	struct spi_operation *read[MAX_SPI_NAND_OP];
	struct spi_operation *write[MAX_SPI_NAND_OP];
	struct spi_operation *erase[MAX_SPI_NAND_OP];
	struct spi_nand_driver *driver;
};
/*****************************************************************************/
extern struct spi_nand_chip_info_t *(*find_spl_spi_nand_chip)(void *nand_dev, char id[8]);

/******************************************************************************/

#endif /* __HISFC_SPI_IDS_H__ */

