/* 

* XHCI for Hi3798cv200 

*/
/******************************************************************************/
#define BIT(nr)                     (1UL << (nr))

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


/******************************************************************************/
static void nano_phy_config_0(void)
{
	u32 reg;
	/* write 0x010c0012 to GUSB3PIPECTL0
	* GUSB3PIPECTL0[17] = 0 : disable U3 suspend 
	* GUSB3PIPECTL0[5:3] = 010 : Tx Margin = 900mV , decrease TX voltage
	* GUSB3PIPECTL0[2:1] = 01 : Tx Deemphasis = -3.5dB, refer to spec
	*/
	reg = readl(REG_BASE_XHCI0 + REG_GUSB3PIPECTL0);
	//reg &= ~USB3_SUSPEND_EN;
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS0;
	reg |= USB3_TX_MARGIN1;
	writel(reg, REG_BASE_XHCI0 + REG_GUSB3PIPECTL0);

	udelay(20);
}

static void nano_phy_config_1(void)
{
	u32 reg;
	/* write 0x010c0012 to GUSB3PIPECTL0
	* GUSB3PIPECTL0[17] = 0 : disable U3 suspend 
	* GUSB3PIPECTL0[5:3] = 010 : Tx Margin = 900mV , decrease TX voltage
	* GUSB3PIPECTL0[2:1] = 01 : Tx Deemphasis = -3.5dB, refer to spec
	*/
	reg = readl(REG_BASE_XHCI1 + REG_GUSB3PIPECTL0);
	//reg &= ~USB3_SUSPEND_EN;
	reg &= ~USB3_DEEMPHASIS_MASK;
	reg |= USB3_DEEMPHASIS0;
	reg |= USB3_TX_MARGIN1;
	writel(reg, REG_BASE_XHCI1 + REG_GUSB3PIPECTL0);

	udelay(20);
}

static void inno_phy_config_2p_0(void)
{
	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa0001c, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xe0001c, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xa0001c, REG_BASE_PERI_CTRL + PERI_USB0);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa00700, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xe00700, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xa00700, REG_BASE_PERI_CTRL + PERI_USB0);
	udelay(20);
	
	/* write 0x40 to addr 0x11  
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will 
	* not stay at 1 when error packets received
	*/
	writel(0xa01140, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xe01140, REG_BASE_PERI_CTRL + PERI_USB0);
	writel(0xa01140, REG_BASE_PERI_CTRL + PERI_USB0);
	udelay(20);
}
/******************************************************************************/

static void inno_phy_config_1p(void)
{
	/* write 0x4 to addr 0x06
	* 0x06[2] = 1 : config 1P PHY clk output 
	* delay 1ms for waiting PLL stable
	*/
	writel(0xa00604, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe00604, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00604, REG_BASE_PERI_CTRL + PERI_USB1);
	mdelay(1);

	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa0001c, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe0001c, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa0001c, REG_BASE_PERI_CTRL + PERI_USB1);
	udelay(20);
	
	/* write 0x92 to 0x05 
	* write 0x06 to 0x06
	* {0x06[1:0],0x05[7]} = 101 : Rcomp = 141mV , increase DC level
	*/
	writel(0xa00592, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe00592, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00592, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00606, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe00606, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00606, REG_BASE_PERI_CTRL + PERI_USB1);
	udelay(20);

	/* write 0xab to 0x0a
	* 0x0a[7:5] = 101 : Icomp = 212mV , increase current drive
	*/
	writel(0xa00aab, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe00aab, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00aab, REG_BASE_PERI_CTRL + PERI_USB1);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa00700, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe00700, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa00700, REG_BASE_PERI_CTRL + PERI_USB1);
	udelay(20);

	/* write 0x40 to addr 0x11  
	* 0x11[7] = 1  : Comp Mode Select
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will 
	* not stay at 1 when error packets received
	*/
	writel(0xa011c0, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xe011c0, REG_BASE_PERI_CTRL + PERI_USB1);
	writel(0xa011c0, REG_BASE_PERI_CTRL + PERI_USB1);
	udelay(20);
}
/******************************************************************************/

static int usb0_cpu_init(void)
{
	u32 reg;

	inno_phy_config_2p_0();

	/* cancel 2p phy utmi reset0 
	* delay 2ms for waiting comp circuit stable
	*/
	reg = readl(REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY01_SRST_TREQ0);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	mdelay(2);

	/* select VPLL 25MHz ref clock for COMB0
	* BPLL may be SSC in Hi3798CV200 
	*/
	reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	reg |= (COMBPHY0_REF_SEL1);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	udelay(100);

	/* open combphy0 cken */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	reg |= (COMBPHY0_REF_CKEN);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	udelay(200);

	/* cancel combphy0 POR */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	reg &= ~(COMBPHY0_SRST_REQ);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
	mdelay(1);

	/* open u3 ctrl0 bus cken */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN
		| USB3_BUS_GS_CKEN
		| USB3_BUS_CKEN);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	udelay(100);

	/* cancel u3 ctrl0 reset */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	udelay(200);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(REG_BASE_XHCI0 + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, REG_BASE_XHCI0 + REG_GUSB2PHYCFG0);
	
	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(REG_BASE_XHCI0 + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, REG_BASE_XHCI0 + REG_GCTL);

	nano_phy_config_0();

	/* open USB3 cken */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg |= (USB3_UTMI_CKEN
		| USB3_PIPE_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	mdelay(1);

	writel(0x23100000, REG_BASE_XHCI0 + GTXTHRCFG);
	writel(0x23100000, REG_BASE_XHCI0 + GRXTHRCFG);
	udelay(200);

	return 0;
}
/******************************************************************************/

static int usb1_cpu_init(void)
{
	u32 reg, mux=0;

	//TODO: make sure if combphy1 connected
	mux = readl(REG_BASE_PERI_CTRL + PERI_CTRL);
	mux &= COMBPHY1_SEL_MASK;

	if (mux != COMBPHY1_SEL_USB3) {
		reg = readl(REG_BASE_PERI_CTRL + PERI_USB7);
		reg |= USB3_PORT_DISABLE;
		writel(reg, REG_BASE_PERI_CTRL + PERI_USB7);
	}

	/* cancel usb2 1p phy POR */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY2_SRST_REQ);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	udelay(200);

	inno_phy_config_1p();

	/* cancel usb2 1p phy utmi reset 
	* delay 2ms for waiting comp circuit stable
	*/
	reg = readl(REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	reg &= ~(USB2_PHY2_SRST_TREQ);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG47_USB2PHY);
	mdelay(2);

	if (mux == COMBPHY1_SEL_USB3) {
		/* select VPLL 25MHz ref clock for COMB1
		* BPLL may be SSC in Hi3798CV200 
		*/
		reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		reg |= (COMBPHY1_REF_SEL1);
		writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		udelay(100);

		/* open combphy1 cken */
		reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		reg |= (COMBPHY1_REF_CKEN);
		writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		udelay(200);

		/* cancel combphy1 POR */
		reg = readl(REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		reg &= ~(COMBPHY1_SRST_REQ);
		writel(reg, REG_BASE_CRG + REG_PERI_CRG98_COMBPHY);
		mdelay(1);
	}

	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg |= (USB3_BUS_GM_CKEN1
		| USB3_BUS_GS_CKEN1
		| USB3_BUS_CKEN1);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	udelay(100);

	/* cancel u3 ctrl1 reset */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ1);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	udelay(200);

	/* USB2 PHY chose ulpi 8bit interface */
	reg = readl(REG_BASE_XHCI1 + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, REG_BASE_XHCI1 + REG_GUSB2PHYCFG0);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(REG_BASE_XHCI1 + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12);
	writel(reg, REG_BASE_XHCI1 + REG_GCTL);

	if (mux == COMBPHY1_SEL_USB3) {
		nano_phy_config_1();
	}

	/* open USB3 cken */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg |= (USB3_UTMI_CKEN1
		| USB3_PIPE_CKEN1
		| USB3_SUSPEND_CKEN1
		| USB3_REF_CKEN1);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	mdelay(1);

	writel(0x23100000, REG_BASE_XHCI1 + GTXTHRCFG);
	writel(0x23100000, REG_BASE_XHCI1 + GRXTHRCFG);
	udelay(200);

	return 0;
}
/******************************************************************************/

static int usb0_cpu_stop(void)
{
	u32 reg;

	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_BUS_CKEN
		| USB3_UTMI_CKEN	
		| USB3_PIPE_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);

	mdelay(1);
}
/******************************************************************************/

static int usb1_cpu_stop(void)
{
	u32 reg;

	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_BUS_CKEN1
		| USB3_UTMI_CKEN1
		| USB3_PIPE_CKEN1
		| USB3_SUSPEND_CKEN1
		| USB3_REF_CKEN1);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);

	mdelay(1);
}
/******************************************************************************/

int usb_xhci_cpu_init(int index, unsigned int *addr)
{
	if (index == 0) {
		if (addr)
			*addr = REG_BASE_XHCI0;

		return usb0_cpu_init();
	} else if (index == 1) {
		if (addr)
			*addr = REG_BASE_XHCI1;

		return usb1_cpu_init();
	}
}
/******************************************************************************/

int usb_xhci_cpu_stop(int index)
{
	if (index == 0)
		return usb0_cpu_stop();
	else if (index == 1)
		return usb1_cpu_stop();

	return -1;
}
