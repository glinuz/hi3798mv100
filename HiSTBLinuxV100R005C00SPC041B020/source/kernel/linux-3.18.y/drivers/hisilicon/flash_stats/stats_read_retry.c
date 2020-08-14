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

#include <linux/ctype.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "flash_stats_priv.h"

/******************************************************************************/

#define MAX_RR_REC           (200)

struct rr_log {
	int page_idx;
	int index;
};

struct rr_rec {
	int warn;
	int record;

	int top;
	int nr_rec;

	struct rr_log rec[MAX_RR_REC];

	int stats[32];
};
/******************************************************************************/

static void stats_read_retry(struct flash_stats_item *item, u64 addr, int index)
{
	struct rr_log *log;
	struct rr_rec *rec = (struct rr_rec *)item->priv;

	if (!item->ctrl->read_retries)
		return;

	log = (struct rr_log *)&rec->rec[rec->top];

	if (rec->warn)
		pr_warn("%s: *** page 0x%08llx do read retry\n", item->ctrl->name, addr);

	if (rec->record) {
		log->page_idx = (int)(addr >> item->ctrl->pageshift);
		log->index = index;

		rec->nr_rec++;
		if (++rec->top >= MAX_RR_REC)
			rec->top = 0;

		BUG_ON(index > sizeof(rec->stats));

		rec->stats[index]++;
	}
}
/*****************************************************************************/

static int stats_rr_fops_open(struct inode *inode, struct file *file)
{
	int ix;
	int count;
	char *prbuf;
	int sz_prbuf;
	int read_retries;
	struct rr_rec *rec;
	struct flash_stats_item_filedata *filedata;

	filedata = kzalloc(sizeof(struct flash_stats_item_filedata), GFP_KERNEL);
	if (!filedata)
		return -ENOMEM;

	filedata->item = inode->i_private;
	file->private_data = filedata;

	rec = (struct rr_rec *)filedata->item->priv;

	prbuf = filedata->prbuf;
	sz_prbuf = sizeof(filedata->prbuf);

	count = snprintf(prbuf, sz_prbuf, "%s read retry statistics\n"
		"distribute: ",
		filedata->item->ctrl->name);
	sz_prbuf -= count;
	prbuf += count;

	read_retries = filedata->item->ctrl->read_retries;

	for (ix = 1; ix < read_retries; ix++) {
		count = snprintf(prbuf, sz_prbuf, "%d, ", rec->stats[ix]);
		sz_prbuf -= count;
		prbuf += count;
	}

	count = snprintf(prbuf, sz_prbuf,
		"fail:%d\n"
		"-----------------------------------------------\n",
		rec->stats[read_retries]);
	sz_prbuf -= count;
	prbuf += count;

	filedata->nr_prbuf = prbuf - filedata->prbuf;
	filedata->pos = filedata->nr_prbuf;
	filedata->index = rec->top;

	filedata->count = rec->nr_rec;
	if (filedata->count > MAX_RR_REC)
		filedata->count = MAX_RR_REC;

	return nonseekable_open(inode, file);
}
/******************************************************************************/

static ssize_t stats_rr_fops_read(struct file *filp, char __user *ubuf,
				  size_t sz_ubuf, loff_t *ppos)
{
	int count;
	loff_t offset;
	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct rr_rec *rec = (struct rr_rec *)filedata->item->priv;

	if (*ppos >= (loff_t)filedata->pos) {
		char *prbuf = filedata->prbuf;
		int sz_prbuf = sizeof(filedata->prbuf);
		struct rr_log *log;

		if (filedata->count-- <= 0)
			return 0;

		if (--filedata->index < 0)
			filedata->index = MAX_RR_REC - 1;

		log = (struct rr_log *)&rec->rec[filedata->index];

		count = snprintf(prbuf, sz_prbuf, "%08llx: %d\n",
			((u64)log->page_idx << filedata->item->ctrl->pageshift),
			log->index);
		sz_prbuf -= count;
		prbuf += count;

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

static ssize_t stats_rr_fops_write(struct file *filp, const char __user *ubuf,
				   size_t sz_ubuf, loff_t *ppos)
{
	char *kbuf;
	int clean;
	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct rr_rec *rec = (struct rr_rec *)filedata->item->priv;

	kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}
	kbuf[sz_ubuf] = '\0';

	clean = 0;

	flash_stats_resolve_key(kbuf, kbuf + sz_ubuf, NULL, NULL, &clean,
		&rec->warn, &rec->record);

	if (clean)
		rec->nr_rec = 0;

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static int stats_rr_fops_release(struct inode *inode, struct file *file)
{
	struct flash_stats_item_filedata *filedata = file->private_data;
	kfree(filedata);
	return 0;
}
/******************************************************************************/

static struct file_operations stats_rr_fops = {
	.open = stats_rr_fops_open,
	.read  = stats_rr_fops_read,
	.write = stats_rr_fops_write,
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.release = stats_rr_fops_release,
};
/******************************************************************************/

static struct flash_stats_item *stats_rr_create(struct flash_stats_ctrl *ctrl)
{
	struct rr_rec *rec;
	struct flash_stats_item *item;

	if (!ctrl->read_retries)
		return NULL;

	item = kzalloc(sizeof(struct flash_stats_item) + sizeof(struct rr_rec),
		GFP_KERNEL);
	if (!item) {
		pr_err("failed to allocate memory.\n");
		return NULL;
	}

	rec = (struct rr_rec *)(item + 1);

	rec->warn = true;
	rec->record = true;
	rec->top = 0;
	rec->nr_rec = 0;

	item->priv = rec;

	flash_stats_item_copy(item, &flash_stats_item_rr);

	item->read_retry = stats_read_retry;

	item->ctrl = ctrl;
	item->fops = &stats_rr_fops;

	mutex_init(&item->lock);

	return item;
}
/******************************************************************************/

static void stats_rr_destory(struct flash_stats_item *item)
{
	BUG_ON(item->enable);
	mutex_destroy(&item->lock);
	kfree(item);
}
/******************************************************************************/

struct flash_stats_item flash_stats_item_rr = {
	FLASH_STATS_ITEM_NAME("readretry"),
	.enable = 0,
	.create = stats_rr_create,
	.destory = stats_rr_destory,
	.compatible = {"spinand", "nand", NULL, },
};
