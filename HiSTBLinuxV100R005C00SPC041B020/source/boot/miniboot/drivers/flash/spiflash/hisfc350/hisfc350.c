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

/*****************************************************************************/
#include <config.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <module.h>
#include <malloc.h>

#include <asm/io.h>

#include "sf_drv.h"
#include "spi_ids.h"
#include "hisfc350.h"

extern void hisfc350_set_system_clock(struct hisfc_host *host,
         struct spi_operation *op, int clk_en);
extern void hisfc350_get_best_clock(unsigned int * clock);
/*****************************************************************************/
/* Don't change the follow config */
#define HISFC350_SUPPORT_READ (SPI_IF_READ_STD \
	| SPI_IF_READ_FAST \
	| SPI_IF_READ_DUAL \
	| SPI_IF_READ_DUAL_ADDR \
	| SPI_IF_READ_QUAD \
	| SPI_IF_READ_QUAD_ADDR)

#define HISFC350_SUPPORT_WRITE (SPI_IF_WRITE_STD \
	| SPI_IF_WRITE_DUAL \
	| SPI_IF_WRITE_DUAL_ADDR \
	| SPI_IF_WRITE_QUAD \
	| SPI_IF_WRITE_QUAD_ADDR)

#define HISFC350_SUPPORT_MAX_DUMMY        (7)

#undef HISFCV350_SUPPORT_REG_READ

/* Don't print time cycle. */
// #define CONFIG_HISFC350_SHOW_CYCLE_TIMING
/*****************************************************************************/

static struct hisfc_host hisfc_host;
static struct spiflash_dev_t *__spiflash = NULL;

extern struct spi_info hisfc350_spi_info_table[];
/*****************************************************************************/
#ifdef CONFIG_HISFC350_SHOW_CYCLE_TIMING
static char *hisfc350_get_ifcycle_str(int ifcycle)
{
	static char *ifcycle_str[] = {
		"single",
		"dual",
		"dual-addr",
		"dual-cmd",
		"reserve",
		"quad",
		"quad-addr",
		"quad-cmd",
	};

	return ifcycle_str[(ifcycle & 0x07)];
}
#endif /* CONFIG_HISFC350_SHOW_CYCLE_TIMING */
/*****************************************************************************/
static void hisfc350_set_host_addr_mode(struct hisfc_host *host, int enable)
{
	unsigned int regval;
	regval = hisfc_read(host, HISFC350_GLOBAL_CONFIG);
	if (enable)
		regval |= HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B;
	else
		regval &= ~HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B;

	hisfc_write(host, HISFC350_GLOBAL_CONFIG, regval);
}
/*****************************************************************************/
#if 0 /* All SPIFlash exit from 4Bytes mode, used for suspend. */
static void hisfc350_exit_4byte_mode(void)
{
	int ix;
	struct hisfc_host *host = &hisfc_host;
	struct hisfc_spi *spi = host->spi;

	for (ix = 0; ix < host->num_chip; ix++) {
		if (spi->addrcycle == 4) 
			spi->driver->entry_4addr(spi, 0);
		spi++;
	}
}
#endif
/*****************************************************************************/
static void hisfc350_map_iftype_and_clock(struct hisfc_spi *spi)
{
	int ix;
	const int iftype_read[] = {
		SPI_IF_READ_STD,       HISFC350_IFCYCLE_STD,
		SPI_IF_READ_FAST,      HISFC350_IFCYCLE_STD,
		SPI_IF_READ_DUAL,      HISFC350_IFCYCLE_DUAL,
		SPI_IF_READ_DUAL_ADDR, HISFC350_IFCYCLE_DUAL_ADDR,
		SPI_IF_READ_QUAD,      HISFC350_IFCYCLE_QUAD,
		SPI_IF_READ_QUAD_ADDR, HISFC350_IFCYCLE_QUAD_ADDR,
		0,0,
	};
	const int iftype_write[] = {
		SPI_IF_WRITE_STD,       HISFC350_IFCYCLE_STD,
		SPI_IF_WRITE_DUAL,      HISFC350_IFCYCLE_DUAL,
		SPI_IF_WRITE_DUAL_ADDR, HISFC350_IFCYCLE_DUAL_ADDR,
		SPI_IF_WRITE_QUAD,      HISFC350_IFCYCLE_QUAD,
		SPI_IF_WRITE_QUAD_ADDR, HISFC350_IFCYCLE_QUAD_ADDR,
		0,0,
	};

	for (ix = 0; iftype_write[ix]; ix += 2) {
		if (spi->write->iftype == iftype_write[ix]) {
			spi->write->iftype = iftype_write[ix + 1];
			break;
		}
	}
	hisfc350_get_best_clock(&spi->write->clock);

	for (ix = 0; iftype_read[ix]; ix += 2) {
		if (spi->read->iftype == iftype_read[ix]) {
			spi->read->iftype = iftype_read[ix + 1];
			break;
		}
	}
	hisfc350_get_best_clock(&spi->read->clock);

	hisfc350_get_best_clock(&spi->erase->clock);
	spi->erase->iftype = HISFC350_IFCYCLE_STD;
}
/*****************************************************************************/
static void hisfc350_dma_transfer(struct hisfc_host *host,
				  unsigned int spi_start_addr,
				  unsigned char *dma_buffer,
				  unsigned char is_read,
				  unsigned int size, unsigned char chipselect)
{
	hisfc_write(host, HISFC350_BUS_DMA_MEM_SADDR, dma_buffer);

	hisfc_write(host, HISFC350_BUS_DMA_FLASH_SADDR,
		spi_start_addr);

	hisfc_write(host, HISFC350_BUS_DMA_LEN,
		HISFC350_BUS_DMA_LEN_DATA_CNT(size));

	hisfc_write(host, HISFC350_BUS_DMA_AHB_CTRL,
		HISFC350_BUS_DMA_AHB_CTRL_INCR4_EN
		| HISFC350_BUS_DMA_AHB_CTRL_INCR8_EN
		| HISFC350_BUS_DMA_AHB_CTRL_INCR16_EN);

	hisfc_write(host, HISFC350_BUS_DMA_CTRL,
		HISFC350_BUS_DMA_CTRL_RW(is_read)
		| HISFC350_BUS_DMA_CTRL_CS(chipselect)
		| HISFC350_BUS_DMA_CTRL_START);

	HISFC350_DMA_WAIT_CPU_FINISH(host);

}
/*****************************************************************************/
#ifdef HISFCV350_SUPPORT_REG_READ
static char * hisfc350_reg_read_buf(struct hisfc_host *host,
				    struct hisfc_spi *spi,
				    unsigned int spi_start_addr,
				    unsigned int size, unsigned char *buffer)
{
	int numread;
	int index = 0;
	unsigned char *ptr = buffer;

	if (size > HISFC350_REG_BUF_SIZE)
		DBG_BUG("reg read out of reg range.\n");

	hisfc_write(host, HISFC350_CMD_INS, spi->read->cmd);
	hisfc_write(host, HISFC350_CMD_ADDR,
		(spi_start_addr & HISFC350_CMD_ADDR_MASK));
	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_MEM_IF_TYPE(spi->read->iftype)
		| HISFC350_CMD_CONFIG_DATA_CNT(size)
		| HISFC350_CMD_CONFIG_RW_READ
		| HISFC350_CMD_CONFIG_DATA_EN
		| HISFC350_CMD_CONFIG_DUMMY_CNT(spi->read->dummy)
		| HISFC350_CMD_CONFIG_ADDR_EN
		| HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
		| HISFC350_CMD_CONFIG_START);

	HISFC350_CMD_WAIT_CPU_FINISH(host);

	numread = (size & ~0x03);
	while (numread) {
		*(unsigned int*)ptr = hisfc_read(host,
			HISFC350_CMD_DATABUF0 + index);
		ptr      += 4;
		index    += 4;
		numread  -= 4;
	}

	numread = (size & 0x03);
	if (numread) {
		char tmp[4];
		*(unsigned int*)tmp = hisfc_read(host,
			HISFC350_CMD_DATABUF0 + index);
		memcpy(ptr, tmp, numread);
	}

	return buffer;
}
/*****************************************************************************/
static int hisfc350_reg_read(struct spiflash_dev_t *spiflash, uint32 from, 
			     size_t len, void *buf)
{
	int num;
	int result = -EIO;
	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((from + len) > spiflash->size) {
		DBG_MSG("read area out of range.\n");
		return -EINVAL;
	}

	if (!len) {
		DBG_MSG("read length is 0.\n");
		return 0;
	}

	if (spi->driver->wait_ready(spi))
		goto fail;
	host->set_system_clock(host, spi->read, TRUE);

	while (len > 0) {
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("read memory out of range.\n");
			if (spi->driver->wait_ready(spi))
				goto fail;
			host->set_system_clock(host, spi->read, TRUE);
		}

		num = ((from + len) >= spi->chipsize)
			? (spi->chipsize - from) : len;

		while (num >= HISFC350_REG_BUF_SIZE) {
			hisfc350_reg_read_buf(host, spi,
				from, HISFC350_REG_BUF_SIZE, ptr);
			ptr  += HISFC350_REG_BUF_SIZE;
			from += HISFC350_REG_BUF_SIZE;
			len  -= HISFC350_REG_BUF_SIZE;
			num  -= HISFC350_REG_BUF_SIZE;
		}

		if (num) {
			hisfc350_reg_read_buf(host, spi,
				from, num, ptr);
			from += num;
			ptr  += num;
			len  -= num;
		}
	}
	result = 0;
fail:
	
	return result;
}
#endif /* HISFCV350_SUPPORT_REG_READ */
/*****************************************************************************/

static int hisfc350_dma_read(struct spiflash_dev_t *spiflash, uint32 from,
			     size_t len, void *buf)
{
	int num;
	int result = -EIO;
	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((from + len) > spiflash->size) {
		DBG_MSG("read area out of range.\n");
		return -EINVAL;
	}

	if (!len) {
		DBG_MSG("read length is 0.\n");
		return 0;
	}

	if (spi->driver->wait_ready(spi))
		goto fail;
	spi->driver->bus_prepare(spi, READ);

	if (from & HISFC350_DMA_ALIGN_MASK) {
		num = HISFC350_DMA_ALIGN_SIZE - (from & HISFC350_DMA_ALIGN_MASK);
		if (num > len)
			num = len;
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("write memory out of range.\n");
			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->bus_prepare(spi, READ);
		}
		hisfc350_dma_transfer(host, from,
			(unsigned char *)host->dma_buffer, READ,
			num, spi->chipselect);
		memcpy(ptr, host->buffer, num);
		from  += num;
		ptr += num;
		len -= num;
	}

	while (len > 0) {
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("read memory out of range.\n");
			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->bus_prepare(spi, READ);
		}

		num = ((from + len) >= spi->chipsize)
			? (spi->chipsize - from) : len;
		while (num >= HISFC350_DMA_MAX_SIZE) {
			hisfc350_dma_transfer(host, from,
				(unsigned char *)host->dma_buffer, READ,
				HISFC350_DMA_MAX_SIZE, spi->chipselect);
			memcpy(ptr, host->buffer, HISFC350_DMA_MAX_SIZE);
			ptr  += HISFC350_DMA_MAX_SIZE;
			from += HISFC350_DMA_MAX_SIZE;
			len  -= HISFC350_DMA_MAX_SIZE;
			num  -= HISFC350_DMA_MAX_SIZE;
		}

		if (num) {
			hisfc350_dma_transfer(host, from,
				(unsigned char *)host->dma_buffer, READ,
				num, spi->chipselect);
			memcpy(ptr, host->buffer, num);
			from += num;
			ptr  += num;
			len  -= num;
		}
	}
	result = 0;
fail:
	
	return result;
}
/*****************************************************************************/
static unsigned char *hisfc350_read_ids(struct hisfc_host *host, int chipselect,
					unsigned char *buffer)
{
	int regindex = 0;
	int numread = 8;
	unsigned int *ptr = (unsigned int *)buffer;

	if (numread > HISFC350_REG_BUF_SIZE)
		numread = HISFC350_REG_BUF_SIZE;

	hisfc_write(host, HISFC350_CMD_INS, SPI_CMD_RDID);
	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_SEL_CS(chipselect)
		| HISFC350_CMD_CONFIG_RW_READ
		| HISFC350_CMD_CONFIG_DATA_EN
		| HISFC350_CMD_CONFIG_DATA_CNT(numread)
		| HISFC350_CMD_CONFIG_START);

	HISFC350_CMD_WAIT_CPU_FINISH(host);

	while (numread) {
		*ptr = hisfc_read(host,
			HISFC350_CMD_DATABUF0 + regindex);
		ptr      += 1;
		regindex += 4;
		numread  -= 4;
	}

	return buffer;
}
/*****************************************************************************/
static int hisfc350_reg_erase_one_block(struct hisfc_host *host,
					struct hisfc_spi *spi,
					unsigned int offset)
{
	if (spi->driver->wait_ready(spi))
		return 1;
	spi->driver->write_enable(spi);
	host->set_system_clock(host, spi->erase, TRUE);

	hisfc_write(host, HISFC350_CMD_INS, spi->erase->cmd);

	hisfc_write(host, HISFC350_CMD_ADDR,
		(offset & HISFC350_CMD_ADDR_MASK));

	hisfc_write(host, HISFC350_CMD_CONFIG,
		HISFC350_CMD_CONFIG_SEL_CS(spi->chipselect)
		| HISFC350_CMD_CONFIG_MEM_IF_TYPE(spi->erase->iftype)
		| HISFC350_CMD_CONFIG_DUMMY_CNT(spi->erase->dummy)
		| HISFC350_CMD_CONFIG_ADDR_EN
		| HISFC350_CMD_CONFIG_START);

	HISFC350_CMD_WAIT_CPU_FINISH(host);

	return 0;
}
/*****************************************************************************/

static int hisfc350_dma_write(struct spiflash_dev_t *spiflash, uint32 to,
			      size_t len, const void *buf)
{
	int num;
	int result = -EIO;

	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((to + len) > spiflash->size) {
		DBG_MSG("write data out of range.\n");
		return -EINVAL;
	}

	if (!len) {
		DBG_MSG("write length is 0.\n");
		return 0;
	}

	if (spi->driver->wait_ready(spi))
		goto fail;

	spi->driver->write_enable(spi);
	spi->driver->bus_prepare(spi, WRITE);

	if (to & HISFC350_DMA_ALIGN_MASK) {
		num = HISFC350_DMA_ALIGN_SIZE - (to & HISFC350_DMA_ALIGN_MASK);
		if (num > len)
			num = len;
		while (to >= spi->chipsize) {
			to -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("write memory out of range.\n");
			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->write_enable(spi);
			spi->driver->bus_prepare(spi, WRITE);
		}
		memcpy(host->buffer, ptr, num);
		hisfc350_dma_transfer(host, to,
			(unsigned char *)host->dma_buffer, WRITE,
			num, spi->chipselect);

		to  += num;
		ptr += num;
		len -= num;
	}

	while (len > 0) {
		num = ((len >= HISFC350_DMA_MAX_SIZE)
			? HISFC350_DMA_MAX_SIZE : len);
		while (to >= spi->chipsize) {
			to -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("write memory out of range.\n");
			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->write_enable(spi);
			spi->driver->bus_prepare(spi, WRITE);
		}

		memcpy(host->buffer, ptr, num);
		hisfc350_dma_transfer(host, to,
			(unsigned char *)host->dma_buffer, WRITE,
			num, spi->chipselect);

		to  += num;
		ptr += num;
		len -= num;
	}

	result = 0;
fail:
	return result;
}
/*****************************************************************************/

static int hisfc350_reg_erase(struct spiflash_dev_t *spiflash, uint32 offset,
			      size_t length)
{
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if (offset + length > spiflash->size) {
		DBG_MSG("erase area out of range of mtd.\n");
		return -EINVAL;
	}

	if (offset & (host->erasesize - 1)) {
		DBG_MSG("erase start address is not alignment.\n");
		return -EINVAL;
	}

	if (length & (host->erasesize - 1)) {
		DBG_MSG("erase length is not alignment.\n");
		return -EINVAL;
	}

	while (length) {
		if (spi->chipsize <= offset) {
			offset -= spi->chipsize;
			spi++;
			if (!spi->name)
				DBG_BUG("erase memory out of range.\n");
		}
		if (hisfc350_reg_erase_one_block(host, spi, offset))
			return -1;

		offset += spi->erase->size;
		length -= spi->erase->size;
	}

	return 0;
}
/*****************************************************************************/
static int hisfc350_map_chipsize(unsigned long long chipsize)
{
	int shift = 0;
	chipsize >>= (19 - 3); /* 19: 512K; 3: Bytes -> bit */

	while (chipsize) {
		chipsize >>= 1;
		shift++;
	}
	return shift;
}
/*****************************************************************************/

static int hisfc350_controller_spi_init(struct hisfc_spi *spi, int offset)
{
	int regval;
	struct hisfc_host *host = (struct hisfc_host *)spi->host;

	regval = hisfc_read(host, HISFC350_BUS_FLASH_SIZE);
	regval &= ~(HISFC350_BUS_FLASH_SIZE_CS0_MASK
		<< (spi->chipselect << 3));
	regval |= (hisfc350_map_chipsize(spi->chipsize)
		<< (spi->chipselect << 3));
	hisfc_write(host, HISFC350_BUS_FLASH_SIZE, regval);

	hisfc_write(host,
		(HISFC350_BUS_BASE_ADDR_CS0 + (spi->chipselect << 2)),
		(CONFIG_HISFC350_BUFFER_BASE_ADDRESS + offset));

	return 0;
}
/*****************************************************************************/

static void hisfc350_show_spi(struct hisfc_spi *spi)
{
	printf("Spi(cs%d): ", spi->chipselect);
	printf("Block:%sB ", u32tohstr(spi->erasesize, NULL));
	printf("Chip:%sB ", u64tohstr(spi->chipsize, NULL));
	printf("Name:\"%s\"\n", spi->name);

#ifdef CONFIG_HISFC350_SHOW_CYCLE_TIMING
	printf("Spi(cs%d): ", spi->chipselect);
	if (spi->addrcycle == 4)
		printf("4 addrcycle ");
	printf("read:%s,%02X,%s ",
		hisfc350_get_ifcycle_str(spi->read->iftype),
		spi->read->cmd,
		hisfc350_get_clock_str(spi->read->clock));
	printf("write:%s,%02X,%s ",
		hisfc350_get_ifcycle_str(spi->write->iftype),
		spi->write->cmd,
		hisfc350_get_clock_str(spi->write->clock));
	printf("erase:%s,%02X,%s\n",
		hisfc350_get_ifcycle_str(spi->erase->iftype),
		spi->erase->cmd,
		hisfc350_get_clock_str(spi->erase->clock));
#endif /* CONFIG_HISFC350_SHOW_CYCLE_TIMING */
}
/*****************************************************************************/
static int hisfc350_spi_probe(struct hisfc_host *host)
{
	unsigned int total = 0;
	unsigned char ids[8];
	struct spi_info *spiinfo;
	struct hisfc_spi *spi = host->spi;
	struct spi_info *first_spiinfo = NULL;
	struct hisfc_spi *first_spi = NULL; 
	int chipselect = CONFIG_HISFC350_CHIP_NUM - 1;

	host->num_chip = 0;

	for (; chipselect >= 0; chipselect--) {

		hisfc350_read_ids(host, chipselect, ids);

		/* can't find spi flash device. */
		if (!(ids[0] | ids[1] | ids[2])
			|| ((ids[0] & ids[1] & ids[2]) == 0xFF))
			continue;

		printf("\nSpi(cs%d) ID: 0x%02X 0x%02X 0x%02X"
			" 0x%02X 0x%02X 0x%02X\n",
			chipselect,
			ids[0], ids[1], ids[2], ids[3], ids[4], ids[5]);

		spiinfo = spi_serach_ids(hisfc350_spi_info_table, ids);

		if (!spiinfo) {
			printf("Spi(cs%d): find unrecognized spi flash.\n",
				chipselect);
			continue;
		}

		spi->name = spiinfo->name;
		spi->chipselect = chipselect;
		spi->chipsize   = spiinfo->chipsize;
		spi->erasesize  = spiinfo->erasesize;
		spi->addrcycle  = spiinfo->addrcycle;
		spi->driver     = spiinfo->driver;
		spi->host       = host;

		spi_search_rw(spiinfo, spi->read,
			HISFC350_SUPPORT_READ,
			HISFC350_SUPPORT_MAX_DUMMY, READ);

		spi_search_rw(spiinfo, spi->write,
			HISFC350_SUPPORT_WRITE,
			HISFC350_SUPPORT_MAX_DUMMY, WRITE);

		spi_get_erase(spiinfo, spi->erase);

		hisfc350_map_iftype_and_clock(spi);

		hisfc350_controller_spi_init(spi, total);

		spi->iobase = (char *)host->iobase + total;

		if (spi->addrcycle == 4)
			spi->driver->entry_4addr(spi, TRUE);

		hisfc350_show_spi(spi);

		if (chipselect == CONFIG_HISFC350_CHIP_NUM - 1) {
			first_spiinfo = spiinfo;
			first_spi = spi;
		}

		host->num_chip++;
		total += spi->chipsize;
		spi++;
	}

	if (first_spiinfo)
		set_spiflash_info(first_spi->chipsize, first_spi->erasesize,
				  host->num_chip, first_spi->name,
				  first_spiinfo->id, first_spiinfo->id_len);

	return host->num_chip;
}
/*****************************************************************************/

static void hisfc350_shutdown(void)
{
	int ix;
	struct hisfc_host *host = &hisfc_host;
	struct hisfc_spi *spi = host->spi;

	for (ix = 0; ix < host->num_chip; ix++, spi++) {
		spi->driver->wait_ready(spi);
		spi->driver->entry_4addr(spi, FALSE);
	}
}
/*****************************************************************************/
static int mcm(int m, int n)
{
	unsigned int total = m * n;
	unsigned int tt;
	if (m < n) {
		tt = m;
		m = n;
		n = tt;
	}

	while (n) {
		m = (m % n);
		if (m < n) {
			tt = m;
			m = n;
			n = tt;
		}
	}

	return (int)(total / m);
}
/*****************************************************************************/

static void hisfc_probe_spi_size(struct hisfc_host *host,
				 struct spiflash_dev_t *spiflash)
{
	int ix = 1;
	struct hisfc_spi *spi = host->spi;

	int total     = spi->chipsize;
	int erasesize = spi->erasesize;

	for (++spi; ix < host->num_chip; ix++, spi++) {
		erasesize = mcm(erasesize, spi->erasesize);
		total += spi->chipsize;
	}

	spiflash->size = total;
	host->erasesize = erasesize;
}
/*****************************************************************************/

static int hisfc350_probe(struct hisfc_host *host)
{
	struct spiflash_dev_t *spiflash = host->spiflash;

	host->set_system_clock(host, NULL, TRUE);
	hisfc_write(host, HISFC350_TIMING,
		HISFC350_TIMING_TCSS(0x6)
		| HISFC350_TIMING_TCSH(0x6)
		| HISFC350_TIMING_TSHSL(0xf));

	if (!hisfc350_spi_probe(host))
		return -1;

	spiflash->erase = hisfc350_reg_erase;
	spiflash->write = hisfc350_dma_write;
	spiflash->read  = hisfc350_dma_read;

	return 0;
}
/*****************************************************************************/

static int hisfc350_version_check(void)
{
	unsigned long regval = readl(CONFIG_HISFC350_REG_BASE_ADDRESS
		+ HISFC350_VERSION);

	return (regval == 0x350);
}
/*****************************************************************************/

static struct spiflash_dev_t *hisfc350_init(void)
{
	struct hisfc_host *host = &hisfc_host;
	if (__spiflash)
		return __spiflash;

	memset(host, 0, sizeof(struct hisfc_host));
	host->regbase = (void *)CONFIG_HISFC350_REG_BASE_ADDRESS;
	host->iobase  = (void *)CONFIG_HISFC350_BUFFER_BASE_ADDRESS;
	host->cfgreg  = (void *)CONFIG_HISFC350_PERIPHERY_REGBASE;


	hisfc350_set_system_clock(host, NULL, TRUE);

	printf("Check SPIflash controller version 350...");
	if (!hisfc350_version_check()) {
		printf("\n");
		return NULL;
	}
	printf("found\n");

	host->buffer = devmalloc(HISFC350_DMA_MAX_SIZE);
	if (!host->buffer) {
		printf("Can't malloc dma memory.\n");
		goto probe_fail;
	}

	host->dma_buffer = host->buffer;
	host->set_system_clock = hisfc350_set_system_clock;
	host->set_host_addr_mode = hisfc350_set_host_addr_mode;

	__spiflash = host->spiflash;

	__spiflash->name = "hi_sfc";

	if (hisfc350_probe(host)) {
		printf("Can't find a valid spi flash chip.\n");
		goto probe_fail;
	}
	hisfc_probe_spi_size(host, __spiflash);

	return __spiflash;

probe_fail:
	__spiflash = NULL;
	return NULL;
}

static void hisfc350_module_init(void *ptr)
{
	spiflash_driver_init = hisfc350_init;
}

static void hisfc350_module_reboot(void *ptr)
{
	if (__spiflash)
		hisfc350_shutdown();
}

static struct module_t module_hisfc350 = {
	.priority = MODULE_PRI_DRV,
	.init = hisfc350_module_init,
	.reboot = hisfc350_module_reboot,
};
MODULE(module_hisfc350);
