/******************************************************************************
 *  Copyright (C) 2016 Cai Zhiyong
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
 * Create By Yan Jiantao
 *           Cai Zhiying
 *
******************************************************************************/

#define pr_fmt(fmt) "ddr_watchpoint: " fmt

#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <asm/sections.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/mm.h>

#include "ddr_watchpoint.h"

#define MASTER_MID_CPU                  0
#define MASTER_MID_GPU                  1
#define MASTER_MID_MCU                  2
#define MASTER_MID_SHA_SEC              3
#define MASTER_MID_SHA_NONSEC           4
#define MASTER_MID_MULTCIPHYER          5
#define MASTER_MID_SDIO_EMMC_FMC        6
#define MASTER_MID_PCIE_SATA_USB3_0     7
#define MASTER_MID_USB2                 8
#define MASTER_MID_DDRT                 9
#define MASTER_MID_JPGD                 10
#define MASTER_MID_BPD                  11
#define MASTER_MID_JPGE                 12
#define MASTER_MID_VEDU                 13
#define MASTER_MID_PGD                  14
#define MASTER_MID_TDE                  15
#define MASTER_MID_VDP                  16
#define MASTER_MID_AIAO                 18
#define MASTER_MID_VDH0                 20
#define MASTER_MID_VDH1                 22
#define MASTER_MID_PASTC                23
#define MASTER_MID_VICAP                24
#define MASTER_MID_GZIP                 25
#define MASTER_MID_HWC                  26
#define MASTER_MID_DMAC                 27
#define MASTER_MID_VPSS                 28
#define MASTER_MID_PVR                  30
#define MASTER_MID_GSF_ETH              31

#define DEVNAME                         "ddr_watchpoint"

#define ENABLE                          1
#define DISABLE                         0

struct mid_info {
	int mid;
	const char *name;
};

#define MID_INFO(_MID, _NAME) \
	[MASTER_MID_##_MID] = {MASTER_MID_##_MID, _NAME,}

static struct mid_info mid_info[] = {
	MID_INFO( CPU,              "CPU"),
	MID_INFO( GPU,              "GPU"),
	MID_INFO( MCU,              "MCU"),
	MID_INFO( SHA_SEC,          "SEC-SHA"),
	MID_INFO( SHA_NONSEC,       "NONSEC-SHA"),
	MID_INFO( MULTCIPHYER,      "MULT-CIPHYER"),
	MID_INFO( SDIO_EMMC_FMC,    "SDIO/EMMC/FMC"),
	MID_INFO( PCIE_SATA_USB3_0, "PCIE/SATA/USB3.0"),
	MID_INFO( USB2,             "USB2.0"),
	MID_INFO( DDRT,             "DDRT"),
	MID_INFO( JPGD,             "JPGD"),
	MID_INFO( BPD,              "BPD"),
	MID_INFO( JPGE,             "JPGE"),
	MID_INFO( VEDU,             "VEDU"),
	MID_INFO( PGD,              "PGD"),
	MID_INFO( TDE,              "TDE"),
	MID_INFO( VDP,              "VDP"),
	MID_INFO( AIAO,             "AIAO"),
	MID_INFO( VDH0,             "VDH0"),
	MID_INFO( VDH1,             "VDH1"),
	MID_INFO( PASTC,            "PASTC"),
	MID_INFO( VICAP,            "VICAP"),
	MID_INFO( GZIP,             "GZIP"),
	MID_INFO( HWC,              "HWC"),
	MID_INFO( DMAC,             "DMAC"),
	MID_INFO( VPSS,             "VPSS"),
	MID_INFO( PVR,              "PVR"),
	MID_INFO( GSF_ETH,          "GSF/ETH"),
	{0, NULL},
};

/* used for debugfs read function. */
struct read_info {
	int index;
	char *ptr, *ptail;
	char *buf;
};

struct args_opt {
	char symbol;
	char *name;
	int size;
	u32 flags;
};

#define WP_ADDRNULL           (0x01) /* lock physics 0 address */
#define WP_KERNELTEXT         (0x02) /* lock kernel text section */
#define WP_PAGEPOISON         (0x04) /* lock page when call kfree and unlock call kmalloc */
#define WP_RANDOM             (0x08) /* random lock any page. */

static struct args_opt args_opt[] = {
	{0,   "all",         3, WP_ADDRNULL | WP_KERNELTEXT | WP_PAGEPOISON},
	{'N', "addrnull",    8, WP_ADDRNULL, },
	{'K', "kerneltext", 10, WP_KERNELTEXT, },
	{'P', "pagepoison", 10, WP_PAGEPOISON, },
	{'R', "random",     6,  WP_RANDOM, },
	{0, NULL, },
};

/* fixed size block item */
struct fs_item {
	union {
		struct rb_node rbnode;
		struct fs_item *next;
	};

	int index; /* offset in ram */

	u32 flags;
	u32 midmask;
	ulong virt_pageoff;
	int phy_pageoff;
};

#define MAX_PHY_PAGEOFF       ((0xFFFFFFFF >> PAGE_SHIFT) + 1)

#define FSITEM_ENTRY(_rbnode) \
	rb_entry(_rbnode, struct fs_item, rbnode)

/* fixed size watch point struct */
struct fs_wp {
	struct rb_root rbroot;

	/* ddr watch point item count. */
	u32 nr_item;

	/* free item list */
	struct fs_item *free_item;

	/*
	 * total fixed size watch point item,
	 * every item is map to a hardware configure in sram
	 */
#define MAX_FIXED_SIZE_ITEM            (512)
	struct fs_item item[MAX_FIXED_SIZE_ITEM];

#define MAX_MID_TABLE                  (10)
	struct mid_info *midoff[MAX_MID_TABLE];

	spinlock_t lock;
};

static int __wp_flags = 0;
static struct fs_wp *__fs_wp = NULL;
static atomic_t enable_pagepoison = {0};

struct hiwp_host {
	void __iomem *regbase;
	unsigned int irq;
	struct dentry *entry;
};
static struct hiwp_host *host = NULL;

static ulong prev_virtaddr = 0;
/******************************************************************************/

static void __maybe_unused hiwp_registers_dump(struct hiwp_host *host)
{
	int ix;

	printk("Register dump:");
	for (ix = 0; ix < 0x80; ix += 0x04) {
		if (!(ix & 0x0F))
			printk("\n0x%08X: ", (0xF8A36000 + ix));
		printk("%08X ", hiwp_read(host, ix));
	}
	printk("\n");
}
/******************************************************************************/
/*
 *    ddr_wp=addrnull,kerneltext,pagepoison;
 */
static int __init ddr_wp_bootargs_setup(char *s)
{
	__wp_flags |= WP_RANDOM;

	while (*s) {
		struct args_opt *args = args_opt;

		while (args->name) {
			if (!strncmp(args->name, s, args->size) &&
			    (s[args->size] == ',' || !s[args->size])) {
				__wp_flags |= args->flags;
				s += args->size;

				break;
			}
			args++;
		}

		if (!args->name) {
			/* unknown args, skip to next word. */
			while (*s && *s != ',')
				s++;
		}

		if (*s == ',')
			s++;
	}

	return 1;
}
__setup("ddr_wp=", ddr_wp_bootargs_setup);
/******************************************************************************/

static struct fs_item *new_fs_item(void)
{
	struct fs_item *item;

	if (__fs_wp->nr_item >= MAX_FIXED_SIZE_ITEM)
		return NULL;

	item = __fs_wp->free_item;
	__fs_wp->free_item = item->next;

	return item;
}
/******************************************************************************/

static struct fs_item *fs_item_find(int phy_pageoff)
{
	struct rb_node *node = __fs_wp->rbroot.rb_node;

	while (node) {
		struct fs_item *leaf = FSITEM_ENTRY(node);

		if (phy_pageoff < leaf->phy_pageoff)
			node = node->rb_left;
		else if (phy_pageoff > leaf->phy_pageoff)
			node = node->rb_right;
		else {
			return leaf;
		}
	}

	return NULL;
}
/******************************************************************************/
/* add item to RB tree. */
static int fs_item_insert(struct fs_item *item)
{
	struct rb_node *parent = NULL;
	struct rb_node **node = &__fs_wp->rbroot.rb_node;
	int phy_pageoff = item->phy_pageoff;

	while (*node) {
		struct fs_item *leaf = FSITEM_ENTRY(*node);

		parent = *node;

		if (phy_pageoff < leaf->phy_pageoff)
			node = &(*node)->rb_left;
		else if (phy_pageoff > leaf->phy_pageoff)
			node = &(*node)->rb_right;
		else {
			/* if phy_pageoff exsit, return -1. */
			return -1;
		}
	}

	rb_link_node(&item->rbnode, parent, node);
	rb_insert_color(&item->rbnode, &__fs_wp->rbroot);

	__fs_wp->nr_item++;

	return 0;
}
/******************************************************************************/
/* delete item from RB tree. */
static int fs_item_remove(struct fs_item *item)
{
	rb_erase(&item->rbnode, &__fs_wp->rbroot);

	item->flags = 0;
	item->next = __fs_wp->free_item;
	__fs_wp->free_item = item;

	__fs_wp->nr_item--;

	return 0;
}
/******************************************************************************/

static int fs_ctrl_update(struct fs_item *item)
{
	u32 regval;

	if (!item->flags && !item->midmask) {
		/* remove watchpoint for this mid & page. */
		regval = hiwp_read(host, AWADDR_TRAMPLE_ITEMS + item->index * 4);
		regval &= ~(AWADDR_TRAMPLE_ITEMS_MID(item->midmask));
		hiwp_write(host, regval, AWADDR_TRAMPLE_ITEMS + item->index * 4);
	} else {
		/* update. */
		regval = hiwp_read(host, AWADDR_TRAMPLE_ITEMS + item->index * 4);
		regval &= ~(AWADDR_TRAMPLE_ITEMS_MID(AWADDR_TRAMPLE_ITEMS_MIDMASK));
		regval &= ~(AWADDR_TRAMPLE_ITEMS_ADDR(0xfffff));
		regval |= (AWADDR_TRAMPLE_ITEMS_ENABLE
			  | AWADDR_TRAMPLE_ITEMS_MID(item->midmask)
			  | AWADDR_TRAMPLE_ITEMS_ADDR(item->phy_pageoff));
		hiwp_write(host, regval, AWADDR_TRAMPLE_ITEMS + item->index * 4);
	}

	return 0;
}
/******************************************************************************/

static int hiwp_clock_config(struct hiwp_host *host, bool enable)
{
	u32 regval;

	if (enable) {
		regval = hiwp_read(host, WADDR_TRAMPLE_CLKGCFG);
		regval |= (WADDR_TRAMPLE_CLKGCFG_ACLK_GT_EN 
			   | WADDR_TRAMPLE_CLKGCFG_CFGCLK_GT_EN);
		hiwp_write(host, regval, WADDR_TRAMPLE_CLKGCFG);
	} else {
		regval = hiwp_read(host, WADDR_TRAMPLE_CLKGCFG);
		regval &= ~(WADDR_TRAMPLE_CLKGCFG_ACLK_GT_EN 
			    | WADDR_TRAMPLE_CLKGCFG_CFGCLK_GT_EN);
		hiwp_write(host, regval, WADDR_TRAMPLE_CLKGCFG);
	}

	return 0;
}
/******************************************************************************/

static int hiwp_startup(struct hiwp_host *host, bool enable)
{
	unsigned int regval;

	if (enable) {
		regval = hiwp_read(host, AWADDR_TRAMPLE_EN);
		if (!(regval & AWADDR_TRAMPLE_EN_BIT)) {
			regval |= AWADDR_TRAMPLE_EN_BIT;
			hiwp_write(host, regval, AWADDR_TRAMPLE_EN);
		}
	} else {
		regval = hiwp_read(host, AWADDR_TRAMPLE_EN);
		if (regval & AWADDR_TRAMPLE_EN_BIT) {
			regval &= ~AWADDR_TRAMPLE_EN_BIT;
			hiwp_write(host, regval, AWADDR_TRAMPLE_EN);
		}
	}

	return 0;
}
/******************************************************************************/

static int hiwp_init_item(struct hiwp_host *host)
{
	int ix;

	/* invalidate all items. */
	for (ix = 0; ix < MAX_FIXED_SIZE_ITEM; ix++)
		hiwp_write(host, 0, AWADDR_TRAMPLE_ITEMS + ix * 4);

	for (ix = 0; ix < MAX_MID_TABLE; ix++)
		hiwp_write(host, __fs_wp->midoff[ix]->mid, AWADDR_TRAMPLE_MID_TBL + ix * 4);

	return 0;
}
/******************************************************************************/

static int fs_ctrl_init(struct hiwp_host *host)
{
	/* enable watchpoint clock. */
	hiwp_clock_config(host, ENABLE);

	/* init watchpoint  */
	hiwp_init_item(host);

	/* startup ddr watchpoint. */
	hiwp_startup(host, ENABLE);

	return 0;
}
/******************************************************************************/

static int fs_wp_insert(ulong phy_addr, ulong virt_addr, int nr_pages,
			u32 midmask, u32 flags)
{
	int ret;
	struct fs_item *item;
	unsigned long lock_flags;
	ulong phy_pageoff = phy_addr >> PAGE_SHIFT;
	ulong virt_pageoff = virt_addr >> PAGE_SHIFT;

	if (!flags)
		flags = WP_RANDOM;

	spin_lock_irqsave(&__fs_wp->lock, lock_flags);

	while (nr_pages > 0 && phy_pageoff < MAX_PHY_PAGEOFF) {
		item = fs_item_find(phy_pageoff);

		if (item) {
			pr_debug("%d: 0x%08x  0x%08x  0x%08x\n",
				item->index, item->phy_pageoff << PAGE_SHIFT, item->flags, item->midmask);

			item->flags |= flags;

			if ((item->midmask & midmask) == midmask)
				goto next;

			item->midmask |= midmask;
		} else {
			item = new_fs_item();
			if (!item)
				goto exit;

			item->flags = flags;
			item->midmask = midmask;
			item->phy_pageoff = phy_pageoff;
			item->virt_pageoff = virt_pageoff;

			ret = fs_item_insert(item);
			BUG_ON(ret != 0);
		}

		fs_ctrl_update(item);
next:
		phy_pageoff++;
		virt_pageoff++;
		nr_pages--;
	}

exit:
	spin_unlock_irqrestore(&__fs_wp->lock, lock_flags);

	return 0;
}
/******************************************************************************/

static int fs_wp_remove(ulong phy_addr, int nr_pages, u32 midmask, u32 flags)
{
	struct fs_item *item;
	unsigned long lock_flags;
	ulong phy_pageoff = phy_addr >> PAGE_SHIFT;

	if (!flags)
		flags = WP_RANDOM;

	spin_lock_irqsave(&__fs_wp->lock, lock_flags);

	while (nr_pages-- > 0 && phy_pageoff < MAX_PHY_PAGEOFF) {
		int update = 0;

		item = fs_item_find(phy_pageoff);
		if (item) {
			if (item->flags & flags) {
				item->flags &= ~flags;
				update = 1;
			}

			if (item->midmask & midmask) {
				item->midmask &= ~midmask;
				update = 1;
			}

			if (update)
				fs_ctrl_update(item);

			if (!item->flags && !item->midmask)
				fs_item_remove(item);

			pr_debug("%d: 0x%08x  0x%08x  0x%08x\n",
				item->index, item->phy_pageoff << PAGE_SHIFT, item->flags, item->midmask);
		}
		phy_pageoff++;
	}

	spin_unlock_irqrestore(&__fs_wp->lock, lock_flags);

	return 0;
}
/******************************************************************************/

static int get_fs_midmask(int mid)
{
	int ix;
	struct mid_info **midoff = __fs_wp->midoff;

	for (ix = 0; ix < MAX_MID_TABLE; ix++) {
		if (midoff[ix]->mid == mid)
			return 1 << ix;
	}

	return 0;
}
/******************************************************************************/

static int get_master_id(const char *name, int len)
{
	int ix;

	for (ix = 0; mid_info[ix].name; ix++) {
		if (!strncmp(mid_info[ix].name, name, len) &&
		    !mid_info[ix].name[len])
			return mid_info[ix].mid;
	}

	return -1;
}
/******************************************************************************/

static char *get_master_name(int mid)
{
	if (mid < 0 || mid >= 32)
		return NULL;

	return (char *)mid_info[mid].name;
}
/******************************************************************************/

static int lock_addrnull(void)
{
	u32 midmask = 0;

	midmask |= get_fs_midmask(MASTER_MID_CPU);

	pr_info("lock physical address 0x%08x-0x%08lx\n", 0, PAGE_SIZE);

	return fs_wp_insert(0UL, 0UL, 1, midmask, WP_ADDRNULL);
}
/******************************************************************************/

static int lock_kerneltext(void)
{
	u32 midmask = 0;
	ulong phyaddr;
	ulong viraddr;
	ulong nr_pages;

	midmask |= get_fs_midmask(MASTER_MID_CPU);
	viraddr = (unsigned long)_stext;
	phyaddr = __virt_to_phys(viraddr);
	nr_pages = (((unsigned long)_etext + PAGE_SIZE - 1) >> PAGE_SHIFT)
		- (viraddr >> PAGE_SHIFT) + 1;

	pr_info("lock kernel .text section, physical/virtual address 0x%08lx/%#lx, %lu pages.\n",
		phyaddr, viraddr, nr_pages);

	fs_wp_insert(phyaddr, viraddr, nr_pages, midmask, WP_KERNELTEXT);

	midmask |= get_fs_midmask(MASTER_MID_CPU);
	viraddr = (unsigned long)__start_rodata;
	phyaddr = __virt_to_phys(viraddr);
	nr_pages = (((unsigned long)__end_rodata + PAGE_SIZE - 1) >> PAGE_SHIFT)
		- (viraddr >> PAGE_SHIFT) + 1;

	pr_info("lock kernel .rodata section, physical/virtual address 0x%08lx/%#lx, %lu pages.\n",
		phyaddr, viraddr, nr_pages);

	fs_wp_insert(phyaddr, viraddr, nr_pages, midmask, WP_KERNELTEXT);

	return 0;
}
/******************************************************************************/

static int fs_item_init(void)
{
	int ix;
	struct fs_item **item;

	__fs_wp = kzalloc(sizeof(struct fs_wp), GFP_KERNEL);
	if (!__fs_wp) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	item = &__fs_wp->free_item;
	for (ix = 0; ix < MAX_FIXED_SIZE_ITEM; ix++) {
		*item = &__fs_wp->item[ix];
		(*item)->index = ix;

		item = &__fs_wp->item[ix].next;
	}
	*item = NULL;

	__fs_wp->midoff[0] = &mid_info[MASTER_MID_CPU];
	__fs_wp->midoff[1] = &mid_info[MASTER_MID_SDIO_EMMC_FMC];
	__fs_wp->midoff[2] = &mid_info[MASTER_MID_PCIE_SATA_USB3_0];
	__fs_wp->midoff[3] = &mid_info[MASTER_MID_USB2];
	__fs_wp->midoff[4] = &mid_info[MASTER_MID_JPGD];
	__fs_wp->midoff[5] = &mid_info[MASTER_MID_GSF_ETH];
	__fs_wp->midoff[6] = &mid_info[MASTER_MID_GPU];
	__fs_wp->midoff[7] = &mid_info[MASTER_MID_JPGD];
	__fs_wp->midoff[8] = &mid_info[MASTER_MID_TDE];
	__fs_wp->midoff[9] = &mid_info[MASTER_MID_VDP];

	spin_lock_init(&__fs_wp->lock);

	return 0;
}
/******************************************************************************/
/*
 *  echo "[+/-]virtstart,virtend,<mid_name>;<[+/-]virtstart,virtend,<mid_name>>"
 */
static ssize_t ddr_wp_fops_write(struct file *filp, const char __user *ubuf,
				 size_t sz_ubuf, loff_t *ppos)
{
	char *ptr, *ptail;
	char *kbuf;
	size_t sz_kbuf = sz_ubuf;

	ptr = kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}

	kbuf[sz_kbuf] = '\0';

	while (sz_kbuf > 0 && isspace(kbuf[sz_kbuf-1]))
		kbuf[--sz_kbuf] = '\0';
	ptail = &kbuf[sz_kbuf];

	while (ptr < ptail) {
		int enable;
		int midmask = 0;
		int nr_pages;
		ulong start_virtaddr, start_physaddr;
		ulong end_virtaddr, end_physaddr;

		if (*ptr != '+' && *ptr != '-') {
			ptr++;
			continue;
		}

		if (*ptr++ == '+')
			enable = 1;
		else
			enable = 0;

		start_virtaddr = simple_strtoul(ptr, &ptr, 16);
		if (*ptr++ != ',') {
			pr_warn("bad format: %s\n", ptr);
			continue;
		}

		if (start_virtaddr < PAGE_OFFSET ||
		    start_virtaddr > (unsigned long)high_memory) {
			pr_err("virt start addr is invalid, must be (0x%08lx~0x%08lx)\n", PAGE_OFFSET, (unsigned long)high_memory);
			return -EINVAL;
		}

		end_virtaddr = simple_strtoul(ptr, &ptr, 16);
		if (start_virtaddr >= end_virtaddr) {
			pr_warn("bad format: %s\n", ptr);
			continue;
		}

		if (end_virtaddr < PAGE_OFFSET ||
		    end_virtaddr > (unsigned long)high_memory) {
			pr_err("virt end addr is invalid, must be (0x%08lx~0x%08lx)\n", PAGE_OFFSET, (unsigned long)high_memory);
			return -EINVAL;
		}

		start_physaddr = virt_to_phys((void *)start_virtaddr);

		end_physaddr = (ulong)virt_to_phys((void *)end_virtaddr);

		nr_pages = (int)((end_physaddr + PAGE_SIZE - 1) >> PAGE_SHIFT)
			- (start_physaddr >> PAGE_SHIFT) + 1;

		if (*ptr++ == ',') {
			int mid;
			char *name = ptr;

			while (ptr < ptail && (*ptr != ';' || *ptr != '\0'))
				ptr++;

			mid = get_master_id(name, ptr - name);
			if (mid >= 0)
				midmask = get_fs_midmask(mid);
		}

		if (*ptr != '\0' && *ptr != ';') {
			pr_warn("bad format: %s\n", ptr);
			break;
		}

		if (enable) {
			fs_wp_insert(start_physaddr, start_virtaddr,
				nr_pages, midmask, WP_KERNELTEXT);
		} else {
			fs_wp_remove(start_physaddr, nr_pages, midmask,
				WP_KERNELTEXT);
		}
	}

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static ssize_t ddr_wp_fops_read(struct file *filp, char __user *ubuf,
				size_t sz_ubuf, loff_t *ppos)
{
	int ix;
	char *ptr;
	int ret;
	int leave = PAGE_SIZE;
	struct read_info *read_info = filp->private_data;
	loff_t __ppos;

	if (*ppos == 0) {
		if (!read_info->buf) {
			read_info->buf = vmalloc(PAGE_SIZE);
			if (!read_info->buf)
				return -ENOMEM;
		}

		ptr = read_info->buf;

		ret = snprintf(ptr, leave, "fixed size ddr watch point config\n  ");
		ptr += ret;
		leave -= ret;

		for (ix = 0; args_opt[ix].name; ix++) {
			if (!args_opt[ix].symbol)
				continue;

			ret = snprintf(ptr, leave, "%c - %s, ", args_opt[ix].symbol, args_opt[ix].name);
			ptr += ret;
			leave -= ret;
		}

		ret = snprintf(ptr, leave, "\nphy-addr    virt-addr   flags  access modules\n");
		ptr += ret;
		leave -= ret;

		read_info->index = 0;
		read_info->ptr = read_info->buf;
		read_info->ptail = ptr;
	}

	if (read_info->ptr >= read_info->ptail) {
		struct fs_item *item = vmalloc(sizeof(struct fs_item));
		if (!item) {
			pr_err("malloc failed.\n");
			return -ENOMEM;
		}

		ptr = read_info->buf;

		for (ix = read_info->index; ix < MAX_FIXED_SIZE_ITEM; ix++) {
			int jx;
			unsigned long flags;

			spin_lock_irqsave(&__fs_wp->lock, flags);

			memcpy(item, &__fs_wp->item[ix], sizeof(struct fs_item));

			spin_unlock_irqrestore(&__fs_wp->lock, flags);

			if (!item->flags && !item->midmask)
				continue;

			ret = snprintf(ptr, leave, "0x%08x  ", item->phy_pageoff << PAGE_SHIFT);
			ptr += ret;
			leave -= ret;

			if (item->virt_pageoff)
				ret = snprintf(ptr, leave, "0x%08lx  ", item->virt_pageoff << PAGE_SHIFT);
			else
				ret = snprintf(ptr, leave, "            ");
			ptr += ret;
			leave -= ret;

			for (jx = 0; args_opt[jx].name; jx++) {
				if (!args_opt[jx].symbol)
					continue;

				if (!(args_opt[jx].flags & item->flags))
					ret = snprintf(ptr, leave, " ");
				else
					ret = snprintf(ptr, leave, "%c", args_opt[jx].symbol);
				ptr += ret;
				leave -= ret;
			}

			ret = snprintf(ptr, leave, "   ");
			ptr += ret;
			leave -= ret;

			for (jx = 0; jx < MAX_MID_TABLE; jx++) {
				if (item->midmask & (1 << jx)) {
					ret = snprintf(ptr, leave, "%s/", __fs_wp->midoff[jx]->name);
					ptr += ret;
					leave -= ret;
				}
			}

			ret = snprintf(ptr, leave, "\n");
			ptr += ret;
			leave -= ret;

			if (leave < 80)
				break;
		}

		read_info->index = ix + 1;
		read_info->ptr = read_info->buf;
		read_info->ptail = ptr;

		vfree(item);

		if (read_info->ptr == read_info->ptail)
			return 0;
	}

	__ppos = 0;
	ret = simple_read_from_buffer(ubuf, sz_ubuf, &__ppos,
		read_info->ptr, read_info->ptail - read_info->ptr);
	if (ret > 0) {
		read_info->ptr += ret;
		*ppos += ret;
	}

	return ret;
}
/******************************************************************************/

static int ddr_wp_fops_open(struct inode *inode, struct file *filp)
{
	struct read_info *read_info;

	read_info = kmalloc(sizeof(struct read_info), GFP_KERNEL);
	if (!read_info)
		return -ENOMEM;

	read_info->buf = NULL;

	filp->private_data = read_info;

	return 0;
}
/******************************************************************************/

static int ddr_wp_fops_release(struct inode *inode, struct file *filp)
{
	struct read_info *read_info = filp->private_data;
	if (read_info->buf)
		vfree(read_info->buf);
	kfree(read_info);
	return 0;
}
/******************************************************************************/

static void dump_status_wp(struct hiwp_host *host)
{
	u32 regval;
	u32 ddrtrample_num;
	u32 ddrtrample_addr;
	u32 ddrtrample_mid;

	regval = readl(host->regbase + AWADDR_TRAMPLE_STATUS);
	regval &= AWADDR_TRAMPLE_STATUS_RINT_CNT;
	ddrtrample_num = regval >> AWADDR_TRAMPLE_STATUS_RINT_CNT_SHIFT;

	regval = readl(host->regbase + AWADDR_TRAMPLE_AWMID);
	regval &= AWADDR_TRAMPLE_AWMID_VALUE;
	ddrtrample_mid = regval;

	regval = readl(host->regbase + AWADDR_TRAMPLE_AWADDR);
	ddrtrample_addr = regval;

	printk("Dump watchpoint status:\n");
	printk("    Master MID:   0x%05x\n", ddrtrample_mid);
	printk("    Master Name:  %s\n", get_master_name(ddrtrample_mid));
	printk("    Trample Addr: 0x%x\n", ddrtrample_addr);
	printk("    Trample Num:  %d\n", ddrtrample_num);
}
/******************************************************************************/

static irqreturn_t hiwp_irq_handle(int irq, void *dev_id)
{
	unsigned int regval;
	struct hiwp_host *host = (struct hiwp_host *)dev_id;

	regval = readl(host->regbase + AWADDR_TRAMPLE_STATUS);
	if (regval & AWADDR_TRAMPLE_STATUS_RINT_RINT_ST) {
		/* dump status must befor clear int, or count will be reset. */
		dump_status_wp(host);

		regval |= AWADDR_TRAMPLE_STATUS_RINT_RINT_ST;
		writel(regval, host->regbase + AWADDR_TRAMPLE_STATUS);
	}

	return IRQ_HANDLED;
}
/******************************************************************************/

static struct file_operations ddr_wp_fops = {
	.open     = ddr_wp_fops_open,
	.read     = ddr_wp_fops_read,
	.write    = ddr_wp_fops_write,
	.llseek   = no_llseek,
	.release  = ddr_wp_fops_release,
};
/*****************************************************************************/

static int hiwp_driver_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct resource *res;

	host = devm_kzalloc(&pdev->dev, sizeof(struct hiwp_host), GFP_KERNEL);
	if (!host) {
		pr_err("failed to allocate host structure.\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, host);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	host->regbase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->regbase)) {
		pr_err("%s: regbase ioremap fail.\n", __func__);
		return -ENOMEM;
	}

	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0) {
		pr_err("no irq defined\n");
		return -EINVAL;
	}

	ret = request_irq(host->irq, hiwp_irq_handle, IRQF_SHARED, DEVNAME"-irq", host);
	if (ret) {
		pr_err("unable to request irq %d\n", host->irq);
		return ret;
	}

	host->entry = debugfs_create_file("ddr_wp", 0644, NULL, NULL,
		&ddr_wp_fops);
	if (!host->entry) {
		free_irq(host->irq, host);
		pr_err("create debugfs file failed.\n");
		return -ENOMEM;
	}

	ret = fs_item_init();
	if (ret) {
		free_irq(host->irq, host);
		debugfs_remove(host->entry);
		return ret;
	}

	fs_ctrl_init(host);

	if (__wp_flags & WP_ADDRNULL)
		lock_addrnull();

	if (__wp_flags & WP_KERNELTEXT)
		lock_kerneltext();

	if (__wp_flags & WP_PAGEPOISON) {
		pr_info("page poison enable.\n");
		atomic_set(&enable_pagepoison, 1);
	}

	return 0;
}
/******************************************************************************/

static int hiwp_driver_remove(struct platform_device *pdev)
{
	struct hiwp_host *host = platform_get_drvdata(pdev);

	debugfs_remove(host->entry);

	free_irq(host->irq, host);

	return 0;
}
/*****************************************************************************/

static const struct of_device_id
hisilicon_ddr_watchpoint_match[] __maybe_unused = {
	{ .compatible = "hisilicon.ddr_watchpoint", },
	{},
};
MODULE_DEVICE_TABLE(of, hisilicon_ddr_watchpoint_match);

static struct platform_driver hiwp_pltm_driver = {
	.probe  = hiwp_driver_probe,
	.remove = hiwp_driver_remove,
	.driver = {
		.name = DEVNAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisilicon_ddr_watchpoint_match),
	},
};
/******************************************************************************/
#ifdef CONFIG_PAGEALLOC_LOCK
void dbg_lock_page(struct page *page, void *virtaddr)
{
	ulong delta;

	if (atomic_read(&enable_pagepoison) <= 0)
		return;

	/* only lock lowmem */
	if ((ulong)virtaddr < PAGE_OFFSET ||
	    (ulong)virtaddr >= (ulong)high_memory)
		return;

	/* ignore continue memory */
	if (prev_virtaddr < (ulong)virtaddr)
		delta = (ulong)virtaddr - prev_virtaddr;
	else
		delta = prev_virtaddr - (ulong)virtaddr;

	if (delta >= SZ_8K)
		fs_wp_insert((ulong)virt_to_phys(virtaddr), (ulong)virtaddr, 1,
			get_fs_midmask(MASTER_MID_CPU), WP_PAGEPOISON);

	prev_virtaddr = (ulong)virtaddr;
}
/******************************************************************************/

void dbg_unlock_page(struct page *page, void *virtaddr)
{
	if (atomic_read(&enable_pagepoison) <= 0)
		return;

	/* only lock lowmem */
	if ((ulong)virtaddr < PAGE_OFFSET ||
	    (ulong)virtaddr >= (ulong)high_memory)
		return;

	fs_wp_remove((ulong)virt_to_phys(virtaddr), 1,
		get_fs_midmask(MASTER_MID_CPU), WP_PAGEPOISON);
}
#endif
/******************************************************************************/

static int __init ddr_wp_module_init(void)
{
	if (!__wp_flags)
		return 0;

	return platform_driver_register(&hiwp_pltm_driver);
}
/******************************************************************************/

static void __exit ddr_wp_module_exit(void)
{
	if (!__wp_flags)
		return;

	platform_driver_unregister(&hiwp_pltm_driver);
}
/******************************************************************************/

core_initcall(ddr_wp_module_init);
module_exit(ddr_wp_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yan Jiantao");
MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("kernel ddr watch point");
