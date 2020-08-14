#ifndef __HIGMAC_MDIO_H__
#define __HIGMAC_MDIO_H__

#include "higmac.h"

/* FIXME: s40v200 may change */
#define		HIGMAC_MDIO_IO_BASE		0x10090000
#define		HIGMAC_MDIO_IO_SIZE		(0x1000)
#define		HIGMAC_MDIO_FRQDIV		(0)

#define		REG_MDIO_SINGLE_CMD		0x000003C0
#define		REG_MDIO_SINGLE_DATA		0x000003C4
#define		REG_MDIO_RDATA_STATUS		0x000003D0

/* 0:mdio operation done, 1: start mdio operation */
#define		mdio_cmd			MK_BITS(20, 1)
#define		mdio_wr_data			MK_BITS(0, 16)
#define		mdio_rdata_status		MK_BITS(0, 1)

#define		mdio_cmd_read			(2)
#define		mdio_cmd_write			(1)

#define higmac_mdio_readl(ld, ofs)	\
		({ unsigned int reg = readl((ld)->gmac_iobase + (ofs)); \
		higmac_trace(2, "readl(0x%04X) = 0x%08X", (ofs), reg); reg; })

#define higmac_mdio_writel(ld, v, ofs)	\
	do { writel(v, (ld)->gmac_iobase + (ofs));\
		higmac_trace(2, "writel(0x%04X) = 0x%08X", (ofs),\
				(unsigned int)(v)); \
	} while (0)

#define higmac_mdio_writel_bits(ld, v, ofs, bits_desc) \
	do { \
		unsigned int _bits_desc = bits_desc; \
		unsigned int _shift = (_bits_desc) >> 16; \
		unsigned int _reg  = higmac_mdio_readl(ld, ofs); \
		unsigned int _mask = ((_bits_desc & 0x3F) < 32) ? \
		(((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) : 0xffffffff;\
		higmac_mdio_writel(ld, (_reg & (~_mask))\
				| (((v) << (_shift)) & _mask), ofs); \
	} while (0)

#define higmac_mdio_readl_bits(ld, ofs, bits_desc) \
	({	unsigned int _bits_desc = bits_desc; \
		unsigned int _shift = (_bits_desc) >> 16; \
		unsigned int _mask = ((_bits_desc & 0x3F) < 32) ? \
		 (((1 << (_bits_desc & 0x3F)) - 1) << (_shift)) : 0xffffffff; \
		(higmac_mdio_readl(ld, ofs) & _mask) >> (_shift); })

#define mdio_mk_rwctl(rw, phy_exaddr, phy_regnum) \
		(((0x1)<<20) | \
		(((rw)&0x3)<<16) | \
		(((phy_exaddr)&0x1f)<<8) | \
		((phy_regnum)&0x1f))

#define mdio_start_phyread(ld, phy_addr, regnum) \
		higmac_mdio_writel(ld,\
			mdio_mk_rwctl(mdio_cmd_read, phy_addr, regnum),\
			REG_MDIO_SINGLE_CMD)

#define mdio_get_phyread_val(ld)	\
	(higmac_mdio_readl(ld, REG_MDIO_SINGLE_DATA) >> 16)

#define mdio_set_phywrite_val(ld, val)	\
do { \
	int reg = 0;	\
	higmac_mdio_writel_bits(ld, val, REG_MDIO_SINGLE_DATA, mdio_wr_data);\
	reg = higmac_mdio_readl(ld, REG_MDIO_SINGLE_DATA);	\
	higmac_trace(2, "write reg 0x%x, bits:0x%x= 0x%x, then read = 0x%x",\
			REG_MDIO_SINGLE_DATA, mdio_wr_data, val, reg);\
} while (0)

#define mdio_phywrite(ld, phy_addr, regnum) \
		higmac_mdio_writel(ld,\
			mdio_mk_rwctl(mdio_cmd_write, phy_addr, regnum),\
				REG_MDIO_SINGLE_CMD)

#define test_mdio_ready(ld)		\
	(higmac_mdio_readl_bits((ld), REG_MDIO_SINGLE_CMD, mdio_cmd) == 0)

#define test_mdio_read_data_done(ld)    \
	(higmac_mdio_readl_bits(ld, REG_MDIO_RDATA_STATUS, mdio_rdata_status)\
		 == 0)

int higmac_mdio_read(struct mii_bus *bus, int phy, int reg);
int higmac_mdio_write(struct mii_bus *bus, int phy, int reg, u16 val);

#endif
