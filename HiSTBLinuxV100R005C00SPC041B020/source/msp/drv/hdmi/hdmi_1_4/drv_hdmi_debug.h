/******************************************************************************

  Copyright (C), 2014-2014, Hisilicon. Co., Ltd.

 ******************************************************************************
 * File Name     : drv_hdmi_debug.h
 * Version       : 1.0
 * Author        : Hisilicon multimedia software group
 * Created       : 2014/4/1
 * Description   : HDMI layer of debuging message printing Driver
 * History       :
 * Date          : 2014/4/1
 * Author        : l00271847
 * Modification  :
 *******************************************************************************
 */

#ifndef __DRV_HDMI_DEBUG_H__
#define __DRV_HDMI_DEBUG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#include "hi_type.h"
#include "drv_abs_debug.h"

#define HDMI_DEBUG_LOGTYPE                  1
#define HDMI_DEBUG_LOGMASK                  2
#define HDMI_DEBUG_LOGCATCH                 3

#define HDMI_LOGMASK_ADD_FUNC               1   // echo logmask +cec > /proc/msp/hdmi0
#define HDMI_LOGMASK_DEC_FUNC               2   // echo logmask -cec > /proc/msp/hdmi0
#define HDMI_LOGMASK_ONLY_FUNC              3   // echo logmask *cec > /proc/msp/hdmi0
#define HDMI_LOGMASK_ALL_FUNC               4   // echo logmask * > /proc/msp/hdmi0

#define HDMI_DEBUG_PRINT_ALL                (0x1 << ABS_FUNC_0)
#define HDMI_DEBUG_PRINT_EDID               (0x1 << ABS_FUNC_1)
#define HDMI_DEBUG_PRINT_HDCP               (0x1 << ABS_FUNC_2)
#define HDMI_DEBUG_PRINT_CEC                (0x1 << ABS_FUNC_3)
#define HDMI_DEBUG_PRINT_HPD                (0x1 << ABS_FUNC_4)
#define HDMI_DEBUG_PRINT_COM                (0x1 << ABS_FUNC_5)
#define HDMI_DEBUG_PRINT_END                (0x1 << ABS_FUNC_6)


#define HDMI_DBG_KMALLOC(a)                 ABS_KMALLOC(HI_ID_HDMI, a)
#define HDMI_DBG_KFREE(a)                   ABS_KFREE(HI_ID_HDMI, a)
#define HDMI_DBG_VMALLOC(a)                 ABS_VMALLOC(HI_ID_HDMI, a)
#define HDMI_DBG_VFREE(a)                   ABS_VFREE(HI_ID_HDMI, a)
#define HDMI_DBG_MEMSET(a, b, c)            ABS_MEMSET(a, b, c)

#define HDMI_PTK_PRINTK(args...)            ABS_PTK(## args)
#define HDMI_PTK_FATAL(fmt...)              ABS_FATAL(HI_ID_HDMI, fmt)
#define HDMI_PTK_ERR(fmt...)                ABS_ERR(HI_ID_HDMI, fmt)
#define HDMI_PTK_WARN(fmt...)               ABS_WARN(HI_ID_HDMI, fmt)
#define HDMI_PTK_INFO(fmt...)               ABS_INFO(HI_ID_HDMI, fmt)

//#define HDMI_DBG_FILE_DEF                   "/root/HDMI_debug.log"  //default file path
#define HDMI_DBG_FILE_DEF                   "/HDMI_debug.log"  //default file path

//#ifdef HDMI_DEBUG
HI_S32  HDMI_DbgInit(HI_VOID);
HI_VOID HDMI_DbgDeInit(HI_VOID);

HI_S32  ABS_ClearMem(ABS_DBG_S_PTR pABSdbg);
HI_S32  HDMI_SetDebLevel(ABS_DBG_LEVEL_T level);

/* print the strings to memory */
HI_VOID HDMI_Dbg(HI_U8 u8DbgLevel, HI_U32 u32HDMI_FuncNum, const HI_CHAR *fmt, ...);

/* print the strings in memory to file or serial */
//echo logcatch > /proc/msp/hdmi0
HI_VOID HDMI_DbgMemPrint(HI_VOID);

/* set the logtype, file or serial */
//echo logtype [serial/file/all] > /proc/msp/hdmi0
HI_VOID  HDMI_DbgSetType(PRINT_TO_DEV_E logtype, const HI_CHAR *pFilePath);

/* set the function mask */
// echo logMask [ cec/hdcp/hpd/edid/all ] > /proc/msp/hdmi0
HI_VOID  HDMI_DbgSetMask(HI_U32 CmdType, HI_U32 LogMask);

/* process the cmd come from hdmi_ProcWrite */
HI_S32  HDMI_ProcessCmd(HI_U32 u32CmdType, const HI_CHAR *pCmdParam, const HI_CHAR *pFilePath);

#define HDMI_DBG_INFO(no, fmt...)           HDMI_Dbg(ABS_DBG_INFO, no, fmt)
#define HDMI_DBG_WARN(no, fmt...)           HDMI_Dbg(ABS_DBG_WARN, no, fmt)
#define HDMI_DBG_ERR(no, fmt...)            HDMI_Dbg(ABS_DBG_ERROR, no, fmt)
#define HDMI_DBG_FATAL(no, fmt...)          HDMI_Dbg(ABS_DBG_FATAL, no, fmt)

//#else
//
//#define HDMI_dbg_Init()                     0
//#define HDMI_dbg_DeInit()
//#define HDMI_set_deb_level(a)               0
//#define HDMI_dbg(a, b, fmt...)
//#define HDMI_dbg_mem_print()
//#define HDMI_dbg_set_type(a, b)
//#define HDMI_dbg_set_mask(a, b)
//#define HDMI_process_cmmd(a, b, c)          0
//
//#define HDMI_DBG_INFO(no, fmt...)           0
//#define HDMI_DBG_WARN(no, fmt...)           0
//#define HDMI_DBG_ERRO(no, fmt...)           0
//#define HDMI_DBG_FATAL(no, fmt...)          0
//
//#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

