/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  unf_osal.c
* Description:  Define interfaces of demux hardware abstract layer.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20090927    y00106256      NULL      Create this file.
***********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /*cplusplus*/
#include "drv_demux_osal.h"
#include <linux/hrtimer.h>

HI_VOID DMX_AcrtUsSleep(HI_U32 us)
{
    ktime_t expires;

    expires = ktime_add_ns(ktime_get(), us*1000);
    set_current_state(TASK_UNINTERRUPTIBLE);
    schedule_hrtimeout(&expires, HRTIMER_MODE_ABS);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*cplusplus*/

