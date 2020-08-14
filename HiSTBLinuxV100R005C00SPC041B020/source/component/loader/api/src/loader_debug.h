/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : loader_debug.h
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

#ifndef __LOADER_DEBUG_H__
#define __LOADER_DEBUG_H__

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
/** \addtogroup     LOADER_DEBUG */
/** @{ */  /** <!-- [LOADER_DEBUG] */

/****** Macro switch definition ***********/
#define LDDB_D_LOG_ENABLE            (1)
#define LDDB_D_DBG_ENABLE            (0)
#define LDDB_D_DBG_FUNCTRACE         (0)

#define LDDB_D_APPID                 (0x80UL + 0x20UL)
#define LDDB_D_MID_LOADER            (0x90)

/****** Macro constants definition ********/

#define LDDB_ERRCODE_DEF(errid) (HI_S32)(((LDDB_D_APPID) << 24) | ((LDDB_D_MID_LOADER) << 16) |  (errid))

/** common error code define */
#define LDDB_ERR_MEM                         LDDB_ERRCODE_DEF(0x101)      /**< 内存操作错误 */
#define LDDB_ERR_SEM                         LDDB_ERRCODE_DEF(0x102)      /**< 信号操作错误 */
#define LDDB_ERR_FILE                        LDDB_ERRCODE_DEF(0x103)      /**< 文件操作错误 */
#define LDDB_ERR_LOCK                        LDDB_ERRCODE_DEF(0x104)      /**< 互斥锁操作错误 */
#define LDDB_ERR_PARAM                       LDDB_ERRCODE_DEF(0x105)      /**< 无效参数 */
#define LDDB_ERR_TIMER                       LDDB_ERRCODE_DEF(0x106)      /**< 定时器错误 */
#define LDDB_ERR_THREAD                      LDDB_ERRCODE_DEF(0x107)      /**< 线程操作错误 */
#define LDDB_ERR_TIMEOUT                     LDDB_ERRCODE_DEF(0x108)      /**< 超时异常 */
#define LDDB_ERR_DEVICE                      LDDB_ERRCODE_DEF(0x109)      /**< 设备异常 */
#define LDDB_ERR_STATUS                      LDDB_ERRCODE_DEF(0x110)      /**< 状态异常 */
#define LDDB_ERR_IOCTRL                      LDDB_ERRCODE_DEF(0x111)      /**< 设备操作错误 */
#define LDDB_ERR_INUSE                       LDDB_ERRCODE_DEF(0x112)      /**< 在使用 */
#define LDDB_ERR_EXIST                       LDDB_ERRCODE_DEF(0x113)      /**< 已存在 */
#define LDDB_ERR_NOEXIST                     LDDB_ERRCODE_DEF(0x114)      /**< 不存在 */
#define LDDB_ERR_UNSUPPORTED                 LDDB_ERRCODE_DEF(0x115)      /**< 不支持 */
#define LDDB_ERR_UNAVAILABLE                 LDDB_ERRCODE_DEF(0x116)      /**< 不可用 */
#define LDDB_ERR_UNINITED                    LDDB_ERRCODE_DEF(0x117)      /**< 设备未初始化 */
#define LDDB_ERR_DATABASE                    LDDB_ERRCODE_DEF(0x118)      /**< 数据库操作失败 */
#define LDDB_ERR_OVERFLOW                    LDDB_ERRCODE_DEF(0x119)      /**< 未知错误 */
#define LDDB_ERR_EXTERNAL                    LDDB_ERRCODE_DEF(0x120)      /**< 外部错误 */
#define LDDB_ERR_UNKNOWNED                   LDDB_ERRCODE_DEF(0x121)      /**< 未知错误 */
#define LDDB_ERR_FLASH_READ                  LDDB_ERRCODE_DEF(0x122)      /** Flash reading error*/
#define LDDB_ERR_FLASH_WRITE                 LDDB_ERRCODE_DEF(0x123)      /** Flash writing error */


/****** Macro Functions definition ********/
#if LDDB_D_LOG_ENABLE

#define LDDB_DEBUG(pszFormat...)             HI_DBG_PRINT(HI_ID_LOADER, pszFormat)
#define LDDB_INFO(pszFormat...)              HI_INFO_PRINT(HI_ID_LOADER, pszFormat)
#define LDDB_WARN(pszFormat...)              HI_WARN_PRINT(HI_ID_LOADER, pszFormat)
#define LDDB_ERROR(pszFormat...)             HI_ERR_PRINT(HI_ID_LOADER, pszFormat)
#define LDDB_FATAL(pszFormat...)             HI_FATAL_PRINT(HI_ID_LOADER, pszFormat)

#else

#define LDDB_DEBUG(pszFormat...)
#define LDDB_INFO(pszFormat...)
#define LDDB_WARN(pszFormat...)
#define LDDB_ERROR(pszFormat...)
#define LDDB_FATAL(pszFormat...)

#endif /** LDDB_D_LOG_ENABLE */

#define LDDB_PrintFuncErr(Func, ErrCode)     LDDB_ERROR("Call [%s] return [0x%08x]\n", #Func, ErrCode);
#define LDDB_PrintErrCode(ErrCode)           LDDB_ERROR("Error Code: [0x%08x]\n", ErrCode);
#define LDDB_PrintErrInfo(ErrInfo)           LDDB_ERROR("<%s>\n", ErrInfo);

#define LDDB_PrintS32(val)                   LDDB_DEBUG("%s = %d\n",    #val, val)
#define LDDB_PrintU32(val)                   LDDB_DEBUG("%s = %u\n",    #val, val)
#define LDDB_PrintS64(val)                   LDDB_DEBUG("%s = %lld\n",  #val, val)
#define LDDB_PrintU64(val)                   LDDB_DEBUG("%s = %llu\n",  #val, val)
#define LDDB_PrintStr(val)                   LDDB_DEBUG("%s = %s\n",    #val, val)
#define LDDB_PrintVoid(val)                  LDDB_DEBUG("%s = %p\n",    #val, val)
#define LDDB_Print32Hex(val)                 LDDB_DEBUG("%s = 0x%08x\n",#val, val)
#define LDDB_Print64Hex(val)                 LDDB_DEBUG("%s = 0x%016llx\n", #val, val)
#define LDDB_PrintFloat(val)                 LDDB_DEBUG("%s = %f\n",    #val, val)
#define LDDB_PrintInfo(val)                  LDDB_DEBUG("<%s>\n", val)

#define LDDB_CHECK(func)                                \
{                                                       \
    HI_S32 s32ErrCode = func;                           \
    if (HI_SUCCESS != s32ErrCode)                       \
    {                                                   \
        LDDB_PrintFuncErr(#func, s32ErrCode);           \
    }                                                   \
}


#define LDDB_CHECK_PARAM(val)                           \
{                                                       \
    if (val)                                            \
    {                                                   \
        LDDB_PrintErrCode(LDDB_ERR_PARAM);              \
        return LDDB_ERR_PARAM;                          \
    }                                                   \
}

#define LDDB_CHECK_INITED(u32InitCount)                 \
{                                                       \
    if (0 == u32InitCount)                              \
    {                                                   \
        LDDB_PrintErrCode(LDDB_ERR_UNINITED);           \
        return LDDB_ERR_UNINITED;                       \
    }                                                   \
}

#define LDDB_CheckFuncErr(Func, ErrCode)                \
{                                                       \
    if (HI_SUCCESS != ErrCode)                          \
    {                                                   \
        LDDB_PrintFuncErr(Func, ErrCode);               \
    }                                                   \
}

#define LDDB_ReturnFuncErr(Func, ErrCode, RetErrCode)   \
{                                                       \
    if (HI_SUCCESS != ErrCode)                          \
    {                                                   \
        LDDB_PrintFuncErr(Func, ErrCode);               \
        return RetErrCode;                              \
    }                                                   \
}

#if LDDB_D_DBG_ENABLE

#if LDDB_D_DBG_FUNCTRACE
#define LDDB_DBG_FuncEnter()                 LDDB_INFO(" =====>[Enter]\n")
#define LDDB_DBG_FuncExit()                  LDDB_INFO(" =====>[Exit]\n")
#else
#define LDDB_DBG_FuncEnter()
#define LDDB_DBG_FuncExit()
#endif

#define LDDB_DBG_DEBUG(pszFormat...)         LDDB_DEBUG(pszFormat)
#define LDDB_DBG_INFO(pszFormat...)          LDDB_INFO(pszFormat)
#define LDDB_DBG_WARN(pszFormat...)          LDDB_WARN(pszFormat)
#define LDDB_DBG_ERROR(pszFormat...)         LDDB_ERROR(pszFormat)
#define LDDB_DBG_FATAL(pszFormat...)         LDDB_FATAL(pszFormat)


#define LDDB_DBG_PrintU32(val)               LDDB_DEBUG("%s = %u\n", #val, val)
#define LDDB_DBG_PrintS32(val)               LDDB_DEBUG("%s = %d\n", #val, val)
#define LDDB_DBG_PrintS64(val)               LDDB_DEBUG("%s = %lld\n",  #val, val)
#define LDDB_DBG_PrintU64(val)               LDDB_DEBUG("%s = %llu\n",  #val, val)
#define LDDB_DBG_PrintStr(val)               LDDB_DEBUG("%s = %s\n", #val, val)
#define LDDB_DBG_PrintVoid(val)              LDDB_DEBUG("%s = %p\n", #val, val)
#define LDDB_DBG_Print32Hex(val)             LDDB_DEBUG("%s = 0x%08x\n", #val, val)
#define LDDB_DBG_Print64Hex(val)             LDDB_DEBUG("%s = 0x%016llx\n", #val, val)
#define LDDB_DBG_PrintFloat(val)             LDDB_DEBUG("%s = %f\n", #val, val)
#define LDDB_DBG_PrintInfo(val)              LDDB_DEBUG("<%s>\n", val)

#define LDDB_DBG_PrintBuffer(pcInfo, pu8Input, u32Length)  \
{  \
    HI_U32 u32InnerI = 0;  \
    printf("[%s][%u] %s\n", __FUNCTION__, __LINE__, pcInfo);  \
    for (u32InnerI = 0 ; u32InnerI < u32Length; u32InnerI++)  \
    {  \
        if((u32InnerI % 16 == 0)  \
            && (u32InnerI != 0))  \
        {  \
            printf("\n");  \
        }  \
        printf("0x%02x ", pu8Input[u32InnerI]);  \
    }  \
    printf("\n");  \
}

#else
#define LDDB_DBG_FuncEnter()
#define LDDB_DBG_FuncExit()

#define LDDB_DBG_DEBUG(pszFormat...)
#define LDDB_DBG_INFO(pszFormat...)
#define LDDB_DBG_ERROR(pszFormat...)
#define LDDB_DBG_FATAL(pszFormat...)

#define LDDB_DBG_PrintU32(val)
#define LDDB_DBG_PrintS32(val)
#define LDDB_DBG_PrintStr(val)
#define LDDB_DBG_PrintVoid(val)
#define LDDB_DBG_Print32Hex(val)
#define LDDB_DBG_Print64Hex(val)
#define LDDB_DBG_PrintFloat(val)
#define LDDB_DBG_PrintInfo(val)

#define LDDB_DBG_PrintBuffer(pcInfo, pu8Input, u32Length)

#endif

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     LOADER_DEBUG */
/** @{ */  /** <!-- [LOADER_DEBUG] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     LOADER_DEBUG */
/** @{ */  /** <!-- [LOADER_DEBUG] */


/** @}*/  /** <!-- ==== API Declaration End ====*/


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif  /* __LOADER_DEBUG_H__ */

