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

#include "clk-hisi.h"

/******************************************************************************/
#define PERI_CTRL_USB0                  (0x120)
#define PERI_CTRL_USB1                  (0x124)
#define PERI_CTRL_USB3                  (0x12c)
#define PERI_CTRL_USB4                  (0x130)

#define WORDINTERFACE                   BIT(0)
#define ULPI_BYPASS_EN_PORT0            BIT(3)
#define SS_BURST16_EN                   BIT(9)

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
#define USB2_BUS_SRST_REQ               BIT(12)
#define USB2_UTMI0_SRST_REQ             BIT(13)
#define USB2_HST_PHY_SYST_REQ           BIT(16)
#define USB2_OTG_PHY_SYST_REQ           BIT(17)
#define USB2_CLK48_SEL                  BIT(20)

#define USB2_BUS_CKEN1                  BIT(0)
#define USB2_OHCI48M_CKEN1              BIT(1)
#define USB2_OHCI12M_CKEN1              BIT(2)
#define USB2_HST_PHY_CKEN1              BIT(4)
#define USB2_UTMI0_CKEN1                BIT(5)
#define USB2_BUS_SRST_REQ1              BIT(12)
#define USB2_UTMI0_SRST_REQ1            BIT(14)
#define USB2_HST_PHY_SYST_REQ1          BIT(16)
#define USB2_CLK48_SEL1                 BIT(20)

#define USB2_PHY01_REF_CKEN             BIT(0)
#define USB2_PHY2_REF_CKEN              BIT(2)
#define USB2_PHY01_SRST_REQ             BIT(4)
#define USB2_PHY2_SRST_REQ              BIT(6)
#define USB2_PHY01_SRST_TREQ0           BIT(8)
#define USB2_PHY2_SRST_TREQ             BIT(10)
#define USB2_PHY01_REFCLK_SEL           BIT(12)
#define USB2_PHY2_REFCLK_SEL            BIT(14)

/******************************************************************************/

static atomic_t dev_open_cnt0 = {
	.counter = 0,
};

static atomic_t dev_open_cnt1 = {
	.counter = 0,
};
/******************************************************************************/

static int hiclk_enable_usb2_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_add_return(1, &dev_open_cnt0) == 1) {

		/* reset controller bus/utmi/roothub  */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		/* reset phy por/utmi */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_SRST_REQ
			| USB2_PHY01_SRST_TREQ0);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB3);
		reg |= ULPI_BYPASS_EN_PORT0;  
		reg &= ~(WORDINTERFACE);   
		reg &= ~(SS_BURST16_EN);
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB3);
		udelay(100);

		/* open ref clk */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY01_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(500);

		/* config clk output */    
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
		mdelay(10);

		/* 2port PHY HS pre-emphasize 000 */
		writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xe018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xb018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xf018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xb018, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		/* 2port PHY squelch trigger point 175mV */
		writel(0xa16e, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xe16e, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xa16e,clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xb16e, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xf16e, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xb16e, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		/* 2port PHY Rcomp=187mV Vref=425mV */
		writel(0xa5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xe5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xa5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xb5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xf5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xb5b2, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xa703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xe703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xa703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xb703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xf703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xb703, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		/* 2port PHY Icomp=187mV */
		writel(0xaa3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xea3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xaa3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);

		writel(0xba3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xfa3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(10);
		writel(0xba3b, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(50);
			
		/* cancel port reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY01_SRST_TREQ0);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* open controller clk */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
		
		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &= ~(USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
	}

	return 0;
}
/******************************************************************************/

static int hiclk_enable_usb2_host1(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_add_return(1, &dev_open_cnt1) == 1) {
		/* reset controller bus/utmi/roothub */
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg |= (USB2_BUS_SRST_REQ1
			| USB2_UTMI0_SRST_REQ1
			| USB2_HST_PHY_SYST_REQ1);
		writel(reg, clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		udelay(200);

		/* reset phy por/utmi */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY2_SRST_REQ
			| USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

		/* config utmi interface */
		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB4);
		reg |= ULPI_BYPASS_EN_PORT0;
		reg &= ~(WORDINTERFACE);
		reg &= ~(SS_BURST16_EN);
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB4);
		udelay(100);

		/* open ref clk */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY2_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY2_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(500);

		/* config clk output */
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		mdelay(10);

		/* 1port PHY HS pre-emphasize 000 */
		writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xe018, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xa018, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(50);

		/* 1port PHY squelch trigger point 175mV */
		writel(0xa16e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xe16e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xa16e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(50);
			
		/* 1port PHY Rcomp=200mV Vref=425mV */
		writel(0xa532, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xe532, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xa532, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(50);
			
		writel(0xa703, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xe703, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xa703, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(50);

		/* 1port PHY Icomp=212mV */
		writel(0xaabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xeabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(10);
		writel(0xaabb, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(50);

		/* cancel phy utmi port reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* open control clk  */
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg |= (USB2_BUS_CKEN1
			| USB2_OHCI48M_CKEN1
			| USB2_OHCI12M_CKEN1
			| USB2_HST_PHY_CKEN1
			| USB2_UTMI0_CKEN1);
		writel(reg, clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		udelay(200);
			
		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg &= ~(USB2_BUS_SRST_REQ1
			| USB2_UTMI0_SRST_REQ1
			| USB2_HST_PHY_SYST_REQ1);
		writel(reg, clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		udelay(200);
	}

	return 0;
}
/******************************************************************************/

static void hiclk_disable_usb2_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_sub_return(1, &dev_open_cnt0) == 0) {

		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_HST_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_SRST_REQ
			| USB2_PHY01_SRST_TREQ0);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(100);
	}
}
/******************************************************************************/

static void hiclk_disable_usb2_host1(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_sub_return(1, &dev_open_cnt1) == 0) {
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg |= (USB2_BUS_SRST_REQ1
			| USB2_UTMI0_SRST_REQ1
			| USB2_HST_PHY_SYST_REQ1);
		writel(reg, clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		udelay(200);

		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY2_SRST_REQ
			| USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(100);
	}
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb2_host0 = {
	.enable = hiclk_enable_usb2_host0,
	.disable = hiclk_disable_usb2_host0,
};

struct clk_ops clk_ops_hiusb2_host1 = {
	.enable = hiclk_enable_usb2_host1,
	.disable = hiclk_disable_usb2_host1,
};
