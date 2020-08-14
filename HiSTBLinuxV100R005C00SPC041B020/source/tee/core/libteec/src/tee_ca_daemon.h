/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2014
*------------------------------------------------------------------------------
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/
#ifndef _TEE_CA_DAEMON_H_
#define _TEE_CA_DAEMON_H_

#define TC_NS_SOCKET_NAME "#tc_ns_socket"

#define TEE_MULTIUSER_SOCKET_NAME "tee-multi-user"
#define SFS_PARTITION_USER_SYMLINK "sec_storage_data_users/"
#define SEC_STORAGE_DATA_USERS "/data/"SFS_PARTITION_USER_SYMLINK
#define SEC_STORAGE_DATA_USER_0 "/data/"SFS_PARTITION_USER_SYMLINK"0"

#define TRANS_BUFF_SIZE (4*1024)    /* agent transfer share buffer size */
#ifdef WITH_DEVCHIP_PLATFORM
#ifdef CFG_TEE_STORAGE_DIR
#define USER_DATA_DIR CFG_TEE_STORAGE_DIR
#else
#define USER_DATA_DIR "/data/"
#endif
#define SEC_STORAGE_ROOT_DIR    USER_DATA_DIR"/sec_storage/"
/*when reset to factory mode, clear the data in this patition*/
#define SEC_STORAGE_DATA_DIR    USER_DATA_DIR"/sec_storage_data/"
#else
/*when reset to factory mode, keep the data in this patition*/
#define SEC_STORAGE_ROOT_DIR    "/sec_storage/"
/*when reset to factory mode, clear the data in this patition*/
#define SEC_STORAGE_DATA_DIR    "/data/sec_storage_data/"
#define USER_DATA_DIR "/data/"
#endif
#define ROOT_DIR "/"
/* 0600 only root can read and write sec_storage folder */
#define ROOT_DIR_PERM   (S_IRUSR|S_IWUSR)
#define SEC_STORAGE_PARTITION "/dev/block/platform/hi_mci.0/by-name/securetystorage"
#define SFS_PARTITION_PERSISTENT "sec_storage/"
#define SFS_PARTITION_TRANSIENT "sec_storage_data/"

#define BUF_MAX_SIZE 4096

typedef struct {
	int magic;
	int status;
	int userid;
	int reserved;
} recv_msg_t;

#define MU_MSG_MAGIC 0xff00ff00

enum MU_MSG_Status {
	MU_MSG_STAT_NEW_USER = 0x01,
	MU_MSG_STAT_RM_USER = 0x02,
	MU_MSG_STAT_SWITCH_USER = 0x03,
};

enum MU_MSG_USERID {
	MU_MSG_USERID_OWNER = 0x0,
	MU_MSG_USERID_MAX = 0xFF,
};

#ifdef WITH_DEVCHIP_PLATFORM
#ifndef _ANDROID_PLATFORM_HISI 
/* User visible structure for SCM_CREDENTIALS message */
struct ucred
{
	pid_t pid;            /* PID of sending process.  */
	uid_t uid;            /* UID of sending process.  */
	gid_t gid;            /* GID of sending process.  */
};
#endif
#endif

int unlink_recursive(const char *name);
#ifndef WITH_DEVCHIP_PLATFORM
/*del by f00216599*/
int make_sec_storage_dir(char *storage_dir);
#endif
void *thread_multi_user_server_work(void *dummy);
void *thread_ca_server_work(void *dummy);

#endif
