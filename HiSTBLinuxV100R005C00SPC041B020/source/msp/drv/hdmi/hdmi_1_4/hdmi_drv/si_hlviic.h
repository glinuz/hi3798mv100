/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _HLVIIC_
#define _HLVIIC_

#include "si_typedefs.h"

#define FLG_SHORT 0x01 // Used for Ri Short Read
#define FLG_NOSTOP 0x02 // Don't release IIC Bus
#define FLG_CONTD 0x04 // Continued from previous operation

//#define IIC_CAPTURED 0xF0
//#define NO_ACK_FROM_IIC_DEV 0xF1
//#define MDDC_IIC_CAPTURED 0xF2
//#define MDDC_NO_ACK_FROM_IIC_DEV 0xF3


#define _IIC_CAPTURED  1
#define _IIC_NOACK     2
#define _MDDC_CAPTURED 3
#define _MDDC_NOACK    4
#define _MDDC_FIFO_FULL  5
#define IIC_OK 0

typedef struct {

    HI_U8 Bus;
    HI_U8 SlaveAddr;
    HI_U8 Flags;
    HI_U8 NBytes;
    HI_U8 RegAddrL;
    HI_U8 RegAddrH;
} I2CShortCommandType;



HI_U8 BlockRead_8BAS(I2CShortCommandType * , HI_U8 * );
HI_U8 BlockWrite_8BAS( I2CShortCommandType * , HI_U8 * );
HI_U8 ReadByte_8BA(HI_U8, HI_U8, HI_U8);
HI_U16 ReadWord_8BA(HI_U8, HI_U8 , HI_U8);
void WriteWord_8BA(HI_U8, HI_U8 , HI_U8, HI_U16 );
void WriteByte_8BA(HI_U8, HI_U8 , HI_U8, HI_U8 );

HI_U8 SI_Region2_ReadByte(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr);
void SI_Region2_WriteByte(HI_U8 Bus, HI_U8 SlaveAddr, HI_U8 RegAddr, HI_U8 Data);

HI_S32 SI_HdmiHardwareReset(int iEnable);

#endif

