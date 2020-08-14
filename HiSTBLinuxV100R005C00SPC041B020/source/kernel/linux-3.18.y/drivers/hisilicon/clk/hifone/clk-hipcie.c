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
#include <mach/hardware.h>

#include "clk-hisi.h"
/******************************************************************************/

#define PERI_CTRL                        (0x0008)
#define PERI_COMBPHY1_CFG                (0x858)
#define PERI_COMBPHY3_CFG                (0x868)

#define COMBPHY1_REF_CKEN                BIT(8)
#define COMBPHY1_SRST_REQ                BIT(12)
#define COMBPHY1_RSTN_SEL_CTRL           BIT(13)
#define COMBPHY1_REFCLK_SEL             (0x7<<9)

#define COMBPHY3_REF_CKEN                BIT(24)
#define COMBPHY3_SRST_REQ                BIT(28)
#define COMBPHY3_RSTN_SEL_CTRL           BIT(29)
#define COMBPHY3_REFCLK_SEL              (0x7<<25)

#define COMBPHY1_SEL0                    BIT(11)
#define COMBPHY1_SEL1                    BIT(12)
#define COMBPHY3_SEL0                    BIT(14)
#define COMBPHY3_SEL1                    BIT(15)

#define NANO_BYPASS_CODEC                BIT(31)
#define BYPASS_PCIECTRL_ANALOG           BIT(30)
#define NANO_BUFFER_MODE                 (0x3<<28)
#define NANO_TEST_WRITE                  BIT(24)
#define NANO_TEST_DATA                   (0xf<<20)
#define NANO_TEST_ADDR                   (0x1f<<12)
#define NANO_RX_STANDBY                  BIT(11)
#define NANO_TX_MARGIN                   (0x7<<8)
#define NANO_TX_DEEMP                    (0x3<<5)
#define NANO_TX_SWING                    BIT(4)
#define NANO_TX_PATTERN                  (0x3<<2)
#define CLKREF_OUT_IEN                   BIT(1)
#define CLKREF_OUT_OEN                   BIT(0)

#define PCIE0_BUS_CKEN                   BIT(0)
#define PCIE0_SYS_CKEN                   BIT(1)
#define PCIE0_PIPE_CKEN                  BIT(2)
#define PCIE0_AUX_CKEN                   BIT(3)
#define PCIE0_BUS_SRST_REQ               BIT(4)
#define PCIE0_SYS_SRST_REQ               BIT(5)
#define PCIE0_SRST_REQ                   BIT(6)
#define PCIE1_BUS_CKEN                   BIT(8)
#define PCIE1_SYS_CKEN                   BIT(9)
#define PCIE1_PIPE_CKEN                  BIT(10)
#define PCIE1_AUX_CKEN                   BIT(11)
#define PCIE1_BUS_SRST_REQ               BIT(12)
#define PCIE1_SYS_SRST_REQ               BIT(13)
#define PCIE1_SRST_REQ                   BIT(14)

extern int combphy_fixed;
 /******************************************************************************/

static void nano_register_write(struct hiclk_hw *clk, u32 addr, 
				u32 offset, u32 value)
{
	u32 reg;

	reg = readl(clk->peri_ctrl_base + addr);
	mdelay(10);
	reg &= ~NANO_TEST_ADDR;
	reg &= ~NANO_TEST_DATA;
	reg |= (offset << 12);
	reg |= (value << 20);
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + addr);
	reg |= NANO_TEST_WRITE;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + addr);
	reg &= ~NANO_TEST_WRITE;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);
}
/******************************************************************************/

static void nano_tx_margin_pcie(struct hiclk_hw *clk, u32 addr, u32 value)
{
	u32 reg;

	reg = readl(clk->peri_ctrl_base + addr);
	reg |= BYPASS_PCIECTRL_ANALOG;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + addr);
	reg &= ~NANO_TX_MARGIN;
	reg |= value << 8;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);
}
/******************************************************************************/

static void nano_tx_deemp_pcie(struct hiclk_hw *clk, u32 addr, u32 value)
{
	u32 reg;

	reg = readl(clk->peri_ctrl_base + addr);
	reg |= BYPASS_PCIECTRL_ANALOG;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + addr);
	reg &= ~NANO_TX_DEEMP;
	reg |= value << 5;
	writel(reg, clk->peri_ctrl_base + addr);
	mdelay(10);
}
/******************************************************************************/

static int hiclk_enable_pcie_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* Config ComboPHY1 to PCIe */
	reg = readl(clk->peri_ctrl_base + PERI_CTRL);
	reg &= ~(COMBPHY1_SEL0 | COMBPHY1_SEL1);
	writel(reg, clk->peri_ctrl_base + PERI_CTRL);
	mdelay(10);

	/* Config ComboPHY1 enable 8B10B */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~NANO_BYPASS_CODEC;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* reset ComboPHY1 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY1_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY1 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY1_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* Config ComboPHY1 refclk */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY1_REFCLK_SEL;
	reg |=0x0<<9;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* open ComboPHY1 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY1_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* release reset ComboPHY1 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY1_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* SSC off
	* before ec: default 0 is ssc on ,here turn off ssc 
	* after ec: default 0 is ssc off , here no operation
	*/
	if (!combphy_fixed)
		nano_register_write(clk, PERI_COMBPHY1_CFG, 0x2, 0x8);

	/* TX_MARGIN 1100mV */
	nano_tx_margin_pcie(clk, PERI_COMBPHY1_CFG, 0x1);

	/* TX_DEEMP -3.5dB */
	nano_tx_deemp_pcie(clk, PERI_COMBPHY1_CFG, 0x1);

	/* TX_SWING_COMP */
	nano_register_write(clk, PERI_COMBPHY1_CFG, 0xd, 0xe);

	/* SPLIT_CP_DIS */
	nano_register_write(clk, PERI_COMBPHY1_CFG, 0xc, 0x9);

	/* Output EP clock by phy1 and phy3 */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg |= CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* Open Pcie0 Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= ( PCIE0_BUS_CKEN
		| PCIE0_PIPE_CKEN
		| PCIE0_AUX_CKEN
		| PCIE0_SYS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* Reset Pcie0 Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE0_BUS_SRST_REQ
		|PCIE0_SYS_SRST_REQ
		|PCIE0_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* Deassert reset Pcie0 Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE0_BUS_SRST_REQ
		|PCIE0_SYS_SRST_REQ
		|PCIE0_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	return 0;
}
/******************************************************************************/

static int hiclk_enable_pcie_host1(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* Config ComboPHY3 to PCIe */
	reg = readl(clk->peri_ctrl_base + PERI_CTRL);
	reg &= ~(COMBPHY3_SEL0 | COMBPHY3_SEL1);
	writel(reg, clk->peri_ctrl_base + PERI_CTRL);
	mdelay(10);

	/* Config ComboPHY3 enable 8B10B */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY3_CFG);
	reg &=~NANO_BYPASS_CODEC;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY3_CFG);
	mdelay(10);

	/* reset ComboPHY3 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY3_SRST_REQ;
	writel(reg,clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY3 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY3_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* Config ComboPHY3 refclk */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY3_REFCLK_SEL;
	reg |= 0x0 << 25;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* open ComboPHY3 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY3_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* release reset Nano ComboPHY3 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY3_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* SSC off
	* before ec: default 0 is ssc on ,here turn off ssc 
	* after ec: default 0 is ssc off , here no operation
	*/
	if (!combphy_fixed)
		nano_register_write(clk, PERI_COMBPHY3_CFG, 0x2, 0x8);

	/* TX_MARGIN 1100mV */
	nano_tx_margin_pcie(clk, PERI_COMBPHY3_CFG, 0x1);

	/* TX_DEEMP -3.5dB */
	nano_tx_deemp_pcie(clk, PERI_COMBPHY3_CFG, 0x1);

	/* TX_SWING_COMP */
	nano_register_write(clk, PERI_COMBPHY3_CFG, 0xd, 0xe);	

	/* SPLIT_CP_DIS */
	nano_register_write(clk, PERI_COMBPHY3_CFG, 0xc,0x9);

	/* Output EP clock by phy1 and phy3 */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg |= CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* Open PCIe1 Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE1_BUS_CKEN 
		| PCIE1_PIPE_CKEN
		| PCIE1_AUX_CKEN
		| PCIE1_SYS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* Reset PCIe1 Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE1_BUS_SRST_REQ
		| PCIE1_SYS_SRST_REQ
		| PCIE1_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* PCIe1 Retreat from the reset state */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE1_BUS_SRST_REQ
		| PCIE1_SYS_SRST_REQ
		| PCIE1_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	return 0;
}
/******************************************************************************/

static void hiclk_disable_pcie_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* reset PCIe0 Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE0_BUS_SRST_REQ
		| PCIE0_SYS_SRST_REQ
		| PCIE0_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* close PCIe0 Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE0_BUS_CKEN
		| PCIE0_SYS_CKEN
		| PCIE0_PIPE_CKEN
		| PCIE0_AUX_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* close EP clock */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* reset ComboPHY1 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY1_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY1 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY1_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* Config ComboPHY1 refclk */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY1_REFCLK_SEL;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);
}
/******************************************************************************/

static void hiclk_disable_pcie_host1(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* reset PCIe1 Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE1_BUS_SRST_REQ
		| PCIE1_SYS_SRST_REQ
		| PCIE1_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* close PCIe1 Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE1_BUS_CKEN
		| PCIE1_SYS_CKEN
		| PCIE1_PIPE_CKEN
		| PCIE1_AUX_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* close EP clock */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* reset ComboPHY3 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY3_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY3 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY3_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* Config ComboPHY3 refclk */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &=~COMBPHY3_REFCLK_SEL;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);
}
/******************************************************************************/

struct clk_ops clk_ops_hipcie_host0 = {
	.enable = hiclk_enable_pcie_host0,
	.disable = hiclk_disable_pcie_host0,
};

struct clk_ops clk_ops_hipcie_host1 = {
	.enable = hiclk_enable_pcie_host1,
	.disable = hiclk_disable_pcie_host1,
};