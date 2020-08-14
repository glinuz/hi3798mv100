#ifndef __STMMAC_H__
#define __STMMAC_H__

#include <configs/godbox.h>
#include <common.h>
#include <command.h>
#include <linux/list.h>
#include <asm/io.h>
#include <malloc.h>         /* malloc, free, realloc*/
#include <net.h>
#include <miiphy.h>

#define print_mac(mac)	 do { int i;\
	printf("MAC:  ");\
	for (i = 0; i < MAC_LEN; i++)\
		printf("%c%02X", i ? '-' : ' ', *(((unsigned char *)mac)+i));\
	printf("\n");\
} while (0)

#define MAC_LEN 6
#define MAX_PHY_NAME_LEN 6

#define MIN_PKG_LEN (42)
#define MAX_PKG_LEN	(1600)

#define STMMAC_INVALID_RXPKG_LEN(len) \
	(!(((len) >= MIN_PKG_LEN) && ((len) <= MAX_PKG_LEN)))

#define PORT_MOD_10M_MII	0
#define PORT_MOD_100M_MII	1
#define PORT_MOD_1000M_GMII	2
#define PORT_MOD_10M_RGMII	3
#define PORT_MOD_100M_RGMII	4
#define PORT_MOD_1000M_RGMII	5

#define STMMAC_LINKED		(1 << 0)
#define STMMAC_DUP_FULL	(1 << 1)
#define STMMAC_SPD_10M		(1 << 2)
#define STMMAC_SPD_100M	(1 << 3)
#define STMMAC_SPD_1000M	(1 << 4)

#define STMMAC_IOBASE STMMAC_IOADDR

#define STMMAC_PHYID STMMAC_PHYADDR

#define SZ_1K 1024

#define STMMAC_HW_QUEUE_DEPTH	1

#define STMMAC_MAX_FRAME_SIZE	(SZ_1K*2)
#define SKB_SIZE		(STMMAC_MAX_FRAME_SIZE)

struct dma_desc {
  unsigned int volatile status;
  unsigned int volatile length;
  unsigned int volatile buffer1;
  unsigned int volatile buffer2;
};

struct stmmac_netdev_local {
	unsigned long iobase;	/* virtual io addr */
	unsigned long iobase_phys; /* physical io addr */

	struct dma_desc *rx_addr;
	struct dma_desc *tx_addr;

	char *phy_name;
	int link_stat;

};

#endif
