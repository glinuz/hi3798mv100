
#include "hieth.h"
#include "mdio.h"
#include <config.h>

/* MDIO Bus Interface */
static struct hieth_mdio_local mdio_bus_ld;

int hieth_mdiobus_read(char *devname, unsigned char addr,
			      unsigned char reg, unsigned short *value)
{
	*value = hieth_mdio_read(&mdio_bus_ld, addr, reg);

	return 0;
}

int hieth_mdiobus_write(char *devname, unsigned char addr,
			       unsigned char reg, unsigned short value)
{
	hieth_mdio_write(&mdio_bus_ld, addr, reg, value);

	return 0;
}

//added by wzh 2009-4-15
unsigned int get_phy_device(char *devname, unsigned char phyaddr)
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

	/* PHY-KSZ8051MNL */
	if ((phy_id & 0xFFFFFFF0) == 0x221550) {
		unsigned short reg;
		if (miiphy_read(devname, phyaddr, 0x1F, &reg) != 0) {
			printf("PHY reg read failed\n");
			return -1;
		};
		reg |= (1 << 7);       /* set phy RMII 50MHz clk; */
		if (miiphy_write(devname, phyaddr, 0x1F, reg) != 0)
			return -1;

		if (miiphy_read(devname, phyaddr, 0x16, &reg) != 0) {
			printf("PHY reg read failed\n");
			return -1;
		};
		reg |= (1 << 1);       /* set phy RMII override; */
		if (miiphy_write(devname, phyaddr, 0x16, reg) != 0)
			return -1;
	}

	/* PHY-KSZ8081RNB */
	if ((phy_id & 0xFFFFFFF0) == 0x221560) {
		unsigned short reg;
		if (miiphy_read(devname, phyaddr, 0x1F, &reg) != 0) {
			printf("PHY IDR1 read failed\n");
			return -1;
		};
		reg |= (1 << 7);       /* set phy RMII 50MHz clk; */
		if (miiphy_write(devname, phyaddr, 0x1F, reg) != 0)
			return -1;
	}

	return 0;
}

int hieth_mdiobus_driver_init(void)
{
	mdio_bus_ld.iobase_phys = ETH_IO_ADDRESS_BASE;
	mdio_bus_ld.mdio_frqdiv = ETH_MDIO_FRQDIV;

	hieth_mdio_init(&mdio_bus_ld);

	return 0;
}

void hieth_mdiobus_driver_exit(void)
{
	hieth_mdio_exit(&mdio_bus_ld);
}
