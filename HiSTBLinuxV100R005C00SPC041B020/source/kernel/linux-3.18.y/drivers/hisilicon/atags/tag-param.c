/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Cai Zhiyong
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

#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/string.h>

#include "setup.h"

struct tags_data_t {
	int version;
	unsigned int item_num;
	unsigned int buflen;
	char buf[4];
};

struct tags_item_t {
	char name[8];
	unsigned short offset;
	unsigned short length;
};

#define BOOT_PARAMS_SIZE   2048

static char tags_data_buf[BOOT_PARAMS_SIZE];
static struct tags_data_t * tags_data = NULL;

# if 0 /* this is a debug interface */
static void dump_param_data(char *tagbuf)
{
#define PRINTF printk

#define PR_HEX32(_ptr, _num) do { \
	int ix; \
	unsigned int *p = (unsigned int *)(_ptr); \
	for (ix = 0; ix < (_num); ix += 4, p++) { \
		if (!(ix % 16)) PRINTF("\n0x%08X: ", (unsigned int)p); \
		PRINTF("%08X ", *p); \
	} \
	PRINTF("\n\n"); \
} while (0)

	char *buf;
	struct tags_item_t *item;
	struct tags_data_t *data = (struct tags_data_t *)(tagbuf);

	PRINTF("version:    0x%08X\n", data->version);
	PRINTF("item count: %d\n", data->item_num);
	PRINTF("length:     0x%08X\n", data->buflen);

	if (data->version != 0x00100000) {
		PRINTF("tag format error. bad version.\n");
		return;
	}

	buf = data->buf + (data->item_num * sizeof(struct tags_item_t));
	if (buf > data->buf + data->buflen) {
		PRINTF("tag format error. too many items\n");
		return;
	}

	for (item = (struct tags_item_t *)data->buf;
		(char *)item < buf; item++) {

		PRINTF("name: %s, length: %d", item->name, item->length);

		if ((buf + item->offset + item->length)
			> (data->buf + data->buflen)) {
			PRINTF("tag format error. item length outof range\n");
			return;
		}

		PR_HEX32(buf + item->offset, item->length);
	}
}
#endif

static int __init parse_param(const struct tag *tag, void *fdt)
{
	int length;

	/* bad tag format. */
	if (tag->hdr.size <= 2) {
		pr_err("%s: bad tag format.\n", __func__);
		goto fail;
	}

	length = ((tag->hdr.size - 2) << 2);
	if (length > BOOT_PARAMS_SIZE) {
		pr_err("%s: not enough parameter buffer.\n", __func__);
		goto fail;
	}

	memcpy(tags_data_buf, &tag->u, length);

	tags_data = (struct tags_data_t *)tags_data_buf;

	if ((tags_data->buflen + offsetof(struct tags_data_t, buf)) > length) {
		pr_err("%s: bad tag format. buflen too larger\n", __func__);
		goto fail;
	}

	if (tags_data->item_num * sizeof(struct tags_item_t)
	    > tags_data->buflen) {
		pr_err("%s: bad tag format. too many item\n", __func__);
		goto fail;
	}

	return 0;
fail:
	tags_data = NULL;
	return 0;
}
struct tagtable tag_param __initdata = {0x70000001, parse_param};

/*
 * name          param name
 * data          param buffer pointer;
 *
 * return       -1:    not found this param;
 *              -2:    input parameter bad;
 *              other: parameter real length;
 */
int find_param_tag(const char *name, char **data)
{
	char *data_buf;
	unsigned int item_num;
	struct tags_item_t *item;

	if (!name || !*name ) {
		pr_err("%s: bad parameter.\n", __func__);
		return -2;
	}

	/* not found this tags */
	if (!tags_data)
		return -1;

	item_num = tags_data->item_num;
	item = (struct tags_item_t *)tags_data->buf;

	while (item_num--) {
		if (!strncmp(item->name, name, 8))
			goto find;
		item++;
	}
	return -1;

find:
	data_buf = tags_data->buf
		+ (tags_data->item_num * sizeof(struct tags_item_t));

	if ((data_buf + item->offset + item->length)
	     > (tags_data->buf + tags_data->buflen)) {
		pr_err("%s: tag format error.\n", __func__);
		return -1;
	}

	if (data)
		*data = (char *)(data_buf + item->offset);

	return (int)item->length;
}
