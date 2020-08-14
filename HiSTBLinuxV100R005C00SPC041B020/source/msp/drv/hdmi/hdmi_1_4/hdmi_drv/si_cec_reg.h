//------------------------------------------------------------------------------
// Copyright 2007, Silicon Image, Inc.  All rights reserved.
//
// No part of this work may be reproduced, modified, distributed, transmitted,
// transcribed, or translated into any language or computer format, in any form
// or by any means without written permission of: Silicon Image, Inc.,
// 1060 East Arques Avenue, Sunnyvale, California 94085
//------------------------------------------------------------------------------
#ifndef __CEC_REGISTERS_H__
#define __CEC_REGISTERS_H__


//------------------------------------------------------------------------------
// Registers in Page 8 for cec
//------------------------------------------------------------------------------
#define REG__CEC_DEV_ID		    0x80  //CEC Device ID Register
#define REG__CEC_SPEC		    0x81  //CEC Specification Register
#define REG__CEC_SUFF		    0x82  //CEC Specification Suffix Register
#define REG__CEC_FW      		0x83  //CEC Firmware Revision Register
#define REG__CEC_DEBUG_0		0x84  //CEC Debug Register#0
#define REG__CEC_DEBUG_1		0x85  //CEC Debug Register#1
#define REG__CEC_DEBUG_2		0x86  //CEC Debug Register#2
#define	REG__CEC_DEBUG_3		0x87  //CEC Debug Register#3
#define BIT_SNOOP_EN            0x01
#define BIT_FLUSH_TX_FIFO       0x80

#define REG__CEC_TX_INIT		0x88  //CEC Tx Initialization Register

#define REG__CEC_TX_DEST		0x89  //CEC Tx Destination Register
#define BIT_SEND_POLL       	0x80

#define REG__CEC_CONFIG_CPI		0x8e  //CEC SetUp Register
#define BIT__CEC_PASS_THROUGH	0x10

#define REG__CEC_TX_COMMAND		0x8f  //CEC Tx Command Register:opcode

#define REG__CEC_TX_OPERANDS_0  0x90  //15Bytes:CEC Tx Operand#0~#14 Register

#define REG__CEC_TRANSMIT_DATA  0x9f  //CEC Transmit Data Register
#define	BIT__TX_AUTO_CALC		0x20
#define BIT_TRANSMIT_CMD        0x10

#define REG__CEC_RETRY_LIMIT    0xa0  //CEC Tx Retry Limit

#define REG__CEC_CAPTURE_ID0    0xa2  //2Btes:CEC Capture ID0 Register

#define REG__CEC_INT_ENABLE_0	0xa4  //CEC Interrupt Enable Register#0
#define REG__CEC_INT_ENABLE_1	0xa5  //CEC Interrupt Enable Register#2

#define REG__CEC_AUTO_PING_CTRL 0xE0
#define REG__CEC_AUTO_PING_MAP0 0xE1
#define REG__CEC_AUTO_PING_MAP1 0xE2

// 0xA6 CPI Interrupt Status Register (R/W)
#define REG__CEC_INT_STATUS_0   0xa6  //CEC Interrupt Status Register#0
#define BIT_CEC_LINE_STATE      0x80
#define BIT_TX_MESSAGE_SENT     0x20
#define BIT_TX_HOTPLUG          0x10
#define BIT_POWER_STAT_CHANGE   0x08
#define BIT_TX_FIFO_EMPTY       0x04
#define BIT_RX_MSG_RECEIVED     0x02
#define BIT_CMD_RECEIVED        0x01
// 0xA7 CPI Interrupt Status Register (R/W)
#define REG__CEC_INT_STATUS_1   0xa7  //CEC Interrupt Status Register#1
#define BIT_RX_FIFO_OVERRUN     0x08
#define BIT_SHORT_PULSE_DET     0x04
#define BIT_FRAME_RETRANSM_OV   0x02
#define BIT_START_IRREGULAR     0x01

#define REG__CEC_RX_CONTROL     0xac  //CEC RX Control Register
// CEC  CEC_RX_CONTROL bits
#define BIT_CLR_RX_FIFO_CUR     0x01
#define BIT_CLR_RX_FIFO_ALL     0x02

#define REG__CEC_RX_COUNT       0xad  //CEC Rx Count Register
#define BIT_MSG_ERROR           0x80

#define REG__CEC_RX_CMD_HEADER	0xae  //CEC Rx Command Header Register
#define CEC_RX_COMMAND          0xaf   //CEC Rx Command Register:opcode
#define REG__CEC_RX_OPERAND_0 	0xb0  //15Bytes:CEC Rx Operand#0~#14  Register

#define REG__CEC_OP_ABORT1      0xc0  //CEC feature abort registers1
#define REG__CEC_OP_ABORT32     0xdf  //CEC feature abort registers32

#define REG__CEC_AUTO_PING_CTL  0xe0  //CEC Auto Ping Control Register
#define CEC_AUTO_PING_MAP0   	0xe1  //CEC Auto Ping mapping Registers0
#define CEC_AUTO_PING_MAP1   	0xe2  //CEC Auto Ping mapping Registers1

#define CEC_RX_CMD_CNT          0x70
#define CEC_RX_BYTE_CNT         0x0F
#endif  // __CEC_REGISTERS_H__
