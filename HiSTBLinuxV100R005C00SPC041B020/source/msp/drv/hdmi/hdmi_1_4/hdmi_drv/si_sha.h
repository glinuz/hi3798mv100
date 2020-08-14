/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_shadefs.h"

#ifndef __SI_SHA_H__
#define __SI_SHA_H__

typedef struct{
        HI_U32 Digest[5];
        HI_U32 Data[16];
        HI_U16 byteCounter;
        HI_U8 BStatusMXCounter;
} SHA_CTX;

void SI_SHAHandler(void);

#endif
