/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _DEFHDMITX_
#define _DEFHDMITX_

//-----------------------------------------------------
#define TX_SLV0								(0x72)
//-----------------------------------------------------
#define TX_SWRST_ADDR						(0x05)
#define		BIT_TX_SW_RST					(0x01)
#define		BIT_TX_FIFO_RST					(0x02)
#define TX_SYS_CTRL1_ADDR					(0x08)
#define		BIT_TX_PD						(0x01)
#define		BIT_BSEL24BITS					(0x04)
#define		BIT_TX_CLOCK_RISING_EDGE		(0x02)
#define TX_STAT_ADDR						(0x09)
#define     BIT_HDMI_PSTABLE                (0X01)
#define		BIT_RSEN					    (0x04)
#define		BIT_HPD_PIN						(0x02)
#define     BIT_HDMI_STABLE_PLUGIN          (0X07)

//del in 1.4
//#define TX_SYS_CTRL4_ADDR					(0x0C)
//end

#define DATA_CTRL_ADDR                      (0X0D)
#define		BIT_AUD_MUTE					(0x02)
#define		BIT_VID_BLANK					(0x04)


#define HDCP_CTRL_ADDR						(0x0F)
#define		BIT_ENC_EN						(0x01)
#define		BIT_RiREADY						(0x02)
#define		BIT_RI_STARTED					(0x01)					
#define		BIT_CP_RESET_N					(0x04)
#define		BIT_AN_STOP						(0x08)
#define		BIT_RX_REPEATER					(0x10)
#define		BIT_BKSV_ERROR					(0x20)
#define		BIT_ENC_ON						(0x40)
#define BKSV_ADDR							(0x10)	// 5 bytes
#define AN_ADDR								(0x15)	// Random Number, 8 bytes
#define AKSV_ADDR							(0x1D)	// 5 bytes
#define Ri_ADDR								(0x22)	// 2 bytes

#define RI_STAT								(0x26)  // Ri check started status..This signal is used for the handshaking between firmware and hardware. Once the Ri check is enabled; the hardware would wait for the DDC master to finish the current transaction to take control over it. Once this bit is set firmware loses ability to use DDC Master; unless it disables Ri Check and this bit goes to 0.
#define RI_CMD_ADDR							(0x27)
#define     BIT_RI_CMD_BCAP_EN				(0x02)  // Enable polling of the BCAP "done" bit (KSV FIFO done 0x40[5]).
#define     BIT_RI_CMD_RI_EN				(0x01)  // Enable Ri Check. Need to check the Ri On bit (0x026[0]) for firmware handshaking.
#define RI_START							(0x28)  // Line number to start Auto Ri check within frame 127 or 0, 2 lsb are 0, default is 16
#define RI_RX_1								(0x29)  // Ri RX value; bits 7:0 - stored in case of any of the Ri Check errors
#define RI_RX_2								(0x2A)  // Ri RX value; bits 15:8

#define DE_HSTART_ADDR						(0x32)
#define DE_CNTRL_ADDR						(0x33)
#define		BIT_DE_ENABLED					(0x40)
#define DE_VSTART_ADDR						(0x34)
#define DE_HRES_ADDR						(0x36)
#define DE_VRES_ADDR						(0x38)
#define INTERLACE_ADJ_MODE					(0x3E)
#define INTERLACE_POL_DETECT                (0x3F)
#define     BIT_I_DETECTR                   (0x04)
#define HBIT_TO_HSYNC_ADDR					(0x40)
#define FIELD2_HSYNC_OFFSET_ADDR			(0x42)
#define HLENGTH_ADDR						(0x44)
#define VBIT_TO_VSYNC_ADDR					(0x46)
#define VLENGTH_ADDR						(0x47)

#define TX_VID_CTRL_ADDR					(0x48)
#define     BIT_VID_CTRL_CSCSEL 			(0x10)  // Color Space Conversion Standard select: BT.601(0) / BT.709(1)

#define VID_ACEN_ADDR						(0x49)
#define     BIT_VID_ACEN_CLIP_CS_ID			(0x10)  // Color Space on the link: RGB (0) / YCbCr (1)
#define     BIT_VID_ACEN_RANGE_CLIP			(0x08)  // enable(1) / disable(0-default) Range Clip from 16 to 235(RGB and Y)/240(C)
#define     BIT_VID_ACEN_RGB2YCBCR			(0x04)  // enable(1) / disable(0-default) RGB to YCbCr color-space converter
#define     BIT_VID_ACEN_RANGE_COMP			(0x02)  // enable(1) / disable(0-default)
#define     BIT_VID_ACEN_DWN_SAMPLE			(0x01)  // enable(1) / disable(0-default) 4:4:4 to 4:2:2 chroma down sampler
//#define		VID_ACEN_DEEP_COLOR_CLR		(0x3F)  // del in 1.4

#define TX_VID_MODE_ADDR					(0x4A)
#define		BIT_TX_SYNC_EXTR				(0x01)
#define		BIT_TX_DEMUX_YC					(0x02)
#define		BIT_TX_422to444					(0x04)  // Upsampling
#define		BIT_TX_CSC						(0x08)
#define		BIT_TX_16_235_RANGE				(0x10)  // Expand range
#define		BIT_TX_DITHER					(0x20)

#define TX_VID_DITHER				        (0x4F)

#define TX_VID_BLANK1_BLUE                  (0x4B)
#define TX_VID_BLANK1_GREEN                 (0x4C)
#define TX_VID_BLANK1_RED                   (0x4D)

//add in 1.4
#define VID_IN_MODE (0x69)
#define     VIN_MODE_CLR (0xf9)
//end

#define HDMI_INT_STATE						(0x70)
#define HDMI_INT_ADDR						(0x71)
#define HDMI_INT1_ADDR						(0x71)	
#define HDMI_INT2_ADDR						(0x72)

#define HDMI_INT3_ADDR						(0x73)  // Ri error & DDC FIFO interrupts
#define     BIT_INT3_RI_ERR_3				(0x80)  // Ri reading was not done within one frame       , write "1" to clear.
#define     BIT_INT3_RI_ERR_2				(0x40)  // Ri did not changed between frames #127 and #0  , write "1" to clear.
#define     BIT_INT3_RI_ERR_1				(0x20)  // Ri and Ri?doesn't match during 2nd frame (0)   , write "1" to clear.
#define     BIT_INT3_RI_ERR_0				(0x10)  // Ri and Ri?doesn't match during 1st frame (127) , write "1" to clear.
#define     BIT_INT3_DDC_CMD_DONE			(0x08)  // Asserted if DDC command is done                , write "1" to clear.
#define     BIT_INT3_DDC_FIFO_HALF			(0x04)  // Asserted if DDC FIFO is half full              , write "1" to clear.
#define     BIT_INT3_DDC_FIFO_FULL			(0x02)  // Asserted if DDC FIFO is full                   , write "1" to clear.
#define     BIT_INT3_DDC_FIFO_EMPTY			(0x01)  // Asserted if DDC FIFO is empty                  , write "1" to clear.

#define		MASK_RI_READING_MORE_ONE_FRAME  (0x80)
#define		MASK_RI_NOT_CHANGED             (0x40)
#define		MASK_RI_MISS_MATCH_FIRST_FRAME  (0x20)
#define		MASK_RI_MISS_MATCH_LAST_FRAME   (0x10)

#define		MASK_AUTO_RI_9134_SPECIFIC		(0xB0)	
#define		MASK_AUTO_RI                    (0xF0)
#define		MASK_AUTO_KSV_READY             (0x80)

//add in 1.4
#define HDMI_INT4_ADDR (0x74)
#define     DBIT_SD_INVALID  (0x01)
#define     BIT_reg_intr4_stat1 (0x02)

#define HDMI_INT5_ADDR (0x75)
#define     BIT_reg_intr5_stat0 (0x01)
#define     BIT_reg_intr5_stat1 (0x02)
#define     BIT_reg_intr5_stat2 (0x04)
#define     BIT_reg_intr5_stat3 (0x08)

#define HDMI_INT4_MASK (0x79)
#define HDMI_INT5_MASK (0x7A)
//end



#define HDMI_INT_MASK_ADDR					(0x76)
#define HDMI_INT1_MASK						(0x76)
#define HDMI_INT2_MASK						(0x77)

#define HDMI_INT3_MASK                      (0x78)  // Ri error interrupts masks & DDC FIFO interrupts mask
#define     BIT_INT3_RI_ERR_3_MASK          (0x80)  // Enable(1) / disable(0-default) ri err #3 interrupt
#define     BIT_INT3_RI_ERR_2_MASK          (0x40)  // Enable(1) / disable(0-default) ri err #2 interrupt
#define     BIT_INT3_RI_ERR_1_MASK          (0x20)  // Enable(1) / disable(0-default) ri err #1 interrupt
#define     BIT_INT3_RI_ERR_0_MASK          (0x10)  // Enable(1) / disable(0-default) ri err #0 interrupt
#define     BIT_INT3_DDC_CMD_DONE_MASK      (0x08)  // Enable(1) / disable(0-default) DDC command is done interrupt
#define     BIT_INT3_DDC_FIFO_HALF_MASK     (0x04)  // Enable(1) / disable(0-default) DDC FIFO is half full interrupt
#define     BIT_INT3_DDC_FIFO_FULL_MASK     (0x02)  // Enable(1) / disable(0-default) DDC FIFO is full interrupt
#define     BIT_INT3_DDC_FIFO_EMPTY_MASK	(0x01)  // Enable(1) / disable(0-default) DDC FIFO is empty interrupt

//addr alter in 1.4
#define INT_CNTRL_ADDR						(0x7B)  
//end

#define		BIT_INT_Ri_CHECK				(0x04)
#define		BIT_INT_HOT_PLUG				(0x40)
#define		BIT_INT_RSEN                    (0x20)
#define		BIT_BIPHASE_ERROR				(0x08)
#define		BIT_DROP_SAMPLE					(0x10)
#define		BIT_INT_VSYNC					(0x01)
#define		BIT_INT_FPIXCHANGE				(0x02)
#define		BIT_INT_KSV_READY				(0x80)

//del in 1.4
//#define TX_TMDS_CCTRL_ADDR					(0x80)
//#define TX_TMDS_CTRL_ADDR					(0x82)
//#define TX_TMDS_CTRL2_ADDR					(0x83)
//#define TX_TMDS_CTRL3_ADDR					(0x84)
//#define TX_TMDS_CTRL4_ADDR					(0x85)

//add in 1.4
#define CEC_CTRL (0xc6)
#define     BIT_CEC_I (0x01)
#define     BIT_CALIB (0x04)
//end

#define KSV_SHA_START_ADDR					(0xC8)
#define KSV_SHA_START1						(0xC8)  // KSV SHA start address [7:0] = bits [7:0]
#define KSV_SHA_START2						(0xC9)  // KSV SHA start address [9:8] = bits [1:0]
#define SHA_LENGTH_ADDR						(0xCA)
#define SHA_LENGTH1							(0xCA)  // SHA_length [7:0]     = bits [7:0]
#define SHA_LENGTH2							(0xCB)  // SHA_length [9:8] = bits [1:0]
#define SHA_CONTROL_ADDR					(0xCC)  // SHA Control
#define		BIT_M0_READ_EN					(0x08)  // M0 readable (1=external, 0-default=internal)
//alter in 1.4  (0x02) -> (0x01)  In SHA_CONTROL_ADDR bit[1] start and dispaly processing state part-time
#define		BIT_SHA_DONE					(0x01)  // The SHA generator picked up the SHA GO START, write a "1" to clear before any new SHA GO START
//end
#define		BIT_SHA_GO_START				(0x01)  // Start the SHA generator
#define KSV_FIFO_ADDR						(0xCD)  // FIFO data
#define DS_BSTATUS_ADDR						(0xCE)
#define DS_BSTATUS_ADDRL					(0xCE)  // Device BStatus 1
#define		BIT_BSTATUS_HDMI_DEV_CNT_ERR	(0x80)  // HDMI Device count exceeded max
#define		BIT_BSTATUS_HDMI_DEV_CNT		(0x7F)  // HDMI Device count
#define DS_BSTATUS_ADDRH					(0xCF)  // Device BStatus 2
#define		BIT_BSTATUS2_HDMI_MODE			(0x10)  // HDMI Mode
#define		BIT_BSTATUS2_HDMI_DEPTH_ERR		(0x08)  // HDMI Device Depth exceeded max
#define		BIT_BSTATUS2_HDMI_DEPTH			(0x07)  // HDMI Device Depth

//del in 1.4  0xD0~0xD7
//#define DS_M0_ADDR							(0xD0)  // Starting address of M0 ( 8 bytes - 0xD0 through 0xD7)
//end

#define V_ADDR								(0xD8)  // Starting address of V  (20 bytes - 0xD8 through 0xEB)

#define DDC_STATUS                          (0xF2)
#define DDC_DELAY_CNT                       (0xF6)

#define HRES_L                              (0x3A)  //h_RES[7:0]
#define HRES_H                              (0x3B)  //H_RES[12:8]
#define VRES_L                              (0x3C)  //V_RES[7:0]
#define VRES_H                              (0x3D)  //V_RES[10:8]

#define REG_BANK0                           (0xFF)  
#define		BIT_REG_BANK0_HDMI_W_EN         (0x80)  // HDMI digit logic out enable

//-----------------------------------------------------
#define TX_SLV1								(0x7A)
//-----------------------------------------------------
#define ACR_CTRL_ADDR						(0x01)
#define FREQ_SVAL_ADDR						(0x02)
#define N_SVAL_ADDR							(0x03)
#define HWCTS_VAL_ADDR						(0x09)

#define AUD_MODE_ADDR						(0x14)
#define SPDIF_CTRL_ADDR						(0x15)
#define SPDIF_SSMPL_ADDR					(0x16)
#define SPDIF_FS_ADDR						(0x17)
#define SPDIF_HWFS_ADDR						(0x18)
#define SD_0_3_EN							(0xF0)
#define SD_0								(0x10)
#define	BIT_AUDIO_EN						(0x01)
#define	BIT_SPDIF_SELECT					(0x02)
#define	BIT_DSD_SELECT						(0x08)
#define	BIT_SPDIF_SAMPLE					(0x04)
#define	BIT_HBR_ON							(0x80)
#define	SCK_RISING							(0x40)							
#define	BIT_CBIT_ORDER						(0x20)
#define	BIT_COMPRESSED						(0x10)
#define I2S_IN_CTRL_ADDR					(0x1D)
#define	SETUP_ENABLE_HBRA					(0xF1)					
#define HBRA_IN_CTRL_VAL					(0x92)
#define HBRA_ZERO_PLUS						(0x01)						
#define NON_PCM_TYPE						(0x04)						
#define HBRA_FOR_CHST4						(0x09)
#define HBRA_FOR_CHST5						(0xE2)						
#define I2S_CHST0_ADDR						(0x1E)
#define I2S_CHST1_ADDR						(0x1F)
#define I2S_CHST2_ADDR						(0x20)
#define I2S_CHST4_ADDR						(0x21)
#define I2S_CHST5_ADDR						(0x22)
#define SAMPLE_RATE_CONVERSION				(0x23)
#define HBR_SPR_MASK						(0x00)
#define I2S_IN_LEN							(0x24)							
#define AUDP_TXCTRL_ADDR					(0x2F) /*control register, Audio packet header layout & HDMI mode*//*CNcomment:¿ØÖÆ¼Ä´æÆ÷£¬Audio packet header layout & HDMI mode */
#define		BIT_DEEPCOLOR_EN				(0x40)
#define		BIT_TXHDMI_MODE					(0x01)
#define		BIT_EN_AUDIO					(0x01)
#define		BIT_LAYOUT						(0x02)
#define		BIT_LAYOUT1						(0x02)
#define TEST_TX_ADDR						(0x3C)
#define		BIT_DVI_ENC_BYPASS				(0x08)
#define DIAG_PD_ADDR						(0x3D)
#define INF_CTRL1							(0x3E)
#define INF_CTRL2							(0x3F)
#define		BIT_AVI_REPEAT					(0x01)
#define		BIT_AVI_ENABLE					(0x02)
#define		BIT_SPD_REPEAT					(0x04)
#define		BIT_SPD_ENABLE					(0x08)
#define		BIT_AUD_REPEAT					(0x10)
#define		BIT_AUD_ENABLE					(0x20)
#define		BIT_MPEG_REPEAT					(0x40)
#define		BIT_MPEG_ENABLE					(0x80)
#define		BIT_GENERIC_REPEAT				(0x01)
#define		BIT_GENERIC_ENABLE				(0x02)
#define		BIT_CP_REPEAT					(0x04)
#define		BIT_CP_ENABLE					(0x08)
#define 	FOUR_LSB						(0x0F)						
#define		AUDIO_IFOFRAMES_EN_RPT			(0x30)

#define HDMI_DEV_CEC					(0xC0)
#define HDMI_HDCP_SUCCESS 0X1000


#endif
