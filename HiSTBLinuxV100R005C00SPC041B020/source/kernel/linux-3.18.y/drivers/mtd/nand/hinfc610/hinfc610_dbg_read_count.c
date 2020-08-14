/******************************************************************************
 *  Copyright (C) 2014 Cai Zhiyong
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
 * Create By Cai Zhiying 2014.11.19
 *
******************************************************************************/

#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

struct hinfc610_dbg_read_count_t {
	struct dentry *dbgfs_file;

	unsigned int *pages;
	unsigned int max_pages;
	unsigned int sz_pages;
	char *dbgfs_name;
};

extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_count;
static DEFINE_MUTEX(read_mutex);
static struct hinfc610_dbg_read_count_t *dbg_read_count = NULL;
/*****************************************************************************/

static int dbgfs_read_count_read(struct file *filp, char __user *usrbuf,
				 size_t sz_usrbuf, loff_t *ppos)
{
	int size;
	int pos;
	char *ptr;
	char *buf;
	unsigned int *pages;
	struct hinfc610_dbg_read_count_t *read_count =
		(struct hinfc610_dbg_read_count_t *)filp->private_data;

	ptr = buf = vmalloc(sz_usrbuf);
	if (!buf)
		return -ENOMEM;

	pos = *ppos;
	pages = read_count->pages + pos;

	while (sz_usrbuf >= 128 && pos < read_count->max_pages) {
		size = snprintf(ptr, sz_usrbuf,
			"%5d: %8u %8u %8u %8u  %8u %8u %8u %8u\n",
			pos, pages[0], pages[1], pages[2], pages[3], 
			pages[4], pages[5], pages[6], pages[7]);

		ptr += size;
		sz_usrbuf -= size;

		pos += 8;
		pages += 8;
	}

	*ppos = pos;
	size = ptr - buf;

	if (copy_to_user(usrbuf, buf, size))
		size = -EFAULT;

	vfree(buf);

	return size;
}
/*****************************************************************************/

static int dbgfs_read_count_write(struct file *filp, const char __user *usrbuf,
				  size_t sz_usrbuf, loff_t *ppos)
{
	char *ptr;
	char *buf;
	struct hinfc610_dbg_read_count_t *read_count =
		(struct hinfc610_dbg_read_count_t *)filp->private_data;

	ptr = buf = vmalloc(sz_usrbuf + 1);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, usrbuf, sz_usrbuf)) {
		vfree(buf);
		return -EFAULT;
	}
	buf[sz_usrbuf] = '\0';

	ptr = skip_spaces(ptr);

	if (*ptr == '?') {
		printk("echo 'clean' > %s   // clean all count\n",
			read_count->dbgfs_name);
	} else if (!memcmp(ptr, "clean", 5)) {
		mutex_lock(&read_mutex);
		memset(read_count->pages, 0, read_count->sz_pages);
		mutex_unlock(&read_mutex);
	}

	vfree(buf);
	return sz_usrbuf;
}
/*****************************************************************************/

static int dbgfs_read_count_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}
/*****************************************************************************/

static struct file_operations dbgfs_read_count_fops = {
	.owner   = THIS_MODULE,
	.open    = dbgfs_read_count_open,
	.read    = dbgfs_read_count_read,
	.write   = dbgfs_read_count_write,
};
/*****************************************************************************/

static int dbgfs_read_count_init(struct dentry *root, struct hinfc_host *host)
{
	int size;
	struct hinfc610_dbg_read_count_t *read_count;

	if (dbg_read_count)
		return 0;

	size = sizeof(struct hinfc610_dbg_read_count_t);
	read_count = vmalloc(size);
	if (!read_count) {
		pr_err("Can't allocate memory.\n");
		return -ENOMEM;
	}
	memset(read_count, 0, size);

	read_count->dbgfs_name = (char *)hinfc610_dbg_inf_read_count.name;

	read_count->max_pages = host->page_per_chip;
	read_count->sz_pages = read_count->max_pages * sizeof(read_count->pages[0]);
	read_count->pages = vmalloc(read_count->sz_pages);
	if (!read_count) {
		pr_err("Can't allocate memory.\n");
		goto pages_fail;
	}
	memset(read_count->pages, 0, read_count->sz_pages);

	read_count->dbgfs_file = debugfs_create_file(read_count->dbgfs_name,
		S_IFREG | S_IRUSR | S_IWUSR, root,
		read_count, &dbgfs_read_count_fops);
	if (!read_count->dbgfs_file) {
		pr_err("Can't create 'read_count' file.\n");
		goto dbgfs_fail;
	}

	dbg_read_count = read_count;

	return 0;

dbgfs_fail:
	vfree(read_count->pages);
pages_fail:
	vfree(read_count);

	return -ENOMEM;
}
/*****************************************************************************/

static int dbgfs_read_count_uninit(struct hinfc_host *host)
{
	if (!dbg_read_count)
		return 0;

	mutex_lock(&read_mutex);

	debugfs_remove(dbg_read_count->dbgfs_file);

	vfree(dbg_read_count->pages);
	vfree(dbg_read_count);

	dbg_read_count = NULL;

	mutex_unlock(&read_mutex);

	return 0;
}
/*****************************************************************************/

static void dbg_read_count_read(struct hinfc_host *host)
{
	unsigned int pageindex;

	mutex_lock(&read_mutex);

	if (!dbg_read_count)
		goto out;

	pageindex = GET_RW_PAGE_INDEX(host);

	if (pageindex >= dbg_read_count->max_pages) {
		pr_err("page index out of range.\n");
		goto out;
	}

	dbg_read_count->pages[pageindex]++;

out:
	mutex_unlock(&read_mutex);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_count = {
	"read_count", 0,
	dbgfs_read_count_init,
	dbgfs_read_count_uninit,
	dbg_read_count_read,
	NULL,
	NULL,
	NULL,
};
