/******************************************************************************

  Copyright (C), 2010-2026, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : hdmi_osal.h
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2015/6/27
 * Description   : Deferent linux version for HDMI model
 * History       :
 * Date          : 2015/6/27
 * Author        : l00232354
 * Modification  :
 *******************************************************************************
 */
#ifndef __HDMI_OSAL_H__
#define __HDMI_OSAL_H__
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include "drv_hdmi_platform.h"
#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/******************************************************************************/
/* define type for hdmi                                                       */
/******************************************************************************/
#define OSAL_INIT_LOCK(lock)     spin_lock_init(&lock)
#define OSAL_LOCK(lock)          spin_lock(&lock)
#define OSAL_UNLOCK(lock)        spin_unlock(&lock)

#define OSAL_MEMSET(vir, data, size)     memset(vir, data, size)
#define OSAL_MEMCPY(dest, src, size)     memcpy(dest, src, size)
#define OSAL_STRLEN(vir)                 strlen(vir)
#define OSAL_STRSTR(vir, str)            strstr(vir, str)
#define OSAL_SMSTRTOUL(str, end, base)   simple_strtoul(str, end, base)

#define HDMI_MUTEX                  	struct semaphore
#define HDMI_MUTEX_LOCK(a)  \
do{\
    HI_S32 s32Ret = down_interruptible(&(a));\
    if (0 != s32Ret)\
        {HDMI_ERR("down_interruptible failed! s32Ret=%d\n", s32Ret);}\
}while(0)  


#define HDMI_MUTEX_UNLOCK(a) 			up(&(a))

#ifdef HDMI_DEBUG
#ifndef HI_ADVCA_FUNCTION_RELEASE
#define OSAL_PRINTK(fmt, args...)        printk(fmt, ## args)
#else
#define OSAL_PRINTK(fmt, args...)
#endif
#else
#define OSAL_PRINTK(fmt, args...)
#endif

/******************************************************************************/
/* Data Structures                                                            */
/******************************************************************************/
/* file flags */
typedef enum{
    OSAL_RDONLY = 0,    /* read file only */
    OSAL_WRONLY,        /* write file only */
    OSAL_RDWR,          /* read and write file */
    OSAL_BUTT
} OSAL_FILE_FLAG_S;

/******************************************************************************/
/* Gloabl Data                                                                */
/******************************************************************************/

/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/
#define IS_STR_EQUAL(p1,p2) ({\
    ( p1 && p2 && (strlen(p1)==strlen(p2)) && (strncmp(p1,p2,strlen(p1))==0) ) ? HI_TRUE : HI_FALSE;\
})

/** open a kernel file 
**/
struct file* HDMI_OSAL_FileOpen(const HI_S8* ps8FileName, OSAL_FILE_FLAG_S eFlags);
/** close a kernel file 
**/
HI_VOID HDMI_OSAL_FileClose(struct file * pFile);
/** read a kernel file 
**/
HI_S32 HDMI_OSAL_FileRead(struct file * pFile,  HI_U8* ps8Buf, HI_U32 u32Len);
/** write a kernel file 
**/
HI_S32 HDMI_OSAL_FileWrite(struct file* pFile, HI_S8* ps8Buf, HI_U32 u32Len);


/** copy str to pszDest from pszSrc
**/
HI_CHAR* HDMI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
/** compare pszStr1 with pszStr2
**/
HI_S32 HDMI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);
/** casecompare pszStr1 with pszStr2
**/
HI_S32 HDMI_OSAL_Strncasecmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);
/** strncat pszSrc
**/
HI_CHAR* HDMI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);
/** snprintf pszSrc
**/
HI_S32 HDMI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...);
/** vsnprintf pszSrc
**/
HI_S32 HDMI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, va_list stVAList);


/** get system time in Ms
**/
HI_U32 HDMI_OSAL_GetTimeInMs(HI_VOID);

/** get system time in Us
**/
HI_U32 HDMI_OSAL_GetTimeInUs(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

