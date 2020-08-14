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
 * Create By Liu Hui 2015.07.10
 *
******************************************************************************/

#include <linux/delay.h>
#include <dt-bindings/clock/hifone-clock.h>
#include <linux/hikapi.h>

#include "clk-hisi.h"

/******************************************************************************/

#define KBASE_HISI_MAX_FREQ_NUM 6

#define KBASE_HISI_INIT_RATE_INX 0

typedef enum
{
	KBASE_HISI_CRG_FREQ_OFFSET		= 0x124,
	KBASE_HISI_CRG_GPLL_OK				= 0x154,
	KBASE_HISI_CRG_REST_OFFSET 			= 0x0d4,
	KBASE_HISI_CRG_GPLL_TUNE_INT 			= 0x208,
	KBASE_HISI_CRG_GPLL_TUNE_FRAC 		= 0x20C,
	KBASE_HISI_CRG_GPLL_TUNE_STEP_INT 	= 0x210,
	KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC 	= 0x214,
	KBASE_HISI_CRG_GPLL_TUNE_MODE 		= 0x218,
	KBASE_HISI_CRG_GPLL0 					= 0x244,
	KBASE_HISI_CRG_GPLL1 					= 0x248,
	KBASE_HISI_CRG_GPLL_SSMOD 			= 0x270,
	KBASE_HISI_CRG_GPLL_TUNE_BUSY		= 0x2BC
} KBASE_HISI_CRG;

typedef struct GPLL_TABLE
{
	unsigned long rate;       		/* unit: KHZ */
	int volt;			/* unit: mV */
	int hpm;
} GPLL_TABLE;

GPLL_TABLE kbase_gpll_table[KBASE_HISI_MAX_FREQ_NUM] =
{
    {198000000, 800, 0x00},
    {300000000, 820, 0x00},
    {396000000, 840, 0xd0},
    {432000000, 860, 0xf0},
    {498000000, 910, 0x100},
    {600000000, 980, 0x126}
};

static unsigned long	hiclk_recalc_rate_gpu(struct clk_hw *hw, unsigned long parent_rate);
static int hiclk_set_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate);

static int hiclk_enable_gpu(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	/*switch to GPLL*/
	writel(0x202, clk->peri_crg_base + KBASE_HISI_CRG_FREQ_OFFSET);
	writel(0x602, clk->peri_crg_base + KBASE_HISI_CRG_FREQ_OFFSET);

	hiclk_set_rate_gpu(hw, kbase_gpll_table[KBASE_HISI_INIT_RATE_INX].rate, 0);

	hiclk_recalc_rate_gpu(hw, 0);

	return 0;
}


static void hiclk_disable_gpu(struct clk_hw *hw)
{
	return;
}

static int hiclk_set_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
	int postdiv1,postdiv2,refdiv,fbdiv;
	int reg;
	int i;

	struct hiclk_hw *clk = to_hiclk_hw(hw);
	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL0);
	/*bit[30:28] pll post div1*/
	postdiv1 = (reg>>24)&0x7;
	/*bit[26:24] pll post div2*/
	postdiv2 = (reg>>28)&0x7;

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL1);
	/*bit[17:12] pll reference div*/
	refdiv = (reg>>12)&0x3f;
	fbdiv = (rate*postdiv1*postdiv2*refdiv)/(24*1000*1000);

	reg = reg&0xfdffffff;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL1);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_SSMOD);
	/*bit[0] SSMOD clock gating enable, bit[4] SSMOD spread control*/
	reg = reg|0x11;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_SSMOD);
	/*bit[2] to 0, SSMOD enable*/
	reg = reg&0xffb;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_SSMOD);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_INT);
	reg = (reg&0xFFFFF000)|fbdiv;
	/* set fbdiv of target cpll */
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_INT);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_FRAC);
	reg = reg&0xFF000000;
	/* set frac of target cpll to 0 */
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_FRAC);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_STEP_INT);
	reg = (reg&0xFF000000)|0x1;
	/* set integer of tune step */
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_STEP_INT);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC);
	reg = reg&0xFF000000;
	/* set integer of tune step */
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_STEP_FRAC);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_MODE);
	/* set gpll tune mode */
	reg = reg|0x4;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_MODE);
	/* begin one tune process */
	reg = reg&0xfffffff7;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_MODE);
	reg = reg|0x8;
	writel(reg, clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_MODE);
	udelay(10);

	/* check if pll switch is over */
	for(i = 0; i < 100; i++)
	{
		reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_BUSY);
		if((reg&0x2000)==0)
		{
			break;
		}
		udelay(10);
	}

	clk->rate = rate;
	hw->clk->rate = rate;

	return 0;
}

static unsigned long	hiclk_recalc_rate_gpu(struct clk_hw *hw, unsigned long parent_rate)
{
	int postdiv1,postdiv2,refdiv,fbdiv;
	int reg;
	unsigned long rate;

	struct hiclk_hw *clk = to_hiclk_hw(hw);

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL0);
	/*bit[30:28] pll post div1*/
	postdiv1 = (reg>>24)&0x7;
	/*bit[26:24] pll post div2*/
	postdiv2 = (reg>>28)&0x7;

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL1);
	/*bit[17:12] pll reference div*/
	refdiv = (reg>>12)&0x3f;

	reg = readl(clk->peri_crg_base + KBASE_HISI_CRG_GPLL_TUNE_BUSY);
	fbdiv = reg&0xfff;

	if(0 != postdiv1 * postdiv2 * refdiv)
	{
		rate = fbdiv * 24000 * 1000/(postdiv1 * postdiv2 * refdiv);
	}

	clk->rate = rate;
	hw->clk->rate = rate;

	return rate;
}

static long	hiclk_round_rate_gpu(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)
{
	unsigned long prev, next;
	int i;

	if(rate < kbase_gpll_table[0].rate)
	{
		return kbase_gpll_table[0].rate;
	}

	for(i = 0; i < KBASE_HISI_MAX_FREQ_NUM; i++)
	{
		if(rate < kbase_gpll_table[i].rate)
		{
			prev = rate - kbase_gpll_table[i -1].rate;
			next = kbase_gpll_table[i].rate - rate;

			return (prev > next) ? kbase_gpll_table[i].rate: kbase_gpll_table[i-1].rate;
		}
	}

	return kbase_gpll_table[KBASE_HISI_MAX_FREQ_NUM - 1].rate;
}


struct clk_ops clk_ops_higpu = {
	.enable = hiclk_enable_gpu,
	.disable = hiclk_disable_gpu,
	.set_rate = hiclk_set_rate_gpu,
	.recalc_rate = hiclk_recalc_rate_gpu,
	.round_rate = hiclk_round_rate_gpu,
};
