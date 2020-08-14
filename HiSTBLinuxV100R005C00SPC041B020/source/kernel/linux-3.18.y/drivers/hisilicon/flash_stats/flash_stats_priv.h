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

#ifndef FLASH_STATS_PRIV_H
#define FLASH_STATS_PRIV_H

/******************************************************************************/

#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/hisilicon/flash_stats.h>

/******************************************************************************/
struct flash_stats_item;

struct flash_stats_ctrl {
	struct flash_stats stats;

	char name[8];

	int pagesize;
	int pageshift;

	int blocksize;
	int blockshift;

	int nr_ecc_sect;

	int read_retries;

	u64 totalsize;

	struct dentry *dentry_root;
	struct dentry *dentry_stats;

	int need_update;
	char prbuf[1024];
	int sz_prbuf;

	struct flash_stats_item *items[10];
	int nr_items;
};
/******************************************************************************/

#define FLASH_STATS_ITEM_NAME(_name) \
	.name = _name, .sz_name = (sizeof(_name) - 1)

struct flash_stats_item {
	char *compatible[8];
	char *name;
	int sz_name;

	int enable;  /* interface status, enable/disable */

	void (*read)(struct flash_stats_item *item, u64 addr, u64 len, u8 *ecc);
	void (*write)(struct flash_stats_item *item, u64 addr, u64 len);
	void (*erase)(struct flash_stats_item *item, u64 addr, u64 len);

	void (*read_retry)(struct flash_stats_item *item, u64 addr, int index);

	struct flash_stats_item *(*create)(struct flash_stats_ctrl *ctrl);
	void (*destory)(struct flash_stats_item *item);

	struct flash_stats_ctrl *ctrl;

	struct mutex lock;
	struct dentry *dentry;
	struct file_operations *fops;

	char title[32];

	void *priv;
};

static inline struct flash_stats_item *
flash_stats_item_copy(struct flash_stats_item *to, struct flash_stats_item *from)
{
	to->name = from->name;
	to->sz_name = from->sz_name;
	to->create = from->create;
	to->destory = from->destory;
	to->enable = from->enable;

	return to;
}

struct flash_stats_item_filedata {
	struct flash_stats_item *item;
	u32 pos;
	int index;
	int count;
	int nr_prbuf;
	char prbuf[2048];
};

struct key_val_t {
	const char *key;
	int sz_key;
	char *buf;
	int sz_buf;
	int def_val;
};

#define _SKEY_VAL(_key, _val, _sz_val, _def)  \
	((void *)&(struct key_val_t){         \
		.key     = _key,              \
		.sz_key  = sizeof(_key) - 1,  \
		.buf     = (char *)_val,      \
		.sz_buf  = _sz_val,           \
		.def_val = _def,              \
})

/******************************************************************************/

void flash_stats_resolve_key(char *buf, char *ptail, u64 *addr, u64 *len,
			     int *clean, int *warn, int *record);

extern struct flash_stats_item flash_stats_item_erase;
extern struct flash_stats_item flash_stats_item_read;
extern struct flash_stats_item flash_stats_item_write;
extern struct flash_stats_item flash_stats_item_ecc;
extern struct flash_stats_item flash_stats_item_rr;
/******************************************************************************/

#endif /* FLASH_STATS_PRIV_H */

