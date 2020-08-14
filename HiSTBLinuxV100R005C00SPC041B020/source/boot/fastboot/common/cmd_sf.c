/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <spi_flash.h>

#include <asm/io.h>
#include <linux/mtd/mtd.h>

#ifndef CONFIG_SF_DEFAULT_SPEED
# define CONFIG_SF_DEFAULT_SPEED	1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
# define CONFIG_SF_DEFAULT_MODE		SPI_MODE_3
#endif

static struct spi_flash *flash;

static int do_spi_flash_probe(int argc, char *argv[])
{
	unsigned int bus = 0;
	unsigned int cs;
	unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
	unsigned int mode = CONFIG_SF_DEFAULT_MODE;
	char *endp;
	struct spi_flash *new;

	if (argc < 2)
		goto usage;

	cs = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || (*endp != 0 && *endp != ':'))
		goto usage;
	if (*endp == ':') {
		if (endp[1] == 0)
			goto usage;

		bus = cs;
		cs = simple_strtoul(endp + 1, &endp, 0);
		if (*endp != 0)
			goto usage;
	}

	if (argc >= 3) {
		speed = simple_strtoul(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			goto usage;
	}
	if (argc >= 4) {
		mode = simple_strtoul(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			goto usage;
	}

	new = spi_flash_probe(bus, cs, speed, mode);
	if (!new) {
		printf("Failed to initialize SPI flash at %u:%u\n", bus, cs);
		return 1;
	}

	if (flash)
		spi_flash_free(flash);
	flash = new;

	printf("%u KiB %s at %u:%u is now current device\n",
			flash->size >> 10, flash->name, bus, cs);

	return 0;

usage:
	puts("Usage: sf probe [bus:]cs [hz] [mode]\n");
	return 1;
}

static int do_spi_flash_read_write(int argc, char *argv[])
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf;
	char *endp;
	int ret;
	struct mtd_info_ex *spiflash_info = get_spiflash_info();

	if (argc < 4)
		goto usage;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		goto usage;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		goto usage;

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (offset > spiflash_info->chipsize) {
		printf("ERROR: read start address is too large! "
		       "offset: 0x%08lx, chipsize: 0x%08lx\n",
		       offset, (unsigned long)spiflash_info->chipsize);
		return 1;
	}

	if (strncmp(argv[0], "read", sizeof("read")) == 0) {
		if ((offset + len) > spiflash_info->chipsize) {
			len = (unsigned long)(spiflash_info->chipsize - offset);
			printf("WARNING: read area is out of range, "
			       "length is truncated to 0x%08lx.\n", len);
		}
		ret = spi_flash_read(flash, offset, len, buf);
	} else {
		unsigned long write_start, write_len, write_step;
		int percent_complete = -1;
		char *pbuf = buf;

		if ((offset + len) > spiflash_info->chipsize) {
			printf("ERROR: write area is out of range!\n\n");
			return 1;
		}

		write_start = offset;
		write_len   = len;
		write_step  = spiflash_info->erasesize;

		while (len > 0) {
			if (len < write_step)
				write_step = len;

			ret = spi_flash_write(flash, offset, write_step, pbuf);
			if (ret)
				break;

			offset += write_step;
			pbuf   += write_step;
			len    -= write_step;

			do {
				unsigned long long n = (unsigned long long)
					(offset - write_start) * 100;
				int percent;

				do_div(n, write_len);
				percent = (int)n;

				/* output progress message only at whole percent
				 * steps to reduce the number of messages
				 * printed on (slow) serial consoles
				 */
				if (percent != percent_complete) {
					percent_complete = percent;

					printf("\rWriting at 0x%lx -- %3d%% "
						"complete.", offset, percent);
				}
			} while (0);
		}
	}
	puts("\n");

	unmap_physmem(buf, len);

	if (ret) {
		printf("SPI flash %s failed\n", argv[0]);
		return 1;
	}

	return 0;

usage:
	printf("Usage: sf %s addr offset len\n", argv[0]);
	return 1;
}

static int do_spi_flash_erase(int argc, char *argv[])
{
	unsigned long offset;
	unsigned long len;
	char *endp;
	int ret;
	struct mtd_info_ex *spiflash_info = get_spiflash_info();
	unsigned long erase_start, erase_len, erase_step;
	int percent_complete = -1;

	if (argc < 3)
		goto usage;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;
	len = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		goto usage;

	if (offset & (spiflash_info->erasesize-1)) {
		printf("ERROR: erase start address is not block aligned!\n\n");
		return 1;
	}

	if (offset > spiflash_info->chipsize) {
		printf("ERROR: erase start address is too large! "
		       "offset: 0x%08lx, chipsize: 0x%08lx\n",
		       offset, (unsigned long)spiflash_info->chipsize);
		return 1;
	}

	if ((offset + len) > spiflash_info->chipsize) {
		len = (unsigned long)(spiflash_info->chipsize - offset);
		printf("WARNING: erase area is out of range, "
		       "length is truncated to 0x%08lx \n", len);
	}

	if (len & (spiflash_info->erasesize-1)) {
		printf("ERROR: erase length is not block aligned!\n\n");
		return 1;
	}

	erase_start = offset;
	erase_len   = len;
	erase_step  = spiflash_info->erasesize;

	while (len > 0) {
		if (len < erase_step)
			erase_step = len;

		ret = spi_flash_erase(flash, offset, erase_step);
		if (ret) {
			printf("SPI flash %s failed\n", argv[0]);
			return 1;
		}

		len -= erase_step;
		offset += erase_step;

		do {
			unsigned long long n = (unsigned long long)
				(offset - erase_start) * 100;
			int percent;

			do_div(n, erase_len);
			percent = (int)n;

			/* output progress message only at whole percent
			 * steps to reduce the number of messages printed
			 * on (slow) serial consoles
			 */
			if (percent != percent_complete) {
				percent_complete = percent;

				printf("\rErasing at 0x%lx -- %3d%% complete.",
						offset, percent);
			}
		} while (0);
	}

	puts("\n");

	return 0;

usage:
	puts("Usage: sf erase offset len\n");
	return 1;
}

static int do_spi_flash(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *cmd;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (strncmp(cmd, "probe", sizeof("probe")) == 0)
		return do_spi_flash_probe(argc - 1, argv + 1);

	/* The remaining commands require a selected device */
	if (!flash) {
		puts("No SPI flash selected. Please run `sf probe'\n");
		return 1;
	}

	if (strncmp(cmd, "read", sizeof("read")) == 0 || strncmp(cmd, "write", sizeof("write")) == 0)
		return do_spi_flash_read_write(argc - 1, argv + 1);
	if (strncmp(cmd, "erase", sizeof("erase")) == 0)
		return do_spi_flash_erase(argc - 1, argv + 1);

usage:
	cmd_usage(cmdtp);
	return 1;
}

#ifndef CONFIG_SUPPORT_CA_RELEASE
U_BOOT_CMD(
	sf,	5,	1,	do_spi_flash,
	"SPI flash sub-system",
	"probe [bus:]cs [hz] [mode]	- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"sf read addr offset len 	- read `len' bytes starting at\n"
	"				  `offset' to memory at `addr'\n"
	"sf write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"sf erase offset len		- erase `len' bytes from `offset'"
);
#else
U_BOOT_CMD(
	sf,	5,	1,	do_spi_flash,
	"",
	""
);
#endif
