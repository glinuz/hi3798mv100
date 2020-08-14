/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#ifndef HUSHH
#define HUSHH
/******************************************************************************/

#include <list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hush_t {
	struct list_t *list;
	unsigned int depth;
	unsigned int nr_item;
	struct {
		unsigned int pos;
		struct list_t *prev;
	} find;
};

int hush_init(struct hush_t *hush, unsigned int depth);

int hush_uninit(struct hush_t *hush);

char *hush_add(struct hush_t *hush, unsigned int tag, unsigned int sz_data);

int hush_remove(struct hush_t *hush, unsigned int tag, void *args,
		void (*do_data)(void *args, unsigned int tag, char *data));

int hush_remove_data(struct hush_t *hush, char *data);

void hush_find_init(struct hush_t *hush);

char *hush_find_next(struct hush_t *hush, unsigned int *tag);

#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif /* HUSHH */
