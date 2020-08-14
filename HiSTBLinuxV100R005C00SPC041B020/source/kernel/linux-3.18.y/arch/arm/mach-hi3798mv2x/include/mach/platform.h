#ifndef __HI_CHIP_REGS_H__
#define __HI_CHIP_REGS_H__

#include <mach/io.h>

/* SRAM Base Address Register */
#define SRAM_BASE_ADDRESS                               0xFFFF0000

#define S40_REG_BASE                                    0xF8000000
#define REG_BASE_SCTL                                   (S40_REG_BASE + 0)


#define REG_BASE_TIMER01                                0xF8002000
#define REG_BASE_TIMER23                                0xF8A29000
#define REG_BASE_TIMER45                                0xF8A2A000
#define REG_BASE_TIMER67                                0xF8A2B000


#define REG_BASE_SCI0                                   0x101F3000
#define REG_BASE_SCI1                                   0x101F4000
#define REG_BASE_CRG                                    0xF8A22000
#define REG_BASE_SDIO1                                  0xF9830000

#define REG_BASE_CPU_LP                                 0xF8A22048
#define REG_CPU_LP_CPU_SW_BEGIN                         10
#define CPU_REG_BASE_RST                                0xF8A22050
#define CPU_REG_COREPO_SRST                             12
#define CPU_REG_COREPDBG_SRST                           6
#define CPU_REG_CORE_SRST                               8
#define CPU_REG_CKEN                                    16

#define HIETHPHY_SYSREG_REG                             0x0388

#define REG_BASE_PMC                                    0xF8A23000
#define REG_PERI_PMC0                                   (REG_BASE_PMC)
#define REG_PERI_PMC1                                   (REG_BASE_PMC + 0x4)
#define REG_PERI_PMC2                                   (REG_BASE_PMC + 0x8)
#define REG_PERI_PMC3                                   (REG_BASE_PMC + 0xC)
#define REG_PERI_PMC4                                   (REG_BASE_PMC + 0x10)
#define REG_PERI_PMC5                                   (REG_BASE_PMC + 0x14)
#define REG_PERI_PMC6                                   (REG_BASE_PMC + 0x18)
#define REG_PERI_PMC7                                   (REG_BASE_PMC + 0x1C)
#define REG_PERI_PMC8                                   (REG_BASE_PMC + 0x20)
#define REG_PERI_PMC9                                   (REG_BASE_PMC + 0x24)
#define REG_PERI_PMC64                                  (REG_BASE_PMC + 0X100)
#define REG_PERI_PMC65                                  (REG_BASE_PMC + 0X104)
#define REG_PERI_PMC66                                  (REG_BASE_PMC + 0X108)
#define REG_PERI_PMC67                                  (REG_BASE_PMC + 0X10c)
#define REG_PERI_PMC68                                  (REG_BASE_PMC + 0X110)
#define REG_PERI_PMC76                                  (REG_BASE_PMC + 0X130)
#define REG_PERI_PMC77                                  (REG_BASE_PMC + 0X134)
#define REG_PERI_PMC78                                  (REG_BASE_PMC + 0X138)

#define REG_BASE_UART0                                  0xF8B00000
#define REG_BASE_UART1                                  0xF8006000
#define REG_BASE_UART2                                  0xF8B02000
#define REG_BASE_UART3                                  0xF8B03000
#define REG_BASE_UART4                                  0xF8B04000
#define REG_BASE_CUR_UART                               REG_BASE_UART0

#define REG_BASE_PERI_CTRL                              0xF8A20000
#define REG_BASE_PERI_CTRL_START_MODE                  (REG_BASE_PERI_CTRL)

#define REG_BASE_GIC_REG                                0xF1000000
#define REG_BASE_CSSYS                                  0x10330000
#define REG_BASE_DNR                                    0x10450000

#define REG_BASE_L2CACHE                                0xF8A10000


/*CORTTX-A9 MPCORE MEMORY REGION*/
#define REG_A9_PERI_SCU                                 0x0000
#define REG_A9_PERI_GIC_CPU                             0x2000
#define REG_A9_PERI_GLOBAL_TIMER                        0x0200
#define REG_A9_PERI_PRI_TIMER_WDT                       0x0600
#define REG_A9_PERI_GIC_DIST                            0x1000 


/* CA clock and reset register */
#define REG_PERI_CRG29                                  0x00B4

#define CA_CTRL_PROC                                    0x78
#define CA_VENDOR_OFFSET                                28
#define CA_VENDOR_MASK                                  0x0f
#define CA_TYPE_NO_CA                                   0
#define CA_TYPE_NAGRA                                   1

/* SYSTEM CONTROL REG */
#define REG_SC_CTRL                                     0x0000
#define REG_SC_SYSRES                                   0x0004
#define REG_SC_XTALCTRL                                 0x0010
#define REG_SC_APLLCTRL                                 0x0014
#define REG_SC_APLLFREQCTRL0                            0x0018
#define REG_SC_APLLFREQCTRL1                            0x001C
#define REG_SC_VPLL0FREQCTRL0                           0x0020
#define REG_SC_VPLL0FREQCTRL1                           0x0024
#define REG_SC_VPLL1FREQCTRL0                           0x0028
#define REG_SC_VPLL1FREQCTRL1                           0x002C
#define REG_SC_EPLLFREQCTRL0                            0x0030
#define REG_SC_EPLLFREQCTRL1                            0x0034
#define REG_SC_QPLLFREQCTRL0                            0x0038
#define REG_SC_QPLLFREQCTRL1                            0x003C
#define REG_SC_LOW_POWER_CTRL                           0x0040
#define REG_SC_IO_REUSE_SEL                             0x0044
#define REG_SC_SRST_REQ_CTRL                            0x0048
#define REG_SC_CA_RST_CTRL                              0x004C
#define REG_SC_WDG_RST_CTRL                             0x0050
#define REG_SC_DDRC_DFI_RST_CTRL                        0x0054
#define REG_SC_PLLLOCK_STAT                             0x0070
#define REG_SC_GEN0                                     0x0080
#define REG_SC_GEN1                                     0x0084
#define REG_SC_GEN2                                     0x0088
#define REG_SC_GEN3                                     0x008C
#define REG_SC_GEN4                                     0x0090
#define REG_SC_GEN5                                     0x0094
#define REG_SC_GEN6                                     0x0098
#define REG_SC_GEN7                                     0x009C
#define REG_SC_GEN8                                     0x00A0
#define REG_SC_GEN9                                     0x00A4
#define REG_SC_GEN10                                    0x00A8
#define REG_SC_GEN11                                    0x00AC
#define REG_SC_GEN12                                    0x00B0
#define REG_SC_GEN13                                    0x00B4
#define REG_SC_GEN14                                    0x00B8
#define REG_SC_GEN15                                    0x00BC
#define REG_SC_GEN16                                    0x00C0
#define REG_SC_GEN17                                    0x00C4
#define REG_SC_GEN18                                    0x00C8
#define REG_SC_GEN19                                    0x00CC
#define REG_SC_GEN20                                    0x00D0
#define REG_SC_GEN21                                    0x00D4
#define REG_SC_GEN22                                    0x00D8
#define REG_SC_GEN23                                    0x00DC
#define REG_SC_GEN24                                    0x00E0
#define REG_SC_GEN25                                    0x00E4
#define REG_SC_GEN26                                    0x00E8
#define REG_SC_GEN27                                    0x00EC
#define REG_SC_GEN28                                    0x00F0
#define REG_SC_GEN29                                    0x00F4
#define REG_SC_GEN30                                    0x00F8
#define REG_SC_GEN31                                    0x00FC
#define REG_SC_LOCKEN                                   0x020C
#define REG_SC_SYSID0                                   0x0EE0
#define REG_SC_SYSID1                                   0x0EE4
#define REG_SC_SYSID2                                   0x0EE8
#define REG_SC_SYSID3                                   0x0EEC

#define REG_PERI_SOC_FUSE                               0x840

#define REG_BASE_WDG0                                   0xF8A2C000
#define REG_BASE_WDG1                                   0xF8A2D000

#define CFG_GIC_CPU_BASE    (IO_ADDRESS(REG_BASE_GIC_REG) + REG_A9_PERI_GIC_CPU)
#define CFG_GIC_DIST_BASE   (IO_ADDRESS(REG_BASE_GIC_REG) + REG_A9_PERI_GIC_DIST)

#define DDR_SUSPEND_SIZE                                0x1000   /* size 1KB */

/*********************************************************************/
/*
 * 0x1-> init item1
 * 0x2-> init item2
 * 0x3->init item1 & item2
 */
#define INIT_REG_ITEM1               1
#define INIT_REG_ITEM2               2
#define INIT_REG_ITEM1_ITEM2         (INIT_REG_ITEM1 | INIT_REG_ITEM2)

#endif /*End of __HI_CHIP_REGS_H__ */
