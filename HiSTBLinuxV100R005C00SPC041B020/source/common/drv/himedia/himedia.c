/*
 * linux/drivers/char/himedia.c
 *
 * Generic misc open routine by Johan Myreen
 *
 * Based on code from Linus
 *
 * Teemu Rantanen's Microsoft Busmouse support and Derrick Cole's
 *   changes incorporated into 0.97pl4
 *   by Peter Cervasio (pete%q106fm.uucp@wupost.wustl.edu) (08SEP92)
 *   See busmouse.c for particulars.
 *
 * Made things a lot mode modular - easy to compile in just one or two
 * of the misc drivers, as they are now completely independent. Linus.
 *
 * Support for loadable modules. 8-Sep-95 Philip Blundell <pjb27@cam.ac.uk>
 *
 * Fixed a failing symbol register to free the device registration
 *		Alan Cox <alan@lxorguk.ukuu.org.uk> 21-Jan-96
 *
 * Dynamic minors and /proc/mice by Alessandro Rubini. 26-Mar-96
 *
 * Renamed to misc and miscdevice to be more accurate. Alan Cox 26-Mar-96
 *
 * Handling of mouse minor numbers for kerneld:
 *  Idea by Jacques Gelinas <jack@solucorp.qc.ca>,
 *  adapted by Bjorn Ekwall <bj0rn@blox.se>
 *  corrected by Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Changes for kmod (from kerneld):
 *	Cyrus Durgin <cider@speakeasy.org>
 *
 * Added devfs support. Richard Gooch <rgooch@atnf.csiro.au>  10-Jan-1998
 * Changed for hisilicon-media devices by Liu Jiandong 10-Dec-2007
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>

#include "himedia_base.h"
#include "hi_drv_proc.h"

#include <linux/delay.h>

/*
 * Head entry for the doubly linked himedia_device list
 */
static LIST_HEAD(himedia_list);
static DEFINE_MUTEX(himedia_sem);

/*
 * Assigned numbers, used for dynamic minors
 */
#define DYNAMIC_MINORS 128 /* like dynamic majors */
static unsigned char himedia_minors[DYNAMIC_MINORS / 8];

extern int pmu_device_init(void);

#ifdef CONFIG_PROC_FS
#if (1 == HI_PROC_SUPPORT)
static void *himedia_seq_start(struct seq_file *seq, loff_t *pos)
{
	mutex_lock(&himedia_sem);
	return seq_list_start(&himedia_list, *pos);
}

static void *himedia_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	return seq_list_next(v, &himedia_list, pos);
}

static void himedia_seq_stop(struct seq_file *seq, void *v)
{
	mutex_unlock(&himedia_sem);
}

static int himedia_seq_show(struct seq_file *seq, void *v)
{
	const PM_DEVICE_S *p = list_entry(v, PM_DEVICE_S, list);

	PROC_PRINT(seq, "%3i %s\n", p->minor, (char*)p->name ? (char*)p->name : "");
	return 0;
}



static struct seq_operations himedia_seq_ops = {
	.start = himedia_seq_start,
	.next  = himedia_seq_next,
	.stop  = himedia_seq_stop,

	.show  = himedia_seq_show,

};

static int himedia_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &himedia_seq_ops);
}

static struct file_operations himedia_proc_fops = {
	.owner	 = THIS_MODULE,
	.open    = himedia_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};

#endif
#endif
static int himedia_open(struct inode * inode, struct file * file)
{
	int minor = iminor(inode);
	PM_DEVICE_S *c;
	int err = -ENODEV;
	const struct file_operations *old_fops, *new_fops = NULL;
	
	mutex_lock(&himedia_sem);
	
	list_for_each_entry(c, &himedia_list, list) {
		if (c->minor == minor) {
			new_fops = fops_get(c->app_ops);		
			break;
		}
	}
		
	if (!new_fops) {
#if 0
		mutex_unlock(&himedia_sem);
		request_module("char-major-%d-%d", HIMEDIA_DEVICE_MAJOR, minor);
		mutex_lock(&himedia_sem);

		list_for_each_entry(c, &himedia_list, list) {
			if (c->minor == minor) {
				new_fops = fops_get(c->app_ops);
				break;
			}
		}
		if (!new_fops)
			goto fail;
#else
		goto fail;
#endif
	}

	err = 0;
	old_fops = file->f_op;
	file->f_op = new_fops;
	if (file->f_op->open) {
		err=file->f_op->open(inode,file);
		if (err) {
			fops_put(file->f_op);
			file->f_op = fops_get(old_fops);
		}
	}
	fops_put(old_fops);
fail:
	mutex_unlock(&himedia_sem);
	return err;
}

static struct class *himedia_class;

static struct file_operations himedia_fops = {
	.owner		= THIS_MODULE,
	.open		= himedia_open,
};


/**
 *	HI_DRV_PM_Register	-	register a himedia device
 *	@himedia: device structure
 *	
 *	Register a himedia device with the kernel. If the minor
 *	number is set to %HIMEDIA_DYNAMIC_MINOR a minor number is assigned
 *	and placed in the minor field of the structure. For other cases
 *	the minor number requested is used.
 *
 *	The structure passed is linked into the kernel and may not be
 *	destroyed until it has been unregistered.
 *
 *	A zero is returned on success and a negative errno code for
 *	failure.
 */


HI_S32 HI_DRV_PM_Register(PM_DEVICE_S * himedia)
{
	int ret;
	dev_t dev;
	PM_DEVICE_S  *c = NULL;
	struct device *adev = NULL;
	PM_BASEDEV_S *bdev = NULL;
	PM_BASEDRV_S *bdrv = NULL;

    if ( (himedia == NULL) || (himedia->name == NULL) || (himedia->app_ops == NULL) )
    {
		//HI_PRINT("%s args invalid \n", __FUNCTION__);
		return -EINVAL;
	}
    
#ifdef MODULE
    if (himedia->owner == NULL)
    {
        //HI_PRINT("%s args himedia->owner invalid \n", __FUNCTION__);
		return -EINVAL;
    }
#endif
	// 1
	mutex_lock(&himedia_sem);
	list_for_each_entry(c, &himedia_list, list) {
		if (c->minor == himedia->minor) {
			ret = -EBUSY;
			goto out;
		}
	}
	if (himedia->minor == HIMEDIA_DYNAMIC_MINOR) {
		int i = DYNAMIC_MINORS;
		while (--i >= 0)
			if ( (himedia_minors[i>>3] & (1 << (i&7))) == 0)
				break;
		if (i < 0) {
			ret = -EBUSY;
			goto out;
		}
		himedia->minor = i;
	}

	if (himedia->minor < DYNAMIC_MINORS)
		himedia_minors[himedia->minor >> 3] |= 1 << (himedia->minor & 7);

	// 2 base device, then class = NULL;
	bdev = himedia_device_alloc(himedia->name, -1);
	if (!bdev) {
		ret = -ENOMEM;
		goto err0;
	}
	ret = himedia_device_add(bdev);
	if (ret){
		himedia_device_put(bdev);
		goto err0;
	}

	// 3 app class
	dev = MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor);
	adev = device_create(himedia_class, &(bdev->dev), dev, NULL, 
						 "%s",  himedia->name);
	if (IS_ERR(adev)) {
		ret = PTR_ERR(adev);
		goto err1;
	}

	// 4 base driver
	bdrv = himedia_driver_alloc(himedia->name, himedia->owner, himedia->base_ops);
	if (!bdrv) {
		ret = -ENOMEM;
		goto err2;
	}

	ret = himedia_driver_register(bdrv);
	if(ret){
		himedia_driver_release(bdrv);
		goto err2;
	}

	himedia->app_device  = adev;
	himedia->base_device = bdev;
	himedia->base_driver = bdrv;
	
	/*
	 * Add it to the front, so that later devices can "override"
	 * earlier defaults
	 */
	INIT_LIST_HEAD(&himedia->list);
	list_add(&himedia->list, &himedia_list);
	goto out;


err2:
	device_destroy(himedia_class, dev);
err1:
	himedia_device_unregister(bdev);
err0:
	himedia_minors[himedia->minor >> 3] &= ~(1 << (himedia->minor & 7));
out:
	mutex_unlock(&himedia_sem);
	return ret;
}

/**
 *	HI_DRV_PM_UnRegister - unregister a himedia device
 *	@himedia: device to unregister
 *
 *	Unregister a himedia device that was previously
 *	successfully registered with HI_DRV_PM_Register(). Success
 *	is indicated by a zero return, a negative errno code
 *	indicates an error.
 */

HI_S32 HI_DRV_PM_UnRegister(PM_DEVICE_S * himedia)
{
	int i = 0;

    if (himedia_class == NULL || himedia == NULL || (himedia->name == NULL)||(himedia->owner == NULL) || (himedia->app_ops == NULL))
    {
        return -EINVAL;
    }

    i = himedia->minor;
	// 0
	if (list_empty(&himedia->list))
		return -EINVAL;
	if ( (i >= DYNAMIC_MINORS) || (i < 0)) {
		//HI_PRINT("%s himedia->minor: %d  invalid \n", __FUNCTION__,  himedia->minor);
		return -EINVAL;
	}
	// 1
	mutex_lock(&himedia_sem);
	// 1.0
	list_del(&himedia->list);
	// 1.1
	if(himedia->base_driver){
		himedia_driver_unregister(himedia->base_driver);
		himedia_driver_release(himedia->base_driver);
		himedia->base_driver = NULL;
	}
	// 1.2
	if(himedia->app_device){
		device_destroy(himedia_class, MKDEV(HIMEDIA_DEVICE_MAJOR, himedia->minor));
		himedia->app_device = NULL;
	}
	// 1.3
	if(himedia->base_device){
		himedia_device_unregister(himedia->base_device);
		himedia->base_device = NULL;
	}
	// 1.4
	himedia_minors[i>>3] &= ~(1 << (i & 7));
	mutex_unlock(&himedia_sem);
	return 0;
}


HI_S32 DRV_PM_ModInit(HI_VOID)
{
	int ret;
	// 0
#if !(0 == HI_PROC_SUPPORT)
	proc_create("himedia", 0, NULL, &himedia_proc_fops);
#endif
	// 1
	ret = himedia_bus_init();
	if(ret)
		goto err0;
	// 2
	himedia_class = class_create(THIS_MODULE, "himediaClass");
	ret = PTR_ERR(himedia_class);
	if (IS_ERR(himedia_class))
		goto err1;
	// 3
	ret = -EIO;
	if (register_chrdev(HIMEDIA_DEVICE_MAJOR, "himediaCharDev", &himedia_fops))
		goto err2;
#if defined(MODULE)
    HI_PRINT("Load hi_media.ko success.\t(%s)\n", VERSION_STRING);
#endif
	return 0;
	// 4
err2:

	HI_PRINT("!!! Module himedia: unable to get major %d for himedia devices\n", HIMEDIA_DEVICE_MAJOR);

	class_destroy(himedia_class);
err1:
	himedia_bus_exit();
err0:
#if !(0 == HI_PROC_SUPPORT)
	remove_proc_entry("himedia", NULL);
#endif
	return ret;
}

HI_VOID DRV_PM_ModExit(HI_VOID)
{
	// 0
	if (list_empty(&himedia_list) == 0)
	{
		//HI_PRINT("!!! Module himedia: there module in list\n");
		return;
	}
	// 1
	unregister_chrdev(HIMEDIA_DEVICE_MAJOR, "himedia");
	// 2
	class_destroy(himedia_class);
	// 3
	himedia_bus_exit();
	// 4
#if !(0 == HI_PROC_SUPPORT)
	remove_proc_entry("himedia", NULL);
#endif
#if defined(MODULE)
    HI_PRINT("remove hi_media.ko success.\n");
#endif
	return;
}



#ifdef MODULE
module_init(DRV_PM_ModInit);
module_exit(DRV_PM_ModExit);
#endif

EXPORT_SYMBOL(HI_DRV_PM_Register);
EXPORT_SYMBOL(HI_DRV_PM_UnRegister);
EXPORT_SYMBOL(DRV_PM_ModInit);
EXPORT_SYMBOL(DRV_PM_ModExit);


MODULE_LICENSE("GPL");
