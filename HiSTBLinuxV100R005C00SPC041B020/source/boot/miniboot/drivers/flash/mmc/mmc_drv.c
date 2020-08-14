/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Lai Yingjun
 *
 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <config.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/io.h>
#include <delay.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <debug.h>
#include "mmc_drv.h"

static int freq_base[] = {
	10000,
	100000,
	1000000,
	10000000,
};

static int speed_multipliers[] = {
	0,
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

static char test_ext_csd[512] = {0};

extern u32 emmc_caps2;


int mmc_set_idle(struct mmc_dev_t* mmc_dev)
{
	struct mmc_cmd cmd;
	int ret;

	ASSERT(mmc_dev);

	udelay(1000);

	cmd.cmd_index = MMC_CMD_GO_IDLE_STATE;
	cmd.cmd_args = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	udelay(2000);

	return 0;
}

int mmc_send_status(struct mmc_dev_t *mmc_dev, int timeout)
{
	struct mmc_cmd cmd;
	int err, retries = 5;

	cmd.cmd_index = MMC_CMD_SEND_STATUS;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = mmc_dev->rca << 16;

	do {
		err = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
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

int mmc_set_blklen(struct mmc_dev_t *mmc_dev, int blklen)
{
	struct mmc_cmd cmd;

	if ((mmc_dev->timing & MMC_TIMING_UHS_DDR50)
		||(mmc_dev->timing & MMC_TIMING_MMC_HS400))
		return 0;

	ASSERT(mmc_dev);
	cmd.cmd_index = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = blklen;
	cmd.flags = 0;

	return mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
}


int mmc_send_if_cnd(struct mmc_dev_t *mmc_dev)
{
	struct mmc_cmd cmd;
	int ret;

	ASSERT(mmc_dev);

	cmd.cmd_index = SD_CMD_SEND_IF_COND;
	cmd.cmd_args = ((mmc_dev->voltages & 0xff8000) != 0) << 8 \
					| 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	if ((cmd.response[0] & 0xff) != 0xaa)
		return UNUSABLE_ERR;
	else
		mmc_dev->version = SD_VERSION_2;

	return 0;
}

int 
mmc_write(struct mmc_dev_t *mmc_dev, uint32 start, size_t cnt, const void *buf)
{
	struct mmc_cmd cmd;
	struct mmc_cmd sbc;
	struct mmc_data data;
	int ret;
	int blklen;
	int timeout = 1000;

	if (!mmc_dev)
		return -1;

	blklen = mmc_dev->w_blk_len;

	ret = mmc_set_blklen(mmc_dev, mmc_dev->w_blk_len);

	if (ret) {
		pr_error("Fail to set writing block length, err %d\n", ret);
		return 0;
	}

	if (cnt > 1)
		cmd.cmd_index = MMC_CMD_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmd_index = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc_dev->high_capacity)
		cmd.cmd_args = start;
	else
		cmd.cmd_args = start * blklen;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.src = buf;
	data.blocks = cnt;
	data.blocksize = blklen;
	data.flags = MMC_DATA_WRITE;

	sbc.cmd_index = 0;
	if ((mmc_dev->host_caps & MMC_MODE_CMD23)
		&& (MMC_CMD_WRITE_MULTIPLE_BLOCK == cmd.cmd_index)) {
		sbc.cmd_index = MMC_CMD_SET_BLOCK_COUNT;
		sbc.cmd_args = data.blocks;
		sbc.resp_type = MMC_RSP_R1;
		sbc.flags = 0;
		ret = mmc_dev->send_cmd(mmc_dev, &sbc, NULL);
		if (ret) {
			printf("Fail to set mmc block count\n\r");
			return 0;
		}
	}

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, &data);

	if (ret) {
		pr_error("Fail to write mmc dev, err %d\n", ret);
		return 0;
	}

	if (!sbc.cmd_index && cnt > 1) {
		cmd.cmd_index = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmd_args = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
		if (ret) {
			pr_error("Fail to stop mmc dev! err %d\n", ret);
			return 0;
		}
	}

	/* Waiting for the ready status */
	if (mmc_send_status(mmc_dev, timeout)) 
			return 0;

	return cnt;
}

int mmc_read(struct mmc_dev_t *mmc_dev, uint32 start, size_t cnt, void *buf)
{
	struct mmc_cmd cmd;
	struct mmc_cmd sbc;
	struct mmc_data data;
	int ret;
	int timeout = 1000;

	if (!mmc_dev)
		return 0;

	ret = mmc_set_blklen(mmc_dev, mmc_dev->r_blk_len);

	if (ret) {
		pr_error("Fail to set reading block length, err %d\n", ret);
		return 0;
	}

	if (cnt > 1)
		cmd.cmd_index = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmd_index = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc_dev->high_capacity)
		cmd.cmd_args = start;
	else
		cmd.cmd_args = start * mmc_dev->r_blk_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.dest = buf;
	data.blocks = cnt;
	data.blocksize = mmc_dev->r_blk_len;
	data.flags = MMC_DATA_READ;

	sbc.cmd_index = 0;
	if ((mmc_dev->host_caps & MMC_MODE_CMD23)
		&& (MMC_CMD_READ_MULTIPLE_BLOCK == cmd.cmd_index)) {
		sbc.cmd_index = MMC_CMD_SET_BLOCK_COUNT;
		sbc.cmd_args = data.blocks;
		sbc.resp_type = MMC_RSP_R1;
		sbc.flags = 0;
		ret = mmc_dev->send_cmd(mmc_dev, &sbc, NULL);
		if (ret) {
			printf("Failt to set block count, err %d\n\r", ret);
			return 0;
		}
	}

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, &data);

	if (ret) {
		pr_error("Fail to read mmc dev! err %d\n\r", ret);
		return 0;
	}

	if (!sbc.cmd_index && cnt > 1) {
		cmd.cmd_index = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmd_args = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
		if (ret) {
			pr_error("Fail to stop mmc dev, err %d!\n", ret);
			return 0;
		}

	}

	/* Waiting for the ready status */
	if (mmc_send_status(mmc_dev, timeout))
		return 0;

	return cnt;
}

int mmc_send_tuning(struct mmc_dev_t *mmc_dev, u32 opcode)
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
			if (mmc_dev->bus_width == 8) {
				tuning_block_pattern = tuning_blk_pattern_8bit;
				size = sizeof(tuning_blk_pattern_8bit);
			} else if (mmc_dev->bus_width == 4){
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
	
	cmd.cmd_index = opcode;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = 0;
	cmd.flags = 0;

	data.dest = (char *)data_buf;
	data.blocks = 1;
	data.blocksize = size;
	data.flags = MMC_DATA_READ;

	err = mmc_dev->send_cmd(mmc_dev, &cmd, &data);

	if (opcode != MMC_CMD_SEND_EXT_CSD) {
		cmd.cmd_index = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmd_args = 0;
		cmd.resp_type = MMC_RSP_R1;
		cmd.flags = 0;

		mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

		if(!err) {
			if (memcmp(data_buf, tuning_block_pattern, size))
				err = -EIO;

			mmc_send_status(mmc_dev, 1000);
		}
	}

	if ((opcode == MMC_CMD_SEND_EXT_CSD) &&(err)) {
		cmd.cmd_index = MMC_CMD_STOP_TRANSMISSION;
		cmd.resp_type = MMC_RSP_R1;

		mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
	}

	free(data_buf);
	return err;
}


int sd_send_op_cond(struct mmc_dev_t *mmc_dev)
{
	int timeout = 1000;
	int ret;
	struct mmc_cmd cmd;

	ASSERT(mmc_dev);

	do {
		cmd.cmd_index = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmd_args = 0;
		cmd.flags = 0;

		ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

		if (ret)
			return ret;

		cmd.cmd_index = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		cmd.cmd_args = mmc_dev->voltages & 0xff8000;

		if (mmc_dev->version == SD_VERSION_2)
			cmd.cmd_args |= OCR_HCS;

		ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

		if (ret)
			return ret;

		udelay(1000);
	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc_dev->version != SD_VERSION_2)
		mmc_dev->version = SD_VERSION_1_0;

	mmc_dev->ocr = cmd.response[0];

	mmc_dev->high_capacity = ((mmc_dev->ocr & OCR_HCS) == OCR_HCS);
	mmc_dev->rca = 0;

	return 0;
}

int mmc_dev_send_op_cnd(struct mmc_dev_t *mmc_dev)
{
	int timeout = 1000;
	struct mmc_cmd cmd;
	int ret;

	ASSERT(mmc_dev);

	mmc_set_idle(mmc_dev);

	do {
		cmd.cmd_index = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmd_args = OCR_HCS | mmc_dev->voltages;
		cmd.flags = 0;

		ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

		if (ret)
			return ret;

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	mmc_dev->version = MMC_VERSION_UNKNOWN;
	mmc_dev->ocr = cmd.response[0];

	mmc_dev->high_capacity = ((mmc_dev->ocr & OCR_HCS) == OCR_HCS);
	mmc_dev->rca = 0;

	return 0;
}


int mmc_send_ext_csd(struct mmc_dev_t *mmc_dev, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int ret;

	ASSERT(mmc_dev);

	cmd.cmd_index = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = 0;
	cmd.flags = 0;

	data.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, &data);

	return ret;
}

int sd_switch(struct mmc_dev_t *mmc_dev, int mode, int group, uint8 value, uint8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	ASSERT(mmc_dev);

	cmd.cmd_index = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = (mode << 31) | 0xffffff;
	cmd.cmd_args &= ~(0xf << (group * 4));
	cmd.cmd_args |= value << (group * 4);
	cmd.flags = 0;

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_dev->send_cmd(mmc_dev, &cmd, &data);
}

int mmc_switch(struct mmc_dev_t *mmc_dev, uint8 set, uint8 index, uint8 value)
{
	struct mmc_cmd cmd;
	int timeout = 1000;
	int ret;

	ASSERT(mmc_dev);

	cmd.cmd_index = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmd_args = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
		(index << 16) |
		(value << 8);
	cmd.flags = 0;

	ret =  mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	/* Waiting for the ready status */
	if (!ret)
		ret = mmc_send_status(mmc_dev, timeout);

	return ret;
}

int mmc_chg_freq(struct mmc_dev_t *mmc_dev)
{
	char ext_csd[512];
	char cardtype;
	u32 dev_caps;
	struct emmc_dev * dev;
	int ret;

	ASSERT(mmc_dev);

	mmc_dev->card_caps = 0;

	if (mmc_dev->version < MMC_VERSION_4)
		return 0;

	mmc_dev->card_caps |= MMC_MODE_4BIT | MMC_MODE_8BIT;

	ret = mmc_send_ext_csd(mmc_dev, ext_csd);

	if (ret)
		return ret;
	cardtype = ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_MASK;

	ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

	if (ret)
		return ret;

	ret = mmc_send_ext_csd(mmc_dev, ext_csd);

	if (ret)
		return ret;

	if (!ext_csd[EXT_CSD_HS_TIMING])
		return 0;

	dev = mmc_get_ids_dev(mmc_dev->cid);
	if (dev) {
		dev_caps = dev->caps;
		if ((dev_caps & EXT_CSD_CARD_TYPE_HS400) == 0)
			emmc_caps2 &= ~((MMC_MODE_HS400)>>7);
		if ((dev_caps & EXT_CSD_CARD_TYPE_SDR_200) == 0)
			emmc_caps2 &= ~((MMC_MODE_HS200)>>7);
	} else
		dev_caps = CAPS_NO_MASK;

	if (cardtype & EXT_CSD_CARD_TYPE_SDR_200 & dev_caps) {
		mmc_dev->card_caps |= MMC_MODE_HS200;
	}

	if (cardtype & EXT_CSD_CARD_TYPE_HS400  & dev_caps) {
		mmc_dev->card_caps |= MMC_MODE_HS400;
	}
	if (cardtype & EXT_CSD_CARD_TYPE_52) {
		if (cardtype & EXT_CSD_CARD_TYPE_DDR_52)
			mmc_dev->card_caps |= MMC_MODE_DDR_52MHz;
		mmc_dev->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	} else
		mmc_dev->card_caps |= MMC_MODE_HS;

	return 0;
}

void mmc_set_bus_width(struct mmc_dev_t *mmc_dev, uint32 width)
{
	ASSERT(mmc_dev);

	mmc_dev->bus_width = width;

	mmc_dev->set_ios(mmc_dev);
}


void mmc_set_clock(struct mmc_dev_t *mmc_dev, uint32 clock)
{
	ASSERT(mmc_dev);

	if (clock < mmc_dev->freq_min)
		clock = mmc_dev->freq_min;

	if (clock > mmc_dev->freq_max)
		clock = mmc_dev->freq_max;

	mmc_dev->clock = clock;

	mmc_dev->set_ios(mmc_dev);
}

void mmc_set_timing(struct mmc_dev_t *mmc_dev, unsigned int timing)
{
	mmc_dev->timing = timing;
	mmc_dev->set_ios(mmc_dev);
}

int sd_chg_freq(struct mmc_dev_t *mmc_dev)
{
	int ret;
	struct mmc_cmd cmd;
	uint32 scr[2];
	uint32 switch_status[16];
	struct mmc_data data;
	int timeout;

	ASSERT(mmc_dev);

	mmc_dev->card_caps = 0;

	cmd.cmd_index = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = mmc_dev->rca << 16;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	cmd.cmd_index = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmd_args = 0;
	cmd.flags = 0;

	timeout = 3;

retry_scr:
	data.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, &data);

	if (ret) {
		if (timeout--)
			goto retry_scr;

		return ret;
	}

	mmc_dev->scr[0] = scr[0];
	mmc_dev->scr[1] = scr[1];

	switch ((mmc_dev->scr[0] >> 24) & 0xf) {
		case 0:
			mmc_dev->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc_dev->version = SD_VERSION_1_10;
			break;
		case 2:
			mmc_dev->version = SD_VERSION_2;
			break;
		default:
			mmc_dev->version = SD_VERSION_1_0;
			break;
	}

	if (mmc_dev->version == SD_VERSION_1_0)
		return 0;

	timeout = 4;
	while (timeout--) {
		ret = sd_switch(mmc_dev, SD_SWITCH_CHECK, 0, 1,
				(uint8 *)&switch_status);

		if (ret)
			return ret;

		if (!(switch_status[7] & SD_HIGHSPEED_BUSY))
			break;
	}

	if (mmc_dev->scr[0] & SD_DATA_4BIT)
		mmc_dev->card_caps |= MMC_MODE_4BIT;

	if (!(switch_status[3] & SD_HIGHSPEED_SUPPORTED))
		return 0;

	ret = sd_switch(mmc_dev, SD_SWITCH_SWITCH, 0, 1, (uint8 *)&switch_status);

	if (ret)
		return ret;

	if ((switch_status[4] & 0x0f000000) == 0x01000000)
		mmc_dev->card_caps |= MMC_MODE_HS;

	return 0;
}

int mmc_dev_start(struct mmc_dev_t *mmc_dev)
{
	int ret;
	uint32 mult, freq;
	uint64 cmult, csize, capacity;
	struct mmc_cmd cmd;
	char ext_csd[512];

	ASSERT(mmc_dev);

	cmd.cmd_index = MMC_CMD_ALL_SEND_CID;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmd_args = 0;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	memcpy(mmc_dev->cid, cmd.response, 16);

	cmd.cmd_index = SD_CMD_SEND_RELATIVE_ADDR;
	cmd.cmd_args = mmc_dev->rca << 16;
	cmd.resp_type = MMC_RSP_R6;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	if (IS_SD(mmc_dev))
		mmc_dev->rca = (cmd.response[0] >> 16) & 0xffff;

	cmd.cmd_index = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmd_args = mmc_dev->rca << 16;
	cmd.flags = 0;

	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	mmc_dev->csd[0] = cmd.response[0];
	mmc_dev->csd[1] = cmd.response[1];
	mmc_dev->csd[2] = cmd.response[2];
	mmc_dev->csd[3] = cmd.response[3];

	if (mmc_dev->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
			case 0:
				mmc_dev->version = MMC_VERSION_1_2;
				break;
			case 1:
				mmc_dev->version = MMC_VERSION_1_4;
				break;
			case 2:
				mmc_dev->version = MMC_VERSION_2_2;
				break;
			case 3:
				mmc_dev->version = MMC_VERSION_3;
				break;
			case 4:
				mmc_dev->version = MMC_VERSION_4;
				break;
			default:
				mmc_dev->version = MMC_VERSION_1_2;
				break;
		}
	}

	freq = freq_base[(cmd.response[0] & 0x7)];
	mult = speed_multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc_dev->speed = freq * mult;

	mmc_dev->r_blk_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc_dev))
		mmc_dev->w_blk_len = mmc_dev->r_blk_len;
	else
		mmc_dev->w_blk_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc_dev->high_capacity) {
		csize = (mmc_dev->csd[1] & 0x3f) << 16
			| (mmc_dev->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = (mmc_dev->csd[1] & 0x3ff) << 2
			| (mmc_dev->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc_dev->csd[2] & 0x00038000) >> 15;
	}

	mmc_dev->capacity = (csize + 1) << (cmult + 2);
	mmc_dev->capacity *= mmc_dev->r_blk_len;

	if (mmc_dev->r_blk_len > 512)
		mmc_dev->r_blk_len = 512;

	if (mmc_dev->w_blk_len > 512)
		mmc_dev->w_blk_len = 512;

	cmd.cmd_index = MMC_CMD_SELECT_CARD;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmd_args = mmc_dev->rca << 16;
	cmd.flags = 0;
	ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);

	if (ret)
		return ret;

	if (!IS_SD(mmc_dev) && (mmc_dev->version >= MMC_VERSION_4)) {
		ret = mmc_send_ext_csd(mmc_dev, ext_csd);

		if (ret)
			return ret;

		if (ext_csd[EXT_CSD_REV] >= 2) {
			capacity = ext_csd[EXT_CSD_SEC_CNT] << 0
					| ext_csd[EXT_CSD_SEC_CNT + 1] << 8
					| ext_csd[EXT_CSD_SEC_CNT + 2] << 16
					| ext_csd[EXT_CSD_SEC_CNT + 3] << 24;
			capacity *= 512;
			if ((capacity >> 20) > 2 * 1024)
				mmc_dev->capacity = capacity;
		}
		
		switch (ext_csd[EXT_CSD_REV]) {
			case 1:
				mmc_dev->version = MMC_VERSION_4_1;
				break;
			case 2:
				mmc_dev->version = MMC_VERSION_4_2;
				break;
			case 3:
				mmc_dev->version = MMC_VERSION_4_3;
				break;
			case 5:
				mmc_dev->version = MMC_VERSION_4_41;
				break;
			case 6:
				mmc_dev->version = MMC_VERSION_4_5;
				break;
			case 7:
				mmc_dev->version = MMC_VERSION_5_0;
				break;
			case 8:
				mmc_dev->version = MMC_VERSION_5_1;
				break;
		}

	}


	if (IS_SD(mmc_dev))
		ret = sd_chg_freq(mmc_dev);
	else
		ret = mmc_chg_freq(mmc_dev);

	if (ret)
		return ret;

	mmc_dev->card_caps &= mmc_dev->host_caps;
SwitchDDR50:
	if (IS_SD(mmc_dev)) {
		if (mmc_dev->card_caps & MMC_MODE_4BIT) {
			cmd.cmd_index = MMC_CMD_APP_CMD;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmd_args = mmc_dev->rca << 16;
			cmd.flags = 0;

			ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
			if (ret)
				return ret;

			cmd.cmd_index = SD_CMD_APP_SET_BUS_WIDTH;
			cmd.resp_type = MMC_RSP_R1;
			cmd.cmd_args = 2;
			cmd.flags = 0;
			ret = mmc_dev->send_cmd(mmc_dev, &cmd, NULL);
			if (ret)
				return ret;

			mmc_set_bus_width(mmc_dev, 4);
		}

		if (mmc_dev->card_caps & MMC_MODE_HS)
			mmc_dev->speed = 50000000;
		else
			mmc_dev->speed = 25000000;
	} else if (mmc_dev->card_caps & MMC_MODE_HS200) {
		int idx;
		static unsigned ext_csd_bits[] = {
			EXT_CSD_BUS_WIDTH_4,
			EXT_CSD_BUS_WIDTH_8,
		};
		static unsigned bus_widths[] = {
			4, 8,
		};

		idx = (mmc_dev->host_caps & MMC_MODE_8BIT) ? 1 : 0;
		for (; idx > 0; idx--) {
			ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
				EXT_CSD_BUS_WIDTH, ext_csd_bits[idx]);
			if (ret)
				continue;
			mmc_set_bus_width(mmc_dev, bus_widths[idx]);

			ret = mmc_send_ext_csd(mmc_dev, test_ext_csd);
			if (!ret && ext_csd[EXT_CSD_REV] \
				== test_ext_csd[EXT_CSD_REV]) {
				break;
			}
		}

		/* switch to HS200 mode if bus width set successfully */
		if (!ret){
			ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2);
		}
		
		if(ret)
			return ret;

		if (mmc_dev->card_caps&MMC_MODE_HS400) {
			mmc_dev->speed = 150000000;
			mmc_set_timing(mmc_dev, MMC_TIMING_MMC_HS400);
		} else {
			mmc_dev->speed = 200000000;
			mmc_set_timing(mmc_dev, MMC_TIMING_MMC_HS200);
		}
		mmc_set_clock(mmc_dev, mmc_dev->speed);

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
					!(mmc_dev->host_caps & ext_to_hostcaps[extw]))
				continue;

			ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH, extw);
			if (ret)
				continue;

			mmc_set_bus_width(mmc_dev, widths[idx]);

			if (mmc_dev->card_caps & MMC_MODE_DDR_52MHz)
				mmc_set_timing(mmc_dev, MMC_TIMING_UHS_DDR50);

			if (mmc_dev->card_caps & MMC_MODE_HS) {
				if (mmc_dev->card_caps & MMC_MODE_HS_52MHz)
					mmc_dev->speed = 52000000;
				else
					mmc_dev->speed = 26000000;
			}
			mmc_set_clock(mmc_dev, mmc_dev->speed);
			ret = mmc_send_ext_csd(mmc_dev, test_ext_csd);
			if (!ret && ext_csd[EXT_CSD_REV] \
				    == test_ext_csd[EXT_CSD_REV]) {
				mmc_dev->card_caps |= ext_to_hostcaps[extw];
				break;
			}
		}
	}

	if ((mmc_dev->card_caps & MMC_MODE_HS200) ||(mmc_dev->card_caps & MMC_MODE_HS400)) {
		if (mmc_dev->card_caps & MMC_MODE_HS400) {
			if (mmc_dev->prepare_hs400_tuning)
				mmc_dev->prepare_hs400_tuning(mmc_dev);
		}

		if (mmc_dev->execute_tuning)
			ret = mmc_dev->execute_tuning(mmc_dev, 0);
		if (ret) {
			mmc_dev->card_caps  &= ~(MMC_MODE_HS400 | MMC_MODE_HS200);
			
			mmc_set_timing(mmc_dev,MMC_TIMING_MMC_HS);

			mmc_set_clock(mmc_dev, 52000000);

			ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
			   	EXT_CSD_HS_TIMING, 1);
			if (ret) {
				printf("switch to high-speed from hs200 failed, err:%d\n", ret);
				return ret;
			}
			goto SwitchDDR50;
		}
	}

	if (mmc_dev->card_caps & MMC_MODE_HS400) {
		/*
		* HS400 mode requires 8-bit bus width
		*/
		if (mmc_dev->bus_width != 8) {
			printf("HS400 mode requires 8-bit bus width\n");
			return -1;
		}

		/*
		* Before switching to dual data rate operation for HS400,
		* it is required to convert from HS200 mode to HS mode.
		*/
		mmc_set_timing(mmc_dev, MMC_TIMING_MMC_HS);

		mmc_set_clock(mmc_dev, 52000000);

		ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, 1);
		if (ret) {
			printf("switch to high-speed from hs200 failed, err:%d\n", ret);
			return ret;
		}

		ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
			 EXT_CSD_BUS_WIDTH, EXT_CSD_DDR_BUS_WIDTH_8);
		if (ret) {
			printf("switch to bus width for hs400 failed, err:%d\n", ret);
			return ret;
		}

		ret = mmc_switch(mmc_dev, EXT_CSD_CMD_SET_NORMAL,
			   EXT_CSD_HS_TIMING, 3);
		if (ret) {
			printf("switch to hs400 failed, err:%d\n", ret);
			return ret;
		}

		mmc_set_timing(mmc_dev, MMC_TIMING_MMC_HS400);
		mmc_set_clock(mmc_dev, mmc_dev->speed);

		if (mmc_dev->execute_tuning)
			ret = mmc_dev->execute_tuning(mmc_dev, 1);
		if (ret) {
			printf("ds tuning failed\n");
			return ret;
		}
	}

	return 0;
}

int mmc_dev_init(struct mmc_dev_t *mmc_dev)
{
	int ret;

	ASSERT(mmc_dev);

	if (mmc_dev->is_init)
		return 0;

	ret = mmc_dev->init(mmc_dev);

	if (ret)
		return ret;

	mmc_set_bus_width(mmc_dev, 1);
	mmc_set_clock(mmc_dev, 1);

	ret = mmc_set_idle(mmc_dev);

	if (ret)
		return ret;

	mmc_send_if_cnd(mmc_dev);

	ret = sd_send_op_cond(mmc_dev);

	if (ret == TIMEOUT) {
		ret = mmc_dev_send_op_cnd(mmc_dev);

		if (ret) {
			return UNUSABLE_ERR;
		}
	}

	ret = mmc_dev_start(mmc_dev);
	if (ret)
		return ret;

	mmc_dev->is_init++;
	return 0;
}


