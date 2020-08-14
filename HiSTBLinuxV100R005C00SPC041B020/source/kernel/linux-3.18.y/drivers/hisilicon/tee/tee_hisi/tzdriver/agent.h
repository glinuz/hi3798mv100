/*
 * FileName:
 * Author:		   v00297436
 * Description:
 * Version:
 * Function List:
 */

#ifndef _AGENT_H_
#define _AGENT_H__
#ifdef CONFIG_DEVCHIP_PLATFORM
#include "rpmb.h"
#else
#include <linux/mmc/rpmb.h>
#endif
#include <linux/fs.h>
#include "teek_ns_client.h"

#define AGENT_FS_ID 0x46536673	/*FSfs*/
#define AGENT_MISC_ID 0x4d495343	/*MISC*/
#define AGENT_RPMB_ID 0x4abe6198	/*RPMB*/
#define AGENT_SOCKET_ID 0x69e85664	/*socket*/

struct __smc_event_data *find_event_control(unsigned int agent_id);

/*for secure agent*/
struct __smc_event_data {
	unsigned int agent_id;
	int agent_alive;
	struct mutex work_lock;
	wait_queue_head_t wait_event_wq;
	int ret_flag;
	wait_queue_head_t send_response_wq;
	int send_flag;
	struct list_head head;
	TC_NS_SMC_CMD cmd;
	TC_NS_DEV_File *owner;
	TC_NS_Shared_MEM *buffer;
};

struct tee_agent_kernel_ops {
	const char *agent_name;	/* MUST NOT be NULL*/
	unsigned int agent_id;	/* MUST NOT be zero*/
	int (*tee_agent_init)(struct tee_agent_kernel_ops *agent_instance);
	int (*tee_agent_run)(struct tee_agent_kernel_ops *agent_instance);
	/* MUST NOT be NULL*/
	int (*tee_agent_work)(struct tee_agent_kernel_ops *agent_instance);
	int (*tee_agent_stop)(struct tee_agent_kernel_ops *agent_instance);
	int (*tee_agent_exit)(struct tee_agent_kernel_ops *agent_instance);
	int (*tee_agent_crash_work)(struct tee_agent_kernel_ops *
					agent_instance,
					TC_NS_ClientContext *context,
					unsigned int dev_file_id);
	struct task_struct *agent_thread;
	void *agent_data;
	TC_NS_Shared_MEM *agent_buffer;
	struct list_head list;
};



int agent_init(void);

int agent_exit(void);

unsigned int agent_process_work(TC_NS_SMC_CMD *smc_cmd, unsigned int agent_id);

int is_agent_alive(unsigned int agent_id);

int TC_NS_set_nativeCA_hash(unsigned long arg);

int TC_NS_register_agent(TC_NS_DEV_File *dev_file,
								 unsigned int agent_id,
								 TC_NS_Shared_MEM *shared_mem);
int TC_NS_unregister_agent(unsigned int agent_id);
int TC_NS_unregister_agent_client(TC_NS_DEV_File *dev_file);

unsigned int TC_NS_incomplete_proceed(TC_NS_SMC_CMD *smc_cmd,
								unsigned int agent_id,
								uint8_t flags);

int TC_NS_wait_event(unsigned int agent_id);

int TC_NS_send_event_response(unsigned int agent_id);
void TC_NS_send_event_response_all(void);

int TC_NS_sync_sys_time(TC_NS_Time *tc_ns_time);

int tee_agent_clear_work(TC_NS_ClientContext *context, unsigned int dev_file_id);
int tee_agent_kernel_register(struct tee_agent_kernel_ops *new_agent);

extern int mmc_blk_ioctl_rpmb_cmd(enum func_id id,
						struct block_device *bdev,
						struct mmc_blk_ioc_rpmb_data *idata);

extern struct mmc_card *get_mmc_card(struct block_device *bdev);

#ifdef CONFIG_MMC
extern int rpmb_agent_register(void);
#endif

#endif /*_AGENT_H_*/
