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

#define pr_fmt(fmt) "stats_rwe: " fmt

#include <linux/ctype.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "flash_stats_priv.h"
#include "radix_count.h"

struct rwe_rec {
	/* save read/write/erase count */
	struct radix_u16 radix;

	u32 idx_start; /* display first address */
	u32 idx_end;   /* display end address */
	u32 total;     /* total page/block... */
	int shift;     /* address switch to block/page index */

	int nr_per_line; /* display count per line */
};
/******************************************************************************/

static void stats_rwe(struct flash_stats_item *item, u64 addr, u64 len)
{
	int index;
	int count;
	struct rwe_rec *rec = item->priv;

	index = (int)(addr >> rec->shift);
	count = (int)(len >> rec->shift);

	BUG_ON(index + count > rec->total);

	mutex_lock(&item->lock);

	while (count-- > 0)
		inc_radix_u16(&rec->radix, index++, 1);

	mutex_unlock(&item->lock);
}
/******************************************************************************/

static void stats_read(struct flash_stats_item *item, u64 addr, u64 len, u8 *ecc)
{
	stats_rwe(item, addr, len);
}
/*****************************************************************************/

static int stats_rwe_fops_open(struct inode *inode, struct file *file)
{
	int count;
	char *prbuf;
	int sz_prbuf;

	struct flash_stats_item_filedata *filedata;
	struct rwe_rec *rec;

	filedata = kzalloc(sizeof(struct flash_stats_item_filedata), GFP_KERNEL);
	if (!filedata)
		return -ENOMEM;

	filedata->nr_prbuf = 0;
	filedata->item = inode->i_private;

	file->private_data = filedata;

	rec = filedata->item->priv;

	prbuf = filedata->prbuf;
	sz_prbuf = sizeof(filedata->prbuf);

	count = snprintf(prbuf, sz_prbuf,
		"Print parameter: \"address=0x%08llx,length=0x%08llx\"\n",
		(u64)(rec->idx_start << rec->shift),
		(u64)((rec->idx_end - rec->idx_start) << rec->shift));
	sz_prbuf -= count;
	prbuf += count;

	count = snprintf(prbuf, sz_prbuf,
		"%s\n"
		"------------------------------------------------------\n",
		filedata->item->title);
	sz_prbuf -= count;
	prbuf += count;

	filedata->nr_prbuf = prbuf - filedata->prbuf;
	filedata->pos = filedata->nr_prbuf;
	filedata->index = rec->idx_start;

	return nonseekable_open(inode, file);
}
/******************************************************************************/

static ssize_t stats_rwe_fops_read(struct file *filp, char __user *ubuf,
				   size_t sz_ubuf, loff_t *ppos)
{
	int count;
	loff_t offset;
	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct rwe_rec *rec = filedata->item->priv;

	if (*ppos >= (loff_t)filedata->pos) {
		int num;
		int skip;
		int index;
		int value;

		char *prbuf = filedata->prbuf;
		int sz_prbuf = sizeof(filedata->prbuf);

		if (filedata->index >= rec->idx_end)
			return 0;

		index = (filedata->index / rec->nr_per_line) * rec->nr_per_line;

		count = snprintf(prbuf, sz_prbuf, "%08x: ", index);
		sz_prbuf -= count;
		prbuf += count;

		num = rec->nr_per_line;
		while (num > 0 && index < filedata->index) {
			count = snprintf(prbuf, sz_prbuf, "         %s",
				(num == 5 ? "  " : ""));
			sz_prbuf -= count;
			prbuf += count;

			index++;
			num--;
		}

		mutex_lock(&filedata->item->lock);
		while (num > 0 && filedata->index < rec->idx_end) {
			count = snprintf(prbuf, sz_prbuf, "%04x %s",
				get_radix_u16(&rec->radix, filedata->index),
				(num == 5 ? "  " : ""));
			sz_prbuf -= count;
			prbuf += count;

			filedata->index++;
			num--;
		}
		mutex_unlock(&filedata->item->lock);

		count = snprintf(prbuf, sz_prbuf, "\n");
		sz_prbuf -= count;
		prbuf += count;

		skip = 0;
		value = 0;

		mutex_lock(&filedata->item->lock);
		while (filedata->index + rec->nr_per_line < rec->idx_end) {
			index = filedata->index;
			num = rec->nr_per_line;

			while (num-- > 0 && !value)
				value = get_radix_u16(&rec->radix, index++);

			if (value)
				break;

			if (!skip)
				skip = 1;

			filedata->index += rec->nr_per_line;
		}
		mutex_unlock(&filedata->item->lock);

		if (skip) {
			count = snprintf(prbuf, sz_prbuf, "...\n");
			sz_prbuf -= count;
			prbuf += count;
		}

		if (filedata->index == rec->idx_end) {
			index = filedata->index + rec->nr_per_line - 1;
			index = (index / rec->nr_per_line) * rec->nr_per_line;
			count = snprintf(prbuf, sz_prbuf, "%08x:\n", index);
			sz_prbuf -= count;
			prbuf += count;
		}

		filedata->nr_prbuf = prbuf - filedata->prbuf;
		filedata->pos += filedata->nr_prbuf;
	}

	offset = filedata->nr_prbuf - (int)(filedata->pos - (*ppos));

	count = simple_read_from_buffer(ubuf, sz_ubuf, &offset, filedata->prbuf,
		filedata->nr_prbuf);

	*ppos += count;

	return count;
}
/******************************************************************************/

static ssize_t stats_rwe_fops_write(struct file *filp, const char __user *ubuf,
				    size_t sz_ubuf, loff_t *ppos)
{
	char *kbuf;
	int clean;
	u64 addr, len;
	u32 idx_start, idx_count;

	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct rwe_rec *rec = filedata->item->priv;

	kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}
	kbuf[sz_ubuf] = '\0';

	clean = 0;
	addr = (u64)(rec->idx_start << rec->shift);
	len = (u64)((rec->idx_end - rec->idx_start) << rec->shift);

	flash_stats_resolve_key(kbuf, kbuf + sz_ubuf, &addr, &len, &clean,
		NULL, NULL);

	idx_start = (int)(addr >> rec->shift);
	idx_count = (int)(len >> rec->shift);

	if (idx_start < rec->total && idx_count > 0) {
		if (idx_start + idx_count > rec->total)
			idx_count = rec->total - idx_start;

		rec->idx_start = idx_start;
		rec->idx_end = idx_start + idx_count;
	}

	if (clean) {
		mutex_lock(&filedata->item->lock);
		free_radix_u16(&rec->radix);
		mutex_unlock(&filedata->item->lock);
	}

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static int stats_rwe_fops_release(struct inode *inode, struct file *file)
{
	struct flash_stats_item_filedata *filedata = file->private_data;
	kfree(filedata);
	return 0;
}
/******************************************************************************/

static struct file_operations stats_rwe_fops = {
	.open = stats_rwe_fops_open,
	.read  = stats_rwe_fops_read,
	.write = stats_rwe_fops_write,
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.release = stats_rwe_fops_release,
};
/******************************************************************************/

static void stats_rwe_destory(struct flash_stats_item *item)
{
	struct rwe_rec *rec = item->priv;

	BUG_ON(item->enable);

	mutex_destroy(&item->lock);

	free_radix_u16(&rec->radix);

	kfree(item);
}
/******************************************************************************/

static struct flash_stats_item *
new_stats_rwe_item(struct flash_stats_ctrl *ctrl, struct flash_stats_item *tmp,
		   int shift)
{
	struct rwe_rec *rec;
	struct flash_stats_item *item;

	item = kzalloc(sizeof(struct flash_stats_item) +
		sizeof(struct rwe_rec), GFP_KERNEL);
	if (!item) {
		pr_err("failed to allocate memory.\n");
		return NULL;
	}

	rec = (struct rwe_rec *)(item + 1);
	item->priv = rec;

	flash_stats_item_copy(item, tmp);

	item->ctrl = ctrl;
	item->fops = &stats_rwe_fops;

	mutex_init(&item->lock);

	rec->nr_per_line = 8;
	rec->shift = shift;
	rec->total = (u32)(ctrl->totalsize >> shift);
	rec->idx_start = 0;
	rec->idx_end = rec->total;

	return item;
}
/******************************************************************************/

static struct flash_stats_item *stats_erase_create(struct flash_stats_ctrl *ctrl)
{
	struct flash_stats_item *item;

	item = new_stats_rwe_item(ctrl, &flash_stats_item_erase,
		ctrl->blockshift);
	if (!item)
		return NULL;

	snprintf(item->title, sizeof(item->title), "%s erase count statistics.",
		ctrl->name);

	item->erase = stats_rwe;

	return item;
}
/******************************************************************************/

struct flash_stats_item flash_stats_item_erase = {
	FLASH_STATS_ITEM_NAME("erase"),
	.enable = 0,
	.create = stats_erase_create,
	.destory = stats_rwe_destory,
	.compatible = {"spinand", "nand", "spinor", NULL},
};
/******************************************************************************/

static struct flash_stats_item *stats_read_create(struct flash_stats_ctrl *ctrl)
{
	struct flash_stats_item *item;

	item = new_stats_rwe_item(ctrl, &flash_stats_item_read,
		ctrl->pageshift);
	if (!item)
		return NULL;

	snprintf(item->title, sizeof(item->title), "%s read count statistics.",
		ctrl->name);

	item->read = stats_read;

	return item;
}
/******************************************************************************/

struct flash_stats_item flash_stats_item_read = {
	FLASH_STATS_ITEM_NAME("read"),
	.enable = 0,
	.create = stats_read_create,
	.destory = stats_rwe_destory,
	.compatible = {"spinand", "nand", "emmc", NULL},
};
/******************************************************************************/

static struct flash_stats_item *stats_write_create(struct flash_stats_ctrl *ctrl)
{
	struct flash_stats_item *item;

	item = new_stats_rwe_item(ctrl, &flash_stats_item_write,
		ctrl->pageshift);
	if (!item)
		return NULL;

	snprintf(item->title, sizeof(item->title), "%s write count statistics.",
		ctrl->name);

	item->write = stats_rwe;

	return item;
}
/******************************************************************************/

struct flash_stats_item flash_stats_item_write = {
	FLASH_STATS_ITEM_NAME("write"),
	.enable = 0,
	.create = stats_write_create,
	.destory = stats_rwe_destory,
	.compatible = {"spinand", "nand", "emmc", NULL},
};
