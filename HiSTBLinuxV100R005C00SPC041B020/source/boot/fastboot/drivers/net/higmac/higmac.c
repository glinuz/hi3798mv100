#include "util.h"
#include "higmac.h"
#include "ctrl.h"
#include "mdio.h"


struct higmac_netdev_local g_higmac_ld ;
static interface_mode_e g_interface_mode = interface_mode_mii;

char PHY_NAME[MAX_PHY_NAME_LEN] = "0:01";
unsigned char PHY_ADDR = 0x01;

void string_to_mac(unsigned char *mac, char* s)
{
	int i;
	char *e;

	for (i=0; i<MAC_LEN; ++i) 
	{
		mac[i] = s ? simple_strtoul(s, &e, 16) : 0;
		if (s)
		{
			s = (*e) ? e+1 : e;
		}
	}
}

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

	mac[0] = ethaddr_high >> 8;
	mac[1] = ethaddr_high & 0xff;
	mac[2] = ethaddr_low >>24;
	mac[3] = (ethaddr_low >> 16) & 0xff;
	mac[4] = (ethaddr_low >> 8) & 0xff;
	mac[5] = ethaddr_low & 0xff;

	mac [0] &= 0xfe;    /* clear multicast bit */
	mac [0] |= 0x02;    /* set local assignment bit (IEEE802) */

}


static int set_random_mac_address(unsigned char * mac, unsigned char * ethaddr)
{
	random_ether_addr(mac);

	sprintf (ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
	        mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	setenv("ethaddr",ethaddr);
	print_mac(mac);

	return 0;
}

int eth_set_host_mac_address(void)
{
	char* s;
	unsigned char mac[MAC_LEN];
	unsigned char ethaddr[20];
	s=getenv("ethaddr");
	if(!s)
	{
		set_random_mac_address(mac, ethaddr);
		return 0;
	}

	string_to_mac(mac,s);
	if(!is_valid_ether_addr(mac))
	{
		set_random_mac_address(mac, ethaddr);
		return 0;
	}
	return 0;
}

static int higmac_net_set_mac_address(struct higmac_netdev_local* ld)
{
	char* s;
	unsigned char mac[MAC_LEN];
	s = getenv("ethaddr");
	if (s)
	{
		string_to_mac(mac,s);
	}
	else
	{
		random_ether_addr(mac);
	}
	
	if (!is_valid_ether_addr(mac))
	{
		random_ether_addr(mac);
	}
	print_mac(mac);

	higmac_hw_set_macaddress(ld, mac);

	return 0;
}

static int higmac_init_hw_desc_queue(struct higmac_netdev_local *ld)
{
	higmac_desc* queue_phy_addr = NULL;
	int queue_count = 0;

	/* init rx fq */
	queue_count = HIGMAC_HWQ_RX_FQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if(queue_phy_addr == NULL)
	{
		higmac_error("alloc rx fq error!\n");
		goto _error_alloc_rx_fq;
	}
		
	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));	
	ld->rx_fq_addr = queue_phy_addr;
	higmac_set_rx_fq_hwq_addr(ld, (void *)queue_phy_addr);

	/* init rx bq */
	queue_count = HIGMAC_HWQ_RX_BQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if(queue_phy_addr == NULL)
	{
		higmac_error("alloc rx bq error!\n");
		goto _error_alloc_rx_bq;
	}
	
	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));	
	ld->rx_bq_addr = queue_phy_addr;
	higmac_set_rx_bq_hwq_addr(ld, (void *)queue_phy_addr);

	/* init tx bq */
	queue_count = HIGMAC_HWQ_TX_BQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if(queue_phy_addr == NULL)
	{
		higmac_error("alloc tx bq error!\n");
		goto _error_alloc_tx_bq;
	}
	
	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));	
	ld->tx_bq_addr = queue_phy_addr;
	higmac_set_tx_bq_hwq_addr(ld, (void *)queue_phy_addr);

	/* init tx rq */
	queue_count = HIGMAC_HWQ_TX_RQ_DEPTH;

	queue_phy_addr= (higmac_desc*)malloc(queue_count * sizeof(higmac_desc));
	if(queue_phy_addr == NULL)
	{
		higmac_error("alloc tx rq error!\n");
		goto _error_alloc_tx_rq;
	}
	
	memset((void*)queue_phy_addr, 0, queue_count * sizeof(higmac_desc));	
	ld->tx_rq_addr = queue_phy_addr;
	higmac_set_tx_rq_hwq_addr(ld, (void *)queue_phy_addr);

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

static speed_port_mode_match_value calculate_port_mode(speed_mode_e speed, interface_mode_e interface)
{
	speed_port_mode_match_value ret = port_mode_butt ;

	if ((speed < speed_mode_butt) && (interface < interface_mode_butt))
		ret = g_speed_portmode_table[speed][interface];

	if (ret == port_mode_butt)
	{
		higmac_error("Invalid speed(%d) & interface(%d) matach.\n", speed, interface);
		higmac_error("Please assign which mode our eth module will correctly work at.\n\
			Currently we support 10M MII, 100M MII, 1000M GMII, 10M RGMII, 100M RGMII, 1000M RGMII interface.\n\
			eg: If your board connected by MII interface, please set 10M MII or 100M MII,using module param 'port_mode'.\
			For logic limited, some speed mode will not work with some *MII interface, eg: 10M GMII, 100M GMII, these will not work yet.\n");
		BUG();
	}
	return ret;	
}

static void higmac_net_adjust_link(struct higmac_netdev_local* ld)
{
	int stat = 0,speed = 0;
	speed_port_mode_match_value match;

	stat |= miiphy_link(ld->phy_name, PHY_ADDR) ? HIGMAC_LINKED : 0;
	stat |= miiphy_duplex(ld->phy_name, PHY_ADDR) ? HIGMAC_DUP_FULL : 0;
	speed = miiphy_speed(ld->phy_name, PHY_ADDR);
	stat |= (speed == _100BASET ? HIGMAC_SPD_100M : 
	(speed == _10BASET? HIGMAC_SPD_10M : HIGMAC_SPD_1000M));

	if (ld->link_stat != stat)
	{
		/*calculate port mode.*/
		match = calculate_port_mode(
				((stat & HIGMAC_SPD_100M) ? speed_mode_100M : 
				 ((stat & HIGMAC_SPD_10M) ? speed_mode_10M : speed_mode_1000M)),
				g_interface_mode);

		higmac_set_linkstat(ld, match);	

		ld->link_stat = stat;
	}
	
	return;
}

int eth_rx(void)
{
	struct higmac_netdev_local* ld = &g_higmac_ld;
	int timeout_us = 100000;
	int rx_fq_wr_offset = 0;
	int rx_fq_rd_offset = 0;
	int rx_bq_wr_offset = 0;
	int rx_bq_rd_offset = 0;
	int len = 0;
	higmac_desc* rx_fq_desc= ld->rx_fq_addr;
	higmac_desc* rx_bq_desc= ld->rx_bq_addr;

	rx_fq_wr_offset = higmac_readl_bits(ld, RX_FQ_WR_ADDR, BITS_RX_FQ_WR_ADDR);
	rx_fq_rd_offset = higmac_readl_bits(ld, RX_FQ_RD_ADDR, BITS_RX_FQ_RD_ADDR);

	if(rx_fq_wr_offset != rx_fq_rd_offset)
	{
		higmac_writel_bits(ld, rx_fq_rd_offset, RX_FQ_WR_ADDR, BITS_RX_FQ_WR_ADDR);	
		//higmac_error("rx fq is error!\n");
		return -1;		
	}

	rx_fq_desc += (rx_fq_wr_offset>>5);
	rx_fq_desc->data_buff_addr = (unsigned int)NetRxPackets[0];
	rx_fq_desc->descvid = DESC_VLD_FREE;
	rx_fq_desc->buffer_len = (HIETH_MAX_FRAME_SIZE - 1);
	rx_fq_desc->data_len = 8;

	rx_fq_wr_offset = (rx_fq_wr_offset == (HIGMAC_HWQ_RX_FQ_DEPTH - 1)<<5) ? 0 : rx_fq_wr_offset + DESC_SIZE;
	higmac_writel_bits(ld, rx_fq_wr_offset, RX_FQ_WR_ADDR, BITS_RX_FQ_WR_ADDR);

	/* enable Rx */
	//higmac_desc_enable(ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);
	//higmac_writel_bits(ld, 1, PORT_EN, BITS_RX_EN);
	
	rx_bq_wr_offset = higmac_readl_bits(ld, RX_BQ_WR_ADDR, BITS_RX_BQ_WR_ADDR);
	rx_bq_rd_offset = higmac_readl_bits(ld, RX_BQ_RD_ADDR, BITS_RX_BQ_RD_ADDR);
	rx_bq_desc += (rx_bq_rd_offset>>5);	
	
	rx_bq_rd_offset = (rx_bq_rd_offset == (HIGMAC_HWQ_RX_BQ_DEPTH - 1)<<5) ? 0 : rx_bq_rd_offset + DESC_SIZE;

	while(--timeout_us && (rx_bq_wr_offset != rx_bq_rd_offset))
	{
		udelay(1);
		rx_bq_wr_offset = higmac_readl_bits(ld, RX_BQ_WR_ADDR, BITS_RX_BQ_WR_ADDR);
	}

	if(! timeout_us)
	{
		//higmac_error("rx time out!\n");
		return -1;
	}

	len = rx_bq_desc->data_len;
	if(HIGMAC_INVALID_RXPKG_LEN(len))
	{
		//higmac_error("rx packet len error %x\n",len);
		return -1;
	}

	/*NetRecive*/
	NetReceive((char*)NetRxPackets[0], len);

	higmac_writel_bits(ld, rx_bq_rd_offset, RX_BQ_RD_ADDR, BITS_RX_BQ_RD_ADDR);

	/* disable Rx */
	//higmac_writel_bits(ld, 0, PORT_EN, BITS_RX_EN);
	//higmac_desc_disable(ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);
	
	return 0;
}

int eth_send(volatile void* packet, int length)
{
	struct higmac_netdev_local* ld = &g_higmac_ld;
	int timeout_us = 1000;	
	int tx_bq_wr_offset = 0;
	int tx_bq_rd_offset = 0;
	int tx_rq_wr_offset = 0;	
	int tx_rq_rd_offset = 0;
	higmac_desc* tx_bq_desc= ld->tx_bq_addr;
	higmac_desc* tx_rq_desc= ld->tx_rq_addr;

	tx_bq_wr_offset = higmac_readl_bits(ld, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);
	tx_bq_rd_offset = higmac_readl_bits(ld, TX_BQ_RD_ADDR, BITS_TX_BQ_RD_ADDR);

	if(tx_bq_rd_offset != tx_bq_wr_offset)
	{
		higmac_writel_bits(ld, tx_bq_rd_offset, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);
		//higmac_error("tx bq is error!\n");
		return -1;
	}

	tx_bq_desc += (tx_bq_wr_offset>>5);
	tx_bq_desc->data_buff_addr =(unsigned int)packet;
	tx_bq_desc->descvid = DESC_VLD_BUSY;
	tx_bq_desc->fl = DESC_FL_FULL;
	tx_bq_desc->data_len = length;
	tx_bq_desc->buffer_len =  (HIETH_MAX_FRAME_SIZE - 1);;

	tx_bq_wr_offset = (tx_bq_wr_offset == (HIGMAC_HWQ_TX_BQ_DEPTH - 1)<<5) ? 0 : tx_bq_wr_offset + DESC_SIZE;
	higmac_writel_bits(ld, tx_bq_wr_offset, TX_BQ_WR_ADDR, BITS_TX_BQ_WR_ADDR);

	/* enable tx */
	//higmac_writel_bits(ld, 1, PORT_EN, BITS_TX_EN);
	//higmac_desc_enable(ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	
	tx_rq_wr_offset = higmac_readl_bits(ld, TX_RQ_WR_ADDR, BITS_TX_RQ_WR_ADDR);
	tx_rq_rd_offset = higmac_readl_bits(ld, TX_RQ_RD_ADDR, BITS_TX_RQ_RD_ADDR);
	tx_rq_rd_offset = (tx_rq_rd_offset == (HIGMAC_HWQ_TX_RQ_DEPTH - 1)<<5) ? 0 : tx_rq_rd_offset + DESC_SIZE;

	while(--timeout_us && (tx_rq_rd_offset != tx_rq_wr_offset))
	{
		udelay(1);
		tx_rq_wr_offset = higmac_readl_bits(ld, TX_RQ_WR_ADDR, BITS_TX_RQ_WR_ADDR);
	}

	if(! timeout_us)
	{
		//higmac_error("send time out!\n");
		return -1;
	}

	higmac_writel_bits(ld, tx_rq_rd_offset, TX_RQ_RD_ADDR, BITS_TX_RQ_RD_ADDR);

	
	/* disable tx */
	//higmac_desc_disable(ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	//higmac_writel_bits(ld, 0, PORT_EN, BITS_TX_EN);

	return 0;
}

static int higmac_dev_remove(void)
{
	return 0;
}

static int higmac_init(void)
{
	int ret = 0;
	char* mdio_intf;
	char* phyaddr;
	/*get mdio interface from env.FORMAT: mdio_intf=mii or mdio_intf=rgmii*/
	mdio_intf = getenv("mdio_intf");
	if (mdio_intf)
	{
		if((strncmp(mdio_intf,"mii",3) && strncmp(mdio_intf,"rgmii",5)))
		{
			higmac_error("Invalid mdio intface assignment(mii or rgmii).Set default to mii\n");
		}
		else if(!strncmp(mdio_intf,"mii",3))
		{
			g_interface_mode = interface_mode_mii;
		}
		else if(!strncmp(mdio_intf,"rgmii",5))
		{
			g_interface_mode = interface_mode_rgmii;
		}
	}
	/*get phy addr & phy name*/ 
	phyaddr = getenv("phyaddr");
	if(phyaddr)
	{
		unsigned long tmp ;
		tmp = simple_strtoul(phyaddr,0,16);
		if (tmp == 0)
		{
			printf("Dectected phy address set to 0,is it right?\n");
		}
		else if (tmp >= (unsigned long)(~(unsigned char)0))
		{
			printf("Dectected phy address set to >= %lux.this may not correct.\n",tmp);
			return -1;
		}
		PHY_ADDR = (unsigned char)tmp;
		sprintf(PHY_NAME,"0:%2x",PHY_ADDR);
	}

	higmac_sys_init();
	
	ret = higmac_mdiobus_driver_init();
	if (ret)
	{
		higmac_error("Init mdio bus fail!\n ");
		higmac_sys_exit();
		return -1;
	}

	g_higmac_ld.phy_name = PHY_NAME;
	g_higmac_ld.iobase_phys = HIGMAC_IOBASE;
	g_higmac_ld.iobase = HIGMAC_IOBASE;

	higmac_glb_preinit_dummy(&g_higmac_ld);

	ret = higmac_set_hwq_depth(&g_higmac_ld);
	if(ret) 
	{
		higmac_error("init hw desc queue depth fail!\n");

		higmac_mdiobus_driver_exit();
		higmac_sys_exit();
		return -1;
	}
	
	ret = higmac_init_hw_desc_queue(&g_higmac_ld);
	if(ret) 
	{
		higmac_error("init hw desc queue fail!\n");

		higmac_mdiobus_driver_exit();
		higmac_sys_exit();
		return -1;
	}

	higmac_desc_enable(&g_higmac_ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);
	higmac_writel_bits(&g_higmac_ld, 1, PORT_EN, BITS_RX_EN);

	higmac_writel_bits(&g_higmac_ld, 1, PORT_EN, BITS_TX_EN);
	higmac_desc_enable(&g_higmac_ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	
	return 0;
}

static void higmac_exit(void)
{	
	higmac_writel_bits(&g_higmac_ld, 0, PORT_EN, BITS_RX_EN);
	higmac_desc_disable(&g_higmac_ld, RX_OUTCFF_WR_DESC_ENA|RX_CFF_RD_DESC_ENA);

	higmac_desc_disable(&g_higmac_ld, TX_OUTCFF_WR_DESC_ENA|TX_CFF_RD_DESC_ENA);
	higmac_writel_bits(&g_higmac_ld, 0, PORT_EN, BITS_TX_EN);
	
	higmac_glb_preinit_dummy(&g_higmac_ld);
	
	higmac_destroy_hw_desc_queue(&g_higmac_ld);
	higmac_mdiobus_driver_exit();
	higmac_sys_exit();
}

int eth_init(bd_t* bd)
{
	int ret;
	int count = 10000;
	char* timeout;
	timeout = getenv("phy_link_time");
	if(timeout)
	{
		count = simple_strtoul(timeout, 0, 10);
	}
	ret = higmac_init();
	if (ret)
	{
		higmac_error("gmac:Init fail!\n");
		return ret;
	}
	
retry:
	higmac_net_adjust_link(&g_higmac_ld);	
	if (g_higmac_ld.link_stat & HIGMAC_LINKED)
		goto link_on;
	if (--count)
		goto retry;

	printf("PHY not link!\n");
	higmac_exit();
	return -1;
link_on:
	higmac_net_set_mac_address(&g_higmac_ld);
	return 0;
}

void eth_halt(void)
{
	if (g_higmac_ld.iobase)
	{
		higmac_exit();
	}
}


