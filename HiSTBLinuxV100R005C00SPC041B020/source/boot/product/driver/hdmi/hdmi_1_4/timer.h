/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "typedefs.h"

#ifndef _TIMER_
#define _TIMER_

#define ProgrVPosHPos 0x03

/// //#define ProgrVNegHPos 0x02
#define ProgrVNegHPos 0x01

/// //#define ProgrVPosHNeg 0x01
#define ProgrVPosHNeg 0x02

#define ProgrVNegHNeg 0x00
#define InterlaceVPosHPos 0x07
#define InterlaceVNgeHPos 0x06
#define InterlaceVPosHNeg 0x05
#define InterlaceVNegHNeg 0x04
void GetTimer(void);
void ResetTimer(void);


#endif

