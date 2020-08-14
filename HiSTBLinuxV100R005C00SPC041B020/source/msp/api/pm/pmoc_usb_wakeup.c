/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <linux/usb/ch9.h>

#include <unistd.h>
#include "hi_drv_pmoc.h"
#include "hi_osal.h"

#define USB_CLASS_HUB 9

#define USB_REQ_CLEAR_FEATURE 0x01
#define USB_REQ_SET_FEATURE 0x03

#define USB_CTRL_GET_TIMEOUT 5000
#define USB_CTRL_SET_TIMEOUT 5000
#define USB_DEVICE_REMOTE_WAKEUP 1      /* dev may initiate wakeup */
#define USB_RECIP_DEVICE 0x00

#define PATH_MAX        4096

struct libusb
{
    char *sysfs;
    char *dev;
    char *devnode1;
    char *devnode2;
    char *sysfs_usb;
    char *usb_dev;
    char *remotewakeup;
    char *autosuspend;
    char *bDeviceClass;
	char *busnum;
	char *devnum;
};

/**
 * mkpath - compose full path from 2 given components.
 * @path: the first component
 * @name: the second component
 *
 * This function returns the resulting path in case of success and %NULL in
 * case of failure.
 */
static char *mkpath(const char *path, const char *name)
{
    char *n = NULL;
	size_t len1 = 0, len2= 0;
	
	if (!path || !name) {
		return NULL;
	}

	len1 = strlen(path);
    len2 = strlen(name);

    if ((len1 > PATH_MAX) || (len2 > PATH_MAX))
    {
        return NULL;
    }

    n = (char*)malloc(len1 + len2 + 2);
    if (!n)
    {
        HI_ERR_PM("cannot allocate %d bytes \n", len1 + len2 + 2);
        return NULL;
    }

    memset(n, 0, (len1 + len2 + 2));

    memcpy(n, path, len1);
    if (n[len1 - 1] != '/')
    {
        n[len1] = '/';
    }

    memcpy(n + len1 + 1, name, len2);
    n[len1 + len2 + 1] = '\0';
    return n;
}

struct libusb * libusb_open(void)
{
    int fd;
    struct libusb *lib;
	int size;
	
	size = sizeof(struct libusb);
    lib = calloc(1, size);
    if (!lib)
    {
        return NULL;
    }

    /* TODO: this must be discovered instead */
    lib->sysfs = strdup("/sys");
    if (!lib->sysfs)
    {
        goto error;
    }

    /* TODO: this must be discovered instead */
    lib->dev = strdup("/dev");
    if (!lib->dev)
    {
        goto error;
    }
	
	lib->devnode1 = mkpath(lib->dev, "usbdev%c%c%c%c%c%c%c");
	if (!lib->devnode1)
	{
		goto error;
	}	

	lib->devnode2 = mkpath(lib->dev, "bus/usb/%s/%s");
    if (!lib->devnode2)
    {
        goto error;
    }

    lib->sysfs_usb = mkpath(lib->sysfs, "bus/usb/devices");
    if (!lib->sysfs_usb)
    {
        goto error;
    }

    /* Make sure present */
    fd = open(lib->sysfs_usb, O_RDONLY);
    if (fd == -1)
    {
        goto error;
    }

    close(fd);

    lib->usb_dev = mkpath(lib->sysfs_usb, "%s");
    if (!lib->usb_dev)
    {
        goto error;
    }

    lib->remotewakeup = mkpath(lib->usb_dev, "power/wakeup");
    if (!lib->remotewakeup)
    {
        goto error;
    }

    lib->autosuspend = mkpath(lib->usb_dev, "power/control");
    if (!lib->autosuspend)
    {
        goto error;
    }

	lib->bDeviceClass = mkpath(lib->usb_dev, "bDeviceClass");
	if (!lib->bDeviceClass)
	{
        goto error;
	}
	
	lib->busnum = mkpath(lib->usb_dev, "busnum");
	if (!lib->busnum)
	{
        goto error;
	}

	lib->devnum = mkpath(lib->usb_dev, "devnum");
	if (!lib->devnum)
	{
        goto error;
	}
    return lib;

error:
	free(lib->devnum);
	free(lib->busnum);
    free(lib->bDeviceClass);
    free(lib->remotewakeup);
    free(lib->autosuspend);
    free(lib->usb_dev);
    free(lib->sysfs_usb);
    free(lib->sysfs);
    free(lib->devnode1);
	free(lib->devnode2);
    free(lib->dev);
    lib->devnum = NULL;
    lib->busnum = NULL;
    lib->bDeviceClass = NULL;
    lib->remotewakeup = NULL;
    lib->autosuspend = NULL;
    lib->usb_dev = NULL;
    lib->sysfs_usb = NULL;
    lib->sysfs = NULL;
    lib->devnode1 = NULL; 
    lib->devnode2 = NULL;
    lib->dev = NULL;
    free(lib);
    lib = NULL;
    
    return NULL;
}

void libusb_close(struct libusb * desc)
{
    struct libusb *lib = (struct libusb *)desc;

	if (!lib) {
		return;
	}
	free(lib->devnum);
	free(lib->busnum);
    free(lib->bDeviceClass);
    free(lib->remotewakeup);
    free(lib->autosuspend);
    free(lib->usb_dev);
    free(lib->sysfs_usb);
    free(lib->sysfs);
    free(lib->devnode1);
	free(lib->devnode2);
    free(lib->dev);
    lib->devnum = NULL;
    lib->busnum = NULL;
    lib->bDeviceClass = NULL;
    lib->remotewakeup = NULL;
    lib->autosuspend = NULL;
    lib->usb_dev = NULL;
    lib->sysfs_usb = NULL;
    lib->sysfs = NULL;
    lib->devnode1 = NULL; 
    lib->devnode2 = NULL;
    lib->dev = NULL;
    free(lib);
    lib = NULL;
    
    return;
}

static int io_ctrl_set_devnode(const char *patt1, const char *patt2, char a, char b, 
	char c, char d, char e, char f, char g,	char * busnum, char* devnum)
{
    int fd, ret;
    struct usbdevfs_ctrltransfer ctrl;
    char file[50];

    memset(file, 0, sizeof(file));
    HI_OSAL_Snprintf(file, sizeof(file), patt1, a, b, c, d, e, f, g);

    errno = 0;
    fd = open(file, O_RDWR);
    if (fd < 0)
    {
		memset(file, 0, sizeof(file));
		HI_OSAL_Snprintf(file, sizeof(file), patt2, busnum, devnum);
		errno = 0;
        fd = open(file, O_RDWR);
		if (fd < 0)
		{
			HI_ERR_PM("open failed: \n");
			return -1;
		}
		HI_INFO_PM("%s\n", file);
    }
	HI_INFO_PM("%s\n", file);
    errno = 0;
    ctrl.bRequest = USB_REQ_SET_FEATURE;
    ctrl.bRequestType = USB_RECIP_DEVICE;
    ctrl.data = NULL;
    ctrl.timeout = USB_CTRL_SET_TIMEOUT;
    ctrl.wIndex	 = 0;
    ctrl.wLength = 0;
    ctrl.wValue	 = (USB_DEVICE_REMOTE_WAKEUP);

    errno = 0;
    ret = ioctl(fd, USBDEVFS_CONTROL, &ctrl);
    if (ret < 0)
    {
        HI_ERR_PM("IOCTL failed:\n");
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

/**
 * dev_read_str - read an 'int' value from an UBI device's sysfs file.
 *
 * @patt     the file pattern to read from
 * @dev_num  UBI device number
 * @value    the result is stored here
 *
 * This function returns %0 in case of success and %-1 in case of failure.
 */
static int dev_read_str(const char *patt, char* string)
{
    int fd, rd;
    char buf[50];
    char file[256];

    memset(buf, 0, sizeof(buf));
    memset(file, 0, sizeof(file));
    HI_OSAL_Snprintf(file, sizeof(file), patt, string);
    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    rd = read(fd, buf, 49);
    buf[49] = '\0';
    if (rd == -1)
    {
        goto error;
    }

    close(fd);
    return 0;

error:
    close(fd);
    return -1;
}

/**
 * dev_read_int - read an 'int' value from an UBI device's sysfs file.
 *
 * @patt     the file pattern to read from
 * @dev_num  UBI device number
 * @value    the result is stored here
 *
 * This function returns %0 in case of success and %-1 in case of failure.
 */
static int dev_write_str(const char *patt, char* string, char *value)
{
    int fd, rd;
	char file[256];

    memset(file, 0, sizeof(file));
    HI_OSAL_Snprintf(file, sizeof(file), patt, string);
    fd = open(file, O_WRONLY);
    if (fd == -1)
    {
        return -1;
    }

    rd = write(fd, value, strlen(value));
    if (rd == -1)
    {
        goto error;
    }

    close(fd);
    return 0;

error:
    close(fd);
    return -1;
}

static int dev_read_int(const char *patt, char* string, int *value)
{
    int fd, rd;
    char buf[50];
    char file[256];

    memset(buf, 0, sizeof(buf));
    memset(file, 0, sizeof(file));
    HI_OSAL_Snprintf(file, sizeof(file), patt, string);
    fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }

    rd = read(fd, buf, 49);
    buf[49] = '\0';
    if (rd == -1)
    {
        goto error;
    }

    *value = atoi(buf);

    close(fd);
    return 0;

error:
    close(fd);
    return -1;
}

int usb_set_remote(struct libusb * desc)
{
    DIR *sysfs_usb;
    struct dirent *dirent;
    struct libusb *lib = (struct libusb *)desc;
	char busnum[4]={0x00};
	char devnum[4]={0x00};
    int rc = 0, dev_cnt = 0;
    int value = 0;

	if ((!lib) || !(lib->sysfs_usb) || !(lib->bDeviceClass) 
		|| !(lib->remotewakeup)	|| !(lib->devnum) || !(lib->autosuspend)
		|| !(lib->devnode1) || !(lib->devnode2)) {
		return -1;
	}

    /*
     * We have to scan the USB sysfs directory to identify how many USB
     * devices are present.
     */
    sysfs_usb = opendir(lib->sysfs_usb);
    if (!sysfs_usb)
    {
        return -1;
    }

    while ((dirent = readdir(sysfs_usb)))
    {
        char *name = &dirent->d_name[0];

        /* dev of root hub support remote wakeup */
        {
            rc = dev_read_str(lib->remotewakeup, name);

            /* remote wake up is supported */
            if (rc != -1)
            {
                rc = dev_read_int(lib->bDeviceClass, name, &value);
                if (rc != -1)
                {
                    /* hub is not consider but gl850g need set remotewakeup*/
                    {
                        rc = dev_write_str(lib->autosuspend, name, "auto");
                        if (rc == -1)
                        {
                            goto close;
                        }

                        rc = dev_write_str(lib->remotewakeup, name, "enabled");
                        if (rc == -1)
                        {
                            goto close;
                        }

						value = 0;
						rc = dev_read_int(lib->busnum,name,&value);
						if(rc == -1)
						{
							goto close;
						}
						
						busnum[0] = '0' + value/100;
						value %= 100;
						busnum[1] = '0' + value/10;
						busnum[2] = '0' + value%10;
					
						value = 0;
						rc = dev_read_int(lib->devnum,name,&value);
						if(rc == -1)
						{
							goto close;
						}
						
						devnum[0] = '0' + value/100;
						value %= 100;
						devnum[1] = '0' + value/10;
						devnum[2] = '0' + value%10;

                        if (strlen(name) == 3)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2,
								name[0], name[2], '\0', '\0', '\0', '\0', '\0',busnum,devnum);
                        }
                        else if (strlen(name) == 5)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2, 
								name[0], name[2], name[4], '\0', '\0', '\0', '\0',busnum,devnum);
                        }
                        else if (strlen(name) == 7)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2, 
								name[0], name[2], name[4], name[6], '\0', '\0', '\0',busnum,devnum);
                        }
                        else if (strlen(name) == 9)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2, 
								name[0], name[2], name[4], name[6], name[8], '\0',
                                                     '\0',busnum,devnum);
                        }
                        else if (strlen(name) == 11)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2, 
                                name[0], name[2], name[4], name[6], name[8], name[10],
                                                    '\0',busnum,devnum);
                        }
                        else if (strlen(name) == 13)
                        {
                            rc = io_ctrl_set_devnode(lib->devnode1, lib->devnode2, 
                                name[0], name[2], name[4], name[6], name[8], name[10],
                                                    name[12],busnum,devnum);
                        }

						if(rc == -1)
						{
							goto close;
                        }
                    }
                }
            }
        }
    }

    closedir(sysfs_usb);
    return dev_cnt;

close:
    closedir(sysfs_usb);
    return -1;
}

int usb_get_remote(struct libusb * desc, unsigned char * intmask)
{
    DIR *sysfs_usb;
    struct dirent *dirent;
    struct libusb *lib = (struct libusb *)desc;
    int rc, dev_cnt = 0;
    int value = 0;

	if ((!lib) || !(lib->sysfs_usb) || !(lib->remotewakeup) 
		|| !(lib->bDeviceClass)) {
		return -1;
	}

    /*
     * We have to scan the USBsysfs directory to identify how many USB
     * devices are present.
     */

    if (intmask == NULL)
    {
        HI_ERR_PM(" initmask is NULL\n");
        return -1;
    }
    else
    {
        *intmask = 0;
    }

    sysfs_usb = opendir(lib->sysfs_usb);
    if (!sysfs_usb)
    {
        return -1;
    }

    while ((dirent = readdir(sysfs_usb)))
    {
        char *name = &dirent->d_name[0];

        /* dev of root hub support remote wakeup */
        {
            rc = dev_read_str(lib->remotewakeup, name);

            /* remote wake up is supported */
            if (rc != -1)
            {
                rc = dev_read_int(lib->bDeviceClass, name, &value);
                if (rc != -1)
                {
                    /* hub is not consider */
                    if (value != USB_CLASS_HUB)
                    {
                        dev_cnt++;
#if    defined(CHIP_TYPE_hi3798cv100)
                        if (name[0]==0x33){
                            *intmask |=(0x01<<1);
                        } else {
                            if(name[2]==0x31)
                               *intmask |= (0x01<<0);
                            else if (name[2]==0x32)
                               *intmask |= (0x01<<3);
                            else if (name[2]==0x33)
                               *intmask |= (0x01<<2);
                        }
#elif defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3798mv100)
                       if (name[0]==0x35) {
                            *intmask |=(0x01<<1);
                       } else {
                           if((name[0]==0x33)&&(name[2]==0x31))
                              *intmask |= (0x01<<0);
                           else if (name[2]==0x32)
                              *intmask |= (0x01<<3);
                           else if ((name[0]==0x34)&&(name[2]==0x31))
                              *intmask |= (0x01<<2);
                      }
#elif defined(CHIP_TYPE_hi3798cv200)
                       if (name[0]==0x32) {
                            *intmask |=(0x01<<1);
                       } else if((name[0]==0x33)) {
                              *intmask |= (0x01<<0);
                       } else if (name[0]==0x35)  {
                              *intmask |= (0x01<<2);
                      }
#else
                      if(name[2]==0x31)
                           *intmask |= (0x01<<0);
                      else if (name[2]==0x32)
                           *intmask |= (0x01<<3);
                      else if (name[2]==0x33)
                           *intmask |= (0x01<<2);
#endif
                   }
                }
            }
        }
    }

    closedir(sysfs_usb);
    return dev_cnt;
}

/**
 * get_remotewakeup_devnum - get device number of support remote wakeup.
 *
 * This function returns  dev numbers in case of success and %-1 in case of failure.
 */
int get_remotewakeup_devnum(unsigned char * intmask)
{
    int rc = 0;
    struct libusb *lib;

    lib = libusb_open();
    if (lib == NULL)
    {
        HI_ERR_PM("libubi_open error. \n");
        return -1;
    }

    rc = usb_get_remote(lib, intmask);
    libusb_close(lib);
    return rc;
}

/**
 * set_remotewakeup - set device to support remote wakeup.
 *
 * This function returns 0 in case of success and %-1 in case of failure.
 */
int set_remotewakeup(void)
{
    int rc = 0;
    struct libusb *lib;

    lib = libusb_open();
    if (lib == NULL)
    {
        HI_ERR_PM("libubi_open error \n");
        return -1;
    }

    rc = usb_set_remote(lib);
    libusb_close(lib);
    return rc;
}
