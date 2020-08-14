/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <version.h>

#include <asm/io.h>

#ifdef CONFIG_CMD_NAND
#  include <hinfc_gen.h>
#endif /* CONFIG_CMD_NAND */
/*****************************************************************************/

int do_getinfo(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	struct mtd_info_ex *mtd_info = NULL;
	unsigned int len, ix;

	if(argc < 2)
		goto usage;

	cmd = argv[1];
	if (strcmp(cmd, "bootmode") == 0) {
		switch (get_bootmedia(NULL, NULL))
		{
			default:
			case BOOT_MEDIA_UNKNOWN:
				printf("Boot from unknown device,"
					" please check your hardware config.\n");
				return -1;

			case BOOT_MEDIA_NAND:
				printf("nand\n");
				break;

			case BOOT_MEDIA_SPIFLASH:
				printf("spi\n");
				break;

			case BOOT_MEDIA_SPI_NAND:
				printf("spi-nand\n");
				break;

			case BOOT_MEDIA_EMMC:
				printf("emmc\n");
				break;
			case BOOT_MEDIA_SD:
				printf("sd\n");
				break;
			case BOOT_MEDIA_DDR:
				printf("ddr\n");
				break;
		}
		return 0;
	}

	if (strcmp(cmd, "version") == 0) {
		unsigned long boot_version = BOOT_VERSION;
		printf("version: %lx.%lx.%lx\n",
			((boot_version >> 16) & 0xFF),
			((boot_version >> 8) & 0xFF),
			(boot_version  & 0xFF));

		return 0;
	}

#ifdef CONFIG_MEM_BASE_FREE_START
	if (strcmp(cmd, "ddrfree") == 0) {
		printf("DDR free region baseaddr:0x%lX size:0x%lX\n", 
			(unsigned long)(MEM_BASE_DDR + CONFIG_MEM_BASE_FREE_START),
			(unsigned long)(get_ddr_size() - CONFIG_MEM_BASE_FREE_START));

		return 0;
	}
#endif

	if (strcmp(cmd, "nand") == 0) {
#ifdef CONFIG_CMD_NAND
		mtd_info = get_nand_info();
#endif
		goto print_info;
	}

	if (strcmp(cmd, "spi") == 0) {
#ifdef CONFIG_CMD_SF
		mtd_info = get_spiflash_info();
#endif
		goto print_info;
	}

usage:
	cmd_usage(cmdtp);
	return -1;

print_info:

	if (mtd_info == NULL || mtd_info->type == 0) {/* no find spi/nand.*/
		printf("no find ");
		if (*cmd == 's') {
			printf("spi");
		} else if (*cmd == 'n') {
			printf("nand");
		}
		printf("\n");
	} else {
		printf("Block:%sB ", ultohstr(mtd_info->erasesize));
		printf("Chip:%sB*%d ",
			ultohstr(mtd_info->chipsize),
			mtd_info->numchips);

		if (*cmd == 's') {
			/* Nothing to display */
		} else if (*cmd == 'n') {
#ifdef CONFIG_CMD_NAND
			/* this printf is only used by hitools. */
			printf("Page:%sB ", ultohstr(mtd_info->pagesize));
			printf("OOB:%sB ", ultohstr(mtd_info->oobused));
			printf("ECC:%s ", nand_ecc_name(mtd_info->ecctype));
#endif
		}
		printf("\nID:");

		len = (mtd_info->id_length > 8 ? 8 : mtd_info->id_length);
		for (ix = 0; ix < len; ix++) {
			printf("0x%02X ", mtd_info->ids[ix]);
		}
		printf("\nName:\"%s\"\n", mtd_info->name);
	}

	return 0;
}

U_BOOT_CMD(
	getinfo, CONFIG_SYS_MAXARGS, 1, do_getinfo,
	"print hardware information",
	"bootmode - get start memeory type e.g. nand/spi etc\n"
	"getinfo nand - get nand flash information\n"
	"getinfo spi - get spi flash information\n"
	"getinfo version - get system version\n"
#ifdef CONFIG_MEM_BASE_FREE_START
	"getinfo ddrfree - get free ddr baseaddr/size\n"
#endif
);

