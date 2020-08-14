#include <linux/phy.h>
#include "../higmac.h"
#include "autoeee.h"

void init_autoeee(struct higmac_netdev_local *ld)
{
	int phy_id = ld->phy->phy_id;
	int eee_available, lp_eee_capable, v;
	struct phy_info *phy_info;
	int link_stat = 0;

	if (ld->eee_init)
		goto eee_init;

	phy_info = phy_search_ids(phy_id);
	if (phy_info) {
		eee_available = phy_info->eee_available;
		if (netif_msg_wol(ld))
			pr_info("fit phy_id:0x%x, phy_name:%s, eee:%d\n",
				phy_info->phy_id, phy_info->name,
				eee_available);

		if (!eee_available)
			goto not_support;

		if (eee_available == PHY_EEE) {
			if (netif_msg_wol(ld))
				pr_info("enter phy-EEE mode\n");

			v = readl(ld->gmac_iobase + EEE_ENABLE);
			v &= ~BIT_EEE_ENABLE;	/* disable auto-EEE */
			writel(v, ld->gmac_iobase + EEE_ENABLE);
			return;
		}

		ld->eee_init = phy_info->eee_init;
eee_init:
		switch (ld->phy->speed) {
		case SPEED_10:
			link_stat |= HIGMAC_SPD_10M;
			break;
		case SPEED_100:
			link_stat |= HIGMAC_SPD_100M;
			break;
		case SPEED_1000:
			link_stat |= HIGMAC_SPD_1000M;
			break;
		default:
			break;
		}

		lp_eee_capable = ld->eee_init(ld->phy);
		if (ld->phy->link) {
			if (lp_eee_capable & link_stat) {
				/* EEE_1us: 0x7c for 125M */
				writel(0x7c, ld->gmac_iobase +
				       EEE_TIME_CLK_CNT);
				writel(0x1e0400, ld->gmac_iobase +
				       EEE_TIMER);/* FIXME */

				v = readl(ld->gmac_iobase + EEE_LINK_STATUS);
				v |= 0x3 << 1;	/* auto EEE and ... */
				v |= BIT_PHY_LINK_STATUS;	/* phy linkup */
				writel(v, ld->gmac_iobase + EEE_LINK_STATUS);

				v = readl(ld->gmac_iobase + EEE_ENABLE);
				v |= BIT_EEE_ENABLE;	/* enable EEE */
				writel(v, ld->gmac_iobase + EEE_ENABLE);

				if (netif_msg_wol(ld))
					pr_info("enter auto-EEE mode\n");
			} else {
				if (netif_msg_wol(ld))
					pr_info("link partner not support EEE\n");
			}
		} else {
			v = readl(ld->gmac_iobase + EEE_LINK_STATUS);
			v &= ~(BIT_PHY_LINK_STATUS);	/* phy linkdown */
			writel(v, ld->gmac_iobase + EEE_LINK_STATUS);
		}

		return;
	}

not_support:
	ld->eee_init = NULL;
	if (netif_msg_wol(ld))
		pr_info("non-EEE mode\n");
}

void eee_phy_linkdown(struct higmac_netdev_local *ld)
{
	int v = readl(ld->gmac_iobase + EEE_LINK_STATUS);
	/* update phy link state */
	v &= ~BIT_PHY_LINK_STATUS;
	writel(v, ld->gmac_iobase + EEE_LINK_STATUS);
}

void eee_phy_linkup(struct higmac_netdev_local *ld)
{
	int v = readl(ld->gmac_iobase + EEE_LINK_STATUS);
	/* update phy link state */
	v |= BIT_PHY_LINK_STATUS;
	writel(v, ld->gmac_iobase + EEE_LINK_STATUS);
}
