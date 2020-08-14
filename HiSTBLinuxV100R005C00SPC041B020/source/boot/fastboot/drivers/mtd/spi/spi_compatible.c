/******************************************************************************
*    Copyright (c) 2009-2011 by CCC.
*    All rights reserved.
* ***
*    Create by CCC. 2011-12-02
*
******************************************************************************/

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <asm-generic/errno.h>
#include <linux/mtd/mtd.h>

/*****************************************************************************/

#ifdef CONFIG_SPI_FLASH_HISFC300
extern int hisfc300_spiflash_init(struct spi_flash ** spiflash,
	struct mtd_info_ex ** spiinfo);
#endif

#ifdef CONFIG_SPI_FLASH_HISFC350
extern int hisfc350_spiflash_init(struct spi_flash ** spiflash,
	struct mtd_info_ex ** spiinfo);
#endif

#ifdef CONFIG_HIFMC100_SPI_NOR_SUPPORT
extern int hifmc100_spinor_init(struct spi_flash ** spiflash,
	struct mtd_info_ex ** spiinfo);
#endif

/*****************************************************************************/

static struct spi_flash * spiflash = NULL;
static struct mtd_info_ex * spiinfo_ex = NULL;
/*****************************************************************************/

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
	unsigned int max_hz, unsigned int spi_mode)
{
	int devs;

	if (spiflash)
		return spiflash;

	devs = (get_cpudevs() & DEV_HISFC_MASK);

#ifdef CONFIG_SPI_FLASH_HISFC300
	if (devs == DEV_HISFC_AUTO || devs == DEV_HISFC300)
		hisfc300_spiflash_init(&spiflash, &spiinfo_ex);
#endif

#ifdef CONFIG_SPI_FLASH_HISFC350
	if (devs == DEV_HISFC_AUTO || devs == DEV_HISFC350)
		hisfc350_spiflash_init(&spiflash, &spiinfo_ex);
#endif

#ifdef CONFIG_HIFMC100_SPI_NOR_SUPPORT
	if (devs == DEV_HISFC_AUTO || devs == DEV_NOR_HIFMC100)
		hifmc100_spinor_init(&spiflash, &spiinfo_ex);
#endif

	return spiflash;
}
/*****************************************************************************/

struct mtd_info_ex * get_spiflash_info(void)
{
	int devs;

	if (spiinfo_ex)
		return spiinfo_ex;

	devs = (get_cpudevs() & DEV_HISFC_MASK);

#ifdef CONFIG_SPI_FLASH_HISFC300
	if (devs == DEV_HISFC_AUTO || devs == DEV_HISFC300)
		hisfc300_spiflash_init(&spiflash, &spiinfo_ex);
#endif

#ifdef CONFIG_SPI_FLASH_HISFC350
	if (devs == DEV_HISFC_AUTO || devs == DEV_HISFC350)
		hisfc350_spiflash_init(&spiflash, &spiinfo_ex);
#endif

#ifdef CONFIG_HIFMC100_SPI_NOR_SUPPORT
	if (devs == DEV_HISFC_AUTO || devs == DEV_NOR_HIFMC100)
		hifmc100_spinor_init(&spiflash, &spiinfo_ex);
#endif

	return spiinfo_ex;
}
/*****************************************************************************/

void spi_flash_free(struct spi_flash *flash)
{
}

