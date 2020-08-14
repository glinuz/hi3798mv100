/*=================================================

Open MAX   Component: Video Decoder
Debug definition for openMAX decoder component.
File:    omx_allocator.h
Author:  liminqi 00228308
Date:    16, 03, 2013.

=================================================*/

#ifndef __OMX_DBG_H__
#define __OMX_DBG_H__

#ifdef ANDROID
#include<utils/Log.h>
#else
#define ALOGD				printf
#define ALOGE				printf
#endif

#undef  LOG_TAG
#define LOG_TAG				"HIOMX_VENC"

#define DEBUG                 0
#define DEBUG_WARN            0
#define DEBUG_STREAM          0
#define DEBUG_EFTB        0
// ENABLE PRINT IN LINUX
#define DEBUG_STATE               1

#if  (1 == DEBUG)
#define DEBUG_PRINT(fmt, args...)               ALOGD(fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)                     
#endif

#if (1 == DEBUG_WARN)
#define DEBUG_PRINT_WARN(fmt, args...)          ALOGD(fmt, ##args)
#else
#define DEBUG_PRINT_WARN(fmt, args...)          
#endif

#if (1 == DEBUG_STREAM)
#define DEBUG_PRINT_STREAM(fmt, args...)        ALOGD(fmt, ##args)
#else
#define DEBUG_PRINT_STREAM(fmt, args...)       
#endif

#if (1 == DEBUG_STATE)
#define DEBUG_PRINT_STATE(fmt, args...)         ALOGD(fmt, ##args)
#else
#define DEBUG_PRINT_STATE(fmt, args...)         
#endif

#if (1 == DEBUG_EFTB)
#define DEBUG_PRINT_EFTB(fmt, args...)          ALOGD(fmt, ##args)
#else
#define DEBUG_PRINT_EFTB(fmt, args...)
#endif
#define DEBUG_PRINT_ALWS(fmt, args...)          ALOGD(fmt, ##args)
#define DEBUG_PRINT_ERROR(fmt, args...)         ALOGE(fmt, ##args)


#endif //__OMX_DBG_H__
