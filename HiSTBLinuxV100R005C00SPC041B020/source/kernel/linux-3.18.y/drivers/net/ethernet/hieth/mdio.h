#ifndef __HIETH_MDIO_H
#define __HIETH_MDIO_H

#define HIETH_MDIO_FRQDIV               0

#define HIETH_MDIO_RWCTRL               0x1100
#define HIETH_MDIO_RO_DATA              0x1104
#define HIETH_U_MDIO_PHYADDR            0x0108
#define HIETH_D_MDIO_PHYADDR            0x2108
#define HIETH_U_MDIO_RO_STAT            0x010C
#define HIETH_D_MDIO_RO_STAT            0x210C
#define HIETH_U_MDIO_ANEG_CTRL          0x0110
#define HIETH_D_MDIO_ANEG_CTRL          0x2110
#define HIETH_U_MDIO_IRQENA             0x0114
#define HIETH_D_MDIO_IRQENA             0x2114

#define MDIO_MK_RWCTL(cpu_data_in, finish, rw, phy_exaddr, frq_div, phy_regnum)\
		(((cpu_data_in) << 16) | \
		  (((finish) & 0x01) << 15) | \
		  (((rw) & 0x01) << 13) | \
		  (((phy_exaddr) & 0x1F) << 8) | \
		  (((frq_div) & 0x7) << 5) | \
		  ((phy_regnum) & 0x1F))

/* hardware set bit'15 of MDIO_REG(0) if mdio ready */
#define mdio_test_ready(priv) (hieth_readl(priv->glb_base, \
			       HIETH_MDIO_RWCTRL) & (1 << 15))

#define mdio_start_phyread(priv, phy_addr, regnum) \
	hieth_writel(priv->glb_base, \
		     MDIO_MK_RWCTL(0, 0, 0, phy_addr, HIETH_MDIO_FRQDIV, \
				   regnum), \
		     HIETH_MDIO_RWCTRL)

#define mdio_get_phyread_val(priv) (hieth_readl(priv->glb_base, \
				    HIETH_MDIO_RO_DATA) & 0xFFFF)

#define mdio_phywrite(priv, phy_addr, regnum, val) \
	hieth_writel(priv->glb_base, \
		     MDIO_MK_RWCTL(val, 0, 1, phy_addr, HIETH_MDIO_FRQDIV, \
				   regnum), \
		     HIETH_MDIO_RWCTRL)

/* write mdio registers reset value */
#define mdio_reg_reset(priv) do { \
	hieth_writel(priv->glb_base, 0x00008000, HIETH_MDIO_RWCTRL); \
	hieth_writel(priv->glb_base, 0x00000001, HIETH_U_MDIO_PHYADDR); \
	hieth_writel(priv->glb_base, 0x00000001, HIETH_D_MDIO_PHYADDR); \
	hieth_writel(priv->glb_base, 0x04631EA9, HIETH_U_MDIO_ANEG_CTRL); \
	hieth_writel(priv->glb_base, 0x04631EA9, HIETH_D_MDIO_ANEG_CTRL); \
	hieth_writel(priv->glb_base, 0x00000000, HIETH_U_MDIO_IRQENA); \
	hieth_writel(priv->glb_base, 0x00000000, HIETH_D_MDIO_IRQENA); \
} while (0)

int hieth_mdiobus_driver_init(struct platform_device *pdev,
			      struct hieth_netdev_priv *priv);
void hieth_mdiobus_driver_exit(struct hieth_netdev_priv *priv);

int hieth_mdiobus_read(struct mii_bus *bus, int phy_addr, int regnum);
int hieth_mdiobus_write(struct mii_bus *bus, int phy_addr, int regnum,
                        u16 val);
#endif

/* vim: set ts=8 sw=8 tw=78: */
