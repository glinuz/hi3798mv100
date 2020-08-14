#include <asm/io.h>
#include <common.h>

#define PERI_CRG46                      (0xF8A22000 + 0xb8)
#define USB2_BUS_CKEN                   (1<<0)
#define USB2_OHCI48M_CKEN               (1<<1)
#define USB2_OHCI12M_CKEN               (1<<2)
#define USB2_OTG_UTMI_CKEN              (1<<3)
#define USB2_HST_PHY_CKEN               (1<<4)
#define USB2_UTMI0_CKEN                 (1<<5)
#define USB2_UTMI1_CKEN                 (1<<6)
#define USB2_UTMI2_CKEN                 (1<<7)
#define USB2_ADP_CKEN                   (1<<8)
#define USB2_BUS_SRST_REQ               (1<<12)
#define USB2_UTMI0_SRST_REQ             (1<<13)
#define USB2_UTMI1_SRST_REQ             (1<<14)
#define USB2_UTMI2_SRST_REQ             (1<<15)
#define USB2_HST_PHY_SYST_REQ           (1<<16)
#define USB2_OTG_PHY_SRST_REQ           (1<<17)
#define USB2_ADP_SRST_REQ               (1<<18)

#define PERI_CRG47                      (0xF8A22000 + 0xbc)
#define USB_PHY0_REF_CKEN               (1 << 0)
#define USB_PHY1_REF_CKEN               (1 << 1)
#define USB_PHY2_REF_CKEN               (1 << 2)
#define USB_PHY0_SRST_REQ               (1 << 8)
#define USB_PHY0_SRST_TREQ              (1 << 9)
#define USB_PHY1_SRST_REQ               (1 << 10)
#define USB_PHY1_SRST_TREQ              (1 << 11)
#define USB_PHY2_SRST_REQ               (1 << 12)
#define USB_PHY2_SRST_TREQ              (1 << 13)
#define USB_PHY0_REFCLK_SEL             (1 << 16)
#define USB_PHY1_REFCLK_SEL             (1 << 17)
#define USB_PHY2_REFCLK_SEL             (1 << 18)

#define PERI_USB0                       (0xF8A20000 + 0x120)
#define WORDINTERFACE                   (1 << 0)
#define ULPI_BYPASS_EN_PORT0            (1 << 3)
#define ULPI_BYPASS_EN_PORT1_2          (1 << 4)
#define SS_BURST4_EN                    (1 << 7)
#define SS_BURST8_EN                    (1 << 8)
#define SS_BURST16_EN                   (1 << 9)
#define DWC_OTG_EN                      (1 << 28)

#define PERI_USB1                       (0xF8A20000 + 0x124)
#define PHY0_TXPREEMPAMPTUNE_MASK       ~(0x3 << 27)
#define PHY0_TXPREEMPAMPTUNE_VALUE      (0x3 << 27)
#define PHY0_SIDDQ_MASK                 ~(0x1 << 22)
#define PHY0_SIDDQ_VALUE                (0x1 << 22)

#define PERI_USB2                       (0xF8A20000 + 0x128)
#define PHY3_TXPREEMPAMPTUNE_MASK       ~(0x3 << 27)
#define PHY3_TXPREEMPAMPTUNE_VALUE      (0x3 << 27)
#define PHY3_TXVREFTUNE_MASK            ~(0xF << 0)
#define PHY3_TXVREFTUNE_VALUE           (0x8 << 0)
#define PHY3_SIDDQ_MASK                 ~(0x1 << 22)
#define PHY3_SIDDQ_VALUE                (0x1 << 22)
#define PERI_USB4                       (0xF8A20000 + 0x130)
#define PHY2_TXPREEMPAMPTUNE_MASK       ~(0x3 << 27)
#define PHY2_TXPREEMPAMPTUNE_VALUE      (0x3 << 27)
#define PHY2_TXVREFTUNE_MASK            ~(0xF << 0)
#define PHY2_TXVREFTUNE_VALUE           (0x8 << 0)
#define PHY2_SIDDQ_MASK                 ~(0x1 << 22)
#define PHY2_SIDDQ_VALUE                (0x1 << 22)

extern long long get_chipid(void);
extern void udelay(unsigned long usec);
static unsigned long flags = 0;

int usb_cpu_init(int index, unsigned int *addr)
{
	if (index != 0)
		return -1;

	if (addr)
#ifdef  CONFIG_USB_EHCI
		*addr = REG_BASE_EHCI;
#else
		*addr = REG_BASE_OHCI;
#endif
	if (!flags) {

		int reg;

		/* reset enable */
		reg = readl(PERI_CRG46);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_UTMI2_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);

		writel(reg, PERI_CRG46);
		udelay(200);

		reg = readl(PERI_CRG47);
		reg |= (USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		writel(reg, PERI_CRG47);
		udelay(200);

#ifdef CONFIG_S40_FPGA
		reg = readl(PERI_USB0);
		reg |= ULPI_BYPASS_EN_PORT0;     /* port0:UTMI 16bit */
		reg &= ~ULPI_BYPASS_EN_PORT1_2;  /*  port1,port2:ulpi*/
		reg |= (WORDINTERFACE);          /*  port0 : 16bit*/
		reg &= ~(SS_BURST16_EN);
		writel(reg, PERI_USB0);
		udelay(100);
#else
		reg = readl(PERI_USB0);
		reg |= ULPI_BYPASS_EN_PORT0;  /* 3 ports utmi */
		reg &= ~(WORDINTERFACE);      /* 8bit */
		reg &= ~(SS_BURST16_EN);      /* 16 bit burst disable */
		writel(reg, PERI_USB0);
		udelay(100);
#endif

		/* cancel power on reset */
		reg = readl(PERI_CRG47);
		reg &= ~(USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ);
		writel(reg, PERI_CRG47);
		udelay(300);

		/* cancel port reset */
		reg = readl(PERI_CRG47);
		reg &= ~(USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		if (_HI3716C_V200ES == get_chipid()) {
			reg |= (USB_PHY0_REFCLK_SEL
				| USB_PHY1_REFCLK_SEL
				| USB_PHY2_REFCLK_SEL
				| USB_PHY0_REF_CKEN
				| USB_PHY1_REF_CKEN
				| USB_PHY2_REF_CKEN);
		} else {
			reg |= (USB_PHY0_REF_CKEN
				| USB_PHY1_REF_CKEN
				| USB_PHY2_REF_CKEN);
		}
		writel(reg, PERI_CRG47);
		udelay(300);

		/* cancel control reset */
		reg = readl(PERI_CRG46);
		reg &= ~(USB2_BUS_SRST_REQ
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
		writel(reg, PERI_CRG46);
		udelay(200);

		reg = readl(PERI_USB1);
		if (_HI3716C_V200ES == get_chipid()) {
			reg &= PHY0_TXPREEMPAMPTUNE_MASK;
			reg |= PHY0_TXPREEMPAMPTUNE_VALUE;
		}
		reg &= PHY0_SIDDQ_MASK;
		reg &= ~PHY0_SIDDQ_VALUE;
		writel(reg, PERI_USB1);
		udelay(200);

		reg = readl(PERI_USB2);
		if (_HI3716C_V200ES == get_chipid()) {
			reg &= PHY3_TXPREEMPAMPTUNE_MASK;
			reg |= PHY3_TXPREEMPAMPTUNE_VALUE;
		}
		reg &= PHY3_SIDDQ_MASK;
		reg &= ~PHY3_SIDDQ_VALUE;
		writel(reg, PERI_USB2);
		udelay(200);

		if (_HI3716C_V200ES == get_chipid()) {
			reg = readl(PERI_USB2);
			reg &= PHY3_TXVREFTUNE_MASK;
			reg |= PHY3_TXVREFTUNE_VALUE;
			writel(reg, PERI_USB2);
			udelay(200);
		}

		if (_HI3716C_V200ES == get_chipid()) {
			/* Fix the disconnect issue Of Inno Phy */
			writel(0xa207, PERI_USB4);
			writel(0xe207, PERI_USB4);
			writel(0xa207, PERI_USB4);
			writel(0xa1a6, PERI_USB4);
			writel(0xe1a6, PERI_USB4);
			writel(0xa1a6, PERI_USB4);
			writel(0xaa1b, PERI_USB4);
			writel(0xea1b, PERI_USB4);
			writel(0xaa1b, PERI_USB4);
		} else {
			reg = readl(PERI_USB4);
			reg &= PHY0_SIDDQ_MASK;
			reg &= ~PHY0_SIDDQ_VALUE;
			writel(reg, PERI_USB4);
			udelay(200);
		}

		flags = 1;
	}

	return 0;
}

int usb_cpu_stop(int index)
{
	if (index != 0)
		return -1;

	if (flags) {
		int reg;
		reg = readl(PERI_CRG46);
		reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_UTMI2_SRST_REQ
			| USB2_HST_PHY_SYST_REQ
			| USB2_OTG_PHY_SRST_REQ);
		writel(reg, PERI_CRG46);
		udelay(200);

		reg = readl(PERI_CRG47);
		reg |= (USB_PHY0_SRST_REQ
			| USB_PHY1_SRST_REQ
			| USB_PHY2_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ
			| USB_PHY2_SRST_TREQ);
		writel(reg, PERI_CRG47);
		udelay(100);

		flags = 0;
	}

	return 0;
}

int usb_cpu_init_fail(void)
{
	usb_cpu_stop(0);

	return 0;
}
