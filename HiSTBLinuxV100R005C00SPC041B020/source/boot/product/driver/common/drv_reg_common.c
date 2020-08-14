/******************************************************************************
  Copyright (C), 2013-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : drv_reg_common.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/08/26
  Description   :
******************************************************************************/

#include "hi_type.h"
#include "hi_reg_common.h"

volatile HI_REG_SYSCTRL_S   *g_pstRegSysCtrl    = (HI_REG_SYSCTRL_S*)(HI_SYS_BASE_ADDR);
volatile HI_REG_PERI_S      *g_pstRegPeri       = (HI_REG_PERI_S*)(HI_PERI_BASE_ADDR);
volatile HI_REG_IO_S        *g_pstRegIO         = (HI_REG_IO_S*)(HI_IO_BASE_ADDR);
volatile HI_REG_CRG_S       *g_pstRegCrg        = (HI_REG_CRG_S*)(HI_CRG_BASE_ADDR);

