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

#define MAX_ECC_REC           (200)
struct ecc_rec {
	int warn;
	int record;

	int nr_ecc_sect;
	int top;
	int nr_rec;
	u8 *rec[MAX_ECC_REC];
};

struct ecc_log {
	int page_idx;
	char ecc[4];
};
/******************************************************************************/

static void stats_ecc_read(struct flash_stats_item *item, u64 addr, u64 len,
			   u8 ecc[])
{
	int ix;
	int unecc = 0;

	struct ecc_log *log;
	struct ecc_rec *rec = (struct ecc_rec *)item->priv;

	log = (struct ecc_log *)rec->rec[rec->top];

	for (ix = 0; ix < rec->nr_ecc_sect; ix++) {
		log->ecc[ix] = ecc[ix];
		if (ecc[ix] == 0xFF && !unecc)
			unecc = 1;
	}

	if (unecc && rec->warn) {
		int count;
		char buf[65];

		char *ptr = buf;
		int nr_buf = sizeof(buf) - 1;

		for (ix = 0; ix < rec->nr_ecc_sect; ix++) {
			if (ecc[ix] == 0xFF)
				count = snprintf(ptr, nr_buf, "*,");
			else
				count = snprintf(ptr, nr_buf, "%d,", ecc[ix]);
			ptr += count;
			nr_buf -= count;
		}

		pr_warn("%s: *** page 0x%08llx has uncorrect ecc. section:%s\n",
			item->ctrl->name, addr, buf);
	}

	if (rec->record) {
		log->page_idx = (int)(addr >> item->ctrl->pageshift);

		rec->nr_rec++;
		if (++rec->top >= MAX_ECC_REC)
			rec->top = 0;
	}
}
/*****************************************************************************/

static int stats_ecc_fops_open(struct inode *inode, struct file *file)
{
	int count;
	char *prbuf;
	int sz_prbuf;
	struct ecc_rec *rec;
	struct flash_stats_item_filedata *filedata;

	filedata = kzalloc(sizeof(struct flash_stats_item_filedata), GFP_KERNEL);
	if (!filedata)
		return -ENOMEM;

	filedata->item = inode->i_private;
	file->private_data = filedata;

	rec = (struct ecc_rec *)filedata->item->priv;

	prbuf = filedata->prbuf;
	sz_prbuf = sizeof(filedata->prbuf);

	count = snprintf(prbuf, sz_prbuf,
		"%s ecc statistics\n"
		"-----------------------------------------------\n",
		filedata->item->ctrl->name);
	sz_prbuf -= count;
	prbuf += count;

	filedata->nr_prbuf = prbuf - filedata->prbuf;
	filedata->pos = filedata->nr_prbuf;
	filedata->index = rec->top;

	filedata->count = rec->nr_rec;
	if (filedata->count > MAX_ECC_REC)
		filedata->count = MAX_ECC_REC;

	return nonseekable_open(inode, file);
}
/******************************************************************************/

static ssize_t stats_ecc_fops_read(struct file *filp, char __user *ubuf,
				   size_t sz_ubuf, loff_t *ppos)
{
	int count;
	loff_t offset;
	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct ecc_rec *rec = (struct ecc_rec *)filedata->item->priv;

	if (*ppos >= (loff_t)filedata->pos) {
		int ix;
		char *prbuf = filedata->prbuf;
		int sz_prbuf = sizeof(filedata->prbuf);
		struct ecc_log *log;

		if (filedata->count-- <= 0)
			return 0;

		if (--filedata->index < 0)
			filedata->index = MAX_ECC_REC - 1;

		log = (struct ecc_log *)rec->rec[filedata->index];

		count = snprintf(prbuf, sz_prbuf, "%08llx: ",
			((u64)log->page_idx << filedata->item->ctrl->pageshift));
		sz_prbuf -= count;
		prbuf += count;

		for (ix = 0; ix < rec->nr_ecc_sect; ix++) {
			count = snprintf(prbuf, sz_prbuf, "%3d, ", log->ecc[ix]);
			sz_prbuf -= count;
			prbuf += count;
		}

		count = snprintf(prbuf, sz_prbuf, "\n");
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

static ssize_t stats_ecc_fops_write(struct file *filp, const char __user *ubuf,
				    size_t sz_ubuf, loff_t *ppos)
{
	char *kbuf;
	int clean;
	struct flash_stats_item_filedata *filedata = filp->private_data;
	struct ecc_rec *rec = (struct ecc_rec *)filedata->item->priv;

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

static int stats_ecc_fops_release(struct inode *inode, struct file *file)
{
	struct flash_stats_item_filedata *filedata = file->private_data;
	kfree(filedata);
	return 0;
}
/******************************************************************************/

static struct file_operations stats_ecc_fops = {
	.open = stats_ecc_fops_open,
	.read  = stats_ecc_fops_read,
	.write = stats_ecc_fops_write,
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.release = stats_ecc_fops_release,
};
/******************************************************************************/

static struct flash_stats_item *stats_ecc_create(struct flash_stats_ctrl *ctrl)
{
	int ix;
	int size;
	char *buf;
	struct ecc_rec *rec;
	struct flash_stats_item *item;

	size = sizeof(struct flash_stats_item) + sizeof(struct ecc_rec) +
		MAX_ECC_REC * (8 + ctrl->nr_ecc_sect);

	item = kzalloc(size, GFP_KERNEL);
	if (!item) {
		pr_err("failed to allocate memory.\n");
		return NULL;
	}

	rec = (struct ecc_rec *)(item + 1);

	rec->warn = true;
	rec->record = true;
	rec->nr_ecc_sect = ctrl->nr_ecc_sect;
	rec->top = 0;
	rec->nr_rec = 0;

	buf = (char *)(rec + 1);
	size = (4 + 3 + rec->nr_ecc_sect) & ~3;
	for (ix = 0; ix < MAX_ECC_REC; ix++) {
		rec->rec[ix] = buf;
		buf += size;
	}

	item->priv = rec;

	flash_stats_item_copy(item, &flash_stats_item_ecc);

	item->read = stats_ecc_read;

	item->ctrl = ctrl;
	item->fops = &stats_ecc_fops;

	mutex_init(&item->lock);

	return item;
}
/******************************************************************************/

static void stats_ecc_destory(struct flash_stats_item *item)
{
	BUG_ON(item->enable);
	mutex_destroy(&item->lock);
	kfree(item);
}
/******************************************************************************/

struct flash_stats_item flash_stats_item_ecc = {
	FLASH_STATS_ITEM_NAME("ecc"),
	.enable = 0,
	.create = stats_ecc_create,
	.destory = stats_ecc_destory,
	.compatible = {"spinand", "nand", NULL,},
};
