/******************************************************************************
Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name       : ui_keyled.h
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

#ifndef __UI_KEYLED_H__
#define __UI_KEYLED_H__

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
/** \addtogroup      UI_KEYLED */
/** @{ */  /** <!-- [UI_KEYLED] */

/****** Macro switch definition ***********/

/****** Macro constants definition ********/

/****** Macro Functions definition ********/

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      UI_KEYLED */
/** @{ */  /** <!-- [UI_KEYLED] */

/****** Enumeration definition ************/

typedef enum hi_UI_KEYVALUE_E
{
    UI_E_KEYVALUE_NUM0    = 0x7887ff00,
    UI_E_KEYVALUE_NUM1    = 0x6d92ff00,
    UI_E_KEYVALUE_NUM2    = 0x6c93ff00,
    UI_E_KEYVALUE_NUM3    = 0x33ccff00,
    UI_E_KEYVALUE_NUM4    = 0x718eff00,
    UI_E_KEYVALUE_NUM5    = 0x708fff00,
    UI_E_KEYVALUE_NUM6    = 0x37c8ff00,
    UI_E_KEYVALUE_NUM7    = 0x758aff00,
    UI_E_KEYVALUE_NUM8    = 0x748bff00,
    UI_E_KEYVALUE_NUM9    = 0x3bc4ff00,
    UI_E_KEYVALUE_UP      = 0x35caff00,
    UI_E_KEYVALUE_DOWN    = 0x2dd2ff00,
    UI_E_KEYVALUE_LEFT    = 0x6699ff00,
    UI_E_KEYVALUE_RIGHT   = 0x3ec1ff00,
    UI_E_KEYVALUE_MENU    = 0x629dff00,
    UI_E_KEYVALUE_OK      = 0x31ceff00,
    UI_E_KEYVALUE_EXIT    = 0x6f90ff00,
    UI_E_KEYVALUE_UNKNOWN,

} UI_KEYVALUE_E;

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      UI_KEYLED */
/** @{ */  /** <!-- [UI_KEYLED] */

HI_S32 UI_KEYLED_Init(HI_VOID);

HI_S32 UI_KEYLED_DeInit(HI_VOID);

HI_S32 UI_KEYLED_GetInput(HI_U32* pu32KeyValue);

HI_S32 UI_KEYLED_Display(HI_CHAR* pcString);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /* __UI_KEYLED_H__ */
