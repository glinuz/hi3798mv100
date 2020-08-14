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
#include <dt-bindings/clock/hi3716cv200-clock.h>
#include <linux/hikapi.h>

#include "clk-hisi.h"

/******************************************************************************/
#define PERI_CTRL_USB0                  (0x120)
#define PERI_CTRL_USB1                  (0x124)
#define PERI_CTRL_USB2                  (0x128)
#define PERI_CTRL_USB4                  (0x130)

#define USB2_BUS_CKEN                   BIT(0)
#define USB2_OHCI48M_CKEN               BIT(1)
#define USB2_OHCI12M_CKEN               BIT(2)
#define USB2_OTG_UTMI_CKEN              BIT(3)
#define USB2_HST_PHY_CKEN               BIT(4)
#define USB2_UTMI0_CKEN                 BIT(5)
#define USB2_UTMI1_CKEN                 BIT(6)
#define USB2_UTMI2_CKEN                 BIT(7)
#define USB2_ADP_CKEN                   BIT(8)
#define USB2_BUS_SRST_REQ               BIT(12)
#define USB2_UTMI0_SRST_REQ             BIT(13)
#define USB2_UTMI1_SRST_REQ             BIT(14)
#define USB2_UTMI2_SRST_REQ             BIT(15)
#define USB2_HST_PHY_SYST_REQ           BIT(16)
#define USB2_OTG_PHY_SRST_REQ           BIT(17)
#define USB2_ADP_SRST_REQ               BIT(18)
#define USB2_CLK48_SEL                  BIT(20)

#define USB_PHY0_REF_CKEN               BIT(0)
#define USB_PHY1_REF_CKEN               BIT(1)
#define USB_PHY2_REF_CKEN               BIT(2)
#define USB_PHY0_SRST_REQ               BIT(8)
#define USB_PHY0_SRST_TREQ              BIT(9)
#define USB_PHY1_SRST_REQ               BIT(10)
#define USB_PHY1_SRST_TREQ              BIT(11)
#define USB_PHY2_SRST_REQ               BIT(12)
#define USB_PHY2_SRST_TREQ              BIT(13)
#define USB_PHY0_REFCLK_SEL             BIT(16)
#define USB_PHY1_REFCLK_SEL             BIT(17)
#define USB_PHY2_REFCLK_SEL             BIT(18)

#define WORDINTERFACE                   BIT(0)
#define ULPI_BYPASS_EN_PORT0            BIT(3)
#define ULPI_BYPASS_EN_PORT1_2          BIT(4)
#define SS_BURST4_EN                    BIT(7)
#define SS_BURST8_EN                    BIT(8)
#define SS_BURST16_EN                   BIT(9)
#define DWC_OTG_EN                      BIT(28)

#define PHY_TXPREEMPAMPTUNE_MASK        ~(0x3 << 27)
#define PHY_TXPREEMPAMPTUNE_VALUE       (0x3 << 27)
#define PHY_TXVREFTUNE_MASK             ~(0xF << 0)
#define PHY_TXVREFTUNE_VALUE            (0x8 << 0)
#define PHY_SIDDQ_MASK                  ~(0x1 << 22)
#define PHY_SIDDQ_VALUE                 BIT(22)
/******************************************************************************/

static atomic_t dev_open_cnt = {
	.counter = 0,
};
/******************************************************************************/

static int hiclk_enable_usb2(struct clk_hw *hw)
{
	u32 reg;
	u64 chipid = get_chipid(0ULL);
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_add_return(1, &dev_open_cnt) == 1) {
		/* power down phy for eye diagram */
		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg |= PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB2);
		reg |= PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB2);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB4);
		reg |= PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB4);
		udelay(2000);

		/* reset enable */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_UTMI2_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);
		if ((chipid == _HI3716MV410) || (chipid == _HI3716MV420)) {
			reg |= USB2_CLK48_SEL;
		}
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB0);
		reg |= ULPI_BYPASS_EN_PORT0;
		reg &= ~(WORDINTERFACE);
		reg &= ~(SS_BURST16_EN);
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);

		udelay(100);

		/* open ref clk */
		if ((chipid == _HI3716MV410) || (chipid == _HI3716MV420)) {
			reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
			reg |= (USB_PHY0_REF_CKEN
				| USB_PHY1_REF_CKEN
				| USB_PHY2_REF_CKEN);
			writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
			udelay(300);
		}

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* config PHY clock */
		if ((chipid == _HI3716MV410) || (chipid == _HI3716MV420)) {
			writel(0xA604, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xE604, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xA604, clk->peri_ctrl_base + PERI_CTRL_USB1);
			mdelay(10);

			/* Icomp=212.5mV */
			writel(0xaabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xeabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xaabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
			udelay(1000);
			/* Rcomp=212.5mV */
			writel(0xa606, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xe606, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xa606, clk->peri_ctrl_base + PERI_CTRL_USB1);
			udelay(1000);
			writel(0xa592, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xe592, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xa592, clk->peri_ctrl_base + PERI_CTRL_USB1);
			/* EOP pre-emphasis off */
			writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xe018, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB1);
			udelay(1000);
			writel(0xb01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xf01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xb01c, clk->peri_ctrl_base + PERI_CTRL_USB1);

			writel(0xb904, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xf904, clk->peri_ctrl_base + PERI_CTRL_USB1);
			writel(0xb904, clk->peri_ctrl_base + PERI_CTRL_USB1);
		}

		/* cancel port reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &=~(USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		reg |= (USB_PHY0_REF_CKEN
			| USB_PHY1_REF_CKEN
			| USB_PHY2_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		mdelay(10);

		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &=~(USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_UTMI2_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);
		reg |= (USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN
			| USB2_UTMI2_CKEN
			| USB2_ADP_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg &= PHY_SIDDQ_MASK;
		reg &= ~PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB2);
		reg &= PHY_SIDDQ_MASK;
		reg &= ~PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB2);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB4);
		reg &= PHY_SIDDQ_MASK;
		reg &= ~PHY_SIDDQ_VALUE;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB4);
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
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |=(USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base +PERI_CRG47_USB2PHY);
		udelay(100);

		/* close clock */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &=~ (USB_PHY0_REFCLK_SEL
			| USB_PHY1_REFCLK_SEL
			| USB_PHY2_REFCLK_SEL
			| USB_PHY0_REF_CKEN
			| USB_PHY1_REF_CKEN
			| USB_PHY2_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* close clock  */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &=~(USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN
			| USB2_UTMI2_CKEN
			| USB2_ADP_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
	}
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb2 = {
	.enable = hiclk_enable_usb2,
	.disable = hiclk_disable_usb2,
};
