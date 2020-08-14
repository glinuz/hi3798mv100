/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Lyj 2013-11-04
 *
******************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <bootimg.h>
#include <errno.h>

void show_bootimg_header(void *buf)
{
	struct boot_img_hdr *hdr = buf;

	if (!hdr)
		return;

	printf("\nFound Initrd at 0x%08X (Size %d Bytes), align at %d Bytes\n", \
		hdr->ramdisk_addr, hdr->ramdisk_size, hdr->page_size);
	printf("\n");
}

int check_bootimg(char *buf, unsigned int len)
{
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	if (len < sizeof(struct boot_img_hdr))
		return -EINVAL;

	if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
		return -ENODATA;

	return 0;
}

int bootimg_get_kernel(char *buf, unsigned int len, unsigned int *entry)
{
	int ret;
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	ret = check_bootimg(buf, len);
	if (ret)
		return ret;

	if (hdr->page_size > len)
		return -EINVAL;

	if (entry)
		*entry = (unsigned int)((unsigned int)hdr + hdr->page_size);

	return 0;
}

int bootimg_get_ramfs(char *buf, unsigned int len, unsigned int *initrd_start,
		      unsigned int *initrd_size)
{
	int ret;
	struct boot_img_hdr *hdr = (struct boot_img_hdr *)buf;

	ret = check_bootimg(buf, len);
	if (ret)
		return ret;

	if (!hdr->ramdisk_size || (hdr->kernel_size + hdr->page_size) > len)
		return -EINVAL;

	if (initrd_start) {
		unsigned int initrd_img_addr = (unsigned int)hdr \
			+ roundup(hdr->kernel_size + hdr->page_size, hdr->page_size);

		memmove((void *)hdr->ramdisk_addr, (const void *)initrd_img_addr,
			hdr->ramdisk_size);
		*initrd_start = hdr->ramdisk_addr;
	}

	if (initrd_size)
		*initrd_size = hdr->ramdisk_size;

	return 0;
}
