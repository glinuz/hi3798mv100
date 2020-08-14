#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/phy.h>
#include "../higmac.h"
#include "../phy_fix.h"
#include "autoeee.h"

struct phy_info phy_info_table[];

struct phy_info *phy_search_ids(int phy_id)
{
	int i;
	struct phy_info *fit_info = NULL;

	for (i = 0; phy_info_table[i].name != NULL; i++) {
		if (phy_id == phy_info_table[i].phy_id)
			fit_info = &phy_info_table[i];
	}

	return fit_info;
}

inline int phy_mmd_read(struct phy_device *phy_dev,
			       u32 mmd_device, u32 regnum)
{
	phy_write(phy_dev, MACR, mmd_device);	/* function = 00 address */
	phy_write(phy_dev, MAADR, regnum);
	phy_write(phy_dev, MACR, 0x4000 | mmd_device);	/* function = 01 data */

	return phy_read(phy_dev, MAADR);
}

inline int phy_mmd_write(struct phy_device *phy_dev, u32 mmd_device,
				u32 regnum, u16 val)
{
	phy_write(phy_dev, MACR, mmd_device);	/* function = 00 address */
	phy_write(phy_dev, MAADR, regnum);
	phy_write(phy_dev, MACR, 0x4000 | mmd_device);	/* function = 01 data */

	return phy_write(phy_dev, MAADR, val);
}

static int smsc_lan8740_init(struct phy_device *phy_dev)
{
	static int first_time;
	int v, eee_type = 0;
	/* Realtek LAN 8740 start to enable eee */
	int eee_lan;

	if (!first_time) {
		eee_lan = phy_read(phy_dev, 0x10);
		eee_lan |= 0x4;
		phy_write(phy_dev, 0x10, eee_lan);
		eee_lan = phy_read(phy_dev, 0x10);
		/* auto negotiate after enable eee */
		eee_lan = phy_read(phy_dev, 0x0);
		eee_lan |= 0x200;
		phy_write(phy_dev, 0x0, eee_lan);
		first_time = 1;
	}

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);

	if (v & LP_1000BASE_EEE)
		eee_type |= HIGMAC_SPD_1000M;
	if (v & LP_100BASE_EEE)
		eee_type |= HIGMAC_SPD_100M;

	return eee_type;
}

#define RTL8211EG_MAC	0
#if RTL8211EG_MAC
static int rtl8211EG_mac_init(struct phy_device *phy_dev)
{
	static int first_time;
	/* Realtek 8211EG start reset to change eee to mac */
	int tmp = 0, v, eee_type = 0;

	if (!first_time) {
		phy_write(phy_dev, 0x1f, 0x0);
		phy_write(phy_dev, MII_BMCR, BMCR_RESET);	/* reset phy */
		do {		/* wait phy restart over */
			udelay(1);
			tmp = phy_read(phy_dev, MII_BMSR);
			/* no need to wait AN finished */
			tmp &= (BMSR_ANEGCOMPLETE | BMSR_ANEGCAPABLE);
		} while (!tmp);

		phy_write(phy_dev, 0x1f, 0x7);
		phy_write(phy_dev, 0x1e, 0x20);
		phy_write(phy_dev, 0x1b, 0xa03a);
		phy_write(phy_dev, 0x1f, 0x0);

		first_time = 1;
	}

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);

	if (v & LP_1000BASE_EEE)
		eee_type |= HIGMAC_SPD_1000M;
	if (v & LP_100BASE_EEE)
		eee_type |= HIGMAC_SPD_100M;

	return eee_type;
}
#else
static int rtl8211EG_init(struct phy_device *phy_dev)
{
	int eee_type = 0, v;

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);

	if (v & LP_1000BASE_EEE)
		eee_type |= HIGMAC_SPD_1000M;
	if (v & LP_100BASE_EEE)
		eee_type |= HIGMAC_SPD_100M;

	return eee_type;
}
#endif

static int festa_v200_init(struct phy_device *phy_dev)
{
	static int first_time_init;
	int v, eee_type = 0;

	if (!first_time_init) {
		/* EEE_CAPABILITY register: support 100M-BaseT */
		v = phy_mmd_read(phy_dev, EEE_DEV, EEE_CAPABILITY);
		phy_mmd_write(phy_dev, EEE_DEV, EEE_CAPABILITY, v | (1 << 1));

		/* EEE_ADVERTISEMENT register: advertising 100M-BaseT */
		v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEE_ADVERTISE);
		phy_mmd_write(phy_dev, EEELPAR_DEV, EEE_ADVERTISE, v|(1<<1));

		v = phy_read(phy_dev, MII_BMCR);
		v |= (BMCR_ANENABLE | BMCR_ANRESTART);
		phy_write(phy_dev, MII_BMCR, v);	/* auto-neg restart */

		first_time_init = 1;
	}

	v = phy_mmd_read(phy_dev, EEELPAR_DEV, EEELPAR);

	if (v & LP_1000BASE_EEE)
		eee_type |= HIGMAC_SPD_1000M;
	if (v & LP_100BASE_EEE)
		eee_type |= HIGMAC_SPD_100M;

	return eee_type;
}

struct phy_info phy_info_table[] = {
	/* phy_name             phy_id  eee_available   phy_driver */
/* SMSC */
	{"SMSC LAN8740", 0x0007c110, MAC_EEE, &smsc_lan8740_init},
/* Realtek */
#if RTL8211EG_MAC
	{"Realtek 8211EG", 0x001cc915, MAC_EEE, &rtl8211EG_mac_init},
#else
	{"Realtek 8211EG", 0x001cc915, PHY_EEE, &rtl8211EG_init},
#endif
	{"Festa V200", HISILICON_PHY_ID_FESTAV200, MAC_EEE, &festa_v200_init},
};
