#ifndef __MMC_REG_H__
#define __MMC_REG_H__

#define MMC_CTRL                    0x00
#define MMC_PWREN                   0x04
#define MMC_CLKDIV                  0x08
#define MMC_CLKSRC                  0x0C
#define MMC_CLKENA                  0x10
#define MMC_TMOUT                   0x14
#define MMC_CTYPE                   0x18
#define MMC_BLKSIZ                  0x1C
#define MMC_BYTCNT                  0x20
#define MMC_INTMASK                 0x24
#define MMC_CMDARG                  0x28
#define MMC_CMD                     0x2C
#define MMC_RESP0                   0x30
#define MMC_RESP1                   0x34
#define MMC_RESP2                   0x38
#define MMC_RESP3                   0x3C
#define MMC_MINTSTS                 0x40
#define MMC_RINTSTS                 0x44
#define MMC_STATUS                  0x48
#define MMC_FIFOTH                  0x4C
#define MMC_CDETECT                 0x50
#define MMC_WRTPRT                  0x54
#define MMC_GPIO                    0x58
#define MMC_TCBCNT                  0x5C
#define MMC_TBBCNT                  0x60
#define MMC_DEBNCE                  0x64
#define MMC_USRID                   0x68
#define MMC_VERID                   0x6C
#define MMC_HCON                    0x70
#define MMC_RSTN                    0x78
#define MMC_BMOD                    0x80
#define MMC_PLDMND                  0x84
#define MMC_DBADDR                  0x88
#define MMC_IDSTS                   0x8C
#define MMC_IDINTEN                 0x90
#define MMC_DSCADDR                 0x94
#define MMC_BUFADDR                 0x98
#define MMC_CARDTHRCTL              0x100
#define MMC_FIFO_START              0x200

// interrupt
#define MMC_INT_EBE                 (0x1<<15)
#define MMC_INT_HLE                 (0x1<<12)
#define MMC_INT_HTO                 (0x1<<10)
#define MMC_INT_BAR                 (0x1<<8)
#define MMC_INT_BDS                 (0x1<<9)
#define MMC_INT_DRTO                (0x1<<9)
#define MMC_INT_HLE                 (0x1<<12)
#define MMC_INT_RXDR                (0x1<<5)
#define MMC_INT_TXDR                (0x1<<4)
#define MMC_INT_DTO                 (0x1<<3)
#define MMC_INT_CD                  (0x1<<2)
#define MMC_INT_DATA_FIFO_ERROR     (0xBE80)
#define MMC_INT_BOOT_DATA_ERROR     (0xA200)

#define FIFO_RESET                  (1<<1)

// MCI CMD(0x2c) details 
#define START_CMD                   (1<<31)
#define USE_HOLD_REG                (1<<29)
#define SEND_INIT                   (1<<15)
#define WT_PD_CPT                   (1<<13)
#define DATA_EXPECT                 (1<<9)
#define CHECK_RESP_CRC              (1<<8)
#define RESP_LENGTH                 (1<<7)
#define RESP_EXPECT                 (1<<6)

/* bit 0: enable of card clk*/
#define CCLK_ENABLE                 (1<<0)

// MCI_STATUS(0x48) details 
#define DATA_BUSY                   (0x1<<9)

// MCI_TIMEOUT(0x14) details
/*bit 31-8: data read timeout param*/
#define DATA_TIMEOUT                (0xffffff<<8)
/* bit 7-0: response timeout param */
#define RESPONSE_TIMEOUT            (0xff)

// MMC_CARDTHRCTL(0x100) details 
#define THRESHOLD                   (0x02000001)

//  MCI_FIFOTH(0x4c) details
#define BURST_SIZE	(0x2<<28)
#define RX_WMARK	(0x7<<16)
#define TX_WMARK	0x8

//CMD
#define MMC_CMD_GO_IDLE_STATE        0
#define MMC_CMD_SEND_OP_COND         1
#define MMC_CMD_ALL_SEND_CID         2
#define MMC_CMD_SET_RELATIVE_ADDR    3
#define MMC_CMD_SWITCH               6
#define MMC_CMD_SELECT_CARD          7
#define MMC_CMD_STOP_TRANSMISSION    12
#define MMC_CMD_SET_BLOCKLEN         16
#define MMC_CMD_READ_MULTIPLE_BLOCK  18
#define SD_CMD_APP_SEND_OP_COND      41
#define MMC_CMD_APP_CMD              55
#define SD_CMD_SEND_IF_COND          8
#define SD_CMD_APP_SET_BUS_WIDTH     6


#define ALL_INT_CLR                  0x1affe

#define OCR_BUSY                     0x80000000
#define OCR_HCS                      0x40000000

#define CARD_WIDTH_1BIT              (0x00)
#define CARD_WIDTH_4BIT              (0x01)
#define CARD_WIDTH_8BIT              (0x10000)

/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_32_33                0x00100000
/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_33_34                0x00200000

#define EXT_CSD_BUS_WIDTH            183
#define EXT_CSD_HS_TIMING            185

#define EXT_CSD_BUS_WIDTH_1          0          /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4          1          /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8          2          /* Card is in 8 bit mode */

#define MMC_SWITCH_MODE_WRITE_BYTE   0x03       /* Set target byte to value */
#endif /* __MMC_REG_H__ */
