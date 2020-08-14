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
#include <dt-bindings/clock/hi3798cv200-clock.h>
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

#define TEST_WRDATA                     (0x4)
#define TEST_ADDR                       (0x6 << 8)
#define TEST_WREN                       BIT(21)
#define TEST_CLK                        BIT(22)
#define TEST_RSTN                       BIT(23)

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
#define USB2_PHY01_SRST_TREQ1           BIT(9)
#define USB2_PHY2_SRST_TREQ             BIT(10)
#define USB2_PHY01_REFCLK_SEL           BIT(12)
#define USB2_PHY2_REFCLK_SEL            BIT(14)

/******************************************************************************/

static atomic_t dev_open_cnt = {
	.counter = 0,
};
/******************************************************************************/

static void inno_phy_config_2p_1(struct hiclk_hw *clk)
{
	u32 reg;
	/* write 0x4 to addr 0x06
	* config 2P PHY clk output 
	* delay 1ms for waiting PLL stable
	*/
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
	writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, clk->peri_ctrl_base + PERI_CTRL_USB0);
	mdelay(1);

	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa1001c, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe1001c, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa1001c, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

	/* write 0x07 to 0x06 
	* {0x06[1:0],0x05[7]} = 110 : Rcomp = 150mV , increase DC level
	*/
	writel(0xa00607, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe00607, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa00607, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa10700, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe10700, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa10700, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

	/* write 0xab to 0x0a
	* 0x0a[7:5] = 101 : Icomp = 212mV , increase current drive
	*/
	writel(0xa00aab, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe00aab, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa00aab, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

	/* write 0x40 to addr 0x11  
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will
	* not stay at 1 when error packets received
	*/
	writel(0xa11140, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe11140, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa11140, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

	/* write 0x41 to addr 0x10  
	* 0x10[0] = 1 : Comp Mode Select
	*/
	writel(0xa11041, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xe11041, clk->peri_ctrl_base + PERI_CTRL_USB0);
	writel(0xa11041, clk->peri_ctrl_base + PERI_CTRL_USB0);
	udelay(20);

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
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SYST_REQ);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);

		/* reset phy por/utmi */
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg |= (USB2_PHY01_SRST_REQ
			| USB2_PHY01_SRST_TREQ1);
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

		inno_phy_config_2p_1(clk);

		/* cancel port reset 
		* delay 2ms for waiting comp circuit stable
		*/
		reg = readl(clk->peri_crg_base + PERI_CRG47_USB2PHY);
		reg &= ~(USB2_PHY01_SRST_TREQ1);
		writel(reg, clk->peri_crg_base + PERI_CRG47_USB2PHY);
		mdelay(2);

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

static void hiclk_disable_usb2(struct clk_hw *hw)
{
	u32 reg;
	struct hiclk_hw *clk = to_hiclk_hw(hw);

	if (atomic_sub_return(1, &dev_open_cnt) == 0) {

		reg = readl(clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		reg &= ~(USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_OTG_UTMI_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN);
		writel(reg, clk->peri_crg_base + PERI_CRG46_USB2CTRL);
		udelay(200);
	}
}
/******************************************************************************/

struct clk_ops clk_ops_hiusb2 = {
	.enable = hiclk_enable_usb2,
	.disable = hiclk_disable_usb2,
};
