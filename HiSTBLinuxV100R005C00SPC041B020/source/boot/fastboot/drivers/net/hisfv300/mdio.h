
#ifndef __HISILICON_HIETH_MDIO_H
#define __HISILICON_HIETH_MDIO_H

struct hieth_mdio_local {
	unsigned long iobase_phys;

	/* mdio_bus freq-div, 1 for 1/100, 0 for 1/50 */
	int mdio_frqdiv;
};

#ifdef HIETH_SFV300

#define MDIO_RWCTRL		0x1100
#define MDIO_RO_DATA		0x1104
#define U_MDIO_PHYADDR		0x0108
#define D_MDIO_PHYADDR		0x2108
#define U_MDIO_RO_STAT		0x010C
#define D_MDIO_RO_STAT		0x210C
#define U_MDIO_ANEG_CTRL	0x0110
#define D_MDIO_ANEG_CTRL	0x2110
#define U_MDIO_IRQENA		0x0114
#define D_MDIO_IRQENA		0x2114

#define MDIO_MK_RWCTL( cpu_data_in, finish, rw, phy_exaddr, frq_div, phy_regnum) \
		( ((cpu_data_in)<<16 ) | \
		  (((finish)&0x01)<<15 ) | \
		  (((rw)&0x01)<<13 ) | \
		  (((phy_exaddr)&0x1F)<<8) | \
		  (((frq_div)&0x7)<<5) | \
		  ((phy_regnum)&0x1F) )

/* hardware set bit'15 of MDIO_REG(0) if mdio ready */
#define test_mdio_ready(ld) (hieth_readl(ld, MDIO_RWCTRL)&(1<<15))

#define mdio_start_phyread(ld, phy_addr, regnum ) \
	hieth_writel(ld, MDIO_MK_RWCTL(0,0,0,phy_addr,(ld)->mdio_frqdiv,regnum), MDIO_RWCTRL)

#define mdio_get_phyread_val(ld) (hieth_readl(ld, MDIO_RO_DATA) & 0xFFFF)

#define mdio_phywrite(ld, phy_addr, regnum, val) \
	hieth_writel(ld, MDIO_MK_RWCTL(val,0,1,phy_addr,(ld)->mdio_frqdiv,regnum), MDIO_RWCTRL)

/* write mdio registers reset value */
#define mdio_reg_reset(ld) do{ \
		hieth_writel(ld, 0x00008000, MDIO_RWCTRL); \
		hieth_writel(ld, 0x00000001, U_MDIO_PHYADDR); \
		hieth_writel(ld, 0x00000001, D_MDIO_PHYADDR); \
		hieth_writel(ld, 0x04631EA9, U_MDIO_ANEG_CTRL); \
		hieth_writel(ld, 0x04631EA9, D_MDIO_ANEG_CTRL); \
		hieth_writel(ld, 0x00000000, U_MDIO_IRQENA); \
		hieth_writel(ld, 0x00000000, D_MDIO_IRQENA); \
	} while(0)

#endif

/* APIs */

int hieth_mdio_read(struct hieth_mdio_local *ld, int phy_addr, int regnum);
int hieth_mdio_write(struct hieth_mdio_local *ld, int phy_addr, int regnum,
		     int val);
int hieth_mdio_reset(struct hieth_mdio_local *ld);
int hieth_mdio_init(struct hieth_mdio_local *ld);
void hieth_mdio_exit(struct hieth_mdio_local *ld);

#endif
