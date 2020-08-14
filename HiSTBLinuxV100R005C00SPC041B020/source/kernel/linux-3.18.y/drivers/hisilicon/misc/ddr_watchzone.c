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

#define pr_fmt(fmt) "hiddr_watchzone: " fmt

#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/list.h>
#include <asm/sections.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/of.h>
#include <linux/mm.h>

#include "ddr_watchzone.h"

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

#define DEVNAME				"ddr_watchzone"

#define ENABLE				1
#define DISABLE				0

struct hiwz_host {
	void __iomem *regbase;
	unsigned int irq;
};

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

static struct hiwz_host *host;

/* watch zone item */
struct wz_item {
	int index; /* offset in ram */

	u32 flags;
	u32 midmask;
	ulong virt_pagestart;
	ulong virt_pageend;
	ulong phy_pagestart;
	ulong phy_pageend;

	/* items list. */
	struct wz_item *next;

	struct list_head list;
};

#define MAX_PHY_PAGEOFF       ((0xFFFFFFFF >> PAGE_SHIFT) + 1)

/* fixed size watch zone struct */
struct ddr_wz {

	/* ddr watch zone item count. */
	u32 nr_item;

	/* pointer of first free item. */
	struct wz_item *free_item;

	/*
	 * total fixed size watch zone item,
	 * every item is map to a hardware configure in sram
	 */
#define MAX_WATCH_ZONE_ITEM            (32)
	struct wz_item item[MAX_WATCH_ZONE_ITEM];

	struct list_head head;

	struct mutex mutex;
};

static int __wz_flags = 0;
static struct dentry *__wz_entry = NULL;
static struct ddr_wz *__ddr_wz = NULL;

/******************************************************************************/

static void __maybe_unused hiwz_registers_dump(struct hiwz_host *host)
{
	int ix;

	printk("Register dump:");
	for (ix = 0; ix < 0x200; ix += 0x04) {
		if (!(ix & 0x0F))
			printk("\n0x%08X: ", (0xF8A35000 + ix));
		printk("%08X ", hiwz_read(host, ix));
	}
	printk("\n");
}
/******************************************************************************/
/*
 *    ddr_wz=addrnull,kerneltext,pagepoison;
 */
static int __init ddr_wz_bootargs_setup(char *s)
{
	__wz_flags |= WP_RANDOM;

	while (*s) {
		struct args_opt *args = args_opt;

		while (args->name) {
			if (!strncmp(args->name, s, args->size) &&
			    (s[args->size] == ',' || !s[args->size])) {
				__wz_flags |= args->flags;
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
__setup("ddr_wz=", ddr_wz_bootargs_setup);
/******************************************************************************/

static struct wz_item *new_wz_item(void)
{
	struct wz_item *item;

	if (__ddr_wz->nr_item >= MAX_WATCH_ZONE_ITEM) {
		return NULL;
	}

	item = __ddr_wz->free_item;
	__ddr_wz->free_item = item->next;

	return item;
}
/******************************************************************************/

static struct wz_item *wz_item_find(ulong phy_pagestart, ulong phy_pageend)
{
	struct list_head *entry;
	struct wz_item *item;

	list_for_each(entry, &(__ddr_wz->head)) {
		item = list_entry(entry, struct wz_item, list);
		if (item) {
			if (phy_pagestart >= item->phy_pagestart && phy_pageend <= item->phy_pageend) {
				return item;
			}
		}
	}

	return NULL;
}
/******************************************************************************/
/* add item to list head. */
static int wz_item_insert(struct wz_item *item)
{
	if (__ddr_wz->nr_item >= MAX_WATCH_ZONE_ITEM)
		return -1;

	list_add(&(item->list), &(__ddr_wz->head));

	__ddr_wz->nr_item++;

	return 0;
}
/******************************************************************************/
/* delete item from list head. */
static int wz_item_remove(struct wz_item *item)
{
	list_del(&(item->list));

	item->flags = 0;
	item->next = __ddr_wz->free_item;
	__ddr_wz->free_item = item;

	__ddr_wz->nr_item--;

	return 0;
}
/******************************************************************************/

static int fs_ctrl_update(struct wz_item *item)
{
	if (!item->flags && !item->midmask) {
		/* invalidate watchzone. */
		hiwz_write(host, 0, AWADDR_SRVLNC_START + item->index * 8);
		hiwz_write(host, 0, AWADDR_SRVLNC_END + item->index * 8);
	} else {
		/* update. */
		hiwz_write(host, item->phy_pagestart << PAGE_SHIFT, AWADDR_SRVLNC_START + item->index * 8);
		hiwz_write(host, item->phy_pageend << PAGE_SHIFT, AWADDR_SRVLNC_END + item->index * 8);
	}

	return 0;
}
/******************************************************************************/

static int hiwz_clock_config(struct hiwz_host *host, bool enable)
{
	u32 regval;

	if (enable) {
		regval = hiwz_read(host, WADDR_SRVLNC_CLKGCFG);
		regval |= (WADDR_SRVLNC_DETADDR_GT_EN 
			   | WADDR_SRVLNC_APB_GT_EN);
		hiwz_write(host, regval, WADDR_SRVLNC_CLKGCFG);
	} else {
		regval = hiwz_read(host, WADDR_SRVLNC_CLKGCFG);
		regval &= ~(WADDR_SRVLNC_DETADDR_GT_EN 
			    | WADDR_SRVLNC_APB_GT_EN);
		hiwz_write(host, regval, WADDR_SRVLNC_CLKGCFG);
	}

	return 0;
}
/******************************************************************************/

static int hiwz_init_item(struct hiwz_host *host)
{
	int ix;

	/* invalidate all watchzone items. */
	for (ix = 0; ix < MAX_WATCH_ZONE_ITEM; ix++) {
		hiwz_write(host, 0, AWADDR_SRVLNC_START + ix * 8);
		hiwz_write(host, 0, AWADDR_SRVLNC_END + ix * 8);
	}

	return 0;
}
/******************************************************************************/

static int fs_ctrl_init(void)
{
	if (!host)
		return -1;

	/* init watchpoint */
	hiwz_init_item(host);

	/* enable watchpoint clock. */
	hiwz_clock_config(host, ENABLE);

	return 0;
}
/******************************************************************************/

static int fs_wz_insert(ulong phy_pagestart, ulong phy_pageend, int midmask, u32 flags)
{
	int ret;
	struct wz_item *item;

	if (!flags)
		flags = WP_RANDOM;

	mutex_lock(&__ddr_wz->mutex);
	item = wz_item_find(phy_pagestart, phy_pageend);
	if (item) {
		pr_debug("%d: 0x%08lx  0x%08x  0x%08x\n",
			item->index, item->phy_pagestart << PAGE_SHIFT, item->flags, item->midmask);

		item->flags |= flags;
		item->midmask |= midmask;
		// TODO: 如果没有变化, 不更新硬件.

		pr_debug("%d: 0x%08lx  0x%08x  0x%08x\n",
			item->index, item->phy_pagestart << PAGE_SHIFT, item->flags, item->midmask);
	} else {
		item = new_wz_item();
		if (!item) {
			goto exit;
		}

		item->flags           = flags;
		item->midmask         = midmask;
		item->phy_pagestart   = phy_pagestart;
		item->phy_pageend     = phy_pageend;
		item->virt_pagestart  = (ulong)phys_to_virt((phy_pagestart << PAGE_SHIFT));
		item->virt_pageend    = (ulong)phys_to_virt((phy_pageend << PAGE_SHIFT));

		ret = wz_item_insert(item);
		BUG_ON(ret != 0);

		fs_ctrl_update(item);
	}

exit:
	mutex_unlock(&__ddr_wz->mutex);

	return 0;
}
/******************************************************************************/

static int fs_wz_remove(ulong phy_pagestart, ulong phy_pageend, int midmask, u32 flags)
{
	int update = 0;
	struct wz_item *item;

	if (!flags)
		flags = WP_RANDOM;

	mutex_lock(&__ddr_wz->mutex);

	item = wz_item_find(phy_pagestart, phy_pageend);
	if (item && item->phy_pagestart == phy_pagestart && item->phy_pageend == phy_pageend) {
		if (item->midmask & midmask) {
			item->midmask &= ~midmask;
			update = 1;
		}

		if (item->flags & flags) {
			item->flags &= ~flags;
			update = 1;
		}

		if (update)
			fs_ctrl_update(item);

		if ((!item->flags) && (!item->midmask))
			wz_item_remove(item);
	}

	mutex_unlock(&__ddr_wz->mutex);

	return 0;
}
/******************************************************************************/

static int get_wz_midmask(int mid)
{
	int ix;

	for (ix = 0; ix < 32; ix++) {
		if (mid_info[ix].mid == mid)
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

static int wz_item_init(void)
{
	int ix;
	struct wz_item **item;

	__ddr_wz = kzalloc(sizeof(struct ddr_wz), GFP_KERNEL);
	if (!__ddr_wz) {
		pr_err("out of memory.\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&(__ddr_wz->head));

	item = &__ddr_wz->free_item;
	for (ix = 0; ix < MAX_WATCH_ZONE_ITEM; ix++) {
		*item = &__ddr_wz->item[ix];
		(*item)->index = ix;

		item = &__ddr_wz->item[ix].next;
	}
	*item = NULL;

	mutex_init(&__ddr_wz->mutex);
	return 0;
}
/******************************************************************************/

static int ddr_wz_init(void)
{
	int ret;

	ret = wz_item_init();
	if (ret)
		return ret;

	fs_ctrl_init();

	return 0;
}
/******************************************************************************/
/*
 *  echo "[+/-]virtstart,virtend,<mid_name>;<[+/-]virtstart,virtend,<mid_name>>"
 */
static ssize_t ddr_wz_fops_write(struct file *filp, const char __user *ubuf,
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
		ulong s_virtaddr, s_physaddr;
		ulong e_virtaddr, e_physaddr;

		if (*ptr != '+' && *ptr != '-') {
			ptr++;
			continue;
		}

		if (*ptr++ == '+')
			enable = 1;
		else
			enable = 0;

		s_virtaddr = simple_strtoul(ptr, &ptr, 16);
		if (*ptr++ != ',') {
			pr_warn("bad format: %s\n", ptr);
			continue;
		}

		if (s_virtaddr < PAGE_OFFSET || s_virtaddr > (unsigned long)high_memory) {
			pr_err("virt start addr is invalid, must be (0x%08lx~0x%08lx)\n", PAGE_OFFSET, (unsigned long)high_memory);
			return -EINVAL;
		}

		e_virtaddr = simple_strtoul(ptr, &ptr, 16);
		if (s_virtaddr >= e_virtaddr) {
			pr_warn("bad format: %s\n", ptr);
			continue;
		}

		if (e_virtaddr < PAGE_OFFSET || e_virtaddr > (unsigned long)high_memory) {
			pr_err("virt end addr is invalid, must be (0x%08lx~0x%08lx)\n", PAGE_OFFSET, (unsigned long)high_memory);
			return -EINVAL;
		}

		s_physaddr = virt_to_phys((void *)s_virtaddr);

		e_physaddr = (ulong)virt_to_phys((void *)e_virtaddr);


		if (*ptr++ == ',') {
			int mid;
			char *name = ptr;

			while (ptr < ptail && (*ptr != ';' || *ptr != '\0'))
				ptr++;

			mid = get_master_id(name, ptr - name);
			if (mid >= 0)
				midmask = get_wz_midmask(mid);
		}

		if (*ptr != '\0' && *ptr != ';') {
			pr_warn("bad format: %s\n", ptr);
			break;
		}

		if (enable) {
			fs_wz_insert(s_physaddr >> PAGE_SHIFT, 
				e_physaddr >> PAGE_SHIFT, 
				midmask, WP_KERNELTEXT);
		} else {
			fs_wz_remove(s_physaddr >> PAGE_SHIFT, 
				e_physaddr >> PAGE_SHIFT, 
				midmask, WP_KERNELTEXT);
		}
	}

	vfree(kbuf);

	return sz_ubuf;
}
/******************************************************************************/

static ssize_t ddr_wz_fops_read(struct file *filp, char __user *ubuf,
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

		ret = snprintf(ptr, leave, "ddr watch zone config\n  ");
		ptr += ret;
		leave -= ret;

		for (ix = 0; args_opt[ix].name; ix++) {
			if (!args_opt[ix].symbol)
				continue;

			ret = snprintf(ptr, leave, "%c - %s, ", args_opt[ix].symbol, args_opt[ix].name);
			ptr += ret;
			leave -= ret;
		}

		ret = snprintf(ptr, leave, "\nphy-addr    virt-start   virt-end    flags  access modules\n");
		ptr += ret;
		leave -= ret;

		read_info->index = 0;
		read_info->ptr = read_info->buf;
		read_info->ptail = ptr;
	}

	if (read_info->ptr >= read_info->ptail) {
		struct wz_item *item = vmalloc(sizeof(struct wz_item));
		if (!item) {
			pr_err("malloc failed.\n");
			return -ENOMEM;
		}

		ptr = read_info->buf;

		for (ix = read_info->index; ix < MAX_WATCH_ZONE_ITEM; ix++) {
			int jx;

			mutex_lock(&__ddr_wz->mutex);
			memcpy(item, &__ddr_wz->item[ix], sizeof(struct wz_item));
			mutex_unlock(&__ddr_wz->mutex);

			if (!item->flags && !item->midmask)
				continue;

			ret = snprintf(ptr, leave, "0x%08lx  ", item->phy_pagestart << PAGE_SHIFT);
			ptr += ret;
			leave -= ret;

			if (item->virt_pagestart)
				ret = snprintf(ptr, leave, "0x%08lx   ", item->virt_pagestart);
			else
				ret = snprintf(ptr, leave, "            ");
			ptr += ret;
			leave -= ret;

			if (item->virt_pageend)
				ret = snprintf(ptr, leave, "0x%08lx  ", item->virt_pageend);
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

			for (jx = 0; jx < MAX_WATCH_ZONE_ITEM; jx++) {
				if (item->midmask & (1 << jx)) {
					ret = snprintf(ptr, leave, "%s/", mid_info[jx].name);
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

static int ddr_wz_fops_open(struct inode *inode, struct file *filp)
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

static int ddr_wz_fops_release(struct inode *inode, struct file *filp)
{
	struct read_info *read_info = filp->private_data;
	if (read_info->buf)
		vfree(read_info->buf);
	kfree(read_info);
	return 0;
}
/******************************************************************************/

static void dump_status_wz(struct hiwz_host *host)
{
	u32 ix, regval;
	u32 ddrtrample_mid;

	regval = readl(host->regbase + AWADDR_SRVLNC_MID);
	ddrtrample_mid = regval & AWADDR_SRVLNC_MID_MASK;

	printk("dump watchzone status:\n");
	printk("	Master MID:	0x%05x\n", ddrtrample_mid);
	printk("	Master Name:	%s\n", get_master_name(ddrtrample_mid));
	printk("	Trample Addr:	\nstartaddr	endaddr\n");
	regval = hiwz_read(host, AWADDR_SRVLNC_STATUS);
	for (ix = 0; ix < 32; ix++) {
		if (regval & (1 << ix))
			printk("0x%08x	0x%08x\n", 
				hiwz_read(host, AWADDR_SRVLNC_START + ix * 4),
				hiwz_read(host, AWADDR_SRVLNC_END + ix * 4));
	}
}
/******************************************************************************/

static irqreturn_t hiwz_irq_handle(int irq, void *dev_id)
{
	unsigned int regval;
	struct hiwz_host *host = (struct hiwz_host *)dev_id;

	regval = readl(host->regbase + AWADDR_SRVLNC_STATUS);
	if (regval) {
		/* dump status must befor clear int, or count will be reset. */
		dump_status_wz(host);

		writel(0xffffffff, host->regbase + AWADDR_SRVLNC_STATUS);
	}

	return IRQ_HANDLED;
}
/******************************************************************************/

static struct file_operations ddr_wz_fops = {
	.open     = ddr_wz_fops_open,
	.read     = ddr_wz_fops_read,
	.write    = ddr_wz_fops_write,
	.llseek   = no_llseek,
	.release  = ddr_wz_fops_release,
};
/*****************************************************************************/

static int hiwz_driver_probe(struct platform_device *pdev)
{
	int ret = -1;
	struct resource *res;

	if (host)
		return 0;

	host = devm_kzalloc(&pdev->dev, sizeof(struct hiwz_host), GFP_KERNEL);
	if (!host) {
		pr_err("failed to allocate host structure.\n");
		goto error;
	}

	platform_set_drvdata(pdev, host);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	host->regbase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR_OR_NULL(host->regbase)) {
		pr_err("%s: regbase ioremap fail.\n", __func__);
		goto error;
	}

	host->irq = platform_get_irq(pdev, 0);
	if (host->irq < 0) {
		pr_err("no irq defined\n");
		goto error;
	}
	ret = request_irq(host->irq, hiwz_irq_handle, IRQF_SHARED, DEVNAME"-irq", host);
	if (ret) {
		pr_err("unable to request irq %d\n", host->irq);
		ret = -EIO;
		goto error;
	}

	return 0;

error:
	return ret;
}
/*****************************************************************************/

static const struct of_device_id
hisilicon_ddr_watchpoint_match[] __maybe_unused = {
	{ .compatible = "hisilicon.ddr_watchzone", },
	{},
};
MODULE_DEVICE_TABLE(of, hisilicon_ddr_watchpoint_match);

static struct platform_driver hiwz_pltm_driver = {
	.probe  = hiwz_driver_probe,
	.driver = {
		.name = DEVNAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(hisilicon_ddr_watchpoint_match),
	},
};
/******************************************************************************/

static int __init ddr_wz_module_init(void)
{
	int ret = 0;

	if (!__wz_flags)
		return 0;

	ret = platform_driver_register(&hiwz_pltm_driver);
	if (ret)
		return ret;

	__wz_entry = debugfs_create_file("ddr_wz", 0644, NULL, NULL,
		&ddr_wz_fops);
	if (!__wz_entry) {
		pr_err("create debugfs file failed.\n");
		platform_driver_unregister(&hiwz_pltm_driver);
		return -ENOMEM;
	}

	ret = ddr_wz_init();
	if (ret) {
		platform_driver_unregister(&hiwz_pltm_driver);
		debugfs_remove(__wz_entry);
	}

	return ret;
}
/******************************************************************************/

static void __exit ddr_wz_module_exit(void)
{
	if (!__wz_flags)
		return;

	if (__wz_entry)
		debugfs_remove(__wz_entry);

	free_irq(host->irq, host);

	platform_driver_unregister(&hiwz_pltm_driver);
}
/******************************************************************************/

module_init(ddr_wz_module_init);
module_exit(ddr_wz_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yan Jiantao");
MODULE_AUTHOR("Cai Zhiyong");
MODULE_DESCRIPTION("kernel ddr watch zone");
