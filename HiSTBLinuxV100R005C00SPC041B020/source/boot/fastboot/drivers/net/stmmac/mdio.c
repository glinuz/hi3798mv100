#include "mdio.h"
#include "ctrl.h"
#include "stmmac.h"

#define MII_BUSY 0x00000001
#define MII_WRITE 0x00000002

struct stmmac_mdio_local stmmac_mdio_local_device;

int stmmac_mdio_read(struct stmmac_mdio_local *ld, int phyaddr, int phyreg)
{
	unsigned long ioaddr = ld->iobase_phys;
	unsigned int mii_address = GMAC_GMII_ADDR;
	unsigned int mii_data = GMAC_GMII_DATA;

	int data;
	u16 regValue = (((phyaddr << 11) & (0x0000F800)) |
			((phyreg << 6) & (0x000007C0)));
	regValue |= MII_BUSY;	/* in case of GMAC */

	do {} while (((readl(ioaddr + mii_address)) & MII_BUSY) == 1);
	writel(regValue, ioaddr + mii_address);
	do {} while (((readl(ioaddr + mii_address)) & MII_BUSY) == 1);

	/*Read the data from the MII data register */
	data = (int)readl(ioaddr + mii_data);

	return data;
}

int stmmac_mdio_write(struct stmmac_mdio_local *ld, int phyaddr, int phyreg,
			    unsigned phydata)
{
	unsigned long ioaddr = ld->iobase_phys;
	unsigned int mii_address = GMAC_GMII_ADDR;
	unsigned int mii_data = GMAC_GMII_DATA;

	u16 value =
	   (((phyaddr << 11) & (0x0000F800)) | ((phyreg << 6) & (0x000007C0)))
	   | MII_WRITE;

	value |= MII_BUSY;

	/*Wait until any existing MII operation is complete */
	do {} while (((readl(ioaddr + mii_address)) & MII_BUSY) == 1);

	/*Set the MII address register to write */
	writel(phydata, ioaddr + mii_data);
	writel(value, ioaddr + mii_address);

	/*Wait until any existing MII operation is complete */
	do {} while (((readl(ioaddr + mii_address)) & MII_BUSY) == 1);

	return 0;
}

static int stmmac_mdiobus_read(char *devname, unsigned char addr,
       unsigned char reg, unsigned short *value)
{
   *value = stmmac_mdio_read(&stmmac_mdio_local_device, addr, reg);

	return 0;
}

static int stmmac_mdiobus_write(char *devname, unsigned char addr,
       unsigned char reg, unsigned short val)
{
   stmmac_mdio_write(&stmmac_mdio_local_device, addr, reg, val);
	return 0;
}

int stmmac_mdiobus_driver_init(void)
{
	stmmac_mdio_local_device.iobase_phys = STMMAC_MDIO_IO_BASE;
	stmmac_mdio_local_device.iobase = STMMAC_MDIO_IO_BASE;

	miiphy_register(PHY_NAME, stmmac_mdiobus_read, stmmac_mdiobus_write);

	miiphy_set_current_dev(PHY_NAME);

	return 0;
}

void stmmac_mdiobus_driver_exit(void)
{

   if (!miiphy_get_current_dev())
	return;
}
