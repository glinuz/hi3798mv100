/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_gfx.h
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

#ifndef __UI_GFX_H__
#define __UI_GFX_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_unf_pdm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      UI_GFX */
/** @{ */  /** <!-- [UI_GFX] */

#define GET_COLOR(a, r, g, b)       ((0xff000000 & ((HI_U32)(a) << 24)) \
                                     | (0x00ff0000 & ((HI_U32)(r) << 16)) \
                                     | (0x0000ff00 & ((HI_U32)(g) << 8)) \
                                     | (0x000000ff & ((HI_U32)b)))

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      UI_GFX */
/** @{ */  /** <!-- [UI_GFX] */

/****** Enumeration definition ************/

/****** Structure definition **************/

/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      UI_GFX */
/** @{ */  /** <!-- [UI_GFX] */

HI_S32 UI_FillRect
(
    HI_HANDLE   hSurface,
    HI_S32      s32PosX,
    HI_S32      s32PosY,
    HI_S32      s32Width,
    HI_S32      s32Height,
    HI_U32      u32Color
);

HI_S32 UI_DrawRect
(
    HI_HANDLE   hSurface,
    HI_S32      s32PosX,
    HI_S32      s32PosY,
    HI_S32      s32Width,
    HI_S32      s32Height,
    HI_U32      u32Color
);

HI_S32 UI_DrawText(HI_HANDLE hSurface, HI_CHAR* pcContent, HI_S32 s32PosX, HI_S32 s32PosY);

HI_S32 UI_DrawTextEx(HI_HANDLE hSurface, HI_CHAR* pcContent, HI_S32 s32PosX, HI_S32 s32PosY, HI_S32 s32Width, HI_S32 s32Height, HI_S32 enAlignType);

HI_S32 UI_GetTextExtent(HI_CHAR* pcText, HI_S32* ps32Width, HI_S32* ps32Height);

HI_S32 UI_SetTextColor(HI_U32 u32Color);

HI_S32 UI_SetTextBGColor(HI_U32 u32Color);

HI_S32 UI_SetTextBGTransparent(HI_BOOL bTransparent);

HI_S32 UI_ScreenRefresh(HI_VOID);

HI_S32 UI_ScreenClean(HI_VOID);

HI_S32 UI_SurfaceCreate(HI_S32 s32Width, HI_S32 s32Height, HI_HANDLE* phSurface);

HI_S32 UI_SurfaceDestory(HI_HANDLE hSurface);

HI_S32 UI_SurfaceBlit(HI_HANDLE hSurface);

HI_S32 UI_GraphicsInit(HI_UNF_PDM_DISP_PARAM_S* pstDispParam);

HI_S32 UI_GraphicsDeInit(HI_VOID);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__UI_GFX_H__*/
