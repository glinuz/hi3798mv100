#include <common.h>
#include <command.h>
#include <version.h>
#include <hibernate.h>
#include <nand.h>
#include <part.h>
#include <mmc.h>
#include <flash_part.h>

#ifndef U_BOOT_VERSION_CODE
#define U_BOOT_VERSION_CODE     0
#endif

#ifndef U_BOOT_VER
#define U_BOOT_VER(a,b,c)       (((a) << 16) + ((b) << 8) + (c))
#endif

#if U_BOOT_VERSION_CODE < U_BOOT_VER(2010,9,0)
#define CMD_CONST
#else
#define CMD_CONST       const
#endif

#ifdef CONFIG_HIBERNATE

static unsigned int user_api_area;
static unsigned int user_api_addr;
static unsigned int user_api_size;
static unsigned int hibernate_drv_area;
static unsigned int hibernate_drv_addr;
static unsigned int hibernate_drv_size;
static unsigned int hibernate_flag_area;
static unsigned int hibernate_flag_size;
static unsigned int hibernate_data_area;
static unsigned int hibernate_data_size;

static int (*hibernate_cancel)(void) = NULL;
static int do_clear_bootf(cmd_tbl_t *cmdtp, int flag,
			  int argc, char * CMD_CONST argv[]);

/* size of flash space */
#define HIBERNATE_TAG_SIZE       128

#define CONFIG_USERAPI_SIZE  0x20000

struct hibernate_savearea hibernate_savearea[] = {
	/*
	CONFIG_HIBERNATE_SAVEAREA
	*/
	{0}
};

int hibernate_saveno = 0;

void *hibernate_bfaddr;

static int check_boot_ok = 0;

char *get_hidrv_reserve_mem(void);
char *get_userapi_reserve_mem(void);

#ifdef CONFIG_GENERIC_NAND

static unsigned long hibernate_nand_skipbad(nand_info_t *nand, unsigned long off,
						unsigned long size)
{
	unsigned long end = off + size;

	while (off < end) {
		if (!nand_block_isbad(nand, off))
			return off;
		off += nand->erasesize;
	}
	printf("all block is bad\n");
	return (unsigned long)-1;
}


static int hibernate_nand_drvload(void)
{
	int ret;
	size_t size;
	unsigned long off;
	nand_info_t *nand = &nand_info[0];

	off = hibernate_nand_skipbad(nand, hibernate_drv_area, hibernate_drv_size);
	if (off == (unsigned long)-1)
		return 1;

	size = HIBERNATE_DRV_PRELOAD;
	if ((ret = nand_read(nand, off, &size,
		(void *)hibernate_drv_addr)) == 0) {
			if (*(int *)(hibernate_drv_addr + HIBERNATE_HEADER_ID) != HIBERNATE_ID_DRIVER)
				return 1;
			size = *(int *)(hibernate_drv_addr + HIBERNATE_HEADER_COPY_SIZE) -
				HIBERNATE_DRV_PRELOAD;
			if (size <= 0 ||
				(ret = nand_read(nand, off + HIBERNATE_DRV_PRELOAD, &size,
				(void *)(hibernate_drv_addr +
				HIBERNATE_DRV_PRELOAD))) == 0) {
					size += HIBERNATE_DRV_PRELOAD;
					flush_cache(hibernate_drv_addr, size);
					return 0;
			}
	}

	printf("hibdrv read error %d\n", ret);
	return ret;
}

static int hibernate_nand_bfload(int saveno)
{
	int ret;
	size_t size;
	unsigned long off;
	nand_info_t *nand = &nand_info[0];

	off = hibernate_nand_skipbad(nand, hibernate_savearea[saveno].bootflag_area,
		hibernate_savearea[saveno].bootflag_size);
	if (off == (unsigned long)-1)
		return 1;

	size = HIBERNATE_BF_LEN;
	ret = nand_read(nand, off, &size, hibernate_bfaddr);
	if (ret != 0) {
		printf("bootflag read error %d\n", ret);
		return ret;
	}

	return 0;
}

static int hibernate_nand_bferase(int saveno)
{
	int ret;
	unsigned long off, end;
	nand_info_t *nand = &nand_info[0];

	off = hibernate_savearea[saveno].bootflag_area;
	end = off + hibernate_savearea[saveno].bootflag_size;
	printf("Hibernate!! bootflag clear NAND: 0x%08lx-0x%08lx\n", off, end - 1);

	while (off < end) {
		if (!nand_block_isbad(nand, off)) {
			if ((ret = nand_erase(nand, off, nand->erasesize)) != 0) {
				printf("bootflag erase error %d\n", ret);
				return ret;
			}
		}
		off += nand->erasesize;
	}
	return 0;
}

#endif

static int hibernate_dev_drvload(disk_partition_t *info, block_dev_desc_t *dev_desc)
{
	int ret;
	unsigned long size, off, blk;

	off = info->start + (hibernate_drv_area>>9);
	if ((ret = dev_desc->block_read(dev_desc->dev, off, 1,
		(void *)hibernate_drv_addr)) >= 0) {
			/* flush cache after read */
			flush_cache((ulong)hibernate_drv_addr, 512); /* FIXME */
			if (*(int *)(hibernate_drv_addr + HIBERNATE_HEADER_ID) != HIBERNATE_ID_DRIVER)
				return 1;
			size = *(int *)(hibernate_drv_addr + HIBERNATE_HEADER_COPY_SIZE);
			blk = (size - 1) / dev_desc->blksz;
			if (blk <= 0 ||
				(ret = dev_desc->block_read(dev_desc->dev, off + 1, blk,
				(void *)(hibernate_drv_addr +
				dev_desc->blksz))) >= 0) {
					flush_cache(hibernate_drv_addr, size);
					return 0;
			}
	}

	printf("hibdrv read error %d\n", ret);
	return ret;
}

static int userapi_drvload(disk_partition_t *info, block_dev_desc_t *dev_desc)
{
	int ret;
	unsigned long size, off, blk;

	off = info->start + (user_api_area>>9);
	size = CONFIG_USERAPI_SIZE;
	blk = (size + dev_desc->blksz - 1) / dev_desc->blksz;

	if ((ret = dev_desc->block_read(dev_desc->dev, off, blk,
		(void *)user_api_addr)) >= 0) {
			flush_cache(user_api_addr, size);
			return 0;
	}

	printf("userapi read error %d\n", ret);
	return ret;
}


static int hibernate_dev_bfload(disk_partition_t *info, block_dev_desc_t *dev_desc,
				int saveno)
{
	int ret;
	unsigned long off;

	off = info->start + (hibernate_savearea[saveno].bootflag_area >> 9);

	ret = dev_desc->block_read(dev_desc->dev, off, 1, hibernate_bfaddr);
	if (ret != 1) {
		printf("bootflag read error %d\n", ret);
		return ret;
	}
	/* flush cache after read */
	flush_cache((ulong)hibernate_bfaddr, 512); /* FIXME */

	return 0;
}

static int hibernate_dev_bferase(disk_partition_t *info, block_dev_desc_t *dev_desc,
				 int saveno, const char *name)
{
#if U_BOOT_VERSION_CODE >= U_BOOT_VER(1,3,0)
	int ret;
	unsigned long off;
	int lun, part;
	char *buf[512];

	if (!dev_desc->block_write) {
		printf("not support\n");
		return 0;
	}

	memset(buf, 0, 512);
	lun = HIBERNATE_DEV_TO_LUN(hibernate_savearea[saveno].bootflag_dev);
	part = HIBERNATE_DEV_TO_PART(hibernate_savearea[saveno].bootflag_dev);
	off = info->start + (hibernate_savearea[saveno].bootflag_area >> 9);
	printf("Hibernate!! bootflag clear %s%d part:%d offs:0x%08x (sect:0x%08lx)\n",
		name, lun, part, hibernate_savearea[saveno].bootflag_area, off);

	ret = dev_desc->block_write(dev_desc->dev, off, 1, (void *)buf);
	if (ret != 1) {
		printf("bootflag erase error %d\n", ret);
		return ret;
	}
#else
	printf("not support\n");
#endif
	return 0;
}

#ifdef CONFIG_GENERIC_MMC

static int hibernate_sd_init(int dev, disk_partition_t *info,
				 block_dev_desc_t **dev_desc)
{
	int ret;
	u32 lun, part;
	block_dev_desc_t *mmc_get_dev(int dev);
#ifdef CONFIG_GENERIC_MMC
	struct mmc *mmc;
#endif

	lun = HIBERNATE_DEV_TO_LUN(dev);
	part = HIBERNATE_DEV_TO_PART(dev);

#ifdef CONFIG_GENERIC_MMC
	if (!(mmc = find_mmc_device(lun))) {
		printf("device %d not found\n", lun);
		return -1;
	}

	ret = mmc_init(mmc);
#else
#if U_BOOT_VERSION_CODE < U_BOOT_VER(2009,3,0)
	ret = mmc_init(0);
#else
	ret = mmc_legacy_init(0);
#endif
#endif

	if (ret != 0) {
		printf ("No MMC card found\n");
		return ret;
	}

	if ((*dev_desc = mmc_get_dev(lun)) == NULL) {
		printf("device %d not found\n", lun);
		return -1;
	}

	if (part == 0) {
		info->start = 0;
		return 0;
	}

	return 0;
}


static int hibernate_sd_drvload(void)
{
	int ret;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;
	int dev = HIBERNATE_DEV_SD;

	ret = hibernate_sd_init(dev, &info, &dev_desc);
	if (ret != 0)
		return ret;

	ret = hibernate_dev_drvload(&info, dev_desc);
	if (ret != 0)
		return ret;

	return userapi_drvload(&info, dev_desc);
}


static int hibernate_sd_bfload(int saveno)
{
	int ret;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;

	ret = hibernate_sd_init(hibernate_savearea[saveno].bootflag_dev, &info, &dev_desc);
	if (ret != 0)
		return ret;

	return hibernate_dev_bfload(&info, dev_desc, saveno);
}

static int hibernate_sd_bferase(int saveno)
{
	int ret;
	block_dev_desc_t *dev_desc;
	disk_partition_t info;

	ret = hibernate_sd_init(hibernate_savearea[saveno].bootflag_dev, &info, &dev_desc);
	if (ret != 0)
		return ret;

	return hibernate_dev_bferase(&info, dev_desc, saveno, "SD");
}
#endif

int hibernate_drvload(void)
{
	int ret = 0;
	int boot_media = get_bootmedia(NULL, NULL);

	switch (boot_media) {
#ifdef CONFIG_GENERIC_NAND
		case BOOT_MEDIA_NAND:
			ret = hibernate_nand_drvload();
			break;
#endif
#ifdef CONFIG_GENERIC_MMC
		case BOOT_MEDIA_EMMC:
		case BOOT_MEDIA_SD:
			ret = hibernate_sd_drvload();
			break;
#endif
		default :
			printf("Unknow bootmedia, hibernation driver load failed\n");
			ret = 1;
			break;
	}

	return ret;
}

int hibernate_check_bootargs(void)
{
	int ret;
	unsigned long long addr, len;
	char *ptr;
	int boot_media;
	char media_name[16];
	char parts_name[16];
	char *userapi = "userapi", *drv = "hibdrv", *flag = "qbflag", *data = "qbdata";
	char tag_buf[HIBERNATE_TAG_SIZE];

	const char *bootargs = getenv("bootargs");
	if (!bootargs) {
		ret = 1;
		goto failed;
	}

	boot_media = get_bootmedia(NULL, NULL);

	switch (boot_media) {
		case BOOT_MEDIA_NAND:
			strcpy(media_name, "hinand");
			strcpy(parts_name, "mtdparts=");
			break;
		case BOOT_MEDIA_EMMC:
		case BOOT_MEDIA_SD:
			strcpy(media_name, "mmcblk0");
			strcpy(parts_name, "blkdevparts=");
			break;
		default:
			ret = 1;
			goto failed;
	}

	if (NULL == (ptr = strstr(bootargs, parts_name))) {
		ret = 1;
		goto failed;
	}
	ptr += strlen(parts_name);

	if (find_flash_part(ptr, media_name, userapi, &addr, &len)) {
		user_api_area = (unsigned int)addr;
		user_api_size = (unsigned int)len;
	} else {
		ret = 1;
		goto failed;
	}

	if (find_flash_part(ptr, media_name, drv, &addr, &len)) {
		hibernate_drv_area = (unsigned int)addr;
		hibernate_drv_size = (unsigned int)len;
	} else {
		ret = 1;
		goto failed;
	}

	if (find_flash_part(ptr, media_name, flag, &addr, &len)) {
		hibernate_flag_area = (unsigned int)addr;
		hibernate_flag_size = (unsigned int)len;
	} else {
		ret = 1;
		goto failed;
	}

	if (find_flash_part(ptr, media_name, data, &addr, &len)) {
		hibernate_data_area = (unsigned int)addr;
		hibernate_data_size = (unsigned int)len;
	} else {
		ret = 1;
		goto failed;
	}

	if (boot_media == BOOT_MEDIA_NAND)
		hibernate_savearea[0].bootflag_dev = HIBERNATE_DEV(NAND, 0, 0);
	else if (boot_media == BOOT_MEDIA_EMMC || boot_media == BOOT_MEDIA_SD)
		hibernate_savearea[0].bootflag_dev = HIBERNATE_DEV(SD, 0, 0);

	hibernate_savearea[0].bootflag_area = hibernate_flag_area;
	hibernate_savearea[0].bootflag_size = hibernate_flag_size;

	hibernate_drv_addr = (unsigned int)get_hidrv_reserve_mem();
	if (!hibernate_drv_addr) {
		ret = 1;
		goto failed;
	}
	/* init loadaddress */
	user_api_addr = (unsigned int)get_userapi_reserve_mem();
	if (!user_api_addr) {
		ret = 1;
		goto failed;
	}

	if (hibernate_drv_addr%0x100000) {
		printf("Error!!Hibernate driver address(0x%X) NOT aligned with 1M\n", hibernate_drv_addr);
		goto failed;
	}

	if (user_api_addr%0x100000) {
		printf("Error!!Hibernate userapi address(0x%X) NOT aligned with 1M\n", user_api_addr);
		goto failed;
	}

	memset(tag_buf, 0, sizeof(tag_buf));
	snprintf(tag_buf, sizeof(tag_buf), "hibernate_drv_addr=%x", hibernate_drv_addr);
	set_param_data("wpaddr", tag_buf, sizeof(tag_buf));

	memset(tag_buf, 0, sizeof(tag_buf));
	snprintf(tag_buf, sizeof(tag_buf), "user_api_addr=%x", user_api_addr);
	set_param_data("userapi", tag_buf, sizeof(tag_buf));

#define CONFIG_SHOW_HIBERNATE_INFO 1
#if CONFIG_SHOW_HIBERNATE_INFO
	printf("Hibernate!! drv  phyaddr 0x%X\n", hibernate_drv_addr);
	printf("Hibernate!! user api phyaddr 0x%X\n", user_api_addr);

	printf("Hibernate!! userapi  start 0x%X, size 0x%X\n", user_api_area, user_api_size);
	printf("Hibernate!! drv  start 0x%X, size 0x%X\n", hibernate_drv_area, hibernate_drv_size);
	printf("Hibernate!! flag start 0x%X, size 0x%X\n", hibernate_flag_area, hibernate_flag_size);
	printf("Hibernate!! data start 0x%X, size 0x%X\n", hibernate_data_area, hibernate_data_size);
#endif
	check_boot_ok = 1;

	return 0;

failed:

	return ret;
}

int register_hibernate_cancel(int (*cancel_func)(void))
{
	if (!cancel_func) {
		return -1;
	}

	hibernate_cancel = cancel_func;

	return 0;
}

int hibernate_checkboot(int saveno)
{
	int ret;
	char *str;

	if (hibernate_cancel && hibernate_cancel()) {
			do_clear_bootf(NULL, 0, 0, NULL);
			return 1;
	}

	ret = hibernate_check_bootargs();
	if (ret)
		return 1;

	if (saveno < 0) {
		if ((str = getenv("hibernate_saveno")))
			saveno = simple_strtoul(str, NULL, 10);
		else
			saveno = hibernate_saveno;
	}
	if (saveno < 0 || saveno >= HIBERNATE_SAVEAREA_NUM) {
		printf("Illegal saveno %d\n", saveno);
		return 1;
	}

	if ((ret = hibernate_drvload()) != 0)
		return ret;

	hibernate_bfaddr = (void *)hibernate_drv_addr +
		*(int *)(hibernate_drv_addr + HIBERNATE_HEADER_COPY_SIZE);

#ifdef CONFIG_GENERIC_NAND
	if ((hibernate_savearea[saveno].bootflag_dev & HIBERNATE_DEV_MASK) == HIBERNATE_DEV_NAND)
		if ((ret = hibernate_nand_bfload(saveno)) != 0)
			return ret;
#endif

#ifdef CONFIG_GENERIC_MMC
	if ((hibernate_savearea[saveno].bootflag_dev & HIBERNATE_DEV_MASK) == HIBERNATE_DEV_SD)
		if ((ret = hibernate_sd_bfload(saveno)) != 0)
			return ret;
#endif

	if (*(unsigned int *)hibernate_bfaddr != HIBERNATE_ID_BOOTFLAG)
		return 1;

	return 0;
}


int hibernate_boot(int saveno)
{
	int ret;
	int (*hibernate)(void);

	if ((ret = hibernate_checkboot(saveno)) != 0)
		return ret;

	hibernate = (void *)(hibernate_drv_addr + HIBERNATE_HEADER_HIBERNATE);
	ret = hibernate();
	printf("Hibernate!! error can not boot %d\n", ret);
	return ret;
}

static int do_hibernate(cmd_tbl_t *cmdtp, int flag,
			int argc, char * CMD_CONST argv[])
{
	int saveno = hibernate_saveno;
	char *str = NULL;

	if (argc > 1)
		str = argv[1];
	else
		str = getenv("hibernate_saveno");
	if (str)
		saveno = simple_strtoul(str, NULL, 10);

	hibernate_boot(saveno);

	return 0;
}

static int do_clear_bootf(cmd_tbl_t *cmdtp, int flag,
			  int argc, char * CMD_CONST argv[])
{
	int ret = 0;
	int saveno = hibernate_saveno;
	char *str = NULL;

	if (!check_boot_ok)
		hibernate_check_bootargs();

	if (argc > 1)
		str = argv[1];
	else
		str = getenv("hibernate_saveno");
	if (str)
		saveno = simple_strtoul(str, NULL, 10);

	if (saveno < 0)
		saveno = hibernate_saveno;
	if (saveno < 0 || saveno >= HIBERNATE_SAVEAREA_NUM) {
		printf("Illegal saveno %d\n", saveno);
		return 1;
	}

#ifdef CONFIG_GENERIC_NAND
	if ((hibernate_savearea[saveno].bootflag_dev & HIBERNATE_DEV_MASK) == HIBERNATE_DEV_NAND)
		ret = hibernate_nand_bferase(saveno);
#endif

#ifdef CONFIG_GENERIC_MMC
	if ((hibernate_savearea[saveno].bootflag_dev & HIBERNATE_DEV_MASK) == HIBERNATE_DEV_SD)
		ret = hibernate_sd_bferase(saveno);
#endif

	return ret;
}

U_BOOT_CMD(
		hibernate, 2, 0, do_hibernate,
#if U_BOOT_VERSION_CODE < U_BOOT_VER(2009,3,0)
		"hibernate    - Hibernate!! boot\n",
#else
		"Hibernate!! boot",
#endif
		"[saveno]"
#if U_BOOT_VERSION_CODE < U_BOOT_VER(2009,8,0)
		"\n"
#endif
		);

U_BOOT_CMD(
		clear_bootf, 2, 0, do_clear_bootf,
#if U_BOOT_VERSION_CODE < U_BOOT_VER(2009,3,0)
		"clear_bootf - clear Hibernate!! bootflag\n",
#else
		"clear Hibernate!! bootflag",
#endif
		"[saveno]"
#if U_BOOT_VERSION_CODE < U_BOOT_VER(2009,8,0)
		"\n"
#endif
		);

#endif  /* CONFIG_HIBERNATE */
