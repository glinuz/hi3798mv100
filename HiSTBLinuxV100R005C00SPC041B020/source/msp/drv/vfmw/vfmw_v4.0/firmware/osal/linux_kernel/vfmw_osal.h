
#ifndef __VFMW_OSAL_HEADER__
#define  __VFMW_OSAL_HEADER__

#include "vfmw.h"
#include "hi_type.h"

#ifndef VFMW_BVT_SUPPORT
#include "hi_module.h"
#include "hi_drv_mem.h"
#include "hi_drv_mmz.h"
#include <asm/cacheflush.h>
#endif

#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/wait.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/ioctl.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/semaphore.h>

#include "vfmw_osal_ext.h"

/*======================================================================*/
/*                            constant define                           */
/*======================================================================*/

#define OSAL_OK     0
#define OSAL_ERR   -1

#define DIV_NS_TO_MS  1000000
#define DIV_NS_TO_US  1000

/*======================================================================*/
/*                            struct define                             */
/*======================================================================*/
typedef struct hiOSAL_MEM_S
{
    UADDR      PhyAddr;
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
    wait_queue_head_t   queue_head;
    SINT32              flag;
} KERN_EVENT_S;

typedef struct hiKERN_IRQ_LOCK_S
{
    spinlock_t     irq_lock;
    unsigned long  irq_lockflags;
    int            isInit;
} KERN_IRQ_LOCK_S;

/*======================================================================*/
/*                              define                                  */
/*======================================================================*/
typedef  struct task_struct    *OSAL_TASK;

typedef  KERN_EVENT_S           OSAL_EVENT;

typedef  KERN_EVENT_S           OSAL_TASK_MUTEX;

typedef  UINT32                 OSAL_IRQ_LOCK;

typedef  KERN_IRQ_LOCK_S        OSAL_IRQ_SPIN_LOCK;

typedef  struct file            OSAL_FILE;
typedef  struct file            FILE;

#define HIBVTALIGN(x, a)        ((a) * (((x) + (a) - 1) / (a)))

typedef struct semaphore OSAL_SEMA;


/*======================================================================*/
/*                          function declare                            */
/*======================================================================*/

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

/************************************************************************/
/* ms sleep */
/************************************************************************/
#define OSAL_MSLEEP(nMs)    msleep(nMs)

/************************************************************************/
/* get time */
/************************************************************************/
UINT32 OSAL_GetTimeInMs(VOID);
UINT32 OSAL_GetTimeInUs(VOID);

VOID OSAL_AcrtUsSleep(UINT32 us);

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
/* file: open/close/read/write */
/************************************************************************/
struct file *klib_fopen(const char *filename, int flags, int mode);
void         klib_fclose(struct file *filp);
int          klib_fread(char *buf, unsigned int len, struct file *filp);
int          klib_fwrite(char *buf, int len, struct file *filp);

/************************************************************************/
/* memory: alloc/free/map/unmap */
/************************************************************************/
void   klib_flush_cache(void *ptr, UADDR phy_addr, unsigned int len);
SINT32 KernelMemMalloc(UINT8 *MemName, UINT32 len, UINT32 align, UINT32 IsCached, MEM_DESC_S *pMemDesc);
SINT32 KernelMemFree(MEM_DESC_S *pMemDesc);
UINT8 *KernelRegisterMap(UADDR PhyAddr);
VOID   KernelRegisterUnMap(UINT8 *VirAddr);
UINT8 *KernelMmap(UADDR phyaddr, UINT32 len);
UINT8 *KernelMmapCache(UADDR phyaddr, UINT32 len);
VOID   KernelMunmap(UINT8 *p );
VOID   KernelFlushCache(VOID *ptr, UADDR phy_addr, UINT32 len);
SINT32 OSAL_DOWN_INTERRUPTIBLE(VOID);
VOID   OSAL_UP(VOID);
VOID   OSAL_SEMA_INTIT(VOID);
char*  OSAL_KMALLOC(SINT32 s32Size);
VOID   OSAL_KFREE(UADDR s32Addr);

SINT32 Down_Interruptible_with_Option(OSAL_SEMA *stSem);
VOID Up_Interruptible_with_Option(OSAL_SEMA *stSem);
VOID Sema_Init_with_Option(OSAL_SEMA *stSem);

VOID   Osal_SpinLockInit(SpinLockType spinLockType);
SINT32 Osal_SpinLock(SpinLockType spinLockType);
SINT32 Osal_SpinUnLock(SpinLockType spinLockType);

UINT32 strlcpy_command_line(char* dest, UINT32 size);

VOID InitVfmwInterface(VOID);

#endif


