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

extern int combphy_fixed;
/******************************************************************************/
#define XHCI_HOST0_ADDR             __io_address(0xF98A0000)
#define XHCI_HOST1_ADDR             __io_address(0xF98B0000)

#define PERI_CTRL                   (0x0008)
#define PERI_USB2                   (0x128)
#define PERI_USB7                   (0x13c)
#define PERI_COMBPHY0               (0x850)
#define PERI_COMBPHY1               (0x858)

#define GTXTHRCFG                   (0xc108)
#define GRXTHRCFG                   (0xc10c)
#define REG_GCTL                    (0xc110)
#define REG_GUSB2PHYCFG0            (0xC200)
#define REG_GUSB3PIPECTL0           (0xc2c0)

#define USB2_PHY3_SRST_REQ            BIT(7)
#define USB2_PHY01_SRST_TREQ1         BIT(9)
#define USB2_PHY3_SRST_TREQ           BIT(11)

#define USB3_BUS_CKEN                 BIT(0)
#define USB3_REF_CKEN                 BIT(1)
#define USB3_SUSPEND_CKEN             BIT(2)
#define USB3_PIPE_CKEN                BIT(3)
#define USB3_UTMI_CKEN                BIT(4)
#define USB3_BUS_GS_CKEN              BIT(5)
#define USB3_BUS_GM_CKEN              BIT(6)
#define USB3_VCC_SRST_REQ             BIT(12)
#define USB3_BUS_CKEN1                BIT(16)
#define USB3_REF_CKEN1                BIT(17)
#define USB3_SUSPEND_CKEN1            BIT(18)
#define USB3_PIPE_CKEN1               BIT(19)
#define USB3_UTMI_CKEN1               BIT(20)
#define USB3_BUS_GS_CKEN1             BIT(21)
#define USB3_BUS_GM_CKEN1             BIT(22)
#define USB3_VCC_SRST_REQ1            BIT(28)

#define USB3_PORT_DISABLE             BIT(5)

#define COMBPHY0_REF_CKEN             BIT(0)
#define COMBPHY0_SRST_REQ             BIT(4)
#define COMBPHY1_REF_CKEN             BIT(8)
#define COMBPHY1_SRST_REQ             BIT(12)

#define COMBPHY1_USB3_SEL0            BIT(12)
#define COMBPHY1_USB3_SEL1            BIT(11)

#define BIT_UTMI_8_16                 BIT(3)
#define BIT_UTMI_ULPI                 BIT(4)

#define PCS_SSP_SOFT_RESET            BIT(31)

#define USB3_DEEMPHASIS               BIT(1)
#define USB3_DEEMPHASIS_MASK          (0x6)

#define CLK_NAME(_id) (#_id)

extern struct clk_ops clk_ops_hiusb2_host0;
/******************************************************************************/

static int hiclk_enable_usb3_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	const char * clkname = CLK_NAME(PERI_CRG46_USB2CTRL);
	struct clk * usb2clk =__clk_lookup(clkname);

	/* init usb2 phy */
	clk_ops_hiusb2_host0.enable(usb2clk->hw);

	/* cancel 2p phy utmi reset1 */
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY01_SRST_TREQ1);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	mdelay(1);

	/* open combphy0 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= (COMBPHY0_REF_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(1);
		
	/* cancel combphy0 POR */
	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg &= ~(COMBPHY0_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	mdelay(5);

	/* open u3 ctrl0 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN
		| USB3_BUS_GS_CKEN
		| USB3_UTMI_CKEN
		| USB3_PIPE_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN
		| USB3_BUS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);

	/* cancel u3 ctrl0 reset */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);

	reg = readl(XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);
	reg |= PCS_SSP_SOFT_RESET;
	writel(reg, XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(XHCI_HOST0_ADDR + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, XHCI_HOST0_ADDR + REG_GUSB2PHYCFG0);
	mdelay(5);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(XHCI_HOST0_ADDR + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, XHCI_HOST0_ADDR + REG_GCTL);

	/* SSC on
	* before ec: default value is ssc on ,here no operation 
	* after ec: default value is ssc off , here turn of SSC
	*/
	if (combphy_fixed) {
		writel(0x00802000, clk->peri_ctrl_base + PERI_COMBPHY0);
		writel(0x01802000, clk->peri_ctrl_base + PERI_COMBPHY0);
		writel(0x00802000, clk->peri_ctrl_base + PERI_COMBPHY0);
		writel(0x00000000, clk->peri_ctrl_base + PERI_COMBPHY0);
		mdelay(1);
	}

	writel(0x00d10000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x01d10000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00d10000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00000000, clk->peri_ctrl_base +PERI_COMBPHY0);
	mdelay(1);

	writel(0x00d1b000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x01d1b000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00d1b000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00000000, clk->peri_ctrl_base +PERI_COMBPHY0);
	mdelay(1);

	writel(0x00202000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x01202000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00202000, clk->peri_ctrl_base +PERI_COMBPHY0);
	writel(0x00000000, clk->peri_ctrl_base +PERI_COMBPHY0);
	mdelay(1);	

	/* disable suspend and set -3.5db de-emphasis */
	reg = readl(XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);
	reg &= ~PCS_SSP_SOFT_RESET;
	reg &= ~(0x1<<17);
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS;
	writel(reg, XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);
	mdelay(5);

	writel(0x23100000, XHCI_HOST0_ADDR + GTXTHRCFG);
	writel(0x23100000, XHCI_HOST0_ADDR + GRXTHRCFG);
	mdelay(5);

	return 0;
}
/******************************************************************************/

static int hiclk_enable_usb3_host1(struct clk_hw *hw)
{
	u32 reg, mux=0;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	//TODO: make sure if combphy1 connected 

	if (mux) {
		/* select U3 COMBPHY1 */
		reg = readl(clk->peri_ctrl_base + PERI_CTRL);
		reg |= COMBPHY1_USB3_SEL1;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL);
		mdelay(1);
	} else {
		reg = readl(clk->peri_ctrl_base + PERI_USB7);
		reg |= USB3_PORT_DISABLE;
		writel(reg, clk->peri_ctrl_base + PERI_USB7);
	}

	/* cancel usb2 1p phy POR */
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY3_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	mdelay(1);

	/* config usb2 1p phy clk */
	writel(0xa60c, clk->peri_ctrl_base + PERI_USB2);
	writel(0xe60c, clk->peri_ctrl_base + PERI_USB2);
	writel(0xa60c, clk->peri_ctrl_base + PERI_USB2);
	mdelay(5);

	/* cancel usb2 1p phy utmi reset */
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY3_SRST_TREQ);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	mdelay(1);

	if (mux) {
		/* open combphy1 cken */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= (COMBPHY1_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		mdelay(1);

		/* cancel combphy1 POR */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg &= ~(COMBPHY1_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		mdelay(5);
	}

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN1
		| USB3_BUS_GS_CKEN1
		| USB3_UTMI_CKEN1
		| USB3_PIPE_CKEN1
		| USB3_SUSPEND_CKEN1
		| USB3_REF_CKEN1
		| USB3_BUS_CKEN1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);

	/* cancel u3 ctrl1 reset */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);

	reg = readl(XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);
	reg |= PCS_SSP_SOFT_RESET;
	writel(reg, XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(XHCI_HOST1_ADDR + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, XHCI_HOST1_ADDR + REG_GUSB2PHYCFG0);
	mdelay(5);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(XHCI_HOST1_ADDR + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, XHCI_HOST1_ADDR + REG_GCTL);

	if (mux) {
		/* SSC on
		* before ec: default value is ssc on ,here no operation 
		* after ec: default value is ssc off , here turn of SSC
		*/
		if (combphy_fixed) {
			writel(0x00802000, clk->peri_ctrl_base + PERI_COMBPHY1);
			writel(0x01802000, clk->peri_ctrl_base + PERI_COMBPHY1);
			writel(0x00802000, clk->peri_ctrl_base + PERI_COMBPHY1);
			writel(0x00000000, clk->peri_ctrl_base + PERI_COMBPHY1);
			mdelay(1);
		}

		writel(0x00d10000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x01d10000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00d10000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00000000, clk->peri_ctrl_base + PERI_COMBPHY1);
		mdelay(1);

		writel(0x00d1b000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x01d1b000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00d1b000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00000000, clk->peri_ctrl_base + PERI_COMBPHY1);
		mdelay(1);

		writel(0x00202000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x01202000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00202000, clk->peri_ctrl_base + PERI_COMBPHY1);
		writel(0x00000000, clk->peri_ctrl_base + PERI_COMBPHY1);
		mdelay(1);
	}

	/* disable suspend and set -3.5db de-emphasis */
	reg = readl(XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);
	reg &= ~PCS_SSP_SOFT_RESET;
	reg &= ~(0x1<<17);
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS;
	writel(reg, XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);
	mdelay(5);

	writel(0x23100000, XHCI_HOST1_ADDR + GTXTHRCFG);
	writel(0x23100000, XHCI_HOST1_ADDR + GRXTHRCFG);
	mdelay(5);

	return 0;
}
/******************************************************************************/

static void hiclk_disable_usb3_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	const char * clkname = CLK_NAME(PERI_CRG46_USB2CTRL);
	struct clk * usb2clk =__clk_lookup(clkname);

	clk_ops_hiusb2_host0.disable(usb2clk->hw);

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= USB3_VCC_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);

	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg |= USB2_PHY01_SRST_TREQ1;
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);

	reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
	reg |= COMBPHY0_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);

	mdelay(500);
}
/******************************************************************************/

static void hiclk_disable_usb3_host1(struct clk_hw *hw)
{
	u32 reg, mux=0;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	//TODO: make sure if combphy1 connected 

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= USB3_VCC_SRST_REQ1;
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);

	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg |= (USB2_PHY3_SRST_REQ
		 |USB2_PHY3_SRST_TREQ);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);

	if (mux) {
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= COMBPHY1_SRST_REQ;
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
	}
		
	mdelay(500);
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb3_host0 = {
	.enable = hiclk_enable_usb3_host0,
	.disable = hiclk_disable_usb3_host0,
};

struct clk_ops clk_ops_hiusb3_host1 = {
	.enable = hiclk_enable_usb3_host1,
	.disable = hiclk_disable_usb3_host1,
};
