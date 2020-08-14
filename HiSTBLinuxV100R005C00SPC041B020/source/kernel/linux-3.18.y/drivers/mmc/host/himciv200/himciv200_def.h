#ifndef _HI_MCI_REG_H_
#define _HI_MCI_REG_H_

#define MCI_CTRL                          0x00
#define MCI_PWREN                         0x04
#define MCI_CLKDIV                        0x08
#define MCI_CLKSRC                        0x0C
#define MCI_CLKENA                        0x10
#define MCI_TIMEOUT                       0x14
#define MCI_CTYPE                         0x18
#define MCI_BLKSIZ                        0x1c
#define MCI_BYTCNT                        0x20
#define MCI_INTMASK                       0x24
#define MCI_CMDARG                        0x28
#define MCI_CMD                           0x2C
#define MCI_RESP0                         0x30
#define MCI_RESP1                         0x34
#define MCI_RESP2                         0x38
#define MCI_RESP3                         0x3C
#define MCI_MINTSTS                       0x40
#define MCI_RINTSTS                       0x44
#define MCI_STATUS                        0x48
#define MCI_FIFOTH                        0x4C
#define MCI_CDETECT                       0x50
#define MCI_WRTPRT                        0x54
#define MCI_GPIO                          0x58
#define MCI_TCBCNT                        0x5C
#define MCI_TBBCNT                        0x60
#define MCI_DEBNCE                        0x64
#define MCI_USRID                         0x68
#define MCI_VERID                         0x6C
#define MCI_HCON                          0x70
#define MCI_UHS_REG                       0x74
#define MCI_RESET_N                       0x78
#define MCI_BMOD                          0x80
#define MCI_DBADDR                        0x88
#define MCI_IDSTS                         0x8C
#define MCI_IDINTEN                       0x90
#define MCI_DSCADDR                       0x94
#define MCI_BUFADDR                       0x98
#define MCI_CARDTHRCTL                    0x100
#define MCI_DDR_REG                       0x10c
#define MCI_ENABLE_SHIFT                  0x110

/* MCI_CTRL(0x00) */
#define CTRL_RESET                        BIT(0)
#define FIFO_RESET                        BIT(1)
#define DMA_RESET                         BIT(2)
#define INTR_EN                           BIT(4)
#define USE_INTERNAL_DMA                  BIT(25)

/*MCI_CLKENA(0X10) */
#define CCLK_ENABLE                       BIT(0)
#define CCLK_LOW_POWER                    BIT(16)

/* MCI_TIMEOUT(0x14) */
#define DATA_TIMEOUT                      (0xffffff<<8)
#define RESPONSE_TIMEOUT                  (0xff)

/* MCI_CTYPE(0x18) details */
#define CARD_WIDTH_1                      BIT(0)
#define CARD_WIDTH_0                      BIT(16)

/* MCI_INTMASK(0x24) */
#define ALL_INT_MASK                      (0x1ffff)
#define CD_INT_MASK                       BIT(2)
#define DTO_INT_MASK                      BIT(3)
#define VOLT_SWITCH_INT_MASK              BIT(10)
#define SDIO_INT_MASK                     BIT(16)

/* MCI_CMD REG(0X2C) */
#define CMD_START                         BIT(31)
#define CMD_USE_HOLD_REG                  BIT(29)
#define CMD_VOLT_SWITCH                   BIT(28)
#define CMD_CCS_EXP                       BIT(23)
#define CMD_CEATA_RD                      BIT(22)
#define CMD_UPD_CLK                       BIT(21)
#define CMD_INIT                          BIT(15)
#define CMD_STOP                          BIT(14)
#define CMD_PRV_DAT_WAIT                  BIT(13)
#define CMD_SEND_STOP                     BIT(12)
#define CMD_STRM_MODE                     BIT(11)
#define CMD_DAT_WR                        BIT(10)
#define CMD_DAT_EXP                       BIT(9)
#define CMD_RESP_CRC                      BIT(8)
#define CMD_RESP_LONG                     BIT(7)
#define CMD_RESP_EXP                      BIT(6)
#define CMD_INDX(n)                       ((n) & 0x1F)

/* MCI_INTSTS(0x44) details */
#define SDIO_INT_STATUS                   BIT(16)
#define EBE_INT_STATUS                    BIT(15)
#define ACD_INT_STATUS                    BIT(14)
#define SBE_INT_STATUS                    BIT(13)
#define HLE_INT_STATUS                    BIT(12)
#define FRUN_INT_STATUS                   BIT(11)
#define HTO_INT_STATUS                    BIT(10)
#define VOLT_SWITCH_INT_STATUS            BIT(10)
#define DRTO_INT_STATUS                   BIT(9)
#define RTO_INT_STATUS                    BIT(8)
#define DCRC_INT_STATUS                   BIT(7)
#define RCRC_INT_STATUS                   BIT(6)
#define RXDR_INT_STATUS                   BIT(5)
#define TXDR_INT_STATUS                   BIT(4)
#define DTO_INT_STATUS                    BIT(3)
#define CD_INT_STATUS                     BIT(2)
#define RE_INT_STATUS                     BIT(1)
#define ALL_INT_CLR                       (0x1efff)

/* MCI_STATUS(0x48) details */
#define DATA_BUSY                         BIT(9)

/* MCI_FIFOTH(0x4c) details */
#if defined(CONFIG_ARCH_HIFONE) || defined(CONFIG_ARCH_HI3798CV2X)
#define BURST_SIZE                        (0x4<<28)
#else
#if defined(CONFIG_ARCH_HI3798MV2X)
#define BURST_SIZE                        (0x3<<28)
#else
#define BURST_SIZE                        (0x2<<28)
#endif
#endif

#ifdef CONFIG_ARCH_GODBOX
#define FIFO_DEPTH                         16
#elif defined(CONFIG_ARCH_S40) || defined(CONFIG_ARCH_HI3798MX) || defined(CONFIG_ARCH_HIFONE) || defined(CONFIG_ARCH_HI3798CV2X) || defined(CONFIG_ARCH_HI3798MV2X)
#define FIFO_DEPTH                         256
#endif

#if defined(CONFIG_ARCH_HI3798MV2X)
#define RX_WMARK                           (0xF << 16)
#define TX_WMARK                           0x10
#else
#define RX_WMARK                           ((FIFO_DEPTH/2 - 1) << 16)
#define TX_WMARK                           ((FIFO_DEPTH/2) << 0)
#endif

/* MCI_CDETECT(0x50) */
#define HIMCI_CARD0                        BIT(0)

/* MCI_UHS_REG(0x74) */
#define ENABLE_UHS_VDD_180                 BIT(0)
#define ENABLE_UHS_DDR_MODE                BIT(16)

/* MCI_BMOD(0x80) */
#define BMOD_SWR                           BIT(0)
#define BURST_8                            (0x2<<8)
#define BURST_16                           (0x3<<8)
#define BURST_INCR                         BIT(1)
#define BMOD_DMA_EN                        BIT(7)

/* MCI_CARDTHRCTL(0x100) details */
#define READ_THRESHOLD_SIZE                (0x2000001)
#define RW_THRESHOLD_SIZE                  (0x2000005)

/* MCI_DDR(0x10C) details */
#define ENABLE_HS400_MODE                  BIT(31)

/* MCI_ENABLE_SHIFT(0x110) details */
#define ENABLE_SHIFT_01                    BIT(0)

/* IDMAC DEST0 details */
#define DMA_DES_OWN                        BIT(31)
#define DMA_DES_NEXT_DES                   BIT(4)
#define DMA_DES_FIRST_DES                  BIT(3)
#define DMA_DES_LAST_DES                   BIT(2)
#define DMA_DES_DIC                        BIT(1)

/* IDMAC DEST1 details */
#define DES_BUFFER_SIZE                    (4096)
#define MAX_DMA_DES                        (20480)

/* PERI_SD_LDO details */
#define SD_LDO_MASK                        (0x70)
#define SD_LDO_BYPASS                      BIT(6)
#define SD_LDO_ENABLE                      BIT(5)
#define SD_LDO_VOLTAGE                     BIT(4)
#endif
