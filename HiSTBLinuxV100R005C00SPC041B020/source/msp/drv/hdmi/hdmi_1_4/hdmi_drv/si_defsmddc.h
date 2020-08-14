/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#define MASTER_BASE                0xEC
#define MDDC_MANUAL_ADDR           0xEC                               // Register Offsets
#define MDDC_SLAVE_ADDR            0xED
#define MDDC_SEGMENT_ADDR          0xEE
#define MDDC_OFFSET_ADDR           0xEF
#define MDDC_DIN_CNT_LSB_ADDR      0xF0
#define MDDC_DIN_CNT_MSB_ADDR      0xF1
#define MDDC_STATUS_ADDR           0xF2
#define MDDC_COMMAND_ADDR          0xF3

#define MDDC_FIFO_ADDR           0xF4
#define MDDC_FIFO_CNT_ADDR       0xF5

#define BIT_MDDC_ST_I2C_LOW     0x40
#define BIT_MDDC_ST_NO_ACK      0x20
#define BIT_MDDC_ST_IN_PROGR    0x10
#define BIT_MDDC_ST_FIFO_FULL   0x08
#define BIT_MDDC_ST_FIFO_EMPL   0x04
#define BIT_MDDC_ST_FRD_USE     0x02
#define BIT_MDDC_ST_FWT_USE     0x01


//      DDC Command[3:0]:
//
//      1111 - Abort transaction
//      1001 - Clear FIFO
//      1010 - Clock SCL
//      0000 - Current address read with no ACK on last HI_U8
//      0001 - Current address read with ACK on last HI_U8
//      0010 - Sequential read with no ACK on last HI_U8
//      0011 - Sequential read with ACK on last HI_U8
//      0100 - Enhanced DDC read with no ACK on last HI_U8
//      0101 - Enhanced DDC read with ACK on last HI_U8
//      0110 - Sequential write ignoring ACK on last HI_U8
//      0111 - Sequential write requiring ACK on last HI_U8


#define MASTER_CMD_ABORT        0x0f                    // Command Codes
#define MASTER_CMD_CLEAR_FIFO   0x09
#define MASTER_CMD_CLOCK        0x0a
#define MASTER_CMD_CUR_RD       0x00
#define MASTER_CMD_SEQ_RD       0x02
#define MASTER_CMD_ENH_RD       0x04
#define MASTER_CMD_SEQ_WR       0x06

#define MASTER_FIFO_WR_USE      0x01
#define MASTER_FIFO_RD_USE      0x02
#define MASTER_FIFO_EMPTY       0x04
#define MASTER_FIFO_FULL        0x08
#define MASTER_DDC_BUSY         0x10
#define MASTER_DDC_NOACK        0x20
#define MASTER_DDC_STUCK        0x40
#define MASTER_DDC_RSVD         0x80

