/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/



#ifndef _DX_PAL_ABORT_H
#define _DX_PAL_ABORT_H


#include "dx_pal_abort_plat.h"

/**
 * DX_PAL_Abort() - Halt running appplication (system)
 * (Platform independent "abort()")
 */
#define DX_PAL_Abort(msg) _DX_PAL_Abort(msg)

#endif


