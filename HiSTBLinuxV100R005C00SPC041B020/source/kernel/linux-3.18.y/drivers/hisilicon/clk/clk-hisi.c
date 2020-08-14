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
 * Create By Cai Zhiyong 2015.6.5
 *
******************************************************************************/

#define DRVNAME "clk-hisi"
#define pr_fmt(fmt) DRVNAME ": " fmt

#include <linux/clkdev.h>
#include <linux/of_address.h>
#include <linux/delay.h>

#include "clk-hisi.h"

/******************************************************************************/

static void __hisi_clk_enable(struct hiclk_hw *clk)
{
	if (clk->value) {
		u32 val = readl(clk->peri_crgx);
		val &= ~clk->mask;
		val |= clk->value;
		writel(val, clk->peri_crgx);

		val = readl(clk->peri_crgx);
		if ((val & clk->mask) != clk->value)
			pr_warning("enable '%s' clock fail: address:%p want:%#x, real:%#x\n",
				clk->name, clk->peri_crgx, clk->value, val);
	}

	clk->flags |= CLKHW_ENABLE;
}
/******************************************************************************/

static void __hisi_clk_disable(struct hiclk_hw *clk)
{
	if (clk->mask) {
		u32 val = readl(clk->peri_crgx);
		val &= ~clk->mask;
		writel(val, clk->peri_crgx);
	}

	clk->flags &= ~CLKHW_ENABLE;
}
/******************************************************************************/

static void __hisi_clk_reset(struct hiclk_hw *clk)
{
	if (clk->rstbit) {
		u32 val = readl(clk->peri_crgx);
		val |= clk->rstbit;
		writel(val, clk->peri_crgx);
	}

	clk->flags |= CLKHW_RESET;
}
/******************************************************************************/

static void __hisi_clk_unreset(struct hiclk_hw *clk)
{
	if (clk->rstbit) {
		u32 val = readl(clk->peri_crgx);
		val &= ~clk->rstbit;
		writel(val, clk->peri_crgx);
	}

	clk->flags &= ~CLKHW_RESET;
}
/******************************************************************************/

int hiclk_enable(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	__hisi_clk_enable(clk);

	if (clk->flags & CLKHW_RESET)
		__hisi_clk_unreset(clk);

	return 0;
}
/******************************************************************************/

void hiclk_disable(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	__hisi_clk_disable(clk);
}
/******************************************************************************/

unsigned long hiclk_recalc_rate(struct clk_hw *hw, unsigned long parent_rate)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	return clk->rate;
}
/******************************************************************************/

long hiclk_round_rate(struct clk_hw *hw, unsigned long rate,
		      unsigned long *parent_rate)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	return rate;
}
/******************************************************************************/

int hiclk_prepare(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	__hisi_clk_enable(clk);
	__hisi_clk_unreset(clk);
	__hisi_clk_reset(clk);

	return 0;
}
/******************************************************************************/

void hiclk_unprepare(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	clk->flags |= CLKHW_RESET;
}
/******************************************************************************/

void hiclk_init(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	pr_debug("%s: %s offset:%#x peri_crgx:%p\n",
		__func__, clk->name, clk->offset, clk->peri_crgx);

	__hisi_clk_enable(clk);
	__hisi_clk_reset(clk);
	__hisi_clk_disable(clk);
}
/******************************************************************************/

struct clk * __init hiclk_register(struct hiclk_hw *hihw,
				   struct clk_ops *clkops)
{
	struct clk *clk;
	struct clk_init_data init;

	init.name = hihw->name;
	init.flags = CLK_IS_ROOT | CLK_IS_BASIC;
	init.parent_names = NULL;
	init.num_parents = 0;

	if (!hihw->ops)
		hihw->ops = clkops;

	if (!hihw->ops->init)
		hihw->ops->init = hiclk_init;
	if (!hihw->ops->prepare)
		hihw->ops->prepare = hiclk_prepare;
	if (!hihw->ops->unprepare)
		hihw->ops->unprepare = hiclk_unprepare;
	if (!hihw->ops->enable)
		hihw->ops->enable = hiclk_enable;
	if (!hihw->ops->disable)
		hihw->ops->disable = hiclk_disable;
	if (!hihw->ops->recalc_rate)
		hihw->ops->recalc_rate = hiclk_recalc_rate;
	if (!hihw->ops->round_rate)
		hihw->ops->round_rate = hiclk_round_rate;

	init.ops = hihw->ops;

	hihw->hw.init = &init;

	clk = clk_register(NULL, &hihw->hw);
	if (IS_ERR(clk)) {
		pr_err("%s: register clock fail.\n", __func__);
		return NULL;
	}

	clk_register_clkdev(clk, hihw->name, NULL);

	return clk;
}
/******************************************************************************/

struct clk *of_hiclk_src_get(struct of_phandle_args *clkspec, void *data)
{
	struct clk_onecell_data *clk_data = data;
	unsigned int idx = clkspec->args[0];

	if (idx >= (clk_data->clk_num << 2)) {
		pr_err("%s: invalid clock index %d\n", __func__, idx);
		return ERR_PTR(-EINVAL);
	}

	return clk_data->clks[idx >> 2];
}
/******************************************************************************/

void __init hiclk_clocks_init(struct device_node *node, struct hiclk_hw *clks_hw,
			      int nr_hw, int clk_num, struct clk_ops *clkops)
{
	int ix;
	void __iomem *peri_crg_base;
	void __iomem *peri_ctrl_base;
	struct clk_onecell_data *clk_data;

	peri_crg_base = of_iomap(node, 0);
	if (!peri_crg_base) {
		pr_err("%s: failed to map peri_crg_base\n", __func__);
		return;
	}
	pr_debug("%s: peri_crg_base:%p\n", __func__, peri_crg_base);

	peri_ctrl_base = of_iomap(node, 1);
	if (!peri_ctrl_base) {
		pr_err("%s: failed to map peri_ctrl_base\n", __func__);
		return;
	}
	pr_debug("%s: peri_ctrl_base:%p\n", __func__, peri_ctrl_base);

	clk_data = kzalloc(sizeof(*clk_data), GFP_KERNEL);
	if (!clk_data) {
		pr_err("%s: failed to allocate clk_data.\n", __func__);
		return;
	}

	clk_data->clk_num = clk_num;
	clk_data->clks = kzalloc(sizeof(struct clk *) * clk_num, GFP_KERNEL);
	if (!clk_data->clks) {
		pr_err("%s: fail to allocate clk\n", __func__);
		kfree(clk_data);
		return;
	}

	for (ix = 0; ix < nr_hw; ix++) {
		struct hiclk_hw *hihw = &clks_hw[ix];

		hihw->peri_crg_base = peri_crg_base;
		hihw->peri_crgx = hihw->peri_crg_base + hihw->offset;
		hihw->peri_ctrl_base = peri_ctrl_base;

		clk_data->clks[hihw->id >> 2] = hiclk_register(hihw, clkops);
	}

	of_clk_add_provider(node, of_hiclk_src_get, clk_data);
}
/******************************************************************************/

static void clkdummy_init(struct clk_hw *hw)
{
}

static int clkdummy_enable(struct clk_hw *hw)
{
	return 0;
}

static void clkdummy_disable(struct clk_hw *hw)
{

}

static int clkdummy_prepare(struct clk_hw *hw)
{
	return 0;
}

static void clkdummy_unprepare(struct clk_hw *hw)
{
}

static unsigned long clkdummy_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate)
{
	return parent_rate;
}

struct clk_ops clk_ops_dummy = {
	.init = clkdummy_init,
	.prepare = clkdummy_prepare,
	.unprepare = clkdummy_unprepare,
	.recalc_rate = clkdummy_recalc_rate,
	.enable = clkdummy_enable,
	.disable = clkdummy_disable,
};
