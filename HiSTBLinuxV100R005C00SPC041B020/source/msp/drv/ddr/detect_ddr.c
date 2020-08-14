#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/hikapi.h>

int get_mem_size(unsigned int *size, int flags);

static ssize_t ddr_read(struct file *filp, char __user *buf,
                size_t count, loff_t *f_pos)
{
    unsigned int s;

    if (get_mem_size(&s, HIKAPI_GET_RAM_SIZE))
	return -1;

    if(copy_to_user(buf, &s, sizeof(s)))
        return -1;

    printk("Total ddr size :%u MByte!\n", s);

    return sizeof(s);
}

static struct file_operations ddr_fops =
{
        .owner  = THIS_MODULE,
        .read   = ddr_read,
};

static struct miscdevice ddr_misc = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "ddr_detect",
        .fops = &ddr_fops,
};

static int ddr_detect_init(void)
{
    if(misc_register(&ddr_misc))
        return -1;
    return 0;
}

static void ddr_detect_exit(void)
{
    misc_deregister(&ddr_misc);
}

module_init(ddr_detect_init);
module_exit(ddr_detect_exit);
MODULE_LICENSE("GPL");
