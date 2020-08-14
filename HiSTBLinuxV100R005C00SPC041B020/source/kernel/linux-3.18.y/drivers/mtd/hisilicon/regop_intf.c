/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.7.25
 *
******************************************************************************/

#define pr_fmt(fmt) "mtd-regop: " fmt

#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/uaccess.h>

#include "hiflash.h"

struct regop_intf_data {
	struct flash_regop_info regop_info;

	struct proc_dir_entry *dentry_regop;
	struct proc_dir_entry *dentry_data;

	char *buf;
	u32 sz_buf;

	/* The lately writing operation data size */
	int sz_buf_lately;

	/*
	 * The proc read/write buffer is invalid, it needs be updated before
	 * the next reading operation.
	 */
	int need_update;
	char prbuf[1024];
	int sz_prbuf;
};

struct cmd_val_t {
	const char *key;
	int sz_key;
	char *buf;
	int sz_buf;
};

static int init = 0;
/******************************************************************************/

static void regop_proc_init(void)
{
	struct proc_dir_entry *root;
	if (init)
		return;

	init++;
	root = proc_mkdir("flash", NULL);
	if (root)
		proc_mkdir("regop", root);
}
/******************************************************************************/

static void regop_dump_help(struct regop_intf_data *data)
{
	int count;
	char *ptr = data->prbuf;
	int sz_buf = sizeof(data->prbuf);
	struct flash_regop_info *info = &data->regop_info;

	count = snprintf(ptr, sz_buf, "The lately regop:\n");
	sz_buf -= count;
	ptr += count;

	count = flash_regop_info_dump(info, ptr, sz_buf);
	if (!count)
		count = snprintf(ptr, sz_buf, "NULL");
	sz_buf -= count;
	ptr += count;

	count = snprintf(ptr, sz_buf, "\n\n"
		"example:\n"
		"  read spi-nor id:\n"
		"    echo \"cmd=0x9f,nr_cmd=1,sz_buf=8,read\" > /proc/flash/regop/spinor \n"
		"    hexdump -v -C /proc/flash/regop/spinor.data\n"
		"\n"
		"  read spi-nand id:\n"
		"    echo \"cmd=0x9f,nr_cmd=1,addr_cycle=1,sz_buf=4,read\" > /proc/flash/regop/spinand\n"
		"    hexdump -v -C /proc/flash/regop/spinand.data\n"
		"\n"
		"  read nand id:\n"
		"    echo \"cmd=0x90,nr_cmd=1,addr_cycle=1,sz_buf=8,read\" > /proc/flash/regop/nand\n"
		"    hexdump -v -C /proc/flash/regop/nand.data\n"
		"\n");

	sz_buf -= count;
	ptr += count;

	count = snprintf(ptr, sz_buf, "Key Word: cmd=0xNNNN,nr_cmd=N,addrh=0xNN,addrl=0xNNNNNNNN,addr_cycle=N,dummy=N,sz_buf=N,msleep=N,read/write,\n");
	sz_buf -= count;
	ptr += count;

	data->sz_prbuf = ptr - data->prbuf;

	data->need_update = false;
}
/******************************************************************************/

static ssize_t regop_read(struct file *filp, char __user *ubuf,
			  size_t sz_ubuf, loff_t *ppos)
{
	struct flash_regop_intf *intf = PDE_DATA(file_inode(filp));
	struct regop_intf_data *data = intf->priv;

	if (!*ppos && data->need_update)
		regop_dump_help(data);

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, data->prbuf,
		data->sz_prbuf);
}
/******************************************************************************/

static char *regop_resolve(char *buf, char *ptail, struct flash_regop_info *info,
			   char *op, int *msecs)
{
	int ix;
	int _msecs = 0;

#define _SCMD(_cmd, _val) \
	((void *)&(struct cmd_val_t){       \
		.key    = _cmd,             \
		.sz_key = sizeof(_cmd) - 1, \
		.buf    = (char *)&_val,    \
		.sz_buf = sizeof(_val),     \
})

	struct cmd_val_t *cmd_val[] = {
		_SCMD("write", ix),
		_SCMD("read", ix),
		_SCMD("cmd=", info->cmd),
		_SCMD("nr_cmd=", info->nr_cmd),
		_SCMD("addrl=", info->addrl),
		_SCMD("addrh=", info->addrh),
		_SCMD("addr_cycle=", info->addr_cycle),
		_SCMD("dummy=", info->dummy),
		_SCMD("sz_buf=", info->sz_buf),
		_SCMD("msleep=", _msecs),
	};

	*op = 0;
	memset(info, 0, sizeof(struct flash_regop_info));

	while (buf < ptail) {
		if (!isalpha(*buf)) {
			buf++;
			continue;
		}

		for (ix = 0; ix < ARRAY_SIZE(cmd_val); ix++) {
			ulong value;
			struct cmd_val_t *val = cmd_val[ix];

			if (buf + val->sz_key > ptail)
				continue;

			if (memcmp(buf, val->key, val->sz_key))
				continue;

			if (ix == 0 || ix == 1) {
				if (isalpha(buf[val->sz_key]))
					continue;

				*op = val->key[0];
				buf += val->sz_key;

				goto out;
			}

			buf += val->sz_key;

			value = (ulong)memparse(buf, &buf);

			memcpy(val->buf, &value,
				min_t(ulong, val->sz_buf, sizeof(value)));

			break;
		}

		if (ix == ARRAY_SIZE(cmd_val)) {
			char chr;
			char *nextbuf = buf;

			while (nextbuf < ptail && isalpha(*nextbuf))
				nextbuf++;

			chr = *nextbuf;
			*nextbuf = '\0';
			pr_err("Invalid key word: \"%s\"\n", buf);
			*nextbuf = chr;
			buf = nextbuf;
		}
	}
out:
	*msecs = _msecs;

	while (buf < ptail && !isalpha(*buf))
		buf++;

	return buf;
}
/******************************************************************************/

static ssize_t regop_write(struct file *filp, const char __user *ubuf,
			   size_t sz_ubuf, loff_t *ppos)
{
	char op;
	int msecs = 0;
	char *ptr, *kbuf, *ptail;

	struct flash_regop_intf *intf = PDE_DATA(file_inode(filp));
	struct regop_intf_data *data = intf->priv;
	struct flash_regop_info *info = &data->regop_info;

	ptr = kbuf = (char *)vmalloc(sz_ubuf + 1);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, ubuf, sz_ubuf)) {
		vfree(kbuf);
		return -EFAULT;
	}

	ptail = kbuf + sz_ubuf;
	*ptail = '\0';

	while (ptr < ptail) {
		ptr = regop_resolve(ptr, ptail, info, &op, &msecs);

		info->priv = intf->regop->priv;

		if (info->sz_buf > data->sz_buf)
			info->sz_buf = data->sz_buf;

		if (info->sz_buf)
			info->buf = data->buf;

		data->sz_buf_lately = info->sz_buf;

		if (op == 'r')
			intf->regop->read_reg(info);
		else if (op == 'w')
			intf->regop->write_reg(info);

		if (msecs) {
			if (msecs > 10000)
				msecs = 10000;
			msleep(msecs);
		}
	}

	vfree(kbuf);

	data->need_update = true;

	return sz_ubuf;
}
/******************************************************************************/

static int regop_open(struct inode *inode, struct file *file)
{
	struct flash_regop_intf *intf = PDE_DATA(inode);

	/* lock driver read/write/erase operation when do regop */
	if (FMODE_WRITE & file->f_mode)
		intf->open(intf->args);

	return nonseekable_open(inode, file);
}
/******************************************************************************/

static int regop_release(struct inode *inode, struct file *file)
{
	struct flash_regop_intf *intf = PDE_DATA(inode);

	if (FMODE_WRITE & file->f_mode)
		intf->release(intf->args);

	return 0;
}
/******************************************************************************/

static struct file_operations regop_fops = {
	.read  = regop_read,
	.write = regop_write,
	.open = regop_open,
	.release = regop_release,
	.llseek = no_llseek,
};
/******************************************************************************/

static ssize_t regop_data_read(struct file *filp, char __user *ubuf,
			       size_t sz_ubuf, loff_t *ppos)
{
	struct regop_intf_data *data = PDE_DATA(file_inode(filp));

	return simple_read_from_buffer(ubuf, sz_ubuf, ppos, data->buf,
		data->sz_buf_lately);
}
/******************************************************************************/

static ssize_t regop_data_write(struct file *filp, const char __user *ubuf,
			        size_t sz_ubuf, loff_t *ppos)
{
	struct regop_intf_data *data = PDE_DATA(file_inode(filp));

	return simple_write_to_buffer(data->buf, data->sz_buf, ppos, ubuf,
		sz_ubuf);
}
/******************************************************************************/

static struct file_operations regop_data_fops = {
	.read  = regop_data_read,
	.write = regop_data_write,
};
/******************************************************************************/

int regop_intf_create(struct flash_regop_intf *intf)
{
	char buf[64];
	struct proc_dir_entry *dentry_regop;
	struct proc_dir_entry *dentry_data;
	struct regop_intf_data *data;

	regop_proc_init();

	pr_info("Create %s register operation interface, buffer size:%d\n",
		intf->name, intf->sz_buf);

	data = kzalloc(sizeof(struct regop_intf_data) + intf->sz_buf, GFP_KERNEL);
	if (!data) {
		pr_err("failed to allocate memory for %s.\n", intf->name);
		return -ENOMEM;
	}

	intf->priv = data;

	snprintf(buf, sizeof(buf), "flash/regop/%s", intf->name);

	dentry_regop = proc_create_data(buf, 0644, NULL, &regop_fops, intf);
	if (!dentry_regop) {
		pr_err("create proc file failed.\n");
		kfree(data);
		return -ENOMEM;
	}
	data->dentry_regop = dentry_regop;

	snprintf(buf, sizeof(buf), "flash/regop/%s.data", intf->name);

	dentry_data = proc_create_data(buf, 0644, NULL, &regop_data_fops, data);
	if (!dentry_data) {
		pr_err("create proc file failed.\n");
		proc_remove(dentry_regop);
		kfree(data);
		return -ENOMEM;
	}
	data->dentry_data = dentry_data;

	data->need_update = true;
	data->sz_buf = intf->sz_buf;
	data->buf = (char *)&data[1];
	data->sz_buf_lately = 0;

	return 0;
}
/******************************************************************************/

void regop_intf_destory(struct flash_regop_intf *regop_intf)
{
	struct regop_intf_data *data = regop_intf->priv;

	proc_remove(data->dentry_data);
	proc_remove(data->dentry_regop);

	kfree(data);
}
