/* 

* XHCI for Hi3798mv100 

*/
/******************************************************************************/
#define BIT(nr)                     (1UL << (nr))

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

static int usb_cpu_init(void)
{
	u32 reg;

	/* use synop phy */
	reg = readl(REG_BASE_PERI_CTRL + PERI_CTRL);
	reg &= ~(USB3_IP_ISO_CTRL);
	writel(reg, REG_BASE_PERI_CTRL + PERI_CTRL);

	/* de-assert usb3_vcc_srst_req */
	writel(0x431, REG_BASE_CRG + REG_PERI_CRG101_USB3PHY);
	writel(0x13ff, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	mdelay(5);
	
	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_VCC_SRST_REQ);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	mdelay(5);

	reg = readl(REG_BASE_XHCI + REG_GUSB3PIPECTL0);
	reg |= PCS_SSP_SOFT_RESET;
	writel(reg, REG_BASE_XHCI + REG_GUSB3PIPECTL0);

	/*USB2 PHY chose ulpi 8bit interface */
	reg = readl(REG_BASE_XHCI + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~(BIT_UTMI_8_16);
	writel(reg, REG_BASE_XHCI + REG_GUSB2PHYCFG0);
	mdelay(1);

	/* [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(REG_BASE_XHCI + REG_GCTL);
	reg &= ~(0x3<<12);
	reg |= (0x1<<12); 
	writel(reg, REG_BASE_XHCI + REG_GCTL);

	/* de-assert usb3phy hard-macro por */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG101_USB3PHY);
	reg &= ~USB3_PHY_SRST_REQ;
	writel(reg, REG_BASE_CRG + REG_PERI_CRG101_USB3PHY);
	mdelay(5);

	/* de-assert hsp hard-macro port reset */
	reg = readl(REG_BASE_CRG + REG_PERI_CRG101_USB3PHY);
	reg &= ~USB3_PHY_SRST_TREQ; 
	writel(reg, REG_BASE_CRG + REG_PERI_CRG101_USB3PHY);

 	/* disable suspend */
	reg = readl(REG_BASE_XHCI + REG_GUSB3PIPECTL0);
	reg &= ~(1<<17);      
	reg &= ~PCS_SSP_SOFT_RESET;
	writel(reg, REG_BASE_XHCI + REG_GUSB3PIPECTL0);
	mdelay(5);

	writel(0x23100000, REG_BASE_XHCI + GTXTHRCFG);
	writel(0x23100000, REG_BASE_XHCI + GRXTHRCFG);
	mdelay(1);

	/* USB3.0 eye config */
	writel(0x7381560d, REG_BASE_PERI_CTRL + PERI_CTRL_USB6);

	return 0;
}
/******************************************************************************/

static int usb_cpu_stop(void)
{
	u32 reg;

	reg = readl(REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);
	reg &= ~(USB3_UTMI_CKEN
		| USB3_SUSPEND_CKEN
		| USB3_REF_CKEN
		| USB3_BUS_CKEN);
	writel(reg, REG_BASE_CRG + REG_PERI_CRG44_USB3CTRL);

	return 0;
}
/******************************************************************************/

int usb_xhci_cpu_init(int index, unsigned int *addr)
{
	if (addr)
		*addr = REG_BASE_XHCI;

	return usb_cpu_init();
}
/******************************************************************************/

int usb_xhci_cpu_stop(int index)
{
	return usb_cpu_stop();
}
