/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/

#ifndef _CCDISP_API_H
#define _CCDISP_API_H

#include "hi_type.h"
#include "hi_unf_cc.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

typedef enum tagCCDISP_WINDOW_ID_E
{
    CCDISP_WINDOW_ID_0 = 0,
    CCDISP_WINDOW_ID_1,
    CCDISP_WINDOW_ID_2,
    CCDISP_WINDOW_ID_3,
    CCDISP_WINDOW_ID_4,
    CCDISP_WINDOW_ID_5,
    CCDISP_WINDOW_ID_6,
    CCDISP_WINDOW_ID_7,
    CCDISP_WINDOW_ID_BUTT
} CCDISP_WINDOW_ID_E;

/*the maximum cc screens for cc*/
#define CCDISP_MAX_WINDOWS    ((HI_S32)CCDISP_WINDOW_ID_BUTT)


/*
typedef enum tagCCDISP_BPP_E
{
    CCDISP_BPP_8BITS,
    CCDISP_BPP_32BITS,
    CCDISP_BPP_64BITS
} CCDISP_BPP_E;
*/

typedef enum tagCCDISP_ROLLSPEED_E
{
    CCDISP_SPEED1 = 0,
    CCDISP_SPEED2,
    CCDISP_SPEED3,
    CCDISP_SPEED4,
    CCDISP_SPEED5,
    CCDISP_SPEED6,
} CCDISP_ROLLSPEED_E;


typedef struct tagCCDISP_OPT_S
{
    HI_UNF_CC_GETPTS_CB_FN       pfnGetPts;
    HI_UNF_CC_DISPLAY_CB_FN      pfnDisplay;
    HI_UNF_CC_GETTEXTSIZE_CB_FN  pfnGetTextSize;
    HI_UNF_CC_BLIT_CB_FN         pfnBlit;
    HI_UNF_CC_VBI_CB_FN          pfnVBIOutput;
    HI_UNF_CC_XDS_CB_FN          pfnXDSOutput;
    HI_VOID*                     pUserData;
} CCDISP_OPT_S;


typedef struct tagCCDisp_Text_S
{
    HI_U32                 u32FgColor;
    HI_UNF_CC_COLOR_S     stFgColor;
    HI_UNF_CC_OPACITY_E   enFgOpacity;
    HI_U32                 u32BgColor;
    HI_UNF_CC_COLOR_S     stBgColor;
    HI_UNF_CC_OPACITY_E   enBgOpacity;
    HI_UNF_CC_FONTNAME_E  enFontName;
    HI_UNF_CC_FONTSTYLE_E enFontStyle;
    HI_UNF_CC_FONTSIZE_E  enFontSize;

    HI_UNF_CC_EdgeType_E  enEdgeType;
    HI_U32                 u32EdgeColor;
    HI_UNF_CC_COLOR_S     stEdgeColor;
    HI_UNF_CC_OPACITY_E   enEdgeOpacity;

    //HI_BOOL bFlash;
    //HI_S32  s32UnderlinePos;
    //HI_S32  s32UnderlineThick;
} CCDisp_Text_S;

typedef struct tagCCDisp_Window_S
{
    CCDISP_WINDOW_ID_E   enWinID;
    HI_BOOL              bVisible;
    HI_UNF_CC_RECT_S     stWinRect;
    HI_U32                u32WinColor;
    HI_UNF_CC_COLOR_S    stWinColor;
    HI_UNF_CC_OPACITY_E  enWinOpacity;
    CCDisp_Text_S        stText;
} CCDisp_Window_S;

typedef struct tagCCDisp_Screen_S
{
    HI_UNF_CC_RECT_S     stScreenRect;
    HI_UNF_CC_RECT_S     stClipArea;
    HI_U32                u32ScreenColor;
    HI_UNF_CC_COLOR_S    stScreenColor;
    HI_UNF_CC_OPACITY_E  enScreenOpacity;

    //HI_U16 u16Display_x;
    //HI_U16 u16Display_y;
    //HI_U16 u16displayWidth;
    //HI_U16 u16DisplayHeight;

    //HI_U8 real_font_size[HI_UNF_CC_FN_BUTT][HI_UNF_CC_FONTSIZE_BUTT];   /*to store the font size of nano-x matched theCCFontSize*/
    //HI_U32 flag;                 /*underline and edge type*/

    CCDisp_Window_S astWindow[CCDISP_MAX_WINDOWS];
    CCDISP_OPT_S stOpt;
} CCDisp_Screen_S;

typedef struct tagCCDISP_INIT_PARAM_S
{
    CCDISP_OPT_S stOpt;
} CCDISP_INIT_PARAM_S;


typedef struct tagCCDisp_FlashText_S
{
    HI_UNF_CC_RECT_S    stRect;
    HI_U16          *            pu16Text;
    HI_S32                        s32TextLen;
    HI_U32                        u32FgColor;
    HI_U32                        u32BgColor;
    HI_U32                        u32DisplayWidth;
    HI_U32                        u32DisplayHeight;
    HI_UNF_CC_FONTSIZE_E  enFontSize;
    HI_BOOL bFgFlash;//fg flash or bg flash
    struct tagCCDisp_FlashText_S *   next;
}CCDISP_FlashText_S;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

HI_S32 CCDISP_Init(CCDISP_INIT_PARAM_S *pstInitParam);
HI_S32 CCDISP_DeInit(void);


HI_S32 CCDISP_Screen_SetSize(HI_UNF_CC_RECT_S stScreenRect);
HI_S32 CCDISP_Screen_SetClipArea(HI_UNF_CC_RECT_S stClipArea);
HI_S32 CCDISP_Screen_DisableClip(void);
HI_S32 CCDISP_Screen_GetColor(HI_U32 *pu32Color, HI_UNF_CC_OPACITY_E *penOpacity);
HI_S32 CCDISP_Screen_SetColor(HI_U32 u32Color, HI_UNF_CC_OPACITY_E enOpacity);
HI_S32 CCDISP_Screen_GetColorByRGB(HI_UNF_CC_COLOR_S *pstColor);
HI_S32 CCDISP_Screen_SetColorByRGB(HI_UNF_CC_COLOR_S stColor);
HI_S32 CCDISP_Screen_Clear(void);


HI_S32 CCDISP_Window_Create(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stWinRect);
HI_S32 CCDISP_Window_Destroy(CCDISP_WINDOW_ID_E enWinID);
HI_S32 CCDISP_Window_Show(CCDISP_WINDOW_ID_E enWinID);
HI_S32 CCDISP_Window_Hide(CCDISP_WINDOW_ID_E enWinID);
HI_S32 CCDISP_Window_GetSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S *pstWinRect);
HI_S32 CCDISP_Window_SetSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stWinRect);
HI_S32 CCDISP_Window_GetColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 *pu32WinColor, HI_UNF_CC_OPACITY_E *penWinOpacity);
HI_S32 CCDISP_Window_SetColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32WinColor, HI_UNF_CC_OPACITY_E enWinOpacity);
HI_S32 CCDISP_Window_GetColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstWinColor);
HI_S32 CCDISP_Window_SetColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stWinColor);
HI_S32 CCDISP_Window_Clear(CCDISP_WINDOW_ID_E enWinID);
HI_S32 CCDISP_Window_FillRect(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32Color,
                              HI_UNF_CC_OPACITY_E enOpacity, HI_UNF_CC_RECT_S stRect,HI_U32 flag);
HI_S32 CCDISP_Window_FillRectByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stColor, HI_UNF_CC_RECT_S stRect,HI_U32 flag);
HI_S32 CCDISP_Window_BlockMove(CCDISP_WINDOW_ID_E enWinID, HI_S32 s32DstX, HI_S32 s32DstY,
                               HI_S32 s32Width, HI_S32 s32Height, HI_S32 s32SrcX, HI_S32 s32SrcY);
HI_S32 CCDISP_Window_Rollup(CCDISP_WINDOW_ID_E enWinID, HI_U16 x, HI_U16 width,
                            HI_U16 u16StartY, HI_U16 u16EndY, HI_U16 u16Offset, CCDISP_ROLLSPEED_E enSpeed);


HI_S32 CCDISP_Text_GetSize(CCDISP_WINDOW_ID_E enWinID, HI_U16 *pu16Str,
                           HI_S32 s32Length, HI_S32 *ps32Width, HI_S32 *ps32Height);
HI_S32 CCDISP_Text_Draw(CCDISP_WINDOW_ID_E enWinID, HI_S32 x, HI_S32 y, 
                                HI_U16 *pu16Str, HI_S32 s32StrLen, HI_S32 *ps32DrawWidth,HI_U32 flag);

HI_S32 CCDISP_Text_SetEdgeType(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_EdgeType_E enEdgeType);
HI_S32 CCDISP_Text_SetEdgeColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32EdgeColor, HI_UNF_CC_OPACITY_E enEdgeOpacity);
HI_S32 CCDISP_Text_SetEdgeColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stEdgeColor);
HI_S32 CCDISP_Text_SetUnderLine(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bUnderline);
HI_S32 CCDISP_Text_SetItalic(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bItalic);
HI_S32 CCDISP_Text_SetFlash(CCDISP_WINDOW_ID_E enWinID, HI_BOOL bFlash);
HI_S32 CCDISP_Text_SetFontName(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_FONTNAME_E enFontName);
HI_S32 CCDISP_Text_SetFontSize(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_FONTSIZE_E enFontSize);
HI_S32 CCDISP_Text_GetFGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 *pu32FgColor, HI_UNF_CC_OPACITY_E *penFgOpacity);
HI_S32 CCDISP_Text_SetFGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32FgColor, HI_UNF_CC_OPACITY_E enFgOpacity);
HI_S32 CCDISP_Text_GetBGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 *pu32BgColor, HI_UNF_CC_OPACITY_E *penBgOpacity);
HI_S32 CCDISP_Text_SetBGColor(CCDISP_WINDOW_ID_E enWinID, HI_U32 u32BgColor, HI_UNF_CC_OPACITY_E enBgOpacity);

HI_S32 CCDISP_Text_GetFGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstFgColor);
HI_S32 CCDISP_Text_SetFGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stFgColor);
HI_S32 CCDISP_Text_GetBGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S *pstBgColor);
HI_S32 CCDISP_Text_SetBGColorByRGB(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_COLOR_S stBgColor);

HI_S32 CCDISP_Bitmap_Draw(CCDISP_WINDOW_ID_E enWinID, HI_UNF_CC_RECT_S stRect, HI_UNF_CC_BITMAP_S *pstBitmap);

HI_S32 CCDISP_SendVbiData(HI_UNF_CC_VBI_DADA_S *pstVbiDataField1,HI_UNF_CC_VBI_DADA_S *pstVbiDataField2);

HI_S32 CCDISP_GetCurPts(HI_S64 *ps64CurPts);

HI_S32 CCDISP_OutputXDSData(HI_U8 u8XDSClass, HI_U8 u8XDSPacketType, HI_U8 *pu8Data, HI_U8 u8Length);

HI_S32 CCDISP_Text_CharFlash(HI_VOID);
HI_S32 CCDISP_Text_DeleteCharFlash(CCDISP_WINDOW_ID_E enWinID);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif //#ifndef _PERU_CCDISP_API_H

/*****************************************************************************
*                    End Of File
*****************************************************************************/
