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

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <debug.h>
#include <hush.h>

struct hitem_t {
	struct list_t next;
	unsigned int tag;
};
/*****************************************************************************/

static unsigned int hush_index(unsigned int tag, unsigned int depth)
{
	tag = (tag * 2654435769UL) >> 28;
	return (tag % depth);
}
/*****************************************************************************/

int hush_init(struct hush_t *hush, unsigned int depth)
{
	unsigned int size = depth * sizeof(struct list_t);

	ASSERT(hush != NULL && depth > 0);

	hush->nr_item = 0;
	hush->depth = depth;
	hush->list = (struct list_t *)zmalloc(size);
	if (!hush->list) {
		pr_error("malloc fail, hush init fail.\n");
		return -1;
	}

	return 0;
}
/*****************************************************************************/
/* this function only for debug */
int hush_uninit(struct hush_t *hush)
{
	unsigned int ix;

	printf("depth: %d\n", hush->depth);
	printf("count: %d\n", hush->nr_item);

	for (ix = 0; ix < hush->depth; ix++) {
		if (hush->list[ix].next)
			printf("BUG: list is not empty.\n");
	}

	free(hush->list);
	hush->list = (void *)0;

	return 0;
}
/*****************************************************************************/

char *hush_add(struct hush_t *hush, unsigned int tag, unsigned int sz_data)
{
	int pos;
	struct list_t *list;
	struct hitem_t *hitem;

	ASSERT(hush != NULL && sz_data > 0);

	hitem = (struct hitem_t *)malloc(sizeof(struct hitem_t) + sz_data);
	if (!hitem) {
		pr_error("malloc fail, add hush data fail.\n");
		return (char*)0;
	}

	pos = hush_index(tag, hush->depth);

	list = &hush->list[pos];
	list_add(&list->next, &hitem->next);
	hitem->tag = tag;

	hush->nr_item++;

	return (char *)&hitem[1];
}
/*****************************************************************************/

int hush_remove(struct hush_t *hush, unsigned int tag, void *args,
		void (*do_data)(void *args, unsigned int tag, char *data))
{
	int pos;
	struct hitem_t *hitem;
	struct list_t *prev, *list;

	ASSERT(hush != NULL);

	pos = hush_index(tag, hush->depth);

	for (prev = &hush->list[pos], list = prev->next; list;
	     prev = list, list = prev->next) {
		hitem = (struct hitem_t *)list;
		if (hitem->tag == tag) {
			hush->nr_item--;
			if (do_data)
				do_data(args, tag, (char *)&hitem[1]);
			list_remove(prev, list);
			free(list);
			return 0;
		}
	}

	if (do_data)
		do_data(args, tag, (char *)0);

	return -1;
}
/*****************************************************************************/

int hush_remove_data(struct hush_t *hush, char *data)
{
	struct hitem_t *hitem = (struct hitem_t *)data;

	ASSERT(hush != NULL && data != NULL);

	return hush_remove(hush, hitem[-1].tag, NULL, NULL);
}
/*****************************************************************************/

static struct hitem_t *find_next(struct hush_t *hush)
{
	struct hitem_t *hitem;
	unsigned int *pos = &hush->find.pos;
	struct list_t **prev = &hush->find.prev;

	while (*pos < hush->depth) {
		if (!*prev)
			*prev = hush->list[*pos].next;

		if (*prev)
			goto found;

		(*pos)++;
	}

	return (struct hitem_t *)0;
found:
	hitem = (struct hitem_t *)(*prev);

	(*prev) = (*prev)->next;
	if (!*prev)
		(*pos)++;

	return hitem;
}
/*****************************************************************************/

void hush_find_init(struct hush_t *hush)
{
	ASSERT(hush != NULL);

	hush->find.pos = 0;
	hush->find.prev = 0;
}
/*****************************************************************************/

char *hush_find_next(struct hush_t *hush, unsigned int *tag)
{
	struct hitem_t *hitem;

	ASSERT(hush != NULL);

	hitem = find_next(hush);
	if (hitem) {
		if (tag)
			*tag = hitem->tag;
		return (char *)&hitem[1];
	} else
		return (char *)0;
}
