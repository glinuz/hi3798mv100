/* Sys headers */
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/* Unf headers */
#include "hi_error_mpi.h"

/* Drv headers */
#include "hi_drv_ao.h"
#include "hi_drv_ai.h"
#include "drv_ao_ioctl.h"
#include "drv_ao_ext.h"
#include "drv_adsp_ext.h"
#include "drv_ao_private.h"

#include "hi_audsp_aoe.h"
#include "hal_aoe.h"
#include "hal_cast.h"
#include "hal_aiao.h"
#include "hi_drv_file.h"
#include "drv_timer_private.h"

#include "audio_util.h"

static TIMER_GLOBAL_RESOURCE_S  g_pstGlobalTIMERRS;

static HI_VOID Timer_GetDefParam(AIAO_TIMER_Attr_S *pstTimerDefaultAttr)
{
    pstTimerDefaultAttr->stIfAttr.enRate = HI_UNF_SAMPLE_RATE_48K;
    pstTimerDefaultAttr->stIfAttr.u32BCLK_DIV = HI_UNF_I2S_BCLK_4_DIV;
    pstTimerDefaultAttr->stIfAttr.u32FCLK_DIV = HI_UNF_I2S_MCLK_256_FS;
    pstTimerDefaultAttr->u32Config = TIMER_INTERRUPT_CONFIG_DF;
}

static HI_S32 Timer_AllocHandle(HI_HANDLE *phHandle)
{
    HI_U32 i;
    TIMER_CHANNEL_STATE_S *state = HI_NULL;

    if (HI_NULL == phHandle)
    {
        HI_ERR_AO("Bad param Null Pointer!\n");
        return HI_FAILURE;
    }

    /* Allocate new Timer channel */
    for (i = 0; i < TIMER_MAX_TOTAL_NUM; i++)
    {
        if (NULL == g_pstGlobalTIMERRS.pstTIMER_ATTR_S[i])
        {
            break;
        }
    }

    if (i >= AIAO_TIMER_BUTT)
    {
        HI_ERR_AO("Too many Timer channel!\n");
        return HI_FAILURE;
    }

    state = HI_KMALLOC(HI_ID_AO, sizeof(TIMER_CHANNEL_STATE_S), GFP_KERNEL);
    if (state == HI_NULL)
    {
        HI_FATAL_AO("HI_KMALLOC TIMER_Create failed\n");
        return HI_FAILURE;
    }
    memset(state, 0, sizeof(TIMER_CHANNEL_STATE_S));

    g_pstGlobalTIMERRS.pstTIMER_ATTR_S[i] = state;
    *phHandle = i;

    return HI_SUCCESS;
}

static HI_VOID Timer_FreeHandle(HI_HANDLE hHandle)
{
    TIMER_CHANNEL_STATE_S *state = HI_NULL;

    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[hHandle];
    if(HI_NULL == state)
    {
        HI_ERR_AO("Timer Handle is already free\n");
        return;
    }

    HI_KFREE(HI_ID_AO, (HI_VOID*)state);
    g_pstGlobalTIMERRS.pstTIMER_ATTR_S[hHandle] = HI_NULL;
}

static HI_S32 Timer_Create(AIAO_Timer_Create_S *pstParam)
{
    HI_S32 Ret;
    TIMER_CHANNEL_STATE_S *state = HI_NULL;
    AIAO_TIMER_ID_E enTimerID;

    enTimerID = pstParam->enTimerID;
    Ret = HAL_AIAO_T_Create(enTimerID,pstParam);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_AO("HAL_AIAO_T_Create failed 0x%x\n", Ret);
        return HI_FAILURE;
    }
    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[enTimerID];
    state->pFunc = pstParam->pTimerIsrFunc;
    state->pSubstream =  pstParam->substream;
    state->bEnable = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 Timer_Destroy(HI_HANDLE handle)
{
    AIAO_TIMER_ID_E enTimerID;

    enTimerID = (AIAO_TIMER_ID_E)handle;

    HAL_AIAO_T_Destroy(enTimerID);

    return HI_SUCCESS;
}

static HI_S32 Timer_SetAttr(HI_HANDLE handle,HI_U32 u32Config,AIAO_SAMPLE_RATE_E enRate)
{
    AIAO_TIMER_ID_E enTimerID;
    TIMER_CHANNEL_STATE_S *state = HI_NULL;
    AIAO_TIMER_Attr_S stParam;
    HI_S32 Ret;

    enTimerID = (AIAO_TIMER_ID_E)handle;
    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[enTimerID];

    stParam.enTimerID = enTimerID;
    stParam.u32Config = u32Config;
    stParam.stIfAttr.enRate = enRate;
    stParam.stIfAttr.u32BCLK_DIV = HI_UNF_I2S_BCLK_4_DIV;

#if 0
    stParam.stIfAttr.u32FCLK_DIV = HI_UNF_I2S_MCLK_256_FS;
#else
    stParam.stIfAttr.u32FCLK_DIV = AUTIL_MclkFclkDiv(HI_UNF_I2S_MCLK_256_FS);
#endif
    Ret = HAL_AIAO_T_SetTimerAttr(enTimerID,&stParam);
    if(Ret != HI_SUCCESS)
    {
        HI_ERR_AO("Timer ID %d Set Attr Failed\n",(HI_U32)enTimerID);
    }

    state->u32Config = u32Config;
    state->stIfAttr.enRate = stParam.stIfAttr.enRate;
    state->stIfAttr.u32BCLK_DIV = stParam.stIfAttr.u32BCLK_DIV;
    state->stIfAttr.u32FCLK_DIV = stParam.stIfAttr.u32FCLK_DIV;

    HI_INFO_AO("Timer ID %d Set Attr Success\n",(HI_U32)enTimerID);
    return Ret;
}

static HI_S32 Timer_SetEnable(HI_HANDLE handle,HI_BOOL bEnalbe)
{
    TIMER_CHANNEL_STATE_S *state = HI_NULL;
    AIAO_TIMER_ID_E enTimerID;
    HI_S32 Ret;

    enTimerID = (AIAO_TIMER_ID_E)handle;
    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[enTimerID];

    Ret = HAL_AIAO_T_SetTimerEnable(enTimerID,bEnalbe);
    if(Ret != HI_SUCCESS)
    {
       HI_ERR_AO("Timer ID %d Set Enable %d Failed\n",(HI_U32)enTimerID,bEnalbe);
    }

    state->bEnable = bEnalbe;
    HI_INFO_AO("Timer ID %d Set Enable %d Success\n",(HI_U32)enTimerID,bEnalbe);

    return Ret;
}


static HI_S32 Timer_GetEnable(HI_HANDLE handle, HI_BOOL *pbEnable)
{
    TIMER_CHANNEL_STATE_S *state = HI_NULL;
    AIAO_TIMER_ID_E enTimerID;

    enTimerID = (AIAO_TIMER_ID_E)handle;
    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[enTimerID];

    if(state)
    {
        *pbEnable = state->bEnable;
        return HI_SUCCESS;
    }
    return HI_FAILURE;
}


static HI_S32 Timer_Drv_Create(HI_HANDLE *phHandle,AIAO_TimerIsrFunc *pFunc,void * substream)
{
    HI_S32 Ret;
    AIAO_Timer_Create_S stParam;
    AIAO_TIMER_ID_E     enAllocID;

    CHECK_AO_NULL_PTR(phHandle);

    Ret = Timer_AllocHandle(&enAllocID);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_AO("Alloc Timer Handle failed!");
        return Ret;
    }
    stParam.enTimerID = enAllocID;
    stParam.pTimerIsrFunc = pFunc;
    stParam.substream = substream;
    Ret = Timer_Create(&stParam);
    if(HI_SUCCESS != Ret)
    {
        Timer_FreeHandle(enAllocID);
        HI_ERR_AO("Timer Create failed!");
        return Ret;
    }
    *phHandle = enAllocID;

    HI_INFO_AO("Timer ID %d Create Success\n",(HI_U32)enAllocID);
    return Ret;
}

static HI_S32 Timer_Drv_Destroy(HI_HANDLE handle)
{
    HI_S32 Ret;

    Ret = Timer_Destroy(handle);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_AO("Timer_Destroy failed %d!", Ret);
        return Ret;
    }

    Timer_FreeHandle(handle);
    HI_INFO_AO("Timer ID %d Destroy Success\n",(HI_U32)handle);

    return Ret;
}

static HI_S32 Timer_Drv_GetSettings(HI_HANDLE handle, SND_TIMER_SETTINGS_S* pstTimerSettings)
{
    TIMER_CHANNEL_STATE_S *state = HI_NULL;

    state = g_pstGlobalTIMERRS.pstTIMER_ATTR_S[(HI_U32)handle];

    pstTimerSettings->bEnable = state->bEnable;
    pstTimerSettings->u32Config = state->u32Config;
    pstTimerSettings->enRate = state->stIfAttr.enRate;
    pstTimerSettings->u32BCLK_DIV = state->stIfAttr.u32BCLK_DIV;
    pstTimerSettings->u32FCLK_DIV = state->stIfAttr.u32FCLK_DIV;
    pstTimerSettings->pFunc = state->pFunc;
    pstTimerSettings->pSubstream = state->pSubstream;

    return HI_SUCCESS;
}

static HI_S32 Timer_DRV_RestoreSettings(HI_HANDLE handle, SND_TIMER_SETTINGS_S* pstTimerSettings)
{
    AIAO_Timer_Create_S stParam;
    HI_S32 Ret = HI_SUCCESS;

    stParam.enTimerID = (AIAO_TIMER_ID_E)handle;
    stParam.pTimerIsrFunc = pstTimerSettings->pFunc;
    stParam.substream = pstTimerSettings->pSubstream;
    Ret = Timer_Create(&stParam);
    if(HI_SUCCESS != Ret)
    {
        HI_ERR_AO("Timer Resume Create failed!");
        return Ret;
    }
    //set attr
    Timer_SetAttr(handle, pstTimerSettings->u32Config, pstTimerSettings->enRate);
    HI_INFO_AO("Timer ID %d Set Attr Success\n",(AIAO_TIMER_ID_E)handle);
    //enable
    Timer_SetEnable(handle, pstTimerSettings->bEnable);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_Timer_GetDefParam(AIAO_TIMER_ID_E enTimerID,AIAO_TIMER_Attr_S *pstParam)
{
    pstParam->enTimerID = enTimerID;
    Timer_GetDefParam(pstParam);

    return HI_SUCCESS;
}

HI_S32 HI_DRV_Timer_Create(HI_HANDLE *phHandle,AIAO_TimerIsrFunc *pFunc, void * substream)
{
    return Timer_Drv_Create(phHandle,pFunc,substream);
}

HI_S32 HI_DRV_Timer_Destroy(HI_HANDLE handle)
{
    return Timer_Drv_Destroy(handle);
}

HI_S32 HI_DRV_Timer_SetAttr(HI_HANDLE handle,HI_U32 u32Config,AIAO_SAMPLE_RATE_E enRate)
{
    return Timer_SetAttr(handle,u32Config,enRate);
}

HI_S32 HI_DRV_Timer_SetEnable(HI_HANDLE handle,HI_BOOL bEnalbe)
{
    return Timer_SetEnable(handle, bEnalbe);
}

HI_S32 HI_DRV_Timer_GetEnable(HI_HANDLE handle, HI_BOOL *pbEnable)
{
    return Timer_GetEnable(handle, pbEnable);
}

HI_S32 HI_DRV_Timer_Suspend(HI_VOID)
{
    HI_S32 Ret = HI_SUCCESS;
#if defined(HI_SND_DRV_SUSPEND_SUPPORT)
    HI_U32 i;

    for(i = 0; i < TIMER_MAX_TOTAL_NUM; i++)
    {
        if(g_pstGlobalTIMERRS.pstTIMER_ATTR_S[i])
        {
             Timer_Drv_GetSettings(i, &g_pstGlobalTIMERRS.stSuspendAttr[i]);
             /* Destory timer */
             Ret = Timer_Destroy(i);
             if(HI_SUCCESS != Ret)
             {
                 HI_ERR_AO("Timer_Destroy failed!");
                 return Ret;
             }
        }
    }
    HI_PRINT("AO Timer suspend OK.\n");
#endif
    return Ret;
}

HI_S32 HI_DRV_Timer_Resume(HI_VOID)
{
    HI_S32 Ret = HI_SUCCESS;
#if defined(HI_SND_DRV_SUSPEND_SUPPORT)
    HI_U32 i;

    for(i = 0;i < TIMER_MAX_TOTAL_NUM;i++)
    {
        if(g_pstGlobalTIMERRS.pstTIMER_ATTR_S[i])
        {
            /* Recreater timer */
            Ret = Timer_DRV_RestoreSettings(i, &g_pstGlobalTIMERRS.stSuspendAttr[i]);
            if(HI_SUCCESS != Ret)
            {
                HI_ERR_AO("Timer_RestoreSettings %d failed 0x%x!", i, Ret);
                return Ret;
            }
        }
    }
    HI_PRINT("AO Timer resume OK.\n");
#endif
    return Ret;
}

HI_S32 Timer_DRV_Init(HI_VOID)
{
    HI_INFO_AO("\n");
    memset(&g_pstGlobalTIMERRS, 0, sizeof(TIMER_GLOBAL_RESOURCE_S));
    return HI_SUCCESS;
}
HI_S32 Timer_DRV_DeInit(HI_VOID)
{
    HI_INFO_AO("\n");
    return HI_SUCCESS;
}
