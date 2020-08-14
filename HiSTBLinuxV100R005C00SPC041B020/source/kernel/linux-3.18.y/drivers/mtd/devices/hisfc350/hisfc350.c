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

#define DRVNAME    "hi_sfc"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/hikapi.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/mtd/mtd.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <asm/io.h>

#include "hisfc_ids.h"

#include "hisfc350.h"

extern struct spi_info hisfc350_spi_info_table[];

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

/*****************************************************************************/

static void hisfc350_set_host_4addr_mode(struct hisfc_host *host, int enable)
{
	u32 regval;

	regval = hisfc_read(host, HISFC350_GLOBAL_CONFIG);
	if (enable)
		regval |= HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B;
	else
		regval &= ~HISFC350_GLOBAL_CONFIG_ADDR_MODE_4B;

	hisfc_write(host, HISFC350_GLOBAL_CONFIG, regval);
}
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

	for (ix = 0; iftype_read[ix]; ix += 2) {
		if (spi->read->iftype == iftype_read[ix]) {
			spi->read->iftype = iftype_read[ix + 1];
			break;
		}
	}

	spi->erase->iftype = HISFC350_IFCYCLE_STD;
}
/*****************************************************************************/

static void hisfc350_dma_transfer(struct hisfc_host *host, u32 spi_start_addr,
				  u32 dma_phyaddr, unsigned char is_read,
				  u32 size, unsigned char chipselect)
{
	hisfc_write(host, HISFC350_BUS_DMA_MEM_SADDR, dma_phyaddr);

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

static int __hisfc350_dma_read(struct hisfc_host *host, uint64_t partsize,
			       u32 from, u32 len, u32 *retlen, u_char *buf)
{
	int num;
	int result = -EIO;
	u_char *ptr = buf;
	struct hisfc_spi *spi = host->spi;

	if ((u64)(from + len) > partsize) {
		pr_warn("read area out of range.\n");
		return -EINVAL;
	}

	*retlen = 0;
	if (!len) {
		pr_warn("read length is 0.\n");
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
				panic("%s: read out of size\n", __func__);

			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->bus_prepare(spi, READ);
		}
		hisfc350_dma_transfer(host, from, (u32)host->dma_phyaddr, READ,
			num, spi->chipselect);
		memcpy(ptr, host->dma_viraddr, num);
		from  += num;
		ptr += num;
		len -= num;
	}

	while (len > 0) {
		while (from >= spi->chipsize) {
			from -= spi->chipsize;
			spi++;
			if (!spi->name)
				panic("%s: read out of size\n", __func__);

			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->bus_prepare(spi, READ);
		}

		num = ((from + len) >= spi->chipsize)
			? (spi->chipsize - from) : len;
		while (num >= HISFC350_DMA_MAX_SIZE) {
			hisfc350_dma_transfer(host, from,
				(u32)host->dma_phyaddr, READ,
				HISFC350_DMA_MAX_SIZE, spi->chipselect);
			memcpy(ptr, host->dma_viraddr, HISFC350_DMA_MAX_SIZE);
			ptr  += HISFC350_DMA_MAX_SIZE;
			from += HISFC350_DMA_MAX_SIZE;
			len  -= HISFC350_DMA_MAX_SIZE;
			num  -= HISFC350_DMA_MAX_SIZE;
		}

		if (num) {
			hisfc350_dma_transfer(host, from,
				(u32)host->dma_phyaddr, READ,
				num, spi->chipselect);
			memcpy(ptr, host->dma_viraddr, num);
			from += num;
			ptr  += num;
			len  -= num;
		}
	}
	result = 0;
	*retlen = (u32)(ptr - buf);

fail:
	return result;
}
/*****************************************************************************/

static u_char *hisfc350_read_ids(struct hisfc_host *host, int chipselect,
				 unsigned char *buffer)
{
	int regindex = 0;
	int numread = 8;
	unsigned int *ptr = (u32 *)buffer;

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

	hisfc350_set_rate(host, spi->erase);

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

static int __hisfc350_dma_write(struct hisfc_host *host, uint64_t partsize,
				u32 to, u32 len, u32 *retlen, u_char *buf)
{
	int num;
	int result = -EIO;
	u_char *ptr = buf;
	struct hisfc_spi *spi = host->spi;

	if ((uint64_t)(to + len) > partsize) {
		pr_warn("write data out of range.\n");
		return -EINVAL;
	}

	*retlen = 0;
	if (!len) {
		pr_warn("write length is 0.\n");
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
				panic("%s: write out of size\n", __func__);

			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->write_enable(spi);
			spi->driver->bus_prepare(spi, WRITE);
		}
		memcpy(host->dma_viraddr, ptr, num);
		hisfc350_dma_transfer(host, to, (u32)host->dma_phyaddr, WRITE,
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
				panic("%s: write out of size\n", __func__);

			if (spi->driver->wait_ready(spi))
				goto fail;
			spi->driver->write_enable(spi);
			spi->driver->bus_prepare(spi, WRITE);
		}

		memcpy(host->dma_viraddr, ptr, num);
		hisfc350_dma_transfer(host, to, (u32)host->dma_phyaddr, WRITE,
			num, spi->chipselect);

		to  += num;
		ptr += num;
		len -= num;
	}
	*retlen = (unsigned int)(ptr - buf);
	result = 0;
fail:
	return result;
}
/*****************************************************************************/

static int __hisfc350_reg_erase(struct hisfc_host *host, uint64_t prt_size,
				uint64_t offset, uint64_t length, int *state)
{
	struct hisfc_spi *spi = host->spi;

	if (offset + length > prt_size) {
		pr_warn("erase area out of range of mtd.\n");
		return -EINVAL;
	}

	if ((unsigned int)offset & (host->erasesize-1)) {
		pr_warn("erase start address is not alignment.\n");
		return -EINVAL;
	}

	if ((unsigned int)length & (host->erasesize-1)) {
		pr_warn("erase length is not alignment.\n");
		return -EINVAL;
	}

	while (length) {
		if (spi->chipsize <= offset) {
			offset -= spi->chipsize;
			spi++;
			if (!spi->name)
				panic("%s: erase out of size\n", __func__);
		}
		if (hisfc350_reg_erase_one_block(host, spi, offset))
			break;

		offset += spi->erase->size;
		length -= spi->erase->size;
	}

	*state = MTD_ERASE_DONE;

	return 0;
}
/*****************************************************************************/

static int hisfc350_map_chipsize(u64 chipsize)
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

static int hisfc350_ctrl_spi_init(struct hisfc_spi *spi, int spisize)
{
	u32 regval;
	struct hisfc_host *host = (struct hisfc_host *)spi->host;

	regval = hisfc_read(host, HISFC350_BUS_FLASH_SIZE);
	regval &= ~(HISFC350_BUS_FLASH_SIZE_CS0_MASK
		<< (spi->chipselect << 3));
	regval |= (hisfc350_map_chipsize(spi->chipsize)
		<< (spi->chipselect << 3));
	hisfc_write(host, HISFC350_BUS_FLASH_SIZE, regval);

	hisfc_write(host,
		(HISFC350_BUS_BASE_ADDR_CS0 + (spi->chipselect << 2)),
		(host->iobase_phyaddr + spisize));

	return 0;
}
/*****************************************************************************/

static void hisfc350_show_spi(struct hisfc_spi *spi)
{
#define MAX_PRINT_BUFFER          1024
	char tmp[20];
	char *ptr, *str;
	int size, num;

	str = (char *)kmalloc(MAX_PRINT_BUFFER, GFP_KERNEL);
	if (!str) {
		pr_err("Can't malloc memory.\n");
		return;
	}

	ptr = str;
	size = MAX_PRINT_BUFFER;

	num = snprintf(ptr, size, "Spi(cs%d): ", spi->chipselect);
	ptr += num;
	size -= num;

	ultohstr((u64)spi->erasesize, tmp, sizeof(tmp));
	num = snprintf(ptr, size, "Block:%s ", tmp);
	ptr += num;
	size -= num;

	ultohstr(spi->chipsize, tmp, sizeof(tmp));
	num = snprintf(ptr, size, "Chip:%s ", tmp);
	ptr += num;
	size -= num;

	num = snprintf(ptr, size, "Name:\"%s\"\n", spi->name);

	pr_info("%s", str);

	kfree(str);

#undef MAX_PRINT_BUFFER
}
/*****************************************************************************/

static int hisfc350_spi_probe(struct hisfc_host *host)
{
	unsigned int total = 0;
	unsigned char ids[8];
	struct spi_info *spiinfo;
	struct hisfc_spi *spi = host->spi;
	int chipselect = HISFC350_MAX_CHIP - 1;

	host->num_chip = 0;
	for (; chipselect >= 0; chipselect--) {

		hisfc350_read_ids(host, chipselect, ids);

		/* can't find spi flash device. */
		if (!(ids[0] | ids[1] | ids[2])
			|| ((ids[0] & ids[1] & ids[2]) == 0xFF))
			continue;

		pr_info("Spi(cs%d) ID: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
			chipselect,
			ids[0], ids[1], ids[2], ids[3], ids[4], ids[5]);

		spiinfo = spi_serach_ids(hisfc350_spi_info_table, ids);

		if (!spiinfo) {
			pr_warn("find unrecognized spi flash.\n");
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

		/* switch unit M */
		spi->read->clock *= 1000000;
		spi->write->clock *= 1000000;
		spi->erase->clock *= 1000000;

		hisfc350_map_iftype_and_clock(spi);

		hisfc350_ctrl_spi_init(spi, total);

		spi->iobase = (char *)host->iobase + total;

		if (spi->addrcycle == 4)
			spi->driver->entry_4addr(spi, TRUE);

		hisfc350_show_spi(spi);

		host->num_chip++;
		total += spi->chipsize;
		spi++;
	}
	return host->num_chip;
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

static void hisfc350_probe_spi_size(struct hisfc_host *host)
{
	int ix = 1;
	struct hisfc_spi *spi = host->spi;
	u64 total = spi->chipsize;
	u32 erasesize = spi->erasesize;

	for (++spi; ix < host->num_chip; ix++, spi++) {
		erasesize = mcm(erasesize, spi->erasesize);
		total += spi->chipsize;
	}

	host->chipsize = total;
	host->erasesize = erasesize;
}
/*****************************************************************************/

static void hisfc350_driver_shutdown(struct platform_device *pdev)
{
	int ix;
	struct hisfc_host *host = platform_get_drvdata(pdev);
	struct hisfc_spi *spi = host->spi;

	for (ix = 0; ix < host->num_chip; ix++, spi++) {
		spi->driver->wait_ready(spi);
		if (spi->addrcycle == 4)
			spi->driver->entry_4addr(spi, FALSE);
	}
}
/*****************************************************************************/

static int hisfc350_probe(struct hisfc_host *host)
{
	host->set_host_4addr_mode = hisfc350_set_host_4addr_mode;

	clk_enable(host->clk);

	hisfc_write(host, HISFC350_TIMING,
		HISFC350_TIMING_TCSS(0x6)
		| HISFC350_TIMING_TCSH(0x6)
		| HISFC350_TIMING_TSHSL(0xf));

	if (!hisfc350_spi_probe(host))
		return -1;

	hisfc350_probe_spi_size(host);

	return 0;
}
/*****************************************************************************/

static int hisfc350_reg_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	int ret;
	int state;
	struct hisfc_host *host = MTD_TO_HOST(mtd);

	mutex_lock(&host->lock);

	ret = __hisfc350_reg_erase(host, mtd->size, instr->addr,
		instr->len, &state);

	mutex_unlock(&host->lock);

	instr->state = (u_char)state;

	mtd_erase_callback(instr);

	return ret;
}
/*****************************************************************************/

static int hisfc350_dma_write(struct mtd_info *mtd, loff_t to, size_t len,
			      size_t *retlen, const u_char *buf)
{
	int ret;
	struct hisfc_host *host = MTD_TO_HOST(mtd);

	mutex_lock(&host->lock);

	ret = __hisfc350_dma_write(host, mtd->size, (u32)to, (u32)len,
		(u32*)retlen, (u_char *)buf);

	mutex_unlock(&host->lock);

	return ret;
}
/*****************************************************************************/

static int hisfc350_dma_read(struct mtd_info *mtd, loff_t from, size_t len,
			     size_t *retlen, u_char *buf)
{
	int ret;
	struct hisfc_host *host = MTD_TO_HOST(mtd);

	mutex_lock(&host->lock);

	ret = __hisfc350_dma_read(host, mtd->size, (unsigned int)from,
		(unsigned int)len, (unsigned int *)retlen, buf);

	mutex_unlock(&host->lock);

	return ret;
}
/*****************************************************************************/
#ifdef CONFIG_PM
static int hisfc350_driver_suspend(struct platform_device *pltdev,
				   pm_message_t state)
{
	int ix;
	struct hisfc_host *host = platform_get_drvdata(pltdev);
	struct hisfc_spi *spi = host->spi;

	for (ix = 0; ix < host->num_chip; ix++, spi++) {
		spi->driver->wait_ready(spi);
		if (spi->addrcycle == 4)
			spi->driver->entry_4addr(spi, FALSE);
	}

	clk_disable(host->clk);

	return 0;
}
/*****************************************************************************/

static int hisfc350_driver_resume(struct platform_device *pltdev)
{
	int ix;
	struct hisfc_host *host = platform_get_drvdata(pltdev);
	struct hisfc_spi *spi = host->spi;

	clk_enable(host->clk);

	hisfc_write(host, HISFC350_TIMING,
		HISFC350_TIMING_TCSS(0x6)
		| HISFC350_TIMING_TCSH(0x6)
		| HISFC350_TIMING_TSHSL(0xf));

	for (ix = 0; ix < host->num_chip; spi++, ix++) {
		hisfc350_ctrl_spi_init(spi,
			(u32)spi->iobase - (u32)host->iobase);

		if (spi->addrcycle == 4)
			spi->driver->entry_4addr(spi, TRUE);
	}
	return 0;
}
#else
#  define hisfc350_driver_suspend NULL
#  define hisfc350_driver_resume  NULL
#endif /* CONFIG_PM */
/*****************************************************************************/

void hisfc350_set_rate(struct hisfc_host *host, struct spi_operation *clkops)
{
	if (host->clkops != clkops) {
		host->clkops = clkops;
		clk_set_rate(host->clk, (unsigned long)clkops->clock);
	}
}
/******************************************************************************/

static int hisfc350_driver_probe(struct platform_device *pdev)
{
	int ret = -ENODEV;
	struct hisfc_host *host;
	struct mtd_info *mtd;
	struct resource *res;

	host = devm_kzalloc(&pdev->dev, sizeof(struct hisfc_host), GFP_KERNEL);
	if (!host) {
		pr_err("%s: allocate memroy fail.\n", __func__);
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, host);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->regbase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->regbase)) {
		pr_err("%s: regbase ioremap fail.\n", __func__);
		return PTR_ERR(host->regbase);
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	host->iobase_phyaddr = res->start;
	host->iobase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->iobase)) {
		pr_err("%s: iobase ioremap fail.\n", __func__);
		return PTR_ERR(host->iobase);
	}

	host->clk = devm_clk_get(&pdev->dev, "clk");
	if (IS_ERR_OR_NULL(host->clk)) {
		pr_err("get clock fail.\n");
		return PTR_ERR(host->clk);
	}

	host->clkops = NULL;

	clk_prepare_enable(host->clk);

	if (hisfc_read(host, HISFC350_VERSION) != 0x350)
		return -ENODEV;

	host->dma_viraddr = dma_alloc_coherent(&pdev->dev, HISFC350_DMA_MAX_SIZE,
		&host->dma_phyaddr, GFP_KERNEL);
	if (host->dma_viraddr == NULL) {
		pr_err("allocate dma buffer failed.\n");
		return -ENOMEM;
	}

	mtd = host->mtd;
	mtd->name = DRVNAME;
	mtd->type = MTD_NORFLASH;
	mtd->writesize = 1;
	mtd->flags = MTD_CAP_NORFLASH;
	mtd->owner = THIS_MODULE;

	if (hisfc350_probe(host)) {
		ret = -ENODEV;
		goto fail;
	}

	mtd->_erase = hisfc350_reg_erase;
	mtd->_write = hisfc350_dma_write;
	mtd->_read  = hisfc350_dma_read;
	mtd->size = host->chipsize;
	mtd->erasesize = host->erasesize;

	mutex_init(&host->lock);

	ret = mtd_device_parse_register(mtd, NULL, NULL, NULL, 0);
	if (ret) {
		mutex_destroy(&host->lock);
		goto fail;
	}

	return 0;
fail:
	if (host->dma_viraddr)
		dma_free_coherent(&pdev->dev, HISFC350_DMA_MAX_SIZE,
		host->dma_viraddr, host->dma_phyaddr);
	return ret;
}
/*****************************************************************************/

static int hisfc350_driver_remove(struct platform_device *pdev)
{
	struct hisfc_host *host = platform_get_drvdata(pdev);

	mutex_destroy(&host->lock);

	if (host->dma_viraddr)
		dma_free_coherent(&pdev->dev, HISFC350_DMA_MAX_SIZE,
		host->dma_viraddr, host->dma_phyaddr);

	mtd_device_unregister(host->mtd);

	return 0;
}
/*****************************************************************************/

static const struct of_device_id
hi3716cv200_hisfc350_match[] __maybe_unused = {
	{ .compatible = "hi3716cv200.hisfc350", },
	{},
};
MODULE_DEVICE_TABLE(of, hi3716cv200_hisfc350_match);

static struct platform_driver hisfc350_pltm_driver = {
	.probe  = hisfc350_driver_probe,
	.remove = hisfc350_driver_remove,
	.shutdown = hisfc350_driver_shutdown,
	.suspend  = hisfc350_driver_suspend,
	.resume   = hisfc350_driver_resume,
	.driver = {
		.name = DRVNAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hi3716cv200_hisfc350_match),
	},
};
/*****************************************************************************/

static int __init hisfc350_module_init(void)
{
	pr_info("registerd new interface driver hisfc350.\n");
	return platform_driver_register(&hisfc350_pltm_driver);
}
module_init(hisfc350_module_init);
/*****************************************************************************/

static void __exit hisfc350_module_exit(void)
{
	platform_driver_unregister(&hisfc350_pltm_driver);
}
module_exit(hisfc350_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("Hisilicon SPIFlash Controller V350 Device Driver, Version 2.00");
