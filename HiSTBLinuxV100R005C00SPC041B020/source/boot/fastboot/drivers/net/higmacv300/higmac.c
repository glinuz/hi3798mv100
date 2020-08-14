#include <ethcfg.h>
#include "util.h"
#include "higmac.h"
#include "ctrl.h"
#include "mdio.h"

#define NET_IP_ALIGN	2

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/* suppose higmac_board_info[i] was initialed! */
#define for_each_gmac_netdev_local_priv(ld, i)  \
	for (i = 0; i < CONFIG_GMAC_NUMS &&     \
		({ld = &higmac_board_info[i].higmac_ld; 1;}); \
		i++)

struct higmac_board_info higmac_board_info[CONFIG_GMAC_NUMS] = {
	{
		{
			.index		= 0,
			.iobase		= HIGMAC0_IOBASE,
			.iobase_phys	= HIGMAC0_IOBASE,
		},
		.mii_name		= "mdio0",
		.phy_intf		= interface_mode_mii,
		.phy_addr		= CONFIG_HIGMAC_PHY1_ADDR,
	},
	{
		{
			.index		= 1,
			.iobase		= HIGMAC1_IOBASE,
			.iobase_phys	= HIGMAC1_IOBASE,
		},
		.mii_name		= "mdio1",
		.phy_intf		= interface_mode_mii,
		.phy_addr		= CONFIG_HIGMAC_PHY2_ADDR,
	},
#if (CONFIG_GMAC_NUMS > 2)
	{
		{
			.index		= 2,
			.iobase		= HIGMAC2_IOBASE,
			.iobase_phys	= HIGMAC2_IOBASE,
		},
		.mii_name		= "mdio2",
		.phy_intf		= interface_mode_mii,
		.phy_addr		= CONFIG_HIGMAC_PHY3_ADDR,
	}
#endif
};

static struct higmac_netdev_local *current_ld ;
static int phy_link_times;
static int gmac_debug;
static int gmac_use = -1;

static char *phy_intf_str[interface_mode_butt] = {
	"mii",
	"rmii",
	"rgmii",
};
static char *mdio_bus[CONFIG_GMAC_NUMS] = {
	"mdio0",
	"mdio1",
#if (CONFIG_GMAC_NUMS > 2)
	"mdio2"
#endif
};

#if defined(CONFIG_MACH_GODEYES)
/*
 *  * 3516(godeyes) port mode def.
 *   * port_mode_butt means not support this mode.
 *    */
static int g_speed_portmode_table[speed_mode_butt][interface_mode_butt] = {
	{port_mode_10_mii,      port_mode_butt,         port_mode_10_rgmii},
	{port_mode_100_mii,     port_mode_butt,         port_mode_100_rgmii},
	{port_mode_butt,        port_mode_butt,         port_mode_1000_rgmii}
};
#else
static int g_speed_portmode_table[speed_mode_butt][interface_mode_butt] = {
	{port_mode_10_mii,      port_mode_10_rmii,      port_mode_10_rgmii},
	{port_mode_100_mii,     port_mode_100_rmii,     port_mode_100_rgmii},
	{port_mode_butt,        port_mode_butt,         port_mode_1000_rgmii}
};
#endif

static int calculate_port_mode(enum speed_mode speed, enum if_mode if_mode,
		int is_duplex_half)
{
	int ret = port_mode_butt ;

	if (speed < speed_mode_butt && if_mode < interface_mode_butt) {
		ret = g_speed_portmode_table[speed][if_mode];
		if (is_duplex_half)
			ret &= ~(1 << 4); /* see mac_if reg def. */
		return ret;
	}

	printf("Invalid speed(%d) & interface(%d) mode.\n", speed, if_mode);
	printf("Please assign which mode our eth will correctly work at.\n"
			"It may support 10M/100M MII, 10M/100M RMII, "
			"10M/100M/1000M RGMII interface.\n"
			"eg. if your board have two NICs, connecting each phy using "
			"mii and rgmii interface, you can use the module param "
			"'port_mode=mii,rgmii' to tell the driver.\n");
	BUG();

	/* can't reach here */
	return -1;
}

/* module parameter
 * 1) phy_intf=mii,rgmii if eth0=mii, eth1=rgmii
 *	phy_intf=mii if eth0=mii, eth1=mii;
 * 2) use_internal_phy=1 if eth1 use internal phy;
 * 3) phy0_addr=1, phy1_addr=2
 * 4) gmac_debug=1
 */
static void parse_module_parameters(void)
{
	int tmp, gmac;
	char *s;

	s = getenv("gmac_debug");
	if (s)
		gmac_debug = simple_strtoul(s, 0, 0);/* debug on */
	else
		gmac_debug = 0;

	s = getenv("phy_link_times");
	if (s)
		phy_link_times = simple_strtoul(s, 0, 0);
	else
		phy_link_times = DEFAULT_PHY_LINK_TIMES;

	s = getenv("ethact");
	if (s)
		gmac_use = simple_strtoul(s, NULL, 0);

	if (gmac_use >= CONFIG_GMAC_NUMS)
		gmac_use = -1;

	BUILD_BUG_ON((int)interface_mode_mii != (int)ETH_PHY_MII);
	BUILD_BUG_ON((int)interface_mode_rmii != (int)ETH_PHY_RMII);
	BUILD_BUG_ON((int)interface_mode_rgmii != (int)ETH_PHY_RGMII);

	higmac_board_info[0].phy_intf = get_eth_phyintf(0, ETH_PHY_MII);
	higmac_board_info[1].phy_intf = get_eth_phyintf(1, ETH_PHY_RGMII);
#if (CONFIG_GMAC_NUMS > 2)
	higmac_board_info[2].phy_intf = get_eth_phyintf(2, ETH_PHY_RGMII);
#endif
#if defined(CONFIG_ARCH_HI3798MV2X) || defined(CONFIG_ARCH_HI3796MV2X)
	higmac_board_info[0].phy_intf = get_eth_phyintf(1, ETH_PHY_MII);
#endif

	tmp = get_eth_phymdio(0, 0);
	higmac_board_info[0].mii_name = mdio_bus[tmp];

	tmp = get_eth_phymdio(1, 1);
	higmac_board_info[1].mii_name = mdio_bus[tmp];

#if (CONFIG_GMAC_NUMS > 2)
	higmac_board_info[2].mii_name = mdio_bus[2];
#endif

	higmac_board_info[0].phy_addr = get_eth_phyaddr(0, INVALID_PHY_ADDR);
	higmac_board_info[1].phy_addr = get_eth_phyaddr(1, INVALID_PHY_ADDR);
#if (CONFIG_GMAC_NUMS > 2)
	higmac_board_info[2].phy_addr = get_eth_phyaddr(2, INVALID_PHY_ADDR);
#endif
#if defined(CONFIG_ARCH_HI3798MV2X) || defined(CONFIG_ARCH_HI3796MV2X)
	higmac_board_info[0].phy_addr = get_eth_phyaddr(1, INVALID_PHY_ADDR);
#endif
	get_eth_phygpio(0, &higmac_board_info[0].gpio_base,
			&higmac_board_info[0].gpio_bit);
	get_eth_phygpio(1, &higmac_board_info[1].gpio_base,
			&higmac_board_info[1].gpio_bit);
#if (CONFIG_GMAC_NUMS > 2)
	get_eth_phygpio(2, &higmac_board_info[2].gpio_base,
			&higmac_board_info[2].gpio_bit);
#endif

	if (gmac_debug) {
		printf("phy_link_times=%d\n", phy_link_times);
		for (gmac = 0; gmac < CONFIG_GMAC_NUMS; gmac++) {
			printf("ETH%d: phyintf=%s, phymdio=%s, phyaddr=%d, "
				"gpio_base=0x%x, gpio_bit=0x%x\n",
				gmac, phy_intf_str[higmac_board_info[gmac].phy_intf],
				higmac_board_info[gmac].mii_name,
				higmac_board_info[gmac].phy_addr,
				higmac_board_info[gmac].gpio_base,
				higmac_board_info[gmac].gpio_bit);
		}
		printf("\n");
	}
}

static unsigned long rstate = 1;
void random_ether_addr(unsigned char *mac)
{
	unsigned long ethaddr_low, ethaddr_high;

	srand((unsigned int)get_timer(0) );
	/*
	 * setting the 2nd LSB in the most significant byte of
	 * the address makes it a locally administered ethernet
	 * address
	 */
	ethaddr_high = (rand() & 0xfeff) | 0x0200;
	ethaddr_low = rand();

	mac[0] = ethaddr_high >> 8;
	mac[1] = ethaddr_high & 0xff;
	mac[2] = ethaddr_low >>24;
	mac[3] = (ethaddr_low >> 16) & 0xff;
	mac[4] = (ethaddr_low >> 8) & 0xff;
	mac[5] = ethaddr_low & 0xff;

	mac [0] &= 0xfe;    /* clear multicast bit */
	mac [0] |= 0x02;    /* set local assignment bit (IEEE802) */

}

static int higmac_net_set_mac_address(struct higmac_netdev_local* ld)
{
	unsigned char mac[MAC_LEN];
	int ret;

	ret = eth_getenv_enetaddr("ethaddr", mac);
	if (!ret) {
		unsigned char ethaddr[20];

		random_ether_addr(mac);

		snprintf((char *)ethaddr, sizeof(ethaddr),"%02X:%02X:%02X:%02X:%02X:%02X",
				mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
		ethaddr[sizeof(ethaddr) - 1] = '\0';
		setenv("ethaddr",(char *)ethaddr);
	}

	higmac_hw_set_macaddress(ld, mac);
	print_mac(mac);

	return 0;
}

static int higmac_init_hw_desc_queue(struct higmac_netdev_local *ld)
{
	higmac_desc* queue_phy_addr = NULL;
	int queue_count = 0;

	/* init rx fq */
	queue_count = HIGMAC_HWQ_RX_FQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if (queue_phy_addr == NULL)
	{
		printf("alloc rx fq error!\n");
		goto _error_alloc_rx_fq;
	}

	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));
	ld->rx_fq_addr = queue_phy_addr;
	higmac_set_rx_fq_hwq_addr(ld, (unsigned int)queue_phy_addr);

	/* init rx bq */
	queue_count = HIGMAC_HWQ_RX_BQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if (queue_phy_addr == NULL)
	{
		printf("alloc rx bq error!\n");
		goto _error_alloc_rx_bq;
	}

	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));
	ld->rx_bq_addr = queue_phy_addr;
	higmac_set_rx_bq_hwq_addr(ld, (unsigned int)queue_phy_addr);

	/* init tx bq */
	queue_count = HIGMAC_HWQ_TX_BQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if (queue_phy_addr == NULL)
	{
		printf("alloc tx bq error!\n");
		goto _error_alloc_tx_bq;
	}

	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));
	ld->tx_bq_addr = queue_phy_addr;
	higmac_set_tx_bq_hwq_addr(ld, (unsigned int)queue_phy_addr);

	/* init tx rq */
	queue_count = HIGMAC_HWQ_TX_RQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if (queue_phy_addr == NULL)
	{
		printf("alloc tx rq error!\n");
		goto _error_alloc_tx_rq;
	}

	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));
	ld->tx_rq_addr = queue_phy_addr;
	higmac_set_tx_rq_hwq_addr(ld, (unsigned int)queue_phy_addr);

	return 0;

_error_alloc_tx_rq:
	free(ld->tx_bq_addr);

_error_alloc_tx_bq:
	free(ld->rx_bq_addr);

_error_alloc_rx_bq:
	free(ld->rx_fq_addr);

_error_alloc_rx_fq:
	return -1;
}

#if 0
static void higmac_destroy_hw_desc_queue(struct higmac_netdev_local* ld)
{
	free(ld->rx_fq_addr);
	free(ld->rx_bq_addr);
	free(ld->tx_bq_addr);
	free(ld->tx_rq_addr);

	ld->rx_fq_addr = NULL;
	ld->rx_bq_addr = NULL;
	ld->tx_bq_addr = NULL;
	ld->tx_rq_addr = NULL;
}
#endif

#define PHY_ID_KSZ8051		0x00221550
#define PHY_ID_KSZ8081		0x00221560
#define PHY_ID_KSZ9031		0x00221620
#define PHY_ID_MASK		0xFFFFFFF0
#define ATH8035_PHY_ID		0x004dd072
#define ATH_PHY_ID_MASK		0xffffffef

/* MMD: MDIO Manageable Device */
#define MACR			0x0D
#define MAADR			0x0E
static void phy_mmd_read(char *devname, u8 phyaddr,
			u16 mmd_device, u8 regnum, u16 *val)
{
	miiphy_write(devname, phyaddr, MACR, mmd_device);
	miiphy_write(devname, phyaddr, MAADR, regnum);
	miiphy_write(devname, phyaddr, MACR, 0x4000 | mmd_device);

	miiphy_read(devname, phyaddr, MAADR, val);
}

static void phy_mmd_write(char *devname, u8 phyaddr,
			u16 mmd_device, u8 regnum, u16 val)
{
	miiphy_write(devname, phyaddr, MACR, mmd_device);
	miiphy_write(devname, phyaddr, MAADR, regnum);
	miiphy_write(devname, phyaddr, MACR, 0x4000 | mmd_device);

	miiphy_write(devname, phyaddr, MAADR, val);
}

#define EXP_ADDR	0x1d
#define EXP_DATA	0x1e
static void phy_expand_read(char *devname, u8 phyaddr, u8 regnum, u16 *val)
{
	miiphy_write(devname, phyaddr, EXP_ADDR, regnum);
	miiphy_read(devname, phyaddr, EXP_DATA, val);
}

static void phy_expand_write(char *devname, u8 phyaddr, u8 regnum, u16 val)
{
	miiphy_write(devname, phyaddr, EXP_ADDR, regnum);
	miiphy_write(devname, phyaddr, EXP_DATA, val);
}

static int phy_fixup(char *devname, unsigned char phyaddr)
{
	u32 phy_id;
	u16 id1, id2;

	if (miiphy_read(devname, phyaddr, PHY_PHYIDR1, &id1) != 0) {
		printf("PHY IDR1 read failed\n");
		return -1;
	};
	if (miiphy_read(devname, phyaddr, PHY_PHYIDR2, &id2) != 0) {
		printf("PHY IDR2 read failed\n");
		return -1;
	};

	phy_id = (id1 & 0xffff) << 16;
	phy_id |= (id2 & 0xffff);

	/* If the phy_id is all Fs, there is no device there */
	if (0xffffffff == phy_id || 0 == phy_id
			|| phy_id == 0xFFFF || phy_id == 0xFFFF0000) {
		return -1;
	}

	/* PHY-KSZ8051 */
	if ((phy_id & PHY_ID_MASK) == PHY_ID_KSZ8051) {
		u16 val;

		if (miiphy_read(devname, phyaddr, 0x1F, &val) != 0) {
			printf("PHY reg read failed\n");
			return -1;
		};
		val |= (1 << 7);       /* set phy RMII 50MHz clk; */
		if (miiphy_write(devname, phyaddr, 0x1F, val) != 0)
			return -1;

		if (miiphy_read(devname, phyaddr, 0x16, &val) != 0) {
			printf("PHY reg read failed\n");
			return -1;
		};
		val |= (1 << 1);       /* set phy RMII override; */
		if (miiphy_write(devname, phyaddr, 0x16, val) != 0)
			return -1;
	}

	/* PHY-KSZ8081 */
	if ((phy_id & PHY_ID_MASK) == PHY_ID_KSZ8081) {
		u16 val;

		if (miiphy_read(devname, phyaddr, 0x1F, &val) != 0) {
			printf("PHY IDR1 read failed\n");
			return -1;
		};
		val |= (1 << 7);       /* set phy RMII 50MHz clk; */
		if (miiphy_write(devname, phyaddr, 0x1F, val) != 0)
			return -1;
	}

	/* PHY-KSZ9031 */
	if ((phy_id & PHY_ID_MASK) == PHY_ID_KSZ9031) {
		u16 val;

		/* RX_CLK Pad Skew: 1_1101(+0.84) */
		phy_mmd_read(devname, phyaddr, 0x2, 0x8, &val);
		val = (val & ~0x1F) | 0x1D;
		phy_mmd_write(devname, phyaddr, 0x2, 0x8, val);
	}

	/* PHY-AR8035 */
	if ((phy_id & ATH_PHY_ID_MASK) == (ATH8035_PHY_ID & ATH_PHY_ID_MASK)) {
		u16 tx_delay;

		/* enable rgmii tx clock delay */
		phy_expand_read(devname, phyaddr, 0x05, &tx_delay);
		tx_delay |= (1 << 8);
		phy_expand_write(devname, phyaddr, 0x05, tx_delay);
	}

	return 0;
}

static int higmac_net_adjust_link(struct higmac_netdev_local* ld)
{
	char *mii_name = higmac_board_info[ld->index].mii_name;
	int phy_addr = higmac_board_info[ld->index].phy_addr;
	int stat = 0, speed = 0, is_duplex_half = 1, port_mode, phy_duplex;
	enum speed_mode speed_mode;

	if (phy_addr == INVALID_PHY_ADDR)
		return stat;

	phy_fixup(mii_name, phy_addr);

	stat |= miiphy_link(mii_name, phy_addr) ? HIGMAC_LINKED : 0;

	phy_duplex = miiphy_duplex(mii_name, phy_addr);
	if (phy_duplex == FULL) {
		stat |= HIGMAC_DUP_FULL;
		is_duplex_half = 0;
	}

	speed = miiphy_speed(mii_name, phy_addr);
	switch (speed) {
	case _10BASET:
		stat |= HIGMAC_SPD_10M;
		speed_mode = speed_mode_10M;
		break;
	default:
		printf("wired, phy speed!\n");
	case _100BASET:
		stat |= HIGMAC_SPD_100M;
		speed_mode = speed_mode_100M;
		break;
	case _1000BASET:
		stat |= HIGMAC_SPD_1000M;
		speed_mode = speed_mode_1000M;
		break;
	}

	if (ld->link_stat != stat) {
		if (stat & HIGMAC_LINKED) {
			port_mode = calculate_port_mode(speed_mode,
				higmac_board_info[ld->index].phy_intf,
				is_duplex_half);

			higmac_set_macif(ld, port_mode, speed_mode);
#ifndef CONFIG_S40_FPGA
			if (is_duplex_half)
				higmac_writel(ld, 0x0, MAC_DUPLEX_HALF_CTRL);
			else
				higmac_writel(ld, 0x1, MAC_DUPLEX_HALF_CTRL);
#endif
		}
		ld->link_stat = stat;
	}

	return stat;
}

static int select_current_linked_phy(void)
{
	int count = phy_link_times ? : DEFAULT_PHY_LINK_TIMES;
	struct higmac_netdev_local *ld;
	int status, i;

	for (; count > 0; count--) {
		if (ctrlc()) {
			puts ("\nAbort\n");
			goto link_failed;
		}

		for (i = 0; i < CONFIG_GMAC_NUMS; i++) {
			if (gmac_use >= 0 && gmac_use != i)
				continue;
			ld = &higmac_board_info[i].higmac_ld;

			status = higmac_net_adjust_link(ld);
			if (status & HIGMAC_LINKED)
				goto link_on;
		}
		udelay(1);
	}

link_failed:
	for (i = 0; i < CONFIG_GMAC_NUMS; i++) {
		if (gmac_use >= 0 && gmac_use != i)
			continue;
		ld = &higmac_board_info[i].higmac_ld;
		printf("ETH%d: PHY(phyaddr=%d, %s) not link!\n",
			i, higmac_board_info[i].phy_addr,
			phy_intf_str[higmac_board_info[i].phy_intf]);
	}

	return -1;

link_on:
	printf("ETH%d: PHY(phyaddr=%d, %s) link UP: DUPLEX=%s : SPEED=%s\n",
			ld->index, higmac_board_info[ld->index].phy_addr,
			phy_intf_str[higmac_board_info[ld->index].phy_intf],
			(status & HIGMAC_DUP_FULL) ? "FULL" : "HALF",
			(status & HIGMAC_SPD_100M) ? "100M" :
			((status & HIGMAC_SPD_10M) ? "10M" : "1000M"));

	higmac_net_set_mac_address(ld);
	miiphy_set_current_dev(higmac_board_info[ld->index].mii_name);
	current_ld = ld;

	return 0;
}

int eth_rx(void)
{
	struct higmac_netdev_local* ld = current_ld;
	int timeout_us = 100000;
	int rx_fq_wr_offset = 0;
	int rx_fq_rd_offset = 0;
	int rx_bq_wr_offset = 0;
	int rx_bq_rd_offset = 0;
	int len = 0;
	int wr_rd_dist;
	int i;
	higmac_desc* rx_fq_desc= ld->rx_fq_addr;
	higmac_desc* rx_bq_desc= ld->rx_bq_addr;

	rx_fq_wr_offset = higmac_readl_bits(ld, RX_FQ_WR_ADDR, BITS_RX_FQ_WR_ADDR);
	rx_fq_rd_offset = higmac_readl_bits(ld, RX_FQ_RD_ADDR, BITS_RX_FQ_RD_ADDR);

	if (rx_fq_wr_offset >= rx_fq_rd_offset)
		wr_rd_dist = (HIGMAC_HWQ_RX_FQ_DEPTH << DESC_BYTE_SHIFT)
			- (rx_fq_wr_offset - rx_fq_rd_offset);
	else
		wr_rd_dist = rx_fq_rd_offset - rx_fq_wr_offset;

	wr_rd_dist >>= DESC_BYTE_SHIFT;/* offset was counted on bytes, desc size = 2^5 */

	/*
	 * wr_rd_dist - 1 for logic reason.
	 * Logic think the desc pool is full filled, ...?
	 */
	for (i = 0; i < wr_rd_dist - 1; i++) {
		rx_fq_desc = ld->rx_fq_addr + (rx_fq_wr_offset >> DESC_BYTE_SHIFT);
		rx_fq_desc->data_buff_addr = (unsigned int)memalign(64, HIETH_MAX_FRAME_SIZE);
		if ((void *)rx_fq_desc->data_buff_addr == NULL)
			break;
		rx_fq_desc->data_buff_addr += NET_IP_ALIGN;
		rx_fq_desc->descvid = DESC_VLD_FREE;
		rx_fq_desc->buffer_len = (HIETH_MAX_FRAME_SIZE - 1);
		rx_fq_desc->data_len = 8;

		rx_fq_wr_offset += DESC_SIZE;
		if (rx_fq_wr_offset >= (HIGMAC_HWQ_RX_FQ_DEPTH << DESC_BYTE_SHIFT))
			rx_fq_wr_offset = 0;
		higmac_writel_bits(ld, rx_fq_wr_offset, RX_FQ_WR_ADDR, BITS_RX_FQ_WR_ADDR);
	}

	/* enable Rx */
	//higmac_desc_enable(ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);
	//higmac_writel_bits(ld, 1, PORT_EN, BITS_RX_EN);

	rx_bq_wr_offset = higmac_readl_bits(ld, RX_BQ_WR_ADDR, BITS_RX_BQ_WR_ADDR);
	rx_bq_rd_offset = higmac_readl_bits(ld, RX_BQ_RD_ADDR, BITS_RX_BQ_RD_ADDR);
	rx_bq_desc += (rx_bq_rd_offset >> DESC_BYTE_SHIFT);

	while(--timeout_us && (rx_bq_wr_offset == rx_bq_rd_offset))
	{
		rx_bq_wr_offset = higmac_readl_bits(ld, RX_BQ_WR_ADDR, BITS_RX_BQ_WR_ADDR);
		udelay(1);
	}

	if (! timeout_us) {
		//printf("rx time out!\n");
		return -1;
	}

	rx_bq_rd_offset += DESC_SIZE;
	if (rx_bq_rd_offset >= (HIGMAC_HWQ_RX_BQ_DEPTH << DESC_BYTE_SHIFT))
		rx_bq_rd_offset = 0;

	len = rx_bq_desc->data_len;
	if (HIGMAC_INVALID_RXPKG_LEN(len)) {
		higmac_writel_bits(ld, rx_bq_rd_offset, RX_BQ_RD_ADDR, BITS_RX_BQ_RD_ADDR);
		free((void *)(rx_bq_desc->data_buff_addr - NET_IP_ALIGN));
		return -1;
	}
	if (gmac_debug)
		printf("got packet!\n");

	memcpy((void *)NetRxPackets[0], (void *)rx_bq_desc->data_buff_addr, len);
	free((void *)(rx_bq_desc->data_buff_addr - NET_IP_ALIGN));

	higmac_writel_bits(ld, rx_bq_rd_offset, RX_BQ_RD_ADDR, BITS_RX_BQ_RD_ADDR);

	/*NetRecive*/
	NetReceive(NetRxPackets[0], len);

	/* disable Rx */
	//higmac_writel_bits(ld, 0, PORT_EN, BITS_RX_EN);
	//higmac_desc_disable(ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);

	return 0;
}

int eth_send(volatile void* packet, int length)
{
	struct higmac_netdev_local* ld = current_ld;
	int timeout_us = 1000;
	int tx_bq_wr_offset = 0;
	int tx_bq_rd_offset = 0;
	int tx_rq_wr_offset = 0;
	int tx_rq_rd_offset = 0;
	unsigned int tso_ver = 0;

	higmac_desc* tx_bq_desc= ld->tx_bq_addr;

	tx_bq_wr_offset = higmac_readl_bits(ld, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);
	tx_bq_rd_offset = higmac_readl_bits(ld, TX_BQ_RD_ADDR, BITS_TX_BQ_RD_ADDR);

	if (tx_bq_rd_offset != tx_bq_wr_offset)
	{
		higmac_writel_bits(ld, tx_bq_rd_offset, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);
		//printf("tx bq is error!\n");
		return -1;
	}

	tso_ver = higmac_readl_bits(ld, CRF_MIN_PACKET, BIT_TSO_VERSION);

	tx_bq_desc += (tx_bq_wr_offset >> DESC_BYTE_SHIFT);
	tx_bq_desc->data_buff_addr =(unsigned int)packet;
	tx_bq_desc->descvid = DESC_VLD_BUSY;

	if (tso_ver) {
		tx_bq_desc->fl = 0;
	} else
		tx_bq_desc->fl = DESC_FL_FULL;
	tx_bq_desc->reserve1 = 0;

	tx_bq_desc->data_len = length;
	tx_bq_desc->buffer_len =  (HIETH_MAX_FRAME_SIZE - 1);;

	tx_bq_wr_offset += DESC_SIZE;
	if (tx_bq_wr_offset >= (HIGMAC_HWQ_TX_BQ_DEPTH << DESC_BYTE_SHIFT))
		tx_bq_wr_offset = 0;
	higmac_writel_bits(ld, tx_bq_wr_offset, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);

	/* enable tx */
	//higmac_writel_bits(ld, 1, PORT_EN, BITS_TX_EN);
	//higmac_desc_enable(ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);

	tx_rq_wr_offset = higmac_readl_bits(ld, TX_RQ_WR_ADDR, BITS_TX_RQ_WR_ADDR);
	tx_rq_rd_offset = higmac_readl_bits(ld, TX_RQ_RD_ADDR, BITS_TX_RQ_RD_ADDR);
	tx_rq_rd_offset += DESC_SIZE;
	if (tx_rq_rd_offset >= (HIGMAC_HWQ_TX_RQ_DEPTH << DESC_BYTE_SHIFT))
		tx_rq_rd_offset = 0;

	while(--timeout_us && (tx_rq_rd_offset != tx_rq_wr_offset))
	{
		tx_rq_wr_offset = higmac_readl_bits(ld, TX_RQ_WR_ADDR, BITS_TX_RQ_WR_ADDR);
		udelay(1);
	}

	if (!timeout_us)
	{
		//printf("send time out!\n");
		return -1;
	}

	higmac_writel_bits(ld, tx_rq_rd_offset, TX_RQ_RD_ADDR, BITS_TX_RQ_RD_ADDR);
	if (gmac_debug)
		printf("send packet!\n");
	/* disable tx */
	//higmac_desc_disable(ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	//higmac_writel_bits(ld, 0, PORT_EN, BITS_TX_EN);

	return 0;
}

static void higmac_phy_gpio_reset(void)
{
	u32 gpio_base, gpio_bit, v;
	int i;
#define RESET_DATA	(1)

	for (i = 0; i < CONFIG_GMAC_NUMS; i++) {
		gpio_base = higmac_board_info[i].gpio_base;
		gpio_bit = higmac_board_info[i].gpio_bit;

		if (!gpio_base)
			continue;

		/* config gpio[x] dir to output */
		v = readb(gpio_base + 0x400);
		v |= (1 << gpio_bit);
		writeb(v, gpio_base + 0x400);

		/* output 1--0--1 */
		writeb(RESET_DATA << gpio_bit,
				gpio_base + (4 << gpio_bit));
		udelay(20000);
		writeb((!RESET_DATA) << gpio_bit,
				gpio_base + (4 << gpio_bit));
		udelay(20000);
		writeb(RESET_DATA << gpio_bit,
				gpio_base + (4 << gpio_bit));
	}
}

static int gmac_inited;
static int higmac_init(void)
{
	int ret = 0;
	struct higmac_netdev_local *ld;
	int i;

	/* init once to save time */
	if (!gmac_inited) {
		gmac_inited = 1;

		higmac_phy_gpio_reset();
		higmac_sys_init();

		for (i = 0; i < CONFIG_GMAC_NUMS; i++)
			miiphy_register(mdio_bus[i], higmac_mdiobus_read,
					higmac_mdiobus_write);

		miiphy_reset(higmac_board_info[0].mii_name,
				higmac_board_info[0].phy_addr);
		miiphy_reset(higmac_board_info[1].mii_name,
				higmac_board_info[1].phy_addr);
#if (CONFIG_GMAC_NUMS > 2)
		miiphy_reset(higmac_board_info[2].mii_name,
				higmac_board_info[2].phy_addr);
#endif

		if (higmac_board_info[0].phy_intf != interface_mode_rgmii) {
			unsigned short val = 0;
			if (0 == miiphy_read(higmac_board_info[0].mii_name,
				higmac_board_info[0].phy_addr, PHY_1000BTCR, &val)) {

				val &= ~(PHY_1000BTCR_1000FD | PHY_1000BTCR_1000HD);

				miiphy_write(higmac_board_info[0].mii_name,
					higmac_board_info[0].phy_addr, PHY_1000BTCR, val);
			}
		}

		if (higmac_board_info[1].phy_intf != interface_mode_rgmii) {
			unsigned short val = 0;
			if (0 == miiphy_read(higmac_board_info[1].mii_name,
				higmac_board_info[1].phy_addr, PHY_1000BTCR, &val)) {

				val &= ~(PHY_1000BTCR_1000FD | PHY_1000BTCR_1000HD);

				miiphy_write(higmac_board_info[1].mii_name,
					higmac_board_info[1].phy_addr, PHY_1000BTCR, val);
			}
		}
#if (CONFIG_GMAC_NUMS > 2)
		if (higmac_board_info[2].phy_intf != interface_mode_rgmii) {
			unsigned short val = 0;
			if (0 == miiphy_read(higmac_board_info[2].mii_name,
				higmac_board_info[2].phy_addr, PHY_1000BTCR, &val)) {

				val &= ~(PHY_1000BTCR_1000FD | PHY_1000BTCR_1000HD);

				miiphy_write(higmac_board_info[2].mii_name,
					higmac_board_info[2].phy_addr, PHY_1000BTCR, val);
			}
		}
#endif
		for_each_gmac_netdev_local_priv(ld, i) {
			higmac_glb_preinit_dummy(ld);

			ret = higmac_set_hwq_depth(ld);
			if (ret) {
				printf("init eth%d hw desc queue depth fail!\n", i);
				return ret;
			}

			ret = higmac_init_hw_desc_queue(ld);
			if (ret) {
				printf("init eth%d hw desc queue fail!\n", i);
				return ret;
			}
		}
	}

	ret = select_current_linked_phy();
	if (ret)
		return ret;

	higmac_desc_enable(current_ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);
	higmac_writel_bits(current_ld, 1, PORT_EN, BITS_RX_EN);

	higmac_writel_bits(current_ld, 1, PORT_EN, BITS_TX_EN);
	higmac_desc_enable(current_ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);

	return 0;
}

static void higmac_exit(void)
{
	higmac_writel_bits(current_ld, 0, PORT_EN, BITS_RX_EN);
	higmac_desc_disable(current_ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);

	higmac_desc_disable(current_ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	higmac_writel_bits(current_ld, 0, PORT_EN, BITS_TX_EN);

#if 0	/* no need to init each time */
	higmac_glb_preinit_dummy(current_ld);

	higmac_destroy_hw_desc_queue(current_ld);
#endif
}

int eth_init(bd_t* bd)
{
	int ret;

	parse_module_parameters();

	ret = higmac_init();
	if (ret)
		printf("higmac init fail!\n");

	return ret;
}

void eth_halt(void)
{
	if (current_ld && current_ld->iobase)
		higmac_exit();
}
