/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : vmx_debug.h
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

#ifndef __VMX_DEBUG_H__
#define __VMX_DEBUG_H__

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
/** \addtogroup     VMX_DEBUG */
/** @{ */  /** <!-- [VMX_DEBUG] */

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define SSA_D_LOG_ENABLE                    (0)
#define SSA_D_DBG_FUNCTRACE                 (0)
#else
#define SSA_D_LOG_ENABLE                    (1)
#define SSA_D_DBG_FUNCTRACE                 (0)
#endif

#define SSA_D_APPID                         (0x80UL + 0x20UL)
#define SSA_D_MID_LOADER                    (0x90)

#define SSA_ERRCODE_DEF(errid)              (HI_S32)(((SSA_D_APPID) << 24) | ((SSA_D_MID_LOADER) << 16) |  (errid))

#define SSA_ERR_MEM                         SSA_ERRCODE_DEF(0x101)      /**< Memory operate error */
#define SSA_ERR_SEM                         SSA_ERRCODE_DEF(0x102)      /**< Semaphore operate error */
#define SSA_ERR_FILE                        SSA_ERRCODE_DEF(0x103)      /**< File operate error */
#define SSA_ERR_LOCK                        SSA_ERRCODE_DEF(0x104)      /**< Lock operate error */
#define SSA_ERR_PARAM                       SSA_ERRCODE_DEF(0x105)      /**< invalid parameter */
#define SSA_ERR_TIMER                       SSA_ERRCODE_DEF(0x106)      /**< Timer error */
#define SSA_ERR_THREAD                      SSA_ERRCODE_DEF(0x107)      /**< thread operate error */
#define SSA_ERR_TIMEOUT                     SSA_ERRCODE_DEF(0x108)      /**< timeout */
#define SSA_ERR_DEVICE                      SSA_ERRCODE_DEF(0x109)      /**< device exception */
#define SSA_ERR_STATUS                      SSA_ERRCODE_DEF(0x110)      /**< status  exception */
#define SSA_ERR_IOCTRL                      SSA_ERRCODE_DEF(0x111)      /**< IO operate exception */
#define SSA_ERR_INUSE                       SSA_ERRCODE_DEF(0x112)      /**< In use */
#define SSA_ERR_EXIST                       SSA_ERRCODE_DEF(0x113)      /**< Already exist */
#define SSA_ERR_NOEXIST                     SSA_ERRCODE_DEF(0x114)      /**< Not exist */
#define SSA_ERR_UNSUPPORTED                 SSA_ERRCODE_DEF(0x115)      /**< Unsupported */
#define SSA_ERR_UNAVAILABLE                 SSA_ERRCODE_DEF(0x116)      /**< unavalilable */
#define SSA_ERR_UNINITED                    SSA_ERRCODE_DEF(0x117)      /**< No initialization */
#define SSA_ERR_DATABASE                    SSA_ERRCODE_DEF(0x118)      /**< Database operate error */
#define SSA_ERR_OVERFLOW                    SSA_ERRCODE_DEF(0x119)      /**< Overflow */
#define SSA_ERR_EXTERNAL                    SSA_ERRCODE_DEF(0x120)      /**< external error */
#define SSA_ERR_UNKNOWNED                   SSA_ERRCODE_DEF(0x121)      /**< unknown error */
#define SSA_ERR_FLASH                       SSA_ERRCODE_DEF(0x122)      /**< Flash operate error*/
#define SSA_ERR_ILLEGAL_IMAGE               SSA_ERRCODE_DEF(0x123)      /**< The image need to be write to flash */

#if SSA_D_LOG_ENABLE
#define SSA_DEBUG(pszFormat...)             HI_DBG_PRINT(HI_ID_LOADER, pszFormat)
#define SSA_INFO(pszFormat...)              HI_INFO_PRINT(HI_ID_LOADER, pszFormat)
#define SSA_WARN(pszFormat...)              HI_WARN_PRINT(HI_ID_LOADER, pszFormat)
#define SSA_ERROR(pszFormat...)             HI_ERR_PRINT(HI_ID_LOADER, pszFormat)
#define SSA_FATAL(pszFormat...)             HI_FATAL_PRINT(HI_ID_LOADER, pszFormat)
#define SSA_PRINT(pszFormat...)             HI_PRINT(pszFormat)
#else
#define SSA_DEBUG(pszFormat...)
#define SSA_INFO(pszFormat...)
#define SSA_WARN(pszFormat...)
#define SSA_ERROR(pszFormat...)
#define SSA_FATAL(pszFormat...)
#define SSA_PRINT(pszFormat...)
#endif

#define SSA_PrintFuncErr(Func, ErrCode)     SSA_ERROR("Call [%s] return [0x%08x]\n", #Func, ErrCode);
#define SSA_PrintErrCode(ErrCode)           SSA_ERROR("Error Code: [0x%08x]\n", ErrCode);
#define SSA_PrintErrInfo(ErrInfo)           SSA_ERROR("%s\n", ErrInfo);

#define SSA_PrintS32(val)                   SSA_DEBUG("%s = %d\n",    #val, val)
#define SSA_PrintU32(val)                   SSA_DEBUG("%s = %u\n",    #val, val)
#define SSA_PrintS64(val)                   SSA_DEBUG("%s = %lld\n",  #val, val)
#define SSA_PrintU64(val)                   SSA_DEBUG("%s = %llu\n",  #val, val)
#define SSA_PrintStr(val)                   SSA_DEBUG("%s = %s\n",    #val, val)
#define SSA_PrintVoid(val)                  SSA_DEBUG("%s = %p\n",    #val, val)
#define SSA_Print32Hex(val)                 SSA_DEBUG("%s = 0x%08x\n",#val, val)
#define SSA_Print64Hex(val)                 SSA_DEBUG("%s = 0x%016llx\n", #val, val)
#define SSA_PrintFloat(val)                 SSA_DEBUG("%s = %f\n",    #val, val)
#define SSA_PrintInfo(val)                  SSA_DEBUG("%s\n", val)

#define SSA_CHECK(func)                                     \
    {                                                       \
        HI_S32 s32iErrCode = func;                          \
        if (HI_SUCCESS != s32iErrCode)                      \
        {                                                   \
            SSA_PrintFuncErr(func, s32iErrCode);            \
        }                                                   \
    }

#define SSA_CHECK_PARAM(val)                                \
    {                                                       \
        if (val)                                            \
        {                                                   \
            SSA_PrintErrCode(SSA_ERR_PARAM);                \
            return SSA_ERR_PARAM;                           \
        }                                                   \
    }

#define SSA_CHECK_INITED(u32InitCount)                      \
    {                                                       \
        if (0 == (u32InitCount))                            \
        {                                                   \
            SSA_PrintErrCode(SSA_ERR_UNINITED);             \
            return SSA_ERR_UNINITED;                        \
        }                                                   \
    }

#define SSA_PrintBuffer(pcInfo, pu8Input, u32Length)        \
    {                                                       \
        HI_U32 u32ii = 0;                                   \
        SSA_PrintInfo(pcInfo);                              \
        for (u32ii = 0; u32ii < (u32Length); u32ii++)       \
        {                                                   \
            if((u32ii % 16 == 0)                            \
               && (u32ii != 0))                             \
            {                                               \
                SSA_PRINT("\n");                            \
            }                                               \
            SSA_PRINT("0x%02x ", (pu8Input)[u32ii]);        \
        }                                                   \
        SSA_PRINT("\n");                                    \
    }

#if SSA_D_DBG_FUNCTRACE
#define SSA_DBG_FuncEnter()                 SSA_INFO(" =====>[Enter]\n")
#define SSA_DBG_FuncExit()                  SSA_INFO(" =====>[Exit]\n")
#else
#define SSA_DBG_FuncEnter()
#define SSA_DBG_FuncExit()
#endif

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup     VMX_DEBUG */
/** @{ */  /** <!-- [VMX_DEBUG] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @}*/  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup     VMX_DEBUG */
/** @{ */  /** <!-- [VMX_DEBUG] */

/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif/*__VMX_DEBUG_H__*/
