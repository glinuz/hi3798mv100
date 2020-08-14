//***************************************************************************
//! @file     New_Cypress2_VidPath_header.h
//! @brief    Silicon Image register definition header.
//
// No part of this work may be reproduced, modified, distributed, 
// transmitted, transcribed, or translated into any language or computer 
// format, in any form or by any means without written permission of 
// Silicon Image, Inc., 1060 East Arques Avenue, Sunnyvale, California 94085
//
// Copyright 2008-2014, Silicon Image, Inc.  All rights reserved.
//***************************************************************************/

#ifndef __SI_VIDPATH_REGS_H__
#define __SI_VIDPATH_REGS_H__

#include "si_datatypes.h"
#include "si_drv_cra_api.h"

void SiiModVideoPathRegRead(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t *ptrdata, size_t rSize);
void SiiModVideoPathRegWrite(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t *ptrdata, size_t rSize);

#define BASE_ADDRESS 0x3000

typedef enum 
{
    video_path_core_registers = (BASE_ADDRESS | 0x0B00),
    video_path_core_registers__cms_core_registers = (BASE_ADDRESS | 0x0C00),
    video_path_core_registers__cms_core_registers__csc_core_registers0 = (BASE_ADDRESS | 0x0D00),
    video_path_core_registers__cms_core_registers__csc_core_registers1 = (BASE_ADDRESS | 0x0D80),
}PageVideopath_t;

//***************************************************************************
// video_path_core_registers. Address: 40
// Module features
#define REG_ADDR__VP__FEATURES                                           (video_path_core_registers | 0x0000)
  // (ReadOnly, Bits 0) 
  // Video Path Core module
  #define BIT_MSK__VP__FEATURES__VIDEO_PATH_CORE                                       0x01
  // (ReadOnly, Bits 1) 
  // Input pin muxing
  #define BIT_MSK__VP__FEATURES__VI_MUXING                                             0x02
  // (ReadOnly, Bits 2) 
  // Input format detector
  #define BIT_MSK__VP__FEATURES__VI_FDET                                               0x04
  // (ReadOnly, Bits 3) 
  // Input sync polarity adjustment
  #define BIT_MSK__VP__FEATURES__VI_SYNC_ADJUST                                        0x08
  // (ReadOnly, Bits 4) 
  // Input rate conversion
  #define BIT_MSK__VP__FEATURES__VI_RATECONV                                           0x10
  // (ReadOnly, Bits 5) 
  // Retiming VTG/FIFO
  #define BIT_MSK__VP__FEATURES__VI_RETIMING                                           0x20
  // (ReadOnly, Bits 6) 
  // 656 Decoder
  #define BIT_MSK__VP__FEATURES__DEC656                                                0x40
  // (ReadOnly, Bits 7) 
  // DE Generator
  #define BIT_MSK__VP__FEATURES__DEGEN                                                 0x80
  // (ReadOnly, Bits 8) 
  // Pixel capture at the input
  #define BIT_MSK__VP__FEATURES__PIXCAP_IN                                             0x100
  // (ReadOnly, Bits 9) 
  // Chroma vertical upsampler
  #define BIT_MSK__VP__FEATURES__C420_C422                                             0x200
  // (ReadOnly, Bits 10) 
  // Chroma horizontal upsampler
  #define BIT_MSK__VP__FEATURES__C422_C444                                             0x400
  // (ReadOnly, Bits 11) 
  // Multi-Colorspace Converter #0
  #define BIT_MSK__VP__FEATURES__MULTI_CSC0                                            0x800
  // (ReadOnly, Bits 12) 
  // Pixel capture before CMS
  #define BIT_MSK__VP__FEATURES__PIXCAP_PRE                                            0x1000
  // (ReadOnly, Bits 13) 
  // PWLI #0
  #define BIT_MSK__VP__FEATURES__PWLI0                                                 0x2000
  // (ReadOnly, Bits 14) 
  // CMS Matrix
  #define BIT_MSK__VP__FEATURES__CMS_MATRIX                                            0x4000
  // (ReadOnly, Bits 15) 
  // PWLI #1
  #define BIT_MSK__VP__FEATURES__PWLI1                                                 0x8000
  // (ReadOnly, Bits 16) 
  // PWLI #2
  #define BIT_MSK__VP__FEATURES__PWLI2                                                 0x10000
  // (ReadOnly, Bits 17) 
  // Pixel capture after CMS
  #define BIT_MSK__VP__FEATURES__PIXCAP_POST                                           0x20000
  // (ReadOnly, Bits 18) 
  // Multi-Colorspace Converter #1
  #define BIT_MSK__VP__FEATURES__MULTI_CSC1                                            0x40000
  // (ReadOnly, Bits 19) 
  // Chroma horizontal subsampler
  #define BIT_MSK__VP__FEATURES__C444_C422                                             0x80000
  // (ReadOnly, Bits 20) 
  // Chroma vertical subsampler
  #define BIT_MSK__VP__FEATURES__C422_C420                                             0x100000
  // (ReadOnly, Bits 21) 
  // Dither/Round
  #define BIT_MSK__VP__FEATURES__DITHER_RND                                            0x200000
  // (ReadOnly, Bits 22) 
  // Range Clip
  #define BIT_MSK__VP__FEATURES__RANGE_CLIP                                            0x400000
  // (ReadOnly, Bits 23) 
  // Pixel capture at the output
  #define BIT_MSK__VP__FEATURES__PIXCAP_OUT                                            0x800000
  // (ReadOnly, Bits 24) 
  // Output rate conversion
  #define BIT_MSK__VP__FEATURES__VO_RATECONV                                           0x1000000
  // (ReadOnly, Bits 25) 
  // Output blanking control
  #define BIT_MSK__VP__FEATURES__VO_BLANK                                              0x2000000
  // (ReadOnly, Bits 26) 
  // Output embedded sync encoder
  #define BIT_MSK__VP__FEATURES__VO_EMBD_SYNC_ENC                                      0x4000000
  // (ReadOnly, Bits 27) 
  // Output pin muxing
  #define BIT_MSK__VP__FEATURES__VO_MUXING                                             0x8000000

// Device build time stamp
#define REG_ADDR__VP__BUILD_TIME                                         (video_path_core_registers | 0x0008)

// Software reset
#define REG_ADDR__VP__SOFT_RESET                                         (video_path_core_registers | 0x000c)
  // (ReadWrite, Bits 0) 
  // Video Input software reset for frontend only (clk_in domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_IN                                          0x01
  // (ReadWrite, Bits 1) 
  // Video Input software reset for core only (clk_core domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_CORE                                        0x02
  // (ReadWrite, Bits 2) 
  // Video Input software reset for backend only (clk_out domain)
  #define BIT_MSK__VP__SOFT_RESET__RESET_CLK_OUT                                         0x04

// Number of data bits for the datapath
#define REG_ADDR__VP__DATA_BITS_VALUE                                    (video_path_core_registers | 0x000e)

// Input muting
#define REG_ADDR__VP__INPUT_MUTE                                         (video_path_core_registers | 0x0010)
  // (ReadWrite, Bits 0) 
  // Disable vsync input
  #define BIT_MSK__VP__INPUT_MUTE__VSYNC_DISABLE                                         0x01
  // (ReadWrite, Bits 1) 
  // Disable hsync input
  #define BIT_MSK__VP__INPUT_MUTE__HSYNC_DISABLE                                         0x02
  // (ReadWrite, Bits 2) 
  // Disable DE input
  #define BIT_MSK__VP__INPUT_MUTE__DE_DISABLE                                            0x04
  // (ReadWrite, Bits 3) 
  // Disable Y input pins
  #define BIT_MSK__VP__INPUT_MUTE__Y_DISABLE                                             0x08
  // (ReadWrite, Bits 4) 
  // Disable Cb input pins
  #define BIT_MSK__VP__INPUT_MUTE__CB_DISABLE                                            0x10
  // (ReadWrite, Bits 5) 
  // Disable Cr input pins
  #define BIT_MSK__VP__INPUT_MUTE__CR_DISABLE                                            0x20

// Input sync configuration
#define REG_ADDR__VP__INPUT_SYNC_CONFIG                                  (video_path_core_registers | 0x0012)
  // (ReadWrite, Bits 0) 
  // Select VSYNC polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__VSYNC_POLARITY                                        0x01
  // (ReadWrite, Bits 1) 
  // Select HSYNC polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__HSYNC_POLARITY                                        0x02
  // (ReadWrite, Bits 2) 
  // Select DE polarity
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__DE_POLARITY                                           0x04
  // (ReadWrite, Bits 3) 
  // DE input is FIELD signal (requires DE generator to be enabled!)
  #define BIT_MSK__VP__INPUT_SYNC_CONFIG__DE_IS_FIELD                                           0x08

// Input Format Selection
#define REG_ADDR__VP__INPUT_FORMAT                                       (video_path_core_registers | 0x0014)
  // (ReadWrite, Bits 0) 
  // Strobe data on both posedge and negedge of input clock
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_ENABLE                                            0x01
  // (ReadWrite, Bits 1) 
  // Select polarity of DDR decoder
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_POLARITY                                          0x02
  // (ReadWrite, Bits 4:2) 
  // Select DDR mode
  #define BIT_MSK__VP__INPUT_FORMAT__DDR_MODE                                              0x1C
  // (ReadWrite, Bits 5) 
  // Enable Y/C demux logic
  #define BIT_MSK__VP__INPUT_FORMAT__YC_DEMUX_ENABLE                                       0x20
  // (ReadWrite, Bits 6) 
  // Select polarity of Y/C demux logic
  #define BIT_MSK__VP__INPUT_FORMAT__YC_DEMUX_POLARITY                                     0x40
  // (ReadWrite, Bits 7) 
  // Select Cb/Cr order for muxed signals
  #define BIT_MSK__VP__INPUT_FORMAT__CBCR_ORDER                                            0x80
  // (ReadWrite, Bits 9:8) 
  // Remove replicated pixels
  #define BIT_MSK__VP__INPUT_FORMAT__PIXEL_RATE                                            0x300
  // (ReadWrite, Bits 10) 
  // Enable 4:2:0 Y mux
  #define BIT_MSK__VP__INPUT_FORMAT__MUX_420_ENABLE                                        0x400
  // (ReadWrite, Bits 11) 
  // Select whether Cb or Cr bus are used as chroma input
  #define BIT_MSK__VP__INPUT_FORMAT__MUX_CB_OR_CR                                          0x800
	  
// Input pin mapping configuration
#define REG_ADDR__VP__INPUT_MAPPING                                      (video_path_core_registers | 0x0016)
  // (ReadWrite, Bits 0) 
  // Reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_Y                                             0x01
  // (ReadWrite, Bits 1) 
  // Reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_CB                                            0x02
  // (ReadWrite, Bits 2) 
  // Reverse input pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__INPUT_MAPPING__REVERSE_CR                                            0x04
  // (ReadWrite, Bits 5:3) 
  // Select input pins for internal Y datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_Y                                              0x38
  // (ReadWrite, Bits 8:6) 
  // Select input pins for internal Cb datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_CB                                             0x1C0
  // (ReadWrite, Bits 11:9) 
  // Select input pins for internal Cr datapath
  #define BIT_MSK__VP__INPUT_MAPPING__SELECT_CR                                             0xE00

// Input mask
#define REG_ADDR__VP__INPUT_MASK                                         (video_path_core_registers | 0x0018)
  // (ReadWrite, Bits 1:0) 
  // Disable LSBs on Y datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_Y                                        0x03
  // (ReadWrite, Bits 3:2) 
  // Disable LSBs on Cb datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_CB                                       0x0C
  // (ReadWrite, Bits 5:4) 
  // Disable LSBs on Cr datapath
  #define BIT_MSK__VP__INPUT_MASK__DISABLE_LSBS_CR                                       0x30

// Configure sync polarity adjustment for datapath #0
#define REG_ADDR__VP__INPUT_SYNC_ADJUST_CONFIG                           (video_path_core_registers | 0x001c)
  // (ReadWrite, Bits 0) 
  // Configure sync polarity adjustment for datapath #0
  #define BIT_MSK__VP__INPUT_SYNC_ADJUST_CONFIG__AUTO_DISABLE                                          0x01
 
// DE Generator configuration
#define REG_ADDR__VP__DEGEN_CONFIG                                       (video_path_core_registers | 0x0020)
  // (ReadWrite, Bits 0) 
  // Enable DE generator
  #define BIT_MSK__VP__DEGEN_CONFIG__ENABLE                                                0x01
  
// Number of clocks from start of HSYNC before generated DE
#define REG_ADDR__VP__DEGEN_PIXEL_DELAY                                  (video_path_core_registers | 0x0022)

// Count (per line) for DE to be active (0=1 clk period, 1=2 clk periods, etc.)
#define REG_ADDR__VP__DEGEN_PIXEL_COUNT_MINUS_ONE                        (video_path_core_registers | 0x0024)

// Number of lines from start of VSYNC before generated DE minus one (0=1 line, 1=2 lines, etc.)
#define REG_ADDR__VP__DEGEN_LINE_DELAY                                   (video_path_core_registers | 0x0026)

// Number of lines of generated DE
#define REG_ADDR__VP__DEGEN_LINE_COUNT                                   (video_path_core_registers | 0x0028)

// 656 decoder configuration
#define REG_ADDR__VP__DEC656_CONFIG                                      (video_path_core_registers | 0x0030)
  // (ReadWrite, Bits 0) 
  // Search for embedded 656 syncs for DE, hsync, vsync generation
  #define BIT_MSK__VP__DEC656_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverts field ID sense for internal vertical timing reference of 656 input module
  #define BIT_MSK__VP__DEC656_CONFIG__INVERT_FIELD_ID                                       0x02
  // (ReadWrite, Bits 2) 
  // Force Y/C demux phase
  #define BIT_MSK__VP__DEC656_CONFIG__FORCE_YC_PHASE                                        0x04
  // (ReadWrite, Bits 3) 
  // Y/C demux phase
  #define BIT_MSK__VP__DEC656_CONFIG__YC_PHASE_VALUE                                        0x08
  // (ReadWrite, Bits 4) 
  // Don't generate DE in the first line of every even field
  #define BIT_MSK__VP__DEC656_CONFIG__SKIP_FIRST_EVEN_LINE                                  0x10
  // (ReadWrite, Bits 5) 
  // Don't generate DE in the first line of every odd field
  #define BIT_MSK__VP__DEC656_CONFIG__SKIP_FIRST_ODD_LINE                                   0x20

// For embedded syncs, delay from EAV to generated HSYNC
#define REG_ADDR__VP__DEC656_DELAY_EAV_TO_HSYNC_ACTIVE                   (video_path_core_registers | 0x0032)

// For embedded syncs, pulse width of hsync pulse (minus 1)
#define REG_ADDR__VP__DEC656_PULSE_WIDTH_HSYNC_MINUS_ONE                 (video_path_core_registers | 0x0034)

// Number of lines after EAV of last active data line to start VSYNC
#define REG_ADDR__VP__DEC656_VSYNC_FRONT_PORCH                           (video_path_core_registers | 0x0036)

// Number of clocks after EAV to delay start of VSYNC pulse for even fields or progressive frames
#define REG_ADDR__VP__DEC656_EAV_TO_VSYNC_DELAY_EVEN                     (video_path_core_registers | 0x0038)

// Number of clocks after EAV to delay start of VSYNC pulse for odd fields
#define REG_ADDR__VP__DEC656_EAV_TO_VSYNC_DELAY_ODD                      (video_path_core_registers | 0x003a)

// Number of lines of width of VSYNC pulse
#define REG_ADDR__VP__DEC656_PULSE_WIDTH_VSYNC                           (video_path_core_registers | 0x003c)

// 656 decoder status
#define REG_ADDR__VP__DEC656_STATUS                                      (video_path_core_registers | 0x003e)
  // (ReadOnly, Bits 0) 
  // Interlaced signal detected
  #define BIT_MSK__VP__DEC656_STATUS__INTERLACED                                            0x01
  // (ReadOnly, Bits 1) 
  // 656 signal detected
  #define BIT_MSK__VP__DEC656_STATUS__VIDEO656                                              0x02
  // (ReadOnly, Bits 2) 
  // Y/C demux phase
  #define BIT_MSK__VP__DEC656_STATUS__YC_PHASE                                              0x04
  // (ReadOnly, Bits 5:3) 
  // Alignment of embedded sync control word
  #define BIT_MSK__VP__DEC656_STATUS__VIDEO656_ALIGNMENT                                    0x38

// Output muting
#define REG_ADDR__VP__OUTPUT_MUTE                                        (video_path_core_registers | 0x0040)
  // (ReadWrite, Bits 0) 
  // Disable vsync output
  #define BIT_MSK__VP__OUTPUT_MUTE__VSYNC_DISABLE                                         0x01
  // (ReadWrite, Bits 1) 
  // Disable hsync output
  #define BIT_MSK__VP__OUTPUT_MUTE__HSYNC_DISABLE                                         0x02
  // (ReadWrite, Bits 2) 
  // Disable csync output
  #define BIT_MSK__VP__OUTPUT_MUTE__CSYNC_DISABLE                                         0x04
  // (ReadWrite, Bits 3) 
  // Disable DE output
  #define BIT_MSK__VP__OUTPUT_MUTE__DE_DISABLE                                            0x08
  // (ReadWrite, Bits 4) 
  // Disable Y/G output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__Y_DISABLE                                             0x10
  // (ReadWrite, Bits 5) 
  // Disable Cb/B output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__CB_DISABLE                                            0x20
  // (ReadWrite, Bits 6) 
  // Disable Cr/R output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__CR_DISABLE                                            0x40
  // (ReadWrite, Bits 7) 
  // Tristate video output pins
  #define BIT_MSK__VP__OUTPUT_MUTE__TRISTATE                                              0x80

// Output port configuration
#define REG_ADDR__VP__OUTPUT_SYNC_CONFIG                                 (video_path_core_registers | 0x0042)
  // (ReadWrite, Bits 0) 
  // Select VSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__VSYNC_POLARITY                                        0x01
  // (ReadWrite, Bits 1) 
  // Select HSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__HSYNC_POLARITY                                        0x02
  // (ReadWrite, Bits 2) 
  // Select CSYNC polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__CSYNC_POLARITY                                        0x04
  // (ReadWrite, Bits 3) 
  // Select DE polarity
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__DE_POLARITY                                           0x08
  // (ReadWrite, Bits 4) 
  // Swap HSYNC and CSYNC output controls
  #define BIT_MSK__VP__OUTPUT_SYNC_CONFIG__SWAP_HS_CS                                            0x10

// Output pin mapping configuration
#define REG_ADDR__VP__OUTPUT_MAPPING                                     (video_path_core_registers | 0x0044)
  // (ReadWrite, Bits 0) 
  // Reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_Y                                             0x01
  // (ReadWrite, Bits 1) 
  // Reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_CB                                            0x02
  // (ReadWrite, Bits 2) 
  // Reverse output pins (11:0 [-] 0:11)
  #define BIT_MSK__VP__OUTPUT_MAPPING__REVERSE_CR                                            0x04
  // (ReadWrite, Bits 5:3) 
  // Select internal datapath for Y output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_Y                                              0x38
  // (ReadWrite, Bits 8:6) 
  // Select internal datapath for Cb output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_CB                                             0x1C0
  // (ReadWrite, Bits 11:9) 
  // Select internal datapath for Cr output pins
  #define BIT_MSK__VP__OUTPUT_MAPPING__SELECT_CR                                             0xE00

// Output masking
#define REG_ADDR__VP__OUTPUT_MASK                                        (video_path_core_registers | 0x0046)
  // (ReadWrite, Bits 1:0) 
  // Disable LSBs on Y output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_Y                                        0x03
  // (ReadWrite, Bits 3:2) 
  // Disable LSBs on Cb output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_CB                                       0x0C
  // (ReadWrite, Bits 5:4) 
  // Disable LSBs on Cr output pins
  #define BIT_MSK__VP__OUTPUT_MASK__DISABLE_LSBS_CR                                       0x30

// Output format
#define REG_ADDR__VP__OUTPUT_FORMAT                                      (video_path_core_registers | 0x0048)
  // (ReadWrite, Bits 0) 
  // Send data on both posedge and negedge of output clock
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_ENABLE                                            0x01
  // (ReadWrite, Bits 1) 
  // Select polarity of DDR encoder
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_POLARITY                                          0x02
  // (ReadWrite, Bits 4:2) 
  // Select DDR mode
  #define BIT_MSK__VP__OUTPUT_FORMAT__DDR_MODE                                              0x1C
  // (ReadWrite, Bits 5) 
  // Enable Y/C mux logic
  #define BIT_MSK__VP__OUTPUT_FORMAT__YC_MUX_ENABLE                                         0x20
  // (ReadWrite, Bits 6) 
  // Select polarity of Y/C mux logic
  #define BIT_MSK__VP__OUTPUT_FORMAT__YC_MUX_POLARITY                                       0x40
  // (ReadWrite, Bits 7) 
  // Select Cb/Cr order for muxed signals
  #define BIT_MSK__VP__OUTPUT_FORMAT__CBCR_ORDER                                            0x80
  // (ReadWrite, Bits 9:8) 
  // Remove replicated pixels
  #define BIT_MSK__VP__OUTPUT_FORMAT__PIXEL_RATE                                            0x300
  // (ReadWrite, Bits 10) 
  // Enable 4:2:0 Y demux
  #define BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_420_ENABLE                                      0x400
  // (ReadWrite, Bits 11) 
  // Select whether Cb or Cr bus are used as chroma output
  #define BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_CB_OR_CR                                        0x800

// Configure blanking data
#define REG_ADDR__VP__OUTPUT_BLANK_CONFIG                                (video_path_core_registers | 0x0050)
  // (ReadWrite, Bits 0) 
  // Enable blanking data override
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__ENABLE_BLANKING_OVERRIDE                              0x01
  // (ReadWrite, Bits 1) 
  // Enable active data override
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__ENABLE_ACTIVE_OVERRIDE                                0x02
  // (ReadWrite, Bits 3:2) 
  // Select chroma mode
  #define BIT_MSK__VP__OUTPUT_BLANK_CONFIG__CHROMA_MODE                                           0x0C

// Use this value for Y during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_Y                                     (video_path_core_registers | 0x0052)

// Use this value for Cb during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_CB                                    (video_path_core_registers | 0x0054)

// Use this value for Cr during blanking when enabled
#define REG_ADDR__VP__OUTPUT_BLANK_CR                                    (video_path_core_registers | 0x0056)

// Use this value for Y during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_Y                                    (video_path_core_registers | 0x0058)

// Use this value for Cb during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_CB                                   (video_path_core_registers | 0x005a)

// Use this value for Cr during active DE when enabled
#define REG_ADDR__VP__OUTPUT_ACTIVE_CR                                   (video_path_core_registers | 0x005c)

// Format Detector #0 configuration
#define REG_ADDR__VP__FDET_CONFIG                                        (video_path_core_registers | 0x0080)
  // (ReadWrite, Bits 0) 
  // Select which sync polarity to use
  #define BIT_MSK__VP__FDET_CONFIG__SYNC_POLARITY_FORCE                                   0x01
  // (ReadWrite, Bits 1) 
  // Select forced HSYNC polarity
  #define BIT_MSK__VP__FDET_CONFIG__HSYNC_POLARITY                                        0x02
  // (ReadWrite, Bits 2) 
  // Select forced VSYNC polarity
  #define BIT_MSK__VP__FDET_CONFIG__VSYNC_POLARITY                                        0x04
  // (ReadWrite, Bits 3) 
  // Enable 656 mode
  #define BIT_MSK__VP__FDET_CONFIG__ENABLE_656                                            0x08

// Format Detector status. Write any value to clear the status
#define REG_ADDR__VP__FDET_STATUS                                        (video_path_core_registers | 0x0081)
  // (ReadOnly, Bits 0) 
  // Detected HSYNC polarity (requires HSYNC/DE input active)
  #define BIT_MSK__VP__FDET_STATUS__HSYNC_POLARITY                                        0x01
  // (ReadOnly, Bits 1) 
  // Detected VSYNC polarity (requires VSYNC/DE input active)
  #define BIT_MSK__VP__FDET_STATUS__VSYNC_POLARITY                                        0x02
  // (ReadOnly, Bits 2) 
  // Interlaced signal detected
  #define BIT_MSK__VP__FDET_STATUS__INTERLACED                                            0x04
  // (ReadOnly, Bits 3) 
  // 656 signal detected
  #define BIT_MSK__VP__FDET_STATUS__VIDEO656                                              0x08

// Delta Threshold. If frame rate counter differs by more than this amount between frames an IRQ is issued.
#define REG_ADDR__VP__FDET_FRAME_RATE_DELTA_THRESHOLD                    (video_path_core_registers | 0x0084)

// Frame rate = (1 / fdet_frame_rate) * f_clk_reg. Write any value to clear the counter
#define REG_ADDR__VP__FDET_FRAME_RATE                                    (video_path_core_registers | 0x0088)

// Pixels per Line (requires DE input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_PIXEL_COUNT                                   (video_path_core_registers | 0x008c)

// Active video lines per frame (requires DE input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_LINE_COUNT                                    (video_path_core_registers | 0x008e)

// Number of HSYNC low cycles (requires HSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_HSYNC_LOW_COUNT                               (video_path_core_registers | 0x0090)

// Number of HSYNC high cycles (requires HSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_HSYNC_HIGH_COUNT                              (video_path_core_registers | 0x0092)

// Number of HFRONT cycles (requires HSYNC/DE inputs active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_HFRONT_COUNT                                  (video_path_core_registers | 0x0094)

// Number of HBACK cycles (requires HSYNC/DE inputs active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_HBACK_COUNT                                   (video_path_core_registers | 0x0096)

// Number of VSYNC low cycles in the EVEN field (or progressive frame) (requires VSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_EVEN                          (video_path_core_registers | 0x0098)

// Number of VSYNC high cycles in the EVEN field (or progressive frame) (requires VSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_HIGH_COUNT_EVEN                         (video_path_core_registers | 0x009a)

// Number of VFRONT cycles in the EVEN field (or progressive frame) (requires VSYNC/DE inputs active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VFRONT_COUNT_EVEN                             (video_path_core_registers | 0x009c)

// Number of VBACK cycles in the EVEN field (or progressive frame) (requires VSYNC/DE input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VBACK_COUNT_EVEN                              (video_path_core_registers | 0x009e)

// Number of VSYNC low cycles in the ODD field (requires VSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_ODD                           (video_path_core_registers | 0x00a0)

// Number of VSYNC high cycles in the ODD field (requires VSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VSYNC_HIGH_COUNT_ODD                          (video_path_core_registers | 0x00a2)

// Number of VFRONT cycles in the ODD field (requires VSYNC/DE input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VFRONT_COUNT_ODD                              (video_path_core_registers | 0x00a4)

// Number of VBACK cycles in the ODD field (requires VSYNC/DE input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_VBACK_COUNT_ODD                               (video_path_core_registers | 0x00a6)

// Running frame count (requires VSYNC input active). Write any value to clear the counter
#define REG_ADDR__VP__FDET_FRAME_COUNT                                   (video_path_core_registers | 0x00a8)

// Configure which format changes cause IRQ - each bit enables the corresponding bit in the IRQ status register to cause an interrupt
#define REG_ADDR__VP__FDET_IRQ_MASK                                      (video_path_core_registers | 0x00b0)
  // (ReadWrite, Bits 0) 
  // HSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_POLARITY                                        0x01
  // (ReadWrite, Bits 1) 
  // VSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_POLARITY                                        0x02
  // (ReadWrite, Bits 2) 
  // Interlaced status changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__INTERLACED                                            0x04
  // (ReadWrite, Bits 3) 
  // BT.656 status changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__VIDEO656                                              0x08
  // (ReadWrite, Bits 4) 
  // Frame rate changes above threshold
  #define BIT_MSK__VP__FDET_IRQ_MASK__FRAME_RATE                                            0x10
  // (ReadWrite, Bits 5) 
  // Pixel count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__PIXEL_COUNT                                           0x20
  // (ReadWrite, Bits 6) 
  // Line count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__LINE_COUNT                                            0x40
  // (ReadWrite, Bits 7) 
  // HSYNC low count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_LOW_COUNT                                       0x80
  // (ReadWrite, Bits 8) 
  // HSYNC high count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_HIGH_COUNT                                      0x100
  // (ReadWrite, Bits 9) 
  // HFRONT count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HFRONT_COUNT                                          0x200
  // (ReadWrite, Bits 10) 
  // HBACK count changes
  #define BIT_MSK__VP__FDET_IRQ_MASK__HBACK_COUNT                                           0x400
  // (ReadWrite, Bits 11) 
  // VSYNC low count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_EVEN                                  0x800
  // (ReadWrite, Bits 12) 
  // VSYNC hight count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_EVEN                                 0x1000
  // (ReadWrite, Bits 13) 
  // VFRONT count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_EVEN                                     0x2000
  // (ReadWrite, Bits 14) 
  // VBACK count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_EVEN                                      0x4000
  // (ReadWrite, Bits 15) 
  // VSYNC low count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_ODD                                   0x8000
  // (ReadWrite, Bits 16) 
  // VSYNC hight count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_ODD                                  0x10000
  // (ReadWrite, Bits 17) 
  // VFRONT count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_ODD                                      0x20000
  // (ReadWrite, Bits 18) 
  // VBACK count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_ODD                                       0x40000

// IRQ status register
#define REG_ADDR__VP__FDET_IRQ_STATUS                                    (video_path_core_registers | 0x00b4)
  // (ReadWrite, Bits 0) 
  // HSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_POLARITY                                        0x01
  // (ReadWrite, Bits 1) 
  // VSYNC polarity changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_POLARITY                                        0x02
  // (ReadWrite, Bits 2) 
  // Interlaced status changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__INTERLACED                                            0x04
  // (ReadWrite, Bits 3) 
  // BT.656 status changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VIDEO656                                              0x08
  // (ReadWrite, Bits 4) 
  // Frame rate changes above threshold
  #define BIT_MSK__VP__FDET_IRQ_STATUS__FRAME_RATE                                            0x10
  // (ReadWrite, Bits 5) 
  // Pixel count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__PIXEL_COUNT                                           0x20
  // (ReadWrite, Bits 6) 
  // Line count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__LINE_COUNT                                            0x40
  // (ReadWrite, Bits 7) 
  // HSYNC low count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_LOW_COUNT                                       0x80
  // (ReadWrite, Bits 8) 
  // HSYNC high count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_HIGH_COUNT                                      0x100
  // (ReadWrite, Bits 9) 
  // HFRONT count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HFRONT_COUNT                                          0x200
  // (ReadWrite, Bits 10) 
  // HBACK count changes
  #define BIT_MSK__VP__FDET_IRQ_STATUS__HBACK_COUNT                                           0x400
  // (ReadWrite, Bits 11) 
  // VSYNC low count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_EVEN                                  0x800
  // (ReadWrite, Bits 12) 
  // VSYNC hight count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_EVEN                                 0x1000
  // (ReadWrite, Bits 13) 
  // VFRONT count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_EVEN                                     0x2000
  // (ReadWrite, Bits 14) 
  // VBACK count changes (even fields/progressive)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_EVEN                                      0x4000
  // (ReadWrite, Bits 15) 
  // VSYNC low count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_ODD                                   0x8000
  // (ReadWrite, Bits 16) 
  // VSYNC hight count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_ODD                                  0x10000
  // (ReadWrite, Bits 17) 
  // VFRONT count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_ODD                                      0x20000
  // (ReadWrite, Bits 18) 
  // VBACK count changes (odd fields)
  #define BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_ODD                                       0x40000

// Embedded sync encoder configuration
#define REG_ADDR__VP__EMBD_SYNC_ENC_CONFIG                               (video_path_core_registers | 0x00c0)
  // (ReadWrite, Bits 0) 
  // Enable embedded sync encoder to create embedded sync signals
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Select polarity for field ID
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__FIELD_ID_POLARITY                                     0x02
  // (ReadWrite, Bits 2) 
  // Select color space
  #define BIT_MSK__VP__EMBD_SYNC_ENC_CONFIG__INPUT_IS_RGB                                          0x04

// Configuration for section #0
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT0_CONFIG                         (video_path_core_registers | 0x00c4)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT0_CONFIG__LAST                                                  0x800000

// Configuration for section #1
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT1_CONFIG                         (video_path_core_registers | 0x00c8)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT1_CONFIG__LAST                                                  0x800000

// Configuration for section #2
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT2_CONFIG                         (video_path_core_registers | 0x00cc)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT2_CONFIG__LAST                                                  0x800000

// Configuration for section #3
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT3_CONFIG                         (video_path_core_registers | 0x00d0)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT3_CONFIG__LAST                                                  0x800000

// Configuration for section #4
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT4_CONFIG                         (video_path_core_registers | 0x00d4)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT4_CONFIG__LAST                                                  0x800000

// Configuration for section #5
#define REG_ADDR__VP__EMBD_SYNC_ENC_SECT5_CONFIG                         (video_path_core_registers | 0x00d8)
  // (ReadWrite, Bits 2:0) 
  // SAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__SAV                                                   0x07
  // (ReadWrite, Bits 5:3) 
  // EAV value
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__EAV                                                   0x38
  // (ReadWrite, Bits 21:6) 
  // Number of lines in this section
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__LINES                                                 0x3FFFC0
  // (ReadWrite, Bits 22) 
  // Select section type
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__DATA                                                  0x400000
  // (ReadWrite, Bits 23) 
  // Last section flag
  #define BIT_MSK__VP__EMBD_SYNC_ENC_SECT5_CONFIG__LAST                                                  0x800000

// Number of active pixels per line
#define REG_ADDR__VP__EMBD_SYNC_ENC_HORIZONTAL_DATA_PIXELS               (video_path_core_registers | 0x00dc)

// Number of blanking pixels per line
#define REG_ADDR__VP__EMBD_SYNC_ENC_HORIZONTAL_BLANKING_PIXELS           (video_path_core_registers | 0x00de)

// Bank update request for embedded sync encoder parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__EMBD_SYNC_ENC_UPDATE_REQUEST                       (video_path_core_registers | 0x00e0)

// Read/write bank configuration for embedded sync encoder parameters
#define REG_ADDR__VP__EMBD_SYNC_ENC_BANK_CONFIG                          (video_path_core_registers | 0x00e1)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (ReadWrite, Bits 3) 
  // When 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__EMBD_SYNC_ENC_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

//***************************************************************************
// video_path_core_registers__cms_core_registers. Address: 40
// Module features
#define REG_ADDR__VP__CMS__FEATURES                                      (video_path_core_registers__cms_core_registers | 0x0000)
  // (ReadOnly, Bits 0) 
  // CMS module enabled
  #define BIT_MSK__VP__CMS__FEATURES__CMS_CORE                                              0x01
  // (ReadOnly, Bits 1) 
  // Pixel capture at the input
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_IN                                             0x02
  // (ReadOnly, Bits 2) 
  // Chroma vertical upsampler
  #define BIT_MSK__VP__CMS__FEATURES__C420_C422                                             0x04
  // (ReadOnly, Bits 3) 
  // Chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__FEATURES__C422_C444                                             0x08
  // (ReadOnly, Bits 4) 
  // Multi-Colorspace Converter #0
  #define BIT_MSK__VP__CMS__FEATURES__MULTI_CSC0                                            0x10
  // (ReadOnly, Bits 5) 
  // Pixel capture before CMS
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_PRE                                            0x20
  // (ReadOnly, Bits 6) 
  // PWLI #0
  #define BIT_MSK__VP__CMS__FEATURES__PWLI0                                                 0x40
  // (ReadOnly, Bits 7) 
  // CMS Matrix
  #define BIT_MSK__VP__CMS__FEATURES__CMS_MATRIX                                            0x80
  // (ReadOnly, Bits 8) 
  // PWLI #1
  #define BIT_MSK__VP__CMS__FEATURES__PWLI1                                                 0x100
  // (ReadOnly, Bits 9) 
  // PWLI #2
  #define BIT_MSK__VP__CMS__FEATURES__PWLI2                                                 0x200
  // (ReadOnly, Bits 10) 
  // Pixel capture after CMS
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_POST                                           0x400
  // (ReadOnly, Bits 11) 
  // Multi-Colorspace Converter #1
  #define BIT_MSK__VP__CMS__FEATURES__MULTI_CSC1                                            0x800
  // (ReadOnly, Bits 12) 
  // Chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__FEATURES__C444_C422                                             0x1000
  // (ReadOnly, Bits 13) 
  // Chroma vertical subsampler
  #define BIT_MSK__VP__CMS__FEATURES__C422_C420                                             0x2000
  // (ReadOnly, Bits 14) 
  // Dither/Round
  #define BIT_MSK__VP__CMS__FEATURES__DITHER_RND                                            0x4000
  // (ReadOnly, Bits 15) 
  // Range Clip
  #define BIT_MSK__VP__CMS__FEATURES__RANGE_CLIP                                            0x8000
  // (ReadOnly, Bits 16) 
  // Pixel capture at the output
  #define BIT_MSK__VP__CMS__FEATURES__PIXCAP_OUT                                            0x10000
  // (ReadOnly, Bits 17) 
  // Retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__FEATURES__RETIMING                                              0x20000

// Device build time stamp
#define REG_ADDR__VP__CMS__BUILD_TIME                                    (video_path_core_registers__cms_core_registers | 0x0008)

// Number of data bits for the datapath
#define REG_ADDR__VP__CMS__DATA_BITS_VALUE                               (video_path_core_registers__cms_core_registers | 0x000e)

// Configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0010)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CONTROL                            (video_path_core_registers__cms_core_registers | 0x0011)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__PIXCAP_PRE_STATUS                             (video_path_core_registers__cms_core_registers | 0x0012)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__PIXCAP_PRE_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__PIXCAP_PRE_PIXEL                              (video_path_core_registers__cms_core_registers | 0x0014)

// Specify captured line number
#define REG_ADDR__VP__CMS__PIXCAP_PRE_LINE                               (video_path_core_registers__cms_core_registers | 0x0016)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_Y                                  (video_path_core_registers__cms_core_registers | 0x0018)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CB                                 (video_path_core_registers__cms_core_registers | 0x001a)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_PRE_CR                                 (video_path_core_registers__cms_core_registers | 0x001c)

// PWLI0 Red Configuration
#define REG_ADDR__VP__CMS__PWLI0_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0020)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI0 Red coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_R_CONFIG__BANK_SEL                                              0x800

// PWLI0 Green Configuration
#define REG_ADDR__VP__CMS__PWLI0_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0022)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI0 Green coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_G_CONFIG__BANK_SEL                                              0x800

// PWLI0 Blue Configuration
#define REG_ADDR__VP__CMS__PWLI0_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0024)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI0 Blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI0_B_CONFIG__BANK_SEL                                              0x800

// Bank update request for PWLI0 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI0_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x0026)

// Read/write bank configuration for PWLI0 parameters
#define REG_ADDR__VP__CMS__PWLI0_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0027)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (ReadWrite, Bits 3) 
  // When 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI0_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// Start address of PWLI0 coefficient table. This register auto-increments when any of the pwli0_r_data/pwli0_g_data/pwli0_b_data registers is read or written. 2 MSBs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI0_ADDR                                    (video_path_core_registers__cms_core_registers | 0x002c)

// PWLI0 Red coefficient table data. Reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x0030)

// PWLI0 Green coefficient table data. Reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x0034)

// PWLI0 Blue coefficient table data. Reading or writing this register increments the pwli0_addr value
#define REG_ADDR__VP__CMS__PWLI0_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x0038)

// PWLI0 combined coefficient table data. Writing this registers increments the pwli0_addr value. No read access!
#define REG_ADDR__VP__CMS__PWLI0_DATA                                    (video_path_core_registers__cms_core_registers | 0x003c)

// 3x3 Matrix Configuration
#define REG_ADDR__VP__CMS__MATRIX3X3_CONFIG                              (video_path_core_registers__cms_core_registers | 0x0040)
  // (ReadWrite, Bits 0) 
  // Enable 3x3 Matrix
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Select dither/round operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__DITHER_ENABLE                                         0x02
  // (ReadWrite, Bits 2) 
  // Enable CMS mode
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__CMS_MODE                                              0x04
  // (ReadWrite, Bits 5:3) 
  // Coefficient set for forced mode
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__FORCE_SET                                             0x38
  // (ReadWrite, Bits 6) 
  // Force selected coefficient set
  #define BIT_MSK__VP__CMS__MATRIX3X3_CONFIG__FORCE_ENABLE                                          0x40

// 3x3 Matrix Set Configuration
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_CONFIG                          (video_path_core_registers__cms_core_registers | 0x0041)
  // (ReadWrite, Bits 2:0) 
  // Coefficient set address
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONFIG__SET_ADDR                                              0x07
  // (ReadWrite, Bits 6:3) 
  // Coefficient read address
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONFIG__COEFF_ADDR                                            0x78

// 3x3 Matrix Set Control
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_CONTROL                         (video_path_core_registers__cms_core_registers | 0x0042)
  // (ReadWrite, Bits 0) 
  // Start write operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONTROL__WRITE                                                 0x01
  // (ReadWrite, Bits 1) 
  // Start read operation
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_CONTROL__READ                                                  0x02

// 3x3 Matrix Set Status
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_STATUS                          (video_path_core_registers__cms_core_registers | 0x0043)
  // (ReadOnly, Bits 0) 
  // Transaction in progress
  #define BIT_MSK__VP__CMS__MATRIX3X3_CMS_STATUS__BUSY                                                  0x01

// R1C1 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C1                       (video_path_core_registers__cms_core_registers | 0x0044)

// R1C2 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C2                       (video_path_core_registers__cms_core_registers | 0x0048)

// R1C3 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR1C3                       (video_path_core_registers__cms_core_registers | 0x004c)

// R2C1 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C1                       (video_path_core_registers__cms_core_registers | 0x0050)

// R2C2 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C2                       (video_path_core_registers__cms_core_registers | 0x0054)

// R2C3 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR2C3                       (video_path_core_registers__cms_core_registers | 0x0058)

// R3C1 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C1                       (video_path_core_registers__cms_core_registers | 0x005c)

// R3C2 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C2                       (video_path_core_registers__cms_core_registers | 0x0060)

// R3C3 matrix multiplier coefficient. Assumed to be 18-bit signed with 4 integer bits and 14 fractional bits.
#define REG_ADDR__VP__CMS__MATRIX3X3_MULTCOEFFR3C3                       (video_path_core_registers__cms_core_registers | 0x0064)

// CMS matrix coefficient read data
#define REG_ADDR__VP__CMS__MATRIX3X3_CMS_COEFF                           (video_path_core_registers__cms_core_registers | 0x0068)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_GY_OFFSET                        (video_path_core_registers__cms_core_registers | 0x0070)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_BCB_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0072)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_IN_RCR_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0074)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_GY_OFFSET                       (video_path_core_registers__cms_core_registers | 0x0076)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_BCB_OFFSET                      (video_path_core_registers__cms_core_registers | 0x0078)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__MATRIX3X3_OUT_RCR_OFFSET                      (video_path_core_registers__cms_core_registers | 0x007a)

// PWLI1 Red Configuration
#define REG_ADDR__VP__CMS__PWLI1_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0080)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI1 Red coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_R_CONFIG__BANK_SEL                                              0x800

// PWLI1 Green Configuration
#define REG_ADDR__VP__CMS__PWLI1_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0082)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI1 Green coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_G_CONFIG__BANK_SEL                                              0x800

// PWLI1 Blue Configuration
#define REG_ADDR__VP__CMS__PWLI1_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x0084)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for PWLI1 Blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI1_B_CONFIG__BANK_SEL                                              0x800

// Bank update request for PWLI1 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI1_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x0086)

// Read/write bank configuration for PWLI1 parameters
#define REG_ADDR__VP__CMS__PWLI1_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x0087)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (ReadWrite, Bits 3) 
  // When 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI1_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// Start address of PWLI1 coefficient table. This register auto-increments when any of the pwli1_r_data/pwli1_g_data/pwli1_b_data registers is read or written. 2 MSBs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI1_ADDR                                    (video_path_core_registers__cms_core_registers | 0x008c)

// PWLI1 Red coefficient table data. Reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x0090)

// PWLI1 Green coefficient table data. Reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x0094)

// PWLI1 Blue coefficient table data. Reading or writing this register increments the pwli1_addr value
#define REG_ADDR__VP__CMS__PWLI1_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x0098)

// PWLI1 combined coefficient table data. Writing this registers increments the pwli1_addr value. No read access!
#define REG_ADDR__VP__CMS__PWLI1_DATA                                    (video_path_core_registers__cms_core_registers | 0x009c)

// pwli2 Red Configuration
#define REG_ADDR__VP__CMS__PWLI2_R_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a0)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for pwli2 Red coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_R_CONFIG__BANK_SEL                                              0x800

// pwli2 Green Configuration
#define REG_ADDR__VP__CMS__PWLI2_G_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a2)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for pwli2 Green coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_G_CONFIG__BANK_SEL                                              0x800

// pwli2 Blue Configuration
#define REG_ADDR__VP__CMS__PWLI2_B_CONFIG                                (video_path_core_registers__cms_core_registers | 0x00a4)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Inverse function
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__INVERSE                                               0x02
  // (ReadWrite, Bits 2) 
  // Enable error adjustment
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ERROR_ADJUST                                          0x04
  // (ReadWrite, Bits 3) 
  // Enable extended y_offset range
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ERROR_RANGE                                           0x08
  // (ReadWrite, Bits 6:4) 
  // Select rounding value for up to 7-bit word size reduction at the output
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__ROUND                                                 0x70
  // (ReadWrite, Bits 7) 
  // Mute output
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__MUTE                                                  0x80
  // (ReadWrite, Bits 10:8) 
  // Operating mode
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__MODE                                                  0x700
  // (ReadWrite, Bits 11) 
  // Bank select for pwli2 Blue coefficient table
  #define BIT_MSK__VP__CMS__PWLI2_B_CONFIG__BANK_SEL                                              0x800

// Bank update request for pwli2 parameters (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__PWLI2_UPDATE_REQUEST                          (video_path_core_registers__cms_core_registers | 0x00a6)

// Read/write bank configuration for pwli2 parameters
#define REG_ADDR__VP__CMS__PWLI2_BANK_CONFIG                             (video_path_core_registers__cms_core_registers | 0x00a7)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__UPDATE_MODE                                           0x04
  // (ReadWrite, Bits 3) 
  // When 1, this double buffer is bypassed for all_update request
  #define BIT_MSK__VP__CMS__PWLI2_BANK_CONFIG__ALL_UPDATE_BYPASS                                     0x08

// Start address of pwli2 coefficient table. This register auto-increments when any of the pwli2_r_data/pwli2_g_data/pwli2_b_data registers is read or written. 2 MSBs select the coefficient bank
#define REG_ADDR__VP__CMS__PWLI2_ADDR                                    (video_path_core_registers__cms_core_registers | 0x00ac)

// pwli2 Red coefficient table data. Reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_R_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b0)

// pwli2 Green coefficient table data. Reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_G_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b4)

// pwli2 Blue coefficient table data. Reading or writing this register increments the pwli2_addr value
#define REG_ADDR__VP__CMS__PWLI2_B_DATA                                  (video_path_core_registers__cms_core_registers | 0x00b8)

// pwli2 combined coefficient table data. Writing this registers increments the pwli2_addr value. No read access!
#define REG_ADDR__VP__CMS__PWLI2_DATA                                    (video_path_core_registers__cms_core_registers | 0x00bc)

// Configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_POST_CONFIG                            (video_path_core_registers__cms_core_registers | 0x00c0)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__PIXCAP_POST_CONTROL                           (video_path_core_registers__cms_core_registers | 0x00c1)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__PIXCAP_POST_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__PIXCAP_POST_STATUS                            (video_path_core_registers__cms_core_registers | 0x00c2)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__PIXCAP_POST_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__PIXCAP_POST_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__PIXCAP_POST_PIXEL                             (video_path_core_registers__cms_core_registers | 0x00c4)

// Specify captured line number
#define REG_ADDR__VP__CMS__PIXCAP_POST_LINE                              (video_path_core_registers__cms_core_registers | 0x00c6)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_Y                                 (video_path_core_registers__cms_core_registers | 0x00c8)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_CB                                (video_path_core_registers__cms_core_registers | 0x00ca)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__PIXCAP_POST_CR                                (video_path_core_registers__cms_core_registers | 0x00cc)

// CMS demo mode configuration
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_CONFIG                         (video_path_core_registers__cms_core_registers | 0x00d0)
  // (ReadWrite, Bits 1:0) 
  // Select CMS demo display
  #define BIT_MSK__VP__CMS__CMS_DEMO_SPLIT_CONFIG__SELECT                                                0x03
  // (ReadWrite, Bits 2) 
  // Interlace mode
  #define BIT_MSK__VP__CMS__CMS_DEMO_SPLIT_CONFIG__INTERLACE                                             0x04

// Demo mode horizontal split point
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_X                              (video_path_core_registers__cms_core_registers | 0x00d2)

// Demo mode vertical split point
#define REG_ADDR__VP__CMS__CMS_DEMO_SPLIT_Y                              (video_path_core_registers__cms_core_registers | 0x00d4)

// Demo mode split bar width
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_WIDTH                            (video_path_core_registers__cms_core_registers | 0x00d6)

// Demo mode split bar color (G/Y)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_Y                           (video_path_core_registers__cms_core_registers | 0x00d8)

// Demo mode split bar color (B/Cb)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_CB                          (video_path_core_registers__cms_core_registers | 0x00da)

// Demo mode split bar color (R/Cr)
#define REG_ADDR__VP__CMS__CMS_DEMO_BAR_DATA_CR                          (video_path_core_registers__cms_core_registers | 0x00dc)

//***************************************************************************
// video_path_core_registers__cms_core_registers__csc_core_registers0. Address: 40
// Module features
#define REG_ADDR__VP__CMS__CSC0__FEATURES                                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0000)
  // (ReadOnly, Bits 0) 
  // CSC module enabled
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__CSC_CORE                                              0x01
  // (ReadOnly, Bits 1) 
  // Pixel capture at the input
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__PIXCAP_IN                                             0x02
  // (ReadOnly, Bits 2) 
  // Chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C420_C422                                             0x04
  // (ReadOnly, Bits 3) 
  // Chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C422_C444                                             0x08
  // (ReadOnly, Bits 4) 
  // Multi-Colorspace Converter
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__MULTI_CSC                                             0x10
  // (ReadOnly, Bits 5) 
  // Chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C444_C422                                             0x20
  // (ReadOnly, Bits 6) 
  // Chroma vertical subsampler
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__C422_C420                                             0x40
  // (ReadOnly, Bits 7) 
  // Dither/Round
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__DITHER_RND                                            0x80
  // (ReadOnly, Bits 8) 
  // Range Clip
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__RANGE_CLIP                                            0x100
  // (ReadOnly, Bits 9) 
  // Pixel capture at the output
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__PIXCAP_OUT                                            0x200
  // (ReadOnly, Bits 10) 
  // Retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__RETIMING                                              0x400
  // (ReadOnly, Bits 11) 
  // Shared 4:2:0 filter module
  #define BIT_MSK__VP__CMS__CSC0__FEATURES__SHARED_420                                            0x800

// Number of data bits for the datapath
#define REG_ADDR__VP__CMS__CSC0__DATA_BITS_VALUE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0002)

// Device build time stamp
#define REG_ADDR__VP__CMS__CSC0__BUILD_TIME                              (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0004)

// Configure chroma vertical upsampler
#define REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Enable chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__BYPASS                                                0x02
  // (ReadWrite, Bits 2) 
  // Select whether Cb or Cr bus are used as chroma input
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__IN_CB_OR_CR                                           0x04
  // (ReadWrite, Bits 3) 
  // Polarity of alternating Cb/Cr
  #define BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__IN_CB_CR_POLARITY                                     0x08

// Configure chroma upsampler
#define REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x000c)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Select whether Cb or Cr bus are used as muxed chroma input
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__USE_CB_OR_CR                                          0x02
  // (ReadWrite, Bits 2) 
  // Disable Lowpass filter
  #define BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__DISABLE_FILTER                                        0x04
	
// Configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0010)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CONTROL                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0011)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_STATUS                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0012)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_IN_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_PIXEL                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0014)

// Specify captured line number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_LINE                          (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0016)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_Y                             (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0018)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CB                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x001a)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_IN_CR                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x001c)

// Colorspace converter configuration
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0020)
  // (ReadWrite, Bits 1:0) 
  // Enable conversion
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__ENABLE                                                0x03
  // (ReadWrite, Bits 3:2) 
  // Select output color standard
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_STD                                               0x0C
  // (ReadWrite, Bits 4) 
  // Select output levels
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_PC                                                0x10
  // (ReadWrite, Bits 5) 
  // Select output color space
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_RGB                                               0x20
  // (ReadWrite, Bits 7:6) 
  // Select input color standard
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_STD                                                0xC0
  // (ReadWrite, Bits 8) 
  // Select input levels
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_PC                                                 0x100
  // (ReadWrite, Bits 9) 
  // Select input color space
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_RGB                                                0x200
  // (ReadWrite, Bits 10) 
  // Select dither/round operation
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__DITHER_ENABLE                                         0x400
  // (ReadWrite, Bits 10) 
  // Select sutration operation
  #define BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__SUTRATION_DISABLE                                     0x800

// R1C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0022)

// R1C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0024)

// R1C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR1C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0026)

// R2C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0028)

// R2C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002a)

// R2C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR2C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002c)

// R3C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C1                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x002e)

// R3C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C2                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0030)

// R3C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_MULTCOEFFR3C3                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0032)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_GY_OFFSET                  (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0034)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_BCB_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0036)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_IN_RCR_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0038)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_GY_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003a)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_BCB_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003c)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC0__MULTI_CSC_OUT_RCR_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x003e)

// Configure chroma downsampler
#define REG_ADDR__VP__CMS__CSC0__C444_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0040)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Reserved
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__RESERVED_1                                            0x02
  // (ReadWrite, Bits 2) 
  // Disable Lowpass filter
  #define BIT_MSK__VP__CMS__CSC0__C444_C422_CONFIG__DISABLE_FILTER                                        0x04

// Configure chroma vertical downsampler
#define REG_ADDR__VP__CMS__CSC0__C422_C420_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0042)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__BYPASS                                                0x02
  // (ReadWrite, Bits 2) 
  // Select whether Cb or Cr bus are used as chroma output
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__OUT_CB_OR_CR                                          0x04
  // (ReadWrite, Bits 3) 
  // Polarity of alternating Cb/Cr
  #define BIT_MSK__VP__CMS__CSC0__C422_C420_CONFIG__OUT_CB_CR_POLARITY                                    0x08

// Dither configuration
#define REG_ADDR__VP__CMS__CSC0__DITHER_CONFIG                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0044)
  // (ReadWrite, Bits 1:0) 
  // Select bits
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__MODE                                                  0x03
  // (ReadWrite, Bits 2) 
  // Select dither/round operation
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__RND_ENABLE                                            0x04
  // (ReadWrite, Bits 3) 
  // LFSR spatial mode
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__SPATIAL_ENABLE                                        0x08
  // (ReadWrite, Bits 4) 
  // LFSR spatial dual mode
  #define BIT_MSK__VP__CMS__CSC0__DITHER_CONFIG__SPATIAL_DUAL                                          0x10

// Enables dynamic range clipping/saturation to programmable levels. In RGB mode, the Y parameters are used for all 3 color components. There is no bypass or enable.
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0046)
  // (ReadWrite, Bits 0) 
  // Select color space
  #define BIT_MSK__VP__CMS__CSC0__RANGE_CLIP_CONFIG__INPUT_IS_RGB                                          0x01

// Y/RGB minimum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_Y_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0048)

// Y/RGB maximum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_Y_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004a)

// Cb/Cr minimum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_C_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004c)

// Cb/Cr maximum clip level
#define REG_ADDR__VP__CMS__CSC0__RANGE_CLIP_C_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x004e)

// Configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0050)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CONTROL                      (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0051)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_STATUS                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0052)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC0__PIXCAP_OUT_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_PIXEL                        (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0054)

// Specify captured line number
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0056)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_Y                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0058)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CB                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x005a)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC0__PIXCAP_OUT_CR                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x005c)

// End of horizontal sync pulse -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_SYNC_END                 (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0060)

// Start of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_START       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0062)

// Midpoint of horizontal line -2
#define REG_ADDR__VP__CMS__CSC0__VTG_HALFLINE                            (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0064)

// End of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_END         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0066)

// End of horizontal line -2
#define REG_ADDR__VP__CMS__CSC0__VTG_END_OF_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0068)

// End of vertical sync pulse (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_SYNC_END                   (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0070)

// Trigger point for processing to start (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_TRIGGER_START                       (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0072)

// Start of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_START         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0074)

// End of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_END           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0076)

// End of vertical frame (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_END_OF_FRAME               (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x0078)

// Configure VTG
#define REG_ADDR__VP__CMS__CSC0__VTG_CONFIG                              (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007a)
  // (ReadWrite, Bits 0) 
  // Enable VTG
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Force resync every frame
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__RESYNC                                                0x02
  // (ReadWrite, Bits 2) 
  // Unlock VTG
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__UNLOCK                                                0x04
  // (ReadWrite, Bits 3) 
  // Blank video data
  #define BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__BLANK                                                 0x08

// Sync threshold
#define REG_ADDR__VP__CMS__CSC0__VTG_THRESHOLD                           (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007b)

// Number of cycles of delay between reference signal and VTG signal
#define REG_ADDR__VP__CMS__CSC0__VTG_CYCLE_DELAY                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007c)

// Bank update request for Video Generator parameters.  (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__CSC0__VTG_UPDATE_REQUEST                      (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007e)

// Read/write bank configuration for video generator parameters
#define REG_ADDR__VP__CMS__CSC0__VTG_BANK_CONFIG                         (video_path_core_registers__cms_core_registers__csc_core_registers0 | 0x007f)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__UPDATE_MODE                                           0x04

//***************************************************************************
// video_path_core_registers__cms_core_registers__csc_core_registers1. Address: 40
// Module features
#define REG_ADDR__VP__CMS__CSC1__FEATURES                                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0000)
  // (ReadOnly, Bits 0) 
  // CSC module enabled
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__CSC_CORE                                              0x01
  // (ReadOnly, Bits 1) 
  // Pixel capture at the input
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__PIXCAP_IN                                             0x02
  // (ReadOnly, Bits 2) 
  // Chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C420_C422                                             0x04
  // (ReadOnly, Bits 3) 
  // Chroma horizontal upsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C422_C444                                             0x08
  // (ReadOnly, Bits 4) 
  // Multi-Colorspace Converter
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__MULTI_CSC                                             0x10
  // (ReadOnly, Bits 5) 
  // Chroma horizontal subsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C444_C422                                             0x20
  // (ReadOnly, Bits 6) 
  // Chroma vertical subsampler
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__C422_C420                                             0x40
  // (ReadOnly, Bits 7) 
  // Dither/Round
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__DITHER_RND                                            0x80
  // (ReadOnly, Bits 8) 
  // Range Clip
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__RANGE_CLIP                                            0x100
  // (ReadOnly, Bits 9) 
  // Pixel capture at the output
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__PIXCAP_OUT                                            0x200
  // (ReadOnly, Bits 10) 
  // Retiming VTG/FIFO
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__RETIMING                                              0x400
  // (ReadOnly, Bits 11) 
  // Shared 4:2:0 filter module
  #define BIT_MSK__VP__CMS__CSC1__FEATURES__SHARED_420                                            0x800

// Number of data bits for the datapath
#define REG_ADDR__VP__CMS__CSC1__DATA_BITS_VALUE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0002)

// Device build time stamp
#define REG_ADDR__VP__CMS__CSC1__BUILD_TIME                              (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0004)

// Configure chroma vertical upsampler
#define REG_ADDR__VP__CMS__CSC1__C420_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0008)
  // (ReadWrite, Bits 0) 
  // Enable chroma vertical upsampler
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__BYPASS                                                0x02
  // (ReadWrite, Bits 2) 
  // Select whether Cb or Cr bus are used as chroma input
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__IN_CB_OR_CR                                           0x04
  // (ReadWrite, Bits 3) 
  // Polarity of alternating Cb/Cr
  #define BIT_MSK__VP__CMS__CSC1__C420_C422_CONFIG__IN_CB_CR_POLARITY                                     0x08

// Configure chroma upsampler
#define REG_ADDR__VP__CMS__CSC1__C422_C444_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x000c)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Select whether Cb or Cr bus are used as muxed chroma input
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__USE_CB_OR_CR                                          0x02
  // (ReadWrite, Bits 2) 
  // Disable Lowpass filter
  #define BIT_MSK__VP__CMS__CSC1__C422_C444_CONFIG__DISABLE_FILTER                                        0x04

// Configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0010)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CONTROL                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0011)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_STATUS                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0012)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_IN_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_PIXEL                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0014)

// Specify captured line number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_LINE                          (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0016)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_Y                             (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0018)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CB                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x001a)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_IN_CR                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x001c)

// Colorspace converter configuration
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0020)
  // (ReadWrite, Bits 1:0) 
  // Enable conversion
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__ENABLE                                                0x03
  // (ReadWrite, Bits 3:2) 
  // Select output color standard
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_STD                                               0x0C
  // (ReadWrite, Bits 4) 
  // Select output levels
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_PC                                                0x10
  // (ReadWrite, Bits 5) 
  // Select output color space
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__OUT_RGB                                               0x20
  // (ReadWrite, Bits 7:6) 
  // Select input color standard
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_STD                                                0xC0
  // (ReadWrite, Bits 8) 
  // Select input levels
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_PC                                                 0x100
  // (ReadWrite, Bits 9) 
  // Select input color space
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__IN_RGB                                                0x200
  // (ReadWrite, Bits 10) 
  // Select dither/round operation
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__DITHER_ENABLE                                         0x400
  // (ReadWrite, Bits 10) 
  // Select sutration operation
  #define BIT_MSK__VP__CMS__CSC1__MULTI_CSC_CONFIG__SUTRATION_DISABLE                                     0x800

// R1C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0022)

// R1C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0024)

// R1C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR1C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0026)

// R2C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0028)

// R2C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002a)

// R2C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR2C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002c)

// R3C1 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C1                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x002e)

// R3C2 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C2                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0030)

// R3C3 matrix multiplier coefficient. Assumed to be 16-bit signed with 4 integer bits and 12 fractional bits.
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_MULTCOEFFR3C3                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0032)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_GY_OFFSET                  (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0034)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_BCB_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0036)

// Signed input offset to be added to the corresponding video data prior to matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_IN_RCR_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0038)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_GY_OFFSET                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003a)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_BCB_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003c)

// Signed output offset to be added to the corresponding video data after matrix multiplication
#define REG_ADDR__VP__CMS__CSC1__MULTI_CSC_OUT_RCR_OFFSET                (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x003e)

// Configure chroma downsampler
#define REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0040)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Reserved
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__RESERVED_1                                            0x02
  // (ReadWrite, Bits 2) 
  // Disable Lowpass filter
  #define BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__DISABLE_FILTER                                        0x04
	
// Configure chroma vertical downsampler
#define REG_ADDR__VP__CMS__CSC1__C422_C420_CONFIG                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0042)
  // (ReadWrite, Bits 0) 
  // Enable
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Bypass module to reduce latency
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__BYPASS                                                0x02
  // (ReadWrite, Bits 2) 
  // Select whether Cb or Cr bus are used as chroma output
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__OUT_CB_OR_CR                                          0x04
  // (ReadWrite, Bits 3) 
  // Polarity of alternating Cb/Cr
  #define BIT_MSK__VP__CMS__CSC1__C422_C420_CONFIG__OUT_CB_CR_POLARITY                                    0x08

// Dither configuration
#define REG_ADDR__VP__CMS__CSC1__DITHER_CONFIG                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0044)
  // (ReadWrite, Bits 1:0) 
  // Select bits
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__MODE                                                  0x03
  // (ReadWrite, Bits 2) 
  // Select dither/round operation
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__RND_ENABLE                                            0x04
  // (ReadWrite, Bits 3) 
  // LFSR spatial mode
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__SPATIAL_ENABLE                                        0x08
  // (ReadWrite, Bits 4) 
  // LFSR spatial dual mode
  #define BIT_MSK__VP__CMS__CSC1__DITHER_CONFIG__SPATIAL_DUAL                                          0x10
	
// Enables dynamic range clipping/saturation to programmable levels. In RGB mode, the Y parameters are used for all 3 color components. There is no bypass or enable.
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0046)
  // (ReadWrite, Bits 0) 
  // Select color space
  #define BIT_MSK__VP__CMS__CSC1__RANGE_CLIP_CONFIG__INPUT_IS_RGB                                          0x01

// Y/RGB minimum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_Y_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0048)

// Y/RGB maximum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_Y_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004a)

// Cb/Cr minimum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_C_MIN                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004c)

// Cb/Cr maximum clip level
#define REG_ADDR__VP__CMS__CSC1__RANGE_CLIP_C_MAX                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x004e)

// Configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CONFIG                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0050)
  // (ReadWrite, Bits 0) 
  // Show capture point
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONFIG__SHOW_POINT                                            0x01
  // (ReadWrite, Bits 1) 
  // Enable auto-capture mode
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONFIG__AUTO_TRIGGER                                          0x02

// Configure capture module
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CONTROL                      (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0051)
  // (ReadWrite, Bits 0) 
  // Trigger pixel value capture
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_CONTROL__TRIGGER                                               0x01

// Capture module status
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_STATUS                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0052)
  // (ReadOnly, Bits 0) 
  // Capture module is busy, value has not been captured yet
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_STATUS__BUSY                                                  0x01
  // (ReadOnly, Bits 1) 
  // Error occurred, capture location not found
  #define BIT_MSK__VP__CMS__CSC1__PIXCAP_OUT_STATUS__ERROR                                                 0x02

// Specify captured pixel number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_PIXEL                        (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0054)

// Specify captured line number
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0056)

// Captured Y value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_Y                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0058)

// Captured Cb value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CB                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x005a)

// Captured Cr value of selected pixel
#define REG_ADDR__VP__CMS__CSC1__PIXCAP_OUT_CR                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x005c)

// End of horizontal sync pulse -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_SYNC_END                 (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0060)

// Start of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_ACTIVE_VIDEO_START       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0062)

// Midpoint of horizontal line -2
#define REG_ADDR__VP__CMS__CSC1__VTG_HALFLINE                            (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0064)

// End of horizontal active video line -1
#define REG_ADDR__VP__CMS__CSC1__VTG_HORIZONTAL_ACTIVE_VIDEO_END         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0066)

// End of horizontal line -2
#define REG_ADDR__VP__CMS__CSC1__VTG_END_OF_LINE                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0068)

// End of vertical sync pulse (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_SYNC_END                   (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0070)

// Trigger point for processing to start (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_TRIGGER_START                       (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0072)

// Start of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_ACTIVE_VIDEO_START         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0074)

// End of vertical active video (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_ACTIVE_VIDEO_END           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0076)

// End of vertical frame (in half-lines) -1
#define REG_ADDR__VP__CMS__CSC1__VTG_VERTICAL_END_OF_FRAME               (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x0078)

// Configure VTG
#define REG_ADDR__VP__CMS__CSC1__VTG_CONFIG                              (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007a)
  // (ReadWrite, Bits 0) 
  // Enable VTG
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__ENABLE                                                0x01
  // (ReadWrite, Bits 1) 
  // Force resync every frame
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__RESYNC                                                0x02
  // (ReadWrite, Bits 2) 
  // Unlock VTG
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__UNLOCK                                                0x04
  // (ReadWrite, Bits 3) 
  // Blank video data
  #define BIT_MSK__VP__CMS__CSC1__VTG_CONFIG__BLANK                                                 0x08

// Sync threshold
#define REG_ADDR__VP__CMS__CSC1__VTG_THRESHOLD                           (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007b)

// Number of cycles of delay between reference signal and VTG signal
#define REG_ADDR__VP__CMS__CSC1__VTG_CYCLE_DELAY                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007c)

// Bank update request for Video Generator parameters.  (write any data value - this is a strobe only)
#define REG_ADDR__VP__CMS__CSC1__VTG_UPDATE_REQUEST                      (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007e)

// Read/write bank configuration for video generator parameters
#define REG_ADDR__VP__CMS__CSC1__VTG_BANK_CONFIG                         (video_path_core_registers__cms_core_registers__csc_core_registers1 | 0x007f)
  // (ReadWrite, Bits 0) 
  // Select which bank to read
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__READ_BANK                                             0x01
  // (ReadWrite, Bits 1) 
  // Select which bank to write
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__WRITE_BANK                                            0x02
  // (ReadWrite, Bits 2) 
  // Select when to update double-buffer
  #define BIT_MSK__VP__CMS__CSC1__VTG_BANK_CONFIG__UPDATE_MODE                                           0x04


#define REG_ADDR__VG_VIDGEN_CONFIG                                       0x3E10

  #define BIT_MSK__VG_VIDGEN_CONFIG_ENBALE                               0x01
  #define BIT_MSK__VG_VIDGEN_CONFIG_USE_INPUT_TIMING                     0x02
  #define BIT_MSK__VG_VIDGEN_CONFIG_CHROMA_MODE                          0x04
  #define BIT_MSK__VG_VIDGEN_CONFIG_REPLACE_Y                            0x08
  #define BIT_MSK__VG_VIDGEN_CONFIG_REPLACE_CB                           0x10
  #define BIT_MSK__VG_VIDGEN_CONFIG_REPLACE_CR                           0x20

#define REG_ADDR__VG_VIDGEN_COLOR_Y                                      0x3E12

#define REG_ADDR__VG_VIDGEN_COLOR_CB                                     0x3E14

#define REG_ADDR__VG_VIDGEN_COLOR_CR                                     0x3E16



#define REG_ADDR__PATTERN_GEN_CTRL0                                      0x3E60

  #define BIT_MSK__PATTERN_GEN_CTRL0_TPG_ENABLE                          0x01
  #define BIT_MSK__PATTERN_GEN_CTRL0_VIDEO_FORMAT                        0x06
  #define BIT_MSK__PATTERN_GEN_CTRL0_SOLID_PATTERN_EN                    0x08
  #define BIT_MSK__PATTERN_GEN_CTRL0_COLORBAR_EN                         0x10
  #define BIT_MSK__PATTERN_GEN_CTRL0_SQUARE_PATTERN_EN                   0x20
  #define BIT_MSK__PATTERN_GEN_CTRL0_BAR_PATTERN_EXTMODE                 0x40

#define REG_ADDR__SOLID_PATTERN_CR                                      0x3E65

#define REG_ADDR__SOLID_PATTERN_Y                                       0x3E66

#define REG_ADDR__SOLID_PATTERN_CB                                      0x3E67



#endif // __SI_VIDPATH_REGS_H__
