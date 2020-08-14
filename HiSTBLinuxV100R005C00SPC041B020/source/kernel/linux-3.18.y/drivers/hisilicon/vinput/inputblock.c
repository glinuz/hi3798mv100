#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/vinput.h>
#define MOTION_PROC_DIR  "inputblock"
#define MOTION_PROC_FILE "status"

static char state_buff[5];
static struct proc_dir_entry *proc_dir;
extern void input_block_status(int);

static ssize_t event_write(struct file *filp, const char __user *buffer,
			   size_t count, loff_t *ppos)
{
	char *options;
	size_t num = count;

	if (count > PAGE_SIZE)
		num = (PAGE_SIZE - 1);

	options = (char *)__get_free_page(GFP_TEMPORARY);
	if (!options)
		return -ENOMEM;

	if (copy_from_user(options, buffer, num)) {
		free_page((unsigned long) options);
		return -EFAULT;
	}   

	memset(state_buff, 0, sizeof(state_buff));

	if (options[0] == state_buff[0])
		return count;

	if (options[0] == '1') {
		input_block_status(INPUT_BLOCK);
		printk(KERN_INFO "inputblock: enable block\n");
		state_buff[0] = '1';
	} else if (options[0] == '0') {
		input_block_status(INPUT_UNBLOCK);
		printk(KERN_INFO "inputblock: disable block\n");
		state_buff[0] = '0';
	}else if (options[0] == '2') {
		input_block_status(INPUT_HALFBLOCK);
		printk(KERN_INFO "inputblock: half enable block\n");
		state_buff[0] = '2';
	}

	return count;
}

static ssize_t event_read(struct file *filp, char __user *buffer,
			  size_t count, loff_t *ppos)
{
	char *msg, *p;
	
	if (*ppos != 0)
		return 0;

	msg = (char *)__get_free_page(GFP_TEMPORARY);
	if (!msg)
		return -ENOMEM;

	p = msg;
	if (count > PAGE_SIZE)
		count = PAGE_SIZE;

	p += sprintf(p, "%s\n", state_buff);
	count = (p - msg);
	if (copy_to_user(buffer, msg, count)) {
		free_page((unsigned long) msg);
		return -EFAULT;
	}

	free_page((unsigned long) msg);
	*ppos += count;
	return count;
}

static const struct file_operations inputblock_fops = {
	.owner = THIS_MODULE,
	.read = event_read,
	.write = event_write,
};

static int __init inputblock_init(void)
{
	state_buff[0] = '0';

	proc_dir = proc_mkdir(MOTION_PROC_DIR, NULL);
	proc_create(MOTION_PROC_FILE, 0, proc_dir, &inputblock_fops);

	return 0;
}

static void __exit inputblock_exit(void)
{
	if (proc_dir) {
		remove_proc_entry(MOTION_PROC_FILE, proc_dir);
		remove_proc_entry(MOTION_PROC_DIR, NULL);
	}
}

module_init(inputblock_init);
module_exit(inputblock_exit);
MODULE_LICENSE("GPL");
