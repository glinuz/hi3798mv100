#ifndef __HIGMAC_CTRL_H
#define __HIGMAC_CTRL_H

#include "higmac.h"

#define STATION_ADDR_LOW		0x0000
#define STATION_ADDR_HIGH		0x0004
#define MAC_DUPLEX_HALF_CTRL		0x0008

#define PORT_MODE			0x0040

#define PORT_EN				0x0044
#define BITS_TX_EN			BIT(2)
#define BITS_RX_EN			BIT(1)

#define REC_FILT_CONTROL		0x0064
#define BIT_CRC_ERR_PASS		BIT(5)
#define BIT_PAUSE_FRM_PASS		BIT(4)
#define BIT_VLAN_DROP_EN		BIT(3)
#define BIT_BC_DROP_EN			BIT(2)
#define BIT_MC_MATCH_EN			BIT(1)
#define BIT_UC_MATCH_EN			BIT(0)

#define	PORT_MC_ADDR_LOW		0x0068
#define	PORT_MC_ADDR_HIGH		0x006C

#define MODE_CHANGE_EN			0x01b4
#define BIT_MODE_CHANGE_EN		BIT(0)

#define COL_SLOT_TIME			0x01c0

#define CRF_MIN_PACKET			0x0210
#define BIT_OFFSET_TSO_VERSION		28
#define BIT_MASK_TSO_VERSION		0x03

#define CONTROL_WORD			0x0214
#define CONTROL_WORD_CONFIG		0x640

#define TSO_COE_CTRL			0x02e8
#define BIT_COE_IPHDR_DROP		BIT(4)
#define BIT_COE_PAYLOAD_DROP		BIT(5)
#define BIT_COE_IPV6_UDP_ZERO_DROP	BIT(6)
#define COE_ERR_DROP			(BIT_COE_IPHDR_DROP | \
					BIT_COE_PAYLOAD_DROP | \
					BIT_COE_IPV6_UDP_ZERO_DROP)

#define RX_FQ_START_ADDR		0x0500
#define RX_FQ_DEPTH			0x0504
#define RX_FQ_WR_ADDR			0x0508
#define BITS_RX_FQ_WR_ADDR		MK_BITS(0, 21)
#define RX_FQ_RD_ADDR			0x050c
#define BITS_RX_FQ_RD_ADDR		MK_BITS(0, 21)
#define RX_FQ_VLDDESC_CNT		0x0510
#define BITS_RX_FQ_VLDDESC_CNT		MK_BITS(0, 16)
#define RX_FQ_ALEMPTY_TH		0x0514
#define BITS_RX_FQ_ALEMPTY_TH		MK_BITS(0, 16)
#define RX_FQ_REG_EN			0x0518
#define BITS_RX_FQ_START_ADDR_EN	BIT(2)
#define BITS_RX_FQ_DEPTH_EN		BIT(1)
#define BITS_RX_FQ_RD_ADDR_EN		MK_BITS(0, 1)
#define RX_FQ_ALFULL_TH			0x051c
#define BITS_RX_FQ_ALFULL_TH		MK_BITS(0, 16)

#define RX_BQ_START_ADDR		0x0520
#define RX_BQ_DEPTH			0x0524
#define RX_BQ_WR_ADDR			0x0528
#define RX_BQ_RD_ADDR			0x052c
#define RX_BQ_FREE_DESC_CNT		0x0530
#define BITS_RX_BQ_FREE_DESC_CNT	MK_BITS(0, 16)
#define RX_BQ_ALEMPTY_TH		0x0534
#define BITS_RX_BQ_ALEMPTY_TH		MK_BITS(0, 16)
#define RX_BQ_REG_EN			0x0538
#define BITS_RX_BQ_START_ADDR_EN	BIT(2)
#define BITS_RX_BQ_DEPTH_EN		BIT(1)
#define BITS_RX_BQ_WR_ADDR_EN		MK_BITS(0, 1)
#define RX_BQ_ALFULL_TH			0x053c
#define BITS_RX_BQ_ALFULL_TH		MK_BITS(0, 16)

#define TX_BQ_START_ADDR		0x0580
#define TX_BQ_DEPTH			0x0584
#define TX_BQ_WR_ADDR			0x0588
#define BITS_TX_BQ_WR_ADDR		MK_BITS(0, 21)
#define TX_BQ_RD_ADDR			0x058c
#define BITS_TX_BQ_RD_ADDR		MK_BITS(0, 21)
#define TX_BQ_VLDDESC_CNT		0x0590
#define BITS_TX_BQ_VLDDESC_CNT		MK_BITS(0, 16)
#define TX_BQ_ALEMPTY_TH		0x0594
#define BITS_TX_BQ_ALEMPTY_TH		MK_BITS(0, 16)
#define TX_BQ_REG_EN			0x0598
#define BITS_TX_BQ_START_ADDR_EN	BIT(2)
#define BITS_TX_BQ_DEPTH_EN		BIT(1)
#define BITS_TX_BQ_RD_ADDR_EN		MK_BITS(0, 1)
#define TX_BQ_ALFULL_TH			0x059c
#define BITS_TX_BQ_ALFULL_TH		MK_BITS(0, 16)

#define TX_RQ_START_ADDR		0x05a0
#define TX_RQ_DEPTH			0x05a4
#define TX_RQ_WR_ADDR			0x05a8
#define BITS_TX_RQ_WR_ADDR		MK_BITS(0, 21)
#define TX_RQ_RD_ADDR			0x05ac
#define BITS_TX_RQ_RD_ADDR		MK_BITS(0, 21)
#define TX_RQ_FREE_DESC_CNT		0x05b0
#define BITS_TX_RQ_FREE_DESC_CNT	MK_BITS(0, 16)
#define TX_RQ_ALEMPTY_TH		0x05b4
#define BITS_TX_RQ_ALEMPTY_TH		MK_BITS(0, 16)
#define TX_RQ_REG_EN			0x05b8
#define BITS_TX_RQ_START_ADDR_EN	BIT(2)
#define BITS_TX_RQ_DEPTH_EN		BIT(1)
#define BITS_TX_RQ_WR_ADDR_EN		MK_BITS(0, 1)
#define TX_RQ_ALFULL_TH			0x05bc
#define BITS_TX_RQ_ALFULL_TH		MK_BITS(0, 16)

#define RAW_PMU_INT			0x05c0
#define ENA_PMU_INT			0x05c4

#define DESC_WR_RD_ENA					0x05CC

#define IN_QUEUE_TH					0x05d8
#define BITS_OFFSET_TX_RQ_IN_TH				16

#define RX_BQ_IN_TIMEOUT_TH				0x05E0

#define TX_RQ_IN_TIMEOUT_TH				0x05e4

#define STOP_CMD					0x05e8
#define BITS_TX_STOP_EN					BIT(1)
#define BITS_RX_STOP_EN					BIT(0)
#define	STOP_RX_TX					(BITS_TX_STOP_EN | BITS_RX_STOP_EN)

#define GMAC_SPEED_1000			0x05
#define GMAC_SPEED_100			0x01
#define GMAC_SPEED_10			0x00

#define	E_MAC_TX_FAIL	2
#define	E_MAC_SW_GSO	3
#define E_MAC_UFO_BROADCAST	4

enum higmac_tx_err {
	ERR_NONE = 0,
	ERR_DESC_CFG = (1 << 0),
	ERR_DATA_LEN = (1 << 1),
	ERR_DESC_NFRAG_NUM = (1 << 2),
	ERR_DESC_IP_HDR_LEN = (1 << 3),
	ERR_DESC_PROT_HDR_LEN = (1 << 4),
	ERR_DESC_MTU = (1 << 5),
	ERR_LINK_SGPKT_LEN = (1 << 8),
	ERR_LINK_TSOPKT_LINEAR = (1 << 9),
	ERR_LINK_NFRAG_LEN = (1 << 10),
	ERR_LINK_TOTAL_LEN = (1 << 11),
	ERR_HDR_TCP_BCMC = (1 << 12),
	ERR_HDR_UDP_BC = (1 << 13),
	ERR_HDR_VLAN_IP_TYPE = (1 << 14),
	ERR_HDR_IP_TYPE = (1 << 15),
	ERR_HDR_IP_VERSION = (1 << 16),
	ERR_HDR_IP_HDR_LEN = (1 << 17),
	ERR_HDR_IP_TOTAL_LEN = (1 << 18),
	ERR_HDR_IPV6_TTL_PROT = (1 << 19),
	ERR_HDR_IPV4_OFFSET = (1 << 20),
	ERR_HDR_IPV4_TTL_PROT = (1 << 21),
	ERR_HDR_UDP_LEN = (1 << 22),
	ERR_HDR_TCP_LEN = (1 << 23),
	ERR_DESC = (ERR_DESC_CFG | ERR_DATA_LEN |
			ERR_DESC_NFRAG_NUM | ERR_DESC_IP_HDR_LEN |
			ERR_DESC_PROT_HDR_LEN | ERR_DESC_MTU),
	ERR_LINK = (ERR_LINK_SGPKT_LEN | ERR_LINK_TSOPKT_LINEAR |
			ERR_LINK_NFRAG_LEN | ERR_LINK_TOTAL_LEN),
	ERR_HDR = (ERR_HDR_TCP_BCMC | ERR_HDR_UDP_BC |
			ERR_HDR_VLAN_IP_TYPE | ERR_HDR_IP_TYPE |
			ERR_HDR_IP_VERSION | ERR_HDR_IP_HDR_LEN |
			ERR_HDR_IP_TOTAL_LEN | ERR_HDR_IPV6_TTL_PROT |
			ERR_HDR_IPV4_OFFSET | ERR_HDR_IPV4_TTL_PROT |
			ERR_HDR_UDP_LEN | ERR_HDR_TCP_LEN),
	ERR_ALL = (ERR_DESC | ERR_LINK | ERR_HDR),
};

/* board related func */
void higmac_mac_core_reset(struct higmac_netdev_local *priv);
void higmac_hw_internal_phy_reset(struct higmac_netdev_local *priv);
void higmac_hw_external_phy_reset(struct higmac_netdev_local *priv);
void higmac_internal_phy_clk_disable(struct higmac_netdev_local *priv);
void higmac_internal_phy_clk_enable(struct higmac_netdev_local *priv);
void higmac_hw_all_clk_disable(struct higmac_netdev_local *priv);
void higmac_hw_all_clk_enable(struct higmac_netdev_local *priv);

/* board independent func */
void higmac_hw_mac_core_init(struct higmac_netdev_local *ld);
void higmac_hw_set_desc_queue_addr(struct higmac_netdev_local *ld);
void higmac_hw_phy_reset(struct higmac_netdev_local *priv);

int higmac_clear_irqstatus(struct higmac_netdev_local *ld, int irqs);
int higmac_read_irqstatus(struct higmac_netdev_local *ld);
void higmac_irq_enable(struct higmac_netdev_local *ld);
void higmac_irq_disable(struct higmac_netdev_local *ld);
void higmac_hw_desc_enable(struct higmac_netdev_local *ld);
void higmac_hw_desc_disable(struct higmac_netdev_local *ld);
void higmac_port_enable(struct higmac_netdev_local *ld);
void higmac_port_disable(struct higmac_netdev_local *ld);

/* TSO supported, main transmit function */
int higmac_get_pkt_info(struct higmac_netdev_local *ld, struct sk_buff *skb,
			struct higmac_tso_desc *tx_bq_desc);
int higmac_xmit_gso(struct higmac_netdev_local *ld, struct sk_buff *skb,
		    struct higmac_tso_desc *tx_bq_desc, unsigned int desc_pos);
int higmac_xmit_release_gso(struct higmac_netdev_local *ld,
			    struct higmac_tso_desc *tx_bq_desc,
			    unsigned int desc_pos);

int higmac_xmit_release_skb(struct higmac_netdev_local *ld);
int higmac_xmit_real_send(struct higmac_netdev_local *ld, struct sk_buff *skb);
int higmac_feed_hw(struct higmac_netdev_local *ld);

void pmt_reg_restore(struct higmac_netdev_local *ld);
#endif
