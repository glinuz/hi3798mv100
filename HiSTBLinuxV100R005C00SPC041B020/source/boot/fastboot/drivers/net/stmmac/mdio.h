#ifndef __STMMAC_MDIO_H__
#define __STMMAC_MDIO_H__

#include "stmmac.h"

extern char PHY_NAME[MAX_PHY_NAME_LEN];
extern unsigned char PHY_ADDR ;


struct stmmac_mdio_local {
	unsigned long iobase;
	unsigned long iobase_phys;

	union {
		unsigned long w32_bits_flags;
		unsigned int mdio_frqdiv;
	};
};

#define STMMAC_MDIO_IO_BASE STMMAC_IOADDR
#define STMMAC_MDIO_IO_SIZe STMMAC_IOSIZE
#define STMMAC_MDIO_FRQDIV STMMAC_FRQDIV

int stmmac_mdio_read(struct stmmac_mdio_local *ld, int phy_addr, int reg);
int stmmac_mdio_write(struct stmmac_mdio_local *ld,\
		int phyaddr, int phyreg, unsigned phydata);
int stmmac_mdiobus_driver_init(void);
void stmmac_mdiobus_driver_exit(void);
#endif
