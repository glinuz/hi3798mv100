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

#define DRVNAME "tag-net"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/kernel.h>
#include <linux/libfdt.h>
#include "setup.h"

#define DTS_PHY_MAX_CNT        3
#define TAG_ETHADDR_VAL        0x726d6d73
#define TAG_PHYADDR_VAL        (TAG_ETHADDR_VAL + 1)
#define TAG_PHYINTF_VAL        0x726d6d80
#define TAG_PHYGPIO_VAL        (TAG_PHYINTF_VAL + 1)

#define DTS_PHY_PATH_PREFIX    "net_phy"
#if defined(CONFIG_HIETH_GMAC) || defined(CONFIG_HIETH_GMAC_MODULE)
#define DTS_PHY_ADDR_PATH_PREFIX    "net_phy_addr"
#else
#define DTS_PHY_ADDR_PATH_PREFIX    DTS_PHY_PATH_PREFIX
#endif
/*****************************************************************************/

static int __init parse_macaddr(const struct tag *tag, void *fdt)
{
	int err;
	int node, index = 0;
	char path[16];
	unsigned char mac[6];

	memcpy(mac, &tag->u, 6);

	do {
		snprintf(path, sizeof(path), "%s%d", DTS_PHY_PATH_PREFIX, index);

		node = fdt_path_offset(fdt, path);
		if (node < 0)
			break;

		err = fdt_setprop(fdt, node, "mac-address", mac, sizeof(mac));
		if (err)
			pr_err("set prop mac %d fail.\n", index);
	} while (++index < DTS_PHY_MAX_CNT);

	return 0;
}
/*****************************************************************************/

static int __init parse_phyaddr(const struct tag *tag, void *fdt)
{
	int err;
	int node, index = 0;
	char path[16];
	int phy_addr;

	do {
		snprintf(path, sizeof(path), "%s%d", DTS_PHY_ADDR_PATH_PREFIX, index);

		node = fdt_path_offset(fdt, path);
		if (node < 0)
			break;

		phy_addr = *((int *)(&tag->u) + index);
		err = fdt_setprop_u32(fdt, node, "reg", phy_addr);
		if (err)
			pr_err("set prop phyaddr %d fail.\n", index);
	} while (++index < DTS_PHY_MAX_CNT);

	return 0;
}
/*****************************************************************************/

static int __init parse_phyintf(const struct tag *tag, void *fdt)
{
	int err;
	int node, index = 0;
	char path[16];
	char phy_intf[16];
	char *str, *nxt;
	int count;

	str = (char *)(&tag->u); /* mii,rmii */

	while (str) {
		snprintf(path, sizeof(path), "%s%d", DTS_PHY_PATH_PREFIX, index);

		nxt = strchr(str, ','); /* nxt = ",rmii" */
		if (nxt)
			count = min((int)(sizeof(path) - 1), (int)(nxt - str));
		else
			count = min(sizeof(path) - 1, strlen(str));

		strncpy(phy_intf, str, count);
		phy_intf[count] = '\0';

		node = fdt_path_offset(fdt, path);
		if (node >= 0) {
			err = fdt_setprop_string(fdt, node, "phy-mode", phy_intf);
			if (err)
				pr_err("set prop phy-mode %d fail.\n", index);
		}

		if (!nxt)
			break;
		str = nxt+1; /* str = "rmii" */
		index++;
	}

	return 0;
}
/*****************************************************************************/
static int __init parse_phygpio(const struct tag *tag, void *fdt)
{
	int err;
	int node, index = 0;
	char path[16];
	u32 gpio_base, gpio_bit;

	do {
		snprintf(path, 16, "%s%d", DTS_PHY_PATH_PREFIX, index);

		node = fdt_path_offset(fdt, path);
		if (node < 0)
			break;

		/* gpio base */
		gpio_base = *((u32 *)(&tag->u) + (index * 2));
		err = fdt_setprop_u32(fdt, node, "phy-gpio-base", gpio_base);
		if (err)
			pr_err("set prop phygpio base %d fail.\n", index);

		/* gpio bit */
		gpio_bit = *((u32 *)(&tag->u) + (index * 2) + 1);
		err = fdt_setprop_u32(fdt, node, "phy-gpio-bit", gpio_bit);
		if (err)
			pr_err("set prop phygpio bit %d fail.\n", index);
	} while (++index < DTS_PHY_MAX_CNT);

	return 0;
}
/*****************************************************************************/

struct tagtable tag_macaddr __initdata = {TAG_ETHADDR_VAL, parse_macaddr};
struct tagtable tag_phyaddr __initdata = {TAG_PHYADDR_VAL, parse_phyaddr};
struct tagtable tag_phyintf __initdata = {TAG_PHYINTF_VAL, parse_phyintf};
struct tagtable tag_phygpio __initdata = {TAG_PHYGPIO_VAL, parse_phygpio};
/*****************************************************************************/

