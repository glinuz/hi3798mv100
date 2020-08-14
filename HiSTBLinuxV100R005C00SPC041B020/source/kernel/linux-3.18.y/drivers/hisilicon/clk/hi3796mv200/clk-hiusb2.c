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
 * Create By Liu Hui 2015.06.23
 *
******************************************************************************/

#include <linux/delay.h>
#include <dt-bindings/clock/hi3798mv200-clock.h>
#include <linux/hikapi.h>

#include "clk-hisi.h"

/******************************************************************************/
#define PERI_CTRL_USB3                  (0x12c)

#define USB2_2P_SS_WORD_IF_I            BIT(0)
#define USB2_2P_SS_ENA_INCR16_I         BIT(9)

#define TEST_WRDATA                     (0xc)
#define TEST_ADDR                       (0x6 << 8)
#define TEST_WREN                       BIT(13)
#define TEST_CLK                        BIT(14)
#define TEST_RSTN                       BIT(15)

#define USB2_BUS_CKEN                   BIT(0)
#define USB2_OHCI48M_CKEN               BIT(1)
#define USB2_OHCI12M_CKEN               BIT(2)
#define USB2_OTG_UTMI_CKEN              BIT(3)
#define USB2_HST_PHY_CKEN               BIT(4)
#define USB2_UTMI0_CKEN                 BIT(5)
#define USB2_UTMI1_CKEN                 BIT(6)
#define USB2_PHY0_SRST_TREQ             BIT(8)
#define USB2_PHY1_SRST_TREQ             BIT(9)
#define USB2_BUS_SRST_REQ               BIT(12)
#define USB2_UTMI0_SRST_REQ             BIT(13)
#define USB2_UTMI1_SRST_REQ             BIT(14)
#define USB2_HST_PHY_SYST_REQ           BIT(16)
#define USB2_OTG_PHY_SYST_REQ           BIT(17)
#define USB2_CLK48_SEL                  BIT(20)

#define USB2_PHY01_REF_CKEN             BIT(0)
#define USB2_PHY2_REF_CKEN              BIT(2)
#define USB2_PHY01_SRST_REQ             BIT(4)
#define USB2_PHY2_SRST_REQ              BIT(6)
#define USB2_PHY0_SRST_TREQ             BIT(8)
#define USB2_PHY1_SRST_TREQ             BIT(9)
#define USB2_PHY2_SRST_TREQ             BIT(10)
#define USB2_PHY01_REFCLK_SEL           BIT(12)
#define USB2_PHY2_REFCLK_SEL            BIT(14)
#define USB2_PHY2_TEST_SRST_REQ         BIT(15)
#define USB2_PHY01_TEST_SRST_REQ        BIT(16)

#define USB2_PHY01_REGBASE              0xf9865000

/******************************************************************************/

static atomic_t dev_open_cnt = {
	.counter = 0,
};
/******************************************************************************/

static void inno_phy_config_2p_1(struct hiclk_hw *clk)
{
	void __iomem *regbase = ioremap_nocache(USB2_PHY01_REGBASE, 0x1000);
	if (!regbase) {
		printk("ioremap ehci regbase failed.\n");
		return;
	}

	writel(0x4, regbase + 0x18);
	mdelay(2);

	iounmap(regbase);
	regbase = NULL;
}
/******************************************************************************/

static int hiclk_enable_usb2(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_add_return(1, &dev_open_cnt) == 1) {

		/* reset controller bus/utmi/roothub  */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		/* reset phy por/utmi */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY0_SRST_TREQ
			| USB2_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

#if defined(CONFIG_HI3798MV2X_FPGA) || defined(CONFIG_HI3796MV2X_FPGA)
		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB3);
		reg |= USB2_2P_SS_WORD_IF_I;  
		reg &= ~(USB2_2P_SS_ENA_INCR16_I);
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB3);
		udelay(100);
#else
		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB3);
		reg &= ~(USB2_2P_SS_WORD_IF_I);
		reg &= ~(USB2_2P_SS_ENA_INCR16_I);
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB3);
		udelay(100);
#endif
		/* open ref clk */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY01_TEST_SRST_REQ | USB2_PHY2_TEST_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY01_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(500);

		inno_phy_config_2p_1(clk);

		/* cancel port reset 
		* delay 2ms for waiting comp circuit stable
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY0_SRST_TREQ | USB2_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		mdelay(2);
		/* open controller clk */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
		
		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &= ~(USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
	}

	return 0;
}
/******************************************************************************/

static void hiclk_disable_usb2(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_sub_return(1, &dev_open_cnt) == 0) {

		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
		printk("%s->%d: CRG47=0x%x\n", __func__, __LINE__, readl(clk->peri_crg_base + PERI_CRG47_USB2PHY));

		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_SRST_REQ
			| USB2_PHY0_SRST_TREQ
			| USB2_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(100);
		printk("%s->%d: CRG47=0x%x\n", __func__, __LINE__, readl(clk->peri_crg_base + PERI_CRG47_USB2PHY));
	}
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb2 = {
	.enable = hiclk_enable_usb2,
	.disable = hiclk_disable_usb2,
};
