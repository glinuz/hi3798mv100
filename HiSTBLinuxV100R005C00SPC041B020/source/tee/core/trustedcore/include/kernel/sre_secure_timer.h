#ifndef __SECURE_TIMER_H__
#define __SECURE_TIMER_H__

#include "sre_ticktimer.h"
#define TIMER_COUNT_MAX	    0xFFFFFFFF
/**
* @brief
*/
enum timer_mode {
	MODE_FREE_RUNNING,
	MODE_PERIODIC,
	MODE_ONESHOT
};

VOID SRE_TimerInit(VOID);
VOID SRE_TimerInitForSR(VOID);
UINT32 SRE_ReadFreeRunningCntr(VOID);
VOID SRE_TriggerTick(UINT64 usecs, int timer_class);
VOID SRE_Clockcycles2timeval(UINT32 clockcycles, INT32 *pSec, INT32 *pNsec);
VOID SRE_SecureTimerInit();
VOID SRE_SecureTimerClearirq(UINT32 timer_base, UINT32 timer_module_index);
VOID SRE_SecureTimerDisable(UINT32 timer_base, UINT32 timer_module_index);
VOID SRE_SecurePeripTimerEnable(VOID);
VOID SRE_SecureTimerEnable(UINT32 timer_base, UINT32 timer_module_index);
UINT32 SRE_SecureTimerReadMis(UINT32 timer_base, UINT32 tim_mod_index);
VOID SRE_SecureSetTimer(UINT32 timer_base, UINT32 tim_mod_index, UINT32 mode, UINT32 usecs);
UINT32 SRE_SecureReadTimer(UINT32 timer_base, UINT32 tim_mod_index);
VOID SRE_FreeRunningfiqhandler();
VOID SRE_OneShotfiqhandler();
VOID SRE_RTCOneShotfiqhandler(UINT32 uwHwiNum);
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
VOID SRE_FreeRunningAndOneShotfiqhandler();
#endif
VOID  SRE_wdt_fiqhandler();
UINT64 SRE_Timeval2clockcycles(timeval_t *time, int timer_class);
VOID SRE_EnableTimer(VOID);

#endif /* __SECURE_TIMER_H__ */

