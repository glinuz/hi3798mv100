/*                                                                           */
/* Author:                                                                   */
/*                                                                           */
/* Version: 1.0                                                              */
/*                                                                           */
/* Date: 2015-05-06                                                          */
/*                                                                           */
/* Description:                                                              */
/*     Permission control definitions                                        */
/* Others:                                                                   */
/*                                                                           */
/*****************************************************************************/

#ifndef __SRE_ACCESS_CONTROL_ID_H_
#define __SRE_ACCESS_CONTROL_ID_H_

#include "sre_typedef.h"

#define HANDLE_SYSCALL(swi_id) switch(swi_id)

#define SYSCALL_PERMISSION(swi_id, current_permissions, permission) \
    case swi_id: { \
        if((permission & current_permissions) == permission) {
#define SYSCALL_END   break;  } \
            else { regs->r0 = OS_ERROR; \
                uart_printf_func("[ERROR!!!] permission denied to access swi_id 0x%x, please check sre_syscalls_id.h to get more info\n", swi_id); \
                break; } }


/* General TA group, all user space apps are allowed */
#define GENERAL_GROUP_PERMISSION       0x00000LL
/* Console initialization */
#define CONSOLE_GROUP_PERMISSION       0x00001LL
/* SMC call permissions */
#define SMC_GROUP_PERMISSION           0x00002LL
/* Memory mapping permissions */
#define MM_GROUP_PERMISSION            0x00004LL
/* Task manipulation permissions */
#define TASK_GROUP_PERMISSION          0x00008LL
/* Memory permissions */
#define MEMORY_GROUP_PERMISSION        0x00010LL
/* Kernel variables permissions */
#define KVAR_GROUP_PERMISSION          0x00020LL
/* SWI permissions */
#define SWI_GROUP_PERMISSION           0x00040LL
/* HWI permissions */
#define HWI_GROUP_PERMISSION           0x00080LL
/* Real time clock(RTC) permissions */
#define RTC_GROUP_PERMISSION           0x00100LL
/* Crypto cell management OPS permissions */
#define CC_OPS_GROUP_PERMISSION        0x00200LL
/* Crypto cell key operations */
#define CC_KEY_GROUP_PERMISSION        GENERAL_GROUP_PERMISSION

/* TODO: we should make CC_KEY_GROUP and CC_KEY_GROUP2 to be one
 * after sfs need't every TA access DX_UTIL_CmacDeriveKey.
 */
#define CC_KEY_GROUP2_PERMISSION       0x00800LL

/* Crypto cell crypto operations */
//#define CC_CRYPTO_GROUP_PERMISSION     0x00800LL
#define CC_CRYPTO_GROUP_PERMISSION     GENERAL_GROUP_PERMISSION

/* Crypto cell RNG operations */
//#define CC_RNG_GROUP_PERMISSION        0x01000LL
#define CC_RNG_GROUP_PERMISSION     GENERAL_GROUP_PERMISSION

/* Crypto cell OEM unpack operations */
//#define CC_RNG_GROUP_PERMISSION        0x02000LL
#define CC_OEM_KEY_GROUP_PERMISSION   GENERAL_GROUP_PERMISSION

/* HDCP driver operations */
#define HDCP_GROUP_PERMISSION          0x04000LL

/* KM ROT operations */
#define KM_ROT_GROUP_PERMISSION          0x08000LL

/* TZASC operations */
#define TZASC_GROUP_PERMISSION         0x10000LL
/* Secboot operations */
#define SECBOOT_GROUP_PERMISSION       0x20000LL
/* IPI operations */
#define IPI_GROUP_PERMISSION           0x40000LL

/* EFUSE operations */
#define EFUSE_GROUP_PERMISSION         0x80000LL

/* timer operations */
#define TIMER_GROUP_PERMISSION         0x100000LL
/* hwi msg operations */
#define HWIMSG_GROUP_PERMISSION         0x200000LL

/* RPMB general operations */
#define RPMB_GENERIC_PERMISSION 0x400000LL
/* RPMB special operations */
#define RPMB_SPECIFIC_PERMISSION 0x800000LL

/*ANTI ROOT*/
#define ROOTSTATUS_GROUP_PERMISSION         0x1000000LL

/* fingerprint sensor operations */
#define FP_GROUP_PERMISSION         0x10000000LL
/* TUI operations */
#define TUI_GROUP_PERMISSION         0x20000000LL
/* vsim operations */
#define VSIM_GROUP_PERMISSION         0x40000000LL

/* vdec operations */
#define VDEC_GROUP_PERMISSION         0x80000000LL

/* modem call operations*/
#define MDMCALL_GROUP_PERMISSION          0x100000000LL

/* iris operations */
#define IRIS_GROUP_PERMISSION         0x200000000LL

/* Reserved operations */
/* All new syscalls use this permissions*/
#define RESERVED_GROUP_PERMISSION         0x400000000LL

/* Access for all functions - ATTN this is reserved for global task
 * and other test tasks */
#define ALL_GROUP_PERMISSION           (UINT64)-1
/**/
#define GT_PERMISSIONS (CONSOLE_GROUP_PERMISSION | SMC_GROUP_PERMISSION | MM_GROUP_PERMISSION | TASK_GROUP_PERMISSION | KVAR_GROUP_PERMISSION | MEMORY_GROUP_PERMISSION | HDCP_GROUP_PERMISSION)

#if defined(TESTSUITE_RTOSck_UT) || defined(TESTSUITE_RTOSck_PT)||defined(TESTSUITE_RTOSck_IT)  //set no mem access isolation when run testsuite
#endif
#endif
