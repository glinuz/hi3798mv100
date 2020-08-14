#include "stmmac.h"
#include "ctrl.h"

int stmmac_hw_set_macaddress(struct stmmac_netdev_local *ld, unsigned char *mac)
{
	unsigned long reg;

	reg = mac[4] | (mac[5] << 8);
	writel(reg, ld->iobase_phys + GMAC_ADDR_HIGH);

	reg = mac[0] | (mac[1] << 8) | (mac[2] << 16) | (mac[3] << 24);
	writel(reg, ld->iobase_phys + GMAC_ADDR_LOW);

	return 0;
}

int stmmac_hw_get_macaddress(struct stmmac_netdev_local *ld, unsigned char *mac)
{
	unsigned long reg;

	reg = readl(ld->iobase_phys + GMAC_ADDR_HIGH);
	mac[0] = (reg >> 8) & 0xff;
	mac[1] = reg & 0xff;

	reg = readl(ld->iobase_phys + GMAC_ADDR_LOW);
	mac[2] = (reg >> 24) & 0xff;
	mac[3] = (reg >> 16) & 0xff;
	mac[4] = (reg >> 8) & 0xff;
	mac[5] = reg & 0xff;

	return 0;
}

void stmmac_sys_init(struct stmmac_netdev_local *ld)
{
	unsigned long p = ld->iobase_phys;
	unsigned int data;

	/*softreset*/
	writel(DMA_BUS_MODE_SFT_RESET, p + DMA_BUS_MODE);
	udelay(100);

	data = readl(ld->iobase_phys + GMAC_FRAME_FILTER);
	data &= (~GMAC_FRAME_FILTER_PM);
	data |= (GMAC_FRAME_FILTER_DBF | GMAC_SA_FILTER_EN);
	writel(data, ld->iobase_phys + GMAC_FRAME_FILTER);

	/*clearall interrupt status*/
	writel(0x1ffff, p + DMA_STATUS);

	/*disable all interrupts */
	writel(0x0, p + DMA_INTR_ENA);

}
