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
#include <dt-bindings/clock/hi3716cv200-clock.h>
#include <linux/hikapi.h>
#include <mach/hardware.h>

#include "clk-hisi.h"
/******************************************************************************/

#define PERI_CTRL                       (0x0008)
#define PERI_COMBPHY1_CFG               (0x0858)

#define SATA0_PWREN                     BIT(10)
#define COMBPHY_SEL0                    BIT(11)
#define COMBPHY_SEL1                    BIT(12)

#define NANO_BYPASS_CODEC               BIT(31)
#define NANO_BUFFER_MODE                (0x3<<28)
#define NANO_TEST_WRITE                 BIT(24)
#define NANO_TEST_DATA                  (0xf<<20)
#define NANO_TEST_ADDR                  (0x1f<<12)
#define NANO_RX_STANDBY                 BIT(11)
#define NANO_TX_PATTERN                 (0x3<<2)
#define CLKREF_OUT_OEN                  BIT(0)

#define COMBPHY_SRST_REQ                BIT(5)
#define COMBPHY_REFCLK_SEL              (0x3<<2)
#define COMBPHY_REF_CKEN                BIT(1)
#define COMBPHY_BUS_CKEN                BIT(0)

#define PCIE_BUS_CKEN                   BIT(0)
#define PCIE_SYS_CKEN                   BIT(1)
#define PCIE_PIPE_CKEN                  BIT(2)
#define PCIE_AUX_CKEN                   BIT(3)
#define PCIE_BUS_SRST_REQ               BIT(4)
#define PCIE_SYS_SRST_REQ               BIT(5)
#define PCIE_SRST_REQ                   BIT(6)
#define PCIEPHY_RST_SEL                 BIT(8)
 /******************************************************************************/

static void nano_register_write(struct hiclk_hw *clk, u32 addr, u32 value)
{
	u32 reg;

	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~NANO_TEST_ADDR;
	reg &= ~NANO_TEST_DATA;
	reg |= (addr<<12);
	reg |= (value<<20);
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg |= NANO_TEST_WRITE;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~NANO_TEST_WRITE;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);
}
 /******************************************************************************/

static int hiclk_enable_pcie(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* Power on SATA0 PWREN */
	reg = readl(clk->peri_ctrl_base + PERI_CTRL);
	reg |= SATA0_PWREN;
	writel(reg, clk->peri_ctrl_base + PERI_CTRL);
	mdelay(10);

	/* Config ComboPHY1 to PCIe */
	reg = readl(clk->peri_ctrl_base + PERI_CTRL);
	reg &= ~(COMBPHY_SEL0 | COMBPHY_SEL1);
	writel(reg, clk->peri_ctrl_base + PERI_CTRL);
	mdelay(10);

	/* Config ComboPHY1 enable 8B10B */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~NANO_BYPASS_CODEC;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* select reset from CRG */
	reg = readl(clk->peri_crg_base +  PERI_CRG99_PCIECTRL);
	reg |= (PCIEPHY_RST_SEL);
	writel(reg,clk->peri_crg_base +  PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* reset ComboPHY1 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY1 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~(COMBPHY_BUS_CKEN |COMBPHY_REF_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* Config ComboPHY1 refclk */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY_REFCLK_SEL;
	reg |= (0x2<<2);
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* open ComboPHY1 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= (COMBPHY_BUS_CKEN |COMBPHY_REF_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* release reset ComboPHY1 */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* SSC off */
	nano_register_write(clk, 0x02, 0x08);

	/* SPLIT_CP_DIS */
	nano_register_write(clk, 0x1a, 0x04);

	/* open EP clock  0 open  1 close */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg &= ~CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* Open PCIe Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE_BUS_CKEN 
		| PCIE_PIPE_CKEN
		| PCIE_AUX_CKEN
		| PCIE_SYS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* Reset PCIe Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE_BUS_SRST_REQ
		| PCIE_SYS_SRST_REQ
		| PCIE_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* PCIe Retreat from the reset state */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE_BUS_SRST_REQ
		| PCIE_SYS_SRST_REQ
		| PCIE_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	return 0;
}
/******************************************************************************/

static void hiclk_disable_pcie(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* reset PCIe Controller */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= (PCIE_BUS_SRST_REQ
		| PCIE_SYS_SRST_REQ
		| PCIE_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* close PCIe Controller clk */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg &= ~(PCIE_BUS_CKEN
		|PCIE_SYS_CKEN
		|PCIE_PIPE_CKEN
		|PCIE_AUX_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);
		
	/* close EP clock */
	reg = readl(clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	reg |= CLKREF_OUT_OEN;
	writel(reg, clk->peri_ctrl_base + PERI_COMBPHY1_CFG);
	mdelay(10);

	/* select reset from CRG */
	reg = readl(clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	reg |= PCIEPHY_RST_SEL;
	writel(reg,clk->peri_crg_base + PERI_CRG99_PCIECTRL);
	mdelay(10);

	/* reset ComboPHY */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);

	/* close ComboPHY cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~COMBPHY_REF_CKEN;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(10);
}
/******************************************************************************/

struct clk_ops clk_ops_hipcie_hi3716mv420 = {
	.enable = hiclk_enable_pcie,
	.disable = hiclk_disable_pcie,
};