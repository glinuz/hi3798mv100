/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"

#define POLYNOM 0x1021
#define MASK_LSBit  0x0001
//---------------------------------------------------------
void SI_UpdateCRC16WithByte( HI_U16 * pCRC, HI_U8 Data){

HI_U8 i;
HI_U16 XORedIn, CRC;

 CRC = *pCRC;
 for(i = 0; i < 8; i++) {
   XORedIn = (HI_U16)Data ^ CRC ; Data >>=1;
   CRC >>=1;
   if(XORedIn & MASK_LSBit)
      CRC ^=POLYNOM;
 }
 *pCRC = CRC;
    return ;
}
//---------------------------------------------------------
void SI_UpdateCRC16WithBlock(  HI_U16 * pCRC, HI_U8 NBytes, HI_U8 * Data){
HI_U8 i;

   for (i = 0; i < NBytes; i++)
     SI_UpdateCRC16WithByte(  pCRC, Data[i]);
    return ;
}

//---------------------------------------------------------


