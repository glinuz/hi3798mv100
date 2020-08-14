/******************************************************************************
 *  Copyright (C) 2016 Cai Zhiyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiying 2016.2.3
 *
******************************************************************************/

#define pr_fmt(fmt) "hw_breakpoint: " fmt

#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/kallsyms.h>

#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/hw_breakpoint.h>
#include <linux/kthread.h>

struct bpinfo {
#define BP_NULL      0
#define BP_WAIT      1
#define BP_ADDING    2
#define BP_DELING    3
#define BP_CHANGING  4
	int state;
	spinlock_t lock;
	ulong bp_addr;
	__u32 bp_type;
#define BP_MAX_TYPE_STR        (4)
	char bp_type_str[BP_MAX_TYPE_STR];

#define BP_MAX_SYM             (128)
	char bp_sym[BP_MAX_SYM];

	struct perf_event * __percpu *perf_event;
	struct perf_event_attr attr;
};

struct bpsem {
	raw_spinlock_t lock;
	int count;
	struct task_struct *task;
};

static struct bpinfo __bpinfo[ARM_MAX_BRP];
static unsigned int __nr_bpinfo = 0;
static struct dentry *__bp_entry = NULL;
static int __bp_init = 0;
static struct bpsem __bpsem;

/******************************************************************************/

static int __wait(struct bpsem *sem)
{
	int need_sleep;
	unsigned long flags;

	raw_spin_lock_irqsave(&sem->lock, flags);
	if (likely(sem->count > 0)) {
		sem->count--;
		need_sleep = 0;
	} else {
		__set_task_state(sem->task, TASK_UNINTERRUPTIBLE);
		need_sleep = 1;
	}
	raw_spin_unlock_irqrestore(&sem->lock, flags);

	if (!need_sleep)
		return 0;

	schedule_timeout(HZ);

	return -1;
}
/******************************************************************************/

static void __wakeup(struct bpsem *sem)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&sem->lock, flags);
	sem->count++;
	wake_up_process(sem->task);
	raw_spin_unlock_irqrestore(&sem->lock, flags);
}
/******************************************************************************/

static char *parse_bpinfo_type(char *s, __u32 *bp_type, char *bp_type_str)
{
	int ix = 0;
	__u32 __bp_type = 0;
	char __bp_type_str[BP_MAX_TYPE_STR] = {0};

	while (isalpha(*s)) {
		if (*s == 'r') {
			if (!(__bp_type & HW_BREAKPOINT_R)) {
				__bp_type |= HW_BREAKPOINT_R;
				__bp_type_str[ix++] = *s;
			}
		} else if (*s == 'w') {
			if (!(__bp_type & HW_BREAKPOINT_W)) {
				__bp_type |= HW_BREAKPOINT_W;
				__bp_type_str[ix++] = *s;
			}
		} else if (*s == 'x') {
			if (!(__bp_type & HW_BREAKPOINT_X)) {
				__bp_type |= HW_BREAKPOINT_X;
				__bp_type_str[ix++] = *s;
			}
		} else {
			pr_err("unknown type character:%c\n", *s);
			return NULL;
		}
		s++;
	}

	if (bp_type)
		*bp_type = __bp_type;

	if (bp_type_str)
		memcpy(bp_type_str, __bp_type_str, sizeof(__bp_type_str));

	return s;
}
/******************************************************************************/

static char *parse_bpinfo_symbol(char *s, char *bp_sym, int count)
{
	while (count-- > 0) {
		if (*s == '\0' || *s == ',' || *s == ';') {
			*bp_sym = '\0';
			return s;
		}

		*bp_sym++ = *s++;
	}

	pr_err("symbol too long, it should shorter than %d.\n", BP_MAX_SYM);

	return NULL;
}
/******************************************************************************/

static char *parse_one_bpinfo(struct bpinfo *bpi, char *s)
{
	if (isalpha(*s) || *s == '_') {
		s = parse_bpinfo_symbol(s, bpi->bp_sym, sizeof(bpi->bp_sym));
		if (!s)
			return NULL;
		bpi->bp_addr = 0;
	} else if (isalnum(*s)) {
		bpi->bp_sym[0] = '\0';
		bpi->bp_addr = (ulong)simple_strtoll(s, &s, 16);
	} else {
		pr_err("no found symbol or address.\n");
		return NULL;
	}

	bpi->bp_type = 0;

	if (*s == ',') {
		s = parse_bpinfo_type(++s, &bpi->bp_type, bpi->bp_type_str);
		if (!s)
			return NULL;
	}

	if (*s == ';') {
		return ++s;
	} else if (*s == '\0') {
		return s;
	} else {
		pr_err("format error.\n");
		return NULL;
	}
}
/******************************************************************************/
/*
 *    breakpoint=<address|symbol>,<r><w><x>;<address|symbol>,<r><w><x>;
 */
static int __init hw_breakpoint_bootargs_setup(char *s)
{
	struct bpinfo *bpinfo;

	__bp_init = 1;

	while (*s) {
		if (__nr_bpinfo >= ARRAY_SIZE(__bpinfo))
			break;

		bpinfo = &__bpinfo[__nr_bpinfo];

		memset(bpinfo, 0, sizeof(struct bpinfo));

		s = parse_one_bpinfo(bpinfo, s);
		if (!s)
			break;

		__nr_bpinfo++;
	}

	return 1;
}
__setup("breakpoint=", hw_breakpoint_bootargs_setup);
/******************************************************************************/

void remove_hw_breakpoint_async(struct perf_event *bf_event, struct bpinfo *bpi)
{
	ulong flags;

	if (!__bp_init)
		return;

	spin_lock_irqsave(&bpi->lock, flags);
	if (bpi->state == BP_WAIT)
		bpi->state = BP_DELING;
	spin_unlock_irqrestore(&bpi->lock, flags);

	arch_uninstall_hw_breakpoint(bf_event);

	__wakeup(&__bpsem);
}
/******************************************************************************/

static struct bpinfo *get_new_bpinfo(void)
{
	int ix;
	ulong flags;
	struct bpinfo *bpi;

	for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
		bpi = &__bpinfo[ix];

		spin_lock_irqsave(&bpi->lock, flags);
		if (bpi->state == BP_NULL) {
			bpi->state = BP_CHANGING;
			spin_unlock_irqrestore(&bpi->lock, flags);

			bpi->bp_sym[0] = '\0';
			bpi->bp_addr = 0;
			bpi->bp_type = 0;
			bpi->bp_type_str[0] = '\0';

			return bpi;
		}
		spin_unlock_irqrestore(&bpi->lock, flags);
	}

	return NULL;
}
/******************************************************************************/

void add_hw_breakpoint_async(char *bp_sym, ulong bp_addr, char *type)
{
	ulong flags;
	struct bpinfo *bpi;

	if (!__bp_init)
		return;

	bpi = get_new_bpinfo();
	if (!bpi)
		return;

	if (bp_sym && bp_sym[0]) {
		strncpy(bpi->bp_sym, bp_sym, sizeof(bpi->bp_sym));
		bpi->bp_sym[sizeof(bpi->bp_sym) - 1] = '\0';
	} else {
		bpi->bp_sym[0] = '\0';
	}

	bpi->bp_addr = bp_addr;

	parse_bpinfo_type(type, &bpi->bp_type, bpi->bp_type_str);

	spin_lock_irqsave(&bpi->lock, flags);
	bpi->state = BP_ADDING;
	spin_unlock_irqrestore(&bpi->lock, flags);

	__wakeup(&__bpsem);
}
/******************************************************************************/

static void hw_break_handler(struct perf_event *bf_event,
			     struct perf_sample_data *data, struct pt_regs *regs)
{
	struct bpinfo *bpi = (struct bpinfo *)bf_event->overflow_handler_context;

	remove_hw_breakpoint_async(bf_event, bpi);

	printk(KERN_INFO "CPU%d %s@%#lx(%s) meet hw_breakpoint.\n",
		smp_processor_id(), bpi->bp_sym, bpi->bp_addr, bpi->bp_type_str);

	show_regs(regs);

	printk(KERN_INFO "----------[stop %s@%#lx hw_breakpoint]----------\n", bpi->bp_sym, bpi->bp_addr);
}
/******************************************************************************/

static int __add_hw_breakpoint(struct bpinfo *bpi)
{
	struct perf_event_attr *attr = &bpi->attr;

	hw_breakpoint_init(attr);

	attr->bp_addr = (__u64)bpi->bp_addr;
	attr->bp_type = bpi->bp_type;
	attr->bp_len = HW_BREAKPOINT_LEN_4;

	bpi->perf_event = register_wide_hw_breakpoint(attr, hw_break_handler, bpi);
	if (IS_ERR((void __force *)bpi->perf_event)) {
		char *errstr = "";
		long errno = PTR_ERR((void __force *)bpi->perf_event);

		switch (errno) {
		case -EACCES:
			errstr = "Permission denied";
			break;
		case -EINVAL:
			errstr = "Invalid argument";
			break;
		}
		pr_warn("%s@%#lx(%s) register breakpoint failed. errno:%ld %s\n",
			bpi->bp_sym, bpi->bp_addr, bpi->bp_type_str, errno, errstr);
		return PTR_ERR((void __force *)bpi->perf_event);
	}

	pr_info("%s@%#lx(%s) register breakpoint success.\n",
		bpi->bp_sym, bpi->bp_addr, bpi->bp_type_str);

	return 0;
}
/******************************************************************************/

static void remove_all_hw_breakpoint(void)
{
	int ix;
	ulong flags;
	struct bpinfo *bpi;

	for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
		bpi = &__bpinfo[ix];

		spin_lock_irqsave(&bpi->lock, flags);
		if (bpi->state == BP_WAIT) {
			bpi->state = BP_CHANGING;
			spin_unlock_irqrestore(&bpi->lock, flags);

			unregister_wide_hw_breakpoint(bpi->perf_event);

			spin_lock_irqsave(&bpi->lock, flags);
			bpi->state = BP_NULL;
		}
		spin_unlock_irqrestore(&bpi->lock, flags);
	}
}
/******************************************************************************/

static ssize_t bp_fops_read(struct file *filp, char __user *ubuf,
			    size_t sz_ubuf, loff_t *ppos)
{
	int ix;
	int ret;
	int leave = PAGE_SIZE;
	char *buf;
	char *ptr;
	struct bpinfo *bpi;
	unsigned long flags;
	ulong bp_addr;
	__u32 bp_type;
	char bp_sym[BP_MAX_SYM];

	ptr = buf = kmalloc(leave, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
		bpi = &__bpinfo[ix];

		spin_lock_irqsave(&bpi->lock, flags);
		if (bpi->state == BP_WAIT) {
			bp_addr = bpi->bp_addr;
			bp_type = bpi->bp_type;
			strncpy(bp_sym, bpi->bp_sym, sizeof(bp_sym));
			bp_sym[sizeof(bp_sym) - 1] = '\0';

			spin_unlock_irqrestore(&bpi->lock, flags);

			ret = snprintf(ptr, leave, "%c%c%c %s@%#lx\n",
				(bp_type & HW_BREAKPOINT_R) ? 'r' : '-',
				(bp_type & HW_BREAKPOINT_W) ? 'w' : '-',
				(bp_type & HW_BREAKPOINT_X) ? 'x' : '-',
				bp_sym, bp_addr);

			ptr += ret;
			leave -= ret;

			spin_lock_irqsave(&bpi->lock, flags);
		}
		spin_unlock_irqrestore(&bpi->lock, flags);
	}

	if (leave == PAGE_SIZE) {
		ret = snprintf(ptr, leave,
			"usage: <symbol|address,<r><w><x>>;<symbol|address,<r><w><x>>;\n"
			"sample:\n"
			"    echo \"do_fork,x\" > breakpoint  // add a breakpoint\n"
			"    echo \"do_fork\" > breakpoint    // remove a breakpoint\n"
			"    echo \"clean\" > breakpoint      // remove all breakpoint\n");
		ptr += ret;
		leave -= ret;
	}

	ret = simple_read_from_buffer(ubuf, sz_ubuf, ppos, buf, ptr - buf);

	kfree(buf);
	return ret;
}
/******************************************************************************/

int remove_hw_breakpoint(char *bp_sym, ulong bp_addr)
{
	int ix;
	ulong flags;
	struct bpinfo *bpi;

	if (!__bp_init)
		return 0;

	if (bp_sym && bp_sym[0]) {
		bp_addr = kallsyms_lookup_name(bp_sym);
		if (!bp_addr) {
			pr_warn("not found symbol: %s\n", bp_sym);
			return -EINVAL;
		}
	}

	for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
		bpi = &__bpinfo[ix];

		spin_lock_irqsave(&bpi->lock, flags);
		if (bpi->state == BP_WAIT && bpi->bp_addr == bp_addr) {
			bpi->state = BP_CHANGING;
			spin_unlock_irqrestore(&bpi->lock, flags);

			unregister_wide_hw_breakpoint(bpi->perf_event);

			spin_lock_irqsave(&bpi->lock, flags);
			bpi->state = BP_NULL;
			spin_unlock_irqrestore(&bpi->lock, flags);

			return 0;
		}
		spin_unlock_irqrestore(&bpi->lock, flags);
	}

	return -EINVAL;
}
/******************************************************************************/

int add_hw_breakpoint(char *bp_sym, ulong bp_addr, char *type)
{
	int ret;
	ulong flags;
	__u32 bp_type;
	char bp_type_str[BP_MAX_TYPE_STR] = {0};
	struct bpinfo *bpi;

	if (!__bp_init)
		return 0;

	if (bp_sym && bp_sym[0]) {
		bp_addr = kallsyms_lookup_name(bp_sym);
		if (!bp_addr) {
			pr_warn("not found symbol: %s\n", bp_sym);
			return -EINVAL;
		}
	}

	if (!parse_bpinfo_type(type, &bp_type, bp_type_str)) {
		pr_warn("type format invalid.\n");
		return -EINVAL;
	}

	bpi = get_new_bpinfo();
	if (!bpi) {
		pr_warn("not enough space.\n");
		return -ENOMEM;
	}

	if (bp_sym && bp_sym[0]) {
		strncpy(bpi->bp_sym, bp_sym, sizeof(bpi->bp_sym));
		bpi->bp_sym[sizeof(bpi->bp_sym) - 1] = '\0';
	}

	bpi->bp_addr = bp_addr;
	bpi->bp_type = bp_type;
	memcpy(bpi->bp_type_str, bp_type_str, sizeof(bp_type_str));

	ret = __add_hw_breakpoint(bpi);

	spin_lock_irqsave(&bpi->lock, flags);
	bpi->state = ret ? BP_NULL: BP_WAIT;
	spin_unlock_irqrestore(&bpi->lock, flags);

	return ret;
}
/******************************************************************************/

static ssize_t bp_fops_write(struct file *filp, const char __user *ubuf,
			     size_t sz_ubuf, loff_t *ppos)
{
	char *ptr;
	char *kbuf;
	size_t sz_kbuf = sz_ubuf;

	ptr = kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}

	kbuf[sz_kbuf] = '\0';
	while (sz_kbuf > 0 && isspace(kbuf[sz_kbuf-1])) {
		kbuf[--sz_kbuf] = '\0';
	}

	while (isspace(*ptr))
		ptr++;

	if (!strncmp("clean", ptr, sizeof("clean"))) {
		remove_all_hw_breakpoint();
	} else {
		struct bpinfo bp;

		while (*ptr) {
			ptr = parse_one_bpinfo(&bp, ptr);
			if (!ptr)
				break;

			if (bp.bp_type) {
				add_hw_breakpoint(bp.bp_sym, bp.bp_addr,
					bp.bp_type_str);
			} else {
				remove_hw_breakpoint(bp.bp_sym, bp.bp_addr);
			}
		}
	}

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static struct file_operations bp_fops = {
	.read  = bp_fops_read,
	.write = bp_fops_write,
	.llseek = no_llseek,
};
/******************************************************************************/

static int hw_breakpoint_func(void *arg)
{
	int ix;
	int ret;
	ulong flags;
	struct bpinfo *bpi;
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };

	sched_setscheduler_nocheck(current, SCHED_FIFO, &param);

	while (1) {
		ret = __wait(&__bpsem);

		if (kthread_should_stop())
			break;

		if (ret)
			continue;

		for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
			bpi = &__bpinfo[ix];

			spin_lock_irqsave(&bpi->lock, flags);
			if (bpi->state == BP_ADDING) {
				spin_unlock_irqrestore(&bpi->lock, flags);

				add_hw_breakpoint(bpi->bp_sym, bpi->bp_addr, bpi->bp_type_str);

				spin_lock_irqsave(&bpi->lock, flags);
				bpi->state = BP_NULL;
				spin_unlock_irqrestore(&bpi->lock, flags);

				continue;
			} else if (bpi->state == BP_DELING) {
				spin_unlock_irqrestore(&bpi->lock, flags);

				unregister_wide_hw_breakpoint(bpi->perf_event);

				spin_lock_irqsave(&bpi->lock, flags);
				bpi->state = BP_NULL;
				spin_unlock_irqrestore(&bpi->lock, flags);

				continue;
			}
			spin_unlock_irqrestore(&bpi->lock, flags);
		}
	}

	return 0;
}
/******************************************************************************/

static int __init hw_break_module_init(void)
{
	int ix;
	int ret;
	struct bpinfo *bpi;
	struct task_struct *task;

	if (!__bp_init)
		return 0;

	__bp_entry = debugfs_create_file("breakpoint", 0644, NULL, NULL, &bp_fops);
	if (!__bp_entry) {
		pr_err("create proc file failed.\n");
		return -ENOMEM;
	}

	task = kthread_create(hw_breakpoint_func, NULL, "hw_breakpoint");
	if (IS_ERR(task)) {
		pr_err("create hw_breakpoint thread failed.\n");
		debugfs_remove(__bp_entry);
		return -EINVAL;
	}
	wake_up_process(task);

	__bpsem.count = 0;
	__bpsem.task = task;
	raw_spin_lock_init(&__bpsem.lock);

	for (ix = 0; ix < ARRAY_SIZE(__bpinfo); ix++) {
		bpi = &__bpinfo[ix];

		spin_lock_init(&bpi->lock);

		bpi->state = BP_NULL;

		if (ix < __nr_bpinfo) {
			ret = __add_hw_breakpoint(bpi);
			if (!ret)
				bpi->state = BP_WAIT;
		}
	}
	return 0;
}
/******************************************************************************/

static void __exit hw_break_module_exit(void)
{
	if (!__bp_init)
		return;

	if (__bp_entry)
		debugfs_remove(__bp_entry);

	remove_all_hw_breakpoint();

	kthread_stop(__bpsem.task);
}
/******************************************************************************/

module_init(hw_break_module_init);
module_exit(hw_break_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("kernel breakpoint");
