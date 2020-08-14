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

#ifndef _DX_PAL_ABORT_PLAT_H
#define _DX_PAL_ABORT_PLAT_H

#include "dx_pal_log.h"

#define _DX_PAL_Abort(exp)	do {					\
	DX_PAL_LOG_ERR("ASSERT:%s:%d: %s", __FILE__, __LINE__, (#exp));	\
	abort();							\
	} while (0)

#endif
