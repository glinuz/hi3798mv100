/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keypad.h
 * Description:
 *
 * History:
 * Version   Date              Author    DefectNum       Description
 *******************************************************************************/
#ifndef __KEYLED_H__
#define __KEYLED_H__
#include "hi_type.h"

#define KEY_PRESS             (0x0)           
#define KEY_HOLD              (0x01)          
#define KEY_RELEASE           (0x02)          

#ifdef HI_KEYLED_PT6964_CLOCK_GPIO
#define CLK_PT6964  HI_KEYLED_PT6964_CLOCK_GPIO
#define STB_PT6964  HI_KEYLED_PT6964_STB_GPIO
#define DIN_PT6964  HI_KEYLED_PT6964_DINOUT_GPIO
#define DOUT_PT6964 HI_KEYLED_PT6964_DINOUT_GPIO
#else
#define CLK_PT6964  0x2a
#define STB_PT6964  0x2b
#define DIN_PT6964  0x2c
#define DOUT_PT6964 0x2c
#endif

#define KEY_MACRO_NO (0xff)
#define DISPLAY_REG_NUM     (14)
#define KEY_COL_NUM     10
#define DISPLAY_MODE_NUM 4

#define HI_ERR_KEYLED_NOT_INIT                      (HI_S32)(0x804B0001)
#define HI_ERR_KEYLED_INVALID_PARA                  (HI_S32)(0x804B0002)
#define HI_ERR_KEYLED_NULL_PTR                      (HI_S32)(0x804B0003)
#define HI_ERR_KEYLED_NO_NEW_KEY                    (HI_S32)(0x804B0004)
#define HI_ERR_KEYLED_TIMEOUT                       (HI_S32)(0x804B0005)
#define HI_ERR_KEYLED_FAILED_SETMODE                (HI_S32)(0x804B0006)
#define HI_ERR_KEYLED_FAILED_DISPLAY                (HI_S32)(0x804B0007)

static unsigned int g_s32KEYLEDFd = 0;

#define KEYLED_TRACE_LEVEL 8

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define keyled_trace(level, msg ...) do \
    { \
        if ((level) >= KEYLED_TRACE_LEVEL) \
        {  \
            printf("keyled_trace:%s:%d: ", __FILE__, __LINE__); \
            printf(msg); \
            printf("\n"); \
        } \
    } while (0)
#else
#define keyled_trace(level, msg ...) do \
    { \
    } while (0)
#endif
#define CheckKeyledDeviceFd() \
    do \
    { \
        if (0 >= g_s32KEYLEDFd) \
        { \
            printf("keyled not init\n"); \
            return HI_ERR_KEYLED_NOT_INIT; \
        } \
    } while (0)

#define CheckKeyledDeviceRepeatFd() \
    do \
    { \
        if (0 < g_s32KEYLEDFd) \
        { \
            printf("keyled repeat init\n"); \
            return HI_SUCCESS; \
        } \
    } while (0)

#define CheckKeyledNULLPointer(p) \
    do \
    { \
        if (HI_NULL == (p)) \
        { \
            printf("pointer is null\n"); \
            return HI_ERR_KEYLED_NULL_PTR; \
        } \
    } while (0)


#endif
