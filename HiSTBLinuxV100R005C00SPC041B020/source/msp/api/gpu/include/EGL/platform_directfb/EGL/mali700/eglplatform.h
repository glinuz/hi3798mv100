/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2011-2014 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/**
 * @file eglplatform.h
 * Platform related definitions
 */

#ifndef _EGLPLATFORM_H_
#define _EGLPLATFORM_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#include <KHR/khrplatform.h>
#include <stdint.h>

#if defined(EGL_FBDEV) || defined(EGL_DUMMY) || defined(EGL_DIRECTFB)
#include "mali_fbdev_types.h"
#endif

#ifndef EGLAPI
#define EGLAPI
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY
#endif

#ifndef EGLAPIENTRYP
#define EGLAPIENTRYP EGLAPIENTRY *
#endif

#if defined(EGL_FBDEV) || defined(EGL_DUMMY) || defined(EGL_DIRECTFB)
typedef void * EGLNativeWindowType;

typedef void * EGLNativePixmapType;

#endif

#ifdef EGL_FBDEV
typedef void * EGLNativeDisplayType;
#elif defined(EGL_DUMMY)
typedef dummy_display * EGLNativeDisplayType;
#elif defined(EGL_DIRECTFB)
typedef void * EGLNativeDisplayType;
#else
#error "Window system unknown"
#endif

typedef EGLNativeWindowType NativeWindowType;
typedef EGLNativePixmapType NativePixmapType;
typedef EGLNativeDisplayType NativeDisplayType;

typedef int32_t EGLint;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EGLPLATFORM_H_ */
