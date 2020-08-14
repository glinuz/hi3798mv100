/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_hlviic.h"
#include "si_globdefs.h"
#include "si_mddc.h"
#ifndef __SI_GLOBAL_H_
#define __SI_GLOBAL_H_

typedef union TmpData {
    HI_U8 FuncRes[16];
    MDDCType MDDC;
} TmpDType;

#endif
