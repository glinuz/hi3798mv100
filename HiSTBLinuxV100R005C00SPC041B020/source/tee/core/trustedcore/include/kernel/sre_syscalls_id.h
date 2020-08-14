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

#ifndef __SRE_SYSCALL_ID_H_
#define __SRE_SYSCALL_ID_H_

/* Syscall Numbers */

/*console*/
#define SW_SYSCALL_CONSOLE_FLUSH                         0xf000
#define SW_SYSCALL_CONSOLE_PUTC                          0xf001
#define SW_SYSCALL_CONSOLE_REINIT                        0xf002
#define SW_SYSCALL_SNPRINTF_PUTC                         0xf003

/*SMC*/
#define SW_SYSCALL_START_TZ                              0xf010
#define SW_SYSCALL_SMC_SWITCH                            0xf011

#define SW_SYSCALL_VIRT_TO_PHYS                          0xf015

/*MMU*/
#define SW_SYSCALL_MAP_TONS                                   0xf020
#define SW_SYSCALL_UNMAP_FROMNS                               0xf021
#define SW_SYSCALL_MAP_FROMNS_PAGE                            0xf022
#define SW_SYSCALL_UNMAP_FROMNS_PAGE                          0xf023
#define SW_SYSCALL_TASK_MAP_FROMNS_PAGE                       0xf024
#define SW_SYSCALL_TASK_UNMAP_FROMNS_PAGE                     0xf025
#define SW_SYSCALL_TASK_MAP_FROM_TASK                         0xf026
#define SW_SYSCALL_CHECK_SECURE_ADDRESS			      0xf027
#define SW_SYSCALL_MMAP                                  0xf028
#define SW_SYSCALL_UNMAP                                 0xf029

/*Task*/
#define SW_SYSCALL_TASK_CREATE                              0xf030
#define SW_SYSCALL_TASK_RESUME                              0xf031
#define SW_SYSCALL_TASK_SUSPEND                            0xf032
#define SW_SYSCALL_TASK_DELETE                                 0xf033
#define SW_SYSCALL_TASK_DELAY                                 0xf034
#define SW_SYSCALL_TASK_LOCK                              0xf035
#define SW_SYSCALL_TASK_UNLOCK                              0xf036
#define SW_SYSCALL_TASK_SELF                             0xf037
#define SW_SYSCALL_TASK_IDCHECK                                 0xf038
#define SW_SYSCALL_TASK_STATUSGET                              0xf039
#define SW_SYSCALL_TASK_INFOGET                              0xf03a
#define SW_SYSCALL_TASK_PRIDATAGET                            0xf03b
#define SW_SYSCALL_TASK_PRIDATASET                                 0xf03c
#define SW_SYSCALL_TASK_PRIORITYGET                             0xf03d
#define SW_SYSCALL_TASK_PRIORITYSET                              0xf03e
#define SW_SYSCALL_TASK_YIELD                             0xf03f
#define SW_SYSCALL_TASK_CREATEHOOKADD                                 0xf040
#define SW_SYSCALL_TASK_CREATEHOOKDELETE                              0xf041
#define SW_SYSCALL_TASK_DELETEHOOKADD                              0xf042
#define SW_SYSCALL_TASK_DELETEHOOKDELETE                             0xf043
#define SW_SYSCALL_TASK_SWITCHHOOKADD                                 0xf044
#define SW_SYSCALL_TASK_SWITCHHOOKDELETE                              0xf045
#define SW_SYSCALL_TASK_PENDSEMGET                              0xf046
#define SW_SYSCALL_TASK_MSGNUMGET                             0xf047
#define SW_SYSCALL_TASK_SCHEDULE                             0xf048
#define SW_SYSCALL_TASK_CURRENTMEMINFO                      0xf049
#define SW_SYSCALL_TASK_SELF_NAME                             0xf04a
#define SW_SYSCALL_TASK_PID_CLASS_HANDLE                      0xf04b

/*Timer*/
#define SW_SYSCALL_TIMER_CREATE                            0xf050
#define SW_SYSCALL_TIMER_START                              0xf051
#define SW_SYSCALL_TIMER_DESTORY                          0xf052
#define SW_SYSCALL_TIMER_STOP                                0xf053
#define SW_SYSCALL_TIMER_READSTAMP                      0xf054
#define SW_SYSCALL_TIMER_MSDELAY                          0xf055
#define SW_SYSCALL_TIMER_SDELAY                            0xf056
#define SW_SYSCALL_SRE_TIMERGETEXPIRE                       0xf057
#define SW_SYSCALL_SRE_TIMERCHECK                       0xf058
#define SW_SYSCALL_TASK_DELETE_EX                             0xf059
#define SW_SYSCALL_GET_STARTUP_TIME                      0xf05a

/*Msg*/
#define SW_SYSCALL_MSG_RECEIVE                              0xf060
#define SW_SYSCALL_MSG_QSEND                                0xf061
#define SW_SYSCALL_MSG_ALLOC                                0xf062
#define SW_SYSCALL_MSG_DELETE                               0xf063
#define SW_SYSCALL_MSGQRECV                                 0xf064
#define SW_SYSCALL_MSGSND                                   0xf065
#define SW_SYSCALL_MSGRCV                                   0xf066
#define SW_SYSCALL_MSGRCVA                                  0xf067

/*Mem*/
#define SW_SYSCALL_MSG_USAGEGET                             0xf070
#define SW_SYSCALL_TASKMEM_MALLOC                           0xf072
#define SW_SYSCALL_TASKMEM_FREE                             0xf073
#define SW_SYSCALL_TASKMEM_GETORDER                         0xf074
#define SW_SYSCALL_TASKMEM_GETBASE                          0xf075
#define SW_SYSCALL_TASKMEM_GETINTERNALTASKINFO              0xf076
#define SW_SYSCALL_DYNAMIC_LOADELF                          0xf077
#define SW_SYSCALL_RELEASE_DYNAMIC_REGION                   0xf078
#define SW_SYSCALL_MEMALLOC                                 0xf079
#define SW_SYSCALL_MEMFREE                                  0xf07a
#define SW_SYSCALL_MEMALLOCALIGN                            0xf07d
#define SW_SYSCALL_MEMALLOCSHARED                           0xf07e
#define SW_SYSCALL_MEMFREESHARED                            0xf07f



/*Kernel Varibles*/
#define SW_SYSCALL_PULL_KVARIABLES                         0xf080
#define SW_SYSCALL_PUSH_KVARIABLES                         0xf081
#define SW_SYSCALL_PUSH_EXCEPTION_MEM_ADDR                 0xf084
#define SW_SYSCALL_UPDATE_KVARIABLES                       0xf085
#define SW_SYSCALL_PUSH_RDR_MEM_ADDR                       0xf086
#define SW_SYSCALL_GET_LOG_LEVEL                           0xf087
#define SW_SYSCALL_SRE_SETKMROT		0xf088
#define SW_SYSCALL_SRE_GETKMROT		0xf089
/*Cache*/
#define SW_SYSCALL_CACHE_CODEUPDATE                        0xf090
#define SW_SYSCALL_DCACHE_FLUSH                            0xf091
#define SW_SYSCALL_V7_CACHE_FLUSH                            0xf092
#define SW_SYSCALL_V7_CACHE_INVALID                          0xf093
#define SW_SYSCALL_V7_CACHE_FLUSHALL                         0xf094

/*hunt*/
#define SW_SYSCALL_HUNT_BY_NAME                            0xf0a0

/* set build version */
#define SW_SYSCALL_GET_ANDROID_VERSION			0xf0a1
#define SW_SYSCALL_TEST_KLOG_FUNC			0xf0a2

/*swi*/
#define SW_SYSCALL_SWI_CREATE                           0xf0b0
#define SW_SYSCALL_SWI_DELETE                          0xf0b1
#define SW_SYSCALL_SWI_ENABLE                           0xf0b2
#define SW_SYSCALL_SWI_DISABLE                           0xf0b3
#define SW_SYSCALL_SWI_ACTIVATE                           0xf0b4
#define SW_SYSCALL_SWI_INC                           0xf0b5
#define SW_SYSCALL_SWI_DEC                           0xf0b6
#define SW_SYSCALL_SWI_OR                           0xf0b7
#define SW_SYSCALL_SWI_ANDN                           0xf0b8
#define SW_SYSCALL_SWI_VALUEGET                           0xf0b9
#define SW_SYSCALL_SWI_STATUSGET                          0xf0ba
#define SW_SYSCALL_SWI_INFOGET                           0xf0bb
#define SW_SYSCALL_SWI_SELF                           0xf0bc
#define SW_SYSCALL_SWI_PRIORITYGET                           0xf0bd
#define SW_SYSCALL_SWI_MSGNUMGET                           0xf0be

/*sem*/
#define SW_SYSCALL_SEM_CREATE                           0xf0c0
#define SW_SYSCALL_SEM_DELETE                          0xf0c1
#define SW_SYSCALL_SEM_RESET                           0xf0c2
#define SW_SYSCALL_SEM_COUNTGET                           0xf0c3
#define SW_SYSCALL_SEM_PEND                           0xf0c4
#define SW_SYSCALL_SEM_POST                           0xf0c5
#define SW_SYSCALL_SEM_PENDLISTGET                          0xf0c6

/*fsem*/
#define SW_SYSCALL_FSEM_SET                           0xf0d0
#define SW_SYSCALL_FSEM_GET                          0xf0d1
#define SW_SYSCALL_FSEM_PEND                           0xf0d2
#define SW_SYSCALL_FSEM_POST                           0xf0d3

/*hwi*/
#define SW_SYSCALL_HWI_CREATE                           0xf0e0
#define SW_SYSCALL_HWI_DELETE                          0xf0e1
#define SW_SYSCALL_HWI_TRIGGER                           0xf0e2
#define SW_SYSCALL_HWI_CLEARPENDING                           0xf0e3
#define SW_SYSCALL_HWI_DISABLE                           0xf0e4
#define SW_SYSCALL_HWI_ENABLE                           0xf0e5
#define SW_SYSCALL_INT_LOCK                                 0xf0e6
#define SW_SYSCALL_INT_RESTORE                            0xf0e7
#define SW_SYSCALL_INT_UNLOCK                             0xf0e8
#define SW_SYSCALL_INT_GETCPSR                            0xf0e9
#define SW_SYSCALL_HWI_IPCREGISTER                        0xf0ea
#define SW_SYSCALL_HWI_IPCDEREGISTER                      0xf0eb
#define SW_SYSCALL_HWI_IPCCREATEENABLE                    0xf0ec

/*sys*/
#define SW_SYSCALL_SYS_OSTSKEXIT                            0xf0f0
#define SW_SYSCALL_ENCRYPT_INTERNALTASK                     0xf0f1

/*RTC*/
#define SW_SYSCALL_RTC_INIT                             0xf101
#define SW_SYSCALL_GET_RTC_TIME                         0xf102
#define SW_SYSCALL_INIT_RTC_TIME                        0xf103
#define SW_SYSCALL_ADJUST_SYS_TIME                      0xf104
#define SW_SYSCALL_GET_SYS_DATE_TIME                    0xf105
#define SW_SYSCALL_GEN_SYS_DATE_TIME                    0xf106

#define SW_SYSCALL_SWI_TEST                                0xf1a0

/*backtraces*/
#define SW_SYSCALL_DUMP_BACKTRACES              0xf1c0
#define SW_SYSCALL_DUMP_BACKTRACES_TEST         0xf1c1

/* tee mutex */
#define SW_SYSCALL_TEE_MUTEX_INIT               0xf1c8
#define SW_SYSCALL_TEE_MUTEX_LOCK               0xf1c9
#define SW_SYSCALL_TEE_MUTEX_UNLOCK             0xf1ca
#define SW_SYSCALL_TEE_MUTEX_RELEASE            0xf1cb
#define SW_SYSCALL_TEE_MUTEX_LOCK_WAIT          0xf1cc

/* get target product */
#define SW_SYSCALL_GET_TARGET_PRODUCT           0xf1d0

/*cc*/
#ifdef CC_DRIVER
#define SW_SYSCALL_CC_DX_CCLIBINIT              0xc000
#define SW_SYSCALL_CC_CRYS_HASH_INIT            0xc004
#define SW_SYSCALL_CC_CRYS_HASH_UPDATE          0xc008
#define SW_SYSCALL_CC_CRYS_HASH_FINISH          0xc00c
#define SW_SYSCALL_CC_CRYS_HASH_FREE            0xc010
#define SW_SYSCALL_CC_CRYS_HASH         0xc014
#define SW_SYSCALL_CC_CRYS_HMAC_INIT            0xc018
#define SW_SYSCALL_CC_CRYS_HMAC_UPDATE          0xc01c
#define SW_SYSCALL_CC_CRYS_HMAC_FINISH          0xc020
#define SW_SYSCALL_CC_CRYS_HMAC_FREE            0xc024
#define SW_SYSCALL_CC_CRYS_HMAC         0xc028
#define SW_SYSCALL_CC_CRYS_DES_INIT             0xc02c
#define SW_SYSCALL_CC_CRYS_DES_BLOCK            0xc030
#define SW_SYSCALL_CC_CRYS_DES          0xc034
#define SW_SYSCALL_CC_CRYS_AES          0xc036
#define SW_SYSCALL_CC_CRYS_AES_INIT             0xc038
#define SW_SYSCALL_CC_CRYS_AES_BLOCK            0xc03c
#define SW_SYSCALL_CC_CRYS_AES_FINISH           0xc040
#define SW_SYSCALL_CC_CRYS_AESCCM               0xc044
#define SW_SYSCALL_CC_CRYS_AESCCM_INIT          0xc048
#define SW_SYSCALL_CC_CRYS_AESCCM_BLOCKADATA            0xc04c
#define SW_SYSCALL_CC_CRYS_AESCCM_FINISH                0xc050
#define SW_SYSCALL_CC_CRYS_KDF_KEYDERIVFUNC             0xc054
#define SW_SYSCALL_CC_DX_UTIL_CMACDERIVEKEY                0xc058
#define SW_SYSCALL_CC_DX_UTIL_CMACDERIVEKEY2               0xc059
#define SW_SYSCALL_CC_CRYS_RND_INSTANTIATION            0xc05c
#define SW_SYSCALL_CC_CRYS_RND_UNINSTANTIATION          0xc060
#define SW_SYSCALL_CC_CRYS_RND_RESEEDING                0xc064
#define SW_SYSCALL_CC_CRYS_RND_GENERATEVECTOR           0xc068
#define SW_SYSCALL_CC_CRYS_RND_GENERATEVECTORINRANGE            0xc06c
#define SW_SYSCALL_CC_CRYS_RND_ADDADDITIONALINPUT               0xc070
#define SW_SYSCALL_CC_CRYS_RND_ENTERKATMODE             0xc074
#define SW_SYSCALL_CC_CRYS_RND_DISABLEKATMODE           0xc078
#define SW_SYSCALL_CC_CRYS_RSA_BUILD_PUBKEY             0xc07c
#define SW_SYSCALL_CC_CRYS_RSA_BUILD_PRIVKEY            0xc080
#define SW_SYSCALL_CC_CRYS_RSA_BUILD_PRIVKEYCRT         0xc084
#define SW_SYSCALL_CC_CRYS_RSA_BUILD_CONVERTPRIVKEYTOCRT                0xc088
#define SW_SYSCALL_CC_CRYS_RSA_GET_PUBKEY               0xc08c
#define SW_SYSCALL_CC_CRYS_RSA_GET_MODSIZEFROMPUBKEY            0xc090
#define SW_SYSCALL_CC_CRYS_RSA_GET_PRIVKEYCRT           0xc098
#define SW_SYSCALL_CC_CRYS_RSA_GET_PRIVKEYMODULUS               0xc09c
#define SW_SYSCALL_CC__DX_RSA_SCHEMES_ENCRYPT           0xc0a0
#define SW_SYSCALL_CC__DX_RSA_SIGN              0xc0a4
#define SW_SYSCALL_CC__DX_RSA_VERIFY            0xc0a8
#define SW_SYSCALL_CC_CRYS_RSA_PRIM_ENCRYPT             0xc0ac
#define SW_SYSCALL_CC_CRYS_RSA_PRIM_DECRYPT             0xc0b0
#define SW_SYSCALL_CC_CRYS_DES_FREE             0xc0b4
#define SW_SYSCALL_CC_CRYS_RSA_KG_GENERATEKEYPAIRCRT            0xc0b8
#define SW_SYSCALL_CC_CRYS_RSA_KG_GENERATEKEYPAIR           0xc0bc
#define SW_SYSCALL_CC__DX_RSA_SCHEMES_DECRYPT           0xc0c0
#define SW_SYSCALL_CC_CRYS_DH_PKCS3_GeneratePubPrv      0xc0c4
#define SW_SYSCALL_CC_CRYS_DH_ANSI_X942_GeneratePubPrv  0xc0c8
#define SW_SYSCALL_CC_CRYS_DH_GetSecretKey              0xc0cc
#define SW_SYSCALL_CC_CRYS_DH_X942_GetSecretData        0xc0d0
#define SW_SYSCALL_CC_CRYS_DH_X942_HybridGetSecretData  0xc0d4
#define SW_SYSCALL_CC_CRYS_AES_SetIv                    0xc0d8
#define SW_SYSCALL_CC_CRYS_AESCCM_BlockTextData         0xc0dc
#define SW_SYSCALL_DX_UTIL_OEMASSETUNPACK		        0xc0e0
#define SW_SYSCALL_GET_PROVISION_KEY		0xc0e4
#define SW_SYSCALL_WRITE_TO_HDMI_SRAM		0xc0e8
//ecc
#define SW_SYSCALL_CC_CRYS_ECPKI_BuildPublKey            0xc0f3
#define SW_SYSCALL_CC_CRYS_ECPKI_BuildPublKeyPartlyCheck 0xc0f4
#define SW_SYSCALL_CC_CRYS_ECPKI_BuildPublKeyFullCheck   0xc0f5
#define SW_SYSCALL_CC_CRYS_ECPKI_BuildPrivKey            0xc0f6
#define SW_SYSCALL_CC_CRYS_ECPKI_ExportPublKey           0xc0f7
#define SW_SYSCALL_CC_CRYS_ECDH_SVDP_DH                  0xc0f8
#define SW_SYSCALL_CC_CRYS_ECDSA_Sign                    0xc0f9
#define SW_SYSCALL_CC_CRYS_ECDSA_Verify                  0xc0fa
#define SW_SYSCALL_CC_CRYS_ECPKI_ELGAMAL_Encrypt         0xc0fb
#define SW_SYSCALL_CC_CRYS_ECPKI_ELGAMAL_Decrypt         0xc0fc
#define SW_SYSCALL_CC_CRYS_ECPKI_GenKeyPair              0xc0fd
#define SW_SYSCALL_CC_CRYS_ECDSA_SignInit                0xc0fe
#define SW_SYSCALL_CC_CRYS_ECDSA_SignUpdate              0xc0ff
#define SW_SYSCALL_CC_CRYS_ECDSA_SignFinish              0xc100
#define SW_SYSCALL_CC_CRYS_ECDSA_VerifyInit              0xc101
#define SW_SYSCALL_CC_CRYS_ECDSA_VerifyUpdate            0xc102
#define SW_SYSCALL_CC_CRYS_ECDSA_VerifyFinish            0xc103
#endif//CC_DRIVER

/*rpmb init status*/
#ifdef TEE_SUPPORT_RPMB_DEVICE
#define SW_SYSCALL_BSP_RPMBINIT_STATUS_WRITE        0xc200
#define SW_SYSCALL_BSP_RPMBINIT_STATUS_READ         0xc201
#define SW_SYSCALL_RPMB_SMC_SWITCH         0xc202
#define SW_SYSCALL_RPMB_PARTITION_WRITE 0xc203
#define SW_SYSCALL_RPMB_PARTITION_READ 0xc204
#define SW_SYSCALL_RPMB_KEYINFO_WRITE 0xc205
#define SW_SYSCALL_RPMB_KEYINFO_READ 0xc206
#define SW_SYSCALL_RPMB_RESET_CAPABLE 0xc207
#define SW_SYSCALL_RPMB_MDT_READ 0xc208
#endif //TEE_SUPPORT_RPMB_DEVICE

#define SW_SYSCALL_CC_SYSCALL_MULTIARGS_TEST            0xd000

/*tzasc*/
#define SW_SYSCALL_TZASC_INIT                           0xe100
#define SW_SYSCALL_TZASC_TERMINATE                      0xe101
#define SW_SYSCALL_TZASC_ISSECURE                       0xe102

/*gic ipi notify*/
#define SW_IPI_NOTIFY                                   0xe002

/*sec boot*/
#define SW_COPY_SOC_DATA_TYPE                           0xe003
#define SW_VERIFY_SOC_DATA_TYPE                         0xe004
#define SW_SOC_IMAGE_RESET                              0xe005
#define SW_SOC_IMAGE_SET                                0xe006
#define SW_SOC_GET_VRL_ADDR                             0xe007
/*anti root*/
#define SW_SYSCALL_ROOT_READ     0xe401
#define SW_SYSCALL_ROOT_WRITE    0xe402
#endif
