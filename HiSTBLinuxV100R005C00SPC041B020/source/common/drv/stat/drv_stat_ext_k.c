#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include "hi_type.h"
#include "hi_drv_stat.h"
#include "drv_stat_ioctl.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

static  STAT_Event_Fun   stat_event_fun = NULL;

HI_S32 HI_DRV_STAT_EventFunc_Register(HI_VOID* pFunc)
{
    if (NULL == pFunc)
    {
        return HI_FAILURE;
    }
	stat_event_fun = (STAT_Event_Fun)pFunc;
	return HI_SUCCESS;
}

HI_VOID HI_DRV_STAT_EventFunc_UnRegister(HI_VOID)
{
	stat_event_fun = NULL;
	return;
}

HI_VOID HI_DRV_STAT_Event(STAT_EVENT_E enEvent, HI_U32 Value)
{
	if(stat_event_fun){
		stat_event_fun(enEvent, Value);
	}
	return;
}

HI_S32 low_delay_start_statistics(HI_LD_Scenes_E scenes_id, HI_HANDLE filter);
HI_S32 HI_DRV_LD_Start_Statistics(HI_LD_Scenes_E scenes_id, HI_HANDLE filter)
{
    return low_delay_start_statistics(scenes_id, filter);
}

HI_VOID low_delay_stop_statistics(HI_VOID);
HI_VOID HI_DRV_LD_Stop_Statistics(HI_VOID)
{
    return low_delay_stop_statistics();
}

HI_VOID low_delay_notify_event( HI_LD_Event_S * evt);
HI_VOID HI_DRV_LD_Notify_Event( HI_LD_Event_S * evt)
{
    return low_delay_notify_event(evt);
}

HI_U32 HI_DRV_STAT_GetTick(HI_VOID)
{
    HI_U64 SysTime;

    SysTime = sched_clock();
    do_div(SysTime, 1000000);
    return (HI_U32)SysTime;
}

HI_S32 HI_DRV_STAT_KInit(void)
{
	stat_event_fun = NULL;
	return HI_SUCCESS;
}

HI_VOID HI_DRV_STAT_KExit(void)
{
	stat_event_fun = NULL;
    return ;
}

#ifndef MODULE
EXPORT_SYMBOL(HI_DRV_STAT_EventFunc_Register);
EXPORT_SYMBOL(HI_DRV_STAT_EventFunc_UnRegister);
#endif
EXPORT_SYMBOL(HI_DRV_STAT_Event);
EXPORT_SYMBOL(HI_DRV_STAT_GetTick);
EXPORT_SYMBOL(HI_DRV_LD_Start_Statistics);
EXPORT_SYMBOL(HI_DRV_LD_Stop_Statistics);
EXPORT_SYMBOL(HI_DRV_LD_Notify_Event);


#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

