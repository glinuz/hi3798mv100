/*
 * (C) Copyright 2003
 * Kyle Harris, kharris@nexus-tech.net
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
#include <mmc.h>

extern int select_boot_part(struct mmc *mmc, int boot_part);
extern void emmc_bootmode_read(void *ptr, unsigned int size);
extern int print_mmc_reg(int dev_num, int show_ext_csd);
extern int mmc_test(struct mmc *mmc, int dev, void *addr_in, void *addr_out, int blk, u32 cnt);

#ifdef CONFIG_EXT4_SPARSE
extern int ext4_unsparse(struct mmc *mmc, u32 dev, u8 *pbuf, u32 blk, u32 cnt);
#endif

#ifndef CONFIG_GENERIC_MMC
static int curr_device = -1;

int do_mmc (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int dev;

	if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}

	if (strncmp(argv[1], "init", sizeof("init")) == 0) {
		if (argc == 2) {
			if (curr_device < 0)
				dev = 1;
			else
				dev = curr_device;
		} else if (argc == 3) {
			dev = (int)simple_strtoul(argv[2], NULL, 10);
		} else {
			cmd_usage(cmdtp);
			return 1;
		}

		if (mmc_legacy_init(dev) != 0) {
			puts("No MMC card found\n");
			return 1;
		}

		curr_device = dev;
		printf("mmc%d is available\n", curr_device);
	} else if (strncmp(argv[1], "device", sizeof("device")) == 0) {
		if (argc == 2) {
			if (curr_device < 0) {
				puts("No MMC device available\n");
				return 1;
			}
		} else if (argc == 3) {
			dev = (int)simple_strtoul(argv[2], NULL, 10);

#ifdef CONFIG_SYS_MMC_SET_DEV
			if (mmc_set_dev(dev) != 0)
				return 1;
#endif
			curr_device = dev;
		} else {
			cmd_usage(cmdtp);
			return 1;
		}

		printf("mmc%d is current device\n", curr_device);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}

	return 0;
}

U_BOOT_CMD(
	mmc, 3, 1, do_mmc,
	"MMC sub-system",
	"init [dev] - init MMC sub system\n"
	"mmc device [dev] - show or set current device"
);
#else /* !CONFIG_GENERIC_MMC */

static void print_mmcinfo(struct mmc *mmc)
{
	printf("Device: %s\n", mmc->name);
	printf("Manufacturer ID: %x\n", mmc->cid[0] >> 24);
	printf("OEM: %x\n", (mmc->cid[0] >> 8) & 0xffff);
	printf("Name: %c%c%c%c%c \n", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);

	printf("Tran Speed: %d\n", mmc->tran_speed);
	printf("Rd Block Len: %d\n", mmc->read_bl_len);

	printf("%s version %d.%d\n", IS_SD(mmc) ? "SD" : "MMC",
			(mmc->version >> 8) & 0xf, mmc->version & 0xff);

	printf("High Capacity: %s\n", mmc->high_capacity ? "Yes" : "No");
	printf("Capacity: %lld\n", mmc->capacity);

	printf("Bus Width: %d-bit\n", mmc->bus_width);
}

int do_mmcinfo (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct mmc *mmc;
	int dev_num;

	if (argc < 2)
		dev_num = 0;
	else
		dev_num = simple_strtoul(argv[1], NULL, 0);

	mmc = find_mmc_device(dev_num);

	if (mmc) {
		mmc_init(mmc);

		print_mmcinfo(mmc);
	}

	return 0;
}

U_BOOT_CMD(mmcinfo, 2, 0, do_mmcinfo,
	"mmcinfo <dev num>-- display MMC info",
	""
);

int do_mmcops(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int rc = 0;
	unsigned long long start_ticks, end_ticks;
	unsigned long long size = 0;

	switch (argc) {
	case 3:
		if (strncmp(argv[1], "rescan", sizeof("rescan")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc)
				return 1;

			mmc->is_init = 0;
			mmc_init(mmc);

			return 0;
		} else if (strncmp(argv[1], "reg", sizeof("reg")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			print_mmc_reg(dev, 1);
			break;
		}
		cmd_usage(cmdtp);
		return 1;
	case 0:
	case 1:
		cmd_usage(cmdtp);
		return 1;

	case 2:
		if (!strncmp(argv[1], "list", sizeof("list"))) {
			print_mmc_devices('\n');
			return 0;
		}
		cmd_usage(cmdtp);
		return 1;
	default: /* at least 5 args */
		if (strncmp(argv[1], "read", sizeof("read")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			u32 cnt = simple_strtoul(argv[5], NULL, 16);
			u32 n;
			u32 blk = simple_strtoul(argv[4], NULL, 16);
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc)
				return 1;

			printf("\nMMC read: dev # %d, block # %d, count %d ... ",
				dev, blk, cnt);

			mmc_init(mmc);

			start_ticks = get_ticks();
			n = mmc->block_dev.block_read(dev, blk, cnt, addr);
			end_ticks = get_ticks();

			/* flush cache after read */
			flush_cache((ulong)addr, cnt * 512); /* FIXME */

			size = mmc->read_bl_len * cnt;

			printf("%d blocks read: %s\n",
				n, (n==cnt) ? "OK" : "ERROR");
			printf("\n%llu Bytes/s\n", (size * CONFIG_SYS_HZ)/(end_ticks - start_ticks));
			return (n == cnt) ? 0 : 1;
		} else if (strncmp(argv[1], "write", sizeof("write")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			u32 cnt = simple_strtoul(argv[5], NULL, 16);
			u32 n;
			struct mmc *mmc = find_mmc_device(dev);

			int blk = simple_strtoul(argv[4], NULL, 16);

			if (!mmc)
				return 1;

			printf("\nMMC write: dev # %d, block # %d, count %d ... ",
				dev, blk, cnt);

			mmc_init(mmc);

			n = mmc->block_dev.block_write(dev, blk, cnt, addr);

			printf("%d blocks written: %s\n",
				n, (n == cnt) ? "OK" : "ERROR");
			return (n == cnt) ? 0 : 1;
		} else if (strncmp(argv[1], "test", sizeof("test")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr_in = (void *)simple_strtoul(argv[3], NULL, 16);
			void *addr_out = (void *)simple_strtoul(argv[4], NULL, 16);
			int blk = simple_strtoul(argv[5], NULL, 16);
			u32 cnt = simple_strtoul(argv[6], NULL, 16);
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc) {
				printf("Invalid mmc device index!\n");
				return 1;
			}

			printf("\nMMC test: dev # %d, block # %d, count %d \n", dev, blk, cnt);

			if (mmc_init(mmc) < 0 ) {
				printf("MMC init failed!\n");
				hang();
				return 1;
			}

			if (mmc_test(mmc, dev, addr_in, addr_out, blk, cnt))
				return 1;

			return 0;
		} else if (strncmp(argv[1], "erase", sizeof("erase")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			u32 blk = simple_strtoul(argv[3], NULL, 16);
			u32 cnt = simple_strtoul(argv[4], NULL, 16);
			u32 n;
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc)
				return 1;

			printf("\nMMC erase: dev # %d, block # %d, count %d ... ",
					dev, blk, cnt);

			mmc_init(mmc);

			n = mmc->block_dev.block_erase(dev, blk, cnt);

			printf("%d blocks erased: %s\n",
				n, (n == cnt) ? "OK" : "ERROR");
			return (n == cnt) ? 0 : 1;
		} else if (strncmp(argv[1], "write.ext4sp", sizeof("write.ext4sp")) == 0) { 
#ifdef CONFIG_EXT4_SPARSE
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			u32 blk = simple_strtoul(argv[4], NULL, 16);
			u32 cnt = simple_strtoul(argv[5], NULL, 16);
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc)
				return 1;
			mmc_init(mmc);

			printf("\nMMC write ext4 sparse: dev # %d, "
				"block # %d, count %d ... \n",
				dev, blk, cnt);

			return ext4_unsparse(mmc, dev, addr, blk, cnt);
#else
	printf("Not support.\n");
	return 0;
#endif
		} else if (strncmp(argv[1], "bootread", sizeof("bootread")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			u32 cnt = simple_strtoul(argv[5], NULL, 16);
			u32 n;
			u32 blk = simple_strtoul(argv[4], NULL, 16);
			struct mmc *mmc = find_mmc_device(dev);

			if (!mmc)
				return 1;

			printf("\nMMC bootread: dev # %d, block # %d, count %d ... ",
				dev, blk, cnt);

			mmc_init(mmc);

			if (select_boot_part(mmc, 0x1) != 0) {
				printf("select boot part on error\n");
				return 1;
			}

			n = mmc->block_dev.block_read(dev, blk, cnt, addr);

			/* flush cache after read */
			flush_cache((ulong)addr, cnt * 512); /* FIXME */

			if (select_boot_part(mmc, 0x0) != 0) {
				printf("select boot part off error\n");
				return 1;
			}

			printf("%d blocks read: %s\n",
				n, (n==cnt) ? "OK" : "ERROR");
			return (n == cnt) ? 0 : 1;
		} else if (strncmp(argv[1], "bootwrite", sizeof("bootwrite")) == 0) {
			int dev = simple_strtoul(argv[2], NULL, 10);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			u32 cnt = simple_strtoul(argv[5], NULL, 16);
			u32 n;
			struct mmc *mmc = find_mmc_device(dev);

			int blk = simple_strtoul(argv[4], NULL, 16);

			if (!mmc)
				return 1;

			printf("\nMMC bootwrite: dev # %d, block # %d, count %d ... ",
				dev, blk, cnt);

			mmc_init(mmc);

			if(select_boot_part(mmc, 0x1) != 0) {
				printf("select boot part on error\n");
				return 1;
			}

			n = mmc->block_dev.block_write(dev, blk, cnt, addr);

			if(select_boot_part(mmc, 0x0) != 0) {
				printf("select boot part off error\n");
				return 1;
			}

			printf("%d blocks written: %s\n",
				n, (n == cnt) ? "OK" : "ERROR");
			return (n == cnt) ? 0 : 1;
		} else if (strncmp(argv[1], "bootmoderead", sizeof("bootmoderead")) == 0) {

			int dev = simple_strtoul(argv[2], NULL, 10);
			struct mmc *mmc = find_mmc_device(dev);
			void *addr = (void *)simple_strtoul(argv[3], NULL, 16);
			unsigned int size = (unsigned int)simple_strtoull(
				argv[4], NULL, 16);

			if (!mmc)
				return 1;

			emmc_bootmode_read(addr, size);

			/* after bootmode read, the mmc need reinit. */
			mmc->is_init = 0;

		} else {
			cmd_usage(cmdtp);
			rc = 1;
		}

		return rc;
	}
	return 1;
}

U_BOOT_CMD(
	mmc, 7, 1, do_mmcops,
	"MMC sub system",
	"read <device num> addr blk# cnt\n"
	"mmc write <device num> addr blk# cnt\n"
	"mmc erase <device num> blk# cnt\n"
	"mmc write.ext4sp <device num> addr blk# cnt\n"
	"mmc bootread <device num> addr blk# cnt\n"
	"mmc bootwrite <device num> addr blk# cnt\n"
	"mmc rescan <device num>\n"
	"mmc list - lists available devices\n"
	"mmc bootmoderead <device num> addr size\n"
	"mmc reg <device num>");
#endif
