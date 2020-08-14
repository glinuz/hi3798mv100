#ifndef  __DRV_VENC_OSAL_H__
#define  __DRV_VENC_OSAL_H__

#include <linux/wait.h>
#include <linux/sched.h>
#include "hi_type.h"
#include "hal_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct hiKERN_EVENT_S
{
    wait_queue_head_t   queue_head;
    HI_S32              flag;
} KERN_EVENT_S;

typedef  KERN_EVENT_S        VEDU_OSAL_EVENT;

HI_VOID       VENC_DRV_OsalIrqFree(HI_U32 Irq);
HI_S32        VENC_DRV_OsalIrqInit(HI_U32 Irq, HI_VOID (*ptrCallBack)(HI_VOID));
HI_S32        VENC_DRV_OsalLockCreate (HI_VOID** phLock);
HI_VOID       VENC_DRV_OsalLockDestroy(HI_VOID* hLock);
HI_VOID       VENC_DRV_OsalLock(HI_VOID* hLock, VEDU_LOCK_FLAG* pFlag);
HI_VOID       VENC_DRV_OsalUnlock(HI_VOID* hLock, VEDU_LOCK_FLAG* pFlag);
HI_S32        VENC_DRV_OsalCreateTask(HI_VOID** phTask, HI_U8 TaskName[], HI_VOID* pTaskFunction);
HI_S32        VENC_DRV_OsalDeleteTask(HI_VOID* hTask);

HI_S32 VENC_DRV_OsalInitEvent( VEDU_OSAL_EVENT* pEvent, HI_S32 InitVal );
HI_S32 VENC_DRV_OsalGiveEvent( VEDU_OSAL_EVENT* pEvent );
HI_S32 VENC_DRV_OsalWaitEvent( VEDU_OSAL_EVENT* pEvent, HI_U32 msWaitTime );

struct file* VENC_DRV_OsalFopen(const char* filename, int flags, int mode);
void VENC_DRV_OsalFclose(struct file* filp);
int VENC_DRV_OsalFread(char* buf, unsigned int len, struct file* filp);
int VENC_DRV_OsalFwrite(char* buf, int len, struct file* filp);

HI_S32 HI_DRV_VENC_AllocAndMap(const char *bufname, char *zone_name, HI_U32 size, int align, VENC_BUFFER_S *psMBuf);
HI_VOID HI_DRV_VENC_UnmapAndRelease(VENC_BUFFER_S *psMBuf);
HI_S32 HI_DRV_VENC_Alloc(const char *bufname, char *zone_name, HI_U32 size, int align, VENC_BUFFER_S *psMBuf);
HI_S32 HI_DRV_VENC_MapCache(VENC_BUFFER_S *psMBuf);
HI_S32 HI_DRV_VENC_Flush(VENC_BUFFER_S *psMBuf);
HI_S32 HI_DRV_VENC_Map(VENC_BUFFER_S *psMBuf);
HI_VOID HI_DRV_VENC_Unmap(VENC_BUFFER_S *psMBuf);
HI_VOID HI_DRV_VENC_Release(VENC_BUFFER_S *psMBuf);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif //__DRV_VENC_OSAL_H__
