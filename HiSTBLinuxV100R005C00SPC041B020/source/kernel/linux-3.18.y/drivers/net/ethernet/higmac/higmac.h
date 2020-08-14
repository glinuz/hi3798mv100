#ifndef __HIGMAC_H__
#define __HIGMAC_H__

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/list.h>
#include <linux/phy.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include "phy_fix.h"

#define HIGMAC_DRIVER_NAME	"hi_gmac_v200"

#define HIGMAC_MAC_CLK_NAME	"higmac_clk"
#define HIGMAC_MACIF_CLK_NAME	"macif_clk"

#define HIGMAC_PORT_RST_NAME	"port_reset"
#define HIGMAC_MACIF_RST_NAME	"macif_reset"
#define HIGMAC_PHY_RST_NAME	"phy_reset"

#define HIGMAC_TSO_DEBUG

#include "tso.h"

#define HIETH_INVALID_RXPKG_LEN(len)	\
		(!((len) >= 42 && (len) <= HIETH_MAX_FRAME_SIZE))

#define HIGMAC_IOSIZE			(0x1000)
#define HIGMAC_OFFSET			(HIGMAC_IOSIZE)

#define RX_BQ_IN_INT			(1<<17)
#define TX_RQ_IN_INT			(1<<19)
#define RX_BQ_IN_TIMEOUT_INT		(1<<28)
#define TX_RQ_IN_TIMEOUT_INT		(1<<29)

/* write or read descriptor need memory barrier */
#define HIGMAC_SYNC_BARRIER() do { isb(); smp_mb(); } while (0)

enum {
	GMAC_PORT0,
	GMAC_PORT1,
	GMAC_MAX_PORT,
};

enum {
	MEM_GMAC_IOBASE,
	MEM_MACIF_IOBASE,
	MEM_FWD_IOBASE,
	MEM_CTRL_IOBASE,
};

#define HIGMAC_LINKED		(1 << 0)
#define HIGMAC_DUP_FULL		(1 << 1)
#define HIGMAC_SPD_10M		(1 << 2)
#define HIGMAC_SPD_100M		(1 << 3)
#define HIGMAC_SPD_1000M	(1 << 4)

#define RX_BQ_INT_THRESHOLD	0x40	/* TODO: */
#define TX_RQ_INT_THRESHOLD	0x20	/* TODO: */

#define HIGMAC_MAX_QUEUE_DEPTH	(2048)

#define HIGMAC_HWQ_RX_FQ_DEPTH	(1024)
#define HIGMAC_HWQ_RX_BQ_DEPTH	(HIGMAC_HWQ_RX_FQ_DEPTH)
#define HIGMAC_HWQ_TX_BQ_DEPTH	(1024)
#define HIGMAC_HWQ_TX_RQ_DEPTH	(HIGMAC_HWQ_TX_BQ_DEPTH)

#define HIGMAC_MONITOR_TIMER	(msecs_to_jiffies(200))

#define HIGMAC_ADDR_BLOCK_SIZE	0x1000

#define MAX_RX_POOLS		SZ_1K
#define HIETH_MAX_FRAME_SIZE	(1600+128)
#define SKB_SIZE		(HIETH_MAX_FRAME_SIZE)

#define DESC_VLD_FREE		0
#define DESC_VLD_BUSY		1

#define DESC_FL_FIRST		2
#define DESC_FL_MID		0
#define DESC_FL_LAST		1
#define DESC_FL_FULL		3

#if defined(CONFIG_HIGMAC_DESC_4WORD)
#define DESC_WORD_SHIFT		2
#else
#define DESC_WORD_SHIFT		3
#endif
#define DESC_BYTE_SHIFT		(DESC_WORD_SHIFT + 2)
#define DESC_WORD_CNT		(1 << DESC_WORD_SHIFT)
#define DESC_SIZE		(1 << DESC_BYTE_SHIFT)

#if defined(CONFIG_HIGMAC_DESC_4WORD)
struct higmac_desc {
	unsigned int data_buff_addr;

	unsigned int buffer_len:11;
#if defined(CONFIG_HIGMAC_RXCSUM)
	unsigned int reserve2:1;
	unsigned int payload_csum_err:1;
	unsigned int header_csum_err:1;
	unsigned int payload_csum_done:1;
	unsigned int header_csum_done:1;
#else
	unsigned int reserve2:5;
#endif
	unsigned int data_len:11;
	unsigned int reserve1:2;
	unsigned int fl:2;
	unsigned int descvid:1;

	unsigned int reserve_desc2;
	unsigned int reserve3;
};

struct higmac_tso_desc {
	unsigned int data_buff_addr;
	union {
		struct {
			unsigned int prot_hdr_len:4;
			unsigned int ip_hdr_len:4;
			unsigned int prot_type:1;
			unsigned int ip_ver:1;
			unsigned int vlan_flag:1;
			unsigned int nfrags_num:5;
			unsigned int data_len:11;
			unsigned int reservel:1;
			unsigned int tso_flag:1;
			unsigned int coe_flag:1;
			unsigned int sg_flag:1;
			unsigned int hw_own:1;
		} tx;
		struct {
			unsigned int buffer_len:11;
			unsigned int reserve2:5;
			unsigned int data_len:11;
			unsigned int reserve1:2;
			unsigned int fl:2;
			unsigned int descvid:1;
		} rx;
		unsigned int val;
	} desc1;
	unsigned int reserve_desc2;
	unsigned int tx_err;
};
#else
struct higmac_desc {
	unsigned int data_buff_addr;

	unsigned int buffer_len:11;
#if defined(CONFIG_HIGMAC_RXCSUM)
	unsigned int reserve2:1;
	unsigned int payload_csum_err:1;
	unsigned int header_csum_err:1;
	unsigned int payload_csum_done:1;
#else
	unsigned int reserve2:5;
#endif
	unsigned int data_len:11;
	unsigned int reserve1:2;
	unsigned int fl:2;
	unsigned int descvid:1;

	unsigned int reserve_desc2;
	unsigned int reserve3;

	unsigned int reserve4;
	unsigned int reserve5;
	unsigned int reserve6;
	unsigned int reserve7;
};

struct higmac_tso_desc {
	unsigned int data_buff_addr;
	union {
		struct {
			unsigned int prot_hdr_len:4;
			unsigned int ip_hdr_len:4;
			unsigned int prot_type:1;
			unsigned int ip_ver:1;
			unsigned int vlan_flag:1;
			unsigned int nfrags_num:5;
			unsigned int data_len:11;
			unsigned int reservel:1;
			unsigned int tso_flag:1;
			unsigned int coe_flag:1;
			unsigned int sg_flag:1;
			unsigned int hw_own:1;
		} tx;
		struct {
			unsigned int buffer_len:11;
			unsigned int reserve2:5;
			unsigned int data_len:11;
			unsigned int reserve1:2;
			unsigned int fl:2;
			unsigned int descvid:1;
		} rx;
		unsigned int val;
	} desc1;
	unsigned int reserve_desc2;
	unsigned int reserve3;

	unsigned int tx_err;
	unsigned int reserve5;
	unsigned int reserve6;
	unsigned int reserve7;
};
#endif

#define SKB_MAGIC	((struct sk_buff *)0x5a)

#define QUEUE_NUMS	(4)
struct higmac_netdev_local {
#define HIGMAC_SG_DESC_ADD	(64U)
	struct sg_desc *dma_sg_desc ____cacheline_aligned;
	dma_addr_t dma_sg_phy;
	unsigned int sg_head;
	unsigned int sg_tail;
	unsigned int sg_count;

	void __iomem *gmac_iobase;
	void __iomem *macif_base;
	int index;		/* 0 -- mac0, 1 -- mac1 */

	bool tso_supported;

	struct reset_control *port_rst;
	struct reset_control *macif_rst;
	struct reset_control *phy_rst;

	struct {
		struct higmac_desc *desc;
		dma_addr_t phys_addr;
		int *sg_desc_offset;

		/* how many desc in the desc pool */
		unsigned int count;
		struct sk_buff **skb;

		/* sizeof(desc) * count */
		unsigned int size;
	} pool[QUEUE_NUMS];
#define rx_fq		pool[0]
#define rx_bq		pool[1]
#define tx_bq		pool[2]
#define tx_rq		pool[3]

	struct sk_buff **tx_skb;
	struct sk_buff **rx_skb;

	struct device *dev;
	struct net_device *netdev;
	struct clk *clk;
	struct clk *macif_clk;

	struct mii_bus *bus;
	struct higmac_adapter *adapter;

	struct timer_list monitor;

	char phy_name[MII_BUS_ID_SIZE];
	struct phy_device *phy;
	struct device_node *phy_node;
	phy_interface_t phy_mode;
	bool autoeee;
	bool internal_phy;
	unsigned int phy_addr;
	int (*eee_init)(struct phy_device *phy_dev);
        /* gpio reset pin if has */
        void __iomem *gpio_base;
        u32 gpio_bit;

	int old_link;
	int old_speed;
	int old_duplex;

	/* receive packet lock */
	spinlock_t rxlock;
	/* transmit packet lock */
	spinlock_t txlock;
	/* power management lock */
	spinlock_t pmtlock;

	struct tasklet_struct bf_recv;
	int dev_state;		/* INIT/OPEN/CLOSE */
	char pm_state;
	bool wol_enable;
	u32 msg_enable;
#define INIT			(0)	/* power off gmac */
#define OPEN			(1)	/* power on gmac */
#define CLOSE			(2)	/* power off gmac */
};

enum tso_version {
	VER_NO_TSO = 0x0,
	VER_BYTE_SPLICE = 0x1,
	VER_SG_COE = 0x2,
	VER_TSO = 0x3,
};

#ifdef HIGMAC_TSO_DEBUG
#define MAX_RECORD	(100)
struct send_pkt_info {
	struct higmac_tso_desc desc;
	int status;
};
#endif

int higmac_tx_avail(struct higmac_netdev_local *ld);

#endif
