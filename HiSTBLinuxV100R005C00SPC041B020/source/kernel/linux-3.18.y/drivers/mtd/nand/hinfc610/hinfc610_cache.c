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
 * Create By Cai Zhiying 2014.11.7
 *
******************************************************************************/

#define pr_fmt(fmt) "hinfc_cache: " fmt

#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/crc32.h>

#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/rbtree.h>
#include <linux/mm.h>

#include "hinfc610_cache.h"

struct hinfc_cache {
	struct rb_root rbroot;

#define HAVE_CACHE(_page)       ((_page) & CACHE_MARK)
#define CACHE_MARK              0x8000
#define EMPTY_MARK              0x4000
#define GET_COUNT(_page)        ((_page) & 0x3FFF)
#define MAX_COUNT               0x3FFF
	unsigned short *pages;

	unsigned int *pagecrc32;

	unsigned int max_pages;  /* total page number. */
	unsigned int max_caches;  /* total cache number. */

	unsigned int nr_cache;  /* current cache number. */

	unsigned int min_count;

#define HINFC_CACHE_FLAGS_STOP      0x01
#define HINFC_CACHE_FLAGS_CRC32     0x02
#define HINFC_CACHE_FLAGS_PEND      0x04
	unsigned int flags;
	unsigned int flags_pend;

	struct kmem_cache *rbcache;

	struct shrinker shrink;
	int shrink_percent;
	int min_shrink;

	unsigned int pagesize;
	unsigned int oobsize;

	struct dentry *dfs_file;

	struct proc_dir_entry *proc_file;
	char *proc_filename;

	spinlock_t lock;

	struct mutex mutex;

	struct {
		int index;  /* current lock index */
		int remove;
		int cause;
		int reset_count;
	} active;

	int nr_alloc;
	unsigned int nr_read;
	unsigned int nr_read_hit;
	unsigned int nr_read_miss;
	unsigned int nr_save;
	unsigned int nr_save_alloc_fail_miss;
	unsigned int nr_save_full_miss;
	unsigned int nr_remove[HINFC_CACHE_REMOVE_CAUSE_MAX];
	unsigned int sz_cache;
};

#define CF_CRC_ON(_cache, _flags)          ((_cache)->_flags & HINFC_CACHE_FLAGS_CRC32)
#define CF_CRC_ON_SET(_cache, _flags)      ((_cache)->_flags |= HINFC_CACHE_FLAGS_CRC32)
#define CF_CRC_ON_CLEAN(_cache, _flags)    ((_cache)->_flags &= ~HINFC_CACHE_FLAGS_CRC32)

#define CF_CACHE_OFF(_cache, _flags)       ((_cache)->_flags & HINFC_CACHE_FLAGS_STOP)
#define CF_CACHE_OFF_SET(_cache, _flags)   ((_cache)->_flags |= HINFC_CACHE_FLAGS_STOP)
#define CF_CACHE_OFF_CLEAN(_cache, _flags) ((_cache)->_flags &= ~HINFC_CACHE_FLAGS_STOP)

#define CF_CACHE_PEND(_cache, _flags)       ((_cache)->_flags & HINFC_CACHE_FLAGS_PEND)
#define CF_CACHE_PEND_SET(_cache, _flags)   ((_cache)->_flags |= HINFC_CACHE_FLAGS_PEND)
#define CF_CACHE_PEND_CLEAN(_cache, _flags) ((_cache)->_flags &= ~HINFC_CACHE_FLAGS_PEND)

#define TO_CACHE(_cache, _handle)  \
	struct hinfc_cache *_cache = (struct hinfc_cache *)_handle

struct hinfc_cache_node {
	struct rb_node rbnode;

	unsigned int count;  /* page read count */
	unsigned int index; /* page index */

	char *pagebuf; /* point to page buffer */
	char *oobbuf;  /* point to oob buffer */
};

#define RB_CACHE_ENTRY(_rbnode) \
	rb_entry(_rbnode, struct hinfc_cache_node, rbnode);

struct hinfc_weight {
	unsigned int index;
	unsigned int count;
};

#define ASSERT(_con, _fmt, _args...) if (!(_con)) { \
	printk(KERN_ERR "%s(%d): ASSERT(%s) " _fmt, __FILE__, __LINE__, #_con, ##_args); \
	dump_stack(); \
}

static int hinfc_dbgfs_init(struct hinfc_cache *cache, struct dentry *parent);
static void hinfc_dbgfs_destory(struct hinfc_cache *cache);

/******************************************************************************/

static int page_compare(struct hinfc_cache_node *node, unsigned int count,
			unsigned int index)
{
	if (node->count > count)
		return -1;
	else if (node->count < count)
		return 1;

	if (node->index > index)
		return -1;
	else if (node->index < index)
		return 1;

	return 0;
}
/******************************************************************************/

static struct hinfc_cache_node *
find_cache_node(struct hinfc_cache *cache, const unsigned int count,
		const unsigned int index)
{
	int comp;
	struct rb_node *node = cache->rbroot.rb_node;

	while (node) {
		struct hinfc_cache_node *leaf = RB_CACHE_ENTRY(node);
		comp = page_compare(leaf, count, index);

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

static void update_cache_min_count(struct hinfc_cache *cache)
{
	struct rb_node *rbnode;
	struct hinfc_cache_node *node;

	rbnode = rb_first(&cache->rbroot);
	if (rbnode) {
		node = RB_CACHE_ENTRY(rbnode);
		cache->min_count = node->count;
	} else
		cache->min_count = 0;

}
/******************************************************************************/

static struct hinfc_cache_node *
insert_cache_node(struct hinfc_cache *cache, struct hinfc_cache_node *newnode,
		  int update_min_count)
{
	int comp;
	struct rb_node **node = &cache->rbroot.rb_node;
	struct rb_node *parent = NULL;

	while (*node) {
		struct hinfc_cache_node *leaf = RB_CACHE_ENTRY(*node);
		comp = page_compare(leaf, newnode->count, newnode->index);

		parent = *node;

		if (comp < 0)
			node = &(*node)->rb_left;
		else if (comp > 0)
			node = &(*node)->rb_right;
		else
			return NULL;
	}

	rb_link_node(&newnode->rbnode, parent, node);
	rb_insert_color(&newnode->rbnode, &cache->rbroot);
	cache->nr_cache++;

	if (update_min_count)
		update_cache_min_count(cache);

	return newnode;
}
/******************************************************************************/

static void remove_cache_node(struct hinfc_cache *cache,
			      struct hinfc_cache_node *node, int free_cache,
			      int update_min_count, unsigned int cause)
{
	rb_erase(&node->rbnode, &cache->rbroot);
	cache->nr_cache--;

	if (cause < HINFC_CACHE_REMOVE_CAUSE_MAX)
		cache->nr_remove[cause]++;

	if (free_cache) {
		if (node->pagebuf) {
			kfree(node->pagebuf);
			cache->nr_alloc--;
			cache->sz_cache -= cache->pagesize;
		}

		if (node->oobbuf) {
			kfree(node->oobbuf);
			cache->nr_alloc--;
			cache->sz_cache -= cache->oobsize;
		}

		kmem_cache_free(cache->rbcache, node);
		cache->sz_cache -= sizeof(struct hinfc_cache_node);
		cache->nr_alloc--;
	}

	if (update_min_count)
		update_cache_min_count(cache);
}
/******************************************************************************/

static struct hinfc_cache_node *
new_cache_node(struct hinfc_cache *cache, unsigned int count,
	       unsigned int index, int cache_page, int cache_oob)
{
	struct hinfc_cache_node *node;

	node = kmem_cache_alloc(cache->rbcache,
		GFP_NOWAIT | __GFP_NOMEMALLOC | __GFP_NOWARN);
	if (!node)
		return NULL;
	cache->nr_alloc++;

	if (cache_page) {
		node->pagebuf = kmalloc(cache->pagesize,
			GFP_NOWAIT | __GFP_NOMEMALLOC | __GFP_NOWARN);
		if (!node->pagebuf)
			goto pagecache_fail;
		cache->nr_alloc++;
	} else
		node->pagebuf = NULL;

	if (cache_oob) {
		node->oobbuf = kmalloc(cache->oobsize,
			GFP_NOWAIT | __GFP_NOMEMALLOC | __GFP_NOWARN);
		if (!node->oobbuf)
			goto oobcache_fail;
		cache->nr_alloc++;
	} else
		node->oobbuf = NULL;

	node->count = count;
	node->index = index;

	if (node->pagebuf)
		cache->sz_cache += cache->pagesize;

	if (node->oobbuf)
		cache->sz_cache += cache->oobsize;

	cache->sz_cache += sizeof(struct hinfc_cache_node);

	return node;

oobcache_fail:
	if (node->pagebuf) {
		kfree(node->pagebuf);
		cache->nr_alloc--;
	}
pagecache_fail:
	kmem_cache_free(cache->rbcache, node);
	cache->nr_alloc--;

	return NULL;
}
/******************************************************************************/

static void free_all_cache_node(struct hinfc_cache *cache)
{
	struct rb_node *rbnode = rb_first(&cache->rbroot);

	while (rbnode) {
		struct hinfc_cache_node *node = RB_CACHE_ENTRY(rbnode);
		rbnode = rb_next(rbnode);

		remove_cache_node(cache, node, 1, 1,
			HINFC_CACHE_REMOVE_CAUSE_MAX);
	}
}
/******************************************************************************/

static void hinfc_reclaim_cache(struct hinfc_cache *cache, int nr_page,
				int cause)
{
	struct rb_node *rbnode;
	struct hinfc_cache_node *node;
	unsigned short *pages = cache->pages;

	rbnode = rb_first(&cache->rbroot);
	while (rbnode && nr_page > 0) {
		node = RB_CACHE_ENTRY(rbnode);
		rbnode = rb_next(rbnode);

		if (node->index == cache->active.index) {
			cache->active.remove = 1;
			cache->active.cause = cause;
			cache->active.reset_count = 0;
			continue;
		}

		pages[node->index] &= ~CACHE_MARK;
		nr_page--;

		remove_cache_node(cache, node, 1, 0, cause);
	}

	update_cache_min_count(cache);
}
/******************************************************************************/

static int hinfc_shrinker(struct shrinker *shrink, struct shrink_control *sc)
{
	unsigned int nr_caches;
	unsigned int shrink_percent;
	struct hinfc_cache *cache =
		container_of(shrink, struct hinfc_cache, shrink);

	spin_lock(&cache->lock);

	nr_caches = cache->nr_cache;
	shrink_percent = cache->shrink_percent;

	spin_unlock(&cache->lock);

	if (!nr_caches || !shrink_percent)
		return 0;

	nr_caches = nr_caches * shrink_percent / 100;

	if (nr_caches < cache->min_shrink)
		nr_caches = cache->min_shrink;

	spin_lock(&cache->lock);

	hinfc_reclaim_cache(cache, nr_caches, HINFC_CACHE_REMOVE_CAUSE_RECLAIM);

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

int hinfc_read_cache(void *handle, const unsigned int index, char **pbuf,
		     char **poob, int *status)
{
	TO_CACHE(cache, handle);

	int count;
	unsigned short *pages;
	struct hinfc_cache_node *node;

	*status = 0;

	if (index >= cache->max_pages)
		return -1;

	spin_lock(&cache->lock);

	cache->active.index = -1;

	pages = cache->pages;

	if (CF_CACHE_OFF(cache, flags)) {
		spin_unlock(&cache->lock);
		return -1;
	}
	cache->nr_read++;

	ASSERT(cache->pages != NULL, "nr_cache:%d\n", cache->nr_cache);

	count = GET_COUNT(pages[index]);
	if (count < MAX_COUNT)
		pages[index]++;

	if (pages[index] & EMPTY_MARK)
		*status |= HINFC_CACHE_STATUS_EMPTY_PAGE;

	if (!HAVE_CACHE(pages[index])) {
		cache->nr_read_miss++;

		spin_unlock(&cache->lock);
		return -1;
	}

	cache->nr_read_hit++;
	cache->active.index = index;

	node = find_cache_node(cache, count, index);
	if (!node) {
		ASSERT(0, "cache hit but no found data, index:%d, count:%d\n", index, count);

		spin_unlock(&cache->lock);
		return -1;
	}

	if (pbuf)
		*pbuf = node->pagebuf;

	if (poob)
		*poob = node->oobbuf;

	if (count == MAX_COUNT) {
		spin_unlock(&cache->lock);
		return 0;
	}

	remove_cache_node(cache, node, 0, 0, HINFC_CACHE_REMOVE_CAUSE_MOVE);

	node->count = GET_COUNT(pages[index]);

	insert_cache_node(cache, node, 1);

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

int hinfc_set_cache_status(void *handle, unsigned int index, int status)
{
	TO_CACHE(cache, handle);

	unsigned short *pages;

	if (index >= cache->max_pages)
		return -1;

	spin_lock(&cache->lock);

	pages = cache->pages;

	if (CF_CACHE_OFF(cache, flags)) {
		spin_unlock(&cache->lock);
		return -1;
	}

	ASSERT(cache->pages != NULL, "\n");

	if (status & HINFC_CACHE_STATUS_EMPTY_PAGE)
		pages[index] |= EMPTY_MARK;

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

int hinfc_save_cache(void *handle, const unsigned int index, char **pbuf,
		     char **poob)
{
	TO_CACHE(cache, handle);

	int count;
	unsigned short *pages;
	struct hinfc_cache_node *node;

	if (index >= cache->max_pages)
		return -1;

	spin_lock(&cache->lock);

	pages = cache->pages;

	if (CF_CACHE_OFF(cache, flags)) {
		ASSERT(cache->pages == NULL && cache->nr_cache == 0, "nr_cache:%d\n", cache->nr_cache);

		spin_unlock(&cache->lock);
		return -1;
	}

	ASSERT(cache->pages != NULL, "nr_cache:%d\n", cache->nr_cache);

	if (HAVE_CACHE(pages[index])) {
		ASSERT(0, "previous no cache, but find cache here\n");

		spin_unlock(&cache->lock);
		return -1;
	}

	count = GET_COUNT(pages[index]);

	if (cache->nr_cache == cache->max_caches &&
	    count <= cache->min_count) {
		cache->nr_save_full_miss++;

		spin_unlock(&cache->lock);
		return -1;
	}

	node = new_cache_node(cache, count, index, pbuf != NULL, poob != NULL);
	if (!node) {
		cache->nr_save_alloc_fail_miss++;

		spin_unlock(&cache->lock);
		return -1;
	}

	if (cache->nr_cache == cache->max_caches) {
		struct hinfc_cache_node *first_node;

		first_node = RB_CACHE_ENTRY(rb_first(&cache->rbroot));

		pages[first_node->index] &= ~CACHE_MARK;

		remove_cache_node(cache, first_node, 1, 0,
			HINFC_CACHE_REMOVE_CAUSE_FULL);
	}

	node = insert_cache_node(cache, node, 1);
	if (!node) {
		ASSERT(0, "no found cache, but insert fail.\n");

		spin_unlock(&cache->lock);
		return -1;
	}

	cache->nr_save++;

	if (pbuf)
		*pbuf = node->pagebuf;

	if (poob)
		*poob = node->oobbuf;

	cache->active.index = index;

	pages[index] |= CACHE_MARK;

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

static int hinfc_cache_crc(void *handle, unsigned int index, int checkit)
{
	TO_CACHE(cache, handle);

	unsigned int crc = 0;
	unsigned short *pages;
	struct hinfc_cache_node *node;

	if (index >= cache->max_pages)
		return -1;

	spin_lock(&cache->lock);

	pages = cache->pages;

	if (CF_CACHE_OFF(cache, flags) || !CF_CRC_ON(cache, flags)) {
		spin_unlock(&cache->lock);
		return -1;
	}

	ASSERT(cache->pages != NULL, "nr_cache:%d\n", cache->nr_cache);

	node = find_cache_node(cache, GET_COUNT(pages[index]), index);
	if (!node) {
		ASSERT(0, "cache hit but no found data, index:%d\n", index);

		spin_unlock(&cache->lock);
		return -1;
	}

	if (node->pagebuf)
		crc = crc32(0, node->pagebuf, cache->pagesize);
	if (node->oobbuf)
		crc = crc32(crc, node->oobbuf, cache->oobsize);

	if (checkit) {
		if (cache->pagecrc32[index] != crc)
			pr_err("crc32 check fail. page:%d, count:%d\n", node->index, node->count);
	} else
		cache->pagecrc32[index] = crc;

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

int hinfc_cache_save_crc(void *handle, unsigned int index)
{
	return hinfc_cache_crc(handle, index, 0);
}
/******************************************************************************/

int hinfc_cache_check_crc(void *handle, unsigned int index)
{
	return hinfc_cache_crc(handle, index, 1);
}
/******************************************************************************/

static int __hinfc_cache_enable(void *handle)
{
	TO_CACHE(cache, handle);

	unsigned int size;
	unsigned short *pages;

	spin_lock(&cache->lock);
	if (CF_CACHE_PEND(cache, flags)) {
		CF_CACHE_OFF_CLEAN(cache, flags_pend);

		spin_unlock(&cache->lock);
		return 0;
	}
	spin_unlock(&cache->lock);

	if (!CF_CACHE_OFF(cache, flags))
		return 0;

	size = cache->max_pages * sizeof(cache->pages[0]);
	pages = (unsigned short *)vmalloc(size);
	if (!pages) {
		pr_err("out of memory.\n");
		return -1;
	}
	memset(pages, 0, size);

	spin_lock(&cache->lock);

	ASSERT(cache->pages == NULL && cache->nr_cache == 0, "nr_cache:%d\n", cache->nr_cache);

	cache->nr_alloc++;
	cache->pages = pages;

	CF_CACHE_OFF_CLEAN(cache, flags);

	spin_unlock(&cache->lock);

	register_shrinker(&cache->shrink);

	return 0;
}
/******************************************************************************/

static int __hinfc_cache_disable(void *handle)
{
	TO_CACHE(cache, handle);

	unsigned short *pages;

	spin_lock(&cache->lock);

	if (CF_CACHE_PEND(cache, flags)) {
		CF_CACHE_OFF_SET(cache, flags_pend);

		spin_unlock(&cache->lock);
		return 0;
	}

	if (CF_CACHE_OFF(cache, flags)) {
		spin_unlock(&cache->lock);
		return 0;
	}

	if (cache->active.index != -1) {
		CF_CACHE_OFF_SET(cache, flags_pend);
		CF_CACHE_PEND_SET(cache, flags);

		spin_unlock(&cache->lock);
		return 0;
	}

	unregister_shrinker(&cache->shrink);

	hinfc_reclaim_cache(cache, cache->nr_cache,
		HINFC_CACHE_REMOVE_CAUSE_MANUAL);

	CF_CACHE_OFF_SET(cache, flags);

	ASSERT(cache->pages != NULL && cache->nr_cache == 0, "nr_cache:%d\n", cache->nr_cache);

	pages = cache->pages;
	cache->pages = NULL;
	cache->nr_alloc--;

	ASSERT((CF_CRC_ON(cache, flags) && cache->nr_alloc == 1) || (!CF_CRC_ON(cache, flags) && cache->nr_alloc == 0), "nr_alloc:%d\n", cache->nr_alloc);

	spin_unlock(&cache->lock);

	vfree(pages);

	return 0;
}
/******************************************************************************/

void hinfc_cache_enable(void *handle, int enable)
{
	TO_CACHE(cache, handle);

	mutex_lock(&cache->mutex);

	if (enable)
		__hinfc_cache_enable(handle);
	else
		__hinfc_cache_disable(handle);

	mutex_unlock(&cache->mutex);
}
/******************************************************************************/

static int hinfc_crc_enable(void *handle)
{
	TO_CACHE(cache, handle);

	unsigned int *pagecrc32;
	unsigned int size = cache->max_pages * sizeof(cache->pagecrc32[0]);

	pagecrc32 = (unsigned int *)vmalloc(size);
	if (!pagecrc32) {
		pr_err("out of memory.\n");
		return -1;
	}
	memset(pagecrc32, 0, size);

	ASSERT(cache->pagecrc32 == NULL, "\n");

	spin_lock(&cache->lock);

	cache->pagecrc32 = pagecrc32;
	cache->nr_alloc++;

	// TODO: XXX mayge it can't remove, some page crc check fail. because of index != -1
	hinfc_reclaim_cache(cache, cache->nr_cache,
		HINFC_CACHE_REMOVE_CAUSE_MANUAL);

	CF_CRC_ON_SET(cache, flags);

	spin_unlock(&cache->lock);

	return 0;
}
/******************************************************************************/

static int hinfc_crc_disable(void *handle)
{
	TO_CACHE(cache, handle);

	unsigned int *pagecrc32;

	ASSERT(cache->pagecrc32 != NULL, "\n");

	spin_lock(&cache->lock);

	CF_CRC_ON_CLEAN(cache, flags);

	cache->nr_alloc--;
	pagecrc32 = cache->pagecrc32;
	cache->pagecrc32 = NULL;

	spin_unlock(&cache->lock);

	vfree(pagecrc32);

	return 0;
}
/******************************************************************************/

int hinfc_cache_crc_enable(void *handle, int enable)
{
	TO_CACHE(cache, handle);
	int ret = 0;

	mutex_lock(&cache->mutex);

	if (!!CF_CRC_ON(cache, flags) + !!enable == 1) {
		if (enable)
			ret = hinfc_crc_enable(handle);
		else
			ret = hinfc_crc_disable(handle);
	}

	mutex_unlock(&cache->mutex);

	return ret;
}
/******************************************************************************/

void hinfc_remove_cache(void *handle, unsigned int index, int nr_page,
			int reset_count, int cause)
{
	TO_CACHE(cache, handle);

	int cache_change = 0;
	unsigned short *pages;
	struct hinfc_cache_node *node;

	if (index >= cache->max_pages)
		return;

	spin_lock(&cache->lock);

	pages = cache->pages;

	if (!cache->nr_cache) {
		spin_unlock(&cache->lock);
		return;
	}

	ASSERT(cache->pages != NULL, "nr_cache:%d\n", cache->nr_cache);

	if (index + nr_page > cache->max_pages)
		nr_page = cache->max_pages - index;

	for (; nr_page > 0; nr_page--, index++) {
		if (index == cache->active.index) {
			cache->active.remove = 1;
			cache->active.cause = cause;
			cache->active.reset_count = reset_count;

			continue;
		}

		if (HAVE_CACHE(pages[index])) {
			++cache_change;

			pages[index] &= ~CACHE_MARK;

			node = find_cache_node(cache, GET_COUNT(pages[index]),
				index);
			if (!node) {
				ASSERT(0, "cache hit but no found data, index:%d count:%d\n",
					index, GET_COUNT(pages[index]));

				spin_unlock(&cache->lock);
				return;
			}

			remove_cache_node(cache, node, 1, 0, cause);
		}

		if (reset_count)
			pages[index] = 0;
	}

	if (cache_change)
		update_cache_min_count(cache);

	spin_unlock(&cache->lock);
}
/******************************************************************************/

void hinfc_read_cache_finish(void *handle)
{
	TO_CACHE(cache, handle);

	spin_lock(&cache->lock);

	if (cache->active.index != -1) {
		int remove = 0;
		int cause = 0;
		int reset_count = 0;
		unsigned int index = cache->active.index;

		cache->active.index = -1;
		if (cache->active.remove) {
			remove = 1;
			cause = cache->active.cause;
			reset_count = cache->active.reset_count;

			cache->active.remove = 0;
		}

		spin_unlock(&cache->lock);

		if (remove)
			hinfc_remove_cache(handle, index, 1, reset_count,
				cause);

		spin_lock(&cache->lock);
	}

	if (CF_CACHE_PEND(cache, flags)) {
		int val;

		CF_CACHE_PEND_CLEAN(cache, flags);

		val = !!CF_CACHE_OFF(cache, flags) +
			!!CF_CACHE_OFF(cache, flags_pend);

		if (val == 1) {
			if (CF_CACHE_OFF(cache, flags_pend)) {
				spin_unlock(&cache->lock);

				__hinfc_cache_disable(handle);

				spin_lock(&cache->lock);
			} else {
				spin_unlock(&cache->lock);

				__hinfc_cache_enable(handle);

				spin_lock(&cache->lock);
			}
		}
	}

	spin_unlock(&cache->lock);
}
/******************************************************************************/

static void hinfc_set_weight(void *handle, struct hinfc_weight *weight,
			     int nr_weight)
{
	TO_CACHE(cache, handle);

	unsigned short *pages;
	struct hinfc_cache_node *node;

	for (; nr_weight > 0; nr_weight--, weight++) {
		if (weight->index >= cache->max_pages)
			continue;

		if (weight->count > MAX_COUNT)
			weight->count = MAX_COUNT;

		spin_lock(&cache->lock);

		pages = cache->pages;

		if (CF_CACHE_OFF(cache, flags)) {
			spin_unlock(&cache->lock);
			return;
		}

		if (HAVE_CACHE(pages[weight->index])) {
			node = find_cache_node(cache,
				GET_COUNT(pages[weight->index]),
				weight->index);
			if (!node) {
				ASSERT(0, "cache hit but no found data, index:%d count:%d\n",
					weight->index, GET_COUNT(pages[weight->index]));

				spin_unlock(&cache->lock);
				return;
			}

			remove_cache_node(cache, node, 0, 0,
				HINFC_CACHE_REMOVE_CAUSE_MOVE);
			node->count = weight->count;
			insert_cache_node(cache, node, 0);

			pages[weight->index] = weight->count | CACHE_MARK;
		} else
			pages[weight->index] = weight->count;

		spin_unlock(&cache->lock);
	}

	spin_lock(&cache->lock);
	update_cache_min_count(cache);
	spin_unlock(&cache->lock);
}
/******************************************************************************/

int hinfc_cache_init(void **handle, unsigned int max_pages,
		     unsigned int max_caches, unsigned int pagesize,
		     unsigned int oobsize, struct dentry *parent)
{
	unsigned int size;
	struct hinfc_cache *cache;

	*handle = NULL;

	ASSERT(max_pages != 0 && max_caches != 0, "\n");

	cache = (struct hinfc_cache *)vmalloc(sizeof(struct hinfc_cache));
	if (!cache) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}
	memset(cache, 0, sizeof(struct hinfc_cache));

	cache->max_pages = max_pages;

	size = cache->max_pages * sizeof(cache->pages[0]);

	cache->pages = (unsigned short *)vmalloc(size);
	if (!cache->pages) {
		pr_err("out of memory.\n");
		goto page_fail;
	}
	memset(cache->pages, 0, size);
	cache->nr_alloc++;

	cache->rbcache = kmem_cache_create("nand_rbcache",
		sizeof(struct hinfc_cache_node), 0, 0, NULL);
	if (!cache->rbcache) {
		pr_err("out of memory.\n");
		goto rbcache_fail;
	}

	if (max_caches > cache->max_pages)
		max_caches = cache->max_pages;

	cache->max_caches = max_caches;
	cache->pagesize = pagesize;
	cache->oobsize = oobsize;

	if (hinfc_dbgfs_init(cache, parent))
		goto dbgfs_fail;

	cache->active.index = -1;
	spin_lock_init(&cache->lock);

	mutex_init(&cache->mutex);

	cache->shrink.seeks = DEFAULT_SEEKS;
	cache->shrink.shrink = hinfc_shrinker;
	cache->shrink_percent = 100;
	cache->min_shrink = (SZ_1M << 4) / cache->pagesize;

	register_shrinker(&cache->shrink);

	*handle = cache;

	printk("Nand: Cache is enable, max size: %d KB.\n",
		(max_caches * (pagesize + oobsize)) >> 10);

	return 0;

dbgfs_fail:
	kmem_cache_destroy(cache->rbcache);
	cache->rbcache = NULL;
rbcache_fail:
	if (cache->pages) {
		vfree(cache->pages);
		cache->pages = NULL;
	}
page_fail:
	vfree(cache);

	return -ENOMEM;
}
/******************************************************************************/

int hinfc_cache_destory(void *handle)
{
	TO_CACHE(cache, handle);

	hinfc_dbgfs_destory(cache);

	unregister_shrinker(&cache->shrink);

	free_all_cache_node(cache);

	if (cache->rbcache)
		kmem_cache_destroy(cache->rbcache);

	if (cache->pagecrc32)
		vfree(cache->pagecrc32);

	if (cache->pages)
		vfree(cache->pages);

	vfree(cache);

	return 0;
}
/******************************************************************************/

static ssize_t dbgfs_data_read(struct file *filp, char __user *usrbuf,
			       size_t count, loff_t *ppos)
{
	int *ptr;
	char *tmp;
	const int max_tmp = 88;
	char *pusrbuf = usrbuf;
	char *buf = filp->private_data;

	int size;
	int nr_caches;

	nr_caches = *(int *)buf;
	if (!nr_caches)
		return 0;

	buf += sizeof(int);
	buf += sizeof(long);

	ptr = (int *)(buf + (*ppos) * sizeof(int) * 2);

	tmp = vmalloc(max_tmp);
	if (!tmp)
		return -ENOMEM;

	if (!*ppos && count >= max_tmp) {
		size = snprintf(tmp, max_tmp, "%8s,%-8s  %8s,%-8s  %8s,%-8s  %8s,%-8s\n",
			"index", "count", "index", "count", "index", "count", "index", "count");

		if (copy_to_user(pusrbuf, tmp, size))
			goto copy_fail;

		count -= size;
		pusrbuf += size;
	}

	while (count >= max_tmp && (*ppos) + 4 <= nr_caches) {
		size = snprintf(tmp, max_tmp, "%8d,%-8d  %8d,%-8d  %8d,%-8d  %8d,%-8d\n",
			ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);

		if (copy_to_user(pusrbuf, tmp, size))
			goto copy_fail;

		count -= size;
		pusrbuf += size;

		*ppos += 4;
		ptr += 8;
	}

	while (count >= max_tmp && (*ppos) < nr_caches) {
		char *suffix;

		if ((*ppos) + 1 < nr_caches)
			suffix = "  ";
		else
			suffix = "\n";

		size = snprintf(tmp, max_tmp, "%8d,%-8d%s", ptr[0], ptr[1], suffix);

		if (copy_to_user(pusrbuf, tmp, size))
			goto copy_fail;

		count -= size;
		pusrbuf += size;

		*ppos += 1;
		ptr += 2;
	}

	vfree(tmp);

	return (pusrbuf - usrbuf);

copy_fail:
	vfree(tmp);

	return -EFAULT;
}
/******************************************************************************/

static int dbgfs_data_release(struct inode *inode, struct file *file)
{
	char *buf = file->private_data;

	vfree(buf);

	return 0;
}
/******************************************************************************/

static int dbgfs_data_open(struct inode *inode, struct file *file)
{
	TO_CACHE(cache, inode->i_private);

	char *ptr, *buf;
	struct rb_node *rbnode;
	struct hinfc_cache_node *node;
	unsigned int nr_caches = cache->max_caches;

	buf = ptr = vmalloc(nr_caches * sizeof(int) * 2
		+ sizeof(int) + sizeof(long));
	if (!ptr)
		return -ENOMEM;

	spin_lock(&cache->lock);

	if (nr_caches > cache->nr_cache)
		nr_caches = cache->nr_cache;

	*(int *)ptr = nr_caches;
	ptr += sizeof(int);

	*(long *)ptr = (long)cache;
	ptr += sizeof(long);

	rbnode = rb_last(&cache->rbroot);
	while (rbnode) {
		node = RB_CACHE_ENTRY(rbnode);
		rbnode = rb_prev(rbnode);

		*(int *)ptr = node->index;
		ptr += sizeof(int);

		*(int *)ptr = node->count;
		ptr += sizeof(int);
	}

	spin_unlock(&cache->lock);

	file->private_data = buf;

	return 0;
}
/******************************************************************************/

static ssize_t dbgfs_data_write(struct file *filp, const char __user *usrbuf,
				size_t count, loff_t *ppos)
{
	char *ptr;
	char *buf;
	char *tptr;
	struct hinfc_cache *cache;

	cache = (struct hinfc_cache *)*(long *)
		((char *)filp->private_data + sizeof(int));

	ptr = buf = vmalloc(count + 1);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, usrbuf, count)) {
		vfree(buf);
		return -EFAULT;
	}
	buf[count] = '\0';

	ptr = skip_spaces(ptr);

	if (*ptr == '?') {
		printk("%s\n", "echo \"remove 0,64\" > data  // remove cache from page 0 to 0+64, not reset count");
		printk("%s\n", "echo \"clean 0,64\" > data  // remove cache from page 0 to 0+64, set count to zero");
		printk("%s\n", "echo \"weight 0,5 1,6 2,5\" > data  // set page 0 count 5, page 1 count 6, ...");
	} else if (!memcmp(ptr, "remove ", 7)) {
		/*
		 * cmd: "remove 0,64"
		 * remove cache, not clean count.
		 */
		int index, count;

		ptr = skip_spaces(ptr + 7);

		tptr = ptr;
		index = simple_strtoul(ptr, &ptr, 10);
		if (index >= cache->max_pages || tptr == ptr)
			goto out;

		tptr = ++ptr;
		count = simple_strtoul(ptr, &ptr, 10);
		if (index + count >= cache->max_pages || tptr == ptr)
			goto out;

		hinfc_remove_cache(cache, index, count, 0,
			HINFC_CACHE_REMOVE_CAUSE_MANUAL);

	} else if (!memcmp(ptr, "clean ", 6)) {
		/*
		 * cmd: "clean 0,64"
		 * remove cache, and clean count.
		 */
		int index, count;

		ptr = skip_spaces(ptr + 6);

		tptr = ptr;
		index = simple_strtoul(ptr, &ptr, 10);
		if (index >= cache->max_pages || tptr == ptr)
			goto out;

		tptr = ++ptr;
		count = simple_strtoul(ptr, &ptr, 10);
		if (index + count >= cache->max_pages || tptr == ptr)
			goto out;

		hinfc_remove_cache(cache, index, count, 1,
			HINFC_CACHE_REMOVE_CAUSE_MANUAL);

	} else if (!memcmp(ptr, "weight ", 7)) {
		/*
		 * cmd "weight 0,10 1,10 2,20"
		 * set multi page count
		 */
		int ix;
		const int max_weight = 10;
		struct hinfc_weight weight[max_weight];

		ptr = skip_spaces(ptr + 7);

next:
		for (ix = 0; *ptr && ix < max_weight; ix++) {
			tptr = ptr;
			weight[ix].index = simple_strtoul(ptr, &ptr, 10);
			if (tptr == ptr)
				goto out;

			tptr = ++ptr;
			weight[ix].count = simple_strtoul(ptr, &ptr, 10);
			if (tptr == ptr)
				goto out;

			if (*ptr == ';')
				ptr++;

			ptr = skip_spaces(ptr);
		}

		hinfc_set_weight(cache, weight, ix);

		if (*ptr)
			goto next;
	}

out:
	vfree(buf);

	return count;
}
/******************************************************************************/

static struct file_operations dbgfs_data_fops = {
	.open      = dbgfs_data_open,
	.read      = dbgfs_data_read,
	.write     = dbgfs_data_write,
	.release   = dbgfs_data_release,
};
/******************************************************************************/
#define CMD_MAX_CACHE            "max_cache"
#define CMD_CACHE_CRC            "crc"
#define CMD_CACHE_ENABLE         "cache"
#define CMD_SHRINK_PERCENT       "shrink_percent"
#define CMD_CACHE_CLEAN          "clean"

static ssize_t proc_info_read(struct file *filp, char __user *usrbuf,
			      size_t count, loff_t *ppos)
{
	TO_CACHE(cache, PDE_DATA(file_inode(filp)));

	char *ptr;
	char *buf;
	int max_buf = 1024;
	char *pusrbuf = usrbuf;
	int size;

	if (*ppos || count < 4096)
		return 0;
	*ppos = 1;

	buf = ptr = vmalloc(max_buf);
	if (!buf)
		return -ENOMEM;
#define CP_BUF(_fmt, _arg...) do { \
	size = snprintf(ptr, max_buf, "%-24s: "_fmt, ##_arg); \
	ptr += size; \
	max_buf -= size; \
} while (0)

	CP_BUF("%s\n", CMD_CACHE_ENABLE, CF_CACHE_OFF(cache, flags) ? "off" : "on");
	CP_BUF("%s\n", CMD_CACHE_CRC, CF_CRC_ON(cache, flags) ? "on" : "off");
	CP_BUF("%d\n", CMD_MAX_CACHE, cache->max_caches);
	CP_BUF("%d\n", "Total Pages", cache->max_pages);
	CP_BUF("%d\n", "Cache Number", cache->nr_cache);
	CP_BUF("%d kB\n", "Page Size", cache->pagesize >> 10);
	CP_BUF("%d kB\n", "Cache Size", cache->sz_cache >> 10);
	CP_BUF("%d\n", "Read", cache->nr_read);
	CP_BUF("%d\n", "Read Hit", cache->nr_read_hit);
	CP_BUF("%d\n", "Read Miss", cache->nr_read_miss);
	CP_BUF("%d\n", "Save", cache->nr_save);
	CP_BUF("%d\n", "Save Miss(full)", cache->nr_save_full_miss);
	CP_BUF("%d\n", "Save Miss(no memory)", cache->nr_save_alloc_fail_miss);
	CP_BUF("%d\n", "Remove(erase)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_ERASE]);
	CP_BUF("%d\n", "Remove(write)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_WRITE]);
	CP_BUF("%d\n", "Remove(empty page)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_EMPTY_PAGE]);
	CP_BUF("%d\n", "Remove(manual)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_MANUAL]);
	CP_BUF("%d\n", "Remove(reclaim)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_RECLAIM]);
	CP_BUF("%d\n", "Remove(move)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_MOVE]);
	CP_BUF("%d\n", "Remove(full)", cache->nr_remove[HINFC_CACHE_REMOVE_CAUSE_FULL]);
	CP_BUF("%d\n", CMD_SHRINK_PERCENT, cache->shrink_percent);
	CP_BUF("%d\n", "Shrink Minimum", cache->min_shrink);

	size = ptr - buf;
	if (copy_to_user(pusrbuf, buf, size))
		goto copy_fail;
	pusrbuf += size;

	vfree(buf);
	return (pusrbuf - usrbuf);

copy_fail:
	vfree(buf);
	return -EFAULT;

#undef CP_BUF
}
/******************************************************************************/

static void set_max_cache(struct hinfc_cache *cache, unsigned int max_caches)
{
	if (max_caches == 0 || max_caches > cache->max_pages)
		max_caches = cache->max_pages;

	if (max_caches == cache->max_caches)
		return;

	spin_lock(&cache->lock);

	if (max_caches < cache->nr_cache)
		hinfc_reclaim_cache(cache, cache->nr_cache - max_caches,
			HINFC_CACHE_REMOVE_CAUSE_MANUAL);

	cache->max_caches = max_caches;

	spin_unlock(&cache->lock);

}
/******************************************************************************/

static ssize_t proc_info_write(struct file *filp, const char __user *usrbuf,
			       size_t count, loff_t *ppos)
{
	TO_CACHE(cache, PDE_DATA(file_inode(filp)));

	char *ptr;
	char *buf;
	char *tptr;

	ptr = buf = vmalloc(count + 1);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, usrbuf, count)) {
		vfree(buf);
		return -EFAULT;
	}
	buf[count] = '\0';

	ptr = strim(ptr);

	if (*ptr == '?') {
		printk("echo \"%s 10\" > /proc/%s   // set shrink percent 10\n", CMD_SHRINK_PERCENT, cache->proc_filename);
		printk("echo \"%s on\" > /proc/%s   // enable nand cache crc\n", CMD_CACHE_CRC, cache->proc_filename);
		printk("echo \"%s off\" > /proc/%s   // disable nand cache crc\n", CMD_CACHE_CRC, cache->proc_filename);
		printk("echo \"%s on\" > /proc/%s   // enable nand cache\n", CMD_CACHE_ENABLE, cache->proc_filename);
		printk("echo \"%s off\" > /proc/%s   // disable nand cache\n", CMD_CACHE_ENABLE, cache->proc_filename);
		printk("echo \"%s 10\" > /proc/%s   // set max cache numbers to 10\n", CMD_MAX_CACHE, cache->proc_filename);
		printk("echo \"%s\" > /proc/%s   // clean all cache\n", CMD_CACHE_CLEAN, cache->proc_filename);

	} else if (!memcmp(ptr, CMD_SHRINK_PERCENT" ", sizeof(CMD_SHRINK_PERCENT))) {
		unsigned int shrink_percent;

		tptr = ptr = skip_spaces(ptr + sizeof(CMD_SHRINK_PERCENT));
		shrink_percent = simple_strtoul(ptr, &ptr, 10);
		if (tptr == ptr)
			goto out;

		if (shrink_percent > 100)
			shrink_percent = 100;

		spin_lock(&cache->lock);
		cache->shrink_percent = shrink_percent;
		spin_unlock(&cache->lock);

	} else if (!memcmp(ptr, CMD_CACHE_ENABLE" ", sizeof(CMD_CACHE_ENABLE))) {
		ptr = skip_spaces(ptr + sizeof(CMD_CACHE_ENABLE));

		if (!memcmp(ptr, "on", 3)) {
			hinfc_cache_enable(cache, 1);
		} else if (!memcmp(ptr, "off", 4)) {
			hinfc_cache_enable(cache, 0);
		}

	} else if (!memcmp(ptr, CMD_CACHE_CRC" ", sizeof(CMD_CACHE_CRC))) {
		ptr = skip_spaces(ptr + sizeof(CMD_CACHE_CRC));

		if (!memcmp(ptr, "on", 3)) {
			hinfc_cache_crc_enable(cache, 1);
		} else if (!memcmp(ptr, "off", 4)) {
			hinfc_cache_crc_enable(cache, 0);
		}

	} else if (!memcmp(ptr, CMD_MAX_CACHE" ", sizeof(CMD_MAX_CACHE))) {
		unsigned int max_caches;

		tptr = ptr = skip_spaces(ptr + sizeof(CMD_MAX_CACHE));
		max_caches = simple_strtoul(ptr, &ptr, 10);
		if (tptr == ptr)
			goto out;

		set_max_cache(cache, max_caches);

	} else if (!memcmp(ptr, CMD_CACHE_CLEAN, sizeof(CMD_CACHE_CLEAN))) {
		spin_lock(&cache->lock);

		hinfc_reclaim_cache(cache, cache->nr_cache,
			HINFC_CACHE_REMOVE_CAUSE_MANUAL);

		spin_unlock(&cache->lock);
	}
out:
	vfree(buf);

	return count;
}
/******************************************************************************/

static struct file_operations proc_info_fops = {
	.read  = proc_info_read,
	.write = proc_info_write,
};
/******************************************************************************/

static int hinfc_dbgfs_init(struct hinfc_cache *cache, struct dentry *parent)
{
	cache->proc_filename = "nandcache";

	cache->proc_file = proc_create_data(cache->proc_filename, 0644, NULL,
		&proc_info_fops, cache);
	if (!cache->proc_file)
		goto out_remove;

	cache->dfs_file = debugfs_create_file("cache_data", S_IFREG | S_IRUSR,
		parent, cache, &dbgfs_data_fops);
	if (!cache->dfs_file)
		goto out_remove;

	return 0;

out_remove:
	debugfs_remove(cache->dfs_file);

	if (cache->proc_file) {
		remove_proc_entry(cache->proc_filename, NULL);
		cache->proc_file = NULL;
	}

	pr_err("Create debugfs fail.\n");
	return -ENOENT;
}
/******************************************************************************/

static void hinfc_dbgfs_destory(struct hinfc_cache *cache)
{
	if (cache->proc_file)
		remove_proc_entry(cache->proc_filename, NULL);

	if (cache->dfs_file)
		debugfs_remove(cache->dfs_file);
}
