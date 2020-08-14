/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv/JzwTgKEHe/9N4yU5rqhEDaRH/mYdIijFrTqr/UaRBfB2ylm29ZtemC0p1r
w4HAPZDg7CZHVO7OeWdV9RjDXmaqi9pg/K0L2WeHMQwYuJ60tQQFERUjwnO7P0sDjHaExkBj
Sw7pEC7CPZoIzOnyYWhGLqs0TGvwHp3G3mXNO3TIPKBbFe8dlgj+4cd8xhLosA==*/
/*--------------------------------------------------------------------------------------------------------------------------*/

#ifndef __VFMW_OSAL_HEADER__
#define  __VFMW_OSAL_HEADER__

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
#include "sec_mmz.h"
#include "vfmw.h"


/*======================================================================*/
/*                            constant define                           */
/*======================================================================*/

#define OSAL_OK     0
#define OSAL_ERR   -1

/*======================================================================*/
/*                            struct define                             */
/*======================================================================*/

typedef SINT32  T_WAITQHEAD;
typedef SINT32  T_SPINLOCK;
typedef VOID*   P_TASK;

typedef int     FILE;
typedef int     OSAL_SEMA;

typedef struct hi_VFMW_MMZ_BUFFER_S
{
    HI_U32 u32StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
}MMZ_VFMW_BUFFER_S;

typedef struct hiOSAL_MEM_S
{
    SINT32     PhyAddr;
	VOID       *VirAddr;
	SINT32     Length;
} OSAL_MEM_S;

typedef enum hiFILE_OP_E
{
    OSAL_FREAD,
	OSAL_FWRITE,
	OSAL_FREAD_FWRITE
} OSAL_FILE_OP_E;

typedef enum hiFILE_TYPE_E
{
    OSAL_FILE_ASCII,
	OSAL_FILE_BIN,
} OSAL_FILE_TYPE_E;

typedef enum hiFILE_SEEK_START_E
{
    OSAL_SEEK_SET,
	OSAL_SEEK_CUR,
	OSAL_SEEK_END
} FILE_SEEK_START_E;

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

typedef  P_TASK                 OSAL_TASK;

typedef  KERN_EVENT_S           OSAL_EVENT;

typedef  KERN_EVENT_S           OSAL_TASK_MUTEX;

typedef  UINT32                 OSAL_IRQ_LOCK;

typedef  KERN_IRQ_LOCK_S        OSAL_IRQ_SPIN_LOCK;


/*======================================================================*/
/*                          function declare                            */
/*======================================================================*/

extern int snprintf(char *str, int sz, const char *fmt, ...);

#define OSAL_IO_ADDRESS(Phy)                         (Phy)
#define OSAL_Sscanf(buffer, format, arg...)          0
#define OSAL_Snprintf(buffer, size, format, arg...)  snprintf(buffer, size, format, ## arg)

//#define sw_printk PRINT_INFO
#define sw_printk uart_printf_func

/************************************************************************/
/* ms sleep                                                             */
/************************************************************************/
#define OSAL_MSLEEP(nMs)  \
do{                       \
     SRE_DelayMs(nMs);    \
}while(0)


/************************************************************************/
/* secure os interface                                                  */
/************************************************************************/

SINT32 OSAL_Request_Irq(UINT32 IrqNum, HWI_PROC_FUNC pfnHandler, UINT32 Arg);
SINT32 OSAL_Delete_Irq(UINT32 IrqNum);

SINT32 SOS_Mmap(UINT32 phy, SINT32 size, UINT32 SecMode, UINT32 IsCached);
SINT32 SOS_Unmap(UINT32 vir, SINT32 size);

VOID   OSAL_Mb(VOID);

/************************************************************************/
/* get time */
/************************************************************************/
UINT32 OSAL_GetTimeInMs(VOID);
UINT32 OSAL_GetTimeInUs(VOID);

/************************************************************************/
/* alloc vir memory */
/************************************************************************/
VOID *OSAL_AllocVirMem(SINT32 Size);

/************************************************************************/
/* free vir memory */
/************************************************************************/
VOID OSAL_FreeVirMem(VOID *p);

/************************************************************************/
/* create task */
/************************************************************************/
SINT32 OSAL_CreateTask( OSAL_TASK *pTask, SINT8 TaskName[], VOID *pTaskFunction );

/************************************************************************/
/* wake up task */
/************************************************************************/
SINT32 OSAL_WakeupTask( OSAL_TASK *pTask );

/************************************************************************/
/* delete task */
/************************************************************************/
SINT32 OSAL_DeleteTask(OSAL_TASK *pTask);

/************************************************************************/
/* init event */
/************************************************************************/
SINT32 OSAL_InitEvent( OSAL_EVENT *pEvent, SINT32 InitVal );

/************************************************************************/
/* post event */
/************************************************************************/
SINT32 OSAL_GiveEvent( OSAL_EVENT *pEvent );

/************************************************************************/
/* wait event */
/************************************************************************/
SINT32 OSAL_WaitEvent( OSAL_EVENT *pEvent, SINT32 msWaitTime );

/************************************************************************/
/* lock init */
/************************************************************************/
VOID OSAL_SpinLockIRQInit(OSAL_IRQ_SPIN_LOCK *pIntrMutex);

/************************************************************************/
/* lock */
/************************************************************************/
SINT32 OSAL_SpinLockIRQ(OSAL_IRQ_SPIN_LOCK *pIntrMutex);

/************************************************************************/
/* unlock */
/************************************************************************/
SINT32 OSAL_SpinUnLockIRQ(OSAL_IRQ_SPIN_LOCK *pIntrMutex);
SINT32 KernelMemFree(MEM_DESC_S *pMemDesc);
UINT8 *KernelRegisterMap(UINT32 PhyAddr);
VOID   KernelRegisterUnMap(UINT8 *VirAddr);
SINT32 KernelMemMalloc(SINT8 * MemName, SINT32 len, SINT32 align, SINT32 IsCached, MEM_DESC_S *pMemDesc);
UINT8* KernelMmap(UINT32 phyaddr, UINT32 len);
UINT8* KernelMmapCache(UINT32 phyaddr, UINT32 len);
VOID   KernelMunmap(UINT8 *p );
SINT32 OSAL_DOWN_INTERRUPTIBLE(VOID);
VOID   OSAL_UP(VOID);
VOID   OSAL_SEMA_INTIT(VOID);

#endif


