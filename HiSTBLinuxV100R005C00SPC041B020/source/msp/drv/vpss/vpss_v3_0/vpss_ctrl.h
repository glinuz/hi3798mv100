#ifndef __VPSS_CTRL_H__
#define __VPSS_CTRL_H__

#include <linux/list.h>
#include "vpss_instance.h"
#include "vpss_fb.h"
#include "vpss_osal.h"
#include "hi_drv_dev.h"
#include <linux/completion.h>
#include <linux/semaphore.h>
#include <linux/rwlock.h>

#define VPSS_INSTANCE_MAX_NUMB 12

#define VPSS0_IRQ_NUM 125  //93+32
#define VPSS1_IRQ_NUM 110  //78+32


#define VPSS_GET_IP(hVPSS) (hVPSS / VPSS_INSTANCE_MAX_NUMB)
#define VPSS_GET_INST_POS(hVPSS) (hVPSS % VPSS_INSTANCE_MAX_NUMB)

typedef struct completion VPSS_IRQ_LOCK;
typedef struct semaphore  VPSS_INSTLIST_LOCK;
typedef  struct task_struct*    VPSS_THREAD;

typedef struct hiVPSS_EXP_CTRL_S
{
    rwlock_t stListLock;

    HI_U32 u32Target;

    HI_U32 u32InstanceNum;

    VPSS_INSTANCE_S* pstInstPool[VPSS_INSTANCE_MAX_NUMB];

} VPSS_INST_CTRL_S;

typedef enum hiVPSS_TASK_STATE_E
{
    TASK_STATE_READY = 0,
    TASK_STATE_WAIT,
    TASK_STATE_IDLE,
    TASK_STATE_BUTT
} VPSS_TASK_STATE_E;

typedef struct hiVPSS_TASK_S
{
    VPSS_TASK_STATE_E stState;
    
    VPSS_INSTANCE_S* pstInstance;
	
	VPSS_HAL_INFO_S stVpssHalInfo;

	HI_BOOL bOutLowDelay;
    VPSS_FB_NODE_S *pstFrmNode[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER*2];   
#ifdef ZME_2L_TEST
	VPSS_FB_NODE_S *pstFrmNodeRoBuf[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];  
	VPSS_FB_NODE_S *pstFrmNodeZME1L;
	HI_BOOL 	   bZME2lTaskNeed[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];	
#endif
    HI_U32 u32SucRate;
    HI_U32 u32LastTotal;
    HI_U32 u32SuccessTotal;
    HI_U32 u32Create;
    HI_U32 u32Fail;
    HI_U32 u32TimeOut;    

} VPSS_TASK_S;

typedef irqreturn_t (*FN_VPSS_InterruptRoute)(HI_S32 irq, HI_VOID *dev_id);

typedef struct hiVPSS_CTRL_S
{
    HI_BOOL         bIPVaild;
    VPSS_IP_E       enIP;
    HI_U32          u32VpssIrqNum;
    HI_CHAR         isr_name[30];
    FN_VPSS_InterruptRoute pVpssIntService;
    
    HI_U32          s32IsVPSSOpen;

    OSAL_EVENT    stTaskLowDelay;
    OSAL_EVENT    stTaskNext;
    OSAL_EVENT      stNewTask;
    VPSS_TASK_S     stTask;
    HI_BOOL         bSuspend;

    HI_BOOL         bInMCE;

    HI_HANDLE       hVpssIRQ;

    HI_U32          u32ThreadKilled;
    volatile HI_U32          u32ThreadSleep;
    VPSS_THREAD     hThread;
    volatile HI_U32          s32ThreadPos;
#ifndef ZME_2L_TEST
    VPSS_BUFFER_S stRoBuf[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
#endif
    VPSS_INST_CTRL_S  stInstCtrlInfo;

} VPSS_CTRL_S;

irqreturn_t VPSS0_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id);
irqreturn_t VPSS1_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id);
HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v);
HI_S32 VPSS_CTRL_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos);


HI_S32 VPSS_CTRL_Init(HI_VOID);
HI_S32 VPSS_CTRL_DelInit(HI_VOID);
HI_S32 VPSS_CTRL_SetMceFlag(HI_BOOL bInMCE);
VPSS_HANDLE VPSS_CTRL_CreateInstance(HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_CTRL_DestoryInstance(VPSS_HANDLE hVPSS);
VPSS_INSTANCE_S* VPSS_CTRL_GetInstance(VPSS_HANDLE hVPSS);
HI_S32 VPSS_CTRL_SyncDistributeIP(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_IPMODE_E enIpmode);
HI_S32 VPSS_CTRL_WakeUpThread(HI_VOID);
HI_S32 VPSS_CTRL_Pause(VPSS_HANDLE hVPSS);
HI_S32 VPSS_CTRL_Resume(VPSS_HANDLE hVPSS);

HI_S32 VPSS_CTRL_GetInstanceNumber(VPSS_IP_E enIp,HI_U32 *pu32Number);

VPSS_INSTANCE_S *VPSS_CTRL_GetOnlyInstance(VPSS_IP_E enIp);
#endif
