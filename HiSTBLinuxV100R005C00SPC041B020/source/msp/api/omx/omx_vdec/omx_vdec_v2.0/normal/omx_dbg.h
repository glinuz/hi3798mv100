/*=================================================

Open MAX   Component: Video Decoder
Debug definition for openMAX decoder component.
File:    omx_dbg.h
Author:  yangyichang 00226912
Date:    16, 03, 2013.

=================================================*/

#ifndef __OMX_DBG_H__
#define __OMX_DBG_H__

#include "OMX_Types.h"

#ifdef ANDROID
#include<utils/Log.h>
#include <cutils/properties.h>
#else
#define ALOGD                      printf
#define ALOGE                      printf
#endif

#undef  LOG_TAG
#define LOG_TAG                    "HISI_OMXVDEC"

#ifdef HI_OMX_TEE_SUPPORT
//#define DEBUG_SAVE_YUV            0
#else
#define DEBUG_SAVE_YUV            1
#endif

#ifdef ANDROID
#define DEBUG_WARN                0
#define DEBUG_STREAM              0
#else
// ENABLE PRINT IN LINUX
#define PRN_ENABLE                0
#define DEBUG_WARN                PRN_ENABLE
#define DEBUG_STREAM              PRN_ENABLE
#endif
#define DEBUG_STATE               1


#ifndef HI_ADVCA_FUNCTION_RELEASE

#if (1 == DEBUG_WARN)
#define DEBUG_PRINT_WARN          ALOGD
#else
#define DEBUG_PRINT_WARN(format...)
#endif

#if (1 == DEBUG_STATE)
#define DEBUG_PRINT_STATE         ALOGD
#else
#define DEBUG_PRINT_STATE(format...)
#endif

#define DEBUG_PRINT_ALWS          ALOGD
#define DEBUG_PRINT_ERROR         ALOGE
#define DEBUG_PRINT_TUNNELED      ALOGD

#ifdef ANDROID
extern OMX_BOOL g_EnableLogcatV;

#define CONDITION(cond)     (__builtin_expect((cond)!=0, 0))

#define OMX_ALOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? ((void)ALOG(LOG_VERBOSE, LOG_TAG, __VA_ARGS__)) \
    : (void)0 )

#define DEBUG_PRINT_TRACE(...) OMX_ALOGD_IF(g_EnableLogcatV, __VA_ARGS__)

#else

#if (1 == DEBUG_STREAM)
#define DEBUG_PRINT_TRACE ALOGD
#else
#define DEBUG_PRINT_TRACE(format...)

#endif

#endif


#else
#define DEBUG_PRINT_WARN(format...)
#define DEBUG_PRINT_TRACE(format...)
#define DEBUG_PRINT_STATE(format...)
#define DEBUG_PRINT_ALWS(format...)
#define DEBUG_PRINT_ERROR(format...)
#define DEBUG_PRINT_TUNNELED(format...)
#endif

#endif //__OMX_DBG_H__
