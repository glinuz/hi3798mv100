/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-04
 *
******************************************************************************/

#include <linux/debugfs.h>
#include <asm/uaccess.h>
#include <linux/ctype.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_dump;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_erase_count;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_ecc_count;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_retry;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_retry_notice;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_ecc_notice;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_if_cache_enable;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_if_cache_crc;
extern struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_count;

struct hinfc610_dbg_inf_t *hinfc610_dbg_inf[] = {
	&hinfc610_dbg_inf_ecc_notice,
	&hinfc610_dbg_inf_read_retry_notice,
#ifdef CONFIG_HINFC610_DBG_NAND_CACHE
	&hinfc610_dbg_if_cache_enable,
	&hinfc610_dbg_if_cache_crc,
#endif
#ifdef CONFIG_HINFC610_DBG_NAND_DUMP
	&hinfc610_dbg_inf_dump,
#endif
#ifdef CONFIG_HINFC610_DBG_NAND_READ_COUNT
	&hinfc610_dbg_inf_read_count,
#endif
#ifdef CONFIG_HINFC610_DBG_NAND_ERASE_COUNT
	&hinfc610_dbg_inf_erase_count,
#endif
#ifdef CONFIG_HINFC610_DBG_NAND_ECC_COUNT
	&hinfc610_dbg_inf_ecc_count,
#endif
#ifdef CONFIG_HINFC610_DBG_NAND_READ_RETRY
	&hinfc610_dbg_inf_read_retry,
#endif
	NULL,
};
/*****************************************************************************/

static ssize_t dbgfs_debug_read(struct file *filp, char __user *buffer,
				size_t count, loff_t *ppos)
{
	char *msg, *p;
	struct hinfc610_dbg_inf_t **inf;

	if (*ppos != 0)
		return 0;

	msg = (char *)__get_free_page(GFP_TEMPORARY);
	if (!msg)
		return -ENOMEM;

	p = msg;
	if (count > PAGE_SIZE)
		count = PAGE_SIZE;

	for (inf = hinfc610_dbg_inf; *inf; inf++) {
		if ((p - msg) + MAX_OPTION_SIZE + 2 > count) {
			pr_err("Not enough memory.\n");
			break;
		}
		p += snprintf(p, (MAX_OPTION_SIZE + 2), "%c%s,",
			      ((*inf)->enable ? '+' : '-'),
			      (*inf)->name);
	}

	p += sprintf(p, "\n");
	count = (p - msg);
	if (copy_to_user(buffer, msg, count)) {
		free_page((unsigned long) msg);
		return -EFAULT;
	}

	free_page((unsigned long) msg);

	*ppos += count;
	return count;
}
/*****************************************************************************/

static void dbgfs_debug_do_cmd(struct hinfc_host *host,
			       struct hinfc610_dbg_inf_t **dbg_inf,
			       const char *cmd, unsigned int length, int enable)
{
	int ret = 0;
	struct hinfc610_dbg_inf_t **inf;

	if (length >= sizeof((*inf)->name))
		return;

	for (inf = dbg_inf; *inf; inf++) {
		if (!(*inf)->name[length] &&
		    !memcmp((*inf)->name, cmd, length))
			break;
	}

	if (!(*inf) || (*inf)->enable == enable)
		return;

	if (enable) {
		if ((*inf)->init)
			ret = (*inf)->init(host->dbgfs_root, host);
	} else {
		if ((*inf)->uninit)
			ret = (*inf)->uninit(host);
	}

	if (!ret)
		(*inf)->enable = enable;
}
/*****************************************************************************/

static void dbgfs_debug_ops(struct hinfc_host *host, const char *options,
			    struct hinfc610_dbg_inf_t **dbg_inf)
{
	int enable;
	const char *pos, *cmd;

	pos = options;

	while (*pos) {

		while (*pos && *pos != '+' && *pos != '-')
			pos++;

		switch (*pos++) {
		case '+': enable = 1; break;
		case '-': enable = 0; break;
		default:  return;
		}

		cmd = pos;
		while (*pos == '_' || isalpha(*pos))
			pos++;

		if (*cmd && pos > cmd)
			dbgfs_debug_do_cmd(host, dbg_inf, cmd,
				(pos - cmd), enable);

		while (isspace(*pos) || *pos == ',' || *pos == ';')
			pos++;
	}
}
/*****************************************************************************/
/*
 * echo "+dump, +read_retry, +ecc_count, +erase_count"  > debug
 */
static ssize_t dbgfs_debug_write(struct file *filp, const char __user *buffer,
				 size_t count, loff_t *ppos)
{
	char *options;
	size_t num = count;

	if (count > PAGE_SIZE)
		num = (PAGE_SIZE - 1);

	options = (char *)__get_free_page(GFP_TEMPORARY);
	if (!options)
		return -ENOMEM;

	if (copy_from_user(options, buffer, num)) {
		free_page((unsigned long) options);
		return -EFAULT;
	}

	options[num] = 0;

	dbgfs_debug_ops((struct hinfc_host *)filp->private_data,
		options, hinfc610_dbg_inf);

	free_page((unsigned long) options);

	*ppos += count;
	return count;
}
/*****************************************************************************/

static int dbgfs_debug_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}
/*****************************************************************************/

static struct file_operations dbgfs_debug_fops = {
	.owner = THIS_MODULE,
	.open  = dbgfs_debug_open,
	.read  = dbgfs_debug_read,
	.write = dbgfs_debug_write,
};
/*****************************************************************************/

int hinfc610_dbgfs_debug_init(struct hinfc_host *host)
{
	struct dentry *dentry;

	dentry = debugfs_create_file("debug", S_IFREG | S_IRUSR | S_IWUSR,
		host->dbgfs_root, host, &dbgfs_debug_fops);
	if (!dentry) {
		pr_err("Can't create debugfs 'debug' file.\n");
		return -ENOENT;
	}

	if (nand_dbgfs_options)
		dbgfs_debug_ops(host, nand_dbgfs_options, hinfc610_dbg_inf);

	return 0;
}
/*****************************************************************************/

static int dbgfs_read_retry_notice_init(struct dentry *root,
					struct hinfc_host *host)
{
	if (!host->read_retry) {
		pr_warn("read_retry_notice: The NAND not support this interface.\n");
		return -1;
	}

	return 0;
}
/*****************************************************************************/

static void hinfc610_dbg_read_retry_notice(struct hinfc_host *host, int index)
{
	pr_warn("Page 0x%08x do read retry (%d/%d) %s.\n",
		GET_RW_PAGE_INDEX(host), index, host->read_retry->count,
		(IS_PS_UN_ECC(host) ? "Fail" : "Success"));
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_retry_notice = {
	"read_retry_notice", 0,
	dbgfs_read_retry_notice_init,
	NULL,
	NULL,
	NULL,
	NULL,
	hinfc610_dbg_read_retry_notice,
};
/*****************************************************************************/

static void hinfc610_dbg_ecc_notice_read(struct hinfc_host *host)
{
	unsigned int pageindex =  GET_RW_PAGE_INDEX(host);

	if (IS_PS_BAD_BLOCK(host) || IS_PS_EMPTY_PAGE(host)) {
		if (IS_PS_BBM_ERR(host))
			pr_warn("page 0x%08x bbm is corruption, bbm: 0x%x.\n",
				pageindex, *host->bbm);

		if (IS_PS_EPM_ERR(host))
			pr_warn("page 0x%08x epm is corruption, epm: 0x%x.\n",
				pageindex, *host->epm);

		return;
	}

	if (IS_PS_UN_ECC(host))
		pr_warn("page 0x%08x has uncorrect ecc.\n", pageindex);
	else if (host->ecc.max_bitsflap >= host->ecc.threshold)
		pr_warn("page 0x%08x has bitsflap (%d >= %d).\n",
			pageindex,
			host->ecc.max_bitsflap,
			host->ecc.threshold);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_ecc_notice = {
	"ecc_notice", 0,
	NULL,
	NULL,
	hinfc610_dbg_ecc_notice_read,
	NULL,
	NULL,
	NULL,
};
/*****************************************************************************/
