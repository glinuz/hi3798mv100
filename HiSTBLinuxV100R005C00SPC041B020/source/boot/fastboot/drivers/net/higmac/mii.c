#include "mdio.h"
#include "util.h"
#include "higmac.h"

extern char PHY_NAME[MAX_PHY_NAME_LEN];
extern unsigned char PHY_ADDR ;

struct higmac_mdio_local hieth_mdio_local_device;

static int higmac_mdiobus_read(char *devname, unsigned char addr,
        unsigned char reg, unsigned short *value)
{
    *value = higmac_mdio_read(&hieth_mdio_local_device, addr, reg);

	return 0;
}

static int higmac_mdiobus_write(char *devname, unsigned char addr,
        unsigned char reg, unsigned short val)
{
    higmac_mdio_write(&hieth_mdio_local_device, addr, reg, val);
	return 0;
}

int higmac_mdiobus_driver_init(void)
{
	hieth_mdio_local_device.iobase_phys = HIGMAC_MDIO_IO_BASE;
	hieth_mdio_local_device.iobase = HIGMAC_MDIO_IO_BASE;
	higmac_mdio_init(&hieth_mdio_local_device);	

	miiphy_register(PHY_NAME,higmac_mdiobus_read,higmac_mdiobus_write);

	miiphy_set_current_dev(PHY_NAME);
	miiphy_reset(PHY_NAME, PHY_ADDR);
	
    return 0;
}

void higmac_mdiobus_driver_exit(void)
{

    if(!miiphy_get_current_dev())
        return;
	miiphy_reset(PHY_NAME,PHY_ADDR);
	higmac_mdio_exit(&hieth_mdio_local_device);
}


