/*
 * =============================================================================
 *    Description:  hisi secure rpmb solution
 *
 *        Created:  05/01/2015
 *
 *         Author:  Zang Leigang (z00265047), zangleigang@hisilicon.com
 *   Organization:  HISILICON
 *
 * =============================================================================
 */
#include <linux/fs.h>
#ifdef CONFIG_DEVCHIP_PLATFORM
#include "rpmb.h"
#else
#include <linux/mmc/rpmb.h>
#endif
#include <linux/mmc/core.h>
#include <linux/mmc/ioctl.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/printk.h>
#include <linux/dma-mapping.h>
#include <linux/wakelock.h>
#include <linux/time.h>

#include "hisi_rpmb.h"
#include <asm/uaccess.h>
#include "queue.h"
#include <linux/syscalls.h>
#include <linux/delay.h>

#define EMMC_RPMB_BLOCK_DEVICE_NAME "/dev/mmcblk0rpmb"
#define EMMC_RPMB_BLOCK_DEVICE_NAME_ANDROID "/dev/block/mmcblk0rpmb"

/* external function declaration */
int mmc_blk_ioctl_rpmb_cmd(enum func_id id,
			   struct block_device *bdev,
			   struct mmc_blk_ioc_rpmb_data *idata);
extern struct mmc_blk_data *mmc_blk_get_ext(struct gendisk *disk);
extern void mmc_blk_put_ext(struct mmc_blk_data *md);
extern int mmc_blk_part_switch_ext(struct mmc_card *card, struct mmc_blk_data *md);
extern int ioctl_rpmb_card_status_poll_ext(struct mmc_card *card,
				       u32 *status,
				       u32 retries_max);

unsigned int  g_start_time = 0;
unsigned int  g_end_time = 0;
unsigned int  g_time_cost = 0;
struct hisi_rpmb {
	struct rpmb_request *rpmb_request;
	struct device *dev;
	struct block_device *blkdev;
	struct workqueue_struct *rpmb_workqueue;
	struct work_struct rpmb_work;
	struct wake_lock wake_lock;
};

/*
 * There is one mmc_blk_data per slot.
 */
struct mmc_blk_data {
	spinlock_t	lock;
	struct gendisk	*disk;
	struct mmc_queue queue;
	struct list_head part;

	unsigned int	flags;
#define MMC_BLK_CMD23	(1 << 0)	/* Can do SET_BLOCK_COUNT for multiblock */
#define MMC_BLK_REL_WR	(1 << 1)	/* MMC Reliable write support */
#define MMC_BLK_PACKED_CMD	(1 << 2)	/* MMC packed command support */

	unsigned int	usage;
	unsigned int	read_only;
	unsigned int	part_type;
	unsigned int	name_idx;
	unsigned int	reset_done;
#define MMC_BLK_READ		BIT(0)
#define MMC_BLK_WRITE		BIT(1)
#define MMC_BLK_DISCARD		BIT(2)
#define MMC_BLK_SECDISCARD	BIT(3)

	/*
	 * Only set in main mmc_blk_data associated
	 * with mmc_card with mmc_set_drvdata, and keeps
	 * track of the current selected device partition.
	 */
	unsigned int	part_curr;
	struct device_attribute force_ro;
	struct device_attribute power_ro_lock;
	int	area_type;
};


static struct hisi_rpmb hisi_rpmb;
#define BOOT_DEVICE_EMMC 0
static int rpmb_support_device = BOOT_DEVICE_EMMC;
static int rpmb_drivers_init_status = RPMB_DRIVER_IS_NOT_READY;

DEFINE_MUTEX(rpmb_counter_lock);

static inline void mmc_rpmb_combine_cmd(struct mmc_blk_ioc_data *data,
					int is_write,
					unsigned short blocks,
					unsigned short blksz,
					bool reliable)
{
	struct mmc_ioc_cmd *ioc;
	ioc = &data->ic;

	ioc->write_flag = is_write | ((is_write & reliable) << 31);
	ioc->opcode =
		is_write ? MMC_WRITE_MULTIPLE_BLOCK : MMC_READ_MULTIPLE_BLOCK;
	ioc->flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	ioc->arg = 0x0;
	ioc->blksz = blksz;
	ioc->blocks = blocks;
}
static inline void mmc_rpmb_combine_request(struct mmc_blk_ioc_data *data,
					    struct rpmb_frame *frame,
					    int is_write,
					    unsigned short blocks,
					    unsigned short blksz,
					    bool reliable)
{
	data->buf = (void *)frame;
	data->buf_bytes = (u64)blocks * blksz;
	mmc_rpmb_combine_cmd(data, is_write, blocks, blksz, reliable);
}

static inline void
mmc_rpmb_basic_request(uint32_t index,
		       struct rpmb_request *request,
		       struct mmc_blk_ioc_rpmb_data *rpmb_data,
		       uint16_t block_count,
		       uint32_t reliable)
{
	mmc_rpmb_combine_request(&rpmb_data->data[0], &request->frame[0], true,
				 block_count, RPMB_BLK_SZ, reliable);
}

static inline void
mmc_rpmb_status_request(uint32_t index,
			struct rpmb_request *request,
			struct mmc_blk_ioc_rpmb_data *rpmb_data,
			uint16_t block_count,
			uint32_t reliable)
{
	struct rpmb_frame *status_frame = &request->status_frame;
	memset(status_frame, 0, sizeof(*status_frame));
	status_frame->request = cpu_to_be16(RPMB_REQ_STATUS);

	mmc_rpmb_combine_request(&rpmb_data->data[1], status_frame, true, 0x1,
				 RPMB_BLK_SZ, false);
}

static inline void
mmc_rpmb_result_request(uint32_t index,
			struct rpmb_request *request,
			struct mmc_blk_ioc_rpmb_data *rpmb_data,
			uint16_t block_count,
			uint32_t reliable)
{
	mmc_rpmb_combine_request(&rpmb_data->data[index], &request->frame[0],
				 false, block_count, RPMB_BLK_SZ, false);
}

void mmc_rpmb_set_key(struct rpmb_request *request,
		      struct mmc_blk_ioc_rpmb_data *rpmb_data)
{
	mmc_rpmb_basic_request(0, request, rpmb_data, 0x1, true);
	mmc_rpmb_status_request(1, request, rpmb_data, 0x1, false);
	mmc_rpmb_result_request(2, request, rpmb_data, 0x1, false);
}

void mmc_rpmb_get_counter(struct rpmb_request *request,
			  struct mmc_blk_ioc_rpmb_data *rpmb_data)
{
	mmc_rpmb_basic_request(0, request, rpmb_data, 0x1, false);
	mmc_rpmb_result_request(1, request, rpmb_data, 0x1, false);
}

void mmc_rpmb_read_data(struct rpmb_request *request,
			struct mmc_blk_ioc_rpmb_data *rpmb_data)
{
	unsigned short blocks_count = request->info.current_rqst.blks;

	mmc_rpmb_basic_request(0, request, rpmb_data, 0x1, false);
	mmc_rpmb_result_request(1, request, rpmb_data, blocks_count, false);
}

void mmc_rpmb_write_data(struct rpmb_request *request,
			 struct mmc_blk_ioc_rpmb_data *rpmb_data)
{
	unsigned short blocks_count = request->info.current_rqst.blks;

	mmc_rpmb_basic_request(0, request, rpmb_data, blocks_count, true);
	mmc_rpmb_status_request(1, request, rpmb_data, 0x1, false);
	mmc_rpmb_result_request(2, request, rpmb_data, 0x1, false);
}


/*
 * hisi_rpmb_active - handle rpmb request from ATF
 */
void hisi_rpmb_active(void)
{
	queue_work(hisi_rpmb.rpmb_workqueue, &hisi_rpmb.rpmb_work);
}

EXPORT_SYMBOL(hisi_rpmb_active);


#define WAIT_INIT_TIMES 3000
int get_rpmb_init_status(void)
{
	int i;
	int ret = 0;
	mm_segment_t oldfs = get_fs();
	set_fs(get_ds());
	for(i = 0;i < WAIT_INIT_TIMES;i++){
		ret = sys_access(EMMC_RPMB_BLOCK_DEVICE_NAME, 0);
		if(!ret)
			break;
		ret = sys_access(EMMC_RPMB_BLOCK_DEVICE_NAME_ANDROID, 0);
		if(!ret)
			break;
		usleep_range(3000, 5000);
	}
	if(i == WAIT_INIT_TIMES){
		pr_err("wait for device init timeout!\n");
		rpmb_drivers_init_status = RPMB_DRIVER_IS_NOT_READY;
	}else{
		rpmb_drivers_init_status = RPMB_DRIVER_IS_READY;
	}
	set_fs(oldfs);
	return rpmb_drivers_init_status;
}

int hisi_mmc_rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation, struct storage_blk_ioc_rpmb_data *storage_data){
	struct block_device *bdev;
	int ret;
	int i;
	struct mmc_blk_ioc_rpmb_data *rpmb_data = kzalloc(sizeof(*rpmb_data), GFP_KERNEL);
	if(NULL == rpmb_data){
		pr_err("alloc rpmb_data failed\n");
		return -1;
	}
	bdev = blkdev_get_by_path(EMMC_RPMB_BLOCK_DEVICE_NAME, FMODE_READ | FMODE_WRITE | FMODE_NDELAY, 0);
	if (IS_ERR(bdev)) {
		bdev = blkdev_get_by_path(EMMC_RPMB_BLOCK_DEVICE_NAME_ANDROID, FMODE_READ | FMODE_WRITE | FMODE_NDELAY, 0);
	}
    if (IS_ERR(bdev)) {
		pr_err("Secure OS open rpmb block failed!\n");
		kfree(rpmb_data);
        return -1;
    }

	for(i = 0;i < STORAGE_IOC_MAX_RPMB_CMD; i++){
		rpmb_data->data[i].buf = storage_data->data[i].buf;
		rpmb_data->data[i].buf_bytes = storage_data->data[i].buf_bytes;
	}
	switch (operation) {
		case RPMB_OP_RD:
			mmc_rpmb_combine_cmd(&rpmb_data->data[0], true, storage_data->data[0].blocks, RPMB_BLK_SZ, false);
			mmc_rpmb_combine_cmd(&rpmb_data->data[1], false, storage_data->data[1].blocks, RPMB_BLK_SZ, false);
			break;
		case RPMB_OP_WR_CNT:
			mmc_rpmb_combine_cmd(&rpmb_data->data[0], true, storage_data->data[0].blocks, RPMB_BLK_SZ, false);
			mmc_rpmb_combine_cmd(&rpmb_data->data[1], false, storage_data->data[1].blocks, RPMB_BLK_SZ, false);
		    break;
		case RPMB_OP_WR_DATA:
			mmc_rpmb_combine_cmd(&rpmb_data->data[0], true, storage_data->data[0].blocks, RPMB_BLK_SZ, true);
			mmc_rpmb_combine_cmd(&rpmb_data->data[1], true, storage_data->data[1].blocks, RPMB_BLK_SZ, false);
			mmc_rpmb_combine_cmd(&rpmb_data->data[2], false, storage_data->data[2].blocks, RPMB_BLK_SZ, false);
			break;
		case RPMB_OP_KEY:
			mmc_rpmb_combine_cmd(&rpmb_data->data[0], true, storage_data->data[0].blocks, RPMB_BLK_SZ, true);
			mmc_rpmb_combine_cmd(&rpmb_data->data[1], true, storage_data->data[1].blocks, RPMB_BLK_SZ, false);
			mmc_rpmb_combine_cmd(&rpmb_data->data[2], false, storage_data->data[2].blocks, RPMB_BLK_SZ, false);
			break;
	}
	ret = mmc_blk_ioctl_rpmb_cmd(id, bdev, rpmb_data);
	blkdev_put(bdev, FMODE_READ | FMODE_WRITE | FMODE_NDELAY);
	kfree(rpmb_data);
	return ret;
}

int hisi_rpmb_ioctl_cmd(enum func_id id, enum rpmb_op_type operation, struct storage_blk_ioc_rpmb_data *storage_data){
	int ret;
	if(RPMB_DRIVER_IS_NOT_READY == get_rpmb_init_status())
		goto error;

	if(BOOT_DEVICE_EMMC == rpmb_support_device){
		ret = hisi_mmc_rpmb_ioctl_cmd(id, operation, storage_data);
	}
	return ret;

	error:
		return -1;
}

static struct mmc_blk_ioc_rpmb_data *
mmc_blk_ioctl_rpmb_copy_data(struct mmc_blk_ioc_rpmb_data *rdata)
{
	struct mmc_blk_ioc_rpmb_data *idata;
	int err, i;

	idata = kzalloc(sizeof(*idata), GFP_KERNEL);
	if (!idata) {
		err = -ENOMEM;
		goto out;
	}

	for (i = 0; i < MMC_IOC_MAX_RPMB_CMD; i++) {
		idata->data[i].buf_bytes = rdata->data[i].buf_bytes;
		idata->data[i].buf =
			kzalloc(idata->data[i].buf_bytes, GFP_KERNEL);
		if (IS_ERR(idata->data[i].buf)) {
			err = PTR_ERR(idata->data[i].buf);
			goto copy_err;
		}
		memcpy(&idata->data[i].ic, &rdata->data[i].ic,
		       sizeof(struct mmc_ioc_cmd));
		memcpy(idata->data[i].buf, rdata->data[i].buf,
		       idata->data[i].buf_bytes);
	}

	return idata;

copy_err:
	while (--i >= 0) {
		kfree(idata->data[i].buf);
	}
	kfree(idata);
out:
	return ERR_PTR(err);
}

/*output api ++*/
struct mmc_card *get_mmc_card(struct block_device *bdev)
{
	struct mmc_blk_data *md;
	struct mmc_card *card;

	if (NULL == bdev) {
		pr_err("bdev is NULL\n");
		return NULL;
	}
	md = mmc_blk_get_ext(bdev->bd_disk);
	if (!md) {
		pr_err("md is NULL\n");
		return NULL;
	}

	card = md->queue.card;
	if (IS_ERR(card)) {
		return NULL;
	}

	return card;
}
EXPORT_SYMBOL(get_mmc_card);

/*This function is responsible for handling RPMB command and is the interface
 *with the eMMC driver.
 *It is used by BL31 and SecureOS.So when modify the fuction please check it
 *with SecureOS.
 *During DMA 64bit development, we modify it using the method of memory copy.
 *idata:the parameter consist of  two command at least and three commd at most,
 *so when copy retuning
 *      data, please confirm copy all the retuning data not include write
 *command.
 */
int mmc_blk_ioctl_rpmb_cmd(enum func_id id,
			   struct block_device *bdev,
			   struct mmc_blk_ioc_rpmb_data *rdata)
{
	struct mmc_blk_data *md;
	struct mmc_card *card;
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};
	struct mmc_request mrq = {NULL};
	struct scatterlist sg;
	struct mmc_blk_ioc_rpmb_data *idata = NULL;
	int err = 0, i = 0;
	u32 status = 0;

#if 0
	/* The caller must have CAP_SYS_RAWIO */
	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;
#endif
	md = mmc_blk_get_ext(bdev->bd_disk);
	/* make sure this is a rpmb partition */
	if ((!md) || (!(md->area_type & MMC_BLK_DATA_AREA_RPMB))) {
		err = -EINVAL;
		return err;
	}

	idata = mmc_blk_ioctl_rpmb_copy_data(rdata);
	if (IS_ERR(idata)) {
		err = PTR_ERR(idata);
		goto cmd_done;
	}

	card = md->queue.card;
	if (IS_ERR(card)) {
		err = PTR_ERR(card);
		goto idata_free;
	}

	mmc_get_card(card);
	/*mmc_claim_host(card->host);*/

	err = mmc_blk_part_switch_ext(card, md);
	if (err)
		goto cmd_rel_host;

	for (i = 0; i < MMC_IOC_MAX_RPMB_CMD; i++) {
		struct mmc_blk_ioc_data *curr_data;
		struct mmc_ioc_cmd *curr_cmd;

		curr_data = &idata->data[i];
		curr_cmd = &curr_data->ic;
		if (!curr_cmd->opcode)
			break;

		cmd.opcode = curr_cmd->opcode;
		cmd.arg = curr_cmd->arg;
		cmd.flags = curr_cmd->flags;

		if (curr_data->buf_bytes) {
			data.sg = &sg;
			data.sg_len = 1;
			data.blksz = curr_cmd->blksz;
			data.blocks = curr_cmd->blocks;

			sg_init_one(data.sg, curr_data->buf,
				    curr_data->buf_bytes);

			if (curr_cmd->write_flag)
				data.flags = MMC_DATA_WRITE;
			else
				data.flags = MMC_DATA_READ;

			/* data.flags must already be set before doing this. */
			mmc_set_data_timeout(&data, card);

			/*
			 * Allow overriding the timeout_ns for empirical tuning.
			 */
			if (curr_cmd->data_timeout_ns)
				data.timeout_ns = curr_cmd->data_timeout_ns;

			mrq.data = &data;
		}

		mrq.cmd = &cmd;

		err = mmc_set_blockcount(card, data.blocks,
					 curr_cmd->write_flag & (1 << 31));
		if (err)
			goto cmd_rel_host;

		mmc_wait_for_req(card->host, &mrq);

		if (cmd.error) {
			dev_err(mmc_dev(card->host), "%s: cmd error %d\n",
				__func__, cmd.error);
			err = cmd.error;
			goto cmd_rel_host;
		}
		if (data.error) {
			dev_err(mmc_dev(card->host), "%s: data error %d\n",
				__func__, data.error);
			err = data.error;
			goto cmd_rel_host;
		}

		memcpy(curr_cmd->response, cmd.resp, sizeof(cmd.resp));

		if (!curr_cmd->write_flag) {
			memcpy(rdata->data[i].buf, curr_data->buf,
			       curr_data->buf_bytes);
		}

		/*
		 * Ensure RPMB command has completed by polling CMD13
		 * "Send Status".
		 */
		err = ioctl_rpmb_card_status_poll_ext(card, &status, 5);
		if (err)
			dev_err(mmc_dev(card->host),
				"%s: Card Status=0x%08X, error %d\n", __func__,
				status, err);
	}

cmd_rel_host:
	mmc_put_card(card);
/*mmc_release_host(card->host);*/

idata_free:
	for (i = 0; i < MMC_IOC_MAX_RPMB_CMD; i++)
		kfree(idata->data[i].buf);

	kfree(idata);

cmd_done:
	mmc_blk_put_ext(md);

	return err;
}
