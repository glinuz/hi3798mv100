
#include <common.h>
#include <config.h>
#include <exports.h>
#include <fat.h>
#include <asm/byteorder.h>
#include <part.h>
#include <malloc.h>
#include <linux/compiler.h>
#include <asm/unaligned.h>
#include <usb.h>

/* support max usb partition num ,start with 1 */
#define USB_MAX_PART_NUM  16

#define USB_DEVICE_NAME  "usb"

extern int get_usb_stor_max(void);

static int usb_dev_part_file_check(const char *filename, 
				   const char *ifname, 
				   block_dev_desc_t *dev_desc, 
				   int dev, int part);

/******************************************************************************/

int usb_detect(void)
{
	int ret;

	ret = usb_stop();
	if (ret)
		return ret;
	printf("(Re)start USB...\n");
	ret = usb_init();
	if (ret)
		return ret;
#ifdef CONFIG_USB_STORAGE
	/* try to recognize storage devices immediately */
	usb_stor_scan(1);
#endif
	return 0;
}

int usb_file_check(const char *filename)
{
	int ret;
	int ix_dev;
	int ix_part;
	const char *ifname = USB_DEVICE_NAME;
	block_dev_desc_t *dev_desc = NULL;

	if (!filename) {
		printf("please input valid file\n");
		return -1;
	}

	for (ix_dev = 0 ; ix_dev < get_usb_stor_max(); ix_dev++) {
		dev_desc = usb_stor_get_dev(ix_dev);
		if (!dev_desc)
			continue;
		for (ix_part = 1;
		     ix_part < USB_MAX_PART_NUM;
		     ix_part++) {
			ret = usb_dev_part_file_check(filename,
						      ifname, 
						      dev_desc, 
						      ix_dev, ix_part);
			if (!ret)
				return 0;
		}
	}

	printf("NO Such file: %s on %s device\n", filename, ifname);
	return -1;
}

long usb_file_read(const char *filename, void *buffer, 
		  unsigned long pos, unsigned long length)
{
	if (!buffer) {
		printf("buffer should not be NULL!\n");
		return -1;
	}
	return file_fat_read_at(filename, buffer, pos, length);
}

long usb_get_file_size (const char *filename)
{
	if (!filename) {
		printf("file name and ifname should not be NULL!\n");
		return -1;
	}

	return file_fat_size(filename);
}

static int usb_dev_part_file_check(const char *filename, 
				   const char *ifname, 
				   block_dev_desc_t *dev_desc, 
				   int dev, int part)
{
	long read_size = 0;

	if (!filename) {
		printf("filename should not be NULL!\n");
		return -1;
	}

	if (strncmp(ifname, "usb", 3)) {
		printf("input ifname: %s, but should be: %s!\n", 
		        ifname, USB_DEVICE_NAME);
		return -1;
	}

	if (part >= USB_MAX_PART_NUM) {
		printf("input partition number: %d is larger than %d!\n", 
		        part, USB_MAX_PART_NUM);
		return -1;
	}

	if (fat_register_device(dev_desc,part)) {
		printf("\n** Unable to register %s %d:%d **\n",
			ifname, dev, part);
		return -1;
	}

	/* if read success, file check return success */
	read_size = file_fat_read(filename, &read_size, sizeof(read_size));
	if (read_size < 0)
		return -1;

	printf("%s on device:%s:%d:%d FOUND!\n",
		filename, ifname, dev, part);
	return 0;
}

