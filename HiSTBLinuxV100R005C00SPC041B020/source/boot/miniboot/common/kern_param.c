/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai YingJun
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

#include <compile.h>
#include <malloc.h>
#include <string.h>
#include <debug.h>

#define CONFIG_PARAMS_SIZE             1024
#define CONFIG_PARAMS_COUNT            20

struct tags_item_t {
	char name[8];
	unsigned short offset;
	unsigned short length;
};

struct tags_ctrl_t {
	unsigned int nr_items;
	unsigned int sz_buf;
	char buf[CONFIG_PARAMS_SIZE];
	struct tags_item_t items[CONFIG_PARAMS_COUNT];
};

static struct tags_ctrl_t *tagsctrl;
/*****************************************************************************/

void kern_param_init(void)
{
	tagsctrl = zmalloc(sizeof(struct tags_ctrl_t));
	if (!tagsctrl) {
		pr_error("Can't malloc memory, %s fail.\n", __FUNCTION__);
		return;
	}
}
/*****************************************************************************/

void set_param_data(const char *name, const char *buf, int buflen)
{
	int ix;
	int len;
	struct tags_item_t *item;

	ASSERT(tagsctrl != NULL);
	ASSERT(name != NULL && *name != '\0' && buf != NULL && buflen > 0);
	ASSERT(tagsctrl->nr_items < CONFIG_PARAMS_COUNT);
	ASSERT(buflen + tagsctrl->sz_buf < CONFIG_PARAMS_SIZE);

	for (item = tagsctrl->items, ix = 0;
	     ix < tagsctrl->nr_items; ix++) {
		if (!strncmp(item->name, name, sizeof(item->name))) {
			pr_error("parameter %s has exist\n", item->name);
			return;
		}
	}

	item = &tagsctrl->items[tagsctrl->nr_items++];
	len = strnlen(name, sizeof(item->name));
	if (len == sizeof(item->name)) {
		pr_error("parameter %s has too longer.\n", name);
		return;
	}
	strncpy(item->name, name, len);
	item->name[len] = '\0';

	item->offset = tagsctrl->sz_buf;
	item->length = buflen;

	memcpy(tagsctrl->buf + tagsctrl->sz_buf, buf, buflen);
	tagsctrl->sz_buf += buflen;
}
/*****************************************************************************/

int get_param_tag_data(char *tagbuf)
{
	struct tags_data_t {
		int version;
		unsigned int nr_items;
		unsigned int sz_buf;
		char buf[4];
	} *data = (struct tags_data_t *)tagbuf;

	if (!tagsctrl->nr_items)
		return 0;

	data->version = 0x00100000;
	data->nr_items = tagsctrl->nr_items;
	data->sz_buf = sizeof(struct tags_item_t) * data->nr_items;
	memcpy(data->buf, tagsctrl->items, data->sz_buf);
	memcpy(data->buf + data->sz_buf, tagsctrl->buf, tagsctrl->sz_buf);
	data->sz_buf += tagsctrl->sz_buf;

	/* more than 4 bytes */
	return AROUND(sizeof(struct tags_data_t) + data->sz_buf, 4);
}
