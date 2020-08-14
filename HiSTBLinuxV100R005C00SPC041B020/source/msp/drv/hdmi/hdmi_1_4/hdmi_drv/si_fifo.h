/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"

#ifndef __SI_FIFO_H__
#define __SI_FIFO_H__ 

HI_U8 SI_MDDCInitReadBlockFromFIFO(HI_U8, HI_U16 );
HI_U8 SI_MDDCTakeBlockFromFIFO(HI_U8 , HI_U8 * );

#endif
