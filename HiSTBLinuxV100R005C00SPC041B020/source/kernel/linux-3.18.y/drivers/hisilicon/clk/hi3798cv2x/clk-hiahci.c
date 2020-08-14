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

#include <linux/clkdev.h>
#include <linux/hisilicon/freq.h>
#include <dt-bindings/clock/hi3798cv200-clock.h>

#include "clk-hisi.h"

/* 00 PCIE,01 USB3.0,10 SATA */
#define COMBPHY1_SEL_MASK                (BIT(11) | BIT(12))
#define COMBPHY1_SEL_SATA                 BIT(12)

#define PERI_CTRL                        (0x0008)
/******************************************************************************/

static void hiclk_init_hiahci(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	u32 reg, mux;

	//TODO: make sure if combphy1 connected
	mux = readl(clk->peri_ctrl_base + PERI_CTRL);
	mux &= COMBPHY1_SEL_MASK;

	if (mux != COMBPHY1_SEL_SATA) {
		return;
	}

	/* enable phy clock */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~0xff00;
	reg |= 0xd00;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	hiclk_init(hw);

}
/******************************************************************************/

static int hiclk_prepare_hiahci(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	u32 reg, mux;

	//TODO: make sure if combphy1 connected
	mux = readl(clk->peri_ctrl_base + PERI_CTRL);
	mux &= COMBPHY1_SEL_MASK;

	if (mux != COMBPHY1_SEL_SATA) {
		return -1;
	}

	/* enable phy clock */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~0xff00;
	reg |= 0xd00; // VPLL 25MHz
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	return hiclk_prepare(hw);
}
/******************************************************************************/

struct clk_ops clk_ops_hiahci = {
	.init = hiclk_init_hiahci,
	.prepare = hiclk_prepare_hiahci,
};
