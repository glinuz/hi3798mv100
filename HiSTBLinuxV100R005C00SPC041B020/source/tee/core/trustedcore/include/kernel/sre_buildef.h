/**
 * @file SRE_buildef.h
 *
 * Copyright(C), 2008-2050, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED. \n
 *
 * 描述：定义硬件平台的编译宏。 \n
 */

#ifndef _SRE_BUILDEF_H
#define _SRE_BUILDEF_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "sre_debug.h"

#define OS_LITTLE_ENDIAN                                    0x1234      /* 小字节序 */
#define OS_BIG_ENDIAN                                       0x4321      /* 大字节序 */

#define OS_CACHE_LINE_SIZE                                  64
#define OS_CACHE_WBSEG_SIZE                                 64

/* To define OS_HARDWARE_PLATFORM */
#define OS_C64X                                             0x0
#define OS_C64XPLUS                                         0x1
#define OS_SC3400                                           0x2
#define OS_XTENSA2                                          0x3
#define OS_SC3850                                           0x4
#define OS_RISC170                                          0x5
#define OS_DSP170                                           0x6
#define OS_RISC220                                          0x7
#define OS_DSP220                                           0x8
#define OS_HACCC                                            0x9
#define OS_CORTEX_A9                                        0xa

/* To define OS_CPU_TYPE */
#define OS_TI6482                                           0x0
#define OS_LSI2603                                          0x1
#define OS_MSC8144                                          0x2
#define OS_SD6153                                           0x3
#define OS_TI6487                                           0x4
#define OS_GBDSP                                            0x5
#define OS_MSC8156                                          0x6
#define OS_SD6108                                           0x7
#define OS_SD6181                                           0x8
#define OS_SD6182                                           0x9
#define OS_HI1380                                           0xa
#define OS_HI3620                                           0xb

#ifndef OS_HARDWARE_PLATFORM
#define OS_HARDWARE_PLATFORM                                OS_CORTEX_A9
#endif

#ifndef OS_CPU_TYPE
#define OS_CPU_TYPE                                         OS_HI3620
#endif


#ifndef OS_BYTE_ORDER
#define OS_BYTE_ORDER                                       OS_LITTLE_ENDIAN
#endif

#define OS_HAVE_COPROCESSOR1                                NO
#define OS_HAVE_DCACHE                                      YES

/* 配置芯片最大的核数。用于初始化时指定waitforallcores的数组长度。
   此后内存模块初始化完成，之后均使用用户配置的启动核数来配置核相关数组长度。 */
/* 各个核定制的Write error interrupt中断号OS_EXC_WRITE_ERROR_INTERRUPT,
   等后续其他核都支持该中断之后，再可以在这里填入，
   并将sre_exc_init.c中引用该宏处的#if删除 */
#define OS_SYNC_CORE_NUM                                 20

#define OS_EXC_WRITE_ERROR_INTERRUPT                     9

#ifdef DEBUG
#define OS_DEBUG
#endif

/* 定义操作系统的代码数据分段 */
/*指示函数为inlne类型*/
#ifndef OS_SEC_ALW_INLINE
#define OS_SEC_ALW_INLINE  __attribute__((always_inline))
#endif

#ifndef OS_SEC_TEXT
#define OS_SEC_TEXT __attribute__((section(".text")))
#endif

#ifndef OS_SEC_TEXT_MINOR
#define OS_SEC_TEXT_MINOR __attribute__((section(".text")))
#endif

#ifndef OS_SEC_TEXT_MONITOR
#define OS_SEC_TEXT_MONITOR __attribute__((section(".text")))
#endif

#ifndef OS_SEC_TEXT_INIT
#define OS_SEC_TEXT_INIT __attribute__((section(".text")))
#endif

#ifndef OS_SEC_TEXT_HSHELL
#define OS_SEC_TEXT_HSHELL  __attribute__((section(".text")))
#endif

#ifndef OS_SEC_TEXT_NONCACHE
#define OS_SEC_TEXT_NONCACHE
#endif

#ifndef OS_SEC_DATA
#define OS_SEC_DATA __attribute__((section(".data")))
#endif

#ifndef OS_SEC_DATA_SHARED
#define OS_SEC_DATA_SHARED __attribute__((section(".shared.data")))
#endif

#ifndef OS_SEC_DATA_INIT
#define OS_SEC_DATA_INIT __attribute__((section(".data")))
#endif

#ifndef OS_SEC_PRI_DATA_HSHELL
#define OS_SEC_PRI_DATA_HSHELL __attribute__((section(".data")))
#endif

#ifndef OS_SEC_DATA_HSHELL
#define OS_SEC_DATA_HSHELL __attribute__((section(".data")))
#endif

#ifndef OS_SEC_BSS
#define OS_SEC_BSS      __attribute__((section(".bss")))
#endif

#ifndef OS_SEC_BSS_SHARED
#define OS_SEC_BSS_SHARED __attribute__((section(".shared.bss")))
#endif

#ifndef OS_SEC_BSS_INIT
#define OS_SEC_BSS_INIT __attribute__((section(".bss")))
#endif

#ifndef OS_SEC_BSS_BACK
#define OS_SEC_BSS_BACK   __attribute__((section(".bss")))
#endif

#ifndef OS_SEC_BSS_HSHELL
#define OS_SEC_BSS_HSHELL   __attribute__((section(".bss")))
#endif

#ifndef OS_SEC_BSS_HSHELL_SVR
#define OS_SEC_BSS_HSHELL_SVR   __attribute__((section(".bss")))
#endif

/* Hi1380 nocache data */
#ifndef OS_SEC_NC_DATA_SHARED
#define OS_SEC_NC_DATA_SHARED   __attribute__((section(".nc.data")))
#endif

/* Hi1380 nocache bss */
#ifndef OS_SEC_NC_BSS_SHARED
#define OS_SEC_NC_BSS_SHARED   __attribute__((section(".nc.bss")))
#endif

/* Hi1380 test data */
#ifndef OS_SEC_TEST_DATA_SHARED
#define OS_SEC_TEST_DATA_SHARED   __attribute__((section(".test.data")))
#endif

/* TEE Heap */
#ifndef OS_TEE_HEAP
#define OS_TEE_HEAP __attribute__((section(".tee_heap.bss")))
#endif

/* TEE lib */
//#define SRAM_LIMIT
#ifdef SRAM_LIMIT
#ifndef OS_TEE_LIB_TEXT
#define OS_TEE_LIB_TEXT __attribute__((section(".tee_lib.text")))
#endif

#ifndef OS_TEE_LIB_DATA
#define OS_TEE_LIB_DATA __attribute__((section(".tee_lib.data")))
#endif
#else
#undef OS_TEE_LIB_TEXT
#undef OS_TEE_LIB_DATA
#define OS_TEE_LIB_TEXT
#define OS_TEE_LIB_DATA
#endif

#define EXTERN_FAR extern

#define PACK1

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#endif /* _SRE_BUILDEF_H */

/**
 * History:
 * 2008-02-20 l66919: Create this file.
 * 2010-05-07 w00164922 modify OS_CACHE_LINE_SIZE
 * vi: set expandtab ts=4 sw=4 tw=80:
 */

