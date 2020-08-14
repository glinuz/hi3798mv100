/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_gfx.c
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

#ifdef HI_LOADER_APPLOADER
#include "hi_go_text.h"
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

#define UI_D_SCREEN_WIDTH           (720)
#define UI_D_SCREEN_HEIGHT          (576)
#define UI_D_SCREEN_BGCOLOR         GET_COLOR(0xFF, 0x00, 0x00, 0x00)
#define UI_D_FONT_FILENAME          "./res/DroidSansFallbackLegacy.ttf"

static HI_HANDLE    s_hFont = HI_INVALID_HANDLE;
static HI_HANDLE    s_hLayer = HI_INVALID_HANDLE;
static HI_HANDLE    s_hLayerSurface = HI_INVALID_HANDLE;

static HI_S32       s_s32SufaceW    = UI_D_SCREEN_WIDTH;
static HI_S32       s_s32SufaceH    = UI_D_SCREEN_HEIGHT;
static HIGO_PF_E    s_enPixelFormat = HIGO_PF_8888;

HI_S32 UI_FillRect
(
    HI_HANDLE   hSurface,
    HI_S32      s32PosX,
    HI_S32      s32PosY,
    HI_S32      s32Width,
    HI_S32      s32Height,
    HI_U32      u32Color
)
{
    HI_RECT stRect;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);

    COM_MemSet(&stRect, 0x00, sizeof(stRect));
    stRect.x = s32PosX;
    stRect.y = s32PosY;
    stRect.w = s32Width;
    stRect.h = s32Height;
    s32Ret = HI_GO_FillRect(hSurface, &stRect, u32Color, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_FillRect, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_DrawRect
(
    HI_HANDLE   hSurface,
    HI_S32      s32PosX,
    HI_S32      s32PosY,
    HI_S32      s32Width,
    HI_S32      s32Height,
    HI_U32      u32Color
)
{
    HI_RECT stRect;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);

    COM_MemSet(&stRect, 0x00, sizeof(stRect));
    stRect.x = s32PosX;
    stRect.y = s32PosY;
    stRect.w = s32Width;
    stRect.h = s32Height;
    s32Ret = HI_GO_DrawRect(hSurface, &stRect, u32Color);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_DrawRect, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_DrawText(HI_HANDLE hSurface, HI_CHAR* pcContent, HI_S32 s32PosX, HI_S32 s32PosY)
{
    HI_RECT stRect;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);
    COM_CHECK_PARAM(HI_NULL_PTR == pcContent);

    COM_MemSet(&stRect, 0x00, sizeof(stRect));
    stRect.x = s32PosX;
    stRect.y = s32PosY;
    s32Ret = HI_GO_GetTextExtent(s_hFont, pcContent, &stRect.w, &stRect.h);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_GetTextExtent, s32Ret);
        return s32Ret;
    }

    s32Ret = HI_GO_TextOut(s_hFont, hSurface, pcContent, &stRect);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_TextOut, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_DrawTextEx(HI_HANDLE hSurface, HI_CHAR* pcContent, HI_S32 s32PosX, HI_S32 s32PosY, HI_S32 s32Width, HI_S32 s32Height, HI_S32 enAlignType)
{
    HI_RECT stRect;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);
    COM_CHECK_PARAM(HI_NULL_PTR == pcContent);

    COM_MemSet(&stRect, 0x00, sizeof(stRect));
    stRect.x = s32PosX;
    stRect.y = s32PosY;
    stRect.w = s32Width;
    stRect.h = s32Height;

    s32Ret = HI_GO_TextOutEx(s_hFont, hSurface, pcContent, &stRect, enAlignType);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_TextOut, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_GetTextExtent(HI_CHAR* pcText, HI_S32* ps32Width, HI_S32* ps32Height)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pcText);
    COM_CHECK_PARAM(HI_NULL_PTR == ps32Width);
    COM_CHECK_PARAM(HI_NULL_PTR == ps32Height);

    s32Ret = HI_GO_GetTextExtent(s_hFont, pcText, ps32Width, ps32Height);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_GetTextExtent, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_SetTextColor(HI_U32 u32Color)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_GO_SetTextColor(s_hFont, u32Color);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_SetTextBGColor, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_SetTextBGColor(HI_U32 u32Color)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_GO_SetTextBGColor(s_hFont, u32Color);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_SetTextBGColor, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_SetTextBGTransparent(HI_BOOL bTransparent)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_GO_SetTextBGTransparent(s_hFont, bTransparent);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_SetTextBGTransparent, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_ScreenRefresh(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_GO_RefreshLayer(s_hLayer, HI_NULL_PTR);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_RefreshLayer, COM_ERR_EXTERNAL);
        return COM_ERR_EXTERNAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_ScreenClean(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    s32Ret = HI_GO_FillRect(s_hLayerSurface, HI_NULL_PTR, UI_D_SCREEN_BGCOLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_FillRect, s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_GO_RefreshLayer(s_hLayer, HI_NULL_PTR);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_RefreshLayer, s32Ret);
        return HI_FAILURE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


HI_S32 UI_SurfaceCreate(HI_S32 s32Width, HI_S32 s32Height, HI_HANDLE* phSurface)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == phSurface);
    COM_CHECK_PARAM(0 == s32Width);
    COM_CHECK_PARAM(0 == s32Height);

    s32Ret = HI_GO_CreateSurface(s32Width, s32Height, s_enPixelFormat, phSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_CreateSurface, s32Ret);
        return COM_ERR_EXTERNAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_SurfaceDestory(HI_HANDLE hSurface)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);

    s32Ret = HI_GO_FreeSurface(hSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_CreateSurface, s32Ret);
        return COM_ERR_EXTERNAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_SurfaceBlit(HI_HANDLE hSurface)
{
    HI_S32 s32W = 0;
    HI_S32 s32H = 0;
    HI_RECT stRect;
    HIGO_BLTOPT_S stBltOpt;

    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == hSurface);

    s32Ret = HI_GO_GetSurfaceSize(hSurface, &s32W, &s32H);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_GetSurfaceSize, s32Ret);
        return COM_ERR_EXTERNAL;
    }

    stRect.w = (s_s32SufaceH * s32W) / UI_D_SCREEN_HEIGHT;
    stRect.h = (s_s32SufaceH * s32H) / UI_D_SCREEN_HEIGHT;
    stRect.x = (s_s32SufaceW - stRect.w) / 2;
    stRect.y = (s_s32SufaceH - stRect.h) / 2;

    COM_MemSet(&stBltOpt, 0x00, sizeof(stBltOpt));
    stBltOpt.EnableScale = HI_TRUE;

    s32Ret = HI_GO_Blit(hSurface, HI_NULL_PTR, s_hLayerSurface, &stRect, &stBltOpt);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_Blit, s32Ret);
        return COM_ERR_EXTERNAL;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_GraphicsInit(HI_UNF_PDM_DISP_PARAM_S* pstDispParam)
{
    HI_S32 s32Ret = HI_FAILURE;
    HIGO_LAYER_INFO_S stLayerInfo;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstDispParam);

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    COM_MemSet(&stLayerInfo, 0x00, sizeof(stLayerInfo));
    COM_CHECK(HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo));
    stLayerInfo.ScreenHeight   = (HI_S32)pstDispParam->u32VirtScreenHeight;
    stLayerInfo.ScreenWidth    = (HI_S32)pstDispParam->u32VirtScreenWidth;
    stLayerInfo.CanvasHeight   = (HI_S32)pstDispParam->u32VirtScreenHeight;
    stLayerInfo.CanvasWidth    = (HI_S32)pstDispParam->u32VirtScreenWidth;
    stLayerInfo.DisplayHeight  = (HI_S32)pstDispParam->u32VirtScreenHeight;
    stLayerInfo.DisplayWidth   = (HI_S32)pstDispParam->u32VirtScreenWidth;
    stLayerInfo.PixelFormat    = pstDispParam->enPixelFormat;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &s_hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_CreateLayer, s32Ret);
        COM_CHECK(HI_GO_Deinit());
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_GO_GetLayerSurface(s_hLayer, &s_hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_GetLayerSurface, s32Ret);
        COM_CHECK(HI_GO_DestroyLayer(s_hLayer));
        COM_CHECK(HI_GO_Deinit());
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_GO_GetSurfaceSize(s_hLayerSurface, &s_s32SufaceW, &s_s32SufaceH);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_GetSurfaceSize, s32Ret);
        COM_CHECK(HI_GO_DestroyLayer(s_hLayer));
        COM_CHECK(HI_GO_Deinit());
        return s32Ret;
    }

    s32Ret = HI_GO_CreateText(UI_D_FONT_FILENAME, HI_NULL_PTR,  &s_hFont);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_GO_CreateText, s32Ret);
        COM_CHECK(HI_GO_DestroyLayer(s_hLayer));
        COM_CHECK(HI_GO_Deinit());
        return s32Ret;
    }

    s_enPixelFormat = pstDispParam->enPixelFormat;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_GraphicsDeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK(HI_GO_DestroyText(s_hFont));
    COM_CHECK(HI_GO_DestroyLayer(s_hLayer));
    COM_CHECK(HI_GO_Deinit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


