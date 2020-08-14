
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "cc_debug.h"
#include "cc_timer.h"

#define MAX_CC_TIMER ((HI_S32)CCTIMER_ID_BUTT)
#define USEC_PER_TICK  (1000)

typedef struct tagCCTIMER_HOOK_S
{
    HI_BOOL bStart;
    HI_VOID (*pfnHook)(HI_U32 u32Args);
    HI_U32 u32Args;
    HI_U32 u32Interval;
    HI_U32 u32Escape;
    CCTIMER_MODE_E enMode;
} CCTIMER_HOOK_S;

typedef struct tagCCTIMER_ATTR_S
{
    pthread_t thread;
    pthread_mutex_t mutex;
    HI_BOOL bRun;
    CCTIMER_HOOK_S astHook[MAX_CC_TIMER];
} CCTIMER_ATTR_S;

static CCTIMER_ATTR_S *s_pstCCTimer = HI_NULL;

static HI_VOID _cctimer_trigger(CCTIMER_HOOK_S *pstHook, HI_U32 escape)
{
    if (HI_TRUE == pstHook->bStart)
    {
        pstHook->u32Escape += escape;
        if(pstHook->u32Escape >= pstHook->u32Interval)
        {
            if (pstHook->pfnHook)
            {
                pstHook->pfnHook(pstHook->u32Args);
            }
            if (TIMER_MODE_ONE_SHOOT == pstHook->enMode)
            {
                pstHook->bStart = HI_FALSE;
            }
            pstHook->u32Escape = 0;
        }
    }
}

static HI_VOID * _cctimer_task(HI_VOID *pvArgs)
{
    CCTIMER_ATTR_S *pstTimer = (CCTIMER_ATTR_S *)pvArgs;
    struct timeval tickval;
    HI_S32 i = 0;

    if (HI_NULL == pstTimer)
    {
        HI_ERR_CC("param is error\n");
        return NULL;
    }

    while (HI_TRUE == pstTimer->bRun)
    {
        tickval.tv_sec = 0;
        tickval.tv_usec = USEC_PER_TICK;

        (HI_VOID)select(0, NULL, NULL, NULL, &tickval); /* I don't care if it is fail */

        for (i = 0; i < MAX_CC_TIMER; i++)
        {
            _cctimer_trigger(pstTimer->astHook + i, 1);
        }
    }

    return NULL;
}

HI_S32 CCTimer_Init(void)
{
    if (HI_NULL == s_pstCCTimer)
    {
        s_pstCCTimer = (CCTIMER_ATTR_S *)malloc(sizeof(CCTIMER_ATTR_S));
        if (HI_NULL == s_pstCCTimer)
        {
            HI_ERR_CC("alloc failed\n");
            return HI_FAILURE;
        }
        (HI_VOID)memset(s_pstCCTimer, 0, sizeof(CCTIMER_ATTR_S));

        s_pstCCTimer->bRun = HI_TRUE;

        (HI_VOID)pthread_mutex_init(&s_pstCCTimer->mutex, NULL);

        (HI_VOID)pthread_create(&s_pstCCTimer->thread, HI_NULL, (void * (*)(void *))_cctimer_task, (HI_VOID *)s_pstCCTimer);
    }

    return HI_SUCCESS;
}

HI_S32 CCTimer_DeInit(void)
{
    if (s_pstCCTimer)
    {
        s_pstCCTimer->bRun = HI_FALSE;

        (HI_VOID)pthread_join(s_pstCCTimer->thread, HI_NULL);

        (HI_VOID)pthread_mutex_destroy(&s_pstCCTimer->mutex);

        free(s_pstCCTimer);
        s_pstCCTimer = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_S32 CCTimer_Open(CCTIMER_ID_E enTimerID, HI_VOID (*pfnHook)(HI_U32), HI_U32 u32Args)
{
    HI_S32 s32TimerID = enTimerID;
    CCTIMER_HOOK_S *pstHook = HI_NULL;

    if (s32TimerID >= MAX_CC_TIMER)
    {
        HI_ERR_CC("TimerID : %d is invalid\n", s32TimerID);
        return HI_FAILURE;
    }
    if (HI_NULL == s_pstCCTimer)
    {
        HI_ERR_CC("cctimer not init yet\n");
        return HI_FAILURE;
    }

    pstHook = s_pstCCTimer->astHook + s32TimerID;

    pstHook->pfnHook = pfnHook;
    pstHook->u32Args = u32Args;
    pstHook->bStart = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 CCTimer_Close(CCTIMER_ID_E enTimerID)
{
    return CCTimer_Stop(enTimerID);
}

HI_S32 CCTimer_Start(CCTIMER_ID_E enTimerID, HI_U32 u32Msec, CCTIMER_MODE_E enMode)
{
    HI_S32 s32TimerID = enTimerID;
    CCTIMER_HOOK_S *pstHook = HI_NULL;

    if (s32TimerID >= MAX_CC_TIMER)
    {
        HI_ERR_CC("TimerID : %d is invalid\n", s32TimerID);
        return HI_FAILURE;
    }
    if (HI_NULL == s_pstCCTimer)
    {
        HI_ERR_CC("cctimer not init yet\n");
        return HI_FAILURE;
    }

    pstHook = s_pstCCTimer->astHook + s32TimerID;

    pstHook->bStart = HI_TRUE;
    pstHook->u32Interval = u32Msec;
    pstHook->u32Escape = 0;
    pstHook->enMode = enMode;

    return HI_SUCCESS;
}

HI_S32 CCTimer_Stop(CCTIMER_ID_E enTimerID)
{
    HI_S32 s32TimerID = enTimerID;
    CCTIMER_HOOK_S *pstHook = HI_NULL;

    if (s32TimerID >= MAX_CC_TIMER)
    {
        HI_ERR_CC("TimerID : %d is invalid\n", s32TimerID);
        return HI_FAILURE;
    }
    if (HI_NULL == s_pstCCTimer)
    {
        HI_ERR_CC("cctimer not init yet\n");
        return HI_FAILURE;
    }

    pstHook = s_pstCCTimer->astHook + s32TimerID;

    pstHook->bStart = HI_FALSE;

    return HI_SUCCESS;
}

