#include <linux/phy.h>
#include "phy.h"
#include "hieth.h"

/*----------------------------Macro definition-------------------------------*/
#define NO_EEE          0
#define MAC_EEE         1
#define PHY_EEE         2
#define PARTNER_EEE     2

#define debug(fmt...)
struct phy_info {
	char *name;
	int phy_id;
	char eee_available;/* eee support by this phy */
	int (*eee_init)(struct phy_device *phy_dev);
};

/* GMAC register definition */
#define EEE_ENABLE		0x488
#define BIT_EEE_ENABLE		(1 << 0)
#define EEE_TIMER		0x48C
#define EEE_LINK_STATUS		0x490
#define BIT_PHY_LINK_STATUS	(1 << 0)
#define EEE_TIME_CLK_CNT	0x494

/* ----------------------------phy register-------------------------------*/
/* MMD: MDIO Manageable Device */
#define MACR		0x0D
#define MAADR		0x0E
#define EEE_DEV		0x3
#define EEE_CAPABILITY	0x14
#define	EEELPAR_DEV	0x7
#define EEELPAR		0x3D	/* EEE link partner ability register */
#define EEE_ADVERTISE	0x3c
#define LP_1000BASE_EEE	(1 << 2)
#define LP_100BASE_EEE	(1 << 1)

static struct phy_info phy_info_table[];

static struct phy_info *phy_search_ids(int phy_id)
{
	int i;
	struct phy_info *fit_info = NULL;

	for (i = 0; phy_info_table[i].name != NULL; i++) {
		if (phy_id == phy_info_table[i].phy_id)
			fit_info = &phy_info_table[i];
	}

	return fit_info;
}

static inline int phy_mmd_read(struct phy_device *phy_dev,
			       u32 mmd_device, u32 regnum)
{
	phy_write(phy_dev, MACR, mmd_device);/* function = 00 address */
	phy_write(phy_dev, MAADR, regnum);
	phy_write(phy_dev, MACR, 0x4000 | mmd_device);/* function = 01 data */

	return phy_read(phy_dev, MAADR);
}

static inline int phy_mmd_write(struct phy_device *phy_dev,
				u32 mmd_device, u32 regnum, u16 val)
{
	phy_write(phy_dev, MACR, mmd_device);/* function = 00 address */
	phy_write(phy_dev, MAADR, regnum);
	phy_write(phy_dev, MACR, 0x4000 | mmd_device);/* function = 01 data */

	return phy_write(phy_dev, MAADR, val);
}

static int smsc_lan8740_init(struct phy_device *phy_dev)
{
	static int first_time;
	int v, eee_type = 0;
	/* Realtek LAN 8740 start to enable eee */
	int eee_lan;

	if (!first_time) {
		eee_lan	= phy_read(phy_dev, 0x10);
		eee_lan |= 0x4;
		phy_write(phy_dev, 0x10, eee_lan);
		eee_lan = phy_read(phy_dev, 0x10);
		debug("eee enable bit[45?] :%x\n", eee_lan);
		/* auto negotiate after enable eee*/
		eee_lan = phy_read(phy_dev, 0x0);
		eee_lan |= 0x200;
		phy_write(phy_dev, 0x0, eee_lan);
		first_time = 1;
	}

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);
	debug("EEELPAR = 0x%x\n", v);

	if (v & LP_100BASE_EEE)
		eee_type |= HIETH_P_MAC_PORTSET_SPD_100M;

	return eee_type;
}

static int rtl8211EG_init(struct phy_device *phy_dev)
{
	int eee_type = 0, v;

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);
	debug("EEELPAR = 0x%x\n", v);

	if (v & LP_100BASE_EEE)
		eee_type |= HIETH_P_MAC_PORTSET_SPD_100M;

	return eee_type;
}

static int festa_v200_init(struct phy_device *phy_dev)
{
	static int first_time_init;
	int v, eee_type = 0;

	if (!first_time_init) {
		/* EEE_CAPABILITY register: support 100M-BaseT */
		v = phy_mmd_read(phy_dev, EEE_DEV, EEE_CAPABILITY);
		phy_mmd_write(phy_dev, EEE_DEV, EEE_CAPABILITY, v|(1<<1));

		/* EEE_ADVERTISEMENT register: advertising 100M-BaseT */
		v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEE_ADVERTISE);
		phy_mmd_write(phy_dev, EEELPAR_DEV, EEE_ADVERTISE, v|(1<<1));

		v = phy_read(phy_dev, MII_BMCR);
		v |= (BMCR_ANENABLE | BMCR_ANRESTART);
		phy_write(phy_dev, MII_BMCR, v);/* auto-neg restart */

		first_time_init = 1;
	}

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);
	debug("EEELPAR = 0x%x\n", v);

	if (v & LP_100BASE_EEE)
		eee_type |= HIETH_P_MAC_PORTSET_SPD_100M;

	return eee_type;
}

static struct phy_info phy_info_table[] = {
	/* phy_name		phy_id	eee_available	phy_driver */
	/* SMSC */
	{"SMSC LAN8740", 0x0007c110,  MAC_EEE, &smsc_lan8740_init},
	/* Realtek */
	{"Realtek 8211EG", 0x001cc915, PHY_EEE, &rtl8211EG_init},
	{"Festa V200", HISILICON_PHY_ID_FESTAV200, MAC_EEE, &festa_v200_init},
	{"Festa V300", HISILICON_PHY_ID_FESTAV300, MAC_EEE, &festa_v200_init},
	{0, 0, 0, 0},
};

void hieth_autoeee_init(struct hieth_netdev_priv *priv, int link_stat)
{
	int phy_id = priv->phy->phy_id;
	int eee_available, lp_eee_capable, v;
	struct phy_info *phy_info;

	if (priv->eee_init)
		goto eee_init;

	phy_info = phy_search_ids(phy_id);
	if (phy_info) {
		eee_available = phy_info->eee_available;
			debug("fit phy_id:0x%x, phy_name:%s, eee:%d\n",
			      phy_info->phy_id, phy_info->name, eee_available);

		if (!eee_available)
			goto not_support;

		if (eee_available == PHY_EEE) {
			debug("enter phy-EEE mode\n");
			v = readl(priv->glb_base + EEE_ENABLE);
			v &= ~BIT_EEE_ENABLE;/* disable auto-EEE */
			writel(v, priv->glb_base + EEE_ENABLE);
			return;
		}

		priv->eee_init = phy_info->eee_init;
eee_init:
		lp_eee_capable = priv->eee_init(priv->phy);
		if (link_stat & HIETH_P_MAC_PORTSET_LINKED) {
			if (lp_eee_capable & link_stat) {
				/* EEE_1us: 0x7c for 125M */
				writel(0x7c, priv->glb_base + EEE_TIME_CLK_CNT);
				writel(0x4002710, priv->glb_base + EEE_TIMER);

				v = readl(priv->glb_base + EEE_LINK_STATUS);
				v |= 0x3 << 1;/* auto EEE and ... */
				v |= BIT_PHY_LINK_STATUS;/* phy linkup */
				writel(v, priv->glb_base + EEE_LINK_STATUS);

				v = readl(priv->glb_base + EEE_ENABLE);
				v |= BIT_EEE_ENABLE;/* enable EEE */
				writel(v, priv->glb_base + EEE_ENABLE);

				debug("enter auto-EEE mode\n");
				return;
			} else {
				debug("link partner not support EEE\n");
				return;
			}
		} else {
			v = readl(priv->glb_base + EEE_LINK_STATUS);
			v &= ~(BIT_PHY_LINK_STATUS);/* phy linkdown */
			writel(v, priv->glb_base + EEE_LINK_STATUS);
			return;
		}
	}

not_support:
	priv->eee_init = NULL;
	debug("non-EEE mode\n");
}

