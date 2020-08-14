/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_egl.c
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

/*********************************add include here******************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "egl_native.h"
#include "hifb.h"
#include "hi_egl.h"

#ifdef NEED_DEPEND_SDK

 #include "hi_unf_disp.h"
 #include "hi_adp_mpi.h"

#endif

#include <hi_go.h>

#if defined (EGL_API_DIRECTFB)
 #include "hi_input_dfb.h"
 #include <directfb.h>
 #define DFBCHECK(x...) \
    {                                                                \
        err = x;                                                    \
        if (err != DFB_OK) {                                        \
            HI3D_TRACE("%s <%d>:\n\t", __FILE__, __LINE__ ); \
            DirectFBErrorFatal( # x, err );                         \
        }                                                           \
    }

IDirectFB              *dfb;
IDirectFBWindow *g_dfbwindow = NULL;
#endif

static HI_S32 console_fd = 0;

/***************************** Macro Definition ******************************/

/******************** to see which include file we want to use***************/

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

EGLDisplay g_EglDisplay;
EGLContext g_EglContext;
EGLSurface g_EglSurface;

NativeDisplayType g_NativeDisplay;
NativeWindowType g_NativeWindow;

#ifdef NEED_DEPEND_SDK
HI_UNF_ENC_FMT_E g_enEncFmt = HI_UNF_ENC_FMT_720P_50;
#endif

#if defined (EGL_API_FB)
int gs_fd = -1;
#endif

/******************************* API forward declarations *******************/

/******************************* API realization *****************************/

/**************************** realize hi_egl.h *************/

/******************************************************************************
func                     : hi_egl_setlayerpara
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
static HI_S32 hi_egl_setlayerpara(HI_U32 width, HI_U32 height)
{
    /* disable layer alpha */
    int err;
    /*HIFB_SIZE_S screenSize;*/
    HIFB_ALPHA_S stAlpha = {0};
    struct fb_var_screeninfo vinfo;

    /**
    ** 要注意视频层和图形层的优先级，一般图形层在视频层上面,
    ** 设置透明可以使视频层上显示图形，HIADP_Disp_Init在这个函数里面
    **/
    memset(&stAlpha, 0, sizeof(stAlpha));
    stAlpha.bAlphaEnable  = HI_TRUE;
    stAlpha.bAlphaChannel = HI_TRUE;
    stAlpha.u8Alpha0 = 0xff;
    stAlpha.u8Alpha1 = 0xff;
    stAlpha.u8GlobalAlpha = 0xff;

    console_fd = open("/dev/fb0", O_RDWR, 0);

    if (ioctl(console_fd, FBIOPUT_ALPHA_HIFB, &stAlpha) < 0)
    {
        HI3D_TRACE("Unable to set alpha!\n");
    }

    if ((err = ioctl(console_fd, FBIOGET_VSCREENINFO, &vinfo)) < 0)
    {
        HI3D_TRACE("Unable to FBIOGET_VSCREENINFO!, err=0x%x\n", err);
        return err;
    }

    vinfo.bits_per_pixel = 32;
    vinfo.red.length    = 8;
    vinfo.green.length  = 8;
    vinfo.blue.length   = 8;
    vinfo.transp.length = 8;

    vinfo.blue.offset  = 0;
    vinfo.green.offset = 8;
    vinfo.red.offset = 16;
    vinfo.transp.offset = 24;

    vinfo.xres = vinfo.xres_virtual = width;
    vinfo.yres = height;
    vinfo.yres_virtual = 2 * vinfo.yres;

    if ((err = ioctl(console_fd, FBIOPUT_VSCREENINFO, &vinfo)) < 0)
    {
        HI3D_TRACE("Unable to set double buffer mode!, err=0x%x\n", err);
        vinfo.yres_virtual = vinfo.yres;
        if ((err = ioctl(console_fd, FBIOPUT_VSCREENINFO, &vinfo)) < 0)
        {
            HI3D_TRACE("Unable to FBIOPUT_VSCREENINFO!, err=0x%x\n", err);
            return err;
        }

        return err;
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : hi_egl_setup
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

EGLint hi_egl_setup(EGLint x, \
                    EGLint y, \
                    EGLint width, \
                    EGLint height, \
                    EGLint sampling, \
                    EGLint bES20)
{
    HI_S32 s32Ret = HI_SUCCESS;
    EGLConfig configs[10];
    EGLint matchingConfigs;
    EGLint err;
    EGLint configAttribs[] =
    {
        EGL_SAMPLES,                   0,
        EGL_RED_SIZE,                  8,
        EGL_GREEN_SIZE,                8,
        EGL_BLUE_SIZE,                 8,
        EGL_ALPHA_SIZE,                8,
        EGL_DEPTH_SIZE,               16,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE,
    };

    EGLint ctxAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 1,
        EGL_NONE
    };

    configAttribs[1] = sampling;
    if (bES20)
    {
        ctxAttribs[1] = 2;
    }

    if (height > 720)
    {
#ifdef NEED_DEPEND_SDK
        g_enEncFmt = HI_UNF_ENC_FMT_1080P_50;
#endif
    }


#ifdef NEED_DEPEND_SDK
    HI_SYS_Init();
    HIADP_Disp_Init(g_enEncFmt);

    s32Ret = HI_GO_Init();
    if (HI_SUCCESS != s32Ret)
    {
        HI3D_TRACE("HI_GO_Init failure 0x%x\n", s32Ret);
        return HI_FAILURE;
    }
#endif

    /**===========================================================**/
    g_NativeDisplay = (NativeDisplayType)nativeCreateDisplay();
    if (g_NativeDisplay < 0)
    {
        HI3D_TRACE("nativeCreateDisplay failed!\n");
        return -1;
    }

    g_NativeWindow = nativeCreateWindow("egl window", width, height, 0);

    HI_UNF_DISP_Init();
    HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, width, height);
    /**===========================================================**/

#ifdef NEED_DEPEND_SDK
 #ifndef EGL_API_HIGO
    hi_egl_setlayerpara(width, height);
 #endif
#endif

    eglBindAPI(EGL_OPENGL_ES_API);

    /* Get EGLDisplay */
    g_EglDisplay = eglGetDisplay(g_NativeDisplay);

    if (g_EglDisplay == EGL_NO_DISPLAY)
    {
        HI3D_TRACE("g_EglDisplay == EGL_NO_DISPLAY\n");
        return HI_FAILURE;
    }

    if (!eglInitialize(g_EglDisplay, NULL, NULL))
    {
        return HI_FAILURE;
    }

    if (!eglChooseConfig(g_EglDisplay, configAttribs, &configs[0], 10, &matchingConfigs))
    {
        return HI_FAILURE;
    }

    if (matchingConfigs < 1)
    {
        return HI_FAILURE;
    }

    g_EglSurface = eglCreateWindowSurface(g_EglDisplay, configs[0], g_NativeWindow, NULL);
    if (g_EglSurface == EGL_NO_SURFACE)
    {
        HI3D_TRACE("g_EglSurface == EGL_NO_SURFACE\n");
        HI3D_TRACE("eglGeterror =0x%x\n", eglGetError());
        return HI_FAILURE;
    }

    g_EglContext = eglCreateContext(g_EglDisplay, configs[0], NULL, ctxAttribs);
    if (g_EglContext == EGL_NO_CONTEXT)
    {
        return HI_FAILURE;
    }

    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    if (EGL_SUCCESS != (err = eglGetError()))
    {
        HI3D_TRACE("eglGetError = 0x%x\n", err);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/******************************************************************************
func                     : hi_egl_destroy
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 hi_egl_destroy()
{
    HI_S32 s32Loop = 0;

    /** clean double buffer  **/
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    for (s32Loop = 0; s32Loop < 2; s32Loop++)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        hi_egl_swap();
    }

    eglMakeCurrent(g_EglDisplay, NULL, NULL, NULL);

    if (g_EglContext)
    {
        eglDestroyContext(g_EglDisplay, g_EglContext);
    }

    if (g_EglSurface)
    {
        eglDestroySurface(g_EglDisplay, g_EglSurface);
    }

    eglTerminate(g_EglDisplay);

    nativeDestroyWindow(g_NativeWindow);
    nativeDestroyDisplay(g_NativeDisplay);

#ifdef NEED_DEPEND_SDK
    HIADP_Disp_DeInit();
    HI_SYS_DeInit();
    
    if (console_fd > 0)
    {
        close(console_fd);
    }
#endif


    return HI_SUCCESS;
}

/******************************************************************************
func                     : hi_egl_swap
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

HI_S32 hi_egl_swap()
{
    eglSwapBuffers(g_EglDisplay, g_EglSurface);

    return HI_SUCCESS;
}

/******************************************************************************
func                     : hi_egl_getdisp
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

EGLDisplay hi_egl_getdisp()
{
    return g_EglDisplay;
}

/******************************************************************************
func                     : hi_egl_getsurface
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

EGLSurface hi_egl_getsurface()
{
    return g_EglSurface;
}

/******************************************************************************
func                     : hi_egl_makecurrent
description              :
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/

void hi_egl_makecurrent()
{
    eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
}

/******************************************************************************
func                     : hi_hide_fb
description              : 隐藏图形层，当videocube全屏显示的时候就使用视频层显示，
                           这样就要把图形层关闭
param[in]                :
param[in]                :
param[in]                :
retval                   : HI_SUCCESS
retval                   : HI_FAILURE
others 	                 :
******************************************************************************/
HI_VOID hi_hide_fb(HI_BOOL bshow)
{
    if (console_fd > 0)
    {
        if (ioctl(console_fd, FBIOPUT_SHOW_HIFB, &bshow) < 0)
        {
            HI3D_TRACE("Unable to show/hide FB !\n");
        }
    }
}
