/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
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
 * Create By Cai Zhiyong 2015.5.28
 *
******************************************************************************/

#define pr_fmt(fmt) "mmcdbg: " fmt

#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>
#include <linux/sched.h>

#define MMC_MAX_BLK                0x2000000   /* 32M block, 16G size */
#define MMC_CHUNK_SIZE_SHIFT       11 /* 2K blocks, 1M size */

#define MMC_CHUNK_SIZE             (1 << MMC_CHUNK_SIZE_SHIFT)
#define MMC_CHUNK_SIZE_MASK        (MMC_CHUNK_SIZE - 1)
#define MMC_NUM_CHUNK              (MMC_MAX_BLK / MMC_CHUNK_SIZE)

#define NUM_PER_LINE               8
#define MAX_BUF                    ((NUM_PER_LINE + 1) * 9 + 24)
#define MAX_PATH_LEN               260

extern int trace_level;

static DEFINE_MUTEX(mutex_rwcount);
static DEFINE_MUTEX(mutex_notice);

static volatile unsigned int *rw_chunk[2][MMC_NUM_CHUNK] = {{0},{0}};

#define GET_CHUNK(_rw, _ofblk) rw_chunk[_rw][(_ofblk) >> MMC_CHUNK_SIZE_SHIFT]

static char *mmcdbg_options_string = NULL;
static char mmcdbg_helper[MAX_PATH_LEN] = {0};
static int mmcdbg_rcount_enable = 0;
static int mmcdbg_wcount_enable = 0;
static int mmcdbg_devid = -1;

struct fo_data_t {
	int is_write;
	unsigned int ofblk;
	int state;
};
/******************************************************************************/

static int mmcdbg_rw_count(int is_write, unsigned int ofblk, unsigned int nrblk)
{
	int ret = -1;
	int rw = is_write ? 1 : 0;

	if (!nrblk)
		return 0;

	if (ofblk + nrblk > MMC_MAX_BLK) {
		pr_err("out of block count.\n");
		return -1;
	}

	mutex_lock(&mutex_rwcount);

	while (nrblk-- > 0) {
		volatile unsigned int *entry = GET_CHUNK(rw, ofblk);
		if (!entry) {
			entry = (unsigned int *)vmalloc(MMC_CHUNK_SIZE << 2);
			if (!entry) {
				pr_err("out of memory.\n");
				goto fail;
			}
			memset((void *)entry, 0, MMC_CHUNK_SIZE << 2);

			GET_CHUNK(rw, ofblk) = entry;
		}
		entry[ofblk & MMC_CHUNK_SIZE_MASK]++;
		ofblk++;
	}
	ret = 0;
fail:
	mutex_unlock(&mutex_rwcount);

	return ret;
}
/******************************************************************************/

static int mmcdbg_rw_free(int is_write)
{
	unsigned int blk;
	int rw = is_write ? 1 : 0;

	mutex_lock(&mutex_rwcount);

	for (blk = 0; blk < MMC_MAX_BLK; blk += MMC_CHUNK_SIZE) {
		volatile unsigned int *entry = GET_CHUNK(rw, blk);
		if (entry) {
			GET_CHUNK(rw, blk) = NULL;
			vfree((void *)entry);
		}
	}

	mutex_unlock(&mutex_rwcount);

	return 0;
}
/******************************************************************************/

static void dump_line(unsigned int ofblk, unsigned int *value, char **buf,
		      unsigned int *sz_buf, int state)
{
	int ret;

	if (state == 0 || state == 1) {
		int ix;
		unsigned int tmp[NUM_PER_LINE] = {0};
		unsigned int *v = (value == NULL ? tmp : value);

		ret = snprintf(*buf, *sz_buf, "%08x:", ofblk);
		*buf += ret;
		*sz_buf -= ret;

		for (ix = 0; ix < NUM_PER_LINE; ix++) {
			ret = snprintf(*buf, *sz_buf, " %04x", v[ix]);
			*buf += ret;
			*sz_buf -= ret;
		}

	} else if (state == 2) {
		ret = snprintf(*buf, *sz_buf, "...");
		*buf += ret;
		*sz_buf -= ret;

	} else if (state == 3) {
		ret = snprintf(*buf, *sz_buf, "%08x:", ofblk);
		*buf += ret;
		*sz_buf -= ret;

	}

	ret = snprintf(*buf, *sz_buf, "\n");
	*buf += ret;
	*sz_buf -= ret;
}
/******************************************************************************/

static int mmcdbg_rw_dump_buf(int is_write, unsigned int *ofblk, char *buf,
			      unsigned int sz_buf, int *state)
{
	char *pbuf = buf;
	unsigned int index;
	unsigned int blk = *ofblk;
	int rw = is_write ? 1 : 0;

	if (*state == 3)
		return 0;

	sz_buf--; /* for trailing end '\0' */

	while (blk < MMC_MAX_BLK) {
		unsigned int *entry = (unsigned int *)GET_CHUNK(rw, blk);

		if (!entry) {
			if (sz_buf < MAX_BUF)
				goto no_buf;

			if (*state == 0) {
				/* XXXXXXX: 0000 0000 0000 0000 0000 0000 0000 0000 */
				*state = 1;
				dump_line(blk, NULL, &pbuf, &sz_buf, *state);
			}
			
			if (*state == 1) {
				/* ... */
				*state = 2;
				dump_line(blk, NULL, &pbuf, &sz_buf, *state);
			}

			blk += MMC_CHUNK_SIZE;
			continue;
		}

		index = blk & MMC_CHUNK_SIZE_MASK;
		blk = blk & ~MMC_CHUNK_SIZE_MASK;

		while (index < MMC_CHUNK_SIZE) {
			int ix, ret;

			if (sz_buf < MAX_BUF) {
				blk += index;
				goto no_buf;
			}

			ret = 0;
			for (ix = 0; ix < NUM_PER_LINE; ix++)
				ret |= entry[ix + index];

			if (!ret) {
				if (*state == 0) {
					*state = 1;
					dump_line(blk + index, NULL, &pbuf,
						  &sz_buf, *state);
				} else if (*state == 1) {
					*state = 2;
					dump_line(blk + index, NULL, &pbuf,
						  &sz_buf, *state);
				}
			} else {
				if (*state)
					*state = 0;

				dump_line(blk + index, &entry[index],
					  &pbuf, &sz_buf, *state);
			}

			index += NUM_PER_LINE;
		}

		blk += MMC_CHUNK_SIZE;
	}

	if (*state == 2) {
		*state = 3;
		dump_line(blk, NULL, &pbuf, &sz_buf, *state);
	}

no_buf:
	*ofblk = blk;

	pbuf[0] = '\0';

	return pbuf - buf;
}
/******************************************************************************/

static int mmcdbg_rw_notice(int is_write, u32 ofblk, u32 nrblk)
{
	int retval = 0;
	char *argv[2];
	struct kobj_uevent_env *env;
	char *rwstr = is_write ? "write" : "read";

	env = kzalloc(sizeof(struct kobj_uevent_env), GFP_KERNEL);
	if (!env)
		return -ENOMEM;

	retval = add_uevent_var(env, "RW=%s", rwstr);
	if (retval)
		goto exit;

	retval = add_uevent_var(env, "OFFSET=0x%08x", ofblk);
	if (retval)
		goto exit;

	retval = add_uevent_var(env, "COUNT=0x%08x", nrblk);
	if (retval)
		goto exit;

	retval = add_uevent_var(env, "PID=%d", current->pid);
	if (retval)
		goto exit;

	retval = add_uevent_var(env, "COMM=%s", current->comm);
	if (retval)
		goto exit;

	argv[0] = mmcdbg_helper;
	argv[1] = NULL;

	retval = add_uevent_var(env, "PATH=/sbin:/bin:/usr/sbin:/usr/bin");
	if (retval)
		goto exit;

	retval = call_usermodehelper(argv[0], argv,
				     env->envp, UMH_WAIT_EXEC);
exit:
	kfree(env);
	return retval;
}
/******************************************************************************/

static ssize_t fo_dump_rwcount_read(struct file *filp, char __user *buffer,
				    size_t count, loff_t *ppos)
{
	int ret;
	char buf[256] = {0};
	char __user *pusrbuf = buffer;
	struct fo_data_t *data = (struct fo_data_t *)filp->private_data;

	if (*ppos == 2)
		return 0;

	if (data->is_write && !mmcdbg_wcount_enable) {
		*ppos = 2;

		ret = snprintf(buf, sizeof(buf), "mmc write count is disable.\n"
			       "input \"echo 1 > write\" to enable.\n");

		if (copy_to_user(pusrbuf, buf, ret))
			return -EFAULT;

		pusrbuf += ret;

		return pusrbuf - buffer;
	}

	if (!data->is_write && !mmcdbg_rcount_enable) {
		*ppos = 2;

		ret = snprintf(buf, sizeof(buf), "mmc read count is disable.\n"
			       "input \"echo 1 > read\" to enable\n");

		if (copy_to_user(pusrbuf, buf, ret))
			return -EFAULT;

		pusrbuf += ret;

		return pusrbuf - buffer;
	}

	do {
		if (count < sizeof(buf)) {
			*ppos = 1;
			break;
		}

		mutex_lock(&mutex_rwcount);

		ret = mmcdbg_rw_dump_buf(data->is_write, &data->ofblk, buf,
					 sizeof(buf), &data->state);

		mutex_unlock(&mutex_rwcount);

		if (!ret) {
			*ppos = 2;
			break;
		}

		if (copy_to_user(pusrbuf, buf, ret))
			return -EFAULT;

		pusrbuf += ret;
		count -= ret;

	} while (ret);

	return pusrbuf - buffer;
}
/******************************************************************************/

static ssize_t fo_dump_rwcount_write(struct file *filp, const char __user *buffer,
				     size_t count, loff_t *ppos)
{
	char *enstr;
	long enable = 0;
	char buf[32] = {0};
	unsigned int sz_buf = sizeof(buf) - 1;
	struct fo_data_t *data = (struct fo_data_t *)filp->private_data;
	int rw = data->is_write ? 1 : 0;

	if (count < sz_buf)
		sz_buf = count;

	if (copy_from_user(buf, buffer, sz_buf))
		return -EFAULT;

	buf[sz_buf] = '\0';
	if (kstrtol(buf, 0, &enable)) {
		pr_err("input error :%s\n", buf);
		enable = rw ? mmcdbg_wcount_enable : mmcdbg_rcount_enable;
	}

	enstr = enable ? "enable" : "disable";

	if (rw) {
		if (enable != mmcdbg_wcount_enable) {
			mmcdbg_wcount_enable = enable;
			if (!mmcdbg_wcount_enable)
				mmcdbg_rw_free(rw);
			pr_info("%s write count.\n", enstr);
		}
	} else {
		if (enable != mmcdbg_rcount_enable) {
			mmcdbg_rcount_enable = enable;
			if (!mmcdbg_rcount_enable)
				mmcdbg_rw_free(rw);
			pr_info("%s read count.\n", enstr);
		}
	}

	return count;
}
/******************************************************************************/

static int fo_dump_rcount_open(struct inode *inode, struct file *file)
{
	struct fo_data_t *data;

	data = (struct fo_data_t *)vmalloc(sizeof(struct fo_data_t));
	if (!data) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	data->is_write = 0;
	data->ofblk = 0;
	data->state = 0;

	file->private_data = (void *)data;

	return 0;
}
/******************************************************************************/

static int fo_dump_wcount_open(struct inode *inode, struct file *file)
{
	struct fo_data_t *data;

	data = (struct fo_data_t *)vmalloc(sizeof(struct fo_data_t));
	if (!data) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	data->is_write = 1;
	data->ofblk = 0;
	data->state = 0;

	file->private_data = (void *)data;

	return 0;
}
/******************************************************************************/

static int fp_dump_rwcount_release(struct inode *inode, struct file *file)
{
	struct fo_data_t *data = (struct fo_data_t *)file->private_data;

	if (data)
		vfree(data);

	file->private_data = NULL;

	return 0;
}
/******************************************************************************/

static ssize_t fo_rw_notice_read(struct file *filp, char __user *buffer,
				 size_t count, loff_t *ppos)
{
	int ret = 0;
	char *kbuf;

	if (*ppos == 1)
		return 0;

	*ppos = 1;

	kbuf = (char *)vmalloc(sizeof(mmcdbg_helper) + 4);
	if (!kbuf) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	ret = snprintf(kbuf, sizeof(mmcdbg_helper) + 4, "<%s>\n", mmcdbg_helper);
	if (copy_to_user(buffer, kbuf, ret)) {
		vfree(kbuf);
		return -EFAULT;
	}

	vfree(kbuf);

	return ret;
}
/******************************************************************************/

static ssize_t fo_rw_notice_write(struct file *filp, const char __user *buffer,
				  size_t count, loff_t *ppos)
{
	char *kbuf;
	char *pkbuf;
	int size = count;

	if (size >= sizeof(mmcdbg_helper))
		size = sizeof(mmcdbg_helper) - 1;

	kbuf = (char *)vmalloc(size + 1);
	if (!kbuf) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	if (copy_from_user(kbuf, buffer, size)) {
		vfree(kbuf);
		return -EFAULT;
	}

	kbuf[size] = '\0';

	pkbuf = strim(kbuf);

	mutex_lock(&mutex_notice);

	strncpy(mmcdbg_helper, pkbuf, size);

	mutex_unlock(&mutex_notice);

	vfree(kbuf);

	return count;
}
/******************************************************************************/

static struct file_operations fo_dump_rcount = {
	.owner = THIS_MODULE,
	.open = fo_dump_rcount_open,
	.read  = fo_dump_rwcount_read,
	.write = fo_dump_rwcount_write,
	.release = fp_dump_rwcount_release,
};

static struct file_operations fo_dump_wcount = {
	.owner = THIS_MODULE,
	.open = fo_dump_wcount_open,
	.read  = fo_dump_rwcount_read,
	.write = fo_dump_rwcount_write,
	.release = fp_dump_rwcount_release,
};

static struct file_operations fo_rw_notice = {
	.owner = THIS_MODULE,
	.read  = fo_rw_notice_read,
	.write = fo_rw_notice_write,
};
/******************************************************************************/

static void mmcdbg_options(const char *string)
{
	int enable;
	const char *pos, *cmd;

	pos = string;

	while (*pos) {
		while (*pos && *pos != '+' && *pos != '-')
			pos++;

		switch (*pos++) {
		case '+': enable = 1; break;
		case '-': enable = 0; break;
		default:  return;
		}

		cmd = pos;

		while (*pos == '_' || isalpha(*pos))
			pos++;

		if (*cmd && pos > cmd) {
			int size = pos - cmd;
			char *enstr = enable ? "enable" : "disable";

			if (!strncmp(cmd, "read", size)) {
				mmcdbg_rcount_enable = enable;
				pr_info("%s read count.\n", enstr);
			} else if (!strncmp(cmd, "write", size)) {
				mmcdbg_wcount_enable = enable;
				pr_info("%s write count.\n", enstr);
			}
		}

		while (isspace(*pos) || *pos == ',' || *pos == ';')
			pos++;
	}
}
/******************************************************************************/

int himci_dbg_rw(int devid, int is_write, u32 ofblk, u32 nrblk)
{
	if (mmcdbg_devid != devid)
		return 0;

	if (is_write && mmcdbg_wcount_enable)
		mmcdbg_rw_count(is_write, ofblk, nrblk);

	if (!is_write && mmcdbg_rcount_enable)
		mmcdbg_rw_count(is_write, ofblk, nrblk);

	if (mmcdbg_helper[0]) {
		mutex_lock(&mutex_notice);

		mmcdbg_rw_notice(is_write, ofblk, nrblk);

		mutex_unlock(&mutex_notice);
	}

	return 0;
}
/*****************************************************************************/

int himci_dbg_init(int devid)
{
	char buf[16];
	struct dentry *root = NULL;
	struct dentry *ret = NULL;
	unsigned int mode = S_IFREG | S_IRUSR | S_IWUSR;

	snprintf(buf, sizeof(buf), "himci%d", devid);
	root = debugfs_create_dir(buf, NULL);
	if (!root) {
		pr_err("Can't create '%s' dir.\n", buf);
		return -ENOENT;
	}

	ret = debugfs_create_file("read", mode, root, NULL, &fo_dump_rcount);
	if (!ret) {
		pr_err("Can't create 'read' file.\n");
		goto fail;
	}

	ret = debugfs_create_file("write", mode, root, NULL, &fo_dump_wcount);
	if (!ret) {
		pr_err("Can't create 'write' file.\n");
		goto fail;
	}

	ret = debugfs_create_file("notice", mode, root, NULL, &fo_rw_notice);
	if (!ret) {
		pr_err("Can't create 'notice' file.\n");
		goto fail;
	}

	if (mmcdbg_options_string)
		mmcdbg_options(mmcdbg_options_string);

	mmcdbg_devid = devid;

	return 0;
fail:
	debugfs_remove_recursive(root);

	return -ENOENT;
}
/*****************************************************************************/

static int __init mmcdbg_options_setup(char *s)
{
	mmcdbg_options_string = s;
	return 1;
}
__setup("mmcdbg=", mmcdbg_options_setup);

static int __init mmcdbg_tracelevel_setup(char *s)
{
	int ret;

	if (!s)
		return 0;

	ret = kstrtoint(s, 10, &trace_level);
	if (ret)
		return ret;

	return 1;
}
__setup("mmctracelevel=",mmcdbg_tracelevel_setup);
