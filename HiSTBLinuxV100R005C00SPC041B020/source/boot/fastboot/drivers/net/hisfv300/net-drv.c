#define HIETH_SFV300
#include "hieth.h"
#include "mdio.h"
#include "mac.h"
#include "ctrl.h"
#include "glb.h"
#include "sys.h"
#include <config.h>
#include <miiphy.h>
#include <net.h>
#define CONFIG_RANDOM_ETHADDR

/*
 * setenv ethact down
 * setenv ethact up
 */
#define U_ETH_NAME                    "up"
#define D_ETH_NAME                    "down"

/*************************************************************************/
int hieth_mdiobus_driver_init(void);
void hieth_mdiobus_driver_exit(void);
extern unsigned int get_phy_device(char *devname, unsigned char phyaddr);
extern int hieth_mdiobus_read(char *devname, unsigned char addr,
			      unsigned char reg, unsigned short *value);
extern int hieth_mdiobus_write(char *devname, unsigned char addr,
			      unsigned char reg, unsigned short value);

static struct hieth_netdev_local hieth_devs[2];

#define MAC_LEN 6

#define print_mac(mac)	 do{ int i;\
	printf("MAC:  ");\
	for (i = 0; i < MAC_LEN; i++)\
		printf("%c%02X", i ? '-' : ' ', *(((unsigned char*)mac)+i));\
	printf("\n");\
} while(0)

void string_to_mac(unsigned char *mac, char *s)
{
	int i;
	char *e;

	for (i = 0; i < MAC_LEN; ++i) {
		mac[i] = s ? simple_strtoul(s, &e, 16) : 0;
		if (s) {
			s = (*e) ? e + 1 : e;
		}
	}
}

static void print_mac_address(const char *pre_msg, const unsigned char *mac,
			      const char *post_msg)
{
	int i;

	if (pre_msg)
		printf(pre_msg);

	for (i = 0; i < 6; i++)
		printf("%02X%s", mac[i], i == 5 ? "" : ":");

	if (post_msg)
		printf(post_msg);
}

#ifdef CONFIG_RANDOM_ETHADDR
//void random_ether_addr(char *mac)
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
	mac[2] = ethaddr_low >> 24;
	mac[3] = (ethaddr_low >> 16) & 0xff;
	mac[4] = (ethaddr_low >> 8) & 0xff;
	mac[5] = ethaddr_low & 0xff;

	mac[0] &= 0xfe;		       /* clear multicast bit */
	mac[0] |= 0x02;		       /* set local assignment bit (IEEE802) */

}
#endif

//added by wzh 2009-4-9 begin
#define ETH_GLB_REG( n )    (GLB_HOSTMAC_L32 + ETH_IO_ADDRESS_BASE + n*4)
#if 0
static int is_valid_ether_addr(const u8 * addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}
#endif
static int set_mac_address(char *mac)
{
	unsigned char t[4] = { 0 };

	t[0] = mac[1];
	t[1] = mac[0];
	*(unsigned int *)ETH_GLB_REG(1) = *((unsigned int *)t);

	t[0] = mac[5];
	t[1] = mac[4];
	t[2] = mac[3];
	t[3] = mac[2];
	*(unsigned int *)ETH_GLB_REG(0) = *((unsigned int *)t);

	return 0;
}

static int set_random_mac_address(unsigned char *mac, unsigned char *ethaddr)
{
	random_ether_addr(mac);
	print_mac_address("Set Random MAC address: ", mac, "\n");

	sprintf((char *)ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	setenv("ethaddr", (char *)ethaddr);

	set_mac_address((char *)mac);
	return 0;
}

static int hieth_init_hw_desc_queue(struct hieth_netdev_local *ld)
{
	struct hieth_frame_desc* queue_phy_addr = NULL;
	int queue_count = 0;
	int i;

	/* init rx fq */
	queue_count = HIETH_HW_DESC_DEPTH;

	queue_phy_addr= (struct hieth_frame_desc*)malloc(queue_count * sizeof(struct hieth_frame_desc));
	if (queue_phy_addr == NULL)
	{
		printf("alloc rx fq error!\n");
		return 1;
	}

	memset((void*)queue_phy_addr, 0, queue_count * sizeof(struct hieth_frame_desc));
	ld->hieth_desc_head = queue_phy_addr;
	ld->desc_hw_offset = 0;
	ld->desc_rec_offset = 0;

	for(i =0;i<HIETH_HW_DESC_DEPTH;i++){
		queue_phy_addr[i].frm_addr = (unsigned long)memalign(PKTALIGN, PKTSIZE_ALIGN);
		queue_phy_addr[i].frm_len = 0;
	}
	return 0;
}

static int hieth_destroy_hw_desc_queue(struct hieth_netdev_local *ld)
{
	struct hieth_frame_desc* queue_phy_addr = ld->hieth_desc_head;
	int i;

	if (queue_phy_addr) {
		for (i =0;i<HIETH_HW_DESC_DEPTH;i++){
			free((void *)queue_phy_addr[i].frm_addr);
		}

		free(ld->hieth_desc_head);
		ld->hieth_desc_head = NULL;
	}
	ld->desc_hw_offset = 0;
	ld->desc_rec_offset = 0;

	return 0;
}

int eth_set_host_mac_address(struct eth_device* dev)
{
	char *s;
	unsigned char ethaddr[20];

	s = getenv("ethaddr");
	if (!s) {
		printf("none ethaddr. \n");
		set_random_mac_address(dev->enetaddr, ethaddr);
		return 0;
	}

	string_to_mac(dev->enetaddr, s);
	if (!is_valid_ether_addr(dev->enetaddr)) {
		printf("MAC address invalid!\n");
		set_random_mac_address(dev->enetaddr, ethaddr);
		return 0;
	}

	print_mac(dev->enetaddr);
	set_mac_address((char *)dev->enetaddr);

	return 0;
}

static void phy_print_status(struct hieth_netdev_local *ld, int stat)
{
	printf("%s : phy status change : LINK=%s : DUPLEX=%s : SPEED=%s\n",
	       (ld->port == UP_PORT) ? "UP_PORT" : "DOWN_PORT",
	       (stat & HIETH_LINKED) ? "UP" : "DOWN",
	       (stat & HIETH_DUP_FULL) ? "FULL" : "HALF",
	       (stat & HIETH_SPD_100M) ? "100M" : "10M");
}

static void hieth_adjust_link(struct eth_device* dev)
{
	u16 reg = 0;
	int stat = 0;
	int timeout_us = 1000;
	struct hieth_netdev_local *ld =(struct hieth_netdev_local*) dev->priv;
	/*this env phy_link_time used to solve the difference phy auto-negotiation time of  various phys */
	char *timeout = getenv("phy_link_time");
	if (timeout) {
		timeout_us = simple_strtol(timeout, 0, 10);
		if (timeout_us < 0)
			timeout_us = 1000;
	}

	reg = 0;
	if (miiphy_read(dev->name, UD_REG_NAME(PHY_ADDR), PHY_ANAR,
		    &reg) != 0) {
		printf("PHY read failed\n");
		return;
	};

	reg |= 0x1E0;
	if (miiphy_write(dev->name, UD_REG_NAME(PHY_ADDR), PHY_ANAR,
		     reg) != 0)
		return;
retry:
	udelay(1);

	stat |=
	    miiphy_link(dev->name,
			UD_REG_NAME(PHY_ADDR)) ? HIETH_LINKED : 0;
	stat |=
	    miiphy_duplex(dev->name,
			  UD_REG_NAME(PHY_ADDR)) == FULL ? HIETH_DUP_FULL : 0;
	stat |=
	    miiphy_speed(dev->name,
			 UD_REG_NAME(PHY_ADDR)) ==
	    _100BASET ? HIETH_SPD_100M : 0;

	if (--timeout_us && !(stat & HIETH_LINKED))
		goto retry;

	if (stat != ld->link_stat) {
		hieth_set_linkstat(ld, stat);
		phy_print_status(ld, stat);
		ld->link_stat = stat;
	}
}

static int hieth_net_open(struct eth_device* dev)
{
	struct hieth_netdev_local *ld =(struct hieth_netdev_local*) dev->priv;
	/* enable sys-ctrl-en and clk-en  */
	hieth_sys_startup();

#ifdef CONFIG_ARCH_HI3798MX
	/* adjust amplitude */
	if (ld->port == UP_PORT) {
		miiphy_write(dev->name, UD_REG_NAME(PHY_ADDR), 0x1e,
			     0x1ff);
		miiphy_write(dev->name, UD_REG_NAME(PHY_ADDR), 0x1d,
			     0x13);
	}
#endif

	/* setup hardware tx dep */
	hieth_writel_bits(ld, HIETH_HW_TXQ_DEPTH, UD_REG_NAME(GLB_QLEN_SET),
			  BITS_TXQ_DEP);

	/* setup hardware rx dep */
	hieth_writel_bits(ld, HIETH_HW_RXQ_DEPTH, UD_REG_NAME(GLB_QLEN_SET),
			  BITS_RXQ_DEP);

	ld->link_stat = 0;
	hieth_adjust_link(dev);

	hieth_irq_enable(ld, UD_BIT_NAME(HIETH_INT_MULTI_RXRDY));

	return 0;
}

static int hieth_net_close(struct hieth_netdev_local *ld)
{
	hieth_glb_preinit_dummy(ld);

	hieth_sys_allstop();

	return 0;
}

static int hieth_dev_probe_init(struct eth_device* dev, int port)
{
	unsigned int regval;
	struct hieth_netdev_local *ld =(struct hieth_netdev_local*) dev->priv;

	if ((UP_PORT != port) && (DOWN_PORT != port))
		return -1;	       //-ENODEV

	ld->iobase_phys = ETH_IO_ADDRESS_BASE;

	ld->port = port;

	ld->phy_name = dev->name;

	hieth_glb_preinit_dummy(ld);

	hieth_init_hw_desc_queue(ld);

	hieth_sys_allstop();

	return 0;
}

static int hieth_dev_remove(struct hieth_netdev_local *ld)
{
	hieth_destroy_hw_desc_queue(ld);

	return 0;
}


int hieth_init(struct eth_device* dev, bd_t * bd)
{
	int count = 3;
	int ret;
	char *devname = NULL;
	unsigned char phyaddr = 0;
	int port = -1;
	struct hieth_netdev_local  *ld= (struct hieth_netdev_local*) dev->priv;

	if (strcmp(dev->name, U_ETH_NAME) == 0) {
		phyaddr = U_PHY_ADDR;
		devname = dev->name;
		port = UP_PORT;
	} else if (strcmp(dev->name, D_ETH_NAME) == 0) {
		phyaddr = D_PHY_ADDR;
		devname = dev->name;
		port = DOWN_PORT;
	}

	printf("Eth %s port phy at 0x%02x is connect\n", dev->name, phyaddr);
	printf(OSDRV_MODULE_VERSION_STRING "\n");

	/* sys-func-sel */
	hieth_sys_init();

	/* register MDIO bus to uboot */
	if (hieth_mdiobus_driver_init()) {
		return -1;
	}

	/* for Hi3716MV330/Hifonev310/Hi3798MV200 */
	hieth_fix_festa_phy_ate(dev);

	if (!get_phy_device(devname, phyaddr)) {
		miiphy_reset(devname, phyaddr);
		miiphy_set_current_dev(devname);

		ret = hieth_dev_probe_init(dev, port);
		if (ret) {
			hieth_error
			    ("register UpEther netdevice driver failed!");
			goto _error_register_driver;
		}
	}

	eth_set_host_mac_address(dev);

retry:
	if (!get_phy_device(devname, phyaddr)) {
		/* open DownEther net dev */
		hieth_net_open(dev);
		if (ld->link_stat & HIETH_LINKED) {
			goto _link_ok;
		}
	}

	if (--count)
		goto retry;

_error_register_driver:
	hieth_mdiobus_driver_exit();
	if (!get_phy_device(devname, phyaddr)) {
		/*add this to avoid the first time to use eth will print 'No such device: XXXXX' message. */
		if (miiphy_get_current_dev())
			miiphy_reset(devname, phyaddr);
		hieth_net_close(ld);
	}
	return -1;

_link_ok:

	return 0;
}

static void hieth_exit(void)
{

	hieth_mdiobus_driver_exit();

	hieth_sys_exit();
}


int hieth_recv(struct eth_device* dev)
{
	int recvq_ready, timeout_us = 10000;
	struct hieth_frame_desc *fd;
	struct hieth_netdev_local *ld = (struct hieth_netdev_local *)dev->priv;
	struct hieth_frame_desc receive_fd;
	int hw_offset,rec_offset;

	fd = ld->hieth_desc_head;
	hw_offset = ld->desc_hw_offset;
	rec_offset = ld->desc_rec_offset;

	/* check this we can add a Rx addr */
	recvq_ready =
		hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
				BITS_RECVQ_RDY);
	if (!recvq_ready) {
		hieth_trace(7, "hw can't add a rx addr.");
	}

	while(recvq_ready &&
			((hw_offset + 1) % HIETH_HW_DESC_DEPTH != rec_offset)) {
		receive_fd = fd[hw_offset];
		hw_recvq_setfd(ld, receive_fd);

		hw_offset = (hw_offset + 1) % HIETH_HW_DESC_DEPTH;

		recvq_ready =
			hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
					BITS_RECVQ_RDY);
	}
	ld->desc_hw_offset = hw_offset;

	/* receive packed, loop in NetLoop */
	while (--timeout_us && !is_recv_packet_rx(ld))
		udelay(1);

	if (is_recv_packet_rx(ld)) {
		receive_fd = fd[rec_offset];

		/*hwid = hw_get_rxpkg_id(ld); */
		receive_fd.frm_len = hw_get_rxpkg_len(ld);

		hw_set_rxpkg_finish(ld);

		if (HIETH_INVALID_RXPKG_LEN(receive_fd.frm_len)) {
			hieth_error("frm_len invalid (%d).", receive_fd.frm_len);
			goto _error_exit;
		}

		memcpy((void *)NetRxPackets[0], (void *)receive_fd.frm_addr,
				receive_fd.frm_len);

		/* Pass the packet up to the protocol layers. */
		NetReceive(NetRxPackets[0], receive_fd.frm_len);

		rec_offset = (rec_offset + 1) % HIETH_HW_DESC_DEPTH;
		ld->desc_rec_offset = rec_offset;

		return 0;
	} else {
		hieth_trace(7, "hw rx timeout.");
	}

_error_exit:
	return -1;
}

int hieth_send(struct eth_device* dev, volatile void *packet, int length)
{
	int ints, xmitq_ready, timeout_us = 3000;
	struct hieth_frame_desc fd;
	struct hieth_netdev_local *ld = (struct hieth_netdev_local *)dev->priv;

	/* check this we can add a Tx addr */
	xmitq_ready =
	    hieth_readl_bits(ld, UD_REG_NAME(GLB_RO_QUEUE_STAT),
			     BITS_XMITQ_RDY);
	if (!xmitq_ready) {
		hieth_error("hw can't add a tx addr.");
		goto _error_exit;
	}

	/* enable tx int */
	hieth_irq_enable(ld, UD_BIT_NAME(HIETH_INT_TXQUE_RDY));

	/* fill tx hwq fd */
	fd.frm_addr = (unsigned long)packet;
	fd.frm_len = length + 4;
	hw_xmitq_setfd(ld, fd);

	do {
		udelay(1);
		ints = hieth_read_irqstatus(ld);
	} while (--timeout_us && !(ints & UD_BIT_NAME(HIETH_INT_TXQUE_RDY)));

	hieth_clear_irqstatus(ld, ints);

	if (!timeout_us) {
		hieth_error("hw tx timeout.");
		goto _error_exit;
	}

	return 0;

_error_exit:
	return -1;
}

void hieth_halt(struct eth_device* dev)
{
	struct hieth_netdev_local * hieth= (struct hieth_netdev_local*) dev->priv;

	if (hieth->iobase_phys)
		hieth_net_close(hieth);

	hieth_dev_remove(hieth);

	hieth_exit();
}

int hieth_initialize(void)
{
	struct eth_device *dev;

	memset(hieth_devs, 0, sizeof(hieth_devs));
	
	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return -1;
	memset(dev, 0, sizeof(*dev));

	dev->iobase = REG_BASE_SF;
	dev->init = hieth_init;
	dev->halt = hieth_halt;
	dev->send = hieth_send;
	dev->recv = hieth_recv;
	dev->priv = &hieth_devs[UP_PORT];
	strncpy(dev->name, U_ETH_NAME, sizeof(dev->name) - 1);

	eth_register(dev);

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	miiphy_register(dev->name, hieth_mdiobus_read, hieth_mdiobus_write);
#endif
	/* only MV310 support 2 port, mv320 mv330 support 1 port */
	if (_HI3716M_V310 != get_chipid())
		return 0;

	dev = malloc(sizeof(*dev));
	if (dev == NULL)
		return -1;
	memset(dev, 0, sizeof(*dev));

	dev->iobase = REG_BASE_SF + 0x2000;
	dev->init = hieth_init;
	dev->halt = hieth_halt;
	dev->send = hieth_send;
	dev->recv = hieth_recv;
	dev->priv = &hieth_devs[DOWN_PORT];
	strncpy(dev->name, D_ETH_NAME, sizeof(dev->name));

	eth_register(dev);

#if defined(CONFIG_MII) || defined(CONFIG_CMD_MII)
	miiphy_register(dev->name, hieth_mdiobus_read, hieth_mdiobus_write);
#endif

	return 0;
}
