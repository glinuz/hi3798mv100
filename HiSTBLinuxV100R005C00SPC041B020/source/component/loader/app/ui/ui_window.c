/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_window.c
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

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_go_bliter.h"
#include "hi_go_surface.h"
#include "hi_go_gdev.h"
#include "com_debug.h"
#include "com_osal.h"
#include "ui_gfx.h"
#include "ui_window.h"

#ifdef HI_LOADER_APPLOADER
#include "hi_go_text.h"
#include "hi_unf_common.h"
#include "ui_display.h"
#else
#include "hi_unf_disp.h"
#include "ui_text.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#define UI_D_WINDOW_TITLE_BORDER            (2)
#define UI_D_WINDOW_TITLE_HEIGHT            (35)
#define UI_D_WINDOW_TITLE_TEXT_POSX         (20)
#define UI_D_WINDOW_TITLE_TEXT_POSY         (5)
#define UI_D_WINDOW_TITLE_COLOR_BG          GET_COLOR(0xFF, 0x14, 0x3D, 0x67)

#define UI_D_WIDGHT_COLOR_TEXT              GET_COLOR(0xFF, 0xFF, 0xFF, 0xFF)

#define UI_D_WIDGHT_COLOR_FRAME             UI_D_WINDOW_COLOR_BG
#define UI_D_WIDGHT_COLOR_FRAME_FOCUS       GET_COLOR(0xFF, 0xFF, 0xFF, 0x00)
#define UI_D_WIDGHT_COLOR_BKGROUND          GET_COLOR(0xFF, 0x10, 0x20, 0x35)
#define UI_D_WIDGHT_COLOR_BKGROUND_FOCUS    UI_D_WIDGHT_COLOR_BKGROUND

#define UI_D_BUTTON_COLOR_FRAME             GET_COLOR(0xFF, 0x9A, 0x9A, 0x9A)
#define UI_D_BUTTON_COLOR_FRAME_FOCUS       UI_D_WIDGHT_COLOR_FRAME_FOCUS
#define UI_D_BUTTON_COLOR_BKGROUND          GET_COLOR(0xFF, 0x14, 0x3D, 0x67)
#define UI_D_BUTTON_COLOR_BKGROUND_FOCUS    GET_COLOR(0xFF, 36, 160, 255)

#define UI_D_CURSOR_COLOR                   GET_COLOR(0xFF, 0xFF, 0xFF, 0xFF)
#define UI_D_CURSOR_HEIGHT                  (2)

#define UI_WIN_GetInstance(hWindow)        ((UI_WINDOW_S *)(hWindow))

static HI_S32 winConverNumKeyToAsii(UI_KEYVALUE_E enKeyValue, HI_S8* ps8Ascii)
{
    switch (enKeyValue)
    {
        case UI_E_KEYVALUE_NUM0:
        {
            *ps8Ascii = 0x30;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM1:
        {
            *ps8Ascii = 0x31;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM2:
        {
            *ps8Ascii = 0x32;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM3:
        {
            *ps8Ascii = 0x33;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM4:
        {
            *ps8Ascii = 0x34;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM5:
        {
            *ps8Ascii = 0x35;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM6:
        {
            *ps8Ascii = 0x36;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM7:
        {
            *ps8Ascii = 0x37;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM8:
        {
            *ps8Ascii = 0x38;
            return HI_SUCCESS;
        }

        case UI_E_KEYVALUE_NUM9:
        {
            *ps8Ascii = 0x39;
            return HI_SUCCESS;
        }

        default:
        {
            break;
        }
    }

    return HI_FAILURE;
}

static HI_S32 winDrawCursor(HI_HANDLE hSurface, UI_ELEMENT_S* pstElement)
{
    HI_S32 s32X = 0;
    HI_S32 s32Y = 0;
    HI_S32 s32W = 0;
    HI_S32 s32H = 0;
    HI_S8  au8Text[UI_D_WIDGHT_TEXT_MAXLEN] = {0};
    HI_S8* ps8Content = pstElement->astContent[pstElement->u32ContentFocus].acText;

    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = UI_GetTextExtent(ps8Content, &s32W, &s32H);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_GetTextExtent, s32Ret);
        return s32Ret;
    }

    s32X = pstElement->s32PosX + (pstElement->s32Width - s32W) / 2;
    s32Y = pstElement->s32PosY + (pstElement->s32Height - s32H) / 2 + s32H;

    if (0 != pstElement->u32CursorPos)
    {
        COM_StrNCpy(au8Text, ps8Content, pstElement->u32CursorPos);

        s32Ret = UI_GetTextExtent(au8Text, &s32W, &s32H);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HI_GO_GetTextExtent, s32Ret);
            return s32Ret;
        }

        s32X += s32W;
    }

    COM_MemSet(au8Text, 0x00, sizeof(au8Text));
    au8Text[0] = ps8Content[pstElement->u32CursorPos];
    s32Ret = UI_GetTextExtent(au8Text, &s32W, &s32H);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_GetTextExtent, s32Ret);
        return s32Ret;
    }

    s32Ret = UI_FillRect(hSurface, s32X, s32Y, s32W, UI_D_CURSOR_HEIGHT, UI_D_CURSOR_COLOR);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_FillRect, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 winElementCompose(UI_WINDOW_S* pstWindow)
{
    HI_S32 s32X = 0;
    HI_S32 s32Y = 0;
    HI_S32 s32H = 0;
    HI_S32 s32W = 0;
    HI_U32 u32BKColor = 0;
    HI_U32 u32FrameColor = 0;
    HI_CHAR* pcText = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = pstWindow->pstElementList;

    HI_S32 s32Ret = HI_FAILURE;

    while (HI_NULL_PTR != pstElement)
    {
        if (HI_TRUE == pstElement->bFocused)
        {
            u32BKColor = pstElement->u32BGFocusColor;
            u32FrameColor = pstElement->u32FrameFocusColor;
        }
        else
        {
            u32BKColor = pstElement->u32BGColor;
            u32FrameColor = pstElement->u32FrameColor;
        }

        s32X = pstElement->s32PosX;
        s32Y = pstElement->s32PosY;
        s32W = pstElement->s32Width;
        s32H = pstElement->s32Height;
        COM_CHECK(UI_FillRect(pstWindow->hMemSurface, s32X, s32Y, s32W, s32H, u32BKColor));
        COM_CHECK(UI_DrawRect(pstWindow->hMemSurface, s32X, s32Y, s32W, s32H, u32FrameColor));

        pcText = pstElement->astContent[pstElement->u32ContentFocus].acText;
        COM_CHECK(UI_DrawTextEx(pstWindow->hMemSurface, pcText, s32X, s32Y, s32W, s32H, HIGO_LAYOUT_VCENTER | HIGO_LAYOUT_HCENTER));

        switch (pstElement->enElementType)
        {
            case UI_E_ELEMENT_TYPE_INPUT:
            {
                if (HI_TRUE == pstElement->bFocused)
                {
                    if (0 != pstElement->u32ContentCount)
                    {
                        s32Ret = winDrawCursor(pstWindow->hMemSurface, pstElement);
                        if (HI_SUCCESS != s32Ret)
                        {
                            COM_PrintFuncErr(winDrawCursor, s32Ret);
                            return s32Ret;
                        }

                    }
                }

                break;
            }

            case UI_E_ELEMENT_TYPE_COMBOX:
            {
                s32Ret = UI_GetTextExtent("<", &s32W, &s32H);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_PrintFuncErr(UI_GetTextExtent, s32Ret);
                    return s32Ret;
                }

                s32X = pstElement->s32PosX - s32W * 2;
                s32Y = pstElement->s32PosY + (pstElement->s32Height - s32H) / 2;
                COM_CHECK(UI_DrawText(pstWindow->hMemSurface, "<", s32X, s32Y));

                s32X = pstElement->s32PosX + pstElement->s32Width + s32W;
                COM_CHECK(UI_DrawText(pstWindow->hMemSurface, ">", s32X, s32Y));

                break;
            }

            default:
            {
                break;
            }
        }


        pstElement = pstElement->pstNext;
    }

    return HI_SUCCESS;
}


static HI_S32 winElementSwitch(UI_WINDOW_S* pstWindow, UI_ELEMENT_S* pstFocusElement, UI_KEYVALUE_E enKeyValue)
{
    UI_ELEMENT_S* pstNextMember = HI_NULL_PTR;

    switch (enKeyValue)
    {
        case UI_E_KEYVALUE_RIGHT:
        case UI_E_KEYVALUE_DOWN:
        {
            pstFocusElement->bFocused = HI_FALSE;

            if (HI_NULL_PTR == pstFocusElement->pstNext)
            {
                pstWindow->pstElementList->bFocused = HI_TRUE;
            }
            else
            {
                pstFocusElement->pstNext->bFocused = HI_TRUE;
            }

            break;
        }

        case UI_E_KEYVALUE_LEFT:
        case UI_E_KEYVALUE_UP:
        {
            pstNextMember = pstWindow->pstElementList;

            if (pstWindow->pstElementList == pstFocusElement)
            {
                while (HI_NULL_PTR != pstNextMember->pstNext)
                {
                    pstNextMember = pstNextMember->pstNext;
                }
            }
            else
            {
                while (pstFocusElement != pstNextMember->pstNext)
                {
                    pstNextMember = pstNextMember->pstNext;
                }

            }

            pstFocusElement->bFocused = HI_FALSE;
            pstNextMember->bFocused = HI_TRUE;

            break;
        }

        default:
        {
            break;
        }
    }

    return HI_SUCCESS;
}

static UI_ELEMENT_S* winGetElement(UI_WINDOW_S* pstWindow, HI_U32 u32ElementID)
{
    UI_ELEMENT_S* pstElement = pstWindow->pstElementList;

    while (HI_NULL_PTR != pstElement)
    {
        if (u32ElementID == pstElement->u32ElementID)
        {
            return pstElement;
        }

        pstElement = pstElement->pstNext;
    }

    return HI_NULL_PTR;
}

static HI_S32 winAddElement(UI_WINDOW_S* pstWindow, UI_ELEMENT_S* pstElement)
{
    UI_ELEMENT_S* pstElementIns = pstWindow->pstElementList;

    if (HI_NULL_PTR == pstElementIns)
    {
        pstWindow->pstElementList = pstElement;
        pstElement->bFocused = HI_TRUE;
    }
    else
    {
        while (HI_NULL_PTR != pstElementIns->pstNext)
        {
            pstElementIns = pstElementIns->pstNext;
        }

        pstElementIns->pstNext = pstElement;
    }

    return HI_SUCCESS;
}

static HI_S32 winDrawTitle(UI_WINDOW_S* pstWindow)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL_PTR == pstWindow->pcTitle)
    {
        return HI_SUCCESS;
    }

    s32Ret = UI_FillRect(pstWindow->hMemSurface,
                         UI_D_WINDOW_TITLE_BORDER,
                         UI_D_WINDOW_TITLE_BORDER,
                         pstWindow->s32Width - UI_D_WINDOW_TITLE_BORDER * 2,
                         UI_D_WINDOW_TITLE_HEIGHT,
                         UI_D_WINDOW_TITLE_COLOR_BG);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_FillRect, s32Ret);
        return s32Ret;
    }

    s32Ret = UI_DrawText(pstWindow->hMemSurface,
                         pstWindow->pcTitle,
                         UI_D_WINDOW_TITLE_TEXT_POSX,
                         UI_D_WINDOW_TITLE_TEXT_POSY);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_DrawText, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementCreate
(
    HI_HANDLE           hWindow,
    HI_U32              u32ElementID,
    UI_ELEMENT_TYPE_E   enElementType,
    HI_S32              s32PosX,
    HI_S32              s32PosY,
    HI_S32              s32Width,
    HI_S32              s32Height
)
{
    UI_ELEMENT_S* pstElementIns = HI_NULL_PTR;
    UI_WINDOW_S*  pstWindow = (UI_WINDOW_S*)hWindow;

    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstWindow);
    COM_CHECK_PARAM(s32PosX > pstWindow->s32Width);
    COM_CHECK_PARAM(s32PosY > pstWindow->s32Height);

    pstElementIns = COM_MemMalloc(sizeof(UI_ELEMENT_S));
    if (HI_NULL_PTR == pstElementIns)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pstElementIns, 0x00, sizeof(UI_ELEMENT_S));
    pstElementIns->u32ElementID  = u32ElementID;
    pstElementIns->enElementType = enElementType;
    pstElementIns->s32PosX       = s32PosX;
    pstElementIns->s32PosY       = s32PosY;
    pstElementIns->s32Width      = s32Width;
    pstElementIns->s32Height     = s32Height;
    pstElementIns->u32MaxLength  = UI_D_WIDGHT_TEXT_MAXLEN;
    pstElementIns->u32CursorPos = 0;
    pstElementIns->pstNext = HI_NULL_PTR;

    if (UI_E_ELEMENT_TYPE_BUTTOM == enElementType)
    {
        pstElementIns->u32BGColor           = UI_D_BUTTON_COLOR_BKGROUND;
        pstElementIns->u32BGFocusColor      = UI_D_BUTTON_COLOR_BKGROUND_FOCUS;
        pstElementIns->u32FrameColor        = UI_D_BUTTON_COLOR_FRAME;
        pstElementIns->u32FrameFocusColor   = UI_D_BUTTON_COLOR_FRAME_FOCUS;
    }
    else
    {
        pstElementIns->u32BGColor           = UI_D_WIDGHT_COLOR_BKGROUND;
        pstElementIns->u32BGFocusColor      = UI_D_WIDGHT_COLOR_BKGROUND_FOCUS;
        pstElementIns->u32FrameColor        = UI_D_WIDGHT_COLOR_FRAME;
        pstElementIns->u32FrameFocusColor   = UI_D_WIDGHT_COLOR_FRAME_FOCUS;
    }

    s32Ret = winAddElement(pstWindow, pstElementIns);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_CreateSurface, s32Ret);
        COM_MemFreeSetNull(pstElementIns);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


HI_S32 UI_WIN_ElementDestory(UI_ELEMENT_S* pstWinMember)
{
    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstWinMember);

    COM_MemFreeSetNull(pstWinMember);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementSetInputCount(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32MaxLength)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(0 == u32MaxLength);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = winGetElement(pstWindow, u32ElementID);
    if (HI_NULL_PTR == pstElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    pstElement->u32MaxLength = u32MaxLength;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementAddContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32Value, HI_CHAR* pcText)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pcText);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = winGetElement(pstWindow, u32ElementID);
    if (HI_NULL_PTR == pstElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    if (UI_D_COMBOX_OPTION_MAXCOUNT <= pstElement->u32ContentCount)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    pstElement->astContent[pstElement->u32ContentCount].u32Value = u32Value;
    COM_StrNCpy(pstElement->astContent[pstElement->u32ContentCount].acText, pcText, UI_D_WIDGHT_TEXT_MAXLEN);

    pstElement->u32ContentCount++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementSetContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32 u32Value, HI_CHAR* pcText)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pcText);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = winGetElement(pstWindow, u32ElementID);
    if (HI_NULL_PTR == pstElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    if (0 == pstElement->u32ContentCount)
    {
        pstElement->u32ContentCount = 1;
        pstElement->u32ContentFocus = 0;
    }

    pstElement->astContent[pstElement->u32ContentFocus].u32Value = u32Value;
    COM_StrNCpy(pstElement->astContent[pstElement->u32ContentFocus].acText, pcText, UI_D_WIDGHT_TEXT_MAXLEN - 1);


    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementGetContent(HI_HANDLE hWindow, HI_U32 u32ElementID, HI_U32* pu32Value, HI_CHAR** ppcText)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu32Value);
    COM_CHECK_PARAM(HI_NULL_PTR == ppcText);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = winGetElement(pstWindow, u32ElementID);
    if (HI_NULL_PTR == pstElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    if (0 == pstElement->u32ContentCount)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    *pu32Value = pstElement->astContent[pstElement->u32ContentFocus].u32Value;
    *ppcText = pstElement->astContent[pstElement->u32ContentFocus].acText;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_ElementClearContents(HI_HANDLE hWindow, HI_U32 u32ElementID)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = winGetElement(pstWindow, u32ElementID);
    if (HI_NULL_PTR == pstElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    pstElement->u32ContentCount = 0;
    pstElement->u32ContentFocus = 0;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_GetFocusElement(HI_HANDLE hWindow, HI_U32* pu32ElementID)
{
    UI_ELEMENT_S* pstFocusElement = HI_NULL_PTR;
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu32ElementID);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstFocusElement = pstWindow->pstElementList;
    if (HI_NULL_PTR == pstFocusElement)
    {
        COM_PrintErrCode(COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    while (HI_TRUE != pstFocusElement->bFocused)
    {
        pstFocusElement = pstFocusElement->pstNext;
    }

    if (HI_NULL_PTR == pstFocusElement)
    {
        COM_PrintErrCode(COM_ERR_STATUS);
        return COM_ERR_STATUS;
    }

    *pu32ElementID = pstFocusElement->u32ElementID;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_GetSurface(HI_HANDLE hWindow, HI_HANDLE* phSurface)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == phSurface);

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    *phSurface = pstWindow->hMemSurface;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


HI_S32 UI_WIN_KeyResolution(HI_HANDLE hWindow, UI_KEYVALUE_E enKeyValue)
{
    UI_ELEMENT_S* pstFocusElement = HI_NULL_PTR;
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstFocusElement = pstWindow->pstElementList;

    if (HI_NULL_PTR == pstFocusElement)
    {
        return HI_SUCCESS;
    }

    while (HI_TRUE != pstFocusElement->bFocused)
    {
        pstFocusElement = pstFocusElement->pstNext;
    }

    if (HI_NULL_PTR == pstFocusElement)
    {
        pstFocusElement = pstWindow->pstElementList;
    }

    if ((UI_E_KEYVALUE_UP == enKeyValue)
        || (UI_E_KEYVALUE_DOWN == enKeyValue))
    {
        s32Ret = winElementSwitch(pstWindow, pstFocusElement, enKeyValue);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(winElementSwitch, s32Ret);
            return s32Ret;
        }

        COM_CHECK(UI_WIN_Show(hWindow));

        return HI_SUCCESS;
    }

    if (UI_E_ELEMENT_TYPE_COMBOX == pstFocusElement->enElementType)
    {
        if (UI_E_KEYVALUE_RIGHT == enKeyValue)
        {
            pstFocusElement->u32ContentFocus = (pstFocusElement->u32ContentFocus + 1) % pstFocusElement->u32ContentCount;
        }
        else if (UI_E_KEYVALUE_LEFT == enKeyValue)
        {
            pstFocusElement->u32ContentFocus = (pstFocusElement->u32ContentFocus + pstFocusElement->u32ContentCount - 1) % pstFocusElement->u32ContentCount;
        }

        COM_CHECK(UI_WIN_Show(hWindow));
        return HI_SUCCESS;
    }

    if (UI_E_ELEMENT_TYPE_INPUT == pstFocusElement->enElementType)
    {
        if ((UI_E_KEYVALUE_NUM0 == enKeyValue)
            || (UI_E_KEYVALUE_NUM1 == enKeyValue)
            || (UI_E_KEYVALUE_NUM2 == enKeyValue)
            || (UI_E_KEYVALUE_NUM3 == enKeyValue)
            || (UI_E_KEYVALUE_NUM4 == enKeyValue)
            || (UI_E_KEYVALUE_NUM5 == enKeyValue)
            || (UI_E_KEYVALUE_NUM6 == enKeyValue)
            || (UI_E_KEYVALUE_NUM7 == enKeyValue)
            || (UI_E_KEYVALUE_NUM8 == enKeyValue)
            || (UI_E_KEYVALUE_NUM9 == enKeyValue))
        {
            HI_S8 s8ASCII = 0;

            s32Ret = winConverNumKeyToAsii(enKeyValue, &s8ASCII);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(winConverNumKeyToAsii, s32Ret);
                return s32Ret;
            }

            pstFocusElement->astContent[pstFocusElement->u32ContentFocus].acText[pstFocusElement->u32CursorPos] = s8ASCII;
            pstFocusElement->u32CursorPos = (pstFocusElement->u32CursorPos + 1) % pstFocusElement->u32MaxLength;

            COM_CHECK(UI_WIN_Show(hWindow));
            return HI_SUCCESS;
        }
    }

    return HI_SUCCESS;
}

HI_S32 UI_WIN_Show(HI_U32 hWindow)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = winElementCompose(pstWindow);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(winElementCompose, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = UI_SurfaceBlit(pstWindow->hMemSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_SurfaceBlit, s32Ret);
        return s32Ret;
    }

    s32Ret = UI_ScreenRefresh();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_ScreenRefresh, s32Ret);
        return HI_FAILURE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_WIN_Hide(HI_U32 hWindow)
{
    HI_S32 s32Ret = HI_FAILURE;

    s32Ret = UI_ScreenClean();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_ScreenClean, s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


HI_HANDLE UI_WIN_Create(HI_S32 s32PosX, HI_S32 s32PosY, HI_S32 s32Width, HI_S32 s32Height, HI_CHAR* pcTitle)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    HI_HANDLE hMemSurface = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = UI_SurfaceCreate(s32Width, s32Height, &hMemSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_SurfaceCreate, s32Ret);
        return HI_NULL_PTR;
    }

    pstWindow = COM_MemMalloc(sizeof(UI_WINDOW_S));
    if (HI_NULL_PTR == pstWindow)
    {
        HI_GO_FreeSurface(hMemSurface);
        COM_PrintErrCode(COM_ERR_MEM);
        return HI_NULL_PTR;
    }

    COM_MemSet(pstWindow, 0, sizeof(UI_WINDOW_S));
    pstWindow->hWindow     = (HI_HANDLE)pstWindow;
    pstWindow->s32PosX     = s32PosX;
    pstWindow->s32PosY     = s32PosY;
    pstWindow->s32Width    = s32Width;
    pstWindow->s32Height   = s32Height;
    pstWindow->hMemSurface = hMemSurface;
    pstWindow->pstElementList = HI_NULL_PTR;
    pstWindow->pcTitle     = pcTitle;

    COM_CHECK(UI_SetTextColor(UI_D_WIDGHT_COLOR_TEXT));
    COM_CHECK(UI_SetTextBGColor(UI_D_WINDOW_COLOR_BG));
    COM_CHECK(UI_FillRect(pstWindow->hMemSurface, 0, 0, s32Width, s32Height, UI_D_WINDOW_COLOR_BG));

    COM_CHECK(winDrawTitle(pstWindow));

    COM_DBG_FuncExit();
    return pstWindow->hWindow;
}

HI_S32 UI_WIN_Destory(HI_HANDLE hWindow)
{
    UI_WINDOW_S* pstWindow = HI_NULL_PTR;
    UI_ELEMENT_S* pstElement  = HI_NULL_PTR;
    UI_ELEMENT_S* pstElementTemp = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    pstWindow = UI_WIN_GetInstance(hWindow);
    if (HI_NULL_PTR == pstWindow)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    pstElement = pstWindow->pstElementList;

    while (HI_NULL_PTR != pstElement)
    {
        pstElementTemp = pstElement;
        pstElement  = pstElement->pstNext;
        COM_CHECK(UI_WIN_ElementDestory(pstElementTemp));
    }

    if (HI_NULL_PTR != pstWindow->hMemSurface)
    {
        COM_CHECK(UI_SurfaceDestory(pstWindow->hMemSurface));
        pstWindow->hMemSurface = HI_NULL_PTR;
    }

    COM_MemFreeSetNull(pstWindow);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


