/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: gfx2d_list.c
* Description: Graphic 2D engine list code
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#include <linux/list.h>
#include <linux/spinlock.h>

#include "hi_gfx_sys_k.h"
#include "hi_gfx2d_type.h"
#include "hi_gfx2d_errcode.h"
#include "hi_gfx_comm_k.h"
#include "gfx2d_hal.h"
#include "gfx2d_mem.h"
#include "gfx2d_config.h"
#include "gfx2d_fence.h"
#ifdef GFX2D_SYNC_TDE
#include "drv_tde_ext.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#define GFX2D_ALIGN(value) ((value + 0xf) & 0xfffffff0)

typedef enum tagGFX2D_LIST_TASKSTATE_E
{
    GFX2D_LIST_TASKSTATE_UNFINISH = 0x0,
    GFX2D_LIST_TASKSTATE_FINISH,
} GFX2D_LIST_TASKSTATE_E;

/*task descriptor*/
typedef struct tagGFX2D_LIST_TASK_S
{
    /*List which links all submitted task by submit order.*/
    struct list_head stList;
    spinlock_t lock;                    /* Spin lock for concurrent access*/
    HI_VOID *pNode;                 /* First node of task,which include info for hardware*/
    HI_GFX2D_DEV_ID_E enDevId;  /* Dev id the task submitted to */
    GFX2D_HAL_DEV_TYPE_E enDevType; /* Dev type the task submitted to */
    GFX2D_LIST_TASKSTATE_E enState; /*Task state*/
    HI_U32 u32Ref;                  /*Reference count*/
    HI_BOOL bWait;                  /*Whether some process is waiting for the task finish*/
    wait_queue_head_t stWaitQueueHead;  /*wait queue head*/
#ifdef GFX2D_FENCE_SUPPORT
    HI_S32 s32ReleaseFenceFd;       /*release fence fd*/
#endif
} GFX2D_LIST_TASK_S;

/*task management descriptor*/
typedef struct tagGFX2D_LIST_TASK_MGR_S
{
    struct list_head stListHead;  /*List head which links all submitted task*/
    spinlock_t lock;        /*Spin lock for concurrent access */
    GFX2D_LIST_TASK_S *pstSubmitedHeadTask;  /*First task submiited to hardware*/
    GFX2D_LIST_TASK_S *pstSubmitedTailTask;  /*Last task submiited to hardware*/
    GFX2D_LIST_TASK_S *pstToSubmitTask;      /*First task waitting or submmitting to hardware*/
    GFX2D_HAL_NODE_OPS_S *pstNodeOps;      /*functions for access node*/
#ifndef GFX2D_PROC_UNSUPPORT
    /*proc info*/
    HI_U32 u32TotalTaskNum;  /*total task num submitted */
    HI_U32 u32RunTaskNum;    /*working task num*/
    HI_U32 u32WaitTaskNum;   /*waiting task num*/
    HI_U32 u32MaxTaskPerList;   /*max task num submitted to hardware*/
    HI_U32 u32LastTimeCost;     /*last task time cost*/
    HI_U32 u32MaxTimeCost;      /*max time cost*/
    HI_U32 u32TotalTimeCost;    /*total time cost*/
#endif
} GFX2D_LIST_TASK_MGR_S;

static GFX2D_LIST_TASK_MGR_S gs_astLISTTaskMgr[HI_GFX2D_DEV_ID_0 + 1][GFX2D_HAL_DEV_TYPE_HWC + 1];

HI_S32 GFX2D_LIST_Isr(HI_S32 irq, HI_VOID *dev_id);
static inline GFX2D_LIST_TASK_S *CreateTask(const HI_GFX2D_DEV_ID_E enDevId, HI_VOID *pNode,
        GFX2D_HAL_DEV_TYPE_E enNodeType, const HI_BOOL bSync,
        const HI_U32 u32Timeout, HI_S32 s32ReleaseFenceFd);
static inline HI_VOID DestroyTask(GFX2D_LIST_TASK_S *pstTask);
static inline HI_VOID InsertTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr,
                                 GFX2D_LIST_TASK_S *pstTask);
static inline HI_VOID DeleteTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr,
                                 GFX2D_LIST_TASK_S *pstTask);
static inline HI_VOID SubmitTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr);
static inline HI_VOID UnrefTask(GFX2D_LIST_TASK_S *pstTask);
static GFX2D_LIST_TASK_S *GetTailTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr);
static HI_S32 WaitForDone(GFX2D_LIST_TASK_S *pstTask, HI_U32 u32Timeout);

#ifdef GFX2D_SYNC_TDE
static HI_U32 g_HWCIrqHandle = 0;
#endif

HI_S32 GFX2D_LIST_Init(HI_VOID)
{
    HI_U32 u32IntNum;
    HI_U32 u32NodeNum;
    HI_U32 u32NodeSize;
    HI_S32 s32Ret;
    GFX2D_LIST_TASK_MGR_S *pstTaskMgr = &gs_astLISTTaskMgr[HI_GFX2D_DEV_ID_0][GFX2D_HAL_DEV_TYPE_HWC];
#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_U32 u32BaseAddr;
#endif

    HI_GFX_Memset(pstTaskMgr, 0, sizeof(GFX2D_LIST_TASK_MGR_S));
    INIT_LIST_HEAD(&(pstTaskMgr->stListHead));
    spin_lock_init(&(pstTaskMgr->lock));
    GFX2D_HAL_GetNodeOps(GFX2D_HAL_DEV_TYPE_HWC, &(pstTaskMgr->pstNodeOps));
    if (NULL == pstTaskMgr->pstNodeOps)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "failed!\n");
        return HI_ERR_GFX2D_SYS;
    }

    u32IntNum = GFX2D_HAL_GetIsrNum(HI_GFX2D_DEV_ID_0, GFX2D_HAL_DEV_TYPE_HWC);

#ifdef GFX2D_SYNC_TDE
    if (0 != request_irq(u32IntNum, (irq_handler_t)GFX2D_LIST_Isr,
                         IRQF_SHARED, "hi_gfx2d_irq", &g_HWCIrqHandle))
#else
    if (0 != request_irq(u32IntNum, (irq_handler_t)GFX2D_LIST_Isr,
                         IRQF_PROBE_SHARED, "hi_gfx2d_irq", NULL))
#endif
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "request_irq failed!\n");
        return HI_ERR_GFX2D_SYS;
    }

    s32Ret = HI_GFX_SetIrq(HIGFX_GFX2D_ID, u32IntNum);
    if (HI_SUCCESS != s32Ret)
    {
#ifdef GFX2D_SYNC_TDE
        free_irq(u32IntNum, &g_HWCIrqHandle);
#else
        free_irq(u32IntNum, NULL);
#endif
        return s32Ret;
    }

    u32NodeNum = GFX2D_CONFIG_GetNodeNum();
    u32NodeSize = ((sizeof(GFX2D_LIST_TASK_S) + 0xf) & 0xfffffff0);

    s32Ret = GFX2D_MEM_Register(u32NodeSize, u32NodeNum);
    if (HI_SUCCESS != s32Ret)
    {
#ifdef GFX2D_SYNC_TDE
        free_irq(u32IntNum, &g_HWCIrqHandle);
#else
        free_irq(u32IntNum, NULL);
#endif
        return s32Ret;
    }

#ifdef CONFIG_GFX_MMU_SUPPORT
    u32BaseAddr = GFX2D_HAL_GetBaseAddr(HI_GFX2D_DEV_ID_0, GFX2D_HAL_DEV_TYPE_HWC);
    s32Ret = HI_GFX_MapSmmuReg(u32BaseAddr + 0xf000);
    s32Ret |= HI_GFX_InitSmmu(u32BaseAddr + 0xf000);
    if (HI_SUCCESS != s32Ret)
    {
        GFX2D_Mem_UnRegister(u32NodeSize);
#ifdef GFX2D_SYNC_TDE
        free_irq(u32IntNum, &g_HWCIrqHandle);
#else
        free_irq(u32IntNum, NULL);
#endif
        return s32Ret;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 GFX2D_LIST_Deinit(HI_VOID)
{
    HI_U32 u32IntNum;
    HI_U32 u32NodeSize;

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_GFX_DeinitSmmu();
    HI_GFX_UnMapSmmuReg();
#endif

    u32NodeSize = GFX2D_ALIGN(sizeof(GFX2D_LIST_TASK_S));
    GFX2D_Mem_UnRegister(u32NodeSize);

    u32IntNum = GFX2D_HAL_GetIsrNum(HI_GFX2D_DEV_ID_0, GFX2D_HAL_DEV_TYPE_HWC);
#ifdef GFX2D_SYNC_TDE
    free_irq(u32IntNum, &g_HWCIrqHandle);
#else
    free_irq(u32IntNum, NULL);
#endif

    return HI_SUCCESS;
}

#ifdef GFX2D_SYNC_TDE
static TDE_EXPORT_FUNC_S *ps_TdeExportFuncs = HI_NULL;
#endif

HI_S32 GFX2D_LIST_Open(HI_VOID)
{
#ifdef GFX2D_SYNC_TDE
    if (ps_TdeExportFuncs != HI_NULL)
    {
        return HI_SUCCESS;
    }

    if (HI_SUCCESS != HI_DRV_MODULE_GetFunction(HI_ID_TDE, (HI_VOID **)&ps_TdeExportFuncs))
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "\n");
        return HI_FAILURE;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 GFX2D_LIST_Close(HI_VOID)
{
#ifdef GFX2D_SYNC_TDE
    ps_TdeExportFuncs = HI_NULL;
#endif

    return HI_SUCCESS;
}

HI_S32 GFX2D_LIST_SubNode(const HI_GFX2D_DEV_ID_E enDevId, HI_VOID *pNode,
                          GFX2D_HAL_DEV_TYPE_E enNodeType,const HI_BOOL bSync,
                          const HI_U32 u32Timeout, HI_S32 s32ReleaseFenceFd,
                          GFX2D_FUNC_CB pFunc, HI_VOID *pParam)
{
    GFX2D_LIST_TASK_S *pstTask;
    GFX2D_LIST_TASK_MGR_S *pstTaskMgr = &gs_astLISTTaskMgr[enDevId][enNodeType];
    HI_S32 s32Ret = HI_SUCCESS;

    pstTask = CreateTask(enDevId, pNode, enNodeType, bSync, u32Timeout, s32ReleaseFenceFd);
    if (HI_NULL == pstTask)
    {
        pstTaskMgr->pstNodeOps->pfnFreeNode(pNode);
        return HI_ERR_GFX2D_NO_MEM;
    }

    InsertTask(pstTaskMgr, pstTask);

    SubmitTask(pstTaskMgr);

    if (bSync)
    {
        s32Ret = WaitForDone(pstTask, u32Timeout);
        if (HI_SUCCESS != s32Ret)
        {
            DestroyTask(pstTask);
            return s32Ret;
        }
        //DeleteTask(pstTaskMgr, pstTask);
        DestroyTask(pstTask);
    }
    else
        UnrefTask(pstTask);

    return HI_SUCCESS;
}

HI_S32 GFX2D_LIST_WaitAllDone(const HI_GFX2D_DEV_ID_E enDevId, const HI_U32 u32Timeout)
{
    GFX2D_LIST_TASK_S *pstTask;
    HI_S32 s32Ret;

    pstTask = GetTailTask(&gs_astLISTTaskMgr[enDevId][GFX2D_HAL_DEV_TYPE_HWC]);
    if (HI_NULL == pstTask)
    {
        return HI_SUCCESS;;
    }

    s32Ret = WaitForDone(pstTask, u32Timeout);
    if (HI_SUCCESS != s32Ret)
    {
        DestroyTask(pstTask);
        return s32Ret;
    }

    //DeleteTask(&gs_astLISTTaskMgr[enDevId][GFX2D_HAL_DEV_TYPE_HWC], pstTask);

    DestroyTask(pstTask);

    return HI_SUCCESS;
}

static inline GFX2D_LIST_TASK_S *CreateTask(const HI_GFX2D_DEV_ID_E enDevId, HI_VOID *pNode,
        GFX2D_HAL_DEV_TYPE_E enNodeType, const HI_BOOL bSync,
        const HI_U32 u32Timeout, HI_S32 s32ReleaseFenceFd)
{
    GFX2D_LIST_TASK_S *pstTask;

    pstTask = (GFX2D_LIST_TASK_S *)HI_GFX_KMALLOC(HIGFX_GFX2D_ID, sizeof(GFX2D_LIST_TASK_S), GFP_KERNEL);
    if (HI_NULL == pstTask)
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "Alloc %d failed!\n", sizeof(GFX2D_LIST_TASK_S));
        return HI_NULL;
    }

    HI_GFX_Memset(pstTask, 0, sizeof(GFX2D_LIST_TASK_S));
    INIT_LIST_HEAD(&(pstTask->stList));
    spin_lock_init(&(pstTask->lock));
    init_waitqueue_head(&(pstTask->stWaitQueueHead));

    pstTask->u32Ref = 1;
    pstTask->pNode = pNode;
    pstTask->enDevId = enDevId;
    pstTask->enDevType = enNodeType;

#ifdef GFX2D_FENCE_SUPPORT
    pstTask->s32ReleaseFenceFd = s32ReleaseFenceFd;
#endif

    return pstTask;
}

static inline HI_VOID DestroyTask(GFX2D_LIST_TASK_S *pstTask)
{
    unsigned long lockflag;
    GFX2D_LIST_TASK_MGR_S *pstTaskMgr = &gs_astLISTTaskMgr[pstTask->enDevId][pstTask->enDevType];

    spin_lock_irqsave(&(pstTask->lock),lockflag);
    pstTask->u32Ref--;
    if (0 == pstTask->u32Ref && (GFX2D_LIST_TASKSTATE_FINISH == pstTask->enState))
    {
        pstTaskMgr->pstNodeOps->pfnFreeNode(pstTask->pNode);
        spin_unlock_irqrestore(&(pstTask->lock),lockflag);
        HI_GFX_KFREE(HIGFX_GFX2D_ID, pstTask);
        return;
    }

    spin_unlock_irqrestore(&(pstTask->lock),lockflag);

    return;
}

static inline HI_VOID InsertTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr,
                                 GFX2D_LIST_TASK_S *pstTask)
{
    unsigned long lockflag;

    spin_lock_irqsave(&(pstTaskMgr->lock), lockflag);

    list_add_tail(&(pstTask->stList), &(pstTaskMgr->stListHead));
    if (NULL == pstTaskMgr->pstToSubmitTask)
    {
        pstTaskMgr->pstToSubmitTask = pstTask;
    }

#ifndef GFX2D_PROC_UNSUPPORT
    pstTaskMgr->u32TotalTaskNum++;
    pstTaskMgr->u32WaitTaskNum++;
#endif

    spin_unlock_irqrestore(&(pstTaskMgr->lock), lockflag);

    return;
}

static inline HI_VOID DeleteTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr,
                                 GFX2D_LIST_TASK_S *pstTask)
{
    unsigned long lockflag;

    spin_lock_irqsave(&(pstTaskMgr->lock),lockflag);

    list_del_init(&(pstTask->stList));

    spin_unlock_irqrestore(&(pstTaskMgr->lock),lockflag);

    return;
}

static inline HI_VOID SubmitTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr)
{
    unsigned long lockflag;
    struct list_head *pstCur, *pstNext;
    GFX2D_LIST_TASK_S *pstCurTask, *pstNextTask;
    HI_S32 s32Ret;
#ifdef GFX2D_SYNC_TDE
    HI_BOOL bDeviceBusy;
    unsigned long deviceflag;
#endif

    spin_lock_irqsave(&(pstTaskMgr->lock), lockflag);

    /*If the hardware is working or no task is submited by app,return!
    Use the pstTaskMgr->pstSubmitedHeadTask to judge whether the hardware
    is working or not:when submitting task to hardware,set the flag the addr
    of submitted task.After the task is done,set the flag null to denote the
    hardware is idle!*/
    if ((NULL != pstTaskMgr->pstSubmitedHeadTask) || (NULL == pstTaskMgr->pstToSubmitTask))
    {
        spin_unlock_irqrestore(&(pstTaskMgr->lock),lockflag);
        return;
    }

#ifdef GFX2D_SYNC_TDE
    ps_TdeExportFuncs->pfnTdeLockWorkingFlag(&deviceflag);

    ps_TdeExportFuncs->pfnTdeGetWorkingFlag(&bDeviceBusy);

    if (bDeviceBusy)
    {
        ps_TdeExportFuncs->pfnTdeUnlockWorkingFlag(&deviceflag);
        spin_unlock_irqrestore(&(pstTaskMgr->lock), lockflag);
        return;
    }
#endif
    pstCur = &(pstTaskMgr->pstToSubmitTask->stList);
    pstNext = pstCur->next;

#ifndef GFX2D_PROC_UNSUPPORT
    pstTaskMgr->u32RunTaskNum = 1;
    pstTaskMgr->u32WaitTaskNum--;
#endif

    while (pstNext != (&pstTaskMgr->stListHead))
    {
        pstCurTask = (GFX2D_LIST_TASK_S *)pstCur;
        pstNextTask = (GFX2D_LIST_TASK_S *)pstNext;

        /*Hardware can process many tasks one time,requires that the tasks
        link each other by the promissory format!*/
        s32Ret = pstTaskMgr->pstNodeOps->pfnLinkNode(pstCurTask->pNode, pstNextTask->pNode);
        if (s32Ret < 0)
        {
            break;
        }
#ifndef GFX2D_PROC_UNSUPPORT
        pstTaskMgr->u32RunTaskNum++;
        pstTaskMgr->u32WaitTaskNum--;
#endif
        pstCur = pstNext;
        pstNext = pstNext->next;
    }

    /*Submit task to hardware!*/
    pstTaskMgr->pstSubmitedHeadTask = pstTaskMgr->pstToSubmitTask;
    pstTaskMgr->pstSubmitedTailTask = (GFX2D_LIST_TASK_S *)pstCur;

    if (pstNext == &(pstTaskMgr->stListHead))
    {
        pstTaskMgr->pstToSubmitTask = HI_NULL;
    }
    else
    {
        pstTaskMgr->pstToSubmitTask = (GFX2D_LIST_TASK_S *)pstNext;
    }

#ifndef GFX2D_PROC_UNSUPPORT
    if (pstTaskMgr->u32RunTaskNum > pstTaskMgr->u32MaxTaskPerList)
    {
        pstTaskMgr->u32MaxTaskPerList = pstTaskMgr->u32RunTaskNum;
    }
#endif


#ifndef GFX2D_PROC_UNSUPPORT
    GFX2D_CONFIG_StartTime();
#endif

    pstTaskMgr->pstNodeOps->pfnSubNode(pstTaskMgr->pstSubmitedHeadTask->enDevId,
                                       pstTaskMgr->pstSubmitedHeadTask->pNode);

#ifdef GFX2D_SYNC_TDE
    ps_TdeExportFuncs->pfnTdeUnlockWorkingFlag(&deviceflag);
#endif
    spin_unlock_irqrestore(&(pstTaskMgr->lock), lockflag);

    return;
}

static GFX2D_LIST_TASK_S *GetTailTask(GFX2D_LIST_TASK_MGR_S *pstTaskMgr)
{
    unsigned long listlockflag;
    unsigned long tasklockflag;
    GFX2D_LIST_TASK_S *pstTask = NULL;

    spin_lock_irqsave(&(pstTaskMgr->lock),listlockflag);
    if (list_empty(&(pstTaskMgr->stListHead)))
    {
        spin_unlock_irqrestore(&(pstTaskMgr->lock),listlockflag);
        return HI_NULL;
    }
    pstTask = (GFX2D_LIST_TASK_S *)pstTaskMgr->stListHead.prev;
    spin_unlock_irqrestore(&(pstTaskMgr->lock),listlockflag);

    /*Add task ref to avoid the task is destroyed by other process!*/
    spin_lock_irqsave(&(pstTask->lock),tasklockflag);
    pstTask->u32Ref++;
    spin_unlock_irqrestore(&(pstTask->lock),tasklockflag);

    return pstTask;
}

static HI_VOID ListCompIsr(HI_VOID)
{
    GFX2D_LIST_TASK_S *pstTask;
    GFX2D_LIST_TASK_S *pstNextTask;
    GFX2D_LIST_TASK_MGR_S *pstTaskMgr = &gs_astLISTTaskMgr[HI_GFX2D_DEV_ID_0][GFX2D_HAL_DEV_TYPE_HWC];
    unsigned long lockflag;

#ifndef GFX2D_PROC_UNSUPPORT
    pstTaskMgr->u32LastTimeCost = GFX2D_CONFIG_EndTime();
    pstTaskMgr->u32TotalTimeCost += pstTaskMgr->u32LastTimeCost;
    if (pstTaskMgr->u32LastTimeCost > pstTaskMgr->u32MaxTimeCost)
    {
        pstTaskMgr->u32MaxTimeCost = pstTaskMgr->u32LastTimeCost;
    }
#endif
    pstNextTask = pstTaskMgr->pstSubmitedHeadTask;
    do
    {
        pstTask = pstNextTask;

        /*Get the next task before wake_up,because process waked up may
        delete the task from the list!*/
        spin_lock_irqsave(&(pstTaskMgr->lock), lockflag);
        pstNextTask = (GFX2D_LIST_TASK_S *)(pstTask->stList.next);
        spin_unlock_irqrestore(&(pstTaskMgr->lock), lockflag);

        pstTaskMgr->pstNodeOps->pfnNodeIsr(pstTask->pNode);
        spin_lock_irqsave(&(pstTask->lock), lockflag);
        pstTask->enState = GFX2D_LIST_TASKSTATE_FINISH;
        pstTask->u32Ref++;
        spin_unlock_irqrestore(&(pstTask->lock), lockflag);

#ifdef GFX2D_FENCE_SUPPORT
        if (pstTask->s32ReleaseFenceFd >= 0)
        {
            GFX2D_FENCE_WakeUp();
        }
#endif
        DeleteTask(pstTaskMgr, pstTask);

        if (pstTask->bWait)
        {
            UnrefTask(pstTask);
            wake_up(&pstTask->stWaitQueueHead);
        }
        else
        {
            DestroyTask(pstTask);
        }
#ifndef GFX2D_PROC_UNSUPPORT
        spin_lock_irqsave(&(pstTaskMgr->lock), lockflag);
        pstTaskMgr->u32RunTaskNum--;
        spin_unlock_irqrestore(&(pstTaskMgr->lock), lockflag);
#endif
    }while(pstTask != pstTaskMgr->pstSubmitedTailTask);

    spin_lock_irqsave(&(pstTaskMgr->lock), lockflag);
    pstTaskMgr->pstSubmitedHeadTask = HI_NULL;
    pstTaskMgr->pstSubmitedTailTask = HI_NULL;
    pstTaskMgr->pstNodeOps->pfnAllNodeIsr();
    spin_unlock_irqrestore(&(pstTaskMgr->lock),lockflag);

    return;
}

static void Gfx2dTaskletFunc(unsigned long int_status);
DECLARE_TASKLET(g_Gfx2dTasklet, Gfx2dTaskletFunc, 0);

static void Gfx2dTaskletFunc(unsigned long int_status)
{
    g_Gfx2dTasklet.data &= (~int_status);

    if (int_status & 0x1)
    {
        ListCompIsr();
    }

    SubmitTask(&gs_astLISTTaskMgr[HI_GFX2D_DEV_ID_0][GFX2D_HAL_DEV_TYPE_HWC]);
}

HI_S32 GFX2D_LIST_Isr(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32Status = 0;

#ifdef GFX2D_SYNC_TDE
    if (ps_TdeExportFuncs == NULL)
    {
        return IRQ_HANDLED;
    }
#endif

#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_GFX_SmmuIsr("HI_MOD_GFX2D");
#endif

    u32Status = GFX2D_HAL_GetIntStatus(GFX2D_HAL_DEV_TYPE_HWC);
    //printk("intstatus:%x\n", u32Status);

    g_Gfx2dTasklet.data = g_Gfx2dTasklet.data |((unsigned long)u32Status);

    tasklet_schedule(&g_Gfx2dTasklet);

    return IRQ_HANDLED;
}


static inline HI_VOID UnrefTask(GFX2D_LIST_TASK_S *pstTask)
{
    unsigned long lockflag;

    spin_lock_irqsave(&(pstTask->lock),lockflag);
    pstTask->u32Ref--;
    spin_unlock_irqrestore(&(pstTask->lock),lockflag);

    return;
}

static HI_S32 WaitForDone(GFX2D_LIST_TASK_S *pstTask, HI_U32 u32Timeout)
{
    unsigned long lockflag;
    HI_S32 s32Ret;
    HI_U32 u32Jiffies;
    GFX2D_LIST_TASKSTATE_E enState;

    if (u32Timeout == 0)
    {
        u32Jiffies = 5 * HZ;
    }
    else
    {
        u32Jiffies = u32Timeout * HZ / 1000;
    }

    spin_lock_irqsave(&(pstTask->lock),lockflag);
    pstTask->bWait = HI_TRUE;
    spin_unlock_irqrestore(&(pstTask->lock), lockflag);

    s32Ret = wait_event_timeout(pstTask->stWaitQueueHead,
             (GFX2D_LIST_TASKSTATE_FINISH == pstTask->enState), u32Jiffies);
    if (s32Ret <= 0)
    {
        s32Ret = HI_ERR_GFX2D_TIMEOUT;
    }

    spin_lock_irqsave(&(pstTask->lock), lockflag);
    pstTask->bWait = HI_FALSE;
    enState = pstTask->enState;
    spin_unlock_irqrestore(&(pstTask->lock), lockflag);

    if (GFX2D_LIST_TASKSTATE_FINISH == enState)
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        HI_GFX_COMM_LOG_ERROR(HIGFX_GFX2D_ID, "timeout!\n");
    }

    return s32Ret;
}

HI_S32 GFX2D_LIST_Resume(HI_VOID)
{
#ifdef CONFIG_GFX_MMU_SUPPORT
    HI_U32 u32BaseAddr;

    u32BaseAddr = GFX2D_HAL_GetBaseAddr(HI_GFX2D_DEV_ID_0, GFX2D_HAL_DEV_TYPE_HWC);

    (HI_VOID)HI_GFX_MapSmmuReg(u32BaseAddr + 0xf000);

    return HI_GFX_InitSmmu(u32BaseAddr + 0xf000);
#else
    return HI_SUCCESS;
#endif
}

#ifndef GFX2D_PROC_UNSUPPORT
HI_VOID GFX2D_LIST_ReadProc(struct seq_file *p, HI_VOID *v)
{
    GFX2D_LIST_TASK_MGR_S *pstTaskMgr = &gs_astLISTTaskMgr[HI_GFX2D_DEV_ID_0][GFX2D_HAL_DEV_TYPE_HWC];
    SEQ_Printf(p, "---------------------GFX2D Task Info--------------------\n");
    SEQ_Printf(p, "TotalTaskNum\t:%u\n", pstTaskMgr->u32TotalTaskNum);
    SEQ_Printf(p, "RunTaskNum\t:%u\n", pstTaskMgr->u32RunTaskNum);
    SEQ_Printf(p, "WaitTaskNum\t:%u\n", pstTaskMgr->u32WaitTaskNum);
    SEQ_Printf(p, "MaxTaskPerList\t:%d\n", pstTaskMgr->u32MaxTaskPerList);
    SEQ_Printf(p, "TotalTimeCost\t:%uus\n", pstTaskMgr->u32TotalTimeCost);
    SEQ_Printf(p, "MaxTimeCost\t:%uus\n", pstTaskMgr->u32MaxTimeCost);
    SEQ_Printf(p, "LastTimeCost\t:%uus\n", pstTaskMgr->u32LastTimeCost);
    return;
}

HI_S32 GFX2D_LIST_WriteProc(struct file *file, const char __user *buf, \
                            size_t count, loff_t *ppos)
{
    return HI_SUCCESS;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/
