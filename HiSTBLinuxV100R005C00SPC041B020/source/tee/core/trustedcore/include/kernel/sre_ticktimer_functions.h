
#ifndef TIMER_FUNCTIONS_H
#define TIMER_FUNCTIONS_H


#include "sre_base.h"
#include "sre_ticktimer.h"


extern UINT32 SRE_WakeUpfromSleep(timer_event *pstTevent);
extern UINT32 SRE_SwSleep(UINT32 uwSecs);
extern UINT32 SRE_SwMsleep(UINT32 uwMsecs);
extern UINT32 SRE_SwSleepTimer(timeval_t time);

#endif
