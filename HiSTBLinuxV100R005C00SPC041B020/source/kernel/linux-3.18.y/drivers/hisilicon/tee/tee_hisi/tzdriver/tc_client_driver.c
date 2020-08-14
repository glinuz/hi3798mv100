/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : tc_client_driver.c
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2012.12.10
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/

#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_reserved_mem.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>
#if defined(CONFIG_DEVCHIP_PLATFORM) && defined(CONFIG_HISI_IOMMU_API)
#include <linux/hisi/tee_smmu.h>  /* add by yangwei   */
#endif
#include <linux/version.h>
#include <linux/ion.h>
#ifndef CONFIG_DEVCHIP_PLATFORM
//modify by y00201671:stb donot support ion
#include <linux/hisi/hisi_ion.h>
#endif
#include <crypto/hash.h>
#include <linux/hash.h>
#include <linux/crypto.h>
/*#define TC_DEBUG*/
#include "smc.h"
#include "tee_client_constants.h"
#include "tc_ns_client.h"
#include "teek_ns_client.h"
#include "agent.h"
#include "mem.h"
#include "tee_rdr_register.h"
#include "tui.h"
#include "gp_ops.h"
#include "teek_client_type.h"

#include <linux/thread_info.h>
#include <linux/highmem.h>
#include <linux/mm.h>

#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/scatterlist.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/security.h>
#include "libhwsecurec/securec.h"
#include "tck_authentication.h"

#include <linux/namei.h>

#define TEEC_PARAM_TYPES(param0Type, param1Type, param2Type, param3Type) \
	((param3Type) << 12 | (param2Type) << 8 | \
	 (param1Type) << 4 | (param0Type))

#define TEEC_PARAM_TYPE_GET(paramTypes, index) \
	(((paramTypes) >> (4*(index))) & 0x0F)

enum timer_class_type {
	/* timer event using timer10 */
	TIMER_GENERIC,
	/* timer event using RTC */
	TIMER_RTC
};

struct TEEC_timer_property {
	unsigned int type;
	unsigned int timer_id;
	unsigned int timer_class;
	unsigned int reserved2;
};

struct tc_notify_data {
	unsigned int dev_file_id;
	unsigned char uuid[16];
	unsigned int session_id;
	struct TEEC_timer_property property;
};

struct ion_client *drm_ion_client;

#define IRQ_TYPE_TP (1)		/*irq from touchscreen */
#define IRQ_TYPE_FP (2)		/*irq from fingerprint */
struct ts_tui_finger {
	int status;
	int x;
	int y;
	int area;
	int pressure;
	int orientation;
	int major;
	int minor;
	int event;
};

#define TS_TUI_MAX_FINGER (10)
struct ts_tui_fingers {
	struct ts_tui_finger fingers[TS_TUI_MAX_FINGER];
	int cur_finger_number;
	unsigned int gesture_wakeup_value;
	unsigned int special_button_key;
	unsigned int special_button_flag;
};

struct tp_notify_data_t {
	int irq_type;
	union {
		struct ts_tui_fingers tui_notify_data;
		int reserved[32];
	};
};

/**
	notify data
*/
#define NOTIFY_DATA_MAX_NUM  7
union notify_context {
	struct tc_notify_data notify_context_timer;
	char notify_context_tp[504];
};

struct notify_data_entry {
	uint32_t entry_type;
	uint32_t filled;
	union notify_context context;
};

struct reg_buf_st {
	__u64 file_buffer;
	uint32_t file_size;
	uint32_t reserved;
};

#define MAX_REGISTER_SIZE (10*sizeof(struct reg_buf_st))

enum notify_data_entry_type {
	NOTIFY_DATA_ENTRY_RESERVED,
	NOTIFY_DATA_ENTRY_TIMER,
	NOTIFY_DATA_ENTRY_TP,
	NOTIFY_DATA_ENTRY_MAX = 8
};

static void *g_notify_data;
static struct notify_data_entry *notify_data_entry_timer;
static struct notify_data_entry *notify_data_entry_tp;

static DEFINE_MUTEX(notify_data_lock);
static DEFINE_MUTEX(load_app_lock);
/*DTS2013030508974 q00209673 begin*/
static DEFINE_MUTEX(device_file_cnt_lock);
/*DTS2013030508974 q00209673 end */
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef CONFIG_IMAX_CIPHER
static DEFINE_MUTEX(general_srv_lock);
static DEFINE_MUTEX(general_srv_cmd_lock);
extern int general_srv_session_handle(TC_NS_DEV_File* dev_file, void* argp);
extern int general_srv_cmd_handle(void* argp);
#endif
#endif
/************global reference start***********/
static dev_t tc_ns_client_devt;
static struct class *driver_class;
static struct cdev tc_ns_client_cdev;
struct device_node *np;
static unsigned int device_file_cnt = 1;

struct TC_NS_DEV_List {
	unsigned int dev_file_cnt;
	/*DTS2013030508974 q00209673 begin*/
	struct mutex dev_lock;
	/*DTS2013030508974 q00209673 end */
	struct list_head dev_file_list;

};
static struct TC_NS_DEV_List g_tc_ns_dev_list;

struct TC_NS_Callback {
	unsigned char uuid[16];
	struct mutex callback_lock;
	void (*callback_func)(void *);
	struct list_head head;
};

struct TC_NS_Callback_List {
	unsigned int callback_count;
	struct mutex callback_list_lock;
	struct list_head callback_list;
};

static struct TC_NS_Callback_List g_ta_callback_func_list;

static struct task_struct *g_teecd_task;
static unsigned int agent_count;
/************global reference end*************/

typedef unsigned int (*rtc_timer_callback_func)(struct TEEC_timer_property *);
static int g_timer_type;

static struct crypto_shash *g_tee_shash_tfm;
static int tee_init_crypt_state;
static struct mutex g_tee_crypto_hash_lock;

static int tee_init_crypto(char *hash_type);

#define BUF_MAX_SIZE 1024
#define MAX_PATH_SIZE 512
#define SHA256_DIGEST_LENTH 32
static char ca_hash[SHA256_DIGEST_LENTH] = {0x59, 0xc0, 0xd6, 0x84,
					    0x8e, 0x52, 0x88, 0xc0,
					    0x6b, 0x22, 0xde, 0xba,
					    0x45, 0xac, 0x97, 0x37,
					    0xae, 0x55, 0x2a, 0x9e,
					    0xf4, 0xdb, 0xad, 0x2a,
					    0xc9, 0x24, 0x47, 0xf,
					    0x77, 0x91, 0x61, 0x49,
					   };

#define SYSTEM_SERVER "system_server"
#define APK_64_PROCESS_PATH "/data/dalvik-cache/arm64/system@framework@boot.oat"
#define APK_32_PROCESS_PATH "/data/dalvik-cache/arm/system@framework@boot.oat"
#ifndef CONFIG_DEVCHIP_PLATFORM
/* factory version img apk dentry path is as belows */
#define APK_64_PROCESS_PATH_FAC "/system/framework/arm64/boot.oat"
#define APK_32_PROCESS_PATH_FAC "/system/framework/arm/boot.oat"
#endif
static char *get_process_path(struct task_struct *task, char *tpath)
{
	char *ret_ptr = NULL;
	struct vm_area_struct *vma = NULL;
	struct path base_path = {0};
	bool find_path = false;
	errno_t sret;

	if (NULL == tpath || NULL == task)
		return NULL;

	sret = memset_s(tpath, MAX_PATH_SIZE, '\0', MAX_PATH_SIZE);
	if (EOK != sret) {
		TCERR("memset_s error sret is %d\n", sret);
		return NULL;
	}

	task_lock(task);
	if (task->mm && task->mm->mmap) {
		vma = task->mm->mmap;
	} else {
		task_unlock(task);
		return NULL;
	}

	while (vma) {
		if ((vma->vm_flags & VM_EXEC) && vma->vm_file) {
			base_path = vma->vm_file->f_path;
			find_path = true;
			break;
		}
		vma = vma->vm_next;
	}
	task_unlock(task);
	if (find_path)
		ret_ptr = d_path(&base_path, tpath, MAX_PATH_SIZE);

	return ret_ptr;
}


static int calc_teecd_path_hash(unsigned char *data, unsigned long len, char *digest)
{
	int rc;
	struct sdesc {
		struct shash_desc shash;
		char ctx[];
	};
	struct sdesc *desc;

	if (tee_init_crypto("sha256")) {
		TCERR("init tee crypto failed\n");
		return -EFAULT;
	}

	desc = kmalloc(sizeof(struct shash_desc)
			+ crypto_shash_descsize(g_tee_shash_tfm), GFP_KERNEL);
	if (!desc) {
		TCERR("alloc desc failed\n");
		return -ENOMEM;
	}

	desc->shash.tfm = g_tee_shash_tfm;
	desc->shash.flags = 0;

	rc = crypto_shash_digest(&desc->shash, data, len, digest);

	kfree(desc);

	return rc;
}

static int check_teecd_access(struct task_struct *ca_task)
{
	char *ca_cert;
	char *path;
	char digest[SHA256_DIGEST_LENTH] = {0};
	const struct cred *cred;
	int message_size;
	int ret = 0;
	char *tpath;

	if (NULL == ca_task) {
		TCERR("task_struct is NULL\n");
		return -EPERM;
	}

	cred = get_task_cred(ca_task);
	if (NULL == cred) {
		TCERR("cred is NULL\n");
		return -EPERM;

	}

	tpath = kmalloc(MAX_PATH_SIZE, GFP_KERNEL);
	if (NULL == tpath) {
		TCERR("tpath kmalloc fail\n");
		return -EPERM;
	}

	ca_cert = kmalloc(BUF_MAX_SIZE, GFP_KERNEL);
	if (NULL == ca_cert) {
		TCERR("ca_cert kmalloc fail\n");
		kfree(tpath);
		return -EPERM;
	}

	path = get_process_path(ca_task, tpath);
	if (!IS_ERR_OR_NULL(path)) {
		errno_t sret;

		sret = memset_s(ca_cert, BUF_MAX_SIZE, 0x00, BUF_MAX_SIZE);
		if (EOK != sret) {
			TCERR("memset_s error sret is %d\n", sret);
			kfree(tpath);
			kfree(ca_cert);
			return -EPERM;
		}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
		message_size = snprintf_s(ca_cert, BUF_MAX_SIZE - 1,
				BUF_MAX_SIZE - 1, "%s%s%u", ca_task->comm, path,
				cred->uid.val);
#else
		message_size = snprintf_s(ca_cert, BUF_MAX_SIZE - 1,
				BUF_MAX_SIZE - 1, "%s%s%u", ca_task->comm, path,
				cred->uid);
#endif

		if (message_size > 0) {
			ret =  calc_teecd_path_hash(ca_cert, message_size, digest);
			if (!ret) {
				if (!memcmp(digest, ca_hash,
							SHA256_DIGEST_LENTH)) {
					kfree(tpath);
					kfree(ca_cert);

					return 0;
				}
			}
		}
	}

	kfree(tpath);
	kfree(ca_cert);

	return -EPERM;
}

static int check_package_name(TC_NS_DEV_File *dev_file)
{
	int ret = -1;
	struct path path;
	char *real_path = NULL;
	char *realpath_buf = NULL;
	char *dentrypath_buf = NULL;
	char *ca_dentry_path = NULL;
	errno_t sret;

	sret = memset_s((void *)&path, sizeof(struct path), 0, sizeof(struct path));
	if (EOK != sret) {
		TCERR("memset_s error sret is %d.\n", sret);
		return -1;
	}

	if (TEE_REQ_FROM_USER_MODE == dev_file->kernel_api) {
		/* get real path from cmdline */
		realpath_buf = kzalloc((size_t)(MAX_PATH_SIZE), GFP_KERNEL);
		if (NULL == realpath_buf) {
			TCERR("realpath_buf kzalloc fail\n");
			return -1;
		}
		ret = kern_path((const char *)(dev_file->pkg_name), LOOKUP_FOLLOW, &path);
		if (!ret) {
			real_path = d_path(&path, realpath_buf, MAX_PATH_SIZE);
			if (IS_ERR(real_path)) {
				TCERR("failed to get real path of %s, err is %ld\n",
						dev_file->pkg_name, PTR_ERR(real_path));
				kfree(realpath_buf);
				return -1;
			}
		} else {
			TCVERBOSE("kern_path return is ret %d, cmdline is %s\n",
					ret, dev_file->pkg_name);
		}

		/* get current process dentry */
		dentrypath_buf = kzalloc((size_t)(MAX_PATH_SIZE), GFP_KERNEL);
		if (NULL == dentrypath_buf) {
			TCERR("dentrypath_buf kmalloc fail\n");
			kfree(realpath_buf);
			return -1;
		}
		ca_dentry_path = get_process_path(current, dentrypath_buf);

		/* cmp the process dentry with real path*/
		if (!IS_ERR_OR_NULL(ca_dentry_path)) {
			if (strlen(SYSTEM_SERVER) == dev_file->pkg_name_len
					&& strlen(APK_64_PROCESS_PATH) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_64_PROCESS_PATH,
						strlen(APK_64_PROCESS_PATH))
					&& !dev_file->pub_key_len) {
				ret = 0;
#ifndef CONFIG_DEVCHIP_PLATFORM
			} else if (strlen(SYSTEM_SERVER) == dev_file->pkg_name_len
					&& strlen(APK_64_PROCESS_PATH_FAC) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_64_PROCESS_PATH_FAC,
						strlen(APK_64_PROCESS_PATH_FAC))
					&& !dev_file->pub_key_len) {
				ret = 0;
#endif
			} else if (strlen(APK_64_PROCESS_PATH) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_64_PROCESS_PATH,
						strlen(APK_64_PROCESS_PATH))
					&& dev_file->pub_key_len) {
				ret = 0;
#ifndef CONFIG_DEVCHIP_PLATFORM
			} else if (strlen(APK_64_PROCESS_PATH_FAC) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_64_PROCESS_PATH_FAC,
						strlen(APK_64_PROCESS_PATH_FAC))
					&& dev_file->pub_key_len) {
				ret = 0;
#endif
			} else if (strlen(APK_32_PROCESS_PATH) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_32_PROCESS_PATH,
						strlen(APK_32_PROCESS_PATH))
					&& dev_file->pub_key_len) {
				ret = 0;
#ifndef CONFIG_DEVCHIP_PLATFORM
			} else if (strlen(APK_32_PROCESS_PATH_FAC) == strlen(ca_dentry_path)
					&& !memcmp(ca_dentry_path, APK_32_PROCESS_PATH_FAC,
						strlen(APK_32_PROCESS_PATH_FAC))
					&& dev_file->pub_key_len) {
				ret = 0;
#endif
			} else if (real_path && strlen(ca_dentry_path) == strlen(real_path)
					&& !memcmp(ca_dentry_path, real_path, strlen(real_path))
					&& !dev_file->pub_key_len) {
				ret = 0;
			} else {
				ret = -1;
			}
			if (ret) {
				if (real_path) {
					TCERR("error real_path is %s, len is %ld\n",
							real_path, strlen(real_path));
				}
				TCERR("error ca_dentry_path is %s, len is %ld\n",
						ca_dentry_path, strlen(ca_dentry_path));
				TCERR("error package name  is %s, len is %d\n",
						dev_file->pkg_name, dev_file->pkg_name_len);
				TCERR("dev_file->pub_key_len is %d\n", dev_file->pub_key_len);
				kfree(dentrypath_buf);
				kfree(realpath_buf);
				return -1;
			} else {
				TCDEBUG("success:package name is right:\n");
				kfree(dentrypath_buf);
				kfree(realpath_buf);
				return 0;
			}
		} else {
			TCERR("get process ca_dentry_path error %ld\n",
					PTR_ERR(ca_dentry_path));
			kfree(dentrypath_buf);
			kfree(realpath_buf);
			return -1;
		}
	} else {
		return 0;
	}
}

struct notify_data_entry *acquire_notify_data_entry(int entry_type)
{
	/**every notify data entry is 512bytes;
	   total notify data mem is 4K(one page)*/
	int i = 0;
	struct notify_data_entry *ptr =
		(struct notify_data_entry *) g_notify_data;

	while (i < NOTIFY_DATA_MAX_NUM) {
		if (NOTIFY_DATA_ENTRY_RESERVED == ptr->entry_type) {
			ptr->entry_type = entry_type;
			ptr->filled = 0;
			break;
		}
		ptr++;
		i++;
	}
	return ptr;
}


static void TST_get_timer_type(int *type)
{
	*type = g_timer_type;
}


TC_NS_Service *tc_find_service(struct list_head *services, char *uuid)
{
	TC_NS_Service *service = NULL;

	/*need service init or not */
	list_for_each_entry(service, services, head) {
		if (0 == memcmp(service->uuid, uuid, sizeof(service->uuid)))
			return service;
	}

	return NULL;
}


TC_NS_Session *tc_find_session(struct list_head *session_list,
			       unsigned int session_id)
{
	TC_NS_Session *session = NULL;

	list_for_each_entry(session, session_list, head) {
		if (session->session_id == session_id)
			return session;
	}

	return NULL;
}


static int close_session(TC_NS_DEV_File *dev, unsigned char *uuid,
			 unsigned int session_id)
{
	TC_NS_ClientContext context;
	int ret = 0;
	errno_t sret;

	sret = memset_s(&context, sizeof(TC_NS_ClientContext), 0,
			sizeof(TC_NS_ClientContext));
	if (EOK != sret)
		return TEEC_ERROR_GENERIC;

	sret = memcpy_s(context.uuid, sizeof(context.uuid), uuid,
			sizeof(context.uuid));
	if (EOK != sret)
		return TEEC_ERROR_GENERIC;

	context.session_id = session_id;
	context.cmd_id = GLOBAL_CMD_ID_CLOSE_SESSION;

	ret = tc_client_call(&context, dev, TC_CALL_GLOBAL | TC_CALL_SYNC);
	if (ret)
		TCERR("close session failed, ret=0x%x\n", ret);

	return ret;
}


static int kill_session(TC_NS_DEV_File *dev, unsigned char *uuid,
			unsigned int session_id)
{
	TC_NS_ClientContext context;
	int ret = 0;
	errno_t sret;

	sret = memset_s(&context, sizeof(TC_NS_ClientContext), 0,
			sizeof(TC_NS_ClientContext));
	if (EOK != sret)
		return TEEC_ERROR_GENERIC;

	sret = memcpy_s(context.uuid, sizeof(context.uuid), uuid,
			sizeof(context.uuid));
	if (EOK != sret)
		return TEEC_ERROR_GENERIC;

	context.session_id = session_id;
	context.cmd_id = GLOBAL_CMD_ID_KILL_TASK;
	TCDEBUG("dev_file_id=%d\n", dev->dev_file_id);

	/*do clear work in agent */
	tee_agent_clear_work(&context, dev->dev_file_id);

	ret = tc_client_call(&context, dev, TC_CALL_GLOBAL | TC_CALL_SYNC);
	if (ret)
		TCERR("close session failed, ret=0x%x\n", ret);

	return ret;
}


#ifdef TC_ASYNC_NOTIFY_SUPPORT
static void tc_notify_fn(struct work_struct *dummy)
{
	TC_NS_DEV_File *temp_dev_file;
	TC_NS_Service *temp_svc;
	TC_NS_Session *temp_ses = NULL;
	int enc_found = 0;
	rtc_timer_callback_func callback_func;
	struct TC_NS_Callback *callback_func_t;
	struct tp_notify_data_t *tp_nofity_data;
	struct tc_notify_data *tc_notify_data_timer =
		&(notify_data_entry_timer->context.notify_context_timer);

	TCDEBUG("step into ipi_secure_notify\n");
	if (notify_data_entry_tp != NULL && notify_data_entry_tp->filled) {
		tp_nofity_data =
			(struct tp_notify_data_t *)(&notify_data_entry_tp
					->context.notify_context_tp);
		if (IRQ_TYPE_FP == tp_nofity_data->irq_type) {
			notify_data_entry_tp->filled = 0;
#if defined(CONFIG_TEE_TUI)
			tui_fp_notify();
#endif
			return;
		}
	}

	if (NULL == notify_data_entry_timer) {
		TCERR("notify_data is NULL\n");
		/*TODO: send tp to touch driver */
		return;
	}
	if (!notify_data_entry_timer->filled) {
		TCDEBUG("notify_data is not filled\n");
		return;
	}

	notify_data_entry_timer->filled = 0;

	TC_TIME_DEBUG("notify_data timer type is 0x%x, timer ID is 0x%x\n",
		      tc_notify_data_timer->property.type,
		      tc_notify_data_timer->property.timer_id);

	mutex_lock(&g_ta_callback_func_list.callback_list_lock);
	list_for_each_entry(callback_func_t,
			    &g_ta_callback_func_list.callback_list, head) {
		if (0 == memcmp(callback_func_t->uuid,
				tc_notify_data_timer->uuid, 16)) {
			if (TIMER_RTC ==
				tc_notify_data_timer->property.timer_class) {
				TC_TIME_DEBUG("start to call callback func\n");
				callback_func =	(rtc_timer_callback_func)
					(callback_func_t->callback_func);
				(void)(callback_func)
					(&(tc_notify_data_timer->property));
				TC_TIME_DEBUG("end to call callback func\n");
			} else if (TIMER_GENERIC ==
				tc_notify_data_timer->property.timer_class) {
				TC_TIME_DEBUG("timer60 no callback func\n");
			}
		}
	}
	mutex_unlock(&g_ta_callback_func_list.callback_list_lock);

	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	list_for_each_entry(temp_dev_file,
			    &g_tc_ns_dev_list.dev_file_list, head) {
		TCDEBUG("dev file id1 = %d, id2 = %d\n",
			temp_dev_file->dev_file_id,
			tc_notify_data_timer->dev_file_id);
		if (temp_dev_file->dev_file_id ==
		    tc_notify_data_timer->dev_file_id) {
			temp_svc =
				tc_find_service(&temp_dev_file->services_list,
						tc_notify_data_timer->uuid);
			if (temp_svc) {
				temp_ses =
					tc_find_session(&temp_svc->session_list,
							tc_notify_data_timer->
							session_id);
				if (temp_ses) {
					TCDEBUG("send cmd ses id %d\n",
						temp_ses->session_id);
					enc_found = 1;
					break;
				}
				break;
			}
			break;
		}

	}
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	if (TIMER_GENERIC == tc_notify_data_timer->property.timer_class) {
		TC_TIME_DEBUG("timer60 wake up event\n");
		if (enc_found && temp_ses) {
			temp_ses->wait_data.send_wait_flag = 1;
			wake_up_interruptible(&temp_ses->wait_data.send_cmd_wq);
		}
	} else {
		TC_TIME_DEBUG("RTC do not need to wakeup\n");
	}
}

static DECLARE_WORK(tc_notify_work, tc_notify_fn);

static irqreturn_t tc_secure_notify(int irq, void *dev_id)
{
	struct tp_notify_data_t *tp_nofity_data;

	if (notify_data_entry_tp != NULL && notify_data_entry_tp->filled) {
		tp_nofity_data = (struct tp_notify_data_t *)
			(&notify_data_entry_tp->context.notify_context_tp);
		if (IRQ_TYPE_TP == tp_nofity_data->irq_type) {
#if defined(CONFIG_TEE_TUI)
			ts_tui_report_input((void *)&tp_nofity_data->
					    tui_notify_data);
#endif
			notify_data_entry_tp->filled = 0;
			/*TODO: send tp to touch driver*/
		} else if (IRQ_TYPE_FP == tp_nofity_data->irq_type) {
			schedule_work(&tc_notify_work);
			notify_data_entry_tp->filled = 0;
#if defined(CONFIG_TEE_TUI)
			tui_fp_notify();
#endif
		} else
			TCDEBUG("invalid irq type\n");
	} else {
		schedule_work(&tc_notify_work);
	}
	isb();
	wmb();
	tc_smc_wakeup();
	return IRQ_HANDLED;
}

#endif

static int TC_NS_ServiceInit(TC_NS_DEV_File *dev_file, unsigned char *uuid,
			     TC_NS_Service **new_service)
{
	int ret = 0;
	TC_NS_Service *service = NULL;
	errno_t sret;

	service = kzalloc(sizeof(TC_NS_Service), GFP_KERNEL);
	if (!service) {
		TCERR("kmalloc failed\n");
		ret = -ENOMEM;
		return ret;
	}

	sret = memcpy_s(service->uuid, sizeof(service->uuid), uuid,
			sizeof(service->uuid));
	if (EOK != sret) {
		kfree(service);
		return -ENOMEM;
	}
	dev_file->service_cnt++;
	INIT_LIST_HEAD(&service->session_list);
	mutex_init(&service->session_lock);
	mutex_lock(&dev_file->service_lock);
	list_add_tail(&service->head, &dev_file->services_list);
	mutex_unlock(&dev_file->service_lock);
	*new_service = service;

	return ret;
}

uint32_t TC_NS_get_uid(void)
{
	struct task_struct *task = current;
	const struct cred *cred = NULL;
	uint32_t uid = 0;

	cred = get_task_cred(task);

	if (!cred) {
		TCERR("failed to get uid of the task\n");
		return -1;
	}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
	uid = cred->uid.val;
#else
	uid = cred->uid;
#endif

	TCDEBUG("current uid is %d\n", uid);
	return uid;
}

/* the input param should be "sha256" */
static int tee_init_crypto(char *hash_type)
{
	long rc = 0;

	if (NULL == hash_type) {
		TCERR("tee init crypto: error input parameter.\n");
		return -5;
	}
	mutex_lock(&g_tee_crypto_hash_lock);
	if (tee_init_crypt_state) {
		mutex_unlock(&g_tee_crypto_hash_lock);
		return 0;
	}

	g_tee_shash_tfm = crypto_alloc_shash(hash_type, 0, 0);
	if (IS_ERR(g_tee_shash_tfm)) {
		rc = PTR_ERR(g_tee_shash_tfm);
		pr_err("Can not allocate %s (reason: %ld)\n", hash_type, rc);
		mutex_unlock(&g_tee_crypto_hash_lock);
		return rc;
	}
	tee_init_crypt_state = 1;
	mutex_unlock(&g_tee_crypto_hash_lock);

	return 0;
}


/* Calculate the SHA256 file digest */
static int tee_calc_task_hash(unsigned char *digest)
{
	unsigned long start_code, end_code, code_size, in_size;
	void *ptr_base = NULL;
	struct page *ptr_page = NULL;
	int rc;
	struct {
		struct shash_desc shash;
		char ctx[crypto_shash_descsize(g_tee_shash_tfm)];
	} desc;

	if (NULL == digest) {
		TCERR("tee hash: input param is error!\n");
		return -2;
	}

	start_code = current->mm->start_code;
	end_code   = current->mm->end_code;
	code_size = end_code - start_code;
	TCDEBUG("code_size = %lu, start_code = %lu, end_code = %lu\n",
		code_size, start_code, end_code);

	desc.shash.tfm = g_tee_shash_tfm;
	desc.shash.flags = 0;

	rc = crypto_shash_init(&desc.shash);
	if (rc != 0)
		return rc;

	while (start_code < end_code) {
		rc = get_user_pages_fast(start_code, 1, 0, &ptr_page);
		if (rc != 1) {
			TCERR("get user pages error[0x%x]\n", rc);
			rc = -EFAULT;
			break;
		}

		ptr_base = kmap_atomic(ptr_page);
		if (NULL == ptr_base) {
			rc = -3;
			put_page(ptr_page);
			break;
		}

		in_size = (code_size > PAGE_SIZE) ? PAGE_SIZE : code_size;
		rc = crypto_shash_update(&desc.shash, ptr_base, in_size);
		if (rc) {
			kunmap_atomic(ptr_base);
			put_page(ptr_page);
			break;
		}
		kunmap_atomic(ptr_base);
		put_page(ptr_page);

		start_code += in_size;
		code_size = end_code - start_code;
	}
	if (!rc)
		rc = crypto_shash_final(&desc.shash, digest);

	return rc;
}

/* Modify the client context so params id 2 and 3 contain temp pointers to the
 * public key and package name for the open session. This is used for the
 * TEEC_LOGIN_IDENTIFY open session method */
static int set_login_information(TC_NS_DEV_File *dev_file,
				 TC_NS_ClientContext *context)
{
	/* The daemon has failed to get login information or not supplied */
	if (0 == dev_file->pkg_name_len)
		return -1;
#ifndef CONFIG_DEVCHIP_PLATFORM
	if (check_package_name(dev_file)) {
		TCERR("package name(%s) not correct\n", dev_file->pkg_name);
		return -1;
    }
#endif
	/* The 3rd parameter buffer points to the pkg name buffer in the
	* device file pointer */
	/* get package name len and package name */
	context->params[3].memref.size_addr = (__u64)&dev_file->pkg_name_len;
	context->params[3].memref.buffer = (__u64)dev_file->pkg_name;

	/* Set public key len and public key */
	if (dev_file->pub_key_len != 0) {
		context->params[2].memref.size_addr =
			(__u64)&dev_file->pub_key_len;
		context->params[2].memref.buffer = (__u64) dev_file->pub_key;
	} else {
		/* If get public key failed, then get uid in kernel */
		uint32_t ca_uid = TC_NS_get_uid();

		if (-1 == ca_uid) {
			TCERR("Failed to get uid of the task\n");
			goto error;
		}

		dev_file->pub_key_len = sizeof(ca_uid);
		context->params[2].memref.size_addr =
			(__u64)&dev_file->pub_key_len;

		if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, &ca_uid,
					dev_file->pub_key_len)) {
			TCERR("Failed to copy pubkey, pub_key_len=%u\n",
					dev_file->pub_key_len);
			goto error;
		}
		context->params[2].memref.buffer = (__u64) dev_file->pub_key;
	}

	/* Now we mark the 2 parameters as input temp buffers */
	context->paramTypes =
		TEEC_PARAM_TYPES(
				TEEC_PARAM_TYPE_GET(context->paramTypes, 0),
				TEEC_PARAM_TYPE_GET(context->paramTypes, 1),
				TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);

	return 0;
error:
	return -1;
}


int TC_NS_RegisterServiceCallbackFunc(char *uuid, void *func,
				      void *private_data)
{
	struct TC_NS_Callback *callback_func = NULL;
	struct TC_NS_Callback *new_callback = NULL;
	int ret = 0;
	errno_t sret;

	if (NULL == uuid || NULL == func)
		return -EINVAL;

	mutex_lock(&g_ta_callback_func_list.callback_list_lock);
	list_for_each_entry(callback_func,
			&g_ta_callback_func_list.callback_list, head) {
		if (0 == memcmp(callback_func->uuid, uuid, 16)) {
			callback_func->callback_func = (void (*)(void *))func;
			TCDEBUG("succeed to find uuid ta_callback_func_list\n");
			goto find_callback;
		}
	}

	/*create a new callback struct if we couldn't find it in list */
	new_callback = kzalloc(sizeof(struct TC_NS_Callback), GFP_KERNEL);
	if (!new_callback) {
		TCERR("kmalloc failed\n");
		ret = -ENOMEM;
		goto find_callback;
	}

	sret = memcpy_s(new_callback->uuid, 16, uuid, 16);
	if (EOK != sret) {
		kfree(new_callback);
		ret = -ENOMEM;
		goto find_callback;
	}
	g_ta_callback_func_list.callback_count++;
	TCDEBUG("ta_callback_func_list.callback_count is %d\n",
		g_ta_callback_func_list.callback_count);
	INIT_LIST_HEAD(&new_callback->head);
	new_callback->callback_func = (void (*)(void *))func;
	mutex_init(&new_callback->callback_lock);
	list_add_tail(&new_callback->head,
		      &g_ta_callback_func_list.callback_list);

find_callback:
	mutex_unlock(&g_ta_callback_func_list.callback_list_lock);
	return ret;
}
EXPORT_SYMBOL(TC_NS_RegisterServiceCallbackFunc);

#ifdef TC_ASYNC_NOTIFY_SUPPORT
void timer_callback_func(struct TEEC_timer_property *timer_property)
{
	TC_TIME_DEBUG
	("timer_property->type = %x, timer_property->timer_id = %x\n",
	 timer_property->type, timer_property->timer_id);
	g_timer_type = (int)timer_property->type;
}

static int TC_NS_register_notify_data_memery(void)
{

	TC_NS_SMC_CMD smc_cmd = { 0 };
	int ret;
	unsigned char uuid[17] = { 0 };
	TC_NS_Operation operation = { 0 };

	operation.paramTypes =
		TEE_PARAM_TYPE_VALUE_INPUT | TEE_PARAM_TYPE_VALUE_INPUT << 4;
	operation.params[0].value.a = virt_to_phys(g_notify_data);
	operation.params[0].value.b = virt_to_phys(g_notify_data) >> 32;
	operation.params[1].value.a = SZ_4K;

	uuid[0] = 1;
	smc_cmd.uuid_phys = virt_to_phys((void *)uuid);
	smc_cmd.uuid_h_phys = virt_to_phys((void *)uuid) >> 32;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_REGISTER_NOTIFY_MEMORY;
	smc_cmd.operation_phys = virt_to_phys(&operation);
	smc_cmd.operation_h_phys = virt_to_phys(&operation) >> 32;

	TCDEBUG("cmd. context_phys:%x\n", smc_cmd.context_id);
	ret = TC_NS_SMC(&smc_cmd, 0);

	return ret;
}

static int TC_NS_unregister_notify_data_memory(void)
{

	TC_NS_SMC_CMD smc_cmd = { 0 };
	int ret;
	unsigned char uuid[17] = { 0 };
	TC_NS_Operation operation = { 0 };

	operation.paramTypes =
		TEE_PARAM_TYPE_VALUE_INPUT | TEE_PARAM_TYPE_VALUE_INPUT << 4;
	operation.params[0].value.a = virt_to_phys(g_notify_data);
	operation.params[0].value.b = virt_to_phys(g_notify_data) >> 32;
	operation.params[1].value.a = SZ_4K;

	uuid[0] = 1;
	smc_cmd.uuid_phys = virt_to_phys((void *)uuid);
	smc_cmd.uuid_h_phys = virt_to_phys((void *)uuid) >> 32;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_UNREGISTER_NOTIFY_MEMORY;
	smc_cmd.operation_phys = virt_to_phys(&operation);
	smc_cmd.operation_h_phys = virt_to_phys(&operation) >> 32;
	TCDEBUG("cmd. context_phys:%x\n", smc_cmd.context_id);

	ret = TC_NS_SMC(&smc_cmd, 0);

	return ret;
}
#endif


static void callback_demo_main(char *uuid)
{
	int ret = 0;

	TC_TIME_DEBUG("step into callback_demo_main\n");

	ret = TC_NS_RegisterServiceCallbackFunc(uuid,
						(void *)&timer_callback_func,
						NULL);
	if (ret != 0)
		TCERR("failed to TC_NS_RegisterServiceCallbackFunc\n");
}


static int TC_NS_Client_Login(TC_NS_DEV_File *dev_file, void __user *buffer)
{
	int ret = -EINVAL;
	uint8_t *cert_buffer, *buf;
	errno_t sret;

	if (dev_file->login_setup) {
		TCERR("Login information cannot be set twice!\n");
		return -EINVAL;
	}

	dev_file->login_setup = true;

	if (NULL == buffer) {
		/* We accept no debug information because the daemon might
		 * have failed */
		TCDEBUG("No debug information\n");
		dev_file->pkg_name_len = 0;
		dev_file->pub_key_len = 0;
		return 0;
	}

#define CERT_BUFFER_SIZE (MAX_PACKAGE_NAME_LEN + MAX_PUBKEY_LEN +\
			  sizeof(dev_file->pkg_name_len) + \
			  sizeof(dev_file->pub_key_len))
	buf = cert_buffer = kmalloc(CERT_BUFFER_SIZE, GFP_KERNEL);
	if (!cert_buffer) {
		TCERR("Failed to allocate login buffer!");
		return -EFAULT;
	}

	/* GET PACKAGE NAME AND APP CERTIFICATE:
	 * The proc_info format is as follows:
	 * package_name_len(4 bytes) || package_name ||
	 * apk_cert_len(4 bytes) || apk_cert.
	 * or package_name_len(4 bytes) || package_name
	 * || exe_uid_len(4 bytes) || exe_uid.
	 * The apk certificate format is as follows:
	 * modulus_size(4bytes) ||modulus buffer
	 * || exponent size || exponent buffer
	 */

	/* get package name len and package name */
	if (copy_from_user(cert_buffer, buffer, CERT_BUFFER_SIZE)) {
		TCERR("Failed to get user login info!\n");
		ret = -EINVAL;
		goto error;
	}

	sret = memcpy_s(&dev_file->pkg_name_len, sizeof(dev_file->pkg_name_len),
			cert_buffer, sizeof(dev_file->pkg_name_len));
	if (EOK != sret) {
		ret = -ENOMEM;
		goto error;
	}
	TCDEBUG("package_name_len is %u\n", dev_file->pkg_name_len);

	if (0 == dev_file->pkg_name_len ||
	    dev_file->pkg_name_len >= MAX_PACKAGE_NAME_LEN) {
		TCERR("Invalid size of package name len login info!\n");
		ret = -EINVAL;
		goto error;
	}

	cert_buffer += sizeof(dev_file->pkg_name_len);
	sret = strncpy_s(dev_file->pkg_name, MAX_PACKAGE_NAME_LEN, cert_buffer,
			dev_file->pkg_name_len);
	if (EOK != sret) {
		ret = -ENOMEM;
		goto error;
	}
	TCDEBUG("package name is %s\n", dev_file->pkg_name);

	cert_buffer += dev_file->pkg_name_len;

	/* get public key len and public key */
	sret = memcpy_s(&dev_file->pub_key_len, sizeof(dev_file->pub_key_len),
			cert_buffer, sizeof(dev_file->pub_key_len));
	if (EOK != sret) {
		ret = -ENOMEM;
		goto error;
	}
	TCDEBUG("publick_key_len is %d\n", dev_file->pub_key_len);

	if (dev_file->pub_key_len > MAX_PUBKEY_LEN) {
		TCERR("Invalid public key length in login info!\n");
		ret = -EINVAL;
		goto error;
	}

	if (dev_file->pub_key_len != 0) {
		cert_buffer += sizeof(dev_file->pub_key_len);
		TCDEBUG("public_key first 4 bytes %x,%x,%x,%x\n",
			cert_buffer[0], cert_buffer[1], cert_buffer[2],
			cert_buffer[3]);
		if (memcpy_s(dev_file->pub_key, MAX_PUBKEY_LEN, cert_buffer,
					dev_file->pub_key_len)) {
			TCERR("Failed to copy cert, pub_key_len=%u\n",
					dev_file->pub_key_len);
			ret = -EINVAL;
			goto error;
		}
		cert_buffer += dev_file->pub_key_len;
	}

	ret = 0;
error:
	kfree(buf);
	return ret;
}

int TC_NS_OpenSession(TC_NS_DEV_File *dev_file, TC_NS_ClientContext *context)
{
	int ret = -EINVAL;
	TC_NS_Service *service = NULL;
	TC_NS_Session *session = NULL;
	uint8_t flags = TC_CALL_GLOBAL;
	unsigned char *hash_buf;

	if (!dev_file || !context) {
		TCERR("invalid dev_file or context\n");
		return ret;
	}
	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	service = tc_find_service(&dev_file->services_list, context->uuid);

	/* Need service init or not */
	if (service)
		goto find_service;

	/* Create a new service if we couldn't find it in list */
	ret = TC_NS_ServiceInit(dev_file, context->uuid, &service);
	if (ret)
		TCERR("service init failed");
	else
		goto find_service;

	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	TCERR("find service failed\n");

	return ret;

find_service:
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	session = kzalloc(sizeof(TC_NS_Session), GFP_KERNEL);
	if (!session) {
		TCERR("kmalloc failed\n");
		ret = -ENOMEM;
		return ret;
	}

	if (TEEC_LOGIN_IDENTIFY == context->login.method) {
		TCDEBUG("login method is IDENTIFY\n");

		/* Check if params 0 and 1 are valid */
		if ((TEE_REQ_FROM_USER_MODE == dev_file->kernel_api) &&
		    (tc_user_param_valid(context, 0) ||
		     tc_user_param_valid(context, 1))) {
			ret = -EFAULT;
			goto error;
		}

		ret = set_login_information(dev_file, context);
		if (ret != 0) {
			TCERR("set_login_information failed ret =%d\n", ret);
			goto error;
		}

		flags |= TC_CALL_LOGIN;
	} else {
		TCDEBUG("login method is not supported\n");
		ret = -EINVAL;
		goto error;
	}

	context->cmd_id = GLOBAL_CMD_ID_OPEN_SESSION;
	mutex_init(&session->ta_session_lock);

	hash_buf = kzalloc((size_t)MAX_SHA_256_SZ, GFP_KERNEL);
	if (NULL == hash_buf) {
		TCERR("malloc 32 bytes hash mem failed\n");
		ret = -ENOMEM;
		goto error;
	}

	if (EOK != memset_s(hash_buf, MAX_SHA_256_SZ, 0, MAX_SHA_256_SZ)) {
		TCERR("init hash mem failed\n");
		kfree(hash_buf);
		ret = -EFAULT;
		goto error;
	}

#ifndef CONFIG_DEVCHIP_PLATFORM
	if (current->mm) {
		if (tee_init_crypto("sha256")) {
			TCERR("init code hash error!!!\n");
			kfree(hash_buf);
			ret = -EFAULT;
			goto error;
		}

		if (tee_calc_task_hash(hash_buf)) {
			TCERR("tee calc task hash failed\n");
			kfree(hash_buf);
			ret = -EFAULT;
			goto error;
		}
	}
#endif
	mutex_lock(&g_tc_ns_dev_list.dev_lock);

	/*cp hash_buf to global var, it is protected by lock */
	ret = memcpy_s(g_ca_auth_hash_buf, (size_t)MAX_SHA_256_SZ,
			hash_buf, (size_t)MAX_SHA_256_SZ);
	kfree(hash_buf);
	if (ret) {
		TCERR("memcpy_s to g_hash_buf failed\n");
		mutex_unlock(&g_tc_ns_dev_list.dev_lock);
		ret = -ENOMEM;
		goto error;
	}
	/*send smc */
	ret = tc_client_call(context, dev_file, flags);
	if (sizeof(uint32_t) == dev_file->pub_key_len)
		dev_file->pub_key_len = 0;
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);

	if (ret != 0) {
		TCERR("smc_call returns error, ret=0x%x\n", ret);
		goto error;
	} else
		TCDEBUG("smc_call returns right\n");

	session->session_id = context->session_id;
	TCDEBUG("session id is %x\n", context->session_id);
#ifdef TC_ASYNC_NOTIFY_SUPPORT
	session->wait_data.send_wait_flag = 0;
	init_waitqueue_head(&session->wait_data.send_cmd_wq);
#endif

	mutex_lock(&service->session_lock);
	list_add_tail(&session->head, &service->session_list);
	mutex_unlock(&service->session_lock);

	return ret;
error:
	TCERR(" CA[%s] UID[0x%02x%02x%02x%02x] to TA[%02x%02x%02x%02x] open session failed.\n",
			dev_file->pkg_name,
			dev_file->pub_key[3], dev_file->pub_key[2],
			dev_file->pub_key[1], dev_file->pub_key[0],
			*(context->uuid+3), *(context->uuid+2),
			*(context->uuid+1), *(context->uuid+0));
	kfree(session);
	return ret;
}


int TC_NS_CloseSession(TC_NS_DEV_File *dev_file, TC_NS_ClientContext *context)
{
	int ret = -EINVAL;
	TC_NS_Service *service = NULL;
	TC_NS_Session *session = NULL;

	if (!dev_file || !context) {
		TCERR("invalid dev_file or context\n");
		return ret;
	}

	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	service = tc_find_service(&dev_file->services_list, context->uuid);
	if (NULL == service) {
		mutex_unlock(&g_tc_ns_dev_list.dev_lock);
		return ret;
	}

	session =
		tc_find_session(&service->session_list,
				context->session_id);

	if (session) {
		int ret2;

		mutex_lock(&session->ta_session_lock);
		ret2 =
			close_session(dev_file, context->uuid,
				      context->session_id);
		mutex_unlock(&session->ta_session_lock);

		if (TEEC_SUCCESS == ret2)
			TCDEBUG("close session smc success.\n");
		else
			TCERR("close session smc failed!\n");

		list_del(&session->head);
		kfree(session);
		ret = TEEC_SUCCESS;
	}

	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	return ret;
}


int TC_NS_Send_CMD(TC_NS_DEV_File *dev_file, TC_NS_ClientContext *context)
{
	int ret = -EINVAL;
	TC_NS_Service *service = NULL;
	TC_NS_Session *session = NULL;

	if (!dev_file || !context) {
		TCERR("invalid dev_file or context\n");
		return ret;
	}
	TCDEBUG("session id :%x\n", context->session_id);

	/*check sessionid is validated or not */
	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	service = tc_find_service(&dev_file->services_list, context->uuid);
	if (service) {
		session =
			tc_find_session(&service->session_list,
					context->session_id);
		if (session) {
			TCDEBUG("send cmd find session id %x\n",
				context->session_id);
			goto find_session;
		}
	}

	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
	TCERR("send cmd can not find session id %d\n", context->session_id);
	return ret;

find_session:
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);

	/*send smc */
	mutex_lock(&session->ta_session_lock);
	ret = tc_client_call(context, dev_file, 0);
	mutex_unlock(&session->ta_session_lock);

	if (ret != 0)
		TCERR("smc_call returns error, ret=0x%x\n", ret);
	else
		TCDEBUG("smc_call returns right\n");

	return ret;
}


static int TC_NS_TST_CMD(TC_NS_DEV_File *dev_id, void *argp)
{
	int ret = 0;
	TC_NS_ClientContext client_context;
	int cmd_id;
	int timer_type;
	TEEC_UUID secure_timer_uuid = {
		0x19b39980, 0x2487, 0x7b84,
		{0xf4, 0x1a, 0xbc, 0x89, 0x22, 0x62, 0xbb, 0x3d}
	};

	if (!argp) {
		TCERR("argp is NULL input buffer\n");
		ret = -EINVAL;
		return ret;
	}

	if (copy_from_user(&client_context, argp,
			   sizeof(TC_NS_ClientContext))) {
		TCERR("copy from user failed\n");
		ret = -ENOMEM;
		return ret;
	}

	if (tc_user_param_valid(&client_context, 0)) {
		TCERR("param 0 is invalid\n");
		ret = -EFAULT;
		return ret;
	}

	/*a_addr contain the command id*/
	if (copy_from_user
	    (&cmd_id, (void *)client_context.params[0].value.a_addr,
	     sizeof(cmd_id))) {
		TCERR("copy from user failed:cmd_id\n");
		ret = -ENOMEM;
		return ret;
	}

	if (memcmp((char *)client_context.uuid,
		   (char *)&secure_timer_uuid, sizeof(TEEC_UUID))) {
		TCERR("request not from secure_timer\n");
		TCERR("request uuid: %x %x %x %x\n",
		      *(client_context.uuid + 0),
		      *(client_context.uuid + 1),
		      *(client_context.uuid + 2),
		      *(client_context.uuid + 3));
		ret = -EACCES;
		return ret;
	}

	switch (cmd_id) {
	case TST_CMD_01:
		callback_demo_main((char *)client_context.uuid);
		break;
	case TST_CMD_02:
		TST_get_timer_type(&timer_type);
		if (tc_user_param_valid(&client_context, 1)) {
			TCERR("param 1 is invalid\n");
			ret = -EFAULT;
			return ret;
		}
		if (copy_to_user
		    ((void *)client_context.params[1].value.a_addr, &timer_type,
		     sizeof(timer_type))) {
			TCERR("copy to user failed:timer_type\n");
			ret = -ENOMEM;
			return ret;
		}
		break;
	default:
		ret = -EINVAL;
		return ret;
	}

	if (copy_to_user(argp, (void *)&client_context,
			 sizeof(client_context))) {
		TCERR("copy to user failed:client context\n");
		ret = -ENOMEM;
		return ret;
	}

	return ret;
}


static int TC_NS_load_image_operation(TC_NS_ClientContext *client_context,
				      TC_NS_Operation *operation,
				      TC_NS_DEV_File *dev_file)
{
	int index = 0;
	int ret = 1;
	TC_NS_Shared_MEM *shared_mem = NULL;
	TC_NS_ClientParam *client_param = NULL;
	unsigned int param_type;

	client_param = &(client_context->params[index]);
	param_type = TEEC_PARAM_TYPE_GET(client_context->paramTypes, index);

	/* TCDEBUG("--->param type is %d\n", param_type); */
	if (param_type != TEEC_MEMREF_PARTIAL_INOUT) {
		TCERR("not valid param_type for load app\n");
		ret = -1;
		return ret;
	}

	if (copy_from_user(&operation->params[index].memref.size,
			   (void *)client_param->memref.size_addr,
			   sizeof(unsigned int))) {
		TCERR("copy param->memref.size_addr failed\n");
		ret = -1;
		return ret;
	}

	list_for_each_entry(shared_mem, &dev_file->shared_mem_list, head) {
		if (shared_mem->user_addr ==
		    (void *)client_param->memref.buffer) {
			if (shared_mem->len >=
			    operation->params[index].memref.size) {
				/* arbitrary CA can control offset by ioctl.
				 * when shared_mem used in TEEC_AppLoadCmd of
				 * libteec.so, offset is 0. so in here don't
				 * add client_param->memref.offset to
				 * kernel_addr. */
				operation->params[index].memref.buffer =
					virt_to_phys((void *)shared_mem
						     ->kernel_addr);
				operation->buffer_h_addr[index] =
					virt_to_phys((void *)shared_mem
						     ->kernel_addr) >> 32;
			}
			break;
		}
	}

	/* for 8G physical memory device, there is a chance that
	 * operation->params[index].memref.buffer could be all 0,
	 * buffer_h_addr cannot be 0 in the same time.  */
	if ((!operation->params[index].memref.buffer)
	    && (!operation->buffer_h_addr[index])) {
		TCERR("can not find shared buffer, exit\n");
		ret = -1;
		return ret;
	}

	/* trans TEEC_MEMREF_PARTIAL_INOUT to TEEC_MEMREF_TEMP_INOUT */
	operation->paramTypes = TEEC_PARAM_TYPES(param_type - 8,
				TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE,
				TEE_PARAM_TYPE_NONE);

	return ret;
}

static int TC_NS_load_image(TC_NS_DEV_File *dev_file, void *argp, unsigned cmd)
{
	int ret, index = 0;
	TC_NS_SMC_CMD *smc_cmd = NULL;
	TC_NS_Operation *operation = NULL;
	TC_NS_ClientContext client_context;
	unsigned char uuid[17] = {0};
	int reg_buf_size = 0;
	struct reg_buf_st  *k_register_buf, *k_register_buf_p;
	TC_NS_Shared_MEM *shared_mem = NULL;

	if (!dev_file) {
		TCERR("dev file id erro\n");
		return IMG_LOAD_FIND_NO_DEV_ID;
	}

	if (copy_from_user(&client_context, argp,
				sizeof(TC_NS_ClientContext))) {
		TCERR("copy from user failed\n");
		ret =  -ENOMEM;
		return ret;
	}

	smc_cmd = kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
	if (!smc_cmd) {
		TCERR("smc_cmd malloc failed");
		ret = -ENOMEM;
		return ret;
	}

	/* load image operation */
	operation = kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
	if (!operation) {
		TCERR("login malloc failed");
		ret = -ENOMEM;
		goto operation_erro;
	}

	/* go with load image smc command */
	if (copy_from_user(&reg_buf_size,
		(void __user *)client_context.params[0].memref.size_addr,
		sizeof(unsigned int))) {
		TCERR("register buf size get fail\n");
		ret = -1;
		goto out;
	}

	if (reg_buf_size > MAX_REGISTER_SIZE) {
		TCERR("TA size is too large(10M)\n");
		ret = -1;
		goto out;
	}

	k_register_buf_p = kzalloc(reg_buf_size, GFP_KERNEL);
	if (!k_register_buf_p) {
		TCERR("alloc register buf failed");
		ret = -ENOMEM;
		goto out;
	}

	if (copy_from_user(k_register_buf_p,
			(void __user *)client_context.params[0].memref.buffer,
			reg_buf_size)) {
		TCERR("register buf get fail\n");
		ret = -1;
		goto out1;
	}

	k_register_buf = k_register_buf_p;
	for (index = 0; index < reg_buf_size/sizeof(struct reg_buf_st);
			index++) {
		errno_t sret;
		/* large buffer, use share-memory to share with secure world */
		if (k_register_buf->file_size > PRE_ALLOCATE_SIZE) {
			TCERR("send_len ls too large\n");
			ret = -1;
			goto out1;
		}
		shared_mem =
			tc_mem_allocate(dev_file, k_register_buf->file_size);
		if (IS_ERR(shared_mem)) {
			TCERR("share mem get fail\n");
			ret = -1;
			goto out1;
		}
		if (copy_from_user(shared_mem->kernel_addr,
				(void __user *)k_register_buf->file_buffer,
				k_register_buf->file_size)) {
			TCERR("file buf get fail\n");
			tc_mem_free(shared_mem);
			ret = -1;
			goto out1;
		}

		/* load image opration */
		sret = memset_s(operation, sizeof(TC_NS_Operation), 0,
				sizeof(TC_NS_Operation));
		if (EOK != sret) {
			tc_mem_free(shared_mem);
			goto out1;
		}
		operation->params[0].memref.buffer  =
			virt_to_phys((void *)shared_mem->kernel_addr);
		operation->buffer_h_addr[0] =
			virt_to_phys((void *)shared_mem->kernel_addr) >> 32;
		/*get out of the load_flag, size is int*/
		operation->params[0].memref.size = k_register_buf->file_size;
		operation->paramTypes = TEEC_PARAM_TYPES(
			TEEC_MEMREF_TEMP_INOUT,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE,
			TEE_PARAM_TYPE_NONE);

		/* load image smc command */
		smc_cmd->cmd_id = GLOBAL_CMD_ID_LOAD_SECURE_APP;
		uuid[0] = 1;
		smc_cmd->uuid_phys = virt_to_phys((void *)uuid);
		smc_cmd->uuid_h_phys = virt_to_phys((void *)uuid) >> 32;
		smc_cmd->dev_file_id = dev_file->dev_file_id;
		smc_cmd->context_id = 0;
		smc_cmd->operation_phys = virt_to_phys(operation);
		smc_cmd->operation_h_phys = virt_to_phys(operation) >> 32;

		TCDEBUG("secure app load smc command, file size %d\n",
			k_register_buf->file_size);
		TCDEBUG("operation phys 0x%x\n", smc_cmd->operation_phys);
		ret = TC_NS_SMC(smc_cmd, 0);
		tc_mem_free(shared_mem);
		k_register_buf++;
		TCDEBUG("smc cmd ret %d\n", ret);
		if (ret != 0) {
			TCERR("smc_call returns error ret 0x%x\n", ret);
			ret = IMG_LOAD_SECURE_RET_ERROR;
			goto out1;
		}
	}

	/* error to free: */
out1:
	kfree(k_register_buf_p);
out:
	kfree(operation);
operation_erro:
	kfree(smc_cmd);

	return ret;
}

static int TC_NS_need_load_image(TC_NS_DEV_File *dev_file,
		void *argp, unsigned cmd)
{
	int ret;
	TC_NS_SMC_CMD *smc_cmd = NULL;
	TC_NS_Operation *operation = NULL;
	TC_NS_ClientContext client_context;
	unsigned char uuid[17] = { 0 };

	if (!dev_file) {
		TCERR("dev file id erro\n");
		return IMG_LOAD_FIND_NO_DEV_ID;
	}
	if (!argp) {
		TCERR("argp is NULL input buffer\n");
		return -EINVAL;
	}

	if (copy_from_user(&client_context, argp,
			   sizeof(TC_NS_ClientContext))) {
		TCERR("copy from user failed\n");
		ret = -ENOMEM;
		return ret;
	}

	smc_cmd = kzalloc(sizeof(TC_NS_SMC_CMD), GFP_KERNEL);
	if (!smc_cmd) {
		TCERR("smc_cmd malloc failed");
		ret = -ENOMEM;
		return ret;
	}

	/* load image operation */
	operation = kzalloc(sizeof(TC_NS_Operation), GFP_KERNEL);
	if (!operation) {
		TCERR("login malloc failed");
		ret = -ENOMEM;
		goto operation_erro;
	}

	if (TC_NS_CLIENT_IOCTL_NEED_LOAD_APP == cmd) {
		client_context.cmd_id = GLOBAL_CMD_ID_NEED_LOAD_APP;
	} else if (TC_NS_CLIENT_IOCTL_LOAD_APP_REQ == cmd) {
		client_context.cmd_id = GLOBAL_CMD_ID_LOAD_SECURE_APP;
	} else {
		TCERR("cmd is is not correct(%d)\n", client_context.cmd_id);
		ret = -EFAULT;
		goto buf_erro;
	}

	ret = TC_NS_load_image_operation(&client_context, operation, dev_file);
	if (ret < 0) {
		ret = IMG_LOAD_FIND_NO_SHARE_MEM;
		goto buf_erro;
	}

	/* load image smc command */
	TCDEBUG("smc cmd id %d\n", client_context.cmd_id);
	smc_cmd->cmd_id = client_context.cmd_id;
	uuid[0] = 1;
	smc_cmd->uuid_phys = virt_to_phys((void *)uuid);
	smc_cmd->uuid_h_phys = virt_to_phys((void *)uuid) >> 32;
	smc_cmd->dev_file_id = dev_file->dev_file_id;
	smc_cmd->context_id = 0;
	smc_cmd->operation_phys = virt_to_phys(operation);
	smc_cmd->operation_h_phys = virt_to_phys(operation) >> 32;

	TCDEBUG("secure app load smc command\n");
	ret = TC_NS_SMC(smc_cmd, 0);

	if (ret != 0) {
		TCERR("smc_call returns error ret 0x%x\n", ret);
		ret = IMG_LOAD_SECURE_RET_ERROR;
	}

	/* error: */
buf_erro:
	kfree(operation);
operation_erro:
	kfree(smc_cmd);

	return ret;
}


int TC_NS_ClientOpen(TC_NS_DEV_File **dev_file, uint8_t kernel_api)
{
	int ret = TEEC_ERROR_GENERIC;
	TC_NS_DEV_File *dev = NULL;

	TCDEBUG("tc_client_open\n");

	if (!dev_file) {
		TCERR("dev_file is NULL");
		return -EFAULT;
	}

	dev = kzalloc(sizeof(TC_NS_DEV_File), GFP_KERNEL);
	if (!dev) {
		TCERR("dev malloc failed");
		return ret;
	}

	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	list_add_tail(&dev->head, &g_tc_ns_dev_list.dev_file_list);
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);

	mutex_lock(&device_file_cnt_lock);
	g_tc_ns_dev_list.dev_file_cnt++;
	dev->dev_file_id = device_file_cnt;
	device_file_cnt++;
	mutex_unlock(&device_file_cnt_lock);

	dev->service_cnt = 0;
	INIT_LIST_HEAD(&dev->services_list);

	dev->shared_mem_cnt = 0;
	INIT_LIST_HEAD(&dev->shared_mem_list);

	dev->login_setup = 0;

	dev->kernel_api = kernel_api;
	dev->load_app_flag = 0;
	mutex_init(&dev->service_lock);
	mutex_init(&dev->shared_mem_lock);
	*dev_file = dev;

	ret = TEEC_SUCCESS;

	return ret;
}


int TC_NS_ClientClose(TC_NS_DEV_File *dev, int flag)
{
	int ret = TEEC_ERROR_GENERIC;
	TC_NS_Service *service = NULL, *service_temp = NULL;
	TC_NS_Shared_MEM *shared_mem = NULL;
	TC_NS_Shared_MEM *shared_mem_temp = NULL;

	if (!dev) {
		TCERR("invalid dev(null)\n");
		return ret;
	}

	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	list_for_each_entry_safe(service, service_temp, &dev->services_list,
				 head) {
		if (service) {
			/* close unclosed session */
			if (!list_empty(&service->session_list)) {
				TC_NS_Session *session, *tmp_session;

				list_for_each_entry_safe(session, tmp_session,
							 &service->session_list,
							 head) {
					TCDEBUG
					("terminate opened service 0x%x\n",
					 *(uint32_t *) service->uuid);
					mutex_lock(&session->ta_session_lock);
					kill_session(dev, service->uuid,
						     session->session_id);
					mutex_unlock(&session->ta_session_lock);
					kfree(session);
				}
			}

			list_del(&service->head);
			kfree(service);
			dev->service_cnt--;
		}
	}
	list_for_each_entry_safe(shared_mem, shared_mem_temp,
				 &dev->shared_mem_list, head) {
		if (shared_mem) {
			list_del(&shared_mem->head);
			if (!flag)
				tc_mem_free(shared_mem);
			dev->shared_mem_cnt--;
		}
	}

	if (!flag)
		TC_NS_unregister_agent_client(dev);

	if (0 == dev->service_cnt && list_empty(&dev->services_list)) {
		ret = TEEC_SUCCESS;
		/*del dev from the list */
		list_del(&dev->head);
		kfree(dev);
	} else {
		TCERR("realse service first!");
	}

	TCDEBUG("dev list  dev file cnt:%d\n", g_tc_ns_dev_list.dev_file_cnt);
	if (g_tc_ns_dev_list.dev_file_cnt != 0) {
		g_tc_ns_dev_list.dev_file_cnt--;
		TCDEBUG("dev list  dev file cnt:%d\n",
			g_tc_ns_dev_list.dev_file_cnt);
	} else {
		TCERR("dev file list had been empty already");
	}

	mutex_unlock(&g_tc_ns_dev_list.dev_lock);

	return ret;
}


void shared_vma_open(struct vm_area_struct *vma)
{
}

void shared_vma_close(struct vm_area_struct *vma)
{
	TC_NS_Shared_MEM *shared_mem = NULL, *shared_mem_temp = NULL;
	TC_NS_Service *service = NULL, *service_temp = NULL;
	TC_NS_DEV_File *dev_file = vma->vm_private_data;
	void *user_addr = (void *)(vma->vm_start);

	if ((g_teecd_task == current->group_leader) && (!TC_NS_get_uid())
			&& (g_teecd_task->flags & PF_EXITING ||
				current->flags & PF_EXITING)) {
		TCDEBUG("teecd is killed, just return in vma close\n");
		return;
	}

	mutex_lock(&g_tc_ns_dev_list.dev_lock);

	/* if there is any session in work(TA is working),then
	 * wait on the session lock before release the sharemem,
	 * otherwise may induce the crash issue WeChat encountered.
	 * here we could not directly kill the TA because user maybe
	 * just called unmap only.
	 */
	list_for_each_entry_safe(service, service_temp,
			&dev_file->services_list, head) {
		if (service) {
			if (!list_empty(&service->session_list)) {
				TC_NS_Session *session, *tmp_session;

				list_for_each_entry_safe(session, tmp_session,
						&service->session_list, head) {
					mutex_lock(&session->ta_session_lock);
					mutex_unlock(&session->ta_session_lock);
				}
			}
		}
	}
	list_for_each_entry_safe(shared_mem, shared_mem_temp,
				 &dev_file->shared_mem_list, head) {
		if (shared_mem && shared_mem->user_addr == (void *)user_addr) {
			list_del(&shared_mem->head);
			tc_mem_free(shared_mem);
			dev_file->shared_mem_cnt--;
			break;
		}
	}
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);
}


static struct vm_operations_struct shared_remap_vm_ops = {
	.open = shared_vma_open,
	.close = shared_vma_close,
};


static int tc_client_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret = TEEC_SUCCESS;
	TC_NS_DEV_File *dev_file = filp->private_data;
	unsigned long len = vma->vm_end - vma->vm_start;
	TC_NS_Shared_MEM *shared_mem = NULL;
	struct __smc_event_data *event_control = NULL;

	if (!dev_file) {
		TCERR("can not find dev in malloc shared buffer!\n");
		return -1;
	}

	if ((g_teecd_task == current->group_leader) && (!TC_NS_get_uid())) {
		if (1 == vma->vm_pgoff)
			event_control = find_event_control(AGENT_FS_ID);
		else if (2 == vma->vm_pgoff)
			event_control = find_event_control(AGENT_MISC_ID);
		else if (3 == vma->vm_pgoff)
			event_control = find_event_control(AGENT_SOCKET_ID);

		if (event_control)
			shared_mem = event_control->buffer;
	}

	if (!shared_mem)
		shared_mem = tc_mem_allocate(dev_file, len);

	if (IS_ERR(shared_mem))
		return -1;

	if (remap_pfn_range(vma, vma->vm_start,
			    virt_to_phys(shared_mem->kernel_addr) >> PAGE_SHIFT,
			    len, vma->vm_page_prot)) {
		TCERR("can not remap_pfn_range!\n");
		if (event_control)
			return -1;

		tc_mem_free(shared_mem);
		return -1;
	}

	vma->vm_flags |= VM_DONTCOPY;
	vma->vm_ops = &shared_remap_vm_ops;
	shared_vma_open(vma);
	shared_mem->user_addr = (void *)vma->vm_start;

	mutex_lock(&g_tc_ns_dev_list.dev_lock);
	dev_file->shared_mem_cnt++;
	vma->vm_private_data = (void *)dev_file;
	list_add_tail(&shared_mem->head, &dev_file->shared_mem_list);
	mutex_unlock(&g_tc_ns_dev_list.dev_lock);

	return ret;
}


static long tc_client_session_ioctl(struct file *file, unsigned cmd,
				    unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	void *argp = (void __user *)arg;
	TC_NS_DEV_File *dev_file = file->private_data;
	TC_NS_ClientContext context;

	if (!argp) {
		TCERR("argp is NULL input buffer\n");
		ret = -EINVAL;
		return ret;
	}

	if (copy_from_user(&context, argp, sizeof(TC_NS_ClientContext))) {
		TCERR("copy from user failed\n");
		return -EFAULT;
	}

	switch (cmd) {
	case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ: {
		ret = TC_NS_OpenSession(dev_file, &context);
		if (ret)
			TCERR("TC_NS_OpenSession Failed ret is %d\n", ret);

		if (copy_to_user(argp, &context, sizeof(context))) {
			if (0 == ret)
				ret = -EFAULT;
			/* Need to close session?
			 * or wait for fd close ? */
		}
		break;
	}
	case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ: {
		ret = TC_NS_CloseSession(dev_file, &context);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ: {
		ret = TC_NS_Send_CMD(dev_file, &context);
		if (ret)
			TCERR("TC_NS_Send_CMD Failed ret is %d\n", ret);

		if (copy_to_user(argp, &context, sizeof(context)))
			if (0 == ret)
				ret = -EFAULT;
		break;
	}
	default:
		TCERR("invalid cmd!");
		return ret;
	}
	return ret;
}


static long tc_agent_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	TC_NS_DEV_File *dev_file = file->private_data;

	if (!dev_file) {
		TCERR("invalid params\n");
		return ret;
	}

	switch (cmd) {
	case TC_NS_CLIENT_IOCTL_WAIT_EVENT: {
		ret = TC_NS_wait_event((unsigned int)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE: {
		ret = TC_NS_send_event_response((unsigned int)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_REGISTER_AGENT: {
		TC_NS_Shared_MEM *shared_mem = NULL, *tmp_mem = NULL;
		/*find sharedmem */
		mutex_lock(&g_tc_ns_dev_list.dev_lock);
		list_for_each_entry(tmp_mem, &dev_file->shared_mem_list,
				    head) {
			if (tmp_mem) {
				shared_mem = tmp_mem;
				break;
			}
		}
		mutex_unlock(&g_tc_ns_dev_list.dev_lock);
		ret =
			TC_NS_register_agent(dev_file, (unsigned int)arg,
					     shared_mem);
		break;
	}
	case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT: {
		ret = TC_NS_unregister_agent((unsigned int)arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SYC_SYS_TIME: {
		ret = TC_NS_sync_sys_time((TC_NS_Time *) arg);
		break;
	}
	case TC_NS_CLIENT_IOCTL_SET_NATIVE_IDENTITY: {
		ret = TC_NS_set_nativeCA_hash(arg);
		break;
	}
	default:
		TCERR("invalid cmd!");
		return ret;
	}

	TCDEBUG("TC_NS_ClientIoctl ret = 0x%x\n", ret);
	return ret;
}


#if defined(CONFIG_TEE_TUI)
static int TC_NS_tui_event(TC_NS_DEV_File *dev_file, void *argp)
{
	int ret = 0;
	TEEC_TUI_Parameter tui_param = { 0 };

	if (!dev_file) {
		TCERR("dev file id erro\n");
		return IMG_LOAD_FIND_NO_DEV_ID;
	}

	if (!argp) {
		TCERR("argp is NULL input buffer\n");
		ret = -EINVAL;
		return ret;
	}
	if (copy_from_user(&tui_param, argp, sizeof(TEEC_TUI_Parameter))) {
		TCERR("copy from user failed\n");
		ret = -ENOMEM;
		return ret;
	}

	if (tui_param.event_type <= TUI_POLL_MAX) {
		ret = tui_send_event(tui_param.event_type);
	} else {
		/* todo: use to get keyCode */
		TCDEBUG("now poll keyCode-------------\n");
		tui_param.value = tui_get_keyCode();
		if (copy_to_user(argp, &tui_param,
				 sizeof(TEEC_TUI_Parameter))) {
			ret = -ENOMEM;
			TCERR("copy to user failed\n");
		}
	}

	return ret;
}
#endif

static long tc_client_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	int ret = TEEC_ERROR_GENERIC;
	void *argp = (void __user *)arg;
	TC_NS_DEV_File *dev_file = file->private_data;
	TC_NS_ClientContext client_context = { { 0 } };

	switch (cmd) {
		/* IOCTLs for the CAs */
	case TC_NS_CLIENT_IOCTL_SES_OPEN_REQ:
	case TC_NS_CLIENT_IOCTL_SES_CLOSE_REQ:
	case TC_NS_CLIENT_IOCTL_SEND_CMD_REQ:
		ret = tc_client_session_ioctl(file, cmd, arg);
		break;

	case TC_NS_CLIENT_IOCTL_NEED_LOAD_APP:{
			ret = TC_NS_need_load_image(dev_file, argp, cmd);
			break;
		}
	case TC_NS_CLIENT_IOCTL_LOAD_APP_REQ:{
		mutex_lock(&load_app_lock);
		ret = TC_NS_load_image(dev_file, argp, cmd);
		mutex_unlock(&load_app_lock);
		break;
	}
	case TC_NS_CLIENT_IOCTL_CANCEL_CMD_REQ:
		TCDEBUG("come into cancel cmd\n");
		if (!argp) {
			TCERR("argp is NULL input buffer\n");
			ret = -EINVAL;
			return ret;
		}
		if (copy_from_user
		    (&client_context, argp, sizeof(TC_NS_ClientContext))) {
			TCERR("copy from user failed\n");
			ret = -ENOMEM;
			return ret;
		}
		ret = TC_NS_Send_CMD(dev_file, &client_context);
		TCDEBUG("cancel cmd end\n");
		break;
		/* This is the login information
		 * and is set teecd when client opens a
		 * new session */
	case TC_NS_CLIENT_IOCTL_LOGIN: {
		ret = TC_NS_Client_Login(dev_file, argp);
		break;
	}
	/* IOCTLs for the secure storage daemon */
	case TC_NS_CLIENT_IOCTL_WAIT_EVENT:
	case TC_NS_CLIENT_IOCTL_SEND_EVENT_RESPONSE:
	case TC_NS_CLIENT_IOCTL_REGISTER_AGENT:
	case TC_NS_CLIENT_IOCTL_UNREGISTER_AGENT:
	case TC_NS_CLIENT_IOCTL_SYC_SYS_TIME:
	case TC_NS_CLIENT_IOCTL_SET_NATIVE_IDENTITY:
		ret = tc_agent_ioctl(file, cmd, arg);
		break;
	case TC_NS_CLIENT_IOCTL_TST_CMD_REQ: {
		TCDEBUG("come into tst cmd\n");
		ret = TC_NS_TST_CMD(dev_file, argp);
		break;
	}
#if defined(CONFIG_TEE_TUI)
	/* for tui service inform TUI TA  event type */
	case TC_NS_CLIENT_IOCTL_TUI_EVENT: {
		TCDEBUG("come into tui cmd\n");
		ret = TC_NS_tui_event(dev_file, argp);
		break;
	}
#endif
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef CONFIG_IMAX_CIPHER
	case TC_NS_CLIENT_IOCTL_GENERAL_SRV:{
		mutex_lock(&general_srv_lock);
		ret = general_srv_session_handle(dev_file, argp);
		mutex_unlock(&general_srv_lock);
		break;
	}
	case TC_NS_CLIENT_IOCTL_GENERAL_SRV_CMD_SEND:{
		mutex_lock(&general_srv_cmd_lock);
		ret = general_srv_cmd_handle(argp);
		mutex_unlock(&general_srv_cmd_lock);
		break;
	}
#endif
#endif
	default:
		TCERR("invalid cmd!");
		return ret;
	}

	TCDEBUG("TC_NS_ClientIoctl ret = 0x%x\n", ret);
	return ret;
}


static int tc_client_open(struct inode *inode, struct file *file)
{
	int ret = TEEC_ERROR_GENERIC;
	TC_NS_DEV_File *dev = NULL;
#ifndef CONFIG_DEVCHIP_PLATFORM
	if (check_teecd_access(current)) {
		TCERR(KERN_ERR "tc_client_open ca verification failed\n");
		return -EPERM;
	}
#endif
	if (!g_teecd_task) {
		g_teecd_task = current->group_leader;
		/*currently we have 3 agents need to care for. */
		agent_count = 3;
	}

	file->private_data = NULL;
	ret = TC_NS_ClientOpen(&dev, TEE_REQ_FROM_USER_MODE);
	if (TEEC_SUCCESS == ret)
		file->private_data = dev;

	return ret;
}


static int tc_client_close(struct inode *inode, struct file *file)
{
	int ret = TEEC_ERROR_GENERIC;
	TC_NS_DEV_File *dev = file->private_data;

	/* release tui resource */
#if defined(CONFIG_TEE_TUI)
	if (dev->dev_file_id == tui_attach_device())
		tui_send_event(TUI_POLL_CANCEL);
#endif
	if ((g_teecd_task == current->group_leader) && (!TC_NS_get_uid())
			&& (g_teecd_task->flags & PF_EXITING
				|| current->flags & PF_EXITING)) {
		TCERR("teecd is killed, something bad must be happened!!!\n");
		TC_NS_send_event_response_all();
		ret = TC_NS_ClientClose(dev, 1);
		if (0 == (--agent_count))
			g_teecd_task = NULL;
	} else {
		ret = TC_NS_ClientClose(dev, 0);
	}

	file->private_data = NULL;
	return ret;
}


#ifdef CONFIG_COMPAT
long tc_compat_client_ioctl(struct file *flie, unsigned int cmd,
			    unsigned long arg)
{
	long ret = -ENOIOCTLCMD;

	arg = (unsigned long)compat_ptr(arg);
	ret = tc_client_ioctl(flie, cmd, arg);
	return ret;
}
#endif

static const struct file_operations TC_NS_ClientFops = {
	.owner = THIS_MODULE,
	.open = tc_client_open,
	.release = tc_client_close,
	.unlocked_ioctl = tc_client_ioctl,
	.mmap = tc_client_mmap,
#ifdef CONFIG_COMPAT
	.compat_ioctl = tc_compat_client_ioctl,
#endif
};

#if defined(CONFIG_DEVCHIP_PLATFORM) && defined(CONFIG_HISI_IOMMU_API)
static int smmu_init; /* add by yangwei  */
#endif
static int tui_flag = 0;
static __init int tc_init(void)
{
	struct device *class_dev = NULL;
	int ret = 0;
	unsigned int irq = 0;
	errno_t sret;

	TCDEBUG("tc_ns_client_init");
	/*TODO:XXX, by laiyingjun, fix compiling error*/
#ifndef CONFIG_DEVCHIP_PLATFORM
	np = of_find_compatible_node(NULL, NULL, "trusted_core");
	if (!np) {
		TCERR("No trusted_core compatible node found.\n");
		return -ENODEV;
	}
#endif

	ret = alloc_chrdev_region(&tc_ns_client_devt, 0, 1, TC_NS_CLIENT_DEV);
	if (ret < 0) {
		TCERR("alloc_chrdev_region failed %d", ret);
		return -EFAULT;
	}

	driver_class = class_create(THIS_MODULE, TC_NS_CLIENT_DEV);
	if (IS_ERR(driver_class)) {
		ret = -ENOMEM;
		TCERR("class_create failed %d", ret);
		goto unregister_chrdev_region;
	}

	class_dev = device_create(driver_class, NULL, tc_ns_client_devt, NULL,
				  TC_NS_CLIENT_DEV);
	if (!class_dev) {
		TCERR("class_device_create failed");
		ret = -ENOMEM;
		goto class_destroy;
	}

#ifndef CONFIG_DEVCHIP_PLATFORM
	/*TODO:XXX, by laiyingjun, fix compiling error*/
	class_dev->of_node = np;
#endif

	cdev_init(&tc_ns_client_cdev, &TC_NS_ClientFops);
	tc_ns_client_cdev.owner = THIS_MODULE;

	ret = cdev_add(&tc_ns_client_cdev,
		       MKDEV(MAJOR(tc_ns_client_devt), 0), 1);
	if (ret < 0) {
		TCERR("cdev_add failed %d", ret);
		goto class_device_destroy;
	}

#ifdef TC_ASYNC_NOTIFY_SUPPORT
	TCDEBUG("register secure notify handler\n");
	/* Map IRQ 0 from the OF interrupts list */
#ifdef CONFIG_DEVCHIP_PLATFORM
	/*TODO: not support DTS*/
	/* irq = irq_of_parse_and_map(np, 0);*/
#if defined(CONFIG_ARCH_HI3798MX)
	/*TODO: current use a reserve IRQ 61 + 32*/
	irq = 93;
#elif defined(CONFIG_ARCH_HI3798CV2X) || defined(CONFIG_ARCH_HI3798MV2X)
	/*TODO: 43 + 32*/
	irq = 75;
#elif defined(CONFIG_ARCH_S5)
	/*TODO: 48 + 32*/
	irq = 80;
#endif
#else
	irq = irq_of_parse_and_map(np, 0);
#endif
	ret = devm_request_irq(class_dev, irq, tc_secure_notify,
			       0, TC_NS_CLIENT_DEV, NULL);
	if (ret < 0) {
		TCERR("device irq %u request failed %u", irq, ret);
		goto class_device_destroy;
	}
#endif
	sret = memset_s(&g_tc_ns_dev_list, sizeof(g_tc_ns_dev_list),
			0, sizeof(g_tc_ns_dev_list));
	if (EOK != sret)
		goto class_device_destroy;

	g_tc_ns_dev_list.dev_file_cnt = 0;
	INIT_LIST_HEAD(&g_tc_ns_dev_list.dev_file_list);
	mutex_init(&g_tc_ns_dev_list.dev_lock);
	mutex_init(&g_tee_crypto_hash_lock);

	sret = memset_s(&g_ta_callback_func_list,
			sizeof(g_ta_callback_func_list), 0,
			sizeof(g_ta_callback_func_list));
	if (EOK != sret)
		goto class_device_destroy;

	g_ta_callback_func_list.callback_count = 0;
	INIT_LIST_HEAD(&g_ta_callback_func_list.callback_list);
	mutex_init(&g_ta_callback_func_list.callback_list_lock);

	ret = smc_init_data(class_dev);
	if (ret < 0)
		goto class_device_destroy;

	ret = agent_init();
	if (ret < 0)
		goto smc_data_free;

	if (tc_mem_init())
		goto free_agent;

#if defined(CONFIG_TEELOG)
	ret = TC_NS_register_rdr_mem();
	if (ret)
		TCERR("TC_NS_register_rdr_mem failed %x\n", ret);

	ret = teeos_register_exception();	/*0:error */
	if (0 == ret)
		TCERR("teeos_register_exception to rdr failed\n");
	else
		ret = 0;
#endif

#ifdef TC_ASYNC_NOTIFY_SUPPORT
	mutex_lock(&notify_data_lock);
	if (!g_notify_data) {
		g_notify_data =
			(void *)__get_free_page(GFP_KERNEL | __GFP_ZERO);
		if (!g_notify_data) {
			TCERR("__get_free_page failed for notification data\n");
			ret = -ENOMEM;
			mutex_unlock(&notify_data_lock);
			goto free_shared_mem;
		}
		ret = TC_NS_register_notify_data_memery();
		if (ret != TEEC_SUCCESS) {
			TCERR("Shared memory failed ret is 0x%x\n", ret);
			ret = -1;
			mutex_unlock(&notify_data_lock);
			goto free_notify_mem;
		}
	}
	mutex_unlock(&notify_data_lock);
	notify_data_entry_timer =
		acquire_notify_data_entry(NOTIFY_DATA_ENTRY_TIMER);
#endif
#ifndef CONFIG_DEVCHIP_PLATFORM
	if (of_get_property(np, "tui-enable", NULL)) {
		ret = of_reserved_mem_device_init(class_dev);
		if (ret < 0) {
			TCERR("reserve tui mem failed\n");
			tui_flag = 1;
			goto skip_tui;
		}
		ret = init_tui(class_dev);
		if (ret) {
			TCERR("init_tui failed 0x%x\n", ret);
			goto unregister_notify_mem;
		} else {
			notify_data_entry_tp =
				acquire_notify_data_entry(NOTIFY_DATA_ENTRY_TP);
			TCDEBUG("NOTIFY MEM: notify_data=0x%x\n",
				(int)virt_to_phys(g_notify_data));
			TCDEBUG("notify_data_entry_timer=0x%x\n",
				(int)virt_to_phys(notify_data_entry_timer));
			TCDEBUG("notify_data_entry_tp=0x%x\n",
				(int)virt_to_phys(notify_data_entry_tp));
		}
	}
skip_tui:
	drm_ion_client = hisi_ion_client_create("DRM_ION");

	if (IS_ERR(drm_ion_client)) {
		TCERR("in %s err: drm ion client create failed! client %p\n",
		      __func__, drm_ion_client);
		ret = -EFAULT;
		goto free_tui;
	}
#endif
#if defined(CONFIG_DEVCHIP_PLATFORM) && defined(CONFIG_HISI_IOMMU_API)
	/* add by yangwei for init smmu ta */
	if (hisi_teesmmu_init())
		TCERR("init smmu CA later!\n");
	else
		smmu_init++;
#endif
	return 0;
	/* if error happens */
free_tui:
	if (of_get_property(np, "tui-enable", NULL) && !tui_flag)
		tui_exit();
unregister_notify_mem:
#ifdef TC_ASYNC_NOTIFY_SUPPORT
	if (g_notify_data)
		TC_NS_unregister_notify_data_memory();
free_notify_mem:
	free_page((unsigned long)g_notify_data);
	g_notify_data = NULL;
#endif
free_shared_mem:
	tc_mem_destroy();
free_agent:
	agent_exit();
smc_data_free:
	smc_free_data();
class_device_destroy:
	device_destroy(driver_class, tc_ns_client_devt);
class_destroy:
	class_destroy(driver_class);
unregister_chrdev_region:
	unregister_chrdev_region(tc_ns_client_devt, 1);

	return ret;
}

static void tc_exit(void)
{
	TCDEBUG("otz_client exit");
#if defined(CONFIG_DEVCHIP_PLATFORM) && defined(CONFIG_HISI_IOMMU_API)
	/* add by yangwei for init smmu ta */
	if (smmu_init)
		hisi_teesmmu_exit();
#endif
#ifdef TC_ASYNC_NOTIFY_SUPPORT
	if (g_notify_data) {
		TC_NS_unregister_notify_data_memory();
		free_page((unsigned long)g_notify_data);
		g_notify_data = NULL;
	}
#endif
#ifndef CONFIG_DEVCHIP_PLATFORM
	/*deleted by fangjian*/
	if (of_get_property(np, "tui-enable", NULL) && !tui_flag)
		tui_exit();
#endif
	device_destroy(driver_class, tc_ns_client_devt);
	class_destroy(driver_class);
	unregister_chrdev_region(tc_ns_client_devt, 1);
	smc_free_data();

	agent_exit();
	tc_mem_destroy();

	if (drm_ion_client) {
		ion_client_destroy(drm_ion_client);
		drm_ion_client = NULL;
	}

	if (g_tee_shash_tfm) {
		crypto_free_shash(g_tee_shash_tfm);
		tee_init_crypt_state = 0;
		g_tee_shash_tfm = NULL;
	}
}

MODULE_AUTHOR("q00209673");
MODULE_DESCRIPTION("TrustCore ns-client driver");
MODULE_VERSION("1.10");
#ifdef CONFIG_DEVCHIP_PLATFORM
subsys_initcall(tc_init);
#else
fs_initcall_sync(tc_init);
#endif
module_exit(tc_exit);
