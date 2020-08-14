#include <asm/io.h>
#include <common.h>

#define PERI_CRG46                      (0xF8A22000 + 0xb8)
#define USB2_BUS_CKEN                   (1<<0)
#define USB2_OHCI48M_CKEN               (1<<1)
#define USB2_OHCI12M_CKEN               (1<<2)
#define USB2_OTG_UTMI_CKEN              (1<<3)
#define USB2_HST_PHY_CKEN               (1<<4)
#define USB2_UTMI0_CKEN                 (1<<5)
#define USB2_BUS_SRST_REQ               (1<<12)
#define USB2_UTMI0_SRST_REQ             (1<<13)
#define USB2_HST_PHY_SYST_REQ           (1<<16)
#define USB2_OTG_PHY_SYST_REQ           (1<<17)
#define USB2_CLK48_SEL                  (1<<20)

#define PERI_CRG102                     (0xF8A22000 + 0x198)
#define USB2_BUS_CKEN1                  (1<<0)
#define USB2_OHCI48M_CKEN1              (1<<1)
#define USB2_OHCI12M_CKEN1              (1<<2)
#define USB2_HST_PHY_CKEN1              (1<<4)
#define USB2_UTMI0_CKEN1                (1<<5)
#define USB2_BUS_SRST_REQ1              (1<<12)
#define USB2_UTMI0_SRST_REQ1            (1<<14)
#define USB2_HST_PHY_SYST_REQ1          (1<<16)
#define USB2_CLK48_SEL1                 (1<<20)

#define PERI_CRG47                      (0xF8A22000 + 0xbc)
#define USB2_PHY01_REF_CKEN              (1 << 0)
#define USB2_PHY2_REF_CKEN               (1 << 2)
#define USB2_PHY3_REF_CKEN               (1 << 3)
#define USB2_PHY01_SRST_REQ              (1 << 4)
#define USB2_PHY2_SRST_REQ               (1 << 6)
#define USB2_PHY3_SRST_REQ               (1 << 7)
#define USB2_PHY01_SRST_TREQ0            (1 << 8)
#define USB2_PHY01_SRST_TREQ1            (1 << 9)
#define USB2_PHY2_SRST_TREQ              (1 << 10)
#define USB2_PHY3_SRST_TREQ              (1 << 11)
#define USB2_PHY01_REFCLK_SEL            (1 << 12)
#define USB2_PHY2_REFCLK_SEL             (1 << 14)
#define USB2_PHY3_REFCLK_SEL             (1 << 15)

#define PERI_USB0                       (0xF8A20000 + 0x120)
#define PERI_USB1                       (0xF8A20000 + 0x124)
#define PERI_USB3                       (0xF8A20000 + 0x12c)
#define PERI_USB4                       (0xF8A20000 + 0x130)

#define TEST_WRDATA                     (0xc)
#define TEST_ADDR                       (0x6 << 8)
#define TEST_WREN                       (1 << 13)
#define TEST_CLK                        (1 << 14)
#define TEST_RSTN                       (1 << 15)

#define WORDINTERFACE                   (1 << 0)
#define ULPI_BYPASS_EN_PORT0            (1 << 3)
#define SS_BURST16_EN                   (1 << 9)


extern void udelay(unsigned long usec);

static int usb1_cpu_init(void)
{
	int reg;

	/* control bus/utmi/roothub reset enable */
	reg = readl(PERI_CRG102);
	reg |= (USB2_BUS_SRST_REQ1 //12th bit
		| USB2_UTMI0_SRST_REQ1 //14th bit
		| USB2_HST_PHY_SYST_REQ1);//16th bit
	writel(reg, PERI_CRG102);
	udelay(200);

	/* phy por/utmi reset enable */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY2_SRST_REQ
		| USB2_PHY2_SRST_TREQ);
	writel(reg, PERI_CRG47);
	udelay(200);

#ifdef CONFIG_HIFONE_FPGA
	reg = readl(PERI_USB4);
	reg &= ~ULPI_BYPASS_EN_PORT0;     /* port2:ulpi */
	reg &= ~(WORDINTERFACE);         /*  port2 : 8bit*/
	reg &= ~(SS_BURST16_EN);
	writel(reg, PERI_USB4);
	udelay(100);
#else
	reg = readl(PERI_USB4);
	reg |= ULPI_BYPASS_EN_PORT0;  /* 1 ports utmi */
	reg &= ~(WORDINTERFACE);      /* 8bit */
	reg &= ~(SS_BURST16_EN);      /* 16 bit burst disable */
	writel(reg, PERI_USB4);
	udelay(100);
#endif

	/* open ref clk */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY2_REF_CKEN);
	writel(reg, PERI_CRG47);
	udelay(300);

	/* cancel phy power on reset */
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY2_SRST_REQ);
	writel(reg, PERI_CRG47);
	udelay(500);

	/* config clk output */
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB1);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
	writel(reg, PERI_USB1);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB1);
	udelay(50);

#ifdef CONFIG_HIFONE_FPGA
	udelay(1000);
#else
	udelay(10000);
#endif

	/* cancel phy utmi port reset */
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY2_SRST_TREQ);
	writel(reg, PERI_CRG47);
	udelay(300);

	/* ctrl control clk open */
	reg = readl(PERI_CRG102);
	reg |= (USB2_BUS_CKEN1 
		| USB2_OHCI48M_CKEN1
		| USB2_OHCI12M_CKEN1
		| USB2_HST_PHY_CKEN1
		| USB2_UTMI0_CKEN1);
	writel(reg, PERI_CRG102);
	udelay(200);

	/* cancel control reset */
	reg = readl(PERI_CRG102);
	reg &= ~(USB2_BUS_SRST_REQ1
		| USB2_UTMI0_SRST_REQ1
		| USB2_HST_PHY_SYST_REQ1);
	writel(reg, PERI_CRG102);
	udelay(200);

	return 0;
}



static int usb0_cpu_init(void)
{
	int reg;

	/* control bus/utmi/roothub reset enable */
	reg = readl(PERI_CRG46);
	reg |= (USB2_BUS_SRST_REQ
		| USB2_UTMI0_SRST_REQ
		| USB2_HST_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);

	/* phy por/utmi reset enable */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_SRST_REQ
		| USB2_PHY01_SRST_TREQ0);
	writel(reg, PERI_CRG47);
	udelay(200);

#ifdef CONFIG_HIFONE_FPGA 
	reg = readl(PERI_USB3);
	reg &= ~ULPI_BYPASS_EN_PORT0;     /* port0/port1:ulpi */
	reg &= ~(WORDINTERFACE);            /*  port0 : 8bit*/
	reg &= ~(SS_BURST16_EN);
	writel(reg, PERI_USB3);
	udelay(100);
#else
	reg = readl(PERI_USB3);
	reg |= ULPI_BYPASS_EN_PORT0;  /* 2 ports utmi */
	reg &= ~(WORDINTERFACE);      /* 8bit */
	reg &= ~(SS_BURST16_EN);      /* 16 bit burst disable */
	writel(reg, PERI_USB3);
	udelay(100);
#endif

	/* open PHY Ref Clock */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_REF_CKEN);
	writel(reg, PERI_CRG47);
	udelay(300);
	
	/* cancel power on reset */
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY01_SRST_REQ);
	writel(reg, PERI_CRG47);
	udelay(500);

	/* config clk output */    
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
	writel(reg, PERI_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB0);
	udelay(50);

#ifdef CONFIG_HIFONE_FPGA
	udelay(1000);
#else
	udelay(10000);
#endif

	/* cancel phy utmi  reset */
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY01_SRST_TREQ0);
	writel(reg, PERI_CRG47);
	udelay(300);

	/* open control cken */
	reg = readl(PERI_CRG46);
	reg |= (USB2_BUS_CKEN
		| USB2_OHCI48M_CKEN
		| USB2_OHCI12M_CKEN
		| USB2_OTG_UTMI_CKEN
		| USB2_HST_PHY_CKEN
		| USB2_UTMI0_CKEN);
	writel(reg, PERI_CRG46);
	udelay(200);

	/* cancel control reset */
	reg = readl(PERI_CRG46);
	reg &= ~(USB2_BUS_SRST_REQ
		| USB2_UTMI0_SRST_REQ
		| USB2_HST_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);
	udelay(200);

	return 0;
}


int usb_cpu_init(int index, unsigned int *addr)
{
	if (index == 0) {
		if (addr)
#ifdef  CONFIG_USB_EHCI
			*addr = REG_BASE_EHCI;
#else
			*addr = REG_BASE_OHCI;
#endif
		return usb0_cpu_init();;
	} else if (index == 1) {
		if (addr)
#ifdef  CONFIG_USB_EHCI
			*addr = REG_BASE_EHCI1;
#else
			*addr = REG_BASE_OHCI1;
#endif
		return usb1_cpu_init();
	}

	return -1;
}



static int usb1_cpu_stop(void)
{
	int reg;

	reg = readl(PERI_CRG102);
	reg |= (USB2_BUS_SRST_REQ1
		| USB2_UTMI0_SRST_REQ1
		| USB2_HST_PHY_SYST_REQ1);
	writel(reg, PERI_CRG102);

	udelay(200);

	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY2_SRST_REQ
		| USB2_PHY2_SRST_TREQ);
	writel(reg, PERI_CRG47);

	udelay(100);

	return 0;
}

static int usb0_cpu_stop(void)
{
	int reg;
	reg = readl(PERI_CRG46);
	reg |= (USB2_BUS_SRST_REQ
		| USB2_UTMI0_SRST_REQ
		| USB2_HST_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);

	udelay(200);

	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_SRST_REQ
		| USB2_PHY01_SRST_TREQ0);
	writel(reg, PERI_CRG47);
	
	udelay(100);

	return 0;
}
int usb_cpu_stop(int index)
{
	if (index == 0)
		return usb0_cpu_stop();
	else if (index == 1)
		return usb1_cpu_stop();

	return -1;
}

int usb_cpu_init_fail(void)
{
	usb0_cpu_stop();
	usb1_cpu_stop();

	return 0;
}
