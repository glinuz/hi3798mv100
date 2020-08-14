/******************************************************************************
 *  Copyright (C) 2014 Hisilicon Technologies CO.,LTD.
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
 * Create By Cai Zhiyong 2014.1.26
 *
******************************************************************************/

#define pr_fmt(fmt) "pdm: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/memblock.h>
#include <linux/hikapi.h>
#include <linux/printk.h>
#include <linux/hisilicon/himisc.h>

static int __init pdm_read_item(char *str, char **name, char *param[],
				int nr_param)
{
	int ix;

	*name = str;
	while (*str && *str != '=')
		str++;
	if (*str != '=')
		return -1;
	*str++ = '\0';

	if (!*str)
		return 0;

	for (ix = 0; ix < nr_param; ix++) {
		param[ix] = str;
		while (*str && *str != ',')
			str++;
		if (!*str)
			return ix+1;
		*str++ = '\0';
	}

	return -2;
}
/******************************************************************************/
/*
 * pdm_tag format:
 * version=1.0.0.0 baseparam=0x86000000,0x1000,nosnapshot logdata=0x870000000,0x2000
 */
void __init pdm_reserve_mem(void)
{
	char pdm_tag[512];
	int  taglen;
	char *phead, *ptail;
	u32 phyaddr = 0;
	u32 addrlen = 0;
	int ret = -EINVAL;
	char *name;
	char *param[3];
	int snapshot;

	memset(pdm_tag, 0x0, sizeof(pdm_tag));
	taglen = get_param_data("pdm_tag", pdm_tag, sizeof(pdm_tag));
	if (taglen <= 0)
		return;

	if (taglen >= sizeof(pdm_tag))
		goto error;

	for (phead = pdm_tag; phead < pdm_tag + taglen; phead = ptail + 1) {
		ptail = phead;
		while (ptail < (pdm_tag + taglen) && *ptail != ' ' && *ptail != '\0')
			ptail++;
		if (!*ptail)
			ptail = pdm_tag + taglen;
		else
			*ptail = '\0';

		ret = pdm_read_item(phead, &name, param, 3);
		if (ret <= 0)
			goto error;

		if (ret >= 2) {
			phyaddr = simple_strtoul(param[0], NULL, 16);
			addrlen = simple_strtoul(param[1], NULL, 16);
		}

		snapshot = 1;
		if (ret >= 3) {
			if (!strncmp(param[2], "nosnapshot", 10))
				snapshot = 0;
		}

		pr_debug("name:%s, phyaddr:0x%08x, addrlen:0x%08x, snapshot:%d\n",
			 name, phyaddr, addrlen, snapshot);

		if (!strncmp(name, "version", 7))
			continue;

		if (ret < 2)
			goto error;

		if (phyaddr & (SZ_4K - 1))
			goto error;

		if (addrlen & (SZ_4K - 1))
			goto error;

		ret = fdt_add_memory_reserve((u64)phyaddr, (u64)addrlen);
		if (ret)
			goto error;
	}

	return;
error:
	pr_err("Invalid PDM tag, errno:%d\n", ret);
}

/*release reserve memory*/
int pdm_free_reserve_mem(u32 phyaddr, u32 size)
{
	u32  pfn_start;
	u32  pfn_end;
	u32  pages = 0;

	pfn_start = __phys_to_pfn(phyaddr);
	pfn_end   = __phys_to_pfn(phyaddr + size);

	for (; pfn_start < pfn_end; pfn_start++) {
		struct page *page = pfn_to_page(pfn_start);
		ClearPageReserved(page);
		init_page_count(page);
		__free_page(page);
		pages++;
	}

	totalram_pages += pages;

	return 0;
}
EXPORT_SYMBOL(pdm_free_reserve_mem);
