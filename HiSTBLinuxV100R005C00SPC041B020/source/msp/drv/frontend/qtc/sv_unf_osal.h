#ifndef  __SV_UNF_OSAL_H__
#define __SV_UNF_OSAL_H__

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_struct.h"

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
#include <linux/smp_lock.h>
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
#include <mach/hardware.h>
#include <asm/signal.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include "common_mem.h"


/** sv_unf_osal.h
 *  Interface definitions of the OS abstraction layer.
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define HI_FATAL_QTC(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_FATAL, HI_DEBUG_ID_TUNER, fmt)

#define HI_ERR_QTC(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_TUNER, fmt)

#define HI_WARN_QTC(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_WARNING, HI_DEBUG_ID_TUNER, fmt)

#define HI_INFO_QTC(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_TUNER, fmt)

/* print test case name, adds to the start of each test case function*/
/*CNcomment:打印测试用例名称， 加在每个用例函数开始处 */
#define D_SVPrintTc()  HI_INFO_QTC(" FUNC = %s\n", __FUNCTION__) 


/***************************************************** 
* the struct of the test case
*****************************************************/
typedef enum hiSV_TC_STATUS_E
{
    /* state of the test case 0-Stop 1-Running*/
    /*CNcomment:用例状态 0-Stop 1-Running*/
    HI_SV_TC_STATUS_STOP = 0,
    HI_SV_TC_STATUS_RUNNING,

    /* execute test cases by sequence automatically*/
    /*CNcomment:自动按顺序执行用例 */
    HI_SV_TC_STATUS_AUTORUN, 

    HI_SV_TC_STATUS_BUTT
}HI_SV_TC_STATUS_E;

/*the length of test case description*//*CNcomment:用例描述长度 */
#define SV_TC_MAX_DESC_LEN 100 

typedef HI_VOID (* SV_TC_FUNC)(HI_VOID);

typedef struct hiSV_TC_S
{
    /*description of the test case*//*CNcomment: 用例描述*/
    HI_CHAR     tcDesc[SV_TC_MAX_DESC_LEN];

    /*the function pointer that executes test case*//*CNcomment: 用例执行函数指针 */
    SV_TC_FUNC pEnterFunc; 

    /*the function pointer that exits test case*//*CNcomment:用例退出函数指针 */
    SV_TC_FUNC pExitFunc; 

    HI_SV_TC_STATUS_E enTCStatus;
}HI_SV_TC_S;

typedef struct hiSV_TC_PROC_S
{
    HI_SV_TC_S * pSVTc;
    HI_U32 u32TcCnt; /*total tc count*/
}HI_SV_TC_PROC_S;

HI_S32 SV_Tc_ProcInit(HI_CHAR * chTcName, HI_SV_TC_PROC_S * stTcProc);
HI_VOID SV_Tc_ProcDeInit(HI_CHAR * chTcName);

typedef struct file SV_FILE;
/**********************************************************************
* MEMORY
**********************************************************************/
HI_VOID*              SV_OSAL_MemMalloc( HI_U32 size);
HI_VOID               SV_OSAL_MemFree( HI_VOID* pData);
HI_VOID*              SV_OSAL_MemSet(HI_VOID *pDest, HI_U32 cChar, HI_U32 nCount);
HI_VOID*              SV_OSAL_MemCpy(HI_VOID *pDest, HI_VOID *pSrc, HI_U32 nCount);

HI_S32 SV_BUFMEM_AllocAndMapMem(const HI_S8 *bufname, HI_U32 size, HI_S32 align, MMZ_BUFFER_S *psMBuf);
HI_VOID SV_BUFMEM_UnmapAndReleaseMem(MMZ_BUFFER_S *psMBuf);
struct file *SV_klib_fopen(const char *filename, int flags, int mode);
HI_VOID SV_klib_fclose(struct file *filp);
HI_S32  SV_klib_fread(HI_S8 *buf, HI_U32 len, struct file *filp);
HI_S32  SV_klib_fwrite(HI_S8 *buf, HI_S32 len, struct file *filp);
HI_S32 SV_klib_seek(loff_t offset, int origin, struct file *filp);
HI_S32 SV_klib_tell(struct file *filp);

HI_U32 SV_GetSysTime(HI_VOID);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
