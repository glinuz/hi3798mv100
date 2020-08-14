/******************************************************************************
                              Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : test_loader.h
Version       : Initial Draft
Author        : Hisilicon multimedia Hardware group l50549
Created       : 2007/08/20
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __UPDATE_TEXT_H__
#define __UPDATE_TEXT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


typedef enum
{
    HIGO_LAYOUT_LEFT             = 0x0001,
    HIGO_LAYOUT_RIGHT            = 0x0002,
    HIGO_LAYOUT_HCENTER          = 0x0004,
    HIGO_LAYOUT_WRAP             = 0x0008,
    HIGO_LAYOUT_WORDELLIPSIS     = 0x0010,
    HIGO_LAYOUT_TOP              = 0x0100,
    HIGO_LAYOUT_BOTTOM           = 0x0200,
    HIGO_LAYOUT_VCENTER          = 0x0400,
    HIGO_LAYOUT_FORCE_LEFT       = 0x8000,
    HIGO_LAYOUT_FORCE_RIGHT      = 0x10000,
    HIGO_LAYOUT_BUTT             = 0x20000,
} HIGO_LAYOUT_E;

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4 */
/** @{ */  /** <!-- [UI display data structure] */

HI_S32 HI_GO_CreateText(const HI_CHAR* pSbFontFile, const HI_CHAR* pMbFontFile, HI_HANDLE* pTextOut);

HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut, const HI_CHAR* pText, HI_S32* pWidth, HI_S32* pHeight);

HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut, HI_COLOR Color);

HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut, HI_COLOR Color);

HI_S32 HI_GO_TextOut(HI_HANDLE TextOut, HI_HANDLE Surface, const HI_CHAR* pText,
                     const HI_RECT* pRect);

HI_S32 HI_GO_TextOutEx(HI_HANDLE hSurface, HI_CHAR* pcContent, HI_S32 s32PosX, HI_S32 s32PosY, HI_S32 s32Width, HI_S32 s32Height, HI_S32 enAlignType);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif /*__UPDATE_OSD_H__*/
