/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	    : hi_egl.h
Version		    : Initial Draft
Author		    :
Created		    : 2012/05/14
Description	    :
Function List 	:


History       	:
Date				Author        		Modification
2011/11/23		            		    Created file
******************************************************************************/

#ifndef __HI_EGL_H__
#define __HI_EGL_H__

/*********************************add include here******************************/

#include <EGL/egl.h>
#include "hi_type.h"
#include "hi_3d_config.h"

/*****************************************************************************/

/*****************************************************************************/

#ifdef __cplusplus
 #if __cplusplus

extern "C"
{
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#if 1
 #define NEED_DEPEND_SDK
#endif

/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/

#define GLCheckError() \
    {                                                                \
        GLint err = glGetError();                                    \
        if (err != GL_NO_ERROR)                                      \
        {                                                            \
            fprintf(stderr, "\nfunction: %s, line: %d, err: 0x%x\n", \
                    __FUNCTION__, __LINE__, err);                              \
            return HI_FAILURE;                                       \
        }                                                            \
    }

#define EGLCheckError() \
    {                                                                \
        GLint err = eglGetError();                                   \
        if (err != EGL_SUCCESS)                                      \
        {                                                            \
            fprintf(stderr, "\nfunction: %s, line: %d, err: 0x%x\n", \
                    __FUNCTION__, __LINE__, err);                              \
            return HI_FAILURE;                                       \
        }                                                            \
    }

/******************************* API declaration *****************************/

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

EGLint     hi_egl_setup(EGLint x, \
                        EGLint     y, \
                        EGLint     width, \
                        EGLint     height, \
                        EGLint     sampling, \
                        EGLint     bES20);

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

HI_S32     hi_egl_destroy();

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

HI_S32     hi_egl_swap();

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

EGLDisplay hi_egl_getdisp();

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

EGLSurface hi_egl_getsurface();

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

HI_VOID    hi_egl_makecurrent();

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
HI_VOID    hi_hide_fb(HI_BOOL bshow);

#ifdef __cplusplus

 #if __cplusplus
}

 #endif

#endif    /* __cplusplus */

#endif /* __HI_EGL_H__*/
