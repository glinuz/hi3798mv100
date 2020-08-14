/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_osal.h
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

#ifndef __COM_OSAL_H__
#define __COM_OSAL_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      COM_OSAL */
/** @{ */  /** <!-- [COM_OSAL] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

#define COM_MemFreeSetNull(pBuf)        \
    {                                   \
        COM_MemFree(pBuf);              \
        pBuf = HI_NULL_PTR;             \
    }

#define COM_StrIsEmpty(str)     (HI_NULL == str[0])

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      COM_OSAL */
/** @{ */  /** <!-- [COM_OSAL] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/

/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      COM_OSAL */
/** @{ */  /** <!-- [COM_OSAL] */


HI_U32 COM_StrLen(const HI_CHAR* pcStr);

HI_S32 COM_StrNCmp(const HI_CHAR* pcStr1, const HI_CHAR* pcStr2, HI_U32 u32Count);

HI_U64 COM_Strtoul(const HI_CHAR* nptr, HI_CHAR** endptr, int base);

HI_S32 COM_StrToS32(const HI_CHAR* pcStr);

HI_U32 COM_StrToU32(const HI_CHAR* pcStr);

HI_S64 COM_StrToS64(const HI_CHAR* pcStr);

HI_U64 COM_StrToU64(const HI_CHAR* pcStr);

HI_CHAR* COM_StrStr(const HI_CHAR* pcStr1, const HI_CHAR* pcStr2);

HI_CHAR* COM_StrNCpy(HI_CHAR* pcDestination, const HI_CHAR* pcSource , HI_U32 u32Count);

HI_VOID* COM_MemCpy(HI_VOID* pDest, const HI_VOID* pSrc, HI_U32 u32Count);

HI_VOID* COM_MemSet(HI_VOID* pToSet, HI_S32 s32SetChar, HI_U32 u32Count);

HI_VOID* COM_MemMalloc(HI_U32 u32Size);

HI_VOID  COM_MemFree(HI_VOID* pBuf);

HI_VOID* COM_MemGetUsable(HI_U32 u32ApplySize, HI_U32* pu32Size);

HI_VOID  COM_MemFreeUsable(HI_VOID* ptr);

HI_VOID  COM_Delayms(HI_U32 u32TimeMS);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__COM_OSAL_H__*/
