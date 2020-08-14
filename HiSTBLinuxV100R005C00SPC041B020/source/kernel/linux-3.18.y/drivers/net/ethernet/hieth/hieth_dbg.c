/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by ZhangYixing
 *
******************************************************************************/
#define pr_fmt(fmt) "mmcdbg: " fmt

#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <linux/mutex.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>
#include <uapi/linux/if_ether.h>
#include <linux/if_ether.h>
#include <uapi/linux/if.h>

#ifndef CONFIG_ARM64
#include <mach/hardware.h>
#endif

#define MAX_MULTICAST_FILTER    8
#define MAX_MAC_LIMIT ETH_ALEN*MAX_MULTICAST_FILTER
#define MAC_FMT "	MAC%d: %02x:%02x:%02x:%02x:%02x:%02x\n"

struct ethstats {
	void __iomem *base;
	void __iomem *macbase[2];

	char prbuf[SZ_4K];
	u32 sz_prbuf;

	struct dentry *dentry;
};

struct cmd_val_t {
	const char *key;
	int sz_key;
	char *buf;
	int sz_buf;
};

struct mcdump {
	void __iomem *base;
	u32 net_flags;
	u32 mc_rcv;
	u32 mc_drop;
	u32 mac_nr;
	spinlock_t lock;
	char mac[MAX_MAC_LIMIT];
	char prbuf[SZ_1K];
	u32 sz_prbuf;

	struct dentry *dentry;
};

/******************************************************************************/

static struct ethstats ethstats = {0};
static struct mcdump mcdump = {0};
static struct dentry *root = NULL;

/******************************************************************************/
static int dump_eth_stats(int index, void __iomem *macbase, char *buf, int sz_buf)
{
	int ix;
	int count;
	char *ptr = buf;

	struct regdef {
		char *fmt;
		u32 offset;
	} regdef[] = {
		{"  Rx:%u Bytes\n", 0x608},
		{"    total packets:%u ", 0x610},
		{"broadcast:%u ", 0x614},
		{"multicast:%u ", 0x618},
		{"unicast:%u ", 0x61C},
		{"to me:%u\n", 0x60C},
		{"    error packets:%u ", 0x620},
		{"crc/alignment:%u ", 0x624},
		{"invalid size:%u ", 0x628},
		{"nibble error:%u\n", 0x62C},
		{"    pause frame:%u, ", 0x630},
		{"overflow: %u ", 0x634},
		{"mac filterd: %u\n", 0x64c},
		{"  Tx:%u Bytes\n", 0x790},
		{"    total packets:%u ", 0x780},
		{"broadcast:%u ", 0x784},
		{"multicast:%u ", 0x788},
		{"unicast:%u\n", 0x78C},
	};

	count = snprintf(ptr, sz_buf, "eth%d:\n", index);
	sz_buf -= count;
	ptr += count;

	for (ix = 0; ix < ARRAY_SIZE(regdef); ix++) {
		count = snprintf(ptr, sz_buf, regdef[ix].fmt, readl(macbase + regdef[ix].offset));
		sz_buf -= count;
		ptr += count;
	}

	return ptr - buf;
}

/******************************************************************************/

static ssize_t fo_dump_ethstats_read(struct file *filp, char __user *ubuf,
			size_t sz_ubuf, loff_t *ppos)
{
	struct ethstats *stats = filp->private_data;

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, stats->prbuf,
		stats->sz_prbuf);
}

/******************************************************************************/

static int fo_dump_ethstats_open(struct inode *inode, struct file *file)
{
	struct ethstats *data;
	int count = 0, sz_buf;
	char *ptr;

	data = &ethstats;

	file->private_data = (void *)data;

	ptr = data->prbuf;
	sz_buf = sizeof(data->prbuf);

	count = dump_eth_stats(0, data->macbase[0], ptr, sz_buf);

	data->sz_prbuf = count;

	return nonseekable_open(inode, file);
}
/******************************************************************************/

int multicast_dump_netdev_flags(u32 flags)
{
	u32 old = mcdump.net_flags;
	spin_lock(&mcdump.lock);
	mcdump.net_flags = flags;
	spin_unlock(&mcdump.lock);
	return old;
}
/******************************************************************************/

void multicast_dump_macaddr(u32 nr, char *macaddr)
{
	char *ptr;
	if (nr > MAX_MULTICAST_FILTER)
		return;

	ptr = mcdump.mac + nr*ETH_ALEN;
	spin_lock(&mcdump.lock);
		memcpy(ptr, macaddr, ETH_ALEN);
		mcdump.mac_nr = nr+1;
	spin_unlock(&mcdump.lock);
}

/******************************************************************************/

static int dump_mc_drop(int index, struct mcdump *dump, char *buf, int sz_buf)
{
	int ix;
	int count;
	char *ptr = buf;
	char *pmac;

	struct regdef {
		char *fmt;
		u32 offset;
	} regdef[] = {
		{"	Rx packets:%u ", 0x618},
		{"dropped:%u\n", 0x64C},
	};

	count = snprintf(ptr, sz_buf, "eth%d multicast:\n", index);
	sz_buf -= count;
	ptr += count;

	for (ix = 0; ix < ARRAY_SIZE(regdef); ix++) {
		count = snprintf(ptr, sz_buf, regdef[ix].fmt, readl(dump->base + regdef[ix].offset));
		sz_buf -= count;
		ptr += count;
	}

	count = snprintf(ptr, sz_buf, "%s", "	state:");
	sz_buf -= count;
	ptr += count;

	if (dump->net_flags & IFF_MULTICAST) {
		count = snprintf(ptr, sz_buf, "%s", "MULTICAST ");
		sz_buf -= count;
		ptr += count;
	}

	if (dump->net_flags & IFF_PROMISC) {
		count = snprintf(ptr, sz_buf, "%s", "PROMISC ");
		sz_buf -= count;
		ptr += count;
	}

	if (dump->net_flags & IFF_ALLMULTI) {
		count = snprintf(ptr, sz_buf, "%s", "ALLMULTI ");
		sz_buf -= count;
		ptr += count;
	}

	count = snprintf(ptr, sz_buf, "\n	mac filters:%d \n", dump->mac_nr);
	sz_buf -= count;
	ptr += count;

	for (ix = 0; ix < dump->mac_nr; ix++) {
		pmac = dump->mac + ix*ETH_ALEN;
		count = snprintf(ptr, sz_buf, MAC_FMT, ix,
					pmac[0], pmac[1], pmac[2],
					pmac[3], pmac[4], pmac[5]);
		sz_buf -= count;
		ptr += count;
	}


	return ptr - buf;
}
/******************************************************************************/

static int fo_dump_ethmc_open(struct inode *inode, struct file *file)
{
	struct mcdump *data;
	int count = 0, sz_buf;
	char *ptr;

	data = &mcdump;

	file->private_data = (void *)data;

	ptr = data->prbuf;
	sz_buf = sizeof(data->prbuf);

	count = dump_mc_drop(0, data, ptr, sz_buf);

	data->sz_prbuf = count;

	return nonseekable_open(inode, file);
	//return 0;
}

/******************************************************************************/

static ssize_t fo_dump_ethmc_read(struct file *filp, char __user *ubuf,
				 size_t sz_ubuf, loff_t *ppos)
{
	struct mcdump *dump = (struct mcdump *)filp->private_data;

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, dump->prbuf,
		dump->sz_prbuf);
}

/******************************************************************************/

static struct file_operations ethmc_fops = {
	.owner = THIS_MODULE,
	.open = fo_dump_ethmc_open,
	.read  = fo_dump_ethmc_read,
	.llseek = no_llseek,
};

static struct file_operations ethstats_fops = {
	.owner = THIS_MODULE,
	.open = fo_dump_ethstats_open,
	.read  = fo_dump_ethstats_read,
	.llseek = no_llseek,
};

/******************************************************************************/

int hieth_dbg_init(void __iomem *base)
{
	char buf[16];
	unsigned int mode = S_IFREG | S_IRUSR | S_IWUSR;

	snprintf(buf, sizeof(buf), "%s", "hieth");
	root = debugfs_create_dir(buf, NULL);
	if (!root) {
		pr_err("Can't create '%s' dir.\n", buf);
		return -ENOENT;
	}

	mcdump.base = base;
	mcdump.dentry = debugfs_create_file("multicast", mode, root, NULL, &ethmc_fops);
	if (!mcdump.dentry) {
		pr_err("Can't create 'read' file.\n");
		goto fail;
	}

	ethstats.base = base;
	ethstats.macbase[0] = ethstats.base;
	ethstats.macbase[1] = ethstats.base + 0x2000;
	ethstats.dentry = debugfs_create_file("stats", mode, root, NULL, &ethstats_fops);
	if (!ethstats.dentry) {
		pr_err("Can't create 'write' file.\n");
		goto fail;
	}

	return 0;
fail:
	debugfs_remove_recursive(root);

	return -ENOENT;
}
/*****************************************************************************/

int hieth_dbg_deinit(void)
{
	debugfs_remove_recursive(root);
	return 0;
}
