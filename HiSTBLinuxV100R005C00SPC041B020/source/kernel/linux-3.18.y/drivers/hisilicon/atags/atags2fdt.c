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

#define DRVNAME "atags"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/kernel.h>
#include <linux/libfdt.h>
#include <asm/memory.h>
#include <linux/hisilicon/himisc.h>

#include "setup.h"

#define MAX_FDT_SIZE           0x4000
static char newfdt[MAX_FDT_SIZE << 1]; /* total 32K */
static char  cmd_line[COMMAND_LINE_SIZE+3]; 
char no_usb3_0 = 0;
char no_usb3_1 = 0;

#ifdef CONFIG_BLK_DEV_RAM
extern int parse_initmrd_bootargs(char *bootargs);
#endif

u64 ddrsize;
/******************************************************************************/

static int __init node_offset(void *fdt, const char *node_path)
{
	int offset = fdt_path_offset(fdt, node_path);
	if (offset == -FDT_ERR_NOTFOUND)
		offset = fdt_add_subnode(fdt, 0, node_path);
	return offset;
}

static int __init setprop(void *fdt, const char *node_path, const char *property,
		   uint32_t *val_array, int size)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop(fdt, offset, property, val_array, size);
}

static int __init setprop_string(void *fdt, const char *node_path,
			  const char *property, const char *string)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop_string(fdt, offset, property, string);
}

static int __init setprop_cell(void *fdt, const char *node_path,
			const char *property, uint32_t val)
{
	int offset = node_offset(fdt, node_path);
	if (offset < 0)
		return offset;
	return fdt_setprop_cell(fdt, offset, property, val);
}

static const void * __init getprop(const void *fdt, const char *node_path,
			   const char *property, int *len)
{
	int offset = fdt_path_offset(fdt, node_path);

	if (offset == -FDT_ERR_NOTFOUND)
		return NULL;

	return fdt_getprop(fdt, offset, property, len);
}

static uint32_t __init get_cell_size(const void *fdt)
{
	int len;
	uint32_t cell_size = 1;
	const uint32_t *size_len =  getprop(fdt, "/", "#size-cells", &len);

	if (size_len)
		cell_size = fdt32_to_cpu(*size_len);
	return cell_size;
}

/******************************************************************************/

int parse_nodev_bootargs(void *fdt, char *p)
{
	int node, length;

	if (!fdt || !p)
		goto error;

	length = strlen(p);
	if ((!length) || (length > COMMAND_LINE_SIZE))
		goto error;

	memset(cmd_line, 0, COMMAND_LINE_SIZE+3);
	memcpy(cmd_line + 1, p, length);

	cmd_line[0] = ' ';
	cmd_line[length+1] = ' ';
	cmd_line[length+2] = '\0';

	if (strstr(cmd_line, " nopcie ")) {
		node = fdt_path_offset(fdt, "pcie");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nosata ")) {
		node = fdt_path_offset(fdt, "sata");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
		node = fdt_path_offset(fdt, "sataphy");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nofmc ")) {
		node = fdt_path_offset(fdt, "fmc");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " noemmc ")) {
		node = fdt_path_offset(fdt, "emmc");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nosd ")) {
		node = fdt_path_offset(fdt, "sd");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nosdio ")) {
		node = fdt_path_offset(fdt, "sdio");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nousb2_0 ")) {
		node = fdt_path_offset(fdt, "ohci0");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
		node = fdt_path_offset(fdt, "ehci0");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nousb2_1 ")) {
		node = fdt_path_offset(fdt, "xhci0");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nousb2_2 ")) {
		node = fdt_path_offset(fdt, "xhci1");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nousb3_0 ")) {
		no_usb3_0 = 1;
	}

	if (strstr(cmd_line, " nousb3_1 ")) {
		no_usb3_1 = 1;
	}

	if (strstr(cmd_line, " noudc ")) {
		node = fdt_path_offset(fdt, "udc");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
		node = fdt_path_offset(fdt, "otg");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nouart0 ")) {
		node = fdt_path_offset(fdt, "uart0");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nouart1 ")) {
		node = fdt_path_offset(fdt, "uart1");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " nouart2 ")) {
		node = fdt_path_offset(fdt, "uart2");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " noeth0 ")) {
		node = fdt_path_offset(fdt, "net_phy0");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	if (strstr(cmd_line, " noeth1 ")) {
		node = fdt_path_offset(fdt, "net_phy1");
		if (node >= 0) {
			fdt_setprop_string(fdt, node, "status", "disabled");
		}
	}

	return 0;
error:
	return -1;
}
/******************************************************************************/
/*
 * Convert and fold provided ATAGs into the provided FDT.
 *
 * REturn values:
 *    = 0 -> pretend success
 *    = 1 -> bad ATAG (may retry with another possible ATAG pointer)
 *    < 0 -> error from libfdt
 */
static int __init deal_atags(void *atag_list, void *fdt,
		      int (*parse)(const struct tag *, void *))
{
	struct tag *atag = atag_list;
	/* In the case of 64 bits memory size, need to reserve 2 cells for
	 * address and size for each bank */
	uint32_t mem_reg_property[2 * 2 * 16];
	int memcount = 0;
	int memsize;

	/* make sure we've got an aligned pointer */
	if ((ulong)atag_list & 0x3)
		return 1;

	/* if we get a DTB here we're done already */
	if (*(u32 *)atag_list == fdt32_to_cpu(FDT_MAGIC))
		return 0;

	/* validate the ATAG */
	if (atag->hdr.tag != ATAG_CORE ||
	    (atag->hdr.size != tag_size(tag_core) &&
	     atag->hdr.size != 2))
		return 1;

	ddrsize = 0;

	for_each_tag(atag, atag_list) {
		if (atag->hdr.tag == ATAG_CMDLINE) {
			setprop_string(fdt, "/chosen", "bootargs",
				atag->u.cmdline.cmdline);
			parse_nodev_bootargs(fdt, atag->u.cmdline.cmdline);

#ifdef CONFIG_BLK_DEV_RAM
			/* init initmrd. */
			parse_initmrd_bootargs(atag->u.cmdline.cmdline);
#endif
		} else if (atag->hdr.tag == ATAG_MEM) {
			if (memcount >= sizeof(mem_reg_property)/4)
				continue;
			if (!atag->u.mem.size)
				continue;
			memsize = get_cell_size(fdt);

			if (memsize == 2) {
				/* if memsize is 2, that means that
				 * each data needs 2 cells of 32 bits,
				 * so the data are 64 bits */
				uint64_t *mem_reg_prop64 =
					(uint64_t *)mem_reg_property;
				mem_reg_prop64[memcount++] =
					cpu_to_fdt64(atag->u.mem.start);
				mem_reg_prop64[memcount++] =
					cpu_to_fdt64(atag->u.mem.size);
				ddrsize += atag->u.mem.size;
			} else {
				mem_reg_property[memcount++] =
					cpu_to_fdt32(atag->u.mem.start);
				mem_reg_property[memcount++] =
					cpu_to_fdt32(atag->u.mem.size);
				ddrsize += atag->u.mem.size;
			}
		} else if (atag->hdr.tag == ATAG_INITRD2) {
			uint32_t initrd_start, initrd_size;
			initrd_start = atag->u.initrd.start;
			initrd_size = atag->u.initrd.size;
			setprop_cell(fdt, "/chosen", "linux,initrd-start",
					initrd_start);
			setprop_cell(fdt, "/chosen", "linux,initrd-end",
					initrd_start + initrd_size);
		} else
			parse(atag, fdt);
	}
#ifdef CONFIG_BLK_DEV_RAM
	/* initmrd reserve. */
	initmrd_reserve_memory();
#endif

	pdm_reserve_mem();
	pcie_reserve_memory();

	if (memcount) {
		setprop(fdt, "/memory", "reg", mem_reg_property,
			4 * memcount * memsize);
	}

	return fdt_pack(fdt);
}
/******************************************************************************/

int __init fdt_add_memory_reserve(u64 base, u64 size)
{
	int err;

	err = fdt_add_mem_rsv(newfdt, base, size);
	if (err) {
		pr_err("%s: fdt add reserve memory fail. %s\n",
			__func__, fdt_strerror(err));
		return err;
	}

	return 0;
}
/******************************************************************************/

void * __init fdt_init_hisi(void *fdt)
{
	int err;
	void *atag_list = __va(0x00000100);
	extern int parse_tags_hisi(const struct tag *tag, void *fdt);

	if (fdt_totalsize(fdt) >= MAX_FDT_SIZE) {
		pr_err("%s: fdt size too big\n", __func__);
		dump_stack();
	}

	err = fdt_open_into(fdt, newfdt, sizeof(newfdt));
	if (err) {
		pr_err("%s: fdt open error: %s\n", __func__, fdt_strerror(err));
		return fdt;
	}

	deal_atags(atag_list, newfdt, parse_tags_hisi);

	return newfdt;
}
