/******************************************************************************
 *  Copyright (C) 2015 Hisilicon Technologies CO.,LTD.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Create By Cai Zhiyong 2015.7.2
 *
******************************************************************************/

#define pr_fmt(fmt) "spinor: " fmt

#include <linux/kernel.h>
#include "hiflash.h"

struct spinor_drvier_table {
	u8 id[FLASH_ID_LEN];
	int id_len;
	struct spinor_driver *drv;
};

extern struct spinor_flash_dev spinor_flash_ids_hisi[];
/******************************************************************************/

static int spinor_write_buf_reg(struct flash_regop *regop, u8 cmd, u8 *buf,
				int sz_buf)
{
	struct flash_regop_info info = {0};

	info.priv = regop->priv;
	info.cmd = cmd;
	info.nr_cmd = 1;
	info.buf = buf;
	info.sz_buf = sz_buf;
	return regop->write_reg(&info);
}
/******************************************************************************/

static int spinor_read_buf_reg(struct flash_regop *regop, u8 cmd, u8 *buf,
			       int sz_buf)
{
	struct flash_regop_info info = {0};

	info.priv = regop->priv;
	info.cmd = cmd;
	info.nr_cmd = 1;
	info.buf = buf;
	info.sz_buf = sz_buf;
	return regop->read_reg(&info);
}
/******************************************************************************/

static int spinor_quad_enable_null(struct flash_regop *regop)
{
	/* do nothing */
	return 0;
}
/******************************************************************************/

static int spinor_quad_disable_null(struct flash_regop *regop)
{
	/* do nothing */
	return 0;
}
/******************************************************************************/

static int spinor_write_enable(struct flash_regop *regop)
{
	return spinor_write_buf_reg(regop, SPINOR_OP_WREN, NULL, 0);
}
/******************************************************************************/

static int spinor_entry_4addr(struct flash_regop *regop)
{
	return spinor_write_buf_reg(regop, SPINOR_OP_EN4B, NULL, 0);
}
/******************************************************************************/

static int spinor_exit_4addr(struct flash_regop *regop)
{
	return spinor_write_buf_reg(regop, SPINOR_OP_EX4B, NULL, 0);
}
/******************************************************************************/

static int spinor_quad_enable_mxic(struct flash_regop *regop)
{
	int status;

	status = regop->read_sr(regop);
	if (status & SPINOR_STATUS_EQ)
		return 0;

	spinor_write_enable(regop);

	status |= SPINOR_STATUS_EQ;

	spinor_write_buf_reg(regop, SPINOR_OP_WRSR, (u8 *)&status, 1);

	regop->wait_device_ready(regop);

	/* chip not support quad mode */
	status = regop->read_sr(regop);
	if (!(status & SPINOR_STATUS_EQ))
		return -EINVAL;

	return 0;
}
/******************************************************************************/

static int spinor_quad_disable_mxic(struct flash_regop *regop)
{
	int status;

	status = regop->read_sr(regop);
	if (!(status & SPINOR_STATUS_EQ))
		return 0;

	spinor_write_enable(regop);

	status &= ~SPINOR_STATUS_EQ;

	spinor_write_buf_reg(regop, SPINOR_OP_WRSR, (u8 *)&status, 1);

	regop->wait_device_ready(regop);

	/* chip disable quad mode failed. */
	status = regop->read_sr(regop);
	if ((status & SPINOR_STATUS_EQ))
		return -EINVAL;

	return 0;
}
/******************************************************************************/

static struct spinor_driver spinor_drv_mxic = {
	.quad_enable = spinor_quad_enable_mxic,
	.quad_disable = spinor_quad_disable_mxic,
};
/******************************************************************************/

static int spinor_quad_enable_spansion(struct flash_regop *regop)
{
	u8 sr[2];

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR2, &sr[1], 1);
	if (sr[1] & SPINOR_STATUS2_EQ)
		return 0;

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR,  &sr[0], 1);

	sr[1] |= SPINOR_STATUS2_EQ;

	spinor_write_enable(regop);

	spinor_write_buf_reg(regop, SPINOR_OP_WRSR, sr, 2);

	regop->wait_device_ready(regop);

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR2, &sr[1], 1);

	/* chip not support quad mode */
	if (!(sr[1] & SPINOR_STATUS2_EQ))
		return -EINVAL;

	return 0;
}
/******************************************************************************/

static int spinor_quad_disable_spansion(struct flash_regop *regop)
{
	u8 sr[2];

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR2, &sr[1], 1);
	if (!(sr[1] & SPINOR_STATUS2_EQ))
		return 0;

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR,  &sr[0], 1);

	sr[1] &= ~SPINOR_STATUS2_EQ;

	spinor_write_enable(regop);

	spinor_write_buf_reg(regop, SPINOR_OP_WRSR, sr, 2);

	regop->wait_device_ready(regop);

	spinor_read_buf_reg(regop, SPINOR_OP_RDSR2, &sr[1], 1);

	/* chip disable quad mode failed. */
	if ((sr[1] & SPINOR_STATUS2_EQ))
		return -EINVAL;

	return 0;
}
/******************************************************************************/

static struct spinor_driver spinor_drv_spansion = {
	.quad_enable = spinor_quad_enable_spansion,
	.quad_disable = spinor_quad_disable_spansion,
};
/******************************************************************************/

static struct spinor_drvier_table spinor_drv_table[] = {
	/* MXIC */
	{{ 0xc2, 0x20, 0x18 }, 3, &spinor_drv_mxic },
	{{ 0xc2, 0x26 }, 2, &spinor_drv_mxic },
	/* Spansion */
	{{ 0x01, 0x02 }, 2, &spinor_drv_spansion },
	{{ 0x01, 0x20 }, 2, &spinor_drv_spansion },
	/* GigaDevice */
	{{ 0xC8, 0x40 }, 2, &spinor_drv_spansion },
	/* Winbond */
	{{ 0xef, 0x40 }, 2, &spinor_drv_spansion },

	{{0},0,0},
};
/******************************************************************************/

struct spinor_flash_dev *spinor_match_id(u8 *id, int id_len)
{
	struct spinor_flash_dev *spinor;
	struct spinor_flash_dev *best = NULL;

	for (spinor = spinor_flash_ids_hisi; spinor->name; spinor++) {
		if (spinor->id_len > id_len ||
		    memcmp(id, spinor->id, spinor->id_len))
			continue;

		if (!best || best->id_len < spinor->id_len)
			best = spinor;
	}

	/* best == NULL means find an unknown chip id */
	return best;
}
/******************************************************************************/

static struct spinor_driver spinor_drv_general = {
	.quad_enable  = spinor_quad_enable_null,
	.quad_disable = spinor_quad_disable_null,
	.write_enable = spinor_write_enable,
	.entry_4addr  = spinor_entry_4addr,
	.exit_4addr   = spinor_exit_4addr,
};
/******************************************************************************/

struct spinor_driver *spinor_get_driver(u8 *id, int id_len)
{
	struct spinor_drvier_table *drv;

	for (drv = spinor_drv_table; drv->id_len; drv++) {
		if (drv->id_len <= id_len &&
		    !memcmp(drv->id, id, drv->id_len))
			goto found;
	}

	return &spinor_drv_general;

found:
	if (!drv->drv->quad_enable)
		drv->drv->quad_enable = spinor_quad_enable_null;

	if (!drv->drv->write_enable)
		drv->drv->write_enable = spinor_write_enable;

	if (!drv->drv->entry_4addr)
		drv->drv->entry_4addr = spinor_entry_4addr;

	if (!drv->drv->exit_4addr)
		drv->drv->exit_4addr = spinor_exit_4addr;

	return drv->drv;
}
