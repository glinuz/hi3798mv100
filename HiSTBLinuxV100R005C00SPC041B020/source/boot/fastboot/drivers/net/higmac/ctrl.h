#ifndef __HIGMAC_CTRL_H
#define __HIGMAC_CTRL_H

#define HIGMAC_SYS_CTL_IO_BASE		0x20030000
#define PERI_CRG21						0x0054

#define STATION_ADDR_LOW				0x0000
#define STATION_ADDR_HIGH				0x0004

#define MAX_FRM_SIZE					0x003c
#define BITS_MAX_FRM_SIZE				MK_BITS(0, 14)

#define PORT_MODE						0x0040
#define BITS_PORT_MODE					MK_BITS(0, 3)

#define PORT_EN							0x0044
#define BITS_TX_EN						MK_BITS(2, 1)
#define BITS_RX_EN						MK_BITS(1, 1)

#define REC_FILT_CONTROL				0x0064
#define BIT_CRC_ERR_PASS				MK_BITS(5, 1)
#define BIT_PAUSE_FRM_PASS				MK_BITS(4, 1)
#define BIT_VLAN_DROP_EN				MK_BITS(3, 1)
#define BIT_BC_DROP_EN					MK_BITS(2, 1)
#define BIT_MC_MATCH_EN				MK_BITS(1, 1)
#define BIT_UC_MATCH_EN				MK_BITS(0, 1)

#define CF_CRC_STRIP					0x01B0
#define BIT_CF_CRC_STRIP				MK_BITS(0, 1)

#define MODE_CHANGE_EN				0x01b4
#define BIT_MODE_CHANGE_EN			MK_BITS(0, 1)

#define RECV_CONTROL					0x01e0
#define BIT_STRIP_PAD_EN				MK_BITS(3, 1)
#define BIT_RUNT_PKT_EN				MK_BITS(4, 1)

#define RX_FQ_START_ADDR				0x0500
#define RX_FQ_DEPTH						0x0504
#define BITS_RX_FQ_DEPTH				MK_BITS(0, 19)
#define RX_FQ_WR_ADDR					0x0508
#define BITS_RX_FQ_WR_ADDR			MK_BITS(0, 21)
#define RX_FQ_RD_ADDR					0x050c
#define BITS_RX_FQ_RD_ADDR				MK_BITS(0, 21)
#define RX_FQ_VLDDESC_CNT				0x0510
#define BITS_RX_FQ_VLDDESC_CNT		MK_BITS(0, 16)
#define RX_FQ_ALEMPTY_TH				0x0514
#define BITS_RX_FQ_ALEMPTY_TH			MK_BITS(0, 16)
#define RX_FQ_REG_EN					0x0518
#define BITS_RX_FQ_START_ADDR_EN		MK_BITS(2, 1)
#define BITS_RX_FQ_DEPTH_EN			MK_BITS(1, 1)
#define BITS_RX_FQ_RD_ADDR_EN			MK_BITS(0, 1)
#define RX_FQ_ALFULL_TH				0x051c
#define BITS_RX_FQ_ALFULL_TH			MK_BITS(0, 16)


#define RX_BQ_START_ADDR				0x0520
#define RX_BQ_DEPTH						0x0524
#define BITS_RX_BQ_DEPTH				MK_BITS(0, 19)
#define RX_BQ_WR_ADDR					0x0528
#define BITS_RX_BQ_WR_ADDR			MK_BITS(0, 21)
#define RX_BQ_RD_ADDR					0x052c
#define BITS_RX_BQ_RD_ADDR				MK_BITS(0, 21)
#define RX_BQ_FREE_DESC_CNT			0x0530
#define BITS_RX_BQ_FREE_DESC_CNT		MK_BITS(0, 16)
#define RX_BQ_ALEMPTY_TH				0x0534
#define BITS_RX_BQ_ALEMPTY_TH			MK_BITS(0, 16)
#define RX_BQ_REG_EN					0x0538
#define BITS_RX_BQ_START_ADDR_EN		MK_BITS(2, 1)
#define BITS_RX_BQ_DEPTH_EN			MK_BITS(1, 1)
#define BITS_RX_BQ_WR_ADDR_EN			MK_BITS(0, 1)
#define RX_BQ_ALFULL_TH				0x053c
#define BITS_RX_BQ_ALFULL_TH			MK_BITS(0, 16)

#define TX_BQ_START_ADDR				0x0580
#define TX_BQ_DEPTH						0x0584
#define BITS_TX_BQ_DEPTH				MK_BITS(0, 19)
#define TX_BQ_WR_ADDR					0x0588
#define BITS_TX_BQ_WR_ADDR			MK_BITS(0, 21)
#define TX_BQ_RD_ADDR					0x058c
#define BITS_TX_BQ_RD_ADDR				MK_BITS(0, 21)
#define TX_BQ_VLDDESC_CNT				0x0590
#define BITS_TX_BQ_VLDDESC_CNT		MK_BITS(0, 16)
#define TX_BQ_ALEMPTY_TH				0x0594
#define BITS_TX_BQ_ALEMPTY_TH			MK_BITS(0, 16)
#define TX_BQ_REG_EN					0x0598
#define BITS_TX_BQ_START_ADDR_EN		MK_BITS(2, 1)
#define BITS_TX_BQ_DEPTH_EN			MK_BITS(1, 1)
#define BITS_TX_BQ_RD_ADDR_EN			MK_BITS(0, 1)
#define TX_BQ_ALFULL_TH				0x059c
#define BITS_TX_BQ_ALFULL_TH			MK_BITS(0, 16)

#define TX_RQ_START_ADDR				0x05a0
#define TX_RQ_DEPTH						0x05a4
#define BITS_TX_RQ_DEPTH				MK_BITS(0, 19)
#define TX_RQ_WR_ADDR					0x05a8
#define BITS_TX_RQ_WR_ADDR			MK_BITS(0, 21)
#define TX_RQ_RD_ADDR					0x05ac
#define BITS_TX_RQ_RD_ADDR				MK_BITS(0, 21)
#define TX_RQ_FREE_DESC_CNT			0x05b0
#define BITS_TX_RQ_FREE_DESC_CNT		MK_BITS(0, 16)
#define TX_RQ_ALEMPTY_TH				0x05b4
#define BITS_TX_RQ_ALEMPTY_TH			MK_BITS(0, 16)
#define TX_RQ_REG_EN					0x05b8
#define BITS_TX_RQ_START_ADDR_EN		MK_BITS(2, 1)
#define BITS_TX_RQ_DEPTH_EN			MK_BITS(1, 1)
#define BITS_TX_RQ_WR_ADDR_EN			MK_BITS(0, 1)
#define TX_RQ_ALFULL_TH				0x05bc
#define BITS_TX_RQ_ALFULL_TH			MK_BITS(0, 16)

#define RAW_PMU_INT					0x05c0

#define ENA_PMU_INT							0x05c4
#define ENA_BITS_MAC_FIFO_ERR_INT				MK_BITS(30, 1)
#define ENA_BITS_TX_RQ_IN_TIMEOUT_INT		MK_BITS(29, 1)
#define ENA_BITS_RX_BQ_IN_TIMEOUT_INT		MK_BITS(28, 1)
#define ENA_BITS_TXOUTCFF_FULL_INT			MK_BITS(27, 1)
#define ENA_BITS_TXOUTCFF_EMPTY_INT			MK_BITS(26, 1)
#define ENA_BITS_TXCFF_FULL_INT				MK_BITS(25, 1)
#define ENA_BITS_TXCFF_EMPTY_INT				MK_BITS(24, 1)
#define ENA_BITS_RXOUTCFF_FULL_INT			MK_BITS(23, 1)
#define ENA_BITS_RXOUTCFF_EMPTY_INT			MK_BITS(22, 1)
#define ENA_BITS_RXCFF_FULL_INT				MK_BITS(21, 1)
#define ENA_BITS_RXCFF_EMPTY_INT				MK_BITS(20, 1)
#define ENA_BITS_TX_RQ_IN_INT					MK_BITS(19, 1)
#define ENA_BITS_TX_BQ_OUT_INT				MK_BITS(18, 1)
#define ENA_BITS_RX_BQ_IN_INT					MK_BITS(17, 1)
#define ENA_BITS_RX_FQ_OUT_INT				MK_BITS(16, 1)
#define ENA_BITS_TX_RQ_EMPTY_INT				MK_BITS(15, 1)
#define ENA_BITS_TX_RQ_FULL_INT				MK_BITS(14, 1)
#define ENA_BITS_TX_RQ_ALEMPTY_INT			MK_BITS(13, 1)
#define ENA_BITS_TX_RQ_ALFULL_INT				MK_BITS(12, 1)
#define ENA_BITS_TX_BQ_EMPTY_INT				MK_BITS(11, 1)
#define ENA_BITS_TX_BQ_FULL_INT				MK_BITS(10, 1)
#define ENA_BITS_TX_BQ_ALEMPTY_INT			MK_BITS(9, 1)
#define ENA_BITS_TX_BQ_ALFULL_INT				MK_BITS(8, 1)
#define ENA_BITS_RX_BQ_EMPTY_INT				MK_BITS(7, 1)
#define ENA_BITS_RX_BQ_FULL_INT				MK_BITS(6, 1)
#define ENA_BITS_RX_BQ_ALEMPTY_INT			MK_BITS(5, 1)
#define ENA_BITS_RX_BQ_ALFULL_INT				MK_BITS(4, 1)
#define ENA_BITS_RX_FQ_EMPTY_INT				MK_BITS(3, 1)
#define ENA_BITS_RX_FQ_FULL_INT				MK_BITS(2, 1)
#define ENA_BITS_RX_FQ_ALEMPTY_INT			MK_BITS(1, 1)
#define ENA_BITS_RX_FQ_ALFULL_INT				MK_BITS(0, 1)

#define STATUS_PMU_INT							0x05c8
#define STATUS_BITS_MAC_FIFO_ERR_INT			MK_BITS(30, 1)
#define STATUS_BITS_TX_RQ_IN_TIMEOUT_INT		MK_BITS(29, 1)
#define STATUS_BITS_RX_BQ_IN_TIMEOUT_INT		MK_BITS(28, 1)
#define STATUS_BITS_TXOUTCFF_FULL_INT		MK_BITS(27, 1)
#define STATUS_BITS_TXOUTCFF_EMPTY_INT		MK_BITS(26, 1)
#define STATUS_BITS_TXCFF_FULL_INT			MK_BITS(25, 1)
#define STATUS_BITS_TXCFF_EMPTY_INT			MK_BITS(24, 1)
#define STATUS_BITS_RXOUTCFF_FULL_INT		MK_BITS(23, 1)
#define STATUS_BITS_RXOUTCFF_EMPTY_INT		MK_BITS(22, 1)
#define STATUS_BITS_RXCFF_FULL_INT			MK_BITS(21, 1)
#define STATUS_BITS_RXCFF_EMPTY_INT			MK_BITS(20, 1)
#define STATUS_BITS_TX_RQ_IN_INT				MK_BITS(19, 1)
#define STATUS_BITS_TX_BQ_OUT_INT				MK_BITS(18, 1)
#define STATUS_BITS_RX_BQ_IN_INT				MK_BITS(17, 1)
#define STATUS_BITS_RX_FQ_OUT_INT				MK_BITS(16, 1)
#define STATUS_BITS_TX_RQ_EMPTY_INT			MK_BITS(15, 1)
#define STATUS_BITS_TX_RQ_FULL_INT			MK_BITS(14, 1)
#define STATUS_BITS_TX_RQ_ALEMPTY_INT		MK_BITS(13, 1)
#define STATUS_BITS_TX_RQ_ALFULL_INT			MK_BITS(12, 1)
#define STATUS_BITS_TX_BQ_EMPTY_INT			MK_BITS(11, 1)
#define STATUS_BITS_TX_BQ_FULL_INT			MK_BITS(10, 1)
#define STATUS_BITS_TX_BQ_ALEMPTY_INT		MK_BITS(9, 1)
#define STATUS_BITS_TX_BQ_ALFULL_INT			MK_BITS(8, 1)
#define STATUS_BITS_RX_BQ_EMPTY_INT			MK_BITS(7, 1)
#define STATUS_BITS_RX_BQ_FULL_INT			MK_BITS(6, 1)
#define STATUS_BITS_RX_BQ_ALEMPTY_INT		MK_BITS(5, 1)
#define STATUS_BITS_RX_BQ_ALFULL_INT			MK_BITS(4, 1)
#define STATUS_BITS_RX_FQ_EMPTY_INT			MK_BITS(3, 1)
#define STATUS_BITS_RX_FQ_FULL_INT			MK_BITS(2, 1)
#define STATUS_BITS_RX_FQ_ALEMPTY_INT		MK_BITS(1, 1)
#define STATUS_BITS_RX_FQ_ALFULL_INT			MK_BITS(0, 1)

#define DESC_WR_RD_ENA				0x05CC

#define IN_QUEUE_TH						0x05d8
#define BITS_TX_RQ_IN_TH				MK_BITS(16, 8)
#define BITS_RX_BQ_IN_TH				MK_BITS(0, 8)

#define OUT_QUEUE_TH					0x05dc
#define BITS_TX_BQ_OUT_TH				MK_BITS(16, 8)
#define BITS_RX_FQ_OUT_TH				MK_BITS(0, 8)

#define RX_BQ_IN_TIMEOUT_TH			0x05E0
#define BITS_RX_BQ_IN_TIMEOUT_TH		MK_BITS(0, 24)

#define TX_RQ_IN_TIMEOUT_TH			0x05e4
#define BITS_TX_RQ_IN_TIMEOUT_TH		MK_BITS(0, 24)

#define STOP_CMD						0x05e8
#define BITS_TX_STOP_EN				MK_BITS(1, 1)
#define BITS_RX_STOP_EN				MK_BITS(0, 1)

#define FLUSH_CMD						0x05EC
#define BITS_TX_FLUSH_CMD				MK_BITS(3, 1)
#define BITS_RX_FLUSH_CMD				MK_BITS(2, 1)
#define BITS_TX_FLUSH_FLAG				MK_BITS(1, 1)
#define BITS_RX_FLUSH_FLAG				MK_BITS(0, 1)

#define  RAW_INT_ALL_MASK 0xffffffff

int higmac_set_linkstat(struct higmac_netdev_local *ld, int mode);
int higmac_hw_set_macaddress(struct higmac_netdev_local *ld, unsigned char *mac);
int higmac_hw_get_macaddress(struct higmac_netdev_local *ld, unsigned char *mac);
int higmac_glb_preinit_dummy(struct higmac_netdev_local *ld);
void higmac_sys_init(void);
void higmac_sys_exit(void);
void higmac_sys_allstop(void);
int higmac_set_hwq_depth(struct higmac_netdev_local *ld);
int higmac_set_rx_fq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr);
int higmac_set_rx_bq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr);
int higmac_set_tx_bq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr);
int higmac_set_tx_rq_hwq_addr(struct higmac_netdev_local *ld, void* phy_addr);
int higmac_clear_irqstatus(struct higmac_netdev_local *ld, int irqs);
int higmac_read_irqstatus(struct higmac_netdev_local *ld);
int higmac_irq_enable(struct higmac_netdev_local *ld, int irqs);
int higmac_irq_disable(struct higmac_netdev_local *ld, int irqs);
int higmac_desc_enable(struct higmac_netdev_local *ld, int desc_ena);
int higmac_desc_disable(struct higmac_netdev_local *ld, int desc_dis);
void higmac_desc_flush(struct higmac_netdev_local *ld);

#endif

