/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
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
 * Create By Cai Zhiyong 2015.6.10
 *
******************************************************************************/

#define DRVNAME "tag-chiptrim"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/kernel.h>
#include <linux/libfdt.h>
#include "setup.h"

/*****************************************************************************/
#define  TAG_CHIPTRIM_VAL        (0x726d6d78)

/*****************************************************************************/
static char chiptrim_value[16] = {0};
static int chiptrim_size = 0;
/*****************************************************************************/

char *get_chiptrim_value(char *buf, unsigned int buflen, unsigned int offset)
{
	if (offset >= chiptrim_size || !buf) {
		pr_err("%s: bad parameter.\n", __func__);
		return NULL;
	}

	if ((offset + buflen) > chiptrim_size)
		buflen = chiptrim_size - offset;

	return memcpy(buf, chiptrim_value + offset, buflen);
}

int get_chiptrim_size(void)
{
	return chiptrim_size;
}
/*****************************************************************************/

static int __init parse_chiptrim(const struct tag *tag, void *fdt)
{
	int ix;
	int ret;
	int node;
	char buf[12];

	printk("==========%s->%d:\n", __func__, __LINE__);
	if (tag->hdr.size <= (sizeof(struct tag_header) >> 2)) {
		pr_err("%s: bad tag format.\n", __func__);
		return 0;
	}

	chiptrim_size = (tag->hdr.size - (sizeof(struct tag_header) >> 2)) << 2;
	if (chiptrim_size > sizeof(chiptrim_value)) {
		pr_err("%s: chiptrim buffer too small.\n", __func__);
		chiptrim_size = sizeof(chiptrim_value);
	}

	memcpy(chiptrim_value, &tag->u, chiptrim_size);

	node = fdt_path_offset(fdt, "chiptrim");
	if (node < 0) {
		pr_err("%s: can't open fdt 'soc'\n", __func__);
		return 0;
	}

	for (ix = 0; ix < chiptrim_size >> 2; ix++) {
		snprintf(buf, sizeof(buf), "chiptrim%d", ix);

		ret = fdt_setprop_u32(fdt, node, buf, *(int *)&chiptrim_value[ix << 2]);
		if (ret) {
			pr_err("%s: can't fdt_setprop_u32 %s\n", __func__, buf);
		}
	}

	return 0;
}
/*****************************************************************************/

struct tagtable tag_chiptrim __initdata = {TAG_CHIPTRIM_VAL,  parse_chiptrim};
/*****************************************************************************/
