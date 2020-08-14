/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_drv_tx_regs_enums.h
*
* @brief Cypress register definition
*
*****************************************************************************/
#ifndef __SI_DRV_TX_REGS_ENUMS_H__
#define __SI_DRV_TX_REGS_ENUMS_H__

#include "si_drv_tx_regs.h"

//Few Register Enum Declarations
//Page 0:0xF3 - REG_ADDR__DDC_CMD
#define BIT_ENUM__DDC_CMD__SEQUENTIAL_READ				0x02
#define BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ			0x04
#define BIT_ENUM__DDC_CMD__SEQUENTIAL_WRITE				0x06
#define BIT_ENUM__DDC_CMD__CLEAR_FIFO					0x09
#define BIT_ENUM__DDC_CMD__ABORT_TRANSACTION			0x0F


//Page 8:0x0C - REG_ADDR__HDCP2X_AUTH_STAT
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_DONE			0x01
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_FAIL			0x02
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE			0x10
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL			0x20
#define HDCP2X_CCHK_FAIL								( BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE | BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL )

//Page 6:0x29 - REG_ADDR__TPI_COPP_DATA1
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL        0x00
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST          0x10
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED        0x20
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_RSVD		   0x30

//Page 6:0xBF - REG_ADDR__TPI_INFO_FSEL
#define BIT_ENUM__TPI_INFO_FSEL__AVI                  0x00
#define BIT_ENUM__TPI_INFO_FSEL__GBD                  0x01
#define BIT_ENUM__TPI_INFO_FSEL__AUDIO                0x02
#define BIT_ENUM__TPI_INFO_FSEL__SPD                  0x03
#define BIT_ENUM__TPI_INFO_FSEL__MPEG                 0x04
#define BIT_ENUM__TPI_INFO_FSEL__VSIF                 0x05
#define BIT_ENUM__TPI_INFO_FSEL__GEN1                 0x06  // use this for ISRC
#define BIT_ENUM__TPI_INFO_FSEL__GEN2                 0x07  // use this for ISRC2
#define BIT_ENUM__TPI_INFO_FSEL__GEN3                 0x08  // use this for ACP
#define BIT_ENUM__TPI_INFO_FSEL__GEN4                 0x09  // use this for DRM
#define BIT_ENUM__TPI_INFO_FSEL__GEN5                 0x0A

//Page A:0x61 - REG_ADDR__TPI_DOWN_SMPL_CTRL
#define BIT_ENUM__TPI_DOWN_SMPL_CTRL__AUDIO_DOWNSAMP_EN	0x02

//Page 5:0xE3 - REG_ADDR__DISC_CTRL4
#define BIT_ENUM__DISC_CTRL4__CBUSDISC_PUP_SEL_10K		0x80

//Page 4:0x30 - REG_ADDR__MHL_STAT_0
#define MHL_REG_CONNECTED_RDY_OFFSET			0x30
#define BIT_ENUM__MHL_STAT_0__DCAP_READY		0x01
#define BIT_ENUM__MHL_STAT_0__XDEVCAP_SUPP		0x02
#define BIT_ENUM__MHL_STAT_0__POW_STAT			0x04
#define BIT_ENUM__MHL_STAT_0__PLIM_STAT_MASK	0x38

//Page 4:0x31 - REG_ADDR__MHL_STAT_1
#define MHL_REG_LINK_MODE_OFFSET				0x31
#define BIT_ENUM__MHL_STAT_1__CLK_MODE_MASK		0x07	//Bits2:0
#define CLK_MODE_PACKED_PIXEL					0x02
#define CLK_MODE_NORMAL							0x03
#define BIT_ENUM__MHL_STAT_1__PATH_EN			0x08	//Bit3
#define BIT_ENUM__MHL_STAT_1__MUTED				0x10	//Bit4

#define MHL_REG_VERSION_STAT_OFFSET				0x32
#define BIT_ENUM__MHL_STAT_2__MHL_VER_MINOR		0x0
#define BIT_ENUM__MHL_STAT_2__MHL_VER_MAJOR		0x30

//Offsets of Registers
#define MHL_REG_RCHANGE_INT_OFFSET				0x20
#define MHL_REG_DCHANGE_INT_OFFSET				0x21
#define MHL_SCRATCHPAD_OFFSET					0x40

//Page_6:0x3B - REG_ADDR__TPI_HPD_RSEN
#define BIT_MSK__TPI_HPD_STATE						  0x05
#define BIT_MSK__TPI_RSEN_STATE						  0x50
//Page_0:0x65 - REG_ADDR__CLKRATIO
#define BIT_MSK__CLKRATIO__REG_PCLKNX2PCLK_RATIO_DEFAULT	0x04
#define BIT_MSK__CLKRATIO__REG_IDCK2PCLK_RATIO_DEFAULT		0x01

#endif // __SI_DRV_TX_REGS_ENUMS_H__
