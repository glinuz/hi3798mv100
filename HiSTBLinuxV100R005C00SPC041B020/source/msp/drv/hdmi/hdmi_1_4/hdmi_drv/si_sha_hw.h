/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"

#ifndef _SHA_HW_
#define _SHA_HW_

#define NDEV_MASK 0x7F
#define SHA_GO 0x01
#define SHA_WITH_DS_M0 0x04

Bool SHA_HW_Handler( void );

#endif

