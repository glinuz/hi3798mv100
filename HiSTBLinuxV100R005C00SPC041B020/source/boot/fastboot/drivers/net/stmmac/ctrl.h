#ifndef __STMMAC_CTRL_H
#define __STMMAC_CTRL_H

#define STMMAC_SYS_CTL_IO_BASE STMMAC_IOADDR

/*GMAC Register Mapping*/
#define MAC_CTRL_REG		0x00000000	/* MAC Control */
#define GMAC_FRAME_FILTER	0x00000004	/* Frame Filter */
#define GMAC_GMII_ADDR		0x00000010
#define GMAC_GMII_DATA		0x00000014
#define GMAC_INT_STATUS		0x00000038	/* interrupt status register */
#define GMAC_INT_MASK		0x0000003c	/* interrupt mask register */
#define GMAC_ADDR_HIGH		0x00000040
#define GMAC_ADDR_LOW		0x00000044

/*MMC Register Mapping */
#define MMC_INTR_MASK_RX        0x0000010c
#define MMC_INTR_MASK_TX	0x00000110
#define MMC_IPC_INTR_MASK_RX	0x00000200

/*DMA Register Mapping */
#define DMA_BUS_MODE		0x00001000	/* Bus Mode */
#define DMA_XMT_POLL_DEMAND	0x00001004	/* Transmit Poll Demand */
#define DMA_RCV_POLL_DEMAND	0x00001008	/* Received Poll Demand */
#define DMA_RCV_BASE_ADDR	0x0000100c	/* Receive List Base */
#define DMA_TX_BASE_ADDR	0x00001010	/* Transmit List Base */
#define DMA_STATUS		0x00001014	/* Status Register */
#define DMA_CONTROL		0x00001018	/* Ctrl (Operational Mode) */
#define DMA_INTR_ENA		0x0000101c	/* Interrupt Enable */
#define DMA_MISSED_FRAME_CTR	0x00001020	/* Missed Frame Counter */
#define DMA_CUR_TX_BUF_ADDR	0x00001050	/* Current Host Tx Buffer */
#define DMA_CUR_RX_BUF_ADDR	0x00001054	/* Current Host Rx Buffer */

/*DMA Bus Mode register defines */
#define DMA_BUS_MODE_SFT_RESET	0x00000001	/* Software Reset */
#define DMA_BURSTLENGTH16		0x00001000
#define DMA_DESCRIPTORSKIP0		0x00000000
#define DMA_DESCRIPTOR4WORDS		0x00000000

/*MACCONTROL*/
#define MACWATCHDOG		0x00800000
#define MACJABBER			0x00400000
#define MACFRAMEBURST		0x00200000
#define MACMIIGMII			0x00008000
#define MACFESpeed			0x00004000
#define MACRXOwn			0x00002000
#define MACDuplex			0x00000800
#define MAC_ENABLE_TX		0x00000008	/* Transmitter Enable */
#define MAC_RNABLE_RX		0x00000004	/* Receiver Enable */
#define MACENABLECRCSTRIP	0x00000080

/*Frame Filter */
#define GMAC_FILTER_RCV_ALL		0x80000000
#define GMAC_FRAME_FILTER_PM		0x00000010	/* Pass all multicast */
#define GMAC_FRAME_FILTER_DBF		0x00000020
#define GMAC_SA_FILTER_EN		0x00000200	/* MAC Filter enable */

/*DMA Control register defines */
#define DMA_CONTROL_ST		0x00002000	/* Start/Stop Transmission */
#define DMA_CONTROL_SR		0x00000002	/* Start/Stop Receive */

/*Descriptor defines */
#define DESCOWNBYDMA		0x80000000
#define RXDESCENDOFRING	0x00008000
#define TXDESCENDOFRING	0x00200000

#define DESCTXSIZESHIFT		0x00000000
#define DESCTXSIZEMASK		0x00001FFF

#define DESCRXSIZESHIFT		0x00000000
#define DESCRXSIZEMASK		0x00001FFF


#define DESCTXFIRST			0x10000000
#define DESCTXLAST			0x20000000

#define DESCRXLENTHMASK	0x3FFF0000
#define DESCRXLENTHSHIFT	16


int stmmac_set_linkstat(struct stmmac_netdev_local *ld, int mode);
int stmmac_hw_set_macaddress(struct stmmac_netdev_local *ld,\
						unsigned char *mac);
int stmmac_hw_get_macaddress(struct stmmac_netdev_local *ld,\
						unsigned char *mac);
void stmmac_sys_init(struct stmmac_netdev_local *ld);

#endif
