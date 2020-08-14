#include "higmac.h"
#include "phy_fix.h"

static const u32 phy_fix_param[] = {
#include "festa_v200.h"
};

static const u32 phy_v300_fix_param[] = {
#include "festa_v300_2204.h"
};

static int __maybe_unused set_phy_expanded_access_mode(struct phy_device *phy_dev, int access_mode)
{
	int v, ret;

	v = phy_read(phy_dev, MII_MISC_CTL);
	v &= (~0x3);
	v |= (access_mode & 0x3);
	ret = phy_write(phy_dev, MII_MISC_CTL, v);

	return ret;
}

static int __maybe_unused phy_expanded_read(struct phy_device *phy_dev, u32 reg_addr)
{
	int v, ret;

	v = phy_read(phy_dev, MII_BMCR);
	v |= BMCR_PDOWN;
	phy_write(phy_dev, MII_BMCR, v);

	phy_write(phy_dev, MII_EXPMA, reg_addr);
	ret = phy_read(phy_dev, MII_EXPMD);

	v = phy_read(phy_dev, MII_BMCR);
	v &= (~BMCR_PDOWN);
	phy_write(phy_dev, MII_BMCR, v);

	return ret;
}

static int __maybe_unused phy_expanded_write(struct phy_device *phy_dev, u32 reg_addr, u16 val)
{
	int v, ret;

	v = phy_read(phy_dev, MII_BMCR);
	v |= BMCR_PDOWN;
	phy_write(phy_dev, MII_BMCR, v);

	phy_write(phy_dev, MII_EXPMA, reg_addr);
	ret = phy_write(phy_dev, MII_EXPMD, val);

	v = phy_read(phy_dev, MII_BMCR);
	v &= (~BMCR_PDOWN);
	phy_write(phy_dev, MII_BMCR, v);

	return ret;
}

static int phy_expanded_write_bulk(struct phy_device *phy_dev, const u32 reg_and_val[],
			    int count)
{
	int i, v, ret = 0;
	u32 reg_addr;
	u16 val;

	v = phy_read(phy_dev, MII_BMCR);
	v |= BMCR_PDOWN;
	phy_write(phy_dev, MII_BMCR, v);

	for (i = 0; i < (2 * count); i += 2) {
		reg_addr = reg_and_val[i];
		val = (u16) reg_and_val[i + 1];
		phy_write(phy_dev, MII_EXPMA, reg_addr);
		ret = phy_write(phy_dev, MII_EXPMD, val);
	}

	v = phy_read(phy_dev, MII_BMCR);
	v &= (~BMCR_PDOWN);
	phy_write(phy_dev, MII_BMCR, v);

	return ret;
}

/* fix FEPHY for better eye diagram */
static int hisilicon_fephy_fix(struct phy_device *phy_dev)
{
	int count;

	count = sizeof(phy_fix_param) / sizeof(phy_fix_param[0]);
	if (count % 2)
		pr_warn("internal FEPHY fix register count is not right.\n");
	count /= 2;

	phy_expanded_write_bulk(phy_dev, phy_fix_param, count);

	return 0;
}

static int hisilicon_fephy_v300_fix(struct phy_device *phy_dev)
{
	int count;

	count = sizeof(phy_v300_fix_param) / sizeof(phy_v300_fix_param[0]);
	if (count % 2)
		pr_warn("internal FEPHY fix register count is not right.\n");
	count /= 2;

	phy_expanded_write_bulk(phy_dev, phy_v300_fix_param, count);

	return 0;
}

/*
 * for a better Electromagnetic Compatibility
 */
static int realtek_gephy_fix(struct phy_device *phy_dev)
{
#if 0
	int v;

	pr_info("RealTek phy fix: phy id=0x%x\n", phy_dev->phy_id);

	v = phy_read(phy_dev, 16);      /* PHYCR reg */
	v |= 1 << 4;                    /* clk125 remains at logic low */
	phy_write(phy_dev, 16, v);

	phy_write(phy_dev, 31, 0x0007);	/* set to extension page */
	phy_write(phy_dev, 30, 0x00A0);	/* set to extension page 160 */

	v = phy_read(phy_dev, 26);
	v &= ~(1 << 2);			/* enable RXC SSC */
	phy_write(phy_dev, 26, v);

	phy_write(phy_dev, 31, 0x0);	/* back to page 0 */

#endif

	return 0;
}

/* copy from phy_quirk() in hieth-sf/net.c */
static int KSZ8051MNL_phy_fix(struct phy_device *phy_dev)
{
	u32 v;

	v = phy_read(phy_dev, 0x1F);
	v |= (1 << 7);       /* set phy RMII 50MHz clk; */
	phy_write(phy_dev, 0x1F, v);

	v = phy_read(phy_dev, 0x16);
	v |= (1 << 1);       /* set phy RMII override; */
	phy_write(phy_dev, 0x16, v);

	return 0;
}

/* copy from phy_quirk() in hieth-sf/net.c */
static int KSZ8081RNB_phy_fix(struct phy_device *phy_dev)
{
	u32 v;

	v = phy_read(phy_dev, 0x1F);
	v |= (1 << 7);       /* set phy RMII 50MHz clk; */
	phy_write(phy_dev, 0x1F, v);

	return 0;
}

static int KSZ9031RNX_phy_fix(struct phy_device *phy_dev)
{
	u32 v;

	/* RX_CLK Pad Skew: 1_1101(+0.84) */
	v = phy_mmd_read(phy_dev, 0x2, 0x8);
	v = (v & ~0x1F) | 0x1D;
	phy_mmd_write(phy_dev, 0x2, 0x8, v);

	return 0;
}

static int at803x_phy_debug_read(struct phy_device *phy_dev, u32 reg_addr)
{
	int ret;

	phy_write(phy_dev, MII_ATH_DEBUG_ADDR, reg_addr);
	ret = phy_read(phy_dev, MII_ATH_DEBUG_DATA);

	return ret;
}

static int at803x_phy_debug_write(struct phy_device *phy_dev, u32 reg_addr, u16 val)
{
	int ret;

	phy_write(phy_dev, MII_ATH_DEBUG_ADDR, reg_addr);
	ret = phy_write(phy_dev, MII_ATH_DEBUG_DATA, val);

	return ret;
}

static int at803x_phy_fix(struct phy_device *phy_dev)
{
	/* PHY-AR8035 */
	u16 tx_delay;
	int ret;

	/* enable rgmii tx clock delay */
	tx_delay = at803x_phy_debug_read(phy_dev, 0x05);
	tx_delay |= BIT(8);
	ret = at803x_phy_debug_write(phy_dev, 0x05, tx_delay);

	return ret;
}

void phy_register_fixups(void)
{
	phy_register_fixup_for_uid(REALTEK_PHY_ID_8211EG,
			REALTEK_PHY_MASK, realtek_gephy_fix);
	phy_register_fixup_for_uid(HISILICON_PHY_ID_FESTAV200,
			HISILICON_PHY_MASK, hisilicon_fephy_fix);
	phy_register_fixup_for_uid(HISILICON_PHY_ID_FESTAV300,
			HISILICON_PHY_MASK, hisilicon_fephy_v300_fix);
	phy_register_fixup_for_uid(PHY_ID_KSZ8051MNL,
			DEFAULT_PHY_MASK, KSZ8051MNL_phy_fix);
	phy_register_fixup_for_uid(PHY_ID_KSZ8081RNB,
			DEFAULT_PHY_MASK, KSZ8081RNB_phy_fix);
	phy_register_fixup_for_uid(PHY_ID_KSZ9031RNX,
			DEFAULT_PHY_MASK, KSZ9031RNX_phy_fix);
	phy_register_fixup_for_uid(ATH8035_PHY_ID,
			ATH_PHY_ID_MASK, at803x_phy_fix);
}
