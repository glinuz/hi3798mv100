/*                                                                           */
/* Author: Wang daneng                                                          */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date: 2013-05-16                                                          */
/*                                                                           */
/* Description:                                                              */
/*     System Call ID                                   */
/* Others:                                                                   */
/*                                                                           */
/* History:                                                                  */
/* 1. Date: 2013-05-16                                                       */
/*    Author: Wang daneng                                                       */
/*    Modification: Create this file                                         */
/*                                                                           */
/*****************************************************************************/

#ifndef __SRE_SYSCALL_H_
#define __SRE_SYSCALL_H_

#include "sre_base.h"
#include "sre_task.h"
#include "sre_ticktimer.h"
#include "sre_msg.h"
#include "sre_swi.h"
#include "sre_sem.h"
#include "sre_fsem.h"
#include "sre_task.h"
#include "sre_hwi.h"
#include "sre_mem.h"
#include "sre_package.h"
#include "sre_securemmu.h"

//cc
#ifdef CC_DRIVER
#include "dx_cclib.h"
#include "crys_hash.h"
#include "crys_hmac.h"
#include "crys_des.h"
#include "crys_aes.h"
#include "crys_aesccm.h"
#include "crys_kdf.h"
#include "dx_util.h"
#include "crys_rnd.h"
#include "crys_rsa_build.h"
#include "crys_rsa_schemes.h"
#include "crys_rsa_prim.h"
#include "crys_dh.h"
#include "crys_ecpki_types.h"
#include "dx_cc_defs.h"
#include "dx_util_oem_asset.h"
#include "hdcp_driver.h"
#endif

typedef struct swi_temp_regs {
	UINT32 spsr;
	UINT32 regs[4];
	UINT32 regs_r4_12[9];
	UINT32 regs_lr;   //r12 and lr pushed in sre_vector.s
	UINT32 regs_fp;   //this push occur in sre_syscalls.S
	UINT32 regs_lr_2; //this push occur in sre_syscalls.S
	UINT32 args[12];

} stSWITempRegs;

extern void swi_handler(int swi_id, void *input);

/*console*/
extern void __console_flush(void);
extern void __console_putc(unsigned int n);
extern void __console_reinit(void);
extern void __sre_snprintf_putc(unsigned n, void *cookie);

/*SMC*/
extern void __start_tz();
extern void __smc_switch();

/*MMU*/
extern int __map_to_ns(UINT32 phy_addr , UINT32 size, secure_mode_type non_secure);
extern int __unmap_from_ns(UINT32 va_addr, UINT32 size);
extern int __map_from_ns_page(UINT32 phy_addr , UINT32 size, UINT32 *virt_addr, secure_mode_type non_secure);
extern int __unmap_from_ns_page(UINT32 va_addr, UINT32 size);
extern int __check_secure_address(UINT32 address, UINT32 size);
extern unsigned int __isSecureContentMemory(paddr_t addr, unsigned int size);

extern int __task_map_from_ns_page(UINT32 task_id, paddr_t phy_addr , UINT32 size, UINT32 *virt_addr, secure_mode_type non_secure);
extern int __task_unmap_from_ns_page(UINT32 task_id, UINT32 va_addr, UINT32 size);

extern UINT32 __virt_to_phys(UINT32 addr);


/*Task*/
extern UINT32 __SRE_TaskCreate(UINT32 *puwTaskPID, TSK_INIT_PARAM_S *pstInitParam);
extern UINT32 __SRE_TaskResume(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskSuspend(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskDelete(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskDelete_ex(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskDelay(UINT32 uwTick);
extern VOID __SRE_TaskLock(VOID);
extern VOID __SRE_TaskUnlock(VOID);
extern UINT32 __SRE_TaskSelf(UINT32 *puwTaskPID);
extern UINT32 __SRE_TaskSelfName(char *task_name);
extern BOOL __SRE_TaskIDCheck(UINT32 uwTaskPID);
extern TSK_STATUS_T __SRE_TaskStatusGet(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskInfoGet(UINT32 uwTaskPID, TSK_INFO_S *pstTaskInfo);
extern UINT32 __SRE_TaskPrivateDataGet();
extern VOID __SRE_TaskPrivateDataSet(UINT32 uwPrivateData);
extern TSK_PRIOR_T __SRE_TaskPriorityGet(UINT32 uwTaskPID);
extern UINT32 __SRE_TaskPrioritySet(UINT32 uwTaskPID, TSK_PRIOR_T usTaskPrio);
extern UINT32 __SRE_TaskYield(TSK_PRIOR_T  usTaskPrio,
			      UINT32 uwNextTask,
			      UINT32 *puwYeildTo);
extern UINT32 __SRE_TaskCreateHookAdd(TSK_CREATE_HOOK pfnHook);
extern UINT32 __SRE_TaskCreateHookDelete(TSK_CREATE_HOOK pfnHook);
extern UINT32 __SRE_TaskDeleteHookAdd(TSK_DELETE_HOOK pfnHook);
extern UINT32 __SRE_TaskDeleteHookDelete(TSK_DELETE_HOOK pfnHook);
extern UINT32 __SRE_TaskSwitchHookAdd(TSK_SWITCH_HOOK pfnHook);
extern UINT32 __SRE_TaskSwitchHookDelete(TSK_SWITCH_HOOK pfnHook);
extern UINT32 __SRE_TaskPendSemGet(TSK_HANDLE_T uwTaskID, UINT16 *pusSemID, UINT16 *pusPendState);
extern UINT32 __SRE_TaskMsgNumGet(UINT32 uwQid);
extern VOID __SRE_TestTaskSchedule(VOID);
extern UINT32 __SRE_TaskCurrentMemInfo(TSK_MEM_INFO_S *usTaskMemInfo);
extern VOID __SRE_TaskPidClassHandle(UINT32 type, UINT32 operation, TSK_HANDLE_T *set_pid);

/*Timer*/
extern timer_event *__SRE_TimerEventCreate(sw_timer_event_handler handler, int timer_class, void *priv_data);
extern INT32 __SRE_TimerEventDestroy(timer_event *pstTevent);
extern UINT32 __SRE_TimerEventStart(timer_event *pstTevent, timeval_t *time);
extern VOID __SRE_TimerEventStop(timer_event *pstTevent);
extern UINT64 __SRE_TimerGetExpire(timer_event *pstTevent);
extern INT32 __SRE_TimerCheck(timer_notify_data_kernel *timer_data);
extern UINT64 __SRE_ReadTimestamp(VOID);
extern UINT32 __SRE_SwMsleep(UINT32 uwMsecs);
extern UINT32 __SRE_SwSleep(UINT32 uwSecs);  /*供测试使用，不对外*/
extern VOID __get_startup_time(TEE_Time_kernel *time, UINT32 *rtc_time);

/*Msg*/
#if 1 /*UT用例专用，用户空间不能使用下面接口*/
extern MSG_HANDLE_T __SRE_MsgReceive(UINT32 uwTimeout, UINT32 *puwMsgID, MSG_PID_T *puwSenderPID);
extern UINT32 __SRE_MsgQSend(MSG_HANDLE_T uwMsgHandle, UINT32 uwMsgID, MSG_PID_T uwDstPID, UINT8 ucDstQID);
extern MSG_HANDLE_T __SRE_MsgAlloc(UINT32 uwPtNo, UINT16 usSize, UINT32 uwAlignPow);
extern UINT32 __SRE_MsgDelete(MSG_HANDLE_T uwMsgHandle);
extern UINT32 __SRE_MsgQRecv(MSG_HANDLE_T *puwMsgHandle, UINT32 *puwMsgID, MSG_PID_T *puwSenderPID, UINT8 ucRecvQID, UINT32 uwTimeout);
#endif

extern UINT32 __SRE_MsgSnd(UINT32 uwMsgID, MSG_PID_T uwDstPID, void *msgp, UINT32 size);
extern UINT32 __SRE_MsgRcv(UINT32 uwTimeout, UINT32 *puwMsgID, void *msgp, UINT32 size);
extern UINT32 __SRE_MsgRcvA(UINT32 uwTimeout, UINT32 *puwMsgID, void *msgp, UINT32 size, MSG_PID_T *puwSenderPID);

/*Mem*/
extern UINT32 __SRE_MemUsageGet(UINT8 ucPtNo);
extern VOID __SRE_PullKernelVariables(void *);
extern VOID __SRE_UpdateKernelVariables(void *);
extern VOID __SRE_PushKernelVariables(void *);
extern VOID __SRE_PushRdrMemAddr(paddr_t addr, UINT32 size);
extern UINT32 __SRE_GetLogLevel();
extern UINT32 __get_android_version();
/* this syscall only used for test*/
extern void __test_klog_func(UINT32 level);
extern INT32 __SRE_SetKMROT(UINT8 *in, UINT32 in_len);
extern INT32 __SRE_GetKMROT(UINT8 *out, UINT32 out_len);
extern VOID *__SRE_TaskMem_Malloc(UINT32 size, UINT32 hint);
extern VOID __SRE_TaskMem_Free(UINT32 *addr);
extern UINT32 __SRE_TaskMem_GetOrder(UINT32 size);
extern UINT32 __SRE_TaskMem_GetBase(UINT32 *tasknum);
extern VOID __SRE_TaskMem_GetInternalTaskInfo(UINT32 *addr, UINT32 start_index, UINT32 num);
extern UINT32 __SRE_Dynamic_Loadelf(char *file_buffer, int file_size, UINT32 stack_size, UINT32 heap_size, section_info_t *p_section_info, struct sessison_task_info *p_sti);
extern UINT32 __SRE_Release_Dynamic_Region(char *task_name, UINT32 namelen, UINT32 *p_addr, UINT32 *vm_addr, UINT32 size);
extern VOID *__SRE_MemAlloc(UINT32 uwMid, UINT8 ucPtNo, UINT32 uwSize);
extern VOID *__SRE_MemAllocAlign(UINT32 uwMid, UINT8 ucPtNo, UINT32 uwSize, OS_MEM_ALIGN_E enAlignPow);
extern UINT32 __SRE_MemFree(UINT32 uwMid, VOID *pAddr);

extern VOID *__SRE_MemAllocShared(UINT32 uwSize);
extern UINT32 __SRE_MemFreeShared(VOID *pAddr);

/*cache*/
extern VOID __SRE_CacheCodeUpdate(UINT32 *addr, UINT32 size);
extern VOID __SRE_DCacheFlush(UINT32 *addr, UINT32 size);
extern void __v7_dma_flush_range(unsigned long start, unsigned long end);
extern void __v7_dma_inv_range(unsigned long start, unsigned long end);
extern void __v7_flush_kern_cache_all(void);

/*hunt*/
extern UINT32 __SRE_HuntByName(UINT8 ucCoreID, CHAR *pThreadName, UINT32 *puwPID);

/*swi*/
#if (OS_INCLUDE_SWI == YES)
extern UINT32 __SRE_SwiCreate(SWI_HANDLE_T *puwSwiID, SWI_INIT_PARA_S *pstInitPara);
extern UINT32 __SRE_SwiDelete(SWI_HANDLE_T uwSwiID);
extern VOID __SRE_SwiEnable(SWI_PRIOR_T usPriority);
extern SWI_PRIOR_T __SRE_SwiDisable(VOID);
extern UINT32 __SRE_SwiActivate(SWI_HANDLE_T uwSwiID);
extern UINT32 __SRE_SwiInc(SWI_HANDLE_T uwSwiID);
extern UINT32 __SRE_SwiDec(SWI_HANDLE_T uwSwiID);
extern UINT32 __SRE_SwiOr(SWI_HANDLE_T uwSwiID, UINT32 uwMask);
extern UINT32 __SRE_SwiAndn(SWI_HANDLE_T uwSwiID, UINT32 uwMask);
extern UINT32 __SRE_SwiValueGet(VOID);
extern SWI_STATUS_T __SRE_SwiStatusGet(SWI_HANDLE_T uwSwiID);
extern UINT32 __SRE_SwiInfoGet(SWI_HANDLE_T uwSwiID, SWI_INFO_S *pstSwiInfo);
extern UINT32 __SRE_SwiSelf(SWI_HANDLE_T *pusSwiID);
extern SWI_PRIOR_T __SRE_SwiPriorityGet(SWI_HANDLE_T uwSwiID);
extern INT32 __SRE_SwiMsgNumGet(SWI_HANDLE_T uwSwiID);
#endif
/*sem*/
extern  UINT32 __SRE_SemCreate(UINT32 uwCount,  SEM_HANDLE_T *puwSemHandle);
extern UINT32 __SRE_SemDelete(SEM_HANDLE_T uwSemHandle);
extern UINT32 __SRE_SemReset(SEM_HANDLE_T uwSemHandle, UINT32 uwCount);
extern UINT32 __SRE_SemCountGet(SEM_HANDLE_T uwSemHandle, UINT32 *puwSemCnt);
extern UINT32 __SRE_SemPend(SEM_HANDLE_T uwSemHandle, UINT32 uwTimeout);
extern UINT32 __SRE_SemPost(SEM_HANDLE_T uwSemHandle);
extern UINT32 __SRE_SemPendListGet(SEM_HANDLE_T uwSemHandle,
				   UINT32 *puwTskCnt,
				   UINT32 *puwPidBuf,
				   UINT32  uwBufLen);

/*fsem*/
extern UINT32 __SRE_FsemSet(TASK_PID_T uwTaskPID, INT32 swFsemVal);
extern INT32 __SRE_FsemGet(TASK_PID_T uwTaskPID);
extern UINT32 __SRE_FsemPend(INT32 swFsemVal);
extern UINT32 __SRE_FsemPost(TASK_PID_T uwTaskPID);

/*hwi*/
extern UINT32 __SRE_HwiCreate(HWI_HANDLE_T  uwHwiNum,
			      HWI_PRIOR_T   usHwiPrio,
			      HWI_MODE_T    usMode,
			      HWI_PROC_FUNC pfnHandler,
			      HWI_ARG_T     uwArg);
extern UINT32 __SRE_HwiDelete(HWI_HANDLE_T uwHwiNum);
extern UINT32 __SRE_HwiTrigger(UINT32 uwMode, UINT32 uwDstCore , HWI_HANDLE_T uwHwiNum);
extern UINT32 __SRE_HwiClearPendingBit(HWI_HANDLE_T uwHwiNum);
extern UINTPTR __SRE_HwiDisable(HWI_HANDLE_T uwHwiNum);
extern UINTPTR __SRE_HwiEnable(HWI_HANDLE_T uwHwiNum);
extern UINTPTR __SRE_IntLock(VOID);
extern VOID __SRE_IntRestore(UINTPTR uvIntSave);
extern UINTPTR __SRE_IntUnLock(VOID);
extern UINT32 __SRE_GetCurrCPSR(VOID);

/* HWI IPC */
extern UINT32 __SRE_HwiMsgRegister(HWI_HANDLE_T uwHwiNum);
extern UINT32 __SRE_HwiMsgDeregister(HWI_HANDLE_T uwHwiNum);
extern UINT32 __SRE_HwiMsgCreateEnable(HWI_HANDLE_T uwHwiNum);
/*sys*/
extern VOID __osTskExit(TSK_HANDLE_T uwTaskID);

/*RTC*/
extern void __secure_rtc_init(void);
extern unsigned int __get_secure_rtc_time(void);
extern void __init_startup_time_kernel(unsigned int rtc_time);

extern void __adjust_sys_time(TEE_Time_kernel *time);
extern void __get_sys_date_time(TEE_Date_Time_kernel *date_time);
extern void __gen_sys_date_time(UINT32 secs, TEE_Date_Time_kernel *date_time);
extern int __SRE_Swi_Test();
extern void __spi_notify();

/*sec boot*/
extern UINT32 __hisi_secboot_copy_soc_data(UINT32 soc_type, UINT32 offset, const paddr_t src_addr, unsigned len);
extern UINT32 __hisi_secboot_soc_verification(UINT32 socType, UINT32 vrlAddress, paddr_t imageAddress, unsigned int lock_state);
extern UINT32 __hisi_secboot_soc_reset(UINT32 soc_type);
extern UINT32 __hisi_secboot_soc_set(UINT32 soc_type, int reset_type);
//extern int __hisi_secboot_get_img_addr(UINT32 running_addr, UINT32 image_size);
extern void __hisi_secboot_get_vrl_addr(UINT32 vrl_address);

//backtraces
extern void __dump_backtraces(void);
extern INT32 __dump_backtraces_test(void);

/*cc*/
#ifdef CC_DRIVER
extern DX_CclibRetCode_t __CC_DX_CclibInit(void);

extern CRYSError_t __CC_CRYS_HASH_Init(
	CRYS_HASHUserContext_t *ContextID_ptr,
	CRYS_HASH_OperationMode_t  OperationMode);
extern CRYSError_t __CC_CRYS_HASH_Init(
	CRYS_HASHUserContext_t *ContextID_ptr,
	CRYS_HASH_OperationMode_t  OperationMode);
extern CRYSError_t __CC_CRYS_HASH_Update(
	CRYS_HASHUserContext_t  *ContextID_ptr,
	DxUint8_t                 *DataIn_ptr,
	DxUint32_t                 DataInSize);
extern CRYSError_t __CC_CRYS_HASH_Finish(
	CRYS_HASHUserContext_t  *ContextID_ptr ,
	CRYS_HASH_Result_t       HashResultBuff);
extern CRYSError_t  __CC_CRYS_HASH_Free(
	CRYS_HASHUserContext_t  *ContextID_ptr);
extern CRYSError_t __CC_CRYS_HASH(
	CRYS_HASH_OperationMode_t  OperationMode,
	DxUint8_t                   *DataIn_ptr,
	DxUint32_t                   DataSize,
	CRYS_HASH_Result_t         HashResultBuff);
extern CRYSError_t __CC_CRYS_HMAC_Init(
	CRYS_HMACUserContext_t     *ContextID_ptr,
	CRYS_HASH_OperationMode_t  OperationMode,
	DxUint8_t                    *key_ptr,
	DxUint16_t                    keySize);
extern CRYSError_t __CC_CRYS_HMAC_Update(
	CRYS_HMACUserContext_t  *ContextID_ptr,
	DxUint8_t                 *DataIn_ptr,
	DxUint32_t                 DataInSize);
extern CRYSError_t __CC_CRYS_HMAC_Finish(
	CRYS_HMACUserContext_t  *ContextID_ptr ,
	CRYS_HASH_Result_t       HmacResultBuff);
extern CRYSError_t  __CC_CRYS_HMAC_Free(
	CRYS_HMACUserContext_t  *ContextID_ptr);
extern CRYSError_t __CC_CRYS_HMAC(
	CRYS_HASH_OperationMode_t  OperationMode,
	DxUint8_t                    *key_ptr,
	DxUint16_t                    keySize,
	DxUint8_t                    *DataIn_ptr,
	DxUint32_t                    DataSize,
	CRYS_HASH_Result_t          HmacResultBuff);
extern CRYSError_t  __CC_CRYS_DES_Init(
	CRYS_DESUserContext_t    *ContextID_ptr,
	CRYS_DES_Iv_t            IV_ptr,
	CRYS_DES_Key_t           *Key_ptr,
	CRYS_DES_NumOfKeys_t     NumOfKeys,
	CRYS_DES_EncryptMode_t   EncryptDecryptFlag,
	CRYS_DES_OperationMode_t OperationMode);
extern CRYSError_t  __CC_CRYS_DES_Block(
	CRYS_DESUserContext_t       *ContextID_ptr, /* in */
	DxUint8_t                     *DataIn_ptr,    /* in */
	DxUint32_t                    DataInSize,     /* in */
	DxUint8_t                     *DataOut_ptr);  /* in/out */
extern CRYSError_t  __CC_CRYS_DES_Free(
	CRYS_DESUserContext_t  *ContextID_ptr);
extern CRYSError_t __CC_CRYS_AES_SetIv(
	CRYS_AESUserContext_t   *ContextID_ptr,
	DxUint8_t               *iv_ptr);
extern CRYSError_t  __CC_CRYS_DES(
	CRYS_DES_Iv_t             IV_ptr,                 /* in */
	CRYS_DES_Key_t           *Key_ptr,                /* in */
	CRYS_DES_NumOfKeys_t      NumOfKeys,              /* in */
	CRYS_DES_EncryptMode_t    EncryptDecryptFlag,     /* in */
	CRYS_DES_OperationMode_t  OperationMode,          /* in */
	DxUint8_t                *DataIn_ptr,             /* in */
	DxUint32_t                DataInSize,             /* in */
	DxUint8_t                *DataOut_ptr);           /* out */
extern CRYSError_t  __CC_CRYS_AES(
	CRYS_AES_IvCounter_t       IVCounter_ptr,
	CRYS_AES_Key_t             Key_ptr,
	CRYS_AES_KeySize_t         KeySize,
	CRYS_AES_EncryptMode_t     EncryptDecryptFlag,
	CRYS_AES_OperationMode_t   OperationMode,
	DxUint8_t                  *DataIn_ptr,
	DxUint32_t                 DataInSize,
	DxUint8_t                  *DataOut_ptr);
extern CRYSError_t  __CC_CRYS_AES_Init(
	CRYS_AESUserContext_t    *ContextID_ptr,
	CRYS_AES_IvCounter_t     IVCounter_ptr,
	CRYS_AES_Key_t           Key_ptr,
	CRYS_AES_KeySize_t       KeySizeID,
	CRYS_AES_EncryptMode_t   EncryptDecryptFlag,
	CRYS_AES_OperationMode_t OperationMode);
extern CRYSError_t  __CC_CRYS_AES_Block(
	CRYS_AESUserContext_t   *ContextID_ptr,
	DxUint8_t               *DataIn_ptr,
	DxUint32_t               DataInSize,
	DxUint8_t               *DataOut_ptr);
extern CRYSError_t  __CC_CRYS_AES_Finish(
	CRYS_AESUserContext_t   *ContextID_ptr,
	DxUint8_t               *DataIn_ptr,
	DxUint32_t               DataInSize,
	DxUint8_t               *DataOut_ptr);
extern CRYSError_t  __CC_CRYS_AESCCM(
	CRYS_AES_EncryptMode_t       EncrDecrMode,   /*CCM encrypt-decrypt mode*/
	CRYS_AESCCM_Key_t            CCM_Key,        /*AES-CCM key*/
	CRYS_AESCCM_KeySize_t        KeySizeId,      /*Key size ID*/
	DxUint8_t                   *N_ptr,          /*Nonce*/
	DxUint8_t                    SizeOfN,        /*size of N buffer*/
	DxUint8_t                   *ADataIn_ptr,    /*input data pointer*/
	DxUint32_t                   ADataInSize,    /*input data size*/
	DxUint8_t                   *TextDataIn_ptr, /*input data pointer*/
	DxUint32_t                   TextDataInSize, /*input data size*/
	DxUint8_t                   *TextDataOut_ptr,/*output data pointer*/
	DxUint8_t                    SizeOfT,        /*size of CCM-MAC (T) */
	CRYS_AESCCM_Mac_Res_t Mac_Res);
extern CRYSError_t __CC_CRYS_AESCCM_Init(
	CRYS_AESCCM_UserContext_t *ContextID_ptr,
	CRYS_AES_EncryptMode_t EncrDecrMode,
	CRYS_AESCCM_Key_t CCM_Key,
	CRYS_AESCCM_KeySize_t KeySizeId,
	DxUint32_t AdataSize,
	DxUint32_t TextSize,
	DxUint8_t *N_ptr,
	DxUint8_t SizeOfN,
	DxUint8_t SizeOfT);
extern CRYSError_t __CC_CRYS_AESCCM_BlockAdata(
	CRYS_AESCCM_UserContext_t *ContextID_ptr,
	DxUint8_t *DataIn_ptr,
	DxUint32_t DataInSize);
extern CRYSError_t __CC_CRYS_AESCCM_BlockTextData(
	CRYS_AESCCM_UserContext_t *ContextID_ptr,
	DxUint8_t *DataIn_ptr,
	DxUint32_t DataInSize,
	DxUint8_t *DataOut_ptr);
extern CRYSError_t __CC_CRYS_AESCCM_Finish(
	CRYS_AESCCM_UserContext_t *ContextID_ptr,
	DxUint8_t *DataIn_ptr,
	DxUint32_t DataInSize,
	DxUint8_t *DataOut_ptr,
	CRYS_AESCCM_Mac_Res_t MacRes,
	DxUint8_t *SizeOfT);
extern CRYSError_t  __CC_CRYS_KDF_KeyDerivFunc(
	DxUint8_t                *ZZSecret_ptr,
	DxUint32_t                ZZSecretSize,
	CRYS_KDF_OtherInfo_t     *OtherInfo_ptr,
	CRYS_KDF_HASH_OpMode_t    KDFhashMode,
	CRYS_KDF_DerivFuncMode_t  derivation_mode,
	DxUint8_t                *KeyingData_ptr,
	DxUint32_t                KeyingDataSizeBytes);
extern DxUTILError_t __CC_DX_UTIL_CmacDeriveKey(
	DX_CRYPTO_KEY_TYPE_t    aesKeyType,
	DxUint8_t                    *pDataIn,
	DxUint32_t             DataInSize,
	DxUint8_t            *pCmacResult);
extern DxUTILError_t __CC_DX_UTIL_CmacDeriveKey2(
	DX_CRYPTO_KEY_TYPE_t    aesKeyType,
	DxUint8_t                    *pDataIn,
	DxUint32_t             DataInSize,
	DxUint8_t            *pCmacResult);
extern CRYSError_t __CC_CRYS_DH_PKCS3_GeneratePubPrv(
	DxUint8_t *Generator_ptr,
	DxUint16_t GeneratorSize,
	DxUint8_t *Prime_ptr,
	DxUint16_t PrimeSize,
	DxUint16_t L,
	CRYS_DHUserPubKey_t *tmpPubKey_ptr,
	CRYS_DHPrimeData_t  *tmpPrimeData_ptr,
	DxUint8_t *ClientPrvKey_ptr,
	DxUint16_t *ClientPrvKeySize_ptr,
	DxUint8_t *ClientPub1_ptr,
	DxUint16_t *ClientPubSize_ptr);
extern CRYSError_t __CC_CRYS_DH_ANSI_X942_GeneratePubPrv(
	DxUint8_t *Generator_ptr,
	DxUint16_t GeneratorSize,
	DxUint8_t *Prime_ptr,
	DxUint16_t PrimeSize,
	DxUint8_t *Q_ptr,
	DxUint16_t QSize,
	CRYS_DHUserPubKey_t *tmpPubKey_ptr,
	CRYS_DHPrimeData_t  *tmpPrimeData_ptr,
	DxUint8_t *ClientPrvKey_ptr,
	DxUint16_t *ClientPrvKeySize_ptr,
	DxUint8_t *ClientPub1_ptr,
	DxUint16_t *ClientPubSize_ptr);
extern CRYSError_t __CC_CRYS_DH_GetSecretKey(
	DxUint8_t *ClientPrvKey_ptr,
	DxUint16_t ClientPrvKeySize,
	DxUint8_t *ServerPubKey_ptr,
	DxUint16_t ServerPubKeySize,
	DxUint8_t *Prime_ptr,
	DxUint16_t PrimeSize,
	CRYS_DHUserPubKey_t *tmpPubKey_ptr,
	CRYS_DHPrimeData_t  *tmpPrimeData_ptr,
	DxUint8_t *SecretKey_ptr,
	DxUint16_t *SecretKeySize_ptr);
extern CRYSError_t __CC_CRYS_DH_X942_GetSecretData(
	DxUint8_t                  *ClientPrvKey_ptr,
	DxUint16_t                  ClientPrvKeySize,
	DxUint8_t                  *ServerPubKey_ptr,
	DxUint16_t                  ServerPubKeySize,
	DxUint8_t                  *Prime_ptr,
	DxUint16_t                  PrimeSize,
	CRYS_DH_OtherInfo_t        *otherInfo_ptr,
	CRYS_DH_HASH_OpMode_t       hashMode,
	CRYS_DH_DerivationFunc_Mode DerivFunc_mode,
	CRYS_DH_Temp_t             *tmpBuff_ptr,
	DxUint8_t                  *SecretKeyingData_ptr,
	DxUint16_t                  SecretKeyingDataSize);
extern CRYSError_t __CC_CRYS_DH_X942_HybridGetSecretData(
	DxUint8_t            *ClientPrvKey_ptr1,
	DxUint16_t            ClientPrvKeySize1,
	DxUint8_t            *ClientPrvKey_ptr2,
	DxUint16_t            ClientPrvKeySize2,
	DxUint8_t            *ServerPubKey_ptr1,
	DxUint16_t            ServerPubKeySize1,
	DxUint8_t            *ServerPubKey_ptr2,
	DxUint16_t            ServerPubKeySize2,
	DxUint8_t            *Prime_ptr,
	DxUint16_t            PrimeSize,
	CRYS_DH_OtherInfo_t  *otherInfo_ptr,
	CRYS_DH_HASH_OpMode_t hashMode,
	CRYS_DH_DerivationFunc_Mode DerivFunc_mode,
	CRYS_DH_HybrTemp_t   *tmpDhHybr_ptr,
	DxUint8_t            *SecretKeyingData_ptr,
	DxUint16_t            SecretKeyingDataSize);
extern CRYSError_t __CC_CRYS_RND_Instantiation(
	CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr/*in/out*/);
extern CRYSError_t __CC_CRYS_RND_UnInstantiation(void);
extern CRYSError_t __CC_CRYS_RND_Reseeding(
	CRYS_RND_WorkBuff_t  *rndWorkBuff_ptr/*in/out*/);
extern CRYSError_t __CC_CRYS_RND_GenerateVector(
	DxUint16_t   outSizeBytes, /*in*/
	DxUint8_t   *out_ptr);     /*out*/
extern CRYSError_t __CC_CRYS_RND_GenerateVectorInRange(
	DxUint32_t  rndSizeInBits, DxUint8_t  *maxVect_ptr,
	DxUint8_t  *rndVect_ptr);
extern CRYSError_t __CC_CRYS_RND_AddAdditionalInput(
	DxUint8_t *AdditonalInput_ptr,
	DxUint16_t AdditonalInputSize);
extern CRYSError_t __CC_CRYS_RND_EnterKatMode(
	DxUint8_t            *entrData_ptr,
	DxUint32_t            entrSize,
	DxUint8_t            *nonce_ptr,
	DxUint32_t            nonceSize,
	CRYS_RND_WorkBuff_t  *workBuff_ptr/*out*/);
extern void __CC_CRYS_RND_DisableKatMode(void);
extern CRYSError_t __CC_CRYS_RSA_Build_PubKey(
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	DxUint8_t *Exponent_ptr,
	DxUint16_t ExponentSize,
	DxUint8_t *Modulus_ptr,
	DxUint16_t ModulusSize);

extern CRYSError_t __CC_CRYS_RSA_KG_GenerateKeyPair(
	DxUint8_t             *pubExp_ptr,
	DxUint16_t             pubExpSizeInBytes,
	DxUint32_t             keySize,
	CRYS_RSAUserPrivKey_t *userPrivKey_ptr,
	CRYS_RSAUserPubKey_t  *userPubKey_ptr,
	CRYS_RSAKGData_t      *keyGenData_ptr);
extern CRYSError_t __CC_CRYS_RSA_KG_GenerateKeyPairCRT(
	DxUint8_t             *pubExp_ptr,
	DxUint16_t             pubExpSizeInBytes,
	DxUint32_t             keySize,
	CRYS_RSAUserPrivKey_t *userPrivKey_ptr,
	CRYS_RSAUserPubKey_t  *userPubKey_ptr,
	CRYS_RSAKGData_t      *keyGenData_ptr);

extern CRYSError_t __CC_CRYS_RSA_Build_PrivKey(
	CRYS_RSAUserPrivKey_t   *UserPrivKey_ptr,
	DxUint8_t               *PrivExponent_ptr,
	DxUint16_t               PrivExponentSize,
	DxUint8_t               *PubExponent_ptr,
	DxUint16_t               PubExponentSize,
	DxUint8_t               *Modulus_ptr,
	DxUint16_t               ModulusSize);
extern CRYSError_t __CC_CRYS_RSA_Build_PrivKeyCRT(
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	DxUint8_t *P_ptr,
	DxUint16_t PSize,
	DxUint8_t *Q_ptr,
	DxUint16_t QSize,
	DxUint8_t *dP_ptr,
	DxUint16_t dPSize,
	DxUint8_t *dQ_ptr,
	DxUint16_t dQSize,
	DxUint8_t *qInv_ptr,
	DxUint16_t qInvSize);
extern CRYSError_t __CC_CRYS_RSA_Build_ConvertPrivKeyToCRT(
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	CRYS_RSAConvertKeyToCrtBuffers_t *Buffers_pt);
extern CRYSError_t __CC_CRYS_RSA_Get_PubKey(
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	DxUint8_t  *Exponent_ptr,
	DxUint16_t *ExponentSize_ptr,
	DxUint8_t  *Modulus_ptr,
	DxUint16_t *ModulusSize_ptr);
extern CRYSError_t __CC_CRYS_RSA_Get_ModSizeFromPubKey(
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	DxUint16_t *ModulusSize_ptr);
extern CRYSError_t __CC_CRYS_RSA_Get_PrivKeyCRT(
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	DxUint8_t *P_ptr, DxUint16_t *PSize_ptr,
	DxUint8_t *Q_ptr, DxUint16_t *QSize_ptr,
	DxUint8_t *dP_ptr, DxUint16_t *dPSize_ptr,
	DxUint8_t *dQ_ptr, DxUint16_t *dQSize_ptr,
	DxUint8_t *qInv_ptr,
	DxUint16_t *qInvSize_ptr);
extern CRYSError_t __CC_CRYS_RSA_Get_PrivKeyModulus(
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	DxUint8_t *N_ptr,
	DxUint16_t *NSize_ptr);
extern CRYSError_t __CC__DX_RSA_SCHEMES_Encrypt(
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	CRYS_RSAPrimeData_t  *PrimeData_ptr,
	CRYS_RSA_HASH_OpMode_t hashFunc,
	DxUint8_t *L,
	DxUint16_t Llen,
	CRYS_PKCS1_MGF_t MGF,
	DxUint8_t     *DataIn_ptr,
	DxUint16_t     DataInSize,
	DxUint8_t     *Output_ptr,
	CRYS_PKCS1_version PKCS1_ver);
extern CRYSError_t __CC__DX_RSA_SCHEMES_Decrypt(
	CRYS_RSAUserPrivKey_t  *UserPrivKey_ptr,
	CRYS_RSAPrimeData_t    *PrimeData_ptr,
	CRYS_RSA_HASH_OpMode_t  hashFunc,
	DxUint8_t              *L,
	DxUint16_t              Llen,
	CRYS_PKCS1_MGF_t        MGF,
	DxUint8_t              *DataIn_ptr,
	DxUint16_t              DataInSize,
	DxUint8_t              *Output_ptr,
	DxUint16_t             *OutputSize_ptr,
	CRYS_PKCS1_version      PKCS1_ver);
extern CRYSError_t __CC__DX_RSA_Sign(
	CRYS_RSAPrivUserContext_t *UserContext_ptr,
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	CRYS_RSA_HASH_OpMode_t hashFunc,
	CRYS_PKCS1_MGF_t MGF,
	DxUint16_t SaltLen,
	DxUint8_t     *DataIn_ptr,
	DxUint32_t     DataInSize,
	DxUint8_t     *Output_ptr,
	DxUint16_t    *OutputSize_ptr,
	CRYS_PKCS1_version PKCS1_ver);

extern CRYSError_t __CC__DX_RSA_Verify(
	CRYS_RSAPubUserContext_t *UserContext_ptr,
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	CRYS_RSA_HASH_OpMode_t hashFunc,
	CRYS_PKCS1_MGF_t MGF,
	DxUint16_t SaltLen,
	DxUint8_t     *DataIn_ptr,
	DxUint32_t     DataInSize,
	DxUint8_t     *Sig_ptr,
	CRYS_PKCS1_version PKCS1_ver);
extern CRYSError_t __CC_CRYS_RSA_PRIM_Encrypt(
	CRYS_RSAUserPubKey_t *UserPubKey_ptr,
	CRYS_RSAPrimeData_t  *PrimeData_ptr,
	DxUint8_t              *Data_ptr,
	DxUint16_t              DataSize,
	DxUint8_t              *Output_ptr);
extern CRYSError_t __CC_CRYS_RSA_PRIM_Decrypt(
	CRYS_RSAUserPrivKey_t *UserPrivKey_ptr,
	CRYS_RSAPrimeData_t   *PrimeData_ptr,
	DxUint8_t     *Data_ptr,
	DxUint16_t     DataSize,
	DxUint8_t     *Output_ptr);
extern DxUTILError_t __DX_UTIL_OemAssetUnpack(
	DX_UTIL_OemKey_t        pOemKey, DxUint32_t      assetId,
	DxUint8_t           *pAssetPackage, DxUint32_t       assetPackageLen,
	DxUint8_t           *pAssetData, DxUint32_t          *pAssetDataLen,
	DxUint32_t             *pUserData);
//ecc
extern CRYSError_t __CC_CRYS_ECPKI_BuildPublKey(
	CRYS_ECPKI_DomainID_t        DomainID,          /*in*/
	DxUint8_t            *PublKeyIn_ptr,       /*in*/
	DxUint32_t                   PublKeySizeInBytes,  /*in*/
	CRYS_ECPKI_UserPublKey_t    *UserPublKey_ptr          /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_BuildPublKeyPartlyCheck(
	CRYS_ECPKI_DomainID_t        DomainID,          /*in*/
	DxUint8_t            *PublKeyIn_ptr,       /*in*/
	DxUint32_t                   PublKeySizeInBytes,  /*in*/
	CRYS_ECPKI_UserPublKey_t    *UserPublKey_ptr,     /*out*/
	CRYS_ECPKI_BUILD_TempData_t *TempBuff_ptr         /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_BuildPublKeyFullCheck(
	CRYS_ECPKI_DomainID_t        DomainID,          /*in*/
	DxUint8_t            *PublKeyIn_ptr,       /*in*/
	DxUint32_t                   PublKeySizeInBytes,  /*in*/
	CRYS_ECPKI_UserPublKey_t    *UserPublKey_ptr,     /*out*/
	CRYS_ECPKI_BUILD_TempData_t *TempBuff_ptr         /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_BuildPrivKey(
	CRYS_ECPKI_DomainID_t      DomainID,      /*in */
	DxUint8_t       *PrivKeyIn_ptr,     /*in*/
	DxUint32_t                 PrivKeySizeInBytes,/*in*/
	CRYS_ECPKI_UserPrivKey_t  *UserPrivKey_ptr    /*out*/);
extern CRYSError_t __CC_CRYS_ECDH_SVDP_DH(
	CRYS_ECPKI_UserPublKey_t *PartnerPublKey_ptr, /*in*/
	CRYS_ECPKI_UserPrivKey_t *UserPrivKey_ptr,           /*in*/
	DxUint8_t         *SharedSecretValue_ptr,     /*out*/
	DxUint32_t               *SharedSecrValSize_ptr,     /*in/out*/
	CRYS_ECDH_TempData_t     *TempBuff_ptr               /*in*/);
extern CRYSError_t __CC_CRYS_ECDSA_Sign(
	CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr,     /*in/out*/
	CRYS_ECPKI_UserPrivKey_t      *SignerPrivKey_ptr,       /*in*/
	CRYS_ECPKI_HASH_OpMode_t       HashMode,                /*in*/
	DxUint8_t                     *MessageDataIn_ptr,       /*in*/
	DxUint32_t                     MessageSizeInBytes,      /*in*/
	DxUint8_t                     *SignatureOut_ptr,        /*out*/
	DxUint32_t                    *SignatureOutSize_ptr     /*in*/);
extern CRYSError_t  __CC_CRYS_ECDSA_Verify(CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in/out*/
		CRYS_ECPKI_UserPublKey_t       *UserPublKey_ptr,        /*in*/
		CRYS_ECPKI_HASH_OpMode_t        HashMode,               /*in*/
		DxUint8_t                      *SignatureIn_ptr,        /*in*/
		DxUint32_t                      SignatureSizeBytes,     /*in*/
		DxUint8_t                      *MessageDataIn_ptr,      /*in*/
		DxUint32_t                      MessageSizeInBytes      /*in*/);

extern CRYSError_t __CC_CRYS_ECPKI_ELGAMAL_Encrypt(
	CRYS_ECPKI_UserPublKey_t       *ReceiverUserPublKey_ptr, /*in*/
	DxUint8_t                      *MessageIn_ptr,           /*in*/
	DxUint32_t                      MessageInSizeBytes,      /*in*/
	DxUint8_t                      *EncrMessageOut_ptr,      /*out*/
	DxUint32_t                     *EncrMessOutSize_ptr,     /*in,out*/
	DxInt8_t                        IsEphemerKeyInternal,    /*in*/
	DxUint8_t                      *EphemerPrivKeyIn_ptr,    /*in*/
	DxUint32_t                      EphemerPrivKeySizeBytes, /*in*/
	CRYS_EC_ELGAMAL_TempData_t     *TempData_ptr             /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_ELGAMAL_Decrypt(
	CRYS_ECPKI_UserPrivKey_t   *ReceiverUserPrivKey_ptr,  /*in*/
	DxUint8_t                  *EncrMessageIn_ptr,        /*in*/
	DxUint32_t                  EncrMessageSizeInBytes,   /*in*/
	DxUint8_t                  *DecrMessageOut_ptr,       /*out*/
	DxUint32_t                 *DecrMessageOutSize_ptr,   /*in*/
	CRYS_EC_ELGAMAL_TempData_t *TempData_ptr              /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_GenKeyPair(
	CRYS_ECPKI_DomainID_t       DomainID,        /*in*/
	CRYS_ECPKI_UserPrivKey_t   *UserPrivKey_ptr,    /*out*/
	CRYS_ECPKI_UserPublKey_t   *UserPublKey_ptr,    /*out*/
	CRYS_ECPKI_KG_TempData_t   *TempData_ptr        /*in*/);
extern CRYSError_t __CC_CRYS_ECDSA_SignUpdate(
	CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr,  /*in/out*/
	DxUint8_t                     *MessageDataIn_ptr,    /* in */
	DxUint32_t                      DataInSize            /* in */);
extern CRYSError_t __CC_CRYS_ECDSA_SignFinish(
	CRYS_ECDSA_SignUserContext_t   *SignUserContext_ptr, /*in*/
	DxUint8_t                      *SignatureOut_ptr,    /*out*/
	DxUint32_t                     *SignatureOutSize_ptr/*in/out*/);
extern CRYSError_t __CC_CRYS_ECDSA_VerifyInit(
	CRYS_ECDSA_VerifyUserContext_t  *VerifyUserContext_ptr, /*in/out*/
	CRYS_ECPKI_UserPublKey_t        *SignerPublKey_ptr,     /*in*/
	CRYS_ECPKI_HASH_OpMode_t         HashMode               /*in*/);
extern CRYSError_t __CC_CRYS_ECDSA_VerifyUpdate(
	CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in/out*/
	DxUint8_t                      *MessageDataIn_ptr,     /* in */
	DxUint32_t                      DataInSize             /* in */);
extern  CRYSError_t __CC_CRYS_ECDSA_VerifyFinish(
	CRYS_ECDSA_VerifyUserContext_t *VerifyUserContext_ptr, /*in*/
	DxUint8_t                      *SignatureIn_ptr,       /*in*/
	DxUint32_t                      SignatureSizeBytes    /*in*/);
extern CRYSError_t __CC_CRYS_ECDSA_SignInit(
	CRYS_ECDSA_SignUserContext_t  *SignUserContext_ptr, /*in/out*/
	CRYS_ECPKI_UserPrivKey_t      *SignerPrivKey_ptr,   /*in*/
	CRYS_ECPKI_HASH_OpMode_t       HashMode             /*in*/);
extern CRYSError_t __CC_CRYS_ECPKI_ExportPublKey(
	CRYS_ECPKI_UserPublKey_t      *UserPublKey_ptr,       /*in*/
	CRYS_ECPKI_PointCompression_t  Compression,           /*in*/
	DxUint8_t                    *ExternPublKey_ptr,     /*in*/
	DxUint32_t                    *PublKeySizeInBytes_ptr /*in/out*/);


extern int __get_provision_key(DX_UTIL_OemKey_t pOemKey);
extern void __write_to_hdmi_sram(const BYTE *hdcp_key,		   const unsigned int hdcp_key_len);

#define __CC_CRYS_RSA_OAEP_Encrypt(UserPubKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr)\
    __CC__DX_RSA_SCHEMES_Encrypt(UserPubKey_ptr,PrimeData_ptr,HashMode/*CRYS_RSA_HASH_SHA1_mode*/,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,CRYS_PKCS1_VER21)

#define __CC_CRYS_RSA_PKCS1v15_Encrypt(UserPubKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr)\
    __CC__DX_RSA_SCHEMES_Encrypt(UserPubKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,DX_NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr,DataInSize,Output_ptr,CRYS_PKCS1_VER15)

#define __CC_CRYS_RSA_OAEP_Decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_SCHEMES_Decrypt(UserPrivKey_ptr,PrimeData_ptr,HashMode/*CRYS_RSA_HASH_SHA1_mode*/,L,Llen,MGF,Data_ptr,DataSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)

#define __CC_CRYS_RSA_PKCS1v15_Decrypt(UserPrivKey_ptr,PrimeData_ptr,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_SCHEMES_Decrypt(UserPrivKey_ptr,PrimeData_ptr,CRYS_RSA_HASH_NO_HASH_mode,DX_NULL,0,CRYS_PKCS1_NO_MGF,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER15)

#define __CC_CRYS_RSA_PKCS1v15_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(hashFunc),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),(DataInSize),(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)

#define __CC_CRYS_RSA_PKCS1v15_SHA1_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA1_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_MD5_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),CRYS_RSA_After_MD5_mode,CRYS_PKCS1_NO_MGF,0,(DataIn_ptr),CRYS_HASH_MD5_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA224_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA224_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA256_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA256_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA384_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA384_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA512_Sign(UserContext_ptr,UserPrivKey_ptr,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign((UserContext_ptr),(UserPrivKey_ptr),(CRYS_RSA_After_SHA512_mode),(CRYS_PKCS1_NO_MGF),0,(DataIn_ptr),CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,(Output_ptr),(OutputSize_ptr),CRYS_PKCS1_VER15)

#define __CC_CRYS_RSA_PSS_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA1_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA1_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA224_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA224_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA256_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA256_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA384_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA384_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA512_Sign(UserContext_ptr,UserPrivKey_ptr,MGF,SaltLen,DataIn_ptr,Output_ptr,OutputSize_ptr)\
    __CC__DX_RSA_Sign(UserContext_ptr,UserPrivKey_ptr,CRYS_RSA_After_SHA512_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Output_ptr,OutputSize_ptr,CRYS_PKCS1_VER21)

#define __CC_CRYS_RSA_PKCS1v15_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,DataIn_ptr,DataInSize,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_MD5_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_MD5_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_MD5_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA1_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA1_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA224_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA224_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA256_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA256_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA384_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA384_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)
#define __CC_CRYS_RSA_PKCS1v15_SHA512_Verify(UserContext_ptr,UserPubKey_ptr,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA512_mode,CRYS_PKCS1_NO_MGF,0,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER15)

#define __CC_CRYS_RSA_PSS_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,hashFunc,MGF,SaltLen,DataIn_ptr,DataInSize,Sig_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA1_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA1_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA1_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA224_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA224_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA224_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA256_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA256_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA256_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA384_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA384_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA384_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)
#define __CC_CRYS_RSA_PSS_SHA512_Verify(UserContext_ptr,UserPubKey_ptr,MGF,SaltLen,DataIn_ptr,Sig_ptr)\
    __CC__DX_RSA_Verify(UserContext_ptr,UserPubKey_ptr,CRYS_RSA_After_SHA512_mode,MGF,SaltLen,DataIn_ptr,CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES,Sig_ptr,CRYS_PKCS1_VER21)
#endif
#if defined(TESTSUITE_RTOSck_UT) || defined(TESTSUITE_RTOSck_PT)||defined(TESTSUITE_RTOSck_IT)  //set no mem access isolation when run testsuite
extern int __CC_syscall_multiargs_test(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15,                           int a16, int a17, int a18, int a19);
#endif
//tzasc
extern void __read_tzc_rgn_cfg(unsigned int u32TzcBaseAddr, unsigned char u8Rgn, unsigned int *attribute, unsigned int *base_addr);
extern void __config_tzc_rgn(unsigned int u32TzcBaseAddr, unsigned char u8Rgn, unsigned int u32AttrVal, unsigned int u32BaseAddrVal);
#ifdef TEE_SUPPORT_RPMB_DEVICE
struct rpmb_atf_info {
	UINT8  stuff[0x200];
	UINT32 ret;
	UINT32 start_blk;
	UINT32 total_blk;
	UINT32 data_len;
	UINT32 data_addr;
	UINT32 mdt;
	// add reserved to ensure end address aligned for 64
	UINT8 reserved[40];
};

extern unsigned int __bsp_rpmbinit_status_write(unsigned int size);
extern unsigned int __bsp_rpmbinit_status_read(void);
extern unsigned int __rpmb_reset_capable();
extern void __rpmb_smc_switch(unsigned int phy_addr);
extern unsigned int __rpmb_partition_info_write(unsigned int start_blk, unsigned int total_blk, unsigned int mdt);
extern unsigned int __rpmb_partition_info_read(unsigned int *start_blk, unsigned int *total_blk);
extern unsigned int __rpmb_rootkey_info_write(const char *data, unsigned int len);
extern unsigned int __rpmb_rootkey_info_read(char *data, unsigned int *len);
extern unsigned int __rpmb_mdt_info_read();
#endif //TEE_SUPPORT_RPMB_DEVICE

int __init_tui_sdriver(void *tui_tp);

int __tee_mutex_init(const char *name, unsigned int size, void **handle);
int __tee_mutex_lock(void *handle);
int __tee_mutex_lock_wait(void *handle);
int __tee_mutex_unlock(void *handle);
void __tee_mutex_release(void *handle);

UINT32 __SRE_ReadRootStatus();
int __SRE_WriteRootStatus(UINT32 status);

#endif
