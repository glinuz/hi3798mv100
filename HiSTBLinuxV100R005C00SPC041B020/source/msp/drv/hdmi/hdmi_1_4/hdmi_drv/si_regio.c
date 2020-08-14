//------------------------------------------------------------------------------
// Copyright ?2007, Silicon Image, Inc.  All rights reserved.
//
// No part of this work may be reproduced, modified, distributed, transmitted,
// transcribed, or translated into any language or computer format, in any form
// or by any means without written permission of: Silicon Image, Inc.,
// 1060 East Arques Avenue, Sunnyvale, California 94085
//------------------------------------------------------------------------------
#include "si_typedefs.h"
#include <si_config.h>
#include <si_hal.h>
#include "si_hdmitx.h"

#if defined (CEC_SUPPORT)
//------------------------------------------------------------------------------
// Function: SI_CEC_RegisterRead
// Description: Read a one HI_U8 register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The I2C slave address and offset are used to perform an I2C read operation.
//------------------------------------------------------------------------------
HI_U8 SI_CEC_RegisterRead(HI_U16 regAddr)
{
    HI_U8 aByte;
    
    aByte = ReadByteHDMICEC(regAddr);
    return aByte;
}

//------------------------------------------------------------------------------
// Function: SI_CEC_RegisterWrite
// Description: Write a one HI_U8 register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The I2C slave address and offset are used to perform an I2C write operation.
//------------------------------------------------------------------------------
void SI_CEC_RegisterWrite(HI_U16 regAddr, HI_U8 value)
{
    WriteByteHDMICEC(regAddr, value);
    return ;
}

//------------------------------------------------------------------------------
// Function: SI_CEC_RegisterModify
// Description: Modify a one HI_U8 register under mask.
//              The register address parameter is translated into an I2C slave address and offset.
//              The I2C slave address and offset are used to perform I2C read and write operations.
//
//              All bits specified in the mask are set in the register according to the value specified.
//              A mask of 0x00 does not change any bits.
//              A mask of 0xFF is the same a writing a HI_U8 - all bits are set to the value given.
//              When only some buts in the mask are set, only those bits are changed to the values given.
//------------------------------------------------------------------------------
void SI_CEC_RegisterModify(HI_U16 regAddr, HI_U8 mask, HI_U8 value)
{
    HI_U8 aByte;

    aByte = ReadByteHDMICEC(regAddr);

    aByte &= (~mask);        //first clear all bits in mask
    aByte |= (mask & value); //then set bits from value

    WriteByteHDMICEC(regAddr, aByte);
    return ;
}

//------------------------------------------------------------------------------
// Function: SI_CEC_RegisterReadBlock
// Description: Read a block of registers starting with the specified register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The block of data bytes is read from the I2C slave address and offset.
//------------------------------------------------------------------------------
void SI_CEC_RegisterReadBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length)
{
    HI_U8 index;

    for(index = 0; index < length; index ++)
    {
        buffer[index] = ReadByteHDMICEC(regAddr+index);
    }
    return ;
}

//------------------------------------------------------------------------------
// Function: SI_CEC_RegisterWriteBlock
// Description: Write a block of registers starting with the specified register.
//              The register address parameter is translated into an I2C slave address and offset.
//              The block of data bytes is written to the I2C slave address and offset.
//------------------------------------------------------------------------------
void SI_CEC_RegisterWriteBlock(HI_U16 regAddr, HI_U8* buffer, HI_U8 length)
{
    HI_U8 index;

    for(index = 0; index < length; index ++)
    {
        WriteByteHDMICEC((regAddr+index), buffer[index]);
    }
    return ;
}

#endif
