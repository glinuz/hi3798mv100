/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/
#include <common.h>
#include <malloc.h>
#include <linux/mtd/mtd.h>
#include <spi_flash.h>
#include <spiflash_logif.h>

/*****************************************************************************/

spiflash_logic_t *spiflash_logic_open(unsigned long long address,
				      unsigned long long length)
{
	struct mtd_info_ex *mtd_info_ex;
	spiflash_logic_t *spiflash_logic;
	struct spi_flash *spiflash;

	spiflash = spi_flash_probe(0,0,0,0);
	if (spiflash == NULL) {
		printf("no devices available\n");
		return NULL;
	}

	mtd_info_ex = get_spiflash_info();
	if (mtd_info_ex == NULL) {
		printf("get spi flash info failed\n");
		return NULL;
	}

	/* Reject open, which are not block aligned */
	if ((address & (mtd_info_ex->erasesize - 1))
	    || (length & (mtd_info_ex->erasesize - 1))) {
		printf("Attempt to open non block aligned, "
			"spi flash blocksize: 0x%08x, address: 0x%08llx,"
			" length: 0x%08llx\n",
			mtd_info_ex->erasesize, address, length);
		return NULL;
	}

	if ((address > spiflash->size)
	    || (length > spiflash->size)
	    || ((address + length) > spiflash->size)) {
		printf("Attempt to open outside the flash area, "
			"spi flash chipsize: 0x%08x, address: 0x%08llx,"
			" length: 0x%08llx\n",
			spiflash->size, address, length);
		return NULL;
	}

	spiflash_logic = malloc(sizeof(spiflash_logic_t));
	if (!spiflash_logic) {
		printf("no many memory.\n");
		return NULL;
	}

	spiflash_logic->spiflash  = spiflash;
	spiflash_logic->address   = address;
	spiflash_logic->length    = length;
	spiflash_logic->erasesize = mtd_info_ex->erasesize;

	return spiflash_logic;
}
/*****************************************************************************/

void spiflash_logic_close(spiflash_logic_t *spiflash_logic)
{
	free(spiflash_logic);
}
/*****************************************************************************/
/*
 * Warn: Do NOT modify the printf string, it may be used by pc tools.
 * offset     should be alignment with spi flash block size
 * length     should be alignment with spi flash block size
 */
long long spiflash_logic_erase(spiflash_logic_t *spiflash_logic,
			 unsigned long long offset,
			 unsigned long long length)
{
	unsigned long blocksize, eraseoffset, eraselen;
	unsigned long long totalerase = 0;
	unsigned long long request_length = length;

	if (!length) {
		printf("Attempt to erase 0 Bytes!\n");
		return -1;
	}

	/* Reject write, which are not block aligned */
	if ((offset & (spiflash_logic->erasesize - 1))
	    || (length & (spiflash_logic->erasesize - 1))) {
		printf("Attempt to erase non block aligned, "
			"spi flash blocksize: 0x%08llx, offset: 0x%08llx,"
			" length: 0x%08llx\n",
			spiflash_logic->erasesize, offset, length);
		return -1;
	}

	if (offset > spiflash_logic->length) {
		printf("Attempt to erase from outside the flash handle area, "
			"flash handle size: 0x%08llx, offset: 0x%08llx\n",
			spiflash_logic->length, offset);
		return -1;
	}

	if ((offset + length) > spiflash_logic->length) {
	    length = spiflash_logic->length - offset;

		printf("Erase length is too large, "
		       "paratition size: 0x%08llx, erase offset: 0x%08llx\n"
		       "Try to erase 0x%08llx instead!\n",
		       spiflash_logic->length,
		       offset,
		       length);
	}

	blocksize   = spiflash_logic->erasesize;
	eraselen    = length;
	eraseoffset = spiflash_logic->address + offset;

	/* eraselen MUST align with blocksize here */
	while (eraselen > 0) {
		int ret;
		printf("\rErasing at 0x%lx\n", eraseoffset);
		ret = spi_flash_erase(spiflash_logic->spiflash,
			eraseoffset, blocksize);
		if (ret) {
			printf("\nSPI Flash Erasing at 0x%lx failed\n",
				eraseoffset);
			return -1;
		}
		eraselen    -= blocksize;
		eraseoffset += blocksize;
		totalerase  += blocksize;
	}
	if (totalerase < request_length)
		printf("request to erase 0x%08llx, and erase 0x%08llx"
		       " successfully!\n",
		       request_length, totalerase);

	return (long long)totalerase;
}
/*****************************************************************************/

int spiflash_logic_write(spiflash_logic_t *spiflash_logic,
			 unsigned long long offset,
			 unsigned int length,
			 unsigned char *buf)
{
	unsigned int totalwrite = length;
	unsigned int ret;

	if (!length) {
		printf("Attempt to write 0 Bytes\n");
		return -1;
	}

	if ((offset > spiflash_logic->length)
	    || (length > spiflash_logic->length)
	    || ((offset + length) > spiflash_logic->length)) {
		printf("Attempt to write outside the flash handle area, "
			"flash handle size: 0x%08llx, offset: 0x%08llx, "
			"length: 0x%08x, phylength:  0x%08llx\n",
			spiflash_logic->length, offset,
			length, offset + length);
		return -1;
	}

	ret = spi_flash_write(spiflash_logic->spiflash,
			      spiflash_logic->address + offset,
			      length,
			      buf);
	if (!ret)
		return totalwrite;

	return ret;
}
/*****************************************************************************/

int spiflash_logic_read(spiflash_logic_t *spiflash_logic,
			unsigned long long offset,
			unsigned int length,
			unsigned char *buf)
{
	int ret;

	if (!length) {
		printf("Attempt to read 0 Bytes\n");
		return -1;
	}

	if (offset > spiflash_logic->length) {
		printf("Attempt to read outside the flash handle area, "
			"flash handle size: 0x%08llx, offset: 0x%08llx\n",
			spiflash_logic->length, offset);
		return -1;
	}

	if ((offset + length) > spiflash_logic->length) {

		length = spiflash_logic->length - offset;

		printf("Read length is too large, "
		       "paratition size: 0x%08llx, read offset: 0x%08llx\n"
		       "Try to read 0x%08x instead!\n",
		       spiflash_logic->length,
		       offset,
		       length);
	}

	ret = spi_flash_read(spiflash_logic->spiflash,
			     spiflash_logic->address + offset,
			     length,
			     buf);
	if (!ret)
		return length;
	return ret;
}
/*****************************************************************************/
