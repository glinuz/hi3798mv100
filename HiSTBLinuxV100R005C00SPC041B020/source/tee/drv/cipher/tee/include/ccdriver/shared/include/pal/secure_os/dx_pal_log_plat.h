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

#ifndef _DX_PAL_LOG_PLAT_H_
#define _DX_PAL_LOG_PLAT_H_

#include "dx_log_mask.h"
#include "debug.h"
//#include <syslog.h>

/************** PRINTF rules ******************/
#if defined(DEBUG)

#define PRINTF PRINT_INFO

#else /* Disable all prints */

#define PRINTF(...)  do {} while (0)

#endif

#define __DX_PAL_LOG_PLAT(level, format, ...) PRINT_INFO(format, ##__VA_ARGS__)

#endif /*_DX_PAL_LOG_PLAT_H_*/
