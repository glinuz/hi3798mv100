#ifndef SECURE_GIC_OFFSET_H
#define SECURE_GIC_OFFSET_H
/* Distributor interface r,egisters */
#define GICD_CTL                     0x0
#define GICD_CTR                     0x4
#define GICD_SEC_BASE               0x80
#define GICD_SGI_SEC_BANK           0x80
#define GICD_PPI_SEC_BANK           0x82
#define GICD_SPI_SEC                0x84 //~0xFC
#define GICD_ENABLESET              0x100
#define GICD_PPI_ENABLESET         0x102
#define GICD_SPI_ENABLESET         0x104 //~0x17C
#define GICD_ENABLECLEAR            0x180
#define GICD_PENDINGSET             0x200
#define GICD_PENDINGCLEAR           0x280
#define GICD_ACTIVESET              0x300
#define GICD_ACTIVECLEAR            0x380
#define GICD_SGI_PRI               0x400
#define GICD_PPI_PRI               0x410
#define GICD_SPI_PRI               0x420 //~0x7FB
#define GICD_CPUS                   0x800
#define GICD_CONFIG                 0xC00
#define GICD_SPI_CONFIG            0xC08
#define GICD_SW                     0xF00
#define GICD_CPENDSGIR              0xF10
#define GICD_SPENDSGIR              0xF20

/* Physical CPU Interface registers */
#define GICC_CTL                     0x0
#define GICC_PRIMASK                 0x4
#define GICC_BP                      0x8
#define GICC_INTACK                  0xC
#define GICC_EOI                    0x10
#define GICC_RUNNINGPRI             0x14
#define GICC_HIGHESTPEND            0x18
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_BALONG_V722_PLATFORM)
#define GICC_ALIASED_BP             0x1C
#endif
#define GICC_DEACTIVATE             0x1000
#endif /* SECURE_GIC_OFFSET_H */
