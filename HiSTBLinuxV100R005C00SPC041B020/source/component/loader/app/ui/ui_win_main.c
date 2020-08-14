/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_win_main.c
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
#include "com_parse.h"
#include "com_debug.h"
#include "ui_language.h"
#include "ui_gfx.h"
#include "ui_window.h"
#include "ui_win_main.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
#define UI_D_UPGWIN_RECT_X              (160)
#define UI_D_UPGWIN_RECT_Y              (138)
#define UI_D_UPGWIN_RECT_W              (400)
#define UI_D_UPGWIN_RECT_H              (240)

#define UI_D_CONTENT_POSX               (20)

#define UI_D_PROGRESS_RECT_W            (300)
#define UI_D_PROGRESS_RECT_H            (20)

#define UI_D_PROGRESS_TEXT_NUM          (10)
#define UI_D_PROGRESS_TEXT_POSX         (UI_D_PROGRESS_RECT_W + 30)

#define UI_D_PROGRESS_COLOR             GET_COLOR(0xFF, 0xFF, 0xFF, 0x00)
#define UI_D_PROGRESS_BKCOLOR           GET_COLOR(0xFF, 0xC0, 0xC0, 0xC0)

#define UI_D_PROGRESS_DOWNLOAD_POSY     (135)
#define UI_D_PROGRESS_BURN_POSY         (195)




/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/

static HI_U32       s_u32DownloadProgress = 0;
static HI_U32       s_u32BurnProgress = 0;
static HI_HANDLE    s_hProgressWin = 0;

static HI_S32 uiShowProgress(HI_HANDLE hSurface, HI_U32 u32Percent, HI_S32 s32PosY)
{
    HI_S32  s32W = 0;
    HI_S32  s32H = 0;
    HI_U32  u32Length = 0;
    HI_CHAR acProgressText[UI_D_PROGRESS_TEXT_NUM] = {0};

    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK(UI_FillRect(hSurface, UI_D_CONTENT_POSX, s32PosY, UI_D_PROGRESS_RECT_W, UI_D_PROGRESS_RECT_H, UI_D_PROGRESS_BKCOLOR));

    if (0 != u32Percent)
    {
        u32Length = u32Percent * UI_D_PROGRESS_RECT_W / 100;
        COM_CHECK(UI_FillRect(hSurface, UI_D_CONTENT_POSX, s32PosY, (HI_S32)u32Length, UI_D_PROGRESS_RECT_H,  UI_D_PROGRESS_COLOR));
    }

    snprintf(acProgressText, sizeof(acProgressText), "%3u%%", u32Percent);

    s32Ret = UI_GetTextExtent(acProgressText, &s32W, &s32H);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
        return s32Ret;
    }

    if (UI_D_PROGRESS_RECT_H > s32H)
    {
        s32PosY += (UI_D_PROGRESS_RECT_H - s32H) / 2;
    }
    else
    {
        s32PosY -= (s32H - UI_D_PROGRESS_RECT_H) / 2;
    }

    COM_CHECK(UI_SetTextBGTransparent(HI_FALSE));
    COM_CHECK(UI_DrawText(hSurface, acProgressText, UI_D_PROGRESS_TEXT_POSX, s32PosY));
    COM_CHECK(UI_SetTextBGTransparent(HI_TRUE));

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


HI_S32 UI_ShowProgressWin(HI_VOID)
{
    HI_HANDLE hWindow = HI_NULL_PTR;
    HI_HANDLE hSurface = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    hWindow = UI_WIN_Create(UI_D_UPGWIN_RECT_X,
                            UI_D_UPGWIN_RECT_Y,
                            UI_D_UPGWIN_RECT_W,
                            UI_D_UPGWIN_RECT_H,
                            UI_LAN_UPGRADING);
    if (HI_NULL_PTR == hWindow)
    {
        COM_PrintFuncErr(UI_WIN_Create, COM_ERR_DEVICE);
        return COM_ERR_DEVICE;
    }

    s32Ret = UI_WIN_GetSurface(hWindow, &hSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
        return s32Ret;
    }

    COM_CHECK(UI_DrawText(hSurface, UI_LAN_PROMPT_POWER1, UI_D_CONTENT_POSX, 60));
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_DWNLD_PROGRESS, UI_D_CONTENT_POSX, 100));
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_BURN_PROGRESS, UI_D_CONTENT_POSX, 160));

    COM_CHECK(uiShowProgress(hSurface, 0, UI_D_PROGRESS_DOWNLOAD_POSY));
    COM_CHECK(uiShowProgress(hSurface, 0, UI_D_PROGRESS_BURN_POSY));

    s32Ret = UI_WIN_Show(hWindow);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_Show, s32Ret);
        UI_WIN_Destory(hWindow);
        return s32Ret;
    }

    s_hProgressWin = hWindow;

    COM_DBG_FuncEnter();
    return HI_SUCCESS;
}

HI_S32 UI_HideProgressWin(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = UI_WIN_Hide();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_Hide, s32Ret);
        return s32Ret;
    }

    s32Ret = UI_WIN_Destory(s_hProgressWin);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_Destory, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncEnter();
    return HI_SUCCESS;
}

HI_S32 UI_UpdateDowloadProgress(HI_U32 u32DownloadSize, HI_U32 u32TotalSize)
{
    HI_U32 u32Percent = 0;
    HI_HANDLE hSurface = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(0 == u32TotalSize);
    COM_CHECK_PARAM(u32DownloadSize > u32TotalSize);

    u32Percent = (HI_U32)(((HI_U64)u32DownloadSize * 100) / (HI_U64)u32TotalSize);

    if (s_u32DownloadProgress != u32Percent)
    {
        s32Ret = UI_WIN_GetSurface(s_hProgressWin, &hSurface);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
            return s32Ret;
        }

        COM_CHECK(uiShowProgress(hSurface, u32Percent, UI_D_PROGRESS_DOWNLOAD_POSY));

        COM_CHECK(UI_WIN_Show(s_hProgressWin));
        s_u32DownloadProgress = u32Percent;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_UpdateBurnProgress(HI_U32 u32Burnize, HI_U32 u32TotalSize)
{
    HI_U32 u32Percent = 0;
    HI_HANDLE hSurface = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(0 == u32TotalSize);
    COM_CHECK_PARAM(u32Burnize > u32TotalSize);

    u32Percent = (HI_U32)(((HI_U64)u32Burnize * 100) / (HI_U64)u32TotalSize);

    if (s_u32BurnProgress != u32Percent)
    {
        s32Ret = UI_WIN_GetSurface(s_hProgressWin, &hSurface);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
            return s32Ret;
        }

        COM_CHECK(uiShowProgress(hSurface, u32Percent, UI_D_PROGRESS_BURN_POSY));

        COM_CHECK(UI_WIN_Show(s_hProgressWin));
        s_u32BurnProgress = u32Percent;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


