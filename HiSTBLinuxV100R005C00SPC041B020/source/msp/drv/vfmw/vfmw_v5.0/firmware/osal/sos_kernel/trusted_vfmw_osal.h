/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhEDaRH/mYdIijFrTqr/UaRBfB2ylm29ZtemC0p1r
w4HAPZDg7CZHVO7OeWdV9RjDXmaqi9pg/K0L2WeHMQwYuJ60tQQFERUjwnO7P0sDjHaExkBj
Sw7pEC7CPZoIzOnyYWhGLqs0TGvwHp3G3mXNO3TIPKBbFe8dlgj+4cd8xhLosA==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

#ifndef __VFMW_SOS_HEADER__
#define  __VFMW_SOS_HEADER__

#include "sre_hwi.h"
#include "debug.h"
#include "sre_mem.h"
#include "sre_task.h"
#include "sre_ticktimer.h"
#include "sre_base.h"
#include "boot.h"
#include "sre_sem.h"
#include "sre_debug.h"
#include "sre_sys.h"
#include "sre_securemmu.h"
#include "secure_gic_common.h"
#include "drv_tee_mem.h"
#include "init_ext.h"
#include "vfmw.h"
#include "vfmw_osal_ext.h"

/*======================================================================*/
/*                            constant define                           */
/*======================================================================*/

#define SOS_OK     0
#define SOS_ERR   -1

/*======================================================================*/
/*                            struct define                             */
/*======================================================================*/

typedef SINT32  T_WAITQHEAD;
typedef SINT32  T_SPINLOCK;
typedef VOID*   P_TASK;

typedef int     FILE;
typedef int     SOS_SEMA;

typedef struct hi_VFMW_MMZ_BUFFER_S
{
    UINT32 u32StartVirAddr;
    UINT32 u32StartPhyAddr;
    UINT32 u32Size;
}MMZ_VFMW_BUFFER_S;

typedef struct hiKERN_EVENT_S
{
	T_WAITQHEAD         queue_head;
	SINT32              flag;
} KERN_EVENT_S;

typedef struct hiKERN_IRQ_LOCK_S
{
    T_SPINLOCK     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} KERN_IRQ_LOCK_S;

typedef  P_TASK                 SOS_TASK;

typedef  KERN_EVENT_S           SOS_EVENT;

typedef  KERN_IRQ_LOCK_S        SOS_IRQ_SPIN_LOCK;


/*======================================================================*/
/*                         secure os function declare                            */
/*======================================================================*/
extern int snprintf(char *str, int sz, const char *fmt, ...);
extern VOID* memmove(VOID *dest, const VOID *src, unsigned long count);
extern void uart_printf_func(const char *fmt, ...);
//extern VOID SRE_DelayMs(UINT32 uwDelay);

#define SOS_Snprintf(buffer, size, format, arg...)  snprintf(buffer, size, format, ## arg)
#define SOS_printk uart_printf_func

SINT32 SOS_Request_Irq(UINT32 IrqNum, HWI_PROC_FUNC pfnHandler, SINT32 flags, SINT8 *name, VOID *dev);
SINT32 SOS_Delete_Irq(UINT32 IrqNum, VOID *dev);
/********Map/Unmap to Sec 仅用作非安全和安全接口交互使用**********/
SINT32 SOS_Mmap(UINT32 phy, SINT32 size, UINT32 SecMode, UINT32 IsCached);
SINT32 SOS_Unmap(UINT32 vir, SINT32 size);
/*****************************************************************/
VOID   SOS_Mb(VOID);
UINT32 SOS_GetTimeInMs(VOID);
UINT32 SOS_GetTimeInUs(VOID);
SINT32 SOS_SpinLockIRQ(SOS_IRQ_SPIN_LOCK *pIntrMutex);
SINT32 SOS_SpinUnLockIRQ(SOS_IRQ_SPIN_LOCK *pIntrMutex);

/*======================================================================*/
/*                          function declare                            */
/*======================================================================*/
SINT32 SOS_CreateTask( SOS_TASK *pTask, SINT8 TaskName[], VOID *pTaskFunction );
SINT32 SOS_Mem_Free(MEM_DESC_S *pMemDesc);
UINT8 *SOS_Register_Map(UINT32 PhyAddr);
VOID   SOS_Register_UnMap(UINT8 *VirAddr);
SINT32 SOS_Mem_Malloc(SINT8 * MemName, SINT32 len, SINT32 align, SINT32 IsCached, MEM_DESC_S *pMemDesc);
UINT8 *SOS_Mem_Map(MEM_RECORD_S *pMemRec);
UINT8* SOS_Mem_MapCache(MEM_RECORD_S *pMemRec);
VOID   SOS_Mem_Unmap(UINT8 *p);
SINT32 SOS_DOWN_INTERRUPTIBLE(SemType semType,int chanID);
VOID   SOS_UP(SemType semType,int chanID); 
VOID   SOS_SEMA_INTIT(SemType semType,int chanID);
/********Map/Unmap 非安全侧申请的内存**********/
UINT8 *SOS_NsMem_Map(UADDR phyaddr); 
VOID   SOS_NsMem_Unmap(UINT8 *p);
/**********************************************/
SINT32 InitTrustedVfmwInterface(VOID);
#endif


