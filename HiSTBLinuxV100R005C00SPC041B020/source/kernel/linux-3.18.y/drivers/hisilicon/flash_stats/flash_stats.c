/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.8.10
 *
******************************************************************************/

#define pr_fmt(fmt) "flashstats: " fmt

#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "flash_stats_priv.h"

/******************************************************************************/

static struct flash_stats_item *feature_items[] = {
	&flash_stats_item_read,
	&flash_stats_item_write,
	&flash_stats_item_erase,
	&flash_stats_item_ecc,
	&flash_stats_item_rr,
	NULL,
};
/******************************************************************************/

static void flash_stats_get_option(struct flash_stats_ctrl *ctrl)
{
	int ix;
	int count;
	char *ptr = ctrl->prbuf;
	int sz_prbuf = sizeof(ctrl->prbuf);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		count = snprintf(ptr, sz_prbuf, "%c%s,",
			item->enable ? '+' : '-', item->name);
		sz_prbuf -= count;
		ptr += count;
	}

	count = snprintf(ptr, sz_prbuf, "\n");
	sz_prbuf -= count;
	ptr += count;

	ctrl->sz_prbuf = ptr - ctrl->prbuf;
	ctrl->need_update = false;
}
/******************************************************************************/

static ssize_t flash_stats_fops_read(struct file *filp, char __user *ubuf,
				     size_t sz_ubuf, loff_t *ppos)
{
	struct flash_stats_ctrl *ctrl = filp->private_data;

	if (!*ppos && ctrl->need_update) {
		flash_stats_get_option(ctrl);
		ctrl->need_update = false;
	}

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, ctrl->prbuf,
		ctrl->sz_prbuf);
}
/******************************************************************************/

static struct flash_stats_item *
flash_stats_match_item(struct flash_stats_ctrl *ctrl, char *buf, char *ptail)
{
	int ix;

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (buf + item->sz_name > ptail)
			continue;

		if (memcmp(buf, item->name, item->sz_name))
			continue;

		if (isalpha(buf[item->sz_name]))
			continue;

		return item;
	}

	return NULL;
}
/******************************************************************************/

static void flash_stats_items_resolve(struct flash_stats_ctrl *ctrl, char *buf,
				      int sz_buf)
{
	int enable;
	struct flash_stats_item *item;
	char *ptail = buf + sz_buf;

	while (buf < ptail) {
		if (*buf != '+' && *buf != '-') {
			buf++;
			continue;
		}

		if (*buf++ == '+')
			enable = 1;
		else
			enable = 0;

		item = flash_stats_match_item(ctrl, buf, ptail);
		if (!item) {
			char chr;
			char *nextbuf = buf;

			while (nextbuf < ptail && isalpha(*nextbuf))
				nextbuf++;

			chr = *nextbuf;
			*nextbuf = '\0';
			pr_err("Invalid key word: \"%s\"\n", buf);
			*nextbuf = chr;
			buf = nextbuf;

			continue;
		}

		buf += item->sz_name;

		if (item->enable == enable)
			continue;
			
		if (enable) {
			BUG_ON(item->dentry);

			item->dentry = debugfs_create_file(item->name,
				0644, ctrl->dentry_root, item, item->fops);
			if (!item->dentry)
				pr_err("create debugfs '%s' fail.\n", item->name);
			else
				item->enable = 1;

		} else {
			debugfs_remove(item->dentry);
			item->dentry = NULL;
			item->enable = 0;
		}
	}
}
/******************************************************************************/

static ssize_t flash_stats_fops_write(struct file *filp, const char __user *ubuf,
				      size_t sz_ubuf, loff_t *ppos)
{
	char *kbuf;
	struct flash_stats_ctrl *ctrl = filp->private_data;

	kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}

	kbuf[sz_ubuf] = '\0';

	flash_stats_items_resolve(ctrl, kbuf, sz_ubuf);

	vfree(kbuf);

	ctrl->need_update = true;

	return sz_ubuf;
}
/******************************************************************************/

static int flash_stats_fops_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return nonseekable_open(inode, file);
}
/*****************************************************************************/

static struct file_operations flash_stats_fops = {
	.open = flash_stats_fops_open,
	.read  = flash_stats_fops_read,
	.write = flash_stats_fops_write,
	.owner = THIS_MODULE,
	.llseek = no_llseek,
};
/******************************************************************************/

void flash_stats_resolve_key(char *buf, char *ptail, u64 *addr, u64 *len,
			     int *clean, int *warn, int *record)
{
	int ix;

	struct key_val_t *key_val[] = {
		_SKEY_VAL("address=", addr,   sizeof(*addr),   0),
		_SKEY_VAL("length=",  len,    sizeof(*len),    0),
		_SKEY_VAL("clean",    clean,  sizeof(*clean),  1),
		_SKEY_VAL("warn=",    warn,   sizeof(*warn),   0),
		_SKEY_VAL("record=",  record, sizeof(*record), 0),
	};

	while (buf < ptail) {
		if (!isalpha(*buf)) {
			buf++;
			continue;
		}

		for (ix = 0; ix < ARRAY_SIZE(key_val); ix++) {
			ulong val;
			struct key_val_t *kv = key_val[ix];

			if (!kv->buf)
				continue;

			if (buf + kv->sz_key > ptail)
				continue;

			if (memcmp(buf, kv->key, kv->sz_key))
				continue;

			if (isalpha(buf[kv->sz_key]))
				continue;

			buf += kv->sz_key;

			if (kv->def_val)
				val = kv->def_val;
			else
				val = (ulong)memparse(buf, &buf);

			memcpy(kv->buf, &val,
				min_t(ulong, kv->sz_buf, sizeof(val)));

			break;
		}

		if (ix == ARRAY_SIZE(key_val)) {
			char chr, *nextbuf;

			nextbuf = buf;

			while (nextbuf < ptail && isalpha(*nextbuf))
				nextbuf++;

			chr = *nextbuf;
			*nextbuf = '\0';

			pr_err("Invalid key word: \"%s\"\n", buf);

			*nextbuf = chr;
			buf = nextbuf;
		}
	}
}
/******************************************************************************/

static void flash_stats_erase(struct flash_stats *stats, u64 addr, u64 len)
{
	int ix;
	struct flash_stats_ctrl *ctrl = 
		container_of(stats, struct flash_stats_ctrl, stats);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (!item->enable || !item->erase)
			continue;

		item->erase(item, addr, len);
	}
}
/******************************************************************************/

static void flash_stats_read(struct flash_stats *stats, u64 addr, u64 len,
			     u8 *ecc)
{
	int ix;
	struct flash_stats_ctrl *ctrl = 
		container_of(stats, struct flash_stats_ctrl, stats);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (!item->enable || !item->read)
			continue;

		item->read(item, addr, len, ecc);
	}
}
/******************************************************************************/

static void flash_stats_write(struct flash_stats *stats, u64 addr, u64 len)
{
	int ix;
	struct flash_stats_ctrl *ctrl = 
		container_of(stats, struct flash_stats_ctrl, stats);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (!item->enable || !item->write)
			continue;

		item->write(item, addr, len);
	}
}
/******************************************************************************/

static void flash_stats_read_retry(struct flash_stats *stats, u64 addr,
				   int read_retries)
{
	int ix;
	struct flash_stats_ctrl *ctrl = 
		container_of(stats, struct flash_stats_ctrl, stats);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (!item->enable || !item->read_retry)
			continue;

		item->read_retry(item, addr, read_retries);
	}
}
/******************************************************************************/

static char *flash_stats_bootargs_options = NULL;

static int __init flash_stats_bootargs_options_setup(char *s)
{
	flash_stats_bootargs_options = s;
	return 1;
}

__setup("flashstats=", flash_stats_bootargs_options_setup);
/******************************************************************************/

static char *find_bootargs_options(struct flash_stats_ctrl *ctrl, int *nr_buf)
{
	int num;
	char *str = flash_stats_bootargs_options;

	if (!str || !*str)
		return NULL;

	num = strlen(ctrl->name);
	if (memcmp(str, ctrl->name, num))
		return NULL;

	str += num;

	if (nr_buf)
		*nr_buf = strlen(str);

	return str;
}
/******************************************************************************/

static int match_items(char *name, char *compatible[])
{
	int ix;

	for (ix = 0; compatible[ix]; ix++)
		if (!strcmp(name, compatible[ix]))
			return 1;

	return 0;
}
/******************************************************************************/

struct flash_stats *flash_stats_create(char *name, int pagesize, int blocksize,
				       u64 totalsize, int nr_ecc_sect,
				       int read_retries)
{
	int ix;
	char *opts;
	int sz_opts = 0;
	struct flash_stats_item *item;
	struct flash_stats_ctrl *ctrl;

	pr_info("Create %s flash statistics interface.\n", name);

	ctrl = kzalloc(sizeof(struct flash_stats_ctrl), GFP_KERNEL);
	if (!ctrl) {
		pr_err("failed to allocate memory for %s.\n", name);
		return NULL;
	}

	strncpy(ctrl->name, name, sizeof(ctrl->name) - 1);

	ctrl->pagesize = pagesize;
	ctrl->pageshift = ffs(pagesize) - 1;

	ctrl->blocksize = blocksize;
	ctrl->blockshift = ffs(blocksize) - 1;

	ctrl->totalsize = totalsize;
	ctrl->need_update = true;
	ctrl->nr_ecc_sect = nr_ecc_sect;

	ctrl->read_retries = read_retries;

	ctrl->dentry_root = debugfs_create_dir(ctrl->name, NULL);
	if (!ctrl->dentry_root) {
		pr_err("create debugfs '%s' fail.\n", ctrl->name);
		kfree(ctrl);
		return NULL;
	}

	ctrl->dentry_stats = debugfs_create_file("stats", 0644,
		ctrl->dentry_root, ctrl, &flash_stats_fops);
	if (!ctrl->dentry_stats) {
		pr_err("create debugfs 'stats' fail.\n");
		debugfs_remove(ctrl->dentry_root);
		kfree(ctrl);
		return NULL;
	}

	for (ix = 0; feature_items[ix]; ix++) {
		if (!match_items(name, feature_items[ix]->compatible))
			continue;

		item = feature_items[ix]->create(ctrl);
		if (!item)
			continue;

		ctrl->items[ctrl->nr_items++] = item;

		if (!item->enable)
			continue;

		item->dentry = debugfs_create_file(item->name, 0644,
			ctrl->dentry_root, item, item->fops);
		if (!item->dentry) {
			pr_err("create debugfs '%s' fail.\n", item->name);
			item->enable = 0;
		}
	}

	if (!ctrl->nr_items) {
		pr_err("not found %s statistics feature.\n", name);
		debugfs_remove(ctrl->dentry_stats);
		debugfs_remove(ctrl->dentry_root);
		kfree(ctrl);
		return NULL;
	}

	ctrl->stats.erase = flash_stats_erase;
	ctrl->stats.read = flash_stats_read;
	ctrl->stats.write = flash_stats_write;
	ctrl->stats.read_retry = flash_stats_read_retry;

	opts = find_bootargs_options(ctrl, &sz_opts);
	if (opts)
		flash_stats_items_resolve(ctrl, opts, sz_opts);

	flash_stats_get_option(ctrl);
	pr_info("%s:%s", ctrl->name, ctrl->prbuf);

	return &ctrl->stats;
}
/******************************************************************************/

void flash_stats_destory(struct flash_stats *stats)
{
	int ix;
	struct flash_stats_ctrl *ctrl = 
		container_of(stats, struct flash_stats_ctrl, stats);

	for (ix = 0; ix < ctrl->nr_items; ix++) {
		struct flash_stats_item *item = ctrl->items[ix];

		if (item->enable)
			debugfs_remove(item->dentry);

		item->destory(item);
	}

	debugfs_remove(ctrl->dentry_stats);
	debugfs_remove(ctrl->dentry_root);

	kfree(ctrl);
}
