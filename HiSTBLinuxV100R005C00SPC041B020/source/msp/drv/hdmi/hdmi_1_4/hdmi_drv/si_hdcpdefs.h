/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/

#ifndef __SI_HDCPDEFS_H__
#define __SI_HDCPDEFS_H__

#define HDCPRX_SLV 0x74
//--------------------------------------------------------------
// HDCP DDC side (HDCP receiver or repearer)

#define DDC_BKSV_ADDR 0x00  // 5 bytes, Read only (RO), receiver KSV
#define DDC_Ri_ADDR     0x08    // Ack from receiver RO
#define DDC_AKSV_ADDR       0x10    // 5 bytes WO, transmitter KSV
#define DDC_AN_ADDR         0x18    // 8 bytes WO, random value.
#define DDC_V_ADDR          0x20    // 20 bytes RO
#define DDC_RSVD_ADDR       0x34    // 12 HI_U8 RO
#define DDC_BCAPS_ADDR      0x40    // Capabilities Status HI_U8 RO

#define DDC_BIT_HDMI_CAP    0x80
#define DDC_BIT_REPEATER    0x40
#define DDC_BIT_FIFO_READY  0x20
#define DDC_BIT_FAST_I2C    0x10

#define DDC_BSTATUS_ADDR         0x41
#define DDC_BSTATUS_1_ADDR       0x41
#define DDC_BSTATUS_2_ADDR       0x42

#define DDC_BIT_BSTATUS1_DEV_EXC   0x80 //device count exceeded
#define DDC_BIT_BSTATUS1_DEV_COUNT 0x7f //device count

#define DDC_BIT_BSTATUS2_HDMI_MODE  0x10//1-device is in HDMI mode, 0-device is in DVI mode
#define DDC_BIT_BSTATUS2_CAS_EXC    0x08//cascade depth exceeded
#define DDC_BIT_BSTATUS2_DEV_DEPTH  0X07//device depth

#define DDC_BIT_HDMI_MODE        0x10

#define DDC_KSV_FIFO_ADDR        0x43

#define _DVI_  0
#define _HDMI_ 0x01


//-------------------------------------------------------------
#define NO_HDCP							0x00
#define NO_DEVICE_WITH_HDCP_SLAVE_ADDR	0x01
#define BKSV_ERROR						0x02
#define R0s_ARE_MISSMATCH				0x03
#define RIs_ARE_MISSMATCH				0x04
#define REAUTHENTATION_REQ				0x05
#define REQ_AUTHENTICATION				0x06
#define NO_ACK_FROM_HDCP_DEV			0x07
#define NO_RSEN							0x08
#define AUTHENTICATED					0x09
#define REPEATER_AUTH_REQ				0x0A
#define REQ_SHA_CALC					0x0B
#define REQ_SHA_HW_CALC					0x0C	// 9032 added
#define FAILED_ViERROR					0x0D	// 9032 moved

#endif

