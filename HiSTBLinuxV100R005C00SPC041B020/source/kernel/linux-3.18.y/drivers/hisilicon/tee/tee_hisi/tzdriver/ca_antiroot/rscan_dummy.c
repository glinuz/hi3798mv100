#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/random.h>
#include <linux/time.h>
#include "rscan_dummy.h"
#include "rootagent_common.h"

static struct rscan_result_dynamic dummy_rs_result;
static struct rscan_status dummy_rs_status;

static int rootagent_enable = CA_DISABLE;
static int rootagent_mode = ENG_MODE;
static int do_fuzz_test;  /* 0 not fuzz, 1 do fuzz test*/

int get_ro_secure(void)
{
	return rootagent_mode;
}

int get_rootagent_enable(void)
{
	return rootagent_enable;
}

int  rscan_dynamic_raw(int op_mask, struct rscan_result_dynamic *result)
{
	if (result)
		*result = dummy_rs_result;

	antiroot_debug(ROOTAGENT_DEBUG_DUMMY,
			"length of rprocs_whitelist %d.\n",
			(int)strlen(rprocs_whitelist));
	dump_hex(result->kcode, sizeof(result->kcode));
	if (memcmp(result->rprocs, rprocs_whitelist,
				strlen(rprocs_whitelist))) {
		antiroot_debug(ROOTAGENT_DEBUG_DUMMY,
				"result->rprocs is not same with rprocs_whitelist.\n");
	}
	return 0;
}

int rscan_get_status(struct rscan_status *status)
{
	if (status)
		*status = dummy_rs_status;
	return 0;
}

int load_rproc_whitelist(char *rproclist, ssize_t size)
{
	if (strlen(rprocs_whitelist) > size - 1) {
		antiroot_error(
				"load_rproc_whitelist: length > MAX_RPROC_SIZE.\n");
		return -1;
	}
	if (rproclist)
		strlcpy(rproclist, rprocs_whitelist, strlen(rprocs_whitelist));
	return 0;
}

static void rscan_result_init(struct rscan_result_dynamic *result)
{
	memcpy(result->kcode, test_kcode, HASH_DIGEST_SIZE);
	memcpy(result->syscalls, test_syscalls, HASH_DIGEST_SIZE);
	memcpy(result->sehooks, test_sehooks, HASH_DIGEST_SIZE);
	result->seenforcing = 1;
	result->hidproc = 0;
	load_rproc_whitelist(result->rprocs, MAX_RPROC_SIZE);
}

static uint32_t read_uint32(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos, uint32_t val)
{
	char buf[16];
	int r;

	memset(buf, 0, 16);
	r = snprintf(buf, 16, "%u\n", val);
	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}

static ssize_t write_uint32(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos, uint32_t *val)
{
	char buf[16];

	memset(buf, 0, 16);
	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	if (val)
		if (kstrtouint(buf, 10, val))
			return -EFAULT;
	return cnt;
}

static uint32_t read_string(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos, char *array, size_t len)
{
	char *tmp_proc;
	int r;

	if (!array || !ubuf)
		return -EINVAL;

	r = strlen(array);
	if (r > MAX_RPROC_SIZE - 2)
		return -EINVAL;

	/* for test, we support read/write 4k rprocs */
	tmp_proc = kzalloc(SZ_4K, GFP_KERNEL);
	if (!tmp_proc) {
		pr_err("tmp_proc kzalloc failed.\n");
		return -ENOMEM;
	}

	strlcpy(tmp_proc, array, r);
	sprintf(tmp_proc + r, "\n");
	r = simple_read_from_buffer(ubuf, cnt, ppos, tmp_proc, r+1);
	kfree(tmp_proc);
	return r;
}

static ssize_t write_string(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos, char *array, size_t len)
{
	char *tmp_proc;

	if (cnt - 1 > len)
		return -EINVAL;

	/* for test, we support read/write 4k rprocs */
	tmp_proc = kzalloc(SZ_4K, GFP_KERNEL);
	if (!tmp_proc) {
		pr_err("tmp_proc kzalloc failed.\n");
		return -ENOMEM;
	}

	if (copy_from_user(tmp_proc, ubuf, cnt - 1)) {
		kfree(tmp_proc);
		return -EFAULT;
	}

	memset(array, 0, MAX_RPROC_SIZE);
	strlcpy(array, tmp_proc, MAX_RPROC_SIZE);
	kfree(tmp_proc);
	return cnt;
}

static uint32_t read_array(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos, char *array, size_t len)
{
	char buf[128];
	int ptr = 0;

	memset(buf, 0, sizeof(buf));
	ptr += sprintf(buf, "hash: ");
	bin2hex(buf + ptr, array, len);
	ptr += 2*len;
	sprintf(buf+ptr, "\n");
	ptr += 1;
	ptr = simple_read_from_buffer(ubuf, cnt, ppos, buf, ptr);

	return ptr;
}

static ssize_t write_array(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos, char *array, size_t len)
{
	int c2, c1;
	char *tmp_array;
	char *buf;
	int i;

	if (cnt - 1 > len*2)
		return -EINVAL;

	buf = kzalloc(len*2, GFP_KERNEL);
	if (!buf) {
		antiroot_error("alloc buf err, len=%u\n", (int)len);
		return 0;
	}
	if (copy_from_user(buf, ubuf, cnt-1)) {
		kfree(buf);
		antiroot_error("copy from ubuf err, 0x%p\n", ubuf);
		return -EFAULT;
	}
	tmp_array = array;
	for (i = 0; i < len; i++) {
		/*TODO::think  hex_to_bin return  ne*/
		c1 = hex_to_bin(*(buf + i*2));
		c2 = hex_to_bin(*(buf + i*2+1));
		c1 = (c1 << 4) | c2;
		*tmp_array++ = c1;
	}
	kfree(buf);

	return cnt;
}

static ssize_t kcode_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_array(filp, ubuf, cnt, ppos, dummy_rs_result.kcode,
			HASH_DIGEST_SIZE);
}

static ssize_t kcode_write(struct file *filp,
				 const char __user *ubuf, size_t cnt,
				 loff_t *ppos)
{
	return write_array(filp, ubuf, cnt, ppos, dummy_rs_result.kcode,
			HASH_DIGEST_SIZE);
}

static const struct file_operations kcode_fops = {
	.owner = THIS_MODULE,
	.read = kcode_read,
	.write = kcode_write,
};

static ssize_t syscalls_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_array(filp, ubuf, cnt, ppos, dummy_rs_result.syscalls,
			HASH_DIGEST_SIZE);
}

static ssize_t syscalls_write(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos)
{
	return write_array(filp, ubuf, cnt, ppos, dummy_rs_result.syscalls,
			HASH_DIGEST_SIZE);
}

static const struct file_operations syscalls_fops = {
	.owner = THIS_MODULE,
	.read = syscalls_read,
	.write = syscalls_write,
};

static ssize_t sehooks_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_array(filp, ubuf, cnt, ppos, dummy_rs_result.sehooks,
			HASH_DIGEST_SIZE);
}

static ssize_t sehooks_write(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos)
{
	return write_array(filp, ubuf, cnt, ppos, dummy_rs_result.sehooks,
			HASH_DIGEST_SIZE);
}

static const struct file_operations sehooks_fops = {
	.owner = THIS_MODULE,
	.read = sehooks_read,
	.write = sehooks_write,
};

static ssize_t seenforcing_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_result.seenforcing);
}

static ssize_t seenforcing_write(struct file *filp,
		const char __user *ubuf, size_t cnt,
		loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos,
			&dummy_rs_result.seenforcing);
}

static const struct file_operations seenforcing_fops = {
	.owner = THIS_MODULE,
	.read = seenforcing_read,
	.write = seenforcing_write,
};

static ssize_t hidproc_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_result.hidproc);
}

static ssize_t hidproc_write(struct file *filp,
				 const char __user *ubuf, size_t cnt,
				 loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_result.hidproc);
}

static const struct file_operations hidproc_fops = {
	.owner = THIS_MODULE,
	.read = hidproc_read,
	.write = hidproc_write,
};

static ssize_t rprocs_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_string(filp, ubuf, cnt, ppos, dummy_rs_result.rprocs,
			MAX_RPROC_SIZE);
}

static ssize_t rprocs_write(struct file *filp,
				 const char __user *ubuf, size_t cnt,
				 loff_t *ppos)
{
	return write_string(filp, ubuf, cnt, ppos, dummy_rs_result.rprocs,
			MAX_RPROC_SIZE);
}

static const struct file_operations rprocs_fops = {
	.owner = THIS_MODULE,
	.read = rprocs_read,
	.write = rprocs_write,
};

static ssize_t cpuload_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_status.cpuload);
}

static ssize_t cpuload_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_status.cpuload);
}

static const struct file_operations cpuload_fops = {
	.owner = THIS_MODULE,
	.read = cpuload_read,
	.write = cpuload_write,
};

static ssize_t battery_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_status.battery);
}

static ssize_t battery_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_status.battery);
}

static const struct file_operations battery_fops = {
	.owner = THIS_MODULE,
	.read = battery_read,
	.write = battery_write,
};

static ssize_t charging_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_status.charging);
}

static ssize_t charging_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_status.charging);
}

static const struct file_operations charging_fops = {
	.owner = THIS_MODULE,
	.read = charging_read,
	.write = charging_write,
};

static ssize_t time_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	struct timeval tv;
	do_gettimeofday(&tv);
	pr_info("---antiroot--time: %u.\n", (uint32_t)tv.tv_sec);

	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_status.time);
}

static ssize_t time_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_status.time);
}

static const struct file_operations time_fops = {
	.owner = THIS_MODULE,
	.read = time_read,
	.write = time_write,
};

static ssize_t timezone_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	pr_info("---antiroot--timezone: %u.\n", (uint32_t)sys_tz.tz_minuteswest);
	return read_uint32(filp, ubuf, cnt, ppos, dummy_rs_status.timezone);
}

static ssize_t timezone_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	return write_uint32(filp, ubuf, cnt, ppos, &dummy_rs_status.timezone);
}

static const struct file_operations timezone_fops = {
	.owner = THIS_MODULE,
	.read = timezone_read,
	.write = timezone_write,
};


static ssize_t enable_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, rootagent_enable);
}

static ssize_t enable_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	char buf[16];
	int val = 0;
	int ret;

	memset(buf, 0, 16);
	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	if (kstrtouint(buf, 10, &val))
		return -EFAULT;

	antiroot_debug(ROOTAGENT_DEBUG_DUMMY, "val = %d.\n", val);
	if (val == rootagent_enable)
		return cnt;

	if (CA_ENABLE == val) {
		ret = dummy_tee_init();
		if (0 == ret) {
			antiroot_info("rootagent set to enable. start dummy_tee_init()\n");
			rootagent_enable = CA_ENABLE;
		} else {
			antiroot_error("rootagent set to enable failed. ret = %d\n", ret);
			return -255;
		}
	} else {
		rootagent_enable = CA_DISABLE;
		antiroot_info("rootagent set to disable. pause now.");
	}

	return cnt;
}

static const struct file_operations enable_fops = {
	.owner = THIS_MODULE,
	.read = enable_read,
	.write = enable_write,
};

static ssize_t mode_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, rootagent_mode);
}

static ssize_t mode_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	char buf[16];
	int val = 0;

	memset(buf, 0, 16);
	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	if (kstrtouint(buf, 10, &val))
		return -EFAULT;

	rootagent_mode = val;
	antiroot_debug(ROOTAGENT_DEBUG_DUMMY,
			"set rootagent_mode to val = %d.\n", val);

	return cnt;
}
static const struct file_operations mode_fops = {
	.owner = THIS_MODULE,
	.read = mode_read,
	.write = mode_write,
};

static ssize_t fuzz_read(struct file *filp, char __user *ubuf,
		size_t cnt, loff_t *ppos)
{
	return read_uint32(filp, ubuf, cnt, ppos, do_fuzz_test);
}

static ssize_t fuzz_write(struct file *filp,
		const char __user *ubuf, size_t cnt, loff_t *ppos)
{
	char buf[16];
	int val = 0;

	memset(buf, 0, 16);
	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, ubuf, cnt))
		return -EFAULT;

	if (kstrtouint(buf, 10, &val))
		return -EFAULT;

	do_fuzz_test = val;
	antiroot_debug(ROOTAGENT_DEBUG_DUMMY,
			"set rootagent_mode to val = %d.\n", val);

	return cnt;
}
static const struct file_operations fuzz_fops = {
	.owner = THIS_MODULE,
	.read = fuzz_read,
	.write = fuzz_write,
};

static struct dentry *dbg_dentry;
int rscan_dummy_init(void)
{
	struct dentry *result_dentry, *status_dentry;

	dbg_dentry = debugfs_create_dir("tee_antiroot", NULL);

	debugfs_create_file("enable", 0660, dbg_dentry,
			NULL, &enable_fops);

	debugfs_create_file("mode", 0660, dbg_dentry,
			NULL, &mode_fops);

	debugfs_create_file("dofuzz", 0660, dbg_dentry,
			NULL, &fuzz_fops);

	result_dentry = debugfs_create_dir("scan_result", dbg_dentry);
	debugfs_create_file("kcode", 0660, result_dentry,
			NULL, &kcode_fops);
	debugfs_create_file("syscalls", 0660, result_dentry,
			NULL, &syscalls_fops);
	debugfs_create_file("sehooks", 0660, result_dentry,
			NULL, &sehooks_fops);
	debugfs_create_file("seenforcing", 0660, result_dentry,
			NULL, &seenforcing_fops);
	debugfs_create_file("hidproc", 0660, result_dentry,
			NULL, &hidproc_fops);
	debugfs_create_file("rprocs", 0660, result_dentry,
			NULL, &rprocs_fops);

	status_dentry = debugfs_create_dir("status", dbg_dentry);
	debugfs_create_file("cpuload", 0660, status_dentry,
			NULL, &cpuload_fops);
	debugfs_create_file("battery", 0660, status_dentry,
			NULL, &battery_fops);
	debugfs_create_file("charging", 0660, status_dentry,
			NULL, &charging_fops);
	debugfs_create_file("time", 0660, status_dentry,
			NULL, &time_fops);
	debugfs_create_file("timezone", 0660, status_dentry,
			NULL, &timezone_fops);

	rscan_result_init(&dummy_rs_result);

	antiroot_debug(ROOTAGENT_DEBUG_DUMMY,
			"rscan_dummy_init done.\n");

	return 0;
}

void rscan_dummy_exit(void)
{
	debugfs_remove(dbg_dentry);
}

void dummy_fuzz(void)
{
	if (0 == do_fuzz_test)
		return;

	/* fill dummy_rs_result and dummy_rs_status with random value */
	get_random_bytes(dummy_rs_result.kcode, HASH_DIGEST_SIZE);
	get_random_bytes(dummy_rs_result.syscalls, HASH_DIGEST_SIZE);
	get_random_bytes(dummy_rs_result.sehooks, HASH_DIGEST_SIZE);
	dummy_rs_result.seenforcing = get_random_int();
	dummy_rs_result.hidproc = get_random_int();
	get_random_bytes(dummy_rs_result.rprocs, MAX_RPROC_SIZE);
	dummy_rs_status.cpuload = get_random_int();
	dummy_rs_status.battery = get_random_int();
	dummy_rs_status.charging = get_random_int();
	dummy_rs_status.time = get_random_int();
	dummy_rs_status.timezone = get_random_int();
}
