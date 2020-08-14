/******************************************************************************
 *    COPYRIGHT (C) 2013 Czyong. Hisilicon
 *    All rights reserved.
 * ***
 *    Create by Czyong 2013-07-04
 *
******************************************************************************/
#include <linux/vmalloc.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

#include "hinfc610_os.h"
#include "hinfc610.h"
#include "hinfc610_dbg.h"

#ifndef CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS
#  define CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS      (80)
#endif /* CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS */

#ifndef CONFIG_HINFC610_DBG_NAND_LOG_LENGTH
#  define CONFIG_HINFC610_DBG_NAND_LOG_LENGTH       (40)
#endif /* CONFIG_HINFC610_DBG_NAND_LOG_LENGTH */

struct hinfc610_dbg_dump_item_t {
	unsigned short hour;
	unsigned short min;
	unsigned short sec;
	unsigned short msec;

	unsigned int cycle;

	unsigned long  page;
	unsigned long  offset;
	unsigned long  length;

	char page_status[4];
	char op;

	unsigned char data[CONFIG_HINFC610_DBG_NAND_LOG_LENGTH];
};

struct hinfc610_dbg_dump_t {

	struct dentry *dentry;
	unsigned int index; /* current logs index */
	int count;          /* number of logs */

	unsigned long offset;
	unsigned long length;

	struct hinfc610_dbg_dump_item_t logs[CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS];

	unsigned int read_index;
};

static DEFINE_MUTEX(dbg_dump_mutex);
static struct hinfc610_dbg_dump_t *dbg_dump = NULL;

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
/*
 *

# cat ./debugfs/nand/dump
Print parameter: "offset=0 length=8"
UTC Clock   op cylce  page-offset     data
00:00:33.0321  W  5   0x0000258F-0000   31 18 10 06 18 EF FE 11 
00:00:33.0325  W  5   0x00002740-0000   31 18 10 06 7C D4 B3 0C 

*
*/
static ssize_t dbgfs_dump_read(struct file *filp, char __user *buffer,
			       size_t count, loff_t *ppos)
{
	int len = 0;
	char buf[128] = {0};
	unsigned int read_index;
	char __user *pusrbuf = buffer;
	struct hinfc610_dbg_dump_item_t *logs;

	if (*ppos == 0) {
		if (dbg_dump->count
		    < CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS)
			dbg_dump->read_index = 0;
		else
			dbg_dump->read_index
				= (dbg_dump->index + 1);

		len = snprintf(buf, sizeof(buf),
			       "Print parameter: \"offset=%ld length=%ld\" (offset is byte number)\n",
			       dbg_dump->offset, dbg_dump->length);

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;

		pusrbuf += len;

		len = snprintf(buf, sizeof(buf),
			       "  UTC Clock   op cylce  page-offset     data\n");

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;

		pusrbuf += len;

	} else if (dbg_dump->read_index == dbg_dump->index)
		return 0;

	for (read_index = dbg_dump->read_index;
	     (read_index != dbg_dump->index);
	     ++read_index) {

		if (read_index >= CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS)
			read_index = 0;

		logs = &dbg_dump->logs[read_index];

		if ((count - (pusrbuf - buffer)) < (50 + logs->length * 3))
			break;

		len = snprintf(buf, sizeof(buf),
			       "%02d:%02d:%02d.%04d  %c  %-2u  0x%08lX-%04lX",
			       logs->hour, logs->min, logs->sec, logs->msec,
			       logs->op, logs->cycle,
			       logs->page, logs->offset);

		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;

		pusrbuf += len;

		if (logs->op == 'E') {

			len = snprintf(buf, sizeof(buf), "   ---");
			if (copy_to_user(pusrbuf, buf, len))
				return -EFAULT;
			pusrbuf += len;

		} else {

			int ix;

			len = snprintf(buf, sizeof(buf), "%s",
				       logs->page_status);
			if (copy_to_user(pusrbuf, buf, len))
				return -EFAULT;
			pusrbuf += len;

			for (ix = 0; ix < logs->length; ix++) {
				if ((ix % 16) == 15) {
					len = snprintf(buf, sizeof(buf),
						       "%02X-",
						       logs->data[ix]);
					if (copy_to_user(pusrbuf, buf, len))
						return -EFAULT;
					pusrbuf += len;
				} else {
					len = snprintf(buf, sizeof(buf),
						       "%02X ",
						       logs->data[ix]);
					if (copy_to_user(pusrbuf, buf, len))
						return -EFAULT;
					pusrbuf += len;
				}
			}
		}
		len = snprintf(buf, sizeof(buf), "\n");
		if (copy_to_user(pusrbuf, buf, len))
			return -EFAULT;
		pusrbuf += len;
	}

	dbg_dump->read_index = read_index;

	*ppos += (pusrbuf - buffer);
	return pusrbuf - buffer;
}
/*****************************************************************************/

static ssize_t dbgfs_dump_write(struct file *filp, const char __user *buffer,
				size_t count, loff_t *ppos)
{
	char *p;
	unsigned long value;
	char buf[128] = {0};
	unsigned long pos = 0;

	if (count > sizeof(buf))
		count = sizeof(buf);

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	while (pos < count) {

		while (pos < count
		       && (buf[pos] == ' '||
			   buf[pos] == ',' || buf[pos] == ';'))
			pos++;

		if (pos >= count)
			break;

		switch (buf[pos]) {
		case 'o':
			if (!memcmp(&buf[pos], CMD_WORD_OFFSET,
				    sizeof(CMD_WORD_OFFSET) - 1)) {

				pos += sizeof(CMD_WORD_OFFSET) - 1;
				p = (char *)(buf + pos);
				if ((value = simple_strtoul(p, &p, 10)) < 0)
					value = 0;
				dbg_dump->offset = value;
			}
			break;

		case 'l':
			if (!memcmp(&buf[pos], CMD_WORD_LENGTH,
				    sizeof(CMD_WORD_LENGTH) - 1)) {

				pos += sizeof(CMD_WORD_LENGTH) - 1;
				p = (char *)(buf + pos);
				if ((value = simple_strtoul(p, &p, 10)) < 0)
					value = 0;
				dbg_dump->length = value;
			}
			break;
		}

		while (pos < count &&
		       (buf[pos] != ' ' && buf[pos] != ',' && buf[pos] != ';'))
			pos++;
	}

	*ppos += count;
	return count;
}
/*****************************************************************************/

static struct file_operations dbgfs_dump_fops = {
	.owner = THIS_MODULE,
	.read  = dbgfs_dump_read,
	.write = dbgfs_dump_write,
};
/*****************************************************************************/

static int dbgfs_dump_init(struct dentry *root, struct hinfc_host *host)
{
	struct hinfc610_dbg_dump_t *dump;

	if (dbg_dump)
		return 0;

	dump = vmalloc(sizeof(struct hinfc610_dbg_dump_t));
	if (!dump) {
		pr_err("Can't allocate memory.\n");
		return -ENOMEM;
	}
	memset(dump, 0, sizeof(struct hinfc610_dbg_dump_t));

	dump->dentry = debugfs_create_file("dump",
		S_IFREG | S_IRUSR | S_IWUSR, 
		root, NULL, &dbgfs_dump_fops);
	if (!dump->dentry) {
		pr_err("Can't create 'dump' file.\n");
		vfree(dump);
		return -ENOENT;
	}

	dump->length = 8;

	dbg_dump = dump;

	return 0;
}
/*****************************************************************************/

static int dbgfs_dump_uninit(struct hinfc_host *host)
{
	if (!dbg_dump)
		return 0;

	mutex_lock(&dbg_dump_mutex);

	debugfs_remove(dbg_dump->dentry);

	vfree(dbg_dump);
	dbg_dump = NULL;

	mutex_unlock(&dbg_dump_mutex);

	return 0;
}
/*****************************************************************************/

static void dbg_dump_rw(struct hinfc_host *host, char op)
{
	int offset;
	unsigned long buflen;
	struct hinfc610_dbg_dump_item_t *logs;

	mutex_lock(&dbg_dump_mutex);

	if (!dbg_dump)
		goto exit;

	buflen = (host->pagesize + host->oobsize);
	logs = &dbg_dump->logs[dbg_dump->index];

	dbg_dump->count++;

	do_gettime(&logs->hour, &logs->min, &logs->sec, &logs->msec);

	memcpy(logs->page_status, "\x20\x20\x20\x00", 4);

	if (host->page_status) {
		if (IS_PS_BAD_BLOCK(host))
			logs->page_status[0] = 'B';
		else if (IS_PS_EMPTY_PAGE(host))
			logs->page_status[0] = 'E';

		if (IS_PS_UN_ECC(host))
			logs->page_status[1] = '*';

		if (IS_PS_EPM_ERR(host) || IS_PS_BBM_ERR(host))
			logs->page_status[2] = '?';
	}

	logs->op = op;
	logs->cycle = host->addr_cycle;
	logs->length = dbg_dump->length;
	logs->offset = (host->addr_value[0] & 0xFFFF);
	logs->page   = GET_RW_PAGE_INDEX(host);

	if (!logs->offset)
		logs->offset = dbg_dump->offset;

	if (logs->offset >= buflen)
		logs->offset = 0;

	if (logs->length > (buflen - logs->offset))
		logs->length = (buflen - logs->offset);

	if (logs->length > CONFIG_HINFC610_DBG_NAND_LOG_LENGTH)
		logs->length = CONFIG_HINFC610_DBG_NAND_LOG_LENGTH;

	offset = logs->offset;
	if (offset >= host->pagesize)
		memcpy(logs->data, host->oobbuf + offset - host->pagesize, logs->length);
	else if (offset + logs->length <= host->pagesize)
		memcpy(logs->data, host->pagebuf + offset, logs->length);
	else {
		int nr_copy = host->pagesize - offset;

		memcpy(logs->data, host->pagebuf + offset, nr_copy);
		memcpy(logs->data + nr_copy, host->oobbuf, logs->length - nr_copy);
	}

	if (++dbg_dump->index >= CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS)
		dbg_dump->index = 0;

exit:
	mutex_unlock(&dbg_dump_mutex);
}
/*****************************************************************************/

static void dbg_dump_read(struct hinfc_host *host)
{
	dbg_dump_rw(host, 'R');
}
/*****************************************************************************/

static void dbg_dump_write(struct hinfc_host *host)
{
	dbg_dump_rw(host, 'W');
}
/*****************************************************************************/

static void dbg_dump_erase(struct hinfc_host *host)
{
	struct hinfc610_dbg_dump_item_t *logs;

	mutex_lock(&dbg_dump_mutex);

	if (!dbg_dump)
		goto exit;

	dbg_dump->count++;
	logs = &dbg_dump->logs[dbg_dump->index];

	do_gettime(&logs->hour, &logs->min, &logs->sec, &logs->msec);

	memcpy(logs->page_status, "\x20\x20\x20\x00", 4);

	logs->op = 'E';
	logs->cycle  = host->addr_cycle;
	logs->length = dbg_dump->length;

	logs->offset = 0;
	logs->page   = host->addr_value[0];
	logs->length = 0;

	if (++dbg_dump->index >= CONFIG_HINFC610_DBG_NAND_NUM_OF_LOGS)
		dbg_dump->index = 0;

exit:
	mutex_unlock(&dbg_dump_mutex);
}
/*****************************************************************************/

struct hinfc610_dbg_inf_t hinfc610_dbg_inf_dump = {
	"dump", 0,
	dbgfs_dump_init,
	dbgfs_dump_uninit,
	dbg_dump_read,
	dbg_dump_write,
	dbg_dump_erase,
	NULL,
};
/*****************************************************************************/
