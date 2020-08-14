#ifndef __VFMW_OSAL_EXT_HEADER__
#define __VFMW_OSAL_EXT_HEADER__

#include "vfmw.h"
#include "basetype.h"

typedef SINT32 (*vfmw_irq_handler_t)(SINT32, VOID *);

typedef enum FileOpenMode
{
    VFMW_O_RDWR   = 00000002,
    VFMW_O_CREATE = 00000100,
    VFMW_O_TRUNC  = 00001000
} FileOpenMode;

typedef enum SpinLockType
{
    G_TASKLOCK            = 0,
    G_RECORDLOCK          = 1,
    G_SPINLOCK            = 2,
    G_SPINLOCK_FSP        = 3,
    G_SPINLOCKIRQ_DESTROY = 4,
    G_ACTIVATELOCK        = 5,
    G_SPINLOCKISR_MSGQUE  = 6,
    G_SPINLOCKISR_IMAGE2D = 7,
    G_FRAMEBUF            = 8
} SpinLockType;

typedef enum FileType
{
    G_VFMW_SAVE_STR_FILE = 0,
    G_VFMW_SAEV_SEG_FILE = 1,
    G_VFMW_SAVE_YUV_FILE = 2
} FileType;

typedef enum MutexType
{
    G_INTEVENT = 0
} MutexType;

typedef enum RWLockType
{
    G_STATE_RWLOCK = 0
} RWLockType;

typedef UINT32 (*FN_OSAL_GetTimeInMs)(VOID);
typedef UINT32 (*FN_OSAL_GetTimeInUs)(VOID);

typedef VOID   (*FN_OSAL_SpinLockInit)(SpinLockType);
typedef SINT32 (*FN_OSAL_SpinLock)(SpinLockType);
typedef SINT32 (*FN_OSAL_SpinUnlock)(SpinLockType);

typedef VOID   (*FN_OSAL_SemaInit)(VOID);
typedef SINT32 (*FN_OSAL_SemaDown)(VOID);
typedef VOID   (*FN_OSAL_SemaUp)(VOID);

typedef VOID  *(*FN_OSAL_FileOpen)(const char *fileName, int flags, int mod);
typedef VOID   (*FN_OSAL_FileClose)(VOID *);
typedef SINT32 (*FN_OSAL_FileRead)(char *buffer, UINT32 length, VOID *pFile);
typedef SINT32 (*FN_OSAL_FileWrite)(char *buffer, UINT32 length, VOID *pFile);

typedef VOID  *(*FN_OSAL_MemSet)(VOID *s, int c, unsigned long n);
typedef VOID  *(*FN_OSAL_MemCpy)(VOID *dest, const VOID *src, unsigned long n);
typedef VOID  *(*FN_OSAL_MemMov)(VOID *dest, const VOID *src, unsigned long n);
typedef SINT32 (*FN_OSAL_MemCmp)(VOID *dest, const VOID *src, unsigned long n);

typedef SINT32 (*FN_OSAL_printk)(const char *fmt, ...);
typedef SINT32 (*FN_OSAL_dprint)(UINT32 type, const SINT8 *format, ...);

typedef SINT32 (*FN_OSAL_snprintf)(char *str, unsigned long size, const char *format, ...);

typedef SINT32 (*FN_OSAL_sscanf)(const char *str, const char *format, ...);

typedef SINT32 (*FN_OSAL_strncmp)(const char *s1, const char *s2, UINT32 n);

typedef UINT32 (*FN_OSAL_strlcpy)(char *dest, const char *src, UINT32 size);
typedef UINT32 (*FN_OSAL_strlcpy_command_line)(char *dest, UINT32 size);
typedef SINT8 *(*FN_OSAL_strsep)(char **stringp, const char *delim);
typedef long   (*FN_OSAL_simple_strtol)(const char *cp, char **endp, UINT32 base);
typedef SINT8 *(*FN_OSAL_strstr)(const char *haystack, const char *needle);

typedef VOID  *(*FN_OSAL_ioremap)(UADDR  phys_addr, unsigned long size);
typedef VOID   (*FN_OSAL_iounmap)(void *addr);

typedef VOID    (*FN_OSAL_mb)(VOID);

typedef VOID   (*FN_OSAL_udelay)(unsigned long usecs);
typedef VOID   (*FN_OSAL_msleep)(unsigned int msecs);

typedef SINT32 (*FN_OSAL_InitEvent)(MutexType mutexType, SINT32 initVal);
typedef SINT32 (*FN_OSAL_GiveEvent)(MutexType mutexType);
typedef SINT32 (*FN_OSAL_WaitEvent)(MutexType mutexType, SINT32 waitTimeInMs);

#if 0
typedef VOID   (*FN_OSAL_RWLockInit)(RWLockType rwLockType);
typedef VOID   (*FN_OSAL_ReadLock)(RWLockType rwLockType);
typedef VOID   (*FN_OSAL_ReadUnLock)(RWLockType rwLockType);
typedef VOID   (*FN_OSAL_WriteLock)(RWLockType rwLockType);
typedef VOID   (*FN_OSAL_WriteUnLock)(RWLockType rwLockType);
#endif

#if 0
typedef SINT32 (*FN_OSAL_MsgQue_InitMsgQue)(VOID);
typedef SINT32 (*FN_OSAL_MsgQue_GetMsg)(MSG_QUEUE_DATA *pMsgData, VOID **pMsgNode);
typedef SINT32 (*FN_OSAL_MsgQue_UpdateReadPtr)(SINT32 retVal, VOID *pMsgNode);
typedef SINT32 (*FN_OSAL_MsgQue_GetNodeNum)(VOID);
typedef SINT32 (*FN_OSAL_MsgQue_Control)(SINT32 chanID, VDEC_CID_E eCmdID, VOID *pArgs);
typedef SINT32 (*FN_OSAL_MsgQue_DeinitMsgQue)(VOID);
#endif

typedef VOID   (*FN_OSAL_FlushCache)(VOID *ptr, UADDR  phy_addr, UINT32 length);
typedef VOID   (*FN_OSAL_KernelFlushCache)(VOID *ptr, UADDR  phy_addr, UINT32 length);

typedef SINT32 (*FN_OSAL_KernelMemAlloc)(UINT8 *memName, UINT32 length, UINT32 align, UINT32 isCached, MEM_DESC_S *pMemDesc);
typedef SINT32 (*FN_OSAL_KernelMemFree)(MEM_DESC_S *pMemDesc);
typedef UINT8  *(*FN_OSAL_KernelRegisterMap)(UADDR  phyAddr);
typedef VOID   (*FN_OSAL_KernelRegisterUnMap)(UINT8 *virAddr);

typedef UINT8 *(*FN_OSAL_KernelMmap)(UADDR  phyAddr, UINT32 length);
typedef VOID   (*FN_OSAL_KernelMunMap)(UINT8 *p);

typedef UINT8 *(*FN_OSAL_KernelMmapCache)(UADDR  phyAddr, UINT32 length);

typedef SINT32 (*FN_OSAL_CreateTask)(VOID *pTask, SINT8 *taskName, VOID *pTaskFunc);

typedef SINT32 (*FN_OSAL_request_irq)(UINT32 irq, vfmw_irq_handler_t handler, unsigned long flags, const char *name, void *dev);
typedef VOID   (*FN_OSAL_free_irq)(UINT32 irq, void *dev);

typedef VOID  *(*FN_OSAL_phys_to_virt)(UADDR address);

typedef VOID  *(*FN_OSAL_AllocVirMem)(SINT32 size);
typedef VOID   (*FN_OSAL_FreeVirMem)(VOID *virMem);

typedef SINT32 (*FN_OSAL_vfmw_proc_init)(VOID);
typedef SINT32 (*FN_OSAL_vfmw_proc_exit)(VOID);

typedef SINT32 (*FN_OSAL_VDEC_Init)(SINT32 (*VdecCallback)(SINT32 ChanID, SINT32 eEventID, VOID *pArgs ));
typedef SINT32 (*FN_OSAL_VDEC_Exit)(VOID);

typedef SINT32 (*FN_OSAL_VDEC_Control)(SINT32 ChanID, VDEC_CID_E eCmdID, VOID *pArgs);

typedef struct Vfmw_Osal_Func_Ptr
{
    FN_OSAL_GetTimeInMs pfun_Osal_GetTimeInMs;
    FN_OSAL_GetTimeInUs pfun_Osal_GetTimeInUs;

    FN_OSAL_SpinLockInit pfun_Osal_SpinLockInit;
    FN_OSAL_SpinLock     pfun_Osal_SpinLock;
    FN_OSAL_SpinUnlock   pfun_Osal_SpinUnlock;

    FN_OSAL_SemaInit pfun_Osal_SemaInit;
    FN_OSAL_SemaDown pfun_Osal_SemaDown;
    FN_OSAL_SemaUp   pfun_Osal_SemaUp;

    FN_OSAL_FileOpen  pfun_Osal_FileOpen;
    FN_OSAL_FileClose pfun_Osal_FileClose;
    FN_OSAL_FileRead  pfun_Osal_FileRead;
    FN_OSAL_FileWrite pfun_Osal_FileWrite;

    FN_OSAL_MemSet pfun_Osal_MemSet;
    FN_OSAL_MemCpy pfun_Osal_MemCpy;
    FN_OSAL_MemMov pfun_Osal_MemMov;
    FN_OSAL_MemCmp pfun_Osal_MemCmp;

    FN_OSAL_printk pfun_Osal_printk;
    FN_OSAL_dprint pfun_Osal_dprint;

    FN_OSAL_snprintf pfun_Osal_snprintf;

    FN_OSAL_sscanf pfun_Osal_sscanf;

    FN_OSAL_strncmp pfun_Osal_strncmp;

    FN_OSAL_strlcpy_command_line pfun_Osal_strlcpy_command_line;

    FN_OSAL_simple_strtol pfun_Osal_simple_strtol;

    FN_OSAL_strlcpy pfun_Osal_strlcpy;
    FN_OSAL_strsep  pfun_Osal_strsep;
    FN_OSAL_strstr  pfun_Osal_strstr;

    FN_OSAL_ioremap pfun_Osal_ioremap;
    FN_OSAL_iounmap pfun_Osal_iounmap;

    FN_OSAL_mb     pfun_Osal_mb;

    FN_OSAL_udelay pfun_Osal_udelay;
    FN_OSAL_msleep pfun_Osal_msleep;

    FN_OSAL_InitEvent pfun_Osal_InitEvent;
    FN_OSAL_GiveEvent pfun_Osal_GiveEvent;
    FN_OSAL_WaitEvent pfun_Osal_WaitEvent;

#if 0
    FN_OSAL_RWLockInit  pfun_Osal_RWLockInit;
    FN_OSAL_ReadLock    pfun_Osal_ReadLock;
    FN_OSAL_ReadUnLock  pfun_Osal_ReadUnLock;
    FN_OSAL_WriteLock   pfun_Osal_WriteLock;
    FN_OSAL_WriteUnLock pfun_Osal_WriteUnLock;

    FN_OSAL_MsgQue_InitMsgQue    pfun_Osal_MsgQue_InitMsgQue;
    FN_OSAL_MsgQue_GetMsg        pfun_Osal_MsgQue_GetMsg;
    FN_OSAL_MsgQue_UpdateReadPtr pfun_Osal_MsgQue_UpdateReadPtr;
    FN_OSAL_MsgQue_GetNodeNum    pfun_Osal_MsgQue_GetNodeNum;
    FN_OSAL_MsgQue_Control       Pfun_Osal_MsgQue_Control;
    FN_OSAL_MsgQue_DeinitMsgQue  pfun_Osal_MsgQue_DeinitMsgQue;
#endif

    FN_OSAL_FlushCache       pfun_Osal_FlushCache;
    FN_OSAL_KernelFlushCache pfun_Osal_KernelFlushCache;

    FN_OSAL_KernelMemAlloc pfun_Osal_KernelMemAlloc;
    FN_OSAL_KernelMemFree  pfun_Osal_KernelMemFree;

    FN_OSAL_KernelRegisterMap   pfun_Osal_KernelRegisterMap;
    FN_OSAL_KernelRegisterUnMap pfun_Osal_KernelRegisterUnMap;

    FN_OSAL_KernelMmap   pfun_Osal_KernelMmap;
    FN_OSAL_KernelMunMap pfun_Osal_KernelMunMap;

    FN_OSAL_KernelMmapCache pfun_Osal_KernelMmapCache;

    FN_OSAL_CreateTask pfun_Osal_CreateTask;

    FN_OSAL_request_irq pfun_Osal_request_irq;
    FN_OSAL_free_irq    pfun_Osal_free_irq;

    FN_OSAL_phys_to_virt pfun_Osal_phys_to_virt;

    FN_OSAL_AllocVirMem pfun_Osal_AllocVirMem;
    FN_OSAL_FreeVirMem  pfun_Osal_FreeVirMem;

    FN_OSAL_vfmw_proc_init pfun_Osal_vfmw_proc_init;
    FN_OSAL_vfmw_proc_exit pfun_Osal_vfmw_proc_exit;

    FN_OSAL_VDEC_Init      pfun_Osal_vdec_init;
    FN_OSAL_VDEC_Exit      pfun_Osal_vdec_exit;

    FN_OSAL_VDEC_Control   pfun_Osal_vdec_control;
} Vfmw_Osal_Func_Ptr;

#endif
