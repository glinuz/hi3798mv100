#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> /*for errno*/
#include <sys/types.h> /*for open close*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h> /*for ioctl*/
#include <sys/mman.h> /*for mmap*/
#include <signal.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/statfs.h>
#ifdef _ANDROID_PLATFORM_HISI
/*DTS2014040405270 chown sec_stroage_data q00209673 */
#include <private/android_filesystem_config.h>
#endif
/* debug switch */
/* #define LOG_NDEBUG 0 */
#ifdef LOG_TAG
#undef	LOG_TAG
#define LOG_TAG "teecd"
#endif

#ifdef _ANDROID_PLATFORM_HISI
#include <cutils/log.h>
#include <cutils/properties.h>
#else
#ifdef WITH_DEVCHIP_PLATFORM
#define PAGE_SIZE 4096
#define AID_SYSTEM        1000
#endif
#endif

#include <securec.h>
#include "tc_ns_client.h"
#include "tee_client_storage_api.h"
#include "tee_ca_daemon.h"
#include "tee_client_api.h"
#include "tee_log.h"
#ifdef TEE_SUPPORT_SOCKET
#include "socket/agent_socket_listener.h"
#endif
/*lint -e457 -e458 -e754 -e774 -e838*/

#ifdef K3V3_FPGA
#define LOGE ALOGE
#define LOGV ALOGV
#endif

#define FILE_NAME_MAX_BUF		256
#define WIDEVINE_NV_WVLOCK_SIZE		68

#ifdef _ANDROID_PLATFORM_HISI
#if LOG_NDEBUG
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define LOGV(...)
#endif
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else//else _ANDROID_PLATFORM
#if LOG_NDEBUG
#define LOGV(fmt, args...) printf("[debug]%s %s: " fmt "", LOG_TAG, __FUNCTION__, ## args)
#else
#define LOGV(...)
#endif
#define LOGE(fmt, args...) printf("[error]%s %s: " fmt "", LOG_TAG, __FUNCTION__, ## args)
#endif

#ifndef _ANDROID_PLATFORM_HISI
//added by fangjian00208632
typedef void (*sighandler_t)(int);
#endif

/* static func declare */

typedef enum {
	sec_open,
	sec_close,
	sec_read,
	sec_write,
	sec_seek,
	sec_remove,
	sec_truncate,
	sec_rename,
	sec_create,
	sec_info,
	sec_access,
	sec_access2,
	sec_fsync,
	sec_cp,
	sec_diskusage
} fs_cmd_t;

typedef enum {
	sec_nv_info,/* add by q00209673 for bootloaderlock status in nv partition */
	sec_get_time,
} misc_cmd_t;

struct sec_storage_t {
	fs_cmd_t cmd;   /* for s to n */
	int32_t ret;    /* fxxx call's return */
	int32_t ret2;   /* just for fread, judge end-of-file or error */
	uint32_t userid;
	uint32_t magic;
	uint32_t error;
	union args1 {
		struct {
			char mode[4];
			uint32_t name_len;
			uint32_t name[1];   /* change name[0] --> name[1], for pc-lint */
		} open;
		struct {
			int32_t fd;
		} close;
		struct {
			int32_t fd;
			uint32_t count;
			uint32_t buffer[1]; /* the same as name[0] --> name[1] */
		} read;
		struct {
			int32_t fd;
			uint32_t count;
			uint32_t buffer[1];
		} write;
		struct {
			int32_t fd;
			int32_t offset;
			uint32_t whence;
		} seek;
		struct {
			uint32_t name_len;
			uint32_t name[1];
		} remove;
		struct {
			uint32_t len;
			uint32_t name_len;
			uint32_t name[1];
		} truncate;
		struct {
			uint32_t old_name_len;
			uint32_t new_name_len;
			uint32_t buffer[1];   /* old_name + new_name */
		} rename;
		struct {
			uint32_t from_path_len;
			uint32_t to_path_len;
			uint32_t buffer[1];   /* from_path+to_path */
		} cp;
		struct {
			char mode[4];
			uint32_t name_len;
			uint32_t name[1];
		} create;
		struct {
			int32_t fd;
			uint32_t cur_pos;
			uint32_t file_len;
		} info;
		struct {
			int mode;
			uint32_t name_len;
			uint32_t name[1];
		} access;
		struct {
			int32_t fd;
		} fsync;
		struct {
			uint32_t sec_storage;
			uint32_t data;
		} diskusage;
	} args;
};

struct misc_control_t {
	misc_cmd_t cmd;   /* for s to n */
	int32_t ret;
	int32_t magic;
	union args2 {
		/* add by q00209673 for bootloader lock status in nv partition */
		struct {
			uint8_t bootloader_info[WIDEVINE_NV_WVLOCK_SIZE];
		} nv_info;
		struct {
			uint32_t seconds;
			uint32_t millis;
			char time_str[30];
		} get_time;
	} args;
};

struct opened_file {
	FILE *file;
	struct opened_file *next;
	struct opened_file *prev;
};

/*  open file list head */
static struct opened_file *m_first_file = (struct opened_file *)NULL;

/* record the current m_userid */
static uint32_t m_userid;
static uint32_t set_current_userid(uint32_t id)
{
	m_userid = id;
	return m_userid;
}
static uint32_t get_current_userid()
{
	return m_userid;
}

/* add to tail */
static void add_open_file(struct opened_file *new_file)
{
	if (NULL == new_file)
		return;

	if (m_first_file == NULL) {
		m_first_file = new_file;
		new_file->next = new_file;
		new_file->prev = new_file;
	} else {
		m_first_file->prev->next = new_file;
		new_file->prev = m_first_file->prev;
		new_file->next = m_first_file;
		m_first_file->prev = new_file;
	}
}

static void del_open_file(struct opened_file *file)
{
	struct opened_file *next;

	if (NULL == file)
		return;

	next = file->next;

	if (file == next) { /* only 1 node */
		m_first_file = NULL;
	} else {
		if (file == m_first_file)
			m_first_file = file->next;

		next->prev = file->prev;
		file->prev->next = next;
	}
}

static int find_open_file(int fd, struct opened_file **file)
{
	struct opened_file *p = m_first_file;
	int find_flag = 0;

	if (p != NULL) {
		do {
			if (fileno(p->file) == fd) {
				find_flag = 1;
				break;
			}

			p = p->next;
		} while (p != m_first_file);
	}

	if (!find_flag)
		p = NULL;

	if (NULL != file)
		*file = p;

	return find_flag;
}

/*  smc dev */
static int m_fs_fd = -1, m_misc_fd = -1;

/*DTS2014040405270 chown sec_stroage_data q00209673 begin */
/*
* path:file or dir to change own
* flag: 0(dir);1(file)
*/
static void chown_sec_storage_data_to_system(char *path, int flag)
{
	int ret = 0;

	if (NULL == path)
		return;

	/*check path is SEC_STORAGE_ROOT_DIR or SEC_STORAGE_DATA_DIR,
	* we only need to change SEC_STORAGE_DATA_DIR from root to system
	*/
	if (strstr(path, "sec_storage_data")) {
		ret = chown(path, AID_SYSTEM, AID_SYSTEM);
		if (ret < 0)
			tloge("chown erro\n");

		if (flag) {
			ret = chmod(path, S_IRUSR | S_IWUSR);
			if (ret < 0)
				tloge("chmod erro\n");
		} else {
			ret = chmod(path, S_IRUSR | S_IWUSR | S_IXUSR);
			if (ret < 0)
				tloge("chmod erro\n");
		}
	}
}
/*DTS2014040405270 chown sec_stroage_data q00209673 end */


/*
* path:file path name.
* e.g. sec_storage_data/app1/sub1/fileA.txt
* then mkdir_r will make dir sec_storage_data, app1 and sub1.
*/
int mkdir_r(const char *path)
{
	if (path == NULL) {
		return -1;
	}

	char *temp = strdup(path);
	char *pos = temp;

	if (temp == NULL) {
		tloge("strdup error\n");
		return -1;
	}

	/* ȥ���ͷ�� './' �� '/' */
	if (strncmp(temp, "/", 1) == 0) {
		pos += 1;
	} else if (strncmp(temp, "./", 2) == 0) {
		pos += 2;
	}

	/* ѭ������Ŀ¼ */
	/* ������һ�����ļ�����������'/'��β��
	����'\0'����ֹѭ����
	��˲��ᴴ�����ļ�����ͬ��Ŀ¼ */
	for (; *pos != '\0'; ++ pos) {
		if (*pos == '/') {
			*pos = '\0';

			if (0 == access(temp, F_OK)) {
				*pos = '/';
				continue;
			}

			if (mkdir(temp, ROOT_DIR_PERM) != 0) { /*[false alarm]: fortify */
				tloge("mkdir %s fail\n", temp);
				free(temp);
				temp = NULL;
				(void)temp;
				return -1;
			}

			/*DTS2014040405270 chown sec_stroage_data q00209673 */
			chown_sec_storage_data_to_system(temp, 0);
			tlogv("for %s\n", temp);
			*pos = '/';
		}
	}

	/*������һ���ַ�����Ŀ¼�������ļ�����
	����Ҫ����������δ���*/
	/*
	if (*(pos - 1) != '/') {
	    printf("if %s\n", temp);
	    mkdir(temp, ROOT_DIR_PERM);
	}
	*/
	free(temp);
	temp = NULL;
	(void)temp;
	return 0;
}

#define FW_SHOW_MULTIUSERUI "fw.show_multiuserui"
#define FW_MAX_USERS "fw.max_users"
static int system_support_multi_users()
{
#ifndef WITH_DEVCHIP_PLATFORM
	char prop_show_multiuser[PROPERTY_VALUE_MAX] = {0};
	char prop_max_users[PROPERTY_VALUE_MAX] = {0};
	int multi_user = 0, max_users = 0;

	if (!(property_get(FW_SHOW_MULTIUSERUI, prop_show_multiuser, "0"))) {
		tloge("property %s is invalid\n", FW_SHOW_MULTIUSERUI);
		return 0;
	}

	if (!(property_get(FW_MAX_USERS, prop_max_users, "0"))) {
		tloge("property %s is invalid\n", FW_MAX_USERS);
		return 0;
	}

	multi_user = atoi(prop_show_multiuser);
	max_users = atoi(prop_max_users);

	if (multi_user == 1 && max_users > 1)
		return 1;
	else
		return 0;
#endif
	return 0;
}

/*
 * example for join_file_name:
 *
 * name                         userid        path
 * -------------------------------------------------------------------------
 * abc/def                      DC          /data/sec_storage/abc/def
 * sec_storage/abc/def          DC          /data/sec_storage/abc/def
 * sec_storage_data/abc/def     0           /data/sec_storage_data/abc/def
 * sec_storage_data/abc/def     1           /data/sec_storage_data/1/abc/def
 */
static int join_file_name(const char *name, char *path)
{
	errno_t rc = EOK;

	if (name == NULL || path == NULL)
		return -1;

	/*If the path name does not start with sec_storage or sec_storage_data,
	add sec_storage str for the path*/
	if (name == strstr(name, SFS_PARTITION_TRANSIENT)) {
		uint32_t t_userid = get_current_userid();

		rc = strncpy_s(path, FILE_NAME_MAX_BUF, USER_DATA_DIR, sizeof(USER_DATA_DIR));
		if (EOK != rc) {
			tloge("strncpy_s failed %d\n", rc);
			return -1;
		}

		if (system_support_multi_users() && t_userid != 0) {
			char user_path[10] = {0};
			rc = snprintf_s(user_path, sizeof(user_path), sizeof(user_path) - 1,
				"%d/", t_userid);
			if (-1 == rc) {
				tloge("snprintf_s failed %d\n", rc);
				return -1;
			}

			rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path),
				SFS_PARTITION_USER_SYMLINK,
				strlen(SFS_PARTITION_USER_SYMLINK) + 1);
			if (EOK != rc) {
				tloge("strncat_s failed %d\n", rc);
				return -1;
			}

			rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path),
				user_path, strlen(user_path) + 1);
			if (EOK != rc) {
				tloge("strncat_s failed %d\n", rc);
				return -1;
			}

			rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path),
				name + strlen(SFS_PARTITION_TRANSIENT),
				(strlen(name) - strlen(SFS_PARTITION_TRANSIENT)) + 1);
			if (EOK != rc) {
				tloge("strncat_s failed %d\n", rc);
				return -1;
			}


		} else {
			rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path),
				name, strlen(name) + 1);
			if (EOK != rc) {
				tloge("strncat_s failed %d\n", rc);
				return -1;
			}
		}
	} else if (name == strstr(name, SFS_PARTITION_PERSISTENT)) {
		rc = strncpy_s(path, FILE_NAME_MAX_BUF, ROOT_DIR, sizeof(ROOT_DIR));
		if (EOK != rc) {
			tloge("strncpy_s failed %d\n", rc);
			return -1;
		}

		rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path), name, strlen(name) + 1);
		if (EOK != rc) {
			tloge("strncat_s failed %d\n", rc);
			return -1;
		}
	} else {
		rc = strncpy_s(path, FILE_NAME_MAX_BUF,
			SEC_STORAGE_ROOT_DIR, sizeof(SEC_STORAGE_ROOT_DIR));
		if (EOK != rc) {
			tloge("strncpy_s failed %d\n", rc);
			return -1;
		}

		rc = strncat_s(path, FILE_NAME_MAX_BUF - strlen(path), name, strlen(name) + 1);
		if (EOK != rc) {
			tloge("strncat_s failed %d\n", rc);
			return -1;
		}
	}

	tlogv("joined path is \"%s\"\n", path);

	return 0;
}

static int is_file_exist(const char *name)
{
	struct stat statbuf;

	if (NULL == name)
		return 0;

	if (stat(name, &statbuf)) {
		if (errno == ENOENT) /* file not exist */
			return 0;
		else {
			tloge("file stat failed\n");
			return 1;
		}
	}

	return 1;
}

static void open_work(struct sec_storage_t *trans_control)
{
	struct opened_file *sel_file;
	FILE *pFile;
	char name_buff[256] = {0};
	errno_t rc = EOK;

	if (NULL == trans_control)
		return;

	set_current_userid(trans_control->userid);

	if (!join_file_name((char *)(trans_control->args.open.name), name_buff)) {
		if (trans_control->cmd == sec_create) {
			/* create a exist file, remove it at first */
			rc = strncpy_s(trans_control->args.open.mode,
				sizeof(trans_control->args.open.mode), "w+", 4);
			if (EOK != rc) {
				tloge("strncpy_s failed %d\n", rc);
				trans_control->ret = -1;
				trans_control->error = ENOENT;
				return;
			}

		} else {
			if (is_file_exist(name_buff) == 0) {
				/* open a nonexist file, return fail */
				tloge("file(%s) not exist, open failed\n", name_buff);
				trans_control->ret = -1;
				trans_control->error = ENOENT;
				return;
			}
		}

		/*mkdir -p for new create files*/
		if (mkdir_r(name_buff)) {
			trans_control->ret = -1;
			trans_control->error = (uint32_t)errno;
			return;
		}

		pFile = fopen(name_buff, trans_control->args.open.mode); /* [false alarm] */
		if (pFile == NULL) {
			tloge("open file(%s) with flag(%s) failed: %s\n",
			     name_buff, trans_control->args.open.mode, strerror(errno));
			trans_control->ret = -1;
			trans_control->error = (uint32_t)errno;
		} else {
			/*DTS2014040405270 chown sec_stroage_data q00209673 */
			chown_sec_storage_data_to_system(name_buff, 1); /* [false alarm]: fortify */
			tlogv("open file(%s) success : fd = %d\n",
				(char *)(trans_control->args.open.name), fileno(pFile));
			sel_file = (struct opened_file *)malloc(sizeof(struct opened_file));

			if (!sel_file) {
				tloge("malloc opened_file failed\n");
				trans_control->ret = -1;
			} else {
				sel_file->file = pFile;
				trans_control->ret = fileno(sel_file->file);  /* return fileno */
				add_open_file(sel_file);
			}
		}
	} else {
		/* NEVER in this case */
		trans_control->ret = -1;
	}
}

static void close_work(struct sec_storage_t *trans_control)
{
	int ret;
	struct opened_file *sel_file = NULL;

	if (NULL == trans_control)
		return;

	if (find_open_file(trans_control->args.close.fd, &sel_file)) {
		ret = fclose(sel_file->file);
		if (ret == 0) {
			tlogv("close file(%d) success\n", trans_control->args.close.fd);
			/* list_del(&sel_file->head); */
			del_open_file(sel_file);
			free(sel_file);
			sel_file = NULL;
			(void)sel_file;
		} else {
			tloge("close file(%d) failed: %s\n", trans_control->args.close.fd,
				strerror(errno));
			trans_control->error = (uint32_t)errno;
		}

		trans_control->ret = ret;

	} else {
		tloge("can't find opened file(fileno = %d)\n", trans_control->args.close.fd);
		trans_control->ret = -1;
		trans_control->error = EBADF;
	}
}

static void read_work(struct sec_storage_t *trans_control)
{
	struct opened_file *sel_file = NULL;
	size_t count;

	if (NULL == trans_control)
		return;

	if (find_open_file(trans_control->args.read.fd, &sel_file)) {
		count = fread((void *)(trans_control->args.read.buffer), 1,
			      trans_control->args.read.count, sel_file->file);
		trans_control->ret = (int32_t)count;

		if (count < trans_control->args.read.count) {
			if (feof(sel_file->file)) {
				trans_control->ret2 = 0;
				tlogv("read end of file\n");
			} else {
				trans_control->ret2 = -1;
				trans_control->error = (uint32_t)errno;
				tloge("read file failed: %s\n", strerror(errno));
			}
		} else {
			trans_control->ret2 = 0;
			tlogv("read file success, content len=%lu\n", count);
		}
	} else {
		trans_control->ret   = 0;
		trans_control->ret2  = -1;
		trans_control->error = EBADF;
		tloge("can't find opened file(fileno = %d)\n", trans_control->args.read.fd);
	}
}

static void write_work(struct sec_storage_t *trans_control)
{
	struct opened_file *sel_file = NULL;
	size_t count;
	int fd;
	int32_t ret;

	if (NULL == trans_control)
		return;

	if (find_open_file(trans_control->args.write.fd, &sel_file)) {
		count = fwrite((void *)(trans_control->args.write.buffer), 1,
			       trans_control->args.write.count, sel_file->file);
		if (count < trans_control->args.write.count) {
			tloge("write file failed: %s\n", strerror(errno));
			trans_control->ret = (int32_t)count;
			trans_control->error = (uint32_t)errno;
			return;
		}

		/* TODO need? */
		if (fflush(sel_file->file) != 0) {
			tloge("fflush file failed: %s\n", strerror(errno));
			trans_control->ret = 0;
			trans_control->error = (uint32_t)errno;
		} else {
			fd = fileno(sel_file->file);
			ret = fsync(fd);
			if (0 != ret) {
				tloge("fsync:fsync file failed: %s\n", strerror(errno));
				trans_control->ret = 0;
				trans_control->error = (uint32_t)errno;
			} else {
				trans_control->ret = (int32_t)count;
			}
		}
	} else {
		tloge("can't find opened file(fileno = %d)\n", trans_control->args.write.fd);
		trans_control->ret = 0;
		trans_control->error = EBADF;
	}
}

static void seek_work(struct sec_storage_t *trans_control)
{
	int ret;
	struct opened_file *sel_file = NULL;

	if (NULL == trans_control)
		return;

	if (find_open_file(trans_control->args.seek.fd, &sel_file)) {
		ret = fseek(sel_file->file, trans_control->args.seek.offset,
			(int)trans_control->args.seek.whence);
		if (ret) {
			tloge("seek file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno;
		} else {
			tlogv("seek file success\n");
		}

		trans_control->ret = ret;

	} else {
		tloge("can't find opened file(fileno = %d)\n", trans_control->args.seek.fd);
		trans_control->ret = -1;
		trans_control->error = EBADF;
	}
}

/* return -1 on failure, with errno set to the first error */
int unlink_recursive(const char *name)
{
	struct stat st;
	DIR *dir;
	struct dirent *de;
	int fail = 0;
	char dn[PATH_MAX] = {0};
	errno_t rc = EOK;

	if (NULL == name) {
		return -1;
	}

	/* is it a file or directory? */
	if (lstat(name, &st) < 0) {
		tloge("lstat %s failed, errno is %x\n", name, errno);
		return -1;
	}

	/* a file, so unlink it */
	if (!S_ISDIR(st.st_mode)) {
		if (unlink(name) < 0) { /* [false alarm]: no perto handle other dirs */
			tloge("unlink %s failed, errno is %d\n", name, errno);
			return -1;
		}

		return 0;
	}

	/* a directory, so open handle */
	dir = opendir(name); /* [false alarm]: teecd has no permission to handle other directorys */
	if (dir == NULL) {
		tloge("dir %s open failed\n", name);
		return -1;
	}

	/* recurse over components */
	errno = 0;

	while ((de = readdir(dir)) != NULL) {
		if (!strncmp(de->d_name, "..", sizeof(".."))
			|| !strncmp(de->d_name, ".", sizeof(".")))
			continue;

		rc = snprintf_s(dn, sizeof(dn), sizeof(dn) - 1, "%s/%s", name, de->d_name);
		if (-1 == rc) {
			tloge("snprintf_s failed %d\n", rc);
			fail = 1;
			break;
		}

		if (unlink_recursive(dn) < 0) { /* [false alarm]: no perto handle other dirs */
			tloge("loop unlink_recursive() failed, there are read-only file\n");
			fail = 1;
			break;
		}

		errno = 0;
	}

	/* in case readdir or unlink_recursive failed */
	if (fail || errno < 0) {
		int save = errno;
		closedir(dir);
		errno = save;
		tloge("fail is %d, errno is %d\n", fail, errno);
		return -1;
	}

	/* close directory handle */
	if (closedir(dir) < 0) {
		tloge("closedir %s failed, errno is %d\n", name, errno);
		return -1;
	}

	/* delete target directory */
	if (rmdir(name) < 0) { /* [false alarm]: no perto handle other dirs */
		tloge("rmdir %s failed, errno is %d\n", name, errno);
		return -1;
	}

	return 0;
}

static void remove_work(struct sec_storage_t *trans_control)
{
	int ret;
	char name_buff[256] = {0};


	set_current_userid(trans_control->userid);

	if (!join_file_name((char *)(trans_control->args.remove.name), name_buff)) {
		ret = unlink_recursive(name_buff);
		if (ret) {
			tloge("remove file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno;
		} else {
			tlogv("remove file success\n");
		}

		trans_control->ret = ret;

	} else {
		trans_control->ret = -1;
	}
}

static void truncate_work(struct sec_storage_t *trans_control)
{
	int ret;
	char name_buff[256] = {0};


	set_current_userid(trans_control->userid);

	if (!join_file_name((char *)(trans_control->args.truncate.name), name_buff)) {
		ret = truncate(name_buff, (long)trans_control->args.truncate.len);
		if (ret) {
			tloge("truncate file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno; /*lint !e852 */
		} else {
			tlogv("truncate file success\n");
		}

		trans_control->ret = ret;

	} else {
		trans_control->ret = -1;
	}
}

static void rename_work(struct sec_storage_t *trans_control)
{
	int ret;
	char name_buff[256] = {0}, name_buff2[256] = {0};

	if (NULL == trans_control)
		return;

	set_current_userid(trans_control->userid);

	if (!join_file_name((char *)(trans_control->args.rename.buffer), name_buff) &&
	    !join_file_name((char *)(trans_control->args.rename.buffer) +
			    trans_control->args.rename.old_name_len, name_buff2)) {
		ret = rename(name_buff, name_buff2);
		if (ret) {
			tloge("rename file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno;
		} else {
			tlogv("rename file success\n");
		}

		trans_control->ret = ret;

	} else {
		trans_control->ret = -1;
	}
}

#define MAXBSIZE 65536

static int copy_file(const char *from_path, const char *to_path)
{
	int from_fd = -1, to_fd = -1;
	ssize_t rcount, wcount;
	static char buf[MAXBSIZE] = {0};
	struct stat from_stat;
	int ret;

	if (NULL == from_path || NULL == to_path)
		return -1;

	if ((from_fd = open(from_path, O_RDONLY, 0)) == -1) {
		tloge("open file %s failed: %s\n", from_path, strerror(errno));
		ret = -1;
		goto out;
	}

	if (fstat(from_fd, &from_stat) == -1) {
		tloge("stat file %s failed: %s\n", from_path, strerror(errno));
		ret = -1;
		goto out;
	}

	if ((to_fd = open(to_path, O_WRONLY | O_TRUNC | O_CREAT, from_stat.st_mode)) == -1) {
		tloge("stat file %s failed: %s\n", to_path, strerror(errno));
		ret = -1;
		goto out;
	}

	while ((rcount = read(from_fd, buf, MAXBSIZE)) > 0) {
		wcount = write(to_fd, buf, (size_t)rcount);
		if (rcount != wcount || wcount == -1) {
			tloge("write file %s failed: %s\n", to_path, strerror(errno));
			ret = -1;
			goto out;
		}
	}

	if (rcount < 0) {
		tloge("read file %s failed: %s\n", from_path, strerror(errno));
		ret = -1;
		goto out;
	}

	chown_sec_storage_data_to_system((char *)to_path, 1); /* [false alarm]: fortify */
	ret = 0;
out:

	if (from_fd >= 0)
		close(from_fd);

	if (to_fd >= 0)
		close(to_fd);

	return ret;
}

static void copy_work(struct sec_storage_t *trans_control)
{
	int ret;
	char from_path[256] = {0}, to_path[256] = {0};

	if (NULL == trans_control)
		return;

	set_current_userid(trans_control->userid);

	if (!join_file_name((char *)(trans_control->args.cp.buffer), from_path) &&
		!join_file_name((char *)(trans_control->args.cp.buffer)
				+ trans_control->args.cp.from_path_len, to_path)) {
		ret = copy_file(from_path, to_path);
		if (ret) {
			tloge("copy file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno;
		} else {
			tlogv("copy file success\n");
		}

		trans_control->ret = ret;

	} else {
		trans_control->ret = -1;
	}
}

static void file_info_work(struct sec_storage_t *trans_control)
{
	int ret;
	struct opened_file *sel_file;
	struct stat stat_buff;

	if (NULL == trans_control)
		return;

	trans_control->args.info.file_len = trans_control->args.info.cur_pos = 0;

	if (find_open_file(trans_control->args.info.fd, &sel_file)) {
		if ((ret = fstat(trans_control->args.info.fd, &stat_buff)) == 0) {
			trans_control->args.info.file_len = (uint32_t)stat_buff.st_size;
			trans_control->args.info.cur_pos = (uint32_t)ftell(sel_file->file);
			tlogv("derive file info success : pos/len = %d/%d\n",
			     trans_control->args.info.cur_pos,
			     trans_control->args.info.file_len);
		} else {
			tloge("fstat file failed: %s\n", strerror(errno));
			trans_control->error = (uint32_t)errno;
		}

		trans_control->ret = ret;
	} else {
		trans_control->ret = -1;
		trans_control->error = EBADF;
	}
}

static void file_access_work(struct sec_storage_t *trans_control)
{
	int ret;
	char name_buff[256] = {0};
	if (NULL == trans_control)
		return;

	if (trans_control->cmd == sec_access) {
		set_current_userid(trans_control->userid);

		if (!join_file_name((char *)(trans_control->args.access.name), name_buff)) {
			ret = access(name_buff, trans_control->args.access.mode);
			if (ret < 0) {
				tloge("access file(%s) mode(%d) failed: %s\n", name_buff,
					trans_control->args.access.mode,
				     strerror(errno));
			}

			trans_control->ret = ret;
			trans_control->error = (uint32_t)errno;

		} else {
			trans_control->ret = -1;
		}
	} else {
		ret = access((char *)(trans_control->args.access.name),
			trans_control->args.access.mode);
		if (ret < 0) {
			tloge("access2 file(%s) mode(%d) failed: %s\n",
				(char *)trans_control->args.access.name,
				trans_control->args.access.mode, strerror(errno));
		}

		trans_control->ret = ret;
		trans_control->error = (uint32_t)errno;
	}
}

static void fsync_work(struct sec_storage_t *trans_control)
{
	int ret, fd;
	struct opened_file *sel_file;

	if (NULL == trans_control)
		return;

	/* opened file */
	if (trans_control->args.fsync.fd
		&& find_open_file(trans_control->args.fsync.fd, &sel_file)) {
		/* first,flush memory from user to kernel */
		ret = fflush(sel_file->file);
		if (0 != ret) {
			tloge("fsync:fflush file failed: %s\n", strerror(errno));
			trans_control->ret = -1;
			trans_control->error = (uint32_t)errno;
			return;
		}

		/* second,fsync memory from kernel to disk */
		fd = fileno(sel_file->file);
		ret = fsync(fd);
		if (0 != ret) {
			tloge("fsync:fsync file failed: %s\n", strerror(errno));
			trans_control->ret = -1;
			trans_control->error = (uint32_t)errno;
			return;
		}

		/* ret = 0; */
		trans_control->ret = 0;
		tlogv("fsync file(%d) success\n", trans_control->args.fsync.fd);
	} else {
		tloge("can't find opened file(fileno = %d)\n", trans_control->args.fsync.fd);
		trans_control->ret = -1;
		trans_control->error = EBADF;
	}
}

static void disk_usage_work(struct sec_storage_t *trans_control)
{
	struct statfs st;
	uint32_t data_remain, sec_storage_remain;

	if (NULL == trans_control)
		return;

	if (statfs("/data", &st) < 0) {
		tloge("statfs /data failed, err=%s\n", strerror(errno));
		goto error;
	}

	data_remain = (long)st.f_bfree * (long)st.f_bsize / 1024; /*lint !e732 */

	if (statfs("/sec_storage", &st) < 0) {
		tloge("statfs /sec_storage failed, err=%s\n", strerror(errno));
		goto error;
	}

	sec_storage_remain = (long)st.f_bfree * (long)st.f_bsize / 1024; /*lint !e732 */

	trans_control->ret = 0;
	trans_control->args.diskusage.data = data_remain;
	trans_control->args.diskusage.sec_storage = sec_storage_remain;
	return;

error:
	trans_control->ret = -1;
	trans_control->error = (uint32_t)errno; /*lint !e732 */
}

#define USERDATA_READY_PROPERTY	"sys.userdata_is_ready"
static int is_userdata_ready(void)
{
#ifndef WITH_DEVCHIP_PLATFORM
	char property[PROPERTY_VALUE_MAX];

	if (property_get(USERDATA_READY_PROPERTY, property, NULL))
		tlogv("userdata ready property is %s\n", property);
	else {
		tloge("property %s is invalid\n", USERDATA_READY_PROPERTY);
		return 0;
	}

	if (!memcmp(property, "1", sizeof("1")))
		return 1;

	return 0;
#else
	return 1;
#endif
}

static void *thread_fs_work(void *control)
{
	struct sec_storage_t *trans_control;
	int ret;

	if (NULL == control)
		return NULL;

	trans_control = (struct sec_storage_t *)control;

	if (-1 == m_fs_fd) {
		tloge("fs is not open\n");
		return NULL;
	}

	while (1) { /*lint !e716 */
		tlogv("++ fs agent loop ++\n");
		ret = ioctl(m_fs_fd, (int)TC_NS_CLIENT_IOCTL_WAIT_EVENT, /*lint !e835 */
			AGENT_FS_ID);
		if (ret) {
			tloge("fs agent  wait event failed\n");
			break;
		}

		tlogv("fs agent wake up and working!!\n");

		if (is_userdata_ready() == 0) {
			trans_control->ret = -1;
			tloge("do secure storage while userdata is not ready, skip!\n");
			goto file_work_done;
		}

		switch (trans_control->cmd) {
		case sec_open:
		case sec_create:
			tlogv("sec storage : open/create\n");
			tlogv("open file name=%s, mode = %s\n",
				(char *)(trans_control->args.open.name),
				trans_control->args.open.mode);
			open_work(trans_control);
			break;

		case sec_close:
			tlogv("sec storage : close\n");
			close_work(trans_control);
			break;

		case sec_read:
			tlogv("sec storage : read count = %d\n", trans_control->args.read.count);
			read_work(trans_control);
			break;

		case sec_write:
			tlogv("sec storage : write count = %d\n", trans_control->args.write.count);
			write_work(trans_control);
			break;

		case sec_seek:
			tlogv("sec storage : seek offset=%d, whence=%d\n",
			     trans_control->args.seek.offset, trans_control->args.seek.whence);
			seek_work(trans_control);
			break;

		case sec_remove:
			tlogv("sec storage : remove \"%s\"\n",
				(char *)(trans_control->args.remove.name));
			remove_work(trans_control);
			break;

		case sec_truncate:
			tlogv("sec storage : truncate \"%s\", len=%d\n",
			     (char *)(trans_control->args.truncate.name),
			     trans_control->args.truncate.len);
			truncate_work(trans_control);
			break;

		case sec_rename:
			tlogv("sec storage : rename old = \"%s\", new = \"%s\"\n",
				(char *)(trans_control->args.rename.buffer),
				(char *)(trans_control->args.rename.buffer)
				+ trans_control->args.rename.old_name_len);
			rename_work(trans_control);
			break;

		case sec_cp:
			tlogv("sec storage : copy from = \"%s\", to = \"%s\"\n",
				(char *)(trans_control->args.cp.buffer),
				(char *)(trans_control->args.cp.buffer)
				+ trans_control->args.cp.from_path_len);
			copy_work(trans_control);
			break;

		case sec_info:
			tlogv("sec storage : file info\n");
			file_info_work(trans_control);
			break;

		case sec_access:
		case sec_access2:
			tlogv("sec storage : file access\n");
			file_access_work(trans_control);
			break;

		case sec_fsync:
			tlogv("sec storage : file fsync\n");
			fsync_work(trans_control);
			break;

		case sec_diskusage:
			tlogv("sec storage : disk usage\n");
			disk_usage_work(trans_control);
			break;

		default:
			/* TODO write a special error code will better? */
			tloge("fs agent error cmd:trans_control->cmd=%x\n", trans_control->cmd);
			break;
		}

file_work_done:
		trans_control->magic = AGENT_FS_ID;
		ret = ioctl(m_fs_fd, (int)TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE, /*lint !e835 */
			AGENT_FS_ID);
		if (ret) {
			tloge("fs agent send reponse failed\n");
			break;
		}

		tlogv("-- fs agent loop --\n");
	}

	return NULL;
}
#ifndef WITH_DEVCHIP_PLATFORM
#ifndef K3V3_FPGA
/* add by q00209673 for bootloaderlock status in nv partition */
static void nv_info_work(struct misc_control_t *trans_control)
{
	int ret;
	SecureBoot_LockInfo info;
	errno_t rc = EOK;

	if (NULL == trans_control)
		return;

	ret = handle_nv(&info);
	if (ret) {
		LOGE("we get bootloader info in nv partition failed\n");
		trans_control->ret = -1;
		return;
	}

	if (sizeof(SecureBoot_LockInfo) != WIDEVINE_NV_WVLOCK_SIZE) {
		LOGE("SecureBoot_LockInfo had been changed\n");
		trans_control->ret = -1;
		return;
	}

	rc = memcpy_s(trans_control->args.nv_info.bootloader_info,
			sizeof(trans_control->args.nv_info.bootloader_info),
			&info, WIDEVINE_NV_WVLOCK_SIZE);
	if (EOK != rc) {
		LOGE("memcpy_s failed: %d\n", rc);
		trans_control->ret = -1;
		return;
	}

	trans_control->ret = ret;
	LOGV("we get bootloader info successfully\n");
	return;
}
#endif
#endif
static void get_time_work(struct misc_control_t *trans_control)
{
	struct timeval time_val;
	errno_t rc = EOK;

	if (NULL == trans_control)
		return;

	if (gettimeofday(&time_val, NULL) == 0) {
		trans_control->ret = 0;
		trans_control->args.get_time.seconds = time_val.tv_sec; /*lint !e732 */
		trans_control->args.get_time.millis = (time_val.tv_usec / 1000); /*lint !e732 */
		struct tm *tstruct = NULL;

		tstruct = localtime(&(time_val.tv_sec));
		if (NULL != tstruct) {
			/* year months days hour min second */
			rc = snprintf_s(trans_control->args.get_time.time_str,
				sizeof(trans_control->args.get_time.time_str),
				sizeof(trans_control->args.get_time.time_str) - 1,
				"%04d-%02d-%02d %02d:%02d:%02d.%03d ",
				tstruct->tm_year + 1900,
				tstruct->tm_mon + 1,
				tstruct->tm_mday,
				tstruct->tm_hour,
				tstruct->tm_min,
				tstruct->tm_sec,
				(uint32_t)(time_val.tv_usec / 1000));
			if (-1 == rc) {
				trans_control->ret = -1;
				tloge("snprintf_s error %d\n", rc);
			}
		} else {
			tloge("get localtiem error\n");
		}
	} else {
		trans_control->ret = -1;
		trans_control->args.get_time.seconds = 0;
		trans_control->args.get_time.millis = 0;
	}
}

static void sync_sys_time_to_secure(void *control)
{
	struct misc_control_t *trans_control;
	TC_NS_Time tc_ns_time;
	int ret;

	if (NULL == control)
		return;

	trans_control = (struct misc_control_t *)control;

	if (-1 == m_misc_fd) {
		tloge("misc file is not open\n");
		return;
	}

	get_time_work(trans_control);

	tc_ns_time.seconds = trans_control->args.get_time.seconds;
	tc_ns_time.millis = trans_control->args.get_time.millis;

	ret = ioctl(m_misc_fd, (int)TC_NS_CLIENT_IOCTL_SYC_SYS_TIME, /*lint !e835 */
		(unsigned long)&tc_ns_time);
	if (ret) {
		tloge("failed to send sys time to teeos\n");
		return;
	}
}

static void *thread_misc_work(void *control)
{
	struct misc_control_t *trans_control;
	int ret;

	if (NULL == control)
		return NULL;

	trans_control = (struct misc_control_t *)control;

	if (-1 == m_misc_fd) {
		tloge("misc file is not open\n");
		return NULL;
	}

	sync_sys_time_to_secure(trans_control);

	while (1) { /*lint !e716 */
		tlogv("++ misc agent loop ++\n");
		ret = ioctl(m_misc_fd, (int)TC_NS_CLIENT_IOCTL_WAIT_EVENT, /*lint !e835 */
			AGENT_MISC_ID);
		if (ret) {
			tloge("misc agent wait event failed\n");
			break;
		}

		tlogv("misc agent wake up and working!!\n");

		switch (trans_control->cmd) {
		case sec_nv_info:/* add by q00209673 for bootloaderlock status in nv partition */
			tlogv("sec nv info access\n");
			break;

		case sec_get_time:
			tlogv("sec get time of day\n");
			get_time_work(trans_control);
			break;

		default:
			/* TODO write a special error code will better? */
			tloge("misc agent error cmd\n");
			break;
		}

		trans_control->magic = AGENT_MISC_ID;
		ret = ioctl(m_misc_fd, (int)TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE, /*lint !e835 */
			AGENT_MISC_ID);
		if (ret) {
			tloge("misc agent send reponse failed\n");
			break;
		}

		tlogv("-- misc agent loop --\n");
	}

	return NULL;
}


static int agent_init(unsigned int id, int *dev_fd, void **control)
{
	int fd;
	void *buff = NULL;
	int ret;
	int index = 0;

	if (NULL == dev_fd || NULL == control)
		return -1;

	fd = open(TC_NS_CLIENT_DEV_NAME, O_RDWR);
	if (fd < 0) {
		tloge("open tee client dev failed, fd is %d\n", fd);
		return -1;
	}

	/* the trick is using vm_pgoff to identify which agent it is in kernel */
	if (id == AGENT_FS_ID)
		index = 1;
	else if (id == AGENT_MISC_ID)
		index = 2;

#ifdef TEE_SUPPORT_SOCKET
	else if (id == AGENT_SOCKET_ID)
		index = 3;

#endif

	/* register agent */
	buff = mmap(0, TRANS_BUFF_SIZE, (PROT_READ | PROT_WRITE), MAP_SHARED, fd,
		index * PAGE_SIZE);
	if ((void *)buff == MAP_FAILED) {
		(void)close(fd);
		tloge("mmap failed\n");
		return -1;
	}

	ret = ioctl(fd, (int)TC_NS_CLIENT_IOCTL_REGISTER_AGENT, id); /*lint !e835 */
	if (ret) {
		(void)close(fd);
		tloge("ioctl failed\n");
		(void)munmap(buff, TRANS_BUFF_SIZE);
		return -1;
	}

	*dev_fd = fd;
	*control = buff;

	return 0;
}

static void agent_exit(unsigned int id, int fd,  void *control)
{
	int ret;

	if (-1 == fd) {
		return;
	}

	ret = ioctl(fd, (int)TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT, id); /*lint !e835 */
	if (ret) {
		tloge("ioctl failed\n");
	}

	if (control != NULL) {
		(void)munmap(control, TRANS_BUFF_SIZE);
	}

	(void)close(fd);
}

/* just mkdir sec_storage_data, mount sec_storage partition in init.chip.rc */
int make_sec_storage_dir(char *storage_dir)
{
	struct stat statbuf;
	int ret;

	if (NULL == storage_dir) {
		tloge("invalid input parameter\n");
		return -1;
	}

	/*mkdir for sec_storage_data partition*/
	if (stat(storage_dir, &statbuf)) {
		tloge("stat %s fail, erron=%d, err:%s\n", storage_dir, errno, strerror(errno));

		if (errno == ENOENT) { /* folder not exist */
			tloge("stat %s, folder not exist\n", storage_dir);

			/*DTS2014040405270 chown sec_stroage_data q00209673 begin*/
			ret = mkdir(storage_dir, S_IRUSR | S_IWUSR | S_IXUSR); /*[false alarm]*/
			if (ret == -1) {
				tloge("make dir %s fail, err %s\n", storage_dir, strerror(errno));
				return -1;
			}

			ret = chown(storage_dir, AID_SYSTEM, AID_SYSTEM);
			if (ret < 0)
				tloge("chown error");

			/*DTS2014040405270 chown sec_stroage_data q00209673 end*/
		} else {
			tloge("stat %s fail else\n", storage_dir);

			if (S_ISDIR(statbuf.st_mode)) {
				tloge("ROOT DATA DIR is exist, but not a dir\n");
				/* TODO rm it and mkdir? */
				return -1;
			}
		}
	}

	return 0;
}

#define READ_PROC_MOUNT_SIZE 0x1000
int check_partition_ready(const char *mp)
{
	int fd = -1;
	char *pbuf = NULL;
	int ret = -1;
	char *PROC_SELF_MOUNTS = "/proc/mounts";
	char *ptmp = NULL;
	int buf_scale = 1;

	if (NULL == mp) {
		return -1;
	}

retry:
	fd = open(PROC_SELF_MOUNTS, O_RDWR);
	if (fd < 0) {
		goto err0;
	}

	pbuf = (char *)malloc(READ_PROC_MOUNT_SIZE * buf_scale + 1); /*lint !e732 */
	if (pbuf == NULL) {
		goto err1;
	}

	ret = read(fd, pbuf, READ_PROC_MOUNT_SIZE * buf_scale); /*lint !e732 */
	if (ret >= READ_PROC_MOUNT_SIZE * buf_scale) {
		buf_scale++;
		free(pbuf);
		pbuf = NULL;
		(void)pbuf;
		close(fd);
		ret = -1;
		goto retry;
	}

	pbuf[ret] = '\0';
	if (NULL == (ptmp = strstr(pbuf, mp))) {
		ret = -1;
	} else {
		*(ptmp + strlen(mp)) = '\0';
		ret = 1;
	}

	free(pbuf);
	pbuf = NULL;
	(void)pbuf;
err1:
	close(fd);
err0:
	return ret;
}


int link_ttf_file(void)
{
	int ret;

	ret = access("/sec_storage/DroidSansFallback.ttf", F_OK);
	if (ret) {
		ret = symlink("/system/fonts/DroidSansFallback.ttf",
			"/sec_storage/DroidSansFallback.ttf");
		if (ret) {
			tloge("symlink font fail\n");
			return -1;
		}
	}

	return 0;
}


int main(void)
{
	struct sec_storage_t *fs_control = NULL;
	struct misc_control_t *misc_control = NULL;
	pthread_t  th_fs = -1, th_misc = -1, th_ca_daemon = -1, th_multi_user_daemon = -1;
	int th_fs_create_flag = 0, th_multi_flag;
#ifdef TEE_SUPPORT_SOCKET
	struct socket_ctrl_t *socket_control = NULL;
	pthread_t th_socket = -1;
#endif

	tlogv("tee client daemon start\n");
	(void)pthread_create(&th_ca_daemon, NULL, thread_ca_server_work, NULL);
#ifdef WITH_DEVCHIP_PLATFORM
	th_fs_create_flag = 1;
#else
	if (0 < check_partition_ready("/sec_storage"))
		th_fs_create_flag = 1;
	else
		tlogi("check /sec_storage partition_ready failed ----------->\n");

#endif

	if (th_fs_create_flag == 1 && make_sec_storage_dir(SEC_STORAGE_DATA_DIR)) {
		tloge("make root dir failed\n");
		th_fs_create_flag = 0;
	}

	if (th_fs_create_flag == 1 &&
	    agent_init(AGENT_FS_ID, &m_fs_fd, (void **)(&fs_control))) {
		tloge("fs agent init failed\n");
		th_fs_create_flag = 0;
	}
#ifndef WITH_DEVCHIP_PLATFORM
	if (th_fs_create_flag == 1 &&
	    property_set(SFS_PROPERTY, SFS_PROPERTY_VALUE)) {
		tloge("property set error\n");
		th_fs_create_flag = 0;
	}

	tlogv("property set ok\n");
#endif
	if (agent_init(AGENT_MISC_ID, &m_misc_fd, (void **)(&misc_control))) {
		tloge("misc agent init failed\n");
		agent_exit(AGENT_FS_ID, m_fs_fd, (void*)fs_control);
		m_fs_fd = -1;
		fs_control = NULL;
		(void)fs_control;
		return -1;
	}

#ifndef WITH_DEVCHIP_PLATFORM
	link_ttf_file();
#endif
#ifdef TEE_SUPPORT_SOCKET
	if (agent_init(AGENT_SOCKET_ID, &g_socket_agent_fd, (void **)(&socket_control)) ||
	    socket_agent_init()) {
		tloge("socket agent init failed\n");
		agent_exit(AGENT_FS_ID, m_fs_fd, (void*)fs_control);
		m_fs_fd = -1;
		fs_control = NULL;
		(void)fs_control;
		agent_exit(AGENT_MISC_ID, m_misc_fd, (void*)misc_control);
		m_misc_fd = -1;
		misc_control = NULL;
		(void)misc_control;

		if (socket_control) {
			agent_exit(AGENT_SOCKET_ID, g_socket_agent_fd, (void*)socket_control);
			g_socket_agent_fd = -1;
			socket_control = NULL;
			(void)socket_control;

		}
		return -1;
	}

#endif

	/*
	 * register our signal handler, catch signal which default action is exit
	 */
	/* ignore SIGPIPE(happened when CA created socket then be killed),teecd will not restart.*/
	signal(SIGPIPE, SIG_IGN);
	/*for(i=0; i<NSIG; i++){
	    if(i==SIGHUP || i==SIGINT || i==SIGALRM || i==SIGTERM || i==SIGSEGV){
	        sig_ret = signal(i, crash_handler);
	        if(sig_ret == SIG_ERR)
	            tloge("setup SIGINT new handler failed\n");
	    }
	}*/

	if (th_fs_create_flag == 1) {
		(void)pthread_create(&th_fs, NULL, thread_fs_work, fs_control);
	}

	(void)pthread_create(&th_misc, NULL, thread_misc_work, misc_control);
	th_multi_flag = system_support_multi_users();
	if (th_multi_flag) {
		(void)pthread_create(&th_multi_user_daemon, NULL,
					thread_multi_user_server_work, NULL);
	}

#ifdef TEE_SUPPORT_SOCKET
	(void)pthread_create(&th_socket, NULL, thread_socket_work, socket_control);
#endif

	if (th_fs_create_flag == 1) {
		(void)pthread_join(th_fs, NULL);
	}

	(void)pthread_join(th_misc, NULL);
	(void)pthread_join(th_ca_daemon, NULL);

	if (th_multi_flag) {
		(void)pthread_join(th_multi_user_daemon, NULL);
	}

#ifdef TEE_SUPPORT_SOCKET
	(void)pthread_join(th_socket, NULL);
#endif

	agent_exit(AGENT_FS_ID, m_fs_fd, (void*)fs_control);
	m_fs_fd = -1;
	fs_control = NULL;
	(void)fs_control;

	agent_exit(AGENT_MISC_ID, m_misc_fd, (void*)misc_control);
	m_misc_fd = -1;
	misc_control = NULL;
	(void)misc_control;


#ifdef TEE_SUPPORT_SOCKET
	socket_agent_exit();
	agent_exit(AGENT_SOCKET_ID, g_socket_agent_fd, (void*)socket_control);
	g_socket_agent_fd = -1;
	socket_control = NULL;
	(void)socket_control;
#endif

	return 0;
}
