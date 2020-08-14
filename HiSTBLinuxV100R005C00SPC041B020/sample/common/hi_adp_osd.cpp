/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : hi_adp_osd.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/02/18
  Description   :
  History       :
  1.Date        : 2014/02/18
    Author      : h00183514
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "OsdManager.h"
#include "hi_adp_osd.h"

#define OSD_PRINT   ALOGE

#define OSD_CHECK_INIT()    \
do{                                 \
    if (g_om == HI_NULL)            \
    {                               \
        OSD_PRINT("not init!\n");   \
        return HI_FAILURE;          \
    }                               \
}while(0)

static android::OsdManager *g_om = HI_NULL;

static HI_S32 GetBytesPerPixel(HIADP_PF_E enPF)
{
    switch(enPF)
    {
        case HIADP_PF_8888:
            return 4;
        case HIADP_PF_565:
        case HIADP_PF_1555:
        case HIADP_PF_4444:
            return 2;
        default:
            return 4;
    }

    return 4;
}

static HI_S32 PixelFormatOSD2Android(HIADP_PF_E enPF)
{
    int pixelFormat = android::PIXEL_FORMAT_RGBA_8888;

    switch (enPF)
    {
        case HIADP_PF_565:
            pixelFormat = android::PIXEL_FORMAT_RGB_565;
             break;

        case HIADP_PF_1555:
            pixelFormat = android::PIXEL_FORMAT_RGBA_5551;
             break;

        case HIADP_PF_4444:
            pixelFormat = android::PIXEL_FORMAT_RGBA_4444;
             break;

        case HIADP_PF_8888:
            pixelFormat = android::PIXEL_FORMAT_RGBA_8888;
             break;

        default:
            OSD_PRINT("this pixel mode can not be supported: %d\n", enPF);
            break;
    }

     return pixelFormat;
}

static HIADP_PF_E PixelFormatAndroid2OSD(int pixelFormat)
{
    HIADP_PF_E enPF = HIADP_PF_8888;

    switch (pixelFormat)
    {
        case android::PIXEL_FORMAT_RGB_565:
            enPF = HIADP_PF_565;
             break;

        case android::PIXEL_FORMAT_RGBA_5551:
            enPF = HIADP_PF_1555;
             break;

        case android::PIXEL_FORMAT_RGBA_4444:
            enPF = HIADP_PF_4444;
             break;

        case android::PIXEL_FORMAT_BGRA_8888:
            enPF = HIADP_PF_8888;
             break;

        default:
            OSD_PRINT("this pixel mode can not be supported:%d\n", pixelFormat);
            break;
    }
     return enPF;
}

static HI_VOID SurfaceAttrOSD2ADP(SURFACE_ATTR_S *pstOsdAttr, HIADP_SURFACE_ATTR_S *pstAdpAttr)
{
    pstAdpAttr->enPixelFormat = PixelFormatAndroid2OSD(pstOsdAttr->PixelFormat);
    pstAdpAttr->u32Width      = pstOsdAttr->Width;
    pstAdpAttr->u32Height     = pstOsdAttr->Height;
}

static HI_VOID SurfaceAttrADP2OSD(HIADP_SURFACE_ATTR_S *pstAdpAttr, SURFACE_ATTR_S *pstOsdAttr)
{
    pstOsdAttr->PixelFormat   = PixelFormatOSD2Android(pstAdpAttr->enPixelFormat);
    pstOsdAttr->Width         = pstAdpAttr->u32Width;
    pstOsdAttr->Height        = pstAdpAttr->u32Height;
    pstOsdAttr->Stride        = pstAdpAttr->u32Width * GetBytesPerPixel(pstAdpAttr->enPixelFormat);
}

static HI_VOID CCTextAttrADP2OSD(HIADP_CCTEXT_ATTR_S *pstAdpAttr, CCTEXT_ATTR_S *pstOsdAttr)
{

    memset(pstOsdAttr,0,sizeof(CCTEXT_ATTR_S));
    memcpy(pstOsdAttr,pstAdpAttr,sizeof(HIADP_CCTEXT_ATTR_S));
}

static HI_VOID CCTextAttrOSD2ADP( CCTEXT_ATTR_S *pstOsdAttr,HIADP_CCTEXT_ATTR_S *pstAdpAttr)
{
    memset(pstAdpAttr,0,sizeof(HIADP_CCTEXT_ATTR_S));
    memcpy(pstAdpAttr,pstOsdAttr,sizeof(CCTEXT_ATTR_S));
}

HI_VOID HIADP_OSD_Init(HI_VOID)
{
    if (g_om != HI_NULL)
    {
        return;
    }

    g_om = new android::OsdManager();

    return;
}

HI_VOID HIADP_OSD_DeInit(HI_VOID)
{
    if (HI_NULL != g_om)
    {
        delete g_om;
        g_om = HI_NULL;
    }

    return;
}

HI_S32 HIADP_OSD_CreateSurface(HIADP_SURFACE_ATTR_S *pstAttr, HI_HANDLE *phSurface)
{
    HI_S32          Ret;
    SURFACE_ATTR_S  SurfaceAttr;

    OSD_CHECK_INIT();

    if (HI_NULL == pstAttr || HI_NULL == phSurface)
    {
        OSD_PRINT("pstAttr or phSurface is null!\n");

        return HI_FAILURE;
    }

    if (pstAttr->u32Width > 1920 || pstAttr->u32Height > 1080)
    {
        OSD_PRINT("pstAttr->u32Width or pstAttr->u32Height is invalid!\n");

        return HI_FAILURE;
    }

    SurfaceAttrADP2OSD(pstAttr, &SurfaceAttr);

    Ret = g_om->CreateSurface(&SurfaceAttr, phSurface);

    return Ret;
}

HI_S32 HIADP_OSD_DestroySurface(HI_HANDLE hSurface)
{
    OSD_CHECK_INIT();

    return g_om->DestroySurface(hSurface);
}

HI_S32 HIADP_OSD_Blit(HI_HANDLE hSrcSurface, HI_RECT_S *pstSrcRect, HI_HANDLE hDstSurface, HI_RECT_S *pstDstRect)
{
    OSD_CHECK_INIT();

    return g_om->Blit(hSrcSurface, pstSrcRect, hDstSurface, pstDstRect);
}

HI_S32 HIADP_OSD_BlitFrameToSurface(HI_UNF_VIDEO_FRAME_INFO_S *pstFrame,HI_HANDLE hDstSurface, HI_RECT_S *pstDstRect)
{
    OSD_CHECK_INIT();

    return g_om->BlitFrameToSurface(pstFrame, hDstSurface, pstDstRect);
}

HI_S32 HIADP_OSD_FillRect(HI_HANDLE hSurface, HI_RECT_S *pstRect, HI_U32 u32Color)
{
    OSD_CHECK_INIT();

    return g_om->FillRect(hSurface, pstRect, u32Color);
}

HI_S32 HIADP_OSD_Refresh(HI_HANDLE hSurface)
{
    OSD_CHECK_INIT();

    return g_om->Refresh(hSurface);
}

HI_S32 HIADP_OSD_DecodeImage(HI_HANDLE hSurface, const HI_VOID *pImageMem, HI_U32 u32MemSize, HI_RECT_S *pstRect)
{
    OSD_CHECK_INIT();

    return g_om->DecodeImage(hSurface,pImageMem, u32MemSize, pstRect);
}

HI_S32 HIADP_OSD_ClearSurface(HI_HANDLE hSurface)
{
    OSD_CHECK_INIT();

    return g_om->ClearSurface(hSurface);
}

HI_S32 HIADP_OSD_EncodeFrame(HI_UNF_VIDEO_FRAME_INFO_S *pstFrame, HI_S32 EncodeType, HI_CHAR *DstFile)
{
    OSD_CHECK_INIT();

    return g_om->EncodeFrame(pstFrame, EncodeType, DstFile);
}


HI_S32 HIADP_OSD_GetCCTextSize(HI_CHAR* pTest,HI_S32 *pWidth, HI_S32 *pHeigh )
{
    OSD_CHECK_INIT();

    return g_om->GetCCTextSize(pTest, pWidth,pHeigh);
}

HI_S32 HIADP_OSD_FillCCRect(HI_HANDLE hSurface, HIADP_SURFACE_ATTR_S * pstSurfaceAttr,HI_RECT_S *pstRect, HI_U32 u32Color)
{
    OSD_CHECK_INIT();

    SURFACE_ATTR_S  SurfaceAttr;

    SurfaceAttrADP2OSD(pstSurfaceAttr, &SurfaceAttr);

    return g_om->FillCCRect(hSurface,  &SurfaceAttr,pstRect, u32Color);
}

HI_S32 HIADP_OSD_DrawCCText(HI_HANDLE hSurface, HIADP_SURFACE_ATTR_S * pstSurfaceAttr,HI_RECT_S *pstRect, HI_CHAR *pTest,HIADP_CCTEXT_ATTR_S * pstAttr)
{
    OSD_CHECK_INIT();

    CCTEXT_ATTR_S pstCCTextAtrr;
    SURFACE_ATTR_S  SurfaceAttr;

    SurfaceAttrADP2OSD(pstSurfaceAttr, &SurfaceAttr);
    CCTextAttrADP2OSD(pstAttr,&pstCCTextAtrr);

    return g_om->DrawCCText(hSurface, &SurfaceAttr,pstRect, pTest,&pstCCTextAtrr);
}

HI_S32 HIADP_OSD_CCBlit(HI_HANDLE hSrcSurface, HIADP_SURFACE_ATTR_S * pstSurfaceAttr,HI_RECT_S *pstSrcRect, HI_RECT_S *pstDstRect)
{
    OSD_CHECK_INIT();

    SURFACE_ATTR_S  SurfaceAttr;

    SurfaceAttrADP2OSD(pstSurfaceAttr, &SurfaceAttr);

    return g_om->CCBlit(hSrcSurface, &SurfaceAttr,pstSrcRect, pstDstRect);
}

HI_S32 HIADP_OSD_TeletextDrawBitmap(HI_HANDLE hSrcSurface, HI_U8 *bitmap,HI_RECT_S *pstSrcRect,HI_RECT_S *pstDstRect)
{
    OSD_CHECK_INIT();

    return g_om->TeletextDrawBitmap(hSrcSurface, bitmap,pstSrcRect,pstDstRect);
}

HI_S32 HIADP_OSD_SubDrawBitmap(HI_HANDLE hSrcSurface, HI_U8 *bitmap,HIADP_SURFACE_ATTR_S *pstAttr,HI_RECT_S *pstSrcRect)
{

    OSD_CHECK_INIT();

    SURFACE_ATTR_S  SurfaceAttr;

    SurfaceAttrADP2OSD(pstAttr, &SurfaceAttr);

    return g_om->SubDrawBitmap(hSrcSurface, bitmap,&SurfaceAttr,pstSrcRect);
}
