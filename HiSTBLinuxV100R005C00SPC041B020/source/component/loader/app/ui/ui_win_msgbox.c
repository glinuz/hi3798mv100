/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_win_msgbox.c
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
#include "com_osal.h"
#include "com_debug.h"
#include "ui_language.h"
#include "ui_gfx.h"
#include "ui_window.h"
#include "ui_win_msgbox.h"

#ifdef HI_LOADER_APPLOADER
#include "hi_go_comm.h"
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

/*************************** Macro Definition *********************************/

#define UI_D_MSGBOX_RECT_X                  (160)
#define UI_D_MSGBOX_RECT_Y                  (138)
#define UI_D_MSGBOX_RECT_W                  (400)
#define UI_D_MSGBOX_RECT_H                  (240)

#define UI_D_MSGBOX_CONTNT_POSY             (100)

/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/

static HI_HANDLE s_hWindow = HI_INVALID_HANDLE;

HI_S32 OSD_ShowMsgBox(HI_CHAR* pcTitle, HI_CHAR* pcContent, HI_S32 s32TimmMs)
{
    HI_HANDLE hSurface = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK_PARAM(HI_NULL_PTR == pcTitle);
    COM_CHECK_PARAM(HI_NULL_PTR == pcContent);

    if (HI_INVALID_HANDLE == s_hWindow)
    {
        s_hWindow = UI_WIN_Create(UI_D_MSGBOX_RECT_X, UI_D_MSGBOX_RECT_Y, UI_D_MSGBOX_RECT_W, UI_D_MSGBOX_RECT_H, pcTitle);
        if (HI_NULL_PTR == s_hWindow)
        {
            COM_PrintFuncErr(UI_WIN_Create, COM_ERR_DEVICE);
            return COM_ERR_DEVICE;
        }
    }

    s32Ret = UI_WIN_GetSurface(s_hWindow, &hSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
        UI_WIN_Destory(s_hWindow);
        return s32Ret;
    }

    COM_CHECK(UI_SetTextBGTransparent(HI_FALSE));
    COM_CHECK(UI_DrawTextEx(hSurface, pcContent, 0, UI_D_MSGBOX_CONTNT_POSY, UI_D_MSGBOX_RECT_W, UI_D_MSGBOX_RECT_H, HIGO_LAYOUT_HCENTER));
    COM_CHECK(UI_SetTextBGTransparent(HI_TRUE));
    COM_CHECK(UI_WIN_Show(s_hWindow));

    if (0 != s32TimmMs)
    {
        COM_Delayms(s32TimmMs);

        COM_CHECK(UI_WIN_Hide(s_hWindow));
        COM_CHECK(UI_WIN_Destory(s_hWindow));

        s_hWindow = HI_NULL_PTR;
    }

    return HI_SUCCESS;
}

HI_S32 OSD_HideMsgBox(HI_VOID)
{
    COM_CHECK_PARAM(HI_INVALID_HANDLE == s_hWindow);

    COM_CHECK(UI_WIN_Hide(s_hWindow));
    COM_CHECK(UI_WIN_Destory(s_hWindow));

    s_hWindow = HI_INVALID_HANDLE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


