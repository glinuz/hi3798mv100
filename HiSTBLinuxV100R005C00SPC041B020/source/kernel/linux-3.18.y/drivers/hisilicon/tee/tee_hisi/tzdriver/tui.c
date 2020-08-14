#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/sched/rt.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/atomic.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/version.h>

#include "tee_client_constants.h"	/*lint !e451 */
#include "agent.h"
#include "teek_ns_client.h"
#include "tui.h"
#include "smc.h"
#include "tc_ns_client.h"
#include "hisi_fb.h"
#include "libhwsecurec/securec.h"

/* add for CMA malloc framebuffer */
#include <linux/of.h>
#include <linux/of_fdt.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/workqueue.h>
#include <linux/sysfs.h>

static void tui_poweroff_work_func(struct work_struct *work);
static ssize_t tui_status_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf);
static DECLARE_DELAYED_WORK(tui_poweroff_work, tui_poweroff_work_func);

static struct kobject *tui_kobj;
static struct kobj_attribute tui_attribute =
	__ATTR(c_state, 0440, tui_status_show, NULL);
static struct attribute *attrs[] = {
	&tui_attribute.attr,
	NULL,
};

static struct attribute_group tui_attr_group = {
	.attrs = attrs,
};

/* command from secure os */
#define TUI_CMD_ENABLE		1
#define TUI_CMD_DISABLE		2
#define TUI_CMD_POLL		3
#define TUI_CMD_SET_STATE	4
#define TUI_CMD_PAUSE		5
#define TUI_CMD_DO_SYNC		6
#define TUI_CMD_START_DELAY_WORK	7
#define TUI_CMD_CANCEL_DELAY_WORK	8
#define TUI_CMD_SEND_INPUT_WORK 9

/* tui states */
#define TUI_STATE_UNUSED	0
#define TUI_STATE_CONFIG	1
#define TUI_STATE_RUNNING	2
#define TUI_STATE_ERROR	3

/* tui-max should be bigger than TUI_POLL_MAX in tui.h*/
static const char *const poll_event_type_name[] = {
	"config-ok",
	"config-fail",
	"tp",
	"tui-tick",
	"tui-delaywork",
	"tui-pause",
	"tui-resume",
	"tui-terminate",
	"tui-handle",
	"tui-hs",
	"tui-sh",
	"tui-01",
	"tui-02",
	"tui-10",
	"tui-20",
	"tui-0",
	"tui-90",
	"tui-180",
	"tui-270",
	"tui-k0",
	"tui-k3",
	"tui-SEMI",
	"tui-cursor",
	"tui-gettp",
	"tui-max"
};

static const char *const state_name[] = {
	"unused",
	"config",
	"running",
	"error"
};

#define tui_dbg(fmt, args...)	pr_info("[tui]: %s :" fmt, \
		__func__, ## args)
#define tui_err(fmt, args...)	pr_err("[tui]: %s :" fmt, \
		__func__, ## args)

struct tui_ctl_shm {
	struct {
		int command;
		int value;
		int ret;
	} s2n;
	struct {
		int event_type;
		int value;
		unsigned int addr;
		unsigned int addr_h;
		int tp_info;
		int tp_info_h_addr;
		int status;
		int x;
		int y;
	} n2s;
};

struct tui_msg_node {
	int type;
	int val;
	void *data;
	struct list_head list;
};

struct tui_drv_node {
	tui_drv_init init_func;
	void *pdata;
	char name[TUI_DRV_NAME_MAX];
	int state;
	int priority;
	struct list_head list;
};

static struct task_struct *tui_task;
static struct tui_ctl_shm *tui_ctl;

static spinlock_t tui_msg_lock;
static struct list_head tui_msg_head;

static atomic_t tui_state = ATOMIC_INIT(TUI_STATE_UNUSED);

DEFINE_MUTEX(tui_drv_lock);
static struct list_head tui_drv_head = LIST_HEAD_INIT(tui_drv_head);

static unsigned int tui_attached_device;

static wait_queue_head_t tui_state_wq;
static int tui_state_flag;

static wait_queue_head_t tui_msg_wq;
static wait_queue_head_t wait_event_get_keyCode;
static int get_keyCode_flag;
static int tui_msg_flag;
struct hisi_fb_data_type *dss_fd;

#define TUI_DSS_NAME "DSS"
#define TUI_GPIO_NAME "fff0d000.gpio"
#define TUI_TP_NAME "tp"
#define TUI_FP_NAME "fp"
char *dss_driver = TUI_DSS_NAME;
char *gpio_driver = TUI_GPIO_NAME;
char *tp_driver = TUI_TP_NAME;

#define DRIVER_NUM 4
/*do fp init(disable fp irq) before gpio init in order not response
* sensor in normal world(when gpio secure status is set)*/
char *init_driver[DRIVER_NUM] = {TUI_DSS_NAME, TUI_TP_NAME,
				 TUI_FP_NAME, TUI_GPIO_NAME
				};
char *deinit_driver[DRIVER_NUM] = {TUI_DSS_NAME, TUI_GPIO_NAME,
				   TUI_FP_NAME, TUI_TP_NAME
				  };
#define TIME_OUT_FOWER_ON 100

#define COLOR_TYPE	4  /*ARGB*/
#define BUFFER_NUM 2

/*tui-need-memory is calculated dynamically according to the screen resolution*/
struct tui_mem{
	unsigned int tui_addr_size;
	unsigned int tui_addr;
	unsigned int tui_addr_h;
	struct device *tui_dev;
	char *tui_virt;
};

static struct tui_mem g_tui_mem;

extern void create_mapping_late(phys_addr_t phys, unsigned long virt,
		                                phys_addr_t size, pgprot_t prot);

int tui_mem_alloc(struct device *class_dev)
{
	u64 len;
	int ret;
	struct device_node *node;
	const char *status = NULL;
	char *buff_virt;
	phys_addr_t buff_phy = 0;

	node = of_find_node_by_path("/reserved-memory/tui-mem");
	if (!node) {
		tui_err("not find tui mem\n");
		return -1;
	}

	ret = of_property_read_string(node, "status", &status);
	if(ret){
		tui_err("get status error\n");
		return -1;
	}

	if (status && strncmp(status, "ok",strlen("ok")) != 0){
		tui_err("status is error\n");
		return -1;
	}

	if(of_property_read_u64(node, "size", &len)) {
		tui_err("get tui-mem size error");
		return -1;
	}

	buff_virt = (void *)dma_alloc_coherent(class_dev, len,
		&buff_phy, GFP_KERNEL);
	if (buff_virt == NULL) {
		tui_err("tui dma_alloc_coherent failed\n");
		return -1;
	}

	create_mapping_late((phys_addr_t)buff_phy,
			(unsigned long)phys_to_virt(buff_phy), (phys_addr_t)len,
			__pgprot(PROT_DEVICE_nGnRE));
	g_tui_mem.tui_addr = buff_phy;
	g_tui_mem.tui_addr_h = buff_phy >> 32;
	g_tui_mem.tui_virt = buff_virt;
	g_tui_mem.tui_addr_size = len;
	g_tui_mem.tui_dev = class_dev;
	tui_dbg("tui_addr=0x%x \n", g_tui_mem.tui_addr);
	return 0;
}

void tui_mem_free(void)
{
	create_mapping_late((phys_addr_t)g_tui_mem.tui_addr,
			(unsigned long)phys_to_virt(g_tui_mem.tui_addr),
			(phys_addr_t)g_tui_mem.tui_addr_size, __pgprot(PAGE_KERNEL));
	dma_free_coherent(g_tui_mem.tui_dev,
		g_tui_mem.tui_addr_size, g_tui_mem.tui_virt, g_tui_mem.tui_addr);
}

unsigned int get_frame_addr(void)
{
	u64 num=1;
	int screen_r;

	if(!dss_fd)
		return 0;

	screen_r = dss_fd->panel_info.xres *
	    dss_fd->panel_info.yres*COLOR_TYPE*BUFFER_NUM;
	if(g_tui_mem.tui_addr_size < (num<<fls(screen_r))){
		tui_err("tui memory is not enough size=0x%x screen=(%d*%d)\n",
			g_tui_mem.tui_addr_size,dss_fd->panel_info.xres,dss_fd->panel_info.yres);
		return 0;
	}
	else
		return g_tui_mem.tui_addr;
}

int register_tui_driver(tui_drv_init fun, const char *name,
			void *pdata, int priority)
{
	struct tui_drv_node *tui_drv, *pos;
	int rc;

	/* Return error if name is invalid */
	if (!name || !fun) {
		TCERR("name or func is null");
		return -EINVAL;
	}

	if (!strncmp(name, TUI_DSS_NAME, (size_t)TUI_DRV_NAME_MAX)) {
		if (!pdata)
			return -1;
		else
			dss_fd = (struct hisi_fb_data_type *)pdata;
	}

	if (!strncmp(name, TUI_TP_NAME, (size_t)TUI_DRV_NAME_MAX) && !pdata)
		return -1;

	mutex_lock(&tui_drv_lock);

	/* name should not have been registered */
	list_for_each_entry(pos, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX-1)) {
			tui_err("this drv(%s) have registered\n", name);
			mutex_unlock(&tui_drv_lock);
			return -EINVAL;
		}
	}
	mutex_unlock(&tui_drv_lock);

	/* Allocate memory for tui_drv */
	tui_drv = kzalloc(sizeof(*tui_drv), GFP_KERNEL);
	if (!tui_drv)
		return -ENOMEM;

	/* Assign content for tui_drv */
	tui_drv->init_func = fun;
	tui_drv->pdata = pdata;
	tui_drv->priority = priority;

	rc = strncpy_s(tui_drv->name, TUI_DRV_NAME_MAX-1, name, TUI_DRV_NAME_MAX-1);
	if (rc != 0) {
		tui_err("strncpy_s error\n");
		kfree(tui_drv);
		return -1;
	}

	INIT_LIST_HEAD(&tui_drv->list);

	/* Link the new tui_drv to the list */
	mutex_lock(&tui_drv_lock);
	list_add_tail(&tui_drv->list, &tui_drv_head);
	mutex_unlock(&tui_drv_lock);

	return 0;
}

void unregister_tui_driver(const char *name)
{
	struct tui_drv_node *pos;

	/* Return error if name is invalid */
	if (!name) {
		TCERR("name is null");
		return;
	}

	mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX)) {
			list_del(&pos->list);
			kfree(pos);
			break;
		}
	}
	mutex_unlock(&tui_drv_lock);
}

static int add_tui_msg(int type, int val, void *data)
{
	struct tui_msg_node *tui_msg;
	unsigned long flags;

	/* Return error if pdata is invalid */
	if (!data) {
		TCERR("data is null");
		return -EINVAL;
	}

	/* Allocate memory for tui_msg */
	tui_msg = kmalloc(sizeof(*tui_msg), GFP_KERNEL);
	if (!tui_msg)
		return -ENOMEM;

	/* Assign the content of tui_msg */
	tui_msg->type = type;
	tui_msg->val = val;
	tui_msg->data = data;
	INIT_LIST_HEAD(&tui_msg->list);

	/* Link the new tui_msg to the list */
	spin_lock_irqsave(&tui_msg_lock, flags);
	list_add_tail(&tui_msg->list, &tui_msg_head);
	tui_msg_flag = 1;
	spin_unlock_irqrestore(&tui_msg_lock, flags);

	return 0;
}
/*WARNING: Too many leading tabs - consider code refactoring*/
/* secure : 0-unsecure, 1-secure */
static int init_tui_driver(int priority, int secure)
{
	struct tui_drv_node *pos;
	char *drv_name = NULL;
	char **drv_array = deinit_driver;
	int count = 0;
	int i = 0;

	if (secure)
		drv_array = init_driver;
	while (i < DRIVER_NUM) {
		drv_name = drv_array[i];
		i++;
		mutex_lock(&tui_drv_lock);

		/* Search all the tui_drv in their list */
		list_for_each_entry(pos, &tui_drv_head, list) {
			if (!strncmp(drv_name, pos->name, TUI_DRV_NAME_MAX)) {
				/* The names match. */

				tui_dbg("init tui drv(%s) state=%d\n",
					pos->name, secure);
				if (!strncmp(TUI_TP_NAME, pos->name, TUI_DRV_NAME_MAX)) {
					/* If the name is "tp", assign pos->pdata to tui_ctl */
					tui_ctl->n2s.tp_info =
						virt_to_phys(pos->pdata);
					tui_ctl->n2s.tp_info_h_addr =
						virt_to_phys(pos->pdata) >> 32;
				}
				if (pos->init_func) {
					if (!strncmp(TUI_DSS_NAME, pos->name,
						     TUI_DRV_NAME_MAX) && secure) {
						tui_dbg("init_tui_driver wait power on status---\n");
						while (!dss_fd->panel_power_on &&
						       count < TIME_OUT_FOWER_ON) {
							count++;
							msleep(20);
						}
						if (count == TIME_OUT_FOWER_ON) {
							/* Time out. So return error. */
							mutex_unlock(&tui_drv_lock);
							tui_err("wait status time out\n");
							return -1;
						}
					}
					if (!secure) {
						tui_dbg("drv(%s) state=%d, priority=%d\n",
							pos->name, secure, priority);
						if (pos->state == 0)
							continue;
						if (pos->init_func(pos->pdata, secure)) {
							/* Process init_func() fail */
							pos->state = -1;
							mutex_unlock(&tui_drv_lock);
							return -1;
						}
						/* set secure state will be proceed in tui msg */
						pos->state = 0;
					} else if (secure && priority == pos->priority) {
						tui_dbg("init tui drv(%s) state=%d, priority is %d\n",
							pos->name, secure, priority);
						/*when init, tp and dss should be async*/
						if (pos->init_func(pos->pdata, secure)) {
							pos->state = -1;
							mutex_unlock(&tui_drv_lock);
							return -1;
						}
					}
				}
			}
		}
		mutex_unlock(&tui_drv_lock);
	}

	return 0;
}

/* Only after all drivers cfg ok or some one failed, it need
 * to add_tui_msg.
 * ret val:	 1 - all cfg ok
 *			 0 - cfg is not complete, or have done
 *			-1 - cfg failed
 *			-2 - invalid name
 */
static int tui_cfg_filter(const char *name, bool ok)
{
	struct tui_drv_node *pos;
	char find = 0;
	int lock_flag = 0;

	/* Return error if name is invalid */
	if (!name) {
		TCERR("name is null");
		return -2;
	}

	/* some drivers may call send_tui_msg_config at the end
	 * of drv_init_func which had got the lock.
	 */
	if (mutex_is_locked(&tui_drv_lock))
		lock_flag = 1;
	if (!lock_flag)
		mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list) {
		if (!strncmp(pos->name, name, TUI_DRV_NAME_MAX)) {
			find = 1;
			if (ok)
				pos->state = 1;
			else {
				mutex_unlock(&tui_drv_lock);
				return -1;
			}
		}
	}
	if (!lock_flag)
		mutex_unlock(&tui_drv_lock);

	if (find == 0)
		return -2;

	return 1;
}

enum poll_class {
	CLASS_POLL_CONFIG,
	CLASS_POLL_RUNNING,
	CLASS_POLL_COMMON
};

static inline enum poll_class tui_poll_class(int event_type)
{
	enum poll_class class = CLASS_POLL_COMMON;

	switch (event_type) {
	case TUI_POLL_CFG_OK:
	case TUI_POLL_CFG_FAIL:
	case TUI_POLL_RESUME_TUI:
		class = CLASS_POLL_CONFIG;
		break;
	case TUI_POLL_TP:
	case TUI_POLL_TICK:
	case TUI_POLL_DELAYED_WORK:
	case TUI_POLL_PAUSE_TUI:
		class = CLASS_POLL_RUNNING;
		break;
	case TUI_POLL_CANCEL:
		class = CLASS_POLL_COMMON;
		break;
	default:
		break;
	}
	return class;
}

int send_tui_msg_config(int type, int val, void *data)
{
	int ret;

	if (type >= TUI_POLL_MAX  || type < 0 || !data) {
		tui_err("invalid tui event type\n");
		return -EINVAL;
	}

	/* The tui_state should be CONFIG */
	if (atomic_read(&tui_state) != TUI_STATE_CONFIG) {
		tui_err("failed to send tui msg(%s)\n",
			poll_event_type_name[type]);
		return -EINVAL;
	}

	if (tui_poll_class(type) == CLASS_POLL_RUNNING) {
		tui_err("invalid tui event type(%s) in config state\n",
			poll_event_type_name[type]);
		return -EINVAL;
	}

	tui_dbg("send config event type %s(%s)\n",
		poll_event_type_name[type], (char *)data);

	if (type == TUI_POLL_CFG_OK || type == TUI_POLL_CFG_FAIL) {
		int cfg_ret;

		cfg_ret = tui_cfg_filter((const char *)data,
					 type == TUI_POLL_CFG_OK);
		tui_dbg("tui driver(%s) cfg ret = %d\n", (char *)data, cfg_ret);
		if (cfg_ret == -2)
			return -EINVAL;
	}

	ret = add_tui_msg(type, val, data);
	if (ret) {
		tui_err("add tui msg ret=%d\n", ret);
		return ret;
	}

	tui_dbg("add config msg type %s\n", poll_event_type_name[type]);

	/* wake up tui kthread */
	wake_up(&tui_msg_wq);

	return 0;
}


/* Send tui event by smc_cmd */
int tui_send_event(int event)
{
	TC_NS_SMC_CMD smc_cmd = { 0 };
	unsigned char uuid[17] = { 0 };
	TC_NS_Operation operation = { 0 };
	uint32_t uid;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 13, 0))
	kuid_t kuid;

	kuid = current_uid();	/*lint !e666 */
	uid = kuid.val;
#else
	uid = current_uid();
#endif

	tui_dbg("need send tui event = %d\n", event);
	uuid[0] = 1;
	smc_cmd.uuid_phys = virt_to_phys(uuid);
	smc_cmd.uuid_h_phys = virt_to_phys(uuid) >> 32;
	smc_cmd.cmd_id = GLOBAL_CMD_ID_TUI_EXCEPTION;
	smc_cmd.agent_id = event;
	smc_cmd.operation_phys = virt_to_phys(&operation);
	smc_cmd.operation_h_phys = virt_to_phys(&operation) >> 32;
	smc_cmd.uid = uid;

	return TC_NS_SMC(&smc_cmd, 0);

}

/* Get key code from tui_ctl->s2n.value */
int tui_get_keyCode(void)
{
	int r;

	if (!tui_ctl) {
		tui_dbg("tui ctl abnormal----\n");
		return -1;
	}

	tui_dbg("wait_event_interruptible----------\n");
	r = wait_event_interruptible(wait_event_get_keyCode, get_keyCode_flag);
	if (r == -ERESTARTSYS) {
		tui_send_event(TUI_POLL_CANCEL);
		return -1;
	}

	tui_dbg("get tui keyCode is interrupted and keyCode=%d---\n",
		tui_ctl->s2n.value);
	get_keyCode_flag = 0;
	return tui_ctl->s2n.value;
}

static void tui_poweroff_work_func(struct work_struct *work)
{
	tui_send_event(TUI_POLL_CANCEL);
}

void tui_poweroff_work_start(void)
{
	tui_dbg("tui_poweroff_work_start----------\n");

	if (atomic_read(&tui_state) != TUI_STATE_UNUSED
	    && dss_fd->panel_power_on) {
		tui_dbg("come in tui_poweroff_work_start state=%d--\n",
			atomic_read(&tui_state));
		queue_work(system_wq, &tui_poweroff_work.work);
	}
}

static void wait_tui_msg(void)
{
	if (wait_event_interruptible(tui_msg_wq, tui_msg_flag))
		tui_err("get tui state is interrupted\n");
}

static int valid_msg(int msg_type)
{
	switch (msg_type) {
	case TUI_POLL_RESUME_TUI:
		if (atomic_read(&tui_state) == TUI_STATE_RUNNING)
			return 0;
		break;
	case TUI_POLL_CANCEL:
		if (atomic_read(&tui_state) == TUI_STATE_UNUSED)
			return 0;
		break;
	default:
		break;
	}

	return 1;
}
/*
 * 1: init ok
 * 0: still do init
 * -1: init failed
 */
static int get_cfg_state(char *name)
{
	struct tui_msg_node *tui_msg;

	/* Return error if name is invalid */
	if (!name) {
		TCERR("name is null");
		return -1;
	}

	list_for_each_entry(tui_msg, &tui_msg_head, list) {
		/* Names match */
		if (!strncmp(tui_msg->data, name, TUI_DRV_NAME_MAX)) {
			if (tui_msg->type == TUI_POLL_CFG_OK)
				return 1;
			else if (tui_msg->type == TUI_POLL_CFG_FAIL)
				return -1;
		}
	}

	return 0;
}
static void tui_msg_del(char *name)
{
	struct tui_msg_node *tui_msg;

	/* Return error if name is invalid */
	if (!name) {
		TCERR("name is null");
		return;
	}

	list_for_each_entry(tui_msg, &tui_msg_head, list) {
		/* Names match */
		if (!strncmp(tui_msg->data, name, TUI_DRV_NAME_MAX)) {
			list_del(&tui_msg->list);
			kfree(tui_msg);
			break;
		}
	}
}
#define DSS_CONFIG_INDEX (1)
#define TP_CONFIG_INDEX (2)
static void process_tui_msg(void)
{
	unsigned int val = 0;
	int type = TUI_POLL_CFG_OK;

fetch_msg:
	spin_lock(&tui_msg_lock);
	if (tui_ctl->s2n.value == DSS_CONFIG_INDEX) {
		/* Wait, until DSS init finishs */
		while (get_cfg_state(TUI_DSS_NAME) == 0) {
			tui_dbg("waiting for dss tui msg\n");
			tui_msg_flag = 0;
			spin_unlock(&tui_msg_lock);
			wait_tui_msg();
			tui_dbg("get dss init ok tui msg\n");
			spin_lock(&tui_msg_lock);
		}
		if (get_cfg_state(TUI_DSS_NAME) == -1) {
			tui_err("dss init failed\n");
			type = TUI_POLL_CFG_FAIL;
		}
		/* Delete DSS msg from tui_msg_head */
		tui_msg_del(TUI_DSS_NAME);
	} else if (tui_ctl->s2n.value == TP_CONFIG_INDEX) {
		while (get_cfg_state(TUI_TP_NAME) == 0) {
			tui_dbg("waiting for tp tui msg\n");
			tui_msg_flag = 0;
			spin_unlock(&tui_msg_lock);
			wait_tui_msg();
			tui_dbg("get tp init ok tui msg\n");
			spin_lock(&tui_msg_lock);
		}
		if (get_cfg_state(TUI_TP_NAME) == -1) {
			tui_err("tp failed to do init\n");
			type = TUI_POLL_CFG_FAIL;
			tui_msg_del(TUI_TP_NAME);
			goto next;
		}
		tui_msg_del(TUI_TP_NAME);
#if defined (CONFIG_TEE_TUI_HI3650)
		spin_unlock(&tui_msg_lock);
		if (init_tui_driver(1, 1) == 0) {
			spin_lock(&tui_msg_lock);
			while (get_cfg_state(TUI_GPIO_NAME) == 0 ||
			       get_cfg_state(TUI_FP_NAME) == 0) {
				tui_dbg("waiting for gpio/fp tui msg\n");
				tui_msg_flag = 0;
				spin_unlock(&tui_msg_lock);
				wait_tui_msg();
				tui_dbg("get gpio/fp init ok tui msg\n");
				spin_lock(&tui_msg_lock);
			}
			if (get_cfg_state(TUI_GPIO_NAME) == -1 ||
			    get_cfg_state(TUI_FP_NAME) == -1) {
				tui_err("one of gpio/fp failed to do init\n");
				type = TUI_POLL_CFG_FAIL;
			}

		}
		tui_msg_del(TUI_GPIO_NAME);
		tui_msg_del(TUI_FP_NAME);
#endif
		tui_dbg("tp/gpio/fp is config result:type = 0x%x\n", type);
	}
next:
	spin_unlock(&tui_msg_lock);

	/* pre-process tui poll event if needed */
	switch (type) {
	case TUI_POLL_CFG_OK:
		if (tui_ctl->s2n.value == DSS_CONFIG_INDEX){
			tui_ctl->n2s.addr = get_frame_addr();
			tui_ctl->n2s.addr_h = g_tui_mem.tui_addr_h;
			if(tui_ctl->n2s.addr == 0)
				val = -1;
		}
		break;
	default:
		break;
	}

	tui_ctl->n2s.event_type = type;
	tui_ctl->n2s.value = val;

	if (!valid_msg(tui_ctl->n2s.event_type)) {
		tui_dbg("refetch tui msg\n");
		goto fetch_msg;
	}
}

static int init_tui_agent(void)
{
	TC_NS_Shared_MEM tui_tc_shm = { 0 };
	int ret;

	tui_ctl = (struct tui_ctl_shm *)kzalloc((size_t)SZ_4K, GFP_KERNEL);
	if (!tui_ctl) {
		tui_err("failed to allocate 4k bytes for tui_ctl\n");
		return -ENOMEM;
	}

	tui_tc_shm.kernel_addr = tui_ctl;
	tui_tc_shm.len = SZ_4K;

	ret = TC_NS_register_agent(NULL, TEE_TUI_AGENT_ID, &tui_tc_shm);
	if (ret) {
		tui_err("register tui agent failed\n");
		kfree(tui_ctl);
	}

	return ret;
}

static void exit_tui_agent(void)
{
	if (TC_NS_unregister_agent(TEE_TUI_AGENT_ID))
		tui_err("unregister tui agent failed\n");
	kfree(tui_ctl);
}

static void set_tui_state(int state)
{
	if (atomic_read(&tui_state) != state) {
		atomic_set(&tui_state, state);
		tui_state_flag = 1;
		wake_up(&tui_state_wq);
	}
}

static int do_tui_work(void)
{
	int ret = 0;

	/* clear s2n cmd ret */
	tui_ctl->s2n.ret = 0;
	switch (tui_ctl->s2n.command) {
	case TUI_CMD_ENABLE:
		if (atomic_read(&tui_state) != TUI_STATE_CONFIG) {
			tui_dbg("tui enable\n");
			set_tui_state(TUI_STATE_CONFIG);
			/*do dss and tp init*/
			if (init_tui_driver(0, 1)) {
				tui_ctl->s2n.ret = -1;
				set_tui_state(TUI_STATE_ERROR);
			}
		}
		break;
	case TUI_CMD_DISABLE:
		if (atomic_read(&tui_state) != TUI_STATE_UNUSED) {
			tui_dbg("tui disable\n");
			init_tui_driver(0, 0);
			set_tui_state(TUI_STATE_UNUSED);
		}
		break;
	case TUI_CMD_PAUSE:
		if (atomic_read(&tui_state) != TUI_STATE_UNUSED) {
			tui_dbg("tui pause\n");
			init_tui_driver(0, 0);
			set_tui_state(TUI_STATE_CONFIG);
		}
		break;
	case TUI_CMD_POLL:
		process_tui_msg();
		break;
	case TUI_CMD_DO_SYNC:
		/*if (tp_sync_func) {*/
		tui_dbg("enable tp irq cmd\n");
		/*		tp_sync_func();*/
		/*		tui_timeout_reset();*/
		/*}*/
		break;
	case TUI_CMD_SET_STATE:
		tui_dbg("tui set state %d\n", tui_ctl->s2n.value);
		set_tui_state(tui_ctl->s2n.value);
		break;
	case TUI_CMD_START_DELAY_WORK:
		tui_dbg("start delay work\n");
		break;
	case TUI_CMD_CANCEL_DELAY_WORK:
		tui_dbg("cancel delay work\n");
		break;
	case TUI_CMD_SEND_INPUT_WORK:
		tui_dbg("send input work\n");
		get_keyCode_flag = 1;
		wake_up(&wait_event_get_keyCode);
		break;
	default:
		ret = -EINVAL;
		tui_err("get error tui command\n");
		break;
	}
	return ret;
}

void set_tui_attach_device(unsigned int id)
{
	tui_attached_device = id;
}

unsigned int tui_attach_device(void)
{
	return tui_attached_device;
}

static int tui_kthread_work_fn(void *data)
{
	int ret;

	ret = init_tui_agent();
	if (ret)
		return ret;

	while (1) {
		/*tui_dbg("tui before sleep  1\n");*/
		TC_NS_wait_event(TEE_TUI_AGENT_ID);
		/*tui_dbg("tui after sleep	 2\n");*/

		if (kthread_should_stop())
			break;

		do_tui_work();

		TC_NS_send_event_response(TEE_TUI_AGENT_ID);
	}

	exit_tui_agent();

	return 0;
}

#define READ_BUF 128
static ssize_t tui_dbg_state_read(struct file *filp, char __user *ubuf,
				  size_t cnt, loff_t *ppos)
{
	char buf[READ_BUF];
	unsigned int r;
	int ret;
	struct tui_drv_node *pos;

	ret = snprintf_s(buf, READ_BUF, READ_BUF, "tui state:%s\n",
		     state_name[atomic_read(&tui_state)]);
	if(ret < 0)
	    return -EINVAL;
	r = (unsigned int)ret;

	ret = snprintf_s(buf + r, READ_BUF - r, READ_BUF - r, "drv config state:");
	if(ret < 0)
	    return -EINVAL;
	r += (unsigned int)ret;
	mutex_lock(&tui_drv_lock);
	list_for_each_entry(pos, &tui_drv_head, list)
	r += snprintf_s(buf + r, READ_BUF - r, READ_BUF - r, "%s-%s,", pos->name,
		      pos->state == 1 ? "ok" : "no ok");/* [false alarm]:buffer足够大不会越界  */
	buf[r - 1] = '\n';
	mutex_unlock(&tui_drv_lock);

	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}

static const struct file_operations tui_dbg_state_fops = {
	.owner = THIS_MODULE,
	.read = tui_dbg_state_read,
};

static ssize_t tui_status_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	int r;

	tui_state_flag = 0;
	r = wait_event_interruptible(tui_state_wq, tui_state_flag);
	if (r) {
		tui_err("get tui state is interrupted\n");
		return r;
	}

	r = snprintf_s(buf, 32, 32, "%s", state_name[atomic_read(&tui_state)]);

	return strlen(buf);
}

#define MSG_BUF 512
static ssize_t tui_dbg_msg_read(struct file *filp, char __user *ubuf,
				size_t cnt, loff_t *ppos)
{
	char buf[MSG_BUF];
	unsigned int r;
	int ret;
	int i;
	struct tui_drv_node *pos;

	ret = snprintf_s(buf, MSG_BUF, MSG_BUF, "%s", "event format: event_type:val\n"
		     "event type:\n");
	if(ret < 0)
	    return -EINVAL;
	r = (unsigned int)ret;

	/* event type list */
	for (i = 0; i < TUI_POLL_MAX - 1; i++){
		ret = snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r, "%s, ",
			      poll_event_type_name[i]);/* [false alarm]:buffer足够大不会越界 */
		if(ret < 0)
			return -EINVAL;
		r += (unsigned int)ret;
	}

	r += snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r, "%s\n", poll_event_type_name[i]);

	/* cfg drv type list */
	r += snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r, "val type for %s or %s:\n",
		      poll_event_type_name[TUI_POLL_CFG_OK],
		      poll_event_type_name[TUI_POLL_CFG_FAIL]);
	list_for_each_entry(pos, &tui_drv_head, list)
	r += snprintf_s(buf + r, MSG_BUF - r, MSG_BUF - r, "%s,", pos->name);

	buf[r - 1] = '\n';
	return simple_read_from_buffer(ubuf, cnt, ppos, buf, r);
}

static ssize_t tui_dbg_msg_write(struct file *filp,
				 const char __user *ubuf, size_t cnt,
				 loff_t *ppos)
{
	char buf[64];
	int i;
	int event_type = -1;
	char *tokens, *begins;
	int ret;

	if (cnt >= sizeof(buf))
		return -EINVAL;

	if (copy_from_user(&buf, ubuf, cnt))
		return -EFAULT;

	buf[cnt] = 0;

	begins = buf;

	/* event type */
	tokens = strsep(&begins, ":");
	tui_dbg("1: tokens:%s\n", tokens);
	for (i = 0; i < TUI_POLL_MAX; i++) {
		if (!strncmp(tokens, poll_event_type_name[i],
			     strlen(poll_event_type_name[i]))) {
			event_type = i;
			break;
		}
	}

	/* only for tp and cancel  */
	if(event_type != TUI_POLL_TP  && event_type != TUI_POLL_CANCEL)
		return -EFAULT;
	/* drv type */
	tokens = strsep(&begins, ":");
	if(!tokens)
		return -EFAULT;
	tui_dbg("2: tokens:%s\n", tokens);
	if (event_type == TUI_POLL_TP) {
		long value = 0;
		int base = 10;

		/* simple_strtol is obsolete, use kstrtol instead*/
		ret = kstrtol(tokens, base, &value);
		tui_ctl->n2s.status = value;

		tokens = strsep(&begins, ":");
		if(!tokens)
			return -EFAULT;
		ret = kstrtol(tokens, base, &value);
		tui_ctl->n2s.x = value;

		tokens = strsep(&begins, ":");
		if(!tokens)
			return -EFAULT;
		ret = kstrtol(tokens, base, &value);
		tui_ctl->n2s.y = value;
	}
	tui_dbg("status=%d x=%d y=%d\n",
		tui_ctl->n2s.status, tui_ctl->n2s.x, tui_ctl->n2s.y);

	tui_send_event(event_type);

	*ppos += cnt;

	return cnt;
}

static const struct file_operations tui_dbg_msg_fops = {
	.owner = THIS_MODULE,
	.read = tui_dbg_msg_read,
	.write = tui_dbg_msg_write,
};

static struct dentry *dbg_dentry;

int __init init_tui(struct device *class_dev)
{
	int retval;
	struct sched_param param;
	param.sched_priority = MAX_RT_PRIO - 1;

	if (!class_dev)
		return -1;

	retval = tui_mem_alloc(class_dev);
	if(retval) {
		tui_err("tui alloc fail\n");
		return -ENOMEM;
	}

	tui_task = kthread_create(tui_kthread_work_fn, NULL, "tuid");
	if (IS_ERR(tui_task)) {
		tui_mem_free();
		return PTR_ERR(tui_task);
	}

	sched_setscheduler_nocheck(tui_task, SCHED_FIFO, &param);
	get_task_struct(tui_task);

	wake_up_process(tui_task);

	INIT_LIST_HEAD(&tui_msg_head);
	spin_lock_init(&tui_msg_lock);

	init_waitqueue_head(&tui_state_wq);
	init_waitqueue_head(&tui_msg_wq);
	init_waitqueue_head(&wait_event_get_keyCode);
	dbg_dentry = debugfs_create_dir("tui", NULL);
#ifdef DEBUG_TUI
	debugfs_create_file("message", 0440, dbg_dentry,
			    NULL, &tui_dbg_msg_fops);
#endif
	debugfs_create_file("d_state", 0440, dbg_dentry,
			    NULL, &tui_dbg_state_fops);
	tui_kobj = kobject_create_and_add("tui", kernel_kobj);
	if (!tui_kobj) {
		tui_err("tui kobj create error\n");
		retval =  -ENOMEM;
		goto error2;
	}
	retval = sysfs_create_group(tui_kobj, &tui_attr_group);

	if (retval) {
		tui_err("sysfs_create_group error\n");
		goto error1;
	}

	return 0;
error1:
	kobject_put(tui_kobj);
error2:
	kthread_stop(tui_task);
	tui_mem_free();
	return retval;

}

void tui_exit(void)
{
	tui_mem_free();
	kthread_stop(tui_task);
	put_task_struct(tui_task);
	debugfs_remove(dbg_dentry);
	sysfs_remove_group(tui_kobj, &tui_attr_group);
	kobject_put(tui_kobj);
}
