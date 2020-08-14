#ifndef __HIGMAC_H__
#define __HIGMAC_H__

#include <common.h>
#include <command.h>
#include <linux/list.h>
#include <asm/io.h>
#include <malloc.h>         /* malloc, free, realloc*/
#include <net.h>
#include <miiphy.h>

/* board parameters */
#define HIGMAC0_IOBASE		0xF9840000
#define HIGMAC1_IOBASE		0xF9841000
#define HIGMAC2_IOBASE		0xF9C30000
#define HIGMAC_IRQNUM		36

#define CONFIG_HIGMAC_PHY1_ADDR	1
#define CONFIG_HIGMAC_PHY2_ADDR	2
#define CONFIG_HIGMAC_PHY3_ADDR	1
#define DEFAULT_PHY_LINK_TIMES	80000

#define INVALID_PHY_ADDR	0xFF

enum if_mode {
	interface_mode_mii,
	interface_mode_rmii,
	interface_mode_rgmii,
	interface_mode_butt
};

enum speed_mode {
	speed_mode_10M,
	speed_mode_100M,
	speed_mode_1000M,
	speed_mode_butt
};

enum {/* DEFAULT: DUPLEX_FULL */
	port_mode_10_mii = 0x1D,
	port_mode_100_mii = 0x1F,
	port_mode_10_rgmii = 0x3D,
	port_mode_100_rgmii = 0x3F,
	port_mode_1000_rgmii = 0x3C,
	port_mode_10_rmii = 0x9D,
	port_mode_100_rmii = 0x9F,
	port_mode_butt
};

#define print_mac(mac)	 do{ int i;\
	printf("MAC:  ");\
	for (i = 0; i < MAC_LEN; i++)\
		printf("%c%02X", i ? '-' : ' ', *(((unsigned char*)mac)+i));\
	printf("\n");\
}while(0)

#define MAC_LEN 6
#define MAX_PHY_NAME_LEN 15

#define MIN_PKG_LEN (42)
#define MAX_PKG_LEN	(1600)
#define HIGMAC_INVALID_RXPKG_LEN(len) (!(((len) >= MIN_PKG_LEN) && ((len) <= MAX_PKG_LEN)))

#define PORT_MOD_10M_MII		0
#define PORT_MOD_100M_MII		1
#define PORT_MOD_1000M_GMII		2
#define PORT_MOD_10M_RGMII		3
#define PORT_MOD_100M_RGMII		4
#define PORT_MOD_1000M_RGMII		5

#define HIGMAC_LINKED       (1 << 0)
#define HIGMAC_DUP_FULL     (1 << 1)
#define HIGMAC_SPD_10M      (1 << 2)
#define HIGMAC_SPD_100M     (1 << 3)
#define HIGMAC_SPD_1000M    (1 << 4)

#define RX_OUTCFF_WR_DESC_ENA	(1<<3)
#define RX_CFF_RD_DESC_ENA	(1<<2)
#define TX_OUTCFF_WR_DESC_ENA	(1<<1)
#define TX_CFF_RD_DESC_ENA	(1<<0)

#define SZ_1K			1024

#define HIGMAC_MAX_QUEUE_DEPTH	(SZ_1K*2)

#define HIGMAC_HWQ_RX_FQ_DEPTH	64
#define HIGMAC_HWQ_RX_BQ_DEPTH	64
#define HIGMAC_HWQ_TX_BQ_DEPTH	2
#define HIGMAC_HWQ_TX_RQ_DEPTH	2

#define HIGMAC_MONITOR_TIMER	(msecs_to_jiffies(10))

#define MAX_RX_POOLS		(SZ_1K)
#define HIETH_MAX_FRAME_SIZE	(SZ_1K*2)
#define SKB_SIZE		(HIETH_MAX_FRAME_SIZE)

#define DESC_VLD_FREE		0
#define DESC_VLD_BUSY		1
#define DESC_FL_FIRST		2
#define DESC_FL_MID		0
#define DESC_FL_LAST		1
#define DESC_FL_FULL		3

#ifdef CONFIG_HIGMAC_DESC_4WORD
#define DESC_WORD_SHIFT		(2)
#else
#define DESC_WORD_SHIFT		(3)
#endif
#define DESC_BYTE_SHIFT		(DESC_WORD_SHIFT + 2)
#define DESC_WORD_CNT		(1 << DESC_WORD_SHIFT)
#define DESC_SIZE		(1 << DESC_BYTE_SHIFT)

typedef struct higmac_descriptor {
	unsigned int data_buff_addr;

	unsigned int buffer_len:11;
	unsigned int reserve2:5;
	unsigned int data_len:11;
	unsigned int reserve1:2;
	unsigned int fl:2;
	unsigned int descvid:1;

	unsigned int reserve3;
	unsigned int reserve4;
#ifndef CONFIG_HIGMAC_DESC_4WORD
	unsigned int reserve5;
	unsigned int reserve6;
	unsigned int reserve7;
	unsigned int reserve8;
#endif
}higmac_desc;

struct higmac_netdev_local {
	unsigned long iobase;	/* virtual io addr */
	unsigned long iobase_phys; /* physical io addr */

	higmac_desc* rx_fq_addr;
	higmac_desc* rx_bq_addr;
	higmac_desc* tx_bq_addr;
	higmac_desc* tx_rq_addr;

	int link_stat;
	int index;
};

/* this driver only support GMAC_NUMS = 2 */
#ifdef CONFIG_ARCH_HIFONE
#define CONFIG_GMAC_NUMS			(3)
#else
#if defined(CONFIG_ARCH_HI3798MV2X) || defined(CONFIG_ARCH_HI3796MV2X)
#define CONFIG_GMAC_NUMS			(1)
#else
#define CONFIG_GMAC_NUMS			(2)
#endif
#endif
struct higmac_board_info {
	struct higmac_netdev_local higmac_ld;
	enum if_mode phy_intf;
	char *mii_name;
	int phy_addr;
	u32 gpio_base;
	u32 gpio_bit;
};

extern struct higmac_board_info higmac_board_info[];
#endif
