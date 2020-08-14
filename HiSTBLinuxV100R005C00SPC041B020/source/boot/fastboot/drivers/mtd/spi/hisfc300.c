/******************************************************************************
*    Copyright (c) 2009-2010 by HiC.
*    All rights reserved.
* ***
*    Create by HiC. 2010-12-15
*
******************************************************************************/

/*****************************************************************************/
#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>
#include <linux/mtd/mtd.h>

#include "spi_ids.h"
#include "hisfc300.h"

/*****************************************************************************/

#define HISFC200_SUPPORT_READ             (SPI_IF_READ_STD \
	| SPI_IF_READ_FAST \
	| SPI_IF_READ_DUAL)

#define HISFC200_SUPPORT_MAX_DUMMY        (1)
#define HISFC200_SUPPORT_WRITE            (SPI_IF_WRITE_STD)

/*****************************************************************************/
static struct hisfc_host  hisfc300_host;
static struct mtd_info_ex spi_info_ex = {.type = 0, };

extern struct spi_info hisfc300_spi_info_table[];
/*****************************************************************************/
#ifdef CONFIG_ARCH_GODBOX
#  include "hisfc300_godbox.c"
#endif

#ifdef CONFIG_ARCH_GODBOX_V1
#  include "hisfc300_godbox.c"
#endif

#ifdef CONFIG_ARCH_GODCUBE
#  include "hisfc300_godcube.c"
#endif

#ifdef CONFIG_ARCH_GODEYES
#  include "hisfc300_godeyes.c"
#endif

#ifdef CONFIG_ARCH_GODNET 
#  include "hisfc300_godnet.c"
#endif
/*****************************************************************************/

static int hisfc200_wait_ready(struct hisfc_host *host, struct hisfc_spi *spi)
{
	unsigned long regval;
	unsigned long deadline = 0;

	do 
	{
		hisfc_write(host, HISFC200_INSTR, SPI_CMD_RDSR);
		hisfc_write(host, HISFC200_CMD,
			HISFC200_CMD_CS(spi->chipselect)
			| HISFC200_CMD_DATA_CNT(1)
			| HISFC200_CMD_RDDATA_EN
			| HISFC200_CMD_START);

		while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
			;

		regval = hisfc_read(host, HISFC200_DATABUFFER1);
		if (!(regval & SPI_CMD_SR_WIP))
			return 0;

		udelay(1);

	} while (deadline++ < (40 << 20));

	printf("Wait spi ready timeout.\n");

	return 1;
}

/*****************************************************************************/

static void hisfc200_enable_4byte_mode(struct hisfc_host *host,
	struct hisfc_spi *spi, int enable)
{
	if (spi->addrcycle != 4 && enable)
	{
		DBG_BUG("spi->addrcycle(%d) != 4\n", spi->addrcycle);
		return;
	}

	if (enable)
		hisfc_write(host, HISFC200_INSTR, SPI_CMD_EN4B);
	else
		hisfc_write(host, HISFC200_INSTR, SPI_CMD_EX4B);

	hisfc_write(host, HISFC200_CMD,
		HISFC200_CMD_CS(spi->chipselect) 
		| HISFC200_CMD_START);

	while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
		;
}
/*****************************************************************************/

static void hisfc200_config(struct hisfc_host *host, struct hisfc_spi *spi,
	struct spi_operation *op, int clk_en, int writecmd, int readcmd)
{
	unsigned int regval = 0;
	if (spi && op)
	{
		regval = HISFC200_CONFIG_IF_TYPE(op->iftype) 
			| HISFC200_CONFIG_DUMMY_BYTE(op->dummy)
			| HISFC200_CONFIG_WIP_LOCATE(0)
			| (spi->addrcycle == 4 ? HISFC200_CONFIG_ADDR_MODE_4B : 0)
			| HISFC200_CONFIG_RD_DELAY(0)
			| HISFC200_CONFIG_PREFETCH_CNT(0)
			| HISFC200_CONFIG_RD_INS(readcmd)
			| HISFC200_CONFIG_WR_INS(writecmd);
	}
	hisfc_write(host, HISFC200_CONFIG, regval);
	hisfc200_set_system_clock(host,op,clk_en);
}
/*****************************************************************************/

static void hisfc200_map_iftype_and_clock(struct hisfc_spi *spi)
{
	int ix, jx;
	int iftype_read[] = {
		SPI_IF_READ_STD,       0, 
		SPI_IF_READ_FAST,      0,
		SPI_IF_READ_DUAL,      4,
		SPI_IF_READ_QUAD,      6,
		SPI_IF_READ_QUAD_ADDR, 7,
		0,0,
	};
	
	for (ix = 0; iftype_read[ix]; ix += 2)
	{
		if (spi->read->iftype == iftype_read[ix])
		{
			spi->read->iftype = iftype_read[ix + 1];
			break;
		}
	}

	/* std write */
	spi->write->iftype = 0;

	hisfc200_get_best_clock(&spi->read->clock);
	hisfc200_get_best_clock(&spi->write->clock);

	DBG_MSG("map: spi->read->iftype:0x%08X\n",  spi->read->iftype);
	DBG_MSG("map: spi->read->clock:0x%08X\n",   spi->read->clock);
	DBG_MSG("map: spi->write->iftype:0x%08X\n", spi->write->iftype);
	DBG_MSG("map: spi->write->clock:0x%08X\n",  spi->write->clock);

	for (jx = 0; (jx < MAX_SPI_OP) && spi->erase[jx].iftype; jx++)
	{
		hisfc200_get_best_clock(&(spi->erase[jx].clock));
		spi->erase[jx].iftype = 0;
		DBG_MSG("map: spi->erase[%d].clock:0x%08X size:%s\n",
			jx, spi->erase[jx].clock, ultohstr(spi->erase[jx].size));
	}
}
/*****************************************************************************/
/* 
 * read data used bus mode 
 * in fastboot, return 0 success. other fail. 
 */
static int hisfc200_bus_read(struct spi_flash *spiflash, u32 from, size_t len,
	void *buf)
{
	int num;
	int result = -EIO;
	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((from + len) > spiflash->size)
	{
		DBG_MSG("read area out of range.\n");
		return -EINVAL;
	}

	if (!len)
	{
		DBG_MSG("read length is 0.\n");
		return 0;
	}

	if (hisfc200_wait_ready(host, spi))
		goto fail;
	hisfc200_config(host, spi, spi->read, 1, 0, spi->read->cmd);

	while (len > 0)
	{
		while (from >= spi->chipsize)
		{
			from -= spi->chipsize;
			spi++;
			if (!spi->name)
			{
				DBG_BUG("read memory out of range.\n");
			}
			if (hisfc200_wait_ready(host, spi))
				goto fail;
			hisfc200_config(host, spi, spi->read, 1, 0, spi->read->cmd);
		}

		num = ((from + len) >= spi->chipsize) ? (spi->chipsize - from) : len;
		while (num >= HISFC200_MEM_READ_SLICE)
		{
			memcpy(ptr, (char *)spi->iobase + from, HISFC200_MEM_READ_SLICE);

			ptr  += HISFC200_MEM_READ_SLICE;
			from += HISFC200_MEM_READ_SLICE;
			len  -= HISFC200_MEM_READ_SLICE;
			num  -= HISFC200_MEM_READ_SLICE;
		}

		if (num)
		{
			memcpy(ptr, (char *)spi->iobase + from, num);
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

static unsigned char *hisfc200_read_ids(struct hisfc_host *host, int chipselect,
	unsigned char *buffer, int numread)
{
	int regindex = 0;
	unsigned long *ptr = (unsigned long *)buffer;

	if (numread > HISFC200_REG_BUF_SIZE)
		numread = HISFC200_REG_BUF_SIZE;

	hisfc_write(host, HISFC200_INSTR, SPI_CMD_RDID);
	hisfc_write(host, HISFC200_CMD,
		HISFC200_CMD_CS(chipselect)
		| HISFC200_CMD_RDDATA_EN
		| HISFC200_CMD_DATA_CNT(numread)
		| HISFC200_CMD_START);

	while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
		;

	numread = ((numread + 0x03) & (~0x03));
	while (numread)
	{
		*ptr = hisfc_read(host, HISFC200_DATABUFFER1 + regindex);
		ptr      += 1;
		regindex += 4;
		numread  -= 4;
	}

	return buffer;
}
/*****************************************************************************/
/* enable spi write. */
static int hisfc200_write_enable(struct hisfc_host *host, struct hisfc_spi *spi)
{
	hisfc_write(host, HISFC200_INSTR, SPI_CMD_WREN);
	
	hisfc_write(host, HISFC200_CMD,
		(HISFC200_CMD_CS(spi->chipselect) | HISFC200_CMD_START));

	while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
		;

	return 0;
}
/*****************************************************************************/
/* erase one block */
static int hisfc200_reg_erase_one_block(struct hisfc_host *host, struct hisfc_spi *spi,
	unsigned long offset, int cmdindex)
{
	if (hisfc200_wait_ready(host, spi))
		return 1;

	hisfc200_config(host, spi, &spi->erase[cmdindex], 1, 0, 0);

	hisfc200_write_enable(host, spi);

	hisfc_write(host, HISFC200_INSTR, spi->erase[cmdindex].cmd);

	hisfc_write(host, HISFC200_ADDR, 
		(offset & HISFC200_ADDR_MASK));

	hisfc_write(host, HISFC200_CMD,
		HISFC200_CMD_CS(spi->chipselect) 
		| HISFC200_CMD_DUMMY_CNT(spi->erase[cmdindex].dummy)
		| HISFC200_CMD_ADDR_EN
		| HISFC200_CMD_START);

	while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
		;

	return 0;
}
/*****************************************************************************/

static int hisfc200_reg_write_one_buffer(struct hisfc_host *host, struct hisfc_spi *spi,
	unsigned int offset, const unsigned char *buf, int count)
{
	int num = 0;
	int regindex = 0;
	int num_write = count;
	unsigned int regval;

	if (hisfc200_wait_ready(host, spi))
		return 1;

	hisfc200_write_enable(host, spi);

	hisfc_write(host, HISFC200_INSTR, spi->write->cmd);

	hisfc_write(host, HISFC200_ADDR, 
		(offset & HISFC200_ADDR_MASK));

	while (count > 0)
	{
		num = ((count > 4) ? 4 : count);
		memcpy(&regval, buf, num);
		hisfc_write(host, (HISFC200_DATABUFFER1 + regindex), regval);

		buf      += num;
		regindex += 4;
		count    -= num;
	}

	hisfc_write(host, HISFC200_CMD,
		HISFC200_CMD_CS(spi->chipselect) 
		| HISFC200_CMD_DUMMY_CNT(spi->write->dummy)
		| HISFC200_CMD_DATA_CNT(num_write)
		| HISFC200_CMD_ADDR_EN
		| HISFC200_CMD_WRDATA_EN
		| HISFC200_CMD_START);

	while ((hisfc_read(host, HISFC200_CMD) & HISFC200_CMD_START))
		;

	return 0;
}
/*****************************************************************************/

static int hisfc200_reg_write(struct spi_flash *spiflash, u32 to, size_t len,
	const void *buf)
{
	int num;
	int result = -EIO;

	unsigned char *ptr = (unsigned char *)buf;
	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if ((to + len) > spiflash->size)
	{
		DBG_MSG("write data out of range.\n");
		return -EINVAL;
	}

	if (!len)
	{
		DBG_MSG("write length is 0.\n");
		return 0;
	}

	if (hisfc200_wait_ready(host, spi))
		goto fail;
	hisfc200_config(host, spi, spi->write, 1, 0, 0);

	if (to & HISFC200_REG_BUF_MASK)
	{
		if ((num = HISFC200_REG_BUF_SIZE - (to & HISFC200_REG_BUF_MASK)) > (int)len)
		{
			num = (int)len;
		}
		while (to >= spi->chipsize)
		{
			to -= spi->chipsize;
			spi++;
			if (!spi->name)
			{
				DBG_BUG("write memory out of range.\n");
			}
			if (hisfc200_wait_ready(host, spi))
				goto fail;
			hisfc200_config(host, spi, spi->write, 1, 0, 0);
		}
		if (hisfc200_reg_write_one_buffer(host, spi, to, ptr, num))
			goto fail;
		to  += num;
		ptr += num;
		len -= num;
	}

	while (len > 0)
	{
		num = ((len >= HISFC200_REG_BUF_SIZE) ? HISFC200_REG_BUF_SIZE : len);
		while (to >= spi->chipsize)
		{
			to -= spi->chipsize;
			spi++;
			if (!spi->name)
			{
				DBG_BUG("write memory out of range.\n");
			}
			if (hisfc200_wait_ready(host, spi))
				goto fail;
			hisfc200_config(host, spi, spi->write, 1, 0, 0);
		}
		if (hisfc200_reg_write_one_buffer(host, spi, to, ptr, num))
			goto fail;
		to  += num;
		ptr += num;
		len -= num;
	}
	result = 0;
fail:
	return result;
}
/*****************************************************************************/

static int hisfc200_reg_erase(struct spi_flash *spiflash, u32 offset, size_t length)
{
	int cmdindex;

	struct hisfc_host *host = SPIFLASH_TO_HOST(spiflash);
	struct hisfc_spi *spi = host->spi;

	if (offset + length > spiflash->size)
	{
		DBG_MSG("erase area out of range of mtd.\n");
		return -EINVAL;
	}

	if (offset & (host->erasesize-1))
	{
		DBG_MSG("erase start address is not alignment.\n");
		return -EINVAL;
	}

	if (length & (host->erasesize-1))
	{
		DBG_MSG("erase length is not alignment.\n");
		return -EINVAL;
	}

	while (length)
	{
		if (spi->chipsize <= offset)
		{
			offset -= (unsigned int)spi->chipsize;
			spi++;
			if (!spi->name)
			{
				DBG_BUG("erase memory out of range.\n");
			}
		}

		for (cmdindex = 0; cmdindex < MAX_SPI_OP; cmdindex++)
		{
			if ((spi->chipsize - offset) >= spi->erase[cmdindex].size
				&& length >= spi->erase[cmdindex].size)
				break;
		}

		if (hisfc200_reg_erase_one_block(host, spi, (unsigned long)offset, cmdindex))
		{
			return -1;
		}

		offset += spi->erase[cmdindex].size;
		length -= spi->erase[cmdindex].size;
	}

	return 0;
}
/*****************************************************************************/

static int hisfc200_map_chipsize(unsigned long long chipsize)
{
	int shift = 0;
	chipsize >>= (19 - 3); /* 19: 512K; 3: Bytes -> bit */

	while (chipsize)
	{
		chipsize >>= 1;
		shift++;
	}
	return shift;
}
/*****************************************************************************/

static int hisfc200_spi_probe(struct hisfc_host *host)
{
	int chipselect;
	unsigned int total = 0;
	unsigned char ids[8];
	struct spi_info *spiinfo;
	struct hisfc_spi *spi = host->spi;

	host->num_chip = 0;
	for (chipselect = (CONFIG_HISFC300_CHIP_NUM - 1);
		chipselect >= 0; chipselect--)
	{
		hisfc200_read_ids(host, chipselect, ids, 8);

		if (!(ids[0] | ids[1] | ids[2]) || ((ids[0] & ids[1] & ids[2]) == 0xFF))
			continue;

		printf("Spi(cs%d) ID: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
			chipselect,
			ids[0], ids[1], ids[2], ids[3], ids[4], ids[5]);

		spiinfo = spi_serach_ids(hisfc300_spi_info_table, ids);
		if (spiinfo)
		{
			spi->name = spiinfo->name;
			spi->chipselect = chipselect;
			spi->chipsize   = spiinfo->chipsize;
			spi->erasesize  = spiinfo->erasesize;
			spi->addrcycle  = spiinfo->addrcycle;

			spi_search_rw(spiinfo, spi->read, HISFC200_SUPPORT_READ,
				HISFC200_SUPPORT_MAX_DUMMY, 1);

			spi_search_rw(spiinfo, spi->write, HISFC200_SUPPORT_WRITE,
				HISFC200_SUPPORT_MAX_DUMMY, 0);

			DBG_MSG("spi->read->iftype:0x%02X, cmd:0x%02X, clock:0x%02X\n",
				spi->read->iftype, spi->read->cmd, spi->read->clock);
			DBG_MSG("spi->write->iftype:0x%02X, cmd:0x%02X\n, clock:0x%02X",
				spi->write->iftype, spi->write->cmd, spi->write->clock);

			spi_get_erase_sfcv300(spiinfo, spi->erase, &spi->erasesize);
			hisfc200_map_iftype_and_clock(spi);

			/* set chip select.*/
			hisfc_write(host, (HISFC200_CS0CONFIG + (chipselect << 2)),
				HISFC200_CSxCONFIG_CHIPSIZE(hisfc200_map_chipsize(spi->chipsize)));

			/* set memory map address. */
			hisfc_write(host, (HISFC200_CS0BASEADDR + (chipselect << 2)),
				(CONFIG_HISFC300_BUFFER_BASE_ADDRESS + total));

			DBG_MSG("0x%08X: 0x%08X\n", 
				(HISFC200_CS0CONFIG + (chipselect << 2)),
				hisfc_read(host, (HISFC200_CS0CONFIG + (chipselect << 2))));
			DBG_MSG("0x%08X: 0x%08X\n", 
				(HISFC200_CS0BASEADDR + (chipselect << 2)),
				hisfc_read(host, (HISFC200_CS0BASEADDR + (chipselect << 2))));

			spi->iobase = (char *)host->iobase + total;

			if (spi->addrcycle == 4)
			{
				DBG_MSG("entry 4 bytes modes.\n");
				hisfc200_enable_4byte_mode(host, spi, 1);
			}

			printf("Spi(cs%d): ", spi->chipselect);
			printf("Block:%sB ",  ultohstr(spi->erasesize));
			printf("Chip:%sB ",   ultohstr(spi->chipsize));
			printf("Name:\"%s\"\n", spi->name);

			if (spi_info_ex.type == 0)
			{
				spi_info_ex.type      = MTD_NORFLASH;
				spi_info_ex.chipsize  = spi->chipsize;
				spi_info_ex.erasesize = spi->erasesize;
				spi_info_ex.pagesize  = 1;
				spi_info_ex.addrcycle = spi->addrcycle;

				if (spiinfo->id_len > sizeof(spi_info_ex.ids))
				{
					printf("BUG!!! id length out of range.\n");
					BUG ();
				}
				
				spi_info_ex.id_length = spiinfo->id_len;
				memcpy(spi_info_ex.ids, spiinfo->id, spiinfo->id_len);

				strncpy(spi_info_ex.name, spi->name, sizeof(spi_info_ex.name));
				spi_info_ex.name[sizeof(spi_info_ex.name)-1] = '\0';
			}

			host->num_chip++;
			total += spi->chipsize;
			spi++;
		}
		else
		{
			printf("Spi(cs%d): find unrecognized spi flash.\n", chipselect);
		}
	}
	spi_info_ex.numchips = host->num_chip;

	return host->num_chip;
}
/*****************************************************************************/

static int hisfc200_probe(struct hisfc_host *host)
{
	struct spi_flash *spiflash = host->spiflash;

	hisfc_write(host, HISFC200_TIMING,
		HISFC200_TIMING_TCSS(0)
		| HISFC200_TIMING_TCSH(0)
		| HISFC200_TIMING_TRPD(0)
		| HISFC200_TIMING_TSHSL(4)
		| HISFC200_TIMING_TSHWL(4));

	if (!hisfc200_spi_probe(host))
		return -1;

	spiflash->erase = hisfc200_reg_erase;
	spiflash->write = hisfc200_reg_write;
	spiflash->read  = hisfc200_bus_read;
	return 0;
}
/*****************************************************************************/

static void hisfc200_shutdown(void)
{
	int ix;
	struct hisfc_host *host = &hisfc300_host;
	struct hisfc_spi *spi = host->spi;

	for (ix = 0; ix < host->num_chip; ix++, spi++) {
		hisfc200_wait_ready(host, spi);
		if (spi->addrcycle == 4)
			hisfc200_enable_4byte_mode(host, spi, 0);
	}
}
/*****************************************************************************/

static int mcm(int m, int n)
{
	unsigned int total = m * n;
	unsigned int tt;
	if (m < n)
	{
		tt = m; m = n; n = tt;
	}

	while (n)
	{
		m = (m % n);
		if (m < n)
		{
			tt = m; m = n; n = tt;
		}
	}

	return (int)(total / m);
}
/*****************************************************************************/

static void hisfc300_probe_spi_size(struct hisfc_host *host,
	struct spi_flash *spiflash)
{
	int ix = 1;
	struct hisfc_spi *spi = host->spi;

	int total     = spi->chipsize;
	int erasesize = spi->erasesize;

	for (++spi; ix < host->num_chip; ix++, spi++)
	{
		erasesize = mcm(erasesize, spi->erasesize);
		total += spi->chipsize;
	}

	spiflash->size  = total;
	host->erasesize = erasesize;
}
/*****************************************************************************/

static int hisfc300_version_check(void)
{
	unsigned long regval = readl(CONFIG_HISFC300_REG_BASE_ADDRESS
		+ HISFC300_VERSION);
	return ((regval == 0x300) || (regval == 0x000));
}
/*****************************************************************************/

static struct spi_flash * hisfc300_init(void)
{
	unsigned int regval;
	struct hisfc_host *host = &hisfc300_host;
	static struct spi_flash *spiflash = NULL;

	if (spiflash)
		return spiflash;

	memset(host, 0, sizeof(struct hisfc_host));
	memset(&spi_info_ex, 0, sizeof(struct mtd_info_ex));

	host->regbase = (void *)CONFIG_HISFC300_REG_BASE_ADDRESS;
	host->iobase  = (void *)CONFIG_HISFC300_BUFFER_BASE_ADDRESS;
	host->cfgreg  = (void *)CONFIG_HISFC300_PERIPHERY_REGBASE;

	spiflash = host->spiflash;

	spiflash->name = "hi_sfc";

	hisfc200_config(host, 0, 0, 1, 0, 0);
	regval = hisfc_read(host, HISFC300_VERSION);

	switch (regval)
	{
	case 0x000:
	case 0x300:
		if (hisfc200_probe(host))
		{
			printf("Can't find a valid spi flash chip.\n");
			goto probe_fail;
		}
		break;
	default:
		printf("Can't find spi controller version.\n");
		goto probe_fail;
	}

	hisfc300_probe_spi_size(host, spiflash);

	add_shutdown(hisfc200_shutdown);

	return spiflash;

probe_fail:
	spiflash = NULL;
	return NULL;
}
/*****************************************************************************/

static struct mtd_info_ex * hisfc300_getinfo(void)
{
	if (spi_info_ex.type == 0)
	{
		if (hisfc300_init() == NULL)
			return NULL;
	}
	return &spi_info_ex;
}
/*****************************************************************************/

int hisfc300_spiflash_init(struct spi_flash ** spiflash,
	struct mtd_info_ex ** spiinfo)
{
	printf("Check spi flash controller v300. ");
	if (!hisfc300_version_check()) {
		printf("\n");
		return 0;
	}
	printf("found\n");

	(*spiflash) = hisfc300_init();
	(*spiinfo) = hisfc300_getinfo();

	return 1;
}
