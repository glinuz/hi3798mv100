#ifndef __HIGMAC_FEPHY_FIX_H
#define __HIGMAC_FEPHY_FIX_H

/* MMD: MDIO Manageable Device */
#define MACR            0x0D
#define MAADR           0x0E

#define MII_MISC_CTL			0x16
#define MII_EXPMD			0x1D
#define MII_EXPMA			0x1E

#define MII_ATH_DEBUG_ADDR		0x1D
#define MII_ATH_DEBUG_DATA		0x1E

#define XMODE_8bit_RW_NO_AUTO_INC	0x0
#define XMODE_8bit_RW_AUTO_INC		0x1
#define XMODE_16bit_RW_AUTO_INC		0x2
#define XMODE_8bit_WO_BUNDLED_ADDR	0x3

#define REALTEK_PHY_ID_8211EG		(0x001cc915)
#define REALTEK_PHY_MASK		(0xfffffff0)
#define HISILICON_PHY_ID_FE_S40		(0x20669813)
#define HISILICON_PHY_ID_FESTAV200	(0x20669823)
#define HISILICON_PHY_ID_FESTAV300      (0x20669843)
#define HISILICON_PHY_MASK		(0xfffffff0)

/* the following two copied from phy_quirk()
 * in "./drivers/net/ethernet/hieth-sf/net.c"
 */
#define PHY_ID_KSZ8051MNL		(0x00221550)
#define PHY_ID_KSZ8081RNB		(0x00221560)
#define PHY_ID_KSZ9031RNX		(0x00221620)
#define DEFAULT_PHY_MASK		(0xfffffff0)

#define ATH8035_PHY_ID			(0x004dd072)
#define ATH_PHY_ID_MASK			(0xffffffef)

int phy_mmd_read(struct phy_device *phy_dev, u32 mmd_device, u32 regnum);
int phy_mmd_write(struct phy_device *phy_dev, u32 mmd_device, u32 regnum, u16 val);

void phy_register_fixups(void);
#endif
