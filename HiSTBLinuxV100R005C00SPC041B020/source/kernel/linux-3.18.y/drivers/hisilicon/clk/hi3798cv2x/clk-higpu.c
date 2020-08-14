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
 * Create By Xia Qing 2015.12.03
 *
******************************************************************************/

#include <linux/delay.h>
#include <dt-bindings/clock/hi3798cv200-clock.h>
#include <linux/hikapi.h>

#include "clk-hisi.h"

/******************************************************************************/

#define GPU_MAX_FREQ_NUM	6
#define GPU_INIT_FREQ_INDEX	0
#define GPU_FREQ_SEL_CFG_CRG	0x7
#define GPU_BEGIN_CFG_BYPASS	0x200
#define GPU_SW_BEGIN_CFG	0x400
#define GPU_CLK_RESET		0xd4
#define GPU_SRST_REG		0x10
#define GPU_CKEN		0x1
#define GPU_CLK_MUX		0x154
#define GPU_CLK_MUX_MASK	0xe0
#define GPU_CLK_MUX_SHIFT	0x5

struct gpll_table {
	unsigned long rate; /* unit: KHZ */
	int reg_value;
};

static struct gpll_table gpu_pll_table[GPU_MAX_FREQ_NUM] = {
	{200000000, 0x7},
	{300000000, 0x4},
	{400000000, 0x1},
	{500000000, 0x6},
	{600000000, 0x3},
	{675000000, 0x5}
};

static unsigned long hiclk_recalc_rate_gpu(struct clk_hw *hw, unsigned long parent_rate);
static int hiclk_set_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate);

static int hiclk_enable_gpu(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	u32 reg;

	reg = readl(clk->peri_crg_base + GPU_CLK_RESET);
	reg |= (GPU_SRST_REG|GPU_CKEN);

	writel(reg, clk->peri_crg_base + GPU_CLK_RESET);

	reg &= ~GPU_SRST_REG;
	writel(reg, clk->peri_crg_base + GPU_CLK_RESET);

	hiclk_set_rate_gpu(hw, gpu_pll_table[GPU_INIT_FREQ_INDEX].rate, 0);

	hiclk_recalc_rate_gpu(hw, 0);

	return 0;
}


static void hiclk_disable_gpu(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	u32 reg;

	reg = readl(clk->peri_crg_base + GPU_CLK_RESET);
	reg &= ~GPU_SRST_REG;
	writel(reg, clk->peri_crg_base + GPU_CLK_RESET);

	reg &= ~GPU_CKEN;
	writel(reg, clk->peri_crg_base + GPU_CLK_RESET);

	return;
}

static int hiclk_set_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	u32 reg, value;
	int i;

	reg = readl(clk->peri_crgx);
	reg &= ~(GPU_FREQ_SEL_CFG_CRG | GPU_SW_BEGIN_CFG);

	for (i = 0; i < GPU_MAX_FREQ_NUM; i++) {
		if (gpu_pll_table[i].rate == rate) {
			value = gpu_pll_table[i].reg_value;

			reg |= (GPU_BEGIN_CFG_BYPASS | value);
			writel(reg, clk->peri_crgx);

			reg |= GPU_SW_BEGIN_CFG;
			writel(reg, clk->peri_crgx);

			/* check if pll switch is over */
			for (i = 0; i < 100; i++) {
				reg = readl(clk->peri_crg_base + GPU_CLK_MUX);
				if (((reg & GPU_CLK_MUX_MASK) >> GPU_CLK_MUX_SHIFT) == value)
					break;
			}

			clk->rate = rate;
			hw->clk->rate = rate;
			return 0;
		}
	}

	return -1;
}

static unsigned long hiclk_recalc_rate_gpu(struct clk_hw *hw, unsigned long parent_rate)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	int i;
	u32 reg;
	unsigned long rate;

	reg = readl(clk->peri_crgx);
	reg &= GPU_FREQ_SEL_CFG_CRG;

	for (i = 0; i < GPU_MAX_FREQ_NUM; i++) {
		if (gpu_pll_table[i].reg_value == reg) {
			rate = gpu_pll_table[i].rate;

			clk->rate = rate;
			hw->clk->rate = rate;

			return rate;
		}
	}

	return (unsigned long)-1;
}

static long hiclk_round_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)
{
	unsigned long prev, next;
	int i;

	if (rate < gpu_pll_table[0].rate)
		return gpu_pll_table[0].rate;

	for (i = 1; i < GPU_MAX_FREQ_NUM; i++) {
		if (rate < gpu_pll_table[i].rate) {
			prev = rate - gpu_pll_table[i -1].rate;
			next = gpu_pll_table[i].rate - rate;

			return (prev > next) ? gpu_pll_table[i].rate : gpu_pll_table[i-1].rate;
		}
	}

	return gpu_pll_table[GPU_MAX_FREQ_NUM - 1].rate;
}

struct clk_ops clk_ops_higpu = {
	.enable = hiclk_enable_gpu,
	.disable = hiclk_disable_gpu,
	.set_rate = hiclk_set_rate_gpu,
	.recalc_rate = hiclk_recalc_rate_gpu,
	.round_rate = hiclk_round_rate_gpu,
};
