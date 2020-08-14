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
#include <dt-bindings/clock/hi3798cv200-clock.h>
#include <linux/hikapi.h>
#include "clk-hisi.h"

extern int combphy_fixed;

static void __iomem *xhci_host0_regbase;
static void __iomem *xhci_host1_regbase;

/******************************************************************************/
#define XHCI_HOST0_ADDR             (0xF98A0000)
#define XHCI_HOST1_ADDR             (0xF98B0000)

#define PERI_CTRL                   (0x0008)
#define PERI_USB0                   (0x120)
#define PERI_USB1                   (0x124)
#define PERI_USB2                   (0x128)
#define PERI_USB5                   (0x134)
#define PERI_USB7                   (0x13c)
#define PERI_COMBPHY0               (0x850)
#define PERI_COMBPHY1               (0x858)

#define GTXTHRCFG                   (0xc108)
#define GRXTHRCFG                   (0xc10c)
#define REG_GCTL                    (0xc110)
#define REG_GUSB2PHYCFG0            (0xC200)
#define REG_GUSB3PIPECTL0           (0xc2c0)

#define USB2_PHY2_SRST_REQ            BIT(6)
#define USB2_PHY3_SRST_REQ            BIT(7)
#define USB2_PHY01_SRST_TREQ0         BIT(8)
#define USB2_PHY01_SRST_TREQ1         BIT(9)
#define USB2_PHY2_SRST_TREQ           BIT(10)
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
#define COMBPHY0_REF_SEL0             BIT(2)
#define COMBPHY0_REF_SEL1             BIT(3)
#define COMBPHY0_SRST_REQ             BIT(4)
#define COMBPHY1_REF_CKEN             BIT(8)
#define COMBPHY1_REF_SEL0             BIT(10)
#define COMBPHY1_REF_SEL1             BIT(11)
#define COMBPHY1_SRST_REQ             BIT(12)

#define COMBPHY1_SEL_MASK             (BIT(11) | BIT(12))
#define COMBPHY1_SEL_USB3              BIT(11)

#define BIT_UTMI_8_16                 BIT(3)
#define BIT_UTMI_ULPI                 BIT(4)

#define PCS_SSP_SOFT_RESET            BIT(31)

#define USB3_DEEMPHASIS0              BIT(1)
#define USB3_DEEMPHASIS1              BIT(2)
#define USB3_TX_MARGIN0               BIT(3)
#define USB3_TX_MARGIN1               BIT(4)
#define USB3_TX_MARGIN2               BIT(5)
#define USB3_SUSPEND_EN               BIT(17)
#define USB3_DEEMPHASIS_MASK          (0x6)

#define CLK_NAME(_id) (#_id)

extern struct clk_ops clk_ops_hiusb2;

extern char no_usb3_0;
extern char no_usb3_1;
/******************************************************************************/
static void nano_phy_config_0(struct hiclk_hw *clk)
{
	u32 reg;
	/* write 0x010c0012 to GUSB3PIPECTL0
	* GUSB3PIPECTL0[17] = 0 : disable U3 suspend 
	* GUSB3PIPECTL0[5:3] = 010 : Tx Margin = 900mV , decrease TX voltage
	* GUSB3PIPECTL0[2:1] = 01 : Tx Deemphasis = -3.5dB, refer to spec
	*/
	reg = readl(xhci_host0_regbase + REG_GUSB3PIPECTL0);
	//reg &= ~USB3_SUSPEND_EN;
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS0;
	reg |= USB3_TX_MARGIN1;
	writel(reg, xhci_host0_regbase + REG_GUSB3PIPECTL0);

	udelay(20);
}

static void nano_phy_config_1(struct hiclk_hw *clk)
{
	u32 reg;
	/* write 0x010c0012 to GUSB3PIPECTL0
	* GUSB3PIPECTL0[17] = 0 : disable U3 suspend 
	* GUSB3PIPECTL0[5:3] = 010 : Tx Margin = 900mV , decrease TX voltage
	* GUSB3PIPECTL0[2:1] = 01 : Tx Deemphasis = -3.5dB, refer to spec
	*/
	reg = readl(xhci_host1_regbase + REG_GUSB3PIPECTL0);
	//reg &= ~USB3_SUSPEND_EN;
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS0;
	reg |= USB3_TX_MARGIN1;
	writel(reg, xhci_host1_regbase + REG_GUSB3PIPECTL0);

	udelay(20);
}

static void inno_phy_config_2p_0(struct hiclk_hw *clk)
{
	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa0001c, clk->peri_ctrl_base + PERI_USB0);
	writel(0xe0001c, clk->peri_ctrl_base + PERI_USB0);
	writel(0xa0001c, clk->peri_ctrl_base + PERI_USB0);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa00700, clk->peri_ctrl_base + PERI_USB0);
	writel(0xe00700, clk->peri_ctrl_base + PERI_USB0);
	writel(0xa00700, clk->peri_ctrl_base + PERI_USB0);
	udelay(20);
	
	/* write 0x40 to addr 0x11  
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will 
	* not stay at 1 when error packets received
	*/
	writel(0xa01140, clk->peri_ctrl_base + PERI_USB0);
	writel(0xe01140, clk->peri_ctrl_base + PERI_USB0);
	writel(0xa01140, clk->peri_ctrl_base + PERI_USB0);
	udelay(20);
}
/******************************************************************************/

static void inno_phy_config_1p(struct hiclk_hw *clk)
{
	/* write 0x4 to addr 0x06
	* 0x06[2] = 1 : config 1P PHY clk output 
	* delay 1ms for waiting PLL stable
	*/
	writel(0xa00604, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe00604, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00604, clk->peri_ctrl_base + PERI_USB1);
	mdelay(1);

	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa0001c, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe0001c, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa0001c, clk->peri_ctrl_base + PERI_USB1);
	udelay(20);
	
	/* write 0x92 to 0x05 
	* write 0x06 to 0x06
	* {0x06[1:0],0x05[7]} = 101 : Rcomp = 141mV , increase DC level
	*/
	writel(0xa00592, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe00592, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00592, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00606, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe00606, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00606, clk->peri_ctrl_base + PERI_USB1);
	udelay(20);

	/* write 0xab to 0x0a
	* 0x0a[7:5] = 101 : Icomp = 212mV , increase current drive
	*/
	writel(0xa00aab, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe00aab, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00aab, clk->peri_ctrl_base + PERI_USB1);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa00700, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe00700, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa00700, clk->peri_ctrl_base + PERI_USB1);
	udelay(20);

	/* write 0x40 to addr 0x11  
	* 0x11[7] = 1  : Comp Mode Select
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will 
	* not stay at 1 when error packets received
	*/
	writel(0xa011c0, clk->peri_ctrl_base + PERI_USB1);
	writel(0xe011c0, clk->peri_ctrl_base + PERI_USB1);
	writel(0xa011c0, clk->peri_ctrl_base + PERI_USB1);
	udelay(20);
}
/******************************************************************************/

static int hiclk_enable_usb3_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	const char * clkname = CLK_NAME(PERI_CRG46_USB2CTRL);
	struct clk * usb2clk =__clk_lookup(clkname);

	if (!xhci_host0_regbase) {
		xhci_host0_regbase = ioremap_nocache(XHCI_HOST0_ADDR, 0x10000);
		if (!xhci_host0_regbase) {
			printk("ioremap xhci host0 failed.\n");
			return -1;
		}
	}

	if (no_usb3_0) {
		reg = readl(clk->peri_ctrl_base + PERI_USB5);
		reg |= USB3_PORT_DISABLE;
		writel(reg, clk->peri_ctrl_base + PERI_USB5);
	}

	/* init usb2 phy */
	clk_ops_hiusb2.enable(usb2clk->hw);

	inno_phy_config_2p_0(clk);

	/* cancel 2p phy utmi reset0 
	* delay 2ms for waiting comp circuit stable
	*/
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY01_SRST_TREQ0);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	mdelay(2);

	if (!no_usb3_0) {
		/* select VPLL 25MHz ref clock for COMB0
		* BPLL may be SSC in Hi3798CV200 
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= (COMBPHY0_REF_SEL1);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		udelay(100);

		/* open combphy0 cken */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= (COMBPHY0_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		udelay(200);

		/* cancel combphy0 POR */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg &= ~(COMBPHY0_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		mdelay(1);
	}

	/* open u3 ctrl0 bus cken */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN
		| USB3_BUS_GS_CKEN
		| USB3_BUS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	udelay(100);

	/* cancel u3 ctrl0 reset */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	udelay(200);

	//reg = readl(XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);
	//reg |= PCS_SSP_SOFT_RESET;
	//writel(reg, XHCI_HOST0_ADDR + REG_GUSB3PIPECTL0);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(xhci_host0_regbase + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, xhci_host0_regbase + REG_GUSB2PHYCFG0);
	
	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(xhci_host0_regbase + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, xhci_host0_regbase + REG_GCTL);

	if (!no_usb3_0) {
		nano_phy_config_0(clk);
	}

	/* open USB3 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_UTMI_CKEN
		| USB3_PIPE_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(1);

	writel(0x23100000, xhci_host0_regbase + GTXTHRCFG);
	writel(0x23100000, xhci_host0_regbase + GRXTHRCFG);

	udelay(200);

	if (xhci_host0_regbase) {
		iounmap(xhci_host0_regbase);
		xhci_host0_regbase = NULL;
	}

	return 0;
}
/******************************************************************************/

static int hiclk_enable_usb3_host1(struct clk_hw *hw)
{
	u32 reg, mux=0;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (!xhci_host1_regbase) {
		xhci_host1_regbase = ioremap_nocache(XHCI_HOST1_ADDR, 0x10000);
		if (!xhci_host1_regbase) {
			printk("ioremap xhci host1 failed.\n");
			return -1;
		}
	}

	//TODO: make sure if combphy1 connected
	mux = readl(clk->peri_ctrl_base + PERI_CTRL);
	mux &= COMBPHY1_SEL_MASK;

	if (no_usb3_1)
		mux &= ~COMBPHY1_SEL_MASK;

	if (mux != COMBPHY1_SEL_USB3) {
		reg = readl(clk->peri_ctrl_base + PERI_USB7);
		reg |= USB3_PORT_DISABLE;
		writel(reg, clk->peri_ctrl_base + PERI_USB7);
	}

	/* cancel usb2 1p phy POR */
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY2_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	udelay(200);

	inno_phy_config_1p(clk);

	/* cancel usb2 1p phy utmi reset 
	* delay 2ms for waiting comp circuit stable
	*/
	reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY2_SRST_TREQ);
	writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
	mdelay(2);

	if (mux == COMBPHY1_SEL_USB3) {
		/* select VPLL 25MHz ref clock for COMB1
		* BPLL may be SSC in Hi3798CV200 
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= (COMBPHY1_REF_SEL1);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		udelay(100);

		/* open combphy1 cken */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg |= (COMBPHY1_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		udelay(200);

		/* cancel combphy1 POR */
		reg = readl(clk->peri_crg_base + PERI_CRG98_COMBPHY);
		reg &= ~(COMBPHY1_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG98_COMBPHY);
		mdelay(1);
	}

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN1
		| USB3_BUS_GS_CKEN1
		| USB3_BUS_CKEN1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	udelay(100);

	/* cancel u3 ctrl1 reset */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	udelay(200);

	//reg = readl(XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);
	//reg |= PCS_SSP_SOFT_RESET;
	//writel(reg, XHCI_HOST1_ADDR + REG_GUSB3PIPECTL0);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(xhci_host1_regbase + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, xhci_host1_regbase + REG_GUSB2PHYCFG0);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(xhci_host1_regbase + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, xhci_host1_regbase + REG_GCTL);

	if (mux == COMBPHY1_SEL_USB3) {
		nano_phy_config_1(clk);
	}

	/* open USB3 cken */
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg |= (USB3_UTMI_CKEN1
		| USB3_PIPE_CKEN1
		| USB3_SUSPEND_CKEN1
		| USB3_REF_CKEN1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(1);

	writel(0x23100000, xhci_host1_regbase + GTXTHRCFG);
	writel(0x23100000, xhci_host1_regbase + GRXTHRCFG);

	udelay(200);

	if (xhci_host1_regbase) {
		iounmap(xhci_host1_regbase);
		xhci_host1_regbase = NULL;
	}

	return 0;
}
/******************************************************************************/

static void hiclk_disable_usb3_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);
	const char * clkname = CLK_NAME(PERI_CRG46_USB2CTRL);
	struct clk * usb2clk =__clk_lookup(clkname);

	clk_ops_hiusb2.disable(usb2clk->hw);

	mdelay(50);

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_BUS_CKEN
		| USB3_UTMI_CKEN	
		| USB3_PIPE_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);

	mdelay(1);

}
/******************************************************************************/

static void hiclk_disable_usb3_host1(struct clk_hw *hw)
{
	u32 reg, mux=1;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_BUS_CKEN1
		| USB3_UTMI_CKEN1
		| USB3_PIPE_CKEN1
		| USB3_SUSPEND_CKEN1
		| USB3_REF_CKEN1);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);

	mdelay(1);

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
