/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_window.h
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

#ifndef __UI_WIDGET_H__
#define __UI_WIDGET_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "ui_keyled.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      UI_WIDGET */
/** @{ */  /** <!-- [UI_WIDGET] */

#define UI_D_WIDGHT_TEXT_MAXLEN             (16)
#define UI_D_COMBOX_OPTION_MAXCOUNT         (16)

#define GET_COLOR(a, r, g, b)       ((0xff000000 & ((HI_U32)(a) << 24)) \
                                     | (0x00ff0000 & ((HI_U32)(r) << 16)) \
                                     | (0x0000ff00 & ((HI_U32)(g) << 8)) \
                                     | (0x000000ff & ((HI_U32)b)))

#define UI_D_WINDOW_COLOR_TEXT      GET_COLOR(0xFF, 0xF8, 0xF8, 0xF8)
#define UI_D_WINDOW_COLOR_BG        GET_COLOR(0xFF, 0x1E, 0x57, 0x95)



/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      UI_WIDGET */
/** @{ */  /** <!-- [UI_WIDGET] */

/****** Enumeration definition ************/
typedef enum hi_UI_ELEMENT_TYPE_E
{
    UI_E_ELEMENT_TYPE_INPUT,        /**< Text box*/
    UI_E_ELEMENT_TYPE_BUTTOM,       /**< Button*/
    UI_E_ELEMENT_TYPE_COMBOX,       /**< Translation box*/
    UI_E_ELEMENT_TYPE_PASSWD,       /**< Password prompt*/
    UI_E_ELEMENT_TYPE_BUT

} UI_ELEMENT_TYPE_E;


/****** Structure definition **************/

typedef struct tagUI_CONTENT_S
{
    HI_U32      u32Value;
    HI_CHAR     acText[UI_D_WIDGHT_TEXT_MAXLEN];

} UI_CONTENT_S;

typedef struct tagUI_ELEMENT_S
{
    UI_ELEMENT_TYPE_E   enElementType;    /**< Window member type: message input or button*/
    HI_U32              u32ElementID;

    HI_S32              s32PosX;
    HI_S32              s32PosY;
    HI_S32              s32Width;
    HI_S32              s32Height;

    HI_U32              u32FrameColor;
    HI_U32              u32FrameFocusColor;
    HI_U32              u32BGColor;
    HI_U32              u32BGFocusColor;

    HI_BOOL             bFocused;
    HI_U32              u32MaxLength;               /**< Maximum characters length allowed, available for text boxes */
    HI_U32              u32CursorPos;               /**< Current focus index */

    HI_U32              u32ContentFocus;
    HI_U32              u32ContentCount;
    UI_CONTENT_S        astContent[UI_D_COMBOX_OPTION_MAXCOUNT];

    struct tagUI_ELEMENT_S* pstNext;                  /**< Pointed to the next member*/
} UI_ELEMENT_S;


typedef struct tagUI_WINDOW_S
{
    HI_HANDLE           hWindow;

    HI_S32              s32PosX;
    HI_S32              s32PosY;
    HI_S32              s32Width;
    HI_S32              s32Height;

    HI_CHAR*            pcTitle;
    HI_HANDLE           hMemSurface;            /**< Memory surface */
    UI_ELEMENT_S*       pstElementList;         /**< Element list */

} UI_WINDOW_S;


/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      UI_WIDGET */
/** @{ */  /** <!-- [UI_WIDGET] */

HI_S32 UI_WIN_Show(HI_U32 hWindow);

HI_S32 UI_WIN_ElementCreate
(
    HI_HANDLE           hWindow,
    HI_U32              u32ElementID,
    UI_ELEMENT_TYPE_E   enElementType,
    HI_S32              s32PosX,
    HI_S32              s32PosY,
    HI_S32              s32Width,
    HI_S32              s32Height
);

HI_S32 UI_WIN_ElementDestory(UI_ELEMENT_S* pstWinMember);

HI_S32 UI_WIN_ElementSetInputCount(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32MaxLength);

HI_S32 UI_WIN_ElementAddContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32Value, HI_CHAR* pcText);

HI_S32 UI_WIN_ElementSetContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32Value, HI_CHAR* pcText);

HI_S32 UI_WIN_ElementGetContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32* pu32Value, HI_CHAR** ppcText);

HI_S32 UI_WIN_ElementClearContents(HI_HANDLE hWindow, HI_U32 u32ElementID);

HI_S32 UI_WIN_GetFocusElement(HI_HANDLE hWindow, HI_U32* pu32ElementID);

HI_HANDLE UI_WIN_Create(HI_S32 s32PosX, HI_S32 s32PosY, HI_S32 s32Width, HI_S32 s32Height, HI_CHAR* pcTitle);

HI_S32 UI_WIN_Destory(HI_HANDLE hWindow);

HI_S32 UI_WIN_SetTextColor(HI_HANDLE hWindow, HI_U32 u32Color);

HI_S32 UI_WIN_SetTextBGColor(HI_HANDLE hWindow, HI_U32 u32Color);

HI_S32 UI_WIN_GetSurface(HI_HANDLE hWindow, HI_HANDLE* phSurface);

HI_S32 UI_WIN_KeyResolution(HI_HANDLE hWindow, UI_KEYVALUE_E enKeyValue);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__UI_WIDGET_H__*/
