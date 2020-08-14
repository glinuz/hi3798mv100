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
 * Create By Cai Zhiying 2014.11.12
 *
******************************************************************************/

#ifndef HINFC610_CACHE_H
#define HINFC610_CACHE_H


int hinfc_cache_init(void **handle, unsigned int max_pages,
		     unsigned int max_caches, unsigned int pagesize,
		     unsigned int oobsize, struct dentry *parent);

int hinfc_cache_destory(void *handle);

#define HINFC_CACHE_REMOVE_CAUSE_ERASE           0
#define HINFC_CACHE_REMOVE_CAUSE_WRITE           1
#define HINFC_CACHE_REMOVE_CAUSE_EMPTY_PAGE      2
#define HINFC_CACHE_REMOVE_CAUSE_MANUAL          3
#define HINFC_CACHE_REMOVE_CAUSE_RECLAIM         4
#define HINFC_CACHE_REMOVE_CAUSE_MOVE            5
#define HINFC_CACHE_REMOVE_CAUSE_FULL            6
#define HINFC_CACHE_REMOVE_CAUSE_MAX             7

void hinfc_remove_cache(void *handle, unsigned int index, int nr_page,
			int reset_count, int cause);

void hinfc_read_cache_finish(void *handle);

int hinfc_save_cache(void *handle, const unsigned int index, char **pbuf,
		     char **poob);

#define HINFC_CACHE_STATUS_EMPTY_PAGE            1
int hinfc_set_cache_status(void *handle, unsigned int index, int status);

int hinfc_read_cache(void *handle, const unsigned int index, char **pbuf,
		     char **poob, int *status);

int hinfc_cache_save_crc(void *handle, unsigned int index);

int hinfc_cache_check_crc(void *handle, unsigned int index);

void hinfc_cache_enable(void *handle, int enable);

int hinfc_cache_crc_enable(void *handle, int enable);

#endif /* HINFC610_CACHE_H*/
