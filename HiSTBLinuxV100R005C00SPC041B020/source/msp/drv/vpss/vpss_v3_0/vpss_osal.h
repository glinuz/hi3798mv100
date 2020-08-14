#ifndef __VPSS_OSAL_H__
#define __VPSS_OSAL_H__

#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/rwlock.h>

#include <asm/uaccess.h>
#include "hi_type.h"
#include "hi_osal.h"
#include "hi_math.h"
#include "hi_common.h"
#include "vpss_common.h"
#include "hi_drv_mmz.h"
#include "hi_drv_video.h"

 
/************************************************************************/
/*                         data structure                               */
/************************************************************************/

#define OSAL_OK     0
#define OSAL_ERR   -1

#define EVENT_DONE 1
#define EVENT_UNDO 0

#define HI_ALIGN_BYTES 16
#define HI_ALIGN_8BIT_YSTRIDE(x)         HI_SYS_GET_STRIDE(x)
#define HI_ALIGN_10BIT_COMP_YSTRIDE(x)   HI_SYS_GET_STRIDE(HICEILING(x*10, 8))   /* l0bit½ô´ÕÄ£Ê½ */

typedef  struct file   FILE;

#define VPSS_MEM_FLAG_NORMAL 0x0
#define VPSS_MEM_FLAG_SECURE 0x1
#define VPSS_MEM_FLAG_MMU 0x2



/************************************************************************/
/* file operation                                                       */
/************************************************************************/
struct file *VPSS_OSAL_fopen(const char *filename, int flags, int mode);
void VPSS_OSAL_fclose(struct file *filp);
int VPSS_OSAL_fread(char *buf, unsigned int len, struct file *filp);
int VPSS_OSAL_fwrite(char *buf, int len, struct file *filp);

/************************************************************************/
/* event operation                                                      */
/************************************************************************/
typedef struct hiKERN_EVENT_S
{
	wait_queue_head_t   queue_head;
	HI_S32              flag_1;
	HI_S32              flag_2;
} KERN_EVENT_S;

typedef  KERN_EVENT_S           OSAL_EVENT;
HI_S32 VPSS_OSAL_InitEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2);

HI_S32 VPSS_OSAL_GiveEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2);

HI_S32 VPSS_OSAL_WaitEvent( OSAL_EVENT *pEvent, HI_S32 s32WaitTime ); 

HI_S32 VPSS_OSAL_ResetEvent( OSAL_EVENT *pEvent, HI_S32 InitVal1, HI_S32 InitVal2);



/************************************************************************/
/* mutux lock operation                                                 */
/************************************************************************/
typedef struct semaphore  VPSS_OSAL_LOCK;

HI_S32 VPSS_OSAL_InitLOCK(VPSS_OSAL_LOCK *pLock, HI_U32 u32InitVal);

HI_S32 VPSS_OSAL_DownLock(VPSS_OSAL_LOCK *pLock);

HI_S32 VPSS_OSAL_UpLock(VPSS_OSAL_LOCK *pLock); 

HI_S32 VPSS_OSAL_TryLock(VPSS_OSAL_LOCK *pLock); 



/************************************************************************/
/* spin lock operation                                                  */
/************************************************************************/
typedef spinlock_t VPSS_OSAL_SPIN;

HI_S32 VPSS_OSAL_InitSpin(VPSS_OSAL_SPIN *pLock);

HI_S32 VPSS_OSAL_DownSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags);

HI_S32 VPSS_OSAL_UpSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags);

HI_S32 VPSS_OSAL_TryLockSpin(VPSS_OSAL_SPIN *pLock,unsigned long *flags);


/************************************************************************/
/* debug operation                                                      */
/************************************************************************/
HI_S32 VPSS_OSAL_GetProcArg(HI_CHAR*  chCmd,HI_CHAR*  chArg,HI_U32 u32ArgIdx);

HI_S32 VPSS_OSAL_ParseCmd(HI_CHAR*  chArg1,HI_CHAR*  chArg2,HI_CHAR*  chArg3,HI_VOID *pstCmd);

HI_S32 VPSS_OSAL_StrToNumb(HI_CHAR*  chStr,HI_U32 *pu32Numb);

HI_S32 VPSS_OSAL_WRITEYUV(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR* pchFile);
HI_S32 VPSS_OSAL_CalBufSize(HI_U32 *pSize,HI_U32 *pStride,HI_U32 u32Height,HI_U32 u32Width,HI_DRV_PIX_FORMAT_E ePixFormat,HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth);

HI_S32 VPSS_OSAL_GetTileOffsetAddr(HI_U32 u32Xoffset,HI_U32 u32Yoffset,
                                   HI_U32 *pu32Yaddr,HI_U32 *pu32Caddr,
                                   HI_DRV_VID_FRAME_ADDR_S *pstOriAddr);

HI_S32 VPSS_OSAL_GetVpssVersion(VPSS_VERSION_E *penVersion, VPSS_CAPABILITY_U *penCapability);

HI_S32 VPSS_OSAL_GetCurTime(HI_U32 *pu32Hour,HI_U32 *pu32Minute,
                                   HI_U32 *pu32Second);

HI_S32 VPSS_OSAL_GetSysMemSize(HI_U32 *pu32MemSize);

typedef struct hiVPSS_MEM_S
{
	HI_U8 u8flag;
	HI_U32 u32Size;
	HI_U32 u32StartPhyAddr;
	HI_U8  *pu8StartVirAddr;
} VPSS_MEM_S;

HI_S32 VPSS_OSAL_AllocateMem(HI_U8 u8flag, 
		HI_U32  u32Size,
		HI_U8  *pu8ZoneName,
		HI_U8  *pu8MemName,
		VPSS_MEM_S *pstMem);

HI_S32 VPSS_OSAL_FreeMem(VPSS_MEM_S *pstMem);
#endif
