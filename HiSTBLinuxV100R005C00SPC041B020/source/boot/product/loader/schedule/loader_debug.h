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

#include "hi_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup     LOADER_DEBUG */
/** @{ */  /** <!-- [LOADER_DEBUG] */

#define LOADER_D_LOG_ENABLE                     (1)
#define LOADER_D_DBG_FUNCTRACE                  (0)

#define LOADER_D_APPID                          (0x80UL + 0x20UL)
#define LOADER_D_MID_LOADER                     (0x90)

#define LOADER_ERRCODE_DEF(errid)               (HI_S32)(((LOADER_D_APPID) << 24) | ((LOADER_D_MID_LOADER) << 16) | (errid))

#define LOADER_ERR_MEM                          LOADER_ERRCODE_DEF(0x101)      /**< Memory operate error */
#define LOADER_ERR_SEM                          LOADER_ERRCODE_DEF(0x102)      /**< Semaphore operate error */
#define LOADER_ERR_FILE                         LOADER_ERRCODE_DEF(0x103)      /**< File operate error */
#define LOADER_ERR_LOCK                         LOADER_ERRCODE_DEF(0x104)      /**< Lock operate error */
#define LOADER_ERR_PARAM                        LOADER_ERRCODE_DEF(0x105)      /**< invalid parameter */
#define LOADER_ERR_TIMER                        LOADER_ERRCODE_DEF(0x106)      /**< Timer error */
#define LOADER_ERR_THREAD                       LOADER_ERRCODE_DEF(0x107)      /**< thread operate error */
#define LOADER_ERR_TIMEOUT                      LOADER_ERRCODE_DEF(0x108)      /**< timeout */
#define LOADER_ERR_DEVICE                       LOADER_ERRCODE_DEF(0x109)      /**< device exception */
#define LOADER_ERR_STATUS                       LOADER_ERRCODE_DEF(0x110)      /**< status  exception */
#define LOADER_ERR_IOCTRL                       LOADER_ERRCODE_DEF(0x111)      /**< IO operate exception */
#define LOADER_ERR_INUSE                        LOADER_ERRCODE_DEF(0x112)      /**< In use */
#define LOADER_ERR_EXIST                        LOADER_ERRCODE_DEF(0x113)      /**< Already exist */
#define LOADER_ERR_NOEXIST                      LOADER_ERRCODE_DEF(0x114)      /**< Not exist */
#define LOADER_ERR_UNSUPPORTED                  LOADER_ERRCODE_DEF(0x115)      /**< Unsupported */
#define LOADER_ERR_UNAVAILABLE                  LOADER_ERRCODE_DEF(0x116)      /**< unavalilable */
#define LOADER_ERR_UNINITED                     LOADER_ERRCODE_DEF(0x117)      /**< No initialization */
#define LOADER_ERR_DATABASE                     LOADER_ERRCODE_DEF(0x118)      /**< Database operate error */
#define LOADER_ERR_OVERFLOW                     LOADER_ERRCODE_DEF(0x119)      /**< Overflow */
#define LOADER_ERR_EXTERNAL                     LOADER_ERRCODE_DEF(0x120)      /**< external error */
#define LOADER_ERR_UNKNOWNED                    LOADER_ERRCODE_DEF(0x121)      /**< unknown error */
#define LOADER_ERR_FLASH                        LOADER_ERRCODE_DEF(0x122)      /**< Flash operate error*/
#define LOADER_ERR_ILLEGAL_IMAGE                LOADER_ERRCODE_DEF(0x123)      /**< The image need to be write to flash */

#if LOADER_D_LOG_ENABLE
#define LOADER_DEBUG(pszFormat...)              HI_DBG_PRINT(HI_ID_LOADER, pszFormat)
#define LOADER_INFO(pszFormat...)               HI_INFO_PRINT(HI_ID_LOADER, pszFormat)
#define LOADER_WARN(pszFormat...)               HI_WARN_PRINT(HI_ID_LOADER, pszFormat)
#define LOADER_ERROR(pszFormat...)              HI_ERR_PRINT(HI_ID_LOADER, pszFormat)
#define LOADER_FATAL(pszFormat...)              HI_FATAL_PRINT(HI_ID_LOADER, pszFormat)
#define LOADER_PRINT(pszFormat...)              HI_PRINT(pszFormat)
#else
#define LOADER_DEBUG(pszFormat...)
#define LOADER_INFO(pszFormat...)
#define LOADER_WARN(pszFormat...)
#define LOADER_ERROR(pszFormat...)
#define LOADER_FATAL(pszFormat...)
#define LOADER_PRINT(pszFormat...)
#endif

#define LOADER_PrintFuncErr(Func, ErrCode)      LOADER_ERROR("Call [%s] return [0x%08x]\n", #Func, ErrCode);
#define LOADER_PrintErrCode(ErrCode)            LOADER_ERROR("Error Code: [0x%08x]\n", ErrCode);
#define LOADER_PrintErrInfo(ErrInfo)            LOADER_ERROR("%s\n", ErrInfo);

#define LOADER_PrintS32(val)                    LOADER_DEBUG("%s = %d\n",    #val, val)
#define LOADER_PrintU32(val)                    LOADER_DEBUG("%s = %u\n",    #val, val)
#define LOADER_PrintS64(val)                    LOADER_DEBUG("%s = %lld\n",  #val, val)
#define LOADER_PrintU64(val)                    LOADER_DEBUG("%s = %llu\n",  #val, val)
#define LOADER_PrintStr(val)                    LOADER_DEBUG("%s = %s\n",    #val, val)
#define LOADER_PrintVoid(val)                   LOADER_DEBUG("%s = %p\n",    #val, val)
#define LOADER_Print32Hex(val)                  LOADER_DEBUG("%s = 0x%08x\n",#val, val)
#define LOADER_Print64Hex(val)                  LOADER_DEBUG("%s = 0x%016llx\n", #val, val)
#define LOADER_PrintFloat(val)                  LOADER_DEBUG("%s = %f\n",    #val, val)
#define LOADER_PrintInfo(val)                   LOADER_DEBUG("%s \n", val)

#define LOADER_CHECK(func)                                  \
    {                                                       \
        HI_S32 s32iErrCode = func;                          \
        if (HI_SUCCESS != s32iErrCode)                      \
        {                                                   \
            LOADER_PrintFuncErr(func, s32iErrCode);         \
        }                                                   \
    }

#define LOADER_CHECK_PARAM(val)                             \
    {                                                       \
        if (val)                                            \
        {                                                   \
            LOADER_PrintErrCode(LOADER_ERR_PARAM);          \
            return LOADER_ERR_PARAM;                        \
        }                                                   \
    }

#define LOADER_CHECK_INITED(u32InitCount)                   \
    {                                                       \
        if (0 == (u32InitCount))                            \
        {                                                   \
            LOADER_PrintErrCode(LOADER_ERR_UNINITED);       \
            return LOADER_ERR_UNINITED;                     \
        }                                                   \
    }

#define LOADER_PrintBuffer(pcInfo, pu8Input, u32Length)     \
    {                                                       \
        HI_U32 u32ii = 0;                                   \
        LOADER_PrintInfo(pcInfo);                           \
        for (u32ii = 0; u32ii < (u32Length); u32ii++)       \
        {                                                   \
            if((u32ii % 16 == 0)                            \
               && (u32ii != 0))                             \
            {                                               \
                LOADER_PRINT("\n");                         \
            }                                               \
            LOADER_PRINT("0x%02x ", (pu8Input)[u32ii]);     \
        }                                                   \
        LOADER_PRINT("\n");                                 \
    }

#if LOADER_D_DBG_FUNCTRACE
#define LOADER_DBG_FuncEnter()                  LOADER_INFO(" =====>[Enter]\n")
#define LOADER_DBG_FuncExit()                   LOADER_INFO(" =====>[Exit]\n")
#else
#define LOADER_DBG_FuncEnter()
#define LOADER_DBG_FuncExit()
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

#endif/*__LOADER_DEBUG_H__*/
