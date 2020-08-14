#ifndef  __UNF_OSAL_H__
#define __UNF_OSAL_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_struct.h"

#ifndef WIN32
#include <linux/vmalloc.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
//#include <linux/kcom.h>
//#include <mach/hardware.h>
#include <asm/signal.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/mm.h>

#else
#include <stdlib.h>
#include <memory.h>
#endif

/** unf_osal.h
 *  Interface definitions of the OS abstraction layer.
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/**********************************************************************
* INTERRUPT
**********************************************************************/
HI_VOID               UNF_OSAL_IrqLock( HI_U32 *pFlag );
HI_VOID               UNF_OSAL_IrqUnlock( HI_U32 *pFlag );

/**********************************************************************
* MEMORY
**********************************************************************/

#if 0
// TODO, hardware special memory
HI_S32                UNF_OSAL_AllocMemory( SINT32 ExpectPhyAddr, SINT32 ExpectSize, OSAL_MEM_S *pOsalMem );
HI_S32                UNF_OSAL_ReleaseMemory( OSAL_MEM_S *pMemRet );
HI_S32                UNF_OSAL_MapRegisterAddr( SINT32 RegPhyAddr, SINT32 Range, OSAL_MEM_S *pOsalMem );
HI_S32                UNF_OSAL_UnmapRegisterAddr( OSAL_MEM_S *pOsalMem );
#endif

/**********************************************************************
* THREADS
**********************************************************************/
HI_S32                UNF_OSAL_ThreadCreate( HI_U32            (*pFunc)(HI_VOID* pParam),
                                             HI_CHAR           TaskName[],
                                             HI_VOID*          pParam,
                                             HI_U32            nPriority,
                                             HI_VOID**         phThread);
HI_S32                UNF_OSAL_ThreadDestroy( HI_VOID* hThread);

/**********************************************************************
* MUTEX
**********************************************************************/
HI_S32                UNF_OSAL_MutexCreate( HI_VOID**phMutex);
HI_S32                UNF_OSAL_MutexDestroy( HI_VOID* hMutex);
HI_S32                UNF_OSAL_MutexLock( HI_VOID* hMutex);
HI_S32                UNF_OSAL_MutexUnlock( HI_VOID* hMutex);

/**********************************************************************
* EVENTS
**********************************************************************/
#define UNF_INFINITE_WAIT 0xffffffff
HI_S32                UNF_OSAL_EventCreate( HI_VOID**phEvent);
HI_S32                UNF_OSAL_EventDestroy( HI_VOID* hEvent);
HI_S32                UNF_OSAL_EventReset( HI_VOID* hEvent);
HI_S32                UNF_OSAL_EventSignal( HI_VOID* hEvent);
HI_S32                UNF_OSAL_EventWait( HI_VOID* hEvent, HI_U32 mSec, HI_BOOL *pbTimedOut);

/**********************************************************************
* TIME
**********************************************************************/
HI_U32                UNF_OSAL_GetTime(HI_VOID);
HI_VOID               UNF_OSAL_Sleep(HI_U32 mSec);


HI_VOID DMX_AcrtUsSleep(HI_U32 us);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
