/******************************************************************************
 *  Copyright (C) 2015 Cai Zhiyong
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
 * Create By Cai Zhiying 2015.7.28
 *
******************************************************************************/

#include <linux/module.h>
#include <linux/mtd/nand.h>
#include <linux/sizes.h>

#include "hiflash.h"

/******************************************************************************/

static int spinand_get_feature(struct flash_regop *regop, int addr)
{
	u8 status = 0;
	struct flash_regop_info info = {0};

	info.priv = regop->priv;
	info.cmd = SPINAND_OP_RDSR;
	info.nr_cmd = 1;
	info.addrl = addr;
	info.addr_cycle = 1;
	info.buf = &status;
	info.sz_buf = 1;
	regop->read_reg(&info);

	return status;
}
/******************************************************************************/

static int spinand_set_feature(struct flash_regop *regop, int addr, u8 value)
{
	struct flash_regop_info info = {0};

	info.priv = regop->priv;
	info.cmd = SPINAND_OP_WREN;
	info.nr_cmd = 1;
	regop->write_reg(&info);

	info.priv = regop->priv;
	info.cmd = SPINAND_OP_WRSR;
	info.nr_cmd = 1;
	info.addrl = addr;
	info.addr_cycle = 1;
	info.buf = &value;
	info.sz_buf = 1;
	regop->write_reg(&info);

	return value;
}
/******************************************************************************/

static void spinand_chip_prepare(struct flash_regop *regop)
{
	//nothing to do.
}
/******************************************************************************/

static void winbond_chip_prepare_w25n01gv(struct flash_regop *regop)
{
	u8 stat, new_stat;

	/* disable block protect */
	new_stat = stat = (u8)spinand_get_feature(regop, 0xa0);
	if (new_stat & SPINAND_BLOCK_PROT_WINBOND)
		new_stat &= ~SPINAND_BLOCK_PROT_WINBOND;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xa0, new_stat);

	new_stat = stat = (u8)spinand_get_feature(regop, 0xb0);

	/* disable w25n01gv continuous read feature if it is enable. */
	if (!(new_stat & SPINAND_STATUS_BUF))
		new_stat |= SPINAND_STATUS_BUF;

	/* disable chip internal ecc */
	if (new_stat & SPINAND_STATUS_ECC_EN)
		new_stat &= ~SPINAND_STATUS_ECC_EN;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xb0, new_stat);
}
/******************************************************************************/

static void giga_chip_prepare_default(struct flash_regop *regop)
{
	u8 stat, new_stat;

	/* disable block protect */
	new_stat = stat = (u8)spinand_get_feature(regop, 0xa0);
	if (new_stat & SPINAND_BLOCK_PROT_GIGA)
		new_stat &= ~SPINAND_BLOCK_PROT_GIGA;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xa0, new_stat);

	new_stat = stat = (u8)spinand_get_feature(regop, 0xb0);

	/* disable chip internal ecc */
	if (new_stat & SPINAND_STATUS_ECC_EN)
		new_stat &= ~(SPINAND_STATUS_ECC_EN);

	/* enable quad command mode */
	if (!(new_stat & SPINAND_STATUS_EQ))
		new_stat |= SPINAND_STATUS_EQ;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xb0, new_stat);
}
/******************************************************************************/

static void mira_chip_prepare_default(struct flash_regop *regop)
{
	u8 stat, new_stat;

	/* disable block protect */
	new_stat = stat = (u8)spinand_get_feature(regop, 0xa0);
	if (new_stat & SPINAND_BLOCK_PROT_GIGA)
		new_stat &= ~SPINAND_BLOCK_PROT_GIGA;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xa0, new_stat);

	new_stat = stat = (u8)spinand_get_feature(regop, 0xb0);

	/* disable chip internal ecc */
	if (new_stat & SPINAND_STATUS_ECC_EN)
		new_stat &= ~SPINAND_STATUS_ECC_EN;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xb0, new_stat);
}
/******************************************************************************/

static void ato_chip_prepare_default(struct flash_regop *regop)
{
	u8 stat, new_stat;

	/* disable block protect */
	new_stat = stat = (u8)spinand_get_feature(regop, 0xa0);
	if (new_stat & SPINAND_BLOCK_PROT_GIGA)
		new_stat &= ~SPINAND_BLOCK_PROT_GIGA;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xa0, new_stat);

	new_stat = stat = (u8)spinand_get_feature(regop, 0xb0);

	/* enable quad command mode */
	if (!(new_stat & SPINAND_STATUS_EQ))
		new_stat |= SPINAND_STATUS_EQ;

	if (stat != new_stat)
		spinand_set_feature(regop, 0xb0, new_stat);
}
/******************************************************************************/

static struct spinand_driver spinand_driver_table[] = {
	{
		.id        = {0xef, 0xaa, 0x21},
		.id_len    = 3,
		.chip_prepare = winbond_chip_prepare_w25n01gv,
	}, {
		.id        = {0xc8},
		.id_len    = 1,
		.chip_prepare = giga_chip_prepare_default,
	}, {
		.id        = {0x9b, 0x12},
		.id_len    = 2,
		.chip_prepare = ato_chip_prepare_default,
	}, {
		.id        = {0xc2},
		.id_len    = 1,
		.chip_prepare = giga_chip_prepare_default,
	}, {
		.id        = {0xc1},
		.id_len    = 1,
		.chip_prepare = giga_chip_prepare_default,
	}, {
		.id        = {0xa1},
		.id_len    = 1,
		.chip_prepare = giga_chip_prepare_default,
	}, {
		.id        = {0xc8, 0x21},
		.id_len    = 2,
		.chip_prepare = mira_chip_prepare_default,
	}, {
		.id        = {0xc9},
		.id_len    = 1,
		.chip_prepare = giga_chip_prepare_default,
	}, {
		.id        = {0x98},
		.id_len    = 1,
		.chip_prepare = mira_chip_prepare_default,
	}, {
		.id        = {0},
		.id_len    = 0,
	}
};
/******************************************************************************/
/*
 *  get some spi-nand special operation interface.
 */
struct spinand_driver *spinand_get_drvier(u8 *id, int id_len)
{
	struct spinand_driver *drv = spinand_driver_table;

	while (drv->id_len) {
		if (drv->id_len <= id_len && !memcmp(drv->id, id, drv->id_len))
			break;
		drv++;
	}

	if (!drv->get_feature)
		drv->get_feature = spinand_get_feature;
	if (!drv->set_feature)
		drv->set_feature = spinand_set_feature;
	if (!drv->chip_prepare)
		drv->chip_prepare = spinand_chip_prepare;

	return drv;
}
