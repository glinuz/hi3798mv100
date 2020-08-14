#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* Register definition */
#define REG_BASE_SCTL                   0xF8000000
#define REG_BASE_PERI_CTRL              0xF8A20000
#define REG_BASE_CRG                    0xF8A22000
#define REG_BASE_WDG0                   0xF8A2C000

/* - timer */
#define REG_BASE_TIMER01                0xF8002000
#define REG_TIMER_CONTROL               0x008


/* LSADC pinmux. */
#define REG_SC_IO_REUSE_SEL		0x0044
#define REG_SC_IO_REUSE_SEL_LSADC0 (1 << 10)

#define REG_SC_CLKGATE_SRST_CTRL        0x0048
#define REG_SC_LSADC_SRST_REQ      (1 << 31)
#define REG_SC_LSADC_CKEN          (1 << 30)

/* - System Contol Register */
#define REG_SC_WDG_RST_CTRL             0x0050

#define REG_SC_LSADC_CTRL               0x006C
#define REG_SC_LSADC_CTRL_EN_A     (1 << 0)
#define REG_SC_LSADC_CTRL_EN_B     (1 << 1)
#define REG_SC_LSADC_CTRL_EN_C     (1 << 2)
#define REG_SC_LSADC_CTRL_PD_CTRL  (1 << 8)
#define REG_SC_LSADC_CTRL_RESET    (1 << 9)
#define REG_SC_LSADC_DATA               0x0070
#define REG_SC_LSADC_DATA_A_MASK   0x3FF
#define REG_SC_LSADC_DATA_B_MASK   (0x3FF << 16)
#define REG_SC_GEN0                     0x0080
#define REG_SC_GEN1                     0x0084
#define REG_SC_GEN2                     0x0088
#define REG_SC_GEN3                     0x008C
#define REG_SC_GEN4                     0x0090
#define REG_SC_GEN5                     0x0094
#define REG_SC_GEN6                     0x0098
#define REG_SC_GEN7                     0x009C
#define REG_SC_GEN8                     0x00A0
#define REG_SC_GEN9                     0x00A4
#define REG_SC_GEN10                    0x00A8
#define REG_SC_GEN11                    0x00AC
#define REG_SC_GEN12                    0x00B0
#define REG_SC_GEN13                    0x00B4
#define REG_SC_GEN14                    0x00B8
#define REG_SC_GEN15                    0x00BC
#define REG_SC_GEN16                    0x00C0
#define REG_SC_GEN17                    0x00C4
#define REG_SC_GEN18                    0x00C8
#define REG_SC_GEN19                    0x00CC
#define REG_SC_GEN20                    0x00D0
#define REG_SC_GEN21                    0x00D4
#define REG_SC_GEN22                    0x00D8
#define REG_SC_GEN23                    0x00DC
#define REG_SC_GEN24                    0x00E0
#define REG_SC_GEN25                    0x00e4
#define REG_SC_GEN26                    0x00e8
#define REG_SC_GEN27                    0x00ec
#define REG_SC_GEN28                    0x00F0
#define REG_SC_GEN29                    0x00F4
#define REG_SC_GEN30                    0x00F8
#define REG_SC_GEN31                    0x00FC
#define REG_SC_GEN32                    0x0F00
#define REG_SC_GEN33                    0x0F04
#define REG_SC_GEN34                    0x0F08
#define REG_SC_GEN35                    0x0F0C
#define REG_SC_GEN36                    0x0F10
#define REG_SC_GEN37                    0x0F14
#define REG_SC_GEN38                    0x0F18
#define REG_SC_GEN39                    0x0F1C
#define REG_SC_GEN40                    0x0F20
#define REG_SC_GEN41                    0x0F24
#define REG_SC_GEN42                    0x0F28
#define REG_SC_GEN43                    0x0F2C
#define REG_SC_GEN44                    0x0F30
#define REG_SC_GEN45                    0x0F34
#define REG_SC_GEN46                    0x0F38
#define REG_SC_GEN47                    0x0F3C
#define REG_SC_GEN48                    0x0F40
#define REG_SC_GEN49                    0x0F44
#define REG_SC_GEN50                    0x0F48
#define REG_SC_GEN51                    0x0F4C
#define REG_SC_GEN52                    0x0F50
#define REG_SC_GEN53                    0x0F54
#define REG_SC_GEN54                    0x0F58
#define REG_SC_GEN55                    0x0F5C
#define REG_SC_GEN56                    0x0F60
#define REG_SC_GEN57                    0x0F64
#define REG_SC_GEN58                    0x0F68
#define REG_SC_GEN59                    0x0F6C
#define REG_SC_GEN60                    0x0F70
#define REG_SC_GEN61                    0x0F74
#define REG_SC_GEN62                    0x0F78
#define REG_SC_GEN63                    0x0F7C

#define BOARD_TYPE_MASK                 0xF

#define REG_SC_SYSID0                   0x0EE0

/* - Peripheral Register */
#define REG_START_MODE                  0x0000
#define REG_PERI_SOC_FUSE               0x840

/* EMMC CRG REG */
#define REG_PERI_CRG40                  0xA0

/* WDG CRG REG */
#define REG_PERI_CRG94                  0x178

/* SOC FUSE */
#define PERI_SOC_FUSE_1                 (REG_BASE_PERI_CTRL + 0x870)
#define OTPCTRL_DEBUG_INFO              0xF8AB011C
#define BOOTROM_DEBUG_UART              0x3c

/* Use GEN18[31] to control the enable of SVB */
#define SVB_ENABLE                      (1 << 31)

/* uart */
#define REG_BASE_UART0                  0xF8B00000

/* Boot mode */
#define BOOT_MODE_SPINAND               0x4
#define BOOT_MODE_EMMC                  0x3
#define BOOT_MODE_NAND                  0x1
#define BOOT_MODE_SPI                   0x0

#define NORMAL_BOOTMODE_OFFSET          9
#define NORMAL_BOOTMODE_MASK            7

/* DDR REG */
#define REG_DDRC_BASE                   0xF8A30000
#define REG_DDR_DMC0_CTRL_SREF          (REG_DDRC_BASE + 0x8000)
#define REG_DDR_DMC0_IN_SREF            (REG_DDRC_BASE + 0x8294)
#define REG_DDR_DMC1_CTRL_SREF          (REG_DDRC_BASE + 0x9000)
#define REG_DDR_DMC0_CURR_FUNC2         (REG_DDRC_BASE + 0x8298)
#define REG_DDR_DMC1_CURR_FUNC2         (REG_DDRC_BASE + 0x9298)
#define REG_DDR_DMC0_CFG_TIMING2        (REG_DDRC_BASE + 0x8108)
#define REG_DDR_DMC1_CFG_TIMING2        (REG_DDRC_BASE + 0x9108)
#define REG_DDR_DMC0_CFG_DDRMODE        (REG_DDRC_BASE + 0x8050)
#define REG_DDR_PHY0_INITCTRL           (REG_DDRC_BASE + 0xC004)
#define REG_DDR_PHY1_INITCTRL           (REG_DDRC_BASE + 0xE004)
#define REG_DDR_PHY0_TRAINCTRL          (REG_DDRC_BASE + 0xC048)
#define REG_DDR_DMC0_LPCTRL             (REG_DDRC_BASE + 0x8028)
#define REG_KLD_BASE                    (0xF8A90000)
#define REG_KLD_SEC_FLAG                (REG_KLD_BASE + 0x174)
#define REG_KLD_DDRSCRAMBLE_EN          (REG_KLD_BASE + 0x178)
#define NONSEC_ID_WORD                  (0x2A13C812)
#define REG_OTPC_BASE                   (0xF8AB0000)
#define REG_OTPC_DDRSCRAMBLE_EN         (REG_OTPC_BASE + 0x80)
#define REG_OTPC_DDRSCRAMBLE_MASK       (1 << 30)
#define REG_RNG_BASE                    (0xF8005000)
#define REG_RNG_DATA                    (REG_RNG_BASE + 0x204)
#define REG_RNG_CNT                     (REG_RNG_BASE + 0x208)
#define REG_SYS_BASE                    (0xF8000000)
#define REG_SYS_DDRSCRAMBLE_SEED_L      (REG_SYS_BASE + 0xed4)
#define REG_SYS_DDRSCRAMBLE_SEED_H      (REG_SYS_BASE + 0xed8)
#define REG_SYS_DDRSCRAMBLE_LOCK        (REG_SYS_BASE + 0xed0)

/* ARM Timer counter reg */
#define REG_SYS_CNT_BASE                0xF8BB0000
#define REG_SYS_CNTCR                   (REG_SYS_CNT_BASE + 0x0)
#define REG_SYS_CNTFID0                 (REG_SYS_CNT_BASE + 0x20)

/* FMC registers to check bootmode of async nand & sync nand. */
#define REG_FMC_BASE			0xF9950000
#define REG_FMC_CFG			0x0
#define REG_FMC_NFMODE_MASK		(0x7 << 13)

#define REG_MMC_BASE                    0xF9830000


#define CRG_MMC_CTRL                    (REG_BASE_CRG + 0xa0)
#define CRG_DMA                         (REG_BASE_CRG + 0x38C)

/* DMA register */
#define REG_DMA_BASE                    (0xF9870000)

/* authenticate success flag, authenticate failed */
#define AUTH_SUCCESS                    (0x3CA5965A)
#define AUTH_FAILURE                    (0xC35A69A5)

#define _HI3798M_V200                   (0x0037986200LL) //(0x0020981000LL)
#define _HI3798M_V200_MASK              (0xFFFFFFFFFFLL)
#endif
