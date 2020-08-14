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

#ifndef CONFIG_HINFC610_DBG_ECC_COUNT_NUM
#  define CONFIG_HINFC610_DBG_ECC_COUNT_NUM          (100)
#endif /* CONFIG_HINFC610_DBG_ECC_COUNT_NUM */

struct hinfc610_dbg_ecc_count_item_t{
	unsigned int page;
	unsigned int page_status;    /* the same as host->page_status */
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
	unsigned short msec;

	unsigned char ecc[4];  /* it will dynamic malloc in init. */
};

struct hinfc610_dbg_ecc_count_t {
	struct dentry *dentry;
	unsigned int index; /* current logs index */
	int count;          /* number of logs */

	unsigned int offset;
	unsigned int length;
	unsigned int nr_page;

	unsigned char *item;

	unsigned int read_index;

	struct {
		int uncorrect; /* number of read that ecc uncorrect */
#define MAX_ECC_DIST         80
		int statis[81];
		int section;
		void (*dump)(void *host, unsigned char *ecc,
			     int *max_bitsflags);
	} ecc;
};

#define GET_ITEM(_ecc_count, _index)     \
	((struct hinfc610_dbg_ecc_count_item_t *)((_ecc_count)->item + \
		((sizeof(struct hinfc610_dbg_ecc_count_item_t) + \
			(_ecc_count)->ecc.section) * (_index))))

static DEFINE_MUTEX(dbg_ecc_count_mutex);
static struct hinfc610_dbg_ecc_count_t *dbg_ecc_count = NULL;
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

static int dbgfs_dump_ecc(struct hinfc610_dbg_ecc_count_t *ecc_count,
			  char __user *usrbuf, unsigned int count, int withhdr)
{
	int len;
	char *ptr;
	int ret = 0;
	unsigned int index;
	char buf[128] = {0};
	struct hinfc610_dbg_ecc_count_item_t *item;

	if (withhdr) {
		ptr = buf;

		ptr += snprintf(ptr, 70,
			"Parameter: \"offset=%d length=%d\" (offset is page number)\n",
			ecc_count->offset,
			ecc_count->length);

		ptr += snprintf(ptr, 50,
			"  UTC Clock    page          ecc data\n");

		len = ptr - buf;
		if (copy_to_user(usrbuf, buf, len))
			return -EFAULT;
		usrbuf += len;
		ret += len;
		count -= len;
	}

	for (index = ecc_count->read_index;
	     index != ecc_count->index && count > 80; ++index) {
		if (index >= CONFIG_HINFC610_DBG_ECC_COUNT_NUM)
			index = 0;

		item = GET_ITEM(ecc_count, index);

		len = snprintf(buf, sizeof(buf),
			"%02d:%02d:%02d.%04d  0x%08X    ",
			item->hour, item->min, item->sec, item->msec,
			item->page);

		if (copy_to_user(usrbuf, buf, len))
			return -EFAULT;
		usrbuf += len;
		ret += len;
		count -= len;

		ptr = buf;

		if (IS_PS_BAD_BLOCK(item) || IS_PS_EMPTY_PAGE(item) ||
		    IS_PS_UN_ECC(item)) {
			if (IS_PS_BAD_BLOCK(item))
				ptr += snprintf(ptr, 16, "bad block ");
			else if (IS_PS_EMPTY_PAGE(item))
				ptr += snprintf(ptr, 16, "empty page ");
			else if (IS_PS_UN_ECC(item))
				ptr += snprintf(ptr, 16, "uncorrect ");

			if (IS_PS_EPM_ERR(item))
				ptr += snprintf(ptr, 16, "bbm valid ");

			if (IS_PS_BBM_ERR(item))
				ptr += snprintf(ptr, 16, "epm valid ");

			ptr += snprintf(ptr, 4, "\n");
		} else {
			int ix;

			for (ix = 0; ix < ecc_count->ecc.section; ix++)
				ptr += snprintf(ptr, 16, "%d/", item->ecc[ix]);

			if (IS_PS_EPM_ERR(item))
				ptr += snprintf(ptr, 12, "epm valid ");

			ptr += snprintf(ptr, 4, "\n");
		}

		len = ptr - buf;
		if (copy_to_user(usrbuf, buf, len))
			return -EFAULT;

		usrbuf += len;
		ret    += len;
		count  -= len;
	}

	ecc_count->read_index = index;

	return ret;
}
/*****************************************************************************/

static int dbgfs_dump_ecc_dist(struct hinfc610_dbg_ecc_count_t *ecc_count,
			       char __user *usrbuf, unsigned int count)
{
	int ix;
	int num;
	int len;
	char *ptr;
	int ret = 0;
	char buf[128] = {0};

	for (num = MAX_ECC_DIST; num >= 0; num--) {
		if (ecc_count->ecc.statis[num])
			break;
	}

	ptr = buf;

	ptr += snprintf(ptr, 40, "ECC distribution: \n");

	if (ecc_count->ecc.uncorrect)
		ptr += snprintf(ptr, 40, "  out of range: %d\n",
				ecc_count->ecc.uncorrect);

	len = ptr - buf;
	if (copy_to_user(usrbuf, buf, len))
		return -EFAULT;
	usrbuf += len;
	ret += len;
	count -= len;

	for (ix = 0; ix <= num; ) {
		int jx;
		ptr = buf;

		ptr += sprintf(ptr, "  %2d: ", ix);

		for (jx = 0; jx < 4 && ix <= num; jx++, ix++)
			ptr += sprintf(ptr, "0x%08x ", ecc_count->ecc.statis[ix]);

		ptr += sprintf(ptr, "\n");

		len = ptr - buf;
		if (copy_to_user(usrbuf, buf, len))
			return -EFAULT;
		usrbuf += len;
		ret += len;
		count -= len;
	}

	return ret;
}
/*****************************************************************************/

static ssize_t dbgfs_ecc_count_read(struct file *filp, char __user *buffer,
				    size_t count, loff_t *ppos)
{
	int len;
	char __user *pusrbuf = buffer;

	if (*ppos == 0) {
		if (dbg_ecc_count->count
		    < CONFIG_HINFC610_DBG_ECC_COUNT_NUM)
			dbg_ecc_count->read_index = 0;
		else
			dbg_ecc_count->read_index
				= (dbg_ecc_count->index + 1);

		len = count - (pusrbuf - buffer);
		len = dbgfs_dump_ecc_dist(dbg_ecc_count, pusrbuf, len);
		if (len < 0)
			return len;
		pusrbuf += len;

	} else if (dbg_ecc_count->read_index == dbg_ecc_count->index)
		return 0;

	len = count - (pusrbuf - buffer);
	len = dbgfs_dump_ecc(dbg_ecc_count, pusrbuf, len, (*ppos == 0));
	if (len < 0)
		return len;
	pusrbuf += len;

	*ppos += (pusrbuf - buffer);

	return pusrbuf - buffer;
}
/******************************************************************************/
/*
 * echo "offset=8192,length=102400" > ecc_count
 *
 */
static ssize_t dbgfs_ecc_count_write(struct file *filp,
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
			value = simple_strtoul(str, &str, 10);

			if (value < 0 || value >= dbg_ecc_count->nr_page)
				value = 0;

			dbg_ecc_count->offset = (value & ~7);

			break;

		case 'l':
			if (memcmp(&buf[pos], CMD_WORD_LENGTH,
				sizeof(CMD_WORD_LENGTH) - 1))
				break;

			pos += sizeof(CMD_WORD_LENGTH) - 1;
			str = (char *)(buf + pos);
			value = simple_strtoul(str, &str, 10);

			if (value < 0)
				value = dbg_ecc_count->nr_page;

			value = ((value + 7) & ~7);

			if (dbg_ecc_count->offset + value >
			    dbg_ecc_count->nr_page)
				value = dbg_ecc_count->nr_page
					- dbg_ecc_count->offset;

			dbg_ecc_count->length = value;

			break;
		}

		while (pos < count &&
			(buf[pos] != ' ' && buf[pos] != ',' && buf[pos] != ';'))
			pos++;
	}

	return count;
}
/******************************************************************************/

static struct file_operations dbgfs_ecc_count_fops = {
	.owner = THIS_MODULE,
	.read  = dbgfs_ecc_count_read,
	.write = dbgfs_ecc_count_write,
};
/******************************************************************************/

static int dbgfs_ecc_count_init(struct dentry *root, struct hinfc_host *host)
{
	unsigned int size;
	unsigned int pagesize;
	unsigned int chipsize;
	struct hinfc610_dbg_ecc_count_t *ecc_count;

	if (dbg_ecc_count)
		return 0;

	size = sizeof(struct hinfc610_dbg_ecc_count_t);
	size += (CONFIG_HINFC610_DBG_ECC_COUNT_NUM + 1) *
		(sizeof(struct hinfc610_dbg_ecc_count_item_t) +
		 host->ecc.section);

	ecc_count = vmalloc(size);
	if (!ecc_count) {
		pr_err("Can't allocate memory.\n");
		return -ENOMEM;
	}
	memset(ecc_count, 0, size);

	ecc_count->item = (char *)ecc_count +
		sizeof(struct hinfc610_dbg_ecc_count_t);
	ecc_count->ecc.dump = host->ecc.dump;
	ecc_count->ecc.section = host->ecc.section;

	pagesize  = (host->pagesize >> 10);
	chipsize = (unsigned int)(host->chip->chipsize >> 10);
	ecc_count->nr_page = (chipsize / pagesize);
	ecc_count->length = ecc_count->nr_page;

	ecc_count->dentry = debugfs_create_file("ecc_count",
		S_IFREG | S_IRUSR | S_IWUSR, 
		root, NULL, &dbgfs_ecc_count_fops);
	if (!ecc_count->dentry) {
		pr_err("Can't create 'ecc_count' file.\n");
		vfree(ecc_count);
		return -ENOENT;
	}

	dbg_ecc_count = ecc_count;

	return 0;
}
/*****************************************************************************/

static int dbgfs_ecc_count_uninit(struct hinfc_host *host)
{
	if (!dbg_ecc_count)
		return 0;

	mutex_lock(&dbg_ecc_count_mutex);

	debugfs_remove(dbg_ecc_count->dentry);

	vfree(dbg_ecc_count);
	dbg_ecc_count = NULL;

	mutex_unlock(&dbg_ecc_count_mutex);

	return 0;
}
/*****************************************************************************/

static void dbg_ecc_log(struct hinfc_host *host,
			struct hinfc610_dbg_ecc_count_item_t *item,
			struct hinfc610_dbg_ecc_count_t *ecc_count)
{
	int ix;
	unsigned int value;

	item->page_status = host->page_status;
	ecc_count->ecc.dump(host, item->ecc, NULL);

	for (ix = 0; ix < ecc_count->ecc.section; ix++) {
		value = item->ecc[ix];

		if (value > MAX_ECC_DIST) {
			if (!IS_PS_UN_ECC(host))
				printk(KERN_WARNING "ecc out of range.\n");

			if (IS_PS_BAD_BLOCK(item) || IS_PS_EMPTY_PAGE(item))
				continue;

			ecc_count->ecc.uncorrect++;
		} else {
			ecc_count->ecc.statis[value]++;
		}
	}
}
/*****************************************************************************/

static void dbg_ecc_count_read(struct hinfc_host *host)
{
	unsigned int page;
	struct hinfc610_dbg_ecc_count_item_t *item;

	mutex_lock(&dbg_ecc_count_mutex);

	if (!dbg_ecc_count)
		goto exit;

	page = GET_RW_PAGE_INDEX(host);

	if (page < dbg_ecc_count->offset ||
	    page > (dbg_ecc_count->offset + dbg_ecc_count->length))
		goto exit;

	item = GET_ITEM(dbg_ecc_count, dbg_ecc_count->index);

	dbg_ecc_count->count++;

	do_gettime(&item->hour, &item->min, &item->sec, &item->msec);

	item->page = page;

	dbg_ecc_log(host, item, dbg_ecc_count);

	if (++dbg_ecc_count->index >= CONFIG_HINFC610_DBG_ECC_COUNT_NUM)
		dbg_ecc_count->index = 0;

exit:
	mutex_unlock(&dbg_ecc_count_mutex);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_ecc_count = {
	"ecc_count", 0,
	dbgfs_ecc_count_init,
	dbgfs_ecc_count_uninit,
	dbg_ecc_count_read,
	NULL,
	NULL,
	NULL,
};
