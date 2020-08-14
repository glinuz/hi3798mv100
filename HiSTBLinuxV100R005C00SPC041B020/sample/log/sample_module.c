/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_module.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/26
  Description   :
******************************************************************************/
#include <stdlib.h>

#include "hi_debug.h"
#include "hi_common.h"

HI_MODULE_DECLARE("SAMPLE_MOD");

HI_VOID Module_Printf(HI_VOID)
{
    HI_MODULE_ERROR("Print test in Module_Printf().\n");
}

