/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef __CRC16_H__
#define __CRC16_H__

void SI_UpdateCRC16WithByte( HI_U16 * , HI_U8 );
void SI_UpdateCRC16WithBlock(  HI_U16 * , HI_U8 , HI_U8 *);
void TestCRC(void);

#endif

