/*
 * hrtimer() test kernel module
 *
 * Test is executed by writing and reading to /sys/kernel/debug/hrtimer_test
 * Tests are configured by writing: usecs ITERATIONS
 * Tests are executed by reading from the same file.
 * Specifying usecs of 0 or negative values will run multiples tests.
 *
 * Copyright (C) 2014 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>
 
#define DEFAULT_ITERATIONS 100

#define DEBUGFS_FILENAME "hrtimer_test"


static DEFINE_MUTEX(hrtimer_test_lock);
static struct dentry *hrtimer_test_debugfs_file;
static int hrtimer_test_usecs;
static int hrtimer_test_iterations = DEFAULT_ITERATIONS;

static int hrtimer_test_single(struct seq_file *s, int usecs, uint32_t iters)
{
	int min = 0, max = 0, fail_count = 0;
	uint64_t sum = 0;
	uint64_t avg;
	int i;
	/* Allow hrtimer to be up to 0.5% fast */
	int allowed_error_ns = usecs * 5;
	ktime_t waittime;

	for (i = 0; i < iters; ++i) {
		struct timespec ts1, ts2;
		int time_passed;
		struct sched_param param = { .sched_priority = 99 };
		sched_setscheduler(current, SCHED_FIFO, &param);
		waittime = ns_to_ktime(usecs * 1000);
		set_current_state(TASK_UNINTERRUPTIBLE);
		ktime_get_ts(&ts1);
		schedule_hrtimeout(&waittime, HRTIMER_MODE_REL);
		ktime_get_ts(&ts2);
		time_passed = timespec_to_ns(&ts2) - timespec_to_ns(&ts1);

		if (i == 0 || time_passed < min)
			min = time_passed;
		if (i == 0 || time_passed > max)
			max = time_passed;
		if ((time_passed + allowed_error_ns) / 1000 < usecs)
			++fail_count;
		WARN_ON(time_passed < 0);
		sum += time_passed;
	}

	avg = sum;
	do_div(avg, iters);
	seq_printf(s, "%d usecs x %d: exp=%d allowed=%d min=%d avg=%lld max=%d",
			usecs, iters, usecs * 1000,
			(usecs * 1000) - allowed_error_ns, min, avg, max);
	if (fail_count)
		seq_printf(s, " FAIL=%d", fail_count);
	seq_puts(s, "\n");

	return 0;
}

static int hrtimer_test_show(struct seq_file *s, void *v)
{
	int usecs;
	int iters;
	int ret = 0;

	mutex_lock(&hrtimer_test_lock);
	usecs = hrtimer_test_usecs;
	iters = hrtimer_test_iterations;
	mutex_unlock(&hrtimer_test_lock);

	if (usecs > 0 && iters > 0) {
		return hrtimer_test_single(s, usecs, iters);
	} else if (usecs == 0) {
		struct timespec ts;

		ktime_get_ts(&ts);
		seq_printf(s, "hrtimer() test (lpj=%ld kt=%ld.%09ld)\n",
				loops_per_jiffy, ts.tv_sec, ts.tv_nsec);
		seq_puts(s, "usage:\n");
		seq_puts(s, "echo usecs [ITERS] > " DEBUGFS_FILENAME "\n");
		seq_puts(s, "cat " DEBUGFS_FILENAME "\n");
	}

	return ret;
}

static int hrtimer_test_open(struct inode *inode, struct file *file)
{
	return single_open(file, hrtimer_test_show, inode->i_private);
}

static ssize_t hrtimer_test_write(struct file *file, const char __user *buf,
		size_t count, loff_t *pos)
{
	char lbuf[32];
	int ret;
	int usecs;
	int iters;

	if (count >= sizeof(lbuf))
		return -EINVAL;

	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;
	lbuf[count] = '\0';

	ret = sscanf(lbuf, "%d %d", &usecs, &iters);
	if (ret < 1)
		return -EINVAL;
	else if (ret < 2)
		iters = DEFAULT_ITERATIONS;

	mutex_lock(&hrtimer_test_lock);
	hrtimer_test_usecs = usecs;
	hrtimer_test_iterations = iters;
	mutex_unlock(&hrtimer_test_lock);

	return count;
}

static const struct file_operations hrtimer_test_debugfs_ops = {
	.owner = THIS_MODULE,
	.open = hrtimer_test_open,
	.read = seq_read,
	.write = hrtimer_test_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init hrtimer_test_init(void)
{
	mutex_lock(&hrtimer_test_lock);
	hrtimer_test_debugfs_file = debugfs_create_file(DEBUGFS_FILENAME,
			S_IRUSR, NULL, NULL, &hrtimer_test_debugfs_ops);
	mutex_unlock(&hrtimer_test_lock);

	return 0;
}

module_init(hrtimer_test_init);

static void __exit hrtimer_test_exit(void)
{
	mutex_lock(&hrtimer_test_lock);
	debugfs_remove(hrtimer_test_debugfs_file);
	mutex_unlock(&hrtimer_test_lock);
}

module_exit(hrtimer_test_exit);

MODULE_AUTHOR("David Riley <davidriley@chromium.org>");
MODULE_LICENSE("GPL");
