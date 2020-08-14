#ifndef __HIGMAC_MDIO_H__
#define __HIGMAC_MDIO_H__

struct higmac_mdio_local
{
	unsigned long iobase;
	unsigned long iobase_phys;
									
	union
	{
		unsigned long w32_bits_flags;
		unsigned int mdio_frqdiv;
	};
	
};

/*FIXME*/
#define HIGMAC_MDIO_IO_BASE       0x10090000
#define HIGMAC_MDIO_IO_SIZE       (0x10000)
#define HIGMAC_MDIO_FRQDIV 			(0)

#define		REG_MDIO_SINGLE_CMD			0x000003C0
#define		REG_MDIO_SINGLE_DATA			0x000003C4
#define		REG_MDIO_RDATA_STATUS		0x000003D0

#define		mdio_cmd					MK_BITS(20, 1)  //0:mdio operation done,1: start mdio operation
#define		mdio_wr_data				MK_BITS(0, 16)
#define		mdio_rdata_status			MK_BITS(0, 1)   

#define		mdio_cmd_read (2)
#define		mdio_cmd_write (1)

#define mdio_mk_rwctl(rw, phy_exaddr, phy_regnum) \
		(((0x1)<<20 ) | \
		(((rw)&0x3)<<16 ) | \
		(((phy_exaddr)&0x3)<<8) | \
		((phy_regnum)&0x1f))

#define mdio_start_phyread(ld, phy_addr, regnum ) \
		higmac_writel(ld, mdio_mk_rwctl(mdio_cmd_read,phy_addr,regnum), REG_MDIO_SINGLE_CMD)

#define mdio_get_phyread_val(ld) (higmac_readl(ld, REG_MDIO_SINGLE_DATA) >> 16)

#define mdio_set_phywrite_val(ld,val) \
do{	\
		int reg = 0;	\
		higmac_writel_bits(ld, val, REG_MDIO_SINGLE_DATA, mdio_wr_data);\
		reg = higmac_readl(ld, REG_MDIO_SINGLE_DATA);	\
		higmac_trace(2,"write reg 0x%x, bits:0x%x= 0x%x, then read = 0x%x", REG_MDIO_SINGLE_DATA, mdio_wr_data, val, reg);\
}while(0)

#define mdio_phywrite(ld, phy_addr, regnum) \
		higmac_writel(ld, mdio_mk_rwctl(mdio_cmd_write,phy_addr, regnum), REG_MDIO_SINGLE_CMD)
		
#define test_mdio_ready(ld)     (higmac_readl_bits((ld), REG_MDIO_SINGLE_CMD, mdio_cmd) == 0)

#define test_mdio_read_data_done(ld)    (higmac_readl_bits(ld, REG_MDIO_RDATA_STATUS, mdio_rdata_status) == 0)

int higmac_mdio_read(struct higmac_mdio_local* ld, int phy_addr, int reg);

int higmac_mdio_write(struct higmac_mdio_local *ld, int phy_addr, int regnum, int val);

int  higmac_mdio_reset(struct higmac_mdio_local *ld);

int  higmac_mdio_init(struct higmac_mdio_local *ld);

void higmac_mdio_exit(struct higmac_mdio_local *ld);

int higmac_mdiobus_driver_init(void);

void higmac_mdiobus_driver_exit(void);

#endif

