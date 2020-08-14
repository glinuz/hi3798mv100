/*******************************************************************************
 * All rights reserved, Copyright (C) huawei LIMITED 2012
 *------------------------------------------------------------------------------
 * File Name   : tlogger.c
 * Description : A TEE Logging Subsystem, read the tee os log from rdr memory
 * Platform    :
 * Author      : z00202529
 * Version     : V1.0
 * Date        : 2014.11.10
 * Notes       :
 *
 *------------------------------------------------------------------------------
 * Modifications:
 *   Date        Author          Modifications
 2014.11.10  z00202529       create
 ******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
 ******************************************************************************/
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/vmalloc.h>
#include <linux/aio.h>
#include <asm/ioctls.h>
#include <linux/hisi/rdr_pub.h>
#include <linux/syscalls.h>
#include "tee_rdr_register.h"
#include <securec.h>
#include "tc_ns_log.h"

static char *m_log_buffer;
static size_t m_addr_start;
static size_t m_addr_end;
#define PER_LEN 1024

/*#define TC_DEBUG*/
#define LOGGER_LOG_TEEOS               "hisi_teelog"	/* tee os log  */

#define __TEELOGGERIO	0xBE	/* for ioctl */
/* get the TEE log level */
#define TEELOGGER_GET_LOG_LEVEL		_IO(__TEELOGGERIO, 1)
/* set the TEE log leve */
#define TEELOGGER_SET_LOG_LEVEL		_IO(__TEELOGGERIO, 2)
/* get the TEE log size */
#define TEELOGGER_GET_LOG_SIZE		_IO(__TEELOGGERIO, 3)
/* get the TEE memory status */
#define TEELOGGER_GET_MEMORY_STATUS		_IO(__TEELOGGERIO, 4)

#define LOG_PATH_HISI_LOGS	"/data/hisi_logs/"
#define LOG_PATH_RUNNING_TRACE	"/data/hisi_logs/running_trace/"
#define LOG_PATH_TEE_LOG	"/data/hisi_logs/running_trace/teeos_logs/"
#define LOG_PATH_TEE_LOG_FILE	"/data/hisi_logs/running_trace/teeos_logs/last_teemsg"

#define ROOT_UID                0
#define SYSTEM_GID              1000

/**
 * struct logger_log - represents a specific log, such as 'main' or 'radio'
 * @buffer:	The actual ring buffer
 * @misc:	The "misc" device representing the log
 * @wq:		The wait queue for @readers
 * @readers:	This log's readers
 * @mutex:	The mutex that protects the @buffer
 * @w_off:	The current write head offset
 * @head:	The head, or location that readers start reading at.
 * @size:	The size of the log
 * @logs:	The list of log channels
 *
 * This structure lives from module insertion until module removal, so it does
 * not need additional reference counting. The structure is protected by the
 * mutex 'mutex'.
 */
struct logger_log {
	unsigned char *buffer;
	struct miscdevice misc;
	wait_queue_head_t wq;
	struct list_head readers;
	struct mutex mutex;
	size_t w_off;
	size_t head;
	size_t size;
	struct list_head logs;
};

static LIST_HEAD(m_log_list);

/**
 * struct logger_reader - a logging device open for reading
 * @log:	The associated log
 * @list:	The associated entry in @logger_log's list
 * @r_off:	The current read head offset.
 * @r_all:	Reader can read all entries
 * @r_ver:	notice:read the whole ring-buffer times, not version
 *
 * This object lives from open to release, so we don't need additional
 * reference counting. The structure is protected by log->mutex.
 */
struct logger_reader {
	struct logger_log *log;
	struct list_head list;
	size_t r_off;
	bool r_all;
	int r_ver;
};

/*
 * file_get_log - Given a file structure, return the associated log
 *
 * This isn't aesthetic. We have several goals:
 *
 *	1) Need to quickly obtain the associated log during an I/O operation
 *	2) Readers need to maintain state (logger_reader)
 *	3) Writers need to be very fast (open() should be a near no-op)
 *
 * In the reader case, we can trivially go file->logger_reader->logger_log.
 * For a writer, we don't want to maintain a logger_reader, so we just go
 * file->logger_log. Thus what file->private_data points at depends on whether
 * or not the file was opened for reading. This function hides that dirtiness.
 */
static inline struct logger_log *file_get_log(struct file *file)
{
	if (file->f_mode & FMODE_READ) {
		struct logger_reader *reader = file->private_data;

		return reader->log;
	} else
		return file->private_data;
}

static struct logger_log *g_log;
/*
 * logger_read - our log's read() method
 *
 * Behavior:
 *
 *	- O_NONBLOCK works
 *	- If there are no log entries to read, blocks until log is written to
 *	- Atomically reads exactly one log entry
 *
 * Will set errno to EINVAL if read
 * buffer is insufficient to hold next entry.
 */
#define CHECK_BOUNDARY(X)	((((X) < m_addr_start) || ((X) > m_addr_end)) ? true : false)

static ssize_t tlogger_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	struct logger_reader *reader = file->private_data;
	struct logger_log *log = reader->log;
	ssize_t ret;
	size_t log_len;
	size_t sub;
	bool disjoin_flag;
	u32 real_addr = 0;
	u32 ring_flag = 0;
	DEFINE_WAIT(wait);

	tlogd("logger_read  start  ++\n");
	if (!access_ok(VERIFY_WRITE, buf, count))/*lint !e530 !e529*/
		return -EFAULT;

start:
	while (1) {
		mutex_lock(&log->mutex);
		prepare_to_wait(&log->wq, &wait, TASK_INTERRUPTIBLE);
		if(memcpy_s(&real_addr, sizeof(real_addr), m_log_buffer, 4)) {
			ret = -EAGAIN;
			break;
		}
		if(memcpy_s(&ring_flag, sizeof(ring_flag), m_log_buffer + 4, 4)) {
			ret = -EAGAIN;
			break;
		}
		tlogd(" real_addr=0x%x ring_flag=%d\n", real_addr, ring_flag);
		log->w_off = real_addr + m_addr_start;

		ret = ((log->w_off == reader->r_off)
		       && (ring_flag == reader->r_ver));

		mutex_unlock(&log->mutex);
		if (!ret)
			break;

		if (file->f_flags & O_NONBLOCK) {
			tloge("nonblock branch\n");
			ret = -EAGAIN;
			break;
		}

		if (signal_pending(current)) {
			tloge("signal branch\n");
			ret = -EINTR;
			break;
		}
		schedule();
	}

	finish_wait(&log->wq, &wait);
	if (ret)
		return ret;

	mutex_lock(&log->mutex);

	/* is there still something to read or did we race? */
	if (unlikely(log->w_off == reader->r_off)) {
		mutex_unlock(&log->mutex);
		goto start;
	}

	if (ring_flag != reader->r_ver /*ring_flag not same */) {
		if ((reader->r_off > log->w_off)
		    && ((ring_flag - reader->r_ver) == 1)) {
			sub = (m_addr_end - reader->r_off) + (log->w_off - m_addr_start);
		} else {
			reader->r_off = log->w_off;
			sub = (m_addr_end - reader->r_off) + (log->w_off - m_addr_start);
		}
		disjoin_flag = true;
	} else {
		disjoin_flag = false;
		if (log->w_off < reader->r_off) {
			reader->r_off = m_addr_start;
			tloge("ERR tlogcat read=0x%lx write=0x%lx\n", reader->r_off, log->w_off);
			ret = -1;
			goto out;
		}
		sub = log->w_off - reader->r_off;
		tlogd("sub=%zx  read->r_ver=%d\n", sub, reader->r_ver);
	}
	log_len = (sub < PER_LEN) ? sub : PER_LEN;

	if (disjoin_flag && ((reader->r_off + log_len) > m_addr_end)) {
		if (CHECK_BOUNDARY(reader->r_off)) {
			tloge("1 CHECK_BOUNDARY error reader->r_off=0x%lu\n", reader->r_off);
		}
		ret = copy_to_user(buf, (void *)(reader->r_off), (m_addr_end - reader->r_off));
		tlogd("read (m_addr_end-reader->r_off)=%zx ret = %zx\n",
			(m_addr_end - reader->r_off), ret);

		ret = copy_to_user(buf, (void *)(m_addr_start),
				     (log_len - (m_addr_end - reader->r_off)));

		reader->r_ver = ring_flag;
	} else {
		if (CHECK_BOUNDARY(reader->r_off)
		    || CHECK_BOUNDARY(reader->r_off + log_len)) {
			tloge("2 CHECK_BOUNDARY error\n");
		}

		if (log_len)
			ret = copy_to_user(buf, (void *)(reader->r_off), log_len);
		tlogd("read  log_len=%zx\n", log_len);
	}

	tlogd("ret = %zx\n", ret);

	/* update reader->r_off */
	if (disjoin_flag && ((reader->r_off + log_len) > m_addr_end))
		reader->r_off = m_addr_start + (log_len - (m_addr_end - reader->r_off));
	else
		reader->r_off += log_len;

	/* return log_len */
	ret = log_len;
out:
	mutex_unlock(&log->mutex);

	return ret;
}

/*its role is same to logger_aio_write*/
void tz_log_write(void)
{
	tlogd("tz_log_write wake up start \n");
	if (g_log != NULL)
		wake_up_interruptible(&g_log->wq);
}

static struct logger_log *get_log_from_minor(int minor)
{
	struct logger_log *log;

	list_for_each_entry(log, &m_log_list, logs)
	if (log->misc.minor == minor)
		return log;
	return NULL;
}

/*
 * logger_open - the log's open() file operation
 *
 * Note how near a no-op this is in the write-only case. Keep it that way!
 */
static int tlogger_open(struct inode *inode, struct file *file)
{
	struct logger_log *log;
	int ret;
	int real_addr = 0;
	int ring_flag = 0;

	tlogd("open logger_open ++\n");
	/*not support seek */
	ret = nonseekable_open(inode, file);
	if (ret)
		return ret;
	tlogd("Before get_log_from_minor\n");
	log = get_log_from_minor(MINOR(inode->i_rdev));
	if (!log)
		return -ENODEV;

	if (file->f_mode & FMODE_READ) {
		struct logger_reader *reader;

		reader = kmalloc(sizeof(struct logger_reader), GFP_KERNEL);
		if (!reader)
			return -ENOMEM;

		reader->log = log;
		ret = memcpy_s(&real_addr, sizeof(real_addr), m_log_buffer, 4);
		if (ret) {
			kfree(reader);
			return ret;
		}
		ret = memcpy_s(&ring_flag, sizeof(ring_flag), m_log_buffer + 4, 4);
		if (ret) {
			kfree(reader);
			return ret;
		}		tlogd(" real_addr=0x%x ring_flag=%d\n", real_addr, ring_flag);
		reader->r_ver = ring_flag;
		reader->r_all = true;

		INIT_LIST_HEAD(&reader->list);

		mutex_lock(&log->mutex);
		if (reader->r_ver) {
			reader->r_off = log->w_off + 1;
			if (log->w_off == (m_addr_end - 1))
				reader->r_off = m_addr_end;
			reader->r_ver--;
		} else
			reader->r_off = log->head;

		list_add_tail(&reader->list, &log->readers);
		mutex_unlock(&log->mutex);

		file->private_data = reader;
	} else
		file->private_data = log;
	return 0;
}

/*
 * logger_release - the log's release file operation
 *
 * Note this is a total no-op in the write-only case. Keep it that way!
 */
static int tlogger_release(struct inode *ignored, struct file *file)
{
	tlogd("logger_release ++\n");
	if (file->f_mode & FMODE_READ) {
		struct logger_reader *reader = file->private_data;
		struct logger_log *log = reader->log;

		mutex_lock(&log->mutex);
		list_del(&reader->list);
		mutex_unlock(&log->mutex);
		kfree(reader);
	}
	return 0;
}

/*
 * logger_poll - the log's poll file operation, for poll/select/epoll
 *
 * Note we always return POLLOUT, because you can always write() to the log.
 * Note also that, strictly speaking, a return value of POLLIN does not
 * guarantee that the log is readable without blocking, as there is a small
 * chance that the writer can lap the reader in the interim between poll()
 * returning and the read() request.
 */
static unsigned int tlogger_poll(struct file *file, poll_table *wait)
{
	struct logger_reader *reader;
	struct logger_log *log;
	unsigned int ret = POLLOUT | POLLWRNORM;
	int ring_flag = 0;
	unsigned int real_offset = 0;
	unsigned long size_num = 4;
	if(memcpy_s(&real_offset, sizeof(real_offset), m_log_buffer, size_num)){
		ret |= POLLERR; 
		return ret;
	}
	if(memcpy_s(&ring_flag, sizeof(ring_flag), m_log_buffer + 4, size_num)){
		ret |= POLLERR; 
		return ret;
	}

	tlogd("logger_poll ++\n");

	if (!(file->f_mode & FMODE_READ))
		return ret;

	reader = file->private_data;
	log = reader->log;

	poll_wait(file, &log->wq, wait);

	mutex_lock(&log->mutex);

	tlogd("poll before  log->w_off=%zx reader->r_off=%zx\n", log->w_off, reader->r_off);
	log->w_off = (size_t)(m_log_buffer + real_offset + 64);
	tlogd("poll after     log->w_off=%zx reader->r_off=%zx\n", log->w_off, reader->r_off);
	if ((log->w_off != reader->r_off) || (ring_flag != reader->r_ver))
		ret |= POLLIN | POLLRDNORM;
	mutex_unlock(&log->mutex);

	return ret;
}

static int tee_get_loglevel(void)
{
	int real_addr = 0;
	int ring_flag = 0;
	int log_level = 0;

	if(memcpy_s(&real_addr, sizeof(real_addr), m_log_buffer, 4)) {
		return -EAGAIN;
	}
	if(memcpy_s(&ring_flag, sizeof(ring_flag), m_log_buffer + 4, 4)) {
		return -EAGAIN;
	}
	if(memcpy_s(&log_level, sizeof(log_level), m_log_buffer + 8, 4)) {
		return -EAGAIN;
	}
	tlogd(" real_addr=0x%x ring_flag=%d log_level=%d\n", real_addr, ring_flag, log_level);
	return log_level;
}

static int tee_set_loglevel(unsigned long arg)
{
	int real_addr = 0;
	int ring_flag = 0;
	int log_level = 0;

	if(memcpy_s(&real_addr, sizeof(real_addr), m_log_buffer, 4)) {
		return -EAGAIN;
	}

	if(memcpy_s(&ring_flag, sizeof(ring_flag), m_log_buffer + 4, 4)) {
		return -EAGAIN;
	}
	if(memcpy_s(&log_level, sizeof(log_level), m_log_buffer + 8, 4)) {
		return -EAGAIN;
	}
	tlogd(" real_addr=0x%x ring_flag=%d log_level=%d\n", real_addr, ring_flag, log_level);
	log_level = arg;
	if(memcpy_s(m_log_buffer + 8, 4, &log_level, 4)) {
		return -EAGAIN;
	}
	return log_level;
}

static long tlogger_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	struct logger_log *log = file_get_log(file);
	long ret = -EINVAL;

	tlogd("logger_ioctl start ++\n");
	mutex_lock(&log->mutex);

	switch (cmd) {
	case TEELOGGER_GET_LOG_LEVEL:
		tlogd("excute tee_get_loglevel\n");
		ret = tee_get_loglevel();
		break;
	case TEELOGGER_SET_LOG_LEVEL:
		tlogd("excute tee_set_loglevel\n");
		ret = tee_set_loglevel(arg);
		break;
	case TEELOGGER_GET_LOG_SIZE:
		ret = log->size;
		break;
	case TEELOGGER_GET_MEMORY_STATUS:
		ret = log->size;
		break;
	default:
		tloge("ioctl error default\n");
		break;
	}

	mutex_unlock(&log->mutex);

	return ret;
}

#ifdef CONFIG_COMPAT
static long tlogger_compat_ioctl(struct file *file, unsigned int cmd,
				 unsigned long arg)
{
	long ret = -ENOIOCTLCMD;

	tlogd("logger_compat_ioctl ++\n");
	arg = (unsigned long)compat_ptr(arg);
	ret = tlogger_ioctl(file, cmd, arg);
	return ret;
}
#endif

static const struct file_operations logger_fops = {
	.owner = THIS_MODULE,
	.read = tlogger_read,
	/*.write = logger_aio_write,*/
	.poll = tlogger_poll,
	.unlocked_ioctl = tlogger_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = tlogger_compat_ioctl,
#endif
	.open = tlogger_open,
	.release = tlogger_release,
};

#define OFFSET1 64
#define OFFSET2  (5*1024)
#define RESERVE_SIZE (20*1024)
static unsigned int m_rar_mem_len;
static unsigned long m_addr_p;
static unsigned int m_rdr_log_size;
static int rdr_mem_init(void)
{
	int ret = 0;
	unsigned long rdr_mem_addr;

	rdr_mem_addr = TC_NS_get_rdr_mem_addr();
	m_rar_mem_len = TC_NS_get_rdr_mem_len();
	if (rdr_mem_addr != 0 && m_rar_mem_len != 0) {
		m_addr_p = rdr_mem_addr + OFFSET2;
		m_rdr_log_size = m_rar_mem_len - RESERVE_SIZE - OFFSET1 - OFFSET2;
	} else
		ret = -1;
	return ret;
}

static int __init create_log(char *log_name, int size)
{
	int ret = 0;
	struct logger_log *log;
	unsigned char *buffer;

	buffer = m_log_buffer;

	if (buffer == NULL)
		return -ENOMEM;

	if(log_name == NULL)
		return -EINVAL;

	log = kzalloc(sizeof(struct logger_log), GFP_KERNEL);
	if (log == NULL) {
		ret = -ENOMEM;
		goto out_free_buffer;
	}
	log->buffer = buffer;
	log->misc.minor = MISC_DYNAMIC_MINOR;
	log->misc.name = kstrdup(log_name, GFP_KERNEL);
	if (log->misc.name == NULL) {
		ret = -ENOMEM;
		goto out_free_log;
	}
	log->misc.fops = &logger_fops;
	log->misc.parent = NULL;

	init_waitqueue_head(&log->wq);
	INIT_LIST_HEAD(&log->readers);
	mutex_init(&log->mutex);
	log->w_off = (size_t)(m_log_buffer + OFFSET1);
	log->head = (size_t)(m_log_buffer + OFFSET1);
	log->size = size;
	tlogd("tlocat  log->w_off=%zx log->head=%zx\n",
		log->w_off, log->head);
	INIT_LIST_HEAD(&log->logs);
	list_add_tail(&log->logs, &m_log_list);
	/* finally, initialize the misc device for this log */
	ret = misc_register(&log->misc);
	if (unlikely(ret)) {
		tloge("failed to register misc device for log '%s'!\n", log->misc.name);
		goto out_free_log;
	}
	tlogd("created %luK log '%s'\n", (unsigned long)log->size >> 10, log->misc.name);
	g_log = log;
	return 0;

out_free_log:
	if (log->misc.name) {
		kfree(log->misc.name);
	}
	kfree(log);

out_free_buffer:
	/*vfree(buffer); */
	return ret;
}

static int tee_create_dir(char *path)
{
	int fd = -1;
	int ret = 0;

	fd = sys_access(path, 0);
	if (0 != fd) {
		tlogd("need create dir %s.\n", path);
		fd = sys_mkdir(path, 0770);
		if (fd < 0) {
			tloge("create dir %s fail, ret: %d.\n", path, fd);
			return fd;
		}
		tlogd("create dir %s successed, fd: %d.\n", path, fd);
		ret = (int)sys_chown((const char __user *)path, ROOT_UID, SYSTEM_GID);
		if (ret)
			tloge("Failed to chown %s\n", path);
	}

	return 0;
}

int tlogger_store_lastmsg(void)
{
	struct file *filep;
	ssize_t write_len;
	char *tmp_log_buffer = phys_to_virt(m_addr_p);
	mm_segment_t old_fs;
	loff_t pos = 0;
	int ret;

	if (!virt_addr_valid(tmp_log_buffer)) {
		tloge("tmp_log_buffer is not valid addr\n");
		return -EFAULT;
	}

	ret = tee_create_dir(LOG_PATH_HISI_LOGS);
	if (0 != ret) {
		tloge("failed to creat log dir\n");
		return -1;
	}

	ret = tee_create_dir(LOG_PATH_RUNNING_TRACE);
	if (0 != ret) {
		tloge("failed to creat log dir\n");
		return -1;
	}

	ret = tee_create_dir(LOG_PATH_TEE_LOG);
	if (0 != ret) {
		tloge("failed to creat log dir\n");
		return -1;
	}

	/*exception handling, store trustedcore exception info to file */
	filep = filp_open(LOG_PATH_TEE_LOG_FILE, O_CREAT | O_RDWR, 0640);
	if (IS_ERR(filep)) {
		tlogd("Failed to filp_open last_teemsg\n");
		return -1;
	}

	tlogd("Succeed to filp_open last_teemsg\n");

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	ret = (int)sys_chown((const char __user *)LOG_PATH_TEE_LOG_FILE, ROOT_UID, SYSTEM_GID);
	if (ret)
		tloge("Failed to chown last_teemsg\n");

	write_len = vfs_write(filep, (char __user *)tmp_log_buffer, m_rar_mem_len - OFFSET2, &pos);
	if (write_len < 0) {
		tloge("Failed to write to last_teemsg\n");
	} else {
		tloge("Succeed to Write to last_teemsg, write_len=%ld\n",
			write_len);
	}
	pos = 0;

	set_fs(old_fs);
	filp_close(filep, 0);
	return 0;
}

static int __init tlogger_init(void)
{
	int ret;

	ret = rdr_mem_init();
	if (ret < 0) {
		tloge("rdr mem init failed!!!\n");
		return -1;
	}
	m_log_buffer = (char *)hisi_bbox_map(m_addr_p, m_rar_mem_len - OFFSET2);
	if(!m_log_buffer){
		return -ENOMEM;
	}
	m_addr_start = (size_t)(m_log_buffer + 64);
	m_addr_end = (size_t)(m_log_buffer + 64 + m_rdr_log_size);
	tloge("0728 tlogcat:start=0x%lx\tend=0x%lx len=0x%x\n", m_addr_start, m_addr_end, m_rar_mem_len);
	ret = create_log(LOGGER_LOG_TEEOS, m_rdr_log_size);
	if(ret) {
		hisi_bbox_unmap(m_log_buffer);
	}
	return ret;
}

static void __exit tlogger_exit(void)
{
	struct logger_log *current_log, *next_log;

	list_for_each_entry_safe(current_log, next_log, &m_log_list, logs) {
		/* we have to delete all the entry inside m_log_list */
		misc_deregister(&current_log->misc);
		/*vfree(current_log->buffer); */
		kfree(current_log->misc.name);
		list_del(&current_log->logs);
		kfree(current_log);
	}
	hisi_bbox_unmap(m_log_buffer);
}

device_initcall(tlogger_init);
module_exit(tlogger_exit);

MODULE_AUTHOR("z00202529");
MODULE_DESCRIPTION("TrustCore Logger");
MODULE_VERSION("1.00");
