#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>
#include <linux/poll.h>
#include <linux/time.h>
#include <linux/netdevice.h>
#include <net/net_namespace.h>
#include <linux/string.h>
#include <mach/mt_boot.h>

#include <mach/mt_gpio.h>

MODULE_LICENSE("Dual BSD/GPL");

#define WIFI_DRIVER_NAME "mtk_WIFI_chrdev"
#define WIFI_NODE "wmtWifi"
#define WLAN_IFACE_NAME "wlan0"
#define WLAN_QUERYDEV_TIME 100

#define WIFI_DEV_MAJOR 0 // never used number

#define PFX                         "[MTK-WIFI] "
#define WIFI_LOG_DBG                  3
#define WIFI_LOG_INFO                 2
#define WIFI_LOG_WARN                 1
#define WIFI_LOG_ERR                  0


static unsigned int gDbgLevel = WIFI_LOG_INFO;

#define WIFI_DBG_FUNC(fmt, arg...)    if(gDbgLevel >= WIFI_LOG_DBG){ printk(PFX "%s: "  fmt, __FUNCTION__ ,##arg);}
#define WIFI_INFO_FUNC(fmt, arg...)   if(gDbgLevel >= WIFI_LOG_INFO){ printk(PFX "%s: "  fmt, __FUNCTION__ ,##arg);}
#define WIFI_WARN_FUNC(fmt, arg...)   if(gDbgLevel >= WIFI_LOG_WARN){ printk(PFX "%s: "  fmt, __FUNCTION__ ,##arg);}
#define WIFI_ERR_FUNC(fmt, arg...)    if(gDbgLevel >= WIFI_LOG_ERR){ printk(PFX "%s: "   fmt, __FUNCTION__ ,##arg);}
#define WIFI_TRC_FUNC(f)              if(gDbgLevel >= WIFI_LOG_DBG){printk(PFX "<%s> <%d>\n", __FUNCTION__, __LINE__);}

static int WIFI_devs = 1;        /* device count */
static int WIFI_major = WIFI_DEV_MAJOR;
struct class *pWIFIClass = NULL;
struct device *pWIFIDev = NULL;

module_param(WIFI_major, uint, 0);
static struct cdev WIFI_cdev;
//volatile int retflag = 0;
static struct semaphore wr_mtx;
static volatile int ready_to_reset = 1; // 0: isn't ready; 1:ready
static int nvm_from_user = 0;
extern unsigned char MT76x2_E2PImage[512];
/***************************************************************************
 *
 *	Platform HW Control
 *
 ***************************************************************************/
ssize_t WIFI_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

/***************************************************************************
 *
 *	MTK-WIFI Device Operations
 *
 ***************************************************************************/
static int WIFI_open(struct inode *inode, struct file *file)
{
    WIFI_INFO_FUNC("%s: major %d minor %d (pid %d)\n", __func__,
        imajor(inode),
        iminor(inode),
        current->pid
        );

    return 0;
}

static int WIFI_close(struct inode *inode, struct file *file)
{
    WIFI_INFO_FUNC("%s: major %d minor %d (pid %d)\n", __func__,
        imajor(inode),
        iminor(inode),
        current->pid
        );
//    retflag = 0;

    return 0;
}

ssize_t WIFI_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int retval = -EIO;
	unsigned char local[512] = {0};

	down(&wr_mtx);
	if (count <= 0) {
	    WIFI_ERR_FUNC("WIFI_write invalid param\n");
	    goto done;
	}
	memset(local, 0, 512);
	/* WIFI_INFO_FUNC("WIFI_write [%d]MT76x2_E2PImage%x %x\n",
	   count, MT76x2_E2PImage[0], MT76x2_E2PImage[1]); */
	if (copy_from_user(local, buf, (count > sizeof(local)) ? sizeof(local) : count)) {
		retval = -EFAULT;
		goto done;
	}
	if (local[0] == 'N' || local[1] == 'V' || local[2] == 'M') {
		if (nvm_from_user == 0)
			nvm_from_user = 1;
		else
			nvm_from_user = 0;
		retval = count;
		WIFI_INFO_FUNC("WIFI_write %s(en:%d)\n", local, nvm_from_user);
		goto done;
	}
	if (nvm_from_user == 1) {
		/* update nvram from user space, 32byte 1 times */
		memcpy(&MT76x2_E2PImage[0], &local[0], sizeof(local));
		retval = count;
	}

done:
    up(&wr_mtx);
    return (retval);
}


struct file_operations WIFI_fops = {
    .open = WIFI_open,
    .release = WIFI_close,
    .write = WIFI_write,
};

static int WIFI_init(void)
{
	dev_t devID = MKDEV(WIFI_major, 0);
	int alloc_ret = 0;
	int cdev_err = 0;

	/*static allocate chrdev*/
	alloc_ret = alloc_chrdev_region(&devID, 0, 1, WIFI_DRIVER_NAME);
	if (alloc_ret) {
        	WIFI_ERR_FUNC("fail to allocate chrdev\n");
        	return alloc_ret;
	}

	cdev_init(&WIFI_cdev, &WIFI_fops);
	WIFI_cdev.owner = THIS_MODULE;

	cdev_err = cdev_add(&WIFI_cdev, devID, WIFI_devs);
	if (cdev_err) {
		goto error;
	}
	pWIFIClass = class_create(THIS_MODULE, WIFI_DRIVER_NAME);
	if(IS_ERR(pWIFIClass))
	{
		WIFI_ERR_FUNC("class create fail, error code(%ld)\n",PTR_ERR(pWIFIClass));
		goto err1;
	}

	pWIFIDev = device_create(pWIFIClass,NULL,devID,NULL,WIFI_NODE);
	if(IS_ERR(pWIFIDev))
	{
		WIFI_ERR_FUNC("device create fail, error code(%ld)\n",PTR_ERR(pWIFIDev));
		goto err2;
	}
	sema_init(&wr_mtx, 1);

	 WIFI_INFO_FUNC("%s driver(major %d) installed.\n", WIFI_DRIVER_NAME, WIFI_major);
//    retflag = 0;

	return 0;
err2:
	if(pWIFIClass)
	{
		class_destroy(pWIFIClass);
		pWIFIClass = NULL;
	}

err1:
error:
    if (cdev_err == 0) {
        cdev_del(&WIFI_cdev);
    }

    if (alloc_ret == 0) {
        unregister_chrdev_region(devID, WIFI_devs);
    }

    return -1;
}

static void WIFI_exit(void)
{
	dev_t dev = MKDEV(WIFI_major, 0);
//    retflag = 0;
	if(pWIFIDev)
	{
		device_destroy(pWIFIClass, dev);
		pWIFIDev = NULL;
	}

	if(pWIFIClass)
	{
		class_destroy(pWIFIClass);
		pWIFIClass = NULL;
	}

	cdev_del(&WIFI_cdev);
	unregister_chrdev_region(dev, WIFI_devs);

	WIFI_INFO_FUNC("%s driver removed.\n", WIFI_DRIVER_NAME);
}

module_init(WIFI_init);
module_exit(WIFI_exit);
