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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "hi_type.h"
#include "cc_debug.h"
#include "aribcc_graphic.h"
#include "ccdisp_api.h"
#include "hi_unf_cc.h"

#define ARIBCC_WINDOW_ID  CCDISP_WINDOW_ID_0

#if 0
static HI_S32 remain_height = 0;  /*defined for the zoom rollup*/

static HI_U8 base_palette[64][4]={
        {0,  0,  0, },{255,0,  0  },{0,  255,0, },{255,255,0, },
        {0,  0,  255},{255,0,  255},{0,  255,255},{255,255,255},
        {170,0,  0  },{0,  170,0  },{170,170,0  },{0,  0,  170},
        {170,0,  170},{0,  170,170},{170,170,170},{0,  0,  85 },
        {0,  85, 0  },{0,  85, 85 },{0,  85, 170},{0,  85, 255},
        {0,  170,85 },{0,  170,255},{0,  255,85 },{0,  255,170},
        {85, 0,  0  },{85, 0,  85 },{85, 0,  170},{85, 0,  255},
        {85, 85, 0  },{85, 85, 85 },{85, 85, 170},{85, 85, 255},
        {85, 170,0  },{85, 170,85 },{85, 170,170},{85, 170,255},
        {85, 255,0  },{85, 255,85 },{85, 255,170},{85, 255,255},
        {170,0,  85 },{170,0,  255},{170,85, 0  },{170,85, 85 },
        {170,85, 170},{170,85, 255},{170,170,85 },{170,170,255},
        {170,255,0  },{170,255,85 },{170,255,170},{170,255,255},
        {255,0,  85 },{255,0,  170},{255,85, 0  },{255,85, 85 },
        {255,85, 170},{255,85, 255},{255,170,0  },{255,170,85 },
        {255,170,170},{255,170,255},{255,255,85 },{255,255,170}};

#endif

HI_S32 ARIBCC_GRAPHIC_Init(void)
{
    return HI_SUCCESS;
}

/*create the display screen*/
HI_S32 ARIBCC_GRAPHIC_CreateScreen(HI_S32 ccType, HI_S32 width, HI_S32 height)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CC_RECT_S stScreenRect;

    stScreenRect.x = stScreenRect.y = 0;
    stScreenRect.width = width;
    stScreenRect.height = height;
    s32Ret = CCDISP_Screen_SetColor(HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_TRANSPARENT);
    s32Ret |= CCDISP_Screen_SetSize(stScreenRect);
    s32Ret |= CCDISP_Screen_DisableClip();
    return s32Ret;
}

HI_S32 ARIBCC_GRAPHIC_DestroyScreen(HI_S32 ccType)
{
    (HI_VOID)CCDISP_Screen_Clear();
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_ClearScreen(HI_S32 ccType)
{
    (HI_VOID)CCDISP_Screen_Clear();
    return HI_SUCCESS;
}

/*create off screen frame buffer for drawing*/
HI_S32 ARIBCC_GRAPHIC_NewWindow(HI_S32 ccType,HI_S32 x,HI_S32 y,HI_S32 width,HI_S32 height)
{
    HI_UNF_CC_RECT_S stWinRect;
    stWinRect.x = x;
    stWinRect.y = y;
    stWinRect.width = width;
    stWinRect.height = height;
    return CCDISP_Window_Create(ARIBCC_WINDOW_ID, stWinRect);
}

/*free the off-screen frame buffer for the window, when the window size changed. call this function*/
HI_S32 ARIBCC_GRAPHIC_DelWindow(HI_S32 ccType)
{
    return CCDISP_Window_Destroy(ARIBCC_WINDOW_ID);
}


/*copy the off-screen frame buffer to on-screen using the bitblt. when CC_Arib_text_show will call this
  function to show the cc*/
HI_S32 ARIBCC_GRAPHIC_ShowWindow(HI_S32 ccType,HI_S32 dstx,HI_S32 dsty,HI_S32 width,HI_S32 height,HI_S32 startx,HI_S32 starty,HI_S32 is_colorkey_enable)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_FillWindow(HI_S32 ccType, HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, HI_UNF_CC_COLOR_S color)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CC_RECT_S stRect;

    stRect.x = x;
    stRect.y = y;
    stRect.width = width;
    stRect.height = height;

    s32Ret = CCDISP_Window_SetColorByRGB(ARIBCC_WINDOW_ID,color);
    s32Ret |= CCDISP_Window_FillRectByRGB(ARIBCC_WINDOW_ID,color,stRect,3);
    return s32Ret;
}

HI_S32 ARIBCC_GRAPHIC_SetFGColor(HI_S32 ccType,HI_UNF_CC_COLOR_S color, HI_S32 is_flash)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bFlash = HI_FALSE;

    if (is_flash)
    {
        bFlash = HI_TRUE;
    }

    s32Ret = CCDISP_Text_SetFGColorByRGB(ARIBCC_WINDOW_ID,color);
    s32Ret |= CCDISP_Text_SetFlash(ARIBCC_WINDOW_ID,bFlash);
    return s32Ret;
}

HI_S32 ARIBCC_GRAPHIC_SetBGColor(HI_S32 ccType,HI_UNF_CC_COLOR_S color)
{
    return CCDISP_Text_SetBGColorByRGB(ARIBCC_WINDOW_ID,color);
}

/*set text background middle color and foreground middle color*/
HI_S32 ARIBCC_GRAPHIC_SetMidColor(HI_S32 ccType,HI_UNF_CC_COLOR_S fg_mid_color, HI_UNF_CC_COLOR_S bg_mid_color)
{
    return HI_SUCCESS;
}


/*Set font edge type edge color*/
HI_S32 ARIBCC_GRAPHIC_SetEdgeColor(HI_S32 ccType,HI_UNF_CC_COLOR_S edge_color)
{
    return CCDISP_Text_SetEdgeColorByRGB(ARIBCC_WINDOW_ID,edge_color);
}

HI_S32 ARIBCC_GRAPHIC_GetTextSize(HI_S32 ccType,HI_S32 *pWidth, HI_S32 *pHeight)
{
    HI_U16 buf[1];

    buf[0] = 0x57;/*W,calc the max width of the code set*/
    (HI_VOID)CCDISP_Text_GetSize(ARIBCC_WINDOW_ID,buf,1,pWidth,pHeight);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_SetFontSize(HI_S32 ccType,HI_U8 size)
{
    HI_UNF_CC_FONTSIZE_E enFontSize;

    if (size < 20)
    {
        enFontSize = HI_UNF_CC_FONTSIZE_SMALL;
    }
    else if (size > 24)
    {
        enFontSize = HI_UNF_CC_FONTSIZE_LARGE;
    }
    else
    {
        enFontSize = HI_UNF_CC_FONTSIZE_STANDARD;
    }
    (HI_VOID)CCDISP_Text_SetFontSize(ARIBCC_WINDOW_ID,enFontSize);
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_FillRect(HI_S32 ccType, HI_S32 x, HI_S32 y, HI_S32 width, HI_S32 height, HI_UNF_CC_COLOR_S color)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_CC_RECT_S stRect;

    stRect.x = x;
    stRect.y = y;
    stRect.width = width;
    stRect.height = height;

    s32Ret = CCDISP_Window_SetColorByRGB(ARIBCC_WINDOW_ID,color);
    s32Ret |= CCDISP_Window_FillRectByRGB(ARIBCC_WINDOW_ID,color,stRect,3);
    return s32Ret;
}

/*if the flag is 1, write the caption to the on screen(displayed screen)*/
HI_S32 ARIBCC_GRAPHIC_TextOut(HI_S32 ccType,HI_S32 x, HI_S32 y, void *str, HI_S32 length, HI_S32 flag,HI_S32 space_interval)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32DrawWidth = 0;

    if(ARIBCC_TYPE_CC == ccType)
    {
        s32Ret = CCDISP_Text_Draw(ARIBCC_WINDOW_ID,x,y,str,length,&s32DrawWidth,3);
    }
    else
    {
        HI_ERR_CC("Not implement superimose!\n");
    }

    return s32Ret;
}

HI_S32 ARIBCC_GRAPHIC_TextOutEUC(HI_S32 ccType,HI_S32 x, HI_S32 y, void *str, HI_S32 length, HI_S32 flag)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_SetUnderline(HI_S32 ccType,HI_S32 is_under)
{
    HI_BOOL bUnderline = HI_FALSE;

    if (is_under)
    {
        bUnderline = HI_TRUE;
    }

    (HI_VOID)CCDISP_Text_SetUnderLine(ARIBCC_WINDOW_ID, bUnderline);
    return HI_SUCCESS;
}

/*set text edge type*/
HI_S32 ARIBCC_GRAPHIC_SetOrnament(HI_S32 ccType,HI_S32 ornament)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_SetFontScale(HI_S32 ccType,ARIBCC_CHARSIZE_E scale)
{
    return HI_SUCCESS;
}

/*set print direction*/
HI_S32 ARIBCC_GRAPHIC_SetPrintDirection(HI_S32 ccType,HI_S32 direction)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_DrawLine(HI_S32 ccType,HI_S32 x1,HI_S32 y1,HI_S32 x2,HI_S32 y2, HI_UNF_CC_COLOR_S color)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_BlockMove(HI_S32 ccType,HI_S32 srcx, HI_S32 srcy, HI_S32 width, HI_S32 height, HI_S32 dstx, HI_S32 dsty)
{
    (HI_VOID)CCDISP_Window_BlockMove(ARIBCC_WINDOW_ID,dstx,dsty,width,height,srcx,srcy);
    return HI_SUCCESS;
}

/***********************************************************************************************
 * function name: ARIBCC_GRAPHIC_DrawFontBitmap(...)                                                       *
 * description:  Draw font bitmap data to screen.                                              *
 *               for DRCS use. The DRCS character draw. copy the pbitmap from system memory    *
 *               to framebuffer.                                                               *
 ***********************************************************************************************/
HI_S32 ARIBCC_GRAPHIC_DrawFontBitmap(HI_S32 ccType,HI_S32 x,HI_S32 y,HI_U16 width, HI_U16 height, HI_S32 depth, HI_U8 *pbitmap, HI_U32 bitmaplen)
{
    HI_UNF_CC_RECT_S stRect;
    HI_UNF_CC_BITMAP_S stBitmap;

    stRect.x = x;
    stRect.y = y;
    stRect.width  = width;
    stRect.height = height;
    memset(&stBitmap, 0, sizeof(HI_UNF_CC_BITMAP_S));
    stBitmap.s32BitWidth = depth;
    stBitmap.pu8BitmapData = pbitmap;
    stBitmap.u32BitmapDataLen = bitmaplen;
    (HI_VOID)CCDISP_Bitmap_Draw(ARIBCC_WINDOW_ID, stRect, &stBitmap);
    return HI_SUCCESS;
}

/*set the flash color opaque. is the is_same_with_fg == 1, We should clear the bit4 of 0x28
  if the is_same_with_fg == 0 we should set the bit4 of ox28 and set the alpha to bit0-3 of 0x28*/
HI_S32 ARIBCC_GRAPHIC_SetFlashOpaqu(HI_S32 ccType, HI_U8 is_same_with_fg, HI_U8 alpha)
{
    return HI_SUCCESS;
}

HI_S32 ARIBCC_GRAPHIC_GetAribPalette(HI_U8 u8Index, HI_UNF_CC_COLOR_S *color)
{
    return HI_SUCCESS;
}


/*There are 64 basic color in ARIB. And plus the alpha (128,0) . There are 129 colors in total
  we set the 0-63 as the basic color(alpha = 255). 64-127 as the trnaslucent color(alpha=128)
  128-191 as the flashing color,   192 as the trnaparent color
  The arib spec has been defined the clut of 129 colors. Please refer to the B14 P267 */
HI_S32 ARIBCC_GRAPHIC_SetAribPalette(HI_S32 ccType)
{
    return HI_SUCCESS;
}


/*The flash behavior is display the foreground color and background color alternatively with
  The frequency 0.5 seconds. The foreground color is pre defined with the lut, but the background
  color combined with the foreground color is random. So we must add this function to set the
  flash background color
  This function is to modify the item in lut of the index specific to fg_color*/
HI_S32 ARIBCC_GRAPHIC_SetFlashBGColor(HI_S32 ccType, HI_UNF_CC_COLOR_S fg_color, HI_UNF_CC_COLOR_S bg_color)
{
    return HI_SUCCESS;
}

#if 0
static HI_S32 ScaleImage(HI_U8 *pSrc,HI_S32 srcWidth,HI_S32 srcHeight,HI_S32 srcRowbyte,
                      HI_U8 *pDst,HI_S32 dstWidth,HI_S32 dstHeight,HI_S32 dstRowbyte)
{
    return HI_SUCCESS;
}
#endif

/*copy the system memory to frame buffer use the 2dge enginee*/
HI_S32 ARIBCC_GRAPHIC_SysMem2FBCopy(HI_S32 ccType,HI_U8 *pBuf,HI_S32 width,HI_S32 height,HI_S32 bpp,HI_S32 rowbytes,HI_S32 dstx,HI_S32 dsty)
{
    HI_UNF_CC_RECT_S stRect;
    HI_UNF_CC_BITMAP_S stBitmap;

    stRect.x = dstx;
    stRect.y = dsty;
    stRect.width = width;
    stRect.height = height;
    memset(&stBitmap, 0, sizeof(HI_UNF_CC_BITMAP_S));
    stBitmap.pu8BitmapData = pBuf;
    stBitmap.u32BitmapDataLen = (HI_U32)rowbytes;
    stBitmap.s32BitWidth = bpp;
    (HI_VOID)CCDISP_Bitmap_Draw(ARIBCC_WINDOW_ID,stRect,&stBitmap);

    return HI_SUCCESS;
}


HI_S32 ARIBCC_GRAPHIC_SetPalette(HI_S32 ccType, ARIBCC_Pallette_S *pLut, HI_S32 start, HI_S32 length)
{
    return HI_SUCCESS;
}

#define ROLLLFT_STEP  4

HI_S32 ARIBCC_GRAPHIC_RollLeft(HI_S32 ccType,HI_U16 x,HI_U16 y, HI_U16 width,HI_U16 height,
                HI_U16 offset,HI_UNF_CC_COLOR_S win_color,HI_U8 speed)
{
    return HI_SUCCESS;
}


HI_S32 ARIBCC_GRAPHIC_RollupStart(HI_S32 ccType)
{
    return HI_SUCCESS;
}


HI_S32 ARIBCC_GRAPHIC_Rollup(HI_S32 ccType,HI_U16 x, HI_U16 width, HI_U16 top_y,
                 HI_U16 btm_y,HI_U16 offset, HI_UNF_CC_COLOR_S win_color,HI_U8 speed)
{
     (HI_VOID)CCDISP_Window_Rollup(ARIBCC_WINDOW_ID,x,width,top_y,btm_y,offset,CCDISP_SPEED6);
    //(HI_VOID)CCDISP_Window_Rollup(ARIBCC_WINDOW_ID,x,width,btm_y,top_y,offset,CCDISP_SPEED1);
    return HI_SUCCESS;
}


HI_S32 ARIBCC_GRAPHIC_SetScaleFactor(HI_S32 ccType, HI_S32 vscale, HI_S32 hscale, HI_S32 fifo_lvl)
{
    return HI_SUCCESS;
}


HI_S32 ARIBCC_GRAPHIC_SetResolution(ARIBCC_WINDOWFORMAT_E resolution)
{

    return HI_SUCCESS;
}

