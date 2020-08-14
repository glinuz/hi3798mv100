#include "stmmac.h"
#include "ctrl.h"
#include "mdio.h"

struct stmmac_netdev_local g_stmmac_ld ;

char PHY_NAME[MAX_PHY_NAME_LEN] = STMMAC_PHYNAME;
unsigned char PHY_ADDR = STMMAC_PHYADDR;

struct dma_desc rd __attribute__ ((aligned(16)));
struct dma_desc td __attribute__ ((aligned(16)));

void string_to_mac(unsigned char *mac, char *s)
{
	int i;
	char *e;

	for (i = 0; i < MAC_LEN; ++i) {
		mac[i] = s ? simple_strtoul(s, &e, 16) : 0;
		if (s)
			s = (*e) ? e+1 : e;
	}
}

static unsigned long rstate = 1;
void random_ether_addr(unsigned char *mac)
{
	unsigned long ethaddr_low, ethaddr_high;

	srand((unsigned int)get_timer(0));
	/*
	 * setting the 2nd LSB in the most significant byte of
	 * the address makes it a locally administered ethernet
	 * address
	 */
	ethaddr_high = (rand() & 0xfeff) | 0x0200;
	ethaddr_low = rand();

	mac[5] = ethaddr_high >> 8;
	mac[4] = ethaddr_high & 0xff;
	mac[3] = ethaddr_low >> 24;
	mac[2] = (ethaddr_low >> 16) & 0xff;
	mac[1] = (ethaddr_low >> 8) & 0xff;
	mac[0] = ethaddr_low & 0xff;

	mac[0] &= 0xfe;    /* clear multicast bit */
	mac[0] |= 0x02;    /* set local assignment bit (IEEE802) */

}

static int set_random_mac_address(unsigned char *mac, unsigned char *ethaddr)
{
	random_ether_addr(mac);

	sprintf(ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	setenv("ethaddr", ethaddr);

	stmmac_hw_set_macaddress(&g_stmmac_ld, mac);

	return 0;
}

static int stmmac_net_set_mac_address(struct stmmac_netdev_local *ld)
{
	char *s;
	unsigned char mac[MAC_LEN];
	unsigned char ethaddr[20];

	s = getenv("ethaddr");
	if (!s)
		set_random_mac_address(mac, ethaddr);

	string_to_mac(mac, s);

	if (!is_valid_ether_addr(mac))
		set_random_mac_address(mac, ethaddr);

	print_mac(mac);

	stmmac_hw_set_macaddress(ld, mac);

	return 0;
}

static int stmmac_init_hw_desc_queue(struct stmmac_netdev_local *ld)
{
	int count = 0;

	count = STMMAC_HW_QUEUE_DEPTH;

	memset((void *)&td, 0, (count * sizeof(struct dma_desc)));
	td.status = TXDESCENDOFRING;
	writel(&td, ld->iobase_phys + DMA_TX_BASE_ADDR);

	/* init rx queue */
	memset((void *)&rd, 0, (count * sizeof(struct dma_desc)));
	rd.length = RXDESCENDOFRING;
	writel(&rd, ld->iobase_phys + DMA_RCV_BASE_ADDR);

	return 0;
}

static void stmmac_destroy_hw_desc_queue(struct stmmac_netdev_local *ld)
{
	ld->tx_addr = NULL;
	ld->rx_addr = NULL;
}

static void stmmac_net_adjust_link(struct stmmac_netdev_local *ld)
{
	int stat = 0, speed = 0;

	stat |= miiphy_link(ld->phy_name, PHY_ADDR) ? STMMAC_LINKED : 0;
	stat |= miiphy_duplex(ld->phy_name, PHY_ADDR) ? STMMAC_DUP_FULL : 0;
	speed = miiphy_speed(ld->phy_name, PHY_ADDR);
	stat |= (speed == _100BASET ? STMMAC_SPD_100M :
	(speed == _10BASET ? STMMAC_SPD_10M : STMMAC_SPD_1000M));

	if (miiphy_link(ld->phy_name, PHY_ADDR) ==  STMMAC_LINKED) {
		if (miiphy_duplex(ld->phy_name, PHY_ADDR) == 44) {
			if (miiphy_speed(ld->phy_name, PHY_ADDR) == 1000)
				writel(0x280c, ld->iobase_phys + MAC_CTRL_REG);
			else if (miiphy_speed(ld->phy_name, PHY_ADDR) == 100)
				writel(0xe80c, ld->iobase_phys + MAC_CTRL_REG);
			else if (miiphy_speed(ld->phy_name, PHY_ADDR) == 10)
				writel(0xa80c, ld->iobase_phys + MAC_CTRL_REG);
		} else if (miiphy_duplex(ld->phy_name, PHY_ADDR) == 22) {
			if (miiphy_speed(ld->phy_name, PHY_ADDR) == 100)
				writel(0xe00c, ld->iobase_phys + MAC_CTRL_REG);
			else if (miiphy_speed(ld->phy_name, PHY_ADDR) == 10)
				writel(0xa00c, ld->iobase_phys + MAC_CTRL_REG);
		}
	} else if (miiphy_link(ld->phy_name, PHY_ADDR) == 0)
		printf("No Link!\n");

	ld->link_stat = stat;
	return;
}

int eth_rx(void)
{
	struct stmmac_netdev_local *ld = &g_stmmac_ld;
	int timeout_us = 100000;
	int lenth = 0;
	unsigned int rx_status = 0;

	rd.length |= 0x7d0;
	rd.buffer1 = (unsigned int)NetRxPackets[0];
	rd.status = DESCOWNBYDMA;

	/* Start RCV */
	writel(DMA_CONTROL_SR, ld->iobase_phys + DMA_CONTROL);

	while((--timeout_us) && ((rd.status & DESCOWNBYDMA) == DESCOWNBYDMA))
	{}

	if (!timeout_us) {
		memset((void *)&rd, 0, sizeof(struct dma_desc));
		rd.length = RXDESCENDOFRING;

		return -1;
	}

	rx_status = rd.status;
	lenth = ((rx_status & DESCRXLENTHMASK) >> DESCRXLENTHSHIFT) - 4;
	NetReceive((char *)NetRxPackets[0], lenth);

	/* Reset desc */
	memset((void *)(&rd), 0, sizeof(struct dma_desc));
	rd.length = RXDESCENDOFRING;

	return 0;
}

int eth_send(volatile void *packet, int length)
{
	struct stmmac_netdev_local *ld = &g_stmmac_ld;
	int timeout_us = 100000;

	td.length = ((length << DESCTXSIZESHIFT) & DESCTXSIZEMASK);
	td.status |= (DESCTXFIRST | DESCTXLAST);
	td.status |= DESCOWNBYDMA;
	td.buffer1 = (unsigned int)packet;

	/* Start transmit */
	writel(DMA_CONTROL_ST, ld->iobase_phys + DMA_CONTROL);
	writel(1, ld->iobase_phys + DMA_XMT_POLL_DEMAND);

	while(--timeout_us && ((td.status & DESCOWNBYDMA) == DESCOWNBYDMA))
	{}

	memset((void *)(&td), 0, sizeof(struct dma_desc));
	td.status = TXDESCENDOFRING;

	if (!timeout_us)
		return -1;

	return 0;
}

static int stmmac_init(void)
{
	int ret = 0;

	ret = stmmac_mdiobus_driver_init();
	if (ret) {
		printf("Init mdio bus fail!\n ");
		return -1;
	}

	g_stmmac_ld.phy_name = PHY_NAME;
	g_stmmac_ld.iobase_phys = STMMAC_IOBASE;
	g_stmmac_ld.iobase = STMMAC_IOBASE;

	stmmac_sys_init(&g_stmmac_ld);

	ret = stmmac_init_hw_desc_queue(&g_stmmac_ld);
	if (ret) {
		printf("init hw desc queue fail!\n");

		stmmac_mdiobus_driver_exit();

		return -1;
	}

	return 0;
}

static int stmmac_exit(void)
{
	unsigned long data;

	data = readl(g_stmmac_ld.iobase_phys + DMA_CONTROL);
	data &= (~DMA_CONTROL_ST);
	data &= (~DMA_CONTROL_SR);
	writel(data, g_stmmac_ld.iobase_phys + DMA_CONTROL);

	data = readl(g_stmmac_ld.iobase_phys + MAC_CTRL_REG);
	data &= (~MAC_ENABLE_TX);
	writel(data, g_stmmac_ld.iobase_phys + MAC_CTRL_REG);
	data &= (~MAC_RNABLE_RX);
	writel(data, g_stmmac_ld.iobase_phys + MAC_CTRL_REG);

	stmmac_destroy_hw_desc_queue(&g_stmmac_ld);
	stmmac_mdiobus_driver_exit();
}

int eth_init(bd_t *bd)
{
	int ret;
	int count = 10000;
	char *timeout;
	unsigned int data;

	timeout = getenv("phy_link_time");

	if (timeout)
		count = simple_strtoul(timeout, 0, 10);

	ret = stmmac_init();

	if (ret) {
		printf("gmac:Init fail!\n");
		return ret;
	}

retry:
	stmmac_net_adjust_link(&g_stmmac_ld);
	if (g_stmmac_ld.link_stat & STMMAC_LINKED)
		goto link_on;
	if (--count)
		goto retry;

	printf("PHY not link!\n");
	stmmac_exit();
	return -1;

link_on:
	stmmac_net_set_mac_address(&g_stmmac_ld);

	writel(0x60f, g_stmmac_ld.iobase_phys + GMAC_INT_MASK);
	writel(0xffffffff, g_stmmac_ld.iobase_phys + MMC_INTR_MASK_RX);
	writel(0xffffffff, g_stmmac_ld.iobase_phys + MMC_INTR_MASK_TX);
	writel(0xffffffff, g_stmmac_ld.iobase_phys + MMC_IPC_INTR_MASK_RX);
	writel(0x00000000, g_stmmac_ld.iobase_phys + DMA_INTR_ENA);
	writel(0x1ffff, g_stmmac_ld.iobase_phys + DMA_STATUS);

	data = readl(g_stmmac_ld.iobase_phys + DMA_CONTROL);
	data |= DMA_CONTROL_ST;
	data |= DMA_CONTROL_SR;
	writel(data, g_stmmac_ld.iobase_phys + DMA_CONTROL);

	return 0;
}

void eth_halt(void)
{
	if (g_stmmac_ld.iobase)
		stmmac_exit();
}
