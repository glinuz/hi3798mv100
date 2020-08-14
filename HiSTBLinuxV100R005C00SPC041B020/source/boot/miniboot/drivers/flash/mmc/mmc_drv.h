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

#ifndef _MMC_DRV_H
#define _MMC_DRV_H

#include <stddef.h>
#include <sys/types.h>

#define SD_VERSION_SD			0x20000
#define SD_VERSION_3			(SD_VERSION_SD | 0x300)
#define SD_VERSION_2			(SD_VERSION_SD | 0x200)
#define SD_VERSION_1_0			(SD_VERSION_SD | 0x100)
#define SD_VERSION_1_10			(SD_VERSION_SD | 0x10a)
#define MMC_VERSION_MMC			0x10000
#define MMC_VERSION_UNKNOWN		(MMC_VERSION_MMC)
#define MMC_VERSION_1_2			(MMC_VERSION_MMC | 0x102)
#define MMC_VERSION_1_4			(MMC_VERSION_MMC | 0x104)
#define MMC_VERSION_2_2			(MMC_VERSION_MMC | 0x202)
#define MMC_VERSION_3			(MMC_VERSION_MMC | 0x300)
#define MMC_VERSION_4			(MMC_VERSION_MMC | 0x400)
#define MMC_VERSION_4_1			(MMC_VERSION_MMC | 0x401)
#define MMC_VERSION_4_2			(MMC_VERSION_MMC | 0x402)
#define MMC_VERSION_4_3			(MMC_VERSION_MMC | 0x403)
#define MMC_VERSION_4_41		(MMC_VERSION_MMC | 0x429)
#define MMC_VERSION_4_5			(MMC_VERSION_MMC | 0x405)
#define MMC_VERSION_5_0			(MMC_VERSION_MMC | 0x500)
#define MMC_VERSION_5_1			(MMC_VERSION_MMC | 0x501)

#define MMC_MODE_HS				(1 << 0)
#define MMC_MODE_HS_52MHz		(1 << 1)
#define MMC_MODE_4BIT			(1 << 2)
#define MMC_MODE_8BIT			(1 << 3)
#define MMC_MODE_SPI			(1 << 4)
#define MMC_MODE_HC			(1 << 5)
#define MMC_MODE_DDR_52MHz		(1 << 6)
#define MMC_MODE_HS200			(1 << 7)
#define MMC_MODE_HS400			(1 << 8)
#define MMC_MODE_CMD23			(1 << 9)

#define SD_DATA_4BIT			0x00040000

#define IS_SD(x)			(x->version & SD_VERSION_SD)

#define MMC_DATA_READ			1
#define MMC_DATA_WRITE			2

#define NO_CARD_ERR			-16
#define UNUSABLE_ERR			-17
#define COMM_ERR			-18
#define TIMEOUT				-19
#define SWITCH_ERR			-21 /* Card reports failure to switch mode */

#define MMC_CMD_GO_IDLE_STATE				0
#define MMC_CMD_SEND_OP_COND				1
#define MMC_CMD_ALL_SEND_CID				2
#define MMC_CMD_SET_RELATIVE_ADDR			3
#define MMC_CMD_SET_DSR					4
#define MMC_CMD_SWITCH					6
#define MMC_CMD_SELECT_CARD				7
#define MMC_CMD_SEND_EXT_CSD				8
#define MMC_CMD_SEND_CSD				9
#define MMC_CMD_SEND_CID				10
#define MMC_CMD_STOP_TRANSMISSION			12
#define MMC_CMD_SEND_STATUS				13
#define MMC_CMD_SET_BLOCKLEN				16
#define MMC_CMD_READ_SINGLE_BLOCK			17
#define MMC_CMD_READ_MULTIPLE_BLOCK			18
#define MMC_CMD_SEND_TUNING_BLOCK_HS200			21
#define MMC_CMD_SET_BLOCK_COUNT				23
#define MMC_CMD_WRITE_SINGLE_BLOCK			24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK			25
#define MMC_CMD_APP_CMD					55

#define SD_CMD_SEND_RELATIVE_ADDR	3
#define SD_CMD_SWITCH_FUNC		6
#define SD_CMD_SEND_IF_COND		8

#define SD_CMD_APP_SET_BUS_WIDTH	6
#define SD_CMD_APP_SEND_OP_COND		41
#define SD_CMD_APP_SEND_SCR		51

#define SD_HIGHSPEED_BUSY		0x00020000
#define SD_HIGHSPEED_SUPPORTED		0x00020000

#define MMC_HS_TIMING			0x00000100
#define MMC_HS_52MHZ			0x2

#define MMC_TIMING_LEGACY		(0x1<<0)
#define MMC_TIMING_MMC_HS		(0x1<<1)
#define MMC_TIMING_UHS_DDR50		(0x1<<2)
#define MMC_TIMING_MMC_HS200		(0x1<<3)
#define MMC_TIMING_MMC_HS400		(0x1<<4)
#define OCR_BUSY			0x80000000
#define OCR_HCS				0x40000000
#define MMC_STATUS_MASK			(~0x0206BF7F)
#define MMC_STATUS_SWITCH_ERROR		(1 << 7)
#define MMC_STATUS_RDY_FOR_DATA		(1 << 8)
#define MMC_STATUS_CURR_STATE		(0xf << 9)
#define MMC_STATUS_ERROR		(1 << 19)

#define MMC_STATE_PRG			(7 << 9)

#define MMC_VDD_165_195			0x00000080
#define MMC_VDD_20_21			0x00000100
#define MMC_VDD_21_22			0x00000200
#define MMC_VDD_22_23			0x00000400
#define MMC_VDD_23_24			0x00000800
#define MMC_VDD_24_25			0x00001000
#define MMC_VDD_25_26			0x00002000
#define MMC_VDD_26_27			0x00004000
#define MMC_VDD_27_28			0x00008000
#define MMC_VDD_28_29			0x00010000
#define MMC_VDD_29_30			0x00020000
#define MMC_VDD_30_31			0x00040000
#define MMC_VDD_31_32			0x00080000
#define MMC_VDD_32_33			0x00100000
#define MMC_VDD_33_34			0x00200000
#define MMC_VDD_34_35			0x00400000
#define MMC_VDD_35_36			0x00800000

#define MMC_SWITCH_MODE_CMD_SET		0x00
#define MMC_SWITCH_MODE_SET_BITS	0x01
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03

#define SD_SWITCH_CHECK			0
#define SD_SWITCH_SWITCH		1

/*
 * EXT_CSD
 */

#define EXT_CSD_WR_REL_PARAM		166
#define EXT_CSD_WR_REL_SET		167
#define EXT_CSD_RST_N_FUNCTION		162
#define EXT_CSD_BOOT_BUS_WIDTH		177
#define EXT_CSD_BOOT_CONFIG		179
#define EXT_CSD_BUS_WIDTH		183
#define EXT_CSD_HS_TIMING		185
#define EXT_CSD_CARD_TYPE		196
#define EXT_CSD_REV			192
#define EXT_CSD_SEC_CNT			212
#define EXT_CSD_BOOT_SIZE_MULTI		226

/*
 * EXT_CSD definitions
 */
#define EXT_CSD_WR_REL_PARAM_EN		(1<<2)
#define EXT_CSD_WR_REL_PARAM_HCR	(1<<0)
#define EXT_CSD_WR_REL_VALUE		(0x1f)
#define EXT_CSD_RST_N_EN_MASK		0x3
#define EXT_CSD_RST_N_ENABLED		(1<<0)
#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

#define EXT_CSD_CARD_TYPE_MASK	0xFF	/* Mask out reserved bits */
#define EXT_CSD_CARD_TYPE_26	(1<<0)	/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52	(1<<1)	/* Card can run at 52MHz */
#define EXT_CSD_CARD_TYPE_DDR_1_8V	(1 << 2)
#define EXT_CSD_CARD_TYPE_DDR_1_2V	(1 << 3)
#define EXT_CSD_CARD_TYPE_DDR_52	(EXT_CSD_CARD_TYPE_DDR_1_8V \
					| EXT_CSD_CARD_TYPE_DDR_1_2V)

#define EXT_CSD_CARD_TYPE_SDR_1_8V	(1<<4)	/* Card can run at 200MHz */
#define EXT_CSD_CARD_TYPE_SDR_1_2V	(1<<5)	/* Card can run at 200MHz */
						/* SDR mode @1.2V I/O */

#define EXT_CSD_CARD_TYPE_SDR_200	(EXT_CSD_CARD_TYPE_SDR_1_8V | \
					 EXT_CSD_CARD_TYPE_SDR_1_2V)

#define EXT_CSD_CARD_TYPE_HS400_1_8V	(1<<6)	/* Card can run at 200MHz */
							/* DDR mode @1.8V I/O */
#define EXT_CSD_CARD_TYPE_HS400_1_2V	(1<<7)	/* Card can run at 200MHz */
							/* DDR mode @1.2V I/O */
#define EXT_CSD_CARD_TYPE_HS400		(EXT_CSD_CARD_TYPE_HS400_1_8V  \
					| EXT_CSD_CARD_TYPE_HS400_1_2V)

#define EXT_CSD_BUS_WIDTH_1	0	/* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	1	/* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	2	/* Card is in 8 bit mode */
#define EXT_CSD_DDR_BUS_WIDTH_4	5
#define EXT_CSD_DDR_BUS_WIDTH_8	6

#define R1_ILLEGAL_COMMAND		(1 << 22)
#define R1_APP_CMD			(1 << 5)

#define MMC_RSP_PRESENT			(1 << 0)
#define MMC_RSP_136			(1 << 1)
#define MMC_RSP_CRC			(1 << 2) 
#define MMC_RSP_BUSY			(1 << 3)
#define MMC_RSP_OPCODE			(1 << 4)

#define MMC_RSP_NONE			(0)
#define MMC_RSP_R1			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
						MMC_RSP_BUSY)
#define MMC_RSP_R2			(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3			(MMC_RSP_PRESENT)
#define MMC_RSP_R4			(MMC_RSP_PRESENT)
#define MMC_RSP_R5			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7			(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

/*
 * Card status definitions
 */
#define MMC_CS_ERROR_MASK		0xFDFFA080

/*  tunning fuction */
#define MMC_TUNING_BLK_PATTERN_4BIT_SIZE	 64
#define MMC_TUNING_BLK_PATTERN_8BIT_SIZE	128

#define CAPS_NO_MASK 0xffffffff

struct mmc_csd
{
	uint8	csd_structure:2,
		spec_vers:4,
		rsvd1:2;
	uint8	taac;
	uint8	nsac;
	uint8	speed;
	uint16	ccc:12,
		r_blk_len:4;
	uint64	read_bl_partial:1,
		write_blk_misalign:1,
		read_blk_misalign:1,
		dsr_imp:1,
		rsvd2:2,
		c_size:12,
		vdd_r_curr_min:3,
		vdd_r_curr_max:3,
		vdd_w_curr_min:3,
		vdd_w_curr_max:3,
		c_size_mult:3,
		sector_size:5,
		erase_grp_size:5,
		wp_grp_size:5,
		wp_grp_enable:1,
		default_ecc:2,
		r2w_factor:3,
		w_blk_len:4,
		write_bl_partial:1,
		rsvd3:5;
	uint8	file_format_grp:1,
		copy:1,
		perm_write_protect:1,
		tmp_write_protect:1,
		file_format:2,
		ecc:2;
	uint8	crc:7;
	uint8	one:1;
};

struct mmc_data {
	union {
		char *dest;
		const char *src;
	};
	uint32 flags;
	uint32 blocks;
	uint32 blocksize;
};

struct mmc_cmd {
	uint16 cmd_index;
	uint32 resp_type;
	uint32 cmd_args;
	uint32 response[4];
	uint32 flags;
};

/* mmc driver interface. */
struct mmc_dev_t {
	uint32 devnum;
	char name[32];
	int is_init;
	int high_capacity;
	void *priv;
	uint32 bus_width;
	uint32 voltages;
	uint32 clock;
	uint32 version;
	uint32 freq_max;
	uint32 freq_min;
	uint32 card_caps;
	uint32 host_caps;
	uint32 ocr;
	uint32 scr[2];
	uint32 csd[4];
	uint32 cid[4];
	uint16 rca;
	uint32 speed;
	uint32 blocksize;
	uint32 r_blk_len;
	uint32 w_blk_len;
	uint32 timing;
	uint32 iovoltage;
	uint64 capacity;
	int (*send_cmd)(struct mmc_dev_t *, struct mmc_cmd *, struct mmc_data *);
	void (*set_ios)(struct mmc_dev_t *);
	int (*init)(struct mmc_dev_t *);
	int (*read)(struct mmc_dev_t *, uint32, size_t, void *);
	int (*write)(struct mmc_dev_t *, uint32, size_t, const void *);
	void	(*prepare_hs400_tuning)(struct mmc_dev_t *);
	int	(*execute_tuning)(struct mmc_dev_t *, unsigned int );
};

struct emmc_dev {
	char mid;
	char *pnm;
	u32  caps;
};

typedef struct mmc_dev_t *(*mmc_driver_init_fn)(void);

extern mmc_driver_init_fn mmcdev_driver_init;
int mmc_dev_init(struct mmc_dev_t *mmc_dev);
int mmc_send_tuning(struct mmc_dev_t *mmc_dev, u32 opcode);
struct emmc_dev *mmc_get_ids_dev(unsigned int * cid);
#endif /* _MMC_DRV_H */
