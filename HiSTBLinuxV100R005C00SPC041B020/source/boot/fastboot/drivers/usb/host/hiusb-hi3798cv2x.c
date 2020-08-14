#include <asm/io.h>
#include <common.h>

#define PERI_USB0                       (0xF8A20000 + 0x120)
#define PERI_USB1                       (0xF8A20000 + 0x124)
#define PERI_USB3                       (0xF8A20000 + 0x12c)
#define PERI_USB4                       (0xF8A20000 + 0x130)

#define WORDINTERFACE                   (1 << 0)
#define ULPI_BYPASS_EN_PORT0            (1 << 3)
#define SS_BURST16_EN                   (1 << 9)
#define TEST_WRDATA                     (0x4)
#define TEST_ADDR                       (0x6 << 8)
#define TEST_WREN                       (1 << 21)
#define TEST_CLK                        (1 << 22)
#define TEST_RSTN                       (1 << 23)

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
#define USB2_PHY01_SRST_REQ              (1 << 4)
#define USB2_PHY2_SRST_REQ               (1 << 6)
#define USB2_PHY01_SRST_TREQ0            (1 << 8)
#define USB2_PHY01_SRST_TREQ1            (1 << 9)
#define USB2_PHY2_SRST_TREQ              (1 << 10)
#define USB2_PHY01_REFCLK_SEL            (1 << 12)
#define USB2_PHY2_REFCLK_SEL             (1 << 14)

extern void udelay(unsigned long usec);
/******************************************************************************/

static void inno_phy_config_2p_1(void)
{
	u32 reg;
	/* write 0x4 to addr 0x06
	* config 2P PHY clk output 
	* delay 1ms for waiting PLL stable
	*/
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN|TEST_CLK;
	writel(reg, PERI_USB0);
	reg = TEST_WRDATA|TEST_ADDR|TEST_WREN|TEST_RSTN;
	writel(reg, PERI_USB0);
	mdelay(1);

	/* write 0x1c to addr 0x00  
	* 0x00[0] = 0 : close EOP pre-emphasis
	* 0x00[2] = 1 : open Data pre-emphasis
	*/
	writel(0xa1001c, PERI_USB0);
	writel(0xe1001c, PERI_USB0);
	writel(0xa1001c, PERI_USB0);
	udelay(20);

	/* write 0x07 to 0x06 
	* {0x06[1:0],0x05[7]} = 110 : Rcomp = 150mV , increase DC level
	*/
	writel(0xa00607, PERI_USB0);
	writel(0xe00607, PERI_USB0);
	writel(0xa00607, PERI_USB0);
	udelay(20);

	/* write 0x00 to addr 0x07  
	* 0x07[1] = 0 : Keep Rcomp working 
	*/
	writel(0xa10700, PERI_USB0);
	writel(0xe10700, PERI_USB0);
	writel(0xa10700, PERI_USB0);
	udelay(20);

	/* write 0xab to 0x0a
	* 0x0a[7:5] = 101 : Icomp = 212mV , increase current drive
	*/
	writel(0xa00aab, PERI_USB0);
	writel(0xe00aab, PERI_USB0);
	writel(0xa00aab, PERI_USB0);
	udelay(20);

	/* write 0x40 to addr 0x11  
	* 0x11[6:5] = 10 : sovle EMI problem, rx_active will
	* not stay at 1 when error packets received
	*/
	writel(0xa11140, PERI_USB0);
	writel(0xe11140, PERI_USB0);
	writel(0xa11140, PERI_USB0);
	udelay(20);

	/* write 0x41 to addr 0x10  
	* 0x10[0] = 1 : Comp Mode Select
	*/
	writel(0xa11041, PERI_USB0);
	writel(0xe11041, PERI_USB0);
	writel(0xa11041, PERI_USB0);
	udelay(20);
}
/******************************************************************************/

int usb_cpu_init(int index, unsigned int *addr)
{
	int reg;
	
	if (index != 0)
		return -1;
		
	if (addr) {
#ifdef  CONFIG_USB_EHCI
		*addr = REG_BASE_EHCI;
#else
		*addr = REG_BASE_OHCI;
#endif
	}

	/* reset controller bus/utmi/roothub  */
	reg = readl(PERI_CRG46);
	reg |= (USB2_BUS_SRST_REQ
		| USB2_UTMI0_SRST_REQ
		| USB2_HST_PHY_SYST_REQ
		| USB2_OTG_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);
	udelay(200);

	/* reset phy por/utmi */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_SRST_REQ
		| USB2_PHY01_SRST_TREQ1);
	writel(reg, PERI_CRG47);
	udelay(200);

	reg = readl(PERI_USB3);
	reg |= ULPI_BYPASS_EN_PORT0;
	reg &= ~(WORDINTERFACE);
	reg &= ~(SS_BURST16_EN);
	writel(reg, PERI_USB3);
	udelay(100);

	/* open ref clk */
	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_REF_CKEN);
	writel(reg, PERI_CRG47);
	udelay(300);

	/* cancel power on reset */
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY01_SRST_REQ);
	writel(reg, PERI_CRG47);
	udelay(500);

	inno_phy_config_2p_1();

	/* cancel port reset 
	 * delay 10ms for waiting comp circuit stable
	*/
	reg = readl(PERI_CRG47);
	reg &= ~(USB2_PHY01_SRST_TREQ1);
	writel(reg, PERI_CRG47);
	mdelay(10);

	/* open controller clk */
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
		| USB2_HST_PHY_SYST_REQ
		| USB2_OTG_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);
	udelay(200);

	return 0;
}
/******************************************************************************/

int usb_cpu_stop(int index)
{
	int reg;

	if (index != 0)
		return -1;

	reg = readl(PERI_CRG46);
	reg |= (USB2_BUS_SRST_REQ
		| USB2_UTMI0_SRST_REQ
		| USB2_HST_PHY_SYST_REQ);
	writel(reg, PERI_CRG46);

	udelay(200);

	reg = readl(PERI_CRG47);
	reg |= (USB2_PHY01_SRST_REQ
		| USB2_PHY01_SRST_TREQ1);
	writel(reg, PERI_CRG47);

	udelay(100);

	return 0;
}
/******************************************************************************/

int usb_cpu_init_fail(void)
{
	return usb_cpu_stop(0);
}
