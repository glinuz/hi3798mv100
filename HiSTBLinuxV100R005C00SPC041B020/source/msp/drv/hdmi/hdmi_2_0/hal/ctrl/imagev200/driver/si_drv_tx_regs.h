//***************************************************************************
//! @file     si_drv_tx_regs.h
//! @brief    Silicon Image register definition header.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2014, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_DRV_TX_REGS_H__
#define __SI_DRV_TX_REGS_H__

#define BASE_ADDRESS 0x3000

typedef enum 
{
    Page_0 = (BASE_ADDRESS | 0x000),
    Page_1 = (BASE_ADDRESS | 0x100),
    Page_2 = (BASE_ADDRESS | 0x200),
    Page_3 = (BASE_ADDRESS | 0x300),
    Page_4 = (BASE_ADDRESS | 0x400),
    Page_5 = (BASE_ADDRESS | 0x500),
    Page_6 = (BASE_ADDRESS | 0x600),
    Page_7 = (BASE_ADDRESS | 0x700),
    Page_8 = (BASE_ADDRESS | 0x800),
    Page_9 = (BASE_ADDRESS | 0x900),
    Page_A = (BASE_ADDRESS | 0xa00),
    Page_F = (BASE_ADDRESS | 0xf00),
}Page_t;

//***************************************************************************
// page 30. Address: 40
// Vendor ID Low byte Register
#define REG_ADDR__VND_IDL                                                (Page_0 | 0x0000)
  // (ReadOnly, Bits 7:0) 
  // Vendor ID Low byte (01h)
  #define BIT_MSK__VND_IDL__REG_VHDL_IDL                                          0xFF

// Vendor ID High byte Register
#define REG_ADDR__VND_IDH                                                (Page_0 | 0x0001)
  // (ReadOnly, Bits 7:0) 
  // Vendor ID High byte (00h)
  #define BIT_MSK__VND_IDH__REG_VHDL_IDH                                          0xFF

// Device ID Low byte Register
#define REG_ADDR__DEV_IDL                                                (Page_0 | 0x0002)
  // (ReadOnly, Bits 7:0) 
  // Device ID Low byte
  #define BIT_MSK__DEV_IDL__REG_DEV_IDL                                           0xFF

// Device ID High byte Register
#define REG_ADDR__DEV_IDH                                                (Page_0 | 0x0003)
  // (ReadOnly, Bits 7:0) 
  // Device ID High
  #define BIT_MSK__DEV_IDH__REG_DEV_IDH                                           0xFF

// Device Revision Register Low Byte
#define REG_ADDR__DEV_REVL                                               (Page_0 | 0x0004)
  // (ReadOnly, Bits 7:0) 
  // Device Revision Low byte
  #define BIT_MSK__DEV_REVL__DEV_REV_IDL                                           0xFF

// Device Revision Register High Byte 
#define REG_ADDR__DEV_REVH                                               (Page_0 | 0x0005)
  // (ReadOnly, Bits 7:0) 
  // Device Revision High byte
  #define BIT_MSK__DEV_REVH__DEV_REV_IDH                                           0xFF

// SOC INTR Register
#define REG_ADDR__SOC_INTR                                               (Page_0 | 0x0006)
  // (ReadWrite, Bits 0) 
  // PRIF timeout
  #define BIT_MSK__SOC_INTR__REG_SOC_INTR0                                         0x01

// SOC INTR Mask Register
#define REG_ADDR__SOC_INTRMASK                                           (Page_0 | 0x0007)
  // (ReadWrite, Bits 0) 
  // mask bit for SOC_INTR[0]
  #define BIT_MSK__SOC_INTRMASK__REG_SOC_INTRMASK0                                     0x01

// Host Control1 Register
#define REG_ADDR__HOST_CTRL1                                             (Page_0 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Soft reset to IP registers other than SOC. Active high.
  #define BIT_MSK__HOST_CTRL1__REG_NONSOC_REG_RST                                    0x01

// Host Control2 Register
#define REG_ADDR__HOST_CTRL2                                             (Page_0 | 0x0009)
  // (ReadOnly, Bits 0) 
  // Interrupt status for cypress_top
  #define BIT_MSK__HOST_CTRL2__REG_INTR_STAT0                                        0x01
  // (ReadOnly, Bits 1) 
  // interrupt status for PRIF monitor
  #define BIT_MSK__HOST_CTRL2__REG_INTR_STAT1                                        0x02
  // (ReadWrite, Bits 7) 
  // Interrupt polarity. 1 (default): inverted or INT active low; 0: original or INT active high.
  #define BIT_MSK__HOST_CTRL2__REG_INTR_POL                                          0x80

// OTP DBYTE510 Register
#define REG_ADDR__OTP_DBYTE510                                           (Page_0 | 0x000A)
  // (ReadOnly, Bits 7:0) 
  // OTP data byte at offset 510
  #define BIT_MSK__OTP_DBYTE510__REG_OTP_DBYTE510                                      0xFF

// Function Select Register
#define REG_ADDR__FUNC_SEL                                               (Page_0 | 0x000B)
  // (ReadWrite, Bits 0) 
  // Enable HDMI for Tx
  #define BIT_MSK__FUNC_SEL__REG_HDMI_EN                                           0x01
  // (ReadWrite, Bits 1) 
  // Enable MHL3 for Tx
  #define BIT_MSK__FUNC_SEL__REG_MHL3_EN                                           0x02

// System Control #1 Register
#define REG_ADDR__SYS_CTRL1                                              (Page_0 | 0x000C)
  // (ReadWrite, Bits 0) 
  // SW OTP HDCP 1.4 and 2.x Audio Mute Override set bit
  #define BIT_MSK__SYS_CTRL1__REG_OTPAMUTEOVR_SET                                   0x01
  // (ReadWrite, Bits 1) 
  // SW OTP HDCP 1.4 and 2.x  Audio Drop Override set bit
  #define BIT_MSK__SYS_CTRL1__REG_OTPADROPOVR_SET                                   0x02
  // (ReadWrite, Bits 2) 
  // SW OTP HDCP 1.4 and 2.x Video Mute Override set bit
  #define BIT_MSK__SYS_CTRL1__REG_OTPVMUTEOVR_SET                                   0x04
  // (ReadWrite, Bits 4) 
  // Enable bit to let SW override OTP HDCP2.x Audio control. Override value is reg_otpamuteovr_set and reg_otpadropovr_set. These 2 bits can not be both 1.
  #define BIT_MSK__SYS_CTRL1__REG_OTP2XAOVR_EN                                      0x10
  // (ReadWrite, Bits 5) 
  // Enable bit to let SW override OTP HDCP2.x Video control. Override value is reg_otpvmuteovr_set.
  #define BIT_MSK__SYS_CTRL1__REG_OTP2XVOVR_EN                                      0x20
  // (ReadWrite, Bits 6) 
  // Enable bit to let SW override OTP HDCP1.4 Audio control. Override value is reg_otpamuteovr_set and reg_otpadropovr_set. These 2 bits can not be both 1.
  #define BIT_MSK__SYS_CTRL1__REG_OTP14AOVR_EN                                      0x40
  // (ReadWrite, Bits 7) 
  // Enable bit to let SW override OTP HDCP1.4 Video control. Override value is reg_otpvmuteovr_set.
  #define BIT_MSK__SYS_CTRL1__REG_OTP14VOVR_EN                                      0x80

// System Clock PWD Register
#define REG_ADDR__CLKPWD                                                 (Page_0 | 0x000D)
  // (ReadWrite, Bits 0) 
  // Gate off DIPT clock. Active low. Default 0. 
  #define BIT_MSK__CLKPWD__REG_PD_DIPT_CLK_N                                     0x01
  // (ReadWrite, Bits 1) 
  // Gate off mhl1/2/3 clock. Active low. Default 0.
  #define BIT_MSK__CLKPWD__REG_PD_MHL_CLK_N                                      0x02
  // (ReadWrite, Bits 2) 
  // Gate off pclk. Active low. Default 0.
  #define BIT_MSK__CLKPWD__REG_PDIDCK_N                                          0x04

// Clock Phase Control1 Register
#define REG_ADDR__CLKPHASE1                                              (Page_0 | 0x000E)
  // (ReadWrite, Bits 0) 
  // register bit to control the phase of mif_clk 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE1__REG_MIF_CLK_PHASE                                     0x01
  // (ReadWrite, Bits 1) 
  // Selects hsic_rx_strobe phase.   0 - Original phase; the same as it comes out of HSIC Strobe pin (default) 1 - Invert hsic_rx_strobe: change its phase 180 degrees 
  #define BIT_MSK__CLKPHASE1__REG_HSIC_RX_STROBE_PHASE                              0x02
  // (ReadWrite, Bits 2) 
  // register bit to control the phase of fc_clk 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE1__REG_FC_CLK_PHASE                                      0x04
  // (ReadWrite, Bits 3) 
  // register bit to control the phase of keeper_clk 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE1__REG_KEEPER_CLK_PHASE                                  0x08
  // (ReadWrite, Bits 4) 
  // register bit to control the phase of iclk 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE1__REG_IDCK_PHASE                                        0x10
  // (ReadWrite, Bits 5) 
  // Selects cts_tck phase.   0 - Original phase; the same as it comes out of PLL (default) 1 - Invert cts_tck: change its phase 180 degrees 
  #define BIT_MSK__CLKPHASE1__REG_CTS_TCK_PHASE                                     0x20
  // (ReadWrite, Bits 6) 
  // Selects hsic_clk phase.   0 - Original phase; the same as it comes out of PLL (default) 1 - Invert hsic_clk: change its phase 180 degrees 
  #define BIT_MSK__CLKPHASE1__REG_HSIC_CLK_PHASE                                    0x40
  // (ReadWrite, Bits 7) 
  // Selects tdm_lclk phase.   0 - Original phase; the same as it comes out of PLL (default) 1 - Invert tdm_lclk: change its phase 180 degrees
  #define BIT_MSK__CLKPHASE1__REG_TDM_LCLK_PHASE                                    0x80

// Clock Phase Control2 Register
#define REG_ADDR__CLKPHASE2                                              (Page_0 | 0x000F)
  // (ReadWrite, Bits 0) 
  // register bit to control the phase of PCLK 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE2__REG_PCLK_PHASE                                        0x01
  // (ReadWrite, Bits 1) 
  // register bit to control the phase of PCLKNX 0: no phase change (default) 1: inverted 
  #define BIT_MSK__CLKPHASE2__REG_PCLKNX_PHASE                                      0x02

// Software Reset Register
#define REG_ADDR__PWD_SRST                                               (Page_0 | 0x0010)
  // (ReadWrite, Bits 0) 
  // Software Reset. Reset all internal logic; except register interface; hdcp; and EEPROM interface.  Note: asserting Software Reset will not reset writable register contents: 1 - Reset 0 - Normal operation (default).
  #define BIT_MSK__PWD_SRST__REG_SW_RST                                            0x01

// SoC SW Reset2 Register
#define REG_ADDR__SW_RST2                                                (Page_0 | 0x0011)
  // (ReadWrite, Bits 3) 
  // Select MHL3 CTS operation mode 1 - 3 lanes (default) 0 - 2 lanes
  #define BIT_MSK__SW_RST2__REG_MHL3CTS_3LANE                                     0x08

// SW Reset #3 Register
#define REG_ADDR__SW_RST3                                                (Page_0 | 0x0012)
  // (ReadWrite, Bits 0) 
  // Reset MHL FIFO when there is overflow or underflow happens 0 - Disable 1 - Enable (default)
  #define BIT_MSK__SW_RST3__REG_MHL_FIFO_AUTO_RST_EN                              0x01
  // (ReadWrite, Bits 1) 
  // Reset timing closure FIFO when there is overflow or underflow happens 0 - Disable (default) 1 - Enable
  #define BIT_MSK__SW_RST3__REG_TCF_AUTO_RST_EN                                   0x02
  // (ReadWrite, Bits 2) 
  // Reset MHL3 CTS FIFO when there is overflow or underflow happens 0 - Disable 1 - Enable (default)
  #define BIT_MSK__SW_RST3__REG_CTS_FIFO_AUTO_RST_EN                              0x04
  // (ReadWrite, Bits 7) 
  // Soft reset for CEC only, active high.
  #define BIT_MSK__SW_RST3__REG_CEC_RST                                           0x80

// SoC MISC Register
#define REG_ADDR__SYS_MISC                                               (Page_0 | 0x0013)
  // (ReadWrite, Bits 0) 
  // Edge select for SPI MOSI. 1'b0: MOSI is latched by the rising edge of spi_sclk 1'b1: MOSI is latched by the falling edge of spi_sclk
  #define BIT_MSK__SYS_MISC__REG_SPI_MOSI_EDGE                                     0x01
  // (ReadWrite, Bits 1) 
  // Edge select for SPI MISO.  1'b0: MISO is launched by the falling edge of spi_sclk 1'b1: MISO is launched by the rising edge of spi_sclk
  #define BIT_MSK__SYS_MISC__REG_SPI_MISO_EDGE                                     0x02
  // (ReadWrite, Bits 7:2) 
  // bit[0]: bypass tclknx bit[1]: bypass hsic_clk bit[2]:  bypass mif_clk  bit[3]:  bypass mhl_clk; bit[4]:  bypass fb_clk;  bit[5]:  bypass ref_clk; 
  #define BIT_MSK__SYS_MISC__REG_BYPASS_PLL_CLK                                    0xFC

// SoC MISC2 Register
#define REG_ADDR__SYS_MISC2                                              (Page_0 | 0x0014)
  // (ReadWrite, Bits 1:0) 
  // OCLK Divide 00: by 2 (default); 01; by 4; 10: by 8; 11: by 16
  #define BIT_MSK__SYS_MISC2__REG_OCLKDIV                                           0x03
  // (ReadWrite, Bits 2) 
  // Bypass OCLK and cbus_disc_clk; use GPIO[3] for cbus_disc_clk; use RPWR for oclk
  #define BIT_MSK__SYS_MISC2__REG_BYP_OCLKRING_OPTION                               0x04
  // (ReadWrite, Bits 3) 
  // Choose osc_clk_dvd_1024 if set 1. Choose osc_clk_dvd_2 if set 0.
  #define BIT_MSK__SYS_MISC2__REG_60K_SEL                                           0x08
  // (ReadWrite, Bits 5) 
  // Enable I2C communicate with EMSC instead of SPI
  #define BIT_MSK__SYS_MISC2__REG_I2C_TO_EMSC_EN                                    0x20

// SoC MISC3 Register
#define REG_ADDR__SYS_MISC3                                              (Page_0 | 0x0015)
  // (ReadOnly, Bits 0) 
  // OTP status for HDCP Video Mute option. 1 indicates when HDCP encryption off video will be blank. 0 indicates when HDCP encryption off video is sent as-is.
  #define BIT_MSK__SYS_MISC3__REG_OTP_HDCPVMUTE                                     0x01
  // (ReadOnly, Bits 2:1) 
  // OTP status for HDCP Audio Mute option.  00 indicates when HDCP encryption off; audio is sent as-is. 01 indicates when HDCP encryption off; audio will be blank.  10 indicates when HDCP encryption off; audio related packets or InfoFrames will be dropped.  11 reserved
  #define BIT_MSK__SYS_MISC3__REG_OTP_HDCPADRPMUT                                   0x06
  // (ReadWrite, Bits 3) 
  // OTP_PS bit
  #define BIT_MSK__SYS_MISC3__REG_OTP_PS                                            0x08
  // (ReadWrite, Bits 4) 
  // OTP_PD bit
  #define BIT_MSK__SYS_MISC3__REG_OTP_PD                                            0x10

// PRIF Control Register
#define REG_ADDR__PRIFCNTL                                               (Page_0 | 0x0016)
  // (ReadWrite, Bits 1:0) 
  // This register defines PRIF timeout criteria. If par_rdy stuck low for too long; an error will be generated and may be reflected by INT. 00: 10ms (default); 01: 20ms; 10: 40ms; 11: 80ms
  #define BIT_MSK__PRIFCNTL__REG_PRIF_TOUT_SEL                                     0x03
  // (ReadWrite, Bits 6) 
  // This register can enable PRIF timeout for simulation mode. PRIF timeout is allowed in 160us.  0: disable (default) 1: enable
  #define BIT_MSK__PRIFCNTL__REG_PRIF_TOUT_SMODE                                   0x40
  // (ReadWrite, Bits 7) 
  // This register enables or disables PRIF timeout. 0: disable (default) 1: enable
  #define BIT_MSK__PRIFCNTL__REG_PRIF_TOUT_EN                                      0x80

// I2C Address for Page 1 Register
#define REG_ADDR__PAGE_1_ADDR                                            (Page_0 | 0x0017)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 1
  #define BIT_MSK__PAGE_1_ADDR__REG_PAGE_1_ADDR_B7_B0                                 0xFF

// I2C Address for Page 2 Register
#define REG_ADDR__PAGE_2_ADDR                                            (Page_0 | 0x0018)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 2
  #define BIT_MSK__PAGE_2_ADDR__REG_PAGE_2_ADDR_B7_B0                                 0xFF

// I2C Address for Page 3 Register
#define REG_ADDR__Page__ADDR                                            (Page_0 | 0x0019)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 3
  #define BIT_MSK__Page__ADDR__REG_Page__ADDR_B7_B0                                 0xFF

// I2C Address for Page MHLSpec Register
#define REG_ADDR__PAGE_MHLSPEC_ADDR                                      (Page_0 | 0x001A)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page MHLSPEC
  #define BIT_MSK__PAGE_MHLSPEC_ADDR__REG_PAGE_MHLSPEC_ADDR_B7_B0                           0xFF

// CBUS Address Register
#define REG_ADDR__PAGE_CBUS_ADDR                                         (Page_0 | 0x001B)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page CBUS
  #define BIT_MSK__PAGE_CBUS_ADDR__REG_PAGE_CBUS_ADDR_B7_B0                              0xFF

// I2C Address for HW TPI Register
#define REG_ADDR__HW_TPI_ADDR                                            (Page_0 | 0x001C)
  // (ReadWrite, Bits 7:0) 
  // I2C address for HW TPI
  #define BIT_MSK__HW_TPI_ADDR__REG_HW_TPI_ADDR_B7_B0                                 0xFF

// I2C Address for Page 7 Register
#define REG_ADDR__PAGE_7_ADDR                                            (Page_0 | 0x001D)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 7
  #define BIT_MSK__PAGE_7_ADDR__REG_PAGE_7_ADDR_B7_B0                                 0xFF

// I2C Address for Page 8 Register
#define REG_ADDR__PAGE_8_ADDR                                            (Page_0 | 0x001E)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 8
  #define BIT_MSK__PAGE_8_ADDR__REG_PAGE_8_ADDR_B7_B0                                 0xFF

// I2C Address for Page 9 Register
#define REG_ADDR__PAGE_9_ADDR                                            (Page_0 | 0x001F)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 9
  #define BIT_MSK__PAGE_9_ADDR__REG_PAGE_9_ADDR_B7_B0                                 0xFF

// I2C Address for Page 10 Register
#define REG_ADDR__PAGE_10_ADDR                                           (Page_0 | 0x0020)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 10
  #define BIT_MSK__PAGE_10_ADDR__REG_PAGE_10_ADDR_B7_B0                                0xFF

// I2C Address for Page 11 Register
#define REG_ADDR__PAGE_11_ADDR                                           (Page_0 | 0x0021)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 11
  #define BIT_MSK__PAGE_11_ADDR__REG_PAGE_11_ADDR_B7_B0                                0xFF

// I2C Address for Page 12 Register
#define REG_ADDR__PAGE_12_ADDR                                           (Page_0 | 0x0022)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 12
  #define BIT_MSK__PAGE_12_ADDR__REG_PAGE_12_ADDR_B7_B0                                0xFF

// I2C Address for Page 13 Register
#define REG_ADDR__PAGE_13_ADDR                                           (Page_0 | 0x0023)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 13
  #define BIT_MSK__PAGE_13_ADDR__REG_PAGE_13_ADDR_B7_B0                                0xFF

// I2C Address for Page 14 Register
#define REG_ADDR__PAGE_14_ADDR                                           (Page_0 | 0x0024)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 14
  #define BIT_MSK__PAGE_14_ADDR__REG_PAGE_14_ADDR_B7_B0                                0xFF

// I2C Address for Page 15 Register
#define REG_ADDR__PAGE_15_ADDR                                           (Page_0 | 0x0025)
  // (ReadWrite, Bits 7:0) 
  // I2C address for Page 15
  #define BIT_MSK__PAGE_15_ADDR__REG_PAGE_15_ADDR_B7_B0                                0xFF

// IP RegRST Register 0
#define REG_ADDR__IPREGRST0                                              (Page_0 | 0x0026)
  // (ReadWrite, Bits 7:0) 
  // IP specific reset for register and PRIF decoding bit 0: AIP reset bit 1: AVLINK reset bit 2: CBUS_DISC reset bit 3: CBUS reset bit 4: reserved bit 5: COC_DOC reset bit 6: DPLL reset bit 7: eMSC reset
  #define BIT_MSK__IPREGRST0__REG_IPREGRST0                                         0xFF

// IP RegRST Register 1
#define REG_ADDR__IPREGRST1                                              (Page_0 | 0x0027)
  // (ReadWrite, Bits 7:0) 
  // IP specific reset for register and PRIF decoding bit 0: Harry reset bit 1: HDCP2 reset bit 2: HDMI2 reset bit 3: HDMI2MHL1 reset bit 4: HDMI2MHL3 reset bit 5: HSIC_BIST reset bit 6: HSIC_PHY reset bit 7: DS_EDID reset
  #define BIT_MSK__IPREGRST1__REG_IPREGRST1                                         0xFF

// IP RegRST Register 2
#define REG_ADDR__IPREGRST2                                              (Page_0 | 0x0028)
  // (ReadWrite, Bits 7:0) 
  // IP specific reset for register and PRIF decoding bit 0: MHL_TX_TOP reset bit 1: P0 reset bit 2: PTPI reset bit 3:  TOP reset bit 4: TDMCORE reset bit 5: USBTCORE reset bit 6: MCU reset
  #define BIT_MSK__IPREGRST2__REG_IPREGRST2                                         0xFF

// IP RegRST Register 3
#define REG_ADDR__IPREGRST3                                              (Page_0 | 0x0029)
  // (ReadWrite, Bits 7:0) 
  // IP specific reset for register and PRIF decoding
  #define BIT_MSK__IPREGRST3__REG_IPREGRST3                                         0xFF

// Top Interrupt Register
#define REG_ADDR__TOP_INTR                                               (Page_0 | 0x002C)
  // (ReadOnly, Bits 0) 
  // Interrupt status at top level
  #define BIT_MSK__TOP_INTR__REG_INTR_TOP                                          0x01
  // (ReadOnly, Bits 1) 
  // Interrupt status for cocdoc wrapper IP
  #define BIT_MSK__TOP_INTR__REG_COCDOC_INTR                                       0x02

// IP MISC Register
#define REG_ADDR__DEBUG_MODE_EN                                          (Page_0 | 0x0030)
  // (ReadWrite, Bits 0) 
  // Simulation mode. When enabled; time constant used is shortened. 0: disable (default) 1: enable
  #define BIT_MSK__DEBUG_MODE_EN__REG_SIMODE                                            0x01
  // (ReadWrite, Bits 1) 
  // Debug Mode 3 Enable 0: Disable 1: vsync; p_stable; HPD (facing upstream); RSEN; hdcp_ri_rdy; sup_an_stop; hdcp_bksv_err; and hdcp_enc_on are available from registers. 
  #define BIT_MSK__DEBUG_MODE_EN__REG_DBGMODE3_EN                                       0x02
  // (ReadWrite, Bits 2) 
  // Enabled to block DDC access when HPD is low
  #define BIT_MSK__DEBUG_MODE_EN__REG_BLOCK_DDC_BY_HPD                                  0x04
  // (ReadOnly, Bits 3) 
  // The current status of the VSYNC input pin. Monitor this bit with I2C to recognize the arrival of VSYNC pulses.
  #define BIT_MSK__DEBUG_MODE_EN__VSYNCPIN                                              0x08

// System Status Register
#define REG_ADDR__SYS_STAT                                               (Page_0 | 0x0031)
  // (ReadOnly, Bits 0) 
  // IDCK to TMDS clock stable.  The state of this internal signal can be read from this bit.  Upon changes to the IDCK and/or TMDS_CTRL register's tclk_sel bits; this bit can go low.  After a low to high transition; a software reset is recommended (unless auto-reset is enabled).  Note: This bit was previously called MDI in earlier Silicon Image transmitters.
  #define BIT_MSK__SYS_STAT__P_STABLE                                              0x01
  // (ReadOnly, Bits 1) 
  // Upstream Hot Plug Detect status. This is the status manipulated by FW based on CBUS HPD and need 
  #define BIT_MSK__SYS_STAT__HPDPIN                                                0x02
  // (ReadOnly, Bits 2) 
  // This bit is HIGH if a powered on receiver is connected to the transmitter outputs; LOW otherwise. This function is only available for use in DC-coupled systems.  Rsen is active even if SYS_CTRL1's PD bit is 0 (in other words; rsen is available during power down).
  #define BIT_MSK__SYS_STAT__RSEN                                                  0x04
  // (ReadWrite, Bits 3) 
  // Set this bit will force IP prif_ready becomes 1 after certain timeout, for register byte located in tclk domain while tclk is not provided by the system.
  #define BIT_MSK__SYS_STAT__REG_FORCERDY_EN                                       0x08
  // (ReadWrite, Bits 5:4) 
  // This register defines a timer. After timer expiration IP prif_ready will be forced high for registers located in tclk domain. 2'b00: 8k cycles; 2'b01: 16k; 2'b10: 21k; 2'b11: 64k 
  #define BIT_MSK__SYS_STAT__REG_FORCERDY_SEL                                      0x30
  // (ReadOnly, Bits 7) 
  // OTP status for ID8620/8660 option. 1 indicates 8620 (CoC only) and 0 indicates 8660(DoC or CoC).
  #define BIT_MSK__SYS_STAT__REG_OTP_ID8620                                        0x80

// System Control #3 Register
#define REG_ADDR__SYS_CTRL3                                              (Page_0 | 0x0032)
  // (ReadWrite, Bits 0) 
  // When set to 1'b1; 0x61A[0] will decide whether output HDMI or DVI
  #define BIT_MSK__SYS_CTRL3__REG_TX_CONTROL_HDMI                                   0x01
  // (ReadWrite, Bits 2:1) 
  // General Purpose Control. These CTL bit states are transmitted across the TMDS link during blanking times only. DVI 1.0 mode only.A754
  #define BIT_MSK__SYS_CTRL3__REG_CTL                                               0x06
  // (ReadWrite, Bits 5) 
  // 1: system wait counter time of 5000ms(needed for HDCP) and 2000ms(needed for CBUS) will start 0: counter will not start
  #define BIT_MSK__SYS_CTRL3__REG_SYS_CNTR                                          0x20

// System Control DPD Register
#define REG_ADDR__DPD                                                    (Page_0 | 0x0033)
  // (ReadWrite, Bits 3) 
  // Power on HSIC
  #define BIT_MSK__DPD__REG_PWRON_HSIC                                        0x08
  // (ReadWrite, Bits 4) 
  // Power Down internal oscillator; effectively disabling Master I2C; Slave I2C; Charge Pump; and preventing uploading of new vectors from EPROM 0 - Power down.   1 - normal operation (default).
  #define BIT_MSK__DPD__REG_OSC_EN                                            0x10
  // (ReadWrite, Bits 5) 
  // Power on ETMDS
  #define BIT_MSK__DPD__REG_PWRON_ETMDS                                       0x20
  // (ReadWrite, Bits 6) 
  // Power on MHL Tx data path
  #define BIT_MSK__DPD__REG_PWRON_DP                                          0x40
  // (ReadWrite, Bits 7) 
  // Power on TX PLL
  #define BIT_MSK__DPD__REG_PWRON_PLL                                         0x80

// System Control #4 Register
#define REG_ADDR__SYS_CTRL4                                              (Page_0 | 0x0034)
  // (ReadWrite, Bits 0) 
  // HDMI TransCode Mode enable bit. When set 1; HDMI stream will bypass internal process and  be sent to MHL TX directly. 0 - disable (default) 1 - enable
  #define BIT_MSK__SYS_CTRL4__REG_TRANSCODE                                         0x01
  // (ReadWrite, Bits 1) 
  // 
  #define BIT_MSK__SYS_CTRL4__REG_HSIC_TX_BIST_START_SEL                            0x02
  // (ReadWrite, Bits 2) 
  // This bit can select between Local I2C (including internal DDC master) and DDC port.  0 - Use Local I2C as a source port (default) 1 - Use DDC port as a source port
  #define BIT_MSK__SYS_CTRL4__REG_EXT_DDC_SEL                                       0x04
  // (ReadOnly, Bits 4) 
  // MHL1/2 FIFO Overflow/Underflow status
  #define BIT_MSK__SYS_CTRL4__REG_MHL_FIFO_OF_UF_FLAG                               0x10
  // (ReadOnly, Bits 5) 
  // TCF FIFO Overflow/Underflow status
  #define BIT_MSK__SYS_CTRL4__REG_TCF_OF_UF_FLAG                                    0x20
  // (ReadOnly, Bits 6) 
  // CTS FIFO Overflow/Underflow status
  #define BIT_MSK__SYS_CTRL4__REG_CTS_FIFO_OF_UF_FLAG                               0x40

// MCU Timeout Register
#define REG_ADDR__MCUTOUT                                                (Page_0 | 0x0035)
  // (ReadWrite, Bits 7:0) 
  // This register defines in how many OCLK cycles MCU timeout will occur if there is no response for the request. The granularity is 256.
  #define BIT_MSK__MCUTOUT__REG_MCU_TIMEOUT                                       0xFF

// HDCP Control Register
#define REG_ADDR__HDCP_CTRL                                              (Page_0 | 0x003F)
  // (ReadWrite, Bits 0) 
  // Encryption Enable.                                               0 - Encryption disabled (default) 1 - Encryption enabled     Note: This bit can now be written to a 0.  If a 1 to 0 transition occurs; appropriate security measures are taken.
  #define BIT_MSK__HDCP_CTRL__REG_ENC_EN                                            0x01
  // (ReadOnly, Bits 1) 
  // Ri Ready.  This bit indicates that the first Ri value is available.  This bit is cleared by a hardware reset; cp_reset (reg. 0x009[2]) or by writing the last byte of the BKSV. It is set when finish loading the last byte of the KEYS.
  #define BIT_MSK__HDCP_CTRL__HDCP_RI_RDY                                           0x02
  // (ReadWrite, Bits 2) 
  // This is the reset bit for the cipher engine.  This bit is asserted by hardware reset or software.  It can be de-asserted (= 1) at any time.
  #define BIT_MSK__HDCP_CTRL__REG_CP_RESETN                                         0x04
  // (ReadWrite, Bits 3) 
  // When cleared; the cipher engine is allowed to free-run; and the WR_AN registers will cycle through a sequence of pseudo-random values.  This bit must be set for normal cipher operation.  When it is; the value read from the WR_AN registers may be used to initialize the AN register in the receiver.                                                This bit is automatically cleared under any of the following conditions:           - Software write 0 into it  - Hardware reset   - BKSV_ERR     - RX_RPTR is changed                                                                                     Note that either external hardware or software should detect when a device is not plugged in; and assert (= 1) this bit in such cases.
  #define BIT_MSK__HDCP_CTRL__REG_AN_STOP                                           0x08
  // (ReadWrite, Bits 4) 
  // Set this bit prior to authentication if the receiver is a repeater.
  #define BIT_MSK__HDCP_CTRL__REG_RX_RPTR                                           0x10
  // (ReadOnly, Bits 5) 
  // BKSV Error.    This bit is cleared (set to 0) when: 1. writing the last byte of the BKSV . 2. Hardware reset through the RESET pin  3. By setting cp_reset bit (reg. 0x00F[2])  It is set when load of the keys is done and BKSV did not had 20 ones.
  #define BIT_MSK__HDCP_CTRL__HDCP_BKSV_ERR                                         0x20
  // (ReadOnly, Bits 6) 
  // This bit is set to 1 whenever the encryption is enabled and encryption is in process.
  #define BIT_MSK__HDCP_CTRL__HDCP_ENC_ON                                           0x40

// Write BKSV1 Register
#define REG_ADDR__WR_BKSV_1                                              (Page_0 | 0x0040)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #1 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv1 register located inside HDCP block.  During read get Data of BKSV #1 register. 
  #define BIT_MSK__WR_BKSV_1__REG_BKSV0                                             0xFF

// Write BKSV2 Register
#define REG_ADDR__WR_BKSV_2                                              (Page_0 | 0x0041)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #2 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv2 register located inside HDCP block.  During read get Data of BKSV #2 register. 
  #define BIT_MSK__WR_BKSV_2__REG_BKSV1                                             0xFF

// Write BKSV3 Register
#define REG_ADDR__WR_BKSV_3                                              (Page_0 | 0x0042)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #3 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv3 register located inside HDCP block.  During read get Data of BKSV #3 register. 
  #define BIT_MSK__WR_BKSV_3__REG_BKSV2                                             0xFF

// Write BKSV4 Register
#define REG_ADDR__WR_BKSV_4                                              (Page_0 | 0x0043)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #4 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv4 register located inside HDCP block.  During read get Data of BKSV #4 register. 
  #define BIT_MSK__WR_BKSV_4__REG_BKSV3                                             0xFF

// Write BKSV5 Register
#define REG_ADDR__WR_BKSV_5                                              (Page_0 | 0x0044)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #5 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv5 register located inside HDCP block. In addition if HDCP is enabled then by writing into this register the authentication is triggered.  During read get Data of BKSV #5 register. 
  #define BIT_MSK__WR_BKSV_5__REG_BKSV4                                             0xFF

// HDCP AN_1 Register
#define REG_ADDR__AN1                                                    (Page_0 | 0x0045)
  // (ReadWrite, Bits 7:0) 
  // Byte #1 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN1__HDCP_AN0                                              0xFF

// HDCP AN_2 Register
#define REG_ADDR__AN2                                                    (Page_0 | 0x0046)
  // (ReadWrite, Bits 7:0) 
  // Byte #2 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN2__HDCP_AN1                                              0xFF

// HDCP AN_3 Register
#define REG_ADDR__AN3                                                    (Page_0 | 0x0047)
  // (ReadWrite, Bits 7:0) 
  // Byte #3 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN3__HDCP_AN2                                              0xFF

// HDCP AN_4 Register
#define REG_ADDR__AN4                                                    (Page_0 | 0x0048)
  // (ReadWrite, Bits 7:0) 
  // Byte #4 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN4__HDCP_AN3                                              0xFF

// HDCP AN_5 Register
#define REG_ADDR__AN5                                                    (Page_0 | 0x0049)
  // (ReadWrite, Bits 7:0) 
  // Byte #5 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN5__HDCP_AN4                                              0xFF

// HDCP AN_6 Register
#define REG_ADDR__AN6                                                    (Page_0 | 0x004A)
  // (ReadWrite, Bits 7:0) 
  // Byte #6 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN6__HDCP_AN5                                              0xFF

// HDCP AN_7 Register
#define REG_ADDR__AN7                                                    (Page_0 | 0x004B)
  // (ReadWrite, Bits 7:0) 
  // Byte #7 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process.  Also can write into this register.
  #define BIT_MSK__AN7__HDCP_AN6                                              0xFF

// HDCP AN_8 Register
#define REG_ADDR__AN8                                                    (Page_0 | 0x004C)
  // (ReadWrite, Bits 7:0) 
  // Byte #8 of a 64-bit pseudo-random value. May be read from this register and used in the authentication process. Also can write into this register
  #define BIT_MSK__AN8__HDCP_AN7                                              0xFF

// AKSV_1 Register
#define REG_ADDR__AKSV_1                                                 (Page_0 | 0x004D)
  // (ReadOnly, Bits 7:0) 
  // Byte #1 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__AKSV_1__REG_AKSV0                                             0xFF

// AKSV_2 Register
#define REG_ADDR__AKSV_2                                                 (Page_0 | 0x004E)
  // (ReadOnly, Bits 7:0) 
  // Byte #2 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__AKSV_2__REG_AKSV1                                             0xFF

// AKSV_3 Register
#define REG_ADDR__AKSV_3                                                 (Page_0 | 0x004F)
  // (ReadOnly, Bits 7:0) 
  // Byte #3 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__AKSV_3__REG_AKSV2                                             0xFF

// AKSV_4 Register
#define REG_ADDR__AKSV_4                                                 (Page_0 | 0x0050)
  // (ReadOnly, Bits 7:0) 
  // Byte #4 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__AKSV_4__REG_AKSV3                                             0xFF

// AKSV_5 Register
#define REG_ADDR__AKSV_5                                                 (Page_0 | 0x0051)
  // (ReadOnly, Bits 7:0) 
  // Byte #5 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__AKSV_5__REG_AKSV4                                             0xFF

// Ri_1 Register
#define REG_ADDR__RI_1                                                   (Page_0 | 0x0052)
  // (ReadOnly, Bits 7:0) 
  // Ri value.  The values of these registers should be read and compared against the Ri value of the receiver to ensure that the encryption process on the transmitter and receiver are synchronized.  with vp_sel = 1 (page 0 0xEC bit 6); the status is from HW TPI; with vp_sel = 0; the status from normal path.
  #define BIT_MSK__RI_1__REG_RI_PRIME0                                         0xFF

// Ri_2 Register
#define REG_ADDR__RI_2                                                   (Page_0 | 0x0053)
  // (ReadOnly, Bits 7:0) 
  // Ri value.  The values of these registers should be read and compared against the Ri value of the receiver to ensure that the encryption process on the transmitter and receiver are synchronized. with vp_sel = 1 (page 0 0xEC bit 6); the status is from HW TPI; with vp_sel = 0; the status from normal path.
  #define BIT_MSK__RI_2__REG_RI_PRIME1                                         0xFF

// HDCP Ri 128 Compare Value Register
#define REG_ADDR__RI_128_COMP                                            (Page_0 | 0x0054)
  // (ReadWrite, Bits 6:0) 
  // HDCP Ri 128 Compare Value
  #define BIT_MSK__RI_128_COMP__REG_RI_128_COMP                                       0x7F

// HDCP I counter Register
#define REG_ADDR__HDCP_I_CNT                                             (Page_0 | 0x0055)
  // (ReadOnly, Bits 6:0) 
  // I counter
  #define BIT_MSK__HDCP_I_CNT__HDCP_I_CNT                                            0x7F

// Ri Status Register
#define REG_ADDR__RI_STAT                                                (Page_0 | 0x0056)
  // (ReadOnly, Bits 0) 
  // Ri check started status.  This signal is used for the handshaking between firmware and hardware. Once the Ri check is enabled; the hardware would wait for the DDC master to finish the current transaction to take control over it. Once this bit is set firmware loses ability to use DDC Master; unless it disables Ri Check and this bit goes to 0. 
  #define BIT_MSK__RI_STAT__RI_ON                                                 0x01

// Ri Command Register
#define REG_ADDR__RI_CMD                                                 (Page_0 | 0x0057)
  // (ReadWrite, Bits 0) 
  // Enable Ri Check.  Need to check the Ri On bit (reg. 0x026[0]) for the firmware - hardware DDC control handshaking.  Should be enabled only after the 2nd step of authentication.
  #define BIT_MSK__RI_CMD__REG_RI_EN                                             0x01
  // (ReadWrite, Bits 1) 
  // enable polling of the BCAP done bit
  #define BIT_MSK__RI_CMD__REG_BCAP_EN                                           0x02

// Ri Line Start Register
#define REG_ADDR__RI_START                                               (Page_0 | 0x0058)
  // (ReadWrite, Bits 7:0) 
  // Indicates at what line within frame 127 or 0 to start Ri Check. This number is power of 2 - 2 lsb are 0; default is 17.
  #define BIT_MSK__RI_START__REG_RI_LN_NUM                                         0xFF

// Ri from Rx #1 Register
#define REG_ADDR__RI_RX_1                                                (Page_0 | 0x0059)
  // (ReadOnly, Bits 7:0) 
  // Ri value from Rx; bits 7:0 of 16 bit value.   The value in this register is stored in case of any of the Ri Check errors.
  #define BIT_MSK__RI_RX_1__RI_RX1                                                0xFF

// Ri from Rx #2 Register
#define REG_ADDR__RI_RX_2                                                (Page_0 | 0x005A)
  // (ReadOnly, Bits 7:0) 
  // Ri value from Rx; bits 15:8 of 16 bit value.   The value in this register is stored in case of any of the Ri Check errors.
  #define BIT_MSK__RI_RX_2__RI_RX2                                                0xFF

//  HDCP Debug Register
#define REG_ADDR__TXHDCP_DEBUG                                           (Page_0 | 0x005B)
  // (ReadWrite, Bits 6) 
  // Setting this bit to 1 will force stop update of the Ri values
  #define BIT_MSK__TXHDCP_DEBUG__REG_RI_HOLD                                           0x40
  // (ReadWrite, Bits 7) 
  // Setting this bit to 1 will force trush of Ri values (it will be set to 0)
  #define BIT_MSK__TXHDCP_DEBUG__REG_RI_TRUSH                                          0x80

// CLK Ratio Control Register
#define REG_ADDR__CLKRATIO                                               (Page_0 | 0x0065)
  // (ReadWrite, Bits 1:0) 
  // This register defines the ratio between IDCK and PCLK when reg_clkratio_sw_en is set. This is a special SWHR register. SW can write. When read; it is HW value decoded based on video_path_core configuration. 00: IDCK is half rate as PCLK. 01: IDCK is the same rate as PCLK (default) 10: Customized relationship driven by SoC without HW automation 11: reserved
  #define BIT_MSK__CLKRATIO__REG_IDCK2PCLK_RATIO                                   0x03
  // (ReadWrite, Bits 3:2) 
  // This register defines the ratio between PCLKNX and PCLK when reg_clkratio_sw_en is set. This is a special SWHR register. SW can write. When read; it is HW value decoded based on video_path_core configuration. 00: PCLKNX is half rate as PCLK. 01: PCLKNX is the same rate as PCLK (default) 10: PCLKNX is twice as fast as PCLK 11: PCLKNX is four times as fast as PCLK.
  #define BIT_MSK__CLKRATIO__REG_PCLKNX2PCLK_RATIO                                 0x0C
  // (ReadWrite, Bits 7) 
  // This register enables SW clock ratios; reg_idck2pclk_ratio and reg_pclknx2pclk_ratio.  1: enable SW clock ratio (default) 0: enable HW decoded clock ratio
  #define BIT_MSK__CLKRATIO__REG_CLKRATIO_SW_EN                                    0x80

// PCLK2TCLK Control Register
#define REG_ADDR__P2T_CTRL                                               (Page_0 | 0x0066)
  // (ReadWrite, Bits 1:0) 
  // PACK Mode 00:  8bpp (default) 01: 10bpp 10: 12bpp 11: 16bpp
  #define BIT_MSK__P2T_CTRL__REG_PACK_MODE                                         0x03
  // (ReadWrite, Bits 5) 
  // Null Packet Enable at VSYNC High 1: enable(default) 0: disable
  #define BIT_MSK__P2T_CTRL__REG_NULL_PKT_EN_VS_HI                                 0x20
  // (ReadWrite, Bits 6) 
  // Null Packet Enable bit 1: enable(default) 0: disable
  #define BIT_MSK__P2T_CTRL__REG_NULL_PKT_EN                                       0x40
  // (ReadWrite, Bits 7) 
  // Deep Color Packet Enable bit 1: enable(default) 0: disable
  #define BIT_MSK__P2T_CTRL__REG_DC_PKT_EN                                         0x80

// Video Blank Data Low Byte Register
#define REG_ADDR__VID_BLANK0                                             (Page_0 | 0x0067)
  // (ReadWrite, Bits 7:0) 
  // This byte defines the blank data for channel B/Cb. Default is 0.
  #define BIT_MSK__VID_BLANK0__REG_VIDEO_MUTE_DATA_B0                                0xFF

// Video Blank Data Mid Byte Register
#define REG_ADDR__VID_BLANK1                                             (Page_0 | 0x0068)
  // (ReadWrite, Bits 7:0) 
  // This byte defines the blank data for channel G/Y. Default is 0.
  #define BIT_MSK__VID_BLANK1__REG_VIDEO_MUTE_DATA_B1                                0xFF

// Video Blank Data High Byte Register
#define REG_ADDR__VID_BLANK2                                             (Page_0 | 0x0069)
  // (ReadWrite, Bits 7:0) 
  // This byte defines the blank data for channel R/Cr. Default is 0.
  #define BIT_MSK__VID_BLANK2__REG_VIDEO_MUTE_DATA_B2                                0xFF

// Video Override Register
#define REG_ADDR__VID_OVRRD                                              (Page_0 | 0x006A)
  // (ReadWrite, Bits 4) 
  // Enable MHL 3D fram packing conversion
  #define BIT_MSK__VID_OVRRD__REG_3DCONV_EN                                         0x10
  // (ReadWrite, Bits 5) 
  // Enable the insertion of a mini Vsync when reg_3dconv_en is set
  #define BIT_MSK__VID_OVRRD__REG_MINIVSYNC_ON                                      0x20

// Video Sync Polarity Detection Register
#define REG_ADDR__POL_DETECT                                             (Page_0 | 0x006F)
  // (ReadOnly, Bits 0) 
  // Internal circuit detected Hsync polarity: 0 - active high 1 - active low
  #define BIT_MSK__POL_DETECT__HSYNCPOLOUT                                           0x01
  // (ReadOnly, Bits 1) 
  // Internal circuit detected Vsync polarity: 0 - active high 1 - active low
  #define BIT_MSK__POL_DETECT__VSYNCPOLOUT                                           0x02
  // (ReadOnly, Bits 2) 
  // A high indicates the detection of interlace video: 1 - interlace video; 0 - noninterlaced. 
  #define BIT_MSK__POL_DETECT__INTERLACEDOUT                                         0x04

// CEA-861 VSI InfoFrame MHL IEEE No #0 Register
#define REG_ADDR__VSI_MHL_IEEE_NO_0                                      (Page_0 | 0x0072)
  // (ReadWrite, Bits 7:0) 
  // Read CEA-861 for detailed description of this register OUI=0x7CA61D
  #define BIT_MSK__VSI_MHL_IEEE_NO_0__REG_MHL_IEEE_NO_B7_B0                                 0xFF

// CEA-861 VSI InfoFrame MHL IEEE No #1 Register
#define REG_ADDR__VSI_MHL_IEEE_NO_1                                      (Page_0 | 0x0073)
  // (ReadWrite, Bits 7:0) 
  // Read CEA-861 for detailed description of this register OUI=0x7CA61D
  #define BIT_MSK__VSI_MHL_IEEE_NO_1__REG_MHL_IEEE_NO_B15_B8                                0xFF

// CEA-861 VSI InfoFrame MHL IEEE No #2 Register
#define REG_ADDR__VSI_MHL_IEEE_NO_2                                      (Page_0 | 0x0074)
  // (ReadWrite, Bits 7:0) 
  // Read CEA-861 for detailed description of this register OUI=0x7CA61D
  #define BIT_MSK__VSI_MHL_IEEE_NO_2__REG_MHL_IEEE_NO_B23_B16                               0xFF

// Packet Filter0 Register
#define REG_ADDR__PKT_FILTER_0                                           (Page_0 | 0x0075)
  // (ReadWrite, Bits 0) 
  // GCP Packet pass through disable. 0 - GCP packet received from the active input port is passed through to the output port 1 - disable the pass through
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT                                      0x01
  // (ReadWrite, Bits 1) 
  // Block ACR packet from pass-through 0 - Pass-through ACR packet from active input pipe 1 - Block ACR packet from active input pipe
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_CTS_PKT                                      0x02
  // (ReadWrite, Bits 2) 
  // Block AVI InfoFrame from pass-through 0 - Pass-through AVI InfoFrame from active input pipe  1 - Block AVI InfoFrame from active input pipe
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_AVI_PKT                                      0x04
  // (ReadWrite, Bits 3) 
  // Block Audio InfoFrame from pass-through 0 - Pass-through Audio InfoFrame from active input pipe 1 - Block Audio InfoFrame from active input pipe
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_AIF_PKT                                      0x08
  // (ReadWrite, Bits 4) 
  // Block SPD InfoFrame from pass-through 0 - Pass-through SPD InfoFrame from active input pipe 1 - Block SPD InfoFrame from active input pipe 
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_SPIF_PKT                                     0x10
  // (ReadWrite, Bits 5) 
  // Block MPEG InfoFrame from pass-through 0 - Pass-through MPEG InfoFrame from active input pipe 1 - Block MPEG InfoFrame from active input pipe
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_MPEG_PKT                                     0x20
  // (ReadWrite, Bits 6) 
  // Block Audio Content Protection packet (0x04) from pass-through 0 - Pass-through Audio Content Protection packet from active input pipe 1 - Block Audio Content Protection packet from active input pipe 
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_CEA_CP_PKT                                   0x40
  // (ReadWrite, Bits 7) 
  // Block Gamut Metadata packet from pass-through 0 - Pass-through Gamut Metadata packet from active input pipe 1 - Block Gamut Metadata packet from active input pipe
  #define BIT_MSK__PKT_FILTER_0__REG_DROP_CEA_GAMUT_PKT                                0x80

// Packet Filter1 Register
#define REG_ADDR__PKT_FILTER_1                                           (Page_0 | 0x0076)
  // (ReadWrite, Bits 0) 
  // Block Vendor-Specific InfoFrame from pass-through 0 - Pass-through Vendor-Specific InfoFrame from active input pipe  1 - Block Vendor-Specific InfoFrame from active input pipe
  #define BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT                                     0x01
  // (ReadWrite, Bits 1) 
  // Block Generic InfoFrame from pass-through 0 - Pass-through Generic InfoFrame from active input pipe 1 - Block Generic InfoFrame from active input pipe The Generic InfoFrame type is decided by the reg_drop_gen_type.
  #define BIT_MSK__PKT_FILTER_1__REG_DROP_GEN_PKT                                      0x02
  // (ReadWrite, Bits 2) 
  // Block Generic (2) InfoFrame from pass-through 0 - Pass-through Generic (2) InfoFrame from active input pipe  1 - Block Generic (2) InfoFrame from active input pipe  The Generic InfoFrame type is decided by the reg_drop_gen2_type.
  #define BIT_MSK__PKT_FILTER_1__REG_DROP_GEN2_PKT                                     0x04
  // (ReadWrite, Bits 3) 
  // Audio Packet pass through enable 0 - Disable the pass through for the Audio packets 1 - Audio packets received from the active input pipe are passed through to the output port 
  #define BIT_MSK__PKT_FILTER_1__REG_DROP_AUDIO_PKT                                    0x08
  // (ReadWrite, Bits 4) 
  // Block Null Packet from pass-through  0 - Pass-through Null Packet from active input pipe  1 - Block Null Packet from active input pipe
  #define BIT_MSK__PKT_FILTER_1__REG_DROP_NULL_PKT                                     0x10
  // (ReadWrite, Bits 6) 
  // Disable AVI override
  #define BIT_MSK__PKT_FILTER_1__REG_AVI_OVERRIDE_DIS                                  0x40
  // (ReadWrite, Bits 7) 
  // Disable VSIF override
  #define BIT_MSK__PKT_FILTER_1__REG_VSI_OVERRIDE_DIS                                  0x80

// DROP GEN PACKET TYPE 0 Register
#define REG_ADDR__DROP_GEN_TYPE_0                                        (Page_0 | 0x0077)
  // (ReadWrite, Bits 7:0) 
  // Block packets matched with this packet header type
  #define BIT_MSK__DROP_GEN_TYPE_0__REG_DROP_GEN_TYPE                                     0xFF

// DROP GEN PACKET TYPE 1 Register
#define REG_ADDR__DROP_GEN_TYPE_1                                        (Page_0 | 0x0078)
  // (ReadWrite, Bits 7:0) 
  // Block packets matched with this packet header type
  #define BIT_MSK__DROP_GEN_TYPE_1__REG_DROP_GEN2_TYPE                                    0xFF

// DI PASS THROUGH CONTROL Register
#define REG_ADDR__DIPT_CNTL                                              (Page_0 | 0x0079)
  // (ReadWrite, Bits 0) 
  // 1 - enable packets pass through function 0 - disable packets pass through function
  #define BIT_MSK__DIPT_CNTL__REG_PKT_BYP_MODE                                      0x01
  // (ReadWrite, Bits 1) 
  // 1 - enable audio packets pass through function 0 - disable audio packets pass through function
  #define BIT_MSK__DIPT_CNTL__REG_AUD_BYP_MODE                                      0x02
  // (ReadWrite, Bits 2) 
  // 1 - split audio packets and other static packets 0 - do not split audio packets and othre static packets
  #define BIT_MSK__DIPT_CNTL__REG_AUD_SPLIT_EN                                      0x04
  // (ReadWrite, Bits 3) 
  // 1 - the contens of deep color control information in General Control Packet are overrided 0 -  the contens of deep color control information in General Control Packet are not overrided
  #define BIT_MSK__DIPT_CNTL__REG_PB_OVR_DC_PKT_EN                                  0x08
  // (ReadWrite, Bits 4) 
  // 1 - the priority of pass through packets is high 0 - the priority of pass through packets is low 
  #define BIT_MSK__DIPT_CNTL__REG_PB_PRIORITY_CTL                                   0x10
  // (ReadWrite, Bits 5) 
  // 1 - the deep color packing mode information is masked 0 - the ddep color packing mode information is not masked This bit is used in bypassing deep color video without unpacking and packing it
  #define BIT_MSK__DIPT_CNTL__REG_DC_PACK_MODE_MASK                                 0x20

// system counter_0 Register
#define REG_ADDR__SYS_CNTR_0                                             (Page_0 | 0x007A)
  // (ReadWrite, Bits 7:0) 
  // System counter low with granularity 0x1000 in 2MHz clock domain. This value is to be loaded in the system counter.
  #define BIT_MSK__SYS_CNTR_0__REG_CNTR_VALUE_B7_B0                                  0xFF

// system counter_1 Register
#define REG_ADDR__SYS_CNTR_1                                             (Page_0 | 0x007B)
  // (ReadWrite, Bits 7:0) 
  // System counter high
  #define BIT_MSK__SYS_CNTR_1__REG_CNTR_VALUE_B15_B8                                 0xFF

// system counter_0 Status Register
#define REG_ADDR__SYS_CNTR_ST0                                           (Page_0 | 0x007C)
  // (ReadOnly, Bits 7:0) 
  // System counter low with granularity 0x1000 in 2MHz clock domain. This is the current value of the system counter.
  #define BIT_MSK__SYS_CNTR_ST0__SYS_CNTR_B7_B0                                        0xFF

// system counter_1 Status Register
#define REG_ADDR__SYS_CNTR_ST1                                           (Page_0 | 0x007D)
  // (ReadOnly, Bits 7:0) 
  // System counter high
  #define BIT_MSK__SYS_CNTR_ST1__SYS_CNTR_B15_B8                                       0xFF

// 1st Layer Interrupt Status #1 Register
#define REG_ADDR__L1_INTR_STAT_0                                         (Page_0 | 0x007E)
  // (ReadOnly, Bits 0) 
  // Aggregated INTR1 interrupts with masks 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B0                                       0x01
  // (ReadOnly, Bits 1) 
  // Aggregated INTR2 interrupts with masks 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B1                                       0x02
  // (ReadOnly, Bits 2) 
  // Aggregated INTR3 interrupts with masks 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B2                                       0x04
  // (ReadOnly, Bits 3) 
  // Aggregated INTR5 interrupts with masks 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B3                                       0x08
  // (ReadOnly, Bits 4) 
  // TPI IP interrupt 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B4                                       0x10
  // (ReadOnly, Bits 5) 
  // MHL_TX IP interrupt
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B5                                       0x20
  // (ReadOnly, Bits 6) 
  // Aggregated SCDC INTR interrupts with masks 
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B6                                       0x40
  // (ReadOnly, Bits 7) 
  // HDCP2X IP interrupt
  #define BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B7                                       0x80

// 1st Layer Interrupt Status #2 Register
#define REG_ADDR__L1_INTR_STAT_1                                         (Page_0 | 0x007F)
  // (ReadOnly, Bits 0) 
  // AIP IP interrupt
  #define BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B8                                       0x01
  // (ReadOnly, Bits 1) 
  // CEC IP interrupt
  #define BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B9                                       0x02
  // (ReadOnly, Bits 2) 
  // Video_Path_Core interrupt
  #define BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B10                                      0x04
  // (ReadOnly, Bits 3) 
  // Downstream EDID interrupt
  #define BIT_MSK__L1_INTR_STAT_1__L1_INTR_STAT_B11                                      0x08

// Hot Plug Connection Debouncing Timer 2 Register
#define REG_ADDR__HTPLG_T2                                               (Page_0 | 0x008A)
  // (ReadWrite, Bits 7:0) 
  // Hot Plug Detect debouncing timer counter 2nd stage
  #define BIT_MSK__HTPLG_T2__REG_HPD_T2                                            0xFF

// Hot Plug Connection Debouncing Timer 1 Register
#define REG_ADDR__HTPLG_T1                                               (Page_0 | 0x008B)
  // (ReadWrite, Bits 7:0) 
  // Hot Plug Detect debouncing timer counter 1st stage
  #define BIT_MSK__HTPLG_T1__REG_HPD_T1                                            0xFF

// Interrupt State Register
#define REG_ADDR__INTR_STATE                                             (Page_0 | 0x008E)
  // (ReadOnly, Bits 0) 
  // Interrupt state bit value.  It shows whether the interrupt is active or not.  It is one gate before the polarity is applied to the interrupt.  In other words; whenever the interrupt is asserted; this bit is high.
  #define BIT_MSK__INTR_STATE__INTR_STATE                                            0x01
  // (ReadWrite, Bits 7) 
  // Set software interrupt INTR2[2] when 1'b1 is written (self resetting) 
  #define BIT_MSK__INTR_STATE__REG_SOFTWARE                                          0x80

// Interrupt Source #1 Register
#define REG_ADDR__INTR1                                                  (Page_0 | 0x008F)
  // (ReadWrite, Bits 2) 
  // Counts frames after encryption is turned on and rolls over after 128 frames. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__INTR1__REG_INTR1_STAT2                                       0x04
  // (ReadWrite, Bits 3) 
  // MCU timeout for request. Write '1' to clear this bit.
  #define BIT_MSK__INTR1__REG_INTR1_STAT3                                       0x08
  // (ReadWrite, Bits 4) 
  // P0 PRIF ready stuck low due for too long and is forced back to high.
  #define BIT_MSK__INTR1__REG_INTR1_STAT4                                       0x10
  // (ReadWrite, Bits 5) 
  // Monitor Detect Interrupt. Not applicable for Cypress HDMI Mode. During normal operation: '1' if detection signal (RSEN) has changed logic level.  During power down: '1' if detection signal (RSEN) changes.  Asserted if set to 1. Write 1 to clear.
  #define BIT_MSK__INTR1__REG_INTR1_STAT5                                       0x20
  // (ReadWrite, Bits 6) 
  // Monitor Detect Interrupt; '1' if detection signal (HPD) has changed logic level. Asserted if set to 1. Write '1' to clear this bit.                                                                   
  #define BIT_MSK__INTR1__REG_INTR1_STAT6                                       0x40
  // (ReadWrite, Bits 7) 
  // 5000ms(HDCP); 2000ms(CBUS) wait time counter. Asserted if set to 1. Write '1' to clear this bit
  #define BIT_MSK__INTR1__REG_INTR1_STAT7                                       0x80

// Interrupt Source #2 Register
#define REG_ADDR__INTR2                                                  (Page_0 | 0x0090)
  // (ReadWrite, Bits 0) 
  // VSync active edge is recognized.  Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__INTR2__REG_INTR2_STAT0                                       0x01
  // (ReadWrite, Bits 1) 
  // TCLK_STABLE has changed state Interrupt.  '1' if internal signal indicating TCLK_STABLE has changed logic level; indicating a change in the stability of the clock going to TMDS (from stable to non-stable or vice-versa).  Read the SYS_STAT register bit 1 to see the live state of TCLK_STABLE. Asserted if set to 1. Write 1 to clear.
  #define BIT_MSK__INTR2__REG_INTR2_STAT1                                       0x02
  // (ReadWrite, Bits 2) 
  // The interrupt will be asserted when 1'b1 is written into INT_CTRL[7]
  #define BIT_MSK__INTR2__REG_INTR2_STAT2                                       0x04
  // (ReadWrite, Bits 5) 
  // ENC_EN changed from 1 to 0. Write 1 to clear
  #define BIT_MSK__INTR2__REG_INTR2_STAT5                                       0x20
  // (ReadWrite, Bits 6) 
  // Hash Done interrupt. Write 1 to clear.
  #define BIT_MSK__INTR2__REG_INTR2_STAT6                                       0x40
  // (ReadWrite, Bits 7) 
  // If 1 then detected that BCAP KSV ready bit is 1;  Write '1' to clear this bit.
  #define BIT_MSK__INTR2__REG_INTR2_STAT7                                       0x80

// Interrupt Source #3 Register
#define REG_ADDR__INTR3                                                  (Page_0 | 0x0091)
  // (ReadWrite, Bits 0) 
  // DDC FIFO is empty. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__INTR3__REG_INTR3_STAT0                                       0x01
  // (ReadWrite, Bits 1) 
  // DDC FIFO is full interrupt. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__INTR3__REG_INTR3_STAT1                                       0x02
  // (ReadWrite, Bits 2) 
  // DDC FIFO is half-full interrupt. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__INTR3__REG_INTR3_STAT2                                       0x04
  // (ReadWrite, Bits 3) 
  // DDC command is done interrupt. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__INTR3__REG_INTR3_STAT3                                       0x08
  // (ReadWrite, Bits 4) 
  // Ri and Ri don't match during 1st frame (default during frame #127 = reg. 0x025 -1); to clear write 1
  #define BIT_MSK__INTR3__REG_INTR3_STAT4                                       0x10
  // (ReadWrite, Bits 5) 
  // Ri and Ri don't match during 2nd frame (default during frame #0 = reg. 0x025);  to clear write 1
  #define BIT_MSK__INTR3__REG_INTR3_STAT5                                       0x20
  // (ReadWrite, Bits 6) 
  // Ri did not changed between frames #127 and #0 (can happens 1/65000); to clear write 1
  #define BIT_MSK__INTR3__REG_INTR3_STAT6                                       0x40
  // (ReadWrite, Bits 7) 
  // Ri reading was not done within one frame; to clear write 1
  #define BIT_MSK__INTR3__REG_INTR3_STAT7                                       0x80

// Interrupt Source #5 Register
#define REG_ADDR__INTR5                                                  (Page_0 | 0x0092)
  // (ReadWrite, Bits 2) 
  // MHL2 3-] FIFO Overflow/Undeflow. Asserted if set to 1. Write 1 to clear. Not applicable for Cypress HDMI Mode.
  #define BIT_MSK__INTR5__REG_INTR5_STAT2                                       0x04
  // (ReadWrite, Bits 3) 
  // MHL2/3 timing closure FIFO Overflow/Underflow. Asserted if set to 1. Write 1 to clear. Not applicable for Cypress HDMI Mode.
  #define BIT_MSK__INTR5__REG_INTR5_STAT3                                       0x08
  // (ReadWrite, Bits 6) 
  // MHL3 CTS FIFO overflow and underflow. Not applicable for Cypress.
  #define BIT_MSK__INTR5__REG_INTR5_STAT6                                       0x40
  // (ReadWrite, Bits 7) 
  // Packet Queue Overflow
  #define BIT_MSK__INTR5__REG_INTR5_STAT7                                       0x80

// Interrupt #1 Mask Register
#define REG_ADDR__INTR1_MASK                                             (Page_0 | 0x0095)
  // (ReadWrite, Bits 2) 
  // Enable INT1[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK2                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable INT1[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK3                                       0x08
  // (ReadWrite, Bits 4) 
  // Enable INT1[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK4                                       0x10
  // (ReadWrite, Bits 5) 
  // Enable INT1[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INT1[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INT1[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR1_MASK__REG_INTR1_MASK7                                       0x80

// Interrupt #2 Mask Register
#define REG_ADDR__INTR2_MASK                                             (Page_0 | 0x0096)
  // (ReadWrite, Bits 0) 
  // Enable INTR2[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK0                                       0x01
  // (ReadWrite, Bits 1) 
  // Enable INTR2[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK1                                       0x02
  // (ReadWrite, Bits 2) 
  // Enable INTR2[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK2                                       0x04
  // (ReadWrite, Bits 5) 
  // Enable INTR2[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INTR2[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INTR2[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR2_MASK__REG_INTR2_MASK7                                       0x80

// Interrupt #3 Mask Register
#define REG_ADDR__INTR3_MASK                                             (Page_0 | 0x0097)
  // (ReadWrite, Bits 0) 
  // Enable INTR3[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK0                                       0x01
  // (ReadWrite, Bits 1) 
  // Enable INTR3[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK1                                       0x02
  // (ReadWrite, Bits 2) 
  // Enable INTR3[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK2                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable INTR3[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK3                                       0x08
  // (ReadWrite, Bits 4) 
  // Enable INTR3[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK4                                       0x10
  // (ReadWrite, Bits 5) 
  // Enable INTR3[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INTR3[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INTR3[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__INTR3_MASK__REG_INTR3_MASK7                                       0x80

// Interrupt #5 Mask Register
#define REG_ADDR__INTR5_MASK                                             (Page_0 | 0x0098)
  // (ReadWrite, Bits 2) 
  // Enable INT5[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__INTR5_MASK__REG_INTR5_MASK2                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable INT5[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__INTR5_MASK__REG_INTR5_MASK3                                       0x08
  // (ReadWrite, Bits 5) 
  // Enable INT5[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__INTR5_MASK__REG_INTR5_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INT5[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__INTR5_MASK__REG_INTR5_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INT5[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__INTR5_MASK__REG_INTR5_MASK7                                       0x80

// Hot Plug Connection Control Register
#define REG_ADDR__HPD_CTRL                                               (Page_0 | 0x009B)
  // (ReadWrite, Bits 4) 
  // 0 : HPD output = SET_HPD status; 1: HPD output = reg_hpd_out_ovr_val
  #define BIT_MSK__HPD_CTRL__REG_HPD_OUT_OVR_EN                                    0x10
  // (ReadWrite, Bits 5) 
  // 0 : HPD output = LOW (if reg_hpd_out_ovr_en=1); 1: HPD output = HIGH (if reg_hpd_out_ovr_en=1)
  #define BIT_MSK__HPD_CTRL__REG_HPD_OUT_OVR_VAL                                   0x20
  // (ReadWrite, Bits 6) 
  // Open Drain 0: disable 1: enable (default)
  #define BIT_MSK__HPD_CTRL__REG_HPD_OUT_OD_EN                                     0x40
  // (ReadWrite, Bits 7) 
  // This value is ORed with Downstream HPD internally.
  #define BIT_MSK__HPD_CTRL__REG_HPD_DS_SIGNAL                                     0x80

// TMDS Clock Status Register
#define REG_ADDR__TMDS_CSTAT                                             (Page_0 | 0x009F)
  // (ReadOnly, Bits 2) 
  // HDMI RPWR5V status 0 - 5V gone and cable disconnected 1 - Cable connected
  #define BIT_MSK__TMDS_CSTAT__RPWR5V                                                0x04

// TMDS Control #4 Register
#define REG_ADDR__TMDS_CTRL4                                             (Page_0 | 0x00A3)
  // (ReadWrite, Bits 0) 
  // Enable SCDT to control tx_en for MHL TMDS TX 0: Disable ( to default by #23345) 1: Enable (default to non-default by #23345)
  #define BIT_MSK__TMDS_CTRL4__REG_TX_EN_BY_SCDT                                     0x01

// SCDT Holdoff MSB Register
#define REG_ADDR__SCDT_HOLDOFF_MSB                                       (Page_0 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // MSB of 24 bit holdoff counter threshold that deglitches SCDT high active status.
  #define BIT_MSK__SCDT_HOLDOFF_MSB__REG_SCDT_HOLDOFF_MSB_B7_B0                            0xFF

// TMDS Control #7 Register
#define REG_ADDR__TMDS_CTRL7                                             (Page_0 | 0x00A6)
  // (ReadWrite, Bits 7) 
  // This bit enables/disables MHL TMDS sequence 0 - disable (default) 1 - enable
  #define BIT_MSK__TMDS_CTRL7__REG_TMDS_SWAP_BIT                                     0x80

// HPD In Override Register
#define REG_ADDR__HPD_IN_CTRL                                            (Page_0 | 0x00A7)
  // (ReadWrite, Bits 0) 
  // HPD input override enable 0:  Auto detect (default) 1:  Software set
  #define BIT_MSK__HPD_IN_CTRL__REG_HPDIN_OVER_EN                                     0x01
  // (ReadWrite, Bits 1) 
  // HPD input override select 0:  HDMI (default) 1:  CBUS
  #define BIT_MSK__HPD_IN_CTRL__REG_HPDIN_OVER_SEL                                    0x02

// LM DDC Register
#define REG_ADDR__LM_DDC                                                 (Page_0 | 0x00CC)
  // (ReadWrite, Bits 0) 
  // 0 = Not using DDC 1 = Request to use DDC
  #define BIT_MSK__LM_DDC__REG_DDC_GPU_REQUEST                                   0x01
  // (ReadOnly, Bits 1) 
  // DDC Grant 1'b0: TX ownx DDC bus 1'b1: Host is granted DDC bus access
  #define BIT_MSK__LM_DDC__REG_DDC_GRANT                                         0x02
  // (ReadWrite, Bits 2) 
  // Write 1'b1 to flip the switch from DDC to I2C so that I2C can access the downstream DDC slave through CBUS like DDC. The switch will be flipped back after STOP condition.
  #define BIT_MSK__LM_DDC__REG_DDC_TPI_SW                                        0x04
  // (ReadWrite, Bits 5) 
  // Video mute enable
  #define BIT_MSK__LM_DDC__REG_VIDEO_MUTE_EN                                     0x20
  // (ReadWrite, Bits 7) 
  // SW TPI enable
  #define BIT_MSK__LM_DDC__REG_SW_TPI_EN                                         0x80

// TX SHA Control Register
#define REG_ADDR__TXSHA_CTRL                                             (Page_0 | 0x00D1)
  // (ReadWrite, Bits 0) 
  // Firmware starts the SHA generation by writing 1; which generates 1 clock strobe.  Reading this bit  will not read what was written; but rather a status of SHA producing  a V value; such as:  If 0 is read then SHA is processing and V is not ready. If 1 is read then SHA is done processing and V is ready.  SHA processes very quickly and will often be finished by the time a read can be performed.  Note: before the above sequence this bit will return a 1 when read. In order to make for sure that SHA picked 1 written to this bit; firmware need to check bit #1 of the same register.
  #define BIT_MSK__TXSHA_CTRL__REG_SHA_GO_STAT                                       0x01
  // (ReadWrite, Bits 1) 
  // If 1 means that SHA picked up the SHA go stat command.  Write 1 to clear; before setting new SHA go stat command.
  #define BIT_MSK__TXSHA_CTRL__REG_SHACTRL_STAT1                                     0x02

// TX KSV FIFO Register
#define REG_ADDR__TXKSV_FIFO                                             (Page_0 | 0x00D2)
  // (ReadWrite, Bits 7:0) 
  // This Address  is a port for access to the KSV FIFO. When the firmware starts a  I2C transaction with the offset address set at 38h the access control will be transferred to the KSV FIFO.  The address located inside the KSV Start Address register acts as the start offset within the KSV FIFO space.   Consecutive I2C transactions to address 38h will be auto-incremented in the KSV FIFO Address space.
  #define BIT_MSK__TXKSV_FIFO__REG_KSV_FIFO_OUT                                      0xFF

// HDCP Repeater Down Stream BSTATUS #1 Register
#define REG_ADDR__TXDS_BSTATUS1                                          (Page_0 | 0x00D3)
  // (ReadWrite, Bits 6:0) 
  // need description
  #define BIT_MSK__TXDS_BSTATUS1__REG_DS_DEV_CNT                                        0x7F
  // (ReadWrite, Bits 7) 
  // need description
  #define BIT_MSK__TXDS_BSTATUS1__REG_DS_DEV_EXCEED                                     0x80

// HDCP Repeater Down Stream BSTATUS #2 Register
#define REG_ADDR__TXDS_BSTATUS2                                          (Page_0 | 0x00D4)
  // (ReadWrite, Bits 2:0) 
  // Depth - need description
  #define BIT_MSK__TXDS_BSTATUS2__REG_DS_DEPTH                                          0x07
  // (ReadWrite, Bits 3) 
  // Max cascade exceeded - need description
  #define BIT_MSK__TXDS_BSTATUS2__REG_DS_CASC_EXCEED                                    0x08
  // (ReadWrite, Bits 4) 
  // HDMI Mode. (controlled through the I2C local side) 1 - receiver is in HDMI Mode; 0 - receiver is in DVI mode.
  #define BIT_MSK__TXDS_BSTATUS2__REG_DS_HDMI_MODE                                      0x10
  // (ReadWrite, Bits 7:5) 
  // Bstatus bits
  #define BIT_MSK__TXDS_BSTATUS2__REG_DS_BSTATUS                                        0xE0

// HDCP Repeater V.H0 #0 Register
#define REG_ADDR__TXVH0_0                                                (Page_0 | 0x00D8)
  // (ReadWrite, Bits 7:0) 
  // Vp.H0 bits [7:0] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH0_0__REG_VP_H0_W_B0                                        0xFF

// HDCP Repeater V.H0 #1 Register
#define REG_ADDR__TXVH0_1                                                (Page_0 | 0x00D9)
  // (ReadWrite, Bits 7:0) 
  // Vp.H0 bits [15:8] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config
  #define BIT_MSK__TXVH0_1__REG_VP_H0_W_B1                                        0xFF

// HDCP Repeater V.H0 #2 Register
#define REG_ADDR__TXVH0_2                                                (Page_0 | 0x00DA)
  // (ReadWrite, Bits 7:0) 
  // Vp.H0 bits [23:16] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH0_2__REG_VP_H0_W_B2                                        0xFF

// HDCP Repeater V.H0 #3 Register
#define REG_ADDR__TXVH0_3                                                (Page_0 | 0x00DB)
  // (ReadWrite, Bits 7:0) 
  // Vp.H0 bits [31:24] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH0_3__REG_VP_H0_W_B3                                        0xFF

// HDCP Repeater V.H1 #0 Register
#define REG_ADDR__TXVH1_0                                                (Page_0 | 0x00DC)
  // (ReadWrite, Bits 7:0) 
  // Vp.H1 bits [7:0] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH1_0__REG_VP_H1_W_B0                                        0xFF

// HDCP Repeater V.H1 #1 Register
#define REG_ADDR__TXVH1_1                                                (Page_0 | 0x00DD)
  // (ReadWrite, Bits 7:0) 
  // Vp.H1 bits [15:8] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH1_1__REG_VP_H1_W_B1                                        0xFF

// HDCP Repeater V.H1 #2 Register
#define REG_ADDR__TXVH1_2                                                (Page_0 | 0x00DE)
  // (ReadWrite, Bits 7:0) 
  // Vp.H1 bits [23:16] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH1_2__REG_VP_H1_W_B2                                        0xFF

// HDCP Repeater V.H1 #3 Register
#define REG_ADDR__TXVH1_3                                                (Page_0 | 0x00DF)
  // (ReadWrite, Bits 7:0) 
  // Vp.H1 bits [31:24] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH1_3__REG_VP_H1_W_B3                                        0xFF

// HDCP Repeater V.H2 #0 Register
#define REG_ADDR__TXVH2_0                                                (Page_0 | 0x00E0)
  // (ReadWrite, Bits 7:0) 
  // Vp.H2 bits [7:0] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH2_0__REG_VP_H2_W_B0                                        0xFF

// HDCP Repeater V.H2 #1 Register
#define REG_ADDR__TXVH2_1                                                (Page_0 | 0x00E1)
  // (ReadWrite, Bits 7:0) 
  // Vp.H2 bits [15:8] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH2_1__REG_VP_H2_W_B1                                        0xFF

// HDCP Repeater V.H2 #2 Register
#define REG_ADDR__TXVH2_2                                                (Page_0 | 0x00E2)
  // (ReadWrite, Bits 7:0) 
  // Vp.H2 bits [23:16] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH2_2__REG_VP_H2_W_B2                                        0xFF

// HDCP Repeater V.H2 #3 Register
#define REG_ADDR__TXVH2_3                                                (Page_0 | 0x00E3)
  // (ReadWrite, Bits 7:0) 
  // Vp.H2 bits [31:24] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH2_3__REG_VP_H2_W_B3                                        0xFF

// HDCP Repeater V.H3 #0 Register
#define REG_ADDR__TXVH3_0                                                (Page_0 | 0x00E4)
  // (ReadWrite, Bits 7:0) 
  // Vp.H3 bits [7:0] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH3_0__REG_VP_H3_W_B0                                        0xFF

// HDCP Repeater V.H3 #1 Register
#define REG_ADDR__TXVH3_1                                                (Page_0 | 0x00E5)
  // (ReadWrite, Bits 7:0) 
  // Vp.H3 bits [15:8] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH3_1__REG_VP_H3_W_B1                                        0xFF

// HDCP Repeater V.H3 #2 Register
#define REG_ADDR__TXVH3_2                                                (Page_0 | 0x00E6)
  // (ReadWrite, Bits 7:0) 
  // Vp.H3 bits [23:16] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH3_2__REG_VP_H3_W_B2                                        0xFF

// HDCP Repeater V.H3 #3 Register
#define REG_ADDR__TXVH3_3                                                (Page_0 | 0x00E7)
  // (ReadWrite, Bits 7:0) 
  // Vp.H3 bits [31:24] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH3_3__REG_VP_H3_W_B3                                        0xFF

// HDCP Repeater V.H4 #0 Register
#define REG_ADDR__TXVH4_0                                                (Page_0 | 0x00E8)
  // (ReadWrite, Bits 7:0) 
  // Vp.H4 bits [7:0] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH4_0__REG_VP_H4_W_B0                                        0xFF

// HDCP Repeater V.H4 #1 Register
#define REG_ADDR__TXVH4_1                                                (Page_0 | 0x00E9)
  // (ReadWrite, Bits 7:0) 
  // Vp.H4 bits [15:8] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH4_1__REG_VP_H4_W_B1                                        0xFF

// HDCP Repeater V.H4 #2 Register
#define REG_ADDR__TXVH4_2                                                (Page_0 | 0x00EA)
  // (ReadWrite, Bits 7:0) 
  // Vp.H4 bits [23:16] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH4_2__REG_VP_H4_W_B2                                        0xFF

// HDCP Repeater V.H4 #3 Register
#define REG_ADDR__TXVH4_3                                                (Page_0 | 0x00EB)
  // (ReadWrite, Bits 7:0) 
  // Vp.H4 bits [31:24] with vp_sel = 1 (page 0 0xEC bit 6). When vp_sel = 0 it is config type
  #define BIT_MSK__TXVH4_3__REG_VP_H4_W_B3                                        0xFF

// DDC I2C Manual Register
#define REG_ADDR__DDC_MANUAL                                             (Page_0 | 0x00EC)
  // (ReadOnly, Bits 0) 
  // DSCL input status.  This bit reflects the live status of the DSCL pin.
  #define BIT_MSK__DDC_MANUAL__IO_DSCL                                               0x01
  // (ReadOnly, Bits 1) 
  // DSDA input status.  This bit reflects the live status of the DSDA pin.
  #define BIT_MSK__DDC_MANUAL__IO_DSDA                                               0x02
  // (ReadWrite, Bits 2) 
  // Write 1 to abort DDC master. Self-clearing
  #define BIT_MSK__DDC_MANUAL__REG_DDCM_ABORT                                        0x04
  // (ReadWrite, Bits 3) 
  // Set to allow HW to drop the incoming GCP packets and automatically send downstream GCP packets
  #define BIT_MSK__DDC_MANUAL__REG_GCP_HW_CTL_EN                                     0x08
  // (ReadWrite, Bits 4) 
  // DSCL manual output value.  Only enabled onto the DSCL output if reg_man_ddc = 1.
  #define BIT_MSK__DDC_MANUAL__REG_DSCL                                              0x10
  // (ReadWrite, Bits 5) 
  // DSDA manual output value.  Only enabled onto the DSDA output if reg_man_ddc = 1.
  #define BIT_MSK__DDC_MANUAL__REG_DSDA                                              0x20
  // (ReadWrite, Bits 6) 
  // 1'b1: select V prime from TPI 1'b0: select V prime from downstream (read through DDC)
  #define BIT_MSK__DDC_MANUAL__VP_SEL                                                0x40
  // (ReadWrite, Bits 7) 
  // manual override of scl and sda output
  #define BIT_MSK__DDC_MANUAL__REG_MAN_DDC                                           0x80

// DDC I2C Target Slave Address Register
#define REG_ADDR__DDC_ADDR                                               (Page_0 | 0x00ED)
  // (ReadWrite, Bits 7:1) 
  // DDC target Slave address 
  #define BIT_MSK__DDC_ADDR__REG_DDC_ADDR                                          0xFE

// DDC I2C Target Segment Address Register
#define REG_ADDR__DDC_SEGM                                               (Page_0 | 0x00EE)
  // (ReadWrite, Bits 7:0) 
  // DDC Target Segment address 
  #define BIT_MSK__DDC_SEGM__REG_DDC_SEGMENT                                       0xFF

// DDC I2C Target Offset Adress Register
#define REG_ADDR__DDC_OFFSET                                             (Page_0 | 0x00EF)
  // (ReadWrite, Bits 7:0) 
  // target slave register offset
  #define BIT_MSK__DDC_OFFSET__REG_DDC_OFFSET                                        0xFF

// DDC I2C Data In count #1 Register
#define REG_ADDR__DDC_DIN_CNT1                                           (Page_0 | 0x00F0)
  // (ReadWrite, Bits 7:0) 
  // Bits [7:0] of the number of bytes to transfer in
  #define BIT_MSK__DDC_DIN_CNT1__REG_DDC_DIN_CNT_B7_B0                                 0xFF

// DDC I2C Data In count #2 Register
#define REG_ADDR__DDC_DIN_CNT2                                           (Page_0 | 0x00F1)
  // (ReadWrite, Bits 1:0) 
  // Bits [9:8] of the number of bytes to transfer in
  #define BIT_MSK__DDC_DIN_CNT2__REG_DDC_DIN_CNT_B9_B8                                 0x03

// DDC I2C Status Register
#define REG_ADDR__DDC_STATUS                                             (Page_0 | 0x00F2)
  // (ReadOnly, Bits 0) 
  // DDC fifo write in use
  #define BIT_MSK__DDC_STATUS__DDC_FIFO_WRITE_IN_USE                                 0x01
  // (ReadOnly, Bits 1) 
  // DDC fifo read in use
  #define BIT_MSK__DDC_STATUS__DDC_FIFO_READ_IN_SUE                                  0x02
  // (ReadOnly, Bits 2) 
  // DDC FIFO is empty
  #define BIT_MSK__DDC_STATUS__DDC_FIFO_EMPTY                                        0x04
  // (ReadOnly, Bits 3) 
  // DDC fifo full
  #define BIT_MSK__DDC_STATUS__DDC_FIFO_FULL                                         0x08
  // (ReadOnly, Bits 4) 
  // DDC in programming mode
  #define BIT_MSK__DDC_STATUS__DDC_I2C_IN_PROG                                       0x10
  // (ReadWrite, Bits 5) 
  // When read get the status from hardware: to clear write 0
  #define BIT_MSK__DDC_STATUS__REG_DDC_NO_ACK                                        0x20
  // (ReadWrite, Bits 6) 
  // When read get the status from hardware: to clear write 0
  #define BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW                                       0x40

// DDC I2C Command Register
#define REG_ADDR__DDC_CMD                                                (Page_0 | 0x00F3)
  // (ReadWrite, Bits 3:0) 
  // DDC Command[3:0]: 1111 - Abort transaction 1001 - Clear FIFO 1010 - Clock SCL 0000 - Current address read with no ACK on last byte 0001 - Current address read with ACK on last byte 0010 - Sequential read with no ACK on last byte 0011 - Sequential read with ACK on last byte 0100 - Enhanced DDC read with no ACK on last byte 0101 - Enhanced DDC read with ACK on last byte 0110 - Sequential write ignoring ACK on last byte 0111 - Sequential write requiring ACK on last byte  write to this register generates 1 clock strobe; which will force decoding of the ddc command
  #define BIT_MSK__DDC_CMD__REG_DDC_CMD                                           0x0F
  // (ReadWrite, Bits 4) 
  // Enable 3ns glitch filtering on the DDC clock and data line. 0 - is enabled (default). Filtering is done using Ring Oscillator.
  #define BIT_MSK__DDC_CMD__REG_DDC_FLT_EN                                        0x10
  // (ReadWrite, Bits 5) 
  // Enable the DDC del: 0 - is enable (default). DDC delay has been inserted into the SDA line to create 300ns delay for the falling edge of the DDC SDA signal in order to avoid erroneous I2C START condition.  The real start condition must have setup time of the 600ns and therefore this delay of 300ns will not remove the real START condition.  Filtering is done using Ring Oscillator.
  #define BIT_MSK__DDC_CMD__REG_SDA_DEL_EN                                        0x20
  // (ReadWrite, Bits 6) 
  // Enable short Ri read for DDC
  #define BIT_MSK__DDC_CMD__REG_HDCP_DDC_EN                                       0x40

// DDC I2C FIFO Data In/Out Register
#define REG_ADDR__DDC_DATA                                               (Page_0 | 0x00F4)
  // (ReadOnly, Bits 7:0) 
  // DDC FIFO register.  During write to FIFO write data into his register During the read from FIFO this register will have the FIFO data
  #define BIT_MSK__DDC_DATA__DDC_DATA_OUT                                          0xFF

// DDC I2C Data Out Counter Register
#define REG_ADDR__DDC_DOUT_CNT                                           (Page_0 | 0x00F5)
  // (ReadOnly, Bits 4:0) 
  // Data out counter register
  #define BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT                                      0x1F
  // (ReadWrite, Bits 7) 
  // DDC I2C delay counter register 9-bit of 9
  #define BIT_MSK__DDC_DOUT_CNT__REG_DDC_DELAY_CNT_B1                                  0x80

// DDC I2C Delay Count Register
#define REG_ADDR__DDC_DELAY_CNT                                          (Page_0 | 0x00F6)
  // (ReadWrite, Bits 7:0) 
  // DDC I2C delay counter register 9-bit of 8
  #define BIT_MSK__DDC_DELAY_CNT__REG_DDC_DELAY_CNT_B0                                  0xFF

// Test Control Register
#define REG_ADDR__TEST_TXCTRL                                            (Page_0 | 0x00F7)
  // (ReadWrite, Bits 0) 
  // Test TMDS PLL Clocks. 0 - Test TMDS PLL Clocks disabled (default) 1 - TMDS filter PLL and main PLL Clocks muxed to test output pins.
  #define BIT_MSK__TEST_TXCTRL__REG_TST_PLLCK                                         0x01
  // (ReadWrite, Bits 1) 
  // Enable HDMI/MHL mode for output when  set 1. Output DVI mode when set 0.
  #define BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE                                         0x02

// I2C Status Register
#define REG_ADDR__EPST                                                   (Page_0 | 0x00F8)
  // (ReadWrite, Bits 0) 
  // Last command had been completed; For the status of this command please check error status bits in this register. Write 0 to clear. 
  #define BIT_MSK__EPST__REG_CMD_DONE_CLR                                      0x01
  // (ReadWrite, Bits 1) 
  // CRC error if  set to 1; Write 0 to clear
  #define BIT_MSK__EPST__REG_BIST_ERR_CLR                                      0x02
  // (ReadWrite, Bits 5) 
  // self-authentication #1 error if set to 1; Write 0 to clear
  #define BIT_MSK__EPST__REG_BIST1_ERR_CLR                                     0x20
  // (ReadWrite, Bits 6) 
  // self-authentication #2 error if set to 1; Write 0 to clear
  #define BIT_MSK__EPST__REG_BIST2_ERR_CLR                                     0x40
  // (ReadOnly, Bits 7) 
  // Status of the OTP programming: 1 - OTP has not been locked and can be programmed again.  The OTP is based on the Jet City fuse based memory and once the fuse has been blown it can be changed. That why every consequent write to the OTP memory will be the OR of the previous ones.
  #define BIT_MSK__EPST__OTP_UNLOCKED                                          0x80

// I2C Command Register
#define REG_ADDR__EPCM                                                   (Page_0 | 0x00F9)
  // (ReadWrite, Bits 4:0) 
  // I2C Master commands: Five bits are SA2_en : SA1_en : CRC_en : 2 bit Command 00000 - Clear command register; no action will be taken xxx01 - rsvd xxx10 - rsvd xxx11 - will enable all BIST: CRC; self-authentication #1 and self_authentication #2 xx100 - will enable individual CRC  x1x00 - will enable individual self-authentication #1  1xx00 - will enable individual self_authentication #2   New value can be safely written into this register only after getting command done bit (reg. 0x0F9[0]) or waiting at least 0.125s. That means that new value can be safely written into this register only after previous command has been completed. Otherwise; it is possible to get either self-authentication #1 error status bit or to get BIST done bit without actually performing one of the new enabled BIST test.
  #define BIT_MSK__EPCM__REG_EPCM                                              0x1F
  // (ReadWrite, Bits 5) 
  // Enable Load of the KSV from OTP : 1 - enable.  Write 0 before enabling again. 
  #define BIT_MSK__EPCM__REG_LD_KSV                                            0x20

// I2C Command Register
#define REG_ADDR__OTP_CLK                                                (Page_0 | 0x00FA)
  // (ReadWrite, Bits 5) 
  // 1: enabled the writing of protect byte (xff) in otp rom.  Default value 0: protect byte will not be written to otp rom.
  #define BIT_MSK__OTP_CLK__REG_OTP_PROTECT_EN                                    0x20

//***************************************************************************
// page 31. Address: 40
// SYS Register
#define REG_ADDR__UTSRST                                                 (Page_1 | 0x0000)
  // (ReadWrite, Bits 0) 
  // Active high reset signal for hsic Rx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_HRX_SRST                                          0x01
  // (ReadWrite, Bits 1) 
  // Active high reset signal for TDM Tx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_TTX_SRST                                          0x02
  // (ReadWrite, Bits 2) 
  // Active high reset signal for TDM Rx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_TRX_SRST                                          0x04
  // (ReadWrite, Bits 3) 
  // Active high reset signal for hsic Tx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_HTX_SRST                                          0x08
  // (ReadWrite, Bits 4) 
  // Active high reset signal for hsic keeper. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_KEEPER_SRST                                       0x10
  // (ReadWrite, Bits 5) 
  // Active high reset signal for hsic flow control Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_FC_SRST                                           0x20
  // (ReadWrite, Bits 6) 
  // Active high reset signal for hsic Rx/Tx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__UTSRST__REG_HCM_SRST                                          0x40
  // (ReadOnly, Bits 7) 
  // general interrupt status.
  #define BIT_MSK__UTSRST__REG_INTR                                              0x80

// Aggregated INT Low Register
#define REG_ADDR__AGGINTL                                                (Page_1 | 0x0001)
  // (ReadOnly, Bits 7:0) 
  // Aggregated interrupt status. Bit 0 for HRXINTL; Bit 1 for HRXINTH; Bit 2 for HTXINTL; Bit 3 for HTXINTH; Bit 4 for FCINT0; Bit 5 for FCINT1; Bit 6 for FCINT2; Bit 7 for FCINT3; Bit 8 for FCINT4; Bit 9 for FCINT5; Bit 10 for FCINT6; Bit 11 for FCINT7. 
  #define BIT_MSK__AGGINTL__REG_AGGRINTR_B7_B0                                    0xFF

// Aggregated INT High Register
#define REG_ADDR__AGGINTH                                                (Page_1 | 0x0002)
  // (ReadOnly, Bits 3:0) 
  // Aggregated interrupt status.
  #define BIT_MSK__AGGINTH__REG_AGGRINTR_B11_B8                                   0x0F

// HSIC RX INT Low Register
#define REG_ADDR__HRXINTL                                                (Page_1 | 0x0003)
  // (ReadWrite, Bits 0) 
  // event 0 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. No EOP at end of USB packet
  #define BIT_MSK__HRXINTL__REG_HRX_INTR0                                         0x01
  // (ReadWrite, Bits 1) 
  // event 1 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Short pulse detected on HSIC bus (Short CONNECT detected)
  #define BIT_MSK__HRXINTL__REG_HRX_INTR1                                         0x02
  // (ReadWrite, Bits 2) 
  // event 2 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC2TDM AFIFO full
  #define BIT_MSK__HRXINTL__REG_HRX_INTR2                                         0x04
  // (ReadWrite, Bits 3) 
  // event 3 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC AFIFO full
  #define BIT_MSK__HRXINTL__REG_HRX_INTR3                                         0x08
  // (ReadWrite, Bits 4) 
  // event 4 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTL__REG_HRX_INTR4                                         0x10
  // (ReadWrite, Bits 5) 
  // event 5 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTL__REG_HRX_INTR5                                         0x20
  // (ReadWrite, Bits 6) 
  // event 6 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTL__REG_HRX_INTR6                                         0x40
  // (ReadWrite, Bits 7) 
  // event 7 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTL__REG_HRX_INTR7                                         0x80

// HSIC RX INT High Register
#define REG_ADDR__HRXINTH                                                (Page_1 | 0x0004)
  // (ReadWrite, Bits 0) 
  // event 8 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC bus status CONNECT/RESUME started
  #define BIT_MSK__HRXINTH__REG_HRX_INTR8                                         0x01
  // (ReadWrite, Bits 1) 
  // event 9 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC bus status CONNECT/RESUME ended
  #define BIT_MSK__HRXINTH__REG_HRX_INTR9                                         0x02
  // (ReadWrite, Bits 2) 
  // event 10 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC bus status RESET started
  #define BIT_MSK__HRXINTH__REG_HRX_INTR10                                        0x04
  // (ReadWrite, Bits 3) 
  // event 11 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC bus status RESET ended
  #define BIT_MSK__HRXINTH__REG_HRX_INTR11                                        0x08
  // (ReadWrite, Bits 4) 
  // event 12 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1.  HSIC packet started
  #define BIT_MSK__HRXINTH__REG_HRX_INTR12                                        0x10
  // (ReadWrite, Bits 5) 
  // event 13 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. HSIC packet ended
  #define BIT_MSK__HRXINTH__REG_HRX_INTR13                                        0x20
  // (ReadWrite, Bits 6) 
  // event 14 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTH__REG_HRX_INTR14                                        0x40
  // (ReadWrite, Bits 7) 
  // event 15 of ro_hsic_rx_intr[15:0]. Interrupt from hsic_rx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HRXINTH__REG_HRX_INTR15                                        0x80

// HSIC TX INT Low Register
#define REG_ADDR__HTXINTL                                                (Page_1 | 0x0005)
  // (ReadWrite, Bits 0) 
  // event 0 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Continuous NULL in USB packet
  #define BIT_MSK__HTXINTL__REG_HTX_INTR0                                         0x01
  // (ReadWrite, Bits 1) 
  // event 1 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. USB sync (SOP) error
  #define BIT_MSK__HTXINTL__REG_HTX_INTR1                                         0x02
  // (ReadWrite, Bits 2) 
  // event 2 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. HSIC IO conflict error
  #define BIT_MSK__HTXINTL__REG_HTX_INTR2                                         0x04
  // (ReadWrite, Bits 3) 
  // event 3 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. State PRE timeout (Too short USB packet)
  #define BIT_MSK__HTXINTL__REG_HTX_INTR3                                         0x08
  // (ReadWrite, Bits 4) 
  // event 4 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HTXINTL__REG_HTX_INTR4                                         0x10
  // (ReadWrite, Bits 5) 
  // event 5 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HTXINTL__REG_HTX_INTR5                                         0x20
  // (ReadWrite, Bits 6) 
  // event 6 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HTXINTL__REG_HTX_INTR6                                         0x40
  // (ReadWrite, Bits 7) 
  // event 7 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__HTXINTL__REG_HTX_INTR7                                         0x80

// HSIC TX INT High Register
#define REG_ADDR__HTXINTH                                                (Page_1 | 0x0006)
  // (ReadWrite, Bits 0) 
  // event 8 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR8                                         0x01
  // (ReadWrite, Bits 1) 
  // event 9 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR9                                         0x02
  // (ReadWrite, Bits 2) 
  // event 10 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR10                                        0x04
  // (ReadWrite, Bits 3) 
  // event 11 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR11                                        0x08
  // (ReadWrite, Bits 4) 
  // event 12 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR12                                        0x10
  // (ReadWrite, Bits 5) 
  // event 13 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR13                                        0x20
  // (ReadWrite, Bits 6) 
  // event 14 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR14                                        0x40
  // (ReadWrite, Bits 7) 
  // event 15 of ro_hsic_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1.
  #define BIT_MSK__HTXINTH__REG_HTX_INTR15                                        0x80

// HSIC Flow Control INTR0 Register
#define REG_ADDR__FCINTR0                                                (Page_1 | 0x0007)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 0 event 0, PID mismtach error
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 0 event 1, CRC error
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 0 event 2, State machine error
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 0 event 3, Too short USB packet
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 0 event 4, hsic2mem FIFO overflow
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 0 event 5, hsic2mem FIFO overflow for periodic
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 0 event 6, hsic2mem CRC16 error
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 0 event 7, Reserved
  #define BIT_MSK__FCINTR0__REG_HFC0_INTR7                                        0x80

// HSIC Flow Control INTR1 Register
#define REG_ADDR__FCINTR1                                                (Page_1 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 1 event 0, FIFO empty
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 1 event 1, hsic2link memory overflow error
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 1 event 2, link2hsic memory overflow error (non-perodic if multiple outsanding mode, single otherwise)  (disabled by ri_hsic_fc_control[125])
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 1 event 3, link2hsic memory overflow error (perodic if multiple outsanding mode, 1'b0 otherwise) (disabled by ri_hsic_fc_control[125])
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 1 event 4, FIFO full
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 1 event 5, Reserved
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 1 event 6, Reserved
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 1 event 7, Reserved
  #define BIT_MSK__FCINTR1__REG_HFC1_INTR7                                        0x80

// HSIC Flow Control INTR2 Register
#define REG_ADDR__FCINTR2                                                (Page_1 | 0x0009)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 2 event 0, Continuous NULL in USB packet
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 2 event 1, USB sync (SOP error)
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 2 event 2, Reserved (HSIC IO conflict error)
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 2 event 3, State PRE timeout (Too short USB packet)
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 2 event 4, Reserved
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 2 event 5, Reserved
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 2 event 6, Reserved
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 2 event 7, Reserved
  #define BIT_MSK__FCINTR2__REG_HFC2_INTR7                                        0x80

// HSIC Flow Control INTR3 Register
#define REG_ADDR__FCINTR3                                                (Page_1 | 0x000A)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 3 event 0, HSIC bus status CONNECT/RESUME started
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 3 event 1, HSIC bus status CONNECT/RESUME ended
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 3 event 2, HSIC bus status RESET started
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 3 event 3, HSIC bus status RESET ended
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 3 event 4, HSIC packet started
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 3 event 5, HSIC packet ended
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 3 event 6, Reserved
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 3 event 7, Reserved
  #define BIT_MSK__FCINTR3__REG_HFC3_INTR7                                        0x80

// HSIC Flow Control INTR4 Register
#define REG_ADDR__FCINTR4                                                (Page_1 | 0x000B)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 4 event 0, PID mismtach error
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 4 event 1, CRC error
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 4 event 2, State machine error
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 4 event 3, Too short USB packet
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 4 event 4, hsic2mem FIFO overflow
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 4 event 5, link2mem FIFO overflow for periodic
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 4 event 6, link2mem CRC16 error
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 4 event 7, Reserved
  #define BIT_MSK__FCINTR4__REG_HFC4_INTR7                                        0x80

// HSIC Flow Control INTR5 Register
#define REG_ADDR__FCINTR5                                                (Page_1 | 0x000C)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 5 event 0, FIFO full
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 5 event 1, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 5 event 2, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 5 event 3, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 5 event 4, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 5 event 5, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 5 event 6, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 5 event 7, Reserved
  #define BIT_MSK__FCINTR5__REG_HFC5_INTR7                                        0x80

// HSIC Flow Control INTR6 Register
#define REG_ADDR__FCINTR6                                                (Page_1 | 0x000D)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 6 event 0, mem2link FIFO overflow
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 6 event 1, mem2hsic FIFO overflow
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 6 event 2, Unexpected USB packet
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 6 event 3, Turnaround timeout
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 6 event 4, Interpaket delay timeout
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 6 event 5, Outstanding knockout
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 6 event 6, SOF Rx timeout (Host enters low power mode)
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 6 event 7, SOF Tx error (SOF generation is skipped)
  #define BIT_MSK__FCINTR6__REG_HFC6_INTR7                                        0x80

// HSIC Flow Control INTR7 Register
#define REG_ADDR__FCINTR7                                                (Page_1 | 0x000E)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 7 event 0, SOF interval is too big/small
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR0                                        0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 7 event 1, Interrupt/Isochronous SSPLIT is sent at micro frame #6
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR1                                        0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 7 event 2, Turnaround timeout upon reception of DATA packet
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR2                                        0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 7 event 3, Turnaround timeout upon reception of DATA packet (multiple times and giveup)
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR3                                        0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 7 event 4, Reserved
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR4                                        0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 7 event 5, Reserved
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR5                                        0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 7 event 6, Reserved
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR6                                        0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 7 event 7, Reserved
  #define BIT_MSK__FCINTR7__REG_HFC7_INTR7                                        0x80

// HSIC RX INTMASK Low Register
#define REG_ADDR__HRXINTML                                               (Page_1 | 0x000F)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 0.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK0                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 1.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK1                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 2.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK2                                     0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 3.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK3                                     0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 4.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK4                                     0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 5.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK5                                     0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 6.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK6                                     0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 7.
  #define BIT_MSK__HRXINTML__REG_HRX_INTRMASK7                                     0x80

// HSIC RX INTMASK High Register
#define REG_ADDR__HRXINTMH                                               (Page_1 | 0x0010)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 8.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK8                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 9.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK9                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 10.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK10                                    0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 11.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK11                                    0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 12.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK12                                    0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 13.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK13                                    0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 14.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK14                                    0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 15.
  #define BIT_MSK__HRXINTMH__REG_HRX_INTRMASK15                                    0x80

// HSIC TX INTMASK Low Register
#define REG_ADDR__HTXINTML                                               (Page_1 | 0x0011)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 0.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK0                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 1.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK1                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 2.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK2                                     0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 3.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK3                                     0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 4.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK4                                     0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 5.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK5                                     0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 6.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK6                                     0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 7.
  #define BIT_MSK__HTXINTML__REG_HTX_INTRMASK7                                     0x80

// HSIC TX INTMASK High Register
#define REG_ADDR__HTXINTMH                                               (Page_1 | 0x0012)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 8.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK8                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 9.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK9                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 10.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK10                                    0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 11.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK11                                    0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 12.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK12                                    0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 13.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK13                                    0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 14.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK14                                    0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 15.
  #define BIT_MSK__HTXINTMH__REG_HTX_INTRMASK15                                    0x80

// HSIC Flow Control INTRMASK0 Register
#define REG_ADDR__FCINTRMASK0                                            (Page_1 | 0x0013)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 0 event mask 0
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 0 event mask 1
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 0 event mask 2
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 0 event mask 3
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 0 event mask 4
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 0 event mask 5
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 0 event mask 6
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 0 event mask 7
  #define BIT_MSK__FCINTRMASK0__REG_HFC0_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK1 Register
#define REG_ADDR__FCINTRMASK1                                            (Page_1 | 0x0014)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 1 event mask 0
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 1  event mask 1
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 1 event mask 2
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 1 event mask 3
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 1 event mask 4
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 1 event mask 5
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 1 event mask 6
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 1 event mask 7
  #define BIT_MSK__FCINTRMASK1__REG_HFC1_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK2 Register
#define REG_ADDR__FCINTRMASK2                                            (Page_1 | 0x0015)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 2 event mask 0
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 2  event mask 1
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 2 event mask 2
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 2 event mask 3
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 2 event mask 4
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 2 event mask 5
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 2 event mask 6
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 2 event mask 7
  #define BIT_MSK__FCINTRMASK2__REG_HFC2_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK3 Register
#define REG_ADDR__FCINTRMASK3                                            (Page_1 | 0x0016)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 3 event mask 0
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 3  event mask 1
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 3 event mask 2
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 3 event mask 3
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 3 event mask 4
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 3 event mask 5
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 3 event mask 6
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 3 event mask 7
  #define BIT_MSK__FCINTRMASK3__REG_HFC3_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK4 Register
#define REG_ADDR__FCINTRMASK4                                            (Page_1 | 0x0017)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 4 event mask 0
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 4  event mask 1
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 4 event mask 2
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 4 event mask 3
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 4 event mask 4
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 4 event mask 5
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 4 event mask 6
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 4 event mask 7
  #define BIT_MSK__FCINTRMASK4__REG_HFC4_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK5 Register
#define REG_ADDR__FCINTRMASK5                                            (Page_1 | 0x0018)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 5 event mask 0
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 5  event mask 1
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 5 event mask 2
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 5 event mask 3
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 5 event mask 4
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 5 event mask 5
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 5 event mask 6
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 5 event mask 7
  #define BIT_MSK__FCINTRMASK5__REG_HFC5_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK6 Register
#define REG_ADDR__FCINTRMASK6                                            (Page_1 | 0x0019)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 6 event mask 0
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 6  event mask 1
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 6 event mask 2
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 6 event mask 3
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 6 event mask 4
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 6 event mask 5
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 6 event mask 6
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 6 event mask 7
  #define BIT_MSK__FCINTRMASK6__REG_HFC6_INTRMASK7                                    0x80

// HSIC Flow Control INTRMASK7 Register
#define REG_ADDR__FCINTRMASK7                                            (Page_1 | 0x001A)
  // (ReadWrite, Bits 0) 
  // Flow control interrupt byte 7 event mask 0
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Flow control interrupt byte 7  event mask 1
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Flow control interrupt byte 7 event mask 2
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Flow control interrupt byte 7 event mask 3
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Flow control interrupt byte 7 event mask 4
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Flow control interrupt byte 7 event mask 5
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Flow control interrupt byte 7 event mask 6
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK6                                    0x40
  // (ReadWrite, Bits 7) 
  // Flow control interrupt byte 7 event mask 7
  #define BIT_MSK__FCINTRMASK7__REG_HFC7_INTRMASK7                                    0x80

// HSIC RX Control0 Register
#define REG_ADDR__HRXCTRL0                                               (Page_1 | 0x001B)
  // (ReadWrite, Bits 0) 
  // ri_stop_reset_command_during_idle_trying. If high, stop sending RESET command during retrying IDLE bus state. Default 1'b1
  #define BIT_MSK__HRXCTRL0__REG_HRX_STOP_RESET                                    0x01
  // (ReadWrite, Bits 1) 
  // ri_null_stuffing_in_hsic. Eanble automatic NULL character stuffing when USB data is not available. Default 1'b1.
  #define BIT_MSK__HRXCTRL0__REG_HRX_NULL_STUFFING                                 0x02
  // (ReadWrite, Bits 2) 
  // ri_extend_incomplete_eop. Automatically extend EOP bits if the length of EOP is less than 8 (for invalid HSIC packet with short EOP). Default 1'b1.
  #define BIT_MSK__HRXCTRL0__REG_HRX_EXTEND                                        0x04
  // (ReadWrite, Bits 3) 
  // ri_hsic_rx_assert_strobe_domain_reset. If high, assert reset at logic in strobe domain. Default 1'b0.
  #define BIT_MSK__HRXCTRL0__REG_HRX_ASSERT_RESET                                  0x08
  // (ReadWrite, Bits 7:4) 
  // ri_hsic_rx_en_margin[3:0]. Time between driving hsic bus and reactivating hsic_rx. Default 4'h3.
  #define BIT_MSK__HRXCTRL0__REG_HRX_MARGIN_B3_B0                                  0xF0

// HSIC RX Control1 Register
#define REG_ADDR__HRXCTRL1                                               (Page_1 | 0x001C)
  // (ReadWrite, Bits 3:0) 
  // ri_resume_detect[3:0]. If high, stop sending RESET command during retrying IDLE bus state. Default 4'h4
  #define BIT_MSK__HRXCTRL1__REG_HRX_RESUME                                        0x0F
  // (ReadWrite, Bits 7:4) 
  // ri_reset_detect[3:0]. If bus stays resume for a given cycles, send RESET symbol. (0=disable detection). Default 4'h4.
  #define BIT_MSK__HRXCTRL1__REG_HRX_RESET                                         0xF0

// HSIC RX Control2 Register
#define REG_ADDR__HRXCTRL2                                               (Page_1 | 0x001D)
  // (ReadWrite, Bits 7:0) 
  // ri_strobe_stay_threshold. If strobe toggle and stay for a given time, it is considered connect or reset bus state depending on ri_strobe_stay_then_reset. Default 8'h10.
  #define BIT_MSK__HRXCTRL2__REG_HRX_STAY_THRESH                                   0xFF

// HSIC RX Control3 Register
#define REG_ADDR__HRXCTRL3                                               (Page_1 | 0x001E)
  // (ReadWrite, Bits 0) 
  // ri_strobe_stay_then_reset.  If strobe toggle and stay for a given time, it is considered connect or reset bus state depending on ri_strobe_stay_then_reset. Default 1'b1.
  #define BIT_MSK__HRXCTRL3__REG_HRX_STAY_RESET                                    0x01
  // (ReadWrite, Bits 1) 
  // ri_status_en. Enable to latch status (ro_hsic_tx_status, ro_hsic_rx_status, ro_tdm_tx_status, ro_tdm_rx_status). Turn off to hold the status.
  #define BIT_MSK__HRXCTRL3__REG_STATUS_EN                                         0x02
  // (ReadWrite, Bits 2) 
  // ri_hsic_rx_out_en. If high, HSIC is forwarded to HSIC2TDM FIFO (default 1)
  #define BIT_MSK__HRXCTRL3__REG_HRX_OUT_EN                                        0x04
  // (ReadWrite, Bits 7:4) 
  // ri_hsic_rx_afifo_control[3:0]. [3]=force reset, [2:0]=error detect disable. Default 4'h0.
  #define BIT_MSK__HRXCTRL3__REG_HRX_AFFCTRL                                       0xF0

// HSIC RX Control4 Register
#define REG_ADDR__HRXCTRL4                                               (Page_1 | 0x001F)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_rx_buffer[7:0]. Size of buffering before reading out from afifo in order to maintain fifo level in middle range. Default 8'h3.
  #define BIT_MSK__HRXCTRL4__REG_HRX_BUFFER                                        0xFF

// HSIC RX Control5 Register
#define REG_ADDR__HRXCTRL5                                               (Page_1 | 0x0020)
  // (ReadWrite, Bits 0) 
  // ri_hsic_rx_separated_ebuffer_balign. Separated 500ppm elastic buffer and byte alignment. Default 1'b1.
  #define BIT_MSK__HRXCTRL5__REG_HRX_BALIGN                                        0x01
  // (ReadWrite, Bits 1) 
  // ri_hsic_rx_link_out_regardless_link_rdy. If high, link out without checking link is ready (TDD backup option). Default 1'b0.
  #define BIT_MSK__HRXCTRL5__REG_HRX_LINK_OUT                                      0x02
  // (ReadWrite, Bits 2) 
  // ri_tdd_enable. TDD mode enable (MHL 3+). Default 1'b0
  #define BIT_MSK__HRXCTRL5__REG_TDD_EN                                            0x04
  // (ReadWrite, Bits 3) 
  // ri_rwu_takeover_host. Remote WakeUp by host. Default 1'b1
  #define BIT_MSK__HRXCTRL5__REG_HRX_RWU_HOST                                      0x08
  // (ReadWrite, Bits 5:4) 
  // ri_usb_io_sel. 2'b00=HSIC(legacy), 2'b01=HSIC, 2'b10=DpDm, 2'b11=Hub. Default 2'b01
  #define BIT_MSK__HRXCTRL5__REG_USB_IO_SEL                                        0x30

// HSIC RX Write Backoff Register
#define REG_ADDR__HRXWRBKOF                                              (Page_1 | 0x0021)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_rx_write_backoff[7:0]. When hsic2tdm FIFO is overflowed, backoff FIFO to restore. (Default=8'h20, Disable=8'h00)
  #define BIT_MSK__HRXWRBKOF__REG_HRX_WRBKOF                                        0xFF

// HSIC RX Override Low Register
#define REG_ADDR__HRXOVRL                                                (Page_1 | 0x0022)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_rx_override_link_out[10:0]. Override outgoing symbols in HSIC slots (for debugging). [10]=override_enable,[9:0]=data (default 11'h0)
  #define BIT_MSK__HRXOVRL__REG_HRX_OVERRIDE_B7_B0                                0xFF

// HSIC RX Override High Register
#define REG_ADDR__HRXOVRH                                                (Page_1 | 0x0023)
  // (ReadWrite, Bits 2:0) 
  // ri_hsic_rx_override_link_out[10:0]. Override outgoing symbols in HSIC slots (for debugging). [10]=override_enable,[9:0]=data (default 11'h0)
  #define BIT_MSK__HRXOVRH__REG_HRX_OVERRIDE_B10_B8                               0x07

// HSIC RX Status 1st Register
#define REG_ADDR__HRXSTA1                                                (Page_1 | 0x0024)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: HSIC current state bit 3: hsic_rx_resetn bit 6-4: HSIC2TDM AFIFO waddr bit 7: HSIC AFIFO error
  #define BIT_MSK__HRXSTA1__REG_HRX_STATUS_B7_B0                                  0xFF

// HSIC RX Status 2nd Register
#define REG_ADDR__HRXSTA2                                                (Page_1 | 0x0025)
  // (ReadOnly, Bits 7:0) 
  // HSIC2TDM AFIFO wtags
  #define BIT_MSK__HRXSTA2__REG_HRX_STATUS_B15_B8                                 0xFF

// HSIC RX Status 3rd Register
#define REG_ADDR__HRXSTA3                                                (Page_1 | 0x0026)
  // (ReadOnly, Bits 7:0) 
  // Low byte of HSIC AFIFO rtags
  #define BIT_MSK__HRXSTA3__REG_HRX_STATUS_B23_B16                                0xFF

// HSIC RX Status 4th Register
#define REG_ADDR__HRXSTA4                                                (Page_1 | 0x0027)
  // (ReadOnly, Bits 7:0) 
  // High byte of HSIC AFIFO rtags
  #define BIT_MSK__HRXSTA4__REG_HRX_STATUS_B31_B24                                0xFF

// HSIC RX Status 5th Register
#define REG_ADDR__HRXSTA5                                                (Page_1 | 0x0028)
  // (ReadOnly, Bits 7:0) 
  // bit 3-0: HSIC AFIFO raddr bit 4: hsic_rx_data_pad_sync bit 5: hsic_rx_strobe_pad_sync bit 7-6: Link current state
  #define BIT_MSK__HRXSTA5__REG_HRX_STATUS_B39_B32                                0xFF

// HSIC RX Status 6th Register
#define REG_ADDR__HRXSTA6                                                (Page_1 | 0x0029)
  // (ReadOnly, Bits 7:0) 
  // link_tx_data value sent out to TDM link
  #define BIT_MSK__HRXSTA6__REG_HRX_STATUS_B47_B40                                0xFF

// HSIC RX Status 7th Register
#define REG_ADDR__HRXSTA7                                                (Page_1 | 0x002A)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__HRXSTA7__REG_HRX_STATUS_B55_B48                                0xFF

// HSIC RX Status 8th Register
#define REG_ADDR__HRXSTA8                                                (Page_1 | 0x002B)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__HRXSTA8__REG_HRX_STATUS_B63_B56                                0xFF

// HSIC TX Strobet EOP Register
#define REG_ADDR__HTXSTEOP                                               (Page_1 | 0x002C)
  // (ReadWrite, Bits 5:0) 
  // ri_strobet_after_eop[5:0]. hsic_tx_strobe_t value at the end of USB packet.  This drive more to make the bus state to IDLE after the end of packet.. 6'b111000=Drive 2 STROBE periods
  #define BIT_MSK__HTXSTEOP__REG_HTX_ST_AEOP_B5_B0                                 0x3F

// HSIC TX Datat EOP Register
#define REG_ADDR__HTXDTEOP                                               (Page_1 | 0x002D)
  // (ReadWrite, Bits 5:0) 
  // ri_datat_after_eop[5:0].hsic_tx_data_t value at the end of USB packet. This drive more to make the bus state to IDLE after the end of packet. 6'b111000=Drive 2 STROBE periods
  #define BIT_MSK__HTXDTEOP__REG_HTX_DT_AEOP_B5_B0                                 0x3F

// HSIC TX CRTL Register
#define REG_ADDR__HTXCTRL                                                (Page_1 | 0x002E)
  // (ReadWrite, Bits 0) 
  // ri_drive_reset_once. 0=keep driving during receiving RESET symbols(default) 1=drive once for backdoor
  #define BIT_MSK__HTXCTRL__REG_HTX_DRVRST1                                       0x01
  // (ReadWrite, Bits 1) 
  // ri_drive_conn_once. 0=keep driving during receiving CONN  symbols(default) 1=drive once for backdoor
  #define BIT_MSK__HTXCTRL__REG_HTX_DRVCONN1                                      0x02
  // (ReadWrite, Bits 2) 
  // ri_respect_tdm_even_if_rx_busy. Repsect received TDM symbols even if hsix_rx is not idle (0=host, 1=device). Default 1'b0.
  #define BIT_MSK__HTXCTRL__REG_HTX_RSPTDM_BUSY                                   0x04
  // (ReadWrite, Bits 3) 
  // ri_regard_invalid_symbol_as_usb. When receiving invalid symbol, regard this as USB packet. (default 0)
  #define BIT_MSK__HTXCTRL__REG_HTX_RGDINV_USB                                    0x08
  // (ReadWrite, Bits 4) 
  // ri_allow_single_bit_error_in_sop. Allow SOP byte 0x54 or 0xA8 instead of 0x55 or 0xAA (default 0)
  #define BIT_MSK__HTXCTRL__REG_HTX_ALLSBE_SOP                                    0x10
  // (ReadWrite, Bits 5) 
  // ri_allow_even_eop_at_lbyte. Allow even number of EOP bits for last byte. MHL spec viloation but allow for backup (default 0)
  #define BIT_MSK__HTXCTRL__REG_HTX_EVEN_EOP                                      0x20

// HSIC TX AFIFO Control Register
#define REG_ADDR__HTXAFFCTRL                                             (Page_1 | 0x002F)
  // (ReadWrite, Bits 7:4) 
  // ri_hsic_tx_afifo_control[3:0]. [3]=force reset, [2:0]=error detect disable. Default 4'h0.
  #define BIT_MSK__HTXAFFCTRL__REG_HTX_AFFCTRL_B3_B0                                 0xF0

// HSIC TX BUSIDLEP 1st Register
#define REG_ADDR__HTXBIDLEP0                                             (Page_1 | 0x0030)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_period[31:0]. When retrying IDLE bus state, period of driving IDLE bus state. Default 32'h80000
  #define BIT_MSK__HTXBIDLEP0__REG_HTX_BUSIDPRD_B7_B0                                0xFF

// HSIC TX BUSIDLEP 2nd Register
#define REG_ADDR__HTXBIDLEP1                                             (Page_1 | 0x0031)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_period[31:0]. When retrying IDLE bus state, period of driving IDLE bus state. Default 32'h80000
  #define BIT_MSK__HTXBIDLEP1__REG_HTX_BUSIDPRD_B15_B8                               0xFF

// HSIC TX BUSIDLEP 3rd Register
#define REG_ADDR__HTXBIDLEP2                                             (Page_1 | 0x0032)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_period[31:0]. When retrying IDLE bus state, period of driving IDLE bus state. Default 32'h80000
  #define BIT_MSK__HTXBIDLEP2__REG_HTX_BUSIDPRD_B23_B16                              0xFF

// HSIC TX BUSIDLEP 4th Register
#define REG_ADDR__HTXBIDLEP3                                             (Page_1 | 0x0033)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_period[31:0]. When retrying IDLE bus state, period of driving IDLE bus state. Default 32'h80000
  #define BIT_MSK__HTXBIDLEP3__REG_HTX_BUSIDPRD_B31_B24                              0xFF

// HSIC TX BUSIDLEC 1st Register
#define REG_ADDR__HTXBIDLEC0                                             (Page_1 | 0x0034)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_check[31:0]. When retrying IDLE bus state, check timing after driving IDLE bus state. Default 32'h10.
  #define BIT_MSK__HTXBIDLEC0__REG_HTX_BUSIDCHK_B7_B0                                0xFF

// HSIC TX BUSIDLEC 2nd Register
#define REG_ADDR__HTXBIDLEC1                                             (Page_1 | 0x0035)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_check[31:0]. When retrying IDLE bus state, check timing after driving IDLE bus state. Default 32'h10.
  #define BIT_MSK__HTXBIDLEC1__REG_HTX_BUSIDCHK_B15_B8                               0xFF

// HSIC TX BUSIDLEC 3rd Register
#define REG_ADDR__HTXBIDLEC2                                             (Page_1 | 0x0036)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_check[31:0]. When retrying IDLE bus state, check timing after driving IDLE bus state. Default 32'h10.
  #define BIT_MSK__HTXBIDLEC2__REG_HTX_BUSIDCHK_B23_B16                              0xFF

// HSIC TX BUSIDLEC 4th Register
#define REG_ADDR__HTXBIDLEC3                                             (Page_1 | 0x0037)
  // (ReadWrite, Bits 7:0) 
  // ri_bus_idle_check[31:0]. When retrying IDLE bus state, check timing after driving IDLE bus state. Default 32'h10.
  #define BIT_MSK__HTXBIDLEC3__REG_HTX_BUSIDCHK_B31_B24                              0xFF

// HSIC TX PRETOUT Low Register
#define REG_ADDR__HTXPRETOUTL                                            (Page_1 | 0x0038)
  // (ReadWrite, Bits 7:0) 
  // ri_tx_state_pre_timeout[15:0]. if state stays at pre for long time, state is forced to IDLE state (0=turn off) (should larger than 4). Default 16'h10.
  #define BIT_MSK__HTXPRETOUTL__REG_HTX_STPRETOUT_B7_B0                               0xFF

// HSIC TX PRETOUT High Register
#define REG_ADDR__HTXPRETOUTH                                            (Page_1 | 0x0039)
  // (ReadWrite, Bits 7:0) 
  // ri_tx_state_pre_timeout[15:0]. if state stays at pre for long time, state is forced to IDLE state (0=turn off) (should larger than 4). Default 16'h10.
  #define BIT_MSK__HTXPRETOUTH__REG_HTX_STPRETOUT_B15_B8                              0xFF

// HSIC TX BUF Register
#define REG_ADDR__HTXBUF                                                 (Page_1 | 0x003A)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_buffer[7:0]. Size of buffering before reading out from afifo in order to maintain fifo level in middle range. Default 8'h8.
  #define BIT_MSK__HTXBUF__REG_HTX_BUF_B7_B0                                     0xFF

// HSIC TX NNULLEOP Register
#define REG_ADDR__HTXNNEOP                                               (Page_1 | 0x003B)
  // (ReadWrite, Bits 7:0) 
  // ri_num_of_null_to_force_eop[7:0]. When a given number of NULL characters are received, force the end of packet. Default 8'h10
  #define BIT_MSK__HTXNNEOP__REG_HTX_NNFEOP                                        0xFF

// HSIC TX OVRHOUT Low Register
#define REG_ADDR__HTXOVRHOL                                              (Page_1 | 0x003C)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_override_hsic_out[9:0]. Override value to HSIC output pad. [9]=data_ovr[8]=strobe_ovr[7:6]=data_o[5:4]=data_t[3:2]=strobe_o[1:0]=strobe_t (default=0x033)
  #define BIT_MSK__HTXOVRHOL__REG_HTX_OVRHOUT_B7_B0                                 0xFF

// HSIC TX OVRHOUT High Register
#define REG_ADDR__HTXOVRHOH                                              (Page_1 | 0x003D)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_override_hsic_out[9:0]. Override value to HSIC output pad. [9]=data_ovr[8]=strobe_ovr[7:6]=data_o[5:4]=data_t[3:2]=strobe_o[1:0]=strobe_t (default=0x033)
  #define BIT_MSK__HTXOVRHOH__REG_HTX_OVRHOUT_B9_B8                                 0x03

// HSIC TX SYMSOP0 Low Register
#define REG_ADDR__TTXSSOP0L                                              (Page_1 | 0x003E)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_sop0[9:0]. Symbol code for SOP0. Default 10'h55.
  #define BIT_MSK__TTXSSOP0L__REG_HTX_SYMSOP0_B7_B0                                 0xFF

// HSIC TX SYMSOP0 High Register
#define REG_ADDR__TTXSSOP0H                                              (Page_1 | 0x003F)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_sop0[9:0]. Symbol code for SOP0. Default 10'h55.
  #define BIT_MSK__TTXSSOP0H__REG_HTX_SYMSOP0_B9_B8                                 0x03

// HSIC TX SYMSOP1 Low Register
#define REG_ADDR__TTXSSOP1L                                              (Page_1 | 0x0040)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_sop1[9:0]. Symbol code for SOP1. Default 10'hAA.
  #define BIT_MSK__TTXSSOP1L__REG_HTX_SYMSOP1_B7_B0                                 0xFF

// HSIC TX SYMSOP1 High Register
#define REG_ADDR__TTXSSOP1H                                              (Page_1 | 0x0041)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_sop1[9:0]. Symbol code for SOP1. Default 10'hAA.
  #define BIT_MSK__TTXSSOP1H__REG_HTX_SYMSOP1_B9_B8                                 0x03

// HSIC TX SYMEOP0 Low Register
#define REG_ADDR__TTXSEOP0L                                              (Page_1 | 0x0042)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_eop0[9:0]. Symbol code for EOP0. Default 10'h0
  #define BIT_MSK__TTXSEOP0L__REG_HTX_SYMEOP0_B7_B0                                 0xFF

// HSIC TX SYMEOP0 High Register
#define REG_ADDR__TTXSEOP0H                                              (Page_1 | 0x0043)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_eop0[9:0]. Symbol code for EOP0. Default 10'h0.
  #define BIT_MSK__TTXSEOP0H__REG_HTX_SYMEOP0_B9_B8                                 0x03

// HSIC TX SYMEOP1 Low Register
#define REG_ADDR__TTXSEOP1L                                              (Page_1 | 0x0044)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_eop1[9:0]. Symbol code for EOP1. Default 10'hFF.
  #define BIT_MSK__TTXSEOP1L__REG_HTX_SYMEOP1_B7_B0                                 0xFF

// HSIC TX SYMEOP1 High Register
#define REG_ADDR__TTXSEOP1H                                              (Page_1 | 0x0045)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_eop1[9:0]. Symbol code for EOP1. Default 10'hFF.
  #define BIT_MSK__TTXSEOP1H__REG_HTX_SYMEOP1_B9_B8                                 0x03

// HSIC TX SYMIDLE Low Register
#define REG_ADDR__TTXSIDLEL_USBT                                         (Page_1 | 0x0046)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_idle[9:0]. Symbol code for IDLE. Default 10'h0.
  #define BIT_MSK__TTXSIDLEL_USBT__REG_HTX_SYMIDLE_B7_B0                                 0xFF

// HSIC TX SYMIDLE High Register
#define REG_ADDR__TTXSIDLEH_USBT                                         (Page_1 | 0x0047)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_idle[9:0]. Symbol code for IDLE. Default 10'h0.
  #define BIT_MSK__TTXSIDLEH_USBT__REG_HTX_SYMIDLE_B9_B8                                 0x03

// HSIC TX SYMRST Low Register
#define REG_ADDR__TTXSRSTL                                               (Page_1 | 0x0048)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_reset[9:0]. Symbol code for RESET. Default 10'h93.
  #define BIT_MSK__TTXSRSTL__REG_HTX_SYMRST_B7_B0                                  0xFF

// HSIC TX SYMRST High Register
#define REG_ADDR__TTXSRSTH                                               (Page_1 | 0x0049)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_reset[9:0]. Symbol code for RESET. Default 10'h93.
  #define BIT_MSK__TTXSRSTH__REG_HTX_SYMRST_B9_B8                                  0x03

// HSIC TX SYMCONN Low Register
#define REG_ADDR__TTXSCONNL                                              (Page_1 | 0x004A)
  // (ReadWrite, Bits 7:0) 
  // ri_hsic_tx_symbol_conn[9:0]. Symbol code for CONN. Default 10'hC6.
  #define BIT_MSK__TTXSCONNL__REG_HTX_SYMCONN_B7_B0                                 0xFF

// HSIC TX SYMCONN High Register
#define REG_ADDR__TTXSCONNH                                              (Page_1 | 0x004B)
  // (ReadWrite, Bits 1:0) 
  // ri_hsic_tx_symbol_conn[9:0]. Symbol code for CONN. Default 10'hC6.
  #define BIT_MSK__TTXSCONNH__REG_HTX_SYMCONN_B9_B8                                 0x03

// HSIC TX Status 1st Register
#define REG_ADDR__HTXSTA1                                                (Page_1 | 0x004C)
  // (ReadOnly, Bits 7:0) 
  // TDM2HSIC AFIFO rtags
  #define BIT_MSK__HTXSTA1__REG_HTX_STATUS_B7_B0                                  0xFF

// HSIC TX Status 2nd Register
#define REG_ADDR__HTXSTA2                                                (Page_1 | 0x004D)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: TDM2HSIC AFIFO raddr bit 3: TDM2HSIC AFIFO error bit 7-4: HSIC current state
  #define BIT_MSK__HTXSTA2__REG_HTX_STATUS_B15_B8                                 0xFF

// HSIC TX Status 3rd Register
#define REG_ADDR__HTXSTA3                                                (Page_1 | 0x004E)
  // (ReadOnly, Bits 7:0) 
  // bit 0: HSIC Tx bus idle trying flag bit 7-1: reserved
  #define BIT_MSK__HTXSTA3__REG_HTX_STATUS_B23_B16                                0xFF

// HSIC TX Status 4th Register
#define REG_ADDR__HTXSTA4                                                (Page_1 | 0x004F)
  // (ReadOnly, Bits 7:0) 
  // hsic_tx driver status [31:30]=data_o,[29:28]=data_t,[27:26]=strobe_o,[25:24]=strobe_t
  #define BIT_MSK__HTXSTA4__REG_HTX_STATUS_B31_B24                                0xFF

// TDM TX Control Register
#define REG_ADDR__TTXCTRL                                                (Page_1 | 0x0050)
  // (ReadWrite, Bits 3:0) 
  // ri_tdm_tx_afifo_control[3:0]. [3]=force reset, [2:0]=error detect disable. Default 4'h0.
  #define BIT_MSK__TTXCTRL__REG_TTX_AFFCTRL_B3_B0                                 0x0F

// HSIC Keeper Register
#define REG_ADDR__KEEPER                                                 (Page_1 | 0x0051)
  // (ReadWrite, Bits 1:0) 
  // ri_keeper_mode[1:0]. Keeper control mode. 2'b00=tied low, 2'b01=tied high, 2'b10=automatic (low@reset), 2'b11=automatic (high@reset)
  #define BIT_MSK__KEEPER__REG_KEEPER_MODE_B1_B0                                 0x03

// HSIC Keeper StartEnd Register
#define REG_ADDR__KPSTEND                                                (Page_1 | 0x0052)
  // (ReadWrite, Bits 3:0) 
  // ri_keeper_start[3:0]. Number of half STROBE cycles to turn on keeper after detecting IDLE bus state. Valid only when ri_keeper_mode[1]=1'b1.
  #define BIT_MSK__KPSTEND__REG_KEEPER_START_B3_B0                                0x0F
  // (ReadWrite, Bits 7:4) 
  // ri_keeper_end[3:0]. Number of half STROBE cycles to turn off keeper after detecting non-IDLE bus state.  Valid only when ri_keeper_mode[1]=1'b1.
  #define BIT_MSK__KPSTEND__REG_KEEPER_END_B3_B0                                  0xF0

// HSIC Flow Control General Register
#define REG_ADDR__FCGC                                                   (Page_1 | 0x0053)
  // (ReadWrite, Bits 0) 
  // Flow control enable 0: Disable (DoC) 1: Enable (CoC)
  #define BIT_MSK__FCGC__REG_HSIC_FC_ENABLE                                    0x01
  // (ReadWrite, Bits 1) 
  // USB host mode 0: device mode 1: host mode  Typical configurations Set to 0x03 if Titan is attached with USB host (AP) using CoC Set to 0x02 if Titan is attached with USB host (AP) using DoC Set to 0x01 if Titan (usually Rogue) is attached with HSIC Hub (Dock) using CoC Set to 0x00 if Titan (usually Rogue) is attached with HSIC Hub (Dock) using DoC
  #define BIT_MSK__FCGC__REG_HSIC_FC_HOSTMODE                                  0x02
  // (ReadWrite, Bits 3:2) 
  // ri_hsic_fc_filter_dev_ctrl. Filtering device control: [1]=allow(1),block(0), [0]=filter enable (Default 2'h0)
  #define BIT_MSK__FCGC__REG_HSIC_FC_FDEVCTRL                                  0x0C

// HSIC Flow Control CTR0 Register
#define REG_ADDR__FCCTR0                                                 (Page_1 | 0x0054)
  // (ReadWrite, Bits 7:0) 
  // bit3-0: hsic2fc_fifo control [3]=force reset, [2:0]=error detect disable bit7-4: fc2link_fifo control [3]=force reset, [2:0]=error detect disable  Internal use for debugging. Use the default value 0x00 for normal operation
  #define BIT_MSK__FCCTR0__REG_HFC_CONF0                                         0xFF

// HSIC Flow Control CTR1 Register
#define REG_ADDR__FCCTR1                                                 (Page_1 | 0x0055)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: link2fc_fifo control [3]=force reset, [2:0]=error detect disable bit 7-4: fc2hsic_fifo control [3]=force reset, [2:0]=error detect disable
  #define BIT_MSK__FCCTR1__REG_HFC_CONF1                                         0xFF

// HSIC Flow Control CTR2 Register
#define REG_ADDR__FCCTR2                                                 (Page_1 | 0x0056)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: hsic2mem_fc fifo control [3]=force reset, [2:0]=error detect disable bit 7-4: link2mem_fc fifo control [3]=force reset, [2:0]=error detect disable
  #define BIT_MSK__FCCTR2__REG_HFC_CONF2                                         0xFF

// HSIC Flow Control CTR3 Register
#define REG_ADDR__FCCTR3                                                 (Page_1 | 0x0057)
  // (ReadWrite, Bits 7:0) 
  // Bit 3-0: mem2link_fc fifo control [3]=force reset, [2:0]=error detect disable bit 4: hsic2mem ri_drop_data_with_crc16: Drop data packet if CRC16 error (Default=1'b1) (ES1.0) bit 5: hsic2mem/link2mem: ri_use_last_byte: Pkt analyzer uses last byte of destuff queue for destuffed EOP detection for CRC16 (Default=1'b1) (ES1.0) bit 6: hsic2mem ri_drop_data_with_destuffed_eop_err: Drop data packet if destuff EOP error (Default 1'b1) bit 7: rsvd 
  #define BIT_MSK__FCCTR3__REG_HFC_CONF3                                         0xFF

// HSIC Flow Control CTR4 Register
#define REG_ADDR__FCCTR4                                                 (Page_1 | 0x0058)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: hsic2mem ri_mem_state_wait_timeout: If state stays at wait for long time, state is forced to IDLE state  (Default=4'hF, Disable=4'h0) bit 5-4: hsic2mem ri_hsic2mem_cen_ctl: Memory write chip enable control: 2'bX0:cen=wen(default), 2'b10:cen low, 2'b11:cen high bit 6: hsic2mem ri_mask_sof_crc_intr bit 7: hsic2mem ri_mem_state_reset_if_pkt_val_low
  #define BIT_MSK__FCCTR4__REG_HFC_CONF4                                         0xFF

// HSIC Flow Control CTR5 Register
#define REG_ADDR__FCCTR5                                                 (Page_1 | 0x0059)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: mem2link ri_write_backoff: When FIFO is overflowed, backoff FIFO to restore. (Default=4'h0, Disable=4'h0) bit 7-4: mem2link ri_blank_after_packet: Insert black symbols after paacket (4'h0=mem2hsic, 4'h8=mem2link)
  #define BIT_MSK__FCCTR5__REG_HFC_CONF5                                         0xFF

// HSIC Flow Control CTR6 Register
#define REG_ADDR__FCCTR6                                                 (Page_1 | 0x005A)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: hsic2mem_rdctl: Give number of cycles for async to buffer USB bytes to prevent hole (default=3) bit 4: mem2link ri_pkt_generator_hsic_wvalid_check (If low, backward compatible with ES0.0) bit 5: Enable asynchronous reset assertion for hsic/TDM logic inside of flow_control_top (If low, backward compatible with ES0.0 bit 7-6: reserved
  #define BIT_MSK__FCCTR6__REG_HFC_CONF6                                         0xFF

// HSIC Flow Control CTR7 Register
#define REG_ADDR__FCCTR7                                                 (Page_1 | 0x005B)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR7__REG_HFC_CONF7                                         0xFF

// HSIC Flow Control CTR8 Register
#define REG_ADDR__FCCTR8                                                 (Page_1 | 0x005C)
  // (ReadWrite, Bits 7:0) 
  // bit 3-0: link2mem ri_mem_state_wait_timeout: If state stays at wait for long time, state is forced to IDLE state  (Default=4'hF, Disable=4'h0) bit 5-4: link2mem ri_hsic2mem_cen_ctl: Memory write chip enable control: 2'bX0:cen=wen(default), 2'b10:cen low, 2'b11:cen high bit 6: link2mem ri_mask_sof_crc_intr bit 7: link2mem ri_mem_state_reset_if_pkt_val_low
  #define BIT_MSK__FCCTR8__REG_HFC_CONF8                                         0xFF

// HSIC Flow Control CTR9 Register
#define REG_ADDR__FCCTR9                                                 (Page_1 | 0x005D)
  // (ReadWrite, Bits 7:0) 
  // bit3-0: mem2hsic ri_write_backoff: When FIFO is overflowed, backoff FIFO to restore. (Default=4'h8, Disable=4'h0) bit7-4: mem2hsic ri_blank_after_packet: Insert black symbols after paacket (4'h0=mem2hsic, 4'h8=mem2link)
  #define BIT_MSK__FCCTR9__REG_HFC_CONF9                                         0xFF

// HSIC Flow Control CTR10 Register
#define REG_ADDR__FCCTR10                                                (Page_1 | 0x005E)
  // (ReadWrite, Bits 7:0) 
  // bit 0: mem2hsic ri_pkt_generator_hsic_wvalid_check (If low, backward compatible with ES0.0) bit 7-1: rsvd
  #define BIT_MSK__FCCTR10__REG_HFC_CONF10                                        0xFF

// HSIC Flow Control CTR11 Register
#define REG_ADDR__FCCTR11                                                (Page_1 | 0x005F)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR11__REG_HFC_CONF11                                        0xFF

// HSIC Flow Control CTR12 Register
#define REG_ADDR__FCCTR12                                                (Page_1 | 0x0060)
  // (ReadWrite, Bits 7:0) 
  // bit 0: ri_sof_gen_enable: 1=SOF generation after receiving SOF from host(default), 0=SOF bypass bit 1: ri_sof_gen_force: 1=SOF generation regardless of SOF from host (ri_soof_gen_enable=1), 0=force off (default) bit 2: ri_ack_outep0: Send ACK for OUT EP0 (3-stage CONTROL transfer) (default 0=off) (SET-DATA-ACK, IN-DATA-ACK, OUT-DATA-ACK) bit 3: ri_split_enable: Enable SPLIT transaction bit 4: clear_outst_if_setup: Clear outstanding if SETUP packet is received and outst response received (l2m_rvalid) bit 5: turnaround_eop2eop_timeout_if_not_link2mem_wvalid: enable outst timeout only when link2mem_wvalid is low bit 6: clear_outst_if_nak_received: Clear outstanding if NAK is received (default=1) bit 7: sof_highest_priority: 1=SOF is highest priority Default=1'b1
  #define BIT_MSK__FCCTR12__REG_HFC_CONF12                                        0xFF

// HSIC Flow Control CTR13 Register
#define REG_ADDR__FCCTR13                                                (Page_1 | 0x0061)
  // (ReadWrite, Bits 7:0) 
  // bit 0: respond_ack_for_all_ping: 0=disable(default) 1=always send ACK for PING bit 1: nonzero_endp_is_interrupt_for_split: 1=Assume nonzero endpoint is interrupt endpoint for SPLIT transactions 0=disable (default=0) bit 2: clear_outst_if_nyet_received_for_csplit: Clear outstanding if NYET is received for CSPLIT transaction (default=1) bit 3: sof_frame_match: Enable to match frame number between host and device at once (Default=1'b1) bit 4: Update SOF frame number without delay (Default=1'b1) bit 5: Send NAK instead of NYET for interrupt CSPLIT during outstanding (Default=1'b1) bit 6: ri_multiple_outst: support multiple outstandin (Default=1'b1) bit 7: ri_nyet_for_csplit_ta_timeout: Device only: Rogue sends NYET to Titan if turnaround timeout occurs (Default=1'b1)
  #define BIT_MSK__FCCTR13__REG_HFC_CONF13                                        0xFF

// HSIC Flow Control CTR14 Register
#define REG_ADDR__FCCTR14                                                (Page_1 | 0x0062)
  // (ReadWrite, Bits 7:0) 
  // bit 0: ri_deliver_every_resp_to_host: Device only: Rogue forward every resp (including ERR) 1'b1=Default, 1'b0=ES0.0 compatible bit 1: ri_es10_fix: 0=ES0.0 backward compatible mode (Default=1'b1) If high, override ri_outst_nakced_knockedout bit 2: ri_forward_setup_wo_outst: 1=Forward SETUP packets regardless of outstanding (default) 0=ES0.0 bit 3: ri_outst_timed_knockout: Host only: Clear too long outst transactions (Default 1'b1) If high, override ri_outst_nakced_knockedout bit 4: ri_turnaround_eof_timeout: Device only: Force timeout during waiting for turnaround at the end of frame (default 1) bit 5: ri_nak_for_in_ta_timeout: Device only: Rogue sends NAK to Titan if turnaround timeout occurs (Default=1'b1) bit 6: ri_nak_for_else_ta_timeout: Device only: Rogue sends NAK to Titan if turnaround timeout occurs (Default=1'b1) bit 7: ri_wait_for_ack_sent_for_in: Device only: Rogue wait until ACK is done with transmission after IN-DATA (default=1'b1) 
  #define BIT_MSK__FCCTR14__REG_HFC_CONF14                                        0xFF

// HSIC Flow Control CTR15 Register
#define REG_ADDR__FCCTR15                                                (Page_1 | 0x0063)
  // (ReadWrite, Bits 7:0) 
  // bit 0: ri_ssplit_out_intr_wo_resp_outst: If high, does not set(do check) outst nor respond NAK for interrupt SSPLIT-OUT-DATA (default=1'b1) bit 1: ri_ssplit_out_isoc_wo_resp_outst: If high, does not set/   check  outst nor respond NAK for isochronous SSPLIT-OUT-DATA (default=1'b1) bit 2: ri_csplit_in_isoc_forward_if_mdata: If high, forward CSPLIT-IN if outst and MDATA is in link2mem (default=1'b1) bit 3: ri_link2mem_rvalid_both_high_fix: clear outst_pdc when link2mem_pdc is filled by NYET while both link2mem_rvalid and link2mem_pdc_rvalid are high (default=1'b1) bit 4: ri_match_pid_for_out_resp: Host only: When OUT is received, check if PID is matched with outst_pid. bit 5: ri_disable_link2hsic_mem_overflow_intr: If high, disable link2hsic memory overflow interrupt (to prevent false alarm). If low, enable the interrupt. bit 6: ri_nyet_for_isoc_csplit_if_outst: If high, send NYET instead of NAK for isochronous CSPLIT during outstanding (Default=1'b1) ES1.0 bit 7: ri_dedicated_ta_timeout_data: Dedicated turnaround timeout upon DATA packet (intr_ta_timeout_data) (default 1'b1) 
  #define BIT_MSK__FCCTR15__REG_HFC_CONF15                                        0xFF

// HSIC Flow Control CTR16 Register
#define REG_ADDR__FCCTR16                                                (Page_1 | 0x0064)
  // (ReadWrite, Bits 7:0) 
  // bit 7-0: ri_turnaround_handshake_timeout: Host/Device: Timeout not occur any earlier than 736 HS bit times + Handshake packet size (96BT) (eop to eop)(default 8'h73=8'd115=(736+96)/8+margin 11) //Additional 1BT cnt overhead
  #define BIT_MSK__FCCTR16__REG_HFC_CONF16                                        0xFF

// HSIC Flow Control CTR17 Register
#define REG_ADDR__FCCTR17                                                (Page_1 | 0x0065)
  // (ReadWrite, Bits 7:0) 
  // Low Byte for ri_turnaround_data_timeout: Device only: Timeout not occur any earlier than 736 HS bit times + data packet size (4096BT) (eop to eop)(default 16'h051E=8'd1310=(736+9644)/8+margin 12) //Additional 1BT cnt overhead
  #define BIT_MSK__FCCTR17__REG_HFC_CONF17                                        0xFF

// HSIC Flow Control CTR18 Register
#define REG_ADDR__FCCTR18                                                (Page_1 | 0x0066)
  // (ReadWrite, Bits 7:0) 
  // High Byte for ri_turnaround_data_timeout: Device only: Timeout not occur any earlier than 736 HS bit times + data packet size (4096BT) (eop to eop)(default 16'h051E=8'd1310=(736+9644)/8+margin 12) //Additional 1BT cnt overhead
  #define BIT_MSK__FCCTR18__REG_HFC_CONF18                                        0xFF

// HSIC Flow Control CTR19 Register
#define REG_ADDR__FCCTR19                                                (Page_1 | 0x0067)
  // (ReadWrite, Bits 7:0) 
  // ri_interpacket_delay: Minimum of 88-bit times, maximum 192-bit times (default 8'd9=88/8-2)(eop to sop)//Additional 1BT cnt overhead
  #define BIT_MSK__FCCTR19__REG_HFC_CONF19                                        0xFF

// HSIC Flow Control CTR20 Register
#define REG_ADDR__FCCTR20                                                (Page_1 | 0x0068)
  // (ReadWrite, Bits 7:0) 
  // First Byte of ri_turnaround_eop2eop_timeout: Host only: turnaround Max(736 BT) plus TA time over MHL((token96+data9644)*8BT) (eop to eop) (default 16'd9832=16'h2668 (736+(96+9644)*8)/8 (0=disable timeout)
  #define BIT_MSK__FCCTR20__REG_HFC_CONF20                                        0xFF

// HSIC Flow Control CTR21 Register
#define REG_ADDR__FCCTR21                                                (Page_1 | 0x0069)
  // (ReadWrite, Bits 7:0) 
  // Second Byte of ri_turnaround_eop2eop_timeout: Host only: turnaround Max(736 BT) plus TA time over MHL((token96+data9644)*8BT) (eop to eop) (default 16'd9832=16'h2668 (736+(96+9644)*8)/8 (0=disable timeout)
  #define BIT_MSK__FCCTR21__REG_HFC_CONF21                                        0xFF

// HSIC Flow Control CTR22 Register
#define REG_ADDR__FCCTR22                                                (Page_1 | 0x006A)
  // (ReadWrite, Bits 7:0) 
  // Third Byte of ri_turnaround_eop2eop_timeout: Host only: turnaround Max(736 BT) plus TA time over MHL((token96+data9644)*8BT) (eop to eop) (default 16'd9832=16'h2668 (736+(96+9644)*8)/8 (0=disable timeout)
  #define BIT_MSK__FCCTR22__REG_HFC_CONF22                                        0xFF

// HSIC Flow Control CTR23 Register
#define REG_ADDR__FCCTR23                                                (Page_1 | 0x006B)
  // (ReadWrite, Bits 7:0) 
  // Fourth Byte of ri_turnaround_eop2eop_timeout: Host only: turnaround Max(736 BT) plus TA time over MHL((token96+data9644)*8BT) (eop to eop) (default 16'd9832=16'h2668 (736+(96+9644)*8)/8 (0=disable timeout)
  #define BIT_MSK__FCCTR23__REG_HFC_CONF23                                        0xFF

// HSIC Flow Control CTR24 Register
#define REG_ADDR__FCCTR24                                                (Page_1 | 0x006C)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of ri_interpacket_eop2eop_timeout: Host only: Inter-packet Max(192BT) plus packet length (9644BT) (eop to eop)(default 16'h04E2=16'd1250 (192BT+9644)/8+21(margin)) (Default(device) 16'h25DA=16'd9690(192+9644*8)/8+22)
  #define BIT_MSK__FCCTR24__REG_HFC_CONF24                                        0xFF

// HSIC Flow Control CTR25 Register
#define REG_ADDR__FCCTR25                                                (Page_1 | 0x006D)
  // (ReadWrite, Bits 7:0) 
  // High Byte of ri_interpacket_eop2eop_timeout: Host only: Inter-packet Max(192BT) plus packet length (9644BT) (eop to eop)(default 16'h04E2=16'd1250 (192BT+9644)/8+21(margin)) (Default(device) 16'h25DA=16'd9690(192+9644*8)/8+22)
  #define BIT_MSK__FCCTR25__REG_HFC_CONF25                                        0xFF

// HSIC Flow Control CTR26 Register
#define REG_ADDR__FCCTR26                                                (Page_1 | 0x006E)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of ri_sof_interval: Device only: SOF interval 60000BT/8=0x1D4C. (default 0x1D4C)
  #define BIT_MSK__FCCTR26__REG_HFC_CONF26                                        0xFF

// HSIC Flow Control CTR27 Register
#define REG_ADDR__FCCTR27                                                (Page_1 | 0x006F)
  // (ReadWrite, Bits 7:0) 
  // High Byte of ri_sof_interval: Device only: SOF interval 60000BT/8=0x1D4C. (default 0x1D4C)
  #define BIT_MSK__FCCTR27__REG_HFC_CONF27                                        0xFF

// HSIC Flow Control CTR28 Register
#define REG_ADDR__FCCTR28                                                (Page_1 | 0x0070)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of ri_sof_timeout: Device only: SOF interval 60000BT/8=0x1D4C. It should more than 0x1D4C (default 2uF=0x3A98, 3uF=57E4=0x1D4C*3) 
  #define BIT_MSK__FCCTR28__REG_HFC_CONF28                                        0xFF

// HSIC Flow Control CTR29 Register
#define REG_ADDR__FCCTR29                                                (Page_1 | 0x0071)
  // (ReadWrite, Bits 7:0) 
  // High Byte of ri_sof_timeout: Device only: SOF interval 60000BT/8=0x1D4C. It should more than 0x1D4C (default 2uF=0x3A98, 3uF=57E4=0x1D4C*3) 
  #define BIT_MSK__FCCTR29__REG_HFC_CONF29                                        0xFF

// HSIC Flow Control CTR30 Register
#define REG_ADDR__FCCTR30                                                (Page_1 | 0x0072)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of ri_sof_keepout: Keepout BT before SOF (default 16'h0400=16'd1024=8192/8)
  #define BIT_MSK__FCCTR30__REG_HFC_CONF30                                        0xFF

// HSIC Flow Control CTR31 Register
#define REG_ADDR__FCCTR31                                                (Page_1 | 0x0073)
  // (ReadWrite, Bits 7:0) 
  // High Byte of ri_sof_keepout: Keepout BT before SOF (default 16'h0400=16'd1024=8192/8)
  #define BIT_MSK__FCCTR31__REG_HFC_CONF31                                        0xFF

// HSIC Flow Control CTR32 Register
#define REG_ADDR__FCCTR32                                                (Page_1 | 0x0074)
  // (ReadWrite, Bits 7:0) 
  // First Byte of outst_nacked_knockout: Host only: Too many NAKed transactions (not matched addr/endp) default=32'h0, 1sec=32'h2B594D=10^6nsec/(88*2*2nsec). If ri_oustst_timed_knock high (ES1.0), [31:18]=SPLIT interrupt timed knockout [17:0] other transaction timed knockout (default value 32'h40010000)
  #define BIT_MSK__FCCTR32__REG_HFC_CONF32                                        0xFF

// HSIC Flow Control CTR33 Register
#define REG_ADDR__FCCTR33                                                (Page_1 | 0x0075)
  // (ReadWrite, Bits 7:0) 
  // Second Byte of outst_nacked_knockout: Host only: Too many NAKed transactions (not matched addr/endp) default=32'h0, 1sec=32'h2B594D=10^6nsec/(88*2*2nsec). If ri_oustst_timed_knock high (ES1.0), [31:18]=SPLIT interrupt timed knockout [17:0] other transaction timed knockout (default value 32'h40010000)
  #define BIT_MSK__FCCTR33__REG_HFC_CONF33                                        0xFF

// HSIC Flow Control CTR34 Register
#define REG_ADDR__FCCTR34                                                (Page_1 | 0x0076)
  // (ReadWrite, Bits 7:0) 
  // Third Byte of outst_nacked_knockout: Host only: Too many NAKed transactions (not matched addr/endp) default=32'h0, 1sec=32'h2B594D=10^6nsec/(88*2*2nsec). If ri_oustst_timed_knock high (ES1.0), [31:18]=SPLIT interrupt timed knockout [17:0] other transaction timed knockout (default value 32'h40010000)
  #define BIT_MSK__FCCTR34__REG_HFC_CONF34                                        0xFF

// HSIC Flow Control CTR35 Register
#define REG_ADDR__FCCTR35                                                (Page_1 | 0x0077)
  // (ReadWrite, Bits 7:0) 
  // Last Byte of outst_nacked_knockout: Host only: Too many NAKed transactions (not matched addr/endp) default=32'h0, 1sec=32'h2B594D=10^6nsec/(88*2*2nsec). If ri_oustst_timed_knock high (ES1.0), [31:18]=SPLIT interrupt timed knockout [17:0] other transaction timed knockout (default value 32'h40010000)
  #define BIT_MSK__FCCTR35__REG_HFC_CONF35                                        0xFF

// HSIC Flow Control CTR36 Register
#define REG_ADDR__FCCTR36                                                (Page_1 | 0x0078)
  // (ReadWrite, Bits 7:0) 
  // First Byte of sof_periodgen: [5]=en_phase[4]=good_only[3:1]=init_window[0]=enable
  #define BIT_MSK__FCCTR36__REG_HFC_CONF36                                        0xFF

// HSIC Flow Control CTR37 Register
#define REG_ADDR__FCCTR37                                                (Page_1 | 0x0079)
  // (ReadWrite, Bits 7:0) 
  // Second Byte of sof_periodgen: [5]=en_phase[4]=good_only[3:1]=init_window[0]=enable
  #define BIT_MSK__FCCTR37__REG_HFC_CONF37                                        0xFF

// HSIC Flow Control CTR38 Register
#define REG_ADDR__FCCTR38                                                (Page_1 | 0x007A)
  // (ReadWrite, Bits 7:0) 
  // Third Byte of sof_periodgen: [5]=en_phase[4]=good_only[3:1]=init_window[0]=enable
  #define BIT_MSK__FCCTR38__REG_HFC_CONF38                                        0xFF

// HSIC Flow Control CTR39 Register
#define REG_ADDR__FCCTR39                                                (Page_1 | 0x007B)
  // (ReadWrite, Bits 7:0) 
  // Last Byte of sof_periodgen: [5]=en_phase[4]=good_only[3:1]=init_window[0]=enable
  #define BIT_MSK__FCCTR39__REG_HFC_CONF39                                        0xFF

// HSIC Flow Control CTR40 Register
#define REG_ADDR__FCCTR40                                                (Page_1 | 0x007C)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of sof_periodgen_threshold
  #define BIT_MSK__FCCTR40__REG_HFC_CONF40                                        0xFF

// HSIC Flow Control CTR41 Register
#define REG_ADDR__FCCTR41                                                (Page_1 | 0x007D)
  // (ReadWrite, Bits 7:0) 
  // High Byte of sof_periodgen_threshold
  #define BIT_MSK__FCCTR41__REG_HFC_CONF41                                        0xFF

// HSIC Flow Control CTR42 Register
#define REG_ADDR__FCCTR42                                                (Page_1 | 0x007E)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of sof_phase_track: [9:8]=interval offset,[7]=adjusted_interval,[6:4]=phase offset,[3:1]=threshold [0]=enable
  #define BIT_MSK__FCCTR42__REG_HFC_CONF42                                        0xFF

// HSIC Flow Control CTR43 Register
#define REG_ADDR__FCCTR43                                                (Page_1 | 0x007F)
  // (ReadWrite, Bits 7:0) 
  // High Byte of sof_phase_track: [9:8]=interval offset,[7]=adjusted_interval,[6:4]=phase offset,[3:1]=threshold [0]=enable
  #define BIT_MSK__FCCTR43__REG_HFC_CONF43                                        0xFF

// HSIC Flow Control CTR44 Register
#define REG_ADDR__FCCTR44                                                (Page_1 | 0x0080)
  // (ReadWrite, Bits 7:0) 
  // Low Byte of intr_snapshot_enable: When high, snapshot status when corresponding intr occurs. Default=16'h0
  #define BIT_MSK__FCCTR44__REG_HFC_CONF44                                        0xFF

// HSIC Flow Control CTR45 Register
#define REG_ADDR__FCCTR45                                                (Page_1 | 0x0081)
  // (ReadWrite, Bits 7:0) 
  // High Byte of intr_snapshot_enable: When high, snapshot status when corresponding intr occurs. Default=16'h0
  #define BIT_MSK__FCCTR45__REG_HFC_CONF45                                        0xFF

// HSIC Flow Control CTR46 Register
#define REG_ADDR__FCCTR46                                                (Page_1 | 0x0082)
  // (ReadWrite, Bits 7:0) 
  // Low byte of ri_sof_keepout_front: Keepout BT after SOF (default 16'h0050=16'd80=1280nsec/2/8)
  #define BIT_MSK__FCCTR46__REG_HFC_CONF46                                        0xFF

// HSIC Flow Control CTR47 Register
#define REG_ADDR__FCCTR47                                                (Page_1 | 0x0083)
  // (ReadWrite, Bits 7:0) 
  // High byte of ri_sof_keepout_front: Keepout BT after SOF (default 16'h0050=16'd80=1280nsec/2/8)
  #define BIT_MSK__FCCTR47__REG_HFC_CONF47                                        0xFF

// HSIC Flow Control CTR48 Register
#define REG_ADDR__FCCTR48                                                (Page_1 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // Low byte of ri_sof_keepout_short: Keepout BT before SOF (short version) for SPLIT,SETUP,IN/OUT(EP=0) (default 16'h0100=16'd256)
  #define BIT_MSK__FCCTR48__REG_HFC_CONF48                                        0xFF

// HSIC Flow Control CTR49 Register
#define REG_ADDR__FCCTR49                                                (Page_1 | 0x0085)
  // (ReadWrite, Bits 7:0) 
  // High byte of ri_sof_keepout_short: Keepout BT before SOF (short version) for SPLIT,SETUP,IN/OUT(EP=0) (default 16'h0100=16'd256)
  #define BIT_MSK__FCCTR49__REG_HFC_CONF49                                        0xFF

// HSIC Flow Control CTR50 Register
#define REG_ADDR__FCCTR50                                                (Page_1 | 0x0086)
  // (ReadWrite, Bits 7:0) 
  // bit 0: ri_keep_outst_if_timeout_for_in_data: Host only: Keep outst and l2m if host return timeout for IN-DATA  (Default 1'b1) bit 1: Allow how many times of retrying IN-DATA-ACK. If reached, clear outstanding. 0=retry forever (until knockout), 1=retry once (Default 1'b1) bit 7-2: rsvd 
  #define BIT_MSK__FCCTR50__REG_HFC_CONF50                                        0xFF

// HSIC Flow Control CTR51 Register
#define REG_ADDR__FCCTR51                                                (Page_1 | 0x0087)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR51__REG_HFC_CONF51                                        0xFF

// HSIC Flow Control CTR52 Register
#define REG_ADDR__FCCTR52                                                (Page_1 | 0x0088)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR52__REG_HFC_CONF52                                        0xFF

// HSIC Flow Control CTR53 Register
#define REG_ADDR__FCCTR53                                                (Page_1 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR53__REG_HFC_CONF53                                        0xFF

// HSIC Flow Control CTR54 Register
#define REG_ADDR__FCCTR54                                                (Page_1 | 0x008A)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR54__REG_HFC_CONF54                                        0xFF

// HSIC Flow Control CTR55 Register
#define REG_ADDR__FCCTR55                                                (Page_1 | 0x008B)
  // (ReadWrite, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCCTR55__REG_HFC_CONF55                                        0xFF

// HSIC Flow Control STAT0 Register
#define REG_ADDR__FCSTAT0                                                (Page_1 | 0x008C)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic2mem mem_state current state bit 14-3: hsic2mem mem_addr bit 17-15: hsic2mem fc_afifo_waddr bit 25-18: hsic2mem fc_afifo_wtags bit 26: hsic2mem hsic_pkt_val bit 27: hsic2mem reserved bit 31-28: hsic2mem CRC16 good count
  #define BIT_MSK__FCSTAT0__REG_HFC_STAT0                                         0xFF

// HSIC Flow Control STAT1 Register
#define REG_ADDR__FCSTAT1                                                (Page_1 | 0x008D)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic2mem mem_state current state bit 14-3: hsic2mem mem_addr bit 17-15: hsic2mem fc_afifo_waddr bit 25-18: hsic2mem fc_afifo_wtags bit 26: hsic2mem hsic_pkt_val bit 27: hsic2mem reserved bit 31-28: hsic2mem CRC16 good count
  #define BIT_MSK__FCSTAT1__REG_HFC_STAT1                                         0xFF

// HSIC Flow Control STAT2 Register
#define REG_ADDR__FCSTAT2                                                (Page_1 | 0x008E)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic2mem mem_state current state bit 14-3: hsic2mem mem_addr bit 17-15: hsic2mem fc_afifo_waddr bit 25-18: hsic2mem fc_afifo_wtags bit 26: hsic2mem hsic_pkt_val bit 27: hsic2mem reserved bit 31-28: hsic2mem CRC16 good count
  #define BIT_MSK__FCSTAT2__REG_HFC_STAT2                                         0xFF

// HSIC Flow Control STAT3 Register
#define REG_ADDR__FCSTAT3                                                (Page_1 | 0x008F)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic2mem mem_state current state bit 14-3: hsic2mem mem_addr bit 17-15: hsic2mem fc_afifo_waddr bit 25-18: hsic2mem fc_afifo_wtags bit 26: hsic2mem hsic_pkt_val bit 27: hsic2mem reserved bit 31-28: hsic2mem CRC16 good count
  #define BIT_MSK__FCSTAT3__REG_HFC_STAT3                                         0xFF

// HSIC Flow Control STAT4 Register
#define REG_ADDR__FCSTAT4                                                (Page_1 | 0x0090)
  // (ReadOnly, Bits 7:0) 
  // hsic2mem_rdctl rtags
  #define BIT_MSK__FCSTAT4__REG_HFC_STAT4                                         0xFF

// HSIC Flow Control STAT5 Register
#define REG_ADDR__FCSTAT5                                                (Page_1 | 0x0091)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic2mem_rdctl raddr bit 3: hsic2mem_rdctl error bit 7-4: rdctl reserved
  #define BIT_MSK__FCSTAT5__REG_HFC_STAT5                                         0xFF

// HSIC Flow Control STAT6 Register
#define REG_ADDR__FCSTAT6                                                (Page_1 | 0x0092)
  // (ReadOnly, Bits 7:0) 
  // rdctl reserved
  #define BIT_MSK__FCSTAT6__REG_HFC_STAT6                                         0xFF

// HSIC Flow Control STAT7 Register
#define REG_ADDR__FCSTAT7                                                (Page_1 | 0x0093)
  // (ReadOnly, Bits 7:0) 
  // rdctl reserved
  #define BIT_MSK__FCSTAT7__REG_HFC_STAT7                                         0xFF

// HSIC Flow Control STAT8 Register
#define REG_ADDR__FCSTAT8                                                (Page_1 | 0x0094)
  // (ReadOnly, Bits 7:0) 
  // mem2link hsic_wtags
  #define BIT_MSK__FCSTAT8__REG_HFC_STAT8                                         0xFF

// HSIC Flow Control STAT9 Register
#define REG_ADDR__FCSTAT9                                                (Page_1 | 0x0095)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: mem2link hsic_waddr bit 4-3: mem2link hsic_state bit 7-5: mem2link state
  #define BIT_MSK__FCSTAT9__REG_HFC_STAT9                                         0xFF

// HSIC Flow Control STAT10 Register
#define REG_ADDR__FCSTAT10                                               (Page_1 | 0x0096)
  // (ReadOnly, Bits 7:0) 
  // bit 7-0: 8 lsbs of mem2link mem_addr
  #define BIT_MSK__FCSTAT10__REG_HFC_STAT10                                        0xFF

// HSIC Flow Control STAT11 Register
#define REG_ADDR__FCSTAT11                                               (Page_1 | 0x0097)
  // (ReadOnly, Bits 7:0) 
  // bit 3-0: 4msbs mem2link mem_addr bit 7-4: rsvd
  #define BIT_MSK__FCSTAT11__REG_HFC_STAT11                                        0xFF

// HSIC Flow Control STAT12 Register
#define REG_ADDR__FCSTAT12                                               (Page_1 | 0x0098)
  // (ReadOnly, Bits 7:0) 
  // hsic_tx_demux link_rx_rtags
  #define BIT_MSK__FCSTAT12__REG_HFC_STAT12                                        0xFF

// HSIC Flow Control STAT13 Register
#define REG_ADDR__FCSTAT13                                               (Page_1 | 0x0099)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: hsic_tx_demux link_rx_raddr bit 3: hsic_tx_demux link_rx_error bit 7-4: hsic_tx_demux current state
  #define BIT_MSK__FCSTAT13__REG_HFC_STAT13                                        0xFF

// HSIC Flow Control STAT14 Register
#define REG_ADDR__FCSTAT14                                               (Page_1 | 0x009A)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCSTAT14__REG_HFC_STAT14                                        0xFF

// HSIC Flow Control STAT15 Register
#define REG_ADDR__FCSTAT15                                               (Page_1 | 0x009B)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__FCSTAT15__REG_HFC_STAT15                                        0xFF

// HSIC Flow Control STAT16 Register
#define REG_ADDR__FCSTAT16                                               (Page_1 | 0x009C)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: link2mem mem_state current state bit 7-3: 5LSBs of link2mem mem_addr
  #define BIT_MSK__FCSTAT16__REG_HFC_STAT16                                        0xFF

// HSIC Flow Control STAT17 Register
#define REG_ADDR__FCSTAT17                                               (Page_1 | 0x009D)
  // (ReadOnly, Bits 7:0) 
  // bit 6-0: 7MSBs of link2mem mem_addr bit 7: LSB of link2mem fc_afifo_waddr
  #define BIT_MSK__FCSTAT17__REG_HFC_STAT17                                        0xFF

// HSIC Flow Control STAT18 Register
#define REG_ADDR__FCSTAT18                                               (Page_1 | 0x009E)
  // (ReadOnly, Bits 7:0) 
  // bit 1-0: 2MSBs of link2mem fc_afifo_waddr bit 7-2: 6 LSBs of link2mem fc_afifo_wtags
  #define BIT_MSK__FCSTAT18__REG_HFC_STAT18                                        0xFF

// HSIC Flow Control STAT19 Register
#define REG_ADDR__FCSTAT19                                               (Page_1 | 0x009F)
  // (ReadOnly, Bits 7:0) 
  // bit 1-0: 2 MSBs of  link2mem fc_afifo_wtags bit 2: link2mem hsic_pkt_val bit 3: link2mem reserved bit 7-4: link2mem CRC16 good count
  #define BIT_MSK__FCSTAT19__REG_HFC_STAT19                                        0xFF

// HSIC Flow Control STAT20 Register
#define REG_ADDR__FCSTAT20                                               (Page_1 | 0x00A0)
  // (ReadOnly, Bits 7:0) 
  // mem2hsic hsic_wtags
  #define BIT_MSK__FCSTAT20__REG_HFC_STAT20                                        0xFF

// HSIC Flow Control STAT21 Register
#define REG_ADDR__FCSTAT21                                               (Page_1 | 0x00A1)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: mem2hsic hsic_waddr bit 4-3: mem2hsic hsic_state bit 7-5: mem2hsic state
  #define BIT_MSK__FCSTAT21__REG_HFC_STAT21                                        0xFF

// HSIC Flow Control STAT22 Register
#define REG_ADDR__FCSTAT22                                               (Page_1 | 0x00A2)
  // (ReadOnly, Bits 7:0) 
  // 8 msbs of mem2hsic mem_addr
  #define BIT_MSK__FCSTAT22__REG_HFC_STAT22                                        0xFF

// HSIC Flow Control STAT23 Register
#define REG_ADDR__FCSTAT23                                               (Page_1 | 0x00A3)
  // (ReadOnly, Bits 7:0) 
  // bit 3-0: 4 lsbs of mem2hsic mem_addr bit 7-4: rsvd
  #define BIT_MSK__FCSTAT23__REG_HFC_STAT23                                        0xFF

// HSIC Flow Control STAT24 Register
#define REG_ADDR__FCSTAT24                                               (Page_1 | 0x00A4)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT24__REG_HFC_STAT24                                        0xFF

// HSIC Flow Control STAT25 Register
#define REG_ADDR__FCSTAT25                                               (Page_1 | 0x00A5)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT25__REG_HFC_STAT25                                        0xFF

// HSIC Flow Control STAT26 Register
#define REG_ADDR__FCSTAT26                                               (Page_1 | 0x00A6)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT26__REG_HFC_STAT26                                        0xFF

// HSIC Flow Control STAT27 Register
#define REG_ADDR__FCSTAT27                                               (Page_1 | 0x00A7)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT27__REG_HFC_STAT27                                        0xFF

// HSIC Flow Control STAT28 Register
#define REG_ADDR__FCSTAT28                                               (Page_1 | 0x00A8)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT28__REG_HFC_STAT28                                        0xFF

// HSIC Flow Control STAT29 Register
#define REG_ADDR__FCSTAT29                                               (Page_1 | 0x00A9)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT29__REG_HFC_STAT29                                        0xFF

// HSIC Flow Control STAT30 Register
#define REG_ADDR__FCSTAT30                                               (Page_1 | 0x00AA)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT30__REG_HFC_STAT30                                        0xFF

// HSIC Flow Control STAT31 Register
#define REG_ADDR__FCSTAT31                                               (Page_1 | 0x00AB)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT31__REG_HFC_STAT31                                        0xFF

// HSIC Flow Control STAT32 Register
#define REG_ADDR__FCSTAT32                                               (Page_1 | 0x00AC)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT32__REG_HFC_STAT32                                        0xFF

// HSIC Flow Control STAT33 Register
#define REG_ADDR__FCSTAT33                                               (Page_1 | 0x00AD)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT33__REG_HFC_STAT33                                        0xFF

// HSIC Flow Control STAT34 Register
#define REG_ADDR__FCSTAT34                                               (Page_1 | 0x00AE)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT34__REG_HFC_STAT34                                        0xFF

// HSIC Flow Control STAT35 Register
#define REG_ADDR__FCSTAT35                                               (Page_1 | 0x00AF)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT35__REG_HFC_STAT35                                        0xFF

// HSIC Flow Control STAT36 Register
#define REG_ADDR__FCSTAT36                                               (Page_1 | 0x00B0)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT36__REG_HFC_STAT36                                        0xFF

// HSIC Flow Control STAT37 Register
#define REG_ADDR__FCSTAT37                                               (Page_1 | 0x00B1)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT37__REG_HFC_STAT37                                        0xFF

// HSIC Flow Control STAT38 Register
#define REG_ADDR__FCSTAT38                                               (Page_1 | 0x00B2)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT38__REG_HFC_STAT38                                        0xFF

// HSIC Flow Control STAT39 Register
#define REG_ADDR__FCSTAT39                                               (Page_1 | 0x00B3)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT39__REG_HFC_STAT39                                        0xFF

// HSIC Flow Control STAT40 Register
#define REG_ADDR__FCSTAT40                                               (Page_1 | 0x00B4)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT40__REG_HFC_STAT40                                        0xFF

// HSIC Flow Control STAT41 Register
#define REG_ADDR__FCSTAT41                                               (Page_1 | 0x00B5)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT41__REG_HFC_STAT41                                        0xFF

// HSIC Flow Control STAT42 Register
#define REG_ADDR__FCSTAT42                                               (Page_1 | 0x00B6)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT42__REG_HFC_STAT42                                        0xFF

// HSIC Flow Control STAT43 Register
#define REG_ADDR__FCSTAT43                                               (Page_1 | 0x00B7)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT43__REG_HFC_STAT43                                        0xFF

// HSIC Flow Control STAT44 Register
#define REG_ADDR__FCSTAT44                                               (Page_1 | 0x00B8)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT44__REG_HFC_STAT44                                        0xFF

// HSIC Flow Control STAT45 Register
#define REG_ADDR__FCSTAT45                                               (Page_1 | 0x00B9)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT45__REG_HFC_STAT45                                        0xFF

// HSIC Flow Control STAT46 Register
#define REG_ADDR__FCSTAT46                                               (Page_1 | 0x00BA)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT46__REG_HFC_STAT46                                        0xFF

// HSIC Flow Control STAT47 Register
#define REG_ADDR__FCSTAT47                                               (Page_1 | 0x00BB)
  // (ReadOnly, Bits 7:0) 
  // flow_control status
  #define BIT_MSK__FCSTAT47__REG_HFC_STAT47                                        0xFF

// HSIC Flow Control Filter Devlist Register 0
#define REG_ADDR__FCFDEVLIST0                                            (Page_1 | 0x00BC)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #0. Filtering device list: One hot encoding device address to allow/block (Default 128'h0)
  #define BIT_MSK__FCFDEVLIST0__REG_HFC_FDEVLIST0                                     0xFF

// HSIC Flow Control Filter Devlist Register 1
#define REG_ADDR__FCFDEVLIST1                                            (Page_1 | 0x00BD)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #1
  #define BIT_MSK__FCFDEVLIST1__REG_HFC_FDEVLIST1                                     0xFF

// HSIC Flow Control Filter Devlist Register 2
#define REG_ADDR__FCFDEVLIST2                                            (Page_1 | 0x00BE)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #2
  #define BIT_MSK__FCFDEVLIST2__REG_HFC_FDEVLIST2                                     0xFF

// HSIC Flow Control Filter Devlist Register 3
#define REG_ADDR__FCFDEVLIST3                                            (Page_1 | 0x00BF)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #3
  #define BIT_MSK__FCFDEVLIST3__REG_HFC_FDEVLIST3                                     0xFF

// HSIC Flow Control Filter Devlist Register 4
#define REG_ADDR__FCFDEVLIST4                                            (Page_1 | 0x00C0)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #4
  #define BIT_MSK__FCFDEVLIST4__REG_HFC_FDEVLIST4                                     0xFF

// HSIC Flow Control Filter Devlist Register 5
#define REG_ADDR__FCFDEVLIST5                                            (Page_1 | 0x00C1)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #5
  #define BIT_MSK__FCFDEVLIST5__REG_HFC_FDEVLIST5                                     0xFF

// HSIC Flow Control Filter Devlist Register 6
#define REG_ADDR__FCFDEVLIST6                                            (Page_1 | 0x00C2)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #6
  #define BIT_MSK__FCFDEVLIST6__REG_HFC_FDEVLIST6                                     0xFF

// HSIC Flow Control Filter Devlist Register 7
#define REG_ADDR__FCFDEVLIST7                                            (Page_1 | 0x00C3)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #7
  #define BIT_MSK__FCFDEVLIST7__REG_HFC_FDEVLIST7                                     0xFF

// HSIC Flow Control Filter Devlist Register 8
#define REG_ADDR__FCFDEVLIST8                                            (Page_1 | 0x00C4)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #8
  #define BIT_MSK__FCFDEVLIST8__REG_HFC_FDEVLIST8                                     0xFF

// HSIC Flow Control Filter Devlist Register 9
#define REG_ADDR__FCFDEVLIST9                                            (Page_1 | 0x00C5)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #9
  #define BIT_MSK__FCFDEVLIST9__REG_HFC_FDEVLIST9                                     0xFF

// HSIC Flow Control Filter Devlist Register 10
#define REG_ADDR__FCFDEVLIST10                                           (Page_1 | 0x00C6)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #10
  #define BIT_MSK__FCFDEVLIST10__REG_HFC_FDEVLIST10                                    0xFF

// HSIC Flow Control Filter Devlist Register 11
#define REG_ADDR__FCFDEVLIST11                                           (Page_1 | 0x00C7)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #11
  #define BIT_MSK__FCFDEVLIST11__REG_HFC_FDEVLIST11                                    0xFF

// HSIC Flow Control Filter Devlist Register 12
#define REG_ADDR__FCFDEVLIST12                                           (Page_1 | 0x00C8)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #12
  #define BIT_MSK__FCFDEVLIST12__REG_HFC_FDEVLIST12                                    0xFF

// HSIC Flow Control Filter Devlist Register 13
#define REG_ADDR__FCFDEVLIST13                                           (Page_1 | 0x00C9)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #13
  #define BIT_MSK__FCFDEVLIST13__REG_HFC_FDEVLIST13                                    0xFF

// HSIC Flow Control Filter Devlist Register 14
#define REG_ADDR__FCFDEVLIST14                                           (Page_1 | 0x00CA)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #14
  #define BIT_MSK__FCFDEVLIST14__REG_HFC_FDEVLIST14                                    0xFF

// HSIC Flow Control Filter Devlist Register 15
#define REG_ADDR__FCFDEVLIST15                                           (Page_1 | 0x00CB)
  // (ReadWrite, Bits 7:0) 
  // HSIC Flow Control Filter device list, byte #15
  #define BIT_MSK__FCFDEVLIST15__REG_HFC_FDEVLIST15                                    0xFF

// HSIC Rx/Tx Remote Wakeup Start Register 0
#define REG_ADDR__HRWUSRT0                                               (Page_1 | 0x00CC)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_start, byte #0. If resume from device sustains for a given time, stop driving RESUME on HSIC. Default=100us=24'h5DC0
  #define BIT_MSK__HRWUSRT0__REG_HRX_RWU_START0                                    0xFF

// HSIC Rx/Tx Remote Wakeup Start Register 1
#define REG_ADDR__HRWUSRT1                                               (Page_1 | 0x00CD)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_start, byte #1. If resume from device sustains for a given time, stop driving RESUME on HSIC. Default=100us=24'h5DC0
  #define BIT_MSK__HRWUSRT1__REG_HRX_RWU_START1                                    0xFF

// HSIC Rx/Tx Remote Wakeup Start Register 2
#define REG_ADDR__HRWUSRT2                                               (Page_1 | 0x00CE)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_start, byte #2. If resume from device sustains for a given time, stop driving RESUME on HSIC. Default=100us=24'h5DC0
  #define BIT_MSK__HRWUSRT2__REG_HRX_RWU_START2                                    0xFF

// HSIC Rx/Tx Remote Wakeup End Register 0
#define REG_ADDR__HRWUEND0                                               (Page_1 | 0x00CF)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_end, byte #0. If resume from device sustains for a given time, send RESUME to link to takeover. Default=10ms=24'h249F00
  #define BIT_MSK__HRWUEND0__REG_HRX_RWU_END0                                      0xFF

// HSIC Rx/Tx Remote Wakeup End Register 1
#define REG_ADDR__HRWUEND1                                               (Page_1 | 0x00D0)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_end, byte #1. If resume from device sustains for a given time, send RESUME to link to takeover. Default=10ms=24'h249F00
  #define BIT_MSK__HRWUEND1__REG_HRX_RWU_END1                                      0xFF

// HSIC Rx/Tx Remote Wakeup End Register 2
#define REG_ADDR__HRWUEND2                                               (Page_1 | 0x00D1)
  // (ReadWrite, Bits 7:0) 
  // ri_twu_takeover_end, byte #2. If resume from device sustains for a given time, send RESUME to link to takeover. Default=10ms=24'h249F00
  #define BIT_MSK__HRWUEND2__REG_HRX_RWU_END2                                      0xFF

//***************************************************************************
// page 32. Address: 40
// EDID Interrupt Source Register
#define REG_ADDR__EDID_INTR                                              (Page_2 | 0x0000)
  // (ReadWrite, Bits 0) 
  // EDID is available in EDID FIFO
  #define BIT_MSK__EDID_INTR__REG_INTR_STAT0                                        0x01
  // (ReadWrite, Bits 1) 
  // EDID read error
  #define BIT_MSK__EDID_INTR__REG_INTR_STAT1                                        0x02

// EDID Interrupt Mask Register
#define REG_ADDR__EDID_INTR_MASK                                         (Page_2 | 0x0001)
  // (ReadWrite, Bits 0) 
  // Enable INT[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__EDID_INTR_MASK__REG_INTR_MASK0                                        0x01
  // (ReadWrite, Bits 1) 
  // Enable INT[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__EDID_INTR_MASK__REG_INTR_MASK1                                        0x02

// EDID Control Register
#define REG_ADDR__EDID_CTRL                                              (Page_2 | 0x0002)
  // (ReadWrite, Bits 0) 
  // 0: Disable EDID function 1: Enable EDID function
  #define BIT_MSK__EDID_CTRL__REG_EDID_MODE_EN                                      0x01
  // (ReadWrite, Bits 2:1) 
  // 00: Downstream-retrived EDID data will be written into FIFO from 0x000 - 0x07F (1st half)  01: Downstream-retrived EDID data will be written into FIFO from 0x080 - 0x0FF (2nd half) 10: Downstream-retrived EDID data will be written into FIFO from 0x100 - 0x17F (3rd half)  11: Downstream-retrived EDID data will be written into FIFO from 0x180 - 0x1FF (4th half) If only 256 byte FIFO is used always tie Bit[2] to zero
  #define BIT_MSK__EDID_CTRL__REG_EDID_FIFO_BLOCK_SEL                               0x06
  // (ReadWrite, Bits 3) 
  // 1: Enable EDID FIFO access all the time 0: I2C can only access EDID FIFO after MHL established
  #define BIT_MSK__EDID_CTRL__REG_EDID_FIFO_ACCESS_ALWAYS_EN                        0x08
  // (ReadWrite, Bits 4) 
  // 0: Do not increment EDID FIFO address pointer  1: Increment EDID FIFO address pointer automatically
  #define BIT_MSK__EDID_CTRL__REG_EDID_FIFO_ADDR_AUTO                               0x10
  // (ReadWrite, Bits 5) 
  // the Most Singnificant Bit of reg_edid_fifo_addr If more than 256 Bytes EDID is required this bit is combined with reg_edid_fifo_addr[7:0] to generate 9 bits address for EDID FIFO access. The 9 bit address will be: {reg_edid_fifo_addr_msb reg_edid_fifo_addr[7:0]}.  Otherwise set this bit as 0
  #define BIT_MSK__EDID_CTRL__REG_EDID_FIFO_ADDR_MSB                                0x20
  // (ReadWrite, Bits 6) 
  // This bit need to be 1 for HDMI mode and 0 for MHL
  #define BIT_MSK__EDID_CTRL__REG_EDID_HDMI_MHL_MODE                                0x40

// EDID FIFO Addr Register
#define REG_ADDR__EDID_FIFO_ADDR                                         (Page_2 | 0x0003)
  // (ReadWrite, Bits 7:0) 
  // EDID FIFO Address
  #define BIT_MSK__EDID_FIFO_ADDR__REG_EDID_FIFO_ADDR                                    0xFF

// EDID FIFO Write Data Register
#define REG_ADDR__EDID_FIFO_WR_DATA                                      (Page_2 | 0x0004)
  // (ReadWrite, Bits 7:0) 
  // EDID FIFO Write Data
  #define BIT_MSK__EDID_FIFO_WR_DATA__REG_EDID_FIFO_WR_DATA                                 0xFF

// EDID FIFO Internal msb Addr Register
#define REG_ADDR__EDID_FIFO_ADDR_MON_MSB                                 (Page_2 | 0x0005)
  // (ReadOnly, Bits 0) 
  // EDID FIFO msb Internal Address Monitor (First read EDID_FIFO_ADDR_MON_LSB to update)
  #define BIT_MSK__EDID_FIFO_ADDR_MON_MSB__REG_EDID_FIFO_ADDR_MON_MSB                            0x01

// EDID FIFO Internal lsb Addr Register
#define REG_ADDR__EDID_FIFO_ADDR_MON_LSB                                 (Page_2 | 0x0006)
  // (ReadOnly, Bits 7:0) 
  // EDID FIFO lsb Internal Address Monitor
  #define BIT_MSK__EDID_FIFO_ADDR_MON_LSB__REG_EDID_FIFO_ADDR_MON_LSB                            0xFF

// EDID FIFO Read Data Register
#define REG_ADDR__EDID_FIFO_RD_DATA                                      (Page_2 | 0x0007)
  // (ReadOnly, Bits 7:0) 
  // EDID FIFO Read Data
  #define BIT_MSK__EDID_FIFO_RD_DATA__REG_EDID_FIFO_RD_DATA                                 0xFF

// EDID DDC Segment Pointer 0  Register
#define REG_ADDR__EDID_START_EXT0                                        (Page_2 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Start to get EDID block 0. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT0__REG_GET_EDID_START_0                                  0x01

// EDID DDC Segment Pointer 1 Register
#define REG_ADDR__EDID_START_EXT1                                        (Page_2 | 0x0009)
  // (ReadWrite, Bits 0) 
  // Start to get EDID block 1. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_1                                  0x01
  // (ReadWrite, Bits 1) 
  // Start to get EDID block 2. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_2                                  0x02
  // (ReadWrite, Bits 2) 
  // Start to get EDID block 3. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_3                                  0x04
  // (ReadWrite, Bits 3) 
  // Start to get EDID block 4. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_4                                  0x08
  // (ReadWrite, Bits 4) 
  // Start to get EDID block 5. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_5                                  0x10
  // (ReadWrite, Bits 5) 
  // Start to get EDID block 6. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_6                                  0x20
  // (ReadWrite, Bits 6) 
  // Start to get EDID block 7. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_7                                  0x40
  // (ReadWrite, Bits 7) 
  // Start to get EDID block 8. Self-resetting to 0. 
  #define BIT_MSK__EDID_START_EXT1__REG_GET_EDID_START_8                                  0x80

// EDID State Machine Status Register
#define REG_ADDR__EDID_STATE_MACHINE                                     (Page_2 | 0x000A)
  // (ReadOnly, Bits 0) 
  // tx_ddc_edid_fetch State Machine  is in IDLE state
  #define BIT_MSK__EDID_STATE_MACHINE__EDID_IDLE_STATE                                       0x01
  // (ReadOnly, Bits 1) 
  // tx_ddc_edid_fetch State Machine  is in IDLE state
  #define BIT_MSK__EDID_STATE_MACHINE__EDID_FETCH_REQ_STATE                                  0x02
  // (ReadOnly, Bits 2) 
  // tx_ddc_edid_fetch State Machine  is in IDLE state
  #define BIT_MSK__EDID_STATE_MACHINE__EDID_WAIT_ACK_STATE                                   0x04

// EDID Restart Register
#define REG_ADDR__EDID_RST                                               (Page_2 | 0x000B)
  // (ReadWrite, Bits 0) 
  // restart edid block. Self-ressetting to 0.
  #define BIT_MSK__EDID_RST__REG_EDID_RESTART                                      0x01

// HDMI2MHL1 Status Register
#define REG_ADDR__HDMI2MHL1_STAT                                         (Page_2 | 0x0018)
  // (ReadOnly, Bits 0) 
  // HDMI to MHL1 FIFO overflow
  #define BIT_MSK__HDMI2MHL1_STAT__HDMI2MHL1_FIFO_OVERFLOW                               0x01
  // (ReadOnly, Bits 1) 
  // HDMI to MHL1 FIFO underflow
  #define BIT_MSK__HDMI2MHL1_STAT__HDMI2MHL1_FIFO_UNDERFLOW                              0x02

// HDMI2MHL1 Control 1st Register
#define REG_ADDR__HDMI2MHL1_CTL0                                         (Page_2 | 0x0019)
  // (ReadWrite, Bits 7:0) 
  //  I2C register storing MHL commn mode clock shape for EMI reduction  i.      goes to clock shaping digital block ii.      default is 10b0000011111  normal MHL clock iii.      can change common mode clock duty by moving 0-1 transition location. For example; 1.       10b0011111111 -] 80/20 duty 2.       10b0000000001 -] 10/90 duty
  #define BIT_MSK__HDMI2MHL1_CTL0__REG_TX_CLK_SHAPE_B7_B0                                0xFF

// HDMI2MHL1 Control 2nd Register
#define REG_ADDR__HDMI2MHL1_CTL1                                         (Page_2 | 0x001A)
  // (ReadWrite, Bits 1:0) 
  //  I2C register storing MHL commn mode clock shape for EMI reduction  i.      goes to clock shaping digital block ii.      default is 10b0000011111  normal MHL clock iii.      can change common mode clock duty by moving 0-1 transition location. For example; 1.       10b0011111111 -] 80/20 duty 2.       10b0000000001 -] 10/90 duty
  #define BIT_MSK__HDMI2MHL1_CTL1__REG_TX_CLK_SHAPE_B9_B8                                0x03
  // (ReadWrite, Bits 7:5) 
  // I2C register selecting EMI mode  i.      goes to both clock shaping digital block and Tx core ii.      3b00x,010 -] normal MHL clock with 50/50 duty, Tx core ignores clock shape from digital block and internally generates 50/50 3x clock pattern which is MHL default clock shape iii.     3'b011 -] clock shaping for 1/2x clock iv.      3b100 -] clock shaping for 3x clock 1.       digital block passes clk_shape[9:0] data from I2C to Tx core w/o change v.      3b101 -] clock shaping for 1x clock 1.       digital block expand clk_shape[9:0] into clk_shape_1x[29:0]. a.       Clk_shape_1x[29:27] = clk_shape[9] b.      Clk_shape_1x[26:24] = clk_shape[8] c.        d.      Clk_shape_1x[5:3] = clk_shape[1] e.       Clk_shape_1x[2:0] = clk_shape[0] 2.       in 3 cycles of mhl_clk (300MHz), send 30bit as 3 chunks of 10-bit data a.       cycle n: clk_shape_1x[9:0] b.      cycle n+1: clk_shape_1x[19:10] c.       cycle n+2: clk_shape_1x[29:20] d.      repeat 3.       no need to align cycle  can start from arbitrary clock cycle vi.      3b110 -] falling edge modulation 40/60 at 3x clock rate (300MHz) 1.       cycle n : send 10b0000_111111 to Tx core 2.       cycle n+1 : send 10b000000_1111 to Tx core 3.       repeat vii.      3b111 -] falling edge modulation 30/70 at 3x clock rate (300MHz) 1.       cycle n: send 10b000_1111111 to Tx core 2.       cycle n+1: send 10b0000000_111 to Tx core 3.       repeat
  #define BIT_MSK__HDMI2MHL1_CTL1__REG_EMI_SEL                                           0xE0

// HDMI2MHL1 Control 3rd Register
#define REG_ADDR__HDMI2MHL1_CTL2                                         (Page_2 | 0x001B)
  // (ReadWrite, Bits 7:0) 
  // Lower 8 bits of the fixed 10 bit pattern that HDMI2MHL1 sends out
  #define BIT_MSK__HDMI2MHL1_CTL2__REG_10BIT_PAT_B7_B0                                   0xFF

// HDMI2MHL1 Control 4th Register
#define REG_ADDR__HDMI2MHL1_CTL3                                         (Page_2 | 0x001C)
  // (ReadWrite, Bits 1:0) 
  // Upper 2 bits of the fixed 10 bit pattern that HDMI2MHL1 sends out
  #define BIT_MSK__HDMI2MHL1_CTL3__REG_10BIT_PAT_B9_B8                                   0x03
  // (ReadWrite, Bits 2) 
  // Enable HDMI2MHL1 send out fixed 10 bit pattern
  #define BIT_MSK__HDMI2MHL1_CTL3__REG_10BIT_PAT_EN                                      0x04
  // (ReadWrite, Bits 3) 
  // 1:  Bit swap Q data out from encoder (default) 0:  No swap
  #define BIT_MSK__HDMI2MHL1_CTL3__REG_REVERSE_BIT_ORDER                                 0x08
  // (ReadWrite, Bits 6) 
  // MHL FIFO reset 1 - Reset MHL FIFO 0 - Normal operation (default).
  #define BIT_MSK__HDMI2MHL1_CTL3__REG_MHLFIFO_RST                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL2 PP mode
  #define BIT_MSK__HDMI2MHL1_CTL3__REG_MHL2_PP_EN                                        0x80

// HDMI2MHL1 Control 5th Register
#define REG_ADDR__HDMI2MHL1_CTL4                                         (Page_2 | 0x001D)
  // (ReadWrite, Bits 0) 
  // Scrambler on/off setting.
  #define BIT_MSK__HDMI2MHL1_CTL4__REG_SCR_ON                                            0x01
  // (ReadWrite, Bits 1) 
  // Scrambler mode. 0: normal; 1: CTS
  #define BIT_MSK__HDMI2MHL1_CTL4__REG_SCR_MD                                            0x02
  // (ReadWrite, Bits 2) 
  // Scrambler on/off for MHL1/2
  #define BIT_MSK__HDMI2MHL1_CTL4__REG_MHL12_SCON                                        0x04
  // (ReadWrite, Bits 3) 
  // HDMI mode overwriting on/off
  #define BIT_MSK__HDMI2MHL1_CTL4__REG_HDMIMD_OVR                                        0x08
  // (ReadWrite, Bits 4) 
  // HDMI mode overwriting value
  #define BIT_MSK__HDMI2MHL1_CTL4__REG_HDMIMD_VAL                                        0x10

// HDMI2MHL1 Control 6th Register
#define REG_ADDR__HDMI2MHL1_CTL5                                         (Page_2 | 0x001E)
  // (ReadWrite, Bits 7:0) 
  // SYNC interval counter limit (default 8'h0F)
  #define BIT_MSK__HDMI2MHL1_CTL5__REG_SYNC_INT_LMT                                      0xFF

// Tx Zone Ctl0 Register
#define REG_ADDR__TX_ZONE_CTL0                                           (Page_2 | 0x0020)
  // (ReadWrite, Bits 7:0) 
  // Used to decide the zone range. Fixed to 8.
  #define BIT_MSK__TX_ZONE_CTL0__REG_MAX_DIFF_LIMIT                                    0xFF

// Tx Zone Ctl1 Register
#define REG_ADDR__TX_ZONE_CTL1                                           (Page_2 | 0x0021)
  // (ReadWrite, Bits 7:0) 
  //   Zone control parameters (from i2c; default = 6'b00000)   [7:6]    : df_tap   [5]    : mult_comp   [4]    : sel_cecclk   [3:2]    : zonectl_mode   [1]    : strict   [0]    : longer   After transition state; it should be programmed to 4'b10** thru   i2c for normal PLL operation
  #define BIT_MSK__TX_ZONE_CTL1__REG_TX_ZONE_CTRL                                      0xFF

// Tx Zone Ctl2 Register
#define REG_ADDR__TX_ZONE_CTL2                                           (Page_2 | 0x0022)
  // (ReadWrite, Bits 1:0) 
  // MHL2 Tx zone select from I2C 00: 1x Zone 01: 2x Zone 10: 4x Zone 11: 8x Zone
  #define BIT_MSK__TX_ZONE_CTL2__REG_TX_ZONE                                           0x03
  // (ReadOnly, Bits 7:6) 
  // MHL2 Tx zone status 00: 1x Zone 01: 2x Zone 10: 4x Zone 11: 8x Zone
  #define BIT_MSK__TX_ZONE_CTL2__REG_TX_ZONE_SEL                                       0xC0

// Tx Zone Ctl3 Register
#define REG_ADDR__TX_ZONE_CTL3                                           (Page_2 | 0x0023)
  // (ReadOnly, Bits 5:0) 
  // [5]    : overflow flag [4:0]    : counter
  #define BIT_MSK__TX_ZONE_CTL3__HRV_ZONE_CTRL3                                        0x3F
  // (ReadWrite, Bits 7:6) 
  // [7]    : run reference clock counter   : reg_tx_zone_ctrl3[1] [6]    : clear reference clock counter : reg_tx_zone_ctrl3[0]
  #define BIT_MSK__TX_ZONE_CTL3__REG_TX_ZONE_CTRL3                                     0xC0

// TX Zone Ctl 4 Register
#define REG_ADDR__TX_ZONEL_CTL4                                          (Page_2 | 0x0024)
  // (ReadWrite, Bits 3:2) 
  // Select frequency ratio betweeen hdmi_clk and pxl_clk 2'b00 0.5x mode. hdmi_clk_freq = 0.5*pxl_clk_freq 2'b01 1x mode. hdmi_clk_freq = 1*pxl_clk_freq (default) 2'b10 2x mode. hdmi_clk_freq = 2*pxl_clk_freq 2'b11 4x mode. hdmi_clk_freq = 4*pxl_clk_freq
  #define BIT_MSK__TX_ZONEL_CTL4__REG_HDMI_CLK_RATIO                                    0x0C

// TX Zone Ctl 5 Register
#define REG_ADDR__TX_ZONE_CTL5                                           (Page_2 | 0x0025)
  // (ReadWrite, Bits 3) 
  // measure enable signal for zone control. when enable;  pll is in open loop
  #define BIT_MSK__TX_ZONE_CTL5__REG_MEAS_FVCO                                         0x08
  // (ReadWrite, Bits 7) 
  // Enable clock detect
  #define BIT_MSK__TX_ZONE_CTL5__REG_CLKDETECT_EN                                      0x80

// MHL3 Tx Zone Ctl Register
#define REG_ADDR__MHL3_TX_ZONE_CTL                                       (Page_2 | 0x0026)
  // (ReadWrite, Bits 1:0) 
  // MHL3 Tx zone control. This controls MHL3 sub-multiple mode 00: 6G mode 01: 3G mode 10: 1.5G mode
  #define BIT_MSK__MHL3_TX_ZONE_CTL__REG_MHL3_TX_ZONE                                      0x03
  // (ReadWrite, Bits 7) 
  // When set to 1'b1; zone controls for Tx PHY and interpolator are separated. The interpolator zone is controlled by reg_mhl3_tx_zone.
  #define BIT_MSK__MHL3_TX_ZONE_CTL__REG_MHL2_INTPLT_ZONE_MANU_EN                          0x80

// Zone VCO Control Register
#define REG_ADDR__TX_ZONEVCO_CTL                                         (Page_2 | 0x0027)
  // (ReadWrite, Bits 0) 
  // 1 to start auto zone calculation. To start next calculation, it should be cleared first and asserted.
  #define BIT_MSK__TX_ZONEVCO_CTL__REG_ZONEVCO_START                                     0x01

// MHL3 CTS Control Register
#define REG_ADDR__MHL3CTS_CTL                                            (Page_2 | 0x0028)
  // (ReadWrite, Bits 1) 
  // Select MHL3 CTS operation mode 1 - 3 lanes (default) 0 - 2 lanes
  #define BIT_MSK__MHL3CTS_CTL__REG_MHL3CTS_3LANE                                     0x02
  // (ReadWrite, Bits 3:2) 
  // Select the output clock for MHL3 CTS 00 - 1/2 of link clock when reg_mhl3cts_3lane = 1'b0 or 1/3 of link clock when reg_mhl3cts_3lane = 1'b1 01 - 1/4 of link clock when reg_mhl3cts_3lane = 1'b0 or 1/6 of link clock when reg_mhl3cts_3lane = 1'b1 10 - 1/8 of link clock when reg_mhl3cts_3lane = 1'b0 or 1/12 of link clock when reg_mhl3cts_3lane = 1'b1 (default) 11 - 1/16 of link clock when reg_mhl3cts_3lane = 1'b0 or 1/24 of link clock when reg_mhl3cts_3lane = 1'b1
  #define BIT_MSK__MHL3CTS_CTL__REG_MHL3CTS_CLKOUT_SEL                                0x0C
  // (ReadWrite, Bits 4) 
  // Select MHL3 CTS operation mode 1 - 1 lanes (default) 0 - up to reg_mhl3cts_3lane (0xB4[1])
  #define BIT_MSK__MHL3CTS_CTL__REG_MHL3CTS_1LANE                                     0x10
  // (ReadWrite, Bits 6) 
  // Enable MHL3 CTS Snoop mode
  #define BIT_MSK__MHL3CTS_CTL__REG_COC_DOC_SNOOP_EN                                  0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL3 CTS mode
  #define BIT_MSK__MHL3CTS_CTL__REG_MHL3CTS_EN                                        0x80

// TX X BIST CNTL Register
#define REG_ADDR__TX_XBIST_CNTL                                          (Page_2 | 0x0029)
  // (ReadWrite, Bits 0) 
  // select bist data path to serializer
  #define BIT_MSK__TX_XBIST_CNTL__REG_BIST_SEL                                          0x01
  // (ReadWrite, Bits 1) 
  // 1'b0  txbist is disabled.    (default value)1'b1 -  txbist is enabled.   
  #define BIT_MSK__TX_XBIST_CNTL__REG_BIST_EN                                           0x02
  // (ReadWrite, Bits 2) 
  // 1'b1 - divide clock by 2
  #define BIT_MSK__TX_XBIST_CNTL__REG_HALF_CLK_SEL                                      0x04
  // (ReadWrite, Bits 3) 
  // Make AV link data speed 4x slower. 1 = enable
  #define BIT_MSK__TX_XBIST_CNTL__REG_QUARTER_CLK_SEL                                   0x08
  // (ReadWrite, Bits 5) 
  // Disable create signature if = 1'b1
  #define BIT_MSK__TX_XBIST_CNTL__REG_DISABLE_ENC                                       0x20
  // (ReadWrite, Bits 6) 
  // Backward compatibility for previous version.
  #define BIT_MSK__TX_XBIST_CNTL__REG_LEGACY                                            0x40

// TX X BIST INST LOW Register
#define REG_ADDR__TX_XBIST_INST_LOW                                      (Page_2 | 0x002A)
  // (ReadWrite, Bits 7:0) 
  // 11-bit Instruction register for Red Green and Blue channels. LSB 8 bits here and 2 bits to follow
  #define BIT_MSK__TX_XBIST_INST_LOW__REG_INSTRUCTION_B7_B0                                 0xFF

// TX X BIST INST HIGH Register
#define REG_ADDR__TX_XBIST_INST_HIGH                                     (Page_2 | 0x002B)
  // (ReadWrite, Bits 2:0) 
  // 11-bit Instruction register for Red Green and Blue channels. LSB 8 bits here and 2 bits to follow
  #define BIT_MSK__TX_XBIST_INST_HIGH__REG_INSTRUCTION_B10_B8                                0x07

// TX X BIST PATTERN LOW Register
#define REG_ADDR__TX_XBIST_PAT_LOW                                       (Page_2 | 0x002C)
  // (ReadWrite, Bits 7:0) 
  // pattern control register for Red Green and Blue channels.
  #define BIT_MSK__TX_XBIST_PAT_LOW__REG_PATTERN_B7_B0                                     0xFF

// TX X BIST PATTERN HIGH Register
#define REG_ADDR__TX_XBIST_PAT_HIGH                                      (Page_2 | 0x002D)
  // (ReadWrite, Bits 1:0) 
  // pattern control register for Red Green and Blue channels.
  #define BIT_MSK__TX_XBIST_PAT_HIGH__REG_PATTERN_B9_B8                                     0x03

// TX X BIST CONFIGURE LOW Register
#define REG_ADDR__TX_XBIST_CONF_LOW                                      (Page_2 | 0x002E)
  // (ReadWrite, Bits 7:0) 
  // TBA
  #define BIT_MSK__TX_XBIST_CONF_LOW__REG_CONFIGURE_B7_B0                                   0xFF

// TX X BIST CONFIGURE HIGH Register
#define REG_ADDR__TX_XBIST_CONF_HIGH                                     (Page_2 | 0x002F)
  // (ReadWrite, Bits 1:0) 
  // TBA
  #define BIT_MSK__TX_XBIST_CONF_HIGH__REG_CONFIGURE_B9_B8                                   0x03

// TX X BIST STATUS Register
#define REG_ADDR__TX_XBIST_STATUS                                        (Page_2 | 0x0030)
  // (ReadOnly, Bits 5:0) 
  // bit 1 and 0 for channel 0; bit 3 and 2 for channel 1; bit 5 and 4 for channel 3. 00 : tx bist is disable. 01 : tx bist is enable. 10 : tx bist is running. 11 : tx bist is done.
  #define BIT_MSK__TX_XBIST_STATUS__BIST_STATE                                            0x3F

// TX XBIST Test Period Count Register
#define REG_ADDR__TX_XBIST_PAT_PERIOD_CNT                                (Page_2 | 0x0031)
  // (ReadWrite, Bits 7:0) 
  // Number of specified test to be repeated ( 3    reg_period_cnt    255 ) for rand1; rand2; DC and hs10Bus
  #define BIT_MSK__TX_XBIST_PAT_PERIOD_CNT__REG_PAT_PERIOD_CNT_B7_B0                              0xFF

// HDMI Control 0 Register
#define REG_ADDR__HDMICTL0                                               (Page_2 | 0x0032)
  // (ReadWrite, Bits 0) 
  // 1:  Bit swap Q data out from HDMI2 encoder 0:  No swap (default)
  #define BIT_MSK__HDMICTL0__REG_Q_9T0                                             0x01
  // (ReadWrite, Bits 1) 
  // 1:  Has 4 full channels (RGB; CK) (default) 0:  Force CK channel to all zero
  #define BIT_MSK__HDMICTL0__REG_USE_CH_MUX                                        0x02
  // (ReadWrite, Bits 2) 
  // 1:  Invert tx bit  for ch0;1;2CK 0:  Normal (default)
  #define BIT_MSK__HDMICTL0__REG_TX_BIT_INV                                        0x04
  // (ReadWrite, Bits 7:6) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMICTL0__REG_Q3_SEL                                            0xC0

// HDMI Control 1 Register
#define REG_ADDR__HDMICTL1                                               (Page_2 | 0x0033)
  // (ReadWrite, Bits 1:0) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMICTL1__REG_Q0_SEL                                            0x03
  // (ReadWrite, Bits 3:2) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMICTL1__REG_Q1_SEL                                            0x0C
  // (ReadWrite, Bits 5:4) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMICTL1__REG_Q2_SEL                                            0x30
  // (ReadWrite, Bits 7:6) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMICTL1__REG_QC_SEL                                            0xC0

// AVLINK SW Reset Register
#define REG_ADDR__SW_RST                                                 (Page_2 | 0x0034)
  // (ReadWrite, Bits 2) 
  // Software reset for MHL3 CTS logic 1 - Reset 0 - Normal operation (default)
  #define BIT_MSK__SW_RST__REG_MHL3CTS_RST                                       0x04

// HSIC RX BIST CNTL Register
#define REG_ADDR__HSIC_RX_BIST_CNTL                                      (Page_2 | 0x0040)
  // (ReadWrite, Bits 0) 
  // 1'b0   HSIC bist is disabled (default value).      1'b1 -  HSIC bist is enabled.       
  #define BIT_MSK__HSIC_RX_BIST_CNTL__REG_HRX_BIST_EN                                       0x01
  // (ReadWrite, Bits 1) 
  // 1'b0 -  run the specific pattern only one time. 1'b1 - run the specific pattern repeatly.
  #define BIT_MSK__HSIC_RX_BIST_CNTL__REG_HRX_RUN_CTRL                                      0x02
  // (ReadWrite, Bits 3:2) 
  // change pattern behavior to be inverted.
  #define BIT_MSK__HSIC_RX_BIST_CNTL__REG_HRX_INVERT                                        0x0C
  // (ReadWrite, Bits 5:4) 
  // select of pattern bit for dfferently generation
  #define BIT_MSK__HSIC_RX_BIST_CNTL__REG_HRX_ODD                                           0x30
  // (ReadWrite, Bits 6) 
  // Select LFSR pattern
  #define BIT_MSK__HSIC_RX_BIST_CNTL__REG_HRX_RNDM                                          0x40

// HSIC RX BIST PATTERN Register
#define REG_ADDR__HSIC_RX_BIST_PAT                                       (Page_2 | 0x0041)
  // (ReadWrite, Bits 7:0) 
  // Initial pattern set.
  #define BIT_MSK__HSIC_RX_BIST_PAT__REG_HRX_PATTERN                                       0xFF

// HSIC RXTX BIST STATUS Register
#define REG_ADDR__HSIC_RX_BIST_STATUS                                    (Page_2 | 0x0042)
  // (ReadOnly, Bits 0) 
  // Tx IP BIST Status 1'b1: Running1'b0: Not running
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HRXBIST_RUN                                       0x01
  // (ReadOnly, Bits 1) 
  // Tx IP BIST on status. 1'b0 = rxbist off ;1'b1 = rxbist running.
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HRXBIST_DONE                                      0x02
  // (ReadOnly, Bits 2) 
  // HSIC BIST error result.
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HRXBIST_ERROR                                     0x04
  // (ReadOnly, Bits 3) 
  // HSIC DATA status from PAD.
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HRX_DATA                                          0x08
  // (ReadOnly, Bits 4) 
  // HSIC STROBE status from PAD.
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HRX_STROBE                                        0x10
  // (ReadOnly, Bits 6) 
  // HSIC TX BIST Status 1'b1: Running1'b0: Not running
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HTXBIST_RUN                                       0x40
  // (ReadOnly, Bits 7) 
  // HSIC TX BIST done.
  #define BIT_MSK__HSIC_RX_BIST_STATUS__REG_HTXBIST_DONE                                      0x80

// HSIC TX BIST CNTL1 Register
#define REG_ADDR__HSIC_TX_BIST_CNTL1                                     (Page_2 | 0x0043)
  // (ReadWrite, Bits 0) 
  // 1'b0   HSIC bist is disabled (default value).      1'b1 -  HSIC bist is enabled.       
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_BIST_SEL                                      0x01
  // (ReadWrite, Bits 1) 
  // 1'b0  txbist is disabled.    (default value)1'b1 -  txbist is enabled.  
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_BIST_EN                                       0x02
  // (ReadWrite, Bits 2) 
  // 1'b0 -  run the specific pattern only one time. 1'b1 - run the specific pattern repeatly.
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_RUN_CTRL                                      0x04
  // (ReadWrite, Bits 3) 
  // Select LFSR pattern
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_RNDM                                          0x08
  // (ReadWrite, Bits 5:4) 
  // TBD
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_NUM_KEEP_CYCLE                                0x30
  // (ReadWrite, Bits 7:6) 
  // Select idle time 00: no idle time 01: 32 clocks 10: 512 clocks 11: 4096 clocks
  #define BIT_MSK__HSIC_TX_BIST_CNTL1__REG_HTX_IDLE_MODE                                     0xC0

// HSIC TX BIST CNTL2 Register
#define REG_ADDR__HSIC_TX_BIST_CNTL2                                     (Page_2 | 0x0044)
  // (ReadWrite, Bits 1:0) 
  // change data pattern behavior to be inverted.
  #define BIT_MSK__HSIC_TX_BIST_CNTL2__REG_HTX_DATA_INVERT                                   0x03
  // (ReadWrite, Bits 3:2) 
  // make data pattern odd sequence
  #define BIT_MSK__HSIC_TX_BIST_CNTL2__REG_HTX_DATA_ODD                                      0x0C
  // (ReadWrite, Bits 5:4) 
  // change strobe pattern behavior to be inverted.
  #define BIT_MSK__HSIC_TX_BIST_CNTL2__REG_HTX_STROBE_INVERT                                 0x30
  // (ReadWrite, Bits 7:6) 
  // make strobe pattern odd sequence
  #define BIT_MSK__HSIC_TX_BIST_CNTL2__REG_HTX_STROBE_ODD                                    0xC0

// HSIC TX BIST DATA PATTERN Register
#define REG_ADDR__HSIC_TX_BIST_DATA_PAT                                  (Page_2 | 0x0045)
  // (ReadWrite, Bits 7:0) 
  // Initial pattern set.
  #define BIT_MSK__HSIC_TX_BIST_DATA_PAT__REG_HTX_DATA_PATTERN                                  0xFF

// HSIC TX BIST STROBE PATTERN Register
#define REG_ADDR__HSIC_TX_BIST_STROBE_PAT                                (Page_2 | 0x0046)
  // (ReadWrite, Bits 7:0) 
  // Initial pattern set.
  #define BIT_MSK__HSIC_TX_BIST_STROBE_PAT__REG_HTX_STROBE_PATTERN                                0xFF



// MHL Misc 2nd Ctl Register
#define REG_ADDR__MHL_MISC_CTL1                                          (Page_2 | 0x005E)
  // (ReadWrite, Bits 7:0) 
  // Spare registers for MHL Tx PHY
  #define BIT_MSK__MHL_MISC_CTL1__REG_RSV_B7_B0      




// TX Phy Register
#define TXPHY_TOP_CTL0                                            (Page_7 | 0x00B0)
  // (ReadWrite, Bits 4) 
  // HDMI mode enable 
  #define BIT_MSK__TXPHY_TOP_CTL0__REG_HDMI_EN                                           0x10
  // (ReadWrite, Bits 5) 
  // MHL 3 in CE mode enable
  #define BIT_MSK__TXPHY_TOP_CTL0__REG_MHL3CE_EN                                         0x20
  // (ReadWrite, Bits 6) 
  // Valid only in MHL2 mode - Normal mode or PackedPixel mode selection 1'b0 Normal mode 1'b1 PackedPixel mode
  #define BIT_MSK__TXPHY_TOP_CTL0__REG_MHL_PP_SEL                                        0x40
  // (ReadWrite, Bits 7) 
  // when mhl3_en is enable : mhl3_doc or mhl3_coc mode  selection 1'b0 mhl3 CoC mode 1'b1 mhl3 DoC mode
  #define BIT_MSK__TXPHY_TOP_CTL0__REG_MHL3_DOC_SEL                                      0x80



#define TXPHY_TOP_CTL1                                            (Page_7 | 0x00B1)
  // (ReadWrite, Bits 1:0) 
  //Deep color control: 2'b00:HDMI normal mode 2'b01:HDMI 10 bit mode 2'b10:HDMI 12 bit mode 2'b11: not used
  #define BIT_MSK__TXPHY_TOP_CTL1__REG_DP                                             0x03
  // (ReadWrite, Bits 4) 
  //HDMI 2 Mode enabled  
  #define BIT_MSK__TXPHY_TOP_CTL1__REG_HDMI2_EN                                        0x10

#define TXPHY_DP_CTL0                                            (Page_7 | 0x00B2)
	 
  // (ReadWrite, Bits 7) 
  //reg_dp_oe  driver output enable signal 
  #define BIT_MSK__TXPHY_DP_CTL0__REG_DP_OE							0x80

	// (ReadWrite, Bits 5:0) 
	//reg_tx_oe:clock/data0/data1/data2 channel drive output enable bit[5]: oe_ck bit[3]: oe_d3 bit[2]: oe_d2 bit[1]: oe_d1 bit[0]: oe_d0
  #define BIT_MSK__TXPHY_DP_CTL0__REG_TX_OE							0x27
// TX Phy PLL Ctl Register
#define TXPHY_PLL_CTL0                                            (Page_7 | 0x00B8)
  // (ReadWrite, Bits 0) 
  // control signal to choose oe signal 1'b0 oe signal from I2C 1'b1 oeandlocked signal
  #define BIT_MSK__TXPHY_PLL_CTL0__REG_ZONE_MASK_OE                                      0x01
  // (ReadWrite, Bits 2:1) 
  // Select frequency ratio betweeen hdmi_clk and pxl_clk 2'b00 0.5x mode, hdmi_clk_freq = 0.5*pxl_clk_freq 2'b01 1x mode, hdmi_clk_freq = 1*pxl_clk_freq (default) 2'b10 2x mode, hdmi_clk_freq = 2*pxl_clk_freq 2'b11 4x mode, hdmi_clk_freq = 4*pxl_clk_freq  For MHL2 Tx auto zone, it should be set to 2'b10. For all other cases, it should programmed to 2'b01
  #define BIT_MSK__TXPHY_PLL_CTL0__REG_HDMI_CLK_RATIO                                    0x06
  // (ReadWrite, Bits 6:3) 
  // Select frequency ratio between aud_clk and hdmi_clk 3'b000  aud_clk_freq = 5/1 hdmi_clk_freq 3'b001  aud_clk_freq = 5/2 hdmi_clk_freq 3'b010  aud_clk_freq = 5/3 hdmi_clk_freq 3'b011  aud_clk_freq = 5/5 hdmi_clk_freq 3'b100  aud_clk_freq = 5/2 hdmi_clk_freq 3'b101  aud_clk_freq = 5/4 hdmi_clk_freq 3'b110  aud_clk_freq = 5/6 hdmi_clk_freq 3'b111  aud_clk_freq = 5/10 hdmi_clk_freq
  #define BIT_MSK__TXPHY_PLL_CTL0__REG_AUD_CLK_RATIO                                     0x78
  // (ReadWrite, Bits 7) 
  // enable audio clock generation
  #define BIT_MSK__TXPHY_PLL_CTL0__REG_AUD_CLK_EN                                        0x80

// PHY Power Down Register
#define TXPHY_PLL_CTL2                                             (Page_7 | 0x00BA)                               
  // (ReadWrite, Bits 5) 
  // Datapath power switch enable 0  Power Down Datapath.  1  Power On Datapath.
  #define BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON_DP                                          0x20
  // (ReadWrite, Bits 7) 
  // Power on TX PLL
  #define BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON_PLL                                         0x10
  #define BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON  (BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON_DP | BIT_MSK__TXPHY_PLL_CTL2__REG_PWRON_PLL)
  #define BIT_MSK__TXPHY_PLL_CTL2__REG_PLL_LF_SEL                                        0x02   

// PHY TMDS Crl Register
#define TXPHY_TMDS_CTL                                            (Page_7 | 0x00F8)
  #define BIT_MSK__TXPHY_TMDS_CTL_REG_TMDS_OE                         0x10       


// HISC PD Control Register
#define REG_ADDR__HSIC_PD_CTL                                            (Page_2 | 0x0080)
  // (ReadWrite, Bits 0) 
  // Power Down enable for main PLL; 0: powerdown
  #define BIT_MSK__HSIC_PD_CTL__REG_PDB_MAIN                                          0x01
  // (ReadWrite, Bits 1) 
  // Power Down enable for IGEN
  #define BIT_MSK__HSIC_PD_CTL__REG_ENBALE_IGEN_MAIN                                  0x02
  // (ReadWrite, Bits 2) 
  // Charge Pump Current back up plan enable    =0 : cp current from igen (default case)    =1: cp current from resistive dividers.
  #define BIT_MSK__HSIC_PD_CTL__REG_SCPCAL_MAIN                                       0x04
  // (ReadWrite, Bits 3) 
  // Power Down enable for skew PLL
  #define BIT_MSK__HSIC_PD_CTL__REG_PDB_SKEW                                          0x08
  // (ReadWrite, Bits 4) 
  // enable Input Buffer 1 : Enable 0: Disable
  #define BIT_MSK__HSIC_PD_CTL__REG_EN_RX                                             0x10
  // (ReadWrite, Bits 5) 
  // power down 1: power down mode 0: normal operating mode
  #define BIT_MSK__HSIC_PD_CTL__REG_PDN_LDO                                           0x20

// HSIC VCOCAL Register
#define REG_ADDR__HSIC_VCOCAL                                            (Page_2 | 0x0081)
  // (ReadWrite, Bits 3:0) 
  //  Reserved for future use
  #define BIT_MSK__HSIC_VCOCAL__REG_VOCAL_MAIN                                        0x0F
  // (ReadWrite, Bits 6:4) 
  //  Reserved for future use
  #define BIT_MSK__HSIC_VCOCAL__REG_VCO_BIAS_MAIN                                     0x70

// HSIC Div Ctl Register
#define REG_ADDR__DIV_CTL_MAIN                                           (Page_2 | 0x0082)
  // (ReadWrite, Bits 1:0) 
  // main pll fb-divider ratio Bits     ratio 2'b00    50 2'b01    40 2'b10    32 2'b11    50
  #define BIT_MSK__DIV_CTL_MAIN__REG_FB_DIV_CTL_MAIN                                   0x03
  // (ReadWrite, Bits 4:2) 
  // main pll pre-divider ratio Bits       ratio 3'b000     1 3'b001     1 3'b010     1 3'b011     2 3'b1XX   20
  #define BIT_MSK__DIV_CTL_MAIN__REG_PRE_DIV_CTL_MAIN                                  0x1C

// HSIC ICP Ctl Register
#define REG_ADDR__ICP_CTL_MAIN                                           (Page_2 | 0x0083)
  // (ReadWrite, Bits 5:0) 
  //  Charge Pump current enable Bits in main PLL
  #define BIT_MSK__ICP_CTL_MAIN__REG_ICP_CTL_MAIN                                      0x3F

// HSIC Bias Bgr Register
#define REG_ADDR__BIAS_BGR                                               (Page_2 | 0x0084)
  // (ReadWrite, Bits 3:0) 
  // Control Bits of Band gap reference voltage in IGEN
  #define BIT_MSK__BIAS_BGR__REG_BIAS_BGR_D                                        0x0F
  // (ReadWrite, Bits 6:4) 
  // Current Calibration bits in IGEN
  #define BIT_MSK__BIAS_BGR__REG_VCOCAL_DEF_MAIN                                   0x70

// HSIC SPLL Bias Register
#define REG_ADDR__SPLL_BIAS                                              (Page_2 | 0x0085)
  // (ReadWrite, Bits 3:0) 
  // Reserved for future use
  #define BIT_MSK__SPLL_BIAS__REG_SPLLBIAS_MAIN                                     0x0F
  // (ReadWrite, Bits 5:4) 
  // lbw_main[1] is reserved. lbw_main[0] is used to select the reference voltage either  from bipolar or from resistive divider   0 : band gap voltage will be selected (default case)    1:  resisitve  divider output will be selected.
  #define BIT_MSK__SPLL_BIAS__REG_LBW_MAIN                                          0x30

// HSIC VCOCAL Skew Register
#define REG_ADDR__HSIC_VCOCAL_SKEW                                       (Page_2 | 0x0086)
  // (ReadWrite, Bits 3:0) 
  //  Reserved for future use
  #define BIT_MSK__HSIC_VCOCAL_SKEW__REG_VOCAL_SKEW                                        0x0F
  // (ReadWrite, Bits 7:4) 
  // Delay line control bits in skew PLL  [3] [2] [1] [0]     ---]      selected phases   x     0     0     0        ---] signal [0] and [4]  (default)   x     0     0     1        ---] signal [1] and [5]    x     0     1     0        ---] signal [2] and [6]     x     1     0     1        ---] signal [3] and [7]
  #define BIT_MSK__HSIC_VCOCAL_SKEW__REG_VCO_CTL_SKEW                                      0xF0

// HSIC Div Ctl Skew Register
#define REG_ADDR__DIV_CTL_SKEW                                           (Page_2 | 0x0087)
  // (ReadWrite, Bits 1:0) 
  // Decides fb-divider ratio in skew PLL Bits       ratio 2'b00     2 2'b01     4 2'b10     8 2'b11   16
  #define BIT_MSK__DIV_CTL_SKEW__REG_FB_DIV_CTL_SKEW                                   0x03
  // (ReadWrite, Bits 3:2) 
  // Decides Pre-divider ratio in skew PLL   Bits       ratio 2'b00   1 2'b01   2 2'b10   4 2'b11   8
  #define BIT_MSK__DIV_CTL_SKEW__REG_PRE_DIV_CTL_SKEW                                  0x0C

// HSIC ICP Ctl Skew Register
#define REG_ADDR__ICP_CTL_SKEW                                           (Page_2 | 0x0088)
  // (ReadWrite, Bits 5:0) 
  //  Charge Pump current enable Bits in skew PLL
  #define BIT_MSK__ICP_CTL_SKEW__REG_ICP_CTL_SKEW                                      0x3F

// HSIC Config Register
#define REG_ADDR__HSIC_CONFIG                                            (Page_2 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // I2C for PAD_hsic configuration; bit [7:1] == reservd  bit[0] == set LDO output level for burn in test 1: set LDO output to 1.5V for burn in test 0: normal operating mode
  #define BIT_MSK__HSIC_CONFIG__REG_CONFIG_HSIC                                       0xFF

// HSIC LDO CTL Register
#define REG_ADDR__LDO_CTL                                                (Page_2 | 0x008A)
  // (ReadWrite, Bits 1:0) 
  // sets LDO quiescent current [00]:  0.5mA [01]:  1.0mA [10]:  1.5mA [11]:  2.0mA
  #define BIT_MSK__LDO_CTL__REG_BLEED_CURRENT                                     0x03
  // (ReadWrite, Bits 2) 
  // select between internal and external compensation 1: selects internal compensation 0: selects external compensation
  #define BIT_MSK__LDO_CTL__REG_SEL_INT_COMP                                      0x04
  // (ReadWrite, Bits 3) 
  // sets opamp input stage bias current 1: nominal bias current 0: reduce bias current by 50%
  #define BIT_MSK__LDO_CTL__REG_SEL_OPAMP_BIASI                                   0x08
  // (ReadWrite, Bits 5:4) 
  // sets LDO output level [00]:  1.05V [01]: 1.10V [10]:  1.15V [11]:  1.20V
  #define BIT_MSK__LDO_CTL__REG_SEL_VREF                                          0x30

// HSIC Slew CTL Register
#define REG_ADDR__SLEW_CTL                                               (Page_2 | 0x008B)
  // (ReadWrite, Bits 3:0) 
  // hsic driver slew rate control
  #define BIT_MSK__SLEW_CTL__REG_SLEW_CTL                                          0x0F
  // (ReadWrite, Bits 6:4) 
  // Vref of Input  Buffer selection 100: 0.65*VddCore 010: 0.60*VddCore 001: 0.55*VddCore
  #define BIT_MSK__SLEW_CTL__REG_SEL_SLICE                                         0x70

// HSIC Clkdetect Register
#define REG_ADDR__CLKDETECT                                              (Page_2 | 0x008C)
  // (ReadOnly, Bits 0) 
  // Detects the clkin.  HIGH if f (clkin) ] 5MHz and pdb = HIGH -]  enables the PLL (charge pump, vcobias, pll_osc)
  #define BIT_MSK__CLKDETECT__REG_CLKDETECT_MAIN                                    0x01

// DPLL Configuration 1 Register
#define REG_ADDR__DPLL_CFG1                                              (Page_2 | 0x0090)
  // (ReadWrite, Bits 0) 
  // Selection of EQV between DEQ(0) and i2c(1)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_EV_SEL                                        0x01
  // (ReadWrite, Bits 1) 
  // Selection of BWV between DEQ(0) and i2c(1)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_BV_SEL                                        0x02
  // (ReadWrite, Bits 3:2) 
  // EQ scanning mode (default = 2'b00)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_MODE                                          0x0C
  // (ReadWrite, Bits 4) 
  // Enable external sync for the ease of testing (default = 1'b0)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_SPCE_EN                                       0x10
  // (ReadWrite, Bits 5) 
  // Auto engine decision enable (default = 1'b1)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_REG_EDON                                      0x20
  // (ReadWrite, Bits 7:6) 
  // LPF tap control (default = 2'b00)
  #define BIT_MSK__DPLL_CFG1__REG_CFG_TAPS                                          0xC0

// DPLL Configuration 2 Register
#define REG_ADDR__DPLL_CFG2                                              (Page_2 | 0x0091)
  // (ReadWrite, Bits 2:0) 
  // Bit count limit control (default = 3'b010)
  #define BIT_MSK__DPLL_CFG2__REG_CFG_BS                                            0x07
  // (ReadWrite, Bits 3) 
  // Enable bypass mode (default = 1'b0)
  #define BIT_MSK__DPLL_CFG2__REG_CFG_BYP                                           0x08
  // (ReadWrite, Bits 5:4) 
  // Bypass channel selection (default = 2'b00)
  #define BIT_MSK__DPLL_CFG2__REG_CFG_CHAN_SEL                                      0x30
  // (ReadWrite, Bits 7:6) 
  // Stable rescan mode (default = 2'b11)
  #define BIT_MSK__DPLL_CFG2__REG_CFG_STB_RSC                                       0xC0

// DPLL Configuration 3 Register
#define REG_ADDR__DPLL_CFG3                                              (Page_2 | 0x0092)
  // (ReadWrite, Bits 0) 
  // Enable extended EQ searching (default = 1'b1)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_EXT_EQ                                        0x01
  // (ReadWrite, Bits 1) 
  // Enable extended BW searching (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_EXT_BW                                        0x02
  // (ReadWrite, Bits 2) 
  // Eye monitor only at DE low period (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_DE_LO                                         0x04
  // (ReadWrite, Bits 3) 
  // Strength feature enable (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_STRTH                                         0x08
  // (ReadWrite, Bits 4) 
  // Tracking decition control (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_LOWLMT                                        0x10
  // (ReadWrite, Bits 5) 
  // Eye openness check control (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_VLDCHK                                        0x20
  // (ReadWrite, Bits 6) 
  // PLL B/W scanning enable (default = 1'b1)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_BWS_ON                                        0x40
  // (ReadWrite, Bits 7) 
  // Relaxed eye enable (default = 1'b0)
  #define BIT_MSK__DPLL_CFG3__REG_CFG_RLXE_ON                                       0x80

// DPLL Configuration 4 Register
#define REG_ADDR__DPLL_CFG4                                              (Page_2 | 0x0093)
  // (ReadWrite, Bits 5:0) 
  // Edge use control (default = 6'b011100)
  #define BIT_MSK__DPLL_CFG4__REG_CFG_W_CON                                         0x3F
  // (ReadWrite, Bits 6) 
  // Manual MHL gbox active high reset   0 - Normal mode; 1 - Reset (default = 1'b0)
  #define BIT_MSK__DPLL_CFG4__REG_CFG_RI_GBOX_RST                                   0x40
  // (ReadWrite, Bits 7) 
  // 1 - Use SCDT based reset in SSC FIFO reset and 0 - No use of SCDT based reset (default = 1'b1)
  #define BIT_MSK__DPLL_CFG4__SCDT_AUTO_RESET_EN                                    0x80

// DPLL ev_b Register
#define REG_ADDR__EV_VAL_B                                               (Page_2 | 0x0094)
  // (ReadWrite, Bits 7:0) 
  // User specified EQV via i2c of channel b
  #define BIT_MSK__EV_VAL_B__REG_CFG_EV_I2C_B                                      0xFF

// DPLL bv Register
#define REG_ADDR__BV_VAL                                                 (Page_2 | 0x0095)
  // (ReadWrite, Bits 7:0) 
  // User specified BWV via i2c 
  #define BIT_MSK__BV_VAL__REG_CFG_BV_I2C                                        0xFF

// PEQ_VAL0 Register
#define REG_ADDR__PEQ_VAL0                                               (Page_2 | 0x0096)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL0__REG_CFG_PEQ_VAL0                                      0xFF

// PEQ_VAL1 Register
#define REG_ADDR__PEQ_VAL1                                               (Page_2 | 0x0097)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL1__REG_CFG_PEQ_VAL1                                      0xFF

// PEQ_VAL2 Register
#define REG_ADDR__PEQ_VAL2                                               (Page_2 | 0x0098)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL2__REG_CFG_PEQ_VAL2                                      0xFF

// PEQ_VAL3 Register
#define REG_ADDR__PEQ_VAL3                                               (Page_2 | 0x0099)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL3__REG_CFG_PEQ_VAL3                                      0xFF

// PEQ_VAL4 Register
#define REG_ADDR__PEQ_VAL4                                               (Page_2 | 0x009A)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL4__REG_CFG_PEQ_VAL4                                      0xFF

// PEQ_VAL5 Register
#define REG_ADDR__PEQ_VAL5                                               (Page_2 | 0x009B)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL5__REG_CFG_PEQ_VAL5                                      0xFF

// PEQ_VAL6 Register
#define REG_ADDR__PEQ_VAL6                                               (Page_2 | 0x009C)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL6__REG_CFG_PEQ_VAL6                                      0xFF

// PEQ_VAL7 Register
#define REG_ADDR__PEQ_VAL7                                               (Page_2 | 0x009D)
  // (ReadWrite, Bits 7:0) 
  // One of EQVs used in DEQ 
  #define BIT_MSK__PEQ_VAL7__REG_CFG_PEQ_VAL7                                      0xFF

// PBW_VAL0 Register
#define REG_ADDR__PBW_VAL0                                               (Page_2 | 0x009E)
  // (ReadWrite, Bits 7:0) 
  // One of BWVs used in DEQ  
  #define BIT_MSK__PBW_VAL0__REG_CFG_PBW_VAL0                                      0xFF

// PBW_VAL1 Register
#define REG_ADDR__PBW_VAL1                                               (Page_2 | 0x009F)
  // (ReadWrite, Bits 7:0) 
  // One of BWVs used in DEQ  
  #define BIT_MSK__PBW_VAL1__REG_CFG_PBW_VAL1                                      0xFF

// PBW_VAL2 Register
#define REG_ADDR__PBW_VAL2                                               (Page_2 | 0x00A0)
  // (ReadWrite, Bits 7:0) 
  // One of BWVs used in DEQ 
  #define BIT_MSK__PBW_VAL2__REG_CFG_PBW_VAL2                                      0xFF

// PBW_VAL3 Register
#define REG_ADDR__PBW_VAL3                                               (Page_2 | 0x00A1)
  // (ReadWrite, Bits 7:0) 
  // One of BWVs used in DEQ 
  #define BIT_MSK__PBW_VAL3__REG_CFG_PBW_VAL3                                      0xFF

// DPLL Configuration 5 Register
#define REG_ADDR__DPLL_CFG5                                              (Page_2 | 0x00A2)
  // (ReadWrite, Bits 0) 
  // TMDS encoder bypass for 10b loop back (default = 1'b0)
  #define BIT_MSK__DPLL_CFG5__REG_CFG_RI_ENC_BYP                                    0x01
  // (ReadWrite, Bits 1) 
  // Enable SYNC recovery (default = 1'b1)
  #define BIT_MSK__DPLL_CFG5__REG_CFG_RI_RECOV_EN                                   0x02
  // (ReadWrite, Bits 2) 
  // Alternative control of MHL gear box(default = 1'b0)
  #define BIT_MSK__DPLL_CFG5__REG_CFG_RI_GBOX_ALT_CTL_EN                            0x04
  // (ReadWrite, Bits 5:3) 
  // SCDT and MHL MODE based MHL gear box reset control(default = 3'b111)
  #define BIT_MSK__DPLL_CFG5__REG_CFG_RI_GBOX_SCDT_RST                              0x38
  // (ReadWrite, Bits 7:6) 
  // Eye monitor chan selection (default = 2'd0)
  #define BIT_MSK__DPLL_CFG5__REG_CFG_RI_ECHAN_SEL                                  0xC0

// DPLL Configuration 6 Register
#define REG_ADDR__DPLL_CFG6                                              (Page_2 | 0x00A3)
  // (ReadWrite, Bits 0) 
  // Eye monitor enable (default = 1'b0)
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_EEVAL                                      0x01
  // (ReadWrite, Bits 3:1) 
  // Eye monitor byte selection (default = 3'd0)
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_EBYTE_SEL                                  0x0E
  // (ReadWrite, Bits 4) 
  // 1 - Reset zone control logic when SCDT is 0; 0 - No use SCDT (default = 1'b0)
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_USE_SCDT                                   0x10
  // (ReadWrite, Bits 5) 
  // Stop bp movement (default = 1'b0)
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_BP_FIX                                     0x20
  // (ReadWrite, Bits 6) 
  // 1 - Zone 2x is not used in zone decision; Only 1x and 4x are used; 0 - Zone 2x is used as well(default = 1'b0
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_NO_ZONE2X                                  0x40
  // (ReadWrite, Bits 7) 
  // TMDS (q0; q1; q2) bit order swap (default = 1'b0)
  #define BIT_MSK__DPLL_CFG6__REG_CFG_RI_SWAP                                       0x80

// GBOX RESET 1 Register
#define REG_ADDR__GBOX_RST1                                              (Page_2 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // Gear box reset length when ri_gbox_scdt_rst is 1(default = 16'h4e20 (16'd20000) for 10ms) *In simulation; you have to reduce this; for example; to 0.5ms (16'h3ff) or less to reduce the simulation time
  #define BIT_MSK__GBOX_RST1__REG_CFG_RI_GBOX_RST_LEN_B7_B0                         0xFF

// GBOX RESET 2 Register
#define REG_ADDR__GBOX_RST2                                              (Page_2 | 0x00A5)
  // (ReadWrite, Bits 7:0) 
  // Gear box reset length when ri_gbox_scdt_rst is 1(default = 16'h4e20 (16'd20000) for 10ms) *In simulation; you have to reduce this; for example; to 0.5ms (16'h3ff) or less to reduce the simulation time
  #define BIT_MSK__GBOX_RST2__REG_CFG_RI_GBOX_RST_LEN_B15_B8                        0xFF

// ZONE CTRL RST 1 Register
#define REG_ADDR__ZCTRL_RST1                                             (Page_2 | 0x00A6)
  // (ReadWrite, Bits 7:0) 
  // 0 - Use CKDT without any delay to reset zone control n - Use CKDT (and SCDT) with given delay n * 256us (default = 12'd5)
  #define BIT_MSK__ZCTRL_RST1__REG_CFG_RI_ZCTL_RST_DLY_B7_B0                         0xFF

// ZONE CTRL RST 2 Register
#define REG_ADDR__ZCTRL_RST2                                             (Page_2 | 0x00A7)
  // (ReadWrite, Bits 3:0) 
  // 0 - Use CKDT without any delay to reset zone control n - Use CKDT (and SCDT) with given delay n * 256us (default = 12'd5)
  #define BIT_MSK__ZCTRL_RST2__REG_CFG_RI_ZCTL_RST_DLY_B11_B8                        0x0F
  // (ReadWrite, Bits 4) 
  // DC compensation (Alice scheme) enable (default = 1'b0)
  #define BIT_MSK__ZCTRL_RST2__REG_CFG_RI_DC_CP_EN                                   0x10
  // (ReadWrite, Bits 7:5) 
  // Eye monitor use decision matrix (default = 3'b100)
  #define BIT_MSK__ZCTRL_RST2__REG_CFG_RI_DEC_CON                                    0xE0

// DPLL Configuration 7 Register
#define REG_ADDR__DPLL_CFG7                                              (Page_2 | 0x00A8)
  // (ReadWrite, Bits 1:0) 
  // Delay of div20sync going to decimator. 2'b00 is the minimum delay and 2'b11 is the maximum (default = 2'b00)
  #define BIT_MSK__DPLL_CFG7__REG_CFG_RI_DIV20_CTRL                                 0x03
  // (ReadWrite, Bits 2) 
  // Use of DE glitch in EQ scanning (default = 1'b0)
  #define BIT_MSK__DPLL_CFG7__REG_CFG_GLT_ON                                        0x04
  // (ReadWrite, Bits 4:3) 
  // PLL mode value (default = 2'b01) 1X MHL mode
  #define BIT_MSK__DPLL_CFG7__REG_CFG_RI_PLL_MODE_I2C                               0x18
  // (ReadWrite, Bits 6:5) 
  // Disable/Enable reset per CKDT and PLL lock (default = 2'b10)
  #define BIT_MSK__DPLL_CFG7__REG_CFG_RI_RST_CON                                    0x60
  // (ReadWrite, Bits 7) 
  // Align DIV3 to SYNC (default = 1'b0)
  #define BIT_MSK__DPLL_CFG7__REG_CFG_RI_SYNC4DIV3                                  0x80

// OLOOP 1 Register
#define REG_ADDR__OLOOP1                                                 (Page_2 | 0x00A9)
  // (ReadWrite, Bits 7:0) 
  // PLL open loop settling time (default = 17'd0)
  #define BIT_MSK__OLOOP1__REG_CFG_RI_OLOOP_STIME_B7_B0                          0xFF

// OLOOP 2 Register
#define REG_ADDR__OLOOP2                                                 (Page_2 | 0x00AA)
  // (ReadWrite, Bits 7:0) 
  // PLL open loop settling time (default = 17'd0)
  #define BIT_MSK__OLOOP2__REG_CFG_RI_OLOOP_STIME_B15_B8                         0xFF

// ZONE CTRL 1 Register
#define REG_ADDR__ZCTRL_1                                                (Page_2 | 0x00AB)
  // (ReadWrite, Bits 0) 
  // PLL open loop settling time (default = 17'd0)
  #define BIT_MSK__ZCTRL_1__REG_CFG_RI_OLOOP_STIME_B16                            0x01
  // (ReadWrite, Bits 4:1) 
  // Zone control parameters (default = 4'b1000) [3 : 2] - zonectl_mode; [1] - strict; [0] - longer
  #define BIT_MSK__ZCTRL_1__REG_CFG_RI_ZONE_CTRL                                  0x1E
  // (ReadWrite, Bits 7:5) 
  // szone value (default = 3'b001)
  #define BIT_MSK__ZCTRL_1__REG_CFG_RI_SZONE_I2C                                  0xE0

// ALT ZONE CTRL 1 Register
#define REG_ADDR__ALT_Z_CTRL1                                            (Page_2 | 0x00AC)
  // (ReadWrite, Bits 7:0) 
  // Alternative zone controller control registers from i2c [7:0]
  #define BIT_MSK__ALT_Z_CTRL1__REG_CFG_RI_ALLW_CL_DIV4_B7_B0                         0xFF

// ALT ZONE CTRL 2 and 1x support Register
#define REG_ADDR__ALT_Z_CTRL2                                            (Page_2 | 0x00AD)
  // (ReadWrite, Bits 2:0) 
  // Alternative zone controller control registers from i2c [10:8]
  #define BIT_MSK__ALT_Z_CTRL2__REG_CFG_RI_ALLW_CL_DIV4_B10_B8                        0x07
  // (ReadWrite, Bits 3) 
  // MHL 1X mode (default 1 to support Link clock at 1x)
  #define BIT_MSK__ALT_Z_CTRL2__MHL1X_MODE                                            0x08
  // (ReadWrite, Bits 4) 
  // auto zone support for 1x Link clock
  #define BIT_MSK__ALT_Z_CTRL2__REG_RI_AUTO_ZONE_MHL1X                                0x10
  // (ReadWrite, Bits 5) 
  // Always in 1x zone control mode (0 for autozone control)
  #define BIT_MSK__ALT_Z_CTRL2__REG_RI_ALWAYS_1X_MHL1X                                0x20
  // (ReadWrite, Bits 6) 
  // 1 - Zone 8x is not used in zone decision;
  #define BIT_MSK__ALT_Z_CTRL2__REG_CFG_RI_NO_ZONE8X                                  0x40
  // (ReadWrite, Bits 7) 
  // 1 - Use new alternative MHL DP (default)  0 - Use conventional MHL DP Even if it's 0 for PP mode; HW automatically uses  new MHL DP
  #define BIT_MSK__ALT_Z_CTRL2__REG_CFG_RI_USE_NEW_MHL                                0x80

// CL REF 1 Register
#define REG_ADDR__CL_REF_CLK1                                            (Page_2 | 0x00AE)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__CL_REF_CLK1__REG_CFG_RI_ALLW_CL_REF_B7_B0                          0xFF

// CL REF 2 Register
#define REG_ADDR__CL_REF_CLK2                                            (Page_2 | 0x00AF)
  // (ReadWrite, Bits 2:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__CL_REF_CLK2__REG_CFG_RI_ALLW_CL_REF_B10_B8                         0x07
  // (ReadWrite, Bits 5) 
  //  
  #define BIT_MSK__CL_REF_CLK2__REG_CFG_RI_ENH_HOP_SM                                 0x20
  // (ReadWrite, Bits 6) 
  //  
  #define BIT_MSK__CL_REF_CLK2__REG_CFG_RI_HOP_SM_IN_SCAN                             0x40
  // (ReadWrite, Bits 7) 
  //  
  #define BIT_MSK__CL_REF_CLK2__REG_CFG_RI_IGN_REOPEN                                 0x80

// OL DIV4 1 Register
#define REG_ADDR__OL_DIV4CLK1                                            (Page_2 | 0x00B0)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__OL_DIV4CLK1__REG_CFG_RI_ALLW_OL_DIV4_B7_B0                         0xFF

// OL DIV4 2 Register
#define REG_ADDR__OL_DIV4CLK2                                            (Page_2 | 0x00B1)
  // (ReadWrite, Bits 2:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__OL_DIV4CLK2__REG_CFG_RI_ALLW_OL_DIV4_B10_B8                        0x07

// OL REF 1 Register
#define REG_ADDR__OL_REF_CLK1                                            (Page_2 | 0x00B2)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__OL_REF_CLK1__REG_CFG_RI_ALLW_OL_REF_B7_B0                          0xFF

// OL REF 2 Register
#define REG_ADDR__OL_REF_CLK2                                            (Page_2 | 0x00B3)
  // (ReadWrite, Bits 2:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__OL_REF_CLK2__REG_CFG_RI_ALLW_OL_REF_B10_B8                         0x07

// ZONE VAR 1 Register
#define REG_ADDR__ZONE_VAR1                                              (Page_2 | 0x00B4)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__ZONE_VAR1__REG_CFG_RI_ALLW_ZONE_VAR_B7_B0                        0xFF

// ZONE VAR 2 Register
#define REG_ADDR__ZONE_VAR2                                              (Page_2 | 0x00B5)
  // (ReadWrite, Bits 2:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__ZONE_VAR2__REG_CFG_RI_ALLW_ZONE_VAR_B10_B8                       0x07
  // (ReadWrite, Bits 7:5) 
  // Align turn on/off for 10b BIST
  #define BIT_MSK__ZONE_VAR2__REG_CFG_RI_CTR_ALIGN_OFF                              0xE0

// N RUN CL Register
#define REG_ADDR__N_RUN_CL                                               (Page_2 | 0x00B6)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__N_RUN_CL__REG_CFG_RI_N_RUN_CL                                   0xFF

// N RUN OL Register
#define REG_ADDR__N_RUN_OL                                               (Page_2 | 0x00B7)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__N_RUN_OL__REG_CFG_RI_N_RUN_OL                                   0xFF

// N IDLE Register
#define REG_ADDR__N_IDLE                                                 (Page_2 | 0x00B8)
  // (ReadWrite, Bits 7:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__N_IDLE__REG_CFG_RI_N_IDLING                                   0xFF

// CMP MON Register
#define REG_ADDR__CMO_MON_PRD                                            (Page_2 | 0x00B9)
  // (ReadWrite, Bits 3:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__CMO_MON_PRD__REG_CFG_RI_CMP_PERD                                   0x0F
  // (ReadWrite, Bits 7:4) 
  // (default = TBD per characterization)
  #define BIT_MSK__CMO_MON_PRD__REG_CFG_RI_MON_PERD                                   0xF0

// INIT DLY Register
#define REG_ADDR__INT_PRMBL                                              (Page_2 | 0x00BA)
  // (ReadWrite, Bits 3:0) 
  // (default = TBD per characterization)
  #define BIT_MSK__INT_PRMBL__REG_CFG_RI_INIT_DLY                                   0x0F
  // (ReadWrite, Bits 7:4) 
  // # of DI preamble for auto 1080p mode detection Max is 8 (default = 4'd6)
  #define BIT_MSK__INT_PRMBL__REG_CFG_RI_PRMBL_THRSHLD                              0xF0

// DPLL MISC Register
#define REG_ADDR__DPLL_MISC                                              (Page_2 | 0x00BB)
  // (ReadWrite, Bits 0) 
  // 1 - Enable additional BCH code to MHL 2x mode 0 - No BCH code (default = 1'b0) 
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_BCH_CODE_EN                                0x01
  // (ReadWrite, Bits 1) 
  // 1 - Auto detection of 1080p (2x) mode 0 - Manual mode Auto mode is removed from MHL spec So this should be 1'b0 (default = 1'b0)
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_M1080P_DET_EN                              0x02
  // (ReadWrite, Bits 2) 
  // mode_1080p value from user. If m1080p_det_en is 0; m1080p_val becomes mode_1080p output       (default = 1'b0; i.e. 3X mode)
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_M1080P_VAL                                 0x04
  // (ReadWrite, Bits 3) 
  // overriding m1080p_val using register method (default from CBUS status register) default is disable
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_M1080P_VAL_OVR                             0x08
  // (ReadWrite, Bits 4) 
  // 0 - Use external div20sync (current scheme) 1 - Use internally generated one (new scheme) (default = 1'b1) 
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_USE_INT_DIV20                              0x10
  // (ReadWrite, Bits 5) 
  // Zone control algorithm selection (default = 1'b0) 0 - Current one; 1 - Alternative one 
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_ZONE_ALG_SEL                               0x20
  // (ReadWrite, Bits 6) 
  // 0 - Eye is not considered in Zone/EQ reseting 1 - Iff eye = 0s; we reset Zone and EQ (default = 1'b1)
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_ZONE_EQ_RST_CON                            0x40
  // (ReadWrite, Bits 7) 
  // Zone controller and EQ controller reset enable per the scan value (default = 1'b0) 
  #define BIT_MSK__DPLL_MISC__REG_CFG_RI_ZONE_EQ_RST_EN                             0x80

// DPLL STATUS1 Register
#define REG_ADDR__DPLL_STATUS1                                           (Page_2 | 0x00BC)
  // (ReadOnly, Bits 4:0) 
  // Monitoring bp for debugging 
  #define BIT_MSK__DPLL_STATUS1__RO_BP_OUT_B                                           0x1F
  // (ReadOnly, Bits 5) 
  // Engine decision output of channel B 
  #define BIT_MSK__DPLL_STATUS1__RO_B_EDOUT                                            0x20

// DPLL STATUS2 Register
#define REG_ADDR__DPLL_STATUS2                                           (Page_2 | 0x00BD)
  // (ReadOnly, Bits 6:5) 
  // PLL mode 
  #define BIT_MSK__DPLL_STATUS2__RO_PLL_MODE_OUT                                       0x60
  // (ReadOnly, Bits 7) 
  // MHL SYNC monitor
  #define BIT_MSK__DPLL_STATUS2__RO_SYNC_MON                                           0x80

// DPLL STATUS3 Register
#define REG_ADDR__DPLL_STATUS3                                           (Page_2 | 0x00BE)
  // (ReadOnly, Bits 5) 
  // EQ scan stable indicator of channel_b
  #define BIT_MSK__DPLL_STATUS3__RO_STABLE_O_B                                         0x20

// DPLL STATUS4 Register
#define REG_ADDR__DPLL_STATUS4                                           (Page_2 | 0x00BF)
  // (ReadOnly, Bits 3:0) 
  // Eye open info for B 
  #define BIT_MSK__DPLL_STATUS4__RO_EOPEN_B                                            0x0F

// DPLL STATUS5 Register
#define REG_ADDR__DPLL_STATUS5                                           (Page_2 | 0x00C0)
  // (ReadOnly, Bits 7:5) 
  // zone control value
  #define BIT_MSK__DPLL_STATUS5__RO_SZONE                                              0xE0

// DPLL STATUS6 Register
#define REG_ADDR__DPLL_STATUS6                                           (Page_2 | 0x00C1)
  // (ReadOnly, Bits 7:0) 
  // PLL B/W value from DEQ
  #define BIT_MSK__DPLL_STATUS6__RO_BV_DEQ                                             0xFF

// DPLL STATUS7 Register
#define REG_ADDR__DPLL_STATUS7                                           (Page_2 | 0x00C2)
  // (ReadOnly, Bits 7:0) 
  // PLL EQ value of channel b from DEQ
  #define BIT_MSK__DPLL_STATUS7__RO_EV_DEQ_B                                           0xFF

// DPLL STATUS10 Register
#define REG_ADDR__DPLL_STATUS10                                          (Page_2 | 0x00C3)
  // (ReadOnly, Bits 7:0) 
  // Current scanning indicator of channel b
  #define BIT_MSK__DPLL_STATUS10__RO_SCAN_VAL_B                                         0xFF

// Calibration OTP Oscillator Compare Register
#define REG_ADDR__DPLL_STATUS13                                          (Page_2 | 0x00C4)
  // (ReadOnly, Bits 7:0) 
  // Internal eye monitor
  #define BIT_MSK__DPLL_STATUS13__RO_EDGE_DET                                           0xFF

// DPLLMHL3 OFIFO ERR Low Byte Register
#define REG_ADDR__DPLLM3_OFEL                                            (Page_2 | 0x00C5)
  // (ReadOnly, Bits 7:0) 
  // OSD FIFO error report
  #define BIT_MSK__DPLLM3_OFEL__RO_DPLL_OFIFO_ERR_B7_B0                               0xFF

// DPLLMHL3 OFIFO ERR High Byte Register
#define REG_ADDR__DPLLM3_OFEH                                            (Page_2 | 0x00C6)
  // (ReadOnly, Bits 3:0) 
  // OSD FIFO error report
  #define BIT_MSK__DPLLM3_OFEH__RO_DPLL_OFIFO_ERR_B11_B8                              0x0F

// DPLLMHL3 CFG Register
#define REG_ADDR__DPLLM3_CFG                                             (Page_2 | 0x00C7)
  // (ReadWrite, Bits 0) 
  // MHL3(1)/other(0) overwriting when ri_mhl3m_ovr is 1
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_MHL3M_VAL                                    0x01
  // (ReadWrite, Bits 1) 
  // 1 - Overwirte MHL3 mode w/ ri_mhl3m_val   0 - MHL3 based on external input (default)
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_MHL3M_OVR                                    0x02
  // (ReadWrite, Bits 2) 
  // ri_algn_4sync
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_ALGN_4SYNC                                   0x04
  // (ReadWrite, Bits 4:3) 
  // 0 - MHL3 1 lane; 1 - MHL3 2 lanes; 2 - MHL3 3 lanes
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_MHL3_NLN                                     0x18
  // (ReadWrite, Bits 5) 
  // Control the bit order for 8b10b decoder input   0 - No swap (default) and 1 - Swap
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_8B10B_SWAP                                   0x20
  // (ReadWrite, Bits 7:6) 
  // AV link code in MHL3; 1 - 8b10b and 0 - TMDS
  #define BIT_MSK__DPLLM3_CFG__REG_DPLL_8B10B_CODE                                   0xC0

// DPLLMHL3 BP FIX B Register
#define REG_ADDR__DPLLM3_BP_B                                            (Page_2 | 0x00C8)
  // (ReadWrite, Bits 4:0) 
  // bp fix value of b channel
  #define BIT_MSK__DPLLM3_BP_B__REG_DPLL_BP_FIX_VAL_B                                 0x1F

// DPLLMHL3 BP FIX G Register
#define REG_ADDR__DPLLM3_BP_G                                            (Page_2 | 0x00C9)
  // (ReadWrite, Bits 4:0) 
  // bp fix value of g channel
  #define BIT_MSK__DPLLM3_BP_G__REG_DPLL_BP_FIX_VAL_G                                 0x1F

// DPLLMHL3 BP FIX R Register
#define REG_ADDR__DPLLM3_BP_R                                            (Page_2 | 0x00CA)
  // (ReadWrite, Bits 4:0) 
  // bp fix value of r channel
  #define BIT_MSK__DPLLM3_BP_R__REG_DPLL_BP_FIX_VAL_R                                 0x1F

// DPLLMHL3 VV Register
#define REG_ADDR__DPLLM3_VV_I2C                                          (Page_2 | 0x00CB)
  // (ReadWrite, Bits 7:0) 
  // User specified PI via i2c 
  #define BIT_MSK__DPLLM3_VV_I2C__REG_DPLL_VV_I2C                                       0xFF

// DPLLMHL3 VV CTL Register
#define REG_ADDR__DPLLM3_VV_CTL                                          (Page_2 | 0x00CC)
  // (ReadWrite, Bits 0) 
  // Selection of PI between DEQ(0) and i2c(1) 
  #define BIT_MSK__DPLLM3_VV_CTL__REG_DPLL_VV_SEL                                       0x01
  // (ReadWrite, Bits 1) 
  // PLL PI scanning enable 
  #define BIT_MSK__DPLLM3_VV_CTL__REG_DPLL_VTS_ON                                       0x02
  // (ReadWrite, Bits 2) 
  // Enable extended PI searching 
  #define BIT_MSK__DPLLM3_VV_CTL__REG_DPLL_EXT_VTS                                      0x04

// DPLLMHL3 PVT VAL0 Register
#define REG_ADDR__DPLLM3_PVT0                                            (Page_2 | 0x00CD)
  // (ReadWrite, Bits 7:0) 
  // one of Pis used in DEQ
  #define BIT_MSK__DPLLM3_PVT0__REG_DPLL_PVT_VAL0                                     0xFF

// DPLLMHL3 PVT VAL1 Register
#define REG_ADDR__DPLLM3_PVT1                                            (Page_2 | 0x00CE)
  // (ReadWrite, Bits 7:0) 
  // one of Pis used in DEQ
  #define BIT_MSK__DPLLM3_PVT1__REG_DPLL_PVT_VAL1                                     0xFF

// DPLLMHL3 PVT VAL2 Register
#define REG_ADDR__DPLLM3_PVT2                                            (Page_2 | 0x00CF)
  // (ReadWrite, Bits 7:0) 
  // one of Pis used in DEQ
  #define BIT_MSK__DPLLM3_PVT2__REG_DPLL_PVT_VAL2                                     0xFF

// DPLLMHL3 PVT VAL3 Register
#define REG_ADDR__DPLLM3_PVT3                                            (Page_2 | 0x00D0)
  // (ReadWrite, Bits 7:0) 
  // one of Pis used in DEQ
  #define BIT_MSK__DPLLM3_PVT3__REG_DPLL_PVT_VAL3                                     0xFF

// DPLLMHL3 MISC0 Register
#define REG_ADDR__DPLLM3_MISC0                                           (Page_2 | 0x00D1)
  // (ReadWrite, Bits 0) 
  // DEQ scan order change 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_EQ_SCAN_ORD                                  0x01
  // (ReadWrite, Bits 1) 
  // Use extra interlane deskewer 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_USE_DESKEW                                   0x02
  // (ReadWrite, Bits 2) 
  // OSD FIFO error recovery scheme 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_ERR_RCV                                      0x04
  // (ReadWrite, Bits 3) 
  // OSD FIFO alternative control enable 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_ALT_CTL_EN                                   0x08
  // (ReadWrite, Bits 4) 
  // OSD FIFO use (1) or not use (0) 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_USE_OSD_BUF                                  0x10
  // (ReadWrite, Bits 5) 
  // OSD FIFO recovery mechanism 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_REC_SEL                                      0x20
  // (ReadWrite, Bits 6) 
  // Load o_osd_ext_* for analog tracking 
  #define BIT_MSK__DPLLM3_MISC0__REG_DPLL_OPEN_OBS                                     0x40

// DPLLMHL3 SZONE Register
#define REG_ADDR__DPLLM3_SZONE                                           (Page_2 | 0x00D2)
  // (ReadWrite, Bits 0) 
  // 1 - Overwriting ao_szone w/ val_ao_szone (MHL3)      0 - No overwriting (from i2c; default = 1'b0) 
  #define BIT_MSK__DPLLM3_SZONE__REG_DPLL_OVR_AO_SZONE                                 0x01
  // (ReadWrite, Bits 3:1) 
  // ao_szone value from i2c
  #define BIT_MSK__DPLLM3_SZONE__REG_DPLL_VAL_AO_SZONE                                 0x0E

// DPLL BW CTRL 1 Register
#define REG_ADDR__DPLL_BW_CFG1                                           (Page_2 | 0x00D3)
  // (ReadWrite, Bits 3:0) 
  // VCOCAL_DEF for MHL  mode (default = 4'b0010)
  #define BIT_MSK__DPLL_BW_CFG1__RI_VCOCAL_DEF_MHL1X                                   0x0F
  // (ReadWrite, Bits 7:4) 
  // VCOCAL_DEF for HDMI mode (default = 4'b0010)
  #define BIT_MSK__DPLL_BW_CFG1__RI_VCOCAL_DEF_HDMI                                    0xF0

// DPLL BW CTRL 2 Register
#define REG_ADDR__DPLL_BW_CFG2                                           (Page_2 | 0x00D4)
  // (ReadWrite, Bits 0) 
  // 1 - Different BW set for MHL and 0 - Same BW set for both of MHL and HDMI (default)
  #define BIT_MSK__DPLL_BW_CFG2__RI_EN_MHL1X_C                                         0x01
  // (ReadWrite, Bits 1) 
  // 1 - Enable to use MEQs in MHL mode and 0 - Always use PEQ regardless of MHL or HDMI
  #define BIT_MSK__DPLL_BW_CFG2__RI_MHL1X_EQ_EN                                        0x02
  // (ReadWrite, Bits 2) 
  // Drives the bit[8] of dpll_eq_ctl_b output of Sherman Digital
  #define BIT_MSK__DPLL_BW_CFG2__RI_EQ_CTL_B                                           0x04

// MEQ_VAL0 Register
#define REG_ADDR__MEQ_VAL0                                               (Page_2 | 0x00D5)
  // (ReadWrite, Bits 7:0) 
  // 1st EQ value in MHL Default value as of now is 8'h20 
  #define BIT_MSK__MEQ_VAL0__REG_CFG_MEQ_VAL0                                      0xFF

// MEQ_VAL1 Register
#define REG_ADDR__MEQ_VAL1                                               (Page_2 | 0x00D6)
  // (ReadWrite, Bits 7:0) 
  // 2nd EQ value in MHL Default value as of now is 8'h40 
  #define BIT_MSK__MEQ_VAL1__REG_CFG_MEQ_VAL1                                      0xFF

// MEQ_VAL2 Register
#define REG_ADDR__MEQ_VAL2                                               (Page_2 | 0x00D7)
  // (ReadWrite, Bits 7:0) 
  // 3rd EQ value in MHL Default value as of now is 8'h33 
  #define BIT_MSK__MEQ_VAL2__REG_CFG_MEQ_VAL2                                      0xFF

// MEQ_VAL3 Register
#define REG_ADDR__MEQ_VAL3                                               (Page_2 | 0x00D8)
  // (ReadWrite, Bits 7:0) 
  // 4th EQ value in MHL Default value as of now is 8'h43 
  #define BIT_MSK__MEQ_VAL3__REG_CFG_MEQ_VAL3                                      0xFF

// DPLL MISC2 Register
#define REG_ADDR__DPLL_MISC2                                             (Page_2 | 0x00D9)
  // (ReadWrite, Bits 0) 
  // 1: MHL1x mode and 0: other mode in overwriting (default).
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_MHL1X_MODE_VAL                             0x01
  // (ReadWrite, Bits 1) 
  // 1: MHLpp mode and 0: other mode in overwriting (default).
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_MHLPP_MODE_VAL                             0x02
  // (ReadWrite, Bits 2) 
  // 1: Overwrite MHL mode (1x or PP) using ri_mhl**_mode_val. 0: MHL mode based on CBUS (default).
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_MHL_MODE_OVR                               0x04
  // (ReadWrite, Bits 3) 
  // HDMI(0: default)/MHL(1) overwriting value when ri_hdmim_ovr is 1.
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_HDMIM_VAL                                  0x08
  // (ReadWrite, Bits 4) 
  // 1: Overwrite MHL/HDMI mode w/ ri_hdmim_val. 0: MHL/HDMI based on CBUS HW (default).
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_HDMIM_OVR                                  0x10
  // (ReadWrite, Bits 6:5) 
  // ao_mhl_test and ao_mhl_pp_en overwriting value (default=2'b00). [1]: ao_mhl_test. [0]: ao_mhl_pp_en
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_PLLCON_VAL                                 0x60
  // (ReadWrite, Bits 7) 
  // 1: Overwriting ao_mhl_test and ao_mhl_pp_en w/ ri_pllcon_val. 0: No overwriting (default).
  #define BIT_MSK__DPLL_MISC2__REG_CFG_RI_PLLCON_OVR                                 0x80

// DPLL HDMI2 Register
#define REG_ADDR__DPLL_HDMI2                                             (Page_2 | 0x00DA)
  // (ReadWrite, Bits 0) 
  // 1: HDMI2 ON Overwrite with ri_hdmi2_on_val 0: HDMI2 ON value from CBUS/Top
  #define BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR                                       0x01
  // (ReadWrite, Bits 1) 
  // HDMI2 ON value. Considered only when ri_hdmi2_on_ovr is HIGH.
  #define BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL                                       0x02
  // (ReadWrite, Bits 2) 
  // 1: Scramble ON Overwrite with ri_scramble_on_val 0: Scramble ON value from CBUS/Top
  #define BIT_MSK__DPLL_HDMI2__RI_SCRAMBLE_ON_OVR                                    0x04
  // (ReadWrite, Bits 3) 
  // Scramble ON value. Considered only when ri_scramble_on_ovr is HIGH.
  #define BIT_MSK__DPLL_HDMI2__RI_SCRAMBLE_ON_VAL                                    0x08
  // (ReadWrite, Bits 4) 
  // Scrambler on only after detecting scrambled stat.
  #define BIT_MSK__DPLL_HDMI2__RI_STAT_AND_ON                                        0x10
  // (ReadWrite, Bits 5) 
  // hdmi2 scdt on
  #define BIT_MSK__DPLL_HDMI2__RI_HDMI2_SCDT_ON                                      0x20

// DPLL PI Control Register
#define REG_ADDR__DPLL_VV_DEQ                                            (Page_2 | 0x00DB)
  // (ReadOnly, Bits 7:0) 
  // Contains the PI Control value from DEQ
  #define BIT_MSK__DPLL_VV_DEQ__RO_VV_DEQ                                             0xFF

// DPLL IPS VAL0 Register
#define REG_ADDR__DPLL_IPS_VAL0                                          (Page_2 | 0x00DC)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL0__RI_IPS_VAL0                                           0xFF

// DPLL IPS VAL1 Register
#define REG_ADDR__DPLL_IPS_VAL1                                          (Page_2 | 0x00DD)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL1__RI_IPS_VAL1                                           0xFF

// DPLL IPS VAL2 Register
#define REG_ADDR__DPLL_IPS_VAL2                                          (Page_2 | 0x00DE)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL2__RI_IPS_VAL2                                           0xFF

// DPLL IPS VAL3 Register
#define REG_ADDR__DPLL_IPS_VAL3                                          (Page_2 | 0x00DF)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL3__RI_IPS_VAL3                                           0xFF

// DPLL IPS VAL4 Register
#define REG_ADDR__DPLL_IPS_VAL4                                          (Page_2 | 0x00E0)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL4__RI_IPS_VAL4                                           0xFF

// DPLL IPS VAL5 Register
#define REG_ADDR__DPLL_IPS_VAL5                                          (Page_2 | 0x00E1)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__DPLL_IPS_VAL5__RI_IPS_VAL5                                           0xFF

// DPLL Control0 Register
#define REG_ADDR__DPLL_CTRL0                                             (Page_2 | 0x00E2)
  // (ReadWrite, Bits 1:0) 
  // 
  #define BIT_MSK__DPLL_CTRL0__RI_IPSCON                                             0x03
  // (ReadWrite, Bits 4:2) 
  // 
  #define BIT_MSK__DPLL_CTRL0__RI_CLR_ERRCNT                                         0x1C
  // (ReadWrite, Bits 7:5) 
  // 
  #define BIT_MSK__DPLL_CTRL0__RI_CLR_LOCKED                                         0xE0

// DPLL Control1 Register
#define REG_ADDR__DPLL_CTRL1                                             (Page_2 | 0x00E3)
  // (ReadWrite, Bits 0) 
  // 
  #define BIT_MSK__DPLL_CTRL1__RI_PERIOD_SEL                                         0x01
  // (ReadWrite, Bits 7:1) 
  // 
  #define BIT_MSK__DPLL_CTRL1__RI_PERIOD_CON                                         0xFE

// DPLL Control2 Register
#define REG_ADDR__DPLL_CTRL2                                             (Page_2 | 0x00E4)
  // (ReadWrite, Bits 0) 
  // 
  #define BIT_MSK__DPLL_CTRL2__RI_USE_INV_CLK                                        0x01
  // (ReadWrite, Bits 1) 
  // DPLL IC Soft Reset: This bit controls the Soft Reset of the DPLL for HDMI2 and MHL3 IC Reset. Polarity of this bit in the logic is an Active LOW but the description below is straight forward. 0 - DPLL IC Reset asserted 1 - DPLL IC Reset deasserted
  #define BIT_MSK__DPLL_CTRL2__RI_SW_ICRST_N                                         0x02
  // (ReadWrite, Bits 6:4) 
  // Scrambler Control
  #define BIT_MSK__DPLL_CTRL2__RI_SCRAMBLER_CON                                      0x70

// DPLL Channel Control Register
#define REG_ADDR__DPLL_CH_CTRL                                           (Page_2 | 0x00E5)
  // (ReadWrite, Bits 1:0) 
  // Channel Swap: This field controls interpretation of the colour per DPLL input Channel. Value programmed into this register indicates the respective colour of the data received on the respective TMDS lane. The Channel swapping is as per the below values. Value - Ch0  Ch1  Ch2 2'b00 -  B      G       R 2'b01 -  R      B       G 2'b1x -  G      R       B
  #define BIT_MSK__DPLL_CH_CTRL__RI_D_CH_SWAP                                          0x03
  // (ReadWrite, Bits 2) 
  // support flappable connector for MHL3 one lane configuration. 0-Data over CH0(normal mode) (default = 1'b0) 1-Data over CH1(flipping mode)
  #define BIT_MSK__DPLL_CH_CTRL__RI_1CH_FLIP                                           0x04
  // (ReadWrite, Bits 5:3) 
  // Data Line Swap: Each bit of this field controls the data pin usage per DPLL input Channel. Value programmed into each bit controls the line used for data latching/capturing. The Line swapping per Channel is as given below. 0 - Does Not Swap Dp - Dn 1 - Swaps Dp - Dn Channel assignment per bit from MSb to LSB is as given below. {Ch2 Ch1 Ch0}
  #define BIT_MSK__DPLL_CH_CTRL__RI_D_PN_SWAP                                          0x38
  // (ReadWrite, Bits 7:6) 
  // reserved
  #define BIT_MSK__DPLL_CH_CTRL__RSVD                                                  0xC0

// NEW DPLL CFG Register
#define REG_ADDR__NEW_DPLL_CFG                                           (Page_2 | 0x00E6)
  // (ReadWrite, Bits 0) 
  // Enable reduced latency option
  #define BIT_MSK__NEW_DPLL_CFG__RI_RED_LAT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable MHL3+ TDD mode sync detection
  #define BIT_MSK__NEW_DPLL_CFG__RI_TDD_MODE                                           0x02
  // (ReadWrite, Bits 2) 
  // Enable limited past upto 16 cycles
  #define BIT_MSK__NEW_DPLL_CFG__RI_ENH_ALGN_FIFO                                      0x04
  // (ReadWrite, Bits 3) 
  // Enable using multi-SYNCs in sync period in MHL+ TDD mode
  #define BIT_MSK__NEW_DPLL_CFG__RI_ALLOW_MSYNC                                        0x08
  // (ReadWrite, Bits 4) 
  // Enable auto EQ per channel for 600MHz+ operation
  #define BIT_MSK__NEW_DPLL_CFG__RI_EQ_PER_CH                                          0x10
  // (ReadWrite, Bits 5) 
  // For TMDS SYNC; enable to look for twp SYNCs instead of one SYNC in TDD mode
  #define BIT_MSK__NEW_DPLL_CFG__RI_TWO_TMDS_SYNC                                      0x20
  // (ReadWrite, Bits 6) 
  // MHL1/2 descrambler ON (if 1), OFF (if 0), deafult is 1'b0
  #define BIT_MSK__NEW_DPLL_CFG__RI_MHL12_SCON                                         0x40

// DPLL SYNC PATTERN TDD #0 Register
#define REG_ADDR__SYNC_PATTERN_TDD_0                                     (Page_2 | 0x00E7)
  // (ReadWrite, Bits 7:0) 
  // 10b sync pattern for MHL3+ TDD mode[7:0]
  #define BIT_MSK__SYNC_PATTERN_TDD_0__RI_SYNC_PAT_B7_B0                                     0xFF

// DPLL SYNC PATTERN TDD #1 Register
#define REG_ADDR__SYNC_PATTERN_TDD_1                                     (Page_2 | 0x00E8)
  // (ReadWrite, Bits 1:0) 
  // 10b sync pattern for MHL3+ TDD mode[9:8]
  #define BIT_MSK__SYNC_PATTERN_TDD_1__RI_SYNC_PAT_B9_B8                                     0x03

// DPLL DFIFO ERROR STATUS Register
#define REG_ADDR__DPLL_DFIFO_ERROR_STS                                   (Page_2 | 0x00E9)
  // (ReadOnly, Bits 3:0) 
  // Error status indication of data fifo, which is added to improve timing
  #define BIT_MSK__DPLL_DFIFO_ERROR_STS__RO_DFIFO_ERR                                          0x0F

//***************************************************************************
// page 33. Address: 40
// EMSC General Control Register
#define REG_ADDR__GENCTL                                                 (Page_3 | 0x0000)
  // (ReadWrite, Bits 0) 
  // Enable eMSC
  #define BIT_MSK__GENCTL__REG_EMSC_EN                                           0x01
  // (ReadWrite, Bits 1) 
  // Software bit to initialize training sequence if needed, eg Tx fails n retries.
  #define BIT_MSK__GENCTL__REG_START_TRAIN_SEQ                                   0x02
  // (ReadWrite, Bits 2) 
  // clear EMSC XFIFO by setting read and write pointer to 0
  #define BIT_MSK__GENCTL__REG_CLR_XFIFO                                         0x04
  // (ReadWrite, Bits 3) 
  // clear EMSC RFIFO by setting read and write pointer to 0
  #define BIT_MSK__GENCTL__REG_CLR_RFIFO                                         0x08
  // (ReadWrite, Bits 5) 
  // EMSC software reset. Set to 1'b1 to reset.
  #define BIT_MSK__GENCTL__REG_EMSC_RST                                          0x20
  // (ReadWrite, Bits 6) 
  // 0 (default): eMSC transmitter will go to error state when grant or ACK timeout, wait FW to clear grant timeout or ack timeout.   1: disable xmit_state to go error state.  eMSC will keep retry message after grant or ACK timeout
  #define BIT_MSK__GENCTL__REG_DIS_XMIT_ERR_STATE                                0x40

// EMSC Comma Char1 Register
#define REG_ADDR__COMMACH1                                               (Page_3 | 0x0001)
  // (ReadWrite, Bits 7:0) 
  // register configurable comma character1 value, default = 0x96
  #define BIT_MSK__COMMACH1__REG_COMMA_CHAR1                                       0xFF

// EMSC Comma Char2 Register
#define REG_ADDR__COMMACH2                                               (Page_3 | 0x0002)
  // (ReadWrite, Bits 7:0) 
  // register configurable comma character2 value, default = 0x69
  #define BIT_MSK__COMMACH2__REG_COMMA_CHAR2                                       0xFF

// EMSC Null Char Register
#define REG_ADDR__NULLCH                                                 (Page_3 | 0x0003)
  // (ReadWrite, Bits 7:0) 
  // register configurable null character value, default = 0x00
  #define BIT_MSK__NULLCH__REG_NULL_CHAR                                         0xFF

// EMSC Train Char Register
#define REG_ADDR__TRAINCH                                                (Page_3 | 0x0004)
  // (ReadWrite, Bits 7:0) 
  // register configurable training character value, default = 0xff
  #define BIT_MSK__TRAINCH__REG_TRAIN_CHAR                                        0xFF

// EMSC Comma ErrorCNT Register
#define REG_ADDR__COMMECNT                                               (Page_3 | 0x0005)
  // (ReadWrite, Bits 3:0) 
  // number of comma char error mismatches to cause spi_comma_char_err interrupt and a new training sequence. Default value = 3 
  #define BIT_MSK__COMMECNT__REG_COMMA_CHAR_ERR_CNT                                0x0F
  // (ReadWrite, Bits 7) 
  // Enable I2C communicate with EMSC instead of SPI
  #define BIT_MSK__COMMECNT__REG_I2C_TO_EMSC_EN                                    0x80

// EMSC Comma Window Register
#define REG_ADDR__COMMAWIN                                               (Page_3 | 0x0006)
  // (ReadWrite, Bits 7:0) 
  // slot numbers between two comma character, default = d24
  #define BIT_MSK__COMMAWIN__REG_COMMA_CHAR_WIN                                    0xFF

// EMSC TX Train ByteCnt Low Register
#define REG_ADDR__TXTRAINBCNTL                                           (Page_3 | 0x0007)
  // (ReadWrite, Bits 7:0) 
  // 8 lsbs for number of Tx training character bytes. Defaut = 'd260
  #define BIT_MSK__TXTRAINBCNTL__REG_TX_TRAIN_CHAR_CNT_B7_B0                           0xFF

// EMSC TX Train ByteCnt High Register
#define REG_ADDR__TXTRAINBCNTH                                           (Page_3 | 0x0008)
  // (ReadWrite, Bits 0) 
  // MSB for number of Tx training character bytes. Default = 'd260
  #define BIT_MSK__TXTRAINBCNTH__REG_TX_TRAIN_CHAR_CNT_B8                              0x01

// EMSC Skip ByteCnt Low Register
#define REG_ADDR__SKIPBCNTL                                              (Page_3 | 0x0009)
  // (ReadWrite, Bits 7:0) 
  // 8 lsbs for number of skipped character bytes when invalid command occurs outside of data transfer. Defaut = 260
  #define BIT_MSK__SKIPBCNTL__REG_SKIP_BYTE_CNT_B7_B0                               0xFF

// EMSC Skip ByteCnt High Register
#define REG_ADDR__SKIPBCNTH                                              (Page_3 | 0x000A)
  // (ReadWrite, Bits 0) 
  // MSB for number of skipped bytes when invalid command occurs outside of data transfer. Defaut = 260
  #define BIT_MSK__SKIPBCNTH__REG_SKIP_BYTE_CNT_B8                                  0x01

// EMSC Rcv Tout Register
#define REG_ADDR__EMSCRCVTOUT                                            (Page_3 | 0x000B)
  // (ReadWrite, Bits 7:0) 
  // 8 bit MSB configurable timeout value for receiver. Default = 0x09. The granularity is 4096 TDM clock period. TDM clock frequency is 37.5MHz for CoC and 300MHz for DoC
  #define BIT_MSK__EMSCRCVTOUT__REG_RECV_TOUT_MSB                                     0xFF

// EMSC Xmit Tout Register
#define REG_ADDR__EMSCXMITTOUT                                           (Page_3 | 0x000C)
  // (ReadWrite, Bits 7:0) 
  // 8 bit MSB configurable timeout value for data transfer from TDM to SPI. Default = 0x14. The granularity is 4096 TDM clock period. TDM clock frequency is 37.5MHz for CoC and 300MHz for DoC
  #define BIT_MSK__EMSCXMITTOUT__REG_XMIT_TOUT_MSB                                     0xFF

// EMSC Tx Xmit Retry Max Low  Register
#define REG_ADDR__EMSCXMITRETRYMAXL                                      (Page_3 | 0x000D)
  // (ReadWrite, Bits 7:0) 
  // 8 bit LSB configurable retry interval for retransmit. Default = 0x20
  #define BIT_MSK__EMSCXMITRETRYMAXL__REG_XMIT_RETRY_MAX_MSB_B7_B0                          0xFF

// EMSC Tx Xmit Retry Max High Register
#define REG_ADDR__EMSCXMITRETRYMAXH                                      (Page_3 | 0x000E)
  // (ReadWrite, Bits 2:0) 
  // 3 bit MSB configurable retry interval for retransmit. Default = 0x00
  #define BIT_MSK__EMSCXMITRETRYMAXH__REG_XMIT_RETRY_MAX_MSB_B10_B8                         0x07

// EMSC Tx Xmit Retry Limit Register
#define REG_ADDR__EMSCXMITRETRYLMT                                       (Page_3 | 0x000F)
  // (ReadWrite, Bits 7:0) 
  // number of retries for transmit to send a packet. Default = 0xFF 
  #define BIT_MSK__EMSCXMITRETRYLMT__REG_XMIT_RETRY_LIMIT                                  0xFF

// EMSC Space Check Register
#define REG_ADDR__EMSCSPCHK                                              (Page_3 | 0x0012)
  // (ReadWrite, Bits 7:0) 
  // Combined with 0x313 (reg_wait_check_space_cnt) to allow HW check repeatedly for the availibility of EMSC RFIFO before replying NACK.
  #define BIT_MSK__EMSCSPCHK__REG_SPACE_CHECK_NUM                                   0xFF

// EMSC Wait Check Register
#define REG_ADDR__EMSCWTCHK                                              (Page_3 | 0x0013)
  // (ReadWrite, Bits 7:0) 
  // Wait {reg_wait_check_space_cnt, 8'hFF} clocks to check the availibility of EMSC RFIFO.
  #define BIT_MSK__EMSCWTCHK__REG_WAIT_CHECK_SPACE_CNT                              0xFF

// EMSC Xmit Debug Status Register
#define REG_ADDR__EMSCXMITSTAT                                           (Page_3 | 0x0014)
  // (ReadOnly, Bits 7:0) 
  // Transmit status
  #define BIT_MSK__EMSCXMITSTAT__REG_XMIT_STATUS                                       0xFF

// EMSC Recv Debug Status Register
#define REG_ADDR__EMSCRCVSTAT                                            (Page_3 | 0x0015)
  // (ReadOnly, Bits 7:0) 
  // Receive status
  #define BIT_MSK__EMSCRCVSTAT__REG_RECV_STATUS                                       0xFF

// MEM XFIFO ByteCnt Status Register
#define REG_ADDR__MEMXFIFOBCNT                                           (Page_3 | 0x0016)

// MEM RFIFO ByteCnt Status Register
#define REG_ADDR__MEMRFIFOBCNT                                           (Page_3 | 0x0017)

// EMSC XFIFO ByteCnt Status Low Register
#define REG_ADDR__EMSCXFIFOBCNTL                                         (Page_3 | 0x0018)
  // (ReadOnly, Bits 7:0) 
  // 8 LSBs for number of available bytes in SPI EMSC XFIFO for debugging
  #define BIT_MSK__EMSCXFIFOBCNTL__REG_EMSC_XFIFO_BYTE_CNT_B7_B0                         0xFF

// EMSC XFIFO ByteCnt Status High Register
#define REG_ADDR__EMSCXFIFOBCNTH                                         (Page_3 | 0x0019)
  // (ReadOnly, Bits 0) 
  // MSB for number of available bytes in SPI EMSC XFIFO for debugging
  #define BIT_MSK__EMSCXFIFOBCNTH__REG_EMSC_XFIFO_BYTE_CNT_B8                            0x01

// EMSC RFIFO ByteCnt Status Low Register
#define REG_ADDR__EMSCRFIFOBCNTL                                         (Page_3 | 0x001A)
  // (ReadOnly, Bits 7:0) 
  // 8 LSBs for number of available bytes in SPI EMSC RFIFO for debugging
  #define BIT_MSK__EMSCRFIFOBCNTL__REG_EMSC_RFIFO_BYTE_CNT_B7_B0                         0xFF

// EMSC RFIFO ByteCnt Status High Register
#define REG_ADDR__EMSCRFIFOBCNTH                                         (Page_3 | 0x001B)
  // (ReadOnly, Bits 0) 
  // MSB for number of available bytes in SPI EMSC RFIFO for debugging
  #define BIT_MSK__EMSCRFIFOBCNTH__REG_EMSC_RFIFO_BYTE_CNT_B8                            0x01

// EMSC XFIFO Space Status Low Register
#define REG_ADDR__EMSCXFIFOSPACEL                                        (Page_3 | 0x001C)
  // (ReadOnly, Bits 7:0) 
  // 8 LSBs for number of available EMSC XFIFO space for SPI to load new packet
  #define BIT_MSK__EMSCXFIFOSPACEL__REG_EMSC_XFIFO_SPACE_B7_B0                            0xFF

// EMSC XFIFO Space Status High Register
#define REG_ADDR__EMSCXFIFOSPACEH                                        (Page_3 | 0x001D)
  // (ReadOnly, Bits 0) 
  // MSB for number of available EMSC XFIFO space for SPI to load new packet
  #define BIT_MSK__EMSCXFIFOSPACEH__REG_EMSC_XFIFO_SPACE_B8                               0x01

// EMSC Burst Cnt Status Register
#define REG_ADDR__EMSCBURSTCNT                                           (Page_3 | 0x001E)
  // (ReadOnly, Bits 7:0) 
  // the first packet size for SPI read
  #define BIT_MSK__EMSCBURSTCNT__REG_EMSC_BURST_CNT                                    0xFF

// EMSC XMIT TOState Register
#define REG_ADDR__EMSCTOSTT                                              (Page_3 | 0x001F)
  // (ReadOnly, Bits 3:0) 
  // Reflects the internal transmit state machine state when it times out. Used only for debugging. 
  #define BIT_MSK__EMSCTOSTT__REG_XMIT_TIMEOUT_STATE                                0x0F

// EMSC Aggregated Interrupt Status Register
#define REG_ADDR__EMSC_AGGREGATED_INT_STAT                               (Page_3 | 0x0020)
  // (ReadOnly, Bits 0) 
  // Aggregated interrupt at 0x23
  #define BIT_MSK__EMSC_AGGREGATED_INT_STAT__REG_EMSC_INTR_STAT_B0                                 0x01
  // (ReadOnly, Bits 1) 
  // Aggregated interrupt at 0x2C
  #define BIT_MSK__EMSC_AGGREGATED_INT_STAT__REG_EMSC_INTR_STAT_B1                                 0x02

// EMSC PRAM RFIFO Byte Count  Register
#define REG_ADDR__PRAMRFFFBCNT                                           (Page_3 | 0x0021)

// EMSC Status Register
#define REG_ADDR__EMSCSTAT                                               (Page_3 | 0x0022)
  // (ReadOnly, Bits 0) 
  // EMSC Normal mode indicator
  #define BIT_MSK__EMSCSTAT__REG_EMSC_NORMAL_MODE                                  0x01

// EMSC 1st Interrupt Register
#define REG_ADDR__EMSCINTR                                               (Page_3 | 0x0023)
  // (ReadOnly, Bits 0) 
  // Indicates if EMSC receiver has data to be fetched. It is special since it cannot be cleared unless all the data have been feteched
  #define BIT_MSK__EMSCINTR__REG_EMSC_RFIFO_NOT_EMPTY                              0x01
  // (ReadWrite, Bits 1) 
  // eMSC transmitter not getting grant timeout interrupt  
  #define BIT_MSK__EMSCINTR__REG_XMIT_GNT_TOUT                                     0x02
  // (ReadWrite, Bits 2) 
  // EMSC Xmit Done interrupt. SW write 1 clear.
  #define BIT_MSK__EMSCINTR__REG_XMIT_DONE                                         0x04
  // (ReadWrite, Bits 3) 
  // Number of comma character mismatch reaches the limit defined by Comma Character Error Count.
  #define BIT_MSK__EMSCINTR__REG_COMMA_CHAR_ERR                                    0x08
  // (ReadWrite, Bits 4) 
  // SPI writes more than XFIFO space. SW write 1 clear.
  #define BIT_MSK__EMSCINTR__REG_XFIFO_WRITE_ERR                                   0x10
  // (ReadWrite, Bits 5) 
  // It is asserted when the host interface SPI or I2C tries to read more bytes than the available bytes in eMSC RFIFO.
  #define BIT_MSK__EMSCINTR__REG_RFIFO_READ_ERR                                    0x20
  // (ReadWrite, Bits 6) 
  // eMSC transmitter not getting ACK timeout interrupt  
  #define BIT_MSK__EMSCINTR__REG_XMIT_ACK_TOUT                                     0x40
  // (ReadWrite, Bits 7) 
  // eMSC Xmit FIFO empty
  #define BIT_MSK__EMSCINTR__REG_XFIFO_EMPTY                                       0x80

// EMSC Interrupt Mask Register
#define REG_ADDR__EMSCINTRMASK                                           (Page_3 | 0x0024)
  // (ReadWrite, Bits 0) 
  // EMSC interrupt mask bit for event 0.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK0                                  0x01
  // (ReadWrite, Bits 1) 
  // EMSC interrupt mask bit for event 1.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK1                                  0x02
  // (ReadWrite, Bits 2) 
  // EMSC interrupt mask bit for event 2.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK2                                  0x04
  // (ReadWrite, Bits 3) 
  // EMSC interrupt mask bit for event 3.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK3                                  0x08
  // (ReadWrite, Bits 4) 
  // EMSC interrupt mask bit for event 4.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK4                                  0x10
  // (ReadWrite, Bits 5) 
  // EMSC interrupt mask bit for event 5.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK5                                  0x20
  // (ReadWrite, Bits 6) 
  // EMSC interrupt mask bit for event 6.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK6                                  0x40
  // (ReadWrite, Bits 7) 
  // EMSC interrupt mask bit for event 7.
  #define BIT_MSK__EMSCINTRMASK__REG_EMSC_INTR0_MASK7                                  0x80

// EMSC RX Train ByteCnt Low Register
#define REG_ADDR__RXTRAINBCNTL                                           (Page_3 | 0x0025)
  // (ReadWrite, Bits 7:0) 
  // 8 lsbs for number of Rx training character bytes. Defaut = 'd260
  #define BIT_MSK__RXTRAINBCNTL__REG_RX_TRAIN_CHAR_CNT_B7_B0                           0xFF

// EMSC RX Train ByteCnt High Register
#define REG_ADDR__RXTRAINBCNTH                                           (Page_3 | 0x0026)
  // (ReadWrite, Bits 0) 
  // MSB for number of Rx training character bytes. Default = 'd260
  #define BIT_MSK__RXTRAINBCNTH__REG_RX_TRAIN_CHAR_CNT_B8                              0x01

// EMSC Grant Retry Cnt Status Register
#define REG_ADDR__EMSCGNTRTYCNT                                          (Page_3 | 0x0027)
  // (ReadOnly, Bits 7:0) 
  // eMSC TX retry counter until GRANT received.
  #define BIT_MSK__EMSCGNTRTYCNT__REG_GNT_RETRY_CNT                                     0xFF

// EMSC ACK Retry Cnt Status Register
#define REG_ADDR__EMSCACKRTYCNT                                          (Page_3 | 0x0028)
  // (ReadOnly, Bits 7:0) 
  // eMSC TX retry counter until ACK received.
  #define BIT_MSK__EMSCACKRTYCNT__REG_ACK_RETRY_CNT                                     0xFF

// SPI EMSC FIFO Status Register
#define REG_ADDR__SPIEMSCFIFOSTAT                                        (Page_3 | 0x0029)
  // (ReadOnly, Bits 7:0) 
  // bit 7: XFIFO is full bit 6: RFIFO is full bit 5:4: Reserved bit 3: XFIFO is empty bit 2: RFIFO is empty bit 1:0: Reserved
  #define BIT_MSK__SPIEMSCFIFOSTAT__REG_EMSC_FIFO_STATUS                                  0xFF

// I2C EMSC XMIT FIFO Write Port
#define REG_ADDR__EMSC_XMIT_WRITE_PORT                                   (Page_3 | 0x002A)
  // (ReadWrite, Bits 7:0) 
  // EMSC transmit write port for I2C
  #define BIT_MSK__EMSC_XMIT_WRITE_PORT__REG_EMSC_XMIT_WRITE_PORT                              0xFF

// I2C EMSC RCV FIFO Write Port
#define REG_ADDR__EMSC_RCV_READ_PORT                                     (Page_3 | 0x002B)
  // (ReadOnly, Bits 7:0) 
  // EMSC receive read port for I2C
  #define BIT_MSK__EMSC_RCV_READ_PORT__REG_EMSC_RCV_READ_PORT                                0xFF

// EMSC 2nd Interrupt Register
#define REG_ADDR__EMSCINTR1                                              (Page_3 | 0x002C)
  // (ReadWrite, Bits 0) 
  // Interrupt to indicate that training sequence wasn't concluded with comma character at slot 0
  #define BIT_MSK__EMSCINTR1__REG_TRAINING_COMMA_ERR                                0x01

// EMSC Interrupt Mask Register
#define REG_ADDR__EMSCINTRMASK1                                          (Page_3 | 0x002D)
  // (ReadWrite, Bits 0) 
  // EMSC interrupt mask bit for event 0.
  #define BIT_MSK__EMSCINTRMASK1__REG_EMSC_INTR1_MASK0                                  0x01

// MHLTX_TOP Interrupt Status Register
#define REG_ADDR__MHLTXTOP_INTR                                          (Page_3 | 0x0030)
  // (ReadOnly, Bits 4:0) 
  // MHL_TX interrupt status 0: CBUS general interrupt status 1: eMSC general interrupt status 2: USBT general interrupt status 3: TDM general interrupt status 4: MHL_TX local interrupt status
  #define BIT_MSK__MHLTXTOP_INTR__REG_MHL_TX_INTR_STAT                                  0x1F

// Interrupt Source #1 Register
#define REG_ADDR__MHLTXTOP_INTR1                                         (Page_3 | 0x0031)
  // (ReadWrite, Bits 0) 
  // PRIF_READY is forced high for eMSC access. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__MHLTXTOP_INTR1__REG_INTR1_STAT0                                       0x01

// Interrupt #1 Mask Register
#define REG_ADDR__MHLTXTOP_INTR1_MASK                                    (Page_3 | 0x0032)
  // (ReadWrite, Bits 0) 
  // Enable INT1[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHLTXTOP_INTR1_MASK__REG_INTR1_MASK0                                       0x01

// PRIF_READY Recovery Register
#define REG_ADDR__MHLTXTOP_RDY_RCC                                       (Page_3 | 0x0033)
  // (ReadWrite, Bits 1:0) 
  // This register defines a timer. After timer expiration IP prif_ready will be forced high for registers located in tclk domain. 2'b00: 8k cycles; 2'b01: 16k; 2'b10: 21k; 2'b11: 64k 
  #define BIT_MSK__MHLTXTOP_RDY_RCC__REG_FORCERDY_SEL                                      0x03
  // (ReadWrite, Bits 2) 
  // Set this bit will force IP prif_ready becomes 1 after certain timeout, for register byte located in tclk domain while tclk is not provided by the system.
  #define BIT_MSK__MHLTXTOP_RDY_RCC__REG_FORCERDY_EN                                       0x04

// SRST Register
#define REG_ADDR__TDMSRST                                                (Page_3 | 0x0040)
  // (ReadWrite, Bits 0) 
  // Active high reset signal for TDM Tx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__TDMSRST__REG_TTX_SRST                                          0x01
  // (ReadWrite, Bits 1) 
  // Active high reset signal for TDM Rx. Power on reset and should be controlled by software. (External synchronizer is required)
  #define BIT_MSK__TDMSRST__REG_TRX_SRST                                          0x02
  // (ReadOnly, Bits 2) 
  // general interrupt status.
  #define BIT_MSK__TDMSRST__REG_INTR                                              0x04
  // (ReadOnly, Bits 7:4) 
  // Byte aggregated interrupt status.  Bit 0 for TTXINTL;  Bit 1 for TTXINTH;  Bit 2 for TRXINTL;  Bit 3 for TRXINTH.
  #define BIT_MSK__TDMSRST__REG_AGGRINTR_B3_B0                                    0xF0

// TDM TX INT Low Register
#define REG_ADDR__TTXINTL                                                (Page_3 | 0x0041)
  // (ReadWrite, Bits 0) 
  // event 0 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. HSIC2TDM AFIFO empty
  #define BIT_MSK__TTXINTL__REG_TTX_INTR0                                         0x01
  // (ReadWrite, Bits 1) 
  // event 1 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. HSIC2TDM AFIFO empty during normal TDM operation
  #define BIT_MSK__TTXINTL__REG_TTX_INTR1                                         0x02
  // (ReadWrite, Bits 2) 
  // event 2 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM Tx CBUS state error due to unexpected CBUS1 transaction
  #define BIT_MSK__TTXINTL__REG_TTX_INTR2                                         0x04
  // (ReadWrite, Bits 3) 
  // event 3 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM Tx slot 0 location is adjusted for slot 0 interleaving
  #define BIT_MSK__TTXINTL__REG_TTX_INTR3                                         0x08
  // (ReadWrite, Bits 4) 
  // event 4 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTL__REG_TTX_INTR4                                         0x10
  // (ReadWrite, Bits 5) 
  // event 5 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTL__REG_TTX_INTR5                                         0x20
  // (ReadWrite, Bits 6) 
  // event 6 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTL__REG_TTX_INTR6                                         0x40
  // (ReadWrite, Bits 7) 
  // event 7 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTL__REG_TTX_INTR7                                         0x80

// TDM TX INT High Register
#define REG_ADDR__TTXINTH                                                (Page_3 | 0x0042)
  // (ReadWrite, Bits 0) 
  // event 8 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR8                                         0x01
  // (ReadWrite, Bits 1) 
  // event 9 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR9                                         0x02
  // (ReadWrite, Bits 2) 
  // event 10 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR10                                        0x04
  // (ReadWrite, Bits 3) 
  // event 11 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR11                                        0x08
  // (ReadWrite, Bits 4) 
  // event 12 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR12                                        0x10
  // (ReadWrite, Bits 5) 
  // event 13 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR13                                        0x20
  // (ReadWrite, Bits 6) 
  // event 14 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR14                                        0x40
  // (ReadWrite, Bits 7) 
  // event 15 of ro_tdm_tx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TTXINTH__REG_TTX_INTR15                                        0x80

// TDM RX INT Low Register
#define REG_ADDR__TRXINTL                                                (Page_3 | 0x0043)
  // (ReadWrite, Bits 0) 
  // event 0 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. CBUS packet dropped
  #define BIT_MSK__TRXINTL__REG_TRX_INTR0                                         0x01
  // (ReadWrite, Bits 1) 
  // event 1 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved (SPI packet dropped)
  #define BIT_MSK__TRXINTL__REG_TRX_INTR1                                         0x02
  // (ReadWrite, Bits 2) 
  // event 2 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Invalid CBUS packet
  #define BIT_MSK__TRXINTL__REG_TRX_INTR2                                         0x04
  // (ReadWrite, Bits 3) 
  // event 3 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM2HSIC AFIFO overflow
  #define BIT_MSK__TRXINTL__REG_TRX_INTR3                                         0x08
  // (ReadWrite, Bits 4) 
  // event 4 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTL__REG_TRX_INTR4                                         0x10
  // (ReadWrite, Bits 5) 
  // event 5 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTL__REG_TRX_INTR5                                         0x20
  // (ReadWrite, Bits 6) 
  // event 6 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTL__REG_TRX_INTR6                                         0x40
  // (ReadWrite, Bits 7) 
  // event 7 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTL__REG_TRX_INTR7                                         0x80

// TDM RX INT High Register
#define REG_ADDR__TRXINTH                                                (Page_3 | 0x0044)
  // (ReadWrite, Bits 0) 
  // event 8 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM status change from SYNC_WAIT to SYNC_DATA
  #define BIT_MSK__TRXINTH__REG_TRX_INTR8                                         0x01
  // (ReadWrite, Bits 1) 
  // event 9 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM status change from SYNC_DATA to SYNC_WAIT
  #define BIT_MSK__TRXINTH__REG_TRX_INTR9                                         0x02
  // (ReadWrite, Bits 2) 
  // event 10 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM status change from SYNC_DATA to SYNC_UNSYNC
  #define BIT_MSK__TRXINTH__REG_TRX_INTR10                                        0x04
  // (ReadWrite, Bits 3) 
  // event 11 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. TDM status change from any state to any state
  #define BIT_MSK__TRXINTH__REG_TRX_INTR11                                        0x08
  // (ReadWrite, Bits 4) 
  // event 12 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTH__REG_TRX_INTR12                                        0x10
  // (ReadWrite, Bits 5) 
  // event 13 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTH__REG_TRX_INTR13                                        0x20
  // (ReadWrite, Bits 6) 
  // event 14 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTH__REG_TRX_INTR14                                        0x40
  // (ReadWrite, Bits 7) 
  // event 15 of ro_tdm_rx_intr[15:0]. Interrupt from tdm_tx module. Only valid when mask bit is 1. Reserved
  #define BIT_MSK__TRXINTH__REG_TRX_INTR15                                        0x80

// TDM TX INTMASK Low Register
#define REG_ADDR__TTXINTML                                               (Page_3 | 0x0045)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 0.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK0                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 1.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK1                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 2.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK2                                     0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 3.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK3                                     0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 4.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK4                                     0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 5.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK5                                     0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 6.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK6                                     0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 7.
  #define BIT_MSK__TTXINTML__REG_TTX_INTRMASK7                                     0x80

// TDM TX INTMASK High Register
#define REG_ADDR__TTXINTMH                                               (Page_3 | 0x0046)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 8.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK8                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 9.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK9                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 10.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK10                                    0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 11.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK11                                    0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 12.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK12                                    0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 13.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK13                                    0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 14.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK14                                    0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 15.
  #define BIT_MSK__TTXINTMH__REG_TTX_INTRMASK15                                    0x80

// TDM RX INTMASK Low Register
#define REG_ADDR__TRXINTML                                               (Page_3 | 0x0047)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 0.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK0                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 1.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK1                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 2.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK2                                     0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 3.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK3                                     0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 4.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK4                                     0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 5.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK5                                     0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 6.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK6                                     0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 7.
  #define BIT_MSK__TRXINTML__REG_TRX_INTRMASK7                                     0x80

// TDM RX INTMASK High Register
#define REG_ADDR__TRXINTMH                                               (Page_3 | 0x0048)
  // (ReadWrite, Bits 0) 
  // mask bit of intr event 8.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK8                                     0x01
  // (ReadWrite, Bits 1) 
  // mask bit of intr event 9.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK9                                     0x02
  // (ReadWrite, Bits 2) 
  // mask bit of intr event 10.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK10                                    0x04
  // (ReadWrite, Bits 3) 
  // mask bit of intr event 11.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK11                                    0x08
  // (ReadWrite, Bits 4) 
  // mask bit of intr event 12.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK12                                    0x10
  // (ReadWrite, Bits 5) 
  // mask bit of intr event 13.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK13                                    0x20
  // (ReadWrite, Bits 6) 
  // mask bit of intr event 14.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK14                                    0x40
  // (ReadWrite, Bits 7) 
  // mask bit of intr event 15.
  #define BIT_MSK__TRXINTMH__REG_TRX_INTRMASK15                                    0x80

// TDM TX Buf Register
#define REG_ADDR__TTXBUF                                                 (Page_3 | 0x0049)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_buffer[7:0]. Size of buffering before reading out from afifo in order to maintain fifo level in middle range. Default 8'h8.
  #define BIT_MSK__TTXBUF__REG_TTX_BUFFER_B7_B0                                  0xFF

// TDM TX NUMBITS Register
#define REG_ADDR__TTXNUMB                                                (Page_3 | 0x004A)
  // (ReadWrite, Bits 2:0) 
  // ri_tdm_tx_num_of_bits_per_symbol[2:0].Number of bits in a symbol 4=(DoC or CoC 8b10b bypass) 5=10-bit(CoC 8b10b)
  #define BIT_MSK__TTXNUMB__REG_TTX_NUMBPS_B2_B0                                  0x07
  // (ReadWrite, Bits 3) 
  // ri_tdm_tx_com1_at_sync_wait. This option will be used only for source device.
  #define BIT_MSK__TTXNUMB__REG_TTX_COM1_AT_SYNC_WAIT                             0x08

// TDM TX NUMSPISYM Register
#define REG_ADDR__TTXSPINUMS                                             (Page_3 | 0x004B)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_num_of_spi_symbols_per_period[7:0]. Number of SPI symbols in a period.
  #define BIT_MSK__TTXSPINUMS__REG_TTX_NUMSPISYM                                     0xFF

// TDM TX NUMHSICSYM Register
#define REG_ADDR__TTXHSICNUMS                                            (Page_3 | 0x004C)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_num_of_hsic_symbols_per_period[7:0]. Number of HSIC symbols in a period.
  #define BIT_MSK__TTXHSICNUMS__REG_TTX_NUMHSICSYM                                    0xFF

// TDM TX NUMTOTSYM Register
#define REG_ADDR__TTXTOTNUMS                                             (Page_3 | 0x004D)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_num_of_total_m1_symbols_per_period[7:0]. Number of total symbols minus one in a period. Should be sum of SPI and HSIC symbols.
  #define BIT_MSK__TTXTOTNUMS__REG_TTX_NUMTOTSYM                                     0xFF

// TDM TX SYMCOM1 Low Register
#define REG_ADDR__TTXSCOM1L                                              (Page_3 | 0x004E)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_com1[9:0]. Symbol code for COM1. 10'hA5 for SE, 10'h1BC for DM.
  #define BIT_MSK__TTXSCOM1L__REG_TTX_SYMCOM1_B7_B0                                 0xFF

// TDM TX SYMCOM1 High Register
#define REG_ADDR__TTXSCOM1H                                              (Page_3 | 0x004F)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_com1[9:0]. Symbol code for COM1. 10'hA5 for SE, 10'h1BC for DM.
  #define BIT_MSK__TTXSCOM1H__REG_TTX_SYMCOM1_B9_B8                                 0x03

// TDM TX SYMCOM2 Low Register
#define REG_ADDR__TTXSCOM2L                                              (Page_3 | 0x0050)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_com2[9:0]. Symbol code for COM2. 10'h5A for SE, 10'h13C for DM.
  #define BIT_MSK__TTXSCOM2L__REG_TTX_SYMCOM2_B7_B0                                 0xFF

// TDM TX SYMCOM2 High Register
#define REG_ADDR__TTXSCOM2H                                              (Page_3 | 0x0051)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_com2[9:0]. Symbol code for COM2. 10'h5A for SE, 10'h13C for DM.
  #define BIT_MSK__TTXSCOM2H__REG_TTX_SYMCOM2_B9_B8                                 0x03

// TDM TX SYMIDLE Low Register
#define REG_ADDR__TTXSIDLEL                                              (Page_3 | 0x0052)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_idle[9:0]. Symbol code for IDLE. Default 10'h0.
  #define BIT_MSK__TTXSIDLEL__REG_TTX_SYMIDLE_B7_B0                                 0xFF

// TDM TX SYMIDLE High Register
#define REG_ADDR__TTXSIDLEH                                              (Page_3 | 0x0053)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_idle[9:0]. Symbol code for IDLE. Default 10'h0.
  #define BIT_MSK__TTXSIDLEH__REG_TTX_SYMIDLE_B9_B8                                 0x03

// TDM TX SYMCBUS Low Register
#define REG_ADDR__TTXSCBUSL                                              (Page_3 | 0x0054)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_cbus[9:0]. Symbol code for CBUS. Default 10'h39.
  #define BIT_MSK__TTXSCBUSL__REG_TTX_SYMCBUS_B7_B0                                 0xFF

// TDM TX SYMCBUS High Register
#define REG_ADDR__TTXSCBUSH                                              (Page_3 | 0x0055)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_cbus[9:0]. Symbol code for CBUS. Default 10'h39.
  #define BIT_MSK__TTXSCBUSH__REG_TTX_SYMCBUS_B9_B8                                 0x03

// TDM TX SYMCBUSA Low Register
#define REG_ADDR__TTXSCBUSAL                                             (Page_3 | 0x0056)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_cbusa[9:0]. Symbol code for CBUSA. Default 10'h63.
  #define BIT_MSK__TTXSCBUSAL__REG_TTX_SYMCBUSA_B7_B0                                0xFF

// TDM TX SYMCBUSA High Register
#define REG_ADDR__TTXSCBUSAH                                             (Page_3 | 0x0057)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_cbusa[9:0]. Symbol code for CBUSA. Default 10'h63.
  #define BIT_MSK__TTXSCBUSAH__REG_TTX_SYMCBUSA_B9_B8                                0x03

// TDM TX SYMCBUSN Low Register
#define REG_ADDR__TTXSCBUSNL                                             (Page_3 | 0x0058)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_symbol_cbusn[9:0]. Symbol code for CBUSN. Default 10'h6C.
  #define BIT_MSK__TTXSCBUSNL__REG_TTX_SYMCBUSN_B7_B0                                0xFF

// TDM TX SYMCBUSN High Register
#define REG_ADDR__TTXSCBUSNH                                             (Page_3 | 0x0059)
  // (ReadWrite, Bits 1:0) 
  // ri_tdm_tx_symbol_cbusn[9:0]. Symbol code for CBUSN. Default 10'h6C.
  #define BIT_MSK__TTXSCBUSNH__REG_TTX_SYMCBUSN_B9_B8                                0x03

// TDM TX Status 1st Register
#define REG_ADDR__TTXSTA1                                                (Page_3 | 0x005A)
  // (ReadOnly, Bits 7:0) 
  // HSIC2TDM AFIFO rtags
  #define BIT_MSK__TTXSTA1__REG_TTX_STATUS_B7_B0                                  0xFF

// TDM TX Status 2nd Register
#define REG_ADDR__TTXSTA2                                                (Page_3 | 0x005B)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: HSIC2TDM AFIFO raddr bit 3: HSIC2TDM AFIFO error bit 4: HSIC current state bit 7-5: TDM current state 
  #define BIT_MSK__TTXSTA2__REG_TTX_STATUS_B15_B8                                 0xFF

// TDM TX Status 3rd Register
#define REG_ADDR__TTXSTA3                                                (Page_3 | 0x005C)
  // (ReadOnly, Bits 7:0) 
  // Current tdm_tx slot 0 distance from tdm_rx slot 0 location (positive number) (ES1.0)
  #define BIT_MSK__TTXSTA3__REG_TTX_STATUS_B23_B16                                0xFF

// TDM TX Status 4th Register
#define REG_ADDR__TTXSTA4                                                (Page_3 | 0x005D)
  // (ReadOnly, Bits 7:0) 
  // link_data value sent out to CoC/DoC (ES1.0)
  #define BIT_MSK__TTXSTA4__REG_TTX_STATUS_B31_B24                                0xFF

// TDM TX Override Low Register
#define REG_ADDR__TTXOVRL                                                (Page_3 | 0x005E)
  // (ReadWrite, Bits 7:0) 
  // LSB of ri_tdm_tx_hsic_override_hsic_in[10:0], [10]=override_enable,[9:0]=data (default 10'h0) (ES1.0)
  #define BIT_MSK__TTXOVRL__REG_TTX_OVERRIDE_HSIC_IN_B7_B0                        0xFF

// TDM TX Override High Register
#define REG_ADDR__TTXOVRH                                                (Page_3 | 0x005F)
  // (ReadWrite, Bits 2:0) 
  // MSB of ri_tdm_tx_hsic_override_hsic_in[10:0], [10]=override_enable,[9:0]=data (default 10'h0) (ES1.0)
  #define BIT_MSK__TTXOVRH__REG_TTX_OVERRIDE_HSIC_IN_B10_B8                       0x07

// TDM TX Misc Control Register
#define REG_ADDR__TTXMISCCTL                                             (Page_3 | 0x0060)
  // (ReadWrite, Bits 0) 
  // ri_tdm_tx_ignore_cbus_in, 0=accept CBUS input, 1=ignore CBUS input (defult 1'b0) (ES1.0)
  #define BIT_MSK__TTXMISCCTL__REG_TTX_IGNORE_CBUS_IN                                0x01
  // (ReadWrite, Bits 1) 
  // ri_tdm_tx_ignore_spi_in, 0=accept SPI input, 1=ignore SPI input (defult 1'b0) (ES1.0)
  #define BIT_MSK__TTXMISCCTL__REG_TTX_IGNORE_SPI_IN                                 0x02
  // (ReadWrite, Bits 2) 
  // ri_tdm_tx_alloc_hsic_then_spi, ES1.0: 0=allocate CBUS/SPI(=N)/HSIC(=M), 1=allocate CBUS/HSIC(=N)/SPI(=M) (default 1'b0)
  #define BIT_MSK__TTXMISCCTL__REG_TTX_ALLOC_HSIC_THEN_SPI                           0x04

// TDM RX Output Enable Register
#define REG_ADDR__TRXOUTEN                                               (Page_3 | 0x0061)
  // (ReadWrite, Bits 5:0) 
  // [5:4]=SPI out enable (default 3'h3F), [3:2]=HSIC out enable, [1:0]=CBUS out enable, 2'b00=disable, 2'b01=enable when both TDM synched, 2'b11=enable when local TDM synched(default)
  #define BIT_MSK__TRXOUTEN__REG_TRX_OUT_EN                                        0x3F
  // (ReadWrite, Bits 6) 
  // ri_tdm_rx_alloc_hsic_then_spi, ES1.0: 0=allocate CBUS/SPI(=N)/HSIC(=M), 1=allocate CBUS/HSIC(=N)/SPI(=M) (default 1'b0)
  #define BIT_MSK__TRXOUTEN__REG_TRX_ALLOC_HSIC_THEN_SPI                           0x40
  // (ReadWrite, Bits 7) 
  // ri_status_en. Enable to latch status (ro_hsic_tx_status, ro_hsic_rx_status, ro_tdm_tx_status, ro_tdm_rx_status). Turn off to hold the status.
  #define BIT_MSK__TRXOUTEN__REG_STATUS_EN                                         0x80

// TDM RX Control Register
#define REG_ADDR__TRXCTRL                                                (Page_3 | 0x0062)
  // (ReadWrite, Bits 2:0) 
  // ri_tdm_rx_num_of_bits_per_symbol[2:0].Number of bits in a symbol 4=(DoC or CoC 8b10b bypass) 5=10-bit(CoC 8b10b)
  #define BIT_MSK__TRXCTRL__REG_TRX_NUMBPS_B2_B0                                  0x07
  // (ReadWrite, Bits 3) 
  // ri_tdm_rx_from_se_coc. Select which analog core will be used. 0=Differential CoC, 1=Single-Ended CoC
  #define BIT_MSK__TRXCTRL__REG_TRX_FROM_SE_COC                                   0x08
  // (ReadWrite, Bits 4) 
  // ri_tdm_rx_clear_when_valid_low. Clear latch of link_rx_data_di when link_rx_valid is low. 1=enable (default), 0=disable
  #define BIT_MSK__TRXCTRL__REG_TRX_CLR_WVALLOW                                   0x10

// TDM RX NUMSPISYM Register
#define REG_ADDR__TRXSPINUMS                                             (Page_3 | 0x0063)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_rx_num_of_spi_symbols_per_period[7:0]. Number of SPI symbols in a period.
  #define BIT_MSK__TRXSPINUMS__REG_TRX_NUMSPISYM                                     0xFF

// TDM RX NUMHSICSYM Register
#define REG_ADDR__TRXHSICNUMS                                            (Page_3 | 0x0064)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_rx_num_of_hsic_symbols_per_period[7:0]. Number of HSIC symbols in a period.
  #define BIT_MSK__TRXHSICNUMS__REG_TRX_NUMHSICSYM                                    0xFF

// TDM RX NUMTOTSYM Register
#define REG_ADDR__TRXTOTNUMS                                             (Page_3 | 0x0065)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_rx_num_of_total_m1_symbols_per_period[7:0]. Number of total symbols minus one in a period.Should be sum of SPI and HSIC symbols.
  #define BIT_MSK__TRXTOTNUMS__REG_TRX_NUMTOTSYM                                     0xFF

// TDM RX MIN12MCHPRD Register
#define REG_ADDR__TRXMIN12                                               (Page_3 | 0x0066)
  // (ReadWrite, Bits 7:0) 
  // ri_minimum_com12_matched_periods[7:0]. Minimum number of period of slot synchronization before sending acknowledge COM2. Default 8'h8.
  #define BIT_MSK__TRXMIN12__REG_TRX_MIN12MP                                       0xFF

// TDM RX MIN2MCHPRD Register
#define REG_ADDR__TRXMIN2                                                (Page_3 | 0x0067)
  // (ReadWrite, Bits 7:0) 
  // ri_minimum_com2_matched_periods[7:0]. Minimum number of period of slot synchronization before sending actual USB/CBUS data packet. Default 8'h8.
  #define BIT_MSK__TRXMIN2__REG_TRX_MIN2MP                                        0xFF

// TDM RX MAX12MCHPRD Register
#define REG_ADDR__TRXMAX12                                               (Page_3 | 0x0068)
  // (ReadWrite, Bits 7:0) 
  // ri_maximum_com12_matched_periods[7:0]. maximum number of period of slot synchronization before sending acknowledge COM2. Default 8'hA.
  #define BIT_MSK__TRXMAX12__REG_TRX_MAX12MP                                       0xFF

// TDM RX MAX2MCHPRD Register
#define REG_ADDR__TRXMAX2                                                (Page_3 | 0x0069)
  // (ReadWrite, Bits 7:0) 
  // ri_maximum_com2_matched_periods[7:0]. maximum number of period of slot synchronization before sending actual USB/CBUS data packet. Default 8'hA.
  #define BIT_MSK__TRXMAX2__REG_TRX_MAX2MP                                        0xFF

// TDM RX Status 1st Register
#define REG_ADDR__TRXSTA1                                                (Page_3 | 0x006A)
  // (ReadOnly, Bits 7:0) 
  // TDM2HSIC AFIFO wtags
  #define BIT_MSK__TRXSTA1__REG_TRX_STATUS_B7_B0                                  0xFF

// TDM RX Status 2nd Register
#define REG_ADDR__TRXSTA2                                                (Page_3 | 0x006B)
  // (ReadOnly, Bits 7:0) 
  // bit 2-0: TDM2HSIC AFIFO waddr bir 5-3: CBUS current state bit 7-6: TDM current state
  #define BIT_MSK__TRXSTA2__REG_TRX_STATUS_B15_B8                                 0xFF

// TDM RX Status 3rd Register
#define REG_ADDR__TRXSTA3                                                (Page_3 | 0x006C)
  // (ReadOnly, Bits 7:0) 
  // Matched COM1/COM2 period count (ES1.0)
  #define BIT_MSK__TRXSTA3__REG_TRX_STATUS_B23_B16                                0xFF

// TDM RX Status 4th Register
#define REG_ADDR__TRXSTA4                                                (Page_3 | 0x006D)
  // (ReadOnly, Bits 7:0) 
  // Matched COM2 period count (ES1.0)
  #define BIT_MSK__TRXSTA4__REG_TRX_STATUS_B31_B24                                0xFF

// TDM RX Status 5th Register
#define REG_ADDR__TRXSTA5                                                (Page_3 | 0x006E)
  // (ReadOnly, Bits 7:0) 
  // bit 1-0: LL channel selection result (ES1.0) bit 7-2: reserved
  #define BIT_MSK__TRXSTA5__REG_TRX_STATUS_B39_B32                                0xFF

// TDM RX Status 6th Register
#define REG_ADDR__TRXSTA6                                                (Page_3 | 0x006F)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__TRXSTA6__REG_TRX_STATUS_B47_B40                                0xFF

// TDM RX Status 7th Register
#define REG_ADDR__TRXSTA7                                                (Page_3 | 0x0070)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__TRXSTA7__REG_TRX_STATUS_B55_B48                                0xFF

// TDM RX Status 8th Register
#define REG_ADDR__TRXSTA8                                                (Page_3 | 0x0071)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__TRXSTA8__REG_TRX_STATUS_B63_B56                                0xFF

// TDM Low Latency Register
#define REG_ADDR__TDMLLCTL                                               (Page_3 | 0x0072)
  // (ReadWrite, Bits 0) 
  // ri_tdm_tx_ll_sel_mode, 0=LL disable 1=LL enable (manual select) (default 1'b0) (ES1.0)
  #define BIT_MSK__TDMLLCTL__REG_TTX_LL_SEL_MODE                                   0x01
  // (ReadWrite, Bits 1) 
  // ri_tdm_tx_ll_tie_low, 0=LL forward after training done 1=LL tied to low (default 1'b0) (ES1.0)
  #define BIT_MSK__TDMLLCTL__REG_TTX_LL_TIE_LOW                                    0x02
  // (ReadWrite, Bits 3:2) 
  // ri_tdm_tx_ll_sel_manual[1:0], ll_sel manual value (default 2'h0) (ES1.0)
  #define BIT_MSK__TDMLLCTL__REG_TTX_LL_SEL_MANUAL                                 0x0C
  // (ReadWrite, Bits 5:4) 
  // ri_tdm_rx_ll_sel_mode[1:0], 0=LL disable 1=LL enable(manual select), 2=LL enable(auto select), 3=invalid (default 2'h0) (ES1.0)
  #define BIT_MSK__TDMLLCTL__REG_TRX_LL_SEL_MODE                                   0x30
  // (ReadWrite, Bits 7:6) 
  // ri_tdm_rx_ll_sel_manual[1:0], ll_sel manual value (default 2'h0) (ES1.0)
  #define BIT_MSK__TDMLLCTL__REG_TRX_LL_SEL_MANUAL                                 0xC0

// TDM Byte Control Register
#define REG_ADDR__TDMBYTE                                                (Page_3 | 0x0073)
  // (ReadWrite, Bits 0) 
  // ri_tdm_tx_8bit_cbus_if, 0=ES0.0(16-bit interface) 1=ES1.0(8-bit interface) (default 1'b1)
  #define BIT_MSK__TDMBYTE__REG_TTX_8BIT_CBUS_IF                                  0x01
  // (ReadWrite, Bits 1) 
  // ri_tdm_rx_8bit_cbus_if, 0=ES0.0(16-bit interface) 1=ES1.0(8-bit interface) (default 1'b1)
  #define BIT_MSK__TDMBYTE__REG_TRX_8BIT_CBUS_IF                                  0x02
  // (ReadWrite, Bits 2) 
  // ri_tdm_tx_slot0_interleaving, 0(disable ES0.0), 1(enable ES1.0)
  #define BIT_MSK__TDMBYTE__REG_TTX_SLOT0_INTERLEAVING                            0x04

// TDM Interleaving Target Register
#define REG_ADDR__S0TGT                                                  (Page_3 | 0x0074)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_slot0_target[7:0], tdm_tx slot 0 position offset from tdm_tx slot 0 (CoC=8'd12/d13, DoC=8'd100)
  #define BIT_MSK__S0TGT__REG_TTX_S0_TARGET                                     0xFF

// TDM Interleaving Allowance Register
#define REG_ADDR__S0ALLOW                                                (Page_3 | 0x0075)
  // (ReadWrite, Bits 7:0) 
  // ri_tdm_tx_slot0_allowance[7:0], tdm_tx slot 0 position offset allowance (CoC=8'd2, DoC=8'd20)
  #define BIT_MSK__S0ALLOW__REG_TTX_S0_ALLOWANCE                                  0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_1                                        (Page_3 | 0x0091)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_1__REG_BIST_CTRL_1                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_2                                        (Page_3 | 0x0092)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_2__REG_BIST_CTRL_2                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_3                                        (Page_3 | 0x0093)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_3__REG_BIST_CTRL_3                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_4                                        (Page_3 | 0x0094)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_4__REG_BIST_CTRL_4                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_5                                        (Page_3 | 0x0095)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_5__REG_BIST_CTRL_5                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_6                                        (Page_3 | 0x0096)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_6__REG_BIST_CTRL_6                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_7                                        (Page_3 | 0x0097)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_7__REG_BIST_CTRL_7                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_8                                        (Page_3 | 0x0098)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_8__REG_BIST_CTRL_8                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_9                                        (Page_3 | 0x0099)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_9__REG_BIST_CTRL_9                                       0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_10                                       (Page_3 | 0x009A)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_10__REG_BIST_CTRL_10                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_11                                       (Page_3 | 0x009B)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_11__REG_BIST_CTRL_11                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_12                                       (Page_3 | 0x009C)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_12__REG_BIST_CTRL_12                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_13                                       (Page_3 | 0x009D)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_13__REG_BIST_CTRL_13                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_14                                       (Page_3 | 0x009E)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_14__REG_BIST_CTRL_14                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_15                                       (Page_3 | 0x009F)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_15__REG_BIST_CTRL_15                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_16                                       (Page_3 | 0x00A0)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_16__REG_BIST_CTRL_16                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_17                                       (Page_3 | 0x00A1)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_17__REG_BIST_CTRL_17                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_18                                       (Page_3 | 0x00A2)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_18__REG_BIST_CTRL_18                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_19                                       (Page_3 | 0x00A3)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_19__REG_BIST_CTRL_19                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_20                                       (Page_3 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // TMDS BIST config control register
  #define BIT_MSK__RX_TBIST_CTRL_20__REG_BIST_CTRL_20                                      0xFF

// Config Register
#define REG_ADDR__RX_TBIST_CTRL_21                                       (Page_3 | 0x00A5)
  // (ReadWrite, Bits 7:0) 
  // bit #0 - reg_rxbist_vgb_en bit #1 - reg_tmds0_bist_vgb_mask bit #2 - enbaled tx tbist bit #3 - enable txbist_vgb
  #define BIT_MSK__RX_TBIST_CTRL_21__REG_BIST_CTRL_21                                      0xFF

// HANA BIST BLUE CONTROL0 Register
#define REG_ADDR__CONFIG_BIST_10BIT_B_1ST_0                              (Page_3 | 0x00A7)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist blue 
  #define BIT_MSK__CONFIG_BIST_10BIT_B_1ST_0__REG_BIST_10BIT_BLUE_1ST_B7_B0                         0xFF

// HANA BIST BLUE CONTROL1 Register
#define REG_ADDR__CONFIG_BIST_10BIT_B_1ST_1                              (Page_3 | 0x00A8)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist blue 
  #define BIT_MSK__CONFIG_BIST_10BIT_B_1ST_1__REG_BIST_10BIT_BLUE_1ST_B9_B8                         0x03
  // (ReadWrite, Bits 7:2) 
  // reserved
  #define BIT_MSK__CONFIG_BIST_10BIT_B_1ST_1__RSVD                                                  0xFC

// HANA BIST BLUE CONTROL 2 Register
#define REG_ADDR__CONFIG_BIST_10BIT_B_2ND_0                              (Page_3 | 0x00A9)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist blue 
  #define BIT_MSK__CONFIG_BIST_10BIT_B_2ND_0__REG_BIST_10BIT_BLUE_2ND_B7_B0                         0xFF

// HANA BIST BLUE CONTROL3 Register
#define REG_ADDR__CONFIG_BIST_10BIT_B_2ND_1                              (Page_3 | 0x00AA)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist blue 
  #define BIT_MSK__CONFIG_BIST_10BIT_B_2ND_1__REG_BIST_10BIT_BLUE_2ND_B9_B8                         0x03

// HANA BIST Green CONTROL0 Register
#define REG_ADDR__CONFIG_BIST_10BIT_G_1ST_0                              (Page_3 | 0x00AB)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist Green
  #define BIT_MSK__CONFIG_BIST_10BIT_G_1ST_0__REG_BIST_10BIT_GREEN_1ST_B7_B0                        0xFF

// HANA BIST Green CONTROL1 Register
#define REG_ADDR__CONFIG_BIST_10BIT_G_1ST_1                              (Page_3 | 0x00AC)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist Green
  #define BIT_MSK__CONFIG_BIST_10BIT_G_1ST_1__REG_BIST_10BIT_GREEN_1ST_B9_B8                        0x03

// HANA BIST Green CONTROL2 Register
#define REG_ADDR__CONFIG_BIST_10BIT_G_2ND_0                              (Page_3 | 0x00AD)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist Green
  #define BIT_MSK__CONFIG_BIST_10BIT_G_2ND_0__REG_BIST_10BIT_GREEN_2ND_B7_B0                        0xFF

// HANA BIST Green CONTROL3 Register
#define REG_ADDR__CONFIG_BIST_10BIT_G_2ND_1                              (Page_3 | 0x00AE)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist Green
  #define BIT_MSK__CONFIG_BIST_10BIT_G_2ND_1__REG_BIST_10BIT_GREEN_2ND_B9_B8                        0x03

// HANA BIST Red CONTROL0 Register
#define REG_ADDR__CONFIG_BIST_10BIT_R_1ST_0                              (Page_3 | 0x00AF)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist Red
  #define BIT_MSK__CONFIG_BIST_10BIT_R_1ST_0__REG_BIST_10BIT_RED_1ST_B7_B0                          0xFF

// HANA BIST Red CONTROL1 Register
#define REG_ADDR__CONFIG_BIST_10BIT_R_1ST_1                              (Page_3 | 0x00B0)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist Red
  #define BIT_MSK__CONFIG_BIST_10BIT_R_1ST_1__REG_BIST_10BIT_RED_1ST_B9_B8                          0x03

// HANA BIST Red CONTROL2 Register
#define REG_ADDR__CONFIG_BIST_10BIT_R_2ND_0                              (Page_3 | 0x00B1)
  // (ReadWrite, Bits 7:0) 
  // Hana Bist Red
  #define BIT_MSK__CONFIG_BIST_10BIT_R_2ND_0__REG_BIST_10BIT_RED_2ND_B7_B0                          0xFF

// HANA BIST Red CONTROL3 Register
#define REG_ADDR__CONFIG_BIST_10BIT_R_2ND_1                              (Page_3 | 0x00B2)
  // (ReadWrite, Bits 1:0) 
  // Hana Bist Red
  #define BIT_MSK__CONFIG_BIST_10BIT_R_2ND_1__REG_BIST_10BIT_RED_2ND_B9_B8                          0x03

// TMDS STPG LUM #0 Register
#define REG_ADDR__STPG_LUM0                                              (Page_3 | 0x00B4)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel R  for STPG Simp92 with Zone ID V5_H4 - V5_H13
  #define BIT_MSK__STPG_LUM0__V5_H4TO13_R                                           0xFF

// TMDS STPG LUM #1 Register
#define REG_ADDR__STPG_LUM1                                              (Page_3 | 0x00B5)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel G  for STPG Simp92 with Zone ID V5_H4 - V5_H13
  #define BIT_MSK__STPG_LUM1__V5_H4TO13_G                                           0xFF

// TMDS STPG LUM #2 Register
#define REG_ADDR__STPG_LUM2                                              (Page_3 | 0x00B6)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel B  for STPG Simp92 with Zone ID V5_H4 - V5_H13
  #define BIT_MSK__STPG_LUM2__V5_H4TO13_B                                           0xFF

// TMDS STPG LUM #3 Register
#define REG_ADDR__STPG_LUM0B                                             (Page_3 | 0x00B7)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel R for STPG Simp92 with Zone V7_H4 - V7_H13
  #define BIT_MSK__STPG_LUM0B__V7_H4TO13_R                                           0xFF

// TMDS STPG LUM #4 Register
#define REG_ADDR__STPG_LUM1B                                             (Page_3 | 0x00B8)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel G for STPG Simp92 with Zone V7_H4 - V7_H13
  #define BIT_MSK__STPG_LUM1B__V7_H4TO13_G                                           0xFF

// TMDS STPG LUM #5 Register
#define REG_ADDR__STPG_LUM2B                                             (Page_3 | 0x00B9)
  // (ReadWrite, Bits 7:0) 
  // Programmable value for channel B for STPG Simp92 with Zone V7_H4 - V7_H13
  #define BIT_MSK__STPG_LUM2B__V7_H4TO13_B                                           0xFF

// TMDS STPG #0 Register
#define REG_ADDR__REG_V5_H4TO13                                          (Page_3 | 0x00BA)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V5H4TO13
  #define BIT_MSK__REG_V5_H4TO13__V5_H4TO13                                             0xFF

// TMDS STPG #1 Register
#define REG_ADDR__REG_V6_H2                                              (Page_3 | 0x00BB)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H2
  #define BIT_MSK__REG_V6_H2__V6_H2                                                 0xFF

// TMDS STPG #2 Register
#define REG_ADDR__REG_V6_H3                                              (Page_3 | 0x00BC)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H3
  #define BIT_MSK__REG_V6_H3__V6_H3                                                 0xFF

// TMDS STPG #3 Register
#define REG_ADDR__REG_V6_H4                                              (Page_3 | 0x00BD)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H4
  #define BIT_MSK__REG_V6_H4__V6_H4                                                 0xFF

// TMDS STPG #4 Register
#define REG_ADDR__REG_V6_H5                                              (Page_3 | 0x00BE)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H5
  #define BIT_MSK__REG_V6_H5__V6_H5                                                 0xFF

// TMDS STPG #5 Register
#define REG_ADDR__REG_V6_H6                                              (Page_3 | 0x00BF)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H6
  #define BIT_MSK__REG_V6_H6__V6_H6                                                 0xFF

// TMDS STPG #6 Register
#define REG_ADDR__REG_V6_H7                                              (Page_3 | 0x00C0)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H7
  #define BIT_MSK__REG_V6_H7__V6_H7                                                 0xFF

// TMDS STPG #7 Register
#define REG_ADDR__REG_V6_H8                                              (Page_3 | 0x00C1)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H8
  #define BIT_MSK__REG_V6_H8__V6_H8                                                 0xFF

// TMDS STPG #8 Register
#define REG_ADDR__REG_V6_H9                                              (Page_3 | 0x00C2)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H9
  #define BIT_MSK__REG_V6_H9__V6_H9                                                 0xFF

// TMDS STPG #9 Register
#define REG_ADDR__REG_V6_H10                                             (Page_3 | 0x00C3)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H10
  #define BIT_MSK__REG_V6_H10__V6_H10                                                0xFF

// TMDS STPG #10 Register
#define REG_ADDR__REG_V6_H11                                             (Page_3 | 0x00C4)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H11
  #define BIT_MSK__REG_V6_H11__V6_H11                                                0xFF

// TMDS STPG #11 Register
#define REG_ADDR__REG_V6_H12                                             (Page_3 | 0x00C5)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H12
  #define BIT_MSK__REG_V6_H12__V6_H12                                                0xFF

// TMDS STPG #12 Register
#define REG_ADDR__REG_V6_H13                                             (Page_3 | 0x00C6)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H13
  #define BIT_MSK__REG_V6_H13__V6_H13                                                0xFF

// TMDS STPG #13 Register
#define REG_ADDR__REG_V6_H14                                             (Page_3 | 0x00C7)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H14
  #define BIT_MSK__REG_V6_H14__V6_H14                                                0xFF

// TMDS STPG #14 Register
#define REG_ADDR__REG_V6_H15                                             (Page_3 | 0x00C8)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H15
  #define BIT_MSK__REG_V6_H15__V6_H15                                                0xFF

// TMDS STPG #15 Register
#define REG_ADDR__REG_V6_H16                                             (Page_3 | 0x00C9)
  // (ReadWrite, Bits 7:0) 
  // NVRAM data for V6H16
  #define BIT_MSK__REG_V6_H16__V6_H16                                                0xFF

// TMDS STPG #16 Register
#define REG_ADDR__REG_V7_CONST61                                         (Page_3 | 0x00CA)
  // (ReadWrite, Bits 7:0) 
  // data for V7
  #define BIT_MSK__REG_V7_CONST61__V7_CONST61                                            0xFF

// TMDS STPG #17 Register
#define REG_ADDR__REG_V7_CONST50                                         (Page_3 | 0x00CB)
  // (ReadWrite, Bits 7:0) 
  // data for V7
  #define BIT_MSK__REG_V7_CONST50__V7_CONST50                                            0xFF

// TMDS STPG #18 Register
#define REG_ADDR__REG_V7_CONST102                                        (Page_3 | 0x00CC)
  // (ReadWrite, Bits 7:0) 
  // data for V7
  #define BIT_MSK__REG_V7_CONST102__V7_CONST102                                           0xFF

// TMDS STPG #19 Register
#define REG_ADDR__REG_V9_CONST204                                        (Page_3 | 0x00CD)
  // (ReadWrite, Bits 7:0) 
  // data for V9
  #define BIT_MSK__REG_V9_CONST204__V9_CONST204                                           0xFF

// TMDS STPG #20 Register
#define REG_ADDR__REG_V9_CONST205                                        (Page_3 | 0x00CE)
  // (ReadWrite, Bits 7:0) 
  // data for V9
  #define BIT_MSK__REG_V9_CONST205__V9_CONST205                                           0xFF

// TMDS STPG #21 Register
#define REG_ADDR__REG_V9_CONST209                                        (Page_3 | 0x00CF)
  // (ReadWrite, Bits 7:0) 
  // data for V9
  #define BIT_MSK__REG_V9_CONST209__V9_CONST209                                           0xFF

// TMDS STPG #22 Register
#define REG_ADDR__REG_V9_CONST229                                        (Page_3 | 0x00D0)
  // (ReadWrite, Bits 7:0) 
  // data for V9
  #define BIT_MSK__REG_V9_CONST229__V9_CONST229                                           0xFF

// TMDS STPG #23 Register
#define REG_ADDR__REG_V9_CONST231                                        (Page_3 | 0x00D1)
  // (ReadWrite, Bits 7:0) 
  // data for V9
  #define BIT_MSK__REG_V9_CONST231__V9_CONST231                                           0xFF

// TMDS STPG #24 Register
#define REG_ADDR__REG_V10_H3TO4                                          (Page_3 | 0x00D2)
  // (ReadWrite, Bits 7:0) 
  // V10_H3TO4
  #define BIT_MSK__REG_V10_H3TO4__V10_H3TO4                                             0xFF

// TMDS STPG #25 Register
#define REG_ADDR__REG_V10_H5TO6                                          (Page_3 | 0x00D3)
  // (ReadWrite, Bits 7:0) 
  // V10_H5TO6
  #define BIT_MSK__REG_V10_H5TO6__V10_H5TO6                                             0xFF

// TMDS STPG #26 Register
#define REG_ADDR__REG_V10_H7TO8                                          (Page_3 | 0x00D4)
  // (ReadWrite, Bits 7:0) 
  // V10_H7TO8
  #define BIT_MSK__REG_V10_H7TO8__V10_H7TO8                                             0xFF

// TMDS STPG #27 Register
#define REG_ADDR__REG_V10_H9TO10                                         (Page_3 | 0x00D5)
  // (ReadWrite, Bits 7:0) 
  // V10_H9TO10
  #define BIT_MSK__REG_V10_H9TO10__V10_H9TO10                                            0xFF

// TMDS STPG #28 Register
#define REG_ADDR__REG_V10_H11TO12                                        (Page_3 | 0x00D6)
  // (ReadWrite, Bits 7:0) 
  // V10_H11TO12
  #define BIT_MSK__REG_V10_H11TO12__V10_H11TO12                                           0xFF

// TMDS STPG #29 Register
#define REG_ADDR__REG_V10_H13TO14                                        (Page_3 | 0x00D7)
  // (ReadWrite, Bits 7:0) 
  // V10_H13TO14
  #define BIT_MSK__REG_V10_H13TO14__V10_H13TO14                                           0xFF

// TMDS 1 BIST CNTL Register
#define REG_ADDR__BIST_CTRL                                              (Page_3 | 0x00D8)
  // (ReadWrite, Bits 0) 
  // 0  bist is disabled       1 -  bist is enabled        Whenever bist is enabled normal operation of the chip ceases and the chip goes into bist mode. 
  #define BIT_MSK__BIST_CTRL__REG_BIST_ENABLE                                       0x01
  // (ReadWrite, Bits 1) 
  // 0  the bist module is out of  reset state      1  the bist module is  forced to be in the reset state.         The Bist reset bit is provided as a backup. Normal operation of Bist does not require any toggling of this bit. This is used only for lab debug.
  #define BIT_MSK__BIST_CTRL__REG_BIST_RESET                                        0x02
  // (ReadWrite, Bits 2) 
  // The enable bit for Samsung Test Pattern Generator. When set, it have higher priority than other BIST settings and reg_stpg_sel[3:0] will decide which pattern to generate. When clear, STPG is disabled. 1'b 0 -- clear (default) 1'b 1 -- set
  #define BIT_MSK__BIST_CTRL__REG_STPG_EN                                           0x04
  // (ReadWrite, Bits 3) 
  // 1b 0  The bist module runs the pattern check  for programmed number of frames. 1b 1  The bist module runs continuously till the bist is disabled.
  #define BIT_MSK__BIST_CTRL__REG_BIST_CONT_PROG_DURAT                              0x08
  // (ReadWrite, Bits 4) 
  // Bist start register is a write only  register. To start a Bist sequence 1 to this register. This will generate a pulse. The falling edge of the pulse triggers the bist. Read to this register will provide unknown value.All bist control logic gets reset at the start of a bist including the read only status registers.  Other configuration registers do not get reset.
  #define BIT_MSK__BIST_CTRL__REG_BIST_START                                        0x10

// TMDS 1 BIST DURATION0 Register
#define REG_ADDR__BIST_DURATION0                                         (Page_3 | 0x00D9)
  // (ReadWrite, Bits 7:0) 
  // BIST Duration register is used to specify the duration of a test in frames. Counting of frames starts with the first vsync falling edge detected after the bist has been enabled. Also a programmed value of n results in n+1 frames. (If zero is programmed; the bist/tx is active for 1 frame)
  #define BIT_MSK__BIST_DURATION0__REG_BIST_DURATION_B7_B0                               0xFF

// TMDS 1 BIST DURATION1 Register
#define REG_ADDR__BIST_DURATION1                                         (Page_3 | 0x00DA)
  // (ReadWrite, Bits 7:0) 
  // Bist duration[15:8]; total is 23 bits.
  #define BIT_MSK__BIST_DURATION1__REG_BIST_DURATION_B15_B8                              0xFF

// TMDS 1 BIST DURATION2 Register
#define REG_ADDR__BIST_DURATION2                                         (Page_3 | 0x00DB)
  // (ReadWrite, Bits 7:0) 
  // Bist duration[23:16]; total is 24 bits.
  #define BIT_MSK__BIST_DURATION2__REG_BIST_DURATION_B23_B16                             0xFF

// TMDS 1 BIST TEST_SEL Register
#define REG_ADDR__BIST_TEST_SEL                                          (Page_3 | 0x00DC)
  // (ReadWrite, Bits 4:0) 
  // 5b 0000X  Walking one pattern 5b 0001X   Ramping pattern       5b 0010X  LFSR pattern1         5b 0011X  LFSR pattern 2          5b 0100X  Static pattern 1           5b 0101X  Static pattern 2             5b 0110X  Max switching pattern            5b 0111X  Two column max- switching pattern            5b 1000X  SDVO/TMDS half clock pattern       5b 1001X  SDVO/TMDS 8 bit  static  pattern         5b 1010X  TMDS 10 bit static  pattern.  5'b 1011X - Enable all 4 TMDS patterns at one time      The pattern corresponding to 4h 10  is applicable only to TMDS transmit and are not applicable for SDVO receive test.
  #define BIT_MSK__BIST_TEST_SEL__REG_BIST_PATTERN_SELECT                               0x1F
  // (ReadWrite, Bits 6:5) 
  // 00-CTL lines[3:0] are static (00);  01-CTL lines[3:0] follow a ramp pattern where each count is valid for 2 clocks;  10-CTL line[3:0] follow a random pattern where each pattern is valid for 2 clocks;  11-reserved (do not use)
  #define BIT_MSK__BIST_TEST_SEL__REG_BIST_TEST_SELECT                                  0x60

// TMDS 1 BIST VIDEO_MODE Register
#define REG_ADDR__BIST_VIDEO_MODE                                        (Page_3 | 0x00DD)
  // (ReadWrite, Bits 2:0) 
  // reserved
  #define BIT_MSK__BIST_VIDEO_MODE__RSVD                                                  0x07
  // (ReadWrite, Bits 3) 
  // Setting this bit 1 forces the DE going to the TMDS core to be forced to be 1.
  #define BIT_MSK__BIST_VIDEO_MODE__REG_BIST_VIDEO_MODE                                   0x08
  // (ReadWrite, Bits 7:4) 
  // Samsung Test Pattern select. This only takes effect when the enable bit (reg_stpg_en) is set. 4b 0000  Full screen red (default)   4b 0001  Full screen green    4b 0010  Full screen blue                   4b 0011  Full screen black    4b 0100  Full screen white   4b 0101  Ramp, from (0, 0, 0) to (255, 255, 255), each color repeats 5 pixels 4b 0110  Chess. The screen is divided into 8x6 blocks, with the size of 160x120. Black and white blocks alternate horizontally and vertically. 4b 0111  Color bar. 8 color strips with the width of 160 pixels. 4b 1000  Simp92. Simplified version of Master Pattern Generator #92 others      reserved
  #define BIT_MSK__BIST_VIDEO_MODE__REG_STPG_SEL                                          0xF0

// TMDS 1 BIST 8BIT_PATTERN Register
#define REG_ADDR__BIST_8BIT_PATTERN                                      (Page_3 | 0x00DE)
  // (ReadWrite, Bits 7:0) 
  // 
  #define BIT_MSK__BIST_8BIT_PATTERN__REG_BIST_8BIT_PATTERN                                 0xFF

// TMDS 1 BIST 10BIT_PATTERN_L Register
#define REG_ADDR__BIST_10BIT_PATTERN_L                                   (Page_3 | 0x00DF)
  // (ReadWrite, Bits 7:0) 
  // Lower 8 bits of the static 10-bit pattern to be provided to the TMDS transmitter
  #define BIT_MSK__BIST_10BIT_PATTERN_L__REG_BIST_10BIT_PATTERN_B7_B0                          0xFF

// TMDS 1 BIST 10BIT_PATTERN_U Register
#define REG_ADDR__BIST_10BIT_PATTERN_U                                   (Page_3 | 0x00E0)
  // (ReadWrite, Bits 1:0) 
  // Upper 2 bits of the static 10-bit pattern to be provided to the TMDS transmitter
  #define BIT_MSK__BIST_10BIT_PATTERN_U__REG_BIST_10BIT_PATTERN_B9_B8                          0x03

// TMDS 1 BIST status Register
#define REG_ADDR__BIST_STATUS                                            (Page_3 | 0x00E1)
  // (ReadOnly, Bits 1:0) 
  // [1:0]  Bist idle/busy/complete bits 00 b  Bist is idle 01 b  Bist is busy (test in           progress) 10 b  Bist is complete and result is ready in the BIST_RESULT  register
  #define BIT_MSK__BIST_STATUS__BIST_CONFIG_STATUS                                    0x03

// TMDS 1 BIST RESULT Register
#define REG_ADDR__BIST_RESULT                                            (Page_3 | 0x00E2)
  // (ReadOnly, Bits 0) 
  // Bist pass/fail bit        0  bist test passed        1  bist test failed
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_FAIL                                      0x01
  // (ReadOnly, Bits 1) 
  // SDVO receive. hsync error
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_HSYNC_ERROR                               0x02
  // (ReadOnly, Bits 2) 
  // SDVO receive. vsync error
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_VSYNC_ERROR                               0x04
  // (ReadOnly, Bits 3) 
  // SDVO receive. de error  
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_DE_ERROR                                  0x08
  // (ReadOnly, Bits 4) 
  // SDVO receive. error count overflow . This bit gets set if  any of the error counters         overflows the max 16 bit count. If this bit is set it may  not be possible to pinpoint  which counter overflowed.All the error bits are valid only if the bit 0 indicates that bist test failed.
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_OVERFLOW                                  0x10
  // (ReadOnly, Bits 5) 
  // SDVO receive ctl error.
  #define BIT_MSK__BIST_RESULT__BIST_CONFIG_CTL_ERROR                                 0x20
  // (ReadWrite, Bits 7:6) 
  // reserved
  #define BIT_MSK__BIST_RESULT__RSVD                                                  0xC0

// TMDS 1 BIST P_ERROR_COUNT_0 Register
#define REG_ADDR__BIST_P_ERR_CNT_0                                       (Page_3 | 0x00E3)
  // (ReadOnly, Bits 7:0) 
  // Lower 8 bits of the pixel error count. This register is a read only register. Its contents are valid only if the bist test failed.A received pixel is in error if any of the 3 channels (Red. Green. Blue) have a value mismatch between generated value and received value.
  #define BIT_MSK__BIST_P_ERR_CNT_0__BIST_P_ERROR_COUNT_0                                  0xFF

// TMDS 1 BIST P_ERROR_COUNT_1 Register
#define REG_ADDR__BIST_P_ERR_CNT_1                                       (Page_3 | 0x00E4)
  // (ReadOnly, Bits 7:0) 
  // Upper 8 bits of the pixel error count. This register is a read only register. Its contents are valid only if the bist test failed.A received pixel is in error if any of the 3 channels (Red. Green. Blue) have a value mismatch between generated value and received value.If the any of the error count registers exceeds the max 16 bit count then the overflow bit is set in BIST_status register.
  #define BIT_MSK__BIST_P_ERR_CNT_1__BIST_P_ERROR_COUNT_1                                  0xFF

// TMDS 1 BIST R_ERROR_COUNT_0 Register
#define REG_ADDR__BIST_R_ERR_CNT_0                                       (Page_3 | 0x00E5)
  // (ReadOnly, Bits 7:0) 
  // Lower 8 bits of the red channel error count. This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if   there is a mismatch between   generated 8 bit value and received 8 bit value
  #define BIT_MSK__BIST_R_ERR_CNT_0__BIST_R_ERROR_COUNT_0                                  0xFF

// TMDS 1 BIST R_ERROR_COUNT_1 Register
#define REG_ADDR__BIST_R_ERR_CNT_1                                       (Page_3 | 0x00E6)
  // (ReadOnly, Bits 7:0) 
  // Upper 8 bits of the red channel error count. This register is a read only register. Its contents are valid only if the bist test failed. A channel is in error if there is a mismatch between generated 8-bit values and received 8-bit value.If the any of the error count registers exceeds the max 16 bit count then the overflow bit is set in BIST_status register.
  #define BIT_MSK__BIST_R_ERR_CNT_1__BIST_R_ERROR_COUNT_1                                  0xFF

// TMDS 1 BIST G_ERROR_COUNT_0 Register
#define REG_ADDR__BIST_G_ERR_CNT_0                                       (Page_3 | 0x00E7)
  // (ReadOnly, Bits 7:0) 
  // Lower 8 bits of the green channel error count. This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if   there is a mismatch between   generated 8-bit values and received 8-bit value.
  #define BIT_MSK__BIST_G_ERR_CNT_0__BIST_G_ERROR_COUNT_0                                  0xFF

// TMDS 1 BIST G_ERROR_COUNT_1 Register
#define REG_ADDR__BIST_G_ERR_CNT_1                                       (Page_3 | 0x00E8)
  // (ReadOnly, Bits 7:0) 
  // Upper 8 bits of the green channel error count. This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if there is a mismatch between generated 8-bit values and received 8-bit value.If the any of the error count registers exceeds the max 16 bit count then the overflow bit is set in BIST_status register.
  #define BIT_MSK__BIST_G_ERR_CNT_1__BIST_G_ERROR_COUNT_1                                  0xFF

// TMDS 1 BIST B_ERROR_COUNT_0 Register
#define REG_ADDR__BIST_B_ERR_CNT_0                                       (Page_3 | 0x00E9)
  // (ReadOnly, Bits 7:0) 
  // Lower 8 bits of the blue channel error count. This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if   there is a mismatch between   generated 8 bit value and receive
  #define BIT_MSK__BIST_B_ERR_CNT_0__BIST_B_ERROR_COUNT_0                                  0xFF

// TMDS 1 BIST B_ERROR_COUNT_1 Register
#define REG_ADDR__BIST_B_ERR_CNT_1                                       (Page_3 | 0x00EA)
  // (ReadOnly, Bits 7:0) 
  // Upper 8 bits of the blue channel error count. This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if there is a mismatch between generated 8-bit values and received 8-bit value.If the any of the error count registers exceeds the max 16 bit count then the overflow bit is set in BIST_status register.
  #define BIT_MSK__BIST_B_ERR_CNT_1__BIST_B_ERROR_COUNT_1                                  0xFF

// TMDS 1 BIST CNTL_ERROR_COUNT Register
#define REG_ADDR__BIST_CNTL_ERR_CNT                                      (Page_3 | 0x00EB)
  // (ReadOnly, Bits 7:0) 
  // 8 bit counter for counting the number of control erros.  This register is a read only register. Its contents are valid only if the bist test failed.A channel is in error if there is a mismatch between generated control[3:0] values and received control[3:0] values.
  #define BIT_MSK__BIST_CNTL_ERR_CNT__BIST_CNTL_ERROR_COUNT                                 0xFF

// BIST STPG Size1 Register
#define REG_ADDR__STPG_SIZE1                                             (Page_3 | 0x00EC)
  // (ReadWrite, Bits 7:0) 
  // This register byte defines the height of V1, V6 and V10 for Simp92. Default is 8'h3C. STPG_SIZE1x3 + STPG_SIZE2x5 + STPG_SIZE3x2 must be equal to 720.
  #define BIT_MSK__STPG_SIZE1__REG_V1610_HEIGHT                                      0xFF

// BIST STPG Size2 Register
#define REG_ADDR__STPG_SIZE2                                             (Page_3 | 0x00ED)
  // (ReadWrite, Bits 7:0) 
  // This register byte defines the height of V2, V3, V4, V8 and V9 for Simp92. Default is 8'h1E. STPG_SIZE1x3 + STPG_SIZE2x5 + STPG_SIZE3x2 must be equal to 720.
  #define BIT_MSK__STPG_SIZE2__REG_V23489_HEIGHT                                     0xFF

// BIST STPG Size3 Register
#define REG_ADDR__STPG_SIZE3                                             (Page_3 | 0x00EE)
  // (ReadWrite, Bits 7:0) 
  // This register byte defines the height of V5 and V7 for Simp92. Default is 8'hC3. STPG_SIZE1x3 + STPG_SIZE2x5 + STPG_SIZE3x2 must be equal to 720.
  #define BIT_MSK__STPG_SIZE3__REG_V57_HEIGHT                                        0xFF

// Fake Vid Ctrl Register
#define REG_ADDR__BIST_CTRL2                                             (Page_3 | 0x00F5)
  // (ReadWrite, Bits 0) 
  // if 1 invert hsync
  #define BIT_MSK__BIST_CTRL2__REG_INV_HSYNC_EN                                      0x01
  // (ReadWrite, Bits 1) 
  // if 1 invert vsync
  #define BIT_MSK__BIST_CTRL2__REG_INV_VSYNC_EN                                      0x02
  // (ReadWrite, Bits 2) 
  // if 0 enable STPG out, else Tx BIST
  #define BIT_MSK__BIST_CTRL2__REG_OUT_SEL                                           0x04
  // (ReadWrite, Bits 7:3) 
  // reserved
  #define BIT_MSK__BIST_CTRL2__RSVD                                                  0xF8

// Fake Vid Ctrl Register
#define REG_ADDR__BIST_TIMING_CTRL                                       (Page_3 | 0x00F6)
  // (ReadWrite, Bits 1:0) 
  // select # of bits: 00 - 8bpp 01 - 10bpp 10 - 12bpp
  #define BIT_MSK__BIST_TIMING_CTRL__REG_DEPTH                                             0x03
  // (ReadWrite, Bits 3:2) 
  // select refresh rate:     for 4k2k:         00 - 30         01 - 25         10 - 24         11 - 24SMPTE    for 1080p:          00 - 50           01 - 60
  #define BIT_MSK__BIST_TIMING_CTRL__REG_REFRESH                                           0x0C
  // (ReadWrite, Bits 7:4) 
  // This register controls BIST timing including video timing.           8'b0000:                    //720p60 case           8'b0001:                     //576p case           8'b0010:                     //480p case           8'b0011:                     //1080p@50 8bpp case           8'b0100:                     //4k2k@30           8'b0101:                     //fully programmable           8'b1000:                     //vga            8'b1001:                     //svga           8'b1010:                     //xga           8'b1011:                      //sxga           8'b1100:                     //uxga           8'b1101:                      //4k2k@30 case
  #define BIT_MSK__BIST_TIMING_CTRL__REG_TIME_MODE                                         0xF0

//***************************************************************************
// page 34. Address: 40
// MHL Capability 1st Byte Register
#define REG_ADDR__MHL_DEVCAP_0                                           (Page_4 | 0x0000)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 1st byte. Identify current connected and powered state. Default value Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_DEVCAP_0__REG_MHL_DEVCAP_0                                      0xFF

// MHL Capability 2nd Byte Register
#define REG_ADDR__MHL_DEVCAP_1                                           (Page_4 | 0x0001)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 2nd byte. Identify level of MHL Spec supported. Default value Tx: 0x10: Rx:0x10: Dongle: 0x20
  #define BIT_MSK__MHL_DEVCAP_1__REG_MHL_DEVCAP_1                                      0xFF

// MHL Capability 3rd Byte Register
#define REG_ADDR__MHL_DEVCAP_2                                           (Page_4 | 0x0002)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 3rd byte. Identify the type of MHL system. Default value Tx: 0x02: Rx: 0x11: Dongle: 0x13
  #define BIT_MSK__MHL_DEVCAP_2__REG_MHL_DEVCAP_2                                      0xFF

// MHL Capability 4th Byte Register
#define REG_ADDR__MHL_DEVCAP_3                                           (Page_4 | 0x0003)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 4th byte. High-order byte of Adopter identifier: assigned by MHL LLC. Default value: Tx: 0x00: Rx:0x00: Dongle: 0x01
  #define BIT_MSK__MHL_DEVCAP_3__REG_MHL_DEVCAP_3                                      0xFF

// MHL Capability 5th Byte Register
#define REG_ADDR__MHL_DEVCAP_4                                           (Page_4 | 0x0004)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 5th byte. Low-order byte of Adopter identifier: assigned by MHL LLC. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x42
  #define BIT_MSK__MHL_DEVCAP_4__REG_MHL_DEVCAP_4                                      0xFF

// MHL Capability 6th Byte Register
#define REG_ADDR__MHL_DEVCAP_5                                           (Page_4 | 0x0005)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 6th byte. List of link modes supported for video. Default value: Tx: 0x37: Rx: 0x37: Dongle: 0x3F
  #define BIT_MSK__MHL_DEVCAP_5__REG_MHL_DEVCAP_5                                      0xFF

// MHL Capability 7th Byte Register
#define REG_ADDR__MHL_DEVCAP_6                                           (Page_4 | 0x0006)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 7th byte. List of link modes supported for audio. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x03
  #define BIT_MSK__MHL_DEVCAP_6__REG_MHL_DEVCAP_6                                      0xFF

// MHL Capability 8th Byte Register
#define REG_ADDR__MHL_DEVCAP_7                                           (Page_4 | 0x0007)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 8th byte. List of video types supported. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x8F
  #define BIT_MSK__MHL_DEVCAP_7__REG_MHL_DEVCAP_7                                      0xFF

// MHL Capability 9th Byte Register
#define REG_ADDR__MHL_DEVCAP_8                                           (Page_4 | 0x0008)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 9th byte. Map of logical device types. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x41
  #define BIT_MSK__MHL_DEVCAP_8__REG_MHL_DEVCAP_8                                      0xFF

// MHL Capability 10th Byte Register
#define REG_ADDR__MHL_DEVCAP_9                                           (Page_4 | 0x0009)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 10th byte. Upper bound of MHL link bandwidth. Default value: Tx: 0x0F: Rx: 0x0F: Dongle: 0x0F
  #define BIT_MSK__MHL_DEVCAP_9__REG_MHL_DEVCAP_9                                      0xFF

// MHL Capability 11th Byte Register
#define REG_ADDR__MHL_DEVCAP_A                                           (Page_4 | 0x000A)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 11th byte. Set flag for each MHL optional feature. Default value: Tx: 0x07: Rx: 0x07: Dongle: 0x07 
  #define BIT_MSK__MHL_DEVCAP_A__REG_MHL_DEVCAP_A                                      0xFF

// MHL Capability 12th Byte Register
#define REG_ADDR__MHL_DEVCAP_B                                           (Page_4 | 0x000B)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 12th byte. High-order byte of system identifier: assigned by Adopter. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x92
  #define BIT_MSK__MHL_DEVCAP_B__REG_MHL_DEVCAP_B                                      0xFF

// MHL Capability 13th Byte Register
#define REG_ADDR__MHL_DEVCAP_C                                           (Page_4 | 0x000C)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 13th byte. Low-order byte of system identifier: assigned by Adopter. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x96
  #define BIT_MSK__MHL_DEVCAP_C__REG_MHL_DEVCAP_C                                      0xFF

// MHL Capability 14th Byte Register
#define REG_ADDR__MHL_DEVCAP_D                                           (Page_4 | 0x000D)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 14th byte. Total count of Scratchpad registers. Default value: Tx: 0x10: Rx: 0x10: Dongle: 0x10
  #define BIT_MSK__MHL_DEVCAP_D__REG_MHL_DEVCAP_D                                      0xFF

// MHL Capability 15th Byte Register
#define REG_ADDR__MHL_DEVCAP_E                                           (Page_4 | 0x000E)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 15th byte. Total count of interrupt and status registers. Default value: Tx: 0x33: Rx: 0x33: Dongle: 0x33
  #define BIT_MSK__MHL_DEVCAP_E__REG_MHL_DEVCAP_E                                      0xFF

// MHL Capability 16th Byte Register
#define REG_ADDR__MHL_DEVCAP_F                                           (Page_4 | 0x000F)
  // (ReadWrite, Bits 7:0) 
  // MHL capability registers 16th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_DEVCAP_F__REG_MHL_DEVCAP_F                                      0xFF

// MHL Interrupt 1st Byte Register
#define REG_ADDR__MHL_INT_0                                              (Page_4 | 0x0020)
  // (ReadWrite, Bits 0) 
  // MHL interrupt registers 1st byte; status 0. DCAP_CHG: Device Capability Register value changed
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT0                                   0x01
  // (ReadWrite, Bits 1) 
  // MHL interrupt registers 1st byte; status 1. DSCR_CHG: Device Scratchpad Register value changed
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT1                                   0x02
  // (ReadWrite, Bits 2) 
  // MHL interrupt registers 1st byte; status 2. REQ_WRT: Request-to-Write scratchpad
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT2                                   0x04
  // (ReadWrite, Bits 3) 
  // MHL interrupt registers 1st byte; status 3. GRT-WRT: Grant-to-Write scratchpad
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT3                                   0x08
  // (ReadWrite, Bits 4) 
  // MHL interrupt registers 1st byte; status 4. Request for 3D Information
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT4                                   0x10
  // (ReadWrite, Bits 5) 
  // MHL interrupt registers 1st byte; status 5. Request Feature information
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT5                                   0x20
  // (ReadWrite, Bits 6) 
  // MHL interrupt registers 1st byte; status 6. Indicate completion of sending Feature information.
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT6                                   0x40
  // (ReadWrite, Bits 7) 
  // MHL interrupt registers 1st byte; status 7. RSVD
  #define BIT_MSK__MHL_INT_0__REG_MHL_INT_0_STAT7                                   0x80

// MHL Interrupt 2nd Byte Register
#define REG_ADDR__MHL_INT_1                                              (Page_4 | 0x0021)
  // (ReadWrite, Bits 0) 
  // MHL interrupt registers 2nd byte; status 0. RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT0                                   0x01
  // (ReadWrite, Bits 1) 
  // MHL interrupt registers 2nd byte; status 1. EDID_CHG: EDID content change on virtual DDC
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT1                                   0x02
  // (ReadWrite, Bits 2) 
  // MHL interrupt registers 2nd byte; status 2. RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT2                                   0x04
  // (ReadWrite, Bits 3) 
  // MHL interrupt registers 2nd byte; status 3: RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT3                                   0x08
  // (ReadWrite, Bits 4) 
  // MHL interrupt registers 2nd byte; status 4: RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT4                                   0x10
  // (ReadWrite, Bits 5) 
  // MHL interrupt registers 2nd byte; status 5. RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT5                                   0x20
  // (ReadWrite, Bits 6) 
  // MHL interrupt registers 2nd byte; status 6. RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT6                                   0x40
  // (ReadWrite, Bits 7) 
  // MHL interrupt registers 2nd byte; status 7. RSVD
  #define BIT_MSK__MHL_INT_1__REG_MHL_INT_1_STAT7                                   0x80

// MHL Interrupt 3rd Byte Register
#define REG_ADDR__MHL_INT_2                                              (Page_4 | 0x0022)
  // (ReadWrite, Bits 0) 
  // MHL interrupt registers 3rd byte; status 0. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT0                                   0x01
  // (ReadWrite, Bits 1) 
  // MHL interrupt registers 3rd byte; status 1. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT1                                   0x02
  // (ReadWrite, Bits 2) 
  // MHL interrupt registers 3rd byte; status 2. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT2                                   0x04
  // (ReadWrite, Bits 3) 
  // MHL interrupt registers 3rd byte; status 3. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT3                                   0x08
  // (ReadWrite, Bits 4) 
  // MHL interrupt registers 3rd byte; status 4. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT4                                   0x10
  // (ReadWrite, Bits 5) 
  // MHL interrupt registers 3rd byte; status 5. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT5                                   0x20
  // (ReadWrite, Bits 6) 
  // MHL interrupt registers 3rd byte; status 6. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT6                                   0x40
  // (ReadWrite, Bits 7) 
  // MHL interrupt registers 3rd byte; status 7. RSVD
  #define BIT_MSK__MHL_INT_2__REG_MHL_INT_2_STAT7                                   0x80

// MHL Interrupt 4th Byte Register
#define REG_ADDR__MHL_INT_3                                              (Page_4 | 0x0023)
  // (ReadWrite, Bits 0) 
  // MHL interrupt registers 4th byte; status 0. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT0                                   0x01
  // (ReadWrite, Bits 1) 
  // MHL interrupt registers 4th byte; status 1. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT1                                   0x02
  // (ReadWrite, Bits 2) 
  // MHL interrupt registers 4th byte; status 2. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT2                                   0x04
  // (ReadWrite, Bits 3) 
  // MHL interrupt registers 4th byte; status 3. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT3                                   0x08
  // (ReadWrite, Bits 4) 
  // MHL interrupt registers 4th byte; status 4. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT4                                   0x10
  // (ReadWrite, Bits 5) 
  // MHL interrupt registers 4th byte; status 5. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT5                                   0x20
  // (ReadWrite, Bits 6) 
  // MHL interrupt registers 4th byte; status 6. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT6                                   0x40
  // (ReadWrite, Bits 7) 
  // MHL interrupt registers 4th byte; status 7. RSVD
  #define BIT_MSK__MHL_INT_3__REG_MHL_INT_3_STAT7                                   0x80

// Device Status 1st byte Register
#define REG_ADDR__MHL_STAT_0                                             (Page_4 | 0x0030)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 1st byte 0: DCAP_RDY. Capability Register values are stable. 1: XDEVCAP_SUPP. Indicates that Extended Device Capability registers are supported. 2: POW_SAT: When set device can output power across VBUS to the device connected through the cable. When clear device cannot output power across VBUS to the device connected through the cable. 5-3 PLIM_STAT: The device maintains a copy of the value of the field PLIM[2:0] 7-6: Reserved
  #define BIT_MSK__MHL_STAT_0__REG_MHL_STAT_0                                        0xFF

// Device Status 2nd byte Register
#define REG_ADDR__MHL_STAT_1                                             (Page_4 | 0x0031)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 2nd byte 0-2: CLK_MODE. The clock mode on the link 3: PATH_EN. Device's TMDS path is not in use 4: MUTED. Device's content stream is muted. 5-7: RSVD
  #define BIT_MSK__MHL_STAT_1__REG_MHL_STAT_1                                        0xFF

// Device Status 3rd byte Register
#define REG_ADDR__MHL_STAT_2                                             (Page_4 | 0x0032)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 3rd byte. 3-0: MHL_VER_MINOR 7-4: MHL_VER_MAJOR
  #define BIT_MSK__MHL_STAT_2__REG_MHL_STAT_2                                        0xFF

// Device Status 4th byte Register
#define REG_ADDR__MHL_STAT_3                                             (Page_4 | 0x0033)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 4th byte. RSVD
  #define BIT_MSK__MHL_STAT_3__REG_MHL_STAT_3                                        0xFF

// CBUS Scratch Pad 1st Byte Register
#define REG_ADDR__MHL_SCRPAD_0                                           (Page_4 | 0x0040)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 1st byte
  #define BIT_MSK__MHL_SCRPAD_0__REG_MHL_SCRPAD_0                                      0xFF

// CBUS Scratch Pad 2nd Byte Register
#define REG_ADDR__MHL_SCRPAD_1                                           (Page_4 | 0x0041)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 2nd byte
  #define BIT_MSK__MHL_SCRPAD_1__REG_MHL_SCRPAD_1                                      0xFF

// CBUS Scratch Pad 3rd Byte Register
#define REG_ADDR__MHL_SCRPAD_2                                           (Page_4 | 0x0042)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 3rd byte
  #define BIT_MSK__MHL_SCRPAD_2__REG_MHL_SCRPAD_2                                      0xFF

// CBUS Scratch Pad 4th Byte Register
#define REG_ADDR__MHL_SCRPAD_3                                           (Page_4 | 0x0043)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 4th byte
  #define BIT_MSK__MHL_SCRPAD_3__REG_MHL_SCRPAD_3                                      0xFF

// CBUS Scratch Pad 5th Byte Register
#define REG_ADDR__MHL_SCRPAD_4                                           (Page_4 | 0x0044)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 5th byte
  #define BIT_MSK__MHL_SCRPAD_4__REG_MHL_SCRPAD_4                                      0xFF

// CBUS Scratch Pad 6th Byte Register
#define REG_ADDR__MHL_SCRPAD_5                                           (Page_4 | 0x0045)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 6th byte
  #define BIT_MSK__MHL_SCRPAD_5__REG_MHL_SCRPAD_5                                      0xFF

// CBUS Scratch Pad 7th Byte Register
#define REG_ADDR__MHL_SCRPAD_6                                           (Page_4 | 0x0046)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 7th byte
  #define BIT_MSK__MHL_SCRPAD_6__REG_MHL_SCRPAD_6                                      0xFF

// CBUS Scratch Pad 8th Byte Register
#define REG_ADDR__MHL_SCRPAD_7                                           (Page_4 | 0x0047)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 8th byte
  #define BIT_MSK__MHL_SCRPAD_7__REG_MHL_SCRPAD_7                                      0xFF

// CBUS Scratch Pad 9th Byte Register
#define REG_ADDR__MHL_SCRPAD_8                                           (Page_4 | 0x0048)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 9th byte
  #define BIT_MSK__MHL_SCRPAD_8__REG_MHL_SCRPAD_8                                      0xFF

// CBUS Scratch Pad 10th Byte Register
#define REG_ADDR__MHL_SCRPAD_9                                           (Page_4 | 0x0049)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 10th byte
  #define BIT_MSK__MHL_SCRPAD_9__REG_MHL_SCRPAD_9                                      0xFF

// CBUS Scratch Pad 11th Byte Register
#define REG_ADDR__MHL_SCRPAD_A                                           (Page_4 | 0x004A)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 11th byte
  #define BIT_MSK__MHL_SCRPAD_A__REG_MHL_SCRPAD_A                                      0xFF

// CBUS Scratch Pad 12th Byte Register
#define REG_ADDR__MHL_SCRPAD_B                                           (Page_4 | 0x004B)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 12th byte
  #define BIT_MSK__MHL_SCRPAD_B__REG_MHL_SCRPAD_B                                      0xFF

// CBUS Scratch Pad 13th Byte Register
#define REG_ADDR__MHL_SCRPAD_C                                           (Page_4 | 0x004C)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 13th byte
  #define BIT_MSK__MHL_SCRPAD_C__REG_MHL_SCRPAD_C                                      0xFF

// CBUS Scratch Pad 14th Byte Register
#define REG_ADDR__MHL_SCRPAD_D                                           (Page_4 | 0x004D)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 14th byte
  #define BIT_MSK__MHL_SCRPAD_D__REG_MHL_SCRPAD_D                                      0xFF

// CBUS Scratch Pad 15th Byte Register
#define REG_ADDR__MHL_SCRPAD_E                                           (Page_4 | 0x004E)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 15th byte
  #define BIT_MSK__MHL_SCRPAD_E__REG_MHL_SCRPAD_E                                      0xFF

// CBUS Scratch Pad 16th Byte Register
#define REG_ADDR__MHL_SCRPAD_F                                           (Page_4 | 0x004F)
  // (ReadOnly, Bits 7:0) 
  // CBUS scratch pad 16th byte
  #define BIT_MSK__MHL_SCRPAD_F__REG_MHL_SCRPAD_F                                      0xFF

// MHL Extended Capability 1st Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_0                                        (Page_4 | 0x0080)
  // (ReadWrite, Bits 7:0) 
  // eCBUS Frequencies 0: ECBUS_S_075 1: ECBUS_S_8BIT 2: ECBUS_S_12BIT 3: reserved 4: ECBUS_D_150 5: ECBUS_D_8BIT 7-6: reserved
  #define BIT_MSK__MHL_EXTDEVCAP_0__REG_MHL_EXTDEVCAP_0                                   0xFF

// MHL Extended Capability 2nd Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_1                                        (Page_4 | 0x0081)
  // (ReadWrite, Bits 7:0) 
  // TMDS Frequencies 0: TMDS_150 1: TMDS_300 2: TMDS_600 7-3: reserved
  #define BIT_MSK__MHL_EXTDEVCAP_1__REG_MHL_EXTDEVCAP_1                                   0xFF

// MHL Extended Capability 3rd Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_2                                        (Page_4 | 0x0082)
  // (ReadWrite, Bits 7:0) 
  // 0: DEV_HOST 1: DEV_DEVICE 2: DEV_CHARGER 3: HID_HOST 4: HID_DEVICE 7:5 reserved
  #define BIT_MSK__MHL_EXTDEVCAP_2__REG_MHL_EXTDEVCAP_2                                   0xFF

// MHL Extended Capability 4th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_3                                        (Page_4 | 0x0083)
  // (ReadWrite, Bits 7:0) 
  // 0: LD_PHONE 7:1 reserved
  #define BIT_MSK__MHL_EXTDEVCAP_3__REG_MHL_EXTDEVCAP_3                                   0xFF

// MHL Extended Capability 5th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_4                                        (Page_4 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // Extended Logical Device Map 0: LD_PHONE 7-1: reserved
  #define BIT_MSK__MHL_EXTDEVCAP_4__REG_MHL_EXTDEVCAP_4                                   0xFF

// MHL Extended Capability 6th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_5                                        (Page_4 | 0x0085)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 6th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_5__REG_MHL_EXTDEVCAP_5                                   0xFF

// MHL Extended Capability 7th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_6                                        (Page_4 | 0x0086)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 7th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_6__REG_MHL_EXTDEVCAP_6                                   0xFF

// MHL Extended Capability 8th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_7                                        (Page_4 | 0x0087)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 8th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_7__REG_MHL_EXTDEVCAP_7                                   0xFF

// MHL Extended Capability 9th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_8                                        (Page_4 | 0x0088)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 9th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_8__REG_MHL_EXTDEVCAP_8                                   0xFF

// MHL Extended Capability 10th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_9                                        (Page_4 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 10th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_9__REG_MHL_EXTDEVCAP_9                                   0xFF

// MHL Extended Capability 11th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_A                                        (Page_4 | 0x008A)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 11th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_A__REG_MHL_EXTDEVCAP_A                                   0xFF

// MHL Extended Capability 12th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_B                                        (Page_4 | 0x008B)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 12th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_B__REG_MHL_EXTDEVCAP_B                                   0xFF

// MHL Extended Capability 13th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_C                                        (Page_4 | 0x008C)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 13th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_C__REG_MHL_EXTDEVCAP_C                                   0xFF

// MHL Extended Capability 14th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_D                                        (Page_4 | 0x008D)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 14th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_D__REG_MHL_EXTDEVCAP_D                                   0xFF

// MHL Extended Capability 15th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_E                                        (Page_4 | 0x008E)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 15th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_E__REG_MHL_EXTDEVCAP_E                                   0xFF

// MHL Extended Capability 16th Byte Register
#define REG_ADDR__MHL_EXTDEVCAP_F                                        (Page_4 | 0x008F)
  // (ReadWrite, Bits 7:0) 
  // MHL extended capability registers 16th byte: RSVD. Default value: Tx: 0x00: Rx: 0x00: Dongle: 0x00
  #define BIT_MSK__MHL_EXTDEVCAP_F__REG_MHL_EXTDEVCAP_F                                   0xFF

// Device Extended Status 1st byte Register
#define REG_ADDR__MHL_EXTSTAT_0                                          (Page_4 | 0x0090)
  // (ReadOnly, Bits 7:0) 
  // Current eCBUS-S or eCBUS-D TDM slot mode. 1-0: SLOT_MODE 3-2: ECBUS_MODE 7-4: reserved
  #define BIT_MSK__MHL_EXTSTAT_0__REG_MHL_EXTSTAT_0                                     0xFF

// Device Extended Status 2nd byte Register
#define REG_ADDR__MHL_EXTSTAT_1                                          (Page_4 | 0x0091)
  // (ReadOnly, Bits 7:0) 
  // Sink indicates TMDS link status to Source. 1-0: LINK_STATUS
  #define BIT_MSK__MHL_EXTSTAT_1__REG_MHL_EXTSTAT_1                                     0xFF

// Device Extended Status 3rd byte Register
#define REG_ADDR__MHL_EXTSTAT_2                                          (Page_4 | 0x0092)
  // (ReadOnly, Bits 7:0) 
  // Source indicates TMDS bandwidth to Sink. 2-0: LINK_RATE 7-3: reserved
  #define BIT_MSK__MHL_EXTSTAT_2__REG_MHL_EXTSTAT_2                                     0xFF

// Device Extended Status 4th byte Register
#define REG_ADDR__MHL_EXTSTAT_3                                          (Page_4 | 0x0093)
  // (ReadOnly, Bits 7:0) 
  // Sink readiness for multiple-sink connection. 1-0: SINK_STATUS_0 3-2: SINK_STATUS_1 5-4: SINK_STATUS_2 7-6: SINK_STATUS_3
  #define BIT_MSK__MHL_EXTSTAT_3__REG_MHL_EXTSTAT_3                                     0xFF

// Device Extended Status 5th byte Register
#define REG_ADDR__MHL_EXTSTAT_4                                          (Page_4 | 0x0094)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_4__REG_MHL_EXTSTAT_4                                     0xFF

// Device Extended Status 6th byte Register
#define REG_ADDR__MHL_EXTSTAT_5                                          (Page_4 | 0x0095)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_5__REG_MHL_EXTSTAT_5                                     0xFF

// Device Extended Status 7th byte Register
#define REG_ADDR__MHL_EXTSTAT_6                                          (Page_4 | 0x0096)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_6__REG_MHL_EXTSTAT_6                                     0xFF

// Device Extended Status 8th byte Register
#define REG_ADDR__MHL_EXTSTAT_7                                          (Page_4 | 0x0097)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_7__REG_MHL_EXTSTAT_7                                     0xFF

// Device Extended Status 9th byte Register
#define REG_ADDR__MHL_EXTSTAT_8                                          (Page_4 | 0x0098)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_8__REG_MHL_EXTSTAT_8                                     0xFF

// Device Extended Status 10th byte Register
#define REG_ADDR__MHL_EXTSTAT_9                                          (Page_4 | 0x0099)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_9__REG_MHL_EXTSTAT_9                                     0xFF

// Device Extended Status 11th byte Register
#define REG_ADDR__MHL_EXTSTAT_A                                          (Page_4 | 0x009A)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_A__REG_MHL_EXTSTAT_A                                     0xFF

// Device Extended Status 12th byte Register
#define REG_ADDR__MHL_EXTSTAT_B                                          (Page_4 | 0x009B)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_B__REG_MHL_EXTSTAT_B                                     0xFF

// Device Extended Status 13th byte Register
#define REG_ADDR__MHL_EXTSTAT_C                                          (Page_4 | 0x009C)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_C__REG_MHL_EXTSTAT_C                                     0xFF

// Device Extended Status 14th byte Register
#define REG_ADDR__MHL_EXTSTAT_D                                          (Page_4 | 0x009D)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_D__REG_MHL_EXTSTAT_D                                     0xFF

// Device Extended Status 15th byte Register
#define REG_ADDR__MHL_EXTSTAT_E                                          (Page_4 | 0x009E)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_E__REG_MHL_EXTSTAT_E                                     0xFF

// Device Extended Status 16th byte Register
#define REG_ADDR__MHL_EXTSTAT_F                                          (Page_4 | 0x009F)
  // (ReadOnly, Bits 7:0) 
  // rsvd
  #define BIT_MSK__MHL_EXTSTAT_F__REG_MHL_EXTSTAT_F                                     0xFF

// Device Extended Status 17th byte Register
#define REG_ADDR__MHL_EXTSTAT_10                                         (Page_4 | 0x00A0)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 17th byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_10__REG_MHL_EXTSTAT_10                                    0xFF

// Device Extended Status 18th byte Register
#define REG_ADDR__MHL_EXTSTAT_11                                         (Page_4 | 0x00A1)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 18th byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_11__REG_MHL_EXTSTAT_11                                    0xFF

// Device Extended Status 19th byte Register
#define REG_ADDR__MHL_EXTSTAT_12                                         (Page_4 | 0x00A2)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 19th byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_12__REG_MHL_EXTSTAT_12                                    0xFF

// Device Extended Status 20th byte Register
#define REG_ADDR__MHL_EXTSTAT_13                                         (Page_4 | 0x00A3)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 20th byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_13__REG_MHL_EXTSTAT_13                                    0xFF

// Device Extended Status 21th byte Register
#define REG_ADDR__MHL_EXTSTAT_14                                         (Page_4 | 0x00A4)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 21st byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_14__REG_MHL_EXTSTAT_14                                    0xFF

// Device Extended Status 22th byte Register
#define REG_ADDR__MHL_EXTSTAT_15                                         (Page_4 | 0x00A5)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 22nd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_15__REG_MHL_EXTSTAT_15                                    0xFF

// Device Extended Status 23th byte Register
#define REG_ADDR__MHL_EXTSTAT_16                                         (Page_4 | 0x00A6)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 23rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_16__REG_MHL_EXTSTAT_16                                    0xFF

// Device Extended Status 24th byte Register
#define REG_ADDR__MHL_EXTSTAT_17                                         (Page_4 | 0x00A7)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 24th byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_17__REG_MHL_EXTSTAT_17                                    0xFF

// Device Extended Status 25th byte Register
#define REG_ADDR__MHL_EXTSTAT_18                                         (Page_4 | 0x00A8)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 25rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_18__REG_MHL_EXTSTAT_18                                    0xFF

// Device Extended Status 26th byte Register
#define REG_ADDR__MHL_EXTSTAT_19                                         (Page_4 | 0x00A9)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 26rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_19__REG_MHL_EXTSTAT_19                                    0xFF

// Device Extended Status 27th byte Register
#define REG_ADDR__MHL_EXTSTAT_1A                                         (Page_4 | 0x00AA)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 27rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1A__REG_MHL_EXTSTAT_1A                                    0xFF

// Device Extended Status 28th byte Register
#define REG_ADDR__MHL_EXTSTAT_1B                                         (Page_4 | 0x00AB)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 28rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1B__REG_MHL_EXTSTAT_1B                                    0xFF

// Device Extended Status 29th byte Register
#define REG_ADDR__MHL_EXTSTAT_1C                                         (Page_4 | 0x00AC)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 29rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1C__REG_MHL_EXTSTAT_1C                                    0xFF

// Device Extended Status 30th byte Register
#define REG_ADDR__MHL_EXTSTAT_1D                                         (Page_4 | 0x00AD)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 30rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1D__REG_MHL_EXTSTAT_1D                                    0xFF

// Device Extended Status 31th byte Register
#define REG_ADDR__MHL_EXTSTAT_1E                                         (Page_4 | 0x00AE)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 31rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1E__REG_MHL_EXTSTAT_1E                                    0xFF

// Device Extended Status 32th byte Register
#define REG_ADDR__MHL_EXTSTAT_1F                                         (Page_4 | 0x00AF)
  // (ReadOnly, Bits 7:0) 
  // MHL status registers 32rd byte. RSVD
  #define BIT_MSK__MHL_EXTSTAT_1F__REG_MHL_EXTSTAT_1F                                    0xFF

//***************************************************************************
// page 35. Address: 40
// CBUS_MSC_AGENT Interrupt Status Register
#define REG_ADDR__CBUS_MSC_AGENT_INTR                                    (Page_5 | 0x0000)
  // (ReadWrite, Bits 0) 
  // DeviceCap registers are available in DEVCAP FIFO
  #define BIT_MSK__CBUS_MSC_AGENT_INTR__REG_CBUS_MSC_AGENT_INTR_STAT0                         0x01

// CBUS_MSC_AGENT Interrupt Mask Register
#define REG_ADDR__CBUS_MSC_AGENT_INTR_MASK                               (Page_5 | 0x0001)
  // (ReadWrite, Bits 0) 
  // Enable INT[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_MSC_AGENT_INTR_MASK__REG_CBUS_MSC_AGENT_INTR_MASK0                         0x01

// CBUS_MSC_AGENT Aggregated Interrupt Status Register
#define REG_ADDR__CBUS_MSC_AGENT_INTR_STAT                               (Page_5 | 0x0002)
  // (ReadOnly, Bits 0) 
  // CBUS_MSC_AGENT aggregated interrupt at 0x00
  #define BIT_MSK__CBUS_MSC_AGENT_INTR_STAT__REG_CBUS_MSC_AGENT_INTR_STAT                          0x01

// CBUS_MSC_AGENT Start Register
#define REG_ADDR__CBUS_MSC_START                                         (Page_5 | 0x0003)
  // (ReadWrite, Bits 0) 
  // Start to get DEVCAP. Self-resetting to 0.
  #define BIT_MSK__CBUS_MSC_START__REG_GET_DEVCAP_START                                  0x01

// CBUS_MSC_AGENT Control Register
#define REG_ADDR__CBUS_MSC_AGENT_CTRL                                    (Page_5 | 0x0004)
  // (ReadWrite, Bits 0) 
  // 0: read DevCap through CBUS from Rx (could be MHL1/2/3 device) 1: read extended DevCap through CBUS from Rx (most likely an MHL3 device) 
  #define BIT_MSK__CBUS_MSC_AGENT_CTRL__REG_XDEVCAP_EN                                        0x01
  // (ReadWrite, Bits 1) 
  // Self-clear. Write 1'b1 to move DEVCAP FIFO read pointer back to 4'h0
  #define BIT_MSK__CBUS_MSC_AGENT_CTRL__REG_DEVCAP_RADDR_CLR                                  0x02

// CBUS_MSC_AGENT Status Register
#define REG_ADDR__CBUS_MSC_AGENT_STAT                                    (Page_5 | 0x0005)
  // (ReadOnly, Bits 0) 
  // Device Capability in FIFO is valid
  #define BIT_MSK__CBUS_MSC_AGENT_STAT__MSC_DEVCAP_VALID                                      0x01
  // (ReadOnly, Bits 1) 
  // Indicates that CBUS is busy after TPI_CBUS_START.* is requested.
  #define BIT_MSK__CBUS_MSC_AGENT_STAT__MSC_STATUS_BUSY                                       0x02

// DEVCAP Read Port
#define REG_ADDR__CBUS_DEVCAP_FIFO                                       (Page_5 | 0x0006)
  // (ReadWrite, Bits 7:0) 
  // Read this port to get peer device capability register after 0x00[0] is asserted
  #define BIT_MSK__CBUS_DEVCAP_FIFO__REG_DEVCAP_RDATA                                      0xFF

// CBUS WRITE_BURST Transmit 1st Byte Register
#define REG_ADDR__WB_XMIT_DATA_0                                         (Page_5 | 0x0060)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 1st byte
  #define BIT_MSK__WB_XMIT_DATA_0__REG_WB_XMIT_DATA_0                                    0xFF

// CBUS WRITE_BURST Transmit 2nd Byte Register
#define REG_ADDR__WB_XMIT_DATA_1                                         (Page_5 | 0x0061)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 2nd byte
  #define BIT_MSK__WB_XMIT_DATA_1__REG_WB_XMIT_DATA_1                                    0xFF

// CBUS WRITE_BURST Transmit 3rd Byte Register
#define REG_ADDR__WB_XMIT_DATA_2                                         (Page_5 | 0x0062)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 3rd byte
  #define BIT_MSK__WB_XMIT_DATA_2__REG_WB_XMIT_DATA_2                                    0xFF

// CBUS WRITE_BURST Transmit 4th Byte Register
#define REG_ADDR__WB_XMIT_DATA_3                                         (Page_5 | 0x0063)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 4th byte
  #define BIT_MSK__WB_XMIT_DATA_3__REG_WB_XMIT_DATA_3                                    0xFF

// CBUS WRITE_BURST Transmit 5th Byte Register
#define REG_ADDR__WB_XMIT_DATA_4                                         (Page_5 | 0x0064)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 5th byte
  #define BIT_MSK__WB_XMIT_DATA_4__REG_WB_XMIT_DATA_4                                    0xFF

// CBUS WRITE_BURST Transmit 6th Byte Register
#define REG_ADDR__WB_XMIT_DATA_5                                         (Page_5 | 0x0065)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 6th byte
  #define BIT_MSK__WB_XMIT_DATA_5__REG_WB_XMIT_DATA_5                                    0xFF

// CBUS WRITE_BURST Transmit 7th Byte Register
#define REG_ADDR__WB_XMIT_DATA_6                                         (Page_5 | 0x0066)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 7th byte
  #define BIT_MSK__WB_XMIT_DATA_6__REG_WB_XMIT_DATA_6                                    0xFF

// CBUS WRITE_BURST Transmit 8th Byte Register
#define REG_ADDR__WB_XMIT_DATA_7                                         (Page_5 | 0x0067)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 8th byte
  #define BIT_MSK__WB_XMIT_DATA_7__REG_WB_XMIT_DATA_7                                    0xFF

// CBUS WRITE_BURST Transmit 9th Byte Register
#define REG_ADDR__WB_XMIT_DATA_8                                         (Page_5 | 0x0068)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 9th byte
  #define BIT_MSK__WB_XMIT_DATA_8__REG_WB_XMIT_DATA_8                                    0xFF

// CBUS WRITE_BURST Transmit 10th Byte Register
#define REG_ADDR__WB_XMIT_DATA_9                                         (Page_5 | 0x0069)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 10th byte
  #define BIT_MSK__WB_XMIT_DATA_9__REG_WB_XMIT_DATA_9                                    0xFF

// CBUS WRITE_BURST Transmit 11th Byte Register
#define REG_ADDR__WB_XMIT_DATA_A                                         (Page_5 | 0x006A)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 11th byte
  #define BIT_MSK__WB_XMIT_DATA_A__REG_WB_XMIT_DATA_A                                    0xFF

// CBUS WRITE_BURST Transmit 12th Byte Register
#define REG_ADDR__WB_XMIT_DATA_B                                         (Page_5 | 0x006B)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 12th byte
  #define BIT_MSK__WB_XMIT_DATA_B__REG_WB_XMIT_DATA_B                                    0xFF

// CBUS WRITE_BURST Transmit 13th Byte Register
#define REG_ADDR__WB_XMIT_DATA_C                                         (Page_5 | 0x006C)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 13th byte
  #define BIT_MSK__WB_XMIT_DATA_C__REG_WB_XMIT_DATA_C                                    0xFF

// CBUS WRITE_BURST Transmit 14th Byte Register
#define REG_ADDR__WB_XMIT_DATA_D                                         (Page_5 | 0x006D)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 14th byte
  #define BIT_MSK__WB_XMIT_DATA_D__REG_WB_XMIT_DATA_D                                    0xFF

// CBUS WRITE_BURST Transmit 15th Byte Register
#define REG_ADDR__WB_XMIT_DATA_E                                         (Page_5 | 0x006E)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 15th byte
  #define BIT_MSK__WB_XMIT_DATA_E__REG_WB_XMIT_DATA_E                                    0xFF

// CBUS WRITE_BURST Transmit 16th Byte Register
#define REG_ADDR__WB_XMIT_DATA_F                                         (Page_5 | 0x006F)
  // (ReadWrite, Bits 7:0) 
  // WRITE_BURST transmit 16th byte
  #define BIT_MSK__WB_XMIT_DATA_F__REG_WB_XMIT_DATA_F                                    0xFF

// Debug Control Low Register
#define REG_ADDR__DBG_CTL_L                                              (Page_5 | 0x0070)
  // (ReadWrite, Bits 7:0) 
  // Debug control register; 1st byte
  #define BIT_MSK__DBG_CTL_L__REG_DBG_CTL_B7_B0                                     0xFF

// Debug Control High Register
#define REG_ADDR__DBG_CTL_H                                              (Page_5 | 0x0071)
  // (ReadWrite, Bits 7:0) 
  // Debug control register; 2nd byte
  #define BIT_MSK__DBG_CTL_H__REG_DBG_CTL_B15_B8                                    0xFF

// Debug External Status Register
#define REG_ADDR__DBG_EXT_STAT                                           (Page_5 | 0x0072)
  // (ReadOnly, Bits 7:0) 
  // External status for debugging purpose
  #define BIT_MSK__DBG_EXT_STAT__REG_DBG_EXT_STAT                                      0xFF

// Debug Internal Status Select Register
#define REG_ADDR__DBG_INT_STAT_SEL                                       (Page_5 | 0x0073)
  // (ReadWrite, Bits 1:0) 
  // Debug internal status select
  #define BIT_MSK__DBG_INT_STAT_SEL__REG_DBG_INT_STAT_SEL                                  0x03

// Debug Internal Status Low Register
#define REG_ADDR__DBG_INT_STAT_L                                         (Page_5 | 0x0074)
  // (ReadOnly, Bits 7:0) 
  // Internal status for debugging purpose; low byte
  #define BIT_MSK__DBG_INT_STAT_L__REG_DBG_INT_STAT_B7_B0                                0xFF

// Debug Internal Status High Register
#define REG_ADDR__DBG_INT_STAT_H                                         (Page_5 | 0x0075)
  // (ReadOnly, Bits 7:0) 
  // Internal status for debugging purpose; high byte
  #define BIT_MSK__DBG_INT_STAT_H__REG_DBG_INT_STAT_B15_B8                               0xFF

// CBUS System Control Register
#define REG_ADDR__CBUS_SYS_CTL                                           (Page_5 | 0x0078)
  // (ReadWrite, Bits 0) 
  // Set to 1'b1 to reset CBUS
  #define BIT_MSK__CBUS_SYS_CTL__REG_CBUS_RST                                          0x01

// CBUS System Aggregated Interrupt Status #1 Register
#define REG_ADDR__CBUS_AGGREGATED_INT_STAT_0                             (Page_5 | 0x0079)
  // (ReadOnly, Bits 0) 
  // Aggregate interrupt status for 0x92
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B0                                 0x01
  // (ReadOnly, Bits 1) 
  // Aggregate interrupt status for 0x94
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B1                                 0x02
  // (ReadOnly, Bits 2) 
  // Aggregate interrupt status for 0x8A
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B2                                 0x04
  // (ReadOnly, Bits 3) 
  // Aggregate interrupt status for 0x8E
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B3                                 0x08
  // (ReadOnly, Bits 4) 
  // Aggregate interrupt status for MHLSPEC page 0x20
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B4                                 0x10
  // (ReadOnly, Bits 5) 
  // Aggregate interrupt status for MHLSPEC page 0x21
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B5                                 0x20
  // (ReadOnly, Bits 6) 
  // Aggregate interrupt status for MHLSPEC page 0x22
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B6                                 0x40
  // (ReadOnly, Bits 7) 
  // Aggregate interrupt status for MHLSPEC page 0x23
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_0__REG_CBUS_INTR_STAT_B7                                 0x80

// CBUS System Aggregated Interrupt Status #2 Register
#define REG_ADDR__CBUS_AGGREGATED_INT_STAT_1                             (Page_5 | 0x007A)
  // (ReadOnly, Bits 0) 
  // Aggregate interrupt status for 0x9A
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B8                                 0x01
  // (ReadOnly, Bits 1) 
  // Aggregate interrupt status for 0x9C
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B9                                 0x02
  // (ReadOnly, Bits 2) 
  // Aggregate interrupt status for 0x98
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B10                                0x04
  // (ReadOnly, Bits 3) 
  // Aggregate interrupt status for 0xDD
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B11                                0x08
  // (ReadOnly, Bits 4) 
  // Aggregate interrupt status for CBUS discovery page (0x10[0])
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B12                                0x10
  // (ReadOnly, Bits 5) 
  // Aggregate interrupt status for  CBUS MSC AGENT page (0x02[0])
  #define BIT_MSK__CBUS_AGGREGATED_INT_STAT_1__REG_CBUS_INTR_STAT_B13                                0x20

// CBUS Discovery Clock Ratio Register
#define REG_ADDR__CBUS_DISC_CLK_RATIO_L                                  (Page_5 | 0x007E)
  // (ReadWrite, Bits 7:0) 
  // Tx Only: The ratio between cbus discovery clock frequency to 60KHz clock (bits [7:0]) 
  #define BIT_MSK__CBUS_DISC_CLK_RATIO_L__REG_RATIO_60KHZ_2_DISC_B7_B0                          0xFF

// CBUS System Control Register
#define REG_ADDR__CBUS_DISC_CLK_RATIO_H                                  (Page_5 | 0x007F)
  // (ReadWrite, Bits 1:0) 
  // Tx Only: The ratio between cbus discovery clock frequency to 60KHz clock (bist [9:8]) 
  #define BIT_MSK__CBUS_DISC_CLK_RATIO_H__REG_RATIO_60KHZ_2_DISC_B9_B8                          0x03
  // (ReadWrite, Bits 7) 
  // 1'b1: CBUS discovery clock is 60KHz 1'b0: The ratio between CBUS discovery clock and 60KHz clock 
  #define BIT_MSK__CBUS_DISC_CLK_RATIO_H__REG_SEL_60KHZ_4_DISC                                  0x80

// Interrupt Mask 1st Register
#define REG_ADDR__MHL_INT_0_MASK                                         (Page_5 | 0x0080)
  // (ReadWrite, Bits 0) 
  // Enable DCAP_CHG interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Eanble DSCR_CHG interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable REQ_WRT interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable GRT_WRT interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK3                                   0x08
  // (ReadWrite, Bits 4) 
  // Enable MHL_INT_0[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK4                                   0x10
  // (ReadWrite, Bits 5) 
  // Enable MHL_INT_0[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK5                                   0x20
  // (ReadWrite, Bits 6) 
  // Enable MHL_INT_0[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL_INT_0[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_0_MASK__REG_MHL_INT_0_MASK7                                   0x80

// Interrupt Mask 2nd Register
#define REG_ADDR__MHL_INT_1_MASK                                         (Page_5 | 0x0081)
  // (ReadWrite, Bits 0) 
  // Enable MHL_INT_1[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable EDID_CHG interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable MHL_INT_1[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable MHL_INT_1[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK3                                   0x08
  // (ReadWrite, Bits 4) 
  // Enable MHL_INT_1[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK4                                   0x10
  // (ReadWrite, Bits 5) 
  // Enable MHL_INT_1[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK5                                   0x20
  // (ReadWrite, Bits 6) 
  // Enable MHL_INT_1[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL_INT_1[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_1_MASK__REG_MHL_INT_1_MASK7                                   0x80

// Interrupt Mask 3rd Register
#define REG_ADDR__MHL_INT_2_MASK                                         (Page_5 | 0x0082)
  // (ReadWrite, Bits 0) 
  // Enable MHL_INT_2[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable MHL_INT_2[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable MHL_INT_2[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable MHL_INT_2[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK3                                   0x08
  // (ReadWrite, Bits 4) 
  // Enable MHL_INT_2[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK4                                   0x10
  // (ReadWrite, Bits 5) 
  // Enable MHL_INT_2[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK5                                   0x20
  // (ReadWrite, Bits 6) 
  // Enable MHL_INT_2[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL_INT_2[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_2_MASK__REG_MHL_INT_2_MASK7                                   0x80

// Interrupt Mask 4th Register
#define REG_ADDR__MHL_INT_3_MASK                                         (Page_5 | 0x0083)
  // (ReadWrite, Bits 0) 
  // Enable MHL_INT_3[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable MHL_INT_3[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable MHL_INT_3[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable MHL_INT_3[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK3                                   0x08
  // (ReadWrite, Bits 4) 
  // Enable MHL_INT_3[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK4                                   0x10
  // (ReadWrite, Bits 5) 
  // Enable MHL_INT_3[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK5                                   0x20
  // (ReadWrite, Bits 6) 
  // Enable MHL_INT_3[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL_INT_3[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MHL_INT_3_MASK__REG_MHL_INT_3_MASK7                                   0x80

// MDT Reveive Time Out Register
#define REG_ADDR__MDT_RCV_TIMEOUT                                        (Page_5 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // MDT receive time out threshold MSB (increment of 1048576 clocks)
  #define BIT_MSK__MDT_RCV_TIMEOUT__REG_MDT_RCV_TIMEOUT_MAX_MSB                           0xFF

// MDT Transmit Time Out Register
#define REG_ADDR__MDT_XMIT_TIMEOUT                                       (Page_5 | 0x0085)
  // (ReadWrite, Bits 7:0) 
  // MDT transmit time out threshold MSB (increment of 1048576 clocks)
  #define BIT_MSK__MDT_XMIT_TIMEOUT__REG_MDT_XMIT_TIMEOUT_MAX_MSB                          0xFF

// MDT Receive Control Register
#define REG_ADDR__MDT_RCV_CONTROL                                        (Page_5 | 0x0086)
  // (ReadWrite, Bits 0) 
  // Increments receive fifo read-pointer to next level.  0 = No 1 = yes (self-resetting bit)
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_RFIFO_CLR_CUR                                 0x01
  // (ReadWrite, Bits 1) 
  // Resets receive fifo read-pointer to zero. 0 = No 1 = yes (self-resetting bit)
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_RFIFO_CLR_ALL                                 0x02
  // (ReadWrite, Bits 2) 
  // Disable MDT transmission and receiving - reset both transmission and receiving state machine to idle
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_DISABLE                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable MDT transmission FIFO over write when it is full
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_XFIFO_OVER_WR_EN                              0x08
  // (ReadWrite, Bits 4) 
  // Enable MDT receiving FIFO over write when it is full
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_RFIFO_OVER_WR_EN                              0x10
  // (ReadWrite, Bits 6) 
  // When this bit is set and reg_mdt_rcv_en is set; internal MDT receive enable will be delayed to set until a following MSC transaction has been triggered and finished. This will gurantee that atomic behavior of enabling MDT receiving.
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_DELAY_RCV_EN                                  0x40
  // (ReadWrite, Bits 7) 
  // Enable MDT receive: 1 - Enable 0 - Disable
  #define BIT_MSK__MDT_RCV_CONTROL__REG_MDT_RCV_EN                                        0x80

// MDT Receive Read Port
#define REG_ADDR__MDT_RCV_READ_PORT                                      (Page_5 | 0x0087)
  // (ReadOnly, Bits 7:0) 
  // MDT receive read data port. Use I2C burst read to read out received WRITE_BURST data.
  #define BIT_MSK__MDT_RCV_READ_PORT__MDT_RFIFO_DATA                                        0xFF

// MDT Transmit Control Register
#define REG_ADDR__MDT_XMIT_CONTROL                                       (Page_5 | 0x0088)
  // (ReadWrite, Bits 0) 
  // Increments transmit fifo read-pointer to next level. 0 = No 1 = Yes (self-resetting bit)
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XFIFO_CLR_CUR                                 0x01
  // (ReadWrite, Bits 1) 
  // Resets transmit fifo read-pointer to zero. 0 = No 1 = Yes (self-resetting bit)
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XFIFO_CLR_ALL                                 0x02
  // (ReadWrite, Bits 2) 
  // Clear wait state due to received ABORT packet. This applies both to MDT receive and transmit 0 = No 1 = Yes (self-resetting bit)
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_CLR_ABORT_WAIT                                0x04
  // (ReadWrite, Bits 3) 
  // Onec enabled: MDT transmit will only do a single WRITE_BURST. For the second single run: it needs to be cleared and set by FW again. 0 = No 1 = Yes (Not self-resetting)
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XMIT_SINGLE_RUN_EN                            0x08
  // (ReadWrite, Bits 4) 
  // MDT transmit use the fixed Adopter ID
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XMIT_FIXED_AID                                0x10
  // (ReadWrite, Bits 5) 
  // MDT transmit use the fixed burst length
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XMIT_FIXED_BURST_LEN                          0x20
  // (ReadWrite, Bits 6) 
  // MDT transmit enable merging DSCR_CHG and REQ_WRT in one SET_INT command if possible
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XMIT_CMD_MERGE_EN                             0x40
  // (ReadWrite, Bits 7) 
  // Enable MDT transmit: 1 - Enable 0 - Disable
  #define BIT_MSK__MDT_XMIT_CONTROL__REG_MDT_XMIT_EN                                       0x80

// MDT Receive WRITE Port
#define REG_ADDR__MDT_XMIT_WRITE_PORT                                    (Page_5 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // MDT transmit write data port. Use I2C burst write to write WRITE_BURST data to be transmitted.
  #define BIT_MSK__MDT_XMIT_WRITE_PORT__MDT_XFIFO_WDATA                                       0xFF

// MDT RFIFO Status Register
#define REG_ADDR__MDT_RFIFO_STAT                                         (Page_5 | 0x008A)
  // (ReadOnly, Bits 4:0) 
  // MDT RFIFO received data byte count for the current level
  #define BIT_MSK__MDT_RFIFO_STAT__REG_MDT_RFIFO_CUR_BYTE_CNT                            0x1F
  // (ReadOnly, Bits 7:5) 
  // MDT RFIFO available data level
  #define BIT_MSK__MDT_RFIFO_STAT__REG_MDT_RFIFO_CNT                                     0xE0

// MDT XFIFO Status Register
#define REG_ADDR__MDT_XFIFO_STAT                                         (Page_5 | 0x008B)
  // (ReadOnly, Bits 3:0) 
  // MDT XFIFO data byte count for next WRITE_BURST if no fixed WRITE_BURST length is selected.
  #define BIT_MSK__MDT_XFIFO_STAT__REG_MDT_WRITE_BURST_LEN                               0x0F
  // (ReadWrite, Bits 4) 
  // MDT Transmit preemptive handshaking enable 1: Enable 0: Disable
  #define BIT_MSK__MDT_XFIFO_STAT__REG_MDT_XMIT_PRE_HS_EN                                0x10
  // (ReadOnly, Bits 7:5) 
  // MDT XFIFO empty/available data level 
  #define BIT_MSK__MDT_XFIFO_STAT__REG_MDT_XFIFO_LEVEL_AVAIL                             0xE0

// MDT Interrupt 0 Register
#define REG_ADDR__MDT_INT_0                                              (Page_5 | 0x008C)
  // (ReadWrite, Bits 0) 
  // MDT RFIFO data ready status
  #define BIT_MSK__MDT_INT_0__REG_MDT_INT_0_0                                       0x01
  // (ReadWrite, Bits 1) 
  // MDT XFIFO full status
  #define BIT_MSK__MDT_INT_0__REG_MDT_INT_0_1                                       0x02
  // (ReadWrite, Bits 2) 
  // MDT state machine comes to IDLE after reg_mdt_xmit_en  AND / OR  reg_mdt_rcv_en are disabled. This informs FW to take over.
  #define BIT_MSK__MDT_INT_0__REG_MDT_INT_0_2                                       0x04
  // (ReadWrite, Bits 3) 
  // MDT XFIFO empty stauts
  #define BIT_MSK__MDT_INT_0__REG_MDT_INT_0_3                                       0x08

// MDT Interrupt 0 Mask Register
#define REG_ADDR__MDT_INT_0_MASK                                         (Page_5 | 0x008D)
  // (ReadWrite, Bits 0) 
  // Enable MDT RFIFO data ready interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_0_MASK__REG_MDT_INT_0_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable MDT XFIFO full interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_0_MASK__REG_MDT_INT_0_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable MDT state machine IDLE interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_0_MASK__REG_MDT_INT_0_MASK2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable MDT XFIFO empty interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_0_MASK__REG_MDT_INT_0_MASK3                                   0x08

// MDT Interrupt 1 Register
#define REG_ADDR__MDT_INT_1                                              (Page_5 | 0x008E)
  // (ReadWrite, Bits 0) 
  // MDT Receive time out status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_0                                       0x01
  // (ReadWrite, Bits 1) 
  // MDT Receive state machine received abort packet status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_1                                       0x02
  // (ReadWrite, Bits 2) 
  // MDT Receive state machine error status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_2                                       0x04
  // (ReadWrite, Bits 5) 
  // MDT Transmit time out status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_5                                       0x20
  // (ReadWrite, Bits 6) 
  // MDT Transmit state machine received abort packet status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_6                                       0x40
  // (ReadWrite, Bits 7) 
  // MDT Transmit state machine error status
  #define BIT_MSK__MDT_INT_1__REG_MDT_INT_1_7                                       0x80

// MDT Interrupt 1 Mask Register
#define REG_ADDR__MDT_INT_1_MASK                                         (Page_5 | 0x008F)
  // (ReadWrite, Bits 0) 
  // Enable MDT Receive time out interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable MDT Receive state machine received abort packet status interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable MDT Receive state machine error interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK2                                   0x04
  // (ReadWrite, Bits 5) 
  // Enable MDT Transmit time out interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK5                                   0x20
  // (ReadWrite, Bits 6) 
  // Enable MDT Transmit state machine received abort packet status interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable MDT Transmit state machine error interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MDT_INT_1_MASK__REG_MDT_INT_1_MASK7                                   0x80

// CBUS Vendor ID Register
#define REG_ADDR__CBUS_VENDOR_ID                                         (Page_5 | 0x0090)
  // (ReadOnly, Bits 7:0) 
  // CBUS Vendor ID. This is the value returned if a GET_VENDOR_ID is received. Note that this is different from adopter ID.
  #define BIT_MSK__CBUS_VENDOR_ID__REG_CBUS_VENDOR_ID                                    0xFF

// CBUS Connection Status Register
#define REG_ADDR__CBUS_STATUS                                            (Page_5 | 0x0091)
  // (ReadOnly, Bits 0) 
  // Rx Only: CBUS connected. N/A for Tx
  #define BIT_MSK__CBUS_STATUS__REG_CBUS_CONNECTED                                    0x01
  // (ReadOnly, Bits 1) 
  // CBUS is ready for commands.
  #define BIT_MSK__CBUS_STATUS__REG_MHL_MODE                                          0x02
  // (ReadOnly, Bits 2) 
  // SET_HPD is received by Tx. N/A for Rx
  #define BIT_MSK__CBUS_STATUS__REG_CBUS_HPD                                          0x04
  // (ReadOnly, Bits 3) 
  // MSC heartbeat success indicator
  #define BIT_MSK__CBUS_STATUS__REG_MSC_HB_SUCCESS                                    0x08
  // (ReadOnly, Bits 4) 
  // MHL cable detect sense
  #define BIT_MSK__CBUS_STATUS__REG_MHL_CABLE_PRESENT                                 0x10

// CBUS Interrupt 1st Register
#define REG_ADDR__CBUS_INT_0                                             (Page_5 | 0x0092)
  // (ReadWrite, Bits 0) 
  // CBUS connection change interrupt status
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT0                                  0x01
  // (ReadWrite, Bits 1) 
  // MSC command done interrupt status
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT1                                  0x02
  // (ReadWrite, Bits 2) 
  // CBUS HPD change status (TX Only)
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT2                                  0x04
  // (ReadWrite, Bits 3) 
  // MSC WRITE_STAT command received interrupt status
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT3                                  0x08
  // (ReadWrite, Bits 4) 
  // MSC_MSG command received interrupt status
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT4                                  0x10
  // (ReadWrite, Bits 5) 
  // MSC WRITE_BURST command received interrupt status
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT5                                  0x20
  // (ReadWrite, Bits 6) 
  // MSC SET_INT command received interrupt status. Note that this is not masked by MHL_INT_*_MASK but is masked by CBUS_INT_0_MASK.B769
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT6                                  0x40
  // (ReadWrite, Bits 7) 
  // MSC_MSG  command done with NACK interrupt status.
  #define BIT_MSK__CBUS_INT_0__REG_CBUS_INT_0_STAT7                                  0x80

// CBUS Interrupt Mask 1st Register
#define REG_ADDR__CBUS_INT_0_MASK                                        (Page_5 | 0x0093)
  // (ReadWrite, Bits 0) 
  // Enable CBUS connection change interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK0                                  0x01
  // (ReadWrite, Bits 1) 
  // Enable MSC command done interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK1                                  0x02
  // (ReadWrite, Bits 2) 
  // Enable CBUS HPD change interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK2                                  0x04
  // (ReadWrite, Bits 3) 
  // Enable MSC WRITE_STAT  received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK3                                  0x08
  // (ReadWrite, Bits 4) 
  // Enable MSC_MSG received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK4                                  0x10
  // (ReadWrite, Bits 5) 
  // Enable MSC WRITE_BURST received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK5                                  0x20
  // (ReadWrite, Bits 6) 
  // Enable MSC SET_INT command received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK6                                  0x40
  // (ReadWrite, Bits 7) 
  // Enable MSC_MSG command done with NACK interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_0_MASK__REG_CBUS_INT_0_MASK7                                  0x80

// CBUS Interrupt 2nd Register
#define REG_ADDR__CBUS_INT_1                                             (Page_5 | 0x0094)
  // (ReadWrite, Bits 0) 
  // MSC heartbeat max fail interrupt staus
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT0                                  0x01
  // (ReadWrite, Bits 2) 
  // DDC abort interrupt status
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT2                                  0x04
  // (ReadWrite, Bits 3) 
  // MSC received command error interrupt status (As Responder). For exact error; check 0x9C
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT3                                  0x08
  // (ReadWrite, Bits 5) 
  // CBUS packet received interrupt status
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT5                                  0x20
  // (ReadWrite, Bits 6) 
  // MSC command error interrupt status (As Requester). For exact error; check 0x9A
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT6                                  0x40
  // (ReadWrite, Bits 7) 
  // MHL Cable connect interrupt status
  #define BIT_MSK__CBUS_INT_1__REG_CBUS_INT_1_STAT7                                  0x80

// CBUS Interrupt Mask 2nd Register
#define REG_ADDR__CBUS_INT_1_MASK                                        (Page_5 | 0x0095)
  // (ReadWrite, Bits 0) 
  // Enable MSC heartbeat max fail interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK0                                  0x01
  // (ReadWrite, Bits 2) 
  // Enable DDC abort interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK2                                  0x04
  // (ReadWrite, Bits 3) 
  // Enable MSC abort received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK3                                  0x08
  // (ReadWrite, Bits 5) 
  // Enable CBUS packet received intrrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK5                                  0x20
  // (ReadWrite, Bits 6) 
  // Enable MSC command abort interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK6                                  0x40
  // (ReadWrite, Bits 7) 
  // Enable MHL cable connect interrupt status 1 - enable 0 - disable (default)
  #define BIT_MSK__CBUS_INT_1_MASK__REG_CBUS_INT_1_MASK7                                  0x80

// CBUS DDC Abort Interrupt Register
#define REG_ADDR__DDC_ABORT_INT                                          (Page_5 | 0x0098)
  // (ReadWrite, Bits 0) 
  // DDC max fail interrupt status
  #define BIT_MSK__DDC_ABORT_INT__REG_DDC_ABORT_INT_STAT0                               0x01
  // (ReadWrite, Bits 1) 
  // DDC protocol error interrupt status
  #define BIT_MSK__DDC_ABORT_INT__REG_DDC_ABORT_INT_STAT1                               0x02
  // (ReadWrite, Bits 2) 
  // DDC time out interrupt status
  #define BIT_MSK__DDC_ABORT_INT__REG_DDC_ABORT_INT_STAT2                               0x04
  // (ReadWrite, Bits 7) 
  // DDC aborted by peer interrupt status
  #define BIT_MSK__DDC_ABORT_INT__REG_DDC_ABORT_INT_STAT7                               0x80

// CBUS DDC Abort Interrupt Mask Register
#define REG_ADDR__DDC_ABORT_INT_MASK                                     (Page_5 | 0x0099)
  // (ReadWrite, Bits 0) 
  // Enable DDC max fail interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__DDC_ABORT_INT_MASK__REG_DDC_ABORT_INT_MASK0                               0x01
  // (ReadWrite, Bits 1) 
  // Enable DDC protocol error interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__DDC_ABORT_INT_MASK__REG_DDC_ABORT_INT_MASK1                               0x02
  // (ReadWrite, Bits 2) 
  // Enable DDC time out interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__DDC_ABORT_INT_MASK__REG_DDC_ABORT_INT_MASK2                               0x04
  // (ReadWrite, Bits 7) 
  // Enable DDC aborted by peer interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__DDC_ABORT_INT_MASK__REG_DDC_ABORT_INT_MASK7                               0x80

// CBUS MSC Requester Abort Interrupt Register
#define REG_ADDR__MSC_MT_ABORT_INT                                       (Page_5 | 0x009A)
  // (ReadWrite, Bits 0) 
  // MSC max fail interrupt status (as requester)
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT0                            0x01
  // (ReadWrite, Bits 1) 
  // MSC protocol error interrupt status (as requester)
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT1                            0x02
  // (ReadWrite, Bits 2) 
  // MSC time out interrupt status (as requester)
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT2                            0x04
  // (ReadWrite, Bits 3) 
  // MSC undefined command initiated interrupt status (as requester)
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT3                            0x08
  // (ReadWrite, Bits 5) 
  // MSC queued command aborted by heartbeat fail interrupt status
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT5                            0x20
  // (ReadWrite, Bits 7) 
  // MSC aborted by peer interrupt status (as requester)
  #define BIT_MSK__MSC_MT_ABORT_INT__REG_MSC_MT_ABORT_INT_STAT7                            0x80

// CBUS MSC Reqeuster Abort Interrupt Mask Register
#define REG_ADDR__MSC_MT_ABORT_INT_MASK                                  (Page_5 | 0x009B)
  // (ReadWrite, Bits 0) 
  // Enable MSC max fail interrupt (as requester): 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK0                            0x01
  // (ReadWrite, Bits 1) 
  // Enable MSC protocol error interrupt (as requester): 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK1                            0x02
  // (ReadWrite, Bits 2) 
  // Enable MSC time out interrupt (as requester): 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK2                            0x04
  // (ReadWrite, Bits 3) 
  // Enable MSC undefined command initiated interrupt (as requester): 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK3                            0x08
  // (ReadWrite, Bits 5) 
  // Enable MSC queued command aborted by heartbeat fail interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK5                            0x20
  // (ReadWrite, Bits 7) 
  // Enable MSC aborted by peer interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MT_ABORT_INT_MASK__REG_MSC_MT_ABORT_INT_MASK7                            0x80

// CBUS MSC Responder Abort Interrupt Register
#define REG_ADDR__MSC_MR_ABORT_INT                                       (Page_5 | 0x009C)
  // (ReadWrite, Bits 0) 
  // MSC max fail interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT0                            0x01
  // (ReadWrite, Bits 1) 
  // MSC protocol error interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT1                            0x02
  // (ReadWrite, Bits 2) 
  // MSC time out interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT2                            0x04
  // (ReadWrite, Bits 3) 
  // MSC undefined command received interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT3                            0x08
  // (ReadWrite, Bits 4) 
  // MSC received bad offset interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT4                            0x10
  // (ReadWrite, Bits 5) 
  // MSC_MSG overflow  interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT5                            0x20
  // (ReadWrite, Bits 7) 
  // MSC aborted by peer interrupt status (as responder)
  #define BIT_MSK__MSC_MR_ABORT_INT__REG_MSC_MR_ABORT_INT_STAT7                            0x80

// CBUS MSC Responder Abort Interrupt Mask Register
#define REG_ADDR__MSC_MR_ABORT_INT_MASK                                  (Page_5 | 0x009D)
  // (ReadWrite, Bits 0) 
  // Enable MSC max fail interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK0                            0x01
  // (ReadWrite, Bits 1) 
  // Enable MSC protocol error interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK1                            0x02
  // (ReadWrite, Bits 2) 
  // Enable MSC time out interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK2                            0x04
  // (ReadWrite, Bits 3) 
  // Enable MSC undefined command received interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK3                            0x08
  // (ReadWrite, Bits 4) 
  // Enable MSC received bad offset interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK4                            0x10
  // (ReadWrite, Bits 5) 
  // Enable MSC_MSG overflow interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK5                            0x20
  // (ReadWrite, Bits 7) 
  // Enable MSC aborted by peer interrupt: 1 - enable; 0 - disable (default)
  #define BIT_MSK__MSC_MR_ABORT_INT_MASK__REG_MSC_MR_ABORT_INT_MASK7                            0x80

// CBUS_Link_Layer Control #1 Register
#define REG_ADDR__CBUS_LINK_CONTROL_1                                    (Page_5 | 0x00A0)
  // (ReadWrite, Bits 5:0) 
  // number of packet for each cbus arbitration
  #define BIT_MSK__CBUS_LINK_CONTROL_1__REG_LNK_PACKET_LIMIT                                  0x3F

// CBUS_Link_Layer Control #2 Register
#define REG_ADDR__CBUS_LINK_CONTROL_2                                    (Page_5 | 0x00A1)
  // (ReadWrite, Bits 3:0) 
  // CBUS Link Layer initiator time-out. Unit in clock cycle. 
  #define BIT_MSK__CBUS_LINK_CONTROL_2__REG_LNK_INI_TIMEOUT_MAX                               0x0F
  // (ReadWrite, Bits 7:4) 
  // CBUS packet receiver glitch filter number of taps. Value 0 means no filter applied.
  #define BIT_MSK__CBUS_LINK_CONTROL_2__REG_LNK_GLITCH_FILTER_SEL                             0xF0

// CBUS_Link_Layer Control #3 Register
#define REG_ADDR__CBUS_LINK_CONTROL_3                                    (Page_5 | 0x00A2)
  // (ReadWrite, Bits 2:0) 
  // CBUS packet receiver algorithm control 000 = Easy CBS with by2 001 = Easy CBC with by2 + by8 (default) 010 = Easy CBC with by2 + by4 011 = Easy CBC with by2 + by4 + by8 1xx = Compare bit count algorithm
  #define BIT_MSK__CBUS_LINK_CONTROL_3__REG_LNK_RCV_BIT_CHECK_OPT                             0x07
  // (ReadWrite, Bits 5:3) 
  // CBUS link follower ACK timing control in clocks.
  #define BIT_MSK__CBUS_LINK_CONTROL_3__REG_LNK_FWR_ACK_OFFSET                                0x38
  // (ReadWrite, Bits 7) 
  // Enable sync filtering
  #define BIT_MSK__CBUS_LINK_CONTROL_3__REG_LNK_SYNC_FILTER_EN                                0x80

// CBUS_Link_Layer Control #4 Register
#define REG_ADDR__CBUS_LINK_CONTROL_4                                    (Page_5 | 0x00A3)
  // (ReadWrite, Bits 2:0) 
  // CBUS link transmission opportunity timeout in bit time unit. This is for after winning the arbitration.
  #define BIT_MSK__CBUS_LINK_CONTROL_4__REG_LNK_XMIT_OPP_TIMEOUT_MAX                          0x07
  // (ReadWrite, Bits 7:3) 
  // CBUS link receiving opportunity timeout in bit time units. This is for after losing the arbitration.
  #define BIT_MSK__CBUS_LINK_CONTROL_4__REG_LNK_RCV_OPP_TIMEOUT_MAX                           0xF8

// CBUS_Link_Layer Control #5 Register
#define REG_ADDR__CBUS_LINK_CONTROL_5                                    (Page_5 | 0x00A4)
  // (ReadWrite, Bits 3:0) 
  // CBUS long HIGH time out (the increment is 36 clocks)
  #define BIT_MSK__CBUS_LINK_CONTROL_5__REG_LNK_HIGH_TIMEOUT_MAX_MSB                          0x0F
  // (ReadWrite, Bits 7:4) 
  // CBUS long LOW time out (the increment is 36 clocks)
  #define BIT_MSK__CBUS_LINK_CONTROL_5__REG_LNK_LOW_TIMEOUT_MAX_MSB                           0xF0

// CBUS_Link_Layer Control #6 Register
#define REG_ADDR__CBUS_LINK_CONTROL_6                                    (Page_5 | 0x00A5)
  // (ReadWrite, Bits 2:0) 
  // CBUS port driver high time in 2 clock unit. (default: 4 clocks)
  #define BIT_MSK__CBUS_LINK_CONTROL_6__REG_LNK_DRV_HIGH_LIMIT                                0x07
  // (ReadWrite, Bits 5:3) 
  // CBUS port driver high disable time in clock unit (default: 2 clocks)
  #define BIT_MSK__CBUS_LINK_CONTROL_6__REG_LNK_CHECK_HIGH_LIMIT                              0x38

// CBUS_Link_Layer Control #7 Register
#define REG_ADDR__CBUS_LINK_CONTROL_7                                    (Page_5 | 0x00A6)
  // (ReadWrite, Bits 4:0) 
  // CBUS link layer bus_controller transmission idle timeout in bit time unit.
  #define BIT_MSK__CBUS_LINK_CONTROL_7__REG_LNK_XMIT_IDLE_TIMEOUT                             0x1F
  // (ReadWrite, Bits 5) 
  // Select the bit time for transmitting (only apply to MHL Tx) 1: register-programmed value 0: calibrated bit time
  #define BIT_MSK__CBUS_LINK_CONTROL_7__REG_LNK_XMIT_BIT_TIME_SEL0                            0x20

// CBUS_Link_Layer Control #8 Register
#define REG_ADDR__CBUS_LINK_CONTROL_8                                    (Page_5 | 0x00A7)
  // (ReadWrite, Bits 7:0) 
  // CBUS link 1 bit time in clocks.  For Source: the default is 8'h1f. For Sink and Dongle: the default is 8'h13.
  #define BIT_MSK__CBUS_LINK_CONTROL_8__REG_LNK_XMIT_BIT_TIME                                 0xFF

// CBUS_Link_Layer Control #9 Register
#define REG_ADDR__CBUS_LINK_CONTROL_9                                    (Page_5 | 0x00A8)
  // (ReadWrite, Bits 2:0) 
  // Number of calibration bytes before the first ACK. Default is 7. 0 is invalid setting.
  #define BIT_MSK__CBUS_LINK_CONTROL_9__REG_LNK_CAL_BYTE_CNT_MIN                              0x07
  // (ReadWrite, Bits 5:3) 
  // Average bit time FIFO depth 
  #define BIT_MSK__CBUS_LINK_CONTROL_9__REG_LNK_CAL_FIFO_DEPTH                                0x38
  // (ReadWrite, Bits 6) 
  // 1: invert CBUS polarity 
  #define BIT_MSK__CBUS_LINK_CONTROL_9__REG_LNK_INV_POLARITY                                  0x40
  // (ReadWrite, Bits 7) 
  // Only applies to MHL Tx (reg_lnk_arb_id = 0) and reg_lnk_xmit_bit_time_sel0 == 1'b1. 0: Tx use the calibrated bit time when calibration is done for initiator 1: Tx use the constantly-calibrated bit time for initiator
  #define BIT_MSK__CBUS_LINK_CONTROL_9__REG_LNK_XMIT_BIT_TIME_SEL1                            0x80

// CBUS_Link_Layer Control #10 Register
#define REG_ADDR__CBUS_LINK_CONTROL_10                                   (Page_5 | 0x00A9)
  // (ReadWrite, Bits 2:0) 
  // In normal operation mode; the max offset value that allows for the different between current sync pulse width and averaged sync pulse width.  Default value is 4. 
  #define BIT_MSK__CBUS_LINK_CONTROL_10__REG_LNK_CAL_SYNC_OFFSET                               0x07
  // (ReadWrite, Bits 5:3) 
  // In normal operation mode; the max offset value that allows for the different between current bit time and averaged bit time value.  Default value is 4. 
  #define BIT_MSK__CBUS_LINK_CONTROL_10__REG_LNK_CAL_BIT_OFFSET                                0x38
  // (ReadWrite, Bits 6) 
  // Select the bit time for undestanding the incoming packets 1: Use the register-programmed transmit bit time 0: Use the calibrated bit time
  #define BIT_MSK__CBUS_LINK_CONTROL_10__REG_LNK_RCV_BIT_TIME_SEL                              0x40
  // (ReadWrite, Bits 7) 
  // 1: Check parity and bi-phase error at the last byte of calication mode before leave calibration mode.   0: Do not check parity and bi-phase error before leave calibration mode. Default value 1
  #define BIT_MSK__CBUS_LINK_CONTROL_10__REG_LNK_CAL_ERROR_CHECK                               0x80

// CBUS_Link_Layer Control #11 Register
#define REG_ADDR__CBUS_LINK_CONTROL_11                                   (Page_5 | 0x00AA)
  // (ReadWrite, Bits 0) 
  // 1: Use CBUS edge transition to synchronize bit boundary in calibration mode 0: Use average bit time for bit boundary in calibration operation mode Default value is 1
  #define BIT_MSK__CBUS_LINK_CONTROL_11__REG_LNK_CAL_BDY_CTL                                   0x01
  // (ReadWrite, Bits 1) 
  // 1: Use CBUS edge transition to synchronize bit boundary in normal operation mode 0: Use average bit time for bit boundary in normal operation mode Default value is 0
  #define BIT_MSK__CBUS_LINK_CONTROL_11__REG_LNK_NORM_BDY_CTL                                  0x02
  // (ReadWrite, Bits 7:2) 
  // CBUS_Link_Layer retry limit. Default value is 0x20
  #define BIT_MSK__CBUS_LINK_CONTROL_11__REG_LNK_XMIT_RETRY_LIMIT                              0xFC

// CBUS Ack Control Register
#define REG_ADDR__CBUS_ACK_CONTROL                                       (Page_5 | 0x00AB)
  // (ReadWrite, Bits 2:0) 
  // CBUS_Link_Layer acknowlodge window size
  #define BIT_MSK__CBUS_ACK_CONTROL__REG_LNK_ACK_WIN_WIDTH                                 0x07

// CBUS Calibration Control Register
#define REG_ADDR__CBUS_CAL_CONTROL                                       (Page_5 | 0x00AC)
  // (ReadWrite, Bits 5:0) 
  // Number of the NACKs on the same packet to enter re-calibaration mode due to bit error. default value is 16 
  #define BIT_MSK__CBUS_CAL_CONTROL__REG_LNK_NACK_CNT_MAX                                  0x3F
  // (ReadWrite, Bits 6) 
  // 1: enable force re-calibration mode when NACK stucks at one packet for number of reg_lnk_nack_limit times due to bit error 0: disable force re-calibration mode 
  #define BIT_MSK__CBUS_CAL_CONTROL__REG_LNK_EN_CAL_BY_NACK                                0x40

// CBUS Link Layer Status #1 Register
#define REG_ADDR__CBUS_LINK_STATUS_0                                     (Page_5 | 0x00AD)
  // (ReadOnly, Bits 7:0) 
  // 1 bit time in number of clock used in receiving
  #define BIT_MSK__CBUS_LINK_STATUS_0__REG_LNK_RCV_BIT_TIME                                  0xFF

// CBUS Xmit ERR CNT Register
#define REG_ADDR__CBUS_XMIT_ERR_CNT                                      (Page_5 | 0x00AE)
  // (ReadWrite, Bits 6:0) 
  // Number of link transmission errors.
  #define BIT_MSK__CBUS_XMIT_ERR_CNT__REG_LNK_XMIT_ERR_COUNT                                0x7F

// CBUS Bi-Phase ERR CNT Register
#define REG_ADDR__CBUS_BIPHZ_ERR_CNT                                     (Page_5 | 0x00AF)
  // (ReadWrite, Bits 3:0) 
  // Bi phase error count in calibration mode; write 0s to clear
  #define BIT_MSK__CBUS_BIPHZ_ERR_CNT__REG_LNK_CAL_BIPHZ_ERR_CNT                             0x0F
  // (ReadWrite, Bits 7:4) 
  // Bi phase error count in normal operation mode; write 0s to clear
  #define BIT_MSK__CBUS_BIPHZ_ERR_CNT__REG_LNK_NORM_BIPHZ_ERR_CNT                            0xF0

// CBUS Parity ERR CNT Register
#define REG_ADDR__CBUS_PARITY_ERR_CNT                                    (Page_5 | 0x00B0)
  // (ReadWrite, Bits 3:0) 
  // Parity error count in calibration mode; write 0s to clear the register
  #define BIT_MSK__CBUS_PARITY_ERR_CNT__REG_LNK_CAL_PARITY_ERR_CNT                            0x0F
  // (ReadWrite, Bits 7:4) 
  // Parity error count in normal operation mode; write 0s to clear the register
  #define BIT_MSK__CBUS_PARITY_ERR_CNT__REG_LNK_RCV_ERR_COUNT                                 0xF0

// CBUS_Link_Layer Sync Filter Control #1 Register
#define REG_ADDR__CBUS_LINK_SYNC_CONTROL_1                               (Page_5 | 0x00B1)
  // (ReadWrite, Bits 3:0) 
  // Adjustments for lower bound to filter sync low time 
  #define BIT_MSK__CBUS_LINK_SYNC_CONTROL_1__REG_LNK_SYNC_LOW_MIN_OFFSET                           0x0F
  // (ReadWrite, Bits 7:4) 
  // Adjustments for upper bound to filter sync low time 
  #define BIT_MSK__CBUS_LINK_SYNC_CONTROL_1__REG_LNK_SYNC_LOW_MAX_OFFSET                           0xF0

// CBUS_Link_Layer Sync Filter Control #2 Register
#define REG_ADDR__CBUS_LINK_SYNC_CONTROL_2                               (Page_5 | 0x00B2)
  // (ReadWrite, Bits 3:0) 
  // Adjustments for lower bound to filter sync bit time 
  #define BIT_MSK__CBUS_LINK_SYNC_CONTROL_2__REG_LNK_SYNC_BIT_MIN_OFFSET                           0x0F
  // (ReadWrite, Bits 7:4) 
  // Adjustments for upper bound to filter sync bit time 
  #define BIT_MSK__CBUS_LINK_SYNC_CONTROL_2__REG_LNK_SYNC_BIT_MAX_OFFSET                           0xF0

// MDT State Machine Status Register
#define REG_ADDR__MDT_SM_STAT                                            (Page_5 | 0x00B5)
  // (ReadOnly, Bits 3:0) 
  // MDT transmit state machine state
  #define BIT_MSK__MDT_SM_STAT__REG_MDT_XMIT_STATE                                    0x0F
  // (ReadOnly, Bits 7:4) 
  // MDT receive state machine state
  #define BIT_MSK__MDT_SM_STAT__REG_MDT_RCV_STATE                                     0xF0

// MDT Peer Adopter ID Low Byte Register
#define REG_ADDR__MDT_PEER_ADOPER_ID_LOW                                 (Page_5 | 0x00B6)
  // (ReadWrite, Bits 7:0) 
  // MDT Peer Adopter ID low byte
  #define BIT_MSK__MDT_PEER_ADOPER_ID_LOW__REG_PEER_ADOPTER_ID_B7_B0                             0xFF

// MDT Peer Adopter ID High Byte Register
#define REG_ADDR__MDT_PEER_ADOPER_ID_HIGH                                (Page_5 | 0x00B7)
  // (ReadWrite, Bits 7:0) 
  // MDT Peer Adopter ID high byte
  #define BIT_MSK__MDT_PEER_ADOPER_ID_HIGH__REG_PEER_ADOPTER_ID_B15_B8                            0xFF

// CBUS MSC 2nd Transmit Data Register
#define REG_ADDR__MSC_2ND_TRANSMIT_DATA                                  (Page_5 | 0x00B8)
  // (ReadWrite, Bits 7:0) 
  // 2nd transmit data sent by MSC requester Only used for MSC_MSG 2nd data (DATA)
  #define BIT_MSK__MSC_2ND_TRANSMIT_DATA__REG_MSC_MT_DATA1                                      0xFF

// CBUS MSC 1st Transmit Data Register
#define REG_ADDR__MSC_1ST_TRANSMIT_DATA                                  (Page_5 | 0x00B9)
  // (ReadWrite, Bits 7:0) 
  // 1st transmit data sent by MSC requester 1. Used for WRITE_STAT/SET_INT transmit data  2. MSC_MSG 1st transmit data (CMD) 3. Used for DEBUG data
  #define BIT_MSK__MSC_1ST_TRANSMIT_DATA__REG_MSC_MT_DATA0                                      0xFF

// CBUS MSC Command/Offset Register
#define REG_ADDR__MSC_CMD_OR_OFFSET                                      (Page_5 | 0x00BA)
  // (ReadWrite, Bits 7:0) 
  // MSC command/offset. This register is used either as command opcode or offset for MSC commands 1. Used as command opcode when MSC_COMMAND_START register bit 0 is set 2. Used as offset when MSC_COMMAND_START register bit 2; 3; or 4 is set 3. Used as {header (2 bits); control (1 bit)} when MSC_COMMAND_START register bit 5 is set 
  #define BIT_MSK__MSC_CMD_OR_OFFSET__REG_MSC_MT_CMD_OR_OFFSET                              0xFF

// CBUS MSC command trigger Register
#define REG_ADDR__MSC_COMMAND_START                                      (Page_5 | 0x00BB)
  // (ReadWrite, Bits 0) 
  // Write 1 to trigger peer command including GET_STATE: GET_VENDOR_ID: SET_HPD: CLR_HPD: SET_CAP_ID: GET_CAP_ID: GET_VS_ABORT: GET_DDC_ABORT: GET_MSC_ABORT. Self-resetting to 0.
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_PEER_CMD                                      0x01
  // (ReadWrite, Bits 1) 
  // Write 1 to trigger MSC_MSG command
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_MSC_MSG_CMD                                   0x02
  // (ReadWrite, Bits 2) 
  // Write 1 to trigger READ_DEVCAP command. Self-resetting to 0.
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_READ_DEVCAP_CMD                               0x04
  // (ReadWrite, Bits 3) 
  // Write 1 to trigger WRITE_STAT/SET_INT command. Self-resetting to 0.
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_WRITE_STAT_CMD                                0x08
  // (ReadWrite, Bits 4) 
  // Write 1 to trigger WRITE_BURST command. Self-resetting to 0.
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_WRITE_BURST_CMD                               0x10
  // (ReadWrite, Bits 5) 
  // Write 1 to send a debug packet. {Header (2 bits): Control (1 bit)} = reg_msc_mt_cmd_or_offset[2:0]: Data = reg_msc_mt_data_cmd[7:0]. Self-resetting to 0.
  #define BIT_MSK__MSC_COMMAND_START__REG_MSC_DEBUG_CMD                                     0x20

// CBUS MSC Requester Received 1st Data Register
#define REG_ADDR__MSC_MT_RCVD_DATA0                                      (Page_5 | 0x00BC)
  // (ReadOnly, Bits 7:0) 
  // 1st data received by MSC requester
  #define BIT_MSK__MSC_MT_RCVD_DATA0__REG_MSC_MT_RCVD_DATA0                                 0xFF

// CBUS MSC Requester Received 2nd Data Register
#define REG_ADDR__MSC_MT_RCVD_DATA1                                      (Page_5 | 0x00BD)
  // (ReadOnly, Bits 7:0) 
  // 2nd data received by MSC requester
  #define BIT_MSK__MSC_MT_RCVD_DATA1__REG_MSC_MT_RCVD_DATA1                                 0xFF

// CBUS MSC Requester Received State Register
#define REG_ADDR__MSC_MT_RCVD_STATE                                      (Page_5 | 0x00BE)
  // (ReadOnly, Bits 7:0) 
  // Peer state received by MSC requester (GET_STATE)
  #define BIT_MSK__MSC_MT_RCVD_STATE__REG_MSC_MT_RCVD_STATE                                 0xFF

// CBUS MSC Responder MSC_MSG Received 1st Data Register
#define REG_ADDR__MSC_MR_MSC_MSG_RCVD_1ST_DATA                           (Page_5 | 0x00BF)
  // (ReadOnly, Bits 7:0) 
  // 1st received data from MSC_MSG
  #define BIT_MSK__MSC_MR_MSC_MSG_RCVD_1ST_DATA__REG_MSC_MR_MSC_MSG_RCVD_DATA0                         0xFF

// CBUS MSC Responder MSC_MSG Received Data Register
#define REG_ADDR__MSC_MR_MSC_MSG_RCVD_2ND_DATA                           (Page_5 | 0x00C0)
  // (ReadOnly, Bits 7:0) 
  // 2nd received data from MSC_MSG
  #define BIT_MSK__MSC_MR_MSC_MSG_RCVD_2ND_DATA__REG_MSC_MR_MSC_MSG_RCVD_DATA1                         0xFF

// CBUS MSC Responder Received Offset Register
#define REG_ADDR__MSC_MR_RCVD_OFFSET                                     (Page_5 | 0x00C1)
  // (ReadOnly, Bits 7:0) 
  // received offset for certain MSC commands. Apply to WRITE_STAT SET_INT WRITE_BURST and READ_DEVCAP.
  #define BIT_MSK__MSC_MR_RCVD_OFFSET__REG_MSC_MR_RCVD_OFFSET                                0xFF

// CBUS MSC Responder Rcvd Data Register
#define REG_ADDR__MSC_MR_RCVD_DATA                                       (Page_5 | 0x00C2)
  // (ReadOnly, Bits 7:0) 
  // received data for certain MSC commands. Apply to WRITE_STAT SET_INT WRITE_BURST (only the last data)
  #define BIT_MSK__MSC_MR_RCVD_DATA__REG_MSC_MR_RCVD_DATA                                  0xFF

// CBUS MSC Rcvd Data Cnt Register
#define REG_ADDR__MSC_RCVD_DATA_CNT                                      (Page_5 | 0x00C3)
  // (ReadOnly, Bits 4:0) 
  // received data count for MSC WRITE_BURST
  #define BIT_MSK__MSC_RCVD_DATA_CNT__REG_MSC_MR_RCVD_DATA_CNT                              0x1F

// CBUS MSC Heartbeat Control Register
#define REG_ADDR__MSC_HEARTBEAT_CONTROL                                  (Page_5 | 0x00C4)
  // (ReadWrite, Bits 3:0) 
  // Select the heart beat period. 4'h0: 3.125ms 4'h1: 6.25ms 4'h2: 10ms 4'h3: 12.5ms 4'h4: 20ms 4'h5: 25ms 4'h6: 37.5ms 4'h7: 50ms 4'h8: 75ms 4'h9: 100ms 4'hA: 125ms 4'hB: 150ms 4'hC: 200ms 4'hD: 250ms 4'hE: 300ms 4'hF: 400ms
  #define BIT_MSK__MSC_HEARTBEAT_CONTROL__REG_MSC_HB_PERIOD_MSB                                 0x0F
  // (ReadWrite, Bits 6:4) 
  // MSC heartbeat maximum number of consecutive fails  3'b000: 5'h01; 3'b001: 5'h02; 3'b010: 5'h04; 3'b011: 5'h06;  3'b100: 5'h08; 3'b101: 5'h0A; 3'b110: 5'h0C; 3'b111: 5'h0E;
  #define BIT_MSK__MSC_HEARTBEAT_CONTROL__REG_MSC_HB_FAIL_LIMIT                                 0x70
  // (ReadWrite, Bits 7) 
  // MSC heartbeat enable
  #define BIT_MSK__MSC_HEARTBEAT_CONTROL__REG_MSC_HB_EN                                         0x80

// CBUS MSC Heartbeat Fail Count Register
#define REG_ADDR__MSC_HEARTBEAT_FAIL_CNT                                 (Page_5 | 0x00C5)
  // (ReadWrite, Bits 4:0) 
  // MSC consecutive heartbeat fail count
  #define BIT_MSK__MSC_HEARTBEAT_FAIL_CNT__REG_MSC_HB_FAIL_CNT                                   0x1F
  // (ReadWrite, Bits 7) 
  // Enable heartbeat automatically after MHL discovered if reg_msc_hb_en (0xC4[7]) is set
  #define BIT_MSK__MSC_HEARTBEAT_FAIL_CNT__REG_HW_HB_EN                                          0x80

// CBUS MSC Write_Burst length Register
#define REG_ADDR__MSC_WRITE_BURST_DATA_LEN                               (Page_5 | 0x00C6)
  // (ReadWrite, Bits 3:0) 
  // The number of data bytes to be transmitted for MSC WRITE_BURST. 0 means 1 byte to transmit and 4'b1111 means 16 bytes to transmit.
  #define BIT_MSK__MSC_WRITE_BURST_DATA_LEN__REG_MSC_WRITE_BURST_LEN                               0x0F

// CBUS MSC Compatibility Control Register
#define REG_ADDR__CBUS_MSC_COMPATIBILITY_CONTROL                         (Page_5 | 0x00C7)
  // (ReadWrite, Bits 2) 
  // Disable GET_VS1_ERRORCODE
  #define BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_DISABLE_GET_VS1_ERRORCODE                         0x04
  // (ReadWrite, Bits 3) 
  // Disable GET_DDC_ERRORCODE
  #define BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_DISABLE_GET_DDC_ERRORCODE                         0x08
  // (ReadWrite, Bits 5) 
  // Disable DDC on CBUS
  #define BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_DISABLE_DDC_ON_CBUS                               0x20
  // (ReadWrite, Bits 6) 
  // Disable MSC on CBUS
  #define BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_DISABLE_MSC_ON_CBUS                               0x40
  // (ReadWrite, Bits 7) 
  // Enable READ_XDEVCAP and WRITE_STAT/SET_INT for extended offset no matter MHL3 or MHL1/2
  #define BIT_MSK__CBUS_MSC_COMPATIBILITY_CONTROL__REG_XDEVCAP_EN                                        0x80

// CBUS DDC Abort Reason Register
#define REG_ADDR__DDC_ERRORCODE                                          (Page_5 | 0x00CB)
  // (ReadWrite, Bits 7:0) 
  // DDC error code that can be fetched over CBUS by GET_DDC_ERRORCODE
  #define BIT_MSK__DDC_ERRORCODE__REG_DDC_ERRORCODE                                     0xFF

// CBUS MSC Abort Reason Register
#define REG_ADDR__MSC_ERRORCODE                                          (Page_5 | 0x00CC)
  // (ReadWrite, Bits 7:0) 
  // MSC error code that can be fetched over CBUS by GET_MSC_ERRORCODE
  #define BIT_MSK__MSC_ERRORCODE__REG_MSC_ERRORCODE                                     0xFF

// CBUS DDC Fail Limit Register
#define REG_ADDR__CBUS_DDC_FAIL_LIMIT                                    (Page_5 | 0x00CE)
  // (ReadWrite, Bits 3:0) 
  // The total number of translation layer retry allowed for DDC before declaring a max fail.
  #define BIT_MSK__CBUS_DDC_FAIL_LIMIT__REG_DDC_RETRY_FAIL_LIMIT                              0x0F

// CBUS MSC Fail Limit Register
#define REG_ADDR__CBUS_MSC_FAIL_LIMIT                                    (Page_5 | 0x00CF)
  // (ReadWrite, Bits 3:0) 
  // The total number of translation layer retry allowed for MSC before declaring a max fail.
  #define BIT_MSK__CBUS_MSC_FAIL_LIMIT__REG_MSC_RETRY_FAIL_LIMIT                              0x0F

// CBUS DDC Time Out Register
#define REG_ADDR__DDC_TIMEOUT                                            (Page_5 | 0x00D1)
  // (ReadWrite, Bits 7:0) 
  // DDC time out thereshold MSB (increment of 1048576 clocks)
  #define BIT_MSK__DDC_TIMEOUT__REG_DDC_TIMEOUT_MAX_MSB                               0xFF

// CBUS MSC Time Out Register
#define REG_ADDR__MSC_TIMEOUT                                            (Page_5 | 0x00D2)
  // (ReadWrite, Bits 7:0) 
  // MSC time out threshold MSB (increment of 1048576 clocks)
  #define BIT_MSK__MSC_TIMEOUT__REG_MSC_TIMEOUT_MAX_MSB                               0xFF

// CBUS DDC Retry Interval Register
#define REG_ADDR__DDC_RETRY_INTERVAL                                     (Page_5 | 0x00D4)
  // (ReadWrite, Bits 7:0) 
  // The idle time MSB for DDC translation layer retry (increment of 512 clocks)
  #define BIT_MSK__DDC_RETRY_INTERVAL__REG_DDC_RETRY_MAX_MSB                                 0xFF

// CBUS MSC Retry Interval Register
#define REG_ADDR__MSC_RETRY_INTERVAL                                     (Page_5 | 0x00D5)
  // (ReadWrite, Bits 7:0) 
  // The idle time MSB for MSC translation layer retry (increment of 512 clocks)
  #define BIT_MSK__MSC_RETRY_INTERVAL__REG_MSC_RETRY_MAX_MSB                                 0xFF

// CBUS DDC Master Speed Control Register
#define REG_ADDR__CBUS_DDC_SPEED_CONTROL                                 (Page_5 | 0x00D6)
  // (ReadWrite, Bits 7:0) 
  // CBUS DDC master speed control. tLOW = 2 * (reg_ddc_speed_ctrl + 1) * clock period. tHIGH = (2 * (reg_ddc_speed_ctrl  + 1)  + 4) * clock period
  #define BIT_MSK__CBUS_DDC_SPEED_CONTROL__REG_DDC_SPEED_CTRL                                    0xFF

// CBUS DDC START STOP Timing Control Register
#define REG_ADDR__CBUS_DDC_ST_SP_TIMING_CONTROL                          (Page_5 | 0x00D7)
  // (ReadWrite, Bits 3:0) 
  // CBUS DDC master tSU;STA control. tSU;STA = (reg_ddc_sp_timing_ctrl * 16 + 1) * 2 * clock_period
  #define BIT_MSK__CBUS_DDC_ST_SP_TIMING_CONTROL__REG_DDC_ST_TIMING_CTRL                                0x0F
  // (ReadWrite, Bits 7:4) 
  // CBUS DDC master tSU;STO control. tSU;STO = (reg_ddc_sp_timing_ctrl * 16 + 5) * clock_period
  #define BIT_MSK__CBUS_DDC_ST_SP_TIMING_CONTROL__REG_DDC_SP_TIMING_CTRL                                0xF0

// CBUS MISC Control Register
#define REG_ADDR__CBUS_MISC_CONTROL                                      (Page_5 | 0x00D8)
  // (ReadWrite, Bits 0) 
  // Enable Ri prefetch when upstream DDC master is trying to read Ri.
  #define BIT_MSK__CBUS_MISC_CONTROL__REG_RI_PREFETCH_EN                                    0x01
  // (ReadWrite, Bits 2) 
  // When is not set: block cbus device capability register DEV_CAT to be changed by FW
  #define BIT_MSK__CBUS_MISC_CONTROL__REG_DEV_CAT_OVR                                       0x04
  // (ReadWrite, Bits 3) 
  // When enabled: enhance the performance when CBUS slew rate is not good.
  #define BIT_MSK__CBUS_MISC_CONTROL__REG_SLOW_SL_ENH                                       0x08

// CBUS MISC Control Register
#define REG_ADDR__CBUS_RCVD_PAYLOAD                                      (Page_5 | 0x00D9)
  // (ReadOnly, Bits 7:0) 
  // payload for last received CBUS packet
  #define BIT_MSK__CBUS_RCVD_PAYLOAD__REG_CBUS_RCVD_PAYLOAD                                 0xFF

// CBUS MISC Control Register
#define REG_ADDR__CBUS_RCVD_HC                                           (Page_5 | 0x00DA)
  // (ReadOnly, Bits 2:0) 
  // header and control for last received CBUS packet
  #define BIT_MSK__CBUS_RCVD_HC__REG_CBUS_RCVD_HC                                      0x07

// CBUS DDC EDID Slave Address Register
#define REG_ADDR__CBUS_DDC_EDID                                          (Page_5 | 0x00DB)
  // (ReadWrite, Bits 7:0) 
  // cbus edid access slave address configuration
  #define BIT_MSK__CBUS_DDC_EDID__REG_CBUS_DDC_EDID_ADDR                                0xFF

// CBUS1 Converter Control Register
#define REG_ADDR__CBUS1_CNVT                                             (Page_5 | 0x00DC)
  // (ReadWrite, Bits 0) 
  // Enable bit for cbus1 Converter.
  #define BIT_MSK__CBUS1_CNVT__REG_CBUS1CNVT_EN                                      0x01
  // (ReadWrite, Bits 1) 
  // Enable bit to tear down CBUS if not needed.
  #define BIT_MSK__CBUS1_CNVT__REG_TEARCBUS_EN                                       0x02
  // (ReadWrite, Bits 3:2) 
  // Peer TDM No ACK/NAK timeout select. 00: 2k TDM cycles; 01: 4k; 10: 8k; 11: 16k 
  #define BIT_MSK__CBUS1_CNVT__REG_CBUS1_PEERTOUT_SEL                                0x0C
  // (ReadWrite, Bits 7:4) 
  // Retry limit for cbus1 transmit. Default is 11 for Source and 9 for Sink. Granularity is 4. Source will retry for (reg_cbus1_retrylmt*4+3) times. Sink will retry for (reg_cbus1_retrylmt*4+1) times.
  #define BIT_MSK__CBUS1_CNVT__REG_CBUS1_RETRYLMT                                    0xF0

// CBUS1 Converter INTR Register
#define REG_ADDR__CBUS1_INTR                                             (Page_5 | 0x00DD)
  // (ReadWrite, Bits 0) 
  // cbus1 Interrupt Event 0; local TDM timeout
  #define BIT_MSK__CBUS1_INTR__REG_CBUS1_INTR_0                                      0x01
  // (ReadWrite, Bits 1) 
  // cbus1 Interrupt Event 1; peer TDM timeout
  #define BIT_MSK__CBUS1_INTR__REG_CBUS1_INTR_1                                      0x02
  // (ReadWrite, Bits 2) 
  // cbus1 Interrupt Event 2; Header collision detected
  #define BIT_MSK__CBUS1_INTR__REG_CBUS1_INTR_2                                      0x04

// CBUS1 Converter INTRMASK Register
#define REG_ADDR__CBUS1_INTRMASK                                         (Page_5 | 0x00DE)
  // (ReadWrite, Bits 0) 
  // cbus1 Interrupt Mask bit for Event 0
  #define BIT_MSK__CBUS1_INTRMASK__REG_CBUS1_INTRMASK_0                                  0x01
  // (ReadWrite, Bits 1) 
  // cbus1 Interrupt Mask bit for Event 1
  #define BIT_MSK__CBUS1_INTRMASK__REG_CBUS1_INTRMASK_1                                  0x02
  // (ReadWrite, Bits 2) 
  // cbus1 Interrupt Mask bit for Event 2
  #define BIT_MSK__CBUS1_INTRMASK__REG_CBUS1_INTRMASK_2                                  0x04

// CBUS1 Converter Status Register
#define REG_ADDR__CBUS1_STATUS                                           (Page_5 | 0x00DF)
  // (ReadWrite, Bits 4:0) 
  // cbus1 Arbiter state.
  #define BIT_MSK__CBUS1_STATUS__REG_CBUS1_STATE                                       0x1F
  // (ReadWrite, Bits 7) 
  // TDM 8bit Interface enable bit.
  #define BIT_MSK__CBUS1_STATUS__REG_TDM8BIF_EN                                        0x80

// Discovery Control1 Register
#define REG_ADDR__DISC_CTRL1                                             (Page_5 | 0x00E0)
  // (ReadWrite, Bits 0) 
  // CBUS device discovery enable bit. 0 - discovery is disabled (default). 1 - discovery is enabled.
  #define BIT_MSK__DISC_CTRL1__REG_DISC_EN                                           0x01
  // (ReadWrite, Bits 3:2) 
  // CBUS discovery cycle time for each drive and float 00: 50us 01: 100us (default) 10: 150us 11: 200us
  #define BIT_MSK__DISC_CTRL1__REG_DISC_CYC                                          0x0C
  // (ReadWrite, Bits 5:4) 
  // Number of sending discovery pulses from TX 00: 10 01: 20 10: 30 11: 40
  #define BIT_MSK__DISC_CTRL1__REG_DISC_ATT                                          0x30
  // (ReadWrite, Bits 6) 
  // Set to make MHL RxSense equal to heartbeat. If both this bit and 0xE1[5] are not set; MHL RxSense is equal to TMDS RxSense
  #define BIT_MSK__DISC_CTRL1__REG_HB_ONLY                                           0x40
  // (ReadWrite, Bits 7) 
  // Cbus interrupt enable bit. When set 1; latched cbus falling edge will generate interrupt. This interrupt can only be cleared by HW reset. Default is 0.
  #define BIT_MSK__DISC_CTRL1__REG_CBUS_INTR_EN                                      0x80

// Discovery Control2 Register
#define REG_ADDR__DISC_CTRL2                                             (Page_5 | 0x00E1)
  // (ReadWrite, Bits 0) 
  // Enable CBUS communication channel running at 125KHz 1- Enable 0- Disable (default)
  #define BIT_MSK__DISC_CTRL2__REG_CBUS125K_EN                                       0x01
  // (ReadWrite, Bits 1) 
  // Indicate whether 125KHz clock for CBUS communication channel be enabled through SW or HW 1- Enable 125KHz through SW (reg_cbus12k_en) (default) 0-Enable 125KHz through HW
  #define BIT_MSK__DISC_CTRL2__REG_CBUS125K_SWOVRIDE                                 0x02
  // (ReadWrite, Bits 3) 
  // Enable internal 100K pull-up.  Default: 1  0  Disable  1  Enable(default)  This bit is reused as USB switch select normal drive high/low control; active low (reg_usb_d_oen). 0: usb_d_sw is output to CI2CA with polarity control from reg_ci2ca_pol; 1: disabled (default)
  #define BIT_MSK__DISC_CTRL2__REG_CBUS100K_EN                                       0x08
  // (ReadWrite, Bits 5) 
  // If 0xE0[6] is not set; set to make MHL RxSense equal to Heartbeat (oCBUS) OR link activity check. If both this bit and 0xE0[6] are not set; MHL RxSense is equal to TMDS RxSense. 
  #define BIT_MSK__DISC_CTRL2__REG_HB_LCC_BOTH                                       0x20
  // (ReadWrite, Bits 6) 
  // To skip RGND detection on TX side if this bit is enabled. =0 (default): detection is conducted; =1: detection is skipped;
  #define BIT_MSK__DISC_CTRL2__REG_SKIP_RGND                                         0x40
  // (ReadWrite, Bits 7) 
  // CBUS delay trigger enable bit. When this bit is enable; a watch dog is triggered upon CBUS_PON; and CBUS reset is activated till the expiration of the timer. 0: Disable 1: Enable (default)
  #define BIT_MSK__DISC_CTRL2__REG_DLYTRG_EN                                         0x80

// Discovery Control3 Register
#define REG_ADDR__DISC_CTRL3                                             (Page_5 | 0x00E2)
  // (ReadWrite, Bits 5) 
  // Simulation mode for CBUS discovery. When enabled; time constant used by CBUS Disc is shortened. 0: disable (default) 1: enable
  #define BIT_MSK__DISC_CTRL3__REG_DISC_SIMODE                                       0x20
  // (ReadWrite, Bits 6) 
  // Forced MHL mode. This bit controls both usb_id_on and usb_d_on signals of the USB switch at the same time. 1 - MHL mode is forced true 0 - Not to force MHL mode. The mode is established based on CBUS discovery (default).
  #define BIT_MSK__DISC_CTRL3__REG_FORCE_MHL                                         0x40
  // (ReadWrite, Bits 7) 
  // This bit controls immediate communication of CBUS after MHL discovered or hold until micro allows to do so. =1 (default); immediate communication; =0: on hold till micro allows it;
  #define BIT_MSK__DISC_CTRL3__REG_COMM_IMME                                         0x80

// Discovery Control4 Register
#define REG_ADDR__DISC_CTRL4                                             (Page_5 | 0x00E3)
  // (ReadWrite, Bits 5:4) 
  // CBUS Pull Up select for IDLE states 00 (default): Off 01: 20k 10: 10k 11: 5k
  #define BIT_MSK__DISC_CTRL4__REG_CBUSIDLE_PUP_SEL                                  0x30
  // (ReadWrite, Bits 7:6) 
  // CBUS Pull Up select for discovery states 00 : Off 01: 20k 10(default): 10k 11: 5k
  #define BIT_MSK__DISC_CTRL4__REG_CBUSDISC_PUP_SEL                                  0xC0

// Discovery Control5 Register
#define REG_ADDR__DISC_CTRL5                                             (Page_5 | 0x00E4)
  // (ReadWrite, Bits 1:0) 
  // CBUS Pull Up select for MHL states 00: Off 01: 20k 10: 10k 11: 5k (default)
  #define BIT_MSK__DISC_CTRL5__REG_CBUSMHL_PUP_SEL                                   0x03
  // (ReadWrite, Bits 3) 
  // Discovery State Machine override. Default is 0. When enable;Discovery SM is forced by FM at reg_disc_sm[4:0].
  #define BIT_MSK__DISC_CTRL5__REG_DSM_OVRIDE                                        0x08

// Discovery Control6 Register
#define REG_ADDR__DISC_CTRL6                                             (Page_5 | 0x00E5)
  // (ReadWrite, Bits 0) 
  // Single discovery attempt. When this bit is enable; MHL will establish if CBUS stays high after first drive and float.  When disable; multiple attempts are needed. Default is 1.
  #define BIT_MSK__DISC_CTRL6__REG_SINGLE_ATT                                        0x01
  // (ReadWrite, Bits 1) 
  // This is enable bit to allow CI2CA serve as a wake up signal at D3; if MCU needs to wake up  quietly. 0 - disable (default) 1 - enable
  #define BIT_MSK__DISC_CTRL6__REG_CI2CA_WKUP                                        0x02
  // (ReadWrite, Bits 2) 
  // This bit indicates the polarity of CI2CA wake up pulse. 0 - wake up pulse active high (default) 1 - wake up pulse active low  This bit has second definition to control the polarity for the option to output USB switch select on CI2CA. When this feature is on through reg_usb_d_oen or reg_use_d_odn; this is the definition: 0: CI2CA output LOW means USB and output HIGH means MHL(default); 1: CI2CA output LOW means MHL and output HIGH means USB; 
  #define BIT_MSK__DISC_CTRL6__REG_CI2CA_POL                                         0x04
  // (ReadWrite, Bits 4) 
  // This bit indicates whether to block RGND INT in Discovery SM. When set block; after interrupt clear; reg_drvflt_sel will decide whether to go to DRV_FLT or to USB_ESTABLISHED state. 0 - no block  1 - block (default)
  #define BIT_MSK__DISC_CTRL6__REG_BLOCK_RGNDINT                                     0x10

// Discovery Control7 Register
#define REG_ADDR__DISC_CTRL7                                             (Page_5 | 0x00E6)
  // (ReadWrite, Bits 1:0) 
  // This register defines the way to interprete RGND. 00 - only 1k is considered MHL impedance value (default) 01 - only 2k is considered MHL impedance value  10 - both 1k and 2k are considered MHL impedance value 11 - invalid
  #define BIT_MSK__DISC_CTRL7__REG_RGND_INTP                                         0x03
  // (ReadWrite, Bits 3) 
  // This bit defines CTRL BUS debug mode. When enable; CTRL BUS related signals will map to GPIO pins (GPIO4 is for cbus_out and GPIO5 is for cbus_oe). 0: disable (default) 1: enable
  #define BIT_MSK__DISC_CTRL7__REG_CBUS_DBGMODE                                      0x08
  // (ReadWrite, Bits 6) 
  // This bit controls CtrlBUS level from SW or HW. 0: from HW (default) 1: from register bit reg_cbuslvl_val
  #define BIT_MSK__DISC_CTRL7__REG_CBUSLVL_SW                                        0x40
  // (ReadWrite, Bits 7) 
  // This bit controls CtrlBUS level at SW mode. 0: level 0 (default) 1: level 1
  #define BIT_MSK__DISC_CTRL7__REG_CBUSLVL_VAL                                       0x80

// Discovery Control8 Register
#define REG_ADDR__DISC_CTRL8                                             (Page_5 | 0x00E7)
  // (ReadWrite, Bits 0) 
  // Delay CBUS discovery related interrupt until internally generated reset go high when enabled
  #define BIT_MSK__DISC_CTRL8__REG_DELAY_CBUS_INTR_EN                                0x01
  // (ReadWrite, Bits 7) 
  // Allow state go from NOMHL_EST to IDLE  without waiting for nomhlint_clr
  #define BIT_MSK__DISC_CTRL8__REG_NOMHLINT_CLR_BYPASS                               0x80

// Discovery Control9 Register
#define REG_ADDR__DISC_CTRL9                                             (Page_5 | 0x00E8)
  // (ReadWrite, Bits 0) 
  // VBUS Output Capability Source 0: No OTG. No need to monitor VBUS level 1: OTG. Need to monitor if VBUS is high for 100ms to 1000ms
  #define BIT_MSK__DISC_CTRL9__REG_VBUS_OUTPUT_CAPABILITY_SRC                        0x01
  // (ReadWrite, Bits 1) 
  // Wake pulse bypass 0: wake pulse sequence will be generated (default) 1: wake pulse sequence will be bypassed and not generated
  #define BIT_MSK__DISC_CTRL9__REG_WAKE_PULSE_BYPASS                                 0x02
  // (ReadWrite, Bits 2) 
  // Discovery pulse proceed 0: discovery pulse will not be generated after wake pulse sequence 1: discovery pulse will be generated after wake pulse sequence (default)
  #define BIT_MSK__DISC_CTRL9__REG_DISC_PULSE_PROCEED                                0x04
  // (ReadWrite, Bits 3) 
  // NO MHL establish 0: NOMHL not establish (default) 1: NOMHL establish
  #define BIT_MSK__DISC_CTRL9__REG_NOMHL_EST                                         0x08
  // (ReadWrite, Bits 4) 
  // 0: float low for wake pulse  1: drive low for wake pulse (default)
  #define BIT_MSK__DISC_CTRL9__REG_WAKE_DRVFLT                                       0x10

// Discovery Control10 Register
#define REG_ADDR__DISC_CTRL10                                            (Page_5 | 0x00E9)
  // (ReadWrite, Bits 7:0) 
  // TSRC:WAKE_PULSE_WIDTH_1
  #define BIT_MSK__DISC_CTRL10__REG_TSRC_WAKE_PULSE_WIDTH_1                           0xFF

// Discovery Control11 Register
#define REG_ADDR__DISC_CTRL11                                            (Page_5 | 0x00EA)
  // (ReadWrite, Bits 7:0) 
  // TSRC:WAKE_PULSE_WIDTH_2
  #define BIT_MSK__DISC_CTRL11__REG_TSRC_WAKE_PULSE_WIDTH_2                           0xFF

// Discovery Status1 Register
#define REG_ADDR__DISC_STAT1                                             (Page_5 | 0x00EB)
  // (ReadWrite, Bits 3:0) 
  // Discovery State Machine read back
  #define BIT_MSK__DISC_STAT1__REG_DISC_SM                                           0x0F
  // (ReadWrite, Bits 5) 
  // Power State Machine override. Default is 0. When enable; Power SM is forced by FM by reg_pwr_sm[1:0].
  #define BIT_MSK__DISC_STAT1__REG_PSM_OVRIDE                                        0x20

// Discovery Status2 Register
#define REG_ADDR__DISC_STAT2                                             (Page_5 | 0x00EC)
  // (ReadOnly, Bits 1:0) 
  // RGND status 00: open 01: 2k 10: 1k 11: short
  #define BIT_MSK__DISC_STAT2__REG_RGND                                              0x03
  // (ReadOnly, Bits 4) 
  // receiver sense
  #define BIT_MSK__DISC_STAT2__REG_RSEN                                              0x10
  // (ReadOnly, Bits 5) 
  // CBUS level status
  #define BIT_MSK__DISC_STAT2__REG_CBUS_SATUS                                        0x20
  // (ReadWrite, Bits 6) 
  // CBUS_OE polarity
  #define BIT_MSK__DISC_STAT2__REG_CBUS_OE_POL                                       0x40

// Interrupt CBUS_reg1 INTR0 Register
#define REG_ADDR__CBUS_DISC_INTR0                                        (Page_5 | 0x00ED)
  // (ReadWrite, Bits 0) 
  // cbus level toggle interrupt for all MHL modes. It is desired to know when cbus level changes and keep the value for some certain clock periods.
  #define BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT0                             0x01
  // (ReadWrite, Bits 2) 
  // MHL mode established. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT2                             0x04
  // (ReadWrite, Bits 3) 
  // Not MHL mode established. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT3                             0x08
  // (ReadWrite, Bits 5) 
  // CBUS (MHL) disconnect. Write 1 to clear.
  #define BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT5                             0x20
  // (ReadWrite, Bits 6) 
  // RGND Detection is done and RGND is available for read.
  #define BIT_MSK__CBUS_DISC_INTR0__REG_CBUS_DISC_INTR0_STAT6                             0x40

// Interrupt CBUS_reg1 INTR0 Mask Register
#define REG_ADDR__CBUS_DISC_INTR0_MASK                                   (Page_5 | 0x00EE)
  // (ReadWrite, Bits 0) 
  // Enable INT[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK0                             0x01
  // (ReadWrite, Bits 2) 
  // Enable INT[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK2                             0x04
  // (ReadWrite, Bits 3) 
  // Enable INT[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK3                             0x08
  // (ReadWrite, Bits 5) 
  // Enable INT[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK5                             0x20
  // (ReadWrite, Bits 6) 
  // Enable INT[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__CBUS_DISC_INTR0_MASK__REG_CBUS_DISC_INTR0_MASK6                             0x40

// MHL TX Test Control Register
#define REG_ADDR__MHLTX_TESTCTRL                                         (Page_5 | 0x00EF)
  // (ReadWrite, Bits 0) 
  // Set to 1'b1 to reset CBUS Discovery
  #define BIT_MSK__MHLTX_TESTCTRL__REG_CBUS_DISC_RST                                     0x01
  // (ReadWrite, Bits 5) 
  // Evaluation Mode
  #define BIT_MSK__MHLTX_TESTCTRL__REG_EVAL_MODE                                         0x20

// Tx Discovery Aggregated Interrupt Status Register
#define REG_ADDR__DISC_TX_AGGREGATED_INTR_STAT                           (Page_5 | 0x00F0)
  // (ReadOnly, Bits 0) 
  // Tx discovery aggregated interrupt status
  #define BIT_MSK__DISC_TX_AGGREGATED_INTR_STAT__REG_CBUS_DISC_TX_INTR_STAT                            0x01

//***************************************************************************
// page 36. Address: 40
// TPI DTD Byte7 Register
#define REG_ADDR__TPI_MISC                                               (Page_6 | 0x0007)
  // (ReadWrite, Bits 0) 
  // Select the event to trigger the interrupt at 0x3D[2] 1: Interrupt will be asserted only after both BKSV and BCAPS are read successfully 0: Interrupt will be asserted after BKSV is read successfully
  #define BIT_MSK__TPI_MISC__REG_ONLY_BKSV_DONE_SEL                                0x01

// TPI Pixel Repetition Data Register
#define REG_ADDR__TPI_PRD                                                (Page_6 | 0x0008)
  // (ReadWrite, Bits 1:0) 
  // Clock mode register bits 00 = Pixel data is not replicated (default).   01 = Pixels are replicated once.   11 = Pixels are replicated four times.   10 = rsvd.   (Note: Whatever is programmed into these 2 bits is also the value to program into the pixel replication field of the AVI v2 data byte 5 if the EnDeMux bit is clear (register 0x4A bit 1). Use the next higher entry if EnDeMux is set. )
  #define BIT_MSK__TPI_PRD__REG_ICLK                                              0x03
  // (ReadWrite, Bits 4) 
  // Edge Select (same function as EDGE pin)                                                  0  Input data is falling edge latched (falling edge latched first in dual edge mode)                                                                                                       1  Input data is rising edge latched (rising edge latched first in dual edge mode)
  #define BIT_MSK__TPI_PRD__REG_EDGE                                              0x10

// Input Format Register
#define REG_ADDR__TPI_INPUT                                              (Page_6 | 0x0009)
  // (ReadWrite, Bits 1:0) 
  // 00 = RGB 01 = YCbCr 4:4:4 10 = YCbCr 4:2:2 11 = Internal RGB solid color (from blank register bytes)
  #define BIT_MSK__TPI_INPUT__REG_INPUT_FORMAT                                      0x03
  // (ReadWrite, Bits 3:2) 
  // 00 = Auto-selected by [1:0] 01 = Full range (0-255) 10 = Limited range (16-235) 11 = Rsvd
  #define BIT_MSK__TPI_INPUT__REG_INPUT_QUAN_RANGE                                  0x0C

// Output Format Register
#define REG_ADDR__TPI_OUTPUT                                             (Page_6 | 0x000A)
  // (ReadWrite, Bits 1:0) 
  // 00 = HDMI to RGB 01 = HDMI to YCbCr 4:4:4 10 = HDMI to YCbCr 4:2:2 11 = DVI to RGB
  #define BIT_MSK__TPI_OUTPUT__REG_OUTPUT_FORMAT                                     0x03
  // (ReadWrite, Bits 3:2) 
  // 00 = Auto-selected by [1:0] 01 = Full range (0-255) 10 = Limited range (16-235) 11 = Rsvd
  #define BIT_MSK__TPI_OUTPUT__REG_OUTPUT_QUAN_RANGE                                 0x0C

// TPI AVI Check Sum Register
#define REG_ADDR__TPI_AVI_CHSUM                                          (Page_6 | 0x000C)
  // (ReadWrite, Bits 7:0) 
  // AVI InfoFrame Checksum (also pre loads AVI Header to 0x0282; and AVI Length to 13)
  #define BIT_MSK__TPI_AVI_CHSUM__TPI_AVI_CHSUM                                         0xFF

// TPI AVI Data Byte 1 Register
#define REG_ADDR__TPI_AVI_BYTE1                                          (Page_6 | 0x000D)
  // (ReadWrite, Bits 7:0) 
  // Bit[1:0] = S1:0 scan information Bit[3:2] = B1:0 bar info data valid Bit[4] = A0 active info Bit[6:5] = Y1:0 RGB/YCbCr Indicator Bit[7] = reserved
  #define BIT_MSK__TPI_AVI_BYTE1__TPI_AVI_BYTE1                                         0xFF

// TPI AVI Data Byte 2 Register
#define REG_ADDR__TPI_AVI_BYTE2                                          (Page_6 | 0x000E)
  // (ReadWrite, Bits 7:0) 
  // Bit[3:0] = R 3:0 active format aspect ratio Bit[5:4] = M 1:0 picture aspect ratio Bit[7:6] = C 1:0 colorimetry info
  #define BIT_MSK__TPI_AVI_BYTE2__TPI_AVI_BYTE2                                         0xFF

// TPI AVI Data Byte 3 Register
#define REG_ADDR__TPI_AVI_BYTE3                                          (Page_6 | 0x000F)
  // (ReadWrite, Bits 7:0) 
  // Bit[1:0] = SC 1:0 non-uniform scaling Bit[3:2] = Q 1:0 RGB Quantization Range Bit[6:4] = ES 2:0 Extended Colorimetry Bit[7] = ITC IT Content
  #define BIT_MSK__TPI_AVI_BYTE3__TPI_AVI_BYTE3                                         0xFF

// TPI AVI Data Byte 4 Register
#define REG_ADDR__TPI_AVI_BYTE4                                          (Page_6 | 0x0010)
  // (ReadWrite, Bits 7:0) 
  // Bit[6:0] = VIC 6:0 video format identification code Bit[7] = reserved
  #define BIT_MSK__TPI_AVI_BYTE4__TPI_AVI_BYTE4                                         0xFF

// TPI AVI Data Byte 5 Register
#define REG_ADDR__TPI_AVI_BYTE5                                          (Page_6 | 0x0011)
  // (ReadWrite, Bits 7:0) 
  // Bit[3:0] = PR 3:0 Pixel Repetition Factor Bit[5:4] = CN1:0 Content Type Bit[7:6] = YQ 1:0 YCC Quantiztion Range
  #define BIT_MSK__TPI_AVI_BYTE5__TPI_AVI_BYTE5                                         0xFF

// TPI AVI Data Byte 6 Register
#define REG_ADDR__TPI_AVI_BYTE6                                          (Page_6 | 0x0012)
  // (ReadWrite, Bits 7:0) 
  // EndTopBar Line Number of the End of Top Bar - LSB
  #define BIT_MSK__TPI_AVI_BYTE6__TPI_AVI_BYTE6                                         0xFF

// TPI AVI Data Byte 7 Register
#define REG_ADDR__TPI_AVI_BYTE7                                          (Page_6 | 0x0013)
  // (ReadWrite, Bits 7:0) 
  // EndTopBar Line Number of the End of Top Bar - MSB
  #define BIT_MSK__TPI_AVI_BYTE7__TPI_AVI_BYTE7                                         0xFF

// TPI AVI Data Byte 8 Register
#define REG_ADDR__TPI_AVI_BYTE8                                          (Page_6 | 0x0014)
  // (ReadWrite, Bits 7:0) 
  // StartBottomBar Line Number of start of Bottom Bar - LSB
  #define BIT_MSK__TPI_AVI_BYTE8__TPI_AVI_BYTE8                                         0xFF

// TPI AVI Data Byte 9 Register
#define REG_ADDR__TPI_AVI_BYTE9                                          (Page_6 | 0x0015)
  // (ReadWrite, Bits 7:0) 
  // StartBottomBar Line Number of start of Bottom Bar - MSB
  #define BIT_MSK__TPI_AVI_BYTE9__TPI_AVI_BYTE9                                         0xFF

// TPI AVI Data Byte 10 Register
#define REG_ADDR__TPI_AVI_BYTE10                                         (Page_6 | 0x0016)
  // (ReadWrite, Bits 7:0) 
  // EndLeftBar Pixel Number of End of Left Bar - LSB
  #define BIT_MSK__TPI_AVI_BYTE10__TPI_AVI_BYTE10                                        0xFF

// TPI AVI Data Byte 11 Register
#define REG_ADDR__TPI_AVI_BYTE11                                         (Page_6 | 0x0017)
  // (ReadWrite, Bits 7:0) 
  // EndLeftBar Pixel Number of End of Left Bar - MSB
  #define BIT_MSK__TPI_AVI_BYTE11__TPI_AVI_BYTE11                                        0xFF

// TPI AVI Data Byte 12 Register
#define REG_ADDR__TPI_AVI_BYTE12                                         (Page_6 | 0x0018)
  // (ReadWrite, Bits 7:0) 
  // EndRightBar Pixel Number of End of Right Bar - LSB
  #define BIT_MSK__TPI_AVI_BYTE12__TPI_AVI_BYTE12                                        0xFF

// TPI AVI Data Byte 13 Register
#define REG_ADDR__TPI_AVI_BYTE13                                         (Page_6 | 0x0019)
  // (ReadWrite, Bits 7:0) 
  // EndRightBar Pixel Number of End of Right Bar - MSB Write Triggers Info Frame to be sent (AVI_RPT and AVI_EN; CP_RPT and CP_EN)
  #define BIT_MSK__TPI_AVI_BYTE13__TPI_AVI_BYTE13                                        0xFF

// TPI System Control Register
#define REG_ADDR__TPI_SC                                                 (Page_6 | 0x001A)
  // (ReadWrite, Bits 0) 
  // Output mode Read: 1'b0: DVI 1'b1: HDMI Write (in Hot Plug Service Loop): 1'b0: set DVI 1'b1 set HDMI
  #define BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0                                0x01
  // (ReadWrite, Bits 1) 
  // DDC TPI SW. This is a special bit. FW can write as normal. When read back; it actually reflects the status of (GPU request and TPI grant).
  #define BIT_MSK__TPI_SC__REG_DDC_TPI_SW                                        0x02
  // (ReadWrite, Bits 2) 
  // 0 = Not using DDC 1 = Request to use DDC
  #define BIT_MSK__TPI_SC__REG_DDC_GPU_REQUEST                                   0x04
  // (ReadWrite, Bits 3) 
  // 0 = Normal audio/video 1 = Mute audio/video at receiver
  #define BIT_MSK__TPI_SC__REG_TPI_AV_MUTE                                       0x08
  // (ReadWrite, Bits 5) 
  // See description in reg_tpi_output_mode[0] 0x1A bit 0
  #define BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B1                                0x20
  // (ReadWrite, Bits 6) 
  // Cipher Initialize bit (0x1A[6]) and Link encryption suspended (0x29[5:4]=11) status need to be considered together. Lets say that link is secured and protection level (0x2A[0]) changes from 1 (max security) to 0 (min security) then based on Cipher Initialize bit HW TPI will behave in two different ways. 0 (default) =] Cipher engine is NOT initialized; 0x29[5:4] =11 (Link encryption suspended); 0x29[6] = 0 (Local link not secured); COPP status change interrupt bit (0x3D[5])) is set 1 =] Initialize Cipher engine; 0x29[5:4] = 00 (normal); 0x29[6] = 0 (Local link not secured); COPP status change interrupt bit (0x3D[5])) is set
  #define BIT_MSK__TPI_SC__REG_TPI_REAUTH_CTL                                    0x40
  // (ReadWrite, Bits 7) 
  // 0 = Normal operation (only r/w in 1932; no use) 1 = Jump to Flash Update
  #define BIT_MSK__TPI_SC__REG_TPI_UPDATE_FLG                                    0x80

// TPI Device ID Register
#define REG_ADDR__TPI_DEV_ID                                             (Page_6 | 0x001B)
  // (ReadOnly, Bits 7:0) 
  // TPI Device ID(0xB2)
  #define BIT_MSK__TPI_DEV_ID__REG_TPI_DEV_ID                                        0xFF

// TPI COPP Query Data Register
#define REG_ADDR__TPI_COPP_DATA1                                         (Page_6 | 0x0029)
  // (ReadOnly, Bits 0) 
  // 00  DVI 01  HDMI 10  Unrecognized 11  UDI (Reserved)
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_CONNTYPE_B1                                  0x01
  // (ReadOnly, Bits 1) 
  // 0  None 1  HDCP
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_PROTYPE                                      0x02
  // (ReadOnly, Bits 2) 
  // See description in reg_copp_conntype[1]
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_CONNTYPE_B0                                  0x04
  // (ReadOnly, Bits 3) 
  // 0  No 1  Yes
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP                                     0x08
  // (ReadOnly, Bits 5:4) 
  // 00  Normal 01  Link Lost 10  Renegotiation required 11 - Rsvd If 0x29[5:4] = 11 and 0x2A[0] goes from 0 (min protection) to 1 (max protection) 0x29[5:4] =] 00 (normal); 0x29[6] = 1 (Local link secured); status change interrupt bit (0x3D[5])) is set.
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_LINK_STATUS                                  0x30
  // (ReadOnly, Bits 6) 
  // 0  No link protection 1  Link secure
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT                                        0x40
  // (ReadOnly, Bits 7) 
  // 0  No link protection 1  Link secure
  #define BIT_MSK__TPI_COPP_DATA1__REG_COPP_GPROT                                        0x80

// TPI COPP Control Data Register
#define REG_ADDR__TPI_COPP_DATA2                                         (Page_6 | 0x002A)
  // (ReadWrite, Bits 0) 
  // 0  Min (no protection needed) 1  Max (HDCP required) Link encryption suspended means that link was secured before protection level (0x2A[0]) changed from 1 to 0 Renegotiation required status happens when authentication fails.
  #define BIT_MSK__TPI_COPP_DATA2__REG_COPP_PROTLEVEL                                    0x01
  // (ReadWrite, Bits 1) 
  // DDC Short Ri read enable bit 0: disable (default) 1: enable
  #define BIT_MSK__TPI_COPP_DATA2__REG_DDC_SHORT_RI_RD                                   0x02
  // (ReadWrite, Bits 2) 
  // Double Ri check enable bit 0: disable (default) 1: enable
  #define BIT_MSK__TPI_COPP_DATA2__REG_DOUBLE_RI_CHECK                                   0x04
  // (ReadWrite, Bits 3) 
  // Intermediate Ri check enable bit 0: disable (default) 1: enable
  #define BIT_MSK__TPI_COPP_DATA2__REG_INTERM_RI_CHECK_EN                                0x08
  // (ReadWrite, Bits 4) 
  // KSV forward enable bit 0: disable (default) 1: enable
  #define BIT_MSK__TPI_COPP_DATA2__REG_KSV_FORWARD                                       0x10
  // (ReadWrite, Bits 5) 
  // Interrupt encryption enable bit 0: disable (default) 1: enable
  #define BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION                                   0x20
  // (ReadWrite, Bits 6) 
  // Cancel Protect Request Enable when 0x2A bit 0 goes from 1 to 0 the HW TPI state machine will get reset CONFIG state 0  disabled 1  enabled
  #define BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN                                    0x40
  // (ReadWrite, Bits 7) 
  // TPI HDCP Prep Enable set to enable the TPI HDCP State machine to advance to the Prep state to start discovering if HDCP is available through reading the BCAPs and BKSV.
  #define BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN                                  0x80

// TPI Write BKSV1 Register
#define REG_ADDR__TPI_WR_BKSV_1                                          (Page_6 | 0x002B)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #1 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv1 register located inside HDCP block.  During read get Data of BKSV #1 register. 
  #define BIT_MSK__TPI_WR_BKSV_1__TPI_BKSV0                                             0xFF

// TPI Write BKSV2 Register
#define REG_ADDR__TPI_WR_BKSV_2                                          (Page_6 | 0x002C)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #2 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv2 register located inside HDCP block.  During read get Data of BKSV #2 register. 
  #define BIT_MSK__TPI_WR_BKSV_2__TPI_BKSV1                                             0xFF

// TPI_Write BKSV3 Register
#define REG_ADDR__TPI_WR_BKSV_3                                          (Page_6 | 0x002D)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #3 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv3 register located inside HDCP block.  During read get Data of BKSV #3 register. 
  #define BIT_MSK__TPI_WR_BKSV_3__TPI_BKSV2                                             0xFF

// TPI Write BKSV4 Register
#define REG_ADDR__TPI_WR_BKSV_4                                          (Page_6 | 0x002E)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #4 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv4 register located inside HDCP block.  During read get Data of BKSV #4 register. 
  #define BIT_MSK__TPI_WR_BKSV_4__TPI_BKSV3                                             0xFF

// TPI Write BKSV5 Register
#define REG_ADDR__TPI_WR_BKSV_5                                          (Page_6 | 0x002F)
  // (ReadWrite, Bits 7:0) 
  // Reciever's Key Select Vector. It is read from the reciever and then writen to here. When software writes byte #5 of Key Selector Vector in this register the 1 clock write strobe is generated and data is loaded into the bksv5 register located inside HDCP block. In addition if HDCP is enabled then by writing into this register the authentication is triggered.  During read get Data of BKSV #5 register. 
  #define BIT_MSK__TPI_WR_BKSV_5__TPI_BKSV4                                             0xFF

// TPI HDCP Revision Register
#define REG_ADDR__TPI_HDCP_REV                                           (Page_6 | 0x0030)
  // (ReadOnly, Bits 7:0) 
  // Without HDCP; 8'h00. else with HDCP is 8'h12
  #define BIT_MSK__TPI_HDCP_REV__REG_TPI_HDCP_REV                                      0xFF

// TPI KSV and V' Value Data Register
#define REG_ADDR__TPI_KSV_V                                              (Page_6 | 0x0031)
  // (ReadWrite, Bits 2:0) 
  // V Value Select 000=H0 001=H1 010=H2 011=H3 100=H4 101=Rsvd 11x=Rsvd
  #define BIT_MSK__TPI_KSV_V__REG_TPI_V_SEL                                         0x07
  // (ReadWrite, Bits 3) 
  // 0=No 1=Yes
  #define BIT_MSK__TPI_KSV_V__REG_COPP_VSEL_RDY                                     0x08
  // (ReadOnly, Bits 7:6) 
  // 00=Auth. required 01=Re-auth. required 10=Authenticated 11=Repeater auth. Required
  #define BIT_MSK__TPI_KSV_V__TPI_AUTH_STATE                                        0xC0

// TPI V' Value Byte 0 Register
#define REG_ADDR__TPI_VVALUE_B0                                          (Page_6 | 0x0032)
  // (ReadOnly, Bits 7:0) 
  // Based on reg_tpi_v_sel (0x31 bit[2:0]) 3'b000 = tpi_vp_h0_b0 3'b001 = tpi_vp_h1_b0 3'b010 = tpi_vp_h2_b0 3'b011 = tpi_vp_h3_b0 3'b100 = tpi_vp_h4_b0
  #define BIT_MSK__TPI_VVALUE_B0__TPI_VP_HX_B0                                          0xFF

// TPI V' Value Byte 1 Register
#define REG_ADDR__TPI_VVALUE_B1                                          (Page_6 | 0x0033)
  // (ReadOnly, Bits 7:0) 
  // Based on reg_tpi_v_sel (0x31 bit[2:0]) 3'b000 = tpi_vp_h0_b1 3'b001 = tpi_vp_h1_b1 3'b010 = tpi_vp_h2_b1 3'b011 = tpi_vp_h3_b1 3'b100 = tpi_vp_h4_b1
  #define BIT_MSK__TPI_VVALUE_B1__TPI_VP_HX_B1                                          0xFF

// TPI V' Value Byte 2 Register
#define REG_ADDR__TPI_VVALUE_B2                                          (Page_6 | 0x0034)
  // (ReadOnly, Bits 7:0) 
  // Based on reg_tpi_v_sel (0x31 bit[2:0]) 3'b000 = tpi_vp_h0_b2 3'b001 = tpi_vp_h1_b2 3'b010 = tpi_vp_h2_b2 3'b011 = tpi_vp_h3_b2 3'b100 = tpi_vp_h4_b2
  #define BIT_MSK__TPI_VVALUE_B2__TPI_VP_HX_B2                                          0xFF

// TPI V' Value Byte 3 Register
#define REG_ADDR__TPI_VVALUE_B3                                          (Page_6 | 0x0035)
  // (ReadOnly, Bits 7:0) 
  // Based on reg_tpi_v_sel (0x31 bit[2:0]) 3'b000 = tpi_vp_h0_b3 3'b001 = tpi_vp_h1_b3 3'b010 = tpi_vp_h2_b3 3'b011 = tpi_vp_h3_b3 3'b100 = tpi_vp_h4_b3
  #define BIT_MSK__TPI_VVALUE_B3__TPI_VP_HX_B3                                          0xFF

// TPI AKSV_1 Register
#define REG_ADDR__TPI_AKSV_1                                             (Page_6 | 0x0036)
  // (ReadOnly, Bits 7:0) 
  // Byte #1 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__TPI_AKSV_1__REG_TPI_AKSV0                                         0xFF

// TPI AKSV_2 Register
#define REG_ADDR__TPI_AKSV_2                                             (Page_6 | 0x0037)
  // (ReadOnly, Bits 7:0) 
  // Byte #2 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__TPI_AKSV_2__REG_TPI_AKSV1                                         0xFF

// TPI AKSV_3 Register
#define REG_ADDR__TPI_AKSV_3                                             (Page_6 | 0x0038)
  // (ReadOnly, Bits 7:0) 
  // Byte #3 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__TPI_AKSV_3__REG_TPI_AKSV2                                         0xFF

// TPI AKSV_4 Register
#define REG_ADDR__TPI_AKSV_4                                             (Page_6 | 0x0039)
  // (ReadOnly, Bits 7:0) 
  // Byte #4 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__TPI_AKSV_4__REG_TPI_AKSV3                                         0xFF

// TPI AKSV_5 Register
#define REG_ADDR__TPI_AKSV_5                                             (Page_6 | 0x003A)
  // (ReadOnly, Bits 7:0) 
  // Byte #5 of transmitters key select vector.  Five bytes: 1 is the LSB; 5 is the MSB.  All five bytes should be read from here and then written to the receiver.  Byte 5 should be written last into the receiver; and will trigger the authentication process in the receiver.
  #define BIT_MSK__TPI_AKSV_5__REG_TPI_AKSV4                                         0xFF

// HPD and RSEN status Register
#define REG_ADDR__TPI_HPD_RSEN                                           (Page_6 | 0x003B)
  // (ReadOnly, Bits 0) 
  // HPD
  #define BIT_MSK__TPI_HPD_RSEN__HPD                                                   0x01
  // (ReadOnly, Bits 2:1) 
  // HPD State 0 - Disconnected 1 - Measuring a Connection 2 - Connection
  #define BIT_MSK__TPI_HPD_RSEN__HPD_STATE                                             0x06
  // (ReadOnly, Bits 4) 
  // RSEN
  #define BIT_MSK__TPI_HPD_RSEN__RSEN                                                  0x10
  // (ReadOnly, Bits 6:5) 
  // RSEN State 0 - Disconnected 1 - Measuring a Connection 2 - Connection
  #define BIT_MSK__TPI_HPD_RSEN__RSEN_STATE                                            0x60

// TPI Interrupt Enable Register
#define REG_ADDR__TPI_INTR_EN                                            (Page_6 | 0x003C)
  // (ReadWrite, Bits 0) 
  // Read Rx repeater bit enable 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B0                                  0x01
  // (ReadWrite, Bits 1) 
  // Read BKSV error enable 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B1                                  0x02
  // (ReadWrite, Bits 2) 
  // Read BKSV done enable 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B2                                  0x04
  // (ReadWrite, Bits 3) 
  // KSV FIFO First enable 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B3                                  0x08
  // (ReadWrite, Bits 4) 
  // rsvd
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B4                                  0x10
  // (ReadWrite, Bits 5) 
  // COPP link status change 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B5                                  0x20
  // (ReadWrite, Bits 6) 
  // HDCP V value ready 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B6                                  0x40
  // (ReadWrite, Bits 7) 
  // HDCP authentication status change 0=Disable 1=Enable
  #define BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B7                                  0x80

// TPI Interrupt Status Low Byte Register
#define REG_ADDR__TPI_INTR_ST0                                           (Page_6 | 0x003D)
  // (ReadWrite, Bits 0) 
  // BKSV read error indicator
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST0                                          0x01
  // (ReadWrite, Bits 1) 
  // Either BKSV or BCAPS read error indicator
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST1                                          0x02
  // (ReadWrite, Bits 2) 
  // Both BKSV and BCAPS read done indicator or only BKSV read done indicator. The behavior can controlled by 0x07[0]
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST2                                          0x04
  // (ReadWrite, Bits 3) 
  // KSV FIFO first byte indicator
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST3                                          0x08
  // (ReadWrite, Bits 5) 
  // indicates a status change event in the LinkStatus value so that the host driver can take appropriate action to re-establish the link.
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST5                                          0x20
  // (ReadWrite, Bits 6) 
  // indicates whether the V value computation has completed.
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST6                                          0x40
  // (ReadWrite, Bits 7) 
  // indicates either that the previous authentication request (from a write to the COPP ProtLevel bit) has completed; or that an Ri mismatch has caused authentication to fail.
  #define BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST7                                          0x80

// TPI KSV FIFO Fill Level Status Register
#define REG_ADDR__TPI_KSV_FIFO_STAT                                      (Page_6 | 0x0041)
  // (ReadOnly, Bits 4:0) 
  // The number of KSV FIFO bytes to forward when KSV Forward feature is enabled
  #define BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_BYTES                                        0x1F
  // (ReadOnly, Bits 7) 
  // This bit indicates KSV FIFO last byte arriving
  #define BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_LAST                                         0x80

// TPI KSV FIFO Forward Port Register
#define REG_ADDR__TPI_KSV_FIFO_FORW                                      (Page_6 | 0x0042)
  // (ReadOnly, Bits 7:0) 
  // Internal port for KSV FIFO Forwarding
  #define BIT_MSK__TPI_KSV_FIFO_FORW__REG_KSV_FIFO_OUT                                      0xFF

// TPI DS BCAPS Status Register
#define REG_ADDR__TPI_DS_BCAPS                                           (Page_6 | 0x0044)
  // (ReadOnly, Bits 7:0) 
  // dowstream BCAPS status
  #define BIT_MSK__TPI_DS_BCAPS__REG_DS_BCAPS                                          0xFF

// TPI BStatus1 Register
#define REG_ADDR__TPI_BSTATUS1                                           (Page_6 | 0x0045)
  // (ReadOnly, Bits 6:0) 
  // Device Count
  #define BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT                                        0x7F
  // (ReadOnly, Bits 7) 
  // Device Count Exceeds
  #define BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED                                     0x80

// TPI BStatus2 Register
#define REG_ADDR__TPI_BSTATUS2                                           (Page_6 | 0x0046)
  // (ReadOnly, Bits 2:0) 
  // Device Depth
  #define BIT_MSK__TPI_BSTATUS2__REG_DS_DEPTH                                          0x07
  // (ReadOnly, Bits 3) 
  // Down Stream Cascade Exceeds
  #define BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED                                    0x08
  // (ReadOnly, Bits 4) 
  // downstream HDMI mode 0 = DVI mode 1 = HDMI mode
  #define BIT_MSK__TPI_BSTATUS2__REG_DS_HDMI_MODE                                      0x10
  // (ReadOnly, Bits 7:5) 
  // Downstream BSTATUS
  #define BIT_MSK__TPI_BSTATUS2__REG_DS_BSTATUS                                        0xE0

// TPI Video Mute Low Byte Register
#define REG_ADDR__TPI_VID_MUTE0                                          (Page_6 | 0x004B)
  // (ReadWrite, Bits 7:0) 
  // Video mute data for channel 0. When video mute is enabled; regular video is replaced by mute data.
  #define BIT_MSK__TPI_VID_MUTE0__TPI_VID_MUTE0                                         0xFF

// TPI Video Mute Mid Byte Register
#define REG_ADDR__TPI_VID_MUTE1                                          (Page_6 | 0x004D)
  // (ReadWrite, Bits 7:0) 
  // Video mute data for channel 1. When video mute is enabled; regular video is replaced by mute data.
  #define BIT_MSK__TPI_VID_MUTE1__TPI_VID_MUTE1                                         0xFF

// TPI Video Mute High Byte Register
#define REG_ADDR__TPI_VID_MUTE2                                          (Page_6 | 0x004F)
  // (ReadWrite, Bits 7:0) 
  // Video mute data for channel 2. When video mute is enabled; regular video is replaced by mute data.
  #define BIT_MSK__TPI_VID_MUTE2__TPI_VID_MUTE2                                         0xFF

// TPI HW Debug #1 Register
#define REG_ADDR__TPI_HW_DBG1                                            (Page_6 | 0x0079)
  // (ReadOnly, Bits 0) 
  // Read BKSV successful
  #define BIT_MSK__TPI_HW_DBG1__READ_BKSV_DONE                                        0x01
  // (ReadOnly, Bits 1) 
  // Read RX repeater successful
  #define BIT_MSK__TPI_HW_DBG1__READ_RX_REPEATER_DONE                                 0x02
  // (ReadOnly, Bits 2) 
  // Write AN successful
  #define BIT_MSK__TPI_HW_DBG1__WRITE_AN_DONE                                         0x04
  // (ReadOnly, Bits 3) 
  // Write AKSV successful
  #define BIT_MSK__TPI_HW_DBG1__WRITE_AKSV_DONE                                       0x08
  // (ReadOnly, Bits 4) 
  // Read R0 Prime successful
  #define BIT_MSK__TPI_HW_DBG1__READ_R0_PRIME_DONE                                    0x10
  // (ReadOnly, Bits 5) 
  // Read KSV FIFO ready successful
  #define BIT_MSK__TPI_HW_DBG1__READ_KSV_FIFO_RDY_DONE                                0x20
  // (ReadOnly, Bits 6) 
  // Read Bstatus successful
  #define BIT_MSK__TPI_HW_DBG1__READ_BSTATUS_DONE                                     0x40
  // (ReadOnly, Bits 7) 
  // Read KSV list successful
  #define BIT_MSK__TPI_HW_DBG1__READ_KSV_LIST_DONE                                    0x80

// TPI HW Debug #2 Register
#define REG_ADDR__TPI_HW_DBG2                                            (Page_6 | 0x007A)
  // (ReadOnly, Bits 0) 
  // Read V prime successful
  #define BIT_MSK__TPI_HW_DBG2__READ_V_PRIME_DONE                                     0x01
  // (ReadOnly, Bits 1) 
  // Read RI prime successful
  #define BIT_MSK__TPI_HW_DBG2__READ_RI_PRIME_DONE                                    0x02
  // (ReadOnly, Bits 2) 
  // Read 2nd RI prime successful
  #define BIT_MSK__TPI_HW_DBG2__READ_RI_2ND_DONE                                      0x04

// TPI HW Debug #3 Register
#define REG_ADDR__TPI_HW_DBG3                                            (Page_6 | 0x007B)
  // (ReadOnly, Bits 0) 
  // Read BKSV error
  #define BIT_MSK__TPI_HW_DBG3__READ_BKSV_ERR                                         0x01
  // (ReadOnly, Bits 1) 
  // Read RX repeater error
  #define BIT_MSK__TPI_HW_DBG3__READ_RX_REPEATER_ERR                                  0x02
  // (ReadOnly, Bits 2) 
  // Write AN error
  #define BIT_MSK__TPI_HW_DBG3__WRITE_AN_ERR                                          0x04
  // (ReadOnly, Bits 3) 
  // Write AKSV error
  #define BIT_MSK__TPI_HW_DBG3__WRITE_AKSV_ERR                                        0x08
  // (ReadOnly, Bits 4) 
  // Read R0 Prime error
  #define BIT_MSK__TPI_HW_DBG3__READ_R0_PRIME_ERR                                     0x10
  // (ReadOnly, Bits 5) 
  // Read KSV FIFO ready error
  #define BIT_MSK__TPI_HW_DBG3__READ_KSV_FIFO_RDY_ERR                                 0x20
  // (ReadOnly, Bits 6) 
  // Read Bstatus error
  #define BIT_MSK__TPI_HW_DBG3__READ_BSTATUS_ERR                                      0x40
  // (ReadOnly, Bits 7) 
  // Read KSV list error
  #define BIT_MSK__TPI_HW_DBG3__READ_KSV_LIST_ERR                                     0x80

// TPI HW Debug #4 Register
#define REG_ADDR__TPI_HW_DBG4                                            (Page_6 | 0x007C)
  // (ReadOnly, Bits 0) 
  // Read V prime error
  #define BIT_MSK__TPI_HW_DBG4__READ_V_PRIME_ERR                                      0x01
  // (ReadOnly, Bits 1) 
  // Read RI prime error
  #define BIT_MSK__TPI_HW_DBG4__READ_RI_PRIME_ERR                                     0x02
  // (ReadOnly, Bits 2) 
  // Read 2nd RI prime error
  #define BIT_MSK__TPI_HW_DBG4__READ_RI_2ND_ERR                                       0x04

// TPI HW Debug #5 Register
#define REG_ADDR__TPI_HW_DBG5                                            (Page_6 | 0x007D)
  // (ReadOnly, Bits 3:0) 
  // TPI HW current state
  #define BIT_MSK__TPI_HW_DBG5__TPI_HW_CS                                             0x0F
  // (ReadOnly, Bits 7:4) 
  // TPI HW Repeater Authentication SM
  #define BIT_MSK__TPI_HW_DBG5__TPI_DS_AUTH_CS                                        0xF0

// TPI HW Debug #6 Register
#define REG_ADDR__TPI_HW_DBG6                                            (Page_6 | 0x007E)
  // (ReadOnly, Bits 4:0) 
  // TPI HW RX Authentication SM
  #define BIT_MSK__TPI_HW_DBG6__TPI_RX_AUTH_CS_B4_B0                                  0x1F
  // (ReadOnly, Bits 7:5) 
  // TPI HW Link Integrity SM
  #define BIT_MSK__TPI_HW_DBG6__TPI_LINK_ENC_CS_B2_B0                                 0xE0

// TPI HW Debug #7 Register
#define REG_ADDR__TPI_HW_DBG7                                            (Page_6 | 0x007F)
  // (ReadOnly, Bits 3:0) 
  // TPI HW DDC Master Controller SM
  #define BIT_MSK__TPI_HW_DBG7__TPI_DDCM_CTL_CS_B3_B0                                 0x0F

// TPI HW Debug #8 Register
#define REG_ADDR__TPI_HW_DBG8                                            (Page_6 | 0x0080)
  // (ReadOnly, Bits 7:0) 
  // # of DDC master access byte (LSb) including read and write
  #define BIT_MSK__TPI_HW_DBG8__REG_DDC_HDCP_ACC_NMB_B7_B0                            0xFF

// TPI HW Debug #9 Register
#define REG_ADDR__TPI_HW_DBG9                                            (Page_6 | 0x0081)
  // (ReadOnly, Bits 1:0) 
  // # of DDC master access byte (MSb) including read and write
  #define BIT_MSK__TPI_HW_DBG9__REG_DDC_HDCP_ACC_NMB_B9_B8                            0x03

// TPI HW Optimization Control #0 Register
#define REG_ADDR__TPI_HW_OPT0                                            (Page_6 | 0x00B8)
  // (ReadWrite, Bits 0) 
  // TPI R0 Wait Absolute time Enable 1: use 2MHz clock to generate a ] 100ms pulse; 2MHz + 5% = 2.1MHz; 18'h34968 (18'd215400) = ~102.57ms  0: use # (0x6B9[3:0]) of vsyncs
  #define BIT_MSK__TPI_HW_OPT0__REG_R0_ABSOLUTE                                       0x01
  // (ReadWrite, Bits 1) 
  // HW TPI State Machine Reset (includes DDC Master) 1: Enabled 0: disabled
  #define BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST                                     0x02

// TPI HW Optimization Control #1 Register
#define REG_ADDR__TPI_HW_OPT1                                            (Page_6 | 0x00B9)
  // (ReadWrite, Bits 3:0) 
  // TPI R0 Calculation Time
  #define BIT_MSK__TPI_HW_OPT1__REG_TPI_R0_CALC_TIME_B3_B0                            0x0F
  // (ReadWrite, Bits 6:4) 
  // TPI Autnentication Retry Counter
  #define BIT_MSK__TPI_HW_OPT1__REG_TPI_AUTH_RETRY_CNT_B2_B0                          0x70
  // (ReadWrite, Bits 7) 
  // DDC delay counter 9 bits of MSB bit
  #define BIT_MSK__TPI_HW_OPT1__REG_DDC_DELAY_CNT_B1                                  0x80

// TPI HW Optimization Control #2 Register
#define REG_ADDR__TPI_HW_OPT2                                            (Page_6 | 0x00BA)
  // (ReadWrite, Bits 7:0) 
  // DDC delay counter 9 bits of lower 8-bit
  #define BIT_MSK__TPI_HW_OPT2__REG_DDC_DELAY_CNT_B0                                  0xFF

// TPI HW Optimization Control #3 Register
#define REG_ADDR__TPI_HW_OPT3                                            (Page_6 | 0x00BB)
  // (ReadWrite, Bits 1:0) 
  // TPI DDC Request Level
  #define BIT_MSK__TPI_HW_OPT3__REG_TPI_DDC_REQ_LEVEL                                 0x03
  // (ReadWrite, Bits 2) 
  // TPI DDC Burst Mode
  #define BIT_MSK__TPI_HW_OPT3__REG_TPI_DDC_BURST_MODE                                0x04
  // (ReadWrite, Bits 3) 
  // RI Check Skip
  #define BIT_MSK__TPI_HW_OPT3__REG_RI_CHECK_SKIP                                     0x08
  // (ReadWrite, Bits 6:4) 
  // Legacy ri check
  #define BIT_MSK__TPI_HW_OPT3__REG_LEGACY_TPI_RI_CHECK                               0x70
  // (ReadWrite, Bits 7) 
  // DDC delay counter 9 bits of MSB bit
  #define BIT_MSK__TPI_HW_OPT3__REG_DDC_DEBUG                                         0x80

// TPI Info Frame Select Register
#define REG_ADDR__TPI_INFO_FSEL                                          (Page_6 | 0x00BF)
  // (ReadWrite, Bits 3:0) 
  // InfoFrame Packet Buffer Selection. 0  Buffer 0 (19 bytes) i.e. AVI 1  Buffer 1 (31 bytes) i.e. GAMUT 2  Buffer 2 (14 bytes) i.e. Audio 3  Buffer 3 (31 bytes) i.e. SPD 4  Buffer 4 (31 bytes) i.e. MPEG 5  Buffer 5 (31 bytes) i.e. VSIF 6  Buffer 6 (31 bytes) i.e. GEN1 7  Buffer 7 (31 bytes) i.e. GEN2 8  Buffer 8 (31 bytes) i.e. GEN3 9  Buffer 9 (31 bytes) i.e. GEN4 10  Buffer 10 (31 bytes) i.e. GEN5 11  Buffer 11 (31 bytes) i.e. VSIF1 for compression/decompression 12  Buffer 12 (31 bytes) i.e. VSIF2 for compression/decompression 13  Buffer 13 (31 bytes) i.e. VSIF3 for compression/decompression 14  Buffer 14 (31 bytes) i.e. VSIF4 for compression/decompression Buffers are in priority order of how they will be sent 0; then 1; etc.
  #define BIT_MSK__TPI_INFO_FSEL__REG_TPI_INFO_SEL                                      0x0F

// TPI Info Byte #0 Register
#define REG_ADDR__TPI_INFO_B0                                            (Page_6 | 0x00C0)
  // (ReadWrite, Bits 7:0) 
  // Bit[6:0] = I-F_Type InfoFrame type per CEA-861-E spec Bit[7] = always write as 1
  #define BIT_MSK__TPI_INFO_B0__TPI_INFO_B0                                           0xFF

// TPI Info Byte #1 Register
#define REG_ADDR__TPI_INFO_B1                                            (Page_6 | 0x00C1)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = I-F_Ver InfoFrame version per CEA-861-E spec
  #define BIT_MSK__TPI_INFO_B1__TPI_INFO_B1                                           0xFF

// TPI Info Byte #2 Register
#define REG_ADDR__TPI_INFO_B2                                            (Page_6 | 0x00C2)
  // (ReadWrite, Bits 7:0) 
  // Bit[4:0] = I-F_Length InfoFrame length per CEA-861-E spec Bit[7:5] = rsvd
  #define BIT_MSK__TPI_INFO_B2__TPI_INFO_B2                                           0xFF

// TPI Info Byte #3 Register
#define REG_ADDR__TPI_INFO_B3                                            (Page_6 | 0x00C3)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame checksum
  #define BIT_MSK__TPI_INFO_B3__TPI_INFO_B3                                           0xFF

// TPI Info Byte #4 Register
#define REG_ADDR__TPI_INFO_B4                                            (Page_6 | 0x00C4)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte1
  #define BIT_MSK__TPI_INFO_B4__TPI_INFO_B4                                           0xFF

// TPI Info Byte #5 Register
#define REG_ADDR__TPI_INFO_B5                                            (Page_6 | 0x00C5)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte2
  #define BIT_MSK__TPI_INFO_B5__TPI_INFO_B5                                           0xFF

// TPI Info Byte #6 Register
#define REG_ADDR__TPI_INFO_B6                                            (Page_6 | 0x00C6)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte3
  #define BIT_MSK__TPI_INFO_B6__TPI_INFO_B6                                           0xFF

// TPI Info Byte #7 Register
#define REG_ADDR__TPI_INFO_B7                                            (Page_6 | 0x00C7)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte4
  #define BIT_MSK__TPI_INFO_B7__TPI_INFO_B7                                           0xFF

// TPI Info Byte #8 Register
#define REG_ADDR__TPI_INFO_B8                                            (Page_6 | 0x00C8)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte5
  #define BIT_MSK__TPI_INFO_B8__TPI_INFO_B8                                           0xFF

// TPI Info Byte #9 Register
#define REG_ADDR__TPI_INFO_B9                                            (Page_6 | 0x00C9)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte6
  #define BIT_MSK__TPI_INFO_B9__TPI_INFO_B9                                           0xFF

// TPI Info Byte #10 Register
#define REG_ADDR__TPI_INFO_B10                                           (Page_6 | 0x00CA)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte7
  #define BIT_MSK__TPI_INFO_B10__TPI_INFO_B10                                          0xFF

// TPI Info Byte #11 Register
#define REG_ADDR__TPI_INFO_B11                                           (Page_6 | 0x00CB)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte8
  #define BIT_MSK__TPI_INFO_B11__TPI_INFO_B11                                          0xFF

// TPI Info Byte #12 Register
#define REG_ADDR__TPI_INFO_B12                                           (Page_6 | 0x00CC)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte9
  #define BIT_MSK__TPI_INFO_B12__TPI_INFO_B12                                          0xFF

// TPI Info Byte #13 Register
#define REG_ADDR__TPI_INFO_B13                                           (Page_6 | 0x00CD)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte10 (the last location for Audio InfoFrames)
  #define BIT_MSK__TPI_INFO_B13__TPI_INFO_B13                                          0xFF

// TPI Info Byte #14 Register
#define REG_ADDR__TPI_INFO_B14                                           (Page_6 | 0x00CE)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte11
  #define BIT_MSK__TPI_INFO_B14__TPI_INFO_B14                                          0xFF

// TPI Info Byte #15 Register
#define REG_ADDR__TPI_INFO_B15                                           (Page_6 | 0x00CF)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte12
  #define BIT_MSK__TPI_INFO_B15__TPI_INFO_B15                                          0xFF

// TPI Info Byte #16 Register
#define REG_ADDR__TPI_INFO_B16                                           (Page_6 | 0x00D0)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte13
  #define BIT_MSK__TPI_INFO_B16__TPI_INFO_B16                                          0xFF

// TPI Info Byte #17 Register
#define REG_ADDR__TPI_INFO_B17                                           (Page_6 | 0x00D1)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte14
  #define BIT_MSK__TPI_INFO_B17__TPI_INFO_B17                                          0xFF

// TPI Info Byte #18 Register
#define REG_ADDR__TPI_INFO_B18                                           (Page_6 | 0x00D2)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte15
  #define BIT_MSK__TPI_INFO_B18__TPI_INFO_B18                                          0xFF

// TPI Info Byte #19 Register
#define REG_ADDR__TPI_INFO_B19                                           (Page_6 | 0x00D3)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte16
  #define BIT_MSK__TPI_INFO_B19__TPI_INFO_B19                                          0xFF

// TPI Info Byte #20 Register
#define REG_ADDR__TPI_INFO_B20                                           (Page_6 | 0x00D4)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte17
  #define BIT_MSK__TPI_INFO_B20__TPI_INFO_B20                                          0xFF

// TPI Info Byte #21 Register
#define REG_ADDR__TPI_INFO_B21                                           (Page_6 | 0x00D5)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte18
  #define BIT_MSK__TPI_INFO_B21__TPI_INFO_B21                                          0xFF

// TPI Info Byte #22 Register
#define REG_ADDR__TPI_INFO_B22                                           (Page_6 | 0x00D6)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte19
  #define BIT_MSK__TPI_INFO_B22__TPI_INFO_B22                                          0xFF

// TPI Info Byte #23 Register
#define REG_ADDR__TPI_INFO_B23                                           (Page_6 | 0x00D7)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte20
  #define BIT_MSK__TPI_INFO_B23__TPI_INFO_B23                                          0xFF

// TPI Info Byte #24 Register
#define REG_ADDR__TPI_INFO_B24                                           (Page_6 | 0x00D8)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte21
  #define BIT_MSK__TPI_INFO_B24__TPI_INFO_B24                                          0xFF

// TPI Info Byte #25 Register
#define REG_ADDR__TPI_INFO_B25                                           (Page_6 | 0x00D9)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte22
  #define BIT_MSK__TPI_INFO_B25__TPI_INFO_B25                                          0xFF

// TPI Info Byte #26 Register
#define REG_ADDR__TPI_INFO_B26                                           (Page_6 | 0x00DA)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte23
  #define BIT_MSK__TPI_INFO_B26__TPI_INFO_B26                                          0xFF

// TPI Info Byte #27 Register
#define REG_ADDR__TPI_INFO_B27                                           (Page_6 | 0x00DB)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte24
  #define BIT_MSK__TPI_INFO_B27__TPI_INFO_B27                                          0xFF

// TPI Info Byte #28 Register
#define REG_ADDR__TPI_INFO_B28                                           (Page_6 | 0x00DC)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte25
  #define BIT_MSK__TPI_INFO_B28__TPI_INFO_B28                                          0xFF

// TPI Info Byte #29 Register
#define REG_ADDR__TPI_INFO_B29                                           (Page_6 | 0x00DD)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = InfoFrame data byte26
  #define BIT_MSK__TPI_INFO_B29__TPI_INFO_B29                                          0xFF

// TPI Info Byte #30 Register
#define REG_ADDR__TPI_INFO_B30                                           (Page_6 | 0x00DE)
  // (ReadWrite, Bits 7:0) 
  // Bit[7:0] = Last InfoFrame data byte (for amm except Audio InfoFrames)
  #define BIT_MSK__TPI_INFO_B30__TPI_INFO_B30                                          0xFF

// TPI Info Enable Register
#define REG_ADDR__TPI_INFO_EN                                            (Page_6 | 0x00DF)
  // (ReadOnly, Bits 5) 
  // Selected TPI InfoFrame transmission on HDMI Enabled and Sending next vsync.
  #define BIT_MSK__TPI_INFO_EN__CEA_INFO_EN                                           0x20
  // (ReadWrite, Bits 6) 
  // Repeat selected TPI InfoFrame Packet data each frame. Once this bit is set and transmition bit is enabled hardware would try to send InfoFrame Packet once every vblank period. Software has to disable this bit first to force clear of the transmission enable bit
  #define BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_RPT                                      0x40
  // (ReadWrite, Bits 7) 
  // Enable request of selected TPI InfoFrame transmission on HDMI. To enable transmission write 1 into this bit. Once transmission is done the hardware will reset it back to 0. Have to be enabled only after packet data is written to the registers:
  #define BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_EN                                       0x80

// VSIF Compression Control Register
#define REG_ADDR__VSIF_COMP_CNTL                                         (Page_6 | 0x00E0)
  // (ReadWrite, Bits 0) 
  // 1: send only one enabled VSIF compression packet per frame 0: send all enabled VSIF compression packets per frame (up to 4 packets)
  #define BIT_MSK__VSIF_COMP_CNTL__REG_VSIF_COMP_ONE_PER_FRAME                           0x01

// TPI DDC Master Enable Register
#define REG_ADDR__TPI_DDC_MASTER_EN                                      (Page_6 | 0x00F8)
  // (ReadWrite, Bits 7) 
  // Write 1 to enable DDC master access in HW TPI mode
  #define BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER                                     0x80

//***************************************************************************
// page 37. Address: 40
// CoC 1st Status Register
#define REG_ADDR__COC_STAT_0                                             (Page_7 | 0x0000)
  // (ReadOnly, Bits 5:0) 
  // CoC status 1st byte
  #define BIT_MSK__COC_STAT_0__COC_STATUS0_B5_B0                                     0x3F
  // (ReadOnly, Bits 6) 
  // bit_error 8b/10 error
  #define BIT_MSK__COC_STAT_0__COC_STATUS0_B6                                        0x40
  // (ReadOnly, Bits 7) 
  // lock; PLL lock status
  #define BIT_MSK__COC_STAT_0__COC_STATUS0_B7                                        0x80

// CoC 2nd Status Register
#define REG_ADDR__COC_STAT_1                                             (Page_7 | 0x0001)
  // (ReadOnly, Bits 5:0) 
  // dt_tmax0: Sink data rising edge position
  #define BIT_MSK__COC_STAT_1__COC_STATUS1_B5_B0                                     0x3F
  // (ReadOnly, Bits 7:6) 
  // CoC status 2nd byte
  #define BIT_MSK__COC_STAT_1__COC_STATUS1_B7_B6                                     0xC0

// CoC 3rd Status Register
#define REG_ADDR__COC_STAT_2                                             (Page_7 | 0x0002)
  // (ReadOnly, Bits 5:0) 
  // dt_tmax1: Sink data falling edge position
  #define BIT_MSK__COC_STAT_2__COC_STATUS2_B5_B0                                     0x3F
  // (ReadOnly, Bits 7:6) 
  // CoC status 3rd byte
  #define BIT_MSK__COC_STAT_2__COC_STATUS2_B7_B6                                     0xC0

// CoC 4th Status Register
#define REG_ADDR__COC_STAT_3                                             (Page_7 | 0x0003)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control15[3:0]
  #define BIT_MSK__COC_STAT_3__COC_STATUS3_B7_B0                                     0xFF

// CoC 5th Status Register
#define REG_ADDR__COC_STAT_4                                             (Page_7 | 0x0004)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control[6:4]
  #define BIT_MSK__COC_STAT_4__COC_STATUS4_B7_B0                                     0xFF

// CoC 6th Status Register
#define REG_ADDR__COC_STAT_5                                             (Page_7 | 0x0005)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control[6:4]
  #define BIT_MSK__COC_STAT_5__COC_STATUS5_B7_B0                                     0xFF

// CoC 7th Status Register
#define REG_ADDR__COC_STAT_6                                             (Page_7 | 0x0006)
  // (ReadOnly, Bits 0) 
  // meas_Aout_done
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B0                                        0x01
  // (ReadOnly, Bits 1) 
  // meas_Ain_done
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B1                                        0x02
  // (ReadOnly, Bits 2) 
  // meas_Aoffset_done
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B2                                        0x04
  // (ReadOnly, Bits 3) 
  // received comma1
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B3                                        0x08
  // (ReadOnly, Bits 4) 
  // received comma2
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B4                                        0x10
  // (ReadOnly, Bits 5) 
  // DAC stable1 done
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B5                                        0x20
  // (ReadOnly, Bits 6) 
  // received sink side signal
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B6                                        0x40
  // (ReadOnly, Bits 7) 
  // Calibration done indicator
  #define BIT_MSK__COC_STAT_6__COC_STATUS6_B7                                        0x80

// CoC 8th Status Register
#define REG_ADDR__COC_STAT_7                                             (Page_7 | 0x0007)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control[6:4]
  #define BIT_MSK__COC_STAT_7__COC_STATUS7_B7_B0                                     0xFF

// CoC 9th Status Register
#define REG_ADDR__COC_STAT_8                                             (Page_7 | 0x0008)
  // (ReadOnly, Bits 7:0) 
  // BIST error cnt[7:0]. Please refer to 0x709[3:0]
  #define BIT_MSK__COC_STAT_8__COC_STATUS8_B7_B0                                     0xFF

// CoC 10th Status Register
#define REG_ADDR__COC_STAT_9                                             (Page_7 | 0x0009)
  // (ReadOnly, Bits 3:0) 
  // BIST error cnt[11:8]. Please refer to 0x708[7:0]
  #define BIT_MSK__COC_STAT_9__COC_STATUS9_B3_B0                                     0x0F
  // (ReadOnly, Bits 4) 
  // BIST done indicator
  #define BIT_MSK__COC_STAT_9__COC_STATUS9_B4                                        0x10
  // (ReadOnly, Bits 5) 
  // decode error
  #define BIT_MSK__COC_STAT_9__COC_STATUS9_B5                                        0x20
  // (ReadOnly, Bits 6) 
  // running disparity error
  #define BIT_MSK__COC_STAT_9__COC_STATUS9_B6                                        0x40
  // (ReadOnly, Bits 7) 
  // code violation error
  #define BIT_MSK__COC_STAT_9__COC_STATUS9_B7                                        0x80

// CoC 11th Status Register
#define REG_ADDR__COC_STAT_A                                             (Page_7 | 0x000A)
  // (ReadOnly, Bits 7:0) 
  // RX 8 bit data after 8b/10b decoder
  #define BIT_MSK__COC_STAT_A__COC_STATUSA_B7_B0                                     0xFF

// CoC 12th Status Register
#define REG_ADDR__COC_STAT_B                                             (Page_7 | 0x000B)
  // (ReadOnly, Bits 7:0) 
  // Rx 10 bit data before 8b/10 decoder LSb[7:0]
  #define BIT_MSK__COC_STAT_B__COC_STATUSB_B7_B0                                     0xFF

// CoC 13th Status Register
#define REG_ADDR__COC_STAT_C                                             (Page_7 | 0x000C)
  // (ReadOnly, Bits 1:0) 
  // Rx 10 bit data before 8b/10b decoder MSb[9:8]
  #define BIT_MSK__COC_STAT_C__COC_STATUSC_B1_B0                                     0x03
  // (ReadOnly, Bits 2) 
  // Rx 8 bit K indicator
  #define BIT_MSK__COC_STAT_C__COC_STATUSC_B2                                        0x04
  // (ReadOnly, Bits 7:3) 
  // CoC status 13th byte
  #define BIT_MSK__COC_STAT_C__COC_STATUSC_B7_B3                                     0xF8

// CoC 14th Status Register
#define REG_ADDR__COC_STAT_D                                             (Page_7 | 0x000D)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control[6:4]
  #define BIT_MSK__COC_STAT_D__COC_STATUSD_B7_B0                                     0xFF

// CoC 15th Status Register
#define REG_ADDR__COC_STAT_E                                             (Page_7 | 0x000E)
  // (ReadOnly, Bits 7:0) 
  // CoC internal register read back. Please refer to coc_control[6:4]
  #define BIT_MSK__COC_STAT_E__COC_STATUSE_B7_B0                                     0xFF

// CoC 16th Status Register
#define REG_ADDR__COC_STAT_F                                             (Page_7 | 0x000F)
  // (ReadOnly, Bits 3:0) 
  // state_COC_FSM: calibration FSM current state
  #define BIT_MSK__COC_STAT_F__COC_STATUSF_B3_B0                                     0x0F
  // (ReadOnly, Bits 7:4) 
  // CoC status 16th byte
  #define BIT_MSK__COC_STAT_F__COC_STATUSF_B7_B4                                     0xF0

// CoC 1st Ctl Register
#define REG_ADDR__COC_CTL0                                               (Page_7 | 0x0010)
  // (ReadWrite, Bits 0) 
  // reg_tx_drv_en: manual tx driver enable
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B0                                   0x01
  // (ReadWrite, Bits 1) 
  // dgb_av_sel: debug data to AV channel 1: dtx_av 0: drx_av
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B1                                   0x02
  // (ReadWrite, Bits 2) 
  // rg_cal_en: Enable auto calibration FSM
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B2                                   0x04
  // (ReadWrite, Bits 4:3) 
  // rg_lpf_sel: LPF Select 11: 7 bit average 10: 5 bit average 01: 3 bit average 00: no LPF
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B4_B3                                0x18
  // (ReadWrite, Bits 5) 
  // rg_sel_alpha: Select between 1: I2C alpha value 0: auto calibrated alpha value
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B5                                   0x20
  // (ReadWrite, Bits 6) 
  // rg_sel_cal_auto: Select state value 1: CoC state machine 0: I2C
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B6                                   0x40
  // (ReadWrite, Bits 7) 
  // coc_150M_en: generate 150 MHz clock for CoC
  #define BIT_MSK__COC_CTL0__REG_COC_CONTROL0_B7                                   0x80

// CoC 2nd Ctl Register
#define REG_ADDR__COC_CTL1                                               (Page_7 | 0x0011)
  // (ReadWrite, Bits 5:0) 
  // rg_coc_fe0: Falling edge position for 0. High for rg_coc_fe0*2 cycles
  #define BIT_MSK__COC_CTL1__REG_COC_CONTROL1_B5_B0                                0x3F
  // (ReadWrite, Bits 7:6) 
  // rg_bist_pattern_msb0: 10 bit MSB 2 bit or K char. Bit 7 and 6 for 2 MSb of 10 bit pattern 0; or 6: 8bit pattern K char
  #define BIT_MSK__COC_CTL1__REG_COC_CONTROL1_B7_B6                                0xC0

// CoC 3rd Ctl Register
#define REG_ADDR__COC_CTL2                                               (Page_7 | 0x0012)
  // (ReadWrite, Bits 5:0) 
  // rg_coc_fe1: Falling edge position for 1. high for rg_coc_fe1*2 cycles
  #define BIT_MSK__COC_CTL2__REG_COC_CONTROL2_B5_B0                                0x3F
  // (ReadWrite, Bits 7:6) 
  // rg_bist_pattern_msb0: 10 bit MSB 2 bit or K char. Bit 7 and 6 for 2 MSb of 10 bit pattern 1; or bit 6 for 8bit pattern K char
  #define BIT_MSK__COC_CTL2__REG_COC_CONTROL2_B7_B6                                0xC0

// CoC 4th Ctl Register
#define REG_ADDR__COC_CTL3                                               (Page_7 | 0x0013)
  // (ReadWrite, Bits 6:0) 
  // dac_stable_time: the clock is 600 MHz if [7] = 0; 100ns/1.6ns/4 = 60/4 = 15 if [7] = 1; 160ns/1.6ns/4 = 100/4 = 25
  #define BIT_MSK__COC_CTL3__REG_COC_CONTROL3_B6_B0                                0x7F
  // (ReadWrite, Bits 7) 
  // DAC step adjust; set [6:0] according to DAC stable time 2 steps each time; requires more DAC stable time 1:2 steps each time; need 160ns DAC stable time 0:1 steps each time; need 100ns DAC stable time
  #define BIT_MSK__COC_CTL3__REG_COC_CONTROL3_B7                                   0x80

// CoC 5th Ctl Register
#define REG_ADDR__COC_CTL4                                               (Page_7 | 0x0014)
  // (ReadWrite, Bits 6:0) 
  // rg_ec_delay: Manual echo cancelling delay parameter. Forsrc 0-89
  #define BIT_MSK__COC_CTL4__REG_COC_CONTROL4_B6_B0                                0x7F
  // (ReadWrite, Bits 7) 
  // rg_ec_delay_sel: Echo cancelling delay select 1: fixed manual echo cancelling 0: calibrate echo cancelling value
  #define BIT_MSK__COC_CTL4__REG_COC_CONTROL4_B7                                   0x80

// CoC 6th Ctl Register
#define REG_ADDR__COC_CTL5                                               (Page_7 | 0x0015)
  // (ReadWrite, Bits 7:0) 
  // CoC control register 6th byte
  #define BIT_MSK__COC_CTL5__REG_COC_CONTROL5                                      0xFF

// CoC 7th Ctl Register
#define REG_ADDR__COC_CTL6                                               (Page_7 | 0x0016)
  // (ReadWrite, Bits 5:0) 
  // rg_smpl_position: Manual sampling position. 0 - 39
  #define BIT_MSK__COC_CTL6__REG_COC_CONTROL6_B5_B0                                0x3F
  // (ReadWrite, Bits 6) 
  // rg_boundary_sel: deserializer boundary position select 1: fixed manual deserializer boundary position 0: calirated deserializer boundary position
  #define BIT_MSK__COC_CTL6__REG_COC_CONTROL6_B6                                   0x40
  // (ReadWrite, Bits 7) 
  // rg_smpl_position_sel: Sampling position select 1: fixed manual sampling position 0: calibrated sampling position
  #define BIT_MSK__COC_CTL6__REG_COC_CONTROL6_B7                                   0x80

// CoC 8th Ctl Register
#define REG_ADDR__COC_CTL7                                               (Page_7 | 0x0017)
  // (ReadWrite, Bits 2:0) 
  // rg_tx_pattern_sel: tx pattern selection 100~111: RSVD 110         : 2 bit interface mode 101         : CTS in non-aligned mode 100         : enable CTS 011         : 10 bit fixed pattern 010         : 8 bit fixed pattern 001         : BIST 000         : Normal mode
  #define BIT_MSK__COC_CTL7__REG_COC_CONTROL7_B2_B0                                0x07
  // (ReadWrite, Bits 4:3) 
  // rg_lb_bist_sel: loopback bit 11: near-end analog loopback bist enabled 10: near-end digital loopback bist enabled 01: far-end loopback bist enabled 00: normal mode
  #define BIT_MSK__COC_CTL7__REG_COC_CONTROL7_B4_B3                                0x18
  // (ReadWrite, Bits 5) 
  // rg_clear_bist_err: clear bist error counter
  #define BIT_MSK__COC_CTL7__REG_COC_CONTROL7_5                                    0x20
  // (ReadWrite, Bits 6) 
  // rg_clear_error: clear 8b/10b error status
  #define BIT_MSK__COC_CTL7__REG_COC_CONTROL7_6                                    0x40
  // (ReadWrite, Bits 7) 
  // reg_error_inject: 8b/10b error inject
  #define BIT_MSK__COC_CTL7__REG_COC_CONTROL7_B7                                   0x80

// CoC 9th Ctl Register
#define REG_ADDR__COC_CTL8                                               (Page_7 | 0x0018)
  // (ReadWrite, Bits 7:0) 
  // CoC control register 9th byte
  #define BIT_MSK__COC_CTL8__REG_COC_CONTROL8                                      0xFF

// CoC 10th Ctl Register
#define REG_ADDR__COC_CTL9                                               (Page_7 | 0x0019)
  // (ReadWrite, Bits 7:0) 
  // reg_bist_pattern_lsb0 For 10 bit fixed pattern 0; {reg_coc_control1[7:6]; reg_control9[7:0] For 8 bit fixed pattern; reg_coc_control1[7:6] should be set to 2'b01
  #define BIT_MSK__COC_CTL9__REG_COC_CONTROL9                                      0xFF

// CoC 11th Ctl Register
#define REG_ADDR__COC_CTLA                                               (Page_7 | 0x001A)
  // (ReadWrite, Bits 7:0) 
  // reg_bist_pattern_lsb1 For 10 bit fixed pattern 1. {reg_coc_control2[7:6]; reg_coc_controlA[7:0]} For 8 bit pattern; reg_coc_control2[7:6] should be set to 2'b01
  #define BIT_MSK__COC_CTLA__REG_COC_CONTROLA                                      0xFF

// CoC 12th Ctl Register
#define REG_ADDR__COC_CTLB                                               (Page_7 | 0x001B)
  // (ReadWrite, Bits 7:0) 
  // rg_max_timeout: calibration maximum timeout time 1.66ns * 2^19 * reg_coc_controlB (0.87ms - 221.9ms)
  #define BIT_MSK__COC_CTLB__REG_COC_CONTROLB                                      0xFF

// CoC 13th Ctl Register
#define REG_ADDR__COC_CTLC                                               (Page_7 | 0x001C)
  // (ReadWrite, Bits 7:0) 
  // rg_min_wait: calibration minimum wait time 1.66ns * 2^8 * reg_coc_controlC (423ns - 107.9us)
  #define BIT_MSK__COC_CTLC__REG_COC_CONTROLC                                      0xFF

// CoC 14th Ctl Register
#define REG_ADDR__COC_CTLD                                               (Page_7 | 0x001D)
  // (ReadWrite, Bits 6:0) 
  // rg_ctrl_vth_h: manual vth_h control; if rg_sel_alpha (reg_coc_control0[5]) is set
  #define BIT_MSK__COC_CTLD__REG_COC_CONTROLD_B6_B0                                0x7F
  // (ReadWrite, Bits 7) 
  // Bit order swap 1: Swap 0: No swap
  #define BIT_MSK__COC_CTLD__REG_COC_CONTROLD_B7                                   0x80

// CoC 15th Ctl Register
#define REG_ADDR__COC_CTLE                                               (Page_7 | 0x001E)
  // (ReadWrite, Bits 6:0) 
  // rg_ctrl_vth_l: manual vth_l control; if rg_sel_alpha (reg_coc_control0[5]) is set
  #define BIT_MSK__COC_CTLE__REG_COC_CONTROLE_B6_B0                                0x7F
  // (ReadWrite, Bits 7) 
  // CoC/DoC control register 15th byte
  #define BIT_MSK__COC_CTLE__REG_COC_CONTROLE_B7                                   0x80

// CoC 16th Ctl Register
#define REG_ADDR__COC_CTLF                                               (Page_7 | 0x001F)
  // (ReadWrite, Bits 7:0) 
  // CoC control register 16th byte
  #define BIT_MSK__COC_CTLF__REG_COC_CONTROLF                                      0xFF

// CoC 17th Ctl Register
#define REG_ADDR__COC_CTL10                                              (Page_7 | 0x0020)
  // (ReadWrite, Bits 3:0) 
  // k_char_1t0_l: default comma K28.1
  #define BIT_MSK__COC_CTL10__REG_COC_CONTROL10_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // k_char_1t0_h: default comma K28.5
  #define BIT_MSK__COC_CTL10__REG_COC_CONTROL10_B7_B4                               0xF0

// CoC 18th Ctl Register
#define REG_ADDR__COC_CTL11                                              (Page_7 | 0x0021)
  // (ReadWrite, Bits 3:0) 
  // CoC control register 18th byte
  #define BIT_MSK__COC_CTL11__REG_COC_CONTROL11_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // BIST length f: infinite length 8: 13us     - 100 7: 10^8us - 27'h5F5_E100 6: 10^7us - 27'h098_9680 5: 10^6us - 27'h00F_4240 4: 10^5us - 27'h001_86A0 3: 4s         - 27'h1C9_C380 2: 2s         - 27'h0E4_E1C0 1: 1s         - 27'h072_70E0 0: stop
  #define BIT_MSK__COC_CTL11__REG_COC_CONTROL11_B7_B4                               0xF0

// CoC 19th Ctl Register
#define REG_ADDR__COC_CTL12                                              (Page_7 | 0x0022)
  // (ReadWrite, Bits 5:0) 
  // Reserved
  #define BIT_MSK__COC_CTL12__REG_COC_CONTROL12_B5_B0                               0x3F
  // (ReadWrite, Bits 6) 
  // Enable CTS non-aligned mode
  #define BIT_MSK__COC_CTL12__REG_COC_CONTROL12_B6                                  0x40
  // (ReadWrite, Bits 7) 
  // Reserved
  #define BIT_MSK__COC_CTL12__REG_COC_CONTROL12_B7                                  0x80

// CoC 20th Ctl Register
#define REG_ADDR__COC_CTL13                                              (Page_7 | 0x0023)
  // (ReadWrite, Bits 7:0) 
  // Reserved
  #define BIT_MSK__COC_CTL13__REG_COC_CONTROL13                                     0xFF

// CoC 21st Ctl Register
#define REG_ADDR__COC_CTL14                                              (Page_7 | 0x0024)
  // (ReadWrite, Bits 3:0) 
  // dbg_state_COC_FSM: I2C selected state; if rg_cal_en (reg_coc_control0[2]) is not set and reg_coc_control15[7] is set
  #define BIT_MSK__COC_CTL14__REG_COC_CONTROL14_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // rg_boundary: deserializer boundary position; if rg_boundary_sel (reg_coc_control6[6] ) is set
  #define BIT_MSK__COC_CTL14__REG_COC_CONTROL14_B7_B4                               0xF0

// CoC 22nd Ctl Register
#define REG_ADDR__COC_CTL15                                              (Page_7 | 0x0025)
  // (ReadWrite, Bits 3:0) 
  // Control what can be read out from status3 1111 - 1011: RSVD 1010: phase adjusted (snk side only) 1001: sap; sampling point - [5:0] 0 - 39 1000: cp; comma pointer - [3:0] 0 - 7 0111: redge_ph (snk side only) 0110: ec_delay - [6:0] 0 - 89 0101: clock rising edge position: dt_redet - [5:0] 0 -39 0100: Aoffset[6:0] 0011: Aout[6:0] 0010: Ain[6:0] 0001: ctrl_vth_h[6:0] 0000: ctrl_vth_l[6:0] RSVD 10: RSVD 01: ec_delay 00: clock rising edge position: dt_redet
  #define BIT_MSK__COC_CTL15__REG_COC_CONTROL15_B3_B0                               0x0F
  // (ReadWrite, Bits 6:4) 
  // Bit 7 related mux selection for internal register read byte 1-5. The read values are on coc_status4 ([39:32]); coc_status5 ([31:24]); coc_status7([23;16]); coc_statusD([15:8]); and coc_statusE([7:0]) 111 - 101: RSVD 100: drx 011: drx_pre 010: ne_del_dtx 001: drx_h_40b_1z 000: drx_l_40b_1z
  #define BIT_MSK__COC_CTL15__REG_COC_CONTROL15_B6_B4                               0x70
  // (ReadWrite, Bits 7) 
  // 150 MHz domain read pulse (auto reset)
  #define BIT_MSK__COC_CTL15__REG_COC_CONTROL15_7                                   0x80

// CoC Interrupt Register
#define REG_ADDR__COC_INTR                                               (Page_7 | 0x0026)
  // (ReadWrite, Bits 0) 
  // COC PLL lock status change interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT0                                    0x01
  // (ReadWrite, Bits 1) 
  // COC Calibration done interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT1                                    0x02
  // (ReadWrite, Bits 2) 
  // COC 8b/10b error interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT2                                    0x04
  // (ReadWrite, Bits 3) 
  // COC calibration state machine received Comma1 interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT3                                    0x08
  // (ReadWrite, Bits 4) 
  // COC calibration state machine received Comma2 interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT4                                    0x10
  // (ReadWrite, Bits 5) 
  // COC calibration state machine timeout interrupt
  #define BIT_MSK__COC_INTR__REG_COC_INTR_STAT5                                    0x20

// CoC Interrupt Mask Register
#define REG_ADDR__COC_INTR_MASK                                          (Page_7 | 0x0027)
  // (ReadWrite, Bits 0) 
  // Interrupt mask for COC PLL lock status change interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Interrupt mask for COC Calibration done interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Interrupt mask for COC 8b/10b error interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Interrupt mask for COC calibration state machine received Comma1 interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Interrupt mask for COC calibration state machine received Comma2 interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Interrupt mask for COC calibration state machine time out interrupt
  #define BIT_MSK__COC_INTR_MASK__REG_COC_INTR_MASK5                                    0x20

// CoC Misc 1st Ctl Register
#define REG_ADDR__COC_MISC_CTL0                                          (Page_7 | 0x0028)
  // (ReadWrite, Bits 0) 
  // Enable to override coc output enable generated by internal state machine
  #define BIT_MSK__COC_MISC_CTL0__REG_COC_OE_OVR                                        0x01
  // (ReadWrite, Bits 1) 
  // If bit 0 is enabled; coc output enable
  #define BIT_MSK__COC_MISC_CTL0__REG_COC_OE                                            0x02
  // (ReadWrite, Bits 2) 
  // Enable to bring DoC debug signal into AV Channel
  #define BIT_MSK__COC_MISC_CTL0__REG_COC_DBG_AV_EN                                     0x04
  // (ReadWrite, Bits 4:3) 
  // When bit 2 is set; select what CoC debug data is put into AV Channel 00: {dtx_av[4:0]; drx_av[4:0]} 01: drx_h_40b[4:0] 10: drx_l_40b[4:0] 11: {dtx_av[4:0]; drx_av[4:0]}
  #define BIT_MSK__COC_MISC_CTL0__REG_COC_DBG_AV_SEL                                    0x18
  // (ReadWrite, Bits 7) 
  // Enable to bring DoC debug signal to GPIO pins
  #define BIT_MSK__COC_MISC_CTL0__REG_COC_DBG_GPIO_EN                                   0x80

// CoC 23rd Ctl Register
#define REG_ADDR__COC_CTL16                                              (Page_7 | 0x0029)
  // (ReadWrite, Bits 7:0) 
  // CoC control 23rd byte
  #define BIT_MSK__COC_CTL16__REG_COC_CONTROL16_B7_B0                               0xFF

// CoC 24th Ctl Register
#define REG_ADDR__COC_CTL17                                              (Page_7 | 0x002A)
  // (ReadWrite, Bits 3:0) 
  // MHL3 Spec timing parameter 0
  #define BIT_MSK__COC_CTL17__REG_COC_CONTROL17_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // MHL3 Spec timing parameter 1
  #define BIT_MSK__COC_CTL17__REG_COC_CONTROL17_B7_B4                               0xF0

// CoC 25th Ctl Register
#define REG_ADDR__COC_CTL18                                              (Page_7 | 0x002B)
  // (ReadWrite, Bits 3:0) 
  // MHL3 Spec timing parameter 2
  #define BIT_MSK__COC_CTL18__REG_COC_CONTROL18_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // MHL3 Spec timing parameter 3
  #define BIT_MSK__COC_CTL18__REG_COC_CONTROL18_B7_B4                               0xF0

// CoC 26th Ctl Register
#define REG_ADDR__COC_CTL19                                              (Page_7 | 0x002C)
  // (ReadWrite, Bits 3:0) 
  // MHL3 Spec timing parameter 4
  #define BIT_MSK__COC_CTL19__REG_COC_CONTROL19_B3_B0                               0x0F
  // (ReadWrite, Bits 7:4) 
  // MHL3 Spec timing parameter 5
  #define BIT_MSK__COC_CTL19__REG_COC_CONTROL19_B7_B4                               0xF0

// CoC 27th Ctl Register
#define REG_ADDR__COC_CTL1A                                              (Page_7 | 0x002D)
  // (ReadWrite, Bits 1:0) 
  // rg_rx_pattern_sel: rx BIST pattern selection 11: 10bit fixed rx BIST 10: 8bit fixed rx BIST 01: LSFR rx BIST 00: rx BIST disable
  #define BIT_MSK__COC_CTL1A__REG_COC_CONTROL1A_B1_B0                               0x03
  // (ReadWrite, Bits 7:2) 
  // Reserved
  #define BIT_MSK__COC_CTL1A__REG_COC_CONTROL1A_B7_B2                               0xFC

// CoC 28th Ctl Register
#define REG_ADDR__COC_CTL1B                                              (Page_7 | 0x002E)
  // (ReadWrite, Bits 7:0) 
  // Reserved
  #define BIT_MSK__COC_CTL1B__REG_COC_CONTROL1B                                     0xFF

// DoC 1st Status Register
#define REG_ADDR__DOC_STAT_0                                             (Page_7 | 0x0038)
  // (ReadOnly, Bits 5:0) 
  // clock rising edge position
  #define BIT_MSK__DOC_STAT_0__DOC_STATUS0_B7_B0                                     0x3F
  // (ReadOnly, Bits 6) 
  // 8b/10b error
  #define BIT_MSK__DOC_STAT_0__DOC_STATUS0_B6                                        0x40
  // (ReadOnly, Bits 7) 
  // pll lock state
  #define BIT_MSK__DOC_STAT_0__DOC_STATUS0_B7                                        0x80

// DoC 2nd Status Register
#define REG_ADDR__DOC_STAT_1                                             (Page_7 | 0x0039)
  // (ReadOnly, Bits 7:0) 
  // DOC status 2nd byte
  #define BIT_MSK__DOC_STAT_1__DOC_STATUS1_B7_B0                                     0xFF

// DoC 3rd Status Register
#define REG_ADDR__DOC_STAT_2                                             (Page_7 | 0x003A)
  // (ReadOnly, Bits 7:0) 
  // DOC status 3rd byte
  #define BIT_MSK__DOC_STAT_2__DOC_STATUS2_B7_B0                                     0xFF

// DoC 4th Status Register
#define REG_ADDR__DOC_STAT_3                                             (Page_7 | 0x003B)
  // (ReadOnly, Bits 7:0) 
  // DOC status 4th byte
  #define BIT_MSK__DOC_STAT_3__DOC_STATUS3_B7_B0                                     0xFF

// DoC 5th Status Register
#define REG_ADDR__DOC_STAT_4                                             (Page_7 | 0x003C)
  // (ReadOnly, Bits 7:0) 
  // DOC status 5th byte
  #define BIT_MSK__DOC_STAT_4__DOC_STATUS4_B7_B0                                     0xFF

// DoC 6th Status Register
#define REG_ADDR__DOC_STAT_5                                             (Page_7 | 0x003D)
  // (ReadOnly, Bits 7:0) 
  // DOC status 6th byte
  #define BIT_MSK__DOC_STAT_5__DOC_STATUS5_B7_B0                                     0xFF

// DoC 7th Status Register
#define REG_ADDR__DOC_STAT_6                                             (Page_7 | 0x003E)
  // (ReadOnly, Bits 0) 
  // meas_Aout_done
  #define BIT_MSK__DOC_STAT_6__DOC_STATUS6_B0                                        0x01
  // (ReadOnly, Bits 1) 
  // meas_Ain_done
  #define BIT_MSK__DOC_STAT_6__DOC_STATUS6_B1                                        0x02
  // (ReadOnly, Bits 2) 
  // received comma1
  #define BIT_MSK__DOC_STAT_6__DOC_STATUS6_B2                                        0x04
  // (ReadOnly, Bits 3) 
  // received comma2
  #define BIT_MSK__DOC_STAT_6__DOC_STATUS6_B3                                        0x08
  // (ReadOnly, Bits 7:4) 
  // DOC status 7th byte
  #define BIT_MSK__DOC_STAT_6__DOC_STATUS6_B7_B4                                     0xF0

// DoC 8th Status Register
#define REG_ADDR__DOC_STAT_7                                             (Page_7 | 0x003F)
  // (ReadOnly, Bits 7:0) 
  // DOC status 8th byte
  #define BIT_MSK__DOC_STAT_7__DOC_STATUS7_B7_B0                                     0xFF

// DoC 9th Status Register
#define REG_ADDR__DOC_STAT_8                                             (Page_7 | 0x0040)
  // (ReadOnly, Bits 7:0) 
  // BIST error cnt[7:0]
  #define BIT_MSK__DOC_STAT_8__DOC_STATUS8_B7_B0                                     0xFF

// DoC 10th Status Register
#define REG_ADDR__DOC_STAT_9                                             (Page_7 | 0x0041)
  // (ReadOnly, Bits 3:0) 
  // BIST error cnt[15:11]
  #define BIT_MSK__DOC_STAT_9__DOC_STATUS9_B3_B0                                     0x0F
  // (ReadOnly, Bits 4) 
  // BIST error saturate status
  #define BIT_MSK__DOC_STAT_9__DOC_STATUS9_B4                                        0x10
  // (ReadOnly, Bits 5) 
  // decode error
  #define BIT_MSK__DOC_STAT_9__DOC_STATUS9_B5                                        0x20
  // (ReadOnly, Bits 6) 
  // running disparity error
  #define BIT_MSK__DOC_STAT_9__DOC_STATUS9_B6                                        0x40
  // (ReadOnly, Bits 7) 
  // code violation error
  #define BIT_MSK__DOC_STAT_9__DOC_STATUS9_B7                                        0x80

// DoC 11th Status Register
#define REG_ADDR__DOC_STAT_A                                             (Page_7 | 0x0042)
  // (ReadOnly, Bits 7:0) 
  // Rx 8 bit data after 8b/10 decoder
  #define BIT_MSK__DOC_STAT_A__DOC_STATUSA_B7_B0                                     0xFF

// DoC 12th Status Register
#define REG_ADDR__DOC_STAT_B                                             (Page_7 | 0x0043)
  // (ReadOnly, Bits 7:0) 
  // Rx 10 bit pattern [7:0] before 8b/10b decoder
  #define BIT_MSK__DOC_STAT_B__DOC_STATUSB_B7_B0                                     0xFF

// DoC 13th Status Register
#define REG_ADDR__DOC_STAT_C                                             (Page_7 | 0x0044)
  // (ReadOnly, Bits 1:0) 
  // Rx 10 bit data [9:8] before 8b/10 decoder
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B1_B0                                     0x03
  // (ReadOnly, Bits 2) 
  // Rx 8 bit K
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B2                                        0x04
  // (ReadOnly, Bits 3) 
  // Rx 8b/10b decode error
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B3                                        0x08
  // (ReadOnly, Bits 4) 
  // Rx 8b/10b rd error
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B4                                        0x10
  // (ReadOnly, Bits 5) 
  // Rx 8b/10b code error
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B5                                        0x20
  // (ReadOnly, Bits 7:6) 
  // DOC status 14th byte
  #define BIT_MSK__DOC_STAT_C__DOC_STATUSC_B7_B6                                     0xC0

// DoC 14th Status Register
#define REG_ADDR__DOC_STAT_D                                             (Page_7 | 0x0045)
  // (ReadOnly, Bits 7:0) 
  // DOC status 14th byte
  #define BIT_MSK__DOC_STAT_D__DOC_STATUSD_B7_B0                                     0xFF

// DoC 15th Status Register
#define REG_ADDR__DOC_STAT_E                                             (Page_7 | 0x0046)
  // (ReadOnly, Bits 7:0) 
  // DOC status 15th byte
  #define BIT_MSK__DOC_STAT_E__DOC_STATUSE_B7_B0                                     0xFF

// DoC 16th Status Register
#define REG_ADDR__DOC_STAT_F                                             (Page_7 | 0x0047)
  // (ReadOnly, Bits 7:0) 
  // Doc state
  #define BIT_MSK__DOC_STAT_F__DOC_STATUSF_B7_B0                                     0xFF

// DoC FSM Status Register
#define REG_ADDR__DOC_FSM                                                (Page_7 | 0x0049)
  // (ReadOnly, Bits 7:4) 
  // DoC state machine state
  #define BIT_MSK__DOC_FSM__STATE_DOC_FSM_B3_B0                                   0xF0

// DoC 1st CFG Register
#define REG_ADDR__DOC_CFG0                                               (Page_7 | 0x004A)
  // (ReadWrite, Bits 3:0) 
  // comma K28.5
  #define BIT_MSK__DOC_CFG0__REG_K_CHAR_1T0_B3_B0                                  0x0F
  // (ReadWrite, Bits 7:4) 
  // comma K28.1
  #define BIT_MSK__DOC_CFG0__REG_K_CHAR_1T0_B7_B4                                  0xF0

// DoC 2nd CFG Register
#define REG_ADDR__DOC_CFG1                                               (Page_7 | 0x004B)
  // (ReadWrite, Bits 3:0) 
  // comma K28.2
  #define BIT_MSK__DOC_CFG1__REG_K_CHAR_3T2_B3_B0                                  0x0F
  // (ReadWrite, Bits 7:4) 
  // comma K28.3
  #define BIT_MSK__DOC_CFG1__REG_K_CHAR_3T2_B7_B4                                  0xF0

// DoC 5th CFG Register
#define REG_ADDR__DOC_CFG4                                               (Page_7 | 0x004E)
  // (ReadWrite, Bits 3:0) 
  // DoC state machine state to be forced
  #define BIT_MSK__DOC_CFG4__REG_DBG_STATE_DOC_FSM                                 0x0F

// DoC 1st Ctl Register
#define REG_ADDR__DOC_CTL0                                               (Page_7 | 0x0051)
  // (ReadWrite, Bits 0) 
  // Generate 150 MHz clock for Doc
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable debug data to AV channel 1: 150 MHz clock 0: drx_h
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B1                                   0x02
  // (ReadWrite, Bits 2) 
  // Enable auto calibration: rg_cal_en
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B2                                   0x04
  // (ReadWrite, Bits 3) 
  // Enable debug data to AV channel 1: dem_rs 0: drx_l
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B3                                   0x08
  // (ReadWrite, Bits 4) 
  // Enable debug data to AV channel 1: dtx 0: drx
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B4                                   0x10
  // (ReadWrite, Bits 5) 
  // Select I2C alpha value (1) or auto calibration alpha (0)
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B5                                   0x20
  // (ReadWrite, Bits 6) 
  // Select state value 1: DoC state machine 0: I2C
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B6                                   0x40
  // (ReadWrite, Bits 7) 
  // Enable LPF
  #define BIT_MSK__DOC_CTL0__REG_DOC_CONTROL0_B7                                   0x80

// DoC 2nd Ctl Register
#define REG_ADDR__DOC_CTL1                                               (Page_7 | 0x0052)
  // (ReadWrite, Bits 0) 
  // DoC tx driver enable in manual mode
  #define BIT_MSK__DOC_CTL1__REG_DOC_CONTROL1_B0                                   0x01
  // (ReadWrite, Bits 1) 
  // Enable reflection noise cancelling
  #define BIT_MSK__DOC_CTL1__REG_DOC_CONTROL1_B1                                   0x02
  // (ReadWrite, Bits 4:2) 
  // manual offset for ck_edge_sel; tx_drv_en
  #define BIT_MSK__DOC_CTL1__REG_DOC_CONTROL1_B4_B2                                0x1C
  // (ReadWrite, Bits 7:5) 
  // manual offset for em_en
  #define BIT_MSK__DOC_CTL1__REG_DOC_CONTROL1_B7_B5                                0xE0

// DoC 3rd Ctl Register
#define REG_ADDR__DOC_CTL2                                               (Page_7 | 0x0053)
  // (ReadWrite, Bits 7:0) 
  // DAC wait time
  #define BIT_MSK__DOC_CTL2__REG_DOC_CONTROL2                                      0xFF

// DoC 4th Ctl Register
#define REG_ADDR__DOC_CTL3                                               (Page_7 | 0x0054)
  // (ReadWrite, Bits 6:0) 
  // manual echo cancelling delay parameter
  #define BIT_MSK__DOC_CTL3__REG_DOC_CONTROL3_B6_B0                                0x7F
  // (ReadWrite, Bits 7) 
  // 1: fixed manual echo cancelling 0: calibrate echo cancelling value
  #define BIT_MSK__DOC_CTL3__REG_DOC_CONTROL3_B7                                   0x80

// DoC 5th Ctl Register
#define REG_ADDR__DOC_CTL4                                               (Page_7 | 0x0055)
  // (ReadWrite, Bits 7:0) 
  // DoC control register 5th byte
  #define BIT_MSK__DOC_CTL4__REG_DOC_CONTROL4_B7_B0                                0xFF

// DoC 6th Ctl Register
#define REG_ADDR__DOC_CTL5                                               (Page_7 | 0x0056)
  // (ReadWrite, Bits 1:0) 
  // manual sampling position (0-3) 4x oversampling
  #define BIT_MSK__DOC_CTL5__REG_DOC_CONTROL5_B1_B0                                0x03
  // (ReadWrite, Bits 5:2) 
  // manual deserialization boundary position
  #define BIT_MSK__DOC_CTL5__REG_DOC_CONTROL5_B5_B2                                0x3C
  // (ReadWrite, Bits 7:6) 
  // DOC control register 6th byte
  #define BIT_MSK__DOC_CTL5__REG_DOC_CONTROL5_B7_B6                                0xC0

// DoC 7th Ctl Register
#define REG_ADDR__DOC_CTL6                                               (Page_7 | 0x0057)
  // (ReadWrite, Bits 3:0) 
  // mux selection
  #define BIT_MSK__DOC_CTL6__REG_DOC_CONTROL6_B3_B0                                0x0F
  // (ReadWrite, Bits 6:4) 
  // DOC control register 7th byte
  #define BIT_MSK__DOC_CTL6__REG_DOC_CONTROL6_B6_B4                                0x70
  // (ReadWrite, Bits 7) 
  // 600 MHz domain read pulse (auto reset)
  #define BIT_MSK__DOC_CTL6__REG_DOC_CONTROL6_7                                    0x80

// DoC 8th Ctl Register
#define REG_ADDR__DOC_CTL7                                               (Page_7 | 0x0058)
  // (ReadWrite, Bits 2:0) 
  // Tx pattern selection 000: Normal mode 001: BIST 010: 8 bit fixed pattern 011: 10 bit fixed pattern 100: enable CTS 101: RSVD 110: RSVD 111: RSVD
  #define BIT_MSK__DOC_CTL7__REG_DOC_CONTROL7_B2_B0                                0x07
  // (ReadWrite, Bits 4:3) 
  // loopback  bist 00: Normal mode 01: far-end loopback bist en 10: near-end loopback bist en 11: near-end analog loopback bist en
  #define BIT_MSK__DOC_CTL7__REG_DOC_CONTROL7_B4_B3                                0x18
  // (ReadWrite, Bits 5) 
  // clear bist error counter (auto reset)
  #define BIT_MSK__DOC_CTL7__REG_DOC_CONTROL7_5                                    0x20
  // (ReadWrite, Bits 6) 
  // clear 8b/10b error status (auto reset)
  #define BIT_MSK__DOC_CTL7__REG_DOC_CONTROL7_6                                    0x40
  // (ReadWrite, Bits 7) 
  // 8b/10b error inject
  #define BIT_MSK__DOC_CTL7__REG_DOC_CONTROL7_B7                                   0x80

// DoC 9th Ctl Register
#define REG_ADDR__DOC_CTL8                                               (Page_7 | 0x006C)
  // (ReadWrite, Bits 1:0) 
  // 10bit fixed pattern 0 MSB; or 0: K char
  #define BIT_MSK__DOC_CTL8__REG_DOC_CONTROL8_B1_B0                                0x03
  // (ReadWrite, Bits 3:2) 
  // 10bit fixed pattern 1 MSB; or 2: K char
  #define BIT_MSK__DOC_CTL8__REG_DOC_CONTROL8_B3_B2                                0x0C
  // (ReadWrite, Bits 7:4) 
  // DOC control register 9th byte
  #define BIT_MSK__DOC_CTL8__REG_DOC_CONTROL8_B7_B4                                0xF0

// DoC 10th Ctl Register
#define REG_ADDR__DOC_CTL9                                               (Page_7 | 0x006D)
  // (ReadWrite, Bits 7:0) 
  // 8/10 bit fixed pattern 0 (for 10 bit combined with reg_doc_control8[1:0])
  #define BIT_MSK__DOC_CTL9__REG_DOC_CONTROL9                                      0xFF

// DoC 11th Ctl Register
#define REG_ADDR__DOC_CTLA                                               (Page_7 | 0x006E)
  // (ReadWrite, Bits 7:0) 
  // 8/10 bit fixed pattern 1 (for 10 bit combined with reg_doc_control8[3:2])
  #define BIT_MSK__DOC_CTLA__REG_DOC_CONTROLA                                      0xFF

// DoC 12th Ctl Register
#define REG_ADDR__DOC_CTLB                                               (Page_7 | 0x006F)
  // (ReadWrite, Bits 7:0) 
  // Calibration maximum timeout time
  #define BIT_MSK__DOC_CTLB__REG_DOC_CONTROLB                                      0xFF

// DoC 13th Ctl Register
#define REG_ADDR__DOC_CTLC                                               (Page_7 | 0x0070)
  // (ReadWrite, Bits 7:0) 
  // Calibration minimum wait time
  #define BIT_MSK__DOC_CTLC__REG_DOC_CONTROLC                                      0xFF

// DoC 14th Ctl Register
#define REG_ADDR__DOC_CTLD                                               (Page_7 | 0x0071)
  // (ReadWrite, Bits 3:0) 
  // minimum number of ones in measure Aout state
  #define BIT_MSK__DOC_CTLD__REG_DOC_CONTROLD_B3_B0                                0x0F
  // (ReadWrite, Bits 6:4) 
  // Phase tracking LPF
  #define BIT_MSK__DOC_CTLD__REG_DOC_CONTROLD_B6_B4                                0x70
  // (ReadWrite, Bits 7) 
  // Stop DoC BIST
  #define BIT_MSK__DOC_CTLD__REG_DOC_CONTROLD_B7                                   0x80

// DoC 15th Ctl Register
#define REG_ADDR__DOC_CTLE                                               (Page_7 | 0x0072)
  // (ReadWrite, Bits 7:0) 
  // DOC/DoC control register 15th byte
  #define BIT_MSK__DOC_CTLE__REG_DOC_CONTROLE                                      0xFF

// DoC 16th Ctl Register
#define REG_ADDR__DOC_CTLF                                               (Page_7 | 0x0073)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 16th byte
  #define BIT_MSK__DOC_CTLF__REG_DOC_CONTROLF                                      0xFF

// DoC Interrupt Register
#define REG_ADDR__DOC_INTR                                               (Page_7 | 0x0074)
  // (ReadWrite, Bits 0) 
  // DOC PLL lock status change interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT0                                    0x01
  // (ReadWrite, Bits 1) 
  // DOC Calibration done interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT1                                    0x02
  // (ReadWrite, Bits 2) 
  // DOC 8b/10b error interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT2                                    0x04
  // (ReadWrite, Bits 3) 
  // DOC calibration state machine received Comma1 interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT3                                    0x08
  // (ReadWrite, Bits 4) 
  // DOC calibration state machine received Comma2 interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT4                                    0x10
  // (ReadWrite, Bits 5) 
  // DOC trap interrupt (including state change; Comma1 received; Comma2 received; meaure Aout done; detect Phout)
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT5                                    0x20
  // (ReadWrite, Bits 6) 
  // DOC time out interrupt
  #define BIT_MSK__DOC_INTR__REG_DOC_INTR_STAT6                                    0x40

// DoC Interrupt Register
#define REG_ADDR__DOC_INTR_MASK                                          (Page_7 | 0x0075)
  // (ReadWrite, Bits 0) 
  // Interrupt mask for DOC PLL lock status change interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK0                                    0x01
  // (ReadWrite, Bits 1) 
  // Interrupt mask for DOC Calibration done interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK1                                    0x02
  // (ReadWrite, Bits 2) 
  // Interrupt mask for DOC 8b/10b error interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK2                                    0x04
  // (ReadWrite, Bits 3) 
  // Interrupt mask for DOC calibration state machine received Comma1 interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK3                                    0x08
  // (ReadWrite, Bits 4) 
  // Interrupt mask for DOC calibration state machine received Comma2 interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK4                                    0x10
  // (ReadWrite, Bits 5) 
  // Interrupt mask for DOC trap interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK5                                    0x20
  // (ReadWrite, Bits 6) 
  // Interrupt mask for DOC time out interrupt
  #define BIT_MSK__DOC_INTR_MASK__REG_DOC_INTR_MASK6                                    0x40

// DoC Misc 1st Ctl Register
#define REG_ADDR__DOC_MISC_CTL0                                          (Page_7 | 0x0076)
  // (ReadWrite, Bits 0) 
  // Enable to override doc output enable generated by internal state machine
  #define BIT_MSK__DOC_MISC_CTL0__REG_DOC_OE_OVR                                        0x01
  // (ReadWrite, Bits 1) 
  // If bit 0 is enabled; doc output enable
  #define BIT_MSK__DOC_MISC_CTL0__REG_DOC_OE                                            0x02
  // (ReadWrite, Bits 2) 
  // Enable to bring DoC debug signal into AV Channel
  #define BIT_MSK__DOC_MISC_CTL0__REG_DOC_DBG_AV_EN                                     0x04
  // (ReadWrite, Bits 4:3) 
  // When bit 2 is set; select what CoC debug data is put into AV Channel 00: doc_r_2_av 01: doc_t_2_av 10: doc_i_2_av 11: doc_i_2_av
  #define BIT_MSK__DOC_MISC_CTL0__REG_DOC_DBG_AV_SEL                                    0x18
  // (ReadWrite, Bits 7) 
  // Enable to override doc output enable generated by internal state machine
  #define BIT_MSK__DOC_MISC_CTL0__REG_DOC_DBG_GPIO_EN                                   0x80

// DoC Misc 3rd Ctl Register
#define REG_ADDR__DOC_MISC_CTL2                                          (Page_7 | 0x0078)
  // (ReadWrite, Bits 7:0) 
  // TBD
  #define BIT_MSK__DOC_MISC_CTL2__REG_DOC_DM_SWING                                      0xFF

// DoC Misc 4th Ctl Register
#define REG_ADDR__DOC_MISC_CTL3                                          (Page_7 | 0x0079)
  // (ReadWrite, Bits 7:0) 
  // TBD
  #define BIT_MSK__DOC_MISC_CTL3__REG_DOC_SLEW_B7_B0                                    0xFF

// DoC Misc 5th Ctl Register
#define REG_ADDR__DOC_MISC_CTL4                                          (Page_7 | 0x007A)
  // (ReadWrite, Bits 7:0) 
  // TBD
  #define BIT_MSK__DOC_MISC_CTL4__REG_DOC_SLEW_B15_B8                                   0xFF

// DoC Misc 6th Ctl Register
#define REG_ADDR__DOC_MISC_CTL5                                          (Page_7 | 0x007B)
  // (ReadWrite, Bits 7:0) 
  // TBD
  #define BIT_MSK__DOC_MISC_CTL5__REG_DOC_SLEW_B23_B16                                  0xFF

// DoC 17th Ctl Register
#define REG_ADDR__DOC_CTL10                                              (Page_7 | 0x0080)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 17th byte
  #define BIT_MSK__DOC_CTL10__REG_DOC_CONTROL10                                     0xFF

// DoC 18th Ctl Register
#define REG_ADDR__DOC_CTL11                                              (Page_7 | 0x0081)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 18th byte
  #define BIT_MSK__DOC_CTL11__REG_DOC_CONTROL11                                     0xFF

// DoC 19th Ctl Register
#define REG_ADDR__DOC_CTL12                                              (Page_7 | 0x0082)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 19th byte
  #define BIT_MSK__DOC_CTL12__REG_DOC_CONTROL12                                     0xFF

// DoC 20th Ctl Register
#define REG_ADDR__DOC_CTL13                                              (Page_7 | 0x0083)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 20th byte
  #define BIT_MSK__DOC_CTL13__REG_DOC_CONTROL13                                     0xFF

// DoC 21st Ctl Register
#define REG_ADDR__DOC_CTL14                                              (Page_7 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 21st byte
  #define BIT_MSK__DOC_CTL14__REG_DOC_CONTROL14                                     0xFF

// DoC 22nd Ctl Register
#define REG_ADDR__DOC_CTL15                                              (Page_7 | 0x0085)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 22nd byte
  #define BIT_MSK__DOC_CTL15__REG_DOC_CONTROL15                                     0xFF

// DoC 23rd Ctl Register
#define REG_ADDR__DOC_CTL16                                              (Page_7 | 0x0086)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 23rd byte
  #define BIT_MSK__DOC_CTL16__REG_DOC_CONTROL16                                     0xFF

// DoC 24th Ctl Register
#define REG_ADDR__DOC_CTL17                                              (Page_7 | 0x0087)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 24th byte
  #define BIT_MSK__DOC_CTL17__REG_DOC_CONTROL17                                     0xFF

// DoC 25th Ctl Register
#define REG_ADDR__DOC_CTL18                                              (Page_7 | 0x0088)
  // (ReadWrite, Bits 7:0) 
  // DOC control register 25th byte
  #define BIT_MSK__DOC_CTL18__REG_DOC_CONTROL18                                     0xFF

// DoC 11th Status Register
#define REG_ADDR__DOC_STAT_10                                            (Page_7 | 0x0090)
  // (ReadOnly, Bits 7:0) 
  // DOC status 11th byte
  #define BIT_MSK__DOC_STAT_10__DOC_STATUS10_B7_B0                                    0xFF

// DoC 12th Status Register
#define REG_ADDR__DOC_STAT_11                                            (Page_7 | 0x0091)
  // (ReadOnly, Bits 7:0) 
  // DOC status 12th byte
  #define BIT_MSK__DOC_STAT_11__DOC_STATUS11_B7_B0                                    0xFF

// DoC 13th Status Register
#define REG_ADDR__DOC_STAT_12                                            (Page_7 | 0x0092)
  // (ReadOnly, Bits 7:0) 
  // DOC status 13th byte
  #define BIT_MSK__DOC_STAT_12__DOC_STATUS12_B7_B0                                    0xFF

// DoC 14th Status Register
#define REG_ADDR__DOC_STAT_13                                            (Page_7 | 0x0093)
  // (ReadOnly, Bits 7:0) 
  // DOC status 14th byte
  #define BIT_MSK__DOC_STAT_13__DOC_STATUS13_B7_B0                                    0xFF

// DoC 15th Status Register
#define REG_ADDR__DOC_STAT_14                                            (Page_7 | 0x0094)
  // (ReadOnly, Bits 7:0) 
  // DOC status 15th byte
  #define BIT_MSK__DOC_STAT_14__DOC_STATUS14_B7_B0                                    0xFF

// DoC 16th Status Register
#define REG_ADDR__DOC_STAT_15                                            (Page_7 | 0x0095)
  // (ReadOnly, Bits 7:0) 
  // DOC status 16th byte
  #define BIT_MSK__DOC_STAT_15__DOC_STATUS15_B7_B0                                    0xFF

// DoC 17th Status Register
#define REG_ADDR__DOC_STAT_16                                            (Page_7 | 0x0096)
  // (ReadOnly, Bits 7:0) 
  // DOC status 17th byte
  #define BIT_MSK__DOC_STAT_16__DOC_STATUS16_B7_B0                                    0xFF

// DoC 18th Status Register
#define REG_ADDR__DOC_STAT_17                                            (Page_7 | 0x0097)
  // (ReadOnly, Bits 7:0) 
  // DOC status 18th byte
  #define BIT_MSK__DOC_STAT_17__DOC_STATUS17_B7_B0                                    0xFF

// DoC 19th Status Register
#define REG_ADDR__DOC_STAT_18                                            (Page_7 | 0x0098)
  // (ReadOnly, Bits 7:0) 
  // DOC status 19th byte
  #define BIT_MSK__DOC_STAT_18__DOC_STATUS18_B7_B0                                    0xFF

// DoC 19th Status Register
#define REG_ADDR__DOC_STAT_19                                            (Page_7 | 0x0099)
  // (ReadOnly, Bits 7:0) 
  // DOC status 20th byte
  #define BIT_MSK__DOC_STAT_19__DOC_STATUS19_B7_B0                                    0xFF

// DoC 1st Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ0_L                                              (Page_7 | 0x009A)
  // (ReadWrite, Bits 7:0) 
  // DoC 1st Equalization LSB
  #define BIT_MSK__DOC_EQ0_L__REG_DOC_EQ0_B7_B0                                     0xFF

// DoC 1st Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ0_M                                              (Page_7 | 0x009B)
  // (ReadWrite, Bits 7:0) 
  // DoC 1st Equalization MSB
  #define BIT_MSK__DOC_EQ0_M__REG_DOC_EQ0_B15_B8                                    0xFF

// DoC 2nd Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ1_L                                              (Page_7 | 0x009C)
  // (ReadWrite, Bits 7:0) 
  // DoC 2nd Equalization LSB
  #define BIT_MSK__DOC_EQ1_L__REG_DOC_EQ1_B7_B0                                     0xFF

// DoC 2nd Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ1_M                                              (Page_7 | 0x009D)
  // (ReadWrite, Bits 7:0) 
  // DoC 2nd Equalization MSB
  #define BIT_MSK__DOC_EQ1_M__REG_DOC_EQ1_B15_B8                                    0xFF

// DoC 3rd Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ2_L                                              (Page_7 | 0x009E)
  // (ReadWrite, Bits 7:0) 
  // DoC 3rd Equalization LSB
  #define BIT_MSK__DOC_EQ2_L__REG_DOC_EQ2_B7_B0                                     0xFF

// DoC 3rd Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ2_H                                              (Page_7 | 0x009F)
  // (ReadWrite, Bits 7:0) 
  // DoC 3rd Equalization MSB
  #define BIT_MSK__DOC_EQ2_H__REG_DOC_EQ2_B15_B8                                    0xFF

// DoC 4th Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ3_L                                              (Page_7 | 0x00A0)
  // (ReadWrite, Bits 7:0) 
  // DoC 4th Equalization LSB
  #define BIT_MSK__DOC_EQ3_L__REG_DOC_EQ3_B7_B0                                     0xFF

// DoC 4th Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ3_H                                              (Page_7 | 0x00A1)
  // (ReadWrite, Bits 7:0) 
  // DoC 4th Equalization MSB
  #define BIT_MSK__DOC_EQ3_H__REG_DOC_EQ3_B15_B8                                    0xFF

// DoC 5th Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ4_L                                              (Page_7 | 0x00A2)
  // (ReadWrite, Bits 7:0) 
  // DoC 5th Equalization LSB
  #define BIT_MSK__DOC_EQ4_L__REG_DOC_EQ4_B7_B0                                     0xFF

// DoC 5th Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ4_H                                              (Page_7 | 0x00A3)
  // (ReadWrite, Bits 7:0) 
  // DoC 5th Equalization MSB
  #define BIT_MSK__DOC_EQ4_H__REG_DOC_EQ4_B15_B8                                    0xFF

// DoC 6th Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ5_L                                              (Page_7 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // DoC 6th Equalization LSB
  #define BIT_MSK__DOC_EQ5_L__REG_DOC_EQ5_B7_B0                                     0xFF

// DoC 6th Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ5_H                                              (Page_7 | 0x00A5)
  // (ReadWrite, Bits 7:0) 
  // DoC 5th Equalization MSB
  #define BIT_MSK__DOC_EQ5_H__REG_DOC_EQ5_B15_B8                                    0xFF

// DoC 7th Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ6_L                                              (Page_7 | 0x00A6)
  // (ReadWrite, Bits 7:0) 
  // DoC 7th Equalization LSB
  #define BIT_MSK__DOC_EQ6_L__REG_DOC_EQ6_B7_B0                                     0xFF

// DoC 7th Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ6_H                                              (Page_7 | 0x00A7)
  // (ReadWrite, Bits 7:0) 
  // DoC 7th Equalization MSB
  #define BIT_MSK__DOC_EQ6_H__REG_DOC_EQ6_B15_B8                                    0xFF

// DoC 8th Equalization LSB Ctl Register
#define REG_ADDR__DOC_EQ7_L                                              (Page_7 | 0x00A8)
  // (ReadWrite, Bits 7:0) 
  // DoC 8th Equalization LSB
  #define BIT_MSK__DOC_EQ7_L__REG_DOC_EQ7_B7_B0                                     0xFF

// DoC 8th Equalization MSB Ctl Register
#define REG_ADDR__DOC_EQ7_H                                              (Page_7 | 0x00A9)
  // (ReadWrite, Bits 7:0) 
  // DoC 8th Equalization MSB
  #define BIT_MSK__DOC_EQ7_H__REG_DOC_EQ7_B15_B8                                    0xFF

// MHL CoC  aecho Ctl Register
#define REG_ADDR__MHL_COC_AECHO_CTL                                      (Page_7 | 0x00AA)
  // (ReadWrite, Bits 0) 
  // Enable CoC analog echo canceller
  #define BIT_MSK__MHL_COC_AECHO_CTL__REG_COC_AECHO_EN                                      0x01

// DoC/CoC Control Register
#define REG_ADDR__COC_DOC_CTL                                            (Page_7 | 0x00AC)
  // (ReadWrite, Bits 7) 
  // when mhl3_en is enable : mhl3_doc or mhl3_coc mode  selection 1'b0 mhl3 CoC mode 1'b1 mhl3 DoC mode
  #define BIT_MSK__COC_DOC_CTL__REG_MHL3_DOC_SEL                                      0x80

// DoC/CoC Software Reset Register
#define REG_ADDR__COC_DOC_SRTS                                           (Page_7 | 0x00AE)
  // (ReadWrite, Bits 7) 
  // CoC/DoC software reset 1'b0 normat operation (default) 1'b1 reset CoC/DoC module
  #define BIT_MSK__COC_DOC_SRTS__REG_COC_DOC_RST                                       0x80

//***************************************************************************
// page 38. Address: 40
// HDCP General Control 0 Register
#define REG_ADDR__HDCP2X_CTRL_0                                          (Page_8 | 0x0000)
  // (ReadWrite, Bits 0) 
  // Constant: 1 for TX 0 for RX
  #define BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDCPTX                                       0x01
  // (ReadWrite, Bits 1) 
  // Valid only for Rx. Don't care for Tx. Constant: 1 for Repeater 0 for Receiver
  #define BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2RX_REPEATER                                   0x02
  // (ReadWrite, Bits 2) 
  // Constant: 1 for HDMI 0 for MHL
  #define BIT_MSK__HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE                                     0x04
  // (ReadWrite, Bits 7:3) 
  // rsvd
  #define BIT_MSK__HDCP2X_CTRL_0__RSVD                                                  0xF8

// HDCP General Control 1 Register
#define REG_ADDR__HDCP2X_CTRL_1                                          (Page_8 | 0x0001)
  // (ReadWrite, Bits 0) 
  // Active high long pulse
  #define BIT_MSK__HDCP2X_CTRL_1__RI_HDCP2_REAUTH_SW                                    0x01
  // (ReadWrite, Bits 1) 
  // Valid only for Tx. Don't care for Rx. 1 to mask auth_done to link (No CTL3) . Default is 0 - unmaked.
  #define BIT_MSK__HDCP2X_CTRL_1__RI_HDCP2TX_CTL3MSK                                    0x02
  // (ReadWrite, Bits 7:4) 
  // {mask_ecc mask_hpd mask_req mask_sw}  1 to mask(off) . 0 to unmask(on)
  #define BIT_MSK__HDCP2X_CTRL_1__RI_HDCP2_REAUTH_MSK                                   0xF0

// HDCP General Control 2 Register
#define REG_ADDR__HDCP2X_CTRL_2                                          (Page_8 | 0x0002)
  // (ReadWrite, Bits 0) 
  // Active high long pulse Write 1 to this register to start patch code check. Write back to 0 after code check finishes.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_DONE                                    0x01
  // (ReadWrite, Bits 1) 
  // Active high long pulse Write 1 to this register to gain write access to PRAM for patch.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_START                                   0x02
  // (ReadWrite, Bits 2) 
  // 1(default): Use i_hw_cupd_start/i_hw_cupd_done. 0: Use ri_hdcp2_cupd_start/ri_hdcp2_cupd_done.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CUPD_HW                                      0x04
  // (ReadWrite, Bits 7:4) 
  // 4'hA for HDCP 2.2 for HDMI or MHL. Do not change this regiseter.
  #define BIT_MSK__HDCP2X_CTRL_2__RI_HDCP2_CPVER_B3_B0                                  0xF0

// HDCP Interrupt0 Status Register
#define REG_ADDR__HDCP2X_INTR0                                           (Page_8 | 0x0003)
  // (ReadWrite, Bits 0) 
  // ro_hdcp2_auth_stat[0] : auth_done
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT0                                           0x01
  // (ReadWrite, Bits 1) 
  // ro_hdcp2_auth_stat[1] : auth_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT1                                           0x02
  // (ReadWrite, Bits 2) 
  // ro_hdcp2_auth_stat[2] : rpt_ready
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT2                                           0x04
  // (ReadWrite, Bits 3) 
  // ro_hdcp2_auth_stat[3] : hash_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT3                                           0x08
  // (ReadWrite, Bits 4) 
  // ro_hdcp2_auth_stat[4] : cchk_done
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT4                                           0x10
  // (ReadWrite, Bits 5) 
  // ro_hdcp2_auth_stat[5] : cchk_fail
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT5                                           0x20
  // (ReadWrite, Bits 6) 
  // ro_hdcp2_auth_stat[6] : reauth_req
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT6                                           0x40
  // (ReadWrite, Bits 7) 
  // ro_hdcp2_auth_stat[7]: polling_interval
  #define BIT_MSK__HDCP2X_INTR0__INTR0_STAT7                                           0x80

// HDCP Interrupt1 Status Register
#define REG_ADDR__HDCP2X_INTR1                                           (Page_8 | 0x0004)
  // (ReadWrite, Bits 0) 
  // ro_rpt_rcvid_changed
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT0                                           0x01
  // (ReadWrite, Bits 1) 
  // ro_rpt_smng_changed
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT1                                           0x02
  // (ReadWrite, Bits 2) 
  // ro_ake_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT2                                           0x04
  // (ReadWrite, Bits 3) 
  // ro_ske_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT3                                           0x08
  // (ReadWrite, Bits 4) 
  // ro_rpt_rcvid_xfer_done
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT4                                           0x10
  // (ReadWrite, Bits 5) 
  // ro_rpt_smng_xfer_done
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT5                                           0x20
  // (ReadWrite, Bits 6) 
  // ro_cert_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT6                                           0x40
  // (ReadWrite, Bits 7) 
  // ro_gp3[7]
  #define BIT_MSK__HDCP2X_INTR1__INTR1_STAT7                                           0x80

// HDCP Interrupt2 Status Register
#define REG_ADDR__HDCP2X_INTR2                                           (Page_8 | 0x0005)
  // (ReadWrite, Bits 0) 
  // ro_msg_intr[0]:ro_km_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT0                                           0x01
  // (ReadWrite, Bits 1) 
  // ro_msg_intr[1]:ro_ekhkm_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT1                                           0x02
  // (ReadWrite, Bits 2) 
  // ro_msg_intr[2]:ro_h_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT2                                           0x04
  // (ReadWrite, Bits 3) 
  // ro_msg_intr[3]:ro_pair_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT3                                           0x08
  // (ReadWrite, Bits 4) 
  // ro_msg_intr[4]:ro_lc_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT4                                           0x10
  // (ReadWrite, Bits 5) 
  // ro_msg_intr[5]:ro_l_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT5                                           0x20
  // (ReadWrite, Bits 6) 
  // ro_msg_intr[6]:ro_vack_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT6                                           0x40
  // (ReadWrite, Bits 7) 
  // ro_msg_intr[7]:ro_mack_sent_rcvd
  #define BIT_MSK__HDCP2X_INTR2__INTR2_STAT7                                           0x80

// HDCP AES Debug Register
#define REG_ADDR__HDCP2X_INTR3                                           (Page_8 | 0x0006)
  // (ReadWrite, Bits 0) 
  // Underrun in AES FIFO
  #define BIT_MSK__HDCP2X_INTR3__INTR3_STAT0                                           0x01
  // (ReadWrite, Bits 1) 
  // Encryption enable status changed(RX only)
  #define BIT_MSK__HDCP2X_INTR3__INTR3_STAT1                                           0x02

// HDCP Interrupt0 Mask Register
#define REG_ADDR__HDCP2X_INTR0_MASK                                      (Page_8 | 0x0007)
  // (ReadWrite, Bits 0) 
  // Mask for INTR0[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B0                                         0x01
  // (ReadWrite, Bits 1) 
  // Mask for INTR0[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B1                                         0x02
  // (ReadWrite, Bits 2) 
  // Mask for INTR0[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B2                                         0x04
  // (ReadWrite, Bits 3) 
  // Mask for INTR0[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B3                                         0x08
  // (ReadWrite, Bits 4) 
  // Mask for INTR0[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B4                                         0x10
  // (ReadWrite, Bits 5) 
  // Mask for INTR0[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B5                                         0x20
  // (ReadWrite, Bits 6) 
  // Mask for INTR0[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B6                                         0x40
  // (ReadWrite, Bits 7) 
  // Mask for INTR0[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR0_MASK__INTR0_MASK_B7                                         0x80

// HDCP Interrupt1 MaskRegister
#define REG_ADDR__HDCP2X_INTR1_MASK                                      (Page_8 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Mask for INTR1[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B0                                         0x01
  // (ReadWrite, Bits 1) 
  // Mask for INTR1[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B1                                         0x02
  // (ReadWrite, Bits 2) 
  // Mask for INTR1[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B2                                         0x04
  // (ReadWrite, Bits 3) 
  // Mask for INTR1[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B3                                         0x08
  // (ReadWrite, Bits 4) 
  // Mask for INTR1[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B4                                         0x10
  // (ReadWrite, Bits 5) 
  // Mask for INTR1[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B5                                         0x20
  // (ReadWrite, Bits 6) 
  // Mask for INTR1[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B6                                         0x40
  // (ReadWrite, Bits 7) 
  // Mask for INTR1[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR1_MASK__INTR1_MASK_B7                                         0x80

// HDCP Interrupt2 Mask Register
#define REG_ADDR__HDCP2X_INTR2_MASK                                      (Page_8 | 0x0009)
  // (ReadWrite, Bits 0) 
  // Mask for INTR2[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B0                                         0x01
  // (ReadWrite, Bits 1) 
  // Mask for INTR2[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B1                                         0x02
  // (ReadWrite, Bits 2) 
  // Mask for INTR2[2]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B2                                         0x04
  // (ReadWrite, Bits 3) 
  // Mask for INTR2[3]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B3                                         0x08
  // (ReadWrite, Bits 4) 
  // Mask for INTR2[4]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B4                                         0x10
  // (ReadWrite, Bits 5) 
  // Mask for INTR2[5]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B5                                         0x20
  // (ReadWrite, Bits 6) 
  // Mask for INTR2[6]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B6                                         0x40
  // (ReadWrite, Bits 7) 
  // Mask for INTR2[7]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR2_MASK__INTR2_MASK_B7                                         0x80

// HDCP Interrupt3 Mask Register
#define REG_ADDR__HDCP2X_INTR3_MASK                                      (Page_8 | 0x000A)
  // (ReadWrite, Bits 0) 
  // Mask for INTR3[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR3_MASK__INTR3_MASK_B0                                         0x01
  // (ReadWrite, Bits 1) 
  // Mask for INTR3[1]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__HDCP2X_INTR3_MASK__INTR3_MASK_B1                                         0x02

// HDCP Interrupt Status Register
#define REG_ADDR__HDCP2X_INTRSTATUS                                      (Page_8 | 0x000B)
  // (ReadOnly, Bits 0) 
  // OR of all HDCP interrupts
  #define BIT_MSK__HDCP2X_INTRSTATUS__RO_INTR                                               0x01
  // (ReadOnly, Bits 7:4) 
  // Byte aggregated interrupt stateu Bit 0 for HDCP2X_INTR0 Bit 1 for HDCP2X_INTR1 Bit 2 for HDCP2X_INTR2 Bit 3 for HDCP2X_INTR3
  #define BIT_MSK__HDCP2X_INTRSTATUS__RO_AGGRINTR_B3_B0                                     0xF0

// HDCP Auth Status Register
#define REG_ADDR__HDCP2X_AUTH_STAT                                       (Page_8 | 0x000C)
  // (ReadOnly, Bits 7:0) 
  // [0]: auth_done [1]: auth_fail [2]: rpt_ready [3]: hash_fail [4]: cchk_done [5]: cchk_fail [6]: reauth_req [7]: polling_inverval
  #define BIT_MSK__HDCP2X_AUTH_STAT__RO_HDCP2_AUTH_STAT_B7_B0                              0xFF

// HDCP State Status Register
#define REG_ADDR__HDCP2X_STATE                                           (Page_8 | 0x000D)
  // (ReadOnly, Bits 7:0) 
  // State for debugging
  #define BIT_MSK__HDCP2X_STATE__RO_HDCP2_STATE_B7_B0                                  0xFF

// HDCP General Staus Register
#define REG_ADDR__HDCP2X_GEN_STATUS                                      (Page_8 | 0x000E)
  // (ReadOnly, Bits 0) 
  // Debugging info: 1 if RAM patch code is running 0 if ROM code is running
  #define BIT_MSK__HDCP2X_GEN_STATUS__RO_HDCP2_PRG_SEL                                      0x01
  // (ReadOnly, Bits 1) 
  // Valid only for Rx. Don't care for Tx. Connected to HDCP2X mode detection signal from i2c. Tuns to 0 if DDC offset address 0 ~ 0x4F are accessed. Turns to 1 if DDC offset address 0x50 and above are accessed. Power on default is 0.
  #define BIT_MSK__HDCP2X_GEN_STATUS__RO_HDCP2RX_MODE_SEL                                   0x02
  // (ReadOnly, Bits 2) 
  // For TX set to 1 when connected downstream device is an HDCP2 repeater. For RX set to 1 if ri_hdcp2x_repeater(HDCP2X_CMD_CTRL_0[1]) is set and HDCP2X core is running.
  #define BIT_MSK__HDCP2X_GEN_STATUS__RO_HDCP2_REPEATER                                     0x04
  // (ReadOnly, Bits 7:4) 
  // encryptino enable status for each stream (up to 4) RX only
  #define BIT_MSK__HDCP2X_GEN_STATUS__ENC_EN                                                0xF0

// HDCP TP0  Register
#define REG_ADDR__HDCP2X_TP0                                             (Page_8 | 0x0010)
  // (ReadWrite, Bits 7:0) 
  // eclk divider selection (2=divide by 8) 0: divide by 2    1: divide by 4    2: divide by 8   other: divide by 16
  #define BIT_MSK__HDCP2X_TP0__RI_HDCP2_TP0_B7_B0                                    0xFF

// HDCP TP1  Register
#define REG_ADDR__HDCP2X_TP1                                             (Page_8 | 0x0011)
  // (ReadWrite, Bits 7:0) 
  // Coutner to generate base timer tick Value for 12ms timer = 12ms * eclk / divider / 256  24MHz eclk divide by 8 -] 140 20MHz eckl divide by 8 -] 117 For a given eclk frequency bigger value means slower tick For a given tick time faster eclk requires bigger value.
  #define BIT_MSK__HDCP2X_TP1__RI_HDCP2_TP1_B7_B0                                    0xFF

// HDCP TP2 Register
#define REG_ADDR__HDCP2X_TP2                                             (Page_8 | 0x0012)
  // (ReadWrite, Bits 7:0) 
  // Restart wait time (Default 1=12ms )
  #define BIT_MSK__HDCP2X_TP2__RI_HDCP2_TP2_B7_B0                                    0xFF

// HDCP TP3  Register
#define REG_ADDR__HDCP2X_TP3                                             (Page_8 | 0x0013)
  // (ReadWrite, Bits 7:0) 
  // DDC hang timeout  (Default 42=0x2A=504ms)
  #define BIT_MSK__HDCP2X_TP3__RI_HDCP2_TP3_B7_B0                                    0xFF

// HDCP TP4  Register
#define REG_ADDR__HDCP2X_TP4                                             (Page_8 | 0x0014)
  // (ReadWrite, Bits 7:0) 
  // H check timeout (storedkm case)  (Default 17=0x11=204ms)
  #define BIT_MSK__HDCP2X_TP4__RI_HDCP2_TP4_B7_B0                                    0xFF

// HDCP TP5  Register
#define REG_ADDR__HDCP2X_TP5                                             (Page_8 | 0x0015)
  // (ReadWrite, Bits 7:0) 
  // H check timeout (no-storedkm case)  (Default 100=1.2s)
  #define BIT_MSK__HDCP2X_TP5__RI_HDCP2_TP5_B7_B0                                    0xFF

// HDCP TP6 Register
#define REG_ADDR__HDCP2X_TP6                                             (Page_8 | 0x0016)
  // (ReadWrite, Bits 7:0) 
  // Locality check timeout  (Default 2=24ms)
  #define BIT_MSK__HDCP2X_TP6__RI_HDCP2_TP6_B7_B0                                    0xFF

// HDCP TP7  Register
#define REG_ADDR__HDCP2X_TP7                                             (Page_8 | 0x0017)
  // (ReadWrite, Bits 7:0) 
  // M check timeout  (Default 9=108ms)
  #define BIT_MSK__HDCP2X_TP7__RI_HDCP2_TP7_B7_B0                                    0xFF

// HDCP TP8  Register
#define REG_ADDR__HDCP2X_TP8                                             (Page_8 | 0x0018)
  // (ReadWrite, Bits 7:0) 
  // Certificate read timeout  (Default 9=108ms)
  #define BIT_MSK__HDCP2X_TP8__RI_HDCP2_TP8_B7_B0                                    0xFF

// HDCP TP9  Register
#define REG_ADDR__HDCP2X_TP9                                             (Page_8 | 0x0019)
  // (ReadWrite, Bits 7:0) 
  // Paring timeout  (Default 17=0x11=204ms)
  #define BIT_MSK__HDCP2X_TP9__RI_HDCP2_TP9_B7_B0                                    0xFF

// HDCP TP10 Register
#define REG_ADDR__HDCP2X_TP10                                            (Page_8 | 0x001A)
  // (ReadWrite, Bits 7:0) 
  // Authdonen wait time  (Default 17=0x11=204ms)
  #define BIT_MSK__HDCP2X_TP10__RI_HDCP2_TP10_B7_B0                                   0xFF

// HDCP TP11  Register
#define REG_ADDR__HDCP2X_TP11                                            (Page_8 | 0x001B)
  // (ReadWrite, Bits 7:0) 
  // V check timeout  (Default 167=0xA7=2s)
  #define BIT_MSK__HDCP2X_TP11__RI_HDCP2_TP11_B7_B0                                   0xFF

// HDCP TP12  Register
#define REG_ADDR__HDCP2X_TP12                                            (Page_8 | 0x001C)
  // (ReadWrite, Bits 7:0) 
  // Retry wait time  (Default 125 = 0x7D=1.5s)
  #define BIT_MSK__HDCP2X_TP12__RI_HDCP2_TP12_B7_B0                                   0xFF

// HDCP TP13  Register
#define REG_ADDR__HDCP2X_TP13                                            (Page_8 | 0x001D)
  // (ReadWrite, Bits 7:0) 
  // Stream_Ready to ENC_EN wait time. (Default 13 = 0x0D = 156ms)
  #define BIT_MSK__HDCP2X_TP13__RI_HDCP2_TP13_B7_B0                                   0xFF

// HDCP TP14 Register
#define REG_ADDR__HDCP2X_TP14                                            (Page_8 | 0x001E)
  // (ReadWrite, Bits 7:0) 
  // Wait time for V (Default 250=0xFA=3s)
  #define BIT_MSK__HDCP2X_TP14__RI_HDCP2_TP14_B7_B0                                   0xFF

// HDCP TP15  Register
#define REG_ADDR__HDCP2X_TP15                                            (Page_8 | 0x001F)
  // (ReadWrite, Bits 7:0) 
  // L check auto retry limit / 8 (Default 0)
  #define BIT_MSK__HDCP2X_TP15__RI_HDCP2_TP15_B7_B0                                   0xFF

// HDCP GP In 0 Register
#define REG_ADDR__HDCP2X_GP_IN0                                          (Page_8 | 0x0020)
  // (ReadWrite, Bits 7:0) 
  // General purpose input 0
  #define BIT_MSK__HDCP2X_GP_IN0__RI_HDCP2_GP0_B7_B0                                    0xFF

// HDCP GP In 1 Register
#define REG_ADDR__HDCP2X_GP_IN1                                          (Page_8 | 0x0021)
  // (ReadWrite, Bits 7:0) 
  // General purpose input 1
  #define BIT_MSK__HDCP2X_GP_IN1__RI_HDCP2_GP1_B7_B0                                    0xFF

// HDCP GP In 2 Register
#define REG_ADDR__HDCP2X_GP_IN2                                          (Page_8 | 0x0022)
  // (ReadWrite, Bits 7:0) 
  // General purpose input 2
  #define BIT_MSK__HDCP2X_GP_IN2__RI_HDCP2_GP2_B7_B0                                    0xFF

// HDCP GP In 3 Register
#define REG_ADDR__HDCP2X_GP_IN3                                          (Page_8 | 0x0023)
  // (ReadWrite, Bits 7:0) 
  // General purpose input 3
  #define BIT_MSK__HDCP2X_GP_IN3__RI_HDCP2_GP3_B7_B0                                    0xFF

// HDCP GP Out 0 Register
#define REG_ADDR__HDCP2X_GP_OUT0                                         (Page_8 | 0x0024)
  // (ReadOnly, Bits 7:0) 
  // Version Indicator. 0X: TX ROM                    5X: RX ROM 2X: TX RAM                     1X: RX RAM 8X: TX RAM                     9X: RX RAM 4X: Sydney TX RAM       3X: Sydney RX RAM 6X: RogueES0 TX RAM  7X: RogueES0 RX RAM
  #define BIT_MSK__HDCP2X_GP_OUT0__RO_HDCP2_GP0_B7_B0                                    0xFF

// HDCP GP Out 1 Register
#define REG_ADDR__HDCP2X_GP_OUT1                                         (Page_8 | 0x0025)
  // (ReadOnly, Bits 7:0) 
  // General purpose output 1
  #define BIT_MSK__HDCP2X_GP_OUT1__RO_HDCP2_GP1_B7_B0                                    0xFF

// HDCP GP Out 2 Register
#define REG_ADDR__HDCP2X_GP_OUT2                                         (Page_8 | 0x0026)
  // (ReadOnly, Bits 7:0) 
  // General purpose output 2
  #define BIT_MSK__HDCP2X_GP_OUT2__RO_HDCP2_GP2_B7_B0                                    0xFF

// HDCP GP Out 3 Register
#define REG_ADDR__HDCP2X_GP_OUT3                                         (Page_8 | 0x0027)
  // (ReadOnly, Bits 7:0) 
  // General purpose output 3
  #define BIT_MSK__HDCP2X_GP_OUT3__RO_HDCP2_GP3_B7_B0                                    0xFF

// HDCP2 Rx ID_0 from Core Register
#define REG_ADDR__HDCP2X_RX_ID_CORE_0                                    (Page_8 | 0x0028)
  // (ReadOnly, Bits 7:0) 
  // hdcp2 receiver ID from core For RX this 40-bit field is filled as soon as HDCP2x core starts running. For TX this 40-bit field is filled when Certification(including Receiver Device ID) is read from Rx.
  #define BIT_MSK__HDCP2X_RX_ID_CORE_0__RO_HDCP2_RCVR_ID_B7_B0                                0xFF

// HDCP2 Rx ID_1 from Core Register
#define REG_ADDR__HDCP2X_RX_ID_CORE_1                                    (Page_8 | 0x0029)
  // (ReadOnly, Bits 7:0) 
  // hdcp2 receiver ID from core
  #define BIT_MSK__HDCP2X_RX_ID_CORE_1__RO_HDCP2_RCVR_ID_B15_B8                               0xFF

// HDCP2 Rx ID_2 from Core Register
#define REG_ADDR__HDCP2X_RX_ID_CORE_2                                    (Page_8 | 0x002A)
  // (ReadOnly, Bits 7:0) 
  // hdcp2 receiver ID from core
  #define BIT_MSK__HDCP2X_RX_ID_CORE_2__RO_HDCP2_RCVR_ID_B23_B16                              0xFF

// HDCP2 Rx ID_3 from Core Register
#define REG_ADDR__HDCP2X_RX_ID_CORE_3                                    (Page_8 | 0x002B)
  // (ReadOnly, Bits 7:0) 
  // hdcp2 receiver ID from core
  #define BIT_MSK__HDCP2X_RX_ID_CORE_3__RO_HDCP2_RCVR_ID_B31_B24                              0xFF

// HDCP2 Rx ID_4 from Core Register
#define REG_ADDR__HDCP2X_RX_ID_CORE_4                                    (Page_8 | 0x002C)
  // (ReadOnly, Bits 7:0) 
  // hdcp2x receiver ID from core
  #define BIT_MSK__HDCP2X_RX_ID_CORE_4__RO_HDCP2_RCVR_ID_B39_B32                              0xFF

// HDCP Misc Control 1 Register
#define REG_ADDR__HDCP2X_RPT_DETAIL                                      (Page_8 | 0x002D)
  // (ReadWrite, Bits 0) 
  // For TX use this bit to read HDCP1DEV_DSTRM field of Receiver ID List message For RX use this bit to program HDCP1DEV_DSTRM field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM                         0x01
  // (ReadWrite, Bits 1) 
  // For TX use this bit to read HDCP20RPT_DSTRM field of Receiver ID List message For RX use this bit to program HDCP20RPT_DSTRM field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM                        0x02
  // (ReadWrite, Bits 2) 
  // For TX use this bit to read MAX_CASCADE_EXCEEDED  field of Receiver ID List message For RX use this bit to program MAX_CASCADE_EXCEEDED field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC                            0x04
  // (ReadWrite, Bits 3) 
  // For TX use this bit to read MAX_DEVICE_EXCEEDED  field of Receiver ID List message For RX use this bit to program MAX_DEVICE_EXCEEDED field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC                            0x08

// HDCP RPT SMNG K Register
#define REG_ADDR__HDCP2X_RPT_SMNG_K                                      (Page_8 | 0x002E)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to program K  field of Stream Management message For RX use this bit to read K field of Stream Manage message
  #define BIT_MSK__HDCP2X_RPT_SMNG_K__RI_HDCP2TX_RPT_SMNG_K_B7_B0                           0xFF

// HDCP Depth Control Register
#define REG_ADDR__HDCP2X_RPT_DEPTH                                       (Page_8 | 0x002F)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read DEPTH  field of Receiver ID List message For RX use this bit to program DEPTH field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DEPTH__RI_HDCP2RX_RPT_DEPTH_B7_B0                            0xFF

// HDCP Devcnt Control Register
#define REG_ADDR__HDCP2X_RPT_DEVCNT                                      (Page_8 | 0x0030)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read DEVICE_COUNT  field of Receiver ID List message For RX use this bit to program DEVICE_COUNT field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RPT_DEVCNT__RI_HDCP2RX_RPT_DEVCNT_B7_B0                           0xFF

// HDCP RPT SEQ NUM V 0 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_V_0                                  (Page_8 | 0x0031)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read seq_num_V  field of Receiver ID List message For RX use this bit to program seq_num_V field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_V_0__RI_HDCP2RX_RPT_SEQ_NUM_V_B7_B0                        0xFF

// HDCP RPT SEQ NUM V 1 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_V_1                                  (Page_8 | 0x0032)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read seq_num_V  field of Receiver ID List message For RX use this bit to program seq_num_V field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_V_1__RI_HDCP2RX_RPT_SEQ_NUM_V_B15_B8                       0xFF

// HDCP RPT SEQ NUM V 2 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_V_2                                  (Page_8 | 0x0033)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to read seq_num_V  field of Receiver ID List message For RX use this bit to program seq_num_V field of Receiver ID List message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_V_2__RI_HDCP2RX_RPT_SEQ_NUM_V_B23_B16                      0xFF

// HDCP RPT SEQ NUM M 0 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_M_0                                  (Page_8 | 0x0034)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to program seq_num_M  field of Stream Management message For RX use this bit to read seq_num_M  field of Stream Manage message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_M_0__RI_HDCP2TX_RPT_SEQ_NUM_M_B7_B0                        0xFF

// HDCP RPT SEQ NUM M 1 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_M_1                                  (Page_8 | 0x0035)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to program seq_num_M  field of Stream Management message For RX use this bit to read seq_num_M  field of Stream Manage message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_M_1__RI_HDCP2TX_RPT_SEQ_NUM_M_B15_B8                       0xFF

// HDCP RPT SEQ NUM M 2 Register
#define REG_ADDR__HDCP2X_RX_SEQ_NUM_M_2                                  (Page_8 | 0x0036)
  // (ReadWrite, Bits 7:0) 
  // For TX use this bit to program seq_num_M  field of Stream Management message For RX use this bit to read seq_num_M  field of Stream Manage message
  #define BIT_MSK__HDCP2X_RX_SEQ_NUM_M_2__RI_HDCP2TX_RPT_SEQ_NUM_M_B23_B16                      0xFF

// HDCP Input Counter 0 Register
#define REG_ADDR__HDCP2X_IPT_CTR_7TO0                                    (Page_8 | 0x0037)
  // (ReadOnly, Bits 7:0) 
  // Frame counter (input counter[33:26]) to monitor if input coutner value changes.
  #define BIT_MSK__HDCP2X_IPT_CTR_7TO0__RO_HDCP2_IPT_CTR_B7_B0                                0xFF

// HDCP Input Counter 1 Register
#define REG_ADDR__HDCP2X_IPT_CTR_15TO8                                   (Page_8 | 0x0038)
  // (ReadOnly, Bits 7:0) 
  // Frame counter (input counter[41:34]) to monitor if input coutner value changes.
  #define BIT_MSK__HDCP2X_IPT_CTR_15TO8__RO_HDCP2_IPT_CTR_B15_B8                               0xFF

// HDCP AES Control Register
#define REG_ADDR__HDCP2X_AESCTL                                          (Page_8 | 0x0039)
  // (ReadWrite, Bits 0) 
  // Apply manual AES reset
  #define BIT_MSK__HDCP2X_AESCTL__RI_AES_RST_MAN                                        0x01
  // (ReadWrite, Bits 1) 
  // Apply AES reset when authdone=0
  #define BIT_MSK__HDCP2X_AESCTL__RI_AES_RST_AUTHDONE                                   0x02

// HDCP Debug Control Register
#define REG_ADDR__HDCP2X_DBGCTL                                          (Page_8 | 0x003A)
  // (ReadWrite, Bits 7:0) 
  // Debug control
  #define BIT_MSK__HDCP2X_DBGCTL__RI_HDCP2_DBG_CTL_B7_B0                                0xFF

// HDCP Debug Control 2 Register
#define REG_ADDR__HDCP2X_DBGCTL2                                         (Page_8 | 0x003B)
  // (ReadWrite, Bits 7:0) 
  // Debug control 2
  #define BIT_MSK__HDCP2X_DBGCTL2__RI_HDCP2_DBG_CTL2_B7_B0                               0xFF

// HDCP Misc Control 0 Register
#define REG_ADDR__HDCP2X_RX_CTRL_0                                       (Page_8 | 0x0040)
  // (ReadWrite, Bits 0) 
  // Valid only for Rx. Don't care for Tx. Write to Receiver ID List message buffer.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_WR                               0x01
  // (ReadWrite, Bits 1) 
  // Valid only for Rx. Don't care for Tx. Write 1 to reset Receiver ID List message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_WR_START                         0x02
  // (ReadWrite, Bits 2) 
  // Valid only for Rx. Don't care for Tx. Write 1 to start transfer of Receiver ID List message.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_XFER_START                       0x04
  // (ReadWrite, Bits 3) 
  // Valid only for Rx. Don't care for Tx. Read from Stream Manage message buffer.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_SMNG_RD                                0x08
  // (ReadWrite, Bits 4) 
  // Valid only for Rx. Don't care for Tx. Write 1 to reset Stream Manage message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_SMNG_RD_START                          0x10
  // (ReadWrite, Bits 5) 
  // Valid only for Rx. Don't care for Tx. 0: Clear reauth_req when RX_STATUS register is read out. 1: Clear reauth_req under internal 8051 control.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_REAUTH_REQ_CLR_OPTION                              0x20
  // (ReadWrite, Bits 6) 
  // Valid only for Rx. Don't care for Tx. 0: Clear rpt_ready when RX_STATUS register is read out. 1: Clear rpt_ready under internal 8051 control.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_RPT_READY_CLR_OPTION                               0x40
  // (ReadWrite, Bits 7) 
  // Valid only for Rx. Don't care for Tx. 0: Clear msg_sz when msg_send_done is asserted. 1: Clear msg_sz under internal 8051 control.
  #define BIT_MSK__HDCP2X_RX_CTRL_0__RI_MSG_SZ_CLR_OPTION                                  0x80

// HDCP Misc Staus Register
#define REG_ADDR__HDCP2X_RX_STATUS                                       (Page_8 | 0x0041)
  // (ReadOnly, Bits 2) 
  // Valid only for Rx. Don't care for Tx. Indicate that Receiver ID List message transfer is done. (Also connected to INTR1[4])
  #define BIT_MSK__HDCP2X_RX_STATUS__RO_HDCP2RX_RPT_RCVID_XFER_DONE                        0x04

// HDCP RPT SMNG Out Register
#define REG_ADDR__HDCP2X_RX_RPT_SMNG_OUT                                 (Page_8 | 0x0042)
  // (ReadOnly, Bits 7:0) 
  // Valid only for Rx. Don't care for Tx. Data output port for Stream Manage message buffer.
  #define BIT_MSK__HDCP2X_RX_RPT_SMNG_OUT__RO_HDCP2RX_RPT_SMNG_OUT_B7_B0                         0xFF

// HDCP RPT RCVID In Register
#define REG_ADDR__HDCP2X_RX_RPT_RCVID_IN                                 (Page_8 | 0x0043)
  // (ReadWrite, Bits 7:0) 
  // Valid only for Rx. Don't care for Tx. Write one byte to Receiver ID List message buffer.
  #define BIT_MSK__HDCP2X_RX_RPT_RCVID_IN__RI_HDCP2RX_RPT_RCVID_IN                               0xFF

// HDCP ECC Control Register
#define REG_ADDR__HDCP2X_RX_ECC_CTRL                                     (Page_8 | 0x0044)
  // (ReadWrite, Bits 0) 
  // Enable ECC based out-of-sync detection
  #define BIT_MSK__HDCP2X_RX_ECC_CTRL__RI_ECC_CHK_EN                                         0x01
  // (ReadWrite, Bits 2:1) 
  // 00: accumulates ECC errors until it reaches a given threshold. 01: In a given number of consecutive frames in which ECC errors keep reaching the threshold 10: In a given number of consecutive frames we don't get any correct ECC 11: Accumulates ECC errors for a given number of frames
  #define BIT_MSK__HDCP2X_RX_ECC_CTRL__RI_ECC_CHK_MODE                                       0x06
  // (ReadWrite, Bits 3) 
  // Write 1 then 0 to clear counter manually.
  #define BIT_MSK__HDCP2X_RX_ECC_CTRL__RI_ACCM_ERR_MANU_CLR                                  0x08

// HDCP ECC Count for Check 0 Register
#define REG_ADDR__HDCP2X_RX_ECC_CNT2CHK_0                                (Page_8 | 0x0045)
  // (ReadWrite, Bits 7:0) 
  // VSYNC count to skip before starting ECC check
  #define BIT_MSK__HDCP2X_RX_ECC_CNT2CHK_0__RI_CNT2CHK_ECC_B7_B0                                  0xFF

// HDCP ECC Count for Check 1 Register
#define REG_ADDR__HDCP2X_RX_ECC_CNT2CHK_1                                (Page_8 | 0x0046)
  // (ReadWrite, Bits 0) 
  // VSYNC count to skip before starting ECC check
  #define BIT_MSK__HDCP2X_RX_ECC_CNT2CHK_1__RI_CNT2CHK_ECC_B8                                     0x01

// HDCP ECC ACCM Error Threshold 0 Register
#define REG_ADDR__HDCP2X_RX_ECC_ACCM_ERR_THR_0                           (Page_8 | 0x0047)
  // (ReadWrite, Bits 7:0) 
  // Threadshold for accumulated ECC error
  #define BIT_MSK__HDCP2X_RX_ECC_ACCM_ERR_THR_0__RI_ACCM_ERR_THR_B7_B0                                 0xFF

// HDCP ECC ACCM Error Threshold 1 Register
#define REG_ADDR__HDCP2X_RX_ECC_ACCM_ERR_THR_1                           (Page_8 | 0x0048)
  // (ReadWrite, Bits 7:0) 
  // Threadshold for accumulated ECC error
  #define BIT_MSK__HDCP2X_RX_ECC_ACCM_ERR_THR_1__RI_ACCM_ERR_THR_B15_B8                                0xFF

// HDCP ECC ACCM Error Threshold 2 Register
#define REG_ADDR__HDCP2X_RX_ECC_ACCM_ERR_THR_2                           (Page_8 | 0x0049)
  // (ReadWrite, Bits 4:0) 
  // Threadshold for accumulated ECC error
  #define BIT_MSK__HDCP2X_RX_ECC_ACCM_ERR_THR_2__RI_ACCM_ERR_THR_B20_B16                               0x1F

// HDCP ECC Frame Error Threshold 0 Register
#define REG_ADDR__HDCP2X_RX_ECC_FRM_ERR_THR_0                            (Page_8 | 0x004A)
  // (ReadWrite, Bits 7:0) 
  // Threadshold for frame ECC error
  #define BIT_MSK__HDCP2X_RX_ECC_FRM_ERR_THR_0__RI_FRAME_ECC_ERR_THR_B7_B0                            0xFF

// HDCP ECC Frame Error Threshold 1 Register
#define REG_ADDR__HDCP2X_RX_ECC_FRM_ERR_THR_1                            (Page_8 | 0x004B)
  // (ReadWrite, Bits 7:0) 
  // Threadshold for frame ECC error
  #define BIT_MSK__HDCP2X_RX_ECC_FRM_ERR_THR_1__RI_FRAME_ECC_ERR_THR_B15_B8                           0xFF

// HDCP ECC Consecutive Frames Error Threshold Register
#define REG_ADDR__HDCP2X_RX_CONS_ERR_THR                                 (Page_8 | 0x004C)
  // (ReadWrite, Bits 7:0) 
  // Number of consecutive frames in which ECC error get threshold
  #define BIT_MSK__HDCP2X_RX_CONS_ERR_THR__RI_CONS_ECC_ERR_THR_B7_B0                             0xFF

// HDCP ECC No Error Threshold Register
#define REG_ADDR__HDCP2X_RX_ECC_NO_ERR_THR                               (Page_8 | 0x004D)
  // (ReadWrite, Bits 7:0) 
  // Number of consecutive frames that does not get correct ECC
  #define BIT_MSK__HDCP2X_RX_ECC_NO_ERR_THR__RI_NO_ECC_THR_B7_B0                                   0xFF

// HDCP ECC Given Frame Error Register
#define REG_ADDR__HDCP2X_RX_GVN_FRM                                      (Page_8 | 0x004E)
  // (ReadWrite, Bits 7:0) 
  // Number of consecutive frames in which accumulate ECC error
  #define BIT_MSK__HDCP2X_RX_GVN_FRM__RI_GIVEN_FRAME_B7_B0                                  0xFF

// HDCP ECC Given Frame Error Threshold 0 Register
#define REG_ADDR__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_0                        (Page_8 | 0x004F)
  // (ReadWrite, Bits 7:0) 
  // Threshold for the number of ECC errors in given frames
  #define BIT_MSK__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_0__RI_GIVEN_FRAME_ERR_THR_B7_B0                          0xFF

// HDCP ECC Given Frame Error Threshold 1 Register
#define REG_ADDR__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_1                        (Page_8 | 0x0050)
  // (ReadWrite, Bits 7:0) 
  // Threshold for the number of ECC errors in given frames
  #define BIT_MSK__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_1__RI_GIVEN_FRAME_ERR_THR_B15_B8                         0xFF

// HDCP ECC Given Frame Error Threshold 2 Register
#define REG_ADDR__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_2                        (Page_8 | 0x0051)
  // (ReadWrite, Bits 4:0) 
  // Threshold for the number of ECC errors in given frames
  #define BIT_MSK__HDCP2X_RX_ECC_GVN_FRM_ERR_THR_2__RI_GIVEN_FRAME_ERR_THR_B20_B16                        0x1F

// HDCP Misc Control 0 Register
#define REG_ADDR__HDCP2X_TX_CTRL_0                                       (Page_8 | 0x0070)
  // (ReadWrite, Bits 0) 
  // Valid only for Tx. Don't care for Rx. Read from Receiver ID List message buffer.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD                               0x01
  // (ReadWrite, Bits 1) 
  // Valid only for Tx. Don't care for Rx. Write 1 to reset Receiver ID List message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START                         0x02
  // (ReadWrite, Bits 2) 
  // Valid only for Tx. Don't care for Rx. Write 1 to start transfer of Stream Manage message.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START                        0x04
  // (ReadWrite, Bits 3) 
  // Valid only for Tx. Don't care for Rx. Write to Stream Manage message buffer.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR                                0x08
  // (ReadWrite, Bits 4) 
  // Valid only for Tx. Don't care for Rx. Write 1 to reset Stream Manage message buffer pointer to 0.
  #define BIT_MSK__HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START                          0x10

// HDCP Misc Staus Register
#define REG_ADDR__HDCP2X_TX_STATUS                                       (Page_8 | 0x0071)
  // (ReadOnly, Bits 2) 
  // Valid only for Tx. Don't care for Rx. Indicate that Stream Manage message transfer is done. (Also connected to INTR1[5])
  #define BIT_MSK__HDCP2X_TX_STATUS__RO_HDCP2TX_RPT_SMNG_XFER_DONE                         0x04

// HDCP RPT SMNG In Register
#define REG_ADDR__HDCP2X_TX_RPT_SMNG_IN                                  (Page_8 | 0x0072)
  // (ReadWrite, Bits 7:0) 
  // Valid only for Tx. Don't care for Rx. Write one byte to Stream Manage message buffer.
  #define BIT_MSK__HDCP2X_TX_RPT_SMNG_IN__RI_HDCP2TX_RPT_SMNG_IN                                0xFF

// HDCP RPT RCVID Out Register
#define REG_ADDR__HDCP2X_TX_RPT_RCVID_OUT                                (Page_8 | 0x0073)
  // (ReadOnly, Bits 7:0) 
  // Valid only for Tx. Don't care for Rx. Data output port for Receiver ID List message buffer.
  #define BIT_MSK__HDCP2X_TX_RPT_RCVID_OUT__RO_HDCP2TX_RPT_RCVID_OUT_B7_B0                        0xFF

// HDCP Stream Count 0a Register
#define REG_ADDR__HDCP2X_STM_CTR_0A                                      (Page_8 | 0x0080)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_0A__RI_HDCP2TX_STM_CTR_B7_B0                              0xFF

// HDCP Stream Count 0b Register
#define REG_ADDR__HDCP2X_STM_CTR_0B                                      (Page_8 | 0x0081)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_0B__RI_HDCP2TX_STM_CTR_B15_B8                             0xFF

// HDCP Stream Count 0c Register
#define REG_ADDR__HDCP2X_STM_CTR_0C                                      (Page_8 | 0x0082)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_0C__RI_HDCP2TX_STM_CTR_B23_B16                            0xFF

// HDCP Stream Count 0d Register
#define REG_ADDR__HDCP2X_STM_CTR_0D                                      (Page_8 | 0x0083)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_0D__RI_HDCP2TX_STM_CTR_B31_B24                            0xFF

// HDCP Stream Count 1a Register
#define REG_ADDR__HDCP2X_STM_CTR_1A                                      (Page_8 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_1A__RI_HDCP2TX_STM_CTR_B39_B32                            0xFF

// HDCP Stream Count 1b Register
#define REG_ADDR__HDCP2X_STM_CTR_1B                                      (Page_8 | 0x0085)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_1B__RI_HDCP2TX_STM_CTR_B47_B40                            0xFF

// HDCP Stream Count 1c Register
#define REG_ADDR__HDCP2X_STM_CTR_1C                                      (Page_8 | 0x0086)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_1C__RI_HDCP2TX_STM_CTR_B55_B48                            0xFF

// HDCP Stream Count 1d Register
#define REG_ADDR__HDCP2X_STM_CTR_1D                                      (Page_8 | 0x0087)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_1D__RI_HDCP2TX_STM_CTR_B63_B56                            0xFF

// HDCP Stream Count 2a Register
#define REG_ADDR__HDCP2X_STM_CTR_2A                                      (Page_8 | 0x0088)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_2A__RI_HDCP2TX_STM_CTR_B71_B64                            0xFF

// HDCP Stream Count 2b Register
#define REG_ADDR__HDCP2X_STM_CTR_2B                                      (Page_8 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_2B__RI_HDCP2TX_STM_CTR_B79_B72                            0xFF

// HDCP Stream Count 2c Register
#define REG_ADDR__HDCP2X_STM_CTR_2C                                      (Page_8 | 0x008A)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_2C__RI_HDCP2TX_STM_CTR_B87_B80                            0xFF

// HDCP Stream Count 2d Register
#define REG_ADDR__HDCP2X_STM_CTR_2D                                      (Page_8 | 0x008B)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_2D__RI_HDCP2TX_STM_CTR_B95_B88                            0xFF

// HDCP Stream Count 3a Register
#define REG_ADDR__HDCP2X_STM_CTR_3A                                      (Page_8 | 0x008C)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_3A__RI_HDCP2TX_STM_CTR_B103_B96                           0xFF

// HDCP Stream Count 3b Register
#define REG_ADDR__HDCP2X_STM_CTR_3B                                      (Page_8 | 0x008D)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_3B__RI_HDCP2TX_STM_CTR_B111_B104                          0xFF

// HDCP Stream Count 3c Register
#define REG_ADDR__HDCP2X_STM_CTR_3C                                      (Page_8 | 0x008E)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_3C__RI_HDCP2TX_STM_CTR_B119_B112                          0xFF

// HDCP Stream Count 3d Register
#define REG_ADDR__HDCP2X_STM_CTR_3D                                      (Page_8 | 0x008F)
  // (ReadWrite, Bits 7:0) 
  // Only for MHL TX
  #define BIT_MSK__HDCP2X_STM_CTR_3D__RI_HDCP2TX_STM_CTR_B127_B120                          0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_CTRL0                                     (Page_8 | 0x00A0)
  // (ReadWrite, Bits 7:0) 
  // Debug Control 0 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_CTRL0__REG_HDCP2X_DEBUG_CTRL0                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_CTRL1                                     (Page_8 | 0x00A1)
  // (ReadWrite, Bits 7:0) 
  // Debug Control 1 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_CTRL1__REG_HDCP2X_DEBUG_CTRL1                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_CTRL2                                     (Page_8 | 0x00A2)
  // (ReadWrite, Bits 7:0) 
  // Debug Control 2 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_CTRL2__REG_HDCP2X_DEBUG_CTRL2                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_CTRL3                                     (Page_8 | 0x00A3)
  // (ReadWrite, Bits 7:0) 
  // Debug Control 3 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_CTRL3__REG_HDCP2X_DEBUG_CTRL3                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_CTRL4                                     (Page_8 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // Debug Control 4 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_CTRL4__REG_HDCP2X_DEBUG_CTRL4                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT0                                     (Page_8 | 0x00A5)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 0 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT0__REG_HDCP2X_DEBUG_STAT0                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT1                                     (Page_8 | 0x00A6)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 1 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT1__REG_HDCP2X_DEBUG_STAT1                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT2                                     (Page_8 | 0x00A7)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 2 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT2__REG_HDCP2X_DEBUG_STAT2                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT3                                     (Page_8 | 0x00A8)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 3 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT3__REG_HDCP2X_DEBUG_STAT3                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT4                                     (Page_8 | 0x00A9)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 4 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT4__REG_HDCP2X_DEBUG_STAT4                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT5                                     (Page_8 | 0x00AA)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 5 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT5__REG_HDCP2X_DEBUG_STAT5                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT6                                     (Page_8 | 0x00AB)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 6 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT6__REG_HDCP2X_DEBUG_STAT6                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT7                                     (Page_8 | 0x00AC)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 7 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT7__REG_HDCP2X_DEBUG_STAT7                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT8                                     (Page_8 | 0x00AD)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 8 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT8__REG_HDCP2X_DEBUG_STAT8                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT9                                     (Page_8 | 0x00AE)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 9 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT9__REG_HDCP2X_DEBUG_STAT9                                0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT10                                    (Page_8 | 0x00AF)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 10 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT10__REG_HDCP2X_DEBUG_STAT10                               0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT11                                    (Page_8 | 0x00B0)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 11 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT11__REG_HDCP2X_DEBUG_STAT11                               0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT12                                    (Page_8 | 0x00B1)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 12 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT12__REG_HDCP2X_DEBUG_STAT12                               0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT13                                    (Page_8 | 0x00B2)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 13 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT13__REG_HDCP2X_DEBUG_STAT13                               0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT14                                    (Page_8 | 0x00B3)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 14 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT14__REG_HDCP2X_DEBUG_STAT14                               0xFF

// HDCP Temp Register
#define REG_ADDR__HDCP2X_DEBUG_STAT15                                    (Page_8 | 0x00B4)
  // (ReadOnly, Bits 7:0) 
  // Debug Status 15 for HDCP 2.x
  #define BIT_MSK__HDCP2X_DEBUG_STAT15__REG_HDCP2X_DEBUG_STAT15                               0xFF

// HDCP Software Reset Register
#define REG_ADDR__HDCP2X_TX_SRST                                         (Page_8 | 0x00B5)
  // (ReadWrite, Bits 0) 
  // HDCP Soft Reset for crystal clock domain
  #define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_CRST                                       0x01
  // (ReadWrite, Bits 1) 
  // HDCP Soft Reset for pixel clock domain
  #define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_PRST                                       0x02
  // (ReadWrite, Bits 5) 
  // Software Reset for hdcp2x logic only 1 - Reset 0 - Normal operation (default).
  #define BIT_MSK__HDCP2X_TX_SRST__REG_HDCP2X_SW_RST                                     0x20

  // HDCP Polling Control and Status Register
#define REG_ADDR__HDCP2X_POLL_CS                                         (Page_8 | 0x00B6)

  #define BIT_MSK__HDCP2X_DS_POLL_EN                                                     0x01

// HDCP PRAM Starting Address Hi Register
#define REG_ADDR__HDCP2X_CUPD_START_ADDR_HI                              (Page_8 | 0x00B8)
  // (ReadWrite, Bits 7:0) 
  // Higher byte starting address for PRAM access This is the starting address for PRAM access. Program this before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset
  #define BIT_MSK__HDCP2X_CUPD_START_ADDR_HI__REG_HDCP2X_CUPD_S_ADDR_B15_B8                         0xFF

// HDCP PRAM Signature Starting Address Lo Register
#define REG_ADDR__HDCP2X_CUPD_SIGN_START_ADDR_LO                         (Page_8 | 0x00B9)
  // (ReadWrite, Bits 7:0) 
  // Lower byte starting address for signature portion of PRAM access This is the starting address for signature portion for PRAM access. Program this before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset
  #define BIT_MSK__HDCP2X_CUPD_SIGN_START_ADDR_LO__REG_HDCP2X_CUPD_SI_S_ADDR_B7_B0                       0xFF

// HDCP PRAM Signature Starting Address Hi Register
#define REG_ADDR__HDCP2X_CUPD_SIGN_START_ADDR_HI                         (Page_8 | 0x00BA)
  // (ReadWrite, Bits 7:0) 
  // Higher byte starting address for signature portion of PRAM access This is the starting address for signature portion for PRAM access. Program this before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset
  #define BIT_MSK__HDCP2X_CUPD_SIGN_START_ADDR_HI__REG_HDCP2X_CUPD_SI_S_ADDR_B15_B8                      0xFF

// HDCP PRAM Signature Ending Address Lo Register
#define REG_ADDR__HDCP2X_PRAM_SIGN_END_ADDR_LO                           (Page_8 | 0x00BB)
  // (ReadWrite, Bits 7:0) 
  // Lower byte ending address for signature portion of PRAM access This is the starting address for signature portion for PRAM access. Program this before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset
  #define BIT_MSK__HDCP2X_PRAM_SIGN_END_ADDR_LO__REG_HDCP2X_CUPD_SI_E_ADDR_B7_B0                       0xFF

// HDCP PRAM Signature Ending Address Hi Register
#define REG_ADDR__HDCP2X_CUPD_SIGN_END_ADDR_HI                           (Page_8 | 0x00BC)
  // (ReadWrite, Bits 7:0) 
  // Higher byte ending address for signature portion of PRAM access This is the starting address for signature portion for PRAM access. Program this before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset
  #define BIT_MSK__HDCP2X_CUPD_SIGN_END_ADDR_HI__REG_HDCP2X_CUPD_SI_E_ADDR_B15_B8                      0xFF

// HDCP General Control 0 Register
#define REG_ADDR__HDCP2X_CTL_0                                           (Page_8 | 0x00BD)
  // (ReadWrite, Bits 0) 
  // HDCP 2.x Enable When enabled video encryption mux and ddc mux will select HDCP 2.x. However this signal doesn't go to the hdcp2xcore
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_EN                                         0x01
  // (ReadWrite, Bits 5) 
  // DDC polling interval override
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_POLINT_OVR                                 0x20
  // (ReadWrite, Bits 6) 
  // DDC polling interval select
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_POLINT_SEL                                 0x40
  // (ReadWrite, Bits 7) 
  // HDCP 2.x Encryption Enable This goes to the TX hdmi block to generate CTL3 signal 0: Disable 1: Enable
  #define BIT_MSK__HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN                                 0x80

// HDCP General Control 1 Register
#define REG_ADDR__HDCP2X_CTL_1                                           (Page_8 | 0x00BE)
  // (ReadWrite, Bits 0) 
  // Reauthentication request This goes to ri_reauth_sw of hdcp2xcore Negative edge triggered
  #define BIT_MSK__HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW                                  0x01
  // (ReadWrite, Bits 1) 
  // HPD Override
  #define BIT_MSK__HDCP2X_CTL_1__REG_HDCP2X_HPD_OVR                                    0x02
  // (ReadWrite, Bits 2) 
  // HDP Override Value
  #define BIT_MSK__HDCP2X_CTL_1__REG_HDCP2X_HPD_SW                                     0x04

// HDCP General Control 1 Register
#define REG_ADDR__HDCP2X_CTL_2                                           (Page_8 | 0x00BF)
  // (ReadWrite, Bits 0) 
  // Address reset for PRAM address reset Write 1 to this to give a pulse to reset the internal address pointer to the addresses specified by the other registers such as reg_hdcp2x_cupd_start_addr; reg_hdcp2x_cupd_sign_start_addr etc.
  #define BIT_MSK__HDCP2X_CTL_2__REG_HDCP2X_CUPD_ADDR_RESET                            0x01

// HDCP CUPD Size Lo Register
#define REG_ADDR__HDCP2X_CUPD_SIZE_LO                                    (Page_8 | 0x00C0)
  // (ReadWrite, Bits 7:0) 
  // Low byte of the size of updated code excluding signature Must be set before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset.
  #define BIT_MSK__HDCP2X_CUPD_SIZE_LO__REG_HDCP2X_CUPD_SIZE_B7_B0                            0xFF

// HDCP CUPD Size Hi Register
#define REG_ADDR__HDCP2X_CUPD_SIZE_HI                                    (Page_8 | 0x00C1)
  // (ReadWrite, Bits 7:0) 
  // Low byte of the size of updated code excluding signature Must be set before reg_hdcp2x_cupd_start or reg_hdcp2x_cupd_addr_reset.
  #define BIT_MSK__HDCP2X_CUPD_SIZE_HI__REG_HDCP2X_CUPD_SIZE_B15_B8                           0xFF

// HDCP General Staus Register
#define REG_ADDR__HDCP2X_GEN_STA                                         (Page_8 | 0x00C2)
  // (ReadOnly, Bits 0) 
  // Code update done
  #define BIT_MSK__HDCP2X_GEN_STA__REG_HDCP2X_CUPD_DONE                                  0x01

// HDCP Poll Interval 0 Register
#define REG_ADDR__HDCP2X_POLL_VAL0                                       (Page_8 | 0x00C3)
  // (ReadWrite, Bits 7:0) 
  // DDC polling interval0
  #define BIT_MSK__HDCP2X_POLL_VAL0__REG_HDCP2X_POL_VAL0_B7_B0                             0xFF

// HDCP Poll Interval 1 Register
#define REG_ADDR__HDCP2X_POLL_VAL1                                       (Page_8 | 0x00C4)
  // (ReadWrite, Bits 7:0) 
  // DDC polling interval1
  #define BIT_MSK__HDCP2X_POLL_VAL1__REG_HDCP2X_POL_VAL1_B7_B0                             0xFF

// HDCP DDCM Status Register
#define REG_ADDR__HDCP2X_DDCM_STS                                        (Page_8 | 0x00C5)
  // (ReadOnly, Bits 3:0) 
  // DDCM Control Status
  #define BIT_MSK__HDCP2X_DDCM_STS__REG_HDCP2X_DDCM_CTL_CS_B3_B0                          0x0F
  // (ReadOnly, Bits 7:4) 
  // DDCM Error Status
  #define BIT_MSK__HDCP2X_DDCM_STS__REG_HDCP2X_DDCM_ERR_STS_B3_B0                         0xF0

// HDCP Ring OSC Bist Register
#define REG_ADDR__HDCP2X_ROSC_BIST                                       (Page_8 | 0x00C6)
  // (ReadWrite, Bits 0) 
  // Start HDCP2 ring oscillator BIST
  #define BIT_MSK__HDCP2X_ROSC_BIST__REG_HDCP2X_RINGOSC_BIST_START                         0x01
  // (ReadOnly, Bits 1) 
  // Indicates that HDCP2 ring oscillator BIST is done
  #define BIT_MSK__HDCP2X_ROSC_BIST__REG_HDCP2X_RINGOSC_BIST_DONE                          0x02
  // (ReadOnly, Bits 2) 
  // Indicates that HDCP2 ring oscillator BIST fails
  #define BIT_MSK__HDCP2X_ROSC_BIST__REG_HDCP2X_RINGOSC_BIST_FAIL                          0x04

// HDCP2 PRAM Data In/Out Register
#define REG_ADDR__HDCP2_PRAM_DATA                                        (Page_8 | 0x00CF)
  // (ReadWrite, Bits 7:0) 
  // HDCP2 PRAM data register for PRAM data read and write.
  #define BIT_MSK__HDCP2_PRAM_DATA__HDCP2_PRAM_DATA                                       0xFF

//***************************************************************************
// page 39. Address: 40
// HDMI2 Scramble Control Register
#define REG_ADDR__SCRCTL                                                 (Page_9 | 0x0000)
  // (ReadWrite, Bits 0) 
  // Scrambler ON/OFF. Should follow HDMI2 spec such as mandatory for 600MHz, optional for lower, etc (from i2c or decision HW if availble; def=0)
  #define BIT_MSK__SCRCTL__REG_SCR_ON                                            0x01
  // (ReadWrite, Bits 1) 
  // Scrambler mode: 0:  normal (default) 1:  CTS
  #define BIT_MSK__SCRCTL__REG_SCR_MD                                            0x02
  // (ReadWrite, Bits 2) 
  // HDMI mode overwriting on/off (from i2c; def=0)
  #define BIT_MSK__SCRCTL__REG_HDMIMD_OVR                                        0x04
  // (ReadWrite, Bits 3) 
  // HDMI mode overwriting value  (from i2c; def=0)
  #define BIT_MSK__SCRCTL__REG_HDMIMD_VAL                                        0x08
  // (ReadWrite, Bits 4) 
  // Special byass ON/OFF         (from i2c; def=0)
  #define BIT_MSK__SCRCTL__REG_HDMI2_BYP                                         0x10
  // (ReadWrite, Bits 5) 
  // 1 - HDMI2 encoder using packet analyzer 0 - Legacy encoder used for HDMI/MHL        - For MHL1/2/3 stream coming from TX datapath,         legacy encoder (0) has to be selected        - For HDMI2 stream, new encoder (1) has to be        selected        - For DVI/HDMI1.4 stream, either one works, but        for the better compatibility, we recommend to        use legacy encoder (0)                In summary, for HDMI2 stream, use 1, and for        all others (HDMI1.x, MHL1/2/3) use 0. This has        to come from TOP HW/FW that knows the stream it        makes
  #define BIT_MSK__SCRCTL__REG_HDMI2_ON                                          0x20

// HDMI Control 0 Register
#define REG_ADDR__HDMI2CTL0                                              (Page_9 | 0x0001)
  // (ReadWrite, Bits 0) 
  // 1:  Bit swap Q data out from HDMI2 encoder 0:  No swap (default)
  #define BIT_MSK__HDMI2CTL0__REG_Q_9T0                                             0x01
  // (ReadWrite, Bits 1) 
  // 1:  Has 4 full channels (RGB, CK) (default) 0:  Force CK channel to all zero
  #define BIT_MSK__HDMI2CTL0__REG_USE_CH_MUX                                        0x02
  // (ReadWrite, Bits 2) 
  // 1:  Invert tx bit  for ch0,1,2CK 0:  Normal (default)
  #define BIT_MSK__HDMI2CTL0__REG_TX_BIT_INV                                        0x04

// HDMI Control 1 Register
#define REG_ADDR__HDMI2CTL1                                              (Page_9 | 0x0002)
  // (ReadWrite, Bits 1:0) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMI2CTL1__REG_Q0_SEL                                            0x03
  // (ReadWrite, Bits 3:2) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMI2CTL1__REG_Q1_SEL                                            0x0C
  // (ReadWrite, Bits 5:4) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMI2CTL1__REG_Q2_SEL                                            0x30
  // (ReadWrite, Bits 7:6) 
  // 00:  select ch0 01:  select ch1 10:  select ch2 11:  select ck
  #define BIT_MSK__HDMI2CTL1__REG_QC_SEL                                            0xC0

// HDMI TXC Data 0L Register
#define REG_ADDR__TXC_DATA0L                                             (Page_9 | 0x0003)
  // (ReadWrite, Bits 7:0) 
  // HDMI TX Clock data byte 0 Low portion
  #define BIT_MSK__TXC_DATA0L__REG_TXC_DATA0_B7_B0                                   0xFF

// HDMI TXC Data 0H Register
#define REG_ADDR__TXC_DATA0H                                             (Page_9 | 0x0004)
  // (ReadWrite, Bits 1:0) 
  // HDMI TX Clock data byte 0 high portion
  #define BIT_MSK__TXC_DATA0H__REG_TXC_DATA0_B9_B8                                   0x03

// HDMI TXC DATA 1L Register
#define REG_ADDR__TXC_DATA1L                                             (Page_9 | 0x0005)
  // (ReadWrite, Bits 7:0) 
  // HDMI TX Clock data byte 1 low portion
  #define BIT_MSK__TXC_DATA1L__REG_TXC_DATA1_B7_B0                                   0xFF

// HDMI TXC DATA 1H Register
#define REG_ADDR__TXC_DATA1H                                             (Page_9 | 0x0006)
  // (ReadWrite, Bits 1:0) 
  // HDMI TX Clock data byte 1 high portion
  #define BIT_MSK__TXC_DATA1H__REG_TXC_DATA1_B9_B8                                   0x03

// HDMI TXC DATA 2L Register
#define REG_ADDR__TXC_DATA2L                                             (Page_9 | 0x0007)
  // (ReadWrite, Bits 7:0) 
  // HDMI TX Clock data byte 2 low portion
  #define BIT_MSK__TXC_DATA2L__REG_TXC_DATA2_B7_B0                                   0xFF

// HDMI TXC DATA 2H Register
#define REG_ADDR__TXC_DATA2H                                             (Page_9 | 0x0008)
  // (ReadWrite, Bits 1:0) 
  // HDMI TX Clock data byte 2 high portion
  #define BIT_MSK__TXC_DATA2H__REG_TXC_DATA2_B9_B8                                   0x03

// HDMI TXC DIVIDER Register
#define REG_ADDR__TXC_DATA_DIV                                           (Page_9 | 0x0009)
  // (ReadWrite, Bits 1:0) 
  // 00:  TXC divide by 1 (default) 01:  TXC divide by 2 10:  TXC divide by 4 11:  TXC divide by 8
  #define BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV                                           0x03

// SCDC Control Register
#define REG_ADDR__SCDC_CTL                                               (Page_9 | 0x0020)
  // (ReadWrite, Bits 0) 
  // SCDC access enable strobe, self clear bit 0:  regular DDC master 1.  Enable SCDC DDC bus cycle Write sequence: Firmware needs to check if DDC bus is idle, then write 1 to take over DDC bus.  After writing this bit, check 0x925[2] to make sure there is no bus conflicting.  If so, repeat above procedures.  Prepare DDC_FIFO data, DDC controls and send write command. Read sequence: Firmware needs to check if DDC bus is idle, then write 1 to take over DDC bus.  After writing this bit, check 0x925[2] to make sure there is no bus conflicting.  If so, repeat above procedures.  Prepare DDC controls, send write command.  When 0x925[0] = 1, read DDC_FIFO.
  #define BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS                                       0x01
  // (ReadWrite, Bits 1) 
  // SCDC auto reply read request from slave for SCDC registers up_flag0 and up_flag1 0:  Disable (default), ignore read request 1:  Enable
  #define BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY                                   0x02
  // (ReadWrite, Bits 2) 
  // SCDC auto polling read from slave for SCDC registers up_flag0 and up_flag1 on every frame 0:  Disable (default), manual polling by firmware 1:  Enable
  #define BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_POLL                                    0x04
  // (ReadWrite, Bits 3) 
  // SCDC auto reply read request with a stop condition instead of reading update flags, 0x920[1] also is needed to set for this function to work. 0:  Disable (default), normal auto reply read request 1:  Enable, SCDC replies with a stop condition
  #define BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY_STOP                              0x08
  // (ReadWrite, Bits 4) 
  // SCDC auto poll select 0:  (default), use vsync as triggering point 1:  Use internal timer (240ms), program 0x927 and 0x928 for different desired time interval
  #define BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_POLL_SEL                                0x10
  // (ReadWrite, Bits 5) 
  // DDC bus stall request; it will disable reg_scdc_access bit; can't use together Setting this bit to 1 will hold ddc bus for SCDC manual transaction until firmware disable it It will stall hdcp1.4, hdcp2.2 and SCDC auto reply as well After setting bit to 1, HW will detect if ddc bus is busy.  Once the bus is free, it will generate an interrupt, ddc_stall_ack, 0x925[5], to notify firmware bus is ready to use 
  #define BIT_MSK__SCDC_CTL__REG_DDC_STALL_REQ                                     0x20
  // (ReadWrite, Bits 6) 
  // 0: (default)  Firmware will grab the ddc bus any time when it is not used 1:  Once reg_ddc_stall_req is set to 1; it will wait for 128 frames to see if there is any hdcp activity; if so; will wait for current hdcp transaction finish before grabbing the bus; or timer expires.
  #define BIT_MSK__SCDC_CTL__REG_SCDC_HDCP_DET_EN                                  0x40

// SCDC Active Status Register
#define REG_ADDR__SCDC_ACT_STATUS                                        (Page_9 | 0x0021)
  // (ReadOnly, Bits 0) 
  // SCDC is using DDC Bus
  #define BIT_MSK__SCDC_ACT_STATUS__REG_SCDC_ACTIVE                                       0x01
  // (ReadOnly, Bits 1) 
  // SCDC auto reply read request  from slave for SCDC registers up_flag0 and up_flag1  or read polling in progress
  #define BIT_MSK__SCDC_ACT_STATUS__REG_SCDC_RREQ_IN_PROG                                 0x02
  // (ReadOnly, Bits 2) 
  // SCDC Read Write in progress
  #define BIT_MSK__SCDC_ACT_STATUS__REG_SCDC_IN_PROG                                      0x04

// SCDC State Machine Status Register
#define REG_ADDR__SCDC_STATE                                             (Page_9 | 0x0022)
  // (ReadOnly, Bits 3:0) 
  // Auto Reply to SCDC slave read request state machine status
  #define BIT_MSK__SCDC_STATE__REG_SCDC_RREQ_STATE                                   0x0F
  // (ReadOnly, Bits 7:4) 
  // SCDC read/write state machine status
  #define BIT_MSK__SCDC_STATE__REG_SCDC_STATE                                        0xF0

// SCDC Update Flag 0 Status Register
#define REG_ADDR__SCDC_UP_FLAG0                                          (Page_9 | 0x0023)
  // (ReadOnly, Bits 7:0) 
  // SCDC Update Flag0 Status
  #define BIT_MSK__SCDC_UP_FLAG0__REG_SCDC_UP_FLAG0                                     0xFF

// SCDC Update Flag 1 Status Register
#define REG_ADDR__SCDC_UP_FLAG1                                          (Page_9 | 0x0024)
  // (ReadOnly, Bits 7:0) 
  // SCDC Update Flag1 Status
  #define BIT_MSK__SCDC_UP_FLAG1__REG_SCDC_UP_FLAG1                                     0xFF

// SCDC interrupt 0 Register
#define REG_ADDR__SCDC_INTR0                                             (Page_9 | 0x0025)
  // (ReadWrite, Bits 0) 
  // SCDC DDC read/write access done
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT0                                  0x01
  // (ReadWrite, Bits 1) 
  // SCDC DDC read update flag done
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT1                                  0x02
  // (ReadWrite, Bits 2) 
  // SCDC DDC conflicting with other client when trying to access DDC Bus
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2                                  0x04
  // (ReadWrite, Bits 3) 
  // SCDC slave read request interrupt; slave is requesting SCDC Master to read update flags
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3                                  0x08
  // (ReadWrite, Bits 4) 
  // SCDC DDC update flag being changed comparing with previous one
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT4                                  0x10
  // (ReadWrite, Bits 5) 
  // SCDC DDC stall request acknowledge
  #define BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT5                                  0x20

// SCDC interrupt 0 Mask Register
#define REG_ADDR__SCDC_INTR0_MASK                                        (Page_9 | 0x0026)
  // (ReadWrite, Bits 0) 
  // SCDC DDC read/write access done mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK0                                  0x01
  // (ReadWrite, Bits 1) 
  // SCDC DDC read update flag done mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK1                                  0x02
  // (ReadWrite, Bits 2) 
  // SCDC DDC bus conflicting mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK2                                  0x04
  // (ReadWrite, Bits 3) 
  // SCDC DDC slave read request mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK3                                  0x08
  // (ReadWrite, Bits 4) 
  // SCDC DDC update flag change mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK4                                  0x10
  // (ReadWrite, Bits 5) 
  // SCDC DDC stall request acknowledge mask 0:  disable (default) 1: enable interrupt
  #define BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK5                                  0x20

// SCDC Auto Poll timer 0 Register
#define REG_ADDR__SCDC_POLL_TIMER0                                       (Page_9 | 0x0027)
  // (ReadWrite, Bits 7:0) 
  // SCDC auto polling timer
  #define BIT_MSK__SCDC_POLL_TIMER0__REG_SCDC_AUTO_POLL_TIMER_B7_B0                        0xFF

// SCDC Auto Poll timer 1 Register
#define REG_ADDR__SCDC_POLL_TIMER1                                       (Page_9 | 0x0028)
  // (ReadWrite, Bits 7:0) 
  // SCDC auto polling timer
  #define BIT_MSK__SCDC_POLL_TIMER1__REG_SCDC_AUTO_POLL_TIMER_B15_B8                       0xFF

// SCDC Auto Poll timer 2 Register
#define REG_ADDR__SCDC_POLL_TIMER2                                       (Page_9 | 0x0029)
  // (ReadWrite, Bits 5:0) 
  // SCDC auto polling timer
  #define BIT_MSK__SCDC_POLL_TIMER2__REG_SCDC_AUTO_POLL_TIMER_B21_B16                      0x3F

// SCDC Frame Limit Register
#define REG_ADDR__SCDC_FRM_LIMIT                                         (Page_9 | 0x002B)
  // (ReadWrite, Bits 7:0) 
  // SCDC Frame Limit; if internal frame counter (sync to ri_check) is less or equal to this value; then it grants the ddc bus for scdc access
  #define BIT_MSK__SCDC_FRM_LIMIT__REG_SCDC_FRAME_LIMIT                                  0xFF

// SCDC Read Req Frame Limit Register
#define REG_ADDR__RREQ_FRM_LIMIT                                         (Page_9 | 0x002C)
  // (ReadWrite, Bits 7:0) 
  // SCDC Read Request Frame Limit; if internal frame counter (sync to ri_check) is less or equal to this value; then it grants the ddc bus for scdc read request
  #define BIT_MSK__RREQ_FRM_LIMIT__REG_RREQ_FRAME_LIMIT                                  0xFF

// HDMI2MHL3 Control Register
#define REG_ADDR__HDMI2MHL3_CTRL                                         (Page_9 | 0x0080)
  // (ReadWrite, Bits 0) 
  // master enable. Override port enable
  #define BIT_MSK__HDMI2MHL3_CTRL__REG_MASTER_EN                                         0x01
  // (ReadWrite, Bits 4) 
  // Swap the order of 10bit data after encoder 
  #define BIT_MSK__HDMI2MHL3_CTRL__REG_ENCODE_SWAP                                       0x10
  // (ReadWrite, Bits 5) 
  // Choose TMDS encoding or ANSI8b10b.  1: TMDS (default) 0: 8b10b
  #define BIT_MSK__HDMI2MHL3_CTRL__REG_ENCODE_TMDS                                       0x20
  // (ReadWrite, Bits 6) 
  // Use EC Coding 1: enable  0: disable (default)
  #define BIT_MSK__HDMI2MHL3_CTRL__REG_USE_ER_CODE                                       0x40
  // (ReadWrite, Bits 7) 
  // SW Reset
  #define BIT_MSK__HDMI2MHL3_CTRL__REG_H2M3_RST                                          0x80

// HDMI2MHL3 Port0 Control Register
#define REG_ADDR__HDMI2MHL3_P0CTRL                                       (Page_9 | 0x0081)
  // (ReadWrite, Bits 0) 
  // port0 enable
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_PORT_EN                                        0x01
  // (ReadWrite, Bits 1) 
  // 0: normal mode 1: packed pixel mode
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_PIXEL_MODE                                     0x02
  // (ReadWrite, Bits 2) 
  // HDCP 2.1 control packet enable
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_HDCP_EN                                        0x04
  // (ReadWrite, Bits 3) 
  // enable unlimit payload mode
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_UNLIMIT_EN                                     0x08
  // (ReadWrite, Bits 4) 
  // Not used
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_HDCP_ENC_EN                                    0x10
  // (ReadWrite, Bits 5) 
  // Enable ECC mode
  #define BIT_MSK__HDMI2MHL3_P0CTRL__REG_P0_ECC_EN                                         0x20

// HDMI2MHL3 P0 Stream Register
#define REG_ADDR__HDMI2MHL3_P0STM                                        (Page_9 | 0x0082)
  // (ReadWrite, Bits 2:0) 
  // port0 stream ID
  #define BIT_MSK__HDMI2MHL3_P0STM__REG_P0_STM_ID                                         0x07

// HDMI2MHL3 Lane Register
#define REG_ADDR__HDMI2MHL3_NLN                                          (Page_9 | 0x0083)
  // (ReadWrite, Bits 1:0) 
  // number of lane - 1
  #define BIT_MSK__HDMI2MHL3_NLN__REG_NUM_LANE                                          0x03

// HDMI2MHL3 Payload Low Register
#define REG_ADDR__HDMI2MHL3_PLDL                                         (Page_9 | 0x0084)
  // (ReadWrite, Bits 7:0) 
  // Low byte of maximum payload of HDMI packet (default 252)
  #define BIT_MSK__HDMI2MHL3_PLDL__REG_MAX_PAYLOAD_B7_B0                                 0xFF

// HDMI2MHL3 Payload High Register
#define REG_ADDR__HDMI2MHL3_PLDH                                         (Page_9 | 0x0085)
  // (ReadWrite, Bits 1:0) 
  // High byte of maximum payload of HDMI packet (default 252)
  #define BIT_MSK__HDMI2MHL3_PLDH__REG_MAX_PAYLOAD_B9_B8                                 0x03

// HDMI2MHL3 Scramble Control Register
#define REG_ADDR__HDMI2MHL3_SCTRL                                        (Page_9 | 0x0086)
  // (ReadWrite, Bits 0) 
  // enable scrambler (default 1)
  #define BIT_MSK__HDMI2MHL3_SCTRL__REG_SCRAMBLER_EN                                      0x01
  // (ReadWrite, Bits 7:4) 
  // # of consecutive SR symbol. 0 for no SR. (default 4)
  #define BIT_MSK__HDMI2MHL3_SCTRL__REG_SR_LENGTH                                         0xF0

// HDMI2MHL3 SR Period 1st Register
#define REG_ADDR__HDMI2MHL3_SRP0                                         (Page_9 | 0x0087)
  // (ReadWrite, Bits 7:0) 
  // The period of SR symbol [7:0]
  #define BIT_MSK__HDMI2MHL3_SRP0__REG_SR_PERIOD_B7_B0                                   0xFF

// HDMI2MHL3 SR Period 2nd Register
#define REG_ADDR__HDMI2MHL3_SRP1                                         (Page_9 | 0x0088)
  // (ReadWrite, Bits 7:0) 
  // The period of SR symbol [15:8]
  #define BIT_MSK__HDMI2MHL3_SRP1__REG_SR_PERIOD_B15_B8                                  0xFF

// HDMI2MHL3 SR Period 3rd Register
#define REG_ADDR__HDMI2MHL3_SRP2                                         (Page_9 | 0x0089)
  // (ReadWrite, Bits 7:0) 
  // The period of SR symbol [23:16]
  #define BIT_MSK__HDMI2MHL3_SRP2__REG_SR_PERIOD_B23_B16                                 0xFF

// HDMI2MHL3 SR Period 4th Register
#define REG_ADDR__HDMI2MHL3_SRP3                                         (Page_9 | 0x008A)
  // (ReadWrite, Bits 7:0) 
  // The period of SR symbol [31:24]
  #define BIT_MSK__HDMI2MHL3_SRP3__REG_SR_PERIOD_B31_B24                                 0xFF

// HDMI2MHL3 Type #1 Register
#define REG_ADDR__HDMI2MHL3_TYPE1                                        (Page_9 | 0x008B)
  // (ReadWrite, Bits 3:0) 
  // type of HDMI packet (default 0x1)
  #define BIT_MSK__HDMI2MHL3_TYPE1__REG_TYPE_HDMI_PKT                                     0x0F
  // (ReadWrite, Bits 7:4) 
  // type of HDCP packet (default 0x2)
  #define BIT_MSK__HDMI2MHL3_TYPE1__REG_TYPE_HDCP_PKT                                     0xF0

// HDMI2MHL3 Type #2 Register
#define REG_ADDR__HDMI2MHL3_TYPE2                                        (Page_9 | 0x008C)
  // (ReadWrite, Bits 3:0) 
  // type of HDMI packet (default 0x3)
  #define BIT_MSK__HDMI2MHL3_TYPE2__REG_TYPE_HDMI2_PKT                                    0x0F
  // (ReadWrite, Bits 7:4) 
  // type of HDMI with ECC packet (default 0x4)
  #define BIT_MSK__HDMI2MHL3_TYPE2__REG_TYPE_ECC_PKT                                      0xF0

// HDMI2MHL3 K_SR Register
#define REG_ADDR__HDMI2MHL3_KSR                                          (Page_9 | 0x008D)
  // (ReadWrite, Bits 7:0) 
  // Scrambler Reset symbol (default 0x0 for TMDS encode)
  #define BIT_MSK__HDMI2MHL3_KSR__REG_K_SR                                              0xFF

// HDMI2MHL3 K_SOP Register
#define REG_ADDR__HDMI2MHL3_KSOP                                         (Page_9 | 0x008E)
  // (ReadWrite, Bits 7:0) 
  // Start of Packet symbol (default 0x01 for TMDS encode)
  #define BIT_MSK__HDMI2MHL3_KSOP__REG_K_SOP                                             0xFF

// HDMI2MHL3 K_EOP Register
#define REG_ADDR__HDMI2MHL3_KEOP                                         (Page_9 | 0x008F)
  // (ReadWrite, Bits 7:0) 
  // End of Packet symbol (default 0x02 for TMDS encode)
  #define BIT_MSK__HDMI2MHL3_KEOP__REG_K_EOP                                             0xFF

// HDMI2MHL3 Compression Register
#define REG_ADDR__HDMI2MHL3_COMPRESSION                                  (Page_9 | 0x0090)
  // (ReadWrite, Bits 2:0) 
  // Blank compression ratio. 0: disable; n: up to n
  #define BIT_MSK__HDMI2MHL3_COMPRESSION__REG_P0_BLK_CPR_AMT                                    0x07
  // (ReadWrite, Bits 5:4) 
  // Active compression ratio. 0: disable; n: up to n
  #define BIT_MSK__HDMI2MHL3_COMPRESSION__REG_P0_ACT_CPR_AMT                                    0x30

// HDMI2MHL3 ECC Register
#define REG_ADDR__HDMI2MHL3_ECC                                          (Page_9 | 0x0091)
  // (ReadWrite, Bits 7:0) 
  // maximum payload of ECC packet (default 246)
  #define BIT_MSK__HDMI2MHL3_ECC__REG_MAX_PAYLOAD2                                      0xFF

// PacketizerTDD Control0 Register
#define REG_ADDR__PKTTDD_CTRL0                                           (Page_9 | 0x00A0)
  // (ReadWrite, Bits 0) 
  // enable TDD mode (1: TDD mode, 0: original mode)
  #define BIT_MSK__PKTTDD_CTRL0__REG_TDD_EN                                            0x01
  // (ReadWrite, Bits 1) 
  // USB interface selection (0: ft, 1: fc)
  #define BIT_MSK__PKTTDD_CTRL0__REG_USB_MODE                                          0x02
  // (ReadWrite, Bits 2) 
  // enable TDD scrambler (1: enable)
  #define BIT_MSK__PKTTDD_CTRL0__REG_TDD_SCR_EN                                        0x04
  // (ReadWrite, Bits 5:4) 
  // test mode (00: normal, 01: Rx, 10: Tx seq, 11: Tx single)
  #define BIT_MSK__PKTTDD_CTRL0__REG_TDD_TEST_MODE                                     0x30

// PacketizerTDD Control1 Register
#define REG_ADDR__PKTTDD_CTRL1                                           (Page_9 | 0x00A1)
  // (ReadWrite, Bits 7:0) 
  // 8 LSBs of test symbol {K, data[7:0]}
  #define BIT_MSK__PKTTDD_CTRL1__REG_TDD_TEST_SYM_B7_B0                                0xFF

// PacketizerTDD Control2 Register
#define REG_ADDR__PKTTDD_CTRL2                                           (Page_9 | 0x00A2)
  // (ReadWrite, Bits 0) 
  // MSB of test symbol {K, data[7:0]}
  #define BIT_MSK__PKTTDD_CTRL2__REG_TDD_TEST_SYM_B8                                   0x01

// PacketizerTDD Sync Register
#define REG_ADDR__PKTTDD_SYNC                                            (Page_9 | 0x00A3)
  // (ReadWrite, Bits 7:0) 
  // TDD sync character (K only)
  #define BIT_MSK__PKTTDD_SYNC__REG_TDD_SYNC                                          0xFF

// PacketizerTDD Period Low Register
#define REG_ADDR__PKTTDD_PRDL                                            (Page_9 | 0x00A4)
  // (ReadWrite, Bits 7:0) 
  // Total TDD period, low byte
  #define BIT_MSK__PKTTDD_PRDL__REG_TDD_PERIOD_B7_B0                                  0xFF

// PacketizerTDD Period High Register
#define REG_ADDR__PKTTDD_PRDH                                            (Page_9 | 0x00A5)
  // (ReadWrite, Bits 1:0) 
  // Total TDD period, high byte
  #define BIT_MSK__PKTTDD_PRDH__REG_TDD_PERIOD_B9_B8                                  0x03

// PacketizerTDD Width1 Register
#define REG_ADDR__PKTTDD_WID1                                            (Page_9 | 0x00A6)
  // (ReadWrite, Bits 3:0) 
  // Sync width
  #define BIT_MSK__PKTTDD_WID1__REG_SYNC_WIDTH                                        0x0F
  // (ReadWrite, Bits 7:4) 
  // overlap of Tx drive
  #define BIT_MSK__PKTTDD_WID1__REG_OVERLAP_WIDTH                                     0xF0

// PacketizerTDD FWD WIDTH Low Register
#define REG_ADDR__PKTTDD_FWDWIDL                                         (Page_9 | 0x00A7)
  // (ReadWrite, Bits 7:0) 
  // forward width (AV + USB bw), low byte
  #define BIT_MSK__PKTTDD_FWDWIDL__REG_FWD_WIDTH_B7_B0                                   0xFF

// PacketizerTDD FWD WIDTH High Register
#define REG_ADDR__PKTTDD_FWDWIDH                                         (Page_9 | 0x00A8)
  // (ReadWrite, Bits 1:0) 
  // forward width (AV + USB bw), high byte
  #define BIT_MSK__PKTTDD_FWDWIDH__REG_FWD_WIDTH_B9_B8                                   0x03

// PacketizerTDD BWD WIDTH Register
#define REG_ADDR__PKTTDD_BWDWID                                          (Page_9 | 0x00A9)
  // (ReadWrite, Bits 5:0) 
  // backward width (USB bw)
  #define BIT_MSK__PKTTDD_BWDWID__REG_BWD_WIDTH                                         0x3F

// PacketizerTDD BWD DDLINE Low Register
#define REG_ADDR__PKTTDD_BWDDDLL                                         (Page_9 | 0x00AA)
  // (ReadWrite, Bits 7:0) 
  // deadline of backward data (sync position), low byte
  #define BIT_MSK__PKTTDD_BWDDDLL__REG_BWD_DDLINE_B7_B0                                  0xFF

// PacketizerTDD BWD DDLINE High Register
#define REG_ADDR__PKTTDD_BWDDDLH                                         (Page_9 | 0x00AB)
  // (ReadWrite, Bits 1:0) 
  // deadline of backward data (sync position), high byte
  #define BIT_MSK__PKTTDD_BWDDDLH__REG_BWD_DDLINE_B9_B8                                  0x03

// PacketizerTDD Lock Threshold Register
#define REG_ADDR__PKTTDD_LKTH                                            (Page_9 | 0x00AC)
  // (ReadWrite, Bits 3:0) 
  // deadline of backward data (sync position), high byte
  #define BIT_MSK__PKTTDD_LKTH__REG_LINK_LOCK_TH                                      0x0F
  // (ReadOnly, Bits 4) 
  // backward link lock
  #define BIT_MSK__PKTTDD_LKTH__REG_BWD_LOCK                                          0x10
  // (ReadWrite, Bits 7:5) 
  // [0]: enable debug, [2:1] debug status mux
  #define BIT_MSK__PKTTDD_LKTH__REG_DEBUG_CTL_B2_B0                                   0xE0

// PacketizerTDD TON Delay Register
#define REG_ADDR__PKTTDD_TONDLY                                          (Page_9 | 0x00AD)
  // (ReadWrite, Bits 5:0) 
  // delay between Tx data and o_tx_turn_on signal
  #define BIT_MSK__PKTTDD_TONDLY__REG_TURNON_DELAY                                      0x3F

// PacketizerTDD Sync Delay Register
#define REG_ADDR__PKTTDD_SYNCDLY                                         (Page_9 | 0x00AE)
  // (ReadWrite, Bits 5:0) 
  // delay between end of Tx data and o_check_sync
  #define BIT_MSK__PKTTDD_SYNCDLY__REG_SYNC_DELAY                                        0x3F

// PacketizerTDD WOO Width Register
#define REG_ADDR__PKTTDD_WOOWID                                          (Page_9 | 0x00AF)
  // (ReadWrite, Bits 5:0) 
  // width of window of opportunity
  #define BIT_MSK__PKTTDD_WOOWID__REG_WOO_WIDTH                                         0x3F

// PacketizerTDD Debug Status Low Register
#define REG_ADDR__PKTTDD_STAL                                            (Page_9 | 0x00B0)
  // (ReadOnly, Bits 7:0) 
  // Debug status low byte
  #define BIT_MSK__PKTTDD_STAL__REG_P0_DEBUG_STATUS_B7_B0                             0xFF

// PacketizerTDD Debug Status High Register
#define REG_ADDR__PKTTDD_STAH                                            (Page_9 | 0x00B1)
  // (ReadOnly, Bits 7:0) 
  // Debug status high byte
  #define BIT_MSK__PKTTDD_STAH__REG_P0_DEBUG_STATUS_B15_B8                            0xFF

//***************************************************************************
// page 3A. Address: 40
// ACR  Control Register
#define REG_ADDR__ACR_CTRL                                               (Page_A | 0x0001)
  // (ReadWrite, Bits 0) 
  // 0 - The CTS value updated by hardware (CTS_HVAL) is sent as HDMI       packet to Rx (default); 1 - The CTS value set by software (CTS_SVAL) is sent as HDMI packet to Rx
  #define BIT_MSK__ACR_CTRL__REG_CTS_HW_SW_SEL                                     0x01
  // (ReadWrite, Bits 1) 
  // 0 - Requests by the ACR module to transmit an CTS/N packet are ignored; so no CTS/N packets are transmitted; multiple CTS/N packet requests that are unserviced do not generate and ACR interrupt (see INTR[2]); 1 - Requests by the ACR module to transmit an CTS/N packet are serviced per microarchitecture spec; multiple CTS/N packet requests that are unserviced generate an ACR interrupt (see INTR[2]);   (default)
  #define BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN                                        0x02
  // (ReadWrite, Bits 2) 
  // select between mclk and tclk time domain mclk counter
  #define BIT_MSK__ACR_CTRL__REG_MCLK_EN                                           0x04
  // (ReadWrite, Bits 3) 
  // 1: CTS Gen without MCLK (auto MCLK generation) 0: CTS Gen with MCLK
  #define BIT_MSK__ACR_CTRL__REG_NO_MCLK_CTSGEN_SEL                                0x08

// ACR  Audio Frequency Register
#define REG_ADDR__FREQ_SVAL                                              (Page_A | 0x0002)
  // (ReadWrite, Bits 2:0) 
  // MCLK input mode: 000 = MCLK is 128*Fs 001 = MCLK is 256*Fs (default) 010 = MCLK is 384*Fs 011 = MCLK is 512*Fs                                                  100 = MCLK is 768*Fs 101 = MCLK is 1024*Fs 110 = MCLK is 1152*Fs 111 = mCLK is 192*Fs
  #define BIT_MSK__FREQ_SVAL__REG_FM_IN_VAL_SW                                      0x07

// ACR N Software Value #1 Register
#define REG_ADDR__N_SVAL1                                                (Page_A | 0x0003)
  // (ReadWrite, Bits 7:0) 
  // N value for audio clock regeneration method. Bits [7:0] of 20-bits value from the N/CTS packet.  Must be entered by software to create the correct divider.  
  #define BIT_MSK__N_SVAL1__REG_N_VAL_SW1                                         0xFF

// ACR N Software Value #2 Register
#define REG_ADDR__N_SVAL2                                                (Page_A | 0x0004)
  // (ReadWrite, Bits 7:0) 
  // N value for audio clock regeneration method. Bits [15:8] of 20-bits value from the N/CTS packet.  Must be entered by software to create the correct divider.  
  #define BIT_MSK__N_SVAL2__REG_N_VAL_SW2                                         0xFF

// ACR N Software Value #3 Register
#define REG_ADDR__N_SVAL3                                                (Page_A | 0x0005)
  // (ReadWrite, Bits 3:0) 
  // N value for audio clock regeneration method. Bits [19:16] of 20-bits value from the N/CTS packet.  Must be entered by software to create the correct divider.  
  #define BIT_MSK__N_SVAL3__REG_N_VAL_SW3                                         0x0F

// ACR CTS Software Value #1 Register
#define REG_ADDR__CTS_TXSVAL1                                            (Page_A | 0x0006)
  // (ReadWrite, Bits 7:0) 
  // Bits [7:0] of 20-bits of the audio clock regeneration CTS value that has been set by software. Whether this software value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXSVAL1__REG_CTS_VAL_SW_B7_B0                                  0xFF

// ACR CTS Software Value #2 Register
#define REG_ADDR__CTS_TXSVAL2                                            (Page_A | 0x0007)
  // (ReadWrite, Bits 7:0) 
  // Bits [15:8] of 20-bits of the audio clock regeneration CTS value that has been set by software. Whether this software value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXSVAL2__REG_CTS_VAL_SW_B15_B8                                 0xFF

// ACR CTS Software Value #3 Register
#define REG_ADDR__CTS_TXSVAL3                                            (Page_A | 0x0008)
  // (ReadWrite, Bits 3:0) 
  // Bits [19:16] of 20-bits of the audio clock regeneration CTS value that has been set by software. Whether this software value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXSVAL3__REG_CTS_VAL_SW_B19_B16                                0x0F

// ACR CTS Hardware Value #1 Register
#define REG_ADDR__CTS_TXHVAL1                                            (Page_A | 0x0009)
  // (ReadOnly, Bits 7:0) 
  // Bits [7:0] of 20-bits of the audio clock regeneration CTS value that is measured and updated by the Tx. Whether this hardware value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXHVAL1__CTS_VAL_HW_B7_B0                                      0xFF

// ACR CTS Hardware Value #2 Register
#define REG_ADDR__CTS_TXHVAL2                                            (Page_A | 0x000A)
  // (ReadOnly, Bits 7:0) 
  // Bits [15:8] of 20-bits of the audio clock regeneration CTS value that is measured and updated by the Tx. Whether this hardware value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXHVAL2__CTS_VAL_HW_B15_B8                                     0xFF

// ACR CTS Hardware Value #3 Register
#define REG_ADDR__CTS_TXHVAL3                                            (Page_A | 0x000B)
  // (ReadOnly, Bits 3:0) 
  // Bits [19:16] of 20-bits of the audio clock regeneration CTS value that is measured and updated by the Tx. Whether this hardware value is sent in the CTS packet to the Rx is controlled by ACR_CTRL[0]
  #define BIT_MSK__CTS_TXHVAL3__CTS_VAL_HW_B19_B16                                    0x0F

// Input Audio ID Register
#define REG_ADDR__AIP_IN_AUD_ID                                          (Page_A | 0x0012)
  // (ReadOnly, Bits 4:0) 
  // Identification of the incoming stream: bit 0 - ID of the SPDIF bit 1 - ID of the I2S bit 2 - ID of the DSD bit 3 - ID of the HBRA bit 4 - ID of the Audio packetized
  #define BIT_MSK__AIP_IN_AUD_ID__IN_AUD_ID                                             0x1F

// Audio En Register
#define REG_ADDR__AUD_EN                                                 (Page_A | 0x0013)
  // (ReadWrite, Bits 0) 
  // Audio input enable 0 - audio inputs disabled (default) 1 - audio inputs enabled
  #define BIT_MSK__AUD_EN__REG_AUD_IN_EN                                         0x01
  // (ReadWrite, Bits 1) 
  // AIP can select to overwrite incoming selection of the audio by values in AUD_MODE
  #define BIT_MSK__AUD_EN__REG_AUD_SEL_OWRT                                      0x02
  // (ReadWrite, Bits 2) 
  // Enable loading of the audio through DMA (parrel audio input)
  #define BIT_MSK__AUD_EN__REG_AUD_PAR_EN                                        0x04

// Audio In Mode Register
#define REG_ADDR__AUD_MODE                                               (Page_A | 0x0014)
  // (ReadWrite, Bits 0) 
  // Enable to use packetize audio as input
  #define BIT_MSK__AUD_MODE__REG_AUD_PKT_EN                                        0x01
  // (ReadWrite, Bits 1) 
  // SPDIF input enable. 0 - SPDIF input stream is disabled (default) 1 - SPDIF input stream is enabled. NOTE: This bit must also be set in case of parallel audio input if the audio format is SPDIF in order to ensure correct use of audio header type.
  #define BIT_MSK__AUD_MODE__REG_SPDIF_EN                                          0x02
  // (ReadWrite, Bits 2) 
  // High Bitrate Audio flag 0 - Input stream is not High Bitrate stream (default) 1 - Input stream is High Bitrate stream (all of the I2S control bits will apply to the control of the HB Audio)
  #define BIT_MSK__AUD_MODE__REG_HBRA_ON                                           0x04
  // (ReadWrite, Bits 3) 
  // DSD enable. Has lower priorety then spdif enable; but higher then I2S stream. When it is set then most of the I2S configuration register bits became control for the DSD logic; such as SD3/2/1/0 enable; I2S FIFO map and Channel Status registers. NOTE: This bit must also be set in case of parallel audio input if the audio format is DSD in order to ensure correct use of audio header type.
  #define BIT_MSK__AUD_MODE__REG_DSD_EN                                            0x08
  // (ReadWrite, Bits 7:4) 
  // I2S enable for SD3; SD2; SD1; SD0.  Bit 7 = SD3_en  Bit 4 = SD0_en.  All inputs disabled at default. When DSD enabled these bits control the DSD stream. NOTE: This bit(s) must also be set in case of parallel audio input if the audio format is I2S in order to ensure correct use of audio header type.
  #define BIT_MSK__AUD_MODE__REG_I2S_EN                                            0xF0

// Audio In SPDIF Control Register
#define REG_ADDR__SPDIF_CTRL                                             (Page_A | 0x0015)
  // (ReadWrite, Bits 0) 
  // 0 - hardware sample # automatically adjusted for sampling frequency changes (default); 1 - hardware is locked to the reg_max_1ui number
  #define BIT_MSK__SPDIF_CTRL__REG_1UI_LOCK                                          0x01
  // (ReadWrite, Bits 1) 
  // 0 - Use input SPDIF stream Fs; reg SPDIF Fs (default); 1 - Override input stream Fs with Software Fs; define in I2S_CHST4
  #define BIT_MSK__SPDIF_CTRL__REG_FS_OVERRIDE                                       0x02
  // (ReadWrite, Bits 2) 
  // 0 - hardware sample # automatically adjusted for sampling frequency changes (default); 1 -  if and only if lock for 1UI is set then hardware is locked to the reg_max_2ui number
  #define BIT_MSK__SPDIF_CTRL__REG_2UI_LOCK                                          0x04
  // (ReadOnly, Bits 3) 
  // 1 - there is no spdif input: spdif input never changes; 0- detected some change on the input SPDIF
  #define BIT_MSK__SPDIF_CTRL__AUDI_NO_AUDIO                                         0x08
  // (ReadOnly, Bits 7:4) 
  // Channel Status bits 33 to 35; where bit 33 = LSB and 35 = MSB               max 24                               max 20 000      not indicated (default)          not indicated (default) 001      20 bits                                 16 bits 010      22 bits                                 18 bits 100      23 bits                                 19 bits 101      24 bits                                 20 bits 110      21 bits                                 17 bits             
  #define BIT_MSK__SPDIF_CTRL__AUDI_LENGTH                                           0xF0

// Audio In SPDIF Software 1UI Overwrite Register
#define REG_ADDR__SPDIF_SSMPL                                            (Page_A | 0x0016)
  // (ReadWrite, Bits 7:0) 
  // Maximum number of the pixel clocks per one SPDIF bi-phase mark encoded Unit Interval (max 1UI).  Hardware is locked to this number if reg_1ui_lock is set to 1;  otherwise hardware calculates this number automaticly.   Default is 4 (max # if pclk @ 25 MHz and Fs  @ 48 KHz) If reg_1ui1_lock and reg_2ui_lock are both asserted  (set to 1) then max 2UI will be set to the reg_max_2ui value; If reg_1ui_lock is set; but reg_2ui_lock is not then max 2UI is set to the reg_max_1ui times 2;
  #define BIT_MSK__SPDIF_SSMPL__REG_MAX_1UI                                           0xFF

// Audio In SPDIF Hardware 1UI Sample Register
#define REG_ADDR__SPDIF_HSMPL                                            (Page_A | 0x0017)
  // (ReadOnly, Bits 7:0) 
  // Maximum number of the pixel clocks per one SPDIF bi-phase encoded Unit Interval (max 1UI); continuously sampled by the hardware. 
  #define BIT_MSK__SPDIF_HSMPL__AUDI_MAX_1UI                                          0xFF

// Audio In SPDIF Extracted Fs Register
#define REG_ADDR__SPDIF_FS                                               (Page_A | 0x0018)
  // (ReadOnly, Bits 5:0) 
  // contain the Fs extraction from SPDIF input channel status bits 24-27 30-31.   31 30 27 26 25 24 0   0   0   1   0   0 - Fs =   22.05 kHz 0   0   0   0   0   0 - Fs =   44.1 kHz 0   0   1   0   0   0 - Fs =   88.2 kHz 0   0   1   1   0   0 - Fs = 176.4 kHz 0   0   1   1   0   1 - Fs = 352.8 kHz 1   0   1   1   0   1 - Fs = 705.6 kHz 0   1   1   1   0   1 - Fs = 1411.2 kHz 0   0   0   1   1   0 - Fs =   24 kHz 0   0   0   0   1   0 - Fs =   48 kHz 0   0   1   0   1   0 - Fs =   96 kHz 0   0   1   1   1   0 - Fs = 192 kHz 0   0   0   1   0   1 - Fs = 384 kHz 0   0   1   0   0   1 - Fs = 768 kHz 0   1   0   1   0   1 - Fs = 1536 kHz 0   0   0   0   1   1 - Fs =   32 kHz 0   0   1   0   1   1 - Fs =   64 kHz 1   0   1   0   1   1 - Fs =   128 kHz 0   1   1   0   1   1 - Fs =   256 kHz 1   1   1   0   1   1 - Fs =   512 kHz 1   1   0   1   0   1 - Fs =   1024 kHz 0   0   0   0   0   1 Sampling frequency not indicated
  #define BIT_MSK__SPDIF_FS__AUDI_SPDIF_FS                                         0x3F

// Audio In SPDIF Software 2UI Overwrite Register
#define REG_ADDR__SPDIF_SSMPL2                                           (Page_A | 0x0019)
  // (ReadWrite, Bits 7:0) 
  // Maximum number of the pixel clocks per two SPDIF bi-phase encoded Unit Intervals (max 2UI).  Hardware is locked to this number if reg_2ui_lock and reg_1ui_lock both are set to 1; If only reg_1ui_lock is set to one then hardware is locked to the multiple of 2 of the software max for 1UI. Otherwise hardware calculates this number automaticly.  Default is 9 (max # if pclk @ 25 MHz and Fs  @ 48 KHz)  In the I2S mode bits 7:4 are used as swap control for the channels 0-3.  Bit 4 is for channel 0: 1 would force swap between left and right; Bit 5 is for channel 1: 1 would force swap between left and right; Bit 6 is for channel 2: 1 would force swap between left and right; Bit 7 is for channel 3: 1 would force swap between left and right;
  #define BIT_MSK__SPDIF_SSMPL2__REG_MAX_2UI                                           0xFF

// Audio In SPDIF Hardware 2UI Sample Register
#define REG_ADDR__SPDIF_HSMPL2                                           (Page_A | 0x001A)
  // (ReadOnly, Bits 7:0) 
  // Maximum number of the pixel clocks per two SPDIF bi-phase encoded Unit Intervals (max 2UI); continuously sampled by the hardware. 
  #define BIT_MSK__SPDIF_HSMPL2__AUDI_MAX_2UI                                          0xFF

// Audio In Error Threshold Register
#define REG_ADDR__SPDIF_ERTH                                             (Page_A | 0x001B)
  // (ReadWrite, Bits 5:0) 
  // Error threshold level. The frame will be marked as invalid (flat flag will be set) if during decoding of the frame the number of the bi-phase mark encode related errors will exceed this threshold level.
  #define BIT_MSK__SPDIF_ERTH__REG_AUD_ERR_THRESH                                    0x3F
  // (ReadWrite, Bits 6) 
  // enable to use I2S and SPDIF ports for 1st 6 pins of the DSD
  #define BIT_MSK__SPDIF_ERTH__REG_I2S2DSD_EN                                        0x40

// Audio IN I2S Data In Map Register
#define REG_ADDR__I2S_IN_MAP                                             (Page_A | 0x001C)
  // (ReadWrite, Bits 1:0) 
  // Channel map to FIFO #0 (for HDMI layout 0 or 1): 00 - map SD0 to FIFO #0 (default) 01 - map SD1 to FIFO #0 10 - map SD2 to FIFO #0 11 - map SD3 to FIFO #0
  #define BIT_MSK__I2S_IN_MAP__REG_FIFO0_MAP                                         0x03
  // (ReadWrite, Bits 3:2) 
  // Channel map to FIFO #1 (for HDMI layout 1): 00 - map SD0 to FIFO #1 01 - map SD1 to FIFO #1 (default) 10 - map SD2 to FIFO #1 11 - map SD3 to FIFO #1
  #define BIT_MSK__I2S_IN_MAP__REG_FIFO1_MAP                                         0x0C
  // (ReadWrite, Bits 5:4) 
  // Channel map to FIFO #2 (for HDMI layout 1): 00 - map SD0 to FIFO #2 01 - map SD1 to FIFO #2 10 - map SD2 to FIFO #2 (default) 11 - map SD3 to FIFO #2
  #define BIT_MSK__I2S_IN_MAP__REG_FIFO2_MAP                                         0x30
  // (ReadWrite, Bits 7:6) 
  // Channel map to FIFO #3 (for HDMI layout 1): 00 - map SD0 to FIFO #3 01 - map SD1 to FIFO #3 10 - map SD2 to FIFO #3 11 - map SD3 to FIFO #3 (default)
  #define BIT_MSK__I2S_IN_MAP__REG_FIFO3_MAP                                         0xC0

// Audio In I2S Control Register
#define REG_ADDR__I2S_IN_CTRL                                            (Page_A | 0x001D)
  // (ReadWrite, Bits 0) 
  // WS to SD: 1st-bit shift 0 - 1st-bit shift; Philips spec. 1 - no shift (default)
  #define BIT_MSK__I2S_IN_CTRL__REG_1ST_BIT                                           0x01
  // (ReadWrite, Bits 1) 
  // SD: MSb/LSb first: 0 - MSb first (default)
  #define BIT_MSK__I2S_IN_CTRL__REG_DATA_DIR                                          0x02
  // (ReadWrite, Bits 2) 
  // SD: Left-; Right-justified: 1 - data is right justified (default)
  #define BIT_MSK__I2S_IN_CTRL__REG_JUSTIFY                                           0x04
  // (ReadWrite, Bits 3) 
  // WS: Left/Right polarity: 0 - Left polarity when Word Select is low (default) When TDM is not used then WS for I2S (reg_ws(I2S_IN_CTRL[3])) have to match left/right orientation that Source sends. When TDM is used then need to set reg_ws(I2S_IN_CTRL[3]) to opposite of the reg_tdm_ws_negedge(TDM_CTRL[5])
  #define BIT_MSK__I2S_IN_CTRL__REG_WS                                                0x08
  // (ReadWrite, Bits 4) 
  // V bit value: 0 - For PCM data (default)
  #define BIT_MSK__I2S_IN_CTRL__REG_VBIT                                              0x10
  // (ReadWrite, Bits 5) 
  // Order of the Channel Status bits in the High Bitrate stream:  0 - Left and Right sample have consecutive C bits (default) 1 - Left and Right sample have the same C bits
  #define BIT_MSK__I2S_IN_CTRL__REG_CBIT_ORDER                                        0x20
  // (ReadWrite, Bits 6) 
  // SCK: Sample edge rising/falling 0 - Sample edge is falling: SD3-SD0 and WS source should change state on the rising edge of the SCK 1 - Sample edge is rising (default): SD3-SD0 and WS source should change state on the falling edge of SCK
  #define BIT_MSK__I2S_IN_CTRL__REG_SCK_EDGE                                          0x40
  // (ReadWrite, Bits 7) 
  // reserved
  #define BIT_MSK__I2S_IN_CTRL__RSVD                                                  0x80

// Audio In I2S Channel Status #0 Register
#define REG_ADDR__I2S_CHST0                                              (Page_A | 0x001E)
  // (ReadWrite, Bits 7:0) 
  // The information in this register is send in Channel Status field across HDMI link Channel Status byte #0. Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST0__REG_CBIT0                                             0xFF

// Audio In I2S Channel Status #1 Register
#define REG_ADDR__I2S_CHST1                                              (Page_A | 0x001F)
  // (ReadWrite, Bits 7:0) 
  // The information in this register is send in Channel Status field across HDMI link.  Category code. Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST1__REG_CBIT1                                             0xFF

// Audio In I2S Channel Status #2 Register
#define REG_ADDR__I2S_CHST2                                              (Page_A | 0x0020)
  // (ReadWrite, Bits 3:0) 
  // The information in this register is send in Channel Status field across HDMI link. Source number. Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST2__REG_CBIT2A                                            0x0F
  // (ReadWrite, Bits 7:4) 
  // The information in this register is send in Channel Status field across HDMI link Channel Number. Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST2__REG_CBIT2B                                            0xF0

// Audio in I2S Channel Status #3 Register
#define REG_ADDR__I2S_CHST3                                              (Page_A | 0x0021)
  // (ReadWrite, Bits 3:0) 
  // The information in this register is send in Channel Status field across HDMI link. Please refer to SPDIF spec for detailed description. Sampling frequency set by software (inserted into I2S stream or into SPDIF if fs_overrride is enabled). These bits correspond to the Channel Status bits 24; 25; 26;27; where bit 24 = LSB and 27 = MSB: 27 26 25 24 0   1   0   0 - Fs =   22.05 kHz 0   0   0   0 - Fs =   44.1 kHz 1   0   0   0 - Fs =   88.2 kHz 1   1   0   0 - Fs = 176.4 kHz 0   1   1   0 - Fs =   24 kHz 0   0   1   0 - Fs =   48 kHz 1   0   1   0 - Fs =   96 kHz 1   1   1   0 - Fs = 192 kHz 0   0   1   1 - Fs =   32 kHz 1  0  0  1 -  Fs =  768 kHz*  All other frequencies (default)
  #define BIT_MSK__I2S_CHST3__REG_CBIT3A                                            0x0F
  // (ReadWrite, Bits 7:4) 
  // [5:4] Clock accuracy [7:6] Sampling frequency extension with sampling frequency bits 24 to 27 hbra_on, aud_sample_freq[5:0];  Audio Rate 1b0, 2b00, 4'b0100; 22.05 kHz (n/a) 1b0, 2b00, 4'b0000; 44.1 kHz 1b0, 2b00, 4'b1000; 88.2 kHz 1b0, 2b00, 4'b1100; 176.4 kHz 1b0, 2b00, 4b1101; 352.8 kHz 1b0, 2b10, 4b1101; 705.6 kHz 1b0, 2b01, 4b1101; 1411.2 kHz (n/a) 1b0, 2b00, 4'b0110; 24 kHz (n/a) 1b0, 2b00, 4'b0010; 48 kHz 1b0, 2b00, 4'b1010; 96 kHz 1b0, 2b00, 4'b1110; 192 kHz 1b0, 2b00; 4b0101; 384 kHz 1b0, 2b00, 4b1001; 768 kHz 1b0, 2b01; 4b0101; 1536 kHz (n/a) 1b0, 2b00, 4'b0011; 32 kHz 1b0, 2b00, 4b1011; 64 kHz 1b0, 2b10, 4b1011; 128 kHz 1b0, 2b01, 4b1011; 256 kHz 1b0, 2b11, 4b1011; 512 kHz 1b0, 2b11; 4b0101; 1024 kHz (n/a) 1b1, 2b00, 4'b1100; HBRA 176.4 kHz 1b1, 2b00, 4'b1101; HBRA 352.8 kHz 1b1, 2b10, 4'b1101; HBRA 705.6 kHz 1b1, 2b01, 4b1101; HBRA 1411.2 kHz 1b1, 2b00, 4'b1110; HBRA 192 kHz 1b1, 2b00, 4'b0101; HBRA 384 kHz 1b1, 2b00, 4'b1001; HBRA 768 kHz 1b1, 2b01; 4b0101; HBRA 1536 kHz 1b1, 2b10, 4'b1011; HBRA 128 kHz 1b1, 2b01, 4b1011; HBRA 256 kHz 1b1, 2b11, 4b1011; HBRA 512 kHz 1b1, 2b11, 4b0101; HBRA 1024 kHz 
  #define BIT_MSK__I2S_CHST3__REG_CBIT3B                                            0xF0

// Audio In I2S Channel Status #4 Register
#define REG_ADDR__I2S_CHST4                                              (Page_A | 0x0022)
  // (ReadWrite, Bits 3:0) 
  // Reg_cbit[32] : The information in this register is send in Channel Status field across HDMI link. Channel Status bits32: 0 - Max audio sample word length is 20 bits 1 - Max audio sample word length is 24 bits (default) Reg_cbit[35:32] : The information in this register is send in Channel Status field across HDMI link.Channel Status bits 33 to 35; where bit 33 = LSB and 35 = MSB               max 24                               max 20 000      not indicated         not indicated  001      20 bits                                 16 bits 010      22 bits                                 18 bits 100      23 bits                                 19 bits 101      24 bits                                 20 bits 110      21 bits                                 17 bits              
  #define BIT_MSK__I2S_CHST4__REG_CBIT4A                                            0x0F
  // (ReadWrite, Bits 7:4) 
  // Original Fs
  #define BIT_MSK__I2S_CHST4__REG_CBIT4B                                            0xF0

// Audio In Sample Rate Conversion Register
#define REG_ADDR__ASRC                                                   (Page_A | 0x0023)
  // (ReadWrite, Bits 0) 
  // Enable Audio Sample rate Conversion: 0 disabled (default)
  #define BIT_MSK__ASRC__REG_SRC_EN                                            0x01
  // (ReadWrite, Bits 1) 
  // Selects downsample mode: if 0 then 2:1 mode and will do following sample rate conversion:         44.1   -] 22.05        48      -] 24        88.2   -] 44.1        96      -] 48         176.4 -] 88.2         192    -] 96 if 1 then 4:1 mode and will do following sample rate conversion:         88.2   -] 22.05        96      -] 24         176.4 -] 44.1         192    -] 48
  #define BIT_MSK__ASRC__REG_SRC_CTRL                                          0x02
  // (ReadWrite, Bits 7:4) 
  // Mask for the sample present and flat bit of the High Bit Rate Audio header.  Each bit mask out 1 of the subpacket sample presetn bits. When 0 mask out. Default; only 1 lsb bit is unmasked.
  #define BIT_MSK__ASRC__REG_HBR_SPR_MASK                                      0xF0

// Audio In I2S Input Size Register
#define REG_ADDR__I2S_IN_SIZE                                            (Page_A | 0x0024)
  // (ReadWrite, Bits 3:0) 
  // The information in this register is used for the extraction of the I2S data from the input stream. Number of the valid bits in the input I2S stream; default is 24: 1011 - 24 bits (default); 1001 - 23 bits; 0101 - 22 bits; 1101 - 21 bits; 1010 - 20 bits 1000 - 19 bits 0100 - 18 bits 1100 - 17 bits 0010 - 16 bits
  #define BIT_MSK__I2S_IN_SIZE__REG_I2S_IN_LENGTH                                     0x0F

// Audio Parallel busclk clock disabling Register 1
#define REG_ADDR__AUD_PAR_BUSCLK_1                                       (Page_A | 0x0025)
  // (ReadWrite, Bits 7:0) 
  // Audio Parallel busclk clock disabling Register
  #define BIT_MSK__AUD_PAR_BUSCLK_1__REG_AUD_PAR_B7_B0                                     0xFF

// Audio Parallel busclk clock disabling Register 2
#define REG_ADDR__AUD_PAR_BUSCLK_2                                       (Page_A | 0x0026)
  // (ReadWrite, Bits 7:0) 
  // Audio Parallel busclk clock disabling Register
  #define BIT_MSK__AUD_PAR_BUSCLK_2__REG_AUD_PAR_B15_B8                                    0xFF

// Audio Parallel busclk clock disabling Register 3
#define REG_ADDR__AUD_PAR_BUSCLK_3                                       (Page_A | 0x0027)
  // (ReadWrite, Bits 7:0) 
  // Audio Parallel busclk clock disabling Register
  #define BIT_MSK__AUD_PAR_BUSCLK_3__REG_AUD_PAR_B23_B16                                   0xFF

// Audio In I2S Channel Status #5 Register
#define REG_ADDR__I2S_CHST6                                              (Page_A | 0x0028)
  // (ReadWrite, Bits 7:0) 
  // cbit stream bits 47:40 Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST6__REG_CBIT_MSB_B7_B0                                    0xFF

// Audio In I2S Channel Status #6 Register
#define REG_ADDR__I2S_CHST7                                              (Page_A | 0x0029)
  // (ReadWrite, Bits 7:0) 
  // cbit stream bits 55:48 Please refer to SPDIF spec for detailed description.
  #define BIT_MSK__I2S_CHST7__REG_CBIT_MSB_B15_B8                                   0xFF

// DSD INTERLEAVE Register
#define REG_ADDR__DSD_INTERLEAVE                                         (Page_A | 0x002A)
  // (ReadWrite, Bits 7:0) 
  // parallel audio settings
  #define BIT_MSK__DSD_INTERLEAVE__REG_DSD_INTERLEAVE_B7_B0                              0xFF

// AUDIO_PAR_MODE_SEL Register
#define REG_ADDR__AUDIO_PAR_MODE_SEL                                     (Page_A | 0x002B)
  // (ReadWrite, Bits 2:0) 
  // parallel audio settings
  #define BIT_MSK__AUDIO_PAR_MODE_SEL__REG_AUDIO_PAR_MODE_B2_B0                              0x07

// AUDIO_RST Register
#define REG_ADDR__AIP_RST                                                (Page_A | 0x002C)
  // (ReadWrite, Bits 0) 
  // software reset for aip - will reset every piece of the AIP logic
  #define BIT_MSK__AIP_RST__REG_RST4AUDIO                                         0x01
  // (ReadWrite, Bits 1) 
  // software reset for aip fifos - will reset only Audio FIFO
  #define BIT_MSK__AIP_RST__REG_RST4AUDIO_FIFO                                    0x02
  // (ReadWrite, Bits 2) 
  // software reset for ACR - will reset only ACR logic
  #define BIT_MSK__AIP_RST__REG_RST4AUDIO_ACR                                     0x04
  // (ReadWrite, Bits 3) 
  // TDM interface software reset
  #define BIT_MSK__AIP_RST__REG_RST4AUDIO_TDM                                     0x08

// AUDIO HDMI2MHL Register
#define REG_ADDR__AIP_HDMI2MHL                                           (Page_A | 0x002D)
  // (ReadWrite, Bits 0) 
  // Indicates output mode: 0 - HDMI, 1 - MHL
  #define BIT_MSK__AIP_HDMI2MHL__REG_MODE_OUT                                          0x01
  // (ReadWrite, Bits 1) 
  // Indicates input mode: 0 - HDMI, 1 - MHL
  #define BIT_MSK__AIP_HDMI2MHL__REG_MODE_IN                                           0x02
  // (ReadWrite, Bits 2) 
  // enable to convert HBRA audio packet from HDMI to MHL or from MHL to HDMI
  #define BIT_MSK__AIP_HDMI2MHL__REG_HBAC_EN                                           0x04
  // (ReadWrite, Bits 7:3) 
  // bit #3 - if 1 then output from AFIFO will be regestered bit #4 - if 1 then logic will identify HBRA packet ID based on the mode, since in HDMI vs MHL HBRA has different header. Otherwise logic will set internal HBRA flag if header is 0x09 (HDMI mode)
  #define BIT_MSK__AIP_HDMI2MHL__REG_AFIFO_TEST                                        0xF8

// Audio In TDM Control Register
#define REG_ADDR__TDM_CTRL                                               (Page_A | 0x002E)
  // (ReadWrite, Bits 0) 
  // TDM enable
  #define BIT_MSK__TDM_CTRL__REG_TDM_EN                                            0x01
  // (ReadWrite, Bits 2:1) 
  // TDM Channel select: 00: 2 channel input (no clock divider) 01: 4 channel input (/2 clock divider) 10: 6 channel input (/3 clock divider) 11: 8 channel input (/4 clock divider)
  #define BIT_MSK__TDM_CTRL__REG_TDM_CH                                            0x06
  // (ReadWrite, Bits 3) 
  // Sampling edge of the TDM data: 1 means that TDM data will be sampled on neg edge
  #define BIT_MSK__TDM_CTRL__REG_TDM_CK_PHASE                                      0x08
  // (ReadWrite, Bits 4) 
  // TDM Incoming Data Block Size 0: 32-bit (default) 1: 16-bit
  #define BIT_MSK__TDM_CTRL__REG_TDM_16_BIT_BLK                                    0x10
  // (ReadWrite, Bits 5) 
  // Select on which edge of the WS to shift in TDM data sample 0: Disabled, i.e. use posedge (default) 1: Enabled, i.e. use negedge
  #define BIT_MSK__TDM_CTRL__REG_TDM_WS_NEGEDGE                                    0x20
  // (ReadWrite, Bits 7:6) 
  // Number of cycles between incoming WS/FS and data. Zero means the data is valid starting from the same cycle WS is active 00: 0 cycle 01: 1 cycle 10: 2 cycles 11: 3 cycles
  #define BIT_MSK__TDM_CTRL__REG_TDM_WS_IN_DELAY                                   0xC0

// Audio PATH Control Register
#define REG_ADDR__AUDP_TXCTRL                                            (Page_A | 0x002F)
  // (ReadWrite, Bits 1) 
  // HDMI Audio Packet layout indicator: 0 - Layout 0 (2-channel) (default) 1 - Layout 1 (Up to 8-channel)
  #define BIT_MSK__AUDP_TXCTRL__REG_LAYOUT                                            0x02
  // (ReadWrite, Bits 7) 
  // Mute Audio in means of no data from interfaces inserted into FIFOs controlled by TPI
  #define BIT_MSK__AUDP_TXCTRL__REG_AUD_MUTE_EN                                       0x80

// Audio PATH Tx FIFO Read Write ptr difference Register
#define REG_ADDR__AUDP_TXFIFO                                            (Page_A | 0x0031)
  // (ReadOnly, Bits 5:0) 
  // Difference between read and write pointers
  #define BIT_MSK__AUDP_TXFIFO__HDMI_FIFO_DIFF                                        0x3F

// Reg Access Time-out Register
#define REG_ADDR__AIP_REG_ACC_TO                                         (Page_A | 0x0032)
  // (ReadWrite, Bits 7:0) 
  // Register access time-out value bits 9:2. LSB tight to 0. If client does not respond within this time frame then flag will be set.
  #define BIT_MSK__AIP_REG_ACC_TO__REG_TO_LIMIT                                          0xFF

// TDM Channel data mapping Register
#define REG_ADDR__AIP_TDM_CH_MAP                                         (Page_A | 0x0033)
  // (ReadWrite, Bits 1:0) 
  // Map which TDM data will go out of SD0:  00 - sample 0 and 1 01 - sample 2 and 3 10 - sample 4 and 5 11 - sample 6 and 7
  #define BIT_MSK__AIP_TDM_CH_MAP__REG_TDM_CH0_MAP                                       0x03
  // (ReadWrite, Bits 3:2) 
  // Map which TDM data will go out of SD1:  00 - sample 0 and 1 01 - sample 2 and 3 10 - sample 4 and 5 11 - sample 6 and 7
  #define BIT_MSK__AIP_TDM_CH_MAP__REG_TDM_CH1_MAP                                       0x0C
  // (ReadWrite, Bits 5:4) 
  // Map which TDM data will go out of SD2:  00 - sample 0 and 1 01 - sample 2 and 3 10 - sample 4 and 5 11 - sample 6 and 7
  #define BIT_MSK__AIP_TDM_CH_MAP__REG_TDM_CH2_MAP                                       0x30
  // (ReadWrite, Bits 7:6) 
  // Map which TDM data will go out of SD3:  00 - sample 0 and 1 01 - sample 2 and 3 10 - sample 4 and 5 11 - sample 6 and 7
  #define BIT_MSK__AIP_TDM_CH_MAP__REG_TDM_CH3_MAP                                       0xC0

// Interrupt State Register
#define REG_ADDR__AIP_INTR_STATE                                         (Page_A | 0x004E)
  // (ReadOnly, Bits 0) 
  // Interrupt state bit value.  It shows whether the interrupt is active or not.  It is one gate before the polarity is applied to the interrupt.  In other words; whenever the interrupt is asserted; this bit is high.
  #define BIT_MSK__AIP_INTR_STATE__INTR_STATE                                            0x01
  // (ReadOnly, Bits 1) 
  // status of the interrupt from ADAM
  #define BIT_MSK__AIP_INTR_STATE__O_ADMA_INTR                                           0x02

// Interrupt Source #1 Register
#define REG_ADDR__AIP_INTR1                                              (Page_A | 0x004F)
  // (ReadWrite, Bits 0) 
  // Audio FIFO underflow. Asserted if set to 1. Writing 1 into this bit would clear it. '0' - wont change anything
  #define BIT_MSK__AIP_INTR1__REG_INTR1_STAT0                                       0x01
  // (ReadWrite, Bits 1) 
  // Audio FIFO overflow. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__AIP_INTR1__REG_INTR1_STAT1                                       0x02
  // (ReadWrite, Bits 3) 
  // Input SPDIF stream had bi-phase error. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__AIP_INTR1__REG_INTR1_STAT3                                       0x08
  // (ReadWrite, Bits 4) 
  // New preamble forced to drop sample. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__AIP_INTR1__REG_INTR1_STAT4                                       0x10

// Interrupt Source #2 Register
#define REG_ADDR__AIP_INTR2                                              (Page_A | 0x0050)
  // (ReadWrite, Bits 0) 
  // DSD stream got invalid sequence: more then 24 bits of the same value. Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__AIP_INTR2__REG_INTR2_STAT0                                       0x01
  // (ReadWrite, Bits 2) 
  // ACR packet overwrite occurred.  Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__AIP_INTR2__REG_INTR2_STAT2                                       0x04
  // (ReadWrite, Bits 3) 
  // ACR CTS Changed.  Asserted if set to 1. Write 1 to clear
  #define BIT_MSK__AIP_INTR2__REG_INTR2_STAT3                                       0x08
  // (ReadWrite, Bits 4) 
  // Did not found expected preamble. Asserted if set to 1. Write '1' to clear this bit.
  #define BIT_MSK__AIP_INTR2__REG_INTR2_STAT4                                       0x10
  // (ReadWrite, Bits 6) 
  // SPDIF parity error. Write 1 to clear
  #define BIT_MSK__AIP_INTR2__REG_INTR2_STAT6                                       0x40

// Interrupt #1 Mask Register
#define REG_ADDR__AIP_INTR1_MASK                                         (Page_A | 0x0051)
  // (ReadWrite, Bits 0) 
  // Enable INT1[0]: 1 - enable; 0 - disable (defualt)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK0                                       0x01
  // (ReadWrite, Bits 1) 
  // Enable INT1[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK1                                       0x02
  // (ReadWrite, Bits 2) 
  // Enable INT1[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK2                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable INT1[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK3                                       0x08
  // (ReadWrite, Bits 4) 
  // Enable INT1[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK4                                       0x10
  // (ReadWrite, Bits 5) 
  // Enable INT1[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INT1[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INT1[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR1_MASK__REG_INTR1_MASK7                                       0x80

// Interrupt #2 Mask Register
#define REG_ADDR__AIP_INTR2_MASK                                         (Page_A | 0x0052)
  // (ReadWrite, Bits 0) 
  // Enable INTR2[0]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK0                                       0x01
  // (ReadWrite, Bits 1) 
  // Enable INTR2[1]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK1                                       0x02
  // (ReadWrite, Bits 2) 
  // Enable INTR2[2]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK2                                       0x04
  // (ReadWrite, Bits 3) 
  // Enable INTR2[3]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK3                                       0x08
  // (ReadWrite, Bits 4) 
  // Enable INTR2[4]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK4                                       0x10
  // (ReadWrite, Bits 5) 
  // Enable INTR2[5]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK5                                       0x20
  // (ReadWrite, Bits 6) 
  // Enable INTR2[6]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK6                                       0x40
  // (ReadWrite, Bits 7) 
  // Enable INTR2[7]: 1 - enable; 0 - disable (default)
  #define BIT_MSK__AIP_INTR2_MASK__REG_INTR2_MASK7                                       0x80

// TPI Audio Down Sample Control Register
#define REG_ADDR__TPI_DOWN_SMPL_CTRL                                     (Page_A | 0x0061)
  // (ReadWrite, Bits 1:0) 
  // S/PDIF Audio Handling. 0b00  Block any audio except audio indicating fs as 32  kHz 44.1 kHz or 48 kHz. 0b01  Same as 0b11 (default for Atmel) 0b10  Down sample incoming audio as needed fs = 32 kHz -] fs = 32 kHz fs = 44.1 kHz -] fs = 44.1 kHz fs = 48 kHz -] fs = 48 kHz fs = 88.2 kHz -] fs = 44.1 kHz fs = 96 kHz -] fs = 48 kHz fs = 176.4 kHz -] fs = 44.1 kHz fs = 192 kHz -] fs = 48 kHz Note: This register will not affect NonPCM streams Note: fS value in the audio status channel is corrected automatically. 0b11  Pass any audio stream without fs modification. (default for SST)  I2S Audio Handling. 0b00 0b01 0b11 - Pass any audio stream without fs modification. 0b10  Down sample incoming audio as needed fs = 32 kHz -] fs = 32 kHz fs = 44.1 kHz -] fs = 44.1 kHz fs = 48 kHz -] fs = 48 kHz fs = 88.2 kHz -] fs = 44.1 kHz fs = 96 kHz -] fs = 48 kHz fs = 176.4 kHz -] fs = 44.1 kHz fs = 192 kHz -] fs = 48 kHz
  #define BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUD_HNDL                                      0x03
  // (ReadWrite, Bits 2) 
  // TPI Audio Lookup Tables Enable 0  Not Enabled 1  Enabled (default)
  #define BIT_MSK__TPI_DOWN_SMPL_CTRL__REG_TPI_AUDIO_LOOKUP_EN                               0x04

// TPI Audio Config Register
#define REG_ADDR__TPI_AUD_CONFIG                                         (Page_A | 0x0062)
  // (ReadWrite, Bits 4) 
  // 0  Normal (clears reg_aud_mute_en) 1  Mute (set reg_aud_mute_en)
  #define BIT_MSK__TPI_AUD_CONFIG__REG_TPI_AUD_MUTE                                      0x10
  // (ReadWrite, Bits 5) 
  // reserved
  #define BIT_MSK__TPI_AUD_CONFIG__RSVD                                                  0x20
  // (ReadWrite, Bits 7:6) 
  // 00=Refer to Stream Header 01=16-bit 10=20-bit 11=24-bit
  #define BIT_MSK__TPI_AUD_CONFIG__REG_TPI_SPDIF_SAMPLE_SIZE                             0xC0

// TPI Audio Sample Size/Frequency Register
#define REG_ADDR__TPI_AUD_FS                                             (Page_A | 0x0063)
  // (ReadWrite, Bits 5:0) 
  // Audio Sample Frequency hbra_on, aud_sample_freq[5:0];  Audio Rate 1b0, 2b00, 4'b0100; 22.05 kHz (n/a) 1b0, 2b00, 4'b0000; 44.1 kHz 1b0, 2b00, 4'b1000; 88.2 kHz 1b0, 2b00, 4'b1100; 176.4 kHz 1b0, 2b00, 4b1101; 352.8 kHz 1b0, 2b10, 4b1101; 705.6 kHz 1b0, 2b01, 4b1101; 1411.2 kHz (n/a) 1b0, 2b00, 4'b0110; 24 kHz (n/a) 1b0, 2b00, 4'b0010; 48 kHz 1b0, 2b00, 4'b1010; 96 kHz 1b0, 2b00, 4'b1110; 192 kHz 1b0, 2b00; 4b0101; 384 kHz 1b0, 2b00, 4b1001; 768 kHz 1b0, 2b01; 4b0101; 1536 kHz (n/a) 1b0, 2b00, 4'b0011; 32 kHz 1b0, 2b00, 4b1011; 64 kHz 1b0, 2b10, 4b1011; 128 kHz 1b0, 2b01, 4b1011; 256 kHz 1b0, 2b11, 4b1011; 512 kHz 1b0, 2b11; 4b0101; 1024 kHz (n/a) 1b1, 2b00, 4'b1100; HBRA 176.4 kHz 1b1, 2b00, 4'b1101; HBRA 352.8 kHz 1b1, 2b10, 4'b1101; HBRA 705.6 kHz 1b1, 2b01, 4b1101; HBRA 1411.2 kHz 1b1, 2b00, 4'b1110; HBRA 192 kHz 1b1, 2b00, 4'b0101; HBRA 384 kHz 1b1, 2b00, 4'b1001; HBRA 768 kHz 1b1, 2b01; 4b0101; HBRA 1536 kHz 1b1, 2b10, 4'b1011; HBRA 128 kHz 1b1, 2b01, 4b1011; HBRA 256 kHz 1b1, 2b11, 4b1011; HBRA 512 kHz 1b1, 2b11, 4b0101; HBRA 1024 kHz
  #define BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_B5_B0                                  0x3F
  // (ReadWrite, Bits 7) 
  // Enable for the TPI Sample Frequency override 1 - Use value in reg_tpi_aud_sf 0x63[5:0] 0 - Use incoming stream
  #define BIT_MSK__TPI_AUD_FS__REG_TPI_AUD_SF_OVRD                                   0x80

//***************************************************************************
// page 3F. Address: 40
// CEC Device ID Register
#define REG_ADDR__CEC_DEVICE_ID                                          (Page_F | 0x0080)
  // (ReadOnly, Bits 7:0) 
  // Device ID  0xCC (CEC)
  #define BIT_MSK__CEC_DEVICE_ID__CEC_DEVICEID                                          0xFF

// CEC Spec Register
#define REG_ADDR__CEC_SPEC                                               (Page_F | 0x0081)
  // (ReadOnly, Bits 3:0) 
  // CEC spec minor release 
  #define BIT_MSK__CEC_SPEC__CEC_REVISION                                          0x0F
  // (ReadOnly, Bits 7:4) 
  // CEC spec major release 
  #define BIT_MSK__CEC_SPEC__CEC_RELEASE                                           0xF0

// CEC Spec suffix Register
#define REG_ADDR__CEC_SPEC_SUFFIX                                        (Page_F | 0x0082)
  // (ReadOnly, Bits 3:0) 
  // CEC spec suffix  0x0 (= a; for rev 1.2a)
  #define BIT_MSK__CEC_SPEC_SUFFIX__CEC_SPEC_SUFFIX                                       0x0F
  // (ReadOnly, Bits 7) 
  // Subsytem 0 = Firmware 1 = Hardware
  #define BIT_MSK__CEC_SPEC_SUFFIX__CEC_SUBSYSTEM                                         0x80

// CEC Firmware Rev Register (CPI Revision)
#define REG_ADDR__CEC_HARDWARE_REV                                       (Page_F | 0x0083)
  // (ReadOnly, Bits 7:0) 
  // Hardware Revision
  #define BIT_MSK__CEC_HARDWARE_REV__CEC_HARD_REVISION                                     0xFF

// CEC Debug 0 Register
#define REG_ADDR__CEC_DEBUG_0                                            (Page_F | 0x0084)
  // (ReadOnly, Bits 7:0) 
  // Start Bit Low Period.  Measured in units of 250us.  (RO) Expected range is 3.5ms (0x0E)  to 3.9ms (0x0F).
  #define BIT_MSK__CEC_DEBUG_0__CEC_START_LOW_PRIOD                                   0xFF

// CEC Debug 1 Register
#define REG_ADDR__CEC_DEBUG_1                                            (Page_F | 0x0085)
  // (ReadOnly, Bits 7:0) 
  // Start Bit Duration Period.  Measured in units of 250us.  (RO) Expected range is 4.3ms (0x11)  to 4.7ms (0x12).
  #define BIT_MSK__CEC_DEBUG_1__CEC_START_BIT_PRIOD                                   0xFF

// CEC Debug 2 Register
#define REG_ADDR__CEC_DEBUG_2                                            (Page_F | 0x0086)
  // (ReadOnly, Bits 3:0) 
  // Current CEC bus owner (RO)   Values 0000 to 1111.
  #define BIT_MSK__CEC_DEBUG_2__CEC_BUS_OWNER                                         0x0F
  // (ReadWrite, Bits 7:4) 
  // CEC Snoop Initiator (RW)    Values 0000 to 1111.
  #define BIT_MSK__CEC_DEBUG_2__CEC_SNOOP_INITIATOR                                   0xF0

// CEC Debug 3 Register
#define REG_ADDR__CEC_DEBUG_3                                            (Page_F | 0x0087)
  // (ReadWrite, Bits 0) 
  // CEC snoop 0 = disable 1 = enable
  #define BIT_MSK__CEC_DEBUG_3__CEC_REG_SNOOP                                         0x01
  // (ReadWrite, Bits 1) 
  // ACK/NACK Header Block 0 = ACK 1 = NACK
  #define BIT_MSK__CEC_DEBUG_3__CEC_REG_NACK_HDR                                      0x02
  // (ReadWrite, Bits 2) 
  // Invert ACK to Broadcast Commands 0 = No 1 = Yes
  #define BIT_MSK__CEC_DEBUG_3__CEC_INV_ACK_BRCST                                     0x04
  // (ReadOnly, Bits 6:4) 
  // Frame Retransmit Count Values 0 to 5
  #define BIT_MSK__CEC_DEBUG_3__CEC_CTL_RETRY_CNT                                     0x70
  // (ReadWrite, Bits 7) 
  // Flush Tx FIFO 0=No 1=Yes Self resetting bit
  #define BIT_MSK__CEC_DEBUG_3__CEC_REG_FLUSH_TX_FF                                   0x80

// CEC_TX_INIT Register
#define REG_ADDR__CEC_TX_INIT                                            (Page_F | 0x0088)
  // (ReadWrite, Bits 3:0) 
  // CEC_INIT_ID [3:0] CEC Initiator ID  needs to be written to identify this device in future transmissions.
  #define BIT_MSK__CEC_TX_INIT__CUR_CEC_INIT_ID                                       0x0F

// CEC_TX_DEST Register
#define REG_ADDR__CEC_TX_DEST                                            (Page_F | 0x0089)
  // (ReadWrite, Bits 3:0) 
  // CEC_DEST_ID [3:0] CEC Destination ID  identifies the target device for the command.  Must be written before writing the corresponding CEC command.
  #define BIT_MSK__CEC_TX_DEST__CUR_READ_INIT_ID                                      0x0F
  // (ReadWrite, Bits 7) 
  // SEND_ POLL Generate a polling message 0=No 1=yes Self resetting bit
  #define BIT_MSK__CEC_TX_DEST__CEC_REG_SD_POLL_INTERN                                0x80

// CEC CPI Config Register
#define REG_ADDR__CEC_CONFIG_CPI                                         (Page_F | 0x008E)
  // (ReadWrite, Bits 0) 
  // CEC calibration start
  #define BIT_MSK__CEC_CONFIG_CPI__CEC_REG_CALIB_CEC                                     0x01
  // (ReadWrite, Bits 1) 
  // CEC calibration enable; self clear next cycle
  #define BIT_MSK__CEC_CONFIG_CPI__CEC_REG_CALIB_CEC_EN                                  0x02
  // (ReadWrite, Bits 2) 
  // Sil_Internal(Force CEC non-calibration mode. (default 1))
  #define BIT_MSK__CEC_CONFIG_CPI__CEC_REG_FORCE_NON_CALIB                               0x04
  // (ReadWrite, Bits 4) 
  // CEC pass through enable
  #define BIT_MSK__CEC_CONFIG_CPI__CEC_REG_I2C_CEC_PASSTHRU                              0x10

// CEC_TX_COMMAND Register
#define REG_ADDR__CEC_TX_COMMAND                                         (Page_F | 0x008F)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_COMMAND ( The Command and operands all are (total 16) considered as a single fifo entry and fifo has the depth of 2)
  #define BIT_MSK__CEC_TX_COMMAND__TX_FIFOX_B00                                          0xFF

// CEC_TX_OPERAND 0 Register
#define REG_ADDR__CEC_TX_OPERAND_0                                       (Page_F | 0x0090)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[0]
  #define BIT_MSK__CEC_TX_OPERAND_0__TX_FIFOX_B01                                          0xFF

// CEC_TX_OPERAND 1 Register
#define REG_ADDR__CEC_TX_OPERAND_1                                       (Page_F | 0x0091)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[1]
  #define BIT_MSK__CEC_TX_OPERAND_1__TX_FIFOX_B02                                          0xFF

// CEC_TX_OPERAND 2 Register
#define REG_ADDR__CEC_TX_OPERAND_2                                       (Page_F | 0x0092)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[2]
  #define BIT_MSK__CEC_TX_OPERAND_2__TX_FIFOX_B03                                          0xFF

// CEC_TX_OPERAND 3 Register
#define REG_ADDR__CEC_TX_OPERAND_3                                       (Page_F | 0x0093)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[3]
  #define BIT_MSK__CEC_TX_OPERAND_3__TX_FIFOX_B04                                          0xFF

// CEC_TX_OPERAND 4 Register
#define REG_ADDR__CEC_TX_OPERAND_4                                       (Page_F | 0x0094)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[4]
  #define BIT_MSK__CEC_TX_OPERAND_4__TX_FIFOX_B05                                          0xFF

// CEC_TX_OPERAND 5 Register
#define REG_ADDR__CEC_TX_OPERAND_5                                       (Page_F | 0x0095)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[5]
  #define BIT_MSK__CEC_TX_OPERAND_5__TX_FIFOX_B06                                          0xFF

// CEC_TX_OPERAND 6 Register
#define REG_ADDR__CEC_TX_OPERAND_6                                       (Page_F | 0x0096)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[6]
  #define BIT_MSK__CEC_TX_OPERAND_6__TX_FIFOX_B07                                          0xFF

// CEC_TX_OPERAND 7 Register
#define REG_ADDR__CEC_TX_OPERAND_7                                       (Page_F | 0x0097)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[7]
  #define BIT_MSK__CEC_TX_OPERAND_7__TX_FIFOX_B08                                          0xFF

// CEC_TX_OPERAND 8 Register
#define REG_ADDR__CEC_TX_OPERAND_8                                       (Page_F | 0x0098)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[8]
  #define BIT_MSK__CEC_TX_OPERAND_8__TX_FIFOX_B09                                          0xFF

// CEC_TX_OPERAND 9 Register
#define REG_ADDR__CEC_TX_OPERAND_9                                       (Page_F | 0x0099)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[9]
  #define BIT_MSK__CEC_TX_OPERAND_9__TX_FIFOX_B10                                          0xFF

// CEC_TX_OPERAND 10 Register
#define REG_ADDR__CEC_TX_OPERAND_10                                      (Page_F | 0x009A)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[10]
  #define BIT_MSK__CEC_TX_OPERAND_10__TX_FIFOX_B11                                          0xFF

// CEC_TX_OPERAND 11 Register
#define REG_ADDR__CEC_TX_OPERAND_11                                      (Page_F | 0x009B)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[11]
  #define BIT_MSK__CEC_TX_OPERAND_11__TX_FIFOX_B12                                          0xFF

// CEC_TX_OPERAND 12 Register
#define REG_ADDR__CEC_TX_OPERAND_12                                      (Page_F | 0x009C)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[12]
  #define BIT_MSK__CEC_TX_OPERAND_12__TX_FIFOX_B13                                          0xFF

// CEC_TX_OPERAND 13 Register
#define REG_ADDR__CEC_TX_OPERAND_13                                      (Page_F | 0x009D)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[13]
  #define BIT_MSK__CEC_TX_OPERAND_13__TX_FIFOX_B14                                          0xFF

// CEC_TX_OPERAND 14 Register
#define REG_ADDR__CEC_TX_OPERAND_14                                      (Page_F | 0x009E)
  // (ReadWrite, Bits 7:0) 
  // CEC_TX_OPERAND[14]
  #define BIT_MSK__CEC_TX_OPERAND_14__TX_FIFOX_B15                                          0xFF

// CEC Transmit Data Register
#define REG_ADDR__CEC_TRANSMIT_DATA                                      (Page_F | 0x009F)
  // (ReadWrite, Bits 3:0) 
  // TX_CNT [3:0]  Transmit Byte Count  selects the number of CEC_TX_OPERAND bytes to send with the command.  0000 = No operands 0001 = one operand .. 1111 = 15 operands
  #define BIT_MSK__CEC_TRANSMIT_DATA__CEC_REG_TX_CMD_CNT                                    0x0F
  // (WriteOnly, Bits 4) 
  // TRANSMIT_CEC_CMD Send CEC Command and TX_CNT Operands  (Write only) 0 = No 1 = Yes
  #define BIT_MSK__CEC_TRANSMIT_DATA__MANUAL_CMD_SET                                        0x10
  // (ReadWrite, Bits 5) 
  // TX_AUTO_CALC Auto-Calculate TX_CNT; and Send 0 = No 1 = Yes
  #define BIT_MSK__CEC_TRANSMIT_DATA__CEC_REG_TX_AUTO_CALC                                  0x20
  // (ReadWrite, Bits 6) 
  // TX_BFR_ACCESS  Read back internal buffer contents from 0x8F-0x9E  0 = No 1 = Yes
  #define BIT_MSK__CEC_TRANSMIT_DATA__CEC_REG_TX_BFR_AC                                     0x40

// CEC_Retry_Limit Register
#define REG_ADDR__CEC_RETRY_LIMIT                                        (Page_F | 0x00A0)
  // (ReadWrite, Bits 2:0) 
  // TX number of retry count limit when NAK received The number in this register is for how many more retries.  Ex. TX_RETRY_LIMIT =4 means total 5 times of transmission try on the bus.
  #define BIT_MSK__CEC_RETRY_LIMIT__CEC_REG_TX_RETRY_LIMIT                                0x07

// CEC_CAPTURE_ID0 Register
#define REG_ADDR__CEC_CAPTURE_ID0                                        (Page_F | 0x00A2)
  // (ReadWrite, Bits 7:0) 
  // The CEC Capture ID register is separate from the CEC Initiator ID.  It selects the received commands that will be captured in the receive FIFO and acknowledged.  If the command destination matches one of the bits set in this register or is a Broadcast cyc
  #define BIT_MSK__CEC_CAPTURE_ID0__CEC_REG_CAPTURE_ID_B7_B0                              0xFF

// CEC_CAPTURE_ID0 Register
#define REG_ADDR__CEC_CAPTURE_ID1                                        (Page_F | 0x00A3)
  // (ReadWrite, Bits 7:0) 
  // The CEC Capture ID register is separate from the CEC Initiator ID.  It selects the received commands that will be captured in the receive FIFO and acknowledged.  If the command destination matches one of the bits set in this register or is a Broadcast cyc
  #define BIT_MSK__CEC_CAPTURE_ID1__CEC_REG_CAPTURE_ID_B15_B8                             0xFF

// CEC_INT_ENABLE 0 Register
#define REG_ADDR__CEC_INT_ENABLE_0                                       (Page_F | 0x00A4)
  // (ReadWrite, Bits 0) 
  // Command Being Received event  0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_0__INTRP_RX_CMD_EN                                       0x01
  // (ReadWrite, Bits 1) 
  // Receiver FIFO Not Empty event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_0__INTRP_RX_FIFO_NEMPTY_EN                               0x02
  // (ReadWrite, Bits 2) 
  // Transmitter FIFO Empty event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_0__INTRP_TX_FIFO_EMPTY_EN                                0x04
  // (ReadWrite, Bits 5) 
  // Tx: Transmit Buffer Full/Empty Change event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_0__INTRP_TX_FF_CSTATE_EN                                 0x20
  // (ReadWrite, Bits 6) 
  // Tx: Transmit Buffer Full event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_0__INTRP_TX_FIFO_FULL_EN                                 0x40

// CEC_INT_ENABLE 1 Register
#define REG_ADDR__CEC_INT_ENABLE_1                                       (Page_F | 0x00A5)
  // (ReadWrite, Bits 0) 
  // Start Bit Irregularity event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_1__INTRP_START_ODD_BIT_EN                                0x01
  // (ReadWrite, Bits 1) 
  // Frame Retransmit Count Exceeded event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_1__INTRP_FRME_RETX_CNT_EN                                0x02
  // (ReadWrite, Bits 2) 
  // Short Pulse Detected event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_1__INTRP_SHRT_PULSE_DT_EN                                0x04
  // (ReadWrite, Bits 3) 
  // Rx FIFO Overrun Error event 0=Disable 1=Enable
  #define BIT_MSK__CEC_INT_ENABLE_1__INTRP_RX_FIFO_OVRUN_EN                                0x08
  // (ReadWrite, Bits 4) 
  // intrp_low_bit_min_err_en: 1 enable
  #define BIT_MSK__CEC_INT_ENABLE_1__INTRP_LOW_BIT_MIN_ERR_EN                              0x10

// CEC_INT_STATUS 0 Register
#define REG_ADDR__CEC_INT_STATUS_0                                       (Page_F | 0x00A6)
  // (ReadWrite, Bits 0) 
  // Command Being Received event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_CMD                                          0x01
  // (ReadWrite, Bits 1) 
  // Receiver FIFO Not Empty event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_0__INTRP_RX_FIFO_NEMPTY                                  0x02
  // (ReadWrite, Bits 2) 
  // Transmitter FIFO Empty event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_EMPTY                                   0x04
  // (ReadOnly, Bits 4) 
  // CEC_INT_STATUS1 register interrupt event summary 0 = no 1 = yes
  #define BIT_MSK__CEC_INT_STATUS_0__CEC_INTR_A7_SUMMARY                                   0x10
  // (ReadWrite, Bits 5) 
  // Tx: Transmit Buffer Full/Empty Change event Pending  0=no 1=yes
  #define BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FF_CSTATE                                    0x20
  // (ReadWrite, Bits 6) 
  // Tx FIFO Transmit Buffer Full  0=No 1=Yes (RO)
  #define BIT_MSK__CEC_INT_STATUS_0__INTRP_TX_FIFO_FULL                                    0x40
  // (ReadOnly, Bits 7) 
  // CEC line current state (RO) 0 = Low 1 = High
  #define BIT_MSK__CEC_INT_STATUS_0__IO_CEC_AI_SYN                                         0x80

// CEC_INT_STATUS 1 Register
#define REG_ADDR__CEC_INT_STATUS_1                                       (Page_F | 0x00A7)
  // (ReadWrite, Bits 0) 
  // Start Bit Irregularity event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_1__INTRP_START_ODD_BIT                                   0x01
  // (ReadWrite, Bits 1) 
  // Frame Retransmit Count Exceeded event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_1__INTRP_FRME_RETX_CNT                                   0x02
  // (ReadWrite, Bits 2) 
  // Short Pulse Detected event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_1__INTRP_SHRT_PULSE_DT                                   0x04
  // (ReadWrite, Bits 3) 
  // Rx FIFO Overrun Error event pending 0=No 1=Yes
  #define BIT_MSK__CEC_INT_STATUS_1__INTRP_RX_FIFO_OVRUN                                   0x08
  // (ReadWrite, Bits 4) 
  // intrp_low_bit_min_err: 1 - yes
  #define BIT_MSK__CEC_INT_STATUS_1__INTRP_LOW_BIT_MIN_ERR                                 0x10

// CEC_RX_CONTROL Register
#define REG_ADDR__CEC_RX_CONTROL                                         (Page_F | 0x00AC)
  // (ReadWrite, Bits 0) 
  // Clear Current Frame from Rx FIFO. Write 1 to clear. Self-resetting bit
  #define BIT_MSK__CEC_RX_CONTROL__CEC_REG_RX_CLR_CUR_SET                                0x01
  // (ReadWrite, Bits 1) 
  // Clear All Frames from Rx FIFO. Write 1 to clear. Self-resetting bit
  #define BIT_MSK__CEC_RX_CONTROL__CEC_REG_RX_CLR_ALL                                    0x02
  // (ReadWrite, Bits 6) 
  // auto clear intr rx fifo mem empty
  #define BIT_MSK__CEC_RX_CONTROL__AUTO_CLEAR_INTRP_RX_FIFO_NEMPTY                       0x40
  // (WriteOnly, Bits 7) 
  // Write 1 to this bit clears all interrupt status bits. This is a self resetting bit. Reading this bit returns  0
  #define BIT_MSK__CEC_RX_CONTROL__INTRP_CLEAR_ALL                                       0x80

// CEC_RX_COUNT Register
#define REG_ADDR__CEC_RX_COUNT                                           (Page_F | 0x00AD)
  // (ReadOnly, Bits 3:0) 
  // CEC_RX_BYTE_CNT (RO) CEC Receive Byte Count  returns the number of operands in the current frame.
  #define BIT_MSK__CEC_RX_COUNT__CEC_REG_RX_CMD_BYTE_CNT                               0x0F
  // (ReadOnly, Bits 5:4) 
  // CEC_RX_CMD_CNT (RO) CEC Receive FIFO Frame Count  returns the number of frames awaiting reading in the FIFO; 0-3.
  #define BIT_MSK__CEC_RX_COUNT__CEC_REG_RX_FF_WR_SEL                                  0x30
  // (ReadOnly, Bits 7) 
  // RX_ ERROR Error associated with this message  0=No 1=yes 
  #define BIT_MSK__CEC_RX_COUNT__CEC_RX_ERROR                                          0x80

// CEC_RX_CMD_HEADER Register
#define REG_ADDR__CEC_RX_CMD_HEADER                                      (Page_F | 0x00AE)
  // (ReadOnly, Bits 3:0) 
  // CEC_RX_DEST [3:0] CEC Destination ID  identifies  the intended target of the current frame.
  #define BIT_MSK__CEC_RX_CMD_HEADER__CEC_REG_RX_CMD_HEADER_B3_B0                           0x0F
  // (ReadOnly, Bits 7:4) 
  // CEC_RX_INIT [3:0] CEC Initiator ID  identifies  the initiator of the current frame. 
  #define BIT_MSK__CEC_RX_CMD_HEADER__CEC_REG_RX_CMD_HEADER_B7_B4                           0xF0

// CEC_RX_COMMAND Register
#define REG_ADDR__CEC_RX_COMMAND                                         (Page_F | 0x00AF)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_COMMAND ( The Command and operands all are (total 16) considered as a single fifo entry and fifo has the depth of 3)
  #define BIT_MSK__CEC_RX_COMMAND__RX_FIFO0_B0                                           0xFF

// CEC_RX_OPERAND 0 Register
#define REG_ADDR__CEC_RX_OPERAND_0                                       (Page_F | 0x00B0)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[0]
  #define BIT_MSK__CEC_RX_OPERAND_0__RX_FIFO0_B1                                           0xFF

// CEC_RX_OPERAND 1 Register
#define REG_ADDR__CEC_RX_OPERAND_1                                       (Page_F | 0x00B1)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[1]
  #define BIT_MSK__CEC_RX_OPERAND_1__RX_FIFO0_B2                                           0xFF

// CEC_RX_OPERAND 2 Register
#define REG_ADDR__CEC_RX_OPERAND_2                                       (Page_F | 0x00B2)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[2]
  #define BIT_MSK__CEC_RX_OPERAND_2__RX_FIFO0_B3                                           0xFF

// CEC_RX_OPERAND 3 Register
#define REG_ADDR__CEC_RX_OPERAND_3                                       (Page_F | 0x00B3)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[3]
  #define BIT_MSK__CEC_RX_OPERAND_3__RX_FIFO0_B4                                           0xFF

// CEC_RX_OPERAND 4 Register
#define REG_ADDR__CEC_RX_OPERAND_4                                       (Page_F | 0x00B4)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[4]
  #define BIT_MSK__CEC_RX_OPERAND_4__RX_FIFO0_B5                                           0xFF

// CEC_RX_OPERAND 5 Register
#define REG_ADDR__CEC_RX_OPERAND_5                                       (Page_F | 0x00B5)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[5]
  #define BIT_MSK__CEC_RX_OPERAND_5__RX_FIFO0_B6                                           0xFF

// CEC_RX_OPERAND 6 Register
#define REG_ADDR__CEC_RX_OPERAND_6                                       (Page_F | 0x00B6)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[6]
  #define BIT_MSK__CEC_RX_OPERAND_6__RX_FIFO0_B7                                           0xFF

// CEC_RX_OPERAND 7 Register
#define REG_ADDR__CEC_RX_OPERAND_7                                       (Page_F | 0x00B7)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[7]
  #define BIT_MSK__CEC_RX_OPERAND_7__RX_FIFO0_B8                                           0xFF

// CEC_RX_OPERAND 8 Register
#define REG_ADDR__CEC_RX_OPERAND_8                                       (Page_F | 0x00B8)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[8]
  #define BIT_MSK__CEC_RX_OPERAND_8__RX_FIFO0_B9                                           0xFF

// CEC_RX_OPERAND 9 Register
#define REG_ADDR__CEC_RX_OPERAND_9                                       (Page_F | 0x00B9)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[9]
  #define BIT_MSK__CEC_RX_OPERAND_9__RX_FIFO0_B10                                          0xFF

// CEC_RX_OPERAND 10 Register
#define REG_ADDR__CEC_RX_OPERAND_10                                      (Page_F | 0x00BA)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[10]
  #define BIT_MSK__CEC_RX_OPERAND_10__RX_FIFO0_B11                                          0xFF

// CEC_RX_OPERAND 11 Register
#define REG_ADDR__CEC_RX_OPERAND_11                                      (Page_F | 0x00BB)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[11]
  #define BIT_MSK__CEC_RX_OPERAND_11__RX_FIFO0_B12                                          0xFF

// CEC_RX_OPERAND 12 Register
#define REG_ADDR__CEC_RX_OPERAND_12                                      (Page_F | 0x00BC)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[12]
  #define BIT_MSK__CEC_RX_OPERAND_12__RX_FIFO0_B13                                          0xFF

// CEC_RX_OPERAND 13 Register
#define REG_ADDR__CEC_RX_OPERAND_13                                      (Page_F | 0x00BD)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[13]
  #define BIT_MSK__CEC_RX_OPERAND_13__RX_FIFO0_B14                                          0xFF

// CEC_RX_OPERAND 14 Register
#define REG_ADDR__CEC_RX_OPERAND_14                                      (Page_F | 0x00BE)
  // (ReadOnly, Bits 7:0) 
  // CEC_RX_OPERAND[14]
  #define BIT_MSK__CEC_RX_OPERAND_14__RX_FIFO0_B15                                          0xFF

// CEC_OPCODE_ABORT 0 Register
#define REG_ADDR__CEC_OP_ABORT_0                                         (Page_F | 0x00C0)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x07 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x06 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x05 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x04 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x03 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x02 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x01 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x00 opcode received
  #define BIT_MSK__CEC_OP_ABORT_0__CEC_OP_ABORT_REG_00_B0                                0x01

// CEC_OPCODE_ABORT 1 Register
#define REG_ADDR__CEC_OP_ABORT_1                                         (Page_F | 0x00C1)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x0F opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x0E opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x0D opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x0C opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x0B opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x0A opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x09 opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x08 opcode received
  #define BIT_MSK__CEC_OP_ABORT_1__CEC_OP_ABORT_REG_01_B0                                0x01

// CEC_OPCODE_ABORT 2 Register
#define REG_ADDR__CEC_OP_ABORT_2                                         (Page_F | 0x00C2)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x17 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x16 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x15 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x14 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x13 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x12 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x11 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x10 opcode received
  #define BIT_MSK__CEC_OP_ABORT_2__CEC_OP_ABORT_REG_02_B0                                0x01

// CEC_OPCODE_ABORT 3 Register
#define REG_ADDR__CEC_OP_ABORT_3                                         (Page_F | 0x00C3)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x1F opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x1E opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x1D opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x1C opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x1B opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x1A opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x19 opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x18 opcode received
  #define BIT_MSK__CEC_OP_ABORT_3__CEC_OP_ABORT_REG_03_B0                                0x01

// CEC_OPCODE_ABORT 4 Register
#define REG_ADDR__CEC_OP_ABORT_4                                         (Page_F | 0x00C4)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x27 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x26 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x25 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x24 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x23 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x22 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x21 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x20 opcode received
  #define BIT_MSK__CEC_OP_ABORT_4__CEC_OP_ABORT_REG_04_B0                                0x01

// CEC_OPCODE_ABORT 5 Register
#define REG_ADDR__CEC_OP_ABORT_5                                         (Page_F | 0x00C5)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x2F opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x2E opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x2D opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x2C opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x2B opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x2A opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x29 opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x28 opcode received
  #define BIT_MSK__CEC_OP_ABORT_5__CEC_OP_ABORT_REG_05_B0                                0x01

// CEC_OPCODE_ABORT 6 Register
#define REG_ADDR__CEC_OP_ABORT_6                                         (Page_F | 0x00C6)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x37 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x36 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x35 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x34 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x33 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x32 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x31 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x30 opcode received
  #define BIT_MSK__CEC_OP_ABORT_6__CEC_OP_ABORT_REG_06_B0                                0x01

// CEC_OPCODE_ABORT 7 Register
#define REG_ADDR__CEC_OP_ABORT_7                                         (Page_F | 0x00C7)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x3F opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x3E opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x3D opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x3C opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x3B opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x3A opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x39 opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x38 opcode received
  #define BIT_MSK__CEC_OP_ABORT_7__CEC_OP_ABORT_REG_07_B0                                0x01

// CEC_OPCODE_ABORT 8 Register
#define REG_ADDR__CEC_OP_ABORT_8                                         (Page_F | 0x00C8)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x47 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x46 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x45 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x44 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x43 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x42 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x41 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x40 opcode received
  #define BIT_MSK__CEC_OP_ABORT_8__CEC_OP_ABORT_REG_08_B0                                0x01

// CEC_OPCODE_ABORT 9 Register
#define REG_ADDR__CEC_OP_ABORT_9                                         (Page_F | 0x00C9)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x4F opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x4E opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x4D opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x4C opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x4B opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x4A opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x49 opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x48 opcode received
  #define BIT_MSK__CEC_OP_ABORT_9__CEC_OP_ABORT_REG_09_B0                                0x01

// CEC_OPCODE_ABORT 10 Register
#define REG_ADDR__CEC_OP_ABORT_10                                        (Page_F | 0x00CA)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x57 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x56 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x55 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x54 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x53 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x52 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x51 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x50 opcode received
  #define BIT_MSK__CEC_OP_ABORT_10__CEC_OP_ABORT_REG_10_B0                                0x01

// CEC_OPCODE_ABORT 11 Register
#define REG_ADDR__CEC_OP_ABORT_11                                        (Page_F | 0x00CB)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x5F opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x5E opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x5D opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x5C opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x5B opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x5A opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x59 opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x58 opcode received
  #define BIT_MSK__CEC_OP_ABORT_11__CEC_OP_ABORT_REG_11_B0                                0x01

// CEC_OPCODE_ABORT 12 Register
#define REG_ADDR__CEC_OP_ABORT_12                                        (Page_F | 0x00CC)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x67 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x66 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x65 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x64 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x63 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x62 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x61 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x60 opcode received
  #define BIT_MSK__CEC_OP_ABORT_12__CEC_OP_ABORT_REG_12_B0                                0x01

// CEC_OPCODE_ABORT 13 Register
#define REG_ADDR__CEC_OP_ABORT_13                                        (Page_F | 0x00CD)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x6F opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x6E opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x6D opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x6C opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x6B opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x6A opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x69 opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x68 opcode received
  #define BIT_MSK__CEC_OP_ABORT_13__CEC_OP_ABORT_REG_13_B0                                0x01

// CEC_OPCODE_ABORT 14 Register
#define REG_ADDR__CEC_OP_ABORT_14                                        (Page_F | 0x00CE)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x77 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x76 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x75 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x74 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x73 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x72 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x71 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x70 opcode received
  #define BIT_MSK__CEC_OP_ABORT_14__CEC_OP_ABORT_REG_14_B0                                0x01

// CEC_OPCODE_ABORT 15 Register
#define REG_ADDR__CEC_OP_ABORT_15                                        (Page_F | 0x00CF)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x7F opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x7E opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x7D opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x7C opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x7B opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x7A opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x79 opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x78 opcode received
  #define BIT_MSK__CEC_OP_ABORT_15__CEC_OP_ABORT_REG_15_B0                                0x01

// CEC_OPCODE_ABORT 16 Register
#define REG_ADDR__CEC_OP_ABORT_16                                        (Page_F | 0x00D0)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x87 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x86 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x85 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x84 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x83 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x82 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x81 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x80 opcode received
  #define BIT_MSK__CEC_OP_ABORT_16__CEC_OP_ABORT_REG_16_B0                                0x01

// CEC_OPCODE_ABORT 17 Register
#define REG_ADDR__CEC_OP_ABORT_17                                        (Page_F | 0x00D1)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x8F opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x8E opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x8D opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x8C opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x8B opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x8A opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x89 opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x88 opcode received
  #define BIT_MSK__CEC_OP_ABORT_17__CEC_OP_ABORT_REG_17_B0                                0x01

// CEC_OPCODE_ABORT 18 Register
#define REG_ADDR__CEC_OP_ABORT_18                                        (Page_F | 0x00D2)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x97 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x96 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x95 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x94 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x93 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x92 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x91 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x90 opcode received
  #define BIT_MSK__CEC_OP_ABORT_18__CEC_OP_ABORT_REG_18_B0                                0x01

// CEC_OPCODE_ABORT 19 Register
#define REG_ADDR__CEC_OP_ABORT_19                                        (Page_F | 0x00D3)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0x9F opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0x9E opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0x9D opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0x9C opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0x9B opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0x9A opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0x99 opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0x98 opcode received
  #define BIT_MSK__CEC_OP_ABORT_19__CEC_OP_ABORT_REG_19_B0                                0x01

// CEC_OPCODE_ABORT 20 Register
#define REG_ADDR__CEC_OP_ABORT_20                                        (Page_F | 0x00D4)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xA7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xA6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xA5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xA4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xA3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xA2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xA1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xA0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_20__CEC_OP_ABORT_REG_20_B0                                0x01

// CEC_OPCODE_ABORT 21 Register
#define REG_ADDR__CEC_OP_ABORT_21                                        (Page_F | 0x00D5)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xAF opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xAE opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xAD opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xAC opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xAB opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xAA opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xA9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xA8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_21__CEC_OP_ABORT_REG_21_B0                                0x01

// CEC_OPCODE_ABORT 22 Register
#define REG_ADDR__CEC_OP_ABORT_22                                        (Page_F | 0x00D6)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xB7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xB6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xB5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xB4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xB3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xB2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xB1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xB0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_22__CEC_OP_ABORT_REG_22_B0                                0x01

// CEC_OPCODE_ABORT 23 Register
#define REG_ADDR__CEC_OP_ABORT_23                                        (Page_F | 0x00D7)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xBF opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xBE opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xBD opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xBC opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xBB opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xBA opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xB9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xB8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_23__CEC_OP_ABORT_REG_23_B0                                0x01

// CEC_OPCODE_ABORT 24 Register
#define REG_ADDR__CEC_OP_ABORT_24                                        (Page_F | 0x00D8)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xC7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xC6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xC5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xC4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xC3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xC2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xC1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xC0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_24__CEC_OP_ABORT_REG_24_B0                                0x01

// CEC_OPCODE_ABORT 25 Register
#define REG_ADDR__CEC_OP_ABORT_25                                        (Page_F | 0x00D9)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xCF opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xCE opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xCD opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xCC opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xCB opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xCA opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xC9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xC8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_25__CEC_OP_ABORT_REG_25_B0                                0x01

// CEC_OPCODE_ABORT 26 Register
#define REG_ADDR__CEC_OP_ABORT_26                                        (Page_F | 0x00DA)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xD7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xD6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xD5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xD4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xD3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xD2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xD1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xD0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_26__CEC_OP_ABORT_REG_26_B0                                0x01

// CEC_OPCODE_ABORT 27 Register
#define REG_ADDR__CEC_OP_ABORT_27                                        (Page_F | 0x00DB)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xDF opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xDE opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xDD opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xDC opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xDB opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xDA opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xD9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xD8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_27__CEC_OP_ABORT_REG_27_B0                                0x01

// CEC_OPCODE_ABORT 28 Register
#define REG_ADDR__CEC_OP_ABORT_28                                        (Page_F | 0x00DC)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xE7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xE6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xE5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xE4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xE3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xE2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xE1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xE0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_28__CEC_OP_ABORT_REG_28_B0                                0x01

// CEC_OPCODE_ABORT 29 Register
#define REG_ADDR__CEC_OP_ABORT_29                                        (Page_F | 0x00DD)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xEF opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xEE opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xED opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xEC opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xEB opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xEA opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xE9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xE8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_29__CEC_OP_ABORT_REG_29_B0                                0x01

// CEC_OPCODE_ABORT 30 Register
#define REG_ADDR__CEC_OP_ABORT_30                                        (Page_F | 0x00DE)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xF7 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xF6 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xF5 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xF4 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xF3 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xF2 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xF1 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xF0 opcode received
  #define BIT_MSK__CEC_OP_ABORT_30__CEC_OP_ABORT_REG_30_B0                                0x01

// CEC_OPCODE_ABORT 31 Register
#define REG_ADDR__CEC_OP_ABORT_31                                        (Page_F | 0x00DF)
  // (ReadWrite, Bits 7) 
  // Send automatic [Feature Abort] when 0xFF opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B7                                0x80
  // (ReadWrite, Bits 6) 
  // Send automatic [Feature Abort] when 0xFE opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B6                                0x40
  // (ReadWrite, Bits 5) 
  // Send automatic [Feature Abort] when 0xFD opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B5                                0x20
  // (ReadWrite, Bits 4) 
  // Send automatic [Feature Abort] when 0xFC opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B4                                0x10
  // (ReadWrite, Bits 3) 
  // Send automatic [Feature Abort] when 0xFB opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B3                                0x08
  // (ReadWrite, Bits 2) 
  // Send automatic [Feature Abort] when 0xFA opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B2                                0x04
  // (ReadWrite, Bits 1) 
  // Send automatic [Feature Abort] when 0xF9 opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B1                                0x02
  // (ReadWrite, Bits 0) 
  // Send automatic [Feature Abort] when 0xF8 opcode received
  #define BIT_MSK__CEC_OP_ABORT_31__CEC_OP_ABORT_REG_31_B0                                0x01

// CEC_10ms_COUNT_L Register
#define REG_ADDR__CEC_AUTO_DISCOVERY                                     (Page_F | 0x00E0)
  // (ReadWrite, Bits 0) 
  // Start Automatic PING Discovery Write 1 to clear
  #define BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_START                                   0x01
  // (ReadWrite, Bits 1) 
  // Clear Automaitc PING Disocvery Done State Write 1 to clear
  #define BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_CLEAR                                   0x02
  // (ReadOnly, Bits 7) 
  // Automatic PING Discovery Done
  #define BIT_MSK__CEC_AUTO_DISCOVERY__CEC_AUTO_PING_DONE                                    0x80

// CEC_10ms_COUNT_M Register
#define REG_ADDR__CEC_AUTODISC_MAP0                                      (Page_F | 0x00E1)
  // (ReadOnly, Bits 0) 
  // Logical address 0 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B0                                  0x01
  // (ReadOnly, Bits 1) 
  // Logical address 1 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B1                                  0x02
  // (ReadOnly, Bits 2) 
  // Logical address 2 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B2                                  0x04
  // (ReadOnly, Bits 3) 
  // Logical address 3 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B3                                  0x08
  // (ReadOnly, Bits 4) 
  // Logical address 4 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B4                                  0x10
  // (ReadOnly, Bits 5) 
  // Logical address 5 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B5                                  0x20
  // (ReadOnly, Bits 6) 
  // Logical address 6 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B6                                  0x40
  // (ReadOnly, Bits 7) 
  // Logical address 7 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP0__CEC_AUTO_PING_MAP_B7                                  0x80

// CEC_10ms_COUNT_L Register
#define REG_ADDR__CEC_AUTODISC_MAP1                                      (Page_F | 0x00E2)
  // (ReadOnly, Bits 0) 
  // Logical address 8 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B8                                  0x01
  // (ReadOnly, Bits 1) 
  // Logical address 9 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B9                                  0x02
  // (ReadOnly, Bits 2) 
  // Logical address 10 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B10                                 0x04
  // (ReadOnly, Bits 3) 
  // Logical address 11 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B11                                 0x08
  // (ReadOnly, Bits 4) 
  // Logical address 12 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B12                                 0x10
  // (ReadOnly, Bits 5) 
  // Logical address 13 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B13                                 0x20
  // (ReadOnly, Bits 6) 
  // Logical address 14 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B14                                 0x40
  // (ReadOnly, Bits 7) 
  // Logical address 15 discovered
  #define BIT_MSK__CEC_AUTODISC_MAP1__CEC_AUTO_PING_MAP_B15                                 0x80

// CEC CDC Arbitration Count and Enable Register
#define REG_ADDR__CEC_CDC_EN_CNT                                         (Page_F | 0x00E3)
  // (ReadWrite, Bits 4:0) 
  // CDC Arbitration Count, number of arbitration blocks after the CEC Header
  #define BIT_MSK__CEC_CDC_EN_CNT__CEC_CDC_ARB_CNT_B4_B0                                 0x1F
  // (ReadWrite, Bits 7) 
  // CDC Arbitration Enable
  #define BIT_MSK__CEC_CDC_EN_CNT__CEC_CDC_ARB_EN                                        0x80

// CEC CDC Opcode Register
#define REG_ADDR__CEC_CDC_OPCODE                                         (Page_F | 0x00E4)
  // (ReadWrite, Bits 7:0) 
  // CDC Opcode to indicate the CDC messages
  #define BIT_MSK__CEC_CDC_OPCODE__CEC_CDC_OPCODE                                        0xFF

  
  #define REG_ADDR__PHY_DATA_SWING                                                       0x7b3
  #define REG_ADDR__PHY_CLK_SWING                                                        0x7b4
  #define REG_ADDR__PHY_SRC_TERMINATION                                                  0x7b5
  #define REG_ADDR__PHY_NVB_VALUE                                                        0x7b6
  #define REG_ADDR__PHY_CLK_FINE_ADJUST                                                  0x7b7
  #define REG_ADDR__PHY_PLL_BAND                                                         0x7b9
  #define REG_ADDR__PHY_PLL_LOOP                                                         0x7ba 
  #define REG_ADDR__PHY_PLL_VCO                                                          0x7bc
  #define REG_ADDR__PHY_PLL_PI                                                           0x7be 
  #define REG_ADDR__PHY_BRG_BIAS                                                         0x7bf
  #define REG_ADDR__PHY_DATA_FINE_ADJUST                                                 0x7c9
  #define REG_ADDR__PHY_DATA_DRV_CTRL                                                    0x7dc
  #define REG_ADDR__PHY_PLL_LDO                                                          0x7e0
  #define REG_ADDR__PHY_PLL_ZONE                                                         0x7e2
  #define REG_ADDR__PHY_PLL_CHARGE                                                       0x7cb
  #define REG_ADDR__PHY_IN_BRG                                                           0x7c0
  #define REG_ADDR__PHY_TX_REG_ZONE                                                      0x222
  #define REG_ADDR__PHY_RISE_TIME                                                        0x7c7
  #define REG_ADDR__PHY_FALL_TIME                                                        0x7ca

  // pattern generator
  #define REG_ADDR__PATTERN_GENERATOR                                                    0x3e60
  // bit0 -- pattern generator tpg enable
  #define BIT_MASK_PATTERN_GENERATOR_TPG_ENABLE                                          0x01

#endif // __SI_DRV_TX_REGS_H__
