#ifndef DDR_WATCHZONE_H
#define DDR_WATCHZONE_H

#define WADDR_SRVLNC_BASE				0xF8A35000
#define WADDR_SRVLNC_END				(WADDR_SRVLNC_BASE + 0x1000)

#define WADDR_SRVLNC_CLKGCFG				0x0000
#define WADDR_SRVLNC_DETADDR_GT_EN		(0x1 << 1)
#define WADDR_SRVLNC_APB_GT_EN			(0x1 << 0)

#define AWADDR_SRVLNC_MID				0x0004
#define AWADDR_SRVLNC_MID_MASK			0x1F

#define AWADDR_SRVLNC_START				0x0100
#define AWADDR_SRVLNC_END				0x0104

#define AWADDR_SRVLNC_STATUS				0x200

#define hiwz_read(_host, _reg) \
        readl((char *)_host->regbase + (_reg))

#define hiwz_write(_host, _value, _reg) \
        writel((_value), (char *)_host->regbase + (_reg))
      
#endif /* #define DDR_WATCHZONE_H */