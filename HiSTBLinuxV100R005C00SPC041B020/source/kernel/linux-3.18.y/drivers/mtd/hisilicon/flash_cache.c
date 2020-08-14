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
 * Create By Cai Zhiying 2015.8.15
 *
******************************************************************************/

#define pr_fmt(fmt) "flashcache: " fmt

#include <linux/ctype.h>
#include <linux/mm.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/shrinker.h>
#include <linux/timer.h>
#include <linux/kthread.h>

#include "flash_cache.h"

struct cache_node {
	struct rb_node rbnode;

	u32 readcount; /* page read count */
	u32 pageindex; /* page index */

	char *pagebuf; /* point to page buffer */
	char *oobbuf;  /* point to oob buffer */
};
#define RB_CACHE_ENTRY(_rbnode) \
	rb_entry(_rbnode, struct cache_node, rbnode);

struct cache_ctrl {
	struct flash_cache cache;

	struct rb_root rbroot;

	u32 *pages;  /* each page status */

	char name[32];

	int pagesize;
	int oobsize;

	int pages_per_block;
	u32 max_pages;
	u32 max_caches;

	struct kmem_cache *rbcache;

	struct mutex mutex;
	int nr_lock_read_page;

	struct proc_dir_entry *dentry;

	struct shrinker shrinker;

	/*
	 * true - the cache status change request is enable cache
	 * false - the cache status change request is disable cache
	 */
	bool enable_change_to;

	/*
	 * true - there is a cache status change request,
	 *        disable -> enable or enable -> disable
	 * false - it has finish the cache status change request.
	 */
	bool status_change_request;

	int lifetime;
	struct delayed_work time_work;

	bool read_ahead_runing; /* read ahead thread status. */

	struct task_struct *read_ahead_task;

	int (*read_ahead)(void *args, int pageindex, int *nr_pages);
	void *read_ahead_args;

	/* proc file buffer, support one thread. */
	char prbuf[2048];
	int sz_prbuf;

	u32 nr_caches;     /* cache node number */
	u32 nr_read_total; /* total read count */
	u32 nr_read_hit;   /* read and found the cache */
	u32 nr_read_miss;  /* not found the cache, new one */
	u32 sz_caches;     /* cache memory size */
	u32 sz_cache_node; /* each cache node size */
};

/* this page need be free when unlock page */
#define PAGE_LOCK_NEED_FREE           (0x10000000)
/* this page is lock by read or by remove */
#define PAGE_LOCK_READ                (0x8000000)

#define PAGE_STATUS_MASK              (0x03000000)

#define PAGE_READ_COUNT_MASK          (0x00FFFFFF)

#define PAGE_IS_LOCK_BY_READ(_ctrl, _index) \
	(_ctrl->pages[_index] & PAGE_LOCK_READ)

#define PAGE_LOCK_BY_READ(_ctrl, _index) \
	_ctrl->pages[_index] |= PAGE_LOCK_READ

#define PAGE_UNLOCK_BY_READ(_ctrl, _index) ({                           \
	u32 ____status = _ctrl->pages[_index];                          \
	_ctrl->pages[_index] &= ~(PAGE_LOCK_READ | PAGE_LOCK_NEED_FREE);\
	____status;                                                     \
})

#define GET_STATUS(_ctrl, _index) \
	(_ctrl->pages[_index] & PAGE_STATUS_MASK)

#define SET_STATUS(_ctrl, _index, _status) ({                   \
	_ctrl->pages[_index] &= ~PAGE_STATUS_MASK;              \
	_ctrl->pages[_index] |= ((_status) & PAGE_STATUS_MASK); \
	(_ctrl->pages[_index] & PAGE_STATUS_MASK);              \
})

#define GET_READ_COUNT(_ctrl, _index) \
	(_ctrl->pages[_index] & PAGE_READ_COUNT_MASK)

#define SET_READ_COUNT(_ctrl, _index, _count) ({                              \
	u32 ___count = (_count);                                              \
	if (___count > PAGE_READ_COUNT_MASK) ___count = PAGE_READ_COUNT_MASK; \
	_ctrl->pages[_index] &= ~PAGE_READ_COUNT_MASK;                        \
	_ctrl->pages[_index] |= ___count;                                     \
	___count;                                                             \
})

/* close flash cache after 30s */
static u32 flash_cache_lifetime = 0;
static int flash_cache_read_ahead_enable = 0;
static u64 flash_cache_read_ahead_address = 0;
/******************************************************************************/

static int __init flash_cache_bootargs_options_setup(char *s)
{
	flash_cache_lifetime = (u32)simple_strtol(s, &s, 10);
	if (*s++ != ',')
		return 1;

	flash_cache_read_ahead_enable = !!(*s++ - '0');

	if (*s++ != ',')
		return 1;

	flash_cache_read_ahead_address = (u64)simple_strtoll(s, &s, 0);

	return 1;
}

/*  flashcache=30,1,0x100000 */
__setup("flashcache=", flash_cache_bootargs_options_setup);
/******************************************************************************/

static int page_compare(struct cache_node *node, u32 readcount, u32 pageindex)
{
	if (node->readcount > readcount)
		return -1;
	else if (node->readcount < readcount)
		return 1;

	if (node->pageindex > pageindex)
		return -1;
	else if (node->pageindex < pageindex)
		return 1;

	return 0;
}
/******************************************************************************/

static struct cache_node *find_cache_node(struct cache_ctrl *ctrl,
					  u32 readcount, u32 pageindex)
{
	int comp;
	struct rb_node *node = ctrl->rbroot.rb_node;

	while (node) {
		struct cache_node *leaf = RB_CACHE_ENTRY(node);
		comp = page_compare(leaf, readcount, pageindex);

		if (comp < 0)
			node = node->rb_left;
		else if (comp > 0)
			node = node->rb_right;
		else
			return leaf;
	}

	return NULL;
}
/******************************************************************************/

static void remove_cache_node(struct cache_ctrl *ctrl, struct cache_node *node,
			      bool free_node)
{
	rb_erase(&node->rbnode, &ctrl->rbroot);

	ctrl->nr_caches--;

	if (free_node) {
		kfree(node->pagebuf);
		kfree(node->oobbuf);
		kmem_cache_free(ctrl->rbcache, node);

		ctrl->sz_caches -= ctrl->sz_cache_node;
	}
}
/******************************************************************************/

static void free_all_cache_node(struct cache_ctrl *ctrl)
{
	struct rb_node *rbnode = rb_first(&ctrl->rbroot);

	while (rbnode) {
		struct cache_node *node = RB_CACHE_ENTRY(rbnode);
		rbnode = rb_next(rbnode);

		remove_cache_node(ctrl, node, true);
	}
}
/******************************************************************************/

static struct cache_node *new_cache_node(struct cache_ctrl *ctrl, u32 readcount,
					 u32 pageindex)
{
	struct cache_node *node;
	gfp_t flags = (GFP_NOWAIT | __GFP_NOMEMALLOC | __GFP_NOWARN);

	node = kmem_cache_zalloc(ctrl->rbcache, flags);
	if (!node)
		return NULL;

	node->pagebuf = kmalloc(ctrl->pagesize, flags);
	if (!node->pagebuf) {
		kmem_cache_free(ctrl->rbcache, node);
		return NULL;
	}

	node->oobbuf = kmalloc(ctrl->oobsize, flags);
	if (!node->oobbuf) {
		kfree(node->pagebuf);
		kmem_cache_free(ctrl->rbcache, node);
		return NULL;
	}

	node->readcount = readcount;
	node->pageindex = pageindex;
	ctrl->sz_caches += ctrl->sz_cache_node;

	return node;
}
/******************************************************************************/

static struct cache_node *
insert_cache_node(struct cache_ctrl *ctrl, struct cache_node *newnode)
{
	int comp;
	struct rb_node **node = &ctrl->rbroot.rb_node;
	struct rb_node *parent = NULL;

	while (*node) {
		struct cache_node *leaf = RB_CACHE_ENTRY(*node);
		comp = page_compare(leaf, newnode->readcount,
			newnode->pageindex);

		parent = *node;

		if (comp < 0)
			node = &(*node)->rb_left;
		else if (comp > 0)
			node = &(*node)->rb_right;
		else
			return NULL;
	}

	rb_link_node(&newnode->rbnode, parent, node);
	rb_insert_color(&newnode->rbnode, &ctrl->rbroot);

	ctrl->nr_caches++;

	return newnode;
}
/******************************************************************************/

static int _new_cache(struct flash_cache *cache, u32 pageindex, char **pagebuf,
		      char **oobbuf)
{
	u32 readcount;
	struct cache_node *node;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable)
		return -EINVAL;

	mutex_lock(&ctrl->mutex);

	if (GET_STATUS(ctrl, pageindex) != FLASHCACHE_PAGE_UNKNOWN) {
		mutex_unlock(&ctrl->mutex);
		return -EINVAL;
	}

	readcount = GET_READ_COUNT(ctrl, pageindex);
	readcount = SET_READ_COUNT(ctrl, pageindex, readcount + 1);

	if (ctrl->nr_caches >= ctrl->max_caches) {
		node = RB_CACHE_ENTRY(rb_first(&ctrl->rbroot));

		if (readcount < node->readcount) {
			mutex_unlock(&ctrl->mutex);
			return -ENOMEM;
		}

		SET_STATUS(ctrl, node->pageindex, FLASHCACHE_PAGE_UNKNOWN);
		remove_cache_node(ctrl, node, true);
	}

	node = new_cache_node(ctrl, readcount, pageindex);
	if (!node) {
		mutex_unlock(&ctrl->mutex);
		return -ENOMEM;
	}

	SET_STATUS(ctrl, pageindex, FLASHCACHE_PAGE_CACHE);

	if (pagebuf)
		*pagebuf = node->pagebuf;
	if (oobbuf)
		*oobbuf = node->oobbuf;

	insert_cache_node(ctrl, node);

	ctrl->nr_read_miss++;

	mutex_unlock(&ctrl->mutex);

	return 0;
}
/******************************************************************************/

static int _get_cache(struct flash_cache *cache, u32 pageindex, char **pagebuf,
		     char **oobbuf)
{
	int status;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable)
		return FLASHCACHE_DISABLE;

	mutex_lock(&ctrl->mutex);

	status = GET_STATUS(ctrl, pageindex);

	if (status == FLASHCACHE_PAGE_CACHE) {
		u32 readcount;
		struct cache_node *node = find_cache_node(ctrl,
			GET_READ_COUNT(ctrl, pageindex), pageindex);

		if (pagebuf)
			*pagebuf = node->pagebuf;
		if (oobbuf)
			*oobbuf = node->oobbuf;

		remove_cache_node(ctrl, node, false);

		readcount = GET_READ_COUNT(ctrl, pageindex);
		node->readcount = SET_READ_COUNT(ctrl, pageindex, readcount + 1);

		insert_cache_node(ctrl, node);

		ctrl->nr_read_hit++;
	}

	mutex_unlock(&ctrl->mutex);

	return status;
}
/******************************************************************************/

static int _discard_cache(struct flash_cache *cache, u32 pageindex,
			  int nr_pages, int status)
{
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable)
		return 0;

	for (; nr_pages > 0; nr_pages--, pageindex++) {
		mutex_lock(&ctrl->mutex);

		if (GET_STATUS(ctrl, pageindex) == FLASHCACHE_PAGE_CACHE) {
			struct cache_node *node = find_cache_node(ctrl,
				GET_READ_COUNT(ctrl, pageindex), pageindex);
			remove_cache_node(ctrl, node, true);
		}

		SET_STATUS(ctrl, pageindex, status);
		SET_READ_COUNT(ctrl, pageindex, 0);

		mutex_unlock(&ctrl->mutex);
	}

	return 0;
}
/******************************************************************************/

static int __read_ahead_thread(void *args)
{
	int pageshift;
	int pageindex = 0;
	int skip_pages = 0;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)args;

	pageshift = ffs(ctrl->pagesize) - 1;

	pageindex = (int)(flash_cache_read_ahead_address >> pageshift);

	pr_info("%s read ahead start from address:0x%08llx.\n", ctrl->name,
		((u64)pageindex << pageshift));

	while (pageindex < ctrl->max_pages) {
		cond_resched();
		ctrl->read_ahead(ctrl->read_ahead_args, pageindex, &skip_pages);
		if (skip_pages <= 0 || skip_pages >= ctrl->max_pages)
			break;
		pageindex += skip_pages;

		if (kthread_should_stop())
			break;
	}

	pr_info("%s read ahead finish.\n", ctrl->name);

	ctrl->read_ahead_runing = false;

	return 0;
}
/******************************************************************************/

static int start_read_ahead_thread(struct cache_ctrl *ctrl)
{
	struct task_struct *task;

	if (!ctrl->read_ahead)
		return 0;

	if (ctrl->read_ahead_runing)
		return 0;

	ctrl->read_ahead_runing = true;

	task = kthread_run(__read_ahead_thread, ctrl, "ra-%s", ctrl->name);
	if (IS_ERR(task)) {
		pr_warning("Failed to create the %s read ahead thread\n", ctrl->name);
		ctrl->read_ahead_runing = false;
		return IS_ERR(task);
	}

	ctrl->read_ahead_task = task;

	return 0;
}
/******************************************************************************/

static void stop_read_ahead_thread(struct cache_ctrl *ctrl)
{
	if (ctrl->read_ahead_task) {
		kthread_stop(ctrl->read_ahead_task);
		ctrl->read_ahead_task = NULL;
	}
}
/******************************************************************************/

static bool flash_cache_try_enable(struct cache_ctrl *ctrl)
{
	mutex_lock(&ctrl->mutex);

	if (ctrl->status_change_request && ctrl->enable_change_to) {
		ctrl->status_change_request = false;

		if (!ctrl->cache.enable) {
			ctrl->pages = kzalloc(
				ctrl->max_pages * sizeof(ctrl->pages),
				GFP_KERNEL);
			if (!ctrl->pages) {
				pr_err("failed to allocate memory.\n");
			} else {
				ctrl->cache.enable = true;
				start_read_ahead_thread(ctrl);
			}
		}
	}

	mutex_unlock(&ctrl->mutex);

	return ctrl->cache.enable;
}
/******************************************************************************/

static bool flash_cache_try_disable(struct cache_ctrl *ctrl)
{
	mutex_lock(&ctrl->mutex);

	if (ctrl->status_change_request && !ctrl->enable_change_to &&
	    !ctrl->nr_lock_read_page) {
		ctrl->status_change_request = false;

		if (ctrl->cache.enable) {
			stop_read_ahead_thread(ctrl);
			free_all_cache_node(ctrl);

			ctrl->cache.enable = false;

			kfree(ctrl->pages);
			ctrl->pages = NULL;
		}
	}

	mutex_unlock(&ctrl->mutex);

	return ctrl->cache.enable;
}
/******************************************************************************/

static int _peek_status(struct flash_cache *cache, u32 pageindex)
{
	int status;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable)
		return FLASHCACHE_DISABLE;

	mutex_lock(&ctrl->mutex);
	status = GET_STATUS(ctrl, pageindex);
	mutex_unlock(&ctrl->mutex);

	return status;
}
/******************************************************************************/

static int _get_status_lock_read_page(struct flash_cache *cache, u32 pageindex)
{
	int status;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable && !flash_cache_try_enable(ctrl))
		return FLASHCACHE_DISABLE;

	mutex_lock(&ctrl->mutex);

	PAGE_LOCK_BY_READ(ctrl, pageindex);

	ctrl->nr_lock_read_page++;
	ctrl->nr_read_total++;

	status = GET_STATUS(ctrl, pageindex);

	mutex_unlock(&ctrl->mutex);

	return status;
}
/******************************************************************************/

static void flash_cache_remove(struct cache_ctrl *ctrl, int nr_caches)
{
	struct cache_node *node, *next;

	mutex_lock(&ctrl->mutex);

	node = RB_CACHE_ENTRY(rb_first(&ctrl->rbroot));

	while (nr_caches-- > 0 && node) {
		next = RB_CACHE_ENTRY(rb_next(&node->rbnode));

		if (PAGE_IS_LOCK_BY_READ(ctrl, node->pageindex)) {
			ctrl->pages[node->pageindex] |= PAGE_LOCK_NEED_FREE;
		} else {
			SET_STATUS(ctrl, node->pageindex, FLASHCACHE_PAGE_UNKNOWN);
			SET_READ_COUNT(ctrl, node->pageindex, 0);

			remove_cache_node(ctrl, node, true);
		}

		node = next;
	}

	mutex_unlock(&ctrl->mutex);
}
/******************************************************************************/

static int _unlock_read_page(struct flash_cache *cache, u32 pageindex)
{
	u32 lock_status;
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (!cache->enable)
		return FLASHCACHE_DISABLE;

	mutex_lock(&ctrl->mutex);

	lock_status = PAGE_UNLOCK_BY_READ(ctrl, pageindex);

	if ((lock_status & PAGE_LOCK_NEED_FREE) &&
	    GET_STATUS(ctrl, pageindex) == FLASHCACHE_PAGE_CACHE) {
		struct cache_node *node = find_cache_node(ctrl,
			GET_READ_COUNT(ctrl, pageindex), pageindex);

		SET_STATUS(ctrl, pageindex, FLASHCACHE_PAGE_UNKNOWN);
		SET_READ_COUNT(ctrl, pageindex, 0);

		remove_cache_node(ctrl, node, true);
	}

	ctrl->nr_lock_read_page--;

	mutex_unlock(&ctrl->mutex);

	flash_cache_try_disable(ctrl);

	return 0;
}
/******************************************************************************/

static void flash_cache_enable(struct cache_ctrl *ctrl)
{
	mutex_lock(&ctrl->mutex);

	ctrl->enable_change_to = true;
	ctrl->status_change_request = true;
	pr_info("%s cache will enable.\n", ctrl->name);

	mutex_unlock(&ctrl->mutex);
}
/******************************************************************************/

static void flash_cache_disable(struct cache_ctrl *ctrl)
{
	mutex_lock(&ctrl->mutex);

	ctrl->enable_change_to = false;
	ctrl->status_change_request = true;
	pr_info("%s cache will disable.\n", ctrl->name);

	mutex_unlock(&ctrl->mutex);

	flash_cache_remove(ctrl, ctrl->max_caches);

	flash_cache_try_disable(ctrl);
}
/******************************************************************************/

static ssize_t proc_fops_read(struct file *filp, char __user *ubuf,
			      size_t sz_ubuf, loff_t *ppos)
{
	struct cache_ctrl *ctrl = PDE_DATA(file_inode(filp));

	if (!*ppos) {
		int count;
		char *ptr = ctrl->prbuf;
		int sz_buf = sizeof(ctrl->prbuf);

		count = snprintf(ptr, sz_buf,
			"Cache:         %s\n"
			"Pending:       %s\n"
			"Max Cache:     %d\n"
			"Total Pages:   %d\n"
			"Cache Number:  %d\n"
			"Cache Size:    %d\n"
			"Read Total:    %d\n"
			"Read Hit:      %d\n"
			"Read Miss:     %d\n"
			"Read Ahead:    %s\n",
			(ctrl->cache.enable ? "on" : "off"),
			(ctrl->enable_change_to ? "on" : "off"),
			ctrl->max_caches,
			ctrl->max_pages,
			ctrl->nr_caches,
			ctrl->sz_caches,
			ctrl->nr_read_total,
			ctrl->nr_read_hit,
			ctrl->nr_read_miss,
			(ctrl->read_ahead_runing ? "runing" : "stop"));

		sz_buf -= count;
		ptr += count;

		ctrl->sz_prbuf = ptr - ctrl->prbuf;
	}

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, ctrl->prbuf,
		ctrl->sz_prbuf);
}
/******************************************************************************/

static void resolve_input(char *buf, char *ptail, int *clean, int *enable)
{
	int ix;
	char buf_enabe[4] = {0};

#define _SKEY_VAL(_key, _val, _sz_val, _def)  \
	((void *)&(struct key_val_t){         \
		.key     = _key,              \
		.sz_key  = sizeof(_key) - 1,  \
		.buf     = (char *)_val,      \
		.sz_buf  = _sz_val,           \
		.def_val = _def,              \
	})

	struct key_val_t {
		const char *key;
		int sz_key;
		char *buf;
		int sz_buf;
		int def_val;
	} *key_val[] = {
		_SKEY_VAL("cache", buf_enabe, sizeof(buf_enabe),   2),
		_SKEY_VAL("clean", clean,     sizeof(*clean),      1),
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

			if (kv->def_val == 1) {
				val = kv->def_val;
				memcpy(kv->buf, &val,
					min_t(ulong, kv->sz_buf, sizeof(val)));
			} else if (kv->def_val == 2) {
				char *ptr = kv->buf;
				int count = kv->sz_buf - 1;

				buf = skip_spaces(buf);

				while (count-- > 0 && isalpha(*buf))
					*ptr++ = *buf++;

				*ptr = '\0';

			} else {
				val = (ulong)memparse(buf, &buf);
				memcpy(kv->buf, &val,
					min_t(ulong, kv->sz_buf, sizeof(val)));
			}

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

	if (buf_enabe[0] && enable) {
		if (!strncmp(buf_enabe, "on", sizeof(buf_enabe)))
			*enable = 1;
		else if (!strncmp(buf_enabe, "off", sizeof(buf_enabe)))
			*enable = 0;
	}
}
/******************************************************************************/

static ssize_t proc_fops_write(struct file *filp, const char __user *ubuf,
			       size_t sz_ubuf, loff_t *ppos)
{
	char *kbuf;
	int enable, clean;
	struct cache_ctrl *ctrl = PDE_DATA(file_inode(filp));

	kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}

	kbuf[sz_ubuf] = '\0';

	enable = ctrl->enable_change_to;
	clean = 0;

	resolve_input(kbuf, kbuf + sz_ubuf, &clean, &enable);

	if (enable != ctrl->enable_change_to) {
		if (enable)
			flash_cache_enable(ctrl);
		else
			flash_cache_disable(ctrl);
	}

	if (clean)
		flash_cache_remove(ctrl, ctrl->max_caches);

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static struct file_operations flash_cache_proc_fops = {
	.read = proc_fops_read,
	.write = proc_fops_write,
};
/******************************************************************************/

static unsigned long
flash_cache_scan_objects(struct shrinker *shrink, struct shrink_control *sc)
{
	int count;
	u32 nr_caches;
	struct cache_ctrl *ctrl;

	ctrl = container_of(shrink, struct cache_ctrl, shrinker);

	if (!sc->nr_to_scan)
		return ctrl->nr_caches;

	nr_caches = ctrl->nr_caches;
	flash_cache_remove(ctrl, sc->nr_to_scan);
	count = nr_caches - ctrl->nr_caches;

	if (count <= 0)
		return SHRINK_STOP;
	else
		return count;
}
/******************************************************************************/

static unsigned long
flash_cache_count_objects(struct shrinker *shrink, struct shrink_control *sc)
{
	struct cache_ctrl *ctrl;
	ctrl = container_of(shrink, struct cache_ctrl, shrinker);
	return ctrl->nr_caches;
}
/******************************************************************************/

static void __flash_cache_time_work(struct work_struct *work)
{
	struct cache_ctrl *ctrl = container_of(work, struct cache_ctrl,
		time_work.work);

	flash_cache_disable(ctrl);
}
/******************************************************************************/

struct flash_cache *flash_cache_create(char *name, u64 totalsize, int blocksize,
				       int pagesize, int oobsize, int nr_cache,
				       int (*read_ahead)(void *, int, int*),
				       void *read_ahead_args)
{
	struct cache_ctrl *ctrl;

	pr_info("initialize %s flash cache feature.\n", name);

	ctrl = kzalloc(sizeof(struct cache_ctrl), GFP_KERNEL);
	if (!ctrl) {
		pr_err("failed to allocate memory.\n");
		return NULL;
	}

	strncpy(ctrl->name, name, sizeof(ctrl->name) - 1);
	ctrl->pages_per_block = blocksize / pagesize;
	ctrl->pagesize = pagesize;
	ctrl->oobsize = oobsize;
	ctrl->max_pages = (u32)(totalsize >> (ffs(pagesize) - 1));
	ctrl->max_caches = (nr_cache <= 0 ? ctrl->max_pages : nr_cache);

	ctrl->sz_cache_node = sizeof(struct cache_node) + ctrl->pagesize +
		ctrl->oobsize;

	if (flash_cache_read_ahead_enable) {
		ctrl->read_ahead = read_ahead;
		ctrl->read_ahead_args = read_ahead_args;
		ctrl->read_ahead_runing = false;
	}

	ctrl->rbcache = kmem_cache_create("flashcache",
		sizeof(struct cache_node), 0, 0, NULL);
	if (!ctrl->rbcache) {
		kfree(ctrl);
		pr_err("failed to allocate memory.\n");
		return NULL;
	}

	ctrl->dentry = proc_create_data("nandcache", 0644, NULL,
		&flash_cache_proc_fops, ctrl);
	if (!ctrl->dentry) {
		kmem_cache_destroy(ctrl->rbcache);
		kfree(ctrl);
		pr_err("create proc file fail.\n");
		return NULL;
	}

	ctrl->shrinker.scan_objects = flash_cache_scan_objects;
	ctrl->shrinker.count_objects = flash_cache_count_objects;
	ctrl->shrinker.seeks = DEFAULT_SEEKS;
	register_shrinker(&ctrl->shrinker);

	mutex_init(&ctrl->mutex);

	ctrl->cache.oobsize = oobsize;
	ctrl->cache.max_pages = ctrl->max_pages;
	ctrl->cache.pages_per_block = ctrl->pages_per_block;
	ctrl->cache.get_status_lock_read_page = _get_status_lock_read_page;
	ctrl->cache.new_cache = _new_cache;
	ctrl->cache.get_cache = _get_cache;
	ctrl->cache.discard_cache = _discard_cache;
	ctrl->cache.unlock_read_page = _unlock_read_page;
	ctrl->cache.peek_status = _peek_status;

	ctrl->lifetime = flash_cache_lifetime;
	if (ctrl->lifetime) {
		bool ret;
		INIT_DELAYED_WORK(&ctrl->time_work, __flash_cache_time_work);
		schedule_delayed_work(&ctrl->time_work,
			msecs_to_jiffies(ctrl->lifetime * 1000));

		flash_cache_enable(ctrl);
		ret = flash_cache_try_enable(ctrl);
		if (ret)
			pr_info("%s cache will disable after %ds.\n", ctrl->name, ctrl->lifetime);
		else
			pr_info("%s cache enable failed.\n", ctrl->name);
	}

	return &ctrl->cache;
}
/******************************************************************************/

void flash_cache_destory(struct flash_cache *cache)
{
	struct cache_ctrl *ctrl = (struct cache_ctrl *)cache;

	if (ctrl->lifetime)
		cancel_delayed_work_sync(&ctrl->time_work);

	flash_cache_disable(ctrl);
	flash_cache_try_disable(ctrl);

	kmem_cache_destroy(ctrl->rbcache);

	unregister_shrinker(&ctrl->shrinker);

	mutex_destroy(&ctrl->mutex);

	kfree(ctrl);
}
