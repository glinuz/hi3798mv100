#include <linux/fs.h>
#include <linux/mmc/ioctl.h>	/* for struct mmc_ioc_rpmb */
#include <linux/mmc/card.h>	/* for struct mmc_card */
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#ifdef CONFIG_DEVCHIP_PLATFORM
#include "rpmb.h"
#else
#include <linux/mmc/rpmb.h>
#endif

#include "tee_client_constants.h"
/*#define TC_DEBUG*/
#include "teek_ns_client.h"
#include "agent.h"
#include "libhwsecurec/securec.h"
#include "tc_ns_log.h"
#ifdef CONFIG_DEVCHIP_PLATFORM
/*#define TEE_RPMB_DEBUG*/

#ifdef TEE_RPMB_DEBUG
#define HI_PRINT_HEX(str, len) \
{\
    unsigned int _i = 0;\
    unsigned int _val; \
    unsigned char *_str = (unsigned char*)str;\
	if (_str != 0) \
    {\
		for ( _i = 0 ; _i < (len); _i+=4 )\
		{\
			_val  = *((_str)+_i+3);\
			_val |= *((_str)+_i+2) << 8;\
			_val |= *((_str)+_i+1) << 16;\
			_val |= *((_str)+_i+0) << 24;\
			if( (_i % 16 == 0) && (_i != 0)) printk("\n");\
			printk("0x%08x ", _val);\
		}\
		printk("\n");\
	}\
}

void show_ioccmd(struct rpmb_ctrl_t *trans_ctrl)
{
	int i;
	int j;
	int buf_bytes;
	unsigned char *buf;

	for (i = 0; i < STORAGE_IOC_MAX_RPMB_CMD; i++) {
		buf_bytes = (int)trans_ctrl->args.send_ioccmd.ioc_rpmb.data[i].buf_bytes;
		buf = trans_ctrl->args.send_ioccmd.ioc_rpmb.data[i].buf;
		printk("-------------cmd[%d] buf_bytes[0x%x]-----------------\n", i, buf_bytes);
		HI_PRINT_HEX(buf, buf_bytes);
	}
}
#endif
#endif

extern int hisi_rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation,
			       struct storage_blk_ioc_rpmb_data *storage_data);

typedef enum {
	sec_get_devinfo,
	sec_send_ioccmd,
	sec_rpmb_lock,
	sec_rpmb_unlock,
} rpmb_cmd_t;

#define RPMB_EMMC_CID_SIZE 32
struct rpmb_devinfo {
	uint8_t cid[RPMB_EMMC_CID_SIZE];	/* eMMC card ID */
	uint8_t rpmb_size_mult;	/* EXT CSD-slice 168 "RPMB Size" */
	/* EXT CSD-slice 222 "Reliable Write Sector Count" */
	uint8_t rel_wr_sec_cnt;
	uint32_t blk_size;	/* RPMB blocksize */
	/* The highest block index supported by current device */
	uint32_t max_blk_idx;
};
struct rpmb_ioc {
	struct storage_blk_ioc_rpmb_data ioc_rpmb;
	uint32_t buf_offset[STORAGE_IOC_MAX_RPMB_CMD];
	uint32_t buf[1];
};
struct rpmb_ctrl_t {
	rpmb_cmd_t cmd;
	int32_t ret;
	uint8_t lock_flag;
	enum rpmb_op_type op_type;
	union __args {
		struct rpmb_devinfo get_devinfo;
		struct rpmb_ioc send_ioccmd;
	} args;
};

#ifdef CONFIG_DEVCHIP_PLATFORM
#define RPMB_DEVICE_NAME "/dev/mmcblk0rpmb"
#define RPMB_DEVICE_NAME_ANDROID "/dev/block/mmcblk0rpmb"
static int rpmb_agent_open_blkdev(struct tee_agent_kernel_ops *agent_instance, int stage)
{
	struct block_device *bdev;
	//if (agent_instance->agent_data)
	//	 return 0;
	bdev = blkdev_get_by_path(RPMB_DEVICE_NAME, FMODE_READ | FMODE_WRITE | FMODE_NDELAY, 0);
	 /* for android version */
	 if (IS_ERR(bdev)) {
		 bdev = blkdev_get_by_path(RPMB_DEVICE_NAME_ANDROID,
				       FMODE_READ | FMODE_WRITE | FMODE_NDELAY, 0);
	 }
	if ((NULL==bdev) || IS_ERR(bdev)) {
		TCERR("open %s failed with %ld in stage-%d\n",
			RPMB_DEVICE_NAME, PTR_ERR(bdev), stage);
		agent_instance->agent_data = NULL;
		return -1;
	}
	TCDEBUG("open %s success in stage-%d\n", RPMB_DEVICE_NAME, stage);
	agent_instance->agent_data = bdev;
	return 0;
}

static int rpmb_agent_close_blkdev(struct tee_agent_kernel_ops *agent_instance)
{
	struct block_device *bdev;

	if (agent_instance->agent_data) {
		bdev = (struct block_device *)agent_instance->agent_data;
		blkdev_put(bdev, FMODE_READ | FMODE_WRITE | FMODE_NDELAY);
	}
	return 0;
}

static void get_devinfo(struct tee_agent_kernel_ops *agent_instance)
{
	struct rpmb_ctrl_t *trans_ctrl =
		(struct rpmb_ctrl_t *)agent_instance->agent_buffer->kernel_addr;
	struct block_device *bdev;
	struct mmc_card *card;
	uint8_t tmp_cid[RPMB_EMMC_CID_SIZE+1] = {0};

	if (agent_instance == NULL) {
		TCERR("agent_instance is NULL\n");
		return;
	}

	TCDEBUG("open blkdev: agent_name = %s\n", agent_instance->agent_name);
	if (rpmb_agent_open_blkdev(agent_instance, 1)) {
		trans_ctrl->ret = -1;
		return;
	}
	bdev = (struct block_device *)agent_instance->agent_data;
	TCDEBUG("get mmc card");
	card = get_mmc_card(bdev);
	if ((NULL==card) || IS_ERR(card)) {
		TCERR("get mmc card fail\n");
		//rpmb_agent_close_blkdev(agent_instance);
		trans_ctrl->ret = -1;
		return;
	}
	TCDEBUG("EMMC Card info:\n");
	TCDEBUG("cid: %08x%08x%08x%08x\n", card->raw_cid[0], card->raw_cid[1],
		card->raw_cid[2], card->raw_cid[3]);
	TCDEBUG("raw_rpmb_size_mult=0x%x, rel_wr_sec_cnt=0x%x\n",
		card->ext_csd.raw_rpmb_size_mult,
		card->ext_csd.rel_sectors);

	/* Get EMMC CID */
	snprintf(tmp_cid, sizeof(tmp_cid),
		"%08x%08x%08x%08x", card->raw_cid[0], card->raw_cid[1],
		card->raw_cid[2], card->raw_cid[3]);
	memcpy(trans_ctrl->args.get_devinfo.cid, tmp_cid, RPMB_EMMC_CID_SIZE);

	/* Get RPMB size */
	trans_ctrl->args.get_devinfo.rpmb_size_mult =
		card->ext_csd.raw_rpmb_size_mult;

	/* Get reliable write sector count */
	trans_ctrl->args.get_devinfo.rel_wr_sec_cnt = card->ext_csd.rel_sectors;
	trans_ctrl->ret = 0;

	rpmb_agent_close_blkdev(agent_instance);
	TCDEBUG("End\n");
}
#endif

/*
 * the data_ptr from SecureOS is physical address,
 * so, we MUST update to the virtual address,
 * otherwise, segment default
 */
static void update_dataptr(struct rpmb_ctrl_t *trans_ctrl)
{
	uint32_t i, offset = 0;
	uint8_t *dst;

	if (NULL == trans_ctrl)
		return;

	for (i = 0; i < STORAGE_IOC_MAX_RPMB_CMD; i++) {
		offset = trans_ctrl->args.send_ioccmd.buf_offset[i];
		if (trans_ctrl->args.send_ioccmd.ioc_rpmb.data[i].buf) {
			dst =
			    (uint8_t *) trans_ctrl->args.send_ioccmd.buf +
			    offset;
			/*update the data_ptr */
			trans_ctrl->args.send_ioccmd.ioc_rpmb.data[i].buf = dst;
		}
	}
}

struct rpmb_agent_lock_info {
	unsigned int dev_id;
	bool lock_need_free;
};
static struct rpmb_agent_lock_info lock_info = { 0 };

static int process_rpmb_lock(struct tee_agent_kernel_ops *agent_instance)
{
	struct __smc_event_data *event_data;

	if (NULL == agent_instance)
		return -1;

	mutex_lock(&rpmb_counter_lock);
	tlogd("obtain rpmb device lock\n");

	event_data = find_event_control(agent_instance->agent_id);
	if (event_data) {
		lock_info.dev_id = event_data->cmd.dev_file_id;
		lock_info.lock_need_free = true;
		tlogd("rpmb counter lock context: dev_id=%d\n",
			lock_info.dev_id);
	}
	return 0;
}

static int process_rpmb_unlock(struct tee_agent_kernel_ops *agent_instance)
{
	errno_t rc = EOK;
	rc = memset_s(&lock_info, sizeof(lock_info), 0, sizeof(lock_info));/*lint !e838*/
	if (rc != EOK) {
		return -1;
	}

	lock_info.lock_need_free = false;
	mutex_unlock(&rpmb_counter_lock);
	tlogd("free rpmb device lock\n");
	return 0;
}

#define GET_RPMB_LOCK_MASK 0x01
#define FREE_RPMB_LOCK_MASK 0x02
static void send_ioccmd(struct tee_agent_kernel_ops *agent_instance)
{
	struct rpmb_ctrl_t *trans_ctrl;
	uint8_t lock_flag;
	int32_t ret;

	if (NULL == agent_instance || NULL == agent_instance->agent_buffer
		|| NULL == agent_instance->agent_buffer->kernel_addr)
		return;

	trans_ctrl = (struct rpmb_ctrl_t *)agent_instance->agent_buffer->kernel_addr;
	lock_flag = trans_ctrl->lock_flag;

	update_dataptr(trans_ctrl);
	if (lock_flag & GET_RPMB_LOCK_MASK)
		process_rpmb_lock(agent_instance);
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef TEE_RPMB_DEBUG
	show_ioccmd(trans_ctrl);
#endif
#endif
	ret =
	    hisi_rpmb_ioctl_cmd(RPMB_FUNC_ID_SECURE_OS, trans_ctrl->op_type,
				&trans_ctrl->args.send_ioccmd.ioc_rpmb);
#ifdef CONFIG_DEVCHIP_PLATFORM
#ifdef TEE_RPMB_DEBUG
	printk("-------------after hisi_rpmb_ioctl_cmd-----------------\n");
	show_ioccmd(trans_ctrl);
#endif
#endif
	if (ret)
		tloge("mmc_blk_ioctl_rpmb_cmd failed: %d\n", ret);

	/*TODO: if globalTask or TA in TrustedCore is crash in middle,
	 * will never free this lock*/
	if (lock_flag & FREE_RPMB_LOCK_MASK)
		process_rpmb_unlock(agent_instance);
	trans_ctrl->ret = ret;
}

static int rpmb_agent_work(struct tee_agent_kernel_ops *agent_instance)
{
	struct rpmb_ctrl_t *trans_ctrl;

	if (NULL == agent_instance || NULL == agent_instance->agent_buffer
		|| NULL == agent_instance->agent_buffer->kernel_addr)
		return -1;

	trans_ctrl = (struct rpmb_ctrl_t *)agent_instance->agent_buffer->kernel_addr;

	switch (trans_ctrl->cmd) {
	case sec_get_devinfo:
		tlogd("rpmb agent cmd is get_devinfo, agent_instance=0x%x\n", agent_instance);
#ifdef CONFIG_DEVCHIP_PLATFORM
		get_devinfo(agent_instance);
#else
		trans_ctrl->ret = -1;
#endif
		break;
	case sec_send_ioccmd:
		tlogd("rpmb agent cmd is send ioc\n");
		send_ioccmd(agent_instance);
		break;
	case sec_rpmb_lock:
		tlogd("rpmb agent cmd is lock\n");
		process_rpmb_lock(agent_instance);
		trans_ctrl->ret = 0;
		break;
	case sec_rpmb_unlock:
		tlogd("rpmb agent cmd is unlock\n");
		process_rpmb_unlock(agent_instance);
		trans_ctrl->ret = 0;
		break;
	default:
		break;
	}
	return 0;
}

static int rpmb_agent_crash_work(struct tee_agent_kernel_ops *agent_instance,
				 TC_NS_ClientContext * context,
				 unsigned int dev_file_id)
{
	tlogd("check free lock or not, dev_id=%d\n", dev_file_id);
	if (lock_info.lock_need_free && (lock_info.dev_id == dev_file_id)) {
		tloge("CA crash, need to free lock\n");
		process_rpmb_unlock(agent_instance);
	}
	return 0;
}

static struct tee_agent_kernel_ops rpmb_agent_ops = {
	.agent_name = "rpmb",
	.agent_id = 0x4abe6198,
	.tee_agent_work = rpmb_agent_work,
	.tee_agent_crash_work = rpmb_agent_crash_work,

	.list = LIST_HEAD_INIT(rpmb_agent_ops.list)
};

int rpmb_agent_register(void)
{
	tee_agent_kernel_register(&rpmb_agent_ops);
	return 0;
}

EXPORT_SYMBOL(rpmb_agent_register);
