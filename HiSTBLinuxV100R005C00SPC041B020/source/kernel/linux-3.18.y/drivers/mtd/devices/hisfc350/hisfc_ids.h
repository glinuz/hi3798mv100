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
 * Create By Cai Zhiyong 2015.6.11
 *
******************************************************************************/

#ifndef HISPI_IDSH
#define HISPI_IDSH
/*****************************************************************************/

#define INFINITE                  (0xFFFFFFFF)

#define SPI_IF_READ_STD           (0x01)
#define SPI_IF_READ_FAST          (0x02)
#define SPI_IF_READ_DUAL          (0x04)
#define SPI_IF_READ_DUAL_ADDR     (0x08)
#define SPI_IF_READ_QUAD          (0x10)
#define SPI_IF_READ_QUAD_ADDR     (0x20)

#define SPI_IF_WRITE_STD          (0x01)
#define SPI_IF_WRITE_DUAL         (0x02)
#define SPI_IF_WRITE_DUAL_ADDR    (0x04)
#define SPI_IF_WRITE_QUAD         (0x08)
#define SPI_IF_WRITE_QUAD_ADDR    (0x10)

/*macro for v350*/
#define SPI_IF_ERASE_SECTOR_4K    (0x01)  /* 4K */
/*macro for v350*/
#define SPI_IF_ERASE_SECTOR_32K   (0x02)  /* 32K */
/*macro for v300*/
#define SPI_IF_ERASE_CHIP         (0x02) /*chip erase*/
/*macro for v350*/
#define SPI_IF_ERASE_SECTOR_64K   (0x04)  /* 64K */
/*macro for v300*/
#define SPI_IF_ERASE_SECTOR       (0x04)  /* sector erase, 64K */
/*macro for v350*/
#define SPI_IF_ERASE_SECTOR_256K  (0x08)  /* 256K */
/*****************************************************************************/

#define SPI_CMD_WREN                   0x06   /* Write Enable */
//-----------------------------------------------------------------------------
#define SPI_CMD_SE_4K                  0x20   /* 4KB sector Erase */
#define SPI_CMD_SE_32K                 0x52   /* 32KB sector Erase */
#define SPI_CMD_SE_64K                 0xD8   /* 64KB sector Erase */
#define SPI_CMD_SE_256K                0xD8   /* 256KB sector Erase */

#define SPI_CMD_SE                     0xD8   /* 64KB Sector Erase */
#define SPI_CMD_BE                     0xC7   /* chip erase */
//-----------------------------------------------------------------------------
#define SPI_CMD_RDSR                   0x05   /* Read Status Register */
#define SPI_CMD_RDID                   0x9F   /* Read Identification */
//-----------------------------------------------------------------------------
#define SPI_CMD_PP                     0x02   /* Page Programming */
#define SPI_CMD_WRITE_DUAL             0xA2   /* fast program dual input */
#define SPI_CMD_WRITE_QUAD             0x32   /* fast program quad input */
#define SPI_CMD_WRITE_DUAL_ADDR        0xD2   /* Dual I/O High Performance Write */
#define SPI_CMD_WRITE_QUAD_ADDR        0x12   /* Quad I/O High Performance Write */
//-----------------------------------------------------------------------------
#define SPI_CMD_READ                   0x03   /* Read Data bytes */
#define SPI_CMD_FAST_READ              0x0B   /* Read Data Bytes at Higher Speed */
#define SPI_CMD_READ_DUAL              0x3B   /* fast read dual output */
#define SPI_CMD_READ_QUAD              0x6B   /* fast read quad output */
#define SPI_CMD_READ_DUAL_ADDR         0xBB   /* Dual I/O High Performance Read */
#define SPI_CMD_READ_QUAD_ADDR         0xEB   /* Quad I/O High Performance Read */
//-----------------------------------------------------------------------------
#define SPI_CMD_SR_WIP                 1      /* Write in Progress */
#define SPI_CMD_SR_WEL                 2      /* Write Enable Latch */
//-----------------------------------------------------------------------------
#define SPI_CMD_EN4B                   0xB7  /* enter to 4 bytes mode and set 4 byte bit as '1' */
#define SPI_CMD_EX4B                   0xE9  /* exit 4 bytes mode and clear 4 byte bit as '0' */

/*****************************************************************************/

struct spi_operation {
	u_char iftype;
	u_char cmd;
	u_char dummy;
	u32 size;
	u32 clock;
};

struct spi_info {
	char *name;

	u_char id[8];
	u32 id_len;

	u32 chipsize;
	u32 erasesize;
	u32 addrcycle;

#define MAX_SPI_OP                       (8)
	struct spi_operation *read[8];
	struct spi_operation *write[8];
	struct spi_operation *erase[8];
	struct spi_driver *driver;
};
/*****************************************************************************/

struct spi_info *spi_serach_ids(struct spi_info *spi_info_table,
	unsigned char ids[8]);

void spi_search_rw(struct spi_info *spiinfo, struct spi_operation *spiop_rw,
	unsigned int iftype, unsigned int max_dummy, int is_read);

void spi_get_erase(struct spi_info *spiinfo, struct spi_operation *spiop_erase);

/******************************************************************************/
#endif /* HISPI_IDSH */
