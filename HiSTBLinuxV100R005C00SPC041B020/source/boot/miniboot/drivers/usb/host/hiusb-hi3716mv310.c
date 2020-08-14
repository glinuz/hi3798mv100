#include <delay.h>
#include <asm/io.h>
#include <cpuinfo.h>

/******************************************************************************/
#define HI3716MV310_PERI_CRG46                      (REG_BASE_CRG + 0xb8)
#define HI3716MV310_USB2_BUS_CKEN                   (1 << 0)
#define HI3716MV310_USB2_OHCI48M_CKEN               (1 << 1)
#define HI3716MV310_USB2_OHCI12M_CKEN               (1 << 2)
#define HI3716MV310_USB2_OTG_UTMI_CKEN              (1 << 3)
#define HI3716MV310_USB2_HST_PHY_CKEN               (1 << 4)
#define HI3716MV310_USB2_UTMI0_CKEN                 (1 << 5)
#define HI3716MV310_USB2_UTMI1_CKEN                 (1 << 6)
#define HI3716MV310_USB2_ADP_CKEN                   (1 << 8)
#define HI3716MV310_USB2_BUS_SRST_REQ               (1 << 12)
#define HI3716MV310_USB2_UTMI0_SRST_REQ             (1 << 13)
#define HI3716MV310_USB2_UTMI1_SRST_REQ             (1 << 14)
#define HI3716MV310_USB2_HST_PHY_SYST_REQ           (1 << 16)
#define HI3716MV310_USB2_OTG_PHY_SRST_REQ           (1 << 17)
#define HI3716MV310_USB2_ADP_SRST_REQ               (1 << 18)

#define HI3716MV310_PERI_CRG47                      (REG_BASE_CRG + 0xbc)
#define HI3716MV310_USB_PHY0_REF_CKEN               (1 << 0)
#define HI3716MV310_USB_PHY1_REF_CKEN               (1 << 1)
#define HI3716MV310_USB_PHY0_SRST_REQ               (1 << 8)
#define HI3716MV310_USB_PHY0_SRST_TREQ              (1 << 9)
#define HI3716MV310_USB_PHY1_SRST_REQ               (1 << 10)
#define HI3716MV310_USB_PHY1_SRST_TREQ              (1 << 11)
#define HI3716MV310_USB_PHY0_REFCLK_SEL             (1 << 16)
#define HI3716MV310_USB_PHY1_REFCLK_SEL             (1 << 17)

#define HI3716MV310_PERI_CRG51                      (REG_BASE_CRG + 0xcc)
#define HI3716MV310_PERI_CRG73                      (REG_BASE_CRG + 0x120)
#define HI3716MV310_PERI_CRG90                      (REG_BASE_CRG + 0x168)

#define HI3716MV310_PERI_FEPHY                      (REG_BASE_PERI_CTRL + 0x118)
#define HI3716MV310_PERI_LDO_CTRL                   (REG_BASE_PERI_CTRL + 0x11c)

#define HI3716MV310_PERI_USB0                       (REG_BASE_PERI_CTRL + 0x120)
#define HI3716MV310_WORDINTERFACE                   (1 << 0)
#define HI3716MV310_ULPI_BYPASS_EN_PORT0            (1 << 3)
#define HI3716MV310_ULPI_BYPASS_EN_PORT1_2          (1 << 4)
#define HI3716MV310_SS_BURST4_EN                    (1 << 7)
#define HI3716MV310_SS_BURST8_EN                    (1 << 8)
#define HI3716MV310_SS_BURST16_EN                   (1 << 9)
#define HI3716MV310_DWC_OTG_EN                      (1 << 28)

#define HI3716MV310_PERI_USB1                       (REG_BASE_PERI_CTRL + 0x124)

/******************************************************************************/
int usb_cpu_init(void)
{
	unsigned int reg;
	long long chipid = get_chipid();

	/* reset enable */
	reg = readl(HI3716MV310_PERI_CRG46);
	reg |= (HI3716MV310_USB2_BUS_SRST_REQ
		| HI3716MV310_USB2_UTMI0_SRST_REQ
		| HI3716MV310_USB2_UTMI1_SRST_REQ
		| HI3716MV310_USB2_HST_PHY_SYST_REQ
		| HI3716MV310_USB2_OTG_PHY_SRST_REQ);

	writel(reg, HI3716MV310_PERI_CRG46);
	udelay(200);

	reg = readl(HI3716MV310_PERI_CRG47);
	reg |= (HI3716MV310_USB_PHY0_SRST_REQ
		| HI3716MV310_USB_PHY1_SRST_REQ
		| HI3716MV310_USB_PHY0_SRST_TREQ
		| HI3716MV310_USB_PHY1_SRST_TREQ);
	writel(reg, HI3716MV310_PERI_CRG47);
	udelay(200);


	reg = readl(HI3716MV310_PERI_USB0);
	reg |= HI3716MV310_ULPI_BYPASS_EN_PORT0;  /* 3 ports utmi */
	reg &= ~(HI3716MV310_WORDINTERFACE);      /* 8bit */
	reg &= ~(HI3716MV310_SS_BURST16_EN);      /* 16 bit burst disable */
	writel(reg, HI3716MV310_PERI_USB0);
	udelay(100);

	/* for ssk usb storage ok */
	mdelay(10);

	/* open phy ref clock */
	reg = readl(HI3716MV310_PERI_CRG47);
	reg |= (HI3716MV310_USB_PHY0_REF_CKEN
		| HI3716MV310_USB_PHY1_REF_CKEN);

	writel(reg, HI3716MV310_PERI_CRG47);
	udelay(300);

	/* cancel power on reset */
	reg = readl(HI3716MV310_PERI_CRG47);
	reg &= ~(HI3716MV310_USB_PHY0_SRST_REQ
		| HI3716MV310_USB_PHY1_SRST_REQ);
	writel(reg , HI3716MV310_PERI_CRG47);
	udelay(300);

	/* config phy clock */
	writel(0xa60c, HI3716MV310_PERI_USB1);
	writel(0xe60c, HI3716MV310_PERI_USB1);
	writel(0xa60c, HI3716MV310_PERI_USB1);
	mdelay(5);

	/* improve eye diagram & disconnect param for MV310/MV320/3110EV500 -ludeng */
	writel(0xA16e, HI3716MV310_PERI_USB1);
	writel(0xE16e, HI3716MV310_PERI_USB1);
	writel(0xA16e, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xB16e, HI3716MV310_PERI_USB1);
	writel(0xF16e, HI3716MV310_PERI_USB1);
	writel(0xB16e, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xA532, HI3716MV310_PERI_USB1);
	writel(0xE532, HI3716MV310_PERI_USB1);
	writel(0xA532, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xB532, HI3716MV310_PERI_USB1);
	writel(0xF532, HI3716MV310_PERI_USB1);
	writel(0xB532, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xA60f, HI3716MV310_PERI_USB1);
	writel(0xE60f, HI3716MV310_PERI_USB1);
	writel(0xA60f, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xB60f, HI3716MV310_PERI_USB1);
	writel(0xF60f, HI3716MV310_PERI_USB1);
	writel(0xB60f, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xA703, HI3716MV310_PERI_USB1);
	writel(0xE703, HI3716MV310_PERI_USB1);
	writel(0xA703, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xB703, HI3716MV310_PERI_USB1);
	writel(0xF703, HI3716MV310_PERI_USB1);
	writel(0xB703, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xA94C, HI3716MV310_PERI_USB1);
	writel(0xE94C, HI3716MV310_PERI_USB1);
	writel(0xA94C, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xB94C, HI3716MV310_PERI_USB1);
	writel(0xF94C, HI3716MV310_PERI_USB1);
	writel(0xB94C, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xAAC3, HI3716MV310_PERI_USB1);
	writel(0xEAC3, HI3716MV310_PERI_USB1);
	writel(0xAAC3, HI3716MV310_PERI_USB1);
	udelay(5);
	writel(0xBAC3, HI3716MV310_PERI_USB1);
	writel(0xFAC3, HI3716MV310_PERI_USB1);
	writel(0xBAC3, HI3716MV310_PERI_USB1);
	udelay(300);

	/* cancel port reset */
	reg = readl(HI3716MV310_PERI_CRG47);
	reg &=~(HI3716MV310_USB_PHY1_SRST_TREQ);

	if (chipid == _HI3716M_V310) {
		reg &= ~HI3716MV310_USB_PHY0_SRST_TREQ;
	}

	writel(reg, HI3716MV310_PERI_CRG47);
	udelay(300);

	/* cancel control reset */
	reg = readl(HI3716MV310_PERI_CRG46);
	reg &=~(HI3716MV310_USB2_BUS_SRST_REQ
		| HI3716MV310_USB2_UTMI1_SRST_REQ
		| HI3716MV310_USB2_HST_PHY_SYST_REQ
		| HI3716MV310_USB2_OTG_PHY_SRST_REQ);

	reg |= (HI3716MV310_USB2_BUS_CKEN
		| HI3716MV310_USB2_OHCI48M_CKEN
		| HI3716MV310_USB2_OHCI12M_CKEN
		| HI3716MV310_USB2_OTG_UTMI_CKEN
		| HI3716MV310_USB2_HST_PHY_CKEN
		| HI3716MV310_USB2_UTMI1_CKEN
		| HI3716MV310_USB2_ADP_CKEN);

	if (chipid == _HI3716M_V310) {
		reg &= ~HI3716MV310_USB2_UTMI0_SRST_REQ;
		reg |= HI3716MV310_USB2_UTMI0_CKEN;
	}

	writel(reg, HI3716MV310_PERI_CRG46);
	udelay(200);

	return 0;
}

int usb_cpu_stop()
{
	int reg;

	/* reset enable */
	reg = readl(HI3716MV310_PERI_CRG46);
	reg |= (HI3716MV310_USB2_BUS_SRST_REQ
		| HI3716MV310_USB2_UTMI0_SRST_REQ
		| HI3716MV310_USB2_UTMI1_SRST_REQ
		| HI3716MV310_USB2_HST_PHY_SYST_REQ
		| HI3716MV310_USB2_OTG_PHY_SRST_REQ);

	writel(reg, HI3716MV310_PERI_CRG46);
	udelay(200);

	reg = readl(HI3716MV310_PERI_CRG47);
	reg |= (HI3716MV310_USB_PHY0_SRST_REQ
		| HI3716MV310_USB_PHY1_SRST_REQ
		| HI3716MV310_USB_PHY0_SRST_TREQ
		| HI3716MV310_USB_PHY1_SRST_TREQ);
	writel(reg, HI3716MV310_PERI_CRG47);
	udelay(200);

	return 0;
}

