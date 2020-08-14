/*
** Copyright 1999-2005, Silicon Graphics, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of Silicon Graphics, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
**
** $Header: /export/home/cvs/conformes/conformes_patch/linux_nws_patch.txt,v 1.2 2005/07/27 10:23:23 mah Exp $
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "egl_native.h"

#if defined (EGL_API_HIGO)
 #include "hi_go.h"
#elif defined (EGL_API_FB)
 #ifndef EXCLUDE_EGL_SPECIFICS
  #include "EGL/eglplatform.h"
 #endif
#endif

#if defined (EGL_API_HIGO)
static HI_HANDLE g_hLayer;
static HI_HANDLE g_hWnd;
 #define INVALID_HANDLE 0
#elif defined (EGL_API_FB)
/*static NativeDisplay g_Display;*/
/*static NativeWindow g_Window;*/
#endif

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

#define DEBUG_POS(Ret)

NativeDisplay nativeCreateDisplay(void)
{
#if defined (EGL_API_HIGO)
    HI_S32 s32Ret;
    HIGO_LAYER_INFO_S stLayerInfo;
    HIGO_LAYER_ALPHA_S stAlpha;

    s32Ret = HI_GO_Init();
    s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);

    stLayerInfo.ScreenWidth  = 1280;
    stLayerInfo.ScreenHeight = 720;
    stLayerInfo.CanvasWidth   = 1280;
    stLayerInfo.CanvasHeight  = 720;
    stLayerInfo.DisplayWidth  = 1280;
    stLayerInfo.DisplayHeight = 720;
    stLayerInfo.PixelFormat = HIGO_PF_8888;

    s32Ret = HI_GO_CreateLayer (&stLayerInfo, &g_hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        printf("<%s> Line %d: Ret = 0x%x\n", __FUNCTION__, __LINE__, s32Ret);
        return 0;
    }

    s32Ret = HI_GO_SetLayerBGColor(g_hLayer, 0xffffffff);
    if (HI_SUCCESS != s32Ret)
    {
        printf("<%s> Line %d: Ret = 0x%x\n", __FUNCTION__, __LINE__, s32Ret);
        return 0;
    }
    return g_hLayer;

#elif defined (EGL_API_FB)
    return (NativeDisplay)EGL_DEFAULT_DISPLAY;
#endif
}

void nativeDestroyDisplay(NativeDisplay display)
{
#if defined (EGL_API_HIGO)
    HI_HANDLE hLayer = (HI_HANDLE)display;
    if (INVALID_HANDLE == hLayer)
    {
        return;
    }

    HI_GO_DestroyLayer(hLayer);
    HI_GO_Deinit();

#elif defined (EGL_API_FB)
    return;
#endif
}

NativeWindow nativeCreateWindow(const char *title, int width, int height, EGLint visualId)
{
#if defined (EGL_API_HIGO)
    HI_S32 s32Ret;
    HI_HANDLE hWnd;
    HIGO_WNDINFO_S stWinInfo = {0};
    stWinInfo.hLayer   = g_hLayer;
    stWinInfo.LayerNum = visualId;
    stWinInfo.PixelFormat = HIGO_PF_8888;
    stWinInfo.rect.x = 0;
    stWinInfo.rect.y = 0;
    stWinInfo.rect.w = width;
    stWinInfo.rect.h = height;

    s32Ret = HI_GO_CreateWindowEx(&stWinInfo, &hWnd);
    if (HI_SUCCESS != s32Ret)
    {
        printf("<%s> Line %d: Ret = 0x%x\n", __FUNCTION__, __LINE__, s32Ret);
        return 0;
    }

    s32Ret = HI_GO_SetWindowOpacity(hWnd, 0xff);
    if (HI_SUCCESS != s32Ret)
    {
        printf("<%s> Line %d: Ret = 0x%x\n", __FUNCTION__, __LINE__, s32Ret);
        return 0;
    }

    return (void*)hWnd;

#elif defined (EGL_API_FB)
    fbdev_window *fbwin = malloc( sizeof(fbdev_window));
    if (NULL == fbwin)
    {
        return 0;
    }

    fbwin->width  = width;
    fbwin->height = height;
    return fbwin;
#endif
}

void nativeDestroyWindow(NativeWindow window)
{
#if defined (EGL_API_HIGO)
    HI_HANDLE hWnd = (HI_HANDLE)window;

    if (INVALID_HANDLE == hWnd)
    {
        return;
    }

    HI_GO_DestroyWindow(hWnd);

#elif defined (EGL_API_FB)
    free(window);
#endif
}

#if 0
NativePixmap nativeCreatePixmap(int width, int height, EGLint visualId)
{
#if defined (EGL_API_HIGO)
    HI_S32 s32Ret;
    HI_HANDLE hSur;

    s32Ret = HI_GO_CreateSurface(width, width, HIGO_PF_8888, &hSur);
    if (HI_SUCCESS != s32Ret)
    {
        printf("<%s> Line %d: Ret = 0x%x\n", __FUNCTION__, __LINE__, s32Ret);
        return 0;
    }

    return (void*)hSur;

#elif defined (EGL_API_FB)
    fbdev_pixmap *pixmap = (fbdev_pixmap*)malloc(sizeof(fbdev_pixmap));
    if (NULL == pixmap)
    {
        return 0;
    }

    memset(pixmap, 0, sizeof(fbdev_pixmap));
    pixmap->width  = width;
    pixmap->height = height;
    pixmap->alpha_size = 8;
    pixmap->red_size   = 8;
    pixmap->green_size = 8;
    pixmap->blue_size  = 8;
    pixmap->luminance_size = 0;
    pixmap->buffer_size = pixmap->red_size + pixmap->green_size + pixmap->blue_size + pixmap->alpha_size
                          + pixmap->luminance_size;
    pixmap->bytes_per_pixel = ((pixmap->buffer_size) + 7) / 8;
    pixmap->data = malloc(pixmap->width * pixmap->height * pixmap->bytes_per_pixel);
    if (NULL == pixmap->data)
    {
        free(pixmap);
        return 0;
    }

    return (EGLNativePixmapType)pixmap;
#endif
}

void nativeDestroyPixmap(NativePixmap pixmap)
{
#if defined (EGL_API_HIGO)
    HI_HANDLE hSur = (HI_HANDLE)pixmap;
    DEBUG_POS(0);
    if (INVALID_HANDLE == hSur)
    {
        return;
    }

    HI_GO_DestroyWindow(hSur);

#elif defined (EGL_API_FB)
    fbdev_pixmap *pixmap_2 = pixmap;

    if (NULL != pixmap)
    {
        free(pixmap_2->data);
        free(pixmap_2);
    }
#endif

}
#endif