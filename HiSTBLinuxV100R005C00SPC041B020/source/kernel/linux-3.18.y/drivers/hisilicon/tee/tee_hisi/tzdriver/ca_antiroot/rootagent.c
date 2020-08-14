#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/random.h>
#include <linux/wakelock.h>
#include <linux/suspend.h>
#include <linux/notifier.h>
#include <linux/delay.h>
#include "rootagent_api.h"
#include "rootagent_common.h"

#define REPEATNUM  3
#define ROOTAGENT_VERSION "rootagent-v0.05"
#define TIMER_PERIOC (jiffies + msecs_to_jiffies(SCAN_CYCLE))
#define TIMER_FIRST (jiffies + msecs_to_jiffies(SCAN_CYCLE))
#define DEFAULT_PROC "/init"

static void do_work(struct work_struct *unused);
static DECLARE_WORK(teeagent_work, do_work);
static struct workqueue_struct *teeagent_workqueue;

static struct timer_list teeagent_timer;
static struct RAGENT_CONFIG gconfig;
static struct rscan_result_dynamic *gscan;

#define PM_SUSPEND  0x1 /*1 for suspend*/
#define PM_RESUME 0x0 /*0 for resume successful*/
static int antiroot_sr_flag = PM_RESUME;

#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
static uint32_t antiroot_debug_mask = 255;
#else
static uint32_t antiroot_debug_mask;
#endif

uint32_t get_antiroot_debug_mask(void)
{
	return antiroot_debug_mask;
}

module_param_named(debug_mask, antiroot_debug_mask, uint, S_IWUSR | S_IRUGO);

/* #define DEBUG_ROOTAGENT */
#ifdef DEBUG_ROOTAGENT

void dump_hex(unsigned char *hex, uint32_t len)
{
	uint32_t i;

	if (NULL == hex) {
		antiroot_debug(ROOTAGENT_DEBUG_DUMPHEX, "Bad expected parameter.\n");
		return;
	}

	for (i = 0; i < (len - len%16); i += 16) {
		antiroot_debug(ROOTAGENT_DEBUG_DUMPHEX,
			"%02x %02x %02x %02x   %02x %02x %02x %02x  %02x %02x %02x %02x   %02x %02x %02x %02x\n",
			hex[i + 0], hex[i + 1], hex[i + 2], hex[i + 3],
			hex[i + 4], hex[i + 5], hex[i + 6], hex[i + 7],
			hex[i + 8], hex[i + 9], hex[i + 10], hex[i + 11],
			hex[i + 12], hex[i + 13], hex[i + 14], hex[i + 15]);
	}

	for (i = (len - len%16); i < len; i++)
		antiroot_debug(ROOTAGENT_DEBUG_DUMPHEX,
				"     hex[%d] = %02x\n", i, hex[i]);
}
#else
void dump_hex(unsigned char *hex, uint32_t len) {};
#endif

static int req_challenge(struct RAGENT_CHALLENGE *chal)
{
	TEEC_Result teer;

	struct rscan_status status;

	if (NULL == chal) {
		antiroot_error("invalid parameter!\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (-1 == rscan_get_status(&status)) {
		antiroot_warning("rscan_get_status failed\n");
		/* we ignore this error. -1 is parameter error*/
	}
	/* can arbitary status values attack TA? */
	chal->cpuload = status.cpuload;
	chal->battery = status.battery;
	chal->charging = status.charging;
	chal->time = status.time;
	chal->timezone = status.timezone;

	antiroot_debug(ROOTAGENT_DEBUG_AGENT, "%d, %d, %d, %d, %d.\n",
			status.cpuload, status.battery, status.charging,
			status.time, status.timezone);

	teer = request_challenge(chal);
	if (teer != TEEC_SUCCESS) {
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"request_challenge failed 0x%x\n", teer);
		return teer;
	}
	return TEEC_SUCCESS;
}

static int send_response(const struct RAGENT_CHALLENGE *chal)
{
	int op_mask = 0;
	struct RAGENT_RESPONSE resp;
	struct RAGENT_ROOTPROC root_proc;
	TEEC_Result teer;
	int id_count;
	int opid;
	int ret_raw = -1;
	int s_ret = 0;

	if (NULL == chal) {
		antiroot_error("invalid parame!\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	for (id_count = 0; id_count < CHALLENGE_MAX_LENGTH
			&& chal->challengeid[id_count]; id_count++) {
		opid = chal->challengeid[id_count];
		antiroot_debug(ROOTAGENT_DEBUG_AGENT,
				"gen_response opid=%d\n", opid);
		if (opid <= 0 || opid >= RAOPID_MAXID) {
			antiroot_warning(
					"gen_response start invalid OPID %d\n",
					opid);
			/*
			 * if TA return challenge error,
			 * we only check and report
			 * the right opid.
			 */
			continue;
		}
		switch (opid) {
		case RAOPID_KCODE:
			op_mask |= D_RSOPID_KCODE;
			break;
		case RAOPID_SYSCALL:
			op_mask |= D_RSOPID_SYS_CALL;
			break;
		case RAOPID_RPROCS:
			op_mask |= D_RSOPID_RRPOCS;
			break;
		case RAOPID_SELINUX_STATUS:
			op_mask |= D_RSOPID_SE_STATUS;
			break;
		case RAOPID_SELINUX_HOOK:
			op_mask |= D_RSOPID_SE_HOOKS;
			break;
		case RAOPID_NOOP:
			break;
		default:
			antiroot_warning(
				"gen_response switch invalid OPID %d\n",
				opid);
			break;
			/*
			 * if TA send a OPID that CA not support,
			 * just not check or report
			 * this OPID, and continue next one.
			 */
		}
	}

	s_ret = memset_s(gscan, sizeof(struct rscan_result_dynamic),
			0x0, sizeof(struct rscan_result_dynamic));
	if (EOK != s_ret) {
		antiroot_error("send_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return s_ret;
	}

	if (op_mask != 0)
		ret_raw = rscan_dynamic_raw(op_mask, gscan);

	s_ret = memset_s(&resp, sizeof(struct RAGENT_RESPONSE),
			0x0, sizeof(struct RAGENT_RESPONSE));
	if (EOK != s_ret) {
		antiroot_error("send_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return s_ret;
	}
	s_ret = memset_s(&root_proc, sizeof(struct RAGENT_ROOTPROC),
			0x0, sizeof(struct RAGENT_ROOTPROC));
	if (EOK != s_ret) {
		antiroot_error("send_response _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return s_ret;
	}

	for (id_count = 0; id_count < CHALLENGE_MAX_LENGTH &&
			chal->challengeid[id_count]; id_count++) {
		switch (chal->challengeid[id_count]) {
		case RAOPID_KCODE:
			if (ret_raw & D_RSOPID_KCODE) {
				s_ret = memcpy_s(resp.runtime_white_list.kcodes,
						HASH_DIGEST_SIZE,
						gconfig.white_list.kcodes,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			} else {
				s_ret = memcpy_s(resp.runtime_white_list.kcodes,
						HASH_DIGEST_SIZE,
						gscan->kcode,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			}
			break;
		case RAOPID_SYSCALL:
			if (ret_raw & D_RSOPID_SYS_CALL) {
				s_ret = memcpy_s(resp.runtime_white_list.syscalls,
						HASH_DIGEST_SIZE,
						gconfig.white_list.syscalls,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			} else {
				s_ret = memcpy_s(resp.runtime_white_list.syscalls,
						HASH_DIGEST_SIZE,
						gscan->syscalls,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			}
			break;
		case RAOPID_RPROCS:
			if ((ret_raw & D_RSOPID_RRPOCS)
					|| (0 == strlen(gscan->rprocs))
					|| (strlen(gscan->rprocs) >
						ROOTAGENT_RPROCS_MAX_LENGTH)) {
				root_proc.length = strlen(DEFAULT_PROC);
				root_proc.procid = DEFAULT_PROC;
			} else {
				root_proc.length = strlen(gscan->rprocs);
				root_proc.procid = gscan->rprocs;
			}
			break;
		case RAOPID_SELINUX_STATUS:
			if (ret_raw & D_RSOPID_SE_STATUS)
				resp.runtime_white_list.selinux =
					gconfig.white_list.selinux;
			else
				resp.runtime_white_list.selinux =
					gscan->seenforcing != 1;
			break;
		case RAOPID_SELINUX_HOOK:
			if (ret_raw & D_RSOPID_SE_HOOKS) {
				s_ret = memcpy_s(resp.runtime_white_list.sehooks,
						HASH_DIGEST_SIZE,
						gconfig.white_list.sehooks,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			} else {
				s_ret = memcpy_s(resp.runtime_white_list.sehooks,
						HASH_DIGEST_SIZE,
						gscan->sehooks,
						HASH_DIGEST_SIZE);
				if (EOK != s_ret) {
					antiroot_error(
						"send_response _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
					return s_ret;
				}
			}
			break;
		case RAOPID_NOOP:
			resp.noop = 0;
			break;
		default:
			break;
		}
	}
	teer = feedback_response(&resp, &root_proc);

	if (teer != TEEC_SUCCESS) {
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"feedback_response failed 0x%x\n",
				teer);
		return teer;
	}
	return TEEC_SUCCESS;
}

static int system_sr_notify(struct notifier_block *nb,
		unsigned long pm_event, void *unused)
{
	switch (pm_event) {
	case PM_SUSPEND_PREPARE:
		antiroot_sr_flag = PM_SUSPEND;
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"system is suspending.\n");
		break;
	case PM_POST_SUSPEND:
		antiroot_sr_flag = PM_RESUME;
		antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"system resume successful.\n");
		break;
	default:
		break;
	}
	return 0;
}

static struct notifier_block system_sr_notifier = {
	.notifier_call = system_sr_notify,
};
static void do_work(struct work_struct *unused)
{
	struct wake_lock wakelock;
	struct RAGENT_CHALLENGE chal;
	int repeat = REPEATNUM;
	uint32_t ret;
	int status = 0;
	int s_ret = 0;

	status = get_ro_secure();
	wake_lock_init(&wakelock, WAKE_LOCK_SUSPEND, "rootscan");
	wake_lock_timeout(&wakelock, (long)(msecs_to_jiffies(AR_WAKELOCK_TIMEOUT)));
	while (PM_SUSPEND == antiroot_sr_flag) {
		antiroot_warning("system is suspend now, try later!\n");
		msleep(50);
	}
#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	struct timeval tstart, tend;

	do_gettimeofday(&tstart);
	pr_err("ca_antiroot  do_work start.\n");
	dummy_fuzz();
#endif

	s_ret = memset_s(&chal, sizeof(struct RAGENT_CHALLENGE),
			0x0, sizeof(struct RAGENT_CHALLENGE));
	if (EOK != s_ret) {
		antiroot_error("do_work _s fail. line=%d, s_ret=%d.\n",
				__LINE__, s_ret);
		return;
	}

	/*
	 * REPEATNUM
	 * if req_challenge failed, agent will request again.
	 * max request time is REPEATNUM.
	 */
	antiroot_debug(ROOTAGENT_DEBUG_AGENT,
			"status = %d.\n", status);
	do {
		ret = req_challenge(&chal);
		antiroot_debug(ROOTAGENT_DEBUG_AGENT,
				"req_challenge ret = %x.\n", ret);
		if (TEEC_SUCCESS == ret)
			break;
		else if (TEE_ERROR_ANTIROOT_RSP_FAIL == ret) {
			antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"req_challenge TEE_ERROR_ANTIROOT_RSP_FAIL, status = %d.\n",
				status);
			/* only when TA readstatus is root */
			goto error;
		} else
			antiroot_error("Impossible! req_challenge ret = %x.\n", ret);
	} while (--repeat);

	if (0 == repeat)
		goto error;
	/*
	 * REPEATNUM
	 * if send_response failed, agent will request again.
	 * max request time is REPEATNUM.
	 */
	repeat = REPEATNUM;
	do {
		ret = send_response(&chal);
		antiroot_debug(ROOTAGENT_DEBUG_AGENT,
			"send_response ret = %x.\n", ret);
		if (TEEC_SUCCESS == ret)
			break;
		else if (TEE_ERROR_ANTIROOT_RSP_FAIL == ret) {
			antiroot_debug(ROOTAGENT_DEBUG_ERROR,
				"send_response TEE_ERROR_ANTIROOT_RSP_FAIL, status = %d.\n",
				status);
			if (status)
				goto error;
			else
				goto out;
		} else
			antiroot_error("Impossible! req_challenge ret = %x.\n", ret);
	} while (--repeat);

	if (repeat > 0)
		goto out;
error:
	/* prepare CA/TA to stop */
	wake_lock_destroy(&wakelock);
	root_monitor_finalize();
	del_timer_sync(&teeagent_timer);
	antiroot_info("antiroot is closing up.\n");
	return;
out:
	wake_lock_destroy(&wakelock);
	antiroot_debug(ROOTAGENT_DEBUG_AGENT, "do_work out!.\n");
#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	do_gettimeofday(&tend);
	pr_err("ca_antiroot--%ld,%06ld---%ld,%06ld.\n",
			tstart.tv_sec, tstart.tv_usec,
			tend.tv_sec, tend.tv_usec);
	if (tstart.tv_usec <= tend.tv_usec)
		pr_err("ca_antiroot  do_work out, usedtime: [%ld.%06ld].\n",
			(tend.tv_sec - tstart.tv_sec),
			(tend.tv_usec - tstart.tv_usec));
	else
		pr_err("ca_antiroot  do_work out, usedtime: [%ld.%06ld].\n",
			(tend.tv_sec - tstart.tv_sec - 1),
			(1000000 + tend.tv_usec - tstart.tv_usec));
#endif
}

static void teeagent_timer_func(unsigned long unused)
{
	mod_timer(&teeagent_timer, TIMER_PERIOC); /* 10 seconds*/

#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	if (CA_DISABLE == get_rootagent_enable())
		return;
#endif

	if (queue_work(teeagent_workqueue, &teeagent_work) == 0)
		antiroot_error("Timer add work queue failed\n");
}

static int rscan_get_baseline(struct rscan_result_dynamic *base)
{
	int ret = 0;
	int s_ret = 0;

	if (base) {
		ret = load_rproc_whitelist(base->rprocs,
				ROOTAGENT_RPROCS_MAX_LENGTH);

		if (ret || 0 == strlen(base->rprocs) ||
				(strlen(base->rprocs) >
				 ROOTAGENT_RPROCS_MAX_LENGTH)) {
			s_ret = memcpy_s(base->rprocs, MAX_RPROC_SIZE,
				DEFAULT_PROC, strlen(DEFAULT_PROC));
			if (EOK != s_ret) {
				antiroot_error(
						"rscan_get_baseline _s fail. line=%d, s_ret=%d.\n",
						__LINE__, s_ret);
				return s_ret;
			}
		}

		rscan_dynamic_raw(D_RSOPID_KCODE | D_RSOPID_SYS_CALL |
				D_RSOPID_SE_HOOKS, base);
		base->seenforcing = 0;
		return ret;
	}
	return -1;
}

static int tee_init(void)
{
	TEEC_Result teer;
	struct RAGENT_ROOTPROC root_proc;
	int s_ret = 0;

	gscan = kzalloc(sizeof(struct rscan_result_dynamic), GFP_KERNEL);
	if (!gscan) {
		antiroot_error("gscan (struct rscan_result_dynamic) malloc failed.\n");
		return -ENOMEM;
	}
	if (0 != rscan_get_baseline(gscan)) {
		antiroot_error("rscan_get_baseline failed.\n");
		kfree(gscan);
		gscan = NULL;
		return -1;
	}

	teer = root_monitor_tee_init();
	if (teer != TEEC_SUCCESS) {
		antiroot_error("root_monitor_tee_init failed 0x%x\n", teer);
		kfree(gscan);
		gscan = NULL;
		return -1;
	}

	gconfig.white_list.dStatus = get_ro_secure();
	antiroot_debug(ROOTAGENT_DEBUG_ERROR,
			"tee_init dStatus = %d.\n",
			gconfig.white_list.dStatus);

	s_ret = memcpy_s(gconfig.white_list.kcodes, KERNEL_CODE_LENGTH_SHA,
			gscan->kcode, KERNEL_CODE_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("tee_init _s fail. line=%d, s_ret=%d.\n",
			__LINE__, s_ret);
		kfree(gscan);
		gscan = NULL;
		return s_ret;
	}

	s_ret = memcpy_s(gconfig.white_list.syscalls, SYSTEM_CALL_LENGTH_SHA,
			gscan->syscalls, SYSTEM_CALL_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("tee_init _s fail. line=%d, s_ret=%d.\n",
			__LINE__, s_ret);
		kfree(gscan);
		gscan = NULL;
		return s_ret;
	}
	gconfig.white_list.selinux = gscan->seenforcing;
	s_ret = memcpy_s(gconfig.white_list.sehooks, SELINUX_HOOKS_LENGTH_SHA,
			gscan->sehooks, SELINUX_HOOKS_LENGTH_SHA);
	if (EOK != s_ret) {
		antiroot_error("tee_init _s fail. line=%d, s_ret=%d.\n",
			__LINE__, s_ret);
		kfree(gscan);
		gscan = NULL;
		return s_ret;
	}
	get_random_bytes(&gconfig.cipher_key.cha_req_key,
			CHALLENGE_REQ_KEY_LENGTH);
	get_random_bytes(&gconfig.cipher_key.cha_key,
			CHALLENGE_KEY_LENGTH);
	root_proc.length = strlen(gscan->rprocs);
	antiroot_debug(ROOTAGENT_DEBUG_AGENT,
			"proc length %d\n", root_proc.length);
	root_proc.procid = (uint8_t *)(gscan->rprocs);
	teer = setting_config(&gconfig, &root_proc);

	if (teer != TEEC_SUCCESS) {
		antiroot_error("setting_config failed 0x%x\n", teer);
		kfree(gscan);
		gscan = NULL;
		root_monitor_finalize();
		return -1;
	}

	return 0;
}

#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
static int dummy_init_once;
int dummy_tee_init(void)
{
	ret = 0;

	if (0 == dummy_init_once) {
		dummy_init_once = 1;
		antiroot_debug(ROOTAGENT_DEBUG_AGENT,
				"dummy_tee_init enter now.\n");
		ret = tee_init();
	} else {
		antiroot_debug(ROOTAGENT_DEBUG_AGENT,
				"dummy_tee_init done before, do nothing now.\n");
	}
	return ret;
}
#endif

static __init int tee_rootscan_init(void)
{
	int ret;

	antiroot_info("tee_rootscan_init versin: %s.\n", ROOTAGENT_VERSION);
#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	rscan_dummy_init();
#endif

	teeagent_workqueue = create_singlethread_workqueue("antiroot-ca");
	if (!teeagent_workqueue) {
		antiroot_error("fail to create queue.\n");
		goto err2;
	}
	ret = register_pm_notifier(&system_sr_notifier);
	if (ret) {
		antiroot_error("fail to register system_sr_notifier.\n");
		goto err1;
	}
#ifndef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	ret = tee_init();
	if (ret) {
		antiroot_error("tee_init() failed. ret = %d.\n", ret);
		unregister_pm_notifier(&system_sr_notifier);
		goto err1;
	}
#endif

	teeagent_timer.function = teeagent_timer_func;
	teeagent_timer.data = 0;
	init_timer(&teeagent_timer);
	mod_timer(&teeagent_timer, TIMER_FIRST);

	return 0;
err1:
	destroy_workqueue(teeagent_workqueue);
err2:
#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	rscan_dummy_exit();
#endif
	return -1;
}

static void tee_rootscan_exit(void)
{
	del_timer_sync(&teeagent_timer);
	destroy_workqueue(teeagent_workqueue);
	teeagent_workqueue = NULL;
	unregister_pm_notifier(&system_sr_notifier);
	kfree(gscan);
	gscan = NULL;
#ifdef CONFIG_HUAWEI_ROOT_SCAN_DUMMY_API
	rscan_dummy_exit();
#endif
}

module_init(tee_rootscan_init);
module_exit(tee_rootscan_exit);
