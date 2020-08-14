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
#include <dt-bindings/clock/hi3716cv200-clock.h>

#include "clk-hisi.h"

/******************************************************************************/

static void hiclk_init_hiahci(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* enable phy clock */
	writel(0x1, clk->peri_crg_base + PERI_CRG43_SATA3PHY);
	hiclk_init(hw);
	writel(0x0, clk->peri_crg_base + PERI_CRG43_SATA3PHY);

}
/******************************************************************************/

static int hiclk_prepare_hiahci(struct clk_hw *hw)
{
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* enable phy clock */
	writel(0x1, clk->peri_crg_base + PERI_CRG43_SATA3PHY);
	return hiclk_prepare(hw);
}
/******************************************************************************/

struct clk_ops clk_ops_hiahci = {
	.init = hiclk_init_hiahci,
	.prepare = hiclk_prepare_hiahci,
};
