/******************************************************************************

  Copyright (C), 2010-2026, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : drv_hdmi_log.h
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2015/6/27
 * Description   : HDMI log of debuging message printing Driver
 * History       :
 * Date          : 2015/6/27
 * Author        : l00232354
 * Modification  :
 *******************************************************************************
 */
#ifndef __DRV_HDMI_LOG_H__
#define __DRV_HDMI_LOG_H__
#include "hdmi_osal.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/******************************************************************************/
/* Constants                                                                  */
/******************************************************************************/
#ifndef LOG_BUF_SIZE
#define LOG_BUF_SIZE               (0x80000)           //default to use 512KB memory
#endif

#define LOG_MEM_BUF_SIZE           512                 //mem size, use to save strings and add func_name to the head
#define LOG_INFO_HEAD_SIZE         0x10
#define LOG_FUN_NO                 3                   //position of function NO. ...<0000...
#define LOG_FUN_LEN                8                   //position of function len ...0000>...
#define LOG_FUN_LEVEL              7                   //position of function debug level ##<00000. last 0
#define LOG_HEAD_INFO              "##<000000000>##"   //information head save to mem, 15 bytes
#define LOG_HEAD_INFO_DATA_LEN     9                   //len of 000000000
#define LOG_HEAD_INFO_DATA_WIDTH   4                   //len of 0000
#define LOG_FILEE_NAME_SIZE        128                 //Max size of logfile name
#define LOG_OUTPUT_NOW             0x1010              //Max size of logfile name
 
#ifdef HDMI_DEBUG
#define EDID_LOG_INFO(fmt...)      //HI_INFO_PRINT   (HI_ID_HDMI, fmt)
#define EDID_LOG_WARN(fmt...)      //HI_WARN_PRINT   (HI_ID_HDMI, fmt)    
#define EDID_LOG_ERR(fmt...)       //HI_ERR_PRINT    (HI_ID_HDMI, fmt)  
#define EDID_LOG_FATAL(fmt...)     // HI_FATAL_PRINT  (HI_ID_HDMI, fmt)    
#define HDMI_LOG_INFO(fmt...)      DRV_HDMI_LogRecord(LOG_LEVEL_INFO, LOG_TYPE_FUNC_API, fmt)
#define HDMI_LOG_WARN(fmt...)      DRV_HDMI_LogRecord(LOG_LEVEL_WARN, LOG_TYPE_FUNC_API, fmt)
#define HDMI_LOG_ERR(fmt...)       DRV_HDMI_LogRecord(LOG_LEVEL_ERROR, LOG_TYPE_FUNC_API, fmt)
#define HDMI_LOG_FATAL(fmt...)     DRV_HDMI_LogRecord(LOG_LEVEL_FATAL, LOG_TYPE_FUNC_API, fmt)
#endif

/******************************************************************************/
/* Data Structures                                                            */
/******************************************************************************/
/* print mode */
typedef enum{
    LOG_PRINT_TO_NONE    = 0,    /* no print */
    LOG_PRINT_TO_SCREEN,         /* print to screen(serial) */
    LOG_PRINT_TO_FILE,           /* save to file */
    LOG_PRINT_TO_ALL,            /* print to screen(serial) and save to file */
    LOG_PRINT_TO_BUTT
} LOG_PRINT_MODE_E;

/* enable type of function */
typedef enum{
    LOG_TYPE_FUNC_ALL,         /* all  functions is enable */
    LOG_TYPE_FUNC_API,         /* api  functions is enable */
    LOG_TYPE_FUNC_HPD,         /* hpd  functions is enable */
    LOG_TYPE_FUNC_EDID,        /* edid functions is enable */
    LOG_TYPE_FUNC_HDCP,        /* hdcp functions is enable */
    LOG_TYPE_FUNC_SCDC,        /* scdc functions is enable */
    LOG_TYPE_FUNC_CEC,         /* cec  functions is enable */
    LOG_TYPE_FUNC_BUTT
} LOG_TYPE_FUNC_E;

/* log level */
typedef enum{
    LOG_LEVEL_INFO,              /* print info  information */
    LOG_LEVEL_WARN,              /* print warn  information */
    LOG_LEVEL_ERROR,             /* print error information */
    LOG_LEVEL_FATAL,             /* print fatal information */
    LOG_LEVEL_BUTT
} LOG_LEVEL_E;

/* structure of log memory */
typedef struct log_mem_struct{
    HI_U8       *pVirAddr;       /* memory address */
    HI_U8       *pPos;           /* indicate pu8LogVirAddr to (pu8LogPos + LogOffset) has been used */
    HI_BOOL      bRollFlag;      /* indicate pu8LogVirAddr to (pu8LogPos + LogOffset) has been used */
    HI_U32       u32MemSize;     /* memory size */
}LOG_MEM_S;

/* structure of log context */
typedef struct log_ctx_struct{
    HI_BOOL      bInited;         /* log ctx is inited or not */
    LOG_MEM_S    stMem;           /* memory information */
    HI_U8        u8Level;         /* u8LogLevel: info¡¢warn¡¢error¡¢fatal */
    HI_BOOL      bReadFlag;       /* the reading flag, to prevent to get lock when writing strings to memory */
    HI_U32       u32Mask;         /* indicate printing strings out of memory should allowed the function */
    LOG_PRINT_MODE_E eMode;       /* indicate where should be printing to, file or serial */
    struct file *pFile;           /* the file pointer, which save strings to */
    spinlock_t   stSpinLock;      /* the lock to protect memory, to prevent to read and write at the same time */
}LOG_CTX_S;

/******************************************************************************/
/* Gloabl Data                                                                */
/******************************************************************************/

/******************************************************************************/
/* Private Interface                                                           */
/******************************************************************************/

/******************************************************************************/
/* Public Interface                                                           */
/******************************************************************************/
HI_VOID DRV_HDMI_LogInit(HI_VOID);
HI_VOID DRV_HDMI_LogDeInit(HI_VOID);
HI_VOID DRV_HDMI_LogRecord(LOG_LEVEL_E eLevel, LOG_TYPE_FUNC_E eFuncType, const HI_CHAR *fmt, ...);
HI_S32 DRV_HDMI_LogCat(LOG_PRINT_MODE_E eMode, HI_CHAR *pFilePath);
HI_S32 DRV_HDMI_LogSetLevel(LOG_LEVEL_E eLevel);
HI_S32 DRV_HDMI_LogSetMask(LOG_TYPE_FUNC_E eFuncType);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

