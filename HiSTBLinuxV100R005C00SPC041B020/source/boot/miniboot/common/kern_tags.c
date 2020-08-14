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

#include <run.h>
#include <string.h>
#include <env_set.h>
#include <boot.h>
#include <stdio.h>
#include <kern_param.h>
#include <malloc.h>
#include <params.h>
#include <debug.h>

struct tag_hdr_t {
	uint32 size;  /* tag size (data + head) 4bytes unit */
	uint32 type;  /* tag type */
};

#ifndef CONFIG_SDKVERSION
#  define CONFIG_SDKVERSION     "sdk version unknown"
#endif /* CONFIG_SDKVERSION */

#ifdef CONFIG_NET
int eth_nums = 0;
#define MAX_ETH_NUMS 4
extern char *get_eth_phyaddr_str(void);
extern char *get_eth_phyintf_str(void);
extern char *get_eth_phygpio_str(void);
extern int get_eth_phyaddr(int index, int defval);
extern int get_eth_phygpio(int index, u32 *gpio_base, u32 *gpio_bit);
#endif

#ifdef CONFIG_GENERIC_MMC
u32 emmc_offline = 1;
u32 emmc_samplephase = 3;
u32 emmc_caps2 = 0;
#endif
/*****************************************************************************/

static struct tag_hdr_t *fill_tag(struct tag_hdr_t *tag, uint32 type, char *buf,
				  uint32 buflen)
{
	tag->type = type;
	tag->size = (sizeof(struct tag_hdr_t) + buflen + 3) >> 2;
	memcpy((char *)&tag[1], buf, buflen);
	return (struct tag_hdr_t *)((uint32 *)tag + tag->size);
}
/*****************************************************************************/

static struct tag_hdr_t *get_tag_buf(struct tag_hdr_t *tag, uint32 type,
				     char **buf, uint32 buflen)
{
	tag->type = type;
	tag->size = (sizeof(struct tag_hdr_t) + buflen + 3) >> 2;
	*buf = (char*)&tag[1];
	return (struct tag_hdr_t *)((uint32 *)tag + tag->size);
}
/*****************************************************************************/

static struct tag_hdr_t *setup_param_tag(struct tag_hdr_t *tag)
{
	char *buf = (char*)&tag[1];

	tag->type = 0x70000001;
	tag->size = (sizeof(struct tag_hdr_t) + get_param_tag_data(buf) + 3) >> 2;

	return (struct tag_hdr_t *)((uint32 *)tag + tag->size);
}

/*****************************************************************************/
#ifdef CONFIG_NET
static void strtomac(unsigned char *mac, char* s)
{
	int i;
	char *e;

	for (i = 0; i < 6; ++i) {
		mac[i] = s ? strtoul(s, &e, 16) : 0;
		if (s)
			s = (*e) ? e+1 : e;
	}
}

struct tag_hdr_t *setup_eth_tags(struct tag_hdr_t *tag)
{
	int i;
	char *env;
	char mac[6] = {0};
	uint32 gpio_base, gpio_bit, data[4];
	int phyaddr[MAX_ETH_NUMS];

	if (MAX_ETH_NUMS < eth_nums) {
		pr_error("MAX_ETH_NUMS(%d) less than actual ETH Num %d", MAX_ETH_NUMS, eth_nums);
		dump_stack();
	}

	/* Set up phy address tag */
	for (i = 0; i < eth_nums; i++) {
		phyaddr[i] = get_eth_phyaddr(i, 2);
	}
	tag = fill_tag(tag, 0x726d6d74, (char *)phyaddr, sizeof(phyaddr));

	/* Set up phy interface tag */
	env = get_eth_phyintf_str();
	tag = fill_tag(tag, 0x726d6d80, (char *)env, strlen(env)+1+4);

	/* Set up eth address tag */
	env = env_get("ethaddr");
	strtomac((unsigned char *)&mac[0], env);
	tag = fill_tag(tag, 0x726d6d73, mac, sizeof(mac));
	mac[6] = '\0';

	/* Set up eth GPIO tag */
	for (i = 0; i < eth_nums; i++) {
		get_eth_phygpio(i, &gpio_base, &gpio_bit);
		if (!gpio_base) {
			data[i*2] = 0;
			data[i*2 + 1] = 0;
			continue;
		}

		data[i*2] = gpio_base;
		data[i*2 + 1] = gpio_bit;
	}
	tag = fill_tag(tag, 0x726d6d81, (char *)data, sizeof(data));

	return tag;
}
#endif /* CONFIG_NET */
/*****************************************************************************/

uint32 get_kern_tags(uint32 kernel)
{
	char *buf;
	int buflen;
#ifdef CONFIG_GENERIC_MMC
	u64 emmcbuf;
#endif
	uint64 *num;
	uint32 start = CONFIG_TAG_OFFSET; // TODO: XXX
	struct tag_hdr_t *tag = (struct tag_hdr_t *)start;

	printf("Save kernel parameter to 0x%08x\n", start);
	/* tag start symbol */
	buflen = sizeof(int) * 3;
	tag = get_tag_buf(tag, 0x54410001, &buf, buflen);
	memset(buf, 0, buflen);

	tag = get_tag_buf(tag, 0x54410002, (char **)&num, sizeof(int) * 4);
	*num++ = (uint64)get_ddr_size();
	*num++ = 0;

#ifdef CONFIG_BOOTARGS_MERGE
	buf = bootargs_merge();
#else
	buf = env_get("bootargs");
#endif
	if (buf) {
		tag = fill_tag(tag, 0x54410009, buf, strlen(buf) + 1);
#ifdef CONFIG_BOOTARGS_MERGE
		free(buf);
#endif
	}
	tag = fill_tag(tag, 0x726d6d75, CONFIG_SDKVERSION, sizeof(CONFIG_SDKVERSION));

#ifdef CONFIG_GENERIC_MMC
	emmcbuf = emmc_offline | (((u64)emmc_samplephase)<<32) | (((u64)emmc_caps2) << 40);
	tag = fill_tag(tag, 0x48694E7b, (char *)&emmcbuf, 8);
#endif

#ifdef CONFIG_BOOTREG
	{
		int tmp = _blank_zone_end - _blank_zone_start;
		tag = get_tag_buf(tag, 0x53000001, &buf, sizeof(int) + tmp);
		*(int*)buf = tmp;
		memcpy(buf + sizeof(int), (char *)_blank_zone_start, tmp);
	}
#endif /* CONFIG_BOOTREG */

#ifdef CONFIG_NET
	tag = setup_eth_tags(tag);
#endif /* CONFIG_NET */

	tag = setup_param_tag(tag);

	/* tag end symbol */
	tag->size = 0;
	tag->type = 0;
	if (((uint32)tag - CONFIG_TAG_OFFSET) >= CONFIG_TAG_MAX_SIZE) {
		printf("ERROR: NOT enough TAG area!\n");
		while(1);
	}

	printf("ATAGS [0x%08X - 0x%08X], %u Bytes\n", CONFIG_TAG_OFFSET, (uint32)tag, ((uint32)tag - CONFIG_TAG_OFFSET));

	return start;
}

