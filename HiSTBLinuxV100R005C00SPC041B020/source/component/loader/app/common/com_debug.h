/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_debug.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_common.h"
#include "hi_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Macro Definition *********************************/
/** \addtogroup     COM_DEBUG */
/** @{ */  /** <!-- [COM_DEBUG] */

/****** Macro switch definition ***********/
#define COM_D_LOG_ENABLE            (1)
#define COM_D_DBG_ENABLE            (0)
#define COM_D_DBG_FUNCTRACE         (0)

#define COM_D_APPID                 (0x80UL + 0x20UL)
#define COM_D_MID_LOADER            (0x90)

/****** Macro constants definition ********/

#define COM_ERRCODE_DEF(errid) (HI_S32)(((COM_D_APPID) << 24) | ((COM_D_MID_LOADER) << 16) |  (errid))

/** common error code define */
#define COM_ERR_MEM                         COM_ERRCODE_DEF(0x101)      /**< 内存操作错误 */
#define COM_ERR_SEM                         COM_ERRCODE_DEF(0x102)      /**< 信号操作错误 */
#define COM_ERR_FILE                        COM_ERRCODE_DEF(0x103)      /**< 文件操作错误 */
#define COM_ERR_LOCK                        COM_ERRCODE_DEF(0x104)      /**< 互斥锁操作错误 */
#define COM_ERR_PARAM                       COM_ERRCODE_DEF(0x105)      /**< 无效参数 */
#define COM_ERR_TIMER                       COM_ERRCODE_DEF(0x106)      /**< 定时器错误 */
#define COM_ERR_THREAD                      COM_ERRCODE_DEF(0x107)      /**< 线程操作错误 */
#define COM_ERR_TIMEOUT                     COM_ERRCODE_DEF(0x108)      /**< 超时异常 */
#define COM_ERR_DEVICE                      COM_ERRCODE_DEF(0x109)      /**< 设备异常 */
#define COM_ERR_STATUS                      COM_ERRCODE_DEF(0x110)      /**< 状态异常 */
#define COM_ERR_IOCTRL                      COM_ERRCODE_DEF(0x111)      /**< 设备操作错误 */
#define COM_ERR_INUSE                       COM_ERRCODE_DEF(0x112)      /**< 在使用 */
#define COM_ERR_EXIST                       COM_ERRCODE_DEF(0x113)      /**< 已存在 */
#define COM_ERR_NOEXIST                     COM_ERRCODE_DEF(0x114)      /**< 不存在 */
#define COM_ERR_UNSUPPORTED                 COM_ERRCODE_DEF(0x115)      /**< 不支持 */
#define COM_ERR_UNAVAILABLE                 COM_ERRCODE_DEF(0x116)      /**< 不可用 */
#define COM_ERR_UNINITED                    COM_ERRCODE_DEF(0x117)      /**< 设备未初始化 */
#define COM_ERR_DATABASE                    COM_ERRCODE_DEF(0x118)      /**< 数据库操作失败 */
#define COM_ERR_OVERFLOW                    COM_ERRCODE_DEF(0x119)      /**< 未知错误 */
#define COM_ERR_EXTERNAL                    COM_ERRCODE_DEF(0x120)      /**< 外部错误 */
#define COM_ERR_UNKNOWNED                   COM_ERRCODE_DEF(0x121)      /**< 未知错误 */
#define COM_ERR_FLASH_READ                  COM_ERRCODE_DEF(0x122)      /** Flash reading error*/
#define COM_ERR_FLASH_WRITE                 COM_ERRCODE_DEF(0x123)      /** Flash writing error */

/** service error code define */
#define COM_ERR_IMAGE_NOEXIST               COM_ERRCODE_DEF(0x151)      /** can't find ip upgrade file */
#define COM_ERR_IMAGE_UNAVAILABLE           COM_ERRCODE_DEF(0x152)      /** software version not match */
#define COM_ERR_IMAGE_ILLEGAL               COM_ERRCODE_DEF(0x153)      /** Download data parsing error */
#define COM_ERR_DOWNLOAD_TIMEOUT            COM_ERRCODE_DEF(0x154)      /** can't find usb or upgrade file */
#define COM_ERR_CONNECT_FAIL                COM_ERRCODE_DEF(0x155)      /** connect server failed */
#define COM_ERR_SIGNAL_LOSE                 COM_ERRCODE_DEF(0x156)      /** Abnormal signal */
#define COM_ERR_NETWORK                     COM_ERRCODE_DEF(0x157)      /** network exception */
#define COM_ERR_IPCONFIG                    COM_ERRCODE_DEF(0x158)      /** ip config failed */

/****** Macro Functions definition ********/
#if COM_D_LOG_ENABLE

#define COM_DEBUG(pszFormat...)             HI_DBG_PRINT(HI_ID_LOADER, pszFormat)
#define COM_INFO(pszFormat...)              HI_INFO_PRINT(HI_ID_LOADER, pszFormat)
#define COM_WARN(pszFormat...)              HI_WARN_PRINT(HI_ID_LOADER, pszFormat)
#define COM_ERROR(pszFormat...)             HI_ERR_PRINT(HI_ID_LOADER, pszFormat)
#define COM_FATAL(pszFormat...)             HI_FATAL_PRINT(HI_ID_LOADER, pszFormat)

#else

#define COM_DEBUG(pszFormat...)
#define COM_INFO(pszFormat...)
#define COM_WARN(pszFormat...)
#define COM_ERROR(pszFormat...)
#define COM_FATAL(pszFormat...)

#endif /** COM_D_LOG_ENABLE */

#define COM_PrintFuncErr(Func, ErrCode)     COM_ERROR("Call [%s] return [0x%08x]\n", #Func, ErrCode);
#define COM_PrintErrCode(ErrCode)           COM_ERROR("Error Code: [0x%08x]\n", ErrCode);
#define COM_PrintErrInfo(ErrInfo)           COM_ERROR("<%s>\n", ErrInfo);

#define COM_PrintS32(val)                   COM_DEBUG("%s = %d\n",    #val, val)
#define COM_PrintU32(val)                   COM_DEBUG("%s = %u\n",    #val, val)
#define COM_PrintS64(val)                   COM_DEBUG("%s = %lld\n",  #val, val)
#define COM_PrintU64(val)                   COM_DEBUG("%s = %llu\n",  #val, val)
#define COM_PrintStr(val)                   COM_DEBUG("%s = %s\n",    #val, val)
#define COM_PrintVoid(val)                  COM_DEBUG("%s = %p\n",    #val, val)
#define COM_Print32Hex(val)                 COM_DEBUG("%s = 0x%08x\n",#val, val)
#define COM_Print64Hex(val)                 COM_DEBUG("%s = 0x%016llx\n", #val, val)
#define COM_PrintFloat(val)                 COM_DEBUG("%s = %f\n",    #val, val)
#define COM_PrintInfo(val)                  COM_DEBUG("<%s>\n", val)

#define COM_PrintIPAddr(title, ipaddr)      COM_DEBUG("%s: %u.%u.%u.%u\n", (title), (((ipaddr) >> 24) & 0xff), (((ipaddr) >> 16) & 0xff), (((ipaddr) >> 8) & 0xff), ((ipaddr) & 0xff));

#define COM_CHECK(func)                                 \
    {                                                   \
        HI_S32 s32ErrCode = func;                       \
        if (HI_SUCCESS != s32ErrCode)                   \
        {                                               \
            COM_PrintFuncErr(#func, s32ErrCode);        \
        }                                               \
    }


#define COM_CHECK_PARAM(val)                            \
    {                                                   \
        if (val)                                        \
        {                                               \
            COM_PrintErrCode(COM_ERR_PARAM);            \
            return COM_ERR_PARAM;                       \
        }                                               \
    }

#define COM_CHECK_INITED(u32InitCount)                  \
    {                                                   \
        if (0 == u32InitCount)                          \
        {                                               \
            COM_PrintErrCode(COM_ERR_UNINITED);         \
            return COM_ERR_UNINITED;                    \
        }                                               \
    }

#define COM_CheckFuncErr(Func, ErrCode)                 \
    {                                                   \
        if (HI_SUCCESS != ErrCode)                      \
        {                                               \
            COM_PrintFuncErr(Func, ErrCode);            \
        }                                               \
    }

#define COM_ReturnFuncErr(Func, ErrCode, RetErrCode)    \
    {                                                   \
        if (HI_SUCCESS != ErrCode)                      \
        {                                               \
            COM_PrintFuncErr(Func, ErrCode);            \
            return RetErrCode;                          \
        }                                               \
    }

#define COM_ReturnMax(a, b)     ((a)>(b) ? (a):(b))
#define COM_ReturnMin(a, b)     ((a)<(b) ? (a):(b))


#if COM_D_DBG_ENABLE

#if COM_D_DBG_FUNCTRACE
#define COM_DBG_FuncEnter()                 COM_INFO(" =====>[Enter]\n")
#define COM_DBG_FuncExit()                  COM_INFO(" =====>[Exit]\n")
#else
#define COM_DBG_FuncEnter()
#define COM_DBG_FuncExit()
#endif

#define COM_DBG_DEBUG(pszFormat...)         COM_DEBUG(pszFormat)
#define COM_DBG_INFO(pszFormat...)          COM_INFO(pszFormat)
#define COM_DBG_WARN(pszFormat...)          COM_WARN(pszFormat)
#define COM_DBG_ERROR(pszFormat...)         COM_ERROR(pszFormat)
#define COM_DBG_FATAL(pszFormat...)         COM_FATAL(pszFormat)


#define COM_DBG_PrintU32(val)               COM_DEBUG("%s = %u\n", #val, val)
#define COM_DBG_PrintS32(val)               COM_DEBUG("%s = %d\n", #val, val)
#define COM_DBG_PrintS64(val)               COM_DEBUG("%s = %lld\n",  #val, val)
#define COM_DBG_PrintU64(val)               COM_DEBUG("%s = %llu\n",  #val, val)
#define COM_DBG_PrintStr(val)               COM_DEBUG("%s = %s\n", #val, val)
#define COM_DBG_PrintVoid(val)              COM_DEBUG("%s = %p\n", #val, val)
#define COM_DBG_Print32Hex(val)             COM_DEBUG("%s = 0x%08x\n", #val, val)
#define COM_DBG_Print64Hex(val)             COM_DEBUG("%s = 0x%016llx\n", #val, val)
#define COM_DBG_PrintFloat(val)             COM_DEBUG("%s = %f\n", #val, val)
#define COM_DBG_PrintInfo(val)              COM_DEBUG("<%s>\n", val)

#define COM_DBG_PrintIPAddr(title, ipaddr)  COM_DEBUG("%s: %u.%u.%u.%u\n", (title), (((ipaddr) >> 24) & 0xff), (((ipaddr) >> 16) & 0xff), (((ipaddr) >> 8) & 0xff), ((ipaddr) & 0xff));

#else
#define COM_DBG_FuncEnter()
#define COM_DBG_FuncExit()

#define COM_DBG_DEBUG(pszFormat...)
#define COM_DBG_INFO(pszFormat...)
#define COM_DBG_ERROR(pszFormat...)
#define COM_DBG_FATAL(pszFormat...)

#define COM_DBG_PrintU32(val)
#define COM_DBG_PrintS32(val)
#define COM_DBG_PrintStr(val)
#define COM_DBG_PrintVoid(val)
#define COM_DBG_Print32Hex(val)
#define COM_DBG_Print64Hex(val)
#define COM_DBG_PrintFloat(val)
#define COM_DBG_PrintInfo(val)

#define COM_DBG_PrintIPAddr(title, ipaddr)

#endif

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     COM_DEBUG */
/** @{ */  /** <!-- [COM_DEBUG] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     COM_DEBUG */
/** @{ */  /** <!-- [COM_DEBUG] */

#ifdef HI_LOADER_APPLOADER
HI_S32 COM_LogSystemInit(HI_VOID);

HI_S32 COM_LogSystemDeInit(HI_VOID);
#endif

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif  /* __COM_DEBUG_H__ */
