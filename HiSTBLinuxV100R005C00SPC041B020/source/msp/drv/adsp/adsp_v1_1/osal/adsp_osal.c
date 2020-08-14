/*$$$!!Warning: Huawei key information asset. No spread without permission.$$$*/
/*CODEMARK:HuE1v2Elv7J9C1aMkGmdiUQZoS/R2sDdIoxXqdBKL9eiPHH/FdSvQYZfBQZKkVdipTcRXX+G
kqk+/W4lTjU7wqFxjZf0LDwCjpr43YYwLpCDTuHYYgBg6fyqztFfdIuFn9XsBwp7fGtfvETM
MUlDLP7OrpQgxS+D1nKur6wKW+2sdEomOKYNBX2oOB9I5mRKYAqB0VrfWd0Rlqt+mcwPHe/Z
DaKhoWMbp67J5NszLUBcxlRFbkljpWKRXa0URgrEX/JZdB98TO0XD4E93K6Sdg==#*/
/*$$$!!Warning: Deleting or modifying the preceding information is prohibited.$$$*/
#include "adsp_osal.h"

static HI_U64 OSAL_GetTimeStampUs(HI_VOID)
{
    static HI_U64 u64TimeBase = 0;
    static HI_U64 u64TimeLast = 0;
    HI_U64 u64TimeNow;
    unsigned long mod;

    u64TimeNow = sched_clock();
    if (u64TimeLast > u64TimeNow)
    {
        u64TimeBase += ((~(0LLU)) >> 10);
    }

    u64TimeLast = u64TimeNow;

    mod = do_div(u64TimeNow, 1000);

    return u64TimeBase + u64TimeNow;
}

static HI_VOID* OSAL_KMALLOC(HI_U32 u32ModuleID, HI_U32 u32Size)
{
    return HI_KMALLOC(u32ModuleID, u32Size, GFP_KERNEL);
}

static HI_VOID OSAL_KFREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    HI_KFREE(u32ModuleID, pMemAddr);
}

static HI_VOID* OSAL_VMALLOC(HI_U32 u32ModuleID, HI_U32 u32Size)
{
    return HI_VMALLOC(u32ModuleID, u32Size);
}

static HI_VOID OSAL_VFREE(HI_U32 u32ModuleID, HI_VOID* pMemAddr)
{
    HI_VFREE(u32ModuleID, pMemAddr);
}

/*
 * ioremap_nocache
 * extern void __iomem *__arm_ioremap(phys_addr_t, size_t, unsigned int);
 * #define ioremap_nocache(cookie,size)    __arm_ioremap((cookie), (size), MT_DEVICE)
 */
static HI_VOID* OSAL_ioremap(HI_U32 cookie, HI_U32 size)  // TODO: ÎïÀíµØÖ·phys_addr_t
{
#ifdef __aarch64__
    return (HI_VOID*)ioremap_wc(cookie, size);
#else
    return (HI_VOID*)ioremap_nocache(cookie, size);
#endif
}

/*
 * iounmap
 * extern void __arm_iounmap(volatile void __iomem *addr);
 * #define iounmap    __arm_iounmap
 */
static HI_VOID OSAL_iounmap(HI_VOID* addr)
{
    iounmap(addr);
}

static HI_VOID OSAL_SchedualTask(HI_U32 u32SleepNs)
{
    ktime_t stWaitTime;
    stWaitTime = ns_to_ktime(u32SleepNs);
    set_current_state(TASK_UNINTERRUPTIBLE);
    schedule_hrtimeout(&stWaitTime, HRTIMER_MODE_REL);
}

static HI_VOID OSAL_SetTaskUninterrupt(HI_VOID)
{
    //struct sched_param param = { .sched_priority = 99 };
    //sched_setscheduler(current, SCHED_FIFO, &param);
    current->flags |= PF_NOFREEZE;
    set_current_state(TASK_UNINTERRUPTIBLE);
}

static HI_VOID OSAL_SetTaskRunning(HI_VOID)
{
    set_current_state(TASK_RUNNING);
}

static HI_VOID OSAL_DestoryTask(HI_VOID* pTask)
{
    struct task_struct* pKthread = (struct task_struct*)pTask;
    if (HI_NULL != pKthread)
    {
        kthread_stop(pKthread);
    }
}

static HI_S32 OSAL_CreateTask(HI_VOID** ppTask, const HI_PCHAR pTaskName, HI_VOID* pTaskFunction)
{
    struct task_struct* pKthread;
    pKthread = kthread_create(pTaskFunction, HI_NULL, pTaskName);
    if (IS_ERR(pKthread))
    {
        return HI_FAILURE;
    }

    *ppTask = (HI_VOID*)pKthread;

    return HI_SUCCESS;
}

static HI_VOID OSAL_WakeupTask(HI_VOID* pTask)
{
    struct task_struct* pKthread;
    pKthread = (struct task_struct*)pTask;
    wake_up_process(pKthread);
}

static struct timer_list g_adsp_timer[TIMER_ID_BUTT];

/* add timer */
static HI_VOID OSAL_AddTimer(OSAL_TIMER_ID_E Id, HI_VOID* pTimerFun, HI_U32 u32Ms)
{
    init_timer(&g_adsp_timer[Id]);
    g_adsp_timer[Id].function = (HI_VOID*)pTimerFun;
    g_adsp_timer[Id].expires  = (jiffies + msecs_to_jiffies(u32Ms));
    add_timer(&g_adsp_timer[Id]);
}

/* modify timer */
static HI_VOID OSAL_ModTimer(OSAL_TIMER_ID_E Id, HI_U32 u32Ms)
{
    mod_timer(&g_adsp_timer[Id], jiffies + msecs_to_jiffies(u32Ms));
}

/* delete timer */
static HI_VOID OSAL_DelTimer(OSAL_TIMER_ID_E Id)
{
    del_timer(&g_adsp_timer[Id]);
}

/* delete timer & sync */
static HI_VOID OSAL_DelTimerSync(OSAL_TIMER_ID_E Id)
{
    del_timer_sync(&g_adsp_timer[Id]);
}

static spinlock_t g_OsalSpinLock[SPIN_LOCK_ID_BUTT];

/* init spin lock */
static HI_VOID OSAL_SpinLockInit(OSAL_SPIN_LOCK_ID_E Id)
{
    spin_lock_init(&g_OsalSpinLock[Id]);
}

/* spin lock */
static HI_VOID OSAL_SpinLock(OSAL_SPIN_LOCK_ID_E Id)
{
    spin_lock_bh(&g_OsalSpinLock[Id]);
}

/* spin unlock */
static HI_VOID OSAL_SpinUnLock(OSAL_SPIN_LOCK_ID_E Id)
{
    spin_unlock_bh(&g_OsalSpinLock[Id]);
}

/* print function */
static HI_VOID OSAL_AdspFatalPrint(const char* fmt, ...)
{
    HI_FATAL_ADSP(fmt);
}

static HI_VOID OSAL_AdspErrPrint(const char* fmt, ...)
{
    HI_ERR_ADSP(fmt);
}

static HI_VOID OSAL_AdspWarnPrint(const char* fmt, ...)
{
    HI_WARN_ADSP(fmt);
}

static HI_VOID OSAL_AdspInfoPrint(const char* fmt, ...)
{
    HI_INFO_ADSP(fmt);
}

HI_VOID ASDP_OSAL_InterfaceInit(ADSP_OSAL_FUNC_S* pstOsalFunc)
{
    pstOsalFunc->pfun_OSAL_adsp_fatal_print     = OSAL_AdspFatalPrint;
    pstOsalFunc->pfun_OSAL_adsp_err_print       = OSAL_AdspErrPrint;
    pstOsalFunc->pfun_OSAL_adsp_warn_print      = OSAL_AdspWarnPrint;
    pstOsalFunc->pfun_OSAL_adsp_info_print      = OSAL_AdspInfoPrint;

    pstOsalFunc->pfun_OSAL_adsp_msleep          = msleep;

    pstOsalFunc->pfun_OSAL_adsp_memset          = memset;
    pstOsalFunc->pfun_OSAL_adsp_memcpy          = memcpy;
    pstOsalFunc->pfun_OSAL_adsp_memmove         = memmove;
#ifndef __aarch64__
    pstOsalFunc->pfun_OSAL_adsp_kneon_begin     = kernel_neon_begin;
    pstOsalFunc->pfun_OSAL_adsp_kneon_end       = kernel_neon_end;
#endif
    pstOsalFunc->pfun_OSAL_adsp_kmalloc         = OSAL_KMALLOC;
    pstOsalFunc->pfun_OSAL_adsp_kfree           = OSAL_KFREE;
    pstOsalFunc->pfun_OSAL_adsp_vmalloc         = OSAL_VMALLOC;
    pstOsalFunc->pfun_OSAL_adsp_vfree           = OSAL_VFREE;

    pstOsalFunc->pfun_OSAL_adsp_ioremap         = OSAL_ioremap;
    pstOsalFunc->pfun_OSAL_adsp_iounmap         = OSAL_iounmap;

    pstOsalFunc->pfun_OSAL_adsp_get_time_us     = OSAL_GetTimeStampUs;

    pstOsalFunc->pfun_OSAL_adsp_spin_init       = OSAL_SpinLockInit;
    pstOsalFunc->pfun_OSAL_adsp_spin_lock       = OSAL_SpinLock;
    pstOsalFunc->pfun_OSAL_adsp_spin_unlock     = OSAL_SpinUnLock;

    pstOsalFunc->pfun_OSAL_adsp_schedule_task         = OSAL_SchedualTask;
    pstOsalFunc->pfun_OSAL_adsp_set_task_uninterrupt  = OSAL_SetTaskUninterrupt;
    pstOsalFunc->pfun_OSAL_adsp_set_task_running      = OSAL_SetTaskRunning;
    pstOsalFunc->pfun_OSAL_adsp_destory_task          = OSAL_DestoryTask;
    pstOsalFunc->pfun_OSAL_adsp_create_task           = OSAL_CreateTask;
    pstOsalFunc->pfun_OSAL_adsp_wakeup_task           = OSAL_WakeupTask;

    pstOsalFunc->pfun_OSAL_adsp_add_timer    = OSAL_AddTimer;
    pstOsalFunc->pfun_OSAL_adsp_mod_timer    = OSAL_ModTimer;
    pstOsalFunc->pfun_OSAL_adsp_del_timer    = OSAL_DelTimer;
    pstOsalFunc->pfun_OSAL_adsp_del_timer_sync = OSAL_DelTimerSync;

    OSAL_AdspInfoPrint("ASDP_OSAL_InterfaceInit\n");
}

