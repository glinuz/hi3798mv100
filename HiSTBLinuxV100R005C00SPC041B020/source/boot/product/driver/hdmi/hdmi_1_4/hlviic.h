/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _HLVIIC_
#define _HLVIIC_

#include "hi_type.h"

//#include "typedefs.h"

/*
#define FLG_SHORT 0x01 // Used for Ri Short Read
#define FLG_NOSTOP 0x02 // Don't release IIC Bus
#define FLG_CONTD 0x04 // Continued from previous operation
*/

//#define IIC_CAPTURED 0xF0
//#define NO_ACK_FROM_IIC_DEV 0xF1
//#define MDDC_IIC_CAPTURED 0xF2
//#define MDDC_NO_ACK_FROM_IIC_DEV 0xF3

/*

typedef struct {
    HI_U8 Dummy;
    HI_U8 NBytes;
    HI_U8 Flags;
    HI_U8 Ctrl;
    HI_U8 SlaveAddr;
    HI_U8 RegAddrL;
    HI_U8 RegAddrH;
    HI_U8 Data[16];
} I2CCommandType;

typedef struct {

    HI_U8 Bus;
    HI_U8 SlaveAddr;
    HI_U8 Flags;
    HI_U8 NBytes;
    HI_U8 RegAddrL;
    HI_U8 RegAddrH;
} I2CShortCommandType;

typedef struct {

    I2CShortCommandType cmd;
    HI_U8 *pData;

} I2CShortCommandType2;

typedef struct {

    HI_U8 SlaveAddr;
    HI_U8 Offset;
    HI_U8 RegAddr;
    HI_U8 NBytesLSB;
    HI_U8 NBytesMSB;
    HI_U8 Dummy;
    HI_U8 Cmd;
    HI_U8 * PData;
    HI_U8 Data[6];

} MDDCType;


#define _IIC_CAPTURED  1
#define _IIC_NOACK     2
#define _MDDC_CAPTURED 3
#define _MDDC_NOACK    4
#define _MDDC_FIFO_FULL  5
#define IIC_OK 0
*/

/*
HI_U8 BlockWrite_16BA( I2CCommandType *);
HI_U8 BlockRead_16BA( I2CCommandType *);
HI_U8 BlockRead_8BA(I2CCommandType *);
HI_U8 BlockWrite_8BA( I2CCommandType *);
HI_U8 BlockRead_8BAS(I2CShortCommandType * , HI_U8 * );
HI_U8 BlockWrite_8BAS( I2CShortCommandType * , HI_U8 * );
HI_U8 BlockRead_16BAS(I2CShortCommandType * , HI_U8 * );
HI_U8 BlockWrite_16BAS( I2CShortCommandType * , HI_U8 * );


HI_U16 ReadWord_8BA(HI_U8, HI_U8 , HI_U8);
HI_U16 ReadWord_16BA(HI_U8, HI_U8 , HI_U16);
HI_U8 ReadByte_16BA(HI_U8, HI_U8 , HI_U16);
void WriteWord_8BA(HI_U8, HI_U8 , HI_U8, HI_U16 );

void WriteByte_16BA(HI_U8, HI_U8 , HI_U16, HI_U8 );
void WriteWord_16BA(HI_U8, HI_U8, HI_U16, HI_U16);
HI_U8 WaitForAck(HI_U8 , HI_U8 , HI_U16 );
HI_U8 DoRecoverySCLs(HI_U8 , HI_U8 );
HI_U8 WriteAddr_8BA(HI_U8 , HI_U8 , HI_U8); 

*/

HI_U8 ReadByte_8BA(HI_U8, HI_U8, HI_U8);
void WriteByte_8BA(HI_U8, HI_U8 , HI_U8, HI_U8 );
HI_U16 ReadWord_8BA(HI_U8, HI_U8 , HI_U8);
void WriteWord_8BA(HI_U8, HI_U8 , HI_U8, HI_U16 );


//void HdmiHardwareReset(int iEnable);

#endif

