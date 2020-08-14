/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/
#ifndef __CC_TIMER_H__
#define __CC_TIMER_H__


#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum tagCCTIMER_ID_E
{
    CCTIMER_ID_608ERASE = 0x1,
    CCTIMER_ID_608XDS,
    CCTIMER_ID_708DELAY,
    CCTIMER_ID_708ERASE,
    CCTIMER_ID_BUTT
} CCTIMER_ID_E;

typedef enum tagCCTIMER_MODE_E
{
    TIMER_MODE_AUTO_RESTART = 0, /* the timer re-arms itself */
    TIMER_MODE_ONE_SHOOT,        /* the timer start once and then stop */
    TIMER_MODE_BUTT
} CCTIMER_MODE_E;

HI_S32 CCTimer_Init(void);

HI_S32 CCTimer_DeInit(void);

HI_S32 CCTimer_Open(CCTIMER_ID_E enTimerID, HI_VOID (*pfnHook)(HI_U32), HI_U32 u32Args);

HI_S32 CCTimer_Close(CCTIMER_ID_E enTimerID);

HI_S32 CCTimer_Start(CCTIMER_ID_E enTimerID, HI_U32 u32Msec, CCTIMER_MODE_E enMode);

HI_S32 CCTimer_Stop(CCTIMER_ID_E enTimerID);

#ifdef __cplusplus
}
#endif

#endif


