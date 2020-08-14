#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/setup.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/semaphore.h>
#include <linux/vinput.h>

#define MAX_FINGER_NUM   5

DEFINE_SEMAPHORE(input_mutex);

static struct input_dev *vinput_kbd_dev = NULL;
static struct input_dev *vinput_mouse_dev = NULL;
static struct input_dev *vinput_tc_dev = NULL;
static struct input_dev *vinput_multi_tc_dev = NULL;

extern void input_block_status(int);

struct finger_info {
	int x;
	int y;
	int press;
};

struct multi_touch_info {
	int fingernr;
	struct finger_info finger[MAX_FINGER_NUM];
};

static int __init vinput_multi_tc_init(void)
{
	int err;

	vinput_multi_tc_dev = input_allocate_device();
	if (!vinput_multi_tc_dev)
		return -ENOMEM;

	vinput_multi_tc_dev->name = "qwerty";

	vinput_multi_tc_dev->id.product = 0;
	vinput_multi_tc_dev->id.version = 0x0100;

	set_bit(EV_SYN, vinput_multi_tc_dev->evbit);
	set_bit(EV_KEY, vinput_multi_tc_dev->evbit);
	set_bit(EV_ABS, vinput_multi_tc_dev->evbit);

	set_bit(BTN_TOUCH, vinput_multi_tc_dev->keybit);

	input_mt_init_slots(vinput_multi_tc_dev, MAX_FINGER_NUM, 0);

	input_set_abs_params(vinput_multi_tc_dev, ABS_X, 0, 1280, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev, ABS_Y, 0, 720, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev, ABS_PRESSURE, 0, 255, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev, ABS_TOOL_WIDTH, 0, 15, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev,
			     ABS_MT_POSITION_X, 0, 1280, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev,
			     ABS_MT_POSITION_Y, 0, 720, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev,
			     ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(vinput_multi_tc_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 15, 0, 0);

	err = input_register_device(vinput_multi_tc_dev);
	if (err) {
		input_free_device(vinput_multi_tc_dev);
		return err;
	}

	return 0;
}

void vinput_multi_tc_event_report(void *data)
{
	struct multi_touch_info *info = NULL;
	int i = 0;
	info = (struct multi_touch_info *)data;

	if (info->fingernr > 0) {
		if (info->fingernr > MAX_FINGER_NUM)
			info->fingernr = MAX_FINGER_NUM;

		for (i = 0; i < info->fingernr; i++) {
			input_mt_slot(vinput_multi_tc_dev, i);
			if (1 == info->finger[i].press) {
				input_mt_report_slot_state(vinput_multi_tc_dev,
							   MT_TOOL_FINGER,
							   true);
				input_report_key(vinput_multi_tc_dev,
						 ABS_MT_TRACKING_ID, i);
				input_report_abs(vinput_multi_tc_dev,
						 ABS_MT_POSITION_X,
						 info->finger[i].x);
				input_report_abs(vinput_multi_tc_dev,
						 ABS_MT_POSITION_Y,
						 info->finger[i].y);
			} else {
				input_mt_report_slot_state(vinput_multi_tc_dev,
							   MT_TOOL_FINGER,
							   false);
			}
		}

		if (1 == info->fingernr) {
			input_mt_slot(vinput_multi_tc_dev, 1);
			input_mt_report_slot_state(vinput_multi_tc_dev,
						   MT_TOOL_FINGER, false);
		}

		input_mt_report_pointer_emulation(vinput_multi_tc_dev, true);
		input_sync(vinput_multi_tc_dev);
	}
}

static void vinput_multi_tc_exit(void)
{
	if (!vinput_multi_tc_dev)
		input_unregister_device(vinput_multi_tc_dev);
}

#if 0 /* for feature support */
static int __init vinput_tc_init(void)
{
	int err;

	vinput_tc_dev = input_allocate_device();
	if (!vinput_tc_dev)
		return -ENOMEM;

	vinput_tc_dev->name = "Hi touch";

	vinput_tc_dev->id.product = 0;
	vinput_tc_dev->id.version = 0x0100;

	set_bit(EV_KEY, vinput_tc_dev->evbit);
	set_bit(EV_ABS, vinput_tc_dev->evbit);
	set_bit(EV_SYN, vinput_tc_dev->evbit);

	set_bit(BTN_TOUCH, vinput_tc_dev->keybit);

	input_set_abs_params(vinput_tc_dev, ABS_X, 0, 1280, 0, 0);
	input_set_abs_params(vinput_tc_dev, ABS_Y, 0, 720, 0, 0);
	input_set_abs_params(vinput_tc_dev, ABS_PRESSURE, 0, 1, 0, 0);

	err = input_register_device(vinput_tc_dev);
	if (err) {
		input_free_device(vinput_tc_dev);
		return err;
	}

	return 0;
}

static void __exit vinput_tc_exit(void)
{
	if (!vinput_tc_dev)
		input_unregister_device(vinput_tc_dev);
}
#endif

void vinput_tc_event_report(int *data)
{
	int x = 0, y = 0, pres = 0;

	x = data[0];
	y = data[1];
	pres = data[2];

	input_report_abs(vinput_tc_dev, ABS_X, x);

	input_report_abs(vinput_tc_dev, ABS_Y, y);

	input_report_key(vinput_tc_dev, BTN_TOUCH, pres);

	input_report_abs(vinput_tc_dev, ABS_PRESSURE, pres);

	input_sync(vinput_tc_dev);
}

static int __init vinput_kbd_init(void)
{
	int error;
	int iKey;

	vinput_kbd_dev = input_allocate_device();
	if (!vinput_kbd_dev) {
		printk(KERN_ERR "v input: Not enough memory\n");
		return -ENOMEM;
	}

	set_bit(EV_KEY, vinput_kbd_dev->evbit);
	vinput_kbd_dev->evbit[0] = BIT_MASK(EV_KEY);
	for (iKey = KEY_ESC; iKey <= KEY_MAX; iKey++) {
		set_bit(iKey, vinput_kbd_dev->keybit);
	}

	vinput_kbd_dev->name = "Hi keyboard";
	vinput_kbd_dev->id.vendor = 0x0001;
	vinput_kbd_dev->id.product = 0x0001;
	vinput_kbd_dev->id.version = 0x0100;

	error = input_register_device(vinput_kbd_dev);
	if (error) {
		printk(KERN_ERR "v input: Failed to register device\n");
		goto fail;
	}
	return 0;
fail:
	input_free_device(vinput_kbd_dev);
	return error;
}

static void vinput_kbd_exit(void)
{
	if (!vinput_kbd_dev)
		input_unregister_device(vinput_kbd_dev);
}

void vinput_kbd_report(int value, int status)
{
	input_report_key(vinput_kbd_dev, value, status);
	input_sync(vinput_kbd_dev);
}

static int __init vinput_mouse_init(void)
{
	int err;

	vinput_mouse_dev = input_allocate_device();
	if (!vinput_mouse_dev)
		return -ENOMEM;

	vinput_mouse_dev->name = "Hi mouse";
	vinput_mouse_dev->id.vendor = 0x0001;
	vinput_mouse_dev->id.product = 0x0002;
	vinput_mouse_dev->id.version = 0x0100;

	set_bit(EV_KEY, vinput_mouse_dev->evbit);
	set_bit(EV_REL, vinput_mouse_dev->evbit);

	set_bit(REL_X, vinput_mouse_dev->relbit);
	set_bit(REL_Y, vinput_mouse_dev->relbit);
	set_bit(REL_WHEEL, vinput_mouse_dev->relbit);

	set_bit(BTN_LEFT, vinput_mouse_dev->keybit);
	set_bit(BTN_MIDDLE, vinput_mouse_dev->keybit);
	set_bit(BTN_RIGHT, vinput_mouse_dev->keybit);
	set_bit(BTN_SIDE, vinput_mouse_dev->keybit);
	set_bit(BTN_EXTRA, vinput_mouse_dev->keybit);

	err = input_register_device(vinput_mouse_dev);
	if (err) {
		input_free_device(vinput_mouse_dev);
		return err;
	}

	return 0;
}

void vinput_mouse_event_report(int *data)
{
	input_report_key(vinput_mouse_dev, BTN_LEFT, data[0] & 0x01);
	input_report_key(vinput_mouse_dev, BTN_RIGHT, data[0] & 0x02);
	input_report_key(vinput_mouse_dev, BTN_MIDDLE, data[0] & 0x04);
	input_report_key(vinput_mouse_dev, BTN_SIDE, data[0] & 0x08);
	input_report_key(vinput_mouse_dev, BTN_EXTRA, data[0] & 0x10);
	input_report_rel(vinput_mouse_dev, REL_X, data[1]);
	input_report_rel(vinput_mouse_dev, REL_Y, data[2]);
	input_report_rel(vinput_mouse_dev, REL_WHEEL, data[3]);
	input_sync(vinput_mouse_dev);
}

static void vinput_mouse_exit(void)
{
	if (!vinput_mouse_dev)
		input_unregister_device(vinput_mouse_dev);
}

static int vinput_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int vinput_close(struct inode *inode, struct file *file)
{
	return 0;
}

static long vinput_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	int data[4] = { 0 };
	struct multi_touch_info info = { 0 };

	switch (cmd) {
	case IOCTL_MOUSE_STATUS:
		if (down_interruptible(&input_mutex))
			return -ERESTARTSYS;
		ret = copy_from_user(data, (int *)arg, sizeof(data));
		vinput_mouse_event_report(data);
		up(&input_mutex);
		return ret;

	case IOCTK_KBD_STATUS:
		if (down_interruptible(&input_mutex))
			return -ERESTARTSYS;

		ret = copy_from_user(data, (int *)arg, sizeof(data));
		vinput_kbd_report(data[0], data[1]);
		up(&input_mutex);
		return ret;

	case IOCTK_TC_STATUS:
		if (down_interruptible(&input_mutex))
			return -ERESTARTSYS;
		ret = copy_from_user(data, (int *)arg, sizeof(data));
		vinput_tc_event_report(data);
		up(&input_mutex);
		return ret;

	case IOCTK_MUTITC_STATUS:
		if (down_interruptible(&input_mutex))
			return -ERESTARTSYS;
		ret = copy_from_user(&info, (struct multi_touch_info *)arg,
				     sizeof(info));
		vinput_multi_tc_event_report(&info);
		up(&input_mutex);
		return ret;
	default:
		printk(KERN_DEBUG "Error: Inappropriate ioctl "
		       "for device. cmd=%d\n", cmd);
		return -ENOTTY;
	}

	return -EINVAL;
}

/* The various file operations we support.*/
static struct file_operations vinput_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = vinput_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = vinput_ioctl,
#endif
	.open = vinput_open,
	.release = vinput_close
};

static struct miscdevice vinput_dev = {
	MISC_DYNAMIC_MINOR,
	"vinput",
	&vinput_fops,
};

static int input_probe(struct platform_device *pdev)
{
	int ret = 0;

	ret = misc_register(&vinput_dev);
	if (ret) {
		printk(KERN_ERR "could not register vinput devices. \n");
		return ret;
	}

	ret = vinput_kbd_init();
	if (ret != 0) {
		printk(KERN_ERR "could not register vinput kbd devices. \n");
		goto fail;
	}

	ret = vinput_mouse_init();
	if (ret != 0) {
		printk(KERN_ERR "could not register vinput mouse devices. \n");
		vinput_kbd_exit();
		goto fail;
	}

	ret = vinput_multi_tc_init();
	if (ret != 0) {
		printk(KERN_ERR
		       "could not register vinput MultiTc devices. \n");
		vinput_mouse_exit();
		vinput_kbd_exit();
		goto fail;
	}

	printk(KERN_INFO "Hi Android vinput driver init successful!\n");
	return 0;

fail:
	misc_deregister(&vinput_dev);
	return ret;

}

static int input_close(struct platform_device *pdev)
{
	vinput_kbd_exit();
	vinput_mouse_exit();
	vinput_multi_tc_exit();
	misc_deregister(&vinput_dev);

	return 0;
}

#ifdef CONFIG_PM
static int input_suspend(struct platform_device *pdev, pm_message_t state)
{
	return 0;
}

static int input_resume(struct platform_device *pdev)
{
	input_block_status(INPUT_UNBLOCK);
	input_report_key(vinput_kbd_dev, KEY_WAKEUP, 0x01);
	input_report_key(vinput_kbd_dev, KEY_WAKEUP, 0x00);
	input_sync(vinput_kbd_dev);
	input_block_status(INPUT_BLOCK);
	return 0;
}
#endif /* CONFIG_PM */

static struct platform_driver input_driver = {
	.probe = input_probe,
	.remove = input_close,
#ifdef CONFIG_PM
	.suspend = input_suspend,
	.resume = input_resume,
#endif /* CONFIG_PM */
	.driver = {
		   .name = "Input_For_Android",
		   .owner = THIS_MODULE,
		   },
};

static void input_dev_release(struct device *dev)
{
}

static struct platform_device input_dev = {
	.name = "Input_For_Android",
	.id = 0,
	.dev = {
		.platform_data = NULL,
		.dma_mask = (u64 *) ~ 0,
		.coherent_dma_mask = (u64) ~ 0,
		.release = input_dev_release,
	},
};

static int __init vinput_init(void)
{
	int retval;

	retval = platform_device_register(&input_dev);
	if (retval)
		return retval;

	retval = platform_driver_register(&input_driver);
	if (retval)
		goto fail2;

	return retval;

fail2:
	platform_device_unregister(&input_dev);

	return -1;
}

static void __exit vinput_exit(void)
{
	platform_driver_unregister(&input_driver);
	platform_device_unregister(&input_dev);
}

MODULE_DESCRIPTION("Hi vinput(mouse keyboard tc) driver");

module_init(vinput_init);
module_exit(vinput_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hisilicon");
