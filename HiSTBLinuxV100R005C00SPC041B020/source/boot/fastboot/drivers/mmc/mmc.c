/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <mmc.h>
#include <part.h>
#include <malloc.h>
#include <linux/list.h>
#include <div64.h>

#define MMC_RW_STEP_BLKS 0xFFFF /* 0xFFFF is the max blocks of CMD23 */
#define CAPS_NO_MASK 0xffffffff

static struct list_head mmc_devices;
static int cur_dev_num = -1;
static char test_ext_csd[512] = {0};

const u8 tuning_blk_pattern_4bit[MMC_TUNING_BLK_PATTERN_4BIT_SIZE] = {
	0xff, 0x0f, 0xff, 0x00, 0xff, 0xcc, 0xc3, 0xcc,
	0xc3, 0x3c, 0xcc, 0xff, 0xfe, 0xff, 0xfe, 0xef,
	0xff, 0xdf, 0xff, 0xdd, 0xff, 0xfb, 0xff, 0xfb,
	0xbf, 0xff, 0x7f, 0xff, 0x77, 0xf7, 0xbd, 0xef,
	0xff, 0xf0, 0xff, 0xf0, 0x0f, 0xfc, 0xcc, 0x3c,
	0xcc, 0x33, 0xcc, 0xcf, 0xff, 0xef, 0xff, 0xee,
	0xff, 0xfd, 0xff, 0xfd, 0xdf, 0xff, 0xbf, 0xff,
	0xbb, 0xff, 0xf7, 0xff, 0xf7, 0x7f, 0x7b, 0xde,
};

const u8 tuning_blk_pattern_8bit[MMC_TUNING_BLK_PATTERN_8BIT_SIZE] = {
	0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
	0xff, 0xff, 0xcc, 0xcc, 0xcc, 0x33, 0xcc, 0xcc,
	0xcc, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xff, 0xff,
	0xff, 0xee, 0xff, 0xff, 0xff, 0xee, 0xee, 0xff,
	0xff, 0xff, 0xdd, 0xff, 0xff, 0xff, 0xdd, 0xdd,
	0xff, 0xff, 0xff, 0xbb, 0xff, 0xff, 0xff, 0xbb,
	0xbb, 0xff, 0xff, 0xff, 0x77, 0xff, 0xff, 0xff,
	0x77, 0x77, 0xff, 0x77, 0xbb, 0xdd, 0xee, 0xff,
	0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0x00,
	0x00, 0xff, 0xff, 0xcc, 0xcc, 0xcc, 0x33, 0xcc,
	0xcc, 0xcc, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xff,
	0xff, 0xff, 0xee, 0xff, 0xff, 0xff, 0xee, 0xee,
	0xff, 0xff, 0xff, 0xdd, 0xff, 0xff, 0xff, 0xdd,
	0xdd, 0xff, 0xff, 0xff, 0xbb, 0xff, 0xff, 0xff,
	0xbb, 0xbb, 0xff, 0xff, 0xff, 0x77, 0xff, 0xff,
	0xff, 0x77, 0x77, 0xff, 0x77, 0xbb, 0xdd, 0xee,
};


#ifdef CONFIG_EMMC_PARAM_TAG
extern u32 emmc_caps2;
#endif

int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value);

int __board_mmc_getcd(u8 *cd, struct mmc *mmc) {
	return -1;
}

int board_mmc_getcd(u8 *cd, struct mmc *mmc)__attribute__((weak,
	alias("__board_mmc_getcd")));

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	return mmc->send_cmd(mmc, cmd, data);
}


int mmc_send_status(struct mmc *mmc, int timeout)
{
	struct mmc_cmd cmd;
	int err, retries = 5;

	cmd.cmdidx = MMC_CMD_SEND_STATUS;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;

	do {
		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (!err) {
			if ((cmd.response[0] & MMC_STATUS_RDY_FOR_DATA) &&
			    (cmd.response[0] & MMC_STATUS_CURR_STATE) !=
			     MMC_STATE_PRG)
				break;
			else if (cmd.response[0] & MMC_STATUS_MASK) {
				printf("Status Error: 0x%08X\n",
					cmd.response[0]);
				return COMM_ERR;
			}
		} else if (--retries < 0)
			return err;

		udelay(1000);

	} while (timeout--);

	if (timeout <= 0) {
		printf("Timeout waiting card ready\n");
		return TIMEOUT;
	}
	if (cmd.response[0] & MMC_STATUS_SWITCH_ERROR)
		return SWITCH_ERR;

	return 0;
}

int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;

	if ((mmc->timing & MMC_TIMING_UHS_DDR50)
		||(mmc->timing & MMC_TIMING_MMC_HS400))
		return 0;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

struct mmc *find_mmc_device(int dev_num)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		if (m->block_dev.dev == dev_num)
			return m;
	}

	printf("No MMC device(%d) driver found!\n", dev_num);

	return NULL;
}


static ulong mmc_erase_t(struct mmc *mmc, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	ulong end;
	int err, start_cmd, end_cmd;

	if (mmc->high_capacity) {
		end = start + blkcnt - 1;
	} else {
		end = (start + blkcnt - 1) * mmc->write_bl_len;
		start *= mmc->write_bl_len;
	}

	if (IS_SD(mmc)) {
		start_cmd = SD_CMD_ERASE_WR_BLK_START;
		end_cmd = SD_CMD_ERASE_WR_BLK_END;
	} else {
		start_cmd = MMC_CMD_ERASE_GROUP_START;
		end_cmd = MMC_CMD_ERASE_GROUP_END;
	}

	cmd.cmdidx = start_cmd;
	cmd.cmdarg = start;
	cmd.resp_type = MMC_RSP_R1;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = end_cmd;
	cmd.cmdarg = end;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	cmd.cmdidx = MMC_CMD_ERASE;
	cmd.cmdarg = SECURE_ERASE;
	cmd.resp_type = MMC_RSP_R1b;

	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err)
		goto err_out;

	return 0;

err_out:
	puts("mmc erase failed\n");
	return err;
}

unsigned long mmc_berase(int dev_num, lbaint_t start, lbaint_t blkcnt)
{
	int err = 0;
	struct mmc *mmc = find_mmc_device(dev_num);
	lbaint_t blk = 0, blk_r = 0;
	int timeout = 1000;

	if (!mmc)
		return -1;

	if ((start % mmc->erase_grp_size) || (blkcnt % mmc->erase_grp_size))
		printf("\n\nCaution! Your devices Erase group is 0x%x\n"
		       "The erase range would be change to "
		       "0x%lx~0x%lx\n\n",
		       mmc->erase_grp_size, start & ~(mmc->erase_grp_size - 1),
		       ((start + blkcnt + mmc->erase_grp_size)
		       & ~(mmc->erase_grp_size - 1)) - 1);

	while (blk < blkcnt) {
		blk_r = ((blkcnt - blk) > mmc->erase_grp_size) ?
			mmc->erase_grp_size : (blkcnt - blk);
		err = mmc_erase_t(mmc, start + blk, blk_r);
		if (err)
			break;

		blk += blk_r;

		/* Waiting for the ready status */
		if (mmc_send_status(mmc, timeout))
			return 0;
	}

	if ((mmc->sec_feature_support & EXT_CSD_SEC_SANITIZE)
		&& (mmc->sec_feature_support & EXT_CSD_SEC_GB_CL_EN)) {
		printf("sanitize ... ");
		err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				 EXT_CSD_SANITIZE_START, 1);
		if (err) {
			printf("sanitize failed!\n");
			return -1;
		}
	}
	return blk;
}

static ulong
mmc_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void*src)
{
	struct mmc_cmd cmd;
	struct mmc_cmd sbc;
	struct mmc_data data;
	int err;
	struct mmc *mmc = find_mmc_device(dev_num);
	int stoperr;
	int blklen;
	u32 total;
	s32 cnt;
	int timeout = 1000;
	char *databuf = (char *)src;
	ulong blkaddr = start;

	if (!mmc)
		return -1;

	blklen = mmc->write_bl_len;

	err = mmc_set_blocklen(mmc, mmc->write_bl_len);

	if (err) {
		printf("set write bl len failed\n\r");
		return err;
	}

	cnt = blkcnt;
	total = 0;
	while (cnt > 0) {
		u32 step;
		if (cnt > MMC_RW_STEP_BLKS) {
			step = MMC_RW_STEP_BLKS;
			cnt -= MMC_RW_STEP_BLKS;
		} else {
			step = cnt;
			cnt = 0;
		}

		if (step > 1)
			cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
		else
			cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

		if (mmc->high_capacity)
			cmd.cmdarg = blkaddr;
		else
			cmd.cmdarg = blkaddr * blklen;

		cmd.resp_type = MMC_RSP_R1;
		cmd.flags = 0;

		data.src = databuf;
		data.blocks = step;
		data.blocksize = blklen;
		data.flags = MMC_DATA_WRITE;

		sbc.cmdidx = 0;
		if ((mmc->host_caps & MMC_MODE_CMD23)
			&& (MMC_CMD_WRITE_MULTIPLE_BLOCK == cmd.cmdidx)) {
			sbc.cmdidx = MMC_CMD_SET_BLOCK_COUNT;
			sbc.cmdarg = data.blocks;
			sbc.resp_type = MMC_RSP_R1;
			sbc.flags = 0;
			err = mmc_send_cmd(mmc, &sbc, NULL);
			if (err) {
				printf("mmc set blk count failed\n\r");
				return err;
			}
		}

		err = mmc_send_cmd(mmc, &cmd, &data);

		if (err) {
			printf("mmc write failed\n\r");
			return err;
		}

		if (!sbc.cmdidx && step > 1) {
			cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
			cmd.cmdarg = 0;
			cmd.resp_type = MMC_RSP_R1b;
			cmd.flags = 0;
			stoperr = mmc_send_cmd(mmc, &cmd, NULL);
			if (stoperr) {
				printf("mmc write stop failed!\n");
				return stoperr;
			}
		}

		/* Waiting for the ready status */
		if (mmc_send_status(mmc, timeout))
			return 0;

		databuf  += step * blklen;
		blkaddr  += step;
		total    += step;
	}

	return total;
}

int mmc_read_block(struct mmc *mmc, void *dst, uint blocknum)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = blocknum;
	else
		cmd.cmdarg = blocknum * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.dest = dst;
	data.blocks = 1;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}

int mmc_read(struct mmc *mmc, u64 src, uchar *dst, int size)
{
	char *buffer;
	int i;
	int blklen = mmc->read_bl_len;
	int startblock = lldiv(src, mmc->read_bl_len);
	int endblock = lldiv(src + size - 1, mmc->read_bl_len);
	int err = 0;

	/* Make a buffer big enough to hold all the blocks we might read */
	buffer = malloc(blklen);

	if (!buffer) {
		printf("Could not allocate buffer for MMC read!\n");
		return -1;
	}

	/* We always do full block reads from the card */
	err = mmc_set_blocklen(mmc, mmc->read_bl_len);

	if (err)
		goto free_buffer;

	for (i = startblock; i <= endblock; i++) {
		int segment_size;
		int offset;

		err = mmc_read_block(mmc, buffer, i);

		if (err)
			goto free_buffer;

		/*
		 * The first block may not be aligned, so we
		 * copy from the desired point in the block
		 */
		offset = (src & (blklen - 1));
		segment_size = MIN(blklen - offset, size);

		memcpy(dst, buffer + offset, segment_size);

		dst += segment_size;
		src += segment_size;
		size -= segment_size;
	}

free_buffer:
	free(buffer);

	return err;
}

/* surport multiblk read */
static ulong mmc_mbread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
	struct mmc_cmd cmd;
	struct mmc_cmd sbc;
	struct mmc_data data;
	int err;
	int stoperr = 0;
	u32 total;
	s32 cnt;
	int timeout = 1000;
	char *databuf = dst;
	ulong blkaddr = start;

	struct mmc *mmc = find_mmc_device(dev_num);

	if (!mmc)
		return 0;

	/* We always do full block reads from the card */
	err = mmc_set_blocklen(mmc, mmc->read_bl_len);

	if (err) {
		printf("set read bl len failed\n");
		return err;
	}

	cnt = blkcnt;
	total = 0;
	while (cnt > 0) {
		u32 step;
		if (cnt > MMC_RW_STEP_BLKS) {
			step = MMC_RW_STEP_BLKS;
			cnt -= MMC_RW_STEP_BLKS;
		} else {
			step = cnt;
			cnt = 0;
		}

		if (step > 1)
			cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
		else
			cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

		if (mmc->high_capacity)
			cmd.cmdarg = blkaddr;
		else
			cmd.cmdarg = blkaddr * mmc->read_bl_len;

		cmd.resp_type = MMC_RSP_R1;
		cmd.flags = 0;

		data.dest = databuf;
		data.blocks = step;
		data.blocksize = mmc->read_bl_len;
		data.flags = MMC_DATA_READ;

		sbc.cmdidx = 0;
		if ((mmc->host_caps & MMC_MODE_CMD23)
			&& (MMC_CMD_READ_MULTIPLE_BLOCK == cmd.cmdidx)) {
			sbc.cmdidx = MMC_CMD_SET_BLOCK_COUNT;
			sbc.cmdarg = data.blocks;
			sbc.resp_type = MMC_RSP_R1;
			sbc.flags = 0;
			err = mmc_send_cmd(mmc, &sbc, NULL);
			if (err) {
				printf("mmc set blk count failed\n\r");
				return err;
			}
		}

		err = mmc_send_cmd(mmc, &cmd, &data);

		if (err) {
			printf("mmc read failed\n\r");
			return err;
		}

		if (!sbc.cmdidx && step > 1) {
			cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
			cmd.cmdarg = 0;
			cmd.resp_type = MMC_RSP_R1b;
			cmd.flags = 0;
			stoperr = mmc_send_cmd(mmc, &cmd, NULL);
			if (stoperr) {
				printf("mmc read stop failed!\n");
				return stoperr;
			}

		}

		/* Waiting for the ready status */
		if (mmc_send_status(mmc, timeout))
			return 0;

		databuf  += step * mmc->read_bl_len;
		blkaddr  += step;
		total    += step;
	}

	return total;
}

int mmc_send_tuning(struct mmc* mmc, u32 opcode)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	const u8 *tuning_block_pattern = NULL;
	int size, err = 0;
	u8 *data_buf;

	memset(&cmd, 0, sizeof(cmd));
	memset(&data, 0, sizeof(data));
	switch (opcode) {
		case MMC_CMD_SEND_TUNING_BLOCK_HS200:
			if (mmc->bus_width == 8) {
				tuning_block_pattern = tuning_blk_pattern_8bit;
				size = sizeof(tuning_blk_pattern_8bit);
			} else if (mmc->bus_width == 4){
				tuning_block_pattern = tuning_blk_pattern_4bit;
				size = sizeof(tuning_blk_pattern_4bit);
			}else {
				return -EINVAL;
			}
			break;
		case MMC_CMD_SEND_EXT_CSD:
			size = 512;
			break;
		default:
			return -EINVAL;
	}

	data_buf = malloc(size);
	if (!data_buf)
		return -ENOMEM;

	memset(data_buf, 0 ,size);

	cmd.cmdidx = opcode;
	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.dest = (char *)data_buf;
	data.blocks = 1;
	data.blocksize = size;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	if (opcode != MMC_CMD_SEND_EXT_CSD) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.resp_type = MMC_RSP_R1;

		mmc_send_cmd(mmc, &cmd, NULL);

		if (!err) {
			if (memcmp(data_buf, tuning_block_pattern, size))
				err = -EIO;

			mmc_send_status(mmc, 1000);
		}
	}

	if ((opcode == MMC_CMD_SEND_EXT_CSD) && (err)) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.resp_type = MMC_RSP_R1;
		mmc_send_cmd(mmc, &cmd, NULL);
	}
	free(data_buf);
	return err;
}

int mmc_go_idle(struct mmc* mmc)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	udelay(2000);

	return 0;
}

int
sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set. However, Some controller
		 * can set bit 7 (reserved for low voltages), but
		 * how to manage low voltages SD card is not yet
		 * specified.
		 */
		cmd.cmdarg = mmc->voltages & 0xff8000;

		if (mmc->version == SD_VERSION_2)
			cmd.cmdarg |= OCR_HCS;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc->version != SD_VERSION_2)
		mmc->version = SD_VERSION_1_0;

	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}

int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	struct mmc_cmd cmd;
	int err;

	/* Some cards seem to need this */
	mmc_go_idle(mmc);

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = OCR_HCS | mmc->voltages;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}


int mmc_send_ext_csd(struct mmc *mmc, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	/* Get the Card Status Register */
	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	return err;
}


int mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;
	int timeout = 1000;
	int ret;

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
				 (index << 16) |
				 (value << 8);
	cmd.flags = 0;

	ret = mmc_send_cmd(mmc, &cmd, NULL);

	/* Waiting for the ready status */
	if (!ret)
		ret = mmc_send_status(mmc, timeout);

	return ret;

}

int mmc_change_freq(struct mmc *mmc)
{
	char ext_csd[512];
	char cardtype;
	u32 dev_caps;
	struct emmc_dev * dev;
	int err;

	mmc->card_caps = 0;

	/* Only version 4 supports high-speed */
	if (mmc->version < MMC_VERSION_4)
		return 0;

	mmc->card_caps |= MMC_MODE_4BIT | MMC_MODE_8BIT;

	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;
	cardtype = ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_MASK;

	err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

	if (err)
		return err == SWITCH_ERR ? 0 : err;

	/* Now check to see that it worked */
	err = mmc_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	/* No high-speed support */
	if (!ext_csd[EXT_CSD_HS_TIMING])
		return 0;

	dev = mmc_get_ids_dev(mmc->cid);
	if (dev) {
		dev_caps = dev->caps;
#ifdef CONFIG_EMMC_PARAM_TAG
		if ((dev_caps & EXT_CSD_CARD_TYPE_HS400) == 0)
			emmc_caps2 &= ~((MMC_MODE_HS400)>>7);
		if ((dev_caps & EXT_CSD_CARD_TYPE_SDR_200) == 0)
			emmc_caps2 &= ~((MMC_MODE_HS200)>>7);
#endif
	} else
		dev_caps = CAPS_NO_MASK;

	if (cardtype & EXT_CSD_CARD_TYPE_SDR_200 & dev_caps) {
		mmc->card_caps |= MMC_MODE_HS200;
	}

	if (cardtype & EXT_CSD_CARD_TYPE_HS400  & dev_caps) {
		mmc->card_caps |= MMC_MODE_HS400;
	}

	/* High Speed is set, there are two types: 52MHz and 26MHz */
	if (cardtype & EXT_CSD_CARD_TYPE_52) {
		if (cardtype & EXT_CSD_CARD_TYPE_DDR_52)
			mmc->card_caps |= MMC_MODE_DDR_52MHz;
		mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	} else
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << (group * 4));
	cmd.cmdarg |= value << (group * 4);
	cmd.flags = 0;

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}


int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	uint scr[2];
	uint switch_status[16];
	struct mmc_data data;
	int timeout;

	mmc->card_caps = 0;

	/* Read the SCR to find out if this card supports higher speeds */
	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	timeout = 3;

retry_scr:
	data.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	if (err) {
		if (timeout--)
			goto retry_scr;

		return err;
	}

	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	switch ((mmc->scr[0] >> 24) & 0xf) {
		case 0:
			mmc->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc->version = SD_VERSION_1_10;
			break;
		case 2:
			mmc->version = SD_VERSION_2;
			break;
		default:
			mmc->version = SD_VERSION_1_0;
			break;
	}

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0)
		return 0;

	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, 0, 1,
				(u8 *)&switch_status);

		if (err)
			return err;

		/* The high-speed function is busy.  Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY))
			break;
	}

	if (mmc->scr[0] & SD_DATA_4BIT)
		mmc->card_caps |= MMC_MODE_4BIT;

	/* If high-speed isn't supported, we return */
	if (!(__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED))
		return 0;

	err = sd_switch(mmc, SD_SWITCH_SWITCH, 0, 1, (u8 *)&switch_status);

	if (err)
		return err;

	if ((__be32_to_cpu(switch_status[4]) & 0x0f000000) == 0x01000000)
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

/* frequency bases */
/* divided by 10 to be nice to platforms without floating point */
int fbase[] = {
	10000,
	100000,
	1000000,
	10000000,
};

/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
 * to platforms without floating point.
 */
int multipliers[] = {
	0,	/* reserved */
	10,
	12,
	13,
	15,
	20,
	25,
	30,
	35,
	40,
	45,
	50,
	55,
	60,
	70,
	80,
};

void mmc_set_ios(struct mmc *mmc)
{
	mmc->set_ios(mmc);
}

void mmc_set_clock(struct mmc *mmc, uint clock)
{
	if (clock > mmc->f_max)
		clock = mmc->f_max;

	if (clock < mmc->f_min)
		clock = mmc->f_min;

	mmc->clock = clock;

	mmc_set_ios(mmc);
}

void mmc_set_bus_width(struct mmc *mmc, uint width)
{
	mmc->bus_width = width;

	mmc_set_ios(mmc);
}

void mmc_set_timing(struct mmc *mmc, unsigned int timing)
{
	mmc->timing = timing;
	mmc_set_ios(mmc);
}

int mmc_startup(struct mmc *mmc)
{
	int err;
	uint mult, freq;
	u64 cmult, csize, capacity;
	struct mmc_cmd cmd = {0};
	char ext_csd[512] = {0};

	/* Put the Card in Identify Mode */
	cmd.cmdidx = MMC_CMD_ALL_SEND_CID;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	memcpy(mmc->cid, cmd.response, 16);

	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmdarg = mmc->rca << 16;
	cmd.resp_type = MMC_RSP_R6;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if (IS_SD(mmc))
		mmc->rca = (cmd.response[0] >> 16) & 0xffff;

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];

	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
			case 0:
				mmc->version = MMC_VERSION_1_2;
				break;
			case 1:
				mmc->version = MMC_VERSION_1_4;
				break;
			case 2:
				mmc->version = MMC_VERSION_2_2;
				break;
			case 3:
				mmc->version = MMC_VERSION_3;
				break;
			case 4:
				mmc->version = MMC_VERSION_4;
				break;
			default:
				mmc->version = MMC_VERSION_1_2;
				break;
		}
	}

	/* divide frequency by 10, since the mults are 10x bigger */
	freq = fbase[(cmd.response[0] & 0x7)];
	mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc->tran_speed = freq * mult;

	mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc->csd[2] & 0x00038000) >> 15;
	}

	mmc->capacity = (csize + 1) << (cmult + 2);
	mmc->capacity *= mmc->read_bl_len;

	if (mmc->read_bl_len > 512)
		mmc->read_bl_len = 512;

	if (mmc->write_bl_len > 512)
		mmc->write_bl_len = 512;

	/* Select the card, and put it into Transfer Mode */
	cmd.cmdidx = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;
	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	/*
	 * For SD, its erase group is always one sector
	 */
	mmc->erase_grp_size = 1;
	if (!IS_SD(mmc) && (mmc->version >= MMC_VERSION_4)) {
		/* check  ext_csd version and capacity */
		err = mmc_send_ext_csd(mmc, ext_csd);

		if (err)
			return err;

		if (ext_csd[EXT_CSD_REV] >= 2) {
			/*
			 * According to the JEDEC Standard, the value of
			 * ext_csd's capacity is valid if the value is more
			 * than 2GB
			 */
			capacity = ext_csd[EXT_CSD_SEC_CNT] << 0
					| ext_csd[EXT_CSD_SEC_CNT + 1] << 8
					| ext_csd[EXT_CSD_SEC_CNT + 2] << 16
					| ext_csd[EXT_CSD_SEC_CNT + 3] << 24;
			capacity *= 512;
			if ((capacity >> 20) > 2 * 1024)
				mmc->capacity = capacity;
		}

		switch (ext_csd[EXT_CSD_REV]) {
			case 1:
				mmc->version = MMC_VERSION_4_1;
				break;
			case 2:
				mmc->version = MMC_VERSION_4_2;
				break;
			case 3:
				mmc->version = MMC_VERSION_4_3;
				break;
			case 5:
				mmc->version = MMC_VERSION_4_41;
				break;
			case 6:
				mmc->version = MMC_VERSION_4_5;
				break;
			case 7:
				mmc->version = MMC_VERSION_5_0;
				break;
			case 8:
				mmc->version = MMC_VERSION_5_1;
				break;
		}

		/*
		 * Host needs to enable ERASE_GRP_DEF bit if device is
		 * partitioned. This bit will be lost every time after a reset
		 * or power off. This will affect erase size.
		 */
		if ((ext_csd[EXT_CSD_PARTITIONING_SUPPORT] & PART_SUPPORT) &&
		    (ext_csd[EXT_CSD_PARTITIONS_ATTRIBUTE] & PART_ENH_ATTRIB)) {
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_ERASE_GROUP_DEF, 1);

			if (err)
				return err;

			/* Read out group size from ext_csd */
			mmc->erase_grp_size =
				ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] *
					MMC_MAX_BLOCK_LEN * 1024;
		} else {
			/* Calculate the group size from the csd value. */
			int erase_gsz, erase_gmul;
			erase_gsz = (mmc->csd[2] & 0x00007c00) >> 10;
			erase_gmul = (mmc->csd[2] & 0x000003e0) >> 5;
			mmc->erase_grp_size = (erase_gsz + 1)
				* (erase_gmul + 1);
		}

		mmc->sec_feature_support = ext_csd[EXT_CSD_SEC_FEATURE_SUPPORT];
	}


	if (IS_SD(mmc))
		err = sd_change_freq(mmc);
	else
		err = mmc_change_freq(mmc);

	if (err)
		return err;

	/* Restrict card's capabilities by what the host can do */
	mmc->card_caps &= mmc->host_caps;
SwitchDDR50:
	if (IS_SD(mmc)) {
		if (mmc->card_caps & MMC_MODE_4BIT) {
			cmd.cmdidx = MMC_CMD_APP_CMD;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = mmc->rca << 16;
			cmd.flags = 0;

			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmdarg = 2;
			cmd.flags = 0;
			err = mmc_send_cmd(mmc, &cmd, NULL);
			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		}

		if (mmc->card_caps & MMC_MODE_HS)
			mmc->tran_speed = 50000000;
		else
			mmc->tran_speed = 25000000;

	} else if (mmc->card_caps & MMC_MODE_HS200) {
		int idx;
		static unsigned ext_csd_bits[] = {
			EXT_CSD_BUS_WIDTH_4,
			EXT_CSD_BUS_WIDTH_8,
		};
		static unsigned bus_widths[] = {
			4, 8,
		};

		idx = (mmc->host_caps & MMC_MODE_8BIT) ? 1 : 0;
		for (; idx > 0; idx--) {
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_BUS_WIDTH, ext_csd_bits[idx]);
			if (err)
				continue;
			mmc_set_bus_width(mmc, bus_widths[idx]);

			err = mmc_send_ext_csd(mmc, test_ext_csd);
			if (!err && ext_csd[EXT_CSD_REV] \
				== test_ext_csd[EXT_CSD_REV]) {
				break;
			}
		}

		/* switch to HS200 mode if bus width set successfully */
		if (!err){
			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2);
		}

		if(err)
			return err;

		if (mmc->card_caps&MMC_MODE_HS400) {
			mmc->tran_speed = 150000000;
			mmc_set_timing(mmc, MMC_TIMING_MMC_HS400);
		} else {
			mmc->tran_speed = 200000000;
			mmc_set_timing(mmc, MMC_TIMING_MMC_HS200);
		}
		mmc_set_clock(mmc, mmc->tran_speed);

	} else {
		int idx;
		static unsigned ext_csd_bits[] = {
			EXT_CSD_DDR_BUS_WIDTH_8,
			EXT_CSD_DDR_BUS_WIDTH_4,
			EXT_CSD_BUS_WIDTH_8,
			EXT_CSD_BUS_WIDTH_4,
			EXT_CSD_BUS_WIDTH_1,
		};

		static unsigned ext_to_hostcaps[] = {
			[EXT_CSD_DDR_BUS_WIDTH_4] = MMC_MODE_DDR_52MHz,
			[EXT_CSD_DDR_BUS_WIDTH_8] = MMC_MODE_DDR_52MHz,
			[EXT_CSD_BUS_WIDTH_4] = MMC_MODE_4BIT,
			[EXT_CSD_BUS_WIDTH_8] = MMC_MODE_8BIT,
		};

		static unsigned widths[] = {
			8, 4, 8, 4, 1,
		};

		for (idx=0; idx < sizeof(ext_csd_bits)/sizeof(ext_csd_bits[0]); idx++) {
			unsigned int extw = ext_csd_bits[idx];

			/*
			 * Check to make sure the controller supports
			 * this bus width, if it's more than 1
			 */
			if (extw != EXT_CSD_BUS_WIDTH_1 &&
					!(mmc->host_caps & ext_to_hostcaps[extw]))
				continue;

			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH, extw);
			if (err)
				continue;

			mmc_set_bus_width(mmc, widths[idx]);

			if (mmc->card_caps & MMC_MODE_DDR_52MHz)
				mmc_set_timing(mmc, MMC_TIMING_UHS_DDR50);

			if (mmc->card_caps & MMC_MODE_HS) {
				if (mmc->card_caps & MMC_MODE_HS_52MHz)
					mmc->tran_speed = 52000000;
				else
					mmc->tran_speed = 26000000;
			}
			mmc_set_clock(mmc, mmc->tran_speed);

			err = mmc_send_ext_csd(mmc, test_ext_csd);
			if (!err && ext_csd[EXT_CSD_REV] \
				    == test_ext_csd[EXT_CSD_REV]) {
				mmc->card_caps |= ext_to_hostcaps[extw];
				break;
			}
		}
	}

	if ((mmc->card_caps & MMC_MODE_HS200) ||(mmc->card_caps & MMC_MODE_HS400)) {
		if (mmc->card_caps & MMC_MODE_HS400) {
			if (mmc->prepare_hs400_tuning)
				mmc->prepare_hs400_tuning(mmc);
		}

		if (mmc->execute_tuning)
			err = mmc->execute_tuning(mmc, 0);
		if (err) {
			mmc->card_caps  &= ~(MMC_MODE_HS400 | MMC_MODE_HS200);

			mmc_set_timing(mmc,MMC_TIMING_MMC_HS);

			mmc_set_clock(mmc, 52000000);

			err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			   	EXT_CSD_HS_TIMING, 1);
			if (err) {
				printf("switch to high-speed from hs200 failed, err:%d\n", err);
				return err;
			}
			goto SwitchDDR50;
		}
	}

	if (mmc->card_caps & MMC_MODE_HS400) {
		/*
		* HS400 mode requires 8-bit bus width
		*/
		if (mmc->bus_width != 8) {
			printf("HS400 mode requires 8-bit bus width\n");
			return -1;
		}

		/*
		* Before switching to dual data rate operation for HS400,
		* it is required to convert from HS200 mode to HS mode.
		*/
		mmc_set_timing(mmc, MMC_TIMING_MMC_HS);

		mmc_set_clock(mmc, 52000000);

		err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, 1);
		if (err) {
			printf("switch to high-speed from hs200 failed, err:%d\n", err);
			return err;
		}

		err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			 EXT_CSD_BUS_WIDTH, EXT_CSD_DDR_BUS_WIDTH_8);
		if (err) {
			printf("switch to bus width for hs400 failed, err:%d\n", err);
			return err;
		}

		err = mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, 3);
		if (err) {
			printf("switch to hs400 failed, err:%d\n", err);
			return err;
		}

		mmc_set_timing(mmc, MMC_TIMING_MMC_HS400);
		mmc_set_clock(mmc, mmc->tran_speed);

		if (mmc->execute_tuning)
			err = mmc->execute_tuning(mmc, 1);
		if (err) {
			printf("ds tuning failed\n");
			return err;
		}
	}

	/* fill in device description */
	mmc->block_dev.lun = 0;
	mmc->block_dev.type = 0;
	mmc->block_dev.blksz = mmc->read_bl_len;
	mmc->block_dev.lba = lldiv(mmc->capacity, mmc->read_bl_len);
	snprintf(mmc->block_dev.vendor, sizeof(mmc->block_dev.vendor),
		"Man %06x Snr %08x", mmc->cid[0] >> 8,
		(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
	snprintf(mmc->block_dev.product, sizeof(mmc->block_dev.product),
		"%c%c%c%c%c", mmc->cid[0] & 0xff,
		(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
		(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
	snprintf(mmc->block_dev.revision, sizeof(mmc->block_dev.revision),
		"%d.%d", mmc->cid[2] >> 28, (mmc->cid[2] >> 24) & 0xf);

	return 0;
}

int mmc_send_if_cond(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->voltages & 0xff8000) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if ((cmd.response[0] & 0xff) != 0xaa)
		return UNUSABLE_ERR;
	else
		mmc->version = SD_VERSION_2;

	return 0;
}

int mmc_register(struct mmc *mmc)
{
	/* Setup the universal parts of the block interface just once */
	mmc->block_dev.if_type = IF_TYPE_MMC;
	mmc->block_dev.dev = cur_dev_num++;
	mmc->block_dev.removable = 1;
	mmc->block_dev.block_read = mmc_mbread;
	mmc->block_dev.block_write = mmc_bwrite;
	mmc->block_dev.block_erase = mmc_berase;

	INIT_LIST_HEAD (&mmc->link);

	list_add_tail (&mmc->link, &mmc_devices);

	return 0;
}

block_dev_desc_t *mmc_get_dev(int dev)
{
	struct mmc *mmc = find_mmc_device(dev);

	return mmc ? &mmc->block_dev : NULL;
}

int mmc_init(struct mmc *mmc)
{
	int err;

	if (mmc->is_init)
		return 0;

	err = mmc->init(mmc);

	if (err)
		return err;

	mmc_set_bus_width(mmc, 1);
	mmc_set_clock(mmc, 1);

	/* Reset the Card */
	err = mmc_go_idle(mmc);

	if (err)
		return err;

	/* Test for SD version 2 */
	err = mmc_send_if_cond(mmc);

	/* Now try to get the SD card's operating condition */
	err = sd_send_op_cond(mmc);

	/* If the command timed out, we check for an MMC card */
	if (err == TIMEOUT) {
		err = mmc_send_op_cond(mmc);

		if (err) {
			/*
			printf("Card did not respond to voltage select!\n");
			*/
			return UNUSABLE_ERR;
		}
	}

	err = mmc_startup(mmc);
	if (err)
		return err;

	mmc->is_init++;
	return 0;
}

extern int board_mmc_init(bd_t *bis);

void print_mmc_devices(char separator)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		printf("%s: %d", m->name, m->block_dev.dev);

		if (entry->next != &mmc_devices)
			printf("%c ", separator);
	}

	printf("\n");
}

int mmc_initialize(bd_t *bis)
{
	static int init = 0;
	INIT_LIST_HEAD (&mmc_devices);
	cur_dev_num = 0;

	if (init)
		return 0;
	init++;

	return board_mmc_init(bis);
}
