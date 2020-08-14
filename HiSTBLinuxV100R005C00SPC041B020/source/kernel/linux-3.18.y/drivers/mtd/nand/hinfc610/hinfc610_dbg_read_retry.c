/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-05
 *
******************************************************************************/

#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

#ifndef CONFIG_HINFC610_DBG_READ_RETRY_NUM
#  define CONFIG_HINFC610_DBG_READ_RETRY_NUM          (100)
#endif /* CONFIG_HINFC610_DBG_READ_RETRY_NUM */

struct hinfc610_dbg_read_retry_item_t {
	unsigned int page;

	unsigned short hour;
	unsigned short min;
	unsigned short sec;
	unsigned short msec;

	unsigned short retry;  /* success retry */
	unsigned short ecc_err;
};

struct hinfc610_dbg_read_retry_t {

	struct dentry *dentry;
	unsigned int index; /* current logs index */
	int count;          /* number of logs */

	unsigned int offset;
	unsigned int length;
	unsigned int pagecount;

	unsigned int read_index;

	unsigned int max_retry; /* the max read retry times */
	unsigned int retry[16];

	struct hinfc610_dbg_read_retry_item_t item[CONFIG_HINFC610_DBG_READ_RETRY_NUM];
};

static DEFINE_MUTEX(dbg_read_retry_mutex);
static struct hinfc610_dbg_read_retry_t *dbg_read_retry = NULL;
/*****************************************************************************/

static void do_gettime(unsigned short *hour, unsigned short *min,
		       unsigned short *sec, unsigned short *msec)
{
	long val;
	struct timeval tv;

	do_gettimeofday(&tv);
	val = tv.tv_sec % 86400; /* the second form 0 hour */

	if (hour)
		*hour = val / 3600;
	val %= 3600;
	if (min)
		*min  = val / 60;
	if (sec)
		*sec  = val % 60;
	if (msec)
		*msec = tv.tv_usec / 1000;
}
/*****************************************************************************/

static ssize_t dbgfs_read_retry_read(struct file *filp, char __user *buffer,
				    size_t count, loff_t *ppos)
{
	int ix;
	char *ptr;
	int len = 0;
	char buf[128] = {0};
	unsigned int read_index;
	char __user *pusrbuf = buffer;
	struct hinfc610_dbg_read_retry_item_t *item;

	if (*ppos == 0) {
		if (dbg_read_retry->count
		    < CONFIG_HINFC610_DBG_READ_RETRY_NUM)
			dbg_read_retry->read_index = 0;
		else
			dbg_read_retry->read_index
				= (dbg_read_retry->index + 1);

		len = snprintf(buf, sizeof(buf),
			"Print parameter: \"offset=%d length=%d\" (offset is page number)\n",
			dbg_read_retry->offset,
			dbg_read_retry->length);

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;

		len = snprintf(buf, sizeof(buf),
			"  UTC Clock    page          read retry (max: %d)\n",
			dbg_read_retry->max_retry);

		ptr = buf;
		ptr += sprintf(ptr, "Read retry: ");
		for (ix = 1; ix <= dbg_read_retry->max_retry; ix++)
			ptr += sprintf(ptr, "%d, ", dbg_read_retry->retry[ix]);
		ptr += sprintf(ptr, "\n");

		len = (ptr - buf);
		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;


		len = snprintf(buf, sizeof(buf),
			"  UTC Clock    page          read retry (max: %d)\n",
			dbg_read_retry->max_retry);

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;

	} else if (dbg_read_retry->read_index == dbg_read_retry->index)
		return 0;

	for (read_index = dbg_read_retry->read_index;
	     (read_index != dbg_read_retry->index); ++read_index) {

		if (read_index >= CONFIG_HINFC610_DBG_READ_RETRY_NUM)
			read_index = 0;

		item = &dbg_read_retry->item[read_index];

		if ((count - (pusrbuf - buffer)) < 80)
			break;

		len = snprintf(buf, sizeof(buf),
			"%02d:%02d:%02d.%04d  0x%08X    ",
			item->hour, item->min, item->sec, item->msec,
			item->page);

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;

		if (!item->ecc_err)
			len = sprintf(buf, "%d\n", item->retry);
		else
			len = sprintf(buf, "fail\n");

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;
	}

	dbg_read_retry->read_index = read_index;

	*ppos += (pusrbuf - buffer);
	return pusrbuf - buffer;
}
/******************************************************************************/
/*
 * echo "offset=8192,length=102400" > read_retry
 *
 */
static ssize_t dbgfs_read_retry_write(struct file *filp,
				     const char __user *buffer, size_t count,
				     loff_t *ppos)
{
	char *str;
	char buf[128] = {0};
	unsigned long value;
	unsigned long pos = 0;

	if (count > sizeof(buf))
		count = sizeof(buf);

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	while (pos < count) {
		while (pos < count &&
			(buf[pos] == ' ' || buf[pos] == ',' || buf[pos] == ';'))
			pos++;

		if (pos >= count)
			break;

		switch (buf[pos]) {

		case 'o':

			if (memcmp(&buf[pos], CMD_WORD_OFFSET,
				sizeof(CMD_WORD_OFFSET) - 1))
				break;

			pos += sizeof(CMD_WORD_OFFSET) - 1;
			str = (char *)(buf + pos);
			value = simple_strtoul(str, &str, 0);

			if (value < 0)
				value = 0;
			if (value >= dbg_read_retry->pagecount)
				value = 0;

			dbg_read_retry->offset = (value & ~7);

			break;

		case 'l':
			if (memcmp(&buf[pos], CMD_WORD_LENGTH,
				sizeof(CMD_WORD_LENGTH) - 1))
				break;

			pos += sizeof(CMD_WORD_LENGTH) - 1;
			str = (char *)(buf + pos);
			value = simple_strtoul(str, &str, 0);

			if (value < 0)
				value = dbg_read_retry->pagecount;

			value = ((value + 7) & ~7);

			if (dbg_read_retry->offset + value >
			    dbg_read_retry->pagecount)
				value = dbg_read_retry->pagecount
					- dbg_read_retry->offset;

			dbg_read_retry->length = value;

			break;
		}

		while (pos < count &&
			(buf[pos] != ' ' && buf[pos] != ',' && buf[pos] != ';'))
			pos++;
	}

	return count;
}
/******************************************************************************/

static struct file_operations dbgfs_read_retry_fops = {
	.owner = THIS_MODULE,
	.read  = dbgfs_read_retry_read,
	.write = dbgfs_read_retry_write,
};
/*****************************************************************************/

static int dbgfs_read_retry_init(struct dentry *root, struct hinfc_host *host)
{
	unsigned int pagesize;
	unsigned int chipsize;
	struct hinfc610_dbg_read_retry_t *read_retry;

	if (dbg_read_retry)
		return 0;

	if (!host->read_retry) {
		pr_err("The NAND not support this interface.\n");
		return -1;
	}

	read_retry = vmalloc(sizeof(struct hinfc610_dbg_read_retry_t));
	if (!read_retry) {
		pr_err("Can't allocate memory.\n");
		return -ENOMEM;
	}
	memset(read_retry, 0, sizeof(struct hinfc610_dbg_read_retry_t));

	pagesize  = (host->pagesize >> 10);
	chipsize = (unsigned int)(host->chip->chipsize >> 10);
	read_retry->pagecount = (chipsize / pagesize);
	read_retry->length = read_retry->pagecount;
	read_retry->max_retry = host->read_retry->count;

	if (read_retry->max_retry > 16) {
		vfree(read_retry);
		pr_err("Bug, max_retry too small.\n");
		return -EFAULT;
	}

	read_retry->dentry = debugfs_create_file("read_retry",
		S_IFREG | S_IRUSR | S_IWUSR, 
		root, NULL, &dbgfs_read_retry_fops);
	if (!read_retry->dentry) {
		pr_err("Can't create 'read_retry' file.\n");
		vfree(read_retry);
		return -ENOENT;
	}

	dbg_read_retry = read_retry;

	return 0;
}
/*****************************************************************************/

static int dbgfs_read_retry_uninit(struct hinfc_host *host)
{
	if (!dbg_read_retry)
		return 0;

	mutex_lock(&dbg_read_retry_mutex);

	debugfs_remove(dbg_read_retry->dentry);

	vfree(dbg_read_retry);
	dbg_read_retry = NULL;

	mutex_unlock(&dbg_read_retry_mutex);

	return 0;
}
/*****************************************************************************/

static void hinfc610_dbg_read_retry_rr(struct hinfc_host *host, int index)
{
	unsigned int page;
	struct hinfc610_dbg_read_retry_item_t *item;

	mutex_lock(&dbg_read_retry_mutex);

	if (!dbg_read_retry)
		goto exit;

	page = GET_RW_PAGE_INDEX(host);

	if (page < dbg_read_retry->offset ||
	    page > (dbg_read_retry->offset + dbg_read_retry->length))
		goto exit;

	item = &dbg_read_retry->item[dbg_read_retry->index];

	dbg_read_retry->count++;

	do_gettime(&item->hour, &item->min, &item->sec, &item->msec);

	item->page = page;
	item->retry = index;

	item->ecc_err = IS_PS_UN_ECC(host) ? 1 : 0;
	if (!item->ecc_err)
		dbg_read_retry->retry[index]++;

	if (++dbg_read_retry->index >= CONFIG_HINFC610_DBG_READ_RETRY_NUM)
		dbg_read_retry->index = 0;

exit:
	mutex_unlock(&dbg_read_retry_mutex);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_read_retry = {
	"read_retry", 0,
	dbgfs_read_retry_init,
	dbgfs_read_retry_uninit,
	NULL,
	NULL,
	NULL,
	hinfc610_dbg_read_retry_rr,
};
/*****************************************************************************/
