/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_base_ext_k.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/01/25
  Description   : common driver moudle define
  History       :
  1.Date        : 2010/01/25
    Author      : jianglei¡¡
    Modification: Created file

******************************************************************************/
#include <linux/version.h>
#include <linux/seq_file.h>
#include "hi_type.h"
#include "hi_drv_log.h"
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"
#include "hi_drv_mmz.h"
#include "hi_drv_module.h"

HI_S32 HI_DRV_CommonInit(HI_VOID)
{
    HI_S32 ret;

    ret = HI_DRV_DEV_KInit();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_DEV_KInit failed:%#x!\n", ret);
        goto ErrExit_DEV;
    }

    ret = HI_DRV_REG_KInit();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_REG_KInit failed:%#x!\n", ret);
        goto ErrorExit_REG;
    }

    ret = HI_DRV_LOG_KInit();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_LOG_KInit failed:%#x!\n", ret);
        goto ErrExit_LOG;
    }

    ret = HI_DRV_PROC_KInit();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_PROC_KInit failed:%#x!\n", ret);
        goto ErrExit_PROC;
    }

    ret = HI_DRV_STAT_KInit();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_STAT_KInit failed:%#x!\n", ret);
        goto ErrExit_STAT;
    }

    ret = HI_DRV_MMNGR_Init(HI_KMODULE_MAX_COUNT, HI_KMODULE_MEM_MAX_COUNT);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_SYS("HI_DRV_MMNGR_Init failed:%#x!\n", ret);
        goto ErrExit_SYS;
    }

    return HI_SUCCESS;

ErrExit_SYS:
    HI_DRV_STAT_KExit();

ErrExit_STAT:
    HI_DRV_PROC_KExit();

ErrExit_PROC:
    HI_DRV_LOG_KExit();

ErrExit_LOG:
    HI_DRV_REG_KExit();

ErrorExit_REG:
    HI_DRV_DEV_KExit();

ErrExit_DEV:
    return ret;
}

HI_VOID HI_DRV_CommonExit(HI_VOID)
{
    HI_DRV_MMNGR_Exit();

    HI_DRV_STAT_KExit();

    HI_DRV_PROC_KExit();

    HI_DRV_LOG_KExit();

    HI_DRV_REG_KExit();

    HI_DRV_DEV_KExit();

    return;
}


