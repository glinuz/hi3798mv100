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

#include "clk-hisi.h"

/******************************************************************************/
#define PERI_CTRL_USB0                  (0x120)
#define PERI_CTRL_USB1                  (0x124)
#define PERI_CTRL_USB13                 (0x154)
#define PERI_CTRL_USB14                 (0x158)

#define USB2_BUS_CKEN                   BIT(0)
#define USB2_OHCI48M_CKEN               BIT(1)
#define USB2_OHCI12M_CKEN               BIT(2)
#define USB2_OTG_UTMI_CKEN              BIT(3)
#define USB2_HST_PHY_CKEN               BIT(4)
#define USB2_UTMI0_CKEN                 BIT(5)
#define USB2_UTMI1_CKEN                 BIT(6)
#define USB2_BUS_SRST_REQ               BIT(12)
#define USB2_UTMI0_SRST_REQ             BIT(13)
#define USB2_UTMI1_SRST_REQ             BIT(14)
#define USB2_HST_PHY_SYST_REQ           BIT(16)
#define USB2_OTG_PHY_SRST_REQ           BIT(17)

#define USB_PHY0_REF_CKEN               BIT(0)
#define USB_PHY1_REF_CKEN               BIT(1)
#define USB_PHY0_SRST_REQ               BIT(8)
#define USB_PHY0_SRST_TREQ              BIT(9)
#define USB_PHY1_SRST_REQ               BIT(10)
#define USB_PHY1_SRST_TREQ              BIT(11)

#define WORDINTERFACE                   BIT(0)
#define ULPI_BYPASS_EN_PORT0            BIT(3)
#define SS_BURST16_EN                   BIT(9)

#define USB2_PHY2_REF_CKEN              BIT(0)
#define USB2_PHY2_SRST_REQ              BIT(8)
#define USB2_PHY2_SRST_TREQ             BIT(9)

#define USB2_BUS_CKEN1                  BIT(0)
#define USB2_OHCI48M_CKEN1              BIT(1)
#define USB2_OHCI12M_CKEN1              BIT(2)
#define USB2_HST_PHY_CKEN1              BIT(4)
#define USB2_UTMI0_CKEN1                BIT(5)
#define USB2_BUS_SRST_REQ1              BIT(12)
#define USB2_UTMI0_SRST_REQ1            BIT(14)
#define USB2_HST_PHY_SYST_REQ1          BIT(16)

#define TEST_ADDR                       (0x6 << 8)
#define TEST_WREN                       BIT(13)
#define TEST_CLK                        BIT(14)
#define TEST_RSTN                       BIT(15)
/******************************************************************************/

static atomic_t dev_open_cnt0 = {
	.counter = 0,
};

static atomic_t dev_open_cnt1 = {
	.counter = 0,
};
/******************************************************************************/

static void inno_phy_config_2p(struct hiclk_hw *clk)
{
	u32 reg;
	u64 chipid = get_chipid(0ULL);
	u32 wrdata = 0;

	if ((chipid & 0xf0) == 0x10) {
		mdelay(2);

		/* PLL core clock 480M */
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(300);

		wrdata = 0x4;
		/* config 2P PHY clk output 
		* delay 1ms for waiting PLL stable
		*/
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB1);
		mdelay(1);

		/* write 0x1c to addr 0x00  
		* 0x00[0] = 0 : close EOP pre-emphasis
		* 0x00[2] = 1 : open Data pre-emphasis
		*/
		writel(0xa01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xb01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xf01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xb01c, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(100);
	} else {
		/* avoid inno phy error */
		/* 1. address 0x07, send data 0x3b */
		writel(0xa73b, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe73b, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa73b, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(500);
		/*
		 * >=1us later
		 * 2. address 0x0b, send data 0xc0
		 */
		writel(0xabc0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xebc0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xabc0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(500);
		/*
		 * >=50us later
		 * 3. address 0x07, send data 0x3f
		 */
		writel(0xa73f, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe73f, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa73f, clk->peri_ctrl_base + PERI_CTRL_USB1);
		mdelay(1);
		/*
		 * >=1ms later
		 * 4. address 0x0b, send data 0xe0
		 */
		writel(0xabe0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xebe0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xabe0, clk->peri_ctrl_base + PERI_CTRL_USB1);
		mdelay(1);
		
		/* solve 2p phy disconnect issue */
		writel(0xaa02, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xea02, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xaa02, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xba1e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xfa1e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xba1e, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(500);

		/* PLL core clock 480M */
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(300);

	}
}

/******************************************************************************/

static void inno_phy_config_1p(struct hiclk_hw *clk)
{
	u32 reg;
	u32 wrdata = 0;
	u64 chipid = get_chipid(0ULL);

	if ((chipid & 0xf0) == 0x10) {

		wrdata = 0x4;
		/* config 1P PHY clk output 
		* delay 1ms for waiting PLL stable
		*/
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		mdelay(1);

		/* write 0x1c to addr 0x00  
		* 0x00[0] = 0 : close EOP pre-emphasis
		* 0x00[2] = 1 : open Data pre-emphasis
		*/
		writel(0xa01c, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xe01c, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xa01c, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(100);
	} else {

		wrdata = 0xc;
		/* config clk output */
		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(500);

		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(500);

		reg = wrdata|TEST_ADDR|TEST_WREN|TEST_RSTN;
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB14);
		mdelay(10);

		/* solve 1p phy disconnect problem */
		writel(0xaa02, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xea02, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xaa02, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(500);
	}
}

/******************************************************************************/

static int hiclk_enable_usb2_host0(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_add_return(1, &dev_open_cnt0) == 1) {

		/* reset enable */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(200);

		reg = readl(clk->peri_ctrl_base +PERI_CTRL_USB0);
		reg |= ULPI_BYPASS_EN_PORT0;  /* 1 ports utmi */
		reg &= ~(WORDINTERFACE);      /* 8bit */
		reg &= ~(SS_BURST16_EN);      /* 16 bit burst disable */
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
		udelay(100);

		/* open ref clk */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB_PHY0_REF_CKEN
			| USB_PHY1_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		/* PLL core clock 240M */
		writel(0xa809, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xe809, clk->peri_ctrl_base + PERI_CTRL_USB1);
		writel(0xa809, clk->peri_ctrl_base + PERI_CTRL_USB1);
		udelay(300);

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		udelay(300);

		inno_phy_config_2p(clk);

		/* cancel port reset 
		* delay 2ms for waiting comp circuit stable
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &=~(USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		mdelay(2);

		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &=~(USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);

		reg |= (USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN);
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
		/* reset enable */
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg |= (USB2_BUS_SRST_REQ1
			| USB2_UTMI0_SRST_REQ1 
			| USB2_HST_PHY_SYST_REQ1);
		writel(reg, clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		udelay(200);

		reg = readl(clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		reg |= (USB2_PHY2_SRST_REQ
			| USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		udelay(200);

		reg = readl(clk->peri_ctrl_base + PERI_CTRL_USB13);
		reg |= ULPI_BYPASS_EN_PORT0; 
		reg &= ~(WORDINTERFACE);      
		reg &= ~(SS_BURST16_EN);     
		writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB13);
		udelay(100);

		/* open ref clk */
		reg = readl(clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		reg |= (USB2_PHY2_REF_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		udelay(300);

		/* PLL core clock 240M */
		writel(0xa809, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xe809, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xa809, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(300);

		/* cancel power on reset */
		reg = readl(clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		reg &= ~(USB2_PHY2_SRST_REQ);
		writel(reg, clk->peri_crg_base +PERI_CRG100_USB2PHY2);
		mdelay(2);

		/* PLL core clock 480M */
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xe801, clk->peri_ctrl_base + PERI_CTRL_USB14);
		writel(0xa801, clk->peri_ctrl_base + PERI_CTRL_USB14);
		udelay(300);

		inno_phy_config_1p(clk);

		/* cancel port reset 
		* delay 2ms for waiting comp circuit stable
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		reg &= ~(USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		mdelay(2);

		/* cancel control reset */
		reg = readl(clk->peri_crg_base + PERI_CRG102_USB2CTRL2);
		reg &= ~(USB2_BUS_SRST_REQ1
			| USB2_UTMI0_SRST_REQ1
			| USB2_HST_PHY_SYST_REQ1);
		reg |= (USB2_BUS_CKEN1
			| USB2_OHCI48M_CKEN1
			| USB2_OHCI12M_CKEN1
			| USB2_HST_PHY_CKEN1
			| USB2_UTMI0_CKEN1);
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
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |=(USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ);
		writel(reg, clk->peri_crg_base +PERI_CRG47_USB2PHY);
		udelay(100);

		/* close clock  */
		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &=~(USB2_BUS_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
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

		reg = readl(clk->peri_crg_base + PERI_CRG100_USB2PHY2);
		reg |= (USB2_PHY2_SRST_REQ
			| USB2_PHY2_SRST_TREQ);
		writel(reg, clk->peri_crg_base + PERI_CRG100_USB2PHY2);
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
