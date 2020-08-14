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
#include <dt-bindings/clock/hi3798mv100-clock.h>
#include <linux/hikapi.h>
#include <mach/hardware.h>

#include "clk-hisi.h"

/******************************************************************************/
#define XHCI_HOST_ADDR               __io_address(0xf98a0000)
#define PERI_CTRL                   (0x0008)
#define PERI_CTRL_USB6              (0x0138)

#define GTXTHRCFG                   (0xc108)
#define GRXTHRCFG                   (0xc10c)
#define REG_GCTL                    (0xc110)
#define REG_GUSB2PHYCFG0            (0xC200)
#define REG_GUSB3PIPECTL0           (0xc2c0)

#define USB3_IP_ISO_CTRL            BIT(26)

#define USB3_VCC_SRST_REQ           BIT(12)
#define USB3_UTMI_CKEN              BIT(4)
#define USB3_SUSPEND_CKEN           BIT(2)
#define USB3_REF_CKEN               BIT(1)
#define USB3_BUS_CKEN               BIT(0)

#define USB3_PHY_SRST_REQ           BIT(4)
#define USB3_PHY_SRST_TREQ          BIT(5)

#define BIT_UTMI_8_16               BIT(3)
#define BIT_UTMI_ULPI               BIT(4)

#define PCS_SSP_SOFT_RESET          BIT(31)
/******************************************************************************/

static int hiclk_enable_usb3(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	/* use synop phy */
	reg = readl(clk->peri_ctrl_base + PERI_CTRL);
	reg &= ~(USB3_IP_ISO_CTRL);
	writel(reg, clk->peri_ctrl_base + PERI_CTRL);

	/* de-assert usb3_vcc_srst_req */
	writel(0x431, clk->peri_crg_base + PERI_CRG101_USB3PHY);
	writel(0x13ff, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);
	
	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	mdelay(5);

	reg = readl(XHCI_HOST_ADDR + REG_GUSB3PIPECTL0);
	reg |= PCS_SSP_SOFT_RESET;
	writel(reg, XHCI_HOST_ADDR + REG_GUSB3PIPECTL0);

	/*USB2 PHY chose ulpi 8bit interface */
	reg = readl(XHCI_HOST_ADDR + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, XHCI_HOST_ADDR + REG_GUSB2PHYCFG0);
	mdelay(1);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(XHCI_HOST_ADDR + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12); 
	writel(reg, XHCI_HOST_ADDR + REG_GCTL);

	/* de-assert usb3phy hard-macro por */
	reg = readl(clk->peri_crg_base + PERI_CRG101_USB3PHY);
	reg &= ~USB3_PHY_SRST_REQ;
	writel(reg, clk->peri_crg_base + PERI_CRG101_USB3PHY);
	mdelay(5);

	/* de-assert hsp hard-macro port reset */
	reg = readl(clk->peri_crg_base + PERI_CRG101_USB3PHY);
	reg &= ~USB3_PHY_SRST_TREQ; 
	writel(reg, clk->peri_crg_base + PERI_CRG101_USB3PHY);

 	/* disable suspend */
	reg = readl(XHCI_HOST_ADDR + REG_GUSB3PIPECTL0);
	reg &= ~(1<<17);      
	reg &= ~PCS_SSP_SOFT_RESET;
	writel(reg, XHCI_HOST_ADDR + REG_GUSB3PIPECTL0);
	mdelay(5);

	writel(0x23100000, XHCI_HOST_ADDR + GTXTHRCFG);
	writel(0x23100000, XHCI_HOST_ADDR + GRXTHRCFG);
	mdelay(1);

	/* USB3.0 eye config */
	writel(0x7381560d, clk->peri_ctrl_base + PERI_CTRL_USB6);

	return 0;
}
/******************************************************************************/

static void hiclk_disable_usb3(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	reg = readl(clk->peri_crg_base + PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_UTMI_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN
		| USB3_BUS_CKEN);
	writel(reg, clk->peri_crg_base + PERI_CRG44_USB3CTRL);

	mdelay(500);
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb3 = {
	.enable = hiclk_enable_usb3,
	.disable = hiclk_disable_usb3,
};
