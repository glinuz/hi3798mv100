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

#ifndef FLASH_CACHE_H
#define FLASH_CACHE_H

#define FLASHCACHE_DISABLE          (0x01)
/* page has not be read, need read flash device */
#define FLASHCACHE_PAGE_UNKNOWN     (0x00)
/* page has be read, but it is empty */
#define FLASHCACHE_PAGE_EMPTY       (0x1000000)
/* page is bad bock, need read flash device */
#define FLASHCACHE_PAGE_BADBLOCK    (0x2000000)
/* page has cache, no need to read from flash device */
#define FLASHCACHE_PAGE_CACHE       (0x3000000)

static inline char *flash_cache_status(int status)
{
	char *str[] = {"unknown", "empty", "badblock", "cache"};
	return str[(status >> 24) & 0x3];
}

struct flash_cache {
	int oobsize;
	u32 max_pages;
	int pages_per_block;

	volatile bool enable;

	int (*peek_status)(struct flash_cache *cache, u32 pageindex);

	int (*get_status_lock_read_page)(struct flash_cache *cache, u32 pageindex);

	int (*unlock_read_page)(struct flash_cache *cache, u32 pageindex);

	int (*new_cache)(struct flash_cache *cache, u32 pageindex,
		char **pagebuf, char **oobbuf);

	int (*get_cache)(struct flash_cache *cache, u32 pageindex,
		char **pagebuf, char **oobbuf);

	int (*discard_cache)(struct flash_cache *cache, u32 pageindex,
		int nr_pages, int status);
};

struct flash_cache *flash_cache_create(char *name, u64 totalsize, int blocksize,
				       int pagesize, int oobsize, int nr_cache,
				       int (*read_ahead)(void *, int, int *),
				       void *read_ahead_args);

void flash_cache_destory(struct flash_cache *cache);
/******************************************************************************/

#endif /* FLASH_CACHE_H*/
