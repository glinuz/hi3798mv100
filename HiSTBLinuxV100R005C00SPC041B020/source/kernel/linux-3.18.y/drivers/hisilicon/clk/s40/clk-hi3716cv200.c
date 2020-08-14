/******************************************************************************
 *  Copyright (C) 2014 Hisilicon Technologies CO.,LTD.
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
 * Create By Cai Zhiyong 2014.12.22
 *
******************************************************************************/
#define DRVNAME "hiclk"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/hisilicon/freq.h>
#include <dt-bindings/clock/hi3716cv200-clock.h>

#include "clk-hisi.h"

#define CLK(_id, _mask, _value, _rstbit, _rate, _ops) \
	{.id = _id,         \
	 .name = #_id,      \
	 .offset = _id,     \
	 .mask = _mask,     \
	 .value = _value,   \
	 .rstbit = _rstbit, \
	 .rate = _rate,     \
	 .ops = _ops,}

/******************************************************************************/

extern struct clk_ops clk_ops_hisfc350;
extern struct clk_ops clk_ops_hiahci;
extern struct clk_ops clk_ops_hiusb2;

static struct hiclk_hw hiclks_hw[] = {
	CLK(PERI_CRG24_NANDC, 0x701,   0x401,   BIT(4), 0, NULL),
	CLK(PERI_CRG39_SDIO0, 0x303,   0x103,   BIT(4), 0, NULL),
	CLK(PERI_CRG40_SDIO1, 0x77303, 0x32103, BIT(4), 0, NULL),
	CLK(PERI_CRG23_SFC,   0x1,     0x1,     BIT(4), 0, &clk_ops_hisfc350),
	CLK(PERI_CRG42_SATA3CTRL, 0x1F, 0x1F, BIT(8)| BIT(9), 0, &clk_ops_hiahci),
	CLK(PERI_CRG46_USB2CTRL, 0x0,  0x0,   0x0, 0, &clk_ops_hiusb2),
};
/******************************************************************************/

static unsigned long hiclk_recalc_rate_hi3716cv200(struct clk_hw *hw,
						   unsigned long parent_rate)
{
	u32 val;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	val = readl(clk->peri_crgx);

	switch (clk->id) {
		case PERI_CRG39_SDIO0:
		case PERI_CRG40_SDIO1: {
			unsigned long rate[] = {
				_75MHz, _100MHz, _50MHz, _15MHz};
			clk->rate = rate[(val >> 8) & 0x3];
			break;
		}
		case PERI_CRG23_SFC: {
			unsigned long rate[] = {
				_75MHz, _100MHz, _50MHz, _37dot5MHz};
			val = (val >> 8) & 0x7;
			if (val < 4)
				clk->rate = _12MHz;
			else
				clk->rate = rate[val - 4];

			break;
		}
		case PERI_CRG24_NANDC: {
			unsigned long rate[] = {
				_200MHz, _150MHz, _125MHz, _100MHz};
				val = (val >> 8) & 0x7;
				if (val < 4)
					clk->rate = _24MHz;
				else
					clk->rate = rate[val - 4];
			break;
		}
		case PERI_CRG42_SATA3CTRL:
			clk->rate = _50MHz;
			break;
		default:
			break;
	}

	return clk->rate;
}
/******************************************************************************/

static int hiclk_enable_hi3716cv200(struct clk_hw *hw)
{
	hiclk_enable(hw);
	hiclk_recalc_rate_hi3716cv200(hw, 0);
	return 0;
}
/******************************************************************************/

static struct clk_ops clk_ops_hi3716cv200 = {
	.enable = hiclk_enable_hi3716cv200,
	.recalc_rate = hiclk_recalc_rate_hi3716cv200,
};
/******************************************************************************/

static void __init hi3716cv200_clocks_init(struct device_node *np)
{
	int ix;

	for (ix = 0; ix < ARRAY_SIZE(hiclks_hw); ix++) {
		struct hiclk_hw *hihw = &hiclks_hw[ix];
		struct clk_ops *ops = hihw->ops;

		if (!ops)
			continue;

		if (!ops->enable)
			ops->enable = hiclk_enable_hi3716cv200;
		if (!ops->recalc_rate)
			ops->recalc_rate = hiclk_recalc_rate_hi3716cv200;
	}

	hiclk_clocks_init(np, hiclks_hw, ARRAY_SIZE(hiclks_hw),
		CLK_MAX >> 2, &clk_ops_hi3716cv200);
}
CLK_OF_DECLARE(fixed_clock, "hi3716cv200.clock", hi3716cv200_clocks_init);
