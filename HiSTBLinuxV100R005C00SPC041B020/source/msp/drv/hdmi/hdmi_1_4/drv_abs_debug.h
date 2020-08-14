/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : drv_abs_debug.h
 * Version       :
 * Author        : Hisilicon multimedia software group
 * Created       : 2014/4/1
 * Description   : Abstract layer of debuging message printing Driver
 * History       :
 * Date          : 2014/4/1
 * Author        : l00271847
 * Modification  :
 *******************************************************************************
 *
 */

#ifndef __DRV_ABS_DEBUG_H__
#define __DRV_ABS_DEBUG_H__

//abstract_debug
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include <linux/semaphore.h>
#include <linux/fs.h>

#include "hi_type.h"

typedef enum{
    DBG_NONE    = 0,    /* no print */
    DBG_ALL,            /* print to screen & save to file */
    DBG_SCREEN,         /* print to screen(serial) */
    DBG_FILE,           /* save to file */
    DBG_BUTT
} PRINT_TO_DEV_E;

/* Function Number*/
typedef enum{
    ABS_FUNC_0 = 0,     /* All Function is enable */
    ABS_FUNC_1,
    ABS_FUNC_2,
    ABS_FUNC_3,
    ABS_FUNC_4,
    ABS_FUNC_5,
    ABS_FUNC_6,
    ABS_FUNC_7,
    ABS_FUNC_8,
    ABS_FUNC_9,
    ABS_FUNC_10,
    ABS_FUNC_11,
    ABS_FUNC_12,
    ABS_FUNC_13,
    ABS_FUNC_14,
    ABS_FUNC_15
} ABS_FUNC_T;

/* Debug level */
typedef enum{
    ABS_DBG_INFO    = 0,     /* All Function is enable */
    ABS_DBG_WARN,
    ABS_DBG_ERROR,
    ABS_DBG_FATAL,
    ABS_DBG_END
} ABS_DBG_LEVEL_T;

/* Memory Description Struct */
typedef struct abs_dbg_struct{
    HI_U8       *pdbg_addr  ;   /* memory address */
    HI_U32      dbg_mem_size;   /* memory size */
    HI_U8       *pdbg_pos   ;   /* indicate dbg_addr to (dbg_pos + dgb_offset) has been used */
    HI_U32      dbg_mask    ;   /* indicate printing strings out of memory should allowed the function */
    PRINT_TO_DEV_E dbg_type ;   /* indicate where should be printing to, file or serial */
    struct file *pdbg_fp    ;   /* the file pointer, which save strings to */
    spinlock_t  dbg_lock    ;   /* the lock to protect memory, to prevent to read and write at the same time */
    HI_BOOL     dbg_rflag   ;   /* the reading flag, to prevent to get lock when writing strings to memory */
    HI_MOD_ID_E module_id   ;   /* module ID : which module call these interface */
    HI_U8       dbg_level   ;   /* debug level : info、warning、error */
}ABS_DBG_S, *ABS_DBG_S_PTR, **ABS_DBG_S_PPTR;

#define ABS_PTK(fmt, args...)           printk(fmt, ## args)
#define ABS_FATAL(module_id, fmt...)    HI_FATAL_PRINT(module_id, fmt)
#define ABS_ERR(module_id, fmt...)      HI_ERR_PRINT(module_id, fmt)
#define ABS_WARN(module_id, fmt...)     HI_WARN_PRINT(module_id, fmt)
#define ABS_INFO(module_id, fmt...)     HI_INFO_PRINT(module_id, fmt)

//#ifdef  //u-boot
#define ABS_INIT_LOCK(lock)             spin_lock_init(&lock)
#define ABS_DBG_LOCK(lock)              spin_lock(&lock)
#define ABS_DBG_UNLOCK(lock)            spin_unlock(&lock)
//#endif

#define ABS_KMALLOC(module_id, size) ({    \
             HI_U32 b = 0;  \
             b = (HI_U32) HI_KMALLOC(module_id, size, GFP_KERNEL);    \
             if (b) \
             {  \
                memset((void*)b, 0, size); \
             }  \
             b; \
})

#define ABS_VMALLOC(module_id, size) ({    \
             HI_U32 b = 0;  \
             b = (HI_U32) HI_VMALLOC(module_id, size); \
             if (b) \
             {  \
                memset((void*)b, 0, size); \
             }  \
             b; \
})

#define ABS_KFREE(module_id, a)         HI_KFREE(module_id, a)
#define ABS_VFREE(module_id, a)         HI_VFREE(module_id, a)
#define ABS_MEMSET(a, b, c)             memset(a, b, c)


#ifndef ABS_BUF_DEBUG_LEN
#define ABS_BUF_DEBUG_LEN              0x80000    //default to use 512KB memory
#endif

#define ABS_DBG_MEM_IN_BUF_SIZE         512         //mem size, use to save strings and add func_name to the head
#define ABS_DBG_INF_HEAD_SIZE           0x10
#define ABS_DBG_INF_FUN_NO              3                   //position of function NO. ...<0000...
#define ABS_DBG_INF_FUN_LEN             8                   //position of function len ...0000>...
#define ABS_DBG_INF_FUN_LEVEL           7                   //position of function debug level ##<00000. last 0
#define ABS_DBG_INF_HEAD                "##<000000000>##"   //information head save to mem, 15 bytes
#define ABS_DBG_INF_DATA_LEN            9                   //len of 000000000
#define ABS_DBG_INF_DATA_WIDTH          4                   //len of 0000


/* alloc and initialize memory */
HI_S32 ABS_DbgInit(ABS_DBG_S_PTR pABSdbg, HI_MOD_ID_E module_id);

/* free memory and resource*/
HI_VOID ABS_DbgDeInit(ABS_DBG_S_PTR pABSdbg);

/* print into memory */
HI_VOID ABS_PtkToMem(ABS_DBG_S_PTR pABSdbg, HI_U8 u8DbgLevel, HI_U32 FuncNum,  HI_U8 *u8Char);

/* get strings from memroy and print it out to file or serial  */
HI_S32  ABS_MemOut(ABS_DBG_S_PTR pABSdbg, const HI_CHAR **pFuncName);

/* set debug level : info、warning、error */
HI_S32  ABS_SetDbgLevel(ABS_DBG_S_PTR pABSdbg, ABS_DBG_LEVEL_T level);

/* set the logtype, file or serial */
HI_S32  ABS_SetType(ABS_DBG_S_PTR pABSdbg, PRINT_TO_DEV_E logtype, struct file *pFile);

/* set the function mask */
HI_S32  ABS_SetMask(ABS_DBG_S_PTR pABSdbg, HI_U32 mask);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

