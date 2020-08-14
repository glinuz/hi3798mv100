#ifndef __DRV_TIMER_COMMON_H__
#define __DRV_TIMER_COMMON_H__

#include "hal_aiao_common.h"

#define  TIMER_MAX_TOTAL_NUM          (2)
#define  TIMER_CHNID_MASK             0xffff

#define TIMER_INTERRUPT_PERIOD_DF  (5)
#define TIMER_INTERRUPT_CONFIG_DF (HI_UNF_SAMPLE_RATE_48K/1000*TIMER_INTERRUPT_PERIOD_DF)

typedef struct
{
    HI_BOOL         bEnable;
    HI_U32          u32Config;
    AIAO_IfTimerAttr_S stIfAttr;

    HI_VOID         *pFunc;
    HI_VOID         *pSubstream;

} TIMER_CHANNEL_STATE_S;

#if defined(HI_SND_DRV_SUSPEND_SUPPORT)
typedef struct
{
    HI_BOOL             bEnable;
    HI_U32              u32Config;
    AIAO_SAMPLE_RATE_E  enRate;
    HI_U32              u32FCLK_DIV;
    HI_U32              u32BCLK_DIV;

    HI_VOID             *pFunc;
    HI_VOID             *pSubstream;

} SND_TIMER_SETTINGS_S;
#endif

typedef struct hiTIMER_GLOBAL_RESOURCE_S
{
    TIMER_CHANNEL_STATE_S       *pstTIMER_ATTR_S[TIMER_MAX_TOTAL_NUM];
#if defined(HI_SND_DRV_SUSPEND_SUPPORT)
    SND_TIMER_SETTINGS_S        stSuspendAttr[TIMER_MAX_TOTAL_NUM];
#endif

}TIMER_GLOBAL_RESOURCE_S;

HI_S32 HI_DRV_Timer_Create(HI_HANDLE *phHandle,AIAO_TimerIsrFunc *pFunc, void * substream);
HI_S32 HI_DRV_Timer_Destroy(HI_HANDLE handle);
HI_S32 HI_DRV_Timer_SetAttr(HI_HANDLE handle,HI_U32 u32Config,AIAO_SAMPLE_RATE_E enRate);
HI_S32 HI_DRV_Timer_SetEnable(HI_HANDLE handle,HI_BOOL bEnalbe);
HI_S32 HI_DRV_Timer_GetEnable(HI_HANDLE handle, HI_BOOL *pbEnable);
HI_S32 HI_DRV_Timer_Suspend(HI_VOID);
HI_S32 HI_DRV_Timer_Resume(HI_VOID);

#endif
